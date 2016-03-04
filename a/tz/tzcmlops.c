/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmlops.c - Configuration management LOD Operations for
//                             managing all tool objects are current
//                             and linked when brought into memory
// DESCRIPTION:   This is the source file which contains the logical object
//                operations for managing the activation of all Zeidon
//                tool objects and ensuring all linkable data is linked
//                and current.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/12/02
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: CM00904
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2001.03.01  BL
   Fix for Windows 98; change TaskID > 0 to TaskID (Win9x has negative IDs)

2000.01.08  BL  Z10  Repositioy
   Modified Operation cfTZEREMDO_PostCheckout for set Cursor Position

from here old version

26.05.1997  DonC
   Corrected cfTZZOLODO_PostCheckOut for new caching.

01.08.1997  DonC
   Modified cfTZERSASO_PostCheckout so that including ER_RelLink, etc.
   entities is not done for Shadow Objects.  Also removed an erroneous
   if statement that skipped some necessary includes.

28.02.1998  DonC
   Modified cfTZWDVORO_PostActivate to re-include the LOD if the LOD ZKey
   had changed, meaning the LOD had been deleted and recreated.

1998/09/29  DC
   Modified cfTZEREMDO_PostActivate to include TextForMissingZKey Domain
   if a relink error occurs and to provide more information when the
   problem occurs.

1998/02/03  DC
   Modified cfTZWDVORO_PostActivate to add back relinking of LOD into
   VOR. Landesbank had problems checking in Dialogs because the LOD entity
   had created flags set, which was the result of no relinking.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"

