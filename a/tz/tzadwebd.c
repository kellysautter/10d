#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
#include "TZVMLIP.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
SELECT_VOR_ForAutodesign( zVIEW     ViewToWindow );


static zSHORT
o_RenameControlsRecurs( zVIEW     TZWINDOW,
                        zPCHAR    szPrefix,
                        zLONG     nLevel );


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
SELECT_ESGL_IncludeObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_SearchObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_SearchObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignCRMSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_AutoDesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_AutoDesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_PotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SAVE_AutoDesignForGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_GroupOld( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_Group( zVIEW     ViewToWindow );


static zSHORT
o_AutodesignGridCtrl( zVIEW     TZWINDOWL,
                      zVIEW     TZCONTROL,
                      zVIEW     AD_Base,
                      zVIEW     SelectedLOD );


static zSHORT
o_BuildAutodesignGroupPotList( zVIEW     TZADCSDO,
                               zVIEW     SelectedLOD,
                               zLONG     lLevel );


zOPER_EXPORT zSHORT OPERATION
SELECT_TopEntityForAutodesign( zVIEW     ViewToWindow );


static zSHORT
o_LocateTopEntityRecurs( zVIEW     TZADCSDO,
                         zVIEW     SelectedLOD,
                         zPCHAR    szTopEntityName );


zOPER_EXPORT zSHORT OPERATION
SELECT_PotentialAttributes( zVIEW     ViewToWindow );


static zSHORT
o_AutodesignUpdateCtrls( zVIEW     TZWINDOW,
                         zVIEW     TZCONTROL,
                         zVIEW     AD_Base,
                         zVIEW     SelectedLOD );


zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignForGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_SelectedControlTypes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
RECALCULATE_UpdatePromptLength( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CHECK_ReorderAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_CRM_FindSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_ObjectFindSubdialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_ReturnedObject( zVIEW     ViewToWindow );


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
SELECT_CRM_DetailObject( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_PotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_CRM_PotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_TypeForSelectedGroups( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_ListPotAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_ListPotAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_DetailPotAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_DetailPotAttributes( zVIEW     ViewToWindow );


static zSHORT
o_BuildAD_GroupPotList( zVIEW     TZADWWKO,
                        zVIEW     SelectedLOD,
                        zPCHAR    szStartEntityName,
                        zPCHAR    szFlatListNameSuffix );


zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_DetailDialog( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_AD_GroupUpdate( zVIEW     ViewToWindow );


static zVOID
o_AddAD_ListUpdateActions( zVIEW     TZCONTROL,
                           zVIEW     TZWINDOWL,
                           zVIEW     AD_BaseCtl,
                           zVIEW     AD_Base );


static zSHORT
o_AddAD_ListIncludeActions( zVIEW     TZCONTROL,
                            zVIEW     TZWINDOWL,
                            zVIEW     AD_BaseCtl,
                            zVIEW     AD_Base );


static zSHORT
o_AddAD_ListMapping( zVIEW     TZCONTROL,
                     zVIEW     TZWINDOWL,
                     zVIEW     MappingLOD,
                     zPCHAR    szGridMappingEntityName,
                     zPCHAR    szMappingPrefix );


zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignCRM_DetailDialog( zVIEW     ViewToWindow );


static zVOID
o_ReturnAttrControlType( zVIEW     AnyView,
                         zPCHAR    szDomainName,
                         zPCHAR    szDomainType,
                         zPCHAR    szControlType,
                         zPCHAR    szUpdateFlag );


static zVOID
o_GenAD_GroupNames( zVIEW     TZCONTROL,
                    zPCHAR    szGroupArea,
                    zPCHAR    szEntityName,
                    zLONG     nLevel );


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
SELECT_AutodesignGroupType( zVIEW     ViewToWindow );


static zSHORT
o_ActivateAD_Base( zPVIEW    ReturnedAD_BaseView,
                   zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_PromptCSS_ClassForAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SET_CtrlCSS_ClassForAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroupTemplate( zVIEW     ViewToWindow );


static zVOID
o_InitializeUpdateGroupsForCSS( zVIEW     AD_Base,
                                zVIEW     TZADWWKO,
                                zPCHAR    szBaseWindowName,
                                zPCHAR    szBaseGroupName );


static zLONG
o_CloneControlAD( zVIEW     ViewToWindow,
                  zVIEW     TZCONTROL,
                  zVIEW     TZWINDOWL,
                  zVIEW     AD_BaseCtl );


static zVOID
o_BuildAD_GroupPotListR( zVIEW     TZADWWKO,
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


static zVOID
o_AddFindRangeInitializationValue( zVIEW     TZADWWKO,
                                   zVIEW     TZWINDOW,
                                   zLONG     nFileOut );


zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignObjectFindSubdlg( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_FindReturnedListAttribs( zVIEW     ViewToWindow );


static zVOID
o_RenameControls( zVIEW     TZWINDOW );


//:DIALOG OPERATION
//:SELECT_VOR_ForAutodesign( VIEW ViewToWindow )

//:   VIEW SelectedLOD BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
SELECT_VOR_ForAutodesign( zVIEW     ViewToWindow )
{
   zVIEW     SelectedLOD = 0; 
   //:VIEW TZWINDOWL   BASED ON LOD TZWDLGSO
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZADCSDO    BASED ON LOD TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 


   //:// For the selected view under the AutoDesign Group, initialie a UI Spec for and create the list of selectable entities.

   //:GET VIEW TZWINDOWL NAMED "TZWINDOWL"
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   //:nRC = ActivateMetaOI_ByZKey( ViewToWindow, SelectedLOD, 0, zREFER_LOD_META, zSINGLE, TZWINDOWL.AD_GroupViewObjRefLOD.ZKey, 0 )
   GetIntegerFromAttribute( &lTempInteger_0, TZWINDOWL, "AD_GroupViewObjRefLOD", "ZKey" );
   nRC = ActivateMetaOI_ByZKey( ViewToWindow, &SelectedLOD, 0, zREFER_LOD_META, zSINGLE, lTempInteger_0, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "The Lod Object could not be read.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "The Lod Object could not be read.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW SelectedLOD "AutodesignSelectedLOD"
   SetNameForView( SelectedLOD, "AutodesignSelectedLOD", 0, zLEVEL_TASK );

   //:GET VIEW TZADCSDO NAMED "TZADCSDO"
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( TZADCSDO )
      DropObjectInstance( TZADCSDO );
   } 

   //:END

   //:ActivateEmptyMetaOI( ViewToWindow, TZADCSDO, zSOURCE_UIS_META, zSINGLE )
   ActivateEmptyMetaOI( ViewToWindow, &TZADCSDO, zSOURCE_UIS_META, zSINGLE );
   //:NAME VIEW TZADCSDO "TZADCSDO"
   SetNameForView( TZADCSDO, "TZADCSDO", 0, zLEVEL_TASK );
   //:CREATE ENTITY TZADCSDO.UI_Spec
   RESULT = CreateEntity( TZADCSDO, "UI_Spec", zPOS_AFTER );

   //:// Build Potential Flat List of entities.
   //:FOR EACH SelectedLOD.LOD_Entity
   RESULT = SetCursorFirstEntity( SelectedLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADCSDO.FlatListPotentialTopEntity
      RESULT = CreateEntity( TZADCSDO, "FlatListPotentialTopEntity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialTopEntity", SelectedLOD, "LOD_Entity", zSET_ALL )
      SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialTopEntity", SelectedLOD, "LOD_Entity", zSET_ALL );
      RESULT = SetCursorNextEntity( SelectedLOD, "LOD_Entity", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:RenameControlsRecurs( VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                      STRING ( 50 ) szPrefix,
//:                      INTEGER nLevel )

//:   STRING ( 50 ) szControlName
static zSHORT
o_RenameControlsRecurs( zVIEW     TZWINDOW,
                        zPCHAR    szPrefix,
                        zLONG     nLevel )
{
   zCHAR     szControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szNewControlName
   zCHAR     szNewControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLevel
   zCHAR     szLevel[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLocalPrefix
   zCHAR     szLocalPrefix[ 51 ] = { 0 }; 
   //:STRING ( 5 )  szCount
   zCHAR     szCount[ 6 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER TopCount
   zLONG     TopCount = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// Recursive routine to name GroupBox, Grid and PushBtn controls to avoid duplicate generated names.
   //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
   SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
   //:nLevel = nLevel + 1
   nLevel = nLevel + 1;
   //:TopCount = 0
   TopCount = 0;
   //:FOR EACH TZWINDOW.Control 
   RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TopCount = TopCount + 1
      TopCount = TopCount + 1;
      //:szCount = TopCount
      ZeidonStringConvertFromNumber( szCount, 1, 0, 5, TopCount, (ZDecimal) 0.0, "I" );
      //:szLocalPrefix = szPrefix + szCount
      ZeidonStringCopy( szLocalPrefix, 1, 0, szPrefix, 1, 0, 51 );
      ZeidonStringConcat( szLocalPrefix, 1, 0, szCount, 1, 0, 51 );
      //:IF TZWINDOW.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:// Add the Prefix and TopCount to the outside GroupBox Tag.
         //:szNewControlName = szLocalPrefix
         ZeidonStringCopy( szNewControlName, 1, 0, szLocalPrefix, 1, 0, 51 );
         //:TZWINDOW.Control.Tag = szNewControlName
         SetAttributeFromString( TZWINDOW, "Control", "Tag", szNewControlName );
         //:IF TZWINDOW.CtrlCtrl EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TZWINDOW, "CtrlCtrl" );
         if ( lTempInteger_0 == 0 )
         { 
            //:RenameControlsRecurs( TZWINDOW, szNewControlName, nLevel )
            o_RenameControlsRecurs( TZWINDOW, szNewControlName, nLevel );
         } 

         //:END
         //:ELSE
      } 
      else
      { 

         //:IF TZWINDOW.ControlDef.Tag = "PushBtn"
         if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "PushBtn" ) == 0 )
         { 
            //:// Add the Prefix at the beginning of the Control Tag.
            //:szControlName = TZWINDOW.Control.Tag 
            GetVariableFromAttribute( szControlName, 0, 'S', 51, TZWINDOW, "Control", "Tag", "", 0 );
            //:szNewControlName = szLocalPrefix + szControlName
            ZeidonStringCopy( szNewControlName, 1, 0, szLocalPrefix, 1, 0, 51 );
            ZeidonStringConcat( szNewControlName, 1, 0, szControlName, 1, 0, 51 );
            //:TZWINDOW.Control.Tag = szNewControlName
            SetAttributeFromString( TZWINDOW, "Control", "Tag", szNewControlName );
            //:ELSE
         } 
         else
         { 

            //:IF TZWINDOW.ControlDef.Tag = "Grid"
            if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "Grid" ) == 0 )
            { 
               //:// Add Prefix to Grid and Grid suboontrols.
               //:TZWINDOW.Control.Tag = "Grid" + szLocalPrefix
               ZeidonStringCopy( szTempString_0, 1, 0, "Grid", 1, 0, 33 );
               ZeidonStringConcat( szTempString_0, 1, 0, szLocalPrefix, 1, 0, 33 );
               SetAttributeFromString( TZWINDOW, "Control", "Tag", szTempString_0 );
               //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
               SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
               //:Count = 0
               Count = 0;
               //:FOR EACH TZWINDOW.Control 
               RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:Count = Count + 1
                  Count = Count + 1;
                  //:szCount = Count
                  ZeidonStringConvertFromNumber( szCount, 1, 0, 5, Count, (ZDecimal) 0.0, "I" );
                  //:TZWINDOW.Control.Tag = "Grid" + szLocalPrefix + szCount
                  ZeidonStringCopy( szTempString_1, 1, 0, "Grid", 1, 0, 33 );
                  ZeidonStringConcat( szTempString_1, 1, 0, szLocalPrefix, 1, 0, 33 );
                  ZeidonStringConcat( szTempString_1, 1, 0, szCount, 1, 0, 33 );
                  SetAttributeFromString( TZWINDOW, "Control", "Tag", szTempString_1 );
                  RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
               } 

               //:END
               //:ResetViewFromSubobject( TZWINDOW )
               ResetViewFromSubobject( TZWINDOW );
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
      //:END
   } 

   //:END
   //:ResetViewFromSubobject( TZWINDOW )
   ResetViewFromSubobject( TZWINDOW );
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
         //:SET CURSOR FIRST TZADWWKO2.ESG_ListLOD_Attribute 
         //:           WHERE TZADWWKO2.ESG_ListLOD_Entity.Name   = TZADWWKO.ESG_FlatListPotentialAttribute.EntityName  
         //:             AND TZADWWKO2.ESG_ListER_Attribute.Name = TZADWWKO.ESG_FlatListPotentialAttribute.AttributeName 
         RESULT = SetCursorFirstEntity( TZADWWKO2, "ESG_ListLOD_Attribute", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADWWKO2, "ESG_ListLOD_Entity", "Name", TZADWWKO, "ESG_FlatListPotentialAttribute", "EntityName" ) != 0 ||
                    CompareAttributeToAttribute( TZADWWKO2, "ESG_ListER_Attribute", "Name", TZADWWKO, "ESG_FlatListPotentialAttribute", "AttributeName" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZADWWKO2, "ESG_ListLOD_Attribute", "" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
            //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute.PromptValue 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptValue" );
            //:TZADWWKO.ESG_ListLOD_Attribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
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

   //:// Set the two Range values in FlatSelectedSearchAttribute from those in FlatListPotentialAttribute and
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
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that both the Search and Include MetaDef entries have been selected.
   //:IF TZADWWKO.ESG_ListSearchW_MetaDef DOES NOT EXIST OR 
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
   //:   TZADWWKO.ESG_ListIncludeW_MetaDef DOES NOT EXIST
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
   if ( lTempInteger_0 != 0 || lTempInteger_1 != 0 )
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
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_ESGL_IncludeObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_ESGL_IncludeObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW SearchLOD BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:INTEGER       nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// For the selected object, build the TZADWWKO.ESG_FlatListPotentialAttribute entries for each Entity/Attribute in the object.

   //:// Activate the selected Search Object.
   //:GET VIEW SearchLOD NAMED "ESGL_ListIncludeLOD"
   RESULT = GetViewByName( &SearchLOD, "ESGL_ListIncludeLOD", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( SearchLOD )
      DropObjectInstance( SearchLOD );
   } 

   //:END
   //:szLOD_Name = TZADWWKO.ESG_ListIncludeW_MetaDef.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 33, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW SearchLOD "ESGL_ListIncludeLOD"
   SetNameForView( SearchLOD, "ESGL_ListIncludeLOD", 0, zLEVEL_TASK );
   //:TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName = SearchLOD.LOD_EntityParent.Name 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", SearchLOD, "LOD_EntityParent", "Name" );

   //:// Make sure any existing entries are deleted.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute3
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute3 NONE 
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
   } 

   //:END

   //:// Build the FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
   //:BuildAD_GroupPotList( TZADWWKO, SearchLOD, SearchLOD.LOD_EntityParent.Name, "3" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), SearchLOD, "LOD_EntityParent", "Name" );
   o_BuildAD_GroupPotList( TZADWWKO, SearchLOD, szTempString_0, "3" );
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
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:INTEGER       nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );

   //:// For the selected object, build the TZADWWKO.ESG_FlatListPotentialAttribute entries for each Entity/Attribute in the object.

   //:// Activate the selected Search Object.
   //:GET VIEW SearchLOD NAMED "ESGL_ListSearchLOD"
   RESULT = GetViewByName( &SearchLOD, "ESGL_ListSearchLOD", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( SearchLOD )
      DropObjectInstance( SearchLOD );
   } 

   //:END
   //:szLOD_Name = TZADWWKO.ESG_ListSearchW_MetaDef.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 33, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW SearchLOD "ESGL_ListSearchLOD"
   SetNameForView( SearchLOD, "ESGL_ListSearchLOD", 0, zLEVEL_TASK );

   //:// Make sure any existing entries are deleted.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute2
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute2 NONE 
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   } 

   //:END 

   //:// Build the FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
   //:BuildAD_GroupPotList( TZADWWKO, SearchLOD, SearchLOD.LOD_EntityParent.Name, "2" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), SearchLOD, "LOD_EntityParent", "Name" );
   o_BuildAD_GroupPotList( TZADWWKO, SearchLOD, szTempString_0, "2" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_CRM_SearchObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_SearchObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //://VIEW TZADCSDO  REGISTERED AS TZADCSDO
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

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// For the selected object, build the TZADCSDO.FlatListPotentialAttribute entries for each Entity/Attribute in the object.

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
   //:NAME VIEW SearchLOD "TZZOLODO_Desc"    // SearchLOD is also the returned object
   SetNameForView( SearchLOD, "TZZOLODO_Desc", 0, zLEVEL_TASK );

   //:// Clear any existing TZADCSDO.FlatListPotentialAttribute entries and rebuild them from selected object.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE  
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END

   //:// Build list of potential attributes to be selected. 
   //:szEntityName = SearchLOD.LOD_EntityParent.Name 
   GetVariableFromAttribute( szEntityName, 0, 'S', 51, SearchLOD, "LOD_EntityParent", "Name", "", 0 );
   //:BuildAD_GroupPotList( TZADWWKO, SearchLOD, szEntityName, "" )
   o_BuildAD_GroupPotList( TZADWWKO, SearchLOD, szEntityName, "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAutodesignCRMSubdialog( VIEW ViewToWindow )

//:   VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignCRMSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO    BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW TZADCSDO    BASED ON LOD  TZADCSDO
   zVIEW     TZADCSDO = 0; 
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

      //:// TZADCSDO object for holding search group mapping attributes.
      //:// We will always begin by deleting the existing object.
      //:GET VIEW TZADCSDO NAMED "TZADCSDO"
      RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
      //:IF RESULT >= 0
      if ( RESULT >= 0 )
      { 
         //:DropObjectInstance( TZADCSDO )
         DropObjectInstance( TZADCSDO );
      } 

      //:END
      //:ActivateEmptyMetaOI( ViewToWindow, TZADCSDO, zSOURCE_UIS_META, zSINGLE )
      ActivateEmptyMetaOI( ViewToWindow, &TZADCSDO, zSOURCE_UIS_META, zSINGLE );
      //:NAME VIEW TZADCSDO "TZADCSDO"
      SetNameForView( TZADCSDO, "TZADCSDO", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADCSDO.UI_Spec
      RESULT = CreateEntity( TZADCSDO, "UI_Spec", zPOS_AFTER );

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
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_AutoDesignGroup( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
CANCEL_AutoDesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:DropObjectInstance( TZADWWKO )
   DropObjectInstance( TZADWWKO );
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
//:REMOVE_PotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADCSDO  REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
REMOVE_PotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADCSDO.FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute" )
      nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:DELETE ENTITY TZADCSDO.FlatListPotentialAttribute NONE
         RESULT = DeleteEntity( TZADCSDO, "FlatListPotentialAttribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SAVE_AutoDesignForGroup( VIEW ViewToWindow )

//:   VIEW TZADCSDO    BASED ON LOD  TZADCSDO
zOPER_EXPORT zSHORT OPERATION
SAVE_AutoDesignForGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   //:VIEW TZWINDOWL   BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZCONTROL   BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 
   //:VIEW SelectedLOD BASED ON LOD  TZZOLODO
   zVIEW     SelectedLOD = 0; 
   //:STRING ( 32 ) szTag
   zCHAR     szTag[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:// Make sure that at least one attribute has been selected.
   //:GET VIEW TZADCSDO NAMED "TZADCSDO"
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
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
   //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute WHERE TZADCSDO.FlatListSelectedAttribute.AttributeName != ""
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "AttributeName", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      } 

   } 

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
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

   //:// Set the AutoDesign attributes in TZWINDOWL from the temporary ones in TZADCSDO.
   //:GET VIEW TZWINDOWL NAMED "TZWINDOWL"
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   //:IF TZWINDOWL.AutoDesignGroup.GenerateGroupType = "F"
   if ( CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "GenerateGroupType", "F" ) == 0 )
   { 
      //:// AutoDesign Request is for regular controls on a Groupbox.
      //:FOR EACH TZWINDOWL.AD_MappingAttribute
      RESULT = SetCursorFirstEntity( TZWINDOWL, "AD_MappingAttribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:DELETE ENTITY TZWINDOWL.AD_MappingAttribute NONE
         RESULT = DeleteEntity( TZWINDOWL, "AD_MappingAttribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZWINDOWL, "AD_MappingAttribute", "" );
      } 

      //:END
      //:FOR EACH TZADCSDO.FlatListSelectedAttribute
      RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TZADCSDO.FlatListSelectedAttribute.EntityName != ""
         if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "EntityName", "" ) != 0 )
         { 
            //:szEntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
            GetVariableFromAttribute( szEntityName, 0, 'S', 33, TZADCSDO, "FlatListSelectedAttribute", "EntityName", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER );
            //:TZWINDOWL.AD_MappingAttribute.EntityName    = szEntityName
            SetAttributeFromString( TZWINDOWL, "AD_MappingAttribute", "EntityName", szEntityName );
            //:TZWINDOWL.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" );
            //:TZWINDOWL.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.ControlType
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "ControlType" );
         } 

         RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// AutoDesign Request is for a Grid control.
      //:IF TZWINDOWL.AD_ListBoxEntity EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AD_ListBoxEntity" );
      if ( lTempInteger_0 == 0 )
      { 
         //:FOR EACH TZWINDOWL.AD_MappingAttribute
         RESULT = SetCursorFirstEntity( TZWINDOWL, "AD_MappingAttribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:DELETE ENTITY TZWINDOWL.AD_MappingAttribute NONE
            RESULT = DeleteEntity( TZWINDOWL, "AD_MappingAttribute", zREPOS_NONE );
            RESULT = SetCursorNextEntity( TZWINDOWL, "AD_MappingAttribute", "" );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_ListBoxEntity", zPOS_AFTER )
         CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_ListBoxEntity", zPOS_AFTER );
         //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute
         RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
         //:TZWINDOWL.AD_ListBoxEntity.EntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
         SetAttributeFromAttribute( TZWINDOWL, "AD_ListBoxEntity", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
      } 

      //:END
      //:FOR EACH TZADCSDO.FlatListSelectedAttribute
      RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TZADCSDO.FlatListSelectedAttribute.EntityName != ""
         if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "EntityName", "" ) != 0 )
         { 
            //:szEntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
            GetVariableFromAttribute( szEntityName, 0, 'S', 33, TZADCSDO, "FlatListSelectedAttribute", "EntityName", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER );
            //:TZWINDOWL.AD_MappingAttribute.EntityName    = szEntityName
            SetAttributeFromString( TZWINDOWL, "AD_MappingAttribute", "EntityName", szEntityName );
            //:TZWINDOWL.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" );
            //:TZWINDOWL.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.ControlType
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "ControlType" );
         } 

         RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
         //:END
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_GroupOld( VIEW ViewToWindow )

//:   VIEW TZADCSDO    REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_GroupOld( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZCONTROL   REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW SelectedLOD BASED ON LOD  TZZOLODO
   zVIEW     SelectedLOD = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:STRING ( 32 ) szTag
   zCHAR     szTag[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );

   //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute WHERE TZADCSDO.FlatListSelectedAttribute.AttributeName != ""
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "AttributeName", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      } 

   } 

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
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

   //:// Make sure that a Base Window and Group has been selected.
   //:IF TZWINDOWL.AutoDesignGroup.BaseWindowName = "" OR TZWINDOWL.AutoDesignGroup.BaseGroupName = ""
   if ( CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "BaseWindowName", "" ) == 0 || CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "BaseGroupName", "" ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "Both a Base Window Name and a Base Group Name must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "Both a Base Window Name and a Base Group Name must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Position on the Base Window and  Control.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:TraceLineS( "*** BaseWindowName: ", TZWINDOWL.AutoDesignGroup.BaseWindowName )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZWINDOWL, "AutoDesignGroup", "BaseWindowName" );
   TraceLineS( "*** BaseWindowName: ", szTempString_0 );
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = TZWINDOWL.AutoDesignGroup.BaseWindowName
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZWINDOWL, "AutoDesignGroup", "BaseWindowName" );
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szTempString_1, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "The Base window name specified doesn't exist in AD_Base.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "The Base window name specified doesn't exist in AD_Base.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:TraceLineS( "*** BaseGroupName: ", TZWINDOWL.AutoDesignGroup.BaseGroupName )
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZWINDOWL, "AutoDesignGroup", "BaseGroupName" );
   TraceLineS( "*** BaseGroupName: ", szTempString_1 );
   //:SET CURSOR FIRST AD_Base.Control WHERE AD_Base.Control.Tag = TZWINDOWL.AutoDesignGroup.BaseGroupName
   GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZWINDOWL, "AutoDesignGroup", "BaseGroupName" );
   RESULT = SetCursorFirstEntityByString( AD_Base, "Control", "Tag", szTempString_2, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "The Base group name specified doesn't exist for the window in AD_Base.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "The Base group name specified doesn't exist for the window in AD_Base.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// If the Group Control has current subcontrols, get rid of them.
   //:IF TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:FOR EACH TZCONTROL.Control
      RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:DELETE ENTITY TZCONTROL.Control NONE
         RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      } 

      //:END
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
   } 

   //:END

   //:// Delete any existing mapping entities.
   //:IF TZWINDOWL.AD_ListBoxEntity EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZWINDOWL, "AD_ListBoxEntity" );
   if ( lTempInteger_1 == 0 )
   { 
      //:DELETE ENTITY TZWINDOWL.AD_ListBoxEntity
      RESULT = DeleteEntity( TZWINDOWL, "AD_ListBoxEntity", zPOS_NEXT );
   } 

   //:END
   //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_ListBoxEntity", zPOS_AFTER )
   CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_ListBoxEntity", zPOS_AFTER );
   //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute   // We'll always create a AD_ListBoxEntity entity.
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   //:TZWINDOWL.AD_ListBoxEntity.EntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
   SetAttributeFromAttribute( TZWINDOWL, "AD_ListBoxEntity", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
   //:AcceptSubobject( TZWINDOWL, "AutoDesignGroup" )
   AcceptSubobject( TZWINDOWL, "AutoDesignGroup" );

   //:// Set the AutoDesign attributes in TZWINDOWL from the temporary ones in TZADCSDO.
   //:IF TZWINDOWL.AutoDesignGroup.GenerateGroupType = "F"
   if ( CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "GenerateGroupType", "F" ) == 0 )
   { 
      //:// AutoDesign Request is for regular controls on a Groupbox.
      //:FOR EACH TZADCSDO.FlatListSelectedAttribute
      RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TZADCSDO.FlatListSelectedAttribute.AttributeName != ""
         if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "AttributeName", "" ) != 0 )
         { 
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER );
            //:TZWINDOWL.AD_MappingAttribute.EntityName    = TZADCSDO.FlatListSelectedAttribute.EntityName
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
            //:TZWINDOWL.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" );
            //:TZWINDOWL.AD_MappingAttribute.ControlType   = TZADCSDO.FlatListSelectedAttribute.ControlType
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "ControlType", TZADCSDO, "FlatListSelectedAttribute", "ControlType" );
            //:TZWINDOWL.AD_MappingAttribute.DataWidth     = TZADCSDO.FlatListSelectedAttribute.DataWidth
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "DataWidth", TZADCSDO, "FlatListSelectedAttribute", "DataWidth" );
            //:TZWINDOWL.AD_MappingAttribute.PromptValue   = TZADCSDO.FlatListSelectedAttribute.PromptValue
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "PromptValue", TZADCSDO, "FlatListSelectedAttribute", "PromptValue" );
         } 

         RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// AutoDesign Request is for a Grid control.
      //:FOR EACH TZADCSDO.FlatListSelectedAttribute
      RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TZADCSDO.FlatListSelectedAttribute.AttributeName != ""
         if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "AttributeName", "" ) != 0 )
         { 
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AD_MappingAttribute", zPOS_AFTER );
            //:TZWINDOWL.AD_MappingAttribute.EntityName    = TZADCSDO.FlatListSelectedAttribute.EntityName
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
            //:TZWINDOWL.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" );
            //:TZWINDOWL.AD_MappingAttribute.ControlType   = TZADCSDO.FlatListSelectedAttribute.ControlType
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "ControlType", TZADCSDO, "FlatListSelectedAttribute", "ControlType" );
            //:TZWINDOWL.AD_MappingAttribute.DataWidth     = TZADCSDO.FlatListSelectedAttribute.DataWidth
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "DataWidth", TZADCSDO, "FlatListSelectedAttribute", "DataWidth" );
            //:TZWINDOWL.AD_MappingAttribute.PromptValue   = TZADCSDO.FlatListSelectedAttribute.PromptValue
            SetAttributeFromAttribute( TZWINDOWL, "AD_MappingAttribute", "PromptValue", TZADCSDO, "FlatListSelectedAttribute", "PromptValue" );
         } 

         RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
         //:END
      } 

      //:END
   } 

   //:END

   //:// Build the grid or group of Text, Editbox, Checkbox, Calendar or Combobox controls depending
   //:// on the value of GenerateGroupType.
   //:GET VIEW SelectedLOD NAMED "AutodesignSelectedLOD"
   RESULT = GetViewByName( &SelectedLOD, "AutodesignSelectedLOD", ViewToWindow, zLEVEL_TASK );
   //:AcceptSubobject( TZCONTROL, "Control" )
   AcceptSubobject( TZCONTROL, "Control" );
   //:IF TZWINDOWL.AutoDesignGroup.GenerateGroupType = "G"
   if ( CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "GenerateGroupType", "G" ) == 0 )
   { 
      //:// Go to build a Grid control for update based on the specifications of FlatListSelectedAttribute.
      //:AutodesignGridCtrl( TZWINDOWL, TZCONTROL, AD_Base, SelectedLOD )
      o_AutodesignGridCtrl( TZWINDOWL, TZCONTROL, AD_Base, SelectedLOD );
      //:szTag = TZWINDOWL.Window.Tag
      GetVariableFromAttribute( szTag, 0, 'S', 33, TZWINDOWL, "Window", "Tag", "", 0 );
      //:fnPainterCall( 8, ViewToWindow, 0, szTag )
      fnPainterCall( 8, ViewToWindow, 0, szTag, zsizeof( szTag ) );
      //:ELSE
   } 
   else
   { 
      //:// Go to build a group of regular controls based on the specifications of FlatListSelectedAttribute.
      //:AutodesignUpdateCtrls( TZWINDOWL, TZCONTROL, AD_Base, SelectedLOD )
      o_AutodesignUpdateCtrls( TZWINDOWL, TZCONTROL, AD_Base, SelectedLOD );
      //:szTag = TZWINDOWL.Window.Tag
      GetVariableFromAttribute( szTag, 0, 'S', 33, TZWINDOWL, "Window", "Tag", "", 0 );
      //:fnPainterCall( 8, ViewToWindow, 0, szTag )
      fnPainterCall( 8, ViewToWindow, 0, szTag, zsizeof( szTag ) );
   } 

   //:END
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
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD  BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
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
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 255 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zSHORT    lTempInteger_7; 
   zSHORT    lTempInteger_8; 
   zSHORT    lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zLONG     lTempInteger_11; 
   zLONG     lTempInteger_12; 
   zLONG     lTempInteger_13; 
   zLONG     lTempInteger_14; 
   zLONG     lTempInteger_15; 

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

   //:// Make sure Group Type is only ListGroupOnly or DetailGroupUpdate.
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:TraceLineS( "*** Group Type: ", szGroupType )
   TraceLineS( "*** Group Type: ", szGroupType );
   //:IF szGroupType != "ListGroupOnly" AND szGroupType != "DetailGroupUpdate"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) != 0 && ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupUpdate", 1, 0, 51 ) != 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "A Group Type of List Group Only or Detail Group Update must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "A Group Type of List Group Only or Detail Group Update must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure a template is selected.
   //:IF TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName = ""
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName", "" ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
      //:             "A Template must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Window Group", "A Template must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
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

   //:IF szGroupType = "ListGroupOnly" 
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 )
   { 

      //:// Generate a ListBox Group.

      //:// Since the GroupType is for a listbox, make sure a LOD_Entity is selected.
      //:IF TZADWWKO.ESG_ListLOD_Entity DOES NOT EXIST
      lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListLOD_Entity" );
      if ( lTempInteger_1 != 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
         //:             "A ListBox Entity must be selected if the Group Type is for any kind of list box.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Window Group", "A ListBox Entity must be selected if the Group Type is for any kind of list box.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// The next action depends on whether this is an update to a Grid or a new Grid.
      //:IF TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" ) == 0 )
      { 
         //:// Grid is Updated.
         //:// Delete all Subcontrols except the first.
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )    // Set view to Grid
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )    // Set view to Grid subcontrol
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:Count = 0
         Count = 0;
         //:FOR EACH TZCONTROL.Control 
         RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:Count = Count + 1
            Count = Count + 1;
            //:IF Count > 1 
            if ( Count > 1 )
            { 
               //:IF TZCONTROL.ControlDef.Tag != "PushBtn" AND TZCONTROL.ControlDef.Tag != "BitmapBtn"
               if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "PushBtn" ) != 0 && CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "BitmapBtn" ) != 0 )
               { 
                  //:// Delete all other Controls except for buttons, etc.
                  //:DELETE ENTITY TZCONTROL.Control NONE 
                  RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
               } 

               //:END
            } 

            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            //:END
         } 

         //:END
         //:ResetViewFromSubobject( TZCONTROL )
         ResetViewFromSubobject( TZCONTROL );
         //:ResetViewFromSubobject( TZCONTROL )
         ResetViewFromSubobject( TZCONTROL );
         //:ELSE
      } 
      else
      { 
         //:// Grid is New.
         //:// Copy the selected List Group and position on the Grid Control.
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ListBoxGroupTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ListBoxGroupTemplates", "" );
         //:SET CURSOR FIRST AD_Base.Control WHERE AD_Base.Control.Tag = TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName 
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName" );
         RESULT = SetCursorFirstEntityByString( AD_Base, "Control", "Tag", szTempString_1, "" );
         //:// Step down a level into the GroupBox for both source and target dialogs.
         //:// Then copy over any subcontrols.
         //:// Make sure that mapping is removed from each AD_BaseCtl.Control as the current mapping is not correct
         //:// and will produce errors in the clone.
         //:CreateViewFromView( AD_BaseCtl, AD_Base )
         CreateViewFromView( &AD_BaseCtl, AD_Base );
         //:NAME VIEW AD_BaseCtl "AD_BaseCtl"
         SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );
         //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
         SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:     
         //:// Remove current mapping in AD_BaseCtl.
         //:IF AD_BaseCtl.CtrlMapLOD_Entity EXISTS
         lTempInteger_2 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlMapLOD_Entity" );
         if ( lTempInteger_2 == 0 )
         { 
            //:EXCLUDE AD_BaseCtl.CtrlMapLOD_Entity 
            RESULT = ExcludeEntity( AD_BaseCtl, "CtrlMapLOD_Entity", zREPOS_AFTER );
            //:EXCLUDE AD_BaseCtl.CtrlMapView   
            RESULT = ExcludeEntity( AD_BaseCtl, "CtrlMapView", zREPOS_AFTER );
         } 

         //:END
         //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
         SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
         //:FOR EACH AD_BaseCtl.Control 
         RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:IF AD_BaseCtl.CtrlMapLOD_Attribute EXISTS
            lTempInteger_3 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlMapLOD_Attribute" );
            if ( lTempInteger_3 == 0 )
            { 
               //:EXCLUDE AD_BaseCtl.CtrlMapLOD_Attribute 
               RESULT = ExcludeEntity( AD_BaseCtl, "CtrlMapLOD_Attribute", zREPOS_AFTER );
               //:EXCLUDE AD_BaseCtl.CtrlMapView
               RESULT = ExcludeEntity( AD_BaseCtl, "CtrlMapView", zREPOS_AFTER );
            } 

            RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
            //:END
         } 

         //:END
         //:ResetViewFromSubobject( AD_BaseCtl )
         ResetViewFromSubobject( AD_BaseCtl );
         //: 
         //:FOR EACH AD_BaseCtl.Control 
         RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOW, AD_BaseCtl )
            nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOW, AD_BaseCtl );
            RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
         } 

         //:END
         //:ResetViewFromSubobject( TZCONTROL )
         ResetViewFromSubobject( TZCONTROL );
         //:DropView( AD_BaseCtl )
         DropView( AD_BaseCtl );
      } 

      //:END

      //:// Build the Grid mapping entries.
      //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
      GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:AddAD_ListMapping( TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, "ESG_List" )    
      o_AddAD_ListMapping( TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, "ESG_List" );

      //:ELSE   
   } 
   else
   { 

      //:// Generate a Detail Group.

      //:IF TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" ) == 0 )
      { 

         //:// Autodesign existing Group Controls
         //:// Since this is updating an existing Group with Controls, delete all Controls that don't have an Action.
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:FOR EACH TZCONTROL.Control 
         RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:IF TZCONTROL.EventAct DOES NOT EXIST
            lTempInteger_4 = CheckExistenceOfEntity( TZCONTROL, "EventAct" );
            if ( lTempInteger_4 != 0 )
            { 
               //:DELETE ENTITY TZCONTROL.Control NONE 
               RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
            } 

            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            //:END 
         } 

         //:END
         //:nRC = ResetViewFromSubobject( TZCONTROL )
         nRC = ResetViewFromSubobject( TZCONTROL );
      } 


      //:END

      //:// Override Prompt and Data Widths if specified.
      //:IF TZADWWKO.AutoDesignWork.UpdateGroupPromptWidth != ""
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "UpdateGroupPromptWidth", "" ) != 0 )
      { 
         //:OverrideWidth = TZADWWKO.AutoDesignWork.UpdateGroupPromptWidth
         GetIntegerFromAttribute( &OverrideWidth, TZADWWKO, "AutoDesignWork", "UpdateGroupPromptWidth" );
         //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
         RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:TZADWWKO.ESG_ListLOD_Attribute.PromptLength = OverrideWidth
            SetAttributeFromInteger( TZADWWKO, "ESG_ListLOD_Attribute", "PromptLength", OverrideWidth );
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
         } 

         //:END
      } 

      //:END
      //:IF TZADWWKO.AutoDesignWork.UpdateGroupDataWidth != ""
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "UpdateGroupDataWidth", "" ) != 0 )
      { 
         //:OverrideWidth = TZADWWKO.AutoDesignWork.UpdateGroupDataWidth
         GetIntegerFromAttribute( &OverrideWidth, TZADWWKO, "AutoDesignWork", "UpdateGroupDataWidth" );
         //:ELSE
      } 
      else
      { 
         //:OverrideWidth = 0
         OverrideWidth = 0;
      } 

      //:END
      //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF OverrideWidth > 0
         if ( OverrideWidth > 0 )
         { 
            //:TZADWWKO.ESG_ListLOD_Attribute.DataWidth = OverrideWidth
            SetAttributeFromInteger( TZADWWKO, "ESG_ListLOD_Attribute", "DataWidth", OverrideWidth );
            //:ELSE
         } 
         else
         { 
            //:TZADWWKO.ESG_ListLOD_Attribute.DataWidth = ""
            SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "DataWidth", "" );
         } 

         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
         //:END
      } 

      //:END

      //:// Autodesign new Group Controls

      //:// Copy the selected Template Group and position on the last GroupBOX Control.
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "UpdateGroupTemplates"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "UpdateGroupTemplates", "" );
      //:SET CURSOR FIRST AD_Base.Control WHERE AD_Base.Control.Tag = TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName 
      GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName" );
      RESULT = SetCursorFirstEntityByString( AD_Base, "Control", "Tag", szTempString_1, "" );
      //:GroupSizeX = AD_Base.Control.SZDLG_X    // We'll set the initial Group width from the template group.
      GetIntegerFromAttribute( &GroupSizeX, AD_Base, "Control", "SZDLG_X" );
      //:// Step down a level into the GroupBox for both source and target dialogs.
      //:// Then copy over any subcontrols.
      //:CreateViewFromView( AD_BaseCtl, AD_Base )
      CreateViewFromView( &AD_BaseCtl, AD_Base );
      //:NAME VIEW AD_BaseCtl "AD_BaseCtl"
      SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );
      //:IF AD_BaseCtl.CtrlCtrl EXISTS
      lTempInteger_5 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
      if ( lTempInteger_5 == 0 )
      { 
         //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
         SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
         //:IF AD_BaseCtl.CtrlCtrl EXISTS
         lTempInteger_6 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
         if ( lTempInteger_6 == 0 )
         { 
            //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
            SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
            //:IF AD_BaseCtl.CtrlCtrl EXISTS
            lTempInteger_7 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
            if ( lTempInteger_7 == 0 )
            { 
               //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
               SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:// Clone each Control, but delete any Text or EditBox Control as they would be for defining CSS_Class values.
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:FOR EACH AD_BaseCtl.Control 
      RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOW, AD_BaseCtl )
         nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOW, AD_BaseCtl );
         //:IF TZCONTROL.ControlDef.Tag = "Text" 
         if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "Text" ) == 0 )
         { 
            //:DELETE ENTITY TZCONTROL.Control NONE 
            RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
            //:ELSE
         } 
         else
         { 
            //:IF TZCONTROL.ControlDef.Tag = "EditBox" 
            if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "EditBox" ) == 0 )
            { 
               //:DELETE ENTITY TZCONTROL.Control NONE 
               RESULT = DeleteEntity( TZCONTROL, "Control", zREPOS_NONE );
            } 

            //:END 
         } 

         RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( TZCONTROL )   // Go back up to the Group Control
      ResetViewFromSubobject( TZCONTROL );
      //:DropView( AD_BaseCtl )
      DropView( AD_BaseCtl );

      //:// Generate a Display/Update Group.
      //:SET CURSOR FIRST AD_Base.Control WITHIN AD_Base.Dialog 
      //:           WHERE AD_Base.Window.Tag = "ObjectUpdateGroups"
      //:             AND AD_Base.Control.Tag = "GroupDetail"
      RESULT = SetCursorFirstEntity( AD_Base, "Control", "Dialog" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( AD_Base, "Window", "Tag", "ObjectUpdateGroups" ) != 0 || CompareAttributeToString( AD_Base, "Control", "Tag", "GroupDetail" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( AD_Base, "Control", "Dialog" );
         } 

      } 

      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "DetailGroupUpdate Control doesn't exist.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "DetailGroupUpdate Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Build the AutodesignGroup subobject in TZWINDOW so that we can reuse the AutodesignUpdateCtrls code.
      //:IF TZWINDOW.AutoDesignWindow EXISTS
      lTempInteger_8 = CheckExistenceOfEntity( TZWINDOW, "AutoDesignWindow" );
      if ( lTempInteger_8 == 0 )
      { 
         //:DELETE ENTITY TZWINDOW.AutoDesignWindow  
         RESULT = DeleteEntity( TZWINDOW, "AutoDesignWindow", zPOS_NEXT );
      } 

      //:END
      //:CREATE ENTITY TZWINDOW.AutoDesignWindow 
      RESULT = CreateEntity( TZWINDOW, "AutoDesignWindow", zPOS_AFTER );
      //:TZWINDOW.AutoDesignWindow.WindowTag              = TZWINDOWL.Window.Tag  
      SetAttributeFromAttribute( TZWINDOW, "AutoDesignWindow", "WindowTag", TZWINDOWL, "Window", "Tag" );
      //:CREATE ENTITY TZWINDOW.AutoDesignGroup
      RESULT = CreateEntity( TZWINDOW, "AutoDesignGroup", zPOS_AFTER );
      //:TZWINDOW.AutoDesignGroup.GroupTag                = TZADWWKO.EntitySubGroup.GroupDisplayArea  
      SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "GroupTag", TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
      //:TZWINDOW.AutoDesignGroup.Title                   = ""    // Title needs to be null or a subgroup is generated to hold title.
      SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "Title", "" );
      //:TZWINDOW.AutoDesignGroup.TempControlType         = ""
      SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "TempControlType", "" );
      //:    
      //:TZWINDOW.AutoDesignGroup.BaseWindowName          = "UpdateFields"
      SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "BaseWindowName", "UpdateFields" );
      //:TZWINDOW.AutoDesignGroup.ActionNameSuffix        = TZADWWKO.AutodesignSubdialog.AD_AreaSuffix 
      SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ActionNameSuffix", TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix" );
      //:TZWINDOW.AutoDesignGroup.ControlWidthInPixels    = GroupSizeX    // Use the current Group width.
      SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels", GroupSizeX );
      //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = 15
      SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", 15 );
      //:TZWINDOW.AutoDesignGroup.GenerateGroupType    = ""
      SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "GenerateGroupType", "" );

      //:// Build the AD_ListBoxEntity subobject which is what is used to generate the controls.
      //:IF TZWINDOW.AD_ListBoxEntity EXISTS
      lTempInteger_9 = CheckExistenceOfEntity( TZWINDOW, "AD_ListBoxEntity" );
      if ( lTempInteger_9 == 0 )
      { 
         //:DELETE ENTITY TZWINDOW.AD_ListBoxEntity
         RESULT = DeleteEntity( TZWINDOW, "AD_ListBoxEntity", zPOS_NEXT );
      } 

      //:END
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER );
      //://TZWINDOW.AD_ListBoxEntity.EntityName = TZADWWKO.ESG_LOD_Entity.Name 

      //:// AutoDesign Group is for regular controls on a Groupbox.
      //:FOR EACH TZADWWKO.ESG_ListLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER );
         //:TZWINDOW.AD_MappingAttribute.EntityName          = TZADWWKO.ESG_ListLOD_Entity.Name 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "EntityName", TZADWWKO, "ESG_ListLOD_Entity", "Name" );
         //:TZWINDOW.AD_MappingAttribute.AttributeName       = TZADWWKO.ESG_ListER_Attribute.Name 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "AttributeName", TZADWWKO, "ESG_ListER_Attribute", "Name" );
         //:TZWINDOW.AD_MappingAttribute.DataWidth           = TZADWWKO.ESG_ListLOD_Attribute.DataWidth 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "DataWidth", TZADWWKO, "ESG_ListLOD_Attribute", "DataWidth" );
         //:TZWINDOW.AD_MappingAttribute.PromptValue         = TZADWWKO.ESG_ListLOD_Attribute.PromptValue 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "PromptValue", TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue" );
         //:TZWINDOW.AD_MappingAttribute.ControlType         = TZADWWKO.ESG_ListLOD_Attribute.ControlType 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "ControlType", TZADWWKO, "ESG_ListLOD_Attribute", "ControlType" );
         //:TZWINDOW.AD_MappingAttribute.CSS_ClassForPrompt  = TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForPrompt 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "CSS_ClassForPrompt", TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForPrompt" );
         //:TZWINDOW.AD_MappingAttribute.CSS_ClassForControl = TZADWWKO.ESG_ListLOD_Attribute.CSS_ClassForControl 
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "CSS_ClassForControl", TZADWWKO, "ESG_ListLOD_Attribute", "CSS_ClassForControl" );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
      } 

      //:END

      //:// We need to make sure that we're positioned on the Group Control. 
      //:AutodesignUpdateCtrls( TZWINDOW, TZCONTROL, AD_Base, UpdateLOD )
      o_AutodesignUpdateCtrls( TZWINDOW, TZCONTROL, AD_Base, UpdateLOD );
   } 

   //:      
   //:END

   //:// Make sure we're on the top Group Box and Accept it.
   //:nRC = ResetViewFromSubobject( TZCONTROL )
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:nRC = ResetViewFromSubobject( TZCONTROL )
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:nRC = ResetViewFromSubobject( TZCONTROL )
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:AcceptSubobject( TZCONTROL, "Control" )
   AcceptSubobject( TZCONTROL, "Control" );

   //:// Set the Area as Text in outer GroupBox.
   //:TZCONTROL.Control.Text = TZADWWKO.EntitySubGroup.GroupDisplayArea 
   SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );

   //:// Compute size of GroupBox.
   //:SET CURSOR LAST TZCONTROL.CtrlCtrl  
   RESULT = SetCursorLastEntity( TZCONTROL, "CtrlCtrl", "" );
   //:GroupSizeX = TZCONTROL.CtrlCtrl.PSDLG_X + TZCONTROL.CtrlCtrl.SZDLG_X + 12
   GetIntegerFromAttribute( &lTempInteger_10, TZCONTROL, "CtrlCtrl", "PSDLG_X" );
   GetIntegerFromAttribute( &lTempInteger_11, TZCONTROL, "CtrlCtrl", "SZDLG_X" );
   GroupSizeX = lTempInteger_10 + lTempInteger_11 + 12;
   //:GroupSizeY = TZCONTROL.CtrlCtrl.PSDLG_Y + TZCONTROL.CtrlCtrl.SZDLG_Y + 12
   GetIntegerFromAttribute( &lTempInteger_12, TZCONTROL, "CtrlCtrl", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_13, TZCONTROL, "CtrlCtrl", "SZDLG_Y" );
   GroupSizeY = lTempInteger_12 + lTempInteger_13 + 12;
   //:TZCONTROL.Control.SZDLG_X = GroupSizeX
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", GroupSizeX );
   //:TZCONTROL.Control.SZDLG_Y = GroupSizeY
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", GroupSizeY );

   //:// If this is not for a Grid and Reposition is selected, readjust the position of each GroupBox based on the size and position of the previous GroupBox.
   //:IF szGroupType != "ListGroupOnly" AND TZADWWKO.AutoDesignWork.RepositionGroupFlag = "Y"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) != 0 && CompareAttributeToString( TZADWWKO, "AutoDesignWork", "RepositionGroupFlag", "Y" ) == 0 )
   { 
      //:nNextPositionY = 0
      nNextPositionY = 0;
      //:SET CURSOR FIRST TZWINDOWL.Control 
      RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
      //:FOR EACH TZWINDOWL.Control 
      RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:TZWINDOWL.Control.PSDLG_Y = nNextPositionY
         SetAttributeFromInteger( TZWINDOWL, "Control", "PSDLG_Y", nNextPositionY );
         //:nNextPositionY = TZWINDOWL.Control.PSDLG_Y + TZWINDOWL.Control.SZDLG_Y + 10
         GetIntegerFromAttribute( &lTempInteger_14, TZWINDOWL, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lTempInteger_15, TZWINDOWL, "Control", "SZDLG_Y" );
         nNextPositionY = lTempInteger_14 + lTempInteger_15 + 10;
         RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
      } 

      //:END 
   } 

   //:END

   //:// Drop object TZADWWKO so it will be created new next time.
   //:DropObjectInstance( TZADWWKO )
   DropObjectInstance( TZADWWKO );

   //:// Tell the Painter to refresh the control.
   //:RepaintWindowControls( ViewToWindow )
   RepaintWindowControls( ViewToWindow );
   return( 0 );
