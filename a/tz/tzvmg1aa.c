/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmg1aa.c - VML SubSystem Generator Main Module
// DESCRIPTION:   This is the source file which contains the main functions
//                for the Generator.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/07/20
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2001.10.08  HH
   initialize local variables (sloppy programmer's request).
2001.04.10  HH
   correct generation of prototype for return type void.
   Correct generation of RETURN for return type void.
2001.01.15  HH
   Improve output of header lines.

2001.01.10  HH
   Restructure.
   R53716, fix bug for Operation with no parms.

2001.01.02  HH
   use external function GetVMLConstChar.

2000.11.29  HH
   optionally generate const zCHAR for non return strings.

2000.09.15  HH
   support local C operations to be called from VML.

2000.08.18  HH
   support return values different from short.

2000.06.21  HH
   generate "static" for local functions.

2000.03.13  DKS
   Remove trailing spaces from generated C.

2000.02.18  HH
   get the VML forward comment switch from zeidon.ini,
   entry [Zeidon].ForwardVMLToC = 'n' or 'N' or '0' to switch off.

1999.06.25  DKS
   Always generate VML as comments in the C source (with a reasonable
   margin).

1999.04.13  HH
   Function prototypes are generated with "zOPER_EXPORT".

13.07.1997  DonC
   Modified OutputHeaderFiles so that an include statement would not be
   generated for header KZOEP1AA.H, since it is automatically generated
   from KZOENGAA.H.

1998.03.02  DGC
   Made changes to generate C++-compatable files:
      o  Change generation of zDECIMAL to ZDecimal.
      o  Add '#ifdef __cplusplus' stuff at beginning and end of each file.

12.06.1998  HH
   Corrected GenerateC_Source so that decimal return parameters would
   be handled right.

15.06.1998 - 17.06.1998   HH
   Changed GetLocalC_DeclarationString to handle Entity- and Attribute-
   parameter of attribute functions.
   Corrected handling of unsigned parameters.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmlopr.hg"
#include "ZeidonOp.H"

zLONG  g_lStaticPrevLineNbr;
zBOOL  g_bForwardVML_ToC;

/* local prototypes */
zSHORT LOCALOPER
OutputText( zVIEW  vSubtask,
            zLONG  lRelLine,
            zLONG  lfCFileHandle,
            zPCHAR pchOutputString );

zSHORT LOCALOPER
OutputLastText( zVIEW   vSubtask,
                zLONG   lfTempCFileHandle,
                zPCHAR  pchOutputString );

zSHORT LOCALOPER
GetLocalC_DeclarationString( zPCHAR pchLocalString,
                             zLONG  lMaxLth,
                             zSHORT nVariableID );
zBOOL LOCALOPER
GetLocalJavaDeclarationString( zPCHAR pchLocalString,
                               zLONG  lMaxLth,
                               zLONG  lVariableID,
                               zBOOL  bLocalBuildQual,
                               zBOOL  bLocalVariables );

zSHORT LOCALOPER
OutputLine( zVIEW  vSubtask,
            zLONG  lFileHandle,
            zPCHAR pchOutputString,
            zSHORT nNull );

zSHORT LOCALOPER
OutputBlankLine( zVIEW  vSubtask,
                 zLONG  lFileHandle );


zSHORT LOCALOPER
OutputHeaderFiles( zVIEW  vSubtask,
                   zLONG  lfCFileHandle,
                   zPCHAR pchFileName,
                   zPCHAR pchOutputString,
                   zLONG  lMaxLth );

zSHORT LOCALOPER
BuildC_CaseStatement( zVIEW  vSubtask,
                      zLONG  lfCFileHandle,
                      zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_ElseStatement( zVIEW  vSubtask,
                      zLONG  lfCFileHandle,
                      zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_EndStatement( zVIEW  vSubtask,
                     zLONG  lfCFileHandle,
                     zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_IfStatement( zVIEW  vSubtask,
                    zLONG  lfCFileHandle,
                    zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_LoopStatement( zVIEW  vSubtask,
                      zLONG  lfCFileHandle,
                      zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_OfStatement( zVIEW  vSubtask,
                    zLONG  lfCFileHandle,
                    zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_ReturnStatement( zVIEW  vSubtask,
                        zLONG  lfCFileHandle,
                        zPCHAR pchOutputString,
                        zLONG  lMaxLth );

zSHORT LOCALOPER
BuildC_UntilStatement( zVIEW  vSubtask,
                       zLONG  lfCFileHandle,
                       zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_WhileStatement( zVIEW  vSubtask,
                       zLONG  lfCFileHandle,
                       zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_OtherwiseStatement( zVIEW  vSubtask,
                           zLONG  lfCFileHandle,
                           zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_BreakStatement( zVIEW  vSubtask,
                       zLONG  lfCFileHandle,
                       zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildC_ExpressionStatement( zVIEW  vSubtask,
                            zLONG  lfCFileHandle,
                            zPCHAR pchOutputString,
                            zLONG  lMaxLth );

zSHORT LOCALOPER
BuildJavaCaseStatement( zVIEW  vSubtask,
                        zLONG  lfCFileHandle,
                        zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaElseStatement( zVIEW  vSubtask,
                        zLONG  lfCFileHandle,
                        zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaEndStatement( zVIEW  vSubtask,
                       zLONG  lfCFileHandle,
                       zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaIfStatement( zVIEW  vSubtask,
                      zLONG  lfCFileHandle,
                      zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaLoopStatement( zVIEW  vSubtask,
                        zLONG  lfCFileHandle,
                        zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaOfStatement( zVIEW  vSubtask,
                      zLONG  lfCFileHandle,
                      zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaReturnStatement( zVIEW  vSubtask,
                          zLONG  lfCFileHandle,
                          zPCHAR pchOutputString,
                          zPCHAR pchReturnParameterName,
                          zLONG  lClass,
                          zBOOL  bLocal );

zSHORT LOCALOPER
BuildJavaUntilStatement( zVIEW  vSubtask,
                         zLONG  lfCFileHandle,
                         zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaWhileStatement( zVIEW  vSubtask,
                         zLONG  lfCFileHandle,
                         zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaOtherwiseStatement( zVIEW  vSubtask,
                             zLONG  lfCFileHandle,
                             zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaBreakStatement( zVIEW  vSubtask,
                         zLONG  lfCFileHandle,
                         zPCHAR pchOutputString );

zSHORT LOCALOPER
BuildJavaExpressionStatement( zVIEW  vSubtask,
                              zLONG  lfCFileHandle,
                              zPCHAR pchOutputString );

static void
WritePrototypesForLocalC( zVIEW  vSubtask,
                          zLONG  lfCFileHandle,
                          zPCHAR pchOutputString );

static void
GenerateC_LocalVariableList( zVIEW  vSubtask,
                             zLONG  lfTempCFileHandle,
                             zPCHAR pchOutputString,
                             zPLONG plRelLine,
                             zSHORT nMoreParms,
                             zBOOL  bDecimalArguments );
static void
GenerateJavaLocalVariableList( zVIEW  vSubtask,
                               zLONG  lfTempCFileHandle,
                               zPCHAR pchOutputString,
                               zPLONG plRelLine,
                               zSHORT nMoreParms,
                               zBOOL  bDecimalArguments );
static void
GenerateC_Prototype( zVIEW   vSubtask,
                     zLONG   lfTempCFileHandle,
                     zLONG   lfHeaderHandle,
                     zPCHAR  pchOutputString,
                     zPCHAR  pchXPGName,
                     zPLONG  plRelLine,
                     zPSHORT pnMoreParms,
                     zPBOOL  pbDecimalArguments,
                     zCHAR   cOperationType );
static zBOOL
GenerateJavaPrototype( zVIEW   vSubtask,
                       zLONG   lfTempCFileHandle,
                       zLONG   lfHeaderHandle,
                       zPCHAR  pchOutputString,
                       zPCHAR  pchReturnParameterName,
                       zPCHAR  pchXPGName,
                       zPLONG  plRelLine,
                       zPSHORT pnMoreParms,
                       zPBOOL  pbDecimalArguments,
                       zCHAR   cOperationType );

static void
GenerateC_ControlStatements( zVIEW  vSubtask,
                             zLONG  lfTempCFileHandle,
                             zPCHAR pchOutputString,
                             zPLONG plRelLine );
static void
GenerateJavaControlStatements( zVIEW  vSubtask,
                               zLONG  lfTempCFileHandle,
                               zPCHAR pchOutputString,
                               zPCHAR pchReturnParameterName,
                               zPLONG plRelLine,
                               zBOOL  bLocal );
static void
fnGenerateC_Init( zPCHAR pchLocalString, zLONG lMaxLth, zLONG lDataType );

static void
fnGenerateJavaInit( zPCHAR pchLocalString, zLONG lMaxLth, zLONG lDataType );

//
// OPERATION: GenerateC_Source
//
// DESCRIPTION: GenerateC_Source
//
zOPER_EXPORT zSHORT OPERATION
GenerateC_Source( zVIEW  vSubtask,
                  zPCHAR pchFileName,
                  zPCHAR pchXPGName,
                  zLONG  lfCFileHandle,
                  zLONG  lfTempCFileHandle )
{
   zPCHAR pchOutputString;
   zCHAR  szIni[ 8 ];
   zCHAR  szTypeString[ 2 ];
   zCHAR  szObjectName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  cOperationType;
   zLONG  lParameterFlag;
   zLONG  lRelLine;
   zLONG  lfHeaderHandle = 0;
   zLONG  hOutputString;
   zSHORT nMoreParms;
   zSHORT nRC;

   /////////////////////////////////////////////////////////////////
   // Allocate memory for pchOutputString
   /////////////////////////////////////////////////////////////////
   hOutputString = SysAllocMemory( (zCOREMEM) &pchOutputString,  VM_MAX_STRING_SIZE, 0, zCOREMEM_ALLOC, 0 );
   if ( pchOutputString == 0 )
   {
      // DM - put error message here.
      SysMessageBox( vSubtask, "DMC1", "Memory allocation failed", 0 );
      return( 0 );
   }

   // Allocate memory for pchOutputExprStr.
   g_hOutputExprStr = SysAllocMemory( (zCOREMEM) &g_pchOutputExprStr, VM_MAX_STRING_SIZE, 0, zCOREMEM_ALLOC, 0 );
   if ( g_hOutputExprStr == 0 )
   {
      // DM - put error message here!
      SysMessageBox( vSubtask, "DMC2", "Memory allocation failed", 0 );
      return( 0 );
   }

   g_nLeftMargin = 0;
   pchOutputString[ 0 ] = 0;

   OutputLine( vSubtask, lfCFileHandle, "#define KZSYSSVC_INCL", 1 );

   /////////////////////////////////////////////////////////////////
   // Include all header files associated with current source file.
   // Put them into the C file, above the function protos.
   /////////////////////////////////////////////////////////////////
   nRC = OutputHeaderFiles( vSubtask, lfCFileHandle, pchFileName, pchOutputString, VM_MAX_STRING_SIZE );

   /////////////////////////////////////////////////////////////////
   // Set switch that determines if VML source is forwarded to the
   // generated C as comments.
   //
   // To throw the switch, the user should use the Object Browser to set
   // TZ_PRFO(EditorProfile).VML.GenVMLasComment to be Y or N
   /////////////////////////////////////////////////////////////////
   /*
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "VML", vSubtask, zCURRENT_OI );

   if ( vProfileXFER == 0 )
      nForwardVML = 0;
   else
   if ( CompareAttributeToString( vProfileXFER, "VML", "GenVMLasComments", "Y" ) == 0 )
   */

   // Get the VML forward switch (output VML as part of the generated source)
   // from zeidon.ini (default is on).
   szIni[ 0 ] = 0;
   SysReadZeidonIni( -1, "[Zeidon]", "ForwardVMLToC", szIni, sizeof( szIni ) );

   if ( szIni[ 0 ] == 'n' || szIni[ 0 ] == 'N' || szIni[ 0 ] == '0' )
      g_bForwardVML_ToC = FALSE;
   else
      g_bForwardVML_ToC = TRUE;

   OutputBlankLine( vSubtask, lfCFileHandle );

   // Write the prototypes of all local C operations, not for "GLOBAL Operations"
   MiGetObjectNameForView( szObjectName, g_vSourceMeta );
   if ( zstrcmp( szObjectName, "TZOGSRCO") != 0 )
      WritePrototypesForLocalC( vSubtask, lfCFileHandle, pchOutputString );

   ///////////////////////////////////////////////////////////////////////////
   // Generate the source, provided there is any in the Parsed Instance (XPG).
   ///////////////////////////////////////////////////////////////////////////
   if ( SetCursorFirstEntityByString( g_lpPIView, "SourceFile", "Name", pchFileName, 0 ) == zCURSOR_SET )
   {
      zBOOL bDecimalArguments = FALSE;

      ///////////////////////////////////////////////////////////////
      // Loop thru each operation in the XPG and generate.
      ///////////////////////////////////////////////////////////////
      SetCursorFirstEntity( g_lpPIView, "OperationSource", 0 );
      do
      {
         nRC = SetCursorFirstEntityByAttr( g_lpPIView, "Operation", "ZKey",  g_lpPIView, "OperationSource", "ZKey", "" );

         /////////////////////////////////////////////////////////////////
         // If the current operation has no source, skip it.
         /////////////////////////////////////////////////////////////////
         if ( nRC == zCURSOR_SET )
         {
            ///////////////////////////////////////////////////////////////
            // Write out prototype to C file or global header file.
            ///////////////////////////////////////////////////////////////
            // find the operation type and store as a character
            GetStringFromAttribute( szTypeString, sizeof( szTypeString ), g_lpPIView, "Operation", "Type" );
            cOperationType = szTypeString[ 0 ];

            // determine if header stuff goes to c file or to external lplr header.
            lfHeaderHandle = 0;
            if ( cOperationType == 'D' ||                  // Dialog
                 cOperationType == 'A' ||
                 cOperationType == 'L' )
            {
               lfHeaderHandle = lfCFileHandle;
            }

            // otherwise if ( cOperationType == 'G' ||
            //                cOperationType == 'O' ||
            //                cOperationType == 'T' ||
            //                cOperationType == 'C' ||     // Object
            //                cOperationType == 'E' )      // Entity
            // Header info goes into ZEIDONOP.H, so we don't have to
            // generate a prototype here (lfHeaderHandle=0).

            SetCursorFirstEntity( g_lpPIView, "OperationText", "" );

            /////////////////////////////////////////////////////////////////////////////
            // If we are to forward the VML as source, write out the VML declaration.
            // We have to scan forward in the XPG to find the end of of the declaration
            // before we start pumping out the VML.
            /////////////////////////////////////////////////////////////////////////////
            if ( g_bForwardVML_ToC )
            {
               CreateViewFromViewForTask( &g_vStatementView, g_lpPIView, 0 );
               SetNameForView( g_vStatementView, "_GenVML_Source", vSubtask, zLEVEL_SUBTASK );

               // If the operation has variables ...
               if ( SetCursorFirstEntity( g_vStatementView, "Variable", "" ) == zCURSOR_SET )
               {
                  // Look for parameters ...
                  GetIntegerFromAttribute( &lParameterFlag, g_vStatementView, "Variable", "ParameterFlag" );

                  // If we have parameters ...
                  if ( lParameterFlag > 0 )
                  {
                     while ( lParameterFlag > 0 )
                     {
                        nMoreParms = SetCursorNextEntity( g_vStatementView, "Variable", "" );
                        if ( nMoreParms == zCURSOR_SET )
                        {
                           GetIntegerFromAttribute( &lParameterFlag, g_vStatementView, "Variable", "ParameterFlag" );
                           GetIntegerFromAttribute( &lRelLine, g_vStatementView, "Variable", "LineNumber" );
                        }
                        else
                        {
                           lParameterFlag = 0;
                        }
                     }
                  }
                  else  // otherwise the operation has no parms
                  {
                     //GetIntegerFromAttribute( &lRelLine, g_vStatementView, "Variable", "LineNumber" );
                     // a fudge factor
                     //lRelLine--;
                     lRelLine = 2;
                  }
               }

               DropView( g_vStatementView );

               // Write out vml declaration as comments.
               OutputText( vSubtask, lRelLine, lfTempCFileHandle, pchOutputString );
            }

            ///////////////////////////////////////////////////////////////
            // Generate the prototype & declaration of the operation.
            ///////////////////////////////////////////////////////////////
            GenerateC_Prototype( vSubtask, lfTempCFileHandle, lfHeaderHandle, pchOutputString,
                                 pchXPGName, &lRelLine, &nMoreParms, &bDecimalArguments, cOperationType );

            // Set indent and margin for rest of processing.
            g_nLeftMargin = 3;

            // Open main body of code.
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{" );
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );

            ////////////////////////////////////////////////////////////
            // Generate the local variable list.
            ////////////////////////////////////////////////////////////
            GenerateC_LocalVariableList( vSubtask, lfTempCFileHandle, pchOutputString, &lRelLine, nMoreParms, bDecimalArguments );

            OutputBlankLine( vSubtask, lfTempCFileHandle );
            OutputBlankLine( vSubtask, lfHeaderHandle );

            ///////////////////////////////////////////////////////////////
            // Generate the statements next, provided there are any.
            ///////////////////////////////////////////////////////////////
            GenerateC_ControlStatements( vSubtask, lfTempCFileHandle, pchOutputString, &lRelLine );
            OutputBlankLine( vSubtask, lfTempCFileHandle );
         }

      } while ( SetCursorNextEntity( g_lpPIView, "OperationSource", "" ) > zCURSOR_UNCHANGED );

   } // if ( SetCursorFirstEntityByString( g_lpPIView, "SourceFile",

   ///////////////////////////////////////////////////////////////
   // Clean up and close up shop.
   ///////////////////////////////////////////////////////////////

   // drop views
   // DropView( vProfileXFER );

   // free memory for pchOutputString
   SysFreeMemory( g_hOutputExprStr );
   SysFreeMemory( hOutputString );

   return( 0 );
}

//
// OPERATION: GenerateJavaSource
//
// DESCRIPTION: GenerateJavaSource
//
zOPER_EXPORT zSHORT OPERATION
GenerateJavaSource( zVIEW  vSubtask,
                    zPCHAR pchFileName,
                    zPCHAR pchXPGName,
                    zLONG  lfCFileHandle,
                    zLONG  lfTempCFileHandle,
                    zLONG  lType )
{
   zVIEW  vViewObject;
   zPCHAR pchOutputString;
   zPCHAR pchSlash;
   zCHAR  szIni[ 8 ];
   zCHAR  szTypeString[ 2 ];
// zCHAR  szObjectName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szParameterName[ 33 ];
   zCHAR  szPackageName[ 256 ];
   zCHAR  szImportName[ 256 ];
   zCHAR  szFullImportName[ 512 ];
   zCHAR  szSourceType[ 10 ];
   zCHAR  cOperationType;
   zLONG  lParameterFlag;
   zLONG  lRelLine;
   zLONG  lfHeaderHandle = 0;
   zLONG  hOutputString;
   zBOOL  bLocal;
   zSHORT nMoreParms;
   zSHORT nRC;

   /////////////////////////////////////////////////////////////////
   // Allocate memory for pchOutputString
   /////////////////////////////////////////////////////////////////
// DrAllocTaskMemory( (zCOREMEM) &pchOutputString, VM_MAX_STRING_SIZE, 0xc8c8c8c8 );
// hOutputString = (zLONG) pchOutputString;
   hOutputString = SysAllocMemory( (zCOREMEM) &pchOutputString, VM_MAX_STRING_SIZE, 0, zCOREMEM_ALLOC, 0 );
   if ( pchOutputString == 0 )
   {
      // DM - put error message here.
      SysMessageBox( vSubtask, "DMJ1", "Memory allocation failed", 0 );
      return( 0 );
   }

   // Allocate memory for pchOutputExprStr.
// DrAllocTaskMemory( (zCOREMEM) &g_pchOutputExprStr, VM_MAX_STRING_SIZE, 0xc8c8c8c8 );
// g_hOutputExprStr = (zLONG) g_pchOutputExprStr;
   g_hOutputExprStr = SysAllocMemory( (zCOREMEM) &g_pchOutputExprStr, VM_MAX_STRING_SIZE, 0, zCOREMEM_ALLOC, 0 );
   if ( g_hOutputExprStr == 0 )
   {
      // DM - put error message here!
      SysMessageBox( vSubtask, "DMJ2", "Memory allocation failed", 0 );
      return( 0 );
   }

   GetViewByName( &vViewObject, "TaskLPLR", vSubtask, zLEVEL_TASK );
   SetNameForView( g_lpPIView, "g_lpPIView", vSubtask, zLEVEL_TASK );

   g_nLeftMargin = 0;

   OutputLine( vSubtask, lfCFileHandle, "/**", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    This file is part of the Zeidon Java Object Engine (Zeidon JOE).", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   OutputLine( vSubtask, lfCFileHandle, "    Zeidon JOE is free software: you can redistribute it and/or modify", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    it under the terms of the GNU Lesser General Public License as published by", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    the Free Software Foundation, either version 3 of the License, or", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    (at your option) any later version.", 2 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   OutputLine( vSubtask, lfCFileHandle, "    Zeidon JOE is distributed in the hope that it will be useful,", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    but WITHOUT ANY WARRANTY; without even the implied warranty of", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    GNU Lesser General Public License for more details.", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   OutputLine( vSubtask, lfCFileHandle, "    You should have received a copy of the GNU Lesser General Public License", 1 );
   OutputLine( vSubtask, lfCFileHandle, "    along with Zeidon JOE.  If not, see <http://www.gnu.org/licenses/>.", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   OutputLine( vSubtask, lfCFileHandle, "    Copyright 2009-2010 QuinSoft", 1 );
   OutputLine( vSubtask, lfCFileHandle, "**/", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "package " );
   GetStringFromAttribute( szPackageName, sizeof( szPackageName ), vViewObject, "LPLR", "JavaPackageName" );
   while ( (pchSlash = zstrchr( szPackageName, '\\' )) != 0 )
      *pchSlash = '.';

   strcpy_s( pchOutputString + 8, VM_MAX_STRING_SIZE - 8, szPackageName );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   OutputLine( vSubtask, lfCFileHandle, pchOutputString, 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.ActivateFlags;", 1 );
   OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.CursorPosition;", 1 );
   if ( lType == zREFER_LOD_META ) // 2007
   {
      OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.TaskQualification;", 1 );
      OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.vml.VmlObjectOperations;", 1 );
   }
   else
   {
      OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.Task;", 1 );
      OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.vml.VmlDialog;", 1 );
   }

   OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.View;", 1 );
   OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.ZeidonException;", 1 );
   OutputLine( vSubtask, lfCFileHandle, "import com.quinsoft.zeidon.vml.zVIEW;", 1 );
   OutputLine( vSubtask, lfCFileHandle, "import org.apache.commons.lang3.mutable.MutableDouble;", 1 );
   OutputLine( vSubtask, lfCFileHandle, "import org.apache.commons.lang3.mutable.MutableInt;", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );

// pchOutputString[ 0 ] = 0;

   /////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////
   // Set switch that determines if VML source is forwarded to the
   // generated C as comments.
   //
   // To throw the switch, the user should use the Object Browser to set
   // TZ_PRFO(EditorProfile).VML.GenVMLasComment to be Y or N
   /////////////////////////////////////////////////////////////////
   /*
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "VML", vSubtask, zCURRENT_OI );

   if ( vProfileXFER == 0 )
      nForwardVML = 0;
   else
   if ( CompareAttributeToString( vProfileXFER, "VML", "GenVMLasComments", "Y" ) == 0 )
   */

   // Get the VML forward switch (output VML as part of the generated source)
   // from zeidon.ini (default is on).
   szIni[ 0 ] = 0;
   SysReadZeidonIni( -1, "[Zeidon]", "ForwardVMLToC", szIni, sizeof( szIni ) );

   if ( szIni[ 0 ] == 'n' || szIni[ 0 ] == 'N' || szIni[ 0 ] == '0' )
      g_bForwardVML_ToC = FALSE;
   else
      g_bForwardVML_ToC = TRUE;

   ///////////////////////////////////////////////////////////////////////////
   // Generate the source, provided there is any in the Parsed Instance (XPG).
   ///////////////////////////////////////////////////////////////////////////
   if ( SetCursorFirstEntityByString( g_lpPIView, "SourceFile", "Name", pchFileName, 0 ) == zCURSOR_SET )
   {
      zBOOL bDecimalArguments = FALSE;

      ///////////////////////////////////////////////////////////////
      // Loop thru each operation in the XPG and generate.
      ///////////////////////////////////////////////////////////////
      SetCursorFirstEntity( g_lpPIView, "OperationSource", 0 );
      do
      {
         nRC = SetCursorFirstEntityByAttr( g_lpPIView, "Operation", "ZKey", g_lpPIView, "OperationSource", "ZKey", "" );

         /////////////////////////////////////////////////////////////////
         // If the current operation has no source, skip it.
         /////////////////////////////////////////////////////////////////
         if ( nRC == zCURSOR_SET )
         {
            ///////////////////////////////////////////////////////////////
            // Write out prototype to java file or global header file.
            ///////////////////////////////////////////////////////////////
            // find the operation type and store as a character
            GetStringFromAttribute( szTypeString, sizeof( szTypeString ), g_lpPIView, "Operation", "Type" );
            cOperationType = szTypeString[ 0 ];

            // Determine if header stuff goes to c file or to external lplr header.
            lfHeaderHandle = 0;
            if ( cOperationType == 'D' ||                  // Dialog
                 cOperationType == 'A' ||
                 cOperationType == 'L' )
            {
               lfHeaderHandle = lfCFileHandle;
            }

            // otherwise if ( cOperationType == 'G' ||
            //                cOperationType == 'O' ||
            //                cOperationType == 'T' ||
            //                cOperationType == 'C' ||     // Object
            //                cOperationType == 'E' )      // Entity
            // Header info goes into ZEIDONOP.H, so we don't have to generate a prototype here (lfHeaderHandle == 0).

            SetCursorFirstEntity( g_lpPIView, "OperationText", "" );

            /////////////////////////////////////////////////////////////////////////////
            // If we are to forward the VML as source, write out the VML declaration.
            // We have to scan forward in the XPG to find the end of of the declaration
            // before we start pumping out the VML.
            /////////////////////////////////////////////////////////////////////////////
            if ( g_bForwardVML_ToC )
            {
               CreateViewFromViewForTask( &g_vStatementView, g_lpPIView, 0 );
               SetNameForView( g_vStatementView, "_GenVML_Source", vSubtask, zLEVEL_SUBTASK );
               SetNameForView( g_lpPIView, "g_lpPIView", vSubtask, zLEVEL_TASK );
            // MessageSend( vSubtask, "VM00402", "VML Parser",
            //              "Please check View: g_lpPIView and _GenVML_Source for Operation Data",
            //              zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );

               // Assume the operation has no parms (see the C generation code above to see the difference in logic).
               lRelLine = 2;  // initialize lRelLine ... not done in previous code???  dks 2009.09.03

               // If the operation has variables ...
               if ( SetCursorFirstEntity( g_vStatementView, "Variable", "" ) == zCURSOR_SET )
               {
                  // Look for parameters ...
                  GetIntegerFromAttribute( &lParameterFlag, g_vStatementView, "Variable", "ParameterFlag" );

                  // If we have parameters ...
                  if ( lParameterFlag > 0 )
                  {
                     while ( lParameterFlag > 0 )
                     {
                        nMoreParms = SetCursorNextEntity( g_vStatementView, "Variable", "" );
                        if ( nMoreParms == zCURSOR_SET )
                        {
                           GetIntegerFromAttribute( &lParameterFlag, g_vStatementView, "Variable", "ParameterFlag" );
                           GetIntegerFromAttribute( &lRelLine, g_vStatementView, "Variable", "LineNumber" );
                        }
                        else
                        {
                           lParameterFlag = 0;
                        }
                     }
                  }
               }

               DropView( g_vStatementView );

               // Write out vml declaration as comments.
               OutputText( vSubtask, lRelLine, lfTempCFileHandle, pchOutputString );
            }

            ///////////////////////////////////////////////////////////////
            // Generate the prototype & declaration of the operation.
            ///////////////////////////////////////////////////////////////
            bLocal = GenerateJavaPrototype( vSubtask, lfTempCFileHandle, lfHeaderHandle, pchOutputString, szParameterName,
                                            pchXPGName, &lRelLine, &nMoreParms, &bDecimalArguments, cOperationType );

            // Set indent and margin for rest of processing.
            g_nLeftMargin = 3;

            // Open main body of code.
            pchOutputString[ 0 ] = '{';   // strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "{" );
            pchOutputString[ 1 ] = 0;
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );

            ///////////////////////////////////////////////////////////////
            // Generate the local variable list.
            ///////////////////////////////////////////////////////////////
            GenerateJavaLocalVariableList( vSubtask, lfTempCFileHandle, pchOutputString, &lRelLine, nMoreParms, bDecimalArguments );

            OutputBlankLine( vSubtask, lfTempCFileHandle );
         // OutputBlankLine( vSubtask, lfHeaderHandle );

            ///////////////////////////////////////////////////////////////
            // Generate the statements next, provided there are any.
            ///////////////////////////////////////////////////////////////
            GenerateJavaControlStatements( vSubtask, lfTempCFileHandle, pchOutputString, szParameterName, &lRelLine, bLocal );
            OutputBlankLine( vSubtask, lfTempCFileHandle );
         }

      } while ( SetCursorNextEntity( g_lpPIView, "OperationSource", "" ) > zCURSOR_UNCHANGED );

   } // if ( SetCursorFirstEntityByString( g_lpPIView, "SourceFile",

   OutputBlankLine( vSubtask, lfTempCFileHandle );
   OutputLine( vSubtask, lfTempCFileHandle, "}", 1 );


   // Loop thru each operation in the XPG and generate an import for each source file referenced.
// MessageSend( vSubtask, "VM00402", "VML Parser",
//              "Please check View: g_lpPIView for Operation Data",
//              zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
   pchOutputString[ 0 ] = 0;
   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalOperation", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Write out import for the source file.
      GetStringFromAttribute( szImportName, sizeof( szImportName ), g_lpPIView, "ExternalOperation", "SourceName" );
      GetStringFromAttribute( szSourceType, sizeof( szSourceType ), g_lpPIView, "ExternalOperation", "SourceType" );
      sprintf_s( szFullImportName, sizeof( szFullImportName ), "import %s.%s_%s;", szPackageName, szImportName, szSourceType );
      if ( zstrstr( pchOutputString, szFullImportName ) == 0 )
      {
         if ( pchOutputString[ 0 ] != 0 )
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "\n" );

         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, szFullImportName );
      }

      nRC = SetCursorNextEntity( g_lpPIView, "ExternalOperation", "" );
   }

   if ( pchOutputString[ 0 ] != 0 )
   {
      OutputLine( vSubtask, lfCFileHandle, pchOutputString, 1 );
      OutputBlankLine( vSubtask, lfCFileHandle );
   }

   pchOutputString[ 0 ] = 0;
   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalOperationJava", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Write out import for the source file.
      GetStringFromAttribute( szImportName, sizeof( szImportName ), g_lpPIView, "ExternalOperationJava", "SourceName" );
      GetStringFromAttribute( szSourceType, sizeof( szSourceType ), g_lpPIView, "ExternalOperationJava", "SourceType" );
      sprintf_s( szFullImportName, sizeof( szFullImportName ), "import com.quinsoft.zeidon.zeidonoperations.%s;", szImportName );

      if ( zstrstr( pchOutputString, szFullImportName ) == 0 )
      {
         if ( pchOutputString[ 0 ] != 0 )
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "\n" );

         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, szFullImportName );
      }

      nRC = SetCursorNextEntity( g_lpPIView, "ExternalOperationJava", "" );
   }

   if ( pchOutputString[ 0 ] != 0 )
   {
      OutputLine( vSubtask, lfCFileHandle, pchOutputString, 1 );
      OutputBlankLine( vSubtask, lfCFileHandle );
   }

   OutputLine( vSubtask, lfCFileHandle, "/**", 1 );
   OutputLine( vSubtask, lfCFileHandle, "   @author QuinSoft", 1 );
   OutputLine( vSubtask, lfCFileHandle, "**/", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );
   strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "public class " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, pchFileName );

   if ( lType == zREFER_LOD_META ) // 2007
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "_Object extends VmlObjectOperations" );
   else
   if ( lType == zREFER_DIALOG_META ) // 2011
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "_Dialog extends VmlDialog" );
   else
   if ( lType == zREFER_GOPGRP_META ) // 2014
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "_Operation extends VmlDialog" );
   else
   {
      TraceLineI( "Make Target Unexpected Meta Type: ", lType );
      MessageSend( vSubtask, "ED00103", "Zeidon Compiler", "Unexpected Meta type",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   OutputLine( vSubtask, lfCFileHandle, pchOutputString, 0 );
   OutputLine( vSubtask, lfCFileHandle, "{", 1 );

#if 0  // going to local declaration/instantiation to prevent java.lang.StackOverflowError
   // For each imported source file, generate private reference variable.
   pchOutputString[ 0 ] = 0;
   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalOperation", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Write out import for the source file.
      GetStringFromAttribute( szImportName, sizeof( szImportName ), g_lpPIView, "ExternalOperation", "SourceName" );
      GetStringFromAttribute( szSourceType, sizeof( szSourceType ), g_lpPIView, "ExternalOperation", "SourceType" );
      sprintf_s( szFullImportName, sizeof( szFullImportName ), "%s_%s", szImportName, szSourceType );
      if ( zstrstr( pchOutputString, szFullImportName ) == 0 )
      {
         if ( pchOutputString[ 0 ] != 0 )
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "\n   " );
         else
            strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "   " );

         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, szFullImportName );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " m_" );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, szFullImportName );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " = null;" );
      }

      nRC = SetCursorNextEntity( g_lpPIView, "ExternalOperation", "" );
   }
#endif

   if ( pchOutputString[ 0 ] != 0 )
   {
      OutputLine( vSubtask, lfCFileHandle, pchOutputString, 1 );
      OutputBlankLine( vSubtask, lfCFileHandle );
   }

   // Generate local declaration/instantiation for each operation header used from TZOPSIGK.
   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalOperationJava", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Write out declaration for the class.
      GetStringFromAttribute( szImportName, sizeof( szImportName ), g_lpPIView, "ExternalOperationJava", "SourceName" );
      sprintf_s( szFullImportName, sizeof( szFullImportName ), "   private final %s m_%s;", szImportName, szImportName );
      OutputLine( vSubtask, lfCFileHandle, szFullImportName, 1 );

      nRC = SetCursorNextEntity( g_lpPIView, "ExternalOperationJava", "" );
   }

   strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "   public " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, pchFileName );
   if ( lType == zREFER_LOD_META ) // 2007
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "_Object( View view )" );
   else
   if ( lType == zREFER_DIALOG_META ) // 2011
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "_Dialog( View view )" ); // ( TaskQualification taskQual )" );
   else
   if ( lType == zREFER_GOPGRP_META ) // 2014
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "_Operation( View view )" ); // ( TaskQualification taskQual )" );

   OutputLine( vSubtask, lfCFileHandle, pchOutputString, 0 );
   OutputLine( vSubtask, lfCFileHandle, "   {", 1 );
// if ( lType == zREFER_LOD_META ) // 2007
      OutputLine( vSubtask, lfCFileHandle, "      super( view );", 1 );
// else
//    OutputLine( vSubtask, lfCFileHandle, "      super( taskQual );", 1 );

   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalOperationJava", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Write out instantiation for the class.
      GetStringFromAttribute( szImportName, sizeof( szImportName ), g_lpPIView, "ExternalOperationJava", "SourceName" );
      sprintf_s( szFullImportName, sizeof( szFullImportName ), "      m_%s = new %s( view );", szImportName, szImportName );
      OutputLine( vSubtask, lfCFileHandle, szFullImportName, 1 );

      nRC = SetCursorNextEntity( g_lpPIView, "ExternalOperationJava", "" );
   }

#if 0  // going to local declaration/instantiation to prevent java.lang.StackOverflowError
   // For each imported source file, instantiate private reference variable.
   pchOutputString[ 0 ] = 0;
   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalOperation", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Write out import for the source file.
      GetStringFromAttribute( szImportName, sizeof( szImportName ), g_lpPIView, "ExternalOperation", "SourceName" );
      GetStringFromAttribute( szSourceType, sizeof( szSourceType ), g_lpPIView, "ExternalOperation", "SourceType" );
      sprintf_s( szFullImportName, sizeof( szFullImportName ), "%s_%s", szImportName, szSourceType );
      if ( zstrstr( pchOutputString, szFullImportName ) == 0 )
      {
         if ( pchOutputString[ 0 ] != 0 )
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "\n      m_" );
         else
            strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "      m_" );

         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, szFullImportName );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " = new " );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, szFullImportName );
      // if ( lType == zREFER_LOD_META ) // 2007
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( view );" );
      // else
      //    strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( task );" );
      }

      nRC = SetCursorNextEntity( g_lpPIView, "ExternalOperation", "" );
   }
