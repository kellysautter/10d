/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp3aa.c - VML SubSystem Action statement build
// DESCRIPTION:   This is the source file which contains the auxilary
//                functions that are necessary to build the Action
//                statements such as Create Entity etc...
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/07/27
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
2000.09.20 HH
   handle Set/Compare Attributes from/to DEFINE's.
2000.08.28 HH
   fix problem with operation returncodes.

2000.08.08 HH
   add flag information for "GetIntegerFromAttribute" first parameter.

2000.06.23 HH
   fix for "FOR integer VIEW.ENTITY".

2000.02.16 HH
   move ExtractOperationCallWithView to tzvmp7aa.c
   (do not delete from SourceView).

OLD CHANGE LOG

22.04.1997  DonC
   Added operation AddGetVariableFromAttribute to be used in generating
   GetVariableFromAttribute operations instead of GetStringFromAttribute.

12.07.1997  DonC
   Modified AddResultCondExprToView so that defined value was properly
   converted to a constant.

12.07.1997  DonC
   Modified operation AddGetVariableFromAttribute to pass an empty string
   instead of an integer 0 for the Context parameter.  The interpretor was
   having difficulty handling a 0 ptr for a string parameter.

16.11.1997  DonC
   Added handling of qTDATE data type in order to process date syntax errors.

07.01.1997  DonC
   Modified AddResultCondExprToView to handle define correctly.

12.05.1998  DonC
   Modified CompareTargetTypeForConsistency so that no syntax error occurs
   if a source is short and a target is long or vice versa.

16.10.1998  HH
   prototype GetOperationParmListPM changed (TB363).

10.02.1998  HH
   TB456 : generate returncode type "zLONG" according to function
   definition. see AddRetCodeLong.

16.02.1999  HH
   Error return in AddCompareToView, if GetOperationZKey returns -1. This
   might happen if a WHERE condition is invalid.

*/


/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS: LoadSourceForMeta
//             LoadSourceForMetaList
//             AddResult
//             AddRetCode
//             AddResultCondEqual
//             AddResultAndLoopControlToView
//             CreateIntegerExpression
//             AddResultCondExprToView
//             AddCompareToView
//             AddGetOrSetAttribute
//             AddGetOrSetAttributeWithString
//             CreateOperationCallUsingAQ
//             CompareDataTypesForConsistency
//             CompareTargetTypeForConsistency
//
/////////////////////////////////////////////////////////////////////////////

#define ADDEXPRESSIONENTRYTOSTRING( p1, p2, p3, p4, p5, p6 ) AddExpressionEntryToString( szLocal3Code, &lLocal3Code, szLocal3Text, &lLocal3Text, p1, p2, p3, p4, p5, p6 )

zCHAR szLocal3Code[ zMAXLINELTH ];
zCHAR szLocal3Text[ zMAXLINELTH ];
zLONG lLocal3Code;
zLONG lLocal3Text;

