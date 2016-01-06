/*
CHANGE LOG
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2001.02.14  BL   R54697
   improve handling of return code from InitializeLPLR
2000.12.27  BL   RAD 53796
   Modified zwfnTZDMUPDD_SaveDomain: if Domain from Type "Format",
   check Context EditString
2000.10.30  RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.16  TMV
   create preliminaries for generating proper domain operation C-prototype :
   see Bug report 050613
2000.10.12  BL
   Modified zwTZDMUPDD_RebuildXDM for delete all Domains in Domain
   Executables for Repostiory LPLR's
2000.09.22  BL
   Bugfix move Domain, reuse ZKey
2000.08.13  BL
   Added zwTZDMUPDD_SaveAsPreBuild, zwTZDMUPDD_SaveAsCheckValues,
   zwfnTZDMUPDD_PromptForDelete, zwfnTZDMUPDD_DeleteFromXLP,
   zwfnTZDMUPDD_DeleteCode, zwTZDMUPDD_CopyOperationWorkLOD,
   zwTZDMUPDD_CreateContext, zwTZDMUPDD_RemoveOperFromGroup
   and modified zwTZDMUPDD_SaveAsPostBuild, zwTZDMUPDD_SaveAsCancel,
   zwTZDMUPDD_SetGroupName, zwTZDMUPDD_SaveAsDomain,
   zwTZDMUPDD_SaveAsNewDomain for save as with Operation Code
   Rename zwTZDMUPDD_SetFlagForSaveAsDom to zwTZDMUPDD_SaveAsRtnFromSubwnd
   Remove zwTZDMUPDD_CheckDomainGroup
2000.07.30  BL
   Rename zwTZDMUPDD_SetFlagForMoveDomain to zwTZDMUPDD_MoveRtnFromSubwindow
   Added zwTZDMUPDD_MoveDomainCancel, zwTZDMUPDD_CreateWorkLOD,
   zwTZDMUPDD_MoveCheckGroupType, zwTZDMUPDD_DeleteOperFromFile,
   zwTZDMUPDD_CreateOperation, zwfnTZDMUPDD_CreateOperation
   and modified zwTZDMUPDD_MoveDomainPreBuild, zwTZDMUPDD_MoveDomainPostBuild,
   zwTZDMUPDD_CreateContextAndOper, zwTZDMUPDD_MoveDomainToNewGroup,
   zwTZDMUPDD_MoveDomainOperToFile for move Domain with Operation Code
   Modified zwTZDMUPDD_DeleteOperation, zwTZDMUPDD_CreateDeleteWorkLod for
   delete Operation from File
   Modified zwTZDMUPDD_GoToNewTableEntry for Bug TB 51445
2000.07.18  BL  Repository
   Added zwfnTZDMUPDD_InitMenuDisableAll, zwTZDMUPDD_LoadPopup,
   zwTZDMUPDD_TablePopup, zwTZDMUPDD_OperationPopup, zwTZDMUPDD_DIL_Popup,
   zwTZDMUPDD_ExtValue_Popup, zwTZDMUPDD_CheckGroupName,
   zwTZDMUPDD_CheckGroupType: if Domain Group not checked out disable all
   function for update
2000.07.17  BL   TB 51372
   Added zwTZDMUPDD_LoadDomainGroupList, zwTZDMUPDD_LoadDomainList,
   zwTZDMUPDD_LoadOperationList for Bug 51372
2000.07.16  BL
   Added zwTZDMUPDD_SetFlagForNewDomain, zwTZDMUPDD_ShowDetail,
   zwTZDMUPDD_SetFlagForMoveDomain, zwTZDMUPDD_SetFlagForSaveAsDom,
   zwTZDMUPDD_AcceptSubobject, zwTZDMUPDD_SetNullEntry, zwTZDMUPDD_TableEntryOK,
   zwTZDMUPDD_CreateDomainParameter, zwTZDMUPDD_CheckDomainGroupTyp
   and modified operations for completely maintained Domain tool.
   Added zwTZDMUPDD_SaveAsKeepCurrentDesc for save as
   Insert defines for save as
2000.05.24  BL
   remove warning
2000.05.02  BL
   Modified zwTZDMUPDD_CheckActionAfterSaveAs: after save as, set new title
2000.04.26  BL
   Modified zwTZDMUPDD_Check_CheckoutStatus: if Domain Group not checked
   out, disable Shortcut for delete Domain
   Remove SortComponents, the Open Window has a Listbox with Remember Sort
   Buttons
2000.04.08  DC  Z10
   Modified MoveDomainToNewGroup so that the ZKey would be saved. That way
   we don't have to rebuild the XODs to pick up reference to the new ZKey.
2000.02.26  BL  Z10
   Modified zwTZDMUPDD_OpenDomainPreBuild for set cursor on current Domain
   Added zwTZDMUPDD_SaveAsCancel, zwTZDMUPDD_CheckActionAfterSaveAs,
   zwTZDMUPDD_OpenDomain, zwTZDMUPDD_NewDomain, zwfnTZDMUPDD_OpenDom
   and modified functions for save Action after save as
   Modified save as functions for set check out state
   Added zwfnTZDMUPDD_AskForSave and zwfnTZDMUPDD_AskForSave_YES for reuse
   code for not checked out Domain Groups
   Added zwfnTZDMUPDD_ActivateDomain and modified zwTZDMUPDD_ActivateDomain,
   zwTZDMUPDD_OpenNextDom, zwTZDMUPDD_OpenPrevDom, zwTZDMUPDD_LoadDomain:
   If Attribute "ContextRestriction" for Entity "Domain" not "Y" or "N",
   then zeidon ask for save. This is a work around for old Domains
2000.02.25  BL  Z10  Repository
   Added zwTZDMUPDD_DeleteSelOperation for delete selected Operation
   Added zwTZDMUPDD_SaveSortOrder and modified zwTZDMUPDD_OpenDomainPreBuild
   for save current sort order
   Modified Operation zwTZDMUPDD_InitMenu and added zwTZDMUPDD_InitListMenu
   Added zwTZDMUPDD_GroupCheckedOut, zwTZDMUPDD_SetCeckOutState and modified
   zwTZDMUPDD_SetGroupName, zwTZDMUPDD_DomainGroupPopup,
   zwTZDMUPDD_MoveDomainPostBuild for check checked out state
   Added zwTZDMUPDD_Check_CheckoutStatus: if Domain Group not checked out, then
   disable Delete and Move Buttons in window "Open Domain"
   Modified zwTZDMUPDD_MainRtnFromSubWnd, zwTZDMUPDD_InitTool2,
   zwTZDMUPDD_OpenPrevDom and zwTZDMUPDD_OpenNextDom: set check out state in Title
2000.01.24  BL  Z10  Repository
   Modified zwTZDMUPDD_InitMenu: if Domain Group not checked out, then disable
   menu items "Save" and "Move Domain"
   Modified zwTZDMUPDD_MainRtnFromSubWnd: if Domain Group not checked out, then set
   "<<< Domain Group is not checked out >>>" in Title
1999.11.03  BL  Z10  new menu structure
   Added Operation zwTZDMUPDD_MoveDomainPostBuild for Move Domain
   Added Operation zwTZDMUPDD_SaveAsInNewGroup for save Domain in new Domain
   Group
   Move Operation SaveDomainFromSource, MoveDomainToNewGroup and AcceptDomainGroup
   from TZDMUP2D.VML
1999.10.30  BL  Z10  new menu structur
   Added Operation zwfnTZDMUPDD_DeleteDomain and zwTZDMUPDD_SaveAsNewDomain and
   modified Operation zwTZDMUPDD_DeleteDomain for Save Domain as
1999.10.29  BL  Z10  new menu structure
   Added Operation zwTZDMUPDD_SaveAsPostBuild, zwTZDMUPDD_SetSaveAsName,
   zwTZDMUPDD_SaveAsDomain, zwTZDMUPDD_RefreshGroupList, zwTZDMUPDD_SetGroupName,
   zwTZDMUPDD_CheckDomainGroup, zwTZDMUPDD_ResetProfileXFER
   for Save Domain as
   Added Operation zwTZDMUPDD_MoveDomainPreBuild and zwTZDMUPDD_Set_CM_List for
   move Domain in a new Domain Group
1999.10.28  BL  Z10  new menu structure
   Added Operation zwTZDMUPDD_InitMenu and modified Operations zwTZDMUPDD_InitTool2,
   zwTZDMUPDD_MainRtnFromSubWnd, zwTZDMUPDD_OpenNextDom, zwTZDMUPDD_OpenPrevDom and
   delete zwTZDMUPDD_SelectOperation for enable or disable menu items
   Modified Operation zwTZDMUPDD_SaveDomain for refresh all Subwindows
1999.10.27  BL  Z10  new menu structure
   Added Operation zwTZDMUPDD_EditOperationcode and zwTZDMUPDD_EditOperationcodeMenu
   for called Operation zwTZDMUPDD_TransferToEditor
   Modified Operation zwTZDMUPDD_TransferToEditor for window OperationList
   Added Operation zwTZDMUPDD_DeleteOperation for delete Operation
   Modified Operation AcceptOperation: if Operation updated, then refresh
   main window
   Modified Operation zwTZDMUPDD_OpenPrevDom and zwTZDMUPDD_OpenNextDom for
   refresh window DomainList ( = Modeless Window)
   Added Operation zwTZDMUPDD_RefreshDomainList: refresh Domain Name in window
   DomainList ( = Modeless Window)
   Modified Operation InitTool2 and zwTZDMUPDD_MainRtnFromSubWnd: Set LPLR and
   Domain name in window title
1999.10.21  BL  Z10  new menu structure
   Modified Operation zwTZDMUPDD_MainRtnFromSubWnd, zwTZDMUPDD_ListRtnFromSubwnd
   and zwTZDMUPDD_AskForSave:
      Refresh all Lists from menu <View> (update Windows in Modeless Subwindows)
   Added Operation zwTZDMUPDD_LoadDomain: Popup in window <Domain within current Group>
   Modified Operation zwTZDMUPDD_DeleteDomainGrp:
      if delete current Domain Group, then refresh Parentwindow and set Focus
      to window Domain Group List again
from here old version
19.07.1997  DonC
   Removed OrderEntityForView for Context entities from serveral operations
   on read and put one in AskForSave.  Thus Contexts are ordered by name on
   save and not on read.  On read they were setting the update flag for the
   object instance and causing prompts for save.
25.07.1997  DonC
   Created NewDomainPrebuild to blank out Name and Desc fields in Profile
   OI so old values wouldn't appear in the window.
06.01.1997  DonC
   Modified TransferToEditor to position on correct Operation entity because
   editor was not being positioned on correct operation.
13.01.1998
   Modified AskForSave to stay on window during mapping error.
15.07.1998  DonC
   Modified MainRtnFromSubWnd, SwitchLPLR, OpenNextDom and OpenPrevDom so
   that Cancel SwitchLPLR would function correctly.
16.07.1998  DonC
   Modified DeleteDomain to correctly handle the situation where the Domain
   being deleted is the Domain currently being displayed.
18.08.1998  DonC
   Modified call to InitializeDefaultLPL to check for error return code.
1998/09/09  DC
   Modified zwTZDMUPDD_DeleteDomain so that deleted Domain "stays deleted"
   under all circumstances. (TB 242)
1998/10/06  DC
   Modified zwTZDMUPDD_CreateNewDomain to check for erroneous return code
   on ActivateMetaOI.
1998/11/03  DC
   Modified zwTZDMUPDD_SortByDate so that date sort would be descending.
1999/02/11  DC
   Modified zwTZDMUPDD_OpenNextDom and zwTZDMUPDD_OpenPrevDom operations
   so that Edit menu option would be correctly grayed. (TB 496)
14.04.1999  HH
   Modified zwTZDMUPDD_DeleteDomain so that, if the meta to delete was
   not found by ActivateMetaOI, the delete from the XLP is done anyway.
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#define zGLOBAL_DATA
#define  KZOESYS_INCL
#define  zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
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

#include "kzoengaa.h"
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZeidonOp.H"

#define zDM_NewDomain              "N"
#define zDM_MoveDomain             "M"
#define zDM_MoveDomainInNewGroup   "X"
#define zDM_SaveAsDomainInNewGroup "S"
#define zDM_SaveAsDomain           "A"
#define zDM_ShowGroupDetail        "D"
#define zDM_UpdateGroupDetail      "U"
#define zDM_ExitNewDomain          "E"
#define zDM_LoadGroupProperties    "P"

#define zDM_REPLACE_CODE            1
#define zDM_NOREPLACE_CODE          0


// LAST MESSAGE ID: DM00116

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SetCeckOutState( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_GroupCheckedOut( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZDMUPDD_SaveDomain( zVIEW vSubtask, zVIEW vDomain );

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_PrepareForOperations( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteDomain( zVIEW vSubtask );

zSHORT /* LOCAL */  OPERATION
zwTZDMUPDD_SaveAsNewDomain( zVIEW vSubtask,
                            zPCHAR szGroupName,
                            zPCHAR szDomainName,
                            zSHORT nCopyCode,
                            zSHORT nDeleteSourceDomain );

zSHORT /* LOCAL */  OPERATION
zwTZDMUPDD_CreateContextAndOper( zVIEW  vSubtask,
                                 zVIEW  vSource,
                                 zVIEW  vTarget,
                                 zSHORT nMoveFlag );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW   vSubtask,
                           zPCHAR  szToolname,
                           zPCHAR  szDomainname,
                           zVIEW   vToolView,
                           zPCHAR  szEntityname,
                           zLONG   lType );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_AcceptSubobject( zVIEW vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szAttributeName,
                            zPCHAR szMessageText );

zOPER_EXPORT zLONG OPERATION
zGetStringWithoutBlank( zPCHAR   szSourceString,
                        zPCHAR   pszTargetString );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_SetNullEntry( zVIEW vSubtask, zVIEW vTZDGSRCO );

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CONTEXT_INIT( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_LoadPopup( zVIEW vSubtask,
                      zPCHAR szEntityName,
                      zPCHAR szActionNameForNew,
                      zPCHAR szActionNameForDelete,
                      zPCHAR szPopupName );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_CreateWorkLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_MoveOperationCode( zVIEW  vSubtask,
                            zSHORT nCopyOrMoveFlag );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_DropOperationFromSource( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_CopyOperationCode( zVIEW vSubtask,
                            zSHORT nRenameOperation );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_DeleteOperFromFile( zVIEW  vSubtask,
                               zVIEW  vTZDGSRCO,
                               zSHORT nMoveOrDeleteFlag );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CreateDeleteWorkLod( zVIEW  vSubtask,
                                zVIEW  vTZDGSRCO,
                                zPCHAR szOperationName );

static zSHORT
zwTZDMUPDD_CheckActionAfterSaveAs( zVIEW  vSubtask );

static zSHORT
zwfnTZDMUPDD_InitMenuDisableAll( zVIEW vSubtask );

static zSHORT
zwfnTZDMUPDD_ActivateDomain( zVIEW vSubtask,
                             zVIEW vDomainGrp );

static zSHORT
zwfnTZDMUPDD_CreateOperation( zVIEW   vSubtask,
                              zVIEW   vTarget,
                              zVIEW   vSource,
                              zPCHAR  szOperationName,
                              zSHORT  nMoveFlag );


////////////////////////////////////////////////////////////////////////
//  OPERATION:  zwTZDMUPDD_InitTool1
//  PURPOSE:    This Entry prepares the environment upon entry
//              to the ER Dialog.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_InitTool1( zVIEW vSubtask )
{
   zVIEW    vCM_List;
   zVIEW    vCM_ListGroup;
   zVIEW    vSaveAs;
   zVIEW    vT;
   zSHORT   nRC;

   nRC = oTZ__PRFO_GetViewToProfile( &vT, "DM", vSubtask, zCURRENT_OI );
   if ( nRC >= 0 )
   {
      SetNameForView( vT, "ProfileXFER", vSubtask, zLEVEL_TASK );
      SetNameForView( vT, "MSG", vSubtask, zLEVEL_TASK );
      // Initialize New Domain Group Flag.
      SetAttributeFromString( vT, "DM", "NewDomainGroupFlag", "" );
   }

   if ( InitializeDefaultLPL( vSubtask ) < 0 )
      return( -1 );

   // Always have a Named List of Meta's.
   // This is done to be consistent with other dialogs.  The only time the
   // that the named List will not exist is during a SwitchLPLR.
   // This fact is used a "flag" to the RtnFromSubWnd logic.
   if ( RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META ) > 0 )
      SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

   // Also get a CM_ListGroup View to the Domain Group.  Thus there will be two lists, one for the
   // Domains themselves and one for the Domain Groups.
   nRC = GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
       if ( RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_DOMAINGRP_META ) > 0 )
         SetNameForView( vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   }

   // if Domain not checked out and user change this Domain, then zeidon
   // call the window "Save Domain as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Domain or Open an other Domain or Switch Project or
   // Exit Domain Tool)
   if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vSaveAs, "TZSAVEAS", vSubtask, zSINGLE );
      SetNameForView( vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
      CreateEntity( vSaveAs, "ActionAfterSaveAS", zPOS_AFTER );
   }
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   return( 0 );
}


////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_InitTool2
//   PURPOSE:    This Entry switches to a NEW screen if no EMD loaded.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_InitTool2( zVIEW vSubtask )
{
   zVIEW  vDomainGrp;
   zVIEW  vDomainGrpDetail;
   zVIEW  vCM_List;
   zSHORT nRC;

   // Is Domain Activated for mapping??
   nRC = GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {    /* no - go to OPEN window */
      if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) > 0 )
      {
         SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                  "TZDMUPDD", "DOMLIST" );
      }
   }
   else
   {
      // create the view for group detail window
      CreateViewFromViewForTask( &vDomainGrpDetail, vDomainGrp, 0 );
      SetNameForView( vDomainGrpDetail, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Domain Maintenance", "TZDGSRCO",
                               vDomainGrp, "Domain", zSOURCE_DOMAINGRP_META );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_MainRtnFromSubWnd
//   PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//               the current EMD.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vCM_ListGroup;
   zVIEW  vDomainGrp;
   zVIEW  vDomainGrpDetail;
   zSHORT nRC;
   zSHORT nRC_Grp;
   zSHORT nRC_List;

   // DonC Note, 8/18/95:  There is also a ListRtnFromSubWnd routine.  I don't know why we
   // need both.

   // If we went to a normal subwindow, views "CM_List" and "TZDGSRCO" will
   // both exist.
   // If we went to SwitchLPLR and canceled, then view "TZDGSRCO" will exist
   // but view "CM_List" will not.
   // If we went to SwitchLPLR and switched to a new LPLR, view "TZDGSRCO"
   // will not exist.

   // Is Domain List Available or we lost our Domain view??
   nRC_List = GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   nRC_Grp = GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   if ( nRC_List < 1 )
   {
      // Case is "Return from SwitchLPLR."

      // Now get a new List view and name it
      if ( RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META ) > 0 )
         SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

      // Also get the list of Domain Groups
      nRC = GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         if ( RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_DOMAINGRP_META ) > 0 )
            SetNameForView( vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
      }
   }

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // create the view for group detail window
      CreateViewFromViewForTask( &vDomainGrpDetail, vDomainGrp, 0 );
      SetNameForView( vDomainGrpDetail, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   }

   // Name this window for future use.
   SetNameForView( vSubtask, "TZDMUPDD_MAIN", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Domain Maintenance", "TZDGSRCO",
                               vDomainGrp, "Domain", zSOURCE_DOMAINGRP_META );

   //BL, 1999.10.21 Refresh all Lists from menu <View> (=Modeless Subwindows)
   RefreshAllSubwindows( vSubtask );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_ListRtnFromSubwnd
//   PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//               the current EMD.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_ListRtnFromSubwnd( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vCM_ListGroup;
   zVIEW  vDomainGrp;
   zSHORT nRC;
   zSHORT nRC2;

