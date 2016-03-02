/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp2aa.c - VML SubSystem Parser Add Entities Module
// DESCRIPTION:   This is the module that contains all of the functions
//                to add delete modify entities in the parsed object.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/07/22
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9A   1995/08/30
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2002.05.07 HH
   Fatal error exit in AddOperationEntityToPI if a function is already defined.
   Otherwise Invalid View error messages might come up.
2000.09.09 HH
   improve handling of generated operations (number = ZKey).
2001.04.18 HH
   correct old pwd's where dialog operations have no ReturnDataType.
2001.02.12 HH
   implement Set Cursor with Object Scope.
2000.12.27 HH
   support § (paragraph sign).
2000.09.17 HH
   do not show generated operations in list
2000.08.28 HH
   fix problem with operation returncodes.
2000.08.08  HH
   Added NewAddExpressionEntryToString.
2000.06.14  HH
   do not strip quotes any more in AddExpressionEntryToString.
2000.02.15  HH
   move CreateSourceZeidonStrOpS,CreateSourceZeidonStrOp,
   CreateSourceZeidonStrOpWithID,CreateSourceZeidonStrOpsWithID
   CreateTargetZeidonStrOp, CreateTargetZeidonStrOpS,
   CreateTargetZeidonStrOpWithID, CreateTargetZeidonStrOpSWithID
   to tzvmp7aa.c,
   and removed "deleting" of source view entities.

OLD CHANGE LOG
20.07.1997  DonC
   Corrected handling of szExprCode and szTextCode so that IF statements
   could handle a larger number of expressions within the IF.

06.01.1998  DonC
   Modified AddVariableEntityToPI to handle VMLType of qTDATE.

18.06.1998  HH
   In CreateConvertZeidonStrOp, Section 2: if the number, for which
   ZeidonStringConvertFromNumber is generated, is an integer, then the
   decimal string (which has no meaning in that case) is "0.0" instead
   of "0". Consequence: The C-Generation (see TZVMG2AA.C) generates a
   (ZDecimal) cast, which is needed since 9J.

17.07.1998  HH
   created NewAddExprNodeEntityToView to handle Flags (needed for
   unsigned parameters ).

16.10.1998  HH
   prototype GetOperationParmListPM changed (TB363).

30.10.1998  HH
   created function GermanUmlautHack, and use it to fix bug TB424.

30.10.1998  HH
   fix "debugger no source" bug (no buglist entry ?).
   Create a new operation in editor (not defined yet!), parse,
   debug --- no lines in the source window.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"

//#define DEBUG_PARSEGEN 1
#ifdef DEBUG_PARSEGEN

typedef struct qDefines
{
   zLONG lType;
   zPCHAR pchName;

} qDefines;

qDefines g_qDefines[ ] =
{
   1002, "qAFTER",
   1003, "qEACH",
   1004, "qTATTRIBUTENAME",
   1005, "qBEFORE",
   1006, "qBASED",
   1007, "qCASE",
   1009, "qCONDOPERATOR",
   1010, "qCONSTANT",
   1011, "qCREATE",
   1012, "qCURSOR",
   1013, "qCOMMIT",
   1014, "qDELETE",
   1015, "qELSE",
   1016, "qEND",
   1017, "qENTITY",
   1018, "qTENTITYNAME",
   1019, "qTDATE",
   1020, "qFIRST",
   1021, "qFOR",
   1022, "qIF",
   1024, "qJOIN",
   1025, "qLAST",
   1026, "qACTIVATE",
   1027, "qLOOP",
   1028, "qLOD",
   1029, "qFORIF",
   1030, "qNEXT",
   1032, "qOF",
   1033, "qOPERATION",
   1034, "qOPERATOR",
   1035, "qPREVIOUS",
   1036, "qRETURN",
   1037, "qRETURNEND",
   1038, "qSET",
   1039, "qTDECIMAL",
   1040, "qTINTEGER",
   1041, "qTSHORT",
   1042, "qTSTRING",
   1043, "qUNTIL",
   1044, "qVARIABLE",
   1045, "qTVIEW",
   1046, "qWHERE",
   1047, "qWHILE",
   1048, "qWITHIN",
   1049, "qOTHERWISE",
   1050, "qON",
   1051, "qAND",
   1052, "qOR",
   1053, "qGLOBAL",
   1054, "qLOCAL",
   1055, "qOBJECT",
   1056, "qDIALOG",
   1057, "qDOMAIN",
   1058, "qRETURNED",
   1059, "qBREAK",
   1060, "qDEFINE",
   1061, "qRESTRICTING",
   1062, "qTO",
   1063, "qMULTIPLE",
   1064, "qROOTONLY",
   1065, "qSINGLE",
   1066, "qROOTONLYMULTIPLE",
   1067, "qEMPTY",
   1068, "qLIKE",
   1069, "qEXISTS",
   1070, "qEXIST",
   1071, "qDOES",
   1072, "qTCHARACTER",
   1073, "qIS",
   1074, "qNULL",
   1075, "qATTRIBUTE",
   1076, "qDERIVED",
   1077, "qCONSTRAINT",
   1078, "qTRANSFORMATION",
   1079, "qSHORTCUTKEY",
   1080, "qREGISTERED",
   1081, "qAS",
   1082, "qGET",
   1083, "qAT",
   1084, "qLEVEL",
   1085, "qSUBTASK",
   1086, "qTASK",
   1087, "qAPPLICATION",
   1088, "qSYSTEM",
   1089, "qANY",
   1090, "qNAME",
   1091, "qUSING",
   1092, "qINCLUDE",
   1093, "qEXCLUDE",
   1094, "qFROM",
   1095, "qNONE",
   1096, "qNAMED",
   1100, "qTUNKNOWN",
   1101, "qTVOID",
   1102, "qCONTINUE",
   1103, "qROOTONLYCONTINUE",
   1104, "qSINGLEFORUPDATE",
   2001, "qATTRIBUTEQUALIFIER",
   2002, "qENTITYQUALIFIER",
   2003, "qRESERVEDWORD",
   2004, "qUSERDEFINED",
   2005, "qDELIMITER",
   2006, "qEXPRESSION",
   2007, "qENDFUNC",
   2008, "qOPERATIONCALL",
   2009, "qNUMERIC",
   2010, "qTEXT",
   3001, "qEQUAL",
   3002, "qMINUS",
   3003, "qADDITION",
   3004, "qMULTIPLICATION",
   3005, "qDIVISION",
   3006, "qCONDEQUAL",
   3007, "qLESSTHANEQUAL",
   3008, "qLESSTHAN",
   3009, "qGREATERTHANEQUAL",
   3010, "qGREATERTHAN",
   3011, "qNOTEQUAL",
   3012, "qNOT",
   3013, "qCOMMA",
   3014, "qLPAREN",
   3015, "qRPAREN",
   3016, "qLBRACE",
   3017, "qRBRACE",
   3018, "qOPENOPER",
   3019, "qCLOSEOPER",
   3020, "qSEMICOLON",
   3021, "qCOLON",
   3022, "qNOTEXISTS",
   3023, "qNOTNULL",
   3024, "qPERIOD",
   3031, "qUNKNOWNTOKEN",
   3032, "qUNARYMINUS",
   4001, "qLOCALOPERATION",
   4002, "qGLOBALOPERATION",
   4003, "qOBJECTOPERATION",
   4004, "qDIALOGOPERATION",
   4005, "qDOMAINOPERATION",
   4006, "qZEIDONOPERATION",
   4007, "qCONTROLOPERATION",
   5001, "qBOTHVAR",
   5002, "qCONCATOPERATOR",
   -1,   "UNKNOWN",
   -2,   "Bottom of Stack",
   0,    "Not Set"
};

void
fnFindDefine( zPCHAR pchName, zLONG lMaxLth, zLONG lType )
{
   zSHORT k = 0;

   while ( g_qDefines[ k ].lType )
   {
      if ( g_qDefines[ k ].lType == lType )
         break;

      k++;
   }

   sprintf_s( pchName, lMaxLth, "[%4d]:%s", g_qDefines[ k ].lType, g_qDefines[ k ].pchName );
}

void
TraceExpressionStruct( sQExprStruct *psExpr, zLONG lExprIdx, zCPCHAR cpcMsg1, zCPCHAR cpcMsg2 )
{
   // zCHAR   szMsg[ 1024 ];
   zCHAR   szElementClass[ 32 ];
   zCHAR   szDataClass[ 32 ];
   zCHAR   szDataType[ 32 ];
   zCHAR   szOffset[ 32 ];
   zCHAR   szLevel[ 32 ];

   if ( psExpr->lFlags )
      TraceLineX( "Flags: ", psExpr->lFlags );

   fnFindDefine( szElementClass, zsizeof( szElementClass ), psExpr->lElementClass );
   fnFindDefine( szDataClass, zsizeof( szDataClass ), psExpr->lDataClass );
   fnFindDefine( szDataType, zsizeof( szDataType ), psExpr->lDataType );
   fnFindDefine( szOffset, zsizeof( szOffset ), psExpr->lOffset );
   fnFindDefine( szLevel, zsizeof( szLevel ), psExpr->lLevel );
   TraceLine( "Expr ECls:%-24s  DCls:%-24s  DType:%-24s  Offset:%-24s  Lvl:%-20s  Flg:%x  Cnt:%3d  Idx:%4d  %s%s",
              szElementClass, szDataClass, szDataType, szOffset, szLevel, psExpr->lFlags, psExpr->lCount, lExprIdx, cpcMsg1, cpcMsg2 );
// zsprintf( szMsg, "ExprStruct Element Class: %4d:%16s   Data Class: %4d:%16s   Data Type: %10d:%16s   Offset: %8d:%16s   Level: %4d:%16s   Flags: %x   Idx: %d",
//           psExpr->lElementClass, szElementClass, psExpr->lDataClass, szDataClass,
//           psExpr->lDataType, szDataType, psExpr->lOffset, szOffset, psExpr->lLevel, szLevel, psExpr->lFlags, nExprIdx );
// TraceLineS( szMsg, "" );
}

zSHORT
TraceStatementExpression( zCPCHAR cpcMsg, zCPCHAR cpcExpression, zLONG lExprIdx )
{
   sQExprStruct *psExpr;         // pointer to expression structure within szExprCode
   zSHORT nCnt = 0;

   TraceLine( "Beginning TraceStatementExpression: %s   Lth: %d", cpcMsg, lExprIdx );
   lExprIdx -= (sizeof( sQExprStruct ) - MAXSTRING); // point to beginning of last ExprStruct
   while ( lExprIdx >= 0 )
   {
      nCnt++;
      psExpr = (sQExprStruct *) ((zPCHAR) cpcExpression + lExprIdx);
      TraceExpressionStruct( psExpr, lExprIdx, "", "" );  // display this expression structure
      lExprIdx -= (sizeof( sQExprStruct ) - MAXSTRING);
   }

   TraceLineS( "", "" );
   return( nCnt );
}

#endif

static zLONG
fnZKeyForfnLocal( zCPCHAR cpcOperationName );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS : PreBuildOperation
//              AddOperationEntityToPI
//              ResetOperation
//              AddVariableEntityToPI
//              AddStatementEntityToPI
//              AddStatementEntityWithStringToPI
//              AddExprNodeEntityToView
//              AddExpressionEntryToString
//              AddOperatorToView
//              InsertExprNodeValuesToEntity
//              IsCondOperator
//              CreateConvertZeidonStrOp
//              AddEntityOperWithScope
//              AddEntityOperWithNoScope
//              AddEntityOperWithEntity
//              AddEntityOperToViewWithNoScope
//
/////////////////////////////////////////////////////////////////////////////

#define ADDEXPRESSIONENTRYTOSTRING( p1, p2, p3, p4, p5, p6 ) AddExpressionEntryToString( szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, p1, p2, p3, p4, p5, p6 )

#define ADDCOMMAEXPRESSIONTOSTRING AddExpressionEntryToString( szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 )

zCHAR szLocal2Code[ EXPRCODE_MAXSTRING ];
//zLONG selMemory;           // Memory handle for allocating szLocal2Code
//zPCHAR szLocal2Code;
zCHAR szLocal2Text[ EXPRTEXT_MAXSTRING ];
zLONG lLocal2Code;
zLONG lLocal2Text;
sQExprStruct sExprStruct = { 0 };

zOPER_EXPORT zSHORT OPERATION
GetExpressionStruct( sQExprStruct *psExprStruct,
                     zVIEW  vSourceView,
                     zPCHAR pchEntity )
{
   zSHORT nRC;
   fQExprStruct fExprStruct = { 0 };

   fExprStruct.lStructSize = sizeof( fQExprStruct );
   nRC = GetStructFromEntityAttrs( (zPCHAR) &fExprStruct, vSourceView, pchEntity );
   memcpy( psExprStruct, &fExprStruct.lElementClass, sizeof( sQExprStruct ) );
   return( nRC );
}

//
// OPERATION: PreBuildOperation
//
// DESCRIPTION: PrebuildOperation
//    is called to build the Operation meta entity in the xpg before we actually get to the point of sucessfully parsing an operation.  This is
//    done to allow some data to be stored before the full operation data is known. Primarily, the text associated with the actual vml source code.
//
//
zOPER_EXPORT zLONG OPERATION
PreBuildOperation( zLONG lOperationType,
                   zLONG lLineNumber )
{
   zLONG lRC = 0;

   // If the parse limit is greater than sig then go back. operation meta entities are only created when the signatures are being parsed.
   if ( g_lParseLimit > zPARSE_LIMIT_SIGNATURES )
      return( 0 );

   //lRC = CreateEntity( g_lpPIView, "OperationSource", zPOS_AFTER );
   //
   //// set the LINENUMBER attribute
   //lRC = SetAttributeFromInteger( g_lpPIView, "OperationSource", "LineNumber", nLineNumber );
   //
   //// position the global lookup view.

   // Allocate memory for szLocal2Code work area
   //selMemory = SysAllocMemory ( (zCOREMEM) &szLocal2Code, 10000, 0, zCOREMEM_ALLOC, 0 );

   return( lRC );  // return all's well
}

