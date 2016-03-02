/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmlplo.c - Zeidon application operations
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
// LAST MESSAGE ID: CM00320
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2001.07.06 BL  F51262, R54476, F51302
   maintained Repository, part 2, improve Handling for Domains and Global
   Operations

2001.04.23 BL  Repository R54834, F50813, F50621, R54671
   Fix für CheckIn from Siron Reports ( read and write REP File from/to
   MetaSrcDir, not from ExecDir ).

2000.12.12 DKS
   Corrected casting

2000.12.06 BL
   Bugfix Handling from Global Operations and Domains

2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.23  SR  Z2000  Size of path variables
   - Modified size of of all filename and path variables in whole module
     because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
     been changed to 254 in datamodel.
   - Adjust the size of messagetext variables.
   - Replace size of extension arrays (4)  with zMAX_EXTENSION_LTH
   - Remove function call GetStringFromAttribute in function oTZCMLPLO_UpdateDomain
     because the result string is never used in the function.
   - Remove second declaration of funciton zwfnTZCMLPLO_GetLPLR_Directories.


2000.05.18  BL    Bug 51342
   Remove Comments
   Modified oTZCMLPLO_CheckInMetas for replace CommitMetaOI through
   CommitOI_ToFile ( do not build XOD by check in)

2000.05.11  BL
   Modified oTZCMLPLO_CheckInMetas for Bugfix LRP.

2000.04.03  HH
   remove checkin of meta (CommitObjectInstance) on delete.

2000.03.16  HH
   if initial checkin fails, reset the state of the meta, so that it is not
   deleted on a following checkin by refresh.

2000.03.13  HH
   call ActivateOI_FromFile with zIGNORE_ATTRIB_ERRORS, needed for refresh
   of old (9j) LPLR's.

2000.02.24  BL
   Modified oTZCMLPLO_CheckInMetas for commit LPLR after Commit ViewCluster
   Remove View vTZCMLPLO_Compare, is View is not in use
   Added oTZCMLPLO_RefreshPartial, oTZCMLPLO_RefreshPartialFromCPLR,
   oTZCMLPLO_Create_RootEntity, oTZCMLPLO_DeleteSelectedMeta,
   oTZCMLPLO_CheckDuplicateERD_TE_PPE for refresh LPLR user defined
   Replace LPLR -> Project
   Added oTZCMLPLO_UpdateDomains, oTZCMLPLO_ERD_Refreshed,
   oTZCMLPLO_RefreshFinish and oTZCMLPLO_CheckUpdateFlags: reuse code
   for refresh user defined

2000.02.23  DKS
   Remove use of static functions so we know when a name collision exists.

2000.02.22  Bl
   Maintain sequence number for entity "Buffer".
   Added zwTZCMLPLO_LoadRefreshListForCPLR and new Parameter in
   zwTZCMLPLO_RefreshReusableCPLRs, zwfnTZCMLPLO_CheckUpdateFromRepo
   for reuse code for window "Refresh User defined"

2000.02.17  BL
   Added zwfnTZCMLPLO_Create_W_MetaType, zwfnTZCMLPLO_AddMetaToLPLR,
   zwfnTZCMLPLO_CheckTypeForAddMeta, zwfnTZCMLPLO_UpdateMetaFromCPLR,
   zwfnTZCMLPLO_DeleteMeta, zwfnTZCMLPLO_DeleteDomainsAndOperations,
   zwTZCMLPLO_CheckDeleteFromRepo, zwfnTZCMLPLO_UpdateMeta,
   zwTZCMLPLO_WriteReportView, zwfnTZCMLPLO_GetLPLR_Directories,
   zwfnTZCMLPLO_OverwriteMetaFromCPLR, zwfnTZCMLPLO_DeleteMetaInLPLR
   for move Code from zwfnTZCMLPLO_RefreshFromCPLR in new functions for reuse
   Added zwfnTZCMLPLO_CheckDuplicateNames, zwfnTZCMLPLO_CheckDuplicateComponents
   and modified zwfnTZCMLPLO_RefreshFromCPLR: Do not automatic refresh for
   component with equal names, equal types and different ZKeys
   Drop DB_Mode

2000.02.16  HH
   remove atomic repository database
   drop DB_Mode

2000.01.25   BL  Z10  Repository
   Modified oTZCMLPLO_CheckInMetas:
   if no delete Componenet, then aborting CheckIn
   if DataModel or TE not updated, then no MessagePrompt for copy XOD's

2000.01.20   BL  Z10  Repository
   Modified oTZCMLPLO_CheckInMetas: Meta is not checked out by this User,
   then Return -2

2000.01.14   BL  Z10  Repository
   Modified oTZCMLPLO_CheckInMetas for insert new ViewCluster for shadow file;
   without this ViewCluster Zeidon save the shadow file with create flag in
   repository

1999.12.23   BL  Z10  Repository
   Modified Operations oTZCMLPLO_CheckInMetas: insert ViewCluster
   Insert Parameter ViewCluster by ofnTZCMLPLO_SironReport_Files,
   ofnTZCMLPLO_CopyX_File, oTZCMLPLO_CopyFileFromServerFile,
   ofnTZCMLPLO_CopyHDR_File, ofnTZCMLPLO_CopySourceFiles

from here old Version

10.04.1997  DonC
   Corrected fnPostRefreshLPLR so that Global Operation entries and Domain
   entries were only added to LPLR if they were not already there.

15.06.1997  DonC
   Made modifications for handling of Report metas on check in and refresh
   and for handling of Zeidon & Siron executables, XRP, REP and XSQ.

08.10.1997  DonC
   Modified fnPostRefreshLPLR to return 0 instead of nRC so that Refresh
   process of meta would continue even if a SourceFile entry did not
   have a corresponding real file to copy.  There is no reason to return
   a negative return code and abort building executables because of a
   problem in PostRefresh.

11.10.1997  DonC
   Modified RefreshLPLR to ask the operator if he wants to refresh XODs
   when an ER or TE is refreshed.  Also modified CheckInMetas to ask the
   reverse question and take reverse action on CheckIn.

11.10.1997  DonC
   Modified CheckInMetas to make sure that all ER_Attribute entities have a
   subordinate Domain entity.

29.10.1997  DonC
   Modified CheckInMetas to use CommitMetaOI call to write metas to LPLR
   so that relinking will pick up dependent meta with correct create flags.

04.11.1997  DonC
   Added DIL message around refreshing XODs from server and sending XODs to
   server on Check In.

04.11.1997  DonC
   Modified RefreshLPLR to abort if an error occurred activating reusable
   CPLR.

10.11.1997  DonC
   Modified CheckInMetas for check in of ERD to delete the cashed copy of
   the ERD in CM because the resulting ActivateOI_FromOI_ForTask did not copy the OI
   flags (such as "OI updated"). Note that the code has a comment that this is
   a "temporary hack" because we probably want to change ActivateOI_FromOI_ForTask to
   copy the OI flags, but at this point it is safer to change this module
   than it is to change core.

16.11.1997  DonC
   Made several modifications to correctly delete metas during refresh
   processing. The modifications included setting a new attribute,
   RepositoryCPLR_ZKey, during checkin. This allowed refresh processing
   to distinguish between an LPLR entry that was new and thus not on the
   Repository from one that had existed but was deleted from the Repository.

21.11.1997  DonC
   Modified CheckInMetas to use CommitOI_ToFile for meta PPE instead of
   CommitMetaOI since the latter caused an error message to be given.

05.01.1998  DonC
   Modified oTZCMLPLO_DeleteServerFile to also remove File entry from
   CPLR.

05.01.1998  DonC
   Moved oTZCMLPLO_DeleteServerFile to TZCMCPRO.C and renamed to operation
   oTZCMCPRO_DeleteServerFile.

18.02.1998  DonC
   Modified use of KZDBHQUA object to not reuse a named view but create it
   empty each time.

10.03.1998  DonC
   Corrected bug in copy XODs that used incorrect view, vTZCMLPLO, instead
   of view, vTemp.

29.05.1998  DonC
   Moved the Operator prompt code for copying XODs up at the beginning of
   check in processing so that user wouldn't be forced to wait for LOD check
   in before having to respond to the prompt. (QS 10)

1998/12/04  DC
   Modified oTZCMLPLO_DeleteMetas so that "set cursor" on the entry in the
   "check in" list uses scoping entity. The prior set cursor didn't work
   when there were deletes of more than one type of meta. Also changed the
   SetCursor on the loop to use "First" instead of "Next" because the
   CommitLPLR was reordering the OI. (TB 391)

26.01.1999  HH
   TB471: in ActivateFileOI activate the FILE object under the same
   Application as TZCMCPRO, so that the File entities can be linked.

22.02.1999  HH
   Improve prev. change, do not use zLEVEL_APPLICATION, so it works both
   under workstation and system administration.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"

/* local prototypes */
zSHORT
oTZCMLPLO_CheckDuplicateERD_TE_PPE( zVIEW   vSubtask,
                                    zVIEW   vTZCMLPLO,
                                    zVIEW   vTZCMCPRO,
                                    zLONG   Type,
                                    zPULONG pulZKeyLPLR );

zSHORT
oTZCMLPLO_DeleteSelectedMeta( zVIEW  vSubtask,
                              zPVIEW vTZCMRPTO,
                              zVIEW  vTZCMRPTO_List,
                              zVIEW  vTZCMLPLO,
                              zVIEW  vTZCMCPRO );

zSHORT
oTZCMLPLO_Create_RootEntity( zPVIEW  vTZCMRPTO,
                             zVIEW   vTZCMRPTO_List,
                             zVIEW   vTZCMCPRO );

zSHORT
oTZCMLPLO_RefreshFinish( zVIEW  vSubtask,
                         zVIEW  vTZCMLPLO,
                         zVIEW  vTZCMCPRO,
                         zVIEW  vTZCMRPTO,
                         zSHORT DomainsUpdated,
                         zSHORT ERD_Refreshed );

zSHORT
oTZCMLPLO_CheckUpdateFlags( zSHORT  nRC,
                            zPSHORT ERD_Refreshed,
                            zPSHORT DomainsUpdated );

zSHORT
oTZCMLPLO_ERD_Refreshed( zVIEW   vSubtask,
                         zVIEW   vTZCMLPLO,
                         zVIEW   vTZCMCPRO );

zSHORT
oTZCMLPLO_UpdateDomains( zVIEW   vSubtask,
                         zVIEW   vTZCMLPLO );

zSHORT
zwfnTZCMLPLO_CheckUpdateFromRepo( zVIEW  vTZCMCPRO,
                                  zVIEW  vTZCMLPLO,
                                  zPCHAR szPrevLPLR_LastRefreshDate );

zSHORT
zwfnTZCMLPLO_CheckDuplicateComponents( zVIEW vTZCMLPLO,
                                       zVIEW vTZCMCPRO );
zSHORT
zwfnTZCMLPLO_CheckDuplicateNames( zVIEW   vTZCMLPLO,
                                  zVIEW   vTZCMCPRO,
                                  zPULONG ulZKeyLPLR,
                                  zBOOL   bDelete );
zSHORT
zwfnTZCMLPLO_Create_W_MetaType( zPVIEW  vTZCMLPLO,
                                zLONG   Type );
zSHORT
zwfnTZCMLPLO_CheckTypeForAddMeta( zVIEW   vSubtask,
                                  zVIEW   vTZCMCPRO,
                                  zLONG   Type,
                                  zPCHAR  szName,
                                  zPCHAR  szL_MetaSrcDir,
                                  zPCHAR  szL_ExecDir,
                                  zPSHORT ERD_Refreshed,
                                  zPSHORT DomainsUpdated );
zSHORT
zwfnTZCMLPLO_GetLPLR_Directories( zVIEW   vTZCMLPLO,
                                  zPCHAR  szL_MetaSrcDir,
                                  zPCHAR  szL_ExecDir );

zSHORT
zwfnTZCMLPLO_DeleteDomainsAndOperations( zVIEW  vTZCMLPLO,
                                         zLONG   Type );
zSHORT
zwfnTZCMLPLO_DeleteMetaInLPLR( zVIEW vSubtask, zPVIEW vTZCMLPLO );
zSHORT
zwfnTZCMLPLO_OverwriteMetaFromCPLR( zVIEW   vSubtask,
                                    zPVIEW  vTZCMLPLO,
                                    zULONG  ulZKeyLPLR );
zSHORT
zwfnTZCMLPLO_AddMetaToLPLR( zVIEW   vSubtask,
                            zVIEW   vTZCMCPRO,
                            zPVIEW  vTZCMLPLO,
                            zPVIEW  vTZCMRPTO,
                            zULONG  ulZKey,
                            zLONG   Type,
                            zPCHAR  szTimestamp,
                            zPSHORT ERD_Refreshed,
                            zPSHORT DomainsUpdated );
zSHORT
zwfnTZCMLPLO_UpdateMetaFromCPLR( zVIEW   vSubtask,
                                 zVIEW   vTZCMCPRO,
                                 zPVIEW  vTZCMLPLO,
                                 zULONG  ulZKey,
                                 zLONG   Type,
                                 zPCHAR  szTimestamp,
                                 zPSHORT ERD_Refreshed,
                                 zPSHORT DomainsUpdated );
zSHORT
zwfnTZCMLPLO_DeleteMeta( zVIEW   vSubtask,
                         zVIEW   vTZCMCPRO,
                         zPVIEW  vTZCMLPLO,
                         zPVIEW  vTZCMRPTO );
zSHORT
zwfnTZCMLPLO_UpdateMeta( zVIEW   vSubtask,
                         zVIEW   vTZCMCPRO,
                         zPVIEW  vTZCMLPLO,
                         zPVIEW  vTZCMRPTO,
                         zULONG  ulZKey,
                         zLONG   Type,
                         zPCHAR  szTimestamp,
                         zPSHORT ERD_Refreshed,
                         zPSHORT DomainsUpdated,
                         zPCHAR  szPrevLPLR_LastRefreshDate );
zSHORT
zwfnTZCMLPLO_RefreshFromCPLR( zVIEW  vSubtask,
                              zVIEW  vTZCMLPLO,
                              zVIEW  vTZCMCPRO,
                              zPCHAR szTimestamp,
                              zPCHAR szPrevLPLR_LastRefreshDate );
zSHORT
ofnTZCMLPLO_CopyMetaToLPLR( zVIEW  vSubtask,
                            zVIEW  vTZCMCPRO,
                            zVIEW  vTZCMLPLO,
                            zPCHAR szL_MetaSrcDir,
                            zPCHAR szName,
                            zLONG  Type);
zSHORT
ofnTZCMLPLO_CopyX_File( zVIEW   vSubtask,
                        zVIEW   vTZCMCPRO,
                        zPCHAR  szL_MetaSrcDir,
                        zPCHAR  szL_ExecDir,
                        zPCHAR  szName,
                        zLONG   Type,
                        zSHORT  Direction,
                        zLONG   ViewCluster);

zSHORT
zwTZCMLPLO_CheckDeleteFromRepo( zPVIEW  vTZCMLPLO,
                                zULONG  ulZKey,
                                zLONG   Type );

zOPER_EXPORT zSHORT OPERATION
zwTZCMLPLO_RefreshReusableCPLRs( zVIEW   vSubtask,
                                 zVIEW   vTZCMLPLO, zVIEW vTZCMCPRO,
                                 zPCHAR  szTimestamp,
                                 zPCHAR  szPrevLPLR_LastRefreshDate,
                                 zBOOL   bRefreshLPLR);

zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_OrderLPLR( zPVIEW vOrderedLPLR, zVIEW vTZCMLPLO );

zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_DeleteMetas( zVIEW vSubtask,
                       zVIEW vTZCMLPLO,
                       zVIEW vTZCMCPRO,
                       zVIEW vTZCM_ChkInList );

zOPER_EXPORT zSHORT OPERATION
oTZCMCPLO_DeleteDependents( zVIEW vTZCMCPRO,
                            zVIEW vMetaOI,
                            zVIEW vKZDBHQUA,
                            zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_CopyHDR_File( zVIEW vSubtask,zVIEW vTZCMCPRO, zVIEW vTZCMLPLO,
                          zVIEW vHdrMeta, zSHORT Direction,
                          zLONG lViewCluster );

zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_CopySourceFiles( zVIEW  vSubtask,
                             zVIEW  vTZCMCPRO,
                             zVIEW  vTZCMLPLO,
                             zVIEW  vMeta,
                             zPCHAR pszSourcFileEntityName,
                             zSHORT Direction,
                             zLONG  lViewCluster );

zSHORT
zwTZCMLPLO_WriteReportView( zVIEW   vSubtask,
                            zVIEW   vTZCMCPRO,
                            zPVIEW  vTZCMRPTO,
                            zPCHAR  szMsg,
                            zPCHAR  szName,
                            zSHORT  nPos );

zSHORT
zwTZCMLPLO_WriteReportView1( zVIEW   vSubtask,
                             zVIEW   vTZCMLPLO,
                             zPVIEW  vTZCMRPTO,
                             zPCHAR  szMsg,
                             zSHORT  nPos );

zSHORT
oTZCMLPLO_RefreshPartialFromCPLR( zVIEW  vSubtask,
                                  zVIEW  vTZCMLPLO,
                                  zVIEW  vTZCMCPRO,
                                  zVIEW  vTZCMRPTO_List,
                                  zPCHAR szTimestamp,
                                  zPCHAR szPrevLPLR_LastRefreshDate );

static zSHORT
fnPostRefreshGlobalOp( zVIEW vMetaOI, zVIEW vTaskLPLR );
static zSHORT
fnPostRefreshDomain( zVIEW vMetaOI, zVIEW vTaskLPLR );



// Rebuild the Domains.XDM file if Domains were refreshed.
// To do this we need to loop through the DomainGroup metas and then all
// the Domains in the group, issuing the rebuild for each Domain.
zSHORT
oTZCMLPLO_UpdateDomains( zVIEW   vSubtask,
                         zVIEW   vTZCMLPLO )
{
   zCHAR    szFileName[ 2*zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW    vMeta;
   zSHORT   nRC;

   zwfnTZCMLPLO_GetLPLR_Directories( vTZCMLPLO, szL_MetaSrcDir, szL_ExecDir );
   MB_SetMessage( vSubtask, 1, "Updating Domains." );

   SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType",
                                  "Type", zREFER_DOMAINGRP_META, 0 );

   strcpy_s( szFileName, zsizeof( szFileName ), szL_ExecDir );
   strcat_s( szFileName, zsizeof( szFileName ), "ZEIDON.XDM" );

   // Loop through each of the meta defs (DomainGroups).  For each one,
   // activate the portable file containing the OI and process each Domain.
   nRC = SetCursorFirstEntity( vTZCMLPLO, "W_MetaDef", 0 );
   vMeta = 0;      // Indicate no current meta.
   while ( nRC == zCURSOR_SET )
   {
      // Construct the name of the portable file containing the domain group.
      strcpy_s( szFileName, zsizeof( szFileName ), szL_MetaSrcDir );
      GetStringFromAttribute( szFileName + zstrlen( szFileName ), zsizeof( szFileName ) - zstrlen( szFileName ),
                              vTZCMLPLO, "W_MetaDef", "Name" );
      strcat_s( szFileName, zsizeof( szFileName ), SRC_CMOD[ zSOURCE_DOMAINGRP_META ].szOD_EXT );

      // Drop the prior meta, if there is one.
      if ( vMeta > 0 )
         DropObjectInstance( vMeta );

      // Activate the domain group.
      ActivateOI_FromFile( &vMeta, SRC_CMOD[ zSOURCE_DOMAINGRP_META ].szOD,
                           vTZCMLPLO, szFileName,
                           zSINGLE | zIGNORE_ATTRIB_ERRORS);

      // Now loop through each of the domains in the domain group and add them to the .XDM,
      nRC = SetCursorFirstEntity( vMeta, "Domain", 0 );
      while ( nRC == zCURSOR_SET )
      {
         // Request building the Domain in the XDM, without writing the XDM to file.
         oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, vMeta, 0 );

         nRC = SetCursorNextEntity( vMeta, "Domain", 0 );
      }

      nRC = SetCursorNextEntity( vTZCMLPLO, "W_MetaDef", 0 );

   } // For all W_MetaDef entities.

   // At the end, rebuild the last Domain again, this time requesting
   // the XDM to be written out.  Then drop the Domain Group.
   oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, vMeta, 1 );
   DropObjectInstance( vMeta );

   MB_SetMessage( vSubtask, 1, 0 );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
