/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp7aa.c - VML SubSystem Where statement build
// DESCRIPTION:   This is the source file of the optimizing code
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/08/17
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
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"

/*
CHANGE LOG
2002.01.18  HH
   fix for R053855, allow any attribute as string parameter in a function.
2000.09.20  HH
   handle Set/Compare Attributes from/to DEFINE's.
2001.07.17  HH
   fixed bug "zstrlen + zstrlen".
2001.04.25  HH
   fixed bug for operations with no return value (HPF DEUKA).
2001.03.08  HH
   corrected bugs introduced by latest restructuring.
2001.02.25  HH
   improved handling parenthese expressions.
2001.02.22  HH
   restructered fnOptNumExpr.
   Corrected handling of string_attribute = integer_constant.
   R53714, corrected V.E.A = 123 for V.E.A beeing of unknown type.
2001.02.20  HH
   Use ExprNode.Flags to store "cast" information for the C generator.
   Handle special case: INTEGER varaible = string_operation()
   Parser changes, unary minus support.
2001.02.13  HH
   fix bug in ExtractOperationCallWithView where qNUMERIC was always
   assigned to DataClass, even for strings. Problems in interpreter!
2000.12.31  HH
   re-arrange boolean expressions according to C precedence of logical
   operators, so that the interpreter works correctly.

2000.08.28  HH
   fix problems with returncodes of Zeidon operations (integer-short).

2000.08.18  HH
   changes to support decimal return values for local operations.

2000.06.16  HH
   enable "NOT", so that "UNTIL" will work.

2000.04.20  HH
   fix bug with operation having decimal returncode.

2000.04.13  HH
   fix bug with constant inside parens.

2000.03.29  HH,BL
   handle datatype qTDATE as text.

2000.03.09  HH
   Solve problem calling a function with a string argument, where
   the actual argument is a date(time) attribute.

2000.02.15  HH
   Major change of fnOptimizeComplexExpression.
   The handling of "right" and "left" view was completely redesigned.

1999.07.13  HH ALL
   bug in fnOptNumExpr: Use Right struct do define the type of a
   constant in an operation call. The bug caused an if statement like
   IF V.E.A = Integer-constant to fail --- but only under the
   interpreter and if the View was incompletely defined (HPF).
   This change should have been made 1999.04.16, but was not !?

1999.06.11  DC Z10
   Corrected generation of ZeidonStringConcat for adding strings together
   so that the max length would be the length of the target and not the
   length of the source.

**** Start of old log entries.

22.04.1997  DonC
   Modified fnOptTextExpr to generate GetVariableFromAttribute instead of
   GetStringFromAttribute. (in 2 places)

24.04.1997  DonC
   Modified OptimizeExpression to return error for statement of form:
     s = a1 + a2 ... where all source variables (a1, a2,...) are not strings.

13.07.1997  DonC
   Modified fnOptTextExpr to properly handle the compare of an attribute to
   a string where the attribute was the first parameter.  It was already
   correct when the attribute was the second parameter.

13.07.1997  DonC
   Modified OptimizeExpression so that a data type of unknown (which is how
   a date is treated) will be processed as a string in all compare operations
   with a variable.

13.07.1997  DonC
   Modified fnOptTextExpr to properly handle the compare of an attribute to
   a string when the attribute is a date.  This required a modification of
   the generated CompareAttributeToString.

29.10.1997  DonC
   Corrected fnOptNumExpr to generate GetIntegerFromAttribute instead of
   GetDecimalFromAttribute when the variable is integer and the attribute
   qualifier is decimal. Made a similar change to correct
   CompareAttributeToDecimal to CompareAttributeToInteger and
   SetAttributeFromDecimal to SetAttributeFromInteger.

16.05.1998  DonC
   Modified fnOptNumExpr so that setting a decimal or integer variable from
   a string attribute would work. (QS 198)

25.06.1998  DonC
   Modified fnOptNumExpr to eliminate Zeidon error message on during a
   Parse error.

10.07.1998  DonC
   Modified fnOptNumExpr so that a decimal or integer could be set from
   a string constant (so it can be set to null).

17.07.1998  HH
   Added a check of complex expressions to fnOptimizeComplexExpression.
   We test
   - whether the expression is valid and issue an error message if not,
   - assign a data type to the left side of the expression, if the left
     side is of unknown type and the right side type is well defined.

27.07.1998  HH
   If, for a text attribute V.E.A, there was an assignment like
   V.E.AText-Attribute = Integer a SetAttributeFromString
   was erroneously generated. We now generate SetAttributeFromInteger
   or SetAttributeFromDecimal respectively.
   Bug fixed in assignment of text attribute from constant or define
   (delimiting 0-byte forgotten).
   Removed a redundant check of string expressions, because this is done in
   fnAnalyzeComplexExpression.

31.07.1998  DonC
   Modified fnOptNumExpr call to CreateOperationCallUsingAQ to set variable
   data type correctly.

14.08.1998  HH
   fixed a bug in fnOptimizeComplexExpression, so that "NOT" is handled now.

10.09.1998  HH
   bug XC221, error on complex IF-expresion with multiple paren levels
   and numerical and string comparisons.

25.09.1998  HH
   bug in fnOptNumExpr: Use Right struct do define the type of a
   variable in an operation call. The bug caused an assignement like
   V.E.A = Integer-variable to fail --- but only under the interpreter.

26.10.1998  HH
   added new function "CaseExpression" for analysis of
   "CASE integer OF" - syntax.
   Improve fnAnalyzeComplexExpression because of XC254.

28.12.1998  HH
   bug TB 457 fixed, "if string = date-attribute" will now generate
   "CompareAttributeToString", see fnOptimizeComplexExpression.

*/

/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS: OptimizeExpression
//             OptimizeSimpleNumericExpression
//             OptimizeSimpleTextExpression
//             AtomizeAQ
//
/////////////////////////////////////////////////////////////////////////////

#define ADDEXPRESSIONENTRYTOSTRING( p1, p2, p3, p4, p5, p6 ) AddExpressionEntryToString( pchLocalCode, plLocalCode, pchLocalText, plLocalText, p1, p2, p3, p4, p5, p6 )

zCHAR szLocal7Code[ MAXSTRING ];
zCHAR szLocal7Text[ MAXSTRING ];
zLONG lLocal7Code;
zLONG lLocal7Text;

// OPERATION: OptimizeExpression
//
// DESCRIPTION: OptimizeExpression is the function that is called to
//    create an expression fit for the XPG from and expression that was
//    created via the parse. This is the last function called before the
//    expression is inserted into the program instance. This function
//    is supposed to be able to handle any type of expression that can
//    be build via the parser regardless of data type or expression type
//    as long as it is in postfixed order. That is all expression fixing
//    should be done here. It extracts operation calls as well as converts
//    AQ's into their correct getsez and setsez.
//
//    The algorithm goes something like this:
//
//    First we get the first and the last elemtent to see what we can opt
//    out. If the last element of the expression is an EQUAL, then we know
//    that we have an assignment expression that the first element of is the
//    target. If the last element is not an EQUAL, then we have some other
//    type of expression (conditional) that we need to work with. If we are
//    dealing with an assignment expression, then we need to do some set up.
//
//    Assignment setup:
//    1: Scan for a reuse of the target.
//
//    Since this expression is in postfix notation, we can safely assume
//    that the target is the first entry in the expression. We need to scan
//    the expression to see if the target is used anywhere else in the expr.
//    If it in fact is, then we need to create a temporary value to use as
//    an intermediary when massaging the expression. Otherwise we would not
//    be able to guarantee the value of the variable given to reuse it when
//    these statements are executed. In other words, this function will
//    break an expression up into several smaller expressions that will be
//    executed independently. If the values are to be fixed, then a temp
//    must be used. For Example: assume the expression of strings: A = B + A
//    where A and B are string variables with values "XXX" and "YYY"
//    respectively. This expression could, at the end of this function, be
//    broken up into two seperate expressions:
//
//             1: strcpy_s( A, B ); A is "YYY" B is "YYY"
//             2: zstrcat( A, A ); A is "YYYYYY" B is "YYY"
//
//    Notice the problem, after expression 1:, the value of AAA is now "YYY"
//    and after expression 2: the value is "YYYYYY". What the expression
//    really wanted was A = "YYY" + "XXX" or "YYYXXX". This function
//    therefore creates the following three expressions for A = B + A
//
//    1: strcpy_s( szTemp, zsizeof( szTemp ), B ); A is "XXX" B is "YYY" szTemp is "YYY"
//    2: strcat_s( szTemp, zsizeof( szTemp ), A ); A is "XXX" B is "YYY" szTemp is "YYYXXX"
//    3: strcpy_s( A, szTemp ); A is "YYYXXX" B is "YYY" szTemp is "YYYXXX
//
//    If during the scan it is discovered that the target is not reused,
//    then it can be the target itself. Note that the target could be an
//    AttributeQualifier. Normally, we would have to create a temp into which
//    to retrieve the value of the AQ. If the AQ is not used any where else,
//    then there is no reason to create the first temp and retrieve the AQ.
//
sQExprStruct sGExprStruct = { 0 };
sQExprStruct sLeftStruct = { 0 };
sQExprStruct sRightStruct = { 0 };
sQExprStruct sOperatorStruct = { 0 };
zLONG       lGlobalTempID;
zLONG       lGlobalRightID;
zVIEW       vGlobalView;
zCHAR       szGlobalTempNumber[ 10 ];

#define qILLFORMEDEXPRESION            -16
#define qSIMPLEOPTIMIZE                  1
#define qSIMPLEOPTIMIZELEFTINDEXED       2
#define qCANNOTOPTIMIZENEEDTEMP          3
#define qCANNOTOPTIMIZEUSELEFT           4
#define qOPTIMIZEBYCONCATENATION         5
#define qCANNOTOPTIMIZECONCAT            6
#define qSIMPLEOPERATIONCALL             7
#define qSIMPLEOPERATIONCALLNORETURN     8
#define qCOPYASIS                        9

#define DATATYPE_ERROR -9999
#define DATATYPE_LPAREN_BIT     0x00000001
#define DATATYPE_ASSIGNMENT_BIT 0x00000002
#define DATATYPE_CONDEXPR_BIT   0x00000004

#define REMOVE_HIGH( n ) (n % 8192)
#define REMOVE_LOW( n )  (n / 8192)
#define ADD_HIGH( nType, nPos ) (nType += nPos*8192)

typedef struct sOperTable T_OPER_TABLE;
struct sOperTable
{
   // relative oper position in the expression view
   zLONG lOperPos;
   // relative position of "right argument" in the expression view.
   // -(data type) if there is no "right" argument.
   zLONG lRightPos;
   // relative position of "left argument" in the expression view.
   // -(data type) if there is no "left" argument.
   zLONG lLeftPos;
};

// prototypes
zLONG
fnOptimizeComplexExpression( zVIEW  vSubtask,
                             zVIEW  vTgt,
                             zVIEW  vSrc,
                             zLONG  lStartLevel );
zSHORT
fnAnalyzeComplexExpression( zVIEW   vSubtask,
                            zVIEW   vSrc,
                            T_OPER_TABLE *pOperTable,
                            zPLONG plOperTablePos );

zLONG
flDataType( zVIEW vExpr, zPLONG plDataTypeStack,
            zPLONG plStackPointer, zPLONG plResult,
            T_OPER_TABLE *pOperTable,
            zPLONG plOperTablePos,
            int iStepOverFirst );

zLONG
fnAnalyzeAssignmentStatement( zPLONG plTargetDataClass,
                              zVIEW  vSearchView );

zLONG
fnHandleOperatorTab( zVIEW vSubtask, zVIEW vSrc, zVIEW vTgt,
                     zVIEW vLeftView, zVIEW vRightView,
                     zVIEW vLeftTargetView,
                     T_OPER_TABLE *pOTab,
                     zLONG lStartLevel, zLONG lMaxLevel );

zVOID
fnHandleUnaryMinus( zVIEW vTgt, zVIEW vRightView,
                    T_OPER_TABLE *pOTab );

zVOID
fnCopyExprWithExtract( zVIEW vSubtask, zVIEW vTgt, zVIEW vLeftView );

zLONG
fnOptTextExpr( zVIEW  vSubtask,
               zVIEW  vTgt,
               zVIEW  vLeftView,
               zVIEW  vRightView,
               zVIEW  vOperatorView,
               zLONG  lStartLevel );

zLONG
fnOptNumExpr( zVIEW  vSubtask,
              zVIEW  vTgt,
              zVIEW  vLeftView,
              zVIEW  vRightView,
              zVIEW  vOperatorView,
              zLONG  lStartLevel );

zVOID
fnGenVarOpCall( zVIEW vSubtask, zVIEW vRightView, zVIEW vLeftView,
                zVIEW vTgt, zLONG lStartLevel );

zVOID
fnGenAnyVar( zVIEW  vSubtask, zVIEW vRightView, zVIEW vLeftView,
             zVIEW vTgt, zVIEW vOperatorView );

zSHORT
fnGenVarAttr( zVIEW  vSubtask, zVIEW vRightView, zVIEW vLeftView,
              zVIEW vTgt, zVIEW vOperatorView );

zVOID
fnGenConstOpCall( zVIEW  vSubtask,
                  zVIEW vRightView, zVIEW vLeftView,
                  zVIEW vTgt, zLONG lStartLevel );

zSHORT
fnGenConstAttr( zVIEW  vSubtask,
                zVIEW vRightView, zVIEW vLeftView,
                zVIEW vTgt, zVIEW vOperatorView );

zVOID
fnGenAttrOpCall( zVIEW  vSubtask,
                 zVIEW vRightView, zVIEW vLeftView,
                 zVIEW vTgt, zLONG lStartLevel );

zSHORT
fnGenAttrVar( zVIEW  vSubtask,
              zVIEW vRightView, zVIEW vLeftView,
              zVIEW vTgt, zVIEW vOperatorView,
              zLONG lViewID, zLONG lEntityID, zLONG lAttributeID,
              zPCHAR szEName, zPCHAR szAName );

zSHORT
fnGenAttrAttr( zVIEW  vSubtask,
               zVIEW vRightView, zVIEW vLeftView,
               zVIEW vTgt, zVIEW vOperatorView,
               zLONG lViewID, zLONG lEntityID, zLONG lAttributeID,
               zPCHAR szVName, zPCHAR szEName, zPCHAR szAName );

zVOID
fnGenOpCallOpCall( zVIEW vSubtask,
                   zVIEW vRightView, zVIEW vTgt,
                   zLONG lStartLevel );

zSHORT
fnGenOpCallAttr( zVIEW  vSubtask,
                 zVIEW vRightView, zVIEW vLeftView,
                 zVIEW vTgt, zVIEW vOperatorView,
                 zPCHAR szVName, zPCHAR szEName, zPCHAR szAName );

zLONG
CreateSourceZeidonStrOp( zVIEW  vSubtask,
                         zVIEW  vSrc,
                         zVIEW  vTgt,
                         zPCHAR pchTempNumber,
                         zLONG  lMaxLth );

zLONG
CreateSourceZeidonStrOpS( zVIEW  vSubtask,
                          zVIEW  vSrc,
                          zPCHAR pchLocalCode,
                          zPLONG plLocalCode,
                          zPCHAR pchLocalText,
                          zPLONG plLocalText,
                          zPCHAR pchTempNumber,
                          zLONG  lMaxLth );

zLONG
CreateSourceZeidonStrOpWithID( zVIEW  vSubtask,
                               zLONG  lID,
                               zVIEW  vTgt,
                               zPCHAR pchTempNumber,
                               zLONG  lMaxLth );

zLONG
CreateSourceZeidonStrOpSWithID( zVIEW  vSubtask,
                                zLONG  lID,
                                zPCHAR pchLocalCode,
                                zPLONG plLocalCode,
                                zPCHAR pchLocalText,
                                zPLONG plLocalText,
                                zPCHAR pchTempNumber,
                                zLONG  lMaxLth );
zLONG
CreateTargetZeidonStrOp( zVIEW  vSubtask,
                         zVIEW  vSrc,
                         zVIEW  vTgt,
                         zPCHAR pchOperationName );

zLONG
CreateTargetZeidonStrOpS( zVIEW  vSubtask,
                          zVIEW  vSrc,
                          zPCHAR pchLocalCode,
                          zPLONG plLocalCode,
                          zPCHAR pchLocalText,
                          zPLONG plLocalText,
                          zPCHAR pchOperationName );

zLONG
CreateTargetZeidonStrOpWithID( zVIEW  vSubtask,
                               zLONG  lID,
                               zVIEW  vTgt,
                               zPCHAR pchOperationName );

zLONG
CreateTargetZeidonStrOpSWithID( zVIEW  vSubtask,
                                zLONG  lID,
                                zPCHAR pchLocalCode,
                                zPLONG plLocalCode,
                                zPCHAR pchLocalText,
                                zPLONG plLocalText,
                                zPCHAR pchOperationName );

zLONG
ExtractOperationCallWithView( zVIEW  vSubtask,
                              zVIEW  vSrc,
                              zVIEW  vTgt,
                              zLONG  lNeedTemp,
                              sQExprStruct *psReturnVariable );

zLONG
AddRetCodeLong( zVIEW vSubtask );

zSHORT
fnCopyExprNode( zVIEW  vTgt,
                zVIEW  vSrc,
                zSHORT nPos,
                zBOOL  bNegateOffset );

zVOID
fnReArrangeLogOper( zVIEW v );
zVOID
fnMoveOR( zVIEW vExprOR, zVIEW vExprAND );


