/////////////////////////////////////////////////////////////////////////////
//
// File:            TZAXCFGD.C
// Copyright:       Ton Beller AG2001
// Autor:           TMV
// Datum:           9. Februar 2001
// describtion:
//
//
/* Change log most recent first order:
   2001.06.10  Bl    R56775
      If create a new Mapping und canceled it, the Cursor is undefined.
      Send a refresh to Window, then the Cursor is not undefined.
   2001.05.30  BL    ActiveX
      Modified zwTZAXCFGD_UpdateSubControl for do not replace Grid Window
   2001.3.13   TMV   ActiveX
      remove all ValueItems of a column prior to create new
      fix bug for refreshing Radiobuttons
   2001.03.05   BL    ActiveX
      Modified zwTZAXCFGD_OK for set Data Mode and WritePersistProperties
      during close of window 'tzaxcfgd'
   2001.03.05  BL    ActiveX
      Added zwTZAXCFGD_DeleteSubControl and zwfnTZAXCFGD_DisableColButtons
      for delete Mapping
      Bugfix Domain Combobox Mapping
      GetViewByName: for parameter zLEVEL_TASK changed View --> 0
   2001.2.28   BL    ActiveX
      1. Delete unused Operations
      2. Rename all Operations
      3. If Dialog not checked out, disable all Controls for update
   2001.2.26   TMV   2000   ActiveX
      Remove fnPainterCall from UPD_OK call
   2001.2.26   TMV   2001   ActiveX
      never use DropObjectInstance within Painter
   2001.2.20   TMV   2000   ActiveX
      removed obsolete includes
   2001.2.9   TMV   2000   ActiveX
      Created
*/
/////////////////////////////////////////////////////////////////////////////
#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS

#include "ax\truegrid\tdbg6.h"

#include "..\zdr\TzCtl.h"
#include "tz__oprs.h"
#include "tzlodopr.h"


#include "tzctlopr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static zCHAR THIS_FILE[] = __FILE__;
#endif

#define TZAXWRKO_DEFAULT_STRING_SIZE   256

// ppe control type identification constants
const static zLONG CONTROL_TYPE_COMBOBOX = 32001;
const static zLONG CONTROL_TYPE_CHECKBOX = 32002;
const static zLONG CONTROL_TYPE_EDIT     = 32003;
const static zLONG CONTROL_TYPE_RADIO    = 32004;
const static zLONG CONTROL_TYPE_RADIOGRP = 32005;

// View name constants
const static zPCHAR pszTZCONTROL = "TZCONTROL";
const static zPCHAR pszTZPNCTWO  = "TZPNCTWO";
const static zPCHAR pszTZGRIDSUBCONTROL = "TZTRUEGRID_SUBCONTROL";
const static zPCHAR pszTZTRUEGRID_SCOPE = "TZTRUEGRID_SCOPE";
const static zPCHAR pszTZTRUEGRID_LIST  = "TZTRUEGRID_LIST";

// Entity name constants
const static zPCHAR pszRadio     = "Radio";
const static zPCHAR pszControl   = "Control";
const static zPCHAR pszCtrlMap   = "CtrlMap";
const static zPCHAR pszCtrlMapView   = "CtrlMapView";
const static zPCHAR pszCtrlMapRelatedEntity   = "CtrlMapRelatedEntity";
const static zPCHAR pszCtrlMapER_Attribute   = "CtrlMapER_Attribute";
const static zPCHAR pszCtrlMapER_Domain   = "CtrlMapER_Domain";
const static zPCHAR pszCtrlMapLOD_Entity = "CtrlMapLOD_Entity";
const static zPCHAR pszCtrlMapContext    = "CtrlMapContext";

// Attribute name constants
const static zPCHAR pszCtrlBOI   = "CtrlBOI";
const static zPCHAR pszCtrlCtrl  = "CtrlCtrl";
const static zPCHAR pszType      = "Type";
const static zPCHAR pszSubType   = "Subtype";
const static zPCHAR pszText      = "Text";
const static zPCHAR pszName      = "Name";
const static zPCHAR pszTag       = "Tag";
const static zPCHAR pszEvent     = "Event";

// Dialog name constants
const static zPCHAR pszTZAXCFGD      = "TZAXCFGD";
const static zPCHAR pszUPD_COMBO     = "UPD_COMBO";
const static zPCHAR pszUPD_CHECK     = "UPD_CHECK";
const static zPCHAR pszUPD_TEXT      = "UPD_TEXT";
const static zPCHAR pszUPD_RADIOGRP  = "UPD_RADIOGRP";
const static zPCHAR pszUPD_RADIO     = "UPD_RADIO";
const static zPCHAR pszNewColumn     = "NewColumn";

// defined Attribute value constants
const static zPCHAR pszScope      = "Scope";
const static zPCHAR pszList       = "List";

#define zStorageDataMode         4

#ifdef __cplusplus
extern "C"
{
#endif
#include "ZEIDONOP.H"


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   Operation prototypes
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_NewColumn_OnOk( zVIEW vSubtask );

zSHORT OPERATION ReCreateHelperViews( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetupGridColumnFromControlView( zVIEW vSubtask, Column &pCol, ZColumns& cols );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetupGridCombo( zVIEW vSubtask, Column &Col, ZColumns& cols, ZColComboBox* pComboBoxProps );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetupGridRadioGroup( zVIEW vSubtask, Column &Col, ZColumns& cols, ZColRadioGrp* pRadioGrpProps );

zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_CtrlContextMappInit( zVIEW vSubtask );

static void
zwfnTZAXCFGD_SetUpSEL_LOD( zVIEW vDialogC,
                           zVIEW vDialogW,
                           zVIEW vSubtask,
                           zCPCHAR szMapView );
static void
zwfnTZAXCFGD_SetUpScopeListOI( zVIEW vDialogC,
                               zVIEW vSubtask,
                               zCPCHAR szMapLOD_Entity );
static void
zwfnTZAXCFGD_SearchForEntityRecursively( zVIEW vLOD,
                                         zLONG lZKey,
                                         zPSHORT pFoundFlag );
static void
zwfnTZAXCFGD_SetUpEventList( zVIEW vDialog );

static void
zwfnTZAXCFGD_PUSH_Init( zVIEW vSubtask );

static void
zwfnTZAXCFGD_DisableButtons( zVIEW  vSubtask,
                             zPCHAR szActionNameForAction,
                             zPCHAR szActionNameForEdit );

static void
zwfnTZAXCFGD_LoadEventPopups( zVIEW  vSubtask,
                              zPCHAR szActionNameForUpdate,
                              zPCHAR szActionNameForNew,
                              zPCHAR szActionNameForSelect,
                              zPCHAR szActionNameForRemove,
                              zPCHAR szPopupName );
static void
zwfnTZAXCFGD_DisableColButtons( zVIEW vSubtask );


/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwfnTZAXCFGD_SearchForEntityRecursively
// PURPOSE:  This function searches the LOD object recursively on
//           LOD_EntityParent, looking for a match on ZKey.  When it
//           finds it, it saves the view under view name ?????
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZAXCFGD_SearchForEntityRecursively( zVIEW vLOD,
                                         zLONG lZKey,
                                         zPSHORT pFoundFlag )
{
   if ( CheckExistenceOfEntity( vLOD, "LOD_EntityChild" ) == zCURSOR_SET )
   {
      do
      {
         if ( CompareAttributeToInteger( vLOD, "LOD_EntityChild",
                                         "ZKey", lZKey ) != 0 )
         {
            SetViewToSubobject( vLOD, "LOD_EntityChild" );
            zwfnTZAXCFGD_SearchForEntityRecursively( vLOD, lZKey, pFoundFlag );
            if ( *pFoundFlag == 0 )
               ResetViewFromSubobject( vLOD );
         }
      }
      while ( (CompareAttributeToInteger( vLOD, "LOD_EntityChild",
                                          "ZKey", lZKey ) != 0) &&
            (*pFoundFlag == 0) &&
            (SetCursorNextEntity( vLOD,
                                   "LOD_EntityChild", "" ) == zCURSOR_SET) );

      if ( CompareAttributeToInteger( vLOD, "LOD_EntityChild",
                                      "ZKey", lZKey ) == 0 )
         *pFoundFlag = 1;    // Indicate a match was found
   }
}

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwfnTZAXCFGD_SetUpScopeListOI
// PURPOSE:  This function sets up the Scoping List OI for use by
//           a Combo Box for listing potential Scoping entities.
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZAXCFGD_SetUpScopeListOI( zVIEW vDialogC,
                               zVIEW vSubtask,
                               zCPCHAR szMapLOD_Entity )
{
   zVIEW    vLOD;
   zVIEW    vScopeL;
   zLONG    lZKey;
   zSHORT   FoundFlag;

   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   ActivateEmptyObjectInstance( &vScopeL, "TZPNCTWO", vSubtask, 0 );
   SetNameForView( vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   CreateEntity( vScopeL, "TZPNCTWO", zPOS_BEFORE );

   // szMapLOD_Entity is CtrlMapView, if this was called for Control
   // mapping, or  ActMapLOD_Entity, if this was called for Action mapping.
   // If szMapLOD_Entity exists, set up the list to reflect that
   // entity value.  Begin looking recursively through all entities
   // if the root entity is not the one mapped.
   if ( CheckExistenceOfEntity( vDialogC, szMapLOD_Entity ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDialogC, szMapLOD_Entity, "ZKey" );
      if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent",
                                      "ZKey", lZKey ) != 0 )
      {
         FoundFlag = 0;
         zwfnTZAXCFGD_SearchForEntityRecursively( vLOD, lZKey, &FoundFlag );
         do
         {
            CreateEntity( vScopeL, "ScopeEntity", zPOS_BEFORE );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity", "Name",
                                       vLOD, "LOD_EntityParent", "Name" );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity",
                                       "LOD_EntityZKey", vLOD,
                                       "LOD_EntityParent", "ZKey" );

         } while ( ResetViewFromSubobject( vLOD ) == 0 );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwfnTZAXCFGD_SetUpSEL_LOD
// PURPOSE:  This function sets up the SEL_LOD OI for Entity or
//           Attribute mapping.  If a current CtrlMapView entity
//           instance exists, it positions on the related ViewObjRef
//           entity and activates the corresponding LOD.  Otherwise
//           it activates an empty OI.
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZAXCFGD_SetUpSEL_LOD( zVIEW vDialogC,
                           zVIEW vDialogW,
                           zVIEW vSubtask,
                           zCPCHAR szMapView )
{
   zVIEW vLOD = (zVIEW) SetUpMapLOD( vSubtask, vDialogW,
                                     vDialogC, (zPCHAR) szMapView );
   if ( vLOD == 0 )
      ActivateEmptyObjectInstance( &vLOD, "TZZOLODO", vSubtask, 0 );

   SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
}

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_CtrlContextMappInit
// PURPOSE:  This function builds the MapDomain View for the Context
//           Combo Box, if there is current mapping.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_CtrlContextMappInit( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   GetViewByName( &vDialog, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );

   if ( CheckExistenceOfEntity( vDialog,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialog, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   }
   else
      ActivateEmptyObjectInstance( &vDomain, "TZDGSRCO", vSubtask, 0 );

   SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   return( 0 );
}

TZActiveX *GetCurrentActiveX( zVIEW vSubtask )
{
   TZActiveX *pAtx = 0;

   fnPainterCall( zMSG_GETCURRCTRLWND, vSubtask, vSubtask, (zPCHAR) &pAtx, 0 );

   return( pAtx );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZAXCFGD_DisableButtons
//
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZAXCFGD_DisableButtons( zVIEW  vSubtask,
                             zPCHAR szActionNameForAction,
                             zPCHAR szActionNameForEdit )
{
   zVIEW vEvents;
   zVIEW vDialogA;
   zVIEW vTZWINDOWL;
   zBOOL bEnableAction    = FALSE;
   zBOOL bEnableEdit      = FALSE;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vEvents, pszEvent ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
         {
            bEnableAction = TRUE;

            if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) > 0 &&
                 CheckExistenceOfEntity( vDialogA, "ActOper" ) >= zCURSOR_SET)
            {
               bEnableEdit = TRUE;
            }
         }
      }

      EnableAction( vSubtask, szActionNameForEdit, bEnableEdit );
      EnableAction( vSubtask, szActionNameForAction, bEnableAction );
   }

} // zwfnTZAXCFGD_DisableButtons

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwfnTZAXCFGD_LoadEventPopups
//
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZAXCFGD_LoadEventPopups( zVIEW  vSubtask,
                              zPCHAR szActionNameForUpdate,
                              zPCHAR szActionNameForNew,
                              zPCHAR szActionNameForSelect,
                              zPCHAR szActionNameForRemove,
                              zPCHAR szPopupName )
{
   zVIEW  vEvents;
   zVIEW  vDialog;
   zVIEW  vTZWINDOWL;
   zBOOL  bNew          = FALSE;
   zBOOL  bUpdate       = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META );

      if ( CheckExistenceOfEntity( vEvents, pszEvent ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
         {
            bUpdate = TRUE;
            if ( nIsCheckedOut == 1 )
               bDelete = TRUE;
         }
      }

      if ( CheckExistenceOfEntity( vEvents, pszEvent ) >= zCURSOR_SET &&
           nIsCheckedOut == 1 )
      {
         bNew = TRUE;
      }

      EnableAction( vSubtask, szActionNameForUpdate, bUpdate );
      EnableAction( vSubtask, szActionNameForNew, bNew );
      EnableAction( vSubtask, szActionNameForSelect, bNew );
      EnableAction( vSubtask, szActionNameForRemove, bDelete );

      CreateTrackingPopupMenu( vSubtask, szPopupName, -1, 0, FALSE, FALSE );
   }

} // zwfnTZAXCFGD_LoadEventPopups


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwfnTZAXCFGD_SetUpEventList
//
// PURPOSE:  This is an internal reusable function to set up the
//           Event entities in TZPNEVWO from the existing Events in
//           TZCONTROL.
//
/////////////////////////////////////////////////////////////////////////////
static void
zwfnTZAXCFGD_SetUpEventList( zVIEW vSubtask )
{
   zVIEW    vEvents;
   zVIEW    vEventsCopy;
   zLONG    lType;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Only perform loop if Events exist.
   if ( CheckExistenceOfEntity( vEvents, pszEvent ) >= zCURSOR_SET )
   {
      // Loop through all vEvents.Event and set the EventAct.Tag value
      // to vSubtask.EventAct.Tag, if the relationship exists, and to
      // NULL otherwise.
      CreateViewFromViewForTask( &vEventsCopy, vEvents, 0 );
      SetCursorFirstEntity( vEventsCopy, pszEvent, 0 );
      do
      {
         GetIntegerFromAttribute( &lType, vEventsCopy, pszEvent, pszType );
         if ( SetCursorFirstEntityByInteger( vSubtask, pszEvent, pszType,
                                             lType, 0 ) == zCURSOR_SET )
         {
            SetAttributeFromAttribute( vEventsCopy, "EventAct", "Tag",
                                       vSubtask, "EventAct", "Tag" );
         }
         else
         if ( CheckExistenceOfEntity( vEventsCopy, "EventAct" ) == 0 )
         {
            SetAttributeFromString( vEventsCopy, "EventAct", "Tag", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", pszType, "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "DialogName", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "WindowName", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "Operation", "" );
            SetAttributeFromString( vEventsCopy, "EventAct", "SB_SubAction", "" );
         }

      } while ( SetCursorNextEntity( vEventsCopy, pszEvent, 0 ) >
                                                      zCURSOR_UNCHANGED );
      DropView( vEventsCopy );
   }
} // zwfnTZAXCFGD_SetUpEventList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_LoadActionPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_LoadActionPopup( zVIEW vSubtask )
{
   zwfnTZAXCFGD_LoadEventPopups( vSubtask,
                                 "SelectUpdate",
                                 "NewAction",
                                 "SelectAction",
                                 "RemoveAction",
                                 "UPD_ActionPopup" );


   return( 0 );
} // zwTZAXCFGD_LoadActionPopup

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_Init
// PURPOSE:  This function sets up the TZACTION view for the
//           UPD_PUSH window.  It is the same as TZWINDOW if a current
//           EventAct exits.  It is null otherwise.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_Init( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vSubCtrl  = NULL;
   zVIEW    vMapScope = NULL;
   zVIEW    vMapList  = NULL;
   zBOOL    bIsCheckedOut = TRUE;

   GetViewByName( &vDialogW,  "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC,  pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vSubCtrl,  pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapScope, pszTZTRUEGRID_SCOPE, vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapList,  pszTZTRUEGRID_LIST,  vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ))
      bIsCheckedOut = FALSE;

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vDialogC, pszControl );
   if ( vSubCtrl )
   {
      DropView ( vSubCtrl );
   }

   if ( vMapScope )
   {
      DropView ( vMapScope );
   }

   if ( vMapList )
   {
      DropView ( vMapList );
   }

   // create view to display column details
   CreateViewFromViewForTask( &vSubCtrl, vDialogC, 0 );
   SetNameForView( vSubCtrl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );
   SetViewToSubobject( vSubCtrl, pszCtrlCtrl );

   // create view to display scope Mapping
   CreateViewFromViewForTask( &vMapScope, vDialogC, 0 );
   SetNameForView( vMapScope, pszTZTRUEGRID_SCOPE, vSubtask, zLEVEL_TASK );
   // look for a specific mapping named "Scope", if it does not exist create it
   if ( bIsCheckedOut &&
         SetCursorFirstEntityByString( vMapScope, pszCtrlMap, pszTag, pszScope,
                                       "" ) < zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vMapScope, pszCtrlMap, zPOS_FIRST );
      SetAttributeFromString( vMapScope, pszCtrlMap, pszTag, pszScope );
   }

   // create view to display List Mapping
   CreateViewFromViewForTask( &vMapList, vDialogC, 0 );
   SetNameForView( vMapList, pszTZTRUEGRID_LIST, vSubtask, zLEVEL_TASK );
   // look for a specific mapping named "List", if it does not exist create it
   if ( bIsCheckedOut &&
        SetCursorFirstEntityByString( vMapList, pszCtrlMap, pszTag, pszList,
                                      "" ) < zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vMapList, pszCtrlMap, zPOS_LAST );
      SetAttributeFromString( vMapList, pszCtrlMap, pszTag, pszList );
   }

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                       vDialogC, "EventAct", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   return( 0 );
} // zwTZAXCFGD_Init

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_SetActionView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_SetActionView( zVIEW vSubtask )
{

   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZPNEVWO
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // If not, set TZACTION to empty instance for auto graying.
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   zwfnTZAXCFGD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   return( 0 );
} // zwTZAXCFGD_SetActionView