// If either the ER or the TE has been refreshed, we will ask the operator
// if he wants to refresh ALL XODs from the server EXCEPT for those LODs
// that are checked out by the local work station.  If the operator answers
// yes, we will then loop through all LODs not checked out and copy those
// XODs from the server.
/////////////////////////////////////////////////////////////////////////////
zSHORT
oTZCMLPLO_ERD_Refreshed( zVIEW   vSubtask,
                         zVIEW   vTZCMLPLO,
                         zVIEW   vTZCMCPRO )
{
   zCHAR    szName[ 33 ];
   zSHORT   nRC;
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];

   if ( MessagePrompt( vSubtask, "CM00317",
                       "Configuration Management",
                       "Either the ER or TE has just been refreshed to your"
                       " Project.  Do you want the executables (ie., XODs)"
                       " for all LODs on the Repository that are not checked out"
                       " by you to be refreshed to your Project?",
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_YES,      0 )  == zRESPONSE_YES )
   {
      MB_SetMessage( vSubtask, 1, "Refreshing XODs" );
      GetStringFromAttribute( szL_ExecDir, zsizeof( szL_ExecDir ), vTZCMLPLO, "LPLR", "ExecDir" );
      ofnTZCMWKSO_AppendSlash( szL_ExecDir );
      GetStringFromAttribute( szL_MetaSrcDir, zsizeof( szL_MetaSrcDir ), vTZCMLPLO, "LPLR", "MetaSrcDir" );
      ofnTZCMWKSO_AppendSlash( szL_MetaSrcDir );

      nRC = SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type",
                                           ( zLONG ) zREFER_LOD_META, 0 );
      nRC = SetCursorFirstEntity( vTZCMLPLO, "W_MetaDef", 0 );
      while ( nRC == zCURSOR_SET )
      {
         // If the LOD is not checked out to this LPLR (Status != 1),
         // then copy its associated XOD.
         if ( CompareAttributeToInteger( vTZCMLPLO, "W_MetaDef", "Status",
                                         1 ) != 0 )
         {
            GetStringFromAttribute( szName, zsizeof( szName ), vTZCMLPLO, "W_MetaDef", "Name" );
            nRC = ofnTZCMLPLO_CopyX_File( vSubtask, vTZCMCPRO,
                                          szL_MetaSrcDir, szL_ExecDir,
                                          szName, zSOURCE_LOD_META, 1, 0 );
         }

         nRC = SetCursorNextEntity( vTZCMLPLO, "W_MetaDef", 0 );
      }

      MB_SetMessage( vSubtask, 1, "Refreshing XODs finished" );
      MessageSend( vSubtask, "CM00318", "Configuration Management",
                   "All executable LODs (XODs) have been refreshed to your Project"
                   " except for the LODs you currently have checked out.\n"
                   "You should save the LODs that you have checked out with the"
                   " current TE before executing applications.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
      MessageSend( vSubtask, "CM00319", "Configuration Management",
                   "The only executable LODs (XODs) refreshed to your Project"
                   " are those that correspond to a LOD that was refreshed to "
                   "your Project.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
}

zSHORT
oTZCMLPLO_CheckUpdateFlags( zSHORT  nRC,
                            zPSHORT ERD_Refreshed,
                            zPSHORT DomainsUpdated )
{
   switch ( nRC )
   {
      case 1:
         *DomainsUpdated = 1;
         break;
      case 2:
         *ERD_Refreshed = 1;
         break;
      case 3:
         *DomainsUpdated = 1;
         *ERD_Refreshed = 1;
         break;
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Refresh the LPLR whose view is passed
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_RefreshLPLR( zVIEW vSubtask,
                       zVIEW vTZCMLPLO,
                       zVIEW vTZCMCPRO )
{
   zVIEW    vDialog;
   zVIEW    vTZCMRPTO;
   zSHORT   ERD_Refreshed = 0;
   zSHORT   DomainsUpdated = 0;
   zCHAR    szTimestamp[ 33 ];
   zCHAR    szPrevLPLR_LastRefreshDate[ 33 ];
   zSHORT   nRC;

   // Begin Refresh process
   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );
   MB_SetMessage( vDialog, 1, "Refreshing Project" );

   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   GetStringFromAttribute( szPrevLPLR_LastRefreshDate, zsizeof( szPrevLPLR_LastRefreshDate ), vTZCMLPLO,
                           "LPLR", "LastRefreshDate" );
   SetAttributeFromString( vTZCMLPLO, "LPLR", "LastRefreshDate", szTimestamp );

   // TZCMRPTO is used to build a Refresh Report
   nRC = ActivateEmptyObjectInstance( &vTZCMRPTO, "TZCMRPTO",
                                      vTZCMLPLO, zMULTIPLE );
   SetNameForView( vTZCMRPTO, "TZCMRPTO", vTZCMLPLO, zLEVEL_TASK );

   // Frank H. (21/04/97) first Refresh the ReusableCPLRs then the rest.
   if ( SetCursorFirstEntity( vTZCMCPRO, "ReusableCPLR", 0 ) == zCURSOR_SET )
   {
      // 4rd Parameter = TRUE means: RefreshLPLR (FALSE = Load Refresh List
      //                             for Window "Refresh User defined")
      nRC = zwTZCMLPLO_RefreshReusableCPLRs( vSubtask, vTZCMLPLO, vTZCMCPRO,
                                             szTimestamp,
                                             szPrevLPLR_LastRefreshDate, TRUE );
      if ( nRC < 0 )
         return( -1 );
      else
         oTZCMLPLO_CheckUpdateFlags( nRC, &ERD_Refreshed, &DomainsUpdated );
   }

   nRC = zwfnTZCMLPLO_RefreshFromCPLR( vSubtask, vTZCMLPLO, vTZCMCPRO,
                                       szTimestamp,
                                       szPrevLPLR_LastRefreshDate );
   if ( nRC < 0 )
      return( -1 );
   else
      oTZCMLPLO_CheckUpdateFlags( nRC, &ERD_Refreshed, &DomainsUpdated );

   GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK );

   oTZCMLPLO_RefreshFinish( vDialog, vTZCMLPLO, vTZCMCPRO, vTZCMRPTO,
                            DomainsUpdated, ERD_Refreshed );

   return( 0 );
}

zSHORT
zwfnTZCMLPLO_CheckDuplicateComponents( zVIEW vTZCMLPLO,
                                       zVIEW vTZCMCPRO )
{
   zCHAR    szMsg[ 256 ];
   zCHAR    szName[ 33 ];

   //exists another ERD in LPLR?
   if ( CheckDuplicateComponents( vTZCMLPLO, vTZCMCPRO,
                                  zSOURCE_ERD_META, szName ) < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "An other Data Model already \nexists in your Project." );
      strcat_s( szMsg, zsizeof( szMsg ), "\n\nRefresh Processing aborted!" );
      MessageSend( vTZCMLPLO, "CM00702", "Configuration Management",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   //exists a other TE in LPLR?
   if ( CheckDuplicateComponents( vTZCMLPLO, vTZCMCPRO, zSOURCE_DTE_META, szName ) < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "An other Technical Environment \nalready exists in " );
      strcat_s( szMsg, zsizeof( szMsg ), "your Project.\n\nRefresh Processing aborted!" );
      MessageSend( vTZCMLPLO, "CM00702", "Configuration Management",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   //exists a other PPE in LPLR?
   if ( CheckDuplicateComponents( vTZCMLPLO, vTZCMCPRO,
                                  zSOURCE_PENV_META, szName ) < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "An other Presentation Environment \nalready exists in " );
      strcat_s( szMsg, zsizeof( szMsg ), "your Project.\n\nRefresh Processing aborted!" );
      MessageSend( vTZCMLPLO, "CM00702", "Configuration Management",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   return( 0 );
}

// Check cuurent component for duplicate component names
//
//  RETURNS:    = 0   - no duplicate component names
//              = 1   - duplicate component names, overwrite this Component
//              = 2   - duplicate component names, no refresh for this Component
//              - 1   - duplicate component names, cancel Refresh Process
zSHORT
zwfnTZCMLPLO_CheckDuplicateNames( zVIEW   vTZCMLPLO,
                                  zVIEW   vTZCMCPRO,
                                  zPULONG ulZKeyLPLR,
                                  zBOOL   bDelete )
{
   zCHAR    szMsg[356];
   zCHAR    szName[ 33 ];
   zCHAR    szTypeLPLR[125];
   zCHAR    szTypeCPLR[125];
   zSHORT   nRC;

   if ( bDelete )
      nRC = CheckDuplicateName( vTZCMLPLO, vTZCMCPRO, szTypeCPLR, szTypeLPLR, szName );
   else
      nRC = CheckDuplicateNameForRefresh( vTZCMLPLO, vTZCMCPRO, szTypeCPLR, szTypeLPLR, szName, ulZKeyLPLR );
   if ( nRC < 0 )
   {
      // duplicate Names between Dialogs and LODs, no refresh for this component
      if ( nRC == -2 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "A component '" );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "' of type " );
         strcat_s( szMsg, zsizeof( szMsg ), szTypeCPLR );
         strcat_s( szMsg, zsizeof( szMsg ), " could not be refreshed,\nbecause a component '" );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "' of type " );
         strcat_s( szMsg, zsizeof( szMsg ), szTypeLPLR );
         strcat_s( szMsg, zsizeof( szMsg ), "\nalready exists in your Project." );
         nRC = MessagePrompt( vTZCMLPLO, "CM00702", "Configuration Management",
                              szMsg, zBEEP, zBUTTONS_OK,
                              zRESPONSE_OK, zICON_INFORMATION );
         return( 2 );
      }

      strcpy_s( szMsg, zsizeof( szMsg ), "A component '" );
      strcat_s( szMsg, zsizeof( szMsg ), szName );
      strcat_s( szMsg, zsizeof( szMsg ), "' of type " );
      strcat_s( szMsg, zsizeof( szMsg ), szTypeLPLR );
      strcat_s( szMsg, zsizeof( szMsg ), "\nalready exists in your Project." );
      strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you want to overwrite?" );
      nRC = MessagePrompt( vTZCMLPLO, "CM00702", "Configuration Management",
                           szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                           zRESPONSE_CANCEL, zICON_QUESTION );
      if ( nRC == zRESPONSE_CANCEL )
         return( -1 );
      else
         return( nRC );
   }

   return( 0 );
}

zSHORT
zwfnTZCMLPLO_Create_W_MetaType( zPVIEW  vTZCMLPLO,
                                zLONG   Type )
{

   if ( SetCursorFirstEntityByInteger( *vTZCMLPLO, "W_MetaType", "Type", Type + 2000, 0 ) != zCURSOR_SET )
   {
      CreateEntity( *vTZCMLPLO, "W_MetaType", zPOS_AFTER );
      SetAttributeFromInteger( *vTZCMLPLO, "W_MetaType", "Type", Type + 2000 );
   }

   return( 0 );
}

zSHORT
zwfnTZCMLPLO_CheckTypeForAddMeta( zVIEW   vSubtask,
                                  zVIEW   vTZCMCPRO,
                                  zLONG   Type,
                                  zPCHAR  szName,
                                  zPCHAR  szL_MetaSrcDir,
                                  zPCHAR  szL_ExecDir,
                                  zPSHORT ERD_Refreshed,
                                  zPSHORT DomainsUpdated )
{
   zSHORT  nRC;

   switch ( Type )
   {
      case zSOURCE_ERD_META:
         *ERD_Refreshed = 1;
         // Fall through!!!

      case zSOURCE_LOD_META:
      case zSOURCE_PENV_META:
      case zSOURCE_DIALOG_META:
      case zSOURCE_REPORT_META:
      case zSOURCE_XSLT_META:

         nRC = ofnTZCMLPLO_CopyX_File( vSubtask, vTZCMCPRO,
                                       szL_MetaSrcDir, szL_ExecDir,
                                       szName, Type, 1, 0 );  // To LPLR
         if ( nRC < 0 )
            return( nRC );
         break;

      case zSOURCE_DOMAINGRP_META:
      case zSOURCE_DOMAIN_META:
         *DomainsUpdated = 1;
         break;

      case zSOURCE_DTE_META:
         *ERD_Refreshed = 1;
         break;
   }

   return( 0 );
}


zSHORT
zwfnTZCMLPLO_GetLPLR_Directories( zVIEW   vTZCMLPLO,
                                  zPCHAR  szL_MetaSrcDir,
                                  zPCHAR  szL_ExecDir )
{
   GetStringFromAttribute( szL_MetaSrcDir, zsizeof( szL_MetaSrcDir ), vTZCMLPLO, "LPLR", "MetaSrcDir" );
   ofnTZCMWKSO_AppendSlash( szL_MetaSrcDir );
   GetStringFromAttribute( szL_ExecDir, zsizeof( szL_ExecDir ), vTZCMLPLO, "LPLR", "ExecDir" );
   ofnTZCMWKSO_AppendSlash( szL_ExecDir );

   return( 0 );
}


zSHORT
zwTZCMLPLO_WriteReportView( zVIEW   vSubtask,
                            zVIEW   vTZCMCPRO,
                            zPVIEW  vTZCMRPTO,
                            zPCHAR  szMsg,
                            zPCHAR  szName,
                            zSHORT  nPos )
{
   zCHAR    szMsgUpdate[ zMAX_MESSAGE_LTH + 1 ];

   strcpy_s( szMsgUpdate, zsizeof( szMsgUpdate ), szMsg );

   CreateEntity( *vTZCMRPTO, "Z_MetaDef", zPOS_AFTER );
   SetMatchingAttributesByName( *vTZCMRPTO, "Z_MetaDef", vTZCMCPRO, "Z_MetaDef", zSET_ALL );

   GetVariableFromAttribute( szMsg + zstrlen( szMsg ), 0, zTYPE_STRING,
                             125, vTZCMCPRO, "Z_MetaDef", "Type", "CM_Type", 0 );
   strcat_s( szMsg, zsizeof( szMsg ), " " );
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   MB_SetMessage( vSubtask, 1, szMsg );

   CreateEntity( *vTZCMRPTO, "ReportLine", zPOS_AFTER );
   szMsgUpdate[ nPos++ ] = 'e';
   szMsgUpdate[ nPos++ ] = 'd';
   szMsgUpdate[ nPos ]   = ' ';

   SetAttributeFromString( *vTZCMRPTO, "ReportLine", "ReportLine", szMsgUpdate );

   return( 0 );
}


zSHORT
zwTZCMLPLO_WriteReportView1( zVIEW   vSubtask,
                             zVIEW   vTZCMLPLO,
                             zPVIEW  vTZCMRPTO,
                             zPCHAR  szMsg,
                             zSHORT  nPos )
{
   zCHAR  szMsgUpdate[ zMAX_MESSAGE_LTH + 1 ];

   strcpy_s( szMsgUpdate, zsizeof( szMsgUpdate ), szMsg );

   // First put out the report entry and send a message to the DIL
   CreateEntity( *vTZCMRPTO, "Z_MetaDef", zPOS_AFTER );
   SetAttributeFromAttribute( *vTZCMRPTO, "Z_MetaDef", "Name", vTZCMLPLO, "W_MetaDef", "Name" );
   SetAttributeFromAttribute( *vTZCMRPTO, "Z_MetaDef", "ZKey", vTZCMLPLO, "W_MetaDef", "CPLR_ZKey" );
   SetAttributeFromAttribute( *vTZCMRPTO, "Z_MetaDef", "Desc", vTZCMLPLO, "W_MetaDef", "Desc" );
   SetAttributeFromAttribute( *vTZCMRPTO, "Z_MetaDef", "LastUpdateDate", vTZCMLPLO, "W_MetaDef", "LastUpdateDate" );
   SetAttributeFromAttribute( *vTZCMRPTO, "Z_MetaDef",  "Type", vTZCMLPLO, "W_MetaType", "Type" );

   GetVariableFromAttribute( szMsg + zstrlen( szMsg ), 0,
                             zTYPE_STRING, 125, vTZCMLPLO, "W_MetaType", "Type", "CM_Type", 0 );
   strcat_s( szMsg, zsizeof( szMsg ), " " );
   GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ), vTZCMLPLO, "W_MetaDef", "Name" );
   MB_SetMessage( vSubtask, 1, szMsg );

   CreateEntity( *vTZCMRPTO, "ReportLine", zPOS_AFTER );
   szMsgUpdate[ nPos++ ] = 'e';
   szMsgUpdate[ nPos++ ] = 'd';
   szMsgUpdate[ nPos ]   = ' ';
   SetAttributeFromString( *vTZCMRPTO, "ReportLine", "ReportLine", szMsgUpdate );

   return( 0 );
}

zSHORT
zwfnTZCMLPLO_DeleteDomainsAndOperations( zVIEW  vTZCMLPLO,
                                         zLONG  Type )
{
   zVIEW    vTemp;
   zSHORT   nRC;
   zCHAR    szName[ 33 ];

   CreateViewFromViewForTask( &vTemp, vTZCMLPLO, 0 );

   if ( Type == zREFER_DOMAINGRP_META )
      SetCursorFirstEntityByInteger( vTemp, "W_MetaType", "Type", zREFER_DOMAIN_META, 0 );
   else
      SetCursorFirstEntityByInteger( vTemp, "W_MetaType", "Type", zREFER_GO_META, 0 );

   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMLPLO, "W_MetaDef", "Name" );

   for ( nRC = SetCursorFirstEntityByString( vTemp, "W_MetaDef", "GroupName", szName, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByString( vTemp, "W_MetaDef", "GroupName", szName, 0 ))
   {
      DeleteEntity( vTemp, "W_MetaDef", zREPOS_NONE );
   }

   DropView( vTemp );
   return( 0 );
}


zSHORT
zwfnTZCMLPLO_DeleteMetaInLPLR( zVIEW vSubtask, zPVIEW vTZCMLPLO )
{
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szName[ 33 ];
   zLONG    lType;
   HFILE    hFile;

   zwfnTZCMLPLO_GetLPLR_Directories( *vTZCMLPLO, szL_MetaSrcDir, szL_ExecDir );

   GetIntegerFromAttribute( &lType, *vTZCMLPLO, "W_MetaType", "Type" );
   GetStringFromAttribute( szName, zsizeof( szName ), *vTZCMLPLO, "W_MetaDef", "Name" );
   strcat_s( szL_MetaSrcDir, zsizeof( szL_MetaSrcDir ), szName );

   hFile = (HFILE) SysOpenFile( vSubtask, szL_MetaSrcDir, COREFILE_DELETE );

   if ( lType == zSOURCE_DIALOG_META || lType == zSOURCE_LOD_META )
   {
      strcat_s( szL_ExecDir, zsizeof( szL_ExecDir ), szName );

      if ( lType == zSOURCE_DIALOG_META )
         strcat_s( szL_ExecDir, zsizeof( szL_ExecDir ), ".XWD" );
      else
         strcat_s( szL_ExecDir, zsizeof( szL_ExecDir ), ".XOD" );

      hFile = (HFILE) SysOpenFile( vSubtask, szL_ExecDir, COREFILE_DELETE );
   }

   // If the meta is Domain Group or Global Operation Group, delete
   // the corresponding Domain or Global Operation entries.
   // The corresponding entries are the ones where the W_MetaDef.GroupName
   // matches the Domain or Global Operation name.
   if ( lType == zSOURCE_DOMAINGRP_META || lType == zSOURCE_GOPGRP_META ||
        lType == zREFER_DOMAINGRP_META  || lType == zREFER_GOPGRP_META )
   {
      if ( lType == zSOURCE_DOMAINGRP_META  || lType == zSOURCE_GOPGRP_META )
         lType = lType + 2000;

      zwfnTZCMLPLO_DeleteDomainsAndOperations( *vTZCMLPLO, lType );
   }

   DeleteEntity( *vTZCMLPLO, "W_MetaDef", zREPOS_NONE );

   return( 0 );
}


zSHORT
zwfnTZCMLPLO_OverwriteMetaFromCPLR( zVIEW   vSubtask,
                                    zPVIEW  vTZCMLPLO,
                                    zULONG  ulZKeyLPLR )
{
   zSHORT  nRC = 0;
   zVIEW   vTemp;

   CreateViewFromViewForTask( &vTemp, *vTZCMLPLO, 0 );

   // Delete component from LPLR
   if ( SetCursorFirstEntityByInteger( vTemp, "W_MetaDef", "CPLR_ZKey",
                                       ulZKeyLPLR, "LPLR" ) >= zCURSOR_SET )
      nRC = zwfnTZCMLPLO_DeleteMetaInLPLR( vSubtask, &vTemp );
   else
      nRC = -1;

   DropView( vTemp );
   return( nRC );
}


