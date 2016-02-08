/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzlooprs.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9E   1996/10/04
//
// LAST MESSAGE ID: LO00111
//
// HISTORY:
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2007.05.08 DKS
   Added the operation CopyOperationCode.
2003.04.30 DonC
   Added the operation, WL_QC.
2002.09.03 DonC
   Added the operation, GetStrFromAttrByContext.
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2002.06.20 DKS
   Removed warnings using casts.
2001.10.04 PAS
   Fix to change made 2001.10.04.  Check did not take into account a
   name that was a substring of another name.
2001.10.23 DKS
   Added code to put out makefile with target of 80 character line.
   Format generated date as YYYY.MM.DD.
2001.10.04 PAS
   Changed the generation of OBJ references for the make file to handle the
   case that more than one LOD reference the same file.  It was giving a MAK
   error - too many rules - and it was hard to determine the source of the
   problem.  This is generally an error in LOD specification, but there is a
   valid case to do this.  I have the 3 "copies" of a LOD with different root
   limits (since this can't be specified at activate time).  There are many
   derived attributes that I need to use regardless of which LOD is used for
   activation.  Having the same source file (supported by the SaveAs function)
   is very useful.  As the warning that is now given to the user indicates,
   there are potential problems if this 'loophole' is used incorrectly.
2001.07.13 RG
   Added operations: VerifyDir and fnDirectoryIsRequiredD
2001.07.06 RG
   - Added support of relative pathnames
   - Removed operations: fnWriteMakefile_MSVC15 and fnWriteMakefile_IBMC20
2001.03.07 BL
   revise ERD Cross Reference
2001.02.25 BL
   revise ERD Cross Reference
2001.01.02  HH
   exported GetVMLConstChar.
2000.12.19  HH
   void for empty data types.
2000.11.27  DKS Z2000
   Workstation generation of .C files now reports if there is a mismatch
   between a Dialog (.PWD) and its generated resource file.
2000.11.29 HH
   (optionally) generate const zCHAR for non return string parameters.
2000.11.10  SR  Z2000 Size of path variables
   - Modified size of all filename and path variables in whole module
     because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
     been changed to 254 in datamodel.
   - Adjust the size of messagetext variables.
   - Remove nMetaLth and szMetaFileName in function fnWriteZeidonResourcesToRc
     because both are not used.
   - Replace fix size 4 for file extensions with zMAX_EXTENSION_LTH
2000.10.30  RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.25  DKS Z2000
   Writing resource files (fnWriteZeidonResourcesToRC) corrected to use
   Key rather than ZKey for Ctrl ID.  It also handles resources defined
   in sub-controls.
2000.09.27  BL  Z2000
   Modified fnWriteZeidonResourcesToRC: insert DropMetaOI by break
2000.08.08  RG  Z2000
   Removed compiler warnings
2000.07.13  DKS
   Moved Split and Create CtrlBOI_BMP to TZCTLCVT.cpp.
2000.06.05  DC
   Modified fnRecursiveXrefLOD_Entity to handle case where we get a
   relationship name match but not a ZKey match.
2000.05.12  HH
   added zdmCardinalityMinimum.
2000.04.28  DC   Z2000
   Modified LOD_DialogListSub to process LOD Entities recursively to
   correct error where the incorrect entity name was being generated for
   some relationships. Also added attribute LOD_EntityRel.LOD_RelationshipDirection
   to the LOD TZBRLOVO.
2000.01.19  BL   Z2000
   Modified CreateMakefileForAllTargets for remove deselection
2000.01.10  RG   Z2000
   If there is no ResourcePath entry in Zeidon.ini, no \\ before the
   Bitmap Filename.
1999.12.16  DC   Z10
   Modified LOD_DialogListSub to change program error message to a valid
   error message.
1999.09.30  DGC  Z10
   Fixed creating zmake.bat--if the compiler directory had a space the
   compile wouldn't work.  Added quotes (") around the compile command.
1999.09.29  BL   Z10   new menu structure, insert help menu in Object Browser
   update Operation StartZeidonHelp
   The View TaskLPLR or TZCMREPO does not exist in the Object Browser.
   I think, the code works also with the View vSubtask.
1999.09.20  DC   Z10
   Modified LOD_DialogListBuildControl and LOD_DialogListSub to make
   sure a parent entity existed.
1999.08.23  DKS  Z10   QS999
   Modified TransferToEditor to make the editor modal.
1998.07.19  DC   Z10
   Modified LOD_DialogListSub to set relationship information for
   Relationship cross reference.
1999.06.03  DKS  Z10   MSVC
   Build of zeidonop.h has zOPER_EXPORT as part of the prototype.
1999.04.20  DC   Z10   Domains
   Modified zdmCardinalityMaximum domain routine to return zDME_NOT_HANDLED
   to the caller instead of call error.
1999.04.09  DC
   Modified LOD_DialogListSub to add zIGNORE_ERRORS to activate from file
   so we wouldn't get error messages caused by the PPE changes for
   Release 10.
1999.02.24  DGC
   Made fix to makefile generation.  We didn't handle the problem when one
   source file called an operation in another source file but both were in
   the same DLL.
1999.01.07  DGC
   Changed creation of .RC files so that we only update the .RC file if there
   are resources in the dialog.
1997.06.13  GT
   Include Siron Reports (kzsirrep.lib) in makefiles
1997.05.16  FrankH
   in creating the Makefile with ext. Obj-& Lib-Files we execute
   splitpath first to eleminate the Drive and Directory
1997.03.19  GT
   Implemented external OBJs for Makefiles
   Changed OS2 DFB/DEF files for multiple instances of domain operations
1996.12.09: Guenter Trageser: correct number of 0s for CtrlBOI of
                              BMP button
1996.11.14: Guenter Trageser: for win: put BMP and ICO in RC
1996.10.04: Guenter Trageser: make for OS/2
1998.11.02  TMV
   Add an argument zLONG  lInvokingCommand to operation TransferToEditor
   which tells us something about a command to invoke when starting the editor
1998.07.14  DGC
   Removed SourceFileClose().
1998.06.24  DGC
   Changed operation prototype generation so that we no longer generate
   "zPPCHAR" for strings.
1998.03.18  DonC  QS168
   Modified fnWriteZeidonResourcesToRC to eliminate TZPESRCO processing that
   seemed to have no purpose and was causing loss of the TZPESRCO view.
1998.03.15  DonC
   Added operation FindTextInAttribute.
1998.02.18  DonC
   Added operations LOD_DialogListBuildControl and LOD_DialogListSub for
   processing LOD cross reference.
1998.01.15  DonC
   Eliminated CommitMetaOI call for dialog source in
   fnWriteZeidonResourcesToRC operation.
1997.11.07  GT
   For external libs/objs in OS/2: if a path is given, put it in the makefile
1997.06.30  GT
   Use operation names from all source files for export entries
   in DEF file for OS/2 (lost fix from 23.04.97)
1997.06.27  DonC
   Modified CreateMakefileForAllTargets to handle selected entries instead
   of all entries
1997.06.17  DonC
   Modified fnWriteZeidonResourcesToRC to use zREFER view of Dialog instead
   of zSOURCE view so that Dialog could be saved after target build.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzctlopr.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"
#include <ctype.h>

HINSTANCE g_hInstanceDLL = 0;

// #define CreateViewFromViewForTask( pvReturn, vSrc, vTask ) CreateViewTrace( __FILE__ " #" mSTR2( __LINE__ ), pvReturn, vSrc, vTask )
// #define DropView( v ) DropViewTrace( __FILE__ " #" mSTR2( __LINE__ ), v )

// global variables
#define INV_EDT_CMD_DEFAULT 0
#define INV_EDT_CMD_DELETE_OPERATION 1

#define SWL( v, str ) SysWriteLine( v, hFile, str )

typedef zSHORT (POPERATION zPAINTERCALL)( zSHORT, zVIEW, zPVOID, zVIEW, zPCHAR, zLONG );

zOPER_EXPORT zLONG  OPERATION
PainterCall( zSHORT, zVIEW, zPVOID, zVIEW, zPCHAR, zLONG );

static void
fnXrefLOD_CreateRelationship( zVIEW   vMeta,
                              zVIEW   vXfer,
                              zVIEW   vERD,
                              zPCHAR  lpLOD_Name );
static void
fnLOD_DialogListBuildAttrControl( zVIEW  vXfer,
                                  zVIEW  vDialog,
                                  zVIEW  vControl );
static void
fnLOD_DialogListBuildDlgCtrl( zVIEW  vXfer );
static void
fnLOD_DialogListCreateControl( zVIEW vXfer,
                               zVIEW vDialog,
                               zVIEW vControl );
static void
fnLOD_DialogList_AddMeta( zVIEW  vSubtask,
                          zVIEW  vXfer,
                          zVIEW  vERD,
                          zVIEW  vMeta,
                          zPCHAR pchMetaName,
                          zPCHAR pchSuffix );

static zBOOL
fnDirectoryIsRequiredD( zVIEW  vSubtask, zPCHAR pchControlName, zBOOL bCreate );

//./ ADD NAME=IssueToolMsg
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: IssueToolMsg
//
//  PURPOSE:    This operation issues a standard tool message to the
//              user and returns the response from the user to the tool.
//              The standard messages and tool names are kept in the
//              resource file for this module so the messages can be
//              changed globally in all tools without changing any code.
//
//  PARAMETERS: vSubtask- View to a zeidon window
//              nStdMsg - zTOOLMSG_???, The message to issue
//                        (ex. zTOOLMSG_SAVE)
//              nTOOL   - zTOOL_???, The active TOOL (ex. zTOOL_LOD)
//              nOBJECT - zREFER_????_META, The meta object type
//              vObject - An optional view to the object for retrieving
//                        the identifier of the object
//              cpcEntityName - An Entity name for the object identifier
//              cpcAttributeName - The attribute name or value if the view
//                                passed is null
//              szContextName - The Context name for the attribute
//
//  RETURNS:    zRESPONSE_? - How the user responded to the message
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zSHORT OPERATION
IssueToolMsg( zVIEW   vSubtask,
              zSHORT  nStdMsg,
              zSHORT  nTool,
              zSHORT  nObject,
              zVIEW   vObject,
              zCPCHAR cpcEntityName,
              zCPCHAR cpcAttributeName,
              zCPCHAR szContextName )
{
   zCHAR szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR szMsgOut[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR szObject[ 34 ];
   zCHAR szAttribute[ 256];
   zCHAR szTool[ 100 ];

   szAttribute[ 0 ] = 0;

   if ( g_hInstanceDLL == 0 )
   {
      HMODULE hModule;

      if ( GetModuleHandleEx( 0, "tzlodopr.dll", &hModule ) )
      {
         g_hInstanceDLL = (HINSTANCE) hModule;
      }
      else
      {
         return( 0 );
      }
   }

   if ( !vObject )
   {
      if ( cpcAttributeName )
         strcpy_s( szAttribute, sizeof( szAttribute ), cpcAttributeName );
   }
   else
   {
      GetVariableFromAttribute( szAttribute, 0, zTYPE_STRING, sizeof( szAttribute ),
                                vObject, cpcEntityName, cpcAttributeName,
                                szContextName, zACCEPT_NULL_ENTITY );
   }

   szMsg[ 0 ] = 0;

   if ( nObject )
      LoadString( g_hInstanceDLL, nObject, szObject, sizeof( szObject ) );
   else
   if ( cpcEntityName )
      strcpy_s( szObject, sizeof( szObject ), cpcEntityName );
   else
      szObject[ 0 ] = 0;

   LoadString( g_hInstanceDLL, nTool, szTool, sizeof( szTool ) );
   LoadString( g_hInstanceDLL, nStdMsg, szMsg, sizeof( szMsg ) );
   sprintf_s( szMsgOut, sizeof( szMsgOut ), szMsg, szObject, szAttribute );
   switch ( nStdMsg )
   {
      case zTOOLMSG_SAVE:
         return( MessagePrompt( vSubtask, "", szTool, szMsgOut, 0,
                                zBUTTONS_YESNOCANCEL, zRESPONSE_YES,
                                zICON_QUESTION ) );
      case zTOOLMSG_DELETE:
      case zTOOLMSG_REMOVE:
         return( MessagePrompt( vSubtask, "", szTool, szMsgOut, 0,
                                zBUTTONS_YESNO, zRESPONSE_NO,
                                zICON_QUESTION ) );
      case zTOOLMSG_INACTIVE:
         return( MessagePrompt( vSubtask, "", szTool, szMsgOut, 0,
                                zBUTTONS_OK, zRESPONSE_OK,
                                zICON_INFORMATION ) );
   }

   return( MessagePrompt( vSubtask, "", szTool, szMsgOut, 0,
                          zBUTTONS_OK, zRESPONSE_OK, zICON_INFORMATION ) );
}

//./ ADD NAME=IssueToolAbout
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: IssueToolAbout
//
//  PURPOSE:    This operation issues a standard about.
//
//  PARAMETERS: vSubtask- View to a zeidon window
//              sTOOL   - zTOOL_???, The active TOOL ( ex. zTOOL_LOD )
//              szFile   - __FILE__ of caller
//              szDate   - __DATE__ of caller
//              szTime   - __TIME__ of caller
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zSHORT OPERATION
IssueToolAbout( zVIEW   vSubtask,
                zSHORT  nTool,
                zCPCHAR szFile,
                zCPCHAR szDate,
                zCPCHAR szTime )
{
   zVIEW  vAbout;
   zCHAR  szTool[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pch;

   if ( g_hInstanceDLL == 0 )
   {
      HMODULE hModule;

      if ( GetModuleHandleEx( 0, "tzlodopr.dll", &hModule ) )
      {
         g_hInstanceDLL = (HINSTANCE) hModule;
      }
      else
      {
         return( 0 );
      }
   }

   ActivateEmptyObjectInstance( &vAbout, "TZ__PRFO", vSubtask, zSINGLE );
   if ( !vAbout )
      return( 0 );

   CreateEntity( vAbout, "TZ", zPOS_FIRST );
   strcpy_s( szTool, sizeof( szTool ), szFile );
   pch = zstrrchr( szTool, '\\' );
   if ( pch )
      pch++;
   else
      pch = szTool;

   SetAttributeFromString( vAbout, "TZ", "AboutFileName", pch );
   SetAttributeFromString( vAbout, "TZ", "AboutCompDate", szDate );
   SetAttributeFromString( vAbout, "TZ", "AboutCompTime", szTime );

   LoadString( g_hInstanceDLL, nTool, szTool, sizeof( szTool ) );
   SetAttributeFromString( vAbout, "TZ", "AboutToolTitle", szTool );
   SetAttributeFromInteger( vAbout, "TZ", "AboutToolTitleID", nTool );
   SetNameForView( vAbout, "ToolAbout", vSubtask, zLEVEL_TASK);

// OperatorConversationalDialog( vSubtask, szMsg, "TZZMAIND", "ToolAbout" );
// DropView( vAbout );

   return( 0 );
}

//./ ADD NAME=zgGetZeidonToolsDir
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zgGetZeidonToolsDir
//
//  PURPOSE:    This routine returns the dir for the running Zeidon Tool,
//              when you may not have a Subtask view...
//
//  PARAMETERS: pchFullDir - An String returned (should be [ 256 ]) that is
//                          directory path with an ending \.
//              nDirType  - same value as GetApplDirectoryFromView
//                          zAPPL_DIR_LIB      = 1 - DLL Directory
//                          zAPPL_DIR_OBJECT   = 2 - Exe Meta Directory
//                          zAPPL_DIR_LOCAL    = 3 - Local directory
//                          zAPPL_DIR_SHARED   = 4 - Shared directory
//
//  RETURNS:              0 - all ok
//              zCALL_ERROR - can't locate a directory for the running tool
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
zgGetZeidonToolsDir( zVIEW  vSubtask,
                     zPCHAR pchFullDir,
                     zSHORT nDirType )
{
   zVIEW  vT;

   pchFullDir[ 0 ] = 0;
   if ( SfCreateSubtask( &vT, vSubtask, "Zeidon System" ) == zCALL_ERROR )
      return( zCALL_ERROR );

   GetApplDirectoryFromView( pchFullDir, vT, nDirType, zMAX_FILENAME_LTH + 1 );
   SfDropSubtask( vT, 0 );
   if ( pchFullDir[ zstrlen( pchFullDir ) - 1 ] != '\\' )
      strcat_s( pchFullDir, 254, "\\" );

// TraceLineS( "zgGetZeidonToolsDir --> ", pchFullDir );

   return( 0 );
}

//./ ADD NAME=zgSortEntityWithinParent4
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zgSortEntityWithinParent4
//
//  PURPOSE:    This routine will sort an Entity ( keeping its dependents )
//              by any given 4 attributes. ( if the attr's 2- 4 are null
//              strings, the only the first attr is used )
//
//              It is not efficient?, but it tends to work.
//
//  PARAMETERS: bAscDsc  -- Indicator for ascending or descending
//                          ( zASCENDING or zDESCENDING )
//              vIn           -- view that contains the entity to be
//                               sorted AND its' parent
//              pchEntityName  -- Entity to be sorted
//              pchAttribName  -- Name of the attribute for primary sort
//              bAscDsc       -- Indicator for ascending or descending
//                               ( zASCENDING or zDESCENDING )
//              pchAttribName2 -- Name of a second sort attribute or
//                               null string
//              bAscDsc2      -- Indicator for ascending or descending
//              pchAttribName3 -- Name of a second sort attribute or
//                               null string
//              bAscDsc3      -- Indicator for ascending or descending
//              pchAttribName4 -- Name of a second sort attribute or
//                               null string
//              bAscDsc4      -- Indicator for ascending or descending
//
//  RETURNS:        number of entity swaps required to sort
//             -1 - more than 32000 swaps required
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
zgSortEntityWithinParent4( zVIEW  vIn,
                           zPCHAR pchEntityName,
                           zPCHAR pchAttribName,
                           zSHORT bAscDsc,
                           zPCHAR pchAttribName2,
                           zSHORT bAscDsc2,
                           zPCHAR pchAttribName3,
                           zSHORT bAscDsc3,
                           zPCHAR pchAttribName4,
                           zSHORT bAscDsc4 )
{
   zVIEW  vSort1;
   zVIEW  vSort2;
   zSHORT nSwap;
   zSHORT nSwapPrev;
   zSHORT nMoves;
   zLONG  lCompares;
   zSHORT nEntities;
   zSHORT nRC;
   zSHORT nRC2;
   zSHORT nRC3;
   zSHORT nRC4;
   zCHAR  sz[ 100 ];
   zLONG  lLth;
   //char   szVal1[ 256 ];
   //char   szVal2[ 256 ];

   nEntities = 0;
   lCompares = 0;
   nMoves = 0;
   nSwap = 0;
   nSwapPrev = 2;
   CreateViewFromViewForTask( &vSort1, vIn, 0 );
   CreateViewFromViewForTask( &vSort2, vIn, 0 );

   if ( SetCursorFirstEntity( vSort1, pchEntityName, 0 ) > zCURSOR_UNCHANGED )
   {
      SetViewFromView( vSort2, vSort1 );
      for ( ; ; )
      {
         nEntities++;
         if ( SetCursorNextEntity( vSort2, pchEntityName, 0 )
                                                   < zCURSOR_SET )
         {
            if ( nSwap == 0 )
            {
               break;
            }
            else
            {
               SetCursorFirstEntity( vSort1, pchEntityName, 0 );
               SetCursorFirstEntity( vSort2, pchEntityName, 0 );
               SetCursorNextEntity( vSort2, pchEntityName, 0 );
               nEntities = 1;
               nSwapPrev = nSwap;
               nSwap = 0;
            }
         }
#if 0
         nRC = GetStringFromAttribute( szVal1, sizeof( szVal1 ),
                        vSort1, pchEntityName, pchAttribName );
         nRC = GetStringFromAttribute( szVal2, sizeof( szVal2 ),
                        vSort2, pchEntityName, pchAttribName );
#endif
         lCompares++;
// Compare Attr 1
         nRC = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName,
                                            vSort2, pchEntityName, pchAttribName );
         if ( bAscDsc == zDESCENDING )
            nRC = nRC * -1;

// Compare Attr 2
         if ( pchAttribName2[ 0 ] == 0 )
         {
            nRC2 = 0;
         }
         else
         {
            nRC2 = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName2,
                                                vSort2, pchEntityName, pchAttribName2 );
            if ( bAscDsc2 == zDESCENDING )
               nRC2 = nRC2 * -1;
         }
// Compare Attr 3
         if ( pchAttribName3[ 0 ] == 0 )
         {
            nRC3 = 0;
         }
         else
         {
            nRC3 = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName3,
                                                vSort2, pchEntityName, pchAttribName3 );
            if ( bAscDsc3 == zDESCENDING )
               nRC3 = nRC3 * -1;
         }
// Compare Attr 4
         if ( pchAttribName4[ 0 ] == 0 )
         {
            nRC4 = 0;
         }
         else
         {
            nRC4 = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName4,
                                                vSort2, pchEntityName, pchAttribName4 );
            if ( bAscDsc3 == zDESCENDING )
               nRC3 = nRC3 * -1;
         }
// See if swap required
         if ( nRC > 0 ||
              (nRC == 0 && nRC2 > 0) ||
              (nRC == 0 && nRC2 == 0 && nRC3 > 0) ||
              (nRC == 0 && nRC2 == 0 && nRC3 == 0 && nRC > 0) )
         {
            nRC = MoveSubobject( vSort2, pchEntityName, vSort1, pchEntityName, zPOS_AFTER, zREPOS_NONE );
            if ( nRC == zCALL_ERROR )
               return( nRC );

            nMoves++;
            if ( nMoves > 32000 )
               return( -1 );

            nSwap++;
            if ( nSwapPrev > 1 )
            {
               // the next two lines pump v1 up - v2 is done at front of loop
               nRC = SetCursorNextEntity( vSort1, pchEntityName, 0 );
               nRC = SetCursorNextEntity( vSort1, pchEntityName, 0 );
            }
            else
            {
               SetCursorFirstEntity( vSort1, pchEntityName, 0 );
               SetCursorFirstEntity( vSort2, pchEntityName, 0 );
               nEntities = 0;
            }
         }
         else
         {
            nRC = SetCursorNextEntity( vSort1, pchEntityName, 0 );
         }
      }
   }

   DropView( vSort1 );
   DropView( vSort2 );

// sprintf_s( sz, sizeof( sz ),"%4d Entities, %4d Moves, %6ld Compares", nEntities, nMoves, nCompares );
   strcpy_s( sz, sizeof( sz ), pchEntityName );
   strcat_s( sz, sizeof( sz ), " -- " );
   lLth = zstrlen( sz );
   zltoa( nEntities, sz + lLth, sizeof( sz ) - lLth );
   strcat_s( sz, sizeof( sz ), " Entities,  " );
   lLth = zstrlen( sz );
   zltoa( nMoves, sz + lLth, sizeof( sz ) - lLth );
   strcat_s( sz, sizeof( sz ), " Moves,  " );
   lLth = zstrlen( sz );
   zltoa( lCompares, sz + lLth, sizeof( sz ) - lLth );
   strcat_s( sz, sizeof( sz ), " Compares" );
   TraceLineS( "Stats zgSortEntityWithinParent4: ", sz );

   return( nMoves );
}

//./ ADD NAME=zgSortEntityWithinGParent4
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zgSortEntityWithinGParent4
//
//  PURPOSE:    This routine will sort an Entity ( keeping its dependents )
//              based on up to 4 attributes in a dependent Entity
//              ( if the attr's 2- 4 are null strings, the only the first
//              attr is used ).  This is used to handle inverted 1 to many
//              relationships that give rise to a 1 to 1 in the Object.
//              You are "sorting" the parent within the grand-parent of the
//              entity that has the sort attributes.
//
//              It is not efficient?, but it tends to work.
//
//  PARAMETERS: bAscDsc  -- Indicator for ascending or descending
//                          ( zASCENDING or zDESCENDING )
//  PARAMETERS: vIn           -- view that contains the entity to be
//                               sorted AND its' parent
//              pchPEntityName -- Parent Entity Name -- this is "moved"
//              pchEntityName  -- Entity (one per parent) that has the sort attrs
//                               sort attributes
//              pchAttribName  -- Name of the attribute for primary sort
//              bAscDsc       -- Indicator for ascending or descending
//                               ( zASCENDING or zDESCENDING )
//              pchAttribName2 -- Name of a second sort attribute or
//                               null string
//              bAscDsc2      -- Indicator for ascending or descending
//              pchAttribName3 -- Name of a second sort attribute or
//                               null string
//              bAscDsc3      -- Indicator for ascending or descending
//              pchAttribName4 -- Name of a second sort attribute or
//                               null string
//              bAscDsc4      -- Indicator for ascending or descending
//
//  RETURNS:        number of entity swaps required to sort
//             -1 - more than 32000 swaps required
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
zgSortEntityWithinGParent4( zVIEW  vIn,
                            zPCHAR pchPEntityName,
                            zPCHAR pchEntityName,
                            zPCHAR pchAttribName,
                            zSHORT bAscDsc,
                            zPCHAR pchAttribName2,
                            zSHORT bAscDsc2,
                            zPCHAR pchAttribName3,
                            zSHORT bAscDsc3,
                            zPCHAR pchAttribName4,
                            zSHORT bAscDsc4 )
{
   zVIEW  vSort1;
   zVIEW  vSort2;
   zSHORT nSwap;
   zSHORT nSwapPrev;
   zSHORT nMoves;
   zLONG  lCompares;
   zSHORT nEntities;
   zSHORT nRC;
   zSHORT nRC2;
   zSHORT nRC3;
   zSHORT nRC4;
   zCHAR  sz[ 100 ];
   zLONG  lLth;
   //char   szVal1[ 256 ];
   //char   szVal2[ 256 ];

   nEntities = 0;
   lCompares = 0;
   nMoves = 0;
   nSwap = 0;
   nSwapPrev = 2;
   CreateViewFromViewForTask( &vSort1, vIn, 0 );
   CreateViewFromViewForTask( &vSort2, vIn, 0 );

   if ( SetCursorFirstEntity( vSort1, pchPEntityName, 0 ) > zCURSOR_UNCHANGED )
   {
      SetViewFromView( vSort2, vSort1 );
      for ( ; ; )
      {
         nEntities++;
         if ( SetCursorNextEntity( vSort2, pchPEntityName, 0 ) < zCURSOR_SET )
         {
            if ( nSwap == 0 )
            {
               break;
            }
            else
            {
               SetCursorFirstEntity( vSort1, pchPEntityName, 0 );
               SetCursorFirstEntity( vSort2, pchPEntityName, 0 );
               SetCursorNextEntity( vSort2, pchPEntityName, 0 );
               nEntities = 1;
               nSwapPrev = nSwap;
               nSwap = 0;
            }
         }
#if 0
         nRC = GetStringFromAttribute( szVal1, sizeof( szVal1 ),
                        vSort1, pchEntityName, pchAttribName );
         nRC = GetStringFromAttribute( szVal2, sizeof( szVal2 ),
                        vSort2, pchEntityName, pchAttribName );
#endif
         lCompares++;
// Compare Attr 1
         nRC = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName,
                                            vSort2, pchEntityName, pchAttribName );
         if ( bAscDsc == zDESCENDING )
            nRC = nRC * -1;

// Compare Attr 2
         if ( pchAttribName2[ 0 ] == 0 )
         {
            nRC2 = 0;
         }
         else
         {
            nRC2 = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName2,
                                                vSort2, pchEntityName, pchAttribName2 );
            if ( bAscDsc2 == zDESCENDING )
               nRC2 = nRC2 * -1;
         }
// Compare Attr 3
         if ( pchAttribName3[ 0 ] == 0 )
         {
            nRC3 = 0;
         }
         else
         {
            nRC3 = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName3,
                                                vSort2, pchEntityName, pchAttribName3 );
            if ( bAscDsc3 == zDESCENDING )
               nRC3 = nRC3 * -1;
         }