/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_ActionUpdate
//
// PURPOSE:  Select or Update the Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_ActionUpdate( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZPNEVWO
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vDialogW, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );

      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
      CreateTemporalSubobjectVersion( vDialogW, "Action" );
   }
   else
   {
      MessageSend( vSubtask, "PN00103", "Dialog",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_SelectAction( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vEvents;
   zVIEW    vWork;
   zLONG    lType;

   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   while ( ResetViewFromSubobject( vDialogW ) == 0 );

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, pszEvent, "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00108", "Dialog",
                   "No Event selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   // Error if Event already exists.  Otherwise add it.
   GetIntegerFromAttribute( &lType, vEvents, pszEvent, pszType );
   if ( SetCursorFirstEntityByInteger( vDialogC, pszEvent, pszType,
                                       lType, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00117", "Dialog Maintenance",
                   "Event already has an Action, \nFirst remove the Action and then select a new one.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }
   else
   {
      CreateTemporalMetaEntity( vSubtask, vDialogC, pszEvent, zPOS_AFTER );
      SetAttributeFromInteger( vDialogC, pszEvent, pszType, lType );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
} // zwTZAXCFGD_SelectAction


static void
zwfnTZAXCFGD_DisableColButtons( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vControlC;
   zVIEW  vTZWINDOWL;
   zBOOL  bNew          = FALSE;
   zBOOL  bUpdate       = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vControl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META );

      CreateViewFromViewForTask( &vControlC, vControl, 0 );
      if ( SetCursorFirstEntity( vControlC, pszControl, "" ) >= zCURSOR_SET )
      {
         bUpdate = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      DropView( vControlC );

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "UpdateSubControl", bUpdate );
      EnableAction( vSubtask, "NewSubControl", bNew );
      EnableAction( vSubtask, "DeleteSubcontrol", bDelete );
   }
}



zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;
   zVIEW  vTZPNEVWO;
   zCHAR  szEventTag[ 33 ];

   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

   ReCreateHelperViews( vSubtask );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      if ( vDialogA )
         DropView( vDialogA );

      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                       vDialogW, "Action", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

      // save cursor
      GetViewByName( &vTZPNEVWO, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEventTag, sizeof( szEventTag ), vTZPNEVWO, pszEvent, "Tag" );

      // Build the TZPNEVWO object for the control events.
      fnPainterCall( zMSG_BUILDTZPNEVWO, vSubtask, 0, 0, 0 );

      // set cursor
      GetViewByName( &vTZPNEVWO, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntityByString( vTZPNEVWO, pszEvent, "Tag", szEventTag, "" );
      RefreshCtrl( vSubtask, "EventAction" );
   }

   if ( CheckExistenceOfEntity( vDialogC, pszCtrlCtrl ) == zCURSOR_SET )
   {
      RefreshCtrl( vSubtask, "ListSubControls" );
   }

   zwfnTZAXCFGD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );
   zwfnTZAXCFGD_DisableColButtons( vSubtask );

   return( 0 );
} // zwTZAXCFGD_ReturnFromSubwindow

zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_ActionRemove( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vEvents;
   zLONG    lType;

   GetViewByName( &vDialog, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lType, vEvents, pszEvent, pszType );
   if ( SetCursorFirstEntityByInteger( vDialog, pszEvent, pszType,
                                       lType, 0 ) == zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDialog, "EventAct" ) == zCURSOR_SET )
         ExcludeEntity( vDialog, "EventAct", zREPOS_PREV );

      DeleteEntity( vDialog, pszEvent, zREPOS_PREV );
   }

   zwfnTZAXCFGD_SetUpEventList( vDialog );
   fnCreateEmptyActionView( vSubtask, vDialog );

   RefreshCtrl( vSubtask, "EventAction" );
   zwfnTZAXCFGD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

    return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;
   zVIEW  vControl;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   zwfnTZAXCFGD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "Properties", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "pbNew", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "pbDelete", zCONTROL_STATUS_ENABLED, FALSE );
   }

   if ( CheckExistenceOfEntity( vControl, pszCtrlMap ) < zCURSOR_SET )
   {
      EnableAction( vSubtask, "DeleteSubcontrol", FALSE );
   }

   return( 0 );
} // zwTZAXCFGD_Postbuild


zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_ActionNew( zVIEW vSubtask )
{
   zVIEW  vDialogW;
   zVIEW  vDialogC;
   zVIEW  vEvents;
   zVIEW  vWork;
   zLONG  lType;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, pszEvent, "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00108", "Dialog",
                   "No Event selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( 0 );
   }

   // Error if Event already exists.  Otherwise add it.
   GetIntegerFromAttribute( &lType, vEvents, pszEvent, pszType );
   if ( SetCursorFirstEntityByInteger( vDialogC, pszEvent, pszType,
                                       lType, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00117", "Dialog Maintenance",
                   "Event already has an Action, \nFirst remove the Action and then select a new one.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }
   else
   {
      CreateTemporalMetaEntity( vSubtask, vDialogC, pszEvent, zPOS_AFTER );
      SetAttributeFromInteger( vDialogC, pszEvent, pszType, lType );
   }

//   ActionSelect( vSubtask );

   // LIST_ACT_AddAction( vSubtask );
   if ( CheckExistenceOfEntity( vDialogW, "Action" ) >= zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );

   CreateTemporalMetaEntity( vSubtask, vDialogW, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialogW, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialogW, "Action", pszType, 1 );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_OperationEdit( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zCHAR  szTag[ 34 ];

   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET &&
        CompareAttributeToString( vDialogC, "EventAct", "Tag", "" ) != 0 )
   {
      GetStringFromAttribute( szTag, sizeof( szTag ), vDialogC, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vDialogW, "Action", "Tag", szTag, "" );
      if ( CheckExistenceOfEntity( vDialogW, "Operation" ) == zCURSOR_SET &&
           CheckExistenceOfEntity( vDialogW, "ActOper" ) == zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vDialogW, "Operation", "Name",
                                     vDialogW, "ActOper", "Name", "Dialog" );
      }

      if ( CheckExistenceOfEntity( vDialogW, "ActOper" ) == zCURSOR_SET )
      {
         TransferToEditor( vSubtask, vDialogW, "PN", 0 );
      }
      else
      {
         MessageSend( vSubtask, "PN00109", "Dialog",
                      "No Operation exists for Action.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      }
   }
   else
   {
      MessageSend( vSubtask, "PN00110", "Dialog",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
} // zwTZAXCFGD_OperationEdit

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_LoadColumnPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_LoadColumnPopup( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTZWINDOWL;
   zBOOL  bNew          = FALSE;
   zBOOL  bUpdate       = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vControl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META );

      if ( CheckExistenceOfEntity( vControl, pszControl ) >= zCURSOR_SET )
      {
         bUpdate = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "UpdateSubControl", bUpdate );
      EnableAction( vSubtask, "NewSubControl", bNew );
      EnableAction( vSubtask, "DeleteSubcontrol", bDelete );

      CreateTrackingPopupMenu( vSubtask, "UPD_ColumnPopup", -1, 0, FALSE, FALSE );
   }

   return( 0 );
} // zwTZAXCFGD_LoadColumnPopup

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_OK
// PURPOSE:  This function tells the Painter to refresh the control.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_OK( zVIEW vSubtask )
{
   TZActiveX *pAtx;
   zVIEW     vControl;
   zCHAR     szTag[ 255 ];
   zCHAR     szText[ 255 ];

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szTag, sizeof( szTag ), vControl, pszControl, pszTag );
   GetStringFromAttribute( szText, sizeof( szText ), vControl, pszControl, pszText );

   pAtx = GetCurrentActiveX( vSubtask );
   if ( pAtx )
   {
      // Set DataMode = storage
      ITrueDBGridCtrl_SetDataMode( vSubtask, szTag, zStorageDataMode );
      pAtx->WritePersistProperties();

      // check the number of columns
      zSHORT nZeidonColCnt = (zSHORT) CountEntitiesForView( vControl, pszCtrlCtrl );
      zLONG lAtxColCnt = 0;
      LPDISPATCH lpGridDisp = NULL;

      if ( pAtx && nZeidonColCnt > 0 )
      {
         // get dispatch interface, create the columns object and create missing columns
         //
         LPUNKNOWN pUnk = pAtx->GetControlUnknown();
         pUnk->QueryInterface( IID_IDispatch, (LPVOID *)&lpGridDisp);
         if ( lpGridDisp )
         {
            CMemFile f;
            CArchive ar( &f, CArchive::store );
            ZColumns zCols;
            SetViewToSubobject( vControl, pszCtrlCtrl );
            zSHORT nRC = SetCursorFirstEntity( vControl, pszControl, "" );
            ITrueDBGridCtrl * pGridInstance = new ITrueDBGridCtrl( NULL, lpGridDisp );
            CString strGridCaption = pGridInstance->GetCaption();
            if ( strGridCaption.IsEmpty() )
               pGridInstance->SetCaption( szText );

            LPDISPATCH lpDispColumns = pGridInstance->GetColumns( );
            if ( lpDispColumns )
            {
               Columns cols ( lpDispColumns );
               lAtxColCnt = cols.GetCount();
               zSHORT i = 0;
               while( i < nZeidonColCnt && nRC > zCURSOR_UNCHANGED )
               {
                  LPDISPATCH lpColumnDispatch = NULL;
                  if ( i < lAtxColCnt )
                  {
                     COleVariant v( i );
                     lpColumnDispatch = cols.GetItem( v );
                  }
                  else
                  {
                     lpColumnDispatch = cols.Add( i );
                  }
                  Column col( lpColumnDispatch );

                  SetupGridColumnFromControlView( vSubtask, col, zCols );

                  nRC = SetCursorNextEntity( vControl, pszControl, "" );
                  i++;
               }
               cols.DetachDispatch();
            }
            ResetViewFromSubobject( vControl );
            if ( zCols.GetCount() > 0 )
            {
               zCols.Serialize( ar );
               ar.Close();
               zLONG  lBuffSize = (zLONG) f.GetLength( );
               LPBYTE pBuff = f.Detach();
               SetAttributeFromBlob( vControl, pszControl, pszCtrlBOI, (LPVOID)pBuff, lBuffSize );
               f.Attach( pBuff, lBuffSize ); //mem is freed when memfile is destroyed
            }
         }
      }
   }

   // Accept control subobject.
   AcceptSubobject( vControl, pszControl );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
} // zwTZAXCFGD_OK

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetupGridColumnFromControlView( zVIEW vSubtask, Column &Col, ZColumns& cols )
{
   zSHORT nRC = 0;
   zVIEW vControl = 0;
   zVIEW vSubControl = 0;
   zVIEW vWork = 0;
   zLONG lControlType = 0;
   zCHAR szHelp[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szView[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szEntity[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szAttribute[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szContext[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   CString strDataField;
   zPCHAR  pCtrlBOI;
   zPCHAR  pszCHECK = NULL;
   zPCHAR  pszUNCHECK = NULL;
   zPCHAR  pszRadioBtnValue = NULL;
   zLONG   CurrentSubtype  = 0;
   zLONG   lSubtype        = 0;
   zLONG   lFormatType     = 0;
   ValueItems ListOfValues;
   ZColEdit *pColEdit;
   ZColCheckBox *pZColCheckBox;
   ZColRadioGrp *pZColRadioGrp;
   ZColComboBox *pZColComboBox;
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vControl, pszCtrlMap, "" );

   if ( nRC >= zCURSOR_UNCHANGED )
   {
      szView[ 0 ]      = 0;
      szEntity[ 0 ]    = 0;
      szAttribute[ 0 ] = 0;
      szContext[ 0 ]   = 0;
       // set common props
      if ( CheckExistenceOfEntity( vControl, pszCtrlMapView ) == zCURSOR_SET )
      {
         // Get the view, entity, attribute name  and context to create edit and listbox mapping
         GetStringFromAttribute( szView, sizeof( szView ), vControl, pszCtrlMapView, pszName );
         GetStringFromAttribute( szEntity, sizeof( szEntity ), vControl, pszCtrlMapRelatedEntity, pszName );
         GetStringFromAttribute( szAttribute, sizeof( szAttribute ), vControl, pszCtrlMapER_Attribute, pszName );
         if ( CheckExistenceOfEntity( vControl, pszCtrlMapContext ) == zCURSOR_SET )
         {
            GetStringFromAttribute( szContext, sizeof( szContext ), vControl, pszCtrlMapContext, pszName );
         }
      }
      //try to setup unique name for datafield property of the column
      //if datafield property is not unique the columns having the same datafield name
      //will display the same values
      strDataField.Empty();
      if ( zstrlen ( szView ) )
         strDataField = szView;
      if ( zstrlen ( szEntity ) )
      {
         if ( strDataField.GetLength() )
            strDataField += "_";
         strDataField += szEntity;
      }
      if ( zstrlen ( szAttribute ) )
      {
         if ( strDataField.GetLength() )
            strDataField += "_";
         strDataField += szAttribute;
      }
      if ( zstrlen ( szContext ) )
      {
         if ( strDataField.GetLength() )
            strDataField += "_";
         strDataField += szContext;
      }
      Col.SetDataField( strDataField );

      GetStringFromAttribute( szHelp, sizeof( szHelp ), vControl, pszControl, pszText );
      Col.SetCaption( szHelp );

      GetIntegerFromAttribute( &lControlType, vControl, pszControl, pszType );
      ListOfValues = Col.GetListOfValues();
      switch( lControlType )
      {
         case CONTROL_TYPE_EDIT:
            pColEdit = new ZColEdit();
            pColEdit->SetMapping( szView, szEntity, szAttribute, szContext );
            ListOfValues.SetPresentation( dbgNormal );
            cols.AddTail( pColEdit );
            break;
         case CONTROL_TYPE_CHECKBOX:
            pZColCheckBox = new ZColCheckBox();
            pZColCheckBox->SetMapping( szView, szEntity, szAttribute, szContext );
            ListOfValues.SetPresentation( dbgCheckBox );
            // Set CheckBox attribute values from Control.CtrlBOI
            GetAddrForAttribute( &pCtrlBOI, vControl, pszControl, pszCtrlBOI );
            if ( pCtrlBOI )
            {
               pszCHECK = pCtrlBOI;
               while ( pCtrlBOI[ 0 ] ) pCtrlBOI++;// values are separated by 0

               pszUNCHECK = pCtrlBOI + 1;
            }
            pZColCheckBox->SetCheckboxTranslationValues( pszCHECK, pszUNCHECK );
            cols.AddTail( pZColCheckBox );
            break;
         case CONTROL_TYPE_RADIOGRP:
            pZColRadioGrp = new ZColRadioGrp();
            ListOfValues.SetPresentation( dbgRadioButton );
            ListOfValues.SetTranslate( TRUE );
            ListOfValues.SetValidate( TRUE );

            pZColRadioGrp->SetMapping( szView, szEntity, szAttribute, szContext );
            SetupGridRadioGroup( vSubtask, Col, cols, pZColRadioGrp );
            cols.AddTail( pZColRadioGrp );
            break;
         case CONTROL_TYPE_COMBOBOX:
            pZColComboBox = new ZColComboBox();
            ListOfValues.SetPresentation( dbgComboBox );
            pZColComboBox->SetMapping( szView, szEntity, szAttribute, szContext );
            SetupGridCombo( vSubtask, Col, cols, pZColComboBox );
            cols.AddTail( pZColComboBox );
            break;
         default:
            break;
      }
   }

   return( nRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetupGridCombo( zVIEW vSubtask, Column &Col, ZColumns& cols, ZColComboBox* pComboBoxProps )
{
   zSHORT nRC = 0;
   zVIEW vControl = 0;
   zVIEW vWork = 0;
   zBOOL bExitLoop = FALSE;
   zCHAR szViewName[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szEntityName[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szAttributeName[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zCHAR szContextName[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zLONG   lMappingError  = 0;
   zPULONG pulSubtype;
   zULONG  ulSubtype        = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( (zCOREMEM) &pulSubtype, vControl, pszControl, pszSubType );

   ulSubtype = (*pulSubtype );
   pComboBoxProps->SetFormatType( ulSubtype );

   // there must be five CtrlMap entities ... enforced by the painter
   nRC = SetCursorFirstEntity( vControl, pszCtrlMap, 0 );
   int k = 0;
   while ( nRC > zCURSOR_UNCHANGED && !bExitLoop )
   {
      k++;
      szViewName[ 0 ] = 0;    // init all buffers
      szEntityName[ 0 ] = 0;
      szAttributeName[ 0 ] = 0;
      szContextName[ 0 ] = 0;
      // if combobox is a domain list, we only need the first mapping
      // otherwise there are up to five CtrlMap entities
      switch( k )
      {
         case 1:
            if ( ulSubtype & zCOMBOBOX_DOMAIN_LIST )
               bExitLoop = TRUE;
            // no break at this point continue with case 2
         case 2:
            if ( CheckExistenceOfEntity( vControl, pszCtrlMapView ) == zCURSOR_SET )
            {
               // Get the view, entity, attribute name  and context to create edit and listbox mapping
               GetStringFromAttribute( szViewName, sizeof( szViewName ), vControl, pszCtrlMapView, pszName );
               GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vControl, pszCtrlMapRelatedEntity, pszName );
               GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ), vControl, pszCtrlMapER_Attribute, pszName );
               if ( CheckExistenceOfEntity( vControl, pszCtrlMapContext ) == zCURSOR_SET )
               {
                  GetStringFromAttribute( szContextName, sizeof( szContextName ), vControl, pszCtrlMapContext, pszName );
               }
               // first is Edit mapping the second is list mapping
               if ( k == 1 )
                  pComboBoxProps->SetMapping( szViewName, szEntityName, szAttributeName, szContextName );
               else
                  pComboBoxProps->SetListMapping(szViewName, szEntityName, szAttributeName, szContextName);
            }
            else
               lMappingError += k;
            break;
         case 3:
         case 4:
            // here we get browse and scope informations
            if ( CheckExistenceOfEntity( vControl, pszCtrlMapLOD_Entity ) == 0 )
            {
               GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vControl, pszCtrlMapLOD_Entity, pszName );
               if ( k == 3 )
                  pComboBoxProps->SetBrowseEntity( szEntityName );
               else
                  pComboBoxProps->SetBrowseScopeEntity( szEntityName );
            }
            else // defaulting to list mapping
            {
               ZColumnMapping *cm = pComboBoxProps->GetListMapping();
               if ( k == 3 )
                  pComboBoxProps->SetBrowseEntity( cm->GetMapEntityName() );

            }
            break;
         case 5:
               // If the last CtrlMap has a view name then this combo box is
               // only going to display selected values. Keep this CtrlMap
               // in the Target, but all the rest will be deleted.
               if ( CheckExistenceOfEntity( vControl, pszCtrlMapView ) == zCURSOR_SET )
               {
                  GetStringFromAttribute( szViewName, sizeof( szViewName ), vControl, pszCtrlMapView, pszName );
                  pComboBoxProps->SetOnlySelectedView( szViewName );
               }
               else
               {
                  pComboBoxProps->SetOnlySelectedView("");
               }
            break;
         default:
            break;
      }

      nRC = SetCursorNextEntity( vControl, pszCtrlMap, 0 );
   }

   // if Mapping Error
   if ( lMappingError > 0 )
   {
      CString strError;
      strError.Empty();
      // if no ComboBox type or ComboBox from type Domain
      if ( lMappingError == 3 || ulSubtype == 0  ||
           (( ulSubtype & ( zCOMBOBOX_POSITION_ONLY | zCOMBOBOX_SELECT |
              zCOMBOBOX_INCLUDE | zCOMBOBOX_FOREIGN_KEY) ) == 0 ) )
      {
         strError = "ComboBox MAPPING error ... no trolling entity specified.\n";
      }
      else
      if ( lMappingError == 1 ) // no Mapping for Edit Control
         strError = "ComboBox MAPPING error ... EditCtrl has no map entity.";
      else  // no Mapping for List Box
         strError = "ComboBox MAPPING error ... ListCtrl has no map entity.\nComboBox - ";
      if ( !strError.IsEmpty() )
         IssueError( vSubtask, 0, 0, strError);
   }

   return( nRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetupGridRadioGroup( zVIEW vSubtask, Column &Col, ZColumns& cols, ZColRadioGrp* pRadioGrpProps )
{
   zSHORT nRC = 0;
   zVIEW vControl = 0;
   zVIEW vWork = 0;
   zCHAR szHelp[ TZAXWRKO_DEFAULT_STRING_SIZE ];
   zPCHAR  pCtrlBOI;
   zPCHAR  pszRadioBtnValue = NULL;
   zCHAR   szIndicator[ 2 ];
   ValueItems ListOfValues;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   ListOfValues = Col.GetListOfValues();
   // if there are Radiobuttons add those too the Translationlist
   if ( CheckExistenceOfEntity( vControl, pszCtrlCtrl ) >= zCURSOR_SET )
   {
      ListOfValues.Clear();
      zVIEW vSubControl = 0;
      CreateViewFromViewForTask( &vSubControl, vControl, 0 );
      nRC = SetCursorFirstEntity( vSubControl, pszCtrlCtrl, 0 );
      SetViewToSubobject( vSubControl, pszCtrlCtrl );
      nRC = SetCursorFirstEntity( vSubControl, pszControl, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         // Set work attributes DefaultIndicator & Value from
         // Control.CtrlBOI.
         GetAddrForAttribute( &pCtrlBOI, vSubControl, pszControl, pszCtrlBOI );
         GetStringFromAttribute( szHelp, sizeof( szHelp ), vSubControl, pszControl, pszText );
         szIndicator[ 0 ] = pCtrlBOI[ 0 ];
         szIndicator[ 1 ] = 0;
         ZColRadioBtn *pZRB = new ZColRadioBtn();
         pZRB->SetDefault( (zBOOL) szIndicator[ 0 ] );
         pZRB->SetCheckValue( pCtrlBOI + 1 );
         pRadioGrpProps->AddButton( pZRB );
         CString strHelp;
         strHelp = pZRB->GetCheckValue( );
         COleVariant vValue = strHelp;

         // setup the list of valid entries for the current Column
         // this list might be reloaded during runtime if the current mapping
         // adresses a table domain
         ValueItemDisp ValueItem;
         ValueItem.CreateDispatch( ValueItem.GetCLSID() );
         ValueItem.SetValue( vValue );        // set internal attribute value
         strHelp = szHelp;
         vValue = strHelp;
         ValueItem.SetDisplayValue( vValue ); // set external attribute value

         ListOfValues.Add( ValueItem );

         nRC = SetCursorNextEntity( vSubControl, pszControl, 0 );
      }
      ResetViewFromSubobject( vSubControl );
      DropView( vSubControl );
   }
   return( nRC );
}


zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_Properties( zVIEW vSubtask )
{
   TZActiveX *pAtx = GetCurrentActiveX( vSubtask );
   if ( pAtx ) // && pAtx->IsKindOf( RUNTIME_CLASS( TZActiveX ) ) )
   {
      pAtx->ShowProperties( );
      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_NewSubControl
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_NewSubControl( zVIEW vSubtask )
{
   zVIEW vSubCtrl = 0;
   zVIEW vControl = 0;
   zLONG lCount = 0;

   //get the view whose cursor is on the right Control entity
   GetViewByName( &vSubCtrl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );

   //Create Control entity right after the current
   CreateTemporalMetaEntity( vSubtask, vSubCtrl, pszControl, zPOS_AFTER );
   lCount = CountEntitiesForView( vSubCtrl, pszControl );

   CString strHelp;
   strHelp.Format( "TrueGridColumn%d", lCount );
   SetAttributeFromString( vSubCtrl, pszControl, pszTag, (LPCSTR) strHelp );

   //Synchronize cursors so TZCONTROL points to the right entity
   // a cancel on a subdialog will undo this
   // ReCreateHelperViews( zVIEW vSubtask ) will reset all views when a
   // occurs "Return from Subwindow"
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   SetViewFromView( vControl, vSubCtrl );

   // Refresh Mapping-ListBox
   RefreshWindow( vSubtask );

   SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                            pszTZAXCFGD, pszNewColumn);
   return( 0 );
} // zwTZAXCFGD_NewSubControl

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZAXCFGD_UpdateSubControl
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_UpdateSubControl( zVIEW vSubtask )
{
   zVIEW vControl = 0;
   zVIEW vSubctrl = 0;
   zVIEW vPPE = 0;
   zLONG lControlType = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vSubctrl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );

   //vSubctrl is already positioned on the correct entity by an earlier call of
   //SetViewToSubobject( vSubctrl, pszCtrlCtrl );
   SetViewFromView( vControl, vSubctrl );
   CreateTemporalSubobjectVersion( vControl, pszControl );

   GetIntegerFromAttribute( &lControlType, vControl, pszControl, pszType );

   switch( lControlType )
   {
      case CONTROL_TYPE_COMBOBOX:
         //run combobox mapping dialogs
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                     pszTZAXCFGD, pszUPD_COMBO );
         break;
      case CONTROL_TYPE_CHECKBOX:
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                     pszTZAXCFGD, pszUPD_CHECK );
         break;
      case CONTROL_TYPE_EDIT:
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                     pszTZAXCFGD, pszUPD_TEXT );
         break;
      case CONTROL_TYPE_RADIOGRP:
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                     pszTZAXCFGD, pszUPD_RADIOGRP );
         break;
      default:
         break;
   }

   return( 0 );
} // zwTZAXCFGD_UpdateSubControl

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_COMBO_Init
// PURPOSE:  This function:
//           1.  Sets up the TZPNCTWO work object
//           2.  Creates the first CtrlMap entity instance, if necessary
//           3.  Sets up the work ComboBox entity
//           4.  Sets up the Domain Context mapping
//           5.  Sets up the Event ListBox properly
//           6.  Switches to either UPD_COMBO_D or UPD_COMBO_S if
//               a Subtype has been previously defined.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vWork;
   zLONG    CurrentSubtype;
   zLONG    lSubtype;
   zLONG    lFormatType;
   zCHAR    szStringSubtype[ 16 ];

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure ComboBox entity exits in Work OI.
   if ( CheckExistenceOfEntity( vWork, "ComboBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "ComboBox", zPOS_BEFORE );

   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   // Set ComboBox attribute values from Control.CtrlBOI
   SetEntityAttributesFromBlob( vWork, "ComboBox",
                                vDialogC, pszControl, "CtrlBOI" );

   // Set Default value for format type - Drop Down List
   //
   if ( GetVariableFromAttribute( &lFormatType, 0, zTYPE_INTEGER, 0,
                               vWork, "ComboBox", "FormatType", 0, 0 ) < 0 )
   {
       SetAttributeFromInteger( vWork, "ComboBox", "FormatType", 0 );
   }

   // If Subtype already is defined, switch to the proper window.
   GetStringFromAttribute( szStringSubtype, sizeof( szStringSubtype ), vWork, "ComboBox", "Subtype" );
   if ( szStringSubtype[ 0 ] != 0 )
   {
      GetIntegerFromAttribute( &CurrentSubtype, vWork,
                               "ComboBox", "Subtype" );
      SetAttributeFromInteger( vWork, "TZPNCTWO",
                               "TempCurrentComboSubtype", CurrentSubtype );
      if ( CurrentSubtype == 0 )
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  pszTZAXCFGD, "UPD_COMBO_D" );
      else
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  pszTZAXCFGD, "UPD_COMBO_S" );
   }

   GetIntegerFromAttribute( &lSubtype, vDialogC, pszControl, "Subtype" );

   if ( lSubtype & zCOMBOBOX_SORT )
      SetAttributeFromString( vWork, "ComboBox", "SortByListValue", "Y" );

   // Set up Domain Context mapping.
   zwTZAXCFGD_CtrlContextMappInit( vSubtask );

   return( 0 );
} // zwTZAXCFGD_COMBO_Init

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_COMBO_SetSubtype
// PURPOSE:  This function brings up the correct Combo Box window
//           based on the Subtype selected.  This code is only
//           execute by the initial Combo Box select window,
//           UPD_COMBO.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_SetSubtype( zVIEW vSubtask )
{
   zVIEW    vWork;
   zLONG    NewSubtype;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &NewSubtype, vWork, "ComboBox", "Subtype" );
   SetAttributeFromInteger( vWork, "TZPNCTWO",
                            "TempCurrentComboSubtype", NewSubtype );

   if ( NewSubtype == 0 )
        SetWindowActionBehavior( vSubtask,
         zWAB_ReplaceWindowWithModalWindow, pszTZAXCFGD, "UPD_COMBO_D" );
   else
        SetWindowActionBehavior( vSubtask,
         zWAB_ReplaceWindowWithModalWindow, pszTZAXCFGD, "UPD_COMBO_S" );

   return( 0 );
} // zwTZAXCFGD_COMBO_SetSubtype

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_COMBO_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_COMBO_PostBuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
      SetCtrlState( vSubtask, "ControlSubtype", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // zwTZAXCFGD_COMBO_PostBuild

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_COMBO_DS_SetSubtype
// PURPOSE:  This function looks at the ComboBox Subtype value and
//           switches to the other Window if the Subtype changes from
//           Domain to another or from another to Domain.
//           (See UPD_COMBO_SelAttrPullDown for related doc.)
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_DS_SetSubtype( zVIEW vSubtask )
{
   zVIEW    vWork;
   zLONG    CurrentSubtype;
   zLONG    NewSubtype;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &CurrentSubtype, vWork,
                            "TZPNCTWO", "TempCurrentComboSubtype" );
   GetIntegerFromAttribute( &NewSubtype, vWork, "ComboBox", "Subtype" );

   if ( (CurrentSubtype == 0) && (NewSubtype > 0) )
   {
      SetAttributeFromInteger( vWork, "TZPNCTWO",
                               "TempCurrentComboSubtype", NewSubtype );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               pszTZAXCFGD, "UPD_COMBO_S" );
   }

   if ( (CurrentSubtype > 0) && (NewSubtype == 0) )
   {
      SetAttributeFromInteger( vWork, "TZPNCTWO",
                               "TempCurrentComboSubtype", NewSubtype );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               pszTZAXCFGD, "UPD_COMBO_D" );
   }

   return( 0 );
} // zwTZAXCFGD_COMBO_DS_SetSubtype

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_COMBO_RtnFrmSbwnd
// PURPOSE:  This function sets up the Event entity in TZPNEVWO from
//           the selected Event entity in TZCONTROL.
//           It also sets the Event Type.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_RtnFrmSbwnd( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vDialogW;
   zVIEW    vWork;

   GetViewByName( &vDialog, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure WindowFlag is reset to indicate LISTBOX window.
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", "L" );

   // Make sure the SEL_LOD view is set up again.
   zwfnTZAXCFGD_SetUpSEL_LOD( vDialog, vDialogW, vSubtask, "CtrlMapView" );

   return( 0 );
} // zwTZAXCFGD_COMBO_RtnFrmSbwnd

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZAXCFGD_COMBO_D_PostBuild
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_D_PostBuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTZWINDOWL;
   zLONG  lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, pszControl, "Subtype" );
   SetCursorFirstEntity( vControl, pszCtrlMap, "" );

   if ( lSubtype & zCOMBOBOX_NO_NULL )
      SetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED, 1 );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "ControlSubtype", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "BoxStyle", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // zwTZAXCFGD_COMBO_D_PostBuild

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_COMBO_D_OK
// PURPOSE:  This function sets up X_ViewName, X_EntityName, etc.
//           attributes for interim compatibility.
//           It also sets up the Control.CtrlBOI and Control.Subtype
//           attributes.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_D_OK( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;
   zLONG    lWorkOrig;
   zLONG    lWorkNew;
   zLONG    lFormat;

   GetViewByName( &vDialog, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );


   // Accept the Control.
   AcceptSubobject( vDialog, pszControl );

   // Set Control.CtrlBOI from work entity ComboBox
   SetBlobFromEntityAttributes( vDialog, pszControl, "CtrlBOI",
                                 vWork, "ComboBox" );

   // Build Control.Subtype attribute
   GetIntegerFromAttribute( &lWorkOrig, vDialog, pszControl, "Subtype" );
   GetIntegerFromAttribute( &lWorkNew, vWork, "ComboBox", "Subtype" );
   GetIntegerFromAttribute( &lFormat, vWork, "ComboBox", "FormatType" );
   lWorkNew |= lFormat;

   // Blank out the low order bits in the original Subtype, because
   // we are writing over them with lWorkNew.
   lWorkOrig &= 0xFFFF0000L;
   lWorkOrig |= lWorkNew;

   /* Get the sort option for combo. */
   if ( CompareAttributeToString( vWork, "ComboBox",
                                   "SortByListValue", "Y" ) == 0 )
      lWorkOrig |= zCOMBOBOX_SORT;
   else
      lWorkOrig &= ~zCOMBOBOX_SORT;

   if ( GetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_NO_NULL;

   SetAttributeFromInteger( vDialog, pszControl, "Subtype", lWorkOrig );

   // Delete any additional CtrlMap entities, if they exist.
   SetCursorFirstEntity( vDialog, pszCtrlMap, "" );
   if ( SetCursorNextEntity( vDialog, pszCtrlMap, "" ) == zCURSOR_SET )
   {
      do
         DeleteEntity( vDialog, pszCtrlMap, zREPOS_NONE );
      while ( SetCursorNextEntity( vDialog, pszCtrlMap, "" ) == zCURSOR_SET );
   }

   // at this point we are a child of a TrueGrid
   ResetViewFromSubobject( vDialog );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
} // zwTZAXCFGD_COMBO_D_OK

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_COMBO_S_Init
// PURPOSE:  This function:
//           1.  Creates the 3 extra CtrlMap entities, if they don't
//               already exist.
//           2.  Names the 3 extra CtrlMap entities at the TASK level.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_S_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vDialogA;
   zVIEW    vDialogE;
   zVIEW    vDialogS;
   zVIEW    vDialogL;
   zVIEW    vWork;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure the last three entities are named at TASK level.
   SetCursorFirstEntity( vDialogC, pszCtrlMap, "" );
   CreateViewFromViewForTask( &vDialogA, vDialogC, 0 );
   SetCursorNextEntity( vDialogA, pszCtrlMap, "" );
   SetNameForView( vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDialogE, vDialogA, 0 );
   SetCursorNextEntity( vDialogE, pszCtrlMap, "" );
   SetNameForView( vDialogE, "TZPULL_Entity", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDialogS, vDialogE, 0 );
   SetCursorNextEntity( vDialogS, pszCtrlMap, "" );
   SetNameForView( vDialogS, "TZSCOPE", vSubtask, zLEVEL_TASK );

   // If combo box is going to only display selected values.
   CreateViewFromViewForTask( &vDialogL, vDialogS, 0 );
   if ( SetCursorNextEntity( vDialogL, pszCtrlMap, "" ) != 0 )
      CreateMetaEntity( vSubtask, vDialogL, pszCtrlMap, zPOS_AFTER );

   SetNameForView( vDialogL, "TZSELLIST", vSubtask, zLEVEL_TASK );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
   zwfnTZAXCFGD_SetUpSEL_LOD( vDialogA, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   zwfnTZAXCFGD_SetUpScopeListOI( vDialogE, vSubtask, "CtrlMapLOD_Entity" );

   // Set up the TempViewName attribute in the work object if there is
   // current TZPULL_Attribute mapping.
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) == zCURSOR_SET )
      SetAttributeFromAttribute( vWork,    "TZPNCTWO",    "TempViewName",
                                 vDialogA, "CtrlMapView", "Name" );

   // Make sure MapDomain is set up for the edit box mapping.
   if ( GetViewByName( &vDomain, "MapDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   }

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogA,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogA, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
} // zwTZAXCFGD_COMBO_S_Init

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_COMBO_SetScopeOI
// PURPOSE:  This function handles the setting of controls based on
//           the setting of the SCOPE Radio Buttons and Combo Boxes.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_SetScopeOI( zVIEW vSubtask )
{
   zVIEW vScope;

   if ( GetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED ) )
   {
      GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );

      // Exclude scoping entity
      if ( CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
         ExcludeEntity( vScope, "CtrlMapLOD_Entity", zREPOS_PREV );

      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      RefreshCtrl( vSubtask, "ScopeList" );
   }
   else
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, TRUE );
   return( 0 );
} // zwTZAXCFGD_COMBO_SetScopeOI

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_COMBO_SelectScope
// PURPOSE:  This function includes the Scoping entity under the
//           CtrlMap entity.  The entity is included from the SEL_LOD
//           object, but which entity to include comes from the
//           TZSCOPEL object.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_SelectScope( zVIEW vSubtask )
{
   zVIEW    vScope;
   zVIEW    vScopeL;
   zVIEW    vLOD;
   zLONG    lZKey;

   GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Exclude existing relationship if necessary.
   if ( CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      ExcludeEntity( vScope, "CtrlMapLOD_Entity", zREPOS_PREV );

   /* If the user selected the blank, then no entities will be selected. */
   if ( SetCursorFirstSelectedEntity( vScopeL, "ScopeEntity", 0 ) == 0 )
   {
      // Get position on correct LOD_Entity in SEL_LOD
      GetIntegerFromAttribute( &lZKey, vScopeL, "ScopeEntity",
                                                      "LOD_EntityZKey" );
      SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                                      "ZKey", lZKey, "" );
      IncludeSubobjectFromSubobject( vScope, "CtrlMapLOD_Entity",
                                    vLOD, "LOD_Entity", zPOS_AFTER );
   }

   return( 0 );
} // zwTZAXCFGD_COMBO_SelectScope

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_COMBO_SelectEntity
// PURPOSE:  This function excludes any Scoping entity already a part
//           of this control and sets up the Scoping List object for
//           the Scope List Box.  Note that the Mapping Entity has been
//           automatically included as a part of the Entity List Box.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_SelectEntity( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vScope;
   zVIEW    vScopeL;
   zVIEW    vLOD;
   zLONG    lZKey;
   zSHORT   FoundFlag;

   GetViewByName( &vDialogC, "TZPULL_Entity", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Exclude scoping entity
   if ( CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      ExcludeEntity( vScope, "CtrlMapLOD_Entity", zREPOS_PREV );

   // Set up Scope list object for list box mapping
   // First, delete existing entities.
   if ( CheckExistenceOfEntity( vScopeL, "ScopeEntity" ) == zCURSOR_SET )
   {
      SetCursorFirstEntity( vScopeL, "ScopeEntity", 0 );
      do
         DeleteEntity( vScopeL, "ScopeEntity", zREPOS_NONE );
      while ( SetCursorNextEntity( vScopeL, "ScopeEntity", "" ) == zCURSOR_SET );
   }
   // If entity List has cursor position, continue on

   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapLOD_Entity", "ZKey" );
      if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent",
                                      "ZKey", lZKey ) != 0 )
      {
         FoundFlag = 0;
         zwfnTZAXCFGD_SearchForEntityRecursively( vLOD, lZKey, &FoundFlag );
         do
         {
            CreateEntity( vScopeL, "ScopeEntity", zPOS_BEFORE );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity", "Name",
                                       vLOD, "LOD_EntityParent", "Name" );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity",
                                       "LOD_EntityZKey", vLOD,
                                       "LOD_EntityParent", "ZKey" );

         } while ( ResetViewFromSubobject( vLOD ) == 0 );
      }
   }

   // Note that ScopeList is name of combo box for both UPD_LISTBOX
   // and UPD_COMBO_S.
   RefreshCtrl( vSubtask, "ScopeList" );

   return( 0 );
} // zwTZAXCFGD_COMBO_SelectEntity

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_COMBO_S_SelAttPullDwn
// PURPOSE:  This function positions TZCONTROL to the 2nd CtrlMap
//           instance for going to the SEL_ATTR subwindow.
//          (See UPD_COMBO_ReturnFromSubwindow for related doc.)
//           It also sets X_ViewName, so that the return code can
//           check to see if the View mapping changed.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_S_SelAttPullDwn( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vWork;

   GetViewByName( &vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) == zCURSOR_SET )
      SetAttributeFromAttribute( vWork, "TZPNCTWO", "TempViewName",
                                 vDialogA, "CtrlMapView", "Name" );

   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   SetCursorNextEntity( vDialogC, pszCtrlMap, "" );

   return( 0 );
} // zwTZAXCFGD_COMBO_S_SelAttPullDwn

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_COMBO_S_RtnFromSubwdn
// PURPOSE:  This function does the following:
//           1.  It makes sure that TZCONTROL is positioned on the
//               first CtrlMap instance, since going to the SEL_ATTR
//               subwindow may have repositioned it.
//           2.  It executes the ReturnFromSubwindow code from LISTBOX
//               to set up the object for the Event list box.
//           3.  It resets cursors in the TZPULL_Attribute view,
//               since an exclude/include may have been done with
//               a different view.
//           4.  It excludes the Entity and Scope LOD_Entity
//               relationships, if the mapping for TZPULL_Attribute
//               was changed to a different view.  In that case, it
//               also includes the new Entity, assuming the Entity
//               is the same as that for the Attribute.
//           5.  It sets up the Entity pull down OI for the Entity
//               combo box.
//           6.  It sets up the Scope pull down OI for the Scope
//               combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_S_RtnFromSubwdn( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vDialogE;
   zVIEW    vDialogW;
   zVIEW    vDialogS;
   zVIEW    vSubCtrl = NULL;
   zVIEW    vWork;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogE, "TZPULL_Entity", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntity( vDialogC, pszCtrlMap, "" );

   // Execute the ReturnFromSubwindow code from LISTBOX to set up
   // the object for the Event list box.
   zwTZAXCFGD_COMBO_RtnFrmSbwnd( vSubtask );

   // Reset cursors for CtrlMap subobject view vDialogA
   SetCursorPrevEntity( vDialogA, pszCtrlMap, "" );
   SetCursorNextEntity( vDialogA, pszCtrlMap, "" );

   // If the mapping for TZPULL_Attribute was changed to a different view
   // or eliminated, exclude the Entity and Scope LOD_Entity relationships.
   //(CtrlMap.X_ViewName was set to the last view mapping.)
   // Also include new Entity, based on the TZPULL_Attribute.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( (CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) != zCURSOR_SET) ||
      (CompareAttributeToAttribute( vWork, "TZPNCTWO", "TempViewName",
                                    vDialogA, "CtrlMapView", "Name" ) != 0 ))
   {
      if ( CheckExistenceOfEntity( vDialogE,
                                   "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      {
         ExcludeEntity( vDialogE, "CtrlMapLOD_Entity", zREPOS_PREV );
      }

      GetViewByName( &vDialogS, "TZSCOPE", vSubtask, zLEVEL_TASK );
      if ( CheckExistenceOfEntity( vDialogS,
                                   "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      {
         ExcludeEntity( vDialogS, "CtrlMapLOD_Entity", zREPOS_PREV);
      }
   }

   // Set up SEL_LOD for EntityList combo box.
   zwfnTZAXCFGD_SetUpSEL_LOD( vDialogA, vDialogW, vSubtask, "CtrlMapView" );

   // Initialize ScopeL OI for ScopeList combo box.
   zwfnTZAXCFGD_SetUpScopeListOI( vDialogE, vSubtask, "CtrlMapLOD_Entity" );

   // Make sure MapDomain is set up for the edit box mapping.
   if ( GetViewByName( &vDomain, "MapDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   }

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogA,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogA, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
} // zwTZAXCFGD_COMBO_S_RtnFromSubwdn

///////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZAXCFGD_COMBO_S_PostBuild
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_S_PostBuild( zVIEW vSubtask )
{
   zVIEW vControl;
   zVIEW vTZWINDOWL;
   zLONG lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, pszControl, "Subtype" );

   if ( lSubtype & zCOMBOBOX_SCOPE_OI )
   {
      SetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED, 1 );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
   }

   if ( lSubtype & zCOMBOBOX_NO_NULL )
      SetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED, 1 );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "ControlSubtype", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "BoxStyle", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // zwTZAXCFGD_COMBO_S_PostBuild

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_COMBO_S_OK
// PURPOSE:  This function sets up X_ViewName, X_EntityName, etc.
//           attributes for interim compatibility.
//           It also sets up the Control.CtrlBOI and Control.Subtype
//           attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_COMBO_S_OK( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;
   zLONG    lWorkNew;
   zLONG    lWorkOrig;
   zLONG    lFormat;

   GetViewByName( &vDialog, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Accept the Control.
   AcceptSubobject( vDialog, pszControl );

   // Set Control.CtrlBOI from work entity ComboBox
   SetBlobFromEntityAttributes( vDialog, pszControl, "CtrlBOI",
                                 vWork, "ComboBox" );

   // Build Control.Subtype attribute
   GetIntegerFromAttribute( &lWorkOrig, vDialog, pszControl, "Subtype" );
   GetIntegerFromAttribute( &lWorkNew, vWork, "ComboBox", "Subtype" );

   GetIntegerFromAttribute( &lFormat, vWork, "ComboBox", "FormatType" );
   lWorkNew |= lFormat;
   // Blank out the low order bits in the original Subtype, because
   // we are writing over them with lWorkNew.
   lWorkOrig &= 0xFFFF0000L;
   lWorkOrig |= lWorkNew;

   /* Get the sort option for combo. */
   if ( CompareAttributeToString( vWork, "ComboBox",
                                   "SortByListValue", "Y" ) == 0 )
      lWorkOrig |= zCOMBOBOX_SORT; /* 16 */
   else
      lWorkOrig &= ~zCOMBOBOX_SORT;

// KELLY
   if ( GetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_SCOPE_OI;

   if ( GetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_NO_NULL;

   SetAttributeFromInteger( vDialog, pszControl, "Subtype", lWorkOrig );
   SetCursorFirstEntity( vDialog, pszCtrlMap, "" );

   ResetViewFromSubobject( vDialog );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
} // zwTZAXCFGD_COMBO_S_OK

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_CreateEdit
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_CreateEdit( zVIEW vSubtask )
{

   zVIEW vControl = 0;
   zLONG lCount = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vControl, pszControl, pszText, 0 );
   SetAttributeFromInteger( vControl, pszControl, pszType, CONTROL_TYPE_EDIT );

   // Create mapping entities for Edit
   lCount = CountEntitiesForView( vControl, pszCtrlMap );
   if ( lCount > 1 )
   {
      SetCursorLastEntity( vControl, pszCtrlMap, "" );
   }

   while ( lCount > 1 )
   {
      DeleteEntity( vControl, pszCtrlMap, zREPOS_PREV );
      lCount--;
   }

   while ( lCount < 1 )
   {
      CreateMetaEntity( vSubtask, vControl, pszCtrlMap, zPOS_AFTER );
      lCount++;
   }

   return( 0 );
} // zwTZAXCFGD_CreateEdit

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_CreateComboBox( zVIEW vSubtask )
{
   zVIEW vControl = 0;
   zLONG lCount = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vControl, pszControl, pszText, 0 );
   SetAttributeFromInteger( vControl, pszControl, pszType, CONTROL_TYPE_COMBOBOX );

   // delete existing mappings and create
   // Create 4 extra mapping entities for Combobox
   lCount = CountEntitiesForView( vControl, pszCtrlMap );
   if ( lCount > 0 )
   {
      SetCursorLastEntity( vControl, pszCtrlMap, "" );
   }

   while ( lCount > 4 )
   {
      DeleteEntity( vControl, pszCtrlMap, zREPOS_PREV );
      lCount--;
   }

   while ( lCount < 4 )
   {
      CreateMetaEntity( vSubtask, vControl, pszCtrlMap, zPOS_AFTER );
      lCount++;
   }

   return( 0 );
} // zwTZAXCFGD_CreateComboBox

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_CreateRadio
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_CreateRadio( zVIEW vSubtask )
{
   zVIEW vControl = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vControl, pszControl, pszText, 0 );
   SetAttributeFromInteger( vControl, pszControl, pszType, CONTROL_TYPE_RADIOGRP );


    return( 0 );
} // zwTZAXCFGD_CreateRadio

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_CreateCheckBox( zVIEW vSubtask )
{
   zVIEW vControl = 0;
   zLONG lCount = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vControl, pszControl, pszText, 0 );
   SetAttributeFromInteger( vControl, pszControl, pszType, CONTROL_TYPE_CHECKBOX );

   // Create mapping entities for CheckBox
   lCount = CountEntitiesForView( vControl, pszCtrlMap );
   if ( lCount > 1 )
   {
      SetCursorLastEntity( vControl, pszCtrlMap, "" );
   }

   while ( lCount > 1 )
   {
      DeleteEntity( vControl, pszCtrlMap, zREPOS_PREV );
      lCount--;
   }

   while ( lCount < 1 )
   {
      CreateMetaEntity( vSubtask, vControl, pszCtrlMap, zPOS_AFTER );
      lCount++;
   }

   return( 0 );

   return( 0 );
} // zwTZAXCFGD_CreateCheckBox

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_NewColumn_OnOk
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_NewColumn_OnOk( zVIEW vSubtask )
{
   zVIEW vControl = 0;
   zVIEW vPPE = 0;
   zLONG lControlType = 0;
   zLONG lRC = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vPPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lControlType, vControl, pszControl, pszType );

   switch( lControlType )
   {
      case CONTROL_TYPE_COMBOBOX:
         //run combobox mapping dialogs
         if ( CheckExistenceOfEntity( vControl, "ControlDef" ) != zCURSOR_SET )
         {
            SetCursorFirstEntityByInteger( vPPE, "ControlDef", "Key", CONTROL_TYPE_COMBOBOX, "" );
            IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                           vPPE,     "ControlDef", zPOS_AFTER );
         }
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                     pszTZAXCFGD, pszUPD_COMBO );
         break;
      case CONTROL_TYPE_CHECKBOX:
         if ( CheckExistenceOfEntity( vControl, "ControlDef" ) != zCURSOR_SET )
         {
            SetCursorFirstEntityByInteger( vPPE, "ControlDef", "Key", CONTROL_TYPE_CHECKBOX, "" );
            IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                           vPPE,     "ControlDef", zPOS_AFTER );
         }
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                     pszTZAXCFGD, pszUPD_CHECK );
         break;
      case CONTROL_TYPE_EDIT:
         if ( CheckExistenceOfEntity( vControl, "ControlDef" ) != zCURSOR_SET )
         {
            SetCursorFirstEntityByInteger( vPPE, "ControlDef", "Key", CONTROL_TYPE_EDIT, "" );
            IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                           vPPE,     "ControlDef", zPOS_AFTER );
         }
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                     pszTZAXCFGD, pszUPD_TEXT );
         break;
      case CONTROL_TYPE_RADIOGRP:
         if ( CheckExistenceOfEntity( vControl, "ControlDef" ) != zCURSOR_SET )
         {
            SetCursorFirstEntityByInteger( vPPE, "ControlDef", "Key", CONTROL_TYPE_RADIOGRP, "" );
            IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                           vPPE,     "ControlDef", zPOS_AFTER );
         }
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  pszTZAXCFGD, pszUPD_RADIOGRP );
         break;
      default:
         break;
   }

   return( 0 );
} // zwTZAXCFGD_NewColumn_OnOk

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_NewColumn_OnCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_NewColumn_OnCancel( zVIEW vSubtask )
{
   zVIEW vControl = 0;

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   CancelSubobject( vControl, pszControl );
   ResetViewFromSubobject( vControl );

   return( 0 );
} // zwTZAXCFGD_NewColumn_OnCancel

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_ReturnFromSub
// PURPOSE:  Make sure TZACTION is the View for the Action just
//           selected.  TZACTION to be a null View, if no EventAct
//           existed prior to the Select.
//           Also drop the TZACTION View if it is null.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_ReturnFromSub( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;
   zVIEW  vEvent;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      if ( vDialogA )
         DropView( vDialogA );

      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

      // Update Event/Action list.
      GetViewByName( &vEvent, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      SetAttributeFromAttribute( vEvent, "EventAct", "Tag",
                                 vDialogC, "EventAct", "Tag" );
   }

   return( 0 );
} // zwTZAXCFGD_ReturnFromSub

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_CHECK_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_CHECK_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "edChecked", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edUnchecked", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // zwTZAXCFGD_CHECK_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_UpdateAction
// PURPOSE:  This function sets the TZWINDOW cursor to the correct
//           Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_UpdateAction( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vDialogW;

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( (CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0) )
   {
      SetCursorFirstEntityByAttr( vDialogW, "Action",   "Tag",
                                  vWork,    "EventAct", "Tag", 0 );
      CreateTemporalSubobjectVersion( vDialogW, "Action" );
   }
   else
   {
      MessageSend( vSubtask, "PN00103", "Dialog Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
} // zwTZAXCFGD_UpdateAction

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_CHECK_Init
// PURPOSE:  This function:
//           1.  Creates the first CtrlMap entity instance, if necessary
//           2.  Sets up the Work CheckBox entity
//           3.  Sets up the Domain Context mapping
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_CHECK_Init( zVIEW vSubtask )
{
   zVIEW   vDialogC;
   zVIEW   vDialogW;
   zVIEW   vDialogA;
   zVIEW   vWork;
   zPCHAR  pCtrlBOI;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   //CreateTemporalSubobjectVersion( vDialogC, "Control" );

   // Create the first CtrlMap entity instance, if necessary.
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMap" ) != zCURSOR_SET )
      CreateEntity( vDialogC, "CtrlMap", zPOS_BEFORE );

   // Make sure CheckBox entity exits in Work OI.
   if ( CheckExistenceOfEntity( vWork, "CheckBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "CheckBox", zPOS_BEFORE );

   // Set CheckBox attribute values from Control.CtrlBOI
   GetAddrForAttribute( &pCtrlBOI, vDialogC, "Control", "CtrlBOI" );
   if ( pCtrlBOI )
   {
      SetAttributeFromString( vWork,
                              "CheckBox", "CheckOn", pCtrlBOI );
      while ( pCtrlBOI[ 0 ] ) pCtrlBOI++;
      pCtrlBOI++;
      SetAttributeFromString( vWork,
                              "CheckBox", "CheckOff", pCtrlBOI );
   }

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // If not, set TZACTION to empty instance for auto graying.
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   // Set up Domain Context mapping.
   zwTZAXCFGD_CtrlContextMappInit( vSubtask );

   return( 0 );
} // zwTZAXCFGD_CHECK_Init

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_CHECK_OK
// PURPOSE:  This function sets Control.CtrlBOI values from CheckBox
//           entity and sets Context.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_CHECK_OK( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vWork;
   zCHAR   szCtrlBOI[ 256 ];
   zSHORT  nLth;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Accept the Control.
   AcceptSubobject( vDialogC, "Control" );

   // Set CtrlBOI values

   // Null out edit text since it has no meaning and we prefer the
   // tag to be displayed in the painter
   GetStringFromAttribute( szCtrlBOI, sizeof( szCtrlBOI ), vWork, "CheckBox", "CheckOn" );
   nLth = 0;
   while ( szCtrlBOI[ nLth ] )
      nLth++;

   nLth++;
   GetStringFromAttribute( szCtrlBOI + nLth, sizeof( szCtrlBOI ) - nLth, vWork,
                           "CheckBox", "CheckOff" );
   while ( szCtrlBOI[ nLth ] )
      nLth++;

   nLth++; // with the code looking like this, I assume the extra byte
           // is for the null terminator.

   SetAttributeFromBlob( vDialogC, "Control", "CtrlBOI", szCtrlBOI, nLth );

   // at this point we are a child of a TrueGrid
   ResetViewFromSubobject( vDialogC );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
} // zwTZAXCFGD_CHECK_OK

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_TEXT_Init
// PURPOSE:  This function does the necessary work to set up
//           the Context combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_TEXT_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vDialogW;
   zVIEW    vWork;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set Subtype in work object for Window EditBox
   if ( CheckExistenceOfEntity( vWork, "EditBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "EditBox", zPOS_BEFORE );

   // Set up Domain Context mapping.
   zwTZAXCFGD_CtrlContextMappInit( vSubtask );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   return( 0 );
} // zwTZAXCFGD_TEXT_Init

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZAXCFGD_TEXT_PostBuild
//
// PURPOSE:  This function does the necessary work to set up
//           the Text font information using the following
//           structure:
//
//             byte  field       value
//             ----  ------      -----------------------------
//               0   Bold        1 - on ... 0 - off
//               1   Italic      1 - on ... 0 - off
//               2   Strikeout   1 - on ... 0 - off
//               3   Underline   1 - on ... 0 - off
//              4-7  PointSize   long > 0
//             8-11  RGB Color   unsigned long RGB color value
//  not used    12   MultiLine      1 - on ... 0 - off
//              13   Left Justify   1 - on ... 0 - off
//              14   Center Justify 1 - on ... 0 - off
//              15   Right Justify  1 - on ... 0 - off
//  not used  16-19  BorderStyle (0-solid 1-dash 2-dot 3-dashdot 4-dashdotdot)
//               (second byte 1-top | 2-left | 4-bottom | 8-right)
//               (high order byte is line width)
//  not used  20-23  BorderColor    unsigned long RGB color value
//            24-27  Escapement
//            28-31  Type      // reserved
//            32-63  Font face name
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_TEXT_PostBuild( zVIEW vSubtask )
{
   zCHAR   szBlob[ 8 + (6 * sizeof( long )) + LF_FACESIZE ];
   zVIEW   vControl;
   zLONG   lColor;
   zLONG   lSize;
// zLONG   lBorderStyle;
// zLONG   lBorderColor = 0;
   zLONG   lEscapement = 0;
   zULONG  uLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetBlobFromAttribute( szBlob, &uLth, vControl, "Control", "CtrlBOI" );
// TraceLineI( "Text control blob lth = ", uLth );
   if ( uLth >= 4 + 2 * sizeof( long ) )
   {
      SetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_CHECKED,
                    szBlob[ 0 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_CHECKED,
                    szBlob[ 1 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_CHECKED,
                    szBlob[ 2 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_CHECKED,
                    szBlob[ 3 ] ? TRUE : FALSE );

      lSize  = *((zPLONG) (szBlob + sizeof( long )));
      lColor = *((zPLONG) (szBlob + sizeof( long ) * 2));

   // SetCtrlState( vSubtask, "MultiLine", zCONTROL_STATUS_CHECKED,
   //               szBlob[ 4 + (2 * sizeof( long )) ] ? TRUE : FALSE );

      if ( uLth > 5 + (2 * sizeof( long )) )
      {
         SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED,
                       szBlob[ 5 + (2 * sizeof( long )) ] ? TRUE : FALSE );
         SetCtrlState( vSubtask, "Center", zCONTROL_STATUS_CHECKED,
                       szBlob[ 6 + (2 * sizeof( long )) ] ? TRUE : FALSE );
         SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED,
                       szBlob[ 7 + (2 * sizeof( long )) ] ? TRUE : FALSE );
         if ( uLth > 8 + (2 * sizeof( long )) )
         {
            SetCtrlText( vSubtask, "FontFace",
                         szBlob + 8 + (6 * sizeof( long )) );

         // lBorderColor = *((zPLONG) (szBlob + 8 + (3 * sizeof( long ))));
            lEscapement = *((zPLONG) (szBlob + 8 + (4 * sizeof( long ))));

            // careful not to mess up szBlob prematurely!
         // lBorderStyle >>= 24;
         // zltoa( lBorderStyle, szBlob, sizeof( szBlob ) );
         // SetCtrlText( vSubtask, "LineWidth", szBlob );
         }
      }
      else
         SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );

      zltoa( lSize, szBlob, sizeof( szBlob ) );
      SetCtrlText( vSubtask, "Size", szBlob );
      zltoa( lColor, szBlob, sizeof( szBlob ) );
      SetCtrlText( vSubtask, "Color", szBlob );
      zltoa( lEscapement, szBlob, sizeof( szBlob ) );
      SetCtrlText( vSubtask, "Escapement", szBlob );
   }

   return( 0 );
} // zwTZAXCFGD_TEXT_PostBuild

/////////////////////////////////////////////////////////////////////////////
// ENTRY:    zwTZAXCFGD_TEXT_OK
// PURPOSE:  This function sets the Context as necessary and tells
//           the Painter to refresh the control.
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_TEXT_OK( zVIEW vSubtask )
{
   zCHAR   szBlob[ 8 + (6 * sizeof( long )) + LF_FACESIZE ];
   zVIEW   vControl;
   zLONG   lColor;
   zLONG   lSize;
   zLONG   lEscapement;
   zLONG   lBorderStyle = 0;
   zLONG   lBorderColor = 0;
   zLONG   lType = 0;
   zBOOL   bMultiLine;
   zBOOL   bBold;
   zBOOL   bItalic;
   zBOOL   bStrikeout;
   zBOOL   bUnderline;
   zBOOL   bLeft;
   zBOOL   bCenter;
   zBOOL   bRight;
   zUSHORT uLth;

   // initialize the blob
   memset( szBlob, 0, sizeof( szBlob ) );

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // set things
   bMultiLine = 0; // (zBOOL) GetCtrlState( vSubtask, "MultiLine", zCONTROL_STATUS_CHECKED );
   bBold      = (zBOOL) GetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_CHECKED );
   bItalic    = (zBOOL) GetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_CHECKED );
   bStrikeout = (zBOOL) GetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_CHECKED );
   bUnderline = (zBOOL) GetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_CHECKED );
   bLeft      = (zBOOL) GetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED );
   bCenter    = (zBOOL) GetCtrlState( vSubtask, "Center", zCONTROL_STATUS_CHECKED );
   bRight     = (zBOOL) GetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED );

   GetCtrlText( vSubtask, "Size", szBlob, sizeof( szBlob ) );
   lSize = atol( szBlob );
   GetCtrlText( vSubtask, "Color", szBlob, sizeof( szBlob ) );
   lColor = atol( szBlob );
   GetCtrlText( vSubtask, "Escapement", szBlob, sizeof( szBlob ) );
   lEscapement = atol( szBlob );
   GetCtrlText( vSubtask, "FontFace", szBlob + 8 + (6 * sizeof( long )),
                LF_FACESIZE );

   if ( bBold || bItalic || bStrikeout || bUnderline ||
        lSize || lColor || lEscapement ||
        lType || bCenter || bRight || szBlob[ 8 + (5 * sizeof( long )) ] )
   {
      zPLONG lpLong;

      uLth = sizeof( szBlob );
      szBlob[ 0 ] = bBold;
      szBlob[ 1 ] = bItalic;
      szBlob[ 2 ] = bStrikeout;
      szBlob[ 3 ] = bUnderline;

      lpLong = (zPLONG) (szBlob + sizeof( long ));
      *lpLong = lSize;
      lpLong = (zPLONG) (szBlob + sizeof( long ) * 2);
      *lpLong = lColor;

      szBlob[ 4 + 2 * sizeof( long ) ] = bMultiLine;
      szBlob[ 5 + 2 * sizeof( long ) ] = bLeft;
      szBlob[ 6 + 2 * sizeof( long ) ] = bCenter;
      szBlob[ 7 + 2 * sizeof( long ) ] = bRight;

      lpLong = (zPLONG) (szBlob + 8 + (2 * sizeof( long )));
      *lpLong = lBorderStyle;
      lpLong = (zPLONG) (szBlob + 8 + (3 * sizeof( long )));
      *lpLong = lBorderColor;
      lpLong = (zPLONG) (szBlob + 8 + (4 * sizeof( long )));
      *lpLong = lEscapement;
      lpLong = (zPLONG) (szBlob + 8 + (5 * sizeof( long )));
      *lpLong = lType;
   }
   else
   {
      uLth = 0;
   }

   // store the blob
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, uLth );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   // at this point we are a child of a TrueGrid
   ResetViewFromSubobject( vControl );


   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
} // zwTZAXCFGD_TEXT_OK