zSHORT
oTZCMLPLO_CheckDuplicateERD_TE_PPE( zVIEW   vSubtask,
                                    zVIEW   vTZCMLPLO,
                                    zVIEW   vTZCMCPRO,
                                    zLONG   lType,
                                    zPULONG pulZKeyLPLR )
{
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 256 ];
   zCHAR    szTypeLPLR[ 33 ];
   zSHORT   nRC;
   zVIEW    vTZCMRPTO_List;

   if ( GetViewByName( &vTZCMRPTO_List, "TZCMRPTO", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   if ( lType == zSOURCE_ERD_META || lType == zSOURCE_DTE_META || lType == zSOURCE_PENV_META )
   {
      nRC = CheckDuplicateComponents( vTZCMLPLO, vTZCMCPRO, lType, szName );
      if ( nRC < 0 )
      {
         GetVariableFromAttribute( szTypeLPLR, 0, zTYPE_STRING, 125,
                                   vTZCMRPTO_List, "Z_MetaDef", "Type", "CM_Type", 0 );

         if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type",
                                             lType + 2000, "" ) >= zCURSOR_SET )
            GetIntegerFromAttribute( (zPLONG) pulZKeyLPLR, vTZCMLPLO,
                                     "W_MetaDef", "CPLR_ZKey" );
         else
            return( 0 );

         strcpy_s( szMsg, zsizeof( szMsg ), "In your Project exists an other " );
         strcat_s( szMsg, zsizeof( szMsg ), szTypeLPLR );
         strcat_s( szMsg, zsizeof( szMsg ), "." );
         strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you want to overwrite?" );
         nRC = MessagePrompt( vTZCMLPLO, "CM00702", "Configuration Management",
                              szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_CANCEL, zICON_QUESTION );
         if ( nRC == zRESPONSE_CANCEL )
            return( -1 );
         else
            return( nRC );
      }
   }

   return( 0 );
}


////////////////////////////////////////////////////////////////////////////////
/*  the Meta doesn't exist on the LPLR so refresh him/her/it (PC note)    */
////////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZCMLPLO_AddMetaToLPLR( zVIEW   vSubtask,
                            zVIEW   vTZCMCPRO,
                            zPVIEW  vTZCMLPLO,
                            zPVIEW  vTZCMRPTO,
                            zULONG  ulZKey,
                            zLONG   Type,
                            zPCHAR  szTimestamp,
                            zPSHORT ERD_Refreshed,
                            zPSHORT DomainsUpdated )
{
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nRC = 0;
   zSHORT   nDuplicate;
   zSHORT   nPos = 3;
   zULONG   ulZKeyLPLR;

   strcpy_s( szMsg, zsizeof( szMsg ), "Adding " );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "Z_MetaDef", "Name" );

   // Check Refresh data for duplicate components (ERD, TE, PPE)
   nDuplicate = oTZCMLPLO_CheckDuplicateERD_TE_PPE( vSubtask, *vTZCMLPLO,
                                                    vTZCMCPRO,
                                                    Type, &ulZKeyLPLR );
   if ( nDuplicate == 0 )
      //Check duplicate component names
      nDuplicate = zwfnTZCMLPLO_CheckDuplicateNames( *vTZCMLPLO, vTZCMCPRO,
                                                     &ulZKeyLPLR, FALSE );
   switch ( nDuplicate )
   {
      case -1:    // duplicate component names, cancel Refresh Process
         return( -1 );
         break;
      case 1:     // duplicate component names, overwrite this Component
         nRC = zwfnTZCMLPLO_OverwriteMetaFromCPLR( vSubtask, vTZCMLPLO,
                                                   ulZKeyLPLR );
         strcpy_s( szMsg, zsizeof( szMsg ), "Updating " );
         nPos = 5;
         if ( nRC < 0 )
            return( nRC );

         break;

      case 2:     // duplicate component names, no refresh for this Component
         strcpy_s( szMsg, zsizeof( szMsg ), "No Adding " );
         nPos = 6;
         zwTZCMLPLO_WriteReportView( vSubtask, vTZCMCPRO, vTZCMRPTO, szMsg, szName, nPos );
         SetAttributeFromString( *vTZCMRPTO, "ReportLine", "ReportLine",
                                 "No Added - overwrite canceled" );
         return( 0 );
         break;
   }

   CreateEntity( *vTZCMLPLO, "W_MetaDef", zPOS_AFTER );
   SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "CPLR_ZKey",
                              vTZCMCPRO, "Z_MetaDef", "ZKey" );

   SetAttributeFromString( *vTZCMLPLO, "W_MetaDef", "Name", szName );
   SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "Desc",
                              vTZCMCPRO, "Z_MetaDef", "Desc" );
   SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "RepositoryCPLR_ZKey",
                              vTZCMCPRO, "CPLR",      "ZKey" );

   zwTZCMLPLO_WriteReportView( vSubtask, vTZCMCPRO, vTZCMRPTO, szMsg, szName, nPos );
   SetAttributeFromInteger( *vTZCMRPTO, "CPLR", "ComponentRefreshed", 1 );

   nRC = 0;
   zwfnTZCMLPLO_GetLPLR_Directories( *vTZCMLPLO, szL_MetaSrcDir, szL_ExecDir );

   if ( Type == zSOURCE_VOR_META )
      zltoxa( ulZKey, szName );

   if ( ofnTZCMLPLO_CopyMetaToLPLR( vSubtask, vTZCMCPRO, *vTZCMLPLO,
                                    szL_MetaSrcDir, szName, Type ) < 0 )
      DeleteEntity( *vTZCMLPLO, "W_MetaDef", zREPOS_AFTER );
   else
   {
      SetAttributeFromString( *vTZCMLPLO, "W_MetaDef",
                              "LastSyncDate", szTimestamp );
      SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "LastUpdateDate",
                                 vTZCMCPRO, "Z_MetaDef", "LastUpdateDate" );

      nRC = zwfnTZCMLPLO_CheckTypeForAddMeta( vSubtask, vTZCMCPRO, Type,
                                              szName, szL_MetaSrcDir,
                                              szL_ExecDir, ERD_Refreshed,
                                              DomainsUpdated );
   }

   return( nRC );
}

zSHORT
zwfnTZCMLPLO_UpdateMetaFromCPLR( zVIEW   vSubtask,
                                 zVIEW   vTZCMCPRO,
                                 zPVIEW  vTZCMLPLO,
                                 zULONG  ulZKey,
                                 zLONG   Type,
                                 zPCHAR  szTimestamp,
                                 zPSHORT ERD_Refreshed,
                                 zPSHORT DomainsUpdated )
{
   zCHAR    szName[ 33 ];
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nRC = 0;

   zwfnTZCMLPLO_GetLPLR_Directories( *vTZCMLPLO, szL_MetaSrcDir, szL_ExecDir );

   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "Z_MetaDef", "Name" );
   SetAttributeFromString( *vTZCMLPLO, "W_MetaDef", "Name", szName );
   SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "Desc",
                               vTZCMCPRO, "Z_MetaDef", "Desc" );

   if ( Type == zSOURCE_VOR_META )
      zltoxa( ulZKey, szName );

   if ( ofnTZCMLPLO_CopyMetaToLPLR( vSubtask, vTZCMCPRO, *vTZCMLPLO,
                                    szL_MetaSrcDir, szName, Type ) < 0 )
   {
      ;
   }
   else
   {
      SetAttributeFromString( *vTZCMLPLO, "W_MetaDef",
                              "LastSyncDate", szTimestamp );
      SetAttributeFromInteger( *vTZCMLPLO, "W_MetaDef", "UpdateInd", 0 );
      SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "LastUpdateDate",
                                 vTZCMCPRO, "Z_MetaDef", "LastUpdateDate" );

      if ( Type == zSOURCE_LOD_META    ||  Type == zSOURCE_ERD_META    ||
           Type == zSOURCE_PENV_META   ||  Type == zSOURCE_DIALOG_META ||
           Type == zSOURCE_REPORT_META ||  Type == zSOURCE_XSLT_META )
      {
         nRC = ofnTZCMLPLO_CopyX_File( vSubtask, vTZCMCPRO,
                                       szL_MetaSrcDir, szL_ExecDir,
                                       szName, Type, 1, 0 );  // To LPLR
         if ( nRC < 0 )
            return( nRC );
      }

      if ( Type == zSOURCE_DOMAIN_META  || Type == zSOURCE_DOMAINGRP_META )
         *DomainsUpdated = 1;
      else
         if ( Type == zSOURCE_ERD_META || Type == zSOURCE_DTE_META )
            *ERD_Refreshed = 1;
   }

   return( 0 );
}

zSHORT
zwfnTZCMLPLO_DeleteMeta( zVIEW   vSubtask,
                         zVIEW   vTZCMCPRO,
                         zPVIEW  vTZCMLPLO,
                         zPVIEW  vTZCMRPTO )
{
   zCHAR    szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zSHORT   nDuplicate;
   zSHORT   nPos = 5;
   zULONG   ulZKeyLPLR;

   strcpy_s( szMsg, zsizeof( szMsg ), "Deleting " );

   //Check duplicate component names
   nDuplicate = zwfnTZCMLPLO_CheckDuplicateNames( *vTZCMLPLO, vTZCMCPRO,
                                                  &ulZKeyLPLR, FALSE );
   switch ( nDuplicate )
   {
      case -1:    // duplicate component names, cancel Refresh Process
         return( -1 );
         break;
      case 2:     // duplicate component names, no refresh for this Component
         strcpy_s( szMsg, zsizeof( szMsg ), "No Deleting " );
         nPos = 8;
         break;
   }

   // First put out the report entry and send a message to the DIL
   zwTZCMLPLO_WriteReportView1( vSubtask, *vTZCMLPLO, vTZCMRPTO, szMsg, nPos );

   if ( nDuplicate == 2 ) //no refresh ( delete) for this component
   {
      SetAttributeFromString( *vTZCMRPTO, "ReportLine", "ReportLine",
                              "No Deleted - overwrite canceled" );
   }
   else
      // Now do the deletions
      SetAttributeFromInteger( *vTZCMRPTO, "CPLR", "ComponentRefreshed", 1 );
      zwfnTZCMLPLO_DeleteMetaInLPLR( vSubtask, vTZCMLPLO );

   return( 0 );
}


zSHORT
zwTZCMLPLO_CheckDeleteFromRepo( zPVIEW  vTZCMLPLO,
                                zULONG  ulZKey,
                                zLONG   Type )
{
   zSHORT  Deleted = 0;

   // Check to make sure this meta isn't slated for delete from repository
   if ( SetCursorFirstEntityByInteger( *vTZCMLPLO, "W_MetaType",
                                       "Type", Type, 0 ) == zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByInteger( *vTZCMLPLO, "W_MetaDef",
                                          "CPLR_ZKey", ( zLONG ) ulZKey, 0 ) == zCURSOR_SET )
      {
         if ( CompareAttributeToInteger( *vTZCMLPLO, "W_MetaDef", "UpdateInd", 3 ) == 0 )
            Deleted = 1;
         else
            DeleteEntity( *vTZCMLPLO, "W_MetaDef", zREPOS_NONE );
      }
   }

   return( Deleted );
}


zSHORT
zwfnTZCMLPLO_UpdateMeta( zVIEW   vSubtask,
                         zVIEW   vTZCMCPRO,
                         zPVIEW  vTZCMLPLO,
                         zPVIEW  vTZCMRPTO,
                         zULONG  ulZKey,
                         zLONG   Type,
                         zPCHAR  szTimestamp,
                         zPSHORT ERD_Refreshed,
                         zPSHORT DomainsUpdated,
                         zPCHAR  szPrevLPLR_LastRefreshDate )
{
   zCHAR    szC_LastCheckInDate[ 33 ];
   zCHAR    szC_LastUpdateDate[ 33 ];
   zCHAR    szL_LastUpdateDate[ 33 ];
   zCHAR    szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR    szName[ 33 ];
   zLONG    L_Status;
   zLONG    L_Updated;
   zSHORT   nRC = 0;
   zSHORT   nDuplicate;
   zSHORT   nPos = 5;
   zULONG   ulZKeyLPLR;

   strcpy_s( szMsg, zsizeof( szMsg ), "Updating " );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "Z_MetaDef", "Name" );

   // Check Refresh data for duplicate components (ERD, TE, PPE)
   nDuplicate = oTZCMLPLO_CheckDuplicateERD_TE_PPE( vSubtask, *vTZCMLPLO,
                                                    vTZCMCPRO, Type, &ulZKeyLPLR );
   if ( nDuplicate == 0 )
      //Check duplicate component names
      nDuplicate = zwfnTZCMLPLO_CheckDuplicateNames( *vTZCMLPLO, vTZCMCPRO, &ulZKeyLPLR, FALSE );

   switch ( nDuplicate )
   {
      case -1:    // duplicate component names, cancel Refresh Process
         return( -1 );
         break;
      case 1:     // duplicate component names, overwrite this Component
         nRC = zwfnTZCMLPLO_OverwriteMetaFromCPLR( vSubtask, vTZCMLPLO, ulZKeyLPLR );
         if ( nRC < 0 )
            return( nRC );

         break;

      case 2:     // duplicate component names, no refresh for this Component
         strcpy_s( szMsg, zsizeof( szMsg ), "No Updating " );
         nPos = 8;
         zwTZCMLPLO_WriteReportView( vSubtask, vTZCMCPRO, vTZCMRPTO,
                                     szMsg, szName, nPos );
         SetAttributeFromString( *vTZCMRPTO, "ReportLine", "ReportLine",
                                 "No Updated - overwrite canceled" );
         return( 0 );
         break;
   }

   // Make sure the RepositoryCPLR_ZKey value is set in the LPLR.
   if ( CompareAttributeToString( *vTZCMLPLO, "W_MetaDef", "RepositoryCPLR_ZKey", "" ) == 0 )
   {
      SetAttributeFromAttribute( *vTZCMLPLO, "W_MetaDef", "RepositoryCPLR_ZKey",
                                  vTZCMCPRO, "CPLR",      "ZKey" );
   }

   GetStringFromAttribute( szC_LastUpdateDate, zsizeof( szC_LastUpdateDate ), vTZCMCPRO, "Z_MetaDef", "LastUpdateDate" );
   GetStringFromAttribute( szC_LastCheckInDate, zsizeof( szC_LastCheckInDate ), vTZCMCPRO, "Z_MetaDef", "LastCheckInDate" );
   GetStringFromAttribute( szL_LastUpdateDate, zsizeof( szL_LastUpdateDate ), *vTZCMLPLO, "W_MetaDef", "LastUpdateDate" );
   GetIntegerFromAttribute( &L_Updated, *vTZCMLPLO, "W_MetaDef", "UpdateInd" );
   GetIntegerFromAttribute( &L_Status, *vTZCMLPLO, "W_MetaDef", "Status" );


   if ( ((zstrcmp( szPrevLPLR_LastRefreshDate, szC_LastUpdateDate ) < 0 )  ||  // CPLR is newer
         (zstrcmp( szPrevLPLR_LastRefreshDate, szC_LastCheckInDate ) < 0 ) ||  // CPLR is newer
         (zstrcmp( szL_LastUpdateDate, szC_LastUpdateDate ) < 0 )) &&  // CPLR is newer
        ((L_Status != 1) ||                        //  not Active
         ((L_Status == 1) && (L_Updated == 0 ))) ) // Active but not updated
   {
      zwTZCMLPLO_WriteReportView( vSubtask, vTZCMCPRO, vTZCMRPTO, szMsg, szName, nPos );
      SetAttributeFromInteger( *vTZCMRPTO, "CPLR", "ComponentRefreshed", 1 );

      // COPY CANIDATE so refresh him/her/it
      nRC = zwfnTZCMLPLO_UpdateMetaFromCPLR( vSubtask, vTZCMCPRO, vTZCMLPLO,
                                             ulZKey, Type, szTimestamp,
                                             ERD_Refreshed, DomainsUpdated );
   }

   return( nRC );
}


/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Refresh the LPLR using the CPLR view that's passed
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZCMLPLO_RefreshFromCPLR( zVIEW  vSubtask,
                              zVIEW  vTZCMLPLO,
                              zVIEW  vTZCMCPRO,
                              zPCHAR szTimestamp,
                              zPCHAR szPrevLPLR_LastRefreshDate )
{
   zVIEW    vDialog;
   zVIEW    vTemp;
   zVIEW    vTZCMRPTO;
   zULONG   ulZKey;
   zULONG   lCPLR_ZKey;
   zLONG    Type;
   zSHORT   ERD_Refreshed = 0;
   zSHORT   DomainsUpdated = 0;
   zSHORT   nRC;

   // Check Refresh data for duplicate components (ERD, TE, PPE)
   if ( zwfnTZCMLPLO_CheckDuplicateComponents( vTZCMLPLO, vTZCMCPRO ) < 0 )
      return( -1 );

   // Begin Refresh process
   CreateViewFromViewForTask( &vTemp, vTZCMLPLO, 0 );
   GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK );

   CreateEntity( vTZCMRPTO, "CPLR", zPOS_AFTER );
   SetMatchingAttributesByName( vTZCMRPTO, "CPLR", vTZCMCPRO, "CPLR", zSET_ALL );

   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vTZCMCPRO, "Z_MetaDef", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &Type, vTZCMCPRO, "Z_MetaDef", "Type" );
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO, "Z_MetaDef", "ZKey" );

      zwfnTZCMLPLO_Create_W_MetaType( &vTZCMLPLO, Type );

      if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey",
                                          (zLONG) ulZKey, 0 ) < zCURSOR_SET )
      {
         // Check to make sure this meta isn't slated for delete from repository
         if ( zwTZCMLPLO_CheckDeleteFromRepo( &vTemp, ulZKey, Type ) == 0 )
         {
            // the Meta doesn't exist on the LPLR so refresh him/her/it (PC note)
            nRC = zwfnTZCMLPLO_AddMetaToLPLR( vDialog, vTZCMCPRO, &vTZCMLPLO,
                                              &vTZCMRPTO, ulZKey, Type,
                                              szTimestamp, &ERD_Refreshed,
                                              &DomainsUpdated );
            if ( nRC < 0 )
            {
               DropView( vTemp );
               return( nRC );
            }
         }
      }
      else
      {
         // Meta does exist on the LPLR
         nRC = zwfnTZCMLPLO_UpdateMeta( vDialog, vTZCMCPRO, &vTZCMLPLO,
                                        &vTZCMRPTO, ulZKey, Type, szTimestamp,
                                        &ERD_Refreshed, &DomainsUpdated,
                                        szPrevLPLR_LastRefreshDate );
         if ( nRC < 0 )
         {
            DropView( vTemp );
            return( nRC );
         }
      }
      nRC = SetCursorNextEntity( vTZCMCPRO, "Z_MetaDef", 0 );
   } //endwhile ( nRC == zCURSOR_SET )

