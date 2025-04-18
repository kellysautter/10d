#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
#include "TZ__OPRS.H" 
#include "TZVMLIP.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
o_InitializeAD_WorkObjectReg( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_AutodesignMultiPageSub( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_AutodesignMultiPageSub( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SAVE_AutodesignDefinition( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_FindReturnedCL_Attribs( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_FindReturnedListAttribs( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_AD_AttributeAsRange( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeTo( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_MainInclAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_GroupListWInclude( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_Group( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_IncludeObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_SearchObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_FindSearchObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignFindSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_AutoDesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_AutoDesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_Group( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CHECK_ReorderAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_CRM_FindSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_ObjectFindSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_FindReturnedObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignCRMSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_ReturnedAttribute( zVIEW     ViewToWindow );


static zSHORT
o_ChangeViewObjRef( zVIEW     ViewToWindow,
                    zVIEW     TZWINDOW,
                    zVIEW     TZWINDOWL,
                    zPCHAR    szViewObjRefName,
                    zPCHAR    szLOD_Name,
                    zPCHAR    szGroupBoxName );


zOPER_EXPORT zSHORT OPERATION
SELECT_UPD_PotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_UPD_PotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_FindDetailObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_FindPotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_FindPotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_TypeForSelectedGroups( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_ListPotAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_ListSelAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_DetailPotAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_DetailSelAttributes( zVIEW     ViewToWindow );


static zSHORT
o_BuildAD_GroupPotAttrList( zVIEW     TZADWWKO,
                            zVIEW     SelectedLOD,
                            zPCHAR    szStartEntityName,
                            zPCHAR    szFlatListNameSuffix );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_MPG_Pages( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_AD_GroupUpdate( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignFindDetailDialog( zVIEW     ViewToWindow );


static zVOID
o_ReturnAttrControlType( zVIEW     AnyView,
                         zPCHAR    szDomainName,
                         zPCHAR    szDomainType,
                         zPCHAR    szControlType,
                         zPCHAR    szUpdateFlag );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_SearchAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_SearchAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_IncludeAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_IncludeAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SETUP_ESGL_MainIncludeObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_MainInclAttributes( zVIEW     ViewToWindow );


static zSHORT
o_AddRegisteredViewName( zVIEW     TZWINDOW,
                         zVIEW     TZZOLODO,
                         zPCHAR    szObjectViewName );


zOPER_EXPORT zSHORT OPERATION
PostbuildAD_MultiGroupPage( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_MultiPageDetailObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
OVERRIDE_ControlsForDetail( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
OVERRIDE_ControlsForList1( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroupObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_PromptCSS_ClassForAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_CtrlCSS_ClassForAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroupTemplate( zVIEW     ViewToWindow );


static zLONG
o_CloneControlAD( zVIEW     ViewToWindow,
                  zVIEW     TZCONTROL,
                  zVIEW     TZWINDOWL,
                  zVIEW     AD_BaseCtl );


static zVOID
o_BuildAD_GroupPotAttrListR( zVIEW     TZADWWKO,
                             zVIEW     MainLOD,
                             zVIEW     RecursiveLOD,
                             zPCHAR    szStartEntityName,
                             zPCHAR    szFlatListNameSuffix,
                             zLONG     lCurrentLevel,
                             zLONG     lStartLevel,
                             zPCHAR    szIndentSpaces );


zOPER_EXPORT zSHORT OPERATION
LIST_AllLOD_FlatAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeFrom( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignObjectFindSubdlg( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_FindReturnedListAttribs( zVIEW     ViewToWindow );


static zSHORT
o_InitializeAD_WorkObjectBootstra( zVIEW     ViewToWindow );


static zLONG
o_FindControlTypeWithinGroup( zVIEW     TempCtrl );


static zSHORT
o_ActivateAD_BaseL( zPVIEW    RetAD_BaseView,
                    zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_FindReturnedCL_Attribs( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildGroupListSpec( zVIEW     ViewToWindow );


static zLONG
o_FindGridWithinGroup( zVIEW     TempCtrl );


zOPER_EXPORT zSHORT OPERATION
CANCEL_AD_GroupSpec( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_ExtensionOptions( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_RecipientEmailAddress( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_Extensions( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_GroupDetailWInclude( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACTIVATE_AutodesignDefinition( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ContactListType( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
VALIDATE_DetailAttributeDef( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
VALIDATE_SearchAttributeDef( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_AutodesignMultiPage( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignSubgroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_AD_GroupSpecification( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_NewGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildGroupIncludeSpec( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CHECK_ReorderAttributesSub( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_AD_AttributeAsRangeSub( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeFromSub( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeToSub( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_MultiGroupSubSpec( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_MultiPageMGP_Object( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignCRM_Subdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ExactMatchAttribute( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_DashboardGroup( zVIEW     ViewToWindow );


//:LOCAL OPERATION
//:InitializeAD_WorkObjectReg( VIEW ViewToWindow )

//:   VIEW TZWINDOWL REGISTERED AS TZWINDOWL
static zSHORT
o_InitializeAD_WorkObjectReg( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZADWWKO  REGISTERED AS TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW TempCtrl  BASED ON LOD  TZWDLGSO
   zVIEW     TempCtrl = 0; 
   //:VIEW TZZOLFLO  BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 20 ) szControlDefTag
   zCHAR     szControlDefTag[ 21 ] = { 0 }; 
   //:STRING ( 20 ) szControlType
   zCHAR     szControlType[ 21 ] = { 0 }; 
   //:STRING ( 50 ) szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szTextPrompt
   zCHAR     szTextPrompt[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szTextCSS_Class
   zCHAR     szTextCSS_Class[ 51 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_1[ 255 ]; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set up work data for "Regular Autodesign Group".
   //:// Also build mapping data for any existing Controls.

   //:// The autodesign work objects need to be set up.
   //:// Default state will be resize and reposition top Groups.
   //:TZADWWKO.AutoDesignWork.RepositionGroupFlag = "Y"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "RepositionGroupFlag", "Y" );

   //:// GroupAreaName is set from Text on outmost Control.
   //:CreateViewFromView( TempCtrl, TZCONTROL )
   CreateViewFromView( &TempCtrl, TZCONTROL );
   //:NAME VIEW TempCtrl "TempCtrl"
   SetNameForView( TempCtrl, "TempCtrl", 0, zLEVEL_TASK );
   //:ResetViewFromSubobject( TempCtrl )
   ResetViewFromSubobject( TempCtrl );
   //:ResetViewFromSubobject( TempCtrl )
   ResetViewFromSubobject( TempCtrl );
   //:TZADWWKO.EntitySubGroup.GroupAreaName = TempCtrl.Control.Text 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "GroupAreaName", TempCtrl, "Control", "Text" );
   //:DropView( TempCtrl )
   DropView( TempCtrl );

   //:// If the current Group has a subcontrol that is not a GroupBox, then set the flag to indicate
   //:// the Autodesign is a redesign of the Group and set up the object fields and mapping. 
   //:// Also delete the current controls that do not have Actions for for UpdateGroupOnly.
   //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = ""
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "" );
   //:IF TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:szControlDefTag = TZCONTROL.ControlDef.Tag 
      GetVariableFromAttribute( szControlDefTag, 0, 'S', 21, TZCONTROL, "ControlDef", "Tag", "", 0 );
      //:IF szControlDefTag != "GroupBox"
      if ( ZeidonStringCompare( szControlDefTag, 1, 0, "GroupBox", 1, 0, 21 ) != 0 )
      { 
         //:// The Group has controls, so this is a redesign.
         //:// If it's a Grid, process accordingly.
         //:// If it's not a Grid, look for the first Control with mapping.
         //:IF szControlDefTag = "Grid"
         if ( ZeidonStringCompare( szControlDefTag, 1, 0, "Grid", 1, 0, 21 ) == 0 )
         { 

            //:// Initialize data for a Grid.
            //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
            SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" );
            //:TZADWWKO.EntitySubGroup.GroupType                     = "ListGroupOnly"
            SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "ListGroupOnly" );
            //:TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName  = TZCONTROL.CtrlMapLOD_Entity.Name 
            SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", TZCONTROL, "CtrlMapLOD_Entity", "Name" );
            //:TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName   = "Simple Grid"
            SetAttributeFromString( TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName", "Simple Grid" );
            //:CREATE ENTITY TZADWWKO.AutodesignGroupTemplate 
            RESULT = CreateEntity( TZADWWKO, "AutodesignGroupTemplate", zPOS_AFTER );
            //:TZADWWKO.AutodesignGroupTemplate.GroupName = "Simple Grid"
            SetAttributeFromString( TZADWWKO, "AutodesignGroupTemplate", "GroupName", "Simple Grid" );

            //:// Activate Object View and LOD.
            //:szViewName = TZCONTROL.CtrlMapView.Name 
            GetVariableFromAttribute( szViewName, 0, 'S', 51, TZCONTROL, "CtrlMapView", "Name", "", 0 );
            //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 )
            nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 );
            //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
            SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
            //:IF TZADWWKO.W_MetaDefUpdateView EXISTS
            lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
            if ( lTempInteger_1 == 0 )
            { 
               //:EXCLUDE TZADWWKO.W_MetaDefUpdateView  
               RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateView", zREPOS_AFTER );
            } 

            //:END 
            //:SET CURSOR FIRST TaskLPLR.W_MetaType WHERE TaskLPLR.W_MetaType.Type = 9   // Read view of Registered View
            RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 9, "" );
            //:SET CURSOR FIRST TaskLPLR.W_MetaDef  WHERE TaskLPLR.W_MetaDef.Name  = UpdateVOR.ViewObjRef.Name  
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateVOR, "ViewObjRef", "Name" );
            RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
            //:INCLUDE TZADWWKO.W_MetaDefUpdateView  FROM TaskLPLR.W_MetaDef 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TaskLPLR, "W_MetaDef", zPOS_AFTER );
            //:szLOD_Name = UpdateVOR.LOD.Name 
            GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
            //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
            nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
            //:NAME VIEW UpdateLOD "TZZOLODO_Update"
            SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
            //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
            RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
            //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = UpdateLOD.LOD.Name   
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
            RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
            //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

            //:// Build the list of selected attributes for the new Grid and delete Grid subcontrols except for the first and buttons.
            //:INCLUDE TZADWWKO.ESG_LOD_Entity FROM TZCONTROL.CtrlMapLOD_Entity 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_LOD_Entity", TZCONTROL, "CtrlMapLOD_Entity", zPOS_AFTER );
            //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
            SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:Count = 0
            Count = 0;
            //:FOR EACH TZCONTROL.Control 
            RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:Count = Count + 1
               Count = Count + 1;
               //:IF TZCONTROL.CtrlMapLOD_Attribute EXISTS
               lTempInteger_2 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Attribute" );
               if ( lTempInteger_2 == 0 )
               { 
                  //:// Make sure there wasn't a duplicate Subcontrol.
                  //:SET CURSOR FIRST TZADWWKO.ESG_ListLOD_Attribute WHERE TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TZCONTROL.Control.Text
                  GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZCONTROL, "Control", "Text" );
                  RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", szTempString_1, "" );
                  //:IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //:SET CURSOR LAST TZADWWKO.ESG_ListLOD_Attribute
                     RESULT = SetCursorLastEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
                     //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZCONTROL.CtrlMapLOD_Attribute 
                     RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZCONTROL, "CtrlMapLOD_Attribute", zPOS_AFTER );
                     //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TZCONTROL.Control.Text 
                     SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZCONTROL, "Control", "Text" );

                     //:// ControlType of "" becomes either Text or EditBox Control depending on WebControlProperty.
                     //:szControlType = TZCONTROL.ControlDef.Tag
                     GetVariableFromAttribute( szControlType, 0, 'S', 21, TZCONTROL, "ControlDef", "Tag", "", 0 );
                     //:IF szControlType = "GridEditCtl"
                     if ( ZeidonStringCompare( szControlType, 1, 0, "GridEditCtl", 1, 0, 21 ) == 0 )
                     { 
                        //:SET CURSOR FIRST TZCONTROL.WebControlProperty WHERE TZCONTROL.WebControlProperty.Name = "Edit"
                        RESULT = SetCursorFirstEntityByString( TZCONTROL, "WebControlProperty", "Name", "Edit", "" );
                        //:IF RESULT >= zCURSOR_SET
                        if ( RESULT >= zCURSOR_SET )
                        { 
                           //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "EditBox"
                           SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "EditBox" );
                           //:ELSE
                        } 
                        else
                        { 
                           //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Text"
                           SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Text" );
                        } 

                        //:END 
                        //:ELSE
                     } 
                     else
                     { 
                        //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = szControlType
                        SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", szControlType );
                     } 

                     //:END
                  } 

                  //:END
               } 

               RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
               //:END 
            } 

            //:END
            //:ResetViewFromSubobject( TZCONTROL )
            ResetViewFromSubobject( TZCONTROL );

            //:// Build the list of selectable LOD Entities for a potential list box.
            //:// We have one entry in the list for each Entity in the Update Object.
            //:// First remove any current entries then rebuild them.
            //:FOR EACH TZADWWKO.PotentialLOD_Entity 
            RESULT = SetCursorFirstEntity( TZADWWKO, "PotentialLOD_Entity", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:EXCLUDE TZADWWKO.PotentialLOD_Entity NONE 
               RESULT = ExcludeEntity( TZADWWKO, "PotentialLOD_Entity", zREPOS_NONE );
               RESULT = SetCursorNextEntity( TZADWWKO, "PotentialLOD_Entity", "" );
            } 

            //:END
            //:FOR EACH UpdateLOD.LOD_Entity 
            RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:INCLUDE TZADWWKO.PotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
               RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "PotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
               RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
            } 

            //:END

            //:// Delete any current potential attributes and then go to build them.
            //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
            o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_0, "" );

            //:ELSE
         } 
         else
         { 

            //:// Update/Detail Controls Group
            //:// The layout of the Group has to be Text field followed by a Control with mapping, though it can be preceded by one or more buttons.
            //:SET CURSOR FIRST TZCONTROL.Control WHERE TZCONTROL.ControlDef.Tag = "Text"
            RESULT = SetCursorFirstEntityByString( TZCONTROL, "ControlDef", "Tag", "Text", "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:IssueError( ViewToWindow,0,0, "The Group layout is not valid. You must Cancel the Autodesign process." ) 
               IssueError( ViewToWindow, 0, 0, "The Group layout is not valid. You must Cancel the Autodesign process." );
               //:RETURN
               return( 0 );
            } 

            //:END 

            //:// Make sure the next Control has mapping.
            //:SET CURSOR NEXT TZCONTROL.Control 
            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            //:IF TZCONTROL.CtrlMapLOD_Attribute DOES NOT EXIST
            lTempInteger_3 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Attribute" );
            if ( lTempInteger_3 != 0 )
            { 
               //:IssueError( ViewToWindow,0,0, "The Group layout is not valid. You must Cancel the Autodesign process." ) 
               IssueError( ViewToWindow, 0, 0, "The Group layout is not valid. You must Cancel the Autodesign process." );
               //:RETURN
               return( 0 );
            } 

            //:END

            //:// Set up the Mapping Object from the Control Mapping.
            //:// Initialize data for Update/Detail Controls.
            //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
            SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" );
            //:TZADWWKO.EntitySubGroup.GroupType                     = "DetailGroupUpdate"
            SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroupUpdate" );
            //:TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName   = "GroupDetail"
            SetAttributeFromString( TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName", "GroupDetail" );
            //:CREATE ENTITY TZADWWKO.AutodesignGroupTemplate 
            RESULT = CreateEntity( TZADWWKO, "AutodesignGroupTemplate", zPOS_AFTER );
            //:TZADWWKO.AutodesignGroupTemplate.GroupName = "GroupDetail"
            SetAttributeFromString( TZADWWKO, "AutodesignGroupTemplate", "GroupName", "GroupDetail" );

            //:// Activate Object View and LOD.
            //:szViewName = TZCONTROL.CtrlMapView.Name 
            GetVariableFromAttribute( szViewName, 0, 'S', 51, TZCONTROL, "CtrlMapView", "Name", "", 0 );
            //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 )
            nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 );
            //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
            SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
            //:IF TZADWWKO.W_MetaDefUpdateView EXISTS
            lTempInteger_4 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
            if ( lTempInteger_4 == 0 )
            { 
               //:EXCLUDE TZADWWKO.W_MetaDefUpdateView  
               RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateView", zREPOS_AFTER );
            } 

            //:END 
            //:SET CURSOR FIRST TaskLPLR.W_MetaType WHERE TaskLPLR.W_MetaType.Type = 9   // Read view of Registered View
            RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 9, "" );
            //:SET CURSOR FIRST TaskLPLR.W_MetaDef  WHERE TaskLPLR.W_MetaDef.Name  = UpdateVOR.ViewObjRef.Name  
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), UpdateVOR, "ViewObjRef", "Name" );
            RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_1, "" );
            //:INCLUDE TZADWWKO.W_MetaDefUpdateView  FROM TaskLPLR.W_MetaDef 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TaskLPLR, "W_MetaDef", zPOS_AFTER );
            //:szLOD_Name = UpdateVOR.LOD.Name 
            GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
            //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
            nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
            //:NAME VIEW UpdateLOD "TZZOLODO_Update"
            SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
            //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
            RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
            //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = UpdateLOD.LOD.Name   
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), UpdateLOD, "LOD", "Name" );
            RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_1, "" );
            //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

            //:// Go back to the Text Control.
            //:SET CURSOR PREVIOUS TZCONTROL.Control 
            RESULT = SetCursorPrevEntity( TZCONTROL, "Control", "" );

            //:// Process each pair of Text/Mapping Controls, creating the ESG_ListLOD_Attribute entries.
            //:RESULT = 0
            RESULT = 0;
            //:LOOP WHILE RESULT >= 0
            while ( RESULT >= 0 )
            { 
               //:szTextPrompt    = TZCONTROL.Control.Text 
               GetVariableFromAttribute( szTextPrompt, 0, 'S', 51, TZCONTROL, "Control", "Text", "", 0 );
               //:szTextCSS_Class = TZCONTROL.Control.CSS_Class 
               GetVariableFromAttribute( szTextCSS_Class, 0, 'S', 51, TZCONTROL, "Control", "CSS_Class", "", 0 );
               //:SET CURSOR NEXT TZCONTROL.Control       // Position on Mapping Control
               RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
               //:IF TZCONTROL.CtrlMapLOD_Attribute DOES NOT EXIST
               lTempInteger_5 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Attribute" );
               if ( lTempInteger_5 != 0 )
               { 
                  //:IssueError( ViewToWindow,0,0, "The Group layout is not valid. You must Cancel the Autodesign process." ) 
                  IssueError( ViewToWindow, 0, 0, "The Group layout is not valid. You must Cancel the Autodesign process." );
                  //:RETURN
                  return( 0 );
               } 

               //:END
               //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZCONTROL.CtrlMapLOD_Attribute 
               RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZCONTROL, "CtrlMapLOD_Attribute", zPOS_AFTER );
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue         = szTextPrompt
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", szTextPrompt );
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType         = TZCONTROL.ControlDef.Tag
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZCONTROL, "ControlDef", "Tag" );
               //:TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForPrompt  = szTextCSS_Class
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForPrompt", szTextCSS_Class );
               //:TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForControl = TZCONTROL.Control.CSS_Class 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForControl", TZCONTROL, "Control", "CSS_Class" );

               //:SET CURSOR NEXT TZCONTROL.Control       // Position on next Text Control.
               RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            } 

            //:END 

            //:// Delete any current potential attributes and then go to build them.
            //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )    
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), UpdateLOD, "LOD_EntityParent", "Name" );
            o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_1, "" );
         } 

         //:      
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_AutodesignMultiPageSub( VIEW ViewToWindow )

//:   VIEW TZADWWKOSub  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CANCEL_AutodesignMultiPageSub( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKOSub = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOMain BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOMain = 0; 

   RESULT = GetViewByName( &TZADWWKOSub, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Accept the TZADWWKO root entity (it's versioned) and rename the 2 views back again.
   //:CancelSubobject( TZADWWKOSub, "AutoDesignWork" )
   CancelSubobject( TZADWWKOSub, "AutoDesignWork" );
   //:SetNameForView( TZADWWKOSub, "TZADWWKOSub", ViewToWindow, zLEVEL_TASK )
   SetNameForView( TZADWWKOSub, "TZADWWKOSub", ViewToWindow, zLEVEL_TASK );
   //:GET VIEW TZADWWKOMain NAMED "TZADWWKOMain"
   RESULT = GetViewByName( &TZADWWKOMain, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
   //:NAME VIEW TZADWWKOMain "TZADWWKO"
   SetNameForView( TZADWWKOMain, "TZADWWKO", 0, zLEVEL_TASK );
   //:DropNameForView( TZADWWKOMain, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK )
   DropNameForView( TZADWWKOMain, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_AutodesignMultiPageSub( VIEW ViewToWindow )

//:   VIEW TZADWWKOSub  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_AutodesignMultiPageSub( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKOSub = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOMain BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOMain = 0; 

   RESULT = GetViewByName( &TZADWWKOSub, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that each EntitySubGroup has an Area Name specified.
   //:FOR EACH TZADWWKOSub.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKOSub, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKOSub.EntitySubGroup.GroupAreaName = ""
      if ( CompareAttributeToString( TZADWWKOSub, "EntitySubGroup", "GroupAreaName", "" ) == 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Set Group Values",
         //:             "Each Group selected must have an Area Name specified.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Set Group Values", "Each Group selected must have an Area Name specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKOSub, "EntitySubGroup", "" );
      //:END 
   } 

   //:END

   //:// Accept the TZADWWKO root entity (it's versioned) and rename the 2 views back again.
   //:AcceptSubobject( TZADWWKOSub, "AutoDesignWork" )
   AcceptSubobject( TZADWWKOSub, "AutoDesignWork" );
   //:SetNameForView( TZADWWKOSub, "TZADWWKOSub", ViewToWindow, zLEVEL_TASK )
   SetNameForView( TZADWWKOSub, "TZADWWKOSub", ViewToWindow, zLEVEL_TASK );
   //:GET VIEW TZADWWKOMain NAMED "TZADWWKOMain"
   RESULT = GetViewByName( &TZADWWKOMain, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
   //:NAME VIEW TZADWWKOMain "TZADWWKO"
   SetNameForView( TZADWWKOMain, "TZADWWKO", 0, zLEVEL_TASK );
   //:DropNameForView( TZADWWKOMain, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK )
   DropNameForView( TZADWWKOMain, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SAVE_AutodesignDefinition( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SAVE_AutodesignDefinition( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TaskLPLR REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:STRING ( 200 ) szFullFileName
   zCHAR     szFullFileName[ 201 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 255 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Save the TZADWWKO OI in the current LPLR under the name specified.
   //:// It will overlay any current file by the same name.
   //:szFullFileName = TaskLPLR.LPLR.MetaSrcDir + "\" + TZADWWKO.AutoDesignWork.StoredDesignSavedName + ".ADD"
   GetStringFromAttribute( szFullFileName, zsizeof( szFullFileName ), TaskLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szFullFileName, 1, 0, "\\", 1, 0, 201 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 255, TZADWWKO, "AutoDesignWork", "StoredDesignSavedName", "", 0 );
   ZeidonStringConcat( szFullFileName, 1, 0, szTempString_0, 1, 0, 201 );
   ZeidonStringConcat( szFullFileName, 1, 0, ".ADD", 1, 0, 201 );
   //:TraceLineS( "*** AD Def File Name: ", szFullFileName )
   TraceLineS( "*** AD Def File Name: ", szFullFileName );
   //:nRC = CommitOI_ToFile( TZADWWKO, szFullFileName, zASCII )
   nRC = CommitOI_ToFile( TZADWWKO, szFullFileName, zASCII );
   //:TraceLineI( "Commit nRC: ", nRC )
   TraceLineI( "Commit nRC: ", nRC );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Def Save", 
      //:             "An error occurred during Commit and the Autodesign Definition was not saved.", 
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Def Save", "An error occurred during Commit and the Autodesign Definition was not saved.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_FindReturnedCL_Attribs( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_FindReturnedCL_Attribs( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADWWKO.FlatSelectedListAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "FlatSelectedListAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "FlatSelectedListAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:DELETE ENTITY TZADWWKO.FlatSelectedListAttribute NONE
         RESULT = DeleteEntity( TZADWWKO, "FlatSelectedListAttribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_FindReturnedListAttribs( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_FindReturnedListAttribs( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO2 BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO2 = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Copy selected attributes from ESG_FlatListPotentialAttribute2 entries to FlatSelectedSearchAttribute entries at the current postiion..
   //:CreateViewFromView( TZADWWKO2, TZADWWKO )
   CreateViewFromView( &TZADWWKO2, TZADWWKO );
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute2 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO2.FlatSelectedListAttribute 
         //:           WHERE TZADWWKO2.FlatSelectedListAttribute.EntityName    = TZADWWKO.ESG_FlatListPotentialAttribute2.EntityName  
         //:             AND TZADWWKO2.FlatSelectedListAttribute.AttributeName = TZADWWKO.ESG_FlatListPotentialAttribute2.AttributeName 
         RESULT = SetCursorFirstEntity( TZADWWKO2, "FlatSelectedListAttribute", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedListAttribute", "EntityName", TZADWWKO, "ESG_FlatListPotentialAttribute2", "EntityName" ) != 0 ||
                    CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedListAttribute", "AttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute2", "AttributeName" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZADWWKO2, "FlatSelectedListAttribute", "" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY TZADWWKO.FlatSelectedListAttribute  
            RESULT = CreateEntity( TZADWWKO, "FlatSelectedListAttribute", zPOS_AFTER );
            //:INCLUDE TZADWWKO.FlatListLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatListLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
            //:TZADWWKO.FlatSelectedListAttribute.EntityName    = TZADWWKO.ESG_FlatListPotentialAttribute2.EntityName 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "EntityName", TZADWWKO, "ESG_FlatListPotentialAttribute2", "EntityName" );
            //:TZADWWKO.FlatSelectedListAttribute.AttributeName = TZADWWKO.ESG_FlatListPotentialAttribute2.AttributeName 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "AttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute2", "AttributeName" );
            //:// We will use the list title value if it exists. Otherwise, we will use the Prompt value.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleValue != ""
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleValue", "" ) != 0 )
            { 
               //:TZADWWKO.FlatSelectedListAttribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleValue 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleValue" );
               //:TZADWWKO.FlatSelectedListAttribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleLength 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleLength" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.FlatSelectedListAttribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptValue 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptValue" );
               //:TZADWWKO.FlatSelectedListAttribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptLength 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptLength" );
            } 

            //:END
            //:TZADWWKO.FlatSelectedListAttribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlType" );
            //:TZADWWKO.FlatSelectedListAttribute.ControlWidth = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlWidth 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlWidth" );
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
      //:END
   } 

   //:END
   //:DropView( TZADWWKO2 )
   DropView( TZADWWKO2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_AD_AttributeAsRange( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_AD_AttributeAsRange( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove the Range data from the FlatSelectedSearchAttribute entry.
   //:TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName = ""
   SetAttributeFromString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName", "" );
   //:TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType          = ""
   SetAttributeFromString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_AD_AttributeAsRangeTo( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeTo( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set the two Range values in FlatSelectedSearchAttribute from those in ESG_FlatListPotentialAttribute and
   //:// indicate the type is "To".

   //:// Make sure that Data Types match between source and target.
   //:IF TZADWWKO.FlatSelectedSearchAttribute.DataType != TZADWWKO.ESG_FlatDomain.DataType 
   if ( CompareAttributeToAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "DataType", TZADWWKO, "ESG_FlatDomain", "DataType" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Data Types of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Data Types of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure that Entity Names match between source and target.
   //:IF TZADWWKO.FlatSelectedSearchAttribute.EntityName != TZADWWKO.ESG_FlatListPotentialAttribute.EntityName 
   if ( CompareAttributeToAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "EntityName", TZADWWKO, "ESG_FlatListPotentialAttribute", "EntityName" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Entity Names of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Entity Names of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Set values in target.
   //:TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName = TZADWWKO.ESG_FlatListPotentialAttribute.AttributeName 
   SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute", "AttributeName" );
   //:TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType          = "To"
   SetAttributeFromString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "To" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_ESGL_MainInclAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_MainInclAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Include entries.
   //:// The selected Include attributes are defined in subobject ESG_ListLOD_Attribute.
   //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:EXCLUDE TZADWWKO.ESG_ListLOD_Attribute NONE
         RESULT = ExcludeEntity( TZADWWKO, "ESG_ListLOD_Attribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_GroupListWInclude( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_GroupListWInclude( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// For ListGroupwInclude Type, make sure Include MetaDef entry has been selected.
   //:IF TZADWWKO.AutoDesignWork.SelectedGroupType = ""
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedGroupType", "" ) == 0 )
   { 
      //:// Group Type is specified in EntitySubGroup.GroupType.
      //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
      GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:// Group Type is specified in AutoDesignWork.SelectedGroupType.
      //:szGroupType = TZADWWKO.AutoDesignWork.SelectedGroupType 
      GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "AutoDesignWork", "SelectedGroupType", "", 0 );
   } 

   //:END
   //:IF szGroupType = "ListGroupwInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 )
   { 
      //:IF TZADWWKO.ESG_ListIncludeW_MetaDef DOES NOT EXIST
      lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
      if ( lTempInteger_0 != 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "An Include object must be selected.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "An Include object must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// For the others, make sure that both the Search and Include MetaDef entries have been selected.
      //:IF TZADWWKO.ESG_ListSearchW_MetaDef DOES NOT EXIST OR 
      lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
      //:   TZADWWKO.ESG_ListIncludeW_MetaDef DOES NOT EXIST
      lTempInteger_2 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
      if ( lTempInteger_1 != 0 || lTempInteger_2 != 0 )
      { 

         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "Both a Search and Include object must be selected.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a Search and Include object must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:END

   //:// Make sure both Group Area Name and Area Title have been selected because they are required for Bootstrap.
   //:IF TZADWWKO.EntitySubGroup.GroupAreaName  = "" OR 
   //:   TZADWWKO.EntitySubGroup.GroupAreaTitle = ""
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaName", "" ) == 0 || CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "" ) == 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both a Group Area Name and Title must be specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a Group Area Name and Title must be specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:AcceptSubobject( TZADWWKO, "EntitySubGroup" )
   AcceptSubobject( TZADWWKO, "EntitySubGroup" );

   //:// The Action/Operation Suffix will be the same as the Group Area Name, at least for now.
   //:TZADWWKO.EntitySubGroup.IncludeOperationSuffix = TZADWWKO.EntitySubGroup.GroupAreaName 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix", TZADWWKO, "EntitySubGroup", "GroupAreaName" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_Group( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_Group( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Make sure both Group Area Name and Area Title have been selected because they are required for Bootstrap.
   //:IF TZADWWKO.EntitySubGroup.GroupAreaName  = "" OR 
   //:   TZADWWKO.EntitySubGroup.GroupAreaTitle = ""
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaName", "" ) == 0 || CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "" ) == 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both a Group Area Name and Title must be specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a Group Area Name and Title must be specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:AcceptSubobject( TZADWWKO, "EntitySubGroup" )
   AcceptSubobject( TZADWWKO, "EntitySubGroup" );

   //:// The Action/Operation Suffix will be the same as the Group Area Name, at least for now.
   //:TZADWWKO.EntitySubGroup.IncludeOperationSuffix = TZADWWKO.EntitySubGroup.GroupAreaName 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix", TZADWWKO, "EntitySubGroup", "GroupAreaName" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_IncludeObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO   REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_IncludeObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW IncludeLOD BASED ON LOD  TZZOLODO
   zVIEW     IncludeLOD = 0; 
   //:VIEW UpdateLOD  BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szVOR_Name
   zCHAR     szVOR_Name[ 33 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_NameInclude
   zCHAR     szLOD_NameInclude[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_NameUpdate
   zCHAR     szLOD_NameUpdate[ 51 ] = { 0 }; 
   //:INTEGER       nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// For the selected VOR, build the TZADWWKO.ESG_FlatListPotentialAttribute2 entries for each Entity/Attribute in the object.

   //:IF TZADWWKO.ESG_ListIncludeW_MetaDef DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
   if ( lTempInteger_0 != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "No Select List Object View was selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "No Select List Object View was selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Determine the Include LOD from the selected VOR.
   //:szVOR_Name = TZADWWKO.ESG_ListIncludeW_MetaDef.Name
   GetVariableFromAttribute( szVOR_Name, 0, 'S', 33, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szVOR_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szVOR_Name, 0 );
   //:NAME VIEW TZWDVORO "TZWDVOROInclude"
   SetNameForView( TZWDVORO, "TZWDVOROInclude", 0, zLEVEL_TASK );
   //:szLOD_Name = TZWDVORO.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 33, TZWDVORO, "LOD", "Name", "", 0 );
   //:TZADWWKO.ESG_ListIncludeW_MetaDef.wObjectName = szLOD_Name    // Save the associated LOD Name.
   SetAttributeFromString( TZADWWKO, "ESG_ListIncludeW_MetaDef", "wObjectName", szLOD_Name );
   //:DropObjectInstance( TZWDVORO )
   DropObjectInstance( TZWDVORO );

   //:// Activate the selected Include Object.
   //:GET VIEW IncludeLOD NAMED "ESGL_IncludeLOD"
   RESULT = GetViewByName( &IncludeLOD, "ESGL_IncludeLOD", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:// Drop any current IncludeLOD view, as well as the object instance if it isn't also the Update LOD.
      //:szLOD_NameInclude = IncludeLOD.LOD.Name 
      GetVariableFromAttribute( szLOD_NameInclude, 0, 'S', 51, IncludeLOD, "LOD", "Name", "", 0 );
      //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
      RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
      //:szLOD_NameUpdate = UpdateLOD.LOD.Name 
      GetVariableFromAttribute( szLOD_NameUpdate, 0, 'S', 51, UpdateLOD, "LOD", "Name", "", 0 );
      //:IF szLOD_NameUpdate = szLOD_NameInclude
      if ( ZeidonStringCompare( szLOD_NameUpdate, 1, 0, szLOD_NameInclude, 1, 0, 51 ) == 0 )
      { 
         //:// Both views are for same object, so don't drop the include object or we'll drop the update object.
         //:DropNameForView( IncludeLOD, "ESGL_IncludeLOD", ViewToWindow, zLEVEL_TASK )
         DropNameForView( IncludeLOD, "ESGL_IncludeLOD", ViewToWindow, zLEVEL_TASK );
         //:ELSE
      } 
      else
      { 
         //:DropObjectInstance( IncludeLOD )
         DropObjectInstance( IncludeLOD );
      } 

      //:END
   } 

   //:END
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, IncludeLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &IncludeLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW IncludeLOD "ESGL_IncludeLOD"
   SetNameForView( IncludeLOD, "ESGL_IncludeLOD", 0, zLEVEL_TASK );
   //:TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName = IncludeLOD.LOD_EntityParent.Name 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", IncludeLOD, "LOD_EntityParent", "Name" );

   //:// Build the ESG_FlatListPotentialAttribute entries for the Potential Select List Attributes.
   //:BuildAD_GroupPotAttrList( TZADWWKO, IncludeLOD, IncludeLOD.LOD_EntityParent.Name, "2" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), IncludeLOD, "LOD_EntityParent", "Name" );
   o_BuildAD_GroupPotAttrList( TZADWWKO, IncludeLOD, szTempString_0, "2" );

   //:// Build the ESG_FlatListPotentialEntity subobject entries for the 3 Entity include select lists.
   //:// First delete any current entries then rebuild them.

   //:// Select List Entity
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialEntity NONE
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialEntity", "" );
   } 

   //:END
   //:FOR EACH IncludeLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( IncludeLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.ESG_FlatListPotentialEntity 
      RESULT = CreateEntity( TZADWWKO, "ESG_FlatListPotentialEntity", zPOS_AFTER );
      //:TZADWWKO.ESG_FlatListPotentialEntity.EntityName = IncludeLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADWWKO, "ESG_FlatListPotentialEntity", "EntityName", IncludeLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADWWKO.ESG_FlatListPotentialLOD_Entity FROM IncludeLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_FlatListPotentialLOD_Entity", IncludeLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADWWKO, "ESG_FlatListPotentialEntity", IncludeLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADWWKO, "ESG_FlatListPotentialEntity", IncludeLOD, "LOD_Entity", zSET_NULL );
      RESULT = SetCursorNextEntity( IncludeLOD, "LOD_Entity", "" );
   } 

   //:END

   //:// Target Include Entity
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialEntity2 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialEntity2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialEntity2 NONE
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialEntity2", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialEntity2", "" );
   } 

   //:END
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.ESG_FlatListPotentialEntity2 
      RESULT = CreateEntity( TZADWWKO, "ESG_FlatListPotentialEntity2", zPOS_AFTER );
      //:TZADWWKO.ESG_FlatListPotentialEntity2.EntityName = UpdateLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADWWKO, "ESG_FlatListPotentialEntity2", "EntityName", UpdateLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADWWKO.ESG_FlatListPotentialLOD_Entity2 FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_FlatListPotentialLOD_Entity2", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADWWKO, "ESG_FlatListPotentialEntity2", UpdateLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADWWKO, "ESG_FlatListPotentialEntity2", UpdateLOD, "LOD_Entity", zSET_NULL );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END

   //:// Source Include Entity
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialEntity3 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialEntity3", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialEntity3 NONE
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialEntity3", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialEntity3", "" );
   } 

   //:END
   //:FOR EACH IncludeLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( IncludeLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.ESG_FlatListPotentialEntity3 
      RESULT = CreateEntity( TZADWWKO, "ESG_FlatListPotentialEntity3", zPOS_AFTER );
      //:TZADWWKO.ESG_FlatListPotentialEntity3.EntityName = IncludeLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADWWKO, "ESG_FlatListPotentialEntity3", "EntityName", IncludeLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADWWKO.ESG_FlatListPotentialLOD_Entity3 FROM IncludeLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_FlatListPotentialLOD_Entity3", IncludeLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADWWKO, "ESG_FlatListPotentialEntity3", IncludeLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADWWKO, "ESG_FlatListPotentialEntity3", IncludeLOD, "LOD_Entity", zSET_NULL );
      RESULT = SetCursorNextEntity( IncludeLOD, "LOD_Entity", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_SearchObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_SearchObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_Base   REGISTERED AS AD_Base
   zVIEW     AD_Base = 0; 
   //:VIEW SearchLOD BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW TZWDVORO  BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szVOR_Name
   zCHAR     szVOR_Name[ 33 ] = { 0 }; 
   //:INTEGER       nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );

   //:// For the selected VOR, build the TZADWWKO.ESG_FlatListPotentialAttribute3 entries for each Entity/Attribute in the object.

   //:IF TZADWWKO.ESG_ListSearchW_MetaDef DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
   if ( lTempInteger_0 != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "No Search Object View was selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "No Search Object View was selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Determine the Search LOD from the selected VOR.
   //:szVOR_Name = TZADWWKO.ESG_ListSearchW_MetaDef.Name
   GetVariableFromAttribute( szVOR_Name, 0, 'S', 33, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szVOR_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szVOR_Name, 0 );
   //:NAME VIEW TZWDVORO "TZWDVOROSearch"
   SetNameForView( TZWDVORO, "TZWDVOROSearch", 0, zLEVEL_TASK );
   //:szLOD_Name = TZWDVORO.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 33, TZWDVORO, "LOD", "Name", "", 0 );
   //:TZADWWKO.ESG_ListSearchW_MetaDef.wObjectName = szLOD_Name    // Save the associated LOD Name.
   SetAttributeFromString( TZADWWKO, "ESG_ListSearchW_MetaDef", "wObjectName", szLOD_Name );
   //:DropObjectInstance( TZWDVORO )
   DropObjectInstance( TZWDVORO );

   //:// Activate the selected Search Object.
   //:GET VIEW SearchLOD NAMED "ESGL_SearchLOD"
   RESULT = GetViewByName( &SearchLOD, "ESGL_SearchLOD", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( SearchLOD )
      DropObjectInstance( SearchLOD );
   } 

   //:END
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW SearchLOD "ESGL_SearchLOD"
   SetNameForView( SearchLOD, "ESGL_SearchLOD", 0, zLEVEL_TASK );

   //:// Build the ESG_FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
   //:BuildAD_GroupPotAttrList( TZADWWKO, SearchLOD, SearchLOD.LOD_EntityParent.Name, "3" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), SearchLOD, "LOD_EntityParent", "Name" );
   o_BuildAD_GroupPotAttrList( TZADWWKO, SearchLOD, szTempString_0, "3" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_FindSearchObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_FindSearchObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW SearchLOD BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW TZWINDOWL BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOWL = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zLONG     lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// For the selected object, build the TZADWWKO.ESG_FlatListPotentialAttribute entries for each Entity/Attribute in the object.

   //:// Make sure a Search object is selected.
   //:IF TZADWWKO.W_MetaDefSearchObject DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefSearchObject" );
   if ( lTempInteger_0 != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Search Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Search Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Activate the selected Search Object.
   //:GET VIEW SearchLOD NAMED "TZZOLOD1"
   RESULT = GetViewByName( &SearchLOD, "TZZOLOD1", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( SearchLOD )
      DropObjectInstance( SearchLOD );
   } 

   //:END
   //:szLOD_Name = TZADWWKO.W_MetaDefSearchObject.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKO, "W_MetaDefSearchObject", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW SearchLOD "TZZOLOD1" 
   SetNameForView( SearchLOD, "TZZOLOD1", 0, zLEVEL_TASK );

   //:// If this is CRM Find, make the search object the returned object.
   //:IF TZADWWKO.AutodesignSubdialog.FindType = "CRM"
   if ( CompareAttributeToString( TZADWWKO, "AutodesignSubdialog", "FindType", "CRM" ) == 0 )
   { 
      //:INCLUDE TZADWWKO.W_MetaDefReturnedObject FROM TZADWWKO.W_MetaDefSearchObject 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefReturnedObject", TZADWWKO, "W_MetaDefSearchObject", zPOS_AFTER );
   } 

   //:END

   //:// Clear any existing TZADWWKO.ESG_FlatListPotentialAttribute entries and rebuild them from selected object.
   //:szEntityName = SearchLOD.LOD_EntityParent.Name 
   GetVariableFromAttribute( szEntityName, 0, 'S', 51, SearchLOD, "LOD_EntityParent", "Name", "", 0 );
   //:BuildAD_GroupPotAttrList( TZADWWKO, SearchLOD, szEntityName, "" )
   o_BuildAD_GroupPotAttrList( TZADWWKO, SearchLOD, szEntityName, "" );

   //:// Clear any existing TZADWWKO.PotentialExactMatchAttribute entries and rebuild them from the root Entity.
   //:SET CURSOR FIRST SearchLOD.LOD_Entity WHERE SearchLOD.LOD_Entity.ZKey = SearchLOD.LOD_EntityParent.ZKey  
   GetIntegerFromAttribute( &lTempInteger_1, SearchLOD, "LOD_EntityParent", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( SearchLOD, "LOD_Entity", "ZKey", lTempInteger_1, "" );
   //:FOR EACH SearchLOD.LOD_Attribute 
   RESULT = SetCursorFirstEntity( SearchLOD, "LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.PotentialExactMatchAttribute FROM SearchLOD.LOD_Attribute 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "PotentialExactMatchAttribute", SearchLOD, "LOD_Attribute", zPOS_AFTER );
      RESULT = SetCursorNextEntity( SearchLOD, "LOD_Attribute", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAutodesignFindSubdialog( VIEW ViewToWindow )

//:   VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignFindSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO    BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW CL_Domain   BASED ON LOD  TZDGSRCO
   zVIEW     CL_Domain = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Set up work data necessary for autodesigning an AutodesignSubdialog.

   //:// Make sure TZADWWKO and the related autodesign objects exist in memory.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// The autodesign work objects need to be set up.

      //:// The TZADWWKO itself
      //:ACTIVATE TZADWWKO EMPTY 
      RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
      //:NAME VIEW TZADWWKO "TZADWWKO"
      SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADWWKO.AutoDesignWork 
      RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
      RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.EntitySubGroup  
      RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
      //:InitializeL_SymbolType( TZADWWKO )
      oTZADWWKO_InitializeL_SymbolType( TZADWWKO );

      //:// Remove any existing Autodesign entries from TZWINDOWL.
      //:IF TZWINDOWL.AutoDesignWindow EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AutoDesignWindow" );
      if ( lTempInteger_0 == 0 )
      { 
         //:DELETE ENTITY TZWINDOWL.AutoDesignWindow  
         RESULT = DeleteEntity( TZWINDOWL, "AutoDesignWindow", zPOS_NEXT );
      } 

      //:END
      //:CREATE ENTITY TZWINDOWL.AutoDesignWindow 
      RESULT = CreateEntity( TZWINDOWL, "AutoDesignWindow", zPOS_AFTER );
      //:CREATE ENTITY TZWINDOWL.AutoDesignGroup  
      RESULT = CreateEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER );

      //:ELSE
   } 
   else
   { 
      //:// The Object exists, also make sure the AutoDesignWork entries exist.
      //:SET CURSOR FIRST TZADWWKO.AutoDesignWork
      RESULT = SetCursorFirstEntity( TZADWWKO, "AutoDesignWork", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CREATE ENTITY TZADWWKO.AutoDesignWork 
         RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
         //:CREATE ENTITY TZADWWKO.AutodesignSubdialog
         RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
      } 

      //:END
   } 

   //:END

   //:// If an update LOD exists, make sure that we also name it TZZOLODO_Desc for the combobox.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:NAME VIEW UpdateLOD "TZZOLODO_Desc"     // We need this named view for the Root select combobox.
      SetNameForView( UpdateLOD, "TZZOLODO_Desc", 0, zLEVEL_TASK );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_AutoDesignGroup( VIEW ViewToWindow )

//:   VIEW TZADWWKO   REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CANCEL_AutoDesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL  REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW CurrentLOD    BASED ON LOD  TZZOLODO
   zVIEW     CurrentLOD = 0; 
   //:VIEW CurrentVOR BASED ON LOD  TZWDVORO
   zVIEW     CurrentVOR = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );

   //:CancelSubobject( TZCONTROL, "Control" )
   CancelSubobject( TZCONTROL, "Control" );
   //:DropObjectInstance( TZADWWKO )
   DropObjectInstance( TZADWWKO );
   //:GET VIEW CurrentVOR NAMED "UpdateObjectViewRef"
   RESULT = GetViewByName( &CurrentVOR, "UpdateObjectViewRef", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( CurrentVOR )
      DropObjectInstance( CurrentVOR );
   } 

   //:END
   //:GET VIEW CurrentLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &CurrentLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( CurrentLOD )
      DropObjectInstance( CurrentLOD );
   } 

   //:END
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:ACCEPT_AutoDesignGroup( VIEW ViewToWindow )

//:   VIEW TZWINDOWL REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
ACCEPT_AutoDesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );

   //:// Accept changes to the AutoDesignGroup subobject, but cancel the Control subobject, since we're
   //:// not making any changes to it and it's a temporal subobject.
   //:CancelSubobject( TZCONTROL, "Control" )
   CancelSubobject( TZCONTROL, "Control" );
   //:AcceptSubobject( TZWINDOWL, "AutoDesignGroup" )
   AcceptSubobject( TZWINDOWL, "AutoDesignGroup" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_Group( VIEW ViewToWindow )

//:   VIEW TZADWWKO   REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_Group( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL  REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW   REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZCONTROL  REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZPESRCO   REGISTERED AS TZPESRCO
   zVIEW     TZPESRCO = 0; 
   //:VIEW TaskLPLR   REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZCtlHier  BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:VIEW TZCtlCopy  BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlCopy = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD  BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW CurrentLOD BASED ON LOD  TZZOLODO
   zVIEW     CurrentLOD = 0; 
   //:VIEW CurrentVOR BASED ON LOD  TZWDVORO
   zVIEW     CurrentVOR = 0; 
   //:STRING ( 50 )  szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 1 )   szGridFlag
   zCHAR     szGridFlag[ 2 ] = { 0 }; 
   //:STRING ( 500 ) szHeaderText
   zCHAR     szHeaderText[ 501 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER GroupSizeX
   zLONG     GroupSizeX = 0; 
   //:INTEGER GroupSizeY
   zLONG     GroupSizeY = 0; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER nNextPositionY
   zLONG     nNextPositionY = 0; 
   //:INTEGER OverrideWidth
   zLONG     OverrideWidth = 0; 
   //:SHORT   lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT   lInitialLevel
   zSHORT    lInitialLevel = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Generate a list box or group of display/update controls based on the GroupType selected.

   //:// Make sure at least one Attribute was selected.
   //:IF TZADWWKO.ESG_ListLOD_Attribute DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" );
   if ( lTempInteger_0 != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "At least one Attribute must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "At least one Attribute must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure Group Type is only ListGroupOnly or DetailGroup.
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:TraceLineS( "*** Group Type: ", szGroupType )
   TraceLineS( "*** Group Type: ", szGroupType );
   //:IF szGroupType != "ListGroupOnly" AND szGroupType != "DetailGroup" 
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) != 0 && ZeidonStringCompare( szGroupType, 1, 0, "DetailGroup", 1, 0, 51 ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "A Group Type of List Group Only or Detail Group must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "A Group Type of List Group Only or Detail Group must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );

   //:// Accept the current Control in case it is versioned.
   //:AcceptSubobject( TZCONTROL, "Control" )
   AcceptSubobject( TZCONTROL, "Control" );

   //:// Make sure the Dialog has a ViewObjRef for the LOD.
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = UpdateLOD.LOD.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szTempString_0, "" );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// Activate the Registered View and include it in the Dialog. It will have been created when the object was created.
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, TZADWWKO.W_MetaDefUpdateObject.Name, 0 )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
      nRC = ActivateMetaOI_ByName( ViewToWindow, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szTempString_0, 0 );
      //:NAME VIEW TZWDVORO "UpdateObjectViewRef"
      SetNameForView( TZWDVORO, "UpdateObjectViewRef", 0, zLEVEL_TASK );
      //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
      RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
      //:DropObjectInstance( TZWDVORO )
      DropObjectInstance( TZWDVORO );
   } 

   //:END 

   //:IF szGroupType = "DetailGroup"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroup", 1, 0, 51 ) == 0 )
   { 

      //:// Detail Group is made up of Prompt / Detail pairs.

      //:// To reuse operation GenDetailGrpBootstrap, we must convert the ESG_ListLOD_Attribute entries to DetailMappingLOD_Attribute entries.
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
         RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

      //:END
      //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_ListLOD_Attribute
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_ListLOD_Attribute", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_ListLOD_Attribute", zSET_NULL )
         SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_ListLOD_Attribute", zSET_NULL );
         //:EXCLUDE TZADWWKO.ESG_ListLOD_Attribute NONE 
         RESULT = ExcludeEntity( TZADWWKO, "ESG_ListLOD_Attribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      } 

      //:END

      //:// If this is update of an existing Group, position on the card-body subgroup as necessary.
      //:IF TZADWWKO.EntitySubGroup.GroupUpdateFlag = "Y"
      if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupUpdateFlag", "Y" ) == 0 )
      { 
         //:IF TZCONTROL.Control.CSS_Class = "card"
         if ( CompareAttributeToString( TZCONTROL, "Control", "CSS_Class", "card" ) == 0 )
         { 
            //:// Step down 2 levels.
            //:SET CURSOR LAST TZCONTROL.CtrlCtrl
            RESULT = SetCursorLastEntity( TZCONTROL, "CtrlCtrl", "" );
            //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
            SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:SET CURSOR LAST TZCONTROL.CtrlCtrl
            RESULT = SetCursorLastEntity( TZCONTROL, "CtrlCtrl", "" );
            //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
            SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:ELSE
         } 
         else
         { 
            //:IF TZCONTROL.Control.CSS_Class = "collapse show"
            if ( CompareAttributeToString( TZCONTROL, "Control", "CSS_Class", "collapse show" ) == 0 )
            { 
               //:// Step down 1 level.
               //:SET CURSOR LAST TZCONTROL.CtrlCtrl
               RESULT = SetCursorLastEntity( TZCONTROL, "CtrlCtrl", "" );
               //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
               SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 

         //:// For a new Group, copy a Bootstrap foundation Group with subgroups and modify the header.
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupDetailOrListTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupDetailOrListTemplates", "" );
         //:SET CURSOR FIRST AD_Base.Control WHERE AD_Base.Control.Tag = "GroupDetail" 
         RESULT = SetCursorFirstEntityByString( AD_Base, "Control", "Tag", "GroupDetail", "" );
         //:DELETE ENTITY TZCONTROL.Control NONE    // Delete the new Control as it will be replaced by the Clone.
         RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
         //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_Base )  
         nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_Base );

         //:// Execute operation to convert Header and Area Group Control data.
         //:// Make sure we're positioned on the Header Text Group.
         //:SET CURSOR FIRST TZCONTROL.CtrlCtrl 
         RESULT = SetCursorFirstEntity( TZCONTROL, "CtrlCtrl", "" );
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:ConvertHeaderText( TZADWWKO, TZCONTROL )
         oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );

         //:// Step down to Control that will hold the Prompt/Detail pairs.
         //:SET CURSOR LAST TZCONTROL.Control  
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      } 

      //:END

      //:// Go to build the Prompt / Detail pairs. However, make sure that TZADWWKO.EntitySubGroup.GroupAreaName is wet
      //:// because it's used for generating names.
      //:IF TZADWWKO.EntitySubGroup.GroupAreaName = ""
      if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaName", "" ) == 0 )
      { 
         //:TZADWWKO.EntitySubGroup.GroupAreaName = TZADWWKO.DetailMappingLOD_Entity.Name 
         SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "GroupAreaName", TZADWWKO, "DetailMappingLOD_Entity", "Name" );
      } 

      //:END
      //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "UpdateGroup" )
      oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "UpdateGroup" );

      //:// Resize and reposition the Group Boxes on the Window
      //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL )
      oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL );
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "ListGroupOnly"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 )
      { 

         //:// List Group is made up of Grid with subcontrols.

         //:// Initialization is different betwee existing Grid and new Grid
         //:IF TZADWWKO.EntitySubGroup.GroupUpdateFlag = "Y"
         if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupUpdateFlag", "Y" ) == 0 )
         { 
            //:// If this is an upate of an existing Group, position on the Grid Control.
            //:CreateViewFromView( TZCtlHier, TZCONTROL )
            CreateViewFromView( &TZCtlHier, TZCONTROL );
            //:DefineHierarchicalCursor( TZCtlHier, "Control" )
            DefineHierarchicalCursor( TZCtlHier, "Control" );
            //:NAME VIEW TZCtlHier "TZCtlHier"
            SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
            //:szGridFlag = ""
            ZeidonStringCopy( szGridFlag, 1, 0, "", 1, 0, 2 );
            //:lInitialLevel = 0
            lInitialLevel = 0;
            //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
            nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
            //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel AND szGridFlag = ""
            while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel && ZeidonStringCompare( szGridFlag, 1, 0, "", 1, 0, 2 ) == 0 )
            { 
               //:IF nRC = zCURSOR_SET_RECURSIVECHILD
               if ( nRC == zCURSOR_SET_RECURSIVECHILD )
               { 
                  //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
                  SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
                  //:IF TZCtlHier.ControlDef.Tag = "Grid"
                  if ( CompareAttributeToString( TZCtlHier, "ControlDef", "Tag", "Grid" ) == 0 )
                  { 
                     //:szGridFlag = "Y"
                     ZeidonStringCopy( szGridFlag, 1, 0, "Y", 1, 0, 2 );
                     //:CreateViewFromView( TZCtlCopy, TZCtlHier )
                     CreateViewFromView( &TZCtlCopy, TZCtlHier );
                     //:NAME VIEW  TZCtlCopy "TZCtlCopy"
                     SetNameForView( TZCtlCopy, "TZCtlCopy", 0, zLEVEL_TASK );
                  } 

                  //:END
               } 

               //:END
               //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
               nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
            } 

            //:END
            //:DropView( TZCtlHier )
            DropView( TZCtlHier );

            //:// Delete all GridCtrl entries under the Grid except for the first.
            //:SetViewToSubobject( TZCtlCopy, "CtrlCtrl" )
            SetViewToSubobject( TZCtlCopy, "CtrlCtrl" );
            //:Count = 0
            Count = 0;
            //:FOR EACH TZCtlCopy.Control 
            RESULT = SetCursorFirstEntity( TZCtlCopy, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:Count = Count + 1
               Count = Count + 1;
               //:IF Count > 1 AND TZCtlCopy.CtrlMap EXISTS
               lTempInteger_1 = CheckExistenceOfEntity( TZCtlCopy, "CtrlMap" );
               if ( Count > 1 && lTempInteger_1 == 0 )
               { 
                  //:DELETE ENTITY TZCtlCopy.Control NONE
                  RESULT = DeleteEntity( TZCtlCopy, "Control", zREPOS_NONE );
               } 

               RESULT = SetCursorNextEntity( TZCtlCopy, "Control", "" );
               //:END
            } 

            //:END
            //:ResetViewFromSubobject( TZCtlCopy )    // Back up to Grid
            ResetViewFromSubobject( TZCtlCopy );

            //:ELSE
         } 
         else
         { 

            //:// For a new Group, copy a Bootstrap foundation Group with subgroups and modify the header.
            //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupDetailOrListTemplates"
            RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupDetailOrListTemplates", "" );
            //:SET CURSOR FIRST AD_Base.Control WHERE AD_Base.Control.Tag = "GroupList" 
            RESULT = SetCursorFirstEntityByString( AD_Base, "Control", "Tag", "GroupList", "" );
            //:DELETE ENTITY TZCONTROL.Control NONE    // Delete the new Control as it will be replaced by the Clone.
            RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
            //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_Base )  
            nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_Base );

            //:// Execute operation to convert Header and Area Group Control data.
            //:// Make sure we're positioned on the Header Text Group.
            //:SET CURSOR FIRST TZCONTROL.CtrlCtrl 
            RESULT = SetCursorFirstEntity( TZCONTROL, "CtrlCtrl", "" );
            //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
            SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:ConvertHeaderText( TZADWWKO, TZCONTROL )
            oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );

            //:// Step down to Control that will hold the Prompt/Detail pairs.
            //:ResetViewFromSubobject( TZCONTROL )  // Step back to main Group
            ResetViewFromSubobject( TZCONTROL );
         } 

         //:END

         //:// Go to make mapping changes to the Grid. We should be positioned on the Grid Control itself.
         //:szViewName           = TZADWWKO.W_MetaDefUpdateView.Name 
         GetVariableFromAttribute( szViewName, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateView", "Name", "", 0 );
         //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name 
         GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
         //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOW, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" )
         oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOW, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" );

         //:// Resize and reposition the Group Boxes on the Window
         //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL )
         oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL );
      } 

      //:END
   } 

   //:END

   //:// Drop object TZADWWKO so it will be created new next time.
   //:// Also drop other temporary objects.
   //:DropObjectInstance( TZADWWKO )
   DropObjectInstance( TZADWWKO );
   //:GET VIEW CurrentVOR NAMED "UpdateObjectViewRef"
   RESULT = GetViewByName( &CurrentVOR, "UpdateObjectViewRef", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( CurrentVOR )
      DropObjectInstance( CurrentVOR );
   } 

   //:END
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:GET VIEW CurrentLOD NAMED "TZZOLODO_Update"
      RESULT = GetViewByName( &CurrentLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   } 

   //:END
   //:DropObjectInstance( CurrentLOD )
   DropObjectInstance( CurrentLOD );

   //:// Reposition and Resize all Group Controls.
   //:// Resize should be done above.
   //://ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL )

   //:// Rename some Controls to avoid duplicate Tags.
   //:SET CURSOR FIRST TZWINDOWL.Control
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:RenameAD_Controls( TZADWWKO, TZWINDOWL ) 
   oTZADWWKO_RenameAD_Controls( TZADWWKO, TZWINDOWL );

   //:// Tell the Painter to refresh the control.
   //:RepaintWindowControls( ViewToWindow )
   RepaintWindowControls( ViewToWindow );
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:CHECK_ReorderAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CHECK_ReorderAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO2 BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO2 = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Validate Row and Group values.
   //:nRC = VALIDATE_SearchAttributeDef( ViewToWindow )
   nRC = VALIDATE_SearchAttributeDef( ViewToWindow );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      return( nRC );
   } 

   //:END

   //:// Make sure that any Entity entries are properly positioned before their corresponding attributes.
   //:CreateViewFromView( TZADWWKO2, TZADWWKO )
   CreateViewFromView( &TZADWWKO2, TZADWWKO );
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKO.FlatSelectedSearchAttribute.IndentEntityName != ""
      if ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "IndentEntityName", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZADWWKO2.FlatSelectedSearchAttribute
         //:           WHERE TZADWWKO2.FlatSelectedSearchAttribute.EntityName = TZADWWKO.FlatSelectedSearchAttribute.EntityName
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "FlatSelectedSearchAttribute", "EntityName" );
         RESULT = SetCursorFirstEntityByString( TZADWWKO2, "FlatSelectedSearchAttribute", "EntityName", szTempString_0, "" );
         //:IF TZADWWKO2.FlatSelectedSearchAttribute.IndentEntityName != TZADWWKO.FlatSelectedSearchAttribute.IndentEntityName
         if ( CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedSearchAttribute", "IndentEntityName", TZADWWKO, "FlatSelectedSearchAttribute", "IndentEntityName" ) != 0 )
         { 
            //:// An Entity entry has been moved, so put it back.
            //:CREATE ENTITY TZADWWKO2.FlatSelectedSearchAttribute BEFORE
            RESULT = CreateEntity( TZADWWKO2, "FlatSelectedSearchAttribute", zPOS_BEFORE );
            //:SetMatchingAttributesByName( TZADWWKO2, "FlatSelectedSearchAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL )
            SetMatchingAttributesByName( TZADWWKO2, "FlatSelectedSearchAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL );
            //:DELETE ENTITY TZADWWKO.FlatSelectedSearchAttribute
            RESULT = DeleteEntity( TZADWWKO, "FlatSelectedSearchAttribute", zPOS_NEXT );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      //:END
   } 

   //:END
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   //:RefreshWindow( ViewToWindow )
   RefreshWindow( ViewToWindow );
   //:DropView( TZADWWKO2 )
   DropView( TZADWWKO2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_CRM_FindSubdialog( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_CRM_FindSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// The code for this function is an operation in TZADWWKO that is used for both Object Find and
   //:// CRM Find.

   //:// Make sure that a Contact List Type has been selected.
   //:IF TZADWWKO.SelectedContactListTypeDomain DOES NOT EXIST OR
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "SelectedContactListTypeDomain" );
   //:   TZADWWKO.SelectedContactListTypeDomain.InternalType = ""
   if ( lTempInteger_0 != 0 || CompareAttributeToString( TZADWWKO, "SelectedContactListTypeDomain", "InternalType", "" ) == 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "A Contact List Type must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "A Contact List Type must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:AutodesignFindPages( TZADWWKO, ViewToWindow, "CRM" )
   oTZADWWKO_AutodesignFindPages( TZADWWKO, ViewToWindow, "CRM" );
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_ObjectFindSubdialog( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_ObjectFindSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// The code for this function is an operation in TZADWWKO that is used for both Object Find and
   //:// CRM Find.

   //:AutodesignFindPages( TZADWWKO, ViewToWindow, "O" )
   oTZADWWKO_AutodesignFindPages( TZADWWKO, ViewToWindow, "O" );
   return( 0 );
//  
// END
} 


//:DIALOG OPERATION
//:SELECT_FindReturnedObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_FindReturnedObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Activate the selected ReturnedLOD and include it in the work object.
   //:// It will also be named "TZZOLODO_Return".
   //:GET VIEW ReturnedLOD NAMED "TZZOLODO_Return"
   RESULT = GetViewByName( &ReturnedLOD, "TZZOLODO_Return", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( ReturnedLOD )
      DropObjectInstance( ReturnedLOD );
   } 

   //:END
   //:szLOD_Name = TZADWWKO.W_MetaDefReturnedObject.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKO, "W_MetaDefReturnedObject", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, ReturnedLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &ReturnedLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW ReturnedLOD "TZZOLODO_Return"
   SetNameForView( ReturnedLOD, "TZZOLODO_Return", 0, zLEVEL_TASK );

   //:// Clear any existing TZADWWKO.ESG_FlatListPotentialAttribute2 entries and rebuild them from selected object.
   //:szEntityName = ReturnedLOD.LOD_EntityParent.Name 
   GetVariableFromAttribute( szEntityName, 0, 'S', 51, ReturnedLOD, "LOD_EntityParent", "Name", "", 0 );
   //:BuildAD_GroupPotAttrList( TZADWWKO, ReturnedLOD, szEntityName, "2" )
   o_BuildAD_GroupPotAttrList( TZADWWKO, ReturnedLOD, szEntityName, "2" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_AutodesignCRMSubdialog( VIEW ViewToWindow )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignCRMSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO    BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW TZZOLFLO    BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW mConListLOD BASED ON LOD  TZZOLODO
   zVIEW     mConListLOD = 0; 
   //:STRING ( 300 ) szAutodesignLPLR_Directory
   zCHAR     szAutodesignLPLR_Directory[ 301 ] = { 0 }; 
   //:STRING ( 300 ) szMsg
   zCHAR     szMsg[ 301 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Make sure an Auto Design Base dialog can be loaded.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// The AD_Base object isn't in memory, so load it.
      //:nRC = ActivateAD_BaseL( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_BaseL( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base"
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Make sure that the TZZOLFLO object of Meta LOD entries exists.
   //:GET VIEW TZZOLFLO NAMED "TZZOLFLO"
   RESULT = GetViewByName( &TZZOLFLO, "TZZOLFLO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZZOLFLO, TaskLPLR )
      CreateViewFromView( &TZZOLFLO, TaskLPLR );
      //:NAME VIEW TZZOLFLO "TZZOLFLO"
      SetNameForView( TZZOLFLO, "TZZOLFLO", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST TZZOLFLO.W_MetaType WHERE TZZOLFLO.W_MetaType.Type = 7   // 7 is read view of LOD 
      RESULT = SetCursorFirstEntityByInteger( TZZOLFLO, "W_MetaType", "Type", 7, "" );
   } 

   //:END

   //:// Make sure that the mConList object exists in the LPLR.
   //:// Then create list of potential mConList Attributes.
   //:SET CURSOR FIRST TZZOLFLO.W_MetaDef WHERE TZZOLFLO.W_MetaDef.Name = "mConList"
   RESULT = SetCursorFirstEntityByString( TZZOLFLO, "W_MetaDef", "Name", "mConList", "" );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign CRM Find",
      //:             "The LPLR does not have the necessary object, 'mConList'.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign CRM Find", "The LPLR does not have the necessary object, 'mConList'.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// The TZADWWKO object  should be created new.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( TZADWWKO )
      DropObjectInstance( TZADWWKO );
   } 

   //:END
   //:ACTIVATE TZADWWKO EMPTY 
   RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
   //:NAME VIEW TZADWWKO "TZADWWKO"
   SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
   //:CREATE ENTITY TZADWWKO.AutoDesignWork 
   RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
   //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
   RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
   //:TZADWWKO.AutodesignSubdialog.FindType = "CRM"
   SetAttributeFromString( TZADWWKO, "AutodesignSubdialog", "FindType", "CRM" );
   //:CREATE ENTITY TZADWWKO.EntitySubGroup  
   RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
   //:InitializeL_SymbolType( TZADWWKO )
   oTZADWWKO_InitializeL_SymbolType( TZADWWKO );

   //:// First activate the mConList Object.
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, mConListLOD, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &mConListLOD, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 );
   //:NAME VIEW mConListLOD "mConListLOD"
   SetNameForView( mConListLOD, "mConListLOD", 0, zLEVEL_TASK );

   //:// Clear any existing TZADWWKO.ESG_FlatListPotentialAttribute2 entries and rebuild them from selected object.
   //:BuildAD_GroupPotAttrList( TZADWWKO, mConListLOD, "ContactList", "2" )
   o_BuildAD_GroupPotAttrList( TZADWWKO, mConListLOD, "ContactList", "2" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_CRM_ReturnedAttribute( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_ReturnedAttribute( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Make sure the TZADWWKO.LOD_EntityCL_ItemDisplay entity has already been selected.
   //:IF TZADWWKO.LOD_EntityCL_ItemDisplay DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityCL_ItemDisplay" );
   if ( lTempInteger_0 != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "A Returned Entity must be selected first.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "A Returned Entity must be selected first.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Include LOD_AttributeCL_ItemDisplay from selected Attribute.
   //:GET VIEW ReturnedLOD NAMED "TZZOLODO_Desc"
   RESULT = GetViewByName( &ReturnedLOD, "TZZOLODO_Desc", ViewToWindow, zLEVEL_TASK );
   //:IF TZADWWKO.LOD_AttributeCL_ItemDisplay EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "LOD_AttributeCL_ItemDisplay" );
   if ( lTempInteger_1 == 0 )
   { 
      //:EXCLUDE TZADWWKO.LOD_AttributeCL_ItemDisplay  
      RESULT = ExcludeEntity( TZADWWKO, "LOD_AttributeCL_ItemDisplay", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.LOD_AttributeCL_ItemDisplay FROM ReturnedLOD.LOD_Attribute 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "LOD_AttributeCL_ItemDisplay", ReturnedLOD, "LOD_Attribute", zPOS_AFTER );
   return( 0 );
// END
} 


//:LOCAL OPERATION  
//:ChangeViewObjRef( VIEW ViewToWindow,
//:                  VIEW TZWINDOW  BASED ON LOD  TZWDLGSO,
//:                  VIEW TZWINDOWL BASED ON LOD  TZWDLGSO,
//:                  STRING ( 50 ) szViewObjRefName,
//:                  STRING ( 50 ) szLOD_Name,
//:                  STRING ( 50 ) szGroupBoxName )
//:   
//:   VIEW TZWDVORO BASED ON LOD TZWDVORO
static zSHORT
o_ChangeViewObjRef( zVIEW     ViewToWindow,
                    zVIEW     TZWINDOW,
                    zVIEW     TZWINDOWL,
                    zPCHAR    szViewObjRefName,
                    zPCHAR    szLOD_Name,
                    zPCHAR    szGroupBoxName )
{
   zVIEW     TZWDVORO = 0; 
   //:VIEW GridLOD  BASED ON LOD TZZOLODO
   zVIEW     GridLOD = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szViewObjRefName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szViewObjRefName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// Activate the Registered View and include it in the Dialog
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szViewObjRefName, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szViewObjRefName, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:// Activate the lConList LOD.
         //:nRC = ActivateMetaOI_ByName( ViewToWindow, GridLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
         nRC = ActivateMetaOI_ByName( ViewToWindow, &GridLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:IssueError( ViewToWindow,0,0, "Grid LOD couldn't be activated." )
            IssueError( ViewToWindow, 0, 0, "Grid LOD couldn't be activated." );
         } 

         //:END
         //:NAME VIEW GridLOD "GridLOD"
         SetNameForView( GridLOD, "GridLOD", 0, zLEVEL_TASK );
         //:// Create the ViewObjRef.
         //:ActivateEmptyMetaOI( ViewToWindow, TZWDVORO, zSOURCE_VOR_META, zSINGLE )
         ActivateEmptyMetaOI( ViewToWindow, &TZWDVORO, zSOURCE_VOR_META, zSINGLE );
         //:NAME VIEW TZWDVORO "TZWDVORO_GridObjRef"
         SetNameForView( TZWDVORO, "TZWDVORO_GridObjRef", 0, zLEVEL_TASK );
         //:CreateMetaEntity( ViewToWindow, TZWDVORO, "ViewObjRef", zPOS_AFTER )
         CreateMetaEntity( ViewToWindow, TZWDVORO, "ViewObjRef", zPOS_AFTER );
         //:TZWDVORO.ViewObjRef.Name = szViewObjRefName 
         SetAttributeFromString( TZWDVORO, "ViewObjRef", "Name", szViewObjRefName );
         //:TZWDVORO.ViewObjRef.Desc = "Created during CRM Build"
         SetAttributeFromString( TZWDVORO, "ViewObjRef", "Desc", "Created during CRM Build" );
         //:INCLUDE TZWDVORO.LOD FROM GridLOD.LOD 
         RESULT = IncludeSubobjectFromSubobject( TZWDVORO, "LOD", GridLOD, "LOD", zPOS_AFTER );
         //:CommitMetaOI( ViewToWindow, TZWDVORO, zSOURCE_VOR_META )
         CommitMetaOI( ViewToWindow, TZWDVORO, zSOURCE_VOR_META );
         //:DropObjectInstance( GridLOD )
         DropObjectInstance( GridLOD );
         //:ELSE
      } 
      else
      { 
         //:NAME VIEW TZWDVORO "TZWDVORO_GridObjRef"
         SetNameForView( TZWDVORO, "TZWDVORO_GridObjRef", 0, zLEVEL_TASK );
      } 

      //:END
      //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
      RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
      //:DropObjectInstance( TZWDVORO )
      DropObjectInstance( TZWDVORO );
   } 

   //:END

   //:// Finally, replace the ViewObjRef in the Grid for GroupBox.
   //:SET CURSOR FIRST TZWINDOWL.Control WHERE TZWINDOWL.Control.Tag = szGroupBoxName
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Control", "Tag", szGroupBoxName, "" );
   //:// Step down levels to the Grid.

   //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
   SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
   //:SET CURSOR LAST TZWINDOWL.Control     // The Grid will be under the last subGroupBox
   RESULT = SetCursorLastEntity( TZWINDOWL, "Control", "" );
   //:IF TZWINDOWL.ControlDef.Tag = "GroupBox"
   if ( CompareAttributeToString( TZWINDOWL, "ControlDef", "Tag", "GroupBox" ) == 0 )
   { 
      //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
      SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
      //:SET CURSOR LAST TZWINDOWL.Control     // The Grid will be under the last subGroupBox
      RESULT = SetCursorLastEntity( TZWINDOWL, "Control", "" );
      //:IF TZWINDOWL.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TZWINDOWL, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
         SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
         //:SET CURSOR LAST TZWINDOWL.Control     // The Grid will be under the last subGroupBox
         RESULT = SetCursorLastEntity( TZWINDOWL, "Control", "" );
         //:IF TZWINDOWL.ControlDef.Tag = "GroupBox"
         if ( CompareAttributeToString( TZWINDOWL, "ControlDef", "Tag", "GroupBox" ) == 0 )
         { 
            //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
            SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// Change the ViewObjRef for the Grid itself.
   //:EXCLUDE TZWINDOWL.CtrlMapView 
   RESULT = ExcludeEntity( TZWINDOWL, "CtrlMapView", zREPOS_AFTER );
   //:INCLUDE TZWINDOWL.CtrlMapView FROM TZWINDOW.ViewObjRef
   RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );

   //:// Change the ViewObjRef for each Grid subcontrol.
   //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
   SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
   //:FOR EACH TZWINDOWL.Control 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZWINDOWL.CtrlMap EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "CtrlMap" );
      if ( lTempInteger_0 == 0 )
      { 
         //:EXCLUDE TZWINDOWL.CtrlMapView 
         RESULT = ExcludeEntity( TZWINDOWL, "CtrlMapView", zREPOS_AFTER );
         //:INCLUDE TZWINDOWL.CtrlMapView FROM TZWINDOW.ViewObjRef 
         RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
      //:END 
   } 

   //:END

   //:// Reset the view back to the top.
   //:ResetViewFromSubobjectTop( TZWINDOWL )
   ResetViewFromSubobjectTop( TZWINDOWL );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_UPD_PotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_UPD_PotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO2 BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO2 = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected Entity to the list of EntitySubGroup entries at the current position.

   //:CreateViewFromView( TZADWWKO2, TZADWWKO)
   CreateViewFromView( &TZADWWKO2, TZADWWKO );
   //:NAME VIEW TZADWWKO2 "TZADWWKO2" 
   SetNameForView( TZADWWKO2, "TZADWWKO2", 0, zLEVEL_TASK );

   //:// On 11/17/2023 DonC switched this code to allow multiple EntitySubGroup entries for the same Entity.
   //:FOR EACH TZADWWKO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "FlatListPotentialEntity" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "FlatListPotentialEntity" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //://SET CURSOR FIRST TZADWWKO2.ESG_LOD_Entity WITHIN TZADWWKO2.AutoDesignWork 
         //://           WHERE TZADWWKO2.ESG_LOD_Entity.ZKey = TZADWWKO.FlatListPotentialLOD_Entity.ZKey 
         //://IF RESULT < zCURSOR_SET
         //:   CREATE ENTITY TZADWWKO.EntitySubGroup 
         RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
         //:   INCLUDE TZADWWKO.ESG_LOD_Entity FROM TZADWWKO.FlatListPotentialLOD_Entity 
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_LOD_Entity", TZADWWKO, "FlatListPotentialLOD_Entity", zPOS_AFTER );
         //:   SetSelectStateOfEntity( TZADWWKO, "FlatListPotentialEntity", 0 )
         SetSelectStateOfEntity( TZADWWKO, "FlatListPotentialEntity", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatListPotentialEntity", "" );
      //:   //END
      //:END
   } 

   //:END

   //:DropView( TZADWWKO2 )
   DropView( TZADWWKO2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_UPD_PotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_UPD_PotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "EntitySubGroup" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "EntitySubGroup" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:DELETE ENTITY TZADWWKO.EntitySubGroup NONE
         RESULT = DeleteEntity( TZADWWKO, "EntitySubGroup", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_FindDetailObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_FindDetailObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Activate the selected UpdateLOD and include it in the work object.
   //:// It will also be named "TZZOLODO_Update".
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( UpdateLOD )
      DropObjectInstance( UpdateLOD );
   } 

   //:END
   //:szLOD_Name = TZADWWKO.W_MetaDefUpdateObject.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW UpdateLOD "TZZOLODO_Update"
   SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   //:NAME VIEW UpdateLOD "TZZOLODO_Desc"     // We need this named view for the Root select combobox.
   SetNameForView( UpdateLOD, "TZZOLODO_Desc", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_FindPotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_FindPotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW  REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZADWWKO2 BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO2 = 0; 
   //:STRING ( 1 )   szCreateEntityOnlyEntry
   zCHAR     szCreateEntityOnlyEntry[ 2 ] = { 0 }; 
   //:STRING ( 100 ) szTempString
   zCHAR     szTempString[ 101 ] = { 0 }; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", ViewToWindow, zLEVEL_TASK );

   //:// Make sure we have position on AutoDesignWindow.
   //:SET CURSOR FIRST TZWINDOW.AutoDesignWindow  
   RESULT = SetCursorFirstEntity( TZWINDOW, "AutoDesignWindow", "" );

   //:// Copy selected attributes from ESG_FlatListPotentialAttribute entries to FlatSelectedSearchAttribute entries at the current postiion..
   //:CreateViewFromView( TZADWWKO2, TZADWWKO )
   CreateViewFromView( &TZADWWKO2, TZADWWKO );
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO2.FlatSelectedSearchAttribute 
         //:           WHERE TZADWWKO2.FlatSelectedSearchAttribute.EntityName    = TZADWWKO.ESG_FlatListPotentialAttribute.EntityName  
         //:             AND TZADWWKO2.FlatSelectedSearchAttribute.AttributeName = TZADWWKO.ESG_FlatListPotentialAttribute.AttributeName 
         RESULT = SetCursorFirstEntity( TZADWWKO2, "FlatSelectedSearchAttribute", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedSearchAttribute", "EntityName", TZADWWKO, "ESG_FlatListPotentialAttribute", "EntityName" ) != 0 ||
                    CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedSearchAttribute", "AttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute", "AttributeName" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZADWWKO2, "FlatSelectedSearchAttribute", "" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY TZADWWKO.FlatSelectedSearchAttribute
            RESULT = CreateEntity( TZADWWKO, "FlatSelectedSearchAttribute", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_FlatListPotentialAttribute", zSET_ALL )
            SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_FlatListPotentialAttribute", zSET_ALL );
            //:TZADWWKO.FlatSelectedSearchAttribute.DataType = TZADWWKO.ESG_FlatDomain.DataType 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "DataType", TZADWWKO, "ESG_FlatDomain", "DataType" );
            //:INCLUDE TZADWWKO.FlatSearchLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatSearchLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   //:DropView( TZADWWKO2 )
   DropView( TZADWWKO2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_FindPotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_FindPotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "FlatSelectedSearchAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "FlatSelectedSearchAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:DELETE ENTITY TZADWWKO.FlatSelectedSearchAttribute NONE
         RESULT = DeleteEntity( TZADWWKO, "FlatSelectedSearchAttribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_TypeForSelectedGroups( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_TypeForSelectedGroups( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set the specified Group Type for each selected entry.
   //:// Also make sure that a Type was selected.
   //:IF TZADWWKO.AutoDesignWork.SelectedGroupType = ""
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedGroupType", "" ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "A Type must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "A Type must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "EntitySubGroup" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "EntitySubGroup" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:TZADWWKO.EntitySubGroup.GroupType = TZADWWKO.AutoDesignWork.SelectedGroupType 
         SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "GroupType", TZADWWKO, "AutoDesignWork", "SelectedGroupType" );
         //:SetSelectStateOfEntity( TZADWWKO, "EntitySubGroup", 0 )
         SetSelectStateOfEntity( TZADWWKO, "EntitySubGroup", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_ListPotAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_ListPotAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected ESG_FlatListPotentialAttribute entry as a selected ESG List Attribute.
   //:// We will add them sequentially to the current ESG_ListLOD_Attribute position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_ListLOD_Attribute  
         //:           WHERE TZADWWKO.ESG_ListLOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_ListLOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
            //:// We will use the list title value if it exists. Otherwise, we will use the Prompt value.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute.ListTitleValue != ""
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute", "ListTitleValue", "" ) != 0 )
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute.ListTitleValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "ListTitleValue" );
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute.ListTitleLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute", "ListTitleLength" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute.PromptValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptValue" );
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute.PromptLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptLength" );
            } 

            //:END
            //:// ControlType is Text if Entity isn't updateable.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute.Create = "Y"
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute", "Create", "Y" ) == 0 )
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Text"
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Text" );
            } 

            //:END
            //:TZADWWKO.ESG_ListLOD_Attribute.ControlWidth = TZADWWKO.ESG_FlatListPotentialAttribute.ControlWidth 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlWidth" );
            //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 )
            SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 );

            //:// The CSS_Class values depend on those set up in EntitySubGroup.
            //:TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForPrompt  = TZADWWKO.EntitySubGroup.CSS_ClassForPrompt 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForPrompt", TZADWWKO, "EntitySubGroup", "CSS_ClassForPrompt" );
            //:TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForControl = TZADWWKO.EntitySubGroup.CSS_ClassForControl 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForControl", TZADWWKO, "EntitySubGroup", "CSS_ClassForControl" );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_ESGL_ListSelAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_ListSelAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected List entries.
   //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:EXCLUDE TZADWWKO.ESG_ListLOD_Attribute NONE
         RESULT = ExcludeEntity( TZADWWKO, "ESG_ListLOD_Attribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_DetailPotAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_DetailPotAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 20 ) szGroupType
   zCHAR     szGroupType[ 21 ] = { 0 }; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected ESG_FlatDetailPotentialAttribute entry as a selected ESG List Attribute.
   //:// We will add them sequentially to the current ESG_ListLOD_Attribute position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute  
         //:           WHERE TZADWWKO.ESG_DetailLOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_DetailLOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:INCLUDE TZADWWKO.ESG_DetailLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_DetailLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
            //: // We will always use the Prompt value for Search.
            //:TZADWWKO.ESG_DetailLOD_Attribute.PromptValue   = TZADWWKO.ESG_FlatListPotentialAttribute.PromptValue 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptValue" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.PromptLength  = TZADWWKO.ESG_FlatListPotentialAttribute.PromptLength 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptLength" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.ControlWidth  = TZADWWKO.ESG_FlatListPotentialAttribute.ControlWidth
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlWidth" );
            //:// ControlType is Text if Entity isn't updateable.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute.Create = "Y"
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute", "Create", "Y" ) == 0 )
            { 
               //:TZADWWKO.ESG_DetailLOD_Attribute.ControlType = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType
               SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_DetailLOD_Attribute.ControlType = "Text"
               SetAttributeFromString( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType", "Text" );
            } 

            //:END
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END

   //:// If the GroupType is "DetailGroupDisplay" OR "DetailGroupInclude", set ControlType to Text.
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 21, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:IF szGroupType = "DetailGroupDisplay" OR szGroupType = "DetailGroupInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupDisplay", 1, 0, 21 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupInclude", 1, 0, 21 ) == 0 )
   { 
      //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:TZADWWKO.ESG_DetailLOD_Attribute.ControlType = "Text" 
         SetAttributeFromString( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType", "Text" );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_ESGL_DetailSelAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_DetailSelAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:EXCLUDE TZADWWKO.ESG_DetailLOD_Attribute NONE
         RESULT = ExcludeEntity( TZADWWKO, "ESG_DetailLOD_Attribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:BuildAD_GroupPotAttrList( VIEW TZADWWKO    BASED ON LOD TZADWWKO,
//:                      VIEW SelectedLOD BASED ON LOD TZZOLODO,
//:                      STRING ( 32 ) szStartEntityName,
//:                      STRING ( 1 ) szFlatListNameSuffix )

//:   VIEW RecursiveLOD BASED ON LOD TZZOLODO
static zSHORT
o_BuildAD_GroupPotAttrList( zVIEW     TZADWWKO,
                            zVIEW     SelectedLOD,
                            zPCHAR    szStartEntityName,
                            zPCHAR    szFlatListNameSuffix )
{
   zVIEW     RecursiveLOD = 0; 
   //:STRING ( 32 ) szFlatListPotAttributeName
   zCHAR     szFlatListPotAttributeName[ 33 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 


   //:// First delete any current ESG_FlatListPotentialAttribute entries based on suffix.
   //:szFlatListPotAttributeName  = "ESG_FlatListPotentialAttribute" + szFlatListNameSuffix
   ZeidonStringCopy( szFlatListPotAttributeName, 1, 0, "ESG_FlatListPotentialAttribute", 1, 0, 33 );
   ZeidonStringConcat( szFlatListPotAttributeName, 1, 0, szFlatListNameSuffix, 1, 0, 33 );
   //:nRC = SetCursorFirstEntity( TZADWWKO, szFlatListPotAttributeName, "" )
   nRC = SetCursorFirstEntity( TZADWWKO, szFlatListPotAttributeName, "" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 
      //:DeleteEntity( TZADWWKO, szFlatListPotAttributeName, zREPOS_NONE )
      DeleteEntity( TZADWWKO, szFlatListPotAttributeName, zREPOS_NONE );
      //:nRC = SetCursorNextEntity( TZADWWKO, szFlatListPotAttributeName, "" )
      nRC = SetCursorNextEntity( TZADWWKO, szFlatListPotAttributeName, "" );
   } 

   //:END

   //:// Begin the recursive calls to BuildAD_GroupPotAttrListR to copy attributes recursively starting with szStartEntityName.
   //:CreateViewFromView( RecursiveLOD, SelectedLOD )
   CreateViewFromView( &RecursiveLOD, SelectedLOD );
   //:NAME VIEW RecursiveLOD "RecursiveLOD"
   SetNameForView( RecursiveLOD, "RecursiveLOD", 0, zLEVEL_TASK );
   //:BuildAD_GroupPotAttrListR( TZADWWKO, SelectedLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, 1, 0, "" )
   o_BuildAD_GroupPotAttrListR( TZADWWKO, SelectedLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, 1, 0, "" );
   //:DropView( RecursiveLOD )
   DropView( RecursiveLOD );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_MPG_Pages( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_MPG_Pages( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOMn  BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOMn = 0; 
   //:VIEW TaskLPLR    REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZPESRCO    REGISTERED AS TZPESRCO
   zVIEW     TZPESRCO = 0; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW    BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZWINDOW2   BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW2 = 0; 
   //:VIEW TZCONTROL   BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 
   //:VIEW AD_Base     REGISTERED AS AD_Base
   zVIEW     AD_Base = 0; 
   //:VIEW AD_BaseSrc  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseSrc = 0; 
   //:VIEW AD_BaseCtl  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:VIEW TZWDVORO    BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW AD_BaseUC   BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseUC = 0; 
   //:STRING ( 50 )  szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Suffix
   zCHAR     szAD_Suffix[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedObjectName
   zCHAR     szReturnedObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingObject
   zCHAR     szMappingObject[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingEntity
   zCHAR     szMappingEntity[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingAttribute
   zCHAR     szMappingAttribute[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSearchObjectName
   zCHAR     szSearchObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSourceFileName
   zCHAR     szSourceFileName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclEntS 
   zCHAR     szInclEntS[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclObjS 
   zCHAR     szInclObjS[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclEntT 
   zCHAR     szInclEntT[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclObjT
   zCHAR     szInclObjT[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclSuff
   zCHAR     szInclSuff[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchN 
   zCHAR     szAD_SchN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetO 
   zCHAR     szAD_RetO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetE 
   zCHAR     szAD_RetE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ID
   zCHAR     szAD_ID[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMainV
   zCHAR     szMainV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMainE
   zCHAR     szMainE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdE
   zCHAR     szAD_UpdE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szOriginalVML
   zCHAR     szOriginalVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCheckForOperation
   zCHAR     szCheckForOperation[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nNextPositionY
   zLONG     nNextPositionY = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength  
   zLONG     nLength = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zSHORT    lTempInteger_7; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );

   //:// Autodesign a Window and possible Subwindows based on the EntitySubGroups defined as follows:
   //:// On the first Window, build a list or detail group for each EntitySubGroup depending on the EntitySubType.
   //:// Also build a subwindow for those EntitySubType entries that include such a subpage.

   //:// Reactivate AD_Base window to make sure we get the correct templates.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( AD_Base )
      DropObjectInstance( AD_Base );
   } 

   //:END
   //:ActivateAD_BaseL( AD_Base, ViewToWindow )
   o_ActivateAD_BaseL( &AD_Base, ViewToWindow );

   //:// If View TZWINDOW doesn't exist, create it from TZWINDOWL. TZWINDOW doesn't exist if a Window isn't open.
   //:GET VIEW TZWINDOW NAMED "TZWINDOW"
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZWINDOW, TZWINDOWL )
      CreateViewFromView( &TZWINDOW, TZWINDOWL );
      //:NAME VIEW TZWINDOW "TZWINDOW"
      SetNameForView( TZWINDOW, "TZWINDOW", 0, zLEVEL_TASK );
   } 

   //:END

   //:SET CURSOR LAST TZWINDOWL.Window    // Position at end of target Dialog.
   RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
   //:szAD_Suffix = TZADWWKO.AutodesignSubdialog.AD_AreaSuffix 
   GetVariableFromAttribute( szAD_Suffix, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix", "", 0 );
   //:szAD_Area   = TZADWWKO.AutodesignSubdialog.AD_AreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "AD_AreaName", "", 0 );
   //:IF szAD_Suffix = "" OR szAD_Area = ""
   if ( ZeidonStringCompare( szAD_Suffix, 1, 0, "", 1, 0, 51 ) == 0 || ZeidonStringCompare( szAD_Area, 1, 0, "", 1, 0, 51 ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both an AD Area and an AD Suffix must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both an AD Area and an AD Suffix must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Update LOD, which is the main LOD for the multi-group page for both Update and Detail.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Update Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Update Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for mapping data.
   //:IF TZADWWKO.ESG_ListLOD_Attribute   DOES NOT EXIST AND
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" );
   //:   TZADWWKO.ESG_DetailLOD_Attribute DOES NOT EXIST
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute" );
   if ( lTempInteger_0 != 0 && lTempInteger_1 != 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "List and Detail Attributes must be specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "List and Detail Attributes must be specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure the current Dialog has a main SourceFile entry.
   //:szSourceFileName = TZWINDOW.Dialog.Tag 
   GetVariableFromAttribute( szSourceFileName, 0, 'S', 51, TZWINDOW, "Dialog", "Tag", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.SourceFile WHERE TZWINDOW.SourceFile.Name = szSourceFileName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "SourceFile", "Name", szSourceFileName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER );
      //:TZWINDOW.SourceFile.Name         = szSourceFileName
      SetAttributeFromString( TZWINDOW, "SourceFile", "Name", szSourceFileName );
      //:TZWINDOW.SourceFile.Extension    = "VML"
      SetAttributeFromString( TZWINDOW, "SourceFile", "Extension", "VML" );
      //:TZWINDOW.SourceFile.LanguageType = "V"
      SetAttributeFromString( TZWINDOW, "SourceFile", "LanguageType", "V" );
   } 

   //:END 

   //:// Initialize the main Window based on Page Type.
   //:IF TZADWWKO.AutoDesignWork.SelectedPageType = "U"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedPageType", "U" ) == 0 )
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ObjectUpdate"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ObjectUpdate", "" );
      //:ELSE
   } 
   else
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ObjectDetail"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ObjectDetail", "" );
   } 

   //:END
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "The Base window named 'ObjectUpdate' doesn't exist in AD_Base.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "The Base window named 'ObjectUpdate' doesn't exist in AD_Base.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:SET CURSOR LAST TZWINDOWL.Window  
   RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
   //:nRC = CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow );
   //:TraceLineI( "*** CloneWindow RC: ", nRC )
   TraceLineI( "*** CloneWindow RC: ", nRC );
   //:szWindowName = szAD_Area + "Detail"
   ZeidonStringCopy( szWindowName, 1, 0, szAD_Area, 1, 0, 51 );
   ZeidonStringConcat( szWindowName, 1, 0, "Detail", 1, 0, 51 );
   //:TZWINDOWL.Window.Tag     = szWindowName
   SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );
   //:TZWINDOWL.Window.Caption = szWindowName
   SetAttributeFromString( TZWINDOWL, "Window", "Caption", szWindowName );

   //:// Create view for generating update controls.
   //:CreateViewFromView( AD_BaseUC, AD_Base )
   CreateViewFromView( &AD_BaseUC, AD_Base );
   //:SET CURSOR FIRST AD_BaseUC.Window WHERE AD_BaseUC.Window.Tag = "UpdateFields" 
   RESULT = SetCursorFirstEntityByString( AD_BaseUC, "Window", "Tag", "UpdateFields", "" );

   //:// Add Update Object Registered View to Dialog as necessary.
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = TZADWWKO.W_MetaDefUpdateObject.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szTempString_0, "" );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// Activate the Registered View and include it in the Dialog. It will have been created when the object was created.
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, TZADWWKO.W_MetaDefUpdateObject.Name, 0 )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
      nRC = ActivateMetaOI_ByName( ViewToWindow, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szTempString_0, 0 );
      //:NAME VIEW TZWDVORO "UpdateObjectView"
      SetNameForView( TZWDVORO, "UpdateObjectView", 0, zLEVEL_TASK );
      //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
      RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
      //:DropObjectInstance( TZWDVORO )
      DropObjectInstance( TZWDVORO );
   } 

   //:END 

   //:// The Clone above creates the Save/Cancel or Return Actions and Operations, but they need to have their names converted.
   //:FOR EACH TZWINDOWL.Action 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOWL.Action.Tag
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
      //:zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOWL.Action.Tag = szActionName 
      SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
      RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
   } 

   //:END

   //:// Copy Save/Cancel or Return Operations and tie them to the Action by the same name.
   //:// Also convert the _AD_Area characters in both source file and Dialog.
   //:// The Clone should create an Operation Source entry by the same name as the AD_Base dialog.
   //:CreateViewFromView( AD_BaseSrc, TZWINDOW )
   CreateViewFromView( &AD_BaseSrc, TZWINDOW );
   //:NAME VIEW AD_BaseSrc "AD_BaseSrc"
   SetNameForView( AD_BaseSrc, "AD_BaseSrc", 0, zLEVEL_TASK );
   //:IF TZADWWKO.AutoDesignWork.SelectedPageType = "U"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedPageType", "U" ) == 0 )
   { 
      //:SET CURSOR FIRST AD_BaseSrc.SourceFile WHERE AD_BaseSrc.SourceFile.Name = "ObjUpdat"
      RESULT = SetCursorFirstEntityByString( AD_BaseSrc, "SourceFile", "Name", "ObjUpdat", "" );
      //:ELSE
   } 
   else
   { 
      //:SET CURSOR FIRST AD_BaseSrc.SourceFile WHERE AD_BaseSrc.SourceFile.Name = "ObjDtl"
      RESULT = SetCursorFirstEntityByString( AD_BaseSrc, "SourceFile", "Name", "ObjDtl", "" );
   } 

   //:END
   //:FOR EACH AD_BaseSrc.Operation 
   RESULT = SetCursorFirstEntity( AD_BaseSrc, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = AD_BaseSrc.Operation.Name
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, AD_BaseSrc, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area );
      //:SET CURSOR FIRST TZWINDOW.Operation WHERE TZWINDOW.Operation.Name = szOperationName 
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "Operation", "Name", szOperationName, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR LAST TZWINDOW.Operation
         RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
         //:TZWINDOW.Operation.Name = szOperationName
         SetAttributeFromString( TZWINDOW, "Operation", "Name", szOperationName );
         //:SetMatchingAttributesByName( TZWINDOW, "Operation", AD_BaseSrc, "Operation", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOW, "Operation", AD_BaseSrc, "Operation", zSET_NULL );
         //:FOR EACH AD_BaseSrc.Parameter 
         RESULT = SetCursorFirstEntity( AD_BaseSrc, "Parameter", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOW, "Parameter", AD_BaseSrc, "Parameter", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOW, "Parameter", AD_BaseSrc, "Parameter", zSET_NULL );
            RESULT = SetCursorNextEntity( AD_BaseSrc, "Parameter", "" );
         } 

         //:END
         //:// If the Operation is for an Action in the new window, exclude the original and include the new.
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = szOperationName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szOperationName, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:EXCLUDE TZWINDOWL.ActOper 
            RESULT = ExcludeEntity( TZWINDOWL, "ActOper", zREPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOW.Operation 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The operation already exists, so we need to tie it to the Action by the same name, if it exists.
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = szOperationName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szOperationName, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:EXCLUDE TZWINDOWL.ActOper 
            RESULT = ExcludeEntity( TZWINDOWL, "ActOper", zREPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOW.Operation 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( AD_BaseSrc, "Operation", "" );
      //:END 
   } 

   //:END

   //:// Check if the Operations have already been copied to the main Source File. If not, copy them with the corrected names.

   //:// Check to see if the Operation, "SAVE__AD_Area", exists in the VML File, where _AD_Area is replaced by Area Name.
   //:// Note that the Operation Name starts in column 1.
   //:IF TZADWWKO.AutoDesignWork.SelectedPageType = "U"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedPageType", "U" ) == 0 )
   { 
      //:szCheckForOperation = "SAVE_" + szAD_Area
      ZeidonStringCopy( szCheckForOperation, 1, 0, "SAVE_", 1, 0, 257 );
      ZeidonStringConcat( szCheckForOperation, 1, 0, szAD_Area, 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szCheckForOperation = "RETURN_" + szAD_Area
      ZeidonStringCopy( szCheckForOperation, 1, 0, "RETURN_", 1, 0, 257 );
      ZeidonStringConcat( szCheckForOperation, 1, 0, szAD_Area, 1, 0, 257 );
   } 

   //:END
   //:nLength = zGetStringLen( szCheckForOperation )
   nLength = zGetStringLen( szCheckForOperation );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_1, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn   = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
   //:nRC2 = -1
   nRC2 = -1;
   //:IF nFileIn > 0   // Don't continue if the VML hasn't been created yet.
   if ( nFileIn > 0 )
   { 
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:zstrncpy( szCompare, szVML_Statement, nLength )
         strncpy_s( szCompare,  zsizeof( szCompare ), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szCheckForOperation )
         nRC2 = zstrcmp( szCompare, szCheckForOperation );
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
   } 

   //:END

   //:IF nRC2 != 0     // Operation was not found.
   if ( nRC2 != 0 )
   { 
      //:// The Operation was not found in the target Source File, so copy the VML
      //:szAD_UpdObj       = TZADWWKO.W_MetaDefUpdateObject.Name
      GetVariableFromAttribute( szAD_UpdObj, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //:szFindVML         = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML" 
      ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szFindVML, 1, 0, szTempString_2, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
      //:IF TZADWWKO.AutoDesignWork.SelectedPageType = "U"
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedPageType", "U" ) == 0 )
      { 


         //:// If this is the MGP for an original szGroupType = "ListGroupwMGP", then we must use the ObjMGP2 VML
         //:// source and set up the RelinkSubobject replace variables. We determine this by the existance of the
         //:// TZADWWKOMain OI, which we will also use for some of the variables.
         //:// DonC 4/11/2024: I'm not sure the ListGroupwMGP logic is still correct.
         //:GET VIEW TZADWWKOMn NAMED "TZADWWKOMain"
         RESULT = GetViewByName( &TZADWWKOMn, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
         //:IF RESULT < 0
         if ( RESULT < 0 )
         { 
            //:// Which save VML to use depends on whether or not there is a returned list object. If there is such
            //:// an object, we need to follow the save with a relink of the returned list object.
            //:// Also, if the returned list is for CRM, then the object view name is "mConList" + the Area Suffix.
            //:IF TZADWWKO.W_MetaDefReturnedObject EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefReturnedObject" );
            if ( lTempInteger_2 == 0 )
            { 
               //:// There is a return list object.
               //:szAD_UpdE = UpdateLOD.LOD_EntityParent.Name               // Relink source is root Entity of Update LOD.
               GetVariableFromAttribute( szAD_UpdE, 0, 'S', 51, UpdateLOD, "LOD_EntityParent", "Name", "", 0 );
               //:szAD_RetE = TZADWWKO.LOD_EntityReturnedObjectInclude.Name // Returned Entity
               GetVariableFromAttribute( szAD_RetE, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
               //:IF TZADWWKO.AutodesignSubdialog.FindType = "CRM"
               if ( CompareAttributeToString( TZADWWKO, "AutodesignSubdialog", "FindType", "CRM" ) == 0 )
               { 
                  //:// The returned object is mConList + suffix.
                  //:szAD_RetO = "mConList" + szAD_Suffix                   // Returned Entity Name
                  ZeidonStringCopy( szAD_RetO, 1, 0, "mConList", 1, 0, 51 );
                  ZeidonStringConcat( szAD_RetO, 1, 0, szAD_Suffix, 1, 0, 51 );
                  //:szOriginalVML = "c:\lplr\AD_Base\ObjUpdcl.VML"         // Use Contact List VML
                  ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjUpdcl.VML", 1, 0, 257 );
                  //:ELSE
               } 
               else
               { 
                  //:// The returned list is identified as TZADWWKO.W_MetaDefReturnedObject.
                  //:szAD_RetO = TZADWWKO.W_MetaDefReturnedObject.Name      // Returned Entity Name
                  GetVariableFromAttribute( szAD_RetO, 0, 'S', 51, TZADWWKO, "W_MetaDefReturnedObject", "Name", "", 0 );
                  //:szOriginalVML = "c:\lplr\AD_Base\ObjUpdRt.VML"         // Use Returned List VML
                  ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjUpdRt.VML", 1, 0, 257 );
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:// There is no return list object, so use VML source without Relink.
               //:szOriginalVML = "c:\lplr\AD_Base\ObjUpdat.VML"
               ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjUpdat.VML", 1, 0, 257 );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:szOriginalVML = "c:\lplr\AD_Base\ObjMGP2.VML"
            ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjMGP2.VML", 1, 0, 257 );
            //:// Check to see if an Activate entity was specifified. If so use it. Otherwise use the List entity.
            //:IF TZADWWKOMn.ESG_LOD_Entity2 EXISTS    // Check to see if an Activate Entity was specifified.
            lTempInteger_3 = CheckExistenceOfEntity( TZADWWKOMn, "ESG_LOD_Entity2" );
            if ( lTempInteger_3 == 0 )
            { 
               //:szMainE = TZADWWKOMn.ESG_LOD_Entity2.Name
               GetVariableFromAttribute( szMainE, 0, 'S', 51, TZADWWKOMn, "ESG_LOD_Entity2", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:szMainE = TZADWWKOMn.ESG_LOD_Entity.Name
               GetVariableFromAttribute( szMainE, 0, 'S', 51, TZADWWKOMn, "ESG_LOD_Entity", "Name", "", 0 );
            } 

            //:END

            //:szMainV   = TZADWWKOMn.W_MetaDefUpdateView.Name
            GetVariableFromAttribute( szMainV, 0, 'S', 51, TZADWWKOMn, "W_MetaDefUpdateView", "Name", "", 0 );
            //:szAD_UpdE = UpdateLOD.LOD_EntityParent.Name    // Root Entity of Update LOD.
            GetVariableFromAttribute( szAD_UpdE, 0, 'S', 51, UpdateLOD, "LOD_EntityParent", "Name", "", 0 );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:szOriginalVML = "c:\lplr\AD_Base\ObjDtl.VML"
         ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjDtl.VML", 1, 0, 257 );
      } 

      //:END
      //:ReturnID_NameForEntity( TZADWWKO, szAD_ID, szAD_UpdE, UpdateLOD )  // Get Identifier Name for UpdateLOD LOD & Entity szAD_UpdE
      oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_ID, szAD_UpdE, UpdateLOD );

      //:// Copy the source VML to the end of the Find VML, converting the variable characters in the process.
      //:nFileIn   = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area",  szAD_Area )    // Area Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO",  szAD_UpdObj )  // Update Object Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdObj );
         //:zSearchAndReplace( szVML_Statement, 256, "_MainV",    szMainV )      // The Main Object View with original listbox
         zSearchAndReplace( szVML_Statement, 256, "_MainV", szMainV );
         //:zSearchAndReplace( szVML_Statement, 256, "_MainE",    szMainE )      // The Main Object Entity for Activate
         zSearchAndReplace( szVML_Statement, 256, "_MainE", szMainE );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetO",  szAD_RetO )    // The Main Object View with original listbox
         zSearchAndReplace( szVML_Statement, 256, "_AD_RetO", szAD_RetO );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetE",  szAD_RetE )    // The Main Object Entity for Activate
         zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdE",  szAD_UpdE )    // The Root Entity of the MGP Update LOD.
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdE", szAD_UpdE );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Suff",  szAD_Suffix )  // The Root Entity of the MGP Update LOD.
         zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_ID",    szAD_ID )      // Update Identifier Name for object and entity above.
         zSearchAndReplace( szVML_Statement, 256, "_AD_ID", szAD_ID );

         //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
         SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );

         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:SysCloseFile( TZWINDOW, nFileOut, 0 )
      SysCloseFile( TZWINDOW, nFileOut, 0 );
   } 

   //:END

   //:// The Operation copy above copied the two operations under the original SourceFile.
   //:// So we just need to delete the ObjUpdat SourceFile and its subordinate Operations.
   //:DELETE ENTITY TZWINDOWL.SourceFile  
   RESULT = DeleteEntity( TZWINDOWL, "SourceFile", zPOS_NEXT );
   //:DropView( AD_BaseSrc )
   DropView( AD_BaseSrc );

   //:// MAIN WINDOW GENERATION

   //:// Build each Group based on the SubGroup type and content.
   //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
   { 
      //:szTemplateWindow = "ObjectUpdateGroupsIcons" 
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroupsIcons", 1, 0, 51 );
      //:ELSE
   } 
   else
   { 
      //:szTemplateWindow = "ObjectUpdateGroups"
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroups", 1, 0, 51 );
   } 

   //:END
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = szTemplateWindow
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szTemplateWindow, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The template window, " + szTemplateWindow + ", could not be found in AD_Base."
      ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTemplateWindow, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 201 );
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:CreateViewFromView( TZCONTROL, TZWINDOWL )    // TZWINDOWL is positioned on the Window being built.
   CreateViewFromView( &TZCONTROL, TZWINDOWL );
   //:NAME VIEW TZCONTROL "TZCONTROLAD"
   SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
   //:CreateViewFromView( AD_BaseCtl, AD_Base )
   CreateViewFromView( &AD_BaseCtl, AD_Base );
   //:NAME VIEW  AD_BaseCtl "AD_BaseCtl"
   SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );

   //:// We are assuming Bootstrap and must step down to the level which contain the SubGroups from GenEntitySubGroup.
   //:// Position on the last and innermost Group, which will contain the Subgroups.
   //:// We assume there is always at least one Group
   //:SET CURSOR LAST TZCONTROL.Control
   RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
   //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SET CURSOR LAST TZCONTROL.Control
   RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
   //:IF TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_4 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   if ( lTempInteger_4 == 0 )
   { 
      //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:SET CURSOR LAST TZCONTROL.Control
      RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
      //:IF TZCONTROL.CtrlCtrl EXISTS
      lTempInteger_5 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
      if ( lTempInteger_5 == 0 )
      { 
         //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:SET CURSOR LAST TZCONTROL.Control
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:IF TZCONTROL.CtrlCtrl EXISTS
         lTempInteger_6 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
         if ( lTempInteger_6 == 0 )
         { 
            //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
            nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:SET CURSOR LAST TZCONTROL.Control
            RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         } 

         //:END
      } 

      //:END
   } 

   //:END
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )   // We need to go down one additional level.
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );

   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
      GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
      //:TraceLineS( "*** szGroupType: ", szGroupType )
      TraceLineS( "*** szGroupType: ", szGroupType );

      //:// Use reusable operation, GenEntitySubGroup, to build out the Group.
      //:nRC = GenEntitySubGroup( TZADWWKO, TZCONTROL, TZWINDOWL, AD_Base, szGroupType, szAD_Area )
      nRC = oTZADWWKO_GenEntitySubGroup( TZADWWKO, TZCONTROL, TZWINDOWL, AD_Base, szGroupType, szAD_Area );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
      //:END
   } 

   //:END
   //:// Always reset to top Group.
   //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:LOOP WHILE nRC = 0
   while ( nRC == 0 )
   { 
      //:nRC = ResetViewFromSubobject( TZCONTROL )
      nRC = ResetViewFromSubobject( TZCONTROL );
   } 

   //:END

   //:// Reposition and Resize all Group Controls.
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL );

   //:// Go to rename some Controls to avoid duplicate Tags.
   //:SET CURSOR FIRST TZWINDOWL.Control
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:RenameAD_Controls( TZADWWKO, TZWINDOWL ) 
   oTZADWWKO_RenameAD_Controls( TZADWWKO, TZWINDOWL );

   //:DropView( TZCONTROL )
   DropView( TZCONTROL );
   //:DropView( AD_BaseCtl )
   DropView( AD_BaseCtl );


   //:// SUBWINDOW GENERATION

   //:// GENERATE SUBORDINATE PAGES BASED ON GROUP TYPE.

   //:// For each EntitySubGroup of type ListGroupWUpdatePage or ListGroupWDetailPage, build a Subwindow.
   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
      GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
      //:IF szGroupType = "ListGroupWUpdatePage" OR szGroupType = "ListGroupWDetailPage"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 )
      { 

         //:// GROUP LIST UPDATE/DETAIL PAGE

         //:GenListDetailPage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow )
         oTZADWWKO_GenListDetailPage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow );

         //:ELSE
      } 
      else
      { 

         //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude" OR 
         //:szGroupType = "ListGroupwInclude"   OR szGroupType = "ListGroupwFindInclude"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 ||
              ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
         { 

            //:// GROUP LIST OR DETAIL INCLUDE PAGE WITH OR WITHOUT FIND

            //:GenIncludePage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow )  
            oTZADWWKO_GenIncludePage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow );
         } 


         //:END
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
      //:END
   } 

   //:END

   //:// If this is NOT part of a Find autodesgin, drop the work object so that it will be create anew.
   //:IF TZADWWKO.FlatSelectedSearchAttribute DOES NOT EXIST
   lTempInteger_7 = CheckExistenceOfEntity( TZADWWKO, "FlatSelectedSearchAttribute" );
   if ( lTempInteger_7 != 0 )
   { 
      //:DropObjectInstance( TZADWWKO )
      DropObjectInstance( TZADWWKO );
   } 

   //:END

   //:// Drop the UpdateLOD if it exists.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:DropObjectInstance( UpdateLOD )
      DropObjectInstance( UpdateLOD );
   } 

   //:END

   //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
   //:             "Generate MultiGroup pages is complete.",
   //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
   MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Generate MultiGroup pages is complete.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:GOTO_AD_GroupUpdate( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
GOTO_AD_GroupUpdate( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_Base   REGISTERED AS AD_Base
   zVIEW     AD_Base = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 30 ) szGroupType
   zCHAR     szGroupType[ 31 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );

   //:// The window for transfer depends on the SubType of the Group.

   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
   GetVariableFromAttribute( szGroupType, 0, 'S', 31, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );

   //:IF szGroupType = "ListGroupOnly" 
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 31 ) == 0 )
   { 
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListOnlySpec" )
      SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListOnlySpec" );
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "ListGroupWDetailPage" OR szGroupType = "ListGroupWUpdatePage"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 31 ) == 0 )
      { 
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwDetailSpec" )
         SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwDetailSpec" );
         //:ELSE
      } 
      else
      { 
         //:IF szGroupType = "DetailGroup" 
         if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroup", 1, 0, 31 ) == 0 )
         { 
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailOnlySpec" )
            SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailOnlySpec" );
            //:ELSE
         } 
         else
         { 
            //:IF szGroupType = "DetailGroupwInclude"
            if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 31 ) == 0 )
            { 
               //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailwIncludeSpec" )
               SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailwIncludeSpec" );
               //:ELSE
            } 
            else
            { 
               //:IF szGroupType = "DetailGroupwFindInclude"
               if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 31 ) == 0 )
               { 
                  //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailwFindIncludeSpec" )
                  SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailwFindIncludeSpec" );
                  //:ELSE
               } 
               else
               { 
                  //:IF szGroupType = "ListGroupwFindInclude"
                  if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 31 ) == 0 )
                  { 
                     //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwFindIncludeSpec" )
                     SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwFindIncludeSpec" );
                     //:ELSE
                  } 
                  else
                  { 
                     //:IF szGroupType = "ListGroupwInclude"
                     if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 31 ) == 0 )
                     { 
                        //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwIncludeSpec" )
                        SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwIncludeSpec" );
                        //:ELSE
                     } 
                     else
                     { 
                        //:IF szGroupType = "ListGroupwMGP"
                        if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 31 ) == 0 )
                        { 
                           //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwMGP_Spec" )
                           SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwMGP_Spec" );
                           //:ELSE
                        } 
                        else
                        { 
                           //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
                           //:"A Valid Type must be selected.",
                           //:zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                           MessageSend( ViewToWindow, "", "Autodesign Subdialog", "A Valid Type must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                           //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
                           SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
                           //:RETURN -2
                           return( -2 );
                        } 

                        //:END
                     } 

                     //:END
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// Build the list of potential attributes for Group selection.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE 
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute2
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute2 NONE 
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute3
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute3 NONE 
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
   } 

   //:END
   //:// Which version of UpdateLOD we use depends on where we are in the 2 TZADWWKO OI's.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_UpdateMGP"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_UpdateMGP", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
      RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   } 

   //:END
   //:IF szGroupType = "ListGroupWIncludePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWIncludePage", 1, 0, 31 ) == 0 )
   { 
      //:SETUP_ESGL_MainIncludeObject( ViewToWindow )
      SETUP_ESGL_MainIncludeObject( ViewToWindow );
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "ListGroupWUpdatePage" OR szGroupType = "ListGroupWDetailPage"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 31 ) == 0 )
      { 
         //:IF TZADWWKO.EntitySubGroup.SavedGroupType = ""
         if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "SavedGroupType", "" ) == 0 )
         { 
            //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_0, "" );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF szGroupType = "ListGroupwMGP"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 31 ) == 0 )
         { 
            //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_1, "" );
            //:ELSE 
         } 
         else
         { 
            //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
            GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_2, "" );
         } 

         //:END
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_AutodesignFindDetailDialog( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignFindDetailDialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Build the FlatListPotentialEntity entries for Autodesign of Update Object.
   //:// We have one entry in the list for each Entity in the Update Object.

   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Update Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Update Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// First delete current entries.
   //:FOR EACH TZADWWKO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.FlatListPotentialEntity NONE
      RESULT = DeleteEntity( TZADWWKO, "FlatListPotentialEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   } 

   //:END

   //:// Create entry for each LOD_Entity in object.
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.FlatListPotentialEntity 
      RESULT = CreateEntity( TZADWWKO, "FlatListPotentialEntity", zPOS_AFTER );
      //:TZADWWKO.FlatListPotentialEntity.EntityName = UpdateLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADWWKO, "FlatListPotentialEntity", "EntityName", UpdateLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADWWKO.FlatListPotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatListPotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADWWKO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADWWKO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END

   //:// Set AD_Area and AD_Suffix (which are used in modifying VML & Dialog) from CRM values.
   //:TZADWWKO.AutodesignSubdialog.AD_AreaName   = TZADWWKO.AutodesignSubdialog.CRM_AreaName 
   SetAttributeFromAttribute( TZADWWKO, "AutodesignSubdialog", "AD_AreaName", TZADWWKO, "AutodesignSubdialog", "CRM_AreaName" );
   //:TZADWWKO.AutodesignSubdialog.AD_AreaSuffix = TZADWWKO.AutodesignSubdialog.CRM_AreaSuffix 
   SetAttributeFromAttribute( TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix", TZADWWKO, "AutodesignSubdialog", "CRM_AreaSuffix" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ReturnAttrControlType( VIEW AnyView,
//:                       STRING ( 32 ) szDomainName,
//:                       STRING ( 32 ) szDomainType,
//:                       STRING ( 10 ) szControlType,
//:                       STRING ( 1 )  szUpdateFlag )
static zVOID
o_ReturnAttrControlType( zVIEW     AnyView,
                         zPCHAR    szDomainName,
                         zPCHAR    szDomainType,
                         zPCHAR    szControlType,
                         zPCHAR    szUpdateFlag )
{


   //:// Return the Control Type based on the Domain and whether or not the Update Flag is set.
   //:// A Domain of Y/N always returns a Checkbox regardless of Update Flag.
   //:IF szDomainName = "Y/N"
   if ( ZeidonStringCompare( szDomainName, 1, 0, "Y/N", 1, 0, 33 ) == 0 )
   { 
      //:// Domain is Y/N.
      //:szControlType   = "CheckBox"
      ZeidonStringCopy( szControlType, 1, 0, "CheckBox", 1, 0, 11 );
      //:ELSE
   } 
   else
   { 
      //:IF szUpdateFlag = "Y"
      if ( ZeidonStringCompare( szUpdateFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:IF szDomainName = "Date" OR szDomainName = "DateTime"
         if ( ZeidonStringCompare( szDomainName, 1, 0, "Date", 1, 0, 33 ) == 0 || ZeidonStringCompare( szDomainName, 1, 0, "DateTime", 1, 0, 33 ) == 0 )
         { 
            //:// A Domain of Date will make the control a Calendar.
            //:szControlType = "Calendar"
            ZeidonStringCopy( szControlType, 1, 0, "Calendar", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:IF szDomainType = "T"
            if ( ZeidonStringCompare( szDomainType, 1, 0, "T", 1, 0, 33 ) == 0 )
            { 
               //:// The Domain is a table, so make control a Combobox.
               //:szControlType = "ComboBox"
               ZeidonStringCopy( szControlType, 1, 0, "ComboBox", 1, 0, 11 );
               //:ELSE
            } 
            else
            { 
               //:IF szDomainName = "Note"
               if ( ZeidonStringCompare( szDomainName, 1, 0, "Note", 1, 0, 33 ) == 0 )
               { 
                  //:// The Domain is Note, so make control a MLEdit.
                  //:szControlType = "MLEdit"
                  ZeidonStringCopy( szControlType, 1, 0, "MLEdit", 1, 0, 11 );
                  //:ELSE
               } 
               else
               { 
                  //:// If not a table, make control an Editbox.
                  //:szControlType = "EditBox"
                  ZeidonStringCopy( szControlType, 1, 0, "EditBox", 1, 0, 11 );
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The entity is not updatable, so Control Type is "Text"
         //:szControlType = "Text"
         ZeidonStringCopy( szControlType, 1, 0, "Text", 1, 0, 11 );
      } 

      //:END
   } 

   //:END
   return;
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_SearchAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_SearchAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected potential Search attribute entry as a selected Search attribute.
   //:// The potential Search attributes are defined in subobject ESG_FlatListPotentialAttribute3.
   //:// The selected Search attributes are defined in subobject ESG_DetailLOD_Attribute.
   //:// We will add them sequentially to the current Search position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute3 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute 
         //:           WHERE TZADWWKO.ESG_SearchLOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute3.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute3", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_SearchLOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_SearchLOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_SearchLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute3  
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_SearchLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute3", zPOS_AFTER );

            //:// We will always use the Prompt value for Search.
            //:TZADWWKO.ESG_SearchLOD_Attribute.PromptValue         = TZADWWKO.ESG_FlatListPotentialAttribute3.PromptValue 
            SetAttributeFromAttribute( TZADWWKO, "ESG_SearchLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute3", "PromptValue" );
            //:TZADWWKO.ESG_SearchLOD_Attribute.PromptLength        = TZADWWKO.ESG_FlatListPotentialAttribute3.PromptLength 
            SetAttributeFromAttribute( TZADWWKO, "ESG_SearchLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute3", "PromptLength" );
            //:TZADWWKO.ESG_SearchLOD_Attribute.ControlType         = TZADWWKO.ESG_FlatListPotentialAttribute3.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "ESG_SearchLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute3", "ControlType" );
            //:TZADWWKO.ESG_SearchLOD_Attribute.ControlWidth        = TZADWWKO.ESG_FlatListPotentialAttribute3.ControlWidth
            SetAttributeFromAttribute( TZADWWKO, "ESG_SearchLOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute3", "ControlWidth" );
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_ESGL_SearchAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_SearchAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Search entries.
   //:// The selected Search attributes are defined in subobject ESG_SearchLOD_Attribute.
   //:FOR EACH TZADWWKO.ESG_SearchLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_SearchLOD_Attribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_SearchLOD_Attribute" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:EXCLUDE TZADWWKO.ESG_SearchLOD_Attribute NONE
         RESULT = ExcludeEntity( TZADWWKO, "ESG_SearchLOD_Attribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_IncludeAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_IncludeAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected potential Include attribute entry as a selected Include attribute.
   //:// The potential Include attributes are defined in subobject ESG_FlatListPotentialAttribute2.
   //:// The selected Include attributes are defined in subobject ESG_List2LOD_Attribute.
   //:// We will add them sequentially to the current Include position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute2 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_List2LOD_Attribute  
         //:           WHERE TZADWWKO.ESG_List2LOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute2.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute2", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_List2LOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_List2LOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_List2LOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_List2LOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute2 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_List2LOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute2", zPOS_AFTER );
            //:// We will use the list value if it exists. Otherwise, we will use the Prompt value.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleValue != ""
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleValue", "" ) != 0 )
            { 
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleValue" );
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleLength" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptValue" );
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptLength" );
            } 

            //:END
            //:TZADWWKO.ESG_List2LOD_Attribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlType" );
            //:TZADWWKO.ESG_List2LOD_Attribute.ControlWidth = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlWidth
            SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlWidth" );
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_ESGL_IncludeAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_IncludeAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Include entries.
   //:// The selected Include attributes are defined in subobject ESG_List2LOD_Attribute.
   //:FOR EACH TZADWWKO.ESG_List2LOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_List2LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_List2LOD_Attribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_List2LOD_Attribute" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:EXCLUDE TZADWWKO.ESG_List2LOD_Attribute NONE
         RESULT = ExcludeEntity( TZADWWKO, "ESG_List2LOD_Attribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_List2LOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SETUP_ESGL_MainIncludeObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SETUP_ESGL_MainIncludeObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:INTEGER       nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set up the potential attributes for the Main Include list object.

   //:// Get Main Update object view.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:IF TZADWWKO.W_MetaDefUpdateObject EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
      if ( lTempInteger_0 == 0 )
      { 
         //:SELECT_AutodesignGroupObject( ViewToWindow )
         SELECT_AutodesignGroupObject( ViewToWindow );
      } 

      //:END
   } 

   //:END

   //:// Build the ESG_FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
   //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
   o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_0, "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_MainInclAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_MainInclAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected potential Include attribute entry as a selected Include attribute.
   //:// The potential Include attributes are defined in subobject ESG_FlatListPotentialAttribute.
   //:// The selected Include attributes are defined in subobject ESG_ListLOD_Attribute.
   //:// We will add them sequentially to the current Include position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_ListLOD_Attribute  
         //:           WHERE TZADWWKO.ESG_ListLOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_ListLOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_ListLOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
            //:// We will use the list value if it exists. Otherwise, we will use the Prompt value.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute.ListTitleValue != ""
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute", "ListTitleValue", "" ) != 0 )
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute.ListTitleValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "ListTitleValue" );
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute.ListTitleLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute", "ListTitleLength" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute.PromptValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptValue" );
               //:TZADWWKO.ESG_ListLOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute.PromptLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptLength" );
            } 

            //:END
            //:// ControlType is Text if Entity isn't updateable.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute.Create = "Y"
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute", "Create", "Y" ) == 0 )
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Text"
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Text" );
            } 

            //:END
            //:TZADWWKO.ESG_ListLOD_Attribute.ControlWidth = TZADWWKO.ESG_FlatListPotentialAttribute.ControlWidth 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlWidth" );
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   //:TraceLineS("TEST FOR KJS COMPILE", "")
   TraceLineS( "TEST FOR KJS COMPILE", "" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:AddRegisteredViewName( VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                       VIEW TZZOLODO BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) szObjectViewName )

//:   VIEW TZWDVORO BASED ON LOD TZWDVORO
static zSHORT
o_AddRegisteredViewName( zVIEW     TZWINDOW,
                         zVIEW     TZZOLODO,
                         zPCHAR    szObjectViewName )
{
   zVIEW     TZWDVORO = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    RESULT; 


   //:// Activate the Registered View and include it in the Dialog, creating it if necessary.
   //:nRC = ActivateMetaOI_ByName( TZWINDOW, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szObjectViewName, 0 )
   nRC = ActivateMetaOI_ByName( TZWINDOW, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szObjectViewName, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:// Create the ViewObjRef.
      //:ActivateEmptyMetaOI( TZWINDOW, TZWDVORO, zSOURCE_VOR_META, zSINGLE )
      ActivateEmptyMetaOI( TZWINDOW, &TZWDVORO, zSOURCE_VOR_META, zSINGLE );
      //:NAME VIEW TZWDVORO "TZWDVORO_ToInclude"
      SetNameForView( TZWDVORO, "TZWDVORO_ToInclude", 0, zLEVEL_TASK );
      //:CreateMetaEntity( TZWINDOW, TZWDVORO, "ViewObjRef", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWDVORO, "ViewObjRef", zPOS_AFTER );
      //:TZWDVORO.ViewObjRef.Name = szObjectViewName
      SetAttributeFromString( TZWDVORO, "ViewObjRef", "Name", szObjectViewName );
      //:TZWDVORO.ViewObjRef.Desc = "Created during AD Build"
      SetAttributeFromString( TZWDVORO, "ViewObjRef", "Desc", "Created during AD Build" );
      //:INCLUDE TZWDVORO.LOD FROM TZZOLODO.LOD 
      RESULT = IncludeSubobjectFromSubobject( TZWDVORO, "LOD", TZZOLODO, "LOD", zPOS_AFTER );
      //:CommitMetaOI( TZWINDOW, TZWDVORO, zSOURCE_VOR_META )
      CommitMetaOI( TZWINDOW, TZWDVORO, zSOURCE_VOR_META );
      //:ELSE
   } 
   else
   { 
      //:NAME VIEW TZWDVORO "TZWDVORO_ToInclude"
      SetNameForView( TZWDVORO, "TZWDVORO_ToInclude", 0, zLEVEL_TASK );
   } 

   //:END
   //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
   RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
   //:DropObjectInstance( TZWDVORO )
   DropObjectInstance( TZWDVORO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAD_MultiGroupPage( VIEW ViewToWindow )

//:   VIEW TZWINDOWL REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAD_MultiGroupPage( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZZOLFLO  BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW TZADWWKO  BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Set up work data necessary for autodesigning an AutodesignSubdialog.

   //:// Make sure TZADWWKO and the related autodesign objects exist in memory.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// The autodesign work objects need to be set up.

      //:// The TZADWWKO itself
      //:ACTIVATE TZADWWKO EMPTY 
      RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
      //:NAME VIEW TZADWWKO "TZADWWKO"
      SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADWWKO.AutoDesignWork 
      RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
      RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
      //:InitializeL_SymbolType( TZADWWKO ) 
      oTZADWWKO_InitializeL_SymbolType( TZADWWKO );

      //:// Remove any existing Autodesign entries from TZWINDOWL.
      //:IF TZWINDOWL.AutoDesignWindow EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AutoDesignWindow" );
      if ( lTempInteger_0 == 0 )
      { 
         //:DELETE ENTITY TZWINDOWL.AutoDesignWindow  
         RESULT = DeleteEntity( TZWINDOWL, "AutoDesignWindow", zPOS_NEXT );
      } 

      //:END
      //:CREATE ENTITY TZWINDOWL.AutoDesignWindow 
      RESULT = CreateEntity( TZWINDOWL, "AutoDesignWindow", zPOS_AFTER );
      //:CREATE ENTITY TZWINDOWL.AutoDesignGroup  
      RESULT = CreateEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER );

      //:// Also activate AD_Base to pick up reusable data values.
      //:nRC = ActivateAD_BaseL( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_BaseL( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base" 
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Make sure default for main page is update.
   //:TZADWWKO.AutoDesignWork.SelectedPageType = "U"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "SelectedPageType", "U" );

   //:// Make sure that the TZZOLFLO object of Meta View entries exists.
   //:GET VIEW TZZOLFLO NAMED "TZZOLFLO"
   RESULT = GetViewByName( &TZZOLFLO, "TZZOLFLO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZZOLFLO, TaskLPLR )
      CreateViewFromView( &TZZOLFLO, TaskLPLR );
      //:NAME VIEW TZZOLFLO "TZZOLFLO"
      SetNameForView( TZZOLFLO, "TZZOLFLO", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST TZZOLFLO.W_MetaType WHERE TZZOLFLO.W_MetaType.Type = 9   // 9 is read view of Registered View.
      RESULT = SetCursorFirstEntityByInteger( TZZOLFLO, "W_MetaType", "Type", 9, "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_MultiPageDetailObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_MultiPageDetailObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 50 ) szVOR_Name
   zCHAR     szVOR_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Just return if nothing was selected.
   //:IF TZADWWKO.W_MetaDefUpdateView DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
   if ( lTempInteger_0 != 0 )
   { 
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Activate the selected Object View.
   //:szVOR_Name = TZADWWKO.W_MetaDefUpdateView.Name 
   GetVariableFromAttribute( szVOR_Name, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateView", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szVOR_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szVOR_Name, 0 );
   //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
   SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );

   //:// Next Activate the LOD for the View.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( UpdateLOD )
      DropObjectInstance( UpdateLOD );
   } 

   //:END
   //:szLOD_Name = UpdateVOR.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW UpdateLOD "TZZOLODO_Update"
   SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   //:DropObjectInstance( UpdateVOR )
   DropObjectInstance( UpdateVOR );

   //:// Include the Object Meta, which may be used later.
   //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = szLOD_Name  
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szLOD_Name, "" );
   //:IF TZADWWKO.W_MetaDefUpdateObject EXISTS 
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
   if ( lTempInteger_1 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateObject
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateObject", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

   //:// Build the FlatListPotentialEntity entries from the update object just activated.
   //:// We have one entry in the list for each Entity in the Update Object.

   //:// First delete any current entries then rebuild them.
   //:FOR EACH TZADWWKO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.FlatListPotentialEntity NONE
      RESULT = DeleteEntity( TZADWWKO, "FlatListPotentialEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   } 

   //:END
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.FlatListPotentialEntity 
      RESULT = CreateEntity( TZADWWKO, "FlatListPotentialEntity", zPOS_AFTER );
      //:TZADWWKO.FlatListPotentialEntity.EntityName = UpdateLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADWWKO, "FlatListPotentialEntity", "EntityName", UpdateLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADWWKO.FlatListPotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatListPotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADWWKO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADWWKO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:OVERRIDE_ControlsForDetail( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
OVERRIDE_ControlsForDetail( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Override the Control Type for each selected Attribute to Text.
   //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute" ) 
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:TZADWWKO.ESG_DetailLOD_Attribute.ControlType = TZADWWKO.AutoDesignWork.SelectedControlType 
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType", TZADWWKO, "AutoDesignWork", "SelectedControlType" );
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute", 0 ) 
         SetSelectStateOfEntity( TZADWWKO, "ESG_DetailLOD_Attribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:OVERRIDE_ControlsForList1( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
OVERRIDE_ControlsForList1( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Override the Control Type for each selected Attribute to Text.
   //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" ) 
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = TZADWWKO.AutoDesignWork.SelectedControlType 
         SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZADWWKO, "AutoDesignWork", "SelectedControlType" );
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute", 0 ) 
         SetSelectStateOfEntity( TZADWWKO, "ESG_ListLOD_Attribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAutodesignGroup( VIEW ViewToWindow )

//:   VIEW TZWINDOWL  REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL  REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR   REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZZOLFLO   BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW TZADWWKO   BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:STRING ( 50 ) szListboxSymbol
   zCHAR     szListboxSymbol[ 51 ] = { 0 }; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );


   //:// Set up work data necessary for autodesigning a new or existing Group

   //:// Force recreation of the work object.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:szListboxSymbol = TZADWWKO.AutoDesignWork.SelectedListboxSymbolType    // Save the symbol value.
      GetVariableFromAttribute( szListboxSymbol, 0, 'S', 51, TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "", 0 );
      //:DropObjectInstance( TZADWWKO )
      DropObjectInstance( TZADWWKO );
   } 

   //:END

   //:// Initialize TZADWWKO

   //:// The TZADWWKO itself
   //:ACTIVATE TZADWWKO EMPTY 
   RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
   //:NAME VIEW TZADWWKO "TZADWWKO"
   SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
   //:CREATE ENTITY TZADWWKO.AutoDesignWork 
   RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
   //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
   RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
   //:CREATE ENTITY TZADWWKO.EntitySubGroup 
   RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
   //:InitializeL_SymbolType( TZADWWKO ) 
   oTZADWWKO_InitializeL_SymbolType( TZADWWKO );
   //:TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = szListboxSymbol   // Restore the original symbol value.
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", szListboxSymbol );

   //:// Default state will be resize and reposition top Groups.
   //:TZADWWKO.AutoDesignWork.RepositionGroupFlag = "Y"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "RepositionGroupFlag", "Y" );

   //:// Remove any existing Autodesign entries from TZWINDOWL.
   //:IF TZWINDOWL.AutoDesignWindow EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AutoDesignWindow" );
   if ( lTempInteger_0 == 0 )
   { 
      //:DELETE ENTITY TZWINDOWL.AutoDesignWindow  
      RESULT = DeleteEntity( TZWINDOWL, "AutoDesignWindow", zPOS_NEXT );
   } 

   //:END
   //:CREATE ENTITY TZWINDOWL.AutoDesignWindow 
   RESULT = CreateEntity( TZWINDOWL, "AutoDesignWindow", zPOS_AFTER );
   //:CREATE ENTITY TZWINDOWL.AutoDesignGroup  
   RESULT = CreateEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER );


   //:// Make sure AD_Base dialog is loaded.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:nRC = ActivateAD_BaseL( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_BaseL( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base" 
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Make sure that the TZZOLFLO object of Meta ViewObjRef entries exists and is sorted.
   //:GET VIEW TZZOLFLO NAMED "TZZOLFLO"
   RESULT = GetViewByName( &TZZOLFLO, "TZZOLFLO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZZOLFLO, TaskLPLR )
      CreateViewFromView( &TZZOLFLO, TaskLPLR );
      //:NAME VIEW TZZOLFLO "TZZOLFLO"
      SetNameForView( TZZOLFLO, "TZZOLFLO", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST TZZOLFLO.W_MetaType WHERE TZZOLFLO.W_MetaType.Type = 9   // 9 is read view of Registered View 
      RESULT = SetCursorFirstEntityByInteger( TZZOLFLO, "W_MetaType", "Type", 9, "" );
   } 

   //:END
   //:OrderEntityForView( TZZOLFLO, "W_MetaDef", "Name" )
   OrderEntityForView( TZZOLFLO, "W_MetaDef", "Name" );
   //:SET CURSOR FIRST TZZOLFLO.W_MetaDef  
   RESULT = SetCursorFirstEntity( TZZOLFLO, "W_MetaDef", "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_AutodesignGroupObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroupObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 50 ) szVOR_Name
   zCHAR     szVOR_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Activate the LOD for the selected Object View and name it TZZOLODO_Update. 
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( UpdateLOD )
      DropObjectInstance( UpdateLOD );
   } 

   //:END

   //:// Activate the selected Object View.
   //:szVOR_Name = TZADWWKO.W_MetaDefUpdateView.Name 
   GetVariableFromAttribute( szVOR_Name, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateView", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szVOR_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szVOR_Name, 0 );
   //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
   SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );

   //:// Next Activate the LOD for the View.
   //:szLOD_Name = UpdateVOR.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW UpdateLOD "TZZOLODO_Update"
   SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );

   //:// Include the Object Meta, which may be used later.
   //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = UpdateLOD.LOD.Name   
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
   //:IF TZADWWKO.W_MetaDefUpdateObject EXISTS 
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
   if ( lTempInteger_0 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateObject
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateObject", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

   //:// Build the list of selectable LOD Entities for a potential list box.
   //:// We have one entry in the list for each Entity in the Update Object.
   //:// First remove any current entries then rebuild them.
   //:FOR EACH TZADWWKO.PotentialLOD_Entity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "PotentialLOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE TZADWWKO.PotentialLOD_Entity NONE 
      RESULT = ExcludeEntity( TZADWWKO, "PotentialLOD_Entity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "PotentialLOD_Entity", "" );
   } 

   //:END
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.PotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "PotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END

   //:// Delete any current potential attributes and then go to build them.
   //:// All Attributes of the selected object are listed for GroupUpdate but only subordinate attributes for lists.
   //:IF TZADWWKO.EntitySubGroup.GroupType = "DetailGroupUpdate"
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroupUpdate" ) == 0 )
   { 
      //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
      o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_0, "" );
      //:ELSE
   } 
   else
   { 
      //:IF TZADWWKO.ESG_LOD_Entity EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_LOD_Entity" );
      if ( lTempInteger_1 == 0 )
      { 
         //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_1, "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_PromptCSS_ClassForAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_PromptCSS_ClassForAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 50 ) szCSS_ClassValue
   zCHAR     szCSS_ClassValue[ 51 ] = { 0 }; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set all  to the current value specified.
   //:szCSS_ClassValue = TZADWWKO.EntitySubGroup.CSS_Class 
   GetVariableFromAttribute( szCSS_ClassValue, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "CSS_Class", "", 0 );
   //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForPrompt = szCSS_ClassValue
      SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForPrompt", szCSS_ClassValue );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_CtrlCSS_ClassForAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_CtrlCSS_ClassForAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 50 ) szCSS_ClassValue
   zCHAR     szCSS_ClassValue[ 51 ] = { 0 }; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set all  to the current value specified.
   //:szCSS_ClassValue = TZADWWKO.EntitySubGroup.CSS_Class 
   GetVariableFromAttribute( szCSS_ClassValue, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "CSS_Class", "", 0 );
   //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForControl = szCSS_ClassValue
      SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForControl", szCSS_ClassValue );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_AutodesignGroupTemplate( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroupTemplate( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_Base  BASED ON LOD  TZWDLGSO 
   zVIEW     AD_Base = 0; 
   //:VIEW AD_Base2 BASED ON LOD  TZWDLGSO 
   zVIEW     AD_Base2 = 0; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   return( 0 );
//    
//    // Eliminated 7/17/2023 because it is no longer used.
//    // If the Group Type is "DetailGroupUpdate" and the Group selected has a Text/Control pair, set the 
//    // CSS_Class specification entries for Prompt and Control from those values.
//    /*IF TZADWWKO.EntitySubGroup.GroupType = "DetailGroupUpdate"
//       GET VIEW AD_Base NAMED "AD_Base"
//       CreateViewFromView(AD_Base2, AD_Base )
//       IF AD_Base2.CtrlCtrl EXISTS
//          SetViewToSubobject( AD_Base2, "CtrlCtrl" )
//          IF AD_Base2.CtrlCtrl EXISTS
//             SetViewToSubobject( AD_Base2, "CtrlCtrl" )
//             IF AD_Base2.CtrlCtrl EXISTS
//                SetViewToSubobject( AD_Base2, "CtrlCtrl" )
//             END
//          END
//       END
//       SET CURSOR FIRST AD_Base2.Control WHERE AD_Base2.ControlDef.Tag = "Text" 
//       IF RESULT >= zCURSOR_SET
//          TZADWWKO.EntitySubGroup.CSS_ClassForPrompt  = AD_Base2.Control.CSS_Class 
//          SET CURSOR NEXT AD_Base2.Control
//          TZADWWKO.EntitySubGroup.CSS_ClassForControl = AD_Base2.Control.CSS_Class
//       END
//       DropView( AD_Base2 )
//    END*/
// END
} 


//:LOCAL OPERATION
//:CloneControlAD( VIEW ViewToWindow,
//:                VIEW TZCONTROL  BASED ON LOD TZWDLGSO,
//:                VIEW TZWINDOWL  BASED ON LOD TZWDLGSO,
//:                VIEW AD_BaseCtl BASED ON LOD TZWDLGSO )

//:   VIEW TZPESRCO REGISTERED AS TZPESRCO
static zLONG
o_CloneControlAD( zVIEW     ViewToWindow,
                  zVIEW     TZCONTROL,
                  zVIEW     TZWINDOWL,
                  zVIEW     AD_BaseCtl )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", ViewToWindow, zLEVEL_TASK );

   //:// Clone the Control from AD_BaseCtl into TZCONTROL, without copying any mapping.

   //:// Copy this Control.
   //:CreateMetaEntity( ViewToWindow, TZCONTROL, "Control", zPOS_AFTER )
   CreateMetaEntity( ViewToWindow, TZCONTROL, "Control", zPOS_AFTER );
   //:SetMatchingAttributesByName( TZCONTROL,  "Control",
   //:                             AD_BaseCtl, "Control", zSET_NULL )
   SetMatchingAttributesByName( TZCONTROL, "Control", AD_BaseCtl, "Control", zSET_NULL );
   //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = AD_BaseCtl.ControlDef.Tag 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "ControlDef", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_0, "" );
   //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
   //:FOR EACH AD_BaseCtl.CtrlMap 
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "CtrlMap", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( ViewToWindow, TZCONTROL, "CtrlMap", zPOS_AFTER )
      CreateMetaEntity( ViewToWindow, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZCONTROL,  "CtrlMap",
      //:                             AD_BaseCtl, "CtrlMap", zSET_NULL )
      SetMatchingAttributesByName( TZCONTROL, "CtrlMap", AD_BaseCtl, "CtrlMap", zSET_NULL );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "CtrlMap", "" );
   } 

   //:END
   //:FOR EACH AD_BaseCtl.WebControlProperty 
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "WebControlProperty", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( ViewToWindow, TZCONTROL, "WebControlProperty", zPOS_AFTER )
      CreateMetaEntity( ViewToWindow, TZCONTROL, "WebControlProperty", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZCONTROL,  "WebControlProperty",
      //:                             AD_BaseCtl, "WebControlProperty", zSET_NULL )
      SetMatchingAttributesByName( TZCONTROL, "WebControlProperty", AD_BaseCtl, "WebControlProperty", zSET_NULL );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "WebControlProperty", "" );
   } 

   //:END

   //:// Clone each Event, as long as there is an Action.
   //:FOR EACH AD_BaseCtl.Event
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "Event", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF AD_BaseCtl.EventAct EXISTS AND AD_BaseCtl.EventAct.Tag != ""
      lTempInteger_0 = CheckExistenceOfEntity( AD_BaseCtl, "EventAct" );
      if ( lTempInteger_0 == 0 && CompareAttributeToString( AD_BaseCtl, "EventAct", "Tag", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = AD_BaseCtl.EventAct.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CreateMetaEntity( ViewToWindow, TZWINDOWL, "Action", zPOS_AFTER )
            CreateMetaEntity( ViewToWindow, TZWINDOWL, "Action", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOWL,  "Action",
            //:                             AD_BaseCtl, "EventAct", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOWL, "Action", AD_BaseCtl, "EventAct", zSET_NULL );
         } 

         //:END
         //:CreateMetaEntity( ViewToWindow, TZCONTROL, "Event", zPOS_AFTER )
         CreateMetaEntity( ViewToWindow, TZCONTROL, "Event", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZCONTROL,  "Event",
         //:                             AD_BaseCtl, "Event", zSET_NULL )
         SetMatchingAttributesByName( TZCONTROL, "Event", AD_BaseCtl, "Event", zSET_NULL );
         //:IncludeSubobjectFromSubobject( TZCONTROL, "EventAct",
         //:                               TZWINDOWL, "Action", zPOS_AFTER )
         IncludeSubobjectFromSubobject( TZCONTROL, "EventAct", TZWINDOWL, "Action", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( AD_BaseCtl, "Event", "" );
      //:END
   } 

   //:END

   //:// Clone subcontrols recursively.
   //:FOR EACH AD_BaseCtl.CtrlCtrl
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
      SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" ) 
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl );
      //:ResetViewFromSubobject( AD_BaseCtl )
      ResetViewFromSubobject( AD_BaseCtl );
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      RESULT = SetCursorNextEntity( AD_BaseCtl, "CtrlCtrl", "" );
      //:END
   } 

   //:END
   return( 0 );
//    
// END
} 


//:LOCAL OPERATION
//:BuildAD_GroupPotAttrListR( VIEW TZADWWKO     BASED ON LOD TZADWWKO,
//:                           VIEW MainLOD      BASED ON LOD TZZOLODO,
//:                           VIEW RecursiveLOD BASED ON LOD TZZOLODO,
//:                           STRING ( 32 ) szStartEntityName,     // The Entity Name at which recursive processing starts
//:                           STRING ( 1 )  szFlatListNameSuffix,  // ESG_FlatListPotentialAttribute suffix as explained below
//:                           INTEGER       lCurrentLevel,         // The current recursive level of the LOD_ParentEntity
//:                           INTEGER       lStartLevel,           // The recursive level at which attributes will be copied
//:                           STRING ( 50 ) szIndentSpaces )       // Spaces for indenting the name shown in the list. They begin with lStartLevel

//:   STRING ( 80 ) szIndentEntityName
static zVOID
o_BuildAD_GroupPotAttrListR( zVIEW     TZADWWKO,
                             zVIEW     MainLOD,
                             zVIEW     RecursiveLOD,
                             zPCHAR    szStartEntityName,
                             zPCHAR    szFlatListNameSuffix,
                             zLONG     lCurrentLevel,
                             zLONG     lStartLevel,
                             zPCHAR    szIndentSpaces )
{
   zCHAR     szIndentEntityName[ 81 ] = { 0 }; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szFlatListPotAttributeName
   zCHAR     szFlatListPotAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szESG_FlatLOD_AttributeName
   zCHAR     szESG_FlatLOD_AttributeName[ 33 ] = { 0 }; 
   //:STRING ( 90 ) szPromptText
   zCHAR     szPromptText[ 91 ] = { 0 }; 
   //:STRING ( 90 ) szPromptTextSource
   zCHAR     szPromptTextSource[ 91 ] = { 0 }; 
   //:STRING ( 10 ) szControlType
   zCHAR     szControlType[ 11 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 2 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 2 ]; 


   //:// Build the ESG_FlatListPotentialAttribute entries from the subobject starting with Entity Name, szStartEntityName, and continuing
   //:// for all entities with a higher recursive level.
   //:// Which ESG_FlatListPotentialAttribute subobject to use depends upon the suffix passed of null, "1" or "2".

   //:szEntityName       = RecursiveLOD.LOD_EntityParent.Name 
   GetVariableFromAttribute( szEntityName, 0, 'S', 33, RecursiveLOD, "LOD_EntityParent", "Name", "", 0 );
   //:szIndentEntityName = RecursiveLOD.LOD_EntityParent.IndentName 
   GetVariableFromAttribute( szIndentEntityName, 0, 'S', 81, RecursiveLOD, "LOD_EntityParent", "IndentName", "", 0 );

   //:// Look for the Start Entity Name to set lStartLevel and begin copying attributes.
   //:// The lStartLevel will be zero until we've come across the Start Entity Name.
   //:IF lStartLevel = 0 AND RecursiveLOD.LOD_EntityParent.Name = szStartEntityName
   if ( lStartLevel == 0 && CompareAttributeToString( RecursiveLOD, "LOD_EntityParent", "Name", szStartEntityName ) == 0 )
   { 
      //:lStartLevel = lCurrentLevel
      lStartLevel = lCurrentLevel;
   } 

   //:END
   //:IF lStartLevel > 0 AND lCurrentLevel >= lStartLevel
   if ( lStartLevel > 0 && lCurrentLevel >= lStartLevel )
   { 

      //:szFlatListPotAttributeName  = "ESG_FlatListPotentialAttribute" + szFlatListNameSuffix
      ZeidonStringCopy( szFlatListPotAttributeName, 1, 0, "ESG_FlatListPotentialAttribute", 1, 0, 33 );
      ZeidonStringConcat( szFlatListPotAttributeName, 1, 0, szFlatListNameSuffix, 1, 0, 33 );
      //:szESG_FlatLOD_AttributeName = "ESG_FlatLOD_Attribute"          + szFlatListNameSuffix
      ZeidonStringCopy( szESG_FlatLOD_AttributeName, 1, 0, "ESG_FlatLOD_Attribute", 1, 0, 33 );
      ZeidonStringConcat( szESG_FlatLOD_AttributeName, 1, 0, szFlatListNameSuffix, 1, 0, 33 );

      //:// Position on the LOD_Entity for the current LOD_EntityParent for copying Attributes because the order of the LOD_AttributeRec
      //:// entities isn't always correct.
      //:SET CURSOR FIRST MainLOD.LOD_Entity WHERE MainLOD.LOD_Entity.ZKey = RecursiveLOD.LOD_EntityParent.ZKey  
      GetIntegerFromAttribute( &lTempInteger_0, RecursiveLOD, "LOD_EntityParent", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( MainLOD, "LOD_Entity", "ZKey", lTempInteger_0, "" );
      //:FOR EACH MainLOD.LOD_Attribute
      RESULT = SetCursorFirstEntity( MainLOD, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateEntity( TZADWWKO, szFlatListPotAttributeName, zPOS_AFTER )
         CreateEntity( TZADWWKO, szFlatListPotAttributeName, zPOS_AFTER );
         //:szAttributeName = MainLOD.ER_Attribute.Name
         GetVariableFromAttribute( szAttributeName, 0, 'S', 33, MainLOD, "ER_Attribute", "Name", "", 0 );
         //:SetMatchingAttributesByName( TZADWWKO, szFlatListPotAttributeName, MainLOD, "LOD_Entity", zSET_ALL )
         SetMatchingAttributesByName( TZADWWKO, szFlatListPotAttributeName, MainLOD, "LOD_Entity", zSET_ALL );
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "EntityName", szEntityName )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "EntityName", szEntityName );
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "AttributeName", szAttributeName )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "AttributeName", szAttributeName );
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "IndentEntityName", szIndentEntityName )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "IndentEntityName", szIndentEntityName );
         //:IncludeSubobjectFromSubobject( TZADWWKO, szESG_FlatLOD_AttributeName, MainLOD, "LOD_Attribute", zPOS_AFTER )
         IncludeSubobjectFromSubobject( TZADWWKO, szESG_FlatLOD_AttributeName, MainLOD, "LOD_Attribute", zPOS_AFTER );

         //:// Create Control Type based on Update characteristic of Entity and Attribute Domain.
         //:// A Domain of Y/N will create a Checkbox even if the entity is not updatable.
         //:IF MainLOD.LOD_EntityParent.Update = "Y"
         if ( CompareAttributeToString( MainLOD, "LOD_EntityParent", "Update", "Y" ) == 0 )
         { 
            //:ReturnAttrControlType( MainLOD, MainLOD.Domain.Name, MainLOD.Domain.DomainType, szControlType, "Y" )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), MainLOD, "Domain", "Name" );
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), MainLOD, "Domain", "DomainType" );
            o_ReturnAttrControlType( MainLOD, szTempString_0, szTempString_1, szControlType, "Y" );
            //:ELSE
         } 
         else
         { 
            //:ReturnAttrControlType( MainLOD, MainLOD.Domain.Name, MainLOD.Domain.DomainType, szControlType, "" )
            GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), MainLOD, "Domain", "Name" );
            GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), MainLOD, "Domain", "DomainType" );
            o_ReturnAttrControlType( MainLOD, szTempString_2, szTempString_3, szControlType, "" );
         } 

         //:END
         //://TZADWWKO.ESG_FlatListPotentialAttribute.ControlType = szControlType
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ControlType", szControlType )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ControlType", szControlType );

         //:// The Data Width will always be set to 30.
         //:SetAttributeFromInteger( TZADWWKO, szFlatListPotAttributeName, "DataWidth", 5 )
         SetAttributeFromInteger( TZADWWKO, szFlatListPotAttributeName, "DataWidth", 5 );

         //:// Use Prompt Values if specified. Otherwise use Attribute Name.
         //:// If PromptPaintLit is specified, we'll set both PromptValue and ListTitleValue to it and then override ListTitleValue if
         //:// it is also set.
         //:IF MainLOD.ER_Attribute.PromptPaintLit != ""
         if ( CompareAttributeToString( MainLOD, "ER_Attribute", "PromptPaintLit", "" ) != 0 )
         { 
            //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptValue", MainLOD, "ER_Attribute", "PromptPaintLit" )
            SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptValue", MainLOD, "ER_Attribute", "PromptPaintLit" );
            //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "PromptPaintLit" )
            SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "PromptPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:szPromptTextSource = szAttributeName
            ZeidonStringCopy( szPromptTextSource, 1, 0, szAttributeName, 1, 0, 91 );
            //:InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 )
            InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 );
            //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "PromptValue", szPromptText )
            SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "PromptValue", szPromptText );
         } 

         //:END
         //:IF MainLOD.ER_Attribute.ListPaintLit != ""
         if ( CompareAttributeToString( MainLOD, "ER_Attribute", "ListPaintLit", "" ) != 0 )
         { 
            //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "ListPaintLit" )
            SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "ListPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:// Don't set it here if it was set from PromtValue.
            //:IF MainLOD.ER_Attribute.PromptPaintLit = ""
            if ( CompareAttributeToString( MainLOD, "ER_Attribute", "PromptPaintLit", "" ) == 0 )
            { 
               //:szPromptTextSource = szAttributeName
               ZeidonStringCopy( szPromptTextSource, 1, 0, szAttributeName, 1, 0, 91 );
               //:InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 )
               InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 );
               //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", szPromptText )
               SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", szPromptText );
            } 

            //:END
         } 

         //:END
         //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptLength", MainLOD, "ER_Attribute", "PromptPaintLth" )
         SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptLength", MainLOD, "ER_Attribute", "PromptPaintLth" );
         //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleLength", MainLOD, "ER_Attribute", "ListPaintLth" )
         SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleLength", MainLOD, "ER_Attribute", "ListPaintLth" );
         RESULT = SetCursorNextEntity( MainLOD, "LOD_Attribute", "" );
      } 

      //:END
   } 

   //:END

   //:// Process subentities.
   //:FOR EACH RecursiveLOD.LOD_EntityChild
   RESULT = SetCursorFirstEntity( RecursiveLOD, "LOD_EntityChild", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( RecursiveLOD, "LOD_EntityChild" )
      SetViewToSubobject( RecursiveLOD, "LOD_EntityChild" );
      //:BuildAD_GroupPotAttrListR( TZADWWKO, MainLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, lCurrentLevel, lStartLevel, szIndentSpaces )
      o_BuildAD_GroupPotAttrListR( TZADWWKO, MainLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, lCurrentLevel, lStartLevel, szIndentSpaces );
      //:ResetViewFromSubobject( RecursiveLOD )
      ResetViewFromSubobject( RecursiveLOD );
      RESULT = SetCursorNextEntity( RecursiveLOD, "LOD_EntityChild", "" );
   } 

   //:END
   return;
// END
} 


//:DIALOG OPERATION
//:LIST_AllLOD_FlatAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
LIST_AllLOD_FlatAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove current list of ESG_FlatListPotentialAttribute entries and recreate with attributes in LOD.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
   o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_0, "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_AD_AttributeAsRangeFrom( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeFrom( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set the two Range values in FlatSelectedSearchAttribute from those in ESG_FlatListPotentialAttribute and
   //:// indicate the type is "From".

   //:// Make sure that Data Types match between source and target.
   //:IF TZADWWKO.FlatSelectedSearchAttribute.DataType != TZADWWKO.ESG_FlatDomain.DataType 
   if ( CompareAttributeToAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "DataType", TZADWWKO, "ESG_FlatDomain", "DataType" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Data Types of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Data Types of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure that Entity Names match between source and target.
   //:IF TZADWWKO.FlatSelectedSearchAttribute.EntityName != TZADWWKO.ESG_FlatListPotentialAttribute.EntityName 
   if ( CompareAttributeToAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "EntityName", TZADWWKO, "ESG_FlatListPotentialAttribute", "EntityName" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Entity Names of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Entity Names of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Set values in target.
   //:TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName = TZADWWKO.ESG_FlatListPotentialAttribute.AttributeName 
   SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute", "AttributeName" );
   //:TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType          = "From"
   SetAttributeFromString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "From" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_AutodesignObjectFindSubdlg( VIEW ViewToWindow )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignObjectFindSubdlg( zVIEW     ViewToWindow )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_Base  BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW TZZOLFLO BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW TZADWWKO BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:STRING ( 300 ) szAutodesignLPLR_Directory
   zCHAR     szAutodesignLPLR_Directory[ 301 ] = { 0 }; 
   //:STRING ( 300 ) szMsg
   zCHAR     szMsg[ 301 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Make sure an Auto Design Base dialog can be loaded.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// The AD_Base object isn't in memory, so load it.
      //:nRC = ActivateAD_BaseL( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_BaseL( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base"
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
   } 

   //:END

   //:// The TZADWWKO object  should be created new.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( TZADWWKO )
      DropObjectInstance( TZADWWKO );
   } 

   //:END
   //:ACTIVATE TZADWWKO EMPTY 
   RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
   //:NAME VIEW TZADWWKO "TZADWWKO"
   SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
   //:CREATE ENTITY TZADWWKO.AutoDesignWork 
   RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
   //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
   RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
   //:TZADWWKO.AutodesignSubdialog.AD_AreaSuffix = "Object"
   SetAttributeFromString( TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix", "Object" );
   //:CREATE ENTITY TZADWWKO.EntitySubGroup 
   RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
   //:InitializeL_SymbolType( TZADWWKO )
   oTZADWWKO_InitializeL_SymbolType( TZADWWKO );

   //:// Make sure that the TZZOLFLO object of Meta LOD entries exists.
   //:GET VIEW TZZOLFLO NAMED "TZZOLFLO"
   RESULT = GetViewByName( &TZZOLFLO, "TZZOLFLO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZZOLFLO, TaskLPLR )
      CreateViewFromView( &TZZOLFLO, TaskLPLR );
      //:NAME VIEW TZZOLFLO "TZZOLFLO"
      SetNameForView( TZZOLFLO, "TZZOLFLO", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST TZZOLFLO.W_MetaType WHERE TZZOLFLO.W_MetaType.Type = 7   // 7 is read view of LOD 
      RESULT = SetCursorFirstEntityByInteger( TZZOLFLO, "W_MetaType", "Type", 7, "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_FindReturnedListAttribs( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_FindReturnedListAttribs( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADWWKO.FlatSelectedListAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "FlatSelectedListAttribute" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "FlatSelectedListAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:DELETE ENTITY TZADWWKO.FlatSelectedListAttribute NONE
         RESULT = DeleteEntity( TZADWWKO, "FlatSelectedListAttribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:InitializeAD_WorkObjectBootstrap( VIEW ViewToWindow )

//:   VIEW TZWINDOWL REGISTERED AS TZWINDOWL
static zSHORT
o_InitializeAD_WorkObjectBootstra( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZADWWKO  REGISTERED AS TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW TempCtrl  BASED ON LOD  TZWDLGSO
   zVIEW     TempCtrl = 0; 
   //:VIEW TempCtrl2 BASED ON LOD  TZWDLGSO
   zVIEW     TempCtrl2 = 0; 
   //:VIEW TZZOLFLO  BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 20 ) szControlDefTag
   zCHAR     szControlDefTag[ 21 ] = { 0 }; 
   //:STRING ( 20 ) szControlType
   zCHAR     szControlType[ 21 ] = { 0 }; 
   //:STRING ( 50 ) szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szTextPrompt
   zCHAR     szTextPrompt[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szTextCSS_Class
   zCHAR     szTextCSS_Class[ 51 ] = { 0 }; 
   //:STRING ( 4 )  szSubControlType
   zCHAR     szSubControlType[ 5 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   //:INTEGER SubRowCount
   zLONG     SubRowCount = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_3; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set up work data for "Regular Autodesign Group".
   //:// Also build mapping data for any existing Controls.

   //:// The autodesign work objects need to be set up.
   //:// Default state will be resize and reposition top Groups.
   //:TZADWWKO.AutoDesignWork.RepositionGroupFlag = "Y"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "RepositionGroupFlag", "Y" );

   //:// Determine type of function as either:
   //:// 1. New Group
   //:// 2. Modification of Grid
   //:// 3. Modification of update/display controls
   //:// For Bootstrap Autodesign, the function depends partly on the current or parent Group Type as follows:
   //:// 1. A new Group must be drawn on an "accordion" parent Group.
   //:// 2. A modification of update/display controls or a Grid must be done on a current Group of Type "card-body".

   //:// Check for modification of Grid or update/display controls.
   //:CreateViewFromView( TempCtrl, TZCONTROL )
   CreateViewFromView( &TempCtrl, TZCONTROL );
   //:NAME VIEW TempCtrl "TempCtrl"
   SetNameForView( TempCtrl, "TempCtrl", 0, zLEVEL_TASK );
   //:IF TempCtrl.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TempCtrl, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:nRC = FindControlTypeWithinGroup( TempCtrl )
      nRC = (zSHORT) o_FindControlTypeWithinGroup( TempCtrl );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:szSubControlType = "Grp"
         ZeidonStringCopy( szSubControlType, 1, 0, "Grp", 1, 0, 5 );
         //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = ""
         SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "" );
         //:ELSE
      } 
      else
      { 
         //:IF nRC = 2
         if ( nRC == 2 )
         { 
            //:szSubControlType = "Grid"
            ZeidonStringCopy( szSubControlType, 1, 0, "Grid", 1, 0, 5 );
            //:ELSE
         } 
         else
         { 
            //:szSubControlType = "Updt"
            ZeidonStringCopy( szSubControlType, 1, 0, "Updt", 1, 0, 5 );
         } 

         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Group",
      //:             "When Autodesigning a new Group, the current Group cannot have subgroups.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Group", "When Autodesigning a new Group, the current Group cannot have subgroups.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:DropView( TempCtrl )
   DropView( TempCtrl );
   //:CreateViewFromView( TempCtrl, TZCONTROL )
   CreateViewFromView( &TempCtrl, TZCONTROL );
   //:NAME VIEW TempCtrl "TempCtrl"
   SetNameForView( TempCtrl, "TempCtrl", 0, zLEVEL_TASK );

   //:// For Grid and update/display controls, make sure current Group Class is card-body,
   //:// Otherwise, current Group Class must be "accordion".
   //:IF szSubControlType = "Grid" OR szSubControlType = "Updt"
   if ( ZeidonStringCompare( szSubControlType, 1, 0, "Grid", 1, 0, 5 ) == 0 || ZeidonStringCompare( szSubControlType, 1, 0, "Updt", 1, 0, 5 ) == 0 )
   { 
      //:IF TempCtrl.Control.CSS_Class != "card-body"
      if ( CompareAttributeToString( TempCtrl, "Control", "CSS_Class", "card-body" ) != 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Group",
         //:             "When modifying an existing Group, the Group Type selected must be of CSS Class, card-body.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Group", "When modifying an existing Group, the Group Type selected must be of CSS Class, card-body.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:END

   //:// We will continue creating current mapping entries only for Grid and update/display controls.
   //:IF szSubControlType = "Grid"
   if ( ZeidonStringCompare( szSubControlType, 1, 0, "Grid", 1, 0, 5 ) == 0 )
   { 
      //:// Set up existing mapping for Grid.
      //:TZADWWKO.EntitySubGroup.GroupType                     = "ListGroupOnly"
      SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "ListGroupOnly" );
      //:InitGridSelectAttribs( TZADWWKO, TempCtrl )
      oTZADWWKO_InitGridSelectAttribs( TZADWWKO, TempCtrl );
   } 

   //:END

   //:IF szSubControlType = "Updt"
   if ( ZeidonStringCompare( szSubControlType, 1, 0, "Updt", 1, 0, 5 ) == 0 )
   { 

      //:// The layout for Bootstrap assumes the Prompt and target control pairs are set within an outer "row" Group and an
      //:// inner "col-X" or null Group. The "row" Groups will be counted to determine the row value for initial mapping entries.

      //:// Find the first update/display control to get object mapping information.
      //:CreateViewFromView( TempCtrl2, TempCtrl )
      CreateViewFromView( &TempCtrl2, TempCtrl );
      //:NAME VIEW TempCtrl2 "TempCtrl2"
      SetNameForView( TempCtrl2, "TempCtrl2", 0, zLEVEL_TASK );
      //:SetViewToSubobject( TempCtrl2, "CtrlCtrl" )   // Position on Row Group.
      SetViewToSubobject( TempCtrl2, "CtrlCtrl" );
      //:SetViewToSubobject( TempCtrl2, "CtrlCtrl" )   // Position on Pairing Group
      SetViewToSubobject( TempCtrl2, "CtrlCtrl" );
      //:SetViewToSubobject( TempCtrl2, "CtrlCtrl" )   // Position on Prompt Control.
      SetViewToSubobject( TempCtrl2, "CtrlCtrl" );
      //:SET CURSOR NEXT TempCtrl2.Control             // Position on update/display Control with mapping. 
      RESULT = SetCursorNextEntity( TempCtrl2, "Control", "" );
      //:IF TempCtrl2.CtrlMap DOES NOT EXIST
      lTempInteger_1 = CheckExistenceOfEntity( TempCtrl2, "CtrlMap" );
      if ( lTempInteger_1 != 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Group",
         //:             "The update/display mapping entry is missing.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Group", "The update/display mapping entry is missing.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Set up the Mapping Object from the Control Mapping.
      //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
      SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" );
      //:TZADWWKO.EntitySubGroup.GroupType                     = "DetailGroupBootstrap"
      SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroupBootstrap" );
      //:TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName   = "GroupDetail"
      SetAttributeFromString( TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName", "GroupDetail" );
      //:CREATE ENTITY TZADWWKO.AutodesignGroupTemplate 
      RESULT = CreateEntity( TZADWWKO, "AutodesignGroupTemplate", zPOS_AFTER );
      //:TZADWWKO.AutodesignGroupTemplate.GroupName = "GroupDetail"
      SetAttributeFromString( TZADWWKO, "AutodesignGroupTemplate", "GroupName", "GroupDetail" );

      //:// Activate Object View and LOD.
      //:szViewName = TempCtrl2.CtrlMapView.Name 
      GetVariableFromAttribute( szViewName, 0, 'S', 51, TempCtrl2, "CtrlMapView", "Name", "", 0 );
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 );
      //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
      SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
      //:IF TZADWWKO.W_MetaDefUpdateView EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
      if ( lTempInteger_2 == 0 )
      { 
         //:EXCLUDE TZADWWKO.W_MetaDefUpdateView  
         RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateView", zREPOS_AFTER );
      } 

      //:END 
      //:SET CURSOR FIRST TaskLPLR.W_MetaType WHERE TaskLPLR.W_MetaType.Type = 9   // Read view of Registered View
      RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 9, "" );
      //:SET CURSOR FIRST TaskLPLR.W_MetaDef  WHERE TaskLPLR.W_MetaDef.Name  = UpdateVOR.ViewObjRef.Name  
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateVOR, "ViewObjRef", "Name" );
      RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
      //:INCLUDE TZADWWKO.W_MetaDefUpdateView  FROM TaskLPLR.W_MetaDef 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TaskLPLR, "W_MetaDef", zPOS_AFTER );
      //:szLOD_Name = UpdateVOR.LOD.Name 
      GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
      //:NAME VIEW UpdateLOD "TZZOLODO_Update"
      SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
      RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
      //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = UpdateLOD.LOD.Name   
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
      RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
      //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

      //:// Loop through each row Group and then each detail pair Group within the row.
      //:RowCount = 0
      RowCount = 0;
      //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )
      SetViewToSubobject( TempCtrl, "CtrlCtrl" );
      //:FOR EACH TempCtrl.Control      // Loop through each Row.
      RESULT = SetCursorFirstEntity( TempCtrl, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TempCtrl.Control.CSS_Class != "row"
         if ( CompareAttributeToString( TempCtrl, "Control", "CSS_Class", "row" ) != 0 )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Group",
            //:             "Invalid subcontrol encountered. Row Class expected..",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Group", "Invalid subcontrol encountered. Row Class expected..", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:DropView( TempCtrl )
            DropView( TempCtrl );
            //:RETURN -2
            return( -2 );
         } 

         //:END
         //:RowCount = RowCount + 1
         RowCount = RowCount + 1;

         //:// Step down to detail pair Groups that contains each Prompt plus Control with mapping.
         //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )
         SetViewToSubobject( TempCtrl, "CtrlCtrl" );
         //:SubRowCount = 0
         SubRowCount = 0;
         //:FOR EACH TempCtrl.Control
         RESULT = SetCursorFirstEntity( TempCtrl, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:SubRowCount = SubRowCount + 1
            SubRowCount = SubRowCount + 1;
            //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )
            SetViewToSubobject( TempCtrl, "CtrlCtrl" );
            //:// Prompt
            //:szTextPrompt = TempCtrl.Control.Text
            GetVariableFromAttribute( szTextPrompt, 0, 'S', 51, TempCtrl, "Control", "Text", "", 0 );
            //:SET CURSOR NEXT TempCtrl.Control    // Step to mapping control
            RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
            //:// Mapping Control
            //:IF TempCtrl.CtrlMapLOD_Attribute DOES NOT EXIST
            lTempInteger_3 = CheckExistenceOfEntity( TempCtrl, "CtrlMapLOD_Attribute" );
            if ( lTempInteger_3 != 0 )
            { 
               //:IssueError( ViewToWindow,0,0, "The Group layout is not valid. You must Cancel the Autodesign process." ) 
               IssueError( ViewToWindow, 0, 0, "The Group layout is not valid. You must Cancel the Autodesign process." );
               //:RETURN
               return( 0 );
            } 

            //:END
            //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TempCtrl.CtrlMapLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TempCtrl, "CtrlMapLOD_Attribute", zPOS_AFTER );
            //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue = szTextPrompt
            SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", szTextPrompt );
            //:TZADWWKO.ESG_ListLOD_Attribute.GroupRow    = RowCount
            SetAttributeFromInteger( TZADWWKO, "ESG_ListLOD_Attribute", "GroupRow", RowCount );
            //:// Convert Control Type to Calendar if CSS_Class = "form-control datefield".
            //:IF TempCtrl.Control.CSS_Class = "form-control datefield"
            if ( CompareAttributeToString( TempCtrl, "Control", "CSS_Class", "form-control datefield" ) == 0 )
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Calendar"
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Calendar" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = TempCtrl.ControlDef.Tag
               SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TempCtrl, "ControlDef", "Tag" );
            } 

            //:END

            //:ResetViewFromSubobject( TempCtrl )
            ResetViewFromSubobject( TempCtrl );
            RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
         } 

         //:END
         //:ResetViewFromSubobject( TempCtrl )
         ResetViewFromSubobject( TempCtrl );
         RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
      } 

      //:END
      //:ResetViewFromSubobject( TempCtrl )
      ResetViewFromSubobject( TempCtrl );

      //:// Delete any current potential attributes and then go to build them.
      //:BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
      o_BuildAD_GroupPotAttrList( TZADWWKO, UpdateLOD, szTempString_0, "" );
   } 

   //:END

   //:DropView( TempCtrl )
   DropView( TempCtrl );
   return( 0 );
//    
// END
} 


//:LOCAL OPERATION
//:FindControlTypeWithinGroup( VIEW TempCtrl BASED ON LOD TZWDLGSO)

//:   INTEGER nRC
static zLONG
o_FindControlTypeWithinGroup( zVIEW     TempCtrl )
{
   zLONG     nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:// Search each subcontrol looking for a Control Type other than Group.
   //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )
   SetViewToSubobject( TempCtrl, "CtrlCtrl" );
   //:FOR EACH TempCtrl.Control 
   RESULT = SetCursorFirstEntity( TempCtrl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TempCtrl.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TempCtrl, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:IF TempCtrl.CtrlCtrl EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TempCtrl, "CtrlCtrl" );
         if ( lTempInteger_0 == 0 )
         { 
            //:nRC = FindControlTypeWithinGroup( TempCtrl )
            nRC = o_FindControlTypeWithinGroup( TempCtrl );
            //:IF nRC != 1    // A return code of 1 means no subcontrol was found except for GroupBox.
            if ( nRC != 1 )
            { 
               //:RETURN nRC  // Since a Control other than GroupBox was found, return.
               return( nRC );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF TempCtrl.ControlDef.Tag = "Grid"
         if ( CompareAttributeToString( TempCtrl, "ControlDef", "Tag", "Grid" ) == 0 )
         { 
            //:RETURN 2    // Returning a 2 is for a Grid.
            return( 2 );
            //:ELSE
         } 
         else
         { 
            //:RETURN 3    // Returning a 3 is for an update/display control.
            return( 3 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
      //:END 
   } 

   //:END
   //:ResetViewFromSubobject( TempCtrl )
   ResetViewFromSubobject( TempCtrl );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ActivateAD_BaseL( VIEW RetAD_BaseView,
//:                  VIEW ViewToWindow )

//:   VIEW TaskLPLR  REGISTERED AS TaskLPLR
static zSHORT
o_ActivateAD_BaseL( zPVIEW    RetAD_BaseView,
                    zVIEW     ViewToWindow )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 300 ) szAutodesignLPLR_Directory
   zCHAR     szAutodesignLPLR_Directory[ 301 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Activate correct AD_Base dialog based on Generation type.
   //:// For now, only activate the Bootstrap AD_Base.
   //:IF TaskLPLR.LPLR.AutodesignBase = ""
   if ( CompareAttributeToString( TaskLPLR, "LPLR", "AutodesignBase", "" ) == 0 )
   { 
      //:szAutodesignLPLR_Directory = "C:\LPLR\AD_Base"                // Use default
      ZeidonStringCopy( szAutodesignLPLR_Directory, 1, 0, "C:\\LPLR\\AD_Base", 1, 0, 301 );
      //:ELSE
   } 
   else
   { 
      //:szAutodesignLPLR_Directory = TaskLPLR.LPLR.AutodesignBase     // Use specified directory
      GetVariableFromAttribute( szAutodesignLPLR_Directory, 0, 'S', 301, TaskLPLR, "LPLR", "AutodesignBase", "", 0 );
   } 

   //:END
   //:szAutodesignLPLR_Directory = szAutodesignLPLR_Directory + "\AD_Base.PWD"
   ZeidonStringConcat( szAutodesignLPLR_Directory, 1, 0, "\\AD_Base.PWD", 1, 0, 301 );
   //:nRC = ActivateOI_FromFile( RetAD_BaseView, "TZWDLGSO", ViewToWindow, szAutodesignLPLR_Directory, zSINGLE )
   nRC = ActivateOI_FromFile( RetAD_BaseView, "TZWDLGSO", ViewToWindow, szAutodesignLPLR_Directory, zSINGLE );

   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "Unable to activate AD_Base.PWD from AD_Base LPLR.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "Unable to activate AD_Base.PWD from AD_Base LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW RetAD_BaseView "AD_Base"
   SetNameForView( *RetAD_BaseView, "AD_Base", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_FindReturnedCL_Attribs( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_FindReturnedCL_Attribs( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW  REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZADWWKO2 BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO2 = 0; 
   //:STRING ( 1 )   szCreateEntityOnlyEntry
   zCHAR     szCreateEntityOnlyEntry[ 2 ] = { 0 }; 
   //:STRING ( 100 ) szTempString
   zCHAR     szTempString[ 101 ] = { 0 }; 
   //:INTEGER lMaxPromptLength
   zLONG     lMaxPromptLength = 0; 
   //:INTEGER lPromptLength
   zLONG     lPromptLength = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", ViewToWindow, zLEVEL_TASK );

   //:// Make sure we have position on AutoDesignWindow.
   //:SET CURSOR FIRST TZWINDOW.AutoDesignWindow  
   RESULT = SetCursorFirstEntity( TZWINDOW, "AutoDesignWindow", "" );

   //:// Copy selected attributes from ESG_FlatListPotentialAttribute2 entries to ESG_ListLOD_Attribute entries at the current postiion..
   //:CreateViewFromView( TZADWWKO2, TZADWWKO )
   CreateViewFromView( &TZADWWKO2, TZADWWKO );
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute2
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" );
      //:IF nRC = 1 
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO2.FlatSelectedListAttribute 
         //:           WHERE TZADWWKO2.FlatSelectedListAttribute.EntityName    = TZADWWKO.ESG_FlatListPotentialAttribute2.EntityName  
         //:             AND TZADWWKO2.FlatSelectedListAttribute.AttributeName = TZADWWKO.ESG_FlatListPotentialAttribute2.AttributeName 
         RESULT = SetCursorFirstEntity( TZADWWKO2, "FlatSelectedListAttribute", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedListAttribute", "EntityName", TZADWWKO, "ESG_FlatListPotentialAttribute2", "EntityName" ) != 0 ||
                    CompareAttributeToAttribute( TZADWWKO2, "FlatSelectedListAttribute", "AttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute2", "AttributeName" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZADWWKO2, "FlatSelectedListAttribute", "" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY TZADWWKO.FlatSelectedListAttribute  
            RESULT = CreateEntity( TZADWWKO, "FlatSelectedListAttribute", zPOS_AFTER );
            //:INCLUDE TZADWWKO.FlatListLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute2 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatListLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute2", zPOS_AFTER );
            //:TZADWWKO.FlatSelectedListAttribute.EntityName    = TZADWWKO.ESG_FlatLOD_Entity2.Name 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "EntityName", TZADWWKO, "ESG_FlatLOD_Entity2", "Name" );
            //:TZADWWKO.FlatSelectedListAttribute.AttributeName = TZADWWKO.ESG_FlatER_Attribute2.Name 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "AttributeName", TZADWWKO, "ESG_FlatER_Attribute2", "Name" );
            //:// We will use the list title value if it exists. Otherwise, we will use the Prompt value.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleValue != ""
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleValue", "" ) != 0 )
            { 
               //:TZADWWKO.FlatSelectedListAttribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleValue 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleValue" );
               //:TZADWWKO.FlatSelectedListAttribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute2.ListTitleLength 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ListTitleLength" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.FlatSelectedListAttribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptValue 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptValue" );
               //:TZADWWKO.FlatSelectedListAttribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptLength 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptLength" );
            } 

            //:END
            //:TZADWWKO.FlatSelectedListAttribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlType" );
            //:TZADWWKO.FlatSelectedListAttribute.ControlWidth = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlWidth 
            SetAttributeFromAttribute( TZADWWKO, "FlatSelectedListAttribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlWidth" );
         } 

         //:END
         //:SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", 0 )
         SetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
      //:END
   } 

   //:END
   //:DropView( TZADWWKO2 )
   DropView( TZADWWKO2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildGroupListSpec( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
PostbuildGroupListSpec( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// If this is for immediately autodesign the List Group, change the text of the "OK" button to "Generate"..
   //:IF TZADWWKO.EntitySubGroup.ListGroupGenerateFlag = "Y"
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "ListGroupGenerateFlag", "Y" ) == 0 )
   { 
      //:SetCtrlText( ViewToWindow, "PBOK", "Generate" )
      SetCtrlText( ViewToWindow, "PBOK", "Generate" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:FindGridWithinGroup( VIEW TempCtrl BASED ON LOD TZWDLGSO)

//:   INTEGER nRC
static zLONG
o_FindGridWithinGroup( zVIEW     TempCtrl )
{
   zLONG     nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:// Search each subcontrol looking for a Grid.
   //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )
   SetViewToSubobject( TempCtrl, "CtrlCtrl" );
   //:FOR EACH TempCtrl.Control 
   RESULT = SetCursorFirstEntity( TempCtrl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TempCtrl.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TempCtrl, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:IF TempCtrl.CtrlCtrl EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TempCtrl, "CtrlCtrl" );
         if ( lTempInteger_0 == 0 )
         { 
            //:nRC = FindGridWithinGroup( TempCtrl )
            nRC = o_FindGridWithinGroup( TempCtrl );
            //:IF nRC = 2  // A return code of 2 means we've found the Grid
            if ( nRC == 2 )
            { 
               //:RETURN nRC  
               return( nRC );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF TempCtrl.ControlDef.Tag = "Grid"
         if ( CompareAttributeToString( TempCtrl, "ControlDef", "Tag", "Grid" ) == 0 )
         { 
            //:RETURN 2    // Returning a 2 is for a Grid.
            return( 2 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
      //:END 
   } 

   //:END
   //:ResetViewFromSubobject( TempCtrl )
   ResetViewFromSubobject( TempCtrl );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_AD_GroupSpec( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CANCEL_AD_GroupSpec( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:CancelSubobject( TZADWWKO, "EntitySubGroup" )
   CancelSubobject( TZADWWKO, "EntitySubGroup" );
   //:GET VIEW TZCONTROL NAMED "TZCONTROL"
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:CancelSubobject( TZCONTROL, "Control" )
      CancelSubobject( TZCONTROL, "Control" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_ExtensionOptions( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
GOTO_ExtensionOptions( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW UserLOD  BASED ON LOD  TZZOLODO
   zVIEW     UserLOD = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that the Email and Merge work attributes exist.

   //:// Check Email work attributes in mUser object.
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UserLOD, 0, zREFER_LOD_META, zSINGLE, "mUser", 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UserLOD, 0, zREFER_LOD_META, zSINGLE, "mUser", 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog", 
      //:             "Object mUser does not exist.", 
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Object mUser does not exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW UserLOD "UserLOD"
   SetNameForView( UserLOD, "UserLOD", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST UserLOD.ER_AttributeRec WITHIN UserLOD.LOD_EntityParent 
   //:           WHERE UserLOD.ER_AttributeRec.Name = "SenderUserEmailAddress"
   RESULT = SetCursorFirstEntityByString( UserLOD, "ER_AttributeRec", "Name", "SenderUserEmailAddress", "LOD_EntityParent" );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:SET CURSOR FIRST UserLOD.ER_AttributeRec WITHIN UserLOD.LOD_EntityParent 
      //:           WHERE UserLOD.ER_AttributeRec.Name = "SenderHostEmailAddress"
      RESULT = SetCursorFirstEntityByString( UserLOD, "ER_AttributeRec", "Name", "SenderHostEmailAddress", "LOD_EntityParent" );
   } 

   //:END
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog", 
      //:             "Both  attributes 'SenderUserEmailAddress' and 'SenderHostEmailAddress' must be defined in mUser.User object/entity.", 
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both  attributes 'SenderUserEmailAddress' and 'SenderHostEmailAddress' must be defined in mUser.User object/entity.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END 

   //:// Add Email Types to work object as necessary.
   //:IF TZADWWKO.SendEmailType DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "SendEmailType" );
   if ( lTempInteger_0 != 0 )
   { 
      //:CREATE ENTITY TZADWWKO.EmailWork  
      RESULT = CreateEntity( TZADWWKO, "EmailWork", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.SendEmailType    // Empty entry.
      RESULT = CreateEntity( TZADWWKO, "SendEmailType", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.SendEmailType
      RESULT = CreateEntity( TZADWWKO, "SendEmailType", zPOS_AFTER );
      //:TZADWWKO.SendEmailType.DisplayName = "Simple Email"
      SetAttributeFromString( TZADWWKO, "SendEmailType", "DisplayName", "Simple Email" );
      //:TZADWWKO.SendEmailType.EmailType   = "SimpleEmail"
      SetAttributeFromString( TZADWWKO, "SendEmailType", "EmailType", "SimpleEmail" );
      //:CREATE ENTITY TZADWWKO.SendEmailType
      RESULT = CreateEntity( TZADWWKO, "SendEmailType", zPOS_AFTER );
      //:TZADWWKO.SendEmailType.DisplayName = "Email Merge"
      SetAttributeFromString( TZADWWKO, "SendEmailType", "DisplayName", "Email Merge" );
      //:TZADWWKO.SendEmailType.EmailType   = "EmailMerge"
      SetAttributeFromString( TZADWWKO, "SendEmailType", "EmailType", "EmailMerge" );
      //:CREATE ENTITY TZADWWKO.SendEmailType
      RESULT = CreateEntity( TZADWWKO, "SendEmailType", zPOS_AFTER );
      //:TZADWWKO.SendEmailType.DisplayName = "Email with Attachment"
      SetAttributeFromString( TZADWWKO, "SendEmailType", "DisplayName", "Email with Attachment" );
      //:TZADWWKO.SendEmailType.EmailType   = "EmailwAttachment"
      SetAttributeFromString( TZADWWKO, "SendEmailType", "EmailType", "EmailwAttachment" );
   } 

   //:END

   //:// Make the AutoDesignWork entity temporal for the subwindow.
   //:CreateTemporalSubobjectVersion( TZADWWKO, "EmailWork" )
   CreateTemporalSubobjectVersion( TZADWWKO, "EmailWork" );

   //:// Position on the correct entry.
   //:SET CURSOR FIRST TZADWWKO.SendEmailType WHERE TZADWWKO.SendEmailType.EmailType = TZADWWKO.AutoDesignWork.SendEmailType 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "AutoDesignWork", "SendEmailType" );
   RESULT = SetCursorFirstEntityByString( TZADWWKO, "SendEmailType", "EmailType", szTempString_0, "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_RecipientEmailAddress( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_RecipientEmailAddress( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 200 ) szEmailAddressViewEntityAttrib
   zCHAR     szEmailAddressViewEntityAttrib[ 201 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set the view.entity.attribute combination for the selected entry.
   //:szEmailAddressViewEntityAttrib = "mConList." + 
   //:                                  TZADWWKO.ESG_FlatListPotentialAttribute2.EntityName + "." +
   //:                                  TZADWWKO.ESG_FlatListPotentialAttribute2.AttributeName 
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZADWWKO, "ESG_FlatListPotentialAttribute2", "EntityName", "", 0 );
   ZeidonStringCopy( szEmailAddressViewEntityAttrib, 1, 0, "mConList.", 1, 0, 201 );
   ZeidonStringConcat( szEmailAddressViewEntityAttrib, 1, 0, szTempString_0, 1, 0, 201 );
   ZeidonStringConcat( szEmailAddressViewEntityAttrib, 1, 0, ".", 1, 0, 201 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZADWWKO, "ESG_FlatListPotentialAttribute2", "AttributeName", "", 0 );
   ZeidonStringConcat( szEmailAddressViewEntityAttrib, 1, 0, szTempString_1, 1, 0, 201 );
   //:TZADWWKO.AutoDesignWork.RecipientEmailAddressMapping = szEmailAddressViewEntityAttrib
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "RecipientEmailAddressMapping", szEmailAddressViewEntityAttrib );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_Extensions( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_Extensions( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// If the Email Type has been selected, make sure that the Recipient Email Address is specified.
   //:IF TZADWWKO.SendEmailType.DisplayName != ""
   if ( CompareAttributeToString( TZADWWKO, "SendEmailType", "DisplayName", "" ) != 0 )
   { 
      //:// An email type has been selected.
      //:IF TZADWWKO.AutoDesignWork.RecipientEmailAddressMapping = ""
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "RecipientEmailAddressMapping", "" ) == 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog", 
         //:             "If an Email Type has been selected, a Recipient Email Address value must be specified.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "If an Email Type has been selected, a Recipient Email Address value must be specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:END
   //:TZADWWKO.AutoDesignWork.SendEmailType = TZADWWKO.SendEmailType.EmailType 
   SetAttributeFromAttribute( TZADWWKO, "AutoDesignWork", "SendEmailType", TZADWWKO, "SendEmailType", "EmailType" );
   //:AcceptSubobject( TZADWWKO, "EmailWork" )
   AcceptSubobject( TZADWWKO, "EmailWork" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_GroupDetailWInclude( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_GroupDetailWInclude( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that both the Search and Include MetaDef entries have been selected.
   //:IF TZADWWKO.ESG_ListIncludeW_MetaDef DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
   if ( lTempInteger_0 != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "An Include object must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "An Include object must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure both Group Area Name and Area Title have been selected because they are required for Bootstrap.
   //:IF TZADWWKO.EntitySubGroup.GroupAreaName  = "" OR 
   //:   TZADWWKO.EntitySubGroup.GroupAreaTitle = ""
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaName", "" ) == 0 || CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "" ) == 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both a Group Area Name and Title must be specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a Group Area Name and Title must be specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:AcceptSubobject( TZADWWKO, "EntitySubGroup" )
   AcceptSubobject( TZADWWKO, "EntitySubGroup" );

   //:// Save the Group Type for checks on update of a group.
   //:TZADWWKO.EntitySubGroup.SavedGroupType = TZADWWKO.EntitySubGroup.GroupType 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "SavedGroupType", TZADWWKO, "EntitySubGroup", "GroupType" );

   //:// The Action/Operation Suffix will be the same as the Group Area Name, at least for now.
   //:TZADWWKO.EntitySubGroup.IncludeOperationSuffix = TZADWWKO.EntitySubGroup.GroupAreaName 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix", TZADWWKO, "EntitySubGroup", "GroupAreaName" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACTIVATE_AutodesignDefinition( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACTIVATE_AutodesignDefinition( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKON   BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKON = 0; 
   //:VIEW TaskLPLR    REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW ReturnLOD   BASED ON LOD  TZZOLODO
   zVIEW     ReturnLOD = 0; 
   //:VIEW mConListLOD BASED ON LOD  TZZOLODO
   zVIEW     mConListLOD = 0; 
   //:STRING ( 200 ) szFullFileName
   zCHAR     szFullFileName[ 201 ] = { 0 }; 
   //:STRING ( 50 )  szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   zCHAR     szTempString_0[ 255 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Activate the TZADWWKO OI in the current LPLR under the name specified.
   //:// It will replace any current TZADWWKO OI.
   //:szFullFileName = TaskLPLR.LPLR.MetaSrcDir + "\" + TZADWWKO.AutoDesignWork.StoredDesignSavedName + ".ADD"
   GetStringFromAttribute( szFullFileName, zsizeof( szFullFileName ), TaskLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szFullFileName, 1, 0, "\\", 1, 0, 201 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 255, TZADWWKO, "AutoDesignWork", "StoredDesignSavedName", "", 0 );
   ZeidonStringConcat( szFullFileName, 1, 0, szTempString_0, 1, 0, 201 );
   ZeidonStringConcat( szFullFileName, 1, 0, ".ADD", 1, 0, 201 );
   //:TraceLineS( "*** AD Def File Name: ", szFullFileName )
   TraceLineS( "*** AD Def File Name: ", szFullFileName );

   //:// Check to see that the file exists.
   //:nFileIn = SysOpenFile( ViewToWindow, szFullFileName, COREFILE_READ )
   nFileIn = SysOpenFile( ViewToWindow, szFullFileName, COREFILE_READ );
   //:IF nFileIn >= 0 
   if ( nFileIn >= 0 )
   { 
      //:SysCloseFile( ViewToWindow, nFileIn, 0 )
      SysCloseFile( ViewToWindow, nFileIn, 0 );
      //:ELSE
   } 
   else
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Def Save", 
      //:             "The file specified does not exist in the LPLR.", 
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Def Save", "The file specified does not exist in the LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:nRC = ActivateOI_FromFile( TZADWWKON, "TZADWWKO", ViewToWindow, szFullFileName, zSINGLE )
   nRC = ActivateOI_FromFile( &TZADWWKON, "TZADWWKO", ViewToWindow, szFullFileName, zSINGLE );
   //:TraceLineI( "Activate nRC: ", nRC )
   TraceLineI( "Activate nRC: ", nRC );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Def Save", 
      //:             "An error occurred during Activate and no action has been taken.", 
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Def Save", "An error occurred during Activate and no action has been taken.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
      //:ELSE
   } 
   else
   { 
      //:DropObjectInstance( TZADWWKO )
      DropObjectInstance( TZADWWKO );
      //:NAME VIEW TZADWWKON "TZADWWKO"
      SetNameForView( TZADWWKON, "TZADWWKO", 0, zLEVEL_TASK );
   } 

   //:END

   //:// If an Update LOD has been selected in the definition, Activate it and name it TZZOLODO_Update. 
   //:// Drop any current entry.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( UpdateLOD )
      DropObjectInstance( UpdateLOD );
   } 

   //:END
   //:IF TZADWWKON.W_MetaDefUpdateObject EXISTS AND TZADWWKON.W_MetaDefUpdateObject.Name != ""
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKON, "W_MetaDefUpdateObject" );
   if ( lTempInteger_0 == 0 && CompareAttributeToString( TZADWWKON, "W_MetaDefUpdateObject", "Name", "" ) != 0 )
   { 
      //:szLOD_Name = TZADWWKON.W_MetaDefUpdateObject.Name 
      GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKON, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
      //:NAME VIEW UpdateLOD "TZZOLODO_Update"
      SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   } 

   //:END

   //:TraceLineS( "*** Before Search LOD", "" )
   TraceLineS( "*** Before Search LOD", "" );
   //:// If a Search LOD has been selected in the definition, Activate it and name it TZZOLODO1. 
   //:// Drop any current entry.
   //:GET VIEW SearchLOD NAMED "TZZOLOD1"
   RESULT = GetViewByName( &SearchLOD, "TZZOLOD1", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( SearchLOD )
      DropObjectInstance( SearchLOD );
   } 

   //:END
   //:IF TZADWWKON.W_MetaDefSearchObject EXISTS AND TZADWWKON.W_MetaDefSearchObject.Name != ""
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKON, "W_MetaDefSearchObject" );
   if ( lTempInteger_1 == 0 && CompareAttributeToString( TZADWWKON, "W_MetaDefSearchObject", "Name", "" ) != 0 )
   { 
      //:szLOD_Name = TZADWWKON.W_MetaDefSearchObject.Name 
      GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKON, "W_MetaDefSearchObject", "Name", "", 0 );
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
      //:NAME VIEW SearchLOD "TZZOLOD1"
      SetNameForView( SearchLOD, "TZZOLOD1", 0, zLEVEL_TASK );
      //:NAME VIEW SearchLOD "TZZOLODO_Desc" 
      SetNameForView( SearchLOD, "TZZOLODO_Desc", 0, zLEVEL_TASK );
   } 

   //:END
   //:TraceLineS( "*** After Search LOD", "" ) 
   TraceLineS( "*** After Search LOD", "" );

   //:// If this is a CRM Find object, Activate mConList. 
   //:// Drop any current entry.
   //:IF TZADWWKON.AutodesignSubdialog EXISTS AND TZADWWKON.AutodesignSubdialog.FindType = "CRM"
   lTempInteger_2 = CheckExistenceOfEntity( TZADWWKON, "AutodesignSubdialog" );
   if ( lTempInteger_2 == 0 && CompareAttributeToString( TZADWWKON, "AutodesignSubdialog", "FindType", "CRM" ) == 0 )
   { 
      //:GET VIEW mConListLOD NAMED "mConListLOD"
      RESULT = GetViewByName( &mConListLOD, "mConListLOD", ViewToWindow, zLEVEL_TASK );
      //:IF RESULT >= 0
      if ( RESULT >= 0 )
      { 
         //:DropObjectInstance( mConListLOD )
         DropObjectInstance( mConListLOD );
      } 

      //:END
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, mConListLOD, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &mConListLOD, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 );
      //:NAME VIEW mConListLOD "mConListLOD"
      SetNameForView( mConListLOD, "mConListLOD", 0, zLEVEL_TASK );

      //:// Clear any existing TZADWWKO.ESG_FlatListPotentialAttribute2 entries and rebuild them from selected object.
      //:BuildAD_GroupPotAttrList( TZADWWKON, mConListLOD, "ContactList", "2" )
      o_BuildAD_GroupPotAttrList( TZADWWKON, mConListLOD, "ContactList", "2" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ContactListType( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ContactListType( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Build the SelectedContactListTypeDomain entity based on the position of ContactListTypeDomain entity.
   //:IF TZADWWKO.SelectedContactListTypeDomain DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "SelectedContactListTypeDomain" );
   if ( lTempInteger_0 != 0 )
   { 
      //:CREATE ENTITY TZADWWKO.SelectedContactListTypeDomain  
      RESULT = CreateEntity( TZADWWKO, "SelectedContactListTypeDomain", zPOS_AFTER );
   } 

   //:END
   //:TZADWWKO.SelectedContactListTypeDomain.DisplayedValue = TZADWWKO.ContactListTypeDomain.DisplayedValue 
   SetAttributeFromAttribute( TZADWWKO, "SelectedContactListTypeDomain", "DisplayedValue", TZADWWKO, "ContactListTypeDomain", "DisplayedValue" );
   //:TZADWWKO.SelectedContactListTypeDomain.InternalType   = TZADWWKO.ContactListTypeDomain.InternalType 
   SetAttributeFromAttribute( TZADWWKO, "SelectedContactListTypeDomain", "InternalType", TZADWWKO, "ContactListTypeDomain", "InternalType" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:VALIDATE_DetailAttributeDef( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
VALIDATE_DetailAttributeDef( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Validate the ESG_DetailLOD_Attribute values, particularly the GroupRow and GroupNumber values.
   //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute WHERE TZADWWKO.ESG_DetailLOD_Attribute.GroupNumber != ""
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "ESG_DetailLOD_Attribute", "GroupNumber", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      } 

   } 

   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Type is Group.
      //:// Make sure all GroupNumbers are set and <= 4 and all GroupRows are null.
      //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute WHERE TZADWWKO.ESG_DetailLOD_Attribute.GroupRow != ""
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "ESG_DetailLOD_Attribute", "GroupRow", "" ) == 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Detail Def Return", 
         //:             "Both Row values and Group values cannot be set.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Detail Def Return", "Both Row values and Group values cannot be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute WHERE TZADWWKO.ESG_DetailLOD_Attribute.GroupNumber = ""
      RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_DetailLOD_Attribute", "GroupNumber", "", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Detail Def Return", 
         //:             "If one Group value is set, all need to be set.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Detail Def Return", "If one Group value is set, all need to be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute WHERE TZADWWKO.ESG_DetailLOD_Attribute.GroupNumber > 4
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToInteger( TZADWWKO, "ESG_DetailLOD_Attribute", "GroupNumber", 4 ) <= 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Detail Def Return", 
         //:             "A Group value cannot be greater than 4.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Detail Def Return", "A Group value cannot be greater than 4.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:ELSE 
   } 
   else
   { 
      //:// Type is Row.
      //:// Make sure all GroupRows are set or null.
      //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute WHERE TZADWWKO.ESG_DetailLOD_Attribute.GroupRow != ""
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "ESG_DetailLOD_Attribute", "GroupRow", "" ) == 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// One Row value has been set, so make sure they are all set.
         //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute WHERE TZADWWKO.ESG_DetailLOD_Attribute.GroupRow = ""
         RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_DetailLOD_Attribute", "GroupRow", "", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Detail Def Return", 
            //:             "If one Row value is set, they must all be set.", 
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Detail Def Return", "If one Row value is set, they must all be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
      } 

      //:END
   } 

   //:   
   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:VALIDATE_SearchAttributeDef( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
VALIDATE_SearchAttributeDef( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Validate the FlatSelectedSearchAttribute values, particularly the GroupRow and GroupNumber values.
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute  
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.GroupNumber != ""
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "GroupNumber", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      } 

   } 

   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Type is Group.
      //:// Make sure all GroupNumbers are set and <= 4 and all GroupRows are null.
      //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.GroupRow != ""
      RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "GroupRow", "" ) == 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Search Def Return", 
         //:             "Both Row values and Group values cannot be set.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Search Def Return", "Both Row values and Group values cannot be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.GroupNumber = ""
      RESULT = SetCursorFirstEntityByString( TZADWWKO, "FlatSelectedSearchAttribute", "GroupNumber", "", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Search Def Return", 
         //:             "If one Group value is set, all need to be set.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Search Def Return", "If one Group value is set, all need to be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.GroupNumber > 4
      RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToInteger( TZADWWKO, "FlatSelectedSearchAttribute", "GroupNumber", 4 ) <= 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Search Def Return", 
         //:             "A Group value cannot be greater than 4.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Search Def Return", "A Group value cannot be greater than 4.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:ELSE 
   } 
   else
   { 
      //:// Type is Row.
      //:// Make sure all GroupRows are set or null.
      //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.GroupRow != ""
      RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "GroupRow", "" ) == 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// One Row value has been set, so make sure they are all set.
         //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.GroupRow = ""
         RESULT = SetCursorFirstEntityByString( TZADWWKO, "FlatSelectedSearchAttribute", "GroupRow", "", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Search Def Return", 
            //:             "If one Row value is set, they must all be set.", 
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Search Def Return", "If one Row value is set, they must all be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
      } 

      //:END
   } 

   //:   
   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_AutodesignMultiPage( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CANCEL_AutodesignMultiPage( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Drop the work object so that it will be create anew.
   //:DropObjectInstance( TZADWWKO )
   DropObjectInstance( TZADWWKO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAutodesignSubgroup( VIEW ViewToWindow )

//:   VIEW TZWINDOWL  REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignSubgroup( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL  REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZCtlHier  BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:VIEW TZCtlCopy  BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlCopy = 0; 
   //:VIEW TaskLPLR   REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZZOLFLO   BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW TZADWWKO   BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW CurrentLOD BASED ON LOD  TZZOLODO
   zVIEW     CurrentLOD = 0; 
   //:VIEW CurrentVOR BASED ON LOD  TZWDVORO
   zVIEW     CurrentVOR = 0; 
   //:STRING ( 50 ) szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szControlTag
   zCHAR     szControlTag[ 51 ] = { 0 }; 
   //:STRING ( 1 )  szGroupType
   zCHAR     szGroupType[ 2 ] = { 0 }; 
   //:STRING ( 50 ) szPromptValue
   zCHAR     szPromptValue[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szWebCtrlLabelLink
   zCHAR     szWebCtrlLabelLink[ 51 ] = { 0 }; 
   //:STRING ( 1 )  szMappingCompleteFlag
   zCHAR     szMappingCompleteFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )  szContainsGridFlag
   zCHAR     szContainsGridFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )  szDetailFound
   zCHAR     szDetailFound[ 2 ] = { 0 }; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:SHORT   lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT   lInitialLevel
   zSHORT    lInitialLevel = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Set up work data necessary for autodesigning an existing Group.

   //:// Force recreation of the work object.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( TZADWWKO )
      DropObjectInstance( TZADWWKO );
   } 

   //:END

   //:// Delete any VOR or LOD from a previous Autodesign Group.
   //:GET VIEW CurrentVOR NAMED "UpdateObjectViewRef"
   RESULT = GetViewByName( &CurrentVOR, "UpdateObjectViewRef", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( CurrentVOR )
      DropObjectInstance( CurrentVOR );
   } 

   //:END
   //:GET VIEW CurrentLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &CurrentLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( CurrentLOD )
      DropObjectInstance( CurrentLOD );
   } 

   //:END

   //:// Initialize TZADWWKO

   //:// The TZADWWKO itself
   //:ACTIVATE TZADWWKO EMPTY 
   RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
   //:NAME VIEW TZADWWKO "TZADWWKO"
   SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
   //:CREATE ENTITY TZADWWKO.AutoDesignWork 
   RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
   //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
   RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
   //:CREATE ENTITY TZADWWKO.EntitySubGroup  
   RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );

   //:// Default state will be resize and reposition top Groups.
   //:TZADWWKO.AutoDesignWork.RepositionGroupFlag = "Y"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "RepositionGroupFlag", "Y" );

   //:// Remove any existing Autodesign entries from TZWINDOWL.
   //:IF TZWINDOWL.AutoDesignWindow EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AutoDesignWindow" );
   if ( lTempInteger_0 == 0 )
   { 
      //:DELETE ENTITY TZWINDOWL.AutoDesignWindow  
      RESULT = DeleteEntity( TZWINDOWL, "AutoDesignWindow", zPOS_NEXT );
   } 

   //:END
   //:CREATE ENTITY TZWINDOWL.AutoDesignWindow 
   RESULT = CreateEntity( TZWINDOWL, "AutoDesignWindow", zPOS_AFTER );
   //:CREATE ENTITY TZWINDOWL.AutoDesignGroup  
   RESULT = CreateEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER );

   //:// Make sure AD_Base dialog is loaded.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:nRC = ActivateAD_BaseL( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_BaseL( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base" 
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Make sure that the TZZOLFLO object of Meta ViewObjRef entries exists.
   //:GET VIEW TZZOLFLO NAMED "TZZOLFLO"
   RESULT = GetViewByName( &TZZOLFLO, "TZZOLFLO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZZOLFLO, TaskLPLR )
      CreateViewFromView( &TZZOLFLO, TaskLPLR );
      //:NAME VIEW TZZOLFLO "TZZOLFLO"
      SetNameForView( TZZOLFLO, "TZZOLFLO", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST TZZOLFLO.W_MetaType WHERE TZZOLFLO.W_MetaType.Type = 9   // 9 is read view of Registered View 
      RESULT = SetCursorFirstEntityByInteger( TZZOLFLO, "W_MetaType", "Type", 9, "" );
   } 

   //:END

   //:// Determine whether the current Group is for a Grid or Detail controls.
   //:// We also need to set up any current mapping entries.

   //:// We will first process the Group hierarchically to find if there is a Grid subcontrol anywhere.
   //:TZADWWKO.EntitySubGroup.GroupUpdateFlag = "Y"
   SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupUpdateFlag", "Y" );
   //:CreateViewFromView( TZCtlHier, TZCONTROL )
   CreateViewFromView( &TZCtlHier, TZCONTROL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:szContainsGridFlag = ""
   ZeidonStringCopy( szContainsGridFlag, 1, 0, "", 1, 0, 2 );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel AND szMappingCompleteFlag = ""
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel && ZeidonStringCompare( szMappingCompleteFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "ControlDef"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "ControlDef", 1, 0, 51 ) == 0 )
      { 
         //:szControlTag = TZCtlHier.ControlDef.Tag 
         GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "ControlDef", "Tag", "", 0 );
         //:IF szControlTag = "Grid"
         if ( ZeidonStringCompare( szControlTag, 1, 0, "Grid", 1, 0, 51 ) == 0 )
         { 
            //:szContainsGridFlag = "Y"
            ZeidonStringCopy( szContainsGridFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );

   //:// Since this is an update of existing Group, process the Group hierarchically to process mapping entries.
   //:// This may be done in either one or two hierarchical loops.
   //:// If the Group contains a Grid, the mapping entries will be created in the first loop and the second will be skipped.
   //:// If the Group doesn't contain a Grid, then the 1st loop will only be done to determine the type and the second loop 
   //:// will be processed to create the mapping entries.

   //:// Process first loop. 
   //:CreateViewFromView( TZCtlHier, TZCONTROL )
   CreateViewFromView( &TZCtlHier, TZCONTROL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:szGroupType           = ""
   ZeidonStringCopy( szGroupType, 1, 0, "", 1, 0, 2 );
   //:szMappingCompleteFlag = ""
   ZeidonStringCopy( szMappingCompleteFlag, 1, 0, "", 1, 0, 2 );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel AND szMappingCompleteFlag = ""
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel && ZeidonStringCompare( szMappingCompleteFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "ControlDef"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "ControlDef", 1, 0, 51 ) == 0 )
      { 
         //:szControlTag = TZCtlHier.ControlDef.Tag 
         GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "ControlDef", "Tag", "", 0 );
         //:IF szControlTag = "Grid"
         if ( ZeidonStringCompare( szControlTag, 1, 0, "Grid", 1, 0, 51 ) == 0 )
         { 
            //:szGroupType = "G"
            ZeidonStringCopy( szGroupType, 1, 0, "G", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 
            //:// If we've come across a mapping attribute not for a Grid, we assume it's a Prompt/Detail Pair.
            //:IF TZCtlHier.CtrlMapLOD_Attribute EXISTS AND szContainsGridFlag = ""
            lTempInteger_1 = CheckExistenceOfEntity( TZCtlHier, "CtrlMapLOD_Attribute" );
            if ( lTempInteger_1 == 0 && ZeidonStringCompare( szContainsGridFlag, 1, 0, "", 1, 0, 2 ) == 0 )
            { 
               //:szGroupType = "P"
               ZeidonStringCopy( szGroupType, 1, 0, "P", 1, 0, 2 );
            } 

            //:END
         } 

         //:END

         //:// Set up mapping entries for Grid.
         //:IF szGroupType = "G" 
         if ( ZeidonStringCompare( szGroupType, 1, 0, "G", 1, 0, 2 ) == 0 )
         { 
            //:TZADWWKO.EntitySubGroup.GroupType = "ListGroupOnly"
            SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "ListGroupOnly" );
            //:TZADWWKO.EntitySubGroup.GroupUpdateFlag = "Y"
            SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupUpdateFlag", "Y" );
            //:CreateViewFromView( TZCtlCopy, TZCtlHier )
            CreateViewFromView( &TZCtlCopy, TZCtlHier );
            //:NAME VIEW TZCtlCopy "TZCtlCopy"
            SetNameForView( TZCtlCopy, "TZCtlCopy", 0, zLEVEL_TASK );
            //:SET CURSOR FIRST TZCtlCopy.CtrlMap     // Sometimes there are 2 CtrlMap entries and the first has the mapping. 
            RESULT = SetCursorFirstEntity( TZCtlCopy, "CtrlMap", "" );
            //:INCLUDE TZADWWKO.ESG_LOD_Entity FROM TZCtlCopy.CtrlMapLOD_Entity 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_LOD_Entity", TZCtlCopy, "CtrlMapLOD_Entity", zPOS_AFTER );
            //:SetViewToSubobject( TZCtlCopy, "CtrlCtrl" )
            SetViewToSubobject( TZCtlCopy, "CtrlCtrl" );
            //:FOR EACH TZCtlCopy.Control 
            RESULT = SetCursorFirstEntity( TZCtlCopy, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF TZCtlCopy.CtrlMap EXISTS
               lTempInteger_2 = CheckExistenceOfEntity( TZCtlCopy, "CtrlMap" );
               if ( lTempInteger_2 == 0 )
               { 
                  //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZCtlCopy.CtrlMapLOD_Attribute 
                  RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZCtlCopy, "CtrlMapLOD_Attribute", zPOS_AFTER );
                  //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TZCtlCopy.Control.Text 
                  SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZCtlCopy, "Control", "Text" );
                  //:SET CURSOR FIRST TZCtlCopy.WebControlProperty WHERE TZCtlCopy.WebControlProperty.Name = "Edit"
                  RESULT = SetCursorFirstEntityByString( TZCtlCopy, "WebControlProperty", "Name", "Edit", "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "EditBox"
                     SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "EditBox" );
                     //:ELSE
                  } 
                  else
                  { 
                     //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Text"
                     SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Text" );
                  } 

                  //:END 
               } 

               RESULT = SetCursorNextEntity( TZCtlCopy, "Control", "" );
               //:END
            } 

            //:END

            //:// Activate the current VOR and LOD LOD which are used for potential mapping.
            //:// Also include Update VOR in TZADWWKO.
            //:ResetViewFromSubobject( TZCtlCopy )
            ResetViewFromSubobject( TZCtlCopy );
            //:SET CURSOR FIRST TZZOLFLO.W_MetaDef WHERE TZZOLFLO.W_MetaDef.Name = TZCtlCopy.CtrlMapView.Name  
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZCtlCopy, "CtrlMapView", "Name" );
            RESULT = SetCursorFirstEntityByString( TZZOLFLO, "W_MetaDef", "Name", szTempString_0, "" );
            //:INCLUDE TZADWWKO.W_MetaDefUpdateView FROM TZZOLFLO.W_MetaDef
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TZZOLFLO, "W_MetaDef", zPOS_AFTER );
            //:nRC = ActivateMetaOI_ByName( ViewToWindow, CurrentVOR, 0, zREFER_VOR_META, zSINGLE, TZADWWKO.W_MetaDefUpdateView.Name, 0 )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "W_MetaDefUpdateView", "Name" );
            nRC = ActivateMetaOI_ByName( ViewToWindow, &CurrentVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_0, 0 );
            //:NAME VIEW CurrentVOR "UpdateObjectViewRef"
            SetNameForView( CurrentVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
            //:nRC = ActivateMetaOI_ByName( ViewToWindow, CurrentLOD, 0, zREFER_LOD_META, zSINGLE, CurrentVOR.LOD.Name, 0 )
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), CurrentVOR, "LOD", "Name" );
            nRC = ActivateMetaOI_ByName( ViewToWindow, &CurrentLOD, 0, zREFER_LOD_META, zSINGLE, szTempString_1, 0 );
            //:NAME VIEW CurrentLOD "TZZOLODO_Update"
            SetNameForView( CurrentLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );

            //:// Activate Grid LOD and build potential list of sttributes.
            //:BuildAD_GroupPotAttrList( TZADWWKO, CurrentLOD, TZCtlCopy.CtrlMapLOD_Entity.Name, "" )
            GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZCtlCopy, "CtrlMapLOD_Entity", "Name" );
            o_BuildAD_GroupPotAttrList( TZADWWKO, CurrentLOD, szTempString_2, "" );

            //:szMappingCompleteFlag = "Y"
            ZeidonStringCopy( szMappingCompleteFlag, 1, 0, "Y", 1, 0, 2 );
            //:DropView( TZCtlCopy )
            DropView( TZCtlCopy );
         } 

         //:END 
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );

   //:// Process second loop if mapping is Prompt/Control pairs.
   //:// The logic is that any Text Control with a WebCtrlLabelLink value will be considered the Prompts and
   //:// the next Control will be considered to be the mapping Control.
   //:IF szGroupType = "P" 
   if ( ZeidonStringCompare( szGroupType, 1, 0, "P", 1, 0, 2 ) == 0 )
   { 

      //:CreateViewFromView( TZCtlHier, TZCONTROL )
      CreateViewFromView( &TZCtlHier, TZCONTROL );
      //:DefineHierarchicalCursor( TZCtlHier, "Control" )
      DefineHierarchicalCursor( TZCtlHier, "Control" );
      //:NAME VIEW TZCtlHier "TZCtlHier"
      SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
      //:szMappingCompleteFlag = ""
      ZeidonStringCopy( szMappingCompleteFlag, 1, 0, "", 1, 0, 2 );
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
      //:lInitialLevel = lReturnedLevel
      lInitialLevel = lReturnedLevel;
      //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel AND szMappingCompleteFlag = ""
      while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel && ZeidonStringCompare( szMappingCompleteFlag, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:IF nRC = zCURSOR_SET_RECURSIVECHILD
         if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         { 
            //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
            SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
         } 

         //:END
         //:IF szReturnedEntityName = "ControlDef"
         if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "ControlDef", 1, 0, 51 ) == 0 )
         { 
            //:szControlTag = TZCtlHier.ControlDef.Tag 
            GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "ControlDef", "Tag", "", 0 );
            //:IF szControlTag = "Text"
            if ( ZeidonStringCompare( szControlTag, 1, 0, "Text", 1, 0, 51 ) == 0 )
            { 
               //:szWebCtrlLabelLink = TZCtlHier.Control.WebCtrlLabelLink 
               GetVariableFromAttribute( szWebCtrlLabelLink, 0, 'S', 51, TZCtlHier, "Control", "WebCtrlLabelLink", "", 0 );
               //:IF szWebCtrlLabelLink != ""
               if ( ZeidonStringCompare( szWebCtrlLabelLink, 1, 0, "", 1, 0, 51 ) != 0 )
               { 
                  //:// This Control is the Prompt for the pair.
                  //:CreateViewFromView( TZCtlCopy, TZCtlHier )
                  CreateViewFromView( &TZCtlCopy, TZCtlHier );
                  //:NAME VIEW TZCtlCopy "TZCtlCopy"
                  SetNameForView( TZCtlCopy, "TZCtlCopy", 0, zLEVEL_TASK );
                  //:szPromptValue = TZCtlCopy.Control.Text 
                  GetVariableFromAttribute( szPromptValue, 0, 'S', 51, TZCtlCopy, "Control", "Text", "", 0 );
                  //:DropView( TZCtlCopy )
                  DropView( TZCtlCopy );

                  //:// Continue Hier Loop looking for the Detail Control.
                  //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
                  nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
                  //:szDetailFound = ""
                  ZeidonStringCopy( szDetailFound, 1, 0, "", 1, 0, 2 );
                  //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel AND szDetailFound = ""
                  while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel && ZeidonStringCompare( szDetailFound, 1, 0, "", 1, 0, 2 ) == 0 )
                  { 
                     //:IF nRC = zCURSOR_SET_RECURSIVECHILD
                     if ( nRC == zCURSOR_SET_RECURSIVECHILD )
                     { 
                        //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
                        SetViewToSubobject( TZCtlHier, "CtrlCtrl" );

                        //:// Check if this is a Control with mapping.
                        //:CreateViewFromView( TZCtlCopy, TZCtlHier )
                        CreateViewFromView( &TZCtlCopy, TZCtlHier );
                        //:NAME VIEW TZCtlCopy "TZCtlCopy"
                        SetNameForView( TZCtlCopy, "TZCtlCopy", 0, zLEVEL_TASK );
                        //:IF TZCtlCopy.CtrlMap EXISTS
                        lTempInteger_3 = CheckExistenceOfEntity( TZCtlCopy, "CtrlMap" );
                        if ( lTempInteger_3 == 0 )
                        { 
                           //:szDetailFound = "F"
                           ZeidonStringCopy( szDetailFound, 1, 0, "F", 1, 0, 2 );
                           //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZCtlCopy.CtrlMapLOD_Attribute 
                           RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZCtlCopy, "CtrlMapLOD_Attribute", zPOS_AFTER );
                           //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue = szPromptValue
                           SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", szPromptValue );
                           //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = TZCtlCopy.ControlDef.Tag 
                           SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZCtlCopy, "ControlDef", "Tag" );

                           //:// If necessary, activate the current VOR and LOD LOD which are used for potential mapping.
                           //:// Also include Update VOR in TZADWWKO.
                           //:GET VIEW CurrentVOR NAMED "UpdateObjectViewRef"
                           RESULT = GetViewByName( &CurrentVOR, "UpdateObjectViewRef", ViewToWindow, zLEVEL_TASK );
                           //:IF RESULT < 0
                           if ( RESULT < 0 )
                           { 
                              //:SET CURSOR FIRST TZZOLFLO.W_MetaDef WHERE TZZOLFLO.W_MetaDef.Name = TZCtlCopy.CtrlMapView.Name  
                              GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), TZCtlCopy, "CtrlMapView", "Name" );
                              RESULT = SetCursorFirstEntityByString( TZZOLFLO, "W_MetaDef", "Name", szTempString_3, "" );
                              //:INCLUDE TZADWWKO.W_MetaDefUpdateView FROM TZZOLFLO.W_MetaDef
                              RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TZZOLFLO, "W_MetaDef", zPOS_AFTER );
                              //:nRC = ActivateMetaOI_ByName( ViewToWindow, CurrentVOR, 0, zREFER_VOR_META, zSINGLE, TZADWWKO.W_MetaDefUpdateView.Name, 0 )
                              GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), TZADWWKO, "W_MetaDefUpdateView", "Name" );
                              nRC = ActivateMetaOI_ByName( ViewToWindow, &CurrentVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_3, 0 );
                              //:NAME VIEW CurrentVOR "UpdateObjectViewRef"
                              SetNameForView( CurrentVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
                              //:nRC = ActivateMetaOI_ByName( ViewToWindow, CurrentLOD, 0, zREFER_LOD_META, zSINGLE, CurrentVOR.LOD.Name, 0 )
                              GetStringFromAttribute( szTempString_4, zsizeof( szTempString_4 ), CurrentVOR, "LOD", "Name" );
                              nRC = ActivateMetaOI_ByName( ViewToWindow, &CurrentLOD, 0, zREFER_LOD_META, zSINGLE, szTempString_4, 0 );
                              //:NAME VIEW CurrentLOD "TZZOLODO_Update"
                              SetNameForView( CurrentLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
                           } 

                           //:END
                        } 

                        //:END
                        //:DropView( TZCtlCopy )
                        DropView( TZCtlCopy );
                     } 

                     //:END
                     //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
                     nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
                  } 

                  //:END 
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
         nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
      } 

      //:END
      //:DropView( TZCtlHier )
      DropView( TZCtlHier );

      //:// Activate Detail LOD and build potential list of sttributes.
      //:BuildAD_GroupPotAttrList( TZADWWKO, CurrentLOD, CurrentLOD.LOD_EntityParent.Name, "" )
      GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), CurrentLOD, "LOD_EntityParent", "Name" );
      o_BuildAD_GroupPotAttrList( TZADWWKO, CurrentLOD, szTempString_5, "" );
   } 

   //:END

   //:// Set Work Object Group Type.
   //:IF szGroupType = "P" 
   if ( ZeidonStringCompare( szGroupType, 1, 0, "P", 1, 0, 2 ) == 0 )
   { 
      //:TZADWWKO.EntitySubGroup.GroupType = "DetailGroup"
      SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroup" );
      //:ELSE
   } 
   else
   { 
      //:TZADWWKO.EntitySubGroup.GroupType = "ListGroupOnly"
      SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupType", "ListGroupOnly" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_AD_GroupSpecification( VIEW ViewToWindow )

//:   VIEW TZADWWKO   REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
GOTO_AD_GroupSpecification( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW SearchLOD  BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW IncludeLOD BASED ON LOD  TZZOLODO
   zVIEW     IncludeLOD = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// The Groupd for Dashboard Entry has no mapping, so we'll skip the initialization code that the other Groups use.
   //:IF TZADWWKO.EntitySubGroup.GroupType = "DetailGroupDBE"
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroupDBE" ) == 0 )
   { 
      //:CreateTemporalSubobjectVersion( TZADWWKO, "EntitySubGroup" )
      CreateTemporalSubobjectVersion( TZADWWKO, "EntitySubGroup" );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDashboardSpec" )
      SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDashboardSpec" );
      //:RETURN 
      return( 0 );
   } 

   //:END

   //:// Set up work areas for Groups other than Dashboard Entry.

   //:// Reuse the operation from Multi-Group Page.
   //:nRC = GOTO_AD_GroupUpdate( ViewToWindow )
   nRC = GOTO_AD_GroupUpdate( ViewToWindow );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN 
      return( 0 );
   } 

   //:END

   //:CreateTemporalSubobjectVersion( TZADWWKO, "EntitySubGroup" )
   CreateTemporalSubobjectVersion( TZADWWKO, "EntitySubGroup" );

   //:// Recreate potential attribute lists for existing object entries.
   //:IF TZADWWKO.ESG_ListSearchW_MetaDef EXISTS 
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szLOD_Name = TZADWWKO.ESG_ListSearchW_MetaDef.Name 
      GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
      //:NAME VIEW SearchLOD "ESGL_SearchLOD"
      SetNameForView( SearchLOD, "ESGL_SearchLOD", 0, zLEVEL_TASK );

      //:// Build the ESG_FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
      //:BuildAD_GroupPotAttrList( TZADWWKO, SearchLOD, SearchLOD.LOD_EntityParent.Name, "3" )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), SearchLOD, "LOD_EntityParent", "Name" );
      o_BuildAD_GroupPotAttrList( TZADWWKO, SearchLOD, szTempString_0, "3" );
   } 

   //:END
   //:IF TZADWWKO.ESG_ListIncludeW_MetaDef EXISTS 
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
   if ( lTempInteger_1 == 0 )
   { 
      //:szLOD_Name = TZADWWKO.ESG_ListIncludeW_MetaDef.Name 
      GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, IncludeLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &IncludeLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
      //:NAME VIEW IncludeLOD "ESGL_IncludeLOD"
      SetNameForView( IncludeLOD, "ESGL_IncludeLOD", 0, zLEVEL_TASK );
      //:TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName = IncludeLOD.LOD_EntityParent.Name 
      SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", IncludeLOD, "LOD_EntityParent", "Name" );

      //:// Build the ESG_FlatListPotentialAttribute entries for the Potential Select List Attributes.
      //:BuildAD_GroupPotAttrList( TZADWWKO, IncludeLOD, IncludeLOD.LOD_EntityParent.Name, "2" )
      GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), IncludeLOD, "LOD_EntityParent", "Name" );
      o_BuildAD_GroupPotAttrList( TZADWWKO, IncludeLOD, szTempString_1, "2" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_NewGroup( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_NewGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOSub BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOSub = 0; 
   //:VIEW TZCONTROL   REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZCONTROL2  BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL2 = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Reuse the GenEntitySubGroup operation to build the Group and an optional, subpage, depending on the Group Type.

   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );

   //:// Also activate AD_Base to pick up reusable data values.
   //:nRC = ActivateAD_BaseL( AD_Base, ViewToWindow )
   nRC = o_ActivateAD_BaseL( &AD_Base, ViewToWindow );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      return( nRC );
   } 

   //:END
   //:NAME VIEW AD_Base "AD_Base" 
   SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
   //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
   { 
      //:szTemplateWindow = "ObjectUpdateGroupsIcons" 
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroupsIcons", 1, 0, 51 );
      //:ELSE
   } 
   else
   { 
      //:szTemplateWindow = "ObjectUpdateGroups"
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroups", 1, 0, 51 );
   } 

   //:END
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = szTemplateWindow
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szTemplateWindow, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The template window, " + szTemplateWindow + ", could not be found in AD_Base."
      ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTemplateWindow, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 201 );
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Update LOD, unless the Group is a DashboardEntry.
   //:IF szGroupType != "DetailGroupDBE"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupDBE", 1, 0, 51 ) != 0 )
   { 
      //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
      RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
      //:IF RESULT < 0
      if ( RESULT < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "The Update Object has not been selected.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Update Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:END

   //:// Make sure that Mapping wasn't skipped. 
   //:// Note that we're not making sure all mapping entries have been selected, but just that the User has gone to mapping.
   //:IF szGroupType = "ListGroupOnly"         OR 
   //:   szGroupType = "ListGroupWDetailPage"  OR 
   //:   szGroupType = "ListGroupWUpdatePage"  OR
   //:   szGroupType = "ListGroupWInclude"     OR 
   //:   szGroupType = "ListGroupwFindInclude" OR 
   //:   szGroupType = "ListGroupwMGP"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 ||
        ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
   { 

      //:// For these options, ESG_ListLOD_Attribute entries must exist.
      //:SET CURSOR FIRST TZADWWKO.ESG_ListLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "No mapping entries have been selected.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "No mapping entries have been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END 
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "DetailGroup" OR szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude" 
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroup", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:// For these options, ESG_DetailLOD_Attribute entries must exist.
         //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute 
         RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
            //:          "No mapping entries have been selected.",
            //:          zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Subdialog", "No mapping entries have been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// Accept the current Control in case it is versioned and then delete it, as the GenEntitySubGroup operation
   //:// will create the necessary Group Control. The current Group Control is only used for positioning.
   //:AcceptSubobject( TZCONTROL, "Control" )
   AcceptSubobject( TZCONTROL, "Control" );
   //:DELETE ENTITY TZCONTROL.Control NONE 
   RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );

   //:// First build the Entity Group on the current page.
   //:// We will create a copy of TZCONTROL because we want to keep it's current position.
   //:CreateViewFromView( TZCONTROL2, TZCONTROL )
   CreateViewFromView( &TZCONTROL2, TZCONTROL );
   //:NAME VIEW TZCONTROL2 "TZCONTROLAD"
   SetNameForView( TZCONTROL2, "TZCONTROLAD", 0, zLEVEL_TASK );
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:szAD_Area   = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:nRC = GenEntitySubGroup( TZADWWKO, TZCONTROL2, TZWINDOWL, AD_Base, szGroupType, szAD_Area )
   nRC = oTZADWWKO_GenEntitySubGroup( TZADWWKO, TZCONTROL2, TZWINDOWL, AD_Base, szGroupType, szAD_Area );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      return( nRC );
   } 

   //:END

   //:DropView( TZCONTROL2 )
   DropView( TZCONTROL2 );

   //:// Reposition and Resize all Group Controls.
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL );

   //:// Rename some Controls to avoid duplicate Tags.
   //:SET CURSOR FIRST TZWINDOWL.Control
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:RenameAD_Controls( TZADWWKO, TZWINDOWL ) 
   oTZADWWKO_RenameAD_Controls( TZADWWKO, TZWINDOWL );

   //:// SUBWINDOWS FOR SELECTED GROUP TYPES

   //:// Next, build the subwindow if there is one for the Group Type.
   //:IF szGroupType = "ListGroupWUpdatePage" OR szGroupType = "ListGroupWDetailPage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 )
   { 

      //:// GROUP LIST UPDATE/DETAIL PAGE

      //:GenListDetailPage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow )
      oTZADWWKO_GenListDetailPage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow );

      //:ELSE
   } 
   else
   { 

      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude" OR 
      //:szGroupType = "ListGroupwInclude"   OR szGroupType = "ListGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 ||
           ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 

         //:// GROUP LIST OR DETAIL INCLUDE PAGE WITH OR WITHOUT FIND

         //:GenIncludePage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow )  
         oTZADWWKO_GenIncludePage( TZADWWKO, TZWINDOWL, AD_Base, ViewToWindow );

         //:ELSE
      } 
      else
      { 

         //:IF szGroupType = "ListGroupwMGP"   
         if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
         { 

            //:// GROUP LIST WITH MULTI-PAGE PAGES

            //:// Rename the current TZADWWKO OI and set TZADWWKOSub to TZADWWKO for use in AUTODESIGN_MPG_Pages.
            //:SetNameForView( TZADWWKO, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK )
            SetNameForView( TZADWWKO, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
            //:DropNameForView( TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK )
            DropNameForView( TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
            //:GET VIEW TZADWWKOSub NAMED "TZADWWKOSub"
            RESULT = GetViewByName( &TZADWWKOSub, "TZADWWKOSub", ViewToWindow, zLEVEL_TASK );
            //:NAME VIEW TZADWWKOSub "TZADWWKO"
            SetNameForView( TZADWWKOSub, "TZADWWKO", 0, zLEVEL_TASK );
            //:// Also rename the MGP Update LOD as it is used in building the Groups.
            //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
            RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
            //:IF RESULT >= 0
            if ( RESULT >= 0 )
            { 
               //:DropObjectInstance( UpdateLOD )
               DropObjectInstance( UpdateLOD );
            } 

            //:END 
            //:GET VIEW UpdateLOD NAMED "TZZOLODO_UpdateMGP"
            RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_UpdateMGP", ViewToWindow, zLEVEL_TASK );
            //:NAME VIEW UpdateLOD "TZZOLODO_Update"
            SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );

            //:// Reuse operation for Multi-Group Page
            //:AUTODESIGN_MPG_Pages( ViewToWindow )
            AUTODESIGN_MPG_Pages( ViewToWindow );

            //:DropObjectInstance( TZADWWKO )
            DropObjectInstance( TZADWWKO );
         } 


         //:END
      } 

      //:END
   } 

   //:END

   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( AD_Base )
      DropObjectInstance( AD_Base );
   } 

   //:END

   //:// Finally, refresh the current window to display changes to the Group.
   //:RepaintWindowControls( ViewToWindow )
   RepaintWindowControls( ViewToWindow );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_AutodesignGroup( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   return( 0 );
//    
//    // Validate current options.
//    // There are currently no functions not active.
// END
} 


//:DIALOG OPERATION
//:PostbuildGroupIncludeSpec( VIEW ViewToWindow )

//:   VIEW VOR_Meta BASED ON LOD  TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
PostbuildGroupIncludeSpec( zVIEW     ViewToWindow )
{
   zVIEW     VOR_Meta = 0; 
   //:VIEW TaskLPLR REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that VOR_Meta is positioned on Registered Named Views for View selection.
   //:GET VIEW VOR_Meta NAMED "LPLR_VOR_Meta"
   RESULT = GetViewByName( &VOR_Meta, "LPLR_VOR_Meta", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( VOR_Meta, TaskLPLR )
      CreateViewFromView( &VOR_Meta, TaskLPLR );
      //:NAME VIEW VOR_Meta "LPLR_VOR_Meta"
      SetNameForView( VOR_Meta, "LPLR_VOR_Meta", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST VOR_Meta.W_MetaType WHERE VOR_Meta.W_MetaType.Type = 9 
      RESULT = SetCursorFirstEntityByInteger( VOR_Meta, "W_MetaType", "Type", 9, "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CHECK_ReorderAttributesSub( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CHECK_ReorderAttributesSub( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO2 BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO2 = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Validate Row and Group values.
   //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute  
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
   //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute WHERE TZADWWKO.ESG_SearchLOD_Attribute.GroupNumber != ""
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "ESG_SearchLOD_Attribute", "GroupNumber", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      } 

   } 

   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Type is Group.
      //:// Make sure all GroupNumbers are set and <= 4 and all GroupRows are null.
      //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute WHERE TZADWWKO.ESG_SearchLOD_Attribute.GroupRow != ""
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "ESG_SearchLOD_Attribute", "GroupRow", "" ) == 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Search Def Return", 
         //:             "Both Row values and Group values cannot be set.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Search Def Return", "Both Row values and Group values cannot be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute WHERE TZADWWKO.ESG_SearchLOD_Attribute.GroupNumber = ""
      RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_SearchLOD_Attribute", "GroupNumber", "", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Search Def Return", 
         //:             "If one Group value is set, all need to be set.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Search Def Return", "If one Group value is set, all need to be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute WHERE TZADWWKO.ESG_SearchLOD_Attribute.GroupNumber > 4
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToInteger( TZADWWKO, "ESG_SearchLOD_Attribute", "GroupNumber", 4 ) <= 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Search Def Return", 
         //:             "A Group value cannot be greater than 4.", 
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Search Def Return", "A Group value cannot be greater than 4.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:ELSE 
   } 
   else
   { 
      //:// Type is Row.
      //:// Make sure all GroupRows are set or null.
      //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute WHERE TZADWWKO.ESG_SearchLOD_Attribute.GroupRow != ""
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "ESG_SearchLOD_Attribute", "GroupRow", "" ) == 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// One Row value has been set, so make sure they are all set.
         //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute WHERE TZADWWKO.ESG_SearchLOD_Attribute.GroupRow = ""
         RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_SearchLOD_Attribute", "GroupRow", "", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Search Def Return", 
            //:             "If one Row value is set, they must all be set.", 
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Search Def Return", "If one Row value is set, they must all be set.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
      } 

      //:END
   } 

   //:   
   //:END

   //:// Make sure that any Entity entries are properly positioned before their corresponding attributes.
   //:/*CreateViewFromView( TZADWWKO2, TZADWWKO )
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute 
   //:   IF TZADWWKO.FlatSelectedSearchAttribute.IndentEntityName != ""
   //:      SET CURSOR FIRST TZADWWKO2.FlatSelectedSearchAttribute
   //:                 WHERE TZADWWKO2.FlatSelectedSearchAttribute.EntityName = TZADWWKO.FlatSelectedSearchAttribute.EntityName
   //:      IF TZADWWKO2.FlatSelectedSearchAttribute.IndentEntityName != TZADWWKO.FlatSelectedSearchAttribute.IndentEntityName
   //:         // An Entity entry has been moved, so put it back.
   //:         CREATE ENTITY TZADWWKO2.FlatSelectedSearchAttribute BEFORE
   //:         SetMatchingAttributesByName( TZADWWKO2, "FlatSelectedSearchAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL )
   //:         DELETE ENTITY TZADWWKO.FlatSelectedSearchAttribute
   //:      END
   //:   END
   //:END*/
   //:SET CURSOR FIRST TZADWWKO.ESG_SearchLOD_Attribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
   return( 0 );
//    //RefreshWindow( ViewToWindow )
//    //DropView( TZADWWKO2 )
// END
} 


//:DIALOG OPERATION
//:REMOVE_AD_AttributeAsRangeSub( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_AD_AttributeAsRangeSub( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove the Range data from the FlatSelectedSearchAttribute entry.
   //:TZADWWKO.ESG_SearchLOD_Attribute.RangeTargetAttributeName = ""
   SetAttributeFromString( TZADWWKO, "ESG_SearchLOD_Attribute", "RangeTargetAttributeName", "" );
   //:TZADWWKO.ESG_SearchLOD_Attribute.RangeFromToType          = ""
   SetAttributeFromString( TZADWWKO, "ESG_SearchLOD_Attribute", "RangeFromToType", "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_AD_AttributeAsRangeFromSub( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeFromSub( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set the two Range values in FlatSelectedSearchAttribute from those in ESG_FlatListPotentialAttribute and
   //:// indicate the type is "From".

   //:// Make sure that Data Types match between source and target.
   //:IF TZADWWKO.ESG_SearchDomain.DomainType != TZADWWKO.ESG_FlatDomain3.DomainType 
   if ( CompareAttributeToAttribute( TZADWWKO, "ESG_SearchDomain", "DomainType", TZADWWKO, "ESG_FlatDomain3", "DomainType" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Data Types of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Data Types of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure that Entity Names match between source and target.
   //:IF TZADWWKO.ESG_SearchLOD_Entity.Name != TZADWWKO.ESG_FlatListPotentialAttribute3.EntityName 
   if ( CompareAttributeToAttribute( TZADWWKO, "ESG_SearchLOD_Entity", "Name", TZADWWKO, "ESG_FlatListPotentialAttribute3", "EntityName" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Entity Names of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Entity Names of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Set values in target.
   //:TZADWWKO.ESG_SearchLOD_Attribute.RangeTargetAttributeName = TZADWWKO.ESG_FlatListPotentialAttribute3.AttributeName 
   SetAttributeFromAttribute( TZADWWKO, "ESG_SearchLOD_Attribute", "RangeTargetAttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute3", "AttributeName" );
   //:TZADWWKO.ESG_SearchLOD_Attribute.RangeFromToType          = "From"
   SetAttributeFromString( TZADWWKO, "ESG_SearchLOD_Attribute", "RangeFromToType", "From" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_AD_AttributeAsRangeToSub( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SET_AD_AttributeAsRangeToSub( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set the two Range values in FlatSelectedSearchAttribute from those in ESG_FlatListPotentialAttribute and
   //:// indicate the type is "To".

   //:// Make sure that Data Types match between source and target.
   //:IF TZADWWKO.ESG_SearchDomain.DomainType != TZADWWKO.ESG_FlatDomain3.DomainType 
   if ( CompareAttributeToAttribute( TZADWWKO, "ESG_SearchDomain", "DomainType", TZADWWKO, "ESG_FlatDomain3", "DomainType" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Data Types of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Data Types of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure that Entity Names match between source and target.
   //:IF TZADWWKO.ESG_SearchLOD_Entity.Name != TZADWWKO.ESG_FlatListPotentialAttribute3.EntityName 
   if ( CompareAttributeToAttribute( TZADWWKO, "ESG_SearchLOD_Entity", "Name", TZADWWKO, "ESG_FlatListPotentialAttribute3", "EntityName" ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Set Range Value",
      //:             "The Entity Names of source and target do not match.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Set Range Value", "The Entity Names of source and target do not match.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Set values in target.
   //:TZADWWKO.ESG_SearchLOD_Attribute.RangeTargetAttributeName = TZADWWKO.ESG_FlatListPotentialAttribute3.AttributeName 
   SetAttributeFromAttribute( TZADWWKO, "ESG_SearchLOD_Attribute", "RangeTargetAttributeName", TZADWWKO, "ESG_FlatListPotentialAttribute3", "AttributeName" );
   //:TZADWWKO.ESG_SearchLOD_Attribute.RangeFromToType          = "To"
   SetAttributeFromString( TZADWWKO, "ESG_SearchLOD_Attribute", "RangeFromToType", "To" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_MultiGroupSubSpec( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
GOTO_MultiGroupSubSpec( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOSub BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOSub = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Rename the current TZADWWKO OI and create/update a new/existing sub TZADWWKO object for use in the subwindow.
   //:SetNameForView( TZADWWKO, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK )
   SetNameForView( TZADWWKO, "TZADWWKOMain", ViewToWindow, zLEVEL_TASK );
   //:DropNameForView( TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK )
   DropNameForView( TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:GET VIEW TZADWWKOSub NAMED "TZADWWKOSub"
   RESULT = GetViewByName( &TZADWWKOSub, "TZADWWKOSub", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:// Name existing view TZADWWKO for its use in subwindow and make temporal.
      //:// If OI doesn't exist, it will be created in postbuild.
      //:NAME VIEW TZADWWKOSub "TZADWWKO"
      SetNameForView( TZADWWKOSub, "TZADWWKO", 0, zLEVEL_TASK );
      //:CreateTemporalSubobjectVersion( TZADWWKOSub, "AutoDesignWork" ) 
      CreateTemporalSubobjectVersion( TZADWWKOSub, "AutoDesignWork" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_MultiPageMGP_Object( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_MultiPageMGP_Object( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 50 ) szVOR_Name
   zCHAR     szVOR_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// This is much the same as the SELECT_MultiPageDetailObject above except that it must execute as a 
   //:// subfunction and thus needs to name the UpdateLOD differently.

   //:// Just return if nothing was selected.
   //:IF TZADWWKO.W_MetaDefUpdateView DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
   if ( lTempInteger_0 != 0 )
   { 
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Default the suffix for MGP
   //:TZADWWKO.AutodesignSubdialog.AD_AreaSuffix = "MGP"
   SetAttributeFromString( TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix", "MGP" );

   //:// Activate the selected Object View.
   //:szVOR_Name = TZADWWKO.W_MetaDefUpdateView.Name 
   GetVariableFromAttribute( szVOR_Name, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateView", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szVOR_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szVOR_Name, 0 );
   //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
   SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );

   //:// Next Activate the LOD for the View.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_UpdateMGP"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_UpdateMGP", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( UpdateLOD )
      DropObjectInstance( UpdateLOD );
   } 

   //:END
   //:szLOD_Name = UpdateVOR.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW UpdateLOD "TZZOLODO_UpdateMGP"
   SetNameForView( UpdateLOD, "TZZOLODO_UpdateMGP", 0, zLEVEL_TASK );
   //:DropObjectInstance( UpdateVOR )
   DropObjectInstance( UpdateVOR );

   //:// Include the Object Meta, which may be used later.
   //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = szLOD_Name  
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szLOD_Name, "" );
   //:IF TZADWWKO.W_MetaDefUpdateObject EXISTS 
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
   if ( lTempInteger_1 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateObject
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateObject", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

   //:// Build the FlatListPotentialEntity entries from the update object just activated.
   //:// We have one entry in the list for each Entity in the Update Object.

   //:// First delete any current entries then rebuild them.
   //:FOR EACH TZADWWKO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.FlatListPotentialEntity NONE
      RESULT = DeleteEntity( TZADWWKO, "FlatListPotentialEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "FlatListPotentialEntity", "" );
   } 

   //:END
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.FlatListPotentialEntity 
      RESULT = CreateEntity( TZADWWKO, "FlatListPotentialEntity", zPOS_AFTER );
      //:TZADWWKO.FlatListPotentialEntity.EntityName = UpdateLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADWWKO, "FlatListPotentialEntity", "EntityName", UpdateLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADWWKO.FlatListPotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatListPotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADWWKO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADWWKO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAutodesignCRM_Subdialog( VIEW ViewToWindow )

//:   VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignCRM_Subdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO    BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW CL_Domain   BASED ON LOD  TZDGSRCO
   zVIEW     CL_Domain = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Set up work data necessary for autodesigning an AutodesignSubdialog.

   //:// Make sure TZADWWKO and the related autodesign objects exist in memory.
   //:GET VIEW TZADWWKO NAMED "TZADWWKO"
   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// The autodesign work objects need to be set up.

      //:// The TZADWWKO itself
      //:ACTIVATE TZADWWKO EMPTY 
      RESULT = ActivateEmptyObjectInstance( &TZADWWKO, "TZADWWKO", ViewToWindow, zSINGLE );
      //:NAME VIEW TZADWWKO "TZADWWKO"
      SetNameForView( TZADWWKO, "TZADWWKO", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADWWKO.AutoDesignWork 
      RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.AutodesignSubdialog 
      RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
      //:CREATE ENTITY TZADWWKO.EntitySubGroup  
      RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
      //:InitializeL_SymbolType( TZADWWKO )
      oTZADWWKO_InitializeL_SymbolType( TZADWWKO );

      //:// Remove any existing Autodesign entries from TZWINDOWL.
      //:IF TZWINDOWL.AutoDesignWindow EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AutoDesignWindow" );
      if ( lTempInteger_0 == 0 )
      { 
         //:DELETE ENTITY TZWINDOWL.AutoDesignWindow  
         RESULT = DeleteEntity( TZWINDOWL, "AutoDesignWindow", zPOS_NEXT );
      } 

      //:END
      //:CREATE ENTITY TZWINDOWL.AutoDesignWindow 
      RESULT = CreateEntity( TZWINDOWL, "AutoDesignWindow", zPOS_AFTER );
      //:CREATE ENTITY TZWINDOWL.AutoDesignGroup  
      RESULT = CreateEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER );

      //:ELSE
   } 
   else
   { 
      //:// The Object exists, also make sure the AutoDesignWork entries exist.
      //:SET CURSOR FIRST TZADWWKO.AutoDesignWork
      RESULT = SetCursorFirstEntity( TZADWWKO, "AutoDesignWork", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CREATE ENTITY TZADWWKO.AutoDesignWork 
         RESULT = CreateEntity( TZADWWKO, "AutoDesignWork", zPOS_AFTER );
         //:CREATE ENTITY TZADWWKO.AutodesignSubdialog
         RESULT = CreateEntity( TZADWWKO, "AutodesignSubdialog", zPOS_AFTER );
      } 

      //:END
   } 

   //:END

   //:// Make sure the list of Contact List Domain Types exists.
   //:IF TZADWWKO.ContactListTypeDomain DOES NOT EXIST
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ContactListTypeDomain" );
   if ( lTempInteger_1 != 0 )
   { 
      //:// Get the list of ContactListType Domain values.
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, CL_Domain, 0, zREFER_DOMAIN_META, zSINGLE, "ContactListType", 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &CL_Domain, 0, zREFER_DOMAIN_META, zSINGLE, "ContactListType", 0 );
      //:IF nRC <0
      if ( nRC < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "Domain ContactListType.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Domain ContactListType.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:NAME VIEW CL_Domain "CL_Domain"
      SetNameForView( CL_Domain, "CL_Domain", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADWWKO.ContactListTypeDomain   // Create empty entry.
      RESULT = CreateEntity( TZADWWKO, "ContactListTypeDomain", zPOS_AFTER );
      //:FOR EACH CL_Domain.TableEntry 
      RESULT = SetCursorFirstEntity( CL_Domain, "TableEntry", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CREATE ENTITY TZADWWKO.ContactListTypeDomain 
         RESULT = CreateEntity( TZADWWKO, "ContactListTypeDomain", zPOS_AFTER );
         //:TZADWWKO.ContactListTypeDomain.DisplayedValue = CL_Domain.TableEntry.ExternalValue 
         SetAttributeFromAttribute( TZADWWKO, "ContactListTypeDomain", "DisplayedValue", CL_Domain, "TableEntry", "ExternalValue" );
         //:TZADWWKO.ContactListTypeDomain.InternalType   = CL_Domain.TableEntry.InternalValue 
         SetAttributeFromAttribute( TZADWWKO, "ContactListTypeDomain", "InternalType", CL_Domain, "TableEntry", "InternalValue" );
         RESULT = SetCursorNextEntity( CL_Domain, "TableEntry", "" );
      } 

      //:END 
      //:DropObjectInstance( CL_Domain )
      DropObjectInstance( CL_Domain );
   } 

   //:END
   //:SET CURSOR FIRST TZADWWKO.ContactListTypeDomain
   RESULT = SetCursorFirstEntity( TZADWWKO, "ContactListTypeDomain", "" );

   //:// If an update LOD exists, make sure that we also name it TZZOLODO_Desc for the combobox.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:NAME VIEW UpdateLOD "TZZOLODO_Desc"     // We need this named view for the Root select combobox.
      SetNameForView( UpdateLOD, "TZZOLODO_Desc", 0, zLEVEL_TASK );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ExactMatchAttribute( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ExactMatchAttribute( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zSHORT    lTempInteger_2; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Include the selected ExactMatch Attribute and set its name.
   //:IF TZADWWKO.SelectedExactMatchAttribute EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "SelectedExactMatchAttribute" );
   if ( lTempInteger_0 == 0 )
   { 
      //:EXCLUDE TZADWWKO.SelectedExactMatchAttribute  
      RESULT = ExcludeEntity( TZADWWKO, "SelectedExactMatchAttribute", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.SelectedExactMatchAttribute FROM TZADWWKO.PotentialExactMatchAttribute 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "SelectedExactMatchAttribute", TZADWWKO, "PotentialExactMatchAttribute", zPOS_AFTER );
   //:TZADWWKO.SelectedExactMatchAttribute.wAttributeName = TZADWWKO.PotentialExactMatchER_Attribute.Name 
   SetAttributeFromAttribute( TZADWWKO, "SelectedExactMatchAttribute", "wAttributeName", TZADWWKO, "PotentialExactMatchER_Attribute", "Name" );

   //:// Also add the Attribute to the selected list.
   //:// Make sure that it's the first in the list
   //:SET CURSOR FIRST TZADWWKO.ESG_FlatLOD_Attribute WITHIN TZADWWKO.AutoDesignWork 
   //:           WHERE TZADWWKO.ESG_FlatLOD_Attribute.ZKey = TZADWWKO.SelectedExactMatchAttribute.ZKey 
   GetIntegerFromAttribute( &lTempInteger_1, TZADWWKO, "SelectedExactMatchAttribute", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_FlatLOD_Attribute", "ZKey", lTempInteger_1, "AutoDesignWork" );
   //:IF TZADWWKO.FlatSelectedSearchAttribute EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( TZADWWKO, "FlatSelectedSearchAttribute" );
   if ( lTempInteger_2 == 0 )
   { 
      //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute
      RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      //:CREATE ENTITY TZADWWKO.FlatSelectedSearchAttribute BEFORE 
      RESULT = CreateEntity( TZADWWKO, "FlatSelectedSearchAttribute", zPOS_BEFORE );
      //:ELSE
   } 
   else
   { 
      //:CREATE ENTITY TZADWWKO.FlatSelectedSearchAttribute
      RESULT = CreateEntity( TZADWWKO, "FlatSelectedSearchAttribute", zPOS_AFTER );
   } 

   //:END
   //:SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_FlatListPotentialAttribute", zSET_ALL )
   SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_FlatListPotentialAttribute", zSET_ALL );
   //:TZADWWKO.FlatSelectedSearchAttribute.DataType = TZADWWKO.ESG_FlatDomain.DataType 
   SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "DataType", TZADWWKO, "ESG_FlatDomain", "DataType" );
   //:INCLUDE TZADWWKO.FlatSearchLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatSearchLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_DashboardGroup( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_DashboardGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:STRING ( 5 ) szSuffix
   zCHAR     szSuffix[ 6 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Make sure both Group Area Name and Area Title and Suffix have been entered
   //:IF TZADWWKO.EntitySubGroup.GroupAreaName          = "" OR 
   //:   TZADWWKO.EntitySubGroup.GroupAreaTitle         = "" OR 
   //:   TZADWWKO.EntitySubGroup.DashboardRunTimeSuffix = ""
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaName", "" ) == 0 || CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "" ) == 0 ||
        CompareAttributeToString( TZADWWKO, "EntitySubGroup", "DashboardRunTimeSuffix", "" ) == 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "A Group Area Name, Title and Run Time Suffix must all be specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "A Group Area Name, Title and Run Time Suffix must all be specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure the suffix is a number from 1 to 5.
   //:szSuffix = TZADWWKO.EntitySubGroup.DashboardRunTimeSuffix 
   GetVariableFromAttribute( szSuffix, 0, 'S', 6, TZADWWKO, "EntitySubGroup", "DashboardRunTimeSuffix", "", 0 );
   //:IF szSuffix != "1" AND   
   //:   szSuffix != "2" AND 
   //:   szSuffix != "3" AND 
   //:   szSuffix != "4" AND 
   //:   szSuffix != "5" AND 
   //:   szSuffix != "6" 
   if ( ZeidonStringCompare( szSuffix, 1, 0, "1", 1, 0, 6 ) != 0 && ZeidonStringCompare( szSuffix, 1, 0, "2", 1, 0, 6 ) != 0 && ZeidonStringCompare( szSuffix, 1, 0, "3", 1, 0, 6 ) != 0 && ZeidonStringCompare( szSuffix, 1, 0, "4", 1, 0, 6 ) != 0 &&
        ZeidonStringCompare( szSuffix, 1, 0, "5", 1, 0, 6 ) != 0 && ZeidonStringCompare( szSuffix, 1, 0, "6", 1, 0, 6 ) != 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Run DateTime Suffix must be an integer from 1 to 6.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Run DateTime Suffix must be an integer from 1 to 6.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:AcceptSubobject( TZADWWKO, "EntitySubGroup" )
   AcceptSubobject( TZADWWKO, "EntitySubGroup" );

   //:// The Action/Operation Suffix will be the same as the Group Area Name, at least for now.
   //:TZADWWKO.EntitySubGroup.IncludeOperationSuffix = TZADWWKO.EntitySubGroup.GroupAreaName 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix", TZADWWKO, "EntitySubGroup", "GroupAreaName" );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
