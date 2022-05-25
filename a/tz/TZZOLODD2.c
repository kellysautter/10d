#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zSHORT zwTZZOLODD_InitMenuForEdit( zVIEW, zSHORT, zSHORT );
zSHORT zwTZZOLODD_InitMenuForView( zVIEW, zSHORT, zSHORT* );
zSHORT zwTZZOLODD_InitMenuDisableAll( zVIEW );
zSHORT zwTZZOLODD_CheckEntityType( zVIEW, zVIEW );
zSHORT zwTZZOLODD_SetAttributeType( zVIEW );

zOPER_EXPORT zSHORT OPERATION
SELECT_CalculationSetEntity( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ADD_CalculationSetAttrParm( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
GOTO_UpdateCalculationSetParm( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_DerivedRuleCalcParm( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_DerivedRuleCalcParm( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
ACCEPT_CalculationRuleSet( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_CalculationRuleSet( zVIEW     ViewToWindow );


static zSHORT
o_BUILD_RuleSetParmName( zVIEW     TZZOLODO );


//:DIALOG OPERATION
//:SELECT_CalculationSetEntity( VIEW ViewToWindow )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
SELECT_CalculationSetEntity( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLOD1  REGISTERED AS TZZOLOD1
   zVIEW     TZZOLOD1 = 0; 
   //:VIEW TZZOLODOT BASED ON LOD  TZZOLODO
   zVIEW     TZZOLODOT = 0; 
   zLONG     lTempInteger_0; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLOD1, "TZZOLOD1", ViewToWindow, zLEVEL_TASK );


   //:// Build the selectable Attribute list for the selected LOD_Entity for which we'll use view TZZOLODOT.
   //:FOR EACH TZZOLODO.SelectableAttribute 
   RESULT = SetCursorFirstEntity( TZZOLODO, "SelectableAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZZOLODO.SelectableAttribute NONE  
      RESULT = DeleteEntity( TZZOLODO, "SelectableAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZZOLODO, "SelectableAttribute", "" );
   } 

   //:END
   //:CreateViewFromView( TZZOLODOT, TZZOLODO )
   CreateViewFromView( &TZZOLODOT, TZZOLODO );
   //:SET CURSOR FIRST TZZOLODOT.LOD_Entity WHERE TZZOLODOT.LOD_Entity.ZKey = TZZOLOD1.LOD_EntityParent.ZKey  
   GetIntegerFromAttribute( &lTempInteger_0, TZZOLOD1, "LOD_EntityParent", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( TZZOLODOT, "LOD_Entity", "ZKey", lTempInteger_0, "" );
   //:FOR EACH TZZOLODOT.LOD_Attribute 
   RESULT = SetCursorFirstEntity( TZZOLODOT, "LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY TZZOLODO.SelectableAttribute 
      RESULT = CreateEntity( TZZOLODO, "SelectableAttribute", zPOS_AFTER );
      //:TZZOLODO.SelectableAttribute.Name = TZZOLODOT.ER_Attribute.Name  
      SetAttributeFromAttribute( TZZOLODO, "SelectableAttribute", "Name", TZZOLODOT, "ER_Attribute", "Name" );
      RESULT = SetCursorNextEntity( TZZOLODOT, "LOD_Attribute", "" );
   } 

   //:END
   //:DropView( TZZOLODOT )
   DropView( TZZOLODOT );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ADD_CalculationSetAttrParm( VIEW ViewToWindow )

//:   VIEW TZZOLOD1  REGISTERED AS TZZOLOD1
zOPER_EXPORT zSHORT OPERATION
ADD_CalculationSetAttrParm( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLOD1 = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLODO  REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:VIEW TZZOLODOT BASED ON LOD  TZZOLODO
   zVIEW     TZZOLODOT = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 

   RESULT = GetViewByName( &TZZOLOD1, "TZZOLOD1", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );

   //:// Create a DerivedRuleParameter entry from the selected SelectableAttribute entity.

   //:// Position on the correct LOD_Entity and LOD_Attribute for the selected Attribute.
   //:// The LOD_Entity position comes from TZZOLOD1.LOD_EntityParent.
   //:// The LOD_Attribute position comes from TZZOLODO.SelectableAttribute
   //:CreateViewFromView( TZZOLODOT, TZZOLODO )
   CreateViewFromView( &TZZOLODOT, TZZOLODO );
   //:SET CURSOR FIRST TZZOLODOT.LOD_Entity WHERE TZZOLODOT.LOD_Entity.Name = TZZOLOD1.LOD_EntityParent.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZZOLOD1, "LOD_EntityParent", "Name" );
   RESULT = SetCursorFirstEntityByString( TZZOLODOT, "LOD_Entity", "Name", szTempString_0, "" );
   //:SET CURSOR FIRST TZZOLODOT.ER_Attribute WITHIN TZZOLODOT.LOD_Entity WHERE TZZOLODOT.ER_Attribute.Name = TZZOLODO.SelectableAttribute.Name 
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZZOLODO, "SelectableAttribute", "Name" );
   RESULT = SetCursorFirstEntityByString( TZZOLODOT, "ER_Attribute", "Name", szTempString_1, "LOD_Entity" );
   //:CreateTemporalEntity( TZZOLODO,"DerivedRuleParameter", zPOS_AFTER )
   CreateTemporalEntity( TZZOLODO, "DerivedRuleParameter", zPOS_AFTER );
   //:TZZOLODO.DerivedRuleParameter.EntityAttributeName = TZZOLODOT.LOD_Entity.Name + "." + TZZOLODOT.ER_Attribute.Name 
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZZOLODOT, "LOD_Entity", "Name" );
   ZeidonStringConcat( szTempString_1, 1, 0, ".", 1, 0, 255 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZZOLODOT, "ER_Attribute", "Name", "", 0 );
   ZeidonStringConcat( szTempString_1, 1, 0, szTempString_0, 1, 0, 255 );
   SetAttributeFromString( TZZOLODO, "DerivedRuleParameter", "EntityAttributeName", szTempString_1 );
   //:TZZOLODO.DerivedRuleParameter.EntityName          = TZZOLODOT.LOD_Entity.Name 
   SetAttributeFromAttribute( TZZOLODO, "DerivedRuleParameter", "EntityName", TZZOLODOT, "LOD_Entity", "Name" );
   //:TZZOLODO.DerivedRuleParameter.AttributeName       = TZZOLODOT.ER_Attribute.Name 
   SetAttributeFromAttribute( TZZOLODO, "DerivedRuleParameter", "AttributeName", TZZOLODOT, "ER_Attribute", "Name" );
   //:TZZOLODO.DerivedRuleParameter.DataType            = TZZOLODOT.Domain.DataType 
   SetAttributeFromAttribute( TZZOLODO, "DerivedRuleParameter", "DataType", TZZOLODOT, "Domain", "DataType" );
   //:DropView( TZZOLODOT )
   DropView( TZZOLODOT );

   //:// Rebuild the Parameter Names.
   //:BUILD_RuleSetParmName( TZZOLODO )
   o_BUILD_RuleSetParmName( TZZOLODO );

   //:// Go to the appropriate subwindow (String, Date or Table( based on DataType.
   //:// For now we'll just go to the string subwindow.
   //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZZOLODD", "CalculationSetParmUpdate" )
   SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZZOLODD", "CalculationSetParmUpdate" );
   return( 0 );
//    
// END
} 


//:DIALOG OPERATION
//:GOTO_UpdateCalculationSetParm( VIEW ViewToWindow )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
GOTO_UpdateCalculationSetParm( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );

   //:CreateTemporalSubobjectVersion( TZZOLODO,"DerivedRuleParameter" )
   CreateTemporalSubobjectVersion( TZZOLODO, "DerivedRuleParameter" );

   //:// Set the appropriate work value attribute from the ExternalValue??? and go to the appropriate subwindow, both based on DataType.
   //:// For now we'll just go to the string subwindow.
   //:SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZZOLODD", "CalculationSetParmUpdate" )
   SetWindowActionBehavior( ViewToWindow, zWAB_StartModalSubwindow, "TZZOLODD", "CalculationSetParmUpdate" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_DerivedRuleCalcParm( VIEW ViewToWindow )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_DerivedRuleCalcParm( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );

   //:AcceptSubobject( TZZOLODO,"DerivedRuleParameter" )
   AcceptSubobject( TZZOLODO, "DerivedRuleParameter" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_DerivedRuleCalcParm( VIEW ViewToWindow )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
CANCEL_DerivedRuleCalcParm( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );

   //:CancelSubobject( TZZOLODO,"DerivedRuleParameter" )
   CancelSubobject( TZZOLODO, "DerivedRuleParameter" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ACCEPT_CalculationRuleSet( VIEW ViewToWindow )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
ACCEPT_CalculationRuleSet( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );

   //:// Make sure that a Description and Calculation Rule are specified.
   //:IF TZZOLODO.DerivedRuleCalculationSet.Description = ""
   if ( CompareAttributeToString( TZZOLODO, "DerivedRuleCalculationSet", "Description", "" ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Save Calculation Set",
      //:                           "A Description must be specified",
      //:                           zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( ViewToWindow, "", "Save Calculation Set", "A Description must be specified", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:IF TZZOLODO.DerivedRuleCalculationSet.DerivedRuleCalculation = ""
   if ( CompareAttributeToString( TZZOLODO, "DerivedRuleCalculationSet", "DerivedRuleCalculation", "" ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Save Calculation Set",
      //:                           "A Calculation Rule must be specified",
      //:                           zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( ViewToWindow, "", "Save Calculation Set", "A Calculation Rule must be specified", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:AcceptSubobject( TZZOLODO,"DerivedRuleCalculationSet" )
   AcceptSubobject( TZZOLODO, "DerivedRuleCalculationSet" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_CalculationRuleSet( VIEW ViewToWindow )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
CANCEL_CalculationRuleSet( zVIEW     ViewToWindow )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", ViewToWindow, zLEVEL_TASK );

   //:CancelSubobject( TZZOLODO,"DerivedRuleCalculationSet" )
   CancelSubobject( TZZOLODO, "DerivedRuleCalculationSet" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:BUILD_RuleSetParmName( VIEW TZZOLODO BASED ON LOD  TZZOLODO )

//:   INTEGER      ParmCount
static zSHORT
o_BUILD_RuleSetParmName( zVIEW     TZZOLODO )
{
   zLONG     ParmCount = 0; 
   //:STRING ( 5 ) szParmCount
   zCHAR     szParmCount[ 6 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 


   //:// Build the Parameter Name values in sequential order such as C1, C2, C3, etc.
   //:ParmCount = 1
   ParmCount = 1;
   //:FOR EACH TZZOLODO.DerivedRuleParameter 
   RESULT = SetCursorFirstEntity( TZZOLODO, "DerivedRuleParameter", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szParmCount = ParmCount
      ZeidonStringConvertFromNumber( szParmCount, 1, 0, 5, ParmCount, (ZDecimal) 0.0, "I" );
      //:TZZOLODO.DerivedRuleParameter.BooleanConditionName = "C" + szParmCount 
      ZeidonStringCopy( szTempString_0, 1, 0, "C", 1, 0, 255 );
      ZeidonStringConcat( szTempString_0, 1, 0, szParmCount, 1, 0, 255 );
      SetAttributeFromString( TZZOLODO, "DerivedRuleParameter", "BooleanConditionName", szTempString_0 );
      //:ParmCount = ParmCount + 1
      ParmCount = ParmCount + 1;
      RESULT = SetCursorNextEntity( TZZOLODO, "DerivedRuleParameter", "" );
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