// >>>OptimizeExpression
// this is the main entry point
zOPER_EXPORT zLONG OPERATION
OptimizeExpression( zVIEW vSubtask,
                    zVIEW vTgt,
                    zVIEW vSrc )
{
   zVIEW vSearchView;      // a view to search on as to not lose real position
   zLONG lAnalyzeResult;   // a return code
   zLONG lDataClass;
   zLONG lLth;
   zLONG lLastOperator = 0;
   zLONG lRC;              // a return code

   // We will reset the expression to the beginning if the set fails, then no
   // expression has be built into vSrc.
   lRC = SetCursorFirstEntity( vSrc, "ExprNode", "" );
   if ( lRC < zCURSOR_SET )
      return( -1 );

   // create the search view
   CreateViewFromViewForTask( &vSearchView, vSrc, 0 );

   lAnalyzeResult = fnAnalyzeAssignmentStatement( &lDataClass, vSearchView );

   SetCursorFirstEntity( vSearchView, "ExprNode", "" );
// sGExprStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSearchView, "ExprNode" );
   GetExpressionStruct( &sGExprStruct, vSearchView, "ExprNode" );

   lGlobalTempID = -1;
   lGlobalRightID = -1;
   lRC = 0;
   switch( lAnalyzeResult )
   {
      case qCOPYASIS:
         lRC = SetViewFromView( vTgt, vSrc );
         break;

      case qSIMPLEOPTIMIZE:
         if ( lDataClass != qTEXT )
            lRC = OptimizeSimpleNumericExpression( vSubtask, vTgt, vSrc, lAnalyzeResult, 0 );
         else
            lRC = OptimizeSimpleTextExpression( vSubtask, vTgt, vSrc, lAnalyzeResult, 0 );

         break;

      case qSIMPLEOPTIMIZELEFTINDEXED:
         lRC = OptimizeSimpleTextExpression( vSubtask, vTgt, vSrc, lAnalyzeResult, 0 );
         break;

      case qCANNOTOPTIMIZENEEDTEMP:
         if ( lDataClass == qNUMERIC )
         {
            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
         }
         else
         {
            if ( sGExprStruct.lElementClass == qVARIABLE )
            {
               lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), sGExprStruct.lOffset );
               CreateViewFromViewForTask( &vGlobalView, vSrc, 0 );
               lGlobalTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
            }
            else
            if ( sGExprStruct.lElementClass == qATTRIBUTEQUALIFIER )
            {
               lRC = AtomizeAQ( sGExprStruct.szText, g_szViewName, g_szEntityName, g_szAttributeName,
                                &g_lViewID, &g_lEntityID, &g_lAttributeID, sRightStruct.lDataClass );
               lLth = -1;
               if ( g_lEntityID == 0 && g_lAttributeID == 0 )
                  lLth = GetLengthFromAttribute( g_lViewID, g_szEntityName, g_szAttributeName );

               if ( lLth == -1 )
                  lLth = 1024;

               lGlobalTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
               lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), lGlobalTempID );
            }

            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
            ReuseTempVariable( lGlobalTempID );
         }

         break;

      case qCANNOTOPTIMIZEUSELEFT:
         if ( lDataClass == qNUMERIC )
         {
            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
         }
         else
         {
            if ( sGExprStruct.lElementClass == qVARIABLE )
            {
               zLONG lLth;
               lGlobalTempID = sGExprStruct.lOffset;
               lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), sGExprStruct.lOffset );
               CreateViewFromViewForTask( &vGlobalView, vSrc, 0 );
            }
            else
            if ( sGExprStruct.lElementClass == qATTRIBUTEQUALIFIER )
            {
               lRC = AtomizeAQ( sGExprStruct.szText, g_szViewName, g_szEntityName, g_szAttributeName,
                                &g_lViewID, &g_lEntityID, &g_lAttributeID, sRightStruct.lDataClass );
               lLth = -1;
               if ( g_lEntityID == 0 && g_lAttributeID == 0 )
                  lLth = GetLengthFromAttribute( g_lViewID, g_szEntityName, g_szAttributeName );

               if ( lLth == -1 )
                  lLth = 1024;

               lGlobalTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
               lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), lGlobalTempID );
            }

            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
         }

         break;

      case qOPTIMIZEBYCONCATENATION:
         if ( lDataClass == qNUMERIC )
         {
            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
         }
         else
         {
            if ( sGExprStruct.lElementClass == qVARIABLE )
            {
               lGlobalTempID = sGExprStruct.lOffset;

               lRC = OptimizeSimpleTextExpression( vSubtask, vTgt, vSrc, lAnalyzeResult, 0 );
            }
            else
            {
               lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
            }
         }

         break;

      case qCANNOTOPTIMIZECONCAT:
         if ( lDataClass == qNUMERIC )
         {
            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
         }
         else
         {
            if ( sGExprStruct.lElementClass == qVARIABLE )
            {
               zLONG lLth;
               lGlobalTempID = sGExprStruct.lOffset;
               lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), sGExprStruct.lOffset );
               CreateViewFromViewForTask( &vGlobalView, vSrc, 0 );
            }
            else
            if ( sGExprStruct.lElementClass == qATTRIBUTEQUALIFIER )
            {
               lRC = AtomizeAQ( sGExprStruct.szText, g_szViewName, g_szEntityName, g_szAttributeName,
                                &g_lViewID, &g_lEntityID, &g_lAttributeID, sRightStruct.lDataClass );
               lLth = -1;
               if ( g_lEntityID == 0 && g_lAttributeID == 0 )
                  lLth = GetLengthFromAttribute( g_lViewID, g_szEntityName, g_szAttributeName );

               if ( lLth == -1 )
                  lLth = 1024;

               lGlobalTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
               lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), lGlobalTempID );
            }

            lRC = fnOptimizeComplexExpression( vSubtask, vTgt, vSrc, 0 );
         }

         break;

      case qSIMPLEOPERATIONCALL:
         ExtractOperationCallWithView( vSubtask, vSrc, vTgt, -1, NULL );
         lRC = 1;
         break;

      case qSIMPLEOPERATIONCALLNORETURN:
         ExtractOperationCallWithView( vSubtask, vSrc, vTgt, -2, NULL );
         lRC = 1;
         break;

      case qILLFORMEDEXPRESION:
         // break omitted

      default:
         DropView( vSearchView );
         return( qILLFORMEDEXPRESION );
         break;
   }

   lGlobalTempID = -1;
   DropView( vSearchView ); // drop the search view

   return( lRC );
}

zLONG
fnAnalyzeAssignmentStatement( zPLONG lpTargetDataClass,
                              zVIEW  vSearchView )
{
   zLONG lTargetVarID;
   zLONG lExprCount;
   zLONG lSimpleFlag;
   zLONG lIsCond;
   zLONG lNeedTemp;
   zLONG lTargetClass;
   zLONG lLeftIsOperation;
   zCHAR szTargetString[ 128 ];
   zLONG lRC;                    // return code

   // reset the search view
   SetCursorFirstEntity( vSearchView, "ExprNode", "" );

   // get the target (left entry) to see if it is a variable or an AQ.
// sGExprStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSearchView, "ExprNode" );
   GetExpressionStruct( &sGExprStruct, vSearchView, "ExprNode" );

   lLeftIsOperation = 0;
   lTargetClass = -1;
   *lpTargetDataClass = 0;

   // save the class
   if ( sGExprStruct.lElementClass == qOPERATIONCALL )
   {
      // if left is void
      if ( sGExprStruct.lDataType == qTVOID || sGExprStruct.lDataType == 0 )
         lLeftIsOperation = -1;
      else
         lLeftIsOperation = 1;
   }

   // if we got a variable or AQ
   if ( sGExprStruct.lElementClass == qVARIABLE ||           // 1044
        sGExprStruct.lElementClass == qOPERATIONCALL ||      // 1033
        sGExprStruct.lElementClass == qCONSTANT ||           // 1010
        sGExprStruct.lElementClass == qDEFINE   ||           // 1060
        sGExprStruct.lElementClass == qATTRIBUTEQUALIFIER )  // 2001
   {
      lTargetClass = sGExprStruct.lElementClass;

      // save the variable's ID
      if ( sGExprStruct.lElementClass == qVARIABLE )
      {
         lTargetVarID = sGExprStruct.lOffset;
         *lpTargetDataClass = sGExprStruct.lDataClass;
      }
      else // or save the aq's text
      if ( sGExprStruct.lElementClass == qATTRIBUTEQUALIFIER )
      {
         *lpTargetDataClass = GetClassBasedOnType( sGExprStruct.lDataType );
         strcpy_s( szTargetString, zsizeof( szTargetString ), sGExprStruct.szText );
      }

      // count the number of entities that are NOT operators
      lExprCount = 1;
      lSimpleFlag = 0;
      lIsCond = 0;

      // try to set cursor to the next expression entry
      lRC = SetCursorNextEntity( vSearchView, "ExprNode", "" );

      // while other expression entries exist...
      while( lRC == zCURSOR_SET )
      {
         // get the next element
      // sGExprStruct.lStructSize = sizeof( sQExprStruct );
      // lRC = GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSearchView, "ExprNode" );
         lRC = GetExpressionStruct( &sGExprStruct, vSearchView, "ExprNode" );

         // only check if it is not an operator
         if ( sGExprStruct.lElementClass != qOPERATOR )  // 1034
         {
            // if this is a variable and the target is a variable.
            if ( lTargetClass == qVARIABLE && sGExprStruct.lElementClass == qVARIABLE )
            {
               // see if it is a reuse of the target.
               if ( sGExprStruct.lOffset == lTargetVarID )
               {
                  // if count is not 1 then we have something like
                  //  x = y  + x + ... where a temp is needed.
                  if ( lExprCount != 1 )
                     lNeedTemp = 1;
                  else  // if it is a 1 then we have some thing like x = x + 1
                     lNeedTemp = -1;
               }
            }
            // see if the class is AQ and is a reuse of the target.
            else
            if ( lTargetClass == qATTRIBUTEQUALIFIER &&
                 sGExprStruct.lElementClass == qATTRIBUTEQUALIFIER )
            {
               // x = x + ...
               if ( zstrcmp( szTargetString, sGExprStruct.szText ) == 0 )
               {
                  if ( lExprCount != 1 )
                     lNeedTemp = 1;
                  else
                     lNeedTemp = -1;
               }
            }

            if ( sGExprStruct.lLevel < 10 )
               lExprCount++; // increment the counter
         }
         else // is operator
         {
            if ( sGExprStruct.lLevel < 10 )
            {
               if ( sGExprStruct.lOffset != qLBRACE &&
                    sGExprStruct.lOffset != qRBRACE )
               {
                  if ( IsCondOperator( sGExprStruct.lOffset ) == 0 )
                     lIsCond = 1;

                  lExprCount++; // increment the counter
               }
               else                  // is LBRACE or RBRACE
               if ( lExprCount == 1 )
               {
                  // 10/6/96 - DonC
                  // I am eliminating the nSimpleFlag setting.  It used to be
                  // used to force a temporary value when the target string
                  // variable was indexed as in: S1[3:5] = S2[1:4] + S3.
                  // Instead we are copying directly into the string in this case.
                  // nSimpleFlag = 1; // left is indexed
                  lSimpleFlag = 0; // no op this case.
               }
            }
         }

         // don't look inside []'s or ( )'s maybe?
         lRC = SetCursorNextEntity( vSearchView, "ExprNode", "" );
      }

      if ( lExprCount < 3 )
         return( qCOPYASIS );

      if ( lExprCount != 3 && lExprCount != 5 )
      {
         if ( lNeedTemp == 1 || lIsCond == 1 || lSimpleFlag == 1 )
         {
            return( qCANNOTOPTIMIZENEEDTEMP );
         }
         else
         if ( lNeedTemp == -1 )
            return( qCANNOTOPTIMIZECONCAT );
         else
            return( qCANNOTOPTIMIZEUSELEFT );
      }

      if ( lExprCount == 3 )
      {
         if ( lLeftIsOperation != 0 && sGExprStruct.lOffset == qCLOSEOPER )
         {
            if ( lLeftIsOperation == -1 )
               return( qSIMPLEOPERATIONCALLNORETURN );
            else
               return( qSIMPLEOPERATIONCALL );
         }

         if ( lSimpleFlag == 1 ) // left is indexed
            return( qSIMPLEOPTIMIZELEFTINDEXED );
         else
            return( qSIMPLEOPTIMIZE );
      }

      if ( lExprCount == 5 )
      {
         if ( lNeedTemp == 1 || lIsCond == 1 )
            return( qCANNOTOPTIMIZENEEDTEMP );
         else
         if ( lNeedTemp == -1 )
            return( qOPTIMIZEBYCONCATENATION );
         else
            return( qCANNOTOPTIMIZEUSELEFT );
      }
   }

   return( qCANNOTOPTIMIZEUSELEFT );
   //return( qCANNOTOPTIMIZENEEDTEMP );
   //return( qILLFORMEDEXPRESION );
}

// OPERATION: OptimizeSimpleNumericExpression
//
// DESCRIPTION: OptimizeSimpleNumericExpression is the function that is
//    ro evaluate and setup a three part assignment expression. A three part
//    assignment expression is something like a = b. Where you have a left,
//    an operator, and a right. We start with the left. If we have a variable
//    then we check out the next entry. If it is also a variable, an expr is
//    formed based on the type of the data. Numeric is the data type expected.
//
//    if LEFT is VARIABLE
//       if RIGHT is VARIABLE
//       if RIGHT is CONSTANT
//       if RIGHT is DEFINE
//          if type is numeric, put out as is.
//       if RIGHT is AQ
//          create a Get(type)from attribute
//    if LEFT is CONSTANT
//    if LEFT is DEFINE
//       if RIGHT is VARIABLE
//       if RIGHT is CONSTANT
//       if RIGHT is DEFINE
//          if type is numeric, put out as is.
//       if RIGHT is AQ
//          create a core call based on type
//    if LEFT is AQ
//       if RIGHT is VARIABLE
//       if RIGHT is CONSTANT
//       if RIGHT is DEFINE
//          create a set core call based on type
//       if RIGHT is AQ
//          create a set core call from aq
//