zOPER_EXPORT zLONG OPERATION
LoadSourceForMeta( zVIEW vSubtask, zPVIEW pvSource, zVIEW vMetaView )
{
   zLONG lRC = 0;

   lRC = CheckExistenceOfEntity( vMetaView, "SourceFile" );
   if ( lRC != 0 )
   {
      return( 1 );
   }

   // Just create a new source view from the meta view, since the meta
   // now contains the source information.
   CreateViewFromViewForTask( pvSource, vMetaView, 0 );

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
LoadSourceForMetaList( zVIEW  vSubtask,
                       zPVIEW pvSource,
                       zVIEW  vMetaListView,
                       zLONG  lMetaType )
{
   zVIEW  vMeta;
   zVIEW  vTempList;
   zLONG  lRC;

   lRC = CheckExistenceOfEntity( vMetaListView, "W_MetaDef" );
   if ( lRC == zCURSOR_UNCHANGED )
   {
      return( 1 );
   }

   // activate it from configuration management.
   // Use vTempList instead of vMetaListView for the Activate because ActivateMetaOI
   // modifies the position on the CM list for global operations.
   CreateViewFromViewForTask( &vTempList, vMetaListView, 0 );
   lRC = ActivateMetaOI( vSubtask, &vMeta, vTempList, (zSHORT) lMetaType, zSINGLE );
   DropView( vTempList );
// lRC = LoadSourceForMeta( pvSource, vMeta );
// DropMetaOI( vSubtask, vMeta );

   *pvSource = vMeta;

   if ( lRC != 0 )
      return( 1 );

   return( 0 );
}

//
// OPERATION: AddResult
//
// DESCRIPTION: AddResult adds the variable RESULT to the local operation
//    RESULT is the variable used the hold the result of Action statements
//    such as CreateEntity, SetCursor, etc. This function simply looks for
//    RESULT and adds it to the variable list if it does not already exist.
//
zOPER_EXPORT zLONG OPERATION
AddResult( zVIEW vSubtask )
{
   zLONG lIDNumber;

   GetVariableID( &lIDNumber, "RESULT" );

   if ( lIDNumber == -1 ) // an entity does not exist ... add it
   {
      lIDNumber = AddVariableEntityToPI( vSubtask, "RESULT", qTSHORT, 0, 0 );
   }

   return( lIDNumber );
}

zOPER_EXPORT zLONG OPERATION
AddRetCode( zVIEW vSubtask )
{
   zLONG lIDNumber;

   GetVariableID( &lIDNumber, "nZRetCode" );

   if ( lIDNumber == -1 ) // an entity does not exist ... add it
   {
      lIDNumber = AddVariableEntityToPI( vSubtask, "nZRetCode", qTSHORT, 0, 0 );
   }

   return( lIDNumber );
}

//
// OPERATION: AddResultCondEqual
//
//    e.g. if ( RESULT > zCURSOR_UNCHANGED )
//
zOPER_EXPORT zLONG OPERATION
AddResultCondEqual( zVIEW  vSubtask,
                    zLONG  lStatementType,
                    zPCHAR pchCompareValue )
{
   zLONG lResultID;
   zLONG lDefineZKey;
   zLONG lVarDataType;
   zVIEW vTargetView;

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, vTargetView, -2 );

   lResultID = AddResult( vSubtask );

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0,
                            qTINTEGER, "(", qLPAREN, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTSHORT, "", lResultID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ">", qGREATERTHAN, 0 );
   lDefineZKey = GetDefineZKey( vSubtask, &lVarDataType, pchCompareValue );
   AddExprNodeEntityToView( vSubtask, vTargetView, qDEFINE, qZEIDONOPERATION,
                            qTINTEGER, pchCompareValue, lDefineZKey, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0,
                            qTINTEGER, ")", qRPAREN, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vTargetView, -1 );

   AddStatementEntityToPI( vSubtask, lStatementType, qqlineno, vTargetView, zPOS_AFTER );

   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );

   return( 0 );
}

//
// OPERATION: AddResultAndLoopControl
//
// DESCRIPTION: AddResultAndLoopControl
//
zOPER_EXPORT zLONG OPERATION
AddResultAndLoopControlToView( zVIEW  vSubtask,
                               zVIEW  vTargetView,
                               zPCHAR pchCompareValue,
                               zLONG  lVariableID,
                               zLONG  lOperator,
                               zPCHAR pchConstantValue )
{
   zLONG lResultID;

   lResultID = AddResult( vSubtask );

   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTSHORT, "", lResultID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "CONDEQUAL", qCONDEQUAL, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC,
                            qTINTEGER, pchCompareValue, 0, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "AND", qAND, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTINTEGER, "", lVariableID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "", lOperator, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC,
                            qTINTEGER, pchConstantValue, 0, 0 );
   return( 0 );
}

