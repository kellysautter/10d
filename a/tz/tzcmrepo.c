/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmrepo.c - Zeidon application operations
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
// LAST MESSAGE ID: CM00523
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
/*
CHANGE LOG

2001.06.26  Bl  Repository
   maintained Repository, part 2 ( delete function oTZCMREPO_InitBaseSampleShadow )

2000.10.24  SR  Z2000  Size of path variables
   Modified size of szMetaPath, szExecPath and szPgmPath in function
   oTZCMREPO_InitBaseSampleShadow because size of attributes ExecDir, MetaSrcDir
   and PgmSrcDir has been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

*/


#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"

//./ ADD NAME=oTZCMREPO_ActivateRepository
// Source Module=tzcmrepo.c
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  oTZCMREPO_ActivateRepository
//
//  PURPOSE:    Activates the Zeidon Repository Object Instance.
//
//  PARAMETERS: vTZCMREPO - A pointer to the return Repository view
//              vSubtask  - A qualifying view for the Activation
//
//  RETURNS:   >= 0  - TZCMREPO activated successfully
//               -1  - Error encountered during activation
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
oTZCMREPO_ActivateRepository( zPVIEW vTZCMREPO,
                              zVIEW vSubtask )
{
   zSHORT   nRC;

   MB_SetMessage( vSubtask, 1, "Activating Repository" );
   nRC = ActivateObjectInstance( vTZCMREPO, "TZCMREPO", vSubtask,
                                 0, zSINGLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "CM00501", "Configuration Management",
                   "Unable to Activate Repository from database!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   return( nRC );
}

BOOL OPERATION
oTZCMREPO_EnumFindWindow( HWND hWnd, zPCHAR szWindowTitle )
{
   zCHAR szWindowText[ 100 ];

   GetWindowText( hWnd, szWindowText, 99 );
   if ( zstrcmp( szWindowText, szWindowTitle ) == 0 )
      return( 0 );

   return( 1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      LocalSysWait
//
//  PURPOSE:    To wait while something else finishes
//
//  PARAMETERS: uMilliseconds to wait
//
//  RETURNS:    Yes, but only after waiting
//
/////////////////////////////////////////////////////////////////////////////
void
LocalSysWait( unsigned int uMilliseconds )
{
   zLONG    lStart, lMill;

   lStart = GetCurrentTime( );

   do
   {
      Yield();
      lMill = GetCurrentTime( ) - lStart;
   } while ( lMill <(zLONG) uMilliseconds );

   return;
}

//./ ADD NAME=oTZCMREPO_ObjectConstraints
// Source Module=tzcmrepo.c
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  oTZCMREPO_ObjectConstraints
//
//  PURPOSE:    Validates the Zeidon Repository Object Instance.
//
//  PARAMETERS: vTZCMREPO - A pointer to the return Repository view
//              vSubtask  - A qualifying view for the Activation
//
//  RETURNS:   >= 0  - TZCMREPO activated successfully
//               -1  - Error encountered during activation
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
oTZCMREPO_ObjectConstraints( zVIEW vTZCMREPO, zSHORT nEventType,
                             zSHORT nState )
{
   zSHORT   nRC;
   zVIEW    vTZCMREPO_Compare;
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zVIEW    vKZDBHQUA;

   if ( nEventType == zOCE_COMMIT )
   {

      // Activate a single, root-only instance of the Installation
      // entity, which has a zero ZKey.  A keyed access is made
      // because of performance.
      ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA",
                                   vTZCMREPO, zSINGLE );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );

      SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName",
                              "Installation" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName",
                              "Installation" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName",
                              "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", "0" );


      nRC = ActivateObjectInstance( &vTZCMREPO_Compare, "TZCMREPO", vTZCMREPO,
                                    vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION | zACTIVATE_ROOTONLY );
      DropObjectInstance( vKZDBHQUA );
      if ( nRC == zCALL_ERROR )
      {
         MessageSend( vTZCMREPO, "CM00516", "Configuration Management",
                      "Unable to Activate Repository from database!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
      if ( CheckExistenceOfEntity( vTZCMREPO_Compare, "Installation" ) == zCURSOR_SET )
      {
         if ( CompareAttributeToAttribute( vTZCMREPO, "Installation",
                                           "NextWKS_Id", vTZCMREPO_Compare,
                                           "Installation", "NextWKS_Id" ) == -1 )
         {
            SetAttributeFromAttribute( vTZCMREPO, "Installation",
                                       "NextWKS_Id", vTZCMREPO_Compare,
                                       "Installation", "NextWKS_Id" );
            strcpy_s( szMsg, zsizeof( szMsg ), "Unable to Commit Repository to database!" );
            strcat_s( szMsg, zsizeof( szMsg ), "\nAttempt to decrement Next Workstation Id." );
            strcat_s( szMsg, zsizeof( szMsg ), "\nNext Workstation Id set to current value on database." );
            MessageSend( vTZCMREPO, "CM00517", "Configuration Management",
                         szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -1 );
         }
      }
   }

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
