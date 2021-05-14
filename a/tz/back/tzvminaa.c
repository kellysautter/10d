/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvminaa.c - VML SubSystem Interpretor Main Module
// DESCRIPTION:   This is the source file which contains the Setup functions
//                for the Interpreter. It contains the functions to do all
//                of the setup.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
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
// LAST MESSAGE ID: VM03001
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2003.03.11 DC
   Corrected code in InitInterp so that the cached TZOPSIGK object
   (LOD: TZOPGRPO) was deleted when the Subtask was deleted.

2001.10.12 HH
   R55883, handle return code.

2001.10.05 HH
   R53708, avoid error message if a C Operation is called.

2001.08.10 HH
   R55612, improve workspace for variables, WRKS system.

2000.11.22 HH
   increment stack size.

2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.10.25  SR  Z2000 Size of path variables
   Modified size of szExecDir in functions GetExecMode, GetExecModeByzKey,
   InitInterp and LoadXPG because size of attributes ExecDir, MetaSrcDir and
   PgmSrcDir has been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

2000.09.03 HH
   Compatibility check for Parser Version.

2000.07.18 HH
   fix for multiple interpreter entry.

2000.03.28   BL  new Repository structure
   ActivateOI_FromFile with zIGNORE_ATTRIB_ERRORS

97/24/02    DonC
   Added check of return code on SfCreateSubtask in SetupInterp.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzwrksto.h"
#include "tzvmlopr.hg"
#include <setjmp.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

// the following global variables are for the multiple entry to the
//  interpreter.
#define MAX_INTEPRETER_ENTRY 16

typedef struct sValueStruct
{
   zLONG    lDataType;
   zLONG    lPtrDataType;
   zLONG    lVal;
   zDECIMAL ldVal;
   zPVOID   lpVal;
   zLONG    lOffset;
} sValueStruct;

static zLONG lMultiple = 0; // multiple counter

#define VALUESTACK_SIZE 64
struct sInterpreterSave
{
   zVIEW vSubtask;
   zVIEW vXPGView;
   zVIEW vStatementView;
   zVIEW vStackObject;
   zSHORT nStackPtr;
   zSHORT nCallStack[ CALLSTACK_SIZE ];
   sValueStruct sValueStack[ VALUESTACK_SIZE ];
};


static struct sInterpreterSave aInterpreterSave[ MAX_INTEPRETER_ENTRY ];

sValueStruct sValueStack[ VALUESTACK_SIZE ]; // value stack to interp

// jump buffer for WKKS out of memory (VERY unlikely)
jmp_buf g_jbWRKS;


/////////////////////////////////////////////////////////////////////////////
//
//   1 - Need to link to TZVSECAA.DLL
//   2 - exec CallInterp
//               zOPER_EXPORT zSHORT OPERATION
//               CallInterp( zVIEW  lpInterpSubtask,
//                           zVIEW  lpAppSubtask,
//                           zPCHAR szSrcDLLName,
//                           zPCHAR szOperationName )
//
//   3 - CallInterp will return
//
//         zXC_NOT_LOADED               1 ---|
//         zXC_OPERATION_NOT_SELECTED   1    |- i.e. Driver needs to run
//         zXC_MODULE_NOT_SELECTED      1 ---|
//         zXC_OPERATION_EXECUTED       0 ----  interp executed
//         zXC_SETUP_DEBUGGER           2 ----  need the debugger up
//
/////////////////////////////////////////////////////////////////////////////

//
// OPERATION: SetupInterp
//
// DESCRIPTION: SetupInterp is called from TZVSXCAD only.   It is called
//    to hook the driver up to the interpretor. The DRIVER does not know that
//    the interpretor exists until we tell it that it does.  Setup interp is
//    passed the subtask view, and the name of the application that we need.
//    Essentially what happens here is that we need to do a couple of things
//    on behalf of the driver so as to interact appropriately.
//    1: We get the address to the function that the driver calls to invoke
//       the interpretor. This is currently called "CallInterp".
//    2: SetupInterp then creates the Interpretor subtask.
//    3: It then calls into Driver at the function InitInterpretor with the
//       current subtask, the newly created interpretor subtask and the
//       address of the operation it needs to call to invoke the interpretor.
//    4: SetupInterp then creates the Application subtask for the application
//       that we are to run. Normally the driver does this but we will do it
//       for the driver.
//    5: lastly we call SetNewApplicationSubtask for some reason.
//
//
zOPER_EXPORT zSHORT OPERATION
SetupInterp( zVIEW vSubtask,        // our current subtask
             zPCHAR szAppName )     // the name of the app to create task for
{
   zVIEW  vTempSubtask;        // temp view
   zSHORT nRC;                 // return code

   // Create the interpretor subtask
   Exec_SfCreateSystemSubtask( &vTempSubtask, "Zeidon_Tools" );

   // call into the driver.
   InitInterpretor( vSubtask, vTempSubtask,
                    (zFARPROC_INTERP) CallInterp ); // address of function

   // name the ZeidonVML subtask at the task level.
   SetNameForView( vTempSubtask, "ZeidonVML", vSubtask, zLEVEL_TASK );

   // Create the appl subtask and name it with the name sent in
// nRC = SfCreateSubtask( &vTempSubtask, vSubtask, szAppName );
   nRC = Exec_SfCreateSubtask( &vTempSubtask, szAppName );
   if ( nRC < 0 )
      return( nRC );

   // 5/10/93
   SetNameForView( vTempSubtask, szAppName, vSubtask, zLEVEL_TASK );
   SetNewApplicationSubtask( vSubtask, vTempSubtask );

   return( 0 );
}