//    
// END
} 


//:LOCAL OPERATION
//:AutodesignGridCtrl( VIEW TZWINDOWL   BASED ON LOD TZWDLGSO,
//:                    VIEW TZCONTROL   BASED ON LOD TZWDLGSO,
//:                    VIEW AD_Base     BASED ON LOD TZWDLGSO,
//:                    VIEW SelectedLOD BASED ON LOD TZZOLODO )

//:   VIEW TZPESRCO    BASED ON LOD TZPESRCO
static zSHORT
o_AutodesignGridCtrl( zVIEW     TZWINDOWL,
                      zVIEW     TZCONTROL,
                      zVIEW     AD_Base,
                      zVIEW     SelectedLOD )
{
   zVIEW     TZPESRCO = 0; 
   //:VIEW AD_BaseRoot BASED ON LOD TZWDLGSO
   zVIEW     AD_BaseRoot = 0; 
   //:INTEGER lGridWidth
   zLONG     lGridWidth = 0; 
   //:INTEGER lGridWidthAvailable
   zLONG     lGridWidthAvailable = 0; 
   //:INTEGER lControlWidthChars
   zLONG     lControlWidthChars = 0; 
   //:INTEGER lControlWidth
   zLONG     lControlWidth = 0; 
   //:INTEGER lControlPosition
   zLONG     lControlPosition = 0; 
   //:INTEGER lCurrentPositionY
   zLONG     lCurrentPositionY = 0; 
   //:INTEGER lTotalDataWidth
   zLONG     lTotalDataWidth = 0; 
   //:INTEGER lAveragPixelWidth
   zLONG     lAveragPixelWidth = 0; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER lAttributeCount
   zLONG     lAttributeCount = 0; 
   //:STRING ( 100 ) szAttributeName
   zCHAR     szAttributeName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szControlDefName
   zCHAR     szControlDefName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szControlBaseName
   zCHAR     szControlBaseName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szActionSuffix
   zCHAR     szActionSuffix[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szActionName
   zCHAR     szActionName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szFirstControlTag
   zCHAR     szFirstControlTag[ 101 ] = { 0 }; 
   //:STRING ( 1 )   szControlNameSuffixGroup
   zCHAR     szControlNameSuffixGroup[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szControlNameSuffixAttribute
   zCHAR     szControlNameSuffixAttribute[ 2 ] = { 0 }; 
   //:STRING ( 10 )  szGroupName
   zCHAR     szGroupName[ 11 ] = { 0 }; 
   //:STRING ( 8 )   szGroupName8
   zCHAR     szGroupName8[ 9 ] = { 0 }; 
   //:STRING ( 20 )  szControlType
   zCHAR     szControlType[ 21 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_4[ 255 ]; 
   zLONG     lTempInteger_4; 
   zCHAR     szTempString_5[ 255 ]; 
   zSHORT    lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zCHAR     szTempString_6[ 255 ]; 
   zSHORT    lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zCHAR     szTempString_7[ 33 ]; 
   zSHORT    lTempInteger_11; 


   //:// Build a Grid control, with a subentity for each AD_MappingAttribute entry.

   //:GET VIEW TZPESRCO NAMED "TZPESRCO"
   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZWINDOWL, zLEVEL_TASK );
   //:szActionSuffix = TZWINDOWL.AutoDesignGroup.ActionNameSuffix
   GetVariableFromAttribute( szActionSuffix, 0, 'S', 101, TZWINDOWL, "AutoDesignGroup", "ActionNameSuffix", "", 0 );
   //:CreateViewFromView( AD_BaseRoot, AD_Base )
   CreateViewFromView( &AD_BaseRoot, AD_Base );
   //:NAME VIEW AD_BaseRoot "AD_BaseRoot"
   SetNameForView( AD_BaseRoot, "AD_BaseRoot", 0, zLEVEL_TASK );
   //:TZCONTROL.Control.Text = ""            // We don't want any text in the highest level Group.
   SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

   //:// To generate unique Control Names when multiple Groups are autodesigned for the same Window, we
   //:// will try to use the 9th character of the Group Name, as it should be unique with groups named like
   //:// "GroupBox2".
   //:szGroupName = TZCONTROL.Control.Tag
   GetVariableFromAttribute( szGroupName, 0, 'S', 11, TZCONTROL, "Control", "Tag", "", 0 );
   //:szGroupName8 = szGroupName[1:8]
   ZeidonStringCopy( szGroupName8, 1, 0, szGroupName, 1, 8, 9 );
   //:IF szGroupName8 = "GroupBox"
   if ( ZeidonStringCompare( szGroupName8, 1, 0, "GroupBox", 1, 0, 9 ) == 0 )
   { 
      //:szControlNameSuffixGroup = szGroupName[9:1]
      ZeidonStringCopy( szControlNameSuffixGroup, 1, 0, szGroupName, 9, 1, 2 );
   } 

   //:END

   //:// If there is a Title for the Group, add the control set.
   //:IF TZWINDOWL.AutoDesignGroup.Title != ""
   if ( CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "Title", "" ) != 0 )
   { 

      //:// Position on Title Base Control and step down into it.
      //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "TitleGroup"
      RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "TitleGroup", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( TZWINDOWL, "", "Autodesign Window Group",
         //:             "The Base group doesn't have a TitleGroup Control defined.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZWINDOWL, "", "Autodesign Window Group", "The Base group doesn't have a TitleGroup Control defined.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
      SetViewToSubobject( AD_Base, "CtrlCtrl" );

      //:// Create Group to hold text and optional Add/Select button.
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GroupBox"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GroupBox", "" );
      //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Tag  = "TitleGroup" + TZWINDOWL.AD_ListBoxEntity.EntityName + szControlNameSuffixGroup
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TZWINDOWL, "AD_ListBoxEntity", "EntityName", "", 0 );
      ZeidonStringCopy( szTempString_0, 1, 0, "TitleGroup", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szControlNameSuffixGroup, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );
      //:SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL )
      SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL );
      //:TZCONTROL.Control.SZDLG_X = TZWINDOWL.AutoDesignGroup.ControlWidthInPixels - 10
      GetIntegerFromAttribute( &lTempInteger_0, TZWINDOWL, "AutoDesignGroup", "ControlWidthInPixels" );
      lTempInteger_1 = lTempInteger_0 - 10;
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lTempInteger_1 );
      //:FOR EACH AD_Base.WebControlProperty
      RESULT = SetCursorFirstEntity( AD_Base, "WebControlProperty", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "WebControlProperty", zPOS_AFTER )
         CreateMetaEntity( TZWINDOWL, TZCONTROL, "WebControlProperty", zPOS_AFTER );
         //:TZCONTROL.WebControlProperty.Name = AD_Base.WebControlProperty.Name
         SetAttributeFromAttribute( TZCONTROL, "WebControlProperty", "Name", AD_Base, "WebControlProperty", "Name" );
         RESULT = SetCursorNextEntity( AD_Base, "WebControlProperty", "" );
      } 

      //:END

      //:// Step down to Title Group subcontrols (Title and possibly Add/Select button) and add each.
      //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
      SetViewToSubobject( AD_Base, "CtrlCtrl" );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:OrderEntityForView( AD_Base, "Control", "PSDLG_X A" )
      OrderEntityForView( AD_Base, "Control", "PSDLG_X A" );
      //:FOR EACH AD_Base.Control
      RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// Set basic Control values.
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = AD_Base.ControlDef.Tag
         GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), AD_Base, "ControlDef", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_2, "" );
         //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
         CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
         //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
         //:szControlBaseName = AD_Base.Control.Tag
         GetVariableFromAttribute( szControlBaseName, 0, 'S', 101, AD_Base, "Control", "Tag", "", 0 );
         //:TZCONTROL.Control.Tag = szControlBaseName + szActionSuffix + szControlNameSuffixGroup
         ZeidonStringCopy( szTempString_2, 1, 0, szControlBaseName, 1, 0, 33 );
         ZeidonStringConcat( szTempString_2, 1, 0, szActionSuffix, 1, 0, 33 );
         ZeidonStringConcat( szTempString_2, 1, 0, szControlNameSuffixGroup, 1, 0, 33 );
         SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_2 );
         //:SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL )
         SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL );

         //:// The Title Control gets the Title text.
         //:IF AD_Base.Control.Tag = "Title"
         if ( CompareAttributeToString( AD_Base, "Control", "Tag", "Title" ) == 0 )
         { 
            //:TZCONTROL.Control.Text = TZWINDOWL.AutoDesignGroup.Title
            SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZWINDOWL, "AutoDesignGroup", "Title" );
         } 

         //:END

         //:// If this Control has an Action, add it (unless it already exists) and include it under the Control.
         //:IF AD_Base.EventAct EXISTS
         lTempInteger_2 = CheckExistenceOfEntity( AD_Base, "EventAct" );
         if ( lTempInteger_2 == 0 )
         { 
            //:SET CURSOR FIRST AD_BaseRoot.Action WHERE AD_BaseRoot.Action.Tag = AD_Base.EventAct.Tag
            GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), AD_Base, "EventAct", "Tag" );
            RESULT = SetCursorFirstEntityByString( AD_BaseRoot, "Action", "Tag", szTempString_3, "" );
            //:szActionName = AD_BaseRoot.Action.Tag + szActionSuffix
            GetStringFromAttribute( szActionName, zsizeof( szActionName ), AD_BaseRoot, "Action", "Tag" );
            ZeidonStringConcat( szActionName, 1, 0, szActionSuffix, 1, 0, 101 );
            //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = szActionName
            RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szActionName, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:// Add the Action.
               //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Action", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Action", zPOS_AFTER );
               //:TZWINDOWL.Action.Tag = szActionName
               SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
               //:TZWINDOWL.Action.Type = AD_BaseRoot.Action.Type
               SetAttributeFromAttribute( TZWINDOWL, "Action", "Type", AD_BaseRoot, "Action", "Type" );
               //:// Add Operation, if necessary.
               //:IF AD_BaseRoot.ActOper EXISTS
               lTempInteger_3 = CheckExistenceOfEntity( AD_BaseRoot, "ActOper" );
               if ( lTempInteger_3 == 0 )
               { 
                  //:SET CURSOR FIRST TZWINDOWL.OperationList WHERE TZWINDOWL.OperationList.Name = szActionName
                  RESULT = SetCursorFirstEntityByString( TZWINDOWL, "OperationList", "Name", szActionName, "" );
                  //:IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //:SET CURSOR FIRST AD_BaseRoot.Operation WHERE AD_BaseRoot.Operation.Name = szActionName
                     RESULT = SetCursorFirstEntityByString( AD_BaseRoot, "Operation", "Name", szActionName, "" );
                     //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
                     CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
                     //:TZWINDOWL.Operation.Name = szActionName
                     SetAttributeFromString( TZWINDOWL, "Operation", "Name", szActionName );
                     //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_BaseRoot, "Operation", zSET_NULL )
                     SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_BaseRoot, "Operation", zSET_NULL );
                     //:FOR EACH AD_BaseRoot.Parameter
                     RESULT = SetCursorFirstEntity( AD_BaseRoot, "Parameter", "" );
                     while ( RESULT > zCURSOR_UNCHANGED )
                     { 
                        //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
                        CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
                        //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_BaseRoot, "Parameter", zSET_NULL )
                        SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_BaseRoot, "Parameter", zSET_NULL );
                        RESULT = SetCursorNextEntity( AD_BaseRoot, "Parameter", "" );
                     } 

                     //:END
                     //:INCLUDE TZWINDOWL.OperationList FROM TZWINDOWL.Operation
                     RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "OperationList", TZWINDOWL, "Operation", zPOS_AFTER );
                  } 

                  //:END
                  //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.OperationList
                  RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "OperationList", zPOS_AFTER );
               } 

               //:END
            } 

            //:END
            //:CreateMetaEntity( TZCONTROL, TZCONTROL, "Event", zPOS_AFTER )
            CreateMetaEntity( TZCONTROL, TZCONTROL, "Event", zPOS_AFTER );
            //:TZCONTROL.Event.Type = AD_Base.Event.Type
            SetAttributeFromAttribute( TZCONTROL, "Event", "Type", AD_Base, "Event", "Type" );
            //:INCLUDE TZCONTROL.EventAct FROM TZWINDOWL.Action
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "EventAct", TZWINDOWL, "Action", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( AD_Base, "Control", "" );
         //:END
      } 


      //:END

      //:// Position back to top for both created Controls and Base.
      //:ResetViewFromSubobject( AD_Base )   // Go back to second Group.
      ResetViewFromSubobject( AD_Base );
      //:ResetViewFromSubobject( AD_Base )   // Go back to first Group.
      ResetViewFromSubobject( AD_Base );
      //:ResetViewFromSubobject( TZCONTROL )   // Go back to second Group.
      ResetViewFromSubobject( TZCONTROL );
      //:ResetViewFromSubobject( TZCONTROL )   // Go back to first Group.
      ResetViewFromSubobject( TZCONTROL );
   } 

   //:END

   //:// Position on Grid Base Control.
   //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "Grid"
   RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "Grid", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZWINDOWL, "", "Autodesign Window Group",
      //:             "The Base group doesn't have a Grid Control defined.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZWINDOWL, "", "Autodesign Window Group", "The Base group doesn't have a Grid Control defined.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Build base control for Grid. Width is 20 less than the original Control.
   //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Grid"
   RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Grid", "" );
   //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER )
   CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER );
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
   //:TZCONTROL.Control.Tag     = "Grid" + TZWINDOWL.AD_ListBoxEntity.EntityName + szControlNameSuffixGroup
   GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TZWINDOWL, "AD_ListBoxEntity", "EntityName", "", 0 );
   ZeidonStringCopy( szTempString_3, 1, 0, "Grid", 1, 0, 33 );
   ZeidonStringConcat( szTempString_3, 1, 0, szTempString_4, 1, 0, 33 );
   ZeidonStringConcat( szTempString_3, 1, 0, szControlNameSuffixGroup, 1, 0, 33 );
   SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_3 );
   //:TZCONTROL.Control.SyncKey = 9999
   SetAttributeFromInteger( TZCONTROL, "Control", "SyncKey", 9999 );

   //:// Position on Grid Base Control and step down into it.
   //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "Grid"
   RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "Grid", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZWINDOWL, "", "Autodesign Window Group",
      //:             "The Base group doesn't have a Title Control defined.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZWINDOWL, "", "Autodesign Window Group", "The Base group doesn't have a Title Control defined.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
   SetViewToSubobject( AD_Base, "CtrlCtrl" );

   //:// All values, except Width will be set from Base.
   //:// Grid width will be slightly smaller that the group on which it is positioned.
   //:lGridWidth = TZWINDOWL.AutoDesignGroup.ControlWidthInPixels - 10
   GetIntegerFromAttribute( &lTempInteger_4, TZWINDOWL, "AutoDesignGroup", "ControlWidthInPixels" );
   lGridWidth = lTempInteger_4 - 10;
   //:TZCONTROL.Control.SZDLG_X = lGridWidth
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lGridWidth );
   //:SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL )
   SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL );

   //:// Build CtrlMap subobject for list entity, which is first entity in FlatListSelectedAttribute.
   //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
   CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
   //:SET CURSOR FIRST SelectedLOD.LOD_Entity WHERE SelectedLOD.LOD_Entity.Name = TZWINDOWL.AutoDesignGroup.TopLevelEntityName
   GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), TZWINDOWL, "AutoDesignGroup", "TopLevelEntityName" );
   RESULT = SetCursorFirstEntityByString( SelectedLOD, "LOD_Entity", "Name", szTempString_5, "" );
   //:INCLUDE TZCONTROL.CtrlMapLOD_Entity FROM SelectedLOD.LOD_Entity
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Entity", SelectedLOD, "LOD_Entity", zPOS_AFTER );
   //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
   //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )   // Create necessary scoping CtrlMap, though empty.
   CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );

   //:// Position and Size of each control on the Grid will be determined from relative widths of attributes making up the Grid.
   //:// First account for any Action controls, for they will be specified as their width in AD_Base. The width available for
   //:// variable controls will be the Grid width minus the total of Action control widths, except for the first, which represents
   //:// the first mapping attribute.
   //:// Then total up variable character widths. Then determine average pixel size of each character width.
   //:lGridWidthAvailable = lGridWidth
   lGridWidthAvailable = lGridWidth;
   //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
   SetViewToSubobject( AD_Base, "CtrlCtrl" );
   //:Count = 0
   Count = 0;
   //:FOR EACH AD_Base.Control
   RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:Count = Count + 1
      Count = Count + 1;
      //:IF AD_Base.EventAct EXISTS AND Count > 1
      lTempInteger_5 = CheckExistenceOfEntity( AD_Base, "EventAct" );
      if ( lTempInteger_5 == 0 && Count > 1 )
      { 
         //:lGridWidthAvailable = lGridWidthAvailable - AD_Base.Control.SZDLG_X
         GetIntegerFromAttribute( &lTempInteger_6, AD_Base, "Control", "SZDLG_X" );
         lGridWidthAvailable = lGridWidthAvailable - lTempInteger_6;
      } 

      RESULT = SetCursorNextEntity( AD_Base, "Control", "" );
      //:END
   } 

   //:END
   //:lTotalDataWidth = 0
   lTotalDataWidth = 0;
   //:FOR EACH TZWINDOWL.AD_MappingAttribute
   RESULT = SetCursorFirstEntity( TZWINDOWL, "AD_MappingAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:lTotalDataWidth = lTotalDataWidth + TZWINDOWL.AD_MappingAttribute.DataWidth
      GetIntegerFromAttribute( &lTempInteger_7, TZWINDOWL, "AD_MappingAttribute", "DataWidth" );
      lTotalDataWidth = lTotalDataWidth + lTempInteger_7;
      RESULT = SetCursorNextEntity( TZWINDOWL, "AD_MappingAttribute", "" );
   } 

   //:END
   //:IF lTotalDataWidth = 0
   if ( lTotalDataWidth == 0 )
   { 
      //:IssueError( TZWINDOWL,0,0, "Null Total Width" )
      IssueError( TZWINDOWL, 0, 0, "Null Total Width" );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:lAveragPixelWidth = lGridWidthAvailable / lTotalDataWidth
   lAveragPixelWidth = lGridWidthAvailable / lTotalDataWidth;
   //:TraceLineI( "*** lGridWidthAvailable: ", lGridWidthAvailable )
   TraceLineI( "*** lGridWidthAvailable: ", lGridWidthAvailable );
   //:TraceLineI( "*** lGridWidth: ", lGridWidth )
   TraceLineI( "*** lGridWidth: ", lGridWidth );
   //:TraceLineI( "*** lTotalDataWidth: ", lTotalDataWidth )
   TraceLineI( "*** lTotalDataWidth: ", lTotalDataWidth );
   //:TraceLineI( "*** lAveragPixelWidth: ", lAveragPixelWidth )
   TraceLineI( "*** lAveragPixelWidth: ", lAveragPixelWidth );

   //:// Build subcontrol for each FlatListSelectedAttribute entry.
   //:lControlPosition = 0
   lControlPosition = 0;
   //:lAttributeCount = 0
   lAttributeCount = 0;
   //:OrderEntityForView( AD_Base, "Control", "PSDLG_X A" )
   OrderEntityForView( AD_Base, "Control", "PSDLG_X A" );
   //:SET CURSOR FIRST AD_Base.Control
   RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
   //:FOR EACH TZWINDOWL.AD_MappingAttribute
   RESULT = SetCursorFirstEntity( TZWINDOWL, "AD_MappingAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:szAttributeName = TZWINDOWL.AD_MappingAttribute.AttributeName
      GetVariableFromAttribute( szAttributeName, 0, 'S', 101, TZWINDOWL, "AD_MappingAttribute", "AttributeName", "", 0 );
      //:lAttributeCount = lAttributeCount + 1
      lAttributeCount = lAttributeCount + 1;
      //:szControlNameSuffixAttribute = lAttributeCount
      ZeidonStringConvertFromNumber( szControlNameSuffixAttribute, 1, 0, 1, lAttributeCount, (ZDecimal) 0.0, "I" );
      //:TZCONTROL.Control.Tag = "GridCtrl" + szAttributeName + szControlNameSuffixGroup + szControlNameSuffixAttribute
      ZeidonStringCopy( szTempString_5, 1, 0, "GridCtrl", 1, 0, 255 );
      ZeidonStringConcat( szTempString_5, 1, 0, szAttributeName, 1, 0, 255 );
      ZeidonStringConcat( szTempString_5, 1, 0, szControlNameSuffixGroup, 1, 0, 255 );
      ZeidonStringConcat( szTempString_5, 1, 0, szControlNameSuffixAttribute, 1, 0, 255 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_5 );
      //:TZCONTROL.Control.Text = TZWINDOWL.AD_MappingAttribute.PromptValue
      SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZWINDOWL, "AD_MappingAttribute", "PromptValue" );
      //:SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL )
      SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL );

      //:// Build the Editbox, Checkbox, Calendar or Combobox controls depending on ControlType.
      //:szControlType = TZWINDOWL.AD_MappingAttribute.ControlType
      GetVariableFromAttribute( szControlType, 0, 'S', 21, TZWINDOWL, "AD_MappingAttribute", "ControlType", "", 0 );
      //:IF szControlType = "CheckBox" OR szControlType = "Calendar"
      if ( ZeidonStringCompare( szControlType, 1, 0, "CheckBox", 1, 0, 21 ) == 0 || ZeidonStringCompare( szControlType, 1, 0, "Calendar", 1, 0, 21 ) == 0 )
      { 
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlType
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlType, "" );
         //:ELSE
      } 
      else
      { 
         //:IF szControlType = "ComboBox"
         if ( ZeidonStringCompare( szControlType, 1, 0, "ComboBox", 1, 0, 21 ) == 0 )
         { 
            //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GridComboCtl"
            RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GridComboCtl", "" );
            //:ELSE
         } 
         else
         { 
            //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GridEditCtl"
            RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GridEditCtl", "" );
            //:// If this is an Editbox, add Edit WebControlProperty entry.
            //:IF szControlType = "EditBox"
            if ( ZeidonStringCompare( szControlType, 1, 0, "EditBox", 1, 0, 21 ) == 0 )
            { 
               //:CreateMetaEntity( TZCONTROL, TZCONTROL, "WebControlProperty", zPOS_AFTER )
               CreateMetaEntity( TZCONTROL, TZCONTROL, "WebControlProperty", zPOS_AFTER );
               //:TZCONTROL.WebControlProperty.Name = "Edit"
               SetAttributeFromString( TZCONTROL, "WebControlProperty", "Name", "Edit" );
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );

      //:// Width of Control is Data Width multiplied by the number of pixels per character.
      //:lControlWidth = TZWINDOWL.AD_MappingAttribute.DataWidth * lAveragPixelWidth
      GetIntegerFromAttribute( &lTempInteger_8, TZWINDOWL, "AD_MappingAttribute", "DataWidth" );
      lControlWidth = lTempInteger_8 * lAveragPixelWidth;

      //:TZCONTROL.Control.PSDLG_X = lControlPosition
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", lControlPosition );
      //:TZCONTROL.Control.SZDLG_X = lControlWidth
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lControlWidth );
      //:TZCONTROL.Control.PSDLG_Y = 0
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 0 );
      //:TZCONTROL.Control.SZDLG_Y = 15
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 15 );
      //:lControlPosition = lControlPosition + lControlWidth
      lControlPosition = lControlPosition + lControlWidth;

      //:// Build CtrlMap subobject for list control entity from Entity.Attribute of FlatListSelectedAttribute
      //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:SET CURSOR FIRST SelectedLOD.LOD_Entity WHERE SelectedLOD.LOD_Entity.Name = TZWINDOWL.AD_MappingAttribute.EntityName
      GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZWINDOWL, "AD_MappingAttribute", "EntityName" );
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "LOD_Entity", "Name", szTempString_6, "" );
      //:SET CURSOR FIRST SelectedLOD.ER_Attribute WITHIN SelectedLOD.LOD_Entity
      //:           WHERE SelectedLOD.ER_Attribute.Name = TZWINDOWL.AD_MappingAttribute.AttributeName
      GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZWINDOWL, "AD_MappingAttribute", "AttributeName" );
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "ER_Attribute", "Name", szTempString_6, "LOD_Entity" );
      //:INCLUDE TZCONTROL.CtrlMapLOD_Attribute FROM SelectedLOD.LOD_Attribute
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", SelectedLOD, "LOD_Attribute", zPOS_AFTER );
      //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );

      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
      RESULT = SetCursorNextEntity( TZWINDOWL, "AD_MappingAttribute", "" );
   } 

   //:END

   //:// Add any Grid subcontrols that have Actions tied to them.
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SET CURSOR FIRST AD_Base.Control
   RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
   //:szFirstControlTag = AD_Base.Control.Tag
   GetVariableFromAttribute( szFirstControlTag, 0, 'S', 101, AD_Base, "Control", "Tag", "", 0 );
   //:FOR EACH AD_Base.Control
   RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:IF AD_Base.EventAct EXISTS
      lTempInteger_9 = CheckExistenceOfEntity( AD_Base, "EventAct" );
      if ( lTempInteger_9 == 0 )
      { 

         //:// If this is an event on the first mapping Control for the base Grid, add it to the first Control in the generated
         //:// Grid, rather than add a new Control.
         //:IF AD_Base.Control.Tag = szFirstControlTag
         if ( CompareAttributeToString( AD_Base, "Control", "Tag", szFirstControlTag ) == 0 )
         { 
            //:SET CURSOR FIRST TZCONTROL.Control
            RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
            //:ELSE
         } 
         else
         { 

            //:// Create Control and set basic Control values.
            //:szControlDefName = AD_Base.ControlDef.Tag
            GetVariableFromAttribute( szControlDefName, 0, 'S', 101, AD_Base, "ControlDef", "Tag", "", 0 );
            //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlDefName
            RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlDefName, "" );
            //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
            //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
            //:TZCONTROL.Control.PSDLG_X = lControlPosition
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", lControlPosition );
            //:szControlBaseName         = AD_Base.Control.Tag
            GetVariableFromAttribute( szControlBaseName, 0, 'S', 101, AD_Base, "Control", "Tag", "", 0 );
            //:TZCONTROL.Control.Tag     = szControlBaseName + szActionSuffix + szControlNameSuffixGroup
            ZeidonStringCopy( szTempString_6, 1, 0, szControlBaseName, 1, 0, 255 );
            ZeidonStringConcat( szTempString_6, 1, 0, szActionSuffix, 1, 0, 255 );
            ZeidonStringConcat( szTempString_6, 1, 0, szControlNameSuffixGroup, 1, 0, 255 );
            SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_6 );
            //:SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL )
            SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL );
            //:lControlPosition = lControlPosition + TZCONTROL.Control.SZDLG_X
            GetIntegerFromAttribute( &lTempInteger_10, TZCONTROL, "Control", "SZDLG_X" );
            lControlPosition = lControlPosition + lTempInteger_10;
         } 

         //:END

         //:// Add the Action (unless it already exists) and include it under the Control.
         //:SET CURSOR FIRST AD_BaseRoot.Action WHERE AD_BaseRoot.Action.Tag = AD_Base.EventAct.Tag
         GetStringFromAttribute( szTempString_7, zsizeof( szTempString_7 ), AD_Base, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( AD_BaseRoot, "Action", "Tag", szTempString_7, "" );
         //:szActionName = AD_BaseRoot.Action.Tag + szActionSuffix
         GetStringFromAttribute( szActionName, zsizeof( szActionName ), AD_BaseRoot, "Action", "Tag" );
         ZeidonStringConcat( szActionName, 1, 0, szActionSuffix, 1, 0, 101 );
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = szActionName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szActionName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// Add the Action.
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Action", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Action", zPOS_AFTER );
            //:TZWINDOWL.Action.Tag = szActionName
            SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
            //:TZWINDOWL.Action.Type = AD_BaseRoot.Action.Type
            SetAttributeFromAttribute( TZWINDOWL, "Action", "Type", AD_BaseRoot, "Action", "Type" );
            //:// Add Operation, if necessary.
            //:IF AD_BaseRoot.ActOper EXISTS
            lTempInteger_11 = CheckExistenceOfEntity( AD_BaseRoot, "ActOper" );
            if ( lTempInteger_11 == 0 )
            { 
               //:SET CURSOR FIRST TZWINDOWL.OperationList WHERE TZWINDOWL.OperationList.Name = szActionName
               RESULT = SetCursorFirstEntityByString( TZWINDOWL, "OperationList", "Name", szActionName, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:SET CURSOR FIRST AD_BaseRoot.Operation WHERE AD_BaseRoot.Operation.Name = szActionName
                  RESULT = SetCursorFirstEntityByString( AD_BaseRoot, "Operation", "Name", szActionName, "" );
                  //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
                  CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
                  //:TZWINDOWL.Operation.Name = szActionName
                  SetAttributeFromString( TZWINDOWL, "Operation", "Name", szActionName );
                  //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_BaseRoot, "Operation", zSET_NULL )
                  SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_BaseRoot, "Operation", zSET_NULL );
                  //:FOR EACH AD_BaseRoot.Parameter
                  RESULT = SetCursorFirstEntity( AD_BaseRoot, "Parameter", "" );
                  while ( RESULT > zCURSOR_UNCHANGED )
                  { 
                     //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
                     CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
                     //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_BaseRoot, "Parameter", zSET_NULL )
                     SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_BaseRoot, "Parameter", zSET_NULL );
                     RESULT = SetCursorNextEntity( AD_BaseRoot, "Parameter", "" );
                  } 

                  //:END
                  //:INCLUDE TZWINDOWL.OperationList FROM TZWINDOWL.Operation
                  RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "OperationList", TZWINDOWL, "Operation", zPOS_AFTER );
               } 

               //:END
               //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.OperationList
               RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "OperationList", zPOS_AFTER );
            } 

            //:END
         } 

         //:END

         //:// Tie the Action to an Event on the Control
         //:CreateMetaEntity( TZCONTROL, TZCONTROL, "Event", zPOS_AFTER )
         CreateMetaEntity( TZCONTROL, TZCONTROL, "Event", zPOS_AFTER );
         //:TZCONTROL.Event.Type = AD_Base.Event.Type
         SetAttributeFromAttribute( TZCONTROL, "Event", "Type", AD_Base, "Event", "Type" );
         //:INCLUDE TZCONTROL.EventAct FROM TZWINDOWL.Action
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "EventAct", TZWINDOWL, "Action", zPOS_AFTER );

         //:// If this is an event on the first mapping Control, we've just processed the action on that Control and need
         //:// to reposition on the last Control
         //:IF AD_Base.Control.Tag = szFirstControlTag
         if ( CompareAttributeToString( AD_Base, "Control", "Tag", szFirstControlTag ) == 0 )
         { 
            //:SET CURSOR LAST TZCONTROL.Control
            RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( AD_Base, "Control", "" );
      //:END
   } 

   //:END

   //:// Position back to top for both created Controls and Base.
   //:ResetViewFromSubobject( AD_Base )   // Go back to second Group.
   ResetViewFromSubobject( AD_Base );
   //:ResetViewFromSubobject( AD_Base )   // Go back to first Group.
   ResetViewFromSubobject( AD_Base );
   //:ResetViewFromSubobject( TZCONTROL )   // Go back to second Group.
   ResetViewFromSubobject( TZCONTROL );
   //:ResetViewFromSubobject( TZCONTROL )   // Go back to first Group.
   ResetViewFromSubobject( TZCONTROL );

   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   //:DropView( AD_BaseRoot )
   DropView( AD_BaseRoot );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:BuildAutodesignGroupPotList( VIEW TZADCSDO    BASED ON LOD TZADCSDO,
//:                             VIEW SelectedLOD BASED ON LOD TZZOLODO,
//:                             INTEGER lLevel )

//:   STRING ( 32 ) szAttributeName
static zSHORT
o_BuildAutodesignGroupPotList( zVIEW     TZADCSDO,
                               zVIEW     SelectedLOD,
                               zLONG     lLevel )
{
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szDomainName
   zCHAR     szDomainName[ 33 ] = { 0 }; 
   //:STRING ( 21 ) szLeadingSpaces
   zCHAR     szLeadingSpaces[ 22 ] = { 0 }; 
   //:STRING ( 21 ) szIndentSpaces
   zCHAR     szIndentSpaces[ 22 ] = { 0 }; 
   //:STRING ( 90 ) szPromptText
   zCHAR     szPromptText[ 91 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 101 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 9 ]; 


   //:// Build the FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
   //:CREATE ENTITY TZADCSDO.FlatListPotentialAttribute
   RESULT = CreateEntity( TZADCSDO, "FlatListPotentialAttribute", zPOS_AFTER );
   //:IF lLevel = 0
   if ( lLevel == 0 )
   { 
      //:szIndentSpaces = ""
      ZeidonStringCopy( szIndentSpaces, 1, 0, "", 1, 0, 22 );
      //:ELSE
   } 
   else
   { 
      //:szLeadingSpaces = "                     "
      ZeidonStringCopy( szLeadingSpaces, 1, 0, "                     ", 1, 0, 22 );
      //:szIndentSpaces = szLeadingSpaces[1:lLevel]
      ZeidonStringCopy( szIndentSpaces, 1, 0, szLeadingSpaces, 1, lLevel, 22 );
   } 

   //:END
   //:TZADCSDO.FlatListPotentialAttribute.EntityName       = SelectedLOD.LOD_EntityParent.Name
   SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialAttribute", "EntityName", SelectedLOD, "LOD_EntityParent", "Name" );
   //:TZADCSDO.FlatListPotentialAttribute.IndentEntityName = szIndentSpaces + SelectedLOD.LOD_EntityParent.Name
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, SelectedLOD, "LOD_EntityParent", "Name", "", 0 );
   ZeidonStringCopy( szTempString_0, 1, 0, szIndentSpaces, 1, 0, 101 );
   ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 101 );
   SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "IndentEntityName", szTempString_0 );
   //:IF SelectedLOD.ER_RelLinkRec EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( SelectedLOD, "ER_RelLinkRec" );
   if ( lTempInteger_0 == 0 )
   { 
      //:TZADCSDO.FlatListPotentialAttribute.ParentRelationshipCardinality = SelectedLOD.ER_RelLinkRec.CardMin + "-" +
      //:                                                                    SelectedLOD.ER_RelLinkRec.CardMax 
      GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), SelectedLOD, "ER_RelLinkRec", "CardMin" );
      ZeidonStringConcat( szTempString_2, 1, 0, "-", 1, 0, 255 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 9, SelectedLOD, "ER_RelLinkRec", "CardMax", "", 0 );
      ZeidonStringConcat( szTempString_2, 1, 0, szTempString_3, 1, 0, 255 );
      SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "ParentRelationshipCardinality", szTempString_2 );
   } 

   //:END

   //:FOR EACH SelectedLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( SelectedLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADCSDO.FlatListPotentialAttribute
      RESULT = CreateEntity( TZADCSDO, "FlatListPotentialAttribute", zPOS_AFTER );
      //:szAttributeName = SelectedLOD.ER_AttributeRec.Name
      GetVariableFromAttribute( szAttributeName, 0, 'S', 33, SelectedLOD, "ER_AttributeRec", "Name", "", 0 );
      //:szDomainName    = SelectedLOD.DomainRec.Name
      GetVariableFromAttribute( szDomainName, 0, 'S', 33, SelectedLOD, "DomainRec", "Name", "", 0 );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialAttribute", SelectedLOD, "LOD_EntityParent", zSET_ALL )
      SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialAttribute", SelectedLOD, "LOD_EntityParent", zSET_ALL );
      //:TZADCSDO.FlatListPotentialAttribute.DataType      = SelectedLOD.DomainRec.DataType 
      SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialAttribute", "DataType", SelectedLOD, "DomainRec", "DataType" );
      //:TZADCSDO.FlatListPotentialAttribute.EntityName    = SelectedLOD.LOD_EntityParent.Name
      SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialAttribute", "EntityName", SelectedLOD, "LOD_EntityParent", "Name" );
      //:TZADCSDO.FlatListPotentialAttribute.AttributeName = szAttributeName
      SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "AttributeName", szAttributeName );
      //:INCLUDE TZADCSDO.FLP_LOD_Attribute FROM SelectedLOD.LOD_AttributeRec 
      RESULT = IncludeSubobjectFromSubobject( TZADCSDO, "FLP_LOD_Attribute", SelectedLOD, "LOD_AttributeRec", zPOS_AFTER );

      //:// Create Control Type based on Update characteristic of Entity and Domain.
      //:// A Domain of Y/N will create a Checkbox even if the entity is not updatable.
      //:IF SelectedLOD.DomainRec.Name = "Y/N"
      if ( CompareAttributeToString( SelectedLOD, "DomainRec", "Name", "Y/N" ) == 0 )
      { 
         //:// Domain is Y/N.
         //:TZADCSDO.FlatListPotentialAttribute.ControlType   = "CheckBox"
         SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "ControlType", "CheckBox" );
         //:ELSE
      } 
      else
      { 
         //:IF TZADCSDO.FlatListPotentialTopEntity.Update = "Y"
         if ( CompareAttributeToString( TZADCSDO, "FlatListPotentialTopEntity", "Update", "Y" ) == 0 )
         { 
            //:IF SelectedLOD.DomainRec.Name = "Date" OR SelectedLOD.DomainRec.Name = "DateTime"
            if ( CompareAttributeToString( SelectedLOD, "DomainRec", "Name", "Date" ) == 0 || CompareAttributeToString( SelectedLOD, "DomainRec", "Name", "DateTime" ) == 0 )
            { 
               //:// A Domain of Date will make the control a Calendar.
               //:TZADCSDO.FlatListPotentialAttribute.ControlType   = "Calendar"
               SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "ControlType", "Calendar" );
               //:ELSE
            } 
            else
            { 
               //:IF SelectedLOD.DomainRec.DomainType = "T"
               if ( CompareAttributeToString( SelectedLOD, "DomainRec", "DomainType", "T" ) == 0 )
               { 
                  //:// The Domain is a table, so make control a Combobox.
                  //:TZADCSDO.FlatListPotentialAttribute.ControlType   = "ComboBox"
                  SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "ControlType", "ComboBox" );
                  //:ELSE
               } 
               else
               { 
                  //:// If not a table, make control an Editbox.
                  //:TZADCSDO.FlatListPotentialAttribute.ControlType   = "EditBox"
                  SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "ControlType", "EditBox" );
               } 

               //:END
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:// The entity is not updatable, so Control Type is "Text"
            //:TZADCSDO.FlatListPotentialAttribute.ControlType   = "Text"
            SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "ControlType", "Text" );
         } 

         //:END
      } 

      //:END

      //:// The Data Width of each Attribute will depend on Domain Type, as follows.
      //:IF SelectedLOD.DomainRec.DataType = "S"
      if ( CompareAttributeToString( SelectedLOD, "DomainRec", "DataType", "S" ) == 0 )
      { 
         //:// String length is just length of Domain or Attribute, with a max value of 20 and a minimum of 5.
         //:IF SelectedLOD.ER_AttributeRec.Lth = ""
         if ( CompareAttributeToString( SelectedLOD, "ER_AttributeRec", "Lth", "" ) == 0 )
         { 
            //:TZADCSDO.FlatListPotentialAttribute.DataWidth = SelectedLOD.DomainRec.MaxStringLth
            SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", SelectedLOD, "DomainRec", "MaxStringLth" );
            //:ELSE
         } 
         else
         { 
            //:TZADCSDO.FlatListPotentialAttribute.DataWidth = SelectedLOD.ER_AttributeRec.Lth
            SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", SelectedLOD, "ER_AttributeRec", "Lth" );
         } 

         //:END
         //:IF TZADCSDO.FlatListPotentialAttribute.DataWidth > 20
         if ( CompareAttributeToInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 20 ) > 0 )
         { 
            //:TZADCSDO.FlatListPotentialAttribute.DataWidth = 20
            SetAttributeFromInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 20 );
            //:ELSE
         } 
         else
         { 
            //:IF TZADCSDO.FlatListPotentialAttribute.DataWidth < 5
            if ( CompareAttributeToInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 5 ) < 0 )
            { 
               //:TZADCSDO.FlatListPotentialAttribute.DataWidth = 5
               SetAttributeFromInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 5 );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF SelectedLOD.DomainRec.DataType = "L" OR SelectedLOD.DomainRec.DataType = "M"
         if ( CompareAttributeToString( SelectedLOD, "DomainRec", "DataType", "L" ) == 0 || CompareAttributeToString( SelectedLOD, "DomainRec", "DataType", "M" ) == 0 )
         { 
            //:// Integer or Decimal length is 8.
            //:TZADCSDO.FlatListPotentialAttribute.DataWidth = 8
            SetAttributeFromInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 8 );
            //:ELSE
         } 
         else
         { 
            //:IF SelectedLOD.DomainRec.DataType = "D" OR SelectedLOD.DomainRec.DataType = "T" OR SelectedLOD.DomainRec.DataType = "I"
            if ( CompareAttributeToString( SelectedLOD, "DomainRec", "DataType", "D" ) == 0 || CompareAttributeToString( SelectedLOD, "DomainRec", "DataType", "T" ) == 0 || CompareAttributeToString( SelectedLOD, "DomainRec", "DataType", "I" ) == 0 )
            { 
               //:// Date, DateTime or Time length is 10.
               //:TZADCSDO.FlatListPotentialAttribute.DataWidth = 10
               SetAttributeFromInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 10 );
               //:ELSE
            } 
            else
            { 
               //:// Anything else is 10.
               //:TZADCSDO.FlatListPotentialAttribute.DataWidth = 10
               SetAttributeFromInteger( TZADCSDO, "FlatListPotentialAttribute", "DataWidth", 10 );
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:// Prompt Value is Attribute Name.
      //:InsertSpacesInPrompt( szPromptText, TZADCSDO, szAttributeName, 100 )
      InsertSpacesInPrompt( szPromptText, TZADCSDO, szAttributeName, 100 );
      //:TZADCSDO.FlatListPotentialAttribute.PromptValue = szPromptText
      SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "PromptValue", szPromptText );
      RESULT = SetCursorNextEntity( SelectedLOD, "LOD_AttributeRec", "" );
   } 

   //:END

   //:// Process subentities.
   //:FOR EACH SelectedLOD.LOD_EntityChild
   RESULT = SetCursorFirstEntity( SelectedLOD, "LOD_EntityChild", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( SelectedLOD, "LOD_EntityChild" )
      SetViewToSubobject( SelectedLOD, "LOD_EntityChild" );
      //:lLevel = lLevel + 3
      lLevel = lLevel + 3;
      //:BuildAutodesignGroupPotList( TZADCSDO, SelectedLOD, lLevel )
      o_BuildAutodesignGroupPotList( TZADCSDO, SelectedLOD, lLevel );
      //:ResetViewFromSubobject( SelectedLOD )
      ResetViewFromSubobject( SelectedLOD );
      RESULT = SetCursorNextEntity( SelectedLOD, "LOD_EntityChild", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_TopEntityForAutodesign( VIEW ViewToWindow )

//:   VIEW TZADCSDO    REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
SELECT_TopEntityForAutodesign( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW SelectedLOD BASED ON LOD  TZZOLODO
   zVIEW     SelectedLOD = 0; 
   //:STRING ( 32 ) szTopEntityName
   zCHAR     szTopEntityName[ 33 ] = { 0 }; 
   //:SHORT         ReturnedLevel
   zSHORT    ReturnedLevel = 0; 
   //:SHORT         nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Create a new selected Entity.
   //:IF TZADCSDO.FlatListSelectedEntity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZADCSDO, "FlatListSelectedEntity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListSelectedEntity
      RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedEntity", zPOS_NEXT );
   } 

   //:END
   //:CREATE ENTITY TZADCSDO.FlatListSelectedEntity
   RESULT = CreateEntity( TZADCSDO, "FlatListSelectedEntity", zPOS_AFTER );
   //:SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedEntity", TZADCSDO, "FlatListPotentialTopEntity", zSET_ALL )
   SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedEntity", TZADCSDO, "FlatListPotentialTopEntity", zSET_ALL );
   //:TZWINDOWL.AutoDesignGroup.TopLevelEntityName = TZADCSDO.FlatListSelectedEntity.Name
   SetAttributeFromAttribute( TZWINDOWL, "AutoDesignGroup", "TopLevelEntityName", TZADCSDO, "FlatListSelectedEntity", "Name" );

   //:// Clear any existing selections.
   //:FOR EACH TZADCSDO.FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListPotentialAttribute NONE
      RESULT = DeleteEntity( TZADCSDO, "FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   } 

   //:END
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListSelectedAttribute NONE
      RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   } 

   //:END

   //:// Build list of potential attributes to be selected. This will contain all subobject entities/attributes for
   //:// the selected Top Entity.
   //:GET VIEW SelectedLOD NAMED "AutodesignSelectedLOD"
   RESULT = GetViewByName( &SelectedLOD, "AutodesignSelectedLOD", ViewToWindow, zLEVEL_TASK );
   //:SET CURSOR FIRST SelectedLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( SelectedLOD, "LOD_EntityParent", "" );
   //:szTopEntityName = TZADCSDO.FlatListSelectedEntity.Name
   GetVariableFromAttribute( szTopEntityName, 0, 'S', 33, TZADCSDO, "FlatListSelectedEntity", "Name", "", 0 );
   //:LocateTopEntityRecurs( TZADCSDO, SelectedLOD, szTopEntityName )
   o_LocateTopEntityRecurs( TZADCSDO, SelectedLOD, szTopEntityName );

   //:// Include entity under.
   //:IF TZWINDOWL.AD_GroupLOD_Entity EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZWINDOWL, "AD_GroupLOD_Entity" );
   if ( lTempInteger_1 == 0 )
   { 
      //:EXCLUDE TZWINDOWL.AD_GroupLOD_Entity
      RESULT = ExcludeEntity( TZWINDOWL, "AD_GroupLOD_Entity", zREPOS_AFTER );
   } 

   //:END
   //:SET CURSOR FIRST SelectedLOD.LOD_Entity WHERE SelectedLOD.LOD_Entity.Name = TZADCSDO.FlatListPotentialTopEntity.Name
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADCSDO, "FlatListPotentialTopEntity", "Name" );
   RESULT = SetCursorFirstEntityByString( SelectedLOD, "LOD_Entity", "Name", szTempString_0, "" );
   //:INCLUDE TZWINDOWL.AD_GroupLOD_Entity FROM SelectedLOD.LOD_Entity
   RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "AD_GroupLOD_Entity", SelectedLOD, "LOD_Entity", zPOS_AFTER );
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
o_LocateTopEntityRecurs( zVIEW     TZADCSDO,
                         zVIEW     SelectedLOD,
                         zPCHAR    szTopEntityName )
{
   zSHORT    RESULT; 

   //:LocateTopEntityRecurs( VIEW TZADCSDO    BASED ON LOD TZADCSDO,
   //:                    VIEW SelectedLOD BASED ON LOD TZZOLODO,
   //:                    STRING ( 32 ) szTopEntityName )

   //:// Search each LOD_EntityParent recursively until the entity identified by TopEntityName is located.
   //:// Then process that subobject to create the FlatListPotentialAttribute entries in TZADCSDO.
   //:FOR EACH SelectedLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( SelectedLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF SelectedLOD.LOD_EntityParent.Name = szTopEntityName
      if ( CompareAttributeToString( SelectedLOD, "LOD_EntityParent", "Name", szTopEntityName ) == 0 )
      { 
         //:// We've got a match on Top Entity, so go to process the subobject creating FlatListPotentialAttribute entries.
         //:BuildAutodesignGroupPotList( TZADCSDO, SelectedLOD, 0 )
         o_BuildAutodesignGroupPotList( TZADCSDO, SelectedLOD, 0 );
         //:ELSE
      } 
      else
      { 
         //:// This isn't a match on Top Entity, so continue recursive search.
         //:SetViewToSubobject( SelectedLOD, "LOD_EntityChild" )
         SetViewToSubobject( SelectedLOD, "LOD_EntityChild" );
         //:LocateTopEntityRecurs( TZADCSDO, SelectedLOD, szTopEntityName )
         o_LocateTopEntityRecurs( TZADCSDO, SelectedLOD, szTopEntityName );
         //:ResetViewFromSubobject( SelectedLOD )
         ResetViewFromSubobject( SelectedLOD );
      } 

      RESULT = SetCursorNextEntity( SelectedLOD, "LOD_EntityParent", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_PotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADCSDO  REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
SELECT_PotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW  REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZADCSDO2 BASED ON LOD  TZADCSDO
   zVIEW     TZADCSDO2 = 0; 
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

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", ViewToWindow, zLEVEL_TASK );

   //:// Make sure we have position on AutoDesignWindow.
   //:SET CURSOR FIRST TZWINDOW.AutoDesignWindow  
   RESULT = SetCursorFirstEntity( TZWINDOW, "AutoDesignWindow", "" );

   //:// First make sure that any currently Selected entry is selected on the Potential side, because we are going to
   //:// delete Selected entries and recreate them.
   //:FOR EACH TZADCSDO.FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST TZADCSDO.FlatListPotentialAttribute
      //:           WHERE TZADCSDO.FlatListPotentialAttribute.EntityName    = TZADCSDO.FlatListPotentialAttribute.EntityName
      //:             AND TZADCSDO.FlatListPotentialAttribute.AttributeName = TZADCSDO.FlatListPotentialAttribute.AttributeName
      RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADCSDO, "FlatListPotentialAttribute", "EntityName", TZADCSDO, "FlatListPotentialAttribute", "EntityName" ) != 0 ||
                 CompareAttributeToAttribute( TZADCSDO, "FlatListPotentialAttribute", "AttributeName", TZADCSDO, "FlatListPotentialAttribute", "AttributeName" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
         } 

      } 

      //:SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute", 1 )
      SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute", 1 );
      //:DELETE ENTITY TZADCSDO.FlatListPotentialAttribute NONE
      RESULT = DeleteEntity( TZADCSDO, "FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   } 

   //:END

   //:// Copy selected attributes from FlatListPotentialAttribute entries to FlatListPotentialAttribute entries.
   //:CreateViewFromView( TZADCSDO2, TZADCSDO )
   CreateViewFromView( &TZADCSDO2, TZADCSDO );
   //:FOR EACH TZADCSDO.FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute" )
      nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute" );
      //:IF nRC = 1 AND TZADCSDO.FlatListPotentialAttribute.AttributeName != ""   // We will skip Entity only entries, since that
      if ( nRC == 1 && CompareAttributeToString( TZADCSDO, "FlatListPotentialAttribute", "AttributeName", "" ) != 0 )
      { 
         //:CREATE ENTITY TZADCSDO.FlatListPotentialAttribute
         RESULT = CreateEntity( TZADCSDO, "FlatListPotentialAttribute", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialAttribute", TZADCSDO, "FlatListPotentialAttribute", zSET_ALL )
         SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialAttribute", TZADCSDO, "FlatListPotentialAttribute", zSET_ALL );

         //:SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute", 0 )
         SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   //:DropView( TZADCSDO2 )
   DropView( TZADCSDO2 );

   //:lMaxPromptLength = 0
   lMaxPromptLength = 0;
   //:FOR EACH TZADCSDO.FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szTempString = TZADCSDO.FlatListPotentialAttribute.PromptValue
      GetVariableFromAttribute( szTempString, 0, 'S', 101, TZADCSDO, "FlatListPotentialAttribute", "PromptValue", "", 0 );
      //:lPromptLength = GetStringLength( szTempString )
      lPromptLength = GetStringLength( szTempString );
      //:IF lPromptLength > lMaxPromptLength
      if ( lPromptLength > lMaxPromptLength )
      { 
         //:lMaxPromptLength = lPromptLength
         lMaxPromptLength = lPromptLength;
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = lMaxPromptLength
   SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", lMaxPromptLength );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:AutodesignUpdateCtrls( VIEW TZWINDOW    BASED ON LOD TZWDLGSO,
//:                       VIEW TZCONTROL   BASED ON LOD TZWDLGSO,
//:                       VIEW AD_Base     BASED ON LOD TZWDLGSO,
//:                       VIEW SelectedLOD BASED ON LOD TZZOLODO )

//:   VIEW TZPESRCO          BASED ON LOD TZPESRCO
static zSHORT
o_AutodesignUpdateCtrls( zVIEW     TZWINDOW,
                         zVIEW     TZCONTROL,
                         zVIEW     AD_Base,
                         zVIEW     SelectedLOD )
{
   zVIEW     TZPESRCO = 0; 
   //:VIEW AD_BaseControlDef BASED ON LOD TZWDLGSO
   zVIEW     AD_BaseControlDef = 0; 
   //:INTEGER lGroupWidth
   zLONG     lGroupWidth = 0; 
   //:INTEGER lMappingDataWidth
   zLONG     lMappingDataWidth = 0; 
   //:INTEGER lMaxMappingDataWidth
   zLONG     lMaxMappingDataWidth = 0; 
   //:INTEGER lPromptWidth
   zLONG     lPromptWidth = 0; 
   //:INTEGER lControlPositionX
   zLONG     lControlPositionX = 0; 
   //:INTEGER lControlPositionY
   zLONG     lControlPositionY = 0; 
   //:INTEGER lCurrentPosition
   zLONG     lCurrentPosition = 0; 
   //:INTEGER lTotalDataWidth
   zLONG     lTotalDataWidth = 0; 
   //:INTEGER lAveragPixelWidth
   zLONG     lAveragPixelWidth = 0; 
   //:INTEGER lAttributeCount
   zLONG     lAttributeCount = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:STRING ( 100 ) szAttributeName
   zCHAR     szAttributeName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szControlDefName
   zCHAR     szControlDefName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szActionSuffix
   zCHAR     szActionSuffix[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szActionName
   zCHAR     szActionName[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szControlNameSuffix
   zCHAR     szControlNameSuffix[ 101 ] = { 0 }; 
   //:STRING ( 1 )   szControlNameSuffixGroup
   zCHAR     szControlNameSuffixGroup[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szControlNameSuffixAttribute
   zCHAR     szControlNameSuffixAttribute[ 2 ] = { 0 }; 
   //:STRING ( 10 )  szGroupName
   zCHAR     szGroupName[ 11 ] = { 0 }; 
   //:STRING ( 8 )   szGroupName8
   zCHAR     szGroupName8[ 9 ] = { 0 }; 
   //:STRING ( 20 )  szControlType
   zCHAR     szControlType[ 21 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zCHAR     szTempString_3[ 255 ]; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zLONG     lTempInteger_10; 


   //:// Build a Text/Update controls 

   //:GET VIEW TZPESRCO NAMED "TZPESRCO"
   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZWINDOW, zLEVEL_TASK );
   //:szActionSuffix = TZWINDOW.AutoDesignGroup.ActionNameSuffix
   GetVariableFromAttribute( szActionSuffix, 0, 'S', 101, TZWINDOW, "AutoDesignGroup", "ActionNameSuffix", "", 0 );
   //:CreateViewFromView( AD_BaseControlDef, AD_Base )
   CreateViewFromView( &AD_BaseControlDef, AD_Base );
   //:NAME VIEW AD_BaseControlDef "AD_BaseRoot"
   SetNameForView( AD_BaseControlDef, "AD_BaseRoot", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST AD_BaseControlDef.Window WHERE AD_BaseControlDef.Window.Tag = "UpdateFields"
   RESULT = SetCursorFirstEntityByString( AD_BaseControlDef, "Window", "Tag", "UpdateFields", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZWINDOW, "", "Autodesign Window Group",
      //:             "The Base group doesn't have a Controls Base Window named 'UpdateFields'.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZWINDOW, "", "Autodesign Window Group", "The Base group doesn't have a Controls Base Window named 'UpdateFields'.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZWINDOW, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( TZWINDOW, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Determine Width of Prompt and Mapping Controls.
   //:// We will take the UpdateFieldPromptLength value (which is max field prompt size) and multiply it by a character
   //:// size of 8 pixels to get the width for Prompt entries. The width of the Mapping entries will be the
   //:// difference in the size of the Group.
   //:lPromptWidth         = TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength * 5
   GetIntegerFromAttribute( &lTempInteger_0, TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength" );
   lPromptWidth = lTempInteger_0 * 5;
   //:lGroupWidth          = TZWINDOW.AutoDesignGroup.ControlWidthInPixels - 10
   GetIntegerFromAttribute( &lTempInteger_1, TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels" );
   lGroupWidth = lTempInteger_1 - 10;
   //:lMaxMappingDataWidth = lGroupWidth - lPromptWidth
   lMaxMappingDataWidth = lGroupWidth - lPromptWidth;

   //:// Build a Prompt and Mapping entry for each FlatListSelectedAttribute entry.
   //:// Starting Y position will be from first control in UpdateGroup.
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:lControlPositionY = 10
   lControlPositionY = 10;
   //:lControlPositionX = 10
   lControlPositionX = 10;
   //:lAttributeCount = 0
   lAttributeCount = 0;
   //:FOR EACH TZWINDOW.AD_MappingAttribute
   RESULT = SetCursorFirstEntity( TZWINDOW, "AD_MappingAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:lAttributeCount = lAttributeCount + 1
      lAttributeCount = lAttributeCount + 1;
      //:szControlNameSuffixAttribute = lAttributeCount
      ZeidonStringConvertFromNumber( szControlNameSuffixAttribute, 1, 0, 1, lAttributeCount, (ZDecimal) 0.0, "I" );

      //:// Create Prompt entry, which is a Text field. 
      //:CreateMetaEntity( TZWINDOW, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZCONTROL, "Control", zPOS_AFTER );
      //:szControlNameSuffix = TZWINDOW.AD_MappingAttribute.AttributeName + szControlNameSuffixGroup + szControlNameSuffixAttribute
      GetStringFromAttribute( szControlNameSuffix, zsizeof( szControlNameSuffix ), TZWINDOW, "AD_MappingAttribute", "AttributeName" );
      ZeidonStringConcat( szControlNameSuffix, 1, 0, szControlNameSuffixGroup, 1, 0, 101 );
      ZeidonStringConcat( szControlNameSuffix, 1, 0, szControlNameSuffixAttribute, 1, 0, 101 );
      //:TZCONTROL.Control.Tag       = "P_" + szControlNameSuffix
      ZeidonStringCopy( szTempString_0, 1, 0, "P_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szControlNameSuffix, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:TZCONTROL.Control.Text      = TZWINDOW.AD_MappingAttribute.PromptValue
      SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZWINDOW, "AD_MappingAttribute", "PromptValue" );
      //:TZCONTROL.Control.CSS_Class = TZWINDOW.AD_MappingAttribute.CSS_ClassForPrompt 
      SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", TZWINDOW, "AD_MappingAttribute", "CSS_ClassForPrompt" );
      //:TZCONTROL.Control.PSDLG_X   = lControlPositionX
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", lControlPositionX );
      //:TZCONTROL.Control.SZDLG_Y   = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 10 );
      //:TZCONTROL.Control.SZDLG_X   = lPromptWidth
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lPromptWidth );
      //:TZCONTROL.Control.PSDLG_Y   = lControlPositionY
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", lControlPositionY );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Text"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Text", "" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );

      //:// Create the link between the Prompt attribute and the control that it goes with. It's the same name as the tag of the generated control below.
      //:TZCONTROL.Control.WebCtrlLabelLink = "M_" + szControlNameSuffix
      ZeidonStringCopy( szTempString_1, 1, 0, "M_", 1, 0, 255 );
      ZeidonStringConcat( szTempString_1, 1, 0, szControlNameSuffix, 1, 0, 255 );
      SetAttributeFromString( TZCONTROL, "Control", "WebCtrlLabelLink", szTempString_1 );

      //:// Create Mapping entry.
      //:CreateMetaEntity( TZWINDOW, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZCONTROL, "Control", zPOS_AFTER );
      //:TZCONTROL.Control.Tag       = "M_" + szControlNameSuffix
      ZeidonStringCopy( szTempString_2, 1, 0, "M_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szControlNameSuffix, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_2 );
      //:TZCONTROL.Control.CSS_Class = TZWINDOW.AD_MappingAttribute.CSS_ClassForControl 
      SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", TZWINDOW, "AD_MappingAttribute", "CSS_ClassForControl" );
      //:TZCONTROL.Control.PSDLG_X   = lControlPositionX + lPromptWidth
      lTempInteger_2 = lControlPositionX + lPromptWidth;
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", lTempInteger_2 );
      //:TZCONTROL.Control.SZDLG_Y   = 12
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 12 );
      //:TZCONTROL.Control.PSDLG_Y   = lControlPositionY
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", lControlPositionY );

      //:// The width of the  Control will normally be determined by the size of the associated attribute, but
      //:// there will always be a max size (lMaxMappingDataWidth) determined above and a min size of 80..
      //:lMappingDataWidth = TZWINDOW.AD_MappingAttribute.DataWidth * 5
      GetIntegerFromAttribute( &lTempInteger_3, TZWINDOW, "AD_MappingAttribute", "DataWidth" );
      lMappingDataWidth = lTempInteger_3 * 5;
      //:IF lMappingDataWidth > lMaxMappingDataWidth
      if ( lMappingDataWidth > lMaxMappingDataWidth )
      { 
         //:lMappingDataWidth = lMaxMappingDataWidth
         lMappingDataWidth = lMaxMappingDataWidth;
         //:ELSE
      } 
      else
      { 
         //:IF lMappingDataWidth < 80
         if ( lMappingDataWidth < 80 )
         { 
            //:lMappingDataWidth = 80
            lMappingDataWidth = 80;
         } 

         //:END
      } 

      //:END
      //:TZCONTROL.Control.SZDLG_X = lMappingDataWidth
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lMappingDataWidth );

      //:// Build the Editbox, Checkbox, Calendar, MLEdit or Combobox controls depending on ControlType.
      //:// If the GenerateGroupType is "F", then the request is for display fields only and the ControlType will be Text.
      //:szControlType = TZWINDOW.AD_MappingAttribute.ControlType
      GetVariableFromAttribute( szControlType, 0, 'S', 21, TZWINDOW, "AD_MappingAttribute", "ControlType", "", 0 );
      //:IF TZWINDOW.AutoDesignGroup.GenerateGroupType = "F"    // "F" is for Display fields only.
      if ( CompareAttributeToString( TZWINDOW, "AutoDesignGroup", "GenerateGroupType", "F" ) == 0 )
      { 
         //:szControlType = "Text"
         ZeidonStringCopy( szControlType, 1, 0, "Text", 1, 0, 21 );
      } 

      //:END

      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlType
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlType, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Text"
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Text", "" );
      } 

      //:END
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );

      //:// If this is a CheckBox, add Prompt as Text.
      //:IF szControlType = "CheckBox"
      if ( ZeidonStringCompare( szControlType, 1, 0, "CheckBox", 1, 0, 21 ) == 0 )
      { 
         //:TZCONTROL.Control.Text = TZWINDOW.AD_MappingAttribute.PromptValue
         SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZWINDOW, "AD_MappingAttribute", "PromptValue" );
      } 

      //:END

      //:// If this is a ComboBox, set the CtrlBOI attribute from the ComboBox Control in AD_BaseControlDef named "ComboBoxBase"..
      //:IF szControlType = "ComboBox"
      if ( ZeidonStringCompare( szControlType, 1, 0, "ComboBox", 1, 0, 21 ) == 0 )
      { 
         //:SET CURSOR FIRST AD_BaseControlDef.Control WHERE AD_BaseControlDef.Control.Tag = "ComboBoxBase"
         RESULT = SetCursorFirstEntityByString( AD_BaseControlDef, "Control", "Tag", "ComboBoxBase", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //://TraceLineS( "*** Setting ComboxBox CtrlBOI: ", TZCONTROL.Control.Tag )
            //:TZCONTROL.Control.CtrlBOI = AD_BaseControlDef.Control.CtrlBOI 
            SetAttributeFromAttribute( TZCONTROL, "Control", "CtrlBOI", AD_BaseControlDef, "Control", "CtrlBOI" );
            //:ELSE
         } 
         else
         { 
            //:MessageSend( TZWINDOW, "", "Autodesign Update Controls",
            //:       "The AD_Base does not have a Control named 'ComboBoxBase' for ComboBox characteristics. The generated Control will not be complete.",
            //:       zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( TZWINDOW, "", "Autodesign Update Controls", "The AD_Base does not have a Control named 'ComboBoxBase' for ComboBox characteristics. The generated Control will not be complete.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         } 

         //:END
      } 

      //:END

      //:// Build CtrlMap subobject for list control entity from Entity.Attribute of FlatListSelectedAttribute
      //:CreateMetaEntity( TZWINDOW, TZCONTROL, "CtrlMap", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:SET CURSOR FIRST SelectedLOD.LOD_Entity WHERE SelectedLOD.LOD_Entity.Name = TZWINDOW.AD_MappingAttribute.EntityName
      GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), TZWINDOW, "AD_MappingAttribute", "EntityName" );
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "LOD_Entity", "Name", szTempString_3, "" );
      //:SET CURSOR FIRST SelectedLOD.ER_Attribute WITHIN SelectedLOD.LOD_Entity
      //:           WHERE SelectedLOD.ER_Attribute.Name = TZWINDOW.AD_MappingAttribute.AttributeName
      GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), TZWINDOW, "AD_MappingAttribute", "AttributeName" );
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "ER_Attribute", "Name", szTempString_3, "LOD_Entity" );
      //:INCLUDE TZCONTROL.CtrlMapLOD_Attribute FROM SelectedLOD.LOD_Attribute
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", SelectedLOD, "LOD_Attribute", zPOS_AFTER );
      //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = SelectedLOD.LOD.Name  
      GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), SelectedLOD, "LOD", "Name" );
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szTempString_3, "" );
      //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOW.ViewObjRef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );

      //:// If the Control is MLEdit, then we want to change the height of the Control from 10 to 36.
      //:IF szControlType = "MLEdit"
      if ( ZeidonStringCompare( szControlType, 1, 0, "MLEdit", 1, 0, 21 ) == 0 )
      { 
         //:TZCONTROL.Control.SZDLG_Y   = 36
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 36 );
         //:lControlPositionY = lControlPositionY + 36
         lControlPositionY = lControlPositionY + 36;
         //:ELSE
      } 
      else
      { 
         //:lControlPositionY = lControlPositionY + 12
         lControlPositionY = lControlPositionY + 12;
      } 

      RESULT = SetCursorNextEntity( TZWINDOW, "AD_MappingAttribute", "" );
      //:END
   } 


   //:END

   //:// Set the Y size of the GroupBox based on the position of the last control generated.
   //:// Also adjust the size of any parent GroupBox accordingly.
   //:ResetViewFromSubobject( TZCONTROL )   // Go back to the GroupBox
   ResetViewFromSubobject( TZCONTROL );
   //:TZCONTROL.Control.SZDLG_Y = lControlPositionY + 15
   lTempInteger_4 = lControlPositionY + 15;
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", lTempInteger_4 );
   //:nRC = ResetViewFromSubobject( TZCONTROL )   // Go back to second Group.
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:IF nRC = 0
   if ( nRC == 0 )
   { 
      //:TZCONTROL.Control.SZDLG_Y = TZCONTROL.CtrlCtrl.PSDLG_Y + TZCONTROL.CtrlCtrl.SZDLG_Y + 10
      GetIntegerFromAttribute( &lTempInteger_5, TZCONTROL, "CtrlCtrl", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_6, TZCONTROL, "CtrlCtrl", "SZDLG_Y" );
      lTempInteger_7 = lTempInteger_5 + lTempInteger_6 + 10;
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", lTempInteger_7 );
      //:nRC = ResetViewFromSubobject( TZCONTROL )   // Go back to first Group.
      nRC = ResetViewFromSubobject( TZCONTROL );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:TZCONTROL.Control.SZDLG_Y = TZCONTROL.CtrlCtrl.PSDLG_Y + TZCONTROL.CtrlCtrl.SZDLG_Y + 10
         GetIntegerFromAttribute( &lTempInteger_8, TZCONTROL, "CtrlCtrl", "PSDLG_Y" );
         GetIntegerFromAttribute( &lTempInteger_9, TZCONTROL, "CtrlCtrl", "SZDLG_Y" );
         lTempInteger_10 = lTempInteger_8 + lTempInteger_9 + 10;
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", lTempInteger_10 );
      } 

      //:END
   } 

   //:END

   //:DropView( AD_BaseControlDef )
   DropView( AD_BaseControlDef );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildAutodesignForGroup( VIEW ViewToWindow )

