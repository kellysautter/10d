#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
#include "TZ__OPRS.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
List_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_PostBuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SQL_DBH_RemoveAll( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_EntityAboutToBeChanged( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_BeforeEntityChange( zVIEW     vSubtask );


static zSHORT
o_fnCountAttributes( zVIEW     TZZOLODO,
                     zPUSHORT   AttribCount );


static zSHORT
o_fnCountJoinedAttributes( zVIEW     TZZOLODO,
                           zPUSHORT   usAttribCount,
                           zSHORT    bIncludeRoot );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_SetX_ToOne( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SQL_DBH_SetX_ToOne( zVIEW     vSubtask );


static zSHORT
o_fnCheckIfJoinIsOK( zVIEW     TZZOLODO_Orig,
                     zSHORT    bDisplayMessage );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_JoinSelected( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_OK( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_Cancel( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_EntityChanged( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_EntityChanged( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SqlSO_TE_Main_Close( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SqlSO_TE_Main_Cancel( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SqlSO_TE_Main_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
DB2_ApplA_Main_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
DB2_ApplA_Main_Close( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_PostBuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ODBC_StoreOI_InBlob( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
GoToSubDialog( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SaveList( zVIEW     vSubtask );


//:DIALOG OPERATION
//:List_Prebuild( VIEW vSubtask )

//:   VIEW TZTEDBLO BASED ON LOD TZTEDBLO
zOPER_EXPORT zSHORT OPERATION
List_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZTEDBLO = 0; 
   zSHORT    RESULT; 


   //:ActivateOI_FromFile( TZTEDBLO, "TZTEDBLO", vSubtask, "tztedblo.xdl",
   //:                     zMULTIPLE )
   ActivateOI_FromFile( &TZTEDBLO, "TZTEDBLO", vSubtask, "tztedblo.xdl", zMULTIPLE );
   //:IF TZTEDBLO = 0
   if ( TZTEDBLO == 0 )
   { 
      //:ACTIVATE TZTEDBLO EMPTY
      RESULT = ActivateEmptyObjectInstance( &TZTEDBLO, "TZTEDBLO", vSubtask, zSINGLE );
   } 

   //:END

   //:NAME VIEW TZTEDBLO "TZTEDBLO"
   SetNameForView( TZTEDBLO, "TZTEDBLO", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SQL_DBH_LOD_Prebuild( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_Prebuild( zVIEW     vSubtask )
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
//:SQL_DBH_LOD_PostBuild( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_PostBuild( zVIEW     vSubtask )
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
//:SQL_DBH_RemoveAll( VIEW vSubtask )
//:   VIEW TZTENVRO      REGISTERED AS TZTENVRO
zOPER_EXPORT zSHORT OPERATION
SQL_DBH_RemoveAll( zVIEW     vSubtask )
{
   zVIEW     TZTENVRO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLODO      REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:VIEW TZZOLODO_Hier BASED ON LOD TZZOLODO
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
            //:  TZZOLODO.POD_Entity.SQL_ActivateInOne = "N"
            SetAttributeFromString( TZZOLODO, "POD_Entity", "SQL_ActivateInOne", "N" );
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

   //:RefreshCtrl( vSubtask, "SQL_DBH_JoinCheck" )
   RefreshCtrl( vSubtask, "SQL_DBH_JoinCheck" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ODBC_LOD_EntityAboutToBeChanged( VIEW vSubtask )

//:   VIEW  TZDBHODO  REGISTERED AS ODBC_EntityInfo
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_EntityAboutToBeChanged( zVIEW     vSubtask )
{
   zVIEW     TZDBHODO = 0; 
   zSHORT    RESULT; 
   //:VIEW  TZZOLODO  REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:SHORT nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 

   RESULT = GetViewByName( &TZDBHODO, "ODBC_EntityInfo", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:// Check to see if there is already an ODBC object loaded.  If it is then store it
   //:// in the blob.
   //:IF TZDBHODO != 0
   if ( TZDBHODO != 0 )
   { 
      //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META )
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META );

      //:IF nIsCheckedOut = 1
      if ( nIsCheckedOut == 1 )
      { 
         //:SetBlobFromOI( TZZOLODO, "POD_Entity", "DBH_Info", TZDBHODO, 0 )
         SetBlobFromOI( TZZOLODO, "POD_Entity", "DBH_Info", TZDBHODO, 0 );
      } 

      //:END
      //:DropView( TZDBHODO )
      DropView( TZDBHODO );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SQL_DBH_LOD_BeforeEntityChange( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_BeforeEntityChange( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:// Map the control, neccesary as be have "Bypass Input Data Mapping"
   //:// TZZOLODO pointing to the "old" Entity
   //:IF  TZZOLODO.POD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //: MapCtrl( vSubtask, "SQL_DBH_JoinCheck" )
      MapCtrl( vSubtask, "SQL_DBH_JoinCheck" );
      //: MapCtrl( vSubtask, "SQL_DBH_OneSelCheck" )
      MapCtrl( vSubtask, "SQL_DBH_OneSelCheck" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
o_fnCountAttributes( zVIEW     TZZOLODO,
                     zPUSHORT   AttribCount )
{
   zSHORT    RESULT; 

   //:fnCountAttributes( VIEW  TZZOLODO,
   //:                SHORT AttribCount )

   //: FOR EACH TZZOLODO.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( TZZOLODO, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:  IF TZZOLODO.LOD_AttributeRec.Work != "Y" AND
      //:     TZZOLODO.LOD_AttributeRec.Hidden != "Y"
      if ( CompareAttributeToString( TZZOLODO, "LOD_AttributeRec", "Work", "Y" ) != 0 && CompareAttributeToString( TZZOLODO, "LOD_AttributeRec", "Hidden", "Y" ) != 0 )
      { 

         //:   AttribCount = AttribCount + 1
         *AttribCount = *AttribCount + 1;
      } 

      RESULT = SetCursorNextEntity( TZZOLODO, "LOD_AttributeRec", "" );
      //:  END
   } 

   //: END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:fnCountJoinedAttributes( VIEW TZZOLODO BASED ON LOD TZZOLODO,
//:                         SHORT usAttribCount,
//:                         SHORT bIncludeRoot )

//:   VIEW TZDBHODO BASED ON LOD TZDBHODO
static zSHORT
o_fnCountJoinedAttributes( zVIEW     TZZOLODO,
                           zPUSHORT   usAttribCount,
                           zSHORT    bIncludeRoot )
{
   zVIEW     TZDBHODO = 0; 
   //:STRING ( 32 )  szOD_Name
   zCHAR     szOD_Name[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 


   //:// If bIncludeRoot is 0, then we only want entities that have the
   //:// Join flag set.
   //:IF  bIncludeRoot = 0
   if ( bIncludeRoot == 0 )
   { 
      //: IF TZZOLODO.POD_Entity DOES NOT EXIST
      lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
      if ( lTempInteger_0 != 0 )
      { 
         //:  RETURN 0
         return( 0 );
      } 

      //: END

      //: nRC = SetOI_FromBlob( TZDBHODO, szOD_Name, TZZOLODO,
      //:                       TZZOLODO, "POD_Entity", "DBH_Info", zNOI_OKAY )
      nRC = SetOI_FromBlob( &TZDBHODO, szOD_Name, TZZOLODO, TZZOLODO, "POD_Entity", "DBH_Info", zNOI_OKAY );
      //: IF  nRC = -1
      if ( nRC == -1 )
      { 
         //:  RETURN 0
         return( 0 );
      } 

      //: END

      //: IF  TZDBHODO.ODBC.JoinWithParent != "Y"
      if ( CompareAttributeToString( TZDBHODO, "ODBC", "JoinWithParent", "Y" ) != 0 )
      { 
         //:  DropView( TZDBHODO )
         DropView( TZDBHODO );
         //:  RETURN 0
         return( 0 );
      } 

      //: END

      //: DropView( TZDBHODO )
      DropView( TZDBHODO );
   } 


   //:END

   //:fnCountAttributes( TZZOLODO, usAttribCount )
   o_fnCountAttributes( TZZOLODO, (zPUSHORT) usAttribCount );

   //:FOR EACH TZZOLODO.LOD_EntityChild
   RESULT = SetCursorFirstEntity( TZZOLODO, "LOD_EntityChild", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: SetViewToSubobject( TZZOLODO, "LOD_EntityChild" )
      SetViewToSubobject( TZZOLODO, "LOD_EntityChild" );
      //: fnCountJoinedAttributes( TZZOLODO, usAttribCount, 0 )
      o_fnCountJoinedAttributes( TZZOLODO, (zPUSHORT) usAttribCount, 0 );
      //: ResetViewFromSubobject( TZZOLODO )
      ResetViewFromSubobject( TZZOLODO );
      RESULT = SetCursorNextEntity( TZZOLODO, "LOD_EntityChild", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ODBC_LOD_SetX_ToOne( VIEW vSubtask )

//:   VIEW TZTENVRO      REGISTERED AS TZTENVRO
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_SetX_ToOne( zVIEW     vSubtask )
{
   zVIEW     TZTENVRO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLODO      REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:VIEW TZZOLODO_Hier BASED ON LOD TZZOLODO
   zVIEW     TZZOLODO_Hier = 0; 
   //:VIEW TZDBHODO      BASED ON LOD TZDBHODO
   zVIEW     TZDBHODO = 0; 
   //:STRING ( 100 ) szEntityName
   zCHAR     szEntityName[ 101 ] = { 0 }; 
   //:SHORT sReturnLevel
   zSHORT    sReturnLevel = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:INTEGER lAbsPos
   zLONG     lAbsPos = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TZTENVRO, "TZTENVRO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:CreateViewFromViewForTask( TZZOLODO_Hier, TZZOLODO, vSubtask )
   CreateViewFromViewForTask( &TZZOLODO_Hier, TZZOLODO, vSubtask );
   //:CreateViewFromViewForTask( TZZOLODO, TZZOLODO, vSubtask )
   CreateViewFromViewForTask( &TZZOLODO, TZZOLODO, vSubtask );
   //:szEntityName = "LOD_EntityParent"
   ZeidonStringCopy( szEntityName, 1, 0, "LOD_EntityParent", 1, 0, 101 );

   //:// Set up an OI that will be stored in each of the LOD_EntityParents
   //:// that do not have a POD_Entity.
   //:ACTIVATE  TZDBHODO EMPTY
   RESULT = ActivateEmptyObjectInstance( &TZDBHODO, "TZDBHODO", vSubtask, zSINGLE );
   //:CREATE ENTITY TZDBHODO.ODBC
   RESULT = CreateEntity( TZDBHODO, "ODBC", zPOS_AFTER );
   //:TZDBHODO.ODBC.JoinWithParent = "Y"
   SetAttributeFromString( TZDBHODO, "ODBC", "JoinWithParent", "Y" );

   //:nRC = DefineHierarchicalCursor( TZZOLODO_Hier, "LOD_EntityParent" )
   nRC = DefineHierarchicalCursor( TZZOLODO_Hier, "LOD_EntityParent" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 

      //: IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:  SetViewToSubobject( TZZOLODO_Hier, "LOD_EntityChild" )
         SetViewToSubobject( TZZOLODO_Hier, "LOD_EntityChild" );
      } 

      //: END

      //: // Set up our temp view from the hier view.
      //: GetAbsolutePositionForEntity( lAbsPos, TZZOLODO_Hier, "LOD_EntityParent" )
      GetAbsolutePositionForEntity( &lAbsPos, TZZOLODO_Hier, "LOD_EntityParent" );
      //: SetCursorAbsolutePosition( szEntityName, TZZOLODO, lAbsPos )
      SetCursorAbsolutePosition( szEntityName, TZZOLODO, lAbsPos );

      //: IF szEntityName = "LOD_EntityParent" AND
      //:                     TZZOLODO.ER_RelLinkRec EXISTS AND
      lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "ER_RelLinkRec" );
      //:                     TZZOLODO.ER_RelLinkRec.CardMax = 1 AND
      //:                     TZZOLODO.LOD_EntityParent.Work != "Y" AND
      //:                     TZZOLODO.LOD_EntityParent.Derived != "Y" AND
      //:                     TZZOLODO.POD_Entity DOES NOT EXIST
      lTempInteger_1 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
      if ( ZeidonStringCompare( szEntityName, 1, 0, "LOD_EntityParent", 1, 0, 101 ) == 0 && lTempInteger_0 == 0 && CompareAttributeToInteger( TZZOLODO, "ER_RelLinkRec", "CardMax", 1 ) == 0 &&
           CompareAttributeToString( TZZOLODO, "LOD_EntityParent", "Work", "Y" ) != 0 && CompareAttributeToString( TZZOLODO, "LOD_EntityParent", "Derived", "Y" ) != 0 && lTempInteger_1 != 0 )
      { 

         //:  CREATE ENTITY TZZOLODO.POD_Entity
         RESULT = CreateEntity( TZZOLODO, "POD_Entity", zPOS_AFTER );

         //:  INCLUDE TZZOLODO.TE_DBMS_SourceForEntity
         //:      FROM TZTENVRO.TE_DBMS_Source
         RESULT = IncludeSubobjectFromSubobject( TZZOLODO, "TE_DBMS_SourceForEntity", TZTENVRO, "TE_DBMS_Source", zPOS_AFTER );

         //:  SetBlobFromOI( TZZOLODO, "POD_Entity", "DBH_Info", TZDBHODO, 0 )
         SetBlobFromOI( TZZOLODO, "POD_Entity", "DBH_Info", TZDBHODO, 0 );
      } 

      //: END

      //: nRC = SetCursorNextEntityHierarchical( sReturnLevel, szEntityName, TZZOLODO_Hier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &sReturnLevel, szEntityName, TZZOLODO_Hier );
   } 

   //:END

   //:DropView( TZDBHODO )
   DropView( TZDBHODO );
   //:DropView( TZZOLODO_Hier )
   DropView( TZZOLODO_Hier );
   //:DropView( TZZOLODO )
   DropView( TZZOLODO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SQL_DBH_SetX_ToOne( VIEW vSubtask )

//:   VIEW TZTENVRO      REGISTERED AS TZTENVRO
zOPER_EXPORT zSHORT OPERATION
SQL_DBH_SetX_ToOne( zVIEW     vSubtask )
{
   zVIEW     TZTENVRO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLODO      REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:VIEW TZZOLODO_Hier BASED ON LOD TZZOLODO
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
   zSHORT    lTempInteger_0; 

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

      //: IF szEntityName = "LOD_EntityParent" AND
      //:                     TZZOLODO.ER_RelLinkRec EXISTS AND
      lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "ER_RelLinkRec" );
      //:                     TZZOLODO.ER_RelLinkRec.CardMax = 1 AND
      //:                     TZZOLODO.LOD_EntityParent.Work != "Y" AND
      //:                     TZZOLODO.LOD_EntityParent.Derived != "Y"
      if ( ZeidonStringCompare( szEntityName, 1, 0, "LOD_EntityParent", 1, 0, 101 ) == 0 && lTempInteger_0 == 0 && CompareAttributeToInteger( TZZOLODO, "ER_RelLinkRec", "CardMax", 1 ) == 0 &&
           CompareAttributeToString( TZZOLODO, "LOD_EntityParent", "Work", "Y" ) != 0 && CompareAttributeToString( TZZOLODO, "LOD_EntityParent", "Derived", "Y" ) != 0 )
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


         //:  IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:  CREATE ENTITY TZZOLODO.POD_Entity
            RESULT = CreateEntity( TZZOLODO, "POD_Entity", zPOS_AFTER );

            //:  INCLUDE TZZOLODO.TE_DBMS_SourceForEntity
            //:          FROM TZTENVRO.TE_DBMS_Source
            RESULT = IncludeSubobjectFromSubobject( TZZOLODO, "TE_DBMS_SourceForEntity", TZTENVRO, "TE_DBMS_Source", zPOS_AFTER );
         } 

         //:  END

         //:  TZZOLODO.POD_Entity.SQL_JoinWithParent = "Y"
         SetAttributeFromString( TZZOLODO, "POD_Entity", "SQL_JoinWithParent", "Y" );
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

   //:RefreshCtrl( vSubtask, "SQL_DBH_JoinCheck" )
   RefreshCtrl( vSubtask, "SQL_DBH_JoinCheck" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:fnCheckIfJoinIsOK( VIEW TZZOLODO_Orig, SHORT bDisplayMessage )

//:   VIEW TZZOLODO BASED ON LOD TZZOLODO
static zSHORT
o_fnCheckIfJoinIsOK( zVIEW     TZZOLODO_Orig,
                     zSHORT    bDisplayMessage )
{
   zVIEW     TZZOLODO = 0; 
   //:VIEW TZDBHODO BASED ON LOD TZDBHODO
   zVIEW     TZDBHODO = 0; 
   //:STRING ( 200 ) szMessage
   zCHAR     szMessage[ 201 ] = { 0 }; 
   //:STRING ( 32 )  szOD_Name
   zCHAR     szOD_Name[ 33 ] = { 0 }; 
   //:SHORT bCountAttribs
   zSHORT    bCountAttribs = 0; 
   //:SHORT AttribCount
   zSHORT    AttribCount = 0; 
   //:SHORT bDone
   zSHORT    bDone = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_0[ 1026 ]; 


   //:// Create a copy so we can muck with the cursors.
   //:CreateViewFromViewForTask( TZZOLODO, TZZOLODO_Orig, TZZOLODO_Orig )
   CreateViewFromViewForTask( &TZZOLODO, TZZOLODO_Orig, TZZOLODO_Orig );

   //:AttribCount = 0
   AttribCount = 0;
   //:bCountAttribs = 1   // Assume we need to count the attributes.
   bCountAttribs = 1;
   //:IF TZZOLODO.POD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //: nRC = SetOI_FromBlob( TZDBHODO, szOD_Name, TZZOLODO_Orig,
      //:                       TZZOLODO, "POD_Entity", "DBH_Info", zNOI_OKAY )
      nRC = SetOI_FromBlob( &TZDBHODO, szOD_Name, TZZOLODO_Orig, TZZOLODO, "POD_Entity", "DBH_Info", zNOI_OKAY );
      //: IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:  IF TZDBHODO.ODBC.JoinWithParent = "Y"
         if ( CompareAttributeToString( TZDBHODO, "ODBC", "JoinWithParent", "Y" ) == 0 )
         { 
            //:  // The current LOD_EntityParent has the join flag turned on.  This
            //:  // means that when we call fnCountJoinedAttributes() later that
            //:  // these attributes will be counted, so we don't need to count
            //:  // them now.
            //:  bCountAttribs = 0
            bCountAttribs = 0;
         } 

         //:  END

         //:  DropView( TZDBHODO )
         DropView( TZDBHODO );
      } 

      //: END
   } 

   //:END

   //:IF bCountAttribs = 1
   if ( bCountAttribs == 1 )
   { 
      //: // Current LOD_EntityParent doesn't have the Join flag set.  This
      //: // will throw off our attrib count later unless we count the
      //: // attribs that are part of this entity and it's children, so
      //: // let's do it now.
      //: fnCountJoinedAttributes( TZZOLODO, AttribCount, 1 )
      o_fnCountJoinedAttributes( TZZOLODO, (zPUSHORT) &AttribCount, 1 );
   } 

   //:END

   //:// If we can not reset the view from the subobject then we must be dealing
   //:// with the root entity.  Join is invalid for the root entity.
   //:IF  ResetViewFromSubobject( TZZOLODO ) = 1
   lTempInteger_1 = ResetViewFromSubobject( TZZOLODO );
   if ( lTempInteger_1 == 1 )
   { 
      //: IF  bDisplayMessage != 0
      if ( bDisplayMessage != 0 )
      { 
         //: MessageSend( TZZOLODO, "DBH10001", "DBH Error",
         //:              "Join=Y is not allowed for the root entity.",
         //:              zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( TZZOLODO, "DBH10001", "DBH Error", "Join=Y is not allowed for the root entity.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      } 

      //: END

      //: DropView( TZZOLODO )
      DropView( TZZOLODO );
      //: RETURN 0
      return( 0 );
   } 

   //:END
   do 
   { 

      //:// Find the top-most parent with the Join flag set.
      //:LOOP
      //: bDone = 1  // Assume that current entity doesn't have flag set.
      bDone = 1;

      //: IF TZZOLODO.POD_Entity EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
      if ( lTempInteger_2 == 0 )
      { 
         //:  nRC = SetOI_FromBlob( TZDBHODO, szOD_Name, TZZOLODO_Orig,
         //:                        TZZOLODO, "POD_Entity", "DBH_Info", zNOI_OKAY )
         nRC = SetOI_FromBlob( &TZDBHODO, szOD_Name, TZZOLODO_Orig, TZZOLODO, "POD_Entity", "DBH_Info", zNOI_OKAY );
         //:  IF  nRC >= 0
         if ( nRC >= 0 )
         { 
            //:   IF TZDBHODO.ODBC.JoinWithParent = "Y"
            if ( CompareAttributeToString( TZDBHODO, "ODBC", "JoinWithParent", "Y" ) == 0 )
            { 
               //:    bDone = 0
               bDone = 0;

               //:    // Check parent for join flag.
               //:    ResetViewFromSubobject( TZZOLODO )
               ResetViewFromSubobject( TZZOLODO );
            } 

            //:   END

            //:   DropView( TZDBHODO )
            DropView( TZDBHODO );
         } 

         //:  END
      } 

      //: END
      //:UNTIL bDone = 1
   }  while ( ! ( bDone == 1 ) );


   //:// Check to see if there are too many attributes.
   //:fnCountJoinedAttributes( TZZOLODO, AttribCount, 1 )
   o_fnCountJoinedAttributes( TZZOLODO, (zPUSHORT) &AttribCount, 1 );
   //:IF AttribCount > 220
   if ( AttribCount > 220 )
   { 
      //: IF  bDisplayMessage != 0
      if ( bDisplayMessage != 0 )
      { 
         //:  szMessage = "Setting Join=Y for entity " +
         //:              TZZOLODO_Orig.LOD_EntityParent.Name +
         //:              " will create a join with too many attributes."
         GetVariableFromAttribute( szTempString_0, 0, 'S', 1026, TZZOLODO_Orig, "LOD_EntityParent", "Name", "", 0 );
         ZeidonStringCopy( szMessage, 1, 0, "Setting Join=Y for entity ", 1, 0, 201 );
         ZeidonStringConcat( szMessage, 1, 0, szTempString_0, 1, 0, 201 );
         ZeidonStringConcat( szMessage, 1, 0, " will create a join with too many attributes.", 1, 0, 201 );
         //:  MessageSend( TZZOLODO, "DBH10002", "DBH Error", szMessage,
         //:               zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( TZZOLODO, "DBH10002", "DBH Error", szMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      } 

      //: END

      //: DropView( TZZOLODO )
      DropView( TZZOLODO );
      //: RETURN 0
      return( 0 );
   } 

   //:END

   //:// Clean up and return.
   //:DropView( TZZOLODO )
   DropView( TZZOLODO );

   //:RETURN 1
   return( 1 );
// END
} 


//:DIALOG OPERATION
//:ODBC_LOD_JoinSelected( VIEW vSubtask )

//:   VIEW TZDBHODO REGISTERED AS ODBC_EntityInfo
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_JoinSelected( zVIEW     vSubtask )
{
   zVIEW     TZDBHODO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZZOLODO REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZDBHODO, "ODBC_EntityInfo", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:IF  TZDBHODO.ODBC.JoinWithParent = "Y"
   if ( CompareAttributeToString( TZDBHODO, "ODBC", "JoinWithParent", "Y" ) == 0 )
   { 
      //: IF fnCheckIfJoinIsOK( TZZOLODO, 1 ) = 0
      lTempInteger_0 = o_fnCheckIfJoinIsOK( TZZOLODO, 1 );
      if ( lTempInteger_0 == 0 )
      { 
         //:  // We got an error.  Turn off the check.
         //:  SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_CHECKED, 0 )
         SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_CHECKED, 0 );
         //:  TZDBHODO.ODBC.JoinWithParent = "N"
         SetAttributeFromString( TZDBHODO, "ODBC", "JoinWithParent", "N" );
         //:  RETURN 0
         return( 0 );
      } 

      //: END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ODBC_LOD_OK( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_OK( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:ODBC_LOD_EntityAboutToBeChanged( vSubtask )
   ODBC_LOD_EntityAboutToBeChanged( vSubtask );

   //:ResetView( TZZOLODO )
   ResetView( TZZOLODO );
   //:AcceptSubobject( TZZOLODO, "LOD_EntityParent" )
   AcceptSubobject( TZZOLODO, "LOD_EntityParent" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ODBC_LOD_Cancel( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_Cancel( zVIEW     vSubtask )
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
//:ODBC_LOD_Prebuild( VIEW vSubtask )

//:   VIEW   TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZZOLODO = 0; 
   zSHORT    RESULT; 
   //:SHORT  nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 

   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //:ResetView( TZZOLODO )
   ResetView( TZZOLODO );
   //:CreateTemporalSubobjectVersion( TZZOLODO, "LOD_EntityParent" )
   CreateTemporalSubobjectVersion( TZZOLODO, "LOD_EntityParent" );

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META );

   //:IF nIsCheckedOut = 1
   if ( nIsCheckedOut == 1 )
   { 
      //:// Turn on the JOIN flag for all NEW entities that have a x-to-one relationship
      //:// with it's parent.  "NEW" entities are entities that do not have a POD_Entity.
      //:ODBC_LOD_SetX_ToOne( vSubtask )
      ODBC_LOD_SetX_ToOne( vSubtask );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SQL_DBH_LOD_EntityChanged( VIEW vSubtask )

//:   VIEW TZZOLODO  REGISTERED AS TZZOLODO
zOPER_EXPORT zSHORT OPERATION
SQL_DBH_LOD_EntityChanged( zVIEW     vSubtask )
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
            //:TZZOLODO.POD_Entity.SQL_ActivateInOne = "N"
            SetAttributeFromString( TZZOLODO, "POD_Entity", "SQL_ActivateInOne", "N" );
            //:INCLUDE TZZOLODO.TE_DBMS_SourceForEntity FROM TZTENVRO.TE_DBMS_Source
            RESULT = IncludeSubobjectFromSubobject( TZZOLODO, "TE_DBMS_SourceForEntity", TZTENVRO, "TE_DBMS_Source", zPOS_AFTER );
         } 

         //:END

         //:// not the root and not work and not derived
         //:SetCtrlState( vSubtask,      "SQL_DBH_JoinCheck", zCONTROL_STATUS_ENABLED, TRUE )
         SetCtrlState( vSubtask, "SQL_DBH_JoinCheck", zCONTROL_STATUS_ENABLED, TRUE );
         //:SetCtrlState( vSubtask,      "SQL_DBH_OneSelCheck", zCONTROL_STATUS_ENABLED, TRUE )
         SetCtrlState( vSubtask, "SQL_DBH_OneSelCheck", zCONTROL_STATUS_ENABLED, TRUE );
         //:ELSE
      } 
      else
      { 
         //:SetCtrlState( vSubtask,      "SQL_DBH_JoinCheck", zCONTROL_STATUS_ENABLED, FALSE )
         SetCtrlState( vSubtask, "SQL_DBH_JoinCheck", zCONTROL_STATUS_ENABLED, FALSE );
         //:SetCtrlState( vSubtask,      "SQL_DBH_OneSelCheck", zCONTROL_STATUS_ENABLED, FALSE )
         SetCtrlState( vSubtask, "SQL_DBH_OneSelCheck", zCONTROL_STATUS_ENABLED, FALSE );
      } 

      //:END

      //:// Refresh checkbox and set the string being displayed.
      //:IF  TZZOLODO.POD_Entity EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( TZZOLODO, "POD_Entity" );
      if ( lTempInteger_1 == 0 )
      { 
         //: TZZOLODO.LOD_EntityParent.WorkString = TZZOLODO.LOD_EntityParent.Name +
         //:                                        " JOIN = " + TZZOLODO.POD_Entity.SQL_JoinWithParent
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZZOLODO, "LOD_EntityParent", "Name" );
         ZeidonStringConcat( szTempString_0, 1, 0, " JOIN = ", 1, 0, 201 );
         GetVariableFromAttribute( szTempString_1, 0, 'S', 2, TZZOLODO, "POD_Entity", "SQL_JoinWithParent", "", 0 );
         ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 201 );
         SetAttributeFromString( TZZOLODO, "LOD_EntityParent", "WorkString", szTempString_0 );
         //:ELSE
      } 
      else
      { 
         //: TZZOLODO.LOD_EntityParent.WorkString = TZZOLODO.LOD_EntityParent.Name + " JOIN = N"
         GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), TZZOLODO, "LOD_EntityParent", "Name" );
         ZeidonStringConcat( szTempString_2, 1, 0, " JOIN = N", 1, 0, 201 );
         SetAttributeFromString( TZZOLODO, "LOD_EntityParent", "WorkString", szTempString_2 );
      } 

      //:END
   } 

   //:END

   //:RefreshCtrl( vSubtask, "SQL_DBH_JoinCheck" )
   RefreshCtrl( vSubtask, "SQL_DBH_JoinCheck" );
   //:RefreshCtrl( vSubtask, "SQL_DBH_OneSelCheck" )
   RefreshCtrl( vSubtask, "SQL_DBH_OneSelCheck" );
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
//:ODBC_LOD_EntityChanged( VIEW vSubtask )

//:   VIEW  TZDBHODO  REGISTERED AS ODBC_EntityInfo
zOPER_EXPORT zSHORT OPERATION
ODBC_LOD_EntityChanged( zVIEW     vSubtask )
{
   zVIEW     TZDBHODO = 0; 
   zSHORT    RESULT; 
   //:VIEW  TZZOLODO  REGISTERED AS TZZOLODO
   zVIEW     TZZOLODO = 0; 
   //:VIEW  TZTENVRO  REGISTERED AS TZTENVRO
   zVIEW     TZTENVRO = 0; 
   //:STRING ( 32 )  szOD_Name
   zCHAR     szOD_Name[ 33 ] = { 0 }; 
   //:SHORT nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 201 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 201 ]; 
   zCHAR     szTempString_2[ 2 ]; 

   RESULT = GetViewByName( &TZDBHODO, "ODBC_EntityInfo", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZTENVRO, "TZTENVRO", vSubtask, zLEVEL_TASK );

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, TZZOLODO, zSOURCE_LOD_META );
   //:SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_ENABLED, FALSE )
   SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_ENABLED, FALSE );

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
         //:    WHERE  TZZOLODO.TE_DBMS_SourceForEntity.ZKey = TZTENVRO.TE_DBMS_Source.ZKey
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
            //:INCLUDE TZZOLODO.TE_DBMS_SourceForEntity FROM TZTENVRO.TE_DBMS_Source
            RESULT = IncludeSubobjectFromSubobject( TZZOLODO, "TE_DBMS_SourceForEntity", TZTENVRO, "TE_DBMS_Source", zPOS_AFTER );

            //:ACTIVATE TZDBHODO EMPTY
            RESULT = ActivateEmptyObjectInstance( &TZDBHODO, "TZDBHODO", vSubtask, zSINGLE );
            //:CREATE ENTITY TZDBHODO.ODBC
            RESULT = CreateEntity( TZDBHODO, "ODBC", zPOS_AFTER );

            //:// Now set the string being displayed.
            //:TZZOLODO.LOD_EntityParent.WorkString = TZZOLODO.LOD_EntityParent.Name + "JOIN = "
            GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZZOLODO, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szTempString_0, 1, 0, "JOIN = ", 1, 0, 201 );
            SetAttributeFromString( TZZOLODO, "LOD_EntityParent", "WorkString", szTempString_0 );
            //:ELSE
         } 
         else
         { 
            //:// POD_Entity exists so activate the ODBC object from the blob.
            //:SetOI_FromBlob( TZDBHODO, szOD_Name, vSubtask,
            //:                TZZOLODO, "POD_Entity", "DBH_Info", zMULTIPLE )
            SetOI_FromBlob( &TZDBHODO, szOD_Name, vSubtask, TZZOLODO, "POD_Entity", "DBH_Info", zMULTIPLE );

            //:// Make sure the ODBC OI exists and has a root entity.
            //:IF TZDBHODO = 0
            if ( TZDBHODO == 0 )
            { 
               //:ACTIVATE TZDBHODO EMPTY
               RESULT = ActivateEmptyObjectInstance( &TZDBHODO, "TZDBHODO", vSubtask, zSINGLE );
            } 

            //:END

            //:IF TZDBHODO.ODBC DOES NOT EXIST
            lTempInteger_1 = CheckExistenceOfEntity( TZDBHODO, "ODBC" );
            if ( lTempInteger_1 != 0 )
            { 
               //:CREATE ENTITY TZDBHODO.ODBC
               RESULT = CreateEntity( TZDBHODO, "ODBC", zPOS_AFTER );
            } 

            //:END

            //:// Now set the string being displayed.
            //:TZZOLODO.LOD_EntityParent.WorkString = TZZOLODO.LOD_EntityParent.Name + "JOIN = " +
            //:                   TZDBHODO.ODBC.JoinWithParent
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TZZOLODO, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szTempString_1, 1, 0, "JOIN = ", 1, 0, 201 );
            GetVariableFromAttribute( szTempString_2, 0, 'S', 2, TZDBHODO, "ODBC", "JoinWithParent", "", 0 );
            ZeidonStringConcat( szTempString_1, 1, 0, szTempString_2, 1, 0, 201 );
            SetAttributeFromString( TZZOLODO, "LOD_EntityParent", "WorkString", szTempString_1 );
         } 

         //:END

         //:// not the root and not work and not derived
         //:SetCtrlState( vSubtask,      "JoinCheck", zCONTROL_STATUS_ENABLED, TRUE )
         SetCtrlState( vSubtask, "JoinCheck", zCONTROL_STATUS_ENABLED, TRUE );
         //:NAME VIEW TZDBHODO "ODBC_EntityInfo"
         SetNameForView( TZDBHODO, "ODBC_EntityInfo", 0, zLEVEL_TASK );
      } 

      //:END
   } 

   //:END // ENDIF nIsCheckedOut = 1

   //:// Refresh the controls in the SqlDisplay area.
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
//:SqlSO_TE_Main_Close( VIEW vSubtask )

//:   VIEW vTZTENVRO    // TE object.
zOPER_EXPORT zSHORT OPERATION
SqlSO_TE_Main_Close( zVIEW     vSubtask )
{
   zVIEW     vTZTENVRO = 0; 
   //:VIEW TZDBHODO REGISTERED AS TZDBHODO
   zVIEW     TZDBHODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZDBHODO, "TZDBHODO", vSubtask, zLEVEL_TASK );

   //:GET VIEW vTZTENVRO NAMED "TE_DB_Environ"
   RESULT = GetViewByName( &vTZTENVRO, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   //:SetBlobFromOI( vTZTENVRO, "TE_DBMS_Source", "DBH_Data", TZDBHODO, 0 )
   SetBlobFromOI( vTZTENVRO, "TE_DBMS_Source", "DBH_Data", TZDBHODO, 0 );

   //:DropView( TZDBHODO )
   DropView( TZDBHODO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SqlSO_TE_Main_Cancel( VIEW vSubtask )

//:   VIEW TZDBHODO REGISTERED AS TZDBHODO
zOPER_EXPORT zSHORT OPERATION
SqlSO_TE_Main_Cancel( zVIEW     vSubtask )
{
   zVIEW     TZDBHODO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZDBHODO, "TZDBHODO", vSubtask, zLEVEL_TASK );
   //:DropView( TZDBHODO )
   DropView( TZDBHODO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SqlSO_TE_Main_Prebuild( VIEW vSubtask )

//:   VIEW  vTZTENVRO    // TE object.
zOPER_EXPORT zSHORT OPERATION
SqlSO_TE_Main_Prebuild( zVIEW     vSubtask )
{
   zVIEW     vTZTENVRO = 0; 
   //:VIEW  vTZTEDBLO     BASED ON LOD TZTEDBLO
   zVIEW     vTZTEDBLO = 0; 
   //:VIEW  TZDBHODO      BASED ON LOD TZDBHODO
   zVIEW     TZDBHODO = 0; 
   //:VIEW  BaseTZDBHODO  BASED ON LOD TZDBHODO
   zVIEW     BaseTZDBHODO = 0; 
   //:STRING ( 32 )  szOD_Name
   zCHAR     szOD_Name[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:// Get the TE view.
   //:GET VIEW vTZTENVRO NAMED "TE_DB_Environ"
   RESULT = GetViewByName( &vTZTENVRO, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   //:// Try to pull out the DBH OI from the TE.
   //:nRC = SetOI_FromBlob( TZDBHODO, szOD_Name, vSubtask,
   //:                      vTZTENVRO, "TE_DBMS_Source", "DBH_Data",
   //:                      zNOI_OKAY )
   nRC = SetOI_FromBlob( &TZDBHODO, szOD_Name, vSubtask, vTZTENVRO, "TE_DBMS_Source", "DBH_Data", zNOI_OKAY );
   //:IF  nRC = -1
   if ( nRC == -1 )
   { 
      //:// Didn't exist so create an empty one.
      //:ACTIVATE TZDBHODO EMPTY
      RESULT = ActivateEmptyObjectInstance( &TZDBHODO, "TZDBHODO", vSubtask, zSINGLE );
      //:CREATE ENTITY TZDBHODO.ODBC
      RESULT = CreateEntity( TZDBHODO, "ODBC", zPOS_AFTER );

      //:// Copy values from TZTEDBLO object
      //:GET VIEW vTZTEDBLO NAMED "TZTEDBLO"
      RESULT = GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
      //:nRC = SetOI_FromBlob( BaseTZDBHODO, szOD_Name, vSubtask,
      //:                      vTZTEDBLO, "TE_DBMS_Source", "DBH_Data",
      //:                      zNOI_OKAY )
      nRC = SetOI_FromBlob( &BaseTZDBHODO, szOD_Name, vSubtask, vTZTEDBLO, "TE_DBMS_Source", "DBH_Data", zNOI_OKAY );

      //:IF nRC != -1
      if ( nRC != -1 )
      { 
         //:SetMatchingAttributesByName( TZDBHODO, "ODBC",
         //:                             BaseTZDBHODO, "ODBC", zSET_ALL )
         SetMatchingAttributesByName( TZDBHODO, "ODBC", BaseTZDBHODO, "ODBC", zSET_ALL );
         //:DropView( BaseTZDBHODO )
         DropView( BaseTZDBHODO );
      } 

      //:END

      //:TZDBHODO.ODBC.AddTableNameToFK_Override = vTZTEDBLO.TE_DBMS_Source.DBS_FullFK_Name
      SetAttributeFromAttribute( TZDBHODO, "ODBC", "AddTableNameToFK_Override", vTZTEDBLO, "TE_DBMS_Source", "DBS_FullFK_Name" );
   } 

   //:END

   //:IF TZDBHODO.ODBC.DropTables = ""
   if ( CompareAttributeToString( TZDBHODO, "ODBC", "DropTables", "" ) == 0 )
   { 
      //:TZDBHODO.ODBC.DropTables = "Y"
      SetAttributeFromString( TZDBHODO, "ODBC", "DropTables", "Y" );
   } 

   //:END

   //:IF TZDBHODO.ODBC.GenDropIdxs = ""
   if ( CompareAttributeToString( TZDBHODO, "ODBC", "GenDropIdxs", "" ) == 0 )
   { 
      //:TZDBHODO.ODBC.GenDropIdxs = "N"
      SetAttributeFromString( TZDBHODO, "ODBC", "GenDropIdxs", "N" );
   } 

   //:END

   //:IF TZDBHODO.ODBC.GenCreateTables = ""
   if ( CompareAttributeToString( TZDBHODO, "ODBC", "GenCreateTables", "" ) == 0 )
   { 
      //:TZDBHODO.ODBC.GenCreateTables = "Y"
      SetAttributeFromString( TZDBHODO, "ODBC", "GenCreateTables", "Y" );
   } 

   //:END

   //:IF TZDBHODO.ODBC.GenCreateIdxs = ""
   if ( CompareAttributeToString( TZDBHODO, "ODBC", "GenCreateIdxs", "" ) == 0 )
   { 
      //:TZDBHODO.ODBC.GenCreateIdxs = "Y"
      SetAttributeFromString( TZDBHODO, "ODBC", "GenCreateIdxs", "Y" );
   } 

   //:END

   //:NAME VIEW TZDBHODO "TZDBHODO"
   SetNameForView( TZDBHODO, "TZDBHODO", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DB2_ApplA_Main_Prebuild( VIEW vSubtask )

//:   VIEW  vApSvDisO    // ApplA object.
zOPER_EXPORT zSHORT OPERATION
DB2_ApplA_Main_Prebuild( zVIEW     vSubtask )
{
   zVIEW     vApSvDisO = 0; 
   //:VIEW  TZDBHDBO BASED ON LOD TZDBHDBO
   zVIEW     TZDBHDBO = 0; 
   //:STRING ( 32 )  szOD_Name
   zCHAR     szOD_Name[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vApSvDisO NAMED "ApSvDisO"
   RESULT = GetViewByName( &vApSvDisO, "ApSvDisO", vSubtask, zLEVEL_TASK );

   //:nRC = SetOI_FromBlob( TZDBHDBO, szOD_Name, vSubtask,
   //:                      vApSvDisO, "DS_TE_SourceAssignment", "DBH_Data",
   //:                      zNOI_OKAY )
   nRC = SetOI_FromBlob( &TZDBHDBO, szOD_Name, vSubtask, vApSvDisO, "DS_TE_SourceAssignment", "DBH_Data", zNOI_OKAY );
   //:IF  nRC = -1
   if ( nRC == -1 )
   { 
      //:ACTIVATE TZDBHDBO EMPTY
      RESULT = ActivateEmptyObjectInstance( &TZDBHDBO, "TZDBHDBO", vSubtask, zSINGLE );
      //:CREATE ENTITY TZDBHDBO.Root
      RESULT = CreateEntity( TZDBHDBO, "Root", zPOS_AFTER );
      //:CREATE ENTITY TZDBHDBO.ApplA
      RESULT = CreateEntity( TZDBHDBO, "ApplA", zPOS_AFTER );
   } 

   //:END

   //:NAME VIEW TZDBHDBO "TZDBHDBO"
   SetNameForView( TZDBHDBO, "TZDBHDBO", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DB2_ApplA_Main_Close( VIEW vSubtask )

//:   VIEW vApSvDisO    // ApplA object.
zOPER_EXPORT zSHORT OPERATION
DB2_ApplA_Main_Close( zVIEW     vSubtask )
{
   zVIEW     vApSvDisO = 0; 
   //:VIEW TZDBHDBO REGISTERED AS TZDBHDBO
   zVIEW     TZDBHDBO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZDBHDBO, "TZDBHDBO", vSubtask, zLEVEL_TASK );

   //:GET VIEW vApSvDisO NAMED "ApSvDisO"
   RESULT = GetViewByName( &vApSvDisO, "ApSvDisO", vSubtask, zLEVEL_TASK );

   //:SetBlobFromOI( vApSvDisO, "DS_TE_SourceAssignment", "DBH_Data", TZDBHDBO, 0 )
   SetBlobFromOI( vApSvDisO, "DS_TE_SourceAssignment", "DBH_Data", TZDBHDBO, 0 );

   //:DropView( TZDBHDBO )
   DropView( TZDBHDBO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ODBC_PostBuild( VIEW vSubtask )

//:   VIEW TZTEDBLO REGISTERED AS TZTEDBLO
zOPER_EXPORT zSHORT OPERATION
ODBC_PostBuild( zVIEW     vSubtask )
{
   zVIEW     TZTEDBLO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZDBHODO BASED ON LOD TZDBHODO
   zVIEW     TZDBHODO = 0; 
   //:STRING ( 32 )  szOD_Name
   zCHAR     szOD_Name[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   //:nRC = SetOI_FromBlob( TZDBHODO, szOD_Name, vSubtask,
   //:                      TZTEDBLO, "TE_DBMS_Source", "DBH_Data", zNOI_OKAY )
   nRC = SetOI_FromBlob( &TZDBHODO, szOD_Name, vSubtask, TZTEDBLO, "TE_DBMS_Source", "DBH_Data", zNOI_OKAY );
   //:IF  nRC = -1
   if ( nRC == -1 )
   { 
      //:ACTIVATE  TZDBHODO EMPTY
      RESULT = ActivateEmptyObjectInstance( &TZDBHODO, "TZDBHODO", vSubtask, zSINGLE );
      //:CREATE ENTITY TZDBHODO.ODBC
      RESULT = CreateEntity( TZDBHODO, "ODBC", zPOS_AFTER );
   } 

   //:END

   //:NAME VIEW TZDBHODO "TZDBHODO"
   SetNameForView( TZDBHODO, "TZDBHODO", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ODBC_StoreOI_InBlob( VIEW vSubtask )

//:   VIEW TZTEDBLO REGISTERED AS TZTEDBLO
zOPER_EXPORT zSHORT OPERATION
ODBC_StoreOI_InBlob( zVIEW     vSubtask )
{
   zVIEW     TZTEDBLO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZDBHODO REGISTERED AS TZDBHODO
   zVIEW     TZDBHODO = 0; 

   RESULT = GetViewByName( &TZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZDBHODO, "TZDBHODO", vSubtask, zLEVEL_TASK );

   //:SetBlobFromOI( TZTEDBLO, "TE_DBMS_Source", "DBH_Data", TZDBHODO, 0 )
   SetBlobFromOI( TZTEDBLO, "TE_DBMS_Source", "DBH_Data", TZDBHODO, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GoToSubDialog( VIEW vSubtask )
//:   STRING ( 32 )  szDialog
zOPER_EXPORT zSHORT OPERATION
GoToSubDialog( zVIEW     vSubtask )
{
   zCHAR     szDialog[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szWindow
   zCHAR     szWindow[ 33 ] = { 0 }; 

   //:VIEW TZTEDBLO REGISTERED AS TZTEDBLO
   zVIEW     TZTEDBLO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   //:SET CURSOR FIRST TZTEDBLO.DBS_SubDialog
   //:   WHERE TZTEDBLO.DBS_SubDialog.Type = "MAIN"
   RESULT = SetCursorFirstEntityByString( TZTEDBLO, "DBS_SubDialog", "Type", "MAIN", "" );

   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szDialog = TZTEDBLO.DBS_SubDialog.Dialog
      GetVariableFromAttribute( szDialog, 0, 'S', 33, TZTEDBLO, "DBS_SubDialog", "Dialog", "", 0 );
      //:szWindow = TZTEDBLO.DBS_SubDialog.Window
      GetVariableFromAttribute( szWindow, 0, 'S', 33, TZTEDBLO, "DBS_SubDialog", "Window", "", 0 );

      //:SetWindowActionBehavior( vSubtask,
      //:                                              zWAB_StartModalWindow,
      //:                                              szDialog, szWindow )
      SetWindowActionBehavior( vSubtask, zWAB_StartModalWindow, szDialog, szWindow );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SaveList( VIEW vSubtask )

//:   VIEW TZTEDBLO REGISTERED AS TZTEDBLO
zOPER_EXPORT zSHORT OPERATION
SaveList( zVIEW     vSubtask )
{
   zVIEW     TZTEDBLO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   //:CommitOI_ToFile( TZTEDBLO, "tztedblo.xdl", zASCII )
   CommitOI_ToFile( TZTEDBLO, "tztedblo.xdl", zASCII );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