//./ ADD NAME=zwTZAXCFGD_RadioGroupOK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioGroupOK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioGroupOK( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szBlob[ 256 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntity( vControl, "CtrlCtrl", "" );
   if ( SetCursorFirstEntity( vWork, "Radio", "" ) >= zCURSOR_SET )
   do
   {
      SetAttributeFromAttribute( vControl, "CtrlCtrl", "Tag",
                                  vWork, "Radio", "Tag" );
      SetAttributeFromAttribute( vControl, "CtrlCtrl", "Text",
                                  vWork, "Radio", "Text" );
      // Set the DefaultIndicator & Value attributes from the Radio
      // entity in the work OI into Control.CtrlBOI.
      GetStringFromAttribute( szBlob, sizeof( szBlob ), vWork, "Radio", "DefaultIndicator" );
      GetStringFromAttribute( szBlob + 1, sizeof( szBlob ) - 1, vWork, "Radio", "Value" );
      SetAttributeFromBlob( vControl, "CtrlCtrl", "CtrlBOI",
                            szBlob, (zUSHORT) (zstrlen( szBlob + 1 ) + 2) );
      SetCursorNextEntity( vWork, "Radio", "" );
   }
   while ( SetCursorNextEntity( vControl, "CtrlCtrl", "" ) >= 0 );

   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", 0 );
   DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );

   AcceptSubobject( vControl, "Control" );

   ResetViewFromSubobject( vControl );

   return( 0 );
} // zwTZAXCFGD_RadioGroupOK