zCHAR   szERR_Msg[ 256 ];

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfPostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
cfPostCheckout( zSHORT   nType,
                zVIEW    vZeidonCM,
                zVIEW    vMOI_View,
                zVIEW    vTaskLPLR,
                zBOOL    bRepository )
{
   zSHORT nRC;

   switch ( nType )
   {
      case  zSOURCE_LOD_META:
      case  zREFER_LOD_META:
         nRC = cfTZZOLODO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_DIALOG_META:
      case  zREFER_DIALOG_META:
         nRC = cfTZWDLGSO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_ERD_META:
      case  zREFER_ERD_META:
         nRC = cfTZEREMDO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_DTE_META:
      case  zREFER_DTE_META:
         nRC = cfTZTENVRO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_SA_META:
      case  zREFER_SA_META:
         nRC = cfTZERSASO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_UIS_META:
      case  zREFER_UIS_META:
         nRC = cfTZADCSDO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_VOR_META:
      case  zREFER_VOR_META:
         nRC = cfTZWDVORO_PostCheckout( vZeidonCM, vMOI_View,  vTaskLPLR, bRepository );
         break;

      case  zSOURCE_PENV_META:
      case  zREFER_PENV_META:
         nRC = cfTZPESRCO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      case  zSOURCE_REPORT_META:
      case  zREFER_REPORT_META:
         nRC = cfTZRPSRCO_PostCheckout( vZeidonCM, vMOI_View, vTaskLPLR, bRepository );
         break;

      default:
         nRC = 0;
         break;
   }

   return( nRC );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfPostActivate
//
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfPostActivate( zVIEW    vSubtask,
                zSHORT   nType,
                zPCHAR   szSyncDate,
                zVIEW    vZeidonCM,
                zVIEW    vMOI_View,
                zVIEW    vTaskLPLR,
                zSHORT   PhaseCtl,
                zBOOL    bCheckin )
{
   zSHORT nRC;

   switch ( nType )
   {
      case  zSOURCE_LOD_META:
      case  zREFER_LOD_META:
         nRC = cfTZZOLODO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_DIALOG_META:
      case  zREFER_DIALOG_META:
         nRC = cfTZWDLGSO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_ERD_META:
      case  zREFER_ERD_META:
         nRC = cfTZEREMDO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR,  PhaseCtl, bCheckin );
         break;

      case  zSOURCE_DTE_META:
      case  zREFER_DTE_META:
         nRC = cfTZTENVRO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_SA_META:
      case  zREFER_SA_META:
         nRC = cfTZERSASO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_UIS_META:
      case  zREFER_UIS_META:
         nRC = cfTZADCSDO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_VOR_META:
      case  zREFER_VOR_META:
         nRC = cfTZWDVORO_PostActivate( vSubtask, szSyncDate, vZeidonCM,  vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_PENV_META:
      case  zREFER_PENV_META:
         nRC = cfTZPESRCO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      case  zSOURCE_REPORT_META:
      case  zREFER_REPORT_META:
         nRC = cfTZRPSRCO_PostActivate( vSubtask, szSyncDate, vZeidonCM, vMOI_View, vTaskLPLR, PhaseCtl, bCheckin );
         break;

      default:
         nRC = 0;
         break;
   }

   return( nRC );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZOPSIGO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZOPSIGO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZOPSIGO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin )
{
   zCHAR    szWork[ 34 ];
   zVIEW    vActiveMetas;
   zVIEW    vWkTZCMLPLO;

   // The Operation Signature has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////

/* Bypass Phase 2 if this is an activation of a SOURCE Type Meta */
   if ( PhaseCtl == 1 )
   {
      /////////////////////////////////////////////
      // Post Activate PHASE 2: Link up to all object instances in memory
      // which depend on us
      /////////////////////////////////////////////
   }

   /////////////////////////////////////////////
   // Post Activate PHASE 3: For all meta types we depend on, issue an
   // activate for all instances whose synchronization date is later than
   // our synchronization date.
   /////////////////////////////////////////////

   // For each type which has a syncronization date later than our own,
   // issue an activate for that lod
   CreateViewFromViewForTask( &vWkTZCMLPLO, vTZCMLPLO, 0 );
   while ( ResetViewFromSubobject( vWkTZCMLPLO ) == 0 )
      ;

   GetStringFromAttribute( szWork, zsizeof( szWork ), vTZCMLPLO, "W_MetaDef", "LastSyncDate" );
   DropView( vWkTZCMLPLO );
   // return success
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZEREMDO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZEREMDO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZEREMDO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // if the object instance just checked out has come from the repository. Re-establish the derived paths
   // used by the ER/Model dialog
   if ( bRepository )
   {
      // For Every RelType_1 found in the object instance, find the RelLink to the Other Entity and include
      // it under the RelType_1 Entity
      if ( SetCursorFirstEntity( vTZEREMDO, "ER_RelType_1",
                                 "EntpER_Model" ) >= zCURSOR_SET )
      {
         do
         {
            if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink_Other" ) < zCURSOR_SET )
            {
               //BL, 2000.01.08 This Code does not work
//             if ( SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink_2", vTZEREMDO, "ER_RelLink", "EntpER_Model" ) >= zCURSOR_SET )
               if ( SetCursorFirstEntityByAttr( vTZEREMDO, "ER_RelLink_2", "ZKey",
                                                vTZEREMDO, "ER_RelLink", "ZKey",
                                                "EntpER_Model" ) >= zCURSOR_SET )
               {
                  if ( SetCursorNextEntity( vTZEREMDO, "ER_RelLink_2", 0 ) != zCURSOR_SET )
                  {
                     SetCursorPrevEntity( vTZEREMDO, "ER_RelLink_2", 0 );
                  }

                  IncludeSubobjectFromSubobject( vTZEREMDO, "ER_RelLink_Other", vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
               }
            }
         }  while( SetCursorNextEntity( vTZEREMDO, "ER_RelType_1", "EntpER_Model" ) >= zCURSOR_SET );
      }
   }

   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZEREMDO_PostActivate
//
//  PURPOSE:   This function post processes the activate of an incremental
//             E/R model incremental object instance. It is intended to
//             be called by configuration management AFTER the file is
//             brought into memory AND the configuration management
//             name is created for it in the configuration management
//             subtask.
//
//             This function links up to any memory active
//             Domains, and LODS.
//
//             Additionally, if the ER/MODEL incremental file's last
//             refresh date is less than the LPLR's last refresh date,
//             then all domains whose refresh date
//             is later than the ER/MODEL's date are activated. Note that
//             simply activating those files will insure their linking
//             to this object instance.
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZEREMDO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZEREMDO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;
   zVIEW    vTZDMSRCO;
   zVIEW    vDomainLPLR;
   zLONG    lZKey;
   zSHORT   nRC;
   zCHAR    szDomainName[ 33 ];
   zCHAR    szTempName[ 33 ];
   zCHAR    szMsg[ 66 ];
   zBOOL    bRelinkError;

   // The E/R Model has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

// Modification made by Don Christensen, 2/10/95
// This modification assumes the forced top-down relink.  It forces
// a relink to all Domains and deletes any ER_Attribute entities that
// reference a non-existent Domain.  It then skips the rest of the
// normal relink processing.
// This code has been enhanced to re-include a Domain by the same name as
// the original and to use the Text Domain if such a Domain could not be found.

   bRelinkError = FALSE;
   CreateViewFromViewForTask( &vDomainLPLR, vTZCMLPLO, 0 );
   SetCursorFirstEntityByInteger( vDomainLPLR, "W_MetaType", "Type", zREFER_DOMAIN_META, 0 );
   CreateViewFromViewForTask( &vTZEREMDO, vTZEREMDO, 0 );
   for ( nRC = SetCursorFirstEntity( vTZEREMDO, "Domain", "EntpER_Model" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZEREMDO, "Domain", "EntpER_Model" ) )
   {

      GetIntegerFromAttribute( &lZKey, vTZEREMDO, "Domain", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( vSubtask, &vTZDMSRCO, 0, zREFER_DOMAIN_META, zSINGLE, lZKey, 0 );
      if ( nRC >= 0 )
      {
         SetNameForView( vTZEREMDO, "DKS_TZEREMDO", vSubtask, zLEVEL_TASK );
         SetNameForView( vTZDMSRCO, "DKS_TZDMSRCO", vSubtask, zLEVEL_TASK );
         RelinkInstanceToInstance( vTZEREMDO, "Domain", vTZDMSRCO, "Domain" );
         DropMetaOI( vSubtask, vTZDMSRCO );
      }
      else
      {
         GetStringFromAttribute( szDomainName, zsizeof( szDomainName ), vTZEREMDO, "Domain", "Name" );
         nRC = ActivateMetaOI_ByName( vSubtask, &vTZDMSRCO, 0, zREFER_DOMAIN_META, zSINGLE, szDomainName, 0 );
         if ( nRC < 0 )
         {
            bRelinkError = TRUE;
            TraceLineI( "Can't relink Domain. ZKey: ", lZKey );
            TraceLineS( "Can't relink Domain. Name: ", szDomainName );
            GetStringFromAttribute( szTempName, zsizeof( szTempName ), vTZEREMDO, "ER_Entity", "Name" );
            TraceLineS( "Can't relink Domain. Entity: ", szTempName );
            GetStringFromAttribute( szTempName, zsizeof( szTempName ), vTZEREMDO, "ER_Attribute", "Name" );
            TraceLineS( "Can't relink Domain. Attribute: ", szTempName );
            nRC = SetCursorFirstEntityByString( vDomainLPLR, "W_MetaDef", "Name", "TextForMissingZKey", 0 );
            if ( nRC >= 0 )
               ActivateMetaOI_ByName( vSubtask, &vTZDMSRCO, 0, zREFER_DOMAIN_META, zSINGLE, "TextForMissingZKey", 0 );
         }

         if ( nRC < 0 )
         {
            nRC = ActivateMetaOI_ByName( vSubtask, &vTZDMSRCO, 0, zREFER_DOMAIN_META, zSINGLE, "Text", 0 );
         }

         if ( nRC >= 0 )
         {
            ExcludeEntity( vTZEREMDO, "Domain", zREPOS_NONE );
            IncludeSubobjectFromSubobject( vTZEREMDO, "Domain", vTZDMSRCO, "Domain", zPOS_AFTER );
            DropMetaOI( vSubtask, vTZDMSRCO );
         }
         else
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "Deleting Attribute because of missing Domain: " );
            strcat_s( szMsg, zsizeof( szMsg ), szDomainName );
            MessageSend( vSubtask, "CM00901", "Configuration Management",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            DeleteEntity( vTZEREMDO, "ER_Attribute", zREPOS_NONE );
         }
      }
   }

   DropView( vTZEREMDO );
   if ( bRelinkError )
   {
      MessageSend( vSubtask, "CM00904", "Configuration Management",
                   "Error in relinking Domains. See trace for more information.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZERSASO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZERSASO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZERSASO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   zVIEW vSA2;

   if ( bRepository )
   {
      // For every ER_RelType in the subject area, instantiate the ER_RelLink
      // under both the source and target entity.
      if ( SetCursorFirstEntity( vTZERSASO, "ER_RelType", "SubjectArea" ) >= zCURSOR_SET )
      {
         CreateViewFromViewForTask( &vSA2, vTZERSASO, 0 );
         do
         {
               SetViewFromView( vSA2, vTZERSASO );
               if ( SetCursorNextEntity( vSA2, "ER_RelLink_2", 0 ) >= zCURSOR_SET &&
                    SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_Entity", vTZERSASO, "ER_Entity_2", "SubjectArea" ) >= zCURSOR_SET &&
                    SetCursorFirstEntityByEntityCsr( vSA2, "ER_Entity", vSA2, "ER_Entity_2", "SubjectArea" ) >= zCURSOR_SET )
               {
                  // We only need to do the following includes if we just activated the SA directly from the database and not
                  // from shadow objects.  If we activated from shadow objects, then the relationships have already been created.
                  if ( SetCursorFirstEntityByAttr( vTZERSASO, "ER_RelLink",   "ZKey",
                                                   vTZERSASO, "ER_RelLink_2", "ZKey", 0 ) < zCURSOR_SET )
                  {
                     SetCursorLastEntity( vTZERSASO, "ER_RelLink", 0 );
                     IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelLink", vTZERSASO, "ER_RelLink_2", zPOS_LAST );
                     IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelLink_Other",vSA2, "ER_RelLink_2", zPOS_FIRST );
                     IncludeSubobjectFromSubobject( vSA2, "ER_RelLink", vSA2, "ER_RelLink_2", zPOS_LAST );
                     IncludeSubobjectFromSubobject( vSA2, "ER_RelLink_Other", vTZERSASO, "ER_RelLink_2", zPOS_FIRST );
                  }
               }
         }  while ( SetCursorNextEntity( vTZERSASO, "ER_RelType", "SubjectArea" ) >= zCURSOR_SET );
         DropView( vSA2 );
      }
   }
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZERSASO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZERSASO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZERSASO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zCHAR    szWork[ 34 ];
   zVIEW    vActiveMetas;
   zVIEW    vMeta;
   zVIEW    vWkTZCMLPLO;
   zSHORT   nRelNbr;
   zBOOL    bERD;

   // The SA has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////

   // TE_RelinkDelete deletes components that reference ER_Entities that
   // no longer exist.  It does not currently do any relinking.
   oTZERSASO_SA_RelinkDelete( vTZERSASO, vSubtask );

   // For each ERD active in memory, relink the Subject Area to the model
   nRelNbr = 0;
   bERD = 0;
   if ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas,
                                          zSOURCE_ERD_META, nRelNbr )) > 0 )
   {
      if ( vMeta )
      {
         // We have retrieved a view to a refer er, link up the
         // er to the sa
         fnLinkAndSyncER_ToSA( vSubtask, vMeta, vTZERSASO );
         bERD = 1;
      }
   }
   else
   if ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas,
                                          zREFER_ERD_META, nRelNbr )) > 0 )
   {
      if ( vMeta )
      {
         // We have retrieved a view to a refer er, link up the
         // er to the sa
         fnLinkAndSyncER_ToSA( vSubtask, vMeta, vTZERSASO );
      }
   }