//
// OPERATION: CreateIntegerExpression
//
// DESCRIPTION: CreateIntegerExpression
//
zOPER_EXPORT zLONG OPERATION
CreateIntegerExpression( zVIEW  vSubtask,
                         zLONG  lTargetID,
                         zLONG  lOperatorType,
                         zLONG  lEntry1Class,
                         zLONG  lEntry1Value,
                         zLONG  lOper1Value,
                         zLONG  lEntry2Class,
                         zPCHAR pchEntry2Value )
{
   zVIEW vTargetView;

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );
   //SetViewToSubobject( vTargetView, "Expression" );

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, vTargetView, -2 );

   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTINTEGER, "", lTargetID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "", lOperatorType, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, lEntry1Class, qNUMERIC,
                            qTINTEGER, "", lEntry1Value, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "", lOper1Value, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, lEntry2Class, qNUMERIC,
                            qTINTEGER, pchEntry2Value, 0, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vTargetView, -1 );

   AddStatementEntityToPI( vSubtask, qEXPRESSION, qqlineno, vTargetView, zPOS_AFTER );

   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );

   return( 0 );
}

//
// OPERATION: AddResultCondExpr
//
// DESCRIPTION: AddResultCondExpr
//
zOPER_EXPORT zLONG OPERATION
AddResultCondExprToView( zVIEW  vSubtask,
                         zVIEW  vTargetView,
                         zLONG  lOperator,
                         zPCHAR pchCompareValue )
{
   zLONG lResultID;
// zCHAR szConstant[ 31 ];
   zLONG lDefineZKey;
   zLONG lVarDataType;

   lResultID = AddResult( vSubtask );

   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTSHORT, "ResultID", lResultID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "", lOperator, 0 );

#if 0
   // Convert the Define value passed into a constant.
   SetCursorFirstEntityByString( g_lpSGView, "DefinedItem", "ExternalValue",
                                 pchCompareValue, "VML_XSource" );
   GetVariableFromAttribute( szConstant, 0, zTYPE_STRING, 30,
                             g_lpSGView, "DefinedItem", "InternalValue", 0, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT,
                            qNUMERIC, qTINTEGER, szConstant, 0, 0 );
#endif

   lDefineZKey = GetDefineZKey( vSubtask, &lVarDataType, pchCompareValue );
   AddExprNodeEntityToView( vSubtask, vTargetView, qDEFINE, qZEIDONOPERATION,
                            qTINTEGER, pchCompareValue, lDefineZKey, 0 );

// SetNameForView( vTargetView, "***TargetView", vSubtask, zLEVEL_TASK );
// IssueError( vSubtask, 0, 0, "AddResultCondExprToView" );
   return( 0 );
}