//
// OPERATION: CallInterp
//
// DESCRIPTION: CallInterp is the only function called by any other part of
//    Zeidon. Currently The driver is the only part to call the interp. Call
//    interp is passed the Interpretor's subtask, the application's subtask
//    the SourceDLLName and the operation to be executed.
//    1: first we get the ExecMode of the Module. If the ret code is < 0 then
//       return to the caller that it must execute code. Not the interpretor
//    2: Otherwise invoke the interpretor.
//
zOPER_EXPORT zLONG OPERATION
CallInterp( zVIEW  lpInterpSubtask,     // the interpretor subtask
            zVIEW  lpAppSubtask,        // the application's subtask
            zPCHAR szSrcDLLName,        // the source dll name
            zPCHAR szOperationName,     // the operation name
            zPLONG plRetVal )
{
   zLONG lRC;                          // a long retcode
   zVIEW lpTempSubtask;                // used for cleanup of unnamed views

   lRC = GetExecMode( lpInterpSubtask, lpAppSubtask, szSrcDLLName,
                      szOperationName, "Dialog" );

   if ( lRC < 0 )
      return( zXC_MODULE_NOT_SELECTED );

   lRC = InvokeInterp( lpInterpSubtask, lpAppSubtask, szSrcDLLName,
                       szOperationName, plRetVal );

   // Clean up any unnamed views for the interpreter by creating
   // another subtask for the interpreter, then dropping the subtask
   // with the option to clean up unnamed views for the application.
   //
   if ( !SfCreateSubtask( &lpTempSubtask, lpInterpSubtask, 0 ) )
      SfDropSubtask( lpTempSubtask, 2 );

   return( lRC );
}

zOPER_EXPORT zLONG OPERATION
GetExecMode( zVIEW  lpInterpSubtask,
             zVIEW  lpAppSubtask,
             zPCHAR szSrcDLLName,
             zPCHAR szOperationName,
             zPCHAR szMetaType )
{
   zVIEW  vXChecker;
   zVIEW  CM_List;
   zCHAR  szExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szScopeMeta[ 33 ];
   zCHAR  szSourceFileMeta1[ 33 ];
   zCHAR  szSourceFileMeta2[ 33 ];
   zCHAR  szOperationMeta[ 33 ];
   zSHORT nRC;

   if ( !szMetaType || !*szMetaType )
      return( -1 );

   nRC = GetViewByName( &vXChecker, "TZVSXCOO",
                             lpInterpSubtask, zLEVEL_APPLICATION );

   if ( nRC != zLEVEL_APPLICATION )
   {
      GetViewByName( &CM_List, "TZCMWKSO", lpInterpSubtask, zLEVEL_APPLICATION );

      // If CM_List is 0 then we must have been called from Core at runtime.
      // In this case all operations are run natively so don't bother checking
      // the exec mode.
      if ( CM_List == 0 )
         return( -1 );

      GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), CM_List, "LPLR", "ExecDir" );
      ZeidonStringConcat( szExecDir, 1, 0, "\\TZVSXC01.POR", 1, 0, zMAX_FILESPEC_LTH+1 );
      nRC = ActivateOI_FromFile( &vXChecker, "TZVSXCOO",
                                 lpAppSubtask, szExecDir,
                                 zSINGLE | zIGNORE_ATTRIB_ERRORS );
      if ( nRC != 0 )
         return( -1 );
   }

   // set up the Operation Meta Name
   if ( zstrcmp( szMetaType, "Dialog" ) == 0 )
   {
      strcpy_s( szSourceFileMeta1, zsizeof( szSourceFileMeta1 ), "DialogSourceFile" );
      strcpy_s( szSourceFileMeta2, zsizeof( szSourceFileMeta2 ), "Dialog" );
      strcpy_s( szOperationMeta, zsizeof( szOperationMeta ), "DialogOperation" );
      strcpy_s( szScopeMeta, zsizeof( szScopeMeta), "Dialogs" );
   }
   else
   if ( zstrcmp( szMetaType, "LOD" ) == 0 )
   {
      strcpy_s( szSourceFileMeta1, zsizeof( szSourceFileMeta1 ), "LODSourceFile" );
      strcpy_s( szSourceFileMeta2, zsizeof( szSourceFileMeta2 ), "LOD" );
      strcpy_s( szOperationMeta, zsizeof( szOperationMeta ), "LOD_Operation" );
      strcpy_s( szScopeMeta, zsizeof( szScopeMeta), "LODs" );
   }
   else
   {
      strcpy_s( szSourceFileMeta1, zsizeof( szSourceFileMeta1 ), "GlobalSourceFile" );
      strcpy_s( szSourceFileMeta2, zsizeof( szSourceFileMeta2 ), "GlobalOperation" );
      strcpy_s( szOperationMeta, zsizeof( szOperationMeta ), "Operation" );
      strcpy_s( szScopeMeta, zsizeof( szScopeMeta), "GlobalOperations" );
   }

   nRC = SetCursorFirstEntityByString( vXChecker, szSourceFileMeta1,
                                       "Name", szSrcDLLName, szScopeMeta );
