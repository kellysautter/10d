/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp5aa.c - VML SubSystem Where statement build
// DESCRIPTION:   This is the source file which contains the auxilary
//                functions that are necessary to build the where clause
//                into sets and Fors
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
/*
CHANGE LOG

2001.09.26 HH
   implement Set Cursor WHERE with parens.
2001.02.18  HH
   Implemented FOR EACH WITHIN OBJECT WHERE ...
1999.02.16  HH
   SyntaxError message eQINVALIDSYNTAXINWHERE.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"

static zSHORT ForEachNoOpt( zVIEW vSubtask, zVIEW vTargetView, zLONG lResultID );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS:   InsertForWithWhere
//
/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: InsertForWithWhere
//
// DESCRIPTION: This function is called only if a where clause has been
//    used after the entity qualifier. If there is not one, then all
//    of the necessary statements have already been added to the parsed
//    object. This function works in a manner similar to the SetCursor
//    statements.
//
//    The first thing to do in here is store the VEA for the FOR stmt
//    as we will need to put in a SET CURSOR at the end of
//    the loop. We can just do this first as it does not depend on any
//    part of the where.
//
//    If g_lSetFNPL is set to qEACH, then the cursor for the
//    entity that is to be looped on needs to be reset back to the
//    beginning. If FNPL is NEXT (no keyword), a VARIABLE or CONST,
//    add a check existence of entity. A const loop (4) means that a
//    constant or symbol was given to furthur control the loop. For
//    example:
//       FOR EACH V.E  <-- sets FNPL to qEACH
//       FOR V.E       <-- sets FNPL to qNOEACH
//       FOR 13 V.E    <-- sets FNPL to qCONSTANT
//       FOR nLoop V.E <-- sets FNPL to qVARIABLE
//
//    If the loop is qCONSTANT or qVARIABLE then we need to create and
//    add a loop variable by calling CreateConstantLoop.
//
//    1: Determine if any OR joins exist. If any do, then all that can
//          be done is build the WHILE statement.
//       Loop through all the stuff stored in the where variables that
//          were built during the parse, and create CompareAttribute
//          statements for each. Note that what we want to do is continue
//          to loop through all of the entities while the condition given
//          in the where is true. Not this is the major difference
//          between this and the SET CURSOR stuff.
//       Done.
//
//    2: This is the case where all the joins are ANDs or, there is only
//          one actual where to check.
//       Find a condition that checks for equality. If one is found,
//       |- and the AQ is the same as the LOOP EQ ( plus attribute ),
//       |  (if both left and right values are AQ's) then do a get
//       |     for the datatype into a temp variable. This is because
//       |     there is no way to set cursor for enity by another
//       |     attribute value.)
//       |  Create a string "RESULT = SetCursor"
//       |  if FNL = ALL, append "FirstEntity" to string.
//       |  if FNL = NEXT, append "NextEntity" to string.
//       |  if FNL = PREVIOUS, append "PreviousEntity" to string.
//       |     if the Datatype is STRING append "ByString" to string
//       |     if the Datatype is INTEGER append "ByInteger" to string
//       |     if the Datatype is DECIMAL append "ByDecimal" to string
//       |
//       |  Loop through all the stuff stored in the where variables that
//       |     were built during the parse EXCEPT the one that we just use
//       |     as the first AQ and create CompareAttribute
//       |     statements for each. Note that what we want to do is
//       |     continue to loop through all of the entities while the
//       |     SAME condition given in the where is true. This is NOT the
//       |     same as the SET CURSOR. Add "AND RESULT == 0" first to set
//       |     the verification value of result and avoid infinate looping
//       |     situations. If there was a looping control then add it > 0
//       |     as the second condition of the while. What we would have
//       |     then is  RESULT == 0 AND nLoop > 0 AND  Add the WHILE.
//       |--Insert statement.
//
//       Find no conditions check for equality then
//       |  Loop through all the stuff stored in the where variables that
//       |     were built during the parse, an create CompareAttribute
//       |     statements for each. Note that what we want to do is cont
//       |     to loop through all of the entities while the SAME
//       |     condition given in the where is true.
//       |- SetHandle Again and put in SetCursor Next/Prev.
//
//       Note that the controlling entity for any set cursor statement
//       will be stored in szWX[ 0 ] (where variables at 0). Therefore
//       always use these for inserting set cursor statements
//
//       At this point what needs to happen is that all the statements
//       that the user has inserted into the loop statement need to
//       be inserted. THEN a SetCursor NEXT whatever needs to be inserted
//       at the end of it all so that the position on the entity actually
//       moves as per the loop. HOWEVER, those statements that have to be
//       inserted have not yet been parsed. What we can do is store all
//       the values and right before the end put that statement in. This
//       is okay but what happens instead is that here, that statement
//       is inserted. Additionally, and empty or useless statement is
//       inserted before it. This is because AddStatement( ) always
//       inserts a new statement after the current position. So If we
//       just add the necessary SetCursor statements, it will precede
//       the loop statements and always be off by one. The Dummy
//       statement will be inserted before the SetCursor so that all
//       future statements in this loop will end up being inserted after
//       the dummy statement and before the SetCursor. When we are all
//       done with the loop statement (have hit the END) we will
//       just delete the First statements under the loop (which  will be
//       the dummy statement).
//
//    RESULT: The VML keyword RESULT will be set to 0 is the operation
//       was a success or something else (as returned from the
//       cursor operations) if not.
//

zOPER_EXPORT zLONG OPERATION
InsertForWithWhere( zVIEW vSubtask )
{
   zCHAR szRightViewName[ 33 ];
   zLONG lRightViewID = 0;
   zCHAR szRightEntityName[ 33 ];
   zLONG lRightEntityID = 0;
   zCHAR szRightAttributeName[ 33 ];
   zLONG lRightAttributeID = 0;
   zLONG lTempID = 0;
   zCHAR szViewName[ 33 ];
   zCHAR szEntityName[ 33 ];
   zLONG lEntityID = 0;
   zCHAR szAttributeName[ 33 ];
   zLONG lAttributeID = 0;
   zCHAR szOperationName[ 33 ];
   zLONG lResultID = 0;
   zLONG lCantOptOut;   /* int flag to say we cant optimize because of ORs */
   zLONG lLoop = 0;                                      /* temp loop variable */
   zLONG lUseFirst = 0;
   zLONG lLth = 0;
   zLONG lOperator = 0;
   zVIEW vTargetView = NULL;
   zBOOL bSetNextJava = FALSE;

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );
// SetViewToSubobject( vTargetView, "Expression" );

   lResultID = AddResult( vSubtask );

   lCantOptOut = 0;

