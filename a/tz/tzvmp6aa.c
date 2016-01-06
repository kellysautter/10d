/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp6aa.c - VML SubSystem Where statement build
// DESCRIPTION:   This is the source file which contains the auxilary
//                functions that are necessary to build the where clause
//                into sets and Fors
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/08/13
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
2001.09.26 HH
   implement Set Cursor WHERE with parens.
2001.02.19 HH
   implement Set Cursor with Object Scope.
2000.10.22  HH
   corrected SET CURSOR WHERE A AND B OR C AND D.
   raise error message on invalid right expression.
2000.03.15  HH
   bug TB1032, the right side of an expression inside a WHERE
   is undefined, give an error message.

old CHANGE LOG

04.08.1998  DonC
   Modified InsertSetCursorWithWhere to eliminate optimized SET CURSOR WHERE
   logic for boolean conditions because it was not working properly and
   because the nonoptimized code is easier to understand from a generated C
   standpoint.

18.08.1998  HH
   Modified InsertSetCursorWithWhere to call SetCursorNext instead of
   CheckExistence in case of "SET CURSOR NEXT WHERE not-opt-expression".

04.09.1998  HH
   Modified InsertSetCursorWithWhere so that PREVIOUS and LAST are not
   optimized, as the SetCursorBy... calls do not exist in this case.

17.12.1998  HH
   use NewGenerateTempVariable, so that a temp variable for a "date"
   attribute is handled as "string".

*/

static zSHORT SetCursorWhereNoOpt( zVIEW vSubtask, zVIEW vTargetView );
static void InsertParens( void );


/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS: InsertSetCursorWithWhere
//
/////////////////////////////////////////////////////////////////////////////