// OPERATION: AddOperationEntityToPI
//
// DESCRIPTION: AddOperationEntityToPI adds an operation to the parsed object as it is being declared.  It first determines if the Operation
//    has already been defined.  If so it generates an error message.  If not then it creates it.  AddOperationEntityToPI also nulls out the
//    necessary globals that deal specifically with an operation. It is passed three parms: the name of the operation, the type of operation
//    it is to be, and the line number on which it was defined.  Other information is amended to it later.
//
//    AddOperationEntityToPI uses two global views.  The PI (parsed instance) view which it will modify, and the GS (global static) view which
//    it will use for searches.
//
zOPER_EXPORT zLONG OPERATION
AddOperationEntityToPI( zVIEW  vSubtask,
                        zPCHAR pchOperationName,   // the name of the operation
                        zSHORT nShow )             // show in Operation List
{
   zCHAR  szCharType[ 2 ];
   zLONG  lLth;
   zLONG  lClass;
   zSHORT nGenerated = 0;
   zLONG  lRC;

   // Try to position on the operation entity by name.  If we can, then we have an error condition of redefining the operation.
   lRC = SetCursorFirstEntityByString( g_lpSGView, "Operation", "Name", pchOperationName, "" );
   if ( lRC >= zCURSOR_SET )
   {
      if ( g_lParseLimit == zPARSE_LIMIT_SIGNATURES )
      {
         SyntaxError( vSubtask, eQOPERATIONALREADYDEFINED, pchOperationName ); // additional error info
         strcpy_s( g_szFatalErrorMsg, zsizeof( g_szFatalErrorMsg ), "Found operations with duplicate name: " );
         strcat_s( g_szFatalErrorMsg, zsizeof( g_szFatalErrorMsg ), pchOperationName );
         longjmp( jbFatalError, -1 ); // fatal error exit
      }
      else
         return( 0 );
   }

   // Determine if we have a generated Operation, with a name like fnLocal...
   lLth = zstrlen( pchOperationName );
   if ( nShow == 0 && lLth > 7 && memcmp( pchOperationName, "fnLocal", 7 ) == 0 && isdigit( pchOperationName[ lLth - 1 ] ) )
   {
      nGenerated = 1;
   }

   // Search for the operation by name in the source meta.
   lRC = SetCursorFirstEntityByString( g_vSourceMeta, "Operation", "Name", pchOperationName, "" );
   if ( nGenerated && lRC >= zCURSOR_SET )
   {
      // A generated operation already exists in the source meta, just delete and re-add it later.
      DeleteEntity( g_vSourceMeta, "Operation", zREPOS_NONE );
      lRC = zCURSOR_SET - 1; // indicate "not there"
   }

   if ( lRC >= zCURSOR_SET )
   {
      // Operation is already there.
      zCHAR szType[ 2 ];

      lClass = 0;
      GetIntegerFromAttribute( &lClass, g_vSourceMeta, "Operation", "Class" );

      // Set Operation.Class Attribute according to nShow parameter ...
      //  Bit 1 = On will prevent the Operation from being shown in the Operation List.
      lClass = lClass | 0x00000001; // set "generated" bit on
      if ( nShow )
         lClass = lClass ^ 0x00000001;  // switch it off

      SetAttributeFromInteger( g_vSourceMeta, "Operation", "Class", lClass );

      // Unfortunately old PWD's sometimes have no return data type for dialog operations, so if we have a dialog operation, correct the problem
      // by forcing the return type to be short (N).
      GetStringFromAttribute( szType, zsizeof( szType ), g_vSourceMeta, "Operation", "Type" );
      if ( szType[ 0 ] == 'D' )
      {
         // It is a dialog operation ... the return data type MUST be short (N).
         SetAttributeFromString( g_vSourceMeta, "Operation", "ReturnDataType", "N" );
      }

      IncludeSubobjectFromSubobject( g_lpPIView, "Operation", g_vSourceMeta, "Operation", zPOS_AFTER );
      SetCursorFirstEntityByAttr( g_lpPIView, "OperationSource", "ZKey", g_lpPIView, "Operation", "ZKey", "" );
      SetAttributeFromInteger( g_lpPIView, "OperationSource", "LineNumber", g_lRelativeLineNumber );
   }
   else
   {
      if ( nGenerated )
      {
         // It is a "generated operation" (e.g. fnLocalBuildQual_12). Do NOT create a meta entity (using CreateMetaEntity) since we want to
         // assign a special ZKey.

         zLONG lZKey;

         CreateEntity( g_vSourceMeta, "Operation", zPOS_LAST );
         // The ZKey  assigned is a combination of a
         // - part generated from the name (5 digits ) plus
         // - the number in the function name
         // The first part (for the function name) is for future use when we could have something different from "BuildQual".
         lZKey = fnZKeyForfnLocal( pchOperationName );
         SetAttributeFromInteger( g_vSourceMeta, "Operation", "ZKey", lZKey );
      }
      else
      {
         // It is a user defined operation.
         CreateMetaEntity( vSubtask, g_vSourceMeta, "Operation", zPOS_AFTER );
      }

      SetAttributeFromString( g_vSourceMeta, "Operation", "Name", pchOperationName );
      GetZeidonOperTypeFromVMLType( szCharType, g_lOperationType );
      SetAttributeFromString( g_vSourceMeta, "Operation", "Type", szCharType );
      SetAttributeFromString( g_vSourceMeta, "Operation", "ReturnDataType", "N" );

      // Set Operation.Class Attribute according to nShow parameter
      //  Bit 1 = On prevents the Operation from being shown in the Operation List.
      lClass = 0x00000001; // set "generated" bit on
      if ( nShow )
         lClass = lClass ^ 0x00000001;  // switch it off

      SetAttributeFromInteger( g_vSourceMeta, "Operation", "Class", lClass );
      IncludeSubobjectFromSubobject( g_lpPIView, "Operation", g_vSourceMeta, "Operation", zPOS_AFTER );
      SetCursorFirstEntityByAttr( g_lpPIView, "OperationSource", "ZKey",
                                  g_lpPIView, "Operation", "ZKey", "" );
      g_lAddToSrc = 1;
      SetNameForView( g_vSourceMeta, "vSourceMeta", lpGlobalSubtask, zLEVEL_TASK );
   }

   g_lSaveText = 1;  // checked in SaveTextLine
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
ResetOperation( zPCHAR pchOperationName )
{
   zLONG lRC;

   // If an operation entity does exist, try to position on it.  If we can, then this is an error situation of redefining the operation.
   lRC = SetCursorFirstEntityByString( g_lpPIView, "Operation", "Name", pchOperationName, "" );
   if ( lRC != zCURSOR_SET )
      return( lRC );

   lRC = SetCursorFirstEntityByString( g_lpSGView, "Operation", "Name", pchOperationName, "" );
   if ( lRC != zCURSOR_SET )
      return( lRC );

   SetCursorFirstEntityByAttr( g_lpPIView, "OperationSource", "ZKey", g_lpSGView, "Operation", "ZKey", "" );

   // Create a statement view to which to add statements.
   if ( g_lOperationFlag == 3 )
   {
      CreateViewFromViewForTask( &g_vStatementView, g_lpPIView, 0 );

      // Name the view so it will apprear in the browser (for debugging purposes).
      SetNameForView( g_vStatementView, "daSTATEMENT", lpGlobalSubtask, zLEVEL_SUBTASK );
   }

   return( 0 );
}

// OPERATION: AddVariableEntityToPI
//
// DESCRIPTION: AddVariableEntityToPI adds a variable that a user is attempting to add
//    to the operation for which it is being defined. Many things need to
//    go on here. First, it is necessary to determine whether or not this
//    name is already being used to declare a variable. If not then it
//    can continue. Secondly, we need to establish a variable id for this
//    variable. This id can be later used to reference the variable in the
//    program object to speed up parsing etc. Currently, this id will be
//    the sequential number that this variable was defined as. For example
//    the first variable in the declaration list will have id 1. If a
//    parameter list exists for the operation then the parameters will be
//    the first variables with the declared one after. If this variable is
//    a parameter, then the ParameterFlag attribute will be set later (since
//    at this point we do not know if it is). The ID is computed on each
//    entry into this operation. If no variables exist then the id will
//    obviously be 1, otherwise the id will be the id of the last variable
//    plus one (big deal huh?). Anyway, this operation takes in four
//    parameters: the name of the variable, the type that it is to be
//    declared as, the number of elements it contains (if it is a string)
//    and the line number on which it was defined. The line number is not
//    extremely important now except that Temp generated variables will be
//    set to a line number of -1.
//
//    There are two different types of TYPE we are talking about here. There
//    is the data type which will be one of: text, integer, decimal; and a
//    vml type which will be one of: string, integer, decimal, entityname,
//    attributename, view, pointer. The data type is what will be most dealt
//    with but we keep the vml type as well.
//
zOPER_EXPORT zLONG OPERATION
AddVariableEntityToPI( zVIEW  vSubtask,
                       zPCHAR pchVariableName,   // the name of the variable
                       zLONG  lVMLType,          // the vml type that was declared as
                       zLONG  lNumberOfElements, // number of chars if a string
                       zLONG  lLineNumber )      // line number on which it was defined
{
   zLONG lID;                   // temp to hold the id of the var to declare
   zLONG lRC;

   // First we want to see if any variable exists in this instance.  First, however, we need to position the static global view to this operation
   // that is being built in case it has been moved.
   SetViewFromView( g_lpSGView,        // point the static view to the
                    g_lpPIView );      // same place as the instance view

   // Check to see if this Variable Name was already declared for this operation.
   lRC = SetCursorFirstEntityByString( g_lpSGView, "Variable", "Name", pchVariableName, "" );

   // If the Variable Name has already been declared ... error.
   if ( lRC == zCURSOR_SET )
   {
      SyntaxError( vSubtask, eQVARIABLEALREADYDEFINED, pchVariableName );
      return( -1 );
   }
   else
   if ( lRC == zCURSOR_UNCHANGED )  // the Variable Name is not there, but others exist
   {
      // Position on the last "Variable" entity, get and increment its ID to be set as the ID for the next "Variable" entity created.
      SetCursorLastEntity( g_lpPIView, "Variable", "" );
      GetIntegerFromAttribute( &lID, g_lpPIView, "Variable", "ID" );
      lID++;
   }
   else
   if ( lRC == zCURSOR_NULL )  // no variable exists so set the ID to 1
   {
      lID = 1;
   }

   // We are all set to add the variable at this point.  We can assume that no positioning is necessary since queries are made to the
   // GS (global static) view and updates are made to the PI (parsed instance) view.  Therefore, we can just add the entity at the current position.
   CreateEntity( g_lpPIView, "Variable", zPOS_AFTER );

   // Set the ID attribute.
   SetAttributeFromInteger( g_lpPIView, "Variable", "ID", lID );

   // Set the Name attribute.
   SetAttributeFromString( g_lpPIView, "Variable", "Name", pchVariableName );
   SetAttributeFromInteger( g_lpPIView, "Variable", "Modify", 1 );  // ??? what is this used for ... dks 2009.09.09

   // Set the VMLType attribute.  If VMLType is a date, set it as a string.  Otherwise set the VMLType value as specified.
   if ( lVMLType == qTDATE )
      SetAttributeFromInteger( g_lpPIView, "Variable", "VMLType", qTSTRING );
   else
      SetAttributeFromInteger( g_lpPIView, "Variable", "VMLType", lVMLType );

// if ( zstrstr( pchVariableName, "ReturnedLevel" ) != 0 )
//    TraceLineS( "", "" );

   // Set the DataType attribute depending on the VML type.
   switch ( lVMLType )
   {
      case qTINTEGER:  // 1040
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTINTEGER );

         // Set the NElements attribute.
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", 0 );
         break;

      case qTSHORT:  // 1041
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTSHORT );

         // Set the NElements attribute (don't know why 2 here ... dks 2009.12.21).
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", 2 );
         break;

      case qTDECIMAL:  // 1039
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTDECIMAL );

         // Set the NElements attribute.
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", 0 );
         break;

      case qTSTRING:  // 1042
      case qTDATE:    // 1019
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTSTRING );

         // Set the NElements attribute (length + 1).
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", lNumberOfElements + 1 );
         break;

      case qTCHARACTER:  // 1072
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTCHARACTER );

         // Set the NElements attribute (me thinks ... as a string, one for the character plus one for a null terminator).
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", 2 );
         break;

      case qTVIEW:  // 1045
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTVIEW );

         // Set the NElements attribute.
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", 0 );
         break;

      case qTENTITYNAME:  // 1018
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTSTRING );

         // Set the NElements attribute.
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", g_lEntityNameLength + 1 );
         break;

      case qTATTRIBUTENAME:  // 1004
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTSTRING );

         // Set the NElements attribute.
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", g_lAttributeNameLength + 1 );
         break;

      default:
         SetAttributeFromInteger( g_lpPIView, "Variable", "DataType", qTINTEGER );

         // Set the NElements attribute.
         SetAttributeFromInteger( g_lpPIView, "Variable", "NElements", 0 );
         break;
   }

   // Set the LineNumber and ParameterFlag attributes.
   SetAttributeFromInteger( g_lpPIView, "Variable", "LineNumber", lLineNumber );
   SetAttributeFromInteger( g_lpPIView, "Variable", "ParameterFlag", 0 );  // local variable
// DisplayObjectInstance( g_lpPIView, "", "" );
// MessageSend( g_lpPIView, "???", "AddVariableEntityToPI", "g_lpPIView", 0, 0 );
   return( lID );
}