//./ ADD NAME=zwTZAXCFGD_RadioUpdateSubCtrl
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioUpdateSubCtrl
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioUpdateSubCtrl( zVIEW vSubtask )
{
   zVIEW vControl;
   zVIEW vWork;
   zCHAR szTag[ 34 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Position on vControl.CtrlCtrl by vWork.Radio position
   GetStringFromAttribute( szTag, sizeof( szTag ), vWork, "Radio", "Tag" );
   SetCursorFirstEntityByString( vControl, "CtrlCtrl", "Tag", szTag, "" );

   SetViewToSubobject( vControl, "CtrlCtrl" );

   return( 0 );
} // zwTZAXCFGD_RadioUpdateSubCtrl

//./ ADD NAME=zwTZAXCFGD_RadioSetDefaultButton
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioSetDefaultButton
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioSetDefaultButton( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zVIEW   vWork2;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vWork, "Radio" ) >= 0 )
   {
      // First clear default
      CreateViewFromViewForTask( &vWork2, vWork, 0 );
      SetCursorFirstEntity( vWork2, "Radio", "" );
      do
      {
         SetAttributeFromString( vWork2, "Radio", "DefaultIndicator", "" );
      }
      while ( SetCursorNextEntity( vWork2, "Radio", "" ) >= 0 );
      DropView( vWork2 );

      // Now set the new default value and refresh ListBox control.
      SetAttributeFromString( vWork, "Radio", "DefaultIndicator", "Y" );
      RefreshCtrl( vSubtask, "ListBox1" );
   }

   return( 0 );
} // zwTZAXCFGD_RadioSetDefaultButton