// Is Domain List Available or we lost our Domain view??
   nRC = GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   nRC2 = GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 || nRC2 < 1 )
   {
      // no - return from SwitchLPLR
      // First, Drop the current Domain view if there
      if ( nRC2 > 0 )
      {
         nRC2 = DropView( vDomainGrp );
      }

      // Now get a new List view and name it
      if ( RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META ) > 0 )
         SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

      // Also get the list of Domain Groups
      if ( RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_DOMAINGRP_META ) > 0 )
         SetNameForView( vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );

      //BL, 1999.10.21
      RefreshAllSubwindows( vSubtask );
      //  There used to be a check here for a single Domain, that would go to the update window
      //  instead of the List window, but that has been removed for simplification.
   }

   return( 0 );
}

static zSHORT
zwfnTZDMUPDD_AskForSave_YES( zVIEW  vSubtask,
                             zBOOL  nSaveAs,
                             zVIEW  vDomainGrp )
{
   zSHORT  nRC = 0;

   // If Domain not checked out and user would save it, then call
   // window save Domain as.
   if ( nSaveAs )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZDMUPDD", "SaveAs" );
      return( 99 );
   }

   // We will remap everything here to make sure we have all the
   // data from the window.
   if ( MapWindow( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Make sure any Contexts are ordered by Name.
   if ( CheckExistenceOfEntity( vDomainGrp, "Context" ) >= zCURSOR_SET )
      OrderEntityForView( vDomainGrp, "Context", "Name A" );

   nRC = zwfnTZDMUPDD_SaveDomain( vSubtask, vDomainGrp );
   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   else
   {
      // BL, 1999.10.21
      RefreshAllSubwindows( vSubtask );
   }

   return( nRC );
}

static zSHORT
zwfnTZDMUPDD_AskForSave( zVIEW  vSubtask,
                         zVIEW  vDomainGrp )
{
   zSHORT  nRC;
   zBOOL   nSaveAs = FALSE;
   zCHAR   szDomainName[ 33 ];
   zCHAR   szMessageText[ 254 ];

   // If Domain Group not checked out, then ask for save as.
   if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ) )
   {
      nSaveAs = TRUE;
      GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vDomainGrp, "Domain", "Name" );
      strcpy_s( szMessageText, sizeof( szMessageText ), "Domain Group is not checked out, but Domain '" );
      strcat_s( szMessageText, sizeof( szMessageText ), szDomainName );
      strcat_s( szMessageText, sizeof( szMessageText ), "' has been modified. \n\nWould you like to " );
      strcat_s( szMessageText, sizeof( szMessageText ), "save it with differend name?" );
      nRC = MessagePrompt( vSubtask, "DM00115", "Domain Maintenance",
                           szMessageText, zBEEP, zBUTTONS_YESNOCANCEL,
                           zRESPONSE_YES, zICON_QUESTION );
   }
   else
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_DOMAIN,
                          zREFER_DOMAIN_META,
                          vDomainGrp, "Domain", "Name", 0 );

   if ( nRC == zRESPONSE_YES )
      nRC = zwfnTZDMUPDD_AskForSave_YES( vSubtask, nSaveAs, vDomainGrp );
   else
      if ( nRC == zRESPONSE_NO )
      {
         if ( CancelSubobject( vDomainGrp, "Domain" ) != 0 )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }
         return( 0 );
      }
      else
         if ( nRC == zRESPONSE_CANCEL )
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   return( nRC );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  AskForSave
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_AskForSave( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zCHAR    szControlText[ 61 ];
   zSHORT   nRC;
   zSHORT   nRC2;

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
   {
      // There is a mapping problem here because of the controls, DataType
      // and DomainType.  We don't want to map those controls prior to the
      // "ask for save" because they will present an error message if they]
      // are null, which is valid if the operator is not going to save the
      // Domain.  On the other hand, we would like to know whether or not
      // those fields have changed, because we won't put up the "ask message"
      // if no changes in the Domain Group have been made.

      // Our solution will be to map all the controls on the window TZDMUPDD
      // individually.  We will check DataType and DomainType to see if they
      // are null and will map them if they are not.

      // KJS - 06/25/15 - I took off the "Not Null" for DataType and DomainType because of an KZOE
      // error we recieved when hitting "Next" or "Prev". We will prompt the user if these fields
      // do not have value.

      MapCtrl( vSubtask, "Restricted" );
      MapCtrl( vSubtask, "Operation" );
      MapCtrl( vSubtask, "DomainName" );
      MapCtrl( vSubtask, "Desc" );
      GetCtrlText( vSubtask, "DataType", szControlText, 60 );
      if ( szControlText[ 0 ] != 0 )
         MapCtrl( vSubtask, "DataType" );
      else
      {
         MessageSend( vSubtask, "DM00101", "Domain Maintenance",
                      "Please enter a Data Type.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "DataType" );
         return( -1 );
      }
      GetCtrlText( vSubtask, "DomainType", szControlText, 60 );
      if ( szControlText[ 0 ] != 0 )
         MapCtrl( vSubtask, "DomainType" );
      else
      {
         MessageSend( vSubtask, "DM00101", "Domain Maintenance",
                      "Please enter a Domain Type.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "DomainType" );
         return( -1 );
      }

      nRC = MapCtrl( vSubtask, "DecimalFormat" );
      nRC2 = MapCtrl( vSubtask, "MaxString" );

      // If either DecimalFormat or MaxString are in error, abort
      // processing.
      if ( nRC > 0 || nRC2 > 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }


      if ( MiGetUpdateForView( vDomainGrp ) == 1  &&
           ObjectInstanceUpdatedFromFile( vDomainGrp ) == 1 )
         nRC = zwfnTZDMUPDD_AskForSave( vSubtask, vDomainGrp );
      else
         nRC = 0;
   }
   else
      return( zRESPONSE_CANCEL );

   return( nRC );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_SwitchLPLR
//   PURPOSE:    This Entry switches to a new LPLR.  All named views to
//               the current LPLR file List must be droped first.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SwitchLPLR( zVIEW vSubtask )
{
   zVIEW  vT;
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwTZDMUPDD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Domain not checked out and user change this Domain, then zeidon
            // call the window "Save Domain as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Domain or Open an other Domain or Switch Project or
            // Exit Domain Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zSwitchLPLRAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   if ( GetViewByName( &vT, "CM_List", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vT );

   if ( GetViewByName( &vT, "CM_ListGroup", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vT );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_ExitSaveDomain
//   PURPOSE:    This Entry File Menu EXIT
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_ExitSaveDomain( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vSaveAs;
   zSHORT   nRC;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   nRC = zwTZDMUPDD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( vSaveAs )
      {
         if ( nRC == 99 )
         {
            // if Domain not checked out and user change this Domain, then zeidon
            // call the window "Save Domain as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Domain or Open an other Domain or Switch Project or
            // Exit Domain Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zExitToolAfterSaveAs );
         }
         else
         {
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
         }
      }

      return( 0 );
   }

   if ( vSaveAs )
      DropMetaOI( vSubtask, vSaveAs );

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
      DropMetaOI( vSubtask, vDomainGrp );

   TerminateLPLR( vSubtask );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_SaveDomain
//   PURPOSE:    This Entry File Menu SAVE
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SaveDomain( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vWindow;
   zCHAR    szType[ 2 ];
   zLONG    lMaxStringLength;
   zSHORT   nRC;

   nRC = GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   // if no domain, just exit...
   if ( nRC < 1 )
      return( 0 );

   nRC = GetStringFromAttribute( szType, sizeof( szType ), vDomainGrp, "Domain", "DomainType" );
   if ( szType[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "DM00101", "Domain Maintenance",
                   "Please enter a Domain Type.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "DomainType" );
      return( -1 );
   }

   nRC = GetStringFromAttribute( szType, sizeof( szType ), vDomainGrp, "Domain", "DataType" );
   if ( szType[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "DM00101", "Domain Maintenance",
                   "Please enter a Data Type.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "DataType" );
      return( -1 );
   }
   GetIntegerFromAttribute( &lMaxStringLength, vDomainGrp, "Domain", "MaxStringLth" );
   if ( *szType == 'S' && lMaxStringLength < 1 )
   {
      MessageSend( vSubtask, "DM00101", "Domain Maintenance",
                   "A String Domain must have a max length specified.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Accept the Domain.
   AcceptSubobject( vDomainGrp, "Domain" );

   nRC = zwfnTZDMUPDD_SaveDomain( vSubtask, vDomainGrp );
   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   RefreshAllSubwindows( vSubtask );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DOMGRPLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZDMUPDD_SaveDomain( zVIEW vSubtask, zVIEW vDomainGrp )
{
   zVIEW    vCM_List;
   zVIEW    vDomainGrpTmp;
   zSHORT   nRC;
   zLONG    lDomainZKey;
   zCHAR    szLanguageType[ 4 ];

   // if Domain from Type "Format", check Context EditString
   if ( oTZDMSRCO_CheckDomainType( vDomainGrp ) < 0 )
      return( -4 );

   // If there is no table entry with a null internal value, create one
   // KJS 08/21/14 - We are not sure we need this now (we delete the null value when
   // opening the domain, so I am commenting this out but leaving the code here just
   // in case we run into issues.
   //zwTZDMUPDD_SetNullEntry( vSubtask, vDomainGrp );

   // Make sure that the Subdirectory attribute is null and that the
   // Extension attribute is set according to the Language type.
   SetAttributeFromString( vDomainGrp, "DomainGroup", "Subdirectory", "" );
   GetStringFromAttribute( szLanguageType, sizeof( szLanguageType ),
                           vDomainGrp, "DomainGroup", "LanguageType" );
   if ( szLanguageType[ 0 ] == 'V' )
      SetAttributeFromString( vDomainGrp, "DomainGroup", "Extension", "VML" );
   else
      SetAttributeFromString( vDomainGrp, "DomainGroup", "Extension", "C" );

   // Since AssignZKeyToMetaOI modifies position, create a temporary
   // view to be used by those routines.
   CreateViewFromViewForTask( &vDomainGrpTmp, vDomainGrp, 0 );
   AssignZKeyToMetaOI( vSubtask, vDomainGrpTmp, zSOURCE_DOMAIN_META, 0 );
   DropView( vDomainGrpTmp );

   nRC = GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

   if ( oTZDMSRCO_UniqueDomainName( vDomainGrp, vCM_List ) == 0 )
   {
      MessageSend( vSubtask, "DM00101", "Domain Maintenance",
                   "Domain Name is not unique.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   nRC = GetIntegerFromAttribute( &lDomainZKey,
                                  vDomainGrp, "Domain", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                        "CPLR_ZKey", lDomainZKey, "" );

   // If the Domain Name is being modified, see if there is a Context by
   // the same name, and if so, change it as well.
   // We are updating a Domain if CM_List.W_MetaDef.CPLR_ZKey = the Domain
   // ZKey.  We compare the CM_List.W_MetaDef.Name value to the Domain name
   // to see if the Domain name has changed.
   // However, we will only do this check if W_MetaDef exists, which it might not for the first Domain.
   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
   {
      if ( CompareAttributeToAttribute( vCM_List,   "W_MetaDef", "CPLR_ZKey",
                                        vDomainGrp, "Domain",    "ZKey" ) == 0 )
      {
         if ( CompareAttributeToAttribute( vCM_List,   "W_MetaDef", "Name",
                                           vDomainGrp, "Domain",    "Name" ) != 0 )
         {
            // Domain Name has changed, so change Context Name as well, if there
            // is one by the old name.
            nRC = SetCursorFirstEntityByAttr( vDomainGrp, "Context",   "Name",
                                              vCM_List,   "W_MetaDef", "Name", 0 );
            if ( nRC >= zCURSOR_SET )
            {
               SetAttributeFromAttribute( vDomainGrp, "Context", "Name",
                                          vDomainGrp, "Domain",  "Name" );
            }
         }
      }
   }

   // Since CommitMetaOI modifies position, create a temporary
   // view to be used by that routine.
   CreateViewFromViewForTask( &vDomainGrpTmp, vDomainGrp, 0 );
   if ( CommitMetaOI( vSubtask, vDomainGrpTmp, zSOURCE_DOMAINGRP_META ) <= 0 )
   {
      DropView( vDomainGrpTmp );
      MessageSend( vSubtask, "DM00115", "Domain Maintenance",
                   "Unable to save Domain Group.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   DropView( vDomainGrpTmp );

   if ( oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, vDomainGrp, 1 ) < 0 )
      return( -3 );

   return( 0 );
}

// If Attribute "ContextRestriction" for Entity "Domain" not "Y" or "N",
// then zeidon ask for save. This is a work around for old Domains
static zSHORT
zwfnTZDMUPDD_ActivateDomain( zVIEW vSubtask,
                             zVIEW vDomainGrp )
{
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   if ( CompareAttributeToString( vDomainGrp, "Domain",
                                  "ContextRestriction", "" ) == 0 )
   {
      SetAttributeFromString( vDomainGrp, "Domain",
                              "ContextRestriction", "N" );
   }

   zwTZDMUPDD_CONTEXT_INIT( vSubtask );

   // remove update flag
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vDomainGrp->hViewCsr );
   lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   lpViewOI->bUpdatedFile = FALSE;

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_ActivateDomain( zVIEW  vSubtask )
{
   zVIEW    vCM_List;
   zVIEW    vCM_List2;
   zVIEW    vDomainGrp;
   zVIEW    vDomainGrpA;
   zVIEW    vTaskLPLR;
   zLONG    lRC;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_ANY );

   // Use a second CM_List view for Activate since Activate will alter
   // its position.
   CreateViewFromViewForTask( &vCM_List2, vCM_List, 0 );

   lRC = ActivateMetaOI( vSubtask, &vDomainGrpA, vCM_List2,
                         zSOURCE_DOMAIN_META,
                         zSINGLE | zLEVEL_APPLICATION );
   DropView( vCM_List2 );

   if ( lRC >= 0 )
   {
      if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
         DropMetaOI( vSubtask, vDomainGrp );
      SetNameForView( vDomainGrpA, "TZDGSRCO", vSubtask, zLEVEL_TASK );

      // If Attribute "ContextRestriction" for Entity "Domain" not "Y" or "N",
      // then zeidon ask for save. This is a work around for old Domains
      zwfnTZDMUPDD_ActivateDomain( vSubtask, vDomainGrpA );

      // if DomainGroup not checked out, set  View read only ->
      // the user cannot update the values in Detail Windows
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrpA, zSOURCE_DOMAINGRP_META ))
         SetViewReadOnly( vDomainGrpA );

      // Position on the blank entry in DomainJavaClass
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
         SetCursorFirstEntity( vTaskLPLR, "DomainJavaClass", "" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CreateNewDomain( zVIEW    vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vDomainGrp;
   zVIEW    vCM_ListGroup;
   zLONG    lRC;
   zCHAR    szTempString[ 33 ];
   zSHORT   nZRetCode;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   nZRetCode = GetStringFromAttribute( szTempString, sizeof( szTempString ),
                                       vProfileXFER, "DM", "DomainName" );
   if ( szTempString[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "DM00103", "Domain Maintenance",
                   "Domain name required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh,
                                 0, 0 );
      SetFocusToCtrl( vSubtask, "edDomainName" );
      return( -1 );
   }

   lRC = CheckExistenceOfMetaOI( vSubtask, szTempString, zSOURCE_DOMAIN_META );
   if ( lRC == 1 )
   {
      MessageSend( vSubtask, "DM00104", "Domain Maintenance",
                   "A Domain already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh,
                                 0, 0 );
      SetFocusToCtrl( vSubtask, "edDomainName" );
      return( -1 );
   }
   else
   {
      if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
         DropMetaOI( vSubtask, vDomainGrp );

      // We are positioned on the Domain Group selected.  Activate the Domain
      // Group by that name.

      lRC = ActivateMetaOI( vSubtask, &vDomainGrp, vCM_ListGroup, zSOURCE_DOMAINGRP_META, zSINGLE );
      if ( lRC < 0 )
         return( -1 );

      CreateMetaEntity( vSubtask, vDomainGrp, "Domain", zPOS_AFTER );
      SetAttributeFromAttribute( vDomainGrp, "Domain", "Name",
                                  vProfileXFER, "DM", "DomainName" );
      SetAttributeFromAttribute( vDomainGrp, "Domain", "Desc",
                                  vProfileXFER, "DM", "DomainDesc" );
      SetNameForView( vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );
      CreateMetaEntity( vSubtask, vDomainGrp, "Context", zPOS_AFTER );
      SetAttributeFromAttribute( vDomainGrp, "Context", "Name",
                                  vProfileXFER, "DM", "DomainName" );
      SetAttributeFromString( vDomainGrp, "Context", "IsDefault", "Y" );

      // KJS 06/23/15 - If the user does not select a DomainType or DataType, an internal IssueOE_Error is given when saving (because these fields are not null),
      // that to a new user would probably be confusing. To this end, I am going to set a default value of "Table" and "String",
      // so that the error doesn't pop up.
      // But now I see that if I set this, for some reason when we go to the AskForSave code, the object is not marked
      // as updated so we never ask for the save and this domain might not get saved. Don't understand why that is so
      // for now we will have to put up with the non user friendly message.
      //SetAttributeFromString( vDomainGrp, "Domain", "DomainType", "T" );
      //SetAttributeFromString( vDomainGrp, "Domain", "DataType", "S" );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZDMUPDD_DeleteFromXLP
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwfnTZDMUPDD_DeleteFromXLP( zVIEW  vCM_List,
                            zULONG ulZKey )
{
   zVIEW vCM_List_Copy;

   // if the meta was not found,
   // we nevertheless want to delete it from XLP.

   // Now we must remove the Domain MetaDef entity from zSOURCE_DOMAIN_META
   // and zREFER_DOMAIN_META.  We are currently positioned on zREFER.
   // Most of the time, this seems to be done by CM, so we'll only do it if
   // the entries still remain.

   CreateViewFromViewForTask( &vCM_List_Copy, vCM_List, 0 );

   if ( SetCursorFirstEntityByInteger( vCM_List_Copy, "W_MetaDef", "CPLR_ZKey",
                                       ulZKey, 0 ) >= 0 )
   {
      DeleteEntity( vCM_List_Copy, "W_MetaDef", zREPOS_PREV );
   }

   // Now do the same for zSOURCE.
   SetCursorFirstEntityByInteger( vCM_List_Copy, "W_MetaType", "Type",
                                  zSOURCE_DOMAIN_META, 0 );
   if ( SetCursorFirstEntityByInteger( vCM_List_Copy, "W_MetaDef", "CPLR_ZKey",
                                       ulZKey, 0 ) >= 0 )
   {
      DeleteEntity( vCM_List_Copy, "W_MetaDef", zREPOS_PREV );
   }

   DropView( vCM_List_Copy );

   SetCursorPrevEntity( vCM_List, "W_MetaDef", "" );

   return( 0 );
} // zwfnTZDMUPDD_DeleteFromXLP


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZDMUPDD_PromptForDelete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwfnTZDMUPDD_PromptForDelete( zVIEW  vSubtask,
                              zVIEW  vTZDGSRCO,
                              zSHORT nPromptForDelete,
                              zSHORT nDeleteOperationCode )
{
   zVIEW   vTZDGSRCO_Copy;
   zSHORT  nRC = zRESPONSE_NO;
   zCHAR   szOperationName[ 33 ];
   zCHAR   szMsg[ 100 ];

   // If save as, no Prompt for delete
   if ( nPromptForDelete == 1 )
   {
      // Prompt for Delete
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DOMAIN,
                          zREFER_DOMAIN_META, vTZDGSRCO, "Domain", "Name", 0 );
      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   // Has Domain a Operation?
   if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) < zCURSOR_SET )
      return( 0 );

   GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vTZDGSRCO, "SelectedOperation", "Name" );

   // if Operation assigned an other Domain?
   CreateViewFromViewForTask( &vTZDGSRCO_Copy, vTZDGSRCO, 0 );
   SetCursorFirstEntityByString( vTZDGSRCO_Copy, "SelectedOperation",
                                 "Name", szOperationName, "DomainGroup" );
   nRC = SetCursorNextEntityByString( vTZDGSRCO_Copy, "SelectedOperation",
                                      "Name", szOperationName, "DomainGroup" );
   DropView( vTZDGSRCO_Copy );
   if ( nRC >= zCURSOR_SET )
      return( 0 );

   nRC = zRESPONSE_NO;
   if ( nPromptForDelete == 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Delete assigned Domain Operation '" );
      strcat_s( szMsg, sizeof( szMsg ), szOperationName );
      strcat_s( szMsg, sizeof( szMsg ), "' from Source File?" );

      nRC = MessagePrompt( vSubtask, "OP00810", "Domain Maintenance",
                           szMsg, 0, zBUTTONS_YESNO, zRESPONSE_YES,
                           zICON_QUESTION );
   }

   if ( nDeleteOperationCode == 1 &&
        ( nPromptForDelete == 0 || nRC == zRESPONSE_YES ) )
   {
      return( 1 );
   }

   return( 0 );
} // zwfnTZDMUPDD_PromptForDelete

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZDMUPDD_DeleteCode
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwfnTZDMUPDD_DeleteCode( zVIEW vSubtask,
                         zVIEW vTZDGSRCO )
{
   zCHAR szOperationName[ 33 ];

   if ( SetCursorFirstEntityByEntityCsr( vTZDGSRCO, "Operation",
                                         vTZDGSRCO, "SelectedOperation",
                                         "" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vTZDGSRCO,
                               "SelectedOperation", "Name" );

      // Create Work LOD for Delete Operation from File
      zwTZDMUPDD_CreateDeleteWorkLod( vSubtask, vTZDGSRCO, szOperationName );
      // delete Operation Code from Target Group
      zwTZDMUPDD_DeleteOperFromFile( vSubtask, vTZDGSRCO, zDELETE_OPERATION );

      DeleteEntity( vTZDGSRCO, "Operation", zREPOS_NEXT );
   }
   return( 0 );
} // zwfnTZDMUPDD_DeleteCode

zSHORT
zwfnTZDMUPDD_DeleteDomain( zVIEW  vSubtask,
                           zSHORT nPromptForDelete,
                           zSHORT nDeleteOperationCode )
{
   zVIEW    vCM_List;
   zVIEW    vCM_List2;
   zVIEW    vDomainGrp;
   zVIEW    vDomainGrpCur;
   zVIEW    vDomainGrpOrig;
   zVIEW    vParentWindow;
   zSHORT   nRC;
   zSHORT   nDeleteCode;
   zULONG   ulZKey;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_ANY );
   // Get the ZKey for later delete of W_MetaDef entries.
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vCM_List,
                            "W_MetaDef", "CPLR_ZKey" );

   // Create a second view since the Activate will loose position on
   // the CM_List view passed in to it.
   CreateViewFromViewForTask( &vCM_List2, vCM_List, 0 );

   nRC = ActivateMetaOI( vSubtask, &vDomainGrp, vCM_List2,
                         zSOURCE_DOMAIN_META,
                         zSINGLE | zLEVEL_APPLICATION );
   DropView( vCM_List2 );

   if ( nRC <= 0 )
   {
      // If the meta was not found, we nevertheless want to delete it from XLP.
      zwfnTZDMUPDD_DeleteFromXLP( vCM_List, ulZKey );
      return( -1 );
   }

   // Prompt for Delete
   nDeleteCode = zwfnTZDMUPDD_PromptForDelete( vSubtask, vDomainGrp, nPromptForDelete,
                                               nDeleteOperationCode );
   if ( nDeleteCode < 0 )
      return( 0 );

   // If the DomainGroup just activated is the same as the one
   // currently in memory, use the one in memory.
   vDomainGrpOrig = 0;
   if ( GetViewByName( &vDomainGrpCur, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 &&
        CompareAttributeToAttribute( vDomainGrpCur, "Domain", "ZKey",
                                     vDomainGrp,    "Domain", "ZKey" ) == 0 )
   {
      SetCursorFirstEntityByAttr( vDomainGrpCur, "Domain", "ZKey",
                                  vDomainGrp,    "Domain", "ZKey", 0 );
      vDomainGrpOrig = vDomainGrp;
      vDomainGrp     = vDomainGrpCur;
   }

   oTZDMSRCO_RemoveDomainFromXDM( vSubtask, vDomainGrp );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vDomainGrp, "Domain", "ZKey" );

   if ( nDeleteCode == 1 )
      zwfnTZDMUPDD_DeleteCode( vSubtask, vDomainGrp );

   DeleteEntity( vDomainGrp, "Domain", zREPOS_NEXT );
   CommitMetaOI( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META );

   if ( vDomainGrpOrig )
      DropObjectInstance( vDomainGrpOrig );

   DropView( vDomainGrp );
   if ( GetViewByName( &vParentWindow, "TZDMUPDD_MAIN", vSubtask, zLEVEL_TASK ) > 0 )
   {
      RefreshWindow( vParentWindow );
      RefreshAllSubwindows( vParentWindow );
   }

   // Delete Domain from XLP.
   zwfnTZDMUPDD_DeleteFromXLP( vCM_List, ulZKey );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteDomain( zVIEW vSubtask )
{
   zSHORT  nRC;

   nRC = zwfnTZDMUPDD_DeleteDomain( vSubtask, 1, 1 );

   return( nRC );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_ContextPopup( zVIEW    vSubtask )
{

   zwTZDMUPDD_LoadPopup( vSubtask,
                         "Context",
                         "AddContext",
                         "DeleteContext",
                         "ContextPopup" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SetDefaultContext( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vDomainGrpTmp;
   zSHORT   nRC;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vDomainGrpTmp, vDomainGrp, 0 );

   for ( nRC = SetCursorFirstEntityByString( vDomainGrpTmp, "Context",
                                             "IsDefault", "Y", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByString( vDomainGrpTmp, "Context",
                                            "IsDefault", "Y", 0 ) )
   {
      SetAttributeFromString( vDomainGrpTmp, "Context", "IsDefault", "" );
   }

   DropView( vDomainGrpTmp );

   SetAttributeFromString( vDomainGrp, "Context", "IsDefault", "Y" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteContext( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zSHORT   nRC;
   zSHORT   nCloseWindow = 0;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   // first check if some context is marked as the default
   if ( zCURSOR_SET == SetEntityCursor( vDomainGrp, "Context", "IsDefault",
                                        zPOS_FIRST | zQUAL_EQUAL |
                                        zQUAL_STRING | zTEST_CSR_RESULT,
                                        "Y",
                                        0, 0, 0, 0, 0 ) )
   {
      if ( 0 == CompareAttributeToString( vDomainGrp, "Context",
                                          "IsDefault", "Y" ) )
      {
         MessageSend( vSubtask, "DM00105", "Domain Maintenance",
                      "Unable to Delete Default Context for Domain",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
   }
   else
   {
      if ( CompareAttributeToAttribute( vDomainGrp, "Context", "Name",
                                        vDomainGrp, "Domain", "Name" ) == 0 )
      {
         MessageSend( vSubtask, "DM00106", "Domain Maintenance",
                      "Unable to Delete Default Context for Domain",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
   }

   // Prompt for Delete
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DOMAIN,
                       zENAME_DOMAIN_CONTEXT,
                       vDomainGrp, "Context", "Name", 0 );

   if ( nRC == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CONTEXT" ) >= 0 )
      nCloseWindow = 1;

   if ( DeleteEntity( vDomainGrp, "Context", zREPOS_PREV ) == zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDomainGrp, "Context", 1, 1 );

   if ( nCloseWindow == 1 &&
        CheckExistenceOfEntity( vDomainGrp, "Context" ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }
   else
   {
      if ( nCloseWindow == 1 )
      {
         SetFocusToCtrl( vSubtask, "ContextName" );
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DuplicateContext( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vDomainGrpT;
   zSHORT   nRC;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Duplicate the Context structure.
   CreateViewFromView( &vDomainGrpT, vDomainGrp);
   CreateMetaEntity( vSubtask, vDomainGrpT, "Context", zPOS_AFTER );
   SetAttributeFromAttribute( vDomainGrpT, "Context", "Name",
                              vDomainGrp,  "Context", "Name" );
   for ( nRC = SetCursorFirstEntity( vDomainGrp, "TableEntry", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDomainGrp, "TableEntry", "" ) )
   {
      CreateMetaEntity( vSubtask, vDomainGrpT, "TableEntry", zPOS_AFTER );
      SetMatchingAttributesByName( vDomainGrpT, "TableEntry",
                                   vDomainGrp,  "TableEntry", zSET_NULL );
   }
   DropView( vDomainGrpT );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_GoToUpdateTableEntry( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;

   // We're not using auto object functionality here because we need to
   // switch to the window for table integers if the DataType is integer.

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   CreateTemporalSubobjectVersion( vDomainGrp, "TableEntry" );

   // If the data type of the InternalValue is integer, init the value of the
   // work attribute to 0 and switch to the TABLEENT_Integer window.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType", "L" ) == 0 )
   {
      // If DomainGroup not checked out, View is read only. To set a attribute
      // View has to be updateble. After change attribute set View read only again
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
      {
         SetViewUpdate( vDomainGrp );
         SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric", "" );
         SetViewReadOnly( vDomainGrp );
      }
      else
         SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric", "" );

      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZDMUPDD", "TABLEENT_Integer" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_GoToNewTableEntry( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;

   // We're not using auto object functionality here because we need to create
   // a meta entity with ZKey.
   // We also need to switch to the window for table integers if the DataType
   // is integer.

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vDomainGrp, "TableEntry" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vDomainGrp, "TableEntry", 0 );

   CreateTemporalMetaEntity( vSubtask, vDomainGrp, "TableEntry", zPOS_AFTER );

   SetSelectStateOfEntity( vDomainGrp, "TableEntry", 1 );
   RefreshWindow( vSubtask );

   // If the data type of the InternalValue is integer, init the value of the
   // work attribute to 0 and switch to the TABLEENT_Integer window.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType",
                                  "L" ) == 0 )
   {
      SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric", "" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZDMUPDD", "TABLEENT_Integer" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_AddTableEntry( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vParentWindow;
   zCHAR    szInternalValue[ 254 ];

   // We're not using auto object functionality here because we need to create
   // a meta entity with ZKey.

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Accept the current Table  Entry before creating a new one, if it exists.
   if ( CheckExistenceOfEntity( vDomainGrp, "TableEntry" ) >= zCURSOR_SET )
   {
      // If the data type of the InternalValue is integer, copy the value from
      // the work attribute to the InternalValue.
      if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType",
                                    "L" ) == 0 )
      {
         GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ),
                                 vDomainGrp, "DomainGroup", "WorkNumeric" );
         SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                                 szInternalValue );
      }
      if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "TableEntry",
                                       "InternalValue", "Internal Value" ) < 0 )
      {
         SetFocusToCtrl( vSubtask, "InternalName" );
         return( -1 );
      }
      SetSelectStateOfEntity( vDomainGrp, "TableEntry", 0 );
   }

   CreateTemporalMetaEntity( vSubtask, vDomainGrp, "TableEntry", zPOS_AFTER );
   SetSelectStateOfEntity( vDomainGrp, "TableEntry", 1 );

   // If the data type of the InternalValue is integer, init the value of the
   // work attribute to 0.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType",
                                  "L" ) == 0 )
   {
      SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                              "" );
   }

   GetParentWindow ( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   // Set the cursor focus to first edit box on window.
   SetFocusToCtrl( vSubtask, "InternalName" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteTableEntry( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zSHORT   nRC;
   zSHORT   nCloseWindow = 0;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Prompt for Delete
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DOMAIN,
                       zENAME_DOMAIN_TABLEITEM,
                       vDomainGrp, "TableEntry", "InternalValue", 0 );

   if ( nRC == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TABLEENT" ) >= 0 ||
        GetSubtaskForWindowName( vSubtask, &vWindow, "TABLEENT_Integer" ) >= 0 )
   {
      nCloseWindow = 1;
   }

   if ( DeleteEntity( vDomainGrp, "TableEntry", zREPOS_NEXT ) == zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDomainGrp, "TableEntry", 1, 1 );

   if ( nCloseWindow == 1 &&
        CheckExistenceOfEntity( vDomainGrp, "TableEntry" ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }
   else
   {
      if ( nCloseWindow == 1 )
      {
         SetFocusToCtrl( vSubtask, "InternalName" );
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CONTEXT_INIT( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vTZDGSRCO_Copy;
   zSHORT   RESULT;
   zSHORT   nRC;

   //  If a null table entry exists(See zwTZDMUPDD_SetNullEntry for how and why),
   //  delete it here so it won't show up in table List Box.

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vTZDGSRCO_Copy, vDomainGrp, 0 );

   //  If there is no table entry with a null internal value, create
   //  one with both null internal and external value.  This is so that
   //  the Domain table handler will give a blank entry to the Combo
   //  Box control.
   //  KJS 08/21/14 - When saving the domain, we used to create a null entry
   //  in zwTZDMUPDD_SetNullEntry(), but we don't think we need that,
   //  but we will keep this code for now.
   for ( nRC = SetCursorFirstEntity( vTZDGSRCO_Copy, "Context", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZDGSRCO_Copy, "Context", "" ) )
   {
      RESULT = SetCursorFirstEntityByString( vTZDGSRCO_Copy, "TableEntry",
                                             "InternalValue", "", "" );
      if ( RESULT >= 0 )
      {
         if ( CompareAttributeToString( vTZDGSRCO_Copy, "TableEntry",
                                        "ExternalValue", "" ) == 0 )
            DeleteEntity( vTZDGSRCO_Copy, "TableEntry", zREPOS_NONE );
      }
   }
   DropView( vTZDGSRCO_Copy );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CONTEXT_PINIT( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zBOOL    bReadOnly;

   //  If a null table entry exists(See CONTEXT_OK for how and why),
   //  delete it here so it won't show up in table List Box.
   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   bReadOnly = !(zBOOL) MiGetUpdateForView( vDomainGrp );
   if ( bReadOnly )
   {
      // if in read only mode, don't allow additions or deletions
#if TRUE
      SetCtrlState( vSubtask, "New", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, FALSE );
#else
      EnableAction( vSubtask, "AddContext", FALSE );
      EnableAction( vSubtask, "DeleteContext", FALSE );
      EnableAction( vSubtask, "AddTableEntry", FALSE );
      EnableAction( vSubtask, "DeleteTableEntry", FALSE );
#endif
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CONTEXT_BUTTON_OK( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // If read only view, just exit...
   if ( !MiGetUpdateForView( vDomainGrp ) )
      return( 0 );

   // Accept the current Context subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "Context",
                                    "Name", "Context" ) < 0 )
   {
      return( -1 );
   }

   // Make sure any Contexts are ordered by Name.
   if ( CheckExistenceOfEntity( vDomainGrp, "Context" ) >= zCURSOR_SET )
      OrderEntityForView( vDomainGrp, "Context", "Name A" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CONTEXT_BUTTON_Next( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vParentWindow;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Accept the current Context subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "Context",
                                    "Name", "Context" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vDomainGrp, "Context", 0 );
   SetCursorNextEntity( vDomainGrp, "Context", 0 );

   // Use a temporal subobject version for updating the Context subobject.
   CreateTemporalSubobjectVersion( vDomainGrp, "Context" );

   SetSelectStateOfEntity( vDomainGrp, "Context", 1 );
   SetFocusToCtrl( vSubtask, "ContextName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CONTEXT_BUTTON_Prev( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vParentWindow;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Accept the current Context subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "Context",
                                    "Name", "Context" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vDomainGrp, "Context", 0 );
   SetCursorPrevEntity( vDomainGrp, "Context", 0 );

   // Use a temporal subobject version for updating the Context subobject.
   CreateTemporalSubobjectVersion( vDomainGrp, "Context" );

   SetSelectStateOfEntity( vDomainGrp, "Context", 1 );
   SetFocusToCtrl( vSubtask, "ContextName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_TABLEENT_BUTTON_Next( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vParentWindow;
   zCHAR    szInternalValue[ 254 ];

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   // If the data type of the InternalValue is integer, copy the value from
   // the work attribute to the InternalValue.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType",
                                  "L" ) == 0 )
   {
      GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ), vDomainGrp,
                              "DomainGroup", "WorkNumeric" );
      // If DomainGroup not checked out, View is read only. To set a attribute
      // View has to be updateble. After change attribute set View read only again
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
      {
         SetViewUpdate( vDomainGrp );
         SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                                 szInternalValue );
         SetViewReadOnly( vDomainGrp );
      }
      else
         SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                                 szInternalValue );
   }

   // Accept the current Context subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "TableEntry",
                                    "InternalValue", "Internal Value" ) < 0 )
   {
      SetFocusToCtrl( vSubtask, "InternalName" );
      return( -1 );
   }

   SetSelectStateOfEntity( vDomainGrp, "TableEntry", 0 );
   SetCursorNextEntity( vDomainGrp, "TableEntry", "" );

   // If the data type of the InternalValue is integer, copy the value to the
   // work attribute of type integer.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType",
                                  "L" ) == 0 )
   {
      GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ),
                              vDomainGrp, "TableEntry", "InternalValue" );
      // If DomainGroup not checked out, View is read only. To set a attribute
      // View has to be updateble. After change attribute set View read only again
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
      {
         SetViewUpdate( vDomainGrp );
         SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                                 szInternalValue );
         SetViewReadOnly( vDomainGrp );
      }
      else
         SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                                 szInternalValue );
   }

   // Use a temporal subobject version for updating the Context subobject.
   CreateTemporalSubobjectVersion( vDomainGrp, "TableEntry" );
   SetSelectStateOfEntity( vDomainGrp, "TableEntry", 1 );

   SetFocusToCtrl( vSubtask, "InternalName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_TABLEENT_BUTTON_Prev( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vParentWindow;
   zCHAR    szInternalValue[ 254 ];

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   // If the data type of the InternalValue is integer, copy the value from
   // the work attribute to the InternalValue.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType", "L" ) == 0 )
   {
      GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ),
                              vDomainGrp, "DomainGroup", "WorkNumeric" );
      // If DomainGroup not checked out, View is read only. To set a attribute
      // View has to be updateble. After change attribute set View read only again
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
      {
         SetViewUpdate( vDomainGrp );
         SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                                 szInternalValue );
         SetViewReadOnly( vDomainGrp );
      }
      else
         SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                                 szInternalValue );
   }

   // Accept the current Context subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "TableEntry",
                                    "InternalValue", "Internal Value" ) < 0 )
   {
      SetFocusToCtrl( vSubtask, "InternalName" );
      return( -1 );
   }

   SetSelectStateOfEntity( vDomainGrp, "TableEntry", 0 );
   SetCursorPrevEntity( vDomainGrp, "TableEntry", "" );

   // If the data type of the InternalValue is integer, copy the value to the
   // work attribute of type integer.
   if ( CompareAttributeToString( vDomainGrp, "Domain", "DataType",
                                  "L" ) == 0 )
   {
      GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ),
                              vDomainGrp, "TableEntry", "InternalValue" );
      // If DomainGroup not checked out, View is read only. To set a attribute
      // View has to be updateble. After change attribute set View read only again
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
      {
         SetViewUpdate( vDomainGrp );
         SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                                 szInternalValue );
         SetViewReadOnly( vDomainGrp );
      }
      else
         SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                                 szInternalValue );
   }

   // Use a temporal subobject version for updating the Context subobject.
   CreateTemporalSubobjectVersion( vDomainGrp, "TableEntry" );
   SetSelectStateOfEntity( vDomainGrp, "TableEntry", 1 );

   SetFocusToCtrl( vSubtask, "InternalName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_REGULARE_BUTTON_Next( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zSHORT   RESULT;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   AcceptSubobject( vDomainGrp, "RegularExpression" );
   RESULT = SetCursorNextEntity( vDomainGrp, "RegularExpression", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_REGULARE_BUTTON_Prev( zVIEW    vSubtask )
{
   zVIEW    vDomainGrp;
   zSHORT   RESULT;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   AcceptSubobject( vDomainGrp, "RegularExpression" );
   RESULT = SetCursorPrevEntity( vDomainGrp, "RegularExpression", "" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CreateByCopy( zVIEW    vSubtask )
/*  Don C note from 8/14/9
    I am leaving this operation, even though it does not appear to be used at this time.
    I am leaving it in case we want to add a function to create a Domain by copying an existing Domain.
*/
{
   zVIEW    vOldDomain;
   zVIEW    vNewDomain;
   zSHORT   nRC;
   zVIEW    vProfileXFER;
   zCHAR    szDomainName[ 33 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szDomainName, sizeof( szDomainName ),
                            vProfileXFER, "DM", "DomainName" );
   nRC = CheckExistenceOfMetaOI( vSubtask, szDomainName, zSOURCE_DOMAIN_META );
   if ( nRC == 1 )
   {
      MessageSend( vSubtask, "DM00107", "Domain Maintenance",
                   "A Domain already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vOldDomain, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   ActivateEmptyMetaOI( vSubtask, &vNewDomain, zSOURCE_DOMAIN_META,
                         zSINGLE | zLEVEL_APPLICATION );

   nRC = SetNameForView( vNewDomain, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   nRC = CreateMetaEntity( vSubtask, vNewDomain, "Domain", zPOS_AFTER );
   nRC = SetMatchingAttributesByName( vNewDomain, "Domain",
                                      vOldDomain, "Domain",
                                      zSET_NULL | zSET_NOTNULL );
   SetAttributeFromString( vNewDomain, "Domain", "Name", szDomainName );

// copy complete structure of the Domain Object to the New Domain.
   nRC = SetCursorFirstEntity( vOldDomain, "Context", "" );
   while ( nRC == zCURSOR_SET )
   {
      nRC = CreateMetaEntity( vSubtask, vNewDomain, "Context", zPOS_AFTER );
      nRC = SetMatchingAttributesByName( vNewDomain, "Context",
                                         vOldDomain, "Context",
                                         zSET_NULL | zSET_NOTNULL );

      nRC = SetCursorFirstEntity( vOldDomain, "TableEntry", "" );
      while ( nRC == zCURSOR_SET )
      {
         nRC = CreateMetaEntity( vSubtask, vNewDomain, "TableEntry", zPOS_AFTER );
         nRC = SetMatchingAttributesByName( vNewDomain, "TableEntry",
                                            vOldDomain, "TableEntry",
                                            zSET_NULL | zSET_NOTNULL );

         nRC = SetCursorNextEntity( vOldDomain, "TableEntry", "" );
      }

      nRC = SetCursorFirstEntity( vOldDomain, "RegularExpression", "" );
      while ( nRC == zCURSOR_SET )
      {
         nRC = CreateMetaEntity( vSubtask, vNewDomain, "RegularExpression", zPOS_AFTER );
         nRC = SetMatchingAttributesByName( vNewDomain, "RegularExpression",
                                            vOldDomain, "RegularExpression",
                                            zSET_NULL | zSET_NOTNULL );

         nRC = SetCursorNextEntity( vOldDomain, "RegularExpression", "" );
      }

      nRC = SetCursorNextEntity( vOldDomain, "Context", "" );
   }

   nRC = zwfnTZDMUPDD_SaveDomain( vSubtask, vNewDomain );

   return( 0 );
}


////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_RebuildXDM
//   PURPOSE:    This Entry selects an operation for the domain
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_RebuildXDM( zVIEW vSubtask )
{
   zSHORT   nRC;

   nRC = RebuildXDM( vSubtask );

   return( nRC );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_HelpAbout
//   PURPOSE:    This Entry does the About
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_HelpAbout( zVIEW vSubtask )
{

   IssueToolAbout( vSubtask, zTOOL_DOMAIN, __FILE__, __DATE__, __TIME__ );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_HelpBasics
//   PURPOSE:    This Entry transfers to basic help
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_HelpBasics( zVIEW vSubtask )
{

   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_HelpOnHelp
//   PURPOSE:    This Entry transfers to HelpOnHelp
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

////////////////////////////////////////////////////////////////////////
//   OPERATION:  zwTZDMUPDD_HelpContents
//   PURPOSE:    This Entry transfers to help Contents
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_HelpContents( zVIEW vSubtask )
{

   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteDomainGrp( zVIEW vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vDomainGrpCur;
   zVIEW    vCM_ListGroup;
   zLONG    lStatus;
   zSHORT   nRC;
   zVIEW    vParentWindow;
   zVIEW    vWindow;

   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   nRC = ActivateMetaOI( vSubtask, &vDomainGrp, vCM_ListGroup, zSOURCE_DOMAINGRP_META, zSINGLE );

   nRC = GetIntegerFromAttribute( &lStatus, vCM_ListGroup, "W_MetaDef", "Status" );

   // If there is no update status or if the status does not indicate that
   // the Domain Group is maintainable through this LPLR, return.
   if ( nRC < 0 || lStatus != 1 )
   {
      MessageSend( vSubtask, "DM00109", "Domain Maintenance",
                   "The Domain Group is not maintainable in this LPLR.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      return( 0 );
   }

   // Prompt for Delete
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DOMAIN,
                       zREFER_DOMAINGRP_META,
                       vDomainGrp, "DomainGroup", "Name", 0 );
   if ( nRC == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // If the DomainGroup being deleted is the one currently in memory,
   // then drop the view in memory so that the deleted information won't
   // be displayed in the current window.
   if ( GetViewByName( &vDomainGrpCur, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
   {
      if ( CompareAttributeToAttribute( vDomainGrpCur, "DomainGroup", "ZKey",
                                        vDomainGrp,    "DomainGroup", "ZKey" ) == 0 )
      {
         DropView( vDomainGrpCur );
         //BL, 1999.10.21 if delete current Domain Group, then refresh Parentwindow
         //and set Focus to window Domain Group List again
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
         SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow,
                                  "TZDMUPDD", "DOMGRPLIST" );
      }
   }

   DropMetaOI( vSubtask, vDomainGrp );
   DeleteMetaOI( vSubtask, vCM_ListGroup, zSOURCE_DOMAINGRP_META );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZDMUPDD" ) >= 0 )
   {
      RefreshWindow( vWindow );
      RefreshAllSubwindows( vWindow );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_AcceptOperation( zVIEW vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vDomainGrp2;
   zVIEW    vMainWindow;

   // This operation is used instead of Subobject Auto functionality
   // because we need to add the ZKey to the Operation entity.
   // Use a new view because AssignZKey modifies the cursor position.

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );
   CreateViewFromViewForTask( &vDomainGrp2, vDomainGrp, 0 );

   AssignZKeyToMetaOI( vSubtask, vDomainGrp2, zSOURCE_DOMAINGRP_META, 0 );
   DropView( vDomainGrp2 );
   SetAttributeFromString( vDomainGrp, "Operation", "Type", "M" );

   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "Operation", "Name",
                                    "Operation" ) < 0 )
      return( -1 );

   //BL, 1999.10.27
   if ( GetSubtaskForWindowName( vSubtask, &vMainWindow, "TZDMUPDD" ) >= 0 )
      RefreshCtrl( vMainWindow, "Operation" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CreateNewContext( zVIEW vSubtask )
{
   zVIEW    vDomainGrp;
   zVIEW    vWindow;
   zVIEW    vParentWindow;

   // This operation is used instead of Subobject Auto functionality
   // because we need to create an entity with a ZKey.

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // If we are positioned on a Parameter, accept it, since it may be a temporal
   // subobject.
   if ( CheckExistenceOfEntity( vDomainGrp, "Context" ) >= zCURSOR_SET )
   {
      if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "Context",
                                       "Name", "Context" ) < 0 )
      {
         return( -1 );
      }
      SetSelectStateOfEntity( vDomainGrp, "Context", 0 );
   }

   CreateTemporalMetaEntity( vSubtask, vDomainGrp, "Context", zPOS_AFTER );
   SetSelectStateOfEntity( vDomainGrp, "Context", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CONTEXT" ) >= 0 )
   {
      GetParentWindow( &vParentWindow, vWindow );
      RefreshWindow( vParentWindow );
      SetFocusToCtrl( vSubtask, "ContextName" );
   }
   else
   {
      RefreshWindow( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_CreateCloneContext( zVIEW vSubtask )
{
   zVIEW    vDomain;
   zVIEW    vDomainBase;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zSHORT   nRC;

   // This operation is used instead of Subobject Auto functionality
   // because we need to create an entity with a ZKey.

   GetViewByName( &vDomain, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // If we are positioned on a Parameter, accept it, since it may be a temporal
   // subobject.
   if ( CheckExistenceOfEntity( vDomain, "Context" ) >= zCURSOR_SET )
   {
      if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "Context",
                                       "Name", "Context" ) < 0 )
      {
         return( -1 );
      }
      SetSelectStateOfEntity( vDomain, "Context", 0 );
   }

   CreateViewFromViewForTask( &vDomainBase, vDomain, 0 );

   CreateTemporalMetaEntity( vSubtask, vDomain, "Context", zPOS_AFTER );
   SetSelectStateOfEntity( vDomain, "Context", 1 );

   SetMatchingAttributesByName( vDomain, "Context", vDomainBase, "Context", zSET_NULL );
   nRC = SetCursorFirstEntity( vDomainBase, "TableEntry", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vDomain, "TableEntry", zPOS_AFTER );
      SetMatchingAttributesByName( vDomain, "TableEntry",
                                   vDomainBase, "TableEntry", zSET_NULL );

      nRC = SetCursorFirstEntity( vDomainBase, "ExtValueNLS_Text", "" );
      while ( nRC == zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vDomain, "NLS_TableEntry", zPOS_AFTER );
         SetMatchingAttributesByName( vDomain, "NLS_TableEntry",
                                      vDomainBase, "ExtValueNLS_Text", zSET_NULL );

         nRC = SetCursorNextEntity( vDomainBase, "ExtValueNLS_Text", "" );
      }

      nRC = SetCursorNextEntity( vDomainBase, "TableEntry", "" );
   }

   nRC = SetCursorFirstEntity( vDomainBase, "RegularExpression", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vDomain, "RegularExpression", zPOS_AFTER );
      SetMatchingAttributesByName( vDomain, "RegularExpression",
                                   vDomainBase, "RegularExpression", zSET_NULL );

      nRC = SetCursorNextEntity( vDomainBase, "RegularExpression", "" );
   }

   DropView( vDomainBase );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "CONTEXT" ) >= 0 )
   {
      GetParentWindow ( &vParentWindow, vWindow );
      RefreshWindow( vParentWindow );
      SetFocusToCtrl( vSubtask, "ContextName" );
   }
   else
   {
      RefreshWindow( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CreateDomainParameter
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CreateDomainParameter( zVIEW vSubtask, zVIEW vDomain )
{
   if ( !ComponentIsCheckedOut( vSubtask, vDomain, zSOURCE_DOMAINGRP_META ) )
      return( 0 );

   // DGC 95/09/06 Short term hack - Make sure that the domain operation
   // entity has parameters.  This can be removed when:
   //    o  Migrate creates the parms when migrating.
   //    o  Domain dialog creates parms when a new oper is created.
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "L" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "EntryType" );

   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "I" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "DomainDataStruct" );

/*
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "Y" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "DataType" );
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "S" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "Data" );
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "S" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "ContextName" );
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "V" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "View" );
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "S" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "ViewEntity" );
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "S" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "ViewAttrib" );
   CreateMetaEntity( vSubtask, vDomain, "Parameter", zPOS_LAST );
   SetAttributeFromString( vDomain, "Parameter", "DataType", "N" );
   SetAttributeFromString( vDomain, "Parameter", "ShortDesc", "MaxStrLth" );
*/
   return( 0 );
} // zwTZDMUPDD_CreateDomainParameter


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CheckDomainGroupTyp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CheckDomainGroupTyp( zVIEW  vSubtask,
                                zVIEW  vDomain )
{
   zCHAR  szMsg[150];

   if ( CompareAttributeToString( vDomain, "DomainGroup",
                                  "LanguageType", "" ) == 0 ||
        CompareAttributeToString( vDomain, "DomainGroup",
                                  "DomainAndGlobalOpGroupName", "" ) == 0 )
  {
      strcpy_s( szMsg, sizeof( szMsg ), "DLL Name does not exists or wrong Language\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Type selected.\n\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Please switch to Domain Group Properties\n" );
      strcat_s( szMsg, sizeof( szMsg ), "and correct the problem." );
      MessageSend( vSubtask, "DM00115", "Domain Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
  }

   return( 0 );
} // zwTZDMUPDD_CheckDomainGroupTyp


/*
   Edits the current operation.
*/
zSHORT
zwTZDMUPDD_TransferToEditor( zVIEW  vSubtask,
                             zSHORT nCallBySelectedOperation )
{
   zVIEW  vDomain;
   zVIEW  vDomainCopy;
   zVIEW  vMeta;
   zSHORT nRC;

   GetViewByName( &vDomain, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   if ( zwTZDMUPDD_CheckDomainGroupTyp( vSubtask, vDomain ) < 0 )
      return( -1 );

   // DGC 95/09/06 Short term hack - Make sure that the domain operation
   // entity has parameters.  This can be removed when:
   //    o  Migrate creates the parms when migrating.
   //    o  Domain dialog creates parms when a new oper is created.
   if ( CheckExistenceOfEntity( vDomain, "Parameter" ) != zCURSOR_SET )
      zwTZDMUPDD_CreateDomainParameter( vSubtask, vDomain );

   // Copy the SourceFile subobject information to a new work object of type TZZOLODO.
   // This is necessary because the Parser requires an object with a subobject starting
   // with entity named "SourceFile".  It is easier to set the source information as a
   // LOD than it is to modify the Parser to handle the different Domain structure.

   ActivateEmptyObjectInstance( &vMeta, "TZZOLODO", vSubtask, zSINGLE );
   SetNameForView( vMeta, "DGP_SourceFile", vSubtask, zLEVEL_TASK );

   CreateMetaEntity( vSubtask, vMeta, "LOD", zPOS_LAST );
   SetAttributeFromAttribute( vMeta,   "LOD",         "Name",
                              vDomain, "DomainGroup", "Name" );

   CreateMetaEntity( vSubtask, vMeta, "SourceFile", zPOS_LAST );
   SetMatchingAttributesByName( vMeta, "SourceFile", vDomain, "DomainGroup", zSET_NULL );

   CreateViewFromViewForTask( &vDomainCopy, vDomain, 0 );
   nRC = SetCursorFirstEntity( vDomainCopy, "Operation", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vMeta, "Operation", zPOS_LAST );
      SetMatchingAttributesByName( vMeta, "Operation",
                                   vDomainCopy, "Operation", zSET_NULL );
      nRC = SetCursorFirstEntity( vDomainCopy, "Parameter", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vMeta, "Parameter", zPOS_LAST );
         SetMatchingAttributesByName( vMeta, "Parameter",
                                      vDomainCopy, "Parameter", zSET_NULL );
         nRC = SetCursorNextEntity( vDomainCopy, "Parameter", 0 );
      }
      nRC = SetCursorNextEntity( vDomainCopy, "Operation", 0 );
   }
   DropView( vDomainCopy );

   //BL, 1999.10.27 for window Operation List
   if ( nCallBySelectedOperation == 1 &&
        CheckExistenceOfEntity( vDomain, "SelectedOperation" ) == zCURSOR_SET )
   {
      // Position on the correct Operation entity for the editor, which is the
      // same entity instance as SelectedOperation.
      SetCursorFirstEntityByAttr( vDomain, "Operation",         "ZKey",
                                  vDomain, "SelectedOperation", "ZKey", 0 );
   }
   else
   {
      SetCursorFirstEntityByAttr( vDomain, "Operation", "ZKey",
                                  vDomain, "Operation", "ZKey", 0 );
   }

   TransferToEditor( vSubtask, vDomain, "DM", 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DomainGroupPopup( zVIEW vSubtask )
{
   zSHORT   nEnable = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   nEnable = zwTZDMUPDD_GroupCheckedOut( vSubtask );

   EnableAction( vSubtask, "DeleteDomainGroup", nEnable );

   CreateTrackingPopupMenu( vSubtask, "DomainGroupPopup", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_OpenDomainPreBuild( zVIEW vSubtask )
{
   // For each domain, set the dates from the corresponding group.
   // Then order the Domainss first by date and then by name.

   zVIEW  vDomainLPLR;
   zVIEW  vGroupLPLR;
   zVIEW  vTZDGSRCO;
   zSHORT nRC;
   zCHAR  szGroupName[ 33 ];

   GetViewByName( &vDomainLPLR, "CM_List", vSubtask, zLEVEL_ANY );
   RetrieveViewForMetaList( vSubtask, &vGroupLPLR, zREFER_DOMAINGRP_META );

   nRC = SetCursorFirstEntity( vDomainLPLR, "W_MetaDef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szGroupName, sizeof( szGroupName ),
                              vDomainLPLR, "W_MetaDef", "GroupName" );
      SetCursorFirstEntityByString( vGroupLPLR, "W_MetaDef", "Name",
                                    szGroupName, 0 );
      SetAttributeFromAttribute( vDomainLPLR,  "W_MetaDef", "LastUpdateDate",
                                 vGroupLPLR, "W_MetaDef", "LastUpdateDate" );
      nRC = SetCursorNextEntity( vDomainLPLR, "W_MetaDef", 0 );
   }

   if ( GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZDGSRCO, "Domain" ) >= zCURSOR_SET )
   {
      // if Domain open, then set cursor of it
      SetCursorFirstEntityByAttr( vDomainLPLR, "W_MetaDef", "Name",
                                  vTZDGSRCO, "Domain", "Name", "" );
   }
   else
      SetCursorFirstEntity( vDomainLPLR, "W_MetaDef", 0 );

   DropView( vGroupLPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SortByName( zVIEW    vSubtask )
{
   // Order the domains by name only.

   zVIEW  vDomainLPLR;

   GetViewByName( &vDomainLPLR, "CM_List", vSubtask, zLEVEL_ANY );
   OrderEntityForView( vDomainLPLR, "W_MetaDef", "Name A" );
   SetCursorFirstEntity( vDomainLPLR, "W_MetaDef", 0 );

   zwTZDMUPDD_GroupCheckedOut( vSubtask );

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SortByDate( zVIEW    vSubtask )
{
   // Order the Domainations by date and then by name within date..

   zVIEW  vDomainLPLR;

   GetViewByName( &vDomainLPLR, "CM_List", vSubtask, zLEVEL_ANY );
   OrderEntityForView( vDomainLPLR, "W_MetaDef", "LastUpdateDate D Name A" );
   SetCursorFirstEntity( vDomainLPLR, "W_MetaDef", 0 );

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SortByGroup( zVIEW    vSubtask )
{
   // Order the domains by Group and then by name within Group.

   zVIEW  vDomainLPLR;

   GetViewByName( &vDomainLPLR, "CM_List", vSubtask, zLEVEL_ANY );
   OrderEntityForView( vDomainLPLR, "W_MetaDef", "GroupName A Name A" );
   SetCursorFirstEntity( vDomainLPLR, "W_MetaDef", 0 );

   return( 0 );

}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_OrderByExternalValue( zVIEW vSubtask )
{
   // Order entries in the table by their External Values.

   zVIEW  vDomainGrp;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDomainGrp, "TableEntry" ) >= zCURSOR_SET )
      OrderEntityForView( vDomainGrp, "TableEntry", "ExternalValue A" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_OrderByInternalValue( zVIEW vSubtask )
{
   // Order entries in the table by their External Values.

   zVIEW  vDomainGrp;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDomainGrp, "TableEntry" ) >= zCURSOR_SET )
      OrderEntityForView( vDomainGrp, "TableEntry", "InternalValue A" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_TableEntryIntegerOK( zVIEW vSubtask )
{
   // Move the internal value that was mapped to a temporary integer
   // attribute to the string internal value of the Domain Table Entry.

   zVIEW  vDomainGrp;
   zCHAR  szInternalValue[ 254 ];

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ),
                           vDomainGrp, "DomainGroup", "WorkNumeric" );
   // If DomainGroup not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
   {
      SetViewUpdate( vDomainGrp );
      SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                              szInternalValue );
      SetViewReadOnly( vDomainGrp );
   }
   else
      SetAttributeFromString( vDomainGrp, "TableEntry", "InternalValue",
                              szInternalValue );

   // Accept the current subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "TableEntry",
                                    "InternalValue", "Internal Value" ) < 0 )
   {
      SetFocusToCtrl( vSubtask, "InternalName" );
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_TableEntryIntegerInit ( zVIEW vSubtask )
{
   zVIEW  vDomainGrp;
   zCHAR  szInternalValue[ 254 ];

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   // Set up the work attribute for the new entity.
   GetStringFromAttribute( szInternalValue, sizeof( szInternalValue ),
                           vDomainGrp, "TableEntry", "InternalValue" );
   // If DomainGroup not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META ))
   {
      SetViewUpdate( vDomainGrp );
      SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                              szInternalValue );
      SetViewReadOnly( vDomainGrp );
   }
   else
      SetAttributeFromString( vDomainGrp, "DomainGroup", "WorkNumeric",
                              szInternalValue );

   return( 0 );
}


static zSHORT
zwfnTZDMUPDD_OpenDom( zVIEW   vSubtask,
                      zVIEW   vCM_List,
                      zPCHAR  szNewDomain )

{
   zSHORT  nRC;
   zVIEW   vDomainGrpA;
   zVIEW   vDomainGrp;
   zVIEW   vDomainGrpDetail;
   zVIEW   vWindow;
   zVIEW   vTaskLPLR;

   nRC = ActivateMetaOI( vSubtask, &vDomainGrpA, vCM_List,
                         zSOURCE_DOMAIN_META,
                         zSINGLE | zLEVEL_APPLICATION );

   if ( nRC >= 0 )
   {
      if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
         DropMetaOI( vSubtask, vDomainGrp );

      SetNameForView( vDomainGrpA, "TZDGSRCO", vSubtask, zLEVEL_TASK );

      // If Attribute "ContextRestriction" for Entity "Domain" not "Y" or "N",
      // then zeidon ask for save. This is a work around for old Domains
      zwfnTZDMUPDD_ActivateDomain( vSubtask, vDomainGrpA );

      // if DomainGroup not checked out, set  View read only ->
      // the user cannot update the values in Detail Windows
      if ( !ComponentIsCheckedOut( vSubtask, vDomainGrpA, zSOURCE_DOMAINGRP_META ))
         SetViewReadOnly( vDomainGrpA );

      // create the view for group detail window
      CreateViewFromViewForTask( &vDomainGrpDetail, vDomainGrpA, 0 );
      SetNameForView( vDomainGrpDetail, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

      // Set window Title with check out state
      SetTitleWithCheckOutState( vSubtask, "Domain Maintenance", "TZDGSRCO",
                                  vDomainGrpA, "Domain", zSOURCE_DOMAINGRP_META );

      if ( zstrcmp( szNewDomain, "" ) != 0 )
         SetCursorFirstEntityByString ( vDomainGrpA, "Domain",
                                        "Name", szNewDomain, "" );
      else
      {
         //BL, 1999.10.27 refresh window DomainList
         if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DOMLISTunderGRP" ) >= 0 )
            RefreshWindow( vWindow );

         if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
            RefreshWindow( vWindow );
      }
   }
   // Position on the blank entry in DomainJavaClass
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
      SetCursorFirstEntity( vTaskLPLR, "DomainJavaClass", "" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_OpenNextDom( zVIEW vSubtask )
{
   zVIEW    vCM_List;
   zVIEW    vDomainGrp;
   zLONG    lRC;

   lRC = zwTZDMUPDD_AskForSave( vSubtask );

   // if Domain not checked out and user change this Domain, then zeidon
   // call the window "Save Domain as". We save the action after Save as
   // in the View TZSAVEAS (for example: Open an other Domain or Switch
   // Project or Exit Domain Tool)
   if ( lRC == 99 )
      return( 0 );

   // If Save is OK, drop the view.
   if ( lRC == 0 )
   {
      if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
         DropView( vDomainGrp );
   }

   if ( lRC != zRESPONSE_CANCEL )
   {
      GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_ANY );
      // Use a second CM_List view for Activate since Activate will alter
      // its position.

      lRC = SetCursorNextEntity( vCM_List, "W_MetaDef", 0 );

      if ( lRC >= zCURSOR_SET )
         zwfnTZDMUPDD_OpenDom( vSubtask, vCM_List, "" );
      else
         MessageSend( vSubtask, "DM00109", "Domain Maintenance",
                      "This was already the last Domain.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_OpenPrevDom( zVIEW vSubtask )
{
   zVIEW    vCM_List;
   zVIEW    vDomainGrp;
   zLONG    lRC;

   lRC = zwTZDMUPDD_AskForSave( vSubtask );

   // if Domain not checked out and user change this Domain, then zeidon
   // call the window "Save Domain as". We save the action after Save as
   // in the View TZSAVEAS (for example: Open an other Domain or Switch
   // Project or Exit Domain Tool)
   if ( lRC == 99 )
      return( 0 );

   // If Save is OK, drop the view.
   if ( lRC == 0 )
   {
      if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
         DropView( vDomainGrp );
   }

   if ( lRC != zRESPONSE_CANCEL )
   {
      GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_ANY );
      // Use a second CM_List view for Activate since Activate will alter
      // its position.

      lRC = SetCursorPrevEntity( vCM_List, "W_MetaDef", 0 );

      if ( lRC >= zCURSOR_SET )
         zwfnTZDMUPDD_OpenDom( vSubtask, vCM_List, "" );
      else
         MessageSend( vSubtask, "DM00109", "Domain Maintenance",
                      "This was already the first Domain.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_NewDomainPrebuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   // Blank out the Name and Desc values in the Profile object so that
   // previous names won't be shown.
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vProfileXFER, "DM" ) >= zCURSOR_SET )
   {
      SetAttributeFromString( vProfileXFER, "DM", "DomainName", "" );
      SetAttributeFromString( vProfileXFER, "DM", "DomainDesc", "" );
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                              zDM_NewDomain );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_LoadDomain
//
//  Popup in window Domain List, load an other Domain in main window
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_LoadDomain( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vDomainGrp;
   zVIEW  vCM_List;
   zVIEW  vSaveAs;
   zSHORT nRC;
   zLONG  lActionAfterSaveAs;
   zCHAR  szNewDomain[ 33 ];
   zCHAR  szOldDomain[ 33 ];

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );

   //save new selected Domain
   GetStringFromAttribute( szNewDomain, sizeof( szNewDomain ), vDomainGrp, "Domain", "Name" );

   GetSubtaskForWindowName( vSubtask, &vWindow, "TZDMUPDD" );
   GetCtrlText( vWindow, "DomainName", szOldDomain, 33 );

   if ( zstrcmp( szNewDomain, szOldDomain ) != 0 )
   {
      SetCursorFirstEntityByString ( vDomainGrp, "Domain",
                                     "Name", szOldDomain, "" );

      if ( lActionAfterSaveAs == 5 )
         return( 0 );

      nRC = zwTZDMUPDD_AskForSave( vWindow );

      // if Domain not checked out and user change this Domain, then zeidon
      // call the window "Save Domain as". We save the action after Save as
      // in the View TZSAVEAS (for example: Open an other Domain or Switch
      // Project or Exit Domain Tool)
      if ( nRC == 99 )
      {
         // 4rd Parameter = 5 means: Load save as window only one
         SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", 5 );
         RefreshWindow( vSubtask );
         SetWindowActionBehavior( vWindow, zWAB_ProcessImmediateAction |
                                  zWAB_StartModalSubwindow, "TZDMUPDD", "SaveAs" );
         return( 0 );
      }

      if ( nRC != zRESPONSE_CANCEL )
      {
         GetViewByName( &vCM_List, "CM_List", vWindow, zLEVEL_ANY );
         // Use a second CM_List view for Activate since Activate will alter
         // its position.
         nRC = SetCursorFirstEntityByString ( vCM_List, "W_MetaDef",
                                              "Name", szOldDomain, "" );

         zwfnTZDMUPDD_OpenDom( vWindow, vCM_List, szNewDomain );
      }

      RefreshWindow( vWindow );
      RefreshWindow( vSubtask );
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0)
         RefreshWindow( vWindow );
   }

   return( 0 );
} // zwTZDMUPDD_LoadDomain

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_RefreshDomainList
//
//  refresh Domain Name in window DomainList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_RefreshDomainList( zVIEW vSubtask )
{
   zVIEW vParentWindow;

   GetParentWindow( &vParentWindow, vSubtask );
   MapCtrl( vParentWindow, "DomainName" );
   MapCtrl( vParentWindow, "Desc" );

   return( 0 );
} // zwTZDMUPDD_RefreshDomainList


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_DeleteOperation
//
//  Delete Operation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteOperation( zVIEW vSubtask )
{
   zVIEW    vWindow;
   zVIEW    vTZDGSRCO;
   zVIEW    vTZDGSRCO_Copy;
   zSHORT   nRC;
   zCHAR    szName[ 34 ];
   zCHAR    szMsg[ 150 ];

   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Prompt for Delete
   strcpy_s( szMsg, sizeof( szMsg ), "OK to Delete Domain Operation '" );
   GetStringFromAttribute( szName, sizeof( szName ), vTZDGSRCO, "Operation", "Name" );
   strcat_s( szMsg, sizeof( szMsg ), szName );
   strcat_s( szMsg, sizeof( szMsg ), "' ?" );
   nRC = MessagePrompt( vSubtask, "OP00805", "Domain Maintenance",
                        szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

   if ( nRC == zRESPONSE_YES )
   {
      // if Operation assigned an other Domain?
      CreateViewFromViewForTask( &vTZDGSRCO_Copy, vTZDGSRCO, 0 );
      nRC = SetCursorFirstEntityByString( vTZDGSRCO_Copy, "SelectedOperation",
                                          "Name", szName, "DomainGroup" );
      if ( nRC >= zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Domain Operation '" );
         strcat_s( szMsg, sizeof( szMsg ), szName );
         strcat_s( szMsg, sizeof( szMsg ), "' cannot be deleted \nif it is used by an Domain." );
         MessageSend( vSubtask, "OP00817", "Domain Maintenance", szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         return( -1 );
      }
      DropView( vTZDGSRCO_Copy );

      if ( MessagePrompt( vSubtask, "OP00810", "Domain Maintenance",
                          "Delete Operation from Source File?",
                          0, zBUTTONS_YESNO, zRESPONSE_YES,
                          zICON_QUESTION ) == zRESPONSE_YES )
      {
         // Create Work LOD for Delete Operation from File
         zwTZDMUPDD_CreateDeleteWorkLod( vSubtask, vTZDGSRCO, szName );
         // delete Operation Code from Target Group
         zwTZDMUPDD_DeleteOperFromFile( vSubtask, vTZDGSRCO, zDELETE_OPERATION );
      }

      DeleteEntity( vTZDGSRCO, "Operation", zREPOS_PREV );
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZDMUPDD" ) >= 0 )
         RefreshWindow( vWindow );
   }

   return( 0 );
} // zwTZDMUPDD_DeleteOperation

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_DeleteSelOperation
//
//  delete Operation about Edit menu
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_DeleteSelOperation( zVIEW vSubtask )
{
   zVIEW    vWindow;
   zVIEW    vTZDGSRCO;

   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) >= zCURSOR_SET )
      SetCursorFirstEntityByEntityCsr( vTZDGSRCO, "SelectedOperation",
                                       vTZDGSRCO, "Operation", "" );

   zwTZDMUPDD_DeleteOperation( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
} // zwTZDMUPDD_DeleteSelOperation

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_EditOperationcode
//
//  call Editor for selected Operation in window OperationList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_EditOperationcode( zVIEW vSubtask )
{
   zwTZDMUPDD_TransferToEditor( vSubtask, 0 );

   return( 0 );
} // zwTZDMUPDD_EditOperationcode

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_EditOperationcodeMenu
//
//  call Editor for selected Operation in main window
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_EditOperationcodeMenu( zVIEW vSubtask )
{
   zwTZDMUPDD_TransferToEditor( vSubtask, 1 );

   return( 0 );
} // zwTZDMUPDD_EditOperationcodeMenu

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_InitListMenu
//
//  enable and disable menu items
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
zwTZDMUPDD_InitListMenu( zVIEW vSubtask )
{
   zVIEW    vT;

   //Domain Group List
   if ( GetSubtaskForWindowName( vSubtask, &vT, "DOMGRPLIST" ) >= 0 )
      SetOptionState( vSubtask, "DomainGroupList", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetOptionState( vSubtask, "DomainGroupList", zCONTROL_STATUS_CHECKED, 0 );

   //Domain List
   if ( GetSubtaskForWindowName( vSubtask, &vT, "DOMLISTunderGRP" ) >= 0 )
      SetOptionState( vSubtask, "DomainsInGroup", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetOptionState( vSubtask, "DomainsInGroup", zCONTROL_STATUS_CHECKED, 0 );

   //Operation List
   if ( GetSubtaskForWindowName( vSubtask, &vT, "OPERLIST" ) >= 0 )
      SetOptionState( vSubtask, "OperationsInGroup", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetOptionState( vSubtask, "OperationsInGroup", zCONTROL_STATUS_CHECKED, 0 );

   return( 0 );
} //zwTZDMUPDD_InitListMenu

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZDMUPDD_InitMenuDisableAll
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
zwfnTZDMUPDD_InitMenuDisableAll( zVIEW vSubtask )
{
   zVIEW vLPLR;
   zBOOL bEnable = TRUE;

   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddContext", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateContext", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteContext", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SetDefaultContext", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "MoveDomain", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EditOperation", zOPTION_STATUS_ENABLED, 0 );

   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      bEnable = FALSE;

   SetOptionState( vSubtask, "New12", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Open", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Generate", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "DomainGroupList", zOPTION_STATUS_ENABLED, bEnable );

   return( 0 );
} // zwfnTZDMUPDD_InitMenuDisableAll

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_InitMenu
//
//  enable and disable menu items
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_InitMenu( zVIEW vSubtask )
{
   zVIEW    vT;
   zVIEW    vDomainGrp;
   zSHORT   nEnable = 0;
   zSHORT   nIsCheckedOut;

   zwfnTZDMUPDD_InitMenuDisableAll( vSubtask );

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDomainGrp,
                                          zSOURCE_DOMAINGRP_META );
   if ( nIsCheckedOut == 1 )
   {
      SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddOperation", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddContext", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "MoveDomain", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( CheckExistenceOfEntity( vDomainGrp, "Context" ) >= zCURSOR_SET &&
        GetSelectStateOfEntity( vDomainGrp, "Context" ) == 1 )
   {
      SetOptionState( vSubtask, "UpdateContext", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
      {
         SetOptionState( vSubtask, "DeleteContext", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "SetDefaultContext", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   MapCtrl( vSubtask, "Operation" );
   if ( CheckExistenceOfEntity( vDomainGrp, "SelectedOperation" ) >= zCURSOR_SET )
   {
      nEnable = 1;
   }
   else
   {
      if ( GetSubtaskForWindowName( vSubtask, &vT, "OPERLIST" ) >= 0 &&
           CheckExistenceOfEntity( vDomainGrp, "Operation" ) >= zCURSOR_SET &&
           GetSelectStateOfEntity( vDomainGrp, "Operation" ) == 1 )
      {
         nEnable = 1;
      }
   }

   if ( nEnable == 1 )
   {
      SetOptionState( vSubtask, "EditOperation", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "UpdateOperation", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
         SetOptionState( vSubtask, "DeleteOperation", zOPTION_STATUS_ENABLED, 1 );
   }

   zwTZDMUPDD_InitListMenu( vSubtask );

   return( 0 );
} // zwTZDMUPDD_InitMenu

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_MoveDomainPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_MoveDomainPreBuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  vDomainList;
   zVIEW  vTZDGSRCO;

   GetViewByName( &vDomainList, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vProfileXFER, "DM" ) >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDomainList, "W_MetaDef" ) >= zCURSOR_SET )
      {
         SetAttributeFromAttribute( vProfileXFER, "DM", "DomainName",
                                    vDomainList, "W_MetaDef", "Name" );
         SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                                    vDomainList, "W_MetaDef", "Desc" );
      }
      else
      {
         SetAttributeFromAttribute( vProfileXFER, "DM", "DomainName",
                                    vTZDGSRCO, "Domain", "Name" );
         SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                                    vTZDGSRCO, "Domain", "Desc" );
      }

      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                              zDM_MoveDomain );
      zwTZDMUPDD_CreateWorkLOD( vSubtask );
    }

   return( 0 );
} // zwTZDMUPDD_MoveDomainPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_MoveDomainPostBuild
//
//  Set Domain Name and Domain Group Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_MoveDomainPostBuild( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vCM_List;
   zVIEW  vTZDGSRCO;
   zVIEW  vMoveDomain;
   zCHAR  szDomainName[ 33 ];
   zCHAR  szDomainGroup[ 33];
   zCHAR  szType[ 4 ];
   zSHORT nRC = -1;
   zBOOL  bSelectedOperation = FALSE;

   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );

   // was window <Move Domain from Group to Group> opened via Move Button or menu item
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DOMLIST" ) >= 0 )
   {
      GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vCM_List, "W_MetaDef", "Name" );
      GetStringFromAttribute( szDomainGroup, sizeof( szDomainGroup ), vCM_List, "W_MetaDef", "GroupName" );
      SetCursorFirstEntityByAttr( vMoveDomain, "SourceFile", "Name",
                                  vCM_List, "W_MetaDef", "GroupName", "" );
      GetStringFromAttributeByContext( szType, vMoveDomain, "SourceFile",
                                       "LanguageType", "LanguageType", 4 );
      // has Domain a Domain Operation?
      ActivateMetaOI( vSubtask, &vTZDGSRCO, vCM_List, zSOURCE_DOMAIN_META,
                      zSINGLE | zLEVEL_TASK );
      if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) >= zCURSOR_SET )
         bSelectedOperation = TRUE;
      DropMetaOI( vSubtask, vTZDGSRCO );
   }
   else
   {
      GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vTZDGSRCO, "Domain", "Name" );
      GetStringFromAttribute( szDomainGroup, sizeof( szDomainGroup ), vTZDGSRCO, "DomainGroup", "Name" );
      GetStringFromAttributeByContext( szType, vTZDGSRCO, "DomainGroup",
                                       "LanguageType", "LanguageType", 4 );
      SetCursorFirstEntityByAttr( vMoveDomain, "SourceFile", "Name",
                                  vTZDGSRCO, "DomainGroup", "Name", "" );
      if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) >= zCURSOR_SET )
         bSelectedOperation = TRUE;
   }

   SetCtrlText( vSubtask, "txtDomainName", szDomainName );
   SetCtrlText( vSubtask, "txtDomainGroup", szDomainGroup );
   SetCtrlText( vSubtask, "txtType", szType );

   if ( bSelectedOperation )
      SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_ENABLED, 0 );

   zwTZDMUPDD_SetCeckOutState( vSubtask );

   return( 0 );
} // zwTZDMUPDD_MoveDomainPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_Set_CM_List
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_Set_CM_List( zVIEW vSubtask )
{
   zVIEW  vDomainGrp;
   zVIEW  vDomainList;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDomainList, "CM_List", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDomainGrp, "Domain" ) >= zCURSOR_SET )
      SetCursorFirstEntityByAttr( vDomainList, "W_MetaDef", "Name",
                                 vDomainGrp, "Domain", "Name", "" );

   return( 0 );
} // zwTZDMUPDD_Set_CM_List

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_SaveAsPreBuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   zwTZDMUPDD_OpenDomainPreBuild( vSubtask );
   zwTZDMUPDD_NewDomainPrebuild( vSubtask );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // set save as flag
   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                           zDM_SaveAsDomain );

   zwTZDMUPDD_CreateWorkLOD( vSubtask );

   return( 0 );
} // zwTZDMUPDD_SaveAsPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SaveAsPostBuild( zVIEW vSubtask )
{
   zVIEW   vTZDGSRCO;
   zVIEW   vCM_List;
   zVIEW   vMoveDomain;
   zVIEW   vProfileXFER;
   zCHAR   szDomainName[ 33 ];


   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vTZDGSRCO, "Domain", "Name" );

   //if edit new Domain Name in main window
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szDomainName, "" ) < zCURSOR_SET )
   {
      SetAttributeFromString( vProfileXFER, "DM", "DomainName", szDomainName );
   }
   else
   {
      SetAttributeFromString( vProfileXFER, "DM", "DomainName", "" );
      SetCtrlText( vSubtask, "txtDomainGroup", "" );
   }

   SetCtrlText( vSubtask, "txtDomainGroup", "" );
   SetCtrlText( vSubtask, "txtType", "" );
   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );

   if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) < zCURSOR_SET )
      SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_ENABLED, 0 );
   else
      SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED, 1 );

   //set current Description
   SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                              vTZDGSRCO, "Domain", "Desc" );

   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vCM_List, "W_MetaDef", 0 );

   if ( CheckExistenceOfEntity( vMoveDomain, "SourceFile" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vMoveDomain, "SourceFile", 0 );


   return( 0 );
} // zwTZDMUPDD_SaveAsPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_RefreshGroupList
//
//  if Single Click on Domain List, then set selection in Group List
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_RefreshGroupList( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vMoveDomain;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );

   SetSelectStateOfEntity( vMoveDomain, "SourceFile", 0 );

   SetCursorFirstEntityByAttr( vMoveDomain, "SourceFile", "Name",
                               vCM_List, "W_MetaDef", "GroupName", "" );
   SetSelectStateOfEntity( vMoveDomain, "SourceFile", 1 );

   RefreshCtrl( vSubtask, "lbGroupList" );

   // set check out state in Title
   zwTZDMUPDD_SetCeckOutState( vSubtask );

   return( 0 );
} // zwTZDMUPDD_RefreshGroupList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SetSaveAsName
//
//  if Double Click on Domain List, then set Domain Name, Description and Group Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SetSaveAsName( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vMoveDomain;
   zVIEW   vProfileXFER;
   zVIEW   vDomainData;
   zCHAR   szGroupName[ 33];
   zCHAR   szType[ 4 ];

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetSelectStateOfEntity( vMoveDomain, "SourceFile", 0 );

   SetAttributeFromAttribute ( vProfileXFER, "DM", "DomainName",
                               vCM_List, "W_MetaDef", "Name" );

   GetStringFromAttribute( szGroupName, sizeof( szGroupName ), vCM_List, "W_MetaDef", "GroupName" );
   GetStringFromAttributeByContext( szType, vMoveDomain, "SourceFile",
                                    "LanguageType", "LanguageType", 4 );
   SetCtrlText( vSubtask, "txtDomainGroup", szGroupName );
   SetCtrlText( vSubtask, "txtType", szType );

   SetCursorFirstEntityByAttr( vMoveDomain, "SourceFile", "Name",
                               vCM_List, "W_MetaDef", "GroupName", "" );
   SetSelectStateOfEntity( vMoveDomain, "SourceFile", 1 );

   // set Description
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) != 1 )
   {
      //no Description in View vCM_List or vCM_ListGroup
      ActivateMetaOI( vSubtask, &vDomainData, vCM_List, zSOURCE_DOMAIN_META,
                      zSINGLE | zLEVEL_TASK );
      SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                                 vDomainData, "Domain", "Desc" );
      DropMetaOI( vSubtask, vDomainData );
      RefreshCtrl( vSubtask, "edDomainDesc" );
   }


   RefreshCtrl( vSubtask, "txtDomainGroup" );
   RefreshCtrl( vSubtask, "edDomainName" );
   RefreshCtrl( vSubtask, "lbGroupList" );
   RefreshCtrl( vSubtask, "txtType" );

   // set check out state in Title
   zwTZDMUPDD_SetCeckOutState( vSubtask );

   return( 0 );
} // zwTZDMUPDD_SetSaveAsName

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SetGroupName
//
//  if single or double click on Group List, then set Group Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SetGroupName( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vMoveDomain;
   zVIEW   vProfileXFER;
   zCHAR   szGroupName[ 33];
   zCHAR   szType[ 4 ];

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szGroupName, sizeof( szGroupName ), vMoveDomain, "SourceFile", "Name" );
   GetStringFromAttributeByContext( szType, vMoveDomain, "SourceFile",
                                    "LanguageType", "LanguageType", 4 );

   SetCtrlText( vSubtask, "txtDomainGroup", szGroupName );
   SetCtrlText( vSubtask, "txtType", szType );
   RefreshCtrl( vSubtask, "txtDomainGroup" );
   RefreshCtrl( vSubtask, "txtType" );

   SetSelectStateOfEntity( vCM_List, "W_MetaDef", 0 );
   RefreshCtrl( vSubtask, "lbDomainList" );

   // set check out state in Title
   zwTZDMUPDD_SetCeckOutState( vSubtask );

   return( 0 );
} // zwTZDMUPDD_SetGroupName

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_ResetProfileXFER
//
//  Reset View ProfileXFER
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_ResetProfileXFER( zVIEW vSubtask )
{
   zVIEW   vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetAttributeFromString( vProfileXFER, "DM", "DomainName", "" );
   SetAttributeFromString( vProfileXFER, "DM", "DomainDesc", "" );
   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "" );

   return( 0 );
} // zwTZDMUPDD_ResetProfileXFER

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SaveAsCancel( zVIEW vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vMoveDomain;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   zwTZDMUPDD_ResetProfileXFER( vSubtask );

   if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveDomain );

   return( 0 );
} // zwTZDMUPDD_SaveAsCancel