// Compare Attr 4
         if ( pchAttribName4[ 0 ] == 0 )
         {
            nRC4 = 0;
         }
         else
         {
            nRC4 = CompareAttributeToAttribute( vSort1, pchEntityName, pchAttribName4,
                                                vSort2, pchEntityName, pchAttribName4 );
            if ( bAscDsc3 == zDESCENDING )
               nRC3 = nRC3 * -1;
         }
// See if swap required
         if ( nRC > 0 ||
              (nRC == 0 && nRC2 > 0) ||
              (nRC == 0 && nRC2 == 0 && nRC3 > 0) ||
              (nRC == 0 && nRC2 == 0 && nRC3 == 0 && nRC > 0) )
         {
            nRC = MoveSubobject( vSort2, pchPEntityName, vSort1, pchPEntityName, zPOS_AFTER, zREPOS_NONE );
            if ( nRC == zCALL_ERROR )
               return( nRC );
            nMoves++;
            if ( nMoves > 32000 )
               return( -1 );
            nSwap++;
            if ( nSwapPrev > 1 )
            {
               // the next two lines pump v1 up - v2 is done at front of loop
               nRC = SetCursorNextEntity( vSort1, pchPEntityName, 0 );
               nRC = SetCursorNextEntity( vSort1, pchPEntityName, 0 );
            }
            else
            {
               SetCursorFirstEntity( vSort1, pchPEntityName, 0 );
               SetCursorFirstEntity( vSort2, pchPEntityName, 0 );
               nEntities = 0;
            }
         }
         else
         {
            nRC = SetCursorNextEntity( vSort1, pchPEntityName, 0 );
         }
      }
   }

   DropView( vSort1 );
   DropView( vSort2 );

// sprintf_s( sz, sizeof( sz ), "%4d Entities, %4d Moves, %6ld Compares", nEntities, nMoves, lCompares );
   strcpy_s( sz, sizeof( sz ), pchPEntityName );
   strcat_s( sz, sizeof( sz ), "||" );
   strcat_s( sz, sizeof( sz ), pchEntityName );
   strcat_s( sz, sizeof( sz ), " -- " );
   lLth = zstrlen( sz );
   zltoa( nEntities, sz + lLth, sizeof( sz ) - lLth );
   strcat_s( sz, sizeof( sz ), " Entities,  " );
   lLth = zstrlen( sz );
   zltoa( nMoves, sz + lLth, sizeof( sz ) - lLth );
   strcat_s( sz, sizeof( sz ), " Moves,  " );
   lLth = zstrlen( sz );
   zltoa( lCompares, sz + lLth, sizeof( sz ) - lLth );
   strcat_s( sz, sizeof( sz ), " Compares" );
   TraceLineS( "Stats zgSortEntityWithinGParent4: ", sz );

   return( nMoves );
}

//./ ADD NAME=zgSysGetTimestamp
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zgSysGetTimestamp
//
//  PURPOSE:    This routine formats the current system timestamp to
//              MM/DD/YYYY HH:MM:SS
//
//              NOTE: this is 19 characters and you need to have allocated
//                    at least 20.
//
//  PARAMETERS: Returned formated date/time
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zgSysGetTimestamp( zPCHAR pchTimestamp )
{
   zCHAR    szWorkString[ 80 ];

   SysGetDateTime( szWorkString, sizeof( szWorkString ) );
   pchTimestamp[ 0 ]  = szWorkString[ 4 ];   // month
   pchTimestamp[ 1 ]  = szWorkString[ 5 ];
   pchTimestamp[ 2 ]  = '/';
   pchTimestamp[ 3 ]  = szWorkString[ 6 ];   // day
   pchTimestamp[ 4 ]  = szWorkString[ 7 ];
   pchTimestamp[ 5 ]  = '/';
   pchTimestamp[ 6 ]  = szWorkString[ 0 ];   // year
   pchTimestamp[ 7 ]  = szWorkString[ 1 ];
   pchTimestamp[ 8 ]  = szWorkString[ 2 ];
   pchTimestamp[ 9 ]  = szWorkString[ 3 ];
   pchTimestamp[ 10 ] = ' ';
   pchTimestamp[ 11 ] = szWorkString[ 8 ];   // hours
   pchTimestamp[ 12 ] = szWorkString[ 9 ];
   pchTimestamp[ 13 ] = ':';
   pchTimestamp[ 14 ] = szWorkString[ 10 ];  // mins
   pchTimestamp[ 15 ] = szWorkString[ 11 ];
   pchTimestamp[ 16 ] = ':';
   pchTimestamp[ 17 ] = szWorkString[ 10 ];  // secs
   pchTimestamp[ 18 ] = szWorkString[ 11 ];
   pchTimestamp[ 19 ] = 0;

   return( 0 );
}

//./ ADD NAME=zgSysGetTimestampFromFFB
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zgSysGetTimestampFromFFB
//
//  PURPOSE:    This routine formats the last update timestamp
//              of an opened file to
//              MM/DD/YYYY HH:MM:SS
//
//              NOTE: this is 19 characters and you need to have allocated
//                    at least 20.
//
//  PARAMETERS: Returned formated date/time
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
zgSysGetTimestampFromFFB( zPCHAR pchTimestamp,
                          HFILE hFile )
{
   zCHAR    szWorkString[ 80 ];

   pchTimestamp[ 0 ] = 0;

   SysGetFileDateTime( hFile, szWorkString, sizeof( szWorkString ) );
   pchTimestamp[ 0 ]  = szWorkString[ 4 ];   // month
   pchTimestamp[ 1 ]  = szWorkString[ 5 ];
   pchTimestamp[ 2 ]  = '/';
   pchTimestamp[ 3 ]  = szWorkString[ 6 ];   // day
   pchTimestamp[ 4 ]  = szWorkString[ 7 ];
   pchTimestamp[ 5 ]  = '/';
   pchTimestamp[ 6 ]  = szWorkString[ 0 ];   // year
   pchTimestamp[ 7 ]  = szWorkString[ 1 ];
   pchTimestamp[ 8 ]  = szWorkString[ 2 ];
   pchTimestamp[ 9 ]  = szWorkString[ 3 ];
   pchTimestamp[ 10 ] = ' ';
   pchTimestamp[ 11 ] = szWorkString[ 8 ];   // hours
   pchTimestamp[ 12 ] = szWorkString[ 9 ];
   pchTimestamp[ 13 ] = ':';
   pchTimestamp[ 14 ] = szWorkString[ 10 ];  // mins
   pchTimestamp[ 15 ] = szWorkString[ 11 ];
   pchTimestamp[ 16 ] = ':';
   pchTimestamp[ 17 ] = szWorkString[ 10 ];  // secs
   pchTimestamp[ 18 ] = szWorkString[ 11 ];
   pchTimestamp[ 19 ] = 0;

   return( 0 );
}

//./ ADD NAME=SourceFileOpenForRead
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: SourceFileOpenForRead
//
//  PURPOSE:    This routine opens a character, sequential file (a source
//              file) for reading.  It is primarily for VML routines that
//              cannot call SysOpenFile directly.
//
//  PARAMETERS: szFileName - File name. If the name is fully qualified,
//                           that qualified name is used.  If it is not
//                           fully qualified, then SysOpenFile will
//                           append the necessary header.
//
//  RETURNS:    The file number to be used in other operations.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zLONG OPERATION
SourceFileOpenForRead( zVIEW lpTaskView, zPCHAR pchFileName )
{
   zLONG  hFile;

   hFile = SysOpenFile( lpTaskView, pchFileName, COREFILE_READ );
   return( hFile );
}

//./ ADD NAME=SourceFileReadLine
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: SourceFileReadLine
//
//  PURPOSE:    This routine reads a line from a character, sequential
//              file.  It is primarily for VML routines that cannot
//              call SysReadLine directly.
//
//  PARAMETERS: hFile  - The file number from SourceFileOpen...
//              szLine - The string to contain the line being read.
//
//  RETURNS:    1 - Line read successful.
//              0 - End of file reached.
//              zCALL_ERROR - Error in processing.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SourceFileReadLine( zVIEW  lpTaskView,
                    zLONG  hFile,
                    zPCHAR pchLine )
{
   zPCHAR pchReturnLine;
   zSHORT nRC;

   nRC = SysReadLine( lpTaskView, &pchReturnLine, hFile );
   if ( nRC == 1 )
      strcpy_s( pchLine, 256, pchReturnLine ); // this does not seem to be used anywhere ... so setting small limit  dks - 2015.10.30
   else
      pchLine[ 0 ] = 0;

   return( nRC );
}

//./ ADD NAME=ConvertStringToQSName
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: ConvertStringToQSName
//
//  PURPOSE:    This routine converts invalid QSName chars (like " " and
//              "/" to valid QSName chars.
//
//  PARAMETERS: pchString - String to be modified.
//
//  RETURNS:    0 - Always
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ConvertStringToQSName( zPCHAR pchString )
{
   zPCHAR pch;

   // Convert all non-alphanumeric chars to underscores.
   for ( pch = pchString; *pch; pch++ )
   {
      if ( !isdigit( *pch ) && (*pch < 'A' || *pch > 'Z') && (*pch < 'a' || *pch > 'z') )
         *pch = '_';
   }

   return( 0 );
}

//./ ADD NAME=ParseLine4
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: ParseLine4
//
//  PURPOSE:    Parse a character line with up to 4 parameters.
//
//  PARAMETERS: szLine  - Line to be parsed.
//              szParm1 - First returned parm.
//              szParm2 - Second returned parm.
//              szParm3 - Third returned parm.
//              szParm4 - Fourth returned parm.
//
//  RETURNS:    0 - If 3 or less parms processed.
//              szLine ptr to end of 4th parm, if 4 parms processed.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zPCHAR OPERATION
ParseLine4 ( zPCHAR pchLine,
             zPCHAR pchParm1,
             zPCHAR pchParm2,
             zPCHAR pchParm3,
             zPCHAR pchParm4 )
{
   int i, j;

   // Initialize non-zero parms to null.
   if ( pchParm1 != 0 )
      *pchParm1 = 0;
   if ( pchParm2 != 0 )
      *pchParm2 = 0;
   if ( pchParm3 != 0 )
      *pchParm3 = 0;
   if ( pchParm4 != 0 )
      *pchParm4 = 0;

   // Now set each parm that is in the input string.
   for ( i = 0; pchLine[ i ] == ' '; i++ ); // Find first nonblank

   // szParm1
   j = 0;
   while ( pchLine[ i ] != 0 && pchLine[ i ] != ' ' )
      pchParm1[j++] = pchLine[i++];   // Copy each nonblank character
   pchParm1[j] = 0;
   if ( pchLine[ i ] == 0 )
      return( 0 );

   // szParm2
   if ( pchParm2 == 0 )    // Exit if no parm.
      return( 0 );
   while ( pchLine[ i ] == ' ' )
      i++;
   j = 0;
   while ( pchLine[ i ] != 0 && pchLine[ i ] != ' ' )
      pchParm2[j++] = pchLine[i++];   // Copy each nonblank character
   pchParm2[j] = 0;
   if ( pchLine[ i ] == 0 )
      return( 0 );

   // szParm3
   if ( pchParm3 == 0 )    // Exit if no parm.
      return( 0 );
   while ( pchLine[ i ] == ' ' )
      i++;
   j = 0;
   while ( pchLine[ i ] != 0 && pchLine[ i ] != ' ' )
      pchParm3[j++] = pchLine[i++];   // Copy each nonblank character
   pchParm3[j] = 0;
   if ( pchLine[ i ] == 0 )
      return( 0 );

   // szParm4
   if ( pchParm4 == 0 )    // Exit if no parm.
      return( 0 );
   while ( pchLine[ i ] == ' ' )
      i++;
   j = 0;
   while ( pchLine[ i ] != 0 && pchLine[ i ] != ' ' )
      pchParm4[j++] = pchLine[i++];   // Copy each nonblank character
   pchParm4[j] = 0;

   return( pchLine + i );
}

//./ ADD NAME=TransferToEditor
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: TransferToEditor
//
//  PURPOSE:    To correctly set up the current session to call the editor
//              and THEN DOES IT.
//
//  PARAMETERS: vSubtask - Subtask view.
//              vMeta    - Meta view -- LOD, Dialog, Domain group, or
//                         GO group.
//              pchInvokingTool - Two-char code for invoking tool:
//                         "ZO" - LOD
//                         "WD" - Dialog
//              InvokingCommand - code for invoking a command:
//                         0 - Default (show operation)
//                         1 - Delete operation from source
//
//  RETURNS:    0 - Everything OK
//            -16 - Error (this should never happen :) )
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
TransferToEditor( zVIEW  vSubtask,
                  zVIEW  vMeta,
                  zPCHAR pchInvokingTool,
                  zLONG  lInvokingCommand )
{
   zVIEW  vProfileXFER;
   zVIEW  vTaskLPLR;
   zVIEW  vEdWrk;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR lpchSourceFileEntityName;
   zCHAR  szTemp[ MAX_PATH ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_TASK );

   if ( zstrcmp( pchInvokingTool, "GO" ) == 0 )
      lpchSourceFileEntityName = "GlobalOperationGroup";
   else
   if ( zstrcmp( pchInvokingTool, "DM" ) == 0 )
      lpchSourceFileEntityName = "DomainGroup";
   else
      lpchSourceFileEntityName = "SourceFile";

   // Get the source dir from the lplr view.
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   SysAppendcDirSep( szFileName );

   // Get the subdir from the source file entity.  If GetString... returns < 0
   // then the subdir attribute is null, so nothing was retrieved.  If subdir
   // is set, then we need to add a '\'.
   if ( GetStringFromAttribute( szFileName + zstrlen( szFileName ), sizeof( szFileName ) - zstrlen( szFileName ),
                                vMeta, lpchSourceFileEntityName, "Subdirectory" ) >= 0 )
   {
      SysAppendcDirSep( szFileName );
   }

   // Get the file name and extension.
   GetStringFromAttribute( szFileName + zstrlen( szFileName ), sizeof( szFileName ) - zstrlen( szFileName ), vMeta, lpchSourceFileEntityName, "Name" );
   strcat_s( szFileName, sizeof( szFileName ), "." );
   GetStringFromAttribute( szFileName + zstrlen( szFileName ), sizeof( szFileName ) - zstrlen( szFileName ), vMeta, lpchSourceFileEntityName, "Extension" );

   // If we found the editor work view then check to see if we are already editing the file.
   if ( vEdWrk )
   {
      // We're about to change a cursor so create temp view.
      CreateViewFromViewForTask( &vEdWrk, vEdWrk, 0 );

      if ( SetCursorFirstEntityByString( vEdWrk, "Buffer", "FileName", szFileName, 0 ) >= zCURSOR_SET )
      {
         zVIEW vEditorSubtask;

         GetIntegerFromAttribute( (zPLONG) &vEditorSubtask, vEdWrk, "Buffer", "vSubtask" );
         ActivateWindow( vEditorSubtask );

         // We don't need to do anything more so ...
         DropView( vEdWrk );
         return( 0 );
      }

      DropView( vEdWrk );
   }

   SetAttributeFromString( vProfileXFER, "ED", "InvokingTool", pchInvokingTool );

   // Tell the editor what to do ... Delete or Show an operation
   if ( lInvokingCommand == INV_EDT_CMD_DELETE_OPERATION )
   {
      SetAttributeFromString(vProfileXFER, "ED", "InvokeEditorCommand", "D" );
   }
   else
   {
      SetAttributeFromString( vProfileXFER, "ED", "InvokeEditorCommand", "S" );
   }

   // Pass views to editor.
   SetNameForView( vMeta, "METAOPERATIONS", vSubtask, zLEVEL_TASK );

   SysReadZeidonIni( -1, "[Workstation]", "Editor", szTemp, sizeof( szTemp ) );
   if ( zstrcmpi( szTemp, "codemax" ) == 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZEDVMLD", "TZEDVMLD" );
   else
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZEDFRMD", "TZEDFRMD" );

   return( 0 );
} // TransferToEditor

//./ ADD NAME=zDeleteFile
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zDeleteFile
//
//  PURPOSE:    This routine simply deletes a file based on the fully qualified
//              file name.
//
//  PARAMETERS: szFileName - The fully qualified file name.
//
//  RETURNS:    0  - File delete successfully.
//              -1 - Error on delete.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zDeleteFile( zPCHAR szFileName )
{
   zLONG lRC;
   zVIEW lpTaskView = GetDefaultViewForActiveTask( );

   lRC = SysOpenFile( lpTaskView, szFileName, COREFILE_DELETE );

   if ( lRC < 0 )
      return( -1 );
   else
      return( 0 );
}

//./ ADD NAME=TranslateStringToUpperCase
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: TranslateStringToUpperCase
//
//  PURPOSE:    This routine translates a string to all upper case.
//
//  PARAMETERS: pchStringIn - the string to translate.
//
//  RETURNS:    0  - File delete successfully.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
TranslateStringToUpperCase( zPCHAR pchStringIn )
{

   SysTranslateString( pchStringIn, 'U' );

   return( 0 );
}

//./ ADD NAME=GetStrFromAttrByContext
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: GetStrFromAttrByContext
//
//  PURPOSE:    This routine retrieves a string from an attribute
//              using either the Context passed or the default Context.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT  /* GLOBAL */  OPERATION
GetStrFromAttrByContext( zPCHAR  pchValue,
                         zLONG   lLth,
                         zVIEW   vView,
                         zPCHAR  pchEntity,
                         zPCHAR  pchAttribute,
                         zPCHAR  pchContext )
{
  zSHORT nRC;

  // If the Context value is null, use the default Context.
  if ( pchContext == 0 || *pchContext == 0 )
     nRC = GetVariableFromAttribute( pchValue, NULL,
                                     zTYPE_STRING, lLth - 1,
                                     vView, pchEntity, pchAttribute,
                                     0, zUSE_DEFAULT_CONTEXT );
  else
     nRC = GetVariableFromAttribute( pchValue, NULL,
                                     zTYPE_STRING, lLth - 1,
                                     vView, pchEntity, pchAttribute,
                                     pchContext, 0 );
  return( nRC );
}

//./ ADD NAME=fnPainterCall
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnPainterCall
//
//  PARAMETERS:
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG  OPERATION
fnPainterCall( zSHORT nMsg,
               zVIEW  vSubtask,
               zVIEW  vDialog,
               zPCHAR pchParm,
               zLONG  lMaxLth )
{
   zPAINTERCALL  fpPainterCall;
   zVIEW         vTZPNTROO;
   zVIEW         vKZCALLBK;
   zPVOID        pTZCToolbar;
   zULONG        ulBlobMax;

   // Get the view to the painter control object
   if ( vSubtask &&
        GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK ) == zLEVEL_TASK )
   {
      ulBlobMax = sizeof( zLONG );
      GetBlobFromAttribute( &pTZCToolbar, &ulBlobMax, vTZPNTROO, "Palette", "Pointer" );
   }
   else
   {
      vTZPNTROO = 0;
      pTZCToolbar = 0;
   }

   if ( vSubtask &&
        GetViewByName( &vKZCALLBK, "KZCALLBK", vSubtask, zLEVEL_TASK ) > 0 )
   {
      ulBlobMax = sizeof( long );
      GetBlobFromAttribute( &fpPainterCall, &ulBlobMax, vKZCALLBK, "Root", "Painter" );
      return( (*fpPainterCall)( nMsg, vTZPNTROO, pTZCToolbar, vDialog, pchParm, lMaxLth ) );
   }
   else
   {
#ifdef __WIN32__
      typedef zSHORT (POPERATION PFN_PAINTERCALL) ( zSHORT, zVIEW, zPVOID, zVIEW, zPCHAR, zLONG );

      static PFN_PAINTERCALL pfnPainterCall = 0;

      // Load PainterCall().  Note that we'll just let Core close the library when all's said and done.
      if ( pfnPainterCall == 0 )
      {
         LPLIBRARY hLibrary = SysLoadLibrary( vSubtask, "TZCTL" );

         if ( hLibrary )
            pfnPainterCall = (PFN_PAINTERCALL) SysGetProc( hLibrary, "PainterCall" );
      }

      if ( pfnPainterCall )
      {
         return( (*pfnPainterCall)( nMsg, vTZPNTROO, pTZCToolbar, vDialog, pchParm, lMaxLth ) );
      }
      else
         return( 0 );
#else
      return( PainterCall( nMsg, vTZPNTROO, pTZCToolbar, vDialog, pchParm, lMaxLth ) );
#endif
   }
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: DetermineDataType
// PURPOSE:   Copies the full data type to lpchReturnType.
// Arguments:
// Returns:
//       The length of the copied data type  e.g. if "zSHORT" is copied to
//       lpchReturnType then the function returns 6.
/////////////////////////////////////////////////////////////////////////////
zSHORT LOCALOPER
DetermineDataType( zVIEW  lpTaskView,
                   zPCHAR lpchReturnType,
                   zPCHAR pchDataTypeCode,
                   zBOOL  bUnsigned,
                   zBOOL  bPointer )
{
   zSHORT nReturnLth = 0;

   lpchReturnType[ nReturnLth++ ] = 'z';

   // If the datatype is a pointer add a 'P' to the return type UNLESS the
   // datatype is also a string type.
   if ( bPointer &&
        (*pchDataTypeCode != 'A' &&
         *pchDataTypeCode != 'E' &&
         *pchDataTypeCode != 'S') )
   {
      lpchReturnType[ nReturnLth++ ] = 'P';
   }

   if ( bUnsigned )
      lpchReturnType[ nReturnLth++ ] = 'U';

   switch ( *pchDataTypeCode )
   {
      case 'A':
      case 'E':
      case 'S':
         if ( bPointer == FALSE && GetVMLConstChar( ) )
         {
            strcpy_s( lpchReturnType + nReturnLth, 16, "CPCHAR" );
            nReturnLth += 6;
         }
         else
         {
            strcpy_s( lpchReturnType + nReturnLth, 16, "PCHAR" );
            nReturnLth += 5;
         }
         break;

      case 'Y':
         strcpy_s( lpchReturnType + nReturnLth, 16, "CHAR" );
         nReturnLth += 4;
         break;

      case 'L':
         strcpy_s( lpchReturnType + nReturnLth, 16, "LONG" );
         nReturnLth += 4;
         break;

      case zTYPE_DECIMAL:
         strcpy_s( lpchReturnType + nReturnLth, 16, "DECIMAL" );
         nReturnLth += 7;
         break;

      case 'V':
         strcpy_s( lpchReturnType + nReturnLth, 16, "VIEW" );
         nReturnLth += 4;
         break;

      // empty data type is assumed to be void
      case 'O':
      case 0:
         strcpy_s( lpchReturnType + nReturnLth, 16, "VOID" );
         nReturnLth += 4;
         break;

      // If datatype is not defined, assume it's SHORT.
      case 'N':
      default:
         strcpy_s( lpchReturnType + nReturnLth, 16, "SHORT" );
         nReturnLth += 5;
         break;

   } // switch ( *pchDataTypeCode )...

   return( nReturnLth );

} // DetermineDataType

// #pragma warning (disable: 4996) // disable strcpy warning

//./ ADD NAME=BuildOperationPrototype
// Source Module=tzlooprs.c
//////////////////////////////////////////////////////////////////////////////
//
// OPERATION: BuildOperationPrototype
//
// PURPOSE:   Build a prototype for the current Operation in vOperation.
//
//            Note: The prototype DOES NOT have a trailing semi-colon. It will
//            be needed for header prototypes.
//
// Arguments:
//
//    vOperation - View to meta containing the operation.
//    pchReturnPrototype - Pointer to string buffer that will contain the
//          built operation prototype.
//    cOperType  - Single character specifying the operation type (e.g. Global,
//          Dialog, etc).
//
// Returns:
//       The total number of characters in the prototype (not including the
//       null-terminator).
//
//////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
BuildOperationPrototype( zVIEW  vOperation,
                         zPCHAR pchSourceFileEntity,
                         zPCHAR pchMetaName,
                         zPCHAR pchReturnPrototype,
                         zLONG  lMaxLth,
                         zCHAR  cOperType )
{
   zPCHAR pchEndPtr = pchReturnPrototype;
   zPCHAR pchDataTypeCode;
   zPCHAR pchOperationName;
   zSHORT nParmStartColumn;
   zBOOL  bFirstParm;
   zSHORT nCount;
   zSHORT nRC;

   // pchEndPtr ALWAYS points to the first character AFTER the end of the
   // current operations string.  i.e. after the call to DetermineDataType,
   // pchEndPtr points to the null terminator.

   strcpy_s( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), "zOPER_EXPORT " );
   pchEndPtr += sizeof( "zOPER_EXPORT " ) - 1; // 13;

   // Set the correct return type string.
   GetAddrForAttribute( &pchDataTypeCode, vOperation, "Operation", "ReturnDataType" );
   pchEndPtr += DetermineDataType( vOperation, pchEndPtr, pchDataTypeCode, FALSE, FALSE );

   // Begin building the prototype.

   GetAddrForAttribute( &pchOperationName, vOperation, "Operation", "Name" );
   pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), " OPERATION\n" );

   // Calculate the offset from the beginning of the line to the
   // first parameter character.  Don't forget to take into
   // account the "( " after the operation name.
   nParmStartColumn = (zSHORT) zstrlen( pchOperationName ) + 2;

   // If the operation is NOT a global op and if source is in VML, then we must
   // add a prefix to the operation name, which will be "o", followed by the
   // LOD name, followed by an "_".
   // We determine the type of operation source by examining the LanguageType
   // attribute of the SourceFile entity.  The only problem is that for a
   // global operation, the SourceFile entity is named GlobalOperationGroup
   // in the LOD.  Thus the correct name was determined in the calling
   // program and is passed to this routine.
   if ( cOperType != 'G' )
   {
      zCHAR szLanguageType[ 5 ];

      GetStringFromAttribute( szLanguageType, sizeof( szLanguageType ), vOperation, pchSourceFileEntity, "LanguageType" );
      if ( szLanguageType[ 0 ] == 'V' )
      {
         zPCHAR pchSourceFileName;

         pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), "o" );

         GetAddrForAttribute( &pchSourceFileName, vOperation, "LOD", "Name" );
         pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), pchSourceFileName );
         pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), "_" );

         // The operation name just got longer, so add the extra chars to
         // nParmStartColumn (don't forget about "o" and "_").
         nParmStartColumn += (zSHORT) zstrlen( pchSourceFileName ) + 2;
      }
   }

   pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), pchOperationName );
   pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), "( " );

   // Now add each parameter entry.  Keep track of the number of parms.
   bFirstParm = TRUE;
   for ( nRC = SetCursorFirstEntity( vOperation, "Parameter", 0 ), nCount = 0;
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vOperation, "Parameter", 0 ), nCount++ )
   {
      zPCHAR pchPointerFlag;
      zPCHAR pchUnsignedFlag;
      zPCHAR pch;
      zPCHAR pchParmName;
      zCHAR  szShortDesc[ 200 ];

      if ( bFirstParm == FALSE )
      {
         zSHORT k;

         // This is not the first parm in the parm list, so add newline and spaces to indent the parm.
         pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), ",\n" );
         for ( k = 0; k < nParmStartColumn; k++ )
            *pchEndPtr++ = ' ';

         *pchEndPtr = 0;
      }
      else
         bFirstParm = FALSE;

      // The parm name will start at pchEndPtr.  Save this value so that we
      // can reference the name later.
      pchParmName = pchEndPtr;

      // Set up the data type field.  *pchPointerFlag will be 'Y' if the
      // parameter is a pointer to the data type.  *pchUnsignedFlag will by
      // 'Y' if the data type is unsigned.
      GetAddrForAttribute( &pchDataTypeCode, vOperation, "Parameter", "DataType" );
      GetAddrForAttribute( &pchPointerFlag, vOperation, "Parameter", "PFlag" );
      GetAddrForAttribute( &pchUnsignedFlag, vOperation, "Parameter", "UFlag" );
      pchEndPtr += DetermineDataType( vOperation, pchEndPtr, pchDataTypeCode,
                                      (zBOOL) (*pchUnsignedFlag == 'Y'),
                                      (zBOOL) (*pchPointerFlag  == 'Y') );

      //
      // Set up the parameter name by creating one from the short description.
      //

      // Get short desc.
      GetStringFromAttribute( szShortDesc, sizeof( szShortDesc ), vOperation, "Parameter", "ShortDesc" );

      // Add space to prototype.
      *pchEndPtr++ = ' ';

      // Add the first letter from the parm's data type code.
      switch ( *pchDataTypeCode )
      {
         case 'S':
         case 's':
            *pchEndPtr++ = 'p';
            *pchEndPtr++ = 'c';
            *pchEndPtr++ = 'h';
            break;

         default:
            *pchEndPtr++ = tolower( *pchDataTypeCode );
      }

      // Copy characters from short desc to prototype.  Skip spaces.  We
      // perform the check using pchParmName to make sure that the name of
      // the parm doesn't get over 30 chars.
      for ( pch = szShortDesc;
            pch && *pch && (pchEndPtr - pchParmName < 30);
            pch++ )
      {
         // Look for a valid character.
         if ( (ztoupper( *pch ) >= 'A' && ztoupper( *pch ) <= 'Z') ||
              (*pch >= '0' && *pch <= '9') || (*pch == '_') )
         {
            // Add the valid zCHAR to the parameter name.
            *pchEndPtr++ = *pch;
         }
      }

      // To ensure that each parm has a unique name, tack on the parm count.
      zltoa( nCount, pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype) );
      pchEndPtr = pchEndPtr + zstrlen( pchEndPtr );

   } // for each parameter.

   // If bFirstParm is TRUE, then there are no parms for the operations, so
   // set the parameter list to 'void'.
   if ( bFirstParm )
   {
      pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), " void" );
   }

   // Now add closing paren.
   pchEndPtr = zstrcpyp( pchEndPtr, lMaxLth - (pchEndPtr - pchReturnPrototype), " )" );

   return( (zSHORT) (pchEndPtr - pchReturnPrototype) );

} // BuildOperationPrototype

