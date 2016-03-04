/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzdmxgpo.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
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
// LAST MESSAGE ID: DM00201
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
   Modified size of szXDM_FileName in function oTZDMXGPO_GetViewForXDM and
   szXDM_FileName and szApplExecDir in function oTZDMXGPO_GetViewForXDM
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMXGPO_GetViewForXDM
//
// PURPOSE:   Get access to XDM View.  Activate if not loaded
//            and get at application level if loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMXGPO_GetViewForXDM( zVIEW  vSubtask,
                         zPVIEW pvTZDMXGPO,
                         zSHORT nCurrentOrReload )
{
   zVIEW  vT;
   zVIEW  vTask;
   zCHAR  szXDM_FileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szAppViewName[ 66 ] = "dmDOMAINS.";
   zSHORT nRScope;
   zSHORT nRC;

// Set up the XDM File Name and the View Name

   GetViewByName( &vTask, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szAppViewName + 10, zsizeof( szAppViewName ) - 10, vTask, "LPLR", "Name" );
   GetStringFromAttribute( szXDM_FileName, zsizeof( szXDM_FileName ), vTask, "LPLR", "ExecDir" );
   strcat_s( szXDM_FileName, zsizeof( szXDM_FileName ), "\\zeidon.xdm" );

// See if XDM View is at the Application level
   nRScope = GetViewByName( &vT, szAppViewName, vTask, zLEVEL_APPLICATION );
   if ( nRScope == zCALL_ERROR )
      return( zCALL_ERROR );

   if ( nRScope == zLEVEL_APPLICATION && nCurrentOrReload == zFORCE_RELOAD )
   {
      nRC = DropObjectInstance( vT );
      nRScope = -1;
   }

   if ( nRScope != zLEVEL_APPLICATION )
   {
      // Load it from a file or create a new one if file is missing.
      if ( ActivateOI_FromFile( &vT, "TZDMXGPO", vTask, szXDM_FileName,
                             zSINGLE | zLEVEL_APPLICATION | zNOI_OKAY ) != 0 )
      {
         ActivateEmptyObjectInstance( &vT, "TZDMXGPO", vTask,
                                      zSINGLE | zLEVEL_APPLICATION );
         CreateEntity( vT, "DomainGroup", zPOS_AFTER );
         SetAttributeFromString( vT, "DomainGroup", "Name",
                                 &szAppViewName[ 10 ] );
         SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
         CommitOI_ToFile( vT, szXDM_FileName, zASCII );

      // MessageSend( vT, "DM00201", "Domain Management",
      //              "The Executable Domain File was not found.  "
      //              "An empty one is being created.  You may want "
      //              "to save all Domains OR re-Initialze the LPLR.",
      //              zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         nRC = 1;
      }
      else
      {
         SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
         nRC = 0;
      }
   }
   else
   {
     nRC = 0;
   }

   CreateViewFromViewForTask( pvTZDMXGPO, vT, 0 );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMXGPO_CommitXDM
//
// PURPOSE:   Commit the current XDM to the DOMAINS.XDM file.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMXGPO_CommitXDM( zVIEW vSubtask, zVIEW vTZDMXGPO )
{
   zVIEW  vT;
   zCHAR  szXDM_FileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szApplExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

// Set up the XDM File Name

   GetViewByName( &vT, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szXDM_FileName, zsizeof( szXDM_FileName ), vT, "LPLR", "ExecDir" );

   GetApplDirectoryFromView( szApplExecDir, vT, 2, zMAX_FILESPEC_LTH+1 );
   szApplExecDir[ zstrlen( szApplExecDir ) - 1 ] = 0;

   strcat_s( szXDM_FileName, zsizeof( szXDM_FileName ), "\\zeidon.xdm" );
   nRC = CommitOI_ToFile( vTZDMXGPO, szXDM_FileName, zASCII );
   return( nRC );
}

///////////////////////////// end of file  //////////////////////////////////