#if 0
   if ( nRC < zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByString( vXChecker, szSourceFileMeta2,
                                          "Name", szSrcDLLName, szScopeMeta );
      if ( nRC == zCURSOR_SET )
      {
         nRC = SetAttributeFromInteger( vXChecker, "APPLICATION", "CurrentMode", 1 );
         return( zXC_OPERATION_STAGED );
      }
      else
         return( -1 );
   }
#endif

   if ( nRC < zCURSOR_SET )
      return( -1 );

   nRC = SetCursorFirstEntityByString( vXChecker, szOperationMeta, "Name", szOperationName, "" );

   if ( nRC >= zCURSOR_SET )
   {
      nRC = SetAttributeFromInteger( vXChecker, "APPLICATION", "CurrentMode", zXC_SETUP_DEBUGGER );
      return( zXC_SETUP_DEBUGGER );
   }
   else
   {
      nRC = SetAttributeFromInteger( vXChecker, "APPLICATION", "CurrentMode", 1 );
      return( zXC_OPERATION_STAGED );
   }
}


zOPER_EXPORT zLONG OPERATION
GetExecModeByZKey( zVIEW  lpInterpSubtask,
                   zVIEW  lpAppSubtask,
                   zLONG  lZKey )

{
   zVIEW  vXChecker;
   zVIEW  CM_List;
   zCHAR  szExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   nRC = GetViewByName( &vXChecker, "TZVSXCOO", lpInterpSubtask, zLEVEL_APPLICATION );

   if ( nRC != zLEVEL_APPLICATION )
   {
      nRC = GetViewByName( &CM_List, "TZCMWKSO", lpInterpSubtask, zLEVEL_APPLICATION );

      nRC = GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), CM_List, "LPLR", "ExecDir" );

      ZeidonStringConcat( szExecDir, 1, 0, "\\TZVSXC01.POR", 1, 0, zMAX_FILESPEC_LTH + 1 );
      nRC = ActivateOI_FromFile( &vXChecker, "TZVSXCOO", lpAppSubtask, szExecDir, zSINGLE | zIGNORE_ATTRIB_ERRORS );
      if ( nRC != 0 )
         return( -1 );
   }

   nRC = SetCursorFirstEntityByInteger( vXChecker, "DialogOperation", "ZKey", lZKey, "APPLICATION" );
#if 0
   if ( nRC != zCURSOR_SET )
      return( -1 );
#else

   if ( nRC < zCURSOR_SET )
      nRC = SetCursorFirstEntityByInteger( vXChecker, "LOD_Operation", "ZKey", lZKey, "APPLICATION" );
   if ( nRC < zCURSOR_SET )
      nRC = SetCursorFirstEntityByInteger( vXChecker, "Operation", "ZKey", lZKey, "APPLICATION" );

   // why do this???
   if ( nRC < zCURSOR_SET )
      return( -1 );
#endif

   if ( nRC >= zCURSOR_SET )
   {
      nRC = SetAttributeFromInteger( vXChecker, "APPLICATION", "CurrentMode", zXC_SETUP_DEBUGGER );
      return( zXC_SETUP_DEBUGGER );
   }
   else
   {
      nRC = SetAttributeFromInteger( vXChecker, "APPLICATION", "CurrentMode", 1 );
      return( zXC_OPERATION_STAGED );
   }
}