//./ ADD NAME=MergeGlobalPrototypes
// Source Module=tzlooprs.c
//////////////////////////////////////////////////////////////////////////////
//
// OPERATION: MergeGlobalPrototypes
//
// PURPOSE:   Build prototypes for each operation in a source meta and
//            merge them with existing prototypes in the common header file:
//               ZEIDONOP.H
//
// ARGUMENTS:
//
//       vMeta - View to object containing the operations.
//       pchMetaName - String that names the meta.
//       pchRootName - Name of root entity for vMeta.
//       vTaskLPLR - View to LPLR.  May be 0.
//
// RETURNS:
//
//       Always 0.
//
//////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
MergeGlobalPrototypes( zVIEW  vMeta,
                       zPCHAR pchMetaName,
                       zPCHAR pchRootName,
                       zVIEW  vTaskLPLR )
{
   zPCHAR   pchLine;
   zCHAR    szName[ 33 ];
   zPCHAR   pchPrototypeBuffer;
   zLONG    hPrototype;
   zLONG    lfNewFile;
   zLONG    lfOldFile;
   zBOOL    bFirstPrototype;
   zCHAR    szOld[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szNew[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nRC;

   // Determine the file names for the original lplr.H file and for the
   // temporary work file that it is copied to.  The directory path for
   // both is gotten from the LPLR object.  We then concatenate the LPLR
   // name and the appropriate suffix.

   if ( vTaskLPLR == 0 )
      GetViewByName( &vTaskLPLR, "TaskLPLR", vMeta, zLEVEL_TASK );

   GetStringFromAttribute( szNew, sizeof( szNew ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szOld, sizeof( szOld ), szNew );
   strcat_s( szOld, sizeof( szOld ), "\\ZEIDONOP" );     // add back slash and name

   strcpy_s( szNew, sizeof( szNew ), szOld );
   strcat_s( szOld, sizeof( szOld ), ".H" );
   strcat_s( szNew, sizeof( szNew ), ".$$$" );

   lfNewFile = SysOpenFile( vMeta, szNew, COREFILE_WRITE );
   lfOldFile = SysOpenFile( vMeta, szOld, COREFILE_READ );

   // If the Old file exists, then we need to copy prototypes from the old
   // header file to the new header file.
   if ( lfOldFile >= 0 )
   {
      zSHORT nStartLineLth;
      zCHAR  szStartLine[ 50 ];

      strcpy_s( szStartLine, sizeof( szStartLine ), "//start " );
      strcat_s( szStartLine, sizeof( szStartLine ), pchMetaName );
      strcat_s( szStartLine, sizeof( szStartLine ), " " );
      nStartLineLth = (zSHORT) zstrlen( szStartLine );

      // Read each line of the old header file looking for the line that
      // indicates the start of the prototype block for the meta we are
      // merging.  Once (and if) it's found, then skip over all the
      // prototypes in that block.
      while ( TRUE )
      {
         // Read a line from old header ... if eof then done.
         if ( SysReadLine( vMeta, (zCOREMEM) &pchLine, lfOldFile ) <= 0 )
            break;

         // Look for the begining of the start block.
         if ( pchLine[ 0 ] == '/' &&
              zstrncmp( pchLine, szStartLine, nStartLineLth ) == 0 )
         {
            // We found the begining of the start block, so skip over it until
            // we find the 'end' line.
            while ( TRUE )
            {
               SysReadLine( vMeta, (zCOREMEM) &pchLine, lfOldFile );
               if ( pchLine == 0 )
                  break;

               if ( pchLine[ 0 ] == '/' && zstrcmp( pchLine, "//end" ) == 0 )
                  break;
            }

            // stop looping
            break;
         }
         else
         {
            // We haven't hit current source section yet, so
            // copy line from old to new header.
            SysWriteLine( vMeta, lfNewFile, pchLine );
         }
      } // while ( TRUE )...

   } // if ( lfOldFile >= 0 )...

   // Allocate a large buffer for prototype generation.
   hPrototype = SysAllocMemory( (zCOREMEM) &pchPrototypeBuffer, 1000, 0, zCOREMEM_ALLOC, 0 );

   // Now loop through all the operations and merge the prototype into the
   // global header file.  We need to skip local operations.
   bFirstPrototype = TRUE;
   for ( nRC = SetCursorFirstEntity( vMeta, "Operation", pchRootName );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMeta, "Operation", pchRootName ) )
   {
      zPCHAR pchOperType;
      zSHORT nLth;

      // Skip operations that are local in scope or are Zeidon operations.
      GetAddrForAttribute( &pchOperType, vMeta, "Operation", "Type" );
      if ( *pchOperType == 'L' ||
           *pchOperType == 'M' ||
           *pchOperType == 'K' ||
           *pchOperType == 'Z' ||
           *pchOperType == 'A' )
      {
         continue;
      }

      if ( bFirstPrototype )
      {
         zCHAR  szStartLine[ 80 ];
         zCHAR  szDateTime[ 40 ];
         zCHAR  szFormat[ 40 ];

         SysGetDateTime( szDateTime, sizeof( szDateTime ) );
         UfFormatDateTime( szFormat, sizeof( szFormat ), szDateTime, "YYYY.MM.DD HH:MI:SS" );
         strcpy_s( szStartLine, sizeof( szStartLine ), "//start " );
         strcat_s( szStartLine, sizeof( szStartLine ), pchMetaName );
         strcat_s( szStartLine, sizeof( szStartLine ), " " );
         strcat_s( szStartLine, sizeof( szStartLine ), szFormat );
         SysWriteLine( vMeta, lfNewFile, szStartLine );

         bFirstPrototype = FALSE;
      }

      // Set the correct name of the SourceFile entity, which is named
      // GlobalOperationGroup for global operations, but SourceFile for LOD operations.
      if ( pchRootName[ 0 ] == 'L' )
         strcpy_s( szName, sizeof( szName ), "SourceFile" );
      else
         strcpy_s( szName, sizeof( szName ), "GlobalOperationGroup" );

      // Build the operation prototype.  We need to tack on the ending semi-colon.
      nLth = BuildOperationPrototype( vMeta, szName, pchMetaName, pchPrototypeBuffer, 1000, *pchOperType );
      pchPrototypeBuffer[ nLth++ ] = ';';
      pchPrototypeBuffer[ nLth++ ] = '\n';
      pchPrototypeBuffer[ nLth ]   = 0;

      SysWriteLine( vMeta, lfNewFile, pchPrototypeBuffer );
   }

   // Free the memory allocated for the prototype buffer.
   SysFreeMemory( hPrototype );

   // If we wrote any prototypes to the new header file, then we need to
   // write a '//end' line to logically terminate the prototype block.
   if ( bFirstPrototype == FALSE )
      SysWriteLine( vMeta, lfNewFile, "//end" );

   // Now copy any other prototypes left in the old header file to the new
   // header file.
   if ( lfOldFile >= 0 )
   {
      while ( SysReadLine( vMeta, (zCOREMEM) &pchLine, lfOldFile ) > 0 )
         SysWriteLine( vMeta, lfNewFile, pchLine );

      SysCloseFile( vMeta, lfOldFile, 0 );
      lfOldFile = SysOpenFile( vMeta, szOld, COREFILE_DELETE );
   }

   // Make the temporary new lplr header file into the old one.
   // DM - This should be done with a system rename call. dos
   // rename is problematic due to problems of short vs long
   // pointers. So, the following recreates a rename and delete
   // using our own system delete and line by line copies.
   // This should be changed when we change our pointer types
   // or when we get a system rename.

   // Reopen old file for rewrite.
   lfOldFile = SysOpenFile( vMeta, szOld, COREFILE_CREATE );

   // Close new header file and reopen it for reading.
   SysCloseFile ( vMeta, lfNewFile, 0 );
   lfNewFile = SysOpenFile( vMeta, szNew, COREFILE_READ );

   while ( SysReadLine( vMeta, (zCOREMEM) &pchLine, lfNewFile ) > 0 )
   {
      SysWriteLine( vMeta, lfOldFile, pchLine );
   }

   // Close the files and destroy the temporary new lplr header file.
   SysCloseFile( vMeta, lfOldFile, 0 );
   SysCloseFile( vMeta, lfNewFile, 0 );
   SysOpenFile( vMeta, szNew, COREFILE_DELETE );

   return( 0 );

} // MergeGlobalPrototypes

//./ ADD NAME=fnWriteDFB_MSVC15
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnWriteDFB_MSVC15
//
//  PURPOSE:    Write a DFB for Windows (MS Visual C++ 1.50)
//
//  PARAMETERS: hFile - File Handle for DFB file
//              pchTargetName - Name of the DLL
//
//  RETURNS:    -
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zVOID
fnWriteDFB_MSVC15( zVIEW  lpTaskView,
                   zLONG  hFile,
                   zPCHAR pchTargetName,
                   zPCHAR pchLine )
{
  SysWriteLine( lpTaskView, hFile, "LIBRARY" );
  sprintf_s( pchLine, zLINESIZE, "DESCRIPTION   '%s'", pchTargetName );
  SysWriteLine( lpTaskView, hFile, pchLine );
  SysWriteLine( lpTaskView, hFile, "EXETYPE       WINDOWS" );
  SysWriteLine( lpTaskView, hFile, "HEAPSIZE      1024" );
  SysWriteLine( lpTaskView, hFile, "CODE          LOADONCALL  DISCARDABLE  MOVEABLE" );
  SysWriteLine( lpTaskView, hFile, "DATA          PRELOAD     MOVEABLE     SINGLE" );
  SysWriteLine( lpTaskView, hFile, "SEGMENTS" );
  SysWriteLine( lpTaskView, hFile, "_TEXT      PRELOAD" );
  SysWriteLine( lpTaskView, hFile, "WEP_TEXT   PRELOAD  FIXED" );
  SysWriteLine( lpTaskView, hFile, "INIT_TEXT  PRELOAD  DISCARDABLE  MOVEABLE" );
  SysWriteLine( lpTaskView, hFile, "" );
  SysWriteLine( lpTaskView, hFile, "IMPORTS" );
  SysWriteLine( lpTaskView, hFile, "" );
  SysWriteLine( lpTaskView, hFile, "EXPORTS" );
} // fnWriteDFB_MSVC15

zVOID LOCALOPER
fnWriteDFB_MSVC50( zVIEW  lpTaskView,
                   zLONG  hFile,
                   zPCHAR pchTargetName,
                   zPCHAR pchLine )
{
  SysWriteLine( lpTaskView, hFile, "LIBRARY" );
  sprintf_s( pchLine, zLINESIZE, "DESCRIPTION   '%s'", pchTargetName );
  SysWriteLine( lpTaskView, hFile, pchLine );
  SysWriteLine( lpTaskView, hFile, "CODE          PRELOAD DISCARDABLE  MOVEABLE" );
  SysWriteLine( lpTaskView, hFile, "DATA          PRELOAD SINGLE" );
  SysWriteLine( lpTaskView, hFile, "" );
  SysWriteLine( lpTaskView, hFile, "EXPORTS" );
} // fnWriteDFB_MSVC15

//./ ADD NAME=fnWriteDFB_IBMC20
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnWriteDFB_IBMC20
//
//  PURPOSE:    Write a DFB for OS/2 (IBM C++ 2.0)
//
//  PARAMETERS: hFile - File Handle for DFB file
//              pchTargetName - Name of the DLL
//
//  RETURNS:    -
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zVOID
fnWriteDFB_IBMC20( zVIEW  lpTaskView,
                   zLONG  hFile,
                   zPCHAR pchTargetName,
                   zPCHAR pchLine )
{
  // We create the OS/2 DLLs with INITINSTANCE and MULTIPLE data segments.
  // There is no difference for normal Zeidon operations, cause this
  // operations do not use a data segment of a DLL. But in special cases,
  // we need this behaviour:
  // We wrote our own Domain operations and used some global variables in
  // the C module, which are initialized. When running multiple instances
  // of the Zeidon application, we need this settings!
  // 19.03.97 GT
  sprintf_s( pchLine, zLINESIZE, "LIBRARY       %s   INITINSTANCE TERMINSTANCE", pchTargetName );
  SysWriteLine( lpTaskView, hFile, pchLine );
  sprintf_s( pchLine, zLINESIZE, "DESCRIPTION   '%s - Generated by Zeidon'", pchTargetName );
  SysWriteLine( lpTaskView, hFile, pchLine );
  SysWriteLine( lpTaskView, hFile, "" );
  SysWriteLine( lpTaskView, hFile, "PROTMODE" );
  SysWriteLine( lpTaskView, hFile, "CODE          LOADONCALL" );
  SysWriteLine( lpTaskView, hFile, "DATA          LOADONCALL MULTIPLE NONSHARED" );
  SysWriteLine( lpTaskView, hFile, "" );
  SysWriteLine( lpTaskView, hFile, "IMPORTS" );
} // fnWriteDFB_IBMC20

//./ ADD NAME=fnCreateDEF_IBMC20
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnCreateDEF_IBMC20
//
//  PURPOSE:    Create a DEF for OS/2 (IBM C++ 2.0)
//
//  PARAMETERS: hFile - File Handle for DFB file
//              vTaskLPLR - Name of the DLL
//
//  RETURNS:    -
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zVOID
fnCreateDEF_IBMC20( zVIEW         lpTaskView,
                    zPCHAR        pchTargetName,
                    zCOMPILE_DIRS *pcd,
                    zVIEW         vTaskLPLR,
                    zPCHAR        pchLine )
{
   zLONG hDFB = -1;
   zLONG hDEF = -1;
   zPCHAR pchGroup;
   zCHAR szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR szFileName2[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR szMetaFileName[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pchDFBLine;
   zSHORT nMetaLth;
   zSHORT nRC;
   zVIEW vMeta;

   strcpy_s( szFileName2, sizeof( szFileName2 ), pcd->pszMetaSrcDir );
   SysConvertEnvironmentString( szMetaFileName, sizeof( szMetaFileName ), szFileName2 );
   strcat_s( szMetaFileName, sizeof( szMetaFileName ), "\\" );
   nMetaLth = (zSHORT) zstrlen( szMetaFileName );

   sprintf_s( szFileName2, sizeof( szFileName2 ), "%s\\%s.DFB", pcd->pszEnvironmentDir, pchTargetName );
   SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szFileName2 );
   hDFB = SysOpenFile( lpTaskView, szFileName, COREFILE_READ );
   if ( hDFB != -1 )
   {
      sprintf_s( szFileName2, sizeof( szFileName2 ), "%s\\%s.DEF", pcd->pszEnvironmentDir, pchTargetName );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szFileName2 );
      hDEF = SysOpenFile( lpTaskView, szFileName, COREFILE_WRITE );
      if ( hDEF != -1 )
      {
         // copy the DFB to the beginning of the DEF
         do
         {
            nRC = SysReadLine( lpTaskView, &pchDFBLine, hDFB );
            if (nRC == 1)
              SysWriteLine( lpTaskView, hDEF, pchDFBLine );

         } while ( nRC == 1 );

         // Add global op entries to the import section.

         // List the Global Op Groups.
         nRC = SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type", zREFER_GOPGRP_META, 0 );

         // Every global op group lib is listed in the ZeidonLib entity.
         if ( nRC >= zCURSOR_SET )
         {
            // For each global op lib.
            for ( nRC = SetCursorFirstEntity( vTaskLPLR, "W_MetaDef", 0 );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vTaskLPLR, "W_MetaDef", 0 ) )
            {
               zSHORT nRCM;
               zSHORT nGroupInTarget = FALSE;

               // Get the group name
               nRCM = GetAddrForAttribute( &pchGroup, vTaskLPLR, "W_MetaDef", "Name" );

               // look, if we have this global op group in our Target DLL
               for ( nRCM = SetCursorFirstEntityByInteger( vTaskLPLR, "Meta", "MetaType",
                                                           zREFER_GOPGRP_META, 0 );
                     nRCM >= zCURSOR_SET;
                     nRCM = SetCursorNextEntityByInteger( vTaskLPLR, "Meta", "MetaType",
                                                          zREFER_GOPGRP_META, 0 ) )
               {
                  if ( SetCursorFirstEntityByString( vTaskLPLR, "Meta", "Name", pchGroup,
                                                     0 ) >= zCURSOR_SET )
                  {
                     nGroupInTarget = TRUE;
                  }
               }

               // If Global Op is not in the target DLL.
               if ( !nGroupInTarget )
               {
                  zPCHAR pchOpName;
                  zPCHAR pchOpDLL;
                  zSHORT nRCG;
                  zCHAR szOpNameUpper[ 33 ];
                  zCHAR szDLLUpper[ 9 ];

                  // Activate the global op group.
                  strcpy_s( szMetaFileName + nMetaLth, sizeof( szMetaFileName ) - nMetaLth, pchGroup );
                  strcat_s( szMetaFileName, sizeof( szMetaFileName ), ".POG" );
                  ActivateOI_FromFile( &vMeta, "TZOGSRCO", vTaskLPLR, szMetaFileName, zSINGLE );
                  GetAddrForAttribute( &pchOpDLL, vMeta, "GlobalOperationGroup", "DomainAndGlobalOpGroupName" );

                  strcpy_s( szDLLUpper, sizeof( szDLLUpper ), pchOpDLL);
                  SysTranslateString( szDLLUpper, 'U');

                  // get the operations
                  OrderEntityForView( vMeta, "Operation", "Name A");
                  for ( nRCG = SetCursorFirstEntity( vMeta, "Operation", 0 );
                        nRCG >= zCURSOR_SET;
                        nRCG = SetCursorNextEntity( vMeta, "Operation", 0 ) )
                  {
                    zPCHAR pchOpType;

                    // look for operation type
                    GetAddrForAttribute( &pchOpType, vMeta, "Operation", "Type" );

                    // if no local operation
                    if (*pchOpType != 'L')
                    {
                      // build import entry
                      GetAddrForAttribute( &pchOpName, vMeta, "Operation", "Name" );
                      strcpy_s( szOpNameUpper, sizeof( szOpNameUpper ), pchOpName );
                      SysTranslateString( szOpNameUpper, 'U' );
                      sprintf_s( pchLine, zLINESIZE, "   %-32s=%s.%s", szOpNameUpper, szDLLUpper, pchOpName );
                      SysWriteLine( lpTaskView, hDEF, pchLine );
                    }

                  } // for each global op group
               } // if !nGroupInTarget
            } // For each "Global Op Lib"...
         } // if

         //
         // Create the export section
         //
         SysWriteLine( lpTaskView, hDEF, "" );
         SysWriteLine( lpTaskView, hDEF, "EXPORTS" );

         // Load each meta.
         for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Meta", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTaskLPLR, "Meta", 0 ) )
         {
            zLONG  lType;
            zPCHAR pchOD_Name;
            zPCHAR pchSourceFileEntityName;
            zPCHAR pchOpEntityName;
            zVIEW  vMeta;

            GetStringFromAttribute( szMetaFileName + nMetaLth, sizeof( szMetaFileName ) - nMetaLth,
                                    vTaskLPLR, "Meta", "Name" );
            GetIntegerFromAttribute( &lType, vTaskLPLR, "Meta", "MetaType" );

            switch ( lType )
            {
               case zREFER_GOPGRP_META:
                  strcat_s( szMetaFileName, sizeof( szMetaFileName ), ".POG" );
                  pchOD_Name = "TZOGSRCO";
                  pchSourceFileEntityName = "GlobalOperationGroup";
                  pchOpEntityName = "Operation";
                  break;

               case zREFER_DIALOG_META:
                  strcat_s( szMetaFileName, sizeof( szMetaFileName ), ".PWD" );
                  pchOD_Name = "TZWDLGSO";
                  pchSourceFileEntityName = "SourceFile";
                  pchOpEntityName = "Operation";
                  break;

               case zREFER_LOD_META:
                  strcat_s( szMetaFileName, sizeof( szMetaFileName ), ".LOD" );
                  pchOD_Name = "TZZOLODO";
                  pchSourceFileEntityName = "SourceFile";
                  pchOpEntityName = "OperationList";
                  break;

               case zREFER_DOMAINGRP_META:
                  strcat_s( szMetaFileName, sizeof( szMetaFileName ), ".PDG" );
                  pchOD_Name = "TZDGSRCO";
                  pchSourceFileEntityName = "DomainGroup";
                  pchOpEntityName = "Operation";
                  TraceLine( "Loading Refer Domain Group: %s ", szMetaFileName );
                  break;
            }

            // Activate the meta.
            ActivateOI_FromFile( &vMeta, pchOD_Name,
                                 vTaskLPLR, szMetaFileName, zSINGLE );

            // For each meta: go through all source files
            for ( nRC = SetCursorFirstEntity( vMeta, pchSourceFileEntityName, 0 );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vMeta, pchSourceFileEntityName, 0 ) )
            {
              // For each source file: get the operations
              OrderEntityForView( vMeta, pchOpEntityName, "Name A" );
              for ( nRC = SetCursorFirstEntity( vMeta, pchOpEntityName, 0 );
                    nRC >= zCURSOR_SET;
                    nRC = SetCursorNextEntity( vMeta, pchOpEntityName, 0 ) )
              {
                 zPCHAR pchOpType;
                 zPCHAR pchOpName;
                 zCHAR szOpNameUpper[ 33 ];

                 // look for operation type
                 GetAddrForAttribute( &pchOpType, vMeta, pchOpEntityName, "Type" );

                 // if no local operation
                 if ( *pchOpType != 'L' )
                 {
                   // build export entry
                   GetAddrForAttribute( &pchOpName, vMeta, pchOpEntityName, "Name" );
                   strcpy_s( szOpNameUpper, sizeof( szOpNameUpper ), pchOpName );
                   SysTranslateString( szOpNameUpper, 'U' );
                   sprintf_s( pchLine, zLINESIZE, "   %-32s=%s", pchOpName, szOpNameUpper );
                   SysWriteLine( lpTaskView, hDEF, pchLine );
                 }
              } // for each pchOpEntityName...
            } // for each pchSourceFileEntityName...

            DropObjectInstance( vMeta );

         } // for each "Meta"

         SysCloseFile( lpTaskView, hDEF, 0 );
      }
      else
      {
         sprintf_s( pchLine, zLINESIZE, "Error creating file '%s'.", szFileName );
         MessageSend( vTaskLPLR, "LO00108", "Zeidon Tools",
                      pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );
      }

      SysCloseFile( lpTaskView, hDFB, 0 );
   }

} // fnCreateDEF_IBMC20

//./ ADD NAME=fnWriteZeidonResourcesToRC
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnWriteZeidonResourcesToRC
//
//  PURPOSE:    Create a RC file, if it does not exist or update an
//              existing one.
//              If a dialog contains resources (BMP or ICO), we will
//              put an entry for each resource in the RC
//
//  PARAMETERS: hFile - File Handle for RC file
//              vTaskLPLR - Name of the DLL
//              pcd - directories for compile
//
//  RETURNS:    - 2 - Dialog (as well as resource file) needs to be saved.
//              - 1 - Resource file needs to be saved.
//              - 0 - Dialog does not need to be saved.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnWriteZeidonResourcesToRC( zLONG         hFile,
                            zVIEW         vSubtask,
                            zVIEW         vTaskLPLR,
                            zCOMPILE_DIRS *pcd,
                            zPCHAR        pchLine,
                            zLONG         lMaxLth )
{
   zSHORT nChangeLevel = 0;
   zLONG  lMetaType;
   zVIEW  vDialog;
// zCHAR  szMetaFileName[ zMAX_FILENAME_LTH ];
   zCHAR  szMetaName[ 33 ];
   zSHORT nRC;

   pcd->nResourceFiles = 0;

   // Write Zeidon header
   SysWriteLine( vTaskLPLR, hFile, ZEIDON_RC_START );
   SysWriteLine( vTaskLPLR, hFile,
                 "//  This section contains resources generated by Zeidon." );
   SysWriteLine( vTaskLPLR, hFile,
                 "//  Reserved resource IDs are above 4096.  Please do not" );
   SysWriteLine( vTaskLPLR, hFile,
                 "//  use these IDs elsewhere in this RC file.");
   SysWriteLine( vTaskLPLR, hFile,
                 "//  NOTE - Zeidon will add and remove resources here." );
   SysWriteLine( vTaskLPLR, hFile,
                 "//         Do NOT edit this block of generated code!" );

   // Write Zeidon resources ... check each meta.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Meta", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Meta", 0 ) )
   {
      GetIntegerFromAttribute( &lMetaType, vTaskLPLR, "Meta", "MetaType" );

      // Is it a dialog meta?
      if ( lMetaType == zREFER_DIALOG_META )
      {
         // Activate the meta.
         GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vTaskLPLR, "Meta", "Name" );
         if ( ActivateMetaOI_ByName( vSubtask, &vDialog, 0,
                                     zREFER_DIALOG_META,
                                     zSINGLE | zLEVEL_TASK,
                                     szMetaName, zCURRENT_OI ) < 0 )
         {
            sprintf_s( pchLine, zLINESIZE, "Could not open dialog meta for update: '%s'", szMetaName );
            MessageSend( vSubtask, "LO00111", "Zeidon Tools",
                         pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );
            break;
         }

         nRC = SetResourceFileIDs( vSubtask, vTaskLPLR, hFile, pcd, pchLine, lMaxLth, vDialog );
         if ( nRC > nChangeLevel )
            nChangeLevel = nRC;

         DropMetaOI( vSubtask, vDialog );

      } // is a dialog meta
   } // for each meta

   // Write Zeidon footer
   SysWriteLine( vTaskLPLR, hFile, ZEIDON_RC_END );

   return( nChangeLevel );

} // fnWriteZeidonResourcesToRC