////////////////////////////////////////////////////////////////////////////////
//  Delete any CPLR entries from the LPLR that do not exist in the CPLR.
//  A CPLR entry in the LPLR is identified by its RepositoryCPLR_ZKey value.
////////////////////////////////////////////////////////////////////////////////

   // Loop through all LPLR entries that are from the current CPLR and delete
   // those that are not in the CPLR.
   // Note that new entries that have yet to be checked in do not have a
   // RepositoryCPLR_ZKey value yet and will thus not be deleted.
   GetIntegerFromAttribute( (zPLONG) &lCPLR_ZKey, vTZCMCPRO, "CPLR", "ZKey" );
   for ( nRC = SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef",
                                              "RepositoryCPLR_ZKey", lCPLR_ZKey, "LPLR" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByInteger( vTZCMLPLO, "W_MetaDef",
                                             "RepositoryCPLR_ZKey", lCPLR_ZKey, "LPLR" ))
   {
      // Try to position on the corresponding meta in the CPLR. If we can't
      // position on it, it must have been deleted and we will delete it
      // from the LPLR.
      nRC = SetCursorFirstEntityByAttr( vTZCMCPRO, "Z_MetaDef", "ZKey",
                                        vTZCMLPLO, "W_MetaDef", "CPLR_ZKey", 0 );
      if ( nRC < zCURSOR_SET )
         zwfnTZCMLPLO_DeleteMeta( vDialog, vTZCMCPRO, &vTZCMLPLO, &vTZCMRPTO );
   }

   nRC = 0;
   if ( DomainsUpdated == 1 )
      nRC++;
   if ( ERD_Refreshed == 1 )
   {
      nRC++;
      nRC++;
   }

   DropView( vTemp );
   return( nRC );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMLPLO_RefreshReusableCPLRs( zVIEW   vSubtask,
                                 zVIEW   vTZCMLPLO,
                                 zVIEW   vTZCMCPRO,
                                 zPCHAR  szTimestamp,
                                 zPCHAR  szPrevLPLR_LastRefreshDate,
                                 zBOOL   bRefreshLPLR )
{
   zSHORT   nRC;
   zSHORT   DomainsUpdated = 0;
   zSHORT   ER_Refreshed = 0;
   zVIEW    vTZCMCPRO_Reusable;
   unsigned long ulZKey;

   nRC = SetCursorFirstEntity( vTZCMCPRO, "ReusableCPLR", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute(( zPLONG ) &ulZKey, vTZCMCPRO, "ReusableCPLR", "ZKey" );
      nRC = oTZCMCPRO_ActivateCPLR( &vTZCMCPRO_Reusable, (zLONG) ulZKey, vTZCMLPLO );
      if ( nRC < 0 )
      {
         MessageSend( vTZCMCPRO, "CM00320", "Configuration Management",
                      "Cannot access reusable CPLR.  Aborting Refresh.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }

      SetNameForView( vTZCMCPRO_Reusable, "***TZCMCPRO_Reusable", vSubtask, zLEVEL_TASK );

      if ( bRefreshLPLR )
         nRC = zwfnTZCMLPLO_RefreshFromCPLR( vSubtask, vTZCMLPLO,
                                             vTZCMCPRO_Reusable, szTimestamp,
                                             szPrevLPLR_LastRefreshDate );
      else
         zwTZCMLPLO_LoadRefreshListForCPLR( vSubtask, vTZCMLPLO,
                                            vTZCMCPRO_Reusable,
                                            szPrevLPLR_LastRefreshDate );

      if ( (nRC == 1) || (nRC == 3) )
         DomainsUpdated = 1;

      if ( (nRC == 2) || (nRC == 3) )
         ER_Refreshed = 1;

      nRC = zwTZCMLPLO_RefreshReusableCPLRs( vSubtask, vTZCMLPLO,
                                             vTZCMCPRO_Reusable, szTimestamp,
                                             szPrevLPLR_LastRefreshDate,
                                             bRefreshLPLR );
      if ( (nRC == 1) || (nRC == 3) )
         DomainsUpdated = 1;

      if ( (nRC == 2) || (nRC == 3) )
         ER_Refreshed = 1;

      DropObjectInstance( vTZCMCPRO_Reusable );
      nRC = SetCursorNextEntity( vTZCMCPRO, "ReusableCPLR", 0 );
   } //endwhile ( nRC == zCURSOR_SET )

   if ( DomainsUpdated == 1 && ER_Refreshed == 1 )
      return( 3 );

   if ( ER_Refreshed == 1 )
      return( 2 );

   if ( DomainsUpdated == 1 )
      return( 1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CommitLPLR( zVIEW vTZCMLPLO )
{
   zVIEW     TZCMULWO = 0;
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szFileName2[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szWork[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szLPLR_Name[ 33 ];
   zSHORT   nRC;
// zSHORT   RESULT;

   GetStringFromAttribute( szWork, zsizeof( szWork ), vTZCMLPLO, "LPLR", "ExecDir" );
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szWork );
   ofnTZCMWKSO_AppendSlash( szFileName );
   // KJS 06/10/15 - Testing creating a LLP file in base directory.
   GetStringFromAttribute( szWork, zsizeof( szWork ), vTZCMLPLO, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szFileName2, zsizeof( szFileName2 ), szWork );
   ofnTZCMWKSO_AppendSlash( szFileName2 );
   GetStringFromAttribute( szLPLR_Name, zsizeof( szLPLR_Name ), vTZCMLPLO, "LPLR", "Name" );
   for ( nRC = 0; nRC < 32; nRC++ )
   {
      if ( !( szLPLR_Name[ nRC ] ) )
         break;
      if ( szLPLR_Name[ nRC ] == ' ' )
         szLPLR_Name[ nRC ] = '_';
   }

   szLPLR_Name[ nRC ] = 0;
   strcat_s( szFileName, zsizeof( szFileName ), szLPLR_Name );
   strcat_s( szFileName, zsizeof( szFileName ), ".XLP" );
   // KJS 06/10/15 - Testing creating a LLP file in base directory.
   strcat_s( szFileName2, zsizeof( szFileName2 ), szLPLR_Name );
   strcat_s( szFileName2, zsizeof( szFileName2 ), ".LLP" );
   zgSortEntityWithinParent( zASCENDING, vTZCMLPLO, "W_MetaType", "Type", 0 );

   // KJS 06/09/15 - I am just going to see what would happen if I created the
   // W_MetaType structure from the TZCMLPLO to TZCMUWLO.
/*
   RESULT = GetViewByName( &TZCMULWO, "TZCMULWO", vTZCMLPLO, zLEVEL_TASK );
   RESULT = SetCursorFirstEntity( TZCMULWO, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      ExcludeEntity( TZCMULWO, "W_MetaType", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZCMULWO, "W_MetaType", "" );
   }

   RESULT = SetCursorFirstEntity( vTZCMLPLO, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      IncludeSubobjectFromSubobject( TZCMULWO, "W_MetaType", vTZCMLPLO, "W_MetaType", zPOS_AFTER );
      RESULT = SetCursorNextEntity( vTZCMLPLO, "W_MetaType", "" );
   }
*/
   // END OF KJS TZCMUWLO code

   // KJS 06/10/15 - Testing creating a LLP file in base directory.
   CommitOI_ToFile( vTZCMLPLO, szFileName2, zSINGLE );
   return( CommitOI_ToFile( vTZCMLPLO, szFileName, zSINGLE ) );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Check In all Metas or just selected Metas from a work LPLR OI
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CheckInMetas( zVIEW  vSubtask,
                        zVIEW  vTZCM_ChkInList,
                        zVIEW  vTZCMCPRO,
                        zSHORT Selected )
{
   zVIEW    vTZCMWKSO;
   zVIEW    vTZCMLPLO;
   zVIEW    vLPLR_Deletes;
   zVIEW    vDialog;
   zVIEW    vTemp;
   zVIEW    vMeta;
   zVIEW    vERD;
   zVIEW    vLPLR;
   zVIEW    vTZCMAUDO;
   zULONG   ulZKey;
   zULONG   ulZKeyCPRO;
   zULONG   ulUserZKey;
   zLONG    lType;
   zLONG    lMetaType;
   zLONG    lCount;
   zLONG    lCurrent;
   zSHORT   nOffset, nCopyExec;
   zCHAR    szName[ 33 ];
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_PgmSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szTimestamp[ 33 ];
   zCHAR    szC_LastCheckInDate[ 33 ];
   zCHAR    szC_LastUpdateDate[ 33 ];
   zCHAR    szTemp[ zLONG_MESSAGE_LTH + 1 ]; // includes szL_MetaSrcDir
   zLONG    lErrorIndex;
   zLONG    CheckIn_ViewCluster;
   zLONG    lUpdateERD;
   zLONG    lUpdateTE;
   zCHAR    szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szMetaOI_File[ 33 ];
   zLONG    lMetaOI_ZKey;
   zSHORT   nXOD_CopyFlag;
   zSHORT   nLength;
   zSHORT   nRC;

   GetViewByName( &vTZCMLPLO, "TZCMLPLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMWKSO, "TZCMWKSO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCM_ChkInList, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szL_MetaSrcDir, zsizeof( szL_MetaSrcDir ), szTemp );
   ofnTZCMWKSO_AppendSlash( szL_MetaSrcDir );
   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCM_ChkInList, "LPLR", "ExecDir" );
   SysConvertEnvironmentString( szL_ExecDir, zsizeof( szL_ExecDir ), szTemp );
   ofnTZCMWKSO_AppendSlash( szL_ExecDir );
   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCM_ChkInList, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szL_PgmSrcDir, zsizeof( szL_PgmSrcDir ), szTemp );
   ofnTZCMWKSO_AppendSlash( szL_PgmSrcDir );

   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( vTZCMCPRO, "CPLR", "LastCheckInDate", szTimestamp );

//////////////////////////////////////////////////////////////////////////////////
// First, go through all Active W_MetaDef's for Delete and perform          */
// necessary delete processing before processing Copy Candidates            */
//////////////////////////////////////////////////////////////////////////////////
   CreateViewFromViewForTask( &vTemp, vTZCMLPLO, 0 );
   nRC = SetCursorFirstEntityByInteger( vTemp, "W_MetaDef", "UpdateInd", 3, "LPLR" );
   DropView( vTemp );
   if ( nRC >= zCURSOR_SET )
   {
      if ( MessagePrompt( vDialog, "CM00303",
                          "Configuration Management",
                           "You have active Components to delete.\n"
                           "Do you wish to process these now?",
                           zBEEP, zBUTTONS_YESNO,
                           zRESPONSE_YES, 0 ) == zRESPONSE_YES )
      {
         oTZCMLPLO_OrderLPLR( &vLPLR_Deletes, vTZCMLPLO );
         nRC = oTZCMLPLO_DeleteMetas( vSubtask, vLPLR_Deletes, vTZCMCPRO, vTZCM_ChkInList );
         if ( nRC )
            return( -1 );

      }
      else  //BL, 2000.01.25 if no delete, then aborting CheckIn
         return( -2 );      // -2 no Error Massage
   }
TraceLineS( "End of Section 1", "" );

//////////////////////////////////////////////////////////////////////////////////
// Go through all Active W_MetaDef and find corresponding Z_MetaDefs        */
// Verify that if there is a UserMetaStatus the User_1 is the current user. */
//////////////////////////////////////////////////////////////////////////////////
   GetIntegerFromAttribute( (zPLONG) &ulUserZKey, vTZCMWKSO, "User", "ZKey" );
   nRC = SetCursorFirstEntity( vTZCM_ChkInList, "W_MetaDef", "LPLR" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCM_ChkInList, "W_MetaDef", "CPLR_ZKey" );
      if ( SetCursorFirstEntityByInteger( vTZCMCPRO, "Z_MetaDef", "ZKey", ulZKey, 0 ) == zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByInteger( vTZCMCPRO, "User_1", "ZKey", (zLONG) ulUserZKey, 0 ) != zCURSOR_SET )
         {
            strcpy_s( szTemp, zsizeof( szTemp ), "The Meta " );
            GetStringFromAttribute( szTemp + zstrlen( szTemp ), zsizeof( szTemp ) - zstrlen( szTemp ), vTZCM_ChkInList, "W_MetaDef", "Name" );
            strcat_s( szTemp, zsizeof( szTemp ), ", Type " );
            GetVariableFromAttribute( szTemp + zstrlen( szTemp ), 0,
                                      zTYPE_STRING, 125, vTZCM_ChkInList,
                                      "W_MetaType", "Type", "CM_Type", 0 );
            strcat_s( szTemp, zsizeof( szTemp ), ", \n is not checked out by this User!" );
            strcat_s( szTemp, zsizeof( szTemp ), "\nResolve problem with CPLR administrator!" );
            MessageSend( vTZCM_ChkInList, "CM00304", "Configuration Management",
                         szTemp, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -2 );  // -2 no Error Message
         }
      }

      nRC = SetCursorNextEntity( vTZCM_ChkInList, "W_MetaDef", "LPLR" );
   }

   TraceLineS( "End of Section 2", "" );

//////////////////////////////////////////////////////////////////////////////////
// If the ER or TE are being checked in, set the flag to later give operator
// the option to copy all XODs to the CPLR.
//////////////////////////////////////////////////////////////////////////////////
   nXOD_CopyFlag = 0;
   lUpdateERD    = 0;
   lUpdateTE     = 0;
   //BL, 2000.01.25 if DataModel or TE not Updated, then no MessagePrompt
   if ( SetCursorFirstEntityByInteger( vTZCM_ChkInList, "W_MetaType", "Type",
                                       (zLONG) zSOURCE_ERD_META, 0 ) == zCURSOR_SET )
      GetIntegerFromAttribute( &lUpdateERD, vTZCM_ChkInList, "W_MetaDef", "UpdateInd" );

   if ( SetCursorFirstEntityByInteger( vTZCM_ChkInList, "W_MetaType", "Type",
                                       (zLONG) zSOURCE_DTE_META, 0 ) == zCURSOR_SET )
      GetIntegerFromAttribute( &lUpdateTE, vTZCM_ChkInList, "W_MetaDef", "UpdateInd" );

   if ( lUpdateERD != 0 || lUpdateTE != 0 )
   {
      if ( MessagePrompt( vDialog, "CM00317",
                          "Configuration Management",
                          "You are checking in the ER or TE.  Do you want"
                          " to copy all the executable LODs (ie., XODs) from your"
                          " Project to the CPLR on the Repository?",
                           zBEEP, zBUTTONS_YESNO,
                           zRESPONSE_YES, 0 ) == zRESPONSE_YES )
      {
         nXOD_CopyFlag = 1;
      }
   }


// If the ER is to be checked in check for all LOD's which have been updated
// dependently and set the corresponding Z_MetaDefs LastUpdateDate to
// LastCheckInDate
// Also make sure that there are no ER_Attribute entities without Domain
// entities.
   if ( SetCursorFirstEntityByInteger( vTZCM_ChkInList, "W_MetaType",
                                       "Type", (zLONG) zSOURCE_ERD_META, 0 ) == zCURSOR_SET )
   {
      // Make sure there are no ER_Attribute entities without Domains.
      RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
      nRC = CheckExistenceOfEntity( vLPLR, "W_MetaDef" );
      if ( nRC >= 0 )
      {
         ActivateMetaOI( vSubtask, &vERD, vLPLR, zREFER_ERD_META, zSINGLE );
         SetNameForView( vERD, "CM_ERD", vSubtask, zLEVEL_TASK );
      }
      DropView( vLPLR );
      nRC= SetCursorFirstEntity( vERD, "ER_Attribute", "EntpER_Model" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vERD, "Domain" ) < zCURSOR_SET )
         {
            strcpy_s( szTemp, zsizeof( szTemp ), "The following Attribute has no Domain:\n\n" );
            strcat_s( szTemp, zsizeof( szTemp ), "Entity:   " );
            GetStringFromAttribute( szName, zsizeof( szName ), vERD, "ER_Entity", "Name" );
            strcat_s( szTemp, zsizeof( szTemp ), szName );
            strcat_s( szTemp, zsizeof( szTemp ), "\nAttribute: " );
            GetStringFromAttribute( szName, zsizeof( szName ), vERD, "ER_Attribute", "Name" );
            strcat_s( szTemp, zsizeof( szTemp ), szName );
            MessageSend( vDialog, "ER00189", "E/R Model Maintenance",
                         szTemp,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            SetWindowActionBehavior( vDialog, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }
         nRC= SetCursorNextEntity( vERD, "ER_Attribute", "EntpER_Model" );
      }
      DropView( vERD );

      // Process LOD LastUpdateDate as explained above.
      if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type",
                       ( zLONG ) zREFER_LOD_META, 0 ) == zCURSOR_SET )
      {
         nRC = SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef",
                                                 "UpdateInd", 1, 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetIntegerFromAttribute(( zPLONG ) &ulZKey, vTZCMLPLO,
                                         "W_MetaDef", "CPLR_ZKey" );
            if ( SetCursorFirstEntityByInteger( vTZCMCPRO, "Z_MetaDef",
                           "ZKey", ulZKey, 0 ) == zCURSOR_SET )
            {
               SetAttributeFromInteger( vTZCMLPLO, "W_MetaDef", "Status", 0 );
               SetAttributeFromInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd",
                                        99 );
            }
            // ### check with Don
            nRC = SetCursorNextEntityByInteger( vTZCMLPLO, "W_MetaDef",
                                                   "UpdateInd", 1, 0 );
         }
      }
   }

TraceLineS( "End of Section 3", "" );

