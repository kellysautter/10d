/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmcpro.c - Zeidon application operations
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
//    1997/04/25: Frank H. addes an operation for CPLR-Enqueueing
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2001.06.25  BL
   maintained Repository part 2 ( added oTZCMCPRO_DeleteMetaWithCluster,
   oTZCMCPRO_DeleteFileWithCluster and oTZCMCPRO_DeleteDependentsWithCluster )

2001.04.08  BL
   maintained Repository and added oTZCMRCRO_DeriveCPLR_CPL_Name for
   reusable CPLR's

2000.03.17  HH
   improve enqueue, if same user on different workstation.

2000.03.13  HH
   call ActivateOI_FromFile with zIGNORE_ATTRIB_ERRORS, needed for delete
   of old (9j) LPLR components.

2000.02.15-03.03  BL
   Modified oTZCMCPRO_DeleteMeta for registered View
   Modified LoadFileAndActivateMeta: added Parameter for delete
   file from temp directory

2000.02.11  HH
   repo change: remove atomic metas (oTZCMCPRO_DeleteMeta).

OLD CHANGE LOG
04.11.1997  DonC
   Modified ActivateCPLR to always create qualification object new since the
   existing one sometimes had bad data in it.

05.01.1998  DonC
   Added oTZCMCPRO_DeleteDependents operation to delete dependent metas
   and files for each meta type and moved DeleteMeta from TZCMCPLD.C to this
   routine and renamed it oTZCMCPRO_DeleteMeta. Also modified it.

29.05.1998    RG!!
   Bugfixes erhalten am 29.05.1998 von DonC, Doug ohne Aenderungsvermerk
   sollte nachgeliefert werden

25.01.1999  HH
   changed DeleteServerFile to correct bug TB471.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"


// prototypes
zOPER_EXPORT zSHORT OPERATION
LoadFileAndActivateMeta( zVIEW  vSubtask,
                         zPVIEW pvMeta,         // return meta
                         zVIEW  vTZCMCPRO,      // CPLR object
                         zPCHAR szName,         // file name
                         zPCHAR szExtension,    // extension
                         zLONG  lType,          // OD type
                         zBOOL  bDeleteFile );   // delete file from temp dir


// DESC:  Parses a string and changes a null ('/0') character and any
//        characters after the null to blanks.  It can be used when
//        setting up a print line to remove nulls and garbage characters
//        following nulls when multiple fields are moved to the print line.
//        if bNull is TRUE, the last character is set to NULL, otherwise
//        it is set to Blank if necessary
void OPERATION
zstrblank( zPCHAR pchString,
           zSHORT nLth,
           zSHORT bNullLastByte )
{
   zSHORT k = (zSHORT) zstrlen( pchString );
   if ( k <= nLth )
      zmemset( pchString + k, ' ', nLth - k );

   if ( bNullLastByte )
      pchString[ nLth - 1 ] = 0;
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Returns the view of the CPLR matching the ZKey passed.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_ActivateCPLR( zPVIEW vTZCMCPRO,
                        zLONG lZKey,
                        zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zCHAR    szZKeyValue[18];
   zSHORT   nRC;

   if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                     zSINGLE ) < 0 )
   {
      return( -1 );
   }

   SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
   CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );

   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "CPLR" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   zltoa( lZKey, szZKeyValue, zsizeof( szZKeyValue ) );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKeyValue );

   nRC = ActivateObjectInstance( vTZCMCPRO, "TZCMCPRO", vSubtask,
                                 vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );

   DropObjectInstance( vKZDBHQUA );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_CleanUpER_TE_SA( zVIEW vTZCMCPRO,
                           zVIEW vTZCMLPLO_In )
{
   zVIEW    vTZCMCPRO2;
   zVIEW    vTZCMLPLO;
   unsigned long ulZKey;
   zLONG    Type;
   zSHORT   nRC;
   zSHORT   nRC2;

   CreateViewFromViewForTask( &vTZCMLPLO, vTZCMLPLO_In, 0 );
   nRC = SetCursorFirstEntity( vTZCMCPRO, "Z_MetaDef", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &Type, vTZCMCPRO, "Z_MetaDef", "Type" );
      if ( (Type == zSOURCE_ERD_META) || (Type == zSOURCE_DTE_META) || (Type == zSOURCE_SA_META) )
      {
         if ( CompareAttributeToAttribute( vTZCMCPRO, "Z_MetaDef", "LastCheckInDate",
                                           vTZCMCPRO, "CPLR", "LastCheckInDate" ) == 0 )
         {
            SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type", zSOURCE_ERD_META, "" );
            if ( SetCursorFirstEntity( vTZCMLPLO, "W_MetaDef", "" ) >= zCURSOR_SET )
            {
               if ( CompareAttributeToInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd", 0 ) == 0 )
               {
                  GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMLPLO, "W_MetaDef", "CPLR_ZKey" );
                  ExcludeEntity( vTZCMLPLO, "W_MetaDef", zREPOS_NONE );
                  CreateViewFromViewForTask( &vTZCMCPRO2, vTZCMCPRO, 0 );
                  SetCursorFirstEntityByInteger( vTZCMCPRO2, "Z_MetaDef",  "ZKey", ulZKey, "" );
                  if ( CheckExistenceOfEntity( vTZCMCPRO2, "UserMetaStatus" ) >= zCURSOR_SET )
                     DeleteEntity( vTZCMCPRO2, "UserMetaStatus", zREPOS_NONE );
                  DropView( vTZCMCPRO2 );
               }
            }
            SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type", zSOURCE_DTE_META, "" );
            if ( SetCursorFirstEntity( vTZCMLPLO, "W_MetaDef", "" ) >= zCURSOR_SET )
            {
               if ( CompareAttributeToInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd", 0 ) == 0 )
               {
                  GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMLPLO, "W_MetaDef", "CPLR_ZKey" );
                  ExcludeEntity( vTZCMLPLO, "W_MetaDef", zREPOS_NONE );
                  CreateViewFromViewForTask( &vTZCMCPRO2, vTZCMCPRO, 0 );
                  SetCursorFirstEntityByInteger( vTZCMCPRO2, "Z_MetaDef", "ZKey", ulZKey, "" );
                  if ( CheckExistenceOfEntity( vTZCMCPRO2, "UserMetaStatus" ) >= zCURSOR_SET )
                     DeleteEntity( vTZCMCPRO2, "UserMetaStatus", zREPOS_NONE );

              DropView( vTZCMCPRO2 );
               }
            }
            SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type", zSOURCE_SA_META, "" );
            nRC2 = SetCursorFirstEntity( vTZCMLPLO, "W_MetaDef", "" );
            while ( nRC2 >= zCURSOR_SET )
            {
               if ( CompareAttributeToInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd", 0 ) == 0 )
               {
                  GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMLPLO, "W_MetaDef", "CPLR_ZKey" );
                  ExcludeEntity( vTZCMLPLO, "W_MetaDef", zREPOS_NONE );
                  CreateViewFromViewForTask( &vTZCMCPRO2, vTZCMCPRO, 0 );
                  SetCursorFirstEntityByInteger( vTZCMCPRO2, "Z_MetaDef", "ZKey", ulZKey, "" );
                  if ( CheckExistenceOfEntity( vTZCMCPRO2, "UserMetaStatus" ) >= zCURSOR_SET )
                     DeleteEntity( vTZCMCPRO2, "UserMetaStatus", zREPOS_NONE );

                  DropView( vTZCMCPRO2 );
               }
               nRC2 = SetCursorNextEntity( vTZCMLPLO, "W_MetaDef", "" );
            }
         }
      }
      nRC = SetCursorNextEntity( vTZCMCPRO, "Z_MetaDef", "" );
   }
   DropView( vTZCMLPLO );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZCMCRWO_DeriveCPL_CPLR_Name( zVIEW            vMeta,
                               LPVIEWENTITY     lpViewEntity,
                               LPVIEWATTRIB     lpViewAttrib,
                               zSHORT           sMsg )
{
   zCHAR   szWorkString[ 70 ];
   zCHAR   szCPLR_Name[ 33 ];

   GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vMeta, "CPL", "Name" );
   zstrblank( szWorkString, 20, 1 );
   strcat_s( szWorkString, zsizeof( szWorkString ), " - " );
   GetStringFromAttribute( szCPLR_Name, zsizeof( szCPLR_Name ), vMeta, "CPLR", "Name" );
   strcat_s( szWorkString, zsizeof( szWorkString ), szCPLR_Name );

   StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szWorkString, 0 );
   return( 0 );
}

