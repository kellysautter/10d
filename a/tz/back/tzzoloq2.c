#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
#include "TZ__OPRS.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zSHORT zwTZZOLODD_QueryFrameSetUpRecur( zVIEW, zVIEW );
zSHORT zwTZZOLODD_InitMenuForEdit( zVIEW, zSHORT, zSHORT );
zSHORT zwTZZOLODD_InitMenuForView( zVIEW, zSHORT, zSHORT* );
zSHORT zwTZZOLODD_InitMenuDisableAll( zVIEW );
zSHORT zwTZZOLODD_CheckEntityType( zVIEW, zVIEW );
zSHORT zwTZZOLODD_SetAttributeType( zVIEW );

zOPER_EXPORT zSHORT OPERATION
OK( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CopyEntityGroupEnd( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CopyEntityGroupMappingStart( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CopyMappingNameToClipBoard( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
BeforeEntityChange( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EntityAboutToBeChanged( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
Cancel( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EntityChanged( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
PostBuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
RemoveAll( zVIEW     vSubtask );


//:DIALOG OPERATION
//:OK ( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
OK( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //://EntityAboutToBeChanged( vSubtask )

   //:ResetView( TZZOLODO )
   ResetView( TZZOLODO );
   //:AcceptSubobject( TZZOLODO, "LOD_EntityParent" )
   AcceptSubobject( TZZOLODO, "LOD_EntityParent" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CopyEntityGroupEnd( VIEW vSubtask )

//:   VIEW TZZOLODO REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
CopyEntityGroupEnd( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 100 ) MappingName
   zCHAR     MappingName[ 101 ] = { 0 }; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:// Copy the mapping name of the attribute to the clip board.
   //:IF TZZOLODO.LOD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "LOD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:MappingName = "[Z:#E]"
      ZeidonStringCopy( MappingName, 1, 0, "[Z:#E]", 1, 0, 101 );
      //:SetClipboardBlob( MappingName, 0, 0 )
      SetClipboardBlob( MappingName, 0, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CopyEntityGroupMappingStart( VIEW vSubtask )

//:   VIEW TZZOLODO REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
CopyEntityGroupMappingStart( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 100 ) MappingName
   zCHAR     MappingName[ 101 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:// Copy the mapping name of the attribute to the clip board.
   //:IF TZZOLODO.LOD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "LOD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:MappingName = "[Z:#S:" +
      //:              TZZOLODO.LOD_Entity.Name + "]"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZZOLODO, "LOD_Entity", "Name", "", 0 );
      ZeidonStringCopy( MappingName, 1, 0, "[Z:#S:", 1, 0, 101 );
      ZeidonStringConcat( MappingName, 1, 0, szTempString_0, 1, 0, 101 );
      ZeidonStringConcat( MappingName, 1, 0, "]", 1, 0, 101 );
      //:SetClipboardBlob( MappingName, 0, 0 )
      SetClipboardBlob( MappingName, 0, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CopyMappingNameToClipBoard( VIEW vSubtask )

//:   VIEW TZZOLODO REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
CopyMappingNameToClipBoard( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 100 ) MappingName
   zCHAR     MappingName[ 101 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:// Copy the mapping name of the attribute to the clip board.
   //:IF TZZOLODO.ER_Attribute EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "ER_Attribute" );
   if ( lTempInteger_0 == 0 )
   { 
      //:MappingName = "[Z:" +
      //:              TZZOLODO.LOD_Entity.Name + "." +
      //:              TZZOLODO.ER_Attribute.Name + "]"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZZOLODO, "LOD_Entity", "Name", "", 0 );
      ZeidonStringCopy( MappingName, 1, 0, "[Z:", 1, 0, 101 );
      ZeidonStringConcat( MappingName, 1, 0, szTempString_0, 1, 0, 101 );
      ZeidonStringConcat( MappingName, 1, 0, ".", 1, 0, 101 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZZOLODO, "ER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( MappingName, 1, 0, szTempString_1, 1, 0, 101 );
      ZeidonStringConcat( MappingName, 1, 0, "]", 1, 0, 101 );
      //:SetClipboardBlob( MappingName, 0, 0 )
      SetClipboardBlob( MappingName, 0, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:BeforeEntityChange ( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
BeforeEntityChange( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:// Map the control, neccesary as be have "Bypass Input Data Mapping"
   //:// TZZOLODO pointing to the "old" Entity
   //:IF TZZOLODO.POD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:MapCtrl( vSubtask, "JoinCheck" )
      MapCtrl( vSubtask, "JoinCheck" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:EntityAboutToBeChanged( VIEW vSubtask )

//:   /*VIEW  TZDBHODO  REGISTERED AS ODBC_EntityInfo
//:   VIEW  TZZOLODO  REGISTERED AS TZZOLODO
//:   SHORT nIsCheckedOut
//:   // Check to see if there is already an ODBC object loaded.  If it is then store it
//:   // in the blob.
//:   IF TZDBHODO != 0
//:      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META )
zOPER_EXPORT zSHORT OPERATION
EntityAboutToBeChanged( zVIEW     vSubtask )
{

   return( 0 );
//       IF nIsCheckedOut = 1
//          SetBlobFromOI( TZZOLODO, "POD_Entity", "DBH_Info", TZDBHODO, 0 )
//       END
//       DropView( TZDBHODO )
//    END*/
// END
} 


//:DIALOG OPERATION
//:Prebuild ( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:ResetView( TZZOLODO )
   ResetView( TZZOLODO );
   //:CreateTemporalSubobjectVersion( TZZOLODO, "LOD_EntityParent" )
   CreateTemporalSubobjectVersion( TZZOLODO, "LOD_EntityParent" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:Cancel ( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
Cancel( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:ResetView( TZZOLODO )
   ResetView( TZZOLODO );
   //:CancelSubobject( TZZOLODO, "LOD_EntityParent" )
   CancelSubobject( TZZOLODO, "LOD_EntityParent" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:EntityChanged ( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
EntityChanged( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZTENVRO  REGISTERED AS TZTENVRO
   zVIEW     TZTENVRO = 0; 
   //:SHORT nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 201 ]; 
   zCHAR     szTempString_1[ 2 ]; 
   zCHAR     szTempString_2[ 201 ]; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZTENVRO, "TZTENVRO", vSubtask, zLEVEL_TASK );

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META );

   //:IF nIsCheckedOut = 1
   if ( nIsCheckedOut == 1 )
   { 
      //:// do not allow any input to the Join checkbox, if we are on the root
      //://  or if the currrent LOD entity is work or if the currrent LOD entity is derived
      //:IF TZZOLODO.ER_RelLinkRec EXISTS AND TZZOLODO.LOD_EntityParent.Work != "Y"
      lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "ER_RelLinkRec" );
      //:   AND TZZOLODO.LOD_EntityParent.Derived != "Y"
      if ( lTempInteger_0 == 0 && CompareAttributeToString( TZZOLODO, "LOD_EntityParent", "Work", "Y" ) != 0 && CompareAttributeToString( TZZOLODO, "LOD_EntityParent", "Derived", "Y" ) != 0 )
      { 

         //:// Set the cursor to the POD_Entity for the current TE Source.
         //:SET CURSOR FIRST TZZOLODO.POD_Entity
         //:           WHERE  TZZOLODO.TE_DBMS_SourceForEntity.ZKey = TZTENVRO.TE_DBMS_Source.ZKey
         RESULT = SetCursorFirstEntity( TZZOLODO, "POD_Entity", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZZOLODO, "TE_DBMS_SourceForEntity", "ZKey", TZTENVRO, "TE_DBMS_Source", "ZKey" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZZOLODO, "POD_Entity", "" );
            } 

         } 


         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// POD_Entity doesn't exist so create one.
            //:CREATE ENTITY TZZOLODO.POD_Entity
            RESULT = CreateEntity( TZZOLODO, "POD_Entity", zPOS_AFTER );

            //:TZZOLODO.POD_Entity.SQL_JoinWithParent = "N"
            SetAttributeFromString( TZZOLODO, "POD_Entity", "SQL_JoinWithParent", "N" );
            //:INCLUDE TZZOLODO.TE_DBMS_SourceForEntity FROM TZTENVRO.TE_DBMS_Source
            RESULT = IncludeSubobjectFromSubobject( TZZOLODO, "TE_DBMS_SourceForEntity", TZTENVRO, "TE_DBMS_Source", zPOS_AFTER );
         } 

         //:END

         //:// not the root and not work and not derived
         //:SetCtrlState( vSubtask,      "JoinCheck", zCONTROL_STATUS_ENABLED, TRUE )
         SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_ENABLED, TRUE );
         //:ELSE
      } 
      else
      { 
         //:SetCtrlState( vSubtask,      "JoinCheck", zCONTROL_STATUS_ENABLED, FALSE )
         SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_ENABLED, FALSE );
      } 

      //:END

      //:// Refresh checkbox and set the string being displayed.
      //:IF  TZZOLODO.POD_Entity EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
      if ( lTempInteger_1 == 0 )
      { 
         //: TZZOLODO.LOD_EntityParent.WorkString = TZZOLODO.LOD_EntityParent.Name +
         //:                                        " JOIN = " + TZZOLODO.POD_Entity.SQL_JoinWithParent
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZZOLODO, "LOD_EntityParent", "Name" );
         ZeidonStringConcat( szTempString_0, 1, 0, " JOIN = ", 1, 0, 201 );
         GetVariableFromAttribute( szTempString_1, 0, 'S', 2, TZZOLODO, "POD_Entity", "SQL_JoinWithParent", "", 0 );
         ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 201 );
         SetAttributeFromString( TZZOLODO, "LOD_EntityParent", "WorkString", szTempString_0 );
         //:ELSE
      } 
      else
      { 
         //: TZZOLODO.LOD_EntityParent.WorkString = TZZOLODO.LOD_EntityParent.Name + " JOIN = N"
         GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZZOLODO, "LOD_EntityParent", "Name" );
         ZeidonStringConcat( szTempString_2, 1, 0, " JOIN = N", 1, 0, 201 );
         SetAttributeFromString( TZZOLODO, "LOD_EntityParent", "WorkString", szTempString_2 );
      } 

      //:END
   } 

   //:END

   //:RefreshCtrl( vSubtask, "JoinCheck" )
   RefreshCtrl( vSubtask, "JoinCheck" );
   //:RefreshCtrl( vSubtask, "edDescription" )
   RefreshCtrl( vSubtask, "edDescription" );
   //:RefreshCtrl( vSubtask, "cbDerived" )
   RefreshCtrl( vSubtask, "cbDerived" );
   //:RefreshCtrl( vSubtask, "cbWork" )
   RefreshCtrl( vSubtask, "cbWork" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostBuild ( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
PostBuild( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:SHORT nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META );

   //:SetCtrlState( vSubtask, "pbSetXtoOne", zCONTROL_STATUS_ENABLED, nIsCheckedOut )
   SetCtrlState( vSubtask, "pbSetXtoOne", zCONTROL_STATUS_ENABLED, (zLONG) nIsCheckedOut );
   //:SetCtrlState( vSubtask, "pbRemoveAll", zCONTROL_STATUS_ENABLED, nIsCheckedOut )
   SetCtrlState( vSubtask, "pbRemoveAll", zCONTROL_STATUS_ENABLED, (zLONG) nIsCheckedOut );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:RemoveAll ( VIEW vSubtask )
//:   VIEW TZTENVRO      REGISTERED AS TZTENVRO
zOPER_EXPORT zSHORT OPERATION
RemoveAll( zVIEW     vSubtask )
{
   zVIEW     TZTENVRO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLODO      REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:VIEW TZZOLODO_Hier BASED ON LOD  TZZOLODO
   zVIEW     TZZOLODO_Hier = 0; 
   //:STRING ( 100 ) szEntityName
   zCHAR     szEntityName[ 101 ] = { 0 }; 
   //:SHORT          sReturnLevel
   zSHORT    sReturnLevel = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   //:INTEGER        lAbsPos
   zLONG     lAbsPos = 0; 
   //:INTEGER        lDataSourceZKey
   zLONG     lDataSourceZKey = 0; 

   RESULT = GetViewByName( &TZTENVRO, "TZTENVRO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:CreateViewFromViewForTask( TZZOLODO, TZZOLODO, vSubtask )
   CreateViewFromViewForTask( &TZZOLODO, TZZOLODO, vSubtask );
   //:ResetView( TZZOLODO )
   ResetView( TZZOLODO );

   //:CreateViewFromViewForTask( TZZOLODO_Hier, TZZOLODO, vSubtask )
   CreateViewFromViewForTask( &TZZOLODO_Hier, TZZOLODO, vSubtask );

   //:szEntityName    = "LOD_EntityParent"
   ZeidonStringCopy( szEntityName, 1, 0, "LOD_EntityParent", 1, 0, 101 );
   //:lDataSourceZKey = TZTENVRO.TE_DBMS_Source.ZKey
   GetIntegerFromAttribute( &lDataSourceZKey, TZTENVRO, "TE_DBMS_Source", "ZKey" );

   //:nRC = DefineHierarchicalCursor( TZZOLODO_Hier, "LOD_EntityParent" )
   nRC = DefineHierarchicalCursor( TZZOLODO_Hier, "LOD_EntityParent" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 

      //: IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //: SetViewToSubobject( TZZOLODO_Hier, "LOD_EntityChild" )
         SetViewToSubobject( TZZOLODO_Hier, "LOD_EntityChild" );
      } 

      //: END

      //: // Set up our temp view from the hier view.
      //: GetAbsolutePositionForEntity( lAbsPos, TZZOLODO_Hier, "LOD_EntityParent" )
      GetAbsolutePositionForEntity( &lAbsPos, TZZOLODO_Hier, "LOD_EntityParent" );
      //: SetCursorAbsolutePosition( szEntityName, TZZOLODO, lAbsPos )
      SetCursorAbsolutePosition( szEntityName, TZZOLODO, lAbsPos );

      //: IF szEntityName = "LOD_EntityParent"
      if ( ZeidonStringCompare( szEntityName, 1, 0, "LOD_EntityParent", 1, 0, 101 ) == 0 )
      { 

         //:  SET CURSOR FIRST TZZOLODO.POD_Entity
         //:             WHERE TZZOLODO.TE_DBMS_SourceForEntity.ZKey = lDataSourceZKey
         RESULT = SetCursorFirstEntity( TZZOLODO, "POD_Entity", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToInteger( TZZOLODO, "TE_DBMS_SourceForEntity", "ZKey", lDataSourceZKey ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZZOLODO, "POD_Entity", "" );
            } 

         } 


         //:  IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:  TZZOLODO.POD_Entity.SQL_JoinWithParent = "N"
            SetAttributeFromString( TZZOLODO, "POD_Entity", "SQL_JoinWithParent", "N" );
         } 

         //:  END
      } 

      //: END

      //: nRC = SetCursorNextEntityHierarchical( sReturnLevel, szEntityName, TZZOLODO_Hier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &sReturnLevel, szEntityName, TZZOLODO_Hier );
   } 

   //:END

   //:DropView( TZZOLODO_Hier )
   DropView( TZZOLODO_Hier );
   //:DropView( TZZOLODO )
   DropView( TZZOLODO );

   //:RefreshCtrl( vSubtask, "JoinCheck" )
   RefreshCtrl( vSubtask, "JoinCheck" );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