//
// OPERATION: AddStatementEntityToPI
//
// DESCRIPTION: AddStatementEntityToPI defines a statement to the parsed instance (pi). It is
//    passed a statement class, the line number on which it was defined, a position and a
//    view to the expression that is associated with it. AddStatementEntityToPI is not too tough
//    (I think). It takes the information passed to it and sets the appropriate attributes in
//    the statement entity. It then sets the pi view to a subobject if necessary, so that the
//    dependent statements will be added as sub-statements. Dependent statements are statements
//    that are only executed depending on the result of another statement.
//       For example:
//               if  k > 9  <-- control statement
//                   j = 5  <-- dependent statement
//
//    This means that the dependent statements are only seen if the control statement is true.
//
//    Statements have a statement number that is the consecutive order in which it has been
//    placed in the object. These numbers are used for debugging and other things. This is a
//    number PER VML statement. That is, if it takes 3 statements to perform one vml statement,
//    then each atomic statement has the same statement number. We assume that the expression
//    is complete at this point and can be optimized and put in the expression.
//
//    Statements are placed in the object depending on what class they are. If the statement
//    is an Expression or a return statement, we simply insert it as is. If the statement is a
//    flow control type of statement then we set subobject views etc. The flow of control
//    statements are: FOR, LOOP, IF, OF, CASE, WHILE, ELSE, UNTIL and END.
//    If any of LOOP FOR IF WHILE or CASE are put in, we put in the statement then set a view to
//    a subobject. If it is an ELSE, we reset from the IF statements, put in the ELSE, then set
//    back down. If it is an OF or UNTIL, we reset then put it in. If it is an END we just put
//    it in then reset.
//
//    Note that the view used is that of g_vStatementView which is a view of the parsed instance
//    but is set to the correct subobject level as needed.
//
zOPER_EXPORT zLONG OPERATION
AddStatementEntityToPI( zVIEW  vSubtask,
                        zLONG  lStatementClass,  // class of the statement
                        zLONG  lLineNumber,      // line number defined on
                        zVIEW  vSourceView,      // view to the expression
                        zLONG  lPosition )       // insert before or after the last
{
   zUSHORT uTemp;
   zLONG   lLinesDefined;

   // First we need to add the statement to the operation.  We do this depending on what the class of the operation is.
   if ( lStatementClass == qELSE || lStatementClass == qUNTIL )
   {
      // Reset the View up one level.
      ResetViewFromSubobject( g_vStatementView );
   }

   // jeff2
   if ( lStatementClass == qEND )
      SetCursorLastEntity( g_vStatementView, "Statement", "" );

   if ( lStatementClass == qOTHERWISE || lStatementClass == qOF )
   {
        // If this is an of or otherwise, we need to add a $qBREAK to the previous one if it exists.
//      if ( CheckExistenceOfEntity( g_vStatementView, "Statement" ) == zCURSOR_SET )
//      {
//         // add the break
//         CreateEntity( g_vStatementView, "Statement", zPOS_AFTER );
//
//         // set the line number on which this statement was defined.
//         SetAttributeFromInteger( g_vStatementView, "Statement", "LineNumber", lLineNumber );
//
//         if ( sQToken.nLastLineNo < sQToken.lLineNo )
//            nLinesDefined = sQToken.nLastLineNo;
//         else
//            nLinesDefined = sQToken.lLineNo;
//
//         SetAttributeFromInteger( g_vStatementView, "Statement", "RelativeLineNumber",
//                                  nLinesDefined - g_lRelativeLineNumber );
//                       // 8/23?   qqlineno - g_lRelativeLineNumber );
//
//         // set the current statement number
//         SetAttributeFromInteger( g_vStatementView, "Statement", "Number", lNumber );
//         // set the statement Class
//         SetAttributeFromInteger( g_vStatementView, "Statement", "Class", qBREAK );
//         nSetOFToSub = 1;
//         // reset back to the "OF" level since we must be on a subordinate sub_statement entity
//         ResetViewFromSubobject( g_vStatementView );
//         // add a break;
//      }
//      else
//         nSetOFToSub = 0;
   }

   // If g_lAddNextStatementBefore == 1, create the statement before the one that is there regardless of the lPosition parm.
   if ( g_lAddNextStatementBefore > 0 )
   {
      // Create the statement entity
      CreateEntity( g_vStatementView, "Statement", zPOS_BEFORE );
      g_lAddNextStatementBefore--;
   }
   else
   {
      // Create the statement entity
      CreateEntity( g_vStatementView, "Statement", (zSHORT) lPosition );
   }

   // Set the line number on which this statement was defined.
   SetAttributeFromInteger( g_vStatementView, "Statement", "LineNumber", lLineNumber );

   if ( sQToken.lLastLineNo < sQToken.lLineNo )
      lLinesDefined = sQToken.lLastLineNo;
   else
      lLinesDefined = sQToken.lLineNo;

   SetAttributeFromInteger( g_vStatementView, "Statement", "RelativeLineNumber",
                            lLinesDefined - g_lRelativeLineNumber );
                         // 8/23 qqlineno - g_lRelativeLineNumber );

   // Set the current statement number.
   SetAttributeFromInteger( g_vStatementView, "Statement", "Number", g_lNumber );

   // Set the statement Class.
   SetAttributeFromInteger( g_vStatementView, "Statement", "Class", lStatementClass );
   SetCursorFirstEntity( vSourceView, "ExprNode", "" );

   MoveViewToStringExpr( vSubtask, vSourceView, szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text );
// DisplayObjectInstance( vSourceView, "Expression", 0 );
   DeleteEntity( vSourceView, "Expression", zREPOS_NONE );
// DisplayObjectInstance( vSourceView, 0, 0 );
// IssueError( vSourceView, 0, 0, "AddStatementEntityToPI" );

   CreateEntity( vSourceView, "Expression", zPOS_AFTER );

   // Set the expression as a blob.
   uTemp = (zUSHORT) lLocal2Code;
   SetAttributeFromBlob( g_vStatementView, "Statement", "Expression", szLocal2Code, uTemp );
#ifdef DEBUG_PARSEGEN
// Not sure this is set to anything
   TraceStatementExpression( "AddStatementEntityToPI", szLocal2Code, uTemp );
#endif

   // Set the expression text as a blob.
   uTemp = (zUSHORT) lLocal2Text;
   SetAttributeFromBlob( g_vStatementView, "Statement", "ExprText", szLocal2Text, uTemp );

   switch ( lStatementClass )
   {
      case qEXPRESSION:
      case qRETURN:
      case qRETURNEND:
      case qUNTIL:
         break;

      case qBREAK:
      case qEND:
      case qENDFUNC:
         ResetViewFromSubobject( g_vStatementView );
         break;

      case qFOR:
      case qIF:
      case qCASE:
      case qLOOP:
      case qWHILE:
      case qELSE:
         SetViewToSubobject( g_vStatementView, "Substatement" );
         break;

      case qOTHERWISE:
      case qOF:
         //if ( nSetOFToSub == 1 )
         //{
              SetViewToSubobject( g_vStatementView, "Substatement" );
         //}
         break;

      default:
         break;
   }

// DisplayObjectInstance( g_vStatementView, "", "" );
// MessageSend( g_vStatementView, "???", "AddStatementEntityToPI", "StatementView", 0, 0 );
   return( 0 );
}

//
// OPERATION: AddStatementEntityWithStringToPI
//
// DESCRIPTION: AddStatementEntityWithStringToPI is the same as AddStatementEntityToPI
//    except that the expression is stored in a string and not in a view.
//
zOPER_EXPORT zLONG OPERATION
AddStatementEntityWithStringToPI( zVIEW  vSubtask,
                                  zLONG  lStatementClass,
                                  zLONG  lLineNumber,
                                  zPCHAR pchLocalCode,
                                  zPLONG plLocalCode,
                                  zPCHAR pchLocalText,
                                  zPLONG plLocalText,
                                  zLONG  lPosition )
{
   zLONG lLinesDefined;

   // First we need to add the statement to the operation.  We do this depending on what the class of the operation is.
   if ( lStatementClass == qELSE || lStatementClass == qUNTIL )
   {
      // Reset the View from lower level back up one
      ResetViewFromSubobject( g_vStatementView );
   }

   if ( lStatementClass == qEND )
      SetCursorLastEntity( g_vStatementView, "Statement", "" );

   if ( lStatementClass == qOTHERWISE || lStatementClass == qOF )
   {
////      if ( CheckExistenceOfEntity( g_vStatementView, "Statement" ) == zCURSOR_SET )
//      {
//         // reset the View from lower level back up one
//         // add a break
//         CreateEntity( g_vStatementView, "Statement", zPOS_AFTER );
//
//         // set the line number on which this statement was defined.
//         SetAttributeFromInteger( g_vStatementView, "Statement", "LineNumber", lLineNumber );
//         if ( sQToken.lLastLineNo < sQToken.lLineNo )
//            lLinesDefined = sQToken.lLastLineNo;
//         else
//            lLinesDefined = sQToken.lLineNo;
//
//         SetAttributeFromInteger( g_vStatementView, "Statement", "RelativeLineNumber",
///// 8/23                                  qqlineno - g_lRelativeLineNumber );
//                                     lLinesDefined - g_lRelativeLineNumber );
//
//         // set the current statement number
//         SetAttributeFromInteger( g_vStatementView, "Statement", "Number", lNumber );
//         // set the statement Class
//         SetAttributeFromInteger( g_vStatementView, "Statement", "Class", qBREAK );
//         nSetOFToSub = 1;
//         ResetViewFromSubobject( g_vStatementView );
//         // add a break;
//      }
//      else
//         nSetOFToSub = 0;
   }

   // If g_lAddNextStatementBefore equals 1, create the statement before the one that is there regardless of the szPosition parm.
   if ( g_lAddNextStatementBefore > 0 )
   {
      // Create the statement entity.
      CreateEntity( g_vStatementView, "Statement", zPOS_BEFORE );
      g_lAddNextStatementBefore--;
   }
   else
   {
      // Create the statement entity.
      CreateEntity( g_vStatementView, "Statement", (zSHORT) lPosition );
   }

   // Set the line number on which this statement was defined.
   SetAttributeFromInteger( g_vStatementView, "Statement", "LineNumber", lLineNumber );

   if ( sQToken.lLastLineNo < sQToken.lLineNo )
      lLinesDefined = sQToken.lLastLineNo;
   else
      lLinesDefined = sQToken.lLineNo;

   SetAttributeFromInteger( g_vStatementView, "Statement", "RelativeLineNumber", lLinesDefined - g_lRelativeLineNumber );
// 8/23                     qqlineno - g_lRelativeLineNumber );

   // Set the current statement number.
   SetAttributeFromInteger( g_vStatementView, "Statement", "Number", g_lNumber );

   // Set the statement Class.
   SetAttributeFromInteger( g_vStatementView, "Statement", "Class", lStatementClass );
   SetAttributeFromBlob( g_vStatementView, "Statement", "Expression", pchLocalCode, *plLocalCode );
#ifdef DEBUG_PARSEGEN
   // pchLocalCode has never been set as far as I can tell ... dks 2015.12.08
   TraceStatementExpression( "AddStatementEntityWithStringToPI", pchLocalCode, *plLocalCode );
#endif

   // Set the expression text as a blob.
   SetAttributeFromBlob( g_vStatementView, "Statement", "ExprText", pchLocalText, *plLocalText );

   *plLocalCode = 0;
   *plLocalText = 0;

   switch ( lStatementClass )
   {
      case qEXPRESSION:
      case qRETURN:
      case qRETURNEND:
      case qUNTIL:
         break;

      case qBREAK:
      case qEND:
      case qENDFUNC:
         ResetViewFromSubobject( g_vStatementView );
         break;

      case qFOR:
      case qIF:
      case qCASE:
      case qLOOP:
      case qWHILE:
      case qELSE:
         SetViewToSubobject( g_vStatementView, "Substatement" );
         break;

      case qOTHERWISE:
      case qOF:
//         if ( nSetOFToSub == 1 )
//         {
              SetViewToSubobject( g_vStatementView, "Substatement" );
//         }
         break;

      default:
         break;
   }

// DisplayObjectInstance( g_vStatementView, "", "" );
// MessageSend( g_vStatementView, "???", "AddStatementEntityWithStringToPI", "StatementView", 0, 0 );
   return( 0 );
}

