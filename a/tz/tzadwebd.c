#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
SELECT_VOR_ForAutodesign( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_AutoDesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_AutoDesignGroup( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
REMOVE_PotentialAttributes( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SAVE_AutoDesignForGroup( zVIEW     ViewToWindow );


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
o_AutodesignUpdateCtrls( zVIEW     TZWINDOWL,
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
   //:VIEW AD_Base     BASED ON LOD TZWDLGSO
   zVIEW     AD_Base = 0; 
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


//:DIALOG OPERATION
//:CANCEL_AutoDesignGroup( VIEW ViewToWindow )

//:   VIEW TZWINDOWL REGISTERED AS TZWINDOWL
zOPER_EXPORT zSHORT OPERATION
CANCEL_AutoDesignGroup( zVIEW     ViewToWindow )
{
   zVIEW     TZWINDOWL = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCONTROL REGISTERED AS TZCONTROL
   zVIEW     TZCONTROL = 0; 

   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCONTROL, "TZCONTROL", ViewToWindow, zLEVEL_TASK );

   //:// Cancel the subobjects for Control and AutoDesignGroup.
   //:CancelSubobject( TZCONTROL, "Control" )
   CancelSubobject( TZCONTROL, "Control" );
   //:CancelSubobject( TZWINDOWL, "AutoDesignGroup" )
   CancelSubobject( TZWINDOWL, "AutoDesignGroup" );
   return( 0 );
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
   //:VIEW TZADCSDO2 BASED ON LOD TZADCSDO
   zVIEW     TZADCSDO2 = 0; 
   //:STRING ( 1 ) szCreateEntityOnlyEntry
   zCHAR     szCreateEntityOnlyEntry[ 2 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );

   //:// Remove any selected Selected entries.
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListSelectedAttribute" )
      nRC = GetSelectStateOfEntity( TZADCSDO, "FlatListSelectedAttribute" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:DELETE ENTITY TZADCSDO.FlatListSelectedAttribute NONE
         RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedAttribute", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
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
//:AUTODESIGN_Group( VIEW ViewToWindow )

//:   VIEW TZADCSDO    REGISTERED AS TZADCSDO
zOPER_EXPORT zSHORT OPERATION
AUTODESIGN_Group( zVIEW     ViewToWindow )
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

   //:// Make sure that at least one attribute has been selected.
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
   //:SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialAttribute", SelectedLOD, "LOD_EntityParent", zSET_ALL )
   SetMatchingAttributesByName( TZADCSDO, "FlatListPotentialAttribute", SelectedLOD, "LOD_EntityParent", zSET_ALL );

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
      //:TZADCSDO.FlatListPotentialAttribute.EntityName    = SelectedLOD.LOD_EntityParent.Name
      SetAttributeFromAttribute( TZADCSDO, "FlatListPotentialAttribute", "EntityName", SelectedLOD, "LOD_EntityParent", "Name" );
      //:TZADCSDO.FlatListPotentialAttribute.AttributeName = szAttributeName
      SetAttributeFromString( TZADCSDO, "FlatListPotentialAttribute", "AttributeName", szAttributeName );

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
   zCHAR     szTempString_0[ 255 ]; 

   RESULT = GetViewByName( &TZADCSDO, "TZADCSDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", ViewToWindow, zLEVEL_TASK );

   //:// First make sure that any currently Selected entry is selected on the Potential side, because we are going to
   //:// delete Selected entries and recreate them.
   //:FOR EACH TZADCSDO.FlatListSelectedAttribute
   RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST TZADCSDO.FlatListPotentialAttribute
      //:           WHERE TZADCSDO.FlatListPotentialAttribute.EntityName    = TZADCSDO.FlatListSelectedAttribute.EntityName
      //:             AND TZADCSDO.FlatListPotentialAttribute.AttributeName = TZADCSDO.FlatListSelectedAttribute.AttributeName
      RESULT = SetCursorFirstEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADCSDO, "FlatListPotentialAttribute", "EntityName", TZADCSDO, "FlatListSelectedAttribute", "EntityName" ) != 0 ||
                 CompareAttributeToAttribute( TZADCSDO, "FlatListPotentialAttribute", "AttributeName", TZADCSDO, "FlatListSelectedAttribute", "AttributeName" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( TZADCSDO, "FlatListPotentialAttribute", "" );
         } 

      } 

      //:SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute", 1 )
      SetSelectStateOfEntity( TZADCSDO, "FlatListPotentialAttribute", 1 );
      //:DELETE ENTITY TZADCSDO.FlatListSelectedAttribute NONE
      RESULT = DeleteEntity( TZADCSDO, "FlatListSelectedAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADCSDO, "FlatListSelectedAttribute", "" );
   } 

   //:END

   //:// Copy selected attributes from FlatListPotentialAttribute entries to FlatListSelectedAttribute entries.
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
         //:CREATE ENTITY TZADCSDO.FlatListSelectedAttribute
         RESULT = CreateEntity( TZADCSDO, "FlatListSelectedAttribute", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZADCSDO, "FlatListPotentialAttribute", zSET_ALL )
         SetMatchingAttributesByName( TZADCSDO, "FlatListSelectedAttribute", TZADCSDO, "FlatListPotentialAttribute", zSET_ALL );

         //:// If this is for an Update Group, add the ":" character at the end of the Prompt.
         //:IF TZWINDOWL.AutoDesignGroup.GenerateGroupType = "F"
         if ( CompareAttributeToString( TZWINDOWL, "AutoDesignGroup", "GenerateGroupType", "F" ) == 0 )
         { 
            //:TZADCSDO.FlatListSelectedAttribute.PromptValue = TZADCSDO.FlatListSelectedAttribute.PromptValue + ":"
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADCSDO, "FlatListSelectedAttribute", "PromptValue" );
            ZeidonStringConcat( szTempString_0, 1, 0, ":", 1, 0, 255 );
            SetAttributeFromString( TZADCSDO, "FlatListSelectedAttribute", "PromptValue", szTempString_0 );
         } 

         //:END

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


//:LOCAL OPERATION
//:AutodesignUpdateCtrls( VIEW TZWINDOWL   BASED ON LOD TZWDLGSO,
//:                       VIEW TZCONTROL   BASED ON LOD TZWDLGSO,
//:                       VIEW AD_Base     BASED ON LOD TZWDLGSO,
//:                       VIEW SelectedLOD BASED ON LOD TZZOLODO )

//:   VIEW TZPESRCO    BASED ON LOD TZPESRCO
static zSHORT
o_AutodesignUpdateCtrls( zVIEW     TZWINDOWL,
                         zVIEW     TZCONTROL,
                         zVIEW     AD_Base,
                         zVIEW     SelectedLOD )
{
   zVIEW     TZPESRCO = 0; 
   //:VIEW AD_BaseRoot BASED ON LOD TZWDLGSO
   zVIEW     AD_BaseRoot = 0; 
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
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 255 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zCHAR     szTempString_8[ 255 ]; 


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
         //:szControlDefName = AD_Base.ControlDef.Tag
         GetVariableFromAttribute( szControlDefName, 0, 'S', 101, AD_Base, "ControlDef", "Tag", "", 0 );
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlDefName
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlDefName, "" );
         //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
         CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
         //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
         //:TZCONTROL.Control.Tag = szControlDefName + szActionSuffix + szControlNameSuffixGroup
         ZeidonStringCopy( szTempString_2, 1, 0, szControlDefName, 1, 0, 33 );
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
         //:IF TZCONTROL.EventAct EXISTS
         lTempInteger_2 = CheckExistenceOfEntity( TZCONTROL, "EventAct" );
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
               //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Action", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZCONTROL, "Action", zPOS_AFTER );
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

   //:// Position on Update Base Control.
   //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "UpdateGroup"
   RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "UpdateGroup", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZWINDOWL, "", "Autodesign Window Group",
      //:             "The Base group doesn't have an UpdateGroup Control defined.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZWINDOWL, "", "Autodesign Window Group", "The Base group doesn't have an UpdateGroup Control defined.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( TZWINDOWL, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Determine Width of Prompt and Mapping Controls.
   //:// We will take the UpdateFieldPromptLength value (which is max field prompt size) and multiply it by a character
   //:// size of 8 pixels to get the width for Prompt entries. The width of the Mapping entries will be the
   //:// difference in the size of the Group.
   //:lPromptWidth         = TZWINDOWL.AutoDesignGroup.UpdateFieldPromptLength * 5
   GetIntegerFromAttribute( &lTempInteger_4, TZWINDOWL, "AutoDesignGroup", "UpdateFieldPromptLength" );
   lPromptWidth = lTempInteger_4 * 5;
   //:lGroupWidth          = TZWINDOWL.AutoDesignGroup.ControlWidthInPixels - 10
   GetIntegerFromAttribute( &lTempInteger_5, TZWINDOWL, "AutoDesignGroup", "ControlWidthInPixels" );
   lGroupWidth = lTempInteger_5 - 10;
   //:lMaxMappingDataWidth = lGroupWidth - lPromptWidth
   lMaxMappingDataWidth = lGroupWidth - lPromptWidth;

   //:// Create Group, which will hold individual field values.
   //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER )
   CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlCtrl", zPOS_AFTER );
   //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
   SetViewToSubobject( AD_Base, "CtrlCtrl" );
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL )
   SetMatchingAttributesByName( TZCONTROL, "Control", AD_Base, "Control", zSET_NULL );
   //:TZCONTROL.Control.SZDLG_X = lGroupWidth    // Size is same as parent Group - 10.
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lGroupWidth );
   //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = AD_Base.ControlDef.Tag
   GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), AD_Base, "ControlDef", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_3, "" );
   //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
   //:TZCONTROL.Control.Tag = "Groupbox" + TZWINDOWL.AD_ListBoxEntity.EntityName + szControlNameSuffixGroup
   GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TZWINDOWL, "AD_ListBoxEntity", "EntityName", "", 0 );
   ZeidonStringCopy( szTempString_3, 1, 0, "Groupbox", 1, 0, 33 );
   ZeidonStringConcat( szTempString_3, 1, 0, szTempString_4, 1, 0, 33 );
   ZeidonStringConcat( szTempString_3, 1, 0, szControlNameSuffixGroup, 1, 0, 33 );
   SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_3 );
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

   //:// Build a Prompt and Mapping entry for each FlatListSelectedAttribute entry.
   //:// Starting Y position will be from first control in UpdateGroup.
   //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
   SetViewToSubobject( AD_Base, "CtrlCtrl" );
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SET CURSOR FIRST AD_Base.Control
   RESULT = SetCursorFirstEntity( AD_Base, "Control", "" );
   //:lControlPositionY = AD_Base.Control.PSDLG_Y
   GetIntegerFromAttribute( &lControlPositionY, AD_Base, "Control", "PSDLG_Y" );
   //:lControlPositionX = AD_Base.Control.PSDLG_X
   GetIntegerFromAttribute( &lControlPositionX, AD_Base, "Control", "PSDLG_X" );
   //:lAttributeCount = 0
   lAttributeCount = 0;
   //:FOR EACH TZWINDOWL.AD_MappingAttribute
   RESULT = SetCursorFirstEntity( TZWINDOWL, "AD_MappingAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:lAttributeCount = lAttributeCount + 1
      lAttributeCount = lAttributeCount + 1;
      //:szControlNameSuffixAttribute = lAttributeCount
      ZeidonStringConvertFromNumber( szControlNameSuffixAttribute, 1, 0, 1, lAttributeCount, (ZDecimal) 0.0, "I" );

      //:// Create Prompt entry, which is a Text field. We will use any CSS Class from the first entry in AD_Base.
      //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
      //:szControlNameSuffix = TZWINDOWL.AD_MappingAttribute.AttributeName + szControlNameSuffixGroup + szControlNameSuffixAttribute
      GetStringFromAttribute( szControlNameSuffix, zsizeof( szControlNameSuffix ), TZWINDOWL, "AD_MappingAttribute", "AttributeName" );
      ZeidonStringConcat( szControlNameSuffix, 1, 0, szControlNameSuffixGroup, 1, 0, 101 );
      ZeidonStringConcat( szControlNameSuffix, 1, 0, szControlNameSuffixAttribute, 1, 0, 101 );
      //:TZCONTROL.Control.Tag       = "P_" + szControlNameSuffix
      ZeidonStringCopy( szTempString_5, 1, 0, "P_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_5, 1, 0, szControlNameSuffix, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_5 );
      //:TZCONTROL.Control.Text      = TZWINDOWL.AD_MappingAttribute.PromptValue
      SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZWINDOWL, "AD_MappingAttribute", "PromptValue" );
      //:TZCONTROL.Control.CSS_Class = AD_Base.Control.CSS_Class
      SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", AD_Base, "Control", "CSS_Class" );
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
      ZeidonStringCopy( szTempString_6, 1, 0, "M_", 1, 0, 255 );
      ZeidonStringConcat( szTempString_6, 1, 0, szControlNameSuffix, 1, 0, 255 );
      SetAttributeFromString( TZCONTROL, "Control", "WebCtrlLabelLink", szTempString_6 );

      //:// Create Mapping entry.
      //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
      //:TZCONTROL.Control.Tag       = "M_" + szControlNameSuffix
      ZeidonStringCopy( szTempString_7, 1, 0, "M_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_7, 1, 0, szControlNameSuffix, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_7 );
      //:TZCONTROL.Control.CSS_Class = AD_Base.Control.CSS_Class
      SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", AD_Base, "Control", "CSS_Class" );
      //:TZCONTROL.Control.PSDLG_X   = lControlPositionX + lPromptWidth
      lTempInteger_6 = lControlPositionX + lPromptWidth;
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", lTempInteger_6 );
      //:TZCONTROL.Control.SZDLG_Y   = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 10 );
      //:TZCONTROL.Control.PSDLG_Y   = lControlPositionY
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", lControlPositionY );

      //:// The width of the  Control will normally be determined by the size of the associated attribute, but
      //:// there will always be a max size (lMaxMappingDataWidth) determined above and a min size of 80..
      //:lMappingDataWidth = TZWINDOWL.AD_MappingAttribute.DataWidth * 5
      GetIntegerFromAttribute( &lTempInteger_7, TZWINDOWL, "AD_MappingAttribute", "DataWidth" );
      lMappingDataWidth = lTempInteger_7 * 5;
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
      //:TZCONTROL.Control.SZDLG_X   = lMappingDataWidth
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lMappingDataWidth );

      //:// Build the Editbox, Checkbox, Calendar, MLEdit or Combobox controls depending on ControlType.
      //:szControlType = TZWINDOWL.AD_MappingAttribute.ControlType
      GetVariableFromAttribute( szControlType, 0, 'S', 21, TZWINDOWL, "AD_MappingAttribute", "ControlType", "", 0 );

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
         //:TZCONTROL.Control.Text = TZWINDOWL.AD_MappingAttribute.PromptValue
         SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZWINDOWL, "AD_MappingAttribute", "PromptValue" );
      } 

      //:END

      //:// Build CtrlMap subobject for list control entity from Entity.Attribute of FlatListSelectedAttribute
      //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:SET CURSOR FIRST SelectedLOD.LOD_Entity WHERE SelectedLOD.LOD_Entity.Name = TZWINDOWL.AD_MappingAttribute.EntityName
      GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZWINDOWL, "AD_MappingAttribute", "EntityName" );
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "LOD_Entity", "Name", szTempString_8, "" );
      //:SET CURSOR FIRST SelectedLOD.ER_Attribute WITHIN SelectedLOD.LOD_Entity
      //:           WHERE SelectedLOD.ER_Attribute.Name = TZWINDOWL.AD_MappingAttribute.AttributeName
      GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZWINDOWL, "AD_MappingAttribute", "AttributeName" );
      RESULT = SetCursorFirstEntityByString( SelectedLOD, "ER_Attribute", "Name", szTempString_8, "LOD_Entity" );
      //:INCLUDE TZCONTROL.CtrlMapLOD_Attribute FROM SelectedLOD.LOD_Attribute
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", SelectedLOD, "LOD_Attribute", zPOS_AFTER );
      //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );

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
         //:lControlPositionY = lControlPositionY + 10
         lControlPositionY = lControlPositionY + 10;
      } 

      RESULT = SetCursorNextEntity( TZWINDOWL, "AD_MappingAttribute", "" );
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
   //:DropView( AD_BaseRoot )
   DropView( AD_BaseRoot );
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
      //:nRC = ActivateMetaOI_ByName( ViewToWindow, AD_Base, 0, zREFER_DIALOG_META, zSINGLE, "AD_Base", 0 )
      nRC = ActivateMetaOI_ByName( ViewToWindow, &AD_Base, 0, zREFER_DIALOG_META, zSINGLE, "AD_Base", 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Window Group",
         //:             "No AD_Base Dialog exists for Autodesign.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Window Group", "No AD_Base Dialog exists for Autodesign.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:RETURN -2
         return( -2 );
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


 
#ifdef __cplusplus
}
#endif
