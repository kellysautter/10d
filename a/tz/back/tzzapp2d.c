//
// FILENAME:     TZZAPP2D.C
//
// DESCRIPTION:  This is the C source file for dialog TZZAPPSD
//
// OPERATIONS:   OPERATIONS of a general nature.
// ///////////////////////////////////////////////////////////////////
// * Copyright (c) 1997 Ton Beller GmbH                              *
// * Copyright (c) 1997 - 2016 QuinSoft, Inc. All rights reserved.    *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// ///////////////////////////////////////////////////////////////////
//  AUTHOR:  Guenter Trageser (reused stuff from TZCMLPLD.C)
//  DATE:    07.04.1997
//  API:     Zeidon
//  ENVIRONMENT: Win, OS/2
//
//  Last Message ID: AM00001
/*
CHANGE LOG:

2000.10.25  SR  Z2000  Size of path variables
   Modified size of szFileName and szLPLR_Path in function
   TZZAPP2D_SendServerExecs because size of attributes ExecDir, MetaSrcDir and
   PgmSrcDir has been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

07.04.1997  GT
   new module
*/


#define zGLOBAL_DATA
#define KZSYSSVC_INCL

#define HZSLOD_ZTOOLS
#include <tzlodopr.h>   /* Operations for tools */
#include <tz__oprs.h>

#include "KZOENGAA.H"
#include "ZDRVROPR.H"
#include "ZEIDONOP.H"