zOPER_EXPORT zLONG OPERATION
Exec_StepOnce( zVIEW vSubtask )
{
   zVIEW vZ_View;
   zVIEW vS_View;
   zLONG lStepRC;
   zLONG lRC;
   zCHAR szDebugFlag[ 2 ];

   // Single step actually loops unless one of these conditions occur.
   //   1.  The next statement to execute is within an operation currently in debug mode.
   //   2.  The return code from the last statement executed indicates debugging is to start for that operation.
   //   3.  Execution is terminating.
   //
   // The loop is very close to that in the Go function.
   //

   szDebugFlag[ 0 ] = 'N';
   lStepRC = zXC_STEP_EXECUTED;
   zPVOID hWRKS;

   // initialize work storage manager
   if ( WRKS_Init( &hWRKS ) < 0 )
   {
      // Error in WRKS system
      MessageSend( vSubtask, "VM03002", "VML Interpretor",
                   "Error Initializing Work Storage",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "VML Interpreter Error ","Initializing Work Storage" );

      return( lStepRC );
   }

   if ( setjmp( g_jbWRKS ) != 0 )
   {
      // Error return from longjmp
      WRKS_Close( &hWRKS );
      return( zXC_STEP_EXECUTED );
   }

   while ( (lStepRC == zXC_STEP_EXECUTED       ||
            lStepRC == zXC_OPERATION_STAGED) &&
            szDebugFlag[ 0 ] != 'Y' )
   {
      lStepRC = StepOnce( vSubtask, hWRKS, &lRC );

      // Don't execute the following if the operation is terminating.
      if ( lStepRC )
      {
         GetViewByName( &vZ_View, "ZeidonVML", vSubtask, zLEVEL_TASK );
         GetViewByName( &vS_View, "XPG", vZ_View, zLEVEL_SUBTASK );
         GetStringFromAttribute( szDebugFlag, zsizeof( szDebugFlag ), vS_View, "Operation", "CurrentDebugFlag" );
      }
   }

   if ( lStepRC == zXC_SETUP_DEBUGGER )
   {
      SetAttributeFromString( vS_View, "Operation", "CurrentDebugFlag", "Y" );
   }

   // close work storage manager
   WRKS_Close( &hWRKS );

   return( lStepRC );
}


zOPER_EXPORT zLONG OPERATION
Exec_StepForward( zVIEW vSubtask )
{
   zLONG lRC = 0;
   zPVOID hWRKS;

   // initialize work storage manager
   if ( WRKS_Init( &hWRKS ) < 0 )
   {
      // Error in WRKS system
      MessageSend( vSubtask, "VM03002", "VML Interpretor",
                   "Error Initializing Work Storage",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "VML Interpreter Error ","Initializing Work Storage" );

      return( zXC_STEP_EXECUTED );
   }

   if ( setjmp( g_jbWRKS ) != 0 )
   {
      // Error return from longjmp
      WRKS_Close( &hWRKS );
      return( zXC_STEP_EXECUTED );
   }

   lRC = StepOnce( vSubtask, hWRKS, &lRC );

   // close work storage manager
   WRKS_Close( &hWRKS );

   return( lRC );
}

zOPER_EXPORT zLONG OPERATION
Exec_Skip( zVIEW vSubtask )
{
   zLONG lRC = Skip( vSubtask );
   return( lRC );
}

zOPER_EXPORT zLONG OPERATION
Exec_Go( zVIEW vSubtask )
{
   zLONG  lRC = 0;
   zPVOID hWRKS;

   // initialize work storage manager
   if ( WRKS_Init( &hWRKS ) < 0 )
   {
      // Error in WRKS system
      MessageSend( vSubtask, "VM03002", "VML Interpretor",
                   "Error Initializing Work Storage",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "VML Interpreter Error ","Initializing Work Storage" );
      return( zXC_OPERATION_EXECUTED );
   }

   if ( setjmp( g_jbWRKS ) != 0 )
   {
      // Error return from longjmp
      WRKS_Close( &hWRKS );
      return( zXC_STEP_EXECUTED );
   }

   lRC = Go( vSubtask, hWRKS, &lRC );

   // close work storage manager
   WRKS_Close( &hWRKS );

   return( lRC );
}