//
// OPERATION: AddExprNodeEntityToView
//
// DESCRIPTION: This is the new redesigned add expression entry. This is
//    bigger/better, leaner meaner. than the old. First, the expression that
//    is being built is sent into the operation. There is no more "global"
//    operation that is being built. Secondly all the information gathered
//    about the expression entry is also sent. Expressions can be comprised
//    of 1: variables 2: operation calls 3: attribute qualifiers 4: constants
//    5: operators or 6: defines
//
//    The expression that is being built is a view of expresssion entries
//    of the 'structure' of this struct is as follows:
//
//       typedef struct sExprRec
//       {
//          //this is the element class of the expression entry. this
//          // could be one of qVARIABLE qCONSTANT qOPERATION qOPERATOR
//          // qATTRIBUTEQUALIFIER
//          zLONG lElementClass;
//
//          // This is the data class for VML one of TEXT, NUMERIC, etc
//          zLONG lDataClass;
//
//          // This is the data type of the expr entry one of qINTEGER qDECIMAL
//          // or qSTRING qATTRIBUTENAME qENTITYNAME etc
//          zLONG lDataType;
//
//          // lOffset can hold either the offset of the text in the expr text
//          // string or some other encoded value.
//          zLONG lOffset;
//       } sExprRec;
//
//
//    Variables: If a variable is being sent in to be put into the expression,
//    then its validity as a variable should have been verified, therefore,
//    this function does nothing to assure that the variable is  correct,
//    it assumes it. Secondly, the name of the variable is, in fact, never
//    sent in. Since it was necessary to confirm the existence of the
//    variable, we should have its id, so that is what gets sent in. The
//    struct gets set as follows :
//
//          lElementClass = qVARIABLE
//          lDataClass    = qTEXT or qNUMERIC depending
//          lDataType     = qINTEGER qDECIMAL qSTRING etc depending on type
//          nOffset       = the Variable's ID. There is no text stored in the
//                          expr text string for a variable entry.
//
//    Operation calls: if an operation call is in an expression, then the
//    struct is encoded differently. The ElementClass is set to qOPERATIONCALL.
//    the data class is set to some value depending on where the operation
//    actually exists. For example. if the operation exists in the same
//    source member as the caller, the the Dataclass is set to
//    qLOCALOPERATION. If the operation is an Object operation, then it is
//    set to qOBJECTOPERATION. If it is a global reusable operation that is
//    published in the applications operation list, then it is a
//    qGLOBALOPERATION and if ( finally ) it is in the Zeidon Global Operation
//    List then it is set to qZEIDONOPERATION. lDataType is always set to
//    qINTEGER as that is what it always returns. nOffset contains the offset
//    of the name in the expr text string or the ZKey depending
//    on what type of operation is being called.
//                      for an operation call
//          lElementClass = qOPERATIONCALL
//          lDataClass    = qLOCALOPERATION or qOBJECTOPERATION or
//                          qGLOBALOPERATION or qZEIDONOPERATION
//          lDataType     = qINTEGER
//          nOffset       = ZKey or offset into text
//
//    Attribute qualifiers: if an attribute qualifier is in an expression
//    then we (me) assume that its existence type et. al. has already
//    been verified. This  new way of building expressions is to keep the
//    info about the aq around and put off building interm statements until
//    it is necessary. That is, get the AQ, verify that it is hunky dory,
//    get the data type of it, and store it as a unit until we optimize the
//    statement. During the optimizing portion we will build the interim
//    statements necessary to get the aq's value. An AQ is
//    VIEW.ENTITY.ATTRIBUTE where VIEW is always a variable, and entity and
//    attribute can be either a variable of type EQ or AQ ( respectively ) or
//    constants. the DataClass will determine if the E or A are const or
//    variable. The Datatype will be either the datatype of the attribute
//    or, unknown at this point. Unknown is true only if the E or A are
//    variables .
//                      for an attribute qualifier
//          lElementClass = qATTRIBUTEQUALIFIER
//          lDataClass    = qENTITYVARIABLE or qATTRIBUTEVARIABLE or
//                          qBOTHVARIABLE
//          lDataType     = qINTEGER, etc or qUNKNOWN depending
//          nOffset       = Ord number or offset into text
//
//    constants: are the simplist to deal with. No longer are integers and
//    decimals converted and toyed with, rather they are simply kept in their
//    text format and manipulated as such till probably run time.
//
//                           for an constant
//          lElementClass = qCONSTANT
//          lDataClass    = qTEXT or NUMERIC
//          lDataType     = qINTEGER, etc or qUNKNOWN depending
//          nOffset       = offset into text
//
//    defines: are pretty much added as is.
//
//    operators are dealt with in AddOperator
//
//    This is called "WithView" because we are adding an expression node
//    to an expression view. This is done so that we can optimize it later
//    using core calls.
//
zOPER_EXPORT zLONG OPERATION
AddExprNodeEntityToView( zVIEW  vSubtask,
                         zVIEW  vTargetView,
                         zLONG  lEntryClass,
                         zLONG  lEntryDataClass,
                         zLONG  lEntryDataType,
                         zPCHAR pchEntryValue,
                         zLONG  lEntry,
                         zLONG  lFlags )
{
   // If the expression entry is an operator, go to add operator.
   if ( lEntryClass == qOPERATOR )
   {
      // AddOperator
      AddOperatorToView( vSubtask, vTargetView, lEntry ); // the operator
      SetNameForView( vTargetView, "***TargetView", vSubtask, zLEVEL_TASK );
   // IssueError( vSubtask, 0, 0, "AddExprNodeEntityToView" );
   // DisplayObjectInstance( vTargetView, "", "" );
      return( 0 );
   }

   // Always add to the end of the expression.
   SetCursorLastEntity( vTargetView, "ExprNode", "" );

   // Create the next Expression entry.
   CreateEntity( vTargetView, "ExprNode", zPOS_AFTER );

   // Entry type
   SetAttributeFromInteger( vTargetView, "ExprNode", "ElementClass", lEntryClass );

   // Set the priority of this operand.
   SetAttributeFromInteger( vTargetView, "ExprNode", "Level", g_lPriorityFudgeFactor );

// if ( g_szGenLang[ 0 ] == 'J' && lEntry > 0 &&
//      (lEntry == 2681057 || // ZeidonStringCopy
//       lEntry == 156416) )  // ZeidonStringConcat
// {
//    lEntry = -lEntry;
// }

   SetAttributeFromInteger( vTargetView, "ExprNode", "Offset", lEntry );

   // Set the data type.
   SetAttributeFromInteger( vTargetView, "ExprNode", "DataType", lEntryDataType );

   // Set flag bits    *+*
   SetAttributeFromInteger( vTargetView, "ExprNode", "Flags", lFlags );

   // Perform differently depending on the token.
   switch ( lEntryClass )
   {
      // Trying to add a variable.
      case qVARIABLE:
         // The variable id was added ... get the data type if not done yet.
         //if ( lEntryDataType == 0 )
         //   lEntryDataType = GetVariableDataType( lEntry );

         // Get the class as necessary.
         if ( lEntryDataClass == 0 )
            lEntryDataClass = GetClassBasedOnType( lEntryDataType );

         // Set the class.
         SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lEntryDataClass );
         break;  // done with variable

      case qDEFINE:
         // Set the data class
         SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lEntryDataClass );
         break;  // done with define

      case qOPERATIONCALL:
         SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lEntryDataClass );
         break;

      case qATTRIBUTEQUALIFIER:
         SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lEntryDataClass );
         SetAttributeFromString( vTargetView,"ExprNode", "Text", pchEntryValue );
         break;

      case qENTITYQUALIFIER:
         SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lEntryDataClass );
         SetAttributeFromString( vTargetView, "ExprNode", "Text", pchEntryValue );
         break;

      case qCONSTANT:  // qTINTEGER 1040   qTVIEW 1045
         lEntryDataClass = GetClassBasedOnType( lEntryDataType );
         SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lEntryDataClass );

         // The following is a temporary fix to handle German umlaut characters until the
         // Parser is fixed.  The umlaut characters were each converted to two characters
         // of the form "|x", where x was the non-umlauted version of the character. Here,
         // we will remove the | and convert the character back to its umlauted version.
         // At the same time, we will eliminate the " or ' characters.
         if ( (lEntryDataType == qTSTRING || lEntryDataType == qTCHARACTER || lEntryDataType == -qTSTRING || lEntryDataType == -qTCHARACTER) )
         {
            zCHAR  szConvertedValue[ 254 ];

            // back converting the german umlaut hack
            GermanUmlautHack( pchEntryValue, szConvertedValue, zsizeof( szConvertedValue ), TRUE );
            SetAttributeFromString( vTargetView, "ExprNode", "Text", szConvertedValue );
         }
         else
         {
            // store the constant as is
            SetAttributeFromString( vTargetView, "ExprNode", "Text", pchEntryValue );
         }

         break;

      default:
         break;
   }

// if ( 0 )
// {
//    SetNameForView( vTargetView, "***TargetView", vSubtask, zLEVEL_TASK );
// // DisplayObjectInstance( vTargetView, "", "" );
//    IssueError( vTargetView, 0, 0, "AddExprNodeEntityToView2" );
// }

   return( 0 );
}

//
// OPERATION: AddExpressionEntryToString
//
// DESCRIPTION: This is like AddExprNodeEntityToView. The differences are
//    that (surprise!) we add the entry to a string as a struct. This is
//    called primarily by build functions that know exactly in what order
//    things need to be added. Therefore we do not need the power of core
//    calls to optimize these statements. Therefore we skip the overhead
//    of core and put it straight in a string. The other difference is that
//    the use of this function as opposed to the one above is that we are
//    putting the expression together in optimized order so operators are
//    added here as well. Expressions built here can NOT be optimized any
//    further by other calls.
//
//    The Pointers to the strings we are building as well as the index
//    pointers are passed in.
//
// For example: if lEntryClass == qCONSTANT && lEntryDataClass == qTEXT ==> quoted string
// (either quotes or apostrophes based on whether lEntryDataType == qTCHARACTER)
//
zOPER_EXPORT zLONG OPERATION
AddExpressionEntryToString( zPCHAR pchExprCode,      // the expression code
                            zPLONG plExprCodeIndex,  // the index of expr code
                            zPCHAR pchExprText,      // the expression text string
                            zPLONG plExprTextIndex,  // the expr text index
                            zLONG  lEntryClass,
                            zLONG  lEntryDataClass,
                            zLONG  lEntryDataType,
                            zPCHAR pchEntryValue,
                            zLONG  lEntry,
                            zLONG  lFlags )
{
   sQExprStruct sCurrExpr = { 0 };

   sCurrExpr.lElementClass = lEntryClass;
   sCurrExpr.lOffset = lEntry;
   sCurrExpr.lDataType = lEntryDataType;
   sCurrExpr.lFlags = lFlags;
   switch ( lEntryClass )
   {
      case qOPERATOR:
         sCurrExpr.lDataClass = qNUMERIC;
         sCurrExpr.lLevel = 0;
         break;

      // trying to add a variable
      case qVARIABLE:
         if ( lEntryDataType == 0 )
            sCurrExpr.lDataType = GetVariableDataType( lEntry );
         else
            sCurrExpr.lDataType = lEntryDataType;

         sCurrExpr.lDataClass = GetClassBasedOnType( lEntryDataType );
         sCurrExpr.lLevel = g_lPriorityFudgeFactor;
         break;

      case qDEFINE:
         sCurrExpr.lDataClass = lEntryDataClass;
         sCurrExpr.lLevel = g_lPriorityFudgeFactor;
         break;

      case qOPERATIONCALL:
         sCurrExpr.lDataClass = lEntryDataClass;
         //if ( lEntry == 0 )
         //{
         //   sCurrExpr.lOffset = *plExprTextIndex;
         //   // store the operation name value
         //   strcpy_s( (zPCHAR)&pchExprText[ *plExprTextIndex ], pchEntryValue );
         //   *plExprTextIndex += zstrlen( pchEntryValue ) + 1;
         //}
         sCurrExpr.lLevel = g_lPriorityFudgeFactor;
         break;

      case qATTRIBUTEQUALIFIER:
         sCurrExpr.lDataClass = lEntryDataClass;
         sCurrExpr.lOffset = *plExprTextIndex;

         // store the attribute qualifier as is
         strcpy_s( (zPCHAR) pchExprText + *plExprTextIndex, MAXSTRING - *plExprTextIndex, pchEntryValue );
         *plExprTextIndex += zstrlen( pchEntryValue ) + 1;
         sCurrExpr.lLevel = g_lPriorityFudgeFactor;
         break;

      case qCONSTANT:
         sCurrExpr.lDataClass = GetClassBasedOnType( lEntryDataType );

         sCurrExpr.lOffset = *plExprTextIndex;

         if ( (lEntryDataType == qTSTRING || lEntryDataType == qTCHARACTER || lEntryDataType == -qTSTRING || lEntryDataType == -qTCHARACTER) && (pchEntryValue[ 0 ] == 0) )
         {
            // copy one zero byte for an empty string
            pchExprText[ *plExprTextIndex ] = 0;
            *plExprTextIndex += 1;
         }
         else
         {
            // store the constant as is
            strcpy_s( (zPCHAR) pchExprText + *plExprTextIndex, MAXSTRING - *plExprTextIndex, pchEntryValue );
            *plExprTextIndex += zstrlen( pchEntryValue ) + 1;
         }

         sCurrExpr.lLevel = g_lPriorityFudgeFactor;
         break;

      default:
         break;
   }

#ifdef DEBUG_PARSEGEN
   sCurrExpr.lCount = ++g_lCount;
   TraceExpressionStruct( &sCurrExpr, *plExprCodeIndex, "AddX: ", pchEntryValue && *pchEntryValue ? pchEntryValue : pchExprText );  // display this expression structure
#endif

   // sQExprStruct without the "zCHAR szText[ MAXSTRING ];" element.
   zmemcpy( (zPCHAR) pchExprCode + *plExprCodeIndex, (zPCHAR) &sCurrExpr, sizeof( sQExprStruct ) - MAXSTRING );
   *plExprCodeIndex = *plExprCodeIndex + sizeof( sQExprStruct ) - MAXSTRING;

   return( 0 );
}