#endif

   if ( pchOutputString[ 0 ] != 0 )
   {
      OutputBlankLine( vSubtask, lfCFileHandle );
      OutputLine( vSubtask, lfCFileHandle, pchOutputString, 1 );
   }

   OutputLine( vSubtask, lfCFileHandle, "   }", 1 );
   OutputBlankLine( vSubtask, lfCFileHandle );

   // Write the prototypes of all local C operations, not for "GLOBAL Operations"
// MiGetObjectNameForView( szObjectName, g_vSourceMeta );
// if ( zstrcmp( szObjectName, "TZOGSRCO") != 0 )
//    WritePrototypesForLocalJava( vSubtask, lfCFileHandle, pchOutputString );

   OutputBlankLine( vSubtask, lfCFileHandle );

   ///////////////////////////////////////////////////////////////
   // Clean up and close up shop.
   ///////////////////////////////////////////////////////////////

   // drop views
   // DropView( vProfileXFER );

   // free memory for pchOutputString
// DrFreeTaskMemory( (zPVOID) hOutputExprStr );
// DrFreeTaskMemory( (zPVOID) hOutputString );
   SysFreeMemory( g_hOutputExprStr );
   SysFreeMemory( hOutputString );

   return( 0 );
}

///////////////////////////////////////////////////////////////
// Generate the prototype & declaration of the operation.
///////////////////////////////////////////////////////////////
static void
GenerateC_Prototype( zVIEW   vSubtask,
                     zLONG   lfTempCFileHandle,
                     zLONG   lfHeaderHandle,
                     zPCHAR  pchOutputString,
                     zPCHAR  pchXPGName,
                     zPLONG  plRelLine,
                     zPSHORT pnMoreParms,
                     zPBOOL  pbDecimalArguments,
                     zCHAR   cOperationType )
{
   zCHAR  szTypeString[ 2 ];
   zCHAR  cReturnType;
   zLONG  lVariableID;
   zLONG  lParameterFlag;
   zSHORT nRC;

   // Put out first line of prototype.
   if ( cOperationType == 'L' )
      strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "static " );         // local operation
   else
      strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "zOPER_EXPORT " );   // global operation

   GetStringFromAttribute( szTypeString, sizeof( szTypeString ), g_lpPIView, "Operation", "ReturnDataType" );
   cReturnType = szTypeString[ 0 ];

   if ( cReturnType == 'L' )
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zLONG" );    // return type long
   else
   if ( cReturnType == 'M' )
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zDECIMAL" ); // return type decimal
   else
   if ( cReturnType == 'O' || cReturnType == 0 )
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zVOID" );    // return type void
   else
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zSHORT" );   // return type short

   if ( cOperationType != 'L' )
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " OPERATION" );  // global operation

   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 1 );
   OutputLine( vSubtask, lfHeaderHandle, pchOutputString, 0 );

   // Get the operation name.
   GetStringFromAttribute( pchOutputString, VM_MAX_STRING_SIZE, g_lpPIView, "Operation", "C_GeneratedOperationName" );

   // If we have any variables of any sort ...
   nRC = SetCursorFirstEntity( g_lpPIView, "Variable", "" );

   if ( nRC >= zCURSOR_SET )
      GetIntegerFromAttribute( &lParameterFlag, g_lpPIView, "Variable", "ParameterFlag" );
   else
      lParameterFlag = 0;

   // Do parameters.  If no parameters make it void.
   if ( lParameterFlag < 1 ) // no parms
   {
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( zVOID ) " );
      OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 1 );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
      OutputLine( vSubtask, lfHeaderHandle,  pchOutputString, 0 );

      if ( nRC >= zCURSOR_SET )
         *pnMoreParms = zCURSOR_SET;
   }
   else  // otherwise, do parm list
   {
      // open parm list
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( " );

      // Play with nLeftMargin only for parm list processing.
      g_nLeftMargin = 0;

      // DM - as best I can tell, lParameterFlag is the position in the parm list for a variable.  That is, 1 for the 1st, 2 for the 2nd, and so on.
      // So, lParameterFlag will increase from 1 as the loop progresses. A call to SetCursorNextEntity will cause lParameterFlag to be set to 0,
      // ending the loop. This logic should be re-examined, time permitting.
      while ( lParameterFlag > 0 ) // while we have parms to put out
      {
         zSHORT nLth = (zSHORT) zstrlen( pchOutputString );

         if ( nLth > 1 && pchOutputString[ nLth - 2 ] != '(' )
            GetIntegerFromAttribute( plRelLine,  g_lpPIView, "Variable", "LineNumber" );

         GetIntegerFromAttribute( &lVariableID, g_lpPIView, "Variable", "ID" );

         // Get the variable name/declaration.  If function returns TRUE then the argument is a decimal and special processing will be needed later.
         if ( GetLocalC_DeclarationString( g_szLocalString, sizeof( g_szLocalString ), (zSHORT) lVariableID ) )
         {
            *pbDecimalArguments = TRUE;
         }

         *pnMoreParms = SetCursorNextEntity( g_lpPIView, "Variable", "" );
         if ( *pnMoreParms == zCURSOR_SET )
            GetIntegerFromAttribute( &lParameterFlag, g_lpPIView, "Variable", "ParameterFlag" );
         else
            lParameterFlag = 0;

         if ( lParameterFlag > 0 )
         {
            // After 1st time thru loop set g_nLeftMargin to beginning of parm list.
            if ( g_nLeftMargin == 0 )
               g_nLeftMargin = (zSHORT) zstrlen( pchOutputString );

            // Output parm line.
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, g_szLocalString );
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "," );
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 1 );
            OutputLine( vSubtask, lfHeaderHandle, pchOutputString, 0 );
         }
      }

      // Close off parm list.
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, g_szLocalString );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " )" );
      OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 1 );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
      OutputLine( vSubtask, lfHeaderHandle, pchOutputString, 0 );
      OutputBlankLine( vSubtask, lfHeaderHandle );
   }
}