//
// OPERATION: AddCompare
//
// DESCRIPTION: AddCompare
//
zOPER_EXPORT zLONG OPERATION
AddCompareToView( zVIEW   vSubtask,
                  zVIEW   vTargetView,
                  zPCHAR  pchOperationName,
                  LPQEXPRSTRUCT sLeft,
                  zPCHAR  pchLeftLiteral,
                  LPQEXPRSTRUCT sRight,
                  zPCHAR  pchRightLiteral,
                  zLONG   lOperator )
{
   zLONG lIDNumber;
   zLONG lZKeyNumber;
   zCHAR szViewName[ 33 ];
   zLONG lViewID;
   zCHAR szEntityName[ 33 ];
   zLONG lEntityID;
   zCHAR szAttributeName[ 33 ];
   zLONG lAttributeID;

   GetOperationClass( vSubtask, &lIDNumber, pchOperationName );

   lZKeyNumber = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   if ( lZKeyNumber < 0 )
      return( -1 ); // this is an error situation

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATIONCALL, lIDNumber,
                            qTSHORT, pchOperationName, lZKeyNumber, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "(", qOPENOPER, 0 );
   SplitQualifier( pchLeftLiteral, szViewName, szEntityName, szAttributeName );

   GetVariableID( &lViewID, szViewName );
   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC, qTVIEW, "", lViewID, 0 );

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // was a variable entity name
   if ( sLeft->lDataClass == qENTITYISUNKNOWN ||
        sLeft->lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
   {
      GetVariableID( &lEntityID, szEntityName );
      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTENTITYNAME, szEntityName, lEntityID, 0 );
   }
   else // was a  constant
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTENTITYNAME, szEntityName, 0, 0 );
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );

   // was a variable attribute name
   if ( sLeft->lDataClass == qATTRIBUTEISUNKNOWN ||
        sLeft->lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
   {
      GetVariableID( &lAttributeID, szAttributeName );
      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTATTRIBUTENAME, szAttributeName, lAttributeID, 0 );
   }
   else // was a  constant
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTATTRIBUTENAME, szAttributeName, 0, 0 );
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );

   if ( sRight->lElementClass == qVARIABLE || sRight->lElementClass == qCONSTANT )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, sRight->lElementClass, sRight->lDataClass,
                               sRight->lDataType, pchRightLiteral, sRight->lOffset, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ")", qCLOSEOPER, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, "", lOperator, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC,
                               qTINTEGER, "0", 0, 0 );
      return( 0 );
   }

   SplitQualifier( pchRightLiteral, szViewName, szEntityName, szAttributeName );

   GetVariableID( &lViewID, szViewName );

   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTVIEW, "", lViewID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );

   // was a variable entity name
   if ( sRight->lDataClass == qENTITYISUNKNOWN ||
        sRight->lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
   {
      GetVariableID( &lEntityID, szEntityName );
      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTENTITYNAME, szEntityName, lEntityID, 0 );
   }
   else // was a  constant
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTENTITYNAME, szEntityName, 0, 0 );
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );

   // was a variable attribute name
   if ( sRight->lDataClass == qATTRIBUTEISUNKNOWN ||
        sRight->lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
   {
      GetVariableID( &lAttributeID, szAttributeName );
      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTATTRIBUTENAME, szAttributeName, lAttributeID, 0 );
   }
   else // was a  constant
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTATTRIBUTENAME, szAttributeName, 0, 0 );
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ")", qCLOSEOPER, 0 );
   if ( lOperator > 0 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, "", lOperator, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qNUMERIC,
                               qTINTEGER, "0", 0, 0 );
   }

   return( 0 );
}

//
// OPERATION: AddGetOrSetAttribute
//
// DESCRIPTION: AddGetOrSetAttribute creates a call to an operation
//    with the result of the operation being put in nZRetCode
//
zOPER_EXPORT zLONG OPERATION
AddGetOrSetAttribute( zVIEW  vSubtask,
                      zPCHAR pchOperationName,
                      zLONG  lViewID,
                      zLONG  lUseVariable,
                      zLONG  lVariableDataType,
                      zLONG  lVariableID,
                      zPCHAR pchEntityName,
                      zLONG  lEntityID,
                      zPCHAR pchAttributeName,
                      zLONG  lAttributeID,
                      zLONG  lUseExtra,
                      zPCHAR pchExtra,
                      zLONG  lExtra,
                      zLONG  lExtraClass )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lRCID = 0;
   zLONG lOperZKey;
   zVIEW vTargetView;