//:   VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignForGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL   REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZADCSDO    BASED ON LOD  TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:VIEW SelectedLOD BASED ON LOD  TZZOLODO
   zVIEW     SelectedLOD = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:STRING ( 100 ) szTopEntityName
   zCHAR     szTopEntityName[ 101 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zLONG     lTempInteger_1; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );

   //:// This function is triggered for autodesigning a specific Group control within a Window.
   //:// Try to position on an existing AutoDesignWindow and AutoDesignGroup entry and if they don't exist,
   //:// create new ones. In any case, make the AutoDesignGroup temporal.

   //:// Make sure an Auto Design Base dialog has been loaded.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// The AD_Base object isn't in memory, so load it.
      //:nRC = ActivateAD_Base( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_Base( &AD_Base, ViewToWindow );
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

   //:SET CURSOR FIRST TZWINDOWL.AutoDesignWindow WHERE TZWINDOWL.AutoDesignWindow.WindowTag = TZWINDOWL.Window.Tag
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZWINDOWL, "Window", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "AutoDesignWindow", "WindowTag", szTempString_0, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AutoDesignWindow", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZWINDOWL, "AutoDesignWindow", zPOS_AFTER );
      //:TZWINDOWL.AutoDesignWindow.WindowTag = TZWINDOWL.Window.Tag
      SetAttributeFromAttribute( TZWINDOWL, "AutoDesignWindow", "WindowTag", TZWINDOWL, "Window", "Tag" );
   } 

   //:END
   //:SET CURSOR FIRST TZWINDOWL.AutoDesignGroup WHERE TZWINDOWL.AutoDesignGroup.GroupTag = TZCONTROL.Control.Tag
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZCONTROL, "Control", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "AutoDesignGroup", "GroupTag", szTempString_0, "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Make the existing subobject temporal.
      //:CreateTemporalSubobjectVersion( TZWINDOWL, "AutoDesignGroup" )
      CreateTemporalSubobjectVersion( TZWINDOWL, "AutoDesignGroup" );
      //:ELSE
   } 
   else
   { 
      //:// Make the new subobject temporal.
      //:CreateTemporalEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER )
      CreateTemporalEntity( TZWINDOWL, "AutoDesignGroup", zPOS_AFTER );
      //:TZWINDOWL.AutoDesignGroup.GroupTag = TZCONTROL.Control.Tag
      SetAttributeFromAttribute( TZWINDOWL, "AutoDesignGroup", "GroupTag", TZCONTROL, "Control", "Tag" );
   } 

   //:END
   //:TZWINDOWL.AutoDesignGroup.ControlWidthInPixels = TZCONTROL.Control.SZDLG_X
   SetAttributeFromAttribute( TZWINDOWL, "AutoDesignGroup", "ControlWidthInPixels", TZCONTROL, "Control", "SZDLG_X" );

   //:// Always delete any existing auto design selection object.
   //:GET VIEW TZADCSDO NAMED "TZADCSDO"
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( TZADCSDO )
      DropObjectInstance( TZADCSDO );
   } 

   //:END

   //:// If the Group has previous mapping entries, build the TZADCSDO object, which will have the selection entries,
   //:// FlatListPotentialAttribute and FlatListSelectedAttribute.
   //:IF TZWINDOWL.AD_MappingAttribute EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "AD_MappingAttribute" );
   if ( lTempInteger_0 == 0 )
   { 

      //:// Initialize new Auto Design object.
      //:ActivateEmptyMetaOI( ViewToWindow, TZADCSDO, zSOURCE_UIS_META, zSINGLE )
      ActivateEmptyMetaOI( ViewToWindow, &TZADCSDO, zSOURCE_UIS_META, zSINGLE );
      //:NAME VIEW TZADCSDO "TZADCSDO"
      SetNameForView( TZADCSDO, "TZADCSDO", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADCSDO.UI_Spec
      RESULT = CreateEntity( TZADCSDO, "UI_Spec", zPOS_AFTER );

      //:// Activate LOD for creation of potential attributes.
      //:nRC = ActivateMetaOI_ByZKey( ViewToWindow, SelectedLOD, 0, zREFER_LOD_META, zSINGLE, TZWINDOWL.AD_GroupViewObjRefLOD.ZKey, 0 )
      GetIntegerFromAttribute( &lTempInteger_1, TZWINDOWL, "AD_GroupViewObjRefLOD", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( ViewToWindow, &SelectedLOD, 0, zREFER_LOD_META, zSINGLE, lTempInteger_1, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
         //:             "The Lod Object could not be read.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Window Group", "The Lod Object could not be read.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:NAME VIEW SelectedLOD "AutodesignSelectedLOD"
      SetNameForView( SelectedLOD, "AutodesignSelectedLOD", 0, zLEVEL_TASK );

      //:// Build list of potential attributes to be selected. This will contain all subobject entities/attributes for
      //:// the selected Top Entity.
      //:szTopEntityName = TZWINDOWL.AutoDesignGroup.TopLevelEntityName
      GetVariableFromAttribute( szTopEntityName, 0, 'S', 101, TZWINDOWL, "AutoDesignGroup", "TopLevelEntityName", "", 0 );
      //:SET CURSOR FIRST SelectedLOD.LOD_Entity WHERE SelectedLOD.LOD_Entity.Name = szTopEntityName
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "LOD_Entity", "Name", szTopEntityName, "" );
      //:CREATE ENTITY TZADCSDO.FlatListPotentialTopEntity
      RESULT = CreateEntity( TZADCSDO, "FlatListPotentialTopEntity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialTopEntity", SelectedLOD, "LOD_Entity", zSET_ALL )
      SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialTopEntity", SelectedLOD, "LOD_Entity", zSET_ALL );
      //:SET CURSOR FIRST SelectedLOD.LOD_EntityParent
      RESULT = SetCursorFirstEntity( SelectedLOD, "LOD_EntityParent", "" );
      //:LocateTopEntityRecurs( TZADCSDO, SelectedLOD, szTopEntityName )
      o_LocateTopEntityRecurs( TZADCSDO, SelectedLOD, szTopEntityName );

      //:// Build currently Selected Flat List of entities.
      //:CREATE ENTITY TZADCSDO.FlatListSelectedEntity
      RESULT = CreateEntity( TZADCSDO, "FlatListSelectedEntity", zPOS_AFTER );
      //:FOR EACH TZWINDOWL.AD_MappingAttribute
      RESULT = SetCursorFirstEntity( TZWINDOWL, "AD_MappingAttribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CREATE ENTITY TZADCSDO.FlatListSelectedAttribute
         RESULT = CreateEntity( TZADCSDO, "FlatListSelectedAttribute", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZWINDOWL, "AD_MappingAttribute", zSET_ALL )
         SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZWINDOWL, "AD_MappingAttribute", zSET_ALL );
         RESULT = SetCursorNextEntity( TZWINDOWL, "AD_MappingAttribute", "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SET_SelectedControlTypes( VIEW ViewToWindow )

//:   VIEW TZWINDOWL BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
SET_SelectedControlTypes( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZADCSDO  BASED ON LOD TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:// Set the Control Type for each selected entry in TZADCSDO.FlatListSelectedAttribute.

   //:GET VIEW TZWINDOWL NAMED "TZWINDOWL"
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   //:GET VIEW TZADCSDO NAMED "TZADCSDO"
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );

   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListSelectedAttribute" )
      nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListSelectedAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:TZADCSDO.FlatListSelectedAttribute.ControlType = TZWINDOWL.AutoDesignGroup.TempControlType
         SetAttributeFromAttribute( TZADCSDO, "FlatListSelectedAttribute", "ControlType", TZWINDOWL, "AutoDesignGroup", "TempControlType" );
         //:SetSelectStateOfEntity( TZADCSDO, "FlatListSelectedAttribute", 0 )
         SetSelectStateOfEntity( TZADCSDO, "FlatListSelectedAttribute", 0 );
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:RECALCULATE_UpdatePromptLength( VIEW ViewToWindow )

//:   VIEW TZADCSDO  REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
RECALCULATE_UpdatePromptLength( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:STRING ( 100 ) szTempString
   zCHAR     szTempString[ 101 ] = { 0 }; 
   //:INTEGER lMaxPromptLength
   zLONG     lMaxPromptLength = 0; 
   //:INTEGER lPromptLength
   zLONG     lPromptLength = 0; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// Recalculate the Length.
   //:lMaxPromptLength = 0
   lMaxPromptLength = 0;
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szTempString = TZADCSDO.FlatListSelectedAttribute.PromptValue
      GetVariableFromAttribute( szTempString, 0, 'S', 101, TZADCSDO, "FlatListSelectedAttribute", "PromptValue", "", 0 );
      //:lPromptLength = GetStringLength( szTempString )
      lPromptLength = GetStringLength( szTempString );
      //:IF lPromptLength > lMaxPromptLength
      if ( lPromptLength > lMaxPromptLength )
      { 
         //:lMaxPromptLength = lPromptLength
         lMaxPromptLength = lPromptLength;
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      //:END
   } 

   //:END
   //:TZWINDOWL.AutoDesignGroup.UpdateFieldPromptLength = lMaxPromptLength
   SetAttributeFromInteger( TZWINDOWL, "AutoDesignGroup", "UpdateFieldPromptLength", lMaxPromptLength );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CHECK_ReorderAttributes( VIEW ViewToWindow )

//:   VIEW TZADCSDO  REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
CHECK_ReorderAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADCSDO2 BASED ON LOD  TZADCSDO
   zVIEW     TZADCSDO2 = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that any Entity entries are properly positioned before their corresponding attributes.
   //:CreateViewFromView( TZADCSDO2, TZADCSDO )
   CreateViewFromView( &TZADCSDO2, TZADCSDO );
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADCSDO.FlatListSelectedAttribute.IndentEntityName != ""
      if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "IndentEntityName", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZADCSDO2.FlatListSelectedAttribute
         //:           WHERE TZADCSDO2.FlatListSelectedAttribute.EntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
         RESULT = SetCursorFirstEntityByString( TZADCSDO2, "FlatListSelectedAttribute", "EntityName", szTempString_0, "" );
         //:IF TZADCSDO2.FlatListSelectedAttribute.IndentEntityName != TZADCSDO.FlatListSelectedAttribute.IndentEntityName
         if ( CompareAttributeToAttribute( TZADCSDO2, "FlatListSelectedAttribute", "IndentEntityName", TZADCSDO, "FlatListSelectedAttribute", "IndentEntityName" ) != 0 )
         { 
            //:// An Entity entry has been moved, so put it back.
            //:CREATE ENTITY TZADCSDO2.FlatListSelectedAttribute BEFORE
            RESULT = CreateEntity( TZADCSDO2, "FlatListSelectedAttribute", zPOS_BEFORE );
            //:SetMatchingAttributesByName( TZADCSDO2, "FlatListSelectedAttribute", TZADCSDO, "FlatListSelectedAttribute", zSET_ALL )
            SetMatchingAttributesByName( TZADCSDO2, "FlatListSelectedAttribute", TZADCSDO, "FlatListSelectedAttribute", zSET_ALL );
            //:DELETE ENTITY TZADCSDO.FlatListSelectedAttribute
            RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedAttribute", zPOS_NEXT );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      //:END
   } 

   //:END
   //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   //:RefreshWindow( ViewToWindow )
   RefreshWindow( ViewToWindow );
   //:DropView( TZADCSDO2 )
   DropView( TZADCSDO2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_CRM_FindSubdialog( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_CRM_FindSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADCSDO    REGISTERED AS TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW    BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW = 0; 
   //:VIEW TaskLPLR    REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW CRMBase     BASED ON LOD  TZWDLGSO
   zVIEW     CRMBase = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW lConList    BASED ON LOD  TZZOLODO
   zVIEW     lConList = 0; 
   //:VIEW mConList    BASED ON LOD  TZZOLODO
   zVIEW     mConList = 0; 
   //:VIEW TZWDVORO    BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW CRMBaseSrc  BASED ON LOD  TZWDLGSO
   zVIEW     CRMBaseSrc = 0; 
   //:VIEW CL_UpdatSrc BASED ON LOD  TZWDLGSO
   zVIEW     CL_UpdatSrc = 0; 
   //:STRING ( 50 )  szAD_Suffix
   zCHAR     szAD_Suffix[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSearchObjectName
   zCHAR     szSearchObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedObjectName
   zCHAR     szReturnedObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupTitle
   zCHAR     szGroupTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewObjRefName
   zCHAR     szViewObjRefName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchN  // Search object name such as fPerson
   zCHAR     szAD_SchN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetN  // Returned object name such as lPerson
   zCHAR     szAD_RetN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetE  // Returned object include Entity Name
   zCHAR     szAD_RetE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetA  // Returned View.Entity.Attribute for display attribute
   zCHAR     szAD_RetA[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdO  // Update object name such as mPerson
   zCHAR     szAD_UpdO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdR  // Update object root entity name such as Person
   zCHAR     szAD_UpdR[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCRMBaseVML
   zCHAR     szCRMBaseVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCL_UpdatVML
   zCHAR     szCL_UpdatVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nNextPositionY
   zLONG     nNextPositionY = 0; 
   //:INTEGER nLength
   zLONG     nLength = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:SHORT   nRC2
   zSHORT    nRC2 = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Build the Windows for CRM Find and Area Maintenance from the CRMBase dialog template.
   //:// The build steps are as follows in adding windows to the current Dialog:
   //:// 1. Copy Window, ContactListUpdate and menus if they don't already exist.
   //:// 2. Copy CRMFind window to create Find_AD_Area window.
   //:// 3. Convert _AD_Suff and _AD_Area characters in CRMFind Action.Tag and Operation.Name for Find_AD_Area window.
   //:// 4. Convert Grid, "GridContactLists", mapping View from lConListLST to lConList_AD_SuffLST.
   //:// 5. Convert Grid, "GroupSearchResultsList", mapping View from mConList to mConList_AD_SuffLST.
   //:// 6. Build the Search fields group from the selected attributes in TZADWWKO.FlatSelectedSearchAttribute.

   //:// Initialize data.

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
   //:TraceLineS( "*** After TZWINDOW check", "" )
   TraceLineS( "*** After TZWINDOW check", "" );

   //:SET CURSOR LAST TZWINDOWL.Window    // Position at end of target Dialog.
   RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
   //:szAD_Suffix = TZADWWKO.AutodesignSubdialog.CRM_AreaSuffix 
   GetVariableFromAttribute( szAD_Suffix, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "CRM_AreaSuffix", "", 0 );
   //:szAD_Area   = TZADWWKO.AutodesignSubdialog.CRM_AreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "CRM_AreaName", "", 0 );
   //:IF szAD_Suffix = "" OR szAD_Area = ""
   if ( ZeidonStringCompare( szAD_Suffix, 1, 0, "", 1, 0, 51 ) == 0 || ZeidonStringCompare( szAD_Area, 1, 0, "", 1, 0, 51 ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both a CRM Area and a CRM Suffix must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a CRM Area and a CRM Suffix must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Search LOD.
   //:GET VIEW SearchLOD NAMED "TZZOLOD1"
   RESULT = GetViewByName( &SearchLOD, "TZZOLOD1", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Search Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Search Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:ReturnedLOD = SearchLOD
   ReturnedLOD = SearchLOD;

   //:// Check for Update LOD.
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

   //:// Check for other Update and Returned Object Data.
   //:IF TZADWWKO.LOD_EntityUpdateObjectRoot DOES NOT EXIST OR
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityUpdateObjectRoot" );
   //:   TZADWWKO.LOD_EntityReturnedObjectInclude DOES NOT EXIST OR
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityReturnedObjectInclude" );
   //:   TZADWWKO.LOD_EntityCL_ItemDisplay        DOES NOT EXIST OR
   lTempInteger_2 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityCL_ItemDisplay" );
   //:   TZADWWKO.LOD_AttributeCL_ItemDisplay     DOES NOT EXIST
   lTempInteger_3 = CheckExistenceOfEntity( TZADWWKO, "LOD_AttributeCL_ItemDisplay" );
   if ( lTempInteger_0 != 0 || lTempInteger_1 != 0 || lTempInteger_2 != 0 || lTempInteger_3 != 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "All Update and Returned Entity/Attribute entries must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "All Update and Returned Entity/Attribute entries must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Activate the CRMBase Dialog.
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, CRMBase, 0, zSOURCE_DIALOG_META, zSINGLE, "CRMBase", 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &CRMBase, 0, zSOURCE_DIALOG_META, zSINGLE, "CRMBase", 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "CRMBase Dialog could not be loaded.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "CRMBase Dialog could not be loaded.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW CRMBase "CRMBase" 
   SetNameForView( CRMBase, "CRMBase", 0, zLEVEL_TASK );

   //:// Make sure at least one Search Attribute has been selected.
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "At least one Search Attribute must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "At least one Search Attribute must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// 1. Copy Window, ContactListUpdate and menus if they don't already exist.
   //://    The follow code uses window CRMBASE.ContactListUpdate and VML CL_Updat.VML.
   //:SET CURSOR FIRST TZWINDOW.Window WHERE TZWINDOW.Window.Tag = "ContactListUpdate"
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "Window", "Tag", "ContactListUpdate", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// Copy over ContactListUpdate Window.
      //:SET CURSOR FIRST CRMBase.Window WHERE CRMBase.Window.Tag = "ContactListUpdate" 
      RESULT = SetCursorFirstEntityByString( CRMBase, "Window", "Tag", "ContactListUpdate", "" );
      //:nRC = CloneWindow( TZWINDOW, CRMBase, TaskLPLR, ViewToWindow )
      nRC = oTZWDLGSO_CloneWindow( TZWINDOW, CRMBase, TaskLPLR, ViewToWindow );

      //:// The Clone should create an Operation Source entry named "CL_Updat" holding the ContactListUpdate Operations.
      //:// Thus we need to move the Operations from that source to the current dialog source.
      //:// We also need to reinclude the moved Operation into the corresponding Window Action.
      //:CreateViewFromView( CL_UpdatSrc, TZWINDOW )
      CreateViewFromView( &CL_UpdatSrc, TZWINDOW );
      //:NAME VIEW CL_UpdatSrc "CL_UpdatSrc"
      SetNameForView( CL_UpdatSrc, "CL_UpdatSrc", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST CL_UpdatSrc.SourceFile WHERE CL_UpdatSrc.SourceFile.Name = "CL_Updat"
      RESULT = SetCursorFirstEntityByString( CL_UpdatSrc, "SourceFile", "Name", "CL_Updat", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "Find PWD Source for 'CL_Updat' does not exist.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Find PWD Source for 'CL_Updat' does not exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:// Make sure SourceFile by name of Dialog exists and that we're positioned on it.
      //:SET CURSOR FIRST TZWINDOW.SourceFile WHERE TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZWINDOW, "Dialog", "Tag" );
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "SourceFile", "Name", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER );
         //:TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag
         SetAttributeFromAttribute( TZWINDOW, "SourceFile", "Name", TZWINDOW, "Dialog", "Tag" );
         //:TZWINDOW.SourceFile.Extension    = "VML"
         SetAttributeFromString( TZWINDOW, "SourceFile", "Extension", "VML" );
         //:TZWINDOW.SourceFile.LanguageType = "V"
         SetAttributeFromString( TZWINDOW, "SourceFile", "LanguageType", "V" );
      } 

      //:END
      //:FOR EACH CL_UpdatSrc.Operation 
      RESULT = SetCursorFirstEntity( CL_UpdatSrc, "Operation", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:szOperationName = CL_UpdatSrc.Operation.Name
         GetVariableFromAttribute( szOperationName, 0, 'S', 51, CL_UpdatSrc, "Operation", "Name", "", 0 );
         //:SET CURSOR FIRST TZWINDOW.Operation WHERE TZWINDOW.Operation.Name = szOperationName 
         RESULT = SetCursorFirstEntityByString( TZWINDOW, "Operation", "Name", szOperationName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZWINDOW.Operation
            RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOW, "Operation", CL_UpdatSrc, "Operation", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOW, "Operation", CL_UpdatSrc, "Operation", zSET_NULL );
            //:FOR EACH CL_UpdatSrc.Parameter 
            RESULT = SetCursorFirstEntity( CL_UpdatSrc, "Parameter", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
               CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZWINDOW, "Parameter", CL_UpdatSrc, "Parameter", zSET_NULL )
               SetMatchingAttributesByName( TZWINDOW, "Parameter", CL_UpdatSrc, "Parameter", zSET_NULL );
               RESULT = SetCursorNextEntity( CL_UpdatSrc, "Parameter", "" );
            } 

            //:END
            //:// If the Operation is for an Action in the new window, exclude the original and include the new.
            //:SET CURSOR FIRST CL_UpdatSrc.ActOper WITHIN CL_UpdatSrc.Window WHERE CL_UpdatSrc.ActOper.Name = szOperationName
            RESULT = SetCursorFirstEntityByString( CL_UpdatSrc, "ActOper", "Name", szOperationName, "Window" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:EXCLUDE CL_UpdatSrc.ActOper 
               RESULT = ExcludeEntity( CL_UpdatSrc, "ActOper", zREPOS_AFTER );
               //:INCLUDE CL_UpdatSrc.ActOper FROM TZWINDOW.Operation  
               RESULT = IncludeSubobjectFromSubobject( CL_UpdatSrc, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( CL_UpdatSrc, "Operation", "" );
         //:END 
      } 

      //:END
      //:DELETE ENTITY CL_UpdatSrc.SourceFile  
      RESULT = DeleteEntity( CL_UpdatSrc, "SourceFile", zPOS_NEXT );
      //:DropView( CL_UpdatSrc )
      DropView( CL_UpdatSrc );

      //:// Now copy the VML statements from the CL_Updat.VML file to the end of the current Dialog source, if they don't already exist.
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //:szCL_UpdatVML = szSourceDirectory + "\" + "CL_Updat.VML"
      ZeidonStringCopy( szCL_UpdatVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szCL_UpdatVML, 1, 0, "\\", 1, 0, 257 );
      ZeidonStringConcat( szCL_UpdatVML, 1, 0, "CL_Updat.VML", 1, 0, 257 );
      //:szFindVML    = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
      ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szFindVML, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
      //:// First check if they already exist there. We will do this by looking for the CANCEL_ContactListUpdate operation.
      //:nRC2 = -1
      nRC2 = -1;
      //:nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
      nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:zstrncpy( szCompare, szVML_Statement, 24 )    // Copy the 24 characters of the line to the compare field.
         zstrncpy( szCompare, szVML_Statement, 24 );
         //:nRC2 = zstrcmp( szCompare, "CANCEL_ContactListUpdate" )
         nRC2 = (zSHORT) zstrcmp( szCompare, "CANCEL_ContactListUpdate" );
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:IF nRC2 != 0
      if ( nRC2 != 0 )
      { 
         //:// The Operations are NOT already in the target so copy them.
         //:nFileIn   = SysOpenFile( TZWINDOW, szCL_UpdatVML, COREFILE_READ ) 
         nFileIn = SysOpenFile( TZWINDOW, szCL_UpdatVML, COREFILE_READ );
         //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
         nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
         //:LOOP WHILE nRC = 1 
         while ( nRC == 1 )
         { 
            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
            //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
            nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
         } 

         //:END
         //:SysCloseFile( TZWINDOW, nFileIn, 0 )
         SysCloseFile( TZWINDOW, nFileIn, 0 );
         //:SysCloseFile( TZWINDOW, nFileOut, 0 )
         SysCloseFile( TZWINDOW, nFileOut, 0 );
      } 

      //:END
   } 

   //:END

   //:// 2. Copy CRMFind window to create Find_AD_Area window.
   //://    The following code uses window CRMBase.CRMFind AND VML CRMBase.VML (which is copied and converted further down below).
   //:SET CURSOR FIRST CRMBase.Window WHERE CRMBase.Window.Tag = "CRMFind" 
   RESULT = SetCursorFirstEntityByString( CRMBase, "Window", "Tag", "CRMFind", "" );
   //:SET CURSOR LAST TZWINDOWL.Window  
   RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
   //:nRC = CloneWindow( TZWINDOWL, CRMBase, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, CRMBase, TaskLPLR, ViewToWindow );
   //:TraceLineI( "*** CloneWindow RC: ", nRC )
   TraceLineI( "*** CloneWindow RC: ", (zLONG) nRC );
   //:szWindowName = "Find" + szAD_Area
   ZeidonStringCopy( szWindowName, 1, 0, "Find", 1, 0, 51 );
   ZeidonStringConcat( szWindowName, 1, 0, szAD_Area, 1, 0, 51 );
   //:TZWINDOWL.Window.Tag = szWindowName
   SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );

   //:// The Find dialog should have the SourceFile by the same name.
   //:// If it doesn't, we'll create it here.
   //:SET CURSOR FIRST TZWINDOW.SourceFile WHERE TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZWINDOW, "Dialog", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "SourceFile", "Name", szTempString_1, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER );
      //:TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
      SetAttributeFromAttribute( TZWINDOW, "SourceFile", "Name", TZWINDOW, "Dialog", "Tag" );
      //:TZWINDOW.SourceFile.Extension    = "VML"
      SetAttributeFromString( TZWINDOW, "SourceFile", "Extension", "VML" );
      //:TZWINDOW.SourceFile.LanguageType = "V"
      SetAttributeFromString( TZWINDOW, "SourceFile", "LanguageType", "V" );
   } 

   //:END 

   //:// The Clone should create an Operation Source entry by the same name as the CRMBase dialog.
   //:// Thus we need to move the Operations from that source to the current dialog source.
   //:// We also need to reinclude the moved Operation into the corresponding Window Action.
   //:CreateViewFromView( CRMBaseSrc, TZWINDOW )
   CreateViewFromView( &CRMBaseSrc, TZWINDOW );
   //:NAME VIEW CRMBaseSrc "CRMBaseSrc"
   SetNameForView( CRMBaseSrc, "CRMBaseSrc", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST CRMBaseSrc.SourceFile WHERE CRMBaseSrc.SourceFile.Name = "CRMBase"
   RESULT = SetCursorFirstEntityByString( CRMBaseSrc, "SourceFile", "Name", "CRMBase", "" );
   //:FOR EACH CRMBaseSrc.Operation 
   RESULT = SetCursorFirstEntity( CRMBaseSrc, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = CRMBaseSrc.Operation.Name
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, CRMBaseSrc, "Operation", "Name", "", 0 );
      //:SET CURSOR FIRST TZWINDOW.Operation WHERE TZWINDOW.Operation.Name = szOperationName 
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "Operation", "Name", szOperationName, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR LAST TZWINDOW.Operation
         RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZWINDOW, "Operation", CRMBaseSrc, "Operation", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOW, "Operation", CRMBaseSrc, "Operation", zSET_NULL );
         //:FOR EACH CRMBaseSrc.Parameter 
         RESULT = SetCursorFirstEntity( CRMBaseSrc, "Parameter", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOW, "Parameter", CRMBaseSrc, "Parameter", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOW, "Parameter", CRMBaseSrc, "Parameter", zSET_NULL );
            RESULT = SetCursorNextEntity( CRMBaseSrc, "Parameter", "" );
         } 

         //:END
         //:// If the Operation is for an Action in the new window, exclude the original and include the new.
         //:SET CURSOR FIRST TZWINDOWL.ActOper WITHIN TZWINDOWL.Window WHERE TZWINDOWL.ActOper.Name = szOperationName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ActOper", "Name", szOperationName, "Window" );
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

      RESULT = SetCursorNextEntity( CRMBaseSrc, "Operation", "" );
      //:END 
   } 

   //:END
   //:DELETE ENTITY CRMBaseSrc.SourceFile  
   RESULT = DeleteEntity( CRMBaseSrc, "SourceFile", zPOS_NEXT );
   //:DropView( CRMBaseSrc )
   DropView( CRMBaseSrc );

   //:// 3. Convert _AD_Suff and _AD_Area characters in CRMFind Action.Tag and Operation.Name for Find_AD_Area window.
   //://    Also add Dialog and Window names to PICK__AD_Area Action
   //:FOR EACH TZWINDOWL.Action 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOWL.Action.Tag 
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
      //:IF szActionName = "PICK__AD_Area"
      if ( ZeidonStringCompare( szActionName, 1, 0, "PICK__AD_Area", 1, 0, 51 ) == 0 )
      { 
         //:TZWINDOWL.Action.DialogName = TZWINDOW.Dialog.Tag 
         SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOW, "Dialog", "Tag" );
         //:TZWINDOWL.Action.WindowName = szAD_Area + "Detail"
         ZeidonStringCopy( szTempString_1, 1, 0, szAD_Area, 1, 0, 33 );
         ZeidonStringConcat( szTempString_1, 1, 0, "Detail", 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_1 );
      } 

      //:END
      //:zSearchAndReplace( szActionName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szActionName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOWL.Action.Tag = szActionName
      SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
      RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
   } 

   //:END
   //:FOR EACH TZWINDOW.Operation 
   RESULT = SetCursorFirstEntity( TZWINDOW, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = TZWINDOW.Operation.Name 
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOW.Operation.Name = szOperationName
      SetAttributeFromString( TZWINDOW, "Operation", "Name", szOperationName );
      RESULT = SetCursorNextEntity( TZWINDOW, "Operation", "" );
   } 

   //:END

   //:// 4. Convert Grid, "GridContactLists", mapping View from lConListLST to lConList_AD_SuffLST.
   //:szViewObjRefName = "lConList" + szAD_Suffix + "LST"
   ZeidonStringCopy( szViewObjRefName, 1, 0, "lConList", 1, 0, 51 );
   ZeidonStringConcat( szViewObjRefName, 1, 0, szAD_Suffix, 1, 0, 51 );
   ZeidonStringConcat( szViewObjRefName, 1, 0, "LST", 1, 0, 51 );
   //:ChangeViewObjRef( ViewToWindow, TZWINDOW, TZWINDOWL, szViewObjRefName, "lConList", "GroupSavedContactLists" )
   o_ChangeViewObjRef( ViewToWindow, TZWINDOW, TZWINDOWL, szViewObjRefName, "lConList", "GroupSavedContactLists" );

   //:// 5. Convert Grid, "GroupSearchResultsList", mapping View from mConList to mConList_AD_SuffLST.
   //:szViewObjRefName = "mConList" + szAD_Suffix 
   ZeidonStringCopy( szViewObjRefName, 1, 0, "mConList", 1, 0, 51 );
   ZeidonStringConcat( szViewObjRefName, 1, 0, szAD_Suffix, 1, 0, 51 );
   //:ChangeViewObjRef( ViewToWindow, TZWINDOW, TZWINDOWL, szViewObjRefName, "mConList", "GroupSearchResults" )
   o_ChangeViewObjRef( ViewToWindow, TZWINDOW, TZWINDOWL, szViewObjRefName, "mConList", "GroupSearchResults" );

   //://    Make sure a ViewObjRef entry exists for the Returned LOD.
   //:szReturnedObjectName = ReturnedLOD.LOD.Name 
   GetVariableFromAttribute( szReturnedObjectName, 0, 'S', 51, ReturnedLOD, "LOD", "Name", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szReturnedObjectName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szReturnedObjectName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZWINDOW, ReturnedLOD, szReturnedObjectName )
      o_AddRegisteredViewName( TZWINDOW, ReturnedLOD, szReturnedObjectName );
   } 

   //:END

   //:// 6. Build the Search fields group from the selected attributes in TZADCSDO.FlatListSelectedAttribute.
   //://    In order to reuse the AutodesignUpdateCtrls operation that uses TZADCSDO.FlatListSelectedAttribute, we will copy the 
   //://    TZADWWKO.FlatSelectedSearchAttribute entries to create TZADCSDO.FlatListSelectedAttribute entries.

   //://    Begin by making sure a ViewObjRef entry exists for the Search LOD.
   //:szSearchObjectName = SearchLOD.LOD.Name 
   GetVariableFromAttribute( szSearchObjectName, 0, 'S', 51, SearchLOD, "LOD", "Name", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szSearchObjectName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szSearchObjectName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZWINDOW, SearchLOD, szSearchObjectName )
      o_AddRegisteredViewName( TZWINDOW, SearchLOD, szSearchObjectName );
   } 

   //:END

   //:// Convert the TZADWWKO.FlatSelectedSearchAttribute entries into TZADCSDO.FlatListSelectedAttribute entries.
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute  
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListSelectedAttribute NONE 
      RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADCSDO.FlatListSelectedAttribute 
      RESULT = CreateEntity( TZADCSDO, "FlatListSelectedAttribute", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL )  
      SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL );
      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   } 

   //:END

   //:// Build the AutodesignGroup subobject in TZWINDOW so that we can reuse the AutodesignUpdateCtrls code.
   //:szGroupTitle = TZWINDOWL.Control.Text 
   GetVariableFromAttribute( szGroupTitle, 0, 'S', 51, TZWINDOWL, "Control", "Text", "", 0 );
   //:SET CURSOR FIRST TZWINDOWL.Control WHERE TZWINDOWL.Control.Tag = "GroupSearchBy" 
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Control", "Tag", "GroupSearchBy", "" );
   //:IF TZWINDOWL.CtrlCtrl EXISTS
   lTempInteger_4 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
   if ( lTempInteger_4 == 0 )
   { 
      //:// The Group is 2-level, so go down a level.
      //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
      SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
      //:szGroupTitle = ""    // Change Title to null for second level group.
      ZeidonStringCopy( szGroupTitle, 1, 0, "", 1, 0, 51 );
   } 

   //:END
   //:IF TZWINDOW.AutoDesignWindow EXISTS
   lTempInteger_5 = CheckExistenceOfEntity( TZWINDOW, "AutoDesignWindow" );
   if ( lTempInteger_5 == 0 )
   { 
      //:DELETE ENTITY TZWINDOW.AutoDesignWindow  
      RESULT = DeleteEntity( TZWINDOW, "AutoDesignWindow", zPOS_NEXT );
   } 

   //:END
   //:CREATE ENTITY TZWINDOW.AutoDesignWindow 
   RESULT = CreateEntity( TZWINDOW, "AutoDesignWindow", zPOS_AFTER );
   //:TZWINDOW.AutoDesignWindow.WindowTag              = szWindowName 
   SetAttributeFromString( TZWINDOW, "AutoDesignWindow", "WindowTag", szWindowName );
   //:CREATE ENTITY TZWINDOW.AutoDesignGroup
   RESULT = CreateEntity( TZWINDOW, "AutoDesignGroup", zPOS_AFTER );
   //:TZWINDOW.AutoDesignGroup.GroupTag                = TZWINDOWL.Control.Tag 
   SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "GroupTag", TZWINDOWL, "Control", "Tag" );
   //:TZWINDOW.AutoDesignGroup.TempControlType         = ""
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "TempControlType", "" );
   //:TZWINDOW.AutoDesignGroup.GenerateGroupType       = ""     // Null is update fields. F is for display fields.
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "GenerateGroupType", "" );
   //:TZWINDOW.AutoDesignGroup.Title                   = ""     // The Title will be set to null because a non-null title will create a subgroup.
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "Title", "" );
   //:TZWINDOW.AutoDesignGroup.BaseWindowName          = "UpdateFields"
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "BaseWindowName", "UpdateFields" );
   //:TZWINDOW.AutoDesignGroup.TopLevelEntityName      = SearchLOD.LOD_EntityParent.Name 
   SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "TopLevelEntityName", SearchLOD, "LOD_EntityParent", "Name" );
   //:TZWINDOW.AutoDesignGroup.ActionNameSuffix        = szAD_Suffix
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "ActionNameSuffix", szAD_Suffix );
   //:TZWINDOW.AutoDesignGroup.ControlWidthInPixels    = TZWINDOWL.Control.SZDLG_X     // Use the current Group width.
   SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels", TZWINDOWL, "Control", "SZDLG_X" );
   //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = 15
   SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", 15 );

   //:// Build the AD_ListBoxEntity subobject which is what is used to generate the controls.
   //:IF TZWINDOW.AD_ListBoxEntity EXISTS
   lTempInteger_6 = CheckExistenceOfEntity( TZWINDOW, "AD_ListBoxEntity" );
   if ( lTempInteger_6 == 0 )
   { 
      //:DELETE ENTITY TZWINDOW.AD_ListBoxEntity
      RESULT = DeleteEntity( TZWINDOW, "AD_ListBoxEntity", zPOS_NEXT );
   } 

   //:END
   //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER )
   CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER );
   //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute   // We'll always create a AD_ListBoxEntity entity.
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   //:TZWINDOW.AD_ListBoxEntity.EntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
   SetAttributeFromAttribute( TZWINDOW, "AD_ListBoxEntity", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );

   //:// AutoDesign Group is for regular controls on a Groupbox.
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADCSDO.FlatListSelectedAttribute.AttributeName != ""
      if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "AttributeName", "" ) != 0 )
      { 
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER );
         //:TZWINDOW.AD_MappingAttribute.EntityName    = TZADCSDO.FlatListSelectedAttribute.EntityName
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
         //:TZWINDOW.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" );
         //:TZWINDOW.AD_MappingAttribute.ControlType   = TZADCSDO.FlatListSelectedAttribute.ControlType
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "ControlType", TZADCSDO, "FlatListSelectedAttribute", "ControlType" );
         //:TZWINDOW.AD_MappingAttribute.DataWidth     = TZADCSDO.FlatListSelectedAttribute.DataWidth
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "DataWidth", TZADCSDO, "FlatListSelectedAttribute", "DataWidth" );
         //:TZWINDOW.AD_MappingAttribute.PromptValue   = TZADCSDO.FlatListSelectedAttribute.PromptValue
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "PromptValue", TZADCSDO, "FlatListSelectedAttribute", "PromptValue" );
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      //:END
   } 

   //:END 
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "UpdateFields" 
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "UpdateFields", "" );
   //:AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_Base, SearchLOD )
   o_AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_Base, SearchLOD );

   //:// Readjust the position of each remaining GroupBox based on the size of the current GroupBox.
   //:// Third Control
   //:nNextPositionY = TZWINDOWL.Control.PSDLG_Y + TZWINDOWL.Control.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_7, TZWINDOWL, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_8, TZWINDOWL, "Control", "SZDLG_Y" );
   nNextPositionY = lTempInteger_7 + lTempInteger_8 + 10;
   //:SET CURSOR NEXT TZWINDOWL.Control 
   RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //: TZWINDOWL.Control.PSDLG_Y = nNextPositionY
      SetAttributeFromInteger( TZWINDOWL, "Control", "PSDLG_Y", nNextPositionY );
   } 

   //:END 
   //:// Fourth Control
   //:nNextPositionY = TZWINDOWL.Control.PSDLG_Y + TZWINDOWL.Control.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_9, TZWINDOWL, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_10, TZWINDOWL, "Control", "SZDLG_Y" );
   nNextPositionY = lTempInteger_9 + lTempInteger_10 + 10;
   //:SET CURSOR NEXT TZWINDOWL.Control 
   RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:TZWINDOWL.Control.PSDLG_Y = nNextPositionY
      SetAttributeFromInteger( TZWINDOWL, "Control", "PSDLG_Y", nNextPositionY );
   } 

   //:END

   //:// VML OPERATIONS FOR FIND

   //:// Copy VML if operation code is not already in the target VML.
   //:// We will check if it doesn't already exist by looking for the last operation currently in the Dialog in the VML (which was created in 2. above)..
   //:// If that operation is there, the others should also be there and we don't need to check for each operation separately.
   //:szOperationName   = TZWINDOW.Operation.Name 
   GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW, "Operation", "Name", "", 0 );
   //:nLength           = zGetStringLen( szOperationName )
   nLength = zGetStringLen( szOperationName );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szCRMBaseVML      = szSourceDirectory + "\" + "CRMBase.VML"
   ZeidonStringCopy( szCRMBaseVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szCRMBaseVML, 1, 0, "\\", 1, 0, 257 );
   ZeidonStringConcat( szCRMBaseVML, 1, 0, "CRMBase.VML", 1, 0, 257 );
   //:szFindVML         = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_2, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn           = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
   //:nRC2              = -1
   nRC2 = -1;
   //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
   nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   //:LOOP WHILE nRC = 1 AND nRC2 != 0
   while ( nRC == 1 && nRC2 != 0 )
   { 
      //:zstrncpy( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
      zstrncpy( szCompare, szVML_Statement, nLength );
      //:nRC2 = zstrncmpi( szCompare, szOperationName, nLength )
      nRC2 = zstrncmpi( szCompare, szOperationName, nLength );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   } 

   //:END
   //:SysCloseFile( TZWINDOW, nFileIn, 0 )
   SysCloseFile( TZWINDOW, nFileIn, 0 );
   //:IF nRC2 != 0
   if ( nRC2 != 0 )
   { 
      //:// The Operations are NOT already in the target, so copy them.

      //:// Copy the VML from CRMBase.VML and append it to the current Find VML
      //:// Copy the CRMBase VML to the end of the Find VML, converting the _AD_Area and _AD_Suff characters in the process.
      //:nFileIn   = SysOpenFile( TZWINDOW, szCRMBaseVML, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOW, szCRMBaseVML, COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
      //:szAD_SchN = TZADWWKO.W_MetaDefSearchObject.Name 
      GetVariableFromAttribute( szAD_SchN, 0, 'S', 51, TZADWWKO, "W_MetaDefSearchObject", "Name", "", 0 );
      //:szAD_RetN = szAD_SchN + "R"
      ZeidonStringCopy( szAD_RetN, 1, 0, szAD_SchN, 1, 0, 51 );
      ZeidonStringConcat( szAD_RetN, 1, 0, "R", 1, 0, 51 );
      //:szAD_RetE = TZADWWKO.LOD_EntityReturnedObjectInclude.Name   // Root entity of search object
      GetVariableFromAttribute( szAD_RetE, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
      //:szAD_RetA = szAD_RetN + "." + TZADWWKO.LOD_EntityCL_ItemDisplay.Name + "." + TZADWWKO.ER_AttributeCL_ItemDisplay.Name 
      ZeidonStringCopy( szAD_RetA, 1, 0, szAD_RetN, 1, 0, 51 );
      ZeidonStringConcat( szAD_RetA, 1, 0, ".", 1, 0, 51 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, TZADWWKO, "LOD_EntityCL_ItemDisplay", "Name", "", 0 );
      ZeidonStringConcat( szAD_RetA, 1, 0, szTempString_3, 1, 0, 51 );
      ZeidonStringConcat( szAD_RetA, 1, 0, ".", 1, 0, 51 );
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, TZADWWKO, "ER_AttributeCL_ItemDisplay", "Name", "", 0 );
      ZeidonStringConcat( szAD_RetA, 1, 0, szTempString_4, 1, 0, 51 );
      //:szAD_UpdO = TZADWWKO.W_MetaDefUpdateObject.Name 
      GetVariableFromAttribute( szAD_UpdO, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:szAD_UpdR = TZADWWKO.LOD_EntityUpdateObjectRoot.Name 
      GetVariableFromAttribute( szAD_UpdR, 0, 'S', 51, TZADWWKO, "LOD_EntityUpdateObjectRoot", "Name", "", 0 );

      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix )  // Suffix for mConList and lConList..LST
         zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area )    // Area name, such as "Person", "Driver" or "Student"
         zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN )    // Search object name such as fPerson
         zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN )    // Returned object name, same as szAD_RetN
         zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE )    // Returned object include entity name such as Person
         zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetA", szAD_RetA )    // Returned View.Entity.Attribute for display attribute
         zSearchAndReplace( szVML_Statement, 256, "_AD_RetA", szAD_RetA );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdO )    // Update object name such as mPerson
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdO );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdR", szAD_UpdR )    // Update object root entity name such as Person
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdR", szAD_UpdR );
         //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
         SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:SysCloseFile( TZWINDOW, nFileOut, 0 )
      SysCloseFile( TZWINDOW, nFileOut, 0 );
   } 

   //:END

   //:// Add any necessary VML statements for Range processing to the InitializeFindRangeSets operation.
   //://AddFindRangeInitializationValues( TZADWWKO, TaskLPLR, TZWINDOW )

   //:// Delete any xx that may have been created during generate.
   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.EntitySubGroup NONE  
      RESULT = DeleteEntity( TZADWWKO, "EntitySubGroup", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
   } 

   //:END

   //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
   //:             "CRM Generate Find Dialog is complete.",
   //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
   MessageSend( ViewToWindow, "", "Autodesign Subdialog", "CRM Generate Find Dialog is complete.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_ObjectFindSubdialog( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_ObjectFindSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADCSDO    REGISTERED AS TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW    BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZCONTROL   BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR    REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW FindBase    BASED ON LOD  TZWDLGSO
   zVIEW     FindBase = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW lConList    BASED ON LOD  TZZOLODO
   zVIEW     lConList = 0; 
   //:VIEW mConList    BASED ON LOD  TZZOLODO
   zVIEW     mConList = 0; 
   //:VIEW TZWDVORO    BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW FindBaseSrc BASED ON LOD  TZWDLGSO
   zVIEW     FindBaseSrc = 0; 
   //:STRING ( 50 )  szAD_Suffix
   zCHAR     szAD_Suffix[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSearchObjectName
   zCHAR     szSearchObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedObjectName
   zCHAR     szReturnedObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedRootEntityName
   zCHAR     szReturnedRootEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompareInit
   zCHAR     szCompareInit[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupTitle
   zCHAR     szGroupTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewObjRefName
   zCHAR     szViewObjRefName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchN  // Search object name such as fPerson
   zCHAR     szAD_SchN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetN  // Returned object name such as lPerson
   zCHAR     szAD_RetN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetE  // Returned object include Entity Name
   zCHAR     szAD_RetE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetA  // Returned View.Entity.Attribute for display attribute
   zCHAR     szAD_RetA[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdO  // Update object name such as mPerson
   zCHAR     szAD_UpdO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdR  // Update object root entity name such as Person
   zCHAR     szAD_UpdR[ 51 ] = { 0 }; 
   //:STRING ( 1 )   szCopyVML_Flag
   zCHAR     szCopyVML_Flag[ 2 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindBaseVML
   zCHAR     szFindBaseVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nNextPositionY
   zLONG     nNextPositionY = 0; 
   //:INTEGER nLength
   zLONG     nLength = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:SHORT   nRC2
   zSHORT    nRC2 = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zCHAR     szTempString_1[ 33 ]; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );

   //:// Build the Windows for Object Find and Area Maintenance from the AD_Base dialog, particularly the FindBase window template.
   //:// The build steps are as follows in adding windows to the current Dialog:
   //:// 1. Copy FindBase window to create Find_AD_Area window.
   //:// 2. Convert _AD_Suff and _AD_Area characters in ObjectFind Action.Tag and Operation.Name for Find_AD_Area window.
   //:// 3. Add the Controls and mapping for the GroupSearchReturnedList grid.e
   //:// 4. Build the Search fields group from the selected attributes in TZADWWKO.FlatSelectedSearchAttribute.

   //:// Initialize data.

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
   //:szAD_Suffix = TZADWWKO.AutodesignSubdialog.CRM_AreaSuffix 
   GetVariableFromAttribute( szAD_Suffix, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "CRM_AreaSuffix", "", 0 );
   //:szAD_Area   = TZADWWKO.AutodesignSubdialog.CRM_AreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "CRM_AreaName", "", 0 );
   //:IF szAD_Suffix = "" OR szAD_Area = ""
   if ( ZeidonStringCompare( szAD_Suffix, 1, 0, "", 1, 0, 51 ) == 0 || ZeidonStringCompare( szAD_Area, 1, 0, "", 1, 0, 51 ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both a CRM Area and a CRM Suffix must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a CRM Area and a CRM Suffix must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Search LOD.
   //:GET VIEW SearchLOD NAMED "TZZOLOD1"
   RESULT = GetViewByName( &SearchLOD, "TZZOLOD1", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Search Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Search Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:ReturnedLOD = SearchLOD
   ReturnedLOD = SearchLOD;

   //:// Check for Update LOD.
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

   //:// Check for other Update and Returned Object Data.
   //:IF TZADWWKO.LOD_EntityUpdateObjectRoot      DOES NOT EXIST OR
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityUpdateObjectRoot" );
   //:   TZADWWKO.LOD_EntityReturnedObjectInclude DOES NOT EXIST 
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityReturnedObjectInclude" );
   if ( lTempInteger_0 != 0 || lTempInteger_1 != 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "All Update and Returned Entity/Attribute entries must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "All Update and Returned Entity/Attribute entries must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure at least one Search Attribute has been selected.
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "At least one Search Attribute must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "At least one Search Attribute must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END


   //:// 1. Copy FindBase window to create Find_AD_Area window.
   //://    The following cod uses window AD_Base FindBase.VML (which is copied and converted further down below).
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:CreateViewFromView( FindBase, AD_Base )
   CreateViewFromView( &FindBase, AD_Base );
   //:NAME VIEW FindBase "FindBase"
   SetNameForView( FindBase, "FindBase", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST FindBase.Window WHERE FindBase.Window.Tag = "FindArea" 
   RESULT = SetCursorFirstEntityByString( FindBase, "Window", "Tag", "FindArea", "" );
   //:SET CURSOR LAST TZWINDOWL.Window  
   RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
   //:nRC = CloneWindow( TZWINDOWL, FindBase, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, FindBase, TaskLPLR, ViewToWindow );
   //:TraceLineI( "*** CloneWindow RC: ", nRC )
   TraceLineI( "*** CloneWindow RC: ", (zLONG) nRC );
   //:szWindowName = "Find" + szAD_Area
   ZeidonStringCopy( szWindowName, 1, 0, "Find", 1, 0, 51 );
   ZeidonStringConcat( szWindowName, 1, 0, szAD_Area, 1, 0, 51 );
   //:TZWINDOWL.Window.Tag = szWindowName
   SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );

   //:// The Find dialog should have the SourceFile by the same name.
   //:// If it doesn't, we'll create it here.
   //:SET CURSOR FIRST TZWINDOW.SourceFile WHERE TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZWINDOW, "Dialog", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "SourceFile", "Name", szTempString_0, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER );
      //:TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
      SetAttributeFromAttribute( TZWINDOW, "SourceFile", "Name", TZWINDOW, "Dialog", "Tag" );
      //:TZWINDOW.SourceFile.Extension    = "VML"
      SetAttributeFromString( TZWINDOW, "SourceFile", "Extension", "VML" );
      //:TZWINDOW.SourceFile.LanguageType = "V"
      SetAttributeFromString( TZWINDOW, "SourceFile", "LanguageType", "V" );
   } 

   //:END 

   //:// The Clone should create an Operation Source entry by the same name as the FindBase dialog.
   //:// Thus we need to move the Operations from that source to the current dialog source.
   //:// We also need to reinclude the moved Operation into the corresponding Window Action.
   //:CreateViewFromView( FindBaseSrc, TZWINDOW )
   CreateViewFromView( &FindBaseSrc, TZWINDOW );
   //:NAME VIEW FindBaseSrc "FindBaseSrc"
   SetNameForView( FindBaseSrc, "FindBaseSrc", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST FindBaseSrc.SourceFile WHERE FindBaseSrc.SourceFile.Name = "FindBase"
   RESULT = SetCursorFirstEntityByString( FindBaseSrc, "SourceFile", "Name", "FindBase", "" );
   //:FOR EACH FindBaseSrc.Operation 
   RESULT = SetCursorFirstEntity( FindBaseSrc, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = FindBaseSrc.Operation.Name
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, FindBaseSrc, "Operation", "Name", "", 0 );
      //:SET CURSOR FIRST TZWINDOW.Operation WHERE TZWINDOW.Operation.Name = szOperationName 
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "Operation", "Name", szOperationName, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR LAST TZWINDOW.Operation
         RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZWINDOW, "Operation", FindBaseSrc, "Operation", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOW, "Operation", FindBaseSrc, "Operation", zSET_NULL );
         //:FOR EACH FindBaseSrc.Parameter 
         RESULT = SetCursorFirstEntity( FindBaseSrc, "Parameter", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOW, "Parameter", FindBaseSrc, "Parameter", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOW, "Parameter", FindBaseSrc, "Parameter", zSET_NULL );
            RESULT = SetCursorNextEntity( FindBaseSrc, "Parameter", "" );
         } 

         //:END
         //:// If the Operation is for an Action in the new window, exclude the original and include the new.
         //:SET CURSOR FIRST TZWINDOWL.ActOper WITHIN TZWINDOWL.Window WHERE TZWINDOWL.ActOper.Name = szOperationName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ActOper", "Name", szOperationName, "Window" );
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

      RESULT = SetCursorNextEntity( FindBaseSrc, "Operation", "" );
      //:END 
   } 

   //:END
   //:DELETE ENTITY FindBaseSrc.SourceFile  
   RESULT = DeleteEntity( FindBaseSrc, "SourceFile", zPOS_NEXT );
   //:DropView( FindBaseSrc )
   DropView( FindBaseSrc );

   //:// 2. Convert _AD_Suff and _AD_Area characters in FindArea Action.Tag and Operation.Name for Find_AD_Area window.
   //://    Also add Dialog and Window names to PICK__AD_Area Action
   //:FOR EACH TZWINDOWL.Action 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOWL.Action.Tag 
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
      //:IF szActionName = "PICK__AD_Area"
      if ( ZeidonStringCompare( szActionName, 1, 0, "PICK__AD_Area", 1, 0, 51 ) == 0 )
      { 
         //:TZWINDOWL.Action.DialogName = TZWINDOW.Dialog.Tag 
         SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOW, "Dialog", "Tag" );
         //:TZWINDOWL.Action.WindowName = szAD_Area + "Detail"
         ZeidonStringCopy( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, "Detail", 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_0 );
      } 

      //:END
      //:zSearchAndReplace( szActionName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szActionName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOWL.Action.Tag = szActionName
      SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
      RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
   } 

   //:END
   //:FOR EACH TZWINDOW.Operation 
   RESULT = SetCursorFirstEntity( TZWINDOW, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = TZWINDOW.Operation.Name 
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOW.Operation.Name = szOperationName
      SetAttributeFromString( TZWINDOW, "Operation", "Name", szOperationName );
      RESULT = SetCursorNextEntity( TZWINDOW, "Operation", "" );
   } 

   //:END

   //://    Make sure a ViewObjRef entry exists for the Returned LOD.
   //:szReturnedObjectName = ReturnedLOD.LOD.Name 
   GetVariableFromAttribute( szReturnedObjectName, 0, 'S', 51, ReturnedLOD, "LOD", "Name", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szReturnedObjectName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szReturnedObjectName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZWINDOW, ReturnedLOD, szReturnedObjectName )
      o_AddRegisteredViewName( TZWINDOW, ReturnedLOD, szReturnedObjectName );
   } 

   //:END

   //:// 3. Build the Search fields group from the selected attributes in TZADCSDO.FlatListSelectedAttribute.
   //://    In order to reuse the AutodesignUpdateCtrls operation that uses TZADCSDO.FlatListSelectedAttribute, we will copy the 
   //://    TZADWWKO.FlatSelectedSearchAttribute entries to create TZADCSDO.FlatListSelectedAttribute entries.

   //://    Begin by making sure a ViewObjRef entry exists for the Search LOD.
   //:szSearchObjectName = SearchLOD.LOD.Name 
   GetVariableFromAttribute( szSearchObjectName, 0, 'S', 51, SearchLOD, "LOD", "Name", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szSearchObjectName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szSearchObjectName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZWINDOW, SearchLOD, szSearchObjectName )
      o_AddRegisteredViewName( TZWINDOW, SearchLOD, szSearchObjectName );
   } 

   //:END

   //:// Convert the TZADWWKO.FlatSelectedSearchAttribute entries into TZADCSDO.FlatListSelectedAttribute entries.
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute  
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListSelectedAttribute NONE 
      RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADCSDO.FlatListSelectedAttribute 
      RESULT = CreateEntity( TZADCSDO, "FlatListSelectedAttribute", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL )  
      SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_ALL );
      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   } 

   //:END

   //:// Build the AutodesignGroup subobject in TZWINDOW so that we can reuse the AutodesignUpdateCtrls code.
   //:szGroupTitle = TZWINDOWL.Control.Text 
   GetVariableFromAttribute( szGroupTitle, 0, 'S', 51, TZWINDOWL, "Control", "Text", "", 0 );
   //:SET CURSOR FIRST TZWINDOWL.Control WHERE TZWINDOWL.Control.Tag = "GroupSearchBy" 
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Control", "Tag", "GroupSearchBy", "" );
   //:IF TZWINDOWL.CtrlCtrl EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
   if ( lTempInteger_2 == 0 )
   { 
      //:// The Group is 2-level, so go down a level.
      //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
      SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
      //:szGroupTitle = ""    // Change Title to null for second level group.
      ZeidonStringCopy( szGroupTitle, 1, 0, "", 1, 0, 51 );
   } 

   //:END
   //:IF TZWINDOW.AutoDesignWindow EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( TZWINDOW, "AutoDesignWindow" );
   if ( lTempInteger_3 == 0 )
   { 
      //:DELETE ENTITY TZWINDOW.AutoDesignWindow  
      RESULT = DeleteEntity( TZWINDOW, "AutoDesignWindow", zPOS_NEXT );
   } 

   //:END
   //:CREATE ENTITY TZWINDOW.AutoDesignWindow 
   RESULT = CreateEntity( TZWINDOW, "AutoDesignWindow", zPOS_AFTER );
   //:TZWINDOW.AutoDesignWindow.WindowTag              = szWindowName 
   SetAttributeFromString( TZWINDOW, "AutoDesignWindow", "WindowTag", szWindowName );
   //:CREATE ENTITY TZWINDOW.AutoDesignGroup
   RESULT = CreateEntity( TZWINDOW, "AutoDesignGroup", zPOS_AFTER );
   //:TZWINDOW.AutoDesignGroup.GroupTag                = TZWINDOWL.Control.Tag 
   SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "GroupTag", TZWINDOWL, "Control", "Tag" );
   //:TZWINDOW.AutoDesignGroup.TempControlType         = ""
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "TempControlType", "" );
   //:TZWINDOW.AutoDesignGroup.GenerateGroupType       = ""     // Null is update fields. F is for display fields.
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "GenerateGroupType", "" );
   //:TZWINDOW.AutoDesignGroup.Title                   = ""     // The Title will be set to null because a non-null title will create a subgroup.
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "Title", "" );
   //:TZWINDOW.AutoDesignGroup.BaseWindowName          = "UpdateFields"
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "BaseWindowName", "UpdateFields" );
   //:TZWINDOW.AutoDesignGroup.TopLevelEntityName      = SearchLOD.LOD_EntityParent.Name 
   SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "TopLevelEntityName", SearchLOD, "LOD_EntityParent", "Name" );
   //:TZWINDOW.AutoDesignGroup.ActionNameSuffix        = szAD_Suffix
   SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "ActionNameSuffix", szAD_Suffix );
   //:TZWINDOW.AutoDesignGroup.ControlWidthInPixels    = TZWINDOWL.Control.SZDLG_X     // Use the current Group width.
   SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels", TZWINDOWL, "Control", "SZDLG_X" );
   //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = 15
   SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", 15 );

   //:// Build the AD_ListBoxEntity subobject which is what is used to generate the controls.
   //:IF TZWINDOW.AD_ListBoxEntity EXISTS
   lTempInteger_4 = CheckExistenceOfEntity( TZWINDOW, "AD_ListBoxEntity" );
   if ( lTempInteger_4 == 0 )
   { 
      //:DELETE ENTITY TZWINDOW.AD_ListBoxEntity
      RESULT = DeleteEntity( TZWINDOW, "AD_ListBoxEntity", zPOS_NEXT );
   } 

   //:END
   //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER )
   CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER );
   //:SET CURSOR FIRST TZADCSDO.FlatListSelectedAttribute   // We'll always create a AD_ListBoxEntity entity.
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   //:TZWINDOW.AD_ListBoxEntity.EntityName = TZADCSDO.FlatListSelectedAttribute.EntityName
   SetAttributeFromAttribute( TZWINDOW, "AD_ListBoxEntity", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );

   //:// AutoDesign Group is for regular controls on a Groupbox.
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADCSDO.FlatListSelectedAttribute.AttributeName != ""
      if ( CompareAttributeToString( TZADCSDO, "FlatListSelectedAttribute", "AttributeName", "" ) != 0 )
      { 
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER );
         //:TZWINDOW.AD_MappingAttribute.EntityName    = TZADCSDO.FlatListSelectedAttribute.EntityName
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" );
         //:TZWINDOW.AD_MappingAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" );
         //:TZWINDOW.AD_MappingAttribute.ControlType   = TZADCSDO.FlatListSelectedAttribute.ControlType
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "ControlType", TZADCSDO, "FlatListSelectedAttribute", "ControlType" );
         //:TZWINDOW.AD_MappingAttribute.DataWidth     = TZADCSDO.FlatListSelectedAttribute.DataWidth
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "DataWidth", TZADCSDO, "FlatListSelectedAttribute", "DataWidth" );
         //:TZWINDOW.AD_MappingAttribute.PromptValue   = TZADCSDO.FlatListSelectedAttribute.PromptValue
         SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "PromptValue", TZADCSDO, "FlatListSelectedAttribute", "PromptValue" );
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
      //:END
   } 

   //:END 
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "UpdateFields" 
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "UpdateFields", "" );
   //:AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_Base, SearchLOD )
   o_AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_Base, SearchLOD );

   //:// 4. Add the Controls and mapping for the GroupSearchReturnedList grid.
   //:szReturnedRootEntityName = TZADWWKO.LOD_EntityReturnedObjectInclude.Name
   GetVariableFromAttribute( szReturnedRootEntityName, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
   //:CreateViewFromView( TZCONTROL, TZWINDOWL )
   CreateViewFromView( &TZCONTROL, TZWINDOWL );
   //:NAME VIEW TZCONTROL "TZCONTROL_Ret"
   SetNameForView( TZCONTROL, "TZCONTROL_Ret", 0, zLEVEL_TASK );
   //:SET CURSOR LAST TZCONTROL.Control  
   RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
   //:AddAD_ListMapping( TZCONTROL, TZWINDOWL, ReturnedLOD, szReturnedRootEntityName, "ESG_List" )
   o_AddAD_ListMapping( TZCONTROL, TZWINDOWL, ReturnedLOD, szReturnedRootEntityName, "ESG_List" );
   //:DropView( TZCONTROL )
   DropView( TZCONTROL );
   //://ChangeViewObjRef( ViewToWindow, TZWINDOW, TZWINDOWL, szViewObjRefName, "mConList", "GroupSearchReturnedList" )

   //:// 5. Add VML Operations for Find.

   //:// Copy VML if operation code is not already in the target VML.
   //:// We will check if it doesn't already exist by looking for the last operation currently in the Dialog in the VML (which was created in 2. above)..
   //:// If that operation is there, the others should also be there and we don't need to check for each operation separately.
   //:szCopyVML_Flag    = ""
   ZeidonStringCopy( szCopyVML_Flag, 1, 0, "", 1, 0, 2 );
   //:szOperationName   = TZWINDOW.Operation.Name 
   GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW, "Operation", "Name", "", 0 );
   //:nLength           = zGetStringLen( szOperationName )
   nLength = zGetStringLen( szOperationName );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szFindBaseVML     = "c:\lplr\AD_Base\FindBase.VML"
   ZeidonStringCopy( szFindBaseVML, 1, 0, "c:\\lplr\\AD_Base\\FindBase.VML", 1, 0, 257 );
   //:szFindVML         = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_1, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn           = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
   //:IF nFileIn >= 0  
   if ( nFileIn >= 0 )
   { 
      //:// The file exists, so check if the operations already exist. We only need to check the one.
      //:nRC  = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:nRC2 = -1
      nRC2 = -1;
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:zstrncpy( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
         zstrncpy( szCompare, szVML_Statement, nLength );
         //:nRC2 = zstrncmpi( szCompare, szOperationName, nLength )
         nRC2 = zstrncmpi( szCompare, szOperationName, nLength );
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:IF nRC2 != 0
      if ( nRC2 != 0 )
      { 
         //:// The file exists but doesn't have operations, so set flag for copy and open output file for APPEND.
         //:szCopyVML_Flag = "Y"
         ZeidonStringCopy( szCopyVML_Flag, 1, 0, "Y", 1, 0, 2 );
         //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
         nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
         //:TraceLineI( "*** nFileOut Append: ", nFileOut )
         TraceLineI( "*** nFileOut Append: ", nFileOut );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// The file doesn't exist, so set flag for copy and open output file for WRITE.
      //:szCopyVML_Flag = "Y"
      ZeidonStringCopy( szCopyVML_Flag, 1, 0, "Y", 1, 0, 2 );
      //:nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE )
      nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE );
   } 

   //:END

   //:IF szCopyVML_Flag = "Y"
   if ( ZeidonStringCompare( szCopyVML_Flag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:// The Operations are NOT already in the target, so copy them.

      //:// Copy the VML from FindBase.VML and append it to the current Find VML
      //:// Copy the FindBase VML to the end of the Find VML, converting the _AD_Area and _AD_Suff characters in the process.
      //:// Also generate the Range Statement values as needed in place of the characters, "   InitializeFindRangeSets()", in the source VML.
      //:nFileIn   = SysOpenFile( TZWINDOW, szFindBaseVML, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOW, szFindBaseVML, COREFILE_READ );
      //:szAD_SchN = TZADWWKO.W_MetaDefSearchObject.Name 
      GetVariableFromAttribute( szAD_SchN, 0, 'S', 51, TZADWWKO, "W_MetaDefSearchObject", "Name", "", 0 );
      //:szAD_RetN = szAD_SchN + "R"
      ZeidonStringCopy( szAD_RetN, 1, 0, szAD_SchN, 1, 0, 51 );
      ZeidonStringConcat( szAD_RetN, 1, 0, "R", 1, 0, 51 );
      //:szAD_RetE = TZADWWKO.LOD_EntityReturnedObjectInclude.Name   // Root entity of search object
      GetVariableFromAttribute( szAD_RetE, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
      //:szAD_UpdO = TZADWWKO.W_MetaDefUpdateObject.Name 
      GetVariableFromAttribute( szAD_UpdO, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:szAD_UpdR = TZADWWKO.LOD_EntityUpdateObjectRoot.Name 
      GetVariableFromAttribute( szAD_UpdR, 0, 'S', 51, TZADWWKO, "LOD_EntityUpdateObjectRoot", "Name", "", 0 );

      //://    Make sure a ViewObjRef entry exists for the szAD_RetN view.
      //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szAD_RetN
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szAD_RetN, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
         //:AddRegisteredViewName( TZWINDOW, SearchLOD, szAD_RetN )
         o_AddRegisteredViewName( TZWINDOW, SearchLOD, szAD_RetN );
      } 

      //:END

      //:szCompareInit = "   InitializeFindRangeSets()"
      ZeidonStringCopy( szCompareInit, 1, 0, "   InitializeFindRangeSets()", 1, 0, 51 );
      //:nLength       = zGetStringLen( szCompareInit )
      nLength = zGetStringLen( szCompareInit );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:// Check for character string, "", and generate Range statements in its place.
         //:zstrncpy( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
         zstrncpy( szCompare, szVML_Statement, nLength );
         //:nRC2 = zstrncmpi( szCompare, szCompareInit, nLength )
         nRC2 = zstrncmpi( szCompare, szCompareInit, nLength );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:// Replace this statement with Range initialization statements.
            //:AddFindRangeInitializationValues( TZADWWKO, TZWINDOW, nFileOut )
            o_AddFindRangeInitializationValue( TZADWWKO, TZWINDOW, nFileOut );
            //:ELSE
         } 
         else
         { 
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix )  // Suffix for mConList and lConList..LST
            zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area )    // Area name, such as "Person", "Driver" or "Student"
            zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN )    // Search object name such as fPerson
            zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN )    // Returned object name, same as szAD_RetN
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE )    // Returned object include entity name such as Person
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdO )    // Update object name such as mPerson
            zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdO );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdR", szAD_UpdR )    // Update object root entity name such as Person
            zSearchAndReplace( szVML_Statement, 256, "_AD_UpdR", szAD_UpdR );
            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
         } 

         //:END
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:SysCloseFile( TZWINDOW, nFileOut, 0 )
      SysCloseFile( TZWINDOW, nFileOut, 0 );
   } 

   //:END

   //:// Readjust the position of the third GroupBox based on the size of the second GroupBox.
   //:SET CURSOR FIRST TZWINDOWL.Control 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:SET CURSOR NEXT TZWINDOWL.Control     // Now positioned on Search GroupBox.
   RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
   //:nNextPositionY = TZWINDOWL.Control.PSDLG_Y + TZWINDOWL.Control.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_5, TZWINDOWL, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_6, TZWINDOWL, "Control", "SZDLG_Y" );
   nNextPositionY = lTempInteger_5 + lTempInteger_6 + 10;
   //:SET CURSOR NEXT TZWINDOWL.Control 
   RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //: TZWINDOWL.Control.PSDLG_Y = nNextPositionY
      SetAttributeFromInteger( TZWINDOWL, "Control", "PSDLG_Y", nNextPositionY );
   } 

   //:END 

   //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
   //:             "CRM Generate Find Dialog is complete.",
   //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
   MessageSend( ViewToWindow, "", "Autodesign Subdialog", "CRM Generate Find Dialog is complete.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:SELECT_CRM_ReturnedObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_ReturnedObject( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   return( 0 );
//    
//    // Activate the selected ReturnedLOD and include it in the work object.
//    // It will also be named "TZZOLODO_Desc".
//    /*GET VIEW ReturnedLOD NAMED "TZZOLODO_Desc"
//    IF RESULT >= 0
//       DropObjectInstance( ReturnedLOD )
//    END
//    szLOD_Name = TZADWWKO.W_MetaDefReturnedObject.Name 
//    nRC = ActivateMetaOI_ByName( ViewToWindow, ReturnedLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
//    NAME VIEW ReturnedLOD "TZZOLODO_Desc"*/
// END
} 