// Generate the prototype & declaration of the operation.
static zBOOL
GenerateJavaPrototype( zVIEW   vSubtask,
                       zLONG   lfTempCFileHandle,
                       zLONG   lfHeaderHandle,
                       zPCHAR  pchOutputString,
                       zPCHAR  pchReturnParameterName,
                       zPCHAR  pchXPGName,
                       zPLONG  plRelLine,
                       zPSHORT pnMoreParms,
                       zPBOOL  pbDecimalArguments,
                       zCHAR   cOperationType )
{
   zCHAR  cReturnType;
   zCHAR  szTypeString[ 2 ];
// zCHAR  szPFlag[ 2 ];
   zLONG  lVariableType;
   zLONG  lVariableID;
   zLONG  lParameterFlag;
   zBOOL  bLocalBuildQual = FALSE;
   zSHORT nReturnParmCnt = 0;
   zSHORT nRC;

   // For public operations, we determine if there is exactly one return parameter (excluding views).
   // If so, we want to make that parameter the return code.  Otherwise, we have either zero or multiple
   // return parameters.  In either case, we will return the normal return code.  For local operations,
   // we will skip all of this since it is not feasible to generate the "return" properly.
   pchReturnParameterName[ 0 ] = 0;
   lVariableType = 0;
// DisplayObjectInstance( g_lpPIView, "Operation", "" );

   // Determine first line of prototype.
   if ( cOperationType == 'L' )
   {
      strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "private " );  // local operation
      nRC = SetCursorLastEntity( g_lpPIView, "Variable", "" );
   }
   else
   {
      strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "public " );   // global operation

