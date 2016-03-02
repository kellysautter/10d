/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmpmoo.c - Zeidon application operations
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
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZVSPOOO_C_GenOperationName
//
// PURPOSE:   For Object and Local operations, concatenate the LOD
//            name in front of the VML operation name.
//            For others, just use the regular operation name.
//            The Type for a Local Oper is L.
//            The Types for Object operations are either O, T, C, E or A.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZVSPOOO_C_GenOperationName( zVIEW  PIView,
                              LPVIEWENTITY EntityStructure,
                              LPVIEWATTRIB AttributeStructure,
                              zSHORT nGetOrSetFlag )
{
   zCHAR  szTempName1[ 64 ];
   zCHAR  szTempName2[ 32 ];
   zCHAR  szType[ 2 ];
   zLONG  lZKey;
   zVIEW  vTempView;

   // Set up a temporary view so that PIView remains unchanged
   CreateViewFromViewForTask( &vTempView, PIView, 0 );

   // Position on the corresponding OperationSource for Operation entity
   GetIntegerFromAttribute( &lZKey, vTempView, "Operation", "ZKey" );
   SetCursorFirstEntityByInteger( vTempView, "OperationSource", "ZKey",
                                  lZKey, "VML_XSource" );

   GetStringFromAttribute( szType, zsizeof( szType ), vTempView, "Operation", "Type" );
   if ( szType[ 0 ] == 'L' ||
        szType[ 0 ] == 'O' ||
        szType[ 0 ] == 'T' ||
        szType[ 0 ] == 'C' ||
        szType[ 0 ] == 'E' ||
        szType[ 0 ] == 'A' )
   {
      szTempName1[ 0 ] = 'o';
      GetStringFromAttribute( szTempName1 + 1, zsizeof( szTempName1 ) - 1, vTempView,
                              "VML_XSource", "MetaName" );
      strcat_s( szTempName1, zsizeof( szTempName1 ), "_" );
      GetStringFromAttribute( szTempName2, zsizeof( szTempName2 ), vTempView, "Operation", "Name" );
      strcat_s( szTempName1, zsizeof( szTempName1 ), szTempName2 );
   }
   else
      GetStringFromAttribute( szTempName1, zsizeof( szTempName1 ), vTempView, "Operation", "Name" );


   StoreValueInRecord( vTempView, EntityStructure, AttributeStructure,
                       szTempName1, 0 );

   // Get rid of temporary view
   DropView( vTempView );

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
