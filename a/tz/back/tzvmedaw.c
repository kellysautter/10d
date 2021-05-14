/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmedaw.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc. All rights reserved.         *
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
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmlopr.hg"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZVSEDAW_InitializeDialog
//
// PURPOSE:  Load a source object
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSEDAW_InitializeDialog( zVIEW vSubtask )
{
   zVIEW  vSource;

   ActivateEmptyObjectInstance( &vSource, "TZVSSROO", vSubtask, zSINGLE );

   CreateEntity( vSource, "VMLMODULE", zPOS_AFTER );

   SetNameForView( vSource, "TZVSSROO", vSubtask, zLEVEL_SUBTASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZVSEDAW_TerminateDialog
//
// PURPOSE:  Delete a source object
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSEDAW_TerminateDialog( zVIEW vSubtask )
{
   zVIEW  vSource;

   GetViewByName( &vSource, "TZVSSROO", vSubtask, zLEVEL_SUBTASK );

   DropView( vSource );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZVSEDAW_OpenSourceFile
//
// PURPOSE:  Put an Instance into a source object
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSEDAW_OpenSourceFile( zVIEW vSubtask )
{
   zVIEW  vSource;
   zPCHAR lpLine;
   zCHAR  szFileName[ 128 ];
   zCHAR  szDirectory[ 128 ];
   zCHAR  szQualifiedName[ 128 ];
   zLONG  k;
   zLONG  lfHandle;


   GetViewByName( &vSource, "TZVSSROO", vSubtask, zLEVEL_SUBTASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vSource, "VMLMODULE", "NAME" );

   strcat_s( szFileName, zsizeof( szFileName ), ".VML" );

   GetStringFromAttribute( szDirectory, zsizeof( szDirectory ), vSource, "VMLMODULE", "DIRECTORY" );

   //   GetApplDirectoryFromView( szDirectory, vSource,
   //                             zAPPL_DIR_OBJECT, 128 );
   //
   strcpy_s( szQualifiedName, zsizeof( szQualifiedName ), szDirectory );
   strcat_s( szQualifiedName, zsizeof( szQualifiedName ), szFileName );

   lfHandle = SysOpenFile( vSubtask, szQualifiedName, COREFILE_READ );

   k = 0;
   while ( SysReadLine( vSubtask, &lpLine, lfHandle ) > 0 )
   {
      k++;
      CreateEntity( vSource, "TEXTLINE", zPOS_AFTER );
      SetAttributeFromString( vSource, "TEXTLINE",
                              "TEXT", lpLine );
      SetAttributeFromInteger( vSource, "TEXTLINE",
                               "TEXTLINENUMBER", k );
   }

   return( 0 );
}
