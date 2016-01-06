/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmenqo.c - Zeidon application operations
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
#include "tzcm_opr.hg"
#include "ZeidonOp.H"


//./ ADD NAME=oTZCMENQO_ActivateREPO_Enqueue
// Source Module=tzcmrepo.c
////////////////////////////////////////////////
//
//  OPERATION:  oTZCMENQO_ActivateREPO_Enqueue
//
//  PURPOSE:    Checks for REPOS_Enqueue
//
//  PARAMETERS: vTZCMREPO - A pointer to the return Repository Enqueue view
//              vSubtask  - A qualifying view for the Activation
//
//  RETURNS:   >= 0  - TZCMREPO activated successfully
//               -1  - Error encountered during activation
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
oTZCMENQO_ActivateREPO_Enqueue( zPVIEW vTZCMENQO,
                                zVIEW  lpAppQualView )
{
   zVIEW    vKZDBHQUA;

   if ( GetViewByName( &vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK ) < 0 )
   {
      if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA",
                                        lpAppQualView, zSINGLE ) >= 0 )
      {
         SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
         CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
         CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      }
      else
         return( -1 );
   }

   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName",
                           "REPOS_Enqueue" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName",
                           "REPOS_Enqueue" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName",
                           "TimeStamp" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value",
                            "19000101000000000" );
   return( ActivateObjectInstance( vTZCMENQO, "TZCMENQO", lpAppQualView,
                                   vKZDBHQUA, zLEVEL_TASK ) );
}

///////////////////////////// end of file  //////////////////////////////////