//./ ADD NAME=fnCreateRC
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnCreateRC
//
//  PURPOSE:    Create a RC file, if it does not exist or update an
//              existing one.
//              If a dialog contains resources (BMP or ICO), we will
//              put an entry for each resource in the RC
//
//  PARAMETERS: pchTargetName - Name of the DLL
//              vTaskLPLR - view of the LPLR
//              pcd - directories for compile
//
//  RETURNS:    2 - OK, Dialog needs to be saved (resource file changed)
//              1 - OK, resource file changed
//              0 - OK, resource file not changed
//              else - failed
//
// called by: fnCreateMakefileForTarget
//            CreateMakefileForTarget
//            CreateMakefileForAllTargets
//            MakeTarget (called from Target Specification dialog)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnCreateRC( zPCHAR        pchTargetName,
            zVIEW         vSubtask,
            zVIEW         vTaskLPLR,
            zCOMPILE_DIRS *pcd,
            zPCHAR        pchLine,
            zLONG         lMaxLth )
{
   zLONG  hFile;
   zLONG  hIn;
   zLONG  hOut;
   zCHAR  szFileName[ 2 * zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFileName2[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFileNameTemp[ 2 * zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pchRC;
   zSHORT nRC = 0;

   // Is there an old RC file?
   sprintf_s( szFileName2, sizeof( szFileName2 ), "%s\\%s.RC", pcd->pszEnvironmentDir, pchTargetName );
   SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szFileName2 );
   hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_EXIST );
   if ( hFile == -1 )
   {
      // if no old RC: create new one
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      if ( hFile == -1 )
      {
         sprintf_s( pchLine, zLINESIZE,
                    "Error creating file: '%s'.  Check specification of Environment directory for a valid directory name.", szFileName );
         MessageSend( vSubtask, "LO00108", "Zeidon Tools",
                      pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );

         return( -1 );
      }

      // write header
      sprintf_s( pchLine, zLINESIZE, "/* %s.rc : Defines the resources for the target "
                 "executable %s */", pchTargetName, pchTargetName );
      SysWriteLine( vTaskLPLR, hFile, pchLine );

      // Write resources.
      nRC = fnWriteZeidonResourcesToRC( hFile, vSubtask, vTaskLPLR, pcd, pchLine, lMaxLth );
      SysCloseFile( vTaskLPLR, hFile, 0 );
   }
   else
   {
      // If an old resource file exists: update ZEIDON section.

      // We will first copy the rc to a temp file and update the temp file.
      // If nothing gets written to the temp file then we'll skip the update.
      // This way the .RC file only gets updated in specific cases and keeps
      // the compile from unnecessarily linking.

      // create new RC as temp file
      sprintf_s( szFileName2, sizeof( szFileName2 ), "%s\\%s.RC~", pcd->pszEnvironmentDir, pchTargetName );
      SysConvertEnvironmentString( szFileNameTemp, sizeof( szFileNameTemp ), szFileName2 );
      hOut = SysOpenFile( vTaskLPLR, szFileNameTemp, COREFILE_WRITE );
      if ( hOut == -1 )
      {
         sprintf_s( pchLine, zLINESIZE,
                    "Error creating file: '%s' ...  check specification of Environment directory for a valid directory name.", szFileNameTemp );
         MessageSend( vSubtask, "LO00108", "Zeidon Tools",
                      pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );

         return( -1 );
      }

      // Open old resource file.
      hIn = SysOpenFile( vTaskLPLR, szFileName, COREFILE_READ );
      if ( hIn == -1 )
      {
         sprintf_s( pchLine, zLINESIZE,
                    "Error opening file '%s'.  Check specification of Environment directory for a valid directory name.", szFileName );
         MessageSend( vSubtask, "LO00108", "Zeidon Tools",
                      pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );

         SysCloseFile( vTaskLPLR, hOut, 0 );
         return( -1 );
      }

      // Copy head of old RC.
      for ( ; ; )
      {
         // Read a line from old RC ... if eof then done.
         if ( SysReadLine( vTaskLPLR, (zCOREMEM) &pchRC, hIn ) <= 0 )
            break;

         // Look for the beginning of the ZEIDON section
         if ( pchRC[ 0 ] == '/' &&
              zstrncmp( pchRC, ZEIDON_RC_START, zstrlen( ZEIDON_RC_START ) ) == 0 )
         {
            // We found the start marking, so skip over it until
            // we find the end marking.
            for ( ; ; )
            {
               SysReadLine( vTaskLPLR, (zCOREMEM) &pchRC, hIn );
               if ( pchRC == 0 )
               {
                  break;
               }

               if ( pchRC[ 0 ] == '/' &&
                    zstrncmp( pchRC, ZEIDON_RC_END, zstrlen( ZEIDON_RC_END ) ) == 0 )
               {
                  break;
               }
            }

            break;  // stop looping
         }
         else
         {
            // We haven't hit ZEIDON section yet ... copy line from old file to new file.
            SysWriteLine( vTaskLPLR, hOut, pchRC );
         }
      } // copy head

      // Write resources in Zeidon section.  If bResources = FALSE then there
      // the Dialog does not need to be saved.
      nRC = fnWriteZeidonResourcesToRC( hOut, vSubtask, vTaskLPLR, pcd, pchLine, lMaxLth );

      // Copy tail of old RC.
      for ( ; ; )
      {
         // Read a line from old RC ... if eof then done.
         if ( SysReadLine( vTaskLPLR, (zCOREMEM) &pchRC, hIn ) <= 0 )
            break;

         // copy it
         SysWriteLine( vTaskLPLR, hOut, pchRC );

      } // copy tail

      SysCloseFile( vTaskLPLR, hIn, 0 );
      SysCloseFile( vTaskLPLR, hOut, 0 );

      if ( nRC )
      {
         // Replace old RC with new temp RC.
         SysOpenFile( vTaskLPLR, szFileName, COREFILE_DELETE );
         strcpy_s( pchLine, zLINESIZE, szFileNameTemp );
         strcpy_s( &pchLine[ zstrlen( pchLine ) + 1 ], zLINESIZE - zstrlen( pchLine ) - 1, szFileName );
         SysOpenFile( vTaskLPLR, pchLine, COREFILE_RENAME );
      }
      else
      {
         // Just kill the temp file.
         SysOpenFile( vTaskLPLR, szFileNameTemp, COREFILE_DELETE );
      }
   }

   return( nRC );
} // fnCreateRC

//./ ADD NAME=FindCompilerType
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: FindCompilerType
//
//  PURPOSE:   Find compiler type for compiler spec
//             Cursor in vTaskLPLR must be set to compiler spec
//
//  PARAMETERS: vTaskLPLR    - task LPLR view
//
//  RETURNS:    0 - error
//              else - compiler type:
//                     zCOMP_TYPE_MSVC15 1
//                     zCOMP_TYPE_IBMC20 2
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
FindCompilerType( zVIEW  vTaskLPLR )
{
   zPCHAR pchCompilerName;
   zSHORT nCompilerType;

   GetAddrForAttribute( &pchCompilerName, vTaskLPLR, "Compiler", "CompilerName" );
   nCompilerType = 0;
   if ( zstrcmp( pchCompilerName, "MSC 1.5" ) == 0 )
     nCompilerType = zCOMP_TYPE_MSVC15;

   if ( zstrcmp( pchCompilerName, "IBMC 2.0" ) == 0 )
     nCompilerType = zCOMP_TYPE_IBMC20;

   return( nCompilerType );

} // FindCompilerType

void
FormatMakefileLine( zVIEW lpTaskView, zLONG hFile,
                    zPCHAR pchLine, zSHORT nTargetLth )
{
   zCHAR  szBuffer[ 512 ];
   zPCHAR pchSpace;
   zPCHAR pch;
   zSHORT nLth;

   if ( nTargetLth >= sizeof( szBuffer ) )
   {
      SWL( lpTaskView, pchLine );
   }
   else
   {
      nLth = (zSHORT) zstrlen( pchLine );
      while ( nLth )
      {
         if ( nLth > nTargetLth )
         {
            pchSpace = zstrchr( pchLine + nTargetLth, ' ' );
            if ( pchSpace )
            {
               *pchSpace = 0;
               pch = zstrrchr( pchLine, ' ' );
               if ( pch && pch > pchLine + 4 )
                  *pch = 0;
               else
                  pch = pchSpace;

               if ( zstrlen( pchLine ) < sizeof( szBuffer ) )
               {
                  strcpy_s( szBuffer, sizeof( szBuffer ), pchLine );
                  strcat_s( szBuffer, sizeof( szBuffer ), " \\" );
                  SWL( lpTaskView, szBuffer );
                  *pchSpace = ' ';
                  pchLine[ 0 ] = pchLine[ 1 ] = pchLine[ 2 ] = ' ';
                  strcpy_s( pchLine + 3, zLINESIZE - 3, pch + 1 );
                  nLth = (zSHORT) zstrlen( pchLine );
               }
               else
               {
                  *pch = ' ';
                  *pchSpace = ' ';
                  SWL( lpTaskView, pchLine );
                  nLth = 0;
               }
            }
            else
            {
               SWL( lpTaskView, pchLine );
               nLth = 0;
            }
         }
         else
         {
            SWL( lpTaskView, pchLine );
            nLth = 0;
         }
      }
   }
}

// See comments for CreateMakefileForTarget
zOPER_EXPORT zSHORT OPERATION
fnCreateMakefileForTarget( zPCHAR pchTargetName,
                           zVIEW  vSubtask,
                           zVIEW  vTaskLPLR,
                           zCPCHAR cpcGenLang )
{
   zCOMPILE_DIRS cdir;
   zSHORT nReturnCode = 1;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFileName2[ zMAX_FILESPEC_LTH + 1 ];
   zLONG  hLine = 0;
   zLONG  hFile = -1;
   zSHORT nCompilerType;
   zPCHAR pchLine;
   zPCHAR pch;
   zSHORT nRC;
   zSHORT nLth;
   zBOOL  bPath;
   zCHAR  szObjectDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMakefileDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szEnvironmentDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTargetDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szObjString[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMsg[ 900 ];
   //
   // The following directories don't have a default and must be specified.
   //
   GetAddrForAttribute( &cdir.pszPgmSrcDir, vTaskLPLR, "LPLR", "PgmSrcDir" );
   GetAddrForAttribute( &cdir.pszMetaSrcDir, vTaskLPLR, "LPLR", "MetaSrcDir" );
   GetAddrForAttribute( &cdir.pszExecDir,   vTaskLPLR, "LPLR", "ExecDir" );
   GetAddrForAttribute( &cdir.pszCompilerDir, vTaskLPLR, "Compiler", "CompilerDir" );

   //
   // The following directories default to PgmSrcDir
   //
   // Object/Libraries directory incl. support of relative paths
   GetAddrForAttribute( &cdir.pszTargetObjectDir, vTaskLPLR, "Compiler", "TargetObjectDir" );
   if ( cdir.pszTargetObjectDir == 0 || *cdir.pszTargetObjectDir == 0 )
      cdir.pszTargetObjectDir = cdir.pszPgmSrcDir;

// strcpy_s( szObjectDir, sizeof( szObjectDir ) cdir.pszTargetObjectDir );
   SysConvertEnvironmentString( szObjectDir, sizeof( szObjectDir ), cdir.pszTargetObjectDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szObjectDir, cdir.pszMetaSrcDir, zMAX_FILENAME_LTH - 2, "OBJ Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   cdir.pszTargetObjectDir = szObjectDir;
   if ( !VerifyDir( vTaskLPLR, cdir.pszTargetObjectDir, "OBJ Directory", TRUE ) )
   {
      goto EndOfFunction;
   }

   // MakefileDir incl. support of relative paths
   GetAddrForAttribute( &cdir.pszMakefileDir, vTaskLPLR, "Compiler", "MakefileDir" );
   if ( cdir.pszMakefileDir == 0 || *cdir.pszMakefileDir == 0 )
      cdir.pszMakefileDir = cdir.pszPgmSrcDir;

// strcpy_s( szMakefileDir, sizeof( szMakefileDir ), cdir.pszMakefileDir );
   SysConvertEnvironmentString( szMakefileDir, sizeof( szMakefileDir ), cdir.pszMakefileDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szMakefileDir, cdir.pszMetaSrcDir, zMAX_FILENAME_LTH - 2, "Makefile Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   cdir.pszMakefileDir = szMakefileDir;
   if ( !VerifyDir( vTaskLPLR, cdir.pszMakefileDir, "Makefile Directory", TRUE ) )
      goto EndOfFunction;

   // Resource-Dir incl. support of relative paths
   GetAddrForAttribute( &cdir.pszEnvironmentDir, vTaskLPLR, "Compiler", "EnvironmentDir" );
   if ( cdir.pszEnvironmentDir == 0 || *cdir.pszEnvironmentDir == 0 )
      cdir.pszEnvironmentDir = cdir.pszPgmSrcDir;

// strcpy_s( szEnvironmentDir, sizeof( szEnvironmentDir ) cdir.pszEnvironmentDir );
   SysConvertEnvironmentString( szEnvironmentDir, sizeof( szEnvironmentDir ), cdir.pszEnvironmentDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szEnvironmentDir, cdir.pszMetaSrcDir, zMAX_FILENAME_LTH - 2, "Resource Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   cdir.pszEnvironmentDir = szEnvironmentDir;
   if ( !VerifyDir( vTaskLPLR, cdir.pszEnvironmentDir, "Resource Directory", TRUE ) )
      goto EndOfFunction;

   //
   // The following directories default to ExecDir
   //
   // Target (Executable) Directory incl. relative paths
   GetAddrForAttribute( &cdir.pszTargetExecutableDir, vTaskLPLR, "Compiler", "TargetExecutableDir" );
   if ( cdir.pszTargetExecutableDir == 0 || *cdir.pszTargetExecutableDir == 0 )
      cdir.pszTargetExecutableDir = cdir.pszExecDir;

// strcpy_s( szTargetDir, sizeof( szTargetDir ) cdir.pszTargetExecutableDir );
   SysConvertEnvironmentString( szTargetDir, sizeof( szTargetDir ), cdir.pszTargetExecutableDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szTargetDir, cdir.pszMetaSrcDir, zMAX_FILENAME_LTH - 2, "Target Executable Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   cdir.pszTargetExecutableDir = szTargetDir;
   if ( !VerifyDir( vTaskLPLR, cdir.pszTargetExecutableDir, "Target Executable Directory", TRUE ) )
   {
      goto EndOfFunction;
   }

   //
   // The following directories default to CompilerDir
   //
   SysGetLocalDirectory( cdir.szZeidonExecutableDir );
   if ( *cdir.szZeidonExecutableDir == 0 )
      strcpy_s( cdir.szZeidonExecutableDir, sizeof( cdir.szZeidonExecutableDir ), cdir.pszCompilerDir );

   // set counters to 0
   cdir.nSourceFiles = cdir.nResourceFiles = 0;

   //
   // Get the compiler type
   //
   nCompilerType = FindCompilerType( vTaskLPLR );

   hLine = SysAllocMemory( (zCOREMEM) &pchLine, zLINESIZE, 0, zCOREMEM_ALLOC, 0 );
   if ( pchLine == 0 )
   {
      nReturnCode = 0;
      hLine = 0;
      goto EndOfFunction;
   }

   //=======================================================================
   //
   // Create the .DFB file (if it doesn't exist)
   //
   // The .DFB file is the .DEF base file.  It is used during the make
   // process to dynamically create a .DEF file.
   //
   //=======================================================================
   sprintf_s( szFileName2, sizeof( szFileName2 ), "%s\\%s.DFB", cdir.pszEnvironmentDir, pchTargetName );
   SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szFileName2 );
   hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_EXIST );
   if ( hFile == -1 )
   {
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      if ( hFile == -1 )
      {
         sprintf_s( pchLine, zLINESIZE,
                    "Error creating file: '%s' ...  Check specification of "
                    "Environment directory for a valid directory name.",
                    szFileName );
         MessageSend( vSubtask, "LO00108", "Zeidon Tools",
                      pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );

         nReturnCode = 0;
         goto EndOfFunction;
      }

#ifdef __WIN32__
      fnWriteDFB_MSVC50( vTaskLPLR, hFile, pchTargetName, pchLine );
#else
      fnWriteDFB_MSVC15( vTaskLPLR, hFile, pchTargetName, pchLine );
#endif

      SysCloseFile( vTaskLPLR, hFile, 0 );
   }

   //=======================================================================
   //
   // Create the .RC file (if it doesn't exist)
   //
   //=======================================================================
   switch ( nCompilerType )
   {
      case zCOMP_TYPE_MSVC15:
         nRC = fnCreateRC( pchTargetName, vSubtask, vTaskLPLR, &cdir, pchLine, zLINESIZE );
         if ( nRC < 0 )
         {
            nReturnCode = 0;
            goto EndOfFunction;
         }
         else
         if ( nRC > nReturnCode )
         {
            nReturnCode = nRC;
         }

         break;

      case zCOMP_TYPE_IBMC20:
         // We do not support OS/2 resources.
         break;
   }

   //=======================================================================
   //
   // Create the makefile.
   //
   //=======================================================================
   sprintf_s( szFileName2, sizeof( szFileName2 ), "%s\\%s.MAK", cdir.pszMakefileDir, pchTargetName );
   SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szFileName2 );
   hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
   if ( hFile == -1 )
   {
      sprintf_s( pchLine, zLINESIZE, "Could not create file\n  '%s'\nCheck Compiler "
                 "Specification for valid Makefile Directory", szFileName );
      MessageSend( vSubtask, "LO00107", "Zeidon Tools",
                   pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );

      nReturnCode = 0;
      goto EndOfFunction;
   }

   // Write comment lines (use szFileName & szFileName2 as temp vars).
   SWL( vTaskLPLR, "#" );
   sprintf_s( pchLine, zLINESIZE, "#  %s.mak", pchTargetName );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, "#  Makefile for Zeidon Source File" );
   SysGetDateTime( szFileName, sizeof( szFileName ) );
   UfFormatDateTime( szFileName2, sizeof( szFileName2 ), szFileName, "YYYY.MM.DD HH:MI:SS" );
   sprintf_s( pchLine, zLINESIZE, "#  Generated at %s", szFileName2 );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, "#" );

   // Write include of zenv.hnm
   SWL( vTaskLPLR, " " );
   SWL( vTaskLPLR, "# Include makefile to set environment variables." );
   sprintf_s( pchLine, zLINESIZE, "!include \"%s\\zenv.hnm\"", cdir.pszMakefileDir );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, " " );

   sprintf_s( pchLine, zLINESIZE, "MODULE = %s", pchTargetName );
   SWL( vTaskLPLR, pchLine );

   // We now need to find all the Zeidon LIB files that need to be linked in
   // when the target exe is created.  Zeidon LIB files are the libs that are
   // created when *another* target exe is created (ie. Zeidon LIBs are
   // made up of operations defined by the user using Zeidon).  To find what
   // libs need to be linked, we must do the following:
   //    o  Delete all ZeidonLib entities.  This is a temp entity that will
   //       store the name of all the external DLLs referenced by the current
   //       target executable.
   //    o  The current TargetExecutable entity in the LPLR OI matches the
   //       target we are building.  This entity lists each of the global
   //       op groups, LODs, and Dialog operations that make up the target
   //       executable.
   //       Each entity under TargetExecutable lists a metas that contain ops
   //       that will be compiled into the target.  For each of these entities
   //       we need to load the meta.
   //    o  For each meta loaded, load the .XPG (the VML parse object) for
   //       all the VML source files that make up the meta.  In the XPG is a
   //       list of all EXTERNAL operations used by the VML source file.
   //       The XPG also lists the DLL that contains the external operation.
   //    o  When we find the external DLL needed, check the temp entity
   //       ZeidonLib to see if the DLL has already been referenced.  If not,
   //       then add it to ZeidonLib.
   //    o  When all the above is done, loop through ZeidonLib and add each
   //       referenced DLL to the makefile.
   // In addition to the above, we need to keep track of all the Source Files
   // that make up the target executable.  We keep track of them in the
   // variable pchLine.

   SWL( vTaskLPLR, "" );
   SWL( vTaskLPLR, "# The following is a list of all sub-modules that make up" );
   SWL( vTaskLPLR, "# the Target Executable." );
   strcpy_s( pchLine, zLINESIZE, "OBJS =" );

   // Delete all the ZeidonLib entities.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ZeidonLib", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "ZeidonLib", 0 ) )
   {
      DeleteEntity( vTaskLPLR, "ZeidonLib", zREPOS_NONE );
   }

   GetStringFromAttribute( szFileName2, sizeof( szFileName2 ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szFileName2 );
   nLth =  SysAppendcDirSep( szFileName );
   cdir.nSourceFiles = 0;

   // Load each meta.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Meta", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Meta", 0 ) )
   {
      zLONG  lType;
      zPCHAR pchOD_Name;
      zPCHAR pchSourceFileEntityName;
      zVIEW  vMeta;

      GetStringFromAttribute( szFileName + nLth, sizeof( szFileName ) - nLth, vTaskLPLR, "Meta", "Name" );
      GetIntegerFromAttribute( &lType, vTaskLPLR, "Meta", "MetaType" );

      switch ( lType )
      {
         case zREFER_GOPGRP_META:
            strcat_s( szFileName, sizeof( szFileName ), ".POG" );
            pchOD_Name = "TZOGSRCO";
            pchSourceFileEntityName = "GlobalOperationGroup";
            break;

         case zREFER_DIALOG_META:
            strcat_s( szFileName, sizeof( szFileName ), ".PWD" );
            pchOD_Name = "TZWDLGSO";
            pchSourceFileEntityName = "SourceFile";
            break;

         case zREFER_LOD_META:
            strcat_s( szFileName, sizeof( szFileName ), ".LOD" );
            pchOD_Name = "TZZOLODO";
            pchSourceFileEntityName = "SourceFile";
            break;

         case zREFER_DOMAINGRP_META:
            strcat_s( szFileName, sizeof( szFileName ), ".PDG" );
            pchOD_Name = "TZDGSRCO";
            pchSourceFileEntityName = "DomainGroup";
            TraceLine( "Loading Refer2Domain Group: %s ", szFileName );
            break;
      }

      // Activate the meta.
      ActivateOI_FromFile( &vMeta, pchOD_Name, vTaskLPLR, szFileName,
                           zSINGLE | zIGNORE_ATTRIB_ERRORS );

      // For each source file check to see if it's a VML file.  If it is,
      // then load the .XPG and get info from it.
      for ( nRC = SetCursorFirstEntity( vMeta, pchSourceFileEntityName, 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vMeta, pchSourceFileEntityName, 0 ) )
      {
         GetAddrForAttribute( &pch, vMeta, pchSourceFileEntityName, "Name" );
         strcpy_s( szObjString, sizeof( szObjString ), " $(OBJ_DIR)\\" );
         strcat_s( szObjString, sizeof( szObjString ), pch );
         strcat_s( szObjString, sizeof( szObjString ), ".OBJ" );
         if ( strstr( pchLine, szObjString ) )
         {
            sprintf_s( szMsg, sizeof( szMsg ), "Warning: source file '%s' is specified for object "
                       "%s and at least one other LOD.  The make file generation "
                       "will not add a second reference to %s.OBJ for file %s.MAK.  "
                       "Using the same file for two different object is not "
                       "recommended, since the addition of operations in one will "
                       "not be reflected in the other.  If one of the LOD is incorrectly "
                       "referencing the source file, please correct the reference.",
                       pch, szFileName + nLth, pch, pchTargetName );
            MessagePrompt( vTaskLPLR, "LO00111", "Zeidon Tools", szMsg, 1,
                          zBUTTONS_OK, zRESPONSE_OK, zICON_INFORMATION );
         }
         else
         {
            strcat_s( pchLine, zLINESIZE, szObjString );
            cdir.nSourceFiles++;
         }

         // If we have a dialog that contains only resources and no source
         // code then skip it.
         if ( SetCursorFirstEntity( vMeta, "Operation", NULL ) != zCURSOR_SET )
         {
            zLONG hCFile;

            // Is there a C file?
            sprintf_s( szMsg, sizeof( szMsg ), "%s\\%s.C", cdir.pszPgmSrcDir, pch );  // borrow szMsg for a sec
            SysConvertEnvironmentString( szFileName2, sizeof( szFileName2 ), szMsg );
            hCFile = SysOpenFile( vTaskLPLR, szFileName2, COREFILE_EXIST );
            if ( hCFile == -1 )
            {
               // if no C: create new one as dummy to avoid make problems
               hCFile = SysOpenFile( vTaskLPLR, szFileName2, COREFILE_WRITE );
               if ( hCFile == -1 )
               {
                  sprintf_s( szMsg, sizeof( szMsg ), "Error creating file '%s'.  Check "
                             "specification of Environment directory for a "
                             "valid directory name.", szFileName2 );
                  MessageSend( vSubtask, "LO00108", "Zeidon Tools",
                               szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );
               }
               else
               {
                  // write comment
                  sprintf_s( szMsg, sizeof( szMsg ), "/* %s : Dummy C file */", szFileName2 );
                  SysWriteLine( vTaskLPLR, hCFile, szMsg );
                  SysCloseFile( vTaskLPLR, hCFile, 0 );
               }
            }

            continue;  // Continue with next SourceFile.
         }

         // Ignore source files that aren't VML.
         if ( CompareAttributeToString( vMeta, pchSourceFileEntityName, "LanguageType", "V" ) == 0 )
         {
            zVIEW  vXPG;
            zPCHAR pchXPG_Name;
            zCHAR  szXPG_FileName[ zMAX_FILESPEC_LTH + 1 ];

            GetStringFromAttribute( szMsg, sizeof( szMsg ), vTaskLPLR, "LPLR", "ExecDir" ); // borrow szMsg for a sec
            SysConvertEnvironmentString( szXPG_FileName, sizeof( szXPG_FileName ), szMsg );
            GetAddrForAttribute( &pchXPG_Name, vMeta, pchSourceFileEntityName, "Name" );
            SysAppendcDirSep( szXPG_FileName );
            strcat_s( szXPG_FileName, sizeof( szXPG_FileName ), pchXPG_Name );
            if ( cpcGenLang[ 0 ] == 'J' )
               strcat_s( szXPG_FileName, sizeof( szXPG_FileName ), ".XPJ" );
            else
               strcat_s( szXPG_FileName, sizeof( szXPG_FileName ), ".XPG" );

            ActivateOI_FromFile( &vXPG, "TZVSPOOO", vTaskLPLR, szXPG_FileName, zSINGLE );

            if ( vXPG == 0 )
            {
               sprintf_s( pchLine, zLINESIZE, "Could not find XPG for '%s'.  This file must be parsed!", pchXPG_Name );
               MessageSend( vSubtask, "LO00108", "Zeidon Tools",
                            pchLine, zMSGQ_SYSTEM_ERROR, zBEEP );

               nReturnCode = 0;
               continue;
            }

            // XPG was loaded OK.  Now loop through each of the external
            // operations and add the dll name to the ZeidonLib entity.
            for ( nRC = SetCursorFirstEntity( vXPG, "ExternalOperation", 0 );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vXPG, "ExternalOperation", 0 ) )
            {
               // Check to see if the ZeidonLib name is the same name as the
               // current TargetExecutable.  If it is then we don't need to
               // add it to ZeidonLib.
               if ( CompareAttributeToAttribute( vXPG, "ExternalOperation", "MetaDLL_Name",
                                                 vTaskLPLR, "TargetExecutable", "Name" ) == 0 )
               {
                  // Skip this one.
                  continue;
               }

               // Check to see if there already is a ZeidonLib entity with
               // name = MetaDLL_Name.  If not, then create one.
               if ( SetCursorFirstEntityByAttr( vTaskLPLR, "ZeidonLib", "Name",
                                                vXPG, "ExternalOperation", "MetaDLL_Name",  0 ) < zCURSOR_SET )
               {
                  CreateEntity( vTaskLPLR, "ZeidonLib", zPOS_LAST );
                  SetAttributeFromAttribute( vTaskLPLR, "ZeidonLib", "Name", vXPG, "ExternalOperation", "MetaDLL_Name" );
               }
            } // for each "ExternalOperation"...

            DropObjectInstance( vXPG );

         } // if CompareAttribute...

      } // for each pchSourceFileEntityName...

      DropObjectInstance( vMeta );

   } // for each "Meta"

   // Now write the line that contains the OBJs.
   FormatMakefileLine( vTaskLPLR, hFile, pchLine, 80 );
   SWL( vTaskLPLR, "" );

   // If there is an ExternalTarget entity with name matching the target name, then the user has possibly specified
   // some external modules that need to be linked in with the current target.
   if ( SetCursorFirstEntityByString( vTaskLPLR, "ExternalTarget", "Name", pchTargetName, 0 ) >= zCURSOR_SET )
   {
      SWL( vTaskLPLR, "# External libraries/objects for this target" );
      strcpy_s( pchLine, zLINESIZE, "LINKDLLS = " );

      // Add each ExternalLibFile to the LINKDLLS line.
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalLibFile", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalLibFile", 0 ) )
      {
         GetStringFromAttribute( szFileName2, sizeof( szFileName2 ), vTaskLPLR, "ExternalLibFile", "Name" );

         // Extract the Filename without Drive, Dir, or Ext ...
         _splitpath_s( szFileName2, 0, 0, 0, 0, szFileName2, sizeof( szFileName2 ), 0, 0 );
         strcat_s( pchLine, zLINESIZE, szFileName2 );
         strcat_s( pchLine, zLINESIZE, ".lib " );
      }

      // The following is a little trick.  The last zCHAR in pchLine is either
      // a comma or a space.  Since we can't have a comma at the end, we
      // change that last zCHAR to a null term. Since the only other
      // possibility is a space, it doesn't hurt to change it to a null-term.
      pchLine[ zstrlen( pchLine ) - 1 ] = 0;

      SWL( vTaskLPLR, pchLine );

      strcpy_s( pchLine, zLINESIZE, "LINKOBJS = " );

      // Add each ExternalObjFile to the LINKOBJS line.
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalObjFile", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalObjFile", 0 ) )
      {
         GetStringFromAttribute( szObjectDir, sizeof( szObjectDir ), vTaskLPLR, "ExternalObjFile", "Name" );

         // Extract the Filename without Drive, Dir, or Ext ...
         _splitpath_s( szObjectDir, 0, 0, 0, 0, szObjectDir, sizeof( szObjectDir ), 0, 0 );

         // if no path given, use OBJ_DIR from LPLR
         for ( pch = szObjectDir, bPath = FALSE;
               !bPath && *pch;
               pch++)
         {
            if ( *pch == '\\' || *pch == ':' )
               bPath = TRUE;
         }

         if ( bPath == FALSE )
            strcat_s( pchLine, zLINESIZE, "$(OBJ_DIR)\\" );

         strcat_s( pchLine, zLINESIZE, szObjectDir );
         strcat_s( pchLine, zLINESIZE, ".OBJ" );
         strcat_s( pchLine, zLINESIZE, " " );
      }

      // The following is a little trick.  The last zCHAR in pchLine is either
      // a comma or a space.  Since we can't have a comma at the end, we
      // change that last zCHAR to a null term. Since the only other possibility
      // is a space, it doesn't hurt to change it to a null-term.
      pchLine[ zstrlen( pchLine ) - 1 ] = 0;
      FormatMakefileLine( vTaskLPLR, hFile, pchLine, 80 );
   }
   else
   {
      // The user didn't specify any special DLLs, so write and empty DLL line.
      SWL( vTaskLPLR, "# No external libraries/objects for this target defined." );
      SWL( vTaskLPLR, "LINKDLLS = " );
      SWL( vTaskLPLR, "LINKOBJS = " );
   }

   SWL( vTaskLPLR, "" );

   // Initialize pchLine to be empty.
   *pchLine = 0;

   // Every external lib referenced by the current target exe is listed in
   // the ZeidonLib entity.  Add each entry to the LINKDLLS line.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ZeidonLib", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "ZeidonLib", 0 ) )
   {
      zPCHAR pchLib;

      GetAddrForAttribute( &pchLib, vTaskLPLR, "ZeidonLib", "Name" );
      if ( *pchLine == 0 )
         sprintf_s( pchLine, zLINESIZE, "LINKDLLS = $(LINKDLLS) %s.lib", pchLib );
      else
      {
         strcat_s( pchLine, zLINESIZE, " " );
         strcat_s( pchLine, zLINESIZE, pchLib );
         strcat_s( pchLine, zLINESIZE, ".lib" );
      }

   } // For each "ZeidonLib"...

   // Don't bother writing pchLine unless there something in it.
   if ( *pchLine )
   {
      SWL( vTaskLPLR, "# Resolve libraries for external operations" );
      SWL( vTaskLPLR, pchLine );
      SWL( vTaskLPLR, "" );
   }

   // Write include of zcompile.hnm
   SWL( vTaskLPLR, " " );
   SWL( vTaskLPLR, "# Include makefile to make target." );
   sprintf_s( pchLine, zLINESIZE, "!include \"%s\\zcompile.hnm\"", cdir.pszMakefileDir );
   SWL( vTaskLPLR, pchLine );

