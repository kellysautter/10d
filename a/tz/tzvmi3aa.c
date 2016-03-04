/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmi3aa.c - Interpretor operations
// DESCRIPTION:
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
// LAST MESSAGE ID: VM03302
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.25  SR  Z2000 Size of path variables
   Modified size of szSourceMetaDirectory, szTargetMetaDirectory,
   szSourceExeDirectory, szTargetExeDirectory in function Copy because size of
   attributes ExecDir, MetaSrcDir and PgmSrcDir has been changed to 254 in
   datamodel.


2000.07.18 HH
   fix for multiple interpreter entry.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmlopr.hg"
#include "ZeidonOp.H"

#define BUFFER_SIZE  65355  // needs to be 64K ... should be allocated


#define NBR_DOMAINS  16
zCHAR  *szDomainFiles[ NBR_DOMAINS ] = { "Z06AD7E9.PDM",
                                         "Z00FC671.PDM",
                                         "Z01EC6D1.PDM",
                                         "Z01EC715.PDM",
                                         "Z0000100.PDM",
                                         "Z0000180.PDM",
                                         "Z0000200.PDM",
                                         "Z0000300.PDM",
                                         "Z0000400.PDM",
                                         "Z0000500.PDM",
                                         "Z0000600.PDM",
                                         "Z0000800.PDM",
                                         "Z0000900.PDM",
                                         "Z0000A00.PDM",
                                         "Z0000B00.PDM",
                                         "Z0000E00.PDM"
                                        };