#if 0  // not going to do this ... implementing multiple return parameters (mutable).
      nRC = SetCursorFirstEntity( g_lpPIView, "Variable", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lParameterFlag, g_lpPIView, "Variable", "ParameterFlag" );
         if ( lParameterFlag > 0 ) // this is a parm
         {
            GetStringFromAttribute( szPFlag, sizeof( szPFlag ), g_lpPIView, "Variable", "PFlag" );
            if ( szPFlag[ 0 ] == 'Y' )
            {
               GetIntegerFromAttribute( &lVariableType, g_lpPIView, "Variable", "VMLType" );
               if ( lVariableType != qTVIEW )
               {
                  nReturnParmCnt++;
                  GetStringFromAttribute( pchReturnParameterName, g_lpPIView, "Variable", "Name" );  // don't need this here, but will ...
                  break;
               }
            }
         }

         nRC = SetCursorNextEntity( g_lpPIView, "Variable", "" );
      }

      if ( nReturnParmCnt > 1 )
      {
         lVariableType = 0;
         pchReturnParameterName[ 0 ] = 0;
      }
#else
      nRC = SetCursorLastEntity( g_lpPIView, "Variable", "" );
#endif
   }

   GetStringFromAttribute( szTypeString, sizeof( szTypeString ), g_lpPIView, "Operation", "ReturnDataType" );
   cReturnType = szTypeString[ 0 ];

   if ( lVariableType != 0 )
   {
      if ( lVariableType == qTSTRING )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "String " );
      else
      if ( lVariableType == qTVOID )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "void " );
      else
      if ( lVariableType == qTDECIMAL )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "double  " );
      else
   // if ( lVariableType == qTINTEGER || lVariableType == qTSHORT  )
      if ( cOperationType == 'C' || cOperationType == 'E' )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "Integer " );
      else
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "int " );
   }
   else
   {
      if ( cReturnType == 'L' )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "int  " ); // for now, return type int
      else
      if ( cReturnType == 'M' )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "double  " ); // return type decimal
      else
      if ( cReturnType == 'O' || cReturnType == 0 )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "void " );    // return type void
      else
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "int " );     // return type short
   }

// if ( cOperationType != 'L' )
//    strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " OPERATION" );  // global operation

   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );  // was: 1 );
// OutputLine( vSubtask, lfHeaderHandle, pchOutputString, 0 );

   // Get the operation name.
   GetStringFromAttribute( pchOutputString, VM_MAX_STRING_SIZE, g_lpPIView, "Operation", "C_GeneratedOperationName" );
   if ( zstrstr( pchOutputString, "LocalBuildQual" ) != 0 )
      bLocalBuildQual = TRUE;

   // If we have any variables of any sort ...
   nRC = SetCursorFirstEntity( g_lpPIView, "Variable", "" );

   if ( nRC >= zCURSOR_SET )
      GetIntegerFromAttribute( &lParameterFlag, g_lpPIView, "Variable", "ParameterFlag" );
   else
      lParameterFlag = 0;

   // Do parameters.  If no parameters make it void.
   if ( lParameterFlag < 1 ) // no parms
   {
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( ) " );  // "( void )" does not work well in Java
      OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );  // was: 1 );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   // OutputLine( vSubtask, lfHeaderHandle,  pchOutputString, 0 );

      if ( nRC >= zCURSOR_SET )
         *pnMoreParms = zCURSOR_SET;
   }
   else  // otherwise, do parm list
   {
      // Open parm list.
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( " );

      // Play with g_nLeftMargin only for parm list processing.
      g_nLeftMargin = 0;

      // DM - as best I can tell, lParameterFlag is the position in the parm list for a variable.
      // That is, 1 for the 1st, 2 for the 2nd, and so on.  So, lParameterFlag will increase from
      // 1 as the loop progresses.  A call to SetCursorNextEntity will cause lParameterFlag to be
      // set to 0, ending the loop. DKS - This is NOT true ... lParameterFlag = 0 if local variable,
      // otherwise generally lParameterFlag = 1 if parameter, lParameterFlag = 2 if return parameter.
      while ( lParameterFlag > 0 ) // while we have parms to put out
      {
         zSHORT nLth = (zSHORT) zstrlen( pchOutputString );

         if ( nLth > 1 && pchOutputString[ nLth - 2 ] != '(' )
            GetIntegerFromAttribute( plRelLine,  g_lpPIView, "Variable", "LineNumber" );

         GetIntegerFromAttribute( &lVariableID, g_lpPIView, "Variable", "ID" );

         // Get the variable name/declaration.  If function returns TRUE then the argument is a decimal and special processing will be needed later.
         if ( GetLocalJavaDeclarationString( g_szLocalString, sizeof( g_szLocalString ), lVariableID, bLocalBuildQual, FALSE ) )
         {
            *pbDecimalArguments = TRUE;
         }

         *pnMoreParms = SetCursorNextEntity( g_lpPIView, "Variable", "" );

         if ( *pnMoreParms == zCURSOR_SET )
            GetIntegerFromAttribute( &lParameterFlag, g_lpPIView, "Variable", "ParameterFlag" );
         else
            lParameterFlag = 0;

         if ( lParameterFlag > 0 )
         {
            // After 1st time thru loop, set g_nLeftMargin to beginning of parm list.
            if ( g_nLeftMargin == 0 )
               g_nLeftMargin = (zSHORT) zstrlen( pchOutputString );

            // Output parm line.
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, g_szLocalString );
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "," );
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );  // was: 1 );
         // OutputLine( vSubtask, lfHeaderHandle,    pchOutputString, 0 );  // don't need this in Java ... dks
         }
      }

      // Close off parm list.
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, g_szLocalString );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " )" );
      OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );  // was: 1 );
   // strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   // OutputLine( vSubtask, lfHeaderHandle, pchOutputString, 0 );
   // OutputBlankLine( vSubtask, lfHeaderHandle );
   }

   return( bLocalBuildQual );
}

////////////////////////////////////////////////////////////
// Generate the local variable list.
////////////////////////////////////////////////////////////
static void
GenerateC_LocalVariableList( zVIEW  vSubtask,
                             zLONG  lfTempCFileHandle,
                             zPCHAR pchOutputString,
                             zPLONG plRelLine,
                             zSHORT nMoreParms,
                             zBOOL  bDecimalArguments )
{
   zLONG  lVariableID;
   zPCHAR pch;

   // If there are any decimal arguments we need to create a few extra variables.
   if ( bDecimalArguments )
   {
      zVIEW  vTemp;
      zLONG  lParameterFlag;
      zSHORT nRC;

      CreateViewFromViewForTask( &vTemp, g_lpPIView, 0 );
      SetNameForView( vTemp, "_GenLocalVariables", vSubtask, zLEVEL_SUBTASK );

      for ( nRC = SetCursorFirstEntity( vTemp, "Variable", 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vTemp, "Variable", 0 ) )
      {
         zLONG  lDataType;
         zPCHAR psz;
         zCHAR szPFlag[ 2 ];

         GetIntegerFromAttribute( &lParameterFlag, vTemp, "Variable", "ParameterFlag" );

         // Skip variables that are not parameters.
         if ( lParameterFlag == 0 )
            continue;

         GetIntegerFromAttribute( &lDataType, vTemp, "Variable", "DataType" );

         // Skip variables that are not decimals.
         if ( lDataType != qTDECIMAL )
            continue;

         psz = pchOutputString + g_nLeftMargin;

         zmemset( pchOutputString, ' ', g_nLeftMargin );

         // If we have a decimal return parameter we must generate "zDECIMAL*"  (HH,12.06.1998)
         szPFlag[ 0 ] = 0;
         GetStringFromAttribute( szPFlag, sizeof( szPFlag ), vTemp, "Variable", "PFlag" );
         if ( szPFlag[ 0 ] == 'Y' )
            strcpy_s( psz, VM_MAX_STRING_SIZE - (psz - pchOutputString), "zDECIMAL *" );
         else
            strcpy_s( psz, VM_MAX_STRING_SIZE - (psz - pchOutputString), "ZDecimal " );

         psz = psz + zstrlen( psz );

         GetStringFromAttribute( psz, VM_MAX_STRING_SIZE - (psz - pchOutputString), vTemp, "Variable", "Name" );
         strcat_s( psz, VM_MAX_STRING_SIZE - (psz - pchOutputString), " = " );
         psz = psz + zstrlen( psz );
         GetStringFromAttribute( psz, VM_MAX_STRING_SIZE - (psz - pchOutputString), vTemp, "Variable", "Name" );
         strcat_s( psz, VM_MAX_STRING_SIZE - (psz - pchOutputString), "_Arg_;" );

         pch = zstrstr( pchOutputString, "nZRetCode" );
         if ( pch == 0 )
            pch = zstrstr( pchOutputString, "lZRetCode" );

         if ( pch == 0 )
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
         else
            pchOutputString[ 0 ] = 0;

         // Get the variable name.
      }

      DropView( vTemp );
   }

   // Do variables.
   if ( nMoreParms == zCURSOR_SET )
   {
      // Loop thru the variable list.
      do
      {
         GetIntegerFromAttribute( plRelLine, g_lpPIView, "Variable", "LineNumber" );

         if ( g_bForwardVML_ToC )
         {
            OutputText( vSubtask, *plRelLine, lfTempCFileHandle, pchOutputString );
         }

         GetIntegerFromAttribute( &lVariableID, g_lpPIView, "Variable", "ID" );
         GetLocalC_DeclarationString( g_szLocalString, sizeof( g_szLocalString ), (zSHORT) lVariableID );

         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, g_szLocalString );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "; " );

         pch = zstrstr( pchOutputString, "nZRetCode" );
         if ( pch == 0 )
            pch = zstrstr( pchOutputString, "lZRetCode" );

         if ( pch == 0 )  // if you want a return code, specify one ... no automatics dks 2013.03.26
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
         else
            pchOutputString[ 0 ] = 0;

         nMoreParms = SetCursorNextEntity( g_lpPIView, "Variable", "" );
         GetIntegerFromAttribute( plRelLine, g_lpPIView, "Variable", "LineNumber" );

      } while( nMoreParms == zCURSOR_SET );
   }
}

