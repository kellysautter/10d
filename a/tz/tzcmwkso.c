/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmwkso.c - Zeidon application operations
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
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

1999.12.22 BL
   turn the created flag off, before we save the workstation to por file.

1997.11.13    DGC
   Changed CopyFile() to SysCopyFile().

*/


#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"


void OPERATION
ofnTZCMWKSO_AppendSlash( zPCHAR pchDirectory )
{
   zLONG k;

   k = zstrlen( pchDirectory );
   if ( pchDirectory[ k - 1 ] != '\\' )
   {
      pchDirectory[ k ] = '\\';
      pchDirectory[ k + 1 ] = 0;
   }
}

//./ ADD NAME=oTZCMWKSO_GetWKS_FileName
// Source Module=tzcmwkso.c
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  oTZCMREPO_GetWKS_FileName
//
//  PURPOSE:    To retrieve the Zeidon WorkStation( RepositoryClient )
//              file name
//
//  PARAMETERS: pchZeidonWKS - a pointer to a string which is returned
//                with the fully qualified RepositoryClient File Name.
//
//  RETURNS:    0 - FileName successfully retrieved
//             -1 - Error encountered
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
oTZCMWKSO_GetWKS_FileName( zPCHAR pchZeidonWKS, zLONG lMaxLth )
{
   if ( SysGetEnvVar( pchZeidonWKS, "ZEIDON", lMaxLth ) == 0 )
   {
      ofnTZCMWKSO_AppendSlash( pchZeidonWKS );
      strcat_s( pchZeidonWKS, lMaxLth, "TZCMWKS8.POR" );
      return( 0 );
   }
   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
oTZCMWKSO_ActivateWorkStation( zPVIEW vTZCMWKSO, zVIEW vQual, zLONG lControl )
{
   zCHAR  szZeidonWKS[ 256 ];

   if ( oTZCMWKSO_GetWKS_FileName( szZeidonWKS, zsizeof( szZeidonWKS ) ) == 0 )
      return( ActivateOI_FromFile( vTZCMWKSO, "TZCMWKSO", vQual, szZeidonWKS, lControl ) );
   return( -1 );
}

//./ ADD NAME=oTZCMWKSO_CommitWorkstation
// Source Module=tzcmoprs.c
/////////////////////////////////////////////////////////////////////////////
//
//  CM OPERATION: oTZCMWKSO_CommitWorkstation
//
//  PURPOSE:    To localize the Commit To File of the Work Station Object,
//              setting the correct flags.
//
//  PARAMETERS: WKS_View - view to the Work Station OI
//
//  RETURNS:    0           - Instance successfully Committed
//              zCALL_ERROR - Error encountered
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
oTZCMWKSO_CommitWorkstation( zVIEW WKS_View )
{
   zCHAR szZeidonWKS[ 256 ];

   if ( oTZCMWKSO_GetWKS_FileName( szZeidonWKS, zsizeof( szZeidonWKS ) ) == 0 )
   {
      // To avoid error messages on checkin of objects, that have RepositoryClient included,
      // we turn the created flag off, before we save the workstation to por file.
      // zSET_INCR_PERSISTENT clears the "created" flag.
      SetIncrementalUpdateFlags( WKS_View, "RepositoryClient",
                                 zSET_INCR_PERSISTENT | zSET_INCR_CURSORPOS );
      return( CommitOI_ToFile( WKS_View, szZeidonWKS, zSINGLE | zINCREMENTAL ) );
   }

   return( -1 );
}

///////////////////////////// end of file  //////////////////////////////////