zOPER_EXPORT zLONG OPERATION
OptimizeSimpleNumericExpression( zVIEW vSubtask, zVIEW vTgt, zVIEW vSrc,
                                 zLONG lAnalyzeResult, zLONG lStartLevel )
{
   zVIEW  vLeftView;
   zVIEW  vRightView;
   zVIEW  vOperatorView;
   zLONG lRC;

   // assume we re positioned correctly.
   CreateViewFromViewForTask( &vLeftView, vSrc, 0 );

   // go to the next expr value by level as it may not be the
   // immediate next level
   SetCursorNextEntityByInteger( vSrc, "ExprNode", "Level", lStartLevel, "" );
   CreateViewFromViewForTask( &vRightView, vSrc, 0 );
   SetCursorLastEntity( vSrc, "ExprNode", "" );
   CreateViewFromViewForTask( &vOperatorView, vSrc, 0 );

   lRC = fnOptNumExpr( vSubtask, vTgt, vLeftView, vRightView, vOperatorView, lStartLevel );
   return( lRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// Don Christensen Comments - 11/1/94
//
// The following is my understanding of the fnOptimizeComplexExpression
// routine from my analyzing the code to fix bugs.  It is therefore
// not complete and probably not completely accurate.
//
// This routine transforms the ExprNode entries from vSrc
// into the ExprNode entries in vTgt.  If all the entries
// are arithmetic and the values regular variables, then the affect
// of this routine is to move the entries from vSrc to
// vTgt, without change.  If all the entries are arithmetic,
// but some of the values are AQ's, this routine changes the AQ's to
// variables and creates GetAttribute or SetAttribute operations as
// necessary to convert the data.
//
// If the entries in vSrc are part of an IF or WHERE, the
// entries are converted to CompareAttribute operations in vTgt.
//
// The ExprNode entries are processed in sequential order from the
// vSrc.
//
// The complexities of this routine are primarily around handling
// the parts of the expression enclosed in parens.  The right and
// left parens themselves are moved from vSrc to vTgt
// in the order they are processed.  The variables, AQ's, etc. are
// not moved as they are processed, but only when the corresponding
// operation is handled.  Sometimes, two variables are processed with
// the operation, as in A+B, which is A,B,+ in vSrc order.
// Other times, only a single variable is moved and at other times
// only the operation itself is moved.  This is controlled by the
// variables lLeftClass and nRightClass.  If lLeftClass is 0, then
// a left variable exists to be moved.  The same is true for
// nRightClass.
//
// Documentation of redesign february 2000, HH
// /////////////
//
// The "optimization" is based on part expressions as
//  "left" "oper" "right" .
// Actually, the whole expression is in polish notation, and the
// problem is to find "right" and "left" for an operator:
// Example:
//    n = 30*40 + 50*60
// is in polish notation
//    n,3,4,*,5,6,*,+,=
// For the first "*" left is "30" and "right" is "40".
// For the second "*" left is "50" and "right" is "60".
// For the "+" left and right are intermediate results (30*40)
//  and (50*0).
// The "=" assignment has a left of n, the right is intermediate.
//
//
// The old code was wrong and could not handle "real" complex expressions
// with lots of parentheses.
//
// We got a function, fnAnalyzeComplexExpression, which goes over the
// expression in a recursive way (using a stack), for checking the
// consistency of the expression.
// We now use this function also to maintain an oper table.
// The table has 3 entries:
// Oper position, left side position, right side position.
// The positions are the sequence numbers in the source expression view.
// Intermediate results have position -t,
//  with "t" being the intermediate data type.
// Later, when an operator is processed, we set "right view" and
// "left view" according to that oper table.
//
// The oper table for the example above (n = 30*40 + 50*60 )
// is as follows:
//    ( 4, 3, 2 ) for 30*40
//    ( 7, 5, 6 ) for 50*60
//    ( 8,-t,-t ) for intermediate + intermediate
//    ( 9, n,-t ) for n = intermediate
//
// However, this concept will only work, if the source view is not
// changed in a way that expression nodes are added, deleted or moved.
// This was the hardest part to accomplish.
//
/////////////////////////////////////////////////////////////////////////////
zLONG
fnOptimizeComplexExpression( zVIEW  vSubtask,
                             zVIEW  vTgt,
                             zVIEW  vSrc,
                             zLONG  lStartLevel )
{
   zLONG  lMaxLevel;
   zVIEW  vLeftView;
   zVIEW  vRightView;
   zVIEW  vOperatorView;
   zVIEW  vLeftTargetView;

   zSHORT nEqualExpression;
   zSHORT nViewIndex;
   zVIEW  vViewStack[ 10 ];
   zSHORT IsCondExpression;
   #define MAX_OPER_TAB 512
   T_OPER_TABLE tOperTable[ MAX_OPER_TAB ];
   zLONG  lOperTablePos = 0;
   zLONG  lRel;
   int    i, iFound;
   T_OPER_TABLE *pOTab;
   zLONG  lRC;

   // The interpreter is not able to handle the C operator precedence
   // in which on the same logical level AND is evaluated prior to OR.
   // Therefore, we rearrange the AND and OR operators according to
   // C operator precedence.
   fnReArrangeLogOper( vSrc );

   // additional analysis and setup of operator table.
   SetCursorFirstEntity( vSrc, "ExprNode", "" );
   IsCondExpression = fnAnalyzeComplexExpression( vSubtask, vSrc, tOperTable, &lOperTablePos );
   if ( IsCondExpression < 0 )
      return( -1 );

   CreateEntity( vTgt, "ExprNode", zPOS_AFTER );
   CreateViewFromViewForTask( &vLeftTargetView, vTgt, 0 );

   // assume we re positioned correctly.
   CreateViewFromViewForTask( &vLeftView, vSrc, 0 );

   // start the left right and operator all pointing at the same position
   // in the expression
   CreateViewFromViewForTask( &vRightView, vSrc, 0 );
   CreateViewFromViewForTask( &vOperatorView, vSrc, 0 );

   nViewIndex = 0;
   vViewStack[ 0 ] = vTgt;

   // Set the nEqualExpression indicator to denote whether or not this
   // expression is to set a variable or AQ.  If it is, later code
   // needs to know to handle the value to be set correctly.
   SetCursorLastEntity ( vLeftView, "ExprNode", "" );
   lRC = CompareAttributeToInteger( vLeftView, "ExprNode", "Offset", qEQUAL );
   if ( lRC == 0 )
      nEqualExpression = 0;
   else
      nEqualExpression = 1;

   SetCursorFirstEntity( vLeftView, "ExprNode", "" );

   lMaxLevel = lStartLevel + 10;
   sGExprStruct.lElementClass = 0;
   sGExprStruct.lDataType = 0;
   sGExprStruct.lFlags = 0;

   do
   {
      // get the ExprNode at the "Front" of the expression
   // sGExprStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSrc, "ExprNode" );
      GetExpressionStruct( &sGExprStruct, vSrc, "ExprNode" );

      switch ( sGExprStruct.lElementClass )
      {
         case qOPERATOR:
            // set up the local expression to retain the postfixed order.
            // since postfixed order would be like A B +, we put the operator
            // two positions past the next insertion.
            // get what the actual operator is.
            // if the operator is a L or R paren, the put it in the local
            // expression in place.
            switch ( sGExprStruct.lOffset )  // Operator Type
            {
            case qLBRACE:
            case qRBRACE:
            case qOPENOPER:
            case qCLOSEOPER:
               continue;

            case qOR:
            case qAND:
               fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
               continue;

            case qNOT:
               fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
               continue;

            case qLPAREN:
               fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
               lMaxLevel += 10;
               SetViewFromView( vLeftTargetView, vTgt );
               CreateViewFromViewForTask( &vViewStack[ nViewIndex++ ], vTgt, 0 );
               continue;

            case qRPAREN:
            {
               // if we come to a right paren and there is an
               //  operation call inside, as we would get for
               //    RETURN operation( params, .. )
               //  we must handle that special situation
               sQExprStruct sExprStruct = { 0 };
               sExprStruct.lElementClass = 0;

               //  we position the left view to the qOPERATIONCALL entry
               SetViewFromView( vLeftView, vSrc );
               lRC = SetCursorPrevEntity( vLeftView, "ExprNode", "" );
            // sExprStruct.lStructSize = sizeof( sQExprStruct );
            // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vLeftView, "ExprNode" );
               GetExpressionStruct( &sExprStruct, vLeftView, "ExprNode" );

               if ( sExprStruct.lElementClass != qOPERATOR )
               {
                  // left of the closing paren there is no operator ...
                  // this is possible for
                  //   RETURN ( 5 )
                  // copy the expression node, with extraction of a temp variable, if needed.
                  fnCopyExprWithExtract( vSubtask, vTgt, vLeftView );
               }
               else
               if ( sExprStruct.lOffset == qCLOSEOPER )
               {
                  // we got an operation call inside ...
                  // position left view to qOPERATIONCALL
                  lRC = zCURSOR_SET;
                  while ( sExprStruct.lElementClass != qOPERATIONCALL && lRC == zCURSOR_SET )
                  {
                     lRC = SetCursorPrevEntity( vLeftView, "ExprNode", "" );
                     if ( lRC == zCURSOR_SET )
                     {
                     // sExprStruct.lStructSize = sizeof( sQExprStruct );
                     // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vLeftView, "ExprNode" );
                        GetExpressionStruct( &sExprStruct, vLeftView, "ExprNode" );
                     }
                  }

                  if ( lRC == zCURSOR_SET )
                  {
                     fnOptNumExpr( vSubtask, vTgt, vLeftView, 0, 0, lStartLevel );
                  }
                  else
                  {
                     // we should never come here
                     SysMessageBox( vTgt, "unexpected error during VML parse", "Operation call not found", zBEEP );
                     return( -1 );
                  }
               }

               lRC = fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
               lMaxLevel -= 10;

               SetViewFromView( vLeftTargetView, vViewStack[ --nViewIndex ] );
               lRC = CheckExistenceOfEntity( vLeftTargetView, "Expression" );
               SetCursorPrevEntity( vLeftTargetView, "ExprNode", "" );
               DropView( vViewStack[ nViewIndex ] );
               continue;
            }

            default:
               // Handle the Operator according to Operator Table entry

               // for the  current operator, determine "right" and "left" side.
               // This was already done under fnAnalyzeComplexExpression.
               // The results were stored in a table, which we want to access in order to get "right" and "left" side.
               // To look up the table, we use the relative position in the source view.
               lRel = GetRelativeEntityNumber( vSrc, "ExprNode", "", 0 );
               iFound = 0;
               pOTab = tOperTable;
               for ( i = 0; i < lOperTablePos; i++, pOTab++ )
               {
                  if ( pOTab->lOperPos - 1 == lRel )
                  {
                     iFound = 1;
                     break;
                  }
               }

               if ( iFound == 0 )
               {
                  // Houston, we got a problem ... should not occur
                  SysMessageBox( vTgt, "unexpected error during VML parse", "Operator not found in Table", zBEEP );
                  return( -1 );
               }

               // special handling for unary operators
               if ( sGExprStruct.lOffset == qUNARYMINUS )
               {
                  fnHandleUnaryMinus( vTgt, vRightView, pOTab );
                  lRC = 0;
                  break;
               }

               // If this is an = oper, set vTgt to beginning,
               // where the value to be set is located.
               if ( sGExprStruct.lOffset == qEQUAL )
                  SetCursorFirstEntity( vLeftTargetView, "ExprNode", "" );

               lRC = fnHandleOperatorTab( vSubtask, vSrc, vTgt, vLeftView, vRightView, vLeftTargetView, pOTab, lStartLevel, lMaxLevel );

               break;
            } // end switch Operator Type

            break;

         // if the entry is an operation call we extract it out into its own expression and put back a temp variable that was generated into this expression
         case qOPERATIONCALL:
         case qVARIABLE:
         case qCONSTANT:
         case qDEFINE:
         case qATTRIBUTEQUALIFIER:
            // if the "thing" was not an operation call or a string variable, just push it onto the stack. Pushing onto the stack is setting
            // the "right" view to the right most position ... and putting the "left" view where the right was. Eventually we have the left
            // pointing at an operand, the right pointing at an operand and the operator pointing at an operator was PUSH( lExprEntry );
            break;
         default:
            break;
      }
   } while ( SetEntityCursor( vSrc, "ExprNode", "Level", zPOS_NEXT | zQUAL_INTEGER | zQUAL_LESS_THAN, &lMaxLevel, "", "", 0, "", 0 ) >= zCURSOR_SET );

   // delete dummy header
   lRC = SetCursorFirstEntity( vTgt, "ExprNode", "" );
   if ( lRC >= zCURSOR_SET )
      DeleteEntity( vTgt, "ExprNode", zREPOS_AFTER );

   return( 0 );
}

// >>>fnReArrangeLogOper
// re-arrange logical operators according to C precedence
zVOID
fnReArrangeLogOper( zVIEW vExpr )
{
   // walk through the expression view
   // if we find, on the same logical level, an OR preceding an AND, we will move it behind the last AND of that logical level.
   zVIEW  vExprAND;
   zVIEW  vExprOR;
   sQExprStruct sExprStruct = { 0 };
   zSHORT nState = 0;
   zSHORT nRC;

   CreateViewFromViewForTask( &vExprAND, vExpr, 0 );
   CreateViewFromViewForTask( &vExprOR, vExpr, 0 );

   for ( nRC = 0;
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vExpr, "ExprNode", "" ) )
   {
      // if we find an OR followed by one ore more AND's, then we will move the OR behind the last AND of that chain.
   // sExprStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vExpr, "ExprNode" );
      GetExpressionStruct( &sExprStruct, vExpr, "ExprNode" );

      // skip over function calls

      // if we find an open paren, go down one level recursively
      if ( sExprStruct.lOffset == qLPAREN )
      {
         nRC = SetCursorNextEntity( vExpr, "ExprNode", "" );
         if ( nRC >= zCURSOR_SET )
            fnReArrangeLogOper( vExpr );
      }

      // if we find a closing paren, go up to calling level
      if ( sExprStruct.lOffset == qRPAREN )
      {
         if ( nState == 2 )
         {
            // we have an AND following an OR, move the OR behind AND
            fnMoveOR( vExprOR, vExprAND );
            nState = 0;
         }

         nRC = SetCursorNextEntity( vExpr, "ExprNode", "" );
         break;
      }

      // if we find an OR set vTargetOR to current
      if ( sExprStruct.lOffset == qOR )
      {
         if ( nState == 2)
         {
            // OR and AND found, move the OR
            fnMoveOR( vExprOR, vExprAND );
         }

         SetViewFromView( vExprOR, vExpr );
         nState = 1;
      }

      // if we find an AND set vTargetAND to current
      if ( sExprStruct.lOffset == qAND )
      {
         switch ( nState )
         {
            case 0: // no OR found, skip
               break; // nothing to do
            case 1:
            case 2:
               SetViewFromView( vExprAND, vExpr );
               nState = 2;
               break;
         }
      }
   }

   if ( nState == 2 )
   {
      // we have an AND following an OR, move the OR behind AND
      fnMoveOR( vExprOR, vExprAND );
   }

   DropView( vExprAND );
   DropView( vExprOR );
   return;
}

// >>> fnHandleUnaryMinus
zVOID
fnHandleUnaryMinus( zVIEW vTgt, zVIEW vRightView,
                    T_OPER_TABLE *pOTab )
{
   // lLeftPos has no meaning for a unary operator
   if ( pOTab->lRightPos > 0 )
   {
      // the operand is not an intermediate result, so copy it to the target view
      SetCursorFirstEntity( vRightView, "ExprNode", "" );
      SetCursorRelativeEntity( vRightView, "ExprNode", pOTab->lRightPos - 1, "" );
      fnCopyExprNode( vTgt, vRightView, zPOS_AFTER, FALSE );
   }

   // copy the unary operator itself to the target view
   SetCursorFirstEntity( vRightView, "ExprNode", "" );
   SetCursorRelativeEntity( vRightView, "ExprNode", pOTab->lOperPos - 1, "" );
   fnCopyExprNode( vTgt, vRightView, zPOS_AFTER, FALSE );
}

// >>> fnHandleOperatorTab
// Handle an Operator Table Entry
zLONG
fnHandleOperatorTab( zVIEW  vSubtask,
                     zVIEW  vSrc, zVIEW vTgt,
                     zVIEW  vLeftView, zVIEW vRightView, zVIEW vLeftTargetView,
                     T_OPER_TABLE *pOTab,
                     zLONG lStartLevel, zLONG lMaxLevel )
{
   sQExprStruct sGExpr = { 0 };
   zLONG lRC;

   if ( (pOTab->lRightPos > 0) && (pOTab->lLeftPos > 0) )
   {
      zSHORT iVarEqStringOper = 0; // indicate INTEGER = string_function()

      // This is the easy case:
      //  right and left side of the operator are well defined
      // Example
      // d = 1 + 2
      // (d, 1, 2, +, =)
      //     |  |  oper
      //     |  right
      //     left
      //

      SetCursorFirstEntity( vRightView, "ExprNode", "" );
      SetCursorRelativeEntity( vRightView, "ExprNode", pOTab->lRightPos - 1, "");
      SetCursorFirstEntity( vLeftView, "ExprNode", "" );
      SetCursorRelativeEntity( vLeftView, "ExprNode", pOTab->lLeftPos - 1, "");

      // Text or Numeric ?
      //  Take vRightView (vLeftView would do as well)
   // sGExpr.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sGExpr, vRightView, "ExprNode" );
      GetExpressionStruct( &sGExpr, vRightView, "ExprNode" );

      if ( sGExpr.lDataType == qTSTRING && sGExpr.lElementClass == qOPERATIONCALL )
      {
         // right side is operation returning string
         sQExprStruct sLeftExpr = { 0 };
      // sLeftExpr.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sLeftExpr, vLeftView, "ExprNode" );
         GetExpressionStruct( &sLeftExpr, vLeftView, "ExprNode" );
         if ( sLeftExpr. lDataType == qTINTEGER &&
              sLeftExpr.lElementClass == qVARIABLE )
         {
            // left side is integer variable
            iVarEqStringOper = 1;  // indicate i = string_function()
         }
      }

      if ( sGExpr.lDataType == qTUNKNOWN )
      {
         // as right type is unknown, better take the left view
      // sGExpr.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sGExpr, vLeftView, "ExprNode" );
         GetExpressionStruct( &sGExpr, vLeftView, "ExprNode" );
      }

      if ( iVarEqStringOper == 0 &&
           (sGExpr.lDataType == qTSTRING ||
            sGExpr.lDataType == qTCHARACTER  ||
            sGExpr.lDataType == qTENTITYNAME ||
            sGExpr.lDataType == qTATTRIBUTENAME ||
            sGExpr.lDataType == qTDATE || // date and datetime
            sGExpr.lDataType == 0 ) )  // 0 is unknown, which Date is
      {
         // This is for text expressions
         lRC = fnOptTextExpr( vSubtask, vTgt, vLeftView, vRightView, vSrc, lMaxLevel - 10 );
      }
      else
      {
         // this is for numerical expressions
         lRC = fnOptNumExpr( vSubtask, vTgt, vLeftView, vRightView, vSrc, lStartLevel );
      }
   }
   else
   if ( (pOTab->lRightPos <= 0) && (pOTab->lLeftPos > 0) )
   {
      // the left side argument is defined in the source view.
      //  the right side is an intermediate result.
      // Example
      // d = 2 + 3*4
      // (d, 2, 3, 4, *, +, =)
      //     |           oper
      //     left
      //        right is (3*4)
      //

      SetCursorFirstEntity( vLeftView, "ExprNode", "" );
      SetCursorRelativeEntity( vLeftView, "ExprNode", pOTab->lLeftPos - 1, "");

   // sGExpr.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sGExpr, vLeftView, "ExprNode" );
      GetExpressionStruct( &sGExpr, vLeftView, "ExprNode" );

      SetViewFromView( vTgt, vLeftTargetView );

      if ( sGExpr.lDataType == qTUNKNOWN )
      {
         // as data type is unknown, take the oper table entry of the other side
         sGExpr.lDataType = - pOTab->lRightPos;
      }

      if ( sGExpr.lDataType == qTSTRING ||
           sGExpr.lDataType == qTCHARACTER  ||
           sGExpr.lDataType == qTENTITYNAME ||
           sGExpr.lDataType == qTDATE || // date and datetime
           sGExpr.lDataType == qTATTRIBUTENAME )
      {
         // text expression
         lRC = fnOptTextExpr( vSubtask, vTgt, vLeftView, 0, vSrc, lMaxLevel - 10 );
      }
      else
      {
         // numerical expression
         lRC = fnOptNumExpr( vSubtask, vTgt, vLeftView, 0, vSrc, lStartLevel );
      }

   }
   else
   if ( (pOTab->lRightPos > 0) && (pOTab->lLeftPos <= 0) )
   {
      // the right side argument is defined in the source view.
      //  the left side is an intermediate result.
      // Example
      // d = 3*4 + 2
      // (d, 3, 4, *, 2, +, =)
      //              |  oper
      //              right
      //     left is (3*4)
      //

      SetCursorFirstEntity( vRightView, "ExprNode", "" );
      SetCursorRelativeEntity( vRightView, "ExprNode", pOTab->lRightPos - 1, "" );

   // sGExpr.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sGExpr, vRightView, "ExprNode" );
      GetExpressionStruct( &sGExpr, vRightView, "ExprNode" );

      if ( sGExpr.lDataType == qTUNKNOWN )
      {
         // as data type is unknown, take the oper table entry of the other side
         sGExpr.lDataType = - pOTab->lLeftPos;
      }

      if ( sGExpr.lDataType == qTSTRING ||
           sGExpr.lDataType == qTCHARACTER  ||
           sGExpr.lDataType == qTENTITYNAME ||
           sGExpr.lDataType == qTDATE || // date and datetime
           sGExpr.lDataType == qTATTRIBUTENAME )

      {
         // text expression
         lRC = fnOptTextExpr( vSubtask, vTgt, 0, vRightView, vSrc, lMaxLevel - 10 );
      }
      else
      {
         // numerical expression
         lRC = fnOptNumExpr( vSubtask, vTgt, 0, vRightView, vSrc, lStartLevel );
      }
   }
   else
   {
      // we get here, if left and right side are
      //  intermediate, as it would be for
      // d = 3*4 + 2*5
      // (d, 3, 4, *, 2, 5, *, +, =)
      //                       oper
      //     left is 3*4
      //              right = 2*5
      //
      // Obviously, it must be a numerical expression

      lRC = fnOptNumExpr( vSubtask, vTgt, 0, 0, vSrc, lStartLevel );
   }

   return( lRC );
}

// >>>fnMoveOR
zVOID
fnMoveOR( zVIEW vExprOR, zVIEW vExprAND )
{
   CreateEntity( vExprAND, "ExprNode", zPOS_AFTER );
   SetMatchingAttributesByName( vExprAND, "ExprNode", vExprOR, "ExprNode", zSET_ALL );
   DeleteEntity( vExprOR, "ExprNode", zREPOS_NONE );
}

// >>>fnCopyExprWithExtract
// copy the expression node (vLeftView) to vTgt
// if needed, a  temp variable is generated.
zVOID
fnCopyExprWithExtract( zVIEW vSubtask, zVIEW vTgt, zVIEW vLeftView )
{
   zLONG   lViewID;
   zLONG   lEntityID;
   zLONG   lAttributeID;
   zCHAR   szVName[ 33 ];
   zCHAR   szEName[ 33 ];
   zCHAR   szAName[ 33 ];
   zCHAR   szOperName[ 33 ];
   zLONG   lTempID;
   zLONG   lLth;

// sLeftStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sLeftStruct, vLeftView, "ExprNode" );
   GetExpressionStruct( &sLeftStruct, vLeftView, "ExprNode" );
   switch( sLeftStruct.lElementClass )
   {
      case qVARIABLE:
      case qCONSTANT:
      case qDEFINE:
         // just move the expression entry to the target
         fnCopyExprNode( vTgt, vLeftView, zPOS_AFTER, FALSE );
         break;

      case qATTRIBUTEQUALIFIER:
         AtomizeAQ( sLeftStruct.szText, szVName, szEName, szAName, &lViewID, &lEntityID,
                    &lAttributeID, sLeftStruct.lDataClass );

         // generate a temp variable according to the attr type
         switch ( sLeftStruct.lDataType )
         {
            case qTDECIMAL:
               lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
               strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
               break;

            case qTINTEGER:
               lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
               strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
               break;

            case qTSTRING: // not yet tested
               lLth = GetLengthFromAttribute( lViewID, szEName, szAName );
               if ( lLth == -1 )
                  lLth = 1024;

               lTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
               strcpy_s( szOperName, zsizeof( szOperName ), "GetStringFromAttribute" );
               break;

            case qTDATE: // not yet tested
               lTempID = GenerateTempVariable( vSubtask, qTSTRING, 18 );
               strcpy_s( szOperName, zsizeof( szOperName ), "GetStringFromAttribute" );
               break;

            default:
               TraceLineS("Unexpected Error Situation ", "TZVMP7AA 2" );
               return;
         }

         if ( lGlobalTempID == -1 ) // what is this good for ?
            lGlobalTempID = lTempID;

         // Generate the setting of the temp variable into the expression blob.
         AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0, sLeftStruct.lDataType,
                                         lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

         // Copy the temp variable as a substitute for the attribute to the target view.
         InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC, sLeftStruct.lDataType,
                                       lTempID, 0, "", 0, zPOS_AFTER );
         SetCursorLastEntity( vTgt, "ExprNode", "" ); // necessary?
         break;

      default:
         // We should never get here!
         TraceLineS( "Unexpected Error Situation ", "TZVMP7AA 1" );
         fnCopyExprNode( vTgt, vLeftView, zPOS_AFTER, FALSE );
         break;
   }
}