//////////////////////////////////////////////////////////////////////////////////
/* Go through all W_MetaDefs marked for check in and Activate them. Find    */
/* the corresponding Z_MetaDef. Commit the OI to the Repository and then    */
/* to file in the CPLR. Copy the file to the LPLR directory, exclude the    */
/* Active and on the CPLR remove the User Meta Status.                      */
//////////////////////////////////////////////////////////////////////////////////
   lType = 999;
   nRC = ofnTZCMLPLO_GetNextCheckInType( vSubtask, vTZCM_ChkInList, lType );
   if ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vTZCM_ChkInList, "W_MetaType", "Type" );
      RetrieveViewForMetaList( vSubtask, &vTemp, (zSHORT) lType );
   }

   while ( nRC == zCURSOR_SET )
   {
      lCount = 0;
      nRC = SetCursorFirstEntity( vTZCM_ChkInList, "W_MetaDef", 0 );
      while ( nRC == zCURSOR_SET )
      {
         lCount++;
         nRC = SetCursorNextEntity( vTZCM_ChkInList, "W_MetaDef", 0 );
      }
      nRC = zCURSOR_SET;

      lCurrent = 0;
      nRC = SetCursorFirstEntity( vTZCM_ChkInList, "W_MetaDef", 0 );
      while ( nRC == zCURSOR_SET )
      {
         zLONG lTypeForRepositoryCPLR_ZKey = 0;

         //BL, 1999.12.23
         CreateViewCluster( vSubtask, &CheckIn_ViewCluster );
         AddToViewCluster( CheckIn_ViewCluster, vTZCMCPRO, 0 );

         strcpy_s( szTemp, zsizeof( szTemp ), "Processing " );
         lCurrent++;
         zltoa( lCurrent, szTemp + 110, zsizeof( szTemp ) - 110 );
         strcat_s( szTemp, zsizeof( szTemp ), &szTemp[ 110 ] );
         strcat_s( szTemp, zsizeof( szTemp ), " of " );
         zltoa( lCount, szTemp + 110, zsizeof( szTemp ) - 110 );
         strcat_s( szTemp, zsizeof( szTemp ), &szTemp[ 110 ] );
         MB_SetMessage( vDialog, 2, szTemp );

         // If the meta to be committed is not already in the TZCMCPRO
         // object, create it.
         GetIntegerFromAttribute(( zPLONG ) &ulZKey, vTZCM_ChkInList, "W_MetaDef", "CPLR_ZKey" );
         if ( SetCursorFirstEntityByInteger( vTZCMCPRO, "Z_MetaDef", "ZKey", ulZKey, 0 ) < zCURSOR_SET )
         {
            zVIEW vTemp2;
            zLONG T_Type;

            // Adding new Meta to CPLR
            GetStringFromAttribute( szName, zsizeof( szName ), vTZCM_ChkInList, "W_MetaDef", "Name" );
            CreateEntity( vTZCMCPRO, "Z_MetaDef", zPOS_AFTER );
            SetAttributeFromAttribute( vTZCMCPRO, "Z_MetaDef", "ZKey", vTZCM_ChkInList, "W_MetaDef", "CPLR_ZKey" );
            SetAttributeFromAttribute( vTZCMCPRO, "Z_MetaDef", "Type", vTZCM_ChkInList, "W_MetaType", "Type" );

            // Put the CPLR ZKey back into the LPLR meta as a flag that the meta
            // is now in the Repository.
            // We will put it back in the REFER MetaDef, since the SOURCE
            // MetaDef will be deleted.

            CreateViewFromViewForTask( &vTemp2, vTZCMLPLO, 0 );
            GetIntegerFromAttribute( &T_Type, vTZCM_ChkInList, "W_MetaType", "Type" );
            T_Type = T_Type + 2000;   // Change to REFER MetaDef.
            SetCursorFirstEntityByInteger( vTemp2, "W_MetaType", "Type", T_Type, 0 );
            nRC = SetCursorFirstEntityByAttr( vTemp2, "W_MetaDef", "CPLR_ZKey",
                                              vTZCM_ChkInList, "W_MetaDef", "CPLR_ZKey", 0 );
            // To set RepositoryCPLR_ZKey indicates, that the component
            //  is on repository. If deleted there, this setting forces that it is
            //  deleted from LPLR too.
            // However, if the first checkin of a component fails, we must reset
            //  this attribute to NULL.
            SetAttributeFromAttribute( vTemp2,     "W_MetaDef", "RepositoryCPLR_ZKey",
                                       vTZCMCPRO,  "CPLR",      "ZKey" );
            lTypeForRepositoryCPLR_ZKey = T_Type;
            DropView( vTemp2 );
         }

         if ( SetCursorFirstEntityByInteger( vTemp, "W_MetaDef", "CPLR_ZKey", ulZKey, 0 ) == zCURSOR_SET )
         {
            strcpy_s( szTemp, zsizeof( szTemp ), "Activating " );
            GetVariableFromAttribute( szTemp + zstrlen( szTemp ), 0,
                                      zTYPE_STRING, 125, vTZCM_ChkInList,
                                      "W_MetaType", "Type", "CM_Type", 0 );
            strcat_s( szTemp, zsizeof( szTemp ), " " );
            GetStringFromAttribute( szName, zsizeof( szName ), vTZCM_ChkInList, "W_MetaDef", "Name" );
            strcat_s( szTemp, zsizeof( szTemp ), szName );
            MB_SetMessage( vDialog, 1, szTemp );

            // This is a temporary hack by DonC on 11/10/97. If we are
            // checking in an ERD, we want to force the Activate to come
            // from the file, not the a reference copy that is cashed. This
            // is because an fnActivateMetaOI will cause an ActivateOI_FromOI_ForTask
            // to be executed, which will not keep the OI updated flag from
            // the reference copy.
            SysMutexLock( vSubtask, "ActMeta", 0, 0 );
         // TraceLineX( "CheckInMetas Locked Mutex: ActMeta  for Task: ", (zLONG) vSubtask->hTask );
            if ( lType == zSOURCE_ERD_META )
            {
               nRC = fnActivateMetaOI( vSubtask, &vMeta, vTemp, zREFER_ERD_META, zLEVEL_APPLICATION | zSINGLE );
               if ( nRC > 0 )
                  DropObjectInstance( vMeta );
            }

            nRC = fnActivateMetaOI( vSubtask, &vMeta, vTemp, (zSHORT) lType, zLEVEL_APPLICATION | zSINGLE );
         // TraceLineX( "CheckInMetas Unlocking Mutex: ActMeta  for Task: ", (zLONG) vSubtask->hTask );
            SysMutexUnlock( vSubtask, "ActMeta", 0 );

            if ( nRC > 0 )
            {
               SetNameForView( vMeta, "***vMeta", vTemp, zLEVEL_TASK );

               if ( lType == zSOURCE_HDR_META )
               {
                  nRC = ofnTZCMLPLO_CopyHDR_File( vSubtask, vTZCMCPRO,
                                                  vTZCMLPLO, vMeta, 0,
                                                  CheckIn_ViewCluster );
                  if ( nRC )
                     return( -1 );
               }

               // Some meta's have source files that need to be copied. Do it!
               if ( lType == zSOURCE_LOD_META       ||
                    lType == zSOURCE_GOPGRP_META    ||
                    lType == zSOURCE_DOMAINGRP_META ||
                    lType == zSOURCE_DIALOG_META )
               {
                  zPCHAR pszSourceFileEntityName;

                  // Loop through the source files.
                  switch ( lType )
                  {
                     case zSOURCE_LOD_META:
                     case zSOURCE_DIALOG_META:
                        pszSourceFileEntityName = "SourceFile";
                        break;

                     case zSOURCE_GOPGRP_META:
                        pszSourceFileEntityName = "GlobalOperationGroup";
                        break;

                     case zSOURCE_DOMAINGRP_META:
                        pszSourceFileEntityName = "DomainGroup";
                        break;
                  }

                  nRC = ofnTZCMLPLO_CopySourceFiles( vSubtask, vTZCMCPRO,
                                                     vTZCMLPLO, vMeta,
                                                     pszSourceFileEntityName, 0,
                                                     CheckIn_ViewCluster );

                  // A global operation group must have a source file so we will
                  // return an error if one wasn't copied.  The other metas
                  // may indicate a source file but there might not be one
                  // there.
                  if ( nRC < 0 && lType == zSOURCE_GOPGRP_META )
                     return( nRC );

               }

               // Update meta data in the CPLR.
               SetAttributeFromString( vTZCMCPRO, "Z_MetaDef", "Name", szName );
               SetAttributeFromAttribute( vTZCMCPRO, "Z_MetaDef", "Desc", vTZCM_ChkInList, "W_MetaDef", "Desc" );
               SetAttributeFromAttribute( vTZCMCPRO, "Z_MetaDef", "LastUpdateDate", vTZCM_ChkInList, "W_MetaDef", "LastUpdateDate" );
               SetAttributeFromString( vTZCMCPRO, "Z_MetaDef", "LastCheckInDate", szTimestamp );

               strcpy_s( szTemp, zsizeof( szTemp ), "Committing" );
               szTemp[ 10 ] = ' ';
               nOffset = (zSHORT) zstrlen( szTemp );

               if ( CheckExistenceOfEntity( vMeta, "Z_MetaDef" ) != zCURSOR_SET )
                  IncludeSubobjectFromSubobject( vMeta, "Z_MetaDef", vTZCMCPRO, "Z_MetaDef", zPOS_AFTER );
               else
                  RelinkInstanceToInstance( vMeta, "Z_MetaDef", vTZCMCPRO, "Z_MetaDef" );

               // We must commit the meta to file, so that the shadow file has the Z_MetaDef included.
               strcpy_s( szFileSpec, zsizeof( szFileSpec ), szL_MetaSrcDir );
               if ( lType == zSOURCE_VOR_META )
               {
                  GetIntegerFromAttribute( &lMetaOI_ZKey, vMeta, SRC_CMOD[ lType ].szOD_ROOT, "ZKey" );
                  zltoxa( lMetaOI_ZKey, szMetaOI_File );
                  strcat_s( szFileSpec, zsizeof( szFileSpec ),szMetaOI_File );
               }
               else
               {
                  nLength = (zSHORT) zstrlen( szFileSpec );
                  GetStringFromAttribute( szFileSpec + nLength, zsizeof( szFileSpec ) - nLength, vTZCM_ChkInList, "W_MetaDef", "Name" );
                  TruncateName8( &szFileSpec[ nLength ] );
               }

               strcat_s( szFileSpec, zsizeof( szFileSpec ),SRC_CMOD[ lType ].szOD_EXT );
               CommitOI_ToFile( vMeta, szFileSpec, zSINGLE | zINCREMENTAL );

               strcpy_s( szTemp + nOffset, zsizeof( szTemp ) - nOffset, " to Repository" );
               MB_SetMessage( vDialog, 1, szTemp );

               // now, commit the AuditTrail Object...
               if ( GetViewByName( &vTZCMAUDO, "TZCMAUDO", vSubtask, zLEVEL_TASK ) >= 0 )
               {
                  MB_SetMessage( vDialog, 1, "Committing AuditTrail" );
                  CreateEntity( vTZCMAUDO, "AuditTrailMeta", zPOS_AFTER );
                  SetAttributeFromString( vTZCMAUDO, "AuditTrailMeta", "bDeleted", "N" );
                  IncludeSubobjectFromSubobject( vTZCMAUDO, "Z_MetaDef", vTZCMCPRO, "Z_MetaDef", zPOS_AFTER );

                  SetAttributeFromAttribute( vTZCMAUDO, "AuditTrailMeta", "MetaDefName",
                                             vTZCMAUDO, "Z_MetaDef", "Name" );
                  SetAttributeFromAttribute( vTZCMAUDO, "AuditTrailMeta", "MetaDefType",
                                             vTZCMAUDO, "Z_MetaDef", "Type" );

                  //BL, 1999.12.23
                  AddToViewCluster( CheckIn_ViewCluster, vTZCMAUDO, 0 );

               }
               else
                  MessageSend( vDialog, "CM00299", "Check In:",
                               "couldn't find Audit Trail object...",
                               zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

               if ( lType == zSOURCE_VOR_META )
                  zltoxa( ulZKey, szName );
               else
                  GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "Z_MetaDef", "Name" );

               if ( lType == zSOURCE_LOD_META     ||
                    lType == zSOURCE_PENV_META    ||
                    lType == zSOURCE_DIALOG_META  ||
                    lType == zSOURCE_REPORT_META  ||
                    lType == zSOURCE_XSLT_META )
               {
                  // Copy Executables to CPLR
                  nRC = ofnTZCMLPLO_CopyX_File( vSubtask, vTZCMCPRO, szL_MetaSrcDir, szL_ExecDir,
                                                szName, lType, 0, CheckIn_ViewCluster );  // To CPLR
                  if ( nRC < 0 )
                     return( nRC );
               }

               if ( CheckExistenceOfEntity( vTZCMCPRO, "UserMetaStatus" ) == zCURSOR_SET )
                  DeleteEntity( vTZCMCPRO, "UserMetaStatus", zREPOS_NONE );
            }
            else
            {
               strcpy_s( szTemp, zsizeof( szTemp ), "Activation of Meta " );
               strcat_s( szTemp, zsizeof( szTemp ), szName );
               strcat_s( szTemp, zsizeof( szTemp ), ", \ntype " );
               GetVariableFromAttribute( szTemp + zstrlen( szTemp ), 0, zTYPE_STRING, 125,
                                         vTZCM_ChkInList, "W_MetaType", "Type", "CM_Type", 0 );
               strcat_s( szTemp, zsizeof( szTemp ), " failed!" );
               strcat_s( szTemp, zsizeof( szTemp ), "\nCheck In Processing Aborted!" );
               MessageSend( vTZCM_ChkInList, "CM00307", "Configuration Management",
                            szTemp,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               TraceLineS( "Configuration Management", szTemp );
               return( -1 );
            }
         }


         // Frank H. (97/05/17): in the case we get a Crash on the next Meta,
         // we want to be sure, that the LastRefreshDate of the LPLR is
         // already set to the correct Date.
         SetAttributeFromString( vTZCMLPLO, "LPLR", "LastRefreshDate",
                                 szTimestamp );

         // Commit the LPLR meta file to the CPLR
         strcpy_s( szTemp, zsizeof( szTemp ), szL_MetaSrcDir );
         strcat_s( szTemp, zsizeof( szTemp ), szName );
         strcat_s( szTemp, zsizeof( szTemp ), SRC_CMOD[ lType ].szOD_EXT );
         nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szTemp,
                                               vTZCMCPRO, szName,
                                               SRC_CMOD[ lType ].szOD_EXT + 1,
                                               CheckIn_ViewCluster );


// Copy dependent files to the CPLR.

         nRC = SetCursorFirstEntity( vTZCMCPRO, "Z_MetaDef", 0 );
         if ( nRC == zCURSOR_SET )
         {
            strcpy_s( szTemp, zsizeof( szTemp ), "Copying " );
            nOffset = (zSHORT) zstrlen( szTemp );
         }

         while ( nRC == zCURSOR_SET )
         {
            nCopyExec = 1;
            GetStringFromAttribute( szC_LastCheckInDate, zsizeof( szC_LastCheckInDate ), vTZCMCPRO, "Z_MetaDef", "LastCheckInDate" );
            GetStringFromAttribute( szC_LastUpdateDate, zsizeof( szC_LastUpdateDate ), vTZCMCPRO, "Z_MetaDef", "LastUpdateDate" );
            if (( zstrcmp( szC_LastUpdateDate, szTimestamp ) == 0 ) ||
               ( zstrcmp( szC_LastCheckInDate, szTimestamp ) == 0 ) )
            {
               GetIntegerFromAttribute( &lMetaType, vTZCMCPRO, "Z_MetaDef", "Type" );
               GetIntegerFromAttribute( (zPLONG) &ulZKeyCPRO, vTZCMCPRO,  "Z_MetaDef", "ZKey" );
               if ( lMetaType == zSOURCE_VOR_META )
                  zltoxa( ulZKeyCPRO, szName );
               else
                  GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "Z_MetaDef", "Name" );

               GetVariableFromAttribute( szTemp + zstrlen( szTemp ), 0, zTYPE_STRING, 125,
                                         vTZCMCPRO, "Z_MetaDef", "Type", "CM_Type", 0 );
               strcpy_s( szTemp + nOffset, zsizeof( szTemp ), " meta file " );
               GetStringFromAttribute( szTemp + zstrlen( szTemp ), zsizeof( szTemp ) - zstrlen( szTemp ), vTZCMCPRO, "Z_MetaDef", "Name" );

/* This Code deletes the metas updated as dependents from the shadow
   files, i.e. LODs, to force an activation on the next Refresh */
               if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey", ulZKeyCPRO, "LPLR" ) >= zCURSOR_SET )
               {
                  if ( CompareAttributeToInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd", 99 ) == 0 )
                  {
                     SetAttributeFromInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd", 0 );
                     nCopyExec = 1;

                     // Since this is a Dependent meta, we will update the
                     // LastUpdateDate so it will be refreshed for all users.
                     SetAttributeFromString( vTZCMCPRO, "Z_MetaDef", "LastUpdateDate", szTimestamp );
                  }
                  else
                     nCopyExec = 0;
               }// SetCursorFirstEntityByInteger

/* Copy Executables */
               if ( (nCopyExec == 1) &
                    ((lMetaType == zSOURCE_LOD_META)    ||
                     (lMetaType == zSOURCE_ERD_META)    ||
                     (lMetaType == zSOURCE_PENV_META)   ||
                     (lMetaType == zSOURCE_DIALOG_META) ||
                     (lMetaType == zSOURCE_REPORT_META) ||
                     (lMetaType == zSOURCE_XSLT_META)) )
               {
                  strcpy_s( szTemp + nOffset, zsizeof( szTemp ) - nOffset, " exec file " );
                  GetStringFromAttribute( szTemp + zstrlen( szTemp ), zsizeof( szTemp ) - zstrlen( szTemp ), vTZCMCPRO, "Z_MetaDef", "Name" );
                  MB_SetMessage( vDialog, 1, szTemp );
                  nRC = ofnTZCMLPLO_CopyX_File( vSubtask, vTZCMCPRO,
                                                szL_MetaSrcDir, szL_ExecDir,
                                                szName, lMetaType, 0,
                                                CheckIn_ViewCluster );  // To CPLR
                  if ( nRC < 0 )
                     return( nRC );

               }// endif (( nCopyExec == 1 )     & ....
            }// endif (( zstrcmp( szC_LastUpdateDate, szTimestamp ) == 0 ) ||

            nRC = SetCursorNextEntity( vTZCMCPRO, "Z_MetaDef", 0 );

         }//endwhile ( nRC == zCURSOR_SET )

         //BL, 1999.12.23
         nRC = CommitMultipleObjectInstances( CheckIn_ViewCluster, &lErrorIndex );
         if ( nRC < 0 )
         {
            // the checkin failed.
            //  if the meta was checked in first time, we must reset the
            //  repository flag in the LPLR object. Otherwise it would be
            //  deleted on the next refresh.
            if ( lTypeForRepositoryCPLR_ZKey )
            {
               // Reset to initial state
               SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type",
                                              lTypeForRepositoryCPLR_ZKey, 0 );
               SetCursorFirstEntityByAttr( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey",
                                           vTZCM_ChkInList, "W_MetaDef", "CPLR_ZKey", 0 );
               // Set NULL
               SetAttributeFromString( vTZCMLPLO, "W_MetaDef", "RepositoryCPLR_ZKey", "" );
            }

            DropMetaOI( vSubtask, vMeta );
            DropViewCluster( CheckIn_ViewCluster );
            DropView( vTemp );
            return( nRC );
         }

         DropMetaOI( vSubtask, vMeta );
         DropViewCluster( CheckIn_ViewCluster );

         SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaType", "Type", lType, 0 );
         if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey", ulZKey, 0 ) == zCURSOR_SET )
         {
            SetAttributeFromInteger( vTZCMLPLO, "W_MetaDef", "Status", 0 );
            SetAttributeFromInteger( vTZCMLPLO, "W_MetaDef", "UpdateInd", 0 );
            ExcludeEntity( vTZCMLPLO, "W_MetaDef", zREPOS_NONE );
         }

         // After each meta is committed, commit the LPLR entry on the
         // workstation.
         nRC = oTZCMLPLO_CommitLPLR( vTZCMLPLO );
         if ( nRC < 0 )
            return( -1 );

         nRC = SetCursorNextEntity( vTZCM_ChkInList, "W_MetaDef", 0 );
      }

      DropView( vTemp );
      nRC = ofnTZCMLPLO_GetNextCheckInType( vSubtask, vTZCM_ChkInList, lType );
      if ( nRC == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lType, vTZCM_ChkInList, "W_MetaType", "Type" );
         RetrieveViewForMetaList( vSubtask, &vTemp, (zSHORT) lType );
      }
   }

   if ( nXOD_CopyFlag == 1 )
   {
      zVIEW vTemp2;
      zLONG lXodCount;
      zLONG lCurrentXOD = 0;

      MB_SetMessage( vDialog, 1, "Copying XODs to CPLR" );

      // Loop through all LODs and copy XODs to CPLR.
      CreateViewFromViewForTask( &vTemp2, vTZCMLPLO, 0 );
      nRC = SetCursorFirstEntityByInteger( vTemp2, "W_MetaType", "Type", zREFER_LOD_META, 0 );

      lXodCount = CountEntitiesForView( vTemp2, "W_MetaDef" );

      nRC = SetCursorFirstEntity( vTemp2, "W_MetaDef", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         strcpy_s( szTemp, zsizeof( szTemp ), "Processing " );
         lCurrentXOD++;
         zltoa( lCurrentXOD, szTemp + 110, zsizeof( szTemp ) - 110 );
         strcat_s( szTemp, zsizeof( szTemp ), &szTemp[ 110 ] );
         strcat_s( szTemp, zsizeof( szTemp ), " of " );
         zltoa( lXodCount, szTemp + 110, zsizeof( szTemp ) - 110 );
         strcat_s( szTemp, zsizeof( szTemp ), &szTemp[ 110 ] );
         MB_SetMessage( vDialog, 2, szTemp );

         GetStringFromAttribute( szName, zsizeof( szName ), vTemp2, "W_MetaDef", "Name" );

         strcpy_s( szTemp, zsizeof( szTemp ), "Copying " );
         strcat_s( szTemp, zsizeof( szTemp ), szName );
         strcat_s( szTemp, zsizeof( szTemp ), ".XOD to CPLR" );
         MB_SetMessage( vDialog, 1, szTemp );

         CreateViewCluster( vSubtask, &CheckIn_ViewCluster );
         AddToViewCluster( CheckIn_ViewCluster, vTZCMCPRO, 0 );

         nRC = ofnTZCMLPLO_CopyX_File( vSubtask, vTZCMCPRO,
                                       szL_MetaSrcDir, szL_ExecDir,
                                       szName, zSOURCE_LOD_META, 0,
                                       CheckIn_ViewCluster );  // To CPLR

         CommitMultipleObjectInstances( CheckIn_ViewCluster, &lErrorIndex );
         DropViewCluster( CheckIn_ViewCluster );

         nRC = SetCursorNextEntity( vTemp2, "W_MetaDef", 0 );
      }

      MB_SetMessage( vDialog, 1, "Copying XODs complete" );
      DropView( vTemp2 );
   }


TraceLineS( "End of Section 5", "" );
   MB_SetMessage( vDialog, 1, 0 );
   MB_SetMessage( vDialog, 2, 0 );

   return( 0 );
}


static zSHORT
fnPostRefreshGlobalOp( zVIEW vMetaOI, zVIEW vTaskLPLR )
{
   zVIEW  vTempTaskLPLR;
   zSHORT nRC;

   // Create a temp TaskLPLR view to save the cursor positions of
   // vTaskLPLR.
   CreateViewFromViewForTask( &vTempTaskLPLR, vTaskLPLR, 0 );
   SetCursorFirstEntityByInteger( vTempTaskLPLR, "W_MetaType", "Type",
                                  zREFER_GO_META, 0 );

   //
   // Any Operations that are in the LPLR but not in the Oper Group
   // need to be deleted from the LPLR.
   //
   for ( nRC = SetCursorFirstEntity( vTempTaskLPLR, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTempTaskLPLR, "W_MetaDef", 0 ) )
   {
      // If the Oper is for the meta group and is not actually in
      // the meta, delete it from LPLR.
      if ( CompareAttributeToAttribute( vMetaOI, "GlobalOperationGroup", "Name",
                                        vTempTaskLPLR, "W_MetaDef", "GroupName" ) == 0 &&
           SetCursorFirstEntityByAttr( vMetaOI, "Operation", "ZKey",
                                       vTempTaskLPLR, "W_MetaDef", "CPLR_ZKey", 0 ) < zCURSOR_SET )
      {
         DeleteEntity( vTempTaskLPLR, "W_MetaDef", zREPOS_NONE );
      }
   }

   //
   // Each of the operations in the op group need to be added to the task LPLR.
   //
   for ( nRC = SetCursorFirstEntity( vMetaOI, "Operation", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMetaOI, "Operation", 0 ) )
   {
      // If the Global Operation is not in the LPLR, add it.
      if ( SetCursorFirstEntityByAttr( vTempTaskLPLR, "W_MetaDef", "CPLR_ZKey",
                                       vMetaOI, "Operation", "ZKey", 0 ) < zCURSOR_SET )
      {
         SetCursorLastEntity( vTempTaskLPLR, "W_MetaDef", 0 );
         CreateEntity( vTempTaskLPLR, "W_MetaDef", zPOS_LAST );
         SetAttributeFromAttribute( vTempTaskLPLR, "W_MetaDef", "CPLR_ZKey",
                                    vMetaOI, "Operation", "ZKey" );
         SetAttributeFromInteger( vTempTaskLPLR, "W_MetaDef", "Status", 0 );
         SetAttributeFromInteger( vTempTaskLPLR, "W_MetaDef", "UpdateInd", 0 );
      }
      SetAttributeFromAttribute( vTempTaskLPLR, "W_MetaDef", "Name",
                                 vMetaOI, "Operation", "Name" );
      SetAttributeFromAttribute( vTempTaskLPLR, "W_MetaDef", "GroupName",
                                 vMetaOI, "GlobalOperationGroup", "Name" );
   }

   // Clean up.
   DropView( vTempTaskLPLR );
   return( 0 );
}


