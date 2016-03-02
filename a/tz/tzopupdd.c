/*
CHANGE LOG
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2001.09.21  PAS
   Added temporary operation to initialize Report Group Exit operations
2001.03.01  BL
   If exiting Operation Tool, drop Operation View
2001.02.14  BL  R54697
   improve handling of return code from InitializeLPLR
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.09.22  BL
   Bugfix move Operation, reuse ZKey
2000.08.12  BL
   Modified fnzwTZOPUPDD_DeleteOperation, zwTZOPUPDD_SaveAsOperation,
   zwTZOPUPDD_SetGroupName and added fnzwTZOPUPDD_PromptForDelete,
   zwTZOPUPDD_CopyOperationWorkLOD, fnzwTZOPUPDD_DeleteFromXLP for
   save as with Operation Code
   Remove zwTZOPUPDD_CheckOperationGroup
2000.08.10  BL
   Added zwTZOPUPDD_SaveAsPreBuild and modified zwTZOPUPDD_SaveAsPostBuild
   for save as with Operation Code
2000.07.31  BL
   Modified zwTZOPUPDD_MoveOperationPreBuild, zwTZOPUPDD_SetExtension,
   zwTZOPUPDD_MoveOperationPostBuil, zwTZOPUPDD_EnableAndSetCheckOut,
   fnzwTZOPUPDD_DropOperFromFile, fnzwTZOPUPDD_DeleteOperation and
   added zwTZOPUPDD_CreateWorkLOD, zwTZOPUPDD_MoveOperationCancel,
   zwTZOPUPDD_MoveCheckGroupType, zwTZOPUPDD_MoveOperationToFile,
   zwTZOPUPDD_DeleteOperFromFile for move Operation with Operation Code
   Modified InitGroupDetail, zwTZOPUPDD_GrayGroupControls for load
   Group Properties for current Operation
   Rename zwTZOPUPDD_SetFlagForMoveOper to zwTZOPUPDD_MoveRtnFromSubwindow
2000.07.19  BL   TB 51372
   Added zwTZOPUPDD_LoadOperGroupList, zwTZOPUPDD_LoadOperationList
   for Bug 51372
2000.07.18  BL
   Added zwfnTZOPUPDD_InitMenuDisableAll, zwfnTZOPUPDD_LoadNewOperation,
   zwTZOPUPDD_CheckGroupName
   if Operation Group not checked out disable all function for update
2000.07.16  BL
   Added zwTZOPUPDD_AcceptSubobject, zwTZOPUPDD_DeleteParameter,
   zwTZOPUPDD_ParameterAccept, zwTZOPUPDD_ParameterNext, zwTZOPUPDD_ShowDetail,
   zwTZOPUPDD_SetFlagForNewOper, zwTZOPUPDD_SetFlagForMoveOper,
   zwTZOPUPDD_SetFlagForSaveAsOper and modified operations for completely
   maintained Global Operations tool.
   Added zwTZOPUPDD_SaveAsKeepCurrentDesc for save as
   Insert defines for save as
2000.05.24  BL
   remove warning
2000.05.02  BL
   Modified zwTZOPUPDD_CheckActionAfterSaveAs: after save as, set new title
2000.04.27  BL
   Modified zwTZOPUPDD_Check_CheckoutStatus: if Operation Group not
   checked out, disable Shortcut for delete Domain
   Remove SortComponents, the Open Window has a Listbox with Remember Sort
   Buttons
2000.02.29  BL  Z10  Repository
   Modified zwTZOPUPDD_MainRtnFromSubWnd and zwTZOPUPDD_LoadOperation for
   Operation List
2000.02.28  BL  Z10  Repository
   Modified zwTZOPUPDD_MainPostBuild and added zwTZOPUPDD_SaveAsCancel,
   zwTZOPUPDD_CheckActionAfterSaveAs, zwTZOPUPDD_NewOperation,
   zwTZOPUPDD_MenuOpenOperation zwTZOPUPDD_SwitchLPLR for save action
   after save as
   Modified zwfnPromptForSave, zwfnSaveOperation: if Operation Group
   not checked out and updated, this call window "Save Operation as"
   Replace "Global Operation" -> "Operation"
   Modified zwTZOPUPDD_OpenOperPreBuild for set cursor on current Operation
   Added zwTZOPUPDD_Check_CheckoutStatus, zwTZOPUPDD_InitMenu,
   zwTZOPUPDD_GroupCheckedOut, zwTZOPUPDD_EnableAndSetCheckOut
   and modified zwTZOPUPDD_MainRtnFromSubWnd, zwTZOPUPDD_SortByName,
   zwTZOPUPDD_MoveOperationPostBuil for check checkout state and set Title
1999.11.05  BL  Z10  new menu structure
   Modified Operation zwTZOPUPDD_DeleteOperation and added Operation
   fnzwTZOPUPDD_DeleteOperation for save as
1999.11.04  BL  Z10  new menu structure
   Added Operation zwTZOPUPDD_SaveAsPostBuild, zwTZOPUPDD_RefreshGroupList,
   zwTZOPUPDD_SetSaveAsName, zwTZOPUPDD_SetGroupName, zwTZOPUPDD_SaveAsInNewGroup,
   zwTZOPUPDD_ResetProfileXFER and zwTZOPUPDD_SaveAsOperation for save as
   Added Operation zwTZOPUPDD_SaveOperFromSource for move Operation and
   save Operation as
   Added Operation zwTZOPUPDD_MoveOperationPostBuil, zwTZOPUPDD_MoveOperToNewGroup
   for move Operation
   Modified Operation zwTZOPUPDD_NewOperationGroup, zwTZOPUPDD_GrayGroupControls
   for move Operation and Save as
1999.11.03  BL  Z10  new menu structure
   Added Operation zwTZOPUPDD_Set_CM_List, zwTZOPUPDD_MoveOperationPreBuild
   for move Operation
   Modified  Operation zwTZOPUPDD_ExitDialog: if Cancel by Prompt for save,
   then no exit Dialog
   Modified Operation zwfnPromptForSave and zwTZOPUPDD_PromptForSave for
   Operation List
   Added Operation zwTZOPUPDD_LoadOperation and zwTZOPUPDD_RefreshOperationList:
   if click in Operation List, then load this Operation in main window and
   refresh all windows
   Modofied Operation zwTZOPUPDD_SaveOperationGroup for check Executable Name
   Modified Operation zwTZOPUPDD_InitGroupDetail for Global Operation Group
   Detail
   Modified Operation zwTZOPUPDD_CreateParameter for set cursor
   Modified Operation zwTZOPUPDD_MainRtnFromSubWnd and zwTZOPUPDD_DeleteOperGroup
   for refresh all Subwindows (=Modeless Windows)
   Modified Operation zwTZOPUPDD_ActivateGroup, show Group Detail for current
   Group
from here old version
25.07.1997  DonC
   Created NewOperPrebuild to blank out Name and Desc fields in Profile
   OI so old values wouldn't appear in the window.
16.02.1998  DonC
   Modified zwTZOPUPDD_CreateOperation to initialize a new Operation to
   Type "G" (Global Operation). (QS 53)
02.06.1998  DonC
   Modified DeleteOperation to execute MergeGlobalPrototypes so that
   zeidonop.h is rebuilt. (QS 202)
18.08.1998  DonC
   Modified call to InitializeDefaultLPL to check for error return code.
1998/09/09  DC
   Modified zwTZopUPDD_DeleteOperation so that deleted Operation "stays deleted"
   under all circumstances. (TB 251)
1998.11.02 TMV
   Changing functioncalls of TransferToEditor because of a new argument
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

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

#include "ZeidonOp.H"

// LAST MESSAGE ID: OP00306

#define zOP_NewOperation              "N"
#define zOP_MoveOperation             "M"
#define zOP_MoveOperationInNewGroup   "X"
#define zOP_SaveAsOperationInNewGroup "S"
#define zOP_SaveAsOperation           "A"
#define zOP_ShowGroupDetail           "D"
#define zOP_UpdateGroupDetail         "U"
#define zOP_LoadGroupProperties       "P"
#define zOP_ExitNewOperation          "E"


static zSHORT
zwTZOPUPDD_CheckActionAfterSaveAs( zVIEW  vSubtask );

zSHORT /* LOCAL */  OPERATION
zwTZOPUPDD_CreateAllParameter( zVIEW vSubtask,
                               zVIEW vSource,
                               zVIEW vTarget );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MoveOperToNewGroup( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveAsNewOperation( zVIEW vSubtask,
                               zPCHAR szGroupName,
                               zPCHAR szOperationName,
                               zSHORT nCopyCode );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveAsOperation( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_GroupCheckedOut( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_EnableAndSetCheckOut( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_PromptForSave( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SwitchLPLR( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW   vSubtask,
                           zPCHAR  szToolname,
                           zPCHAR  szOperationname,
                           zVIEW   vToolView,
                           zPCHAR  szEntityname,
                           zLONG   lType );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZOPUPDD_AcceptSubobject( zVIEW vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szAttributeName,
                            zPCHAR szMessageText );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_CreateWorkLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_MoveOperationCode( zVIEW  vSubtask,
                            zSHORT nCopyOrMoveFlag );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_DropOperationFromSource( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_CopyOperationCode( zVIEW vSubtask,
                            zSHORT nRenameOperation );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZOPUPDD_DeleteOperFromFile( zVIEW  vSubtask,
                               zVIEW  vTZOGSRCO,
                               zSHORT nMoveOrDeleteFlag );

static zSHORT
zwfnTZOPUPDD_InitMenuDisableAll( zVIEW vSubtask );

static zSHORT
zwfnTZOPUPDD_LoadNewOperation( zVIEW   vSubtask,
                               zVIEW   vMainWindow,
                               zPCHAR  szOldOperation,
                               zPCHAR  szNewOperation );


zSHORT
zwfnSaveOperation( zVIEW vSubtask,
                   zBOOL nSaveAs )
{
   zVIEW    vOperGrp;
   zVIEW    vOperGrpTmp;
   zVIEW    vCM_List;
   zVIEW    vWindow;
   zCHAR    szLanguageType[ 4 ];
   zCHAR    szMetaName[ 33 ];
   zSHORT   nRC;

   // Don't continue if the Global Oper Group cannot be updated.
   if ( GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      return( 0 );
   }

   // We will remap everything here to make sure we have all the
   // data from the window.
   if ( MapWindow( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // if Domain not checked out and user would save it, then call
   // window save Domain as
   if ( nSaveAs )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZOPUPDD", "SaveAs" );
      return( 99 );
   }

   // Make sure that the Subdirectory attribute is null and that the
   // Extension attribute is set according to the Language type.
   SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Subdirectory", "" );
   GetStringFromAttribute( szLanguageType, zsizeof( szLanguageType ),
                           vOperGrp, "GlobalOperationGroup", "LanguageType" );
   if ( szLanguageType[ 0 ] == 'V' )
      SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                              "VML" );
   else
   {
      if ( szLanguageType[ 0 ] == 'S' )
         SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                                 "Scala" );
      else
      {
      if ( szLanguageType[ 0 ] == 'J' )
         SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                                 "Java" );
      else
         SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                                 "C" );
      }
   }

   // Since CommitMetaOI modifies position, create a temporary
   // view to be used by that routine.
   CreateViewFromViewForTask( &vOperGrpTmp, vOperGrp, 0 );
   nRC = CommitMetaOI( vSubtask, vOperGrpTmp, zSOURCE_GOPGRP_META );
   DropView( vOperGrpTmp );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "OP00305", "Operation Maintenance",
                   "Unable to save Operation Group",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Make sure operation list is sorted alphabetically.
   GetViewByName( &vCM_List, "TaskLPLR", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaType", "Type",
                                  zREFER_GO_META, 0 );
   OrderEntityForView( vCM_List, "W_MetaDef", "Name A" );

   // Now merge operation prototypes into the LPLR header file.
   GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vOperGrp, "GlobalOperationGroup", "Name" );

   // Since MergeGlobalPrototypes also modifies position, create a
   // temporary view to be used by that routine.
   CreateViewFromViewForTask( &vOperGrpTmp, vOperGrp, 0 );
   MergeGlobalPrototypes( vOperGrpTmp, szMetaName, "GlobalOperationGroup",
                          vCM_List );
   DropView( vOperGrpTmp );

   // Make sure that the global executable is generated.
   oTZOGSRCO_GenerateXOG( vOperGrp );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERGRPLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwfnTZOPUPDD_PromptForSave
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zSHORT
zwfnPromptForSave( zVIEW vSubtask )
{
   zVIEW   vOperGrp;
   zSHORT  nRC;
   zBOOL   nSaveAs = FALSE;
   zCHAR   szControlText[ 61 ];
   zCHAR   szOperationName[ 33 ];
   zCHAR   szMessageText[ 254 ];

   nRC = 0;
   if ( GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // Our solution will be to map all the controls on the window TZOPUPDD
      // individually.  We will check OperationType to see if they
      // are null and will map them if they are not.
      MapCtrl( vSubtask, "OperName" );
      MapCtrl( vSubtask, "Desc" );
      MapCtrl( vSubtask, "ReturnDesc" );
      MapCtrl( vSubtask, "ReturnDataType" );
      MapCtrl( vSubtask, "Parameter" );
      GetCtrlText( vSubtask, "GLOBALCONTEXT", szControlText, 60 );
      if ( szControlText[ 0 ] )
         MapCtrl( vSubtask, "GLOBALCONTEXT" );

      if ( MiGetUpdateForView( vOperGrp ) == 1 && ObjectInstanceUpdatedFromFile( vOperGrp ) )
      {
         // if Operation Group not checked out, then ask for save as
         if ( !ComponentIsCheckedOut( vSubtask, vOperGrp, zSOURCE_GOPGRP_META ) )
         {
            nSaveAs = TRUE;
            GetStringFromAttribute( szOperationName, zsizeof( szOperationName ), vOperGrp, "Operation", "Name" );
            strcpy_s( szMessageText, zsizeof( szMessageText ), "Operation Group is not checked out," );
            strcat_s( szMessageText, zsizeof( szMessageText ), " but Operation '" );
            strcat_s( szMessageText, zsizeof( szMessageText ), szOperationName );
            strcat_s( szMessageText, zsizeof( szMessageText ), "' has been modified. \n\nWould you like" );
            strcat_s( szMessageText, zsizeof( szMessageText ), " to save it with differend name?" );
            nRC = MessagePrompt( vSubtask, "DM00115", "Operation Maintenance",
                                 szMessageText, zBEEP, zBUTTONS_YESNOCANCEL,
                                 zRESPONSE_YES, zICON_QUESTION );
         }
         else
            nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_GO,
                                zREFER_GO_META,
                                vOperGrp, "Operation", "Name", 0 );

         if ( nRC == zRESPONSE_YES )
            nRC = zwfnSaveOperation( vSubtask, nSaveAs );
      }
   }

   return( nRC );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_MainPostBuild
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MainPostBuild( zVIEW vSubtask )
{
   zVIEW  vCM_List = NULL;
   zVIEW  vCM_ListGroup = NULL;
   zVIEW  vOperGrp;
   zVIEW  vOperGrpDetail;
   zVIEW  vProfileXFER;
   zVIEW  vSaveAs;
   zSHORT nRC;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "OP", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
   {
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag", "" );
   }

   if ( InitializeDefaultLPL( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_GO_META );
   if ( vCM_List )
      nRC = SetNameForView( vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );

   // Create the view for group detail window
   if ( GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      CreateViewFromViewForTask( &vOperGrpDetail, vOperGrp, 0 );
      SetNameForView( vOperGrpDetail, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   }

   //  Also get a CM_ListGroup View to the Operation Group.  Thus there will be two lists, one for the
   //  Global Operations themselves and one for the Global Operation Groups.
   nRC = RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_GOPGRP_META );
   if ( vCM_ListGroup )
      nRC = SetNameForView( vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Operation Maintenance", "TZOGSRCO",
                               vOperGrp, "Operation", zSOURCE_GOPGRP_META );

   // if Operation not checked out and user change this Operation, then zeidon
   // call the window "Save Domain as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Operation or Open an other Operation or Switch Project or
   // Exit Operation Tool)
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

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_MainRtnFromSubWnd
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//             the current EMD.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_ListGroup;
   zVIEW   vOperGrp;
   zVIEW   vOperGrpDetail;
   zSHORT  nRC;

   if ( GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK ) < 1 )
   {
      if ( RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_GO_META ) > 0 )
         nRC = SetNameForView( vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );

      if ( GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK ) < 0 )
      {
         if ( RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_GOPGRP_META ) > 0 )
            nRC = SetNameForView( vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
      }
   }

   // Create the view for group detail window
   if ( GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      CreateViewFromViewForTask( &vOperGrpDetail, vOperGrp, 0 );
      SetNameForView( vOperGrpDetail, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
      // if OperationGroup not checked out disable Combobox Operation Type
      if ( ComponentIsCheckedOut( vSubtask, vOperGrp, zSOURCE_GOPGRP_META ))
         SetCtrlState( vSubtask, "GLOBALCONTEXT", zCONTROL_STATUS_ENABLED, 1 );
      else
         SetCtrlState( vSubtask, "GLOBALCONTEXT", zCONTROL_STATUS_ENABLED, 0 );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Operation Maintenance", "TZOGSRCO",
                              vOperGrp, "Operation", zSOURCE_GOPGRP_META );

   //BL, 1999.11.03 Refresh all Lists from menu <View> (=Modeless Subwindows)
   RefreshAllSubwindows( vSubtask );

   return( 0 );
}

zSHORT
zwfnDropActiveOperation( zVIEW vSubtask )
{
#if 0
   zVIEW vSOURCEMETA;
   zVIEW vCMLIST;
   zVIEW vNEW;
   zVIEW vSRC;
   zCHAR szSrcZKey[ 12 ];
   zLONG lOperZKey;

   if ( GetViewByName( &vSOURCEMETA, "SOURCEMETA", vSubtask, zLEVEL_TASK ) < 1 )
      return( 0 );
   GetIntegerFromAttribute( &lOperZKey, vSOURCEMETA, "Operation", "ZKey" );
   GetStringFromAttribute( szSrcZKey, zsizeof( szSrcZKey ), vSOURCEMETA, "SourceFile", "ZKey" );

   if ( GetViewByName( &vNEW, "NEWOPER", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropMetaOI( vSubtask, vNEW );
      if ( GetViewByName( &vSRC, "SourceFile", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( SetCursorFirstEntityByInteger( vSRC, "GlobalOperation",
                                   "ZKey", lOperZKey, 0 ) >= zCURSOR_SET )
         {
            DropEntity( vSRC, "GlobalOperation", zREPOS_PREV );
         }
      }
      RetrieveViewForMetaList( vSubtask, &vCMLIST, zSOURCE_GO_META );
      if ( SetCursorFirstEntityByInteger( vCMLIST,
               "W_MetaDef", "CPLR_ZKey", lOperZKey, 0 ) >= zCURSOR_SET )
      {
         DeleteMetaOI( vSubtask, vCMLIST, zSOURCE_GO_META );
      }
      DropView( vCMLIST );
   }
   else
      DropMetaOI( vSubtask, vSOURCEMETA );

   zwfnDropNewSourceFile( vSubtask, "NEWSRC" );
#endif

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_ExitDialog
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//             the current EMD.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_ExitDialog( zVIEW vSubtask )
{
   zSHORT  nRC;
   zVIEW   vSaveAs;
   zVIEW   vOperationGrp;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   nRC = zwTZOPUPDD_PromptForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( vSaveAs )
      {
         if ( nRC == 99 )
            // if Operation not checked out and user change this Operation,
            // then zeidon call the window "Save Operation as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Operation or Open an other Operation or Switch Project
            // or Exit Operation Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zExitToolAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   if ( vSaveAs )
      DropMetaOI( vSubtask, vSaveAs );

   if ( GetViewByName( &vOperationGrp, "TZOGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
      DropMetaOI( vSubtask, vOperationGrp );

   zwfnDropActiveOperation( vSubtask );
   TerminateLPLR( vSubtask );
   return( nRC );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_PromptForSave
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//             the current EMD.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_PromptForSave( zVIEW vSubtask )
{
   zSHORT  nRC;

   nRC = zwfnPromptForSave( vSubtask );
   if ( nRC < 0 || nRC == zRESPONSE_CANCEL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( nRC );
   }

   return( nRC );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_OpenOperation
// PURPOSE:    This operation is invoked when the user selects the OK
//             button on the operation OPEN window. The Global Operation
//             Group for the identified operation is Activated and
//             the view is positioned on the correct Operation within the Group.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_OpenOperation( zVIEW vSubtask )
{
   zVIEW vCM_List;
   zVIEW vOperGrp;
   zVIEW vOperGrp2;
   zVIEW vMainWindow;
   zCHAR szOperGroupName[ 33 ];
   zCHAR szOperName[ 33 ];
   zSHORT nRC;

   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) < zCURSOR_SET )
      return( 0 );

   // vCM_List is positioned on the Operation to be opened.  There are two diferent situations.
   // 1. The new Operation is in the current Global Operation Group.  In this case, just position
   //    on the correct Operation entity.
   // 2. The new Operation is not in the current Global Operation Group.  In this case, Activate
   //    a new Global Operation Group and position on the correct Operation entity.
   GetStringFromAttribute( szOperName, zsizeof( szOperName ), vCM_List, "W_MetaDef", "Name" );
   nRC = GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      nRC = SetCursorFirstEntityByString( vOperGrp, "Operation", "Name", szOperName, 0 );
   else
      vOperGrp = 0;  // Indicate no current Oper Group exists.

   if ( nRC >= 0 )
   {
      // The Operation is in the current Oper Group, and we just positioned on it.
      // Don't do anything here.
   }
   else
   {
      // Either there is no current Oper Group or it does not contain the new Operation
      GetStringFromAttribute( szOperGroupName, zsizeof( szOperGroupName ), vCM_List, "W_MetaDef", "GroupName" );

      if ( ActivateMetaOI_ByName( vSubtask, &vOperGrp2, 0, zSOURCE_GOPGRP_META,
                                  zSINGLE, szOperGroupName, 0 ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
      else
      {
         if ( vOperGrp != 0 )   // A current Oper Group exists.
            DropMetaOI( vSubtask, vOperGrp ); // Get rid of the last Oper Group OI
         // The Activate was successful.  Now position on the correct oper and name the view.
         SetCursorFirstEntityByString( vOperGrp2, "Operation", "Name", szOperName, 0 );
         SetNameForView( vOperGrp2, "TZOGSRCO", vSubtask, zLEVEL_TASK );

         GetSubtaskForWindowName( vSubtask, &vMainWindow, "TZOPUPDD" );
         // if OperationGroup not checked out, set  View read only ->
         // the user cannot update the values in Detail Windows
         if ( ComponentIsCheckedOut( vSubtask, vOperGrp2, zSOURCE_GOPGRP_META ))
            SetCtrlState( vMainWindow, "GLOBALCONTEXT", zCONTROL_STATUS_ENABLED, 1 );
         else
         {
            SetViewReadOnly( vOperGrp2 );
            SetCtrlState( vMainWindow, "GLOBALCONTEXT", zCONTROL_STATUS_ENABLED, 0 );
         }
      }
   }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_SaveOperation
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveOperation( zVIEW vSubtask )
{
   zwfnSaveOperation( vSubtask, FALSE );
   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_CreateOperation
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_CreateOperation( zVIEW vSubtask )
{

   zVIEW    vProfileXFER;
   zVIEW    vOperGrp;
   zVIEW    vCM_ListGroup;
   zVIEW    vCM_List;
   zCHAR    szName[ 33 ];
   zCHAR    szProfileGroupName[ 33 ];
   zSHORT   nZRetCode;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_ANY );

   nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vProfileXFER, "OP", "Name" );
   if ( szName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "OP00301", "Operation Maintenance",
                   "Operation name required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh,
                                 0, 0 );
      return( -1 );
   }

   // Make sure the operation to be created does not already exist
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szName, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "OP00302", "Operation Maintenance",
                   "A Operation already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   else
   {
      if ( GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_ANY ) > 0 )
         DropMetaOI( vSubtask, vOperGrp );

      // We first need to set up the proper Global Operation Group. There are two possible situations:
      //    1. We are positioned on the Operation Group selected.  Activate the Operation Group
      //       by that name.
      //    2. A new Operation Group is in the process of being created(ie., we just came from the
      //       NewOperGroup window.  Create a new Global Operation Group OI.
      // We determine which of the above two situations we are in by checking if the GroupName
      // attribute in the Profile object is null.  If it is null, we assume condition 1 above is
      // occurring..

      GetStringFromAttribute( szProfileGroupName, zsizeof( szProfileGroupName ), vProfileXFER, "OP", "GroupName" );
      if ( szProfileGroupName[ 0 ] == 0 )
      {
         // This is not a new Global Operation Group, Activate the one on which we have position.
         ActivateMetaOI( vSubtask, &vOperGrp, vCM_ListGroup, zSOURCE_GOPGRP_META, zSINGLE );
      }
      else
      {
         // This is a new Global Operation Group, create it.
         ActivateEmptyMetaOI( vSubtask, &vOperGrp, zSOURCE_GOPGRP_META,
                               zSINGLE | zLEVEL_APPLICATION );
         CreateMetaEntity( vSubtask, vOperGrp, "GlobalOperationGroup", zPOS_AFTER );
         SetAttributeFromAttribute( vOperGrp, "GlobalOperationGroup", "Name",
                                     vProfileXFER, "OP", "GroupName" );
         SetAttributeFromAttribute( vOperGrp, "GlobalOperationGroup", "Desc",
                                     vProfileXFER, "OP", "GroupDesc" );

         // Null out the vProfileXFER.GroupName attribute for the next occurrence of this code.
         SetAttributeFromString( vProfileXFER, "OP", "GroupName", "" );
      }

      SetNameForView( vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK );

      CreateMetaEntity( vSubtask, vOperGrp, "Operation", zPOS_AFTER );
      SetAttributeFromAttribute( vOperGrp,      "Operation", "Name",
                                 vProfileXFER,  "OP",        "Name" );
      SetAttributeFromAttribute( vOperGrp,      "Operation", "Desc",
                                 vProfileXFER,  "OP",        "Desc" );
      SetAttributeFromString( vOperGrp, "Operation",  "Type", "G" );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: fnzwTZOPUPDD_DropOperFromFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnzwTZOPUPDD_DropOperFromFile( zVIEW vSubtask,
                               zVIEW vOperGrp )
{
   zVIEW  vDeleteOp;

   // Create LOD for move Operation Code from old Source Files
   if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vDeleteOp );

   ActivateEmptyObjectInstance( &vDeleteOp, "TZEDWDEL", vSubtask, zMULTIPLE );

   // Create Source File
   CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
   SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                vOperGrp, "GlobalOperationGroup", zSET_ALL );
   SetNameForView( vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );

   // Create Operation
   CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
   SetAttributeFromAttribute( vDeleteOp, "Operation", "Name",
                              vOperGrp, "Operation", "Name");

   // Delete Operation from Source File
   zwTZOPUPDD_DeleteOperFromFile( vSubtask, vOperGrp, zDELETE_OPERATION );

   return( 0 );
} // fnzwTZOPUPDD_DropOperFromFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnzwTZOPUPDD_PromptForDelete
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
fnzwTZOPUPDD_PromptForDelete( zVIEW  vSubtask,
                              zVIEW  vOperGrp,
                              zSHORT nPromptForDelete,
                              zSHORT nDeleteOperationCode )
{
   zSHORT  nRC = zRESPONSE_NO;

   // If save as, no Prompt for delete
   if ( nPromptForDelete == 1 )
   {
      // Prompt for Delete
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_GO,
                          zREFER_GO_META, vOperGrp, "Operation", "Name", 0 );
      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      // OK to delete Global Operation from Source File
      nRC = MessagePrompt( vSubtask, "OP00810", "Operation Maintenance",
                           "Delete Operation from Source File?", 0,
                           zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
   }

   if ( nDeleteOperationCode == 1 &&
        ( nPromptForDelete == 0 || nRC == zRESPONSE_YES ) )
   {
      fnzwTZOPUPDD_DropOperFromFile( vSubtask, vOperGrp );
   }

   return( 0 );
} // fnzwTZOPUPDD_PromptForDelete

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnzwTZOPUPDD_DeleteFromXLP
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnzwTZOPUPDD_DeleteFromXLP( zVIEW  vCM_List,
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
                                  zSOURCE_GO_META, 0 );
   if ( SetCursorFirstEntityByInteger( vCM_List_Copy, "W_MetaDef", "CPLR_ZKey",
                                       ulZKey, 0 ) >= 0 )
   {
      DeleteEntity( vCM_List_Copy, "W_MetaDef", zREPOS_PREV );
   }

   DropView( vCM_List_Copy );

   SetCursorPrevEntity( vCM_List, "W_MetaDef", "" );

   return( 0 );
} // fnzwTZOPUPDD_DeleteFromXLP

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnzwTZOPUPDD_DeleteOperation
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
fnzwTZOPUPDD_DeleteOperation( zVIEW  vSubtask,
                              zSHORT nPromptForDelete,
                              zSHORT nDeleteOperationCode )
{
   zVIEW    vCM_List;
   zVIEW    vCM_ListOrig;
   zVIEW    vOperGrp;
   zVIEW    vOperGrpCur;
   zVIEW    vOperGrpOrig;
   zVIEW    vOperGrpTmp;
   zVIEW    vWindow;
   zBOOL    bRefreshMainWindow = FALSE;
   zSHORT   nRC;
   zULONG   ulZKey;
   zCHAR    szGroupName[ 33 ];

   GetViewByName( &vCM_ListOrig, "CMLIST", vSubtask, zLEVEL_ANY );
   // Get the ZKey for later delete of W_MetaDef entries.
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vCM_ListOrig,
                            "W_MetaDef", "CPLR_ZKey" );

   // Create a new view, since the ActivateMetaOI will alter position on the
   // W_MetaType
   CreateViewFromViewForTask( &vCM_List, vCM_ListOrig, 0 );

   nRC = ActivateMetaOI( vSubtask, &vOperGrp, vCM_List, zSOURCE_GO_META,
                         zSINGLE | zLEVEL_APPLICATION );
   GetStringFromAttribute( szGroupName, zsizeof( szGroupName ), vCM_ListOrig, "W_MetaDef", "GroupName" );
   DropView( vCM_List );
   vCM_List = 0;

   if ( nRC <= 0 )
   {
      // If the meta was not found, we nevertheless want to delete it from XLP.
      fnzwTZOPUPDD_DeleteFromXLP( vCM_ListOrig, ulZKey );
      return( 0 );
   }

   // Prompt for delete
   if ( fnzwTZOPUPDD_PromptForDelete( vSubtask, vOperGrp, nPromptForDelete,
                                      nDeleteOperationCode ) < 0 )
      return( 0 );

   // If the OperationGroup just activated is the same as the one currently
   // in memory, use the one in memory.
   vOperGrpOrig = 0;
   if ( GetViewByName( &vOperGrpCur, "TZOGSRCO", vSubtask, zLEVEL_ANY ) > 0 &&
        CompareAttributeToAttribute( vOperGrpCur, "Operation", "ZKey",
                                     vOperGrp,    "Operation", "ZKey" ) == 0 )
   {
         vOperGrpOrig = vOperGrp;
         vOperGrp     = vOperGrpCur;
         bRefreshMainWindow = TRUE;
         GetSubtaskForWindowName( vSubtask, &vWindow, "TZOPUPDD" );
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vOperGrp, "Operation", "ZKey" );
   DeleteEntity( vOperGrp, "Operation", zREPOS_PREV );
   CommitMetaOI( vSubtask, vOperGrp, zSOURCE_GOPGRP_META );

   if ( vOperGrpOrig != 0 )
      DropObjectInstance( vOperGrpOrig );

   // Since MergeGlobalPrototypes also modifies position, create a
   // temporary view to be used by that routine.
   CreateViewFromViewForTask( &vOperGrpTmp, vOperGrp, 0 );
   MergeGlobalPrototypes( vOperGrpTmp, szGroupName, "GlobalOperationGroup",
                          vCM_List );
   DropView( vOperGrpTmp );
   DropObjectInstance( vOperGrp );

   if ( bRefreshMainWindow )
   {
      RefreshWindow( vWindow );
      RefreshAllSubwindows( vWindow );
   }

   fnzwTZOPUPDD_DeleteFromXLP( vCM_ListOrig, ulZKey );

   return( 0 );
} // fnzwTZOPUPDD_DeleteOperation

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_DeleteOperation
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_DeleteOperation( zVIEW vSubtask )
{
   fnzwTZOPUPDD_DeleteOperation( vSubtask, 1, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_ParameterPopup
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_ParameterPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vTZOGSRCO;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   if ( GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZOGSRCO, zSOURCE_GOPGRP_META );

      if ( CheckExistenceOfEntity( vTZOGSRCO, "Parameter" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "ParmAdd", bNew );
      EnableAction( vSubtask, "ParmDelete", bDelete );

      CreateTrackingPopupMenu( vSubtask, "ParameterPopup",
                               pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_TransferToEditor
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_TransferToEditor( zVIEW vSubtask )
{
   zVIEW  vOper;
   zVIEW  vMeta;
   zVIEW  vT;
   zSHORT nRC;

   if ( GetViewByName( &vOper, "TZOGSRCO", vSubtask, zLEVEL_TASK ) < 1 )
      return( 0 );

   // *** TEMPORARY HACK ***
   // This should be replaced when/if VML is re-written.
   //
   // Copy the SourceFile subobject information to a new work object of type
   // TZZOLODO.  This is necessary because the Parser requires an object with
   // a subobject starting with entity named "SourceFile".  It is easier to
   // set the source information as a LOD than it is to modify the Parser to
   // handle the different Global Operation structure.
   //
   CreateViewFromViewForTask( &vT, vOper, 0 );
   ActivateEmptyObjectInstance( &vMeta, "TZZOLODO", vSubtask, zSINGLE );
   SetNameForView( vMeta, "GOP_SourceFile", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vMeta, "LOD", zPOS_LAST );
   SetAttributeFromAttribute( vMeta, "LOD", "Name",
                              vT, "GlobalOperationGroup", "Name" );
   CreateMetaEntity( vSubtask, vMeta, "SourceFile", zPOS_LAST );
   SetMatchingAttributesByName( vMeta, "SourceFile",
                                vT, "GlobalOperationGroup", zSET_NULL );
   nRC = SetCursorFirstEntity( vT, "Operation", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vMeta, "Operation", zPOS_LAST );
      SetMatchingAttributesByName( vMeta, "Operation",
                                   vT, "Operation", zSET_NULL );
      nRC = SetCursorFirstEntity( vT, "Parameter", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vMeta, "Parameter", zPOS_LAST );
         SetMatchingAttributesByName( vMeta, "Parameter",
                                      vT, "Parameter", zSET_NULL );
         nRC = SetCursorNextEntity( vT, "Parameter", 0 );
      }

      nRC = SetCursorNextEntity( vT, "Operation", 0 );
   }

   DropView( vT );
   // *** END OF TEMPORARY HACK ***

   TransferToEditor( vSubtask, vOper, "GO", 0 );
   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION   zwTZOPUPDD_HelpAbout
// PURPOSE:    This Entry does the About
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_GO, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOPUPDD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_DeleteOperGroup( zVIEW vSubtask )
{
   zVIEW    vOperGrp;
   zVIEW    vActiveOperGrp;
   zVIEW    vCM_ListGroup;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zLONG    lStatus;
   zSHORT   nRC;

   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   nRC = ActivateMetaOI( vSubtask, &vOperGrp, vCM_ListGroup, zSOURCE_GOPGRP_META, zSINGLE );

   nRC = GetIntegerFromAttribute( &lStatus, vCM_ListGroup, "W_MetaDef", "Status" );

   if ( lStatus <= 0 )
      return( 0 );

   // if the status of the operation group is active, allow possible delete
   if ( lStatus )
   {
   // Prompt for Delete
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_GO,
                          zREFER_GOPGRP_META,
                          vOperGrp, "GlobalOperationGroup", "Name", 0 );
      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
   }

   // If the Operation Group currently active, "TZOGSRCO", is the one being
   // deleted, delete that member from memory.
   if ( GetViewByName( &vActiveOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CompareAttributeToAttribute( vActiveOperGrp, "GlobalOperationGroup", "Name",
                                        vOperGrp, "GlobalOperationGroup", "Name" ) == 0 )
      {
         DropMetaOI( vSubtask, vActiveOperGrp );
         //BL, 1999.11.03 if delete current Domain Group, then refresh Parentwindow
         //and set Focus to window Domain Group List again
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
         SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow,
                                  "TZOPUPDD", "OPERGRPLIST" );
      }
   }

   DropMetaOI( vSubtask, vOperGrp );
   DeleteMetaOI( vSubtask, vCM_ListGroup, zSOURCE_GOPGRP_META );

   //BL, 1999.11.03
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLISTunderGRP" ) >= 0 )
      RefreshWindow( vWindow );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZOPUPDD" ) >= 0 )
      RefreshWindow( vWindow );

   RefreshWindow( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_NewOperationGroup( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vCM_List;
   zVIEW    vOperGrp;
   zLONG    lRC;
   zCHAR    szOperName[ 33 ];

   // This Operation creates a new Operation Group from the new Operation window,
   // prior to transferring to the Operation Group Update window.

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szOperName, zsizeof( szOperName ), vProfileXFER, "OP", "Name" );

   // Check that the Operation name was entered.
   // (This is same message as in CreateOperation.)
   if ( szOperName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "OP00301", "Operation Maintenance",
                   "Operation name required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "Operation");
      return( -1 );
   }

   //BL, 1999.11.04 no check by move Operation in new Operation Group
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_MoveOperation ) != 0 )
   {
      // Check that the Operation Name is unique.
      // (This is same message as in CreateOperation.)
      lRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                          szOperName, "" );
      if ( lRC >= zCURSOR_SET )
      {
         MessageSend( vSubtask, "OP00302", "Operation Maintenance",
                      "A Operation already exists by that name.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
         SetFocusToCtrl( vSubtask, "Operation");
         return( -1 );
      }
   }

   ActivateEmptyMetaOI( vSubtask, &vOperGrp, zSOURCE_GOPGRP_META, zSINGLE );
   CreateMetaEntity( vSubtask, vOperGrp, "GlobalOperationGroup", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vOperGrp, "Operation", zPOS_AFTER );
   SetNameForView( vOperGrp, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_MoveOperation ) == 0 )
   {
      //Flag, move Operation in new Operation Group
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_MoveOperationInNewGroup );
   }
   else
   {
      //Flag, create new Operation in new Operation Group
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_NewOperation );
      SetAttributeFromAttribute( vOperGrp, "Operation", "Name",
                                 vProfileXFER, "OP", "Name" );
      SetAttributeFromAttribute( vOperGrp, "Operation", "Desc",
                                 vProfileXFER, "OP", "Desc" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_CreateParameter( zVIEW vSubtask )
{
   zVIEW    vOperGrp;
   zVIEW    vWindow;
   zVIEW    vParentWindow;

   // Use this operation instead of auto object functionality because of ZKey.
   GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_ANY );

   // If we are positioned on a Parameter, accept it, since it may be a temporal
   // subobject.
   if ( CheckExistenceOfEntity( vOperGrp, "Parameter" ) >= zCURSOR_SET )
   {
      if ( zwTZOPUPDD_AcceptSubobject( vSubtask, "Parameter",
                                       "ShortDesc", "Parameter" ) < 0 )
      {
         return( -1 );
      }

      SetSelectStateOfEntity( vOperGrp, "Parameter", 0 );
   }

   CreateTemporalMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetSelectStateOfEntity( vOperGrp, "Parameter", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "PARAMETEREDIT" ) >= 0 )
   {
      GetParentWindow ( &vParentWindow, vWindow );
      RefreshWindow( vParentWindow );
      SetFocusToCtrl( vSubtask, "edDescription" );
   }
   else
   {
      RefreshWindow( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_OperGroupPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   // Pop up the OperGroup Popup window.
   CreateTrackingPopupMenu( vSubtask, "OperGroupPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_CheckGroupName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZOPUPDD_CheckGroupName( zVIEW  vSubtask,
                           zPCHAR szOperGrpName,
                           zVIEW  vOperGrp,
                           zVIEW  vCM_ListGroup )
{
   zCHAR    szExecName[ 33 ];
   zSHORT   nRC;

   if ( szOperGrpName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "OP00303", "Operation Maintenance",
                   "Operation Group Name required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "OperGroupName" );
      return( -1 );
   }

   // Check that the Operation Name is unique.
   // (This is same message as in CreateOperation.)
   nRC = SetCursorFirstEntityByString( vCM_ListGroup, "W_MetaDef", "Name",
                                       szOperGrpName, "" );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "OP00304", "Operation Maintenance",
                   "A Operation Group already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "OperGroupName" );
      return( -1 );
   }

   //BL, 1999.11.03  check Executable Name
   GetStringFromAttribute( szExecName, zsizeof( szExecName ), vOperGrp, "GlobalOperationGroup",
                           "DomainAndGlobalOpGroupName" );
   if ( szExecName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "OP00303", "Operation Maintenance",
                   "Executable Name required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "edExecName" );
      return( -1 );
   }

   return( 0 );
} // zwTZOPUPDD_CheckGroupName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SetExtension
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_SetExtension( zVIEW vSubtask )
{
   zVIEW  vOperGrp;
   zCHAR  szLanguageType[ 4 ];

   GetViewByName( &vOperGrp, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szLanguageType, zsizeof( szLanguageType ), vOperGrp, "GlobalOperationGroup",
                           "LanguageType" );

   if ( szLanguageType[ 0 ] == 0 )  // '\0f' ???
   {
      MessageSend( vSubtask, "OP00806", "Operation Maintenance",
                   "A Language Type was not selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Make sure that the Subdirectory attribute is null and that the
   // Extension attribute is set according to the Language type.
   if ( szLanguageType[ 0 ] == 'V' )
      SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                              "VML" );
   else
   {
      if ( szLanguageType[ 0 ] == 'S' )
         SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                                 "Scala" );
      else
      {
      if ( szLanguageType[ 0 ] == 'J' )
         SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                                 "Java" );
      else
         SetAttributeFromString( vOperGrp, "GlobalOperationGroup", "Extension",
                                 "C" );
      }
   }
   return( 0 );
} // zwTZOPUPDD_SetExtension

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveOperationGroup( zVIEW vSubtask )
{
   zVIEW    vOperGrp;
   zVIEW    vProfileXFER;
   zVIEW    vCM_ListGroup;
   zVIEW    vParentWindow;
   zCHAR    szOperGrpName[ 33 ];
   zCHAR    szControltext[ 4 ];

   // Commit the Operation Group to a file.  This would be done whether the
   // group was being processed as new or for update.
   GetViewByName( &vOperGrp, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_ANY );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szOperGrpName, zsizeof( szOperGrpName ), vOperGrp, "GlobalOperationGroup", "Name" );

   if ( !ComponentIsCheckedOut( vSubtask, vOperGrp, zSOURCE_GOPGRP_META ))
      return( 0 );

   // Validate that a valid name has been entered (meaning it is not blank and not
   // an existing Operation Group name.  Do these checks only if this is a new Operation
   // Group function.
   if ( (CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                   zOP_NewOperation ) == 0) ||
        (CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                   zOP_MoveOperationInNewGroup ) == 0) ||
        (CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                   zOP_SaveAsOperationInNewGroup ) == 0) )
   {
      if ( zwTZOPUPDD_CheckGroupName( vSubtask, szOperGrpName, vOperGrp,
                                      vCM_ListGroup ) < 0 )
      {
         return( -1 );
      }
   }

   if ( zwTZOPUPDD_SetExtension( vSubtask ) < 0 )
      return( -1 );

   // If this is part of a new Operation Group function, set the view name
   // TZOGSRCO to this view so the correct operation group will appear upon
   // return to the main window.
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_MoveOperationInNewGroup ) == 0 )
   {
      zwTZOPUPDD_MoveOperToNewGroup( vSubtask );
   }
   else
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_NewOperation ) == 0 )
   {
      SetNameForView( vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK );
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_ExitNewOperation );
   }
   else
   // If this is part of an Update Operation Group function, commit the Operation Group.
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_UpdateGroupDetail ) == 0 )
   {
      CommitMetaOI( vSubtask, vOperGrp, zSOURCE_GOPGRP_META );
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag", "" );
   }
   else
   //if Save As?
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_SaveAsOperationInNewGroup ) == 0 )
   {
      GetParentWindow( &vParentWindow, vSubtask );
      SetCtrlText( vParentWindow, "txtOperationGroup", szOperGrpName );
      RefreshCtrl( vParentWindow, "txtOperationGroup" );
      GetStringFromAttributeByContext( szControltext, vOperGrp, "GlobalOperationGroup",
                                       "LanguageType", "LanguageType", 4 );
      SetCtrlText( vParentWindow, "txtType", szControltext );
      RefreshCtrl( vParentWindow, "txtType" );
      zwTZOPUPDD_SaveAsOperation( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_GrayGroupControls( zVIEW vSubtask )
{
   zVIEW   vCM_ListGroup;
   zVIEW   vProfileXFER;
   zVIEW   vTZOGSRCO_DETAIL;
   zSHORT  nEnable;
   zSHORT  nRC;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   // Gray the OperationGroup Name control, if this is not a new Operation Group
   // function.
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                   zOP_NewOperation ) == 0 )
   {
      SetCtrlState( vSubtask, "OperGroupName", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlText( vSubtask, "OK", "Create" );
   }
   else
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_MoveOperationInNewGroup ) == 0 )
   {
      SetCtrlState( vSubtask, "OperGroupName", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlText( vSubtask, "OK", "Move" );
   }
   else
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_SaveAsOperationInNewGroup ) == 0 )
   {
      SetCtrlState( vSubtask, "OperGroupName", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlText( vSubtask, "OK", "Save as" );
   }
   else
   {
      SetCtrlState( vSubtask, "OperGroupName", zCONTROL_STATUS_ENABLED, FALSE );

      // set check out state in Title
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );

      if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                     zOP_LoadGroupProperties ) == 0 )
      {
         if ( GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK ) > 0 )
         {
            nRC = SetCursorFirstEntityByAttr( vCM_ListGroup, "W_MetaDef", "Name",
                                              vTZOGSRCO_DETAIL, "GlobalOperationGroup",
                                              "Name", "" );
            if ( nRC < zCURSOR_SET )
            {
               nEnable = 1;
               SetWindowCaptionTitle( vSubtask, "", "Operation Group Properties" );
            }
         }
      } //endif ( CompareAttributeToString( vProfileXFER, "OP"...

      SetCtrlState( vSubtask, "edDescription", zCONTROL_STATUS_ENABLED, nEnable );
      SetCtrlState( vSubtask, "edExecName", zCONTROL_STATUS_ENABLED, nEnable );
      SetCtrlState( vSubtask, "rbVML", zCONTROL_STATUS_ENABLED, nEnable );
      SetCtrlState( vSubtask, "rbC", zCONTROL_STATUS_ENABLED, nEnable );
      if ( nEnable == 1 )
          SetFocusToCtrl( vSubtask, "edDescription" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_InitGroupDetail( zVIEW vSubtask )
{
   // Set the flag that indicates whether or not this is a
   // new Operation Group or an update.
   zVIEW   vProfileXFER;
   zVIEW   vTZOGSRCO;
   zVIEW   vTZOGSRCO_DETAIL;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                           zOP_LoadGroupProperties );

   if ( GetViewByName( &vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( vTZOGSRCO_DETAIL );
   }

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZOGSRCO_DETAIL, vTZOGSRCO, 0 );
   SetNameForView( vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_OpenOperPreBuild( zVIEW vSubtask )
{
   // For each operation, set the dates from the corresponding group.
   // Then order the Operations first by date and then by name.

   zVIEW  vOperLPLR;
   zVIEW  vGroupLPLR;
   zVIEW  vTZOGSRCO;
   zSHORT nRC;
   zCHAR  szGroupName[ 33 ];

   GetViewByName( &vOperLPLR, "CMLIST", vSubtask, zLEVEL_ANY );
   RetrieveViewForMetaList( vSubtask, &vGroupLPLR, zREFER_GOPGRP_META );

   nRC = SetCursorFirstEntity( vOperLPLR, "W_MetaDef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szGroupName, zsizeof( szGroupName ),
                              vOperLPLR, "W_MetaDef", "GroupName" );
      SetCursorFirstEntityByString( vGroupLPLR, "W_MetaDef", "Name",
                                    szGroupName, 0 );
      SetAttributeFromAttribute( vOperLPLR,  "W_MetaDef", "LastUpdateDate",
                                 vGroupLPLR, "W_MetaDef", "LastUpdateDate" );
      nRC = SetCursorNextEntity( vOperLPLR, "W_MetaDef", 0 );
   }

   if ( GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZOGSRCO, "Operation" ) >= zCURSOR_SET )
   {
      // if Operation open, then set cursor of it
      SetCursorFirstEntityByAttr( vOperLPLR, "W_MetaDef", "Name",
                                  vTZOGSRCO, "Operation", "Name", "" );
   }
   else
      SetCursorFirstEntity( vOperLPLR, "W_MetaDef", 0 );

   DropView( vGroupLPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SortByName( zVIEW  vSubtask )
{
   zVIEW  vOperLPLR;

   // Order the Operations by name only.
   GetViewByName( &vOperLPLR, "CMLIST", vSubtask, zLEVEL_ANY );
   OrderEntityForView( vOperLPLR, "W_MetaDef", "Name A" );
   SetCursorFirstEntity( vOperLPLR, "W_MetaDef", 0 );

   zwTZOPUPDD_GroupCheckedOut( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SortByDate( zVIEW    vSubtask )
{
   // Order the Operations by date and then by name within date..

   zVIEW  vOperLPLR;

   GetViewByName( &vOperLPLR, "CMLIST", vSubtask, zLEVEL_ANY );
   OrderEntityForView( vOperLPLR, "W_MetaDef", "LastUpdateDate D Name A" );
   SetCursorFirstEntity( vOperLPLR, "W_MetaDef", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SortByGroup( zVIEW    vSubtask )
{
   // Order the Operations by date and then by name within date..

   zVIEW  vOperLPLR;

   GetViewByName( &vOperLPLR, "CMLIST", vSubtask, zLEVEL_ANY );
   OrderEntityForView( vOperLPLR, "W_MetaDef", "GroupName A Name A" );
   SetCursorFirstEntity( vOperLPLR, "W_MetaDef", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_NewOperPrebuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   // Blank out the Name and Desc values in the Profile object so that
   // previous names won't be shown.
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vProfileXFER, "OP" ) >= zCURSOR_SET )
   {
      SetAttributeFromString( vProfileXFER, "OP", "Name", "" );
      SetAttributeFromString( vProfileXFER, "OP", "Desc", "" );
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_NewOperation );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_RefreshOperationList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_RefreshOperationList( zVIEW vSubtask )
{
   zVIEW vParentWindow;

   GetParentWindow( &vParentWindow, vSubtask );
   MapCtrl( vParentWindow, "OperName" );
   MapCtrl( vParentWindow, "Desc" );

   return( 0 );
} // zwTZOPUPDD_RefreshOperationList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZOPUPDD_LoadNewOperation
//
// by click load Operation in main window
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
zwfnTZOPUPDD_LoadNewOperation( zVIEW   vSubtask,
                               zVIEW   vMainWindow,
                               zPCHAR  szOldOperation,
                               zPCHAR  szNewOperation )
{
   zVIEW  vCM_List;
   zVIEW  vTZOGSRCO;
   zVIEW  vTZOGSRCO1;
   zVIEW  vOperationGrpDetail;

   GetViewByName( &vCM_List, "CMLIST", vMainWindow, zLEVEL_ANY );

   // Use a second CM_List view for Activate since Activate will alter
   // its position.
   SetCursorFirstEntityByString ( vCM_List, "W_MetaDef",
                                  "Name", szOldOperation, "" );

   if ( ActivateMetaOI( vSubtask, &vTZOGSRCO1, vCM_List, zSOURCE_GO_META,
                        zSINGLE | zLEVEL_APPLICATION ) >= 0 )
   {
      if ( GetViewByName( &vTZOGSRCO, "TZOGSRCO", vMainWindow, zLEVEL_ANY ) > 0 )
         DropMetaOI( vSubtask, vTZOGSRCO );

      SetNameForView( vTZOGSRCO1, "TZOGSRCO", vMainWindow, zLEVEL_TASK );

      // if OperationGroup not checked out, set  View read only ->
      // the user cannot update the values in Detail Windows
      if ( ComponentIsCheckedOut( vSubtask, vTZOGSRCO1, zSOURCE_GOPGRP_META ))
         SetCtrlState( vMainWindow, "GLOBALCONTEXT", zCONTROL_STATUS_ENABLED, 1 );
      else
      {
         SetViewReadOnly( vTZOGSRCO1 );
         SetCtrlState( vMainWindow, "GLOBALCONTEXT", zCONTROL_STATUS_ENABLED, 0 );
      }

      // create the view for group detail window
      CreateViewFromViewForTask( &vOperationGrpDetail, vTZOGSRCO1, 0 );
      SetNameForView( vOperationGrpDetail, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

      SetCursorFirstEntityByString( vTZOGSRCO1, "Operation", "Name",
                                    szNewOperation, "" );
      SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                    szNewOperation, "" );

      // Set window Title with check out state
      SetTitleWithCheckOutState( vMainWindow, "Operation Maintenance",
                                 "TZOGSRCO", vTZOGSRCO1, "Operation",
                                 zSOURCE_GOPGRP_META );
   }

   return( 0 );
} // zwfnTZOPUPDD_LoadNewOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_LoadOperation
//
// by click load Operation in main window
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_LoadOperation( zVIEW vSubtask )
{
   zVIEW  vMainWindow;
   zVIEW  vTZOGSRCO;
   zVIEW  vSaveAs;
   zSHORT nRC;
   zLONG  lActionAfterSaveAs;
   zCHAR  szNewOperation[ 33 ];
   zCHAR  szOldOperation[ 33 ];

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );

   // save new selected Operation
   GetStringFromAttribute( szNewOperation, zsizeof( szNewOperation ), vTZOGSRCO, "Operation", "Name" );

   GetSubtaskForWindowName( vSubtask, &vMainWindow, "TZOPUPDD" );
   GetCtrlText( vMainWindow, "OperName", szOldOperation, 33 );

   if ( zstrcmp( szNewOperation, szOldOperation ) != 0 )
   {
      SetCursorFirstEntityByString( vTZOGSRCO, "Operation",
                                    "Name", szOldOperation, "" );

      if ( lActionAfterSaveAs == 5 )
         return( 0 );

      nRC = zwTZOPUPDD_PromptForSave( vMainWindow );

      // if Operation not checked out and user change this Operation, then
      // zeidon call the window "Save Operation as". We save the action after
      // Save as in the View TZSAVEAS (for example: Open an other Operation
      // or Switch Project or Exit Operation Tool)
      if ( nRC == 99 )
      {
         // 4rd Parameter = 5 means: Load save as window only one
         SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", 5 );
         RefreshWindow( vSubtask );
         SetWindowActionBehavior( vMainWindow, zWAB_ProcessImmediateAction |
                                  zWAB_StartModalSubwindow, "TZOPUPDD", "SaveAs" );
         return( 0 );
      }

      if ( nRC != zRESPONSE_CANCEL )
      {
         zwfnTZOPUPDD_LoadNewOperation( vSubtask, vMainWindow, szOldOperation,
                                        szNewOperation );
      }

      RefreshWindow( vMainWindow );
      RefreshWindow( vSubtask );
   }

   return( 0 );
} // zwTZOPUPDD_LoadOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_Set_CM_List
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_Set_CM_List( zVIEW vSubtask )
{
   zVIEW  vTZOGSRCO;
   zVIEW  vCM_List;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZOGSRCO, "Operation" ) >= zCURSOR_SET )
      SetCursorFirstEntityByAttr( vCM_List, "W_MetaDef", "Name",
                                 vTZOGSRCO, "Operation", "Name", "" );

   return( 0 );
} // zwTZOPUPDD_Set_CM_List

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveOperationPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MoveOperationPreBuild( zVIEW vSubtask )
{
   zVIEW  vTZOGSRCO;
   zVIEW  vProfileXFER;
   zVIEW  vCM_List;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vProfileXFER, "OP" ) >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      {
         SetAttributeFromAttribute( vProfileXFER, "OP", "Name",
                                    vCM_List, "W_MetaDef", "Name" );
         SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                                    vCM_List, "W_MetaDef", "Desc" );
      }
      else
      {
         SetAttributeFromAttribute( vProfileXFER, "OP", "Name",
                                    vTZOGSRCO, "Operation", "Name" );
         SetAttributeFromAttribute( vProfileXFER, "DM", "DomainDesc",
                                    vTZOGSRCO, "Operation", "Desc" );
      }

      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_MoveOperation );

      zwTZOPUPDD_CreateWorkLOD( vSubtask );
   }

   return( 0 );
} // zwTZOPUPDD_MoveOperationPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveOperationPostBuil
//
// set Operation Name and Operation Group
// Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MoveOperationPostBuil( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vCM_List;
   zVIEW  vTZOGSRCO;
   zVIEW  vMoveOperation;
   zCHAR  szOperationName[ 33 ];
   zCHAR  szOperationGroup[33];
   zCHAR  szType[ 4 ];
   zSHORT nRC = -1;

   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );

   //was window <Move Domain from Group to Group> opened via Move Button or menu item
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPENOPER" ) >= 0 )
   {
      GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szOperationName, zsizeof( szOperationName ), vCM_List, "W_MetaDef", "Name" );
      GetStringFromAttribute( szOperationGroup, zsizeof( szOperationGroup ), vCM_List, "W_MetaDef", "GroupName" );
      SetCursorFirstEntityByAttr( vMoveOperation, "SourceFile", "Name",
                                  vCM_List, "W_MetaDef", "GroupName", "" );
      GetStringFromAttributeByContext( szType, vMoveOperation, "SourceFile",
                                       "LanguageType", "LanguageType", 4 );
   }
   else
   {
      GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szOperationName, zsizeof( szOperationName ), vTZOGSRCO, "Operation", "Name" );
      GetStringFromAttribute( szOperationGroup, zsizeof( szOperationGroup ), vTZOGSRCO,
                              "GlobalOperationGroup", "Name" );
      GetStringFromAttributeByContext( szType, vTZOGSRCO, "GlobalOperationGroup",
                              "LanguageType", "LanguageType", 4 );
      SetCursorFirstEntityByAttr( vMoveOperation, "SourceFile", "Name",
                                  vTZOGSRCO, "GlobalOperationGroup", "Name", "" );
   }

   SetCtrlText( vSubtask, "txtOperationName", szOperationName );
   SetCtrlText( vSubtask, "txtOperationGroup", szOperationGroup );
   SetCtrlText( vSubtask, "txtType", szType );

   SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED, 1 );

   // set check out state in Title
   zwTZOPUPDD_EnableAndSetCheckOut( vSubtask );

   return( 0 );
} // zwTZOPUPDD_MoveOperationPostBuil

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveCheckGroupType
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_MoveCheckGroupType( zVIEW  vSubtask,
                               zSHORT nCheckIdenticalGroup,
                               zSHORT nCreateNewGroup )
{
   zVIEW    vMoveOperation;
   zVIEW    vWindow;
   zVIEW    vNewGroup;
   zCHAR    szMsg[ 100 ];
   zCHAR    szControltext[ 4 ];
   zCHAR    szLanguageType[ 4 ];
   zCHAR    szGroupName[33];

   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vSubtask, &vWindow, "MoveOperation" );

   // if move Operation with Operation Code
   if ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      GetCtrlText( vWindow, "txtType", szControltext, 4 );
      if ( nCreateNewGroup == 1 )
      {
         GetViewByName( &vNewGroup, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
         GetStringFromAttributeByContext( szLanguageType, vNewGroup, "GlobalOperationGroup",
                                          "LanguageType", "LanguageType", 4 );
      }
      else
      {
         GetStringFromAttributeByContext( szLanguageType, vMoveOperation, "SourceFile",
                                          "LanguageType", "LanguageType", 4 );
      }

      if ( zstrcmp( szControltext, szLanguageType ) != 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Language Type of Operation Group do not match." );
         MessageSend( vSubtask, "OP00819", "Operation Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }
   }

   if ( nCheckIdenticalGroup == 1 )
   {
      // if source and target group identical
      GetCtrlText( vWindow, "txtOperationGroup", szGroupName, 33 );
      if ( CompareAttributeToString( vMoveOperation, "SourceFile", "Name",
                                     szGroupName ) == 0 )
      {
         MessageSend( vSubtask, "OP00819", "Operation Maintenance",
                      "Target and Source Operation Group are identical.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }
   }

   return( 0 );
} // zwTZOPUPDD_MoveCheckGroupType

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_DeleteOperFromFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZOPUPDD_DeleteOperFromFile( zVIEW  vSubtask,
                               zVIEW  vTZOGSRCO,
                               zSHORT nMoveOrDeleteFlag )
{
   zVIEW  vProfileXFER;
   zVIEW  vTaskLPLR;
   zVIEW  vEdWrk;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );

   SetNameForView( vTZOGSRCO, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
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
      // Move Operation from File to File
      TZEDFRMD_MoveOperationCode( vSubtask, zMOVE_OPERATION );
   }
   else
   if ( nMoveOrDeleteFlag == zCOPY_OPERATION )
   {
      // Copy Operation from File to File
      TZEDFRMD_CopyOperationCode( vSubtask, zRENAME_OPERATION );
   }

   return( 0 );
} // zwTZOPUPDD_DeleteOperFromFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveOperationToFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_MoveOperationToFile( zVIEW vSubtask )
{
   zVIEW  vMoveOperation;
   zVIEW  vMoveOperation_Copy;
   zVIEW  vDeleteOp;
   zVIEW  vTZOGSRCO;
   zVIEW  vWindow;
   zCHAR  szGroupName[ 33 ];
   zCHAR  szOperationName[ 33 ];

   // Move Operation Code to new Source File
   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vWindow, "MoveOperation" );

   if ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      CreateViewFromViewForTask( &vMoveOperation_Copy, vMoveOperation, 0 );

      // Create LOD for move Operation Code from old Source Files
      if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) >= 0 )
         DropObjectInstance( vDeleteOp );

      ActivateEmptyObjectInstance( &vDeleteOp, "TZEDWDEL", vSubtask, zMULTIPLE );

      // Create Source File
      CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
      GetCtrlText( vWindow, "txtOperationGroup", szGroupName, 33 );
      SetCursorFirstEntityByString( vMoveOperation_Copy, "SourceFile", "Name",
                                    szGroupName, "" );
      SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                   vMoveOperation_Copy, "SourceFile", zSET_ALL );
      SetNameForView( vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );
      DropView( vMoveOperation_Copy );

      // Create Operation
      CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
      GetCtrlText( vWindow, "txtOperationName", szOperationName, 33 );
      SetAttributeFromString( vDeleteOp, "Operation", "Name", szOperationName );

      // Added new Sourcen File for move Opertion to new File
      CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                   vMoveOperation, "SourceFile", zSET_ALL );

      // Move Operation from Source File
      zwTZOPUPDD_DeleteOperFromFile( vWindow, vTZOGSRCO, zMOVE_OPERATION );
   } //endif ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ...

   return( 0 );
} // zwTZOPUPDD_MoveOperationToFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveOperToNewGroup
//
// move Operation to new Operation Group
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MoveOperToNewGroup( zVIEW vSubtask )
{
   zVIEW   vCM_ListGroup;
   zVIEW   vCM_List;
   zVIEW   vTarget;
   zVIEW   vSource;
   zVIEW   vProfileXFER;
   zVIEW   vWindow;
   zVIEW   vMoveOperation;
   zCHAR   szOperationName[ 33 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );

   // Was window <Move Operation from Group to Group> opened via
   // Move Button or menu item?  Get the Operation to be moved.
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPENOPER" ) >= 0 )
      ActivateMetaOI( vSubtask, &vSource, vCM_List, zSOURCE_GO_META, zSINGLE );
   else
      GetViewByName( &vSource, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szOperationName, zsizeof( szOperationName ), vSource, "Operation", "Name" );

   // Move Operation in a new Operation Group?
   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_MoveOperationInNewGroup ) == 0 )
   {
      if ( zwTZOPUPDD_MoveCheckGroupType( vSubtask, 0, 1 ) < 0 )
         return( -1 );
      GetViewByName( &vTarget, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
      RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zREFER_GOPGRP_META );
      // added new Operation Group to Work LOD
      CreateEntity( vMoveOperation, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vMoveOperation, "SourceFile",
                                   vTarget, "GlobalOperationGroup", zSET_ALL );
   }
   else
   {
      if ( zwTZOPUPDD_MoveCheckGroupType( vSubtask, 1, 0 ) < 0 )
         return( -1 );
      // Get the Operation Group which will hold the new Operation
      ActivateMetaOI( vSubtask, &vTarget, vCM_ListGroup, zSOURCE_GOPGRP_META, zSINGLE );
      CreateEntity ( vTarget, "Operation", zPOS_AFTER );
   }

   // reuse ZKey
   SetMatchingAttributesByName( vTarget, "Operation", vSource, "Operation", zSET_ALL );
   // Create Parameter for Operation
   zwTZOPUPDD_CreateAllParameter( vSubtask, vSource, vTarget );

   DeleteEntity( vSource, "Operation", zREPOS_NEXT );

   // First COMMIT the Operation Group from which the Operation in was deleted.
   CommitMetaOI( vSubtask, vSource, zSOURCE_GOPGRP_META );

   // Second, COMMIT the Operation Group with the new Operation.
   // This order is important because if we reverse it, the Operation will be added twice.
   CommitMetaOI( vSubtask, vTarget, zSOURCE_GOPGRP_META );

   // Finally, drop the source Operation Group and the current TZOGSRCO Operation.
   // Group, if there is one, and name the target Operation Group TZOGSRCO.
   DropMetaOI( vSubtask, vSource );

   if ( GetViewByName( &vSource, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vSource );

   SetNameForView( vTarget, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByString( vTarget, "Operation", "Name", szOperationName, "" );
   SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", szOperationName, "" );
   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag", "" );

   // move Operation Code
   zwTZOPUPDD_MoveOperationToFile( vSubtask );

   // Drop Work LOD for Move Operation
   DropObjectInstance( vMoveOperation );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZOPUPDD" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
} // zwTZOPUPDD_MoveOperToNewGroup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_CreateAllParameter
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZOPUPDD_CreateAllParameter( zVIEW vSubtask,
                               zVIEW vSource,
                               zVIEW vTarget )
{
   zSHORT   nRC;

   nRC = SetCursorFirstEntity( vSource, "Parameter", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateMetaEntity( vSubtask, vTarget, "Parameter", zPOS_AFTER );
      SetMatchingAttributesByName( vTarget, "Parameter", vSource, "Parameter", zSET_NULL );
      nRC = SetCursorNextEntity( vSource, "Parameter", "" );
   }

   return( 0 );
} // zwTZOPUPDD_CreateAllParameter

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_SaveAsPreBuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   zwTZOPUPDD_OpenOperPreBuild( vSubtask );
   zwTZOPUPDD_NewOperPrebuild( vSubtask );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // set save as flag
   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                           zOP_SaveAsOperation );

   zwTZOPUPDD_CreateWorkLOD( vSubtask );

   return( 0 );
} // zwTZOPUPDD_SaveAsPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveAsPostBuild( zVIEW vSubtask )
{
   zVIEW   vTZOGSRCO;
   zVIEW   vCM_List;
   zVIEW   vMoveOperation;
   zVIEW   vProfileXFER;
   zCHAR   szOperationName[ 33 ];

   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szOperationName, zsizeof( szOperationName ), vTZOGSRCO, "Operation", "Name" );

   //if edit new Operation Name in main window
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szOperationName, "" ) < zCURSOR_SET )
   {
      SetAttributeFromString( vProfileXFER, "OP", "Name", szOperationName );
   }
   else
   {
      SetAttributeFromString( vProfileXFER, "OP", "Name", "" );
      SetCtrlText( vSubtask, "txtOperationGroup", "" );
   }

   SetCtrlText( vSubtask, "txtOperationGroup", "" );
   SetCtrlText( vSubtask, "txtType", "" );
   SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED, 1 );
   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );

   //set current Description
   SetAttributeFromAttribute( vProfileXFER, "OP", "Desc",
                              vTZOGSRCO, "Operation", "Desc" );

   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vCM_List, "W_MetaDef", 0 );

   if ( CheckExistenceOfEntity( vMoveOperation, "SourceFile" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vMoveOperation, "SourceFile", 0 );

   return( 0 );

} // zwTZOPUPDD_SaveAsPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_RefreshGroupList
//
// if Single Click on Operation List, then set selection in Group List
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_RefreshGroupList( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vMoveOperation;

   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );

   SetSelectStateOfEntity( vMoveOperation, "SourceFile", 0 );

   SetCursorFirstEntityByAttr( vMoveOperation, "SourceFile", "Name",
                               vCM_List, "W_MetaDef", "GroupName", "" );
   SetSelectStateOfEntity( vMoveOperation, "SourceFile", 1 );

   RefreshCtrl( vSubtask, "lbGroupList" );

   // set check out state in Title
   zwTZOPUPDD_EnableAndSetCheckOut( vSubtask );

   return( 0 );
} // zwTZOPUPDD_RefreshGroupList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SetSaveAsName
//
// if Double Click on Operation List, then set Operation Name, Description and Group Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SetSaveAsName( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vMoveOperation;
   zVIEW   vProfileXFER;
   zVIEW   vTZOGSRCO;
   zCHAR   szGroupName[33];
   zCHAR   szType[ 4 ];

   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetSelectStateOfEntity( vMoveOperation, "SourceFile", 0 );

   SetAttributeFromAttribute( vProfileXFER, "OP", "Name",
                              vCM_List, "W_MetaDef", "Name" );

   GetStringFromAttribute( szGroupName, zsizeof( szGroupName ), vCM_List, "W_MetaDef", "GroupName" );
   GetStringFromAttributeByContext( szType, vMoveOperation, "SourceFile",
                                    "LanguageType", "LanguageType", 4 );
   SetCtrlText( vSubtask, "txtOperationGroup", szGroupName );
   SetCtrlText( vSubtask, "txtType", szType );

   SetCursorFirstEntityByAttr( vMoveOperation, "SourceFile", "Name",
                               vCM_List, "W_MetaDef", "GroupName", "" );
   SetSelectStateOfEntity( vMoveOperation, "SourceFile", 1 );

   // set Description
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) != 1 )
   {
      // no Description in View vCM_List or vCM_ListGroup
      ActivateMetaOI( vSubtask, &vTZOGSRCO, vCM_List,
                      zREFER_GO_META, zSINGLE | zLEVEL_TASK );
      SetAttributeFromAttribute( vProfileXFER, "OP", "Desc",
                                 vTZOGSRCO, "Operation", "Desc" );
      DropMetaOI( vSubtask, vTZOGSRCO );
      RefreshCtrl( vSubtask, "edOperationDesc" );
   }

   RefreshCtrl( vSubtask, "txtOperationGroup" );
   RefreshCtrl( vSubtask, "edOperationName" );
   RefreshCtrl( vSubtask, "lbGroupList" );
   RefreshCtrl( vSubtask, "txtType" );

   // set check out state in Title
   zwTZOPUPDD_EnableAndSetCheckOut( vSubtask );

   return( 0 );
} // zwTZOPUPDD_SetSaveAsName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SetGroupName
//
// if single or double click on Group List, then set Group Name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SetGroupName( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vMoveOperation;
   zVIEW   vProfileXFER;
   zCHAR   szGroupName[33];
   zCHAR   szType[ 4 ];

   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szGroupName, zsizeof( szGroupName ), vMoveOperation, "SourceFile", "Name" );
   GetStringFromAttributeByContext( szType, vMoveOperation, "SourceFile",
                                    "LanguageType", "LanguageType", 4 );
   SetCtrlText( vSubtask, "txtOperationGroup", szGroupName );
   SetCtrlText( vSubtask, "txtType", szType );
   RefreshCtrl( vSubtask, "txtOperationGroup" );
   RefreshCtrl( vSubtask, "txtType" );

   SetSelectStateOfEntity( vCM_List, "W_MetaDef", 0 );
   RefreshCtrl( vSubtask, "lbOperationList" );

   // set check out state in Title
   zwTZOPUPDD_EnableAndSetCheckOut( vSubtask );

   return( 0 );
} // zwTZOPUPDD_SetGroupName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsInNewGroup
//
// Save Operation as in new Operation Group
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveAsInNewGroup( zVIEW vSubtask )
{
   zVIEW  vTZOGSRCO_DETAIL;
   zVIEW  vProfileXFER;
   zVIEW  vCM_List;
   zVIEW  vCM_List_Copy;
   zCHAR  szOutName[ 33 ];
   zCHAR  szNewName[ 33 ];
   zCHAR  szGroupName[33];
   zCHAR  szMsg[ 100 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vProfileXFER, "OP", "Name" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   // Check that a Operation Name has been entered.
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      MessageSend( vSubtask, "DM00103", "Operation Maintenance",
                   "Operation Name required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag", "" );
      SetFocusToCtrl( vSubtask, "edOperationName" );
      return( -1 );
   }

   // Check: if existing Operation?
   CreateViewFromViewForTask( &vCM_List_Copy, vCM_List, 0 );
   if ( SetCursorFirstEntityByString( vCM_List_Copy, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      GetStringFromAttribute( szGroupName, zsizeof( szGroupName ), vCM_List_Copy, "W_MetaDef", "GroupName" );
      strcpy_s( szMsg, zsizeof( szMsg ), "Cannot create a new Operation Group for an "
                      "existing Operation." );
      strcat_s( szMsg, zsizeof( szMsg ), "\nOperation exists in Operation Group '" );
      strcat_s( szMsg, zsizeof( szMsg ), szGroupName );
      strcat_s( szMsg, zsizeof( szMsg ), "'.");
      MessageSend( vSubtask, "DM00103", "Operation Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetAttributeFromString( vProfileXFER, "OP",
                              "NewOperationGroupFlag", "" );
      DropView( vCM_List_Copy );
      return( -1 );
   }
   DropView( vCM_List_Copy );

   ActivateEmptyMetaOI( vSubtask, &vTZOGSRCO_DETAIL, zSOURCE_GOPGRP_META, zSINGLE );
   CreateMetaEntity( vSubtask, vTZOGSRCO_DETAIL, "GlobalOperationGroup", zPOS_AFTER );
   SetNameForView( vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                           zOP_SaveAsOperationInNewGroup );

   return( 0 );
} // zwTZOPUPDD_SaveAsInNewGroup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_ResetProfileXFER
//
// Reset View ProfileXFER
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_ResetProfileXFER( zVIEW vSubtask )
{
   zVIEW   vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetAttributeFromString( vProfileXFER, "OP", "Name", "" );
   SetAttributeFromString( vProfileXFER, "OP", "Desc", "" );
   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag", "" );

   return( 0 );
} // zwTZOPUPDD_ResetProfileXFER

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsCheckValues
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZOPUPDD_SaveAsCheckValues( zVIEW  vSubtask,
                              zPCHAR szGroupName,
                              zPCHAR szOutName )
{
   zCHAR   szNewName[ 33 ];
   zCHAR   szControltext[ 4 ];
   zCHAR   szLanguageType[ 4 ];
   zCHAR   szMsg[ 100 ];
   zVIEW   vProfileXFER;
   zVIEW   vTZOGSRCO;
   zVIEW   vCM_List;
   zVIEW   vCM_List_Copy;
   zVIEW   vWindow;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" );

   //Operation Group Name is required
   GetCtrlText( vWindow, "txtOperationGroup", szGroupName, 33 );
   if ( zstrcmp( szGroupName, "" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00137", "Operation Maintenance",
                   "Target Group Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   //Operation Name is required
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vProfileXFER, "OP", "Name" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00137", "Operation Maintenance",
                   "Operation Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vWindow, "edOperationName" );
      return( -2 );
   }

   CreateViewFromViewForTask( &vCM_List_Copy, vCM_List, 0 );
   if ( SetCursorFirstEntityByString( vCM_List_Copy, "W_MetaDef", "Name",
                                      szOutName, "" ) >= zCURSOR_SET &&
        CompareAttributeToString( vCM_List_Copy, "W_MetaDef", "GroupName",
                                  szGroupName ) != 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Cannot save Operation, because already\n" );
      strcat_s( szMsg, zsizeof( szMsg ), "exists in Operation Group '" );
      GetStringFromAttribute( szNewName, zsizeof( szNewName ), vCM_List_Copy, "W_MetaDef", "GroupName" );
      strcat_s( szMsg, zsizeof( szMsg ), szNewName );
      strcat_s( szMsg, zsizeof( szMsg ), "'.");
      MessageSend( vSubtask, "ZO00137", "Operation Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // if save Operation as with Operation Code
   if ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      GetCtrlText( vWindow, "txtType", szControltext, 4 );
      GetStringFromAttributeByContext( szLanguageType, vTZOGSRCO,
                                       "GlobalOperationGroup",
                                       "LanguageType", "LanguageType", 4 );
      if ( zstrcmp( szControltext, szLanguageType ) != 0 )
      {
         MessageSend( vSubtask, "OP00819", "Operation Maintenance",
                      "Language Type of Operation Group do not match.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }
   }

   return( 0 );
} // zwTZOPUPDD_SaveAsCheckValues

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_CopyOperationWorkLOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_CopyOperationWorkLOD( zVIEW  vSubtask,
                                 zVIEW  vTZOGSRCO,
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
                                vTZOGSRCO, "GlobalOperationGroup", zSET_ALL );

   // Create Operation
   CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
   SetAttributeFromAttribute( vDeleteOp, "Operation", "Name",
                              vTZOGSRCO, "Operation", "Name" );

   return( 0 );
} // zwTZOPUPDD_CopyOperationWorkLOD

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveAsOperation( zVIEW vSubtask )
{
   zVIEW   vTZOGSRCO;
   zVIEW   vCM_List;
   zVIEW   vProfileXFER;
   zVIEW   vWindow;
   zCHAR   szGroupName[33];
   zCHAR   szOutName[ 33 ];
   zSHORT  nCopyCode = 0;
   zSHORT  nRC;

   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   szGroupName[ 0 ] = 0;
   szOutName[ 0 ] = 0;

   if ( zwTZOPUPDD_SaveAsCheckValues( vSubtask, szGroupName, szOutName ) < 0 )
      return( -1 );

   // if copy Operation Code
   GetSubtaskForWindowName( vSubtask, &vWindow, "SaveAs" );
   if ( GetCtrlState( vWindow, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
      nCopyCode = 1;

   GetSubtaskForWindowName( vSubtask, &vWindow, "TZOPUPDD" );
   MapWindow( vWindow );

   //Operation Name already exists
   SetAttributeFromString( vProfileXFER, "OP", "Name", szOutName );
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                     szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      nRC = MessagePrompt( vSubtask, "ZO00138", "Operation Maintenance",
                           "Operation already exists. Replace existing Operation?",
                           zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edOperationName" );
         return( -1 );
      }

      // if this Operation open, then save Operation, not save as
      if ( CompareAttributeToString( vTZOGSRCO, "Operation",
                                     "Name", szOutName ) == 0 )
      {
         zwfnSaveOperation( vWindow, FALSE );
         zwTZOPUPDD_ResetProfileXFER( vSubtask );
         return( 0 );
      }
      else
      {
         // if another Domain open, then replace existing Domain
         fnzwTZOPUPDD_DeleteOperation( vSubtask, 0, nCopyCode );
      }

   }

   zwTZOPUPDD_SaveAsNewOperation( vSubtask, szGroupName, szOutName, nCopyCode );
   zwTZOPUPDD_ResetProfileXFER( vSubtask );
   RefreshWindow( vWindow );
   zwTZOPUPDD_CheckActionAfterSaveAs( vSubtask );

   return( 0 );
} // zwTZOPUPDD_SaveAsOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsNewOperation
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZOPUPDD_SaveAsNewOperation( zVIEW  vSubtask,
                               zPCHAR szGroupName,
                               zPCHAR szOperationName,
                               zSHORT nCopyCode )
{
   zVIEW   vProfileXFER;
   zVIEW   vSource;
   zVIEW   vTarget;
   zVIEW   vCM_List;
   zVIEW   vCM_ListGroup;
   zVIEW   vMoveOperation;
   zCHAR   szOperationGroup[33];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vSource, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );

   if ( nCopyCode == 1 )
      zwTZOPUPDD_CopyOperationWorkLOD( vSubtask, vSource, 1 );

   GetCtrlText( vSubtask, "txtOperationGroup", szOperationGroup, 33 );
   SetCursorFirstEntityByString( vCM_ListGroup, "W_MetaDef", "Name",
                                 szOperationGroup, 0 );

   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_SaveAsOperationInNewGroup ) == 0 )
   {
      GetViewByName( &vTarget, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
      RetrieveViewForMetaList( vSubtask, &vCM_ListGroup, zSOURCE_GOPGRP_META );
   }
   else
   {
      SetCursorFirstEntityByString( vCM_ListGroup, "W_MetaDef", "Name",
                                    szGroupName, 0 );
      ActivateMetaOI( vSubtask, &vTarget, vCM_ListGroup, zSOURCE_GOPGRP_META, zSINGLE );
   }

   CreateMetaEntity( vSubtask, vTarget, "Operation", zPOS_AFTER );
   SetMatchingAttributesByName( vTarget, "Operation", vSource,
                                "Operation", zSET_NULL );
   SetAttributeFromAttribute( vTarget, "Operation", "Name",
                              vProfileXFER, "OP", "Name" );
   SetAttributeFromAttribute( vTarget, "Operation", "Desc",
                              vProfileXFER, "OP", "Desc" );

   // Create Parameter for Operation
   zwTZOPUPDD_CreateAllParameter( vSubtask, vSource, vTarget );

   CommitMetaOI( vSubtask, vTarget, zSOURCE_GOPGRP_META );

   if ( GetViewByName( &vSource, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vSource );

   SetNameForView( vTarget, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByString( vTarget, "Operation", "Name", szOperationName, "" );
   SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", szOperationName, "" );

   // copy Operation Code
   if ( nCopyCode == 1 )
   {
      zwTZOPUPDD_CopyOperationWorkLOD( vSubtask, vTarget, 0 );
      zwTZOPUPDD_DeleteOperFromFile( vSubtask, vTarget, zCOPY_OPERATION );
   }

    // Drop Work LOD for Save As Window
   if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveOperation );

   return( 0 );
} // zwTZOPUPDD_SaveAsNewOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZOPUPDD_InitMenuDisableAll
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
zwfnTZOPUPDD_InitMenuDisableAll( zVIEW vSubtask )
{
   zVIEW vLPLR;
   zBOOL bEnable = TRUE;

   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddParamenter", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateParameter", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteParamenter", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "MoveOperation", zOPTION_STATUS_ENABLED, 0 );

   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      bEnable = FALSE;

   SetOptionState( vSubtask, "New", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Open", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "OperationGroupList", zOPTION_STATUS_ENABLED, bEnable );

   return( 0 );
} // zwfnTZOPUPDD_InitMenuDisableAll

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_InitMenu
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_InitMenu( zVIEW vSubtask )
{
   zVIEW   vTZOGSRCO;
   zVIEW   vWindow;
   zSHORT  nIsCheckedOut;

   zwfnTZOPUPDD_InitMenuDisableAll( vSubtask );

   if ( GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZOGSRCO, zSOURCE_GOPGRP_META );

   if ( nIsCheckedOut )
   {
      SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddParamenter", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "MoveOperation", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( CheckExistenceOfEntity( vTZOGSRCO, "Parameter" ) >= zCURSOR_SET &&
        GetSelectStateOfEntity( vTZOGSRCO, "Parameter" ) == 1 )
   {
      SetOptionState( vSubtask, "UpdateParameter", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
         SetOptionState( vSubtask, "DeleteParamenter", zOPTION_STATUS_ENABLED, 1 );
   }

   //Operation Group List
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERGRPLIST" ) >= 0 )
      SetOptionState( vSubtask, "OperationGroupList", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetOptionState( vSubtask, "OperationGroupList", zCONTROL_STATUS_CHECKED, 0 );

   //Operation List
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLISTunderGRP" ) >= 0 )
      SetOptionState( vSubtask, "OperationsInGroup", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetOptionState( vSubtask, "OperationsInGroup", zCONTROL_STATUS_CHECKED, 0 );

   return( 0 );
} // zwTZOPUPDD_InitMenu

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_Check_CheckoutStatus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_Check_CheckoutStatus( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_ListGrp;
   zVIEW   vCM_ListGroup;
   zSHORT  nEnable = 0;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vCM_ListGrp, "CM_ListGroup", vSubtask, zLEVEL_TASK ) >= 0 &&
        GetViewByName( &vCM_List, "CMLIST", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      CreateViewFromViewForTask( &vCM_ListGroup, vCM_ListGrp, 0 );
      SetCursorFirstEntityByInteger( vCM_ListGroup, "W_MetaType", "Type",
                                     zREFER_GOPGRP_META, "" );

      if ( CheckExistenceOfEntity( vCM_ListGroup, "W_MetaDef" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      {
         // Position on the correct Domain within the group type.
         SetCursorFirstEntityByAttr( vCM_ListGroup, "W_MetaDef", "Name",
                                     vCM_List, "W_MetaDef", "GroupName", "" );

         nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );
         if ( nEnable )
            bEnable = TRUE;
      }
      DropView( vCM_ListGroup );
   }

   SetCtrlState( vSubtask, "Delete", zOPTION_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "MoveOperation", zOPTION_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "DeleteOperation", bEnable );

   return( 0 );
} // zwTZOPUPDD_Check_CheckoutStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_GroupCheckedOut
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_GroupCheckedOut( zVIEW vSubtask )
{
   zVIEW    vCM_ListGroup;
   zSHORT   nEnable = 0;

   if ( GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK ) >= 0 )
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );

   return( nEnable );
} // zwTZOPUPDD_GroupCheckedOut

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_OperationGroupPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_OperationGroupPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zSHORT   nEnable = 0;

   nEnable = zwTZOPUPDD_GroupCheckedOut( vSubtask );

   EnableAction( vSubtask, "DeleteOperationGroup", nEnable );

   CreateTrackingPopupMenu( vSubtask, "GroupPupup", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
} // zwTZOPUPDD_OperationGroupPopup


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_EnableAndSetCheckOut
//
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_EnableAndSetCheckOut( zVIEW vSubtask )
{
   zVIEW    vCM_ListGroup;
   zVIEW    vMoveOperation;
   zSHORT   nEnable = 0;

   if ( GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         SetCursorFirstEntityByAttr( vCM_ListGroup, "W_MetaDef", "Name",
                                     vMoveOperation, "SourceFile", "Name", "" );
      }

      nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
} // zwTZOPUPDD_EnableAndSetCheckOut

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SaveAsCancel( zVIEW vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vMoveOperation;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   zwTZOPUPDD_ResetProfileXFER( vSubtask );

   if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveOperation );

   return( 0 );
} // zwTZOPUPDD_SaveAsCancel

static zSHORT
zwTZOPUPDD_CheckActionAfterSaveAs( zVIEW  vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vOperGrp;
   zVIEW   vParentWindow;
   zLONG   lActionAfterSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   GetParentWindow( &vParentWindow, vSubtask );
   SetTitleWithCheckOutState( vParentWindow, "Operation Maintenance", "TZOGSRCO",
                               vOperGrp, "Operation", zSOURCE_GOPGRP_META );

   // if Operation not checked out and user change this Operation, then
   // zeidon call the window "Save Operation as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Operation or Open an other Operation or Switch Project
   // or Exit Operation Tool)

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   switch ( lActionAfterSaveAs )
   {
      case zNewComponentAfterSaveAs:   // Create new Operation
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZOPUPDD", "NEWOPER" );
         break;

      case zOpenComponentAfterSaveAs:   // Open Operation
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZOPUPDD", "OPENOPER" );
         break;

      case zSwitchLPLRAfterSaveAs:   // Switch Project
         zwTZOPUPDD_SwitchLPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZCMSLPD", "SwitchLPLR" );
         break;

      case zExitToolAfterSaveAs:   // Exit Operation Tool
         zwTZOPUPDD_ExitDialog( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, "", "" );
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_NewOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_NewOperation( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwTZOPUPDD_PromptForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Operation not checked out and user change this Operation, then
            // zeidon call the window "Save Operation as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Operation or Open an other Operation or Switch Project
            // or Exit Operation Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNewComponentAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   return( 0 );
} // zwTZOPUPDD_NewOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MenuOpenOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_MenuOpenOperation( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwTZOPUPDD_PromptForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Operation not checked out and user change this Operation, then
            // zeidon call the window "Save Operation as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Operation or Open an other Operation or Switch Project
            // or Exit Operation Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zOpenComponentAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   return( 0 );
} // zwTZOPUPDD_MenuOpenOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SwitchLPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPUPDD_SwitchLPLR( zVIEW vSubtask )
{
   zVIEW  vT;
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwTZOPUPDD_PromptForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Operation not checked out and user change this Operation, then
            // zeidon call the window "Save Operation as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Operation or Open an other Operation or Switch Project
            // or Exit Operation Tool)
            // 4rd Parameter = 3 means: Switch Project after save as.
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zSwitchLPLRAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   if ( GetViewByName( &vT, "CMLIST", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vT );

   if ( GetViewByName( &vT, "CM_ListGroup", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vT );

   return( 0 );
} // zwTZOPUPDD_SwitchLPLR

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsKeepCurrentDesc
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vTZOGSRCO;
   zVIEW  vProfileXFER;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetAttributeFromAttribute( vProfileXFER, "OP", "Desc",
                                 vTZOGSRCO, "Operation", "Desc" );
      RefreshCtrl( vSubtask, "edOperationDesc" );
   }

   return( 0 );
} // zwTZOPUPDD_SaveAsKeepCurrentDesc

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_AcceptSubobject
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZOPUPDD_AcceptSubobject( zVIEW vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szAttributeName,
                            zPCHAR szMessageText )
{
   zVIEW    vTZOGSRCO;
   zVIEW    vTZOGSRCO_Copy;
   zCHAR    szNewName[ 100 ];
   zCHAR    szMessage[ 200 ];
   zSHORT   nRC;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vTZOGSRCO, szEntityName, szAttributeName );

   CreateViewFromViewForTask( &vTZOGSRCO_Copy, vTZOGSRCO, 0 );

   nRC = SetCursorFirstEntityByString( vTZOGSRCO_Copy, szEntityName,
                                       szAttributeName, szNewName, "" );
   nRC = SetCursorNextEntityByString( vTZOGSRCO_Copy, szEntityName,
                                      szAttributeName, szNewName, "" );
   DropView( vTZOGSRCO_Copy );

   if ( nRC == zCURSOR_SET )
   {
      strcpy_s( szMessage, zsizeof( szMessage ), "A " );
      strcat_s( szMessage, zsizeof( szMessage ), szMessageText );
      strcat_s( szMessage, zsizeof( szMessage ), " already exists by this name!" );
      MessageSend( vSubtask, "OP00814", "Operations",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edDescription" );
      return( -1 );
   }

   if ( AcceptSubobject( vTZOGSRCO, szEntityName ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
} // zwTZOPUPDD_AcceptSubobject

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_DeleteParameter
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_DeleteParameter( zVIEW vSubtask )
{
   zVIEW    vTZOGSRCO;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zSHORT   nCloseWindow = 0;
   zSHORT   nRC;
   zCHAR    szShortDesc[ 65 ];
   zCHAR    szMsg[ 150 ];

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szShortDesc, zsizeof( szShortDesc ), vTZOGSRCO, "Parameter", "ShortDesc" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Parameter '" );
   strcat_s( szMsg, zsizeof( szMsg ), szShortDesc );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );
   nRC = MessagePrompt( vSubtask, "OP00305", "Operation Maintenance", szMsg, 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_NO )
      return( 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "PARAMETEREDIT" ) >= 0 )
      nCloseWindow = 1;

   DeleteEntity( vTZOGSRCO, "Parameter", zREPOS_PREV );

   if ( nCloseWindow == 1 &&
        CheckExistenceOfEntity( vTZOGSRCO, "Parameter" ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }
   else
   {
      if ( nCloseWindow == 1 )
      {
         SetFocusToCtrl( vSubtask, "edDescription" );
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
      }
   }

   return( 0 );
} // zwTZOPUPDD_DeleteParameter

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_ParameterAccept
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_ParameterAccept( zVIEW vSubtask )
{
   zVIEW    vTZOGSRCO;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   // Accept the current Parameter subobject.
   if ( zwTZOPUPDD_AcceptSubobject( vSubtask, "Parameter",
                                    "ShortDesc", "Parameter" ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
} // zwTZOPUPDD_ParameterAccept

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_ParameterNext
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_ParameterNext( zVIEW vSubtask )
{
   zVIEW    vTZOGSRCO;
   zVIEW    vParentWindow;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   // Accept the current Parameter subobject.
   if ( zwTZOPUPDD_AcceptSubobject( vSubtask, "Parameter",
                                    "ShortDesc", "Parameter" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vTZOGSRCO, "Parameter", 0 );
   SetCursorNextEntity( vTZOGSRCO, "Parameter", 0 );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vTZOGSRCO, "Parameter" );
   SetSelectStateOfEntity( vTZOGSRCO, "Parameter", 1 );
   SetFocusToCtrl( vSubtask, "edDescription" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
} // zwTZOPUPDD_ParameterNext

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_ParameterPrev
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_ParameterPrev( zVIEW vSubtask )
{
   zVIEW    vTZOGSRCO;
   zVIEW    vParentWindow;

   GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );

   // Accept the current Parameter subobject.
   if ( zwTZOPUPDD_AcceptSubobject( vSubtask, "Parameter",
                                    "ShortDesc", "Parameter" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vTZOGSRCO, "Parameter", 0 );
   SetCursorPrevEntity( vTZOGSRCO, "Parameter", 0 );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vTZOGSRCO, "Parameter" );
   SetSelectStateOfEntity( vTZOGSRCO, "Parameter", 1 );
   SetFocusToCtrl( vSubtask, "edDescription" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
} // zwTZOPUPDD_ParameterPrev

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_ShowDetail
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_ShowDetail( zVIEW vSubtask )
{
   zVIEW    vCM_ListGroup;
   zVIEW    vProfileXFER;
   zVIEW    vTZOGSRCO_DETAIL;
   zVIEW    vTZOGSRCO;
   zVIEW    vMoveOperation;
   zBOOL    bLoadCurrentGroup = FALSE;

   // Activate the Operation Group Detail for the Oper Group Update window.
   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( CheckExistenceOfEntity( vCM_ListGroup, "W_MetaDef" ) < zCURSOR_SET ||
           CompareAttributeToAttribute( vCM_ListGroup, "W_MetaDef", "CPLR_ZKey",
                                        vTZOGSRCO, "GlobalOperationGroup",
                                        "ZKey" ) == 0 )
      {
         bLoadCurrentGroup = TRUE;
      }
      else
      if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) >= 0 &&
           CompareAttributeToAttribute( vMoveOperation, "SourceFile", "ZKey",
                                        vTZOGSRCO, "GlobalOperationGroup",
                                        "ZKey" ) == 0 )
      {
         bLoadCurrentGroup = TRUE;
      }
   }

   if ( bLoadCurrentGroup )
   {
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_LoadGroupProperties );
      if ( GetViewByName( &vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         DropView( vTZOGSRCO_DETAIL );
      }

      GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vTZOGSRCO_DETAIL, vTZOGSRCO, 0 );
      SetNameForView( vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // Activate and Name
      ActivateMetaOI( vSubtask, &vTZOGSRCO_DETAIL, vCM_ListGroup, zSOURCE_GOPGRP_META, zSINGLE );
      SetNameForView( vTZOGSRCO_DETAIL, "TZOGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

      // set flag for update Group Detail with commit
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_UpdateGroupDetail );
   }

   return( 0 );
} // zwTZOPUPDD_ShowDetail

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SetFlagForNewOper
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_SetFlagForNewOper( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( CompareAttributeToString( vProfileXFER, "OP", "NewOperationGroupFlag",
                                  zOP_ExitNewOperation ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag", "" );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
   }
   else
   {
      // set flag for new Operation
      SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                              zOP_NewOperation );
   }

   return( 0 );
} // zwTZOPUPDD_SetFlagForNewOper

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveRtnFromSubwindow
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_MoveRtnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vMoveOperation;
   zVIEW    vTZOGSRCO;
   zLONG    lZKey;
   zCHAR    szLanguageType[ 4 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // set flag for move Operation
   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                           zOP_MoveOperation );

   // save ZKey for set cursor to selected source file
   GetIntegerFromAttribute( &lZKey, vMoveOperation, "SourceFile", "ZKey" );
   zwTZOPUPDD_CreateWorkLOD( vSubtask );

   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vMoveOperation, "SourceFile", "ZKey", lZKey, "" );

   // if change current Group, set new Values
   if (  GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 &&
         CompareAttributeToInteger( vTZOGSRCO, "GlobalOperationGroup",
                                    "ZKey", lZKey ) == 0 )
   {
      GetStringFromAttributeByContext( szLanguageType, vMoveOperation, "SourceFile",
                                       "LanguageType", "LanguageType", 4 );
      SetCtrlText( vSubtask, "txtType", szLanguageType );
   }

   RefreshWindow( vSubtask );

   return( 0 );
} // zwTZOPUPDD_MoveRtnFromSubwindow

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_SaveAsRtnFromSubwnd
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_SaveAsRtnFromSubwnd( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vMoveOperation;
   zCHAR    szGroupName[33];
   zCHAR    szLanguageType[ 4 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // set flag for save as Operation
   SetAttributeFromString( vProfileXFER, "OP", "NewOperationGroupFlag",
                           zOP_SaveAsOperation );

   // save GroupName for set cursor to selected Operation Group
   GetCtrlText( vSubtask, "txtOperationGroup", szGroupName, 33 );
   zwTZOPUPDD_CreateWorkLOD( vSubtask );

   GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByString( vMoveOperation, "SourceFile", "Name", szGroupName, "" );

   // Set new LanguageType
   GetStringFromAttributeByContext( szLanguageType, vMoveOperation, "SourceFile",
                                    "LanguageType", "LanguageType", 4 );
   SetCtrlText( vSubtask, "txtType", szLanguageType );
   SetSelectStateOfEntity( vMoveOperation, "SourceFile", 1 );

   RefreshWindow( vSubtask );

   return( 0 );
} // zwTZOPUPDD_SaveAsRtnFromSubwnd

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_LoadOperGroupList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_LoadOperGroupList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERGRPLIST" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZOPUPDD_LoadOperGroupList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_LoadOperationList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_LoadOperationList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLISTunderGRP" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZOPUPDD_LoadOperationList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_CreateWorkLOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_CreateWorkLOD( zVIEW vSubtask )
{
   zVIEW  vMoveOperation;
   zVIEW  vOperGrp;
   zVIEW  vTZOGSRCO;
   zVIEW  vCM_Group_Copy;
   zVIEW  vCM_ListGroup;
   zSHORT nRC;

   GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vCM_Group_Copy, vCM_ListGroup, 0 );

   if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveOperation );

   ActivateEmptyObjectInstance( &vMoveOperation, "TZEDWDEL", vSubtask, zMULTIPLE );

   for ( nRC = SetCursorFirstEntity( vCM_Group_Copy, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vCM_Group_Copy, "W_MetaDef", "" ) )
   {
      // Activate and Name
      ActivateMetaOI( vSubtask, &vOperGrp, vCM_Group_Copy, zREFER_GOPGRP_META, zSINGLE );

      CreateEntity( vMoveOperation, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vMoveOperation, "SourceFile",
                                   vOperGrp, "GlobalOperationGroup", zSET_ALL );
      SetAttributeFromAttribute( vMoveOperation, "SourceFile", "Status",
                                 vCM_Group_Copy, "W_MetaDef", "Status" );
      DropMetaOI( vSubtask, vOperGrp );
   }

   SetNameForView( vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK );
   DropView( vCM_Group_Copy );

   // load Values from current Group
   if ( GetViewByName( &vTZOGSRCO, "TZOGSRCO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( SetCursorFirstEntityByAttr( vMoveOperation, "SourceFile", "ZKey",
                                       vTZOGSRCO, "GlobalOperationGroup", "ZKey",
                                       "" ) < zCURSOR_SET )
      {
         // current Operation Group is a new Group, is not in CM_ListGroup
         CreateEntity( vMoveOperation, "SourceFile", zPOS_AFTER );
         SetMatchingAttributesByName( vMoveOperation, "SourceFile",
                                      vTZOGSRCO, "GlobalOperationGroup", zSET_ALL );
         SetAttributeFromInteger( vMoveOperation, "SourceFile", "Status", 1 );
      }
      else
      {
         SetMatchingAttributesByName( vMoveOperation, "SourceFile",
                                      vTZOGSRCO, "GlobalOperationGroup", zSET_ALL );
      }
   }

   return( 0 );
} // zwTZOPUPDD_CreateWorkLOD

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_MoveOperationCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_MoveOperationCancel( zVIEW vSubtask )
{
   zVIEW vMoveOperation;

   if ( GetViewByName( &vMoveOperation, "MoveOperation", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMoveOperation );

   return( 0 );
} // zwTZOPUPDD_MoveOperationCancel

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZOPUPDD_ReportExitInitialize
//
//    Temporary until ReportExit is supported as an Operation type.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZOPUPDD_ReportExitInitialize( zVIEW vSubtask )
{
   zVIEW    vOperGrp;
   zSHORT   nRC;

   GetViewByName( &vOperGrp, "TZOGSRCO", vSubtask, zLEVEL_ANY );

   // Clear all existing parameters for operation
   for ( nRC = SetCursorFirstEntity( vOperGrp, "Parameter", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vOperGrp, "Parameter", 0 ) )
   {
      DeleteEntity( vOperGrp, "Parameter", zREPOS_NONE );
   }

   SetAttributeFromString( vOperGrp, "Operation", "ReturnDataType", "N" );

   CreateMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vOperGrp, "Parameter", "ShortDesc", "vSubtask" );
   SetAttributeFromString( vOperGrp, "Parameter", "DataType", "V" );
   SetAttributeFromString( vOperGrp, "Parameter", "PFlag", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "UFlag", "N" );

   CreateMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vOperGrp, "Parameter", "ShortDesc", "vDrivingOI" );
   SetAttributeFromString( vOperGrp, "Parameter", "DataType", "V" );
   SetAttributeFromString( vOperGrp, "Parameter", "PFlag", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "UFlag", "N" );

   CreateMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vOperGrp, "Parameter", "ShortDesc", "GrpSetName" );
   SetAttributeFromString( vOperGrp, "Parameter", "DataType", "S" );
   SetAttributeFromString( vOperGrp, "Parameter", "PFlag", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "UFlag", "N" );

   CreateMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vOperGrp, "Parameter", "ShortDesc", "GroupName" );
   SetAttributeFromString( vOperGrp, "Parameter", "DataType", "S" );
   SetAttributeFromString( vOperGrp, "Parameter", "PFlag", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "UFlag", "N" );

   CreateMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vOperGrp, "Parameter", "ShortDesc", "nEvent" );
   SetAttributeFromString( vOperGrp, "Parameter", "DataType", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "PFlag", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "UFlag", "N" );

   CreateMetaEntity( vSubtask, vOperGrp, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vOperGrp, "Parameter", "ShortDesc", "nCurrentPage" );
   SetAttributeFromString( vOperGrp, "Parameter", "DataType", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "PFlag", "N" );
   SetAttributeFromString( vOperGrp, "Parameter", "UFlag", "N" );

   SetCursorFirstEntity( vOperGrp, "Parameter", 0 );

   return( 0 );
} // zwTZOPUPDD_ReportExitInitialize

#ifdef __cplusplus
}
#endif