//
// OPERATION: InsertSetCursorWithWhere
//
// DESCRIPTION:
//
zOPER_EXPORT zLONG OPERATION
InsertSetCursorWithWhere( zVIEW vSubtask )  // java???
{
   zCHAR szViewName[ 33 ];
   zLONG lViewID;
   zCHAR szEntityName[ 33 ];
   zLONG lEntityID;
   zCHAR szAttributeName[ 33 ];
   zLONG lAttributeID;
   zCHAR szRightViewName[ 33 ];
   zLONG lRightViewID;
   zCHAR szRightEntityName[ 33 ];
   zLONG lRightEntityID;
   zCHAR szRightAttributeName[ 33 ];
   zLONG lRightAttributeID;
   zLONG lTempID;
   zLONG lLth;
   zCHAR szOperationName[ 33 ];
   zLONG lResultID;
   zLONG lCantOptOut;   // int flag to say we can't optimize because of ORs
   zLONG lLoop;
   zLONG lOperator;
   zLONG lUseFirst;
   zVIEW vTargetView;
   zLONG lNewDataType;
   zBOOL bSetNextJava = FALSE;

   // TB 519: first check, whether the WHERE Clause uses an EXISTS or a DOES NOT EXIST or
   //  other clauses, which are only allowed for ACTIVATE, not for SET CURSOR.
   //  In that case, issue an error
   if ( g_lWhereOp[ 1 ] == qEXISTS ||
        g_lWhereOp[ 1 ] == qNOTEXISTS ||
        g_lWhereOp[ 1 ] == qLIKE )
   {
      SyntaxError( vSubtask, eQINVALIDSYNTAXINWHERE, "");
      return( -1 );
   }

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );
   //SetViewToSubobject( vTargetView, "Expression" );

   lResultID = AddResult( vSubtask );

   // Change 04.08.1998 DonC
   // Forced off optimization if there are more than two values in the WHERE
   // because the code was not being generated correctly and because it added
   // more complexity to the generated code with little benefit.
   // If we have a set cursor PREVIOUS or LAST, there is no way to
   //  optimize, the calls SetCursor...By... do not exist.
   // As the "optimized" code is generated with SetCursor...By... functions,
   //  and those functions are NOT working with object scope, we do not
   //  optimize, if we got "object scope".

   if ( g_lWhereIndex > 2 || g_szScopeEntityName[ 0 ] == '\1' ||
        g_lSetFNPL == qPREVIOUS || g_lSetFNPL == qLAST )
   {
      lCantOptOut = 1;
   }
   else
      lCantOptOut = 0;

   // if there are no ORs then we might possible be able to "opt" out some
   // calls if there is at least one cond equal check. Therefore, check to
   // see if there are any. If Not then we use the same logic as we would if
   // there were ORs so what the heck, check
   if ( lCantOptOut == 0 )
   {
      // init lUseFirst to -1
      lUseFirst = -1;

      // Try to find a AQ == somevalue so that we can do SetFirst .. by value.
      // if we find one, make sure that the AQ is within the controlling entity.
      // Remember the controlling entity is the one we are setting for. It
      // should be in szWO[ 0 ] and szWE[ 1 ].
      for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
      {
         // if the operator is ==, and obj is same, and cur is same,
         // and ent is same and it is not an AQ == AQ
         if ( g_lWhereOp[ lLoop ] == qCONDEQUAL &&
          //  g_sWhereRight[ lLoop ].lElementClass != qATTRIBUTEQUALIFIER &&
              zstrncmp( g_sWhereLeft[ 0 ].szText,
                        g_sWhereLeft[ lLoop ].szText,
                        (zSHORT) g_sWhereLeft[ 0 ].lOffset ) == 0 ) // length of EQ, attr irrelvant
         {
            lUseFirst = lLoop;
            break;
         }
      }

      // If lUseFirst == -1, then we cannot "promote" any equality to the cond equal
      // position so do the same thing as was done for the statements that use ORs.
      if ( lUseFirst == -1 )
         lCantOptOut = 1;
   }

   // Case of all CompareAttributes ... cannot optimize out any statements
   if ( lCantOptOut == 1 )
   {
      return( SetCursorWhereNoOpt( vSubtask, vTargetView ) );
   }

   // /////////////////////////////////////////////////////////////////////
   // we are able to opt out at least one.
   // /////////////////////////////////////////////////////////////////
   // else there were no OR conditions so we can go happy with optimizing
   // out some calls. Additionally, lUseFirst is set to the first possible
   // cond equal condition we can use.

   // If the g_lSetFNPL is each, then we need to put in a set first else all
   // we need to do is a CheckExistence.
   if ( g_lSetFNPL == qFIRST || g_lSetFNPL == qLAST || g_lSetFNPL == qNEXT )
   {
      // since the first aq == lUseFirst.aq use nUse first to get attr
      SplitQualifier( g_sWhereLeft[ lUseFirst ].szText, szViewName, szEntityName, szAttributeName );
      GetVariableID( &lViewID, szViewName );

      if ( g_sWhereLeft[ lUseFirst ].lDataClass == qENTITYISUNKNOWN ||
           g_sWhereLeft[ lUseFirst ].lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
         GetVariableID( &lEntityID, szEntityName );
      else
         lEntityID = 0;

      if ( g_sWhereLeft[ lUseFirst ].lDataClass == qATTRIBUTEISUNKNOWN ||
           g_sWhereLeft[ lUseFirst ].lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
         GetVariableID( &lAttributeID, szAttributeName );
      else
         lAttributeID = 0;

      if ( g_sWhereRight[ lUseFirst ].lElementClass == qATTRIBUTEQUALIFIER )
      {
         SplitQualifier( g_sWhereRight[ lUseFirst ].szText, szRightViewName,
                         szRightEntityName, szRightAttributeName );
         GetVariableID( &lRightViewID, szRightViewName );

         if ( g_sWhereRight[ lUseFirst ].lDataClass == qENTITYISUNKNOWN ||
              g_sWhereRight[ lUseFirst ].lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
            GetVariableID( &lRightEntityID, szRightEntityName );
         else
            lRightEntityID = 0;

         if ( g_sWhereRight[ lUseFirst ].lDataClass == qATTRIBUTEISUNKNOWN ||
              g_sWhereRight[ lUseFirst ].lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
            GetVariableID( &lRightAttributeID, szRightAttributeName );
         else
            lRightAttributeID = 0;

         // create the get an then put a get at the end...
         strcpy_s( szOperationName, sizeof( szOperationName ), "Get" );

         if ( g_sWhereRight[ lUseFirst ].lDataType == qTUNKNOWN )
              g_sWhereRight[ lUseFirst ].lDataType = g_sWhereLeft[ lUseFirst ].lDataType;

         switch ( g_sWhereRight[ lUseFirst ].lDataType )
         {
            case qTDECIMAL:
               g_sWhereRight[ lUseFirst ].lDataClass = qNUMERIC;
               strcat_s( szOperationName, sizeof( szOperationName ), "DecimalFromAttribute" );
               break;

            case qTINTEGER:
               case qTSHORT:
               g_sWhereRight[ lUseFirst ].lDataClass = qNUMERIC;
               strcat_s( szOperationName, sizeof( szOperationName ), "IntegerFromAttribute" );
               break;

            default:
               g_sWhereRight[ lUseFirst ].lDataClass = qTEXT;
               strcat_s( szOperationName, sizeof( szOperationName ), "StringFromAttribute" );
               break;
         }

         if ( lRightEntityID == 0 && lRightAttributeID == 0 )
            lLth = GetLengthFromAttribute( lRightViewID, szRightEntityName, szRightAttributeName );
         else
         if ( lEntityID == 0 && lAttributeID == 0 )
         {
            lLth = GetLengthFromAttribute( lViewID, szEntityName, szAttributeName );
         }
         else
            lLth = -1;

         if ( lLth == -1 )
            lLth = 1024;

         lTempID = NewGenerateTempVariable( vSubtask, g_sWhereRight[ lUseFirst ].lDataType,
                                            lLth, &lNewDataType );

         // cheat
         g_sWhereRight[ lUseFirst ].lElementClass = qVARIABLE;
         g_sWhereRight[ lUseFirst ].lOffset = lTempID;

         AddGetOrSetAttributeWithString( vSubtask, szOperationName, lRightViewID, 0,
                                         lNewDataType, g_sWhereRight[ lUseFirst ].lOffset,
                                         szRightEntityName, lRightEntityID,
                                         szRightAttributeName, lRightAttributeID,
                                         0, "", 0, 0 );
      }

     // KJS 09/04/12 - We are currently going back to using SetCursorFirstEntity etc. instead of .setFirst etc.
     // This is because DG has change the return code from the .set cursor functions and our generated code no
     // longer works.  We may want to go back at some point so I am keeping the code.
     /*
      if ( g_szGenLang[ 0 ] == 'J' )
      {
         if ( g_lSetFNPL == qFIRST )
            strcpy_s( szOperationName, sizeof( szOperationName ), "setFirst" );
         else
         if ( g_lSetFNPL == qNEXT )
         {
            bSetNextJava = TRUE;
            strcpy_s( szOperationName, sizeof( szOperationName ), "setNextContinue" );
         }
         else
            strcpy_s( szOperationName, sizeof( szOperationName ), "setLast" );
#if 0
         switch ( g_sWhereRight[ lUseFirst ].lDataClass )
         {
            case qNUMERIC:
               if ( g_sWhereRight[ lUseFirst ].lDataType == qTDECIMAL )
                  strcat_s( szOperationName, sizeof( szOperationName ), "ByDecimal" );
               else
                  strcat_s( szOperationName, sizeof( szOperationName ), "ByInteger" );
               break;

            case qTEXT:
               strcat_s( szOperationName, sizeof( szOperationName ), "ByString" );
               break;

            default:
               // Issue Error, TB1032
               //  the right side of an expression inside a WHERE
               //  is undefined.
               SyntaxError( vSubtask, eQINVALIDSYNTAXINWHERE, "");
               return( -1 );
         }
#endif
      }
      else
      {
     */
         if ( g_lSetFNPL == qFIRST )
            strcpy_s( szOperationName, sizeof( szOperationName ), "SetCursorFirstEntity" );
         else
         if ( g_lSetFNPL == qNEXT )
            strcpy_s( szOperationName, sizeof( szOperationName ), "SetCursorNextEntity" );
         else
            strcpy_s( szOperationName, sizeof( szOperationName ), "SetCursorLastEntity" );

         switch ( g_sWhereRight[ lUseFirst ].lDataClass )
         {
            case qNUMERIC:
               if ( g_sWhereRight[ lUseFirst ].lDataType == qTDECIMAL )
                  strcat_s( szOperationName, sizeof( szOperationName ), "ByDecimal" );
               else
                  strcat_s( szOperationName, sizeof( szOperationName ), "ByInteger" );
               break;

            case qTEXT:
               strcat_s( szOperationName, sizeof( szOperationName ), "ByString" );
               break;

            default:
               // Issue Error, TB1032
               //  the right side of an expression inside a WHERE
               //  is undefined.
               SyntaxError( vSubtask, eQINVALIDSYNTAXINWHERE, "");
               return( -1 );
         }
      //}

      AddEntityOperWithScope( vSubtask, szOperationName, lViewID, szEntityName, lEntityID,
                              g_sWhereRight[ lUseFirst ].lDataType,
                              szAttributeName, lAttributeID,
                              bSetNextJava ? "" : g_szScopeEntityName,
                              bSetNextJava ? 0 : g_lScopeEntityID,
                              g_sWhereRight[ lUseFirst ].lElementClass,
                              g_sWhereRight[ lUseFirst ].szText,
                              g_sWhereRight[ lUseFirst ].lOffset );

      if ( g_lWhereIndex > 2 )
         AddResultCondEqual( vSubtask, qIF, "zCURSOR_UNCHANGED" );
   }
   else
   {
      SplitQualifier( g_sWhereLeft[ 0 ].szText, szViewName,
                            szEntityName, szAttributeName );
      GetVariableID( &lViewID, szViewName );

      if ( g_sWhereLeft[ 0 ].lDataClass == qENTITYISUNKNOWN )
         GetVariableID( &lEntityID, szEntityName );
      else
         lEntityID = 0;

      AddEntityOperWithNoScope( vSubtask, "CheckExistenceOfEntity", lViewID,
                                szEntityName, lEntityID, 0, "", lResultID );
      AddResultCondEqual( vSubtask, qIF, "zCURSOR_NULL" );
   }

   if ( g_lWhereIndex > 2 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qLPAREN, 0 );
      AddResultCondExprToView( vSubtask, vTargetView, qGREATERTHAN, "zCURSOR_UNCHANGED" );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qAND, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qLPAREN, 0 );
   }

   for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
   {
      if ( lLoop != lUseFirst )
      {
         // A compare to zero will be added based on the datatype of the attribute or value that was
         // given. This is stored in cWLiteralType at this loop counter. First, however, we need to
         // change the operator to the opposite since we are doing the "other" comparison.
         strcpy_s( szOperationName, sizeof( szOperationName ), "CompareAttributeTo" );

         if ( g_sWhereRight[ lLoop ].lElementClass == qATTRIBUTEQUALIFIER )
         {
            strcat_s( szOperationName, sizeof( szOperationName ), "Attribute" );
         }
         else
         {
            switch( g_sWhereRight[ lLoop ].lDataType )
            {
               // if the data type was String
               case qTENTITYNAME:
               case qTATTRIBUTENAME:
               case qTSTRING:
               case qTCHARACTER:
                  strcat_s( szOperationName, sizeof( szOperationName ), "String" );
                  break;

               // if the data type was Integer
               case qTINTEGER:
               case qTSHORT:
                  strcat_s( szOperationName, sizeof( szOperationName ), "Integer" );
                  break;

               // if the data type was Decimal
               case qTDECIMAL:
                  strcat_s( szOperationName, sizeof( szOperationName ), "Decimal" );
                  break;

               default:
                  break;
            }
         }

         //if ( g_lWhereOp[ lLoop ] != qCONDEQUAL &&
         //     g_lWhereOp[ lLoop ] != qNOTEQUAL )
            lOperator = GetOppositeOperator( g_lWhereOp[ lLoop ] );
         //else
         //   lOperator = g_lWhereOp[ lLoop ];


         AddCompareToView( vSubtask, vTargetView, szOperationName,
                           &g_sWhereLeft[ lLoop ], g_sWhereLeft[ lLoop ].szText,
                           &g_sWhereRight[ lLoop ], g_sWhereRight[ lLoop ].szText, lOperator );
         if ( (g_lWhereJoin[ lLoop ] != 0) && (lLoop + 1 < g_lWhereIndex) )
         {
            AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "",
                                     GetOppositeOperator( g_lWhereJoin[ lLoop ] ), 0 );
         }
      }
   }

   if ( g_lWhereIndex > 2 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qRPAREN, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qRPAREN, 0 );

      // Dump the expression from the stack.
      AddOperatorToView( vSubtask, vTargetView, -1 );

      // Add the while statement to do all of these compares...
      AddStatementEntityToPI( vSubtask, qWHILE, qqlineno, vTargetView, zPOS_AFTER );

     /*
      if ( g_szGenLang[ 0 ] == 'J' )
      {
         bSetNextJava = TRUE;
         if ( g_lSetFNPL == qFIRST || g_lSetFNPL == qNEXT )
            strcpy_s( szOperationName, sizeof( szOperationName ), "setNextContinue" );
         else
            strcpy_s( szOperationName, sizeof( szOperationName ), "setPrevContinue" );
      }
      else
      {
     */
         bSetNextJava = FALSE;
         if ( g_lSetFNPL == qFIRST || g_lSetFNPL == qNEXT )
            strcpy_s( szOperationName, sizeof( szOperationName ), "SetCursorNextEntity" );
         else
            strcpy_s( szOperationName, sizeof( szOperationName ), "SetCursorPrevEntity" );

         switch ( g_sWhereRight[ lUseFirst ].lDataClass )
         {
            case qNUMERIC:
               if ( g_sWhereRight[ lUseFirst ].lDataType == qTDECIMAL )
                  strcat_s( szOperationName, sizeof( szOperationName ), "ByDecimal" );
               else
                  strcat_s( szOperationName, sizeof( szOperationName ), "ByInteger" );

               break;

            case qTEXT:
               strcat_s( szOperationName, sizeof( szOperationName ), "ByString" );
               break;
         }
      //}

      AddEntityOperWithScope( vSubtask, szOperationName, lViewID, szEntityName, lEntityID,
                              g_sWhereRight[ lUseFirst ].lDataType, szAttributeName, lAttributeID,
                              bSetNextJava ? "" : g_szScopeEntityName,
                              bSetNextJava ? 0 : g_lScopeEntityID,
                              g_sWhereRight[ lUseFirst ].lElementClass,
                              g_sWhereRight[ lUseFirst ].szText,
                              g_sWhereRight[ lUseFirst ].lOffset );

      // Dump the expression from the stack.
      AddOperatorToView( vSubtask, vTargetView, -1 );

      // Add the while statement to do all of these compares...
      AddStatementEntityToPI( vSubtask, qEND, qqlineno, vTargetView, zPOS_AFTER ); // IF

      // Add the while statement to do all of these compares...
      AddStatementEntityToPI( vSubtask, qEND, qqlineno, vTargetView, zPOS_AFTER ); // WHILE
   }

   ReuseTempVariable( lTempID );
   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );

   return( 0 );
}