static void
GenerateJavaLocalVariableList( zVIEW  vSubtask,
                               zLONG  lfTempCFileHandle,
                               zPCHAR pchOutputString,
                               zPLONG plRelLine,
                               zSHORT nMoreParms,
                               zBOOL  bDecimalArguments )
{
   zLONG  lVariableID;
   zPCHAR pch;

#if 0  // not in Java

   // If there are any decimal arguments we need to create a few extra variables.
   if ( bDecimalArguments )
   {
      zVIEW  vTemp;
      zLONG  lParameterFlag;
      zSHORT nRC;

      CreateViewFromViewForTask( &vTemp, g_lpPIView, 0 );
      SetNameForView( vTemp, "_GenLocalVariables", vSubtask, zLEVEL_SUBTASK );

      for ( nRC = SetCursorFirstEntity( vTemp, "Variable", 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vTemp, "Variable", 0 ) )
      {
         zLONG  lDataType;
         zPCHAR psz;
         zCHAR szPFlag[ 2 ];

         GetIntegerFromAttribute( &lParameterFlag, vTemp, "Variable", "ParameterFlag" );

         // Skip variables that are not parameters.
         if ( lParameterFlag == 0 )
            continue;

         GetIntegerFromAttribute( &lDataType, vTemp, "Variable", "DataType" );

         // Skip variables that are not decimals.
         if ( lDataType != qTDECIMAL )
            continue;

         psz = pchOutputString + g_nLeftMargin;

         zmemset( pchOutputString, ' ', g_nLeftMargin );

         // If we have a decimal return parameter we must generate "decimal"  (HH,12.06.1998)
         szPFlag[ 0 ] = 0;
         GetStringFromAttribute( szPFlag, sizeof( szPFlag ), vTemp, "Variable", "PFlag" );
      // if ( szPFlag[ 0 ] == 'Y' )
      //    zstrcpy( psz, "double *" );  // zstrcpy( psz, "decimal *" );
      // else
            strcpy_s( psz, commented out, "double " );   // zstrcpy( psz, "decimal " );

         psz = psz + zstrlen( psz );

         GetStringFromAttribute( psz, vTemp, "Variable", "Name" );
         strcat_s( psz, commented out, " = " );
         psz = psz + zstrlen( psz );
         GetStringFromAttribute( psz, vTemp, "Variable", "Name" );
         strcat_s( psz, commented out, "_Arg_;" );

         pch = zstrstr( pchOutputString, "nZRetCode" );
         if ( pch == 0 )
            pch = zstrstr( pchOutputString, "lZRetCode" );

         if ( pch == 0 )
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
         else
            pchOutputString[ 0 ] = 0;

         // Get the variable name.
      }

      DropView( vTemp );
   }
#endif

   // Do variables.
   if ( nMoreParms == zCURSOR_SET )
   {
      // Loop thru the variable list.
      do
      {
         GetIntegerFromAttribute( plRelLine, g_lpPIView, "Variable", "LineNumber" );

         if ( g_bForwardVML_ToC )
         {
            OutputText( vSubtask, *plRelLine, lfTempCFileHandle, pchOutputString );
         }

         GetIntegerFromAttribute( &lVariableID, g_lpPIView, "Variable", "ID" );
         GetLocalJavaDeclarationString( g_szLocalString, sizeof( g_szLocalString ), lVariableID, FALSE, TRUE );

         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, g_szLocalString );
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );

         pch = zstrstr( pchOutputString, "nZRetCode" );
         if ( pch == 0 )
            pch = zstrstr( pchOutputString, "lZRetCode" );

      // if ( pch == 0 )
      //    pch = zstrstr( pchOutputString, "RESULT" );  we want this now ... dks 2011.02.08

         if ( pch == 0 )
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
         else
            pchOutputString[ 0 ] = 0;

         nMoreParms = SetCursorNextEntity( g_lpPIView, "Variable", "" );
         GetIntegerFromAttribute( plRelLine, g_lpPIView, "Variable", "LineNumber" );

      } while( nMoreParms == zCURSOR_SET );
   }
}

///////////////////////////////////////////////////////////////
// Generate the statements next, provided there are any.
///////////////////////////////////////////////////////////////
static void
GenerateC_ControlStatements( zVIEW  vSubtask,
                             zLONG  lfTempCFileHandle,
                             zPCHAR pchOutputString,
                             zPLONG plRelLine )
{
   zLONG  lClass;
   zLONG  lNextStatement;
   zSHORT nIndent;

   if ( SetCursorFirstEntity( g_lpPIView, "Statement", "" ) < zCURSOR_SET )
      return;

   // Make sure indent and margin are set.
   g_nLeftMargin = 3;
   nIndent = 3;
   g_lStaticPrevLineNbr = 0;

   CreateViewFromViewForTask( &g_vStatementView, g_lpPIView, 0 );
   SetNameForView( g_vStatementView, "_GenStatements", vSubtask, zLEVEL_SUBTASK );

   // Loop thru each statement in the XPG and generate it.
   do
   {
      GetIntegerFromAttribute( &lClass, g_vStatementView, "Statement", "Class" );
      GetIntegerFromAttribute( plRelLine, g_vStatementView, "Statement", "RelativeLineNumber" );

      if ( g_bForwardVML_ToC )
      {
         OutputText( vSubtask, *plRelLine, lfTempCFileHandle, pchOutputString );
      }

      switch( lClass )
      {
         case qIF:
            BuildC_IfStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qELSE:
            g_nLeftMargin -= nIndent;
            BuildC_ElseStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qCASE:
            BuildC_CaseStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qOF:
            BuildC_OfStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qFOR:
         case qLOOP:
            BuildC_LoopStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qUNTIL:
            g_nLeftMargin -= nIndent;
            BuildC_UntilStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;

         case qWHILE:
            BuildC_WhileStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qRETURNEND:
         case qRETURN:
            BuildC_ReturnStatement( vSubtask, lfTempCFileHandle, pchOutputString, VM_MAX_STRING_SIZE );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;

         case qEND:
         case qENDFUNC:
            g_nLeftMargin -= nIndent;

            // This call is a temporary fix. see comments for OutputLastText!
            if ( g_bForwardVML_ToC && g_nLeftMargin == 0 )
            {
               OutputLastText( vSubtask, lfTempCFileHandle, pchOutputString );
            }

            BuildC_EndStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = ResetViewFromSubobject( g_vStatementView );
            if ( lNextStatement == zCURSOR_SET )
            {
               lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            }

            break;

         case qOTHERWISE:
            BuildC_OtherwiseStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qBREAK:
            BuildC_BreakStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = ResetViewFromSubobject( g_vStatementView );
            if ( lNextStatement == zCURSOR_SET )
            {
               lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            }

            g_nLeftMargin -= nIndent;
            break;

         case qEXPRESSION:
            BuildC_ExpressionStatement( vSubtask, lfTempCFileHandle, pchOutputString, VM_MAX_STRING_SIZE );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;

         case qTUNKNOWN:
         default:
            zmemset( pchOutputString, ' ', g_nLeftMargin );
            pchOutputString[ g_nLeftMargin ] = 0;
            sprintf_s( pchOutputString + g_nLeftMargin, VM_MAX_STRING_SIZE - g_nLeftMargin, "Some Other Statement: %d", lClass );
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;
      }

      if ( lNextStatement != zCURSOR_SET )
      {
         lNextStatement = ResetViewFromSubobject( g_vStatementView );

         if ( lNextStatement == zCURSOR_SET )
         {
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
         }
      }

   } while( lNextStatement == zCURSOR_SET );

   DropView( g_vStatementView );
}

static void
GenerateJavaControlStatements( zVIEW  vSubtask,
                               zLONG  lfTempCFileHandle,
                               zPCHAR pchOutputString,
                               zPCHAR pchReturnParameterName,
                               zPLONG plRelLine,
                               zBOOL  bLocal )
{
   zLONG  lClass;
   zLONG  lNextStatement;
   zSHORT nIndent;

   if ( SetCursorFirstEntity( g_lpPIView, "Statement", "" ) < zCURSOR_SET )
      return;

   // Make sure indent and margin are set.
   g_nLeftMargin = 3;
   nIndent = 3;
   g_lStaticPrevLineNbr = 0;

   CreateViewFromViewForTask( &g_vStatementView, g_lpPIView, 0 );
   SetNameForView( g_vStatementView, "_GenStatements", vSubtask, zLEVEL_SUBTASK );

   // Loop thru each statement in the XPJ and generate it.
   do
   {
      GetIntegerFromAttribute( &lClass, g_vStatementView, "Statement", "Class" );
      GetIntegerFromAttribute( plRelLine, g_vStatementView, "Statement", "RelativeLineNumber" );

      if ( g_bForwardVML_ToC )
      {
         OutputText( vSubtask, *plRelLine, lfTempCFileHandle, pchOutputString );
      }

      switch( lClass )
      {
         case qIF:
            BuildJavaIfStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qELSE:
            g_nLeftMargin -= nIndent;
            BuildJavaElseStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qCASE:
            BuildJavaCaseStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qOF:
            BuildJavaOfStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qFOR:
         case qLOOP:
            BuildJavaLoopStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qUNTIL:
            g_nLeftMargin -= nIndent;
            BuildJavaUntilStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;

         case qWHILE:
            BuildJavaWhileStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qRETURNEND:
         case qRETURN:
            BuildJavaReturnStatement( vSubtask, lfTempCFileHandle, pchOutputString, pchReturnParameterName, lClass, bLocal );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;

         case qEND:
         case qENDFUNC:
            g_nLeftMargin -= nIndent;

            // This call is a temporary fix. see comments for OutputLastText!
            if ( g_bForwardVML_ToC && g_nLeftMargin == 0 )
            {
               OutputLastText( vSubtask, lfTempCFileHandle, pchOutputString );
            }

            BuildJavaEndStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = ResetViewFromSubobject( g_vStatementView );
            if ( lNextStatement == zCURSOR_SET )
            {
               lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            }

            break;

         case qOTHERWISE:
            BuildJavaOtherwiseStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            g_nLeftMargin += nIndent;
            lNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
            lNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            break;

         case qBREAK:
            BuildJavaBreakStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = ResetViewFromSubobject( g_vStatementView );
            if ( lNextStatement == zCURSOR_SET )
            {
               lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            }

            g_nLeftMargin -= nIndent;
            break;

         case qEXPRESSION:
            BuildJavaExpressionStatement( vSubtask, lfTempCFileHandle, pchOutputString );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;

         case qTUNKNOWN:
         default:
            zmemset( pchOutputString, ' ', g_nLeftMargin );
            pchOutputString[ g_nLeftMargin ] = 0;
            sprintf_s( pchOutputString + g_nLeftMargin, VM_MAX_STRING_SIZE - g_nLeftMargin,
                       "Some Other Statement: %d", lClass );
            OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            break;
      }

      if ( lNextStatement != zCURSOR_SET )
      {
         lNextStatement = ResetViewFromSubobject( g_vStatementView );

         if ( lNextStatement == zCURSOR_SET )
         {
            lNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
         }
      }

   } while( lNextStatement == zCURSOR_SET );

   DropView( g_vStatementView );
}