//*** Change Start  -  Don Christensen  -  3/02/94
// // loop to see if any ORs exist, if any do set the CantOptOut flag
// // to say that we cant optimize any statements
// for ( nLoop = 1; nLoop < g_lWhereIndex; nLoop++ )
// {
//    if ( g_lWhereJoin[ nLoop ] == qOR )
//       lCantOptOut = 1;
// }

   // The above code was commented out and the following added so that
   // the "opt" was done only when a single expression existed in the
   // WHERE.  This is because the code generated erroneously with
   // multiple expressions.
   // As the "optimized" code is generated with SetCursor...By... functions,
   //  and those functions are NOT working with object scope, we do not
   //  optimize, if we got "object scope".

   if ( g_lWhereIndex > 2 || g_szScopeEntityName[ 0 ] == '\1' )
      lCantOptOut = 1;

//*** Change End
   // if there are no ORs then we might possible be able to "opt" out some
   // calls if there is at least one cond equal check. Therefore, check to
   // see if there are any. If Not then we use the same logic as we would if
   // there were ORs so what the heck, check
   if ( lCantOptOut == 0 )
   {
      /* init nUseFirst to -1 */
      lUseFirst = -1;

      // try to find a AQ == somevalue so that we can do SetFirst .. by value.
      // if we find one, make sure that the AQ is within the controlling entity.
      // Remember the controlling entity is the one we are setting for. It
      // should be in szWO[ 0 ] and szWE[ 1 ] */
      for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
      {
         /* if the operator is ==, and obj is same, and cur is same,
            and ent is same and it is not an AQ == AQ */
         if ( g_lWhereOp[ lLoop ] == qCONDEQUAL &&
              zstrncmp( g_sWhereLeft[ 0 ].szText,
                        g_sWhereLeft[ lLoop ].szText,
                        (zSHORT)g_sWhereLeft[ 0 ].lOffset ) == 0 ) // len of EQ, attr irrelvnt
         {
            lUseFirst = lLoop;
            break;
         }

      }
      /* if lUseFirst == -1, then we cannot "promote" any equality to the cond
         equal position so do the same thing as was done for the statement that
         use ORs */
      if ( lUseFirst == -1 )
         lCantOptOut = 1;
   }

   /* case of all CompareAttributes.... cannot optimize out any statements */
   if ( lCantOptOut == 1 )
   {
      return( ForEachNoOpt( vSubtask, vTargetView, lResultID ) );
   }
   // /////////////////////////////////////////////////////////////////
   // else there were no OR conditions so we can go happy with optimizing
   // out some calls. Additionally, lUseFirst is set to the first possible
   // cond equal condition we can use.

   // if the g_lSetFNPL is each, then we need to put in a set first else all
   // we need to do is a CheckExistence.

   // since the first aq == lUseFirst.aq use nUse first to get attr
   SplitQualifier( g_sWhereLeft[ lUseFirst ].szText, szViewName, szEntityName, szAttributeName );
   GetVariableID( &g_lViewID, szViewName );

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
      {
         GetVariableID( &lRightEntityID, szRightEntityName );
      }
      else
         lRightEntityID = 0;

      if ( g_sWhereRight[ lUseFirst ].lDataClass == qATTRIBUTEISUNKNOWN ||
           g_sWhereRight[ lUseFirst ].lDataClass == qENTITYANDATTRIBUTEAREUNKNOWN )
      {
         GetVariableID( &lRightAttributeID, szRightAttributeName );
      }
      else
         lRightAttributeID = 0;

      // create the get and then put a get at the end...
      strcpy_s( szOperationName, zsizeof( szOperationName ), "Get" );

      switch ( g_sWhereRight[ lUseFirst ].lDataType )
      {
         case qTDECIMAL :
            g_sWhereRight[ lUseFirst ].lDataClass = qNUMERIC;
            strcat_s( szOperationName, zsizeof( szOperationName ), "DecimalFromAttribute" );
            break;
         case qTINTEGER :
         case qTSHORT   :
            g_sWhereRight[ lUseFirst ].lDataClass = qNUMERIC;
            strcat_s( szOperationName, zsizeof( szOperationName ), "IntegerFromAttribute" );
            break;
         default:
            g_sWhereRight[ lUseFirst ].lDataClass = qTEXT;
            strcat_s( szOperationName, zsizeof( szOperationName ), "StringFromAttribute" );
            break;
      }

      if ( lRightEntityID == 0 && lRightAttributeID == 0 )
      {
         lLth = GetLengthFromAttribute( lRightViewID,
                                        szRightEntityName, szRightAttributeName );
      }
      else
      if ( lEntityID == 0 && lAttributeID == 0 )
      {
         lLth = GetLengthFromAttribute( g_lViewID, szEntityName, szAttributeName );
      }
      else
      {
         lLth = -1;
      }

      if ( lLth == -1 )
         lLth = 1024;

      lTempID = GenerateTempVariable( vSubtask, g_sWhereRight[ lUseFirst ].lDataType, lLth );

      // cheat
      g_sWhereRight[ lUseFirst ].lElementClass = qVARIABLE;
      g_sWhereRight[ lUseFirst ].lOffset = lTempID;

      AddGetOrSetAttributeWithString( vSubtask, szOperationName, lRightViewID, 0,
                                      g_sWhereRight[ lUseFirst ].lDataType,
                                      g_sWhereRight[ lUseFirst ].lOffset,
                                      szRightEntityName, lRightEntityID,
                                      szRightAttributeName, lRightAttributeID,
                                      0, "", 0, 0 );
   }
   // this was above...
   if ( g_lSetFNPL == qEACH )
   {
     /* KJS - 09/04/12
      if ( g_szGenLang[ 0 ] == 'J' )
        // KJS 09/04/12 - Changing because we are now using SetCursorNextEntity instead of setNextContinue...
         //strcpy_s( szOperationName, zsizeof( szOperationName ), "setFirst" );
         strcpy_s( szOperationName, zsizeof( szOperationName ), "SetCursorFirstEntity" );
      else
      {
     */
         strcpy_s( szOperationName, zsizeof( szOperationName ), "SetCursorFirstEntity" );
         switch ( g_sWhereRight[ lUseFirst ].lDataClass )
         {
            case qNUMERIC:
               if ( g_sWhereRight[ lUseFirst ].lDataType == qTDECIMAL )
                  strcat_s( szOperationName, zsizeof( szOperationName ), "ByDecimal" );
               else
                  strcat_s( szOperationName, zsizeof( szOperationName ), "ByInteger" );
               break;

            case qTEXT:
               strcat_s( szOperationName, zsizeof( szOperationName ), "ByString" );
               break;
         }
      //}

      AddEntityOperWithScope( vSubtask, szOperationName, g_lViewID,
                              szEntityName, lEntityID,
                              g_sWhereRight[ lUseFirst ].lDataType,
                              szAttributeName, lAttributeID,
                              g_szScopeEntityName, g_lScopeEntityID,
                              g_sWhereRight[ lUseFirst ].lElementClass,
                              g_sWhereRight[ lUseFirst ].szText,
                              g_sWhereRight[ lUseFirst ].lOffset );
      if ( g_lWhereIndex > 2 )
      {
         AddResultCondEqual( vSubtask, qIF, "zCURSOR_UNCHANGED" );
         g_lNeedExtraEnd++;
      }
   }
   else
   {
      SplitQualifier( g_sWhereLeft[ lUseFirst ].szText, szViewName, szEntityName, szAttributeName );
      GetVariableID( &g_lViewID, szViewName );

      if ( g_sWhereLeft[ 0 ].lDataClass == qENTITYISUNKNOWN )
         GetVariableID( &lEntityID, szEntityName );
      else
         lEntityID = 0;

      AddEntityOperWithNoScope( vSubtask, "CheckExistenceOfEntity", g_lViewID,
                                szEntityName, lEntityID, 0, "", lResultID );
      if ( g_lWhereIndex > 2 )
      {
         AddResultCondEqual( vSubtask, qIF, "zCURSOR_UNCHANGED" );
         g_lNeedExtraEnd++;
      }
   }

   // if there was no positional indicator (EACH etc.) then put a compare
   // here to keep it out of the While
   if ( g_lSetFNPL == qNOEACH )  // *1/4/93*
   {
      strcpy_s( szOperationName, zsizeof( szOperationName ), "CompareAttributeTo" );

      if ( g_sWhereRight[ lUseFirst ].lElementClass == qATTRIBUTEQUALIFIER )
      {
         strcat_s( szOperationName, zsizeof( szOperationName ), "Attribute" );
      }
      else
      {
         switch( g_sWhereRight[ lUseFirst ].lDataType )
         {
            /* if the data type was String */
            case qTENTITYNAME    :
            case qTATTRIBUTENAME :
            case qTSTRING  :
            case qTCHARACTER :
               strcat_s( szOperationName, zsizeof( szOperationName ), "String" );
               break;

            /* if the data type was Integer */
            case qTINTEGER :
            case qTSHORT   :
               strcat_s( szOperationName, zsizeof( szOperationName ), "Integer" );
               break;

            /* if the data type was Decimal */
            case qTDECIMAL :
               strcat_s( szOperationName, zsizeof( szOperationName ), "Decimal" );
               break;

            default:
               break;
         }
      }

      lOperator = GetOppositeOperator( g_lWhereOp[ lUseFirst ] );
      AddCompareToView( vSubtask, vTargetView,
                        szOperationName,
                        &g_sWhereLeft[ lUseFirst ],
                        g_sWhereLeft[ lUseFirst ].szText,
                        &g_sWhereRight[ lUseFirst ],
                        g_sWhereRight[ lUseFirst ].szText,
                        lOperator );

      // Dump the expression from the stack.
      AddOperatorToView( vSubtask, vTargetView, -1 );

      AddStatementEntityToPI( vSubtask, qIF, qqlineno, vTargetView, zPOS_AFTER );

      strcpy_s( szOperationName, zsizeof( szOperationName ), "SetCursorNextEntity" );
      switch ( g_sWhereRight[ lUseFirst ].lDataClass )
      {
         case qNUMERIC:
            if ( g_sWhereRight[ lUseFirst ].lDataType == qTDECIMAL )
               strcat_s( szOperationName, zsizeof( szOperationName ), "ByDecimal" );
            else
               strcat_s( szOperationName, zsizeof( szOperationName ), "ByInteger" );
            break;

         case qTEXT:
            strcat_s( szOperationName, zsizeof( szOperationName ), "ByString" );
            break;
      }

      AddEntityOperWithScope( vSubtask, szOperationName, g_lViewID,
                              szEntityName, lEntityID,
                              g_sWhereRight[ lUseFirst ].lDataType,
                              szAttributeName, lAttributeID,
                              g_szScopeEntityName, g_lScopeEntityID,
                              g_sWhereRight[ lUseFirst ].lElementClass,
                              g_sWhereRight[ lUseFirst ].szText,
                              g_sWhereRight[ lUseFirst ].lOffset );
      AddStatementEntityToPI( vSubtask, qEND, qqlineno, vTargetView, zPOS_AFTER );
   }

   // begin building the WHILE
   // add result == 1

   //JEFF
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qLPAREN, 0 );
   if ( g_lSetFNPL == qCONSTANT || g_lSetFNPL == qVARIABLE )
   {
      AddResultAndLoopControlToView( vSubtask, vTargetView, "zCURSOR_UNCHANGED",
                                     g_lLoopControlID, qGREATERTHAN, "0" );
   }
   else
   {
      AddResultCondExprToView( vSubtask, vTargetView, qGREATERTHAN, "zCURSOR_UNCHANGED" );
   }

   if ( g_lWhereIndex > 2 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qAND, 0 );
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC, qTINTEGER, "", qLPAREN, 0 );
   }

   for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
   {
      if ( lLoop != lUseFirst ) // || g_lSetFNPL == qNOEACH )  // 1/4/93
      {
         // a compare to zero will be added based on the datatype of the
         // attribute or value that was given. This is stored in
         // cWLiteralType at this loop counter. First, however, we need
         // to change the operator to the opposite since we are doing the
         // "other" comparison
         strcpy_s( szOperationName, zsizeof( szOperationName ), "CompareAttributeTo" );

         if ( g_sWhereRight[ lLoop ].lElementClass == qATTRIBUTEQUALIFIER )
         {
            strcat_s( szOperationName, zsizeof( szOperationName ), "Attribute" );
         }
         else
         {
            switch( g_sWhereRight[ lLoop ].lDataType )
            {
               /* if the data type was String   */
               case qTENTITYNAME    :
               case qTATTRIBUTENAME :
               case qTSTRING  :
               case qTCHARACTER :
                  strcat_s( szOperationName, zsizeof( szOperationName ), "String" );
                  break;

               /* if the data type was Integer  */
               case qTINTEGER :
               case qTSHORT   :
                  strcat_s( szOperationName, zsizeof( szOperationName ), "Integer" );
                  break;

               /* if the data type was Decimal  */
               case qTDECIMAL :
                  strcat_s( szOperationName, zsizeof( szOperationName ), "Decimal" );
                  break;

               default:
                  break;
            }
         }

         AddCompareToView( vSubtask, vTargetView, szOperationName,
                           &g_sWhereLeft[ lLoop ], g_sWhereLeft[ lLoop ].szText,
                           &g_sWhereRight[ lLoop ], g_sWhereRight[ lLoop ].szText,
                           g_lWhereOp[ lLoop ] );

         if ( (g_lWhereJoin[ lLoop ] != 0) && (lLoop + 1 < g_lWhereIndex) )
         {
            AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                                     qTINTEGER, "", qAND, 0 );
         }
      }
   }

   if ( g_lWhereIndex > 2 )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                               qTINTEGER, "", qRPAREN, 0 );
   }

   //JEFF
   AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0, qTINTEGER, "", qRPAREN, 0 );

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vTargetView, -1 );

   AddStatementEntityToPI( vSubtask, qWHILE, qqlineno, vTargetView, zPOS_AFTER );

   /*
   if ( g_szGenLang[ 0 ] == 'J' )
   {
      bSetNextJava = TRUE;
     // KJS 09/04/12 - Changing because we are now using SetCursorNextEntity instead of setNextContinue...
      //strcpy_s( szOperationName, zsizeof( szOperationName ), "setNextContinue" );
      strcpy_s( szOperationName, zsizeof( szOperationName ), "SetCursorNextEntity" );
   }
   else
   {
   */
      strcpy_s( szOperationName, zsizeof( szOperationName ), "SetCursorNextEntity" );
      switch ( g_sWhereRight[ lUseFirst ].lDataClass )
      {
         case qNUMERIC:
            if ( g_sWhereRight[ lUseFirst ].lDataType == qTDECIMAL )
               strcat_s( szOperationName, zsizeof( szOperationName ), "ByDecimal" );
            else
               strcat_s( szOperationName, zsizeof( szOperationName ), "ByInteger" );
            break;

         case qTEXT:
            strcat_s( szOperationName, zsizeof( szOperationName ), "ByString" );
            break;
      }
   //}

   /*
   AddEntityOperWithScope( vSubtask, szOperationName, g_lViewID,
                           szEntityName, lEntityID,
                           g_sWhereRight[ lUseFirst ].lDataType,
                           szAttributeName, lAttributeID,
                           bSetNextJava ? "" : g_szScopeEntityName,
                           bSetNextJava ? 0 : g_lScopeEntityID,
                           g_sWhereRight[ lUseFirst ].lElementClass,
                           g_sWhereRight[ lUseFirst ].szText,
                           g_sWhereRight[ lUseFirst ].lOffset );
   */
   AddEntityOperWithScope( vSubtask, szOperationName, g_lViewID,
                           szEntityName, lEntityID,
                           g_sWhereRight[ lUseFirst ].lDataType,
                           szAttributeName, lAttributeID,
                           g_szScopeEntityName,
                           g_lScopeEntityID,
                           g_sWhereRight[ lUseFirst ].lElementClass,
                           g_sWhereRight[ lUseFirst ].szText,
                           g_sWhereRight[ lUseFirst ].lOffset );

   if ( g_lSetFNPL == qCONSTANT || g_lSetFNPL == qVARIABLE )
      CreateIntegerExpression( vSubtask, g_lLoopControlID, qEQUAL,
                               qVARIABLE, g_lLoopControlID, qMINUS,
                               qCONSTANT, "1" );

   ReuseTempVariable( lTempID );
   g_lAddNextStatementBefore++;
   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