//:DIALOG OPERATION
//:GOTO_AutodesignCRMSubdialog( VIEW ViewToWindow )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignCRMSubdialog( zVIEW     ViewToWindow )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_Base  BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW TZZOLFLO BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
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
      //:TraceLineS( "*** Start of ActivateAD_Base", "" )
      TraceLineS( "*** Start of ActivateAD_Base", "" );
      //:nRC = ActivateAD_Base( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_Base( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base"
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
      //:TraceLineS( "*** After of ActivateAD_Base", "" )
      TraceLineS( "*** After of ActivateAD_Base", "" );
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
   //:VIEW TZADCSDO  REGISTERED AS TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );

   //:// Add each selected Entity to the list of EntitySubGroup entries at the current position.

   //:CreateViewFromView( TZADWWKO2, TZADWWKO)
   CreateViewFromView( &TZADWWKO2, TZADWWKO );
   //:NAME VIEW  TZADWWKO2 "TZADWWKO2" 
   SetNameForView( TZADWWKO2, "TZADWWKO2", 0, zLEVEL_TASK );

   //:FOR EACH TZADCSDO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListPotentialEntity" )
      nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListPotentialEntity" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO2.ESG_LOD_Entity WITHIN TZADWWKO2.AutoDesignWork 
         //:           WHERE TZADWWKO2.ESG_LOD_Entity.ZKey = TZADCSDO.FlatListPotentialLOD_Entity.ZKey 
         GetIntegerFromAttribute( &lTempInteger_0, TZADCSDO, "FlatListPotentialLOD_Entity", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO2, "ESG_LOD_Entity", "ZKey", lTempInteger_0, "AutoDesignWork" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY TZADWWKO.EntitySubGroup 
            RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
            //:INCLUDE TZADWWKO.ESG_LOD_Entity FROM TZADCSDO.FlatListPotentialLOD_Entity 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_LOD_Entity", TZADCSDO, "FlatListPotentialLOD_Entity", zPOS_AFTER );
            //:SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialEntity", 0 )
            SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialEntity", 0 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialEntity", "" );
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
//:SELECT_CRM_DetailObject( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_DetailObject( zVIEW     ViewToWindow )
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
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_CRM_PotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_CRM_PotentialAttributes( zVIEW     ViewToWindow )
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

   //:lMaxPromptLength = 0
   lMaxPromptLength = 0;
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szTempString = TZADWWKO.FlatSelectedSearchAttribute.PromptValue
      GetVariableFromAttribute( szTempString, 0, 'S', 101, TZADWWKO, "FlatSelectedSearchAttribute", "PromptValue", "", 0 );
      //:lPromptLength = GetStringLength( szTempString )
      lPromptLength = GetStringLength( szTempString );
      //:IF lPromptLength > lMaxPromptLength
      if ( lPromptLength > lMaxPromptLength )
      { 
         //:lMaxPromptLength = lPromptLength
         lMaxPromptLength = lPromptLength;
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      //:END
   } 

   //:END
   //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = lMaxPromptLength
   SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", lMaxPromptLength );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_CRM_PotentialAttributes( VIEW ViewToWindow )