//
// OPERATION: AddOperatorToView
//
// DESCRIPTION: This function adds an operator either to a stack or to the view of
//    the expression being built.  It prioritizes the order of the operators as they
//    come into the expression, the result of which is a postfix expression.  If
//    a -1 or lower number is sent in then the stack is "dumped".  To dump the stack
//    means to continue popping the stack until the bottom is reached.  The bottom
//    may be when the stack pointer is zero or when the bottom indicator is reached.
//    A bottom indicator is just a negative entry that means we want to build a new
//    stack on top of the one currently being used.  Kind of like pushing the whole
//    stack.  Note that there is no "String" version of this function as string
//    expressions are built in postfix order automatically.
//
zOPER_EXPORT zLONG OPERATION
AddOperatorToView( zVIEW  vSubtask,
                   zVIEW  vTargetView,
                   zLONG  lOperator )
{
#ifdef DEBUG_PARSEGEN
   zCHAR  szOperator[ 32 ];
#endif
   zLONG  lInPriority;
   zLONG  lTopPriority;

   // If lOperator is -1, then empty (dump) the stack into the expression.
   if ( lOperator == -1 )
   {
#ifdef DEBUG_PARSEGEN
      fnFindDefine( szOperator, zsizeof( szOperator ), g_nOperStack[ g_nStackPtr ].lOffset );
      TraceLine( "Start Dumping stack into the expression =========================================="
                 "================================================================================= %d  Operator: %s",
                 g_nStackPtr, szOperator );
#endif
      while ( g_nStackPtr > 0 && g_nOperStack[ g_nStackPtr ].lOffset > -1 )  // not bottom indicator
      {
         // Always add to the end of the string.
         if ( vTargetView )
         {
            SetCursorLastEntity( vTargetView, "ExprNode", "" );

            // Insert the values.
            InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, g_nOperStack[ g_nStackPtr ].lOffset,
                                          g_nOperStack[ g_nStackPtr ].lLevel, "", 0, zPOS_AFTER );
         }

         // Zero out the stack entry.
         g_nOperStack[ g_nStackPtr ].lOffset = 0;
         g_nOperStack[ g_nStackPtr ].lLevel = 0;

         // Decrement the stack pointer.
         g_nStackPtr--;
#ifdef DEBUG_PARSEGEN
      TraceLine( "Continue Dumping stack into the expression ======================================="
                 "================================================================================= %d  Operator: %s",
                 g_nStackPtr, szOperator );
#endif
      }

      // Clear off the bottom indicator.
      if ( g_nStackPtr > 0 && g_nOperStack[ g_nStackPtr ].lOffset < 0 )
      {
         g_nOperStack[ g_nStackPtr ].lOffset = 0;
         g_nOperStack[ g_nStackPtr ].lLevel = 0;
         g_nStackPtr--;
      }

#ifdef DEBUG_PARSEGEN
      fnFindDefine( szOperator, zsizeof( szOperator ), g_nOperStack[ g_nStackPtr ].lOffset );
      TraceLine( "End Dumping stack into the expression ============================================"
                 "================================================================================= %d  Operator: %s",
                 g_nStackPtr, szOperator );
#endif
      return( 0 );
   }

   // Insert a new bottom of the stack.
   if ( lOperator < -1 )  // new bottom of stack
   {
      // Increment the stack pointer.
      g_nStackPtr++;

      // Put in a < 0 value.
      g_nOperStack[ g_nStackPtr ].lOffset = lOperator;
      g_nOperStack[ g_nStackPtr ].lLevel = 0;

#ifdef DEBUG_PARSEGEN
      fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
      TraceLine( "Insert new bottom of stack ======================================================="
                 "================================================================================= %d  Operator: %s",
                 g_nStackPtr, szOperator );
#endif
      return( 0 );
   }

   // Set values in expression entry.  If the operator is a left or open operator, then
   // increment the priority weighting factor by ten.  This is so that all of the operators
   // within this grouping will be of higher priority that those outside of it.  For
   // example: a "+" might have a priority of 4 but a "+" inside parens (6 + 7) would have
   // a priority of 14.  For (6 + 7 * (8 + 9)) the first "+" would have a priority of 14
   // and the second would have a priority of 24.
   if ( lOperator == qLPAREN || lOperator == qLBRACE || lOperator == qOPENOPER )
   {
      InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, lOperator, g_lPriorityFudgeFactor, "", 0, zPOS_AFTER );

      // Increment the factor (these were only for OPENOPER, try with LPAREN)
      if ( lOperator == qOPENOPER )
      {
         g_nStackPtr++;
#ifdef DEBUG_PARSEGEN
         fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
         TraceLine( "Incrementing stack 1 ============================================================="
                    "================================================================================= %d  Operator: %s",
                    g_nStackPtr, szOperator );
#endif
         g_nOperStack[ g_nStackPtr ].lOffset = lOperator;
         g_nOperStack[ g_nStackPtr ].lLevel = g_lPriorityFudgeFactor;
      }

      g_lPriorityFudgeFactor += 10;
      return( 0 );
   }

   // If we get a "right", then pop the stack until the prioritys are = or <.
   if ( lOperator == qRBRACE || lOperator == qCLOSEOPER || lOperator == qCOMMA || lOperator == qPERIOD || lOperator == qCOLON ||
        lOperator == qOR || lOperator == qAND )
   {
      // The priority of the top of the stack.
      lTopPriority = g_nOperStack[ g_nStackPtr ].lLevel;

      // While not at the bottom ...
      while ( g_nStackPtr > 0 && lTopPriority > g_lPriorityFudgeFactor )
      {
         InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, g_nOperStack[ g_nStackPtr ].lOffset,
                                       g_nOperStack[ g_nStackPtr ].lLevel, "", 0, zPOS_AFTER );

         // Zero out the stack entry.
         g_nOperStack[ g_nStackPtr ].lOffset = 0;
         g_nOperStack[ g_nStackPtr ].lLevel = 0;

         // Special processing for ANDs and ORs.
      // if ( lOperator == qOR && lOperator == qAND )  // this could never be true ... dks 2009.11.02
      //    IssueError( vTargetView, 0, 0, "AND processing" );

         g_nStackPtr--;
#ifdef DEBUG_PARSEGEN
         fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
         TraceLine( "Decrementing stack 1 ============================================================="
                    "================================================================================= %d  Operator: %s",
                    g_nStackPtr, szOperator );
#endif
         if ( g_nStackPtr > 0 )
            lTopPriority = g_nOperStack[ g_nStackPtr ].lLevel;
      }

      if ( lOperator == qRBRACE || lOperator == qRPAREN || lOperator == qCLOSEOPER )
      {
         if ( g_lPriorityFudgeFactor > 0 )
            g_lPriorityFudgeFactor -= 10;

         // Zero out the OPENOPER entry (was for close oper only).
         if ( lOperator == qCLOSEOPER )
         {
            g_nOperStack[ g_nStackPtr ].lOffset = 0;
            g_nOperStack[ g_nStackPtr ].lLevel = 0;
            g_nStackPtr--;
#ifdef DEBUG_PARSEGEN
            fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
            TraceLine( "Decrementing stack 2 ============================================================="
                       "================================================================================= %d  Operator: %s",
                       g_nStackPtr, szOperator );
#endif
         }
      }

      // Only insert the Operator if it's not AND or OR.
      if  ( lOperator != qOR && lOperator != qAND )
      {
         InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, lOperator, g_lPriorityFudgeFactor, "", 0, zPOS_AFTER );
         return( 0 );
      }
   }

   if ( lOperator == qRPAREN )
      g_lPriorityFudgeFactor -= 10;

   // Get the priority of the operator being sent in.
   lInPriority = Priority( lOperator ) + g_lPriorityFudgeFactor;

   // Get the priority of top of the stack.
   lTopPriority = g_nOperStack[ g_nStackPtr ].lLevel;

   // If the priority of the operator being sent in is greater than the priority of the operator on the top of the stack, then push
   // the "operator sent in" onto the stack.
   if ( lInPriority > lTopPriority )
   {
      // Increase the stack pointer first.
      g_nStackPtr++;
#ifdef DEBUG_PARSEGEN
      fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
      TraceLine( "Incrementing stack 2 ============================================================="
                 "================================================================================= %d  Operator: %s",
                 g_nStackPtr, szOperator );
#endif
      g_nOperStack[ g_nStackPtr ].lOffset = lOperator;
      g_nOperStack[ g_nStackPtr ].lLevel = lInPriority;
   }
   else
   {
      // If the priority of the operator being sent in is less than or equal to the priority or the top operator, pop the stack onto the expression
      // being built and push the incoming operator onto the stack.  While there are entries in the stack, and the priority of what's there
      // is greater than what's being sent in, put the stack out to the expr.
      while ( g_nStackPtr > 0 && lInPriority <= lTopPriority )
      {
         InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, g_nOperStack[ g_nStackPtr ].lOffset,
                                       g_nOperStack[ g_nStackPtr ].lLevel, "", 0, zPOS_AFTER );

         // Zero out the stack entry.
         g_nOperStack[ g_nStackPtr ].lOffset = 0;
         g_nOperStack[ g_nStackPtr ].lLevel = 0;

         g_nStackPtr--;
#ifdef DEBUG_PARSEGEN
         fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
         TraceLine( "Decrementing stack 3 ============================================================="
                    "================================================================================= %d  Operator: %s",
                    g_nStackPtr, szOperator );
#endif
         if ( g_nStackPtr > 0 )
            lTopPriority = g_nOperStack[ g_nStackPtr ].lLevel;
      }

      // Now push the incoming operator onto the stack.
      g_nStackPtr++;
#ifdef DEBUG_PARSEGEN
      fnFindDefine( szOperator, zsizeof( szOperator ), lOperator );
      TraceLine( "Incrementing stack 3 ============================================================="
                 "================================================================================= %d  Operator: %s",
                 g_nStackPtr, szOperator );
#endif
      g_nOperStack[ g_nStackPtr ].lOffset = lOperator;
      g_nOperStack[ g_nStackPtr ].lLevel = lInPriority;
   }

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
InsertExprNodeValuesToEntity( zVIEW  vTargetView,
                              zLONG  lElementClass,
                              zLONG  lDataClass,
                              zLONG  lDataType,
                              zLONG  lOffset,
                              zLONG  lLevel,
                              zPCHAR pchText,
                              zLONG  lFlags,
                              zLONG  lPos )
{
   CreateEntity( vTargetView, "ExprNode", (zSHORT) lPos );  // vTargetView Def: TZVSEXPO

   SetAttributeFromInteger( vTargetView, "ExprNode", "ElementClass", lElementClass );
   SetAttributeFromInteger( vTargetView, "ExprNode", "DataClass", lDataClass );
   SetAttributeFromInteger( vTargetView, "ExprNode", "DataType", lDataType );
   SetAttributeFromInteger( vTargetView, "ExprNode", "Offset", lOffset );
   SetAttributeFromInteger( vTargetView, "ExprNode", "Level", lLevel );
   SetAttributeFromString( vTargetView, "ExprNode", "Text", pchText );
   SetAttributeFromInteger( vTargetView, "ExprNode", "Flags", lFlags );
   SetNameForView( vTargetView, "***TargetView", vTargetView, zLEVEL_TASK );
// DisplayEntityInstance( vTargetView, "ExprNode" );
// IssueError( vTargetView, 0, 0, "InsertExprNodeValuesToEntity" );
   return( 0 );
}

//
// OPERATION: AnalyzeRightExpression
//
// DESCRIPTION: AnalyzeRightExpression is passed a postfixed right expression
//    and tries to get out of it what it can. It goes like this:
//    It is passed a postfix expression. It uses a stack that is an array of
//    integers with a top index. Also, it uses another array integer that
//    represents the order of the expression after it is analyzed. The object
//    is to maintain the postfixed order while using core operations where-
//    ever possible. After this function is done, all attribute qualifiers
//    should be out of the expression ans well as all operation calls that
//    were coded.
//
//    The order will be maintained by adding entries to the left and right
//    of the order array depending on the push popping of the stack.
//
//    DataStructures:
//       szExprCode - the postfixed expression as it is sent in.
//       lpExprCode - pointer to an integer that contains the index of the
//                    end of the expression
//       szExprText - the text associated with any enrties in szExprCode
//       lpExprText - pointer to an integer that contains the index of the
//                    end of the text area
//
//       nStack - a list, implimented as a stack, of indexes.
//       lTop - index of nStack pointing to the Top of the stack
//       nNewExprOrder - an array of integers that is being built. This will
//                       be the postfixed/analyzed expression
//       nRight - index into nNewExprOrder that represents where the next
//                right entry will be.
//       nLeft  - index into nNewExprOrder that represents where the next
//                Left entry will be.
//
//    Algorithm
//
//    nSTester = (zLONG) *((zPLONG) &szExprCode[ lLoopControl ]);
//
// define the macro PUSH to be as follows:
//    Put the index of expression into the value stack
//    bump the stack pointer up by one
//
#define PUSH(n) (nStack[ lTop++ ] = n)

// define the macro POP to be as follows:
//    Decrement the stackpointer
//    return the index into expression that was stored there.
#define POP       ((lTop > 0) ? nStack[ --lTop ] : -1)

#define PUT(a,b) (nNewExprOrder[ b ] = a)
#define UNPUT(a) (nNewExprOrder[ a ] = 0)

#define POP       ((lTop > 0) ? nStack[ --lTop ] : -1)

#define PUT(a,b) (nNewExprOrder[ b ] = a)


zOPER_EXPORT zLONG OPERATION
IsCondOperator( zLONG lOper )
{
   if ( lOper == qCONDEQUAL || lOper == qLESSTHANEQUAL || lOper == qLESSTHAN || lOper == qGREATERTHANEQUAL || lOper == qGREATERTHAN || lOper == qNOTEQUAL || lOper == qNOT )
   {
      return( 0 );
   }
   else
   {
      return( 1 );
   }
}

zOPER_EXPORT zLONG OPERATION
CreateConvertZeidonStrOp( zVIEW  vSubtask,
                          zVIEW  vSourceView,
                          zVIEW  vTargetView,
                          zCHAR  cNumberType,
                          zPCHAR pchStringLth )
{
   zCHAR szOperationName[ 33 ];
   zLONG lOffset;
   zLONG lElementClass;
   zLONG lOperationZKey;
   zLONG lRC;

   /////////////  ElementClass - qVARIABLE:1044  qOPERATOR:1034  qOPERATIONCALL:2008
   //  Section 1  DataClass - qNUMERIC:2009  qZEIDONOPERATION:4006  qTEXT:2010
   /////////////  DataType - qTINTEGER:1040  qTSHORT:1041  qTSTRING:1042  Offset - qEQUAL:3001  qOPENOPER:3018  qCOMMA:3013

   // The first part of this code is very close to CreateTargetZeidonStrOp, except we always use the convert operation and the max length parm is
   // set to the max string size, if there is no indexing on the target string.  This part creates the first part of the operation as follows:
   //    "ZeidonStringConvertFromNumber( szTargetString, lIndex, lMaxLth,"

   // The operation is always ZeidonStringConvertFromNumber.
   strcpy_s( szOperationName, zsizeof( szOperationName ), "ZeidonStringConvertFromNumber" );

   lOperationZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, szOperationName );
   IncludeCoreHeader( g_lpPIView, "ZDRVROPR" );

   // Set up the Operation name and the left paren.
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATIONCALL, qZEIDONOPERATION, qTSHORT,
                            szOperationName, lOperationZKey, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qOPENOPER, 0 );

   // Move the variable as the first parm in the operation, + a comma.
   MoveSubobject( vTargetView, "ExprNode", vSourceView, "ExprNode", zPOS_AFTER, zREPOS_AFTER );

   // This is a hack to ensure the first parameter is set to be a return parameter (for Java).
   GetIntegerFromAttribute( &lOffset, vTargetView, "ExprNode", "Offset" );
   if ( lOffset > 0 )
      SetAttributeFromInteger( vTargetView, "ExprNode", "Offset", -lOffset );

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qCOMMA, 0 );