static zSHORT
zwTZDMUPDD_CheckActionAfterSaveAs( zVIEW  vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vTZDGSRCO;
   zVIEW   vParentWindow;
   zLONG   lActionAfterSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   GetParentWindow( &vParentWindow, vSubtask );
   SetTitleWithCheckOutState( vParentWindow, "Domain Maintenance", "TZDGSRCO",
                               vTZDGSRCO, "Domain", zSOURCE_DOMAINGRP_META );

   // if Domain not checked out and user change this Domain, then zeidon
   // call the window "Save Domain as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Domain or Open an other Domain or Switch Project or Exit
   // Domain Tool)

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   switch ( lActionAfterSaveAs )
   {
      case zNewComponentAfterSaveAs:   // Create new Domain
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZDMUPDD", "NEWNAME" );
         break;

      case zOpenComponentAfterSaveAs:   // Open Domain
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZDMUPDD", "DOMLIST" );
         break;

      case zSwitchLPLRAfterSaveAs:   // Switch Project
         zwTZDMUPDD_SwitchLPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZCMSLPD", "SwitchLPLR" );
         break;

      case zExitToolAfterSaveAs:   // Exit Domain Tool
         zwTZDMUPDD_ExitSaveDomain( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, "", "" );
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsCheckValues
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_SaveAsCheckValues( zVIEW  vSubtask,
                              zPCHAR szGroupName,
                              zPCHAR szOutName )
{
   zCHAR   szNewName[ 33 ];
   zCHAR   szControlText[ 4 ];
   zCHAR   szLanguageType[ 4 ];
   zCHAR   szMsg[ 100 ];
   zVIEW   vProfileXFER;
   zVIEW   vTZDGSRCO;
   zVIEW   vCM_List;
   zVIEW   vCM_List_Copy;
   zVIEW   vWindow;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" );

   //Domain Group Name is required
   GetCtrlText( vWindow, "txtDomainGroup", szGroupName, 33 );
   if ( zstrcmp( szGroupName, "" ) == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Target Group Name is required." );
      MessageSend( vSubtask, "ZO00137", "Domain Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   //Domain Name is required
   GetStringFromAttribute( szNewName, sizeof( szNewName ), vProfileXFER, "DM", "DomainName" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Domain Name is required." );
      MessageSend( vSubtask, "ZO00137", "Domain Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vWindow, "edDomainName" );
      return( -2 );
   }

   CreateViewFromViewForTask( &vCM_List_Copy, vCM_List, 0 );
   if ( SetCursorFirstEntityByString( vCM_List_Copy, "W_MetaDef", "Name",
                                      szOutName, "" ) >= zCURSOR_SET &&
        CompareAttributeToString( vCM_List_Copy, "W_MetaDef", "GroupName",
                                  szGroupName ) != 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Cannot save Domain, because already exists\n" );
      strcat_s( szMsg, sizeof( szMsg ), "in Domain Group '" );
      GetStringFromAttribute( szNewName, sizeof( szNewName ), vCM_List_Copy, "W_MetaDef", "GroupName" );
      strcat_s( szMsg, sizeof( szMsg ), szNewName );
      strcat_s( szMsg, sizeof( szMsg ), "'.");
      MessageSend( vSubtask, "ZO00137", "Domain Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // if save Operation as with Operation Code
   if ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      GetCtrlText( vWindow, "txtType", szControlText, 4 );
      GetStringFromAttributeByContext( szLanguageType, vTZDGSRCO, "DomainGroup",
                                       "LanguageType", "LanguageType", 4 );
      if ( zstrcmp( szControlText, szLanguageType ) != 0 )
      {
         MessageSend( vSubtask, "ZO00137", "Domain Maintenance",
                      "Language Type of Domain Group do not match.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }
   }

   return( 0 );
} // zwTZDMUPDD_SaveAsCheckValues

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsDomain
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SaveAsDomain( zVIEW vSubtask )
{
   zVIEW   vTZDGSRCO;
   zVIEW   vCM_List;
   zVIEW   vProfileXFER;
   zVIEW   vWindow;
   zCHAR   szGroupName[ 33 ];
   zCHAR   szOutName[ 33 ];
   zSHORT  nCopyCode = 0;
   zSHORT  nRC;
   zSHORT  nDeleteSourceDomain = 0;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   szGroupName[ 0 ] = 0;
   szOutName[ 0 ] = 0;

   if ( zwTZDMUPDD_SaveAsCheckValues( vSubtask, szGroupName, szOutName ) < 0 )
      return( -1 );

   // if copy Operation Code
   GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" );
   if ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
      nCopyCode = 1;

   GetSubtaskForWindowName( vSubtask, &vWindow, "TZDMUPDD" );
   MapWindow( vWindow );

   //Domain Name already exists
   SetAttributeFromString( vProfileXFER, "DM", "DomainName", szOutName );
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      nRC = MessagePrompt( vSubtask, "ZO00138", "Domain Maintenance",
                           "Domain already exists. Replace existing Domain?",
                           zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edDomainName" );
         return( -1 );
      }
      //if this Domain open, then save Domain, not save as
      if ( CompareAttributeToString( vTZDGSRCO, "Domain",
                                    "Name", szOutName ) == 0 )
      {
         zwTZDMUPDD_SaveDomain( vSubtask );
         zwTZDMUPDD_ResetProfileXFER( vSubtask );
         return( 0 );
      }
      else
         // if another Domain open, then replace existing Domain
         nDeleteSourceDomain = 1;
   }

   if ( zwTZDMUPDD_SaveAsNewDomain( vWindow, szGroupName, szOutName,
                                    nCopyCode, nDeleteSourceDomain ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edDomainName" );
      return( -1 );
   }

   zwTZDMUPDD_ResetProfileXFER( vSubtask );
   RefreshWindow( vWindow );
   zwTZDMUPDD_CheckActionAfterSaveAs( vSubtask );

   return( 0 );
} // zwTZDMUPDD_SaveAsDomain

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CopyOperationWorkLOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CopyOperationWorkLOD( zVIEW  vSubtask,
                                 zVIEW  vTZDGSRCO,
                                 zSHORT nCreateLOD )
{
   zVIEW  vDeleteOp;

   GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );

   if ( nCreateLOD == 1 )
   {
      // Create LOD for copy Operation Code from old Source Files
      if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) >= 0 )
         DropObjectInstance( vDeleteOp );

      ActivateEmptyObjectInstance( &vDeleteOp, "TZEDWDEL", vSubtask, zMULTIPLE );
      SetNameForView( vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );
   }

   // Create Source File
   CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
   SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                vTZDGSRCO, "DomainGroup", zSET_ALL );

   // Create Operation
   CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
   SetAttributeFromAttribute( vDeleteOp, "Operation", "Name",
                              vTZDGSRCO, "Operation", "Name" );

   return( 0 );
} // zwTZDMUPDD_CopyOperationWorkLOD

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsNewDomain
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZDMUPDD_SaveAsNewDomain( zVIEW  vSubtask,
                            zPCHAR szGroupName,
                            zPCHAR szDomainName,
                            zSHORT nCopyCode,
                            zSHORT nDeleteSourceDomain )
{
   zVIEW   vProfileXFER;
   zVIEW   vSource;
   zVIEW   vTarget;
   zVIEW   vCM_List;
   zVIEW   vCM_ListGroup;
   zVIEW   vMoveDomain;
   zCHAR   szDomainGroup[ 33 ];
   zSHORT  nCopyCodeFlag;
   zULONG  ulZKey;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vSource, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_SaveAsDomainInNewGroup ) == 0 )
   {
      GetViewByName( &vTarget, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
      RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_DOMAINGRP_META );
   }
   else
   {
      SetCursorFirstEntityByString( vCM_ListGroup, "W_MetaDef", "Name",
                                    szDomainGroup, 0 );
      ActivateMetaOI( vSubtask, &vTarget, vCM_ListGroup, zSOURCE_DOMAINGRP_META, zSINGLE );
   }

   if ( nDeleteSourceDomain == 1 )
   {
      SetCursorFirstEntityByString( vTarget, "Domain", "Name", szDomainName, 0 );
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vTarget, "Domain", "ZKey" );
      DeleteEntity( vTarget, "Domain", zREPOS_NEXT );
   }

   CreateMetaEntity( vSubtask, vTarget, "Domain", zPOS_AFTER );
   SetMatchingAttributesByName( vTarget, "Domain", vSource, "Domain", zSET_NULL );
   SetAttributeFromAttribute( vTarget, "Domain", "Name",
                              vProfileXFER, "DM", "DomainName" );
   SetAttributeFromAttribute( vTarget, "Domain", "Desc",
                              vProfileXFER, "DM", "DomainDesc" );

   if ( nDeleteSourceDomain == 1 )
      SetAttributeFromInteger( vTarget, "Domain", "ZKey", ulZKey );

   // Create Context and Operation
   nCopyCodeFlag = zwTZDMUPDD_CreateContextAndOper( vSubtask, vSource, vTarget,
                                                    zCOPY_OPERATION );

   if ( nCopyCodeFlag < 0 )
      return( -1 );

   if ( nCopyCode == 1 && nCopyCodeFlag > zDM_NOREPLACE_CODE )
      zwTZDMUPDD_CopyOperationWorkLOD( vSubtask, vSource, 1 );

   CommitMetaOI( vSubtask, vTarget, zSOURCE_DOMAINGRP_META );

   if ( GetViewByName( &vSource, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vSource );

   SetNameForView( vTarget, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByString( vTarget, "Domain", "Name", szDomainName, "" );
   SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", szDomainName, "" );

   // copy Operation Code
   if ( nCopyCode == 1 && nCopyCodeFlag > zDM_NOREPLACE_CODE )
   {
      zwTZDMUPDD_CopyOperationWorkLOD( vSubtask, vTarget, 0 );
      zwTZDMUPDD_DeleteOperFromFile( vSubtask, vTarget, zCOPY_OPERATION );
   }

   // Drop Work LOD for Save As Window
   if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveDomain );

   return( 0 );
} // zwTZDMUPDD_SaveAsNewDomain

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CreateDeleteWorkLod
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CreateDeleteWorkLod( zVIEW  vSubtask,
                                zVIEW  vTZDGSRCO,
                                zPCHAR szOperationName )
{
   zVIEW    vDeleteOp;

   // delete Operation Code from Target Group
   if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vDeleteOp );

   ActivateEmptyObjectInstance( &vDeleteOp, "TZEDWDEL", vSubtask, zMULTIPLE );

   // Create Source File for Delete LOD
   CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
   SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                vTZDGSRCO, "DomainGroup", zSET_ALL );
   SetNameForView( vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );

   // Create Operation for Delete LOD
   CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
   SetAttributeFromString( vDeleteOp, "Operation", "Name", szOperationName );

   return( 0 );
} // zwTZDMUPDD_CreateDeleteWorkLod

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZDMUPDD_CreateOperation
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
zwfnTZDMUPDD_CreateOperation( zVIEW   vSubtask,
                              zVIEW   vTarget,
                              zVIEW   vSource,
                              zPCHAR  szOperationName,
                              zSHORT  nMoveFlag )
{
   zVIEW    vCopy;
   zSHORT   nRC;
   zCHAR    szMsg[150];

   if ( CompareAttributeToAttribute( vTarget, "DomainGroup", "ZKey",
                                     vSource, "DomainGroup", "ZKey" ) == 0 )
   {
      return( zDM_NOREPLACE_CODE );
   }

   CreateViewFromViewForTask( &vCopy, vTarget, 0 );
   nRC = SetCursorFirstEntityByString( vCopy, "SelectedOperation", "Name",
                                       szOperationName, "DomainGroup" );
   DropView( vCopy );
   strcpy_s( szMsg, sizeof( szMsg ), "Domain Operation '" );
   strcat_s( szMsg, sizeof( szMsg ), szOperationName );
   strcat_s( szMsg, sizeof( szMsg ), "' " );

   // Operation already exists in Target Group and is assigned to an other Domain
   if ( nRC >= zCURSOR_SET )
   {
      strcat_s( szMsg, sizeof( szMsg ), "is assigned to more than one Domains.\n\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Replace existing Operation? ");
   }
   else
   {
      strcat_s( szMsg, sizeof( szMsg ), "already exists.\n\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Replace existing Operation? ");
   }

   nRC = MessagePrompt( vSubtask, "ZO00138", "Domain Maintenance",
                        szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                        zRESPONSE_NO, zICON_QUESTION );
   if ( nRC == zRESPONSE_CANCEL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }
   else
   {
      if ( nRC == zRESPONSE_NO )
        return( zDM_NOREPLACE_CODE );
   }

   return( zDM_REPLACE_CODE );
} // zwfnTZDMUPDD_CreateOperation

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_RemoveOperFromGroup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_RemoveOperFromGroup( zVIEW   vSubtask,
                                zVIEW   vSource,
                                zPCHAR szOperationName )
{
   zVIEW    vCopy;
   zSHORT   nRC;
   zSHORT   nCopyOrMoveFlag;
   zCHAR    szMsg[ 150 ];

   // Delete Operation in Source, if not assigned to an other Domain
   CreateViewFromViewForTask( &vCopy, vSource, 0 );
   SetCursorFirstEntityByString( vCopy, "SelectedOperation", "Name",
                                 szOperationName, "DomainGroup" );
   nRC = SetCursorNextEntityByString( vCopy, "SelectedOperation", "Name",
                                      szOperationName, "DomainGroup" );
   DropView( vCopy );

   if ( nRC >= zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Operation Code cannot be moved, because\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Operation is related to an other Domain.\n\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Operation Code will be copied." );
      MessageSend( vSubtask, "OP00819", "Domain Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      nCopyOrMoveFlag = zCOPY_OPERATION;
   }
   else
   {
      SetCursorFirstEntityByString( vSource, "Operation",
                                    "Name", szOperationName, "" );
      DeleteEntity( vSource, "Operation", zREPOS_PREV );
      nCopyOrMoveFlag = zMOVE_OPERATION;
   }

   return( nCopyOrMoveFlag );
} // zwTZDMUPDD_RemoveOperFromGroup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CreateOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CreateOperation( zVIEW  vSubtask,
                            zVIEW  vSource,
                            zVIEW  vTarget,
                            zSHORT nMoveFlag )
{
   zVIEW    vCopySource;
   zSHORT   nRC;
   zSHORT   nCopyOrMoveFlag  = -1;
   zCHAR    szOperationName[ 33 ];

   if ( CheckExistenceOfEntity( vSource, "SelectedOperation" ) < zCURSOR_SET )
      return( zDM_NOREPLACE_CODE );

   GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vSource, "SelectedOperation", "Name" );

   nRC = SetCursorFirstEntityByString( vTarget, "Operation",
                                       "Name", szOperationName, "" );
   if ( nRC < zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vTarget, "Operation", zPOS_AFTER );
      SetMatchingAttributesByName( vTarget, "Operation",
                                   vSource, "Operation", zSET_NULL );
      CreateViewFromViewForTask( &vCopySource, vSource, 0 );
      nRC = SetCursorFirstEntity( vCopySource, "Parameter", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CreateMetaEntity( vSubtask, vTarget, "Parameter", zPOS_AFTER );
         SetMatchingAttributesByName( vTarget, "Parameter",
                                      vCopySource, "Parameter", zSET_NULL );
         nRC = SetCursorNextEntity( vCopySource, "Parameter", "" );
      }
      DropView( vCopySource );
   }
   else
   {
      nCopyOrMoveFlag = zwfnTZDMUPDD_CreateOperation( vSubtask, vTarget, vSource,
                                                      szOperationName, nMoveFlag );
      if ( nCopyOrMoveFlag < 0 )
         return( -1 );
   } // endif ( nRC < zCURSOR_SET )

   SetCursorFirstEntityByString( vTarget, "Operation", "Name", szOperationName, "" );
   IncludeSubobjectFromSubobject( vTarget, "SelectedOperation",
                                  vTarget, "Operation", zPOS_AFTER );

   if ( nCopyOrMoveFlag == zDM_NOREPLACE_CODE )
      return( zDM_NOREPLACE_CODE );

   if ( nCopyOrMoveFlag == zDM_REPLACE_CODE )
   {
      // Operation exists in Target Group and user world replace
      SetMatchingAttributesByName( vTarget, "Operation",
                                   vSource, "Operation", zSET_NOTNULL );
      // Create Work LOD for Delete Operation from File
      zwTZDMUPDD_CreateDeleteWorkLod( vSubtask, vTarget, szOperationName );
      // delete Operation Code from Target Group
      zwTZDMUPDD_DeleteOperFromFile( vSubtask, vTarget, zDELETE_OPERATION );
   }

   if ( nMoveFlag == zMOVE_OPERATION )
      nCopyOrMoveFlag = zwTZDMUPDD_RemoveOperFromGroup( vSubtask, vSource,
                                                        szOperationName );
   else
   {
      if ( nCopyOrMoveFlag == -1 )
         nCopyOrMoveFlag = zCOPY_OPERATION;
   }

   return( nCopyOrMoveFlag );
} // zwTZDMUPDD_CreateOperation

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CreateContext
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CreateContext( zVIEW vSubtask,
                          zVIEW vSourceView,
                          zVIEW vTarget )
{
   zSHORT   nRC;
   zVIEW    vSource;

   CreateViewFromViewForTask( &vSource, vSourceView, 0 );

   // Create Context
   nRC = SetCursorFirstEntity( vSource, "Context", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateMetaEntity( vSubtask, vTarget, "Context", zPOS_AFTER );
      SetMatchingAttributesByName( vTarget, "Context", vSource, "Context", zSET_NULL );

      nRC = SetCursorFirstEntity( vSource, "TableEntry", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CreateMetaEntity( vSubtask, vTarget, "TableEntry", zPOS_AFTER );
         SetMatchingAttributesByName( vTarget, "TableEntry",
                                      vSource, "TableEntry", zSET_NULL );
         nRC = SetCursorFirstEntity( vSource, "ExtValueNLS_Text", "" );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            CreateMetaEntity( vSubtask, vTarget, "ExtValueNLS_Text", zPOS_AFTER );
            SetMatchingAttributesByName( vTarget, "ExtValueNLS_Text",
                                         vSource, "ExtValueNLS_Text", zSET_NULL );
            nRC = SetCursorNextEntity( vSource, "ExtValueNLS_Text", "" );
         }

         nRC = SetCursorNextEntity( vSource, "TableEntry", "" );
      }

      nRC = SetCursorFirstEntity( vSource, "RegularExpression", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CreateMetaEntity( vSubtask, vTarget, "RegularExpression", zPOS_AFTER );
         SetMatchingAttributesByName( vTarget, "RegularExpression",
                                      vSource, "RegularExpression", zSET_NULL );
         nRC = SetCursorNextEntity( vSource, "RegularExpression", "" );
      }

      nRC = SetCursorFirstEntity( vSource, "DIL_NLS_Text", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CreateMetaEntity( vSubtask, vTarget, "DIL_NLS_Text", zPOS_AFTER );
         SetMatchingAttributesByName( vTarget, "DIL_NLS_Text",
                                      vSource, "DIL_NLS_Text", zSET_NULL );
         nRC = SetCursorNextEntity( vSource, "DIL_NLS_Text", "" );
      }

      nRC = SetCursorNextEntity( vSource, "Context", "" );
   }

   DropView( vSource );

   return( 0 );
} // zwTZDMUPDD_CreateContext

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CreateContextAndOper
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZDMUPDD_CreateContextAndOper( zVIEW  vSubtask,
                                 zVIEW  vSource,
                                 zVIEW  vTarget,
                                 zSHORT nMoveFlag )
{
   zSHORT   nCopyOrMoveFlag;

   // Create Operation
   nCopyOrMoveFlag = zwTZDMUPDD_CreateOperation( vSubtask, vSource,
                                                 vTarget, nMoveFlag );

   if ( nCopyOrMoveFlag >= 0 )
   {
      // Create Context
      zwTZDMUPDD_CreateContext( vSubtask, vSource, vTarget );
   }

   return( nCopyOrMoveFlag );
} //zwTZDMUPDD_CreateContextAndOper

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_MoveCheckGroupType
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_MoveCheckGroupType( zVIEW  vSubtask,
                               zVIEW  vTZDGSRCO,
                               zSHORT nCheckIdenticalGroup )
{
   zVIEW    vMoveDomain;
   zVIEW    vWindow;
   zCHAR    szMsg[ 100 ];
   zCHAR    szControlText[ 4 ];
   zCHAR    szLanguageType[ 4 ];
   zCHAR    szGroupName[ 33 ];

   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vSubtask, &vWindow, "MoveDomain" );

   // if move Domain with Operation Code selected and Domain has a Operation
   if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) >= zCURSOR_SET &&
        GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      GetCtrlText( vWindow, "txtType", szControlText, 4 );
      GetStringFromAttributeByContext( szLanguageType, vMoveDomain, "SourceFile",
                                       "LanguageType", "LanguageType", 4 );

      if ( zstrcmp( szControlText, szLanguageType ) != 0 )
      {
         zSHORT nRC;

         strcpy_s( szMsg, sizeof( szMsg ), "Language Type of Domain Group do not match.\n"
                         "Are you sure you want to move the domain?" );
         nRC = OperatorPrompt( vSubtask, "Move Domain", szMsg, 0, zBUTTONS_YESNO,
                               zRESPONSE_NO, zICON_QUESTION );
         if ( nRC == zRESPONSE_NO )
         {
            MessageSend( vSubtask, "OP00819", "Domain Maintenance",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
            return( -1 );
         }
      }
   }

   // If source and target group identical ... error!
   if ( nCheckIdenticalGroup == 1 )
   {
      GetCtrlText( vWindow, "txtDomainGroup", szGroupName, 33 );
      if ( CompareAttributeToString( vMoveDomain, "SourceFile", "Name",
                                     szGroupName ) == 0 )
      {
         MessageSend( vSubtask, "OP00819", "Domain Maintenance",
                      "Target and Source Domain Group are identical.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }
   }

   return( 0 );
} // zwTZDMUPDD_MoveCheckGroupType

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_DeleteOperFromFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_DeleteOperFromFile( zVIEW  vSubtask,
                               zVIEW  vTZDGSRCO,
                               zSHORT nMoveOrDeleteFlag )
{
   zVIEW  vProfileXFER;
   zVIEW  vTaskLPLR;
   zVIEW  vEdWrk;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );

   SetNameForView( vTZDGSRCO, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vProfileXFER, "ED", "InvokingTool", "GO" );

   if ( vEdWrk == 0 )
   {
      ActivateEmptyObjectInstance( &vEdWrk, "TZEDWRKO", vTaskLPLR, zSINGLE );

      // Create empty root.
      CreateEntity( vEdWrk, "Editor", zPOS_FIRST );
      SetNameForView( vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );
   }

   SetAttributeFromString( vProfileXFER, "ED", "InvokeEditorCommand", "S" );

   if ( nMoveOrDeleteFlag == zDELETE_OPERATION )
   {
      // Delete Operation from File
      TZEDFRMD_DropOperationFromSource( vSubtask );
   }
   else
   if ( nMoveOrDeleteFlag == zMOVE_OPERATION )
   {
      // Copy or Move Operation from File to File
      TZEDFRMD_MoveOperationCode( vSubtask, nMoveOrDeleteFlag );
   }

   if ( nMoveOrDeleteFlag == zCOPY_OPERATION )
   {
      // Copy Operation from File to File
      TZEDFRMD_CopyOperationCode( vSubtask, zNORENAME_OPERATION );
   }

   return( 0 );
} // zwTZDMUPDD_DeleteOperFromFile

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_MoveDomainOperToFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_MoveDomainOperToFile( zVIEW  vSubtask,
                                 zSHORT nCopyOrMoveFlag )
{
   zVIEW  vMoveDomain;
   zVIEW  vMoveDomain_Copy;
   zVIEW  vDeleteOp;
   zVIEW  vTZDGSRCO;
   zVIEW  vWindow;
   zCHAR  szGroupName[ 33];
   zCHAR  szOperationName[ 33 ];

   if ( nCopyOrMoveFlag <= 0 )
      return( 0 );

   // Move Domain Operation Code to new Domain Group
   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vWindow, "MoveDomain" );

   if ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" ) >= zCURSOR_SET &&
        GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      CreateViewFromViewForTask( &vMoveDomain_Copy, vMoveDomain, 0 );

      // Create LOD for move Operation Code from old Domain Group
      if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) >= 0 )
         DropObjectInstance( vDeleteOp );

      ActivateEmptyObjectInstance( &vDeleteOp, "TZEDWDEL", vSubtask, zMULTIPLE );

      // Create Source File
      CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
      GetCtrlText( vWindow, "txtDomainGroup", szGroupName, 33 );
      SetCursorFirstEntityByString( vMoveDomain_Copy, "SourceFile", "Name",
                                    szGroupName, "" );
      SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                   vMoveDomain_Copy, "SourceFile", zSET_ALL );
      SetNameForView( vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );
      DropView( vMoveDomain_Copy );

      // Create Operation
      CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
      GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vTZDGSRCO, "SelectedOperation", "Name" );
      SetAttributeFromString( vDeleteOp, "Operation", "Name", szOperationName );

      // Added new Sourcen File for move Opertion to new File
      CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                   vMoveDomain, "SourceFile", zSET_ALL );

      // Move Operation from Domain Group
      zwTZDMUPDD_DeleteOperFromFile( vWindow, vTZDGSRCO, nCopyOrMoveFlag );

   } //endif ( CheckExistenceOfEntity( vTZDGSRCO, "SelectedOperation" )...

   return( 0 );
} // zwTZDMUPDD_MoveDomainOperToFile