//:   VIEW TZADCSDO  REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
REMOVE_CRM_PotentialAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO  REGISTERED AS TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
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
         //:TZADWWKO.ESG_ListLOD_Attribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType 
         SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
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

      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:REMOVE_ESGL_ListPotAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_ListPotAttributes( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
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
         //:INCLUDE TZADWWKO.ESG_DetailLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute 
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_DetailLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute", zPOS_AFTER );
         //: // We will always use the Prompt value for Search.
         //:TZADWWKO.ESG_DetailLOD_Attribute.PromptValue         = TZADWWKO.ESG_FlatListPotentialAttribute.PromptValue 
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptValue" );
         //:TZADWWKO.ESG_DetailLOD_Attribute.PromptLength        = TZADWWKO.ESG_FlatListPotentialAttribute.PromptLength 
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute", "PromptLength" );
         //:TZADWWKO.ESG_DetailLOD_Attribute.ControlType         = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType 
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
         //:TZADWWKO.ESG_DetailLOD_Attribute.ControlWidth        = TZADWWKO.ESG_FlatListPotentialAttribute.ControlWidth
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlWidth" );
         //:TZADWWKO.ESG_DetailLOD_Attribute.CSS_ClassForPrompt  = TZADWWKO.CSS_ClassPageGroup.CSS_ClassPromptValue 
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "CSS_ClassForPrompt", TZADWWKO, "CSS_ClassPageGroup", "CSS_ClassPromptValue" );
         //:TZADWWKO.ESG_DetailLOD_Attribute.CSS_ClassForControl = TZADWWKO.CSS_ClassPageGroup.CSS_ClassControlValue 
         SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "CSS_ClassForControl", TZADWWKO, "CSS_ClassPageGroup", "CSS_ClassControlValue" );
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
//:REMOVE_ESGL_DetailPotAttributes( VIEW ViewToWindow )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
REMOVE_ESGL_DetailPotAttributes( zVIEW     ViewToWindow )
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
//:BuildAD_GroupPotList( VIEW TZADWWKO    BASED ON LOD TZADWWKO,
//:                      VIEW SelectedLOD BASED ON LOD TZZOLODO,
//:                      STRING ( 32 ) szStartEntityName,
//:                      STRING ( 1 ) szFlatListNameSuffix )

