/*
CHANGE LOG
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2002.06.24  BL
   Set Text Limit for Password Verification
   Bugfix delete Repository Users
   Insert Workaround for set Default Radio Buttons
2002.06.20  BL
   Added zwTZCMCPLD_AddAnotherTextLimit for set Text Limit for Password
   Verification
2002.06.18  BL
   Bugfix remove reusable CPLR
2002.06.11  BL   R56839, R56837
   Bugfix and new Design for Component List
2001.10.01  BL   Repository
   Fix for AD in Maintain CPLR Users: If delete the CPLR Authority, then
   remove the CPLR User Flag
2001.07.06  BL   Repository
   maintained Repository, part 2
2001.06.24  BL   Repository
   maintained Repository, part 2
2001.06.18  BL   Repository
   maintained Repository + R53375, R53553, F51483, F51344, F50829, F51274
2000.10.30  RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.04.13  DKS
   Removed static functions.
2000.04.04  BL
   Relink File and CPLR object before delete.
   Modified zwTZCMCPLD_ExitDialog for bugfix close SYSADM by error
   Modified zwTZCMCPLD_CPLR_OK, if error by commit vTZCMREPO, then do
   not commit vTZCMCPRO
2000.03.02  BL    10  Repository
   Modified zwTZCMCPLD_ExitDialog: if error on commit Repository, then ask
   user for close Tool
   Remove zwTZCMCPLD_BuildEntityList for new Repository structure
2000.02.17  BL   10   DB_Mode
   Seqence number in "Buffer" entity
   Drop DB_Mode and #if 0
2000.02.14  BL   10   Save CPLR as
   Added zwTZCMCPLD_FileErrorMessage for error message by copying Fileobject
   Added zwTZCMCPLD_CopyShadowWithZKey, zwTZCMCPLD_LoadShadowFile and
   zwTZCMCPLD_SearchFile for set new ZKey in Shadowfile
   Added zwTZCMCPLD_SaveAsAborted: if error while copying CPLR, then
   delete new CPLR
2000.02.11  BL   10A  Save CPLR as
   Added zwTZCMCPLD_SaveAsCheckData, zwTZCMCPLD_SaveAsSetAuditTrail,
   zwTZCMCPLD_SaveAsAccessAuthority, zwTZCMCPLD_SaveAsReusableCPLR
   zwTZCMCPLD_DeleteCPLWithoutCPLR, zwTZCMCPLD_SaveAsZ_MetaDef,
   zwTZCMCPLD_SaveAsAuditTrail, zwTZCMCPLD_CreateAuditTrail,
   zwTZCMCPLD_SaveAsFile, zwTZCMCPLD_LoadAuditTrailObject,
   zwTZCMCPLD_LoadFileObject for Save CPLR as
2000.02.02  BL   10A  Repositiory
   Added zwTZCMCPLD_SaveAsPreBuild, zwTZCMCPLD_SetCPLName,
   zwTZCMCPLD_SaveAsCancel, zwTZCMCPLD_SaveAsInNewCPL, zwTZCMCPLD_SaveAsCPLR,
   zwTZCMCPLD_SaveAS for Save CPLR as
   Modified zwTZCMCPLD_CPLR_OK for set new CPLR name und Desc in
   Main Window
   Modidified zwTZCMCPLD_CancelCPL: co delete by save CPLR as
   Modified zwTZCMCPLD_SaveCPL:
       no CPLR window by Save CPLR as
       if save CPLR as and update CPL name, then refresh main window
2000.01.26  BL   10A  Repository
   Modified zwTZCMCPLD_BuildEntityList: insert CheckExistsOfEntity
   Insert zwTZCMCPLD_RefreshFileList for refresh Window FileList
   (= modeless Window)
   Modified zwTZCMCPLD_ListAll, zwTZCMCPLD_BuildMetaList and
   zwTZCMCPLD_DeleteMetaFromList for refresh Window MetaList
   (= modeless Window )
   Modified zwTZCMCPLD_CPLR_OK: Check new CPLR name is unique.
   Modified zwTZCMCPLD_SaveCPL: Check new CPL name is unique.
2000.01.08  BL   10A
   Modified Operation zwTZCMCPLD_SignOnOK for set Focus to Control
1999.09.21  BL   10A new menu structure
   update Operation zwTZCMCPLD_AddCPLR_Users
   insert Operation zwTZCMCPLD_CancelRepositoryUsers
1999.09.20  BL   10A
   Operation zwTZCMCPLD_SetDIL_Message: insert CheckExistenceOfEntity
from here old version
15.06.1997  DonC
   Made modifications for handling of Report metas within System Admin.
09.07.1997  DonC
   Made Modifications to DeleteWorkstation to delete from TZCMWKSO object
   so that relationships are properly processed.
27.08.1997  DonC
   Added operations OrderFileByName, OrderFileByType, OrderComponentByName
   and OrderComponentByType for ordering File list and Component list.
15.11.1997  DonC
   Added operation DeleteMetaFromList to delete metas from the meta list.
05.01.1998  DonC
   Moved zwfnTZCMCPLD_DeleteMeta to TZCMCPRO.C and changed prefix to
   oTZCMCPRO.
05.01.1998  DonC
   Modified LoadCPLR to get rid of DropEntity of CPL. Also corrected
   DeleteMetaFromList to remove dead entries from list.
02.02.1998  DonC
   Modified SelectReusableCPLR to not commit the CPLR when a reusable CPLR
   is included.
16.05.1998  DonC
   Modified RefreshMenu and CommitCPLO to disable actions and eliminate
   spurious error message. (QS 194)
1998/12/14  DC
   Modified DeleteMetas so that any Shadow Files not deleted with the
   individual metas would be deleted. (TB 436)
22.02.1999  HH
   In DeleteFile relink the FILE to TZCMCPRO in order to avoid the
   database warning "no rows updated or deleted". See tzcmcpro.c, TB471.
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#define zGLOBAL_DATA
#define KZOESYS_INCL
#define zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

#ifdef __cplusplus
extern "C"
{
#endif

// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

#define zCheckAllReusables       -1
#define zCheckReusableWithError  -2

#define zCheckReusableCPLR        0
#define zCheckReusingCPLR         1

#include "kzoengaa.h"
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"

// LAST MESSAGE ID: CM00156

/* local prototypes */
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_AddInstallUser( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadCPRO_WithEnq( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadUser( zVIEW    vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SetDIL_Message( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_Cancel( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_DropComponentViews( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildMetaList( zVIEW  vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_DisableCompCtrls( zVIEW vSubtask );

zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsAborted( zVIEW vSubtask,
                          zVIEW vTZCMCPRO_New,
                          zVIEW vTZCMCPRO_Source );

zOPER_EXPORT zSHORT OPERATION
zGetFirstEntityNameForView( zVIEW   vViewToInstance,
                            zPCHAR  pszEntityName );

zOPER_EXPORT zSHORT OPERATION
LoadFileAndActivateMeta( zVIEW  vSubtask,
                         zPVIEW pvMeta,         // return meta
                         zVIEW  vTZCMCPRO,      // CPLR object
                         zPCHAR szName,         // file name
                         zPCHAR szExtension,    // extension
                         zLONG  lType,          // OD type
                         zBOOL  bDeleteFile );   // delete file from temp dir

static zSHORT
zwfnTZCMCPLD_LoadCPLR_ForViewName( zVIEW vSubtask, zPCHAR szViewName );
static zLONG
zwfnTZCMCPLD_GetListMeta( zVIEW vSubtask, zVIEW vTZCMCUSR, zVIEW vAllMetas,
                          zVIEW vMetaList, zSHORT Status, zLONG lCount, zLONG Type );
static zSHORT
zwfnTZCMCPLD_DeleteMetas( zVIEW vSubtask );
static zSHORT
CreateInstallEntity( zVIEW vSubtask, zVIEW lpView,
                     zPCHAR szEntityName, zSHORT nPosition );
static zSHORT
zwfnTZCMCPLD_CheckCPLR_ForAddReus( zVIEW vSubtask, zVIEW vReusableToAdd,
                                   zBOOL bCheckAllMetas, zSHORT nCheckType );
static zSHORT
zwfnTZCMCPLD_LoadMeta( zVIEW vSubtask, zVIEW vTZCMCPRO );
static zSHORT
zwfnTZCMCPLD_CheckCPL_Name( zVIEW vSubtask, zVIEW vTZCMCPL );
static zSHORT
zwfnTZCMCPLD_CheckCPLR_Name( zVIEW vSubtask, zVIEW vTZCMCPL, zULONG ulZKeyCPRO );
static zSHORT
zwfnTZCMCPLD_CheckPassword( zVIEW vSubtask, zPCHAR szPassword );
static zSHORT
zwfnTZCMCPLD_CommitCPL_CPLR( zVIEW vSubtask, zVIEW vTZCMCPL, zVIEW vTZCMCPLR );
static void
zwfnTZCMCPLD_TrollReusableCPLRs( zVIEW vSubtask, zVIEW vCurrentReusables,
                                 zVIEW vReusableCPLR, zVIEW vTempREPO,
                                 zVIEW vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_TrollReusableCPLR( zVIEW vSubtask, zVIEW  vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_TrollReusableCPLR2( zVIEW vSubtask, zVIEW vTZCMRCRO, zLONG ulZKey );
static zSHORT
zwTZCMCPLD_BuildReusCPLR_Views( zVIEW vSubtask, zPVIEW vTZCMRCRO,
                                zPVIEW vTZCMRCRO_All, zULONG ulCPRO_ZKey );
static zSHORT
zwfnTZCMCPLD_CreateCurrentReusableCPLR( zVIEW vReusableCPLR, zVIEW vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_ReusableCPLR_CreateQual( zVIEW vSubtask, zULONG ulZKey, zPVIEW vKZDBHQUA );
static zSHORT
zwfnTZCMCPLD_LoadCPLR( zVIEW vSubtask, zPVIEW vTZCMCPRO, zPCHAR szLodName,
                       zULONG ulZKey, zPCHAR szName, zLONG lControl );
static zSHORT
zwfnTZCMCPLD_TrollReusableMetasForPopup( zVIEW vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_LoadPopupForReusable( zVIEW vSubtask, zPCHAR szViewName );
static zSHORT
zwfnTZCMCPLD_ExpandReusable( zVIEW vSubtask, zPCHAR szViewName );
static zSHORT
zwfnTZCMCPLD_CollapseReusable( zVIEW vSubtask, zPCHAR szViewName );
static zSHORT
zwfnTZCMCPLD_ReusableSetButtonText( zVIEW vSubtask, zPCHAR szViewName );
static zSHORT
zwfnTZCMCPLD_LoadCompForReusable( zVIEW vSubtask, zPCHAR szViewName, zBOOL bShowMetas );
static zSHORT
zwfnTZCMCPLD_RemoveCompForReusable( zVIEW vSubtask, zPCHAR szViewName );
static zSHORT
zwfnTZCMCPLD_SelectReusable_UpdateView( zVIEW vSubtask, zVIEW vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_CreateView_ForAddReus( zVIEW vSubtask, zPVIEW vReusableToAdd,
                                    zVIEW vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_TrollReusableToAdd( zVIEW vReusableToAdd, zVIEW  vTZCMRCRO,
                                 zVIEW vTempREPO );
static zSHORT
zwfnTZCMCPLD_LoadMetas( zVIEW vSubtask, zVIEW vReusableCPLR );
static zSHORT
zwfnTZCMCPLD_LoadMetasForCurrentReusables( zVIEW vSubtask );
static zSHORT
zwfnTZCMCPLD_CheckMetasForAddReus( zVIEW vSubtask, zVIEW vCurrentReusables,
                                   zVIEW vReusableToAdd, zBOOL bCheckAllMetas,
                                   zSHORT nCheckType );
static zSHORT
zwfnTZCMCPLD_WriteErrorMsg( zVIEW vSubtask, zVIEW vCurrentReusables,
                            zVIEW vReusableToAdd, zLONG lMetaType,
                            zBOOL bCheckAllMetas, zSHORT nCheckType );
static void
zwfnTZCMCPLD_WriteErrorListTitle( zVIEW vReusError, zLONG  FileHandle );
static zSHORT
zwfnTZCMCPLD_CheckERD_TE_PPE( zVIEW vSubtask, zVIEW vCurrentReusables,
                              zVIEW vReusableToAdd, zBOOL bCheckAllMetas,
                              zSHORT nCheckType );
static void
zwfnTZCMCPLD_Reusable_CreateErrorRoot( zVIEW vSubtask, zVIEW vTZCMRCRO,
                                       zSHORT nCheckType );
static void
zwfnTZCMCPLD_WriteErrorListToFile( zVIEW vReusError, zLONG  FileHandle );
static zSHORT
zwfnTZCMCPLD_ReusGetParentAndCheck( zVIEW vSubtask, zVIEW vTZCMRCRO_All,
                                    zBOOL bCheckAllMetas );
static zSHORT
zwTZCMCPLD_CreateViewForCurrentReusables( zVIEW vSubtask, zPVIEW vCurrentReusables,
                                          zVIEW vTZCMRCRO, zVIEW vReusableCPLR,
                                          zULONG ulZKey );
static zSHORT
zwfnTZCMCPLD_CreateReusablesForCurrentCPLR( zVIEW vSubtask, zULONG ulZKey,
                                            zPVIEW vCurrentReusables );
static void
zwfnTZCMCPLD_GetErrorMsg( zPCHAR szMsg, zLONG lMetaType, zSHORT nCheckType,
                          zPCHAR szMetaName, zPCHAR szMetaType,
                          zVIEW vCurrentReusables, zVIEW vReusableToAdd );
static zSHORT
zwfnTZCMCPLD_LoadAllCPLRs( zVIEW vSubtask, zPVIEW vTZCMRCRO_All );
static zSHORT
zwfnTZCMCPLD_Search_CPLR( zVIEW vSubtask, zVIEW vTZCMRCRO );
static zSHORT
zwfnTZCMCPLD_CreateMetaViews( zVIEW vSubtask, zPVIEW vAllMetas, zPVIEW vMetaList,
                              zBOOL bAllMetas );
static zSHORT
zwfnTZCMCPLD_BuildFileView( zVIEW vSubtask, zPVIEW vTZCMFILE );
static zBOOL
zwfnTZCMCPLD_IsSysadm( zVIEW vSubtask );
static zSHORT
zwfnTZCMCPLD_OKFiles( zVIEW vSubtask, zVIEW vTZCMFILE, zVIEW vCPLR_AllMetas,
                      zLONG lCPLR_ViewCluster, zLONG lFile_ViewCluster,
                      zPBOOL bCommitMetaList );
static zSHORT
zwfnTZCMCPLD_IfZeidonToolOpen( zVIEW vSubtask );
static zSHORT
zwfnTZCMCPLD_CreateNewRepo( zVIEW vSubtask, zPVIEW vTZCMREPO );
static zSHORT
zwfnTZCMCPLD_ActivateREPO_Enqueue( zVIEW vSubtask );
static zSHORT
zwfnTZCMCPLD_ActivateQualForInstall( zVIEW vSubtask, zPVIEW vKZDBHQUA );
static zSHORT
zwfnTZCMCPLD_ActivateWIPO_ZKEY( zVIEW zSubtask );
static zSHORT
zwfnTZCMCPLD_ActivateCPL( zVIEW zSubtask );
static zSHORT
zwTZCMCPLD_CheckLoginValues( zVIEW vSubtask );
static zSHORT
zwTZCMCPLD_CheckRepoEnqueue( zVIEW vSubtask );
static zSHORT
zwTZCMCPLD_SetRepoEnqueue( zVIEW vSubtask );
static zSHORT
zwfnTZCMCPLD_AskForRemoveERD_TE_SA( zVIEW vSubtask, zLONG lType );
static zSHORT
zwfnTZCMCPLD_RemoveStatusERD_TE_SA( zVIEW vCPLR_AllMetas );


static zSHORT
zwfnTZCMCPLD_IfZeidonToolOpen( zVIEW vSubtask )
{
   LPTASK   lpTask;

   // Check to make sure that no other tools are up.
   lpTask = 0;
   while ( (lpTask = SfGetNextTask( lpTask )) != 0 )
   {
      zCHAR szDialogName[ 200 ];

      // Check the name of the dialog associated with the task.  If it begins
      // with TZ then it's a Zeidon Tools task.  We allow the object browser
      // to run and we have to ignore the current task.
      SfGetTaskInfo( szDialogName, zsizeof( szDialogName ), zTASK_DIALOG, lpTask );

      // We allow non-Zeidon tasks...
      if ( zstrncmpi( szDialogName, "TZ", 2 ) != 0 )
         continue;

      // We allow the browser...
      if ( zstrcmpi( szDialogName, "TZOBRWAD" ) == 0 )
         continue;

      // We allow the current task.
      if ( lpTask == SfGetCurrentTask( vSubtask ) )
         continue;

      // Uh-oh...some other Zeidon task is running.
      MessageSend( vSubtask, "CM00141", "Configuration Management",
                   "Zeidon Tools still running.\n"
                   "Shut down all Zeidon Tools before running System "
                   "Administration.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
      return( 1 );
   }

   return( 0 );
}



static zSHORT
zwfnTZCMCPLD_CreateNewRepo( zVIEW vSubtask, zPVIEW vTZCMREPO )
{
   zVIEW   vTZCMWIPO;
   zSHORT  nRC;

   if ( MessagePrompt( vSubtask, "CM00102",
                       "Configuration Management",
                       "Unable to initialize Repository at this time!\n"
                       "Only respond no with the support of \n"
                       "TONBELLER personnel.\n\n"
                       "Exit dialog now?", zBEEP,
                       zBUTTONS_YESNO, zRESPONSE_YES, 0 ) == zRESPONSE_YES )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
      return( -1 );
   }

   nRC = ActivateEmptyObjectInstance( vTZCMREPO, "TZCMREPO", vSubtask,
                                      zSINGLE );

   if ( CreateEntity( *vTZCMREPO, "Installation", zPOS_AFTER ) >= 0 )
   {
      SetAttributeFromInteger( *vTZCMREPO, "Installation", "ZKey", 0 );
      SetAttributeFromInteger( *vTZCMREPO, "Installation", "NextWKS_Id",
                               90 );
      SetAttributeFromString( *vTZCMREPO, "Installation", "Sysadm",
                              "SYSADM" );
      SetAttributeFromString( *vTZCMREPO, "Installation", "Name",
                              "New installation" );
      SetAttributeFromString( *vTZCMREPO, "Installation", "Password",
                              "SYSADM" );
      SetAttributeFromString( *vTZCMREPO, "Installation", "Desc",
                              "Zeidon Repository" );

      CommitObjectInstance( *vTZCMREPO );

      ActivateEmptyObjectInstance( &vTZCMWIPO, "TZCMWIPO", vSubtask, zSINGLE );
      CreateEntity( vTZCMWIPO, "ROOT", zPOS_AFTER );
      SetNameForView( vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

      SetAttributeFromString( vTZCMWIPO, "ROOT", "UserName", "SYSADM" );
      SetAttributeFromString( vTZCMWIPO, "ROOT", "Password", "SYSADM" );
      SetAttributeFromInteger( vTZCMWIPO, "ROOT", "Sysadm", 1 );
   }

   return( 0 );
}


static zSHORT
zwfnTZCMCPLD_ActivateREPO_Enqueue( zVIEW vSubtask )
{
   zVIEW   vTZCMQSYO;
   zSHORT  nRC;
   zCHAR   szMsg[512];
   zCHAR   szDateEnqueue[ 20 ];

   // look, whether there's already someone in the SystemAdmin. and issue a warning.
   nRC = oTZCMQSYO_ActivateREPO_Enqueue( &vTZCMQSYO, vSubtask );
   if ( nRC >= zCURSOR_SET )
   {
      // go to the last (latest) entry:
      OrderEntityForView( vTZCMQSYO, "SysEnqueue", "TimeStamp" );
      SetCursorLastEntity( vTZCMQSYO, "SysEnqueue", "" );

      strcpy_s( szMsg, zsizeof( szMsg ), "The repository was enqueued \non\t" );
      GetStringFromAttributeByContext( szDateEnqueue,
                                       vTZCMQSYO, "SysEnqueue", "TimeStamp",
                                       "DD.MM.YYYY HH:MM:SS", 19 );
      strcat_s( szMsg, zsizeof( szMsg ), szDateEnqueue );
      strcat_s( szMsg, zsizeof( szMsg ), "\nby\t" );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ),
                           vTZCMQSYO, "SysEnqueue", "UserName" );
      strcat_s( szMsg, zsizeof( szMsg ), " (" );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ),
                           vTZCMQSYO, "SysEnqueue", "UserDesc" );
      strcat_s( szMsg, zsizeof( szMsg ), "). \n\n " );
      strcat_s( szMsg, zsizeof( szMsg ), "Respond YES to exit the dialog now.\n" );
      strcat_s( szMsg, zsizeof( szMsg ), "Responding NO will allow you into the dialog,\n" );
      strcat_s( szMsg, zsizeof( szMsg ), "however, any current enqueues will be deleted.\n" );
      strcat_s( szMsg, zsizeof( szMsg ), " Exit Dialog Now?" );

      if ( MessagePrompt( vSubtask, "CM00103", "Configuration Management",
                          szMsg, zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_YES, 0 ) == zRESPONSE_YES )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
         return( -1 );
      }
      DropView( vTZCMQSYO );
   }

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_ActivateQualForInstall( zVIEW vSubtask, zPVIEW vKZDBHQUA )
{
   // Activate a single instance of the REPO object.  A keyed access is
   // made because of performance.
   ActivateEmptyObjectInstance( vKZDBHQUA, "KZDBHQUA", vSubtask, zSINGLE );
   CreateEntity( *vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   CreateEntity( *vKZDBHQUA, "QualAttrib", zPOS_AFTER );

   SetAttributeFromString( *vKZDBHQUA, "EntitySpec", "EntityName", "Installation" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "EntityName", "Installation" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Oper", "=" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Value", "0" );

   return( 0 );
}


static zSHORT
zwfnTZCMCPLD_ActivateWIPO_ZKEY( zVIEW vSubtask )
{
   zVIEW    vTZCMWIPO;
   zVIEW    vTZCMZKEY;
   zVIEW    vKZDBHQUA;
   zSHORT   nRC;

   if ( GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // Create a work object for entering the Name and password.
      ActivateEmptyObjectInstance( &vTZCMWIPO, "TZCMWIPO", vSubtask,
                                   zSINGLE );
      CreateEntity( vTZCMWIPO, "ROOT", zPOS_AFTER );
      SetNameForView( vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );
   }

   zwfnTZCMCPLD_ActivateQualForInstall( vSubtask, &vKZDBHQUA );
   nRC = ActivateObjectInstance( &vTZCMZKEY, "TZCMZKEY", vSubtask,
                                 vKZDBHQUA, zSINGLE );

   DropObjectInstance( vKZDBHQUA );

   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "CM00101", "Configuration Management",
                   "Unable to load Installation!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
      return( -1 );
   }

   SetNameForView( vTZCMZKEY,  "TZCMZKEY",  vSubtask, zLEVEL_TASK );
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_ActivateCPL( zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zVIEW    vTZCMCPL;
   zSHORT   nRC;

   if ( GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwfnTZCMCPLD_ActivateQualForInstall( vSubtask, &vKZDBHQUA );
      nRC = ActivateObjectInstance( &vTZCMCPL, "TZCMCPL", vSubtask,
                                    vKZDBHQUA, zSINGLE );

      DropObjectInstance( vKZDBHQUA );

      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "CM00101", "Configuration Management",
                      "Unable to load CPLR List!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
         return( -1 );
      }
      SetNameForView( vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );
   }

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPL" ) >= zCURSOR_SET )
      OrderEntityForView( vTZCMCPL, "CPL", "Name A " );

   SetCursorFirstEntity( vTZCMCPL, "CPLR", "Installation" );
   return( 0 );
}



zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_InitSignOn( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vKZDBHQUA;
   zSHORT   nRC;
   zSHORT   ReposIsEmpty = 0;

   // Check to make sure that no other tools are up.
   if ( zwfnTZCMCPLD_IfZeidonToolOpen( vSubtask ) == 1 )
      return( 0 );

   // Activate a single instance of the REPO object.  A keyed access is
   // made because of performance.
   zwfnTZCMCPLD_ActivateQualForInstall( vSubtask, &vKZDBHQUA );
   nRC = ActivateObjectInstance( &vTZCMREPO, "TZCMREPO", vSubtask,
                                 vKZDBHQUA, zSINGLE );
   DropObjectInstance( vKZDBHQUA );

   if ( nRC == zCALL_ERROR )
   {
      MessageSend( vSubtask, "CM00101", "Configuration Management",
                   "Unable to activate Repository at this time!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
      return( 0 );
   }

   if ( nRC == -1 )
      ReposIsEmpty = 1;

   if ( ReposIsEmpty == 1 &&
        zwfnTZCMCPLD_CreateNewRepo( vSubtask, &vTZCMREPO ) < 0 )
   {
      return( 0 );
   }

   if ( zwfnTZCMCPLD_ActivateCPL( vSubtask ) < 0 )
      return( 0 );

   if ( zwfnTZCMCPLD_ActivateWIPO_ZKEY( vSubtask ) < 0 )
      return( 0 );

   // Determine if there's already someone in the SystemAdmin. If so, issue
   // a warning.
   if ( zwfnTZCMCPLD_ActivateREPO_Enqueue( vSubtask ) < 0 )
      return( -1 );

   SetNameForView( vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveInstall( zVIEW vSubtask )
{
   zVIEW  vTZCMREPO;
   zLONG  lRepo_ViewCluster;
   zLONG  lErrorIndex;
   zSHORT nRC;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   // Get Repo View Cluster.and commit it
   GetIntegerFromAttribute( &lRepo_ViewCluster, vTZCMREPO, "Installation",
                           "Repo_ViewCluster" );

   nRC = CommitMultipleObjectInstances( lRepo_ViewCluster, &lErrorIndex );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "CM00101", "Configuration Management",
                   "Unable to save Repository View Cluster!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( nRC );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_ExitDialog( zVIEW vSubtask )
{
   zSHORT   nRC;
   zLONG    lRepo_ViewCluster;
   zVIEW    vTZCMREPO;
   zVIEW    vTZCMQSYO;
   zVIEW    vView;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) &&
        (ObjectInstanceUpdatedFromFile( vTZCMREPO ) ||
         CompareAttributeToInteger( vTZCMREPO, "Installation",
                                    "UpdateAuditTrail", 1 ) == 0) )
   {
      nRC = MessagePrompt( vSubtask, "DM00115", "Configuration Maintenance",
                           "Repository has been modified, save it?", zBEEP,
                           zBUTTONS_YESNOCANCEL, zRESPONSE_YES, zICON_QUESTION );

      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
      else
      if ( nRC == zRESPONSE_YES )
      {
         if ( zwTZCMCPLD_SaveInstall( vSubtask ) < 0 )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }
      }
   } // endif ( ObjectInstanceUpdatedFromFile( vTZCMREPO ) )

   if ( GetViewByName( &vView, "TZCMREVO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZCMCPL", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZCMZKEY", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZCMWIPO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZCMCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZCMREUS", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vTZCMQSYO, "TZCMQSYO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DeleteEntity( vTZCMQSYO, "SysEnqueue", zREPOS_NONE );

      if ( CommitObjectInstance( vTZCMQSYO ) == 0 )
         DropObjectInstance( vTZCMQSYO );
   }

   // Get Repo View Cluster.and drop it
   GetIntegerFromAttribute( &lRepo_ViewCluster, vTZCMREPO, "Installation",
                            "Repo_ViewCluster" );
   DropViewCluster( lRepo_ViewCluster );
   DropObjectInstance( vTZCMREPO );

   return( 0 );
}


static zSHORT
zwTZCMCPLD_CheckLoginValues( zVIEW vSubtask )
{
   zVIEW  vTZCMREPO;
   zVIEW  vTZCMWIPO;
   zCHAR  szName[ 33 ];
   zCHAR  szPassword[9];

   if ( GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK ) < 0 )
   {
     MessageSend( vSubtask, "CM00107", "Configuration Management",
                  "Internal Error: View TZCMWIPO not found (SignOnOK) !",
                  zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
     SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
     return( -1 );
   }

   if ( GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK ) < 0 )
   {
     MessageSend( vSubtask, "CM00107", "Configuration Management",
                  "Internal Error: View TZCMREPO not found (SignOnOK) !",
                  zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
     SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
     return( -1 );
   }

   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMWIPO, "ROOT", "UserName" );
   GetStringFromAttribute( szPassword, zsizeof( szPassword ), vTZCMWIPO, "ROOT", "Password" );

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      if ( CompareAttributeToAttribute( vTZCMWIPO, "ROOT",         "Password",
                                        vTZCMREPO, "Installation", "Password" ) == 0 )
      {
         SetAttributeFromInteger( vTZCMWIPO, "ROOT", "Sysadm", 1 );
      }
      else
      {
         MessageSend( vSubtask, "CM00107", "Configuration Management",
                      "Password is invalid for Sysadm!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetFocusToCtrl( vSubtask, "edPassword" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }
   else
   {
      if ( SetCursorFirstEntityByString( vTZCMREPO, "User", "Name",
                                          szName, "" ) != zCURSOR_SET )
      {
         MessageSend( vSubtask, "CM00108", "Configuration Management",
                      "User is Not registered to Repository!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetFocusToCtrl( vSubtask, "edName" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      if ( CompareAttributeToAttribute( vTZCMWIPO, "ROOT", "Password",
                                        vTZCMREPO, "User", "Password" ) != 0 )
      {
         MessageSend( vSubtask, "CM00109", "Configuration Management",
                      "Password is invalid for User!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetFocusToCtrl( vSubtask, "edPassword" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   return( 0 );
}



static zSHORT
zwTZCMCPLD_CheckRepoEnqueue( zVIEW vSubtask )
{
   zVIEW    vTZCMQSYO;
   zVIEW    vTZCMQSYO_All;
   zSHORT   nRC;
   zCHAR    szMsg[512];
   zCHAR    szDateEnqueue[ 20 ];

   if ( GetViewByName( &vTZCMQSYO, "TZCMQSYO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );


   // look, whether there's already someone in the SystemAdmin. and issue a warning.
   nRC = oTZCMQSYO_ActivateREPO_Enqueue( &vTZCMQSYO_All, vSubtask );
   if ( nRC >= zCURSOR_SET )
   {
      // go to the last (latest) entry:
      OrderEntityForView( vTZCMQSYO_All, "SysEnqueue", "TimeStamp" );
      SetCursorLastEntity( vTZCMQSYO_All, "SysEnqueue", "" );

      if ( CompareAttributeToAttribute( vTZCMQSYO_All, "SysEnqueue", "UserName",
                                        vTZCMQSYO, "SysEnqueue", "UserName" ) == 0 )
      {
         nRC = SetCursorPrevEntity( vTZCMQSYO_All, "SysEnqueue", "" );
      }

      if ( nRC >= zCURSOR_SET )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Be careful, the repository was enqueued \non\t" );
         GetStringFromAttributeByContext( szDateEnqueue,
                                          vTZCMQSYO_All, "SysEnqueue", "TimeStamp",
                                          "DD.MM.YYYY HH:MM:SS", 19 );
         strcat_s( szMsg, zsizeof( szMsg ), szDateEnqueue );
         strcat_s( szMsg, zsizeof( szMsg ), "\nby\t" );
         GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ),
                                 vTZCMQSYO_All, "SysEnqueue", "UserName" );
         strcat_s( szMsg, zsizeof( szMsg ), " (" );
         GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ),
                                 vTZCMQSYO_All, "SysEnqueue", "UserDesc" );
         strcat_s( szMsg, zsizeof( szMsg ), ")." );

         MessageSend( vSubtask, "CM00107", "Configuration Management",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      }
   }
   DropObjectInstance( vTZCMQSYO_All );

   return( 0 );
}


static zSHORT
zwTZCMCPLD_SetRepoEnqueue( zVIEW vSubtask )
{
   zVIEW    vTZCMWIPO;
   zVIEW    vTZCMQSYO;
   zVIEW    vKZDBHQUA;
   zSHORT   nRC;
   zCHAR    szTimeStamp[ 22 ];

   GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

   nRC = ActivateEmptyObjectInstance( &vTZCMQSYO, "TZCMQSYO",
                                      vSubtask, zSINGLE );
   CreateEntity( vTZCMQSYO, "SysEnqueue", zPOS_AFTER );

   SysGetDateTime( szTimeStamp, zsizeof( szTimeStamp ) );
   SetAttributeFromString( vTZCMQSYO, "SysEnqueue", "TimeStamp", szTimeStamp );
   SetAttributeFromAttribute( vTZCMQSYO, "SysEnqueue", "UserName",
                              vTZCMWIPO, "ROOT", "UserName" );
   // Frank H.: This statement may in future be replaced by the description
   // of the User, who is logged on.
   SetAttributeFromString( vTZCMQSYO, "SysEnqueue", "UserDesc",
                           "SystemAdministrator" );

   nRC = CommitObjectInstance( vTZCMQSYO );
   if ( nRC == zDUPLICATE_ROOT )
   {
      // if this user has already an Enqueue, drop the View and
      // reload it from the Database:
      DropObjectInstance( vTZCMQSYO );
      if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA",
                                        vSubtask, zSINGLE ) >= 0 )
      {
         SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
         CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
         CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      }
      else
         return( -1 );

      // Build the QualificationObject for the ActivateByName:
      SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName",
                              "SysEnqueue" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName",
                              "SysEnqueue" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName",
                              "UserName" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
      SetAttributeFromAttribute( vKZDBHQUA, "QualAttrib", "Value",
                                 vTZCMWIPO, "ROOT", "UserName" );

      nRC = ActivateObjectInstance( &vTZCMQSYO, "TZCMQSYO", vSubtask,
                              vKZDBHQUA, zLEVEL_TASK );
      DropObjectInstance( vKZDBHQUA );
   }
   else
   if ( nRC < zCURSOR_SET )
   {
      MessageSend( vSubtask, "CM00107", "Configuration Management",
                   "Unknown Error during Commit of Enqueue Object for SystemAdministration",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   SetNameForView( vTZCMQSYO, "TZCMQSYO", vSubtask, zLEVEL_TASK );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SignOnOK( zVIEW vSubtask )
{
   zVIEW   vTZCMREPO;
   zLONG   lRepo_ViewCluster;

   if ( zwTZCMCPLD_CheckLoginValues( vSubtask ) < 0 )
      return( -1 );

   if ( zwTZCMCPLD_SetRepoEnqueue( vSubtask ) < 0 )
      return( -1 );

   // look, whether there's already someone in the SystemAdmin. and issue a warning.
   zwTZCMCPLD_CheckRepoEnqueue( vSubtask );

   // Create Repository View Cluster
   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   CreateViewCluster( vSubtask, &lRepo_ViewCluster );
   AddToViewCluster( lRepo_ViewCluster, vTZCMREPO, 0 );

   SetAttributeFromInteger( vTZCMREPO, "Installation", "Repo_ViewCluster",
                            lRepo_ViewCluster );
   SetIncrementalUpdateFlags( vTZCMREPO, "Installation", zSET_INCR_NOT_UPDATED );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DropUserView( zVIEW vSubtask )
{
   zVIEW  vTZCMUSER;

   if ( GetViewByName( &vTZCMUSER, "TZCMUSER", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMUSER );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CPL_PopUp( zVIEW vSubtask )
{
   zVIEW  vTZCMCPL;
   zBOOL  bSysadm = FALSE;
   zBOOL  bDelete = FALSE;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      bSysadm = TRUE;
      bDelete = TRUE;
   }

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPL" ) < zCURSOR_SET )
      bDelete = FALSE;

   EnableAction( vSubtask, "TransferToCPL_Add", bSysadm );
   EnableAction( vSubtask, "DeleteCPL", bDelete );
   EnableAction( vSubtask, "TransferToAddCPLR", bSysadm );
   EnableAction( vSubtask, "DeleteCPLR_WithEnq", bDelete );

   CreateTrackingPopupMenu( vSubtask, "CPL_PopUp", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_InitMenu
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_InitMenu( zVIEW vSubtask )
{
   zVIEW vTZCMCPL;
   zBOOL bSysadm = FALSE;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      bSysadm = TRUE;
   }

   SetOptionState( vSubtask, "Password", zOPTION_STATUS_ENABLED, bSysadm );
   SetOptionState( vSubtask, "NewCPL", zOPTION_STATUS_ENABLED, bSysadm );
   SetOptionState( vSubtask, "NewUser", zOPTION_STATUS_ENABLED, bSysadm );
   SetOptionState( vSubtask, "SaveRepository", zOPTION_STATUS_ENABLED, bSysadm );

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPL" ) < zCURSOR_SET )
      bSysadm = FALSE;

   SetOptionState( vSubtask, "NewCPLR", zOPTION_STATUS_ENABLED, bSysadm );
   SetOptionState( vSubtask, "SaveAsCPLR", zOPTION_STATUS_ENABLED, bSysadm );
   SetOptionState( vSubtask, "DeleteAuditTrail", zOPTION_STATUS_ENABLED, bSysadm );

   return( 0 );
} // zwTZCMCPLD_InitMenu

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DisableControls( zVIEW vSubtask )
{
   zVIEW    vTZCMWIPO;
   zVIEW    vTZCMREPO;

   GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

   // If the person signing on is not the System Administrator, protect the password
   // action, review types and the NextWKS_Id field.
   if ( GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CompareAttributeToAttribute( vTZCMREPO, "Installation", "Sysadm",
                                        vTZCMWIPO, "ROOT",         "UserName" ) == 0 )
      {
         SetCtrlState( vSubtask, "NextWKS_Id", zCONTROL_STATUS_ENABLED, TRUE );
         SetCtrlState( vSubtask, "Name", zCONTROL_STATUS_ENABLED, TRUE );
         SetCtrlState( vSubtask, "Desc", zCONTROL_STATUS_ENABLED, TRUE );
         SetCtrlState( vSubtask, "SYSADM", zCONTROL_STATUS_ENABLED, TRUE );
      }
      else
      {
         SetCtrlState( vSubtask, "NextWKS_Id", zCONTROL_STATUS_ENABLED, FALSE );
         SetCtrlState( vSubtask, "Name", zCONTROL_STATUS_ENABLED, FALSE );
         SetCtrlState( vSubtask, "Desc", zCONTROL_STATUS_ENABLED, FALSE );
         SetCtrlState( vSubtask, "SYSADM", zCONTROL_STATUS_ENABLED, FALSE );
      }
   }

   zwTZCMCPLD_SetDIL_Message( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_AddAnother( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vWindow;

   if ( zwTZCMCPLD_AddInstallUser( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   SetSelectStateOfEntity( vTZCMREPO, "User", 0 );
   CreateInstallEntity( vSubtask, vTZCMREPO, "User", zPOS_AFTER );
   SetSelectStateOfEntity( vTZCMREPO, "User", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OpenUsers" ) >= 0 )
      RefreshWindow( vWindow );

   SetCtrlText( vSubtask, "edVerification", "" );
   SetFocusToCtrl( vSubtask, "UserName" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteCPL( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // Don't allow a CPL to be deleted if it has subordinate CPLRs.
   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "CM00144", "Configuration Management",
                   "You cannot delete a CPL with existing CPLRs.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPL", "Name" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete CPL '" );
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( MessagePrompt( vSubtask,  "CM00110", "Configuration Management", szMsg,
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   // Now delete the CPL and commit the result.
   DeleteEntity( vTZCMCPL, "CPL", zREPOS_NONE );
   CommitObjectInstance( vTZCMCPL );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_TransferToAddCPLR( zVIEW vSubtask )
{
   zVIEW vTZCMCPLR;
   zVIEW vTZCMCPL;
   zLONG lCPLR_ViewCluster;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // Initialize the CPLR object to a single, empty entity.
   ActivateEmptyObjectInstance( &vTZCMCPLR, "TZCMCPLR", vSubtask,
                                zSINGLE );
   CreateInstallEntity( vSubtask, vTZCMCPLR, "CPLR", zPOS_AFTER );
   SetNameForView( vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );

   // Create CPLR View Cluster
   CreateViewCluster( vSubtask, &lCPLR_ViewCluster );
   AddToViewCluster( lCPLR_ViewCluster, vTZCMCPL, 0 );
   AddToViewCluster( lCPLR_ViewCluster, vTZCMCPLR, 0 );

   SetAttributeFromInteger( vTZCMCPLR, "CPLR", "CPLR_ViewCluster",
                            lCPLR_ViewCluster );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CancelCPLR( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vTZCMCPLR;
   zVIEW    vReusableCPLR;
   zLONG    lCPLR_ViewCluster;

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPL,  "TZCMCPL", vSubtask, zLEVEL_TASK );

   oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );

   // If the CPL does not contain a CPLR in the REPO object,
   // then delete it.  This means the CPLR being canceled must have
   // been created as part of a "new" CPL.
   if ( CheckExistenceOfEntity ( vTZCMCPL, "CPLR" ) != zCURSOR_SET )
      DeleteEntity( vTZCMCPL, "CPL", zREPOS_BEFORE );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   // Drop Component Views
   zwTZCMCPLD_DropComponentViews( vSubtask );

   // Drop CPLR View Cluster
   GetIntegerFromAttribute( &lCPLR_ViewCluster, vTZCMCPLR, "CPLR",
                            "CPLR_ViewCluster" );
   DropViewCluster( lCPLR_ViewCluster );
   SetAttributeFromInteger( vTZCMCPLR, "CPLR", "CPLR_ViewCluster", 0 );

   DropObjectInstance( vTZCMCPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteCPLR( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vTZCMCPRO;
   zVIEW    vReusableCPLR;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;
   zSHORT   nNewCPLR;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // Load the CPLR object and get its view.
   nNewCPLR = zwfnTZCMCPLD_LoadCPLR_ForViewName( vSubtask, "TZCMCPRO" );
   if ( nNewCPLR < 0 )
      return( -1 );

   GetViewByName( &vTZCMCPRO, "TZCMCPRO", vSubtask, zLEVEL_TASK );

   // Ask the operator if it is ok to delete.
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "CPLR", "Name" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete CPLR '" );
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( MessagePrompt( vSubtask, "CM00111", "Configuration Management", szMsg,
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   if ( nNewCPLR == 0 &&
        zwfnTZCMCPLD_DeleteMetas( vSubtask ) <= 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   DeleteEntity( vTZCMCPRO, "CPLR", zREPOS_NONE );
   nRC = CommitObjectInstance( vTZCMCPRO );

   // Frank H.: Exclude the Object in the TZCMCPL (ListView for main Window in SysAdm)
   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
      ExcludeEntity( vTZCMCPL, "CPLR", zREPOS_NONE );

   // If the CPL does not contain a CPLR in the REPO object,
   // then delete it.  This means this is the last CPLR for the
   // CPL, which cannot exist without at least one CPLR.
   if ( SetCursorFirstEntity( vTZCMCPL, "CPLR", "" ) != zCURSOR_SET )
   {
      DeleteEntity( vTZCMCPL, "CPL", zREPOS_BEFORE );
   }

   CommitObjectInstance( vTZCMCPL );
   DropObjectInstance( vTZCMCPRO );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteCPLR_WithEnq( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vTZCMCPRO;
   zULONG   ulZKey;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
      return( 0 );

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // Load the CPLR object and get its view.
   if ( zwTZCMCPLD_LoadCPRO_WithEnq( vSubtask ) < 0 )
   {
      return( -1 );
   }

   GetViewByName( &vTZCMCPRO, "TZCMCPRO", vSubtask, zLEVEL_TASK );

   // Ask the operator if it is ok to delete.
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete CPLR '" );
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( MessagePrompt( vSubtask, "CM00111", "Configuration Management", szMsg,
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO, vSubtask, zCPLR_DEQUEUE );
      DropObjectInstance( vTZCMCPRO );
      return( 0 );
   }

   if ( zwfnTZCMCPLD_DeleteMetas( vSubtask ) <= 0 )
   {
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO, vSubtask, zCPLR_DEQUEUE );
      return( 0 );
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );

   oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO, vSubtask, zCPLR_DEQUEUE );

   //  relink the TZCMCPL.CPLR to TZCMCPRO.CPLR so that
   //  no problem occurs, when TZCMCPL is committed.
   RelinkInstanceToInstance( vTZCMCPRO, "CPLR",
                             vTZCMCPL, "CPLR" );

   DeleteEntity( vTZCMCPRO, "CPLR", zREPOS_NONE );
   nRC = CommitObjectInstance( vTZCMCPRO );

   // If the CPL does not contain a CPLR in the REPO object,
   // then delete it.  This means this was the last CPLR
   // for the CPL, which cannot exist without at least one CPLR.
   if ( SetCursorFirstEntity( vTZCMCPL, "CPLR", "" ) != zCURSOR_SET )
   {
      DeleteEntity( vTZCMCPL, "CPL", zREPOS_BEFORE );
   }

   CommitObjectInstance( vTZCMCPL );
   DropObjectInstance( vTZCMCPRO );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteMetaFromListAll( zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zVIEW    vCPLR_AllMetas;
   zVIEW    vTZCMFILE;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zBOOL    bDeleteMeta = FALSE;
   zSHORT   nRC;

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( MessagePrompt( vSubtask, "CM00112", "Configuration Management",
                       "OK to delete selected Components?",
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Set up qualification object in advance.
   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   }
   else
      return( -1 );

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) < 0 )
      zwfnTZCMCPLD_BuildFileView( vSubtask, &vTZCMFILE );

   // Loop for each selected meta, asking user for delete verification and
   // then deleting the meta.
   for ( nRC = SetCursorFirstSelectedEntity( vCPLR_AllMetas, "Z_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vCPLR_AllMetas, "Z_MetaDef", 0 ) )
   {
      if ( CheckExistenceOfEntity( vCPLR_AllMetas, "UserMetaStatus" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vCPLR_AllMetas, "Z_MetaDef", "Name" );
         strcpy_s( szMsg, zsizeof( szMsg ), "Meta '" );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "' could not be deleted because it is checked out.\n" );
         strcat_s( szMsg, zsizeof( szMsg ), "Remove check out status before deleting." );
         MessageSend( vSubtask, "CM00145", "Configuration Management",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         continue;
      }

      bDeleteMeta = TRUE;
      nRC = oTZCMCPRO_DeleteMetaWithCluster( vCPLR_AllMetas, vTZCMFILE,
                                             vKZDBHQUA, vSubtask );
   }

   if ( bDeleteMeta ) // set Update flag for Delete in View
      SetAttributeFromInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta", 1 );

   DropObjectInstance( vKZDBHQUA );

   MB_SetMessage( vSubtask, 1, "Meta deletion complete." );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteMetaFromList( zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zVIEW    vCPLR_AllMetas;
   zVIEW    vCPLR_MetaList;
   zVIEW    vTZCMFILE;
   zULONG   ulZKey;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zBOOL    bDeleteMeta = FALSE;
   zSHORT   nRC;

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas",
                       vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( &vCPLR_MetaList, "CPLR_MetaList",
                       vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   if ( MessagePrompt( vSubtask, "CM00112", "Configuration Management",
                       "OK to delete selected Components?",
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Set up qualification object in advance.
   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   }
   else
      return( -1 );

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) < 0 )
      zwfnTZCMCPLD_BuildFileView( vSubtask, &vTZCMFILE );

   // Loop for each selected meta, asking user for delete verification and
   // then deleting the meta.
   for ( nRC = SetCursorFirstSelectedEntity( vCPLR_MetaList, "Z_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vCPLR_MetaList, "Z_MetaDef", 0 ) )
   {
      if ( CheckExistenceOfEntity( vCPLR_MetaList, "UserMetaStatus" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vCPLR_MetaList, "Z_MetaDef", "Name" );
         strcpy_s( szMsg, zsizeof( szMsg ), "Meta '" );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "' could not be deleted because it is checked out.\n" );
         strcat_s( szMsg, zsizeof( szMsg ), "Remove check out status before deleting." );
         MessageSend( vSubtask, "CM00145", "Configuration Management",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         continue;
      }

      GetIntegerFromAttribute( (zPLONG) &ulZKey, vCPLR_MetaList,
                               "Z_MetaDef", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vCPLR_AllMetas, "Z_MetaDef", "ZKey",
                                          ulZKey, "" ) >= zCURSOR_SET )
      {
         bDeleteMeta = TRUE;
         nRC = oTZCMCPRO_DeleteMetaWithCluster( vCPLR_AllMetas, vTZCMFILE,
                                                vKZDBHQUA, vSubtask );
         DeleteEntity( vCPLR_MetaList, "Z_MetaDef", zREPOS_NONE );
      }
   }

   DropObjectInstance( vKZDBHQUA );
   zwTZCMCPLD_BuildMetaList( vSubtask );

   if ( bDeleteMeta ) // set Update flag for Delete in View
      SetAttributeFromInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta", 1 );

   MB_SetMessage( vSubtask, 1, "Meta deletion complete." );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteMeta( zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zVIEW    vCPLR_AllMetas;
   zVIEW    vTZCMFILE;
   zVIEW    vWindow;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetStringFromAttribute( szName, zsizeof( szName ), vCPLR_AllMetas, "Z_MetaDef", "Name" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Component '");
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( MessagePrompt( vSubtask, "CM00112", "Configuration Management", szMsg,
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   }
   else
      return( -1 );

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) < 0 )
      zwfnTZCMCPLD_BuildFileView( vSubtask, &vTZCMFILE );

   nRC = oTZCMCPRO_DeleteMetaWithCluster( vCPLR_AllMetas, vTZCMFILE,
                                          vKZDBHQUA, vSubtask );

   // set Update flag for View
   SetAttributeFromInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta", 1 );
   DropObjectInstance( vKZDBHQUA );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Components" ) >= 0 )
   {
      if ( GetCtrlState( vWindow, "lbMetaList", zCONTROL_STATUS_VISIBLE ) )
         zwTZCMCPLD_BuildMetaList( vSubtask );

      RefreshWindow( vWindow );
   }

   return( nRC );
}

static zSHORT
zwfnTZCMCPLD_DeleteMetas( zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zVIEW    vTZCMCPRO;
   zVIEW    vFile;
   zLONG    lZKey;
   zLONG    lType;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;

   if ( GetViewByName( &vTZCMCPRO, "TZCMCPRO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   //Frank H. (21/4/1997): Delete in inverse order.
   //This means: First delete the Dialog, then LOD, then TE, ERD, Domain...
   //since I got an error when i was implementing
   // SetCursorLast --> SerCursorPrev --> SetCursorPrev ...,,
   // I'm reordering the Z_MetaDefs and then proceed:
   // SetCursorFirst --> SerCursorNext --> SetCursorNext ...,,
   OrderEntityForView( vTZCMCPRO, "Z_MetaDef", "Type D Name A" );

   nRC = SetCursorFirstEntity( vTZCMCPRO, "Z_MetaDef", "" );

   if ( nRC != zCURSOR_SET )
      return( 1 );

   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO, "CPLR", "Name" );
   strcpy_s( szMsg, zsizeof( szMsg ), "CPLR " );
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   strcat_s( szMsg, zsizeof( szMsg ), " contains Metas. Deleting\n the CPLR will delete the Components as ");
   strcat_s( szMsg, zsizeof( szMsg ), "well.\nDo you wish to continue with the delete?");

   if ( MessagePrompt( vSubtask, "CM00113", "Configuration Management",
                       szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                       0 ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   }
   else
      return( -1 );

   MB_SetMessage( vSubtask, 2, "" );

   for ( nRC = SetCursorFirstEntity( vTZCMCPRO, "Z_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZCMCPRO, "Z_MetaDef", 0 ) )
   {
      GetIntegerFromAttribute( &lType, vTZCMCPRO, "Z_MetaDef", "Type" );
      nRC = oTZCMCPRO_DeleteMeta( vTZCMCPRO, vKZDBHQUA, vSubtask );
   }

   DropObjectInstance( vKZDBHQUA );

   // Delete any Shadow File entries that were not deleted with individual
   // metas (like XODs for LODs from a reusable CPLR).
   for ( nRC = SetCursorFirstEntity( vTZCMCPRO, "File", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZCMCPRO, "File", 0 ) )
   {
      GetIntegerFromAttribute( &lZKey, vTZCMCPRO, "File", "ZKey" );
      nRC = oTZCMLPLO_ActivateFileOI( &vFile, vTZCMCPRO, lZKey );
      if ( nRC >= 0 )
      {
         // If the activate found a file, we will delete it.
         if ( nRC >= 0 )
         {
            //  we relink the FILE to TZCMCPRO.
            //  so, by deleting FILE, the TZCMCPRO file entity
            //  would get the "deleted" marking.
            //  This is important, when TZCMCPRO is committed.

            RelinkInstanceToInstance( vFile, "File",
                                      vTZCMCPRO, "File" );
            // Delete the File object.
            DeleteEntity( vFile, "File", zREPOS_NONE );
            nRC = CommitObjectInstance( vFile );
         }
      }
      else
      {
         MessageSend( vSubtask, "CM00299", "Configuration Management",
                      "File not found",
                      zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );

         // Get rid of the entry in the CPLR anyway.
         ExcludeEntity( vTZCMCPRO, "File", zREPOS_NONE );
      }
   }

   MB_SetMessage( vSubtask, 1, "Delete CPLR completed successfully" );
   return( 1 );
}



zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_XferToAddUser( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vWindow;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMREPO, "User" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZCMREPO, "User", 0 );

   CreateInstallEntity( vSubtask, vTZCMREPO, "User", zPOS_AFTER );

   SetSelectStateOfEntity( vTZCMREPO, "User", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OpenUsers" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteInstallUser( zVIEW    vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vTZCMUSER;
   zSHORT   nRC;
   zCHAR    szMsg[ 100 ];
   zCHAR    szUserName[ 33 ];
   zCHAR    szUserDesc[ 33 ];

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   // Make sure that the user is not authorized for any CPLR.
   // A User cannot be deleted if he is still authorized for a CPLR.
   if ( zwTZCMCPLD_LoadUser( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vTZCMUSER, "TZCMUSER", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstEntity( vTZCMUSER, "AccessAuthority", "" ) >= zCURSOR_SET )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "You cannot delete a User with current authority." );
      MessageSend( vSubtask, "CM00114", "Configuration Management",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Ask if delete is OK.
   GetStringFromAttribute( szUserName, zsizeof( szUserName ), vTZCMREPO, "User", "Name" );
   GetStringFromAttribute( szUserDesc, zsizeof( szUserDesc ), vTZCMREPO, "User", "Desc" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete User '" );
   strcat_s( szMsg, zsizeof( szMsg ), szUserName );
   strcat_s( szMsg, zsizeof( szMsg ), " (" );
   strcat_s( szMsg, zsizeof( szMsg ), szUserDesc );
   strcat_s( szMsg, zsizeof( szMsg ), ")'?" );
   nRC = MessagePrompt( vSubtask, "CM00140", "Configuration Management",
                        szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

   if ( nRC == zRESPONSE_YES )
   {
      nRC = DeleteEntity( vTZCMREPO, "User", zREPOS_NEXT );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_AddInstallUser( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zCHAR    szUserName[ 33 ];
   zVIEW    vTZCMREPO_Work;
   zCHAR    szPassword[9];

   // Make sure user name isn't a duplicate.
   // Note that because the new User entry exists in REPO, we do
   // the SetCursor twice.
   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szUserName, zsizeof( szUserName ), vTZCMREPO, "User", "Name" );

   CreateViewFromViewForTask( &vTZCMREPO_Work, vTZCMREPO, 0 );
   SetCursorFirstEntityByString( vTZCMREPO_Work, "User", "Name", szUserName, "" );

   if ( SetCursorNextEntityByString( vTZCMREPO_Work, "User", "Name", szUserName,
                                     "" ) >= zCURSOR_SET )
   {
       MessageSend( vSubtask, "CM00115", "Configuration Management",
                    "User already exists.",
                    zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
       SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
       SetFocusToCtrl( vSubtask, "UserName" );
       DropView( vTZCMREPO_Work );
       return( -1 );
   }

   DropView( vTZCMREPO_Work );

   GetStringFromAttribute( szPassword, zsizeof( szPassword ), vTZCMREPO, "User", "Password" );
   if ( zwfnTZCMCPLD_CheckPassword( vSubtask, szPassword ) < 0 )
      return( -1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CancelAddUser( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   DeleteEntity( vTZCMREPO, "User", zREPOS_PREV );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_GetNextUser( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    ParentWindow;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   zwTZCMCPLD_DropUserView( vSubtask );

   SetSelectStateOfEntity( vTZCMREPO, "User", 0 );
   SetCursorNextEntity( vTZCMREPO, "User", "" );

   SetSelectStateOfEntity( vTZCMREPO, "User", 1 );

   if ( GetParentWindow( &ParentWindow, vSubtask ) == 0 )
      RefreshWindow( ParentWindow );

   zwTZCMCPLD_LoadUser( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_GetPrevUser( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    ParentWindow;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   zwTZCMCPLD_DropUserView( vSubtask );

   SetSelectStateOfEntity( vTZCMREPO, "User", 0 );
   SetCursorPrevEntity( vTZCMREPO, "User", "" );

   SetSelectStateOfEntity( vTZCMREPO, "User", 1 );

   if ( GetParentWindow( &ParentWindow, vSubtask ) == 0 )
      RefreshWindow( ParentWindow );

   zwTZCMCPLD_LoadUser( vSubtask );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildUserWkList( zVIEW  vSubtask )
{
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SetAuthority( zVIEW vSubtask )
{
   zVIEW  vTZCMRUWK;
   zCHAR  szValue[61];
   zLONG  lRow = -1;
   zLONG  lColumn = -1;

   if ( GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vTZCMRUWK, "User" ) >= zCURSOR_SET )
   {

      TG_GetActiveCell( vSubtask, "tgUser", &lRow, &lColumn );

      // if Click on CheckBox
      if ( lColumn == 0 )
      {
         if ( CompareAttributeToString( vTZCMRUWK, "User", "CPLR_User", "Y" )
                                        == 0 )
         {
            SetAttributeFromAttribute( vTZCMRUWK, "User", "Authority",
                                       vTZCMRUWK, "Installation", "Authority" );
            GetStringFromAttributeByContext( szValue, vTZCMRUWK, "User",
                                             "Authority", "AccessAuthority", 60 );
         }
         else
         {
            SetAttributeFromString( vTZCMRUWK, "User", "Authority", "" );
            strcpy_s( szValue, zsizeof( szValue ), "" );
         }

         TG_SetCellData( vSubtask, "tgUser", szValue, lRow, 1 );
      }
      else
      if ( lColumn == 1 ) // edit ComboBox
      {
         if ( CompareAttributeToString( vTZCMRUWK, "User", "Authority", "" ) == 0 )
         {
            SetAttributeFromString( vTZCMRUWK, "User", "CPLR_User", "N" );
            TG_SetCellData( vSubtask, "tgUser", "", lRow, 0 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_AddUserToCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_AddUserToCPLR( zVIEW vSubtask )
{
   zVIEW  vTZCMRUWK;

   if ( GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vTZCMRUWK, "User" ) >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vTZCMRUWK, "User",
                                     "CPLR_User", "Y" ) != 0 &&
           CompareAttributeToString( vTZCMRUWK, "User",
                                     "Authority", "" ) != 0 )
      {
         SetAttributeFromString( vTZCMRUWK, "User", "CPLR_User", "Y" );
         TG_SetCellData( vSubtask, "tgUser", "Y", -1, 0 );
      }
      else
      if ( CompareAttributeToString( vTZCMRUWK, "User",
                                     "CPLR_User", "Y" ) == 0 &&
           CompareAttributeToString( vTZCMRUWK, "User",
                                     "Authority", "" ) == 0 )
      {
         SetAttributeFromString( vTZCMRUWK, "User", "CPLR_User", "N" );
         TG_SetCellData( vSubtask, "tgUser", "", -1, 0 );
      }
   }

   return( 0 );
} // zwTZCMCPLD_AddUserToCPLR

static zSHORT
zwfnTZCMCPLD_CheckCPLR_Name( zVIEW vSubtask, zVIEW vTZCMCPL, zULONG ulZKeyCPRO )
{
   zCHAR  szName[ 33 ];
   zSHORT nRC;
   zULONG ulZKeyREPO;
   zVIEW  vCopy;

   GetCtrlText( vSubtask, "Name", szName, 33 );
   UfCompressName( szName, szName, 32, "", "", "", "", 0 );

   // if new name required.
   if ( zstrcmp( szName, "" ) == 0 )
   {
      MessageSend( vSubtask, "CM00283", "Configuration Management",
                   "CPLR Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   // if new name not unique.
   CreateViewFromViewForTask( &vCopy, vTZCMCPL, 0 );

   nRC = SetCursorFirstEntityByString( vCopy, "CPLR", "Name", szName, 0 );
   GetIntegerFromAttribute( (zPLONG) &ulZKeyREPO, vCopy, "CPLR", "ZKey" );

   if ( ulZKeyCPRO == ulZKeyREPO )
      nRC = SetCursorNextEntityByString( vCopy, "CPLR", "Name", szName, 0 );

   DropView( vCopy );

   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "CM00283", "Configuration Management",
                   "CPLR Name already exists in CPL.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   MapWindow( vSubtask );
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_CommitCPL_CPLR( zVIEW vSubtask, zVIEW vTZCMCPL, zVIEW vTZCMCPLR )
{
   // Now save the CPLR object, using common code with validations.
   // Then save the REPO object.  This must always be saved, since
   // the ZKey incremental attribute may have been updated.

   // first COMMIT the CPL then the CPLR
   if ( CommitObjectInstance( vTZCMCPL ) < 0 )
   {
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );
      return( -1 );
   }

   CommitObjectInstance( vTZCMCPLR );

   oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CPLR_OK( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vTZCMCPLR;
   zVIEW    vReusableCPLR;
   zULONG   ulZKey;
   zLONG    lCPLR_ViewCluster;
   zLONG    lErrorIndex;
   zSHORT   nRC;
   zCHAR    szCPLR_Name[ 33 ];

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      zwTZCMCPLD_CancelCPLR( vSubtask );
      return( 0 );
   }

   GetViewByName( &vTZCMCPL,  "TZCMCPL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPLR, "CPLR", "ZKey" );

   if ( zwfnTZCMCPLD_CheckCPLR_Name( vSubtask, vTZCMCPL, ulZKey ) < 0 )
      return( -1 );

   GetStringFromAttribute( szCPLR_Name, zsizeof( szCPLR_Name ), vTZCMCPLR, "CPLR", "Name" );

   // If the CPLR is not in the REPO object, include it.
   if ( SetCursorFirstEntityByInteger( vTZCMCPL, "CPLR", "ZKey", ulZKey,
                                       "" ) != zCURSOR_SET )
   {
      nRC = IncludeSubobjectFromSubobject( vTZCMCPL, "CPLR",
                                           vTZCMCPLR, "CPLR", zPOS_AFTER );
      RelinkInstanceToInstance( vTZCMCPL, "CPLR",
                                vTZCMCPLR, "CPLR" );
   }
   else //set new CPLR name und desc in Main Window
   {
      SetAttributeFromString( vTZCMCPL, "CPLR", "Name", szCPLR_Name );
      SetAttributeFromAttribute( vTZCMCPL, "CPLR", "Desc",
                                 vTZCMCPLR, "CPLR", "Desc" );
   }

   // Get CPLR View Cluster.
   GetIntegerFromAttribute( &lCPLR_ViewCluster, vTZCMCPLR, "CPLR",
                            "CPLR_ViewCluster" );

   nRC = CommitMultipleObjectInstances( lCPLR_ViewCluster, &lErrorIndex );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "CM00101", "Configuration Management",
                   "Unable to Commit CPLR View Cluster!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( nRC );
   }

   oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );

   // Drop CPLR View Cluster
   DropViewCluster( lCPLR_ViewCluster );
   SetAttributeFromInteger( vTZCMCPLR, "CPLR", "CPLR_ViewCluster", 0 );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   // Drop Component Views
   zwTZCMCPLD_DropComponentViews( vSubtask );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CancelUserToCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CancelUserToCPLR( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vTZCMRUWK;
   zVIEW  vTZCMCPLR;

   if ( GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // Was the window <MaintainUsers> opened via the menu option
   // <Assign User to CPLR> or the window <CPLR_Maintenance>?
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) < 0 )
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );

   if ( GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRUWK );

   return( 0 );
} // zwTZCMCPLD_CancelUserToCPLR


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_AddCPLR_Users( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vTZCMCPLR;
   zVIEW    vTZCMCPL;
   zVIEW    vTZCMRUWK;
   zVIEW    vWindow;
   zULONG   ulZKey;
   zSHORT   nRC;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      zwTZCMCPLD_CancelUserToCPLR( vSubtask );
      return( 0 );
   }

   // The current Users that are to be set under this CPLR are in
   // the work object, TZCMCUWK.  First loop through REPO, deleting
   // all the Users that are no longer in TZCMCUWK.  Then loop
   // through REPO, adding or updating the rest of the entries.

   GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // First delete the entries.
   nRC = SetCursorFirstEntity( vTZCMCPLR, "AccessAuthority", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPLR, "User", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vTZCMRUWK, "User", "ZKey",
                                           ulZKey, "" );
      if ( nRC != zCURSOR_SET ||
           CompareAttributeToString( vTZCMRUWK, "User", "CPLR_User", "N" ) == 0 ||
           CompareAttributeToString( vTZCMRUWK, "User", "Authority", "" ) == 0 )
      {
         DeleteEntity( vTZCMCPLR, "AccessAuthority", zREPOS_NONE );
      }

      nRC = SetCursorNextEntity( vTZCMCPLR, "AccessAuthority", "" );
   }

   // Now add or update the rest of the entries.
   nRC = SetCursorFirstEntity( vTZCMRUWK, "User", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMRUWK, "User", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vTZCMCPLR, "User", "ZKey",
                                           ulZKey, "CPLR" );

      if ( CompareAttributeToString( vTZCMRUWK, "User", "CPLR_User", "Y" ) == 0 &&
           CompareAttributeToString( vTZCMRUWK, "User", "Authority", "" ) != 0 )
      {
         if ( nRC >= zCURSOR_SET )
         {
            SetAttributeFromAttribute( vTZCMCPLR, "AccessAuthority", "Authority",
                                       vTZCMRUWK, "User", "Authority" );
         }
         else
         {
            SetCursorFirstEntityByInteger( vTZCMREPO, "User", "ZKey", ulZKey, "" );
            CreateInstallEntity( vSubtask, vTZCMCPLR, "AccessAuthority", zPOS_AFTER );
            SetAttributeFromAttribute( vTZCMCPLR, "AccessAuthority", "Authority",
                                       vTZCMRUWK, "User", "Authority" );
            IncludeSubobjectFromSubobject( vTZCMCPLR, "User",
                                           vTZCMREPO, "User", zPOS_AFTER );
         }
      }
      nRC = SetCursorNextEntity( vTZCMRUWK, "User", "" );
   }

   zgSortEntityWithinGParent4( vTZCMCPLR, "CPLR", "User", "Name", zASCENDING,
                               "", 0, "", 0, "", 0 );

   nRC = 0;
   DropObjectInstance( vTZCMRUWK );

   //BL, 1999.09.21 Was the window <MaintainUsers> opened via the menu option
   //<Assign User to CPLR> or the window <CPLR_Maintenance>?
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) < 0 )
      nRC = zwfnTZCMCPLD_CommitCPL_CPLR( vSubtask, vTZCMCPL, vTZCMCPLR );

   return( nRC );
}


static zSHORT
zwfnTZCMCPLD_LoadCPLR( zVIEW vSubtask, zPVIEW vTZCMCPRO, zPCHAR szLodName,
                       zULONG ulZKey, zPCHAR szName, zLONG lControl )
{
   zVIEW    vKZDBHQUA;
   zCHAR    szZKey[ 20 ];
   zCHAR    szERR_Msg[ 254 ];
   zSHORT   nRC;

   // Set up qualification object and activate CPLR
   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "CPLR" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );

   nRC = ActivateObjectInstance( vTZCMCPRO, szLodName,
                                 vSubtask, vKZDBHQUA, lControl );
   DropObjectInstance( vKZDBHQUA );

   if ( nRC < 0 )
   {
      strcpy_s( szERR_Msg, zsizeof( szERR_Msg ), "Unable to Activate CPLR '" );
      strcat_s( szERR_Msg, zsizeof( szERR_Msg ), szName );
      strcat_s( szERR_Msg, zsizeof( szERR_Msg ), "' !" );
      MessageSend( vSubtask, "CM00118", "Configuration Management",
                   szERR_Msg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
}


static zSHORT
zwfnTZCMCPLD_LoadCPLR_ForViewName( zVIEW  vSubtask, zPCHAR szViewName )
{
   zVIEW    vTZCMWIPO;
   zVIEW    vTZCMCPL;
   zVIEW    vTZCMCPLR;
   zCHAR    szName[ 33 ];
   zULONG   ulZKey;
   zULONG   ulZKey2;

   GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPL,  "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( (zPLONG)&ulZKey, vTZCMCPL, "CPLR", "ZKey" );
   }
   else
   if ( GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetNameForView( vTZCMCPLR, szViewName, vSubtask, zLEVEL_TASK );
      return( 1 ); // if a new CPLR
   }
   else
   {
      return( -1 );
   }

   // The following seems to save the I/O if the CPLR requested is the
   // last one processed.
   if ( GetViewByName( &vTZCMCPLR, szViewName, vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey2, vTZCMCPLR, "CPLR", "ZKey" );

      if ( ulZKey2 == ulZKey )
         return( 0 );
      else
         DropObjectInstance( vTZCMCPLR );
   }

   // Set up qualification object and activate CPLR
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );

   if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vTZCMCPLR, szViewName, ulZKey,
                               szName, zSINGLE ) < 0 )
   {
      return( -1 );
   }
   SetNameForView( vTZCMCPLR, szViewName, vSubtask, zLEVEL_TASK );

   // Identify the current user in the WIPO object as an administrator
   // depending on how he is set in the CPLR.
   SetAttributeFromInteger( vTZCMWIPO, "ROOT", "CPLR_Administrator", 0 );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMWIPO, "ROOT", "UserName" );

   if ( SetCursorFirstEntityByString( vTZCMCPLR, "User", "Name",
                                      szName, "CPLR" ) >= zCURSOR_SET &&
        CompareAttributeToInteger( vTZCMCPLR, "AccessAuthority",
                                   "Authority", 2 ) == 0 )
   {
      SetAttributeFromInteger( vTZCMWIPO, "ROOT", "CPLR_Administrator", 1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadCPRO_WithEnq( zVIEW vSubtask )
{
   zVIEW  vTZCMCPRO;
   zSHORT nRC;

   nRC = zwfnTZCMCPLD_LoadCPLR_ForViewName( vSubtask, "TZCMCPRO" );

   if ( nRC == 0 )
   {
      GetViewByName( &vTZCMCPRO, "TZCMCPRO", vSubtask, zLEVEL_TASK );

      // Test whether the CPLR ist not enqueued and enqueue it.
      if ( oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO, vSubtask, zCPLR_ENQUEUE_BY_SYSADM ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadCPLR_WithEnq
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadCPLR_WithEnq( zVIEW vSubtask )
{
   zVIEW  vTZCMCPLR;
   zSHORT nRC;

   nRC = zwfnTZCMCPLD_LoadCPLR_ForViewName( vSubtask, "TZCMCPLR" );

   if ( nRC == 0 )
   {
      GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );

      // Test whether the CPLR ist not enqueued and enqueue it.
      if ( oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_ENQUEUE_BY_SYSADM ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   return( nRC );
} // zwTZCMCPLD_LoadCPLR_WithEnq

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadUser( zVIEW    vSubtask )
{
   zVIEW    vTZCMUSER;
   zVIEW    vTZCMREPO;
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szZKey[ 20 ];
   zCHAR    szERR_Msg[ 254 ];
   zCHAR    szName[ 33 ];
   zSHORT   nRC;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG)&ulZKey, vTZCMREPO, "User", "ZKey" );

   if ( GetViewByName( &vTZCMUSER, "TZCMUSER", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMUSER );

   // Set up qualification object and activate User
   ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA", vSubtask, zSINGLE );
   CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "User" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "User" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );

   nRC = ActivateObjectInstance( &vTZCMUSER, "TZCMUSER",
                                 vSubtask, vKZDBHQUA, zSINGLE );
   DropObjectInstance( vKZDBHQUA );

   if ( nRC < 0 )
   {
      // No Error Message for new Users
      if ( GetIncrementalUpdateFlags( vTZCMREPO, "User", zSET_INCR_CREATED ) != 1 )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMREPO, "User", "Name" );
         strcpy_s( szERR_Msg, zsizeof( szERR_Msg ), "Unable to Activate User '" );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), szName );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), "'." );
         MessageSend( vSubtask, "CM00119", "Configuration Management",
                      szERR_Msg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
   }

   SetNameForView( vTZCMUSER, "TZCMUSER", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_AddCPL( zVIEW vSubtask )
{
   zVIEW  vTZCMCPL;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // Use CreateInstallEntity to create the new CPL instance.
   CreateInstallEntity( vSubtask, vTZCMCPL, "CPL", zPOS_AFTER );

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_CheckCPL_Name( zVIEW vSubtask, zVIEW vTZCMCPL )
{
   zCHAR  szName[ 33 ];
   zSHORT nRC;
   zVIEW  vCopy;

   GetCtrlText( vSubtask, "Name", szName, 33 );
   UfCompressName( szName, szName, 32, "", "", "", "", 0 );

   // if new name required.
   if ( zstrcmp( szName, "" ) == 0 )
   {
      MessageSend( vSubtask, "CM00283", "Configuration Management",
                   "CPL Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   // if new name not unique.
   CreateViewFromViewForTask( &vCopy, vTZCMCPL, 0 );

   SetCursorFirstEntityByString( vCopy, "CPL", "Name", szName, 0 );
   nRC = SetCursorNextEntityByString( vCopy, "CPL", "Name", szName, 0 );
   DropView( vCopy );

   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "CM00283", "Configuration Management",
                   "CPL Name already exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   MapWindow( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_AcceptCPL( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vWindow;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( zwfnTZCMCPLD_CheckCPL_Name( vSubtask, vTZCMCPL ) < 0 )
      return( -1 );

   SetAttributeFromString( vTZCMCPL, "CPL", "NewOrUpdate", "" );

   // If there is no CPLR under the CPL, create the CPLR object and
   // transfer to CPLR_Maintenance.
   if ( CheckExistenceOfEntity ( vTZCMCPL, "CPLR" ) == zCURSOR_NULL )
   {
      // No CPLR window by Save CPLR As
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" ) >= 0 )
      {
         RefreshCtrl( vWindow, "lbCPLList" );
      }
      else
      {
         zwTZCMCPLD_TransferToAddCPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZCMCPLD", "CPLR_Maintenance" );
      }
   }
   else
   {
      // Refresh Window Save CPLR As
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" ) >= 0 )
      {
         RefreshCtrl( vWindow, "lbCPLList" );
      }
      else
      {
         GetSubtaskForWindowName( vSubtask, &vWindow, "CPL_List" );
         RefreshWindow( vWindow );

         if ( ObjectInstanceUpdatedFromFile( vTZCMCPL ) )
         {
            if ( CommitObjectInstance( vTZCMCPL ) < 0 )
            {
               MessageSend( vSubtask, "CM00101", "Configuration Management",
                            "Unable to save CPL List!",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               return( -1 );
            }
         }
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildAdminList( zVIEW  vSubtask )
{
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_LoadMeta( zVIEW vSubtask, zVIEW vTZCMCPLR )
{
   if ( !vTZCMCPLR )
      return( -1 );

   if ( CheckExistenceOfEntity( vTZCMCPLR, "UserMetaStatus" ) == zCURSOR_SET )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZCMCPLD", "Status" );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZCMCPLD", "MetaDetail" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_MetaInfoInit( zVIEW  vSubtask )
{
   zVIEW  vMetaList;
   zVIEW  vCPLR_AllMetas;
   zULONG ulZKey;

   if ( GetViewByName( &vMetaList, "CPLR_MetaList", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vMetaList, "Z_MetaDef", "ZKey" );
   SetCursorFirstEntityByInteger( vCPLR_AllMetas, "Z_MetaDef", "ZKey",
                                 (zLONG) ulZKey, "" );

   zwfnTZCMCPLD_LoadMeta( vSubtask, vCPLR_AllMetas );

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_CreateMetaViews( zVIEW vSubtask, zPVIEW vAllMetas, zPVIEW vMetaList,
                              zBOOL bAllMetas )
{
   zVIEW   vTZCMCPL;
   zVIEW   vWindow;
   zCHAR   szName[ 33 ];
   zULONG  ulZKey;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) < zCURSOR_SET )
   {
      MB_SetMessage( vSubtask, 1, "List requested is empty." );
      return( -1 );
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );

   if ( GetViewByName( vAllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // Set up qualification object and activate all CPLR Metas
      if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, vAllMetas, "TZCMCOML",
                                  ulZKey, szName, zSINGLE ) < 0 )
      {
         return( -1 );
      }
      SetNameForView( *vAllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK );

      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) >= 0 )
         CreateTemporalSubobjectVersion( *vAllMetas, "CPLR" );
   }

   if ( bAllMetas )
      return( 0 );

   if ( GetViewByName( vMetaList, "CPLR_MetaList", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( *vMetaList );

   if ( ActivateEmptyObjectInstance( vMetaList, "TZCMCOML", vSubtask,
                                     zSINGLE ) >= 0 )
   {
      CreateEntity( *vMetaList, "CPLR", zPOS_AFTER );
      SetMatchingAttributesByName( *vMetaList, "CPLR",
                                   vTZCMCPL, "CPLR", zSET_ALL );
      SetNameForView( *vMetaList,"CPLR_MetaList", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_BuildFileView( zVIEW vSubtask, zPVIEW vTZCMFILE )
{
   zVIEW   vTZCMCPL;
   zVIEW   vWindow;
   zULONG  ulZKey;
   zCHAR   szName[ 33 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );
   }
   else
   {
      MB_SetMessage( vSubtask, 1, "List requested is empty." );
      return( -1 );
   }

   // Set up qualification object and activate all CPLR Files
   if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, vTZCMFILE, "TZCMFILE",
                               ulZKey, szName, zSINGLE ) < 0 )
   {
      return( -1 );
   }
   SetNameForView( *vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) >= 0 )
      CreateTemporalSubobjectVersion( *vTZCMFILE, "CPLR" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_BuildFileList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_BuildFileList( zVIEW vSubtask )
{
   zVIEW   vTZCMFILE;

   MB_SetMessage( vSubtask, 1, "Loading CPLR Files." );
   MB_SetMessage( vSubtask, 2, "" );

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) < 0 )
   {
      if ( zwfnTZCMCPLD_BuildFileView( vSubtask, &vTZCMFILE ) < 0 )
         return( -1 );
   }

   RefreshCtrl( vSubtask, "lbFileList" );

   if ( CheckExistenceOfEntity( vTZCMFILE, "File" ) < zCURSOR_SET )
      MB_SetMessage( vSubtask, 1, "List requested is empty." );
   else
      MB_SetMessage( vSubtask, 1, "" );

   return( 0 );
} // zwTZCMCPLD_BuildFileList

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildMetaList( zVIEW  vSubtask )
{
   zVIEW    vTZCMWIPO;
   zVIEW    vTZCMCUSR;
   zVIEW    vMetaList;
   zVIEW    vAllMetas;
   zLONG    lCount;
   zSHORT   nStatus;

   GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK );

   if ( zwfnTZCMCPLD_CreateMetaViews( vSubtask, &vAllMetas, &vMetaList, FALSE ) < 0 )
      return( -1 );

   lCount = 0;
   GetIntegerFromAttribute( (zPLONG) &nStatus, vTZCMWIPO, "ROOT", "ComponentStatus" );

   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListDomain", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_DOMAINGRP_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListLOD", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_LOD_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListERD", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_ERD_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListDialogs", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_DIALOG_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListGlobalOper", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_GOPGRP_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListSA", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_SA_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListUIS", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_UIS_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListPE", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_PENV_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListVOR", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_VOR_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListTE", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_DTE_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListReports", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_XSLT_META );
   if ( CompareAttributeToInteger( vTZCMWIPO, "ROOT", "ListXSLT", 1 ) == 0 )
      lCount = zwfnTZCMCPLD_GetListMeta( vSubtask, vTZCMCUSR, vAllMetas, vMetaList,
                                nStatus, lCount, (zLONG) zSOURCE_REPORT_META );
   ResetView( vMetaList );
   RefreshCtrl( vSubtask, "lbMetaList" );

   if ( CheckExistenceOfEntity( vMetaList, "Z_MetaDef" ) < zCURSOR_SET )
      MB_SetMessage( vSubtask, 1, "List requested is empty." );
   else
      MB_SetMessage( vSubtask, 1, "" );

   MB_SetMessage( vSubtask, 2, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_ListAll( zVIEW  vSubtask )
{
   zVIEW    vMetaList;
   zVIEW    vAllMetas;

   if ( zwfnTZCMCPLD_CreateMetaViews( vSubtask, &vAllMetas, &vMetaList, TRUE ) < 0 )
      return( -1 );

   RefreshCtrl( vSubtask, "lbAllMetaList" );

   if ( CheckExistenceOfEntity( vAllMetas, "Z_MetaDef" ) < zCURSOR_SET )
      MB_SetMessage( vSubtask, 1, "List requested is empty." );
   else
      MB_SetMessage( vSubtask, 1, "" );

   MB_SetMessage( vSubtask, 2, "" );

   return( 0 );
}

static zLONG
zwfnTZCMCPLD_GetListMeta( zVIEW vSubtask, zVIEW vTZCMCUSR, zVIEW vAllMetas,
                          zVIEW vMetaList, zSHORT nStatus, zLONG lCount, zLONG Type )
{
   zSHORT nRC;
   zSHORT Copy;
   zCHAR szName[ 56 ];
   zCHAR szCO_Name[ 56 ];
   zCHAR szMsg[ 200 ];
   zCHAR szMetaName[ 33 ];
   zLONG lCountMetas;

   lCountMetas = CountEntitiesForView( vAllMetas, "Z_MetaDef" );

   if ( nStatus == 3 )
   {
      if ( CheckExistenceOfEntity( vTZCMCUSR, "User" ) == zCURSOR_SET )
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCUSR, "User", "Name" );
      else
         return( lCount );
   }

   nRC = SetCursorFirstEntityByInteger( vAllMetas, "Z_MetaDef", "Type", Type, "" );
   while ( nRC == zCURSOR_SET )
   {
      lCount++;
      zltoa( lCount, szMsg + 110, zsizeof( szMsg ) - 110 );
      strcpy_s( szMsg, zsizeof( szMsg ), szMsg + 110 );
      strcat_s( szMsg, zsizeof( szMsg ), " of " );
      zltoa( lCountMetas, szMsg + 110, zsizeof( szMsg ) - 110 );
      strcat_s( szMsg, zsizeof( szMsg ), szMsg + 110 );
      MB_SetMessage( vSubtask, 2, szMsg );

      Copy = 0;
      if ( nStatus == 3 ) // Checked Out By User
      {
         if ( CheckExistenceOfEntity( vAllMetas, "UserMetaStatus" ) == zCURSOR_SET )
         {
            GetStringFromAttribute( szCO_Name, zsizeof( szCO_Name ), vAllMetas, "User", "Name" );
            if ( zstrcmp( szName, szCO_Name ) == 0 )
               Copy = 1;
         }
      }
      else
      if ( nStatus == 1 ) // All Checked Out
      {
         if ( CheckExistenceOfEntity( vAllMetas, "UserMetaStatus" ) == zCURSOR_SET )
            Copy = 1;
      }
      else
      if ( nStatus == 2 )
      {
         if ( CheckExistenceOfEntity( vAllMetas, "UserMetaStatus" ) != zCURSOR_SET )
         {
            Copy = 1;
         }
      }
      else
         Copy = 1;

      if ( Copy == 1 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Loading " );
         GetStringFromAttributeByContext( szMsg + zstrlen( szMsg ), vAllMetas,
                                          "Z_MetaDef", "Type", "CM_Type", 50 );
         strcat_s( szMsg, zsizeof( szMsg ), ": " );
         GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vAllMetas, "Z_MetaDef", "Name" );
         strcat_s( szMsg, zsizeof( szMsg ), szMetaName );
         MB_SetMessage( vSubtask, 0, szMsg );

         CreateEntity( vMetaList, "Z_MetaDef", zPOS_AFTER );
         SetMatchingAttributesByName( vMetaList, "Z_MetaDef",
                                      vAllMetas, "Z_MetaDef", zSET_ALL );
      }

      nRC = SetCursorNextEntityByInteger( vAllMetas, "Z_MetaDef", "Type", Type, "" );
   }

   return( lCount );
}

static zSHORT
zwTZCMCPLD_BuildReusCPLR_Views( zVIEW vSubtask, zPVIEW vTZCMRCRO,
                                zPVIEW vTZCMRCRO_All, zULONG ulCPRO_ZKey )
{
   if ( ActivateObjectInstance( vTZCMRCRO, "TZCMRCRO", vSubtask,
                                      0, zSINGLE ) == zCALL_ERROR )
   {
      MessageSend( vSubtask, "CM00129", "Configuration Management",
                   "Activation failed on load of Repository",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   if ( GetViewByName( vTZCMRCRO_All, "TZCMRCRO_All", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( *vTZCMRCRO_All );

   if ( ActivateOI_FromOI_ForTask( vTZCMRCRO_All, *vTZCMRCRO, 0, zSINGLE ) == zCALL_ERROR )
   {
      MessageSend( vSubtask, "CM00130", "Configuration Management",
                   "Activation failed on load of Repository",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SetNameForView( *vTZCMRCRO_All, "TZCMRCRO_All", vSubtask, zLEVEL_TASK );
   SetNameForView( *vTZCMRCRO, "TZCMRCRO_Select", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstEntityByInteger( *vTZCMRCRO, "CPLR", "ZKey", (zLONG) ulCPRO_ZKey,
                                       "Installation" ) >= zCURSOR_SET )
   {
      DeleteEntity( *vTZCMRCRO, "CPL", zREPOS_NONE );
   }

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_CreateCurrentReusableCPLR( zVIEW vReusableCPLR, zVIEW vTZCMRCRO )
{
   zSHORT   nRC;

   if ( SetViewToSubobject( vTZCMRCRO, "ReusableCPLR" ) == zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "" );
      if ( nRC != zCURSOR_SET )
      {
         ResetViewFromSubobject( vTZCMRCRO );
         return( 0 );
      }

      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vReusableCPLR, "CPLR", zPOS_AFTER );
         SetMatchingAttributesByName( vReusableCPLR, "CPLR",
                                      vTZCMRCRO, "CPLR", zSET_ALL );

         if ( CheckExistenceOfEntity( vTZCMRCRO, "ReusableCPLR" ) >= zCURSOR_SET )
         {
            SetViewToSubobject( vReusableCPLR, "ReusableCPLR" );
         }

         zwfnTZCMCPLD_CreateCurrentReusableCPLR( vReusableCPLR, vTZCMRCRO );
         nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "" );
      }
   }

   ResetViewFromSubobject( vTZCMRCRO );
   ResetViewFromSubobject( vReusableCPLR );

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_ReusableCPLR_CreateQual( zVIEW vSubtask, zULONG ulZKey, zPVIEW vKZDBHQUA )
{
   zCHAR    szZKey[ 20 ];

   if ( ActivateEmptyObjectInstance ( vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zMULTIPLE ) >= 0 )
   {
      SetNameForView( *vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( *vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( *vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   }
   else
   {
      return( -1 );
   }

   SetAttributeFromString( *vKZDBHQUA, "EntitySpec", "EntityName", "Installation" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Value", szZKey );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Oper", "=" );

   CreateEntity( *vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vKZDBHQUA, "EntitySpec", "EntityName", "CPL" );
   CreateEntity( *vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Value", szZKey );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Oper", "=" );

   CreateEntity( *vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vKZDBHQUA, "EntitySpec", "EntityName", "CPLR" );
   CreateEntity( *vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Value", szZKey );
   SetAttributeFromString( *vKZDBHQUA, "QualAttrib", "Oper", "=" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_PreBuildReusableCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_PreBuildReusableCPLR( zVIEW vSubtask )
{
   zVIEW    vKZDBHQUA;
   zVIEW    vTZCMRCRO;
   zVIEW    vReusableCPLR;
   zVIEW    vTZCMCPLR;
   zVIEW    vWindow;
   zULONG   ulZKey;
   zSHORT   nRC;

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 &&
        GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) >= 0 )
   {
      return( 0 );
   }

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPLR, "CPLR", "ZKey" );

   if ( zwfnTZCMCPLD_ReusableCPLR_CreateQual( vSubtask, ulZKey, &vKZDBHQUA ) < 0 )
      return( -1 );

   nRC = ActivateObjectInstance( &vTZCMRCRO, "TZCMRCRO",
                                 vSubtask, vKZDBHQUA, zSINGLE );

   DropObjectInstance( vKZDBHQUA );
   SetNameForView( vTZCMRCRO, "TZCMRCRO", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vTZCMCPLR, "CPLR" );

   ActivateEmptyObjectInstance( &vReusableCPLR, "TZCMRCRO", vSubtask,
                                zLEVEL_TASK | zSINGLE );
   CreateEntity( vReusableCPLR, "Installation", zPOS_AFTER );
   SetNameForView( vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK );

   if ( nRC < 0 )
      return( -1 );

   nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateEntity( vReusableCPLR, "CPL", zPOS_AFTER );
      SetMatchingAttributesByName( vReusableCPLR, "CPL",
                                   vTZCMRCRO, "CPL", zSET_ALL );
      zwfnTZCMCPLD_CreateCurrentReusableCPLR( vReusableCPLR, vTZCMRCRO );
      nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "" );
   }

   DropObjectInstance( vTZCMRCRO );

   while ( ResetViewFromSubobject( vReusableCPLR ) == 0 );
   SetCursorFirstEntity( vReusableCPLR, "CPLR", "Installation" );

   return( 0 );
} // zwTZCMCPLD_PreBuildReusableCPLR

/////////////////////////////////////////////////////////////////////////////
//
// Trolls through a ReusableCPLR chain and, for each CPLR found in the chain,
// removes its parent CPL from consideration. It also build a work list of
// CPLs and single CPLRs for a flat view of the current CPLR/ReusableCPLR
// chain.  That view is used by zwfnTZCMCPLD_TrollReusableCPLR.
//
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZCMCPLD_TrollReusableCPLRs( zVIEW vSubtask, zVIEW vCurrentReusables,
                                 zVIEW vReusableCPLR,
                                 zVIEW vTempREPO, zVIEW vTZCMRCRO )
{
   zSHORT   nRC;
   zULONG   ulZKey;

   do
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vReusableCPLR, "CPLR", "ZKey" );

      if ( SetCursorFirstEntityByInteger( vCurrentReusables, "CPLR", "ZKey",
                                          (zLONG) ulZKey, "Installation" )
                                          < zCURSOR_SET )
      {
         // Create the compare REPO list
         nRC = SetCursorFirstEntityByInteger( vTempREPO, "CPLR", "ZKey",
                                              (zLONG) ulZKey, "Installation" );

         CreateEntity( vCurrentReusables, "CPL", zPOS_AFTER );
         SetMatchingAttributesByName( vCurrentReusables, "CPL",
                                      vTempREPO, "CPL", zSET_ALL );

         CreateEntity( vCurrentReusables, "CPLR", zPOS_AFTER );
         SetMatchingAttributesByName( vCurrentReusables, "CPLR",
                                      vTempREPO, "CPLR", zSET_ALL );
      }

      if ( vTZCMRCRO &&
           SetCursorFirstEntityByInteger( vTZCMRCRO, "CPLR", "ZKey",
                            (zLONG) ulZKey, "Installation" ) >= zCURSOR_SET )
      {
         DeleteEntity( vTZCMRCRO, "CPL", zREPOS_NONE );
      }

      if ( CheckExistenceOfEntity( vReusableCPLR, "ReusableCPLR" ) >= zCURSOR_SET )
      {
         SetViewToSubobject( vReusableCPLR, "ReusableCPLR" );
         SetCursorFirstEntity( vReusableCPLR, "CPLR", "" );
         zwfnTZCMCPLD_TrollReusableCPLRs( vSubtask, vCurrentReusables,
                                          vReusableCPLR,
                                          vTempREPO, vTZCMRCRO );
      }

      nRC = SetCursorNextEntity( vReusableCPLR, "CPLR", "" );
      if ( nRC < zCURSOR_SET )
      {
         ResetViewFromSubobject( vReusableCPLR );
         nRC = SetCursorNextEntity( vReusableCPLR, "CPLR", "" );
      }

   } while ( nRC == zCURSOR_SET );

   return;
}

static zSHORT
zwTZCMCPLD_CreateViewForCurrentReusables( zVIEW vSubtask, zPVIEW vCurrentReusables,
                                          zVIEW vTZCMRCRO, zVIEW vReusableCPLR,
                                          zULONG ulZKey )
{
   zVIEW  vWork;
   zVIEW  vTempREPO;
   zSHORT nRC;

   /* The following Temp work object will list all CPL/CPLR relationships in the
      current CPLR\ReusableCPLR chain in a multiple CPL view */
   if ( GetViewByName( vCurrentReusables, "CurrentReusables", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( vCurrentReusables, "TZCMRCRO", vSubtask,
                                   zLEVEL_TASK | zSINGLE );
      SetNameForView( *vCurrentReusables, "CurrentReusables", vSubtask, zLEVEL_TASK );
      CreateEntity( *vCurrentReusables, "Installation", zPOS_AFTER );
   }

   // Get the CPL View to obtain the CPL
   GetViewByName( &vWork, "TZCMCPL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTempREPO, vWork, 0 );
   SetNameForView( vTempREPO, "TempREPO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTempREPO, "CPLR" ) >= zCURSOR_SET )
   {
      SetCursorFirstEntityByInteger( vTempREPO, "CPLR", "ZKey",
                                     (zLONG) ulZKey, "Installation" );
   }

   CreateEntity( *vCurrentReusables, "CPL", zPOS_AFTER );
   SetMatchingAttributesByName( *vCurrentReusables, "CPL",
                                vTempREPO, "CPL", zSET_ALL );

   if ( CheckExistenceOfEntity( vTempREPO, "CPLR" ) >= zCURSOR_SET )
   {
      CreateEntity( *vCurrentReusables, "CPLR", zPOS_AFTER );
      SetMatchingAttributesByName( *vCurrentReusables, "CPLR",
                                   vTempREPO, "CPLR", zSET_ALL );
   }

   //////////////////////////////////////////////////////////////////////////
   // Troll through the current CPLR's ReusableCPLR chain. For each CPLR in
   // the chain delete it's parent CPL from consideration.
   //////////////////////////////////////////////////////////////////////////

   while ( ResetViewFromSubobject( vReusableCPLR ) == 0 );

   nRC = SetCursorFirstEntity( vReusableCPLR, "CPLR", "Installation" );
   while ( nRC >= zCURSOR_SET )
   {
      zwfnTZCMCPLD_TrollReusableCPLRs( vSubtask, *vCurrentReusables,
                                       vReusableCPLR,
                                       vTempREPO, vTZCMRCRO );
      while ( ResetViewFromSubobject( vReusableCPLR ) == 0 );
      nRC = SetCursorNextEntity( vReusableCPLR, "CPLR", "Installation" );
   }

   DropView( vTempREPO );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildReusCPLR_List( zVIEW  vSubtask )
{
   zVIEW    vTempREPO;
   zVIEW    vWork;
   zVIEW    vReusableCPLR;
   zVIEW    vTZCMRCRO     = NULL;
   zVIEW    vTZCMRCRO_All = NULL;
   zVIEW    vTZCMCPLR     = NULL;
   zVIEW    vCurrentReusables = NULL;
   zULONG   ulZKey;
   zSHORT   nRC;

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPLR, "CPLR", "ZKey" );

   nRC = zwTZCMCPLD_BuildReusCPLR_Views( vSubtask,&vTZCMRCRO,
                                         &vTZCMRCRO_All, ulZKey );
   if ( nRC < 0 )
      return( -1 );

   //////////////////////////////////////////////////////////////////////////
   // Troll through the current CPLR's ReusableCPLR chain. For each CPLR in
   // the chain delete it's parent CPL from consideration.
   //////////////////////////////////////////////////////////////////////////
   GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK );
   zwTZCMCPLD_CreateViewForCurrentReusables( vSubtask, &vCurrentReusables,
                                             vTZCMRCRO, vReusableCPLR, ulZKey );

   // Get the REPO View to obtain the CPL
   GetViewByName( &vWork, "TZCMCPL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTempREPO, vWork, 0 );
   SetNameForView( vTempREPO, "TempREPO", vSubtask, zLEVEL_TASK );

   //////////////////////////////////////////////////////////////////////////
   // Now troll through the remaining CPL/CPLR list and verify that the
   // CPLR/ReusableCPLR chains do not contain sister CPLRs( within the same
   // CPL but not identical ) of the current CPLR/ReusableCPLR Chain. For
   // example, if the current CPLR has a ReusableCPLR( 2A ) of CPL 2, then
   // a CPLR with a ReusableCPLR( 2B ) of CPL 2 may not be a candidate,
   // however, a CPLR with ReusableCPLR 2A of CPL 2 may be a candidate.
   //////////////////////////////////////////////////////////////////////////
   nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "Installation" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTZCMRCRO, "ReusableCPLR", "" );
      while ( nRC == zCURSOR_SET )
      {
         if ( zwfnTZCMCPLD_TrollReusableCPLR( vSubtask, vTZCMRCRO ) == -1 )
         {
            while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );
            nRC = DeleteEntity( vTZCMRCRO, "CPLR", zREPOS_NEXT );
            break;
         }
         else
            nRC = SetCursorNextEntity( vTZCMRCRO, "ReusableCPLR", "" );
      }
      nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "Installation" );
   }

   //////////////////////////////////////////////////////////////////////////
   // Now troll through the remaining CPL/CPLR list and verify that the
   // CPLR/ReusableCPLR chains do not contain the current CPLRs.
   //////////////////////////////////////////////////////////////////////////
   nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "Installation" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTZCMRCRO, "ReusableCPLR", "" );
      while ( nRC == zCURSOR_SET )
      {
         if ( zwfnTZCMCPLD_TrollReusableCPLR2( vSubtask, vTZCMRCRO,
                                               (zLONG) ulZKey ) == -1 )
         {
            while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );
            nRC = DeleteEntity( vTZCMRCRO, "CPLR", zREPOS_NEXT );
            break;
         }
         else
            nRC = SetCursorNextEntity( vTZCMRCRO, "ReusableCPLR", "" );
      }
      nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "Installation" );
   }

   DropView( vTempREPO );

   while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );
   SetCursorFirstEntity( vTZCMRCRO, "CPLR", "Installation" );

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_TrollReusableCPLR( zVIEW vSubtask, zVIEW  vTZCMRCRO )
{
   zVIEW    vCurrentReusables;
   zVIEW    vTempREPO;
   zSHORT   nRC;
   zULONG   ulZKey;
   zULONG   ulCPL_ZKey;

   if ( SetViewToSubobject( vTZCMRCRO, "ReusableCPLR" ) == zCURSOR_SET )
   {
      GetViewByName( &vCurrentReusables, "CurrentReusables", vSubtask, zLEVEL_TASK );
      GetViewByName( &vTempREPO, "TempREPO", vSubtask, zLEVEL_TASK );

      nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "" );
      if ( nRC != zCURSOR_SET )
      {
         ResetViewFromSubobject( vTZCMRCRO );
         return( 0 );
      }

      while ( nRC == zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMRCRO, "CPLR", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vTempREPO, "CPLR", "ZKey",
                                              (zLONG) ulZKey, "Installation" );
         GetIntegerFromAttribute( (zPLONG) &ulCPL_ZKey, vTempREPO, "CPL", "ZKey" );

         if ( SetCursorFirstEntityByInteger( vCurrentReusables, "CPL", "ZKey",
                                    (zLONG) ulCPL_ZKey, "" ) == zCURSOR_SET )
         {
            if ( CompareAttributeToInteger( vCurrentReusables, "CPLR", "ZKey",
                                            (zLONG) ulZKey ) != 0 )
            {
               return( -1 );
            }
         }

         if ( SetCursorFirstEntityByInteger( vCurrentReusables, "CPLR", "ZKey",
                                 (zLONG) ulZKey, "Installation" ) == zCURSOR_SET )
         {
               return( -1 );
         }


         if ( zwfnTZCMCPLD_TrollReusableCPLR( vSubtask, vTZCMRCRO ) == -1 )
            return( -1 );

         nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "" );
      }
   }

   ResetViewFromSubobject( vTZCMRCRO );
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_TrollReusableCPLR2( zVIEW vSubtask, zVIEW vTZCMRCRO, zLONG ulZKey )
{
   zSHORT   nRC;
   zULONG   ulReusZKey;

   if ( SetViewToSubobject( vTZCMRCRO, "ReusableCPLR" ) == zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "" );
      if ( nRC != zCURSOR_SET )
      {
         ResetViewFromSubobject( vTZCMRCRO );
         return( 0 );
      }

      while ( nRC == zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zPLONG) &ulReusZKey, vTZCMRCRO, "CPLR", "ZKey" );

         if ( (zLONG) ulReusZKey == ulZKey )
            return( -1 );

         if ( zwfnTZCMCPLD_TrollReusableCPLR2( vSubtask, vTZCMRCRO, ulZKey ) == -1 )
            return( -1 );

         nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "" );
      }
   }

   ResetViewFromSubobject( vTZCMRCRO );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_RemoveReusableCPLR( zVIEW  vSubtask )
{
   zVIEW    vTZCMCPLR;
   zVIEW    vReusableCPLR;
   zCHAR    szMsg[ 254 ];
   zCHAR    szCPLR[ 33 ];

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR",
                       vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   while ( ResetViewFromSubobject( vReusableCPLR ) == 0 );

   strcpy_s( szMsg, zsizeof( szMsg ), "OK to remove CPLR '" );
   GetStringFromAttribute( szCPLR, zsizeof( szCPLR ), vReusableCPLR, "CPLR", "Name" );
   strcat_s( szMsg, zsizeof( szMsg ), szCPLR );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( OperatorPrompt( vSubtask, "Configuration Management", szMsg,
                        zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                        zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   if ( SetCursorFirstEntityByAttr( vTZCMCPLR, "ReusableCPLR", "ZKey",
                                    vReusableCPLR, "CPLR", "ZKey",
                                    "" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vTZCMCPLR, "ReusableCPLR", zREPOS_PREV );
   }

   DeleteEntity( vReusableCPLR, "CPLR", zREPOS_PREV );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CancelAddReusableCPLR( zVIEW  vSubtask )
{
   zVIEW vTZCMRCRO;

   if ( GetViewByName( &vTZCMRCRO, "CurrentReusables", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRCRO );

   if ( GetViewByName( &vTZCMRCRO, "TZCMRCRO_Select", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRCRO );

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_SelectReusable_UpdateView( zVIEW vSubtask, zVIEW vTZCMRCRO )
{
   zVIEW vReusableCPLR;

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // update View for Window <ReusableCPLR_List>
   while ( ResetViewFromSubobject( vReusableCPLR ) == 0 )
      ;

   while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 )
      ;

   CreateEntity( vReusableCPLR, "CPL", zPOS_LAST );
   SetMatchingAttributesByName( vReusableCPLR, "CPL",
                                vTZCMRCRO, "CPL", zSET_ALL );

   CreateEntity( vReusableCPLR, "CPLR", zPOS_LAST );
   SetMatchingAttributesByName( vReusableCPLR, "CPLR",
                                vTZCMRCRO, "CPLR", zSET_ALL );

   if ( CheckExistenceOfEntity( vTZCMRCRO, "ReusableCPLR" ) == zCURSOR_SET )
   {
      SetViewToSubobject( vReusableCPLR, "ReusableCPLR" );
      zwfnTZCMCPLD_CreateCurrentReusableCPLR( vReusableCPLR, vTZCMRCRO );
   }

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_LoadMetasForCurrentReusables( zVIEW vSubtask )
{
   zVIEW    vCurrentReusables;
   zSHORT   nRC;

   if ( GetViewByName( &vCurrentReusables, "CurrentReusables", 0,
                       zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   // Load Metas for current all reusable CPLR's, not for current CPLR
   nRC = SetCursorFirstEntity( vCurrentReusables, "CPLR", "Installation" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( zwfnTZCMCPLD_LoadCompForReusable( vSubtask, "CurrentReusables",
                                             FALSE ) < 0 )
      {
         return( -1 );
      }

      nRC = SetCursorNextEntity( vCurrentReusables, "CPLR", "Installation" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ReusCreateErrorLod
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ReusCreateErrorLod( zVIEW vSubtask )
{
   zVIEW  vReusError;
   zVIEW  vTZCMCPLR;
   zULONG ulZKey;

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vReusError, "TZCMREUS", vSubtask,
                                   zLEVEL_TASK | zMULTIPLE );
      SetNameForView( vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK );
   }


   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPLR, "CPLR", "ZKey" );

   if ( CheckExistenceOfEntity( vReusError, "CPLR" ) < zCURSOR_SET ||
        SetCursorFirstEntityByInteger( vReusError, "CPLR", "ZKey",
                                       ulZKey, "" ) < zCURSOR_SET )
   {
      CreateEntity( vReusError, "CPLR", zPOS_LAST );
      SetAttributeFromAttribute( vReusError, "CPLR", "Name",
                                 vTZCMCPLR, "CPLR", "Name" );
      SetAttributeFromAttribute( vReusError, "CPLR", "ZKey",
                                 vTZCMCPLR, "CPLR", "ZKey" );
   }

   return( 0 );
} // zwTZCMCPLD_ReusCreateErrorLod

static void
zwfnTZCMCPLD_Reusable_CreateErrorRoot( zVIEW vSubtask, zVIEW vTZCMRCRO,
                                       zSHORT nCheckType )
{
   zVIEW  vReusError;
   zCHAR  szCPLR_CPL_Name[76];
   zCHAR  szCurrentDateTime[ 20 ];

   zwTZCMCPLD_ReusCreateErrorLod( vSubtask );
   GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szCPLR_CPL_Name, zsizeof( szCPLR_CPL_Name ), vTZCMRCRO, "CPLR", "CPLR_CPL_Name" );
   if ( SetCursorFirstEntityByString( vReusError, "ErrorList",
                                      "ReusableCPLR_CPL_Name",
                                      szCPLR_CPL_Name, "" ) >= zCURSOR_SET )
   {
      DeleteEntity( vReusError, "ErrorList", zREPOS_LAST );
   }

   CreateEntity( vReusError, "ErrorList", zPOS_LAST );
   SetAttributeFromString( vReusError, "ErrorList", "ReusableCPLR_CPL_Name",
                           szCPLR_CPL_Name );

  SysGetDateTime( szCurrentDateTime, zsizeof( szCurrentDateTime ) );
   SetAttributeFromString( vReusError, "ErrorList", "DateTime", szCurrentDateTime );

   if ( nCheckType == zCheckReusingCPLR )
      SetAttributeFromString( vReusError, "ErrorList", "CheckReusedBy", "Y" );

   return;
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ReusSetCountError
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ReusSetCountError( zVIEW vSubtask )
{
   zVIEW     vReusError;
   zLONG     lCount;
   zCHAR     szCount[ 10 ];
   zCHAR     szControlText[ 100 ];
   zCHAR     szListBoxTitle[40];
   zCHAR     szReusedBy[ 2 ];

   if ( GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vReusError, "ErrorMsg" ) >= zCURSOR_SET )
   {
      // Set Count Errors to Textfield
      lCount = CountEntitiesForView( vReusError, "ErrorMsg" );

      if ( SetCursorFirstEntityByString( vReusError, "ErrorMsg", "MetaName",
                              "*** Check aborted by User ***", "" ) >= zCURSOR_SET )
      {
         lCount--;
      }

      zltoa( lCount, szCount, zsizeof( szCount ) );
      strcpy_s( szControlText, zsizeof( szControlText ), " " );
      strcat_s( szControlText, zsizeof( szControlText ), szCount );
      strcat_s( szControlText, zsizeof( szControlText ), " duplicate Component(s) found" );
   }
   else
   {
      strcpy_s( szControlText, zsizeof( szControlText ), " No duplicate Component(s)" );
   }

   SetCtrlText( vSubtask, "txtCountError", szControlText );

   // Set ListBox Title and Textfield Text
   strcpy_s( szControlText, zsizeof( szControlText ), "Add Reusable CPLR:" );
   strcpy_s( szListBoxTitle, zsizeof( szListBoxTitle ), "CPLR to be reused" );

   if ( CheckExistenceOfEntity( vReusError, "ErrorList" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szReusedBy, zsizeof( szReusedBy ), vReusError, "ErrorList", "CheckReusedBy" );
      if ( zstrcmp( szReusedBy, "Y" ) == 0 )
      {
         strcpy_s( szControlText, zsizeof( szControlText ), "Check reusing CPLR:" );
         strcpy_s( szListBoxTitle, zsizeof( szListBoxTitle ), "CPLR reusing the current one" );
      }
   }

   SetCtrlText( vSubtask, "txtCheck", szControlText );
   SetCtrlRowColText( vSubtask, "lbErrorList", 0, 4, szListBoxTitle );

   return( 0 );
} // zwTZCMCPLD_ReusSetCountError

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ReusRefreshErrorList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ReusRefreshErrorList( zVIEW vSubtask )
{
   zVIEW vWindow;

   if ( GetSubtaskForWindowName(   vSubtask, &vWindow, "ReusableErrorList" ) >= 0 )
   {
      zwTZCMCPLD_ReusSetCountError( vWindow );
      RefreshWindow( vWindow );
   }

   return( 0 );
} // zwTZCMCPLD_ReusRefreshErrorList

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SelectReusableCPLR( zVIEW  vSubtask )
{
   zVIEW    vTZCMRCRO;
   zVIEW    vTZCMCPLR;
   zVIEW    vReusableToAdd;
   zVIEW    vReusError;
   zVIEW    vWindow;
   zSHORT   nRC;

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMRCRO, "TZCMRCRO_Select", vSubtask, zLEVEL_TASK );

   while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );

   if ( zwfnTZCMCPLD_LoadMetasForCurrentReusables( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Create View and Root Entity for Error List
   zwfnTZCMCPLD_Reusable_CreateErrorRoot( vSubtask, vTZCMRCRO, zCheckReusableCPLR );
   // Create View for Add Reusable CPLR
   zwfnTZCMCPLD_CreateView_ForAddReus( vSubtask, &vReusableToAdd, vTZCMRCRO );
   // Check duplicate Components
   nRC = zwfnTZCMCPLD_CheckCPLR_ForAddReus( vSubtask, vReusableToAdd, FALSE,
                                            zCheckReusableCPLR );
   DropObjectInstance( vReusableToAdd );

   if ( nRC < 0 ) // duplicate Components found
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

      if ( nRC != zCheckReusableWithError &&
           GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK ) > 0 &&
           CheckExistenceOfEntity( vReusError, "ErrorMsg" ) >= zCURSOR_SET )
      {
         // Load Error List Window
         SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessWindow,
                                  "TZCMCPLD", "ReusableErrorList" );
      }
      zwTZCMCPLD_ReusRefreshErrorList( vSubtask );
      return( -1 );
   }

   IncludeSubobjectFromSubobject( vTZCMCPLR, "ReusableCPLR",
                                  vTZCMRCRO, "CPLR", zPOS_AFTER );

   // update View for Window <ReusableCPLR_List>
   zwfnTZCMCPLD_SelectReusable_UpdateView( vSubtask, vTZCMRCRO );

   // Refresh Window <ReusableCPLR_List>
   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   OL_SetCtrlState( vWindow, "ResuableCPLR", zCONTROL_STATUS_EXPANDALL, TRUE );

   // Close ErrorList Window
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ReusableErrorList" ) >= 0 )
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );

   DropObjectInstance( vTZCMRCRO );

   if ( GetViewByName( &vTZCMRCRO, "CurrentReusables", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRCRO );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   MB_SetMessage( vSubtask, 0, "" );
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_TrollReusableToAdd( zVIEW vReusableToAdd, zVIEW  vTZCMRCRO, zVIEW vTempREPO )
{
   zSHORT   nRC;
   zULONG   ulZKey;

   if ( SetViewToSubobject( vTZCMRCRO, "ReusableCPLR" ) == zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "" );
      if ( nRC != zCURSOR_SET )
      {
         ResetViewFromSubobject( vTZCMRCRO );
         return( 0 );
      }

      while ( nRC == zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMRCRO, "CPLR", "ZKey" );
         // Create the compare REPO list
         nRC = SetCursorFirstEntityByInteger( vTempREPO, "CPLR", "ZKey",
                                              (zLONG) ulZKey, "Installation" );

         CreateEntity( vReusableToAdd, "CPL", zPOS_AFTER );
         SetMatchingAttributesByName( vReusableToAdd, "CPL",
                                      vTempREPO, "CPL", zSET_ALL );

         CreateEntity( vReusableToAdd, "CPLR", zPOS_AFTER );
         SetMatchingAttributesByName( vReusableToAdd, "CPLR",
                                      vTempREPO, "CPLR", zSET_ALL );

         zwfnTZCMCPLD_TrollReusableToAdd( vReusableToAdd, vTZCMRCRO, vTempREPO );
         nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "" );
      }
   }

   ResetViewFromSubobject( vTZCMRCRO );
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_CreateView_ForAddReus( zVIEW vSubtask, zPVIEW vReusableToAdd, zVIEW vTZCMRCRO )
{
   zVIEW   vTZCMRCRO_Copy;
   zVIEW   vTZCMCPL;
   zVIEW   vTempREPO;
   zULONG  ulZKey;
   zSHORT  nRC;

   ActivateEmptyObjectInstance( vReusableToAdd, "TZCMRCRO", vSubtask,
                                zLEVEL_TASK | zSINGLE );
   SetNameForView( *vReusableToAdd, "ReusableToAdd", vSubtask, zLEVEL_TASK );
   CreateEntity( *vReusableToAdd, "Installation", zPOS_AFTER );

   // Get the REPO View to obtain the CPL
   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTempREPO, vTZCMCPL, 0 );
   SetNameForView( vTempREPO, "TempREPO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMRCRO, "CPLR", "ZKey" );
   SetCursorFirstEntityByInteger( vTempREPO, "CPLR", "ZKey",
                                  (zLONG) ulZKey, "Installation" );

   CreateEntity( *vReusableToAdd, "CPL", zPOS_AFTER );
   SetMatchingAttributesByName( *vReusableToAdd, "CPL",
                                vTempREPO, "CPL", zSET_ALL );

   if ( CheckExistenceOfEntity( vTempREPO, "CPLR" ) >= zCURSOR_SET )
   {
      CreateEntity( *vReusableToAdd, "CPLR", zPOS_AFTER );
      SetMatchingAttributesByName( *vReusableToAdd, "CPLR",
                                   vTempREPO, "CPLR", zSET_ALL );
   }

   CreateViewFromViewForTask( &vTZCMRCRO_Copy, vTZCMRCRO, 0 );

   nRC = SetCursorFirstEntity( vTZCMRCRO_Copy, "ReusableCPLR", "" );
   while ( nRC >= zCURSOR_SET )
   {
      zwfnTZCMCPLD_TrollReusableToAdd( *vReusableToAdd, vTZCMRCRO_Copy, vTempREPO );
      nRC = SetCursorNextEntity( vTZCMRCRO_Copy, "ReusableCPLR", "" );
   }

   DropView( vTZCMRCRO_Copy );
   DropView( vTempREPO );

   return( nRC );
}

static void
zwfnTZCMCPLD_GetErrorMsg( zPCHAR pchMsg, zLONG lMetaType, zSHORT nCheckType,
                          zPCHAR pchMetaName, zPCHAR pchMetaType,
                          zVIEW vCurrentReusables, zVIEW vReusableToAdd )
{
   zCHAR  szTemp[ 100 ];

   strcpy_s( pchMsg, 500, "CPLR reuse rejected because following Component " );
   strcat_s( pchMsg, 500, "is defined more than once:\n\n\t" );
   strcat_s( pchMsg, 500, pchMetaType );

   if ( lMetaType == zSOURCE_ERD_META || lMetaType == zSOURCE_PENV_META )
   {
      strcpy_s( pchMetaName, 33, "" );
      strcat_s( pchMsg, 500, " exists " );
      strcat_s( pchMsg, 500, "\n\t\tin CPLR '" );
   }
   else
   {
      strcat_s( pchMsg, 500, " '" );
      strcat_s( pchMsg, 500, pchMetaName );
      strcat_s( pchMsg, 500, "' exists \n\t\tin CPLR '" );
   }

   if ( nCheckType == zCheckReusableCPLR )
   {
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCurrentReusables, "CPLR", "Name" );
      strcat_s( pchMsg, 500, szTemp );
      strcat_s( pchMsg, 500, "' and \n\t\tin CPLR '" );
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusableToAdd, "CPLR", "Name" );
   }
   else
   {
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusableToAdd, "CPLR", "Name" );
      strcat_s( pchMsg, 500, szTemp );
      strcat_s( pchMsg, 500, "', \n\t\twith reuses CPLR '" );
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCurrentReusables, "CPLR", "Name" );
   }

   strcat_s( pchMsg, 500, szTemp );
   strcat_s( pchMsg, 500, "'.\n\n" );
   strcat_s( pchMsg, 500, "Would you like to continue to check duplicate Components?" );
}

static zSHORT
zwfnTZCMCPLD_WriteErrorMsg( zVIEW vSubtask, zVIEW vCurrentReusables,
                            zVIEW vReusableToAdd, zLONG lMetaType,
                            zBOOL bCheckAllMetas, zSHORT nCheckType )
{
   zVIEW  vReusError = NULL;
   zCHAR  szMsg[ 500 ];
   zCHAR  szMetaType[50];
   zCHAR  szMetaName[ 33 ];
   zSHORT nRC = zCheckAllReusables;

   GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vCurrentReusables, "Z_MetaDef", "Name" );
   GetStringFromAttributeByContext( szMetaType, vCurrentReusables, "Z_MetaType",
                                    "Type", "CM_Type", 50 );

   if ( !bCheckAllMetas )
   {
      zwfnTZCMCPLD_GetErrorMsg( szMsg, lMetaType, nCheckType, szMetaName,
                                szMetaType, vCurrentReusables, vReusableToAdd );

      zwTZCMCPLD_ReusRefreshErrorList( vSubtask );

      if ( OperatorPrompt( vSubtask, "Configuration Management", szMsg,
                           zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                           zICON_QUESTION ) == zRESPONSE_NO )
      {
         nRC = zCheckReusableWithError;
      }
   }

   // write duplicate Components to Error List
   CreateEntity( vReusError, "ErrorMsg", zPOS_LAST );

   if ( lMetaType == -1 )
      SetAttributeFromString( vReusError, "ErrorMsg", "MetaName", szMetaName );

   SetAttributeFromAttribute( vReusError, "ErrorMsg", "MetaType",
                              vCurrentReusables, "Z_MetaType", "Type" );
   SetAttributeFromAttribute( vReusError, "ErrorMsg", "CurrentCPLR",
                              vCurrentReusables, "CPLR", "Name" );
   SetAttributeFromAttribute( vReusError, "ErrorMsg", "CPLR_ToBeReused",
                              vReusableToAdd, "CPLR", "Name" );

   if ( nRC == zCheckReusableWithError )
   {
      CreateEntity( vReusError, "ErrorMsg", zPOS_LAST );
      SetAttributeFromString( vReusError, "ErrorMsg", "MetaName",
                              "*** Check aborted by User ***" );
   }

   return( nRC );
}

static zSHORT
zwfnTZCMCPLD_CheckERD_TE_PPE( zVIEW vSubtask, zVIEW vCurrentReusables,
                              zVIEW vReusableToAdd, zBOOL bCheckAllMetas,
                              zSHORT nCheckType )
{
   zSHORT  nRC = 0;
   zSHORT  nReturnValue = 0;

   // check duplicate ERD
   if ( SetCursorFirstEntityByInteger( vReusableToAdd, "Z_MetaType",
                                       "Type", zSOURCE_ERD_META,
                                       "" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByInteger( vCurrentReusables, "Z_MetaType",
                                          "Type", zSOURCE_ERD_META,
                                          "Installation" ) >= zCURSOR_SET )
      {
         nRC = zwfnTZCMCPLD_WriteErrorMsg( vSubtask, vCurrentReusables,
                                           vReusableToAdd, zSOURCE_ERD_META,
                                           bCheckAllMetas, nCheckType );
         if ( nRC < 0 )
         {
            nReturnValue = nRC;

            if ( nReturnValue == zCheckReusableWithError )
               return( nReturnValue );
            else
               bCheckAllMetas = TRUE;
         }
      }
   }

   // check duplicate PPE
   if ( SetCursorFirstEntityByInteger( vReusableToAdd, "Z_MetaType",
                                       "Type", zSOURCE_PENV_META,
                                       "" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByInteger( vCurrentReusables, "Z_MetaType",
                                          "Type", zSOURCE_PENV_META,
                                          "Installation" ) >= zCURSOR_SET )
      {
         nRC = zwfnTZCMCPLD_WriteErrorMsg( vSubtask, vCurrentReusables,
                                           vReusableToAdd, zSOURCE_PENV_META,
                                           bCheckAllMetas, nCheckType );
         if ( nRC < 0 )
         {
            nReturnValue = nRC;

            if ( nReturnValue == zCheckReusableWithError )
               return( nReturnValue );
            else
               bCheckAllMetas = TRUE;
         }
      }
   }

   return( nReturnValue );
}


static zSHORT
zwfnTZCMCPLD_CheckMetasForAddReus( zVIEW vSubtask, zVIEW vCurrentReusables,
                                   zVIEW vReusableToAdd, zBOOL bCheckAllMetas,
                                   zSHORT nCheckType )
{
   zCHAR  szName[ 33 ];
   zLONG  lType;
   zLONG  lTypeToAdd;
   zSHORT nRC;
   zSHORT nReturnValue = 0;

   nRC = SetCursorFirstEntity( vReusableToAdd, "Z_MetaDef", "CPLR" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szName, zsizeof( szName ), vReusableToAdd, "Z_MetaDef", "Name" );

      // Exists a component with this name?
      if ( SetCursorFirstEntityByString( vCurrentReusables, "Z_MetaDef", "Name",
                                         szName, "Installation" ) >= zCURSOR_SET )
      {
         do
         {
            nRC = 0;

            // check the type
            GetIntegerFromAttribute( &lType, vCurrentReusables, "Z_MetaType", "Type" );
            GetIntegerFromAttribute( &lTypeToAdd, vReusableToAdd, "Z_MetaType", "Type" );

            if ( lType == lTypeToAdd && lType != zSOURCE_PENV_META )
               nRC = -1;

            // check duplicate Name between Dialogs and LODs
            if ( lType == zSOURCE_DIALOG_META && lTypeToAdd == zSOURCE_LOD_META ||
                 lType == zSOURCE_LOD_META && lTypeToAdd == zSOURCE_DIALOG_META )
            {
               nRC = -2;
            }

            if ( nRC < 0 )
            {
               nRC = zwfnTZCMCPLD_WriteErrorMsg( vSubtask, vCurrentReusables,
                                                 vReusableToAdd, -1, bCheckAllMetas,
                                                 nCheckType );
               if ( nRC < 0 )
               {
                  nReturnValue = nRC;

                  if ( nReturnValue == zCheckReusableWithError )
                     return( nReturnValue );
                  else
                     bCheckAllMetas = TRUE;
               }
            }

         } while ( SetCursorNextEntityByString( vCurrentReusables, "Z_MetaDef",
                                                "Name", szName,
                                                "Installation" ) >= zCURSOR_SET );
      } //endif ( SetCursorFirstEntityByString( vCurrentReusables, "Z_MetaDef"...

      nRC = SetCursorNextEntity( vReusableToAdd, "Z_MetaDef", "CPLR" );
   } // endwhile ( nRC >= zCURSOR_SET )

   return( nReturnValue );
}



static zSHORT
zwfnTZCMCPLD_CheckCPLR_ForAddReus( zVIEW vSubtask, zVIEW vReusableToAdd,
                                   zBOOL bCheckAllMetas, zSHORT nCheckType )
{
   zVIEW    vCurrentReusables;
   zSHORT   nRC;
   zSHORT   nReturnValue = 0;
   zCHAR    szMsg[ 100 ];

   GetViewByName( &vCurrentReusables, "CurrentReusables", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vReusableToAdd, "CPLR", "Installation" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( zwfnTZCMCPLD_LoadCompForReusable( vSubtask, "ReusableToAdd",
                                             FALSE ) < 0 )
      {
         return( -1 );
      }

      strcpy_s( szMsg, zsizeof( szMsg ), "Checking CPLR " );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ),
                              vReusableToAdd, "CPLR", "Name" );
      MB_SetMessage( vSubtask, 0, szMsg );

      nRC = zwfnTZCMCPLD_CheckERD_TE_PPE( vSubtask, vCurrentReusables,
                                          vReusableToAdd, bCheckAllMetas,
                                          nCheckType);
      if ( nRC < 0 )
      {
         if ( nRC == zCheckReusableWithError )
         {
            return( zCheckReusableWithError );
         }
         else
         {
            bCheckAllMetas = TRUE;
            nReturnValue = -1;
         }
      }

      nRC = zwfnTZCMCPLD_CheckMetasForAddReus( vSubtask, vCurrentReusables,
                                               vReusableToAdd, bCheckAllMetas,
                                               nCheckType );
      if ( nRC < 0 )
      {
         if ( nRC == zCheckReusableWithError )
         {
            return( zCheckReusableWithError );
         }
         else
         {
            bCheckAllMetas = TRUE;
            nReturnValue = -1;
         }
      }

      nRC = SetCursorNextEntity( vReusableToAdd, "CPLR", "Installation" );
   }

   MB_SetMessage( vSubtask, 0, "" );
   return( nReturnValue );
}


static zSHORT
zwfnTZCMCPLD_AskForRemoveERD_TE_SA( zVIEW vSubtask, zLONG lType )
{
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   if ( lType == (zLONG) zSOURCE_ERD_META )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Removing Check Out State from Data Model will remove\n"
                      "Check Out State for all Subject Areas and TE!" );
   }
   else
   if ( lType == (zLONG) zSOURCE_DTE_META )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Removing Check Out State from TE will remove\n"
                      "Check Out State for Data Model and all Subject Areas!" );
   }
   else
   if ( lType == (zLONG) zSOURCE_SA_META )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Removing Check Out State from a Subject Area will remove\n"
                      "Check Out State for Data Model, all Subject Areas and TE!" );
   }
   else
   {
      return( 0 );
   }

   strcat_s( szMsg, zsizeof( szMsg ), "\n\nContinue with Remove Check Out State?" );

   if ( MessagePrompt( vSubtask, "CM00253", "Configuration Management",
                       szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                       0 ) == zRESPONSE_YES )
   {
      return( zRESPONSE_YES );
   }
   else
   {
      return( -1 );
   }
}


static zSHORT
zwfnTZCMCPLD_RemoveStatusERD_TE_SA( zVIEW vCPLR_AllMetas )
{
   zVIEW  vCPLR_AllCopy;
   zSHORT nRC;

   CreateViewFromViewForTask( &vCPLR_AllCopy, vCPLR_AllMetas, 0 );

   if ( SetCursorFirstEntityByInteger( vCPLR_AllCopy, "Z_MetaDef", "Type",
                                       (zLONG) zSOURCE_ERD_META,
                                       "" ) >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vCPLR_AllCopy, "UserMetaStatus" )
                                   == zCURSOR_SET )
      {
         DeleteEntity( vCPLR_AllCopy, "UserMetaStatus", zREPOS_NONE );
      }
   }

   if ( SetCursorFirstEntityByInteger( vCPLR_AllCopy, "Z_MetaDef", "Type",
                                       (zLONG) zSOURCE_DTE_META,
                                       "" ) >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vCPLR_AllCopy, "UserMetaStatus" )
                                   == zCURSOR_SET )
      {
         DeleteEntity( vCPLR_AllCopy, "UserMetaStatus", zREPOS_NONE );
      }
   }

   nRC = SetCursorFirstEntityByInteger( vCPLR_AllCopy, "Z_MetaDef", "Type",
                                        (zLONG) zSOURCE_SA_META, "" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vCPLR_AllCopy, "UserMetaStatus" )
                                   == zCURSOR_SET )
      {
         DeleteEntity( vCPLR_AllCopy, "UserMetaStatus", zREPOS_NONE );
      }

      nRC = SetCursorNextEntityByInteger( vCPLR_AllCopy, "Z_MetaDef", "Type",
                                          (zLONG) zSOURCE_SA_META, "" );
   }

   SetAttributeFromInteger( vCPLR_AllCopy, "CPLR", "DeleteMeta", 1 );
   DropView( vCPLR_AllCopy );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_RemoveCO_Status( zVIEW  vSubtask )
{
   zVIEW  vCPLR_AllMetas;
   zLONG  lType;
   zCHAR  szMsg[ 100 ];
   zCHAR  szName[ 33 ];
   zSHORT nRC;

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vCPLR_AllMetas, "UserMetaStatus" )
                                == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vCPLR_AllMetas, "Z_MetaDef", "Type" );
      nRC = zwfnTZCMCPLD_AskForRemoveERD_TE_SA( vSubtask, lType );

      if ( nRC == 0 )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vCPLR_AllMetas, "Z_MetaDef", "Name" );
         strcpy_s( szMsg, zsizeof( szMsg ), "OK to Remove Check Out Status for '" );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "'?" );

         nRC = MessagePrompt( vSubtask, "CM00140", "Configuration Management",
                              szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 );
         if ( nRC == zRESPONSE_YES )
         {
            DeleteEntity( vCPLR_AllMetas, "UserMetaStatus", zREPOS_NONE );
            SetAttributeFromInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta", 1 );
         }
      }
      else
      if ( nRC == zRESPONSE_YES )
      {
         zwfnTZCMCPLD_RemoveStatusERD_TE_SA( vCPLR_AllMetas );
      }

      if ( nRC != zRESPONSE_YES )
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}


static zSHORT
CreateInstallEntity( zVIEW  vSubtask,
                     zVIEW  lpView,
                     zPCHAR szEntityName,
                     zSHORT nPosition )
{
   zVIEW   vTZCMZKEY;
   zULONG  ulZKey;

   if ( GetViewByName( &vTZCMZKEY, "TZCMZKEY", vSubtask, zLEVEL_TASK ) < 0 )
   {
      MessageSend( lpView, "CM00136", "Configuration Management",
                   "Unable to set ZKey in operation CreateInstallEntity",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMZKEY,
                            "Installation", "NextInstallZKey" );

   if ( ulZKey == 0 )
      ulZKey++;

   CreateEntity( lpView, szEntityName, nPosition );

   if ( SetAttributeFromInteger( lpView,
                                 szEntityName, "ZKey", ulZKey ) != 0 )
   {
      DeleteEntity( lpView, szEntityName, zREPOS_NONE );
      MessageSend( lpView, "CM00136", "Configuration Management",
                   "Unable to set ZKey in operation CreateInstallEntity",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SetAttributeFromInteger( vTZCMZKEY, "Installation", "NextInstallZKey",
                            ++ulZKey );

   if ( CommitObjectInstance( vTZCMZKEY ) != 0 )
   {
      MessageSend( lpView, "PN00219", "Configuration Maintenance",
                  "Unable to save lase ZKey in operation CreateInstallEntity",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteWorkstation( zVIEW  vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vTZCMWKSO;
   zVIEW    vKZDBHQUA;
   zLONG    lRepo_ViewCluster;
   zCHAR    szZKey[ 20 ];
   zCHAR    szMsg[ 254 ];
   zCHAR    szViewName[ 100 ];
   zSHORT   nRC;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Workstation '" );
   GetStringFromAttribute( szZKey, zsizeof( szZKey ), vTZCMREPO, "RepositoryClient", "WKS_Id" );
   strcat_s( szMsg, zsizeof( szMsg ), szZKey );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( OperatorPrompt( vSubtask, "Configuration Management", szMsg,
                        zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                        zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   // Activate an instance of the work station object and use it
   // to delete the RepositoryClient entity because it has all the
   // paths necessary to support the delete.
   ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA", vSubtask, zSINGLE );
   CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );

   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "RepositoryClient" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "RepositoryClient" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   GetStringFromAttribute( szZKey, zsizeof( szZKey ), vTZCMREPO, "RepositoryClient", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );

   nRC = ActivateObjectInstance( &vTZCMWKSO, "TZCMWKSO", vSubtask, vKZDBHQUA, zSINGLE );
   DropObjectInstance( vKZDBHQUA );

   if ( nRC != zCALL_ERROR )
   {
      // Get Repo View Cluster.and commit it
      GetIntegerFromAttribute( &lRepo_ViewCluster, vTZCMREPO, "Installation",
                              "Repo_ViewCluster" );

      zsprintf( szViewName, "TZCMFILO_%p", vTZCMWKSO );
      SetNameForView( vTZCMWKSO, szViewName, vSubtask, zLEVEL_TASK );

      AddToViewCluster( lRepo_ViewCluster, vTZCMWKSO, zCOMMIT_DROPOBJECTINSTANCE );

      if ( DeleteEntity( vTZCMWKSO, "RepositoryClient", zREPOS_PREV ) != zCALL_ERROR )
         DeleteEntity( vTZCMREPO, "RepositoryClient", zREPOS_PREV );
   }

   return( nRC );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildWS_UserList( zVIEW  vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vKZDBHQUA;
   zVIEW    vTZCMWULO;
   zLONG    ulWKS_ID;
   zSHORT   nRC;
   zCHAR    szZKey[12];
   zCHAR    szERR_Msg[ 254 ];
   zCHAR    szWKS_Desc[ 61 ];

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG)&ulWKS_ID, vTZCMREPO, "RepositoryClient", "ZKey" );

   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   }
   else
      return( -1 );

   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "RepositoryClient" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "RepositoryClient" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   zltoa( ulWKS_ID, szZKey, zsizeof( szZKey ) );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );
   nRC = ActivateObjectInstance( &vTZCMWULO, "TZCMWULO",
                                 vSubtask, vKZDBHQUA, zSINGLE );
   DropObjectInstance( vKZDBHQUA );

   if ( nRC < 0 )
   {
      GetStringFromAttribute( szWKS_Desc, zsizeof( szWKS_Desc ), vTZCMREPO, "RepositoryClient", "Desc" );
      strcpy_s( szERR_Msg, zsizeof( szERR_Msg ), "Unable to Activate Workstation " );
      strcat_s( szERR_Msg, zsizeof( szERR_Msg ), szWKS_Desc );
      strcat_s( szERR_Msg, zsizeof( szERR_Msg ), "!" );
      MessageSend( vSubtask, "CM00138", "Configuration Management",
                   szERR_Msg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SetNameForView( vTZCMWULO, "TZCMWULO", vSubtask, zLEVEL_TASK );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DropTZCMWULO( zVIEW  vSubtask )
{
   zVIEW    vTZCMWULO;

   if ( GetViewByName( &vTZCMWULO, "TZCMWULO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMWULO );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
// OPERATION   zwTZCMCPLD_HelpAbout
// PURPOSE:    This Entry does the About
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_SYSADM, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZCMCPLD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZCMCPLD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZCMCPLD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_GrayWorkstation( zVIEW vSubtask )
{
   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      EnableAction( vSubtask, "DeleteWorkstation", FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_GrayInstallUsers( zVIEW vSubtask )
{
   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      EnableAction( vSubtask, "AddUser", FALSE );
      EnableAction( vSubtask, "DeleteUser", FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_GrayUserMaintenance( zVIEW vSubtask )
{
   zBOOL bSysadm = FALSE;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      bSysadm = TRUE;
   }

   ED_LimitTextLength( vSubtask, "edVerification", 8 );

   SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ENABLED, bSysadm );
   SetCtrlState( vSubtask, "edDesc", zCONTROL_STATUS_ENABLED, bSysadm );
   SetCtrlState( vSubtask, "edPassword", zCONTROL_STATUS_ENABLED, bSysadm );
   SetCtrlState( vSubtask, "edVerification", zCONTROL_STATUS_ENABLED, bSysadm );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CancelSignOn( zVIEW vSubtask )
{
   zSHORT   nRC;
   zVIEW    vTZCMQSYO;

   // Clean up as necessary for cancel on sign on.
   if ( GetViewByName( &vTZCMQSYO, "vTZCMQSYO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DeleteEntity( vTZCMQSYO, "SysEnqueue", zREPOS_NONE );
      nRC = CommitObjectInstance( vTZCMQSYO );
      if ( nRC == 0 )
      {
         DropView( vTZCMQSYO );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SetDIL_Message( zVIEW vSubtask )
{
   zCHAR szWorkMessage[ 300 ];
   zVIEW vTZCMCPL;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPL" ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szWorkMessage, zsizeof( szWorkMessage ), vTZCMCPL, "CPL", "Desc" );
      MB_SetMessage( vSubtask, 1, szWorkMessage );
   }

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szWorkMessage, zsizeof( szWorkMessage ), vTZCMCPL, "CPLR", "Desc" );
      MB_SetMessage( vSubtask, 1, szWorkMessage );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CancelCPL( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vWindow;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   // If the CPL does not contain a CPLR in the REPO object,
   // then delete it.  This means the CPLR being canceled must have
   // been created as part of a "new" CPL.

   // No delete by save CPLR as and update CPL
   GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" );

   if ( (CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) != zCURSOR_SET) &&
         (!vWindow || !CompareAttributeToString( vTZCMCPL, "CPL",
                                                 "NewOrUpdate", "N" )) )
   {
      DeleteEntity( vTZCMCPL, "CPL", zREPOS_BEFORE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadAuditTrailC( zVIEW vSubtask )
{
   zVIEW    vTZCMCPL;
   zVIEW    vTZBRAU2O;
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szZKey[ 18 ];
   zSHORT   nRC;

   if ( GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) == zCURSOR_SET )
   {

      if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                         zSINGLE ) >= 0 )
      {
         SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
         CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
         CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      }
      else
         return( -1 );

      GetIntegerFromAttribute( (zPLONG)&ulZKey, vTZCMCPL, "CPLR", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "CPLR" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
      zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );


      nRC = ActivateObjectInstance( &vTZBRAU2O, "TZBRAU2O", vSubtask,
                                    vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
      DropObjectInstance( vKZDBHQUA );

      if ( CheckExistenceOfEntity( vTZBRAU2O, "AuditTrail" ) == 0 )
         SetNameForView( vTZBRAU2O, "TZBRAU2O", vSubtask, zLEVEL_TASK );
   }
   else
   {
      MessageSend( vSubtask, "CM00299", "Configuration Management",
                   "There are no Audit Trail Information for a non-repository LPLR.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadAuditTrailM( zVIEW vSubtask )
{

   zVIEW    vAllMetas;
   zVIEW    vTZBRAUDO;
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szZKey[ 18 ];
   zSHORT   nRC;

   if ( GetViewByName( &vAllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vAllMetas, "Z_MetaDef" ) == 0 )
      {
         if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                          zSINGLE ) >= 0 )
         {
            SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
            CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
            CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
         }
         else
            return( -1 );

         GetIntegerFromAttribute( (zPLONG)&ulZKey, vAllMetas, "Z_MetaDef", "ZKey" );
         SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "Z_MetaDef" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "Z_MetaDef" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
         zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );


         nRC = ActivateObjectInstance( &vTZBRAUDO, "TZBRAUDO", vSubtask,
                                       vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
         DropObjectInstance( vKZDBHQUA );

         if ( CheckExistenceOfEntity( vTZBRAUDO, "AuditTrail" ) ==0 )
            SetNameForView( vTZBRAUDO, "TZBRAUDO", vSubtask, zLEVEL_TASK );
      }
      else
      {
         MessageSend( vSubtask, "CM00299", "Configuration Management",
                     "There are no AuditTrail-Information for the component (2).",
                     zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

         return( -1 );

      }

   }
   else
      return( -1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_LoadAuditTrailM2_All( zVIEW vSubtask )
{
   zVIEW    vAllMetas;
   zVIEW    vTZBRAUDO;
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szZKey[ 18 ];
   zSHORT   nRC;

   if ( GetViewByName( &vAllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vAllMetas, "Z_MetaDef" ) == 0 )
      {
         if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                            zSINGLE ) >= 0 )
         {
            SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
            CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
            CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
         }
         else
            return( -1 );

         GetIntegerFromAttribute( (zPLONG) &ulZKey, vAllMetas, "Z_MetaDef", "ZKey" );
         SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "Z_MetaDef" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "Z_MetaDef" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
         zltoa( ulZKey, szZKey, zsizeof( szZKey ) );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );

         nRC = ActivateObjectInstance( &vTZBRAUDO, "TZBRAUDO", vSubtask,
                                       vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
         DropObjectInstance( vKZDBHQUA );

         if ( CheckExistenceOfEntity( vTZBRAUDO, "AuditTrail" ) == zCURSOR_SET )
         {
            SetNameForView( vTZBRAUDO, "TZBRAUDO", vSubtask, zLEVEL_TASK );
         }
      }
      else
      {
         MessageSend( vSubtask, "CM00299", "Configuration Management",
                      "There are no AuditTrail-Information for the component (2).",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }
   else
      return( -1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadAuditTrailM2
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadAuditTrailM2( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zVIEW  vCPLR_AllMetas;
   zULONG ulZKey;

   if ( GetViewByName( &vMetaList, "CPLR_MetaList", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vMetaList, "Z_MetaDef", "ZKey" );
   SetCursorFirstEntityByInteger( vCPLR_AllMetas, "Z_MetaDef", "ZKey",
                                 (zLONG) ulZKey, "" );

   zwTZCMCPLD_LoadAuditTrailM2_All( vSubtask );

   return( 0 );
} // zwTZCMCPLD_LoadAuditTrailM2

zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteFile( zVIEW vSubtask )
{
   zVIEW  vTZCMFILE;
   zCHAR  szName[ 33 ];
   zCHAR  szExtension[ 4 ];
   zCHAR  szMsg[ 300 ];

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // Prompt the user before actually deleting the file.
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMFILE, "File", "Name" );
   GetStringFromAttribute( szExtension, zsizeof( szExtension ), vTZCMFILE, "File", "Extension" );
   strcpy_s( szMsg, zsizeof( szMsg ),"W A R N I N G");
   strcat_s( szMsg, zsizeof( szMsg ), "\n\n You are going to delete a file from Repository \n'");
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   strcat_s( szMsg, zsizeof( szMsg ), "." );
   strcat_s( szMsg, zsizeof( szMsg ), szExtension );
   strcat_s( szMsg, zsizeof( szMsg ), "'.\n\n" );
   strcat_s( szMsg, zsizeof( szMsg ), "This could D E S T R O Y the Repository,\n" );
   strcat_s( szMsg, zsizeof( szMsg ), "if you do not fully understand what you are doing!" );
   strcat_s( szMsg, zsizeof( szMsg ), "\n\nContinue?" );

   if ( MessagePrompt( vSubtask, "CM00112", "Configuration Management",
                        szMsg, zBEEP, zBUTTONS_YESNO,
                        zRESPONSE_NO, zICON_ERROR ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   // delete the Meta File
   oTZCMCPRO_DeleteFileWithCluster( vSubtask, vTZCMFILE, szName, szExtension );

   RefreshCtrl( vSubtask, "lbFileList" );
   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_CreateReusablesForCurrentCPLR( zVIEW vSubtask, zULONG ulZKey,
                                            zPVIEW vCurrentReusables )
{
   zVIEW   vReusableCPLR;
   zSHORT  nRC;

   GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK );

   zwTZCMCPLD_CreateViewForCurrentReusables( vSubtask, vCurrentReusables,
                                             NULL, vReusableCPLR, ulZKey );

   nRC = zwfnTZCMCPLD_LoadMetasForCurrentReusables( vSubtask );

   return( nRC );
}

static zSHORT
zwfnTZCMCPLD_ReusGetParentAndCheck( zVIEW vSubtask, zVIEW vTZCMRCRO_All,
                                    zBOOL bCheckAllMetas )
{
   zVIEW   vReusParent;
   zVIEW   vCurrentReusables;
   zVIEW   vReusableToAdd = NULL;
   zULONG  ulZKey;
   zSHORT  nRC;

   if ( GetViewByName( &vCurrentReusables, "CurrentReusables", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vReusParent, vTZCMRCRO_All, 0 );
   SetNameForView( vReusParent, "ResuParent", vSubtask, zLEVEL_TASK );

   // Search Parent CPLR
   do
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vReusParent, "CPLR", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vReusParent, "ReusableCPLR", "ZKey",
                                           ulZKey, "Installation" );

   } while ( nRC >= zCURSOR_SET );

   zwfnTZCMCPLD_CreateView_ForAddReus( vSubtask, &vReusableToAdd, vReusParent );
   DropView( vReusParent );

   // Delete current reusables from vReusableToAdd
   nRC = SetCursorFirstEntity( vCurrentReusables, "CPLR", "Installation" );
   while( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByAttr( vReusableToAdd, "CPLR", "ZKey",
                                        vCurrentReusables, "CPLR", "ZKey",
                                        "Installation" );
      while( nRC >= zCURSOR_SET )
      {
         DeleteEntity( vReusableToAdd, "CPLR", zREPOS_NONE );
         nRC = SetCursorNextEntityByAttr( vReusableToAdd, "CPLR", "ZKey",
                                          vCurrentReusables, "CPLR", "ZKey",
                                          "Installation" );
      }

      nRC = SetCursorNextEntity( vCurrentReusables, "CPLR", "Installation" );
   }

   nRC = zwfnTZCMCPLD_CheckCPLR_ForAddReus( vSubtask, vReusableToAdd,
                                            bCheckAllMetas, zCheckReusingCPLR );
   DropObjectInstance( vReusableToAdd );

   return( nRC );
}

static zSHORT
zwfnTZCMCPLD_LoadAllCPLRs( zVIEW vSubtask, zPVIEW vTZCMRCRO_All )
{
   // Load all CPLR's with reusable CPLR'S
   if ( GetViewByName( vTZCMRCRO_All, "TZCMRCRO_All", vSubtask, zLEVEL_TASK ) < 0 )
   {
      if ( ActivateObjectInstance( vTZCMRCRO_All, "TZCMRCRO", vSubtask,
                                   0, zSINGLE ) == zCALL_ERROR )
      {
         MessageSend( vSubtask, "CM00129", "Configuration Management",
                      "Activation failed on load of Repository",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      SetNameForView( *vTZCMRCRO_All, "TZCMRCRO_All", vSubtask, zLEVEL_TASK );
   }

   while ( ResetViewFromSubobject( *vTZCMRCRO_All ) == 0 );

   return( 0 );
}

static zSHORT
zwfnTZCMCPLD_ReusCheckAllCPLRs( zVIEW vSubtask, zVIEW vTZCMCPLR )
{
   zVIEW  vCurrentReusables;
   zVIEW  vTZCMRCRO_All;
   zULONG ulZKey;
   zSHORT nRC;
   zSHORT nReturnValue = 0;
   zBOOL  bCheckAllMetas = FALSE;

   // Load all CPLR's with reusable CPLR'S
   if ( zwfnTZCMCPLD_LoadAllCPLRs( vSubtask, &vTZCMRCRO_All ) < 0 )
      return( -1 );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPLR, "CPLR", "ZKey" );

   // Will this CPLR be reused by an other CPLR's, I check if duplicate
   // components exist in one or more CPLR's
   if ( SetCursorFirstEntityByInteger( vTZCMRCRO_All, "ReusableCPLR", "ZKey",
                                       ulZKey, "Installation" ) < zCURSOR_SET )
   {
      return( 0 );
   }

   // Create Work View for current CPLR with all Reusables
   if ( zwfnTZCMCPLD_CreateReusablesForCurrentCPLR( vSubtask, ulZKey,
                                                    &vCurrentReusables ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Create View and Root Entity for Error List
   SetCursorFirstEntityByInteger( vCurrentReusables, "CPLR", "ZKey",
                                  ulZKey, "Installation" );
   zwfnTZCMCPLD_Reusable_CreateErrorRoot( vSubtask, vCurrentReusables,
                                          zCheckReusingCPLR );

   // Searching for all CPLR, who reusing the current CPLR
   nRC = SetCursorFirstEntityByInteger( vTZCMRCRO_All, "ReusableCPLR", "ZKey",
                                        ulZKey, "Installation" );
   while ( nRC >= zCURSOR_SET )
   {
      // Create Work View for CPLR reusing the current one and check duplicate
      // Components
      nRC = zwfnTZCMCPLD_ReusGetParentAndCheck( vSubtask, vTZCMRCRO_All,
                                                bCheckAllMetas );
      if ( nRC < 0 )
      {
         if ( nRC == zCheckReusableWithError )
            return( zCheckReusableWithError );
         else
         {
            bCheckAllMetas = TRUE;
            nReturnValue = -1;
         }
      }
      nRC = SetCursorNextEntityByInteger( vTZCMRCRO_All, "ReusableCPLR", "ZKey",
                                          ulZKey, "Installation" );
   }

   if ( GetViewByName( &vCurrentReusables, "CurrentReusables", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vCurrentReusables );

   return( nReturnValue );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CancelReusableCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CancelReusableCPLR( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vReusableCPLR;
   zVIEW  vTZCMCPLR;

   if ( GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CancelSubobject( vTZCMCPLR, "CPLR" );

   if ( GetViewByName( &vReusableCPLR, "TZCMRCRO_All", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   // Was the window <Reusable CPLR> opened via the menu option
   // <Reusable CPLR> or the window <CPLR_Maintenance>?
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) < 0 )
   {
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );

      if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
         DropObjectInstance( vReusableCPLR );
   }

   return( 0 );
} // zwTZCMCPLD_CancelReusableCPLR

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_OKReusableCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_OKReusableCPLR( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vReusError;
   zVIEW  vTZCMCPL;
   zVIEW  vTZCMCPLR;
   zVIEW  vReusableCPLR;
   zVIEW  vTZCMRCRO_All;
   zSHORT nRC = 0;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      zwTZCMCPLD_CancelReusableCPLR( vSubtask );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vTZCMCPL,  "TZCMCPL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );

   // Will this CPLR be reused by an other CPLR's, I check if duplicate
   // components exist in one or more CPLR's
   if ( CheckExistenceOfEntity( vTZCMCPLR, "ReusableCPLR" ) >= zCURSOR_SET )
   {
      nRC = zwfnTZCMCPLD_ReusCheckAllCPLRs( vSubtask, vTZCMCPLR );
      if ( nRC < 0 ) // duplicate Components found
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

         if ( nRC != zCheckReusableWithError &&
              GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK ) > 0 &&
              CheckExistenceOfEntity( vReusError, "ErrorMsg" ) >= zCURSOR_SET )
         {
            // Load Error List Window
            SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessWindow,
                                     "TZCMCPLD", "ReusableErrorList" );
         }
         zwTZCMCPLD_ReusRefreshErrorList( vSubtask );
         return( -1 );
      }
      // Close ErrorList Window
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ReusableErrorList" ) >= 0 )
         InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );
   }

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   MB_SetMessage( vSubtask, 0, "" );

   AcceptSubobject( vTZCMCPLR, "CPLR" );

   if ( GetViewByName( &vTZCMRCRO_All, "TZCMRCRO_All", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRCRO_All );

   // Was the window <Reusable CPLR> opened via the menu option
   // <Reusable CPLR>, commit values
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) < 0 )
   {
      if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
         DropObjectInstance( vReusableCPLR );

      nRC = zwfnTZCMCPLD_CommitCPL_CPLR( vSubtask, vTZCMCPL, vTZCMCPLR );
   }

   return( nRC );
} // zwTZCMCPLD_OKReusableCPLR

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_BuildUserOnWorkstList
//
// load Workstations with Users
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_BuildUserOnWorkstList( zVIEW vSubtask )
{
   zVIEW vTZCMWKUS;

   ActivateObjectInstance( &vTZCMWKUS, "TZCMWKUS", vSubtask, 0, zMULTIPLE );
   SetNameForView( vTZCMWKUS, "TZCMWKUS", vSubtask, zLEVEL_TASK );

   return( 0 );
} // zwTZCMCPLD_BuildUserOnWorkstList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_RefreshUserOnWorkst
//
// refresh the Listbox lbUserOnWorkstation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_RefreshUserOnWorkst( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "edDesc" );
   RefreshCtrl( vSubtask, "lbUserOnWorkstation" );

   return( 0 );
} // zwTZCMCPLD_RefreshUserOnWorkst

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CloseUserOnWorkst
//
// DropView TZCMWKUS
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CloseUserOnWorkst( zVIEW vSubtask )
{
   zVIEW  vTZCMWKUS;
   zVIEW vTZCMWULO;

   GetViewByName( &vTZCMWKUS, "TZCMWKUS", vSubtask, zLEVEL_TASK );
   DropObjectInstance( vTZCMWKUS );

   if ( GetViewByName( &vTZCMWULO, "TZCMWULO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMWULO );

   return( 0 );
} // zwTZCMCPLD_CloseUserOnWorkst

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CheckOutItems
//
// load checked Out Items for select user
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_CheckOutItems( zVIEW vSubtask )
{
   zVIEW    vTZCMWULO;
   zVIEW    vKZDBHQUA;
   zVIEW    vTZCMWKUS;
   zLONG    ulWKS_ID;
   zSHORT   nRC;
   zSHORT   nNewActivate;
   zCHAR    szZKey[12];
   zCHAR    szERR_Msg[ 254 ];
   zCHAR    szWKS_Desc[ 61 ];

   GetViewByName( &vTZCMWKUS, "TZCMWKUS", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTZCMWULO, "TZCMWULO", vSubtask, zLEVEL_TASK ) < 0 )
      nNewActivate = 1;
   else
   {
      nNewActivate = 0;
      //if a new Workstation selected, then activate vTZCMWKUS, else not activate
      if ( CompareAttributeToAttribute( vTZCMWKUS, "RepositoryClient", "ZKey",
                                        vTZCMWULO, "RepositoryClient", "ZKey") != 0 )
         nNewActivate = 1;
   }


   if ( nNewActivate == 1 )
   {
      GetIntegerFromAttribute( (zPLONG)&ulWKS_ID, vTZCMWKUS, "RepositoryClient", "ZKey" );

      if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                      zSINGLE ) >= 0 )
      {
         SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
         CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
         CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      }
      else
         return( -1 );

      SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "RepositoryClient" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "RepositoryClient" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
      zltoa( ulWKS_ID, szZKey, zsizeof( szZKey ) );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );
      nRC = ActivateObjectInstance( &vTZCMWULO, "TZCMWULO",
                                    vSubtask, vKZDBHQUA, zSINGLE );
      DropObjectInstance( vKZDBHQUA );

      if ( nRC < 0 )
      {
         GetStringFromAttribute( szWKS_Desc, zsizeof( szWKS_Desc ), vTZCMWKUS, "RepositoryClient", "Desc" );
         strcpy_s( szERR_Msg, zsizeof( szERR_Msg ), "Unable to Activate Workstation " );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), szWKS_Desc );
         strcat_s( szERR_Msg, zsizeof( szERR_Msg ), "!" );
         MessageSend( vSubtask, "CM00138", "Configuration Management",
                      szERR_Msg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }

      SetNameForView( vTZCMWULO, "TZCMWULO", vSubtask, zLEVEL_TASK );
   }

   SetCursorFirstEntityByAttr( vTZCMWULO, "User", "ZKey",
                               vTZCMWKUS, "User", "ZKey", "" );

   return( 0 );
} // zwTZCMCPLD_CheckOutItems

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_BuildWS_RefreshDetail
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_BuildWS_RefreshDetail( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW  vView;
   zVIEW  vTZCMWULO;
   zBOOL  bEnableUser = FALSE;
   zBOOL  bEnableMeta = FALSE;

   if ( GetViewByName( &vTZCMWULO, "TZCMWULO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   szEntityName[ 0 ] = 0;

   if ( OL_GetCurrentEntityName( vSubtask, "UserList", &vView,
                                 szEntityName ) >= 0 )
   {
      if ( zstrcmp( szEntityName, "User" ) == 0 )
      {
         bEnableUser = TRUE;
      }
      else
      {
         bEnableMeta = TRUE;
      }

      SetCtrlState( vSubtask, "txtMetaType", zCONTROL_STATUS_VISIBLE, bEnableMeta );
      SetCtrlState( vSubtask, "txtMetaTypeShow", zCONTROL_STATUS_VISIBLE, bEnableMeta );
      SetCtrlState( vSubtask, "txtDesc", zCONTROL_STATUS_VISIBLE, bEnableMeta );
      SetCtrlState( vSubtask, "txtDescShow", zCONTROL_STATUS_VISIBLE, bEnableMeta );
      SetCtrlState( vSubtask, "gbCheckOut", zCONTROL_STATUS_VISIBLE, bEnableMeta );

      SetCtrlState( vSubtask, "gbUserDesc", zCONTROL_STATUS_VISIBLE, bEnableUser );

      RefreshCtrl( vSubtask, "txtMetaType" );
      RefreshCtrl( vSubtask, "txtMetaTypeShow" );
      RefreshCtrl( vSubtask, "txtDesc" );
      RefreshCtrl( vSubtask, "txtDescShow" );
      RefreshCtrl( vSubtask, "txtUserInfos" );
      RefreshCtrl( vSubtask, "txtCheckoutDate" );
      RefreshCtrl( vSubtask, "txtWorkstation" );
      RefreshCtrl( vSubtask, "txtProject" );
      RefreshCtrl( vSubtask, "txtCPL" );
      RefreshCtrl( vSubtask, "txtCLPR" );
      RefreshCtrl( vSubtask, "edUserDesc" );
      RefreshCtrl( vSubtask, "edComments" );
   }

   return( 0 );
} // zwTZCMCPLD_BuildWS_RefreshDetail

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_RefreshFileList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_RefreshFileList( zVIEW vSubtask )
{

   zVIEW vWindow;

   // Refresh Window FileList (= modeless Subwindow)
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "FileList" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsPreBuild
//
// load CPLR Components
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsPreBuild( zVIEW vSubtask )
{
   zVIEW    vTZCMCPRO;
   zVIEW    vTZCMCPRO_New;

   if ( zwfnTZCMCPLD_LoadCPLR_ForViewName( vSubtask, "TZCMCPRO" ) < 0 )
      return( -1 );

   GetViewByName( &vTZCMCPRO, "TZCMCPRO", vSubtask, zLEVEL_TASK );
   SetNameForView( vTZCMCPRO, "TZCMCPRO_Source", vSubtask, zLEVEL_TASK );

   // Initialize the CPLR object to a single, empty entity.
   ActivateEmptyObjectInstance( &vTZCMCPRO_New, "TZCMCPRO", vSubtask, zSINGLE );

   CreateInstallEntity( vSubtask, vTZCMCPRO_New, "CPLR", zPOS_AFTER );
   SetNameForView( vTZCMCPRO_New, "TZCMCPRO_New", vSubtask, zLEVEL_TASK );

   return( 0 );
} // zwTZCMCPLD_SaveAsPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsSetCPLName
//
// if single or double click on Group List, then set CPL Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsSetCPLName( zVIEW vSubtask )
{
   zVIEW   vTZCMCPL;
   zCHAR   szCPL_Name[ 33 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPL" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szCPL_Name, zsizeof( szCPL_Name ), vTZCMCPL, "CPL", "Name" );
      SetCtrlText( vSubtask, "txtCPL", szCPL_Name );
      RefreshCtrl( vSubtask, "txtCPL" );
   }

   return( 0 );
} // zwTZCMCPLD_SaveAsSetCPLName

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_DeleteCPLWithoutCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_DeleteCPLWithoutCPLR( zVIEW vSubtask )
{
   zVIEW  vTZCMCPL;
   zVIEW  vTZCMCPL2;
   zSHORT nRC;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vTZCMCPL2, vTZCMCPL, 0 );

   nRC = SetCursorFirstEntity( vTZCMCPL2, "CPL", 0 );
   while ( nRC == zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity ( vTZCMCPL2, "CPLR" ) != zCURSOR_SET )
         DeleteEntity( vTZCMCPL2, "CPL", zREPOS_NONE );

      nRC = SetCursorNextEntity( vTZCMCPL2, "CPL", 0 );
   }

   DropView( vTZCMCPL2 );
   CommitObjectInstance( vTZCMCPL );

   return( 0 );
} // zwTZCMCPLD_DeleteCPLWithoutCPLR

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsCancel( zVIEW vSubtask )
{
   zVIEW    vTZCMCPRO_New;
   zVIEW    vTZCMCPRO_Source;

   if ( GetViewByName( &vTZCMCPRO_New, "TZCMCPRO_New", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMCPRO_New );

   if ( GetViewByName( &vTZCMCPRO_Source, "TZCMCPRO_Source", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMCPRO_Source );

   // Delete all CPL's without Subobject CPLR
   zwTZCMCPLD_DeleteCPLWithoutCPLR( vSubtask );

   return( 0 );
} // zwTZCMCPLD_SaveAsCancel

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsInNewCPL
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsInNewCPL( zVIEW vSubtask )
{
   zVIEW  vTZCMCPL;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   CreateInstallEntity( vSubtask, vTZCMCPL, "CPL", zPOS_AFTER );

   //Set Flag for Create new CPL
   SetAttributeFromString( vTZCMCPL, "CPL", "NewOrUpdate", "N" );

   return( 0 );
} // zwTZCMCPLD_SaveAsInNewCPL

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsCheckData
//
// check CPLR Data for save as
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsCheckData( zVIEW vSubtask )
{
   zVIEW  vTZCMCPRO_New;
   zVIEW  vTZCMCPL;
   zVIEW  vTZCMCPL2;

   zCHAR  szOutName[ 33 ];
   zCHAR  szNewName[ 33 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPRO_New, "TZCMCPRO_New", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vTZCMCPRO_New, "CPLR", "SaveAsName" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   // Check that a CPLR name has been entered.
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      MessageSend( vSubtask, "CM00146", "Configuration Management",
                   "CPLR name required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edCPLRName" );
      return( -1 );
   }

   //Check: CPL is set
   GetCtrlText( vSubtask, "txtCPL", szNewName, 33 );
   if ( zstrcmp( szNewName, "" ) == 0 )
   {
      MessageSend( vSubtask, "CM00147", "Configuration Management",
                   "CPL is not selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Check: new CPLR name is unique.
   CreateViewFromViewForTask( &vTZCMCPL2, vTZCMCPL, 0 );
   if ( SetCursorFirstEntityByString( vTZCMCPL2, "CPLR", "Name",
                                      szOutName, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "CM00148", "Configuration Management",
                   "CPLR Name already exists in CPL.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edCPLRName" );
      DropView( vTZCMCPL2 );
      return( -1 );
   }

   DropView( vTZCMCPL2 );

   return( 0 );
} // zwTZCMCPLD_SaveAsCheckData

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsAccessAuthority
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsAccessAuthority( zVIEW vTZCMCPRO_Source,
                                  zVIEW vTZCMCPRO_New,
                                  zVIEW vSubtask )
{
   zSHORT  nRC;
   zVIEW   vTZCMREPO;
   zCHAR   szMessage[250];
   zCHAR   szName[ 33 ];

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vTZCMCPRO_Source, "AccessAuthority", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateInstallEntity( vSubtask, vTZCMCPRO_New, "AccessAuthority", zPOS_AFTER );
      SetMatchingAttributesByName( vTZCMCPRO_New,    "AccessAuthority",
                                   vTZCMCPRO_Source, "AccessAuthority", zSET_NULL );

      //Include Subobject User
      if ( SetCursorFirstEntityByAttr( vTZCMREPO, "User", "ZKey",
                      vTZCMCPRO_Source, "User", "ZKey", 0 ) >= zCURSOR_SET )
      {
         //Set DIL Message Text
         strcpy_s( szMessage, zsizeof( szMessage ), "Copying Access Authority for User: " );
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMREPO, "User", "Name" );
         strcat_s( szMessage, zsizeof( szMessage ), szName );
         MB_SetMessage( vSubtask, 1, szMessage );

         IncludeSubobjectFromSubobject( vTZCMCPRO_New, "User",
                                        vTZCMREPO,     "User", zPOS_AFTER );
      }

      nRC = SetCursorNextEntity( vTZCMCPRO_Source, "AccessAuthority", "" );
   }

   return( 0 );
} // zwTZCMCPLD_SaveAsAccessAuthority

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsReusableCPLR
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsReusableCPLR( zVIEW vTZCMCPRO_Source,
                               zVIEW vTZCMCPRO_New,
                               zVIEW vSubtask )
{
   zSHORT  nRC;
   zVIEW   vTZCMCPL;
   zCHAR   szMessage[250];
   zCHAR   szName[ 33 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vTZCMCPRO_Source, "ReusableCPLR", "" );
   while ( nRC == zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vTZCMCPL, "CPLR", "ZKey",
                      vTZCMCPRO_Source, "ReusableCPLR", "ZKey",
                      "Installation" ) >= zCURSOR_SET )
      {
         //Set DIL Message Text
         strcpy_s( szMessage, zsizeof( szMessage ), "Copying Reusable CPLR: " );
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );
         strcat_s( szMessage, zsizeof( szMessage ), szName );
         MB_SetMessage( vSubtask, 1, szMessage );

         IncludeSubobjectFromSubobject( vTZCMCPRO_New, "ReusableCPLR",
                                        vTZCMCPL, "CPLR", zPOS_AFTER );
      }
      nRC = SetCursorNextEntity( vTZCMCPRO_Source, "ReusableCPLR", "" );
   }

   return( 0 );
} // zwTZCMCPLD_SaveAsReusableCPLR

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_FileErrorMessage
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwTZCMCPLD_FileErrorMessage( zVIEW  vSubtask,
                             zPCHAR szName,
                             zPCHAR szExtension )
{
   zCHAR    szMessage[250];

   strcpy_s( szMessage, zsizeof( szMessage ), "Error while copying File: " );
   strcat_s( szMessage, zsizeof( szMessage ), szName );
   strcat_s( szMessage, zsizeof( szMessage ), "." );
   strcat_s( szMessage, zsizeof( szMessage ), szExtension );
   strcat_s( szMessage, zsizeof( szMessage ), ".\n\n Save as aborted!" );
   MessageSend( vSubtask, "CM00152", "Configuration Management", szMessage,
                zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
} // zwTZCMCPLD_FileErrorMessage

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SearchFile
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
zwTZCMCPLD_SearchFile( zVIEW  vTZCMCPRO,
                       zPCHAR szName,
                       zPCHAR szExtension )
{
   zLONG    lType;
   zULONG   ulZKey;
   zBOOL    bFound;
   zSHORT   nRC;
   zVIEW    vTZCMCPRO_Copy;

   bFound  = FALSE;
   CreateViewFromViewForTask( &vTZCMCPRO_Copy, vTZCMCPRO, 0 );

   if ( CheckExistenceOfEntity( vTZCMCPRO_Copy, "Z_MetaDef" ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vTZCMCPRO_Copy, "Z_MetaDef", "Type" );
      if ( lType == zSOURCE_VOR_META )
      {
         GetIntegerFromAttribute(( zPLONG ) &ulZKey, vTZCMCPRO_Copy,
                                  "Z_MetaDef", "ZKey" );
         zltoxa( ulZKey, szName );
      }
   }

   if ( CheckExistenceOfEntity( vTZCMCPRO_Copy, "File" ) >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByString( vTZCMCPRO_Copy, "File",
                                          "Name", szName, 0 );
      while ( !bFound && nRC >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vTZCMCPRO_Copy, "File", "Extension",
                                        szExtension ) == 0 )
         {
            bFound = TRUE;
         }
         else
         {
            nRC = SetCursorNextEntityByString( vTZCMCPRO_Copy, "File",
                                               "Name", szName, 0 );
         }
      }
   }

   DropView( vTZCMCPRO_Copy );

   return( bFound );
} // zwTZCMCPLD_SearchFile

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadShadowFile
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwTZCMCPLD_LoadShadowFile( zVIEW  vSubtask,
                           zVIEW  vTZCMCPRO_New,
                           zPVIEW pvFile,
                           zPCHAR szName,
                           zPCHAR szExtension,
                           zPCHAR szTempName )
{
   zBOOL     bFound;
   zSHORT    nRC;
   zSHORT    nLth;
   zLONG     lType;
   zULONG    ulZKey;
   zLONG     lSeq;
   HFILE     hFileFrom;
   zCHAR     szBuffer[ 4097 ];

   *pvFile = NULL;

   bFound = zwTZCMCPLD_SearchFile( vTZCMCPRO_New, szName, szExtension );

   // If found, activate the existing file
   if ( bFound )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_New, "File", "ZKey" );
      nRC = oTZCMLPLO_ActivateFileOI( pvFile, vTZCMCPRO_New, ulZKey );

      if ( nRC >= 0 )
      {
         // If the file exists, delete the Buffer entries.
         nRC = SetCursorFirstEntity( *pvFile, "Buffer", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            DeleteEntity( *pvFile, "Buffer", zREPOS_NONE );
            nRC = SetCursorNextEntity( *pvFile, "Buffer", 0 );
         }
      }
      else
         bFound = FALSE;
   }

   // If it doesn't exist, create File Object
   if ( bFound == FALSE )
   {
      ActivateEmptyObjectInstance( pvFile, "TZCMFILO", vTZCMCPRO_New,
                                   zSINGLE | zLEVEL_APPLICATION );
      CreateInstallEntity( vSubtask, *pvFile, "File", zPOS_AFTER );
      GetIntegerFromAttribute( &lType, vTZCMCPRO_New, "Z_MetaDef", "Type" );

      if ( lType == zSOURCE_VOR_META )
      {
         GetIntegerFromAttribute(( zPLONG ) &ulZKey, vTZCMCPRO_New,
                                   "Z_MetaDef", "ZKey" );
         zltoxa( ulZKey, szName );
      }

      SetAttributeFromString( *pvFile, "File", "Name", szName );
      SetAttributeFromString( *pvFile, "File", "Extension", szExtension );
      IncludeSubobjectFromSubobject( vTZCMCPRO_New, "File",
                                     *pvFile,         "File", zPOS_AFTER );
   }

   // Create the CPLR object from the temp directory
   hFileFrom = (HFILE) SysOpenFile( vTZCMCPRO_New, szTempName, COREFILE_READ );
   if ( hFileFrom <= 0 )
      return( 0 );

   for ( lSeq = 0;; lSeq ++ )
   {
      nLth = SysReadFile( vTZCMCPRO_New, hFileFrom,
                          szBuffer, zsizeof( szBuffer ) - 1 );
      if ( nLth > 0 )
      {
         CreateEntity( *pvFile, "Buffer", zPOS_AFTER );
         SetAttributeFromInteger( *pvFile, "Buffer", "Seqno", lSeq );
         SetAttributeFromBlob( *pvFile, "Buffer", "Value", szBuffer, nLth );
      }
      else
         break;
   }

   SysCloseFile( vTZCMCPRO_New, hFileFrom, 0 );
   SysOpenFile( vTZCMCPRO_New, szTempName, COREFILE_DELETE );

   return( 0 );
} // zwTZCMCPLD_LoadShadowFile

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CopyShadowWithZKey
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_CopyShadowWithZKey( zVIEW vSubtask,
                               zVIEW vTZCMCPRO_New,
                               zVIEW vTZCMCPRO_Source )
{
   zCHAR    szName[ 33 ];
   zCHAR    szExtension[ 4 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szTempName[ 256 ];
   zLONG    lType;
   zPLONG   lplInteger;
   zPLONG   lplInteger1;
   zULONG   ulZKey;
   zSHORT   nRC;
   zSHORT   ReturnValue;
   zVIEW    vFile;
   zVIEW    vMetaShadow;

   ReturnValue = 0;

   GetIntegerFromAttribute( &lType, vTZCMCPRO_New, "Z_MetaDef", "Type" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO_New, "Z_MetaDef", "Name" );
   strcpy_s( szExtension, zsizeof( szExtension ), SRC_CMOD[ lType ].szOD_EXT + 1 );

   if ( lType == zSOURCE_VOR_META )
   {
      GetIntegerFromAttribute(( zPLONG ) &ulZKey, vTZCMCPRO_Source,
                              "Z_MetaDef", "ZKey" );
      zltoxa( ulZKey, szName );
   }

   nRC = LoadFileAndActivateMeta( vSubtask, &vMetaShadow, vTZCMCPRO_Source,
                                  szName, szExtension, lType, FALSE );

   // First get temp directory to store the file
   if ( nRC < 0 )
   {
      if ( nRC == -1 )
      {
         MessageSend( vTZCMCPRO_New, "CM00316", "Configuration Management",
                      "zAPPL_DIR_LOCAL Directory does not exists."
                      "\n\n Save as aborted!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         zwTZCMCPLD_FileErrorMessage( vTZCMCPRO_New, szName, szExtension );
      }
      return( -1 );
   }

   // First get temp directory to store the file
   zgGetZeidonToolsDir( vSubtask, szTempName, zAPPL_DIR_LOCAL );
   strcat_s( szTempName, zsizeof( szTempName ), szName );
   strcat_s( szTempName, zsizeof( szTempName ), ".tmp" );

   // Set ZKey from Z_MetaDef in Shadow file
   strcpy_s( szEntityName, zsizeof( szEntityName ), "" );
   zGetFirstEntityNameForView( vMetaShadow, szEntityName );
   GetAddrForAttribute( (zCOREMEM) &lplInteger, vMetaShadow,
                        szEntityName, "ZKey" );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_New,
                            "Z_MetaDef", "ZKey" );
   *lplInteger=ulZKey;

   if ( CheckExistenceOfEntity( vMetaShadow, "Z_MetaDef" ) >= zCURSOR_SET )
   {
      GetAddrForAttribute( (zCOREMEM) &lplInteger1, vMetaShadow,
                           "Z_MetaDef", "ZKey" );
      *lplInteger1=ulZKey;
   }

   // Commit Meta file
   CommitOI_ToFile( vMetaShadow, szTempName, zSINGLE );

   //Load Shadow file
   zwTZCMCPLD_LoadShadowFile( vSubtask, vTZCMCPRO_New, &vFile, szName,
                              szExtension, szTempName );

   // Commit the File object.
   if ( CommitObjectInstance( vFile ) < 0 )
   {
      zwTZCMCPLD_FileErrorMessage( vTZCMCPRO_New, szName, szExtension );
      ReturnValue = -1;
   }

   DropView( vMetaShadow );
   DropObjectInstance( vFile );

   return( ReturnValue );
} // zwTZCMCPLD_CopyShadowWithZKey

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsZ_MetaDef
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsZ_MetaDef( zVIEW vTZCMCPRO_Source,
                            zVIEW vTZCMCPRO_New,
                            zVIEW vSubtask )
{
   zSHORT  nRC;
   zCHAR   szMessage[ 250 ];
   zCHAR   szName[ 33 ];

   nRC = SetCursorFirstEntity( vTZCMCPRO_Source, "Z_MetaDef", "" );
   while ( nRC == zCURSOR_SET )
   {
      //Set DIL Message Text
      strcpy_s( szMessage, zsizeof( szMessage ), "Copying Component: " );
      GetVariableFromAttribute( &szMessage[ 19 ], 0, zTYPE_STRING, 125,
                  vTZCMCPRO_Source, "Z_MetaDef", "Type", "CM_Type", 0 );
      strcat_s( szMessage, zsizeof( szMessage ), " " );
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO_Source, "Z_MetaDef", "Name" );
      strcat_s( szMessage, zsizeof( szMessage ), szName );
      MB_SetMessage( vSubtask, 1, szMessage );

      //Create Z_MetaDef
      CreateInstallEntity( vSubtask, vTZCMCPRO_New, "Z_MetaDef", zPOS_AFTER );
      SetMatchingAttributesByName( vTZCMCPRO_New,    "Z_MetaDef",
                                   vTZCMCPRO_Source, "Z_MetaDef", zSET_NULL );

      //Create Shadow file for this Meta and set ZKey from Z_MetaDef in Shadow file
      if ( zwTZCMCPLD_CopyShadowWithZKey( vSubtask, vTZCMCPRO_New,
                                          vTZCMCPRO_Source ) < 0 )
      {
         zwTZCMCPLD_SaveAsAborted( vSubtask, vTZCMCPRO_New, vTZCMCPRO_Source );
         return( -1 );
      }

      nRC = SetCursorNextEntity( vTZCMCPRO_Source, "Z_MetaDef", "" );
   }

   return( 0 );
} // zwTZCMCPLD_SaveAsZ_MetaDef

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CreateAuditTrail
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_CreateAuditTrail( zVIEW vTZCMAUDO_Source,
                             zVIEW vTZCMAUDO_New,
                             zVIEW vTZCMREVO,
                             zVIEW vTZCMREPO,
                             zVIEW vTZCMCPRO_New,
                             zVIEW vTZCMCPRO_Source,
                             zVIEW vSubtask )
{
   zSHORT  nRC;
   zCHAR   szMessage[ 250 ];
   zCHAR   szName[ 33 ];

   CreateEntity( vTZCMAUDO_New, "AuditTrail", zPOS_AFTER );
   SetMatchingAttributesByName( vTZCMAUDO_New,    "AuditTrail",
                                vTZCMAUDO_Source, "AuditTrail", zSET_ALL );
   //Include CPLR Subobject
   IncludeSubobjectFromSubobject( vTZCMAUDO_New, "CPLR",
                                  vTZCMCPRO_New, "CPLR", zPOS_AFTER );

   //Include RepositoryClient Subobject
   if ( CheckExistenceOfEntity( vTZCMAUDO_Source, "RepositoryClient" )
        >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vTZCMAUDO_New, "RepositoryClient",
                                vTZCMAUDO_Source, "RepositoryClient", zPOS_AFTER );
   }

   //Include RevEvent Subobject
   if ( CheckExistenceOfEntity( vTZCMAUDO_Source, "RevEvent" )
        >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vTZCMREVO, "RevEvent", "Type",
           vTZCMAUDO_Source, "RevEvent", "Type", 0 ) >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vTZCMAUDO_New, "RevEvent",
                               vTZCMREVO, "RevEvent", zPOS_AFTER );
      }
   }

   //Include RevSubEvent Subobject
   if ( CheckExistenceOfEntity( vTZCMAUDO_Source, "RevSubEvent" )
        >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vTZCMREVO, "RevSubEvent", "Subtype",
           vTZCMAUDO_Source, "RevSubEvent", "Subtype", 0 ) >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vTZCMAUDO_New, "RevSubEvent",
                               vTZCMREVO, "RevSubEvent", zPOS_AFTER );
      }
   }

   //Create AuditTrailMeta Subobject
   nRC = SetCursorFirstEntity( vTZCMAUDO_Source, "AuditTrailMeta", "" );
   while ( nRC == zCURSOR_SET )
   {
      //Set DIL Message Text
      strcpy_s( szMessage, zsizeof( szMessage ), "Copying Audit Trail for Component: " );
      GetVariableFromAttribute( &szMessage[ 35 ], 0, zTYPE_STRING, 125,
                vTZCMAUDO_Source, "AuditTrailMeta", "MetaDefType", "CM_Type", 0 );
      strcat_s( szMessage, zsizeof( szMessage ), " " );
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMAUDO_Source,
                              "AuditTrailMeta", "MetaDefName" );
      strcat_s( szMessage, zsizeof( szMessage ), szName );
      MB_SetMessage( vSubtask, 1, szMessage );

      CreateEntity( vTZCMAUDO_New, "AuditTrailMeta", zPOS_AFTER );
      SetMatchingAttributesByName( vTZCMAUDO_New, "AuditTrailMeta",
                             vTZCMAUDO_Source, "AuditTrailMeta", zSET_ALL );

      //Include Subobject Z_MetaDef
      if ( CheckExistenceOfEntity( vTZCMAUDO_Source, "Z_MetaDef" ) >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByAttr( vTZCMCPRO_New, "Z_MetaDef", "ZKey",
              vTZCMAUDO_Source, "Z_MetaDef", "ZKey", 0 ) >= zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( vTZCMAUDO_New, "Z_MetaDef",
                                     vTZCMCPRO_New, "Z_MetaDef", zPOS_AFTER );
         }
      }
      nRC = SetCursorNextEntity( vTZCMAUDO_Source, "AuditTrailMeta", "" );
   }

   return( 0 );
} // zwTZCMCPLD_CreateAuditTrail

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadAuditTrailObject
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_LoadAuditTrailObject( zVIEW   vTZCMCPRO_Source,
                                 zPVIEW  vTZCMAUDO_Source,
                                 zVIEW   vSubtask )
{
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szDateTime[ 20 ];

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_Source,
                            "CPLR", "ZKey" );
   GetStringFromAttribute( szDateTime, zsizeof( szDateTime ), vTZCMCPRO_Source,
                           "AuditTrail", "TimeStamp" );

   if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA",
        vSubtask, zMULTIPLE ) < 0 )
   {
         MessageSend( vSubtask, "CM00149", "Configuration Management",
                      "Couldn't load Message Queue Object."
                      "\n\n Save as aborted!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
   }

   CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "AuditTrail" );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( vKZDBHQUA, "QualAttrib", "Value", ulZKey );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "AND" );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "AuditTrail" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "TimeStamp" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szDateTime );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );

   ActivateObjectInstance( vTZCMAUDO_Source, "TZCMAUDO", vSubtask,
                           vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( *vTZCMAUDO_Source, "TZCMAUDO_Source", vSubtask, zLEVEL_TASK );

   DropObjectInstance( vKZDBHQUA );

   return( 0 );
} // zwTZCMCPLD_LoadAuditTrailObject

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsAuditTrail
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsAuditTrail( zVIEW vTZCMCPRO_Source,
                             zVIEW vTZCMCPRO_New,
                             zVIEW vTZCMREPO,
                             zVIEW vSubtask )
{
   zVIEW         vTZCMAUDO_New;
   zVIEW         vTZCMAUDO_Source;
   zVIEW         vTZCMREVO;
   zSHORT        nRC;
   zCHAR         szMessage[ 350 ];
   zCHAR         szName[ 33 ];

   // Load ReviewType Object
   ActivateObjectInstance( &vTZCMREVO, "TZCMREVO",
                          vSubtask, 0, zMULTIPLE| zLEVEL_APPLICATION );

   // Create new AuditTrail Object
   nRC = SetCursorFirstEntity( vTZCMCPRO_Source, "AuditTrail", "" );
   while ( nRC == zCURSOR_SET )
   {
      if ( zwTZCMCPLD_LoadAuditTrailObject( vTZCMCPRO_Source,
                                            &vTZCMAUDO_Source,
                                            vSubtask ) < 0 )
      {
         DropObjectInstance( vTZCMREVO );
         zwTZCMCPLD_SaveAsAborted( vSubtask, vTZCMCPRO_New, vTZCMCPRO_Source );
         return( -1 );
      }

      ActivateEmptyObjectInstance ( &vTZCMAUDO_New, "TZCMAUDO", vSubtask, zSINGLE );
      SetNameForView( vTZCMAUDO_New, "TZCMAUDO_New", vSubtask, zLEVEL_TASK );

      zwTZCMCPLD_CreateAuditTrail( vTZCMAUDO_Source, vTZCMAUDO_New,
                                   vTZCMREVO, vTZCMREPO, vTZCMCPRO_New,
                                   vTZCMCPRO_Source, vSubtask );

      // Commit AuditTrail Object
      if ( CommitObjectInstance( vTZCMAUDO_New ) < 0 )
      {
         //Set DIL Message Text
         strcpy_s( szMessage, zsizeof( szMessage ), "Error while copying Audit Trail for Component: " );
         GetVariableFromAttribute( &szMessage[ 47 ], 0, zTYPE_STRING, 125,
                   vTZCMAUDO_New, "AuditTrailMeta", "MetaDefType", "CM_Type", 0 );
         strcat_s( szMessage, zsizeof( szMessage ), " " );
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMAUDO_New,
                                 "AuditTrailMeta", "MetaDefName" );
         strcat_s( szMessage, zsizeof( szMessage ), szName );
         strcat_s( szMessage, zsizeof( szMessage ), "\n\n Do you want to continue?" );
         if ( MessagePrompt( vSubtask, "CM00150",
                             "Save CPLR as:",
                             szMessage,
                             zBEEP,
                             zBUTTONS_YESNO, zRESPONSE_YES, 0 ) == zRESPONSE_NO )
         {
            DropObjectInstance( vTZCMREVO );
            DropObjectInstance( vTZCMAUDO_New );
            DropObjectInstance( vTZCMAUDO_Source );
            zwTZCMCPLD_SaveAsAborted( vSubtask, vTZCMCPRO_New, vTZCMCPRO_Source );
            return( -1 );
         }
      }

      DropObjectInstance( vTZCMAUDO_New );
      DropObjectInstance( vTZCMAUDO_Source );

      nRC = SetCursorNextEntity( vTZCMCPRO_Source, "AuditTrail", "" );
   }//endwhile ( nRC == zCURSOR_SET )

   DropObjectInstance( vTZCMREVO );
   return( 0 );
} // zwTZCMCPLD_SaveAsAuditTrail


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadFileObject
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_LoadFileObject( zVIEW   vTZCMCPRO_Source,
                           zPVIEW  vTZCMFILO_Source,
                           zVIEW   vSubtask )
{
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_Source, "File", "ZKey" );

   if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA",
        vSubtask, zMULTIPLE ) < 0 )
   {
      MessageSend( vSubtask, "CM00151", "Configuration Management",
                   "Couldn't load Message Queue Object."
                   "\n\n Save as aborted!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "File" );
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "File" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( vKZDBHQUA, "QualAttrib", "Value", ulZKey );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );

   ActivateObjectInstance( vTZCMFILO_Source, "TZCMFILO", vSubtask,
                           vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( *vTZCMFILO_Source, "TZCMFILO_Source", vSubtask, zLEVEL_TASK );

   DropObjectInstance( vKZDBHQUA );

   return( 0 );
} //zwTZCMCPLD_LoadFileObject

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsFile
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsFile( zVIEW vTZCMCPRO_Source,
                       zVIEW vTZCMCPRO_New,
                       zVIEW vSubtask )
{
   zVIEW         vTZCMFILO_New;
   zVIEW         vTZCMFILO_Source;
   zSHORT        nRC;
   zCHAR         szMessage[ 250 ];
   zCHAR         szName[ 33 ];
   zCHAR         szExtension[ 4 ];
   zBOOL         bFound;

   nRC = SetCursorFirstEntity( vTZCMCPRO_Source, "File", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO_Source, "File", "Name" );
      GetStringFromAttribute( szExtension, zsizeof( szExtension ), vTZCMCPRO_Source, "File", "Extension" );
      // Do not copy Registered Views
      if ( zstrcmp( szExtension, "PVR" ) == 0 )
         bFound = TRUE;
      else
         bFound = zwTZCMCPLD_SearchFile( vTZCMCPRO_New, szName, szExtension );

      // File Object does not exists, then copy it
      if ( !bFound )
      {
         zLONG lSeq;

         // Activate Source File Object
         if ( zwTZCMCPLD_LoadFileObject( vTZCMCPRO_Source,
                                         &vTZCMFILO_Source,
                                         vSubtask ) < 0 )
         {
            zwTZCMCPLD_SaveAsAborted( vSubtask, vTZCMCPRO_New, vTZCMCPRO_Source );
            return( -1 );
         }

         //Activate new Target File Object
         ActivateEmptyObjectInstance ( &vTZCMFILO_New, "TZCMFILO", vSubtask, zSINGLE );
         SetNameForView( vTZCMFILO_New, "TZCMFILO_New", vSubtask, zLEVEL_TASK );

         //Set DIL Message Text
         strcpy_s( szMessage, zsizeof( szMessage ), "Copying File: " );
         strcat_s( szMessage, zsizeof( szMessage ), szName );
         strcat_s( szMessage, zsizeof( szMessage ), "." );
         strcat_s( szMessage, zsizeof( szMessage ), szExtension );
         MB_SetMessage( vSubtask, 1, szMessage );

         //Copy File Object
         CreateInstallEntity( vSubtask, vTZCMFILO_New, "File", zPOS_AFTER );
         SetMatchingAttributesByName( vTZCMFILO_New,    "File",
                                      vTZCMFILO_Source, "File", zSET_NULL );
         IncludeSubobjectFromSubobject( vTZCMCPRO_New, "File",
                                        vTZCMFILO_New, "File", zPOS_AFTER );

         nRC = SetCursorFirstEntity( vTZCMFILO_Source, "Buffer", "" );
         lSeq = 0;
         while ( nRC == zCURSOR_SET )
         {
            CreateEntity( vTZCMFILO_New, "Buffer", zPOS_AFTER );
            SetAttributeFromAttribute( vTZCMFILO_New, "Buffer", "Value",
                                       vTZCMFILO_Source, "Buffer", "Value" );
            SetAttributeFromInteger( vTZCMFILO_New, "Buffer", "Seqno", lSeq );
            nRC = SetCursorNextEntity( vTZCMFILO_Source, "Buffer", "" );
            lSeq++;
         }

         // Commit File Object
         if ( CommitObjectInstance( vTZCMFILO_New ) < 0 )
         {
            zwTZCMCPLD_FileErrorMessage( vSubtask, szName, szExtension );
            DropObjectInstance( vTZCMFILO_New );
            DropObjectInstance( vTZCMFILO_Source );
            zwTZCMCPLD_SaveAsAborted( vSubtask, vTZCMCPRO_New, vTZCMCPRO_Source );
            return( -1 );
         }

         DropObjectInstance( vTZCMFILO_New );
         DropObjectInstance( vTZCMFILO_Source );
      } // endif ( !bFound )

      nRC = SetCursorNextEntity( vTZCMCPRO_Source, "File", "" );
   }

   return( 0 );
} // zwTZCMCPLD_SaveAsFile

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAS
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAS( zVIEW vSubtask,
                   zVIEW vTZCMCPRO_Source,
                   zVIEW vTZCMCPRO_New )
{
   zVIEW   vTZCMREPO;
   zCHAR   szName[ 33 ];
   zCHAR   szMessage[ 150 ];

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );

   //Copy Root Entity
   MB_SetMessage( vSubtask, 1, "Copy CPLR Directories." );
   SetAttributeFromAttribute( vTZCMCPRO_New,    "CPLR", "LastCheckInDate",
                              vTZCMCPRO_Source, "CPLR", "LastCheckInDate" );
   SetAttributeFromAttribute( vTZCMCPRO_New,    "CPLR", "MetaSrcDir",
                              vTZCMCPRO_Source, "CPLR", "MetaSrcDir" );
   SetAttributeFromAttribute( vTZCMCPRO_New,    "CPLR", "PgmSrcDir",
                              vTZCMCPRO_Source, "CPLR", "PgmSrcDir" );
   SetAttributeFromAttribute( vTZCMCPRO_New,    "CPLR", "ExecDir",
                              vTZCMCPRO_Source, "CPLR", "ExecDir" );

   //Copy Subobject Z_MetaDef
   if ( zwTZCMCPLD_SaveAsZ_MetaDef( vTZCMCPRO_Source, vTZCMCPRO_New, vSubtask ) < 0 )
   {
      return( -1 );
   }

   //Copy Subobject AccessAuthority
   zwTZCMCPLD_SaveAsAccessAuthority( vTZCMCPRO_Source, vTZCMCPRO_New, vSubtask );

   //Include Subobject ReusableCPLR
   zwTZCMCPLD_SaveAsReusableCPLR( vTZCMCPRO_Source, vTZCMCPRO_New, vSubtask );

   //Include Subobject File
   if ( zwTZCMCPLD_SaveAsFile( vTZCMCPRO_Source, vTZCMCPRO_New, vSubtask ) < 0 )
   {
      return( -1 );
   }

   //Copy Subobject AuditTrail
   if ( CheckExistenceOfEntity( vTZCMCPRO_Source, "AuditTrail" ) == zCURSOR_SET &&
        GetCtrlState( vSubtask, "cbAuditTrail", zCONTROL_STATUS_CHECKED ))
   {
      if ( zwTZCMCPLD_SaveAsAuditTrail( vTZCMCPRO_Source,
                                        vTZCMCPRO_New, vTZCMREPO,
                                        vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   //Commit vTZCMCPRO Object
   MB_SetMessage( vSubtask, 1, "Commiting new CPLR." );
   if ( CommitObjectInstance( vTZCMCPRO_New ) < 0 )
   {
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPRO_New, "CPLR", "Name" );
      strcpy_s( szMessage, zsizeof( szMessage ), "Couldn't commit CPLR " );
      strcat_s( szMessage, zsizeof( szMessage ), szName );
      strcat_s( szMessage, zsizeof( szMessage ), "\n\n Save as aborted!" );
      MessageSend( vSubtask, "CM00153", "Configuration Management",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      zwTZCMCPLD_SaveAsAborted( vSubtask, vTZCMCPRO_New, vTZCMCPRO_Source );
      return( -1 );
   }

   return( 0 );
} // zwTZCMCPLD_SaveAS

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetCPLR_CPL_Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetCPLR_CPL_Name( zVIEW vSubtask )
{
   zVIEW   vTZCMCPL;
   zVIEW   vTZCMCPLR;
   zCHAR   szName[ 33 ];
   zCHAR   szTitle[ 100 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );
   }
   else
   if ( GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPLR, "CPLR", "Name" );
   }
   else
   {
      strcpy_s( szName, zsizeof( szName ), "" );
   }

   strcpy_s( szTitle, zsizeof( szTitle ), "CPLR: " );
   strcat_s( szTitle, zsizeof( szTitle ), szName );
   strcat_s( szTitle, zsizeof( szTitle ), ", CPL: " );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPL", "Name" );
   strcat_s( szTitle, zsizeof( szTitle ), szName );
   SetWindowCaptionTitle( vSubtask, 0, szTitle );

   return( 0 );
} // zwTZCMCPLD_SetCPLR_CPL_Name

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsSetAuditTrail
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsSetAuditTrail( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "cbAuditTrail", zCONTROL_STATUS_CHECKED, 1 );

   return( 0 );
} // zwTZCMCPLD_SaveAsSetAuditTrail

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZCMCPLD_SaveAsCPLR( zVIEW vSubtask )
{
   zVIEW         vTZCMCPRO_New;
   zVIEW         vTZCMCPRO_Source;
   zVIEW         vTZCMCPL;
   zULONG        ulZKey;
   zCHAR         szOutName[ 33 ];
   zCHAR         szNewName[ 33 ];
   zCHAR         szMessage[ 150 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPRO_New, "TZCMCPRO_New", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPRO_Source, "TZCMCPRO_Source", vSubtask, zLEVEL_TASK );

   // Check data
   if ( zwTZCMCPLD_SaveAsCheckData( vSubtask ) < 0 )
      return( -1 );

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vTZCMCPRO_New, "CPLR", "SaveAsName" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   // Include new CPLR in TZCMREPO
   SetAttributeFromString( vTZCMCPRO_New, "CPLR", "Name", szOutName );
   IncludeSubobjectFromSubobject( vTZCMCPL,      "CPLR",
                                  vTZCMCPRO_New, "CPLR", zPOS_AFTER );

   // Delete all CPL's without Subobject CPLR
   zwTZCMCPLD_DeleteCPLWithoutCPLR( vSubtask );

   // Set Cursor in TZCMREPO
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_Source, "CPLR", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vTZCMCPL, "CPLR", "ZKey",
                             ulZKey, "Installation" ) >= zCURSOR_SET )
   {
      // Load the CPLR object and get its view.
      if ( zwTZCMCPLD_LoadCPRO_WithEnq( vSubtask ) < 0 )
      {
         DropObjectInstance( vTZCMCPRO_Source );
         DropObjectInstance( vTZCMCPRO_New );
         return( -1 );
      }
   }
   else  //Source CPLR not found in TZCMREPO
   {
      GetStringFromAttribute( szOutName, zsizeof( szOutName ), vTZCMCPRO_Source, "CPLR", "Name" );
      strcpy_s( szMessage, zsizeof( szMessage ), "Unable to Activate CPLR " );
      strcat_s( szMessage, zsizeof( szMessage ), szOutName );
      MessageSend( vSubtask, "CM00154", "Configuration Management",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      // Delete new created CPLR from TZCMREPO
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_New, "CPLR", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vTZCMCPL, "CPLR", "ZKey",
                             ulZKey, "Installation" ) >= zCURSOR_SET )
      {
         ExcludeEntity( vTZCMCPL, "CPLR", zREPOS_NONE );
      }

      if ( SetCursorFirstEntity( vTZCMCPL, "CPLR", "" ) != zCURSOR_SET )
         DeleteEntity( vTZCMCPL, "CPL", zREPOS_BEFORE );

      CommitObjectInstance( vTZCMCPL );

      DropObjectInstance( vTZCMCPRO_Source );
      DropObjectInstance( vTZCMCPRO_New );
      return( -1 );
   }

   GetViewByName( &vTZCMCPRO_Source, "TZCMCPRO", vSubtask, zLEVEL_TASK );
   SetNameForView( vTZCMCPRO_Source, "TZCMCPRO_Source", vSubtask, zLEVEL_TASK );

   if ( zwTZCMCPLD_SaveAS( vSubtask, vTZCMCPRO_Source, vTZCMCPRO_New ) >= 0 )
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO_Source, vSubtask, zCPLR_DEQUEUE );

   DropObjectInstance( vTZCMCPRO_Source );
   DropObjectInstance( vTZCMCPRO_New );

   return( 0 );
} // zwTZCMCPLD_SaveAsCPLR

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SaveAsAborted
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZCMCPLD_SaveAsAborted( zVIEW vSubtask,
                          zVIEW vTZCMCPRO_New,
                          zVIEW vTZCMCPRO_Source )
{
   zVIEW   vTZCMCPL;
   zVIEW   vTZCMFILO_New;
   zULONG  ulZKey;
   zSHORT  nRC;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   MB_SetMessage( vSubtask, 1, "Rollback of CPLR data." );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPRO_New, "CPLR", "ZKey" );

   oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO_Source, vSubtask, zCPLR_DEQUEUE );
   MB_SetMessage( vSubtask, 1, "Rollback of CPLR data." );

   // Set Equeue for new CPLR
   if ( SetCursorFirstEntityByInteger( vTZCMCPL, "CPLR", "ZKey",
                             ulZKey, "Installation" ) >= zCURSOR_SET )
   {
      // Test whether the CPLR ist not enqueued and enqueue it.
      if ( oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO_New, vSubtask,
                                   zCPLR_ENQUEUE_BY_SYSADM ) < 0 )
      {
         return( -1 );
      }
   }

   // Delete File Object
   for ( nRC = SetCursorFirstEntity( vTZCMCPRO_New, "File", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZCMCPRO_New, "File", 0 ))
   {
        if ( zwTZCMCPLD_LoadFileObject( vTZCMCPRO_New,
                                        &vTZCMFILO_New,
                                        vSubtask ) >= 0 )
        {
            DeleteEntity( vTZCMFILO_New, "File", zREPOS_NONE );
            CommitObjectInstance( vTZCMFILO_New );
            DropObjectInstance( vTZCMFILO_New );
        }

        ExcludeEntity( vTZCMCPRO_New, "File", zREPOS_NONE );
   }

   oTZCMCPRO_CPLR_Enqueue( vTZCMCPRO_New, vSubtask, zCPLR_DEQUEUE );
   MB_SetMessage( vSubtask, 1, "Rollback of CPLR data." );

   // Delete CPLR
   DeleteEntity( vTZCMCPRO_New, "CPLR", zREPOS_NONE );
   CommitObjectInstance( vTZCMCPRO_New );

   //Clean up vTZCMCPL
   if ( SetCursorFirstEntityByInteger( vTZCMCPL, "CPLR", "ZKey",
                             ulZKey, "Installation" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vTZCMCPL, "CPLR", zREPOS_NONE );
   }

   if ( SetCursorFirstEntity( vTZCMCPL, "CPLR", "" ) != zCURSOR_SET )
      DeleteEntity( vTZCMCPL, "CPL", zREPOS_BEFORE );

   CommitObjectInstance( vTZCMCPL );

   return( 0 );

} // zwTZCMCPLD_SaveAsAborted

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_PasswordPostbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_PasswordPostbuild( zVIEW vSubtask )
{
   ED_LimitTextLength( vSubtask, "edVerification", 8 );

   return( 0 );
} // zwTZCMCPLD_PasswordPostbuild


static zSHORT
zwfnTZCMCPLD_CheckPassword( zVIEW vSubtask, zPCHAR szPassword )
{
  zCHAR szVerification[9];

   GetCtrlText( vSubtask, "edVerification", szVerification, 9 );

   if ( zstrcmp( szPassword, szVerification ) != 0 )
   {
      MessageSend( vSubtask, "CM00154", "Configuration Management",
                   "Password and Verification Password does not match.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "edVerification" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_Password_OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_Password_OK( zVIEW vSubtask )
{
   zVIEW vTZCMREPO;
   zCHAR szPassword[ 9 ];

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szPassword, zsizeof( szPassword ), vTZCMREPO, "Installation", "Password" );

   if ( zwfnTZCMCPLD_CheckPassword( vSubtask, szPassword ) < 0 )
      return( -1 );

   return( 0 );
} // zwTZCMCPLD_Password_OK

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetMinCtrlPos_Users
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetMinCtrlPos_Users( zVIEW vSubtask )
{
   zLONG  lSize_X;
   zLONG  lCtrlSize_X;
   zLONG  lCtrlPos_X;
   zLONG  lTemp;

   GetCtrlSize( vSubtask, "tgUser", &lSize_X, &lTemp, TRUE );

   // Set new position for Buttons
   GetCtrlSize( vSubtask, "pbOK", &lCtrlSize_X, &lTemp, TRUE );
   lCtrlPos_X = lSize_X - lCtrlSize_X + 3;

   if ( lCtrlPos_X < 180 )
      lCtrlPos_X = 180;

   SetCtrlPosition( vSubtask, "pbOK", lCtrlPos_X, 5, TRUE );
   SetCtrlPosition( vSubtask, "pbCancel", lCtrlPos_X, 20, TRUE );

   return( 0 );
} // zwTZCMCPLD_SetMinCtrlPos_Users


static zSHORT
zwfnTZCMCPLD_LoadMetas( zVIEW vSubtask, zVIEW vReusableCPLR )
{
   zVIEW   vReusableComponents;
   zSHORT  nRC;
   zULONG  ulZKey;
   zLONG   lTypeOld;
   zLONG   lCount;
   zLONG   lCurrent;
   zCHAR   szName[ 33 ];
   zCHAR   szMetaName[ 33 ];
   zCHAR   szMsg[ 200 ];
   zCHAR   szTemp[ 200 ];

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vReusableCPLR, "CPLR", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vReusableCPLR, "CPLR", "Name" );

   strcpy_s( szMsg, zsizeof( szMsg ), "Activating Reusable CPLR " );
   strcat_s( szMsg, zsizeof( szMsg ), szName );
   MB_SetMessage( vSubtask, 0, szMsg );

   // Set up qualification object and activate CPLR
   if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vReusableComponents, "TZCMCOMP",
                               ulZKey, szName, zSINGLE ) < 0 )
   {
      return( -1 );
   }

   MB_SetMessage( vSubtask, 0, "Loading CPLR Components." );
   OrderEntityForView( vReusableComponents, "Z_MetaDef", "Type A Name A" );

   lTypeOld = -1;
   lCurrent = 0;
   lCount = CountEntitiesForView( vReusableComponents, "Z_MetaDef" );

   for ( nRC = SetCursorFirstEntity( vReusableComponents, "Z_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReusableComponents, "Z_MetaDef", "" ) )
   {
      if ( CompareAttributeToInteger( vReusableComponents, "Z_MetaDef", "Type",
                                      lTypeOld ) != 0 )
      {
         CreateEntity( vReusableCPLR, "Z_MetaType", zPOS_AFTER );
         SetAttributeFromAttribute( vReusableCPLR, "Z_MetaType", "Type",
                                    vReusableComponents, "Z_MetaDef", "Type" );
      }

      lCurrent++;
      zltoa( lCurrent, szTemp + 110, zsizeof( szTemp ) + 110 );
      strcpy_s( szTemp, zsizeof( szTemp ), szTemp + 110 );
      strcat_s( szTemp, zsizeof( szTemp ), " of " );
      zltoa( lCount, szTemp + 110, zsizeof( szTemp ) - 110 );
      strcat_s( szTemp, zsizeof( szTemp ), szTemp + 110 );
      MB_SetMessage( vSubtask, 2, szTemp );

      strcpy_s( szMsg, zsizeof( szMsg ), "Loading " );
      GetStringFromAttributeByContext( szMsg + zstrlen( szMsg ),
                                       vReusableComponents, "Z_MetaDef", "Type",
                                       "CM_Type", 50 );
      strcat_s( szMsg, zsizeof( szMsg ), ": " );
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vReusableComponents, "Z_MetaDef", "Name" );
      strcat_s( szMsg, zsizeof( szMsg ), szMetaName );
      MB_SetMessage( vSubtask, 0, szMsg );

      CreateEntity( vReusableCPLR, "Z_MetaDef", zPOS_AFTER );
      SetMatchingAttributesByName( vReusableCPLR, "Z_MetaDef",
                                   vReusableComponents, "Z_MetaDef", zSET_ALL );

      GetIntegerFromAttribute( &lTypeOld, vReusableComponents, "Z_MetaDef", "Type" );
   }

   DropObjectInstance( vReusableComponents );
   return( 0 );
}


static zSHORT
zwfnTZCMCPLD_LoadCompForReusable( zVIEW vSubtask, zPCHAR szViewName, zBOOL bShowMetas )
{
   zVIEW   vReusableCPLR;
   zCHAR   szName[ 33 ];
   zCHAR   szMsg[ 200 ];

   if ( GetViewByName( &vReusableCPLR, szViewName, vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) >= zCURSOR_SET )
   {
      if ( bShowMetas )
         OL_ExpandEntity( vSubtask, "ResuableCPLR", "CPLR", zEXPAND );

      return( 0 );
   }

   if ( zwfnTZCMCPLD_LoadMetas( vSubtask, vReusableCPLR ) < 0 )
      return( -1 );

   if ( bShowMetas )
   {
      if ( CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) < zCURSOR_SET )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vReusableCPLR, "CPLR", "Name" );
         strcpy_s( szMsg, zsizeof( szMsg ), "CPLR '" );
         strcat_s( szMsg, zsizeof( szMsg ), szName );
         strcat_s( szMsg, zsizeof( szMsg ), "' has no Components." );
         MessageSend( vSubtask, "CM00101", "Configuration Management",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         SetCursorFirstEntity( vReusableCPLR, "Z_MetaType", "" );
         RefreshCtrl( vSubtask, "ResuableCPLR" );
         OL_ExpandEntity( vSubtask, "ResuableCPLR", "CPLR", zEXPAND );
      }
   }

   MB_SetMessage( vSubtask, 0, "" );
   MB_SetMessage( vSubtask, 2, "" );

   return( 0 );
} // zwTZCMCPLD_LoadCompForReusable


static zSHORT
zwfnTZCMCPLD_RemoveCompForReusable( zVIEW vSubtask, zPCHAR szViewName )
{
   zVIEW  vReusableCPLR;
   zSHORT nRC;

   if ( GetViewByName( &vReusableCPLR, szViewName, vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET ||
        CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   MB_SetMessage( vSubtask, 0, "Removing Components from List." );

   for ( nRC = SetCursorFirstEntity( vReusableCPLR, "Z_MetaType", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReusableCPLR, "Z_MetaType", "" ) )
   {
      DeleteEntity( vReusableCPLR, "Z_MetaType", zREPOS_NONE );
   }

   RefreshCtrl( vSubtask, "ResuableCPLR" );
   MB_SetMessage( vSubtask, 0, "" );

   return( 0 );
} // zwTZCMCPLD_RemoveCompForReusable


static zSHORT
zwfnTZCMCPLD_ReusableSetButtonText( zVIEW vSubtask, zPCHAR szViewName )
{
   zVIEW vReusableCPLR;
   zCHAR szButtonText[ 20 ];

   if ( GetViewByName( &vReusableCPLR, szViewName, vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) >= zCURSOR_SET )
      strcpy_s( szButtonText, zsizeof( szButtonText ), "Re&move Comp." );
   else
      strcpy_s( szButtonText, zsizeof( szButtonText ), "&Components" );

   SetCtrlText( vSubtask, "pbComponents", szButtonText );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ReusableSetButtonText
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ReusableSetButtonText( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_ReusableSetButtonText( vSubtask, "ReusableCPLR" ) );

} // zwTZCMCPLD_ReusableSetButtonText

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_AddReusableSetBtnText
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_AddReusableSetBtnText( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_ReusableSetButtonText( vSubtask, "TZCMRCRO_Select" ) );

} // zwTZCMCPLD_AddReusableSetBtnText

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadOperForReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadOperForReusable( zVIEW vSubtask )
{
   zVIEW vReusableCPLR;

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) >= zCURSOR_SET )
      zwfnTZCMCPLD_RemoveCompForReusable( vSubtask, "ReusableCPLR" );
   else
      zwfnTZCMCPLD_LoadCompForReusable( vSubtask, "ReusableCPLR", TRUE );

   zwTZCMCPLD_ReusableSetButtonText( vSubtask );

   return( 0 );
} // zwTZCMCPLD_LoadOperForReusable


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadOperForAddReus
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadOperForAddReus( zVIEW vSubtask )
{
   zVIEW vReusableCPLR;

   if ( GetViewByName( &vReusableCPLR, "TZCMRCRO_Select", vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) >= zCURSOR_SET )
      zwfnTZCMCPLD_RemoveCompForReusable( vSubtask, "TZCMRCRO_Select" );
   else
      zwfnTZCMCPLD_LoadCompForReusable( vSubtask, "TZCMRCRO_Select", TRUE );

   zwTZCMCPLD_AddReusableSetBtnText( vSubtask );

   return( 0 );
} // zwTZCMCPLD_LoadOperForAddReus

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetButtonPos
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetButtonPos( zVIEW vSubtask )
{
   zLONG  lSize_X;
   zLONG  lSize_Y;
   zLONG  lCtrlPos_X;

   // Set new position for Buttons
   GetCtrlSize( vSubtask, "ResuableCPLR", &lSize_X, &lSize_Y, TRUE );
   lCtrlPos_X = lSize_X + 10;

   SetCtrlPosition( vSubtask, "pbOK", lCtrlPos_X, 4, TRUE );
   SetCtrlPosition( vSubtask, "pbCancel", lCtrlPos_X, 19, TRUE );
   SetCtrlPosition( vSubtask, "pbAdd", lCtrlPos_X, 38, TRUE );
   SetCtrlPosition( vSubtask, "pbRemove", lCtrlPos_X, 53, TRUE );
   SetCtrlPosition( vSubtask, "pbComponents", lCtrlPos_X, 71, TRUE );
   SetCtrlPosition( vSubtask, "pbErrorList", lCtrlPos_X, 87, TRUE );

   return( 0 );
} // zwTZCMCPLD_SetButtonPos

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetButtonPos_Select
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetButtonPos_Select( zVIEW vSubtask )
{
   zLONG  lSize_X;
   zLONG  lSize_Y;
   zLONG  lCtrlPos_X;

   // Set new position for Buttons
   GetCtrlSize( vSubtask, "ResuableCPLR", &lSize_X, &lSize_Y, TRUE );
   lCtrlPos_X = lSize_X + 10;

   SetCtrlPosition( vSubtask, "pbOK", lCtrlPos_X, 4, TRUE );
   SetCtrlPosition( vSubtask, "pbCancel", lCtrlPos_X, 19, TRUE );
   SetCtrlPosition( vSubtask, "pbComponents", lCtrlPos_X, 38, TRUE );
   SetCtrlPosition( vSubtask, "pbErrorList", lCtrlPos_X, 53, TRUE );

   return( 0 );
} // zwTZCMCPLD_SetButtonPos_Select

static zSHORT
zwfnTZCMCPLD_ExpandReusable( zVIEW vSubtask, zPCHAR szViewName )
{
   zVIEW  vReusableCPLR;
   zSHORT nRC;

   if ( GetViewByName( &vReusableCPLR, szViewName, vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   for ( nRC = SetCursorFirstEntity( vReusableCPLR, "Z_MetaType", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReusableCPLR, "Z_MetaType", "" ) )
   {
      OL_ExpandEntity( vSubtask, "ResuableCPLR", "Z_MetaType", zEXPAND );
   }

   OL_ExpandEntity( vSubtask, "ResuableCPLR", "CPLR", zEXPAND );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ExpandReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ExpandReusable( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_ExpandReusable( vSubtask, "ReusableCPLR" ) );

} // zwTZCMCPLD_ExpandReusable

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ExpandAddReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ExpandAddReusable( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_ExpandReusable( vSubtask, "TZCMRCRO_Select" ) );

} // zwTZCMCPLD_ExpandAddReusable

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ExpandAllReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ExpandAllReusable( zVIEW vSubtask )
{
   OL_SetCtrlState( vSubtask, "ResuableCPLR", zCONTROL_STATUS_EXPANDALL, TRUE );
   return( 0 );
} // zwTZCMCPLD_ExpandAllReusable

static zSHORT
zwfnTZCMCPLD_CollapseReusable( zVIEW vSubtask, zPCHAR szViewName )
{
   zVIEW  vReusableCPLR;
   zSHORT nRC;

   if ( GetViewByName( &vReusableCPLR, szViewName, vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   for ( nRC = SetCursorFirstEntity( vReusableCPLR, "Z_MetaType", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReusableCPLR, "Z_MetaType", "" ) )
   {
      OL_ExpandEntity( vSubtask, "ResuableCPLR", "Z_MetaType", zCOLLAPSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CollapseReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CollapseReusable( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_CollapseReusable( vSubtask, "ReusableCPLR" ) );

} // zwTZCMCPLD_CollapseReusable

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CollapseAddReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CollapseAddReusable( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_CollapseReusable( vSubtask, "TZCMRCRO_Select" ) );

} // zwTZCMCPLD_CollapseAddReusable

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CollapseAllReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CollapseAllReusable( zVIEW vSubtask )
{
   OL_ExpandEntity( vSubtask, "ResuableCPLR", "Z_MetaType", zCOLLAPSEALL );

   return( 0 );
} // zwTZCMCPLD_CollapseAllReusable

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ExpandAllCPLRs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ExpandAllCPLRs( zVIEW vSubtask )
{
   OL_ExpandEntity( vSubtask, "ResuableCPLR", "CPLR", zEXPANDALL );

   return( 0 );
} // zwTZCMCPLD_ExpandAllCPLRs

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CollapseAllCPLRs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CollapseAllCPLRs( zVIEW vSubtask )
{

   OL_ExpandEntity( vSubtask, "ResuableCPLR", "CPLR", zCOLLAPSEALL );

   return( 0 );
} // zwTZCMCPLD_CollapseAllCPLRs


static zSHORT
zwfnTZCMCPLD_TrollReusableMetasForPopup( zVIEW vTZCMRCRO )
{
   zSHORT   nRC;

   if ( SetCursorFirstEntity( vTZCMRCRO, "Z_MetaType", "" ) == zCURSOR_SET )
      return( 1 );

   if ( SetViewToSubobject( vTZCMRCRO, "ReusableCPLR" ) == zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "" );
      if ( nRC != zCURSOR_SET )
      {
         ResetViewFromSubobject( vTZCMRCRO );
         return( 0 );
      }

      while ( nRC == zCURSOR_SET )
      {
         if ( zwfnTZCMCPLD_TrollReusableMetasForPopup( vTZCMRCRO ) == 1 )
            return( 1 );

         nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "" );
      }
   }

   ResetViewFromSubobject( vTZCMRCRO );

   return( 0 );
}


static zSHORT
zwfnTZCMCPLD_LoadPopupForReusable( zVIEW vSubtask, zPCHAR szViewName )
{
   zVIEW  vReusableCPLR;
   zVIEW  vTZCMRCRO;
   zSHORT nRC;
   zBOOL  bFoudMeta = FALSE;
   zBOOL  bFoudCurrentMeta = FALSE;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vReusableCPLR, szViewName, vSubtask, zLEVEL_TASK ) < 0 ||
        CheckExistenceOfEntity( vReusableCPLR, "CPLR" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vReusableCPLR, "Z_MetaType" ) >= zCURSOR_SET )
      bFoudCurrentMeta = TRUE;

   CreateViewFromViewForTask( &vTZCMRCRO, vReusableCPLR, 0 );

   while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );

   nRC = SetCursorFirstEntity( vTZCMRCRO, "CPLR", "Installation" );
   while ( nRC >= zCURSOR_SET && !bFoudMeta )
   {
      if ( SetCursorFirstEntity( vTZCMRCRO, "Z_MetaType", "" ) == zCURSOR_SET )
         bFoudMeta = TRUE;

      nRC = SetCursorFirstEntity( vTZCMRCRO, "ReusableCPLR", "" );
      while ( nRC == zCURSOR_SET && !bFoudMeta )
      {
         if ( zwfnTZCMCPLD_TrollReusableMetasForPopup( vTZCMRCRO ) == 1 )
            bFoudMeta = TRUE;
         else
            nRC = SetCursorNextEntity( vTZCMRCRO, "ReusableCPLR", "" );
      }
      while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );
      nRC = SetCursorNextEntity( vTZCMRCRO, "CPLR", "Installation" );
   }

   EnableAction( vSubtask, "Expand", bFoudCurrentMeta );
   EnableAction( vSubtask, "Collapse", bFoudCurrentMeta );
   EnableAction( vSubtask, "ExpandAll", bFoudMeta );
   EnableAction( vSubtask, "CollapseAll", bFoudMeta );

   CreateTrackingPopupMenu( vSubtask, "ReusablePopup", pt.x, pt.y,
                            FALSE, FALSE );


   DropView( vTZCMRCRO );
   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadPopupForReusable
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadPopupForReusable( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_LoadPopupForReusable( vSubtask, "ReusableCPLR" ) );

} // zwTZCMCPLD_LoadPopupForReusable

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadPopupForAddReus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadPopupForAddReus( zVIEW vSubtask )
{
   return( zwfnTZCMCPLD_LoadPopupForReusable( vSubtask, "TZCMRCRO_Select" ) );

} // zwTZCMCPLD_LoadPopupForAddReus

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_DropComponentViews
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_DropComponentViews( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   if ( GetViewByName( &vMetaList, "CPLR_MetaList", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vMetaList );

   if ( GetViewByName( &vMetaList, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vMetaList );

   if ( GetViewByName( &vMetaList, "TZCMCUSR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vMetaList );

   if ( GetViewByName( &vMetaList, "TZCMFILE", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vMetaList );

   return( 0 );
} // zwTZCMCPLD_DropComponentViews

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetOkButtonText
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetOkButtonText( zVIEW vSubtask )
{
   zVIEW  vWindow;

   // If the window was opened from the window <CPLR_Maintenance>, then set
   // Button Text <OK>, otherwise set Button Text <Save>. in this case we save
   // directly tzcmcpro Object.
   // if opended window <CPLR_Maintenance>, then we have the change to save or
   // cancel
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) >= 0 )
      SetCtrlText( vSubtask, "pbOK", "OK" );
   else
   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
      SetCtrlText( vSubtask, "pbOK", "OK" );
   else
      SetCtrlText( vSubtask, "pbOK", "Save" );

   zwTZCMCPLD_SetCPLR_CPL_Name( vSubtask );

   return( 0 );
} // zwTZCMCPLD_SetOkButtonText

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_GrayCPLR_User
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_GrayCPLR_User( zVIEW vSubtask )
{
   zBOOL bLockColumn = TRUE;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      bLockColumn = FALSE;
   }

   TG_LockColumn( vSubtask, "tgUser", 0, bLockColumn );
   TG_LockColumn( vSubtask, "tgUser", 1, bLockColumn );

   zwTZCMCPLD_SetOkButtonText( vSubtask );

   return( 0 );
} // zwTZCMCPLD_GrayCPLR_User

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_GrayReusableCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_GrayReusableCPLR( zVIEW vSubtask )
{
   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      EnableAction( vSubtask, "XferToReusableCPLRs", FALSE );
      EnableAction( vSubtask, "RemoveReusableCPLR", FALSE );
   }

   zwTZCMCPLD_SetOkButtonText( vSubtask );

   return( 0 );
} // zwTZCMCPLD_GrayReusableCPLR

static void
zwfnTZCMCPLD_WriteErrorListTitle( zVIEW vReusError, zLONG  FileHandle )
{
   zCHAR  szReusedBy[ 2 ];
   zCHAR  szMsg[ 500 ];
   zCHAR  szTemp[ 255 ];

   GetStringFromAttribute( szReusedBy, zsizeof( szReusedBy ), vReusError, "ErrorList", "CheckReusedBy" );

   // write Tool and DateTime
   strcpy_s( szMsg, zsizeof( szMsg ), "Zeidon: Error List from " );
   GetStringFromAttributeByContext( szTemp, vReusError, "ErrorList",
                                    "DateTime", "DD.MM.YYYY HH:MM:SS", 20 );
   strcat_s( szMsg, zsizeof( szMsg ), szTemp );
   SysWriteLine( vReusError, FileHandle, szMsg );
   SysWriteLine( vReusError, FileHandle, "" );

   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusError, "ErrorList", "ReusableCPLR_CPL_Name" );
   if ( zstrcmp( szReusedBy, "Y" ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), " Check reusing CPLR '" );
      strcat_s( szMsg, zsizeof( szMsg ), szTemp );
      strcat_s( szMsg, zsizeof( szMsg ), "'" );
   }
   else
   {
      strcpy_s( szMsg, zsizeof( szMsg ), " Add Reusable CPLR '" );
      strcat_s( szMsg, zsizeof( szMsg ), szTemp );
      strcat_s( szMsg, zsizeof( szMsg ), "' to CPLR '" );
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusError, "CPLR", "Name" );
      strcat_s( szMsg, zsizeof( szMsg ), szTemp );
      strcat_s( szMsg, zsizeof( szMsg ), "'" );
   }

   SysWriteLine( vReusError, FileHandle, szMsg );
   SysWriteLine( vReusError, FileHandle, "" );
   SysWriteLine( vReusError, FileHandle, "" );

   strcpy_s( szMsg, zsizeof( szMsg ), "Meta Name:      " );
   strcat_s( szMsg, zsizeof( szMsg ), "Meta Type:   " );
   strcat_s( szMsg, zsizeof( szMsg ), "CPLR:           " );
   if ( zstrcmp( szReusedBy, "Y" ) == 0 )
      strcat_s( szMsg, zsizeof( szMsg ), "CPLR reusing the current one: " );
   else
      strcat_s( szMsg, zsizeof( szMsg ), "CPLR to be reused: " );

   SysWriteLine( vReusError, FileHandle, szMsg );

   strcpy_s( szMsg, zsizeof( szMsg ), "___________________________________" );
   strcat_s( szMsg, zsizeof( szMsg ), "________________________________" );
   strcat_s( szMsg, zsizeof( szMsg ), "___________________________________" );
   strcat_s( szMsg, zsizeof( szMsg ), "__________________________________" );
   SysWriteLine( vReusError, FileHandle, szMsg );
   SysWriteLine( vReusError, FileHandle, "" );
}

static void
zwfnTZCMCPLD_WriteErrorListToFile( zVIEW vReusError, zLONG  FileHandle )
{
   zSHORT nRC;
   zCHAR  szMsg[ 500 ];
   zCHAR  szTemp[ 255 ];
   zLONG  i;

   zwfnTZCMCPLD_WriteErrorListTitle( vReusError, FileHandle );

   // write Error Message
   for ( nRC = SetCursorFirstEntity( vReusError, "ErrorMsg", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReusError, "ErrorMsg", "" ) )
   {
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusError, "ErrorMsg", "MetaName" );
      strcpy_s( szMsg, zsizeof( szMsg ), szTemp );
      for ( i = zstrlen( szTemp ); i < 33; i++ )
         strcat_s( szMsg, zsizeof( szMsg ), " " );

      strcat_s( szMsg, zsizeof( szMsg ), " " );
      GetStringFromAttributeByContext( szTemp, vReusError, "ErrorMsg", "MetaType",
                                       "CM_Type", 30 );
      if ( zstrcmp( szTemp, "" ) != 0 )
      {
         strcat_s( szMsg, zsizeof( szMsg ), szTemp );
         for ( i = zstrlen( szTemp ); i < 30; i++ )
            strcat_s( szMsg, zsizeof( szMsg ), " " );
      }

      strcat_s( szMsg, zsizeof( szMsg ), " " );
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusError, "ErrorMsg", "CurrentCPLR" );
      if ( zstrcmp( szTemp, "" ) != 0 )
      {
         strcat_s( szMsg, zsizeof( szMsg ), szTemp );
         for ( i = zstrlen( szTemp ); i < 33; i++ )
            strcat_s( szMsg, zsizeof( szMsg ), " " );
      }

      strcat_s( szMsg, zsizeof( szMsg ), " " );
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vReusError, "ErrorMsg", "CPLR_ToBeReused" );
      if ( zstrcmp( szTemp, "" ) != 0 )
      {
         strcat_s( szMsg, zsizeof( szMsg ), szTemp );
         for ( i = zstrlen( szTemp ); i < 33; i++ )
            strcat_s( szMsg, zsizeof( szMsg ), " " );
      }

      SysWriteLine( vReusError, FileHandle, szMsg );
   }

   SysWriteLine( vReusError, FileHandle, "" );
   SysWriteLine( vReusError, FileHandle, "End of Error List" );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ErrorListSaveToFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ErrorListSaveToFile( zVIEW vSubtask )
{
   zVIEW  vReusError;
   zULONG ulZFlags = 0;
   zLONG  FileHandle;
   zCHAR  szMsg[ 300 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];

   if ( GetViewByName( &vReusError, "TZCMREUS", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   szFileName[ 0 ] = 0;
   ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT | zOFN_SAVEAS | zOFN_HIDEREADONLY;

   if ( OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ),
                               "Text Files (*.txt)|*.txt|", "txt", ulZFlags ) == 1 )
   {
      FileHandle = SysOpenFile( vReusError, szFileName, COREFILE_WRITE );

      if ( FileHandle == -1 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Unable to open " );
         strcat_s( szMsg, zsizeof( szMsg ), szFileName );
         MessageSend( vSubtask, "FEH001", "Configuration Management",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         return( -1 );
      }

      zwfnTZCMCPLD_WriteErrorListToFile( vReusError, FileHandle );
      SysCloseFile( vReusError, FileHandle, 0 );
   }

   return( 0 );
} // zwTZCMCPLD_ErrorListSaveToFile

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetCPL_Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetCPL_Name( zVIEW vSubtask )
{
   zVIEW   vTZCMCPL;
   zBOOL   bSysadm = FALSE;
   zCHAR   szName[ 33 ];
   zCHAR   szControlText[ 50 ];

   if ( GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPL", "Name" );
   strcpy_s( szControlText, zsizeof( szControlText ), " CPL: " );
   strcat_s( szControlText, zsizeof( szControlText ), szName );
   SetCtrlText( vSubtask, "txtCPL", szControlText );

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      bSysadm = TRUE;
   }
   else
   {
      SetCtrlText( vSubtask, "OK", "OK" );
   }

   SetCtrlState( vSubtask, "Name", zCONTROL_STATUS_ENABLED, bSysadm );
   SetCtrlState( vSubtask, "Desc", zCONTROL_STATUS_ENABLED, bSysadm );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, bSysadm );

   return( 0 );
} // zwTZCMCPLD_SetCPL_Name

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadCPLR_Users
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadCPLR_Users( zVIEW vSubtask )
{
   zVIEW   vTZCMCPL;
   zVIEW   vTZCMCUSR;
   zVIEW   vParentWindow;
   zULONG  ulZKey;
   zULONG  ulZKeyCPLR;
   zCHAR   szName[ 33 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );

   zwTZCMCPLD_SetCPL_Name( vSubtask );

   if ( GetViewByName( &vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKeyCPLR, vTZCMCUSR, "CPLR", "ZKey" );
      if ( ulZKey == ulZKeyCPLR )
         return( 0 );
      else
         DropObjectInstance( vTZCMCUSR );
   }
   // Set up qualification object and activate CPLR Users
   if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vTZCMCUSR, "TZCMCUSR",
                               ulZKey, szName, zMULTIPLE ) < 0 )
   {
      return( -1 );
   }
   SetNameForView( vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshCtrl( vParentWindow, "lbCPLRList" );

   return( 0 );
} // zwTZCMCPLD_LoadCPLR_Users

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_DropViewForCPLR_Users
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_DropViewForCPLR_Users( zVIEW vSubtask )
{
   zVIEW vTZCMCUSR;

   if ( GetViewByName( &vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMCUSR );

   return( 0 );
} // zwTZCMCPLD_DropViewForCPLR_Users

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CPLR_Dependencies
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CPLR_Dependencies( zVIEW vSubtask )
{
   zVIEW vAllReusableCPLR;

   if ( ActivateObjectInstance( &vAllReusableCPLR, "TZCMRCRO", vSubtask,
                                      0, zSINGLE ) == zCALL_ERROR )
   {
      MessageSend( vSubtask, "CM00129", "Configuration Management",
                   "Activation failed on load of Repository",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   SetNameForView( vAllReusableCPLR, "AllReusableCPLR", vSubtask, zLEVEL_TASK );

   return( 0 );
} // zwTZCMCPLD_CPLR_Dependencies

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CloseDependencies
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CloseDependencies( zVIEW vSubtask )
{
   zVIEW vReusableCPLR;

   if ( GetViewByName( &vReusableCPLR, "AllReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   return( 0 );
} // zwTZCMCPLD_CloseDependencies

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_Search_CPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_Search_CPLR( zVIEW vSubtask )
{
   zVIEW   vTZCMCPL;
   zVIEW   vReusableCPLR;
   zVIEW   vAllReusableCPLR;
   zVIEW   vTZCMRCRO;
   zVIEW   vParentWindow;
   zCHAR   szName[ 33 ];
   zULONG  ulZKey;
   zSHORT  nRC;

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vAllReusableCPLR, "AllReusableCPLR", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vReusableCPLR );

   CreateViewFromViewForTask( &vTZCMRCRO, vAllReusableCPLR, 0 );
   while ( ResetViewFromSubobject( vTZCMRCRO ) == 0 );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );

   // Searching for all CPLR, who reusing the current CPLR
   nRC = SetCursorFirstEntityByInteger( vTZCMRCRO, "ReusableCPLR", "ZKey",
                                        ulZKey, "Installation" );

   if ( nRC < zCURSOR_SET )
   {
      if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vReusableCPLR, "TZCMRCRO",
                                  ulZKey, szName, zSINGLE ) < 0 )
      {
         return( -1 );
      }
      SetNameForView( vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK );
   }

   while ( nRC >= zCURSOR_SET )
   {
      if ( zwfnTZCMCPLD_Search_CPLR( vSubtask, vTZCMRCRO ) < 0 )
         return( -1 );

      nRC = SetCursorNextEntityByInteger( vTZCMRCRO, "ReusableCPLR", "ZKey",
                                          ulZKey, "Installation" );
   }

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshCtrl( vParentWindow, "lbCPLRList" );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      while ( ResetViewFromSubobject( vReusableCPLR ) == 0 );

      if ( SetCursorFirstEntityByInteger( vReusableCPLR, "CPLR", "ZKey",
                                    ulZKey, "Installation" ) >= zCURSOR_SET )
      {
         DeleteEntity( vReusableCPLR, "CPLR", zREPOS_NONE );
      }
      SetCursorFirstEntity( vReusableCPLR, "CPLR", "Installation" );
   }

   RefreshCtrl( vSubtask, "SearchResult" );
   OL_ExpandEntity( vSubtask, "SearchResult", "CPLR", zEXPANDALL );

   OL_ResetAllTextColorsForItemText( vSubtask, "SearchResult" );
   OL_ResetAllTextColorsForItemText( vSubtask, "ResuableCPLR" );

   OL_SetTextColorForItemText( vSubtask, "SearchResult", szName, 255, FALSE );
   OL_SetTextColorForItemText( vSubtask, "ResuableCPLR", szName, 255, FALSE );

   DropView( vTZCMRCRO );
   return( 0 );
} // zwTZCMCPLD_Search_CPLR

static zSHORT
zwfnTZCMCPLD_Search_CPLR( zVIEW vSubtask, zVIEW vTZCMRCRO )
{
   zVIEW  vReusParent;
   zVIEW  vReusableCPLR;
   zULONG ulZKey;
   zSHORT nRC;

   CreateViewFromViewForTask( &vReusParent, vTZCMRCRO, 0 );
   SetNameForView( vReusParent, "ResuParent", vSubtask, zLEVEL_TASK );

   // Search Parent CPLR
   do
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vReusParent, "CPLR", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vReusParent, "ReusableCPLR", "ZKey",
                                           ulZKey, "Installation" );

   } while ( nRC >= zCURSOR_SET );

   if ( GetViewByName( &vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vReusableCPLR, "TZCMRCRO", vSubtask,
                                   zLEVEL_TASK | zSINGLE );
      SetNameForView( vReusableCPLR, "ReusableCPLR", vSubtask, zLEVEL_TASK );
      CreateEntity( vReusableCPLR, "Installation", zPOS_AFTER );
   }

   nRC = zwfnTZCMCPLD_SelectReusable_UpdateView( vSubtask, vReusParent );

   DropView( vReusParent );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ExpandAllSearchCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ExpandAllSearchCPLR( zVIEW vSubtask )
{
   OL_ExpandEntity( vSubtask, "SearchResult", "CPLR", zEXPANDALL );

   return( 0 );
} // zwTZCMCPLD_ExpandAllSearchCPLR


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CollapseAllSearchCPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CollapseAllSearchCPLR( zVIEW vSubtask )
{
   OL_ExpandEntity( vSubtask, "SearchResult", "CPLR", zCOLLAPSEALL );

   return( 0 );
} // zwTZCMCPLD_CollapseAllSearchCPLR


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadReusedBy
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadReusedBy( zVIEW vSubtask )
{
   zVIEW   vParentWindow;

   zwTZCMCPLD_SetCPL_Name( vSubtask );

   zwTZCMCPLD_CPLR_Dependencies( vSubtask );
   zwTZCMCPLD_Search_CPLR( vSubtask );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshCtrl( vParentWindow, "lbCPLRList" );

   return( 0 );
} // zwTZCMCPLD_LoadReusedBy

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadNewReusedBy
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadNewReusedBy( zVIEW vSubtask )
{
   zVIEW   vParentWindow;

   zwTZCMCPLD_SetCPL_Name( vSubtask );
   zwTZCMCPLD_Search_CPLR( vSubtask );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshCtrl( vParentWindow, "lbCPLRList" );

   return( 0 );
} // zwTZCMCPLD_LoadNewReusedBy

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetMinCtrlPos_Depend
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetMinCtrlPos_Depend( zVIEW vSubtask )
{
   zLONG  lCtrlPos_X;
   zLONG  lCtrlSize_X;
   zLONG  lTemp;

   // Set new Size for ComboBox
   GetCtrlPosition( vSubtask, "pbSearch", &lCtrlPos_X, &lTemp, TRUE );
   lCtrlSize_X = lCtrlPos_X -30;

   SetCtrlSize( vSubtask, "cbCPLR", lCtrlSize_X, 150, TRUE );

   return( 0 );
} // zwTZCMCPLD_SetMinCtrlPos_Depend

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ListPrefDisableUserCB
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ListPrefDisableUserCB( zVIEW vSubtask )
{
   zVIEW   vTZCMWIPO;
   zSHORT  nStatus;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetIntegerFromAttribute( (zPLONG) &nStatus, vTZCMWIPO, "ROOT", "ComponentStatus" );

   if ( nStatus == 3 )
      bEnable = TRUE;

   SetCtrlState( vSubtask, "cbUser", zCONTROL_STATUS_ENABLED, bEnable );

   return( 0 );
} // zwTZCMCPLD_ListPrefDisableUserCB

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ListPrefSetCPL_Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ListPrefSetCPL_Name( zVIEW vSubtask )
{
   zVIEW   vAllMetas;
   zVIEW   vTZCMCPL;
   zVIEW   vTZCMCUSR;
   zVIEW   vTZCMFILE;
   zVIEW   vParentWindow;
   zULONG  ulZKey;
   zULONG  ulZKeyCPLR;
   zCHAR   szName[ 33 ];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
   GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPL", "Name" );

   zwTZCMCPLD_SetCPL_Name( vSubtask );

   if ( GetViewByName( &vAllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKeyCPLR, vAllMetas, "CPLR", "ZKey" );
      if ( ulZKey == ulZKeyCPLR )
      {
         return( 0 );
      }
      else
      {
         DropObjectInstance( vAllMetas );
      }
   }

   if ( GetViewByName( &vAllMetas, "CPLR_MetaList", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vAllMetas );

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMFILE );

   if ( GetViewByName( &vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMCUSR );

   // Set up qualification object and activate all CPLR Users
   if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vTZCMCUSR, "TZCMCUSR",
                               ulZKey, szName, zSINGLE ) < 0 )
   {
      return( -1 );
   }
   SetNameForView( vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshCtrl( vParentWindow, "lbCPLRList" );

   return( 0 );
} // zwTZCMCPLD_ListPrefSetCPL_Name

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ListPrefPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ListPrefPostBuild( zVIEW vSubtask )
{
   zVIEW   vTZCMCUSR;
   zVIEW   vTZCMCPL;
   zULONG  ulZKey;
   zCHAR   szName[ 33 ];

   SetCtrlState( vSubtask, "AllComp", zCONTROL_STATUS_CHECKED, TRUE );
   zwTZCMCPLD_DisableCompCtrls( vSubtask );
   zwTZCMCPLD_SetCPL_Name( vSubtask );

   if ( GetViewByName( &vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

      if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );
      }
      else
      {
         return( -1 );
      }

      // Set up qualification object and activate all CPLR Users
      if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vTZCMCUSR, "TZCMCUSR",
                                  ulZKey, szName, zSINGLE ) < 0 )
      {
         return( -1 );
      }
      SetNameForView( vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
} // zwTZCMCPLD_ListPrefPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_MetaInfoInit_All
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_MetaInfoInit_All( zVIEW vSubtask )
{
   zVIEW vCPLR_AllMetas;

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   zwfnTZCMCPLD_LoadMeta( vSubtask, vCPLR_AllMetas );

   return( 0 );
} // zwTZCMCPLD_MetaInfoInit_All

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_DisableButton
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_DisableButton( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW vTZCMWIPO;
   zVIEW vCPLR_AllMetas;
   zBOOL bVisibleDelete = FALSE;
   zBOOL bVisibleRemove = FALSE;

   // No Delete Meta in View Menu and no delete for "Update" or "Read Only"
   // Users
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ListPref" ) < 0 )
   {
      if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
      {
         bVisibleDelete = TRUE;
         bVisibleRemove = TRUE;
      }
      else
      {
         // if Component checked out by this user
         GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK );
         GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

         if ( CheckExistenceOfEntity( vCPLR_AllMetas, "User" ) >= zCURSOR_SET &&
              CompareAttributeToAttribute( vTZCMWIPO, "ROOT", "UserName",
                                           vCPLR_AllMetas, "User", "Name" ) == 0 )
         {
            bVisibleRemove = TRUE;
         }
      }
   }

   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_VISIBLE, bVisibleDelete );
   SetCtrlState( vSubtask, "Remove", zCONTROL_STATUS_VISIBLE, bVisibleRemove );

   return( 0 );
} // zwTZCMCPLD_DisableButton

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_Load_WithEnq_Cluster
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_Load_WithEnq_Cluster( zVIEW vSubtask )
{
   zVIEW   vTZCMCPLR;
   zVIEW   vTZCMCPL;
   zLONG   lCPLR_ViewCluster;

   if ( zwTZCMCPLD_LoadCPLR_WithEnq( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   CreateViewCluster( vSubtask, &lCPLR_ViewCluster );
   AddToViewCluster( lCPLR_ViewCluster, vTZCMCPL, 0 );
   AddToViewCluster( lCPLR_ViewCluster, vTZCMCPLR, 0 );

   SetAttributeFromInteger( vTZCMCPLR, "CPLR", "CPLR_ViewCluster",
                            lCPLR_ViewCluster );

   return( 0 );
} // zwTZCMCPLD_Load_WithEnq_Cluster

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_ComponentPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_ComponentPostBuild( zVIEW vSubtask )
{
   zVIEW   vMetas;
   zVIEW   vTZCMCUSR;
   zVIEW   vTZCMCPL;
   zVIEW   vTZCMFILE;
   zVIEW   vCPLR_AllMetas;
   zULONG  ulZKey;
   zCHAR   szName[ 33 ];

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      EnableAction( vSubtask, "DeleteFile", FALSE );
      EnableAction( vSubtask, "DeleteMetaFromList", FALSE );
      EnableAction( vSubtask, "DeleteMetaFromListAll", FALSE );
   }

   SetCtrlState( vSubtask, "AllComp", zCONTROL_STATUS_CHECKED, TRUE );
   zwTZCMCPLD_SetOkButtonText( vSubtask );
   zwTZCMCPLD_DisableCompCtrls( vSubtask );

   if ( GetViewByName( &vMetas, "CPLR_MetaList", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vMetas );

   if ( GetViewByName( &vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

      if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
         GetStringFromAttribute( szName, zsizeof( szName ), vTZCMCPL, "CPLR", "Name" );
      }
      else
      {
         return( -1 );
      }

      // Set up qualification object and activate all CPLR Users
      if ( zwfnTZCMCPLD_LoadCPLR( vSubtask, &vTZCMCUSR, "TZCMCUSR",
                                  ulZKey, szName, zSINGLE ) < 0 )
      {
         return( -1 );
      }
      SetNameForView( vTZCMCUSR, "TZCMCUSR", vSubtask, zLEVEL_TASK );
   }

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) > 0 )
      CreateTemporalSubobjectVersion( vTZCMFILE, "CPLR" );

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
      CreateTemporalSubobjectVersion( vCPLR_AllMetas, "CPLR" );

   return( 0 );
} // zwTZCMCPLD_ComponentPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_CancelComponents
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_CancelComponents( zVIEW vSubtask )
{
   zVIEW  vTZCMFILE;
   zVIEW  vTZCMCPLR;
   zVIEW  vCPLR_AllMetas;
   zVIEW  vWindow;
   zLONG  lFile_ViewCluster;

   if ( GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // Get CPLR View Cluster.
      GetIntegerFromAttribute( &lFile_ViewCluster, vTZCMFILE, "CPLR",
                               "File_ViewCluster" );

      CancelSubobject( vTZCMFILE, "CPLR" );

      // Drop File View Cluster
      if ( lFile_ViewCluster != 0 )
      {
         DropViewCluster( lFile_ViewCluster );
         SetAttributeFromInteger( vTZCMFILE, "CPLR", "File_ViewCluster", 0 );
      }
   }

   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
   {
      CancelSubobject( vCPLR_AllMetas, "CPLR" );
      // reset Update flag for View
      SetAttributeFromInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta", 0 );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) < 0 )
   {
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );
      zwTZCMCPLD_DropComponentViews( vSubtask );
   }

   return( 0 );
} // zwTZCMCPLD_CancelComponents

