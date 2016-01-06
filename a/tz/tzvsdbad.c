/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   TZVSDBAD.C
// DESCRIPTION:   This module contains the operations for the
//                Zeidon VML Debugger dialog (see TZVSDBAD.PWD)
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
==========
2002.02.19 HH
   adaption to making StackObject.Variable.Value BLOB.
*/

#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZeidonOp.H"

// LAST MESSAGE ID: VS00201

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_BuildVar( zVIEW    vSubtask )
{
   zVIEW    vStackObject;
   zVIEW    lpZeidonVML;
   zSHORT   nZRetCode;

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &vStackObject, "StackObject", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( vStackObject, "StackObject", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = CreateTemporalSubobjectVersion( vStackObject, "Variable" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_Resume( zVIEW    vSubtask )
{
   zVIEW    vAppSubtask;
   zVIEW    lpZeidonVML;
   zSHORT   nZRetCode;

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &vAppSubtask, "ApplicationView", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = Exec_ResumeFromDebugger( vSubtask, "", vAppSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_Exit( zVIEW    ViewtoWindow )
{
   zSHORT   nZRetCode;

   nZRetCode = zwTZVSDBAD_Resume( ViewtoWindow );
   nZRetCode = SetWindowActionBehavior( ViewtoWindow, zWAB_ReturnToParentWithRefresh, "", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_FindStatement( zVIEW    XPG2,
                          zVIEW    XPG )
{
   zSHORT   StatementFound;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zSHORT   nZRetCode;

   RESULT = SetCursorFirstEntity( XPG2, "Statement", "" );
   StatementFound = 0;
   while ( StatementFound == 0 && RESULT == zCURSOR_SET )
   {
      lTempInteger_0 = CheckExistenceOfEntity( XPG2, "Substatement" );
      if ( lTempInteger_0 == 0 )
      {
         nZRetCode = SetViewToSubobject( XPG2, "Substatement" );
         StatementFound = zwTZVSDBAD_FindStatement( XPG2, XPG );
         nZRetCode = ResetViewFromSubobject( XPG2 );
      }
      if ( StatementFound == 0 )
      {
         if ( CompareAttributeToAttribute( XPG2, "Statement", "RelativeLineNumber", XPG, "OperationText", "RelativeLineNumber" ) == 0 )
         {
            StatementFound = 1;
            if ( CompareAttributeToString ( XPG, "OperationText", "BreakPointFlag", ">" ) == 0 )
            {
               nZRetCode = SetAttributeFromString( XPG2, "Statement", "BreakPointFlag", "" );
               nZRetCode = SetAttributeFromString( XPG, "OperationText", "BreakPointFlag", "" );
            }
            else
            {
               nZRetCode = SetAttributeFromString( XPG2, "Statement", "BreakPointFlag", "Y" );
               nZRetCode = SetAttributeFromString( XPG, "OperationText", "BreakPointFlag", ">" );
            }
         }
         else
         {
            RESULT = SetCursorNextEntity( XPG2, "Statement", "" );
         }
      }

      lTempInteger_0 = CheckExistenceOfEntity( XPG2, "Statement" );
   }

   return( StatementFound );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_Go( zVIEW    vSubtask )
{
   zVIEW    XPG;
   zVIEW    vStatementView;
   zVIEW    lpZeidonVML;
   zLONG    lStepCode;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   lStepCode = Exec_Go( vSubtask );
   if ( lStepCode == zXC_OPERATION_EXECUTED )
   {
      nZRetCode = zwTZVSDBAD_Resume( vSubtask );
      nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, "", "" );
      return( -1 );
   }

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( XPG, "XPG", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vStatementView, "StatementView", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vStatementView, "Statement", "RelativeLineNumber" );
   RESULT = SetCursorFirstEntityByInteger( XPG, "OperationText", "RelativeLineNumber", lTempInteger_0, "" );
   nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, "", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_InitDebugger( zVIEW    vSubtask )
{
   zVIEW    vXPGView;
   zVIEW    vXPG;
   zVIEW    vZeidonStatementView;
   zVIEW    lpZeidonVML;
   zSHORT   nZRetCode;

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &vXPGView, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( vXPGView, "XPG", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vZeidonStatementView, "StatementView", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( vZeidonStatementView, "StatementView", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vXPG, "StackObject", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( vXPG, "StackObject", vSubtask, zLEVEL_SUBTASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_PostInitDebugger( zVIEW    vSubtask )
{
   zVIEW    XPG;
   zVIEW    vStatementView;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   nZRetCode = GetViewByName( &XPG, "XPG", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vStatementView, "StatementView", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vStatementView, "Statement", "RelativeLineNumber" );
   RESULT = SetCursorFirstEntityByInteger( XPG, "OperationText", "RelativeLineNumber", lTempInteger_0, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_SetBreakPoint( zVIEW    vSubtask )
{
   zVIEW    XPG;
   zVIEW    XPG2;
   zVIEW    lpZeidonVML;
   zLONG    FoundFlag;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = CreateViewFromViewForTask( &XPG2, XPG, 0 );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, XPG2, "OperationSource", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( XPG2, "Operation", "ZKey", lTempInteger_0, "" );
   FoundFlag = zwTZVSDBAD_FindStatement( XPG2, XPG );
   if ( FoundFlag == 1 )
   {
      nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, "", "" );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_Skip( zVIEW    vSubtask )
{
   zVIEW    XPG;
   zVIEW    lpZeidonVML;
   zVIEW    vStatementView;
   zLONG    lStepCode;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   lStepCode = Exec_Skip( vSubtask );
   if ( lStepCode == zXC_OPERATION_EXECUTED )
   {
      nZRetCode = zwTZVSDBAD_Resume( vSubtask );
      nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, "", "" );
      return( -1 );
   }

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( XPG, "XPG", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vStatementView, "StatementView", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vStatementView, "Statement", "RelativeLineNumber" );
   RESULT = SetCursorFirstEntityByInteger( XPG, "OperationText", "RelativeLineNumber", lTempInteger_0, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_SpawnObjectBrowser( zVIEW    vSubtask )
{
   zVIEW    StackObject;
   zVIEW    lpZeidonVML;
   zVIEW    v;
   zLONG    lDataLth;
   zSHORT   nZRetCode;

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &StackObject, "StackObject", lpZeidonVML, zLEVEL_SUBTASK );
   lDataLth = 4;
   GetBlobFromAttribute( &v, &lDataLth,
                         StackObject, "Variable", "Value" );
   if ( v != 0 )
   {
      nZRetCode = StageBrowser( vSubtask, v );
   }
   if ( nZRetCode == -1 )
   {
      MessageSend( vSubtask, "VS00201", "Launcher",
                   "View is not yet valid",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_StepForward( zVIEW    vSubtask )
{
   zVIEW    XPG;
   zVIEW    lpZeidonVML;
   zVIEW    vStatementView;
   zLONG    lStepCode;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   lStepCode = Exec_StepForward( vSubtask );
   if ( lStepCode == zXC_OPERATION_EXECUTED )
   {
      nZRetCode = zwTZVSDBAD_Resume( vSubtask );
      nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, "", "" );
      return( -1 );
   }

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( XPG, "XPG", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vStatementView, "StatementView", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vStatementView, "Statement", "RelativeLineNumber" );
   RESULT = SetCursorFirstEntityByInteger( XPG, "OperationText", "RelativeLineNumber", lTempInteger_0, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_StepOnce( zVIEW    vSubtask )
{
   zVIEW    XPG;
   zVIEW    lpZeidonVML;
   zVIEW    vStatementView;
   zLONG    lStepCode;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   lStepCode = Exec_StepOnce( vSubtask );
   if ( lStepCode == zXC_OPERATION_EXECUTED )
   {
      nZRetCode = zwTZVSDBAD_Resume( vSubtask );
      nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, "", "" );
      return( -1 );
   }

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = SetNameForView( XPG, "XPG", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &vStatementView, "StatementView", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vStatementView, "Statement", "RelativeLineNumber" );
   RESULT = SetCursorFirstEntityByInteger( XPG, "OperationText", "RelativeLineNumber", lTempInteger_0, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSDBAD_Update( zVIEW    vSubtask )
{
   zVIEW    StackObject;
   zVIEW    XPG;
   zVIEW    lpZeidonVML;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;

   nZRetCode = GetViewByName( &lpZeidonVML, "ZeidonVML", vSubtask, zLEVEL_TASK );
   nZRetCode = GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   nZRetCode = GetViewByName( &StackObject, "StackObject", vSubtask, zLEVEL_SUBTASK );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, StackObject, "Variable", "ID" );
   RESULT = SetCursorFirstEntityByInteger( XPG, "Variable", "ID", lTempInteger_0, "" );
   if ( CompareAttributeToInteger ( XPG, "Variable", "DataType", 1045 ) != 0 )
   {
      nZRetCode = SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZVSDBAD", "UPDATEVAR" );
   }
   else
   {
      zwTZVSDBAD_SpawnObjectBrowser( vSubtask );
   }

   return( 0 );
}
