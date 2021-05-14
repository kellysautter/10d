#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
ActivateERD_List( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ActivateEntityInstance( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ActivateLOD_AttributeList( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ActivateLOD_AttributeMapping( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ActivateLOD_Mapping( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ActivateRelTypeInstance( zVIEW     vSubtask );


static zVOID
o_DeriveRootLOD( zVIEW     vTZBRENTO );


zOPER_EXPORT zSHORT OPERATION
DropInstance( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EntityListPopUp( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
LOD_EntityPopUp( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NextEntityInstance( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NextRelTypeInstance( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
PreviousEntityInstance( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
PreviousRelTypeInstance( zVIEW     vSubtask );


static zSHORT
o_fnLocalBuildQual_0( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_1( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_2( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_3( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_4( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_5( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_6( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_7( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_8( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_9( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 );


static zSHORT
o_fnLocalBuildQual_10( zVIEW     vSubtask,
                       zPVIEW    vQualObject,
                       zLONG     lTempInteger_1 );


//:DIALOG OPERATION
//:ActivateERD_List( VIEW vSubtask )

//:   VIEW vTZCMCPRO BASED ON LOD TZCMCPRO
zOPER_EXPORT zSHORT OPERATION
ActivateERD_List( zVIEW     vSubtask )
{
   zVIEW     vTZCMCPRO = 0; 
   //:VIEW vTZBRERDO BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 


   //:GET VIEW vTZCMCPRO NAMED "TZCMCPRO"
   RESULT = GetViewByName( &vTZCMCPRO, "TZCMCPRO", vSubtask, zLEVEL_TASK );

   //:// Position on ERD MetaDef and then activate Browse Data Model
   //:SET CURSOR FIRST vTZCMCPRO.Z_MetaDef WHERE vTZCMCPRO.Z_MetaDef.Type = 4
   RESULT = SetCursorFirstEntityByInteger( vTZCMCPRO, "Z_MetaDef", "Type", 4, "" );
   //:ACTIVATE vTZBRERDO SINGLE WHERE vTZBRERDO.Z_MetaDef.ZKey = vTZCMCPRO.Z_MetaDef.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZCMCPRO, "Z_MetaDef", "ZKey" );
   o_fnLocalBuildQual_3( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRERDO, "TZBRERDO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRERDO "TZBRERDO"
   SetNameForView( vTZBRERDO, "TZBRERDO", 0, zLEVEL_TASK );

   //:// Exclude duplicate RelLink instance
   //:FOR EACH vTZBRERDO.ER_RelLink1 WITHIN vTZBRERDO.EntpER_Model
   RESULT = SetCursorFirstEntity( vTZBRERDO, "ER_RelLink1", "EntpER_Model" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vTZBRERDO.ER_RelLink2.ZKey != vTZBRERDO.ER_RelLink1.ZKey
      if ( CompareAttributeToAttribute( vTZBRERDO, "ER_RelLink2", "ZKey", vTZBRERDO, "ER_RelLink1", "ZKey" ) != 0 )
      { 
         //:SET CURSOR NEXT vTZBRERDO.ER_RelLink2
         RESULT = SetCursorNextEntity( vTZBRERDO, "ER_RelLink2", "" );
      } 

      //:END
      //:EXCLUDE vTZBRERDO.ER_RelLink2
      RESULT = ExcludeEntity( vTZBRERDO, "ER_RelLink2", zREPOS_AFTER );
      RESULT = SetCursorNextEntity( vTZBRERDO, "ER_RelLink1", "EntpER_Model" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ActivateEntityInstance( VIEW vSubtask )

//:   VIEW vTZBRENTO BASED ON LOD TZBRENTO
zOPER_EXPORT zSHORT OPERATION
ActivateEntityInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRENTO = 0; 
   //:VIEW vTZBRERDO BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   //:VIEW vTZBRLIAO  BASED ON LOD TZBRLIAO
   zVIEW     vTZBRLIAO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 
   zLONG     lTempInteger_1; 


   //:GET VIEW vTZBRERDO NAMED "TZBRERDO"
   RESULT = GetViewByName( &vTZBRERDO, "TZBRERDO", vSubtask, zLEVEL_TASK );
   //:ACTIVATE vTZBRENTO SINGLE WHERE vTZBRENTO.ER_Entity.ZKey = vTZBRERDO.ER_Entity.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRERDO, "ER_Entity", "ZKey" );
   o_fnLocalBuildQual_0( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRENTO, "TZBRENTO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRENTO "TZBRENTO"
   SetNameForView( vTZBRENTO, "TZBRENTO", 0, zLEVEL_TASK );

   //:// Create the derived attributes RootLOD.
   //:DeriveRootLOD( vTZBRENTO )
   o_DeriveRootLOD( vTZBRENTO );

   //:// Derive LOD_Name in LOD_Attribute
   //:FOR EACH vTZBRENTO.LOD_Attribute WITHIN vTZBRENTO.ER_Entity
   RESULT = SetCursorFirstEntity( vTZBRENTO, "LOD_Attribute", "ER_Entity" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vTZBRENTO.LOD_Entity WHERE
      //:    vTZBRENTO.LOD_Entity.ZKey = vTZBRENTO.AttributeLOD_Entity.ZKey
      GetIntegerFromAttribute( &lTempInteger_1, vTZBRENTO, "AttributeLOD_Entity", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vTZBRENTO, "LOD_Entity", "ZKey", lTempInteger_1, "" );
      //:vTZBRENTO.LOD_Attribute.LOD_Name = vTZBRENTO.RootLOD.Name
      SetAttributeFromAttribute( vTZBRENTO, "LOD_Attribute", "LOD_Name", vTZBRENTO, "RootLOD", "Name" );
      RESULT = SetCursorNextEntity( vTZBRENTO, "LOD_Attribute", "ER_Entity" );
   } 

   //:END

   //:// Delete the TZBRLIAO object if it exists.  The ActivateLOD_AttributeMapping
   //:// operation has a check on that object to see what code was last executed:
   //:// ActivateEntityInstance or ActivateLOD_AttributeList.
   //:GET VIEW vTZBRLIAO NAMED "TZBRLIAO"
   RESULT = GetViewByName( &vTZBRLIAO, "TZBRLIAO", vSubtask, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:DropObjectInstance( vTZBRLIAO )
      DropObjectInstance( vTZBRLIAO );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ActivateLOD_AttributeList( VIEW vSubtask )

//:   VIEW vTZBRLIAO  BASED ON LOD TZBRLIAO
zOPER_EXPORT zSHORT OPERATION
ActivateLOD_AttributeList( zVIEW     vSubtask )
{
   zVIEW     vTZBRLIAO = 0; 
   //:VIEW vTZBRENTO  BASED ON LOD TZBRENTO
   zVIEW     vTZBRENTO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 


   //:GET VIEW vTZBRENTO NAMED "TZBRENTO"
   RESULT = GetViewByName( &vTZBRENTO, "TZBRENTO", vSubtask, zLEVEL_TASK );
   //:ACTIVATE vTZBRLIAO SINGLE WHERE vTZBRLIAO.LOD_Entity.ZKey = vTZBRENTO.LOD_Entity.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRENTO, "LOD_Entity", "ZKey" );
   o_fnLocalBuildQual_8( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRLIAO, "TZBRLIAO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRLIAO "TZBRLIAO"
   SetNameForView( vTZBRLIAO, "TZBRLIAO", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ActivateLOD_AttributeMapping( VIEW vSubtask )

//:   VIEW vTZBRLIAO  BASED ON LOD TZBRLIAO
zOPER_EXPORT zSHORT OPERATION
ActivateLOD_AttributeMapping( zVIEW     vSubtask )
{
   zVIEW     vTZBRLIAO = 0; 
   //:VIEW vTZBRENTO  BASED ON LOD TZBRENTO
   zVIEW     vTZBRENTO = 0; 
   //:VIEW vRecur     BASED ON LOD TZBRLOAO
   zVIEW     vRecur = 0; 
   //:VIEW vTZBRLOAO  BASED ON LOD TZBRLOAO
   zVIEW     vTZBRLOAO = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 
   zLONG     lTempInteger_1; 
   zVIEW     vTempViewVar_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 


   //:// Activate the Attribute mapping object from either the list of LOD_Attributes for a
   //:// LOD_Entity (TZBRLIAO) or the list of ER_Attributes and corresponding LOD_Attributes
   //:// from the ER_Attribute list (TZBRENTO).
   //:GET VIEW vTZBRLIAO NAMED "TZBRLIAO"
   RESULT = GetViewByName( &vTZBRLIAO, "TZBRLIAO", vSubtask, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:ACTIVATE vTZBRLOAO SINGLE WHERE vTZBRLOAO.LOD_Attribute.ZKey = vTZBRLIAO.LOD_Attribute.ZKey
      GetIntegerFromAttribute( &lTempInteger_0, vTZBRLIAO, "LOD_Attribute", "ZKey" );
      o_fnLocalBuildQual_9( vSubtask, &vTempViewVar_0, lTempInteger_0 );
      RESULT = ActivateObjectInstance( &vTZBRLOAO, "TZBRLOAO", vSubtask, vTempViewVar_0, zSINGLE );
      DropView( vTempViewVar_0 );
      //:NAME VIEW vTZBRLOAO "TZBRLOAO"
      SetNameForView( vTZBRLOAO, "TZBRLOAO", 0, zLEVEL_TASK );
      //:ELSE
   } 
   else
   { 
      //:GET VIEW vTZBRENTO NAMED "TZBRENTO"
      RESULT = GetViewByName( &vTZBRENTO, "TZBRENTO", vSubtask, zLEVEL_TASK );
      //:ACTIVATE vTZBRLOAO SINGLE WHERE vTZBRLOAO.LOD_Attribute.ZKey = vTZBRENTO.LOD_Attribute.ZKey
      GetIntegerFromAttribute( &lTempInteger_1, vTZBRENTO, "LOD_Attribute", "ZKey" );
      o_fnLocalBuildQual_10( vSubtask, &vTempViewVar_1, lTempInteger_1 );
      RESULT = ActivateObjectInstance( &vTZBRLOAO, "TZBRLOAO", vSubtask, vTempViewVar_1, zSINGLE );
      DropView( vTempViewVar_1 );
      //:NAME VIEW vTZBRLOAO "TZBRLOAO"
      SetNameForView( vTZBRLOAO, "TZBRLOAO", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Derive Window and Dialog Names
   //:FOR EACH vTZBRLOAO.MapRef
   RESULT = SetCursorFirstEntity( vTZBRLOAO, "MapRef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vTZBRLOAO.Control EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( vTZBRLOAO, "Control" );
      if ( lTempInteger_2 == 0 )
      { 
         //:// Follow the recursive Control path to set the work entity containing Dialog
         //:// and Window names.
         //:CreateViewFromViewForTask( vRecur, vTZBRLOAO, vSubtask )
         CreateViewFromViewForTask( &vRecur, vTZBRLOAO, vSubtask );
         //:nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" )
         nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" );
         //:LOOP WHILE nRC < zCURSOR_SET
         while ( nRC < zCURSOR_SET )
         { 
            //:SetViewToSubobject( vRecur, "ParentControl" )
            SetViewToSubobject( vRecur, "ParentControl" );
            //:nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" )
            nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" );
         } 

         //:END

         //:CREATE ENTITY vTZBRLOAO.ControlWindowInfo
         RESULT = CreateEntity( vTZBRLOAO, "ControlWindowInfo", zPOS_AFTER );
         //:vTZBRLOAO.ControlWindowInfo.WindowName = vRecur.ControlWindow.Tag
         SetAttributeFromAttribute( vTZBRLOAO, "ControlWindowInfo", "WindowName", vRecur, "ControlWindow", "Tag" );
         //:vTZBRLOAO.ControlWindowInfo.DialogName = vRecur.ControlDialog.Tag
         SetAttributeFromAttribute( vTZBRLOAO, "ControlWindowInfo", "DialogName", vRecur, "ControlDialog", "Tag" );
         //:DropView( vRecur )
         DropView( vRecur );
      } 

      //:END
      //:IF vTZBRLOAO.Option EXISTS
      lTempInteger_3 = CheckExistenceOfEntity( vTZBRLOAO, "Option" );
      if ( lTempInteger_3 == 0 )
      { 
         //:// Follow the recursive Control path to set the work entity containing Dialog
         //:// and Window names.
         //:CreateViewFromViewForTask( vRecur, vTZBRLOAO, vSubtask )
         CreateViewFromViewForTask( &vRecur, vTZBRLOAO, vSubtask );
         //:nRC = CheckExistenceOfEntity( vRecur, "OptionWindow" )
         nRC = CheckExistenceOfEntity( vRecur, "OptionWindow" );
         //:LOOP WHILE nRC < zCURSOR_SET
         while ( nRC < zCURSOR_SET )
         { 
            //:SetViewToSubobject( vRecur, "ParentOption" )
            SetViewToSubobject( vRecur, "ParentOption" );
            //:nRC = CheckExistenceOfEntity( vRecur, "OptionWindow" )
            nRC = CheckExistenceOfEntity( vRecur, "OptionWindow" );
         } 

         //:END

         //:CREATE ENTITY vTZBRLOAO.OptionWindowInfo
         RESULT = CreateEntity( vTZBRLOAO, "OptionWindowInfo", zPOS_AFTER );
         //:vTZBRLOAO.OptionWindowInfo.WindowName = vRecur.OptionWindow.Tag
         SetAttributeFromAttribute( vTZBRLOAO, "OptionWindowInfo", "WindowName", vRecur, "OptionWindow", "Tag" );
         //:vTZBRLOAO.OptionWindowInfo.DialogName = vRecur.OptionDialog.Tag
         SetAttributeFromAttribute( vTZBRLOAO, "OptionWindowInfo", "DialogName", vRecur, "OptionDialog", "Tag" );
         //:DropView( vRecur )
         DropView( vRecur );
      } 

      RESULT = SetCursorNextEntity( vTZBRLOAO, "MapRef", "" );
      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ActivateLOD_Mapping( VIEW vSubtask )

//:   VIEW vTZBRLOEO  BASED ON LOD TZBRLOEO
zOPER_EXPORT zSHORT OPERATION
ActivateLOD_Mapping( zVIEW     vSubtask )
{
   zVIEW     vTZBRLOEO = 0; 
   //:VIEW vRecur     BASED ON LOD TZBRLOEO
   zVIEW     vRecur = 0; 
   //:VIEW vTZBRENTO  BASED ON LOD TZBRENTO
   zVIEW     vTZBRENTO = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 
   zSHORT    lTempInteger_1; 


   //:GET VIEW vTZBRENTO NAMED "TZBRENTO"
   RESULT = GetViewByName( &vTZBRENTO, "TZBRENTO", vSubtask, zLEVEL_TASK );
   //:ACTIVATE vTZBRLOEO SINGLE WHERE vTZBRLOEO.LOD_Entity.ZKey = vTZBRENTO.LOD_Entity.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRENTO, "LOD_Entity", "ZKey" );
   o_fnLocalBuildQual_7( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRLOEO, "TZBRLOEO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRLOEO "TZBRLOEO"
   SetNameForView( vTZBRLOEO, "TZBRLOEO", 0, zLEVEL_TASK );

   //:// Derive Window and Dialog Names
   //:FOR EACH vTZBRLOEO.MapRef
   RESULT = SetCursorFirstEntity( vTZBRLOEO, "MapRef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vTZBRLOEO.Control EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vTZBRLOEO, "Control" );
      if ( lTempInteger_1 == 0 )
      { 
         //:// Follow the recursive Control path to set the work entity containing Dialog
         //:// and Window names.
         //:CreateViewFromViewForTask( vRecur, vTZBRLOEO, vSubtask )
         CreateViewFromViewForTask( &vRecur, vTZBRLOEO, vSubtask );
         //:nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" )
         nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" );
         //:LOOP WHILE nRC < zCURSOR_SET
         while ( nRC < zCURSOR_SET )
         { 
            //:SetViewToSubobject( vRecur, "ParentControl" )
            SetViewToSubobject( vRecur, "ParentControl" );
            //:nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" )
            nRC = CheckExistenceOfEntity( vRecur, "ControlWindow" );
         } 

         //:END

         //:CREATE ENTITY vTZBRLOEO.ControlWindowInfo
         RESULT = CreateEntity( vTZBRLOEO, "ControlWindowInfo", zPOS_AFTER );
         //:vTZBRLOEO.ControlWindowInfo.WindowName = vRecur.ControlWindow.Tag
         SetAttributeFromAttribute( vTZBRLOEO, "ControlWindowInfo", "WindowName", vRecur, "ControlWindow", "Tag" );
         //:vTZBRLOEO.ControlWindowInfo.DialogName = vRecur.ControlDialog.Tag
         SetAttributeFromAttribute( vTZBRLOEO, "ControlWindowInfo", "DialogName", vRecur, "ControlDialog", "Tag" );
         //:DropView( vRecur )
         DropView( vRecur );
      } 

      RESULT = SetCursorNextEntity( vTZBRLOEO, "MapRef", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ActivateRelTypeInstance( VIEW vSubtask )

//:   VIEW vTZBRRELO  BASED ON LOD TZBRRELO
zOPER_EXPORT zSHORT OPERATION
ActivateRelTypeInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRRELO = 0; 
   //:VIEW vTZBRRELO2 BASED ON LOD TZBRRELO
   zVIEW     vTZBRRELO2 = 0; 
   //:VIEW vTZBRERDO  BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 


   //:GET VIEW vTZBRERDO NAMED "TZBRERDO"
   RESULT = GetViewByName( &vTZBRERDO, "TZBRERDO", vSubtask, zLEVEL_TASK );
   //:ACTIVATE vTZBRRELO SINGLE WHERE vTZBRRELO.ER_RelType.ZKey = vTZBRERDO.ER_RelType.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRERDO, "ER_RelType", "ZKey" );
   o_fnLocalBuildQual_4( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRRELO, "TZBRRELO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRRELO "TZBRRELO"
   SetNameForView( vTZBRRELO, "TZBRRELO", 0, zLEVEL_TASK );

   //:// Create additional view for Dialog mapping, with position on next ER_RelLink.
   //:CreateViewFromViewForTask( vTZBRRELO2, vTZBRRELO, vSubtask )
   CreateViewFromViewForTask( &vTZBRRELO2, vTZBRRELO, vSubtask );
   //:NAME VIEW vTZBRRELO2 "TZBRRELO2"
   SetNameForView( vTZBRRELO2, "TZBRRELO2", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST vTZBRRELO.ER_RelLink WHERE
   //:    vTZBRRELO.ER_RelLink.ZKey = vTZBRERDO.ER_RelLink1.ZKey
   GetIntegerFromAttribute( &lTempInteger_1, vTZBRERDO, "ER_RelLink1", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vTZBRRELO, "ER_RelLink", "ZKey", lTempInteger_1, "" );
   //:SET CURSOR FIRST vTZBRRELO2.ER_RelLink WHERE
   //:    vTZBRRELO2.ER_RelLink.ZKey = vTZBRERDO.ER_RelLink2.ZKey
   GetIntegerFromAttribute( &lTempInteger_2, vTZBRERDO, "ER_RelLink2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vTZBRRELO2, "ER_RelLink", "ZKey", lTempInteger_2, "" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:DeriveRootLOD( VIEW vTZBRENTO BASED ON LOD TZBRENTO )

//:   VIEW  vRecur    BASED ON LOD TZBRENTO
static zVOID
o_DeriveRootLOD( zVIEW     vTZBRENTO )
{
   zVIEW     vRecur = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:FOR EACH vTZBRENTO.LOD_Entity
   RESULT = SetCursorFirstEntity( vTZBRENTO, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateViewFromViewForTask( vRecur, vTZBRENTO, vTZBRENTO )
      CreateViewFromViewForTask( &vRecur, vTZBRENTO, vTZBRENTO );
      //:nRC = CheckExistenceOfEntity( vRecur, "LOD" )
      nRC = CheckExistenceOfEntity( vRecur, "LOD" );
      //:LOOP WHILE nRC < zCURSOR_SET
      while ( nRC < zCURSOR_SET )
      { 
         //:SetViewToSubobject( vRecur, "ParentLOD_Entity" )
         SetViewToSubobject( vRecur, "ParentLOD_Entity" );
         //:nRC = CheckExistenceOfEntity( vRecur, "LOD" )
         nRC = CheckExistenceOfEntity( vRecur, "LOD" );
      } 

      //:END

      //:INCLUDE vTZBRENTO.RootLOD FROM vRecur.LOD
      RESULT = IncludeSubobjectFromSubobject( vTZBRENTO, "RootLOD", vRecur, "LOD", zPOS_AFTER );
      //:DropView( vRecur )
      DropView( vRecur );
      RESULT = SetCursorNextEntity( vTZBRENTO, "LOD_Entity", "" );
   } 

   //:END
   return;
// END
} 


//:DIALOG OPERATION
//:DropInstance( VIEW vSubtask )

//:   VIEW vTZBRENTO BASED ON LOD TZBRENTO
zOPER_EXPORT zSHORT OPERATION
DropInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRENTO = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vTZBRENTO NAMED "TZBRENTO"
   RESULT = GetViewByName( &vTZBRENTO, "TZBRENTO", vSubtask, zLEVEL_TASK );
   //:DropObjectInstance( vTZBRENTO )
   DropObjectInstance( vTZBRENTO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
EntityListPopUp( zVIEW     vSubtask )
{

   //:EntityListPopUp( VIEW vSubtask )

   //:CreateTrackingPopupMenu( vSubtask, "EntityListPopUp", -1, 0, 0, 0 )
   CreateTrackingPopupMenu( vSubtask, "EntityListPopUp", -1, 0, 0, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
LOD_EntityPopUp( zVIEW     vSubtask )
{

   //:LOD_EntityPopUp( VIEW vSubtask )

   //:CreateTrackingPopupMenu( vSubtask, "LOD_EntityPopUp", -1, 0, 0, 0 )
   CreateTrackingPopupMenu( vSubtask, "LOD_EntityPopUp", -1, 0, 0, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NextEntityInstance( VIEW vSubtask )

//:   VIEW vTZBRENTO BASED ON LOD TZBRENTO
zOPER_EXPORT zSHORT OPERATION
NextEntityInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRENTO = 0; 
   //:VIEW vTZBRERDO BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 


   //:GET VIEW vTZBRENTO NAMED "TZBRENTO"
   RESULT = GetViewByName( &vTZBRENTO, "TZBRENTO", vSubtask, zLEVEL_TASK );
   //:DropObjectInstance( vTZBRENTO )
   DropObjectInstance( vTZBRENTO );

   //:GET VIEW vTZBRERDO NAMED "TZBRERDO"
   RESULT = GetViewByName( &vTZBRERDO, "TZBRERDO", vSubtask, zLEVEL_TASK );
   //:SET CURSOR NEXT vTZBRERDO.ER_Entity
   RESULT = SetCursorNextEntity( vTZBRERDO, "ER_Entity", "" );
   //:ACTIVATE vTZBRENTO SINGLE WHERE vTZBRENTO.ER_Entity.ZKey = vTZBRERDO.ER_Entity.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRERDO, "ER_Entity", "ZKey" );
   o_fnLocalBuildQual_1( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRENTO, "TZBRENTO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRENTO "TZBRENTO"
   SetNameForView( vTZBRENTO, "TZBRENTO", 0, zLEVEL_TASK );

   //:// Create the derived attributes RootLOD.
   //:DeriveRootLOD( vTZBRENTO )
   o_DeriveRootLOD( vTZBRENTO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NextRelTypeInstance( VIEW vSubtask )

//:   VIEW vTZBRRELO  BASED ON LOD TZBRRELO
zOPER_EXPORT zSHORT OPERATION
NextRelTypeInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRRELO = 0; 
   //:VIEW vTZBRRELO2 BASED ON LOD TZBRRELO
   zVIEW     vTZBRRELO2 = 0; 
   //:VIEW vTZBRERDO  BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 


   //:GET VIEW vTZBRRELO NAMED "TZBRRELO"
   RESULT = GetViewByName( &vTZBRRELO, "TZBRRELO", vSubtask, zLEVEL_TASK );
   //:DropView( vTZBRRELO )
   DropView( vTZBRRELO );

   //:GET VIEW vTZBRERDO NAMED "TZBRERDO"
   RESULT = GetViewByName( &vTZBRERDO, "TZBRERDO", vSubtask, zLEVEL_TASK );
   //:SET CURSOR NEXT vTZBRERDO.ER_RelLink1 WITHIN vTZBRERDO.EntpER_Model
   RESULT = SetCursorNextEntity( vTZBRERDO, "ER_RelLink1", "EntpER_Model" );
   //:ACTIVATE vTZBRRELO SINGLE WHERE vTZBRRELO.ER_RelType.ZKey = vTZBRERDO.ER_RelType.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRERDO, "ER_RelType", "ZKey" );
   o_fnLocalBuildQual_5( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRRELO, "TZBRRELO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRRELO "TZBRRELO"
   SetNameForView( vTZBRRELO, "TZBRRELO", 0, zLEVEL_TASK );

   //:// Create additional view for Dialog mapping, with position on next ER_RelLink.
   //:CreateViewFromViewForTask( vTZBRRELO2, vTZBRRELO, vSubtask )
   CreateViewFromViewForTask( &vTZBRRELO2, vTZBRRELO, vSubtask );
   //:NAME VIEW vTZBRRELO2 "TZBRRELO2"
   SetNameForView( vTZBRRELO2, "TZBRRELO2", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST vTZBRRELO.ER_RelLink WHERE
   //:    vTZBRRELO.ER_RelLink.ZKey = vTZBRERDO.ER_RelLink1.ZKey
   GetIntegerFromAttribute( &lTempInteger_1, vTZBRERDO, "ER_RelLink1", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vTZBRRELO, "ER_RelLink", "ZKey", lTempInteger_1, "" );
   //:SET CURSOR FIRST vTZBRRELO2.ER_RelLink WHERE
   //:    vTZBRRELO2.ER_RelLink.ZKey = vTZBRERDO.ER_RelLink2.ZKey
   GetIntegerFromAttribute( &lTempInteger_2, vTZBRERDO, "ER_RelLink2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vTZBRRELO2, "ER_RelLink", "ZKey", lTempInteger_2, "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PreviousEntityInstance( VIEW vSubtask )

//:   VIEW vTZBRENTO BASED ON LOD TZBRENTO
zOPER_EXPORT zSHORT OPERATION
PreviousEntityInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRENTO = 0; 
   //:VIEW vTZBRERDO BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 


   //:GET VIEW vTZBRENTO NAMED "TZBRENTO"
   RESULT = GetViewByName( &vTZBRENTO, "TZBRENTO", vSubtask, zLEVEL_TASK );
   //:DropObjectInstance( vTZBRENTO )
   DropObjectInstance( vTZBRENTO );

   //:GET VIEW vTZBRERDO NAMED "TZBRERDO"
   RESULT = GetViewByName( &vTZBRERDO, "TZBRERDO", vSubtask, zLEVEL_TASK );
   //:SET CURSOR PREVIOUS vTZBRERDO.ER_Entity
   RESULT = SetCursorPrevEntity( vTZBRERDO, "ER_Entity", "" );
   //:ACTIVATE vTZBRENTO SINGLE WHERE vTZBRENTO.ER_Entity.ZKey = vTZBRERDO.ER_Entity.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRERDO, "ER_Entity", "ZKey" );
   o_fnLocalBuildQual_2( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRENTO, "TZBRENTO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRENTO "TZBRENTO"
   SetNameForView( vTZBRENTO, "TZBRENTO", 0, zLEVEL_TASK );

   //:// Create the derived attributes RootLOD.
   //:DeriveRootLOD( vTZBRENTO )
   o_DeriveRootLOD( vTZBRENTO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PreviousRelTypeInstance( VIEW vSubtask )

//:   VIEW vTZBRRELO  BASED ON LOD TZBRRELO
zOPER_EXPORT zSHORT OPERATION
PreviousRelTypeInstance( zVIEW     vSubtask )
{
   zVIEW     vTZBRRELO = 0; 
   //:VIEW vTZBRRELO2 BASED ON LOD TZBRRELO
   zVIEW     vTZBRRELO2 = 0; 
   //:VIEW vTZBRERDO  BASED ON LOD TZBRERDO
   zVIEW     vTZBRERDO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zVIEW     vTempViewVar_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 


   //:GET VIEW vTZBRRELO NAMED "TZBRRELO"
   RESULT = GetViewByName( &vTZBRRELO, "TZBRRELO", vSubtask, zLEVEL_TASK );
   //:DropView( vTZBRRELO )
   DropView( vTZBRRELO );

   //:GET VIEW vTZBRERDO NAMED "TZBRERDO"
   RESULT = GetViewByName( &vTZBRERDO, "TZBRERDO", vSubtask, zLEVEL_TASK );
   //:SET CURSOR PREVIOUS vTZBRERDO.ER_RelLink1 WITHIN vTZBRERDO.EntpER_Model
   RESULT = SetCursorPrevEntity( vTZBRERDO, "ER_RelLink1", "EntpER_Model" );
   //:ACTIVATE vTZBRRELO SINGLE WHERE vTZBRRELO.ER_RelType.ZKey = vTZBRERDO.ER_RelType.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vTZBRERDO, "ER_RelType", "ZKey" );
   o_fnLocalBuildQual_6( vSubtask, &vTempViewVar_0, lTempInteger_0 );
   RESULT = ActivateObjectInstance( &vTZBRRELO, "TZBRRELO", vSubtask, vTempViewVar_0, zSINGLE );
   DropView( vTempViewVar_0 );
   //:NAME VIEW vTZBRRELO "TZBRRELO"
   SetNameForView( vTZBRRELO, "TZBRRELO", 0, zLEVEL_TASK );

   //:// Create additional view for Dialog mapping, with position on next ER_RelLink.
   //:CreateViewFromViewForTask( vTZBRRELO2, vTZBRRELO, vSubtask )
   CreateViewFromViewForTask( &vTZBRRELO2, vTZBRRELO, vSubtask );
   //:NAME VIEW vTZBRRELO2 "TZBRRELO2"
   SetNameForView( vTZBRRELO2, "TZBRRELO2", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST vTZBRRELO.ER_RelLink WHERE
   //:    vTZBRRELO.ER_RelLink.ZKey = vTZBRERDO.ER_RelLink1.ZKey
   GetIntegerFromAttribute( &lTempInteger_1, vTZBRERDO, "ER_RelLink1", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vTZBRRELO, "ER_RelLink", "ZKey", lTempInteger_1, "" );
   //:SET CURSOR FIRST vTZBRRELO2.ER_RelLink WHERE
   //:    vTZBRRELO2.ER_RelLink.ZKey = vTZBRERDO.ER_RelLink2.ZKey
   GetIntegerFromAttribute( &lTempInteger_2, vTZBRERDO, "ER_RelLink2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vTZBRRELO2, "ER_RelLink", "ZKey", lTempInteger_2, "" );
   return( 0 );
// END
} 


static zSHORT
o_fnLocalBuildQual_0( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "ER_Entity" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "ER_Entity" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_1( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "ER_Entity" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "ER_Entity" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_2( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "ER_Entity" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "ER_Entity" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_3( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "Z_MetaDef" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "Z_MetaDef" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_4( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "ER_RelType" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "ER_RelType" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_5( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "ER_RelType" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "ER_RelType" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_6( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "ER_RelType" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "ER_RelType" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_7( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "LOD_Entity" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "LOD_Entity" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_8( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "LOD_Entity" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "LOD_Entity" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_9( zVIEW     vSubtask,
                      zPVIEW    vQualObject,
                      zLONG     lTempInteger_0 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "LOD_Attribute" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "LOD_Attribute" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_0 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


static zSHORT
o_fnLocalBuildQual_10( zVIEW     vSubtask,
                       zPVIEW    vQualObject,
                       zLONG     lTempInteger_1 )
{
   zSHORT    RESULT; 

   RESULT = SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "LOD_Attribute" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "LOD_Attribute" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromInteger( *vQualObject, "QualAttrib", "Value", lTempInteger_1 );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
} 


 
#ifdef __cplusplus
}
#endif
