/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmslpd.c - Zeidon Switch LPLR dialog operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon workstation application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: CM00609
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2001.02.12 BL  R54697
   improve handling of return code from InitializeLPLR
   Replace LPLR to Project
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
From here old version
2000.10.24  SR  Z2000 Size of path variables
   Modified size of szLPLR_FileSpec, szDirectorySpec, szLPLR_FileName and
   szDetachSpec in function zwTZCMSLPD_RebuildMetaLists because size of attributes
   ExecDir, MetaSrcDir and PgmSrcDir has been changed to 254 in datamodel.
12.10.1997  DonC
   Modified SwitchLPLR to make sure no other tools are up.
18.02.1998  DonC
   Modified RebuildMetaLists to give prompt message to allow operator to
   abort before rebuild.
1999/04/07  DC
   Modified zwTZCMSLPD_RebuildMetaLists to use zstrcmpi instead of zstrcmp
   on compare of LPLR names. This ignores case differences.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_InitDialog( zVIEW vSubtask )
{
   zVIEW    vZeidonCM;
   zVIEW    vTZCMWKSO;
   zVIEW    vTZCMSLPL;
   int      nRC;

   if ( GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask,
                       zLEVEL_APPLICATION ) > 0 &&
        GetViewByName( &vTZCMWKSO, "TZCMWKSO", vZeidonCM,
                       zLEVEL_SUBTASK ) > 0 )
   {
      nRC = CreateViewFromViewForTask( &vTZCMSLPL, vTZCMWKSO, 0 );
      SetNameForView( vTZCMSLPL, "TZCMSLPL", vSubtask, zLEVEL_TASK );
   }
   else
   {
      MessageSend( vSubtask, "CM00601", "Configuration Management",
                   "Unable to activate RepositoryClient",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }
   nRC = SetSelectStateOfEntityForSet( vTZCMSLPL, "LPLR", 1, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_TerminateDialog( zVIEW vSubtask )
{
   zVIEW    vTZCMSLPL;

   if ( GetViewByName( &vTZCMSLPL, "TZCMSLPL", vSubtask, zLEVEL_TASK ) > 0 )
      DropNameForView( vTZCMSLPL, "TZCMSLPL", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_SwitchLPLR( zVIEW vSubtask )
{
   zLONG  lZKey, lDfltZKey;
   zVIEW  vZeidonCM;
   zVIEW  vTZCMSLPL;
   zVIEW  vTZCMWKSO;
   LPTASK lpTask;
// zVIEW  TZCMULWO;
   zCHAR  szFileName[ 514 ] = { 0 };
   zCHAR  szDirectoryName[ 514 ] = { 0 };
// zSHORT nRC;

   GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &vTZCMWKSO, "TZCMWKSO", vZeidonCM, zLEVEL_SUBTASK );
   if ( vTZCMWKSO == 0 )  // View isn't there
   {
      MessageSend( vSubtask, "CM00602", "Configuration Management",
                   "Unable to obtain RepositoryClient View",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Make sure this is the only tool up.
   lpTask = 0;
   while ( (lpTask = SfGetNextTask( lpTask )) != 0 )
   {
      zCHAR szDialogName[ 200 ];

      // Check the name of the dialog associated with the task.  If it begins
      // with TZ then it's a Zeidon Tools task.  We allow the object browser
      // to run and we have to ignore the current task.
      SfGetTaskInfo( szDialogName, sizeof( szDialogName ), zTASK_DIALOG, lpTask );

      // We allow non-Zeidon tasks...
      if ( zstrncmpi( szDialogName, "TZ", 2 ) != 0 )
         continue;

      // We allow the browser...
      if ( zstrcmpi( szDialogName, "TZOBRWAD" ) == 0 )
         continue;

      // We allow the current task.
      if ( lpTask == SfGetCurrentTask( vSubtask ) )
         continue;

      // Uh-oh...some other Zeidon task is running.
      MessageSend( vSubtask, "CM00299", "Configuration Management",
                   "Another Zeidon Tool is currently running.\n"
                   "Shut down all Zeidon Tools before switching Projects. ",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vTZCMSLPL, "TZCMSLPL", vSubtask, zLEVEL_TASK );

   // Get the previous Default LPLR ZKey.
   GetIntegerFromAttribute( &lDfltZKey, vTZCMWKSO, "RepositoryClient",
                            "DefaultLPLR_ZKey" );

   // Get and set the new Default LPLR ZKey.
   GetIntegerFromAttribute( &lZKey, vTZCMSLPL, "LPLR", "ZKey" );
   TerminateLPLR( vSubtask );

   // PETTIT - In the future I will check the return code and continue with the
   // switch based upon it.
   SetAttributeFromInteger( vTZCMWKSO, "RepositoryClient",
                            "DefaultLPLR_ZKey", lZKey );
   SetCursorFirstEntityByInteger( vTZCMWKSO, "LPLR", "ZKey", lZKey, "" );

   // Establish new position on workstation.
   if ( InitializeDefaultLPL( vSubtask ) < 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   else
   {
      oTZCMWKSO_CommitWorkstation( vTZCMWKSO );
      DropNameForView( vTZCMSLPL, "TZCMSLPL", vSubtask, zLEVEL_TASK );
   }
#if 0
   // KJS 04/30/15
   nRC = GetViewByName( &TZCMULWO, "TZCMULWO", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      DropObjectInstance( TZCMULWO );
   }

   GetVariableFromAttribute( szDirectoryName, 0, 'S', 514, vTZCMSLPL, "LPLR", "MetaSrcDir", "", 0 );
   ZeidonStringCopy( szFileName, 1, 0, szDirectoryName, 1, 0, 514 );
   ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( szFileName, 1, 0, "TZCMULWO.POR", 1, 0, 514 );
   nRC = ActivateOI_FromFile( &TZCMULWO, "TZCMULWO", vSubtask, szFileName, zIGNORE_ERRORS | zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = ActivateEmptyObjectInstance( &TZCMULWO, "TZCMULWO", vSubtask, zSINGLE );
      SetNameForView( TZCMULWO, "TZCMULWO", 0, zLEVEL_TASK );
      nRC = CreateEntity( TZCMULWO, "Installation", zPOS_AFTER );
      SetAttributeFromInteger( TZCMULWO, "Installation", "ZKey", 1 );
      SetAttributeFromAttribute( TZCMULWO, "Installation", "Name", vTZCMSLPL, "LPLR", "Name" );
      CommitOI_ToFile( TZCMULWO, szFileName, zASCII );
   }
#endif // looks like the above was eliminated by Don/Kelly???  2105.12.14

   return( 1 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_RebuildMetaLists( zVIEW vSubtask )
{
   if ( MessagePrompt( vSubtask, "CM00609",
                       "Configuration Management",
                       "Do you want to rebuild your Project?",
                       zBEEP,
                       zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   if ( RebuildMetaLists( vSubtask ) == 0 )
   {
      MessageSend( vSubtask, "CM00608", "Configuration Management",
                   "Your project has been rebuilt. ",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_SwapLanguage( zVIEW vSubtask )
{
   zSHORT nLanguageCode;

   SysGetLanguageCode( &nLanguageCode );
   if ( nLanguageCode == 49 )
   {
      MessageSend( vSubtask, "CM00607", "Configuration Management",
                   "Note: The Language has been set to ENGLISH.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );

      SysSetLanguageCode( 1 );
   }
   else
   {
      MessageSend( vSubtask, "CM00607", "Configuration Management",
                   "Hinweis: Der Sprachspeicher wurde auf DEUTSCH gestellt.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SysSetLanguageCode( 49 );
   }

   return( 0 );
}