static zSHORT
zwfnTZCMCPLD_OKFiles( zVIEW vSubtask, zVIEW vTZCMFILE, zVIEW vCPLR_AllMetas,
                      zLONG lCPLR_ViewCluster, zLONG lFile_ViewCluster,
                      zPBOOL bCommitMetaList )
{
   zVIEW  vWindow;
   zLONG  lErrorIndex;
   zSHORT nRC;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) >= 0 )
   {
      // Move View from File View Cluster to CPLR View Cluster
      AppendViewCluster( lCPLR_ViewCluster, lFile_ViewCluster );
      SetAttributeFromInteger( vTZCMFILE, "CPLR", "File_ViewCluster", 0 );
      return( 1 );
   }
   else
   {
      if ( *bCommitMetaList )
      {
         AddToViewCluster( lFile_ViewCluster, vCPLR_AllMetas, 0 );
         *bCommitMetaList = FALSE;
      }

      nRC = CommitMultipleObjectInstances( lFile_ViewCluster, &lErrorIndex );
      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "CM00101", "Configuration Management",
                      "Unable to Commit Component/File View Cluster!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( nRC );
      }

      // Drop File View Cluster
      DropViewCluster( lFile_ViewCluster );
      SetAttributeFromInteger( vTZCMFILE, "CPLR", "File_ViewCluster", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_OKComponents
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_OKComponents( zVIEW vSubtask )
{
   zVIEW  vTZCMFILE;
   zVIEW  vTZCMCPLR;
   zVIEW  vCPLR_AllMetas;
   zVIEW  vWindow;
   zLONG  lCPLR_ViewCluster;
   zLONG  lFile_ViewCluster;
   zSHORT nRC;
   zBOOL  bCommitMetaList = FALSE;

   if ( GetViewByName( &vTZCMCPLR, "TZCMCPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) == FALSE )
   {
      zwTZCMCPLD_CancelComponents( vSubtask );
      return( 0 );
   }

   // Get CPLR View Cluster.
   GetIntegerFromAttribute( &lCPLR_ViewCluster, vTZCMCPLR, "CPLR", "CPLR_ViewCluster" );

   // get Meta List View and check update flag for View
   if ( GetViewByName( &vCPLR_AllMetas, "CPLR_AllMetas", vSubtask, zLEVEL_TASK ) > 0 )
   {
      AcceptSubobject( vCPLR_AllMetas, "CPLR" );
      if ( CompareAttributeToInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta",
                                      1 ) == 0 )
      {
         // reset Update flag for View
         SetAttributeFromInteger( vCPLR_AllMetas, "CPLR", "DeleteMeta", 0 );

         if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) >= 0 )
            AddToViewCluster( lCPLR_ViewCluster, vCPLR_AllMetas, 0 );
         else
            bCommitMetaList = TRUE;
      }
   }

   if ( GetViewByName( &vTZCMFILE, "TZCMFILE", vSubtask, zLEVEL_TASK ) > 0 )
   {
      AcceptSubobject( vTZCMFILE, "CPLR" );

      // Get File View Cluster.
      GetIntegerFromAttribute( &lFile_ViewCluster, vTZCMFILE, "CPLR",
                               "File_ViewCluster" );

      if ( lFile_ViewCluster != 0 )
      {
         nRC = zwfnTZCMCPLD_OKFiles( vSubtask, vTZCMFILE, vCPLR_AllMetas,
                                     lCPLR_ViewCluster, lFile_ViewCluster,
                                     &bCommitMetaList );
         if ( nRC )
            return( nRC );
      }
   }

   if ( bCommitMetaList )
   {
      nRC = CommitObjectInstance( vCPLR_AllMetas );
      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "CM00101", "Configuration Management",
                      "Unable to Commit Component View!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( nRC );
      }
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CPLR_Maintenance" ) < 0 )
   {
      oTZCMCPRO_CPLR_Enqueue( vTZCMCPLR, vSubtask, zCPLR_DEQUEUE );
      zwTZCMCPLD_DropComponentViews( vSubtask );
   }

   return( 0 );
} // zwTZCMCPLD_OKComponents

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_LoadPopup_Users
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_LoadPopup_Users( zVIEW vSubtask )
{
   zVIEW  vTZCMRUWK;
   zVIEW  vUser;

   if ( TG_GetMouseClick( vSubtask, zMOUSE_RIGHT ) == 1 )
   {
      if ( GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK ) > 0 &&
           CheckExistenceOfEntity( vTZCMRUWK, "User" ) >= zCURSOR_SET )
      {
         CreateViewFromViewForTask( &vUser, vTZCMRUWK, 0 );

         if ( SetCursorFirstSelectedEntity( vUser, "User", "" ) >= zCURSOR_SET &&
              zwfnTZCMCPLD_IsSysadm( vSubtask ) )
         {
            CreateTrackingPopupMenu( vSubtask, "Users", -1, -1, FALSE, FALSE );
         }

         DropView( vUser );
      }
   }

   return( 0 );
} // zwTZCMCPLD_LoadPopup_Users

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SetAuthForSelUsers
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SetAuthForSelUsers( zVIEW vSubtask )
{
   zVIEW  vTZCMRUWK;
   zVIEW  vUser;
   zSHORT nRC;

   if ( GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vTZCMRUWK, "User" ) >= zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vUser, vTZCMRUWK, 0 );

      for ( nRC = SetCursorFirstSelectedEntity( vUser, "User", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vUser, "User", 0 ) )
      {
         SetAttributeFromString( vUser, "User", "CPLR_User", "Y" );
         SetAttributeFromAttribute( vUser, "User", "Authority",
                                    vTZCMRUWK, "Installation", "Authority" );
      }

      DropView( vUser );
   }

    return( 0 );
} // zwTZCMCPLD_SetAuthForSelUsers

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_RemoveAuthForSelUsers
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_RemoveAuthForSelUsers( zVIEW vSubtask )
{
   zVIEW  vTZCMRUWK;
   zVIEW  vUser;
   zSHORT nRC;

   if ( GetViewByName( &vTZCMRUWK, "TZCMRUWK", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vTZCMRUWK, "User" ) >= zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vUser, vTZCMRUWK, 0 );

      for ( nRC = SetCursorFirstSelectedEntity( vUser, "User", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vUser, "User", 0 ) )
      {
         SetAttributeFromString( vUser, "User", "CPLR_User", "N" );
         SetAttributeFromString( vUser, "User", "Authority", "" );
      }

      DropView( vUser );
   }

   return( 0 );
} // zwTZCMCPLD_RemoveAuthForSelUsers

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_GrayCPL_Detail
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_GrayCPL_Detail( zVIEW vSubtask )
{
   zVIEW vTZCMCPL;
   zBOOL bSysadm = FALSE;

   if ( zwfnTZCMCPLD_IsSysadm( vSubtask ) )
   {
      bSysadm = TRUE;
   }

   SetCtrlState( vSubtask, "Name", zCONTROL_STATUS_ENABLED, bSysadm );
   SetCtrlState( vSubtask, "Desc", zCONTROL_STATUS_ENABLED, bSysadm );

   if ( GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // if a new CPL set Save Button Text to OK
      if ( CheckExistenceOfEntity ( vTZCMCPL, "CPLR" ) == zCURSOR_NULL )
         SetCtrlText( vSubtask, "OK", "OK" );
   }

   return( 0 );
} // zwTZCMCPLD_GrayCPL_Detail