EndOfFunction:
   if ( hFile != -1 )
      SysCloseFile( vTaskLPLR, hFile, 0 );

   if ( hLine )
      SysFreeMemory( hLine );

   // check, if we either have resources or source files for our make
   if ( (cdir.nSourceFiles + cdir.nResourceFiles) == 0 )
     nReturnCode = 0;

   return( nReturnCode );

} // fnCreateMakefileForTarget

//./ ADD NAME=CreateMakefileForTarget
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: CreateMakefileForTarget
//
//  PURPOSE:   This routine creates all the files necessary to make the
//             target executable.  They are (files listed without full
//             directory path):
//
//                target.MAK  - The makefile.
//                target.DFB  - The base .DEF file.
//                target.RC   - The Windows resource config file.
//                ZMAKE.BAT   - Batch file that runs the makefile.
//
//  PARAMETERS: pchTargetName   - the name of the target executable.
//              vSrcTaskLPLR    - task LPLR view.  May be 0.
//              pchCompilerSpec - Name of compiler spec to use.  May be 0
//                                (or null string) in which case the current
//                                compiler spec as specified in the root
//                                entity of the LPLR is used.
//              bBuildAllTargets - If TRUE, makefiles are run using the -a
//                                command to build all targets in the
//                                makefile.
//              pchReturnCommand - Pointer to a buffer.  A string that can
//                                be used to run the makefile will be copied
//                                into the buffer.  May be 0.
//
//  RETURNS:    1 - Make file created OK.
//              0 - Error creating makefile.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
CreateMakefileForTarget( zPCHAR pchTargetName,
                         zVIEW  vSubtask,
                         zVIEW  vSrcTaskLPLR,
                         zPCHAR pchCompilerSpec,
                         zBOOL  bBuildAllTargets,
                         zPCHAR pchReturnCommand,
                         zLONG  lMaxCommandLth,
                         zCPCHAR cpcGenLang )
{
   zVIEW  vTaskLPLR;
   zPCHAR pchCompilerDir;
   zPCHAR pchMakefileDir;
   zCHAR  szMakefileDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szBaseDir[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pchEnvironmentDir;
   zCHAR  szEnvDir[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pchPgmSrcDir;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szLocalDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szLine[ 2 * zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMsg[ zMAX_FILESPEC_LTH + 1  ];
   zLONG  hFile = -1;
   zSHORT nCompilerType;
   zSHORT nRC;

   if ( vSrcTaskLPLR == 0 )
   {
      GetViewByName( &vSrcTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vSrcTaskLPLR == 0 )
         return( FALSE );
   }

   // Create a new view to the task lplr so that we don't screw up the current
   // values of the cursors.
   CreateViewFromViewForTask( &vTaskLPLR, vSrcTaskLPLR, 0 );

   // Check to see if pchTargetName is valid.
   if ( pchTargetName == 0 ||
        SetCursorFirstEntityByString( vTaskLPLR, "TargetExecutable",
                                      "Name", pchTargetName, 0 ) < zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Target Executable is invalid:\n'" );
      strcat_s( szMsg, sizeof( szMsg ), pchTargetName );
      strcat_s( szMsg, sizeof( szMsg ), "'" );
      MessageSend( vSubtask, "LO00106", "Zeidon Tools",
                   szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );

      DropView( vTaskLPLR );
      return( FALSE );
   }

   // If pchCompilerSpec is specified, try to find the CompilerSpec with
   // a name that matches it.  If it's not specified, then try using
   // LPLR.CurrentCompilerSpec.  In either case it's an error if the compiler
   // spec isn't found.
   if ( pchCompilerSpec && *pchCompilerSpec )
   {
      nRC = SetCursorFirstEntityByString( vTaskLPLR, "Compiler", "Name",
                                          pchCompilerSpec, 0 );
      if ( nRC < zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid name for Compiler Specification:\n'" );
         strcat_s( szMsg, sizeof( szMsg ), pchCompilerSpec );
         strcat_s( szMsg, sizeof( szMsg ), "'" );
         MessageSend( vSubtask, "LO00104", "Zeidon Tools",
                      szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );

         DropView( vTaskLPLR );
         return( FALSE );
      }
   }
   else
   {
      // CompilerSpec was NOT specified, so use the default.
      nRC = SetCursorFirstEntityByAttr( vTaskLPLR, "Compiler", "Name",
                                        vTaskLPLR, "LPLR",
                                        "CurrentCompilerSpec", 0 );
      if ( nRC < zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Current Compiler Specification is invalid:\n'" );
         GetStringFromAttribute( szMsg + zstrlen( szMsg ), sizeof( szMsg ) - zstrlen( szMsg ),
                                 vTaskLPLR, "LPLR", "CurrentCompilerSpec" );
         strcat_s( szMsg, sizeof( szMsg ), "'" );
         MessageSend( vSubtask, "LO00105", "Zeidon Tools",
                      szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );

         DropView( vTaskLPLR );
         return( FALSE );
      }
   }

   nCompilerType = FindCompilerType( vTaskLPLR );

   //=======================================================================
   //
   // Find project directories.
   //
   //=======================================================================

   // Base directory for relative paths.
   GetStringFromAttribute( szLocalDir, sizeof( szLocalDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szBaseDir, sizeof( szBaseDir ), szLocalDir );

   GetAddrForAttribute( &pchPgmSrcDir, vTaskLPLR, "LPLR", "PgmSrcDir" );
   GetAddrForAttribute( &pchEnvironmentDir, vTaskLPLR, "Compiler", "EnvironmentDir" );
   if ( pchEnvironmentDir == 0 || *pchEnvironmentDir == 0 )
      pchEnvironmentDir = pchPgmSrcDir;

   // Build relative Path; base is Component Source directory.
   strcpy_s( szLocalDir, sizeof( szLocalDir ), pchEnvironmentDir );
   SysConvertEnvironmentString( szEnvDir, sizeof( szEnvDir ), szLocalDir );
   if (IntGenFullQualPathFromRelPath( vTaskLPLR, szEnvDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Resource Directory" ) == -1 )
   {
      return( FALSE );
   }

   pchEnvironmentDir = szEnvDir;
   if ( !VerifyDir( vTaskLPLR, pchEnvironmentDir, "Resource Directory", TRUE ) )
      return( FALSE );

   GetAddrForAttribute( &pchMakefileDir, vTaskLPLR, "Compiler", "MakefileDir" );
   if ( pchMakefileDir == 0 || *pchMakefileDir == 0 )
      pchMakefileDir = pchPgmSrcDir;

   // Build relative Path; base is Component Source directory.
   strcpy_s( szMakefileDir, sizeof( szMakefileDir ), pchMakefileDir);
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szMakefileDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Makefile Directory" ) == -1 )
   {
      return( FALSE );
   }

   pchMakefileDir = szMakefileDir;
   if ( !VerifyDir( vTaskLPLR, pchMakefileDir, "Makefile Directory", TRUE ) )
      return( FALSE );

   GetAddrForAttribute( &pchCompilerDir, vTaskLPLR, "Compiler", "CompilerDir" );

   //=======================================================================
   //
   // Create makefile
   //
   //=======================================================================
   if ( !fnCreateMakefileForTarget( pchTargetName, vSubtask, vTaskLPLR, cpcGenLang ) )
   {
      DropView( vTaskLPLR );
      return( FALSE );
   }

   //=======================================================================
   //
   // Create ZMAKE.BAT
   //
   //=======================================================================
   if ( nCompilerType == zCOMP_TYPE_MSVC15 )
   {
      // create ZMAKE.BAT for windows
      sprintf_s( szMsg, sizeof( szMsg ), "%s\\ZMAKE.BAT", pchEnvironmentDir );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szMsg );
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      if ( bBuildAllTargets )
         sprintf_s( szLine, sizeof( szLine ), "%s\\nmake -nologo -a -f %s\\%s.MAK -k",
                    pchCompilerDir, pchMakefileDir, pchTargetName );
      else
         sprintf_s( szLine, sizeof( szLine ), "%s\\nmake -nologo -f %s\\%s.MAK -k",
                    pchCompilerDir, pchMakefileDir, pchTargetName );

      SysWriteLine( vTaskLPLR, hFile, szLine );
      SysCloseFile( vTaskLPLR, hFile, 0 );
   }
   else
   {
      // create ZMAKEOS2.BAT for WinOS2
      sprintf_s( szMsg, sizeof( szMsg ), "%s\\ZMAKEOS2.BAT", pchMakefileDir );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szMsg );
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      SysGetLocalDirectory( szLocalDir );
      sprintf_s( szLine, sizeof( szLine ),
                 "%sHSTART.EXE \"Zeidon Build for OS/2\" /Win /K %s\\ZMAKEOS2.CMD",
                 szLocalDir, pchMakefileDir );
      SysWriteLine( vTaskLPLR, hFile, szLine );
      SysCloseFile( vTaskLPLR, hFile, 0 );

      // create ZMAKEOS2.CMD for OS/2
      sprintf_s( szMsg, sizeof( szMsg ), "%s\\ZMAKEOS2.CMD", pchMakefileDir );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szMsg );
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      SysWriteLine( vTaskLPLR, hFile, "/*                             */" );
      SysWriteLine( vTaskLPLR, hFile, "/* ZMAKEOS2.CMD                */" );
      SysWriteLine( vTaskLPLR, hFile, "/* Compiling Zeidon Operations */" );
      SysWriteLine( vTaskLPLR, hFile, "/*                             */" );
      sprintf_s( szLine, sizeof( szLine ), "'@SET BEGINLIBPATH=%s;'", pchCompilerDir);
      SysWriteLine( vTaskLPLR, hFile, szLine );
      sprintf_s( szLine, sizeof( szLine ), "'@SET PATH=%s;%%PATH%%'", pchCompilerDir);
      SysWriteLine( vTaskLPLR, hFile, szLine );
      if (pchMakefileDir[ 1 ] == ':')
      {
        sprintf_s( szLine, sizeof( szLine ), "'@%c:'", pchMakefileDir[ 0 ] );
        SysWriteLine( vTaskLPLR, hFile, szLine );
        sprintf_s( szLine, sizeof( szLine ), "'@CD %s'", &pchMakefileDir[ 2 ] );
      }
      else
      {
        sprintf_s( szLine, sizeof( szLine ), "'@CD %s'", pchMakefileDir );
      }

      SysWriteLine( vTaskLPLR, hFile, szLine );
      if ( bBuildAllTargets )
         sprintf_s( szLine, sizeof( szLine ), "'%s\\nmake -nologo -a -f %s.MAK > %s.ERR'",
                    pchCompilerDir, pchTargetName, pchTargetName );
      else
         sprintf_s( szLine, sizeof( szLine ), "'%s\\nmake -nologo -f %s.MAK > %s.ERR'",
                    pchCompilerDir, pchTargetName, pchTargetName );

      SysWriteLine( vTaskLPLR, hFile, szLine );
      SysWriteLine( vTaskLPLR, hFile, "'@ECHO \xff'" );
      SysWriteLine( vTaskLPLR, hFile, "'@ECHO Make is done!'" );
      SysWriteLine( vTaskLPLR, hFile, "'@ECHO \xff'" );
      SysCloseFile( vTaskLPLR, hFile, 0 );
   }

   //=======================================================================
   //
   // Create the command buffer.
   //
   //=======================================================================
   // Check to see if the user specified a command buffer.  If so, then
   // create the command that will compile the target executable.
   if ( pchReturnCommand )
   {
      if (nCompilerType == zCOMP_TYPE_MSVC15)
         sprintf_s( pchReturnCommand, lMaxCommandLth, "%s\\ZCOMPILE.PIF", pchEnvironmentDir );
      else
         sprintf_s( pchReturnCommand, lMaxCommandLth, "%s\\ZCOMPOS2.PIF", pchEnvironmentDir );
   }

   DropView( vTaskLPLR );
   return( TRUE );

} // CreateMakefileForTarget

//./ ADD NAME=CreateMakefileForAllTargets
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: CreateMakefileForAllTargets
//
//  PURPOSE:   This routine creates all the makefiles to make all the
//             targets for the current LPLR.  See the comments for
//             CreateMakefileForTarget for more information.
//
//  PARAMETERS: vSrcTaskLPLR    - task LPLR view.  May be 0.
//              pchCompilerSpec - Name of compiler spec to use.  May be 0
//                                (or null string) in which case the current
//                                compiler spec as specified in the root
//                                entity of the LPLR is used.
//              bBuildAllTargets - If TRUE, makefiles are run using the -a
//                                command to build all targets in the
//                                makefile.
//              pchReturnCommand - Pointer to a buffer.  A string that can
//                                be used to run the makefile will be copied
//                                into the buffer.  May be 0.
//
//  RETURNS:    2 - Make file created OK ... Dialog changed.
//              1 - Make file created OK.
//              0 - Error creating makefile.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
CreateMakefileForAllTargets( zVIEW  vSubtask,
                             zVIEW  vTaskLPLR,
                             zPCHAR pchCompilerSpec,
                             zBOOL  bBuildAllTargets,
                             zPCHAR pchReturnCommand,
                             zLONG  lMaxCommandLth,
                             zCPCHAR cpcGenLang )
{
   zPCHAR pchCompilerDir;
   zPCHAR pchMakefileDir;
   zCHAR  szMakefileDir[zMAX_FILESPEC_LTH + 1];
   zPCHAR pchEnvironmentDir;
   zCHAR  szEnvironmentDir[zMAX_FILESPEC_LTH + 1];
   zPCHAR pchPgmSrcDir;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szLocalDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szLine[ 2 * zMAX_FILESPEC_LTH + zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szBaseDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMsg[ zMAX_FILESPEC_LTH + 1  ];
   zLONG  hFile = -1;
   zSHORT nReturnCode = 1;
   zSHORT nRC = 0;
   zSHORT nCompilerType;

   if ( vTaskLPLR == 0 )
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   }

   // If pchCompilerSpec is specified, try to find the CompilerSpec with
   // a name that matches it.  If it's not specified, then try using
   // LPLR.CurrentCompilerSpec.  In either case it's an error if the compiler
   // spec isn't found.
   if ( pchCompilerSpec && *pchCompilerSpec )
   {
      nRC = SetCursorFirstEntityByString( vTaskLPLR, "Compiler", "Name",
                                          pchCompilerSpec, 0 );
      if ( nRC < zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid name for Compiler Specification:\n'" );
         strcat_s( szMsg, sizeof( szMsg ), pchCompilerSpec );
         strcat_s( szMsg, sizeof( szMsg ), "'" );
         MessageSend( vSubtask, "LO00104", "Zeidon Tools",
                      szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );

         DropView( vTaskLPLR );
         return( 0 );
      }
   }
   else
   {
      // CompilerSpec was NOT specified, so use the default.
      nRC = SetCursorFirstEntityByAttr( vTaskLPLR, "Compiler", "Name",
                                        vTaskLPLR, "LPLR",
                                        "CurrentCompilerSpec", 0 );
      if ( nRC < zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Current Compiler Specification is invalid:\n'" );
         GetStringFromAttribute( szMsg + zstrlen( szMsg ), sizeof( szMsg ) - zstrlen( szMsg ),
                                 vTaskLPLR, "LPLR", "CurrentCompilerSpec" );
         strcat_s( szMsg, sizeof( szMsg ), "'" );
         MessageSend( vSubtask, "LO00105", "Zeidon Tools",
                      szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );

//BL     DropView( vTaskLPLR );
         return( 0 );
      }
   }

   //=======================================================================
   //
   // Find project directories.
   //
   //=======================================================================
   // Basedirectory for relative paths.
   GetStringFromAttribute( szLocalDir, sizeof( szLocalDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szBaseDir, sizeof( szBaseDir ), szLocalDir );

   // Get the resource-directory and support relative paths.
   GetAddrForAttribute( &pchPgmSrcDir, vTaskLPLR, "LPLR", "PgmSrcDir" );
   GetAddrForAttribute( &pchEnvironmentDir, vTaskLPLR, "Compiler", "EnvironmentDir" );
   if ( pchEnvironmentDir == 0 || *pchEnvironmentDir == 0 )
      pchEnvironmentDir = pchPgmSrcDir;

   strcpy_s( szLocalDir, sizeof( szLocalDir ), pchEnvironmentDir );
   SysConvertEnvironmentString( szEnvironmentDir, sizeof( szEnvironmentDir ), szLocalDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szEnvironmentDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Resource Directory" ) == -1 )
   {
      return( 0 );
   }

   pchEnvironmentDir = szEnvironmentDir;
   if ( !VerifyDir( vTaskLPLR, pchEnvironmentDir, "Resource Directory", TRUE ) )
      return( FALSE );

   // Get the makefile directory and support relative paths.
   GetAddrForAttribute( &pchMakefileDir, vTaskLPLR, "Compiler", "MakefileDir" );
   if ( pchMakefileDir == 0 || *pchMakefileDir == 0 )
      pchMakefileDir = pchPgmSrcDir;

   strcpy_s( szMakefileDir, sizeof( szMakefileDir ), pchMakefileDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szMakefileDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Makefile Directory" ) == -1 )
   {
      return( 0 );
   }

   pchMakefileDir = szMakefileDir;
   if ( !VerifyDir( vTaskLPLR, pchMakefileDir, "Makefile Directory", TRUE ) )
      return( FALSE );

   GetAddrForAttribute( &pchCompilerDir, vTaskLPLR, "Compiler", "CompilerDir" );

   //=======================================================================
   //
   // Create makefile and ZMAKE.BAT (at the same time).
   //
   //=======================================================================
   nCompilerType = FindCompilerType( vTaskLPLR );
   if ( nCompilerType == zCOMP_TYPE_MSVC15 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), pchEnvironmentDir );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szMsg );
      SysAppendcDirSep( szFileName );
      strcat_s( szFileName, sizeof( szFileName ), "ZMAKE.BAT" );
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );

      for ( nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "TargetExecutable", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTaskLPLR, "TargetExecutable", 0 ) )
      {
         zPCHAR pchTargetName;
         zCHAR  szLine[ 200 ];

         //BL. 2000.01.19 Don't deselect the entity because the target in the Listbox
         //               is still selected after "Make Target"
         // SetSelectStateOfEntity( vTaskLPLR, "TargetExecutable", 0 );  // Turn off select
         GetAddrForAttribute( &pchTargetName, vTaskLPLR, "TargetExecutable", "Name" );
         nRC = fnCreateMakefileForTarget( pchTargetName, vSubtask, vTaskLPLR, cpcGenLang );
         if ( nRC == 0 )
         {
            sprintf_s( szMsg, sizeof( szMsg ), "Error creating make file for target '%s'. Continuing to build makefiles.", pchTargetName );
            MessageSend( vSubtask, "LO00110", "Zeidon Tools",
                         szMsg, zMSGQ_SYSTEM_ERROR, 0 );
            continue;
         }
         else
         if ( nRC > 1 )
            nReturnCode = 2;

         if ( bBuildAllTargets )
            sprintf_s( szLine, sizeof( szLine ), "\"%s\\nmake\" -nologo -a -f \"%s\\%s.MAK\" -k",
                       pchCompilerDir, pchMakefileDir, pchTargetName );
         else
            sprintf_s( szLine, sizeof( szLine ), "\"%s\\nmake\" -nologo -f \"%s\\%s.MAK\" -k",
                       pchCompilerDir, pchMakefileDir, pchTargetName );

         SysWriteLine( vTaskLPLR, hFile, szLine );
      }

      // Close ZMAKE.BAT
      SysCloseFile( vTaskLPLR, hFile, 0 );
   } // end if ZMAKE for win
   else
   {
      // create ZMAKEOS2.BAT for WinOS2
      strcpy_s( szMsg, sizeof( szMsg ), pchEnvironmentDir );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szMsg );
      SysAppendcDirSep( szFileName );
      strcat_s( szFileName, sizeof( szFileName ), "ZMAKEOS2.BAT" );
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      SysGetLocalDirectory( szMsg );
      SysConvertEnvironmentString( szLocalDir, sizeof( szLocalDir ), szMsg );
      sprintf_s( szLine, sizeof( szLine ),
            "%sHSTART.EXE \"Zeidon Build for OS/2\" /Win /K %s\\ZMAKEOS2.CMD",
            szLocalDir, pchMakefileDir );
      SysWriteLine( vTaskLPLR, hFile, szLine );
      SysCloseFile( vTaskLPLR, hFile, 0 );

      // create ZMAKEOS2.CMD for OS/2
      sprintf_s( szMsg, sizeof( szMsg ), "%s\\ZMAKEOS2.CMD", pchMakefileDir );
      SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szMsg );
      hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
      SysWriteLine( vTaskLPLR, hFile, "/*                             */" );
      SysWriteLine( vTaskLPLR, hFile, "/* ZMAKEOS2.CMD                */" );
      SysWriteLine( vTaskLPLR, hFile, "/* Compiling Zeidon Operations */" );
      SysWriteLine( vTaskLPLR, hFile, "/*                             */" );
      sprintf_s( szLine, sizeof( szLine ), "'@SET BEGINLIBPATH=%s;'", pchCompilerDir);
      SysWriteLine( vTaskLPLR, hFile, szLine );
      sprintf_s( szLine, sizeof( szLine ), "'@SET PATH=%s;%%PATH%%'", pchCompilerDir);
      SysWriteLine( vTaskLPLR, hFile, szLine );
      if ( pchMakefileDir[ 1 ] == ':' )
      {
        sprintf_s( szLine, sizeof( szLine ), "'@%c:'", pchMakefileDir[ 0 ] );
        SysWriteLine( vTaskLPLR, hFile, szLine );
        sprintf_s( szLine, sizeof( szLine ), "'@CD %s'", &pchMakefileDir[ 2 ] );
      }
      else
      {
        sprintf_s( szLine, sizeof( szLine ), "'@CD %s'", pchMakefileDir );
      }

      SysWriteLine( vTaskLPLR, hFile, szLine );

      // go through all Executables
      for ( nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "TargetExecutable", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTaskLPLR, "TargetExecutable", 0 ) )
      {
         zPCHAR pchTargetName;
         zCHAR  szLine[ 200 ];

         SetSelectStateOfEntity( vTaskLPLR, "TargetExecutable", 0 );  // Turn off select
         GetAddrForAttribute( &pchTargetName, vTaskLPLR, "TargetExecutable", "Name" );

         nRC = fnCreateMakefileForTarget( pchTargetName, vSubtask, vTaskLPLR, cpcGenLang );
         if ( nRC == 0 )
         {
            sprintf_s( szMsg, sizeof( szMsg ), "Error creating makefile for target '%s'. Continuing to build makefiles.", pchTargetName );
            MessageSend( vSubtask, "LO00110", "Zeidon Tools",
                         szMsg, zMSGQ_SYSTEM_ERROR, 0 );
            continue;
         }
         else
         if ( nRC > 1 )
            nReturnCode = 2;

         if ( bBuildAllTargets )
            sprintf_s( szLine, sizeof( szLine ), "'%s\\nmake -nologo -a -f %s.MAK > %s.ERR'",
                       pchCompilerDir, pchTargetName, pchTargetName );
         else
            sprintf_s( szLine, sizeof( szLine ), "'%s\\nmake -nologo -f %s.MAK > %s.ERR'",
                       pchCompilerDir, pchTargetName, pchTargetName );

         SysWriteLine( vTaskLPLR, hFile, szLine );
      } // end for

      SysWriteLine( vTaskLPLR, hFile, "'@ECHO \xff'" );
      SysWriteLine( vTaskLPLR, hFile, "'@ECHO Make is done!'" );
      SysWriteLine( vTaskLPLR, hFile, "'@ECHO \xff'" );

      // Close ZMAKEOS2.CMD
      SysCloseFile( vTaskLPLR, hFile, 0 );
   } // end if ZMAKEOS2

   //=======================================================================
   //
   // Create the command buffer.
   //
   //=======================================================================
   // Check to see if the user specified a command buffer.  If so, then
   // create the command that will compile the target executable.
   if ( pchReturnCommand )
   {
      if ( nCompilerType == zCOMP_TYPE_MSVC15 )
        sprintf_s( pchReturnCommand, lMaxCommandLth, "%s\\ZCOMPILE.PIF", pchEnvironmentDir );
      else
        sprintf_s( pchReturnCommand, lMaxCommandLth, "%s\\ZCOMPOS2.PIF", pchEnvironmentDir );
   }

   return( nReturnCode );

} // CreateMakefileForAllTargets