// memset( sParmListExt, 0, zsizeof( sParmListExt ) );

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );
   //SetViewToSubobject( vTargetView, "Expression" );

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, vTargetView, -2 );

   lRCID = AddRetCode( vSubtask );
   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );

   // add result
   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTSHORT, "RCID", lRCID, 0 );

   // add EQUAL
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "=", qEQUAL, 0 );

   // add operation name
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATIONCALL, qZEIDONOPERATION,
                            qTSHORT, pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "(", qOPENOPER, 0 );

   if ( lUseVariable == 0 )
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, 0,
                               lVariableDataType, "", lVariableID, sParmListExt[ lParmListIndex ] );
      lParmListIndex++;
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ",", qCOMMA, 0 );
   }

   if ( sParmList[ lParmListIndex] < 0 )
      lViewID *= -1;

   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTVIEW, "", lViewID, sParmListExt[ lParmListIndex ] );
   lParmListIndex++;
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );

   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lEntityID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT, qTENTITYNAME,
                               "", lEntityID, sParmListExt[ lParmListIndex ] );
      lParmListIndex++;
   }
   else  // is a string constant
   {
      lParmListIndex++;
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTENTITYNAME, pchEntityName, lEntityID, 0 );
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );
   if ( lAttributeID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex++ ] < 0 )
         lAttributeID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTATTRIBUTENAME, "", lAttributeID, sParmListExt[ lParmListIndex ] );
      lParmListIndex++;
   }
   else  // is a string constant
   {
      lParmListIndex++;
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTATTRIBUTENAME, pchAttributeName, lAttributeID, 0 );
   }

   if ( lUseVariable == 1 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ",", qCOMMA, 0 );
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, 0,
                               lVariableDataType, "", lVariableID, sParmListExt[ lParmListIndex ] );
      lParmListIndex++;
   }

   if ( lUseExtra > 0 ) // use the extra parameter
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ",", qCOMMA, 0 );
      if ( lExtraClass == qDEFINE )
      {
         AddExprNodeEntityToView( vSubtask, vTargetView, qDEFINE, 0,
                                  lUseExtra, pchExtra, lExtra, 0 );
      }
      else
      {
         AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, 0,
                                  lUseExtra, pchExtra, lExtra, 0 );
      }
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ")", qCLOSEOPER, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vTargetView, -1 );

   AddStatementEntityToPI( vSubtask, qEXPRESSION, qqlineno, vTargetView, zPOS_AFTER );

   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
AddGetOrSetAttributeWithString( zVIEW  vSubtask,
                                zPCHAR pchOperationName,
                                zLONG  lViewID,
                                zLONG  lUseVariable,
                                zLONG  lVariableDataType,
                                zLONG  lVariableID,
                                zPCHAR pchEntityName,
                                zLONG  lEntityID,
                                zPCHAR pchAttributeName,
                                zLONG  lAttributeID,
                                zLONG  lUseExtra,
                                zPCHAR pchExtra,
                                zLONG  lExtra,
                                zLONG  lExtraClass )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lRCID = 0;
   zLONG lOperZKey;

// memset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal3Code = 0;
   lLocal3Text = 0;

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );
   lRCID = AddRetCode( vSubtask );
   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );

   // add result
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RC", lRCID, 0 );

   // add EQUAL
   // add operation name
   ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

   if ( lUseVariable == 0 )
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, lVariableDataType, "", lVariableID, sParmListExt[ lParmListIndex++ ] );
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   }

   if ( sParmList[ lParmListIndex ] < 0 )
      lViewID *= -1;

   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTVIEW, "", lViewID, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   lParmListIndex++;
   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lEntityID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, qTENTITYNAME, "", lEntityID, 0 );
   }
   else  // is a string constant
   {
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTENTITYNAME, pchEntityName, lEntityID, 0 );
   }

   lParmListIndex++;
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
   if ( lAttributeID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lAttributeID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, qTATTRIBUTENAME, "", lAttributeID, 0 );
   }
   else  // is a string constant
   {
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTATTRIBUTENAME, pchAttributeName, lAttributeID, 0 );
   }

   lParmListIndex++;

   if ( lUseVariable == 1 )
   {
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, lVariableDataType, "", lVariableID, 0 );

      lParmListIndex++;
   }

   if ( lUseExtra > 0 ) // use the extra parameter
   {
      ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );
      if ( lExtraClass == qDEFINE )
      {
         // lExtra = nOffset for a DEFINE
         ADDEXPRESSIONENTRYTOSTRING( qDEFINE,   // ElementClass
                                     0,         // data class
                                     lUseExtra, // data type
                                     pchExtra,   // text
                                     lExtra,    // offset
                                     0 );
      }
      else
      {
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, 0, lUseExtra, pchExtra, lExtra, 0 );
      }
   }

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );

   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal3Code, &lLocal3Code, szLocal3Text, &lLocal3Text, zPOS_AFTER );
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
CreateOperationCallUsingAQ( zVIEW  vSubtask,
                            zVIEW  vTargetView,
                            zPCHAR pchOperationName,
                            zLONG  lViewID,
                            zLONG  lUseVariable,
                            zLONG  lVariableDataType,
                            zLONG  lVariableID,
                            zPCHAR pchEntityName,
                            zLONG  lEntityID,
                            zPCHAR pchAttributeName,
                            zLONG  lAttributeID,
                            zLONG  lUseExtra,
                            zPCHAR pchExtra,
                            zLONG  lExtra,
                            zLONG  lExtraClass )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lOperZKey;