/* Bypass Phase 2 if this is an activation of a SOURCE Type Meta */

   /////////////////////////////////////////////
   // Post Activate PHASE 2: Link up to all object instances in memory
   // which depend on us
   /////////////////////////////////////////////
   // Nothing to DO!

   /////////////////////////////////////////////
   // Post Activate PHASE 3: For all meta types we depend on, issue an
   // activate for all instances whose synchronization date is later than
   // our synchronization date.
   /////////////////////////////////////////////

   // For each type which has a syncronization date later than our own,
   // issue an activate for that type
   CreateViewFromViewForTask( &vWkTZCMLPLO, vTZCMLPLO, 0 );
   while ( ResetViewFromSubobject( vWkTZCMLPLO ) == 0 )
      ;

   GetStringFromAttribute( szWork, zsizeof( szWork ), vTZCMLPLO, "W_MetaDef", "LastSyncDate" );

   // FORCE THE ER/MODEL IN REGARDLESS OF THE SYNC DATE
   if ( bERD == FALSE )
   {
      fnActivateDependentBySyncDate( vCM_Subtask, szReturnSyncDate, vWkTZCMLPLO,
                                     szWork, zREFER_ERD_META, 1 );
   }

   DropView( vWkTZCMLPLO );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZTENVRO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZTENVRO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZTENVRO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZTENVRO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZTENVRO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZTENVRO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zCHAR    szWork[ 34 ];
   zVIEW    vActiveMetas;
   zVIEW    vMeta;
   zVIEW    vWkTZCMLPLO;
   zSHORT   nRelNbr;

   // This starts the standard processing for all Metas
   GetViewByName( &vActiveMetas, "OpenCM_Metas", vCM_Subtask, zLEVEL_SUBTASK );

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////
   // For each ERD active in memory, relink the TE to the ER_MODEL

   oTZTENVRO_TE_RelinkDelete( vTZTENVRO, vSubtask );
   return( 0 );

   nRelNbr = 0;
   while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas,
                                             zREFER_ERD_META, nRelNbr )) > 0 )
   {
      if ( vMeta )
      {
         // We have retrieved a view to an active ERD, link up the
         // ER_Model to the TE
         RelinkAllSubobjectsForOI( vTZTENVRO, "ER_Attribute", vMeta, "ER_Attribute" );
         RelinkAllSubobjectsForOI( vTZTENVRO, "ER_RelLink", vMeta, "ER_RelLink" );
         RelinkAllSubobjectsForOI( vTZTENVRO, "ER_Entity", vMeta, "ER_Entity" );
         RelinkAllSubobjectsForOI( vTZTENVRO, "ER_RelType", vMeta, "ER_RelType" );
         RelinkAllSubobjectsForOI( vTZTENVRO, "ER_EntIdentifier", vMeta, "ER_EntIdentifier" );
      }
   }

