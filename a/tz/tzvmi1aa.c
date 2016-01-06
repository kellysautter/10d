/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmi1aa.c - VML SubSystem Interpretor Main Module
// DESCRIPTION:   This is the source file which contains the Setup functions
//                for the Interpreter. It contains the functions to do all
//                of the setup.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/11/06
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: VM03101
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//    1994/02/03: Revised by Don Christensen
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2003.03.11 DC
   Deleted the DropView( vXPG) in RestoreLastOperCall so that the view could
   be cached back at the beginning of the call path.

2001.08.10 HH
   R55612, improve workspace for variables, WRKS system.

2000.08.09 RG
   Removed compiler warnings

2000.07.18 HH
   fix for multiple interpreter entry.

2000.06.16 HH
   fix for "UNTIL".

05.08.1997  DonC
   Added qOF and qOTHERWISE cases to properly handle the interpretation
   of CASE statements.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmlopr.hg"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

zOPER_EXPORT zLONG OPERATION
RestoreLastOperCall( zVIEW lpInterpSubtask,
                     zVIEW lpAppSubtask );

zLONG LOCALOPER
fnGetStackValue( zSHORT nIdx );

//
// OPERATION: StepOnce
//
// DESCRIPTION: StepOnce is the main execution function of the interpretor
//    Everything to execute code is done from here. First we get the interp
//    subtask. From which we get several views. the xpg, statement, and
//    operation list views from the interpretor subtask. We then get the
//    class of the Statement. The code associated with the statement is
//    executed and flow of control is handled based on that. We must RE-get
//    all of the views on entry as the DLL may bnot stay around.  This is
//    true if the debugger is calling the interpretor.
//
zOPER_EXPORT zLONG OPERATION
StepOnce( zVIEW vSubtask, zPVOID hWRKS, zPLONG plRetVal )
{
   zVIEW  lpInterpSubtask;
   zLONG  lEvaluationCode;
   union { zLONG    lEvaluationResult;
           zDECIMAL dEvaluationResult;
         } u;
   zSHORT nNextStatement;
   zLONG  lClass;
   zLONG  lNextClass;
   zCHAR  szParentName[ 32 ];
   zCHAR  szIfConditionResult[ 2 ];
   zSHORT nRC;

   // Get the  interp subtask
   nRC = GetViewByName( &lpInterpSubtask, "ZeidonVML", vSubtask, zLEVEL_TASK );

   // Get the XPG View
   nRC = GetViewByName( &g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );

   // get the statement view
   nRC = GetViewByName( &g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );

   // get the class of the statement
   GetIntegerFromAttribute( &lClass, g_vStatementView, "Statement", "Class" );

   // branch based on the Class of the statement
   switch( lClass )
   {
      // if we get an IF Statement, then we need to evaluate the expression.
      // If EvaluateExpression returns a non-zero, then the expression
      // evaluated True. Thesefore we SetHandle to the Statement subobject
      // view and exec all of these statements ( on future calls to StepOnce).
      // if the expr evaluated false, then we skip on down to the ELSE case.
      case qIF:
         // evaluate the expression
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );

         // if was true
         if ( u.lEvaluationResult != 0 )
         {
            // Indicate IF evaluated true.
            SetAttributeFromString( g_vStatementView, "Statement",
                                    "IfConditionResult", "T" );

            // set view to the subobject so that all we see are statements.
            nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );

            // besure we are at the first statement incase we have been here
            // before.
            nNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );

            // do a break here so as to not go into the else
            lEvaluationCode = zXC_STEP_EXECUTED;
            break;
         }
         else // was false. Try to set to else if there is one
         {
            // Indicate IF evaluated false.
            SetAttributeFromString( g_vStatementView, "Statement",
                                    "IfConditionResult", "F" );

            // the statement was false. Therefore we want to to do the
            // else portion. However, we need to be sure that the next stmt
            // is an else. If not, then we need to break.
            nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );

            // get the class
            GetIntegerFromAttribute( &lNextClass, g_vStatementView, "Statement", "Class" );

            // if not an else then break.
            if ( lNextClass != qELSE )
            {
               lEvaluationCode = zXC_STEP_EXECUTED;
            }
            else
            {
               // This else code is much the same as the code for the ELSE
               // statement below.
               nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );

               // be sure we are at the first statement in case we have been here before.
               nNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
               lEvaluationCode = zXC_STEP_EXECUTED;
            }
         }

         break;

      case qELSE:
         // Execute this code only if the previous IF condition was false.
         // Set cursor back to the if and then forward again to the else.
         SetCursorPrevEntity( g_vStatementView, "Statement", 0 );
         GetStringFromAttribute( szIfConditionResult, sizeof( szIfConditionResult ), g_vStatementView, "Statement", "IfConditionResult" );
         SetCursorNextEntity( g_vStatementView, "Statement", 0 );

         if ( szIfConditionResult[ 0 ] == 'F' )
         {
            nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );

            // be sure we are at the first statement incase we have been here before.
            nNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
            lEvaluationCode = zXC_STEP_EXECUTED;
         }
         else
         {
            nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
            lEvaluationCode = zXC_STEP_EXECUTED;
         }

         break;

      // THIS IS NOT DONE.
      case qCASE:
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );
         nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      case qOF:
         g_nStackPtr++;
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );

         if ( fnGetStackValue( g_nStackPtr ) == fnGetStackValue( (zSHORT) (g_nStackPtr - 1) ) )
         {
            nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
         }
         else
         {
            nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
         }

         g_nStackPtr--;
         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      case qOTHERWISE:
         nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      // if we have a LOOP (do) just go the subobject view a there is no
      // expression to evaluate
      case qLOOP:
         nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );
         // Be sure we are at the first statement in case we have been here before.
         nNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      // if we have an until then we need to evaluate the expression of that
      // until. and reset back up to the LOOP. If the expression evaluated
      // TRUE then go on to the next statement. Otherwise, stay on the loop
      // and the next step will put up back into the loop
      case qUNTIL:
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );
         // get back to loop level.
         nNextStatement = ResetViewFromSubobject( g_vStatementView );

         // UNTIL x = WHILE NOT x
         // lEvaluationResult is the result of (NOT x)
         // if (NOT x) is  NOT TRUE, then continue.
         // go back 1 statement, the LOOP statement
         if ( u.lEvaluationResult != 0 )
            nNextStatement = SetCursorPrevEntity( g_vStatementView, "Statement", "" );

         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      // if we are on a while, evluate the expression. If true set to the
      // subobject view. If False, go merrily on our way.
      case qWHILE:
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );
         if ( u.lEvaluationResult != 0 ) // if was true
         {
            // go down
            nNextStatement = SetViewToSubobject( g_vStatementView, "Substatement" );

            // Be sure we are at the first statement in case we have been here before.
            nNextStatement = SetCursorFirstEntity( g_vStatementView, "Statement", "" );
         }
         else
         {
            // was false so skip the loop code and go on.
            nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
         }

         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      // For a return, simply evaluate the expression and go to the next statement
      // which would have to be an end. This is assured
      // via the syntax the value of the return will be on the top of the
      // value stack
      case qRETURN:
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );
         MiGetParentEntityNameForView( szParentName, g_vStatementView, "Statement" );
         nRC = zstrcmp( "Operation", szParentName );
         while ( nRC != zCURSOR_SET )
         {
            nRC = ResetViewFromSubobject( g_vStatementView );

            if ( nRC == zCURSOR_SET )
            {
               MiGetParentEntityNameForView( szParentName, g_vStatementView, "Statement" );
               nRC = zstrcmp( "Operation", szParentName );
            }
            else
               break; // some sort of error
         }

         nNextStatement = SetCursorLastEntity( g_vStatementView, "Statement", "" );

         // callStageOperation Return to "Re-setup things" as they were.
         lEvaluationCode = RestoreLastOperCall( lpInterpSubtask, vSubtask );

         // A negative return code indicates the end of execution.
         // Therefore, terminate execution.
         if ( lEvaluationCode == -1 )
         {
            // As the top level operation must be of type dialog,
            //  the return value is integer (short)
            *plRetVal = u.lEvaluationResult;
            return( zXC_OPERATION_EXECUTED );
         }

         // Get the statement view as it probably changed, and set cursor
         // back one to the statement calling the operation that we just
         // executed so that we can finish executing the expression.
         GetViewByName( &g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );

         return( zXC_STEP_EXECUTED );
         break;

      // the END . reset the view back to the parent. If the Parent is a
      // WHILE then stay there and let the next iteration re-evaluate the
      // while condition. Otherwise go on to the next statement.
      case qEND:
         // get back to the parent.
         nNextStatement = ResetViewFromSubobject( g_vStatementView );

         // if it was a success, then get the class of the parent.
         if ( nNextStatement == zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lClass, g_vStatementView, "Statement", "Class" );
            // if not a while go on.
            if ( lClass != qWHILE )
               nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
         }

         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      // break just comes out one level and goes to the next statement.
      case qBREAK:
         nNextStatement = ResetViewFromSubobject( g_vStatementView );
         if ( nNextStatement == zCURSOR_SET )
         {
            nNextStatement = SetCursorLastEntity( g_vStatementView, "Statement", "" );
         }

         lEvaluationCode = zXC_STEP_EXECUTED;
         break;

      // just evalute the expression of the expression staement and step next.
      // the simplest of all
      case qEXPRESSION:
         // and it says..
         lEvaluationCode = EvaluateExpression( &u, lpInterpSubtask, vSubtask,
                                               g_vStatementView, hWRKS );

         // Step to the next instruction, unless we are about to
         // interpret a new operation.  In the latter case, simulate
         // that the set cursor was ok so that later code does not
         // think we were at the last statement under a parent.
         if ( lEvaluationCode == zXC_SETUP_DEBUGGER || lEvaluationCode == zXC_OPERATION_STAGED )
            nNextStatement = zCURSOR_SET;
         else
            nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );

         break;

      // skip the unknown or default condition
      case qTUNKNOWN:
      default:
         nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
         break;
   }

   // if the previous statement was the last statement under its parent
   // statement, then we want to simply reset up a level.  If we can't
   // reset up a level, then there must be an error, as we shouldn't be
   // able to get to the top without hitting a RETURN statement.  We're
   // not bothering to check for the error.
   if ( nNextStatement != zCURSOR_SET )
   {
      nNextStatement = ResetViewFromSubobject( g_vStatementView );

      // Also position on the next statement
      nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );

      GetIntegerFromAttribute( &lClass, g_vStatementView, "Statement", "Class" );
      if ( lClass == qELSE ) // set another one for else
         nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
   }

   return( lEvaluationCode );
}