// memset( sParmListExt, 0, zsizeof( sParmListExt ) );

   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );

   // add operation name
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATIONCALL, qZEIDONOPERATION,
                            qTSHORT, pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   lParmListIndex = 1;

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, "(", qOPENOPER, 0 );

   if ( lUseVariable == 0 )
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, 0,
                               lVariableDataType, "", lVariableID, sParmListExt[ lParmListIndex ] );
      lParmListIndex++;
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ",", qCOMMA, 0 );
   }

   if ( sParmList[ lParmListIndex ] < 0 )
      lViewID *= -1;

   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                            qTVIEW, "", lViewID, sParmListExt[ lParmListIndex ] );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );

   lParmListIndex++;
   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lEntityID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTENTITYNAME, "", lEntityID, sParmListExt[ lParmListIndex ] );
   }
   else  // is a string constant
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTENTITYNAME, pchEntityName, lEntityID, 0 );
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ",", qCOMMA, 0 );
   lParmListIndex++;
   if ( lAttributeID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lAttributeID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qTEXT,
                               qTATTRIBUTENAME, "", lAttributeID, sParmListExt[ lParmListIndex ] );
   }
   else  // is a string constant
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, qTEXT,
                               qTATTRIBUTENAME, pchAttributeName, lAttributeID, 0 );
   }

   lParmListIndex++;
   if ( lUseVariable == 1 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ",", qCOMMA, 0 );
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, 0,
                               lVariableDataType, "", lVariableID, sParmListExt[ lParmListIndex ] );
   }

   if ( lUseExtra > 0 ) // use the extra parameter
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, ",", qCOMMA, 0 );

      if ( lExtraClass == qDEFINE )
      {
          // lExtra = offset for a DEFINE
         AddExprNodeEntityToView( vSubtask, vTargetView,
                                  qDEFINE,   // ElementClass
                                  0,         // data class
                                  lUseExtra, // data type
                                  pchExtra,   // text
                                  lExtra, 0 );  // offset
      }
      else
      {
         AddExprNodeEntityToView( vSubtask, vTargetView, qCONSTANT, 0,
                                  lUseExtra, pchExtra, lExtra, 0 );
      }
   }

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                            qTINTEGER, ")", qCLOSEOPER, 0 );
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
CompareDataTypesForConsistency( zLONG lTgtDataType,
                                zLONG lTgtDataClass,
                                zLONG lSrcDataType,
                                zLONG lSrcDataClass )
{
   // compare the data types for consistency
   if ( lTgtDataType == lSrcDataType )
      return( qTYPEEXACTMATCH );

   if ( lTgtDataType == qTUNKNOWN || lTgtDataType == 0 )
      return( qSETTARGETTOSOURCE );

   if ( lTgtDataClass == lSrcDataClass )
      return( qCLASSEXACTMATCH );

   return( qNOMATCHONTYPE );
}

