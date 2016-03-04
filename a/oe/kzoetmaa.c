//
// MODULE NAME:  KZOETMAA  -  Object services Task Management
// DESCRIPTION:  This is the source file which contains Object services
//               task management operations.
// OPERATIONS:   All OPERATIONS managing Application Tasks and
//               Subtasks.
// **********************************************************************
// * Copyright (c) 1993-2016 QuinSoft, Inc. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    3/31/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2001.08.07  DGC  10b
   Doubled the valid length of a mutex name.

2000.11.07  DGC Z10
   Added argument 'lControl' to fnMutexLock() and fnSysMutexLock().

2000.04.27 DGC  10b
   Added code to allow user to query mutex status.

2000.01.19 HH
   For the main task, first shut the DBHandlers, then drop the views.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.10.04  DGC  10a
   Each task is flagged as 'bDisable' until it is fully initialized.

   SfGetNextSubtask( ) does not return tasks flagged as 'bDisable' or
   'bShutdown'.

1999.04.26  DGC  10a  ApplA
   Fixed a bug--when we are closing a task we now drop views and OIs before
   closing networks, db-handlers, etc.

   Moved fnFindTask() to kzoesyaa.c.

1999.04.19  DGC  10a  ApplA
   Changed mutex logic to use use-counts.

*/

//#define DKS_DBG

#include "kzoephdr.h"

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

LPVIEWENTITYCSR LOCALOPER
fnCreateEntityCsrHandle( LPVIEWENTITYCSR lpViewEntityCsr,
                         LPVIEWCSR       lpViewCsr,
                         zSHORT          nIndex );