//:   VIEW RecursiveLOD BASED ON LOD TZZOLODO
static zSHORT
o_BuildAD_GroupPotList( zVIEW     TZADWWKO,
                        zVIEW     SelectedLOD,
                        zPCHAR    szStartEntityName,
                        zPCHAR    szFlatListNameSuffix )
{
   zVIEW     RecursiveLOD = 0; 


   //:// Begin the recursive calls to BuildAD_GroupPotListR to copy attributes recursively starting with szStartEntityName.
   //:CreateViewFromView( RecursiveLOD, SelectedLOD )
   CreateViewFromView( &RecursiveLOD, SelectedLOD );
   //:NAME VIEW RecursiveLOD "RecursiveLOD"
   SetNameForView( RecursiveLOD, "RecursiveLOD", 0, zLEVEL_TASK );
   //:BuildAD_GroupPotListR( TZADWWKO, SelectedLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, 1, 0, "" )
   o_BuildAD_GroupPotListR( TZADWWKO, SelectedLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, 1, 0, "" );
   //:DropView( RecursiveLOD )
   DropView( RecursiveLOD );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:AUTODESIGN_DetailDialog( VIEW ViewToWindow )

//:   VIEW TZADWWKO    REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_DetailDialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
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
   //:STRING ( 50 )  szAD_RetN 
   zCHAR     szAD_RetN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
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
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_7[ 255 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 255 ]; 
   zCHAR     szTempString_11[ 255 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zSHORT    lTempInteger_8; 
   zSHORT    lTempInteger_9; 
   zSHORT    lTempInteger_10; 
   zSHORT    lTempInteger_11; 
   zCHAR     szTempString_13[ 255 ]; 
   zCHAR     szTempString_14[ 33 ]; 
   zCHAR     szTempString_15[ 33 ]; 
   zSHORT    lTempInteger_12; 
   zSHORT    lTempInteger_13; 
   zSHORT    lTempInteger_14; 
   zSHORT    lTempInteger_15; 
   zCHAR     szTempString_16[ 33 ]; 
   zLONG     lTempInteger_16; 
   zLONG     lTempInteger_17; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );

   //:// Autodesign a Window and possible Subwindows based on the EntitySubGroups defined as follows:
   //:// On the first Window, build a list or detail group for each EntitySubGroup depending on the EntitySubType.
   //:// For each EntitySubGroup of type ListGroupWUpdatePage or ListGroupWDetailPage, also build a SubWindow based on the detail attribures.
   //:// GroupTypes are:
   //://   ListGroupOnly
   //://   ListGroupWUpdateControls
   //://   ListGroupWUpdatePage
   //://   ListGroupWDetailPage
   //://   DetailGroupUpdate
   //://   DetailGroupDisplay

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

   //:// Check for Update LOD.
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

   //:// Initialize the main Window
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ObjectUpdate"
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ObjectUpdate", "" );
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
   //:TZWINDOWL.Window.Tag = szWindowName
   SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );

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

   //:// The Clone above creates the Save and Cancel Actions and Operations, but they need to have their names converted.
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

   //:// Copy Save and Cancel Operations and tie them to the Action by the same name.
   //:// Also convert the _AD_Area characters in both source file and Dialog.
   //:// The Clone should create an Operation Source entry by the same name as the AD_Base dialog.
   //:// Thus we need to ....
   //:CreateViewFromView( AD_BaseSrc, TZWINDOW )
   CreateViewFromView( &AD_BaseSrc, TZWINDOW );
   //:NAME VIEW AD_BaseSrc "AD_BaseSrc"
   SetNameForView( AD_BaseSrc, "AD_BaseSrc", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST AD_BaseSrc.SourceFile WHERE AD_BaseSrc.SourceFile.Name = "ObjUpdat"
   RESULT = SetCursorFirstEntityByString( AD_BaseSrc, "SourceFile", "Name", "ObjUpdat", "" );
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
      } 

      RESULT = SetCursorNextEntity( AD_BaseSrc, "Operation", "" );
      //:END 
   } 

   //:END

   //:// Check if the Operations have already been copied to the main Source File. If not, copy them with the corrected names.

   //:// Check to see if the Operation, "SAVE__AD_Area", exists in the VML File, where _AD_Area is replaced by Area Name.
   //:// Note that the Operation Name starts in column 1.
   //:szCheckForOperation = "SAVE_" + szAD_Area
   ZeidonStringCopy( szCheckForOperation, 1, 0, "SAVE_", 1, 0, 257 );
   ZeidonStringConcat( szCheckForOperation, 1, 0, szAD_Area, 1, 0, 257 );
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
         zstrncpy( szCompare, szVML_Statement, nLength );
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
      //:// The Operation was not found in the Source File.
      //:// Copy the VML from ObjIncl.VML and append it to the current Find VML
      //:szAD_UpdObj       = TZADWWKO.W_MetaDefUpdateObject.Name
      GetVariableFromAttribute( szAD_UpdObj, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //://szOriginalVML     = szSourceDirectory + "\" + "ObjUpdat.VML"
      //:szOriginalVML     = "c:\lplr\AD_Base\ObjUpdat.VML"
      ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjUpdat.VML", 1, 0, 257 );
      //:szFindVML         = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
      ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szFindVML, 1, 0, szTempString_2, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );

      //:// Copy the ObjIncl VML to the end of the Find VML, converting the variable characters in the process.
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
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO",  szAD_UpdObj )    // Update Object Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdObj );

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
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ObjectUpdateGroups"
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ObjectUpdateGroups", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "ObjectUpdateGroups Window doesn't exist.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "ObjectUpdateGroups Window doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
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
   //:SET CURSOR LAST TZCONTROL.Control 
   RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
      GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
      //:TraceLineS( "*** szGroupType: ", szGroupType )
      TraceLineS( "*** szGroupType: ", szGroupType );

      //:// LIST GROUP OF ALL TYPES EXCEPT INCLUDE.

      //:IF szGroupType = "ListGroupOnly" OR 
      //:   szGroupType = "ListGroupWUpdateControls" OR 
      //:   szGroupType = "ListGroupWUpdatePage" OR 
      //:   szGroupType = "ListGroupWDetailPage"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdateControls", 1, 0, 51 ) == 0 ||
           ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 )
      { 

         //:// Build Group List with Update Page.
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupUpdateList"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupUpdateList", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
            //:             "ListGroupWUpdatePage Control doesn't exist.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Subdialog", "ListGroupWUpdatePage Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END

         //:// Build the Grid Control with subcontrols on the main window.
         //:SET CURSOR LAST TZCONTROL.Control    // Make sure we're adding to the end of the Group Controls. 
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl )
         nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl );

         //:// Set top Group Tag and Text.
         //:SET CURSOR LAST TZCONTROL.Control
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:szGroupName = TZADWWKO.EntitySubGroup.GroupDisplayArea
         GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupDisplayArea", "", 0 );
         //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "List"
         ZeidonStringCopy( szTempString_3, 1, 0, "Group", 1, 0, 33 );
         ZeidonStringConcat( szTempString_3, 1, 0, szGroupName, 1, 0, 33 );
         ZeidonStringConcat( szTempString_3, 1, 0, "List", 1, 0, 33 );
         SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_3 );
         //:TZCONTROL.Control.Text = "Group " + szGroupName + " List"
         ZeidonStringCopy( szTempString_4, 1, 0, "Group ", 1, 0, 255 );
         ZeidonStringConcat( szTempString_4, 1, 0, szGroupName, 1, 0, 255 );
         ZeidonStringConcat( szTempString_4, 1, 0, " List", 1, 0, 255 );
         SetAttributeFromString( TZCONTROL, "Control", "Text", szTempString_4 );

         //:// Process the 3 Actions tied to the New, Update and Delete buttons.
         //:AddAD_ListUpdateActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
         o_AddAD_ListUpdateActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );

         //:// Build the list mapping entries.
         //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
         GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
         //:AddAD_ListMapping( TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, "ESG_List" )
         o_AddAD_ListMapping( TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, "ESG_List" );

         //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
         nRC = ResetViewFromSubobject( TZCONTROL );
         //:LOOP WHILE nRC = 0
         while ( nRC == 0 )
         { 
            //:nRC = ResetViewFromSubobject( TZCONTROL )
            nRC = ResetViewFromSubobject( TZCONTROL );
         } 

         //:END

         //:// Go to name each of the Group Controls using main Attribute.
         //:nLevel = 1
         nLevel = 1;
         //:GenAD_GroupNames( TZCONTROL, 
         //:                  TZADWWKO.EntitySubGroup.GroupDisplayArea, 
         //:                  TZADWWKO.ESG_LOD_Entity.Name, nLevel )            
         GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
         GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_GenAD_GroupNames( TZCONTROL, szTempString_5, szTempString_6, nLevel );
      } 

      //:END

      //:// UPDATE OR DETAIL GROUP

      //:IF szGroupType = "DetailGroupUpdate" OR szGroupType = "DetailGroupDisplay"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupUpdate", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupDisplay", 1, 0, 51 ) == 0 )
      { 

         //:// Build Group List with Update Page.
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDetail"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDetail", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
            //:             "DetailGroupUpdate Control doesn't exist.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Subdialog", "DetailGroupUpdate Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END

         //:// Build the Detail control Group set.
         //:SET CURSOR LAST TZCONTROL.Control    // Make sure we're adding to the end of the Group Controls. 
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl )
         nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl );

         //:// 
         //:// Build the AutodesignGroup subobject in TZWINDOW so that we can reuse the AutodesignUpdateCtrls code.
         //:IF TZWINDOW.AutoDesignWindow EXISTS
         lTempInteger_2 = CheckExistenceOfEntity( TZWINDOW, "AutoDesignWindow" );
         if ( lTempInteger_2 == 0 )
         { 
            //:DELETE ENTITY TZWINDOW.AutoDesignWindow  
            RESULT = DeleteEntity( TZWINDOW, "AutoDesignWindow", zPOS_NEXT );
         } 

         //:END
         //:CREATE ENTITY TZWINDOW.AutoDesignWindow 
         RESULT = CreateEntity( TZWINDOW, "AutoDesignWindow", zPOS_AFTER );
         //:TZWINDOW.AutoDesignWindow.WindowTag              = TZWINDOWL.Window.Tag  
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignWindow", "WindowTag", TZWINDOWL, "Window", "Tag" );
         //:CREATE ENTITY TZWINDOW.AutoDesignGroup
         RESULT = CreateEntity( TZWINDOW, "AutoDesignGroup", zPOS_AFTER );
         //:TZWINDOW.AutoDesignGroup.GroupTag                = TZADWWKO.EntitySubGroup.GroupDisplayArea  
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "GroupTag", TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
         //:TZWINDOW.AutoDesignGroup.Title                   = ""    // Title needs to be null or a subgroup is generated to hold title.
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "Title", "" );
         //:TZWINDOW.AutoDesignGroup.TempControlType         = ""
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "TempControlType", "" );
         //:    
         //:TZWINDOW.AutoDesignGroup.BaseWindowName          = "UpdateFields"
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "BaseWindowName", "UpdateFields" );
         //:TZWINDOW.AutoDesignGroup.TopLevelEntityName      = TZADWWKO.ESG_LOD_Entity.Name 
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "TopLevelEntityName", TZADWWKO, "ESG_LOD_Entity", "Name" );
         //:TZWINDOW.AutoDesignGroup.ActionNameSuffix        = TZADWWKO.AutodesignSubdialog.AD_AreaSuffix 
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ActionNameSuffix", TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix" );
         //:TZWINDOW.AutoDesignGroup.ControlWidthInPixels    = TZWINDOWL.Control.SZDLG_X     // Use the current Group width.
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels", TZWINDOWL, "Control", "SZDLG_X" );
         //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = 15
         SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", 15 );
         //:TZWINDOW.AutoDesignGroup.GenerateGroupType    = ""
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "GenerateGroupType", "" );

         //:// Build the AD_ListBoxEntity subobject which is what is used to generate the controls.
         //:IF TZWINDOW.AD_ListBoxEntity EXISTS
         lTempInteger_3 = CheckExistenceOfEntity( TZWINDOW, "AD_ListBoxEntity" );
         if ( lTempInteger_3 == 0 )
         { 
            //:DELETE ENTITY TZWINDOW.AD_ListBoxEntity
            RESULT = DeleteEntity( TZWINDOW, "AD_ListBoxEntity", zPOS_NEXT );
         } 

         //:END
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER );
         //:TZWINDOW.AD_ListBoxEntity.EntityName = TZADWWKO.ESG_LOD_Entity.Name 
         SetAttributeFromAttribute( TZWINDOW, "AD_ListBoxEntity", "EntityName", TZADWWKO, "ESG_LOD_Entity", "Name" );

         //:// AutoDesign Group is for regular controls on a Groupbox.
         //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
         RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER );
            //:TZWINDOW.AD_MappingAttribute.EntityName    = TZADWWKO.ESG_DetailLOD_Entity.Name 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "EntityName", TZADWWKO, "ESG_DetailLOD_Entity", "Name" );
            //:TZWINDOW.AD_MappingAttribute.AttributeName = TZADWWKO.ESG_DetailER_Attribute.Name 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "AttributeName", TZADWWKO, "ESG_DetailER_Attribute", "Name" );
            //:TZWINDOW.AD_MappingAttribute.DataWidth     = ""
            SetAttributeFromString( TZWINDOW, "AD_MappingAttribute", "DataWidth", "" );
            //:TZWINDOW.AD_MappingAttribute.PromptValue   = TZADWWKO.ESG_DetailLOD_Attribute.PromptValue 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "PromptValue", TZADWWKO, "ESG_DetailLOD_Attribute", "PromptValue" );
            //:TZWINDOW.AD_MappingAttribute.ControlType   = TZADWWKO.ESG_DetailLOD_Attribute.ControlType 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "ControlType", TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType" );
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         } 

         //:END

         //:// Set up to reuse AutodesignUpdateCtrls to build the detail controls.
         //:SET CURSOR LAST TZWINDOWL.Control
         RESULT = SetCursorLastEntity( TZWINDOWL, "Control", "" );
         //:IF TZWINDOWL.CtrlCtrl EXISTS
         lTempInteger_4 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
         if ( lTempInteger_4 == 0 )
         { 
            //:// The Group is at least 2-level, so go down a level.
            //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
            SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
            //:IF TZWINDOWL.CtrlCtrl EXISTS
            lTempInteger_5 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
            if ( lTempInteger_5 == 0 )
            { 
               //:// The Group is 3-level, so go down a level.
               //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
               SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
            } 

            //:END
         } 

         //:END
         //:AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_BaseUC, UpdateLOD )
         o_AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_BaseUC, UpdateLOD );
         //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
         nRC = ResetViewFromSubobject( TZCONTROL );
         //:LOOP WHILE nRC = 0
         while ( nRC == 0 )
         { 
            //:nRC = ResetViewFromSubobject( TZCONTROL )
            nRC = ResetViewFromSubobject( TZCONTROL );
         } 

         //:END

         //:// Go to name each of the Group Controls using main Attribute.
         //:nLevel = 1
         nLevel = 1;
         //:GenAD_GroupNames( TZWINDOWL, 
         //:                  TZADWWKO.EntitySubGroup.GroupDisplayArea, 
         //:                  TZADWWKO.ESG_LOD_Entity.Name, nLevel )
         GetStringFromAttribute( szTempString_7, zsizeof( szTempString_7 ), TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
         GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_GenAD_GroupNames( TZWINDOWL, szTempString_7, szTempString_8, nLevel );
      } 

      //:END

      //:// LIST INCLUDE GROUP

      //:IF szGroupType = "ListGroupWIncludePage" 
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWIncludePage", 1, 0, 51 ) == 0 )
      { 

         //:// Build Group List with transfer to Select/Include Page.
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupIncludeList"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupIncludeList", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
            //:             "ListGroupWIncludePage Control doesn't exist.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Subdialog", "ListGroupWIncludePage Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END

         //:// Build the list control Group set.
         //:SET CURSOR LAST TZCONTROL.Control    // Make sure we're adding to the end of the Group Controls. 
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:nRC = CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl )
         nRC = o_CloneControlAD( ViewToWindow, TZCONTROL, TZWINDOWL, AD_BaseCtl );

         //:// Set top Group Tag and Text.
         //:SET CURSOR LAST TZCONTROL.Control
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         //:szGroupName = TZADWWKO.EntitySubGroup.GroupDisplayArea
         GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupDisplayArea", "", 0 );
         //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "List"
         ZeidonStringCopy( szTempString_9, 1, 0, "Group", 1, 0, 33 );
         ZeidonStringConcat( szTempString_9, 1, 0, szGroupName, 1, 0, 33 );
         ZeidonStringConcat( szTempString_9, 1, 0, "List", 1, 0, 33 );
         SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_9 );
         //:TZCONTROL.Control.Text = "Group " + szGroupName + " List"
         ZeidonStringCopy( szTempString_10, 1, 0, "Group ", 1, 0, 255 );
         ZeidonStringConcat( szTempString_10, 1, 0, szGroupName, 1, 0, 255 );
         ZeidonStringConcat( szTempString_10, 1, 0, " List", 1, 0, 255 );
         SetAttributeFromString( TZCONTROL, "Control", "Text", szTempString_10 );

         //:// Process the 2 Actions tied to the New and Delete buttons.
         //:AddAD_ListIncludeActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
         o_AddAD_ListIncludeActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );

         //:// Build the list mapping entries.
         //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
         GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
         //:TraceLineS( "*** Before AddAD_ListMapping List Group Include Update Page: ", szLOD_ListEntityName )
         TraceLineS( "*** Before AddAD_ListMapping List Group Include Update Page: ", szLOD_ListEntityName );
         //:AddAD_ListMapping( TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, "ESG_List" )
         o_AddAD_ListMapping( TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, "ESG_List" );

         //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
         nRC = ResetViewFromSubobject( TZCONTROL );
         //:LOOP WHILE nRC = 0
         while ( nRC == 0 )
         { 
            //:nRC = ResetViewFromSubobject( TZCONTROL )
            nRC = ResetViewFromSubobject( TZCONTROL );
         } 

         //:END

         //:// Go to name each of the Group Controls using main Attribute.
         //:nLevel = 1
         nLevel = 1;
         //:GenAD_GroupNames( TZCONTROL, 
         //:                  TZADWWKO.EntitySubGroup.GroupDisplayArea, 
         //:                  TZADWWKO.ESG_LOD_Entity.Name, nLevel )
         GetStringFromAttribute( szTempString_11, zsizeof( szTempString_11 ), TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
         GetStringFromAttribute( szTempString_12, zsizeof( szTempString_12 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_GenAD_GroupNames( TZCONTROL, szTempString_11, szTempString_12, nLevel );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
      //:END
   } 

   //:END

   //:// Readjust the position of each GroupBox based on the size and position of the previous GroupBox.
   //:nNextPositionY = 0
   nNextPositionY = 0;
   //:SET CURSOR FIRST TZWINDOWL.Control 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:FOR EACH TZWINDOWL.Control 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TZWINDOWL.Control.PSDLG_Y = nNextPositionY
      SetAttributeFromInteger( TZWINDOWL, "Control", "PSDLG_Y", nNextPositionY );
      //:nNextPositionY = TZWINDOWL.Control.PSDLG_Y + TZWINDOWL.Control.SZDLG_Y + 10
      GetIntegerFromAttribute( &lTempInteger_6, TZWINDOWL, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_7, TZWINDOWL, "Control", "SZDLG_Y" );
      nNextPositionY = lTempInteger_6 + lTempInteger_7 + 10;
      RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
   } 

   //:END 

   //:// Go to rename some Controls to avoid duplicate Tags.
   //:SET CURSOR FIRST TZWINDOWL.Control
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:RenameControls( TZWINDOWL ) 
   o_RenameControls( TZWINDOWL );

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

         //:// GROUP LIST DETAIL PAGE

         //:// First clone the GroupListUpdate Window.
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupListUpdate"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupListUpdate", "" );
         //:SET CURSOR LAST TZWINDOWL.Window  
         RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
         //:nRC = CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow )
         nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow );
         //:TraceLineI( "*** CloneWindow RC: ", nRC )
         TraceLineI( "*** CloneWindow RC: ", nRC );
         //:szWindowName = TZADWWKO.ESG_LOD_Entity.Name + "Detail"
         GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         ZeidonStringConcat( szWindowName, 1, 0, "Detail", 1, 0, 51 );
         //:TZWINDOWL.Window.Tag = szWindowName
         SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );

         //:// Generate the Update Controls.

         //:// Build the AutodesignGroup subobject in TZWINDOW so that we can reuse the AutodesignUpdateCtrls code.
         //:IF TZWINDOW.AutoDesignWindow EXISTS
         lTempInteger_8 = CheckExistenceOfEntity( TZWINDOW, "AutoDesignWindow" );
         if ( lTempInteger_8 == 0 )
         { 
            //:DELETE ENTITY TZWINDOW.AutoDesignWindow  
            RESULT = DeleteEntity( TZWINDOW, "AutoDesignWindow", zPOS_NEXT );
         } 

         //:END
         //:CREATE ENTITY TZWINDOW.AutoDesignWindow 
         RESULT = CreateEntity( TZWINDOW, "AutoDesignWindow", zPOS_AFTER );
         //:TZWINDOW.AutoDesignWindow.WindowTag              = TZWINDOWL.Window.Tag  
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignWindow", "WindowTag", TZWINDOWL, "Window", "Tag" );
         //:CREATE ENTITY TZWINDOW.AutoDesignGroup
         RESULT = CreateEntity( TZWINDOW, "AutoDesignGroup", zPOS_AFTER );
         //:TZWINDOW.AutoDesignGroup.GroupTag                = TZADWWKO.EntitySubGroup.GroupDisplayArea  
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "GroupTag", TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
         //:TZWINDOW.AutoDesignGroup.Title                   = ""    // Title needs to be null or a subgroup is generated to hold title.
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "Title", "" );
         //:TZWINDOW.AutoDesignGroup.TempControlType         = ""
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "TempControlType", "" );
         //:TZWINDOW.AutoDesignGroup.BaseWindowName          = "UpdateFields"
         SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "BaseWindowName", "UpdateFields" );
         //:TZWINDOW.AutoDesignGroup.TopLevelEntityName      = TZADWWKO.ESG_LOD_Entity.Name 
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "TopLevelEntityName", TZADWWKO, "ESG_LOD_Entity", "Name" );
         //:TZWINDOW.AutoDesignGroup.ActionNameSuffix        = TZADWWKO.AutodesignSubdialog.AD_AreaSuffix 
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ActionNameSuffix", TZADWWKO, "AutodesignSubdialog", "AD_AreaSuffix" );
         //:TZWINDOW.AutoDesignGroup.ControlWidthInPixels    = TZWINDOWL.Control.SZDLG_X     // Use the current Group width.
         SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels", TZWINDOWL, "Control", "SZDLG_X" );
         //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = 15
         SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", 15 );

         //:// Build the AD_ListBoxEntity subobject which is what is used to generate the controls.
         //:IF TZWINDOW.AD_ListBoxEntity EXISTS
         lTempInteger_9 = CheckExistenceOfEntity( TZWINDOW, "AD_ListBoxEntity" );
         if ( lTempInteger_9 == 0 )
         { 
            //:DELETE ENTITY TZWINDOW.AD_ListBoxEntity
            RESULT = DeleteEntity( TZWINDOW, "AD_ListBoxEntity", zPOS_NEXT );
         } 

         //:END
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER );
         //:TZWINDOW.AD_ListBoxEntity.EntityName = TZADWWKO.ESG_LOD_Entity.Name
         SetAttributeFromAttribute( TZWINDOW, "AD_ListBoxEntity", "EntityName", TZADWWKO, "ESG_LOD_Entity", "Name" );

         //:// AutoDesign Group is for regular controls on a Groupbox.
         //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
         RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER );
            //:TZWINDOW.AD_MappingAttribute.EntityName    = TZADWWKO.ESG_DetailLOD_Entity.Name 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "EntityName", TZADWWKO, "ESG_DetailLOD_Entity", "Name" );
            //:TZWINDOW.AD_MappingAttribute.AttributeName = TZADWWKO.ESG_DetailER_Attribute.Name 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "AttributeName", TZADWWKO, "ESG_DetailER_Attribute", "Name" );
            //:TZWINDOW.AD_MappingAttribute.DataWidth     = ""
            SetAttributeFromString( TZWINDOW, "AD_MappingAttribute", "DataWidth", "" );
            //:TZWINDOW.AD_MappingAttribute.PromptValue   = TZADWWKO.ESG_DetailLOD_Attribute.PromptValue 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "PromptValue", TZADWWKO, "ESG_DetailLOD_Attribute", "PromptValue" );
            //:TZWINDOW.AD_MappingAttribute.ControlType   = TZADWWKO.ESG_DetailLOD_Attribute.ControlType 
            SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "ControlType", TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType" );
            RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
         } 

         //:END

         //:// Set up to reuse AutodesignUpdateCtrls to build the detail controls.
         //:SET CURSOR LAST TZWINDOWL.Control
         RESULT = SetCursorLastEntity( TZWINDOWL, "Control", "" );
         //:IF TZWINDOWL.CtrlCtrl EXISTS
         lTempInteger_10 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
         if ( lTempInteger_10 == 0 )
         { 
            //:// The Group is at least 2-level, so go down a level.
            //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
            SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
            //:IF TZWINDOWL.CtrlCtrl EXISTS
            lTempInteger_11 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
            if ( lTempInteger_11 == 0 )
            { 
               //:// The Group is 3-level, so go down a level.
               //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
               SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
            } 

            //:END
         } 

         //:END
         //:// Note that AD_BaseUC will be positioned correctly within AutodesignUpdateCtrls.
         //:AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_BaseUC, UpdateLOD )
         o_AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_BaseUC, UpdateLOD );

         //:ResetViewFromSubobject( TZWINDOWL )  // Make sure we reset to the top.
         ResetViewFromSubobject( TZWINDOWL );
         //:ResetViewFromSubobject( TZWINDOWL )
         ResetViewFromSubobject( TZWINDOWL );

         //:// Go to name each of the Group Controls using main Attribute.
         //:nLevel = 1
         nLevel = 1;
         //:GenAD_GroupNames( TZWINDOWL, 
         //:                  TZADWWKO.EntitySubGroup.GroupDisplayArea, 
         //:                  TZADWWKO.ESG_LOD_Entity.Name, nLevel ) 
         GetStringFromAttribute( szTempString_13, zsizeof( szTempString_13 ), TZADWWKO, "EntitySubGroup", "GroupDisplayArea" );
         GetStringFromAttribute( szTempString_14, zsizeof( szTempString_14 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_GenAD_GroupNames( TZWINDOWL, szTempString_13, szTempString_14, nLevel );

         //:// Change mapping for ACCEPT_Subobject and CANCEL_Subobject Actions to include the ActMapLOD_Entity and ActMapView entities.
         //:SET CURSOR FIRST TZWINDOWL.ViewObjRef WHERE TZWINDOWL.ViewObjRef.Name = TZADWWKO.W_MetaDefUpdateObject.Name 
         GetStringFromAttribute( szTempString_15, zsizeof( szTempString_15 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ViewObjRef", "Name", szTempString_15, "" );
         //:FOR EACH TZWINDOWL.Action 
         RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:IF TZWINDOWL.ActMap EXISTS
            lTempInteger_12 = CheckExistenceOfEntity( TZWINDOWL, "ActMap" );
            if ( lTempInteger_12 == 0 )
            { 
               //:DELETE ENTITY TZWINDOWL.ActMap  
               RESULT = DeleteEntity( TZWINDOWL, "ActMap", zPOS_NEXT );
            } 

            //:END
            //:CREATE ENTITY TZWINDOWL.ActMap
            RESULT = CreateEntity( TZWINDOWL, "ActMap", zPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActMapLOD_Entity FROM TZADWWKO.ESG_LOD_Entity 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActMapLOD_Entity", TZADWWKO, "ESG_LOD_Entity", zPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActMapView FROM TZWINDOWL.ViewObjRef  
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
            RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
         } 

         //:END 

         //:ELSE
      } 
      else
      { 

         //:IF szGroupType = "ListGroupWIncludePage"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWIncludePage", 1, 0, 51 ) == 0 )
         { 

            //:// GROUP LIST INCLUDE PAGE

            //:// First clone the GroupListInclude Window.
            //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupListInclude"
            RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupListInclude", "" );
            //:SET CURSOR LAST TZWINDOWL.Window  
            RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
            //:nRC = CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow )
            nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow );
            //:TraceLineI( "*** CloneWindow RC: ", nRC )
            TraceLineI( "*** CloneWindow RC: ", nRC );
            //:szWindowName = TZADWWKO.ESG_LOD_Entity.Name + "Include"
            GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            ZeidonStringConcat( szWindowName, 1, 0, "Include", 1, 0, 51 );
            //:TZWINDOWL.Window.Tag = szWindowName
            SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );

            //:// Build the SearchBy Group.

            //:// Begin by activating the Search LOD and making sure a ViewObjRef entry exists for it.
            //:szSearchObjectName = TZADWWKO.ESG_ListSearchW_MetaDef.Name 
            GetVariableFromAttribute( szSearchObjectName, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
            //:GET VIEW SearchLOD NAMED "ESGL_ListSearchLOD"
            RESULT = GetViewByName( &SearchLOD, "ESGL_ListSearchLOD", ViewToWindow, zLEVEL_TASK );
            //:IF RESULT < 0
            if ( RESULT < 0 )
            { 
               //:nRC = ActivateMetaOI_ByName( ViewToWindow, SearchLOD, 0, zREFER_LOD_META, zSINGLE, szSearchObjectName, 0 )
               nRC = ActivateMetaOI_ByName( ViewToWindow, &SearchLOD, 0, zREFER_LOD_META, zSINGLE, szSearchObjectName, 0 );
               //:NAME VIEW SearchLOD "ESGL_ListSearchLOD"
               SetNameForView( SearchLOD, "ESGL_ListSearchLOD", 0, zLEVEL_TASK );
            } 

            //:END
            //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szSearchObjectName
            RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szSearchObjectName, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:// Activate the Registered View and include it in the Dialog
               //:nRC = ActivateMetaOI_ByName( ViewToWindow, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szSearchObjectName, 0 )
               nRC = ActivateMetaOI_ByName( ViewToWindow, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szSearchObjectName, 0 );
               //:NAME VIEW TZWDVORO "TZWDVORO_Search"
               SetNameForView( TZWDVORO, "TZWDVORO_Search", 0, zLEVEL_TASK );
               //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
               RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
               //:DropObjectInstance( TZWDVORO )
               DropObjectInstance( TZWDVORO );
            } 

            //:END 

            //:// Build the AutodesignGroup subobject in TZWINDOW so that we can reuse the AutodesignUpdateCtrls code. 
            //:SET CURSOR FIRST TZWINDOWL.Control WHERE TZWINDOWL.Control.Tag = "GroupSearchBy" 
            RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Control", "Tag", "GroupSearchBy", "" );
            //:IF TZWINDOWL.CtrlCtrl EXISTS
            lTempInteger_13 = CheckExistenceOfEntity( TZWINDOWL, "CtrlCtrl" );
            if ( lTempInteger_13 == 0 )
            { 
               //:// The Group is 2-level, so go down a level.
               //:SetViewToSubobject( TZWINDOWL, "CtrlCtrl" )
               SetViewToSubobject( TZWINDOWL, "CtrlCtrl" );
            } 

            //:END
            //:IF TZWINDOW.AutoDesignWindow EXISTS
            lTempInteger_14 = CheckExistenceOfEntity( TZWINDOW, "AutoDesignWindow" );
            if ( lTempInteger_14 == 0 )
            { 
               //:DELETE ENTITY TZWINDOW.AutoDesignWindow  
               RESULT = DeleteEntity( TZWINDOW, "AutoDesignWindow", zPOS_NEXT );
            } 

            //:END
            //:CREATE ENTITY TZWINDOW.AutoDesignWindow 
            RESULT = CreateEntity( TZWINDOW, "AutoDesignWindow", zPOS_AFTER );
            //:TZWINDOW.AutoDesignWindow.WindowTag              = szWindowName 
            SetAttributeFromString( TZWINDOW, "AutoDesignWindow", "WindowTag", szWindowName );
            //:CREATE ENTITY TZWINDOW.AutoDesignGroup
            RESULT = CreateEntity( TZWINDOW, "AutoDesignGroup", zPOS_AFTER );
            //:TZWINDOW.AutoDesignGroup.GroupTag                = TZWINDOWL.Control.Tag 
            SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "GroupTag", TZWINDOWL, "Control", "Tag" );
            //:TZWINDOW.AutoDesignGroup.TempControlType         = ""
            SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "TempControlType", "" );
            //:TZWINDOW.AutoDesignGroup.GenerateGroupType       = ""     // Null is update fields
            SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "GenerateGroupType", "" );
            //:TZWINDOW.AutoDesignGroup.Title                   = ""     // The Title will be set to null because a non-null title will create a subgroup.
            SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "Title", "" );
            //:TZWINDOW.AutoDesignGroup.BaseWindowName          = "UpdateFields"
            SetAttributeFromString( TZWINDOW, "AutoDesignGroup", "BaseWindowName", "UpdateFields" );
            //:TZWINDOW.AutoDesignGroup.TopLevelEntityName      = SearchLOD.LOD_EntityParent.Name 
            SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "TopLevelEntityName", SearchLOD, "LOD_EntityParent", "Name" );
            //://TZWINDOW.AutoDesignGroup.ActionNameSuffix        = szAD_Suffix
            //:TZWINDOW.AutoDesignGroup.ControlWidthInPixels    = TZWINDOWL.Control.SZDLG_X     // Use the current Group width.
            SetAttributeFromAttribute( TZWINDOW, "AutoDesignGroup", "ControlWidthInPixels", TZWINDOWL, "Control", "SZDLG_X" );
            //:TZWINDOW.AutoDesignGroup.UpdateFieldPromptLength = 15
            SetAttributeFromInteger( TZWINDOW, "AutoDesignGroup", "UpdateFieldPromptLength", 15 );

            //:// Build the AD_ListBoxEntity subobject which is what is used to generate the controls.
            //:IF TZWINDOW.AD_ListBoxEntity EXISTS
            lTempInteger_15 = CheckExistenceOfEntity( TZWINDOW, "AD_ListBoxEntity" );
            if ( lTempInteger_15 == 0 )
            { 
               //:DELETE ENTITY TZWINDOW.AD_ListBoxEntity
               RESULT = DeleteEntity( TZWINDOW, "AD_ListBoxEntity", zPOS_NEXT );
            } 

            //:END
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_ListBoxEntity", zPOS_AFTER );
            //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute 
            RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
            //:TZWINDOW.AD_ListBoxEntity.EntityName = TZADWWKO.ESG_DetailLOD_Entity.Name
            SetAttributeFromAttribute( TZWINDOW, "AD_ListBoxEntity", "EntityName", TZADWWKO, "ESG_DetailLOD_Entity", "Name" );

            //:// AutoDesign Group is for regular controls on a Groupbox.
            //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
            RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER )
               CreateMetaEntity( TZWINDOW, TZWINDOW, "AD_MappingAttribute", zPOS_AFTER );
               //:TZWINDOW.AD_MappingAttribute.EntityName    = TZADWWKO.ESG_DetailLOD_Entity.Name 
               SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "EntityName", TZADWWKO, "ESG_DetailLOD_Entity", "Name" );
               //:TZWINDOW.AD_MappingAttribute.AttributeName = TZADWWKO.ESG_DetailER_Attribute.Name 
               SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "AttributeName", TZADWWKO, "ESG_DetailER_Attribute", "Name" );
               //:TZWINDOW.AD_MappingAttribute.DataWidth     = ""
               SetAttributeFromString( TZWINDOW, "AD_MappingAttribute", "DataWidth", "" );
               //:TZWINDOW.AD_MappingAttribute.PromptValue   = TZADWWKO.ESG_DetailLOD_Attribute.PromptValue 
               SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "PromptValue", TZADWWKO, "ESG_DetailLOD_Attribute", "PromptValue" );
               //:TZWINDOW.AD_MappingAttribute.ControlType   = TZADWWKO.ESG_DetailLOD_Attribute.ControlType 
               SetAttributeFromAttribute( TZWINDOW, "AD_MappingAttribute", "ControlType", TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType" );
               RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
            } 

            //:END 
            //:AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_BaseUC, SearchLOD )
            o_AutodesignUpdateCtrls( TZWINDOW, TZWINDOWL, AD_BaseUC, SearchLOD );

            //:// Build the Returned Object List Group.

            //:CreateViewFromView( TZCONTROL, TZWINDOWL )    // TZWINDOWL is positioned on theTZCONTROL Window being built.
            CreateViewFromView( &TZCONTROL, TZWINDOWL );
            //:NAME VIEW TZCONTROL "TZCONTROL"
            SetNameForView( TZCONTROL, "TZCONTROL", 0, zLEVEL_TASK );

            //:// Begin by activating the Returned LOD and making sure a ViewObjRef entry exists for it.
            //:szReturnedObjectName = TZADWWKO.ESG_ListIncludeW_MetaDef.Name 
            GetVariableFromAttribute( szReturnedObjectName, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
            //:GET VIEW ReturnedLOD NAMED "ESGL_ListReturnedLOD"
            RESULT = GetViewByName( &ReturnedLOD, "ESGL_ListReturnedLOD", ViewToWindow, zLEVEL_TASK );
            //:IF RESULT < 0
            if ( RESULT < 0 )
            { 
               //:nRC = ActivateMetaOI_ByName( ViewToWindow, ReturnedLOD, 0, zREFER_LOD_META, zSINGLE, szReturnedObjectName, 0 )
               nRC = ActivateMetaOI_ByName( ViewToWindow, &ReturnedLOD, 0, zREFER_LOD_META, zSINGLE, szReturnedObjectName, 0 );
               //:NAME VIEW ReturnedLOD "ESGL_ListReturnedLOD"
               SetNameForView( ReturnedLOD, "ESGL_ListReturnedLOD", 0, zLEVEL_TASK );
            } 

            //:END
            //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szReturnedObjectName
            RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szReturnedObjectName, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
               //:AddRegisteredViewName( TZWINDOW, ReturnedLOD, szReturnedObjectName )
               o_AddRegisteredViewName( TZWINDOW, ReturnedLOD, szReturnedObjectName );
            } 

            //:END

            //:// Build the list mapping entries for the Returned Control.
            //:// We need to step to the next Control, which is the Group containing the select list.
            //:SET CURSOR NEXT TZCONTROL.Control    // Position on the list group. 
            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            //:szLOD_ListEntityName = ReturnedLOD.LOD_EntityParent.Name    // List Entity is Root Entity
            GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, ReturnedLOD, "LOD_EntityParent", "Name", "", 0 );
            //:TraceLineS( "*** Before AddAD_ListMapping List Group Include Update Page: ", szLOD_ListEntityName )
            TraceLineS( "*** Before AddAD_ListMapping List Group Include Update Page: ", szLOD_ListEntityName );
            //:AddAD_ListMapping( TZCONTROL, TZWINDOWL, ReturnedLOD, szLOD_ListEntityName, "ESG_List2" )
            o_AddAD_ListMapping( TZCONTROL, TZWINDOWL, ReturnedLOD, szLOD_ListEntityName, "ESG_List2" );

            //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
            nRC = ResetViewFromSubobject( TZCONTROL );
            //:LOOP WHILE nRC = 0
            while ( nRC == 0 )
            { 
               //:nRC = ResetViewFromSubobject( TZCONTROL )
               nRC = ResetViewFromSubobject( TZCONTROL );
            } 

            //:END

            //:// Set up Action and Operation variable values.
            //:szInclObjT = TZADWWKO.W_MetaDefUpdateObject.Name                  // Target Include Object Name is update object.
            GetVariableFromAttribute( szInclObjT, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
            //:szInclEntT = TZADWWKO.ESG_LOD_Entity.Name                         // Target Include Entity Name is used in include VML code
            GetVariableFromAttribute( szInclEntT, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
            //:szAD_SchN  = TZADWWKO.ESG_ListSearchW_MetaDef.Name                // Search Object Name is used in include VML code
            GetVariableFromAttribute( szAD_SchN, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
            //:szAD_RetN  = TZADWWKO.ESG_ListIncludeW_MetaDef.Name               // Returned / Source Include Object is used in include VML code
            GetVariableFromAttribute( szAD_RetN, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
            //:szInclEntS = TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName // Source Include Entity is used in include VML code
            GetVariableFromAttribute( szInclEntS, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", "", 0 );
            //:szInclSuff = TZADWWKO.EntitySubGroup.IncludeOperationSuffix       // Suffix to be added to the end of each Action and Operation.
            GetVariableFromAttribute( szInclSuff, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix", "", 0 );

            //:// Alter names for all Actions to add suffix _InclEnt, which is the Include Object Name.
            //:FOR EACH TZWINDOWL.Action 
            RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:szActionName = TZWINDOWL.Action.Tag
               GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
               //:zSearchAndReplace( szActionName, 50, "_InclSuff", szInclSuff )
               zSearchAndReplace( szActionName, 50, "_InclSuff", szInclSuff );
               //:TZWINDOWL.Action.Tag = szActionName 
               SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
               RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
            } 

            //:END

            //:// Alter names for the 3 Operations in Dialog.
            //:// Because the current Operations are under the Source entry, ObjIncl, from the CloneWindow, we will actually
            //:// create new Operations under the main Source entry and in the end delete the ObjIncl entry.
            //:SET CURSOR FIRST TZWINDOW.SourceFile  
            RESULT = SetCursorFirstEntity( TZWINDOW, "SourceFile", "" );
            //:SET CURSOR LAST TZWINDOW.Operation  
            RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
            //:CreateViewFromView( TZWINDOW2, TZWINDOW )
            CreateViewFromView( &TZWINDOW2, TZWINDOW );
            //:NAME VIEW TZWINDOW2 "TZWINDOW2"
            SetNameForView( TZWINDOW2, "TZWINDOW2", 0, zLEVEL_TASK );
            //:SET CURSOR FIRST TZWINDOW2.SourceFile WHERE TZWINDOW2.SourceFile.Name = "ObjIncl"
            RESULT = SetCursorFirstEntityByString( TZWINDOW2, "SourceFile", "Name", "ObjIncl", "" );
            //:FOR EACH TZWINDOW2.Operation 
            RESULT = SetCursorFirstEntity( TZWINDOW2, "Operation", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:szOperationName = TZWINDOW2.Operation.Name 
               GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW2, "Operation", "Name", "", 0 );
               //:zSearchAndReplace( szOperationName, 50, "_InclSuff", szInclSuff )
               zSearchAndReplace( szOperationName, 50, "_InclSuff", szInclSuff );
               //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
               CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZWINDOW, "Operation", TZWINDOW2, "Operation", zSET_NULL )
               SetMatchingAttributesByName( TZWINDOW, "Operation", TZWINDOW2, "Operation", zSET_NULL );
               //:FOR EACH TZWINDOW2.Parameter 
               RESULT = SetCursorFirstEntity( TZWINDOW2, "Parameter", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
                  CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
                  //:SetMatchingAttributesByName( TZWINDOW, "Parameter", TZWINDOW2, "Parameter", zSET_NULL )
                  SetMatchingAttributesByName( TZWINDOW, "Parameter", TZWINDOW2, "Parameter", zSET_NULL );
                  RESULT = SetCursorNextEntity( TZWINDOW2, "Parameter", "" );
               } 

               //:END
               //:TZWINDOW.Operation.Name = szOperationName
               SetAttributeFromString( TZWINDOW, "Operation", "Name", szOperationName );
               //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = szOperationName
               RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szOperationName, "" );
               //:EXCLUDE TZWINDOWL.ActOper 
               RESULT = ExcludeEntity( TZWINDOWL, "ActOper", zREPOS_AFTER );
               //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOW.Operation 
               RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
               RESULT = SetCursorNextEntity( TZWINDOW2, "Operation", "" );
            } 

            //:END
            //:DELETE ENTITY TZWINDOW2.SourceFile     // Delete the ObjIncl Source entry.
            RESULT = DeleteEntity( TZWINDOW2, "SourceFile", zPOS_NEXT );
            //:DropView( TZWINDOW2 )
            DropView( TZWINDOW2 );

            //:// Check if the Operations have already been copied to the main Source File. If not, copy them with the corrected names.
            //:// Check to see if the Operation, "EXEC_FindInclude_InclSuff", exists in the VML File, where _InclSuff is replaced by the specified Suffix.
            //:// Note that the Operation Name starts in column 1.
            //:szCheckForOperation = "EXEC_FindInclude" + szInclSuff
            ZeidonStringCopy( szCheckForOperation, 1, 0, "EXEC_FindInclude", 1, 0, 257 );
            ZeidonStringConcat( szCheckForOperation, 1, 0, szInclSuff, 1, 0, 257 );
            //:nLength = zGetStringLen( szCheckForOperation )
            nLength = zGetStringLen( szCheckForOperation );
            //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
            GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
            //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
            ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
            ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
            GetVariableFromAttribute( szTempString_15, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
            ZeidonStringConcat( szFindVML, 1, 0, szTempString_15, 1, 0, 257 );
            ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
            //:nFileIn   = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
            nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
            //:nRC2 = -1
            nRC2 = -1;
            //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
            nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
            //:LOOP WHILE nRC = 1 AND nRC2 != 0
            while ( nRC == 1 && nRC2 != 0 )
            { 
               //:zstrncpy( szCompare, szVML_Statement, nLength )
               zstrncpy( szCompare, szVML_Statement, nLength );
               //:nRC2 = zstrcmp( szCompare, szCheckForOperation )
               nRC2 = zstrcmp( szCompare, szCheckForOperation );
               //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
               nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
            } 

            //:END
            //:SysCloseFile( TZWINDOW, nFileIn, 0 )
            SysCloseFile( TZWINDOW, nFileIn, 0 );

            //:IF nRC2 != 0     // Operation was not found.
            if ( nRC2 != 0 )
            { 
               //:// Copy the VML from ObjIncl.VML and append it to the current Find VML
               //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
               GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
               //://szOriginalVML     = szSourceDirectory + "\" + "ObjIncl.VML"
               //:szOriginalVML     = "c:\lplr\AD_Base\ObjIncl.VML"
               ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjIncl.VML", 1, 0, 257 );
               //:szFindVML         = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
               ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
               ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
               GetVariableFromAttribute( szTempString_16, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
               ZeidonStringConcat( szFindVML, 1, 0, szTempString_16, 1, 0, 257 );
               ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );

               //:// Copy the ObjIncl VML to the end of the Find VML, converting the variable characters in the process.
               //:nFileIn   = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ ) 
               nFileIn = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ );
               //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
               nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
               //:IF nFileOut < 0
               if ( nFileOut < 0 )
               { 
                  //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE )
                  nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE );
               } 

               //:END
               //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
               nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
               //:LOOP WHILE nRC = 1 
               while ( nRC == 1 )
               { 
                  //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
                  zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
                  //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
                  zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
                  //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
                  zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN",  szAD_SchN )   // Search object name such as fPerson
                  zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN );
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetN )   // Returned / SourceInclude object name such as lPerson
                  zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN );
                  //:zSearchAndReplace( szVML_Statement, 256, "_InclSuff", szInclSuff )  // Operation Suffix such as "CombinedMail"
                  zSearchAndReplace( szVML_Statement, 256, "_InclSuff", szInclSuff );

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

            //:// Readjust the position of each GroupBox based on the size and position of the previous GroupBox.
            //:nNextPositionY = 0
            nNextPositionY = 0;
            //:SET CURSOR FIRST TZWINDOWL.Control 
            RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
            //:FOR EACH TZWINDOWL.Control 
            RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:TZWINDOWL.Control.PSDLG_Y = nNextPositionY
               SetAttributeFromInteger( TZWINDOWL, "Control", "PSDLG_Y", nNextPositionY );
               //:nNextPositionY = TZWINDOWL.Control.PSDLG_Y + TZWINDOWL.Control.SZDLG_Y + 10
               GetIntegerFromAttribute( &lTempInteger_16, TZWINDOWL, "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lTempInteger_17, TZWINDOWL, "Control", "SZDLG_Y" );
               nNextPositionY = lTempInteger_16 + lTempInteger_17 + 10;
               RESULT = SetCursorNextEntity( TZWINDOWL, "Control", "" );
            } 

            //:END

            //:DropView( TZCONTROL )
            DropView( TZCONTROL );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
      //:END
   } 


   //:END
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
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );

   //:// The window for transfer depends on the SubType of the Group.

   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
   GetVariableFromAttribute( szGroupType, 0, 'S', 31, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );

   //:IF szGroupType = "ListGroupOnly" OR szGroupType = "ListGroupWUpdateControls"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdateControls", 1, 0, 31 ) == 0 )
   { 
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListOnlySpec" )
      SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListOnlySpec" );
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "ListGroupWUpdatePage" OR szGroupType = "ListGroupWDetailPage"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 31 ) == 0 )
      { 
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwDetailSpec" )
         SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwDetailSpec" );
         //:ELSE
      } 
      else
      { 
         //:IF szGroupType = "DetailGroupUpdate" OR szGroupType = "DetailGroupDisplay" OR szGroupType = "DetailGroupInclude"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupUpdate", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupDisplay", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupInclude", 1, 0, 31 ) == 0 )
         { 
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailOnlySpec" )
            SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupDetailOnlySpec" );
            //:ELSE
         } 
         else
         { 
            //:IF szGroupType = "ListGroupWIncludePage" 
            if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWIncludePage", 1, 0, 31 ) == 0 )
            { 
               //:IF TZADWWKO.ESG_ListSearchW_MetaDef EXISTS
               lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
               if ( lTempInteger_0 == 0 )
               { 
                  //:EXCLUDE TZADWWKO.ESG_ListSearchW_MetaDef  
                  RESULT = ExcludeEntity( TZADWWKO, "ESG_ListSearchW_MetaDef", zREPOS_AFTER );
               } 

               //:END 
               //:IF TZADWWKO.ESG_ListIncludeW_MetaDef EXISTS
               lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef" );
               if ( lTempInteger_1 == 0 )
               { 
                  //:EXCLUDE TZADWWKO.ESG_ListIncludeW_MetaDef  
                  RESULT = ExcludeEntity( TZADWWKO, "ESG_ListIncludeW_MetaDef", zREPOS_AFTER );
               } 

               //:END 
               //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwIncludeSpec" )
               SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AD_GroupListwIncludeSpec" );
               //:ELSE
            } 
            else
            { 
               //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
               //:    "A Valid Type must be selected.",
               //:    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
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
   //:IF szGroupType = "ListGroupWIncludePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWIncludePage", 1, 0, 31 ) == 0 )
   { 
      //:InitializeUpdateGroupsForCSS( AD_Base, TZADWWKO, "GroupListInclude", "GroupSearchBy" )
      o_InitializeUpdateGroupsForCSS( AD_Base, TZADWWKO, "GroupListInclude", "GroupSearchBy" );
      //:SETUP_ESGL_MainIncludeObject( ViewToWindow )
      SETUP_ESGL_MainIncludeObject( ViewToWindow );
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "ListGroupWUpdatePage" OR szGroupType = "ListGroupWDetailPage"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 31 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 31 ) == 0 )
      { 
         //:InitializeUpdateGroupsForCSS( AD_Base, TZADWWKO, "GroupListUpdate", "GroupDetail" )
         o_InitializeUpdateGroupsForCSS( AD_Base, TZADWWKO, "GroupListUpdate", "GroupDetail" );
         //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
         RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
         //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_0, "" );
         //:ELSE
      } 
      else
      { 
         //:InitializeUpdateGroupsForCSS( AD_Base, TZADWWKO, "ObjectUpdateGroups", "GroupDetail" )
         o_InitializeUpdateGroupsForCSS( AD_Base, TZADWWKO, "ObjectUpdateGroups", "GroupDetail" );
         //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
         RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
         //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_1, "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:AddAD_ListUpdateActions( VIEW TZCONTROL  BASED ON LOD  TZWDLGSO,
//:                         VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                         VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO,
//:                         VIEW AD_Base    BASED ON LOD  TZWDLGSO )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
static zVOID
o_AddAD_ListUpdateActions( zVIEW     TZCONTROL,
                           zVIEW     TZWINDOWL,
                           zVIEW     AD_BaseCtl,
                           zVIEW     AD_Base )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", TZCONTROL, zLEVEL_TASK );

   //:// Process CtrlCtrl entries recursively and building Actions for AD New, Update and Delete entries.
   //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
   SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SET CURSOR FIRST TZCONTROL.Control  
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   //:FOR EACH AD_BaseCtl.Control
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF AD_BaseCtl.Control.Text = "New" OR AD_BaseCtl.Control.Text = "Update" OR AD_BaseCtl.Control.Text = "Delete"
      if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "New" ) == 0 || CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Update" ) == 0 || CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Delete" ) == 0 )
      { 
         //:// The Actions and related Events were created during the CloneControl for the whole update group.
         //:// So we only need to correct the subdialog/window names for New and Update and the mapping for Delete.
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = AD_BaseCtl.EventAct.Tag 
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZWINDOWL,0,0, "Programming Error: No Action match for Update Actions." )
            IssueError( TZWINDOWL, 0, 0, "Programming Error: No Action match for Update Actions." );
            //:RETURN 
            return;
         } 

         //:END

         //:// New and Update need subdialog/window name changes.
         //:// If the Action is New or Update, set the Dialog and Window names to the correct update Window.
         //:IF AD_BaseCtl.Control.Text = "New" OR AD_BaseCtl.Control.Text = "Update"
         if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "New" ) == 0 || CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Update" ) == 0 )
         { 
            //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
            SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
            //:TZWINDOWL.Action.WindowName = TZADWWKO.ESG_LOD_Entity.Name + "Detail"
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            ZeidonStringConcat( szTempString_0, 1, 0, "Detail", 1, 0, 33 );
            SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_0 );
         } 

         //:END

         //:// All 3 Controls need mapping change.
         //:IF TZWINDOWL.ActMap EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "ActMap" );
         if ( lTempInteger_0 == 0 )
         { 
            //:DELETE ENTITY TZWINDOWL.ActMap
            RESULT = DeleteEntity( TZWINDOWL, "ActMap", zPOS_NEXT );
         } 

         //:END
         //:CREATE ENTITY TZWINDOWL.ActMap 
         RESULT = CreateEntity( TZWINDOWL, "ActMap", zPOS_AFTER );
         //:INCLUDE TZWINDOWL.ActMapLOD_Entity FROM TZADWWKO.ESG_LOD_Entity 
         RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActMapLOD_Entity", TZADWWKO, "ESG_LOD_Entity", zPOS_AFTER );
         //:SET CURSOR FIRST TZWINDOWL.ViewObjRef WHERE TZWINDOWL.ViewObjRef.Name = TZADWWKO.W_MetaDefUpdateObject.Name 
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ViewObjRef", "Name", szTempString_1, "" );
         //:INCLUDE TZWINDOWL.ActMapView FROM TZWINDOWL.ViewObjRef 
         RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );

         //:// Add Suffix to Action Name.
         //:TZWINDOWL.Action.Tag = TZWINDOWL.Action.Tag + TZADWWKO.EntitySubGroup.IncludeOperationSuffix 
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZWINDOWL, "Action", "Tag" );
         GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix" );
         ZeidonStringConcat( szTempString_1, 1, 0, szTempString_2, 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "Tag", szTempString_1 );
         //:ELSE
      } 
      else
      { 
         //:IF AD_BaseCtl.CtrlCtrl EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
         if ( lTempInteger_1 == 0 )
         { 
            //:AddAD_ListUpdateActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
            o_AddAD_ListUpdateActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );
         } 

         //:END
      } 

      //:END
      //:SET CURSOR NEXT TZCONTROL.Control  
      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
   } 

   //:END
   //:ResetViewFromSubobject( AD_BaseCtl )
   ResetViewFromSubobject( AD_BaseCtl );
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   return;
// END
} 