// >>>fnOptNumExpr
zLONG
fnOptNumExpr( zVIEW  vSubtask,
              zVIEW  vTgt,
              zVIEW  vLeftView,
              zVIEW  vRightView,
              zVIEW  vOperatorView,
              zLONG  lStartLevel )
{
   zLONG   lTemp;
   zLONG   lViewID;
   zLONG   lEntityID;
   zLONG   lAttributeID;
   zCHAR   szVName[ 33 ];
   zCHAR   szEName[ 33 ];
   zCHAR   szAName[ 33 ];
   zVIEW   vSave;
   zLONG   lRC;

   if ( vOperatorView != 0 )
   {
   // sOperatorStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sOperatorStruct, vOperatorView, "ExprNode" );
      GetExpressionStruct( &sOperatorStruct, vOperatorView, "ExprNode" );
   }
   else
      sOperatorStruct.lOffset = qEQUAL;

   // assume were positioned correctly.
   if ( vLeftView != 0 )
   {
   // sLeftStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sLeftStruct, vLeftView, "ExprNode" );
      GetExpressionStruct( &sLeftStruct, vLeftView, "ExprNode" );
   }
   else
   {
      if ( sOperatorStruct.lOffset == qEQUAL )
      {
         sLeftStruct.lElementClass = qVARIABLE;
         sLeftStruct.lOffset = lGlobalTempID;
      }
      else
      {
         sLeftStruct.lElementClass = 0;
         sLeftStruct.lOffset = 0;
      }
   }

   if ( vRightView != 0 )
   {
   // sRightStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sRightStruct, vRightView, "ExprNode" );
      GetExpressionStruct( &sRightStruct, vRightView, "ExprNode" );
   }
   else
   {
      // Depending on whether the result attribute is integer or decimal,
      // create a temporary variable to hold the result.
      if ( sLeftStruct.lDataType == qTDECIMAL )
      {
      // lGlobalTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         sRightStruct.lElementClass = qVARIABLE;
         sRightStruct.lOffset = lGlobalTempID;
         sRightStruct.lDataType = qTDECIMAL;
      }
      else
      {
      // lGlobalTempID = GenerateTempVariable( vSubtask, qTINTEGER, 10 );
         sRightStruct.lElementClass = qVARIABLE;
         sRightStruct.lOffset = lGlobalRightID;
         sRightStruct.lDataType = qTINTEGER;
      // 9/17/93 sRightStruct.lOffset = lGlobalTempID;
      }
   }

   // do different logic based on the element classes of the expr entries
   switch ( sLeftStruct.lElementClass )
   {
      // if the left is a variable or nothing
      case 0:
      case qVARIABLE:
         // set next expr element will tell us what to do.
         switch( sRightStruct.lElementClass )
         {
            case qOPERATIONCALL:
               fnGenVarOpCall( vSubtask, vRightView, vLeftView, vTgt, lStartLevel );
               if ( sOperatorStruct.lOffset == qEQUAL )
                  break;

               // else no break;
            case qVARIABLE:
            case qCONSTANT:
            case qDEFINE:
               // GenerateVariable = Variable|Constant|Define
               fnGenAnyVar( vSubtask, vRightView, vLeftView, vTgt, vOperatorView );
               return( 0 ); // put out as is

            case qATTRIBUTEQUALIFIER:
               return( fnGenVarAttr( vSubtask, vRightView, vLeftView, vTgt, vOperatorView ) );

            default:
               break;
         }

         break;

      case qCONSTANT:
      case qDEFINE:
         switch( sRightStruct.lElementClass )
         {
            case qOPERATIONCALL:
               fnGenConstOpCall( vSubtask, vRightView, vLeftView, vTgt, lStartLevel );
               // no break;
            case qVARIABLE:
            case qCONSTANT:
            case qDEFINE:
               fnGenAnyVar( vSubtask, vRightView, vLeftView, vTgt, vOperatorView );
               return( 0 ); // put out as is

            case qATTRIBUTEQUALIFIER:
               // Ee need to do a compare. Create the compare operation and put out
               // the opposite operator and 0.
               return( fnGenConstAttr( vSubtask, vRightView, vLeftView, vTgt, vOperatorView ) );

            default:
               break;
         }

         break;

      case qATTRIBUTEQUALIFIER:
         AtomizeAQ( sLeftStruct.szText, szVName, szEName, szAName,
                    &lViewID, &lEntityID, &lAttributeID, sLeftStruct.lDataClass );

         switch( sRightStruct.lElementClass )
         {
            case qOPERATIONCALL:
               fnGenAttrOpCall( vSubtask, vRightView, vLeftView, vTgt, lStartLevel );
               // no break;
            case qVARIABLE:
            case qCONSTANT:
            case qDEFINE:
               return( fnGenAttrVar( vSubtask, vRightView, vLeftView, vTgt, vOperatorView,
                                     lViewID, lEntityID, lAttributeID, szEName, szAName ) );

            case qATTRIBUTEQUALIFIER:
               return( fnGenAttrAttr( vSubtask, vRightView, vLeftView, vTgt, vOperatorView,
                                      lViewID, lEntityID, lAttributeID, szVName, szEName, szAName ) );

            default:
               break;
         }

         break;

      case qOPERATIONCALL:
         // set next expr element will tell us what to do.
         if ( sRightStruct.lDataType == qTDECIMAL )
            lTemp = -3;
         else
            lTemp = 0;

         CreateViewFromViewForTask( &vSave, vLeftView, 0 );
         lTemp = ExtractOperationCallWithView( vSubtask, vLeftView, vTgt, lTemp, NULL );
         SetViewFromView( vLeftView, vSave );
         DropView( vSave );

         // Overwrite the operation call ExprNode with the return variable
         // CreateEntity( vLeftView, "ExprNode", zPOS_BEFORE );
         SetAttributeFromInteger( vLeftView, "ExprNode", "ElementClass", qVARIABLE );
         SetAttributeFromInteger( vLeftView, "ExprNode", "DataClass", qNUMERIC );
         SetAttributeFromInteger( vLeftView, "ExprNode", "DataType", qTINTEGER );
         SetAttributeFromInteger( vLeftView, "ExprNode", "Offset", lTemp );
         SetAttributeFromInteger( vLeftView, "ExprNode", "Level", lStartLevel );
      // sLeftStruct.lStructSize = sizeof( sQExprStruct );
      // lRC = GetStructFromEntityAttrs( (zPCHAR) &sLeftStruct, vLeftView, "ExprNode" );
         lRC = GetExpressionStruct( &sLeftStruct, vLeftView, "ExprNode" );

         switch( sRightStruct.lElementClass )
         {
            case qOPERATIONCALL:
               fnGenOpCallOpCall( vSubtask, vRightView, vTgt, lStartLevel );
               //no break
            case qVARIABLE:
            case qCONSTANT:
            case qDEFINE:
               fnGenAnyVar( vSubtask, vRightView, vLeftView, vTgt, vOperatorView );
               return( 0 ); // put out as is

            case qATTRIBUTEQUALIFIER:
               return( fnGenOpCallAttr( vSubtask, vRightView, vLeftView, vTgt, vOperatorView,
                                        szVName, szEName, szAName ) );
            default:
               break;
         }

         break;

      default:
         break;
   }
   return( 1 );
}

// GenerateVariable ? OperationCall
zVOID
fnGenVarOpCall( zVIEW  vSubtask,
                zVIEW vRightView,
                zVIEW vLeftView,
                zVIEW vTgt,
                zLONG lStartLevel )
{
   zLONG lTemp;
   zVIEW vSave;

   // extract
   if ( sOperatorStruct.lOffset == qEQUAL )
      lTemp = sLeftStruct.lOffset;
   else
      lTemp = 0;

   CreateViewFromViewForTask( &vSave, vRightView, 0 );
   lTemp = ExtractOperationCallWithView( vSubtask, vRightView, vTgt, lTemp, &sLeftStruct );
   SetViewFromView( vRightView, vSave );
   DropView( vSave );

   // Overwrite the operation call ExprNode with the return variable
   // CreateEntity( vRightView, "ExprNode", zPOS_BEFORE );
   SetAttributeFromInteger( vRightView, "ExprNode", "ElementClass", qVARIABLE );
   SetAttributeFromInteger( vRightView, "ExprNode", "DataClass", qNUMERIC );
   if ( sRightStruct.lDataType == qTDECIMAL)
      SetAttributeFromInteger( vRightView, "ExprNode", "DataType", qTDECIMAL );
   else
      SetAttributeFromInteger( vRightView, "ExprNode", "DataType", qTINTEGER );

   SetAttributeFromInteger( vRightView, "ExprNode", "Offset", lTemp );
   SetAttributeFromInteger( vRightView, "ExprNode", "Level", lStartLevel );
}

// GenerateAnything ? Variable|Constant|Define
zVOID
fnGenAnyVar( zVIEW vSubtask,
             zVIEW vRightView,
             zVIEW vLeftView,
             zVIEW vTgt,
             zVIEW vOperatorView )
{

   if ( vLeftView )
      fnCopyExprNode( vTgt, vLeftView, zPOS_AFTER, FALSE );

   if ( vRightView && CheckExistenceOfEntity( vRightView, "ExprNode" ) >= zCURSOR_SET )
   {
      fnCopyExprNode( vTgt, vRightView, zPOS_AFTER, FALSE );
   }
   else
   {
      // so it is positioned
      SetCursorLastEntity( vTgt, "ExprNode", "" );
   }

   if ( vOperatorView && CheckExistenceOfEntity( vOperatorView, "ExprNode" ) >= zCURSOR_SET )
   {
      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
   }
}

// GenerateVariable ? Attribute
zSHORT
fnGenVarAttr( zVIEW vSubtask, zVIEW vRightView, zVIEW vLeftView,
              zVIEW vTgt, zVIEW vOperatorView )
{
   zLONG   lTempID;
   zLONG   lViewID;
   zLONG   lEntityID;
   zLONG   lAttributeID;
   zLONG   lReturnLthID;
   zCHAR   szOperName[ 33 ];
   zCHAR   szVName[ 33 ];
   zCHAR   szEName[ 33 ];
   zCHAR   szAName[ 33 ];


   AtomizeAQ( sRightStruct.szText, szVName, szEName, szAName,
              &lViewID, &lEntityID, &lAttributeID, sRightStruct.lDataClass );

   // see if if is an assignment or a condition.
   if ( sOperatorStruct.lOffset == qEQUAL )
   {
      strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      if ( sLeftStruct.lDataType == qTSTRING && sLeftStruct.lElementClass == qVARIABLE )
      {
         if ( g_szGenLang[ 0 ] == 'J' )
            lReturnLthID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         else
            lReturnLthID = 0;

         strcpy_s( szOperName, zsizeof( szOperName ), "GetVariableFromAttribute" );
         AddGetVariableFromAttribute( vSubtask, szOperName, lViewID, lReturnLthID, 0,
                                      sLeftStruct.lDataType, sLeftStruct.lOffset,
                                      szEName, lEntityID, szAName, lAttributeID, 0, "", 0 );
         SetCursorPrevEntity( vLeftView, "ExprNode", "" );
         return( 1 );
      }
      else
      if ( sLeftStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );

      // On 16.05.1998 DonC changed the sRightStruct parm below to sLeftStruct.
      // This was to make the following VML statements work:
      //    dTest = VIP.VIP.NAME
      //    iTest = VIP.VIP.NAME
      // where dTest was decimal, iTest was integer and VIP.VIP.NAME was a string
      // attribute containing a string that was a valid integer.
      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0,
                                      sLeftStruct.lDataType, sLeftStruct.lOffset,
                                      szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
      SetCursorPrevEntity( vLeftView, "ExprNode", "" );
      return( 1 );
   }
   else
   if ( IsCondOperator( sOperatorStruct.lOffset ) == 0 )
   {
      // We need to do a compare. Create the compare operation and put out the
      // opposite operator and 0.
      if ( sLeftStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToDecimal" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToInteger" );

      // On 31.07.1998 DonC changed the following function call to use the sLeftStruct.lDataType
      // instead of the sRightStruct.lDataType because in a statement such as VIP.VIP.NUM = lNumber,
      // the generated call is SetAttributeFromInteger and the data type of the variable is
      // integer (the left data struct).
      CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, 1,
                                  sLeftStruct.lDataType, sLeftStruct.lOffset,
                                  szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );
      sOperatorStruct.lOffset = ReverseOperator( sOperatorStruct.lOffset );

      InsertExprNodeValuesToEntity( vTgt, qOPERATOR, qNUMERIC, qTINTEGER,
                                    sOperatorStruct.lOffset, 0, "0", 0, zPOS_AFTER );
      if ( vLeftView != 0 )
         SetCursorPrevEntity( vLeftView, "ExprNode", "" );

      return( 0 );
   }
   else // is something like 6 v.e.a + ...
   {
      if ( vLeftView != 0 )
      {
         fnCopyExprNode( vTgt, vLeftView, zPOS_AFTER, FALSE );
      }

      if ( sRightStruct.lDataType == qTDECIMAL )
      {
         lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      }
      else
      {
         lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      }

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0,
                                      sRightStruct.lDataType, lTempID, szEName,
                                      lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC, sRightStruct.lDataType,
                                    lTempID, 0, "", 0, zPOS_AFTER );
      lGlobalRightID = lTempID;
      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
      return( 0 );
   }
}

// GenerateConstant|Define ? OperationCall
zVOID
fnGenConstOpCall( zVIEW  vSubtask,
                  zVIEW vRightView, zVIEW vLeftView,
                  zVIEW vTgt, zLONG lStartLevel )
{
   zLONG lTemp = 0;
   zVIEW vSave;

   // Extract.
   CreateViewFromViewForTask( &vSave, vRightView, 0 );
   lTemp = ExtractOperationCallWithView( vSubtask, vRightView, vTgt, lTemp, NULL );
   SetViewFromView( vRightView, vSave );
   DropView( vSave );

   // Overwrite the operation call ExprNode with the return variable.
   SetAttributeFromInteger( vRightView, "ExprNode", "ElementClass", qVARIABLE );
   SetAttributeFromInteger( vRightView, "ExprNode", "DataClass", qNUMERIC );
   SetAttributeFromInteger( vRightView, "ExprNode", "DataType", qTINTEGER );
   SetAttributeFromInteger( vRightView, "ExprNode", "Offset", lTemp );
   SetAttributeFromInteger( vRightView, "ExprNode", "Level", lStartLevel );
}

// GenerateConstant|Define ? Attribute
zSHORT
fnGenConstAttr( zVIEW  vSubtask,
                zVIEW vRightView, zVIEW vLeftView,
                zVIEW vTgt, zVIEW vOperatorView )
{
   zLONG lTempID;
   zLONG lViewID, lEntityID, lAttributeID;
   zCHAR szOperName[ 33 ];
   zCHAR   szVName[ 33 ];
   zCHAR   szEName[ 33 ];
   zCHAR   szAName[ 33 ];

   AtomizeAQ( sRightStruct.szText, szVName, szEName, szAName,
              &lViewID, &lEntityID, &lAttributeID, sRightStruct.lDataClass );


   if ( IsCondOperator( sOperatorStruct.lOffset ) == 0 )
   {
      if ( sLeftStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToDecimal" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToInteger" );

      if ( sLeftStruct.lElementClass == qDEFINE )
         CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1,
                                     sRightStruct.lDataType, sLeftStruct.lOffset,
                                     szEName, lEntityID, szAName, lAttributeID,
                                     sLeftStruct.lDataType, sLeftStruct.szText,
                                     sLeftStruct.lOffset, sLeftStruct.lElementClass );
      else
         // qCONSTANT
         CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1,
                                     sRightStruct.lDataType, sLeftStruct.lOffset,
                                     szEName, lEntityID, szAName, lAttributeID,
                                     sLeftStruct.lDataType, sLeftStruct.szText, 0,
                                     sLeftStruct.lElementClass );

      InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );

      if ( sOperatorStruct.lOffset != qCONDEQUAL && sOperatorStruct.lOffset != qNOTEQUAL )
         sOperatorStruct.lOffset = ReverseOperator( sOperatorStruct.lOffset );

      InsertExprNodeValuesToEntity( vTgt, qOPERATOR, qNUMERIC, qTINTEGER,
                                    sOperatorStruct.lOffset, 0, "0", 0, zPOS_AFTER );
      return( 0 );
   }
   else // is something like 6 v.e.a + ...
   {
      if ( vLeftView != 0 )
         fnCopyExprNode( vTgt, vLeftView, zPOS_AFTER, FALSE );

      if ( sRightStruct.lDataType == qTDECIMAL )
      {
         lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      }
      else
      {
         lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      }

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0, sRightStruct.lDataType,
                                      lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC,
                                    sRightStruct.lDataType, lTempID, 0, "", 0, zPOS_AFTER );
      lGlobalRightID = lTempID;

      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
      return( 0 );
   }
}

// GenerateAttribute ? OperationCall
zVOID
fnGenAttrOpCall( zVIEW  vSubtask,
                 zVIEW vRightView, zVIEW vLeftView,
                 zVIEW vTgt, zLONG lStartLevel )
{
   zLONG lTemp = 0;
   zVIEW vSave;

   if ( sRightStruct.lDataType == qTDECIMAL )
      lTemp = -3; // force a decimal to be created

   // extract
   CreateViewFromViewForTask( &vSave, vRightView, 0 );
   lTemp = ExtractOperationCallWithView( vSubtask, vRightView, vTgt, lTemp, NULL );
   SetViewFromView( vRightView, vSave );
   DropView( vSave );

   // Overwrite the operation call ExprNode with the return variable.
// CreateEntity( vRightView, "ExprNode", zPOS_BEFORE );
   SetAttributeFromInteger( vRightView, "ExprNode", "ElementClass", qVARIABLE );
   SetAttributeFromInteger( vRightView, "ExprNode", "DataClass", qNUMERIC );
   SetAttributeFromInteger( vRightView, "ExprNode", "Offset", lTemp );
   SetAttributeFromInteger( vRightView, "ExprNode", "Level", lStartLevel );

   // Reset right.
// sRightStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sRightStruct, vRightView, "ExprNode" );
   GetExpressionStruct( &sRightStruct, vRightView, "ExprNode" );
}

// GenerateAttribute ? Variable|Constant|Define
zSHORT
fnGenAttrVar( zVIEW  vSubtask,
              zVIEW vRightView, zVIEW vLeftView,
              zVIEW vTgt, zVIEW vOperatorView,
              zLONG lViewID, zLONG lEntityID, zLONG lAttributeID,
              zPCHAR szEName, zPCHAR szAName )
{
   zLONG lTempID;
   zCHAR szOperName[ 33 ];

   SetCursorPrevEntity( vLeftView, "ExprNode", "" );
   if ( sOperatorStruct.lOffset == qEQUAL && vRightView == 0 )
   {
      // This is an assignment to an attribute with the source (right) being an "intermediate"
      // expression (right=0). We want to generate a temp variable with data type of the target
      // attribute.
      zLONG lID;

      if ( sLeftStruct.lDataType == qTDECIMAL )
         lID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
      else
         lID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC,
                                    sLeftStruct.lDataType, lID, 0, "", 0, zPOS_AFTER );

      SetCursorLastEntity( vTgt, "ExprNode", "" );

      InsertExprNodeValuesToEntity( vTgt, qOPERATOR, qNUMERIC, qTINTEGER, qEQUAL, 0, "0", 0, zPOS_AFTER );

      AddStatementEntityToPI( vSubtask, qEXPRESSION, qqlineno, vTgt, zPOS_AFTER );

      if ( sLeftStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromDecimal" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromInteger" );

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 1, sLeftStruct.lDataType,
                                      lID, szEName, lEntityID, szAName,lAttributeID, 0, "", 0, 0 );
      return( 1 );
   }

   if ( sOperatorStruct.lOffset == qEQUAL )
   {
      if ( sRightStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromDecimal" );
      else
      if ( sRightStruct.lDataType == qTSTRING )
         strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromString" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromInteger" );

      if ( sRightStruct.lElementClass == qVARIABLE )
      {
         AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 1,
           /* HH 25.9.98 */              sRightStruct.lDataType, sRightStruct.lOffset,
                                         szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
      }
      else
      if (  sRightStruct.lElementClass == qDEFINE )
      {
         AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, -1, 0, 0, szEName,
                                         lEntityID, szAName, lAttributeID, sRightStruct.lDataType,
                                         sRightStruct.szText, sRightStruct.lOffset, qDEFINE );
      }
      else
      if (  sRightStruct.lElementClass == qCONSTANT )
      {
         AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, -1, 0, 0, szEName,
                                         lEntityID, szAName, lAttributeID,
                                         sRightStruct.lDataType, sRightStruct.szText, 0, qCONSTANT );
      }

      return( 1 );
   }
   else
   if ( IsCondOperator( sOperatorStruct.lOffset ) == 0 )
   {
      if ( sRightStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToDecimal" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToInteger" );

      if ( sRightStruct.lElementClass == qVARIABLE )
         CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, 1,
                                     sRightStruct.lDataType, sRightStruct.lOffset,
                                     szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
      else
      if ( sRightStruct.lElementClass == qDEFINE )
         CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1, 0, 0,
                                     szEName, lEntityID, szAName, lAttributeID,
                                     sRightStruct.lDataType, sRightStruct.szText,
                                     sRightStruct.lOffset, sRightStruct.lElementClass );
      else
         CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1, 0, 0,
                                     szEName, lEntityID, szAName, lAttributeID,
          /* HH 13.7.99 */           sRightStruct.lDataType, sRightStruct.szText,
                                     0, sRightStruct.lElementClass );

      // Do not need the opposite operator here.
      InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );
      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
      return( 0 );
   }
   else
   {
      if ( sLeftStruct.lDataType == qTDECIMAL )
      {
         lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      }
      else
      {
         lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      }

      if ( lGlobalTempID == -1 )
         lGlobalTempID = lTempID;

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0, sLeftStruct.lDataType,
                                      lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC,
                                    sLeftStruct.lDataType, lTempID, 0, "", 0, zPOS_AFTER );
      // 9/24
      if ( vRightView != 0 )
         fnCopyExprNode( vTgt, vRightView, zPOS_AFTER, FALSE );
      else // is == 0
         SetCursorLastEntity( vTgt, "ExprNode", "" );
      // end 9/24

      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
      return( 0 );
   }
}

// GenerateAttribute ? Attribute
zSHORT
fnGenAttrAttr( zVIEW  vSubtask,
               zVIEW vRightView, zVIEW vLeftView,
               zVIEW vTgt, zVIEW vOperatorView,
               zLONG lViewID, zLONG lEntityID, zLONG lAttributeID,
               zPCHAR szVName, zPCHAR szEName, zPCHAR szAName )
{
   zLONG lTempID;
   zCHAR szOperName[ 33 ];

   // Use this to create a SetAttributeFromAttribute.
   if ( sOperatorStruct.lOffset == qEQUAL ||
        IsCondOperator( sOperatorStruct.lOffset ) == 0 )
   {
      if ( sOperatorStruct.lOffset == qEQUAL )
      {
         strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromAttribute" );
         AddCompareToView( vSubtask, vTgt, szOperName, &sLeftStruct, sLeftStruct.szText,
                           &sRightStruct, sRightStruct.szText, 0 );
         SetCursorPrevEntity( vLeftView, "ExprNode", "" );
      }
      else
      {
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToAttribute" );
         AddCompareToView( vSubtask, vTgt, szOperName, &sLeftStruct, sLeftStruct.szText,
                           &sRightStruct, sRightStruct.szText, 0 );
         InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );
         fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
         SetCursorPrevEntity( vLeftView, "ExprNode", "" );
      }
      return( 0 );
   }
   else
   {
      if ( sLeftStruct.lDataType == qTDECIMAL )
      {
         lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      }
      else
      {
         lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      }

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0, sLeftStruct.lDataType,
                                      lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC, sLeftStruct.lDataType, lTempID, 0, "", 0, zPOS_AFTER );
      if ( lGlobalTempID == -1 )
         lGlobalTempID = lTempID;

      ////
      AtomizeAQ( sRightStruct.szText, szVName, szEName, szAName,
                 &lViewID, &lEntityID, &lAttributeID, sRightStruct.lDataClass );

      if ( sRightStruct.lDataType == qTDECIMAL )
      {
         lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      }
      else
      {
         lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      }

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0, sRightStruct.lDataType,
                                      lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC, sRightStruct.lDataType, lTempID, 0, "", 0, zPOS_AFTER );
      // 9/17/93
      lGlobalRightID = lTempID;
      ////
      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
      return( 0 );
   }
}