/* Bypass Phase 2 if this is an activation of a SOURCE Type Meta */
   if ( nPhaseCtl == 1 )
   {
   /////////////////////////////////////////////
   // Post Activate PHASE 2: Link up to all object instances in memory
   // which depend on us
   /////////////////////////////////////////////
      // For each REFER LOD active in memory, relink the TE to the LOD
      nRelNbr = 0;
      while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas, zREFER_LOD_META, nRelNbr )) > 0 )
      {
         if ( vMeta )
         {
            // We have retrieved a view to an active LOD, link up the LOD to the TE
            RelinkAllSubobjectsForOI( vMeta, "TE_DB_Environ", vTZTENVRO, "TE_DB_Environ" );
         }
      }

      // For each SOURCE LOD active in memory, relink the TE to the LOD
      nRelNbr = 0;
      while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas, zSOURCE_LOD_META, nRelNbr )) > 0 )
      {
         if ( vMeta )
         {
            // We have retrieved a view to an active LOD, link up the
            // LOD to the TE
            RelinkAllSubobjectsForOI( vMeta, "TE_DB_Environ", vTZTENVRO, "TE_DB_Environ" );
         }
      }
   }

   /////////////////////////////////////////////
   // Post Activate PHASE 3: For all meta types we depend on, issue an
   // activate for all instances whose synchronization date is later than
   // our synchronization date.
   /////////////////////////////////////////////

   // For each type which has a syncronization date later than our own,
   // issue an activate for that type
   CreateViewFromViewForTask( &vWkTZCMLPLO, vTZCMLPLO, 0 );
   while ( ResetViewFromSubobject( vWkTZCMLPLO ) == 0 );
   GetStringFromAttribute( szWork, zsizeof( szWork ), vTZCMLPLO, "W_MetaDef", "LastSyncDate" );
   fnActivateDependentBySyncDate( vCM_Subtask, szReturnSyncDate, vWkTZCMLPLO,
                                  szWork, zREFER_ERD_META, 1 );

   DropView( vWkTZCMLPLO );
   // Return success
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZZOLODO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZZOLODO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZZOLODO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // After a lod has been checked out, we need to instantiate
   // the right 'flat' side of the lod from the left 'recursive'
   // side.  We will only do this, however, if it doesn't already exist.
   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Entity" ) < zCURSOR_SET )
   {
      fnTZZOLODO_BuildFlatFromRec( vTZZOLODO );
   }

   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZZOLODO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZZOLODO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZZOLODO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;

   // The LOD has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////
   // For each ERD active in memory, relink the ER_MODEL to the LOD

   oTZZOLODO_LOD_RelinkDelete( vTZZOLODO, vSubtask );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZWDVORO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZWDVORO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZWDVORO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZWDVORO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZWDVORO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZWDVORO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   // This function verifies that the ZKey of the LOD hasn't changed. If
   // it has, then the LOD was deleted and recreated and we need to
   // re-include the LOD within the VOR.

   // This routine used to relink with the corresponding LOD, but this code
   // was removed for performance reasons. The relink wasn't necessary
   // because the only information in the LOD was the LOD Name, which could
   // not be changed. (Relink code added back by DonC on 1999.02.03 because
   // we got incorrect update flags without it.

   zVIEW  vLOD;
   zVIEW  vTempLPLR;
   zSHORT nRC;
   zCHAR  szLOD_Name[ 33 ];

   CreateViewFromViewForTask( &vTempLPLR, vTZCMLPLO, 0 );
   SetCursorFirstEntityByInteger( vTempLPLR, "W_MetaType", "Type", zREFER_LOD_META, 0 );
   nRC = SetCursorFirstEntityByAttr( vTempLPLR, "W_MetaDef", "CPLR_ZKey", vTZWDVORO, "LOD", "ZKey", 0 );
   if ( nRC < zCURSOR_SET )
   {
      // The LOD in the VOR was not found in the LPLR by ZKey, so re-include.
      GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), vTZWDVORO, "LOD", "Name" );
      nRC = ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META,
                                   zSINGLE, szLOD_Name, 0 );
      if ( nRC >= 0 )
      {
         ExcludeEntity( vTZWDVORO, "LOD", zREPOS_NONE );
         IncludeSubobjectFromSubobject( vTZWDVORO, "LOD", vLOD, "LOD", zPOS_AFTER );
         DropMetaOI( vSubtask, vLOD );
      }
   }
   else
   {
      // Activate the LOD and relink it.
      GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), vTZWDVORO, "LOD", "Name" );
      nRC = ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META,
                                   zSINGLE, szLOD_Name, 0 );
      if ( nRC >= 0 )
      {
         RelinkInstanceToInstance( vTZWDVORO, "LOD", vLOD, "LOD" );
         DropMetaOI( vSubtask, vLOD );
      }
   }

   DropView( vTempLPLR );

   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZPESRCO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZPESRCO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZPESRCO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZPESRCO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;
   zVIEW    vMeta;
   zSHORT   nRelNbr;

   // The Penv has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////