// Returns TRUE if variable is a Decimal.
zSHORT LOCALOPER
GetLocalC_DeclarationString( zPCHAR pchLocalString,
                             zLONG  lMaxLth,
                             zSHORT lVariableID )
{
   zLONG  lDataType;
   zLONG  lParm;
   zLONG  lElements;
   zLONG  lLine = 0;
   zCHAR  szVarName[ 40 ];
   zCHAR  szNumber[ 10 ];
   zCHAR  szType[ 2 ];
   zSHORT nPointer = 0;
   zSHORT nRC = FALSE;

   SetCursorFirstEntityByInteger( g_lpPIView, "Variable", "ID", lVariableID, "" );
   GetStringFromAttribute( szVarName, sizeof( szVarName ), g_lpPIView, "Variable", "Name" );
   GetIntegerFromAttribute( &lDataType, g_lpPIView, "Variable", "DataType" );
   GetIntegerFromAttribute( &lElements, g_lpPIView, "Variable", "NElements" );

   zmemset( pchLocalString, ' ', g_nLeftMargin );
   pchLocalString[ g_nLeftMargin ] = 0;

   GetIntegerFromAttribute( &lParm, g_lpPIView, "Variable", "ParameterFlag" );

   // If we have a derived Attribute operation, the standard handling of parameter 2 and 3 is
   //  STRING(32) InternalEntityStructure    and
   //  STRING(32) InternalAttribStructure  .
   // Normal C generation yields "PCHAR InternalxStructure", and the Microsoft compiler does
   // not allow overlay of LPVIEWENTITY- respectively LPVIEWATTRIB-parameters without a cast.
   // However, we know about Attribute Operations, and we generate the correct types.
   szType[ 0 ] = 0;
   GetStringFromAttribute( szType, sizeof( szType ), g_lpPIView, "Operation", "Type" );
   if ( szType[ 0 ] == 'A' && lVariableID == 2 )
   {
      // Here we have the 2. parameter of an attribute operation.
      // By standard, this is the view entity struct.
      strcat_s( pchLocalString, lMaxLth, "LPVIEWENTITY " );
      strcat_s( pchLocalString, lMaxLth, szVarName );
   }
   else
   if ( szType[ 0 ] == 'A' && lVariableID == 3 )
   {
      // Here we have the 3. parameter of an attribute operation.
      // By standard, this is the attribute struct
      strcat_s( pchLocalString, lMaxLth, "LPVIEWATTRIB " );
      strcat_s( pchLocalString, lMaxLth, szVarName );
   }
   else
   {
      zCHAR  szUFlag[ 2 ] = { 0 };
      zCHAR  szPFlag[ 2 ] = { 0 };

      // Determine whether the Variable is unsigned.
      GetStringFromAttribute( szUFlag, sizeof( szUFlag ), g_lpPIView, "Variable", "UFlag" );

      // Determine whether the Variable is a pointer.
      GetStringFromAttribute( szPFlag, sizeof( szPFlag ), g_lpPIView, "Variable", "PFlag" );

      if ( lDataType == qTDECIMAL && lParm <= 0 )
         strcat_s( pchLocalString, lMaxLth, "Z" );  // class ZDecimal
      else
         strcat_s( pchLocalString, lMaxLth, "z" );

      if ( lParm == 2 && lDataType != qTSTRING &&
           lDataType != qTENTITYNAME && lDataType != qTATTRIBUTENAME )
      {
         strcat_s( pchLocalString, lMaxLth, "P" );
         nPointer = 1;
      }

      switch ( lDataType )
      {
         case qTINTEGER:
            if ( szUFlag[ 0 ] == 'Y' )
               strcat_s (pchLocalString, lMaxLth, "U" );

            if ( lElements == 2 )
            {
               strcat_s( pchLocalString, lMaxLth, "SHORT  " );
               lElements = 0;
            }
            else
               strcat_s( pchLocalString, lMaxLth, "LONG   " );

            break;

         case qTSHORT:
            if ( szUFlag[ 0 ] == 'Y' )
               strcat_s( pchLocalString, lMaxLth, "U" );

            strcat_s( pchLocalString, lMaxLth, "SHORT  " );
            lElements = 0;
            break;

         case qTDECIMAL:
            // Check to see if the variable is a parameter or a local variable.
            // We use zDECIMAL for arguments and ZDecimal for local variables
            // so that the operation signature doesn't contain C++ objects.
            if ( lParm > 0 )
            {
               strcat_s( pchLocalString, lMaxLth, "DECIMAL" );  // Parameter

               // Concat a string at the end of szVarName.  This will allow us
               // to create a local variable with the name szVarName.
               strcat_s( szVarName, sizeof( szVarName ), "_Arg_" );
            }
            else
               strcat_s( pchLocalString, lMaxLth, "Decimal" );  // Argument

            nRC = TRUE;
            break;

         case qTCHARACTER:
            strcat_s( pchLocalString, lMaxLth, "CHAR   " );
            break;

         case qTSTRING:
         case qTENTITYNAME:
         case qTATTRIBUTENAME:
            if ( lParm != 2 )
               strcat_s( pchLocalString, lMaxLth, "CHAR   " );
            else
            if ( GetVMLConstChar( ) && szPFlag[ 0 ] != 'Y' )
               strcat_s( pchLocalString, lMaxLth, "CPCHAR  " );
            else
               strcat_s( pchLocalString, lMaxLth, "PCHAR   " );

            break;

         case qTVIEW:
            strcat_s( pchLocalString, lMaxLth, "VIEW   " );
            break;

         default:
            strcat_s( pchLocalString, lMaxLth, "No Type " );
            break;
      }

      if ( lParm == 2 )
         strcat_s( pchLocalString, lMaxLth, " " );
      else
         strcat_s( pchLocalString, lMaxLth, "  " );

      strcat_s( pchLocalString, lMaxLth, szVarName );

      //if ( lElements > 0 && lParm != 2 )
      if ( lElements > 0 && lParm == 0 )
      {
         strcat_s( pchLocalString, lMaxLth, "[ " );
         zltoa( lElements, szNumber, sizeof( szNumber ) );
         strcat_s( pchLocalString, lMaxLth, szNumber );
         strcat_s( pchLocalString, lMaxLth, " ]" );
      }

      // Generate initialization of local variables ... only for user defined variables,
      // not for parser generated temp variables. A parser generated temp variable has
      // line number = 0.  Also, if the variable is a pointer (not STRING) we do not want
      // to initialze it (nPointer != 0).
      if ( lParm == 0 && nPointer == 0 )
      {
         GetIntegerFromAttribute( &lLine, g_lpPIView, "Variable", "LineNumber" );
         if ( lLine )
            fnGenerateC_Init( pchLocalString, lMaxLth, lDataType );
      }
   }

   return( nRC );
}

// Returns TRUE if variable is a Decimal.
zBOOL LOCALOPER
GetLocalJavaDeclarationString( zPCHAR pchLocalString,
                               zLONG  lMaxLth,
                               zLONG  lVariableID,
                               zBOOL  bLocalBuildQual,
                               zBOOL  bLocalVariables )
{
   zLONG  lDataType;
   zLONG  lParm;
   zLONG  lElements;
   zLONG  lLine = 0;
   zCHAR  szVarName[ 40 ];
   zCHAR  szType[ 2 ];
   zSHORT nPointer = 0;
   zBOOL  bDecimalArgument = FALSE;
// zCHAR  szMsgDebug[ 64 ];

   SetCursorFirstEntityByInteger( g_lpPIView, "Variable", "ID", lVariableID, "" );
   GetStringFromAttribute( szVarName, sizeof( szVarName ), g_lpPIView, "Variable", "Name" );
   GetIntegerFromAttribute( &lDataType, g_lpPIView, "Variable", "DataType" );
   GetIntegerFromAttribute( &lElements, g_lpPIView, "Variable", "NElements" );

   zmemset( pchLocalString, ' ', g_nLeftMargin );
   pchLocalString[ g_nLeftMargin ] = 0;

   GetIntegerFromAttribute( &lParm, g_lpPIView, "Variable", "ParameterFlag" );  // 0 ==> local var; otw parm

   // If we have a derived Attribute operation, the standard handling of parameter 2 and 3 is
   //  STRING(32) InternalEntityStructure    and
   //  STRING(32) InternalAttribStructure  .
   // Normal C generation yields "PCHAR InternalxStructure", and the Microsoft compiler does
   // not allow overlay of LPVIEWENTITY- respectively LPVIEWATTRIB-parameters without a cast.
   // However, we know about Attribute Operations, and we generate the correct types.
   szType[ 0 ] = 0;
   GetStringFromAttribute( szType, sizeof( szType ), g_lpPIView, "Operation", "Type" );
   if ( szType[ 0 ] == 'A' && lVariableID == 2 )
   {
      // Here we have the 2. parameter of an attribute operation.
      // By standard, this is the view entity struct.
   // strcat_s( pchLocalString, lMaxLth, "LPVIEWENTITY " ); not in Java
      strcat_s( pchLocalString, lMaxLth, "String " );
      strcat_s( pchLocalString, lMaxLth, szVarName );
   }
   else
   if ( szType[ 0 ] == 'A' && lVariableID == 3 )
   {
      // Here we have the 3. parameter of an attribute operation.
      // By standard, this is the attribute struct
   // strcat_s( pchLocalString, lMaxLth, "LPVIEWATTRIB " );  not in Java
      strcat_s( pchLocalString, lMaxLth, "String " );
      strcat_s( pchLocalString, lMaxLth, szVarName );
   }
   else
   {
      zCHAR  szUFlag[ 2 ];
      zCHAR  szPFlag[ 2 ];

      // Determine whether the Variable is unsigned.
      GetStringFromAttribute( szUFlag, sizeof( szUFlag ), g_lpPIView, "Variable", "UFlag" );

      // Determine whether the Variable is a pointer.
      GetStringFromAttribute( szPFlag, sizeof( szPFlag ), g_lpPIView, "Variable", "PFlag" );

//    if ( lDataType == qTDECIMAL && lParm <= 0 )
//       strcat_s( pchLocalString, lMaxLth, "Z" );  // class ZDecimal
//    else
//       strcat_s( pchLocalString, lMaxLth, "z" );

      if ( lParm == 2 && lDataType != qTSTRING &&
           lDataType != qTENTITYNAME && lDataType != qTATTRIBUTENAME )
      {
      // strcat_s( pchLocalString, lMaxLth, "P" );  no pointers in java
      // nPointer = 1;
      }

      switch ( lDataType )
      {
         case qTINTEGER:
         // zsprintf( szMsgDebug, "qTINTEGER: Type: %s   VariableID: %d   Elements: %d  ... ", szType, lVariableID, lElements );
         // strcat_s( pchLocalString, lMaxLth, szMsgDebug );
            if ( szUFlag[ 0 ] == 'Y' )
               strcat_s( pchLocalString, lMaxLth, "unsigned " );

            if ( lElements == 2 )
            {
               if ( (szType[ 0 ] == 'A' && lVariableID == 4) ||
                    (szType[ 0 ] == 'C' && (lVariableID == 2 || lVariableID == 3)) ||
                    (szType[ 0 ] == 'E' && (lVariableID == 3 || lVariableID == 4)) )
               {
                  strcat_s( pchLocalString, lMaxLth, "Integer " );
               }
               else
               {
                  strcat_s( pchLocalString, lMaxLth, "int    " );
               }

               lElements = 0;
            }
            else
            {
               if ( (szType[ 0 ] == 'A' && lVariableID == 4) ||
                    (szType[ 0 ] == 'C' && (lVariableID == 2 || lVariableID == 3)) ||
                    (szType[ 0 ] == 'E' && (lVariableID == 3 || lVariableID == 4)) )
               {
                  strcat_s( pchLocalString, lMaxLth, "Integer " );
               }
               else
               if ( /*GetVMLConstChar( ) == 0 ||*/ szPFlag[ 0 ] == 'Y' )
               {
                  strcat_s( pchLocalString, lMaxLth, "MutableInt " );  // for now
               }
               else
               {
               // strcat_s( pchLocalString, lMaxLth, "long   " );
                  strcat_s( pchLocalString, lMaxLth, "int    " );  // qTINTEGER: Type: A   VariableID: 6+  Elements: 0 ... qTINTEGER: Type: L   VariableID: 5-6  Elements: 0
                                                          // qTINTEGER: Type: T   VariableID: 2+  Elements: 0 ... qTINTEGER: Type: C   VariableID: 33   Elements: 0
               }
            }

            break;

         case qTSHORT:
         // zsprintf( szMsgDebug, "qTSHORT: Type: %s   VariableID: %d   Elements: %d  ... ", szType, lVariableID, lElements );
         // strcat_s( pchLocalString, lMaxLth, szMsgDebug );
            if ( szUFlag[ 0 ] == 'Y' )
               strcat_s( pchLocalString, lMaxLth, "unsigned " );

            if ( (szType[ 0 ] == 'A' && lVariableID == 4) ||  // DERIVED ATTRIBUTE OPERATION ...
                 (szType[ 0 ] == 'C' && (lVariableID == 2 || lVariableID == 3)) ||
                 (szType[ 0 ] == 'E' && (lVariableID == 3 || lVariableID == 4)) )
            {
               strcat_s( pchLocalString, lMaxLth, "Integer " );  // qTSHORT: Type: A   VariableID: 4   Elements: 2 ... qTSHORT: Type: C   VariableID: 2-3  Elements: 2
                                                        // qTSHORT: Type: E   VariableID: 3-4 Elements: 2
            }
            else
            {
               if ( /*GetVMLConstChar( ) == 0 ||*/ szPFlag[ 0 ] == 'Y' )
               {
                  strcat_s( pchLocalString, lMaxLth, "MutableInt " );  // for now
               }
               else
               {
                  strcat_s( pchLocalString, lMaxLth, "int    " );  // qTSHORT: Type: L   VariableID: 3   Elements: 2 ... qTSHORT: Type: A   VariableID: 6+   Elements: 2
                                                          // qTSHORT: Type: T   VariableID: 2+  Elements: 2 ... qTSHORT: Type: C   VariableID: 9+   Elements: 2
                                                          // qTSHORT: Type: E   VariableID: 5   Elements: 2
               }
            }

            lElements = 0;
            break;

         case qTDECIMAL:
            // Check to see if the variable is a parameter or a local variable.
            // We use zDECIMAL for arguments and ZDecimal for local variables
            // so that the operation signature doesn't contain C++ objects.
            if ( lParm > 0 )
            {
               if ( /*GetVMLConstChar( ) == 0 ||*/ szPFlag[ 0 ] == 'Y' )
                  strcat_s( pchLocalString, lMaxLth, "MutableDouble" );   // Parameter
               else
                  strcat_s( pchLocalString, lMaxLth, "double" );   // Parameter

               // Concat a string at the end of szVarName.  This will allow us
               // to create a local variable with the name szVarName.
            // strcat_s( szVarName, sizeof( szVarName ), "_Arg_" );  not in Java
            }
            else
            {
               strcat_s( pchLocalString, lMaxLth, "double" );   // Argument
            }

            bDecimalArgument = TRUE;
            break;

         case qTCHARACTER:
            strcat_s( pchLocalString, lMaxLth, "char   " );
            break;

         case qTSTRING:
         case qTENTITYNAME:
         case qTATTRIBUTENAME:
            if ( lParm != 2 || bLocalBuildQual )
               strcat_s( pchLocalString, lMaxLth, "String " );
            else
            if ( /*GetVMLConstChar( ) == 0 ||*/ szPFlag[ 0 ] == 'Y' )
               strcat_s( pchLocalString, lMaxLth, "StringBuilder " );
            else
               strcat_s( pchLocalString, lMaxLth, "String " );

            break;

         case qTVIEW:
            if ( szType[ 0 ] == 'A' && lVariableID == 1 )
               strcat_s( pchLocalString, lMaxLth, "View   " );
            else
            if ( szPFlag[ 0 ] == 'Y' || bLocalVariables )
               strcat_s( pchLocalString, lMaxLth, "zVIEW  " );
            else
               strcat_s( pchLocalString, lMaxLth, "View   " );

            break;

         default:
            strcat_s( pchLocalString, lMaxLth, "No Type " );
            break;
      }

   // if ( lParm == 2 )  not in Java ... dks 2010.03.12
   //    strcat_s( pchLocalString, lMaxLth, " " );
   // else
         strcat_s( pchLocalString, lMaxLth, "  " );

      strcat_s( pchLocalString, lMaxLth, szVarName );

#if 0  // not in Java
      //if ( lElements > 0 && lParm != 2 )
      if ( lElements > 0 && lParm == 0 )
      {
         zCHAR  szNumber[ 10 ];

         strcat_s( pchLocalString, lMaxLth, "[ " );
         zltoa( lElements, szNumber, sizeof( szNumber ) );
         strcat_s( pchLocalString, lMaxLth, szNumber );
         strcat_s( pchLocalString, lMaxLth, " ]" );
      }
#endif

      // Generate initialization of local variables ... only for user defined variables,
      // not for parser generated temp variables. A parser generated temp variable has
      // line number = 0.  Also, if the variable is a pointer (not STRING) we do not want
      // to initialze it (nPointer != 0).
      if ( lParm == 0 && nPointer == 0 )
      {
         GetIntegerFromAttribute( &lLine, g_lpPIView, "Variable", "LineNumber" );
      // if ( lLine )  unconditionally do it in Java
            fnGenerateJavaInit( pchLocalString, lMaxLth, lDataType );
      }
   }

   return( bDecimalArgument );
}