//./ ADD NAME=CheckDuplicateEntityByString
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: CheckDuplicateEntityByString
//
//  PURPOSE:   This routine checkes to see if there are two or more entities
//             in an object with the same string value.  Like the SetCursor
//             operations, it allows a Scoping entity.
//
//  PARAMETERS: vView         -
//
//              EntityName    -
//
//              AttributeName -
//
//              StringValue   -
//
//              ScopingEntity -
//
//  RETURNS:    -1            - At least one duplicate exists.
//              0             - Single entity found to match.
//              1             - No entity found to match.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
CheckDuplicateEntityByString( zVIEW   vView,
                              zPCHAR  pchEntityName,
                              zPCHAR  pchAttributeName,
                              zPCHAR  pchStringValue,
                              zPCHAR  pchScopingEntity )
{
   zVIEW  vWkView;
   zSHORT nRC;

   // Use a work view so that the cursor position of original is not altered.
   CreateViewFromViewForTask( &vWkView, vView, 0 );
   nRC = SetCursorFirstEntityByString( vWkView, pchEntityName,
                                       pchAttributeName,
                                       pchStringValue,
                                       pchScopingEntity );
   if ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorNextEntityByString( vWkView, pchEntityName,
                                         pchAttributeName,
                                         pchStringValue,
                                         pchScopingEntity );
      if ( nRC >= zCURSOR_SET )
         nRC = -1;
      else
         nRC = 0;
   }
   else
      nRC = 1;

   DropView( vWkView );
   return( nRC );

} // CheckDuplicateEntityByString

//./ ADD NAME=GetStringLength
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: GetStringLength
//
//  PURPOSE:   This routine returns the length of a string.  It is the same
//             as zstrlen, but for use by VML routines.
//
//  PARAMETERS: pchStringValue - The string value for which the length is
//                              being determined.
//
//  RETURNS:   Length of string.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
GetStringLength( zPCHAR pchStringValue )
{
   return( zstrlen( pchStringValue ) );

} // GetStringLength

//./ ADD NAME=ConvertIntegerToString
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: ConvertIntegerToString
//
//  PURPOSE:   This routine converts an integer to a string.  It is the same
//             as zltoa, but for use by VML routines.
//
//  PARAMETERS: nIntegerValue - Integer to convert.
//              pchStringValue - String holding result.
//              lLength       - Length of string holding result.
//
//  RETURNS:    0             - Conversion ok.
//              -1            - String wasn't long enough to hold integer.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
ConvertIntegerToString( zLONG  lIntegerValue,
                        zPCHAR pchStringValue,
                        zLONG  lLength )
{
   zCHAR  szWorkString[ 20 ];
   zSHORT nWkLth;
   zSHORT nWkPos;

   zltoa( lIntegerValue, szWorkString, sizeof( szWorkString ) );
   nWkPos = (zSHORT) zstrlen( szWorkString );
   nWkLth = (zSHORT) lLength;

   // If the length of the converted integer is longer that the length
   // of the string to hold the result, return a null string and a -1 code.
   if ( nWkPos > nWkLth )
   {
      szWorkString[ 0 ] = 0;
      return( -1 );
   }

   strcpy_s( pchStringValue, lLength, szWorkString );
#if 0
   // Now copy each character to the result string.
   while ( nWkLth >= 0 )
   {
      if ( nWkPos >= 0 )
         pchStringValue[ nWkLth ] = szWorkString[ nWkPos ];
      else
         pchStringValue[ nWkLth ] = 0;

      nWkPos--;
      nWkLth--;
   }
#endif

   return( 0 );

} // ConvertIntegerToString

//./ ADD NAME=StartZeidonHelp
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: StartZeidonHelp
//
//  PURPOSE: Use this operation to start help for a particular help
//           topic as identified by a unique tag. On line help utilities
//           (often?, sometimes?) allow you to access a particular
//           help topic. In the case of MS-WinHelp, help topics
//           are identified by Context Numbers. In Zeidon, we identify
//           help topics with tags. The mapping from our internal tags
//           and the OS dependent context identifiers is maintained
//           in the LOD TZHLPCXO which is stored in the file "tzhlpcxp.phl".
//           This mapping information can be maintained using the internal
//           tool, Help Mapping.
//
//  PARAMETERS:
//          vSubtask - The view to the window requesting the help.
//          pchHelpTag - The unique tag identifying the requested
//                       help topic.
//
//  RETURNS:
//          0 - Help was started successfully.
//          zCALL_ERROR - The requested help topic was not found.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
StartZeidonHelp( zVIEW vSubtask, zPCHAR pchHelpTag )
{
   zVIEW   vHelpMap;
   zCHAR   szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR   szFile[  zBASE_FILENAME_LTH + 1 ];
   zCHAR   szContextID[ 80 ];
   zSHORT  nRC;

   // NOTE: SysStartHelp may need to start the system dependent
   //       help utility differently depending on whether or not
   //       we want to enter at the table of contents or at a given
   //       topic in a "context-sensitive" mode. If SysStartHelp
   //       receives a null string for a tag (the second param),
   //       then it will start help at the table of contents.
   //       Otherwise, it will start help in a context-sensitive
   //       mode, should such a thing exist (as it does in MS-Windows.

   // IF WE NEED TO CALL SYSTEM HELP, CALL IT AND GET OUT
   if ( zstrcmp( pchHelpTag, "SystemHelp" ) == 0 )
   {
      strcpy_s( szFileName, sizeof( szFileName ), "SYSTEM HELP" );
      // start system help at table of contents
      SysStartHelp( szFileName, 0 );
      return( 0 );
   }

   // OTHERWISE, WE NEED TO CALL ZEIDON HELP

   // LOAD THE TAG TO CONTEXT NUMBER MAPPING INFORMATION
   // We need a view for the ActivateOI_FromFile operation.  Most tools
   // will have a named view to "TaskLPLR", so we'll use this most of the
   // time.  However, the SystemAdmin task doesn't have that view, so if
   // vView doesn't exist, we'll use "TZCMREPO".

   // BL, 1999.09.29 Insert Help menu in the Object Browser
   // The View TaskLPLR or TZCMREPO does not exist in the Object Browser.
   // I think, the code works also with the View vSubtask.

// nRC = GetViewByName( &vView, "TaskLPLR", vSubtask, zLEVEL_TASK );
// if ( nRC < 0 )
//    GetViewByName( &vView, "TZCMREPO", vSubtask, zLEVEL_TASK );

   // if mapping info is not already loaded...
// if ( GetViewByName( &vHelpMap, "TZHLPCXO", vSubtask, zLEVEL_TASK ) < 0 )
   if ( GetViewByName( &vHelpMap, "TZHLPCXO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // load mapping info
      SysGetLocalDirectory( szFileName );
      strcat_s( szFileName, sizeof( szFileName ), "TZHLPCXP.PHL");
      ActivateOI_FromFile( &vHelpMap, "TZHLPCXO", vSubtask,
                           szFileName, zSINGLE );
   // ActivateOI_FromFile( &vHelpMap, "TZHLPCXO", vView,
   //                      szFileName, zSINGLE );
      SetNameForView( vHelpMap, "TZHLPCXO", vSubtask, zLEVEL_TASK );

      // set cursor to current operating system
      // currently this is a fudge and we hard wire it to set to windows.
      SetCursorFirstEntityByString( vHelpMap, "OperSystem", "OSName",
                                    "MS-Windows", 0 );
   }

   // LOOK UP THE TAG IN THE MAPPING
   nRC = SetCursorFirstEntityByString( vHelpMap, "Context", "Tag",
                                       pchHelpTag, 0 );

   // if the tag was not found, notify the user
   if ( nRC < zCURSOR_SET )
   {
      return( zCALL_ERROR );
   }

   // GET THE MAPPING INFORMATION
   // get the context number
   GetStringFromAttribute( szContextID, sizeof( szContextID ), vHelpMap, "Context", "Number" );

   // build up file name for help file
   GetStringFromAttribute( szFile, sizeof( szFile ), vHelpMap, "Context", "FileName" );
   SysGetLocalDirectory( szFileName );
   strcat_s( szFileName, sizeof( szFileName ), szFile );

   // CALL HELP
   // if context number is 0, start help at table of contents
   if ( zstrcmp( szContextID, "0" ) == 0 )
   {
      SysStartHelp( szFileName, 0 );
   }

   // otherwise, context number is not 0, so we open help in
   // a context-sensitive mode.
   else
   {
      SysStartHelp( szFileName, szContextID );
   }

   return( 0 );

} // StartZeidonHelp


//./ ADD NAME=CardinalityMaximum
// Source Module=tzapdmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CardinalityMaximum
//
//  PURPOSE:    Domain Validation for E/R Relationship Cardinality
//              Validates that the data entered is either a valid integer
//              or "m" or "M".
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has been set
//    zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT int OPERATION
zdmCardinalityMaximum( zLONG        lEntryType,
                       LPDOMAINDATA lpDomainData )
{
   zSHORT     nRC;
   zCHAR      szWorkString[ 256 ];
   zCHAR      sz[ 256 ];
   LPDOMAIN   lpDomain;
   zSHORT     nPos;
   zLONG      lWork;

   nRC = 0;      // Default to ok status.
   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            // Only handle string data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  // string to string
                  strcpy_s( sz, sizeof( sz ), (zPCHAR) lpDomainData->lpData );

                  // Validate that the input is either the single characters
                  // "M" or "m", or an integer number.
                  if ( zstrcmp( sz, "M" ) != 0 && zstrcmp( sz, "m" ) != 0 )
                  {
                     // Validate each character to be an integer digit.
                     for ( nPos = 0; sz[ nPos ]; nPos++ )
                     {
                        if ( isdigit( sz[ nPos ] ) == 0 )
                        {
                           // "TZDME201 - Invalid Max Cardinality "
                           MessageSend( lpDomainData->zView, "TZDME201",
                                        "Domain Error",
                                        "Invalid Max Cardinality",
                                        zMSGQ_DOMAIN_ERROR, zBEEP );
                           return( zCALL_ERROR );
                        }
                     }
                  }

                  break;

               default:
                  return( zCALL_ERROR );
            }

            if ( (zUSHORT) zstrlen( (zCPCHAR) lpDomainData->lpData ) <=
                                     lpDomainData->lpViewAttribute->ulLth - 1 )
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView,
                                            lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            lpDomainData->lpData, 0 );
               else
                  nRC = 0;
            }
            else
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView,
                                            lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            lpDomainData->lpData, 0 );
               return( zCALL_ERROR );
            }
         }
         else
         {
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_GET_VARIABLE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            nRC = GetValueFromRecord( lpDomainData->zView,
                                      lpDomainData->lpViewEntity,
                                      lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            // Handle only for input type string
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
                     strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, (zPCHAR) lpAttrAddr );
                     nRC = 0;
                  }

                  break;

               default:
                  return( zCALL_ERROR );
            }

            nRC = 0;
         }
         else
         {
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_ADD_TO_ATTRIBUTE:
         break;

      case zDME_COMPARE_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            nRC = GetValueFromRecord( lpDomainData->zView,
                                      lpDomainData->lpViewEntity,
                                      lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            if ( lpAttrAddr == 0 )
            {
               nRC = 1;
               break;
            }

            // Handle only for data types integer and string.
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( lpDomainData->lpViewAttribute->bCaseSens )
                  {
                     nRC = zstrcmp( lpAttrAddr,
                                    (zPCHAR) lpDomainData->lpData );
                  }
                  else
                  {
                     nRC = zstrcmpi( lpAttrAddr,
                                     (zPCHAR) lpDomainData->lpData );
                  }

                  break;

               case zTYPE_INTEGER:
                  lWork = atol( lpAttrAddr );
                  lWork -= *((zPLONG) lpDomainData->lpData);
                  nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
                  break;

               default:
                  return( zCALL_ERROR );
            }
         }
         else
         {
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
         break;

      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
         break;

      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
         break;

      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         break;

      case zDME_GET_COMPARE_VALUE:
         // Handlle only for data types integer and string
         switch ( lpDomainData->cType )
         {
            case zTYPE_STRING:
               nRC = 0;
               break;

            case zTYPE_INTEGER:
               zltoa( *((zPLONG) lpDomainData->lpData), szWorkString, sizeof( szWorkString ) );
               strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szWorkString );
               nRC = 1; // indicate returning a string value
               break;

            default:
               nRC = -2;
         }

         break;

      default:
         return( zDME_NOT_HANDLED );
   }

   return( nRC );
}

//./ ADD NAME=CardinalityMinimum
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CardinalityMinimum
//
//  PURPOSE:    Domain Validation for E/R Relationship Cardinality
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has been set
//    zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT int OPERATION
zdmCardinalityMinimum( zLONG        lEntryType,
                       LPDOMAINDATA lpDomainData )
{
   zSHORT     nRC;
   zCHAR      szWorkString[ 256 ];
   zCHAR      sz[ 256 ];
   LPDOMAIN   lpDomain;
   zSHORT     nPos;
   zLONG      lWork;

   nRC = 0;      // Default to ok status.
   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            // Only handle string data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  // string to string
                  strcpy_s( sz, sizeof( sz ), (zPCHAR) lpDomainData->lpData );

                  // Validate each character to be an integer digit.
                  for ( nPos = 0; sz[ nPos ]; nPos++ )
                  {
                     if ( isdigit( sz[ nPos ] ) == 0 )
                     {
                        // "TZDME201 - Invalid Max Cardinality "
                        MessageSend( lpDomainData->zView, "TZDME201",
                                     "Domain Error",
                                     "Invalid Min Cardinality",
                                     zMSGQ_DOMAIN_ERROR, zBEEP );
                        return( zCALL_ERROR );
                     }
                  }

                  break;

               default:
                  return( zCALL_ERROR );
            }

            if ( (zUSHORT) zstrlen( (zCPCHAR) lpDomainData->lpData ) <=
                                      lpDomainData->lpViewAttribute->ulLth - 1 )
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView,
                                            lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            lpDomainData->lpData, 0 );
               else
                  nRC = 0;
            }
            else
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView,
                                            lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            lpDomainData->lpData, 0 );

               return( zCALL_ERROR );
            }
         }
         else
         {
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_GET_VARIABLE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            nRC = GetValueFromRecord( lpDomainData->zView,
                                      lpDomainData->lpViewEntity,
                                      lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            // Handle only for input type string
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
                     strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, (zPCHAR) lpAttrAddr );
                     nRC = 0;
                  }

                  break;

               default:
                  return( zCALL_ERROR );
            }

            nRC = 0;
         }
         else
         {
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_ADD_TO_ATTRIBUTE:
         break;

      case zDME_COMPARE_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            nRC = GetValueFromRecord( lpDomainData->zView,
                                      lpDomainData->lpViewEntity,
                                      lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            if ( lpAttrAddr == 0 )
            {
               nRC = 1;
               break;
            }

            // Handlle only for data types integer and string
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( lpDomainData->lpViewAttribute->bCaseSens )
                     nRC = zstrcmp( lpAttrAddr, (zPCHAR) lpDomainData->lpData );
                  else
                     nRC = zstrcmpi( lpAttrAddr, (zPCHAR) lpDomainData->lpData );

                  break;

               case zTYPE_INTEGER:
                  lWork = atol( lpAttrAddr );
                  lWork -= *((zPLONG) lpDomainData->lpData);
                  nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
                  break;

               default:
                  return( zCALL_ERROR );
            }
         }
         else
         {
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
         break;

      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
         break;

      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
         break;

      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         break;

      case zDME_GET_COMPARE_VALUE:
         // Handlle only for data types integer and string
         switch ( lpDomainData->cType )
         {
            case zTYPE_STRING:
               nRC = 0;
               break;

            case zTYPE_INTEGER:
               zltoa( *((zPLONG) lpDomainData->lpData), szWorkString, sizeof( szWorkString ) );
               strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szWorkString );
               nRC = 1; // indicate returning a string value
               break;

            default:
               nRC = -2;
         }

         break;

      default:
         return( zDME_NOT_HANDLED );
   }

   return( nRC );
}

/*
   Deletes an operation from a source file.  Note that we determine whether
   a source file is .C or .VML by looking at the last character in the file
   name.
   For VML we delete all lines after the previous 'END' in column 1 to the
   'END' in column 1 after the operation declaration.  This should take care
   of comments at the beginning of each operation.
   For .C files, we can only delete lines between the '{' and '}', including
   all lines before the '{' that begin in column 1.
   In general we delete the operation by reading each line of the source file
   and determining if it should be deleted.  If the line should not be deleted
   it is copied to a temp file.  If the line is to be deleted then it is not
   copied to the temp file.  At the end we delete the old file and rename the
   temp file to match the old file name.
*/
zOPER_EXPORT zSHORT OPERATION
DeleteOperationFromSourceFile( zVIEW  vSubtask,
                               zPCHAR pchOperName,
                               zPCHAR pchFileName )
{
#define LINE_CACHE_SIZE   200
#define CACHE_SIZE      10000

   zSHORT nRC = 0;
   zBOOL  bC_File;
   zBOOL  bOperationFound = FALSE;
   zLONG  fIn = -1;
   zLONG  fOut = -1;
   zLONG  lLineCount;
   zSHORT nLineIdx;
   zCHAR  szTempFileName[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pch;
   zPCHAR pchLine;
   zSHORT nOperNameLth;
   zPCHAR pchBufferPtr[ LINE_CACHE_SIZE ];
   zLONG  hMem;
   zPCHAR pchBuffer = 0;
   zPCHAR pchBufferEnd;
   zSHORT nBufferIdx;

   bC_File = (ztoupper( pchFileName[ zstrlen( pchFileName ) - 1 ] ) == 'C');

   fIn = SysOpenFile( vSubtask, pchFileName, COREFILE_READ );
   if ( fIn < 0 )
   {
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

   // Create temp file name and open it.
   strcpy_s( szTempFileName, sizeof( szTempFileName ), pchFileName );
   pch = &szTempFileName[ zstrlen( szTempFileName ) - 1 ];
   while ( *pch != '.' )
      pch--;

   strcpy_s( pch, sizeof( szTempFileName ) - zstrlen( szTempFileName ), ".TMP" );
   fOut = SysOpenFile( vSubtask, szTempFileName, COREFILE_WRITE );
   if ( fOut < 0 )
   {
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

   // Try to allocate the line storage buffer.
   hMem = SysAllocMemory( &pchBuffer, CACHE_SIZE, 0, zCOREMEM_ALLOC, 0 );
   if ( pchBuffer == 0 )
   {
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

   // Read each line of the old file.  Depending on conditions we do the
   // following:
   //
   // o  If we haven't already found the operation name, then store the file
   //    in a temp buffer.  We do this because when we do find the oper name
   //    we will need to back up in the source file and delete the lines
   //    immediatly before the oper name.  This allows us to delete the
   //    operation declaration (e.g. "zOPER_EXPORT zSHORT OPERATION") and any comments that
   //    precede the operation declaration.  Once we have found the oper name
   //    we can go back through the buffer to delete previous lines.
   //
   //    We then check to see if the line contains the operation name starting
   //    in the first column.  If it does, then we assume that this declares
   //    the operation name.  We skip writing all lines until we find the end
   //    of the operation.
   //
   // o  If we already found the operation name, then we can just write the
   //    line from the old file to the temp file.  It as assumed at this point
   //    that the operation has been skipped over.

   nOperNameLth = (zSHORT) zstrlen( pchOperName );
   lLineCount   = 0;
   nLineIdx     = 0;
   nBufferIdx   = 0;
   pchBufferEnd = pchBuffer;
   while ( SysReadLine( vSubtask, &pchLine, fIn ) == 1 )
   {
      zSHORT nLineLth;
      zLONG  lLastLineWritten = 0;

      // If we've already found the operation, then it has been skipped over
      // so just write the line out to the temp file
      if ( bOperationFound )
      {
         if ( SysWriteLine( vSubtask, fOut, pchLine ) == zCALL_ERROR )
         {
            nRC = zCALL_ERROR;
            goto EndOfFunction;
         }

         continue;
      }

      lLineCount++;
      nLineIdx = (zSHORT) (lLineCount % LINE_CACHE_SIZE);

      //
      // Store the last LINE_CACHE_SIZE lines in a temp buffer.
      //

      // First, if the number of lines we have read is greater than
      // LINE_CACHE_SIZE then we are about to overwrite a line already stored
      // in the buffer.  Before this happens, write it to the temp file.
      if ( lLineCount > LINE_CACHE_SIZE )
      {
         if ( SysWriteLine( vSubtask, fOut, pchBufferPtr[ nLineIdx ] ) == zCALL_ERROR )
         {
            nRC = zCALL_ERROR;
            goto EndOfFunction;
         }

         lLastLineWritten = lLineCount - LINE_CACHE_SIZE;
      }

      // Check to see if there is enough room in the buffer to store the new line.
      nLineLth = (zSHORT) zstrlen( pchLine ) + 1;
      if ( pchBufferEnd + nLineLth > pchBuffer + CACHE_SIZE )
      {
         // The new line can't fit at the end of the buffer so put it at the beginning of the buffer.
         pchBufferEnd = pchBuffer;
      }

      // Store the line.
      pchBufferPtr[ nLineIdx ] = pchBufferEnd;
      strcpy_s( pchBufferEnd, CACHE_SIZE - (pchBufferEnd - pchBuffer), pchLine );
      pchBufferEnd += nLineLth;

      // Right now we only care about lines that begin in the first column.
      // If this line doesn't, then get the next line.
      if ( *pchLine == ' ' )
         continue;

      // If this line doesn't declare the operation, get the next line.
      if ( zstrncmp( pchOperName, pchLine, nOperNameLth ) != 0 )
         continue;

      // We have found the line that starts the operation.  We now want to
      // back up in our buffer to the end of the previous operation.  We will
      // then set lLineIdx, lLineCount, and pchBufferEnd to skip everything
      // after the end of the previous operation.

      bOperationFound = TRUE;

      // If this is the first line in the file then don't try to back up in
      // the buffer.
      if ( lLineCount > 1 )
      {
         zLONG  lTempLineCount = lLineCount - 1;
         zSHORT nTempIdx = (zSHORT) (lTempLineCount % LINE_CACHE_SIZE);

         // Look for the last line of the previous operation.  In C, look for
         // a '}'.  In VML, look for an 'END'.  Either way it must be in the
         // first column.
         while ( lTempLineCount > 0 )
         {
            if ( bC_File )
            {
               if ( *pchBufferPtr[ nTempIdx ] == '}' )
               {
                  break;
               }
            }
            else
            {
               // Check for 'END' followed by a whitespace, '/', or 0.
               if ( zstrncmpi( pchBufferPtr[ nTempIdx ], "END", 3 ) == 0 &&
                    (isspace( *( pchBufferPtr[ nTempIdx ] + 3 )) ||
                      *(pchBufferPtr[ nTempIdx ] + 3 ) == 0 ||
                      *(pchBufferPtr[ nTempIdx ] + 3 ) == '/') )
               {
                  break;
               }
            }

            // End not found, so back up another line.
            lTempLineCount--;
            nTempIdx = (zSHORT) (lTempLineCount % LINE_CACHE_SIZE);

            // We keep track of the last LINE_CACHE_SIZE lines.  It's possible
            // that this isn't enough.  In that case we'll just skip the line
            // immediately in front of the operation declaration.
            if ( lTempLineCount <= lLineCount - LINE_CACHE_SIZE )
            {
               // Set lTempLineCount to 2 less the lLineCount -- this means
               // that we assume that the end of the previous operation is
               // two lines in front of the current operation declaration.
               lTempLineCount = lLineCount - 2;
               break;  // Stop backing up.
            }
         } // while ( lTempLineCount > 0 )...

         // At this point lTempLineCount is the line number of the line that
         // ends the previous operation.

         // If lTempLineCount is 0, then no operation was found in front of
         // the operation we're deleting.  In VML this is OK--we'll delete
         // everything in front of the file.  For C we have a problem.  How
         // can we delete all the operation stuff (declaration and comments)
         // but not delete any important stuff (like #includes)?
         if ( lTempLineCount > 0 )
         {
            // We want to write the last line of the previous operation.
            lTempLineCount++;
         }
         else
         if ( lTempLineCount == 0 && bC_File )
         {
            // For right now we'll just delete the line before the operation.
            lTempLineCount = lLineCount - 1;
         }

         // lLastLineWritten is the line number of the last line written to
         // the temp file.  Write all the lines between lLastLineWritten and
         // lTempLineCount to the temp file.
         for ( lLastLineWritten++;
               lLastLineWritten < lTempLineCount;
               lLastLineWritten++ )
         {
            nTempIdx = (zSHORT) ( lLastLineWritten % LINE_CACHE_SIZE );
            if ( SysWriteLine( vSubtask, fOut, pchBufferPtr[ nTempIdx ] ) == zCALL_ERROR )
            {
               nRC = zCALL_ERROR;
               goto EndOfFunction;
            }
         }

      } // if ( lLineCount > 1 )

      // We've written all the lines in the temp buffer that need to be
      // written. Now skip over the lines in the source file until we get
      // to the end of the operation.
      while ( SysReadLine( vSubtask, &pchLine, fIn ) == 1 )
      {
         // Look for the end of the operation.
         if ( bC_File )
         {
            if ( *pchLine == '}' )
            {
               break;
            }
         }
         else
         {
            // Check for 'END' followed by a whitespace, '/', or 0.
            if ( zstrncmpi( pchLine, "END", 3 ) == 0 && (isspace( *(pchLine + 3))  ||
                   *(pchLine + 3) == 0 || *(pchLine + 3) == '/' ) )
            {
               break;
            }
         }
      } // while ( SysReadLine( &pchLine, fIn ) == 1 )...

   } // while ( SysReadLine( &pchLine, fIn ) == 1 )...

   if ( !bOperationFound )
   {
      // Print an error message re-using pchBuffer.
      sprintf_s( pchBuffer, CACHE_SIZE, "Couldn't find the operation '%s' in the source "
                 "file %s", pchOperName, pchFileName );
      MessageSend( vSubtask, "TZLOE202", "Operation Error",
                   pchBuffer, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      nRC = zCALL_ERROR;
   }
   else
   {
      // Delete the previous file and rename the new file.
      SysCloseFile( vSubtask, fIn, 0 );
      SysCloseFile( vSubtask, fOut, 0 );
      fIn = fOut = -1;

      SysOpenFile( vSubtask, pchFileName, COREFILE_DELETE );

      strcpy_s( pchBuffer, CACHE_SIZE, szTempFileName );
      strcpy_s( &pchBuffer[ zstrlen( pchBuffer ) + 1 ], CACHE_SIZE - zstrlen( pchBuffer ) - 1, pchFileName );
      SysOpenFile( vSubtask, pchBuffer, COREFILE_RENAME );
   }

EndOfFunction:
   if ( pchBuffer )
      SysFreeMemory( hMem );

   if ( fIn != -1 )
      SysCloseFile( vSubtask, fIn, 0 );

   if ( fOut != -1 )
      SysCloseFile( vSubtask, fOut, 0 );

   return( nRC );

} // DeleteOperationFromSourceFile

// Following is a hack to allow VML to call SysGetTickCount.
zOPER_EXPORT zLONG OPERATION
zGetTickCount( zLONG lDummy )
{
   lDummy = lDummy;
   return( SysGetTickCount() );
}

static void
fnLOD_DialogListBuildAttrControl( zVIEW  vXfer,
                                  zVIEW  vDialog,
                                  zVIEW  vControl )
{
   // If there is attribute mapping (the CtrlMapER_Attribute
   // exists, also add the Control to the attribute side of Xfer.
   if ( CheckExistenceOfEntity( vControl, "CtrlMapER_Attribute" ) >= zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vXfer,    "ER_Attribute",        "ZKey",
                                  vControl, "CtrlMapER_Attribute", "ZKey", "Root" );

      CreateEntity( vXfer, "AttrControl", zPOS_AFTER );
      SetAttributeFromAttribute( vXfer,    "AttrControl", "Tag",
                                 vControl, "Control",     "Tag" );
      SetAttributeFromAttribute( vXfer,    "AttrControl", "DialogName",
                                 vDialog,  "Dialog",      "Tag" );
      SetAttributeFromAttribute( vXfer,    "AttrControl", "WindowName",
                                 vDialog,  "Window",      "Tag" );
   }
}

static void
fnLOD_DialogListBuildDlgCtrl( zVIEW  vXfer )
{

   zLONG lZKey = 0;
   zSHORT nRC;

   // Add the DialogControl entity under the ER_Entity
   GetIntegerFromAttribute( &lZKey, vXfer, "Control", "LOD_EntityZKey" );
   nRC = SetCursorFirstEntityByInteger( vXfer, "LOD_Entity", "ZKey",
                                        lZKey, "Root" );
   if ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vXfer, "DialogControl", zPOS_AFTER );
      SetAttributeFromAttribute( vXfer, "DialogControl", "DialogName",
                                 vXfer, "Dialog",        "Tag" );
      SetAttributeFromAttribute( vXfer, "DialogControl", "WindowName",
                                 vXfer, "Control",       "WindowName" );
      SetAttributeFromAttribute( vXfer, "DialogControl", "ControlName",
                                 vXfer, "Control",       "Tag" );
      SetAttributeFromAttribute( vXfer, "DialogControl", "RegisteredViewName",
                                 vXfer, "ViewObjRef",    "Name" );
      SetAttributeFromAttribute( vXfer, "DialogControl", "LOD_Name",
                                 vXfer, "LOD",           "Name" );
      SetAttributeFromAttribute( vXfer, "DialogControl", "LOD_EntityName",
                                 vXfer, "Control",       "LOD_EntityName" );
      SetAttributeFromAttribute( vXfer, "DialogControl", "LOD_AttributeName",
                                 vXfer, "Control",       "LOD_AttributeName" );
   }
}

static void
fnLOD_DialogListCreateControl( zVIEW vXfer,
                               zVIEW vDialog,
                               zVIEW vControl )
{
   zSHORT nRC;

   // View vControl is now positioned on a Control entity.

   // What we want to do here is add a Control entry to the Xfer
   // under the appropriate ViewObjRef/Dialog path for each
   // ViewObjRef (CtrlMapView) entry under the Control.

   // To accomplish this, we will loop through each CtrlMapView
   // entity within the Control, creating the appropriate Control
   // entry in the Xfer.

   for ( nRC = SetCursorFirstEntity( vControl, "CtrlMapView", "Control" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vControl, "CtrlMapView", "Control" ) )
   {
      if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) >= zCURSOR_SET )
      {
         // To position on the correct LOD/ViewObjRef/Dialog entry,
         // we position go through the:
         //       vControl.CtrlMapView to the
         //       vControl.ViewObjRef (for the LOD)
         if ( SetCursorFirstEntityByAttr( vXfer,    "ViewObjRef",  "ZKey",
                                          vControl, "CtrlMapView", "ZKey",
                                          "Root" ) >= zCURSOR_SET )
         {
            nRC = SetCursorFirstEntityByAttr( vXfer,    "Dialog", "ZKey",
                                              vDialog,  "Dialog", "ZKey", 0 );
            if ( nRC < zCURSOR_SET )
            {
               // If a Dialog is not in list, add it.
               IncludeSubobjectFromSubobject( vXfer,   "Dialog",
                                              vDialog, "Dialog", zPOS_AFTER );
            }

            CreateEntity( vXfer, "Control", zPOS_AFTER );
            SetAttributeFromAttribute( vXfer,    "Control", "Tag",
                                       vControl, "Control", "Tag" );
            SetAttributeFromAttribute( vXfer,    "Control", "WindowName",
                                       vDialog,  "Window",  "Tag" );

            if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Attribute" ) >= zCURSOR_SET )
            {

               SetAttributeFromAttribute( vXfer,    "Control",              "Work",
                                          vControl, "CtrlMapLOD_Attribute", "Work" );
               if ( CheckExistenceOfEntity( vControl, "CtrlMapER_Attribute" ) >= zCURSOR_SET )
               {
                  SetAttributeFromAttribute( vXfer,    "Control",             "ER_AttributeZKey",
                                             vControl, "CtrlMapER_Attribute", "ZKey" );
                  SetAttributeFromAttribute( vXfer,    "Control",             "LOD_AttributeName",
                                             vControl, "CtrlMapER_Attribute", "Name" );
               }

               if ( CheckExistenceOfEntity( vControl, "CtrlMapRelatedEntity" ) >= zCURSOR_SET )
               {
                  SetAttributeFromAttribute( vXfer,    "Control",              "LOD_EntityZKey",
                                             vControl, "CtrlMapRelatedEntity", "ZKey" );
                  SetAttributeFromAttribute( vXfer,    "Control",              "LOD_EntityName",
                                             vControl, "CtrlMapRelatedEntity", "Name" );
               }
            }
            else
            {
               if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) >= zCURSOR_SET )
               {
                 SetAttributeFromAttribute( vXfer,    "Control",           "LOD_EntityZKey",
                                            vControl, "CtrlMapLOD_Entity", "ZKey" );
                 SetAttributeFromAttribute( vXfer,    "Control",           "LOD_EntityName",
                                            vControl, "CtrlMapLOD_Entity", "Name" );
               }
            }

            fnLOD_DialogListBuildAttrControl( vXfer, vDialog, vControl );
            fnLOD_DialogListBuildDlgCtrl( vXfer );
         }
      }
   }
}