/* Bypass Phase 2 if this is an activation of a SOURCE Type Meta */
   if ( nPhaseCtl == 1 )
   {
   /////////////////////////////////////////////
   // Post Activate PHASE 2: Link up to all object instances in memory
   // which depend on us
   /////////////////////////////////////////////
   // For each REFER Dialog active in memory, relink the PE to the Dialog
      nRelNbr = 0;
      while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask,
                                                vActiveMetas,
                                                zREFER_DIALOG_META,
                                                nRelNbr )) > 0 )
      {
         if ( vMeta )
         {
            // We have retrieved a view to an active DIALOG, link up the
            // Dialog to the PE
            RelinkAllSubobjectsForOI( vMeta, "ControlDef", vTZPESRCO, "ControlDef" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlColorDef", vTZPESRCO, "CtrlColorDef" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlColor", vTZPESRCO, "Color" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlFontDef", vTZPESRCO, "CtrlFontDef" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlFont", vTZPESRCO, "Font" );
            RelinkAllSubobjectsForOI( vMeta, "WndStyle", vTZPESRCO, "WindowStyle" );
         // RelinkAllSubobjectsForOI( vMeta, "WndColorDef", vTZPESRCO, "WndColorDef" );
         // RelinkAllSubobjectsForOI( vMeta, "WndColor",  vTZPESRCO, "Color" );
         // RelinkAllSubobjectsForOI( vMeta, "WndFontDef", vTZPESRCO, "WndFontDef" );
         // RelinkAllSubobjectsForOI( vMeta, "WndFont", vTZPESRCO, "Font" );
         }
      }

   // For each SOURCE Dialog active in memory, relink the PE to the Dialog
      nRelNbr = 0;
      while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask,
                                                vActiveMetas,
                                                zSOURCE_DIALOG_META,
                                                nRelNbr )) > 0 )
      {
         if ( vMeta )
         {
            // We have retrieved a view to an active DIALOG, link up the Dialog to the PE
            RelinkAllSubobjectsForOI( vMeta, "ControlDef", vTZPESRCO, "ControlDef" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlColorDef", vTZPESRCO, "CtrlColorDef" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlColor", vTZPESRCO, "Color" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlFontDef", vTZPESRCO, "CtrlFontDef" );
         // RelinkAllSubobjectsForOI( vMeta, "CtrlFont", vTZPESRCO, "Font" );
            RelinkAllSubobjectsForOI( vMeta, "WndStyle", vTZPESRCO, "WindowStyle" );
         // RelinkAllSubobjectsForOI( vMeta, "WndColorDef", vTZPESRCO, "WndColorDef" );
         // RelinkAllSubobjectsForOI( vMeta, "WndColor", vTZPESRCO, "Color" );
         // RelinkAllSubobjectsForOI( vMeta, "WndFontDef", vTZPESRCO, "WndFontDef" );
         // RelinkAllSubobjectsForOI( vMeta, "WndFont", vTZPESRCO, "Font" );
         }
      }

      // For each REFER UI Spec active in memory, relink the PE to the UI Spec
      nRelNbr = 0;
      while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas, zREFER_UIS_META, nRelNbr )) > 0 )
      {
         if ( vMeta )
         {
            // We have retrieved a view to an active UI Spec, link up the
            // UI Spec to the PE
            RelinkAllSubobjectsForOI( vMeta, "WndStyle", vTZPESRCO, "WindowStyle" );
            RelinkAllSubobjectsForOI( vMeta, "DfltWndStyle", vTZPESRCO, "WindowStyle" );
         }
      }

      // For each SOURCE UI Spec active in memory, relink the PE to the UI Spec
      nRelNbr = 0;
      while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas, zSOURCE_UIS_META, nRelNbr )) > 0 )
      {
         if ( vMeta )
         {
            // We have retrieved a view to an active UI Spec, link up the
            // UI Spec to the PE
            RelinkAllSubobjectsForOI( vMeta, "WndStyle", vTZPESRCO, "WindowStyle" );
            RelinkAllSubobjectsForOI( vMeta, "DfltWndStyle", vTZPESRCO, "WindowStyle" );
         }
      }
   }

   /////////////////////////////////////////////
   // Post Activate PHASE 3: For all meta types we depend on, issue an
   // activate for all instances used by this PE
   /////////////////////////////////////////////
   // Phase III - Nothing to do...

   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZWDLGSO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZWDLGSO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZWDLGSO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZWDLGSO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZWDLGSO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZWDLGSO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;

   // The Dialog has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////

   oTZWDLGSO_DialogRelinkDelete( vTZWDLGSO, vSubtask );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZRPSRCO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZRPSRCO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZWDLGSO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZRPSRCO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZRPSRCO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZRPSRCO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;

   // The Report has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////

   oTZRPSRCO_ReportRelinkDelete( vTZRPSRCO, vSubtask );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZXSLTSO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZXSLTSO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZXSLTSO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZXSLTSO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZXSLTSO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZXSLTSO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;

   // The Report has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////

// oTZXSLTSO_XSLT_RelinkDelete( vTZXSLTSO, vSubtask );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZADCSDO_PostCheckout
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZADCSDO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZADCSDO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository )
{
   // Nothing to do
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  cfTZADCSDO_PostActivate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
cfTZADCSDO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   szReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZADCSDO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   nPhaseCtl,
                         zBOOL    bCheckin )
{
   zVIEW    vActiveMetas;
   zVIEW    vMeta;
   zVIEW    vWkTZADCSDO;
   zVIEW    vTZWDVORO;
   zVIEW    vCM_List;
   zPCHAR   szName;
   zLONG    lZKey;
   zSHORT   nRelNbr;
   zSHORT   nRC;

   // The UI Spec has been brought into memory, now go through all instances
   // already in memory on which it is dependent and link up to those
   // instances.  Also, go through all instance which are dependent on it
   // and link up to those instances.
   GetViewByName( &vActiveMetas, "OpenCM_Metas",  // why in the world
                  vCM_Subtask, zLEVEL_SUBTASK );  // are we doing this?
                                                  // dks ... 2004.09.16

   /////////////////////////////////////////////
   // Post Activate PHASE 1: Link up to all object instances in memory
   // which we depend on
   /////////////////////////////////////////////
   // For each LOD active in memory, relink the UI Spec to the LOD

   oTZADCSDO_DS_RelinkDelete( vTZADCSDO, vSubtask );
   return( 0 );

   nRelNbr = 0;
   while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas,
                                             zREFER_LOD_META, nRelNbr )) > 0 )
   {
      if ( vMeta )
      {
         // We have retrieved a view to an active LOD, link up the UI Spec to the LOD
         RelinkAllSubobjectsForOI( vTZADCSDO, "LOD_Entity", vMeta, "LOD_Entity" );
         RelinkAllSubobjectsForOI( vTZADCSDO, "M_LOD_Attribute", vMeta, "LOD_Attribute" );
         RelinkAllSubobjectsForOI( vTZADCSDO, "L_LOD_Attribute", vMeta, "LOD_Attribute" );
      }
   }

   // For each VOR active in memory, relink the UI Spec to the VOR
   nRelNbr = 0;
   while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas, zREFER_VOR_META, nRelNbr ) ) > 0 )
   {
      if ( vMeta )
      {
         // We have retrieved a view to an active VOR, link up the
         // UI Spec to the VOR
         RelinkAllSubobjectsForOI( vTZADCSDO, "SubObjViewObjRef", vMeta, "ViewObjRef" );
         RelinkAllSubobjectsForOI( vTZADCSDO, "UIS_ViewObjRef", vMeta, "ViewObjRef" );
         RelinkAllSubobjectsForOI( vTZADCSDO, "ListVOR", vMeta, "ViewObjRef" );
      }
   }

   // For each PE active in memory, relink the PE to the UIS
   nRelNbr = 0;
   while ( (nRelNbr = fnGetActiveMetaByType( &vMeta, vCM_Subtask, vActiveMetas, zREFER_PENV_META, nRelNbr )) > 0 )
   {
      if ( vMeta )
      {
         // We have retrieved a view to an active View Object Ref,
         // link up the Dialog to the View Object Ref
         RelinkAllSubobjectsForOI( vTZADCSDO, "WndStyle", vMeta, "WindowStyle" );
         RelinkAllSubobjectsForOI( vTZADCSDO, "DfltWndStyle", vMeta, "WindowStyle" );
      }
   }