// GenerateOperationCall ? OperationCall
zVOID
fnGenOpCallOpCall( zVIEW vSubtask,
                   zVIEW vRightView, zVIEW vTgt,
                   zLONG lStartLevel )
{
   zVIEW vSave;
   zLONG lTemp = 0;

   CreateViewFromViewForTask( &vSave, vRightView, 0 );
   lTemp = ExtractOperationCallWithView( vSubtask, vRightView, vTgt, 0, NULL );
   SetViewFromView( vRightView, vSave );
   DropView( vSave );

   // Overwrite the operation call ExprNode with the return variable.
// CreateEntity( vRightView, "ExprNode", zPOS_BEFORE );
   SetAttributeFromInteger( vRightView, "ExprNode", "ElementClass", qVARIABLE );
   SetAttributeFromInteger( vRightView, "ExprNode", "DataClass", qNUMERIC );
   SetAttributeFromInteger( vRightView, "ExprNode", "DataType", qTINTEGER );
   SetAttributeFromInteger( vRightView, "ExprNode", "Offset", lTemp );
   SetAttributeFromInteger( vRightView, "ExprNode", "Level", lStartLevel );
}

// GenerateOperationcall ? Attribute
zSHORT
fnGenOpCallAttr( zVIEW  vSubtask,
                 zVIEW vRightView, zVIEW vLeftView,
                 zVIEW vTgt, zVIEW vOperatorView,
                 zPCHAR szVName, zPCHAR szEName, zPCHAR szAName )
{
   zLONG lTempID = 0;
   zLONG lViewID, lEntityID, lAttributeID;
   zCHAR szOperName[ 33 ];

   AtomizeAQ( sRightStruct.szText, szVName, szEName, szAName,
              &lViewID, &lEntityID, &lAttributeID, sRightStruct.lDataClass );

   if ( IsCondOperator( sOperatorStruct.lOffset ) == 0 )
   {
      // We need to do a compare. Create the compare operation and put out the opposite
      // operator and 0.
      if ( sRightStruct.lDataType == qTDECIMAL )
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToDecimal" );
      else
         strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToInteger" );

      CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, 1,
                                  sRightStruct.lDataType, sLeftStruct.lOffset,
                                  szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );

      if ( sOperatorStruct.lOffset != qCONDEQUAL && sOperatorStruct.lOffset != qNOTEQUAL )
         sOperatorStruct.lOffset = ReverseOperator( sOperatorStruct.lOffset );

      InsertExprNodeValuesToEntity( vTgt, qOPERATOR, qNUMERIC, qTINTEGER,
                                    sOperatorStruct.lOffset, 0, "0", 0, zPOS_AFTER );

      SetCursorPrevEntity( vLeftView, "ExprNode", "" );
      return( 0 );
   }
   else // is something like 6 v.e.a + ...
   {
      fnCopyExprNode( vTgt, vLeftView, zPOS_AFTER, FALSE );
      if ( sRightStruct.lDataType == qTDECIMAL )
      {
         lTempID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetDecimalFromAttribute" );
      }
      else
      {
         lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
         strcpy_s( szOperName, zsizeof( szOperName ), "GetIntegerFromAttribute" );
      }

      AddGetOrSetAttributeWithString( vSubtask, szOperName, lViewID, 0, sRightStruct.lDataType,
                                      lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC, sRightStruct.lDataType, lTempID, 0, "", 0, zPOS_AFTER );

      fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
      return( 0 );
   }
}


zOPER_EXPORT zLONG OPERATION
OptimizeSimpleTextExpression( zVIEW  vSubtask, zVIEW  vTgt, zVIEW  vSrc,
                              zLONG  lAnalyzeResult, zLONG  lStartLevel )
{
   zVIEW vLeftView;
   zVIEW vRightView;
   zVIEW vOperatorView;
   zLONG lTargetElementClass;
   zLONG lTargetID;
   zLONG lRightIndexed;
   zLONG lRightElementClass;
   zLONG lMaxLevel;
   zLONG lLth;
   zLONG lRC;

   //SetNameForView( vSrc, "EXPRESSION", vSubtask, zLEVEL_TASK );
   //STOP
   // assume we re positioned correctly.
   CreateViewFromViewForTask( &vLeftView, vSrc, 0 );
// sGExprStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSrc, "ExprNode" );
   GetExpressionStruct( &sGExprStruct, vSrc, "ExprNode" );
   lTargetElementClass = sGExprStruct.lElementClass;
   lTargetID = sGExprStruct.lOffset;

   // go to the next expr value by level as it may not be the immediate next level this will skip the [] combo as they have weighted priorities.
   lMaxLevel = lStartLevel + 10;
   while ( SetEntityCursor( vSrc, "ExprNode", "Level", zPOS_NEXT | zQUAL_INTEGER | zQUAL_LESS_THAN, &lMaxLevel, "", "", 0, "", 0 ) >= zCURSOR_SET )
   {
   // sGExprStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSrc, "ExprNode" );
      GetExpressionStruct( &sGExprStruct, vSrc, "ExprNode" );
      if ( sGExprStruct.lElementClass == qOPERATOR && sGExprStruct.lOffset == qRBRACE )
      {
         SetCursorNextEntityByInteger( vSrc, "ExprNode", "Level", lStartLevel, "" );
      // sGExprStruct.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSrc, "ExprNode" );
         GetExpressionStruct( &sGExprStruct, vSrc, "ExprNode" );
         break;

      }

      if ( sGExprStruct.lElementClass == qOPERATOR && sGExprStruct.lOffset == qLBRACE )
         continue;

      break;
   }

   lRightElementClass = sGExprStruct.lElementClass;

   // if we are optimizing by concatenation then we need to skip past this expression entry as it is the same as the left (i.e. a = a + b)
   if ( lAnalyzeResult == qOPTIMIZEBYCONCATENATION )
   {
      DeleteEntity( vSrc, "ExprNode", zREPOS_NONE );

      // if we are concatenating skip the second one and go to the last
      while ( SetEntityCursor( vSrc, "ExprNode", "Level", zPOS_NEXT | zQUAL_INTEGER | zQUAL_LESS_THAN, &lMaxLevel, "", "", 0, "", 0 ) >= zCURSOR_SET )
      {
      // sGExprStruct.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSrc, "ExprNode" );
         GetExpressionStruct( &sGExprStruct, vSrc, "ExprNode" );
         if ( sGExprStruct.lElementClass == qOPERATOR && sGExprStruct.lOffset == qRBRACE )
         {
            DeleteEntity( vSrc, "ExprNode", zREPOS_NONE );
            break;
         }

         if ( sGExprStruct.lElementClass == qOPERATOR &&
            sGExprStruct.lOffset == qLBRACE )
         {
            DeleteEntity( vSrc, "ExprNode", zREPOS_NONE );
            continue;
         }

         break;
      }
   }

   CreateViewFromViewForTask( &vRightView, vSrc, 0 );
   lRightIndexed = 0;
   // goto operator

   while ( SetEntityCursor( vSrc, "ExprNode", "Level", zPOS_NEXT | zQUAL_INTEGER | zQUAL_LESS_THAN, &lMaxLevel, "", "", 0, "", 0 ) >= zCURSOR_SET )
   {
   // sGExprStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sGExprStruct, vSrc, "ExprNode" );
      GetExpressionStruct( &sGExprStruct, vSrc, "ExprNode" );
      if ( sGExprStruct.lElementClass == qOPERATOR && sGExprStruct.lOffset == qRBRACE )
      {
         SetCursorNextEntityByInteger( vSrc, "ExprNode", "Level", lStartLevel, "" );
         break;
      }

      if ( sGExprStruct.lElementClass == qOPERATOR && sGExprStruct.lOffset == qLBRACE )
      {
         lRightIndexed = 1;
         continue;
      }

      break;
   }

   SetCursorLastEntity( vSrc, "ExprNode", "" );

   if ( lAnalyzeResult == qOPTIMIZEBYCONCATENATION )
      DeleteEntity( vSrc, "ExprNode", zREPOS_PREV );

   lRC = CreateViewFromViewForTask( &vOperatorView, vSrc, 0 );

   if ( (lAnalyzeResult == qSIMPLEOPTIMIZELEFTINDEXED && lRightElementClass == qATTRIBUTEQUALIFIER) ||
        (lTargetElementClass == qATTRIBUTEQUALIFIER && lRightIndexed == 1) )
   {
      if ( lTargetElementClass == qVARIABLE )
         lLth = GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), lTargetID );
      else
         lLth = -1;

      CreateViewFromViewForTask( &vGlobalView, vSrc, 0 );

      lGlobalTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );

      // add right to temp
      fnOptTextExpr( vSubtask, vTgt, 0, vRightView, vOperatorView, lMaxLevel - 10 );
      lGlobalRightID = lGlobalTempID;

      // copy back to left
      fnOptTextExpr( vSubtask, vTgt, vLeftView, 0, 0, lMaxLevel - 10 );

      if ( lAnalyzeResult == qOPTIMIZEBYCONCATENATION )
      {
         DeleteEntity( vRightView, "ExprNode", zREPOS_NONE );
      } // was below
   }
   else
      fnOptTextExpr( vSubtask, vTgt, vLeftView, vRightView, vOperatorView, lMaxLevel - 10 );

   return( lRC );
}

