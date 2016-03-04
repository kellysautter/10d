/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzersast.c - Zeidon application operations
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
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RefreshSA_FromEMD( zVIEW  vSubjectArea,
                             zVIEW  vEntpModel )
{
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vSubjectArea, "SA_Entity", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      oTZERSASO_RefreshOneSA_Entity( vSubjectArea, vEntpModel );
      nRC = SetCursorNextEntity( vSubjectArea, "SA_Entity", "" );
   }
}

zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RefreshOneSA_Entity( zVIEW    vSubjectArea,
                               zVIEW    vEntpModel )
{
   zVIEW    vEntpModel_Wrk;
   zLONG    lEntityZKey;
   zSHORT   nRC;

   CreateViewFromViewForTask( &vEntpModel_Wrk, vEntpModel, 0 );
   GetIntegerFromAttribute( &lEntityZKey, vSubjectArea, "ER_Entity", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vEntpModel_Wrk, "ER_Entity",
                                        "ZKey", lEntityZKey, "" );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      ExcludeEntity( vSubjectArea, "ER_Entity" );
//    SetCursorPrevEntity( vSubjectArea, "SA_Entity", "" );
      IncludeSubobjectFromSubobject( vSubjectArea, "ER_Entity", vEntpModel_Wrk, "ER_Entity", zPOS_AFTER );
      nRC = 0;
   }
   else
   {
      DeleteEntity( vSubjectArea, "SA_Entity" );
      nRC = 1;
   }

   DropView( vEntpModel_Wrk );
   return( nRC );
}