//:LOCAL OPERATION
//:AddAD_ListIncludeActions( VIEW TZCONTROL  BASED ON LOD  TZWDLGSO,
//:                          VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                          VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO,
//:                          VIEW AD_Base    BASED ON LOD  TZWDLGSO )

//:   VIEW TZADWWKO REGISTERED AS TZADWWKO
static zSHORT
o_AddAD_ListIncludeActions( zVIEW     TZCONTROL,
                            zVIEW     TZWINDOWL,
                            zVIEW     AD_BaseCtl,
                            zVIEW     AD_Base )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", TZCONTROL, zLEVEL_TASK );

   //:// Process CrtlCrtl entries recursively for building Actions for AD New and Delete entries.
   //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
   SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SET CURSOR FIRST TZCONTROL.Control  
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   //:FOR EACH AD_BaseCtl.Control
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF AD_BaseCtl.Control.Text = "New" OR AD_BaseCtl.Control.Text = "Delete"
      if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "New" ) == 0 || CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Delete" ) == 0 )
      { 
         //:// Update the New Action if necessary and tie it to the Control.
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = AD_BaseCtl.EventAct.Tag 
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:TraceLineS( "*** No Action match on ", AD_BaseCtl.EventAct.Tag )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
            TraceLineS( "*** No Action match on ", szTempString_0 );
            //:IssueError( TZWINDOWL,0,0, "Programming Error: No Action match for Update Actions." )
            IssueError( TZWINDOWL, 0, 0, "Programming Error: No Action match for Update Actions." );
            //:RETURN 
            return( 0 );
         } 

         //:END

         //:// If the Action is New, set the Dialog and Window names to the correct update Window.
         //:IF AD_BaseCtl.Control.Text = "New" 
         if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "New" ) == 0 )
         { 
            //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
            SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
            //:TZWINDOWL.Action.WindowName = TZADWWKO.ESG_LOD_Entity.Name + "Include"
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            ZeidonStringConcat( szTempString_1, 1, 0, "Include", 1, 0, 33 );
            SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_1 );
         } 

         //:END

         //:// If the Action is Delete, build the ActMap entry to include the list LOD_Entity and View.
         //:IF AD_BaseCtl.Control.Text = "Delete" 
         if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Delete" ) == 0 )
         { 
            //:IF TZWINDOWL.ActMap EXISTS
            lTempInteger_0 = CheckExistenceOfEntity( TZWINDOWL, "ActMap" );
            if ( lTempInteger_0 == 0 )
            { 
               //:DELETE ENTITY TZWINDOWL.ActMap 
               RESULT = DeleteEntity( TZWINDOWL, "ActMap", zPOS_NEXT );
            } 

            //:END
            //:CREATE ENTITY TZWINDOWL.ActMap 
            RESULT = CreateEntity( TZWINDOWL, "ActMap", zPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActMapLOD_Entity FROM TZADWWKO.ESG_LOD_Entity 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActMapLOD_Entity", TZADWWKO, "ESG_LOD_Entity", zPOS_AFTER );
            //:SET CURSOR FIRST TZWINDOWL.ViewObjRef WHERE TZWINDOWL.ViewObjRef.Name = TZADWWKO.W_MetaDefUpdateObject.Name 
            GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
            RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ViewObjRef", "Name", szTempString_2, "" );
            //:INCLUDE TZWINDOWL.ActMapView FROM TZWINDOWL.ViewObjRef 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
         } 

         //:END

         //:// Add Suffix to Action Name.
         //:TZWINDOWL.Action.Tag = TZWINDOWL.Action.Tag + TZADWWKO.EntitySubGroup.IncludeOperationSuffix 
         GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZWINDOWL, "Action", "Tag" );
         GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix" );
         ZeidonStringConcat( szTempString_2, 1, 0, szTempString_3, 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "Tag", szTempString_2 );
         //:ELSE
      } 
      else
      { 
         //:IF AD_BaseCtl.CtrlCtrl EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
         if ( lTempInteger_1 == 0 )
         { 
            //:AddAD_ListIncludeActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
            o_AddAD_ListIncludeActions( TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );
         } 

         //:END
      } 

      //:END
      //:SET CURSOR NEXT TZCONTROL.Control  
      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
   } 

   //:END
   //:ResetViewFromSubobject( AD_BaseCtl )
   ResetViewFromSubobject( AD_BaseCtl );
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:AddAD_ListMapping( VIEW TZCONTROL  BASED ON LOD  TZWDLGSO,
//:                   VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                   VIEW MappingLOD BASED ON LOD  TZZOLODO,
//:                   STRING ( 10 ) szGridMappingEntityName,
//:                   STRING ( 10 ) szMappingPrefix )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
static zSHORT
o_AddAD_ListMapping( zVIEW     TZCONTROL,
                     zVIEW     TZWINDOWL,
                     zVIEW     MappingLOD,
                     zPCHAR    szGridMappingEntityName,
                     zPCHAR    szMappingPrefix )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZPESRCO  REGISTERED AS TZPESRCO
   zVIEW     TZPESRCO = 0; 
   //:VIEW vCtrl1Pos BASED ON LOD  TZWDLGSO
   zVIEW     vCtrl1Pos = 0; 
   //:STRING ( 32 ) szESG_ListLOD_Attribute
   zCHAR     szESG_ListLOD_Attribute[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szESG_ListER_Attribute
   zCHAR     szESG_ListER_Attribute[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szESG_ListLOD_Entity
   zCHAR     szESG_ListLOD_Entity[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szER_AttributeName
   zCHAR     szER_AttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szLOD_EntityName
   zCHAR     szLOD_EntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:STRING ( 50 ) szPrompt
   zCHAR     szPrompt[ 51 ] = { 0 }; 
   //:INTEGER nControlCount
   zLONG     nControlCount = 0; 
   //:INTEGER nPosition
   zLONG     nPosition = 0; 
   //:INTEGER nTotalLengths
   zLONG     nTotalLengths = 0; 
   //:INTEGER nGridLength
   zLONG     nGridLength = 0; 
   //:INTEGER nControlDiff
   zLONG     nControlDiff = 0; 
   //:INTEGER nTotalControls
   zLONG     nTotalControls = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zSHORT    lTempInteger_8; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", TZCONTROL, zLEVEL_TASK );
   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZCONTROL, zLEVEL_TASK );

   //:// The prefix (szMappingPrefix) passed allows this operation to handle any subobject with the ESG_ListLOD_Attribute structure.
   //:// In the code below the regular VML statement referencing the ESG_ListLOD_Attribute structure names is commented out and 
   //:// followed by a statement using a name created from the prefix, which are:
   //://    szESG_ListLOD_Attribute
   //://    szESG_ListER_Attribute
   //://    szESG_ListLOD_Entity

   //:NAME VIEW MappingLOD "MappingLOD"
   SetNameForView( MappingLOD, "MappingLOD", 0, zLEVEL_TASK );

   //:// Build names from szMappingPrefix.
   //:szESG_ListLOD_Attribute = szMappingPrefix + "LOD_Attribute"
   ZeidonStringCopy( szESG_ListLOD_Attribute, 1, 0, szMappingPrefix, 1, 0, 33 );
   ZeidonStringConcat( szESG_ListLOD_Attribute, 1, 0, "LOD_Attribute", 1, 0, 33 );
   //:szESG_ListER_Attribute  = szMappingPrefix + "ER_Attribute"
   ZeidonStringCopy( szESG_ListER_Attribute, 1, 0, szMappingPrefix, 1, 0, 33 );
   ZeidonStringConcat( szESG_ListER_Attribute, 1, 0, "ER_Attribute", 1, 0, 33 );
   //:szESG_ListLOD_Entity    = szMappingPrefix + "LOD_Entity"
   ZeidonStringCopy( szESG_ListLOD_Entity, 1, 0, szMappingPrefix, 1, 0, 33 );
   ZeidonStringConcat( szESG_ListLOD_Entity, 1, 0, "LOD_Entity", 1, 0, 33 );
   //:// Process CrtlCrtl entries recursively to find Grid Control. Then adding Grid mapping.
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:FOR EACH TZCONTROL.Control
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZCONTROL.ControlDef.Tag = "Grid"
      if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "Grid" ) == 0 )
      { 

         //:// We are positioned on the generated Grid Control.
         //:// We first need to change the Grid mapping to the object and entity being processed.
         //:GetStringFromAttribute( szLOD_Name, MappingLOD, "LOD", "Name" )
         GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), MappingLOD, "LOD", "Name" );
         //:SET CURSOR FIRST TZWINDOWL.ViewObjRef WHERE TZWINDOWL.ViewObjRef.Name = szLOD_Name  // This is the list Object Name
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ViewObjRef", "Name", szLOD_Name, "" );
         //:IF TZCONTROL.CtrlMap EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlMap" );
         if ( lTempInteger_0 == 0 )
         { 
            //:IF TZCONTROL.CtrlMapView EXISTS
            lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapView" );
            if ( lTempInteger_1 == 0 )
            { 
               //:EXCLUDE TZCONTROL.CtrlMapView 
               RESULT = ExcludeEntity( TZCONTROL, "CtrlMapView", zREPOS_AFTER );
               //:EXCLUDE TZCONTROL.CtrlMapLOD_Entity
               RESULT = ExcludeEntity( TZCONTROL, "CtrlMapLOD_Entity", zREPOS_AFTER );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
         } 

         //:END
         //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
         //:SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szGridMappingEntityName, "" )  // Position of List Entity
         SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szGridMappingEntityName, "" );
         //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Entity", MappingLOD, "LOD_Entity", zPOS_AFTER ) 
         IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Entity", MappingLOD, "LOD_Entity", zPOS_AFTER );
         //:nGridLength = TZCONTROL.Control.SZDLG_X 
         GetIntegerFromAttribute( &nGridLength, TZCONTROL, "Control", "SZDLG_X" );

         //:// Next, we now need to replace the first Grid item with those selected in ESG_ListLOD_Attribute
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" ) 
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:CreateViewFromView( vCtrl1Pos, TZCONTROL )   // vCtrl1Pos is positioned on first entry in Grid.
         CreateViewFromView( &vCtrl1Pos, TZCONTROL );
         //:NAME VIEW vCtrl1Pos "vCtrl1Pos"
         SetNameForView( vCtrl1Pos, "vCtrl1Pos", 0, zLEVEL_TASK );
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = vCtrl1Pos.ControlDef.Tag 
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vCtrl1Pos, "ControlDef", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_0, "" );
         //:nControlCount = 0
         nControlCount = 0;
         //:nRC = SetCursorFirstEntity( TZADWWKO, szESG_ListLOD_Attribute, "" )
         nRC = SetCursorFirstEntity( TZADWWKO, szESG_ListLOD_Attribute, "" );
         //:LOOP WHILE nRC >= zCURSOR_SET 
         while ( nRC >= zCURSOR_SET )
         { 
            //:nControlCount = nControlCount + 1
            nControlCount = nControlCount + 1;
            //:IF nControlCount = 1
            if ( nControlCount == 1 )
            { 
               //:// For first entry, remove current mapping if it exists or create CtrlMap.
               //:IF TZCONTROL.CtrlMap EXISTS
               lTempInteger_2 = CheckExistenceOfEntity( TZCONTROL, "CtrlMap" );
               if ( lTempInteger_2 == 0 )
               { 
                  //:IF TZCONTROL.CtrlMapView EXISTS
                  lTempInteger_3 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapView" );
                  if ( lTempInteger_3 == 0 )
                  { 
                     //:EXCLUDE TZCONTROL.CtrlMapView 
                     RESULT = ExcludeEntity( TZCONTROL, "CtrlMapView", zREPOS_AFTER );
                     //:EXCLUDE TZCONTROL.CtrlMapLOD_Attribute
                     RESULT = ExcludeEntity( TZCONTROL, "CtrlMapLOD_Attribute", zREPOS_AFTER );
                  } 

                  //:END
                  //:ELSE
               } 
               else
               { 
                  //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
                  CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:// After first entry we are creating Control..
               //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZCONTROL, "Control", vCtrl1Pos, "Control", zSET_NULL )
               SetMatchingAttributesByName( TZCONTROL, "Control", vCtrl1Pos, "Control", zSET_NULL );
               //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZCONTROL, "CtrlMap", vCtrl1Pos, "CtrlMap", zSET_NULL )
               SetMatchingAttributesByName( TZCONTROL, "CtrlMap", vCtrl1Pos, "CtrlMap", zSET_NULL );
               //:INCLUDE TZCONTROL.ControlDef FROM vCtrl1Pos.ControlDef
               RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", vCtrl1Pos, "ControlDef", zPOS_AFTER );
            } 

            //:END       

            //:// Position on list ctrl mapping LOD Entity and Attribute for CtrlMap include.
            //:GetStringFromAttribute( szER_AttributeName, TZADWWKO, szESG_ListER_Attribute, "Name" )
            GetStringFromAttribute( szER_AttributeName, zsizeof( szER_AttributeName ), TZADWWKO, szESG_ListER_Attribute, "Name" );
            //:GetStringFromAttribute( szLOD_EntityName,   TZADWWKO, szESG_ListLOD_Entity, "Name" )
            GetStringFromAttribute( szLOD_EntityName, zsizeof( szLOD_EntityName ), TZADWWKO, szESG_ListLOD_Entity, "Name" );
            //:TZCONTROL.Control.Tag  = "GridCtl" + szER_AttributeName 
            ZeidonStringCopy( szTempString_0, 1, 0, "GridCtl", 1, 0, 33 );
            ZeidonStringConcat( szTempString_0, 1, 0, szER_AttributeName, 1, 0, 33 );
            SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );

            //:GetStringFromAttribute( szPrompt, TZADWWKO, szESG_ListLOD_Attribute, "PromptValue" )
            GetStringFromAttribute( szPrompt, zsizeof( szPrompt ), TZADWWKO, szESG_ListLOD_Attribute, "PromptValue" );
            //:TZCONTROL.Control.Text = szPrompt 
            SetAttributeFromString( TZCONTROL, "Control", "Text", szPrompt );

            //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
            //:SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szLOD_EntityName, "" )  // Position of List Entity
            SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szLOD_EntityName, "" );
            //:SetCursorFirstEntityByString( MappingLOD, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" )  // Position of List Entity
            SetCursorFirstEntityByString( MappingLOD, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" );
            //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", MappingLOD, "LOD_Attribute", zPOS_AFTER )
            IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", MappingLOD, "LOD_Attribute", zPOS_AFTER );

            //:nRC = SetCursorNextEntity( TZADWWKO, szESG_ListLOD_Attribute, "" )
            nRC = SetCursorNextEntity( TZADWWKO, szESG_ListLOD_Attribute, "" );
         } 

         //:END
         //:DropView( vCtrl1Pos )
         DropView( vCtrl1Pos );

         //:// Determine the total of Control lengths in case we need to shrink them to make them fit on the Grid.
         //:nTotalLengths  = 0
         nTotalLengths = 0;
         //:nTotalControls = 0
         nTotalControls = 0;
         //:FOR EACH TZCONTROL.Control 
         RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:nTotalLengths  = nTotalLengths + TZCONTROL.Control.SZDLG_X 
            GetIntegerFromAttribute( &lTempInteger_4, TZCONTROL, "Control", "SZDLG_X" );
            nTotalLengths = nTotalLengths + lTempInteger_4;
            //:IF TZCONTROL.ControlDef.Tag = "GridEditCtl"
            if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "GridEditCtl" ) == 0 )
            { 
               //:nTotalControls = nTotalControls + 1
               nTotalControls = nTotalControls + 1;
            } 

            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            //:END
         } 

         //:END 
         //:IF nGridLength < nTotalLengths      // nGridLength was set earlier in this operation.
         if ( nGridLength < nTotalLengths )
         { 
            //:// We need to subtrack the difference from the length of each Control
            //:nControlDiff = ((nTotalLengths - nGridLength) / nTotalControls ) + 1    // Round up
            nControlDiff = ( ( nTotalLengths - nGridLength ) / nTotalControls ) + 1;
            //:FOR EACH TZCONTROL.Control 
            RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF TZCONTROL.ControlDef.Tag = "GridEditCtl"
               if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "GridEditCtl" ) == 0 )
               { 
                  //:TZCONTROL.Control.SZDLG_X = TZCONTROL.Control.SZDLG_X - nControlDiff
                  GetIntegerFromAttribute( &lTempInteger_5, TZCONTROL, "Control", "SZDLG_X" );
                  lTempInteger_6 = lTempInteger_5 - nControlDiff;
                  SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lTempInteger_6 );
               } 

               RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
               //:END
            } 

            //:END
         } 

         //:END

         //:// Compute the X position for each entry in the Grid.
         //:nPosition = 0
         nPosition = 0;
         //:FOR EACH TZCONTROL.Control 
         RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:TZCONTROL.Control.PSDLG_X = nPosition
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", nPosition );
            //:nPosition = nPosition + TZCONTROL.Control.SZDLG_X 
            GetIntegerFromAttribute( &lTempInteger_7, TZCONTROL, "Control", "SZDLG_X" );
            nPosition = nPosition + lTempInteger_7;
            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
         } 

         //:END    
         //:ELSE
      } 
      else
      { 
         //:IF TZCONTROL.CtrlCtrl EXISTS
         lTempInteger_8 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
         if ( lTempInteger_8 == 0 )
         { 
            //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
            SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:AddAD_ListMapping( TZCONTROL, TZWINDOWL, MappingLOD, szGridMappingEntityName, szMappingPrefix )
            o_AddAD_ListMapping( TZCONTROL, TZWINDOWL, MappingLOD, szGridMappingEntityName, szMappingPrefix );
            //:ResetViewFromSubobject( TZCONTROL )
            ResetViewFromSubobject( TZCONTROL );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      //:END  
   } 

   //:END
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GOTO_AutodesignCRM_DetailDialog( VIEW ViewToWindow )

//:   VIEW TZADCSDO  REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
GOTO_AutodesignCRM_DetailDialog( zVIEW     ViewToWindow )
{
   zVIEW     TZADCSDO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKO  REGISTERED AS TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
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
   //:FOR EACH TZADCSDO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListPotentialEntity NONE
      RESULT = DeleteEntity( TZADCSDO, "FlatListPotentialEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialEntity", "" );
   } 

   //:END

   //:// Create entry for each LOD_Entity in object.
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADCSDO.FlatListPotentialEntity 
      RESULT = CreateEntity( TZADCSDO, "FlatListPotentialEntity", zPOS_AFTER );
      //:TZADCSDO.FlatListPotentialEntity.EntityName = UpdateLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialEntity", "EntityName", UpdateLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADCSDO.FlatListPotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADCSDO, "FlatListPotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL );
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
               //:// If not a table, make control an Editbox.
               //:szControlType = "EditBox"
               ZeidonStringCopy( szControlType, 1, 0, "EditBox", 1, 0, 11 );
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


//:LOCAL OPERATION
//:GenAD_GroupNames( VIEW TZCONTROL BASED ON LOD  TZWDLGSO,
//:                  STRING ( 20 ) szGroupArea,
//:                  STRING ( 32 ) szEntityName,
//:                  INTEGER nLevel )

//:   STRING ( 1 ) szLevel
static zVOID
o_GenAD_GroupNames( zVIEW     TZCONTROL,
                    zPCHAR    szGroupArea,
                    zPCHAR    szEntityName,
                    zLONG     nLevel )
{
   zCHAR     szLevel[ 2 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 


   //:// Recursive routine to set Tag and Title of each Group set for Area generation.
   //:szLevel = nLevel
   ZeidonStringConvertFromNumber( szLevel, 1, 0, 1, nLevel, (ZDecimal) 0.0, "I" );
   //:IF nLevel = 1
   if ( nLevel == 1 )
   { 
      //:// Top level sets both Tag and Title.
      //:TZCONTROL.Control.Tag  = "Group" + szEntityName + szLevel
      ZeidonStringCopy( szTempString_0, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szEntityName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szLevel, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:TZCONTROL.Control.Text = szGroupArea
      SetAttributeFromString( TZCONTROL, "Control", "Text", szGroupArea );
      //:ELSE
   } 
   else
   { 
      //:// Other levels only set Tag.
      //:TZCONTROL.Control.Tag  = "Group" + szEntityName + szLevel
      ZeidonStringCopy( szTempString_1, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szEntityName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szLevel, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_1 );
   } 

   //:END

   //:// If CtrlCtrl exists and is GroupBox, also process it recursively.
   //:IF TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" ) 
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:IF TZCONTROL.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:nLevel = nLevel + 1
         nLevel = nLevel + 1;
         //:GenAD_GroupNames( TZCONTROL, szGroupArea, szEntityName, nLevel )
         o_GenAD_GroupNames( TZCONTROL, szGroupArea, szEntityName, nLevel );
      } 

      //:END
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
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
   //:// The potential Search attributes are defined in subobject ESG_FlatListPotentialAttribute2.
   //:// The selected Search attributes are defined in subobject ESG_DetailLOD_Attribute.
   //:// We will add them sequentially to the current Search position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute2 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute2" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_DetailLOD_Attribute  
         //:           WHERE TZADWWKO.ESG_DetailLOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute2.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute2", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_DetailLOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_DetailLOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_DetailLOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute2 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_DetailLOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute2", zPOS_AFTER );
            //:// We will always use the Prompt value for Search.
            //:TZADWWKO.ESG_DetailLOD_Attribute.PromptValue         = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptValue 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptValue" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.PromptLength        = TZADWWKO.ESG_FlatListPotentialAttribute2.PromptLength 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute2", "PromptLength" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.ControlType         = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlType" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.ControlWidth        = TZADWWKO.ESG_FlatListPotentialAttribute2.ControlWidth
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute2", "ControlWidth" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.CSS_ClassForPrompt  = TZADWWKO.CSS_ClassPageGroup.CSS_ClassPromptValue 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "CSS_ClassForPrompt", TZADWWKO, "CSS_ClassPageGroup", "CSS_ClassPromptValue" );
            //:TZADWWKO.ESG_DetailLOD_Attribute.CSS_ClassForControl = TZADWWKO.CSS_ClassPageGroup.CSS_ClassControlValue 
            SetAttributeFromAttribute( TZADWWKO, "ESG_DetailLOD_Attribute", "CSS_ClassForControl", TZADWWKO, "CSS_ClassPageGroup", "CSS_ClassControlValue" );
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
   //:// The selected Search attributes are defined in subobject ESG_DetailLOD_Attribute.
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
   //:// The potential Include attributes are defined in subobject ESG_FlatListPotentialAttribute3.
   //:// The selected Include attributes are defined in subobject ESG_List2LOD_Attribute.
   //:// We will add them sequentially to the current Include position 

   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute3 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3" )
      nRC = GetSelectStateOfEntity( TZADWWKO, "ESG_FlatListPotentialAttribute3" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:SET CURSOR FIRST TZADWWKO.ESG_List2LOD_Attribute  
         //:           WHERE TZADWWKO.ESG_List2LOD_Attribute.ZKey = TZADWWKO.ESG_FlatLOD_Attribute3.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, TZADWWKO, "ESG_FlatLOD_Attribute3", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( TZADWWKO, "ESG_List2LOD_Attribute", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_List2LOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_List2LOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_List2LOD_Attribute FROM TZADWWKO.ESG_FlatLOD_Attribute3 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_List2LOD_Attribute", TZADWWKO, "ESG_FlatLOD_Attribute3", zPOS_AFTER );
            //:// We will use the list value if it exists. Otherwise, we will use the Prompt value.
            //:IF TZADWWKO.ESG_FlatListPotentialAttribute3.ListTitleValue != ""
            if ( CompareAttributeToString( TZADWWKO, "ESG_FlatListPotentialAttribute3", "ListTitleValue", "" ) != 0 )
            { 
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute3.ListTitleValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute3", "ListTitleValue" );
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute3.ListTitleLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute3", "ListTitleLength" );
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptValue  = TZADWWKO.ESG_FlatListPotentialAttribute3.PromptValue 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptValue", TZADWWKO, "ESG_FlatListPotentialAttribute3", "PromptValue" );
               //:TZADWWKO.ESG_List2LOD_Attribute.PromptLength = TZADWWKO.ESG_FlatListPotentialAttribute3.PromptLength 
               SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "PromptLength", TZADWWKO, "ESG_FlatListPotentialAttribute3", "PromptLength" );
            } 

            //:END
            //:TZADWWKO.ESG_List2LOD_Attribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute3.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute3", "ControlType" );
            //:TZADWWKO.ESG_List2LOD_Attribute.ControlWidth = TZADWWKO.ESG_FlatListPotentialAttribute3.ControlWidth
            SetAttributeFromAttribute( TZADWWKO, "ESG_List2LOD_Attribute", "ControlWidth", TZADWWKO, "ESG_FlatListPotentialAttribute3", "ControlWidth" );
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
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Set up the potential attributes for the Main Include list object.

   //:// Get Main Update object view.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );

   //:// Make sure any existing mapping entries are deleted.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE 
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END

   //:// Build the FlatListPotentialAttribute entries from the subobject starting with LOD_EntityParent.
   //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
   o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_0, "" );
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
            //:TZADWWKO.ESG_ListLOD_Attribute.ControlType  = TZADWWKO.ESG_FlatListPotentialAttribute.ControlType 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", TZADWWKO, "ESG_FlatListPotentialAttribute", "ControlType" );
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
   //:VIEW TZADCSDO  BASED ON LOD  TZADCSDO
   zVIEW     TZADCSDO = 0; 
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

      //:// TZADCSDO object for holding search group mapping attributes.
      //:// We will always begin by deleting the existing object.
      //:GET VIEW TZADCSDO NAMED "TZADCSDO"
      RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
      //:IF RESULT >= 0
      if ( RESULT >= 0 )
      { 
         //:DropObjectInstance( TZADCSDO )
         DropObjectInstance( TZADCSDO );
      } 

      //:END
      //:ActivateEmptyMetaOI( ViewToWindow, TZADCSDO, zSOURCE_UIS_META, zSINGLE )
      ActivateEmptyMetaOI( ViewToWindow, &TZADCSDO, zSOURCE_UIS_META, zSINGLE );
      //:NAME VIEW TZADCSDO "TZADCSDO"
      SetNameForView( TZADCSDO, "TZADCSDO", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADCSDO.UI_Spec
      RESULT = CreateEntity( TZADCSDO, "UI_Spec", zPOS_AFTER );

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
      //:nRC = ActivateAD_Base( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_Base( &AD_Base, ViewToWindow );
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
   //:VIEW TZADCSDO  REGISTERED AS TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );

   //:// Just return if nothing was selected.
   //:IF TZADWWKO.W_MetaDefUpdateObject DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
   if ( lTempInteger_0 != 0 )
   { 
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Activate the selected Detail LOD and name it TZZOLODO_Update. 
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

   //:// Build the FlatListPotentialEntity entries from the update object just activated.
   //:// We have one entry in the list for each Entity in the Update Object.

   //:// First delete any current entries then rebuild them.
   //:FOR EACH TZADCSDO.FlatListPotentialEntity 
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADCSDO.FlatListPotentialEntity NONE
      RESULT = DeleteEntity( TZADCSDO, "FlatListPotentialEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialEntity", "" );
   } 

   //:END
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADCSDO.FlatListPotentialEntity 
      RESULT = CreateEntity( TZADCSDO, "FlatListPotentialEntity", zPOS_AFTER );
      //:TZADCSDO.FlatListPotentialEntity.EntityName = UpdateLOD.LOD_Entity.Name 
      SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialEntity", "EntityName", UpdateLOD, "LOD_Entity", "Name" );
      //:INCLUDE TZADCSDO.FlatListPotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADCSDO, "FlatListPotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL )
      SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialEntity", UpdateLOD, "LOD_Entity", zSET_NULL );
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

//:   VIEW TZWINDOWL REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
PostbuildAutodesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW TZZOLFLO  BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW TempCtrl  BASED ON LOD  TZWDLGSO
   zVIEW     TempCtrl = 0; 
   //:VIEW TZADWWKO  BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKO = 0; 
   //:VIEW TZADCSDO  BASED ON LOD  TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
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
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_1[ 255 ]; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );
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
      //:CREATE ENTITY TZADWWKO.EntitySubGroup  
      RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );

      //:// Default state will be resize and reposition top Groups.
      //:TZADWWKO.AutoDesignWork.RepositionGroupFlag = "Y"
      SetAttributeFromString( TZADWWKO, "AutoDesignWork", "RepositionGroupFlag", "Y" );

      //:// GroupDisplayArea is set from Text on outmost Control.
      //:CreateViewFromView( TempCtrl, TZCONTROL )
      CreateViewFromView( &TempCtrl, TZCONTROL );
      //:NAME VIEW TempCtrl "TempCtrl"
      SetNameForView( TempCtrl, "TempCtrl", 0, zLEVEL_TASK );
      //:ResetViewFromSubobject( TempCtrl )
      ResetViewFromSubobject( TempCtrl );
      //:ResetViewFromSubobject( TempCtrl )
      ResetViewFromSubobject( TempCtrl );
      //:TZADWWKO.EntitySubGroup.GroupDisplayArea = TempCtrl.Control.Text 
      SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "GroupDisplayArea", TempCtrl, "Control", "Text" );
      //:DropView( TempCtrl )
      DropView( TempCtrl );

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

      //:// If the current Group has a subcontrol that is not a GroupBox, then set the flag to indicate
      //:// the Autodesign is a redesign of the Group and set up the object fields and mapping. 
      //:// Also delete the current controls that do not have Actions for for UpdateGroupOnly.
      //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = ""
      SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "" );
      //:IF TZCONTROL.CtrlCtrl EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
      if ( lTempInteger_1 == 0 )
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
                  lTempInteger_3 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Attribute" );
                  if ( lTempInteger_3 == 0 )
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
               //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
               RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE  
                  RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
                  RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
               } 

               //:END
               //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
               GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
               o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_0, "" );

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
               lTempInteger_4 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Attribute" );
               if ( lTempInteger_4 != 0 )
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
               lTempInteger_5 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
               if ( lTempInteger_5 == 0 )
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
                  lTempInteger_6 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Attribute" );
                  if ( lTempInteger_6 != 0 )
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
               //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
               RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE  
                  RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
                  RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
               } 

               //:END   
               //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )    
               GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), UpdateLOD, "LOD_EntityParent", "Name" );
               o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_1, "" );
            } 

            //:      
            //:END
         } 

         //:END
         //:ResetViewFromSubobject( TZCONTROL )
         ResetViewFromSubobject( TZCONTROL );
      } 

      //:END
   } 


   //:END

   //:// Make sure AD_Base dialog is loaded.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:nRC = ActivateAD_Base( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_Base( &AD_Base, ViewToWindow );
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

   //:// Accept the current Control because it is the GroupBox that should be versioned.
   //:AcceptSubobject( TZCONTROL, "Control" )
   AcceptSubobject( TZCONTROL, "Control" );
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
   //:VIEW TZADCSDO  REGISTERED AS TZADCSDO
   zVIEW     TZADCSDO = 0; 
   //:VIEW TaskLPLR  REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_2[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
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

   //:// First Activate the selected Object View.
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, TZADWWKO.W_MetaDefUpdateView.Name, 0 )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "W_MetaDefUpdateView", "Name" );
   nRC = ActivateMetaOI_ByName( ViewToWindow, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_0, 0 );
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
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), UpdateLOD, "LOD", "Name" );
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_1, "" );
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
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE  
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END
   //:IF TZADWWKO.EntitySubGroup.GroupType = "DetailGroupUpdate"
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroupUpdate" ) == 0 )
   { 
      //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
      GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), UpdateLOD, "LOD_EntityParent", "Name" );
      o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_1, "" );
      //:ELSE
   } 
   else
   { 
      //:IF TZADWWKO.ESG_LOD_Entity EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_LOD_Entity" );
      if ( lTempInteger_1 == 0 )
      { 
         //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, TZADWWKO.ESG_LOD_Entity.Name, "" )
         GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
         o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_2, "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SELECT_AutodesignGroupType( VIEW ViewToWindow )

//:   VIEW TZADWWKO  REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
SELECT_AutodesignGroupType( zVIEW     ViewToWindow )
{
   zVIEW     TZADWWKO = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_Base   BASED ON LOD  TZWDLGSO 
   zVIEW     AD_Base = 0; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// Build the list of Group Templates (either Update or List) depending on the Group Type selected.

   //:// Delete any current entries.
   //:FOR EACH TZADWWKO.AutodesignGroupTemplate 
   RESULT = SetCursorFirstEntity( TZADWWKO, "AutodesignGroupTemplate", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.AutodesignGroupTemplate NONE  
      RESULT = DeleteEntity( TZADWWKO, "AutodesignGroupTemplate", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "AutodesignGroupTemplate", "" );
   } 

   //:END

   //:// Position on correct Template based on Group Type.
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
   //:IF szGroupType = "ListGroupOnly"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 )
   { 
      //:// Position on Template Window of List Groups.
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ListBoxGroupTemplates"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ListBoxGroupTemplates", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
         //:             "AD_Base does not have Autodesign Template named 'ListBoxGroupTemplates'.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Window Group", "AD_Base does not have Autodesign Template named 'ListBoxGroupTemplates'.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "DetailGroupUpdate"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupUpdate", 1, 0, 51 ) == 0 )
      { 
         //:// Position on Template Window of Update Groups.
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "UpdateGroupTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "UpdateGroupTemplates", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
            //:             "AD_Base does not have Autodesign Template named 'UpdateGroupTemplates'.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Window Group", "AD_Base does not have Autodesign Template named 'UpdateGroupTemplates'.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// Build new list.
   //:FOR EACH AD_Base.Control 
   RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZADWWKO.AutodesignGroupTemplate 
      RESULT = CreateEntity( TZADWWKO, "AutodesignGroupTemplate", zPOS_AFTER );
      //:TZADWWKO.AutodesignGroupTemplate.GroupName = AD_Base.Control.Tag   
      SetAttributeFromAttribute( TZADWWKO, "AutodesignGroupTemplate", "GroupName", AD_Base, "Control", "Tag" );
      RESULT = SetCursorNextEntity( AD_Base, "Control", "" );
   } 

   //:END

   //:// If this is ListGroupWUpdatePage, switch to the xx window.
   //:IF szGroupType = "ListGroupWUpdatePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
   { 
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AutodesignForGroupListUpd" )
      SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZADWEBD", "AutodesignForGroupListUpd" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ActivateAD_Base( VIEW ReturnedAD_BaseView,
//:                 VIEW ViewToWindow )

//:   INTEGER nRC
static zSHORT
o_ActivateAD_Base( zPVIEW    ReturnedAD_BaseView,
                   zVIEW     ViewToWindow )
{
   zLONG     nRC = 0; 


   //:nRC = ActivateOI_FromFile( ReturnedAD_BaseView, "TZWDLGSO", ViewToWindow, "C:\LPLR\AD_Base\AD_Base.PWD", zSINGLE )
   nRC = ActivateOI_FromFile( ReturnedAD_BaseView, "TZWDLGSO", ViewToWindow, "C:\\LPLR\\AD_Base\\AD_Base.PWD", zSINGLE );
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
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 

   RESULT = GetViewByName( &TZADWWKO, "TZADWWKO", ViewToWindow, zLEVEL_TASK );

   //:// If the Group Type is "DetailGroupUpdate" and the Group selected has a Text/Control pair, set the 
   //:// CSS_Class specification entries for Prompt and Control from those values.
   //:IF TZADWWKO.EntitySubGroup.GroupType = "DetailGroupUpdate"
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "GroupType", "DetailGroupUpdate" ) == 0 )
   { 
      //:GET VIEW AD_Base NAMED "AD_Base"
      RESULT = GetViewByName( &AD_Base, "AD_Base", ViewToWindow, zLEVEL_TASK );
      //:CreateViewFromView(AD_Base2, AD_Base )
      CreateViewFromView( &AD_Base2, AD_Base );
      //:IF AD_Base2.CtrlCtrl EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( AD_Base2, "CtrlCtrl" );
      if ( lTempInteger_0 == 0 )
      { 
         //:SetViewToSubobject( AD_Base2, "CtrlCtrl" )
         SetViewToSubobject( AD_Base2, "CtrlCtrl" );
         //:IF AD_Base2.CtrlCtrl EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( AD_Base2, "CtrlCtrl" );
         if ( lTempInteger_1 == 0 )
         { 
            //:SetViewToSubobject( AD_Base2, "CtrlCtrl" )
            SetViewToSubobject( AD_Base2, "CtrlCtrl" );
            //:IF AD_Base2.CtrlCtrl EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( AD_Base2, "CtrlCtrl" );
            if ( lTempInteger_2 == 0 )
            { 
               //:SetViewToSubobject( AD_Base2, "CtrlCtrl" )
               SetViewToSubobject( AD_Base2, "CtrlCtrl" );
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:SET CURSOR FIRST AD_Base2.Control WHERE AD_Base2.ControlDef.Tag = "Text" 
      RESULT = SetCursorFirstEntityByString( AD_Base2, "ControlDef", "Tag", "Text", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:TZADWWKO.EntitySubGroup.CSS_ClassForPrompt  = AD_Base2.Control.CSS_Class 
         SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "CSS_ClassForPrompt", AD_Base2, "Control", "CSS_Class" );
         //:SET CURSOR NEXT AD_Base2.Control
         RESULT = SetCursorNextEntity( AD_Base2, "Control", "" );
         //:TZADWWKO.EntitySubGroup.CSS_ClassForControl = AD_Base2.Control.CSS_Class
         SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "CSS_ClassForControl", AD_Base2, "Control", "CSS_Class" );
      } 

      //:END
      //:DropView( AD_Base2 )
      DropView( AD_Base2 );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:InitializeUpdateGroupsForCSS( VIEW AD_Base  BASED ON LOD TZWDLGSO,
//:                              VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                              STRING ( 50 ) szBaseWindowName,
//:                              STRING ( 50 ) szBaseGroupName )

//:   VIEW AD_BaseCtl BASED ON LOD TZWDLGSO
static zVOID
o_InitializeUpdateGroupsForCSS( zVIEW     AD_Base,
                                zVIEW     TZADWWKO,
                                zPCHAR    szBaseWindowName,
                                zPCHAR    szBaseGroupName )
{
   zVIEW     AD_BaseCtl = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 


   //:// Initialize CSS_Class work areas in TZADWWKO for a particular AD_Base Template Window.
   //:// The Template Window and GroupBox names are passed to this operation. 
   //:// If an entry for them already exists, no action is taken.
   //:// If an entry doesn't exist, then the work area is created and the existing Text and EditBox in the AD_Base Template in memory
   //:// are deleted so that they won't create a problem when the Control is cloned later in processing.

   //:SET CURSOR FIRST TZADWWKO.CSS_ClassPageGroup
   //:           WHERE TZADWWKO.CSS_ClassPageGroup.WindowName   = szBaseWindowName
   //:             AND TZADWWKO.CSS_ClassPageGroup.GroupBoxName = szBaseGroupName
   RESULT = SetCursorFirstEntity( TZADWWKO, "CSS_ClassPageGroup", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "CSS_ClassPageGroup", "WindowName", szBaseWindowName ) != 0 || CompareAttributeToString( TZADWWKO, "CSS_ClassPageGroup", "GroupBoxName", szBaseGroupName ) != 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "CSS_ClassPageGroup", "" );
      } 

   } 

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// We need to create the entry.
      //:CreateViewFromView( AD_BaseCtl, AD_Base )
      CreateViewFromView( &AD_BaseCtl, AD_Base );
      //:NAME VIEW  AD_BaseCtl "AD_BaseCtl"
      SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );
      //:CREATE ENTITY TZADWWKO.CSS_ClassPageGroup 
      RESULT = CreateEntity( TZADWWKO, "CSS_ClassPageGroup", zPOS_AFTER );
      //:TZADWWKO.CSS_ClassPageGroup.WindowName   = szBaseWindowName
      SetAttributeFromString( TZADWWKO, "CSS_ClassPageGroup", "WindowName", szBaseWindowName );
      //:TZADWWKO.CSS_ClassPageGroup.GroupBoxName = szBaseGroupName
      SetAttributeFromString( TZADWWKO, "CSS_ClassPageGroup", "GroupBoxName", szBaseGroupName );
      //:SET CURSOR FIRST AD_BaseCtl.Window  WHERE AD_BaseCtl.Window.Tag  = szBaseWindowName
      RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Window", "Tag", szBaseWindowName, "" );
      //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szBaseGroupName
      RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szBaseGroupName, "" );
      //:IF AD_BaseCtl.CtrlCtrl EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
      if ( lTempInteger_0 == 0 )
      { 
         //:// There is a subgroup, so look for Text and Control GroupListUpdatetemplate controls.
         //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
         SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
         //:IF AD_BaseCtl.CtrlCtrl EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
         if ( lTempInteger_1 == 0 )
         { 
            //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
            SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
            //:IF AD_BaseCtl.CtrlCtrl EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
            if ( lTempInteger_2 == 0 )
            { 
               //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
               SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
               //:IF AD_BaseCtl.CtrlCtrl EXISTS
               lTempInteger_3 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
               if ( lTempInteger_3 == 0 )
               { 
                  //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
                  SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
               } 

               //:END
            } 

            //:END
         } 

         //:END

         //:// We should be at the level of the Text and EditBox controls if they exist.
         //:// Set the CSS values as necessary and delete the base Controls.
         //:FOR EACH AD_BaseCtl.Control
         RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:IF AD_BaseCtl.ControlDef.Tag = "Text"
            if ( CompareAttributeToString( AD_BaseCtl, "ControlDef", "Tag", "Text" ) == 0 )
            { 
               //:TZADWWKO.CSS_ClassPageGroup.CSS_ClassPromptValue = AD_BaseCtl.Control.CSS_Class 
               SetAttributeFromAttribute( TZADWWKO, "CSS_ClassPageGroup", "CSS_ClassPromptValue", AD_BaseCtl, "Control", "CSS_Class" );
               //:DELETE ENTITY AD_BaseCtl.Control NONE
               RESULT = DeleteEntity( AD_BaseCtl, "Control", zREPOS_NONE );
               //:ELSE
            } 
            else
            { 
               //:IF AD_BaseCtl.ControlDef.Tag = "EditBox"
               if ( CompareAttributeToString( AD_BaseCtl, "ControlDef", "Tag", "EditBox" ) == 0 )
               { 
                  //:TZADWWKO.CSS_ClassPageGroup.CSS_ClassControlValue = AD_BaseCtl.Control.CSS_Class 
                  SetAttributeFromAttribute( TZADWWKO, "CSS_ClassPageGroup", "CSS_ClassControlValue", AD_BaseCtl, "Control", "CSS_Class" );
                  //:DELETE ENTITY AD_BaseCtl.Control NONE
                  RESULT = DeleteEntity( AD_BaseCtl, "Control", zREPOS_NONE );
               } 

               //:END
            } 

            RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
            //:END
         } 

         //:END
      } 

      //:END
      //:DropView( AD_BaseCtl )
      DropView( AD_BaseCtl );
   } 

   //:END
   return;
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
//:BuildAD_GroupPotListR( VIEW TZADWWKO     BASED ON LOD TZADWWKO,
//:                       VIEW MainLOD      BASED ON LOD TZZOLODO,
//:                       VIEW RecursiveLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) szStartEntityName,     // The Entity Name at which recursive processing starts
//:                       STRING ( 1 )  szFlatListNameSuffix,  // FlatListPotentialAttribute suffix as explained below
//:                       INTEGER       lCurrentLevel,         // The current recursive level of the LOD_ParentEntity
//:                       INTEGER       lStartLevel,           // The recursive level at which attributes will be copied
//:                       STRING ( 50 ) szIndentSpaces )       // Spaces for indenting the name shown in the list. They begin with lStartLevel

//:   STRING ( 80 ) szIndentEntityName
static zVOID
o_BuildAD_GroupPotListR( zVIEW     TZADWWKO,
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


   //:// Build the FlatListPotentialAttribute entries from the subobject starting with Entity Name, szStartEntityName, and continuing
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
      //:BuildAD_GroupPotListR( TZADWWKO, MainLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, lCurrentLevel, lStartLevel, szIndentSpaces )
      o_BuildAD_GroupPotListR( TZADWWKO, MainLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, lCurrentLevel, lStartLevel, szIndentSpaces );
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
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
   o_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_0, "" );
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

   //:// Set the two Range values in FlatSelectedSearchAttribute from those in FlatListPotentialAttribute and
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


//:LOCAL OPERATION
//:AddFindRangeInitializationValues( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                                  VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                                  INTEGER nFileOut )

//:   STRING ( 256 ) szVML_Statement
static zVOID
o_AddFindRangeInitializationValue( zVIEW     TZADWWKO,
                                   zVIEW     TZWINDOW,
                                   zLONG     nFileOut )
{
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 


   //:// Generate the VML wXferO.FindRangeSetAttributes entries for the specified Range entries.

   //:// Don't take action if no Range entries were specified.
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType != ""
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      } 

   } 

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:RETURN
      return;
   } 

   //:END 

   //:// First create work entries, RangeTempTargetEntity, from FlatSelectedSearchAttribute entries.
   //:FOR EACH TZADWWKO.RangeTempTargetEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.FlatSelectedSearchAttribute NONE
      RESULT = DeleteEntity( TZADWWKO, "FlatSelectedSearchAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType != ""
      if ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZADWWKO.RangeTempTargetEntity 
         //:           WHERE TZADWWKO.RangeTempTargetEntity.SearchEntityName    = TZADWWKO.FlatSelectedSearchAttribute.EntityName 
         //:             AND TZADWWKO.RangeTempTargetEntity.SearchAttributeName = TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName 
         RESULT = SetCursorFirstEntity( TZADWWKO, "RangeTempTargetEntity", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchEntityName", TZADWWKO, "FlatSelectedSearchAttribute", "EntityName" ) != 0 ||
                    CompareAttributeToAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZADWWKO, "RangeTempTargetEntity", "" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY TZADWWKO.RangeTempTargetEntity 
            RESULT = CreateEntity( TZADWWKO, "RangeTempTargetEntity", zPOS_AFTER );
            //:TZADWWKO.RangeTempTargetEntity.SearchEntityName    = TZADWWKO.FlatSelectedSearchAttribute.EntityName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchEntityName", TZADWWKO, "FlatSelectedSearchAttribute", "EntityName" );
            //:TZADWWKO.RangeTempTargetEntity.SearchAttributeName = TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName" );
         } 

         //:END
         //:IF TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType = "From"
         if ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "From" ) == 0 )
         { 
            //:TZADWWKO.RangeTempTargetEntity.FromValueAttributeName = TZADWWKO.FlatSelectedSearchAttribute.AttributeName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "FromValueAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "AttributeName" );
            //:ELSE
         } 
         else
         { 
            //:TZADWWKO.RangeTempTargetEntity.ToValueAttributeName = TZADWWKO.FlatSelectedSearchAttribute.AttributeName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "ToValueAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "AttributeName" );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      //:END
   } 

   //:END

   //:// Next, generate the Range setting entries into the current position of the output file
   //:FOR EACH TZADWWKO.RangeTempTargetEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szVML_Statement = "   CREATE ENTITY wXferO.FindRangeSetAttributes"
      ZeidonStringCopy( szVML_Statement, 1, 0, "   CREATE ENTITY wXferO.FindRangeSetAttributes", 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.SearchEntityName       = " + QUOTES + TZADWWKO.RangeTempTargetEntity.SearchEntityName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.SearchEntityName       = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "SearchEntityName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.SearchAttributeName    = " + QUOTES + TZADWWKO.RangeTempTargetEntity.SearchAttributeName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.SearchAttributeName    = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "SearchAttributeName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.FromValueAttributeName = " + QUOTES + TZADWWKO.RangeTempTargetEntity.FromValueAttributeName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.FromValueAttributeName = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "FromValueAttributeName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_2, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.ToValueAttributeName   = " + QUOTES + TZADWWKO.RangeTempTargetEntity.ToValueAttributeName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.ToValueAttributeName   = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "ToValueAttributeName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_3, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      RESULT = SetCursorNextEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   } 

   //:END
   return;
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
      //:TraceLineS( "*** Start of ActivateAD_Base", "" )
      TraceLineS( "*** Start of ActivateAD_Base", "" );
      //:nRC = ActivateAD_Base( AD_Base, ViewToWindow )
      nRC = o_ActivateAD_Base( &AD_Base, ViewToWindow );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      //:END
      //:NAME VIEW AD_Base "AD_Base"
      SetNameForView( AD_Base, "AD_Base", 0, zLEVEL_TASK );
      //:TraceLineS( "*** After of ActivateAD_Base", "" )
      TraceLineS( "*** After of ActivateAD_Base", "" );
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