//
// OPERATION: Skip
//
// DESCRIPTION: Skips a statement All it really does is set cursor past the
//    statement that would next be executed.
//
//
zOPER_EXPORT zLONG OPERATION
Skip( zVIEW  vSubtask )
{
   //zVIEW  vStatementView;
   zSHORT nNextStatement;
   zVIEW  lpInterpSubtask;

   // Get the interp subtask.
   GetViewByName( &lpInterpSubtask, "ZeidonVML", 0, zLEVEL_TASK );

   // get the statement view from the interp subtask
   GetViewByName( &g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );

   // try to set cursor to the next entity as the one we are positioned on
   // is the one that would be executed next.
   nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );

   // if the next statement could not be set to try and reset from the
   // subobject view
   if ( nNextStatement != zCURSOR_SET )
   {
      // try and reset.
      nNextStatement = ResetViewFromSubobject( g_vStatementView );

      // if we did, then set next from here.
      if ( nNextStatement == zCURSOR_SET )
      {
         nNextStatement = SetCursorNextEntity( g_vStatementView, "Statement", "" );
      }
   }

   // if it was good then return a zero
   if ( nNextStatement == zCURSOR_SET )
      return( zXC_STEP_EXECUTED );
   else
      return( zXC_OPERATION_EXECUTED );
}