/* Bypass Phase 2 if this is an activation of a SOURCE Type Meta */
   /////////////////////////////////////////////
   // Post Activate PHASE 2: Link up to all object instances in memory
   // which depend on us
   /////////////////////////////////////////////

   /////////////////////////////////////////////
   // Post Activate PHASE 3: For all meta types we depend on, issue an
   // activate for all VOR instances used by this UIS
   /////////////////////////////////////////////

   // For each VOR used by this UIS activate it and inturn activate
   // the corresponding LOD
   if ( (nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_VOR_META )) < 0 )
   {
      return( 0 );
   }

   CreateViewFromViewForTask( &vWkTZADCSDO, vTZADCSDO, 0 );
   nRC = SetCursorFirstEntity( vWkTZADCSDO, "UIS_ViewObjRef", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vWkTZADCSDO, "UIS_ViewObjRef", "ZKey" );
      if ( ActivateMetaOI_ByZKey( vSubtask, &vTZWDVORO, vCM_List, zREFER_VOR_META,
                                  zSINGLE | zLEVEL_APPLICATION, lZKey, zCURRENT_OI ) == 1 )
      {
         DropView( vTZWDVORO );
      }
      else
      {
         strcpy_s( szERR_Msg, zsizeof( szERR_Msg ), "Unable to activate Registered View ( file " );
         zltoxa( lZKey, szERR_Msg + zstrlen( szERR_Msg ) );
         GetAddrForAttribute( &szName, vTZWDVORO, "ViewObjRef", "Name" );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), "' for UI Specification '" );
         GetAddrForAttribute( &szName, vWkTZADCSDO, "UI_Spec", "Name" );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), szName );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), "'.\n\nA rebuild of the Meta List may be needed." );
         MessageSend( vSubtask, "CM00903", "Configuration Management",
                      szERR_Msg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      nRC = SetCursorNextEntity( vWkTZADCSDO, "UIS_ViewObjRef", "" );
   }

   DropView( vCM_List );
   DropView( vWkTZADCSDO );
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  fnGetActiveMetaByType
//
//  PURPOSE:   This function is used by PHASES 1 and 2 of PostActivate to
//             return views to active instances in memory
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
fnGetActiveMetaByType( zPVIEW   vMeta,
                       zVIEW    vCM_Subtask,
                       zVIEW    vActiveMetas,
                       zSHORT   nType,
                       zSHORT   nRelNbr )
{
   zCHAR  szViewName[ 80 ];
   zSHORT nCursor;
   zLONG  lTaskID;

   // if nRelNbr is 0, position on the first active instance of the type
   // requested
   // If we've positioned on an instance which has at one time been active
   // in memory attempt to retrieve the activated view to the instance
   if ( nRelNbr == 0 )
   {
      if ( nType <= zSOURCE_MAX_META )
      {
         nCursor = SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaType", "Type", nType, 0 );
         if ( nCursor == zCURSOR_SET )
         {
            nCursor = SetCursorFirstEntity( vActiveMetas, "W_MetaDef", 0 );
            while ( nCursor == zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lTaskID, vActiveMetas, "W_MetaDef", "TaskID" );
               if ( lTaskID ) // any non-zero task id is valid (Win9x has negative IDs)
               {  // get the active view name for the meta in memory
                  GetStringFromAttribute( szViewName, zsizeof( szViewName ), vActiveMetas, "W_MetaDef", "CM_ViewName" );
                  strcat_s( szViewName, zsizeof( szViewName ), ".u" );
                  GetViewByName( vMeta, szViewName, vCM_Subtask, zLEVEL_SUBTASK );
                  return( ++nRelNbr );
               }

               nCursor = SetCursorNextEntity( vActiveMetas, "W_MetaDef", 0 );
            }
         }
      }
      else
      {
         nCursor = SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaType",
                                                  "Type", nType - 2000, 0 );
         if ( nCursor == zCURSOR_SET )
         {
            nCursor = SetCursorFirstEntity( vActiveMetas, "W_MetaDef", 0 );
            while ( nCursor == zCURSOR_SET )
            {
               // get the active view name for the meta in memory
               GetStringFromAttribute( szViewName, zsizeof( szViewName ), vActiveMetas, "W_MetaDef", "CM_ViewName" );
               strcat_s( szViewName, zsizeof( szViewName ), ".r" );
               if ( GetViewByName( vMeta, szViewName, vCM_Subtask, zLEVEL_SUBTASK ) >= 0 )
               {
                  return( ++nRelNbr );
               }

               nCursor = SetCursorNextEntity( vActiveMetas, "W_MetaDef", 0 );
            }
         }
      }
   }
   else
   {
      nCursor = SetCursorNextEntity( vActiveMetas, "W_MetaDef", 0 );
      if ( nType <= zSOURCE_MAX_META )
      {
         while ( nCursor == zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lTaskID, vActiveMetas, "W_MetaDef", "TaskID" );
            if ( lTaskID ) // any non-zero task id is valid (Win9x has negative IDs)
            {  // get the active view name for the meta in memory
               GetStringFromAttribute( szViewName, zsizeof( szViewName ), vActiveMetas, "W_MetaDef", "CM_ViewName" );
               strcat_s( szViewName, zsizeof( szViewName ), ".u" );
               GetViewByName( vMeta, szViewName, vCM_Subtask, zLEVEL_SUBTASK );
               return( ++nRelNbr );
            }

            nCursor = SetCursorNextEntity( vActiveMetas, "W_MetaDef", 0 );
         }
      }
      else
      {
         while ( nCursor == zCURSOR_SET )
         {
            // get the active view name for the meta in memory
            GetStringFromAttribute( szViewName, zsizeof( szViewName ), vActiveMetas, "W_MetaDef", "CM_ViewName" );
            strcat_s( szViewName, zsizeof( szViewName ), ".r" );
            if ( GetViewByName( vMeta, szViewName, vCM_Subtask, zLEVEL_SUBTASK ) >= 0 )
            {
               return( ++nRelNbr );
            }

            nCursor = SetCursorNextEntity( vActiveMetas, "W_MetaDef", 0 );
         }
      }
   }

   // Return 0, there are no more
   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//