/////////////////////////////////////////////////////////////////////////////
//  Move a Domain from the current Group (TZDGSCRO) to the Group selected
//  in view CM_ListGroup
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_MoveDomainToNewGroup( zVIEW vSubtask )
{
   zVIEW   vGroupList;
   zVIEW   vDomainList;
   zVIEW   vTarget;
   zVIEW   vSource;
   zVIEW   vProfileXFER;
   zVIEW   vWindow;
   zVIEW   vMoveDomain;
   zCHAR   szDomainName[ 33 ];
   zSHORT  nCopyOrMoveFlag = 0;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vDomainList, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vGroupList, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );

   //was window <Move Domain from Group to Group> opened via Move Button or menu item
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DOMLIST" ) >= 0 )
      // Get the Domain to be moved.
      ActivateMetaOI( vSubtask, &vSource, vDomainList, zSOURCE_DOMAIN_META, zSINGLE );
   else
      GetViewByName( &vSource, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vSource, "Domain", "Name" );

   // Move Domain in a new Domain Group?
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_MoveDomainInNewGroup ) == 0 )
   {
      GetViewByName( &vTarget, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
      RetrieveViewForMetaList( vSubtask, &vGroupList, zREFER_DOMAINGRP_META );
      // added new Domain Group to Work LOD
      CreateEntity( vMoveDomain, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vMoveDomain, "SourceFile", vTarget, "DomainGroup", zSET_ALL );
      if ( zwTZDMUPDD_MoveCheckGroupType( vSubtask, vSource, 0 ) < 0 )
      {
         DeleteEntity( vMoveDomain, "SourceFile", zPOS_PREV );
         return( -1 );
      }
   }
   else
   {
      if ( zwTZDMUPDD_MoveCheckGroupType( vSubtask, vSource, 1 ) < 0 )
         return( -1 );

      // Get the Domain Group which will hold the new Domain
      ActivateMetaOI( vSubtask, &vTarget, vGroupList, zSOURCE_DOMAINGRP_META, zSINGLE );
      CreateEntity ( vTarget, "Domain", zPOS_AFTER );
      // Keep the same ZKey so that we won't have to rebuild XODs.
      SetAttributeFromAttribute( vTarget, "Domain", "ZKey", vSource, "Domain", "ZKey" );
   }

   // reuse ZKey
   SetMatchingAttributesByName( vTarget, "Domain", vSource, "Domain", zSET_ALL );
   // Create Context And Operation
   nCopyOrMoveFlag = zwTZDMUPDD_CreateContextAndOper( vSubtask, vSource, vTarget, zMOVE_OPERATION );
   if ( nCopyOrMoveFlag < 0 )
      return( -1 );

   DeleteEntity( vSource, "Domain", zREPOS_NEXT );

   // First COMMIT the Domain Group from which the Domain was deleted.
   CommitMetaOI( vSubtask, vSource, zSOURCE_DOMAINGRP_META );

   // Second, COMMIT the Domain Group with the new Domain.  This order is
   // important because if we reverse it, the Domain will be added twice.
   CommitMetaOI( vSubtask, vTarget, zSOURCE_DOMAINGRP_META );

   // Finally, drop the source Domain Group and the current TZDGSRCO Domain
   // Group, if there is one, and name the target Domain Group TZDGSRCO.
   DropMetaOI( vSubtask, vSource );

   if ( GetViewByName( &vSource, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vSource );

   SetNameForView( vTarget, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByString( vTarget, "Domain", "Name", szDomainName, "" );
   SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", szDomainName, "" );
   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "" );

   // move Domain Operation Code
   if ( nCopyOrMoveFlag > zDM_NOREPLACE_CODE )
      zwTZDMUPDD_MoveDomainOperToFile( vSubtask, nCopyOrMoveFlag );

   // Drop Work LOD for Move Operation
   DropObjectInstance( vMoveDomain );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZDMUPDD" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CheckGroupName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CheckGroupName( zVIEW vSubtask,
                           zVIEW vTZDGSRCO_DETAIL,
                           zVIEW vCM_ListGroup )
{
   zSHORT  nRC;

   if ( CompareAttributeToString( vTZDGSRCO_DETAIL, "DomainGroup",
                                  "Name", "" ) == 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      MessageSend( vSubtask, "DM00201", "Domain Maintenance",
                   "Domain Group Name required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      SetFocusToCtrl( vSubtask, "DomainGroupName" );
      return( -1 );
   }

   nRC = SetCursorFirstEntityByAttr( vCM_ListGroup, "W_MetaDef", "Name",
                                     vTZDGSRCO_DETAIL, "DomainGroup",
                                     "Name", "" );
   if ( nRC >= zCURSOR_SET )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      MessageSend( vSubtask, "DM00202", "Domain Maintenance",
                   "A Domain Group already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetFocusToCtrl( vSubtask, "DomainGroupName" );
      return( -1 );
   }

   return( 0 );
} // zwTZDMUPDD_CheckGroupName

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CheckGroupType
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_CheckGroupType( zVIEW vSubtask,
                           zVIEW vTZDGSRCO_DETAIL )
{
   zCHAR  szLanguageType[ 4 ];

   GetStringFromAttribute( szLanguageType, sizeof( szLanguageType ),
                           vTZDGSRCO_DETAIL, "DomainGroup", "LanguageType" );

   if ( szLanguageType[ 0 ] == '\0f' )
   {
      MessageSend( vSubtask, "OP00806", "Domain Maintenance",
                   "A Language Type was not selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Validate that the use of DLL name and Language Type is consistent.
   // However an error is only a warning.
   if ( (CompareAttributeToString( vTZDGSRCO_DETAIL, "DomainGroup",
                                   "DomainAndGlobalOpGroupName", "" ) != 0) &&
        (CompareAttributeToString( vTZDGSRCO_DETAIL, "DomainGroup",
                                   "LanguageType", "" ) == 0) )
   {
      MessageSend( vSubtask, "DM00203", "Domain Maintenance",
                   "Language Type should be entered "
                   "when Executable Name is entered.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edExecutable" );
      return( -1 );
   }

   if ( (CompareAttributeToString( vTZDGSRCO_DETAIL, "DomainGroup",
                                   "DomainAndGlobalOpGroupName", "" ) == 0) &&
        (CompareAttributeToString( vTZDGSRCO_DETAIL, "DomainGroup",
                                   "LanguageType", "" ) != 0) )
   {
      MessageSend( vSubtask, "DM00204", "Domain Maintenance",
                   "Executable Name should be entered when Language Type is entered.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edExecutable" );
      return( -1 );
   }

   // that the Extension attribute is set according to the Language type.
   if ( szLanguageType[ 0 ] == 'C' )
      SetAttributeFromString( vTZDGSRCO_DETAIL, "DomainGroup", "Extension",
                              "C" );

   return( 0 );
} // zwTZDMUPDD_CheckGroupType

zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_AcceptDomainGroup( zVIEW vSubtask )
{
   zVIEW   vTZDGSRCO_DETAIL;
   zVIEW   vCM_ListGroup;
   zVIEW   vProfileXFER;
   zVIEW   vParentWindow;
   zCHAR   szDomainGroup[ 33];
   zCHAR   szControlText[ 4 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szDomainGroup, sizeof( szDomainGroup ), vTZDGSRCO_DETAIL, "DomainGroup", "Name" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZDGSRCO_DETAIL, zSOURCE_DOMAINGRP_META ))
      return( 0 );

   // First validate that a valid name has been entered (meaning it is not blank and not
   // an existing Domain Group name.  Do these checks only if this is a new Domain
   // Group function.
   if ( (CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                   zDM_NewDomain ) == 0) ||
        (CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                   zDM_MoveDomainInNewGroup ) == 0) ||
        (CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                   zDM_SaveAsDomainInNewGroup ) == 0) )
   {
      if ( zwTZDMUPDD_CheckGroupName( vSubtask, vTZDGSRCO_DETAIL,
                                      vCM_ListGroup ) < 0 )
         return( -1 );
   }

   // Validate that the use of DLL name and Language Type is consistent.
   // However an error is only a warning.
   if ( zwTZDMUPDD_CheckGroupType( vSubtask, vTZDGSRCO_DETAIL ) < 0 )
      return( -1 );

   // If this is part of a new Domain Group function, set the view name
   // TZDGSRCO to this view so the correct domain group will appear upon
   // return to the main window.
   //BL, 1999.10.29 Move Domain in the new DomainGroup
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_MoveDomainInNewGroup ) == 0 )
      zwTZDMUPDD_MoveDomainToNewGroup( vSubtask );
   else
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_NewDomain ) == 0 )
   {
      SetNameForView( vTZDGSRCO_DETAIL, "TZDGSRCO", vSubtask, zLEVEL_TASK );
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                              zDM_ExitNewDomain );
   }
   else
   // If this is part of an Update Domain Group function, commit the Domain Group.
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_UpdateGroupDetail ) == 0 )
   {
      CommitMetaOI( vSubtask, vTZDGSRCO_DETAIL, zSOURCE_DOMAINGRP_META );
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "" );
   }
   else
   //if Save As?
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_SaveAsDomainInNewGroup ) == 0 )
   {
      GetParentWindow( &vParentWindow, vSubtask );
      SetCtrlText( vParentWindow, "txtDomainGroup", szDomainGroup );
      RefreshCtrl( vParentWindow, "txtDomainGroup" );
      GetStringFromAttributeByContext( szControlText, vTZDGSRCO_DETAIL, "DomainGroup",
                                       "LanguageType", "LanguageType", 4 );
      SetCtrlText( vParentWindow, "txtType", szControlText );
      RefreshCtrl( vParentWindow, "txtType" );
      zwTZDMUPDD_SaveAsDomain( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsInNewGroup
//
// Save Domain as in new Domain Group
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SaveAsInNewGroup( zVIEW vSubtask )
{
   zVIEW  vTZDGSRCO_DETAIL;
   zVIEW  vProfileXFER;
   zVIEW  vCM_List;
   zCHAR  szOutName[ 33 ];
   zCHAR  szNewName[ 33 ];
   zCHAR  szGroupName[ 33];
   zCHAR  szMsg[ 100 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vProfileXFER, "DM", "DomainName" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   // Check that a domain name has been entered.
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      MessageSend( vSubtask, "DM00103", "Domain Maintenance",
                   "Domain name required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "" );
      SetFocusToCtrl( vSubtask, "edDomainName" );
      return( -1 );
   }

   //Check: if existing Domain?
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                     szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      GetStringFromAttribute( szGroupName, sizeof( szGroupName ), vCM_List, "W_MetaDef", "GroupName" );
      strcpy_s( szMsg, sizeof( szMsg ), "Cannot create a new Domain Group for an existing Domain.\n" );
      strcat_s( szMsg, sizeof( szMsg ), "Domain exists in Doamin Group '");
      strcat_s( szMsg, sizeof( szMsg ), szGroupName );
      strcat_s( szMsg, sizeof( szMsg ), "'.");
      MessageSend( vSubtask, "DM00103", "Domain Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "" );
      return( -1 );
   }

   ActivateEmptyMetaOI( vSubtask, &vTZDGSRCO_DETAIL, zSOURCE_DOMAINGRP_META, zSINGLE );
   CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "DomainGroup", zPOS_AFTER );

   SetNameForView( vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                           zDM_SaveAsDomainInNewGroup );

   return( 0 );
} // zwTZDMUPDD_SaveAsInNewGroup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_Check_CheckoutStatus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_Check_CheckoutStatus( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_ListGrp;
   zVIEW   vCM_ListGroup;
   zVIEW   vTaskLPLR;
   zVIEW   vXDM;
   zSHORT  nEnable = 0;
   zSHORT  nRC = 0;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vCM_ListGrp, "CM_ListGroup", vSubtask, zLEVEL_TASK ) >= 0 &&
        GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      CreateViewFromViewForTask( &vCM_ListGroup, vCM_ListGrp, 0 );
      SetCursorFirstEntityByInteger( vCM_ListGroup, "W_MetaType", "Type",
                                     zREFER_DOMAINGRP_META, "" );

      if ( CheckExistenceOfEntity( vCM_ListGroup, "W_MetaDef" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      {
         // Position on the correct Operation within the group type.
         SetCursorFirstEntityByAttr( vCM_ListGroup, "W_MetaDef", "Name",
                                     vCM_List, "W_MetaDef", "GroupName", "" );

         nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );
         if ( nEnable )
            bEnable = TRUE;
      }
      DropView( vCM_ListGroup );
   }


   SetCtrlState( vSubtask, "Delete", zOPTION_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Move", zOPTION_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "DeleteDomain", bEnable );

   // KJS 06/24/15 - Adding work entities that contain the different JavaClass options available for
   // domains. This is so that the user can select classes from a list.
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( CheckExistenceOfEntity( vTaskLPLR, "DomainJavaClass" ) < zCURSOR_SET )
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
      //kkkkkkk
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.StaticTableDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.StaticTableDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.DynamicTableDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.DynamicTableDomain" );
      }
/////////
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.BlobDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.BlobDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.DateDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.DateDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.DoubleDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.DoubleDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.EmailAddressDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.EmailAddressDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.IntegerDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.IntegerDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.PasswordDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.PasswordDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.StringDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.StringDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.SSNDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.SSNDomain" );
      }
      if ( SetCursorFirstEntityByString( vTaskLPLR, "DomainJavaClass",
                                         "JavaClassName", "com.quinsoft.zeidon.domains.TimeDomain", 0 ) < zCURSOR_SET )
      {
         CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DomainJavaClass", "JavaClassName", "com.quinsoft.zeidon.domains.TimeDomain" );
      }

      // After building domains that exist in zeidon-joe, we should look at the xdm file and add any new domains that
      // have been added for this LPLR project.
      nRC = oTZDMXGPO_GetViewForXDM( vSubtask, &vXDM, zCURRENT_OI );
      if ( nRC >= 0 )
      {
         for ( nRC = SetCursorFirstEntity( vXDM, "Domain", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vXDM, "Domain", "" ) )
         {


            if ( SetCursorFirstEntityByAttr( vTaskLPLR, "DomainJavaClass", "JavaClassName",
                                             vXDM, "Domain", "JavaClass", "" ) < zCURSOR_SET )
            {
               CreateEntity( vTaskLPLR, "DomainJavaClass", zPOS_AFTER );
               SetAttributeFromAttribute( vTaskLPLR, "DomainJavaClass", "JavaClassName", vXDM, "Domain", "JavaClass" );
            }

        }
      }
   }

   return( 0 );
} // zwTZDMUPDD_Check_CheckoutStatus

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_GroupCheckedOut
//
// is Domain Group checked out
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_GroupCheckedOut( zVIEW vSubtask )
{
   zVIEW    vCM_ListGroup;
   zSHORT   nEnable = 0;

   if ( GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK ) >= 0 )
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );

   return( nEnable );
} // zwTZDMUPDD_GroupCheckedOut

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SetCeckOutState
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_SetCeckOutState( zVIEW vSubtask )
{
   zSHORT  nEnable = 0;
   zVIEW   vMoveDomain;
   zVIEW   vCM_ListGroup;

   if ( GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         SetCursorFirstEntityByAttr( vCM_ListGroup, "W_MetaDef", "Name",
                                     vMoveDomain, "SourceFile", "Name", "" );
      }

      nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
} // zwTZDMUPDD_SetCeckOutState

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_NewDomain
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_NewDomain( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwTZDMUPDD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Domain not checked out and user change this Domain, then zeidon
            // call the window "Save Domain as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Domain or Open an other Domain or Switch Project or
            // Exit Domain Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNewComponentAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   return( 0 );
} // zwTZDMUPDD_NewDomain

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_OpenDomain
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZDMUPDD_OpenDomain( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zVIEW  vTaskLPLR;
   zSHORT nRC;

   // Ask and do save
   nRC = zwTZDMUPDD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Domain not checked out and user change this Domain, then zeidon
            // call the window "Save Domain as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Domain or Open an other Domain or Switch Project or
            // Exit Domain Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zOpenComponentAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

   // Position on the blank entry in DomainJavaClass
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
      SetCursorFirstEntity( vTaskLPLR, "DomainJavaClass", "" );


      return( 0 );
   }

   return( 0 );
} // zwTZDMUPDD_OpenDomain

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsKeepCurrentDesc
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vTZDGSRCO;
   zVIEW  vProfileXFER;

   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                                 vTZDGSRCO, "Domain", "Desc" );
      RefreshCtrl( vSubtask, "edDomainDesc" );
   }

   return( 0 );
} // zwTZDMUPDD_SaveAsKeepCurrentDesc

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SetFlagForNewDomain
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_SetFlagForNewDomain( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag",
                                  zDM_ExitNewDomain ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "" );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
   }
   else
   {
      // set flag for new Operation
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                              zDM_NewDomain );
   }

   return( 0 );
} // zwTZDMUPDD_SetFlagForNewDomain


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_ShowDetail
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_ShowDetail( zVIEW vSubtask )
{
   zVIEW    vCM_ListGroup;
   zVIEW    vProfileXFER;
   zVIEW    vTZDGSRCO;
   zVIEW    vDomain;
   zVIEW    vMoveDomain;
   zBOOL    bLoadCurrentGroup = FALSE;

   // Activate the Operation Group Detail for the Oper Group Update window.
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( CheckExistenceOfEntity( vCM_ListGroup, "W_MetaDef" ) < zCURSOR_SET ||
           CompareAttributeToAttribute( vCM_ListGroup, "W_MetaDef", "CPLR_ZKey",
                                        vTZDGSRCO, "DomainGroup", "ZKey" ) == 0 )
      {
         bLoadCurrentGroup = TRUE;
      }
      else
      if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) >= 0 &&
           CompareAttributeToAttribute( vMoveDomain, "SourceFile", "ZKey",
                                        vTZDGSRCO, "DomainGroup", "ZKey" ) == 0 )
      {
         bLoadCurrentGroup = TRUE;
      }
   }

   if ( bLoadCurrentGroup )
   {
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                              zDM_LoadGroupProperties );
      if ( GetViewByName( &vDomain, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         DropView( vDomain );
      }
      GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vDomain, vTZDGSRCO, 0 );
      SetNameForView( vDomain, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   }
   else
   {
      // Activate and Name
      ActivateMetaOI( vSubtask, &vDomain, vCM_ListGroup, zSOURCE_DOMAINGRP_META, zSINGLE );
      SetNameForView( vDomain, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

      // set flag for show Group Detail
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                              zDM_UpdateGroupDetail );
   }

   return( 0 );
} // zwTZDMUPDD_ShowDetail

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_CreateWorkLOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_CreateWorkLOD( zVIEW vSubtask )
{
   zVIEW  vMoveDomain;
   zVIEW  vDomainGrp;
   zVIEW  vTZDGSRCO;
   zVIEW  vCM_Group_Copy;
   zVIEW  vCM_ListGroup;
   zSHORT nRC;

   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vCM_Group_Copy, vCM_ListGroup, 0 );

   if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveDomain );

   ActivateEmptyObjectInstance( &vMoveDomain, "TZEDWDEL", vSubtask, zMULTIPLE );

   for ( nRC = SetCursorFirstEntity( vCM_Group_Copy, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vCM_Group_Copy, "W_MetaDef", "" ) )
   {
      // Activate and Name
      ActivateMetaOI( vSubtask, &vDomainGrp, vCM_Group_Copy, zREFER_DOMAINGRP_META, zSINGLE );
      CreateEntity( vMoveDomain, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vMoveDomain, "SourceFile",
                                   vDomainGrp, "DomainGroup", zSET_ALL );
      SetAttributeFromAttribute( vMoveDomain, "SourceFile", "Status",
                                 vCM_Group_Copy, "W_MetaDef", "Status" );
      DropMetaOI( vSubtask, vDomainGrp );
   }

   SetNameForView( vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   DropView( vCM_Group_Copy );

   // load Values from current Group
   if ( GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( SetCursorFirstEntityByAttr( vMoveDomain, "SourceFile", "ZKey",
                                       vTZDGSRCO, "DomainGroup", "ZKey",
                                       "" ) < zCURSOR_SET )
      {
         // current Domain Group is a new Group, is not in CM_ListGroup
         CreateEntity( vMoveDomain, "SourceFile", zPOS_AFTER );
         SetMatchingAttributesByName( vMoveDomain, "SourceFile",
                                      vTZDGSRCO, "DomainGroup", zSET_ALL );
         SetAttributeFromInteger( vMoveDomain, "SourceFile", "Status", 1 );
      }
      else
      {
         SetMatchingAttributesByName( vMoveDomain, "SourceFile",
                                      vTZDGSRCO, "DomainGroup", zSET_ALL );
      }
   }

   return( 0 );
} // zwTZDMUPDD_CreateWorkLOD

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_MoveRtnFromSubwindow
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_MoveRtnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vMoveDomain;
   zVIEW    vTZDGSRCO;
   zLONG    lZKey;
   zCHAR    szLanguageType[ 4 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // set flag for new Domain
   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                           zDM_MoveDomain );

   // save ZKey for set cursor to selected source file
   GetIntegerFromAttribute( &lZKey, vMoveDomain, "SourceFile", "ZKey" );
   zwTZDMUPDD_CreateWorkLOD( vSubtask );

   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vMoveDomain, "SourceFile", "ZKey", lZKey, "" );

   // if change current Group, set new Values
   if ( GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CompareAttributeToInteger( vTZDGSRCO, "DomainGroup", "ZKey",
                                   lZKey ) == 0 )
   {
      GetStringFromAttributeByContext( szLanguageType, vMoveDomain, "SourceFile",
                                       "LanguageType", "LanguageType", 4 );
      SetCtrlText( vSubtask, "txtType", szLanguageType );
   }

   RefreshWindow( vSubtask );

   return( 0 );
} // zwTZDMUPDD_MoveRtnFromSubwindow

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SaveAsRtnFromSubwnd
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_SaveAsRtnFromSubwnd( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vMoveDomain;
   zCHAR    szGroupName[ 33];
   zCHAR    szLanguageType[ 4 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // set flag for new Domain
   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag",
                           zDM_SaveAsDomain );

   // save GroupName for set cursor to selected Domain Group
   GetCtrlText( vSubtask, "txtDomainGroup", szGroupName, 33 );
   zwTZDMUPDD_CreateWorkLOD( vSubtask );

   GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByString( vMoveDomain, "SourceFile", "Name", szGroupName, "" );

   // Set new LanguageType
   GetStringFromAttributeByContext( szLanguageType, vMoveDomain, "SourceFile",
                                    "LanguageType", "LanguageType", 4 );
   SetCtrlText( vSubtask, "txtType", szLanguageType );
   SetSelectStateOfEntity( vMoveDomain, "SourceFile", 1 );

   RefreshWindow( vSubtask );

   return( 0 );
} // zwTZDMUPDD_SaveAsRtnFromSubwnd

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_AcceptSubobject
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_AcceptSubobject( zVIEW vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szAttributeName,
                            zPCHAR szMessageText )
{
   zVIEW    vTZDGSRCO;
   zVIEW    vTZDGSRCO_Copy;
   zCHAR    szNewName[ 255 ];
   zCHAR    szMessage[ 355 ];
   zSHORT   nRC;

   GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vTZDGSRCO, szEntityName, szAttributeName );

   if ( zstrcmp( szEntityName, "TableEntry" ) &&
        zstrcmp( szEntityName, "Operation" ) != 0 )
   {
      zGetStringWithoutBlank( szNewName, szNewName );

      if ( zstrcmp( szNewName, "") == 0 )
      {
         strcpy_s( szMessage, sizeof( szMessage ), szMessageText );
         strcat_s( szMessage, sizeof( szMessage ), " Name is required.");
         MessageSend( vSubtask, "PE00114", "Domain Maintenance",
                      szMessage,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "ContextName" );
         return( -1 );
      }
   }

   CreateViewFromViewForTask( &vTZDGSRCO_Copy, vTZDGSRCO, 0 );

   nRC = SetCursorFirstEntityByString( vTZDGSRCO_Copy, szEntityName,
                                       szAttributeName, szNewName, "" );
   nRC = SetCursorNextEntityByString( vTZDGSRCO_Copy, szEntityName,
                                      szAttributeName, szNewName, "" );
   DropView( vTZDGSRCO_Copy );

   if ( nRC == zCURSOR_SET )
   {
      strcpy_s( szMessage, sizeof( szMessage ), "A " );
      strcat_s( szMessage, sizeof( szMessage ), szMessageText );
      strcat_s( szMessage, sizeof( szMessage ), " already exists by this name!" );
      MessageSend( vSubtask, "OP00814", "Domain Maintenance",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "ContextName" );
      return( -1 );
   }

   if ( AcceptSubobject( vTZDGSRCO, szEntityName ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );

} // zwTZDMUPDD_AcceptSubobject

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_SetNullEntry
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_SetNullEntry( zVIEW  vSubtask,
                         zVIEW  vTZDGSRCO )
{
   zVIEW  vTZDGSRCO_Copy;
   zSHORT nRC;

   CreateViewFromViewForTask( &vTZDGSRCO_Copy, vTZDGSRCO, 0 );

   //  If there is no table entry with a null internal value, create
   //  one with both null internal and external value.  This is so that
   //  the Domain table handler will give a blank entry to the Combo
   //  Box control.
   for ( nRC = SetCursorFirstEntity( vTZDGSRCO_Copy, "Context", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZDGSRCO_Copy, "Context", "" ) )
   {
      if ( SetCursorFirstEntity( vTZDGSRCO_Copy, "TableEntry", "" ) >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByString( vTZDGSRCO_Copy, "TableEntry",
                                         "InternalValue", "", "" ) < zCURSOR_SET )
         {
            nRC = CreateMetaEntity( vSubtask, vTZDGSRCO_Copy, "TableEntry", zPOS_BEFORE );
         }
      }
   }

   DropView( vTZDGSRCO_Copy );

   return( 0 );
} // zwTZDMUPDD_SetNullEntry

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_TableEntryOK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_TableEntryOK( zVIEW vSubtask )
{
   zVIEW    vDomainGrp;

   GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY );

   // Accept the current subobject.
   if ( zwTZDMUPDD_AcceptSubobject( vSubtask, "TableEntry",
                                    "InternalValue", "Internal Value" ) < 0 )
   {
      SetFocusToCtrl( vSubtask, "InternalName" );
      return( -1 );
   }

   return( 0 );
} // zwTZDMUPDD_TableEntryOK

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_LoadDomainGroupList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_LoadDomainGroupList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DOMGRPLIST" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZDMUPDD_LoadDomainGroupList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_LoadDomainList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_LoadDomainList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DOMLISTunderGRP" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZDMUPDD_LoadDomainList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_LoadOperationList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_LoadOperationList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZDMUPDD_LoadOperationList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_LoadPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZDMUPDD_LoadPopup( zVIEW vSubtask,
                      zPCHAR szEntityName,
                      zPCHAR szActionNameForNew,
                      zPCHAR szActionNameForDelete,
                      zPCHAR szPopupName )
{
   zVIEW  vDomainGrp;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META );

      if ( CheckExistenceOfEntity( vDomainGrp, szEntityName ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      // Set Default Context
      if ( zstrcmp( szPopupName, "ContextPopup" ) == 0 )
         EnableAction( vSubtask, "SetDefaultContext", bDelete );

      EnableAction( vSubtask, szActionNameForNew, bNew );
      EnableAction( vSubtask, szActionNameForDelete, bDelete );

      CreateTrackingPopupMenu( vSubtask, szPopupName, pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZDMUPDD_LoadPopup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_TablePopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_TablePopup( zVIEW vSubtask )
{
   zwTZDMUPDD_LoadPopup( vSubtask,
                         "TableEntry",
                         "AddTableEntry",
                         "DeleteTableEntry",
                         "TablePopup" );

   return( 0 );
} // zwTZDMUPDD_TablePopup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_OperationPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_OperationPopup( zVIEW vSubtask )
{
   zwTZDMUPDD_LoadPopup( vSubtask,
                         "Operation",
                         "NewOperation",
                         "DeleteOperation",
                         "OperationPopup" );

   return( 0 );
} // zwTZDMUPDD_OperationPopup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_DIL_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_DIL_Popup( zVIEW vSubtask )
{
   zVIEW  vDomainGrp;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO_NLS", vSubtask, zLEVEL_ANY ) > 0 &&
        CheckExistenceOfEntity( vDomainGrp, "Context" ) >= zCURSOR_SET )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META );

      if ( CheckExistenceOfEntity( vDomainGrp, "DIL_NLS_Text" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "NewText", bNew );
      EnableAction( vSubtask, "DeleteText", bDelete );

      CreateTrackingPopupMenu( vSubtask, "NLS_TextPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZDMUPDD_DIL_Popup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_ExtValue_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_ExtValue_Popup( zVIEW vSubtask )
{
   zVIEW  vDomainGrp;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vDomainGrp, "TZDGSRCO_NLS", vSubtask, zLEVEL_ANY ) > 0 &&
        CheckExistenceOfEntity( vDomainGrp, "TableEntry" ) >= zCURSOR_SET )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDomainGrp, zSOURCE_DOMAINGRP_META );

      if ( CheckExistenceOfEntity( vDomainGrp, "ExtValueNLS_Text" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "NewText", bNew );
      EnableAction( vSubtask, "DeleteText", bDelete );

      CreateTrackingPopupMenu( vSubtask, "NLS_ExternalValuePopup", pt.x, pt.y,
                               FALSE, FALSE );
   }

   return( 0 );
} // zwTZDMUPDD_ExtValue_Popup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZDMUPDD_MoveDomainCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZDMUPDD_MoveDomainCancel( zVIEW vSubtask )
{
   zVIEW vMoveDomain;

   if ( GetViewByName( &vMoveDomain, "MoveDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveDomain );

   return( 0 );
} // zwTZDMUPDD_MoveDomainCancel


#ifdef __cplusplus
}
#endif