// if ( 0 )
// {
//    SetNameForView( vTargetView, "***TargetView", vSubtask, zLEVEL_TASK );
//    IssueError( vTargetView, 0, 0, "CreateConvertZeidonStrOp" );
// }

   GetIntegerFromAttribute( &lElementClass, vSourceView, "ExprNode", "ElementClass" );
   GetIntegerFromAttribute( &lOffset, vSourceView, "ExprNode", "Offset" );

   // Handle special indexing, if there is any.
   // If we immediately get an lbrace, then the string was indexed
   if ( lElementClass == qOPERATOR && lOffset == qLBRACE )
   {
      DeleteEntity( vSourceView, "ExprNode", zREPOS_AFTER );
   // SetCursorNextEntity( vSourceView, "ExprNode", "" );

      GetIntegerFromAttribute( &lElementClass, vSourceView, "ExprNode", "ElementClass" );
      GetIntegerFromAttribute( &lOffset, vSourceView, "ExprNode", "Offset" );

      // Get the offset expression (i.e. just copy it from one expr to next)
      while ( !(lElementClass == qOPERATOR && (lOffset == qCOLON || lOffset == qRBRACE)) )
      {
         lRC = MoveSubobject( vTargetView, "ExprNode", vSourceView, "ExprNode", zPOS_AFTER, zREPOS_AFTER );
         if ( lRC <= zCURSOR_UNCHANGED )
            break;

         GetIntegerFromAttribute( &lElementClass, vSourceView, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSourceView, "ExprNode", "Offset" );
      }

      // Add COMMA
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 );

      // If we ended on an rbrace use the default, else we must have ended on a colon.
      if ( lElementClass == qOPERATOR && lOffset == qRBRACE )
      {
         // use default of 0
         AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
         DeleteEntity( vSourceView, "ExprNode", zREPOS_AFTER );
      }
      else
      {
         DeleteEntity( vSourceView, "ExprNode", zREPOS_AFTER );

         // Get the next entry
         GetIntegerFromAttribute( &lElementClass, vSourceView, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSourceView, "ExprNode", "Offset" );

         // Get the length expression (i.e. just copy it from one expr to next).
         while ( !(lElementClass == qOPERATOR && lOffset == qRBRACE) )
         {
            MoveSubobject( vTargetView, "ExprNode", vSourceView, "ExprNode", zPOS_AFTER, zREPOS_AFTER );
            if ( lRC <= zCURSOR_UNCHANGED )  // I can't believe we're not checking the RC from the MoveSubobject??? ... dks 2009.09.10
               break;

            GetIntegerFromAttribute( &lElementClass, vSourceView, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vSourceView, "ExprNode", "Offset" );
         }

         DeleteEntity( vSourceView, "ExprNode", zREPOS_AFTER );
      }

      // Add COMMA
      lRC = AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 );
   }
   else // use both defaults cause there is no indexing
   {
      // Add default offset of 1 and COMMA ... if no offset default to 1.
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 );

      // Add default MaxReceive of 0 and COMMA.
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 );
   }

   // Add Max string length and COMMA.
   AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC, qTINTEGER, pchStringLth, 0, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 );

   /////////////
   //  Section 2
   /////////////

   // This section is somewhat like CreateSourceZeidonStrOp, except that routine adds a string variable or constant, whereas the following
   // code adds the numeric variable or constant.  If the variable or constant is integer, it adds the integer value in the integer position,
   // adds a 0 in the decimal position and a NumberTypeFlag of 'I'.  If the variable or constant is decimal, it adds a 0 to the integer position,
   // the decimal variable or constant to the decimal position and a NumberTypeFlag of 'D'.
   // The result is that this section adds code to the previous section of the form:
   //     "lIntegerValue, dDecimalValue, cNumberType );"

   if ( cNumberType == 'I' )
   {
      // Add the Integer value and a COMMA.
      MoveSubobject( vTargetView, "ExprNode",  vSourceView, "ExprNode", zPOS_AFTER, zREPOS_AFTER );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qCOMMA, 0 );

      // Add a Decimal 0.0 and a COMMA.
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC, qTDECIMAL, "0.0", 0, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qCOMMA, 0 );

      // The NumberType is "I"
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT, qTSTRING, "I", 0, 0 );
   }
   else
   {
      // Add an Integer 0 and a COMMA.
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0,  qTINTEGER, "", qCOMMA, 0 );

      // Add the Decimal value and a COMMA.
      MoveSubobject( vTargetView, "ExprNode", vSourceView, "ExprNode", zPOS_AFTER, zREPOS_AFTER );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qCOMMA, 0 );

      // The NumberType is "D"
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT, qTSTRING, "D", 0, 0 );
   }

   // Finally add the close operation.
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qCLOSEOPER, 0 );

// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}

//
// OPERATION: AddEntityOperWithScope
//
// DESCRIPTION: AddEntityOperWithScope creates a call to an operation with the result of the operation being put in RESULT
//
// VML example:
// SET CURSOR FIRST KZXMLPGO.DynamicBannerName WITHIN KZXMLPGO.Session
//            WHERE KZXMLPGO.DynamicBannerName.DialogName = "abc"
//              AND KZXMLPGO.DynamicBannerName.BannerName = "def"
//
// C Generation:
// RESULT = SetCursorFirstEntity( KZXMLPGO, "DynamicBannerName", "Session" );
// if ( RESULT > zCURSOR_UNCHANGED )
// {
//    while ( RESULT > zCURSOR_UNCHANGED &&
//            (CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "DialogName", "abc" ) != 0 ||
//             CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "BannerName", "def" ) != 0) )
//    {
//       RESULT = SetCursorNextEntity( KZXMLPGO, "DynamicBannerName", "Session" );
//    }
// }
//
// Java Generation:
// RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setFirst( "DialogName", "abc", "Session" );
// if ( RESULT >= CursorResult.SET )
// {
//    while ( RESULT >= CursorResult.SET &&
//            (!KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "DialogName" ).equals( "abc" ) ||  // note--this line isn't necessary
//             !KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "BannerName" ).equals( "def" )) )
//    {
//       RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setNextContinue( );
//    }
// }
//
zOPER_EXPORT zLONG OPERATION
AddEntityOperWithScope( zVIEW  vSubtask,
                        zPCHAR pchOperationName,
                        zLONG  lViewID,
                        zPCHAR pchEntityName,
                        zLONG  lEntityID,
                        zLONG  lUseAttribute,
                        zPCHAR pchAttributeName,
                        zLONG  lAttributeID,
                        zPCHAR pchScopeEntityName,
                        zLONG  lScopeEntityID,
                        zLONG  lUseExtra,
                        zPCHAR pchExtra,
                        zLONG  lExtra )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lResultID;
   zLONG lOperZKey;
// zLONG k;

// zmemset( sParmList, 0, zsizeof( sParmList ) );
// zmemset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal2Code = 0;
   lLocal2Text = 0;

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // Add RESULT.
   lResultID = AddResult( vSubtask );
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RESULT", lResultID, 0 );

   // KJS 08/30/12 - I am currently commenting out the Java code because DG has made a change
   // to the return code of the "set" cursor methods and they now return an object instead of an
   // int. Ultimately, I think we want to come back with this with a different return code but for now
   // this is what we will do...
   // Should be something like the following:
   // CursorResult cr = vComboBox3.cursor( "CollegeYear" ).setFirst(  );
   // while ( cr.isSet() )

  //if ( g_szGenLang[ 0 ] == 'J' )
   if ( g_szGenLang[ 0 ] == 'X' )
   {
      // sExpr.lElementClass == qCONSTANT && sExpr.lDataClass == qTEXT ==> quoted string
      // (either quotes or apostrophes based on whether sExpr.lDataType == qTCHARACTER)
      // When should sParmList[ lParmListIndex++ ] be called ... apparently only when we are building a parm list? ... no!

      //:SET CURSOR FIRST mUTrace.UserTraceRecord WHERE mUTrace.UserTraceRecord.UserName = mUser.User.UserName AND mUTrace.UserTraceRecord.Type = "W"
      // RESULT = mUTrace.cursor( "UserTraceRecord" ).setFirst( "UserName", "", "" );
      // while ( RESULT > zCURSOR_UNCHANGED && mUTrace.cursor( "UserTraceRecord" ).attribute( "Type" ).equals( "W" ) == false )
      //    RESULT = mUTrace.cursor( "UserTraceRecord" ).setNextContinue( );

#if 0
===========================
         //:SET CURSOR FIRST mUTrace.UserTraceRecord WHERE mUTrace.UserTraceRecord.UserName = mUser.User.UserName AND mUTrace.UserTraceRecord.Type = "W"
         RESULT = mUTrace.cursor( "UserTraceRecord" ).setFirst( );
         while ( RESULT > zCURSOR_UNCHANGED &&
           mUTrace.cursor( "UserTraceRecord" ).attribute( "Type" ).equals( "W" ) == false )
===========================
         //://Need to first make sure that the student doesn't already exist.
         //:SET CURSOR FIRST mFaculty.ListAssignableStudent WHERE mFaculty.ListAssignableStudent.ID = lStudDpt.Student.ID
         lTempInteger_13 = GetIntegerFromAttribute( lTempInteger_13, lStudDpt, "Student", "ID" );
         RESULT = mFaculty.cursor( "ListAssignableStudent" ).setFirst( "ID", lTempInteger_13 );
===========================
#endif

      // Add EQUAL ... add early to keep it from terminating generation of the operation early.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

      // View.cursor
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "cursor", 0, 0 );

      // Open paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      // Parameter = Entity
      if ( lEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Entity", lEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

      // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, pchEntityName, 0, 0 );
      }

      // Close paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
   //xADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], pchOperationName, 0, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, pchOperationName, 0, 0 );

      // Open paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

#if 0 // don't need this with Java???

      for ( k = 1; k < g_lWhereIndex; k++ ) // where condition
      {
         zCHAR szViewName[ 33 ];
         zCHAR szEntityName[ 33 ];
         zCHAR szAttributeName[ 33 ];
         zLONG lID;

         SplitQualifier( g_sWhereLeft[ k ].szText, szViewName, szEntityName, szAttributeName );
         GetVariableID( &lID, szAttributeName );
         if ( lID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Attrib", lID, 0 );
         }
         else  // is a string constant
         {
            //lParmListIndex++;
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

         //xADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], g_szAttributeName, lID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, szAttributeName, 0, 0 );
         }

         ADDCOMMAEXPRESSIONTOSTRING;

      // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], g_sWhereRight[ 1 ].szText, -1, 0 );

         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lID *= -1;
         }
         else  // is a string constant
         {
            //lParmListIndex++;
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

         //xADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], g_szAttributeName, g_lAttributeID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, g_szAttributeName, 0, 0 );
         }

         ADDCOMMAEXPRESSIONTOSTRING;

         if ( lScopeEntityID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lScopeEntityID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "ScopeEntity", lScopeEntityID, 0 );
         }
         else  // is a string constant
         {
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

         // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchScopeEntityName, lScopeEntityID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, pchScopeEntityName, 0, 0 );
         }
      }
#else
      if ( lUseAttribute > 0 && lUseExtra > 0 ) // use the extra parameter
      {
         // pchExtra might contain special zCHAR combinations created by the german umlaut hack.
         zCHAR szCopy[ 256 ];

         if ( lAttributeID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lAttributeID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Attrib", lAttributeID, 0 );
         }
         else  // is a string constant
         {
            //lParmListIndex++;
         // if ( sParmList[ lParmListIndex ] < 0 )
         //    sParmList[ lParmListIndex ] *= -1;

         // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchAttributeName, lAttributeID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, pchAttributeName, 0, 0 );
         }

         ADDCOMMAEXPRESSIONTOSTRING;
         GermanUmlautHack( pchExtra, szCopy, zsizeof( szCopy ), TRUE );

         //if ( sParmList[ lParmListIndex ] < 0 )
         //   lUseExtra *= -1;

      // ADDEXPRESSIONENTRYTOSTRING( lUseExtra, 0, sParmList[ lParmListIndex++ ], szCopy, lExtra, 0 );
         ADDEXPRESSIONENTRYTOSTRING( lUseExtra, qTEXT, qTSTRING, szCopy, lExtra, 0 );
      }

      if ( lScopeEntityID > 0 ) // is a var
      {
         ADDCOMMAEXPRESSIONTOSTRING;
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lScopeEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "ScopeEntity", lScopeEntityID, 0 );
      }
      else  // is a string constant
      if ( pchScopeEntityName && pchScopeEntityName[ 0 ] )
      {
         ADDCOMMAEXPRESSIONTOSTRING;
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

      // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchScopeEntityName, lScopeEntityID, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, pchScopeEntityName, 0, 0 );
      }

#endif

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );
   }
   else  // C generation
   {
      // Add operation name.
      lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
      ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperZKey, 0 );

      GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
      lParmListIndex = 1;

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      if ( sParmList[ lParmListIndex ] < 0 )
         lViewID *= -1;

      // 1. parameter = VIEW
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      // 2. parameter = Entity
      if ( lEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Entity", lEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
      }

      ADDCOMMAEXPRESSIONTOSTRING;

      if ( lUseAttribute > 0 )
      {
         if ( lAttributeID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lAttributeID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Attrib", lAttributeID, 0 );
         }
         else  // is a string constant
         {
            //lParmListIndex++;
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchAttributeName, lAttributeID, 0 );
         }

         ADDCOMMAEXPRESSIONTOSTRING;
      }

      if ( lUseExtra > 0 ) // use the extra parameter
      {
         // pchExtra might contain special zCHAR combinations created by the german umlaut hack.
         zCHAR szCopy[ 256 ];
         GermanUmlautHack( pchExtra, szCopy, zsizeof( szCopy ), TRUE );

         //if ( sParmList[ lParmListIndex ] < 0 )
         //   lUseExtra *= -1;

         ADDEXPRESSIONENTRYTOSTRING( lUseExtra, 0, sParmList[ lParmListIndex++ ], szCopy, lExtra, 0 );
         ADDCOMMAEXPRESSIONTOSTRING;
      }

      if ( lScopeEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lScopeEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "ScopeEntity", lScopeEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

      // lParmListIndex++;
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchScopeEntityName, lScopeEntityID, 0 );
      }

      // Close paren.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

      // Add EQUAL.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );
   }

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );
   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, zPOS_AFTER );
// IssueError( vSubtask, 0, 0, "End of: AddEntityOperWithScope" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}

// VML example:
// SET CURSOR FIRST KZXMLPGO.DynamicBannerName WITHIN KZXMLPGO.Session
//            WHERE KZXMLPGO.DynamicBannerName.DialogName = "abc"
//              AND KZXMLPGO.DynamicBannerName.BannerName = "def"
//
// SET CURSOR FIRST KZXMLPGO.DynamicBannerName
//
// Java Generation:
//       //:SET CURSOR FIRST mUTrace.UserTraceRecord WHERE mUTrace.UserTraceRecord.UserName = mUser.User.UserName AND mUTrace.UserTraceRecord.Type = "W"
//       RESULT = mUTrace.cursor( "UserTraceRecord" ).setFirst( );
//       while ( RESULT > zCURSOR_UNCHANGED &&
//              ( mUTrace.cursor( "UserTraceRecord" ).attribute( "UserName" ).equals( mUser.cursor( "User" ).attribute( "UserName" ) ) == false ||
//               mUTrace.cursor( "UserTraceRecord" ).attribute( "Type" ).equals( "W" ) == false ) )
//          RESULT = mUTrace.cursor( "UserTraceRecord" ).setNextContinue( );
//
zOPER_EXPORT zLONG OPERATION
AddJavaWhileWithScope( zVIEW vSubtask, zVIEW vTargetView )
{
   zCHAR szCursorVEA[ 128 ];
   zCHAR szViewName[ 33 ];
   zLONG lViewID;
   zCHAR szEntityName[ 33 ];
   zLONG lEntityID;
   zCHAR szAttributeName[ 33 ];
   zLONG lAttributeID;
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lResultID;
   zLONG lDefineZKey;
   zLONG lVarDataType;
   zLONG lLoop;

// zmemset( sParmList, 0, zsizeof( sParmList ) );
// zmemset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal2Code = 0;
   lLocal2Text = 0;

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // Add RESULT.
   lResultID = AddResult( vSubtask );

   // Add the while statement to do all of these compares...
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "while ", 0, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );  // 1 open
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RESULT", lResultID, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ">", qGREATERTHAN, 0 );

   lDefineZKey = GetDefineZKey( vSubtask, &lVarDataType, "zCURSOR_UNCHANGED" );
   ADDEXPRESSIONENTRYTOSTRING( qDEFINE, qNUMERIC, sParmList[ lParmListIndex++ ], "CSR", lDefineZKey, 0 );

   if ( g_lWhereIndex > 1 ) // where condition
   {
      // Skip first compare since it is taken care of by earlier SetCursor in java??? sometimes ... can we figure it out???
      for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
      {
         zLONG k;

         if ( lLoop == 1 )
            ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "&&", qAND, 0 );
         else
            ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "||", qOR, 0 );

         if ( lLoop == 1 && g_lWhereIndex > 1 )
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "\n                (", 0, 0 );  // 2 open
         else
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "\n                ", 0, 0 );

         SplitQualifier( g_sWhereLeft[ lLoop ].szText, szViewName, szEntityName, szAttributeName );

         // Insert opening parens, if necessary.
         for ( k = 0; k < g_sWhereLeft[ lLoop ].lLevel; k++ )
         {
            ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qLPAREN, 0 );  // no open added
         }

         // while ( RESULT > CursorResult.UNCHANGED && ( KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "BannerName" ).equals( "def" ) == false ) )

         // View.cursor
         GetVariableID( &lViewID, szViewName );