//     fnActivateDependentBySyncDate
//
//////////////////////////////////////////////////////////////////////////////
zSHORT
fnActivateDependentBySyncDate( zVIEW   vSubtask,
                               zPCHAR  szReturnSyncDate,
                               zVIEW   vWkTZCMLPLO,
                               zPCHAR  pchSyncDate,
                               zSHORT  nDependentType,
                               zSHORT  nForce )
{
   zVIEW   vMeta;
   zCHAR   szDepSyncDate[ 20 ];
   zSHORT  nRC;

/* PETTIT Bypass of sync date code */
   if ( nForce != 1 )
      return( 0 );

   nRC = 0;
   if ( SetCursorFirstEntityByInteger( vWkTZCMLPLO, "W_MetaType", "Type", nDependentType, 0 ) == zCURSOR_SET )
   {
      if ( SetCursorFirstEntity( vWkTZCMLPLO, "W_MetaDef", 0 ) == zCURSOR_SET )
      {
         do
         {
            GetStringFromAttribute( szDepSyncDate, zsizeof( szDepSyncDate ), vWkTZCMLPLO, "W_MetaDef", "LastSyncDate" );
            if ( nForce || zstrcmp( szDepSyncDate, pchSyncDate ) > 0 )
            {
               SysMutexLock( vSubtask, "ActMeta", 0, 0 );
            // TraceLineX( "fnActivateDependentBySyncDate Locked Mutex: ActMeta  for Task: ", (zLONG) vSubtask->hTask );
               fnActivateMetaOI( vSubtask, &vMeta, vWkTZCMLPLO, nDependentType, 0 );
            // TraceLineX( "fnActivateDependentBySyncDate Unlocking Mutex: ActMeta  for Task: ", (zLONG) vSubtask->hTask );
               SysMutexUnlock( vSubtask, "ActMeta", 0 );

               // After the object instance has been forced in, get its syncronization date again in case it was in
               // turn updated by being forced in.
               GetStringFromAttribute( szDepSyncDate, zsizeof( szDepSyncDate ), vWkTZCMLPLO, "W_MetaDef", "LastSyncDate" );
               if ( zstrcmp( szDepSyncDate, szReturnSyncDate ) > 0 )
                  strcpy_s( szReturnSyncDate, zsizeof( szReturnSyncDate ), szDepSyncDate );

               nRC++;
               if ( vMeta )
                  DropView( vMeta );
            }
         }  while ( SetCursorNextEntity( vWkTZCMLPLO, "W_MetaDef", 0 ) == zCURSOR_SET );
      }
   }

   // Return the number of dependents forced in.
   return( nRC );
}

//////////////////////////////////////////////////////////////////////////////
//
//     fnTZZOLODO_BldFlatFromChild
//           Post checkout of a LOD build's the derived flat side
//
//////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZZOLODO_BldFlatFromChild( zVIEW vTZZOLOD1, zVIEW vTZZOLODO )
{
   zSHORT   nRC;

   nRC = CheckExistenceOfEntity( vTZZOLOD1, "LOD_EntityChild" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
//  Include Recursive Representation to Flat Representation
      nRC = IncludeSubobjectFromSubobject( vTZZOLODO, "LOD_Entity", vTZZOLOD1, "LOD_EntityParent", zPOS_AFTER );

      nRC = CheckExistenceOfEntity( vTZZOLOD1, "LOD_EntityChild" );
      if ( nRC >= zCURSOR_SET )
      {
         nRC = ofnTZZOLODO_BldFlatFromChild( vTZZOLOD1, vTZZOLODO );
      }

      nRC = ResetViewFromSubobject( vTZZOLOD1 );
      nRC = SetCursorNextEntity( vTZZOLOD1, "LOD_EntityChild", "" );
   }

   return( nRC );
}

//////////////////////////////////////////////////////////////////////////////
//
//     fnTZZOLODO_BldFlatFromRec
//           Post checkout of a LOD build's the derived flat side
//
//////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnTZZOLODO_BuildFlatFromRec( zVIEW  vLOD )
{
   zVIEW    vTZZOLOD1;
   zVIEW    vTZZOLODO;
   zSHORT   nRC;

   CreateViewFromViewForTask( &vTZZOLOD1, vLOD, 0 );
   CreateViewFromViewForTask( &vTZZOLODO, vLOD, 0 );

   nRC = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityParent", "" );
//  Include Recursive Representation to Flat Representation
   nRC = IncludeSubobjectFromSubobject( vTZZOLODO, "LOD_Entity", vTZZOLOD1, "LOD_EntityParent", zPOS_AFTER );


   nRC = CheckExistenceOfEntity( vTZZOLOD1, "LOD_EntityChild" );
   if ( nRC >= zCURSOR_SET )
   {
      nRC = ofnTZZOLODO_BldFlatFromChild( vTZZOLOD1, vTZZOLODO );
   }

   return( nRC );
}

// Gig 7/5