static zBOOL
zwfnTZCMCPLD_IsSysadm( zVIEW vSubtask )
{
   zVIEW    vTZCMREPO;
   zVIEW    vTZCMWIPO;
   zBOOL    bSysadm = FALSE;

   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToAttribute( vTZCMWIPO, "ROOT",         "UserName",
                                     vTZCMREPO, "Installation", "Sysadm" )
                                     == 0 )
   {
      bSysadm = TRUE;
   }

   return( bSysadm );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_DisableCompCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_DisableCompCtrls( zVIEW vSubtask )
{
   zVIEW   vTZCMWIPO;
   zSHORT  nStatus;
   zBOOL   bEnableUser = FALSE;
   zBOOL   bEnable = FALSE;

   if ( GetCtrlState( vSubtask, "SelectedComp", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      bEnable = TRUE;
      SetCtrlState( vSubtask, "lbFileList", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "lbMetaList", zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "lbAllMetaList", zCONTROL_STATUS_VISIBLE, FALSE );

      if ( GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         GetIntegerFromAttribute( (zPLONG) &nStatus, vTZCMWIPO, "ROOT",
                                  "ComponentStatus" );

         if ( nStatus == 3 )
            bEnableUser = TRUE;
      }
   }

   SetCtrlState( vSubtask, "ComponentTyp", zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "Status", zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "cbUser", zCONTROL_STATUS_ENABLED, bEnableUser );

   if ( GetCtrlState( vSubtask, "AllComp", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetCtrlState( vSubtask, "lbFileList", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "lbMetaList", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "lbAllMetaList", zCONTROL_STATUS_VISIBLE, TRUE );
   }

   if ( GetCtrlState( vSubtask, "AllFiles", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetCtrlState( vSubtask, "lbFileList", zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "lbMetaList", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "lbAllMetaList", zCONTROL_STATUS_VISIBLE, FALSE );
   }

   return( 0 );
} // zwTZCMCPLD_DisableCompCtrls

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_SearchComponents
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_SearchComponents( zVIEW vSubtask )
{
   if ( GetCtrlState( vSubtask, "SelectedComp", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      zwTZCMCPLD_BuildMetaList( vSubtask );
      return( 0 );
   }

   if ( GetCtrlState( vSubtask, "AllComp", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      zwTZCMCPLD_ListAll( vSubtask );
      return( 0 );
   }

   if ( GetCtrlState( vSubtask, "AllFiles", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      zwTZCMCPLD_BuildFileList( vSubtask );
      return( 0 );
   }

   return( -1 );
} // zwTZCMCPLD_SearchComponents


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZCMCPLD_AddAnotherTextLimit
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMCPLD_AddAnotherTextLimit( zVIEW vSubtask )
{
   ED_LimitTextLength( vSubtask, "edVerification", 8 );

   return( 0 );
} // zwTZCMCPLD_AddAnotherTextLimit


#ifdef __cplusplus
}
#endif

///////////////////////////// end of file  //////////////////////////////////