#if 1
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qLPAREN, 0 );  // 3 open
         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTINTEGER, "", lViewID, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "cursor", 0, 0 );

         // Open paren
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );  // 4 open

         // Parameter = Entity
         GetVariableID( &lEntityID, szEntityName );
         if ( lEntityID > 0 ) // is a var
         {
            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, qTSTRING, "", lEntityID, 0 );
         }
         else  // is a string constant
         {
         // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, szEntityName, lEntityID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, szEntityName, 0, 0 );
         }
#else
         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "cursor", 0, 0 );

         // Open paren
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

         // Parameter = Entity
         GetVariableID( &lEntityID, szEntityName );
         if ( lEntityID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lEntityID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Entity", lEntityID, 0 );
         }
         else  // is a string constant
         {
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

         // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, pchEntityName, 0, 0 );
         }
#endif

         // Close paren
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );  // close 4 open

         // Period
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );

         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "getStringFromAttribute", 0, 0 );

         // Open paren
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );  // 5 open

         GetVariableID( &lAttributeID, szAttributeName );
#if 1
         if ( lAttributeID > 0 ) // is a var
         {
            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, qTSTRING, "", lAttributeID, 0 );
         }
         else  // is a string constant
         {
             ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, szAttributeName, 0, 0 );
         }
#else
         if ( lAttributeID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lAttributeID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Attrib", lAttributeID, 0 );
         }
         else  // is a string constant
         {
            //lParmListIndex++;
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

         //xADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], g_szAttributeName, lAttributeID, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, szAttributeName, 0, 0 );
         }
#endif
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );  // close 5 open
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "equals", 0, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );  // 6 open

         if ( g_sWhereRight[ lLoop ].lElementClass == qCONSTANT )
         {
            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, g_sWhereRight[ lLoop ].szText, 0, 0 );
         }
         else
         if ( g_sWhereRight[ lLoop ].lElementClass == qVARIABLE )
         {
         // AddExprNodeEntityToView( vSubtask, vTargetView, g_sWhereRight[ lLoop ].lElementClass, g_sWhereRight[ lLoop ].lDataClass,
         //                          g_sWhereRight[ lLoop ].lDataType, g_sWhereRight[ lLoop ].szText, g_sWhereRight[ lLoop ].lOffset, 0 );
            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Value", g_sWhereRight[ lLoop ].lOffset, 0 );
         }
         else
         {
            k = SplitQualifier( g_sWhereRight[ lLoop ].szText, szViewName, szEntityName, szAttributeName );
            if ( k == 3 )  // full attribute qualification
            {
               // e.g. mUser.cursor( "User" ).attribute( "UserName" )
               strcpy_s( szCursorVEA, zsizeof( szCursorVEA ), szViewName );
               strcat_s( szCursorVEA, zsizeof( szCursorVEA ), ".cursor( \"" );
               strcat_s( szCursorVEA, zsizeof( szCursorVEA ), szEntityName );
               strcat_s( szCursorVEA, zsizeof( szCursorVEA ), "\" ).getStringFromAttribute( \"" );
               strcat_s( szCursorVEA, zsizeof( szCursorVEA ), szAttributeName );
               strcat_s( szCursorVEA, zsizeof( szCursorVEA ), "\" )" ); // close 6 open
               ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, szCursorVEA, 0, 0 );
            }
            else
            {
            // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], g_sWhereRight[ lLoop ].szText, -1, 0 );
               ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, g_sWhereRight[ lLoop ].szText, 0, 0 );
            }
         }

         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );  // close 3 open
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTINTEGER, "== false", 0, 0 );
         ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );  // close 2 open

         if ( lLoop >= 1 && lLoop == g_lWhereIndex - 1 )  // change > to >=  dks  2010.05.21
            ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

      }
   }

   // Close paren
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

   // New line with setNextContinue ... e.g. RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setNextContinue( );
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "\n           ", 0, 0 );

   // Add RESULT =.
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RESULT", lResultID, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // View.cursor
   SplitQualifier( g_sWhereLeft[ 1 ].szText, szViewName, szEntityName, szAttributeName );
   GetVariableID( &lViewID, szViewName );
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "cursor", 0, 0 );

   // Open paren
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

   // Parameter = Entity
   GetVariableID( &lEntityID, szEntityName );
   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
      {
         sParmList[ lParmListIndex ] *= -1;
         lEntityID *= -1;
      }

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Entity", lEntityID, 0 );
   }
   else  // is a string constant
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         sParmList[ lParmListIndex ] *= -1;

   // ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], szEntityName, lEntityID, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, szEntityName, 0, 0 );
   }

   // Close paren
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "setNextContinue", 0, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );
   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, zPOS_AFTER );
// IssueError( vSubtask, 0, 0, "End of: AddJavaWhileWithScope" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}

//
// OPERATION: AddSetEntityCursor
//
// DESCRIPTION: Add SetEntityCursor function
// Generate:
//  RESULT = SetEntityCursor(
//              zView               - Required. View to Object Instance.
//              pchEntityName       - Required. Entity Name.
//              pchAttributeName    - Opt. Dependent on lControl qualifiers
//                                    from 'set 1.1' (see below).
//              lControl            - Required, consists of one required
//                                    positioning control indicator, and
//                                    three optional qualifying control
//                                    indicators.
//              pQualValue          - Opt. Dependent on lControl qualifiers
//              szSrcEntityName     - Opt. Dependent on lControl qualifiers
//              szSrcAttributeName  - Opt. Dependent on lControl qualifiers
//              lRelPosition        - Opt. indicates relative position to
//                                    move to.  zPOS_RELATIVE must be
//                                    specified in lControl.
//              szScopingEntityName - scoping entity
//              szContextName       - Opt. Context name )
//
zOPER_EXPORT zLONG OPERATION
AddSetEntityCursor( zVIEW  vSubtask,
                    zLONG  lViewID,
                    zPCHAR pchEntityName,
                    zLONG  lEntityID,
                    zLONG  lControl ) // zFIRST or zNEXT or ..
{
   zCHAR szNumber[ 16 ];
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lResultID;
   zLONG lOperZKey;

// zmemset( sParmList, 0, zsizeof( sParmList ) );
// zmemset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal2Code = 0;
   lLocal2Text = 0;

// VML example:
// SET CURSOR FIRST KZXMLPGO.DynamicBannerName WITHIN KZXMLPGO.Session
//            WHERE KZXMLPGO.DynamicBannerName.DialogName = "abc"
//              AND KZXMLPGO.DynamicBannerName.BannerName = "def"

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // Add RESULT.
   lResultID = AddResult( vSubtask );
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RESULT", lResultID, 0 );

   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, "SetEntityCursor" );
   if ( g_szGenLang[ 0 ] == 'J' )
   {
   // Java Generation:
   // RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setFirst( "DialogName", "abc", "Session" );
   // if ( RESULT >= CursorResult.SET )
   // {
   //    while ( RESULT >= CursorResult.SET &&
   //            (!KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "DialogName" ).equals( "abc" ) ||  // note--this line isn't necessary
   //             !KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "BannerName" ).equals( "def" )) )
   //    {
   //       RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setNextContinue( );
   //    }
   // }

      // View.cursor
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "cursor", 0, 0 );

      // Open paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      // Parameter = Entity
      if ( lEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "Entity", lEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
      }

      // Close paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "setFirst", 0, 0 );

   }
   else
   {
      zLONG k;

   // C Generation:
   // RESULT = SetCursorFirstEntity( KZXMLPGO, "DynamicBannerName", "Session" );
   // if ( RESULT > zCURSOR_UNCHANGED )
   // {
   //    while ( RESULT > zCURSOR_UNCHANGED &&
   //            (CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "DialogName", "abc" ) != 0 ||
   //             CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "BannerName", "def" ) != 0) )
   //    {
   //       RESULT = SetCursorNextEntity( KZXMLPGO, "DynamicBannerName", "Session" );
   //    }
   // }

      // Add operation name.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, "SetEntityCursor", lOperZKey, 0 );

      GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
      lParmListIndex = 1;

      // Open paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      if ( sParmList[ lParmListIndex ] < 0 )
         lViewID *= -1;

      // 1. parameter = VIEW
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      // 2. parameter = Entity
      if ( lEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "EntityID", lEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
      }

      ADDCOMMAEXPRESSIONTOSTRING;

      // 3. parameter Attribute, always empty
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], "Attrib", 0, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      // 4. parameter lControl
      sprintf_s( szNumber, zsizeof( szNumber ), "%ld", lControl );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], szNumber, 0, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      // 5.parameter QualValue, always empty, type is PVOID
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, "QualValue", 0, 0 );
      lParmListIndex++;
      ADDCOMMAEXPRESSIONTOSTRING;

      // 6.parameter  szSrcEntityName always empty
      // 7.parameter  szSrcAttributeName always empty
      for ( k = 0; k < 2; k++ )
      {
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], "SrcEntityAttrib", 0, 0 );
         ADDCOMMAEXPRESSIONTOSTRING;
      }

      // 8.parameter  lRelPosition always 0
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "0", 0, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      // 9.parameter  szScopingEntityName always empty
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], "ScopeEntity", 0, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      // 10.parameter szContextName always empty
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], "Context", 0, 0 );

      // Close paren.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );
   }

   // Add EQUAL.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );
   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, zPOS_AFTER );
// IssueError( vSubtask, 0, 0, "End of: AddSetEntityCursor" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}

#if 0  // This does not seem to be called from anywhere and it has a problem with szScopeEntityName (which would be a global)  // dks 2009/10/22
//
// OPERATION: AddSetCursorObjectScope
//
// DESCRIPTION: Add SetEntityCursor with Object Scope
//
zOPER_EXPORT zLONG OPERATION
AddSetCursorObjectScope( zVIEW  vSubtask,
                         zLONG  lViewID,
                         zPCHAR pchEntityName,
                         zLONG  lEntityID,
                         zLONG  lUseAttribute,
                         zPCHAR pchAttributeName,
                         zLONG  lAttributeID,
                         zLONG  lUseExtra,
                         zPCHAR pchExtra,
                         zLONG  lExtra )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lResultID;
   zLONG lOperZKey;

// zmemset( sParmList, 0, zsizeof( sParmList ) );
// zmemset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal2Code = 0;
   lLocal2Text = 0;

// VML example:
// SET CURSOR FIRST KZXMLPGO.DynamicBannerName WITHIN KZXMLPGO.Session
//            WHERE KZXMLPGO.DynamicBannerName.DialogName = "abc"
//              AND KZXMLPGO.DynamicBannerName.BannerName = "def"

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // Add RESULT.
   lResultID = AddResult( vSubtask );
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RESULT", lResultID, 0 );

   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, "SetEntityCursor" );
   if ( g_szGenLang[ 0 ] == 'J' )
   {
   // Java Generation:
   // RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setFirst( "DialogName", "abc", "Session" );
   // if ( RESULT >= CursorResult.SET )
   // {
   //    while ( RESULT >= CursorResult.SET &&
   //            (!KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "DialogName" ).equals( "abc" ) ||  // note--this line isn't necessary
   //             !KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "BannerName" ).equals( "def" )) )
   //    {
   //       RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setNextContinue( );
   //    }
   // }

      // View.cursor.
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "cursor", 0, 0 );

      // Open paren.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      // Parameter = Entity
      if ( lEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "EntityID", lEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
      }

      // Close paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ".", qPERIOD, 0 );
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, sParmList[ lParmListIndex++ ], "setFirst", 0, 0 );

      // Open paren
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      if ( lUseAttribute > 0 )
      {
         if ( lAttributeID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lAttributeID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "AttribID", lAttributeID, 0 );
         }
         else  // is a string constant
         {
            //lParmListIndex++;
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchAttributeName, lAttributeID, 0 );
         }

         ADDCOMMAEXPRESSIONTOSTRING;
      }

      if ( lUseExtra > 0 ) // use the extra parameter
      {
         // pchExtra might contain special zCHAR combinations created by the german umlaut hack.
         zCHAR szCopy[ 256 ];
         GermanUmlautHack( pchExtra, szCopy, zsizeof( szCopy ), TRUE );

         //if ( sParmList[ lParmListIndex ] < 0 )
         //   lUseExtra *= -1;

         ADDEXPRESSIONENTRYTOSTRING( lUseExtra, 0, sParmList[ lParmListIndex++ ], szCopy, lExtra, 0 );
         ADDCOMMAEXPRESSIONTOSTRING;
      }
   }
   else
   {
   // C Generation:
   // RESULT = SetCursorFirstEntity( KZXMLPGO, "DynamicBannerName", "Session" );
   // if ( RESULT > zCURSOR_UNCHANGED )
   // {
   //    while ( RESULT > zCURSOR_UNCHANGED &&
   //            (CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "DialogName", "abc" ) != 0 ||
   //             CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "BannerName", "def" ) != 0) )
   //    {
   //       RESULT = SetCursorNextEntity( KZXMLPGO, "DynamicBannerName", "Session" );
   //    }
   // }

      // Add operation name
      ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperZKey, 0 );

      GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
      lParmListIndex = 1;

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

      if ( sParmList[ lParmListIndex ] < 0 )
         lViewID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
      ADDCOMMAEXPRESSIONTOSTRING;

      if ( lEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "EntityID", lEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
      }

      ADDCOMMAEXPRESSIONTOSTRING;
      if ( lUseAttribute > 0 )
      {
         if ( lAttributeID > 0 ) // is a var
         {
            if ( sParmList[ lParmListIndex ] < 0 )
            {
               sParmList[ lParmListIndex ] *= -1;
               lAttributeID *= -1;
            }

            ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "AttribID", lAttributeID, 0 );
         }
         else  // is a string constant
         {
            //lParmListIndex++;
            if ( sParmList[ lParmListIndex ] < 0 )
               sParmList[ lParmListIndex ] *= -1;

            ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchAttributeName, lAttributeID, 0 );
         }

         ADDCOMMAEXPRESSIONTOSTRING;
      }

      if ( lUseExtra > 0 ) // use the extra parameter
      {
         // pchExtra might contain special zCHAR combinations created by the german umlaut hack.
         zCHAR szCopy[ 256 ];
         GermanUmlautHack( pchExtra, szCopy, zsizeof( szCopy ), TRUE );

         //if ( sParmList[ lParmListIndex ] < 0 )
         //   lUseExtra *= -1;

         ADDEXPRESSIONENTRYTOSTRING( lUseExtra, 0, sParmList[ lParmListIndex++ ], szCopy, lExtra, 0 );
         ADDCOMMAEXPRESSIONTOSTRING;
      }

      if ( lScopeEntityID > 0 ) // is a var
      {
         if ( sParmList[ lParmListIndex ] < 0 )
         {
            sParmList[ lParmListIndex ] *= -1;
            lScopeEntityID *= -1;
         }

         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "ScopeEntity", lScopeEntityID, 0 );
      }
      else  // is a string constant
      {
         if ( sParmList[ lParmListIndex ] < 0 )
            sParmList[ lParmListIndex ] *= -1;

         //lParmListIndex++;
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], szScopeEntityName, lScopeEntityID, 0 );
      }

      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );
   }

   // Add EQUAL.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );
   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, zPOS_AFTER );