///////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  TZZAPPSD_SendServerExecs
//
// PURPOSE:    This Entry takes the zeidon.xdm and all xod object instances
//       and sends them to the Server for running the application on
//       the server.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZZAPP2D_SendServerExecs( zVIEW vSubtask )
{
   zVIEW  vZApp;
   zVIEW  vLPLR;
   zVIEW  vMeta;
   zSHORT nRC;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMetaName[ 13 ];
   zCHAR  szAppName[ 32 ];
   zCHAR  szServerSubDir[ 33 ];
   zCHAR  szLPLR_Path[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zVIEW  vApplSubtask;
   zCHAR  szServer[ 33 ];

   // get XLP for selceted application
   GetViewByName( &vZApp, "KZAPPLOO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szAppName, zsizeof( szAppName ), vZApp, "APPLICATION", "APP_NAME" );
   GetStringFromAttribute( szLPLR_Path, zsizeof( szLPLR_Path ), vZApp, "APPLICATION", "APP_ADOBIN" );
   SysAppendcDirSep( szLPLR_Path );
   strcpy_s( szFileName, zsizeof( szFileName ), szLPLR_Path );
   strcat_s( szFileName, zsizeof( szFileName ), szAppName );
   strcat_s( szFileName, zsizeof( szFileName ), ".XLP" );

   // open Task LPLR
   nRC = ActivateOI_FromFile( &vLPLR, "TZCMLPLO", vSubtask, szFileName,
                              zMULTIPLE );
   if ( nRC )
   {
      MessageSend( vSubtask, "CM00285", "Configuration Management",
                   "Can't activate XLP for task.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SetNameForView( vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK);

   zmemset (szServer,0,zsizeof(szServer));
// GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Start the network if necessary.  Use zeidon and hzkzoe as constants for now.
   nRC = NetStatus( "zeidon" );
   if ( nRC == 0 )
   {
      nRC = NetStartup( "zeidon", "hzkzoe" );
      if ( nRC )
      {
         MessageSend( vSubtask, "CM00285", "Configuration Management",
                      "Network cannot be started.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
   }

   // First get the application name and make sure it exists in the
   // zeidon.app object on the server.  If it isn't there, add it.
   GetStringFromAttribute( szAppName, zsizeof( szAppName ), vLPLR, "LPLR", "Name" );
   strcpy_s( szFileName, zsizeof( szFileName ), "zeidon.app" );
   SfCreateSubtask( &vApplSubtask, vSubtask, "Zeidon System" );
   nRC = NetActivateOI_FromFile( 0, szServer, &vMeta, "KZAPPLOO",
                                 vApplSubtask, szFileName, zSINGLE );
   if ( nRC >= 0 )
   {
      nRC = SetCursorFirstEntityByString( vMeta, "APPLICATION",
                                          "APP_NAME", szAppName, 0 );
      if ( nRC >= zCURSOR_SET )
         DropObjectInstance( vMeta );
      else
      {
         CreateEntity( vMeta, "APPLICATION", zPOS_LAST );
         SetAttributeFromString( vMeta, "APPLICATION", "APP_NAME", szAppName );
         strcpy_s( szServerSubDir, zsizeof( szServerSubDir ), szAppName );
         strcat_s( szServerSubDir, zsizeof( szServerSubDir ), "/" );
         SetAttributeFromString( vMeta, "APPLICATION",
                                 "APP_DLL", szServerSubDir );
         SetAttributeFromString( vMeta, "APPLICATION",
                                 "APP_ADOBIN", szServerSubDir );
         SetAttributeFromString( vMeta, "APPLICATION",
                                 "APP_LOCAL", szServerSubDir );
         TraceLineS( "*** Committing server file: ", szFileName );
         strcpy_s( szMsg, zsizeof( szMsg ), "Sending ZEIDON.APP: " );
         MB_SetMessage( vSubtask, 1, szMsg );

         // Send OI to server.
         NetCommitOI_ToFile( 0, szServer, vMeta, szFileName, zASCII );

         DropObjectInstance( vMeta );
      }
   }
   else
   {
      SfDropSubtask( vApplSubtask, 0 );
      MessageSend( vSubtask, "CM00286", "Configuration Management",
                   "ZEIDON.APP cannot be activated.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   SfDropSubtask( vApplSubtask, 0 );

   // Send the XDM before all the xods.
   // First format the file name for activation on the client and then
   // format it for commit on the server.
// GetStringFromAttribute( szFileName, zsizeof( szFileName ), vLPLR, "LPLR", "ExecDir" );
   strcpy_s( szFileName, zsizeof( szFileName ), szLPLR_Path );
   strcpy_s( szMetaName, zsizeof( szMetaName ), "ZEIDON.XDM" );
   strcat_s( szFileName, zsizeof( szFileName ), szMetaName );
   TraceLineS( "*** Activating client file: ", szFileName );
   nRC = ActivateOI_FromFile( &vMeta, "TZDMXGPO", vSubtask,
                              szFileName, zSINGLE );

   if ( nRC >= 0 )
   {
      // Filename includes Directory-prefix **HH**
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vLPLR, "LPLR", "Name" );
      strcat_s( szFileName, zsizeof( szFileName ), "/" );
      strcat_s( szFileName, zsizeof( szFileName ), szMetaName );
      TraceLineS( "*** Committing server file: ", szFileName );
      MB_SetMessage( vSubtask, 1, "Sending XDM" );

      // Send OI to server.
      NetCommitOI_ToFile( 0, szServer, vMeta, szFileName, zASCII );
   }

   // Next loop through all the LODs, activating each XOD and sending
   // it to the server.
   SetCursorFirstEntityByInteger( vLPLR, "W_MetaType", "Type",
                                  zREFER_LOD_META, 0 );
   nRC = SetCursorFirstEntity( vLPLR, "W_MetaDef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      strcpy_s( szFileName, zsizeof( szFileName ), szLPLR_Path );
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vLPLR, "W_MetaDef", "Name" );
      strcat_s( szFileName, zsizeof( szFileName ), szMetaName );
      strcat_s( szFileName, zsizeof( szFileName ), ".XOD" );
      TraceLineS( "*** Activating client file: ", szFileName );
      nRC = ActivateOI_FromFile( &vMeta, "TZZOXODO", vSubtask,
                                 szFileName, zSINGLE );
      if ( nRC >= 0 )
      {
        // Filename includes Directory-prefix **HH**
        GetStringFromAttribute( szFileName, zsizeof( szFileName ), vLPLR, "LPLR", "Name" );
        strcat_s( szFileName, zsizeof( szFileName ), "/" );
        strcat_s( szFileName, zsizeof( szFileName ), szMetaName );
        strcat_s( szFileName, zsizeof( szFileName ), ".XOD" );
        TraceLineS( "*** Committing server file: ", szFileName );
        strcpy_s( szMsg, zsizeof( szMsg ), "Sending XOD: " );
        strcat_s( szMsg, zsizeof( szMsg ), szMetaName );
        MB_SetMessage( vSubtask, 1, szMsg );

        // Send OI to server.
        NetCommitOI_ToFile( 0, szServer, vMeta, szFileName, zASCII );
      }

      nRC = SetCursorNextEntity( vLPLR, "W_MetaDef", 0 );
   }

   MB_SetMessage( vSubtask, 1, "Transmission Complete" );

   return( 0 );
}