zVOID
fnLinkAndSyncER_ToSA( zVIEW vSubtask, zVIEW vTZEREMDO, zVIEW vTZERSASO )
{
   zVIEW vER2;
   zVIEW vSA2;
   zVIEW vSA3;
   zLONG lZKey1;
   zLONG lZKey2;

   // We have retrieved a view to an active ERD, link up the
   // Subject Area to the ER_MODEL.
   RelinkAllSubobjectsForOI( vTZERSASO, "ER_Entity", vTZEREMDO, "ER_Entity" );
   RelinkAllSubobjectsForOI( vTZERSASO, "ER_RelType", vTZEREMDO, "ER_RelType" );
   RelinkAllSubobjectsForOI( vTZERSASO, "EntpER_Model", vTZEREMDO, "EntpER_Model" );
   RelinkAllSubobjectsForOI( vTZERSASO, "ER_RelLink", vTZEREMDO, "ER_RelLink" );
   RelinkAllSubobjectsForOI( vTZERSASO, "ER_RelLink_Other", vTZEREMDO, "ER_RelLink_Other" );
   RelinkAllSubobjectsForOI( vTZERSASO, "OwnedER_Entity", vTZEREMDO, "ER_Entity" );
   RelinkAllSubobjectsForOI( vTZERSASO, "OwnedER_RelType", vTZEREMDO, "ER_RelType" );
   RelinkAllSubobjectsForOI( vTZEREMDO, "EntOwningSA", vTZERSASO, "SubjectArea" );
   RelinkAllSubobjectsForOI( vTZEREMDO, "RelOwningSA", vTZERSASO, "SubjectArea" );

   // Now that we have relinked to the er_model, we need to syncronize the SA with the model
   if ( SetCursorFirstEntity( vTZERSASO, "SA_Entity", "SubjectArea" ) >= zCURSOR_SET )
   {
      do
      {
         GetIntegerFromAttribute( &lZKey1, vTZERSASO, "ER_Entity", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vTZEREMDO, "ER_Entity", "ZKey", lZKey1, 0 ) != zCURSOR_SET )
         {
            DeleteEntity( vTZERSASO, "SA_Entity", zREPOS_NONE );
         }

      }  while ( SetCursorNextEntity( vTZERSASO, "SA_Entity", "SubjectArea" ) > zCURSOR_UNCHANGED );
   }

   // Now that we have relinked to the er_model, we need to syncronize the SA with the model
   if ( SetCursorFirstEntity( vTZERSASO, "SA_RelType", "SubjectArea" ) >= zCURSOR_SET )
   {
      do
      {
         GetIntegerFromAttribute( &lZKey1, vTZERSASO, "ER_RelType", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vTZEREMDO, "ER_RelType", "ZKey", lZKey1, 0 ) != zCURSOR_SET )
         {
            if ( SetCursorFirstEntityByInteger( vTZERSASO, "ER_RelType_1", "ZKey", lZKey1, "SubjectArea" ) )
            {
               ExcludeEntity( vTZERSASO, "ER_RelLink", zREPOS_NONE );
               if ( SetCursorNextEntityByInteger( vTZERSASO, "ER_RelType_1", "ZKey", lZKey1, "SubjectArea" ) >= zCURSOR_SET )
               {
                  ExcludeEntity( vTZERSASO, "ER_RelLink", zREPOS_NONE );
               }
            }

            DeleteEntity( vTZERSASO, "SA_RelType", zREPOS_NONE );
         }
      }  while ( SetCursorNextEntity( vTZERSASO, "SA_RelType", "SubjectArea" ) > zCURSOR_UNCHANGED );
   }

   // Now re-establish the RelLink_Other path
   if ( SetCursorFirstEntity( vTZERSASO, "ER_RelType_1", "SubjectArea" ) >= zCURSOR_SET )
   {
      do
      {
         if ( CheckExistenceOfEntity( vTZERSASO, "ER_RelLink_Other" ) < zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lZKey1, vTZERSASO, "ER_RelLink", "ZKey" );
            SetCursorFirstEntityByInteger( vTZEREMDO, "ER_RelLink", "ZKey", lZKey1, "EntpER_Model" );
            IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelLink_Other", vTZEREMDO, "ER_RelLink_Other", zPOS_AFTER );
         }
      } while ( SetCursorNextEntity( vTZERSASO, "ER_RelType_1", "SubjectArea" ) >= zCURSOR_SET );
   }

   // STEP 3, add in all Reltypes from the model whose source and
   //         Target Entities both exist in the subject area
   CreateViewFromViewForTask( &vER2, vTZEREMDO, 0 );
   CreateViewFromViewForTask( &vSA2, vTZERSASO, 0 );
   CreateViewFromViewForTask( &vSA3, vTZERSASO, 0 );
   if ( SetCursorFirstEntity( vTZEREMDO, "ER_RelType", 0 ) >= zCURSOR_SET )
   do
   {
      GetIntegerFromAttribute( &lZKey1, vTZEREMDO, "ER_RelType", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vTZERSASO, "ER_RelType", "ZKey", lZKey1, "SubjectArea" ) < zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey1, vTZEREMDO, "ER_Entity_2", "ZKey" );
         SetViewFromView( vER2, vTZEREMDO );
         SetCursorNextEntity( vER2, "ER_RelLink_2", 0 );
         GetIntegerFromAttribute( &lZKey2, vER2, "ER_Entity_2", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vSA2, "ER_Entity", "ZKey", lZKey1, "SubjectArea" ) >= zCURSOR_SET &&
              SetCursorFirstEntityByInteger( vSA3, "ER_Entity", "ZKey", lZKey2, "SubjectArea" ) >= zCURSOR_SET )
         {
            CreateMetaEntity( vSubtask, vTZERSASO, "SA_RelType", zPOS_LAST );
            IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelType", vTZEREMDO, "ER_RelType", zPOS_BEFORE );
            SetCursorLastEntity( vSA2, "ER_RelLink", 0 );
            IncludeSubobjectFromSubobject( vSA2, "ER_RelLink", vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vSA2, "ER_RelLink_Other", vER2, "ER_RelLink_2", zPOS_AFTER );
            SetCursorLastEntity( vSA3, "ER_RelLink", 0 );
            IncludeSubobjectFromSubobject( vSA3, "ER_RelLink", vER2, "ER_RelLink_2", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vSA3, "ER_RelLink_Other", vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
         }
      }
   }  while ( SetCursorNextEntity( vTZEREMDO, "ER_RelType", 0 ) >= zCURSOR_SET );

   DropView( vER2 );
   DropView( vSA2 );
   DropView( vSA3 );
}
// Gig 7/5