// DisplayObjectInstance( vTargetView, "", "" );
// MessageSend( vTargetView, "???", "InsertForWithWhere", "vTargetView", 0, 0 );
   DropView( vTargetView );
   return( 0 );
}

// generate FOR EACH without optimization
static zSHORT
ForEachNoOpt( zVIEW vSubtask, zVIEW vTargetView, zLONG lResultID )
{
   zCHAR szAttributeName[ 33 ];
   zCHAR szViewName[ 33 ];
   zCHAR szOperationName[ 33 ];
   zLONG lEntityID;
   zLONG lLoop;
   zLONG lRC = 0;

   /* insert the correct CoreOperation call depending on whether or
   not we are setting EACH NEXT VAR CONST */
   SplitQualifier( g_sWhereLeft[ 0 ].szText, szViewName,
                   g_szEntityName, szAttributeName );

   GetVariableID( &g_lViewID, szViewName );

   if ( g_sWhereLeft[ 0 ].lDataClass == qENTITYISUNKNOWN )
      GetVariableID( &lEntityID, g_szEntityName );
   else
      lEntityID = 0;

   switch ( g_lSetFNPL )
   {
      case qEACH:
         GenerateSetCursorWithScope( vSubtask, zPOS_FIRST );
         // before we put in the first compare, put in the RESULT == 0
         // so that if we hit the last entity and the condition is not
         // true, we get out quicker
         break;

      case qNOEACH:
         AddEntityOperWithNoScope( vSubtask, "CheckExistenceOfEntity", g_lViewID,
                                   g_szEntityName, lEntityID, 0, "", lResultID );

         // before we put in the first compare, put in the RESULT == 0
         // so that if we hit the last entity and the condition is not
         // true, we get out quicker
         break;

      case qVARIABLE:
      case qCONSTANT:
         AddEntityOperWithNoScope( vSubtask, "CheckExistenceOfEntity", g_lViewID,
                                   g_szEntityName, lEntityID, 0, "", lResultID );
         // before we put in the first compare, put in the RESULT == 0
         // so that if we hit the last entity and the condition is not
         // true, we get out quicker
         // if there was a constant or a SYMBOL in the loop expression,
         //  then add the check for it here
         AddResultAndLoopControlToView( vSubtask, vTargetView, "zCURSOR_NULL",
                                        g_lLoopControlID, qGREATERTHAN, "0" );
         // dont add the expression yet
         break;

      default :
         break;
   }

   AddResultCondEqual( vSubtask, qWHILE, "zCURSOR_UNCHANGED" );

   // here we begin adding the comparisons to create the while loop. For
   // each where condition parsed, do a CompareAttributeToXXXXXX where
   // XXXXX is the datatype of the comparison OR another Attribute.

   InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, qLPAREN, 0, "", 0, zPOS_BEFORE );

   for ( lLoop = 1; lLoop < g_lWhereIndex; lLoop++ )
   {
      zLONG i;

      // insert opening parens, if necessary
      for ( i = 0; i < g_sWhereLeft[ lLoop ].lLevel; i++ )
      {
         AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                                  qTINTEGER, "", qLPAREN, 0 );
      }

      // a compare to zero will be added based on the datatype of the
      // attribute or value that was given. This is stored in
      // .DataType at this loop counter. First, however, we need
      // to change the operator to the opposite since we are doing the
      // "other" comparison
      strcpy_s( szOperationName, zsizeof( szOperationName ), "CompareAttributeTo" );

      if ( g_sWhereRight[ lLoop ].lElementClass == qATTRIBUTEQUALIFIER )
         strcat_s( szOperationName, zsizeof( szOperationName ), "Attribute" );
      else
      {
         switch( g_sWhereRight[ lLoop ].lDataType )
         {
            /* if the data type was String   */
            case qTENTITYNAME  :
            case qTATTRIBUTENAME :
            case qTSTRING  :
            case qTCHARACTER :
               strcat_s( szOperationName, zsizeof( szOperationName ), "String" );
               break;

            /* if the data type was Integer  */
            case qTINTEGER :
            case qTSHORT   :
               strcat_s( szOperationName, zsizeof( szOperationName ), "Integer" );
               break;

            /* if the data type was Decimal  */
            case qTDECIMAL :
               strcat_s( szOperationName, zsizeof( szOperationName ), "Decimal" );
               break;

            default:
               // Error
               SyntaxError( vSubtask, eQINVALIDSYNTAXINWHERE, "");
               DropView( vTargetView );
               return( -1 );
         } // end switch
      }

      lRC = AddCompareToView( vSubtask, vTargetView, szOperationName,
                              &g_sWhereLeft[ lLoop ], g_sWhereLeft[ lLoop ].szText,
                              &g_sWhereRight[ lLoop ], g_sWhereRight[ lLoop ].szText,
                              g_lWhereOp[ lLoop ] );
      if ( lRC < 0 )
      {
         // Error
         SyntaxError( vSubtask, eQINVALIDSYNTAXINWHERE, "");
         DropView( vTargetView );
         return( -1 );
      }

      // insert closing parens, if necessary
      for ( i = 0; i < g_sWhereRight[ lLoop ].lLevel; i++ )
      {
         AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                                  qTINTEGER, "", qRPAREN, 0 );
      }

      // now check for a JOIN (AND or OR)
      if ( g_lWhereJoin[ lLoop ] != 0 )
         AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                                  qTINTEGER, "", g_lWhereJoin[ lLoop ], 0 );
   } // end for

   // Dump the expression from the stack.
   AddOperatorToView( vSubtask, vTargetView, -1 );

   InsertExprNodeValuesToEntity( vTargetView, qOPERATOR, 0, qTINTEGER, qRPAREN, 0, "", 0, zPOS_AFTER );

   // add the while statement to do al of these compares...
   AddStatementEntityToPI( vSubtask, qIF, qqlineno, vTargetView, zPOS_AFTER );

   AddStatementEntityToPI( vSubtask, qEND,   // IF
                           qqlineno, vTargetView, zPOS_AFTER );

   // Add the FORIF Statement entry which will be used in the
   // routine, INSERTENDSTMT, for testing returning levels.
   CreateEntity ( g_vStatementView, "Statement", zPOS_AFTER );
   SetAttributeFromInteger( g_vStatementView, "Statement", "Class", qFORIF );

   //*** Change End

   // Add a SetCursorNext for the loop.
   GenerateSetCursorWithScope( vSubtask, zPOS_NEXT );

   // If it was a constant loop, then decrement the loop counter.
   if ( g_lSetFNPL == qCONSTANT || g_lSetFNPL == qVARIABLE )
   {
      CreateIntegerExpression( vSubtask, g_lLoopControlID, qEQUAL, qVARIABLE, g_lLoopControlID, qMINUS, qCONSTANT, "1" );
   }

   SetCursorPrevEntity( g_lpPIView, "Statement", "" );

   // Back up to IF statement.
   SetCursorPrevEntity ( g_vStatementView, "Statement", "" );
   SetCursorPrevEntity ( g_vStatementView, "Statement", "" );
   SetViewToSubobject( g_vStatementView, "Substatement" );

   //*** Change End
   g_lAddNextStatementBefore++;
   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
// DisplayObjectInstance( vTargetView, "", "" );
// MessageSend( vTargetView, "???", "ForEachNoOpt", "vTargetView", 0, 0 );
   DropView( vTargetView );
   return( 0 );
}