//./ ADD NAME=zwTZAXCFGD_RadioPrebuildRadioGrp
// Source Module=tzpnctad.c
////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioPrebuildRadioGrp
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioPrebuildRadioGrp( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szIndicator[ 2 ];
   zPCHAR  pCtrlBOI;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Build RadioGroup and Radio entities in TZPNCTWO OI
   if ( CheckExistenceOfEntity( vWork, "RadioGroup" ) >= 0 )
      DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );

   CreateEntity( vWork, "RadioGroup", zPOS_AFTER );
   if ( CheckExistenceOfEntity( vControl, "CtrlCtrl" ) >= 0 )
   {
      do
      {
         CreateEntity( vWork, "Radio", zPOS_AFTER );
         // Set work attributes DefaultIndicator & Value from
         // Control.CtrlBOI.
         GetAddrForAttribute( &pCtrlBOI, vControl, "CtrlCtrl", "CtrlBOI" );
         szIndicator[ 0 ] = pCtrlBOI[ 0 ];
         szIndicator[ 1 ] = 0;
         SetAttributeFromString( vWork, "Radio", "DefaultIndicator",
                                 szIndicator );
         SetAttributeFromString( vWork, "Radio", "Value",
                                 pCtrlBOI + 1 );
         SetAttributeFromAttribute( vWork, "Radio", "Tag",
                                    vControl, "CtrlCtrl", "Tag" );
         SetAttributeFromAttribute( vWork, "Radio", "Text",
                                    vControl, "CtrlCtrl", "Text" );
      }
      while ( SetCursorNextEntity( vControl, "CtrlCtrl", "" ) >= 0 );
   }

   // Set WindowFlag to 'R' to indicate being in radiogroup update window.
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", "R" );

   // Set up Domain Context mapping.
   zwTZAXCFGD_CtrlContextMappInit( vSubtask );

   return( 0 );
} // zwTZAXCFGD_RadioPrebuildRadioGrp