zOPER_EXPORT zLONG OPERATION
CompareTargetTypeForConsistency( zLONG lTgtDataType,
                                 zLONG lLeftDataType,
                                 zLONG lLeftDataClass,
                                 zLONG lSrcDataType,
                                 zLONG lSrcDataClass )
{
   // The following are the normal checks above as in
   // CompareDataTypesForConsistency.
   // compare the data types for consistency

   if ( lLeftDataType == qTDATE )
      if ( lSrcDataType == qTSTRING )
         return( qSETTARGETTOSOURCE );
      else
         return( qNOMATCHONTYPE );

   if ( lLeftDataType == lSrcDataType )
      return( qTYPEEXACTMATCH );

   if ( lLeftDataType == qTUNKNOWN || lLeftDataType == 0 )
      return( qSETTARGETTOSOURCE );

   if ( lLeftDataClass == lSrcDataClass )
      return( qCLASSEXACTMATCH );

   // The following allows a source to be integer or decimal for
   // a string target.  The combination is valid if the TargetDataType
   // (which is the target of the equal) is string and the LeftDataClass
   // (which is the last value in the expression) is text, and the
   // SourceDataType is integer.  This will be the combination for a simple
   // A = B expression, where A is a string and B is an integer.  A complex
   // expression will somewhere violate this check.
   if ( lTgtDataType == qTSTRING && lLeftDataClass == qTEXT &&
        (lSrcDataType == qTINTEGER || lSrcDataType == qTDECIMAL) )
   {
      return( qSETTARGETTOSOURCE );
   }

   // The following allows the source and target to be combinations of short
   // and long. (For example, the target can be short and the source long.)
   if ( lLeftDataType == qTSHORT && lSrcDataType == qTINTEGER )
      return( qCLASSEXACTMATCH );

   if ( lLeftDataType == qTINTEGER && lSrcDataType == qTSHORT )
      return( qCLASSEXACTMATCH );

   return( qNOMATCHONTYPE );
}