static zSHORT
fnPostRefreshDomain( zVIEW vMetaOI, zVIEW vTaskLPLR )
{
   zVIEW  vTempTaskLPLR;
   zSHORT nRC;

   // Create a temp TaskLPLR view to save the cursor positions of
   // vTaskLPLR.
   CreateViewFromViewForTask( &vTempTaskLPLR, vTaskLPLR, 0 );
   SetCursorFirstEntityByInteger( vTempTaskLPLR, "W_MetaType", "Type",
                                  zREFER_DOMAIN_META, 0 );

   //
   // Any Domains that are in the LPLR but not in the Domain Group
   // need to be deleted from the LPLR.
   //
   for ( nRC = SetCursorFirstEntity( vTempTaskLPLR, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTempTaskLPLR, "W_MetaDef", 0 ) )
   {
      // If the Domain is for the meta group and is not actually in
      // the meta, delete it from LPLR.
      if ( CompareAttributeToAttribute( vMetaOI, "DomainGroup", "Name",
                                        vTempTaskLPLR, "W_MetaDef",
                                        "GroupName" ) == 0 &&
           SetCursorFirstEntityByAttr( vMetaOI, "Domain", "ZKey",
                                       vTempTaskLPLR, "W_MetaDef", "CPLR_ZKey", 0 ) < zCURSOR_SET )
      {
         DeleteEntity( vTempTaskLPLR, "W_MetaDef", zREPOS_NONE );
      }
   }

   // Each of the domains in the domain group need to be added to the
   // task LPLR.
   for ( nRC = SetCursorFirstEntity( vMetaOI, "Domain", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMetaOI, "Domain", 0 ) )
   {
      // If the Domain is not in the LPLR, add it.
      if ( SetCursorFirstEntityByAttr( vTempTaskLPLR, "W_MetaDef", "CPLR_ZKey",
                                       vMetaOI, "Domain", "ZKey", 0 ) < zCURSOR_SET )
      {
         SetCursorLastEntity( vTempTaskLPLR, "W_MetaDef", 0 );
         CreateEntity( vTempTaskLPLR, "W_MetaDef", zPOS_LAST );
         SetAttributeFromAttribute( vTempTaskLPLR, "W_MetaDef", "CPLR_ZKey",
                                    vMetaOI, "Domain", "ZKey" );
         SetAttributeFromInteger( vTempTaskLPLR, "W_MetaDef", "Status", 0 );
         SetAttributeFromInteger( vTempTaskLPLR, "W_MetaDef", "UpdateInd", 0 );
      }

      SetAttributeFromAttribute( vTempTaskLPLR, "W_MetaDef", "Name",
                                 vMetaOI, "Domain", "Name" );
      SetAttributeFromAttribute( vTempTaskLPLR, "W_MetaDef", "GroupName",
                                 vMetaOI, "DomainGroup", "Name" );
   }

   // Clean up.
   DropView( vTempTaskLPLR );
   return( 0 );
}


/*

   A meta has been copied to the target LPLR.  For some metatypes, some special
   processing needs to be performed.  It's done here.

   Arguments:  lType - Meta type (e.g. zSOURCE_HDR_META).
               lpszName - The name of the meta (eg TZZOLODO).
               lpszFileSpec - The full of the portable file that contains the
                       meta.
               vTZCMCPRO - Needed for HDR processing. (???)
               vTZCMLPLO - Used as a subtask view when activating OI's. (???)
               vTaskLPLR - Task view.

*/
zSHORT LOCALOPER
fnPostRefreshLPLR( zVIEW  vSubtask,
                   zLONG  lType,
                   zPCHAR lpszName,
                   zPCHAR lpszFileSpec,
                   zVIEW  vTZCMCPRO,
                   zVIEW  vTZCMLPLO,
                   zVIEW  vTaskLPLR )
{
   zVIEW  vMetaOI;
   zSHORT nRC = 0;

   switch ( lType )
   {
      case zSOURCE_HDR_META:
         ActivateOI_FromFile( &vMetaOI, SRC_CMOD[ zSOURCE_HDR_META ].szOD,
                              vTZCMLPLO, lpszFileSpec,
                              zLEVEL_TASK | zIGNORE_ATTRIB_ERRORS );
         nRC = ofnTZCMLPLO_CopyHDR_File( vSubtask, vTZCMCPRO, vTZCMLPLO,
                                         vMetaOI, 1, 0 );
         DropObjectInstance( vMetaOI );
         break;

      case zSOURCE_LOD_META:
         // Merge the prototypes from the LOD operations to the external
         // header file.
         ActivateOI_FromFile( &vMetaOI, SRC_CMOD[ zSOURCE_LOD_META ].szOD,
                              vTZCMLPLO, lpszFileSpec,
                              zLEVEL_TASK | zIGNORE_ATTRIB_ERRORS );
         MergeGlobalPrototypes( vMetaOI, lpszName, "LOD", 0 );
         nRC = ofnTZCMLPLO_CopySourceFiles( vSubtask, vTZCMCPRO, vTZCMLPLO,
                                            vMetaOI, "SourceFile", 1, 0 );
         DropObjectInstance( vMetaOI );

         break;

      case zSOURCE_DIALOG_META:
         // Copy any source files from the CPLR to the LPLR.
         ActivateOI_FromFile( &vMetaOI, SRC_CMOD[ zSOURCE_DIALOG_META ].szOD,
                              vTZCMLPLO, lpszFileSpec,
                              zLEVEL_TASK | zIGNORE_ATTRIB_ERRORS );
         nRC = ofnTZCMLPLO_CopySourceFiles( vSubtask, vTZCMCPRO, vTZCMLPLO,
                                            vMetaOI, "SourceFile", 1, 0 );
         DropObjectInstance( vMetaOI );

         break;

      case zSOURCE_GOPGRP_META:
         // Merge the prototypes from the Global operations to the external
         // header file.
         ActivateOI_FromFile( &vMetaOI, SRC_CMOD[ zSOURCE_GOPGRP_META ].szOD,
                              vTZCMLPLO, lpszFileSpec,
                              zLEVEL_TASK | zIGNORE_ATTRIB_ERRORS );
         MergeGlobalPrototypes( vMetaOI, lpszName, "GlobalOperationGroup", 0 );
         nRC = ofnTZCMLPLO_CopySourceFiles( vSubtask, vTZCMCPRO, vTZCMLPLO,
                                            vMetaOI, "GlobalOperationGroup", 1, 0 );

         fnPostRefreshGlobalOp( vMetaOI, vTaskLPLR );
         DropObjectInstance( vMetaOI );

         break;

      case zSOURCE_DOMAINGRP_META:
         // Activate the domain group meta.
         ActivateOI_FromFile( &vMetaOI,
                              SRC_CMOD[ zSOURCE_DOMAINGRP_META ].szOD,
                              vTZCMLPLO, lpszFileSpec,
                              zLEVEL_TASK | zIGNORE_ATTRIB_ERRORS );

         nRC = ofnTZCMLPLO_CopySourceFiles( vSubtask, vTZCMCPRO, vTZCMLPLO,
                                            vMetaOI, "DomainGroup", 1, 0 );

         fnPostRefreshDomain( vMetaOI, vTaskLPLR );
         DropObjectInstance( vMetaOI );

         break;

   } // switch ( lType )...

   // We will always return 0 because we want the refresh process to
   // continue even if we can't find the source code file.
   return( 0 );

} // fnPostRefreshLPLR

zSHORT
ofnTZCMLPLO_CopyMetaToLPLR( zVIEW  vSubtask,
                            zVIEW  vTZCMCPRO,
                            zVIEW  vTZCMLPLO,
                            zPCHAR szL_MetaSrcDir,
                            zPCHAR szName,
                            zLONG  Type )
{
   zVIEW    vDialog;
   zVIEW    vTaskLPLR;
   zCHAR    szFileName[ 2 * zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szNameType[ 254 ];
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR    szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zSHORT   nRC;

   // This routine assumes that there is a shadow file and will try to
   // activate it.  If it isn't there, it will activate the meta object
   // and create the shadow file.

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );

   // Set up LPLR file name.
   strcpy_s( szFileName, zsizeof( szFileName ), szL_MetaSrcDir );
   strcat_s( szFileName, zsizeof( szFileName ), szName );
   strcat_s( szFileName, zsizeof( szFileName ), SRC_CMOD[ Type ].szOD_EXT );

   strcpy_s( szExtension, zsizeof( szFileName ), SRC_CMOD[ Type ].szOD_EXT + 1 );

   // Tell the operator what is happening.
   GetStringFromAttribute( szNameType, zsizeof( szNameType ), vTZCMCPRO, "Z_MetaDef", "Name" );
   strcat_s( szNameType, zsizeof( szNameType ), ", Type " );
   GetVariableFromAttribute( &szNameType[ zstrlen( szNameType ) ], 0,
                             zTYPE_STRING, 125, vTZCMCPRO,
                             "Z_MetaDef", "Type", "CM_Type", 0 );
   strcat_s( szNameType, zsizeof( szNameType ), "," );
   strcpy_s( szMsg, zsizeof( szMsg ), "Refreshing " );
   strcat_s( szMsg, zsizeof( szMsg ), szNameType );
   strcat_s( szMsg, zsizeof( szMsg ), " to Project" );
   MB_SetMessage( vDialog, 1, szMsg );

   // First try to get the shadow file.
   nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szFileName,
                                           vTZCMCPRO, szName, szExtension );

   if ( nRC < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Error activating Meta " );
      strcat_s( szMsg, zsizeof( szMsg ), szNameType );
      strcat_s( szMsg, zsizeof( szMsg ), "\nfrom Repository!" );

      MessageSend( vTZCMCPRO, "CM00310", "Configuration Management",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      TraceLineS( "Configuration Management", szMsg );
      return( -1 );
   }

   // Special processing needs to be performed for each different meta
   // type.
   nRC = fnPostRefreshLPLR( vSubtask, Type, szName, szFileName, vTZCMCPRO,
                            vTZCMLPLO, vTaskLPLR );
   if ( nRC < 0 )
      TraceLineS( "Error in fnPostRefreshLPLR, file= ", szFileName );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_CopyHDR_File( zVIEW  vSubtask,
                          zVIEW  vTZCMCPRO,
                          zVIEW  vTZCMLPLO,
                          zVIEW  vHdrMeta,
                          zSHORT Direction,
                          zLONG  lViewCluster )
{
   zCHAR    szL_FileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szTemp[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szName[ 33 ];
   zCHAR    szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zSHORT   nRC;

   GetStringFromAttribute( szName, zsizeof( szName ), vHdrMeta,
                           SRC_CMOD[ zSOURCE_HDR_META ].szOD_ROOT, "Name" );
   GetStringFromAttribute( szExtension, zsizeof( szExtension ), vHdrMeta,
                           SRC_CMOD[ zSOURCE_HDR_META ].szOD_ROOT, "Extension" );
   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCMLPLO, "LPLR", "PgmSrcDir" );
   if ( szTemp[ 0 ] == 0 )
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCMLPLO, "LPLR", "MetaSrcDir" );

   SysConvertEnvironmentString( szL_FileName, zsizeof( szL_FileName ), szTemp );
   ofnTZCMWKSO_AppendSlash( szL_FileName );
   strcat_s( szL_FileName, zsizeof( szL_FileName ), szName );
   if ( szExtension[ 1 ] != '.' )
   {
      strcat_s( szL_FileName, zsizeof( szL_FileName ), "." );
   }

   strcat_s( szL_FileName, zsizeof( szL_FileName ), szExtension );

   if ( Direction == 1 )   // To LPLR
   {
      nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szL_FileName,
                                              vTZCMCPRO, szName, szExtension );
   }
   else                    // To CPLR
   {
      //BL, 1999.12.23 new Parameter
      nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szL_FileName,
                                            vTZCMCPRO, szName,
                                            szExtension, lViewCluster );
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_CopySourceFiles( zVIEW  vSubtask,
                             zVIEW  vTZCMCPRO,
                             zVIEW  vTZCMLPLO,
                             zVIEW  vSrcMeta,
                             zPCHAR pszSourceFileEntityName,
                             zSHORT Direction,
                             zLONG  lViewCluster)
{
   zCHAR    szL_FileName[ 2 * zMAX_FILESPEC_LTH + 1 ]; // szL_FileName includes szSubdir
   zCHAR    szTemp[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nLth;
   zCHAR    szSubdir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szName[ 33 ];
   zCHAR    szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zSHORT   nRC;
   zVIEW    vMeta;

   // Create a temp view so we don't screw up any cursors.
   CreateViewFromViewForTask( &vMeta, vSrcMeta, 0 );

   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCMLPLO, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szL_FileName, zsizeof( szL_FileName ), szTemp );
   ofnTZCMWKSO_AppendSlash( szL_FileName );
   nLth = (zSHORT) zstrlen( szL_FileName );

   // Loop through each of the source file entities.
   for ( nRC = SetCursorFirstEntity( vMeta, pszSourceFileEntityName, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMeta, pszSourceFileEntityName, 0 ) )

   {
      // Reset PgmSrcDirs to just contain the PgmSrcDirs.
      szL_FileName[ nLth ] = 0;

      GetStringFromAttribute( szName, zsizeof( szName ), vMeta, pszSourceFileEntityName, "Name" );
      GetStringFromAttribute( szExtension, zsizeof( szExtension ), vMeta, pszSourceFileEntityName, "Extension" );

      GetStringFromAttribute( szSubdir, zsizeof( szSubdir ), vMeta, pszSourceFileEntityName, "Subdirectory" );
      if ( *szSubdir != 0 )
      {
         strcat_s( szL_FileName, zsizeof( szL_FileName ), szSubdir );
         ofnTZCMWKSO_AppendSlash( szL_FileName );
      }

      strcat_s( szL_FileName, zsizeof( szL_FileName ), szName );
      strcat_s( szL_FileName, zsizeof( szL_FileName ), "." );
      strcat_s( szL_FileName, zsizeof( szL_FileName ), szExtension );

      if ( Direction == 1 )   // To LPLR
      {
         nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szL_FileName,
                                                 vTZCMCPRO, szName, szExtension );
         if ( nRC < 0 )
            return( -1 );
      }
      else                    // To CPLR
      {
         nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szL_FileName,
                                               vTZCMCPRO, szName,
                                               szExtension, lViewCluster );
         if ( nRC < 0 )
            return( -1 );
      }
   }

//BL   DropView( vMeta );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_SironReport_Files( zVIEW   vSubtask,
                               zVIEW   vTZCMCPRO,
                               zPCHAR  szL_MetaSrcDir,
                               zPCHAR  szL_ExecDir,
                               zPCHAR  szName,
                               zSHORT  Direction,
                               zLONG   ViewCluster )
{
   zCHAR  szL_FileName[ 2*zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zSHORT nRC;

   // For Siron Reports, we will process the two Siron Report
   // executables, REP and XSQ.
   // We will not check the return codes as it will be common for
   // the executables not to exist for a given Report.

   // Copy the REP file if it exists.
   strcpy_s( szL_FileName, zsizeof( szL_FileName ), szL_MetaSrcDir );
   strcat_s( szL_FileName, zsizeof( szL_FileName ), szName );
   strcat_s( szL_FileName, zsizeof( szL_FileName ), ".REP" );
   strcpy_s( szExtension, zsizeof( szExtension ), "REP" );

   if ( Direction == 1 )   // To LPLR
   {
      nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szL_FileName,
                                              vTZCMCPRO, szName, szExtension );
   }
   else                    // To CPLR
   {
      nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szL_FileName,
                                            vTZCMCPRO, szName,
                                            szExtension, ViewCluster );
   }

   // Copy the XSQ file if it exists.
   strcpy_s( szL_FileName, zsizeof( szL_FileName ), szL_ExecDir );
   strcat_s( szL_FileName, zsizeof( szL_FileName ), szName );
   strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XSQ" );
   strcpy_s( szExtension, zsizeof( szExtension ), "XSQ" );

   if ( Direction == 1 )   // To LPLR
   {
      nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szL_FileName,
                                              vTZCMCPRO, szName, szExtension );
   }
   else                    // To CPLR
   {
      nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szL_FileName,
                                            vTZCMCPRO, szName,
                                            szExtension, ViewCluster );
   }

   return( 0 );
}