////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_TestCPLR( zULONG lCPLR_ZKey,
                    zPCHAR pszCPLR_Name,
                    zPVIEW vTZCMQCPO_List,
                    zVIEW  vSubtask,
                    zVIEW  vTZCMWIPO,
                    zSHORT EnqueueType)
{
   zSHORT   TestRC;
   zSHORT   nRC;
   zCHAR    szMsg[500];
   zCHAR    UserName[ 32 ];
   zSHORT   EnqueueType_Queue;
   zCHAR    szUserName_Queue[ 32 ];
   zCHAR    szUserDesc_Queue[ 32 ];
   zCHAR    szDate_Queue[ 20 ];

   // position on the EnqueueObject with the CPLR_Key.
   nRC = SetCursorFirstEntity( *vTZCMQCPO_List, "CPLR_Enqueue", 0 );
   TestRC = 1;

   while ( nRC >= zCURSOR_SET && TestRC != 0)
   {
      // if there is no CPLR --> the Enqueue is not valid --> Delete it...
      if ( CheckExistenceOfEntity( *vTZCMQCPO_List, "CPLR" ) >= zCURSOR_SET )
      {
         TestRC = CompareAttributeToInteger( *vTZCMQCPO_List, "CPLR", "ZKey", lCPLR_ZKey );
      }
      else
      if ( CheckExistenceOfEntity( *vTZCMQCPO_List, "CPLR_Enqueue" ) >= zCURSOR_SET )
      {
         DeleteEntity( *vTZCMQCPO_List, "CPLR_Enqueue", zREPOS_NONE );
      }

      nRC = SetCursorNextEntity( *vTZCMQCPO_List, "CPLR_Enqueue", 0 );
   }

   if ( TestRC == 0 )
   {
      zSHORT iSame = 0;

      // found a CPLR-Enqueue
      // Better compare Workstation ID instead of User Name, because we allow
      //  the same user on different workstations.
      if ( CompareAttributeToAttribute( *vTZCMQCPO_List, "CPLR_Enqueue", "UserName",
                                        vTZCMWIPO, "ROOT", "UserName" ) == 0 )
      {
         iSame = 1;
      }

      if ( iSame )
      {
         // same user, compare workstations as well
         zVIEW vTZCMWKSO;
         nRC = GetViewByName( &vTZCMWKSO, "TZCMWKSO", vSubtask, zLEVEL_TASK );
         if ( nRC >= zLEVEL_TASK &&
              CompareAttributeToAttribute( *vTZCMQCPO_List, "RepositoryClient", "WKS_Id",
                                           vTZCMWKSO, "RepositoryClient", "WKS_Id" ) != 0 )
         {
            iSame = 0;
         }
      }

      if ( iSame == 0 )
      {
         // the found CPLR-Enqueue is caused by another user.
         //  also if it is the same user name, but a different workstation

         GetStringFromAttribute( UserName, zsizeof( UserName ), vTZCMWIPO, "ROOT", "UserName" );
         strcpy_s( szMsg, zsizeof( szMsg ), "The CPLR '" );
         strcat_s( szMsg, zsizeof( szMsg ), pszCPLR_Name );
         strcat_s( szMsg, zsizeof( szMsg ), "' is at this time enqueued \n\n by\t" );
         GetStringFromAttribute( szUserName_Queue, zsizeof( szUserName_Queue ), *vTZCMQCPO_List, "CPLR_Enqueue", "UserName" );
         strcat_s( szMsg, zsizeof( szMsg ), szUserName_Queue );
         GetStringFromAttribute( szUserDesc_Queue, zsizeof( szUserDesc_Queue ), *vTZCMQCPO_List, "CPLR_Enqueue", "UserDesc" );
         if ( szUserDesc_Queue[ 0 ] != 0 )
         {
            strcat_s( szMsg, zsizeof( szMsg ), " (" );

            strcat_s( szMsg, zsizeof( szMsg ), szUserDesc_Queue );
            strcat_s( szMsg, zsizeof( szMsg ), ")" );
         }
         strcat_s( szMsg, zsizeof( szMsg ), "\n on\t" );
         GetStringFromAttributeByContext( szDate_Queue, *vTZCMQCPO_List, "CPLR_Enqueue", "TimeStamp",
                                          "DD.MM.YYYY HH:MM:SS", 19 );
         strcat_s( szMsg, zsizeof( szMsg ), szDate_Queue );

         GetIntegerFromAttribute( (zPLONG) &EnqueueType_Queue, *vTZCMQCPO_List, "CPLR_Enqueue", "EnqueueType" );
         strcat_s( szMsg, zsizeof( szMsg ), "\n for\t" );

         switch( EnqueueType_Queue )
         {
            case zCPLR_ENQUEUE_FOR_REFRESH:   strcat_s( szMsg, zsizeof( szMsg ), "Refresh." );
                                              break;
            case zCPLR_ENQUEUE_FOR_CHECK_IN:  strcat_s( szMsg, zsizeof( szMsg ), "CheckIn." );
                                              break;
            case zCPLR_ENQUEUE_FOR_CHECK_OUT: strcat_s( szMsg, zsizeof( szMsg ), "CheckOut." );
                                              break;
            case zCPLR_ENQUEUE_BY_SYSADM:     strcat_s( szMsg, zsizeof( szMsg ), "Maintenance." );
                                              break;
            default:strcat_s( szMsg, zsizeof( szMsg ), "unknown EnqueueType (!)" );

         }

         // if the EnqueueType = 5 --> SystemAdmin
         // Only the SystemAdmin has the authority to cancel the Enqueue !
         if ( EnqueueType < zCPLR_ENQUEUE_BY_SYSADM )
         {
            // cannot do anything about this Enqueue
            strcat_s( szMsg, zsizeof( szMsg ), "\n\nPlease try later or resolve the problem with the SystemAdministrator." );

            MessageSend( vSubtask, "CM00503", "Configuration Management",
                         szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

            return( -1 );

         }

         else
         // the SystemAdmin has the authority to cancel the Enqueue !!!
         {
            strcat_s( szMsg, zsizeof( szMsg ), "\n\nSince YOU are the System Administrator, " );
            strcat_s( szMsg, zsizeof( szMsg ), "you have the authority to cancel this user action." );
            strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you want to do so (please contact " );
            strcat_s( szMsg, zsizeof( szMsg ), szUserName_Queue );
            strcat_s( szMsg, zsizeof( szMsg ), " first) ?" );

            if ( OperatorPrompt( vSubtask, "Confirmation", szMsg, 1, zBUTTONS_YESNO,
                                zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_YES )
            {
               if ( CheckExistenceOfEntity( *vTZCMQCPO_List, "CPLR_Enqueue" ) >= zCURSOR_SET )
                  DeleteEntity( *vTZCMQCPO_List, "CPLR_Enqueue", zREPOS_NONE );

               nRC = CommitObjectInstance( *vTZCMQCPO_List );
               return( 0 );
            }
            else
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
               return( -1 );
         }
      }
      else
      {
         // There was an enqueue caused by the same user un same workstation
         // who tries to enqueue the CPLR

         MB_SetMessage( vSubtask, 1,
                       "There was an enqueue caused by you, which will be cleared.");

         if (CheckExistenceOfEntity( *vTZCMQCPO_List, "CPLR_Enqueue" ) >= zCURSOR_SET )
            DeleteEntity( *vTZCMQCPO_List, "CPLR_Enqueue", zREPOS_NONE );

         nRC = CommitObjectInstance( *vTZCMQCPO_List );
         return( 0 );
      }
   }
   else
   {
      // no enqueue found with conflicts
      strcpy_s( szMsg, zsizeof( szMsg ), "OK, no enqueue conflict for CPLR: " );
      strcat_s( szMsg, zsizeof( szMsg ), pszCPLR_Name );

      MB_SetMessage( vSubtask, 1, szMsg );
   }
   return( 0 );
}

////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_WriteEnqueue( zVIEW  vTZCMCPRO,
                        zVIEW  vTZCMWIPO,
                        zPVIEW vTZCMQCPO,
                        zSHORT EnqueueType )
{
   zSHORT   nRC;
   zVIEW    vTZCMWKSO;
   zCHAR    szTimestamp[ 22 ];

   nRC = ActivateEmptyObjectInstance( vTZCMQCPO, "TZCMQCPO",
                                      vTZCMCPRO, zSINGLE );
   CreateEntity( *vTZCMQCPO, "CPLR_Enqueue", zPOS_AFTER );

   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( *vTZCMQCPO, "CPLR_Enqueue", "TimeStamp", szTimestamp );
   SetAttributeFromInteger( *vTZCMQCPO, "CPLR_Enqueue", "EnqueueType", EnqueueType );

   IncludeSubobjectFromSubobject( *vTZCMQCPO, "CPLR", vTZCMCPRO, "CPLR", zPOS_AFTER );

   nRC = GetViewByName( &vTZCMWKSO, "TZCMWKSO", vTZCMCPRO, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      // we enqueued the CPLR thru the WksAdmin.
      // --> include the Workstation (= repositoryClient)
      IncludeSubobjectFromSubobject( *vTZCMQCPO, "RepositoryClient", vTZCMWKSO, "RepositoryClient", zPOS_AFTER );
      SetAttributeFromAttribute( *vTZCMQCPO, "CPLR_Enqueue", "UserName", vTZCMWKSO, "User", "Name" );
      SetAttributeFromAttribute( *vTZCMQCPO, "CPLR_Enqueue", "UserDesc", vTZCMWKSO, "User", "Desc" );
   }
   else
   {
      // we must have enqueued the CPLR thru the SystemAdmin.
      // --> there is no workstation.
      SetAttributeFromAttribute( *vTZCMQCPO, "CPLR_Enqueue", "UserName", vTZCMWIPO, "ROOT", "UserName" );
      SetAttributeFromAttribute( *vTZCMQCPO, "CPLR_Enqueue", "UserDesc", vTZCMWIPO, "ROOT", "UserDesc" );
   }

   nRC = CommitObjectInstance( *vTZCMQCPO );
   return( nRC );
}

////////////////////////////////////////////////
//
//  OPERATION:  oTZCMCPRO_Enqueue_CPLR_Enqueue
//
//  PURPOSE:    Checks for a CPLR Enqueue via the via SystemAdmin or
//              CheckIn, CheckOut or Refresh.
//
//  PARAMETERS: vTZCMCPRO - A pointer to the return(changed) CPLR view
//              vSubtask  - A qualifying view for the Activation
//              EnqueueType
//                Values:   0  - Dequeue CPLR          = zCPLR_DEQUEUE
//                          1  - Enqueue for Refresh   = zCPLR_ENQUEUE_FOR_REFRESH
//                          2  - Enqueue for Check In  = zCPLR_ENQUEUE_FOR_CHECK_IN
//                          3  - Enqueue for Check Out = zCPLR_ENQUEUE_FOR_CHECK_OUT
//                          4  - Not currently in use
//                          5  - Enqueued by SYSADM    = zCPLR_ENQUEUE_BY_SYSADM
//
//
//  RETURNS:   >= 0  - TZCMCPRO was not enqueued and
//                     is now activated successfully
//               -1  - Error encountered during activation
//
////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_CPLR_Enqueue( zVIEW  vTZCMCPRO,
                        zVIEW  vSubtask,
                        zSHORT EnqueueType )
{
   zVIEW    vTZCMWIPO;
   zVIEW    vTZCMQCPO_List = NULL;
   zVIEW    vTZCMQCPO = NULL;
   zSHORT   nRC;
   zSHORT   TestRC;
   zLONG    lCPLR_ZKey;
   zCHAR    szCPLR_Name[ 32 ];
   zCHAR    szMsg[500];

   // Temporary trace to identify who and when for enqueues.
   GetStringFromAttribute( szMsg, zsizeof( szMsg ), vTZCMCPRO, "CPLR", "Name" );
   strcat_s( szMsg, zsizeof( szMsg ), ", CPLR Enqueue: " );
   TraceLineI( szMsg, EnqueueType );

   if ( EnqueueType > 0 )
   {
      // ENqueue the CPLR:
      GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

      // try to enqueue the CPLR
      nRC = oTZCMCPRO_WriteEnqueue( vTZCMCPRO, vTZCMWIPO, &vTZCMQCPO, EnqueueType );

      // load all Enqueues for the testing of the ReusableCPLRs
      ActivateObjectInstance( &vTZCMQCPO_List,"TZCMQCPO", vSubtask, 0, zMULTIPLE );
      if ( nRC == zDUPLICATE_ROOT )
      {
         // if the first Enqueue wasn't successfull, give an OperatorMessage
         // (thru oTZCMCPRO_TestCPLR) and then try one more time
         GetIntegerFromAttribute( &lCPLR_ZKey, vTZCMCPRO, "CPLR", "ZKey" );
         GetStringFromAttribute( szCPLR_Name, zsizeof( szCPLR_Name ), vTZCMCPRO, "CPLR", "Name" );
         TestRC = oTZCMCPRO_TestCPLR( lCPLR_ZKey, szCPLR_Name, &vTZCMQCPO_List, vSubtask, vTZCMWIPO, EnqueueType);
         if ( TestRC < 0 )
         {
            // can't do anything against the Enqueue.
            if ( vTZCMQCPO_List )
               DropObjectInstance( vTZCMQCPO_List );

            return( TestRC );
         }
         else
         {
            // now the Enqueue-Object ist removed thru oTZCMCPRO_TestCPLR and should be enqueued (2nd try):
            nRC = oTZCMCPRO_WriteEnqueue( vTZCMCPRO, vTZCMWIPO, &vTZCMQCPO, EnqueueType );
            if (nRC < 0 )
            {
               // this message should never appear !!!
               MessageSend( vSubtask, "CM00503", "Configuration Management",
                            "can't write Enqueue-Object ... (2nd try).",
                            zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

               if ( vTZCMQCPO_List )
                  DropObjectInstance( vTZCMQCPO_List );

               return( -1 );
            }
         }
      }
      else
      if ( nRC < 0 )
      {
         // this message should never appear !!!
         MessageSend( vSubtask, "CM00503", "Configuration Management",
                      "can't write Enqueue-Object ...",
                      zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

         if ( vTZCMQCPO_List )
            DropObjectInstance( vTZCMQCPO_List );

         return( -1 );
      }

      // the Enqueue was successfull and we may proceed with the reusableCPLRs
      SetNameForView( vTZCMQCPO, "TZCMQCPO", vSubtask, zLEVEL_TASK );

      GetIntegerFromAttribute( &lCPLR_ZKey, vTZCMCPRO, "CPLR", "ZKey" );
      GetStringFromAttribute( szCPLR_Name, zsizeof( szCPLR_Name ), vTZCMCPRO, "CPLR", "Name" );

      // now, loop through all reusable CPLRs:
      TestRC = 0;
      for ( nRC = SetCursorFirstEntity( vTZCMCPRO, "ReusableCPLR", 0 );
            nRC >= zCURSOR_SET && TestRC >= 0;
            nRC = SetCursorNextEntity( vTZCMCPRO, "ReusableCPLR", 0 ) )
      {
         GetIntegerFromAttribute( &lCPLR_ZKey, vTZCMCPRO, "ReusableCPLR", "ZKey" );
         GetStringFromAttribute( szCPLR_Name, zsizeof( szCPLR_Name ), vTZCMCPRO, "ReusableCPLR", "Name" );
         // test with oTZCMCPRO_TestCPLR whether the ReusableCPLR ist currently enqueued
         TestRC = oTZCMCPRO_TestCPLR( lCPLR_ZKey, szCPLR_Name,
                                      &vTZCMQCPO_List, vSubtask,
                                      vTZCMWIPO, EnqueueType);
      }

      if ( TestRC < 0 )
      {
         // Delete the EnqueueObject which was already written
         // because a ReusableCPLR is currently enqueued.
         oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO, vSubtask, zCPLR_DEQUEUE );
      }

      if ( vTZCMQCPO_List )
         DropObjectInstance( vTZCMQCPO_List );

      return( TestRC );
   }
   else
   {
      // DEqueue the CPLR:
      if ( GetViewByName( &vTZCMQCPO, "TZCMQCPO", vSubtask, zLEVEL_TASK ) >= zCURSOR_SET )
      {
         MB_SetMessage( vSubtask, 1, "Dequeue..." );

         // Delete the existing EnqueueObject
         if ( CheckExistenceOfEntity( vTZCMQCPO, "CPLR_Enqueue" ) >= zCURSOR_SET )
            DeleteEntity( vTZCMQCPO, "CPLR_Enqueue", zREPOS_NONE );

         nRC = CommitObjectInstance( vTZCMQCPO );
         MB_SetMessage( vSubtask, 1, "" );

         if ( nRC == 0 )
         {
            DropObjectInstance( vTZCMQCPO );
         }
         else
         {
            // this message should never appear !!!
            MessageSend( vSubtask, "CM00503", "Configuration Management",
                         "PROBLEM with dequeue !!!",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }
   }

   if ( vTZCMQCPO_List )
      DropObjectInstance( vTZCMQCPO_List );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteFileWithCluster( zVIEW  vSubtask,
                                 zVIEW  vTZCMFILE,
                                 zPCHAR szName,
                                 zPCHAR szExtension)
{
   zVIEW   vFile;
   zVIEW   vTempFile;
   zULONG  ulZKey;
   zLONG   lFile_ViewCluster;
   zSHORT  nRC;
   zBOOL   bFound;
   zCHAR   szViewName[ 100 ];
   zCHAR   szMsg[ 100 ];

   // Get File View Cluster.
   GetIntegerFromAttribute( &lFile_ViewCluster, vTZCMFILE, "CPLR", "File_ViewCluster" );

   if ( lFile_ViewCluster == 0 )
   {
      CreateViewCluster( vSubtask, &lFile_ViewCluster );
      SetAttributeFromInteger( vTZCMFILE, "CPLR", "File_ViewCluster", lFile_ViewCluster );
   }

   // Position on File in CPLR object.
   CreateViewFromViewForTask( &vTempFile, vTZCMFILE, 0 );

   bFound = FALSE;
   nRC = SetCursorFirstEntityByString( vTempFile, "File", "Name", szName, "" );

   while ( bFound == FALSE && nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vTempFile, "File", "Extension",  szExtension ) == 0 )
         bFound = TRUE;
      else
         nRC = SetCursorNextEntityByString( vTempFile, "File", "Name", szName, "" );
   }

   if ( bFound == FALSE )
   {
      DropView( vTempFile );
      return( -1 );
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTempFile, "File", "ZKey" );
   nRC = oTZCMLPLO_ActivateFileOI( &vFile, vTempFile, ulZKey );

   if ( nRC >= 0 )
   {
      sprintf_s( szViewName, zsizeof( szViewName ), "TZCMFILO_%p", vFile );
      SetNameForView( vFile, szViewName, vSubtask, zLEVEL_TASK );

      AddToViewCluster( lFile_ViewCluster, vFile, zCOMMIT_DROPOBJECTINSTANCE );

      if ( DeleteEntity( vFile, "File", zREPOS_NONE ) != zCALL_ERROR )
         DeleteEntity( vTempFile, "File", zREPOS_PREV );
   }
   else
   {
      sprintf_s( szMsg, zsizeof( szMsg ), "File '%s.%s' not found.", szName, szExtension );
      MessageSend( vSubtask, "CM00299", "Configuration Management",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   DropView( vTempFile );
   return( nRC );
}



zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteDependentsWithCluster( zVIEW vTZCMCPLR,
                                       zVIEW vTZCMFILE,
                                       zVIEW vMetaOI,
                                       zVIEW vKZDBHQUA,
                                       zVIEW vSubtask )
{
   // For each meta type, this routine deletes any associated files.
   zVIEW    vTempCPLR;
   zLONG    lType;
   zULONG   ulZKey;
   zSHORT   nRC;
   zCHAR    szZKey[12];
   zCHAR    szName[ 33 ];
   zCHAR    szExtension[ 4 ];

   GetIntegerFromAttribute( &lType, vTZCMCPLR, "Z_MetaDef", "Type" );
   CreateViewFromViewForTask( &vTempCPLR, vTZCMCPLR, 0 );

   // Process each special meta type.

   switch ( lType )
   {
      case zSOURCE_PENV_META:

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "PresEnvDef", "Name" );
         nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, "XPE" );

         break;
      case zSOURCE_GOPGRP_META:

         // A Global Operation Group needs to delete the associated source file.
         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "GlobalOperationGroup", "Name" );
         GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "GlobalOperationGroup", "Extension" );
         nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, szExtension );

         break;

      case zSOURCE_DIALOG_META:

         // A Dialog needs to delete any associated source files and the XWD.
         for ( nRC = SetCursorFirstEntity( vMetaOI, "SourceFile", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vMetaOI, "SourceFile", 0 ) )
         {
            GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "SourceFile", "Name" );
            GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "SourceFile", "Extension" );
            nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, szExtension );
         }

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "Dialog", "Tag" );
         nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, "XWD" );

         break;

      case zSOURCE_VOR_META:
         break;

      case zSOURCE_LOD_META:

         // A LOD needs to delete any associated source files, the XOD and any VOR metas.
         for ( nRC = SetCursorFirstEntity( vMetaOI, "SourceFile", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vMetaOI, "SourceFile", 0 ) )
         {
            GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "SourceFile", "Name" );
            GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "SourceFile", "Extension" );
            nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, szExtension );
         }

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "LOD", "Name" );
         nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, "XOD" );

         // To delete associated VORs, we will have to activate every VOR and check its LOD for a match on name.
         for ( nRC = SetCursorFirstEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_VOR_META, 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_VOR_META, 0 ) )
         {
            GetIntegerFromAttribute( (zPLONG) &ulZKey, vTempCPLR, "Z_MetaDef", "ZKey" );
            zltoxa( ulZKey, szZKey );

            // Load the meta from repository file
            nRC = LoadFileAndActivateMeta( vSubtask,
                                           &vMetaOI,  // return meta
                                           vTZCMFILE, // File List object
                                           szZKey,    // file name
                                           "PVR",     // extension
                                           zSOURCE_VOR_META, // OD type
                                           TRUE );    // delete file from temp dir

            if ( nRC >= 0 )
            {
               if ( CheckExistenceOfEntity( vMetaOI, "LOD" ) >= zCURSOR_SET &&
                    CompareAttributeToString( vMetaOI, "LOD", "Name", szName ) == 0 )
               {
                  DeleteEntity( vTempCPLR, "Z_MetaDef", zREPOS_NONE );
                  oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szZKey, "PVR" );
               }
               DropView( vMetaOI );
            }
         }

         break;

      case zSOURCE_SA_META:
         break;

      case zSOURCE_ERD_META:

         // An ERD needs to delete the associated DTE and any SAs.
         nRC = SetCursorFirstEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_DTE_META, 0 );
         if ( nRC >= zCURSOR_SET )
         {
            oTZCMCPRO_DeleteMetaWithCluster( vTempCPLR, vTZCMFILE, vKZDBHQUA, vSubtask );
         }

         for ( nRC = SetCursorFirstEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_SA_META, 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_SA_META, 0 ) )
         {
            oTZCMCPRO_DeleteMetaWithCluster( vTempCPLR, vTZCMFILE, vKZDBHQUA, vSubtask );
         }

         break;

      case zSOURCE_DTE_META:
         break;

      case zSOURCE_DOMAINGRP_META:

         // A Domain Group needs to delete the associated source file.
         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "DomainGroup", "Name" );
         GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "DomainGroup", "Extension" );
         if ( szExtension[ 0 ] )
            nRC = oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, szExtension );

         break;
   }

   DropView( vTempCPLR );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteMetaWithCluster( zVIEW vTZCMCPLR,
                                 zVIEW vTZCMFILE,
                                 zVIEW vKZDBHQUA,
                                 zVIEW vSubtask )
{
   zVIEW    vTempCPLR;
   zVIEW    vMetaOI;
   zULONG   ulZKey;
   zLONG    lType;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;

   CreateViewFromViewForTask( &vTempCPLR, vTZCMCPLR, 0 );

   GetIntegerFromAttribute( &lType, vTempCPLR, "Z_MetaDef", "Type" );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTempCPLR, "Z_MetaDef", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTempCPLR, "Z_MetaDef", "Name" );

   if ( ( lType >= 0 ) && ( lType <=  zSOURCE_MAX_META ) )
   {
      // Issue a Message, which component is being deleted.
      strcpy_s( szMsg, zsizeof( szMsg ), "Processing delete of " );

      switch ( lType )
      {
         case zSOURCE_GOPGRP_META:
            strcat_s( szMsg, zsizeof( szMsg ), "GlobalOperationGroup" );
            break;

         case zSOURCE_DIALOG_META:
            strcat_s( szMsg, zsizeof( szMsg ), "Dialog" );
            break;

         case zSOURCE_VOR_META:
            strcat_s( szMsg, zsizeof( szMsg ), "Registered View" );
            break;

         case zSOURCE_LOD_META:
            strcat_s( szMsg, zsizeof( szMsg ), "LOD" );
            break;

         case zSOURCE_SA_META:
            strcat_s( szMsg, zsizeof( szMsg ), "SA" );
            break;

         case zSOURCE_ERD_META:
            strcat_s( szMsg, zsizeof( szMsg ), "ERD" );
            break;

         case zSOURCE_DTE_META:
            strcat_s( szMsg, zsizeof( szMsg ), "TE" );
            break;

         case zSOURCE_DOMAINGRP_META:
            strcat_s( szMsg, zsizeof( szMsg ), "DomainGroup" );
            break;

      }

      strcat_s( szMsg, zsizeof( szMsg ), " " );
      strcat_s( szMsg, zsizeof( szMsg ), szName );

      MB_SetMessage( vSubtask, 1, szMsg );

      if ( lType == zSOURCE_VOR_META )
         zltoxa( ulZKey, szName );

      // First, Activate the meta from File
      nRC = LoadFileAndActivateMeta( vSubtask, &vMetaOI, vTZCMFILE, szName,
                                     SRC_CMOD[ lType ].szOD_EXT + 1, lType, TRUE );

      if ( nRC < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Activate failed on load of " );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), ", OD type " );
         strcat_s( szMsg, zsizeof( szMsg ), SRC_CMOD[ lType ].szOD );
         MessageSend( vSubtask, "CM00141", "Configuration Management",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         TraceLineS( "Configuration Management", szMsg );
      }
      else
      {
         // Delete any subordinate metas.
         oTZCMCPRO_DeleteDependentsWithCluster( vTempCPLR, vTZCMFILE, vMetaOI, vKZDBHQUA, vSubtask );
         DropView( vMetaOI );

         // delete the Meta File
         oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, SRC_CMOD[ lType ].szOD_EXT + 1 );
      }
   }

   nRC = DeleteEntity( vTempCPLR, "Z_MetaDef", zREPOS_NONE );

   // Blank out message.
   MB_SetMessage( vSubtask, 1, "" );

   DropView( vTempCPLR );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteMeta( zVIEW vTZCMCPRO,
                      zVIEW vKZDBHQUA,
                      zVIEW vSubtask )
{
   zVIEW    vTempCPLR;
   zVIEW    vMetaOI;
   zULONG   ulZKey;
   zLONG    lType;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;

   CreateViewFromViewForTask( &vTempCPLR, vTZCMCPRO, 0 );

   GetIntegerFromAttribute( &lType, vTempCPLR, "Z_MetaDef", "Type" );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTempCPLR, "Z_MetaDef", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTempCPLR, "Z_MetaDef", "Name" );

   if ( (lType >= 0) && (lType <=  zSOURCE_MAX_META) )
   {
      // Issue a Message, which component is being deleted.
      strcpy_s( szMsg, zsizeof( szMsg ), "Processing delete of " );
      switch ( lType )
      {
         case zSOURCE_GOPGRP_META:
            strcat_s( szMsg, zsizeof( szMsg ), "GlobalOperationGroup" );
            break;

         case zSOURCE_DIALOG_META:
            strcat_s( szMsg, zsizeof( szMsg ), "Dialog" );
            break;

         case zSOURCE_VOR_META:
            strcat_s( szMsg, zsizeof( szMsg ), "Registered View" );
            break;

         case zSOURCE_LOD_META:
            strcat_s( szMsg, zsizeof( szMsg ), "LOD" );
            break;

         case zSOURCE_SA_META:
            strcat_s( szMsg, zsizeof( szMsg ), "SA" );
            break;

         case zSOURCE_ERD_META:
            strcat_s( szMsg, zsizeof( szMsg ), "ERD" );
            break;

         case zSOURCE_DTE_META:
            strcat_s( szMsg, zsizeof( szMsg ), "TE" );
            break;

         case zSOURCE_DOMAINGRP_META:
            strcat_s( szMsg, zsizeof( szMsg ), "DomainGroup" );
            break;
      }

      strcat_s( szMsg, zsizeof( szMsg ), " " );
      strcat_s( szMsg, zsizeof( szMsg ), szName );

      MB_SetMessage( vSubtask, 1, szMsg );

      if ( lType == zSOURCE_VOR_META )
         zltoxa( ulZKey, szName );

      // First, Activate the meta from File
      nRC = LoadFileAndActivateMeta( vSubtask, &vMetaOI, vTempCPLR, szName,
                                     SRC_CMOD[ lType ].szOD_EXT + 1, lType, TRUE );


      if ( nRC < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Activate failed on load of " );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), ", OD type " );
         strcat_s( szMsg, zsizeof( szMsg ), SRC_CMOD[ lType ].szOD );
         MessageSend( vSubtask, "CM00141", "Configuration Management",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         TraceLineS( "Configuration Management", szMsg );
      }
      else
      {
         // Delete any subordinate metas.
         oTZCMCPRO_DeleteDependents( vTempCPLR, vMetaOI, vKZDBHQUA, vSubtask );

         // Delete the meta file itself.
         // Lastly, Delete the Meta File
         nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, SRC_CMOD[ lType ].szOD_EXT + 1 );
         DropView( vMetaOI );
      }
   }

   nRC = DeleteEntity( vTempCPLR, "Z_MetaDef", zREPOS_NONE );
   nRC = CommitObjectInstance( vTempCPLR );

   // Blank out message.
   MB_SetMessage( vSubtask, 1, "" );

   DropView( vTempCPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteDependents( zVIEW vTZCMCPRO,
                            zVIEW vMetaOI,
                            zVIEW vKZDBHQUA,
                            zVIEW vSubtask )
{
   // For each meta type, this routine deletes any associated files.

   zVIEW    vTempCPLR;
   zLONG    lType;
   zULONG   ulZKey;
   zSHORT   nRC;
   zCHAR    szZKey[12];
   zCHAR    szName[ 33 ];
   zCHAR    szExtension[ 4 ];

   GetIntegerFromAttribute( &lType, vTZCMCPRO, "Z_MetaDef", "Type" );
   CreateViewFromViewForTask( &vTempCPLR, vTZCMCPRO, 0 );

   // Process each special meta type.

   switch ( lType )
   {
      case zSOURCE_PENV_META:

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "PresEnvDef", "Name" );
         nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, "XPE" );

         break;

      case zSOURCE_GOPGRP_META:

         // A Global Operation Group needs to delete the associated source
         // file.

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "GlobalOperationGroup", "Name" );
         GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "GlobalOperationGroup", "Extension" );
         nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, szExtension );

         break;

      case zSOURCE_DIALOG_META:

         // A Dialog needs to delete any associated source files and the XWD.

         for ( nRC = SetCursorFirstEntity( vMetaOI, "SourceFile", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vMetaOI, "SourceFile", 0 ) )
         {
            GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "SourceFile", "Name" );
            GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "SourceFile", "Extension" );
            nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, szExtension );
         }

         GetStringFromAttribute( szName, zsizeof( szName ),vMetaOI, "Dialog", "Tag" );
         nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, "XWD" );

         break;

      case zSOURCE_VOR_META:
         break;

      case zSOURCE_LOD_META:

         // A LOD needs to delete any associated source files, the XOD and
         // any VOR metas.

         for ( nRC = SetCursorFirstEntity( vMetaOI, "SourceFile", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vMetaOI, "SourceFile", 0 ) )
         {
            GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "SourceFile", "Name" );
            GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "SourceFile", "Extension" );
            nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, szExtension );
         }

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "LOD", "Name" );
         nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, "XOD" );

         // To delete associated VORs, we will have to activate every VOR
         // and check its LOD for a match on name.
         for ( nRC = SetCursorFirstEntityByInteger( vTempCPLR, "Z_MetaDef", "Type",
                                                    zSOURCE_VOR_META, 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vTempCPLR, "Z_MetaDef", "Type",
                                                    zSOURCE_VOR_META, 0 ) )
         {

            GetIntegerFromAttribute( (zPLONG) &ulZKey, vTempCPLR, "Z_MetaDef", "ZKey" );
            zltoxa( ulZKey, szZKey );

            // Load the meta from repository file
            nRC = LoadFileAndActivateMeta( vSubtask,
                                           &vMetaOI,  // return meta
                                           vTempCPLR, // CPLR object
                                           szZKey,    // file name
                                           "PVR",     // extension
                                           zSOURCE_VOR_META, // OD type
                                           TRUE ); // delete file from temp dir


            if ( nRC >= 0 )
            {
               if ( CheckExistenceOfEntity( vMetaOI, "LOD" ) >= zCURSOR_SET &&
                    CompareAttributeToString( vMetaOI, "LOD", "Name", szName ) == 0 )
               {
                  DeleteEntity( vTempCPLR, "Z_MetaDef", zREPOS_NONE );
                  oTZCMCPRO_DeleteServerFile( vTempCPLR, szZKey, "PVR" );
               }
               DropView( vMetaOI );
            }
         }

         break;

      case zSOURCE_SA_META:
         break;

      case zSOURCE_ERD_META:

         // An ERD needs to delete the associated DTE and any SAs.

         nRC = SetCursorFirstEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_DTE_META, 0 );
         if ( nRC >= zCURSOR_SET )
         {
            oTZCMCPRO_DeleteMeta( vTempCPLR, vKZDBHQUA, vSubtask );
         }

         for ( nRC = SetCursorFirstEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_SA_META, 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vTempCPLR, "Z_MetaDef", "Type", zSOURCE_SA_META, 0 ) )
         {
            oTZCMCPRO_DeleteMeta( vTempCPLR, vKZDBHQUA, vSubtask );
         }

         break;

      case zSOURCE_DTE_META:
         break;

      case zSOURCE_DOMAINGRP_META:

         // A Domain Group needs to delete the associated source file.

         GetStringFromAttribute( szName, zsizeof( szName ), vMetaOI, "DomainGroup", "Name" );
         GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMetaOI, "DomainGroup", "Extension" );
         if ( szExtension[ 0 ] )
            nRC = oTZCMCPRO_DeleteServerFile( vTempCPLR, szName, szExtension );
         break;

   }

   DropView( vTempCPLR );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Delete a File from the CPLR at the Server
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteServerFile( zVIEW  vTZCMCPRO,
                            zPCHAR szName,
                            zPCHAR szExtension )
{
   // First Delete the file at the CPLR and then remove it from the CPLR
   // object. If a failure were to occur between the two commits, we could
   // have a CPLR entry with no corresponding file, but we will treat this
   // the same as having no file. It is better than have a File object
   // instance on the server that we don't know about.

   zVIEW  vFile;
   zVIEW  vTempCPLR;
   zSHORT nFound;
   zSHORT nRC;
   zLONG  lZKey;

   // Position on File in CPLR object.
   CreateViewFromViewForTask( &vTempCPLR, vTZCMCPRO, 0 );
   nFound = 0;
   nRC = SetCursorFirstEntityByString( vTempCPLR, "File", "Name", szName, 0 );
   while ( nFound == 0 && nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vTempCPLR, "File", "Extension", szExtension ) == 0 )
         nFound = 1;
      else
         nRC = SetCursorNextEntityByString( vTempCPLR, "File", "Name", szName, 0 );
   }

   // If found, proceed with delete logic.
   if ( nFound == 1 )
   {
      GetIntegerFromAttribute( &lZKey, vTempCPLR, "File", "ZKey" );
      nRC = oTZCMLPLO_ActivateFileOI( &vFile, vTempCPLR, lZKey );
      if ( nRC >= 0 )
      {
         // to fix bug TB471, we relink the FILE to TZCMCPRO.
         //  so, by deleting FILE, the TZCMCPRO object would get the
         //  information that it was deleted and nothing has to be
         //  done, when TZCMCPRO is committed.
         RelinkInstanceToInstance( vFile, "File", vTempCPLR, "File" );
         // Delete the File object.
         DeleteEntity( vFile, "File", zREPOS_NONE );
         nRC = CommitObjectInstance( vFile );
      }
      else
      {
         // if the file was already deleted, we want to
         //  remove it from the TZCMCPRO object.
         ExcludeEntity( vTempCPLR, "File", zREPOS_NONE );
      }
   }

   DropView( vTempCPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LoadFileAndActivateMeta( zVIEW  vSubtask,
                         zPVIEW pvMeta,         // return meta
                         zVIEW  vTZCMCPRO,      // CPLR object
                         zPCHAR szName,         // file name
                         zPCHAR szExtension,    // extension
                         zLONG  lType,          // OD type
                         zBOOL  bDeleteFile )   // delete file from temp dir
{
   // First Delete the file at the CPLR and then remove it from the CPLR
   // object. If a failure were to occur between the two commits, we could
   // have a CPLR entry with no corresponding file, but we will treat this
   // the same as having no file. It is better than have a File object
   // instance on the server that we don't know about.

   zSHORT nRC;
   zCHAR szTempName[ 256 ];

   // First get temp directory to store the file
   if ( zgGetZeidonToolsDir( vSubtask, szTempName, zAPPL_DIR_LOCAL ) != 0 )
   {
      *pvMeta = NULL;
      return( -1 );
   }

   // create the file
   strcat_s( szTempName, zsizeof( szTempName ), szName );
   strcat_s( szTempName, zsizeof( szTempName ), ".tmp" );
   nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szTempName, vTZCMCPRO, szName, szExtension );
   if ( nRC < 0 )
   {
      *pvMeta = 0;

      // Delete the file, if it exists.
      SysOpenFile( vTZCMCPRO, szTempName, COREFILE_DELETE );
      return( -2 );
   }

   // Activate the Meta from the file.
   nRC = ActivateOI_FromFile( pvMeta, SRC_CMOD[ lType ].szOD, // View OD name
                              vTZCMCPRO, szTempName, zSINGLE | zIGNORE_ATTRIB_ERRORS );

   if ( bDeleteFile )
      SysOpenFile( vTZCMCPRO, szTempName, COREFILE_DELETE ); // delete the file

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZCMRCRO_DeriveCPLR_CPL_Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DERIVED ATTRIBUTE */  OPERATION
oTZCMRCRO_DeriveCPLR_CPL_Name( zVIEW         vTZCMRCRO  /*  BASED ON LOD TZCMRCRO  */,
                               LPVIEWENTITY  lpViewEntity,
                               LPVIEWATTRIB  lpViewAttrib,
                               zSHORT        nGetOrSetFlag )
{
   zCHAR   szCPLR_CPL_Name[ 76 ];
   zULONG  ulZKey;
   zVIEW   vTZCMCPL;
   zVIEW   vTempCPL;

   if ( nGetOrSetFlag == zDERIVED_GET )
   {
      if ( GetViewByName( &vTZCMCPL, "TZCMCPL", vTZCMRCRO, zLEVEL_TASK ) < 0 )
         return( -1 );

      CreateViewFromViewForTask( &vTempCPL, vTZCMCPL, 0 );

      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMRCRO, "CPLR", "ZKey" );
      SetCursorFirstEntityByInteger( vTempCPL, "CPLR", "ZKey", (zLONG) ulZKey, "Installation" );
      GetStringFromAttribute( szCPLR_CPL_Name, zsizeof( szCPLR_CPL_Name ), vTempCPL, "CPLR", "Name" );
      strcat_s( szCPLR_CPL_Name, zsizeof( szCPLR_CPL_Name ), "  CPL: " );
      GetStringFromAttribute( szCPLR_CPL_Name + zstrlen( szCPLR_CPL_Name ), zsizeof( szCPLR_CPL_Name ) - zstrlen( szCPLR_CPL_Name ), vTempCPL, "CPL", "Name" );

      StoreValueInRecord( vTZCMRCRO, lpViewEntity, lpViewAttrib, szCPLR_CPL_Name, 0 );

      DropView( vTempCPL );
   }

   return( 0 );

} // oTZCMRCRO_DeriveCPLR_CPL_Name


///////////////////////////// end of file  //////////////////////////////////