//
// OPERATION: AddResultGreaterThan
//
//    e.g. while ( RESULT > CursorResult.UNCHANGED &&
//
zOPER_EXPORT zLONG OPERATION
AddResultGreaterThan( zVIEW  vSubtask,
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

   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qLPAREN, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC, qTSHORT, "", lResultID, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qGREATERTHAN, 0 );
   lDefineZKey = GetDefineZKey( vSubtask, &lVarDataType, pchCompareValue );
   AddExprNodeEntityToView( vSubtask, vTargetView, qDEFINE, qZEIDONOPERATION, qTINTEGER, pchCompareValue, lDefineZKey, 0 );
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qAND, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vTargetView, -1 );

   AddStatementEntityToPI( vSubtask, lStatementType, qqlineno, vTargetView, zPOS_AFTER );

   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );

   return( 0 );
}

// Generate Set Cursor with where and scope.
// "No Opt" means, that the where condition check is explicitly generated by an IF statement.
// The "optimized" functions as SetCursorFirstEntityByString are NOT generated.
static zSHORT
SetCursorWhereNoOpt( zVIEW vSubtask, zVIEW vTargetView )  // java???
{
   zLONG lLoop;
   zLONG lOperator;
   zLONG lRC;

   // Do AND-OR-Analysis.
   // If the expression contains AND and OR we must regard, that
   // the AND has higher priority. After converting to the opposite
   // operator for negating, we get wrong logic if we do not put
   // parens around the expressions, which are joined by OR.
   // Example:
   // V.E.A1 > 2 AND V.E.A1 < 4 OR V.E.A2 = 'X' OR V.E.A2 = 'Y'
   // would be transformed into
   // (V.E.A1 > 2 AND V.E.A1 < 4) OR (V.E.A2 = 'X' OR V.E.A2 = 'Y')
   // so that the opposite is
   // (V.E.A1 <= 2 OR V.E.A1 >= 4) AND (V.E.A2 != 'X' AND V.E.A2 != 'Y')

   if ( g_lWhereIndex > 2 )
      InsertParens( );

   // Insert the correct CoreOperation call depending on whether or not we are
   // setting EACH NEXT VAR CONST.
   SplitQualifier( g_sWhereLeft[ 0 ].szText, g_szViewName, g_szEntityName, g_szAttributeName );
   GetVariableID( &g_lViewID, g_szViewName );

   if ( g_sWhereLeft[ 0 ].lDataClass == qENTITYISUNKNOWN )
      GetVariableID( &g_lEntityID, g_szEntityName );
   else
      g_lEntityID = 0;

   switch ( g_lSetFNPL )
   {
      case qFIRST:
         GenerateSetCursorWithScope( vSubtask, zPOS_FIRST );
         break;

      case qNEXT:
         GenerateSetCursorWithScope( vSubtask, zPOS_NEXT );
         break;

      case qPREVIOUS:
         GenerateSetCursorWithScope( vSubtask, zPOS_PREV );
         break;

      case qLAST:
         GenerateSetCursorWithScope( vSubtask, zPOS_LAST );
         break;

      default:
         break;
   }

   // VML:
   // SET CURSOR FIRST KZXMLPGO.DynamicBannerName WITHIN KZXMLPGO.Session
   //            WHERE KZXMLPGO.DynamicBannerName.DialogName = "abc"
   //              AND KZXMLPGO.DynamicBannerName.BannerName = "def"
   //
   // Java Generation:
   // RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setFirst( "DialogName", "abc", "Session" );
   // if ( RESULT > CursorResult.UNCHANGED )
   // {
   //    while ( RESULT > CursorResult.UNCHANGED &&
   //            (KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "DialogName" ).equals( "abc" ) == false ||  // note--this line isn't necessary
   //             KZXMLPGO.cursor( "DynamicBannerName" ).attribute( "BannerName" ).equals( "def" ) == false) )
   //    {
   //       RESULT = KZXMLPGO.cursor( "DynamicBannerName" ).setNextContinue( );
   //    }
   // }
   //
   // C generation:
   // if ( RESULT > zCURSOR_UNCHANGED )
   // {
   //    while ( RESULT > zCURSOR_UNCHANGED &&
   //            (CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "DialogName", "abc" ) != 0 ||
   //             CompareAttributeToString( KZXMLPGO, "DynamicBannerName", "BannerName", "def" ) != 0) )
   //    {
   //       RESULT = SetCursorNextEntity( KZXMLPGO, "DynamicBannerName", "Session" );
   //    }
   // }

// if ( g_szGenLang[ 0 ] == 'J' )
// {
//    AddJavaWhileWithScope( vSubtask, vTargetView );
// }
// else
   {
   // if ( RESULT > zCURSOR_UNCHANGED )
   // {
   //    while ( RESULT > zCURSOR_UNCHANGED &&

      // Before we put in the first compare, put in the RESULT == zCURSOR_UNCHANGED so that if we hit
      // the end of the entities and the condition is not true, we get out quicker.
      AddResultCondEqual( vSubtask, qIF, "zCURSOR_UNCHANGED" );

      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qLPAREN, 0 );
      AddResultCondExprToView( vSubtask, vTargetView, qGREATERTHAN, "zCURSOR_UNCHANGED" );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qAND, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qLPAREN, 0 );

      // This is where we begin adding the comparisons to create the while loop. For each where
      // condition parsed, do a CompareAttributeToXXXXXX where XXXXX is the datatype of the
      // comparison OR another Attribute.
      for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
      {
         zLONG k;

         // Insert opening parens, if necessary.
         for ( k = 0; k < g_sWhereLeft[ lLoop ].lLevel; k++ )
         {
            AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qLPAREN, 0 );
         }

         // A compare to zero will be added based on the datatype of the attribute or value that was
         // given. This is stored in .DataType at this loop counter.  First, however, we need to
         // change the operator to the opposite since we are doing the "other" comparison.
         strcpy_s( g_szOperationName, sizeof( g_szOperationName ), "CompareAttributeTo" );

         if ( g_sWhereRight[ lLoop ].lElementClass == qATTRIBUTEQUALIFIER )
         {
            strcat_s( g_szOperationName, sizeof( g_szOperationName ), "Attribute" );
         }
         else
         {
            if ( g_sWhereRight[ lLoop ].lDataType == qTUNKNOWN )
               g_sWhereRight[ lLoop ].lDataType = g_sWhereLeft[ lLoop ].lDataType;

            switch( g_sWhereRight[ lLoop ].lDataType )
            {
               // if the data type was String
               case qTENTITYNAME:
               case qTATTRIBUTENAME:
               case qTSTRING:
               case qTCHARACTER:
                  strcat_s( g_szOperationName, sizeof( g_szOperationName ), "String" );
                  break;

               // if the data type was Integer
               case qTINTEGER:
               case qTSHORT:
                  strcat_s( g_szOperationName, sizeof( g_szOperationName ), "Integer" );
                  break;

               // if the data type was Decimal
               case qTDECIMAL:
                  strcat_s( g_szOperationName, sizeof( g_szOperationName ), "Decimal" );
                  break;

               default:
                  break;
            }
         }

         //if ( g_lWhereOp[ lLoop ] != qCONDEQUAL && g_lWhereOp[ lLoop ] != qNOTEQUAL )
            lOperator = GetOppositeOperator( g_lWhereOp[ lLoop ] );
         //else
         // lOperator = g_lWhereOp[ lLoop ];

         lRC = AddCompareToView( vSubtask, vTargetView, g_szOperationName,
                                 &g_sWhereLeft[ lLoop ], g_sWhereLeft[ lLoop ].szText,
                                 &g_sWhereRight[ lLoop ], g_sWhereRight[ lLoop ].szText, lOperator );
         if ( lRC < 0 )
         {
            SyntaxError( vSubtask, eQINVALIDSYNTAXINWHERE, "");
            return( -1 );
         }

         // Insert closing parens, if necessary.
         for ( k = 0; k < g_sWhereRight[ lLoop ].lLevel; k++ )
         {
            AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qRPAREN, 0 );
         }

         // Now check for a JOIN (AND or OR).
         if ( g_lWhereJoin[ lLoop ] != 0 )
            AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "",
                                     GetOppositeOperator( g_lWhereJoin[ lLoop ] ), 0 );
      }

      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qRPAREN, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qRPAREN, 0 );

      // Dump the expression from the stack.
      AddOperatorToView( vSubtask, vTargetView, -1 );

      // Add the while statement to do all of these compares...
      AddStatementEntityToPI( vSubtask, qWHILE, qqlineno, vTargetView, zPOS_AFTER );

      if ( g_lSetFNPL == qFIRST || g_lSetFNPL == qNEXT )
         GenerateSetCursorWithScope( vSubtask, zPOS_NEXT );
      else
         GenerateSetCursorWithScope( vSubtask, zPOS_PREV );

      // Dump the expression from the stack.
      AddOperatorToView( vSubtask, vTargetView, -1 );

      // End the if statement to do all of these compares...
      AddStatementEntityToPI( vSubtask, qEND, qqlineno, vTargetView, zPOS_AFTER ); // IF

      // End the while statement to do all of these compares...
      AddStatementEntityToPI( vSubtask, qEND, qqlineno, vTargetView, zPOS_AFTER ); // WHILE
   }

   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );
   return( 0 );
}