zOPER_EXPORT zSHORT OPERATION
LOD_DialogListBuildControl( zVIEW  vSubtask,
                            zVIEW  vOrigDialog,
                            zVIEW  vOrigXfer )
{
   // This routine loops through all the Controls in the Dialog and builds
   // a cross reference Control record for each Control That is mapped to
   // the ViewObjRef that has current position.

   zVIEW  vDialogHier;
   zVIEW  vDialog;
   zVIEW  vXfer;
   zVIEW  vControl;
   zSHORT nRC;
   zSHORT lLevel;
   zCHAR  szEntityName[ 33 ];

   // Use temporary views of original Dialog and Xfer views so as not
   // to alter cursor positions of those views.
   CreateViewFromViewForTask( &vDialog, vOrigDialog, 0 );
   CreateViewFromViewForTask( &vXfer, vOrigXfer, 0 );

   // Loop through all Windows.
   for ( nRC = SetCursorFirstEntity( vDialog, "Window", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, "Window", 0 ) )
   {
      CreateViewFromViewForTask( &vDialogHier, vDialog, 0 );

      DefineHierarchicalCursor( vDialogHier, "Window" );
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lLevel,
                                             szEntityName, vDialogHier );
      // Loop hierarchically through all Controls and CtrlCtrls for this
      // Window.
      while ( nRC >= zCURSOR_SET && lLevel > 3 ||
              zstrcmp( szEntityName, "Control" ) == 0 )
      {
         if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         {
            SetViewToSubobject( vDialogHier, "CtrlCtrl" );
         }

         if ( zstrcmp( szEntityName, "Control" ) == 0 ||
              zstrcmp( szEntityName, "CtrlCtrl" ) == 0 )
         {
            CreateViewFromViewForTask( &vControl, vDialogHier, 0 );

            DropHierarchicalCursor( vControl );
            fnLOD_DialogListCreateControl( vXfer, vDialog, vControl );

            DropView( vControl );
         }

         nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lLevel,
                                                szEntityName, vDialogHier );
      }

      DropView( vDialogHier );
   }

   DropView( vDialog );
   DropView( vXfer );

   return( 0 );
}

static void
fnXrefLOD_CreateRelationship( zVIEW   vMeta,
                              zVIEW   vXfer,
                              zVIEW   vERD,
                              zPCHAR  lpLOD_Name )
{
   zVIEW  vMeta2;
   zVIEW  vXfer2;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zSHORT nRC;

   // Add Relationship information.
   for ( nRC = SetCursorFirstEntity( vMeta, "ER_RelLinkRec", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMeta, "ER_RelLinkRec", 0 ) )
   {
      nRC = SetCursorFirstEntityByAttr( vXfer, "ER_Entity",    "ZKey",
                                        vMeta, "ER_EntityRec", "ZKey", 0 );
      nRC = SetCursorFirstEntityByAttr( vXfer, "ER_RelType",    "ZKey",
                                        vMeta, "ER_RelTypeRec", "ZKey", 0 );
      if ( nRC < zCURSOR_SET )
      {
         zCHAR szRelLinkName[ 33 ];
         zCHAR szTargetEntityName[ 33 ];

         // We couldn't find a match by ZKey from the LOD, so let's try
         // to position in the ERD by name and then try by ZKey.
         CreateViewFromViewForTask( &vMeta2, vMeta, 0 );
         GetStringFromAttribute( szRelLinkName, sizeof( szRelLinkName ), vMeta2,
                                 "ER_RelLinkRec", "Name" );
         ResetViewFromSubobject( vMeta2 );
         GetStringFromAttribute( szTargetEntityName, sizeof( szTargetEntityName ), vMeta2,
                                 "ER_EntityRec", "Name" );
         DropView( vMeta2 );

         nRC = SetCursorFirstEntityByAttr( vERD,  "ER_Entity",    "Name",
                                           vMeta, "ER_EntityRec", "Name", 0 );
         for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vERD, "ER_RelLink", 0 ) )
         {
            if ( CompareAttributeToString( vERD, "ER_RelLink_Other", "Name",
                                           szRelLinkName ) == 0 &&
                 CompareAttributeToString( vERD, "ER_Entity_Other", "Name",
                                           szTargetEntityName ) == 0 )
            {
               break;
            }
         }

         if ( nRC >= zCURSOR_SET )
         {
            nRC = SetCursorFirstEntityByAttr( vXfer, "ER_RelType",   "ZKey",
                                              vERD,  "ER_RelType_1", "ZKey", 0 );
         }
      } // endif ( nRC < zCURSOR_SET )

      if ( nRC < zCURSOR_SET )
      {
         // We couldn't find the relationship in vXfer, so it must have been
         // changed in the ERD but not saved in the LOD.
         strcpy_s( szMsg, sizeof( szMsg ), "A relationship in LOD, " );
         strcat_s( szMsg, sizeof( szMsg ), lpLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), ", has been altered in the ERD.\nSave the LOD " );
         strcat_s( szMsg, sizeof( szMsg ), "and rerun the report to make sure it is accurate." );
         MessageSend( vMeta, "LO00106", "Zeidon Tools",
                      szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );
      }
      else
      {
         // The LOD_EntityParent entity that we are positioned on is
         // the child entity of the relationship in this LOD. Set up
         // vMeta2 to position on the parent entity of the relationship
         // in this LOD.
         CreateViewFromViewForTask( &vMeta2, vMeta, 0 );
         ResetViewFromSubobject( vMeta2 );
         CreateViewFromViewForTask( &vXfer2, vXfer, 0 );

         // Position on Entity/Rel in ER to get "Target" Entity.
         nRC = SetCursorFirstEntityByAttr( vERD, "ER_Entity", "ZKey",
                                           vMeta, "ER_EntityRec", "ZKey", 0 );
         nRC = SetCursorFirstEntityByAttr( vERD, "ER_RelType_1", "ZKey",
                                           vMeta, "ER_RelTypeRec", "ZKey",
                                           "ER_Entity" );
         nRC = SetCursorFirstEntityByAttr( vXfer2, "ER_Entity", "ZKey",
                                           vERD, "ER_Entity_Other", "ZKey", 0 );
         nRC = SetCursorFirstEntityByAttr( vXfer2, "ER_RelType", "ZKey",
                                           vMeta, "ER_RelTypeRec", "ZKey", 0 );

         // Create the cross reference entry under source Entity.
         CreateEntity( vXfer, "LOD_EntityRel", zPOS_AFTER );
         SetAttributeFromAttribute( vXfer, "LOD_EntityRel", "LOD_SourceEntityName",
                                    vMeta, "LOD_EntityParent", "Name" );
         SetAttributeFromAttribute( vXfer, "LOD_EntityRel", "LOD_TargetEntityName",
                                    vMeta2, "LOD_EntityParent", "Name" );
         SetAttributeFromString( vXfer, "LOD_EntityRel", "LOD_Name", lpLOD_Name );
         SetCursorFirstEntityByString( vXfer2, "LOD", "Name", lpLOD_Name, "" );
         SetAttributeFromAttribute( vXfer, "LOD_EntityRel", "LOD_Desc",
                                    vXfer2, "LOD", "Desc" );
         SetAttributeFromString( vXfer, "LOD_EntityRel",
                                 "LOD_RelationshipDirection", "is child of" );

         // Create the cross reference entry under target Entity.
         CreateEntity( vXfer2, "LOD_EntityRel", zPOS_AFTER );
         SetAttributeFromAttribute( vXfer2, "LOD_EntityRel", "LOD_SourceEntityName",
                                    vMeta2, "LOD_EntityParent", "Name" );
         SetAttributeFromAttribute( vXfer2, "LOD_EntityRel", "LOD_TargetEntityName",
                                    vMeta, "LOD_EntityParent", "Name" );
         SetAttributeFromString( vXfer2, "LOD_EntityRel", "LOD_Name", lpLOD_Name );
         SetAttributeFromString( vXfer2, "LOD_EntityRel",
                                 "LOD_RelationshipDirection", "is parent of" );
         SetAttributeFromAttribute( vXfer2, "LOD_EntityRel", "LOD_Desc",
                                    vXfer2, "LOD", "Desc" );
         DropView( vMeta2 );
         DropView( vXfer2 );
      }
   }
}

zOPER_EXPORT zSHORT OPERATION
fnRecursiveXrefLOD_Entity( zVIEW   vMeta,
                           zVIEW   vXfer,
                           zVIEW   vERD,
                           zPCHAR  lpLOD_Name,
                           zPCHAR  lpWorkFlag,
                           zPCHAR  lpRootFlag )
{
   // This Operation is called recursively and processes relationship xref
   // information for each LOD_Entity.
   zSHORT nRC;

   // Process each LOD_EntityParent subobject.
   for ( nRC = SetCursorFirstEntity( vMeta, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMeta, "LOD_EntityParent", 0 ) )
   {
      nRC = CheckExistenceOfEntity( vMeta, "ER_EntityRec" );
      if ( nRC >= 0 )
         nRC = SetCursorFirstEntityByAttr( vXfer, "ER_Entity",    "ZKey",
                                           vMeta, "ER_EntityRec", "ZKey", 0 );

      if ( nRC >= zCURSOR_SET )
      {
         // Create LOD Entity
         CreateEntity( vXfer, "LOD_Entity", zPOS_AFTER );
         SetAttributeFromString( vXfer, "LOD_Entity", "LOD_Name", lpLOD_Name );
         SetAttributeFromString( vXfer, "LOD_Entity", "WorkLOD_Flag", lpWorkFlag );

         SetAttributeFromAttribute( vXfer, "LOD_Entity",       "Name",
                                    vMeta, "LOD_EntityParent", "Name" );
         SetAttributeFromAttribute( vXfer, "LOD_Entity",       "ZKey",
                                    vMeta, "LOD_EntityParent", "ZKey" );

         // Set Usage
         if ( CompareAttributeToString( vMeta, "LOD_EntityParent", "Create", "Y" ) == 0 )
            SetAttributeFromString( vXfer, "LOD_Entity", "CreateFlag", "X" );
         if ( CompareAttributeToString( vMeta, "LOD_EntityParent", "Update", "Y" ) == 0 )
            SetAttributeFromString( vXfer, "LOD_Entity", "UpdateFlag", "X" );
         if ( CompareAttributeToString( vMeta, "LOD_EntityParent", "Delete", "Y" ) == 0 )
            SetAttributeFromString( vXfer, "LOD_Entity", "DeleteFlag", "X" );
         if ( CompareAttributeToString( vMeta, "LOD_EntityParent", "Include", "Y" ) == 0 )
            SetAttributeFromString( vXfer, "LOD_Entity", "IncludeFlag", "X" );
         if ( CompareAttributeToString( vMeta, "LOD_EntityParent", "Exclude", "Y" ) == 0 )
            SetAttributeFromString( vXfer, "LOD_Entity", "ExcludeFlag", "X" );

         SetAttributeFromAttribute( vXfer, "LOD_Entity",       "ParentDeleteBehave",
                                    vMeta, "LOD_EntityParent", "ParentDeleteBehave" );
         if ( *lpRootFlag == 'Y' )
         {
            SetAttributeFromString( vXfer, "LOD_Entity", "RootFlag", "X" );
         }

         fnXrefLOD_CreateRelationship( vMeta, vXfer, vERD, lpLOD_Name );
      }

      // Process each LOD_Entity subobject.
      SetViewToSubobject( vMeta, "LOD_EntityChild" );
      fnRecursiveXrefLOD_Entity( vMeta, vXfer, vERD, lpLOD_Name, lpWorkFlag, "N" );
      ResetViewFromSubobject( vMeta );
   }

   return( 0 );
}

static void
fnLOD_DialogList_AddMeta( zVIEW  vSubtask,
                          zVIEW  vXfer,
                          zVIEW  vERD,
                          zVIEW  vMeta,
                          zPCHAR pchMetaName,
                          zPCHAR pchSuffix )
{
   zSHORT nRC;
   zCHAR  szWorkFlag[ 2 ];

   if ( zstrcmp( pchSuffix, "LOD" ) == 0 )
   {
      // *** LOD ***

      // If a LOD, add it to the cross reference list.
      CreateEntity( vXfer, "LOD", zPOS_AFTER );
      SetAttributeFromAttribute( vXfer, "LOD", "Name",
                                 vMeta, "LOD", "Name" );
      SetAttributeFromAttribute( vXfer, "LOD", "ZKey",
                                 vMeta, "LOD", "ZKey" );
      SetAttributeFromAttribute( vXfer, "LOD", "Desc",
                                 vMeta, "LOD", "Desc" );

      // Process each LOD_Entity subobject, starting with the
      // LOD_EntityParent entity.
      if ( CheckExistenceOfEntity( vMeta, "POD" ) < zCURSOR_SET )
         strcpy_s( szWorkFlag, sizeof( szWorkFlag ), "X" );
      else
         strcpy_s( szWorkFlag, sizeof( szWorkFlag ), "" );

      fnRecursiveXrefLOD_Entity( vMeta, vXfer, vERD, pchMetaName, szWorkFlag, "Y" );
   }
   else
   if ( zstrcmp( pchSuffix, "PVR" ) == 0 )
   {
      // *** Registered View ***

      // If a registered view, add it to the cross reference list.
      if ( SetCursorFirstEntityByAttr( vXfer, "LOD", "Name",
                                       vMeta, "LOD", "Name", 0 ) >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vXfer, "ViewObjRef",
                                        vMeta, "ViewObjRef", zPOS_AFTER );
      }
   }
   else
   if ( zstrcmp( pchSuffix, "PWD" ) == 0 )
   {
      // *** Dialog ***

      for ( nRC = SetCursorFirstEntity( vMeta, "ViewObjRef", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vMeta, "ViewObjRef", 0 ) )
      {
         if ( SetCursorFirstEntityByAttr( vXfer, "LOD", "ZKey",
                                          vMeta, "LOD", "ZKey", 0 ) >= zCURSOR_SET )
         {
            if ( SetCursorFirstEntityByAttr( vXfer, "ViewObjRef", "ZKey",
                                             vMeta, "ViewObjRef", "ZKey", 0 )
                                                               < zCURSOR_SET )
            {
               // If a VOR is not in list, add it.
               IncludeSubobjectFromSubobject( vXfer, "ViewObjRef",
                                              vMeta, "ViewObjRef", zPOS_AFTER );
            }

            if ( SetCursorFirstEntityByAttr( vXfer, "Dialog", "Tag",
                                             vMeta, "Dialog", "Tag", 0 )
                                                           < zCURSOR_SET )
            {
               // If a Dialog is not in list, add it.
               IncludeSubobjectFromSubobject( vXfer, "Dialog",
                                              vMeta, "Dialog", zPOS_AFTER );
            }
         }
      }

      // Call the routine to format Control information.
      LOD_DialogListBuildControl( vSubtask, vMeta, vXfer );
   }
}

zOPER_EXPORT zSHORT OPERATION
LOD_DialogListActivateMeta( zVIEW  vSubtask,
                            zVIEW  vTaskLPLR,
                            zPVIEW pvMeta,
                            zPCHAR pchType,
                            zPCHAR pchMetaName,
                            zPCHAR pchSuffix )
{
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szFullName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   HFILE  hFile;

   if ( vTaskLPLR == 0 )
      return( -1 );

   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szFullName, sizeof( szFullName ), szFileName );
   strcat_s( szFullName, sizeof( szFullName ), "\\" );
   strcat_s( szFullName, sizeof( szFullName ), pchMetaName );
   strcat_s( szFullName, sizeof( szFullName ), "." );
   strcat_s( szFullName, sizeof( szFullName ), pchSuffix );

   hFile = SysOpenFile( vTaskLPLR, szFullName, COREFILE_READ );
   if ( hFile >= 0 )
   {
      SysCloseFile( vTaskLPLR, hFile, 0 );
      if ( ActivateOI_FromFile( pvMeta, pchType, vSubtask, szFullName,
                                zSINGLE | zIGNORE_ERRORS | zLEVEL_SYSTEM ) < 0 )
      {
         TraceLineS( "Error reading file: ", szFullName );
         strcpy_s( szMsg, sizeof( szMsg ), "Error in reading Meta: " );
         strcat_s( szMsg, sizeof( szMsg ), pchMetaName );
         MessageSend( vSubtask, "ER00190", "E/R Model Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         return( 1 );
      }
   }
   else
   {
      TraceLineS( "Error reading file: ", szFullName );
      strcpy_s( szMsg, sizeof( szMsg ), "Error in reading Meta: " );
      strcat_s( szMsg, sizeof( szMsg ), pchMetaName );
      MessageSend( vSubtask, "ER00190", "E/R Model Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( -1 );
}

// This operation is called by LOD_DialogList. It is reusable routine that
// actually creates the Xfer entries for LODs, VORs and Dialogs.
zOPER_EXPORT zSHORT OPERATION
LOD_DialogListSub( zVIEW  vSubtask,
                   zPCHAR pchSuffix,
                   zVIEW  vTaskLPLR,
                   zVIEW  vXfer,
                   zVIEW  vERD )
{
   zVIEW  vMeta = 0;
   zCHAR  szMetaName[ 33 ];
   zCHAR  szType[ 33 ];
   zULONG ulZKeyVOR;
   zSHORT nRC;

   // Position on correct meta type in LPLR.
   if ( zstrcmp( pchSuffix, "LOD" ) == 0 )
   {
      SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type",
                                     zREFER_LOD_META, 0 );
      strcpy_s( szType, sizeof( szType ), "TZZOLODO" );
   }
   else
   if ( zstrcmp( pchSuffix, "PVR" ) == 0 )
   {
      SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type",
                                     zREFER_VOR_META, 0 );
      strcpy_s( szType, sizeof( szType ), "TZWDVORO" );
   }
   else
   {
      SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type",
                                     zREFER_DIALOG_META, 0 );
      strcpy_s( szType, sizeof( szType ), "TZWDLGSO" );
   }

   // Loop through all Metas.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "W_MetaDef", 0 ) )
   {
      if ( zstrcmp( pchSuffix, "PVR" ) == 0 )
      {
         GetIntegerFromAttribute( (zPLONG) &ulZKeyVOR,
                                  vTaskLPLR, "W_MetaDef", "CPLR_ZKey" );
         zltoxa( ulZKeyVOR, szMetaName );
      }
      else
      {
         GetStringFromAttribute( szMetaName, sizeof( szMetaName ),
                                 vTaskLPLR, "W_MetaDef", "Name" );
      }

      if ( LOD_DialogListActivateMeta( vSubtask, vTaskLPLR, &vMeta, szType,
                                       szMetaName, pchSuffix ) > 0 )
      {
         fnLOD_DialogList_AddMeta( vSubtask, vXfer, vERD, vMeta,
                                   szMetaName, pchSuffix );
      }

      if ( vMeta )
      {
         DropView( vMeta );
         vMeta = 0;
      }
   }

   return( 0 );
}

// Do a search on the string attribute vView.szEntityName.szAttributeName
// by the string pchTextValue. Return a 0 if pchTextValue was found in the
// attribute value and a -1 if it was not found.
zOPER_EXPORT zSHORT OPERATION
FindTextInAttribute( zPCHAR pchTextValue,
                     zVIEW  vView,
                     zPCHAR pchEntityName,
                     zPCHAR pchAttributeName )
{
   zSHORT nRC;
   int    nPatternLth;
   zLONG  lDelta1Table[ ASCIISIZE ];
   zLONG  lDelta2Table[ MAXPATTERN ];
   zLONG  lBufferLth;
   zLONG  lFindPos;
   zPCHAR lpBuffer;

   nPatternLth = zstrlen( pchTextValue );
   if ( nPatternLth == 0 )
   {
      return( -1 );
   }

   // Builds a table to be used in qfind.
   GetAddrForAttribute( &lpBuffer, vView, pchEntityName, pchAttributeName );
   lBufferLth = zstrlen( lpBuffer );
   qbldelta( nPatternLth, pchTextValue, lDelta1Table, lDelta2Table );

   // Searches the buffer area.
   lFindPos = qfind( nPatternLth,
                     pchTextValue,
                     lBufferLth,          /* Buffer length */
                     lpBuffer,            /* Buffer text */
                     lDelta1Table,
                     lDelta2Table );

   if ( lFindPos == -1 )
      nRC = -1;
   else
      nRC = 0;

   return( nRC );
}

// Get the VMLConstChar flag from zeidon.ini
//  return( 1 ), if it is ON
zOPER_EXPORT zSHORT OPERATION
GetVMLConstChar( )
{
   static zSHORT nVMLConstChar = -1;
   zCHAR szTemp[ 8 ];
   zVIEW lpTaskView = GetDefaultViewForActiveTask( );

   if ( nVMLConstChar >= 0 )
      return( nVMLConstChar ); // read from .ini file once

   // get the VMLConstChar switch from zeidon.ini
   // default is off, will be later on changed to on (hopefully)
   nVMLConstChar = 0;
   szTemp[ 0 ] = 0;
   SysReadZeidonIni( -1, "[Zeidon]", "VMLConstChar", szTemp, sizeof( szTemp ) );

   if ( szTemp[ 0 ] && szTemp[ 0 ] != 'n' &&
        szTemp[ 0 ] != 'N' && szTemp[ 0 ] != '0' )
   {
      nVMLConstChar = 1;
   }

   return( nVMLConstChar );
}

