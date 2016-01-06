#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

//:TRANSFORMATION OPERATION
//:InitWebSessionObject( VIEW vKZXMLPGO BASED ON LOD KZXMLPGO,
//:                      VIEW AnyView,
//:                      STRING ( 32 ) UserID )

//:   SHORT nRC
zOPER_EXPORT zSHORT OPERATION
oKZXMLPGO_InitWebSessionObject( zPVIEW    vKZXMLPGO,
                                zVIEW     AnyView,
                                zPCHAR    UserID )
{
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:nRC = SfActivateSysEmptyOI( vKZXMLPGO, "KZXMLPGO", AnyView, zSINGLE )
   nRC = SfActivateSysEmptyOI( vKZXMLPGO, "KZXMLPGO", AnyView, zSINGLE );
   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:CREATE ENTITY vKZXMLPGO.Session
      RESULT = CreateEntity( *vKZXMLPGO, "Session", zPOS_AFTER );
      //:vKZXMLPGO.Session.UserID = UserID
      SetAttributeFromString( *vKZXMLPGO, "Session", "UserID", UserID );
   } 

   //:END
   //:RETURN nRC
   return( nRC );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oKZXMLPGO_InitWebPage( zVIEW     vKZXMLPGO,
                       zPCHAR    DialogName,
                       zPCHAR    WindowName )
{
   zSHORT    RESULT; 

   //:InitWebPage( VIEW vKZXMLPGO BASED ON LOD KZXMLPGO,
   //:          STRING ( 32 ) DialogName,
   //:          STRING ( 32 ) WindowName )

   //:// Delete any existing Page entry and initialize a new one.
   //:SET CURSOR FIRST vKZXMLPGO.Page
   RESULT = SetCursorFirstEntity( vKZXMLPGO, "Page", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:DELETE ENTITY vKZXMLPGO.Page NONE
      RESULT = DeleteEntity( vKZXMLPGO, "Page", zREPOS_NONE );
   } 

   //:END
   //:CREATE ENTITY vKZXMLPGO.Page
   RESULT = CreateEntity( vKZXMLPGO, "Page", zPOS_AFTER );
   //:vKZXMLPGO.Page.DialogName = DialogName
   SetAttributeFromString( vKZXMLPGO, "Page", "DialogName", DialogName );
   //:vKZXMLPGO.Page.WindowName = WindowName
   SetAttributeFromString( vKZXMLPGO, "Page", "WindowName", WindowName );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddWebControlSubobject( VIEW vKZXMLPGO BASED ON LOD KZXMLPGO,
//:                        STRING ( 32 )  ControlName,
//:                        STRING ( 32 )  ControlType,
//:                        STRING ( 32 )  ControlContext,
//:                        STRING ( 32 )  SourceViewName,
//:                        STRING ( 32 )  SourceEntity,
//:                        STRING ( 32 )  SourceAttribute,
//:                        STRING ( 200 ) AutoComboBoxExternalValue )

//:   VIEW            SourceView
zOPER_EXPORT zSHORT OPERATION
oKZXMLPGO_AddWebControlSubobject( zVIEW     vKZXMLPGO,
                                  zPCHAR    ControlName,
                                  zPCHAR    ControlType,
                                  zPCHAR    ControlContext,
                                  zPCHAR    SourceViewName,
                                  zPCHAR    SourceEntity,
                                  zPCHAR    SourceAttribute,
                                  zPCHAR    AutoComboBoxExternalValue )
{
   zVIEW     SourceView = 0; 
   //:VIEW            SourceView2
   zVIEW     SourceView2 = 0; 
   //:SHORT           nRC
   zSHORT    nRC = 0; 
   //:INTEGER         CursorPos
   zLONG     CursorPos = 0; 
   //:STRING ( 2000 ) ExternalValue
   zCHAR     ExternalValue[ 2001 ] = { 0 }; 
   //:STRING ( 200 )  CurrentValue
   zCHAR     CurrentValue[ 201 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 32001 ]; 


   //:// Create a Control entity for the data passed.
   //:// If the Control is a ListBox or ComboBox, also create the SubControl entries for each list value.

   //:CREATE ENTITY vKZXMLPGO.Control
   RESULT = CreateEntity( vKZXMLPGO, "Control", zPOS_AFTER );
   //:vKZXMLPGO.Control.Name = ControlName
   SetAttributeFromString( vKZXMLPGO, "Control", "Name", ControlName );
   //:vKZXMLPGO.Control.Type = ControlType
   SetAttributeFromString( vKZXMLPGO, "Control", "Type", ControlType );

   //:// If the Named View does not exist or the EntityName doesn't exist, simply return.
   //:GET VIEW SourceView NAMED SourceViewName
   RESULT = GetViewByName( &SourceView, SourceViewName, vKZXMLPGO, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// There is no mapping, so return having just created the entity, with Name and Type,
      //:// unless the Control is Text, in which case we'll add the Text value, which is passed as
      //:// AutoComboBoxExternalValue.
      //:IF vKZXMLPGO.Control.Type = "Text" OR
      //:   vKZXMLPGO.Control.Type = "PushBtn"
      if ( CompareAttributeToString( vKZXMLPGO, "Control", "Type", "Text" ) == 0 || CompareAttributeToString( vKZXMLPGO, "Control", "Type", "PushBtn" ) == 0 )
      { 

         //:vKZXMLPGO.Control.CurrentValue = AutoComboBoxExternalValue
         SetAttributeFromString( vKZXMLPGO, "Control", "CurrentValue", AutoComboBoxExternalValue );
      } 

      //:END
      //:RETURN
      return( 0 );
   } 

   //:END
   //:IF SourceEntity != ""
   if ( ZeidonStringCompare( SourceEntity, 1, 0, "", 1, 0, 33 ) != 0 )
   { 
      //:nRC = CheckExistenceOfEntity( SourceView, SourceEntity )
      nRC = CheckExistenceOfEntity( SourceView, SourceEntity );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN
         return( 0 );
      } 

      //:END
   } 

   //:END

   //:// If there is mapping, process according to Control Type
   //:IF vKZXMLPGO.Control.Type = "EditBox" OR
   //:   vKZXMLPGO.Control.Type = "Text" OR
   //:   vKZXMLPGO.Control.Type = "CheckBox"
   if ( CompareAttributeToString( vKZXMLPGO, "Control", "Type", "EditBox" ) == 0 || CompareAttributeToString( vKZXMLPGO, "Control", "Type", "Text" ) == 0 || CompareAttributeToString( vKZXMLPGO, "Control", "Type", "CheckBox" ) == 0 )
   { 

      //:GetStringFromAttributeByContext( ExternalValue,
      //:                                 SourceView, SourceEntity, SourceAttribute, ControlContext, 2000 )
      GetStringFromAttributeByContext( ExternalValue, SourceView, SourceEntity, SourceAttribute, ControlContext, 2000 );
      //:vKZXMLPGO.Control.CurrentValue = ExternalValue
      SetAttributeFromString( vKZXMLPGO, "Control", "CurrentValue", ExternalValue );
      //:ELSE
   } 
   else
   { 

      //:IF vKZXMLPGO.Control.Type = "MLEdit"
      if ( CompareAttributeToString( vKZXMLPGO, "Control", "Type", "MLEdit" ) == 0 )
      { 
         //:SetAttributeFromAttribute( vKZXMLPGO, "Control", "CurrentValue",
         //:                        SourceView, SourceEntity, SourceAttribute )
         SetAttributeFromAttribute( vKZXMLPGO, "Control", "CurrentValue", SourceView, SourceEntity, SourceAttribute );
         //:ELSE
      } 
      else
      { 

         //:IF vKZXMLPGO.Control.Type = "ComboBox"
         if ( CompareAttributeToString( vKZXMLPGO, "Control", "Type", "ComboBox" ) == 0 )
         { 
            //:// Build the list of selection values from the Domain table entries.
            //:GetStringFromAttributeByContext( CurrentValue,
            //:                           SourceView, SourceEntity, SourceAttribute, ControlContext, 200 )
            GetStringFromAttributeByContext( CurrentValue, SourceView, SourceEntity, SourceAttribute, ControlContext, 200 );
            //:vKZXMLPGO.Control.CurrentValue = CurrentValue
            SetAttributeFromString( vKZXMLPGO, "Control", "CurrentValue", CurrentValue );
            //:nRC = GetFirstTableEntryForAttribute( ExternalValue,
            //:                                SourceView, SourceEntity, SourceAttribute,
            //:                                ControlContext, CursorPos )
            nRC = GetFirstTableEntryForAttribute( ExternalValue, sizeof( ExternalValue ), SourceView, SourceEntity, SourceAttribute, ControlContext, &CursorPos );
            //:LOOP WHILE nRC >= 0
            while ( nRC >= 0 )
            { 
               //:CREATE ENTITY vKZXMLPGO.ComboValue
               RESULT = CreateEntity( vKZXMLPGO, "ComboValue", zPOS_AFTER );
               //:vKZXMLPGO.ComboValue.ExternalValue = ExternalValue
               SetAttributeFromString( vKZXMLPGO, "ComboValue", "ExternalValue", ExternalValue );
               //:IF ExternalValue = CurrentValue
               if ( ZeidonStringCompare( ExternalValue, 1, 0, CurrentValue, 1, 0, 2001 ) == 0 )
               { 
                  //:SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 1 )
                  SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 1 );
                  //:ELSE
               } 
               else
               { 
                  //:SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 0 )
                  SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 0 );
               } 

               //:END
               //:nRC = GetNextTableEntryForAttribute( ExternalValue,
               //:                               SourceView, SourceEntity, SourceAttribute,
               //:                               ControlContext, CursorPos )
               nRC = GetNextTableEntryForAttribute( ExternalValue, sizeof( ExternalValue ), SourceView, SourceEntity, SourceAttribute, ControlContext, &CursorPos );
            } 

            //:END
            //:ELSE
         } 
         else
         { 

            //:IF vKZXMLPGO.Control.Type = "ComboBoxAuto"
            if ( CompareAttributeToString( vKZXMLPGO, "Control", "Type", "ComboBoxAuto" ) == 0 )
            { 
               //:// Build the list of selection values from the list of entities.
               //:// Note that the entry selected is defined by the AutoComboBoxExternalValue passed, which
               //:// was set from the value of the Edit Mapping.
               //:vKZXMLPGO.Control.CurrentValue = AutoComboBoxExternalValue
               SetAttributeFromString( vKZXMLPGO, "Control", "CurrentValue", AutoComboBoxExternalValue );
               //:nRC = SetCursorFirstEntity( SourceView, SourceEntity, "" )
               nRC = SetCursorFirstEntity( SourceView, SourceEntity, "" );
               //:LOOP WHILE nRC >= 0
               while ( nRC >= 0 )
               { 
                  //:CREATE ENTITY vKZXMLPGO.ComboValue
                  RESULT = CreateEntity( vKZXMLPGO, "ComboValue", zPOS_AFTER );
                  //:GetStringFromAttributeByContext( ExternalValue,
                  //:                        SourceView, SourceEntity, SourceAttribute, "", 2000 )
                  GetStringFromAttributeByContext( ExternalValue, SourceView, SourceEntity, SourceAttribute, "", 2000 );
                  //:vKZXMLPGO.ComboValue.ExternalValue = ExternalValue
                  SetAttributeFromString( vKZXMLPGO, "ComboValue", "ExternalValue", ExternalValue );
                  //:IF ExternalValue = AutoComboBoxExternalValue
                  if ( ZeidonStringCompare( ExternalValue, 1, 0, AutoComboBoxExternalValue, 1, 0, 2001 ) == 0 )
                  { 
                     //:SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 1 )
                     SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 1 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 0 )
                     SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 0 );
                  } 

                  //:END
                  //:nRC = SetCursorNextEntity( SourceView, SourceEntity, "" )
                  nRC = SetCursorNextEntity( SourceView, SourceEntity, "" );
               } 

               //:END
               //:ELSE
            } 
            else
            { 

               //:IF vKZXMLPGO.Control.Type = "ComboBoxList"
               if ( CompareAttributeToString( vKZXMLPGO, "Control", "Type", "ComboBoxList" ) == 0 )
               { 
                  //:// Build the list of selection values from the list of entities.
                  //:// Note that the entry selected is defined by the cursor position.
                  //:CreateViewFromViewForTask( SourceView2, SourceView, vKZXMLPGO )
                  CreateViewFromViewForTask( &SourceView2, SourceView, vKZXMLPGO );
                  //:GetStringFromAttributeByContext( CurrentValue,
                  //:                     SourceView, SourceEntity, SourceAttribute, "", 2000 )
                  GetStringFromAttributeByContext( CurrentValue, SourceView, SourceEntity, SourceAttribute, "", 2000 );
                  //:vKZXMLPGO.Control.CurrentValue = CurrentValue
                  SetAttributeFromString( vKZXMLPGO, "Control", "CurrentValue", CurrentValue );
                  //:nRC = SetCursorFirstEntity( SourceView2, SourceEntity, "" )
                  nRC = SetCursorFirstEntity( SourceView2, SourceEntity, "" );
                  //:LOOP WHILE nRC >= 0
                  while ( nRC >= 0 )
                  { 
                     //:CREATE ENTITY vKZXMLPGO.ComboValue
                     RESULT = CreateEntity( vKZXMLPGO, "ComboValue", zPOS_AFTER );
                     //:GetStringFromAttributeByContext( ExternalValue,
                     //:                     SourceView2, SourceEntity, SourceAttribute, "", 2000 )
                     GetStringFromAttributeByContext( ExternalValue, SourceView2, SourceEntity, SourceAttribute, "", 2000 );
                     //:vKZXMLPGO.ComboValue.ExternalValue = ExternalValue
                     SetAttributeFromString( vKZXMLPGO, "ComboValue", "ExternalValue", ExternalValue );
                     //:IF ExternalValue = CurrentValue
                     if ( ZeidonStringCompare( ExternalValue, 1, 0, CurrentValue, 1, 0, 2001 ) == 0 )
                     { 
                        //:SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 1 )
                        SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 1 );
                        //:ELSE
                     } 
                     else
                     { 
                        //:SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 0 )
                        SetSelectStateOfEntity( vKZXMLPGO, "ComboValue", 0 );
                     } 

                     //:END
                     //:nRC = SetCursorNextEntity( SourceView2, SourceEntity, "" )
                     nRC = SetCursorNextEntity( SourceView2, SourceEntity, "" );
                  } 

                  //:END
                  //:DropView( SourceView2 )
                  DropView( SourceView2 );
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

   //:TraceLineS( "AddWebControlSubobject Type: ", vKZXMLPGO.Control.Type )
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vKZXMLPGO, "Control", "Type" );
   TraceLineS( "AddWebControlSubobject Type: ", szTempString_0 );
   //:TraceLineS( "      =============== Value: ", vKZXMLPGO.Control.CurrentValue )
   GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vKZXMLPGO, "Control", "CurrentValue" );
   TraceLineS( "      =============== Value: ", szTempString_1 );
   return( 0 );
//    /* vKZXMLPGO.Control.Type = "EditBox" OR
//       vKZXMLPGO.Control.Type = "ListBox" OR
//       vKZXMLPGO.Control.Type = "Text" OR
//       vKZXMLPGO.Control.Type = "CheckBox" OR
//       vKZXMLPGO.Control.Type = "ListBox" OR
//       vKZXMLPGO.Control.Type = "MLE" OR
//       vKZXMLPGO.Control.Type = "Outliner" OR
//       vKZXMLPGO.Control.Type = "RadioBtnGroup" */
// END
} 


 
#ifdef __cplusplus
}
#endif