//
// OPERATION: Go
//
// DESCRIPTION: Go just calls StepOnce until there are no more
// statements or until an operation is loaded which is to be debugged.
// In the latter case, it sets a flag in the XPG instance indicating
// that the operation is in the process of being debugged.
//
// At the beginning it turns off the current debug flag.
//
zOPER_EXPORT zLONG OPERATION
Go( zVIEW  vSubtask, zPVOID hWRKS, zPLONG plRetVal )
{
   zVIEW  vZ_View;
   zVIEW  vX_View;
   zVIEW  vS_View;
   zLONG  lStepRC;
   zLONG  lRC;

   GetViewByName( &vZ_View, "ZeidonVML", vSubtask, zLEVEL_TASK );
   GetViewByName( &vX_View, "XPG", vZ_View, zLEVEL_SUBTASK );
   SetAttributeFromString( vX_View, "Operation", "CurrentDebugFlag", "" );

   lStepRC = zXC_STEP_EXECUTED;
   lRC = 1;
   while ( (lStepRC == zXC_STEP_EXECUTED || lStepRC == zXC_OPERATION_STAGED ) && lRC != 0 )
   {
      *plRetVal = 0;
      lStepRC = StepOnce( vSubtask, hWRKS, plRetVal );

      if ( lStepRC == zXC_STEP_EXECUTED )
      {
         GetViewByName( &vS_View, "StatementView", vZ_View, zLEVEL_SUBTASK );
         lRC = CompareAttributeToString( vS_View, "Statement", "BreakPointFlag", "Y" );
      }
   }

   if ( lStepRC == zXC_SETUP_DEBUGGER || lRC == 0 )
   {

      GetViewByName( &vS_View, "XPG", vZ_View, zLEVEL_SUBTASK );

      SetAttributeFromString( vS_View, "Operation", "CurrentDebugFlag", "Y" );
   }

   return( lStepRC );

}