//:LOCAL OPERATION
//:RenameControls( VIEW TZWINDOW BASED ON LOD TZWDLGSO )

//:   STRING ( 5 ) szPrefix
static zVOID
o_RenameControls( zVIEW     TZWINDOW )
{
   zCHAR     szPrefix[ 6 ] = { 0 }; 
   //:STRING ( 5 ) szCount
   zCHAR     szCount[ 6 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:// Rename GroupBox, Grid and PushBtn controls to avoid duplicate generated names.
   //:Count = 0
   Count = 0;
   //:FOR EACH TZWINDOW.Control 
   RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:Count = Count + 1
      Count = Count + 1;
      //:szCount = Count
      ZeidonStringConvertFromNumber( szCount, 1, 0, 5, Count, (ZDecimal) 0.0, "I" );
      //:szPrefix = "GB" + szCount
      ZeidonStringCopy( szPrefix, 1, 0, "GB", 1, 0, 6 );
      ZeidonStringConcat( szPrefix, 1, 0, szCount, 1, 0, 6 );
      //:TZWINDOW.Control.Tag = szPrefix
      SetAttributeFromString( TZWINDOW, "Control", "Tag", szPrefix );
      //:// Call recursive operation to set other Control Tags.
      //:IF TZWINDOW.CtrlCtrl EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZWINDOW, "CtrlCtrl" );
      if ( lTempInteger_0 == 0 )
      { 
         //:RenameControlsRecurs( TZWINDOW, szPrefix, 1 )
         o_RenameControlsRecurs( TZWINDOW, szPrefix, 1 );
      } 

      RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
      //:END
   } 

   //:END
   return;
// END
} 


 
#ifdef __cplusplus
}
#endif