// IssueError( vSubtask, 0, 0, "End of: AddSetCursorObjectScope" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}
#endif

//
// OPERATION: AddEntityOperWithNoScope
//
// DESCRIPTION:
//
zOPER_EXPORT zLONG OPERATION
AddEntityOperWithNoScope( zVIEW  vSubtask,
                          zPCHAR pchOperationName,
                          zLONG  lViewID,
                          zPCHAR pchEntityName,
                          zLONG  lEntityID,
                          zLONG  lUseExtra,
                          zPCHAR pchExtra,
                          zLONG  lExtra )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lOperZKey;
   zLONG lDefineZKey;
   zLONG lVarReturnType;

// memset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal2Code = 0;
   lLocal2Text = 0;

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // Add result.
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "Result", lExtra, 0 );

   // Add operation name.
   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

   if ( sParmList[ lParmListIndex ] < 0 )
      lViewID *= -1;

   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
   ADDCOMMAEXPRESSIONTOSTRING;

   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
      {
         sParmList[ lParmListIndex ] *= -1;
         lEntityID *= -1;
      }

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "EntityID", lEntityID, 0 );
   }
   else  // is a string constant
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         sParmList[ lParmListIndex ] *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
   }

   if ( lUseExtra > 0 ) // use the extra parameter
   {
      ADDCOMMAEXPRESSIONTOSTRING;

      lDefineZKey = GetDefineZKey( vSubtask, &lVarReturnType, pchExtra );

      if ( lUseExtra == qTINTEGER ||
           lUseExtra == qTSHORT   ||
           lUseExtra == qTDECIMAL )
      {
         ADDEXPRESSIONENTRYTOSTRING( qDEFINE, qNUMERIC, sParmList[ lParmListIndex++ ], pchExtra, lDefineZKey, 0 );
      }
      else // if ( lUseExtra == qTSTRING ) // use the integer
      {
         ADDEXPRESSIONENTRYTOSTRING( qDEFINE, qTEXT, sParmList[ lParmListIndex++ ], pchExtra, lDefineZKey, 0 );
      }
   }

   // Close paren
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

   // Add EQUAL.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );
   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, zPOS_AFTER );
// IssueError( vSubtask, 0, 0, "End of: AddEntityOperWithNoScope" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
AddEntityOperWithEntity( zVIEW  vSubtask,
                         zPCHAR pchOperationName,
                         zLONG  lViewID,
                         zPCHAR pchEntityName,
                         zLONG  lEntityID,
                         zPCHAR pchScopeViewName,
                         zLONG  lScopeViewID,
                         zPCHAR pchScopeEntityName,
                         zLONG  lScopeEntityID,
                         zLONG  lUseExtra,
                         zPCHAR pchExtra,
                         zLONG  lExtra )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lResultID;
   zLONG lOperZKey;

// zmemset( sParmList, 0, zsizeof( sParmList ) );
// zmemset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal2Code = 0;
   lLocal2Text = 0;

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // Add RESULT.
   lResultID = AddResult( vSubtask );
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RESULT", lResultID, 0 );

   // Add operation name.
   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

   if ( sParmList[ lParmListIndex ] < 0 )
      lViewID *= -1;

   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ViewID", lViewID, 0 );
   ADDCOMMAEXPRESSIONTOSTRING;

   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lEntityID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "EntityID", lEntityID, 0 );
   }
   else  // is a string constant
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         sParmList[ lParmListIndex ] *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
   }

   ADDCOMMAEXPRESSIONTOSTRING;
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, sParmList[ lParmListIndex++ ], "ScopeView", lScopeViewID, 0 );
   ADDCOMMAEXPRESSIONTOSTRING;

   if ( lScopeEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lScopeEntityID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, sParmList[ lParmListIndex++ ], "ScopeEntity", lScopeEntityID, 0 );
   }
   else  // is a string constant
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         sParmList[ lParmListIndex ] *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, sParmList[ lParmListIndex++ ], pchScopeEntityName, lScopeEntityID, 0 );
   }

   if ( lUseExtra > 0 ) // use the extra parameter
   {
      ADDCOMMAEXPRESSIONTOSTRING;
      ADDEXPRESSIONENTRYTOSTRING( lUseExtra, 0, sParmList[ lParmListIndex++ ], pchExtra, lExtra, 0 );
   }

   // Close paren.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

   // Add EQUAL.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );
   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal2Code, &lLocal2Code, szLocal2Text, &lLocal2Text, zPOS_AFTER );
// IssueError( vSubtask, 0, 0, "End of: AddEntityOperWithEntity" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}

#if 0  // This does not seem to be called from anywhere and it would have a problem
       // at the end with two equals anyways. // dks 2009/10/22
zOPER_EXPORT zLONG OPERATION
AddEntityOperToViewWithNoScope( zVIEW  vSubtask,
                                zVIEW  vExprView,
                                zPCHAR pchOperationName,
                                zLONG  lViewID,
                                zPCHAR pchEntityName,
                                zLONG  lEntityID,
                                zLONG  lUseExtra,
                                zPCHAR pchExtra,
                                zLONG  lExtra,
                                zLONG  lOperator,
                                zLONG  lValue )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lOperZKey;

// memset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;

   // Add result.
   AddExprNodeEntityToView( vSubtask, vExprView, qVARIABLE, qNUMERIC, qTSHORT, "", lExtra, 0 );

   // Add operation name.
   AddExprNodeEntityToView( vSubtask, vExprView, qOPERATIONCALL, qZEIDONOPERATION, qTSHORT,
                            pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;

   AddExprNodeEntityToView( vSubtask, vExprView, qOPERATOR, qNUMERIC, qTINTEGER, "", qOPENOPER, 0 );

   if ( sParmList[ lParmListIndex ] < 0 )
      lViewID *= -1;

   AddExprNodeEntityToView( vSubtask, vExprView, qVARIABLE, qNUMERIC,
                            sParmList[ lParmListIndex ], "ViewID", lViewID,
                            sParmListExt[ lParmListIndex ] );
   lParmListIndex++;
   AddExprNodeEntityToView( vSubtask, vExprView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCOMMA, 0 );
   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lEntityID *= -1;

      AddExprNodeEntityToView( vSubtask, vExprView, qVARIABLE, qTEXT,
                               sParmList[ lParmListIndex ], "EntityID", lEntityID,
                               sParmListExt[ lParmListIndex ] );
      lParmListIndex++;
   }
   else  // is a string constant
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         sParmList[ lParmListIndex ] *= -1;

      AddExprNodeEntityToView( vSubtask, vExprView, qCONSTANT, qTEXT,
                               sParmList[ lParmListIndex++ ], pchEntityName, lEntityID, 0 );
   }

   // Close paren.
   AddExprNodeEntityToView( vSubtask, vExprView, qOPERATOR, qNUMERIC, qTINTEGER, "", qCLOSEOPER, 0 );

   // Add EQUAL.
   AddExprNodeEntityToView( vSubtask, vExprView, qOPERATOR, qNUMERIC, qTINTEGER, "", lOperator, 0 );

   // Add EQUAL.
   AddExprNodeEntityToView( vSubtask, vExprView, qCONSTANT, qNUMERIC, qTINTEGER, pchExtra, lValue, 0 );
// IssueError( vSubtask, 0, 0, "End of: AddEntityOperToViewWithNoScope" );
// DisplayObjectInstance( vTargetView, "", "" );
   return( 0 );
}
#endif

//
// This is Don's German Umlaut Hack
//  Prior to parse all german umlauts are converted into '|' + normal ascii .
//  Then, during parse, the string constants are converted back.  Note that
//  a pipe character must be doubled in VML to produce a single pipe.  A
//  single pipe (followed by a space) is translated to be a carriage return.
//
// pchIn = in-buffer
// pchOut = out-buffer
// lBuf = Length of in/out-buffer
// ucBack = 0 for  umlaut to |-combination
//        = 1 for  |-combination to umlaut
//
zOPER_EXPORT zVOID OPERATION
GermanUmlautHack( zPCHAR pchIn, zPCHAR pchOut, zLONG lBuf, zBOOL bReset )
{
   zPCHAR pcIn = pchIn;
   zPCHAR pcOut = pchOut;
   if ( strchr( pchIn, '|' ) )
      TraceLineS( "GermanUmlautHack PIPE found: ", pchIn );

   if ( bReset )
   {
      // back from |... to umlaut
      zPCHAR pMaxLine = pcOut + lBuf -1;

      // Get rid of any leading " or ' character.
      if ( *pcIn == '\"' || *pcIn == '\'' )
         pcIn++;

      while ( *pcIn && pcOut < pMaxLine )
      {
         if ( *pcIn == '|' )
         {
            // Process any |x character pair.
            pcIn++;
            switch ( *pcIn )
            {
               case 'u':  // ue
                  *(pcOut++) = '\xFC';
                  break;

               case 'U':  // UE
                  *(pcOut++) = '\xDC';
                  break;

               case 'o':  // oe
                  *(pcOut++) = '\xF6';
                  break;

               case 'O':  // OE
                  *(pcOut++) = '\xD6';
                  break;

               case 'a':  // ae
                  *(pcOut++) = '\xE4';
                  break;

               case 'A':  // AE
                  *(pcOut++) = '\xC4';
                  break;

               case 's':  // ss
                  *(pcOut++) = '\xDF';
                  break;

               case 'P':  // §
                  *(pcOut++) = '\xA7';
                  break;

               case ' ':  // blank
                  *(pcOut++) = '\x7C';  // retain the single pipe
                  *(pcOut++) = ' ';
                  break;

               case '|':  // |
                  *(pcOut++) = '\x7C';
                  *(pcOut++) = '\x7C';  // retain the double pipe
                  break;

               default:
                  *(pcOut++) = *pcIn;
                  break;
            }
         }
         else
            *(pcOut++) = *pcIn;

         pcIn++;
      }

      if ( *(pcIn - 1) == '\"' || *(pcIn - 1) == '\'' )
      {
         // Get rid of any trailing " or ' character.
         *(pcOut - 1) = 0;
      }
      else
         *(pcOut) = 0;
   }
   else
   {
      // convert umlaut to |...
      zPCHAR pMaxLine = pchIn + lBuf - 1;

      while ( *pcIn )
      {
         switch ( *pcIn )
         {
            case '\xFC': /* ue */
               *(pcOut++) = '|';
               *(pcOut++) = 'u';
               break;

            case '\xDC': /* UE */
               *(pcOut++) = '|';
               *(pcOut++) = 'U';
               break;

            case '\xF6': /* oe */
               *(pcOut++) = '|';
               *(pcOut++) = 'o';
               break;

            case '\xD6': /* OE */
               *(pcOut++) = '|';
               *(pcOut++) = 'O';
               break;

            case '\xE4': /* ae */
               *(pcOut++) = '|';
               *(pcOut++) = 'a';
               break;

            case '\xC4': /* AE */
               *(pcOut++) = '|';
               *(pcOut++) = 'A';
               break;

            case '\xDF': /* ss */
               *(pcOut++) = '|';
               *(pcOut++) = 's';
               break;

            case '\xA7': /* § */
               *(pcOut++) = '|';
               *(pcOut++) = 'P';
               break;
#if 0
            case '\x7C': /* | */
               *(pcOut++) = '|';
               *(pcOut++) = '|';  // double it for gen ... dks 2004.09.22
               break;
#endif
            default:
               if ( *(unsigned char *) pcIn >= 0x80 )
                  *(pcOut++) = ' ';
               else
                  *(pcOut++) = *pcIn;

               break;
         }

         pcIn++;
         if ( pcIn > pMaxLine )
            break;
      }

      *pcOut = 0;
   }
}

// >>>fnZKeyForfnLocal
//  generate ZKey for local function
//  the name is like fnLocalBuildQual_12
//  The ZKey is the unique number generated at the end of the name.
//
static zLONG
fnZKeyForfnLocal( zCPCHAR cpcOperationName )
{
   zLONG lZKey = 0;
   zLONG k, j;

   k = zstrlen( cpcOperationName ) -1;
   j = 1;
   while ( isdigit( cpcOperationName[ k ] ) )
   {
      lZKey = lZKey + j * ((unsigned) cpcOperationName[ k ] - (unsigned) '0');
      k--;
      j= j * 10;
   }

   // The C generator accepts operation ZKeys only, if they are > MAXSTRING (currently 1024).
   // So we add 10000 to get into that range.
   lZKey = 10000 + lZKey;
   return( lZKey );
}