// >>>fnOptTextExpr
zLONG
fnOptTextExpr( zVIEW  vSubtask,
               zVIEW  vTgt,
               zVIEW  vLeftView,
               zVIEW  vRightView,
               zVIEW  vOperatorView,
               zLONG  lStartLevel )
{
   zCHAR   szVName[ 33 ];
   zCHAR   szEName[ 33 ];
   zCHAR   szAName[ 33 ];
   zCHAR   szOperName[ 33 ];
   zCHAR   szTempNumber[ 10 ];
   zLONG   lViewID;
   zLONG   lEntityID;
   zLONG   lAttributeID;
   zLONG   lTempID;
   zLONG   lReturnLthID;
   zLONG   lLth;
   zLONG   lRC;

   // get the operator information (e.g. "a b +" the + is the operator)
   if ( vOperatorView )
   {
   // sOperatorStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sOperatorStruct, vOperatorView, "ExprNode" );
      GetExpressionStruct( &sOperatorStruct, vOperatorView, "ExprNode" );
   }
   else
      sOperatorStruct.lOffset = qEQUAL;

   // get the target information. (e.g. "a b +" the a is the left)
   if ( vLeftView )
   {
   // sLeftStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sLeftStruct, vLeftView, "ExprNode" );
      GetExpressionStruct( &sLeftStruct, vLeftView, "ExprNode" );
   }
   else
   {
      // If the left view was zero, then we are sitting on an operator.  In this case what is going on is that we are going to "operate" into
      // the id stored in global so just set the left struct from this value.
      if ( sOperatorStruct.lOffset == qEQUAL )
      {
         sLeftStruct.lElementClass = qVARIABLE;

         // the global id
         // was sLeftStruct.lOffset = lGlobalRightID;
         sLeftStruct.lOffset = lGlobalTempID;
      }
      else
      {
         // set type as variable
         sLeftStruct.lElementClass = qVARIABLE;

         // the global id
         sLeftStruct.lOffset = lGlobalTempID; //JEFFJEFF

         // 2/18/sLeftStruct.lOffset = lGlobalRightID;
         // the global length just so we ain't gotta get it all the time.
         strcpy_s( szTempNumber, zsizeof( szTempNumber ), szGlobalTempNumber );
      }
   }

   // get the right view (e.g. "a b +" the b is the right)
   if ( vRightView != 0 )
   {
   // sRightStruct.lStructSize = sizeof( sQExprStruct );
   // GetStructFromEntityAttrs( (zPCHAR) &sRightStruct, vRightView, "ExprNode" );
      GetExpressionStruct( &sRightStruct, vRightView, "ExprNode" );
   }
   else  // if the right was a zero, then we need to handle this case
   {
      // If the operator is equal and the left and right are the same variable, then we are done so delete the entities.
      // We are done because the left has been set by a series of concats.  If a temp is used then the oper would not be equal
      if ( sOperatorStruct.lOffset == qEQUAL && sLeftStruct.lOffset == lGlobalTempID // && // was
         ) // lGlobalRightID == -1 )
      {
         return( 1 );
      }
      else
      if ( (sOperatorStruct.lOffset == qEQUAL && sLeftStruct.lElementClass == qVARIABLE) ||
           (sOperatorStruct.lOffset == qEQUAL && sLeftStruct.lElementClass != qVARIABLE && lGlobalTempID != -1) )
      {
         sRightStruct.lElementClass = qVARIABLE;
         sRightStruct.lOffset = lGlobalTempID;
      }
      else
      {
         sRightStruct.lElementClass = qVARIABLE;
         sRightStruct.lOffset = lGlobalRightID;
      }
   }

   lTempID = -1;

   // do different logic based on the element classes of the expr entries
   switch( sLeftStruct.lElementClass )
   {
      // if the left is a variable
      case qVARIABLE:
      case qCONSTANT:
      case qDEFINE:
         // see what the right is.
         switch( sRightStruct.lElementClass )
         {
            // if the right was an attribute qualifier, then we need to break up the aq.
            case qATTRIBUTEQUALIFIER:
               AtomizeAQ( sRightStruct.szText, szVName, szEName, szAName,
                          &lViewID, &lEntityID, &lAttributeID, sRightStruct.lDataClass );

               // if the operator is a + then it is a stringcat ... we therefore need to do a retrieve
               if ( sOperatorStruct.lOffset == qADDITION )
               {
                  // GETLENGTHFORENTITYATTRS...
                  lLth = GetLengthFromAttribute( lViewID, szEName, szAName );
                  if ( lLth == -1 )
                     lLth = 1024;

                  lTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
                  lLth = GetVariableLength( szTempNumber, zsizeof( szTempNumber ), lTempID );
                  if ( g_szGenLang[ 0 ] == 'J' )
                     lReturnLthID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
                  else
                     lReturnLthID = 0;

                  // if the target is indexed we need to work with a temp
                  strcpy_s( szOperName, zsizeof( szOperName ), "GetVariableFromAttribute" );

                  // add the get string. If it was left indexed, then lTempID will hold the temp generated to hold the left. If it was
                  // concatenate then lTempID will hold the temp for the GetfromAQ. either way we want to get into lTempid
                  AddGetVariableFromAttribute( vSubtask, szOperName, lViewID, lReturnLthID, 0, sRightStruct.lDataType, lTempID,
                                               szEName, lEntityID, szAName, lAttributeID, 0, "", 0 );
                  // break omitted here
               }
               else
               {
                 // break;
               }

            // if the right was any of...
            case qVARIABLE:
            case qCONSTANT:
            case qDEFINE:
               // if the operator is a + then it is a string cat
               //if ( sOperatorStruct.lOffset == qEQUAL ||
               //     sOperatorStruct.lOffset == qADDITION )
               // Insert new bottom of stack.
               AddOperatorToView( vSubtask, 0, -2 );

               if ( sOperatorStruct.lOffset == qADDITION )  // Generated ZeidonStringCopy/ZeidonStringConcat
               {
                  // If the global is is not the same as the Left and we are doing a concatenation,
                  // we first need to string copy the left into the global, then concatenate the
                  // right onto the global. For example, if we have a = b + c + a, we have already
                  // created a global t that will be used to hold b+c+a.  This is the b c + part
                  // where we need to copy b into t then concatenate c onto t.
                  if ( lGlobalTempID != sLeftStruct.lOffset )
                  {
                     // Insert new bottom of stack.
                     AddOperatorToView( vSubtask, 0, -2 );

                     // copy into the global
                     CreateTargetZeidonStrOpSWithID( vSubtask, lGlobalTempID, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, "ZeidonStringCopy" );

                     // 9/15/?? was commented out, don't know why
                     ///
                     if ( vLeftView )
                     {
                        // copy the left into it
                        CreateSourceZeidonStrOpS( vSubtask, vLeftView, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, szGlobalTempNumber, zsizeof( szGlobalTempNumber ) );
                     }
                     else
                     {
                        CreateSourceZeidonStrOpSWithID( vSubtask, sLeftStruct.lOffset, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, szGlobalTempNumber, zsizeof( szGlobalTempNumber ) );
                     }

                     // Finally add the statement from the stack.
                     AddOperatorToView( vSubtask, 0, -1 );
                     AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, zPOS_AFTER );
                  }

                  // Now regardless of what is in global, we are going to concat to it.
                  // GlobalID is either a global variable generated as a temp or is the left itself.
                  CreateTargetZeidonStrOpWithID( vSubtask, lGlobalTempID, vTgt, "ZeidonStringConcat" );

                  GetVariableLength( szGlobalTempNumber, zsizeof( szGlobalTempNumber ), lGlobalTempID );

                  // concatenate the right element (the c in b c +)
                  if ( sRightStruct.lElementClass == qATTRIBUTEQUALIFIER )
                  {
                     CreateSourceZeidonStrOpWithID( vSubtask, lTempID, vTgt, szGlobalTempNumber, zsizeof( szGlobalTempNumber ) );
                  }
                  else
                  {
                     if ( vRightView != 0 )
                        CreateSourceZeidonStrOp( vSubtask, vRightView, vTgt, szGlobalTempNumber, zsizeof( szGlobalTempNumber ) );
                     else
                        CreateSourceZeidonStrOpWithID( vSubtask, sRightStruct.lOffset, vTgt, szGlobalTempNumber, zsizeof( szGlobalTempNumber ) );
                  }
               }
               // else it is not a +. Since we are dealing exclusively with strings in this operation,
               // the only possibilities are an = (as in a = b) or some conditional operator.
               else
               {
                  if ( sLeftStruct.lElementClass == qVARIABLE )
                     lLth = GetVariableLength( szTempNumber, zsizeof( szTempNumber ), sLeftStruct.lOffset );
                  else
                  {
                     lLth = zstrlen( sLeftStruct.szText );
                     zltoa( lLth, szTempNumber, zsizeof( szTempNumber ) );
                  }

                  if ( sRightStruct.lElementClass != qATTRIBUTEQUALIFIER )
                  {
                     // if it is an equal we need to do a string copy
                     if ( sOperatorStruct.lOffset == qEQUAL )
                        strcpy_s( szOperName, zsizeof( szOperName ), "ZeidonStringCopy" );
                     else // is cond operator
                        strcpy_s( szOperName, zsizeof( szOperName ), "ZeidonStringCompare" );

                     if ( vLeftView != 0 )       //
                     {
                        // copy the left into it //

                        // What operation gets created here depends on the
                        // data type of the source value.
                        // If it is string, we will create ZeidonStringC...
                        // If it is integer, we will create ....
                        // If it is decimal, we will create ....

                        if ( sRightStruct.lDataType == qTSTRING || sRightStruct.lDataType == qTCHARACTER )
                        {
                           // Because CreateTargetZeidonStrOp can issue a syntax error, we will return on a -1 return code.
                           lRC = CreateTargetZeidonStrOp( vSubtask, vLeftView, vTgt, szOperName );
                           if ( lRC == -1 )
                              return( -1 );
                        }

                        if ( sRightStruct.lDataType == qTINTEGER )
                        {
                           CreateConvertZeidonStrOp( vSubtask, vLeftView, vTgt, 'I', szTempNumber );
                        }

                        if ( sRightStruct.lDataType == qTDECIMAL )
                        {
                           CreateConvertZeidonStrOp( vSubtask, vLeftView, vTgt, 'D', szTempNumber );
                        }
                     }
                     else //JSB                 //
                        CreateTargetZeidonStrOpWithID( vSubtask, sLeftStruct.lOffset, vTgt, szOperName );

                     //if ( vLeftView != 0 )
                     //   DeleteEntity( vLeftView, "ExprNode", zPOS_AFTER );

                     if ( sRightStruct.lDataType == qTSTRING || sRightStruct.lDataType == qTCHARACTER )
                     {
                        // if the right view exists,
                        if ( vRightView != 0 )
                        {
                           CreateSourceZeidonStrOp( vSubtask, vRightView, vTgt, szTempNumber, zsizeof( szTempNumber ) );
                        }
                        // else, avoid errors by just using the id
                        else
                        {
                           CreateSourceZeidonStrOpWithID( vSubtask, sRightStruct.lOffset, vTgt, szTempNumber, zsizeof( szTempNumber ) );
                        }
                     }

                     if ( sOperatorStruct.lOffset != qEQUAL )
                     {
                        AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );

                        if ( sOperatorStruct.lOffset != qCONDEQUAL && sOperatorStruct.lOffset != qNOTEQUAL )
                           sOperatorStruct.lOffset = ReverseOperator( sOperatorStruct.lOffset );

                        AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, "0", sOperatorStruct.lOffset, 0 );

                        // Dump the expression from the stack.
                        AddOperatorToView( vSubtask, vTgt, -1 ); // ???????????????????
                     }
                  }
                  else
                  {
                     if ( sOperatorStruct.lOffset == qEQUAL )
                     {
                        if ( g_szGenLang[ 0 ] == 'J' )
                           lReturnLthID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
                        else
                           lReturnLthID = 0;

                        strcpy_s( szOperName, zsizeof( szOperName ), "GetVariableFromAttribute" );
                        AddGetVariableFromAttribute( vSubtask, szOperName, lViewID, lReturnLthID, 0, sLeftStruct.lDataType, sLeftStruct.lOffset,
                                                     szEName, lEntityID, szAName, lAttributeID, 0, "", 0 );
                        break;
                     }
                     else // is cond operator
                     {
                        strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToString" );
                        if ( sLeftStruct.lElementClass == qVARIABLE )
                        {
                           // If the sRightStruct.lDataType value is 0, then the type is unknown and we
                           // will force it to be string.
                           if ( sRightStruct.lDataType == 0 )
                              CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, 1, qTSTRING, sLeftStruct.lOffset, szEName, lEntityID, szAName,
                                                          lAttributeID, 0, "", 0, 0 );
                           else
                              CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, 1, sRightStruct.lDataType, sLeftStruct.lOffset,
                                                          szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
                        }
                        else
                        {
                           CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1, sRightStruct.lDataType, sLeftStruct.lOffset,
                                                       szEName, lEntityID, szAName, lAttributeID, sLeftStruct.lDataType, sLeftStruct.szText, 0, sLeftStruct.lElementClass );
                        }

                        if ( sOperatorStruct.lOffset != qCONDEQUAL && sOperatorStruct.lOffset != qNOTEQUAL )
                           sOperatorStruct.lOffset = ReverseOperator( sOperatorStruct.lOffset );

                        AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, "0", sOperatorStruct.lOffset, 0 );
                        AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );

                        // Dump the expression from the stack.
                        AddOperatorToView( vSubtask, vTgt, -1 );
                        return( 0 );
                     }
                  }
               }

               // Finally add the statement
               if ( sOperatorStruct.lOffset == qEQUAL || sOperatorStruct.lOffset == qADDITION )
               {
                  // Dump the expression from the stack.
                  //AddOperatorToView( vSubtask, vTgt, -1 ); // was 0 not targetvw
                  AddOperatorToView( vSubtask, 0, -1 ); // was 0 not targetvw
                  AddStatementEntityToPI( vSubtask, qEXPRESSION, qqlineno, vTgt, zPOS_AFTER );
               }

               // If this was a condition, we need to add it to the expression not output it as
               // a statement.

               break;

            default:
               break;
         }

         break;
      //
      case qATTRIBUTEQUALIFIER:
         AtomizeAQ( sLeftStruct.szText, szVName, szEName, szAName, &lViewID, &lEntityID, &lAttributeID, sLeftStruct.lDataClass );

         // if the operator is a '+' then it is a string cat
         if ( sOperatorStruct.lOffset == qADDITION )
         {
            if ( lEntityID == 0 && lAttributeID == 0 )
               lLth = GetLengthFromAttribute( lViewID, szEName, szAName );
            else
               lLth = -1;

            if ( lLth == -1 )
               lLth = 1024;
/////////// test
            if ( lGlobalTempID != -1 && lStartLevel < 10 )
               lTempID = lGlobalTempID;
            else
               lTempID = GenerateTempVariable( vSubtask, qTSTRING, lLth );

/////////// end test

            // add a get then a string concat
            AddGetOrSetAttribute( vSubtask, "GetStringFromAttribute", lViewID, 0, sLeftStruct.lDataType, lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
            if ( lGlobalTempID == -1 )
               lGlobalTempID = lTempID;
         }

         // see what the right is.
         switch( sRightStruct.lElementClass )
         {
            case qCONSTANT:
            case qDEFINE:
            case qVARIABLE:
               // If the right is one of these and the AnalyzeCode was concatenation then we need
               // to create a Zeidon StringConcat and a SetAttributeFromString. The target of the
               // concat and the var to be set from is the var created above.
               if ( sOperatorStruct.lOffset == qADDITION )
               {
                  CreateTargetZeidonStrOpSWithID( vSubtask, lTempID, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, "ZeidonStringConcat" );
                  GetVariableLength( szTempNumber, zsizeof( szTempNumber ), lTempID );
                  CreateSourceZeidonStrOpS( vSubtask, vRightView, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, szTempNumber, zsizeof( szTempNumber ) );

                  // Dump the statement from the stack.
                  AddOperatorToView( vSubtask, 0, -1 );
                  AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, zPOS_AFTER );
                  lGlobalRightID = lTempID;
                  break;
               }
               else
                  lTempID = sRightStruct.lOffset;

               if ( IsCondOperator( sOperatorStruct.lOffset ) == 0 )
               {
                  // We need to do a compare. Create the compare operation and put out
                  // the operator and 0
                  strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToString" );

                  if ( sRightStruct.lElementClass == qCONSTANT )
                  {
                     CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1, sRightStruct.lDataType, sLeftStruct.lOffset, szEName, lEntityID,
                                                 szAName, lAttributeID, sRightStruct.lDataType, sRightStruct.szText, 0, sRightStruct.lElementClass );
                  }
                  else
                  if ( sRightStruct.lElementClass == qDEFINE )
                  {
                     CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, -1, sRightStruct.lDataType, sLeftStruct.lOffset, szEName, lEntityID,
                                                 szAName, lAttributeID, sRightStruct.lDataType, sRightStruct.szText, sRightStruct.lOffset, sRightStruct.lElementClass );
                  }
                  else
                  {
                     CreateOperationCallUsingAQ( vSubtask, vTgt, szOperName, lViewID, 1, sRightStruct.lDataType, sRightStruct.lOffset, szEName, lEntityID,
                                                 szAName, lAttributeID, 0, "", 0, 0 );
                  }

                  InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );
                  InsertExprNodeValuesToEntity( vTgt, qOPERATOR, qNUMERIC, qTINTEGER, sOperatorStruct.lOffset, 0, "0", 0, zPOS_AFTER );
                  break;
               }
               else
               {
                  if ( sRightStruct.lElementClass == qCONSTANT || sRightStruct.lElementClass == qDEFINE )
                  {
                     if ( sRightStruct.lDataType == qTINTEGER )
                        AddGetOrSetAttribute( vSubtask, "SetAttributeFromInteger", lViewID, -1, qTINTEGER, lTempID, szEName, lEntityID, szAName, lAttributeID,
                                              sRightStruct.lDataType, sRightStruct.szText, sRightStruct.lOffset, sRightStruct.lElementClass );
                     else
                     if ( sRightStruct.lDataType == qTDECIMAL )
                        AddGetOrSetAttribute( vSubtask, "SetAttributeFromDecimal", lViewID, -1, qTDECIMAL, lTempID, szEName, lEntityID, szAName, lAttributeID,
                                              sRightStruct.lDataType, sRightStruct.szText, sRightStruct.lOffset, sRightStruct.lElementClass );
                     else
                        AddGetOrSetAttribute( vSubtask, "SetAttributeFromString", lViewID, -1, qTSTRING, lTempID, szEName, lEntityID, szAName, lAttributeID,
                                              sRightStruct.lDataType, sRightStruct.szText, sRightStruct.lOffset, sRightStruct.lElementClass );
                  }
                  else
                  {
                     // It is possible, that the right side of the assignment is NOT a string.  We want to handle that. **HH**
                     if ( sRightStruct.lDataType == qTINTEGER )
                     {
                        AddGetOrSetAttribute( vSubtask, "SetAttributeFromInteger", lViewID, 1, qTINTEGER, lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
                     }
                     else
                     if ( sRightStruct.lDataType == qTDECIMAL )
                     {
                        AddGetOrSetAttribute( vSubtask, "SetAttributeFromDecimal", lViewID, 1, qTDECIMAL, lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
                     }
                     else
                     {
                        AddGetOrSetAttribute( vSubtask, "SetAttributeFromString", lViewID, 1, qTSTRING, lTempID, szEName, lEntityID, szAName, lAttributeID, 0, "", 0, 0 );
                     }
                  }
               }

               break;

            case qATTRIBUTEQUALIFIER:
               if ( sOperatorStruct.lOffset == qADDITION )
               {
                  zLONG   lViewIDR;
                  zLONG   lTempIDR;
                  zLONG   lEntityIDR;
                  zLONG   lAttributeIDR;
                  zCHAR   szViewNameR[ 33 ];
                  zCHAR   szEntityNameR[ 33 ];
                  zCHAR   szAttributeNameR[ 33 ];

                  AtomizeAQ( sRightStruct.szText, szViewNameR, szEntityNameR, szAttributeNameR, &lViewIDR, &lEntityIDR, &lAttributeIDR, sRightStruct.lDataClass );
                  if ( lEntityIDR == 0 && lAttributeIDR == 0 )
                     lLth = GetLengthFromAttribute( lViewIDR, szEntityNameR, szAttributeNameR );
                  else
                     lLth = -1;

                  if ( lLth == -1 )
                     lLth = 1024;

                  lTempIDR = GenerateTempVariable( vSubtask, qTSTRING, lLth );

                  // itoa would be quicker
/*  DonC change 1999.06.11   Old code used lTempIDR for length, which
    was for source variable. New code uses lTempID for length, which is
    for target variable.
                  lLth = GetVariableLength( szTempNumber, zsizeof( szTempNumber ), lTempIDR );
*/
                  lLth = GetVariableLength( szTempNumber, zsizeof( szTempNumber ), lTempID );

                  // add a get then a string concat
                  AddGetOrSetAttribute( vSubtask, "GetStringFromAttribute", lViewIDR, 0, sRightStruct.lDataType, lTempIDR, szEntityNameR, lEntityIDR,
                                        szAttributeNameR, lAttributeIDR, 0, "", 0, 0 );
                  CreateTargetZeidonStrOpSWithID( vSubtask, lTempID, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, "ZeidonStringConcat" );
                  CreateSourceZeidonStrOpSWithID( vSubtask, lTempIDR, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, szTempNumber, zsizeof( szTempNumber ) );

                  // Dump the expression from the stack.
                  AddOperatorToView( vSubtask, 0, -1 );
                  AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno, szLocal7Code, &lLocal7Code, szLocal7Text, &lLocal7Text, zPOS_AFTER );
                  lGlobalRightID = lTempIDR;
                  break;
               }
               else
               {
                  // use this the create a SetAttributeFromAttribute
                  if ( sOperatorStruct.lOffset == qEQUAL || IsCondOperator( sOperatorStruct.lOffset ) == 0 )
                  {
                     if ( sOperatorStruct.lOffset == qEQUAL )
                     {
                        strcpy_s( szOperName, zsizeof( szOperName ), "SetAttributeFromAttribute" );
                        AddCompareToView( vSubtask, vTgt, szOperName, &sLeftStruct, sLeftStruct.szText, &sRightStruct, sRightStruct.szText, 0 );
                     }
                     else
                     {
                        strcpy_s( szOperName, zsizeof( szOperName ), "CompareAttributeToAttribute" );
                        AddCompareToView( vSubtask, vTgt, szOperName, &sLeftStruct, sLeftStruct.szText, &sRightStruct, sRightStruct.szText, 0 );
                        InsertExprNodeValuesToEntity( vTgt, qCONSTANT, qNUMERIC, qTINTEGER, 0, 0, "0", 0, zPOS_AFTER );

                        fnCopyExprNode( vTgt, vOperatorView, zPOS_AFTER, FALSE );
                        SetCursorPrevEntity( vLeftView, "ExprNode", "" );
                     }

                     return( 0 );
                  }
               }

               break;

            default:
               break;
         }

         break;

      default:
         break;

   }// end switch

   if ( IsCondOperator( sOperatorStruct.lOffset ) == 0 )
      return( 0 ); // could be on a cond

   return( 1 );
}

// >>>AtomizeAQ
zOPER_EXPORT zLONG OPERATION
AtomizeAQ( zPCHAR  szAQ,
           zPCHAR  pchViewName,
           zPCHAR  pchEntityName,
           zPCHAR  pchAttributeName,
           zPLONG  plViewID,
           zPLONG  plEntityID,
           zPLONG  plAttributeID,
           zLONG   lDataClass )
{
   // split the parts out
   SplitQualifier( szAQ, pchViewName, pchEntityName, pchAttributeName );
   GetVariableID( plViewID, pchViewName );
   *plEntityID = 0;
   *plAttributeID = 0;

   if ( lDataClass == qENTITYISUNKNOWN || lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN || lDataClass == 0 )
   {
      // is unknown i.e a var
      GetVariableID( plEntityID, pchEntityName );
      GetVariableID( plAttributeID, pchAttributeName );
   }
   else
   if ( lDataClass == qATTRIBUTEISUNKNOWN )
   {
      GetVariableID( plAttributeID, pchAttributeName );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateSourceZeidonStrOp
/////////////////////////////////////////////////////////////////
zLONG
CreateSourceZeidonStrOp( zVIEW  vSubtask,
                         zVIEW  vSrc,
                         zVIEW  vTgt,
                         zPCHAR pchTempNumber,
                         zLONG  lMaxLth )
{
   zLONG lOffset;
   zLONG lElementClass;
   zLONG lWork;
   zLONG lRC;

   // Set the source string.
   fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
   SetCursorNextEntity( vSrc, "ExprNode", 0 );

   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );
   GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
   GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

   // If we immediately get a lbrace, then the string was indexed.
   if ( lElementClass == qOPERATOR && lOffset == qLBRACE )
   {
      SetCursorNextEntity( vSrc, "ExprNode", "" );
      GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
      GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

      // Get the offset expression (i.e. just copy it from one expr to next).
      while ( !(lElementClass == qOPERATOR && (lOffset == qCOLON || lOffset == qRBRACE)) )
      {
         fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
         lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );
         if ( lRC <= zCURSOR_UNCHANGED )
            break;

         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

      }

      // Add COMMA.
      AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // if we ended on an rbrace use the default, else we must have ended on a colon
      if ( lElementClass == qOPERATOR && lOffset == qRBRACE )
      {
         // Use default of 0.
         AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
      }
      else
      {
         SetCursorNextEntity( vSrc, "ExprNode", 0 );

         // Get the next entry.
         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

         // Get the length expression (i.e. just copy it from one expr to next).
         while ( !(lElementClass == qOPERATOR && lOffset == qRBRACE) )
         {
            fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
            lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );

            if ( lRC <= zCURSOR_UNCHANGED )
               break;

            GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
         }
      }
   }
   else // use both defaults cause there is no indexing
   {
      // Add default offset of 1 ... if no offset default to 1.
      AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

      // Add COMMA.
      AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // Use default of 0.
      AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
   }

   // Add COMMA.
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add the Max Target String Length parm.  We will add 1 to the length because the length passed must be the C length, not the VML length.
   lWork = zatol( pchTempNumber );
   lWork++;
   zltoa( lWork, pchTempNumber, lMaxLth );
   AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, pchTempNumber, 0, 0 );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, ")", qCLOSEOPER, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateSourceZeidonStrOpS
/////////////////////////////////////////////////////////////////

zLONG
CreateSourceZeidonStrOpS( zVIEW  vSubtask,
                          zVIEW  vSrc,
                          zPCHAR pchLocalCode,
                          zPLONG plLocalCode,
                          zPCHAR pchLocalText,
                          zPLONG plLocalText,
                          zPCHAR pchTempNumber,
                          zLONG  lMaxLth )
{
   sQExprStruct sExprStruct = { 0 };
   zLONG lOffset;
   zLONG lElementClass;
   zLONG lWork;
   zLONG lRC;

   // Set the source string.
// sExprStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vSrc, "ExprNode" );
   GetExpressionStruct( &sExprStruct, vSrc, "ExprNode" );
   ADDEXPRESSIONENTRYTOSTRING( sExprStruct.lElementClass, sExprStruct.lDataClass, sExprStruct.lDataType, sExprStruct.szText, sExprStruct.lOffset, 0 );

   SetCursorNextEntity( vSrc, "ExprNode", 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
   GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

   // If we immediately get an lbrace, then the string was indexed.
   if ( lElementClass == qOPERATOR && lOffset == qLBRACE )
   {
      SetCursorNextEntity( vSrc, "ExprNode", 0 );
      //lRC = SetCursorNextEntity( vSrc, "ExprNode", "" );

      GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
      GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

      // Get the offset expression (i.e. just copy it from one expr to next).
      while ( !(lElementClass == qOPERATOR && (lOffset == qCOLON || lOffset == qRBRACE)) )
      {
      // sExprStruct.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vSrc, "ExprNode" );
         GetExpressionStruct( &sExprStruct, vSrc, "ExprNode" );
         ADDEXPRESSIONENTRYTOSTRING( sExprStruct.lElementClass, sExprStruct.lDataClass, sExprStruct.lDataType, sExprStruct.szText, sExprStruct.lOffset, 0 );

         lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );
         if ( lRC <= zCURSOR_UNCHANGED )
            break;

         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
      }

      // Add COMMA.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // If we ended on an rbrace use the default, else we must have ended on a colon.
      if ( lElementClass == qOPERATOR && lOffset == qRBRACE )
      {
         // Use default of 0.
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
         SetCursorNextEntity( vSrc, "ExprNode", 0 );
      }
      else
      {
         SetCursorNextEntity( vSrc, "ExprNode", 0 );

         // Get the next entry.
         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

         // Get the length expression (i.e. just copy it from one expr to next).
         while ( !(lElementClass == qOPERATOR && lOffset == qRBRACE) )
         {
         // sExprStruct.lStructSize = sizeof( sQExprStruct );
         // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vSrc, "ExprNode" );
            GetExpressionStruct( &sExprStruct, vSrc, "ExprNode" );
            ADDEXPRESSIONENTRYTOSTRING( sExprStruct.lElementClass, sExprStruct.lDataClass, sExprStruct.lDataType, sExprStruct.szText, sExprStruct.lOffset, 0 );

            SetCursorNextEntity( vSrc, "ExprNode", 0 );
            if ( lRC <= zCURSOR_UNCHANGED )
               break;

            GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
         }

         SetCursorNextEntity( vSrc, "ExprNode", 0 );
      }
   }
   else // use both defaults cause there is no indexing
   {
      // Add default offset of 1 ... if no offset default to 1
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

      // Add COMMA.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // Use default of 0.
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
   }

   // Add COMMA.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add the Max Target String Length parm.  We will add 1 to the length because the length passed must be the C length, not the VML length.
   lWork = zatol( pchTempNumber );
   lWork++;
   zltoa( lWork, pchTempNumber, lMaxLth );
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, pchTempNumber, 0, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, ")", qCLOSEOPER, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateSourceZeidonStrOpWithID
/////////////////////////////////////////////////////////////////
zLONG
CreateSourceZeidonStrOpWithID( zVIEW  vSubtask,
                               zLONG  lID,
                               zVIEW  vTgt,
                               zPCHAR pchTempNumber,
                               zLONG  lMaxLth )
{
   zLONG lWork;

   // Set the source string.
// if ( g_szGenLang[ 0 ] == 'J' )
      AddExprNodeEntityToView( vSubtask, vTgt, qVARIABLE, 0, qTSTRING, "ID", lID, 0 );   // change for Java to not be negated
// else
//    AddExprNodeEntityToView( vSubtask, vTgt, qVARIABLE, 0, qTSTRING, "ID", -lID, 0 );  // dks 2010.06.16  don't know why this is negated

   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   // Add default offset of 1 ... if no offset default to 1.
   AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

   // Add COMMA.
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Use default of 0.
   AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );

   // Add COMMA.
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add the Max Target String Length parm.  We will add 1 to the length because the length passed must be the C length, not the VML length.
   lWork = zatol( pchTempNumber );
   lWork++;
   zltoa( lWork, pchTempNumber, lMaxLth );
   AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, pchTempNumber, 0, 0 );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, ")", qCLOSEOPER, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateSourceZeidonStrOpSWithID
/////////////////////////////////////////////////////////////////
zLONG
CreateSourceZeidonStrOpSWithID( zVIEW  vSubtask,
                                zLONG  lID,
                                zPCHAR pchLocalCode,
                                zPLONG plLocalCode,
                                zPCHAR pchLocalText,
                                zPLONG plLocalText,
                                zPCHAR pchTempNumber,
                                zLONG  lMaxLth )
{
   zLONG lWork;

   // Set the source string.
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, qTSTRING, "", lID, 0 );

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   // Add default offset of 1 ... if no offset default to 1.
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

   // Add COMMA.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Use default of 0.
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );

   // Add COMMA.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add the Max Target String Length parm.  We will add 1 to the length because the length passed must be the C length, not the VML length.
   lWork = zatol( pchTempNumber );
   lWork++;
   zltoa( lWork, pchTempNumber, lMaxLth );
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, pchTempNumber, 0, 0 );

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, ")", qCLOSEOPER, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateTargetZeidonStrOp:  ZeidonStringCopy
/////////////////////////////////////////////////////////////////
zLONG
CreateTargetZeidonStrOp( zVIEW  vSubtask,
                         zVIEW  vSrc,
                         zVIEW  vTgt,
                         zPCHAR pchOperationName )
{
   zLONG lOffset;
   zLONG lElementClass;
   zLONG lOperationZKey;
   zLONG lRC;

   // We will check to see if vTgt has valid position on Expression.
   // If it doesn't, we have some kind of invalid expression such as
   // szWork = szWork + szWork.  If we process it, we get all kinds of
   // Zeidon errors.  Thus we will generate a syntax error for this case.
   lRC = CheckExistenceOfEntity( vTgt, "Expression" );
   if ( lRC == zCURSOR_UNDEFINED )
   {
      SyntaxError( vSubtask, eQINVALIDTARGETSTRING, "" );
      return( -1 );
   }

   lOperationZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   IncludeCoreHeader( g_lpPIView, "ZDRVROPR" );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATIONCALL, qZEIDONOPERATION,
                            qTSHORT, pchOperationName, lOperationZKey, 0 );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, "(", qOPENOPER, 0 );

   //lRC = SetCursorNextEntity( vSrc, "ExprNode", "" );

   // Set the source string.
   fnCopyExprNode( vTgt, vSrc, zPOS_AFTER,
                   (zBOOL) ((g_szGenLang[ 0 ] == 'J' &&
                    (strcmp( pchOperationName, "ZeidonStringCopy" ) == 0 ||
                     strcmp( pchOperationName, "ZeidonStringConcat" ) == 0)) ? TRUE : FALSE) );  // offset needs to be negated???
   SetCursorNextEntity( vSrc, "ExprNode", 0 );

   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
   GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

   // If we immediately get an lbrace, then the string was indexed.
   if ( lElementClass == qOPERATOR && lOffset == qLBRACE )
   {
      SetCursorNextEntity( vSrc, "ExprNode", 0 );

      //lRC = SetCursorNextEntity( vSrc, "ExprNode", "" );

      GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
      GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

      // Get the offset expression (i.e. just copy it from one expr to next).
      while ( !(lElementClass == qOPERATOR && (lOffset == qCOLON || lOffset == qRBRACE)) )
      {
         fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
         lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );
         if ( lRC <= zCURSOR_UNCHANGED )
            break;

         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
      }

      // Add COMMA.
      AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // If we ended on an rbrace use the default, else we must have ended on a colon.
      if ( lElementClass == qOPERATOR && lOffset == qRBRACE )
      {
         // If we got a qRBRACE, then we have a statement of the form:
         //     szSTR[ 4 ] = "N",
         // which means we are to move a single character to the target
         // field.  To signal this the the ZeidonStringCopy routine, we
         // will set this parm to -1.
         AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "-1", 0, 0 );
         SetCursorNextEntity( vSrc, "ExprNode", 0 );
      }
      else
      {
         SetCursorNextEntity( vSrc, "ExprNode", 0 );

         // Get the next entry.
         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

         // Get the length expression (i.e. just copy it from one expr to next).
         while ( !(lElementClass == qOPERATOR && lOffset == qRBRACE) )
         {
            fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
            lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );
            if ( lRC <= zCURSOR_UNCHANGED )
               break;

            GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
         }

         SetCursorNextEntity( vSrc, "ExprNode", 0 );
      }
   }
   else // use both defaults cause there is no indexing
   {
      // Add default offset of 1 ... if no offset default to 1
      AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

      // Add COMMA.
      AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // Use default of 0.
      AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
   }

   // Add COMMA.
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateTargetZeidonStrOpS
/////////////////////////////////////////////////////////////////
zLONG
CreateTargetZeidonStrOpS( zVIEW  vSubtask,
                          zVIEW  vSrc,
                          zPCHAR pchLocalCode,
                          zPLONG plLocalCode,
                          zPCHAR pchLocalText,
                          zPLONG plLocalText,
                          zPCHAR pchOperationName )
{
   sQExprStruct sExprStruct = { 0 };
   zLONG lOffset;
   zLONG lElementClass;
   zLONG lOperationZKey;
   zLONG lRC;

   lOperationZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   IncludeCoreHeader( g_lpPIView, "ZDRVROPR" );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperationZKey, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, "(", qOPENOPER, 0 );

// sExprStruct.lStructSize = sizeof( sQExprStruct );
// GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vSrc, "ExprNode" );
   GetExpressionStruct( &sExprStruct, vSrc, "ExprNode" );
   ADDEXPRESSIONENTRYTOSTRING( sExprStruct.lElementClass, sExprStruct.lDataClass,
                               sExprStruct.lDataType, sExprStruct.szText,
                               sExprStruct.lOffset * -1, 0 );
   SetCursorNextEntity( vSrc, "ExprNode", 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
   GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

   // If we immediately get an lbrace, then the string was indexed.
   if ( lElementClass == qOPERATOR && lOffset == qLBRACE )
   {
      SetCursorNextEntity( vSrc, "ExprNode", 0 );
      GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
      GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

      // Get the offset expression (i.e. just copy it from one expr to next).
      while ( !(lElementClass == qOPERATOR && (lOffset == qCOLON || lOffset == qRBRACE)) )
      {
      // sExprStruct.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vSrc, "ExprNode" );
         GetExpressionStruct( &sExprStruct, vSrc, "ExprNode" );
         ADDEXPRESSIONENTRYTOSTRING( sExprStruct.lElementClass, sExprStruct.lDataClass,
                                     sExprStruct.lDataType, sExprStruct.szText,
                                     sExprStruct.lOffset, 0 );

         lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );
         if ( lRC <= zCURSOR_UNCHANGED )
            break;

         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
      }

      // Add COMMA.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );


      // If we ended on an rbrace use the default, else we must have ended on a colon.
      if ( lElementClass == qOPERATOR && lOffset == qRBRACE )
      {
         // Use default of 0.
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
         SetCursorNextEntity( vSrc, "ExprNode", 0 );
      }
      else
      {
         SetCursorNextEntity( vSrc, "ExprNode", 0 );

         // Get the next entry.
         GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

         // Get the length expression (i.e. just copy it from one expr to next).
         while ( !(lElementClass == qOPERATOR && lOffset == qRBRACE) )
         {
         // sExprStruct.lStructSize = sizeof( sQExprStruct );
         // GetStructFromEntityAttrs( (zPCHAR) &sExprStruct, vSrc, "ExprNode" );
            GetExpressionStruct( &sExprStruct, vSrc, "ExprNode" );
            ADDEXPRESSIONENTRYTOSTRING( sExprStruct.lElementClass, sExprStruct.lDataClass,
                                        sExprStruct.lDataType, sExprStruct.szText,
                                        sExprStruct.lOffset, 0 );
            lRC = SetCursorNextEntity( vSrc, "ExprNode", 0 );
            if ( lRC <= zCURSOR_UNCHANGED )
               break;

            GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );
         }

         SetCursorNextEntity( vSrc, "ExprNode", 0 );
      }

      lRC = SetCursorNextEntity( vSrc, "ExprNode", "" );
   }
   else // use both defaults cause there is no indexing
   {
      // Add default offset of 1 ... if no offset default to 1
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

      // Add COMMA.
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

      // Use default of 0.
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
   }

   // Add COMMA.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>CreateTargetZeidonStrOpWithID:  ZeidonStringConcat
/////////////////////////////////////////////////////////////////
zLONG
CreateTargetZeidonStrOpWithID( zVIEW  vSubtask,
                               zLONG lID,
                               zVIEW vTgt,
                               zPCHAR pchOperationName )
{
   zLONG lOperationZKey;

   lOperationZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   IncludeCoreHeader( g_lpPIView, "ZDRVROPR" );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATIONCALL, qZEIDONOPERATION,
                             qTSHORT, pchOperationName, lOperationZKey, 0 );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, "(", qOPENOPER, 0 );
   AddExprNodeEntityToView( vSubtask, vTgt, qVARIABLE, 0, qTSTRING, "ID", lID * -1, 0 );
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   // If no offset default to 1.
   AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

   // Add COMMA.
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Use default of 0.
   AddExprNodeEntityToView( vSubtask, vTgt, qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );

   // Add COMMA.
   AddExprNodeEntityToView( vSubtask, vTgt, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////
// >>>CreateTargetZeidonStrOpSWithID: Operation ZeidonStringCopy or ZeidonStringConcat
//////////////////////////////////////////////////////////////////////////////////////
zLONG
CreateTargetZeidonStrOpSWithID( zVIEW  vSubtask,
                                zLONG  lID,
                                zPCHAR pchLocalCode,
                                zPLONG plLocalCode,
                                zPCHAR pchLocalText,
                                zPLONG plLocalText,
                                zPCHAR pchOperationName )
{
   zLONG lOperationZKey;

   lOperationZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   IncludeCoreHeader( g_lpPIView, "ZDRVROPR" );

// if ( g_szGenLang[ 0 ] == 'J' )
// {
//    ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, qTSTRING, "ID", lID, 0 );
//    ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, "=", qEQUAL, 0 );
// }

   ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperationZKey, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, "(", qOPENOPER, 0 );
// if ( g_szGenLang[ 0 ] == 'J' )
//    ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, qTSTRING, "ID", lID, 0 );
// else
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, qTSTRING, "ID", lID * -1, 0 );

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, 0, qTINTEGER, ",", qCOMMA, 0 );

   // If no offset default to 1.
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "1", 1, 0 );

   // Add COMMA.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   // Use default of 0.
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qNUMERIC, qTINTEGER, "0", 0, 0 );
   // Add COMMA.
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>ExtractOperationCallWithView
/////////////////////////////////////////////////////////////////
zLONG
ExtractOperationCallWithView( zVIEW  vSubtask,
                              zVIEW  vSrc,
                              zVIEW  vTgt,
                              zLONG  lNeedTemp,
                              sQExprStruct *psReturnVariable )
{
   zLONG lViewID;
   zLONG lLth;
   zCHAR szEntityName[ 33 ];
   zLONG lEntityID;
   zCHAR szAttributeName[ 33 ];
   zLONG lAttributeID;
   zLONG lVariableID;
   zLONG lReturnVariableID;
   zCHAR szOperationName[ 33 ];
   zCHAR szText[ zMAXLINELTH ];

   zLONG lEntry;
   zLONG lOffset;
   zLONG lNextEntity;
   zLONG lDataClass;
   zLONG lDataType;
   zLONG lParamCount = 0;
   zVIEW vLocalView;

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vLocalView, g_lpExprView, 0 );

   //SetViewToSubobject( vLocalView, "Expression" );

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, vLocalView, -2 );

#if 0
   // Default data type is integer
   lDataType = qTINTEGER;
#endif

   // Set the lDataType to the operand DataType.
   GetIntegerFromAttribute( &lDataType, vSrc, "ExprNode", "DataType" );

   // put a temp
   if ( lNeedTemp == 0 )
   {
      if ( lDataType == qTINTEGER )  // long
         lReturnVariableID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
      else
      if ( lDataType == qTDECIMAL )  // long
         lReturnVariableID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
      else  // short
         lReturnVariableID = GenerateTempVariable( vSubtask, qTINTEGER, 2 );
   }
   else
   if ( lNeedTemp == -3 )
   {
      // Variable is decimal
      lReturnVariableID = GenerateTempVariable( vSubtask, qTDECIMAL, 10 );
      lDataType = qTDECIMAL;
      lNeedTemp = 0;
   }
   else
   if ( lNeedTemp == -1 )
   {
      // use nZRetCode
      if ( lDataType == qTINTEGER )
         lReturnVariableID = AddRetCodeLong( vSubtask );
      else
         lReturnVariableID = AddRetCode( vSubtask );
   }
   else
   if ( lNeedTemp == -2 )
   {
      // use nZRetCode
      lReturnVariableID = -2;
   }
   else
   {
      lReturnVariableID = lNeedTemp;
   }

   // if > 0, then return variable is explicitly defined
   // if == -2 then the return is void so dont add temp
   if ( lNeedTemp > 0 )
   {
      CreateEntity( vLocalView, "ExprNode", zPOS_AFTER );
      SetAttributeFromInteger( vLocalView, "ExprNode", "ElementClass", qVARIABLE );
      SetAttributeFromInteger( vLocalView, "ExprNode", "DataClass", psReturnVariable->lDataClass );
      SetAttributeFromInteger( vLocalView, "ExprNode", "DataType", psReturnVariable->lDataType );
      SetAttributeFromInteger( vLocalView, "ExprNode", "Offset", lReturnVariableID );
      SetAttributeFromInteger( vLocalView, "ExprNode", "Level", 0 );

      // if operation.lDataType differs from variable.lDataType, then set Flags so that
      // the C generater would generate a cast.
      SetAttributeFromInteger( vSrc, "ExprNode", "Flags", psReturnVariable->lDataType );
   }
   else
   if ( lNeedTemp > -2 )
   {
      // put in temp
      CreateEntity( vLocalView, "ExprNode", zPOS_AFTER );
      SetAttributeFromInteger( vLocalView, "ExprNode", "ElementClass", qVARIABLE );
      SetAttributeFromInteger( vLocalView, "ExprNode", "DataClass", qNUMERIC );
      SetAttributeFromInteger( vLocalView, "ExprNode", "DataType", lDataType );
      SetAttributeFromInteger( vLocalView, "ExprNode", "Offset", lReturnVariableID );
      SetAttributeFromInteger( vLocalView, "ExprNode", "Level", 0 );
   }

   // Move the operation name over.
   fnCopyExprNode( vLocalView, vSrc, zPOS_AFTER, FALSE );
   SetCursorNextEntity( vSrc, "ExprNode", 0 );

   // Move the OPENOPER over.
   fnCopyExprNode( vLocalView, vSrc, zPOS_AFTER, FALSE );
   SetCursorNextEntity( vSrc, "ExprNode", 0 );

   if ( CheckExistenceOfEntity( vSrc, "ExprNode" ) == zCURSOR_SET )
   {
      // Get the ElementClass out of the ExprEntry with out a zmemcpy.
      // since the class is the first entry in the string.
      GetIntegerFromAttribute( &lEntry, vSrc, "ExprNode", "ElementClass" );
      lOffset = 0;
      if ( lEntry == qOPERATOR )
         GetIntegerFromAttribute( &lOffset, vSrc, "ExprNode", "Offset" );

      lNextEntity = zCURSOR_SET;

      // while entries exist...
      while ( lOffset != qCLOSEOPER && lNextEntity == zCURSOR_SET )
      {
         if ( lEntry != qOPERATOR )
            lParamCount++; // current parameter index

         if ( lEntry != qATTRIBUTEQUALIFIER )
         {
            fnCopyExprNode( vLocalView, vSrc, zPOS_AFTER, FALSE );
            SetCursorNextEntity( vSrc, "ExprNode", 0 );
         }
         else
         {
            // we got an attribute parameter
            GetStringFromAttribute( szText, zsizeof( szText ), vSrc, "ExprNode", "Text" );

            // split the parts out
            SplitQualifier( szText, g_szViewName, szEntityName, szAttributeName );
            GetIntegerFromAttribute( (zPLONG) &lDataClass, vSrc, "ExprNode", "DataClass" );
            GetIntegerFromAttribute( (zPLONG) &lDataType, vSrc, "ExprNode", "DataType" );
            GetVariableID( &lViewID, g_szViewName );

            lEntityID = 0;
            lAttributeID = 0;

            // regardless of the attribute type: if the parameter type is string, we can do a
            // conversion by using "GetStringFromAttribute", as this call is always valid.
            if ( g_lParmList[ lParamCount ] == qTSTRING )
            {
               lDataType = qTSTRING;
               lDataClass = qTEXT;
            }

            if ( lDataClass == qENTITYISUNKNOWN || lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
            {
               // is unknown i.e. a var
               GetVariableID( &lEntityID, szEntityName );
               GetVariableID( &lAttributeID, szAttributeName );

            }
            else
            if ( lDataClass == qATTRIBUTEISUNKNOWN )
            {
               GetVariableID( &lAttributeID, szAttributeName );
            }

            // add the get
            if ( lDataType == qTSTRING )
            {
               lLth = GetLengthFromAttribute( lViewID, szEntityName, szAttributeName );
               if ( lLth == -1 )
                  lLth = 1024;

               lVariableID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
               strcpy_s( szOperationName, zsizeof( szOperationName ), "GetStringFromAttribute" );
            }
            else
            if ( lDataType == qTDECIMAL )
            {
               lVariableID = GenerateTempVariable( vSubtask, qTDECIMAL, 4 );
               strcpy_s( szOperationName, zsizeof( szOperationName ), "GetDecimalFromAttribute" );
            }
            else
            if ( lDataType == qTCHARACTER )
            {
               lLth = 2;
               lVariableID = GenerateTempVariable( vSubtask, qTCHARACTER, lLth );
               strcpy_s( szOperationName, zsizeof( szOperationName ), "GetStringFromAttribute" );
            }
            else
            if ( lDataType == qTDATE )
            {
               // if we got a date attribute, the argument type must be string.
               // A datetime string has a maximum of 17 chars.
               lLth = 18;
               lVariableID = GenerateTempVariable( vSubtask, qTSTRING, lLth );
               strcpy_s( szOperationName, zsizeof( szOperationName ), "GetStringFromAttribute" );

               // reset data type to character ... date is not handled correctly by the
               // interpreter or the C generator
               lDataType = qTCHARACTER;
            }
            else
            {
               lVariableID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
               strcpy_s( szOperationName, zsizeof( szOperationName ), "GetIntegerFromAttribute" );
            }

            AddGetOrSetAttributeWithString( vSubtask, szOperationName, lViewID, 0, lDataType,
                                            lVariableID, szEntityName, lEntityID, szAttributeName,
                                            lAttributeID, 0, "", 0, 0 );

            CreateEntity( vLocalView, "ExprNode", zPOS_AFTER );
            SetAttributeFromInteger( vLocalView, "ExprNode", "ElementClass", qVARIABLE );
            SetAttributeFromInteger( vLocalView, "ExprNode", "DataClass", lDataClass );
            SetAttributeFromInteger( vLocalView, "ExprNode", "DataType", lDataType );
            SetAttributeFromInteger( vLocalView, "ExprNode", "Offset", lVariableID );
            SetAttributeFromInteger( vLocalView, "ExprNode", "Level", 0 );

            lNextEntity = SetCursorNextEntity( vSrc, "ExprNode", "" );
         }

         GetIntegerFromAttribute( (zPLONG) &lEntry, vSrc, "ExprNode", "ElementClass" );
         lOffset = 0;
         if ( lEntry == qOPERATOR )
            GetIntegerFromAttribute( (zPLONG) &lOffset, vSrc, "ExprNode", "Offset" );
      }
   }

   //  move the CLOSEOPER over
   fnCopyExprNode( vLocalView, vSrc, zPOS_AFTER, FALSE );
   SetCursorNextEntity( vSrc, "ExprNode", 0 );

   // build an EQUAL
   // if == -2 then the return is void so dont add =
   if ( lNeedTemp > -2 )
   {
      CreateEntity( vLocalView, "ExprNode", zPOS_AFTER );
      SetAttributeFromInteger( vLocalView, "ExprNode", "ElementClass", qOPERATOR );
      SetAttributeFromInteger( vLocalView, "ExprNode", "DataClass", qNUMERIC );
      SetAttributeFromInteger( vLocalView, "ExprNode", "DataType", qTINTEGER );
      SetAttributeFromInteger( vLocalView, "ExprNode", "Offset", qEQUAL );
      SetAttributeFromInteger( vLocalView, "ExprNode", "Level", 0 );
   }

   SetCursorFirstEntity( vLocalView, "ExprNode", "" );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vLocalView, -1 );
   SetCursorFirstEntity( vLocalView, "ExprNode", "" );
   AddStatementEntityToPI( vSubtask, qEXPRESSION, qqlineno, vLocalView, zPOS_AFTER );