zOPER_EXPORT zSHORT OPERATION
Exec_ResumeFromDebugger( zVIEW  vDebuggerSubtask,
                         zPCHAR pchDoNothing,
                         zVIEW  vAppSubtask )
{
   zSHORT nRC = ResumeFromDebugger( vDebuggerSubtask, 0, vAppSubtask );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
Exec_SfCreateSystemSubtask( zPVIEW vReturnSubtask,
                            zPCHAR pchAppName )
{
   zVIEW  vSubtask = GetDefaultViewForActiveTask( );
   zSHORT nRC = SfCreateSystemSubtask( vReturnSubtask, vSubtask, pchAppName );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
Exec_SfCreateSubtask( zPVIEW vReturnSubtask,
                      zPCHAR pchAppName )
{
   zVIEW  vSubtask = GetDefaultViewForActiveTask( );
   zSHORT nRC = SfCreateSubtask( vReturnSubtask, vSubtask, pchAppName );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
StageSingleTask( zVIEW vSubtask, zVIEW vViewToStage );

zOPER_EXPORT zSHORT OPERATION
StageBrowser( zVIEW vSubtask, zVIEW vVarView )
{
   zSHORT nRC = StageSingleTask( vSubtask, vVarView );
   return( nRC );
}

//
// OPERATION:
//
// DESCRIPTION:

zOPER_EXPORT zLONG OPERATION
InvokeInterp( zVIEW  lpInterpSubtask,                // Interpretor's subtask
              zVIEW  lpAppSubtask,                   // application's subtask
              zPCHAR szSrcDLLName,                   // name of the dialog
              zPCHAR szOperationName,                // operation to interp
              zPLONG plRC )
{
   zVIEW  vXChecker;
   zSHORT nRC;
   zLONG  lStepRC;
   zVIEW  vStackObject;                           // view to the stack object
   zVIEW  vS_View;
   zVIEW  vZ_View;
   zVIEW  vSO;
   zVIEW  vSubtask = 0;
   zVIEW  vTaskLPLR;
   zPVOID hWRKS = NULL;

   // Check to make sure the LPLR is active.
   GetViewByName( &vTaskLPLR, "TaskLPLR", lpAppSubtask, zLEVEL_TASK );
   if ( vTaskLPLR == 0 )
      InitializeLPLR( lpInterpSubtask, "" );

   aInterpreterSave[ lMultiple ].vSubtask = lpInterpSubtask;

   if ( lMultiple > 0 )
   {
      if ( lMultiple >= MAX_INTEPRETER_ENTRY )
      {
         // Error, because number of recursive entries
         //  is greater than MAX_INTEPRETER_ENTRY
         return( -99 );
      }

      vSubtask = aInterpreterSave[ lMultiple -1 ].vSubtask;

      // save the global variables
      aInterpreterSave[ lMultiple -1 ].nStackPtr = g_nStackPtr;
      g_nStackPtr = 0;
      memcpy( aInterpreterSave[ lMultiple -1 ].nCallStack,
              g_nCallStack, zsizeof( g_nCallStack ) );
      memset( g_nCallStack, 0, zsizeof( g_nCallStack ) );

      aInterpreterSave[ lMultiple -1 ].vXPGView = g_vXPGView;
      g_vXPGView = 0;

      GetViewByName( &g_vStatementView, "StatementView",
                     vSubtask, zLEVEL_SUBTASK );

      aInterpreterSave[ lMultiple -1 ].vStatementView = g_vStatementView;
      g_vStatementView = 0;

      GetViewByName( &vSO, "StackObject",
                     vSubtask, zLEVEL_SUBTASK );
      aInterpreterSave[ lMultiple -1 ].vStackObject = vSO;

      memcpy( aInterpreterSave[ lMultiple -1 ].sValueStack,
              sValueStack, zsizeof( sValueStack ) );
      memset( sValueStack, 0, zsizeof( sValueStack ) );
   }

   lMultiple++;

   // see if we can load the XPG. if not, don't parse for now, exit out
   nRC = InitInterp( lpInterpSubtask, lpAppSubtask, szSrcDLLName );
   if ( nRC < 0 )
   {
      lStepRC = 1; // if the init failed return.
      goto EndOfInvokeInterp;
   }

   // Initialize the Working Storage Manager
   if ( WRKS_Init( &hWRKS ) < 0 )
   {
      // Error in WRKS system
      MessageSend( vSubtask, "VM03002", "VML Interpretor",
                   "Error Initializing Work Storage",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "VML Interpreter Error ","Initializing Work Storage" );
      lStepRC = 1; // if the init failed return.
      goto EndOfInvokeInterp;
   }

   if ( setjmp( g_jbWRKS ) != 0 )
   {
      // Error return from longjmp
      WRKS_Close( &hWRKS );
      lStepRC = 1;
      goto EndOfInvokeInterp;
   }

   // get the program object as it was loaded by the init function.
   GetViewByName( &g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );

   // try to position at the correct operation in the Subtask
   nRC = SetCursorFirstEntityByString( g_vXPGView, "Operation",
                                       "Name", szOperationName, "" );
   if ( nRC != zCURSOR_SET )
   {
      // no error message here, as we assume that the operation
      //  to be called is a C operation.
      // Returning -1, the driver will try to load it from the DLL
      lStepRC = -1;
      goto EndOfInvokeInterp;
   }

   SetCursorFirstEntityByEntityCsr( g_vXPGView,
                                    "SourceFile",
                                    g_vXPGView,
                                    "SourceFileOfOperation", "" );
   SetCursorFirstEntityByEntityCsr( g_vXPGView,
                                    "OperationSource",
                                    g_vXPGView,
                                    "Operation", "" );
   SetCursorFirstEntity( g_vXPGView, "OperationText", "" );
   SetCursorFirstEntity( g_vXPGView, "Statement", "" );
   CreateViewFromViewForTask( &g_vStatementView, g_vXPGView, 0 );

   // Create all of the entities needed in the stack object
   SetStackObjectFromViews( lpInterpSubtask,
                            g_vXPGView,         // View to the XPG
                            g_vStatementView, // View to the statement
                            0 );              // index into the expression

   // Get the stack object as it was loaded with the XPG
   GetViewByName( &vStackObject,
                  "StackObject",
                  lpInterpSubtask,
                  zLEVEL_SUBTASK );
// ###blob  SetAttributeFromInteger( vStackObject,
//                            "Variable", "Value", (zLONG)lpAppSubtask );
   SetAttributeFromBlob( vStackObject, "Variable", "Value", &lpAppSubtask, sizeof( void * ) );
   SetNameForView( g_vStatementView, "StatementView", lpInterpSubtask, zLEVEL_SUBTASK );
   SetNameForView( lpAppSubtask, "ApplicationView", lpInterpSubtask, zLEVEL_SUBTASK );

   GetViewByName( &vXChecker, "TZVSXCOO", lpInterpSubtask, zLEVEL_APPLICATION );
   if ( vXChecker != 0 )
   {
      nRC = SetCursorFirstEntityByString( vXChecker, "DialogOperation", "Name", szOperationName, "" );

      if ( nRC == zCURSOR_SET )
      {
         GetViewByName( &vZ_View, "ZeidonVML", vSubtask, zLEVEL_TASK );
         GetViewByName( &vS_View, "XPG", vZ_View, zLEVEL_SUBTASK );

         SetAttributeFromString( vS_View, "Operation", "CurrentDebugFlag", "Y" );
         lStepRC = zXC_SETUP_DEBUGGER;
         goto EndOfInvokeInterp;
      }
   }

// lStepRC = zXC_STEP_EXECUTED;
#if 0
   OperationCount( 3 );
   OperationCount( 1 );
#endif
// Loop through each statement using the Go function.
   lStepRC = Go( lpAppSubtask, hWRKS, plRC );

EndOfInvokeInterp:
   // close work storage manager
   if ( hWRKS )
      WRKS_Close( &hWRKS );

   lMultiple--;
   if ( lMultiple > 0 )
   {
      vSubtask = aInterpreterSave[ lMultiple - 1 ].vSubtask;

      // restore the global variables
      g_nStackPtr = aInterpreterSave[ lMultiple - 1 ].nStackPtr;
      memcpy( g_nCallStack, aInterpreterSave[ lMultiple -1 ].nCallStack, zsizeof( g_nCallStack ) );

      g_vXPGView = aInterpreterSave[ lMultiple - 1 ].vXPGView;
      SetNameForView( g_vXPGView, "XPG", vSubtask, zLEVEL_SUBTASK );
      g_vStatementView = aInterpreterSave[ lMultiple - 1 ].vStatementView;
      SetNameForView( g_vStatementView, "StatementView", vSubtask, zLEVEL_SUBTASK );

      vSO = aInterpreterSave[ lMultiple - 1 ].vStackObject;
      SetNameForView( vSO, "StackObject", vSubtask, zLEVEL_SUBTASK );

      memcpy( sValueStack, aInterpreterSave[ lMultiple - 1 ].sValueStack, zsizeof( sValueStack ) );
   }

   return( lStepRC );
}

//
// OPERATION:   InitializeInterpretor
//
// DESCRIPTION: InitializeInterpretor does all of the set up for the VML
//    interpretor. Loads the XPG File. Next be sure that we have access to
//   the global as well as Zeidon operation lists. Also Init two Globals.
//
zOPER_EXPORT zSHORT OPERATION
InitInterp( zVIEW  lpInterpSubtask,
            zVIEW  lpAppSubtask,
            zPCHAR szSrcDLLName )
{
   zVIEW  vStackObject;
// zVIEW  vViewObject;
   zVIEW  vTZOPGRPO;
   zCHAR  szExecDir[ zMAX_FILESPEC_LTH + 1 ];                   // dir string
   zSHORT nRC;

   nRC = LoadXPG( lpInterpSubtask, lpAppSubtask, szSrcDLLName, "C" );
   if ( nRC != 0 ) //  failed
      return( -1 );

   if ( GetViewByName( &vStackObject, "StackObject", lpInterpSubtask, zLEVEL_SUBTASK ) != zLEVEL_SUBTASK )
   {
      // activate an empty stack object.
      ActivateEmptyObjectInstance( &vStackObject, "TZVSRTOO", lpInterpSubtask, zSINGLE );

      // create the root of the stack object.
      CreateEntity( vStackObject, "StackObject", zPOS_AFTER );

      // Name the Stack Object
      SetNameForView( vStackObject, "StackObject", lpInterpSubtask, zLEVEL_SUBTASK );
   }

#if 0 // DGC  The following to OIs do not appear to be used anywhere so I'm
      // removing them so that we don't need an LPLR to be active.
   // get access to the global operation list and the Zeidon operation list
   nRC = RetrieveViewForMetaList( vSubtask, &lpGOListView, zREFER_GO_META );
   if ( nRC < 0 ) //  failed
      lpGOListView = 0;

   // get a view to the Task LPLR
   if ( GetViewByName( &vViewObject, "TaskLPLR", vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
      return( -1 );
#endif

   if ( GetViewByName( &vTZOPGRPO, "TZOPSIGK.Interp", lpInterpSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      // get the exec directory
      zgGetZeidonToolsDir( lpAppSubtask, szExecDir, zAPPL_DIR_OBJECT );

      // concat the name. and the .XGO extention
      strcat_s( szExecDir, zsizeof( szExecDir ), "TZOPSIGK.XGO" );

      nRC = ActivateOI_FromFile( &vTZOPGRPO, "TZOPGRPO", lpInterpSubtask, szExecDir, zSINGLE );

      if ( nRC < 0 ) //  failed
         vTZOPGRPO = 0;
      else
         SetNameForView( vTZOPGRPO, "TZOPSIGK.Interp", lpInterpSubtask, zLEVEL_TASK );
   }

   g_nStackPtr = 0;
   g_nCallStack[ 0 ] = 1;
   return( 0 );
}

//
// OPERATION: LoadXPG
//
// DESCRIPTION: LoadXPG Gets the executable directory from the TaskLPLR.
//    It then creates a file name from which to load the pared instance file.
//    If the load is successful then the view is names in the interp
//    subtask as XPG.
//
zOPER_EXPORT zSHORT OPERATION
LoadXPG( zVIEW  lpInterpSubtask,                      // interpretor subtask
         zVIEW  vSubtask,                             // application subtask
         zPCHAR szSrcDLLName,                         // source dllname
         zCPCHAR cpcGenLang )
{

   zVIEW  vViewObject;                                       // task view
   zCHAR  szExecDir[ zMAX_FILESPEC_LTH + 1 ];                // dir string
   zCHAR  szMessage[ zMAX_MESSAGE_LTH + 1 ];
   zLONG  lParserVersion = 0;
   zSHORT nLth;                                              // string length
   zSHORT nRC;

   // First check to see if it already exists.
   GetViewByName( &g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );
   if ( g_vXPGView != 0 )
      return( 0 );

   // get a view to the Task LPLR
   if ( GetViewByName( &vViewObject, "TaskLPLR", vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      return( -1 );
   }

   // get the exec directory
   GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), vViewObject, "LPLR", "ExecDir" );

   // be sure it was terminated with a '\'
   nLth = (zSHORT) zstrlen( szExecDir );
   if ( szExecDir[ nLth ] != '\\' )
   {
      szExecDir[ nLth ] = '\\';
      szExecDir[ nLth + 1 ] = 0;
   }

   // concat the name. and the .XPG extention
   strcat_s( szExecDir, zsizeof( szExecDir ), szSrcDLLName );
   if ( cpcGenLang[ 0 ] == 'J' )
      strcat_s( szExecDir, zsizeof( szExecDir ), ".XPJ" );
   else
      strcat_s( szExecDir, zsizeof( szExecDir ), ".XPG" );

   // activate the file
   nRC = ActivateOI_FromFile( &g_vXPGView,
                                   "TZVSPOOO",
                                   lpInterpSubtask,
                                   szExecDir,
                                   zSINGLE | zNOI_OKAY |
                                   zIGNORE_ATTRIB_ERRORS );

   if ( nRC != 0 ) //  failed
   {
      strcpy_s( szMessage, zsizeof( szMessage ), "Source VML :" );
      strcat_s( szMessage, zsizeof( szMessage ), szSrcDLLName );
      strcat_s( szMessage, zsizeof( szMessage ), " must be parsed prior to execution" );
      MessageSend( lpInterpSubtask, "VM03002", "VML Interpretor",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "Could Not Load ", szExecDir );
      return( -1 );
   }
   // do the Parser Version Compatibility Check
#define PARSER_VERSION_REQUIRED 1
   nRC = GetIntegerFromAttribute( &lParserVersion, g_vXPGView,
                                       "VML_XSource",  "ParserVersion" );
   if ( nRC != 0 || lParserVersion < PARSER_VERSION_REQUIRED )
   {
      // the XPG is too old, and not compatible with the current
      //  interpreter.
      strcpy_s( szMessage, zsizeof( szMessage ), "Source VML :" );
      strcat_s( szMessage, zsizeof( szMessage ), szSrcDLLName );
      strcat_s( szMessage, zsizeof( szMessage ),
         " is outdated and must be re-parsed prior to execution" );
      MessageSend( lpInterpSubtask, "VM03002", "VML Interpretor",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "Program File Outdated : ", szExecDir );
      return( -1 );
   }

   // name the view to the interpretor subtask
   SetNameForView( g_vXPGView, "XPG", lpInterpSubtask, zLEVEL_SUBTASK );

   return( 0 );
}

//
// OPERATION: SetStackObjectFromViews
//
// DESCRIPTION: SetStackObjectFromViews creates an operation entity in the
//    Stack call object an copies all of the variable data thers as well.
//
//
//
zOPER_EXPORT zSHORT OPERATION
SetStackObjectFromViews( zVIEW  lpInterpSubtask,            // interp subtask
                         zVIEW  vXPG,                      // View to the XPG
                         zVIEW  vStmtView,           // View to the statement
                         zSHORT nIndex )         // index into the expression
{
   zSHORT nRC;                                          // a Return code
   //zSHORT nPrevOperation;                                       // a ret code
   zVIEW  vStackObject;                           // view to the stack object
   //zVIEW  vPriorOperation; // view to the prior operation in the stack object

   // Get the stack object as it was loaded with the XPG
   nRC =
      GetViewByName( &vStackObject,
                     "StackObject",
                     lpInterpSubtask,
                     zLEVEL_SUBTASK );
   if ( nRC != zLEVEL_SUBTASK )
   {
      return( -1 );
   }

   SfLockView( vXPG );
   SfLockView( vStmtView );

// DGC 8/19/02
// vPriorOperation doesn't appear to be used outside of this function, so
// I'm removing the code.
#if 0
   nRC =
      CreateViewFromViewForTask( &vPriorOperation,
                          vStackObject, 0 );


   nPrevOperation =
      SetCursorPrevEntity( vPriorOperation,
                           "OperationCall", "" );
#endif

   if ( SetCursorFirstEntityByAttr( vStackObject, "OperationCall", "Name",
                                    vXPG, "Operation", "Name", 0 ) < zCURSOR_SET )
   {
      // create an operation entity at the end of the list ( stack )
      CreateEntity( vStackObject, "OperationCall", zPOS_AFTER );


      // set the name of the operation that was called.
      SetAttributeFromAttribute( vStackObject, "OperationCall", "Name",
                                 vXPG, "Operation", "Name" );
   }

   // set the views
   SetAttributeFromInteger( vStackObject, "OperationCall", "XPGView",
                            (zLONG) vXPG );

   // set the views
   SetAttributeFromInteger( vStackObject, "OperationCall", "StatementView",
                            (zLONG) vStmtView );
   // set the offset
   SetAttributeFromInteger( vStackObject, "OperationCall", "StatementOffset",
                            nIndex );

   // now loop through and do all of the variables
   for ( nRC = SetCursorFirstEntity( vXPG, "Variable", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vXPG, "Variable", "" ) )
   {
      // Check to see if the variable already exists in the stack object.  It
      // will exist if the interpreter is being called from a different app
      // (like Core) and the app has set up the arguments already.
      if ( SetCursorFirstEntityByAttr( vStackObject, "Variable", "Name",
                                       vXPG, "Variable", "Name", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vStackObject, "Variable", zPOS_LAST );
      }

      SetMatchingAttributesByName( vStackObject, "Variable",
                                   vXPG, "Variable", zSET_NULL );
   }

   SetCursorFirstEntity( vStackObject, "Variable", "" );
   SetCursorFirstEntity( vXPG, "Variable", "" );

// DropView( vPriorOperation );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
UpdateStackObjectFromViews( zVIEW  lpInterpSubtask,         // interp subtask
                            zVIEW  vXPG,                   // View to the XPG
                            zVIEW  vStmtView,        // View to the statement
                            zSHORT nIndex,       // index into the expression
                            zSHORT nStackOffset )      // index into the expression
{
   zSHORT nRC;                                          // a Return code
   zVIEW  vStackObject;                           // view to the stack object

   // Get the stack object as it was loaded with the XPG
   nRC =
      GetViewByName( &vStackObject,
                     "StackObject",
                     lpInterpSubtask,
                     zLEVEL_SUBTASK );
   if ( nRC != zLEVEL_SUBTASK )
   {
      return( -1 );
   }

   SfLockView( vXPG );
   SfLockView( vStmtView );

   // set the views
   nRC =
      SetAttributeFromInteger( vStackObject,
                               "OperationCall",
                               "XPGView",
                               (zLONG)vXPG );
   nRC =
      SetAttributeFromInteger( vStackObject,
                               "OperationCall",
                               "StackOffset",
                               nStackOffset );
   // set the views
   nRC =
      SetAttributeFromInteger( vStackObject,
                               "OperationCall",
                               "StatementView",
                               (zLONG)vStmtView );
   // set the offset
   nRC =
      SetAttributeFromInteger( vStackObject,
                               "OperationCall",
                               "StatementOffset",
                               nIndex );
   return( 0 );
}

#ifdef __cplusplus
}
#endif