static void
fnGenerateC_Init( zPCHAR pchLocalString, zLONG lMaxLth, zLONG lDataType )
{
   switch ( lDataType )
   {
      case qTINTEGER:
      case qTSHORT:
      case qTCHARACTER:
         strcat_s( pchLocalString, lMaxLth, " = 0" );
         break;

      case qTDECIMAL:
         strcat_s( pchLocalString, lMaxLth, " = 0.0" );
         break;

      case qTSTRING:
      case qTENTITYNAME:
      case qTATTRIBUTENAME:
         strcat_s( pchLocalString, lMaxLth, " = { 0 }" );
         break;

      case qTVIEW:
         strcat_s( pchLocalString, lMaxLth, " = 0" );
         break;

      default:
         break; // we should not get here
   }
}

static void
fnGenerateJavaInit( zPCHAR pchLocalString, zLONG lMaxLth, zLONG lDataType )
{
   switch ( lDataType )
   {
      case qTINTEGER:
      case qTSHORT:
         strcat_s( pchLocalString, lMaxLth, " = 0" );
         break;

      case qTDECIMAL:
         strcat_s( pchLocalString, lMaxLth, " = 0.0" );
         break;

      case qTSTRING:
      case qTCHARACTER:
      case qTENTITYNAME:
      case qTATTRIBUTENAME:
         strcat_s( pchLocalString, lMaxLth, " = null" );
         break;

      case qTVIEW:
         strcat_s( pchLocalString, lMaxLth, " = new zVIEW( )" );
         break;

      default:
         break; // we should not get here
   }
}

zSHORT LOCALOPER
OutputBlankLine( zVIEW  vSubtask,
                 zLONG  lFileHandle )
{
   SysWriteLine( vSubtask, lFileHandle, "" );
   return( 0 );
}

zSHORT LOCALOPER
OutputLine( zVIEW  vSubtask,
            zLONG  lFileHandle,
            zPCHAR pchOutputString,
            zSHORT nNull )
{
   // Remove trailing spaces.  dks ... this is too aggressive since when putting out parameters
   // in a function definition, at times, the type and the parameter name are not separated.
// zSHORT nLth = zstrlen( pchOutputString );
// while ( nLth && pchOutputString[ nLth - 1 ] == ' ' )
//    pchOutputString[ --nLth ] = 0;

   if ( lFileHandle )
      SysWriteLine( vSubtask, lFileHandle, pchOutputString );

   if ( nNull == 0 )
      pchOutputString[ 0 ] = 0;

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////
//
// OPERATION: OutputLastText
//
// PURPOSE: This routine is a temporary patch. For some reason unknown
// to me at this time, the parser assigns a relative line number to
// the qEND token for the end of a VML operation in a strange way. Currently,
// the qEND token is given the same relative line number as the last
// VML operation. As a result, the routine OutputText (below) fails
// to pump out any VML source lines and comments that follow the
// last operation. In particular, it fails to pass the END statement
// forward to the C. In the future, it might be best to correct the
// assignment of relative line numbers to the qEND token in the parser.
//
///////////////////////////////////////////////////////////////////////////
zSHORT LOCALOPER
OutputLastText( zVIEW   vSubtask,
                zLONG   lfTempCFileHandle,
                zPCHAR  pchOutputString )
{
   zSHORT nRC;

   // Verify existance of entity to prevent core errors.
   nRC = CheckExistenceOfEntity( g_lpPIView, "OperationText" );
   if ( nRC )
   {
      return( 0 );
   }

   // While we have vml lines left in the XPG, put out the vml.
   nRC = zCURSOR_SET;
   while ( nRC == zCURSOR_SET )
   {
      // Pad out margin with spaces.
      zmemset( pchOutputString, ' ', g_nLeftMargin );
      pchOutputString[ g_nLeftMargin ] = 0;

      strcpy_s( pchOutputString + g_nLeftMargin, VM_MAX_STRING_SIZE - g_nLeftMargin, "// " );
      nRC = GetStringFromAttribute( pchOutputString + 3 + g_nLeftMargin, VM_MAX_STRING_SIZE - 3 - g_nLeftMargin,
                                    g_lpPIView, "OperationText", "Text" );

      // Skip blank lines.
      if ( pchOutputString[ 3 + g_nLeftMargin ] != 0 )
      {
         OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
      }

      nRC = SetCursorNextEntity( g_lpPIView, "OperationText", "" );
   }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////
//
// OPERATION: OutputText
//
// PURPOSE: This routine will write out VML to the C source as comments.
//
///////////////////////////////////////////////////////////////////////////
zSHORT LOCALOPER
OutputText( zVIEW   vSubtask,
            zLONG   lRelLine,
            zLONG   lfTempCFileHandle,
            zPCHAR  pchOutputString )
{
   zLONG  lRelText;
   zLONG  k;
   zSHORT nRC;

   // Verify existance of entity to prevent core errors.
   nRC = CheckExistenceOfEntity( g_lpPIView, "OperationText" );
   if ( nRC )
   {
      return( 0 );
   }

   // Get current relative text line.
   GetIntegerFromAttribute( &lRelText, g_lpPIView, "OperationText", "RelativeLineNumber" );

   // While we have vml lines that come before generated c/java put out the vml.
   nRC = zCURSOR_SET;
   while ( lRelText <= lRelLine && nRC == zCURSOR_SET )
   {
      // Pad out margin with spaces.
      zmemset( pchOutputString, ' ', g_nLeftMargin );
      pchOutputString[ g_nLeftMargin ] = 0;

      strcpy_s( pchOutputString + g_nLeftMargin, VM_MAX_STRING_SIZE - g_nLeftMargin, "//:" );
      nRC = GetStringFromAttribute( pchOutputString + 3 + g_nLeftMargin, VM_MAX_STRING_SIZE -3 - g_nLeftMargin,
                                    g_lpPIView, "OperationText", "Text" );

      // Remove leading spaces in "commented VML" ... dks 2004.01.29
      k = strspn( pchOutputString + 3 + g_nLeftMargin, " " );  // count blanks starting from left margin
      if ( k > g_nLeftMargin )  // take out a maximum of g_nLeftMargin blanks
         k = g_nLeftMargin;

      if ( k > 0 )
         strcpy_s( pchOutputString + 3 + g_nLeftMargin, VM_MAX_STRING_SIZE - 3 - g_nLeftMargin, pchOutputString + 3 + g_nLeftMargin + k );

   // if ( zstrstr( pchOutputString, "CloneWindow" ) || zstrstr( pchOutputString, "PositionOnVOR" ) )
   // {
   //    MessageBox( 0, "DKS: ", pchOutputString, MB_OK );
   // }

      // Skip blank lines.  // no no no ... blank lines are nice!  dks 2004.01.29
      if ( pchOutputString[ 3 + g_nLeftMargin ] == 0 )
         OutputLine( vSubtask, lfTempCFileHandle, "", 0 );
      else
         OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );

      nRC = SetCursorNextEntity( g_lpPIView, "OperationText", "" );

      if ( nRC == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lRelText, g_lpPIView, "OperationText", "RelativeLineNumber" );
      }
   }

   if ( lRelText == lRelLine )
   {
      SetCursorNextEntity( g_lpPIView, "OperationText", "" );
   }

   return( 0 );
}

zSHORT LOCALOPER
OutputHeaderFiles( zVIEW  vSubtask,
                   zLONG  lfCFileHandle,
                   zPCHAR pchFileName,
                   zPCHAR pchOutputString,
                   zLONG  lMaxLth )
{
   zVIEW  vHeader;
   zLONG  lZKey;
   zSHORT nIsLocal;
   zSHORT k;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( g_lpPIView, "ExternalHeaderFile", "" );

   nIsLocal = 0;
   while ( nRC == zCURSOR_SET )
   {
      strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "#include \"" );
      GetIntegerFromAttribute( &lZKey, g_lpPIView, "ExternalHeaderFile", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( vSubtask, &vHeader, 0, zREFER_HDR_META,
                                   zSINGLE, lZKey, zCURRENT_OI );
      if ( nRC == 1 )
      {
         nRC = GetStringFromAttribute( pchOutputString + 10, lMaxLth - 10, vHeader, "HeaderFile", "Name" );
      }
      else
      {
         vHeader = 0;
         nRC = SetCursorFirstEntityByInteger( g_lpZOListView, "HeaderFile", "ZKey", lZKey, "" );
         if ( nRC == zCURSOR_SET )
         {
            // Skip the entry KZOEP1AA.C since it is automatically included by KZOENGAA.H.
            if ( CompareAttributeToString( g_lpZOListView, "HeaderFile", "Name", "KZOEP1AA" ) == 0 )
            {
               nRC = SetCursorNextEntity( g_lpPIView, "ExternalHeaderFile", "" );
               continue;
            }


            GetStringFromAttribute( pchOutputString + 10, lMaxLth - 10, g_lpZOListView, "HeaderFile", "Name" );
         }
         else
         {
            TraceLineI( "Unable to locate header file ", lZKey );
         }
      }

      if ( zstrcmpi( pchFileName, pchOutputString + 10 ) == 0 ) // this header
      {
         nIsLocal = 1;
         pchOutputString[ 9 ] = '\"';
      }

      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "." );
      k = (zSHORT) zstrlen( pchOutputString );

      if ( vHeader != 0 )
      {
         nRC = GetStringFromAttribute( pchOutputString + k, lMaxLth - k, vHeader, "HeaderFile", "Extension" );
         DropMetaOI( vSubtask, vHeader );  // drop oi
      }
      else
      {
         nRC = GetStringFromAttribute( pchOutputString + k, lMaxLth - k, g_lpZOListView, "HeaderFile", "Extension" );
      }

      if ( pchOutputString[ k ] == 0 )
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "H" );

      if ( nIsLocal == 1 )
      {
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "\"" );
      }
      else
         strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "\"" );

      nIsLocal = 0;
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " " );
      OutputLine( vSubtask, lfCFileHandle, pchOutputString, 0 );
      nRC = SetCursorNextEntity( g_lpPIView, "ExternalHeaderFile", "" );
   }

   // Always add the ZDRVROPR.H statement.  This is a temporary fix made on
   // 1/27/1996 by DonC.  It should probably be handled somewhere in the Parser.
   // It will check to see if ZDRVROPR.H was added for a defined item, but
   // will force it in here if it wasn't.  This is because using a Driver
   // operation, such as RefreshCtrl, does not cause an ExternalHeaderFile
   // entry to be created.
   SetCursorFirstEntityByString( g_lpZOListView, "HeaderFile", "Name", "ZDRVROPR", 0 );
   GetIntegerFromAttribute( &lZKey, g_lpZOListView, "HeaderFile", "ZKey" );
   if ( SetCursorFirstEntityByInteger( g_lpPIView, "ExternalHeaderFile", "ZKey", lZKey, 0 ) < 0 )
   {
      OutputLine( vSubtask, lfCFileHandle, "#include \"ZDRVROPR.H\"", 1 );
   }

   // Add "#ifdef __cplusplus..." so that if the current file is to be
   // compiled as C++ all the operations will be undecorated.
   OutputLine( vSubtask, lfCFileHandle, " ", 1 );
   OutputLine( vSubtask, lfCFileHandle, "#ifdef __cplusplus", 1 );
   OutputLine( vSubtask, lfCFileHandle, "extern \"C\"", 1 );
   OutputLine( vSubtask, lfCFileHandle, "{", 1 );
   OutputLine( vSubtask, lfCFileHandle, "#endif", 1 );
   OutputLine( vSubtask, lfCFileHandle, " ", 1 );

   // Toss in include for LPLR-wide header file.
   OutputLine( vSubtask, lfCFileHandle, "#include \"ZEIDONOP.H\"", 1 );

   return( 0 );
}

