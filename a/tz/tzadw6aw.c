/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw6aw.c - Zeidon application operations
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
// LAST MESSAGE ID: AD10503
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

/*
CHANGE LOG

29.07.1998  DonC
   Eliminated unused operations.

*/

////////////////////////////////////////////////////////////////////////////
//
// ENTRY: CreateGenericWDOD
//
// PURPOSE: This function changes the name of the first window from
// the name of the WDOD to the name of the first entity in the style
// object.  This is because the first window of this type of WDOD is
// window created for the first entity in the style object.  There is
// no initial load window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateGenericDialog( zVIEW vUserSpec, zVIEW vNewDialog )
{
   // Get the name of the first entity in the style object.
   SetAttributeFromAttribute( vNewDialog, "Window", "Tag",
                              vUserSpec, "LOD_Entity", "Name" );
   IncludeSubobjectFromSubobject( vNewDialog, "DfltWnd",
                                  vNewDialog, "Window", zPOS_AFTER );
   return( 0 );
}