zOPER_EXPORT zBOOL OPERATION
VerifyDir( zVIEW  vSubtask,
           zPCHAR pchDir,
           zPCHAR pchControlName,
           zBOOL  bCreate )
{
   zCHAR     szMsg[ 400 ];
   zCHAR     szTempPath[ 2 * zMAX_FILESPEC_LTH + 1 ];
   zSHORT    nRC;

   // Check to see if dir exists.
   SysConvertEnvironmentString( szTempPath, sizeof( szTempPath ), pchDir );
   if ( SysValidDirOrFile( szTempPath, TRUE, FALSE,
                           (zSHORT) (zstrlen( szTempPath ) + 1) ) == FALSE )
   {
      if ( zstrcmp( szTempPath, "" ) == 0 )
         return( fnDirectoryIsRequiredD( vSubtask, pchControlName, bCreate ));

      if ( !bCreate )
      {
         sprintf_s( szMsg, sizeof( szMsg ), "%s '%s' does not exist.\nIgnore error?",
                   pchControlName, szTempPath );
         if ( MessagePrompt( vSubtask, "ED1003", "Zeidon Compiler",
                             szMsg, TRUE, zBUTTONS_YESNO, zRESPONSE_NO,
                             zICON_QUESTION ) == zRESPONSE_YES )
         {
            return( TRUE );    // User wants to ignore the error.
         }

         SetCtrlState( vSubtask, pchControlName, zCONTROL_STATUS_ERROR, TRUE );
         SetFocusToCtrl( vSubtask, pchControlName );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( FALSE );
      }

      sprintf_s( szMsg, sizeof( szMsg ), "%s '%s' does not exist.\nDo you "
                "want to create it?", pchControlName, szTempPath );

      nRC = MessagePrompt( vSubtask, "ED1002", "Zeidon Compiler",
                           szMsg,
                           TRUE, zBUTTONS_YESNOCANCEL,
                           zRESPONSE_YES, zICON_QUESTION );

      // If user responed with "no", then we return OK.
      if ( nRC == zRESPONSE_CANCEL )
      {
         // User cancel'd the msg, so return error.
         SetCtrlState( vSubtask, pchControlName, zCONTROL_STATUS_ERROR, TRUE );
         SetFocusToCtrl( vSubtask, pchControlName );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( FALSE );
      }
      else
      if ( nRC == zRESPONSE_YES )
      {
         // Try to create dir.
         if ( !SysValidDirOrFile( szTempPath, TRUE, TRUE,
                                  (zSHORT) (zstrlen( szTempPath ) + 1) ) )
         {
            // Couldn't create the dir, so show error and exit.
            SetCtrlState( vSubtask, pchControlName, zCONTROL_STATUS_ERROR, TRUE );
            SetFocusToCtrl( vSubtask, pchControlName );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            sprintf_s( szMsg, sizeof( szMsg ), "%s '%s' could not be created.",
                      pchControlName, szTempPath );
            MessagePrompt( vSubtask, "ED1003", "Zeidon Compiler",
                           szMsg, TRUE, zBUTTONS_OK, 0, zICON_STOP );

            return( FALSE );
         }
      }
   }

   return( TRUE );

} // VerifyDir

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnSetupEventList
//
// PURPOSE:  This is an internal reusable function to set up the
//           Event entities in TZPNEVWO from the existing Events in
//           TZCONTROL.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSetupEventList( zVIEW vSubtask, zVIEW vDialog )
{
   zVIEW    vEvents;
   zVIEW    vEventsCopy;
   zLONG    lType;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Only perform loop if Events exist.
   if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET )
   {
      // Loop through all vEvents.Event and set the EventAct.Tag value
      // to vSubtask.EventAct.Tag, if the relationship exists, and to
      // null otherwise.
      CreateViewFromViewForTask( &vEventsCopy, vEvents, 0 );
      SetCursorFirstEntity( vEventsCopy, "Event", 0 );
      do
      {
         GetIntegerFromAttribute( &lType, vEventsCopy, "Event", "Type" );
         if ( SetCursorFirstEntityByInteger( vDialog, "Event", "Type",
                                             lType, 0 ) == zCURSOR_SET )
         {
            SetAttributeFromAttribute( vEventsCopy, "EventAct", "Tag",
                                       vDialog, "EventAct", "Tag" );
         }
         else
         if ( CheckExistenceOfEntity( vEventsCopy, "EventAct" ) == 0 )
         {
            SetAttributeFromString( vEventsCopy, "EventAct", "Tag", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "Type", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "DialogName", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "WindowName", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "Operation", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "SB_SubAction", "" );
         }

      } while ( SetCursorNextEntity( vEventsCopy,
                                     "Event", 0 ) > zCURSOR_UNCHANGED );
      DropView( vEventsCopy );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnSetupWndEventList
//
// PURPOSE:  This is an internal reusable function to set up the
//           WndEvent entities in TZPNEVWO from the existing Events in
//           TZWINDOW.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSetupWndEventList( zVIEW vSubtask )
{
   zVIEW    vEvents;
   zVIEW    vEventsTmp;
   zLONG    lType;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Use a temp view so as not to alter cursor position of the view
   // passed in.
   CreateViewFromViewForTask( &vEventsTmp, vEvents, 0 );

   // Loop through all vEventsTmp.Event and set the EventAct.Tag value to
   // vSubtask.EventAct.Tag, if the relationship exists, and to null otherwise.
   SetCursorFirstEntity( vEventsTmp, "Event", 0 );
   do
   {
      GetIntegerFromAttribute( &lType, vEventsTmp, "Event", "Type" );
      if ( SetCursorFirstEntityByInteger( vSubtask, "WndEvent", "Type",
                                          lType, 0 ) == zCURSOR_SET )
      {
         SetAttributeFromAttribute( vEventsTmp, "EventAct", "Tag",
                                    vSubtask, "WndAct", "Tag" );
      }
      else
         SetAttributeFromString( vEventsTmp, "EventAct", "Tag", "" );

   } while ( SetCursorNextEntity( vEventsTmp, "Event", 0 ) >
                                                      zCURSOR_UNCHANGED );
   DropView( vEventsTmp );

   return( 0 );
}

static zBOOL
fnDirectoryIsRequiredD( zVIEW  vSubtask,
                        zPCHAR pchControlName,
                        zBOOL  bCreate )
{
   zCHAR  szMsg[ 100 ];


   if ( bCreate == FALSE )
      return( TRUE );

   sprintf_s( szMsg, sizeof( szMsg ), "%s is required. \nIgnore error?", pchControlName );
   if ( MessagePrompt( vSubtask, "ED1003", "Zeidon Compiler",
                       szMsg, TRUE, zBUTTONS_YESNO, zRESPONSE_NO,
                       zICON_QUESTION ) == zRESPONSE_YES )
   {
      return( TRUE );    // User wants to ignore the error.
   }

   SetCtrlState( vSubtask, pchControlName, zCONTROL_STATUS_ERROR, TRUE );
   SetFocusToCtrl( vSubtask, pchControlName );
   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      fnCreateEmptyActionView
//
// PURPOSE:    Create an empty action view so when an action is removed
//             from a button or option the action mapping goes away.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
fnCreateEmptyActionView( zVIEW vSubtask, zVIEW vDialog )
{
   zVIEW  vDialogA;
   zSHORT nRC;

   nRC = GetViewByName( &vDialogA, "TZACTIONE", vSubtask, zLEVEL_TASK );
   if ( vDialogA == 0 )
   {
      ActivateEmptyObjectInstance( &vDialogA, 0, vDialog, zSINGLE );
      SetNameForView( vDialogA, "TZACTIONE", vSubtask, zLEVEL_TASK );
   }

   nRC = SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   return( 0 );
}

//./ ADD NAME=WL_QC
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: WL_QC
//
//  PURPOSE:    This routine Converts an instance of a special character in
//              a buffer and then writes out the buffer. The character to
//              be translated is szTransChar and any instance of it is
//              converted to a double quote.
//
//  PARAMETERS: lFile - File handle
//              pchBuffer - the string to be converted.
//              pchTransChar - The character to be converted to a quote.
//              nAddBlankLineCnt - Number of blank lines to append.
//
//  RETURNS:    0  - File delete successfully.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
WL_QC( zVIEW  vDialog,
       zLONG  lFile,
       zPCHAR pchBuffer,
       zPCHAR pchTransChar,
       zSHORT nBlankLineCnt )
{
   zPCHAR pch;

   for ( pch = pchBuffer; *pch; pch++ )
   {
      if ( *pch == *pchTransChar )
         *pch = '"';
   }

   SysWriteLine( vDialog, lFile, pchBuffer );
   while ( nBlankLineCnt-- > 0 )
      SysWriteLine( vDialog, lFile, "" );

   return( 0 );
}

//./ ADD NAME=GetStringFromControlBlob
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: GetStringFromControlBlob
//
//  PURPOSE:    This routine returns a string value for a C variable in
//              a Control Blob.
//              Note that it only handles special variables.
//
//  PARAMETERS: lpReturnString - The C value returned as a string.
//              vDialog -View to the Dialog, positioned on the BitMap control
//              lMaxLength - The max length of the string.
//              lpC_VariableName - The name of the C variable
//
//  RETURNS:    0  - File delete successfully.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetStringFromControlBlob( zPCHAR lpReturnString,
                          zVIEW  vDialog,
                          zLONG  lMaxLength,
                          zPCHAR lpC_VariableName )
{
   zPCHAR   pCtrlBOI;

   GetAddrForAttribute( &pCtrlBOI, vDialog, "Control", "CtrlBOI" );
   if ( pCtrlBOI )
   {
      if ( zstrcmp( lpC_VariableName, "BMP.szBMPUpOrIconName" ) == 0 )
      {
         zCTRLBOI_BMP BMP;
         zULONG ulLth;

         // get CtrlBOI into CtrlBOI_BMP structure
         GetAttributeLength( &ulLth, vDialog, "Control", "CtrlBOI" );
         SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );
         ulLth = zstrlen( BMP.szBMPUpOrIconName );
         if ( ulLth > (zULONG) lMaxLength )
            return( -1 );

         strcpy_s( lpReturnString, lMaxLength, (zPCHAR) BMP.szBMPUpOrIconName );
      }
   }

   if ( zstrcmp( lpC_VariableName, "zCONTROL_BORDEROFF" ) == 0 )
   {
      zLONG  lSubtype;

      // Return a "Y" if the Border has been set, otherwise return null.
      GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
      if ( (lSubtype & zCONTROL_BORDEROFF ) == 0 )
         strcpy_s( lpReturnString, lMaxLength, "Y" );
      else
         strcpy_s( lpReturnString, lMaxLength, "" );
   }

   if ( zstrcmp( lpC_VariableName, "zCONTROL_CENTERJUSTIFY" ) == 0 )
   {
      zLONG  lSubtype;

      // Return a "Y" if the Center Justify has been set, otherwise return null.
      GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
      if ( (lSubtype & zCONTROL_CENTERJUSTIFY ) == 0 )
         strcpy_s( lpReturnString, lMaxLength, "" );
      else
         strcpy_s( lpReturnString, lMaxLength, "Y" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zSHORT
fnIsComment( zCPCHAR cpcLine, zSHORT nComment )
{
   zSHORT k = 0;

   if ( nComment == 1 )
      nComment = 0;

   while ( cpcLine[ k ] )
   {
      if ( zisspace( cpcLine[ k ] ) )
         k++;
      else
      {
         if ( nComment == -1 )  // start of multi-line comment has been found
         {
            if ( cpcLine[ k ] == '*' && cpcLine[ k + 1 ] == '/' )
               return( 1 ); // end of comment, but is a comment line
            else
               k++; // keep looking for end of comment
         }
         else
         if ( cpcLine[ k ] == '/' && cpcLine[ k + 1 ] == '/' )
            return( 1 ); // single-line comment
         else
         if ( cpcLine[ k ] == '/' && cpcLine[ k + 1 ] == '*' )
         {
            nComment = -1; // multi-line comment
            k += 2;
         }
         else
            break;
      }
   }

   return( nComment );
}

zBOOL
fnLocateOperInLine( zCPCHAR cpcLine, zCPCHAR cpcOperationName )
{
   zSHORT nLth = (zSHORT) zstrlen( cpcOperationName );
   zSHORT nPos;
   zSHORT k = 0;

   while ( cpcLine[ k ] )
   {
      if ( zisspace( cpcLine[ k ] ) == FALSE )
      {
         if ( cpcLine[ k ] == '/' && cpcLine[ k + 1 ] == '/' )
            return( FALSE );

         nPos = 0;
         while ( nPos < nLth )
         {
            if ( cpcLine[ k ] == cpcOperationName[ nPos ] )
            {
               k++;
               nPos++;
            }
            else
               break;
         }

         if ( nPos == nLth && (cpcLine[ k ] == '(' || cpcLine[ k ] == 0 || zisspace( cpcLine[ k ] )) )
            return( TRUE );
      }

      k++;
   }

   return( FALSE );
}

zBOOL
fnLocateOperationInLine( zCPCHAR cpcLine )
{
   static zPCHAR cpcOperationName = "OPERATION";
   zSHORT nLth = (zSHORT) zstrlen( cpcOperationName );
   zSHORT nPos;
   zSHORT k = 0;
   zBOOL  bNeedSpace = FALSE;

   while ( cpcLine[ k ] )
   {
      if ( zisspace( cpcLine[ k ] ) )
         bNeedSpace = FALSE;
      else
      if ( bNeedSpace == FALSE )
      {
         bNeedSpace = TRUE;
         if ( cpcLine[ k ] == '/' && cpcLine[ k + 1 ] == '/' )
            return( FALSE );

         nPos = 0;
         while ( nPos < nLth )
         {
            if ( cpcLine[ k ] == cpcOperationName[ nPos ] )
            {
               k++;
               nPos++;
            }
            else
               break;
         }

         if ( nPos == nLth && (cpcLine[ k ] == 0 || cpcLine[ k ] == ',' || zisspace( cpcLine[ k ] )) )
            return( TRUE );
      }

      k++;
   }

   return( FALSE );
}

zSHORT
fnTestForKeyWord( zCPCHAR cpcLine, zBOOL *pbMultiLineComment )
{
#if 1
   zCHAR  szLine[ 4096 ];
   zCHAR  chSeparators[ ] = " ,\t\n";
   zPCHAR pchToken;
   zPCHAR pchLine;
   zPCHAR pchContext = 0;
   zSHORT nCnt = 0;
   zSHORT nLineLth;
   zSHORT nLth;

   nLineLth = (zSHORT) zstrlen( cpcLine ) + 1;
   if ( nLineLth >= sizeof( szLine ) )
   {
      TraceLineI( "CopyOperationCode Line greater than buffer size (4096) ... using malloc: ", nLineLth );
      pchLine = (zPCHAR) SysMalloc( nLineLth );
   }
   else
   {
      nLineLth = sizeof( szLine );
      pchLine = szLine;
   }

   // strtok modifies cpcLine ... so we copy it to a modifiable buffer!
   strcpy_s( pchLine, nLineLth, cpcLine );

   // Establish string and get the first token.
   pchToken = strtok_s( pchLine, chSeparators, &pchContext );

   while ( pchToken ) // while there are tokens in szLine
   {
      nLth = (zSHORT) zstrlen( pchToken );

      if ( *pbMultiLineComment )
      {
         if ( nLth >= 2 && pchToken[ nLth - 1 ] == '/' && pchToken[ nLth - 2 ] == '*' )
            *pbMultiLineComment = FALSE;
      }
      else
      {
         if ( nLth >= 2 && pchToken[ 0 ] == '/' && pchToken[ 1 ] == '*' )
            *pbMultiLineComment = TRUE;

         if ( nLth >= 4 && pchToken[ nLth - 1 ] == '/' && pchToken[ nLth - 2 ] == '*' )
            *pbMultiLineComment = FALSE;
      }

      if ( *pbMultiLineComment == FALSE )
      {
         if ( nLth >= 2 && pchToken[ 0 ] == '/' && pchToken[ 1 ] == '/' )
            return( nCnt );  // found a comment

         if ( nLth == sizeof( "END" ) - 1 && zstrncmp( pchToken, "END", nLth ) == 0 )
            nCnt--;
         else
         if ( nLth == sizeof( "IF" ) - 1 && zstrncmp( pchToken, "IF", nLth ) == 0 )
            nCnt++;
         else
         if ( nLth == sizeof( "FOR" ) - 1 && zstrncmp( pchToken, "FOR", nLth ) == 0 )
            nCnt++;
         else
         if ( nLth == sizeof( "LOOP" ) - 1 && zstrncmp( pchToken, "LOOP", nLth ) == 0 )
            nCnt++;
         else
         if ( nLth == sizeof( "CASE" ) - 1 && zstrncmp( pchToken, "CASE", nLth ) == 0 )
            nCnt++;
         else
         if ( nLth == sizeof( "LOOP" ) - 1 && zstrncmp( pchToken, "LOOP", nLth ) == 0 )
            nCnt++;
      }

      // Get next token.
      pchToken = strtok_s( 0, chSeparators, &pchContext );
   }

   if ( pchLine != szLine )
      SysFree( pchLine );

#else

   zSHORT nPos = 0;
   zSHORT nCnt = 0;
   zSHORT nLth = 0;

   while ( cpcLine[ nLth ] || nPos > 0 )
   {
      if ( cpcLine[ nLth ] == 0 || cpcLine[ nLth ] == ',' || zisspace( cpcLine[ nLth ] ) )
      {
         if ( nPos > 0 )
         {
            if ( *pbMultiLineComment )
            {
               if ( nLth >= 2 && pchToken[ nLth - 1 ] == '/' && pchToken[ nLth - 1 ] == '*' )
                  *pbMultiLineComment = FALSE;
            }
            else
            {
               if ( nLth >= 2 && pchToken[ 0 ] == '/' && pchToken[ 1 ] == '*' )
                  *pbMultiLineComment = TRUE;

               if ( nLth >= 4 && pchToken[ nLth - 1 ] == '/' && pchToken[ nLth - 1 ] == '*' )
                  *pbMultiLineComment = FALSE;
            }

            if ( *pbMultiLineComment == FALSE )
            {
               if ( nLth >= 2 && pchToken[ 0 ] == '/' && pchToken[ 1 ] == '/' )
                  return( nCnt );  // found a comment

               if ( nLth == sizeof( "END" ) - 1 && zstrncmp( cpcLine, "END", nLth ) == 0 )
                  nCnt--;
               else
               if ( nLth == sizeof( "IF" ) - 1 && zstrncmp( cpcLine, "IF", nLth ) == 0 )
                  nCnt++;
               else
               if ( nLth == sizeof( "FOR" ) - 1 && zstrncmp( cpcLine, "FOR", nLth ) == 0 )
                  nCnt++;
               else
               if ( nLth == sizeof( "LOOP" ) - 1 && zstrncmp( cpcLine, "LOOP", nLth ) == 0 )
                  nCnt++;
               else
               if ( nLth == sizeof( "CASE" ) - 1 && zstrncmp( cpcLine, "CASE", nLth ) == 0 )
                  nCnt++;
               else
               if ( nLth == sizeof( "LOOP" ) - 1 && zstrncmp( cpcLine, "LOOP", nLth ) == 0 )
                  nCnt++;
            }
         }

         nPos = -1;
      }

      if ( cpcLine[ k ] == 0 )
         break;

      nLth++;
      nPos++;
   }
#endif

   return( nCnt );
}
#pragma warning (default: 4996)

zSHORT
fnParseLine( zCPCHAR cpcLine, zBOOL *pbMultiLineComment )
{
   zSHORT k = 0;

   while ( cpcLine[ k ] )
   {
      if ( cpcLine[ k ] == ',' || zisspace( cpcLine[ k ] ) )
      {
         k++;
      }
      else
      {
         return( fnTestForKeyWord( cpcLine + k, pbMultiLineComment ) );
      }
   }

   return( 0 );
}

zSHORT
fnLocateOperationInFile( zVIEW vSubtask, zLONG lFile, zPLONG plLineCnt,
                         zPLONG plPrevCommentStart, zCPCHAR cpcOperationName )
{
   zPCHAR pchLine;
   zSHORT nComment;
   zSHORT nRC;

   *plLineCnt = 0;
   *plPrevCommentStart = -1;
   nComment = 0; // ==> last line not a comment
   nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lFile );
   while ( nRC > 0 )
   {
      nComment = fnIsComment( pchLine, nComment );
      if ( nComment )
      {
         if ( *plPrevCommentStart < 0 )
            *plPrevCommentStart = *plLineCnt;
      }
      else
      {
         if ( fnLocateOperInLine( pchLine, cpcOperationName ) )
         {
            (*plLineCnt)++;
            break;
         }

         if ( fnLocateOperationInLine( pchLine ) == FALSE )
            *plPrevCommentStart = -1;
      }

      (*plLineCnt)++;
      nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lFile );
   }

   return( nRC );
}

//./ ADD NAME=CopyOperationCode
// Source Module=tzlooprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: CopyOperationCode
//
//  PURPOSE:    This routine copies an operation from one source file to
//              another source file.
//
//  PARAMETERS: vSubtask - view for the current application
//              pchTgtFileName - target file name
//              pchSrcFileName - source file name
//              pchOperationName - name of the operation to copy
//
//  RETURNS: >= 0  - Operation successfully copied.
//             -1  - Could not open Source or Target file
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
CopyOperationCode( zVIEW   vSubtask,
                   zPCHAR  pchTgtFileName,
                   zPCHAR  pchSrcFileName,
                   zPCHAR  pchOperationName )
{
   LPAPP  lpApp;
   zPCHAR pchLine;
   zCHAR  szTgtFile[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szSrcFile[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTmpFile[ zMAX_FILESPEC_LTH + 1 ];
   zLONG  lTgtFile;
   zLONG  lTgtLineCnt;
   zLONG  lTgtPrevCommentStart;
   zLONG  lSrcFile;
   zLONG  lSrcLineCnt;
   zLONG  lSrcPrevCommentStart;
   zLONG  lTmpFile;
   zLONG  k;
   zBOOL  bMultiLineComment;
   zSHORT nOpenType;
   zSHORT nEND;
   zSHORT nRC;

   SfGetApplicationForSubtask( &lpApp, vSubtask );
   if ( lpApp )
   {
      strcpy_s( szTmpFile, sizeof( szTmpFile ), lpApp->szSourceDir );
      SysConvertEnvironmentString( szTgtFile, sizeof( szTgtFile ), szTmpFile );
      strcpy_s( szSrcFile, sizeof( szSrcFile ), szTgtFile );
   }
   else
   {
      szTgtFile[ 0 ] = 0;
      szSrcFile[ 0 ] = 0;
   }

   strcat_s( szTgtFile, sizeof( szTgtFile ), pchTgtFileName );
   strcat_s( szSrcFile, sizeof( szSrcFile ), pchSrcFileName );

   TraceLine( "CopyOperationCode %s from: %s to: %s", pchOperationName, szSrcFile, szTgtFile );
   if ( (lSrcFile = SysOpenFile( vSubtask, szSrcFile, COREFILE_READ )) == -1 )
   {
      TraceLineS( "CopyOperationCode cannot open Source File: ", szSrcFile );
      return( -1 );
   }

   nRC = fnLocateOperationInFile( vSubtask, lSrcFile, &lSrcLineCnt,
                                  &lSrcPrevCommentStart, pchOperationName );
   if ( nRC > 0 )
   {
      SysCloseFile( vSubtask, lSrcFile, 0 );
      if ( (lTgtFile = SysOpenFile( vSubtask, szTgtFile, COREFILE_EXIST )) == -1 )
      {
         nOpenType = COREFILE_WRITE;
         lTgtFile = SysOpenFile( vSubtask, szTgtFile, COREFILE_WRITE );
      }
      else
      {
         lTgtFile = SysOpenFile( vSubtask, szTgtFile, COREFILE_READ );
         if ( lTgtFile == -1 )
         {
            TraceLineS( "CopyOperationCode cannot open Target File: ", szTgtFile );
            return( -1 );
         }

         nRC = fnLocateOperationInFile( vSubtask, lTgtFile, &lTgtLineCnt,
                                        &lTgtPrevCommentStart, pchOperationName );
         SysCloseFile( vSubtask, lTgtFile, 0 );
         if ( nRC > 0 )
         {
            // Most difficult situation.  Need to create a new file with merge of specified
            // operation from source file into the "middle" of the original target file.
            nOpenType = COREFILE_UPDATE;
            strcpy_s( szTmpFile, sizeof( szTmpFile ), szTgtFile );
            strcat_s( szTmpFile, sizeof( szTmpFile ), ".tmp" );
            lTgtFile = SysOpenFile( vSubtask, szTmpFile, COREFILE_WRITE ); // Tmp and Tgt are reversed on purpose!
            if ( lTgtFile == -1 )
            {
               TraceLineS( "CopyOperationCode cannot open Target File: ", szTmpFile );
               return( -1 );
            }

            // We've opened this file once ... it had better still be OK.
            lTmpFile = SysOpenFile( vSubtask, szTgtFile, COREFILE_READ );  // Tmp and Tgt are reversed on purpose!

            // Read/Write until "top" of original target file is written to temp file.
            k = 1;
            nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lTmpFile );
            while ( nRC > 0 && k < lTgtPrevCommentStart )
            {
               k++;
               SysWriteLine( vSubtask, lTgtFile, pchLine );
               nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lTmpFile );
            }

            // Skip "old" body of operation.
            bMultiLineComment = FALSE;
            nEND = 1;
            while ( nRC > 0 && nEND > 0 )
            {
               nEND += fnParseLine( pchLine, &bMultiLineComment );
               nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lTmpFile );
            }
         }
         else
         {
            nOpenType = COREFILE_APPEND;
            lTgtFile = SysOpenFile( vSubtask, szTgtFile, COREFILE_APPEND );
         }
      }

      // We've done it once ... it had better still be OK.
      lSrcFile = SysOpenFile( vSubtask, szSrcFile, COREFILE_READ );
      k = 1;
      nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lSrcFile );
      while ( nRC > 0 && k < lSrcPrevCommentStart )
      {
         k++;
         nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lSrcFile );
      }

      // Write out leading comments.
      while ( nRC > 0 && k < lSrcLineCnt )
      {
         k++;
         SysWriteLine( vSubtask, lTgtFile, pchLine );
         nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lSrcFile );
      }

      // Write out body of operation.
      bMultiLineComment = FALSE;
      nEND = 1;
      while ( nRC > 0 && nEND > 0 )
      {
         nEND += fnParseLine( pchLine, &bMultiLineComment );
         SysWriteLine( vSubtask, lTgtFile, pchLine );
         nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lSrcFile );
      }

      if ( nOpenType == COREFILE_UPDATE )
      {
         // We need to write out the remaining "old" file.
         nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lTmpFile );
         while ( nRC > 0 )
         {
            SysWriteLine( vSubtask, lTgtFile, pchLine );
            nRC = SysReadLine( vSubtask, (zCOREMEM) &pchLine, lTmpFile );
         }

         SysCloseFile( vSubtask, lTmpFile, 0 );
         SysCloseFile( vSubtask, lTgtFile, 0 );
         lTgtFile = -1;
         SysRenameFile( vSubtask, szTmpFile, szTgtFile, TRUE );
      }

      nRC = 1;
   }

   if ( lTgtFile != -1 )
      SysCloseFile( vSubtask, lTgtFile, 0 );

   SysCloseFile( vSubtask, lSrcFile, 0 );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: InsertSpacesInPrompt
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
InsertSpacesInPrompt( zPCHAR lpReturnString,
                      zVIEW  vDialog,
                      zPCHAR lpOriginalString,
                      zLONG  lMaxLength )
{
   zSHORT j, k;

   lpReturnString[ 0 ] = lpOriginalString[ 0 ];
   j = 1;
   k = 1;
   while ( lpOriginalString[ j ] != 0 && k < lMaxLength )
   {
      if ( lpOriginalString[ j ] >= 'A' && lpOriginalString[ j ] <= 'Z' )
      {
         lpReturnString[ k++ ] = ' ';
         lpReturnString[ k++ ] = lpOriginalString[ j++ ];
      }
      else
         lpReturnString[ k++ ] = lpOriginalString[ j++ ];
   }

   lpReturnString[ k ] = 0;
   strcpy_s( lpOriginalString, lMaxLength, lpReturnString );
   return( 0 );
}