//./ ADD NAME=zwTZAXCFGD_RadioGroupCancel
// Source Module=tzpnctad.c
////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioGroupCancel
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioGroupCancel( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", 0 );
   DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );
   CancelSubobject( vControl, "Control" );

   ResetViewFromSubobject( vControl );

   return( 0 );
} // zwTZAXCFGD_RadioGroupCancel



//./ ADD NAME=zwTZAXCFGD_RadioClearDefaultBtn
// Source Module=tzpnctad.c
////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioClearDefaultBtn
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioClearDefaultBtn( zVIEW vSubtask )
{
   zVIEW   vWork;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vWork, "Radio" ) >= 0 )
   {
      SetCursorFirstEntity( vWork, "Radio", "" );
      do
      {
         SetAttributeFromString( vWork, "Radio", "DefaultIndicator", "" );
      }
      while ( SetCursorNextEntity( vWork, "Radio", "" ) >= 0 );
      RefreshCtrl( vSubtask, "ListBox1" );
   }

   return( 0 );
} // zwTZAXCFGD_RadioClearDefaultBtn



//./ ADD NAME=zwTZAXCFGD_RadioSubCtrlCancel
// Source Module=tzpnctad.c
////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioSubCtrlCancel
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioSubCtrlCancel( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Delete the RadioGroup entity if control came directly from
   // the Painter.
   // Else, reset the subobject.
   if ( CompareAttributeToString( vWork, "TZPNCTWO", "WindowFlag",
                                   "R" ) != 0 )
      DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );
   CancelSubobject( vControl, "Control" );

   // go back to Group level
   ResetViewFromSubobject( vControl );

   return( 0 );
} // zwTZAXCFGD_RadioSubCtrlCancel