zSHORT
ofnTZCMLPLO_CopyX_File( zVIEW   vSubtask,
                        zVIEW   vTZCMCPRO,
                        zPCHAR  szL_MetaSrcDir,
                        zPCHAR  szL_ExecDir,
                        zPCHAR  szName,
                        zLONG   Type,
                        zSHORT  Direction,
                        zLONG   ViewCluster )
{
   zCHAR  szL_FileName[ 2 * zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zSHORT nRC;

   strcpy_s( szL_FileName, zsizeof( szL_FileName ), szL_ExecDir );

   // Add the body of name.
   if ( Type == zSOURCE_ERD_META )
      strcat_s( szL_FileName, zsizeof( szL_FileName ), "ZEIDON" );
   else
   if ( Type == zSOURCE_PENV_META )
      strcat_s( szL_FileName, zsizeof( szL_FileName ), "ZEIDON" );
   else
      strcat_s( szL_FileName, zsizeof( szL_FileName ), szName );

   // Set up the extension of name.
   if ( Type == zSOURCE_LOD_META )
   {
      strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XOD" );
      strcpy_s( szExtension, zsizeof( szExtension ), "XOD" );
   }
   else
   if ( Type == zSOURCE_DIALOG_META )
   {
      zCHAR  szGenLang[ 32 ];

      SysReadZeidonIni( -1, "[Workstation]", "GenLang", szGenLang, zsizeof( szGenLang ) );
      if ( szGenLang[ 0 ] == 'j' )
         szGenLang[ 0 ] = 'J';

      szGenLang[ 1 ] = 0;

      // First copy the XPG and then contine normally with the XWD.
      if ( szGenLang[ 0 ] == 'J' )
      {
         strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XPJ" );
         strcpy_s( szExtension, zsizeof( szExtension ), "XPJ" );
      }
      else
      {
         strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XPG" );
         strcpy_s( szExtension, zsizeof( szExtension ), "XPG" );
      }

      if ( Direction == 1 )   // To LPLR
         nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szL_FileName, vTZCMCPRO, szName, szExtension );
      else                    // To CPLR
         nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szL_FileName, vTZCMCPRO, szName, szExtension, ViewCluster );

      strcpy_s( szL_FileName, zsizeof( szL_FileName ), szL_ExecDir );
      strcat_s( szL_FileName, zsizeof( szL_FileName ), szName );
      strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XWD" );
      strcpy_s( szExtension, zsizeof( szExtension ), "XWD" );
   }
   else
   if ( Type == zSOURCE_REPORT_META )
   {
      // For a Report Def, we will process the Siron Report executables
      // before continuing with the normal Zeidon Report executable.
      ofnTZCMLPLO_SironReport_Files( vSubtask, vTZCMCPRO, szL_MetaSrcDir, szL_ExecDir, szName, Direction, ViewCluster );

      strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XRP" );
      strcpy_s( szExtension, zsizeof( szExtension ), "XRP" );
   }
   else
   if ( Type == zSOURCE_XSLT_META )
   {
      strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XSL" );
      strcpy_s( szExtension, zsizeof( szExtension ), "XSL" );
   }
   else
   if ( Type == zSOURCE_PENV_META )
   {
      strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XPE" );
      strcpy_s( szExtension, zsizeof( szExtension ), "XPE" );
   }
   else
   if ( Type == zSOURCE_ERD_META )
   {
      return( 0 );
//      strcat_s( szL_FileName, zsizeof( szL_FileName ), ".XMD" );
//      strcpy_s( szExtension, zsizeof( szExtension ), "XMD" );
   }

   if ( Direction == 1 )   // To LPLR
   {
      nRC = oTZCMLPLO_CopyFileFromServerFile( vSubtask, szL_FileName, vTZCMCPRO, szName, szExtension );
      if ( nRC < 0 && Type != zSOURCE_REPORT_META && Type != zSOURCE_PENV_META )
         return( -1 );
   }
   else                    // To CPLR
   {
      nRC = oTZCMLPLO_CopyFileToServerFile( vSubtask, szL_FileName, vTZCMCPRO, szName, szExtension, ViewCluster );
      if ( nRC < 0 && Type != zSOURCE_REPORT_META && Type != zSOURCE_PENV_META )
      {
         return( -1 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_GetNextCheckInType( zVIEW vSubtask,
                                zVIEW vTZCM_ChkInList,
                                zLONG Type )
{
   zLONG lType;
   zSHORT nRC = -1;

   lType = Type;

   // zSOURCE_XSLT_META is the last one in the list, so return -1.
   if ( lType == zSOURCE_XSLT_META )
      return( -1 );

   // Find the next type.  For some reason we can't just add/subtract 1 to the
   // current type.
   while ( nRC != zCURSOR_SET )
   {
      switch ( lType )
      {
         case zSOURCE_UIS_META:
            lType = zSOURCE_DIALOG_META;
            break;

         case zSOURCE_DIALOG_META:
            lType = zSOURCE_REPORT_META;
            break;

         case zSOURCE_PENV_META:
            lType = zSOURCE_UIS_META;
            break;

         case zSOURCE_VOR_META:
            lType = zSOURCE_PENV_META;
            break;

         case zSOURCE_LOD_META:
            lType = zSOURCE_VOR_META;
            break;

         case zSOURCE_DTE_META:
            lType = zSOURCE_LOD_META;
            break;

         case zSOURCE_SA_META:
            lType = zSOURCE_DTE_META;
            break;

         case zSOURCE_ERD_META:
            lType = zSOURCE_SA_META;
            break;

         case zSOURCE_DOMAINGRP_META:
            lType = zSOURCE_ERD_META;
            break;

         case zSOURCE_GOPGRP_META:
            lType = zSOURCE_DOMAINGRP_META;
            break;

         case zSOURCE_HDR_META:
            lType = zSOURCE_GOPGRP_META;
            break;

         case 999:
            lType = zSOURCE_HDR_META;
            break;
      }

      nRC = SetCursorFirstEntityByInteger( vTZCM_ChkInList, "W_MetaType",
                                           "Type", lType, 0 );
      if ( lType == zSOURCE_REPORT_META )
         return( nRC );
   }

   return( zCURSOR_SET );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Delete all pending Metas as apart of Check In Process
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_DeleteMetas( zVIEW vSubtask,
                       zVIEW vLPLR_Deletes,
                       zVIEW vTZCMCPRO,
                       zVIEW vTZCM_ChkInList )
{
   zVIEW    vTZCMLPLO;
   zVIEW    vTZCMLPLO_Deletes;
   zVIEW    vCPLR_Deletes;
   zVIEW    vCPLR_VOR_Deletes;
   zVIEW    vTZCMAUDO;
   zVIEW    vDialog;
   zVIEW    vVOR;
   zVIEW    vLPLR_VOR;
   zLONG    Type;
   zULONG   ulZKey;
   zULONG   ulZKeyVOR;
   zCHAR    szName[ 33 ];
   zCHAR    szTempName[ 33 ];
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szC_MetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szL_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szC_ExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zSHORT   nRC;
   HFILE    hFile;

   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );

   GetViewByName( &vTZCMLPLO, "TZCMLPLO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZCMLPLO_Deletes, vTZCMLPLO, 0 );

   CreateViewFromViewForTask( &vCPLR_Deletes, vTZCMCPRO, 0 );

   GetStringFromAttribute( szL_MetaSrcDir, zsizeof( szL_MetaSrcDir ), vTZCMLPLO, "LPLR", "MetaSrcDir" );
   ofnTZCMWKSO_AppendSlash( szL_MetaSrcDir );
   GetStringFromAttribute( szC_MetaSrcDir, zsizeof( szC_MetaSrcDir ), vTZCMCPRO, "CPLR", "MetaSrcDir" );
   ofnTZCMWKSO_AppendSlash( szC_MetaSrcDir );
   GetStringFromAttribute( szL_ExecDir, zsizeof( szL_ExecDir ), vTZCMLPLO, "LPLR", "ExecDir" );
   ofnTZCMWKSO_AppendSlash( szL_ExecDir );
   GetStringFromAttribute( szC_ExecDir, zsizeof( szC_ExecDir ), vTZCMCPRO, "CPLR", "ExecDir" );
   ofnTZCMWKSO_AppendSlash( szC_ExecDir );

   nRC = SetCursorFirstEntityByInteger( vLPLR_Deletes, "W_MetaDef",
                                        "UpdateInd", 3,
                                        "LPLR" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &Type, vLPLR_Deletes, "W_MetaType", "Type" );
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vLPLR_Deletes, "W_MetaDef", "CPLR_ZKey" );
      GetStringFromAttribute( szName, zsizeof( szName ), vLPLR_Deletes, "W_MetaDef", "Name" );
      strcpy_s( szMsg, zsizeof( szMsg ), "Deleting " );
      GetVariableFromAttribute( szMsg + zstrlen( szMsg ), 0, zTYPE_STRING, 125,
                              vLPLR_Deletes, "W_MetaType", "Type", "CM_Type", 0 );
      strcat_s( szMsg, zsizeof( szMsg ), " " );
      strcat_s( szMsg, zsizeof( szMsg ), szName );
      MB_SetMessage( vDialog, 1, szMsg );

      // If the meta is a LOD, we must loop through all the VORs that are
      // for that LOD and delete them.
      if ( Type == zSOURCE_LOD_META )
      {
         CreateViewFromViewForTask( &vLPLR_VOR, vTZCMLPLO_Deletes, 0 );
         SetCursorFirstEntityByInteger( vLPLR_VOR, "W_MetaType", "Type",
                                        zREFER_VOR_META, 0 );
         // Loop through all the VORs and determine which ones use the LOD
         // being deleted. To determine this, we will have to activate each
         // from file.
         nRC = SetCursorFirstEntity( vLPLR_VOR, "W_MetaDef", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( (zPLONG) &ulZKeyVOR,
                                     vLPLR_VOR, "W_MetaDef", "CPLR_ZKey" );
            zltoxa( ulZKeyVOR, szTempName );
            strcpy_s( szFileName, zsizeof( szFileName ), szL_MetaSrcDir );
            strcat_s( szFileName, zsizeof( szFileName ), szTempName );
            strcat_s( szFileName, zsizeof( szFileName ), ".PVR" );

            if ( ActivateOI_FromFile( &vVOR, "TZWDVORO", vLPLR_VOR,
                                      szFileName,
                                      zSINGLE | zIGNORE_ATTRIB_ERRORS ) == 0 )
            {
               if ( CompareAttributeToString( vVOR, "LOD", "Name", szName ) == 0 )
               {
                  // as the meta does not exist on the database any more
                  //  only delete the PVR file.
                  hFile = (HFILE) SysOpenFile( vSubtask, szFileName, COREFILE_DELETE );

                  // Lastly, we will remove the LPLR & CPLR entries for the VOR.
                  CreateViewFromViewForTask( &vCPLR_VOR_Deletes, vTZCMCPRO, 0 );
                  SetCursorFirstEntityByAttr( vCPLR_VOR_Deletes, "Z_MetaDef", "ZKey",
                                              vLPLR_VOR, "W_MetaDef", "CPLR_ZKey", 0 );
                  DeleteEntity( vCPLR_VOR_Deletes, "Z_MetaDef", zREPOS_NONE );
                  DeleteEntity( vLPLR_VOR, "W_MetaDef", zREPOS_NONE );
                  DropView( vCPLR_VOR_Deletes );
               }

               DropObjectInstance( vVOR );
            }

            nRC = SetCursorNextEntity( vLPLR_VOR, "W_MetaDef", 0 );
         }

         DropView( vLPLR_VOR );
      }

      strcpy_s( szFileName, zsizeof( szFileName ), szL_MetaSrcDir );
      if ( Type == zSOURCE_VOR_META )
      {
         zltoxa( ulZKey, szName );
         strcat_s( szFileName, zsizeof( szFileName ), szName );
      }
      else
         strcat_s( szFileName, zsizeof( szFileName ), szName );
      strcat_s( szFileName, zsizeof( szFileName ), SRC_CMOD[ Type ].szOD_EXT );

      // now, commit the AuditTrail Object...
      if ( GetViewByName( &vTZCMAUDO, "TZCMAUDO", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         MB_SetMessage( vDialog, 1, "Committing AuditTrail" );
         CreateEntity( vTZCMAUDO, "AuditTrailMeta", zPOS_AFTER );
         SetAttributeFromString( vTZCMAUDO, "AuditTrailMeta", "bDeleted",
                                 "Y" );

         // cannot do the Include, 'cause the Metadef was just deleted.
         //IncludeSubobjectFromSubobject( vTZCMAUDO, "Z_MetaDef",
         //                               vLPLR_Deletes, "Z_MetaDef", zPOS_AFTER );
         SetAttributeFromString( vTZCMAUDO, "AuditTrailMeta", "MetaDefName", szName );
         SetAttributeFromInteger( vTZCMAUDO, "AuditTrailMeta", "MetaDefType", Type );

         if ( CommitObjectInstance( vTZCMAUDO ) < 0 )
            MessageSend( vDialog, "CM00299", "Check In:",
                         "couldn't commit Audit Trail object...",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
         MessageSend( vDialog, "CM00299", "Check In:",
                      "couldn't find Audit Trail object...",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      // Delete the Z_MetaDef in TZCMCPRO.
      nRC = SetCursorFirstEntityByInteger( vTZCMCPRO, "Z_MetaDef", "ZKey",
                                           ulZKey, "CPLR" );
      if ( nRC >= zCURSOR_SET )
      {
         DeleteEntity( vTZCMCPRO, "Z_MetaDef", zREPOS_PREV );
      }

      // Delete the entry from the Check In list.
      nRC = SetCursorFirstEntityByInteger( vTZCM_ChkInList, "W_MetaDef",
                                           "CPLR_ZKey", ulZKey, "LPLR" );
      if ( nRC >= zCURSOR_SET )
         DeleteEntity( vTZCM_ChkInList, "W_MetaDef", zREPOS_NONE );

      nRC = SetCursorFirstEntityByInteger( vCPLR_Deletes, "Z_MetaDef", "ZKey", ulZKey, "" );
      if ( nRC >= zCURSOR_SET )
         DeleteEntity( vCPLR_Deletes, "Z_MetaDef", zREPOS_NONE );

      nRC = SetCursorFirstEntityByInteger( vTZCMLPLO_Deletes, "W_MetaDef", "CPLR_ZKey", ulZKey, "LPLR" );

      // First delete the meta and corresponding executable from LPLR.
      if ( nRC >= zCURSOR_SET )
         DeleteEntity( vTZCMLPLO_Deletes, "W_MetaDef", zREPOS_NONE );
      DeleteEntity( vLPLR_Deletes, "W_MetaDef", zREPOS_NONE );
      hFile = (HFILE) SysOpenFile( vSubtask, szFileName, COREFILE_DELETE );
      if ( Type == zSOURCE_DIALOG_META || Type == zSOURCE_LOD_META )
      {
         strcpy_s( szFileName, zsizeof( szFileName ), szL_ExecDir );
         strcat_s( szFileName, zsizeof( szFileName ), szName );
         if ( Type == zSOURCE_DIALOG_META )
            strcat_s( szFileName, zsizeof( szFileName ), ".XWD" );
         else
            strcat_s( szFileName, zsizeof( szFileName ), ".XOD" );

         hFile = (HFILE) SysOpenFile( vSubtask, szFileName, COREFILE_DELETE );
      }

      // Next delete the meta shadow file and corresponding executable
      // shadow file.
      nRC = oTZCMCPRO_DeleteServerFile( vTZCMCPRO, szName,
                                        SRC_CMOD[ Type ].szOD_EXT + 1 );
      if ( nRC < 0 )
         return( nRC );

      if ( Type == zSOURCE_DIALOG_META || Type == zSOURCE_LOD_META )
      {
         if ( Type == zSOURCE_DIALOG_META )
         {
            nRC = oTZCMCPRO_DeleteServerFile( vTZCMCPRO, szName, "XWD" );
            if ( nRC < 0 )
               return( nRC );
         }
         else
         {
            nRC = oTZCMCPRO_DeleteServerFile( vTZCMCPRO, szName, "XOD" );
            if ( nRC < 0 )
               return( nRC );
         }
      }

      // As each meta is deleted commit the LPLR entry on the
      // workstation.
      nRC = CommitObjectInstance( vTZCMCPRO );
      if ( nRC < 0 )
         return( -1 );

      nRC = oTZCMLPLO_CommitLPLR( vLPLR_Deletes );
      if ( nRC < 0 )
         return( -1 );

      // Use Set Cursor First because the CommitLPLR above does a reorder,
      // which moves cursor position. Since we removed the entry just
      // processed, the First works fine.
      nRC = SetCursorFirstEntityByInteger( vLPLR_Deletes, "W_MetaDef",
                                          "UpdateInd", 3, "LPLR" );
   }

   DropView( vLPLR_Deletes );
   DropView( vCPLR_Deletes );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Sequence LPLR in Pyramid Order
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_OrderLPLR( zPVIEW vOrderedLPLR,
                     zVIEW  vTZCMLPLO )
{
   ActivateOI_FromOI_ForTask( vOrderedLPLR, vTZCMLPLO, 0, zLEVEL_TASK );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_GOPGRP_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_DOMAINGRP_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_ERD_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_DTE_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_SA_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_LOD_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_VOR_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_PENV_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_DIALOG_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   SetCursorFirstEntityByInteger( *vOrderedLPLR, "W_MetaType",
                                  "Type", zSOURCE_UIS_META, "" );
   MoveSubobject( *vOrderedLPLR, "W_MetaType",
                  *vOrderedLPLR, "W_MetaType",
                  zPOS_FIRST, zREPOS_NONE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZCMLPLO_SetCM_ViewName
//
// PURPOSE:   This derived attribute determines the view name for an
//            activated meta instance.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_SetCM_ViewName( zVIEW         zView,
                          LPVIEWENTITY  lpViewEntity,
                          LPVIEWATTRIB  lpViewAttrib,
                          zSHORT        nMsg )
{
   zLONG  lType;
   zLONG  lZKey;
   zCHAR  szMetaName[ 34 ];
   zCHAR  szCM_ViewName[ 80 ];
   zSHORT nReset;

   szCM_ViewName[ 0 ] = 0;
   if ( nMsg == zDERIVED_GET )
   {
      nReset = ResetViewFromSubobject( zView );
      GetStringFromAttribute( szCM_ViewName, zsizeof( szCM_ViewName ), zView, "LPLR", "Name" );
      strcat_s( szCM_ViewName, zsizeof( szCM_ViewName ), "." );

      GetIntegerFromAttribute( &lType, zView, "W_MetaType", "Type" );
   // CM_GetTypePrefix( (zSHORT) lType, szCM_ViewName + zstrlen( szCM_ViewName ) );
      if ( lType <= zSOURCE_MAX_META )
         strcpy_s( szCM_ViewName + zstrlen( szCM_ViewName ), zsizeof( szCM_ViewName ) - zstrlen( szCM_ViewName ), SRC_CMOD[ lType ].szOD_PREFIX );
      else
         strcpy_s( szCM_ViewName + zstrlen( szCM_ViewName ), zsizeof( szCM_ViewName ) - zstrlen( szCM_ViewName ), REFER_CMOD[ lType - 2000 ].szOD_PREFIX );

      GetIntegerFromAttribute( &lZKey, zView, "W_MetaDef", "CPLR_ZKey" );
      zltoxa( lZKey, szMetaName );
      strcat_s( szCM_ViewName, zsizeof( szCM_ViewName ), szMetaName );

      // Update the value of the DIL message in the object.
      StoreValueInRecord( zView, lpViewEntity, lpViewAttrib, szCM_ViewName, 0 );
      if ( nReset == 0 )
         SetViewToSubobject( zView, "W_MetaType" );
   }

   if ( szCM_ViewName[ 0 ] == 0 )
   {
      TraceLine( "oTZCMLPLO_SetCM_ViewName cannot set CM_ViewName using VEA: 0x%08x.%s.%s for message: %d",
                 zView, lpViewEntity->szName, lpViewAttrib->szName, nMsg );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Activate a File object instance
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_ActivateFileOI( zPVIEW vFile,
                          zVIEW  vTZCMCPRO,
                          zLONG  lZKey )
{
   zVIEW  vKZDBHQUA;
   zCHAR  szValue[ 20 ];
   zSHORT nRC;

   if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA", vTZCMCPRO,
                                     zSINGLE ) >= 0 )
   {
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   }
   else
      return( -1 );

   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "File" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "File" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   zltoa( lZKey, szValue, zsizeof( szValue ) );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szValue );
   nRC = ActivateObjectInstance( vFile, "TZCMFILO", vTZCMCPRO, vKZDBHQUA, zSINGLE );

   DropObjectInstance( vKZDBHQUA );
   return( nRC );
}


/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Copy a File from the CPLR at the Server to the LPLR
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CopyFileFromServerFile( zVIEW  vSubtask,
                                  zPCHAR szL_FileName,
                                  zVIEW  vTZCMCPRO,
                                  zPCHAR szName,
                                  zPCHAR szExtension )
{
   // Simply activate the file object and copy the Buffer entity values
   // to create the LPLR file.
   // A return code of -1 is set if the ServerFile could not be found.

   zVIEW  vFile;
   zSHORT nFound;
   zSHORT nRC;
   zULONG nLth;
   zSHORT lErrorFlag = 0;
   zLONG  lZKey;
   HFILE  hFileTo;
   zCHAR  szBuffer[ 4097 ];
   zCHAR  szMsg[ zLONG_MESSAGE_LTH + 1 ]; // includes szL_FileName
                                          // of size zMAX_FILESPEC_LTH+1

   // Position on File in CPLR object.
   nFound = 0;
   nRC = SetCursorFirstEntityByString( vTZCMCPRO, "File", "Name", szName, 0 );
   while ( nFound == 0 && nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vTZCMCPRO, "File", "Extension", szExtension ) == 0 )
         nFound = 1;
      else
         nRC = SetCursorNextEntityByString( vTZCMCPRO, "File", "Name", szName, 0 );
   }

   // If found, proceed with copy logic.
   if ( nFound == 1 )
   {
      GetIntegerFromAttribute( &lZKey, vTZCMCPRO, "File", "ZKey" );
      nRC = oTZCMLPLO_ActivateFileOI( &vFile, vTZCMCPRO, lZKey );
      if ( nRC >= 0 )
      {
         // Create the LPLR file from the File object.
         hFileTo = (HFILE) SysOpenFile( vSubtask, szL_FileName,
                                        COREFILE_WRITE );
         if ( hFileTo <= 0 )
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "File " );
            strcat_s( szMsg, zsizeof( szMsg ), szL_FileName );
            strcat_s( szMsg, zsizeof( szMsg ), " could not be opened for creation." );
            strcat_s( szMsg, zsizeof( szMsg ), "\nAborting file copy to Project process." );
            MessageSend( vTZCMCPRO, "CM00313", "Configuration Management",
                         szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -1 );
         }

         nRC = SetCursorFirstEntity( vFile, "Buffer", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            nLth = zsizeof( szBuffer ) - 1;
            GetBlobFromAttribute( szBuffer, &nLth, vFile, "Buffer", "Value" );
            SysWriteFile( vSubtask, hFileTo, szBuffer, (zSHORT) nLth );
            nRC = SetCursorNextEntity( vFile, "Buffer", 0 );
         }

         SysCloseFile( vSubtask, hFileTo, 0 );
         DropObjectInstance( vFile );
      }
   }
   else
   {
      return( -1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Copy a File from the LPLR to the CPLR at the Server
// PARAMETERS:
// szL_FileName : size at least zMAX_FILESPEC_LTH+1
// szName       : size = 33
// szExtension  : size = 4
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CopyFileToServerFile( zVIEW  vSubtask,
                                zPCHAR szL_FileName,
                                zVIEW  vTZCMCPRO,
                                zPCHAR szName,
                                zPCHAR szExtension,
                                zLONG  ViewCluster )
{
   // If the file already exists, we will simply delete the existing Buffer
   // entities and copy the file to create new Buffer entities.
   // If the file doesn't exist, we will create the File object afresh and
   // include it in the CPLR.  We will commit the CPLR first because if a
   // failure were to occur between the two commits, we could have a CPLR
   // entry with no corresponding file, but we will treat this the same as
   // having no file. It is better than have a File object instance on the
   // server that we don't know about.
   // A return code of -1 is set if the ServerFile could not be committed.

   zVIEW  vFile;
   zVIEW  vTZCMWKSO;
   zSHORT nFound;
   zSHORT nRC;
   zSHORT nLth;
   zLONG  lZKey;
   zLONG  lSeq;
   HFILE  hFileFrom;
   zCHAR  szBuffer[ 4097 ];
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // First check, whether the file does exist at all.
   // If we can't open the file, we assume there is no file on the LPLR,
   // which is a valid condition and we simply return.
   hFileFrom = (HFILE) SysOpenFile( vSubtask, szL_FileName, COREFILE_READ );
   if ( hFileFrom <= 0 )
      return( 0 );

   // Position on File in CPLR object.
   nFound = 0;
   nRC = SetCursorFirstEntityByString( vTZCMCPRO, "File", "Name", szName, 0 );
   while ( nFound == 0 && nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vTZCMCPRO, "File", "Extension", szExtension ) == 0 )
         nFound = 1;
      else
         nRC = SetCursorNextEntityByString( vTZCMCPRO, "File", "Name", szName, 0 );
   }

   if ( nFound == 1 )
   {
      // If found, activate the existing file.

      GetIntegerFromAttribute( &lZKey, vTZCMCPRO, "File", "ZKey" );
      nRC = oTZCMLPLO_ActivateFileOI( &vFile, vTZCMCPRO, lZKey );
      if ( nRC >= 0 )
      {
         // If the file exists, delete the Buffer entries.
         nRC = SetCursorFirstEntity( vFile, "Buffer", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            DeleteEntity( vFile, "Buffer", zREPOS_NONE );
            nRC = SetCursorNextEntity( vFile, "Buffer", 0 );
         }
      }
      else
      {
         // If it doesn't exist, create an empty one to match the CPLR File
         // entry.
         ActivateEmptyObjectInstance( &vFile, "TZCMFILO", vTZCMCPRO,
                                      zSINGLE | zLEVEL_APPLICATION );
         CreateEntity( vFile, "File", zPOS_AFTER );
         SetAttributeFromAttribute( vFile,     "File", "ZKey",
                                    vTZCMCPRO, "File", "ZKey" );
         SetAttributeFromString( vFile, "File", "Name", szName );
         SetAttributeFromString( vFile, "File", "Extension", szExtension );

         IncludeSubobjectFromSubobject( vTZCMCPRO, "File",
                                        vFile,     "File", zPOS_AFTER );
      // IncludeSubobjectFromSubobject( vFile,     "CPLR",
      //                                vTZCMCPRO, "CPLR", zPOS_AFTER );
      }
   }
   else
   {
      // If not found, Activate empty OI and include CPLR.
      ActivateEmptyObjectInstance( &vFile, "TZCMFILO", vTZCMCPRO,
                                   zSINGLE | zLEVEL_APPLICATION );
      CreateMetaEntity( vSubtask, vFile, "File", zPOS_AFTER );
      SetAttributeFromString( vFile, "File", "Name", szName );
      SetAttributeFromString( vFile, "File", "Extension", szExtension );

      IncludeSubobjectFromSubobject( vTZCMCPRO, "File",
                                     vFile,     "File", zPOS_AFTER );
   // IncludeSubobjectFromSubobject( vFile,     "CPLR",
   //                                vTZCMCPRO, "CPLR", zPOS_AFTER );
   }

   // Now read the file.
   // The files is stored in chunks (entity "Buffer")
   //  with a max length of 4k .
   for ( lSeq = 0; ; lSeq++ )
   {
      nLth = SysReadFile( vSubtask, hFileFrom, szBuffer, zsizeof( szBuffer ) - 1 );
      if ( nLth > 0 )
      {
         CreateEntity( vFile, "Buffer", zPOS_AFTER );
         SetAttributeFromInteger( vFile, "Buffer", "Seqno", lSeq );
         SetAttributeFromBlob( vFile, "Buffer", "Value", szBuffer, nLth );
      }
      else
         break;
   }

   SysCloseFile( vSubtask, hFileFrom, 0 );

   // Before we commit the File object, we will commit the TZCMWKSO object
   // so that the last ZKey will be updated in case of a crash.
   // ### necessary?
   GetViewByName( &vTZCMWKSO, "TZCMWKSO", vSubtask, zLEVEL_TASK );
   oTZCMWKSO_CommitWorkstation( vTZCMWKSO );

   //BL, 1999.12.23
   if ( ViewCluster != 0 )
   {
      AddToViewCluster( ViewCluster, vFile, zCOMMIT_DROPOBJECTINSTANCE );
   }
   else
   {
      // Commit the File object.
      nRC = CommitObjectInstance( vFile );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Error on committing File." );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "." );
         strcat_s( szMsg, zsizeof( szMsg ), szExtension );
         strcat_s( szMsg, zsizeof( szMsg ), " to Server." );
         strcat_s( szMsg, zsizeof( szMsg ), "\nAborting file copy to CPLR process." );
         MessageSend( vTZCMCPRO, "CM00316", "Configuration Management",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }

      DropObjectInstance( vFile );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMLPLO_LoadRefreshListForCPLR( zVIEW  vSubtask,
                                   zVIEW  vTZCMLPLO,
                                   zVIEW  vTZCMCPRO,
                                   zPCHAR szPrevLPLR_LastRefreshDate )
{
   zVIEW    vDialog = 0;  // initialize to something DKS 2000.03.09?
   zVIEW    vTemp;
   zVIEW    vTZCMRPTO;
   zULONG   ulZKey;
   zULONG   lCPLR_ZKey;
   zLONG    Type;
   zSHORT   nRC;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];

   CreateViewFromViewForTask( &vTemp, vTZCMLPLO, 0 );
   GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK );

   CreateEntity( vTZCMRPTO, "CPLR", zPOS_AFTER );
   SetMatchingAttributesByName( vTZCMRPTO, "CPLR", vTZCMCPRO, "CPLR", zSET_ALL );

   nRC = SetCursorFirstEntity( vTZCMCPRO, "Z_MetaDef", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &Type, vTZCMCPRO, "Z_MetaDef", "Type" );
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO, "Z_MetaDef", "ZKey" );
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "Z_MetaDef", "Name" );

      if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey",
                                          ( zLONG ) ulZKey, "LPLR" ) < zCURSOR_SET )
      {
         // Check to make sure this meta isn't slated for delete from repository
        if ( zwTZCMLPLO_CheckDeleteFromRepo( &vTemp, ulZKey, Type ) == 0 )
         {
            // the Meta doesn't exist on the LPLR so refresh him/her/it (PC note)
            strcpy_s( szMsg, zsizeof( szMsg ), "Adding " );
            zwTZCMLPLO_WriteReportView( vDialog, vTZCMCPRO, &vTZCMRPTO,
                                        szMsg, szName, 3 );
         }
      }
      else
      {
         // Meta does exist on the LPLR
         if ( zwfnTZCMLPLO_CheckUpdateFromRepo( vTZCMCPRO, vTZCMLPLO,
                                                szPrevLPLR_LastRefreshDate ) == 1 )
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "Updating " );
            zwTZCMLPLO_WriteReportView( vDialog, vTZCMCPRO, &vTZCMRPTO, szMsg,
                                        szName, 5 );
         }
      }
      nRC = SetCursorNextEntity( vTZCMCPRO, "Z_MetaDef", 0 );
   } //endwhile ( nRC == zCURSOR_SET )

   // Delete Meta
   GetIntegerFromAttribute( (zPLONG) &lCPLR_ZKey, vTZCMCPRO, "CPLR", "ZKey" );
   for ( nRC = SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef",
                                              "RepositoryCPLR_ZKey", lCPLR_ZKey,
                                              "LPLR" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByInteger( vTZCMLPLO, "W_MetaDef",
                                             "RepositoryCPLR_ZKey", lCPLR_ZKey,
                                             "LPLR" ))
   {
      nRC = SetCursorFirstEntityByAttr( vTZCMCPRO, "Z_MetaDef", "ZKey",
                                        vTZCMLPLO, "W_MetaDef", "CPLR_ZKey", 0 );
      if ( nRC < zCURSOR_SET )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Deleting " );
         zwTZCMLPLO_WriteReportView1( vDialog, vTZCMLPLO, &vTZCMRPTO, szMsg, 5 );
      }
   }

   DropView( vTemp );
   return( 0 );
}