// DisplayObjectInstance( vLocalView, "Expression", 0 );
// IssueError( vLocalView, 0, 0, "ExtractOperationCallWithView" );

   DeleteEntity( vLocalView, "Expression", zREPOS_NONE );
   DropView ( vLocalView );
   return( lReturnVariableID );
}

/////////////////////////////////////////////////////////////////
// >>>AddRetCodeLong
/////////////////////////////////////////////////////////////////
zLONG
AddRetCodeLong( zVIEW vSubtask )
{
   zLONG lIDNumber;

   GetVariableID( &lIDNumber, "lZRetCode" );
   if ( lIDNumber == -1 ) // an entity does not exist; add it
   {
      lIDNumber = AddVariableEntityToPI( vSubtask, "lZRetCode", qTINTEGER, 0, 0 );
   }

   return( lIDNumber );
}

/////////////////////////////////////////////////////////////////
// >>>fnCopyExprNode
/////////////////////////////////////////////////////////////////
zSHORT
fnCopyExprNode( zVIEW  vTgt,
                zVIEW  vSrc,
                zSHORT nPos,
                zBOOL  bNegateOffset )
{
   zSHORT nRC = CreateEntity( vTgt, "ExprNode", nPos );
   if ( nRC < 0 )
      return( nRC );

   SetMatchingAttributesByName( vTgt, "ExprNode", vSrc, "ExprNode", zSET_ALL );
   if ( bNegateOffset )
   {
      zLONG lOffset;

      GetIntegerFromAttribute( &lOffset, vTgt, "ExprNode", "Offset" );
      if ( lOffset > 0 )
         SetAttributeFromInteger( vTgt, "ExprNode", "Offset", -lOffset );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////
// >>>fnAnalyzeComplexExpression
// if the result of the complex expression is of unknown data type,
//  we go through the whole expression, do determine the type of
//  the "left side " by analyzing the "right side".
//  Examples:
//  "string" + "string" should give string.
//  numeric + numeric   should give decimal.
//  "string" + numeric  should give an error.
//
// Unfortunately, when an error is caught here, the line no (qqlineno)
// is already set to the next line, so the line no in the message
// will be incorrect.
//
/////////////////////////////////////////////////////////////////
zSHORT
fnAnalyzeComplexExpression( zVIEW vSubtask,
                            zVIEW vSrc,
                            T_OPER_TABLE *pOperTable,
                            zPLONG plOperTablePos )
{
   zVIEW vExpr;
   #define MAX_STACK 200
   zLONG lDataTypeStack[ MAX_STACK ];
   zLONG lStackPointer = 0;
   zLONG lDataType = 0, nOldDataType = 0;
   zLONG lResult = 0;

   CreateViewFromViewForTask( &vExpr, vSrc, 0 );

   // We are positioned on the first ExprNode, which is the result.
   lDataType = flDataType( vExpr, lDataTypeStack, &lStackPointer, &lResult,
                           pOperTable, plOperTablePos, 0 ); // include first in Analysis
   DropView( vExpr );

   if ( lDataType == DATATYPE_ERROR )
   {
      SyntaxError( vSubtask, eQCOMPLEXEXPR, "" );
      return( -1 );
   }

   lDataType = REMOVE_HIGH( lDataType );

   if ( lResult & DATATYPE_ASSIGNMENT_BIT )
   {
      // Only if the complex expression is an assignment.
      if ( lDataType == qTSTRING && lResult & DATATYPE_LPAREN_BIT )
      {
         // The parser cannot handle string concatenation and parentheses, so give an error in that case.
         SyntaxError( vSubtask, eQSTRINGCONCATPAREN, "" );
         return( -1 );
      }

      GetIntegerFromAttribute( &nOldDataType, vSrc, "ExprNode", "DataType" );
      if ( nOldDataType == qTUNKNOWN )
      {
         // overwrite the "unknown" data type with the result of our analysis
         SetAttributeFromInteger( vSrc, "ExprNode", "DataType", lDataType );

         // if we got a string, set the data class to "text", otherwise to "numeric"
         if ( lDataType == qTSTRING )
            SetAttributeFromInteger( vSrc, "ExprNode", "DataClass", qTEXT );
         else
            SetAttributeFromInteger( vSrc, "ExprNode", "DataClass", qNUMERIC );
      }
      else
      if ( nOldDataType == qTSTRING && (lDataType == qTDECIMAL || lDataType == qTINTEGER) )
      {
         // this is an error
         //  for the moment we cannot handle the assignment of a complex numerical expression to a string
         SyntaxError( vSubtask, eQINCOMPATIBLETYPES, "" );
         return( -1 );
      }
   }

   if ( lResult & DATATYPE_CONDEXPR_BIT )
      return( 1 );
   else
      return( 0 );
}

///////////////////////////////////////////////////////////////
// >>>flDataTypeCode
//   return special code for datatype,
//          so that addition of 2 codes gives you a unique result to determine the combination.
///////////////////////////////////////////////////////////////

#define NUM_NUM       2
#define NUM_STRING    11
#define CHAR_CHAR     6
#define STRING_CHAR   13
#define STRING_STRING 20
#define STRING_DATE   110
#define DATE_DATE     200
#define BOOL_BOOL     60

zSHORT
flDataTypeCode( zLONG lDataType )
{
   zLONG lDType = REMOVE_HIGH( lDataType );
   if ( lDType == qTDECIMAL || lDType == qTINTEGER || lDType == qTSHORT )
      return( 1 );
   else
   if ( lDType == qTCHARACTER )
      return( 3 );
   else
   if ( lDType == qTSTRING )
      return( 10 );
   else
   if ( lDType == qTDATE )
      return( 100 );
   else
   if ( lDType == 0 )  // Boolean
      return( 30 );
   else
      return( 0 );
}

// Set Operator table Positon from Stack value
//  the low part of the stack value normally contains the position in the expression view.
// If it is <= 0, then we have a computed expression and we set the OTab position to the negative data type instead
zLONG
SetOPosFromStack( nStack )
{
   zLONG lRelPos;

   if ( (lRelPos = REMOVE_LOW( nStack )) > 0 )
   {
      // right side has a rel position h > 0 (high part)
      return( lRelPos );
   }
   else
   {
      // the high part is <= 0, so it is a computed expression
      // set lRightPos to the negative data type
      zLONG lType = REMOVE_HIGH( nStack );
      return( -lType );
   }
}

/////////////////////////////////////////////////////////////////
// >>>flDataType
// recursive function to analyze the datatype of a
//  complex expression.
/////////////////////////////////////////////////////////////////
zLONG
flDataType( zVIEW vExpr, zPLONG plDataTypeStack,
            zPLONG plStackPointer, zPLONG plResult,
            T_OPER_TABLE *pOperTable,
            zPLONG plOperTablePos,
            int iStepOverFirst )
{
   zLONG lDataType = 0;
   zLONG lElementClass = 0;
   zLONG lOffset = 0;
   int iFirst = iStepOverFirst;
   zLONG lRelPos;
   T_OPER_TABLE *pOTab;

   // ///////////////////////////////////////////
   // get data type of operand
   //  go down recursively if we come to left paren.
   //  loop, until if we come to a right paren or equal sign.
   // ///////////////////////////////////////////

   for ( ; ; )
   {
      if ( iFirst )
      {
         if ( SetCursorNextEntity( vExpr, "ExprNode", 0 )
               < zCURSOR_SET )
         {
            if ( *plStackPointer <= 0 )
            {
               return( DATATYPE_ERROR );
            }
            else
            {
               return( plDataTypeStack[ *plStackPointer - 1 ] );
            }
         }
      }

      iFirst = 1;

      // determine our current position in expression view
      lRelPos = GetRelativeEntityNumber( vExpr, "ExprNode", "", 0 ) + 1;

      GetIntegerFromAttribute( &lElementClass, vExpr, "ExprNode", "ElementClass" );
      GetIntegerFromAttribute( &lOffset, vExpr, "ExprNode", "Offset" );

      if ( lElementClass == qOPERATOR && lOffset == qRPAREN )
      {
         // handle the right paren
         if ( *plStackPointer <= 0 )
         {
            return( DATATYPE_ERROR );
         }
         else
         {
            return( plDataTypeStack[ *plStackPointer - 1 ] );
         }
      }

      if ( lElementClass == qOPERATOR && lOffset == qLPAREN )
      {
         // indicate left paren in expression
         *plResult |= DATATYPE_LPAREN_BIT;

         // left parenthese found, go down recursively
         lDataType = flDataType( vExpr, plDataTypeStack, plStackPointer, plResult, pOperTable, plOperTablePos, 1 ); // Step Over First
         if ( lDataType == DATATYPE_ERROR )
            return( DATATYPE_ERROR );

         // if we come back from a paren expression,
         //  the returned data type does normally not have a high part.
         // this is, because we have a "computed" result of the term inside the parens.
         // however, if there is a constant inside the parens, like (-1),
         //  then there is a high part in lDataType, which is pointing to the constant in the source view. The parser later
         //  does not like that, because parens are always "copied" to the target view. A subseqent Operator must relate
         //  to the paren expression.
         // That is the reason for removing the high part of lDataType.
         plDataTypeStack[ *plStackPointer - 1 ] = REMOVE_HIGH( lDataType );
      }
      else
      if ( lElementClass == qOPERATOR && lOffset == qLBRACE )
      {
         // we don't care for what is inside the braces.
         //  just skip everything until we get the right brace.
         //  **something to do**
         do
         {
            if ( SetCursorNextEntity( vExpr, "ExprNode", 0 ) < zCURSOR_SET )
               return( DATATYPE_ERROR );

            GetIntegerFromAttribute( &lElementClass, vExpr, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vExpr, "ExprNode", "Offset" );

         } while ( lElementClass != qOPERATOR || lOffset != qRBRACE );
      }
      else
      if ( lElementClass == qCONSTANT ||
           lElementClass == qVARIABLE ||
           lElementClass == qDEFINE   ||
           lElementClass == qATTRIBUTEQUALIFIER )
      {
         GetIntegerFromAttribute( &lDataType, vExpr, "ExprNode", "DataType" );
         if ( ++(*plStackPointer) >= MAX_STACK )
            return( DATATYPE_ERROR );

         // set the expr view positon in the higher bits of lDataType
         ADD_HIGH( lDataType, lRelPos);
         plDataTypeStack[ *plStackPointer - 1 ] = lDataType;
      }
      else
      if ( lElementClass == qOPERATIONCALL )
      {
         GetIntegerFromAttribute( &lDataType, vExpr, "ExprNode", "DataType" );
         if ( ++(*plStackPointer) >= MAX_STACK )
            return( DATATYPE_ERROR );

         // set the expr view positon in the higher bits of lDataType
         ADD_HIGH( lDataType, lRelPos);

         plDataTypeStack[ *plStackPointer - 1 ] = lDataType;

         // we found an operation call.
         //  as we are only interested in the return type,
         //  we will now go over all entries until we find qCLOSEOPER.
         do
         {
            if ( SetCursorNextEntity( vExpr, "ExprNode", 0 ) < zCURSOR_SET )
               return( DATATYPE_ERROR );

            GetIntegerFromAttribute( &lElementClass, vExpr, "ExprNode", "ElementClass" );
            GetIntegerFromAttribute( &lOffset, vExpr, "ExprNode", "Offset" );

        } while ( lElementClass != qOPERATOR || lOffset != qCLOSEOPER );
      }
      else
      if ( lElementClass == qOPERATOR )
      {
         zLONG lDTRight;
         zLONG lDTLeft;
         zSHORT lDataTypeAnalysis;

         if ( lOffset == qNOT )
         {
            // we have a unary NOT operator ... indicate, that we have a conditional expression
            *plResult |= DATATYPE_CONDEXPR_BIT;
            continue;
         }
         else
         if ( lOffset == qUNARYMINUS )
         {
            zLONG lType = plDataTypeStack[ *plStackPointer - 1 ];

            // add unary minus operator.  it does not have a left side, as it is unary
            pOTab = pOperTable + (*plOperTablePos)++;
            pOTab->lOperPos = lRelPos; // pos in view
            pOTab->lRightPos = SetOPosFromStack( lType );
            pOTab->lLeftPos = 0;  // nothing, it is unary

            // current stack value is replaced by a computed expression of same data type
            plDataTypeStack[ *plStackPointer - 1 ] = REMOVE_HIGH( lType );
            continue;
         }

         // the data type stack is an array of long
         //  each long consists of a high part (h) and a low part (l)
         // h = relative position in expression view
         // l = data type

         lDTRight = plDataTypeStack[ *plStackPointer - 1 ];
         lDTLeft = plDataTypeStack[ *plStackPointer - 2 ];

         lDataTypeAnalysis = flDataTypeCode( lDTRight ) + flDataTypeCode( lDTLeft );

         // /////////////////////////////////////////////////////////////
         // Now we got  at least 2 datatypes  and the operator between them
         // /////////////////////////////////////////////////////////////
         if ( *plStackPointer < 2 )
            return( DATATYPE_ERROR );

         // populate the oper table entry
         //  if we have a "computed" expression as the result of an evaluation, the high parts are zero. Then, there is no
         //  view argument to match that expression and the table position is set to -1.
         pOTab = pOperTable + (*plOperTablePos)++;
         if ( *plOperTablePos >= MAX_OPER_TAB )
            return( DATATYPE_ERROR );

         pOTab->lOperPos = lRelPos; // pos in view

         pOTab->lRightPos = SetOPosFromStack( lDTRight );
         pOTab->lLeftPos = SetOPosFromStack( lDTLeft );

         switch ( lOffset )
         {
            case qADDITION:
               // possible combinations are
               //  numeric + numeric   or
               //  string + string
               if ( lDataTypeAnalysis == NUM_NUM )
               {
                  // numeric + numeric = decimal
                  --(*plStackPointer);
                  plDataTypeStack[ *plStackPointer - 1 ] = qTDECIMAL;
                  break;
               }
               else
               if ( lDataTypeAnalysis == STRING_STRING ||
                    lDataTypeAnalysis == CHAR_CHAR ||
                    lDataTypeAnalysis == STRING_CHAR )
               {
                  // string + string = string
                   --(*plStackPointer);
                  plDataTypeStack[ *plStackPointer - 1 ] = qTSTRING;
                  break;
               }

               // what about Timestamp ??? ask Don
               return( DATATYPE_ERROR );

            case qMINUS:
            case qMULTIPLICATION:
            case qDIVISION:
               // other operators:
               //  only numeric (*) numeric is allowed
               if ( lDataTypeAnalysis == NUM_NUM )
               {
                  // numeric (*) numeric = decimal
                  --(*plStackPointer);
                  plDataTypeStack[ *plStackPointer - 1 ] = qTDECIMAL;
                  break;
               }

               return( DATATYPE_ERROR );

            case qCONDEQUAL:
            case qLESSTHANEQUAL:
            case qLESSTHAN:
            case qGREATERTHANEQUAL:
            case qGREATERTHAN:
            case qNOTEQUAL:
               // Comparison Operators
               // indicate, that we have a conditional expression
               *plResult |= DATATYPE_CONDEXPR_BIT;

               // numeric (<=>) numeric is ok.
               // string (<=>) string is allowed.
               //   maybe this will be restricted later, so that < and >
               //   are not allowed.
               // date (<=>) date is allowed.
               // date (<=>) string is allowed.

               if ( lDataTypeAnalysis = NUM_NUM  ||
                    lDataTypeAnalysis == STRING_STRING ||
                    lDataTypeAnalysis == DATE_DATE ||
                    lDataTypeAnalysis == STRING_DATE )
               {
                  // set "result" = 0 for "boolean" .
                  --(*plStackPointer);
                  plDataTypeStack[ *plStackPointer - 1 ] = 0;
                  break;
               }

               return( DATATYPE_ERROR );

            case qAND:
            case qOR:
               // And - Or Operators
               // indicate, that we have a conditional expression
               *plResult |= DATATYPE_CONDEXPR_BIT;

               // the 2 current items in stack must be of type Boolean

               if ( lDataTypeAnalysis = BOOL_BOOL )
               {
                 // set "result" = 0 for "boolean" .
                  --(*plStackPointer);
                  plDataTypeStack[ *plStackPointer - 1 ] = 0;
                  break;
               }

               return( DATATYPE_ERROR );

            case qEQUAL:
               // Assignment Operator
               // handle the "=" assignment
               // indicate, that this is an assignment statement
               *plResult |= DATATYPE_ASSIGNMENT_BIT;

               // as this is always the end,
               //  we just return the datatype of the assignment target
               --(*plStackPointer);
               return( plDataTypeStack[ *plStackPointer - 1 ] );

            default:
               return( DATATYPE_ERROR );

         } // end switch (lOffset)
      }
      else
      {
         // This is an error, which should not occur
         return( DATATYPE_ERROR );
      }

   }  // end for(;;)
}

// >>>CaseExpression
//  This function is called, when a CASE expression is analyzed .
//  There is only 1 expression entity in the source view,
//   which has to be an integer attribute or variable.
//  If it is an attribute, a temp variable must be generated.
zOPER_EXPORT zLONG OPERATION
CaseExpression( zVIEW vSubtask, zVIEW vTgt, zVIEW vSrc )
{
   zLONG    lDataClass, lElementClass;
   zCHAR    *pchText;
   zCHAR    szViewName[ 33 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szAttributeName[ 33 ];
   zLONG    lViewID;
   zLONG    lTempID;
   zLONG    lEntityID;
   zLONG    lAttributeID;

   SetCursorFirstEntity( vSrc, "ExprNode", "" );
   GetIntegerFromAttribute( &lElementClass, vSrc, "ExprNode", "ElementClass" );
   GetIntegerFromAttribute( &lDataClass, vSrc, "ExprNode", "DataClass" );
   GetAddrForAttribute( (zCOREMEM) &pchText, vSrc, "ExprNode", "Text" );

   if ( lElementClass == qATTRIBUTEQUALIFIER )
   {
      // Attribute - resolve to integer temp variable
      AtomizeAQ( pchText, szViewName, szEntityName, szAttributeName, &lViewID, &lEntityID, &lAttributeID, lDataClass );

      lTempID = GenerateTempVariable( vSubtask, qTINTEGER, 4 );
      AddGetOrSetAttributeWithString( vSubtask, "GetIntegerFromAttribute", lViewID, 0, qTINTEGER, lTempID,
                                      szEntityName, lEntityID, szAttributeName, lAttributeID, 0, "", 0, 0 );

      InsertExprNodeValuesToEntity( vTgt, qVARIABLE, qNUMERIC, qTINTEGER, lTempID, 0, "", 0, zPOS_AFTER );
   }
   else
   if ( lElementClass == qVARIABLE )
   {
      // Variable - just copy from source to Target
      fnCopyExprNode( vTgt, vSrc, zPOS_AFTER, FALSE );
   }
   else
   {
      // Error, we never should come here
      SyntaxError( vSubtask, eQNOVARIABLENAMEGIVEN, "" );
   }

   return( 0 );
}