//-------------------------------------------------------------------
// >>>InsertParens
//
// Find out, where parens are to be put
//  if AND and OR are not mixed, we do not need any
//  otherwise: the expressions joined with AND must be inside
//  parens
// Add necessary parens around AND if there is an OR on the
//  same level.
//
// The WHERE expression is as follows    A = B AND C > D
//
// g_sWhereLeft[ 1 ] = A
// g_lWhereOp[ 1 ] = '='
// g_sWhereRight[ 1 ] = B
// g_lWhereJoin[ 1 ] = AND
// g_sWhereLeft[ 2 ] = C
// g_lWhereOp[ 2 ] = '>'
// g_sWhereRight[ 2 ] = D
//
//-------------------------------------------------------------------
static void
InsertParens( void )
{
   zSHORT nRestart;
   zLONG  lLevel= 0;
   zLONG  lJoinLevel[ WHERE_ARRAY_SIZE ] = { 0 };
   zLONG  i, j, k;
   zLONG  lConditions = g_lWhereIndex - 1; // # Where parts
   zLONG  lJoinOper = lConditions - 1;   // # Join Operators
   zLONG  lMaxLevel;

   nRestart = 1;
   while ( nRestart )
   {
      nRestart = 0;
      lMaxLevel = 0;
      lLevel = 0;

      // set the Join operator levels ... we have g_lWhereIndex - 1 join operators
      for ( i = 1; i <= lConditions; i++ )
      {
         if ( g_sWhereLeft[ i ].lLevel > 0 )
            lLevel += g_sWhereLeft[ i ].lLevel;  // opening parens before

         if ( g_sWhereRight[ i ].lLevel > 0 )
            lLevel -= g_sWhereRight[ i ].lLevel; // closing parens after

         lJoinLevel[ i ] = lLevel; // level of Join operator
         if ( lLevel > lMaxLevel )
            lMaxLevel = lLevel;
      }

      // start at the top level (1) going down to lMaxLevel
      //  look for AND and OR on the same level
      //  after a paren was inserted, restart the process
      for ( lLevel = 0; lLevel <= lMaxLevel && nRestart == 0; lLevel++ )
      {
         zSHORT nBoth;
         zLONG  lStart;
         zLONG  lFirstAnd;

         lFirstAnd = 0;
         lStart = 1;
         while ( lStart <= lJoinOper && nRestart == 0)
         {
            // starting from lStart position, find Operator with lLevel
            for ( i = lStart; i <= lJoinOper; i++ )
            {
               if ( lJoinLevel[ i ] == lLevel )
                  break;
            }

            if ( i >= lJoinOper )
            {
               // nothing found on this level
               break; // continue with next level
            }

            // advance until end of current level
            //  set flag, if AND or OR are encountered
            nBoth = 0;
            for ( j = i; j <= lJoinOper; j++ )
            {
               if ( lJoinLevel[ j ] < lLevel )
                  break;

               if ( lJoinLevel[ j ] == lLevel )
               {
                  if ( g_lWhereJoin[ j ] == qAND )
                  {
                     nBoth = nBoth | 1; // indicate AND found
                     if ( lFirstAnd == 0 )
                        lFirstAnd = j;
                  }

                  if ( g_lWhereJoin[ j ] == qOR )
                  {
                     nBoth = nBoth | 2; // indicate OR found
                  }
               }
            }

            if ( nBoth < 3 )
            {
               // we did not find AND and OR at the same level
               lStart = j;
               continue;
            }

            // found AND and OR at the same level, insert parens
            // we first insert an opening paren before the first AND
            // if there are operators with higher level before,
            // we must insert the paren before those.
            for ( k = lFirstAnd - 1; k >= 1; k-- )
            {
               if ( lJoinLevel[ k ] <= lLevel )
                  break; // break on lower or same level
            }

            g_sWhereLeft[ k+1 ].lLevel++;
            // then go right until the end of the and chain,
            //  if there is more than one.
            for ( k = lFirstAnd +1; k < j; k++ )
            {
               if ( lJoinLevel[ k ] == lLevel && g_lWhereJoin[ k ] != qAND )
                  break; // break, if we find an OR on same level
            }

            // put closing paren after last end in chain
            g_sWhereRight[ k ].lLevel++;

            // restart process as we put in a new pair of parens
            nRestart = 1;
         } // while ( lStart < g_lWhereIndex )

      } // for ( lLevel = 0; lLevel <= nMaxLevel && nRestart == 0; lLevel++ )

   } // while ( nRestart )
}