zOPER_EXPORT zSHORT OPERATION
StageOperationCall( zVIEW  lpInterpSubtask,
                    zVIEW  vSubtask,
                    zPCHAR szModuleName,
                    zPCHAR szOperName )
{
   zVIEW  vCalledXPGView;
   zSHORT nRC;

   // get the statement view
   GetViewByName( &g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );

   // Update was here
   DropNameForView( g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );
   DropNameForView( g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   nRC = CompareAttributeToString( g_vXPGView, "VML_XSource", "Name", szModuleName );
   if ( nRC )
   {
      nRC = LoadXPG( lpInterpSubtask, vSubtask, szModuleName, "C" );
   }
// else
   {
      CreateViewFromViewForTask( &vCalledXPGView, g_vXPGView, 0 );
      nRC = SetCursorFirstEntityByString( vCalledXPGView, "Operation",
                                          "Name", szOperName, "" );
      if ( nRC != zCURSOR_SET )
      {
         zCHAR szMsg[ 32 ];

         strcpy_s( szMsg, sizeof( szMsg ), szOperName );
         strcat_s( szMsg, sizeof( szMsg ), " could not be located in the VML code, re-parse" );
         MessageSend( vSubtask, "VM03101", "VML Interpretor",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }

      SetCursorFirstEntityByEntityCsr( vCalledXPGView, "OperationSource",
                                       vCalledXPGView, "Operation", "" );
      SetCursorFirstEntity( vCalledXPGView, "OperationText", "" );
      SetNameForView( vCalledXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   }

   GetViewByName( &g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   SetCursorFirstEntity( g_vXPGView, "Statement", "" );
   SetCursorFirstEntity( g_vXPGView, "Variable", "" );

   CreateViewFromViewForTask( &g_vStatementView, g_vXPGView, 0 );

   SetNameForView( g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );

   SetStackObjectFromViews( lpInterpSubtask, g_vXPGView, g_vStatementView, 0 );
   GetViewByName( &g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   return( 2 );
}

zOPER_EXPORT zLONG OPERATION
RestoreLastOperCall( zVIEW lpInterpSubtask,
                     zVIEW  vSubtask )

{
   zVIEW  vStackObject;                  // view to the stack object
   zVIEW  vXPG;
   zSHORT nStartStackPtr;
   zLONG  lZKey;
   zLONG  lRC;

   // Get the stack object as it was loaded with the XPG
   GetViewByName( &vStackObject, "StackObject", lpInterpSubtask, zLEVEL_SUBTASK );
   lRC = SetCursorLastEntity( vStackObject, "OperationCall", "" );

   if ( lRC != zCURSOR_SET )
      return( -1 );   // return( zXC_OPERATION_EXECUTED );

   GetViewByName( &g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vXPG, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   DropView( g_vStatementView );
// DropView( vXPG );

   if ( SetCursorPrevEntity( vStackObject, "OperationCall", "" ) != zCURSOR_SET )
   {
      // we are on the first Operation in stack object
      DeleteEntity( vStackObject, "OperationCall", zREPOS_NONE );
      return( -1 );
   }

   GetIntegerFromAttribute( &lZKey,  // temp use of lZKey
                            vStackObject,
                            "OperationCall",
                            "StackOffset" );
   nStartStackPtr = (zSHORT) lZKey;

   SetCursorNextEntity( vStackObject, "OperationCall", "" );
   SetPointerValuesFromLocalCall( nStartStackPtr, lpInterpSubtask );
   DeleteEntity( vStackObject, "OperationCall", zREPOS_BEFORE );

   g_nCallStack[ 0 ]--;
   SetPointerValues( &nStartStackPtr, lpInterpSubtask );

   // Add one to g_nStackPtr so that the Operate function stores the
   // return code correctly.
   IncrementStackPtr ( );
   //g_nStackPtr = g_nCallStack[ g_nCallStack[ 0 ] ];

   g_nCallStack[ g_nCallStack[ 0 ] ] = 0; // ??

   //g_nStackPtr = nStartStackPtr; // 7/27/93

   GetIntegerFromAttribute( (zPLONG) &g_vXPGView, vStackObject,
                                 "OperationCall", "XPGView" );
   GetIntegerFromAttribute( (zPLONG) &g_vStatementView,
                                 vStackObject, "OperationCall",
                                 "StatementView" );
   SetNameForView( g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   SetNameForView( g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );

   GetIntegerFromAttribute( &lZKey, g_vXPGView, "Operation", "ZKey" );
   lRC = GetExecModeByZKey( lpInterpSubtask, vSubtask, lZKey );
   if ( lRC == zXC_SETUP_DEBUGGER )
      return( lRC );

   return( zXC_OPERATION_EXECUTED );
}

#ifdef __cplusplus
}
#endif