zSHORT
zwfnTZCMLPLO_CheckUpdateFromRepo( zVIEW  vTZCMCPRO,
                                  zVIEW  vTZCMLPLO,
                                  zPCHAR szPrevLPLR_LastRefreshDate )
{
   zCHAR    szC_LastCheckInDate[ 33 ];
   zCHAR    szC_LastUpdateDate[ 33 ];
   zCHAR    szL_LastUpdateDate[ 33 ];
   zLONG    L_Status;
   zLONG    L_Updated;

   GetStringFromAttribute( szC_LastUpdateDate, zsizeof( szC_LastUpdateDate ), vTZCMCPRO, "Z_MetaDef",
                           "LastUpdateDate" );
   GetStringFromAttribute( szC_LastCheckInDate, zsizeof( szC_LastCheckInDate ), vTZCMCPRO, "Z_MetaDef",
                           "LastCheckInDate" );
   GetStringFromAttribute( szL_LastUpdateDate, zsizeof( szL_LastUpdateDate ), vTZCMLPLO, "W_MetaDef",
                           "LastUpdateDate" );
   GetIntegerFromAttribute( &L_Updated, vTZCMLPLO, "W_MetaDef", "UpdateInd" );
   GetIntegerFromAttribute( &L_Status, vTZCMLPLO, "W_MetaDef", "Status" );

   if ( ((zstrcmp( szPrevLPLR_LastRefreshDate,
                   szC_LastUpdateDate ) < 0)   ||   // CPLR is newer
         (zstrcmp( szPrevLPLR_LastRefreshDate,
                   szC_LastCheckInDate ) < 0 ) ||   // CPLR is newer
         (zstrcmp( szL_LastUpdateDate,
                   szC_LastUpdateDate ) < 0 )) &&   // CPLR is newer
        ((L_Status != 1)   ||                       //  not Active
         ((L_Status == 1) && (L_Updated == 0))) )   // Active but not updated

   {
      return( 1 );
   }
   else
      return( 0 );
}

zSHORT
oTZCMLPLO_Create_RootEntity( zPVIEW  vTZCMRPTO,
                             zVIEW   vTZCMRPTO_List,
                             zVIEW   vTZCMCPRO )
{
   if ( SetCursorFirstEntityByAttr(   *vTZCMRPTO, "CPLR", "ZKey",
                           vTZCMRPTO_List, "CPLR", "ZKey", "" ) < zCURSOR_SET )
   {
      CreateEntity( *vTZCMRPTO, "CPLR", zPOS_AFTER );
      SetMatchingAttributesByName( *vTZCMRPTO, "CPLR",
                                    vTZCMCPRO, "CPLR", zSET_ALL );
      SetAttributeFromInteger( *vTZCMRPTO, "CPLR", "ComponentRefreshed", 0 );
   }

   return( 0 );
}

zSHORT
oTZCMLPLO_DeleteSelectedMeta( zVIEW  vSubtask,
                              zPVIEW vTZCMRPTO,
                              zVIEW  vTZCMRPTO_List,
                              zVIEW  vTZCMLPLO,
                              zVIEW  vTZCMCPRO )
{
   zSHORT  nRC;
   zULONG  lCPLR_ZKey;

   GetIntegerFromAttribute( (zPLONG) &lCPLR_ZKey, vTZCMCPRO, "CPLR", "ZKey" );

   for ( nRC = SetCursorFirstSelectedEntity( vTZCMRPTO_List, "ReportLine", "CPLR" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZCMRPTO_List, "ReportLine", "CPLR" ))
   {
      nRC = SetCursorFirstEntityByAttr( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey",
                                        vTZCMRPTO_List, "Z_MetaDef", "ZKey",
                                        "LPLR" );
      if ( nRC >= zCURSOR_SET )
      {
         if ( CompareAttributeToInteger( vTZCMLPLO, "W_MetaDef",
                                         "RepositoryCPLR_ZKey", lCPLR_ZKey ) == 0 )
         {
            nRC = SetCursorFirstEntityByAttr( vTZCMCPRO, "Z_MetaDef", "ZKey",
                                        vTZCMLPLO, "W_MetaDef", "CPLR_ZKey", 0 );
            if ( nRC < zCURSOR_SET )
               zwfnTZCMLPLO_DeleteMeta( vSubtask, vTZCMCPRO, &vTZCMLPLO, vTZCMRPTO );
         }
      }
   }

   return( 0 );
}

zSHORT
oTZCMLPLO_RefreshPartialFromCPLR( zVIEW  vSubtask,
                                  zVIEW  vTZCMLPLO,
                                  zVIEW  vTZCMCPRO,
                                  zVIEW  vTZCMRPTO_List,
                                  zPCHAR szTimestamp,
                                  zPCHAR szPrevLPLR_LastRefreshDate )
{
   zVIEW    vDialog;
   zVIEW    vTemp;
   zVIEW    vTZCMRPTO;
   zLONG    ulZKey;
   zLONG    Type;
   zSHORT   ERD_Refreshed = 0;
   zSHORT   DomainsUpdated = 0;
   zSHORT   nRC;

   // Begin Refresh process
   CreateViewFromViewForTask( &vTemp, vTZCMLPLO, 0 );
   GetViewByName( &vTZCMRPTO, "TZCMRPTO_New", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );

   oTZCMLPLO_Create_RootEntity( &vTZCMRPTO, vTZCMRPTO_List, vTZCMCPRO );

   for ( nRC = SetCursorFirstSelectedEntity( vTZCMRPTO_List, "ReportLine", "CPLR" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZCMRPTO_List, "ReportLine", "CPLR" ))
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMRPTO_List,
                               "Z_MetaDef", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vTZCMCPRO, "Z_MetaDef", "ZKey",
                                          (zLONG) ulZKey, 0 ) < zCURSOR_SET )
      {
         continue;
      }

      GetIntegerFromAttribute( &Type, vTZCMCPRO, "Z_MetaDef", "Type" );
      zwfnTZCMLPLO_Create_W_MetaType( &vTZCMLPLO, Type );

      if ( SetCursorFirstEntityByInteger( vTZCMLPLO, "W_MetaDef", "CPLR_ZKey",
                                          ( zLONG ) ulZKey, 0 ) < zCURSOR_SET )
      {
         // Check to make sure this meta isn't slated for delete from repository
         if ( zwTZCMLPLO_CheckDeleteFromRepo( &vTemp, ulZKey, Type ) == 0 )
         {
            // the Meta doesn't exist on the LPLR so refresh him/her/it (PC note)
            nRC = zwfnTZCMLPLO_AddMetaToLPLR( vDialog, vTZCMCPRO, &vTZCMLPLO,
                                              &vTZCMRPTO, ulZKey, Type, szTimestamp,
                                              &ERD_Refreshed, &DomainsUpdated );
            if ( nRC < 0 )
            {
               DropView( vTemp );
               return( nRC );
            }
         }
      }
      else
      {
         // Meta does exist on the LPLR
         nRC = zwfnTZCMLPLO_UpdateMeta( vDialog, vTZCMCPRO, &vTZCMLPLO,
                                        &vTZCMRPTO, ulZKey, Type, szTimestamp,
                                        &ERD_Refreshed, &DomainsUpdated,
                                        szPrevLPLR_LastRefreshDate );
         if ( nRC < 0 )
         {
            DropView( vTemp );
            return( nRC );
         }
      }
   } //endfor

   // Delete Meta from LPLR
   oTZCMLPLO_DeleteSelectedMeta( vDialog, &vTZCMRPTO, vTZCMRPTO_List,
                                 vTZCMLPLO, vTZCMCPRO );

   nRC = 0;
   if ( DomainsUpdated == 1 )
      nRC++;
   if ( ERD_Refreshed == 1 )
   {
      nRC++;
      nRC++;
   }

   DropView( vTZCMRPTO_List );
   DropView( vTemp );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
// PURPOSE: Refresh the LPLR partial ( user defined )
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_RefreshPartial( zVIEW vSubtask,
                          zVIEW vTZCMLPLO,
                          zVIEW vTZCMCPRO )
{
   zVIEW    vDialog;
   zVIEW    vTZCMRPTO;
   zVIEW    vTZCMRPTO_List;
   zSHORT   ERD_Refreshed = 0;
   zSHORT   DomainsUpdated = 0;
   zCHAR    szTimestamp[ 33 ];
   zCHAR    szPrevLPLR_LastRefreshDate[ 33 ];
   zSHORT   nRC;

   GetViewByName( &vDialog, "TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );
   MB_SetMessage( vDialog, 1, "Refreshing Project" );

   GetViewByName( &vTZCMRPTO_List, "TZCMRPTO", vSubtask, zLEVEL_TASK );

   // Begin Refresh process
   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   GetStringFromAttribute( szPrevLPLR_LastRefreshDate, zsizeof( szPrevLPLR_LastRefreshDate ), vTZCMLPLO, "LPLR", "LastRefreshDate" );
   SetAttributeFromString( vTZCMLPLO, "LPLR", "LastRefreshDate", szTimestamp );

   if ( GetViewByName( &vTZCMRPTO, "TZCMRPTO_New", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // TZCMRPTO is used to build a Refresh Report
      nRC = ActivateEmptyObjectInstance( &vTZCMRPTO, "TZCMRPTO", vTZCMLPLO, zMULTIPLE );
      SetNameForView( vTZCMRPTO, "TZCMRPTO_New", vTZCMLPLO, zLEVEL_TASK );
   }

   nRC = oTZCMLPLO_RefreshPartialFromCPLR( vSubtask, vTZCMLPLO, vTZCMCPRO,
                                           vTZCMRPTO_List, szTimestamp,
                                           szPrevLPLR_LastRefreshDate );
   if ( nRC < 0 )
      return( -1 );
   else
      oTZCMLPLO_CheckUpdateFlags( nRC, &ERD_Refreshed, &DomainsUpdated );

   oTZCMLPLO_RefreshFinish( vDialog, vTZCMLPLO, vTZCMCPRO, vTZCMRPTO,
                            DomainsUpdated, ERD_Refreshed );

   return( 0 );
}

zSHORT
oTZCMLPLO_RefreshFinish( zVIEW  vSubtask,
                         zVIEW  vTZCMLPLO,
                         zVIEW  vTZCMCPRO,
                         zVIEW  vTZCMRPTO,
                         zSHORT DomainsUpdated,
                         zSHORT ERD_Refreshed )
{
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szTemp[ zMAX_FILESPEC_LTH + 1 ];

   MB_SetMessage( vSubtask, 1, "Committing Project" );
   oTZCMLPLO_CommitLPLR( vTZCMLPLO );

   // Rebuild the Domains.XDM file if Domains were refreshed.
   // To do this we need to loop through the DomainGroup metas and then all
   // the Domains in the group, issuing the rebuild for each Domain.

   if ( DomainsUpdated == 1 )
      oTZCMLPLO_UpdateDomains( vSubtask, vTZCMLPLO );

   // If either the ER or the TE has been refreshed, we will ask the operator
   // if he wants to refresh ALL XODs from the server EXCEPT for those LODs
   // that are checked out by the local work station.  If the operator answers
   // yes, we will then loop through all LODs not checked out and copy those
   // XODs from the server.

   if ( ERD_Refreshed == 1 )
      oTZCMLPLO_ERD_Refreshed( vSubtask, vTZCMLPLO, vTZCMCPRO );

   if ( ObjectInstanceUpdatedFromFile( vTZCMLPLO ) == 1 )
   {
      MB_SetMessage( vSubtask, 1, "Committing Project" );
      oTZCMLPLO_CommitLPLR( vTZCMLPLO );
   }

   MB_SetMessage( vSubtask, 1, 0 );

   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZCMLPLO, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szTemp );
   ofnTZCMWKSO_AppendSlash( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "ZREFRESH.RPT" );
   CommitOI_ToFile( vTZCMRPTO, szFileName, zASCII );

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