//
// OPERATION: AddGetVariableFromAttribute
//
// DESCRIPTION: Creates a GetVariableFromAttribute call with the
//    result of the operation being put in nZRetCode
//
zOPER_EXPORT zLONG OPERATION
AddGetVariableFromAttribute( zVIEW  vSubtask,
                             zPCHAR pchOperationName,
                             zLONG  lViewID,
                             zLONG  lReturnLthID,
                             zLONG  lUseVariable,
                             zLONG  lVariableDataType,
                             zLONG  lVariableID,
                             zPCHAR pchEntityName,
                             zLONG  lEntityID,
                             zPCHAR pchAttributeName,
                             zLONG  lAttributeID,
                             zLONG  lUseExtra,
                             zPCHAR pchExtra,
                             zLONG  lExtra )
{
   zLONG sParmList[ PARM_LIST_SIZE ] = { 0 };
   zLONG sParmListExt[ PARM_LIST_SIZE ] = { 0 };
   zLONG lParmListIndex = 0;
   zLONG lRCID = 0;
   zLONG lOperZKey;
   zLONG lTempVariableID;
   zCHAR szVariableLth[ 12 ];

// memset( sParmListExt, 0, zsizeof( sParmListExt ) );
// lParmListIndex = 0;

   lLocal3Code = 0;
   lLocal3Text = 0;

   // Set temp variable ID because VariableID can be set to negative.
   lTempVariableID = lVariableID;

   // Insert new bottom of stack.
   AddOperatorToView( vSubtask, 0, -2 );

   // add result
   lRCID = AddRetCode( vSubtask );
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTSHORT, "RC", lRCID, 0 );

   // add operation name
   lOperZKey = GetOperationZKey( vSubtask, qZEIDONOPERATION, pchOperationName );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATIONCALL, qZEIDONOPERATION, qTSHORT, pchOperationName, lOperZKey, 0 );

   GetOperationParmListPM( vSubtask, sParmList, sParmListExt, lOperZKey, qZEIDONOPERATION );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "(", qOPENOPER, 0 );

   lParmListIndex = 1;  // return attribute value
   // Add target string variable.
   if ( sParmList[ lParmListIndex++ ] < 0 )
      lVariableID *= -1;

   if ( g_szGenLang[ 0 ] == 'J' )
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, qTSTRING, "", lVariableID, 0 );  // add return attribute value
   else
      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, lVariableDataType, "", lVariableID, 0 );  // add return attribute value

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   if ( sParmList[ lParmListIndex++ ] < 0 )  // return attribute value length
      lReturnLthID *= -1;

   if ( g_szGenLang[ 0 ] == 'J' )
   {
      // Add zero parm and comma.
      if ( lReturnLthID == 0 )
         ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, 0, qTINTEGER, "0", lVariableID, 0 );  // add return attribute value length
      else
         ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, qTINTEGER, "", lReturnLthID, 0 );  // add return attribute value length

   }
   else
   {
      // Add zero parm and comma.
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, 0, qTINTEGER, "0", lVariableID, 0 );  // add return attribute value length
   }

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add string type 'S' and comma.
   lParmListIndex++;  // variable type
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, 0, qTCHARACTER, "S", lVariableID, 0 );  // add variable type
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add variable length and comma. First position on variable.
   lParmListIndex++;  // variable length
   SetCursorFirstEntityByInteger( g_lpPIView, "Variable", "ID", lTempVariableID, 0 );
   GetStringFromAttribute( szVariableLth, zsizeof( szVariableLth ), g_lpPIView, "Variable", "NElements" );

   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, 0, qTINTEGER, szVariableLth, lVariableID, 0 );  // add variable length
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add view and comma.
   lParmListIndex++;  // view
   ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qNUMERIC, qTVIEW, "", lViewID, 0 );  // add view
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add entity name and comma.
   lParmListIndex++;  // entity name
   if ( lEntityID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lEntityID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, qTENTITYNAME, "", lEntityID, 0 );  // add entity name
   }
   else  // is a string constant
   {
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTENTITYNAME, pchEntityName, lEntityID, 0 );  // add entity name
   }

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add attribute name and comma.
   lParmListIndex++;  // attribute name
   if ( lAttributeID > 0 ) // is a var
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lAttributeID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, qTEXT, qTATTRIBUTENAME, "", lAttributeID, 0 );  // add attribute name
   }
   else  // is a string constant
   {
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTATTRIBUTENAME, pchAttributeName, lAttributeID, 0 );  // add attribute name
   }

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add context name and comma.
   lParmListIndex++;  // context name
   if ( lUseVariable == 1 )
   {
      if ( sParmList[ lParmListIndex ] < 0 )
         lVariableID *= -1;

      ADDEXPRESSIONENTRYTOSTRING( qVARIABLE, 0, lVariableDataType, "", lVariableID, 0 );  // add context name
   }
   else
   {
      ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, qTEXT, qTSTRING, "", lVariableID, 0 );
   }

   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ",", qCOMMA, 0 );

   // Add zero parm and close paren.
   lParmListIndex++;
   ADDEXPRESSIONENTRYTOSTRING( qCONSTANT, 0, qTINTEGER, "0", lVariableID, 0 );
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, ")", qCLOSEOPER, 0 );

   // add EQUAL
   ADDEXPRESSIONENTRYTOSTRING( qOPERATOR, qNUMERIC, qTINTEGER, "=", qEQUAL, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, 0, -1 );

   AddStatementEntityWithStringToPI( vSubtask, qEXPRESSION, qqlineno,
                                     szLocal3Code, &lLocal3Code, szLocal3Text, &lLocal3Text, zPOS_AFTER );
   return( 0 );
}