zSHORT LOCALOPER
BuildC_CaseStatement( zVIEW  vSubtask,
                      zLONG  lfTempCFileHandle,
                      zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "switch" );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( " );
   GenerateNewExpression( vSubtask, pchOutputString );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " )" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_ElseStatement( zVIEW  vSubtask,
                      zLONG  lfTempCFileHandle,
                      zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "} " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "else" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_EndStatement( zVIEW  vSubtask,
                     zLONG  lfTempCFileHandle,
                     zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "} " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   OutputBlankLine( vSubtask, lfTempCFileHandle );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_IfStatement( zVIEW  vSubtask,
                    zLONG  lfTempCFileHandle,
                    zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "if " );

   // Temporarily adjust g_nLeftMargin to line up boolean expression.
   g_nLeftMargin = g_nLeftMargin + 4;
   GenerateNewExpression( vSubtask, pchOutputString );
   g_nLeftMargin = g_nLeftMargin - 4;
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}


zSHORT LOCALOPER
BuildC_LoopStatement( zVIEW  vSubtask,
                      zLONG  lfTempCFileHandle,
                      zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "do " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_OfStatement( zVIEW  vSubtask,
                    zLONG  lfTempCFileHandle,
                    zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "case " );
   GenerateNewExpression( vSubtask, pchOutputString );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " :" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_ReturnStatement( zVIEW  vSubtask,
                        zLONG  lfTempCFileHandle,
                        zPCHAR pchOutputString,
                        zLONG  lMaxLth )
{
   zCHAR  szExprCode[ 1200 ];
   zULONG uCodeIndex = sizeof( szExprCode );
   zPCHAR pch;

   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;

   // The following GetBlobFromAttribute and if were added to eliminate
   // the call to AppendExpression if there was no such expression
   // (which generated a bad parameter on the return statement).
   GetBlobFromAttribute( szExprCode, &uCodeIndex, g_vStatementView, "Statement", "Expression" );

   if ( uCodeIndex == 0 )
   {
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "return" );
   }
   else
   {
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "return( " );
      GenerateNewExpression( vSubtask, pchOutputString );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " )" );
      pch = zstrstr( pchOutputString, "return( ( 0 ) )" );
      if ( pch )
      {
         strcpy_s( pch + 7, lMaxLth - ((pch + 7)- pchOutputString), pch + 9 );
         strcpy_s( pch + 11, lMaxLth - ((pch + 11) - pchOutputString), pch + 13 );
      }
   }

   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_UntilStatement( zVIEW  vSubtask,
                       zLONG  lfTempCFileHandle,
                       zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;

   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "} " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " while " );

   // Temporarily adjust g_nLeftMargin to line up boolean expression.
   g_nLeftMargin = g_nLeftMargin + 10;
   GenerateNewExpression( vSubtask, pchOutputString );
   g_nLeftMargin = g_nLeftMargin - 10;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   OutputBlankLine( vSubtask, lfTempCFileHandle );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_WhileStatement( zVIEW  vSubtask,
                       zLONG  lfTempCFileHandle,
                       zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "while " );

   // Temporarily adjust g_nLeftMargin to line up boolean expression.
   g_nLeftMargin = g_nLeftMargin + 7;
   GenerateNewExpression( vSubtask, pchOutputString );
   g_nLeftMargin = g_nLeftMargin - 7;
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_OtherwiseStatement( zVIEW  vSubtask,
                           zLONG  lfTempCFileHandle,
                           zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "default " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " :" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_BreakStatement( zVIEW  vSubtask,
                       zLONG  lfTempCFileHandle,
                       zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;

   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "break " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildC_ExpressionStatement( zVIEW  vSubtask,
                            zLONG  lfTempCFileHandle,
                            zPCHAR pchOutputString,
                            zLONG  lMaxLth )
{
   zPCHAR pch;

   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   GenerateNewExpression( vSubtask, pchOutputString );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   pch = zstrstr( pchOutputString, "nZRetCode = " );
   if ( pch == 0 )
      pch = zstrstr( pchOutputString, "lZRetCode = " );

   if ( pch  )
      strcpy_s( pch, lMaxLth - (pch - pchOutputString), pch + 12 );

   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaCaseStatement( zVIEW  vSubtask,
                        zLONG  lfTempCFileHandle,
                        zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "switch" );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "( " );
   GenerateNewExpression( vSubtask, pchOutputString );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " )" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaElseStatement( zVIEW  vSubtask,
                        zLONG  lfTempCFileHandle,
                        zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "} " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "else" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaEndStatement( zVIEW  vSubtask,
                       zLONG  lfTempCFileHandle,
                       zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "} " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   OutputBlankLine( vSubtask, lfTempCFileHandle );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaIfStatement( zVIEW  vSubtask,
                      zLONG  lfTempCFileHandle,
                      zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "if " );

   // Temporarily adjust g_nLeftMargin to line up boolean expression.
   g_nLeftMargin = g_nLeftMargin + 4;
   GenerateNewExpression( vSubtask, pchOutputString );
   g_nLeftMargin = g_nLeftMargin - 4;
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}


zSHORT LOCALOPER
BuildJavaLoopStatement( zVIEW  vSubtask,
                        zLONG  lfTempCFileHandle,
                        zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "do " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaOfStatement( zVIEW  vSubtask,
                      zLONG  lfTempCFileHandle,
                      zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "case " );
   GenerateNewExpression( vSubtask, pchOutputString );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " :" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaReturnStatement( zVIEW  vSubtask,
                          zLONG  lfTempCFileHandle,
                          zPCHAR pchOutputString,
                          zPCHAR pchReturnParameterName,
                          zLONG  lClass,
                          zBOOL  bLocal )
{
   zCHAR  szExprCode[ 1200 ];
   zULONG uCodeIndex = sizeof( szExprCode );
   zPCHAR pch;

   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;

   // The following GetBlobFromAttribute and if were added to eliminate
   // the call to AppendExpression if there was no such expression
   // (which generated a bad parameter on the return statement).
   GetBlobFromAttribute( szExprCode, &uCodeIndex, g_vStatementView, "Statement", "Expression" );

   if ( uCodeIndex == 0 )
   {
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "return" );
   }
   else
   {
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "return( " );
      GenerateNewExpression( vSubtask, pchOutputString );
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " )" );
      pch = zstrstr( pchOutputString, "return( ( 0 ) )" );
      if ( pch )
      {
         strcpy_s( pch + 7, VM_MAX_STRING_SIZE - ((pch + 7) - pchOutputString), pch + 9 );
         strcpy_s( pch + 11, VM_MAX_STRING_SIZE - ((pch + 11) - pchOutputString), pch + 13 );
      }
      else
      {
         pch = zstrstr( pchOutputString, "return( ( null ) )" );
         if ( pch )
         {
            strcpy_s( pch + 7, VM_MAX_STRING_SIZE - ((pch + 7) - pchOutputString), pch + 9 );
            strcpy_s( pch + 14, VM_MAX_STRING_SIZE - ((pch + 14) - pchOutputString), pch + 16 );
         }
      }
   }

   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );

   if ( lClass == 1037 || bLocal ) // zRETURNEND
   {
      pch = zstrstr( pchOutputString, "if(8==8)" );
      if ( pch )
         strcpy_s( pch, VM_MAX_STRING_SIZE - (pch - pchOutputString), pch + 8 );

#if 0  // not going to do this anymore ... implemented mutable return parameters.
      if ( pchReturnParameterName[ 0 ] )
      {
         pch = zstrchr( pchOutputString, '(' );
         if ( pch )
         {
            zCHAR  szEnd[ 256 ];
            zPCHAR pchClose = zstrchr( pch + 1, ')' );

            strcpy_s( szEnd, sizeof( szEnd ) pchClose );
            strcpy_s( pch + 1, commented out, " " );
            strcpy_s( pch + 2, commented out, pchReturnParameterName );
            strcat_s( pch + 2, commented out, " " );
            strcat_s( pch + 2, commented out, szEnd );
         }
         else
         {
            pch = zstrstr( pchOutputString, "return" );
            strcpy_s( pch + 6, commented out, " " );
            strcpy_s( pch + 7, commented out, pchReturnParameterName );
            strcat_s( pch + 7, commented out, ";" );
         }
      }
#endif
   }

   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaUntilStatement( zVIEW  vSubtask,
                         zLONG  lfTempCFileHandle,
                         zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;

   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "} " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " while " );

   // Temporarily adjust g_nLeftMargin to line up boolean expression.
   g_nLeftMargin = g_nLeftMargin + 10;
   GenerateNewExpression( vSubtask, pchOutputString );
   g_nLeftMargin = g_nLeftMargin - 10;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   OutputBlankLine( vSubtask, lfTempCFileHandle );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaWhileStatement( zVIEW  vSubtask,
                         zLONG  lfTempCFileHandle,
                         zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "while " );

   // Temporarily adjust g_nLeftMargin to line up boolean expression.
   g_nLeftMargin = g_nLeftMargin + 7;
   GenerateNewExpression( vSubtask, pchOutputString );
   g_nLeftMargin = g_nLeftMargin - 7;
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "{ " );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaOtherwiseStatement( zVIEW  vSubtask,
                             zLONG  lfTempCFileHandle,
                             zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "default " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " :" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaBreakStatement( zVIEW  vSubtask,
                         zLONG  lfTempCFileHandle,
                         zPCHAR pchOutputString )
{
   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;

   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "break " );
   strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );
   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

zSHORT LOCALOPER
BuildJavaExpressionStatement( zVIEW  vSubtask,
                              zLONG  lfTempCFileHandle,
                              zPCHAR pchOutputString )
{
   zPCHAR pch;

   zmemset( pchOutputString, ' ', g_nLeftMargin );
   pchOutputString[ g_nLeftMargin ] = 0;
   GenerateNewExpression( vSubtask, pchOutputString );
   if ( pchOutputString[ 0 ] && pchOutputString[ zstrlen( pchOutputString ) - 1 ] != '}' )
      strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ";" );

   pch = zstrstr( pchOutputString, "nZRetCode = " );
   if ( pch == 0 )
      pch = zstrstr( pchOutputString, "lZRetCode = " );

   if ( pch  )
      strcpy_s( pch, VM_MAX_STRING_SIZE - (pch - pchOutputString), pch + 12 );

   pch = zstrstr( pchOutputString, "setFirst(," );
   if ( pch )
      strcpy_s( pch + 9, VM_MAX_STRING_SIZE - ((pch + 9) - pchOutputString), pch + 10 );

   pch = zstrstr( pchOutputString, "setNext(," );
   if ( pch )
      strcpy_s( pch + 8, VM_MAX_STRING_SIZE - ((pch + 8) - pchOutputString), pch + 9 );

   pch = zstrstr( pchOutputString, "setLast(," );
   if ( pch )
      strcpy_s( pch + 8, VM_MAX_STRING_SIZE - ((pch + 8) - pchOutputString), pch + 9 );

   OutputLine( vSubtask, lfTempCFileHandle, pchOutputString, 0 );
   return( 0 );
}

static zVOID
WritePrototypesForLocalC( zVIEW  vSubtask,
                          zLONG  lfCFileHandle,
                          zPCHAR pchOutputString )
{
   zVIEW  v;
   zLONG  lLine = 0;
   zSHORT n;

   CreateViewFromViewForTask( &v, g_vSourceMeta, 0 );
   SetNameForView( v, "_GenPrototypes", vSubtask, zLEVEL_SUBTASK );

   n = SetCursorFirstEntityByString( v, "SourceFile", "LanguageType", "C", "" );
   while ( n >= zCURSOR_SET )
   {
      zSHORT k;
      k = SetCursorFirstEntity( v, "Operation", "" );
      while ( k >= zCURSOR_SET )
      {
         zCHAR szType[ 2 ];
         szType[ 0 ] = 0;
         GetStringFromAttribute( szType, sizeof( szType ), v, "Operation", "Type" );
         if ( szType[ 0 ] == 'L' )
         {
            zCHAR szReturnDataType[ 2 ];
            zCHAR sz[ 256 ];
            zSHORT m;
            zSHORT nCount = 0;

            // It is a local Operation - so generate the prototype, whether it is called or not.

            // return type
            GetStringFromAttribute( szReturnDataType, sizeof( szReturnDataType ), v, "Operation", "ReturnDataType" );
            switch ( szReturnDataType[ 0 ] )
            {
               case 'M':
                  strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "zDECIMAL " ); // decimal
                  break;
               case 'L':
                  strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "zLONG " );    // long
                  break;
               default:
                  strcpy_s( pchOutputString, VM_MAX_STRING_SIZE, "zSHORT " );   // short
                  break;
            }

            GetStringFromAttribute( sz, sizeof( sz ), v, "Operation", "Name" );
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, sz );
            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "(" );
            m = SetCursorFirstEntity( v, "Parameter", "" );
            if ( m < zCURSOR_SET )
            {
               strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " void" );
            }

            while ( m >= zCURSOR_SET )
            {
               zCHAR szDataType[ 2 ];
               zCHAR szPFlag[ 2 ];
               zCHAR szUFlag[ 2 ];

               szDataType[ 0 ] = 0;
               szPFlag[ 0 ] = szUFlag[ 0 ] = 'N';

               GetStringFromAttribute( szDataType, sizeof( szDataType ), v, "Parameter", "DataType" );
               GetStringFromAttribute( szPFlag, sizeof( szPFlag ), v, "Parameter", "PFlag" );
               GetStringFromAttribute( szUFlag, sizeof( szUFlag ), v, "Parameter", "UFlag" );
               if ( nCount )
                  strcat_s( pchOutputString, VM_MAX_STRING_SIZE, ", " );
               else
                  strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " " );

               nCount++;

               switch ( szDataType[ 0 ] )
               {
                  case 'S':
                     // String
                     strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zPCHAR" );
                     break;

                  case 'M':
                     // Decimal
                     strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zDECIMAL" );
                     if ( szPFlag[ 0 ] != 'N' )
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "*" );

                     break;

                  case 'L':
                     // long
                     if ( szUFlag[ 0 ] != 'N' )
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zULONG" );
                     else
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zLONG" );

                     if ( szPFlag[ 0 ] != 'N' )
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "*" );

                     break;

                  case 'V':
                     // view
                     strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zVIEW" );
                     if ( szPFlag[ 0 ] != 'N' )
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "*" );

                     break;

                  default:
                     // short
                     if ( szUFlag[ 0 ] != 'N' )
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zUSHORT" );
                     else
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "zSHORT" );

                     if ( szPFlag[ 0 ] != 'N' )
                        strcat_s( pchOutputString, VM_MAX_STRING_SIZE, "*" );

                     break;
               }

               m = SetCursorNextEntity( v, "Parameter", "" );
            } // for each parameter

            strcat_s( pchOutputString, VM_MAX_STRING_SIZE, " );" );
            FormatString( pchOutputString );
            OutputLine( vSubtask, lfCFileHandle, pchOutputString, 0 );
            lLine++;
         }

         k = SetCursorNextEntity( v, "Operation", "" );
      } // for each operation

      n = SetCursorNextEntityByString( v, "SourceFile", "LanguageType", "S", "" );
   }  // for each "C" source file

   if ( lLine )
      OutputBlankLine( v, lfCFileHandle );

   DropView( v );
}