//./ ADD NAME=SfAllocTaskMemory
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfAllocTaskMemory
//
//  PURPOSE:    Allocate core memory on behalf of the application task.
//              Maximum size that should be specified is 65000.
//
//              MEMORY IS RETURNED AS A HANDLE!!
//
//  RETURNS:    0 - Memory allocated
//    zCALL_ERROR - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfAllocTaskMemory( zVIEW vTaskView, zCOREMEM Memory, zLONG lBytes )
{
   LPTASK   lpCurrentTask;

   // initialize to no memory
   *Memory = 0;
   if ( (lpCurrentTask = fnOperationCall( iSfAllocTaskMemory,
                                          vTaskView, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   *Memory = (zPCHAR) fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                        lBytes, 1, 0, iTaskMemory );

   fnOperationReturn( iSfAllocTaskMemory, lpCurrentTask );
   if ( *Memory )
      return( 0 );
   else
      return( zCALL_ERROR );
}

//./ ADD NAME=SfReAllocTaskMemory
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfReAllocTaskMemory
//
//  PURPOSE:    Allocate core memory on behalf of the application task.
//              Maximum size that should be specified is 65000.
//
//              MEMORY IS RETURNED AS A HANDLE!!
//
//  RETURNS:    0 - Memory allocated
//    zCALL_ERROR - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfReAllocTaskMemory( zVIEW vTaskView, zCOREMEM Memory, zLONG lBytes )
{
   LPTASK   lpCurrentTask;
   zLONG    lMemoryLth;

   if ( (lpCurrentTask = fnOperationCall( iSfReAllocTaskMemory,
                                          vTaskView, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Realloc requires initial memory.
   if ( *Memory == 0 || (lMemoryLth = SfGetTaskMemoryLength( *Memory )) == 0 )
   {
      *Memory = 0;
   }
   else
   if ( lMemoryLth < lBytes )
   {
      zPVOID MemoryOrig = *Memory;

      *Memory = (zPCHAR) fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                           lBytes, 1, 0, iTaskMemory );
      zmemcpy( zGETPTR( *Memory ), zGETPTR( MemoryOrig ), lMemoryLth );

      SfFreeTaskMemory( MemoryOrig );
   }

   fnOperationReturn( iSfReAllocTaskMemory, lpCurrentTask );
   if ( *Memory )
      return( 0 );
   else
      return( zCALL_ERROR );
}

//./ ADD NAME=SfGetTaskMemoryLength
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetTaskMemoryLength
//
//  PURPOSE:    Return length of Core memory that was Alloc'd on behalf of
//              the application task
//
//  RETURNS:    0 - Invalid address
//              otherwise - Length of allocated memory
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG OPERATION
SfGetTaskMemoryLength( zPVOID Address )
{
   return( fnGetDataspaceLength( zGETPTR( Address ) ) );
}

//./ ADD NAME=SfFreeTaskMemory
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfFreeTaskMemory
//
//  PURPOSE:    Free core memory that was Alloc'd on behalf of
//              the application task
//
//  RETURNS:    0 - Memory free
//    zCALL_ERROR - Address to be freed is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfFreeTaskMemory( zPVOID Address )
{
   if ( fnFreeDataspace( zGETPTR( Address ) ) )
      return( zCALL_ERROR );
   else
      return( 0 );
}

//./ ADD NAME=SfLockTaskMemory
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfLockTaskMemory
//
//  PURPOSE:    Lock core memory that was Alloc'd on behalf of
//              the application task
//
//  RETURNS:    address of locked memory
//              0 - Address to be locked is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zPVOID OPERATION
SfLockTaskMemory( zPVOID Address )
{
   return( zGETPTR( Address ) );
}

//./ ADD NAME=SfUnlockTaskMemory
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfUnlockTaskMemory
//
//  PURPOSE:    Unlock core memory that was locked on behalf of
//              the application task
//
//  RETURNS:    0 - Memory unlocked
//    zCALL_ERROR - Address to be unlocked is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zPVOID OPERATION
SfUnlockTaskMemory( zPVOID Address )
{
   return( zGETHNDL( Address ) );
}

//./ ADD NAME=SfSetTaskFlag
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfSetTaskFlag
//
//  PURPOSE:    To set the task flag for the flags requested to the value
//              desired (On or Off).
//
//       zTASKFLAG_AUDIT  0x0001
//       zTASKFLAG_DEBUG  0x0002
//       zTASKFLAG_TRACE  0x0004
//
//       zSET_ON   1
//       zSET_OFF  0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfSetTaskFlag( zVIEW lpView, zSHORT nFlag, zSHORT bSet )
{
   LPTASK lpTask;

   // get the task block associated with input ID
   lpTask = zGETPTR( lpView->hTask );
   if ( lpTask == 0 )
      return( zCALL_ERROR );

   if ( nFlag & zTASKFLAG_AUDIT )
      lpTask->bAudit = bSet;

   if ( nFlag & zTASKFLAG_DEBUG )
      lpTask->bDebug = bSet;

   if ( nFlag & zTASKFLAG_TRACE )
      lpTask->bTrace = bSet;

   return( 0 );
}

//./ ADD NAME=SfIsObjectServicesTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfIsObjectServicesTask
//
//  PURPOSE:    To determine if a tesk is a registered task with
//              object services. If a 0 task id is passed, assume
//              the request is for the current task.
//
//  RETURNS:    0 - Task is not the ObjectServices Task
//              1 - Task is the ObjectServices Task
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL  OPERATION
SfIsObjectServicesTask( zLONG hTask )
{
   LPTASK lpTask;

   // get the task block associated with input ID
   lpTask = zGETPTR( hTask );

   // We should walk the task chains to find out for sure???
   if ( lpTask )
      return( 1 );
   else
      return( 0 );
}

//./ ADD NAME=DisableTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DisableTask
//
//  PURPOSE:    To disable operations for a given task
//
//  RETURNS:    0           - Task disabled
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT  OPERATION
DisableTask( zVIEW  lpView )
{
   LPTASK lpCurrentTask;
   LPTASK lpTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iDisableTask, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpTask = zGETPTR( lpView->hTask );
   lpTask->bDisable = TRUE;

   fnOperationReturn( iDisableTask, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=EnableTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      EnableTask
//
//  PURPOSE:    To enable task operations for a given task
//
//  RETURNS:    0           - Task enabled
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT  OPERATION
EnableTask( zVIEW  lpView )
{
   LPTASK lpCurrentTask;
   LPTASK lpTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iEnableTask, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpTask = zGETPTR( lpView->hTask );
   lpTask->bDisable = 0;

   fnOperationReturn( iEnableTask, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=fnCreateProcess
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCreateProcess
//
//  PURPOSE:    To create a process record
//
//  PARAMETERS: SystemAnchorBlock - a system block for the current
//                                  operating system (if there is one);
//              hMainWnd          - Main Window for the task to which
//                                  Object Services can send messages
//              lAppOE_Msg        - User message for the task which
//                                  object services can send and have
//                                  the task call ProcessZeidonMessage
//
//  RETURNS:    LPPROCESS - Handle of process record created
//              0 (NULL)  - Could not create a process (bad news!)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
LPPROCESS
fnCreateProcess( zLONG   hMainWnd,
                 zLONG   lAppOE_Msg )
{
   LPPROCESS hProcess = 0;

#if 0
   LPPROCESS lpProcess;

   // Allocate a process record.
   hProcess = fnAllocDataspace( (LPDATAHEADER) g_hAnchorBlock,
                                zsizeof( ProcessRecord ), 1, 0, iProcess );
   // If a process record could not be acquired, return.
   if ( hProcess == 0 )
      return( 0 );  // return Failure

   lpProcess = zGETPTR( hProcess );

   // Chain Process to main anchor block.
   lpProcess->hNextProcess = AnchorBlock->hFirstProcess;
   AnchorBlock->hFirstProcess = hProcess;

   lpProcess->lProcessID    = SysGetProcessID( 0 );
#endif

   return( hProcess );
}

//./ ADD NAME=fnCreateTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCreateTask
//
//  PURPOSE:    To create an application task when a new task
//              is registered with object services.
//              Note that this is protected by the ANCHORBLOCK mutex.
//
//  PARAMETERS: lSystemInstance   - a system block for the current
//                                  operating system (if there is one);
//              hMainWnd          - Main Window for the task to which
//                                  Object Services can send messages
//              lAppOE_Msg        - User message for the task which
//                                  object services can send and have
//                                  the task call ProcessZeidonMessage
//              cpcTaskTitle      - An identifying task title
//
//  RETURNS:    LPTASK - Address of task record created
//              0 (NULL)  - Could not create a task (bad news!)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
LPTASK
fnCreateTask( zLONG   lSystemInstance,
              zLONG   hMainWnd,
              zLONG   lAppOE_Msg,
              zCPCHAR cpcTaskTitle )
{
// LPPROCESS hProcess;
   LPTASK    lpTask;
   zPVOID    hTask;
   zCHAR     szTaskName[ 32 ];
   zCHAR     szMessage[ 60 ];

   // Allocate an application task record.
   hTask = fnAllocDataspace( (LPDATAHEADER) g_hAnchorBlock,
                             sizeof( TaskRecord ), 1, 0, iTask );
   lpTask = zGETPTR( hTask );

   // If a task record could not be acquired, return.
   if ( lpTask == 0 )
      return( 0 );  // return Failure

   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   // Add Application Task to Anchor Chain of tasks.
   lpTask->lHab             = lSystemInstance;
   lpTask->hMainWnd         = hMainWnd;
   lpTask->uAppOE_Msg       = (zUSHORT) lAppOE_Msg;
// lpTask->lTaskID          = (zLONG) hTask; // SysGetProcessID( );
   lpTask->bAudit           = TRUE;
   lpTask->bDebug           = TRUE;
   lpTask->bTrace           = TRUE;
   lpTask->nOperIdx         = -1;

   // Initialize szTaskName
   sprintf_s( szTaskName, zsizeof( szTaskName ), "App Task 0x%08x", (zULONG) hTask );

   // Task is disabled until we finish initializing it.
   lpTask->bDisable         = TRUE;

   // Chain Task to main anchor block BEFORE allocating dataspace.
   fnStartBrowseOfTaskList( hTask, TRUE ); // Get a 'write' lock on the task list.
   lpTask->hNextTask = AnchorBlock->hFirstTask;
   AnchorBlock->hFirstTask = hTask;
   fnEndBrowseOfTaskList( TRUE );

   // Create a dataspace for the task unless it is the main task.
   strcpy_s( szMessage, zsizeof( szMessage ), "(tm) "); // prefix message
   if ( AnchorBlock->hMainTask == 0 ||
        AnchorBlock->hMainTask == hTask )
   {
      strcat_s( szMessage, zsizeof( szMessage ), "Main " );
   }

   strcat_s( szMessage, zsizeof( szMessage ), szTaskName );
   strncpy_s( lpTask->szTaskTitle, zsizeof( lpTask->szTaskTitle ), cpcTaskTitle, 127 );
   lpTask->szTaskTitle[ 127 ] = 0;

#if 0
   hProcess = fnFindProcess( 0 );
   if ( hProcess == 0 )
      hProcess = fnCreateProcess( hMainWnd, lAppOE_Msg );

   lpTask->hProcess = hProcess;
#endif

   lpTask->hFirstDataHandle =
      fnInitializeDataspace( lpTask, (zCOREMEM) &lpTask->hFirstDataHeader,
                             iTask, szTaskName, zSHARED_ALLOC, 0 );

   // Reset pointer to handle.
   lpTask->hFirstDataHeader = (LPDATAHEADER) lpTask->hFirstDataHandle;

   if ( AnchorBlock->hMainTask == 0 )
   {
      AnchorBlock->hMainTask = hTask;
      AnchorBlock->hMainFirstDataHeader = lpTask->hFirstDataHeader;
   }

   fnCreateSubtask( lpTask, zGETPTR( lpTask->hApp ) );

   // If Dataspace initialization failed, delete the task and
   // return a bad RC if the task is not the Main task.
   if ( AnchorBlock->hMainTask &&
        AnchorBlock->hMainTask != hTask &&
        lpTask->hFirstDataHeader == 0 )
   {
      LPTASK lpFirstTask = zGETPTR( AnchorBlock->hFirstTask );

      // Unchain new task created.
      AnchorBlock->hFirstTask = lpFirstTask->hNextTask;
      fnFreeDataspace( lpTask );
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      return( 0 );  // return Failure
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( szMessage, " created" );

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   return( lpTask );  // return Success
}

//./ ADD NAME=fnFreeTaskDataspace
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnFreeTaskDataspace
//
//  PURPOSE:    To issue a free on all of the dataspaces for the
//              passed task.
//
//  PARAMETERS: lpTask - the pointer to the object services task
//
//  RETURNS:    0 - Task space freed
//             -1 - Task pointer passed is null
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnFreeTaskDataspace( LPTASK lpTask )
{
   // If we are running in server mode, there are no dataspaces to free ...
   // everything is allocated using local memory.
   //if ( g_bServerMode )
// {
   // return( 0 );  not quite ... we need to free the Data Headers
// }

   // Make sure pointer is not null
   if ( lpTask && lpTask->nTableID != iTask )
   {
      // "KZOEE016 - Invalid Task"
      fnIssueCoreError( lpTask, 0, 16, 16, 0, 0, 0 );
      return( -1 );
   }

   if ( lpTask )
   {
      LPDATAHEADER lpDataHeader;

      // Free all memory allocated for the task ... freeing the first data
      // header last.  DG ... is there a better way???
      lpDataHeader = zGETPTR( lpTask->hFirstDataHeader );
      if ( lpDataHeader )
      {
      // TraceLineS( "Freeing a page", "" );
         while ( lpDataHeader->hNextDataHeader )
            lpDataHeader = zGETPTR( lpDataHeader->hNextDataHeader );

         for ( ; ; )
         {
            if ( lpDataHeader->lNextDataHandle )
            {
#if 0
               if ( g_bServerMode )
                  SysFree( (zPVOID) lpDataHeader->lNextDataHandle );
               else
#endif
                  fnFreeSharedMemory( lpTask, lpDataHeader->lNextDataHandle );

               lpDataHeader->lNextDataHandle = 0;
            }

            lpDataHeader = zGETPTR( lpDataHeader->hPrevDataHeader );
            if ( lpDataHeader == 0 )
               break;
         }
      }

      if ( lpTask->hFirstDataHandle )
      {
#if 0
         if ( g_bServerMode )
            SysFree( (zPVOID) lpTask->hFirstDataHandle );
         else
#endif
            fnFreeSharedMemory( lpTask, lpTask->hFirstDataHandle );

         lpTask->hFirstDataHandle = 0;
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=fnDropTaskDomain
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropTaskDomain
//
//  PURPOSE:    To issue a free on all of the dataspaces for the
//              associated with TaskDomain struct
//
//  PARAMETERS: lpTask - the pointer to the object services task
//              lpTaskDomain - TaskDomain to drop
//
//  RETURNS:    0 - TaskDomain space freed
//            -16 - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnDropTaskDomain( LPTASK lpTask, LPTASKDOMAIN lpTaskDomain )
{
   LPDOMAIN     lpDomain;
// LPTASKDOMAIN hTaskDomain;
   LPTASKDOMAIN lpNextTaskDomain;
   LPTASKDOMAIN lpPrevTaskDomain;

   if ( lpTask == 0 || lpTaskDomain == 0 ) // ensure pointers are not null
      return( zCALL_ERROR );

   // Point to next item in the chain off the task structure.
   lpPrevTaskDomain = 0;
   lpNextTaskDomain = zGETPTR( lpTask->hFirstTaskDomain );
   while ( lpNextTaskDomain )
   {
      if ( lpTaskDomain == lpNextTaskDomain )
      {
         if ( lpPrevTaskDomain )
            lpPrevTaskDomain->hNextTaskDomainForTask = lpTaskDomain->hNextTaskDomainForTask;
         else
            lpTask->hFirstTaskDomain = lpTaskDomain->hNextTaskDomainForTask;

         break;
      }

      lpPrevTaskDomain = lpNextTaskDomain;
      lpNextTaskDomain = zGETPTR( lpNextTaskDomain->hNextTaskDomainForTask );
   }

   // Remove from the chain off of the Domain structure.
   lpDomain = zGETPTR( lpTaskDomain->hDomain ); // save domain
// TraceLine( "fnDropTaskDomain Task: 0x%08x   Domain: 0x%08x - %s",
//             zGETHNDL( lpTask ), lpTaskDomain, lpDomain->szName );

   lpNextTaskDomain = zGETPTR( lpTaskDomain->hNextTaskDomainForDomain );
   lpPrevTaskDomain = zGETPTR( lpTaskDomain->hPrevTaskDomainForDomain );

   // If no next link, then the previous one becomes the tail of the chain.
   if ( lpNextTaskDomain )
      lpNextTaskDomain->hPrevTaskDomainForDomain = zGETHNDL( lpPrevTaskDomain );
   else
      lpDomain->hLastTaskDomain = zGETHNDL( lpPrevTaskDomain );

   // If no previous link, then the next one becomes the head of the chain.
   if ( lpPrevTaskDomain )
      lpPrevTaskDomain->hNextTaskDomainForDomain = zGETHNDL( lpNextTaskDomain );
   else
      lpDomain->hFirstTaskDomain = zGETHNDL( lpNextTaskDomain );

#if 0  // cannot do this here ... will be done when application is deleted

   // If the domain has no other TaskDomain (other than AnchorBlock's MainTask)
   // pointing to it, delete it.
   if ( lpDomain->hApp != AnchorBlock->hSystemApp )
   {
      hTaskDomain = zGETHNDL( lpTaskDomain );
      if ( lpDomain->hFirstTaskDomain == 0 ||
           (lpDomain->hFirstTaskDomain == hTaskDomain &&
            lpDomain->hLastTaskDomain == hTaskDomain) )
      {
         LPAPP lpApp = zGETPTR( lpDomain->hApp );
         if ( lpApp )
         {
            LPDOMAIN lpDomainNext = zGETPTR( lpApp->hFirstDomain );
            LPDOMAIN lpDomainPrev = 0;

            // Clean up this domain.
            while ( lpDomainNext )
            {
               if ( lpDomain == lpDomainNext )
               {
                  if ( lpDomainPrev )
                     lpDomainPrev->hNextDomain = lpDomainNext->hNextDomain;
                  else
                     lpApp->hFirstDomain = lpDomainNext->hNextDomain;

                  TraceLine( "fnDropTaskDomain DELETING Domain for Task: 0x%08x   Domain: 0x%08x - %s",
                             zGETHNDL( lpTask ), lpTaskDomain, lpDomain->szName );
                  fnDeleteContext( lpDomain );
                  fnFreeDataspace( lpDomain );
                  break;
               }

               lpDomainPrev = lpDomainNext;
               lpDomainNext = zGETPTR( lpDomainPrev->hNextDomain );
            }
         }
      }
   }
#endif

   // Free the TaskDomain space.
   fnFreeDataspace( lpTaskDomain );
   return( 0 );
}

void OPERATION
fnDropTaskDomains( LPTASK lpTask )
{
   LPTASKDOMAIN lpTaskDomain = zGETPTR( lpTask->hFirstTaskDomain );
   LPTASKDOMAIN lpNextTaskDomain;
   zLONG        lProcessID = SysGetProcessID( 0 );

   while ( lpTaskDomain )
   {
      lpNextTaskDomain = zGETPTR( lpTaskDomain->hNextTaskDomainForTask );
      if ( lProcessID = lpTaskDomain->lProcessID && fnDropTaskDomain( lpTask, lpTaskDomain ) < 0 )
      {
         TraceLineS( "(tm) DeleteTask error dropping task domain", "" );
         break;  // some kind of problem has occurred ... punt!
      }

      lpTaskDomain = lpNextTaskDomain;
   }
}

//./ ADD NAME=fnDropTaskOperation
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropTaskOperation
//
//  PURPOSE:    To issue a free on all of the dataspaces for the
//              associated with TaskOperation struct.  We do not
//              do any process specific work (like FreeLibrary)
//              here ... we expect it to happen later if necessary.
//
//  PARAMETERS: lpTask - the pointer to the object services task
//              lpTaskOperation - TaskOperation to drop
//
//  RETURNS:    0 - TaskObjectOper space freed
//            -16 - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT LOCALOPER
fnDropTaskOperation( LPTASK lpTask, LPTASKOPER lpTaskOperation )
{
   LPTASKOPER  hTaskOperation = zGETHNDL( lpTaskOperation );
   LPTASKOPER  lpNextTaskOperation;
   LPTASKOPER  lpPrevTaskOperation;
// zBOOL       bMutexLocked;

   // Make sure pointers are not null.
   if ( lpTaskOperation == 0 )
      return( zCALL_ERROR );

   if ( lpTask == 0 )
   {
      lpTask = zGETPTR( lpTaskOperation->hTask );
      if ( lpTask == 0 )
         return( zCALL_ERROR );
   }

#if 0
   // We don't need this since fnDropTaskOperation is only called through
   // DeleteTask which already is protected by the ANCHORBLOCK mutex.
   if ( zGETHNDL( lpTask ) == AnchorBlock->hMainTask )  // added dks
   {
      bMutexLocked = TRUE;
      zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   }
   else
      bMutexLocked = FALSE;
#endif

   // Remove the TaskOperation from the operation list for the task.
   if ( lpTask->hFirstTaskOperation == hTaskOperation )
      lpTask->hFirstTaskOperation = lpTaskOperation->hNextTaskOperationForTask;
   else
   {
      for ( lpPrevTaskOperation = zGETPTR( lpTask->hFirstTaskOperation );
            lpPrevTaskOperation->hNextTaskOperationForTask != hTaskOperation;
            lpPrevTaskOperation =
                  zGETPTR( lpPrevTaskOperation->hNextTaskOperationForTask ) )
      {
         // Nothing needs to be done here.
      }

      lpPrevTaskOperation->hNextTaskOperationForTask =
                                    lpTaskOperation->hNextTaskOperationForTask;
   }

// if ( bMutexLocked )
//    zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( lpTaskOperation->hViewOD )
   {
      LPVIEWOD lpViewOD;

      // Remove from the chain off of the ViewOD structure.
      lpViewOD = zGETPTR( lpTaskOperation->hViewOD ); // save ViewAttribute
      lpNextTaskOperation =
                     zGETPTR( lpTaskOperation->hNextTaskOperationForOEA );
      lpPrevTaskOperation =
                     zGETPTR( lpTaskOperation->hPrevTaskOperationForOEA );  // blew on this line 2006.07.03 dks

      // If no next link, then the previous one becomes the tail of the chain.
      if ( lpNextTaskOperation )
         lpNextTaskOperation->hPrevTaskOperationForOEA = zGETHNDL( lpPrevTaskOperation );
      else
         lpViewOD->hLastTaskOperation = zGETHNDL( lpPrevTaskOperation );

      // If no previous link, then the next one becomes the head of the chain.
      if ( lpPrevTaskOperation )
         lpPrevTaskOperation->hNextTaskOperationForOEA = zGETHNDL( lpNextTaskOperation );
      else
         lpViewOD->hFirstTaskOperation = zGETHNDL( lpNextTaskOperation );
   }
   else
   if ( lpTaskOperation->hViewEntity )
   {
      LPVIEWENTITY   lpViewEntity;

      // Remove from the chain off of the ViewEntity structure.
      lpViewEntity = zGETPTR( lpTaskOperation->hViewEntity ); // save ViewAttribute
      lpNextTaskOperation =
                        zGETPTR( lpTaskOperation->hNextTaskOperationForOEA );
      lpPrevTaskOperation =
                        zGETPTR( lpTaskOperation->hPrevTaskOperationForOEA );

      // If no next link, then the previous one becomes the tail of the chain.
      if ( lpNextTaskOperation )
         lpNextTaskOperation->hPrevTaskOperationForOEA = zGETHNDL( lpPrevTaskOperation );
      else
         lpViewEntity->hLastTaskOperation = zGETHNDL( lpPrevTaskOperation );

      // If no previous link, then the next one becomes the head of the chain.
      if ( lpPrevTaskOperation )
         lpPrevTaskOperation->hNextTaskOperationForOEA = zGETHNDL( lpNextTaskOperation );
      else
         lpViewEntity->hFirstTaskOperation = zGETHNDL( lpNextTaskOperation );
   }
   else
   if ( lpTaskOperation->hViewAttrib )
   {
      LPVIEWATTRIB lpViewAttrib;

      // Remove from the chain off of the ViewAttrib structure.
      lpViewAttrib = zGETPTR( lpTaskOperation->hViewAttrib ); // save ViewAttribute
      lpNextTaskOperation =
                     zGETPTR( lpTaskOperation->hNextTaskOperationForOEA );
      lpPrevTaskOperation =
                     zGETPTR( lpTaskOperation->hPrevTaskOperationForOEA );

      // If no next link, then the previous one becomes the tail of the chain.
      if ( lpNextTaskOperation )
         lpNextTaskOperation->hPrevTaskOperationForOEA = zGETHNDL( lpPrevTaskOperation );
      else
         lpViewAttrib->hLastTaskOperation = zGETHNDL( lpPrevTaskOperation );

      // If no previous link, then the next one becomes the head of the chain.
      if ( lpPrevTaskOperation )
         lpPrevTaskOperation->hNextTaskOperationForOEA = zGETHNDL( lpNextTaskOperation );
      else
         lpViewAttrib->hFirstTaskOperation = zGETHNDL( lpNextTaskOperation );
   }
   else
   {
      // "KZOEE021 - Internal error, TaskOperation block has no
      //  Object/Entity/Attr"
      fnIssueCoreError( lpTask, 0, 16, 21, 0, 0, 0 );
   }

   fnFreeDataspace( lpTaskOperation );
   return( 0 );
}

void OPERATION
fnDropTaskOperations( LPTASK lpTask )
{
   while ( lpTask->hFirstTaskOperation )
   {
      if ( fnDropTaskOperation( lpTask,
                                zGETPTR( lpTask->hFirstTaskOperation ) ) < 0 )  // blew on this line 2006.07.03 dks
      {
         TraceLineS( "(tm) DeleteTask error dropping task operation", "" );
         break;  // some kind of problem has occurred ... punt!
      }
   }
}

//./ ADD NAME=SfCloseTaskDatabases
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   SfCloseTaskDatabases
//
//  PURPOSE:    To issue a DBH_TaskCloseAll message to all database handlers
//              currently in the task.
//
//  PARAMETERS: NONE
//
//  RETURNS:    0 - All database handlers active for task called
//              zCALL_ERROR - invalid task
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfCloseTaskDatabases( zVIEW vTaskView )
{
   LPTASKDBHANDLER lpTaskDBHandler;
   LPTASK          lpTask;
   zPDBHOPER       pDBOper;
   zPVOID          lpPtr;

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
   {
      TraceLine( "SfCloseTaskDatabases for Process: %d   Task: %d (0x%08x)",
                 SysGetProcessID( 0 ), vTaskView->hTask, vTaskView->hTask );
   }

   lpTask = zGETPTR( vTaskView->hTask );
   if ( lpTask == 0 )
      return( zCALL_ERROR );

   SysMutexLock( vTaskView, "Zeidon DBH", 0, 0 );

   lpTaskDBHandler = zGETPTR( lpTask->hFirstTaskDBHandler );
   while ( lpTaskDBHandler )
   {
      lpPtr = lpTaskDBHandler->lpConnection;

      // Call DBHandler Close routine to close any open connections.
      pDBOper = lpTaskDBHandler->pfDBHandlerOper;
      (*pDBOper)( (zLONG) vTaskView->hTask, DBH_TaskCloseAll,
                  0, 0, 0, 0, 0, &lpPtr );
      lpTaskDBHandler->lpConnection = 0;

      lpTaskDBHandler = zGETPTR( lpTaskDBHandler->hNextTaskDBHandler );
   }

   SysMutexUnlock( vTaskView, "Zeidon DBH", 0 );

   return( 0 );
}

//./ ADD NAME=fnDropTaskHandlers
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropTaskHandlers
//
//  PURPOSE:    To issue a Terminate Transaction to the database handler
//              (to close any open databases) and free any space used by
//              the TaskDBHandler.
//
//              Performs the same thing for Genkey handlers.
//
//              NOTE: This function assumes that lpTaskDBHandler and
//              lpTaskGKHandler are the first handlers for the task.
//
//  PARAMETERS: lpTask - the pointer to the object services task
//              lpTaskDBHandler - TaskDBHandler to drop
//
//  RETURNS:    0 - TaskDomain space freed
//            -16 - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
fnDropTaskHandlers( LPTASK lpTask )
{
   LPTASK          lpMainTask = zGETPTR( AnchorBlock->hMainTask );
   LPTASK          hTask = zGETHNDL( lpTask );
   LPTASKDBHANDLER lpTaskDBHandler;
   LPTASKGKHANDLER lpTaskGKHandler;
   zPDBHOPER       pDBOper;
   zPGKHOPER       pGKOper;
   zPVOID          lpPtr;
   zVIEW           vTask;
   zSHORT          nRC = 0;
   zLONG           lProcessID = SysGetProcessID( 0 );

   if ( lpTask == 0 || lpTask->hFirstView == 0 )
      return( zCALL_ERROR );

   vTask = zGETPTR( lpTask->hFirstView );
   if ( lpTask != lpMainTask )
      SysMutexLock( vTask, "Zeidon DBH", 0, 0 );

   lpTaskDBHandler = zGETPTR( lpTask->hFirstTaskDBHandler );
   while ( lpTaskDBHandler )
   {
      if ( lpTaskDBHandler->lProcessID == lProcessID )
      {
         lpPtr = lpTaskDBHandler->lpConnection;

         // Call DBHandler routine to terminate database processing.
         pDBOper = lpTaskDBHandler->pfDBHandlerOper;
         if ( (*pDBOper)( (zLONG) hTask, DBH_Term,
                          0, 0, 0, 0, vTask, &lpPtr ) < 0 )
         {
            nRC = zCALL_ERROR;  // continue to try to drop handlers
         }
      }

      // Point to next item in the chain.
      lpTask->hFirstTaskDBHandler = lpTaskDBHandler->hNextTaskDBHandler;

      // Free the TaskDBHandler space.
      fnFreeDataspace( lpTaskDBHandler );

      lpTaskDBHandler = zGETPTR( lpTask->hFirstTaskDBHandler );
   }

   if ( lpTask != lpMainTask )
   {
      SysMutexUnlock( vTask, "Zeidon DBH", 0 );
      SysMutexLock( vTask, "Zeidon GKH", 0, 0 );
   }

   lpTaskGKHandler = zGETPTR( lpTask->hFirstTaskGKHandler );
   while ( lpTaskGKHandler )
   {
      if ( lpTaskGKHandler->lProcessID == lProcessID )
      {
         lpPtr = lpTaskGKHandler->lpConnection;

         // Call GKHandler routine to terminate database processing.
         pGKOper = lpTaskGKHandler->pfGKHandlerOper;
         if ( (*pGKOper)( DBH_Term, 0, 0, 0, 0 ) < 0 )
         {
            nRC = zCALL_ERROR;  // continue to try to drop handlers
         }
      }

      // Point to next item in the chain.
      lpTask->hFirstTaskGKHandler = lpTaskGKHandler->hNextTaskGKHandler;

      // Free the TaskGKHandler space.
      fnFreeDataspace( lpTaskGKHandler );

      lpTaskGKHandler = zGETPTR( lpTask->hFirstTaskGKHandler );
   }

   if ( lpTask != lpMainTask )
      SysMutexUnlock( vTask, "Zeidon GKH", 0 );

   return( nRC );

} // fnDropTaskHandlers

//./ ADD NAME=fnDropTaskApp
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropTaskApp
//
//  PURPOSE:    To issue a free on all of the dataspaces for the
//              associated with TaskApp struct
//
//  PARAMETERS: lpTask - the pointer to the object services task
//              lpTaskObjectOper - TaskObjectOper to drop
//
//  RETURNS:    0 - TaskObjectOper space freed
//            -16 - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnDropTaskApp( LPTASK lpTask, LPTASKAPP lpTaskApp )
{
   if ( lpTask && lpTaskApp )  // make sure pointers are not null
   {
      // Point to next item in the chain off the task structure.
      lpTask->hFirstTaskApp = lpTaskApp->hNextTaskApp;

      // If MQ Interface pointers, drop them.
      if ( lpTaskApp->hMQInterface )
         fnFreeDataspace( zGETPTR( lpTaskApp->hMQInterface ) );

      // Free the TaskApp space.
      fnFreeDataspace( lpTaskApp );
      return( 0 );
   }

   return( zCALL_ERROR );
}

//./ ADD NAME=SetTaskDBHandlerTraceLevel
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetTaskDBHandlerTraceLevel
//
//  PURPOSE:    To set the dbhandler tracing level for the task.
//
//              The following shows trace values and there effects:
//
//                0 - No tracing at all (default)
//                1 - Minimal: traces SQL commands, etc.
//                2 - Debug: traces most DB API calls and gives a feel for
//                    the flow of the data access.
//                3 - Debug extra: traces all DB API calls as well as extra
//                    info that (may) require(s) additional API calls.
//
//  RETURNS:    0           - Tracing level set.
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT  OPERATION
SetTaskDBHandlerTraceLevel( zVIEW  lpView, zLONG lTraceLevel )
{
   LPTASK lpCurrentTask;
   LPTASK lpTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetTaskDBHandlerTraceLevel,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpTask = zGETPTR( lpView->hTask );
   lpTask->nDBHandlerTraceLevel = (zSHORT) lTraceLevel;

   fnOperationReturn( iSetTaskDBHandlerTraceLevel, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=GetTaskDBHandlerTraceLevel
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetTaskDBHandlerTraceLevel
//
//  PURPOSE:    To get the dbhandler tracing level for the task.
//
//  RETURNS:    >= 0        - Current trace level for task.
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT  OPERATION
GetTaskDBHandlerTraceLevel( zVIEW  lpView )
{
   LPTASK lpCurrentTask;
   LPTASK lpTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetTaskDBHandlerTraceLevel,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpTask = zGETPTR( lpView->hTask );

   fnOperationReturn( iGetTaskDBHandlerTraceLevel, lpCurrentTask );
   return( lpTask->nDBHandlerTraceLevel );
}

#if 0
void
DisplayDbHandlerWork( zCPCHAR cpcTitle, zVIEW lpView, zLONG hTask )
{
   zVIEW        vDbhWork;
   LPTASK       lpTask = zGETPTR( hTask );

   GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME, lpView, zLEVEL_SYSTEM );
   if ( vDbhWork )
   {
      TraceLine( "%s   DBHandlerWork for Task: 0x%08x", cpcTitle, hTask );
      DisplayObjectInstance( vDbhWork, 0, 0 );
      TraceLineS( "", "" );
   }
   else
      TraceLineS( "Cannot find DbHandler work object: ", cpcTitle );
   }
}
#endif

#if 1
#define fnValidateTaskChains( a, b )
#else
//./ ADD NAME=fnValidateTaskChains
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidateTaskChains
//
//  PURPOSE:    Run chains for a task (all tasks if hTaskToValidate is null).
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
fnValidateTaskChains( LPTASK hTaskToValidate, zCPCHAR cpcText )
{
   LPTASK      hTask;
   LPTASK      lpTask;
   LPAPP       hApp;
   LPAPP       lpApp;
   LPSUBTASK   hSubtask;
   LPSUBTASK   lpSubtask;
   LPVIEWNAME  lpViewName;

   TraceLine( "ValidateTaskChains (%s) Task to validate: 0x%08x",
              cpcText, hTaskToValidate );

   // Loop through the tasks.  Note that we don't need to worry about a
   // task being deleted while we are here because we are protected by
   // the ANCHORBLOCK mutex ... as of 2006.07.06, this is not called ... dks
   hTask = AnchorBlock->hFirstTask;
   while ( hTask )
   {
      lpTask = zGETPTR( hTask );
      if ( hTaskToValidate == 0 || hTaskToValidate == hTask )
      {
         // Check all names for the view 1: first go through all the subtasks and
         // find all names for the view at any subtask level within the task.
         hSubtask = lpTask->hFirstSubtask;
         while ( hSubtask )
         {
            lpSubtask = zGETPTR( hSubtask );
            lpViewName = zGETPTR( lpSubtask->hFirstViewName );
            while ( lpViewName )
            {
               TraceLine( "ValidateTaskChains1 Task: 0x%08x   Subtask: 0x%08x   View: 0x%08x   Name: %s",
                          hTask, hSubtask, lpViewName->hView, lpViewName->szName );
               lpViewName = zGETPTR( lpViewName->hNextViewName );
            }

            hSubtask = lpSubtask->hNextSubtask;
         }

         // Check all names for the view 2: if a name was created at the task
         // level, go through all names for the owning task.
         lpViewName = zGETPTR( lpTask->hFirstViewName );
         while ( lpViewName )
         {
            TraceLine( "ValidateTaskChains2 Task: 0x%08x   View: 0x%08x   Name: %s",
                       hTask, lpViewName->hView, lpViewName->szName );
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // Check all names for the view 3: if a name was created at the
         // application level, go through all names for the application.
         hApp = lpTask->hApp;
         lpApp = zGETPTR( hApp );
         lpViewName = zGETPTR( lpApp->hFirstViewName );
         while ( lpViewName )
         {
            TraceLine( "ValidateTaskChains3 Task: 0x%08x   App: 0x%08x   View: 0x%08x   Name: %s",
                       hTask, hApp, lpViewName->hView, lpViewName->szName );
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // Check all names for the view 4: if a name was created at
         // the system level, go through all names for the system.
         lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
         while ( lpViewName )
         {
            TraceLine( "ValidateTaskChains4 Task: 0x%08x   View: 0x%08x   Name: %s",
                       hTask, lpViewName->hView, lpViewName->szName );
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // Check all names for the view 5: if a name was created at the
         // application level, go through all apps and see if the view
         // can be found in one of them.
         hApp = AnchorBlock->hFirstApp;
         while ( hApp )
         {
            lpApp = zGETPTR( hApp );
            lpViewName = zGETPTR( lpApp->hFirstViewName );
            while ( lpViewName )
            {
               TraceLine( "ValidateTaskChains5 Task: 0x%08x   App: 0x%08x   View: 0x%08x   Name: %s",
                          hTask, hApp, lpViewName->hView, lpViewName->szName );
               lpViewName = zGETPTR( lpViewName->hNextViewName );
            }

            hApp = lpApp->hNextApp;
         }
      }

      hTask = lpTask->hNextTask;
   }

   return( 0 );
}
#endif

#if 0
//./ ADD NAME=fnCheckTaskForOrphanOI
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnCheckTaskForOrphanOI
//
//  PURPOSE:    Check to see if this is the owner task for any existing OI.
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
fnCheckTaskForOrphanOI( LPTASK hDeletedTask, zCPCHAR cpcText )
{
   LPTASK    hTask;
   LPTASK    lpTask = zGETPTR( hDeletedTask );
   LPAPP     hApp = lpTask->hApp;
   LPVIEWOI  lpViewOI;
   zVIEW     lpView;
   LPVIEWCSR lpViewCsr;

   if ( cpcText && cpcText[ 0 ] )
   {
   // TraceLine( "CheckTaskForOrphanOI %s validating Task: 0x%08x",
   //            cpcText, hDeletedTask );
   }

   hTask = AnchorBlock->hFirstTask;
   while ( hTask )
   {
      lpTask = zGETPTR( hTask );
      lpViewOI = zGETPTR( lpTask->hFirstViewOI );
      while ( lpViewOI )
      {
         lpViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );
         while ( lpViewCsr )
         {
            if ( lpViewOI != zGETPTR( lpViewCsr->hViewOI ) )
            {
               if ( cpcText && cpcText[ 0 ] )
                  SysMessageBox( 0, "CheckTaskForOrphanOI", "View OI / View Csr mismatch???", 0 );
               else
                  TraceLine( "CheckTaskForOrphanOI View OI / View Csr mismatch 0x%08x ... 0x%08x ???",
                             lpViewOI, zGETPTR( lpViewCsr->hViewOI ) );
            }

            lpView = zGETPTR( lpViewCsr->hView );
         // if ( lpView->hTask != hTask )
         //    SysMessageBox( 0, "CheckTaskForOrphanOI", "View / Task mismatch???", 0 );

            if ( hDeletedTask == lpView->hTask )
            {
               if ( cpcText && cpcText[ 0 ] )
                  SysMessageBox( 0, "CheckTaskForOrphanOI", "View found for deleted task", 0 );
               else
               {
               // TraceLine( "CheckTaskForOrphanOI View found for deleted task ... deleting view" );
                  DropView( lpView );
                  lpViewOI = zGETPTR( lpTask->hFirstViewOI );
                  goto NEXT_OI_label;  // jump out of inner loop and do not reset lpViewOI
               }
            }

            lpViewCsr = zGETPTR( lpViewCsr->hNextViewCsr );
         }

         lpViewOI = zGETPTR( lpViewOI->hNextViewOI );
NEXT_OI_label:
         ;
      }

      hTask = lpTask->hNextTask;
   }

   return( 0 );
}
#endif

// #define DEBUG_TASKPROCESS

void OPERATION
fnCloseTaskNetworkModules( LPTASK lpTask )  // free libraries only
{
#if 0
   LPNETWORK lpNetwork;

   // Free any libraries for the networks.
   lpNetwork = zGETPTR( lpTask->hFirstNetwork );
   while ( lpNetwork )
   {
      // If we have a server directory function, use it.
      if ( lpNetwork->hServDirLib && lpNetwork->lpfnServDirGetAddr )
      {
         fnSysFreeLibrary( lpTask, zGETPTR( lpNetwork->hServDirLib ), FALSE );
         lpNetwork->hServDirLib = 0;
      }

      lpNetwork = zGETPTR( lpNetwork->hNextNetwork );
   }
#endif
}

void OPERATION
fnCloseTaskOpenFiles( LPTASK lpTask )
{
   LPOPENFILE lpOpenFile;

   while ( lpTask->hFirstOpenFile )
   {
      lpOpenFile = zGETPTR( lpTask->hFirstOpenFile );

      if ( fnSysCloseFile( lpTask, lpOpenFile->lFileHandle, lpTask ) < 0 )
      {
         TraceLineS( "(tm) DeleteTask error closing file", "" );
         break;  // some kind of problem has occurred ... punt!
      }
   }
}

void OPERATION
fnFreeTaskModules( LPTASK lpTask )
{
   LPLIBRARY lpLibrary;
   LPLIBRARY lpLibraryNext;
   LPTASK    hTask = zGETHNDL( lpTask );
   zLONG     lProcessID = SysGetProcessID( 0 );
   zLONG     lRC;

   // Free all modules loaded for this process.
   while ( lpTask->hFirstLibrary )
   {
      lpLibrary = zGETPTR( lpTask->hFirstLibrary );
      while ( lpLibrary )
      {
         do
            lRC = fnSysFreeLibrary( lpTask, lpLibrary, TRUE );
         while ( lRC > 1 );  // account for multiple loads

         if ( lRC < 0 )
         {
            TraceLineS( "(tm) DeleteTask error freeing library", "" );
            break;  // some kind of problem has occurred ... punt!
         }

         lpLibraryNext = zGETPTR( lpTask->hFirstLibrary );
         if ( lpLibraryNext == lpLibrary )
         {
            fnSysMessageBox( lpTask, "Zeidon Library Error",
                             "Error freeing library", 1 );
            TraceLineS( "(tm) DeleteTask error freeing library", "" );
            break;  // some kind of problem has occurred ... punt!
         }

         lpLibrary = lpLibraryNext;
      }
   }
}

void OPERATION
fnDropTaskMutexes( LPTASK lpTask )
{
   LPTASK   lpMainTask = zGETPTR( AnchorBlock->hMainTask );
   LPMUTEX  lpMutex;
   zLONG    lProcessID = SysGetProcessID( 0 );

#if 0  // I don't know why this is deprecated ... dks 2004.12.27

   // Drop all task mutexes.  Note that we don't care about freeing memory
   // taken up by mutex structures because we'll soon free it with the task.

   if ( AnchorBlock->hFirstMutex )
   {
      LPMUTEXTASK lpMutexTaskPrev;
      LPMUTEXTASK hMutexTaskNext;
      LPMUTEXTASK lpMutexTask;

      // Set lock to make sure nobody's changing the mutex chains.
      zLOCK_MUTEX( zMUTEX_MUTEX );

      // Pass through all mutexes.
      lpMutex = zGETPTR( AnchorBlock->hFirstMutex );
      while ( lpMutex )
      {
         lpMutexTaskPrev = 0;

         // Try finding task mutexes.
         lpMutexTask = zGETPTR( lpMutex->hFirstMutexTask );
         while ( lpMutexTask )
         {
            hMutexTaskNext = lpMutexTask->hNextMutexTask;
            if ( lpMutexTask->hTask == hTask )            // in the correct task
            {
               if ( lpMutexTaskPrev )
                  lpMutexTaskPrev->hNextMutexTask = hMutexTaskNext;
               else
                  lpMutex->hFirstMutexTask = hMutexTaskNext;

               if ( lpMutexTask->hMutexOS )
               {
                  if ( SysMutexClose( lpMutexTask->hMutexOS ) == 0 )
                     TraceLineS( "Error destroying mutex: ", lpMutex->szName );
               }

               fnFreeDataspace( lpMutexTask );
            }
            else
            {
               lpMutexTaskPrev = lpMutexTask;
            }

            lpMutexTask = zGETPTR( hMutexTaskNext );
         }

         lpMutex = zGETPTR( lpMutex->hNextMutex );
      }

      zUNLOCK_MUTEX( zMUTEX_MUTEX );
   }

#else

   zLOCK_MUTEX( zMUTEX_MUTEX );

   // Drop all task mutexes.  Note that we don't care about freeing memory
   // taken up by mutex structures because we'll soon free it with the task.
   // Ah shucks ... go ahead and free it just to make me happy. // dks 2006.02.24
   if ( lpTask->hFirstMutex )
   {
      while ( (lpMutex = zGETPTR( lpTask->hFirstMutex )) != 0 )
      {
         if ( lpMutex->nLockCount )
         {
            // First set nLockCount to 1 so that the next call to
            // fnSysMutexUnlock actually unlocks the mutex.
            lpMutex->nLockCount = 1;
            fnSysMutexUnlock( lpTask, lpMutex );
         }

         fnSysMutexDestroy( lpTask, lpMutex );

         lpTask->hFirstMutex = lpMutex->hNextMutex;
         fnFreeDataspace( lpMutex );
      }
   }

   // We only dump the system mutexes when we are destroying the main task.
   // The system mutexes are only used to track the lLockedTaskID.
   if ( lpTask == lpMainTask )
   {
      LPMUTEX  lpMutexPrev = 0;
      LPMUTEX  lpMutexNext;

      // We're dropping the main task so let's dump all the system mutexes.
      for ( lpMutex = zGETPTR( AnchorBlock->hFirstSysMutex );
            lpMutex;
            lpMutex = lpMutexNext )
      {
         lpMutexNext = zGETPTR( lpMutex->hNextMutex );
         if ( lpMutex->lProcessID != lProcessID )
         {
            lpMutexPrev = lpMutex;
            continue;
         }

         if ( lpMutex->nLockCount )
         {
            // First set nLockCount to 1 so that the next call to
            // fnSysMutexUnlock actually unlocks the mutex.
            lpMutex->nLockCount = 1;
            fnSysMutexUnlock( lpTask, lpMutex );
         }

         if ( lpMutexPrev )
            lpMutexPrev->hNextMutex = lpMutex->hNextMutex;
         else
            AnchorBlock->hFirstSysMutex = lpMutex->hNextMutex;

         // Destroy the mutex--pass 0 for lpTask to indicate that we're
         // destroying system mutexes.
         fnSysMutexDestroy( 0, lpMutex );  // this call will do nothing
         fnFreeDataspace( lpMutex );
      }
   }

   zUNLOCK_MUTEX( zMUTEX_MUTEX );

#endif
}

zPVOID
fnChainFreespaces( LPDATAHEADER lpDataHeader);

zULONG
fnBuildHexAscii( zPCHAR pchHex, zPCHAR pchAscii, zCPCHAR cpcBuffer,
                 zULONG ulPos, zULONG ulLth )
{
   zCHAR   sz[ 256 ];
   zSHORT  nLthHex;
   zSHORT  nLthAscii;
   zULONG  k;

   pchHex[ 0 ] = 0;
   pchAscii[ 0 ] = 0;
   nLthHex = 0;
   nLthAscii = 0;

   // Loop through each byte in the buffer and display a block of ulLth bytes
   // on a line in the trace window.
   for ( k = 0; k < (zsizeof( sz ) - 2) && ulPos < ulLth; k++, ulPos++ )
   {
      if ( k && (k % 8) == 0 )
         strcat_s( pchHex, 1024, "  " );

      // Convert the current byte to a 2-char hex number.
      zltox( (zLONG) cpcBuffer[ ulPos ], sz, zsizeof( sz ) );

      // Add the hex number to pchHex.
      if ( sz[ 0 ] == 0 || sz[ 1 ] == 0 )
      {
         pchHex[ nLthHex++ ] = '0';
         if ( sz[ 0 ] == 0 )
            pchHex[ nLthHex++ ] = '0';
         else
            pchHex[ nLthHex++ ] = sz[ 0 ];
      }
      else
      {
         pchHex[ nLthHex++ ] = sz[ 0 ];
         pchHex[ nLthHex++ ] = sz[ 1 ];
      }

      pchHex[ nLthHex++ ] = ' ';
      pchHex[ nLthHex ] = 0;

      // If the current byte is a printable char, then copy it to the ASCII
      // buffer.  Otherwise add a "." to the buffer.
      if ( cpcBuffer[ ulPos ] >= 32 && cpcBuffer[ ulPos ] <= 126 )
         pchAscii[ nLthAscii++ ] = cpcBuffer[ ulPos ];
      else
         pchAscii[ nLthAscii++ ] = '.';

      pchAscii[ nLthAscii ] = 0;
   }

   return( ulPos );
}

// Protected by the ANCHORBLOCK mutex.
zSHORT
fnCleanupTask( LPTASK lpTask )
{
   // TraceLine( "CleanupTask         Task: 0x%08x   Process: %d",
   //            hTask, lProcessID );

   if ( lpTask->hFirstLibrary )
   {
      TraceLineS( "CleanupTask unexpected Library", "" );
      fnFreeTaskModules( lpTask );
   }

   if ( lpTask->hFirstOpenFile )
   {
      TraceLineS( "CleanupTask unexpected OpenFile", "" );
   }

   if ( lpTask->hFirstTaskDBHandler )
   {
      TraceLineS( "CleanupTask unexpected DBHandler", "" );
   }

   if ( lpTask->hFirstTaskGKHandler )
   {
      TraceLineS( "CleanupTask unexpected GKHandler", "" );
   }

   if ( lpTask->hFirstTaskDomain )
   {
      TraceLineS( "CleanupTask unexpected Domain", "" );
   }

#if 1 // debugging code

   if ( lpTask->bWebTrace )
   {
      LPAPP      lpApp;
      LPSUBTASK  lpSubtask;
      LPVIEWNAME lpViewName;
      LPVIEWOI   lpViewOI;
      LPVIEWOD   lpViewOD;
      zVIEW      lpView;
      LPDATAHEADER lpDataHeader;
   // LPFREESPACE lpNextFreespace;
   // LPFREESPACE lpPrevFreespace;
   // LPFREESPACE lpFreespace; // actual space
   // zPVOID      pvEndOfBuffer;
   // zCHAR       szHex[ 1024 ];
   // zCHAR       szAscii[ 256 ];
   // zCHAR       szMsg[ 2048 ];
   // zCPCHAR     cpcText;

      zLOCK_MUTEX( zMUTEX_MEMORY );

      TraceLineS( "", "" );
      TraceLine( "After Cleanup ==================================>> Task: 0x%08x   Task: 0x%08x",
                 zGETHNDL( lpTask ), lpTask );
      lpTask = zGETPTR( AnchorBlock->hFirstTask );
      while ( lpTask )
      {
         TraceLine( "   Task: 0x%08x              Task: 0x%08x", zGETHNDL( lpTask ), lpTask );

         lpSubtask = zGETPTR( lpTask->hFirstSubtask );
         while ( lpSubtask )
         {
            lpApp = zGETPTR( lpSubtask->hApp );
            TraceLine( "      Subtask: 0x%08x   Task: 0x%08x   App: 0x%08x (%s)   Subtask View: 0x%08x",
                       lpSubtask, lpSubtask->hTask, lpSubtask->hApp, lpApp ? lpApp->szName : "???", lpSubtask->hSubtaskView );

            lpViewName = zGETPTR( lpSubtask->hFirstViewName );
            while ( lpViewName )
            {
               TraceLine( "         ViewName: 0x%08x   View: 0x%08x   View Name %s",
                          lpViewName, lpViewName->hView, lpViewName->szName );

               lpViewName = zGETPTR( lpViewName->hNextViewName );
            }

            lpSubtask = zGETPTR( lpSubtask->hNextSubtask );
         }

         lpViewOI = zGETPTR( lpTask->hFirstViewOI );
         while ( lpViewOI )
         {
            lpViewOD = zGETPTR( lpViewOI->hViewOD );
            TraceLine( "      ViewOI: 0x%08x   Task: 0x%08x   Alloc Task: 0x%08x   ViewOD: 0x%08x  (%s)",
                       lpViewOI, lpViewOI->hTask, lpViewOI->hAllocTask, lpViewOI->hViewOD, lpViewOD->szName );

            lpViewOI = zGETPTR( lpViewOI->hNextViewOI );
         }

         lpViewName = zGETPTR( lpTask->hFirstViewName );
         while ( lpViewName )
         {
            TraceLine( "         ViewName: 0x%08x   View: 0x%08x   View Name %s",
                       lpViewName, lpViewName->hView, lpViewName->szName );

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         lpView = zGETPTR( lpTask->hFirstView );
         while ( lpView )
         {
            lpViewOD = zGETPTR( lpView->hViewOD );
            TraceLine( "         View: 0x%08x   Task: 0x%08x   ViewOD: 0x%08x (%s)  Name Cnt: %d",
                       lpView, lpView->hTask, lpView->hViewOD, lpViewOD ? lpViewOD->szName : "???",
                       lpView->nViewNames );

            lpView = zGETPTR( lpView->hNextView );
         }

         lpDataHeader = zGETPTR( lpTask->hFirstDataHeader );
         while ( lpDataHeader )
         {
            fnChainFreespaces( lpDataHeader);
            TraceLine( "      DataHeader: 0x%08x   Nbr: %d   Total Size: %d"
                          "   Used Size: %d   Max Free Blocksize: %d"
                          "   Used Blocks: %d   Free Blocks: %d",
                       lpDataHeader,
                       (unsigned long) lpDataHeader->hDataHandle >> 24,
                       lpDataHeader->ulUseableSize,
                       lpDataHeader->ulUsedSpace,
                       lpDataHeader->ulMaxEmptyBlockSize,
                       lpDataHeader->ulCntUsedEntries,
                       lpDataHeader->ulCntFreeSpaces );
#if 0
#if 0
         // TraceDataHeader( lpDataHeader, FALSE ); // trace this header
            lpPrevFreespace = &lpDataHeader->Freespace;
            lpNextFreespace = zGETPTR( lpPrevFreespace->hNextFreespace );
            while ( lpNextFreespace )
            {
               TraceLine( "         %s: 0x%08x   Lth: %8d   Next: 0x%08x",
                          lpNextFreespace->nTableID == iFreespace ? "Free Space" :
                          lpNextFreespace->nTableID == iUsedFreespace ? "Used Space" : "???? Space",
                          lpNextFreespace, lpNextFreespace->ulFreeLth,
                          ((zPCHAR) lpNextFreespace) + lpNextFreespace->ulFreeLth );

               lpNextFreespace = zGETPTR( lpNextFreespace->hNextFreespace );
            }
#else
            // Trace out the freespace here.
            lpFreespace = (LPFREESPACE) ((zPCHAR) lpDataHeader + sizeof( DataHeaderRecord ));
            pvEndOfBuffer = (zPCHAR) lpDataHeader + sizeof( DataHeaderRecord ) +
                                                                 lpDataHeader->ulUseableSize;
            while ( (void *) lpFreespace < pvEndOfBuffer )
            {
               if ( lpFreespace->nTableID == iUsedFreespace ||
                    lpFreespace->nTableID == iUsedFreespace1 )
               {
                  fnBuildHexAscii( szHex, szAscii, (zCPCHAR) zGETPTR( lpFreespace->hFreespace ), 0,
                                   lpDataHeader->ulUseableSize );
                                // lpDataHeader->ulUseableSize >= 32 ? 32 : lpDataHeader->ulUseableSize );
                  strcpy_s( szMsg, zsizeof( szMsg ), szHex );
                  strcat_s( szMsg, zsizeof( szMsg ), " | " );
                  strcat_s( szMsg, zsizeof( szMsg ), szAscii );
                  cpcText = " Used Space";
               }
               else
               {
                  szMsg[ 0 ] = 0;
                  if ( lpFreespace->nTableID == iFreespace )
                     cpcText = "*Free Space";
                  else
                     cpcText = "????? Space";
               }

#if 1
               if ( lpFreespace->nTableID == iUsedFreespace )
               {
                  TraceLine( "        %s: 0x%08x  Lth: %8d  Next: 0x%08x  %s",
                             cpcText, lpFreespace, lpFreespace->ulFreeLth,
                             ((zPCHAR) lpFreespace) + lpFreespace->ulFreeLth, szMsg );
                  lpFreespace->nTableID = iUsedFreespace1; // mark it as previously traced
               }
#endif

               lpFreespace = (LPFREESPACE) ((zPCHAR) lpFreespace + lpFreespace->ulFreeLth);
            }
#endif
#endif
            lpDataHeader = zGETPTR( lpDataHeader->hNextDataHeader );
         }

         lpTask = zGETPTR( lpTask->hNextTask );
      }

      zUNLOCK_MUTEX( zMUTEX_MEMORY );

      lpApp = zGETPTR( AnchorBlock->hFirstApp );
      while ( lpApp )
      {
         TraceLine( "   App: 0x%08x (%s)   Application Task: 0x%08x   %s",
                    lpApp, lpApp->szName, lpApp->hApplicationTask, lpApp->bKeepAlive ? "Keep Alive" : "" );

         lpApp = zGETPTR( lpApp->hNextApp );
      }
   }

// MiListOE_Memory( 0, 0 );

#endif

   return( 0 );
}

void OPERATION
fnListDataspaceLeak( LPANCHOR lpAnchorBlock, zPVOID hTask, zSHORT nFlag );

//./ ADD NAME=DeleteTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteTask
//
//  PURPOSE:    To delete an application task from the object services
//              task list.
//
//  PARAMETERS: lpTask - pointer to the object services task to delete
//
//  RETURNS:    0           - Task deleted
//              zCALL_ERROR - Task pointer passed is null
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
DeleteTask( LPTASK lpTask )
{
// LPSUBTASK  hFirstSubtask;
   LPTASK     hTask;
   LPTASK     lpPrevTask;
   LPTASK     lpMainTask;
   LPVIEWNAME lpViewName;
   LPAPP      lpNextApp;
   zCHAR      szMsg[ 4000 ];
   LPAPP      lpApp;
   LPVIEWOI   lpViewOI;
// LPOPENFILE lpOpenFileNext;
   zVIEW      lpView;
   zVIEW      vDbhWork;
   zBOOL      bCurrentTask;
   zLONG      lProcessID = SysGetProcessID( 0 );

   // Make sure task is not null and is valid.
   if ( lpTask == 0 || lpTask->nTableID != iTask )
      return( zCALL_ERROR );

   // Check to see if the task is being shutdown already by someone else.
   // If so then we'll get out and let the other task do it.
   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( lpTask->bShutdown )
   {
      // It's already being shut down so skip it.
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      return( 0 );
   }

   // Mark task as being shutdown.
   lpTask->bShutdown = TRUE;

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   hTask = zGETHNDL( lpTask );
   lpMainTask = zGETPTR( AnchorBlock->hMainTask );

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
   {
      TraceLine( "DeleteTask ==================> 0x%08x   ProcessID: %d   Task: 0x%08x          FirstOpenFile: 0x%08x",
                 hTask, lProcessID, lpTask, lpTask->hFirstOpenFile );
   }

   // Get Current Application Task.
   lpView = zGETPTR( lpTask->hFirstView );
   GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME, lpView, zLEVEL_SYSTEM );

   // Free the task's counting buffer.
   fnFreeDataspace( zGETPTR( lpTask->hCountBuf ) );
   lpTask->hCountBuf = 0;

   // Before releasing database handlers, call the drop constraint
   // for all viewoi's for the task which have a drop constraint.
// if ( lpTask == lpCurrentTask )  // gots to be in 10c+
   {
      zSHORT nDrops;
      zSHORT nPasses;

      bCurrentTask = TRUE;
      for ( nPasses = 3; nPasses; nPasses-- )
      {
         lpViewOI = zGETPTR( lpTask->hFirstViewOI );
         nDrops = 0;
         while ( lpViewOI )
         {
            LPVIEWOD lpViewOD = zGETPTR( lpViewOI->hViewOD );

            if ( lpViewOD && lpViewOD->bDropOIConstraint &&
                 lpViewOI->bDropConstraintInvoked == FALSE )
            {
               LPVIEWCSR lpFirstViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );

               fnInvokeOCEOperation( zGETPTR( lpFirstViewCsr->hView ),
                                     lpTask,
                                     zOCE_DROPOI, zOCE_STATE_SHUTDOWN );
               lpViewOI->bDropConstraintInvoked = TRUE;
               nDrops++;
            }

            lpViewOI = zGETPTR( lpViewOI->hNextViewOI );
         }

         // The drop constraint may have activated an object which in turn has
         // a drop constraint ... if so, try again until all passes complete
         // or no drops are issued.
         if ( nDrops == 0 )
            break;
      }
   }
// else
// {
//    bCurrentTask = FALSE;
// }

   // If the task being deleted is the main task, we first shut
   // the DBHandlers, before we drop the views.
   // This is for engine server under MVS. Here, the only existing
   // task is the main task. The DBHandler work object must not be
   // dropped prior to the DBHandler shutdown.
   // DG reversed the order of dropping views and DBH shutdown because
   // of Lock Views. Those should not exist for the main task.

   if ( lpTask == lpMainTask )
      fnDropTaskHandlers( lpTask );

   // Drop all view names tied to the task.
   while ( lpTask->hFirstViewName )
   {
      lpViewName = zGETPTR( lpTask->hFirstViewName );

      // Unchain the current view name.
      lpTask->hFirstViewName = lpViewName->hNextViewName;

      // Decrement the name count in the View referenced by the ViewName.
      lpView = zGETPTR( lpViewName->hView );
      lpView->nViewNames--;

      // Free up storage for name string and the ViewName itself.
      fnFreeDataspace( lpViewName );
   }

#if 0  // this is done by fnDropTaskHandlers!
   // I think this was a problem in previous versions of Zeidon, but we never
   // saw it because we never had the same "task id".  Now we get the same
   // task quite often (after delete), so we must remove the references in
   // the system work object that apply to this task id.  dks 2004.05.27
   // Do this after dropping task handlers.
   if ( MiValidView( vDbhWork ) )
   {
      zVIEW vWork;

   // SfCloseTaskDatabases( lpView );
   // zltoa( (zLONG) hTask, szMsg, zsizeof( szMsg ) );
      sprintf_s( szMsg, zsizeof( szMsg ), "0x%08x", hTask );

      CreateViewFromViewForTask( &vWork, vDbhWork, lpView );
      while ( SetCursorFirstEntityByString( vWork, "Connection",
                                            "LogicalUserName",
                                            szMsg, zSCOPE_OI ) == zCURSOR_SET )
      {
      // LPCONNECTION lpConnection;

      // GetAddrForAttribute( (zPVOID) &lpConnection, vWork,
      //                      "Connection", "ConnectionBlob" );

      // Free connection memory (last parm zero).
      // fnAllocateConnectionMemory( vWork, lpConnection, TRUE, 0 );
      // fnAllocateConnectionMemory( vWork, lpConnection, FALSE, 0 );

         SetAttributeFromBlob( vWork, "Connection", "ConnectionBlob", 0, 0 );
         DeleteEntity( vWork, "Connection", zREPOS_PREV );
      }

      DropView( vWork );
   }
#endif

   // If object services coming down, skip freeing views and applications.
   // DGC 11/8/96
   // We should no longer care if the task is the main task.
   // Drop all the views chained to the task, any view names will
   // fall with the chips ...
   // Hey ... I'm with DGC 11/8/96 ... skip all this if we are the main
   // task because there ain't nothin nobody can do to stop us.
// if ( lpTask != lpMainTask ) // if main task, forget it ... can't forget it when OE on server/tomcat
   {
      zVIEW lpNextView;

      lpView = zGETPTR( lpTask->hFirstView );
      lpTask->hFirstView = 0;
      while ( lpView )
      {
         lpNextView = zGETPTR( lpView->hNextView );
         fnDropView( lpView );  // dks 2006.09.18 error while cleaning up client OE
         lpView = lpNextView;
      }

      if ( lpTask->hFirstView )
         SysMessageBox( 0, "DKS", "First View not NULL!!!", -1 );

#if 1 // this is hooey!!! but necessary for fnDropTaskHandlers
      // Dummy view just to pass the task in case we need it.   dks
      fnDeclareView( &lpView, lpTask, (zVIEW) -1, 0, FALSE );
      lpTask->hFirstView = zGETHNDL( lpView );
#endif // this is hooey!!!

      // If this task has any instances remaining, then some views are
      // outstanding to the instance in another task, drop those views
      // so the instance goes away!!
      while ( lpTask->hFirstViewOI )
      {
         LPVIEWOI  lpFirstViewOI;
         LPVIEWCSR lpFirstViewCsr;

         lpFirstViewOI  = zGETPTR( lpTask->hFirstViewOI );
         lpFirstViewCsr = zGETPTR( lpFirstViewOI->hFirstViewCsr );
         fnDropView( zGETPTR( lpFirstViewCsr->hView ) );
      }

      // Free all TaskDBHandlers, if not for the main task.
      if ( lpTask != lpMainTask )
         fnDropTaskHandlers( lpTask );

      fnDropView( lpView );
   }

// DGC 6/21/2006
// We might need to add code to tell the network handler that a task is
// shutting down.  It's not needed yet because the winsock handler doesn't
// do anything for a task.
#if 0
   // Close any networks.  Since we now need to supply a view to the NetClose
   // operation, we create a dummy view (all other views have been dropped).
   if ( lpTask->hFirstNetwork )
   {
      LPNETWORK lpNetwork = zGETPTR( lpTask->hFirstNetwork );
      zVIEW vTask;

      vTask = zGETPTR( lpTask->hFirstView );

      while ( lpTask->hFirstNetwork )
      {
         lpNetwork = zGETPTR( lpTask->hFirstNetwork );

         NetClose( vTask, lpNetwork->szNetworkName );
         if ( lpNetwork == zGETPTR( lpTask->hFirstNetwork ) )
         {
            fnSysMessageBox( lpTask, "Zeidon Network Error",
                             "Couldn't close network--assume system is unstable",
                             1 );
            break;
         }
      }
   }
#endif

   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   // Let's make sure that no one else will browse the task list while we're
   // removing a task.  First set flag to tell all processes that we're
   // about to delete a task.
   AnchorBlock->bDeleteTaskInProgress = TRUE;

   // Free the task's application allocated memory.  We have to
   // do this after dropping the DB Handler, because when closing
   // the DB Handler, it accesses its allocated memory!!!   97.07.10 GT
   if ( lpTask->hFirstAppMem )
   {
   // zPVOID hFirstAppMem = lpTask->hFirstAppMem;

      if ( AnchorBlock->TraceFlags.bTaskMsgs )
      {
         TraceLineI( "(tm) Freeing application allocated memory for task ",
                     (zLONG) zGETHNDL( lpTask ) );
      }

      SysCheckTaskMemory( );  // list unfreed memory

      // Free any memory not taken care of by the application.
      while ( lpTask->hFirstAppMem )
      {
         SysFreeMemory( (zLONG) lpTask->hFirstAppMem );
      // if ( hFirstAppMem == lpTask->hFirstAppMem )
      // {
      //    TraceLineS( "(tm) DeleteTask error removing app memory", "" );
      //    break;  // some kind of problem has occurred ... punt!
      // }
      //
      // hFirstAppMem = lpTask->hFirstAppMem;
      }
   }

   while ( lpTask->hFirstSubtask )
   {
   // hFirstSubtask = lpTask->hFirstSubtask;
      fnDropSubtask( lpTask, zGETPTR( lpTask->hFirstSubtask ), 1 );
   // if ( hFirstSubtask == lpTask->hFirstSubtask )
   // {
   //    TraceLineS( "(tm) DeleteTask error removing subtask", "" );
   //    break;  // some kind of problem has occurred ... punt!
   // }
   }

   // Close all Open files.
   fnCloseTaskOpenFiles( lpTask );

   // Free TaskDomains for this Task.
   fnDropTaskDomains( lpTask );

   // Drop all task mutexes.
// fnDropTaskMutexes( lpTask );

   // Null the application pointer for the task and issue a delete for
   // all applications currently active.
   lpTask->hApp = 0;
   lpApp = zGETPTR( AnchorBlock->hFirstApp );
   while ( lpApp )
   {
      lpNextApp = zGETPTR( lpApp->hNextApp );
      fnDeleteApp( lpApp );   // delete all apps with no tasks
      lpApp = lpNextApp;
   }

   // Free TaskOperations for this Task.
   fnDropTaskOperations( lpTask );

   // Free TaskApps for this Task.
   while ( lpTask->hFirstTaskApp )
   {
      if ( fnDropTaskApp( lpTask, zGETPTR( lpTask->hFirstTaskApp ) ) < 0 )
      {
         TraceLineS( "(tm) DeleteTask error dropping task app", "" );
         break;  // some kind of problem has occurred ... punt!
      }
   }

   // Free the task dataspace now that all others have freed it.
   // First issue a SysFreeLibrary for all loaded modules for this task.
// fnFreeTaskModules( lpTask );
// if ( lpTask == lpCurrentTask )  gots to be in 10c+
   {
      while ( fnSysFreeLibrary( lpTask, 0, TRUE ) > 0 )
      {
      }
   }

   // Drop all task mutexes.  try it here ... dks 2006.06.28
   fnDropTaskMutexes( lpTask );

// fnDropView( lpView );  memory has already been freed
   lpTask->hFirstView = 0;
   SetDefaultViewForActiveTask( 0, 1 );

   fnCleanupTask( lpTask );
// SysMalloc( -1 );  // report memory usage

#if 0
   // Since we do not create an application task share record
   // for sharing with the main object engine task. Go to
   // the main object engine task and tell it to free this
   // task's dataspaces.
   // Have this task free all shared memory "given" to it by other
   // tasks that own the shared memory.
   if ( lpTask != lpMainTask && g_bServerMode == FALSE )
   {
      ProcessZeidonMessage( (zLONG) hTask, -1 );
   }

   // We've deleted a task.  Memory management was smart enough to not free
   // each memory fragment to save time.  We now need to free each of the
   // pages since it wasn't done by memory management.  In g_bServerMode
   // we use malloc/free so this isn't necessary.
   if ( g_bServerMode == FALSE )
   {
      LPDATAHEADER lpNextDataHeader;
      LPDATAHEADER lpDataHeader;

      for ( lpDataHeader = zGETPTR( lpTask->hFirstDataHeader );
            lpDataHeader;
            lpDataHeader = lpNextDataHeader )
      {
      // TraceLineS( "Freeing a page", "" );
         lpNextDataHeader = zGETPTR( lpDataHeader->hNextDataHeader );
         fnFreeSharedMemory( lpTask, lpDataHeader->hDataHandle );
      }
   }
#endif

   // Remove the task from the task chain.
   fnStartBrowseOfTaskList( hTask, TRUE ); // Get a 'write' lock on the task list.
   if ( AnchorBlock->hFirstTask == hTask )
      AnchorBlock->hFirstTask = lpTask->hNextTask;
   else
   {
      lpPrevTask = zGETPTR( AnchorBlock->hFirstTask );
      while ( lpPrevTask && lpPrevTask->hNextTask != hTask )
         lpPrevTask = zGETPTR( lpPrevTask->hNextTask );

      if ( lpPrevTask )
         lpPrevTask->hNextTask = lpTask->hNextTask;
      else
         lpPrevTask = 0;
   }

#ifdef ALLOC_FREE_TRACE
   if ( AnchorBlock->bTraceMem )
      fnListDataspaceLeak( AnchorBlock, hTask, 0 );
#endif

   // Free the 'write' lock on the task list.
   fnEndBrowseOfTaskList( TRUE );

   // That was quick ... now indicate we are no longer deleting a task.
   AnchorBlock->bDeleteTaskInProgress = FALSE;

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   // Send message that task deleted (before we delete the task).
   if ( AnchorBlock->TraceFlags.bTaskMsgs )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), "(tm) Task (0x%08x) deleted for Process %d",
                (zULONG) hTask, SysGetProcessID( 0 ) );

      if ( lpTask && lpTask->szUserId[ 0 ] )
      {
         strcat_s( szMsg, zsizeof( szMsg ), " UserName: " );
         strcat_s( szMsg, zsizeof( szMsg ), lpTask->szUserId );
      }
   }

   // Free the stuff that is part of the system task's dataspace.
   fnFreeDataspace( zGETPTR( lpTask->hUserID ) );
   fnFreeDataspace( zGETPTR( lpTask->hPassword ) );
   fnFreeDataspace( zGETPTR( lpTask->hDBUserID ) );
   fnFreeDataspace( zGETPTR( lpTask->hDBPassword ) );
   fnFreeDataspace( zGETPTR( lpTask->hCommand ) );
   fnFreeTaskDataspace( lpTask );
   fnFreeDataspace( lpTask );


   if ( AnchorBlock->TraceFlags.bTaskMsgs )
   {
      // Use SysMessageList because TraceLineS expects a current task.
      SysMessageList( szMsg );
   // MiListOE_Memory( 0, 0 );
      SysDescribeZeidonPageTable( szMsg, zsizeof( szMsg ) );
      TraceLineS( "Post DeleteTask - ", szMsg );
   }

   // Free the shared segment if we deleted our own task and we are not
   // the main task.
// if ( bCurrentTask && lpTask != lpMainTask )
//    fnFreeLocalAnchorBlock( FALSE );

   return( 0 );

} // end of:  DeleteTask

//./ ADD NAME=SfGetNextTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetNextTask
//
//  PURPOSE:    To retrieve the next task after lpTask.  If lpTask
//              is 0, then returns the first task.
//
//  PARAMETERS: lpTask - The "current" task.
//
//  RETURNS:    0           - No more tasks.
//              ptr         - First/next task.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPTASK OPERATION
SfGetNextTask( LPTASK lpTask )
{
   if ( AnchorBlock == 0 )
      return( 0 );

   // Make sure nobody else is deleting a task.
   fnStartBrowseOfTaskList( zGETHNDL( lpTask ), FALSE );

   if ( lpTask == 0 )
      lpTask = zGETPTR( AnchorBlock->hFirstTask );
   else
   {
      LPTASK lpSearchTask;

      // The user passed in a task pointer.  Make sure that it is a valid
      // task pointer.  If it is then return the next task; otherwise return 0.

      for ( lpSearchTask = zGETPTR( AnchorBlock->hFirstTask );
            lpSearchTask && lpSearchTask != lpTask;
            lpSearchTask = zGETPTR( lpSearchTask->hNextTask ) )
      {
         // Nothing needs to be done here.
      }

      if ( lpSearchTask )
         lpTask = zGETPTR( lpSearchTask->hNextTask );
      else
         lpTask = 0;
   }

   // Now make sure task is in good condition.
   while ( lpTask && (lpTask->bDisable || lpTask->bShutdown) )
      lpTask = zGETPTR( lpTask->hNextTask );

   fnEndBrowseOfTaskList( FALSE );

   return( lpTask );

} // SfGetNextTask

//./ ADD NAME=SfGetCurrentTask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetCurrentTask
//
//  PURPOSE:    To retrieve the task for the view.
//
//  PARAMETERS:
//
//  RETURNS:    0           - No task.
//              ptr         - Current task.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPTASK OPERATION
SfGetCurrentTask( zVIEW vTaskView )
{
   if ( AnchorBlock == 0 )
      return( 0 );

   return( zGETPTR( vTaskView->hTask ) );

} // SfGetCurrentTask

//./ ADD NAME=SfGetTaskInfo
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetTaskInfo
//
//  PURPOSE:    To retrieve some task related information
//
//  PARAMETERS: pchReturnString - pointer to string where info is returned.
//              lMaxLth        - maximimum length of return string.
//              nInfoRequest   - indicates what info is to be returned.
//                            zTASK_DIALOG  - returns name of the current
//                                            dialog.  May be null.
//                            zTASK_COMMAND - returns command line info
//                                            of the current task.  May
//                                            be null.
//              lpTask         - Task pointer.  User can use SfGetCurrentTask
//                               to obtain this value from a view tied to the
//                               current task (ViewToWindow should be great).
//
//  RETURNS:    0           - Information has been returned
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfGetTaskInfo( zPCHAR pchReturnString, zLONG lMaxLth, zSHORT nInfoRequest, LPTASK lpTask )
{
   int    nRC;

   switch ( nInfoRequest )
   {
      case zTASK_DIALOG:
         if ( lpTask->szDialog[ 0 ] )
         {
            strcpy_s( pchReturnString, lMaxLth, lpTask->szDialog );
            SysTranslateString( pchReturnString, 'U' ); // guarantee upper-case
         }
         else
            pchReturnString[ 0 ] = 0;

         break;

      case zTASK_COMMAND:
         if ( lpTask->hCommand )
            strcpy_s( pchReturnString, lMaxLth, zGETPTR( lpTask->hCommand ) );
         else
            pchReturnString[ 0 ] = 0;

         break;

      default:
         // "KZOEE019 - Invalid GetTaskInfo request: "
         fnIssueCoreError( lpTask, 0, 8, 19, (zLONG) nInfoRequest, 0, 0 );
         nRC = zCALL_ERROR;
   }

   nRC = 0;
   return( nRC );
}

//./ ADD NAME=SfCreateSubtask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfCreateSubtask
//
//  PURPOSE:    To create and application subtask under a System task.
//
//  PARAMETERS: pvReturnSubtask - returned view to new subtask
//              lpView          - Task view.  If cpcAppName is NULL, then the
//                                new subtask uses the app name from lpView.
//              cpcAppName      - An optional application name
//
//
//  DESCRIPTION:  The subtask is created for an application and a view
//                to the subtask is returned which has no object type
//                associated with it. If an application name is passed,
//                then the new subtask is created for the application name
//                passed.  If the application name is not passed
//                (i.e. NULL), then the new subtask is created for the
//                application associated with the View passed. If BOTH the
//                application name and view passed are NULL, then the
//                application associated with the new subtask is the
//                application initially defined in the
//                RegisterZeidonApplication call.
//
//  RETURNS:    0           - subtask created successfully
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SfCreateSubtask( zPVIEW  pvReturnSubtask,
                 zVIEW   lpView,
                 zCPCHAR cpcAppName )
{
   LPTASK      lpCurrentTask;
   zVIEW       lpNewSubtaskView;
   LPAPP       lpApp;
   int         nRC = zCALL_ERROR;

#ifdef DEBUG
   if ( lpView == 0 )
   {
      TraceLine( "SfCreateSubtask now REQUIRES a non-zero qualification view for Application: %s.", cpcAppName );
      SysMessageBox( 0, "Zeidon OE", "SfCreateSubtask now REQUIRES a non-zero qualification view.  System will now crash", 0 );
      nRC = 0;
      nRC /= nRC;
   }
#endif

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfCreateSubtask, lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // We need to lock the anchor mutex because we might create a new app.
   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( cpcAppName && cpcAppName[ 0 ] )
   {
      lpApp = fnCreateApp( lpCurrentTask, cpcAppName );
      if ( lpApp == 0 )
      {
         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
         fnOperationReturn( iSfCreateSubtask, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }
   else
      fnGetApplicationForSubtask( &lpApp, lpView );

   // Use the current task in the call to fnCreateSubtask
   lpNewSubtaskView = fnCreateSubtask( lpCurrentTask, lpApp );

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( lpNewSubtaskView )
   {
      LPSUBTASK lpSubtask = zGETPTR( lpNewSubtaskView->hSubtask );
      LPAPP     lpNewApp  = zGETPTR( lpSubtask->hApp );

      *pvReturnSubtask = lpNewSubtaskView;

      // When possibly creating a new App struct, we want to preload the
      // Message Object Definition.  The assumption that we are making
      // below is that the first object definition that is put on the
      // app struct will be the message object definition, so if there
      // is already a pointer in the app struct, to a ViewOD chain, then
      // the message object must already be preloaded. (We Hope!!!)
      nRC = 0;
      if ( lpNewApp->hFirstViewOD == 0 )
      {
         if ( ActivateViewObject( lpNewSubtaskView, szlMessageObject, 0 ) == 0 )
            nRC = zCALL_ERROR;
      }
   }
   else
      nRC = zCALL_ERROR;

   fnOperationReturn( iSfCreateSubtask, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfCreateSystemSubtask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfCreateSystemSubtask
//
//  PURPOSE:    To create an application subtask under a System task.
//
//  PARAMETERS: pvReturnSubtask - returned view to new subtask
//              lpView          - a valid View
//              cpcAppName      - an optional application name
//
//
//  DESCRIPTION:  The subtask is created for an application and a view
//                to the subtask is returned which has no object type
//                associated with it. If an application name is passed,
//                then the new subtask is created for the application name
//                passed.  If the application name is not passed
//                (i.e. NULL), then the new subtask is created for the
//                application associated with the View passed. If BOTH the
//                application name and view passed are NULL, then the
//                application associated with the new subtask is the
//                application initially defined in the
//                RegisterZeidonApplication call.
//
//  RETURNS:    0           - subtask created successfully
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SfCreateSystemSubtask( zPVIEW  pvReturnSubtask,
                       zVIEW   lpView,
                       zCPCHAR cpcAppName )
{
   LPTASK      lpCurrentTask;
   zVIEW       lpNewSubtaskView;
   LPAPP       lpApp;
   int         nRC;

#ifdef DEBUG
   if ( lpView == 0 )
   {
      TraceLine( "SfCreateSystemSubtask now REQUIRES a non-zero qualification view"
                   "for Application: %s.", cpcAppName );
      SysMessageBox( 0, "Zeidon OE", "SfCreateSystemSubtask now REQUIRES a "
                     "non-zero qualification view.  System will now crash", 0 );
      nRC = 0;
      nRC /= nRC;
   }
#endif

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfCreateSubtask, lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // We need to lock the anchor mutex because we might create a new app.
   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( cpcAppName && cpcAppName[ 0 ] )
   {
      lpApp = fnCreateApp( lpCurrentTask, cpcAppName );
      if ( lpApp == 0 )
      {
         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
         fnOperationReturn( iSfCreateSubtask, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }
   else
      fnGetApplicationForSubtask( &lpApp, lpView );

   // Use the system task in the call to fnCreateSubtask
   lpNewSubtaskView = fnCreateSubtask( zGETPTR( AnchorBlock->hMainTask ), lpApp );

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( lpNewSubtaskView )
   {
      *pvReturnSubtask = lpNewSubtaskView;

      // When possibly creating a new App struct, we want to preload the
      // Message Object Definition.
      ActivateViewObject( lpNewSubtaskView, szlMessageObject, 1 );
      nRC = 0;
   }
   else
      nRC = zCALL_ERROR;

   fnOperationReturn( iSfCreateSubtask, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnCreateSubtask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnCreateSubtask
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zVIEW
fnCreateSubtask( LPTASK lpTask, LPAPP lpApp )
{
   LPSUBTASK lpSubtask;
   zVIEW     lpView;
   zPVOID    hView;
   zPVOID    hSubtask;

   // Return if application task is invalid.
   if ( lpTask == 0 || lpTask->nTableID != iTask )
      return( 0 );

   // Create an application subtask.
   // Allocate an application task record.
   hView = fnAllocDataspace( (LPDATAHEADER) g_hAnchorBlock, sizeof( ViewRecord ), 1, 0, iView );
   lpView = zGETPTR( hView );
   if ( lpView )
   {
      hSubtask = fnAllocDataspace( (LPDATAHEADER) g_hAnchorBlock, sizeof( SubtaskRecord ), 1, 0, iSubtask );
      lpSubtask = zGETPTR( hSubtask );

      // If Allocate was successful, chain to task.
      if ( lpSubtask )
      {
         lpView->hSubtask = hSubtask;
         lpView->bViewLocked = TRUE; // automatically lock subtask views
         lpView->hTask = zGETHNDL( lpTask );
         lpView->hNextView = lpTask->hFirstView;

         lpSubtask->hNextSubtask = lpTask->hFirstSubtask;
         lpSubtask->hTask = lpView->hTask; // = zGETHNDL( lpTask );
         lpSubtask->hApp = zGETHNDL( lpApp );
         lpSubtask->hSubtaskView = hView;

         lpTask->hFirstSubtask = hSubtask;
         lpTask->hFirstView    = hView;
      // TraceLine( "CreateSubtask from task: 0x%08x   app: 0x%08x %s", lpView->hTask, lpSubtask->hApp, lpApp ? lpApp->szName : "" );
      }
      else
      {
         fnFreeDataspace( lpView );
         lpView = 0;
      }
   }

   return( lpView );
}

//./ ADD NAME=SfDropSubtask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfDropSubtask
//
//  PURPOSE:    To delete an existing subtask
//
//  PARAMETERS: vSubtask   - the subtask to delete
//              nCleanup   - 0 Bypass cleanup of Views
//                           1 For the Task of the subtask, drop all
//                             views that are NOT NAMED or NOT LOCKED.
//                           2 For the Task of the subtask, drop all
//                             views that are NOT NAMED or NOT LOCKED AND
//                             are also part of the same application.
//
//  RETURNS:    0           - Subtask delete successful
//              zCALL_ERROR - subtask is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfDropSubtask( zVIEW vSubtask, zSHORT nCleanup )
{
   LPTASK    lpCurrentTask;
   LPSUBTASK lpSubtask;
   int       nRC;

   // If task not active or disabled, or subtask view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfDropSubtask, vSubtask, zVALID_SUBTASK_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpSubtask = zGETPTR( vSubtask->hSubtask );
   nRC = fnDropSubtask( zGETPTR( lpSubtask->hTask ), lpSubtask, nCleanup );

   fnOperationReturn( iSfDropSubtask, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GarbageCollectViews
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GarbageCollectViews
//
//  PURPOSE:    To cleanup views.  For the specified task, we drop all
//              views that are NOT NAMED or are NOT LOCKED.
//
//  PARAMETERS: lpTaskView - Task for which to clean up views.
//              lpDropApp -1 - drop all non-named non-locked views
//                         0 - use lpTask to determine application
//                       otw - drop non-named non-locked views for this app
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
void
fnGarbageCollectViews( LPTASK lpTask, LPAPP lpDropApp )
{
   zVIEW  lpView;
   zVIEW  lpNextView = 0;

   if ( lpDropApp == 0 )
      lpDropApp = zGETPTR( lpTask->hApp );

   // Go through views for the task and drop views where nViewName == 0,
   // unless the View is marked as locked (bViewLocked == TRUE).
   for ( lpView = zGETPTR( lpTask->hFirstView );
         lpView;
         lpView = lpNextView )
   {
      // Get the next view.  We do it here because we might drop the current
      // lpView in which case lpView->hNextView is undefined.
      lpNextView = zGETPTR( lpView->hNextView );

      // Don't drop locked views.
      if ( lpView->bViewLocked )
         continue;

      // If number of view names for the view is greater than the number of
      // names allowed for auto dropped views (usually 0) then don't drop it.
      if ( lpView->nViewNames > lpView->nAutoDropNames )
         continue;

      if ( lpDropApp && lpDropApp != (LPAPP) -1 )
      {
         LPAPP lpViewApp;

         fnGetApplicationForSubtask( &lpViewApp, lpView );
         if ( lpViewApp != lpDropApp )
            continue;
      }

      // If we get here then all conditions indicate we can drop the view.
      fnDropView( lpView );
   }
}

zVOID  OPERATION
GarbageCollectViews( zVIEW  lpTaskView,
                     zLONG  lpDropApp )
{
   LPTASK hTask;
   LPTASK lpTask;
   LPTASK lpTaskCurr;
   zCHAR  szDateTime[ 20 ];
   zLONG  lDiffMinutes;

   lpTask = zGETPTR( lpTaskView->hTask );
   fnGarbageCollectViews( lpTask, (LPAPP) lpDropApp );

   fnStartBrowseOfTaskList( lpTaskView->hTask, FALSE );
   hTask = AnchorBlock->hFirstTask;
   while ( hTask )
   {
      lpTask = zGETPTR( hTask );
      if ( lpTask->bTransientTask && hTask != lpTaskView->hTask )
      {
         SysGetDateTime( szDateTime, zsizeof( szDateTime ) );
         SysGetDateTimeDifference( &lDiffMinutes,
                                   lpTask->szTimeStamp,
                                   szDateTime, zDT_MINUTE );
         if ( lDiffMinutes > 45 )
         {
            fnEndBrowseOfTaskList( FALSE );
            TraceLineX( "GarbageCollectViews Unregistering Task: ", (zLONG) hTask );
            lpTaskCurr = zGETPTR( lpTaskView->hTask );
            UnregisterZeidonApplication( zGETPTR( lpTask->hFirstView ) );

            // only remove one at a time so as to mimimize the impact on performance.
            // This also allows us to remove the lock on the task chain before
            // calling Unregister.  Unregister will lock it again.
            return;
         }
      }

      hTask = lpTask->hNextTask;
   }

   fnEndBrowseOfTaskList( FALSE );
}

//./ ADD NAME=fnDropSubtask
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnDropSubtask
//
//  PURPOSE:    To delete an existing subtask
//
//  PARAMETERS: lpSubtask - the subtask to be dropped
//              Note: The subtask view is also dropped with the subtask
//                    whether or not it points to a view object and
//                    instance data
//
//     NOTE: New parameter, nCleanup, used in view cleanup. When set to
//           a non-zero value, processing occurs as we did before this
//           parameter addition, that is, for the task associated with
//           the subtask, we drop ALL views that are NOT NAMED or are
//           NOT LOCKED.  Specifying a zero value will bypass this
//           'garbage' collection.
//
//  RETURNS:    0           - Subtask delete successful
//              zCALL_ERROR - subtask is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnDropSubtask( LPTASK lpTask, LPSUBTASK lpSubtask, zSHORT nCleanup )
{
   LPAPP       lpApp;
   LPAPP       lpDropApp;
   LPVIEWNAME  lpViewName;
   zVIEW       vSubtask;
   LPSUBTASK   hSubtask = zGETHNDL( lpSubtask );

   // Get application for subtask
   lpApp = zGETPTR( lpSubtask->hApp );
   if ( nCleanup == 2 )
      lpDropApp = lpApp;  // drop non-named non-locked views for this subtask
   else
      lpDropApp = (LPAPP) -1;  // drop all non-named non-locked views

   // NULL out the subtask pointer in the subtask view so fnDropView
   // will remove the view from the task.  Also unlock the view...
   vSubtask = zGETPTR( lpSubtask->hSubtaskView );
   vSubtask->hSubtask = 0;
   vSubtask->bViewLocked = 0;
   fnDropView( vSubtask );

   // Drop all the view names chained to the subtask.
   while ( lpSubtask->hFirstViewName )
   {
      zVIEW lpView;

      lpViewName = zGETPTR( lpSubtask->hFirstViewName );

      // Unchain the current view name.
      lpSubtask->hFirstViewName = lpViewName->hNextViewName;

      // Decrement the name count in the View referenced by the ViewName.
      lpView = zGETPTR( lpViewName->hView );
      lpView->nViewNames--;

      // Free up storage for name string and the ViewName itself.
      fnFreeDataspace( lpViewName );
   }

   // We're about to unchain a few things (views, subtasks, etc.).  We have
   // to put this in a mutex so that other tasks won't try to delete an
   // application while we are unchaining.
   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

//
// NOTE: New parameter, nCleanup, used in view cleanup. When set to a
//       non-zero value, processing occurs as we did before this parameter
//       addition, that is, for the task associated with the subtask, we
//       drop ALL views that are NOT NAMED or are NOT LOCKED.  Specifying
//       a zero value will bypass this 'garbage' collection.
//       Note: nCleanup == 1 means drop all non-named non-locked views,
//       and nCleanup == 2 means drop only non-named non-locked views
//       associated with the subtask being dropped.
//
   if ( nCleanup > 0 )
      fnGarbageCollectViews( zGETPTR( lpSubtask->hTask ), lpDropApp );

   // Unchain the subtask from the task.
   if ( lpTask->hFirstSubtask == hSubtask )
   {
      lpTask->hFirstSubtask = lpSubtask->hNextSubtask;
   }
   else
   {
      LPSUBTASK lpSearchSubtask;

      lpSearchSubtask = zGETPTR( lpTask->hFirstSubtask );
      while ( lpSearchSubtask->hNextSubtask != hSubtask )
         lpSearchSubtask = zGETPTR( lpSearchSubtask->hNextSubtask );

      if ( lpSearchSubtask->hNextSubtask == hSubtask )
         lpSearchSubtask->hNextSubtask = lpSubtask->hNextSubtask;
   }

   lpSubtask = (LPSUBTASK) fnFreeDataspace( lpSubtask );

   // If there is an application for the subtask and the application does
   // not match the startup application for the task, issue a delete for
   // the application.
   if ( nCleanup >= 0 && lpApp && lpApp != zGETPTR( lpTask->hApp ) )
   {
      fnDeleteApp( lpApp );
   }

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   return( 0 );
}

//./ ADD NAME=SfCurrentTaskIsServer
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfCurrentTaskIsServer
//
//  PURPOSE:    Indicates whether the current task is currently handling
//              a client request.
//
//  PARAMETERS: None.
//
//  DESCRIPTION:  When the current node is set up to handle incoming requests
//                from clients a listen has been done on the network.  When
//                the current task is handling a request from a client this
//                operation returns TRUE.  If the current task is not
//                handling a client request, this operation returns FALSE.
//
//  RETURNS:    TRUE  - Current task is currently handling a client request.
//              FALSE - No client request.
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL OPERATION
SfCurrentTaskIsServer( zVIEW vTaskView )
{
   LPTASK lpTask = zGETPTR( vTaskView->hTask );

   if ( lpTask )
      return( lpTask->bServer );
   else
      return( (zBOOL) zCALL_ERROR );
}

//./ ADD NAME=fnStartBrowseOfTaskList
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnStartBrowseOfTaskList
//
//  PURPOSE:    To protect processes that are going through the list of
//              tasks.  This function prevents another process from deleting
//              a task.
//
//  PARAMETERS: None.
//
//  DESCRIPTION:  When the current process is searching through the list of
//                tasks it is possible that a task is being deleted by
//                another process.  This could cause the current process to
//                crash if it tries to access a deleted task pointer after it
//                has been deleted.  To keep this from happening we will set
//                up a semaphore using only code--we will avoid using a
//                system mutex for speed.  This code will allow any number
//                of processes to browse the task list unless one of the
//                processes is deleting a task and then no process is allowed.
//                This function will wait until it's OK to browse.
//
//  RETURNS:    0  - Everythink OK.
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT LOCALOPER
fnStartBrowseOfTaskList( LPTASK hTask, zBOOL bForWrite )
{
   if ( bForWrite )
   {
      while ( TRUE )
      {
         // See note below about locking the 'write' mutex to find out
         // why we use a global variable instead of a regular mutex.
         zLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
         if ( AnchorBlock->hTaskChainWriteLockTaskID == 0 )
         {
            AnchorBlock->hTaskChainWriteLockTaskID = (zLONG) hTask;
            zUNLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
            break;
         }

         zUNLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
         SysWait( 0 );
      }
   }
   else
   {
      // Lock the 'browse' mutex.
      zLOCK_MUTEX( zMUTEX_TASKCHAIN );

      // Bump up the counter that keeps track of all the tasks that are
      // reading the task list.
      AnchorBlock->nBrowseTaskListCounter++;

      // If the counter is 1 then this means no other tasks are currently
      // reading the task list and so we need to make sure no other task
      // has the 'write' mutex locked.
      if ( AnchorBlock->nBrowseTaskListCounter == 1 )
      {
         // NOTE
         // We need to lock the 'write' mutex to keep another task from
         // acquiring a 'write' lock while we are reading the task list.
         // Since the 'write' mutex can be locked by one task and then
         // unlocked by another task we will use a global variable to indicate
         // that the 'write' mutex is locked.  If lTaskChainWriteLockTaskID is
         // not 0 then the mutex is locked.
         while ( TRUE )
         {
            zLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
            if ( AnchorBlock->hTaskChainWriteLockTaskID == 0 )
            {
               AnchorBlock->hTaskChainWriteLockTaskID = (zLONG) hTask;
               zUNLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
               break;
            }

            zUNLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
            SysWait( 0 );
         }
      }

      zUNLOCK_MUTEX( zMUTEX_TASKCHAIN );
   }

   // If we get here everything's OK and the current task can now
   // browse the task list.
   return( 0 );
}

void LOCALOPER
fnEndBrowseOfTaskList( zBOOL bForWrite )
{
   if ( bForWrite )
   {
      // See NOTE in fnStartBrowseOfTaskList for an explanation of why
      // we use a global variable.
      zLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
      AnchorBlock->hTaskChainWriteLockTaskID = 0;
      zUNLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
   }
   else
   {
      // Lock the 'browse' mutex.
      zLOCK_MUTEX( zMUTEX_TASKCHAIN );

      // Decrement the counter that keeps track of all the tasks that are
      // reading the task list.
      AnchorBlock->nBrowseTaskListCounter--;

      // If the counter is 0 then this means no other tasks are currently
      // reading the task list and the 'write' lock can be freed.
      if ( AnchorBlock->nBrowseTaskListCounter == 0 )
      {
         // See NOTE in fnStartBrowseOfTaskList for an explanation of why
         // we use a global variable.
         zLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
         AnchorBlock->hTaskChainWriteLockTaskID = 0;
         zUNLOCK_MUTEX( zMUTEX_TASKCHAIN_WRITE );
      }

      zUNLOCK_MUTEX( zMUTEX_TASKCHAIN );
   }
}

//./ ADD NAME=fnStartBrowseOfProcessList
// Source Module=kzoetmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnStartBrowseOfProcessList
//
//  PURPOSE:    To protect processes that are going through the list of
//              tasks.  This function prevents another process from deleting
//              a task.
//
//  PARAMETERS: None.
//
//  DESCRIPTION:  When the current process is searching through the list of
//                tasks it is possible that a task is being deleted by
//                another process.  This could cause the current process to
//                crash if it tries to access a deleted task pointer after it
//                has been deleted.  To keep this from happening we will set
//                up a semaphore using only code--we will avoid using a
//                system mutex for speed.  This code will allow any number
//                of processes to browse the task list unless one of the
//                processes is deleting a task and then no process is allowed.
//                This function will wait until it's OK to browse.
//
//  RETURNS:    0  - Everythink OK.
//              zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT LOCALOPER
fnStartBrowseOfProcessList( LPTASK hTask, zBOOL bForWrite )
{
   if ( bForWrite )
   {
      while ( TRUE )
      {
         // See note below about locking the 'write' mutex to find out
         // why we use a global variable instead of a regular mutex.
         zLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
         if ( AnchorBlock->hTaskChainWriteLockTaskID == 0 )
         {
            AnchorBlock->hTaskChainWriteLockTaskID = (zLONG) hTask;
            zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
            break;
         }

         zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
         SysWait( 0 );
      }
   }
   else
   {
      // Lock the 'browse' mutex.
      zLOCK_MUTEX( zMUTEX_PROCESSCHAIN );

      // Bump up the counter that keeps track of all the tasks that are
      // reading the task list.
      AnchorBlock->nBrowseTaskListCounter++;

      // If the counter is 1 then this means no other tasks are currently
      // reading the task list and so we need to make sure no other task
      // has the 'write' mutex locked.
      if ( AnchorBlock->nBrowseTaskListCounter == 1 )
      {
         // NOTE
         // We need to lock the 'write' mutex to keep another task from
         // acquiring a 'write' lock while we are reading the task list.
         // Since the 'write' mutex can be locked by one task and then
         // unlocked by another task we will use a global variable to indicate
         // that the 'write' mutex is locked.  If lTaskChainWriteLockTaskID is
         // not 0 then the mutex is locked.
         while ( TRUE )
         {
            zLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
            if ( AnchorBlock->hTaskChainWriteLockTaskID == 0 )
            {
               AnchorBlock->hTaskChainWriteLockTaskID = (zLONG) hTask;
               zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
               break;
            }

            zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
            SysWait( 0 );
         }
      }

      zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN );
   }

   // If we get here everything's OK and the current task can now
   // browse the task list.
   return( 0 );
}

void LOCALOPER
fnEndBrowseOfProcessList( zBOOL bForWrite )
{
   if ( bForWrite )
   {
      // See NOTE in fnStartBrowseOfProcessList for an explanation of why
      // we use a global variable.
      zLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
      AnchorBlock->hTaskChainWriteLockTaskID = 0;
      zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
   }
   else
   {
      // Lock the 'browse' mutex.
      zLOCK_MUTEX( zMUTEX_PROCESSCHAIN );

      // Decrement the counter that keeps track of all the tasks that are
      // reading the task list.
      AnchorBlock->nBrowseTaskListCounter--;

      // If the counter is 0 then this means no other tasks are currently
      // reading the task list and the 'write' lock can be freed.
      if ( AnchorBlock->nBrowseTaskListCounter == 0 )
      {
         // See NOTE in fnStartBrowseOfProcessList for an explanation of why
         // we use a global variable.
         zLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
         AnchorBlock->hTaskChainWriteLockTaskID = 0;
         zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN_WRITE );
      }

      zUNLOCK_MUTEX( zMUTEX_PROCESSCHAIN );
   }
}

void OPERATION
DisplayOpenFiles( zCPCHAR cpcTitle, zPVOID hTask )
{
   LPAPP  lpApp;
   LPTASK lpTask;
   zLONG         lProcessID = SysGetProcessID( 0 );
   LPOPENFILE    lpOpenFile;
   LPVIEWOD      lpViewOD;
   zCHAR szMsg[ 256 ];

   TraceLineS( "Display Tasks/Apps ", cpcTitle );

   // Protected by fnStartBrowseOfTaskList.
   lpTask = zGETPTR( AnchorBlock->hFirstTask );
   while ( lpTask )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), " Task (0x%08x) %s Process: %d",
                (zULONG) zGETHNDL( lpTask ), lpTask->szTaskTitle, lProcessID );

//*********************************************************************************
      // KJS 04/20/09 - Added this so that we could also see who the user
      // is for this task (because of call sfSetUserIdForTask).
      if ( lpTask->szUserId[ 0 ] )
      {
         strcat_s( szMsg, zsizeof( szMsg ), " UserName: " );
         strcat_s( szMsg, zsizeof( szMsg ), lpTask->szUserId );
      }
//*********************************************************************************

      SysMessageList( szMsg );

      lpOpenFile = zGETPTR( lpTask->hFirstOpenFile );
      while ( lpOpenFile )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), " Open file: '%s'", lpOpenFile->szFileName );
         SysMessageList( szMsg );
         lpOpenFile = zGETPTR( lpOpenFile->hNextOpenFile );
      }

      lpTask = zGETPTR( lpTask->hNextTask );
   }

   lpApp = zGETPTR( AnchorBlock->hFirstApp );
   while ( lpApp )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), " App (0x%08x) %s %s",
                (zULONG) zGETHNDL( lpApp ), lpApp->szName, lpApp->bKeepAlive ? "(KeepAlive)" : "" );
      SysMessageList( szMsg );
      lpViewOD = zGETPTR( lpApp->hFirstViewOD );
      while ( lpViewOD )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), " View OD (0x%08x): '%s'",
                   (zULONG) zGETHNDL( lpViewOD), lpViewOD->szName );
         SysMessageList( szMsg );
         lpViewOD = zGETPTR( lpViewOD->hNextViewOD );
      }

      lpApp = zGETPTR( lpApp->hNextApp );
   }
}