static void
zwfnTZAXCFGD_PUSH_Init( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }
}

//./ ADD NAME=zwTZAXCFGD_RadioPrebuildSubCtrl
// Source Module=tzpnctad.c
////////////////////////////////////////////////
//
// ENTRY:      zwTZAXCFGD_RadioPrebuildSubCtrl
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioPrebuildSubCtrl( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zVIEW   vGroup;
   zPCHAR  pCtrlBOI;
   zPCHAR  pchTag;
   zCHAR   szIndicator[ 2 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // If RadioGroup entity exists, control has been passed from the
   // RadioGroup window.  Otherwise, control came directly from
   // the Painter.  In the latter case, create the RadioGroup and
   // Radio entities and set the flag to indicate control came from
   // the Painter.
   if ( CheckExistenceOfEntity( vWork, "RadioGroup" ) < 0 )
   {
      CreateViewFromViewForTask( &vGroup, vControl, 0 );
      ResetViewFromSubobject( vGroup );
      CreateEntity( vWork, "RadioGroup", zPOS_AFTER );
      SetCursorFirstEntity( vGroup, "CtrlCtrl", 0 );
      do
      {
         CreateEntity( vWork, "Radio", zPOS_AFTER );
         // Set work attributes DefaultIndicator & Value from
         // Control.CtrlBOI.
         GetAddrForAttribute( &pCtrlBOI, vGroup, "CtrlCtrl", "CtrlBOI" );
         szIndicator[ 0 ] = pCtrlBOI[ 0 ];
         szIndicator[ 1 ] = 0;
         SetAttributeFromString( vWork, "Radio", "DefaultIndicator",
                                 szIndicator );
         SetAttributeFromString( vWork, "Radio", "Value",
                                 pCtrlBOI + 1 );
         SetAttributeFromAttribute( vWork, "Radio", "Tag",
                                    vGroup, "CtrlCtrl", "Tag" );
         SetAttributeFromAttribute( vWork, "Radio", "Text",
                                    vGroup, "CtrlCtrl", "Text" );
      }
      while ( SetCursorNextEntity( vGroup, "CtrlCtrl", "" ) >= 0 );
      DropView( vGroup );
      GetAddrForAttribute( &pchTag, vControl, "Control", "Tag" );
      SetCursorFirstEntityByString( vWork, "Radio", "Tag", pchTag, 0 );
   }

   zwfnTZAXCFGD_PUSH_Init( vSubtask );
   return( 0 );
} // zwTZAXCFGD_RadioPrebuildSubCtrl

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_RadioPostbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_RadioPostbuild( zVIEW vSubtask )
{
   zVIEW    vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "edValue", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Check1", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // zwTZAXCFGD_RadioPostbuild

//./ ADD NAME=zwTZAXCFGD_RadioSubCtrlOK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      zwTZAXCFGD_RadioSubCtrlOK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZAXCFGD_RadioSubCtrlOK( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szBlob[ 256 ];
   zPCHAR  szWork;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Note that the Radio control is not updated as a temporal
   // subobject as the corresponding window only has edit boxes,
   // where canceling mapping is all that is required to abort
   // the transaction.

   // Set the DefaultIndicator & Value attributes from the Radio
   // entity in the work OI into Control.CtrlBOI.
   GetStringFromAttribute( szBlob, sizeof( szBlob ), vWork, "Radio", "DefaultIndicator" );
   GetStringFromAttribute( szBlob + 1, sizeof( szBlob ) - 1, vWork, "Radio", "Value" );
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI",
                         szBlob, (zUSHORT) (zstrlen( szBlob + 1 ) + 2) );

   // Delete the RadioGroup entity if control came directly from
   // the Painter.
   // Else, set Tag and Text values and reset the subobject.
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "R" ) != 0 )
   {
      DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );
   }
   else
   {
      SetAttributeFromAttribute( vWork, "Radio", "Tag",
                                 vControl, "Control", "Tag" );
      SetAttributeFromAttribute( vWork, "Radio", "Text",
                                 vControl, "Control", "Text" );
   }

   AcceptSubobject( vControl, "Control" );

   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "R" ) != 0 )
   {
      // If we are returning to the painter, then we must turn off
      // the default indicator on other radio buttons if we are
      // now the default button
      if ( szBlob[ 0 ] == 'Y' )
      {
         CreateViewFromViewForTask( &vWork, vControl, 0 );
         while ( SetCursorPrevEntity( vWork, "Control", 0 ) >= zCURSOR_SET )
         {
            GetAddrForAttribute( &szWork, vWork, "Control", "CtrlBOI" );
            if ( szWork && szWork[ 0 ] == 'Y' )
               szWork[ 0 ] = 0;
         }
         SetViewFromView( vWork, vControl );
         while ( SetCursorNextEntity( vWork, "Control", 0 ) >= zCURSOR_SET )
         {
            GetAddrForAttribute( &szWork, vWork, "Control", "CtrlBOI" );
            if ( szWork && szWork[ 0 ] == 'Y' )
               szWork[ 0 ] = 0;
         }
         DropView( vWork );
      }
   }
   else
      ResetViewFromSubobject( vControl );

   return( 0 );
} // zwTZAXCFGD_RadioSubCtrlOK

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_CreateRadioButton
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_CreateRadioButton( zVIEW vSubtask )
{
   zVIEW vPPE = 0;
   zVIEW vControl = 0;
   zVIEW vWork = 0;
   zLONG lCount = 0;
   zCHAR szIndicator[ 3 ];

   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vPPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   CreateTemporalMetaEntity( vSubtask, vControl, pszCtrlCtrl, zPOS_AFTER );

   lCount = CountEntitiesForView( vControl, pszCtrlCtrl );
   lCount++;

   CString strHelp;
   strHelp.Format( "GridRadioButton%d", lCount );
   SetAttributeFromString( vControl, pszCtrlCtrl, pszTag, (LPCSTR)strHelp );

   CreateEntity( vWork, "Radio", zPOS_AFTER );
   // Set work attributes DefaultIndicator & Value from
   // Control.CtrlBOI.
   szIndicator[ 0 ] = 0;
   szIndicator[ 1 ] = 0;
   SetAttributeFromString( vWork, "Radio", "DefaultIndicator",
                           szIndicator );
   SetAttributeFromString( vWork, "Radio", "Value",
                           "" );
   SetAttributeFromAttribute( vWork, "Radio", "Tag",
                              vControl, "CtrlCtrl", "Tag" );
   SetAttributeFromAttribute( vWork, "Radio", "Text",
                              vControl, "CtrlCtrl", "Text" );

   SetViewToSubobject( vControl, pszCtrlCtrl );

   if ( CheckExistenceOfEntity( vControl, "ControlDef" ) != zCURSOR_SET )
   {
      SetCursorFirstEntityByInteger( vPPE, "ControlDef", "Key", CONTROL_TYPE_RADIO, "" );
      IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                     vPPE,     "ControlDef", zPOS_AFTER );
   }

   return( 0 );
} // zwTZAXCFGD_CreateRadioButton

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_DeleteRadioButton
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_DeleteRadioButton( zVIEW vSubtask )
{
   zVIEW vRadioWork = 0;
   zSHORT nRC = zCURSOR_NULL;
   zVIEW vControl = 0;
   zCHAR szTag[ 256 ];

   // delete work object
   GetViewByName( &vRadioWork, pszTZPNCTWO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szTag, sizeof( szTag ), vRadioWork, pszRadio, pszTag );
   nRC = DeleteEntity( vRadioWork, pszRadio, zREPOS_NEXT );

   // delete real control entry
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );
   nRC = SetCursorFirstEntityByString( vControl, pszCtrlCtrl, pszTag, szTag, "" );
   if ( nRC > zCURSOR_UNCHANGED )
      DeleteEntity( vControl, pszCtrlCtrl, zREPOS_NEXT );

   return( 0 );
} // zwTZAXCFGD_DeleteRadioButton

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_DeleteAllRadioButtons
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_DeleteAllRadioButtons( zVIEW vSubtask )
{
   zVIEW vRadioWork = 0;
   zVIEW vControl = 0;
   zSHORT nRC = zCURSOR_NULL;

   GetViewByName( &vRadioWork, pszTZPNCTWO, vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, pszTZCONTROL, vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vRadioWork, pszRadio,"" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      nRC = DeleteEntity( vRadioWork, pszRadio, zREPOS_FIRST );
   }

   nRC = SetCursorFirstEntity( vControl, pszCtrlCtrl,"" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      // delete real control entry
      nRC = DeleteEntity( vControl, pszCtrlCtrl, zREPOS_FIRST );
   }

   return( 0 );
} // zwTZAXCFGD_DeleteAllRadioButtons

zSHORT OPERATION
ReCreateHelperViews( zVIEW vSubtask )
{
   zVIEW  vDialogC  = NULL;
   zVIEW  vSubCtrl  = NULL;
   zVIEW  vMapScope = NULL;
   zVIEW  vMapList  = NULL;

   GetViewByName( &vDialogC,  pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vSubCtrl,  pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapScope, pszTZTRUEGRID_SCOPE, vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapList,  pszTZTRUEGRID_LIST,  vSubtask, zLEVEL_TASK );

   if ( vSubCtrl )
   {
       DropView ( vSubCtrl );
   }
   if ( vMapScope )
   {
       DropView ( vMapScope );
   }
   if ( vMapList )
   {
       DropView ( vMapList );
   }
   // create view to display column details
   CreateViewFromViewForTask( &vSubCtrl, vDialogC, 0 );
   SetNameForView( vSubCtrl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );
   SetViewToSubobject( vSubCtrl, pszCtrlCtrl );

   // create view to display scope Mapping
   CreateViewFromViewForTask( &vMapScope, vDialogC, 0 );
   SetNameForView( vMapScope, pszTZTRUEGRID_SCOPE, vSubtask, zLEVEL_TASK );
   // Position to a specific mapping named "Scope"
   SetCursorFirstEntityByString( vMapScope, pszCtrlMap, pszTag, pszScope, "" );

   // create view to display List Mapping
   CreateViewFromViewForTask( &vMapList, vDialogC, 0 );
   SetNameForView( vMapList, pszTZTRUEGRID_LIST, vSubtask, zLEVEL_TASK );
   // Position to a specific mapping named "List"
   SetCursorFirstEntityByString( vMapList, pszCtrlMap, pszTag, pszList, "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_GridScopeMapping
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_GridScopeMapping( zVIEW vSubtask )
{
   zVIEW  vControl  = NULL;
   zVIEW  vMapScope = NULL;

   GetViewByName( &vControl,  pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapScope,  pszTZTRUEGRID_SCOPE,  vSubtask, zLEVEL_TASK );

   SetViewFromView( vControl, vMapScope );

   return( 0 );
} // zwTZAXCFGD_GridScopeMapping

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_GridListMapping
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_GridListMapping( zVIEW vSubtask )
{
   zVIEW  vControl  = NULL;
   zVIEW  vMapList = NULL;

   GetViewByName( &vControl,  pszTZCONTROL, vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapList,  pszTZTRUEGRID_LIST,  vSubtask, zLEVEL_TASK );

   SetViewFromView( vControl, vMapList );

   return( 0 );

} // zwTZAXCFGD_GridListMapping

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZAXCFGD_DeleteSubControl
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZAXCFGD_DeleteSubControl( zVIEW vSubtask )
{
   zVIEW vSubctrl;
   zBOOL bEnable = TRUE;

   GetViewByName( &vSubctrl, pszTZGRIDSUBCONTROL, vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vSubctrl, pszControl ) == zCURSOR_SET )
   {
      DeleteEntity( vSubctrl, "Control", zREPOS_PREV );
      RefreshCtrl( vSubtask, "ListSubControls" );
   }

   if ( CheckExistenceOfEntity( vSubctrl, pszControl ) < zCURSOR_SET )
      bEnable = FALSE;

   EnableAction( vSubtask, "DeleteSubcontrol", bEnable );
   EnableAction( vSubtask, "UpdateSubControl", bEnable );

   return( 0 );
} // zwTZAXCFGD_DeleteSubControl

#ifdef __cplusplus
}
#endif