zOPER_EXPORT zSHORT OPERATION
PreBuild( zVIEW vSubtask )
{
   zVIEW  vTZVSINIT;
   zVIEW  vLPLR;

   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 1 )
      InitializeDefaultLPL( vSubtask );

   ActivateEmptyObjectInstance( &vTZVSINIT, "TZVSINIT", vSubtask, zSINGLE );

   SetNameForView( vTZVSINIT, "TZVSINIT", vSubtask, zLEVEL_TASK );

   CreateEntity( vTZVSINIT, "ROOT", zPOS_AFTER );
   SetAttributeFromString( vTZVSINIT, "ROOT", "SourceOfSourceCopy",
                           "C:\\10D\\A\\BIN\\SYS" );
   SetAttributeFromString( vTZVSINIT, "ROOT", "SourceOfExeCopy",
                           "C:\\10D\\W\\BIN\\SYS" );
   if ( vLPLR )
   {
      SetAttributeFromAttribute( vTZVSINIT, "ROOT", "TargetOfSourceCopy",
                                 vLPLR, "LPLR", "MetaSrcDir" );
      SetAttributeFromAttribute( vTZVSINIT, "ROOT", "TargetOfExeCopy",
                                 vLPLR, "LPLR", "ExecDir" );
   }

   SetAttributeFromInteger( vTZVSINIT, "ROOT", "IncludeDomains", 1 );
   SetAttributeFromInteger( vTZVSINIT, "ROOT", "IncludePPE", 1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PostBuild( zVIEW vSubtask )
{
   zVIEW  vLPLR;
   zPCHAR szLPLR_Name;

// Set LPL Name on Window Title
   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetAddrForAttribute( &szLPLR_Name, vLPLR, "LPLR", "Name" );
      SetWindowCaptionTitle( vSubtask, 0, szLPLR_Name );
   }

   MessageSend( vSubtask, "VM03301", "VML Interpretor",
                "Select NEW LPLR.\n\n"
                "If the current LPLR is not the one you are "
                "trying to initialize, simply Switch LPLR to the "
                "correct one.\n\n"
                 "DO NOT rebuild the MetaList yet.",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
RtnFromSubWnd( zVIEW  vSubtask )
{
   zVIEW  vTZVSINIT;
   zVIEW  vLPLR;
   zPCHAR szLPLR_Name;

// Set LPL Dirs into work and reset caption
   GetViewByName( &vTZVSINIT, "TZVSINIT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vLPLR && vTZVSINIT )
   {
      GetAddrForAttribute( &szLPLR_Name, vLPLR, "LPLR", "Name" );
      SetWindowCaptionTitle( vSubtask, 0, szLPLR_Name );
      SetAttributeFromAttribute( vTZVSINIT, "ROOT", "TargetOfSourceCopy",
                                 vLPLR, "LPLR", "MetaSrcDir" );
      SetAttributeFromAttribute( vTZVSINIT, "ROOT", "TargetOfExeCopy",
                                 vLPLR, "LPLR", "ExecDir" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Copy( zVIEW  vSubtask )
{
   zLONG  lSourceMetaLen;
   zLONG  lTargetMetaLen;
   zLONG  lSourceExeLen;
   zLONG  lTargetExeLen;
   zLONG  lLth;
   zLONG  lBytesRead;
   zLONG  SourceFileHandle;
   zLONG  TargetFileHandle;
   zCHAR  szSourceMetaDirectory[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTargetMetaDirectory[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szSourceExeDirectory[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTargetExeDirectory[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR lpReadBuffer;
   zPCHAR szDir;
   zVIEW  vTZVSINIT;
   zVIEW  vLPLR;
   zLONG  hMemory;
   zSHORT k;
   zLONG  lRC;

   GetViewByName( &vTZVSINIT, "TZVSINIT", vSubtask, zLEVEL_TASK );

   // init LPLR dir's if they are not set
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vLPLR )
   {
      GetAddrForAttribute( &szDir, vLPLR, "LPLR", "MetaSrcDir" );
      if ( szDir[ 0 ] == 0 )
      {
         SetAttributeFromAttribute( vLPLR, "LPLR", "MetaSrcDir",
                                    vTZVSINIT, "ROOT", "TargetOfSourceCopy" );
      }
      GetAddrForAttribute( &szDir, vLPLR, "LPLR", "PgmSrcDir" );
      if ( szDir[ 0 ] == 0 )
      {
         SetAttributeFromAttribute( vLPLR, "LPLR", "PgmSrcDir",
                                    vTZVSINIT, "ROOT", "TargetOfSourceCopy" );
      }
      GetAddrForAttribute( &szDir, vLPLR, "LPLR", "ExecDir" );
      if ( szDir[ 0 ] == 0 )
      {
         SetAttributeFromAttribute( vLPLR, "LPLR", "ExecDir",
                                    vTZVSINIT, "ROOT", "TargetOfExeCopy" );
      }
   }

   GetViewByName( &vLPLR, "TZCMWKSO", vSubtask, zLEVEL_TASK );
   if ( vLPLR )
   {
      GetAddrForAttribute( &szDir, vLPLR, "LPLR", "MetaSrcDir" );
      if ( szDir[ 0 ] == 0 )
      {
         SetAttributeFromAttribute( vLPLR, "LPLR", "MetaSrcDir",
                                    vTZVSINIT, "ROOT", "TargetOfSourceCopy" );
      }
      GetAddrForAttribute( &szDir, vLPLR, "LPLR", "PgmSrcDir" );
      if ( szDir[ 0 ] == 0 )
      {
         SetAttributeFromAttribute( vLPLR, "LPLR", "PgmSrcDir",
                                    vTZVSINIT, "ROOT", "TargetOfSourceCopy" );
      }
      GetAddrForAttribute( &szDir, vLPLR, "LPLR", "ExecDir" );
      if ( szDir[ 0 ] == 0 )
      {
         SetAttributeFromAttribute( vLPLR, "LPLR", "ExecDir",
                                    vTZVSINIT, "ROOT", "TargetOfExeCopy" );
      }
   }

   // set up the Source meta directory first,
   lRC =
      GetStringFromAttribute( szSourceMetaDirectory, zsizeof( szSourceMetaDirectory ),
                              vTZVSINIT, "ROOT", "SourceOfSourceCopy" );

   lLth = zstrlen( szSourceMetaDirectory );
   if ( szSourceMetaDirectory[ lLth ] != '\\' )
   {
      szSourceMetaDirectory[ lLth ] = '\\';
      szSourceMetaDirectory[ lLth + 1 ] = 0;
   }
   lLth = 0;
   // set up the target meta directory
   lRC =
      GetStringFromAttribute( szTargetMetaDirectory, zsizeof( szTargetMetaDirectory ),
                              vTZVSINIT, "ROOT", "TargetOfSourceCopy" );

   lLth = zstrlen( szTargetMetaDirectory );
   if ( szTargetMetaDirectory[ lLth ] != '\\' )
   {
      szTargetMetaDirectory[ lLth ] = '\\';
      szTargetMetaDirectory[ lLth + 1 ] = 0;
   }
   lLth = 0;

   // set up the Source exec directory,
   lRC =
      GetStringFromAttribute( szSourceExeDirectory, zsizeof( szSourceExeDirectory ),
                              vTZVSINIT, "ROOT", "SourceOfExeCopy" );

   lLth = zstrlen( szSourceExeDirectory );
   if ( szSourceExeDirectory[ lLth ] != '\\' )
   {
      szSourceExeDirectory[ lLth ] = '\\';
      szSourceExeDirectory[ lLth + 1 ] = 0;
   }
   lLth = 0;
   // set up the Target exec directory,
   lRC =
      GetStringFromAttribute( szTargetExeDirectory, zsizeof( szTargetExeDirectory ),
                              vTZVSINIT, "ROOT", "TargetOfExeCopy" );

   lLth = zstrlen( szTargetExeDirectory );
   if ( szTargetExeDirectory[ lLth ] != '\\' )
   {
      szTargetExeDirectory[ lLth ] = '\\';
      szTargetExeDirectory[ lLth + 1 ] = 0;
   }
   lLth = 0;

   // DOMAINS
   // if the Include Domains was selected, open each Domain, copy
   //   it to the target an save it.

   hMemory = SysAllocMemory( &lpReadBuffer, BUFFER_SIZE,
                             0, zCOREMEM_ALLOC, 0 );

   lSourceExeLen = zstrlen( szSourceExeDirectory );
   lTargetExeLen = zstrlen( szTargetExeDirectory );

   lSourceMetaLen = zstrlen( szSourceMetaDirectory );
   lTargetMetaLen = zstrlen( szTargetMetaDirectory );
   if ( CompareAttributeToInteger( vTZVSINIT, "ROOT", "IncludeDomains", 1 ) == 0 )
   {
      k = 0;

      while ( k < NBR_DOMAINS )
      {
         strcpy_s( szSourceMetaDirectory + lSourceMetaLen, zsizeof( szSourceMetaDirectory ) - lSourceMetaLen, szDomainFiles[ k ] );
         strcpy_s( szTargetMetaDirectory + lTargetMetaLen, zsizeof( szTargetMetaDirectory ) - lTargetMetaLen, szDomainFiles[ k ] );
         SourceFileHandle = SysOpenFile( vSubtask, szSourceMetaDirectory,COREFILE_READ );
         if ( SourceFileHandle == -1 )
            TraceLineS( "ERROR Could not open: ", szSourceMetaDirectory );

         TargetFileHandle =
            SysOpenFile( vSubtask, szTargetMetaDirectory,COREFILE_CREATE );

         if ( TargetFileHandle == -1 )
            TraceLineS( "ERROR Could not open: ", szTargetMetaDirectory );

         lBytesRead = SysReadFile( vSubtask, SourceFileHandle, lpReadBuffer, BUFFER_SIZE );
         lRC = SysWriteFile( vSubtask, TargetFileHandle, lpReadBuffer, (zUSHORT) lBytesRead );

         SysCloseFile( vSubtask, SourceFileHandle, 0 );
         SysCloseFile( vSubtask, TargetFileHandle, 0 );
         k++;
      }

      strcpy_s( szSourceExeDirectory + lSourceExeLen, zsizeof( szSourceExeDirectory ) - lSourceExeLen, "DOMAINS.IDM" );
      strcpy_s( szTargetExeDirectory + lTargetExeLen, zsizeof( szTargetExeDirectory ) - lTargetExeLen, "ZEIDON.XDM" );

      SourceFileHandle = SysOpenFile( vSubtask, szSourceExeDirectory, COREFILE_READ );

      if ( SourceFileHandle == -1 )
         TraceLineS( "ERROR Could not open: ", szSourceExeDirectory );

      TargetFileHandle = SysOpenFile( vSubtask, szTargetExeDirectory,
                                      COREFILE_CREATE );

      if ( TargetFileHandle == -1 )
         TraceLineS( "ERROR Could not open: ", szTargetExeDirectory );

      lBytesRead = SysReadFile( vSubtask, SourceFileHandle, lpReadBuffer, (zUSHORT) BUFFER_SIZE );
      lRC = SysWriteFile( vSubtask, TargetFileHandle, lpReadBuffer, (zUSHORT) lBytesRead );
      SysCloseFile( vSubtask, SourceFileHandle, 0 );
      SysCloseFile( vSubtask, TargetFileHandle, 0 );
   }

   // PPE
   if ( CompareAttributeToInteger( vTZVSINIT, "ROOT", "IncludePPE", 1 ) == 0 )
   {
      strcpy_s( szSourceMetaDirectory + lSourceMetaLen, zsizeof( szSourceMetaDirectory ) - lSourceMetaLen, "ZEIDON.PPE" );
      strcpy_s( szTargetMetaDirectory + lTargetMetaLen, zsizeof( szTargetMetaDirectory ) - lTargetMetaLen, "ZEIDON.PPE" );

      SourceFileHandle = SysOpenFile( vSubtask, szSourceMetaDirectory,COREFILE_READ );

      if ( SourceFileHandle == -1 )
         TraceLineS( "ERROR Could not open: ", szSourceMetaDirectory );

      TargetFileHandle = SysOpenFile( vSubtask, szTargetMetaDirectory,COREFILE_CREATE );

      if ( TargetFileHandle == -1 )
         TraceLineS( "ERROR Could not open: ", szTargetMetaDirectory );

      lBytesRead = SysReadFile( vSubtask, SourceFileHandle, lpReadBuffer, (zUSHORT) BUFFER_SIZE );
      lRC = SysWriteFile( vSubtask, TargetFileHandle, lpReadBuffer, (zUSHORT) lBytesRead );
      SysCloseFile( vSubtask, SourceFileHandle, 0 );
      SysCloseFile( vSubtask, TargetFileHandle, 0 );

      strcpy_s( szSourceExeDirectory + lSourceExeLen, zsizeof( szSourceExeDirectory ) - lSourceExeLen, "ZEIDON.XPE" );
      strcpy_s( szTargetExeDirectory + lTargetExeLen, zsizeof( szTargetExeDirectory ) - lTargetExeLen, "ZEIDON.XPE" );

      SourceFileHandle = SysOpenFile( vSubtask, szSourceExeDirectory,
                                      COREFILE_READ );
      if ( SourceFileHandle == -1 )
         TraceLineS( "ERROR Could not open: ", szSourceExeDirectory );

      TargetFileHandle = SysOpenFile( vSubtask, szTargetExeDirectory,
                                      COREFILE_CREATE );

      if ( TargetFileHandle == -1 )
         TraceLineS( "ERROR Could not open: ", szTargetExeDirectory );

      lBytesRead = SysReadFile( vSubtask, SourceFileHandle,
                                lpReadBuffer, (zUSHORT) BUFFER_SIZE );
      SysWriteFile( vSubtask, TargetFileHandle,
                    lpReadBuffer, (zUSHORT) lBytesRead );
      SysCloseFile( vSubtask, SourceFileHandle, 0 );
      SysCloseFile( vSubtask, TargetFileHandle, 0 );
   }

   MessageSend( vSubtask, "VM03302", "VML Interpretor",
                "Goto Rebuild.\n\n"
                "Please Select the Rebuild Button and Rebuild the Meta"
                 "List now. ",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   SysFreeMemory( hMemory );
   return( 0 );
}
