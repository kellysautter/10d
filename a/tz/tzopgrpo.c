/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzopgrpo.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
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
// LAST MESSAGE ID: OP01001
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
/*
CHANGE LOG

2000.10.24  SR  Z2000 Size of path variables
   Modified size of szXGO_FileName in function oTZOPGRPO_GetViewForXGO
   and szXGO_FileName and szApplExecDirbecause in function oTZOPGRPO_CommitXGO
   because the size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZOPGRPO_GetViewForXGO
//
// PURPOSE:   Get access to XGO View.  Activate if not loaded
//            and get at application level if loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZOPGRPO_GetViewForXGO( zVIEW  vSubtask,
                         zPVIEW pvTZOPGRPO,
                         zSHORT nCurrentOrReload )
{
   zVIEW  vT;
   zVIEW  vTask;
   zCHAR  szXGO_FileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szOpGenType[ 2 ];
   zSHORT nRC;
   zSHORT nRScope;
   zCHAR  szAppViewName[ 66 ];

// Set up the XGO File Name and the View Name
   if ( nCurrentOrReload == zCURRENT_OI || nCurrentOrReload == zFORCE_RELOAD )
   {
      strcpy_s( szAppViewName, zsizeof( szAppViewName ), "xoOPERSIGS." );
      nRC = GetViewByName( &vT, "TaskLPLR", vSubtask, zLEVEL_TASK );
      nRC = GetStringFromAttribute( szXGO_FileName, zsizeof( szXGO_FileName ), vT, "LPLR", "ExecDir" );
      if ( szXGO_FileName[ zstrlen( szXGO_FileName ) - 1 ] != '\\' )
          strcat_s( szXGO_FileName, zsizeof( szXGO_FileName ), "\\" );

      strcat_s( szXGO_FileName, zsizeof( szXGO_FileName ), "opersigs.xgo" );
   }
   else
   {
      strcpy_s( szAppViewName, zsizeof( szAppViewName ), "xoTZOPSIGK." );
      nRC = SfCreateSubtask( &vT, vSubtask, "Zeidon System" );
      if ( nRC == zCALL_ERROR )
         return( zCALL_ERROR );

      nRC = GetApplDirectoryFromView( szXGO_FileName, vT, zAPPL_DIR_OBJECT, zMAX_FILENAME_LTH + 1 );
      nRC = SfDropSubtask( vT, 0 );
      strcat_s( szXGO_FileName, zsizeof( szXGO_FileName ), "tzopsigk.xgo" );
   }

// See if XGO View is at the Application level
   nRC = GetViewByName( &vTask, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vTask )
   {
      GetStringFromAttribute( szAppViewName + 11, zsizeof( szAppViewName ) - 11, vTask, "LPLR", "Name" );
      nRScope = GetViewByName( &vT, szAppViewName, vTask, zLEVEL_APPLICATION );
      if ( nRScope == zCALL_ERROR )
         return( nRScope );

      if ( nRScope == zLEVEL_APPLICATION &&
           (nCurrentOrReload == zFORCE_RELOAD ||
            nCurrentOrReload == zSYS_FORCE_RELOAD) )
      {
         nRC = DropObjectInstance( vT );
         nRScope = -1;
      }
   }
   else
      nRScope = -1;

   if ( nRScope != zLEVEL_APPLICATION )
   {
      // Load it from a file or create a new one if file is missing.
      if ( ActivateOI_FromFile( &vT, "TZOPGRPO", vTask, szXGO_FileName, zSINGLE | zLEVEL_APPLICATION ) != 0 )
      {
         nRC = ActivateEmptyObjectInstance( &vT, "TZOPGRPO", vTask, zSINGLE | zLEVEL_APPLICATION );
         if ( nRC >= 0 )
         {
            nRC = CreateEntity( vT, "OPERSIG", zPOS_AFTER );
            nRC = SetAttributeFromString( vT, "OPERSIG", "Name", &szAppViewName[ 10 ] );
            //nRC = CommitOI_ToFile( vT, szXGO_FileName, zASCII );
            nRC = SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
         }
         else
            return( -1 );
      }
      else
     {
        // KJS 04/24/13 - We have a couple of TZOPSIGK operations that are different when generating to c or to java. I have implemented a new Operation Generation Type for a
       // TZOPSIGK operation where you can choose Java or C and so for some operations we have two declarations one for java and one for c.
       // I am wondering if we should have a flag perhaps in the zeidon.ini file and/or on the "Operation Insert" screen to indicate what operations you want
       // to see in the list but because for right now the operations are exactly the same except the C versions have some parameters as pointers (Return Value),
       // I am going to limit the list to show only the C version of an operation. That way when the user tries to insert an operation they are not confused by
       // seeing two. This is only for a couple of operations like StoreValueInRecord and SetEntityCursor etc.
         nRC = SetCursorFirstEntity( vT, "Operation", "" );
         while ( nRC >= zCURSOR_SET )
         {
            szOpGenType[ 0 ] = 0;
            GetStringFromAttribute( szOpGenType, zsizeof( szOpGenType ), vT, "Operation", "GenerationType" );
           if ( szOpGenType[0] == 'J' )
           {
              DropEntity( vT, "Operation", zREPOS_NONE );
           }
            nRC = SetCursorNextEntity( vT, "Operation", "" );
         }
         nRC = SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
     }
   }

   nRC = CreateViewFromViewForTask( pvTZOPGRPO, vT, 0 );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZOPGRPO_CommitXGO
//
// PURPOSE:   Commit the current XGO to the OPERSIGS.XGO file.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZOPGRPO_CommitXGO( zVIEW vTZOPGRPO )
{
   zVIEW  vT;
   zCHAR  szXGO_FileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szApplExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMsg[ zMAX_FILESPEC_LTH + zSHORT_MESSAGE_LTH + 1 ];
   zSHORT nRC;

// Set up the XGO File Name

   GetViewByName( &vT, "TaskLPLR", vTZOPGRPO, zLEVEL_TASK );
   GetStringFromAttribute( szXGO_FileName, zsizeof( szXGO_FileName ), vT, "LPLR", "ExecDir" );

   GetApplDirectoryFromView( szApplExecDir, vT, 2, zMAX_FILESPEC_LTH + 1 );
   szApplExecDir[ zstrlen( szApplExecDir ) - 1 ] = 0;
   if ( zstrcmpi( szApplExecDir, szXGO_FileName ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "The Executable version of the Operation is about to "
                      "stored in the executable directory of your currently "
                      "executing Application.\n\n\t" );
      strcat_s( szMsg, zsizeof( szMsg ), szApplExecDir );
      strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you wish to continue?" );
      nRC = MessagePrompt( vT, "OP01001", "Operations", szMsg,
                           zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,  0 );
      if ( nRC == zRESPONSE_NO )
         return( -1 );
   }

   strcat_s( szXGO_FileName, zsizeof( szXGO_FileName ), "\\opersigs.xgo" );
   nRC = CommitOI_ToFile( vTZOPGRPO, szXGO_FileName, zASCII );
   return( nRC );
}

///////////////////////////// end of file  //////////////////////////////////
