#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDRVROPR.H"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZEIDONOP.H"

zLONG fnGetTickCount( zUSHORT* );

zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
Include_LinkSubobject( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditID_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditID_Clear( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditID_OK( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
Include_IncludeSubobject( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
Include_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_RefreshViewList( zVIEW     vSubtask );


static zSHORT
o_lAddViewNameToViewList( zPCHAR    szViewName );


static zSHORT
o_lAddViewNameToQualList( zPCHAR    szViewName );


zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_SaveConfiguration( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditViewName_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditLOD_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditQual_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditFile_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zSetDBH_TraceLevel( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zExecuteOperation( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zStartBrowser( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditEntitySpec_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EditQualAttrib_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_EditQualOI( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_EnableControls( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_SetOptionsFromResultLst( zVIEW     vSubtask );


static zSHORT
o_fnAddListValue( zVIEW     TZDBHTSO,
                  zPCHAR    szEntityName,
                  zPCHAR    szValue );


zOPER_EXPORT zSHORT OPERATION
EditApp_Prebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ShowTrans_Postbuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ShowTrans_RefreshArgs( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_RefreshTrans( zVIEW     vSubtask );


//:DIALOG OPERATION
//:TZDBHTSD_Prebuild( VIEW vSubtask )
//:   VIEW    TZDBHTSO BASED ON LOD TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   //:VIEW    vTemp
   zVIEW     vTemp = 0;
   //:VIEW    vDialog
   zVIEW     vDialog = 0;
   //:Integer nRC
   zLONG     nRC = 0;
   //:Integer nPos
   zLONG     nPos = 0;
   //:INTEGER nFlags
   zLONG     nFlags = 0;
   //:STRING ( 300 ) szTempDir
   zCHAR     szTempDir[ 301 ] = { 0 };
   //:STRING ( 300 ) szFileName
   zCHAR     szFileName[ 301 ] = { 0 };
   //:STRING (  30 ) szTemp
   zCHAR     szTemp[ 31 ] = { 0 };
   zSHORT    RESULT;
   zSHORT    lTempInteger_0;
   zSHORT    lTempInteger_1;


   //:NAME VIEW vSubtask "MainWindow"
   SetNameForView( vSubtask, "MainWindow", 0, zLEVEL_TASK );

   //:SysGetEnvVar( szTempDir, "TEMP", 300 )
   SysGetEnvVar( szTempDir, "TEMP", 300 );
   //:szFileName = szTempDir + "\TZDBHTSD.POR"
   ZeidonStringCopy( szFileName, 1, 0, szTempDir, 1, 0, 301 );
   ZeidonStringConcat( szFileName, 1, 0, "\\TZDBHTSD.POR", 1, 0, 301 );
   //:nFlags = zSINGLE + zNOI_OKAY
   nFlags = zSINGLE + zNOI_OKAY;
   //:nRC = ActivateOI_FromFile( TZDBHTSO, "TZDBHTSO", vSubtask, szFileName,
   //:                           nFlags )
   nRC = ActivateOI_FromFile( &TZDBHTSO, "TZDBHTSO", vSubtask, szFileName, nFlags );
   //:IF nRC < 0
   if ( nRC < 0 )
   {
      //:ActivateEmptyObjectInstance( TZDBHTSO, "TZDBHTSO", vSubtask, zSINGLE )
      ActivateEmptyObjectInstance( &TZDBHTSO, "TZDBHTSO", vSubtask, zSINGLE );
      //:CREATE ENTITY TZDBHTSO.Root
      RESULT = CreateEntity( TZDBHTSO, "Root", zPOS_AFTER );
      //:ELSE
   }
   else
   {
      //:SetSelectStateOfEntity( TZDBHTSO, "View", 1 )
      SetSelectStateOfEntity( TZDBHTSO, "View", 1 );
   }

   //:END

   //:IF TZDBHTSO.Qual DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZDBHTSO, "Qual" );
   if ( lTempInteger_0 != 0 )
   {
      //:CREATE ENTITY TZDBHTSO.Qual FIRST
      RESULT = CreateEntity( TZDBHTSO, "Qual", zPOS_FIRST );
      //:TZDBHTSO.Qual.Name = "NONE"
      SetAttributeFromString( TZDBHTSO, "Qual", "Name", "NONE" );
   }

   //:END

   //:IF TZDBHTSO.Application DOES NOT EXIST
   lTempInteger_1 = CheckExistenceOfEntity( TZDBHTSO, "Application" );
   if ( lTempInteger_1 != 0 )
   {
      //:CREATE ENTITY TZDBHTSO.Application FIRST
      RESULT = CreateEntity( TZDBHTSO, "Application", zPOS_FIRST );
      //:TZDBHTSO.Application.Name = "Zeidon_Tools"
      SetAttributeFromString( TZDBHTSO, "Application", "Name", "Zeidon_Tools" );
   }

   //:END

   //:NAME VIEW TZDBHTSO "TZDBHTSO"
   SetNameForView( TZDBHTSO, "TZDBHTSO", 0, zLEVEL_TASK );

   //:// Loop through each of the views and retrieve any that were saved.
   //:CreateViewFromViewForTask( TZDBHTSO, TZDBHTSO, vSubtask  )
   CreateViewFromViewForTask( &TZDBHTSO, TZDBHTSO, vSubtask );
   //:FOR EACH TZDBHTSO.View WHERE TZDBHTSO.View.SaveOI = "*" AND
   //:                             TZDBHTSO.View.OI_WasSaved = "Y"
   RESULT = SetCursorFirstEntity( TZDBHTSO, "View", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      if ( CompareAttributeToString( TZDBHTSO, "View", "SaveOI", "*" ) == 0 && CompareAttributeToString( TZDBHTSO, "View", "OI_WasSaved", "Y" ) == 0 )
      {
         //:nPos = GetRelativeEntityNumber( TZDBHTSO, "View", "", 0 )
         nPos = GetRelativeEntityNumber( TZDBHTSO, "View", "", 0 );
         //:ConvertIntegerToString( nPos, szTemp, 10 )
         ConvertIntegerToString( nPos, szTemp, 10 );
         //:szFileName = szTempDir + "\TZDBH" + szTemp + ".POR"
         ZeidonStringCopy( szFileName, 1, 0, szTempDir, 1, 0, 301 );
         ZeidonStringConcat( szFileName, 1, 0, "\\TZDBH", 1, 0, 301 );
         ZeidonStringConcat( szFileName, 1, 0, szTemp, 1, 0, 301 );
         ZeidonStringConcat( szFileName, 1, 0, ".POR", 1, 0, 301 );

         //:szTemp = TZDBHTSO.View.AppName
         GetVariableFromAttribute( szTemp, 0, 'S', 31, TZDBHTSO, "View", "AppName", "", 0 );
         //:SfCreateSubtask( vDialog, 0, szTemp )
         SfCreateSubtask( &vDialog, 0, szTemp );

         //:szTemp = TZDBHTSO.View.CurrentLOD
         GetVariableFromAttribute( szTemp, 0, 'S', 31, TZDBHTSO, "View", "CurrentLOD", "", 0 );

         //:nFlags = zMULTIPLE + zNOI_OKAY
         nFlags = zMULTIPLE + zNOI_OKAY;
         //:nRC = ActivateOI_FromFile( vTemp, szTemp, vDialog, szFileName,
         //:                        nFlags )
         nRC = ActivateOI_FromFile( &vTemp, szTemp, vDialog, szFileName, nFlags );

         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 31, TZDBHTSO, "View", "Name", "", 0 );
         //:NAME VIEW vTemp szTemp
         SetNameForView( vTemp, szTemp, 0, zLEVEL_TASK );
         //:SfDropSubtask( vDialog, 0  )
         SfDropSubtask( vDialog, 0 );
      }

      RESULT = SetCursorNextEntity( TZDBHTSO, "View", "" );
   }

   //:END

   //:DropView( TZDBHTSO )
   DropView( TZDBHTSO );
   //:TZDBHTSO = 0
   TZDBHTSO = 0;

   //:/* Set DB UserID and password */
   //:EditID_OK( vSubtask )
   EditID_OK( vSubtask );

   //:/*
   //:   Check to see if any of the views listed in the View list are active.
   //:   If they are, then retrieve the LOD name for the view.
   //:*/
   //:TZDBHTSD_RefreshViewList( vSubtask )
   TZDBHTSD_RefreshViewList( vSubtask );

   //:zSetDBH_TraceLevel( vSubtask )
   zSetDBH_TraceLevel( vSubtask );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:Include_LinkSubobject( VIEW vSubtask )

//:   VIEW TZDBHTSO2 REGISTERED AS TZDBHTSO2
zOPER_EXPORT zSHORT OPERATION
Include_LinkSubobject( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO2 = 0;
   zSHORT    RESULT;
   //:VIEW TZDBHTSO  REGISTERED AS TZDBHTSO
   zVIEW     TZDBHTSO = 0;
   //:VIEW vTarget
   zVIEW     vTarget = 0;
   //:VIEW vSource
   zVIEW     vSource = 0;
   zCHAR     szTempString_0[ 255 ];
   zCHAR     szTempString_1[ 255 ];

   RESULT = GetViewByName( &TZDBHTSO2, "TZDBHTSO2", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.View.CurrentLOD = "?"
   if ( CompareAttributeToString( TZDBHTSO, "View", "CurrentLOD", "?" ) == 0 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:IF TZDBHTSO2.View.CurrentLOD = "?"
   if ( CompareAttributeToString( TZDBHTSO2, "View", "CurrentLOD", "?" ) == 0 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:GET VIEW vTarget NAMED "_Include1"
   RESULT = GetViewByName( &vTarget, "_Include1", vSubtask, zLEVEL_TASK );
   //:GET VIEW vSource NAMED "_Include2"
   RESULT = GetViewByName( &vSource, "_Include2", vSubtask, zLEVEL_TASK );

   //:RelinkInstanceToInstance( vTarget, TZDBHTSO.Entity.Name,
   //:                          vSource, TZDBHTSO2.Entity.Name )
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZDBHTSO, "Entity", "Name" );
   GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TZDBHTSO2, "Entity", "Name" );
   RelinkInstanceToInstance( vTarget, szTempString_0, vSource, szTempString_1 );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditID_Prebuild( VIEW vSubtask )

//:   VIEW TZDBHTSO  REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditID_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:STRING( 30 ) szUserID
   zCHAR     szUserID[ 31 ] = { 0 };
   //:STRING( 30 ) szPassword
   zCHAR     szPassword[ 31 ] = { 0 };

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:SysGetDB_UserID( vSubtask, szUserID, szPassword )
   SysGetDB_UserID( vSubtask, szUserID, sizeof( szUserID ), szPassword, sizeof( szPassword ) );
   //:TZDBHTSO.Root.DB_UserID   = szUserID
   SetAttributeFromString( TZDBHTSO, "Root", "DB_UserID", szUserID );
   //:TZDBHTSO.Root.DB_Password = szPassword
   SetAttributeFromString( TZDBHTSO, "Root", "DB_Password", szPassword );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditID_Clear( VIEW vSubtask )
//:   VIEW TZDBHTSO  REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditID_Clear( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:TZDBHTSO.Root.DB_UserID   = ""
   SetAttributeFromString( TZDBHTSO, "Root", "DB_UserID", "" );
   //:TZDBHTSO.Root.DB_Password = ""
   SetAttributeFromString( TZDBHTSO, "Root", "DB_Password", "" );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditID_OK( VIEW vSubtask )

//:   VIEW TZDBHTSO  REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditID_OK( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:STRING( 30 ) szUserID
   zCHAR     szUserID[ 31 ] = { 0 };
   //:STRING( 30 ) szPassword
   zCHAR     szPassword[ 31 ] = { 0 };

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:szUserID   = TZDBHTSO.Root.DB_UserID
   GetVariableFromAttribute( szUserID, 0, 'S', 31, TZDBHTSO, "Root", "DB_UserID", "", 0 );
   //:szPassword = TZDBHTSO.Root.DB_Password
   GetVariableFromAttribute( szPassword, 0, 'S', 31, TZDBHTSO, "Root", "DB_Password", "", 0 );
   //:SysSetDB_UserID( vSubtask, szUserID, szPassword )
   SysSetDB_UserID( vSubtask, szUserID, szPassword );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:Include_IncludeSubobject( VIEW vSubtask )
//:   VIEW TZDBHTSO2 REGISTERED AS TZDBHTSO2
zOPER_EXPORT zSHORT OPERATION
Include_IncludeSubobject( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO2 = 0;
   zSHORT    RESULT;
   //:VIEW TZDBHTSO  REGISTERED AS TZDBHTSO
   zVIEW     TZDBHTSO = 0;
   //:VIEW vTarget
   zVIEW     vTarget = 0;
   //:VIEW vSource
   zVIEW     vSource = 0;
   zCHAR     szTempString_0[ 255 ];
   zCHAR     szTempString_1[ 255 ];

   RESULT = GetViewByName( &TZDBHTSO2, "TZDBHTSO2", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.View.CurrentLOD = "?"
   if ( CompareAttributeToString( TZDBHTSO, "View", "CurrentLOD", "?" ) == 0 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:IF TZDBHTSO2.View.CurrentLOD = "?"
   if ( CompareAttributeToString( TZDBHTSO2, "View", "CurrentLOD", "?" ) == 0 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:GET VIEW vTarget NAMED "_Include1"
   RESULT = GetViewByName( &vTarget, "_Include1", vSubtask, zLEVEL_TASK );
   //:GET VIEW vSource NAMED "_Include2"
   RESULT = GetViewByName( &vSource, "_Include2", vSubtask, zLEVEL_TASK );

   //:IncludeSubobjectFromSubobject( vTarget, TZDBHTSO.Entity.Name,
   //:                               vSource, TZDBHTSO2.Entity.Name, zPOS_AFTER )
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZDBHTSO, "Entity", "Name" );
   GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TZDBHTSO2, "Entity", "Name" );
   IncludeSubobjectFromSubobject( vTarget, szTempString_0, vSource, szTempString_1, zPOS_AFTER );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:Include_Prebuild( VIEW vSubtask )
//:   VIEW TZDBHTSO2 REGISTERED AS TZDBHTSO2
zOPER_EXPORT zSHORT OPERATION
Include_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO2 = 0;
   zSHORT    RESULT;
   //:VIEW TZDBHTSO  REGISTERED AS TZDBHTSO
   zVIEW     TZDBHTSO = 0;

   RESULT = GetViewByName( &TZDBHTSO2, "TZDBHTSO2", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:FOR EACH TZDBHTSO.Entity
   RESULT = SetCursorFirstEntity( TZDBHTSO, "Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:DELETE ENTITY TZDBHTSO.Entity NONE
      RESULT = DeleteEntity( TZDBHTSO, "Entity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZDBHTSO, "Entity", "" );
   }

   //:END

   //:IF TZDBHTSO2 != 0
   if ( TZDBHTSO2 != 0 )
   {
      //:DropView( TZDBHTSO2 )
      DropView( TZDBHTSO2 );
   }

   //:END

   //:ActivateOI_FromOI_ForTask( TZDBHTSO2, TZDBHTSO, 0, zMULTIPLE )
   ActivateOI_FromOI_ForTask( &TZDBHTSO2, TZDBHTSO, 0, zMULTIPLE );
   //:NAME VIEW TZDBHTSO2 "TZDBHTSO2"
   SetNameForView( TZDBHTSO2, "TZDBHTSO2", 0, zLEVEL_TASK );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:TZDBHTSD_RefreshViewList( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_RefreshViewList( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:VIEW vTemp
   zVIEW     vTemp = 0;
   //:STRING ( 100 ) szTemp
   zCHAR     szTemp[ 101 ] = { 0 };
   zSHORT    lTempInteger_0;
   zCHAR     szTempString_0[ 255 ];

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:// If TZDBHTSO wasn't found, then it hasn't been created yet so just exit.
   //:IF TZDBHTSO = 0
   if ( TZDBHTSO == 0 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:/*
   //:   Check to see if any of the views listed in the View list are active.
   //:   If they are, then retrieve the LOD name for the view.
   //:*/
   //:CreateViewFromViewForTask( TZDBHTSO, TZDBHTSO, vSubtask )
   CreateViewFromViewForTask( &TZDBHTSO, TZDBHTSO, vSubtask );

   //:FOR EACH TZDBHTSO.View
   RESULT = SetCursorFirstEntity( TZDBHTSO, "View", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:szTemp = TZDBHTSO.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
      //:IF RESULT > 0
      if ( RESULT > 0 )
      {
         //:MiGetObjectNameForView( szTemp, vTemp )
         MiGetObjectNameForView( szTemp, vTemp );
         //:TZDBHTSO.View.CurrentLOD = szTemp
         SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", szTemp );

         //:IF ObjectInstanceUpdated( vTemp ) = 1
         lTempInteger_0 = ObjectInstanceUpdated( vTemp );
         if ( lTempInteger_0 == 1 )
         {
            //:TZDBHTSO.View.Updated = "Y"
            SetAttributeFromString( TZDBHTSO, "View", "Updated", "Y" );
            //:ELSE
         }
         else
         {
            //:TZDBHTSO.View.Updated = "N"
            SetAttributeFromString( TZDBHTSO, "View", "Updated", "N" );
         }

         //:END

         //:// If the object name is KZDBHQUA, try adding it to the list of
         //:// qualification views.
         //:// IF zstrcmpi( szTemp, "KZDBHQUA" ) = 0
         //:IF szTemp = "KZDBHQUA"
         if ( ZeidonStringCompare( szTemp, 1, 0, "KZDBHQUA", 1, 0, 101 ) == 0 )
         {
            //:lAddViewNameToQualList( TZDBHTSO.View.Name )
            GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZDBHTSO, "View", "Name" );
            o_lAddViewNameToQualList( szTempString_0 );
         }

         //:END
         //:ELSE
      }
      else
      {
         //:TZDBHTSO.View.CurrentLOD = "?"
         SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", "?" );
         //:TZDBHTSO.View.Updated = ""
         SetAttributeFromString( TZDBHTSO, "View", "Updated", "" );
      }

      RESULT = SetCursorNextEntity( TZDBHTSO, "View", "" );
      //:END
   }

   //:END

   //:DropView( TZDBHTSO )
   DropView( TZDBHTSO );
   return( 0 );
// END
}


//:LOCAL OPERATION
//:lAddViewNameToViewList( STRING ( 32 ) szViewName )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
static zSHORT
o_lAddViewNameToViewList( zPCHAR    szViewName )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", TZDBHTSO, zLEVEL_TASK );

   //:CreateViewFromViewForTask( TZDBHTSO, TZDBHTSO, 0 )
   CreateViewFromViewForTask( &TZDBHTSO, TZDBHTSO, 0 );

   //:SET CURSOR FIRST TZDBHTSO.View
   //:   WHERE TZDBHTSO.View.Name = szViewName
   RESULT = SetCursorFirstEntityByString( TZDBHTSO, "View", "Name", szViewName, "" );

   //:IF RESULT != zCURSOR_SET
   if ( RESULT != zCURSOR_SET )
   {
      //:CREATE ENTITY TZDBHTSO.View LAST
      RESULT = CreateEntity( TZDBHTSO, "View", zPOS_LAST );
      //:TZDBHTSO.View.Name = szViewName
      SetAttributeFromString( TZDBHTSO, "View", "Name", szViewName );
   }

   //:END

   //:DropView( TZDBHTSO )
   DropView( TZDBHTSO );
   return( 0 );
// END
}


//:LOCAL OPERATION
//:lAddViewNameToQualList( STRING ( 32 ) szViewName )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
static zSHORT
o_lAddViewNameToQualList( zPCHAR    szViewName )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", TZDBHTSO, zLEVEL_TASK );

   //:CreateViewFromViewForTask( TZDBHTSO, TZDBHTSO, 0 )
   CreateViewFromViewForTask( &TZDBHTSO, TZDBHTSO, 0 );

   //:SET CURSOR FIRST TZDBHTSO.Qual
   //:   WHERE TZDBHTSO.Qual.Name = szViewName
   RESULT = SetCursorFirstEntityByString( TZDBHTSO, "Qual", "Name", szViewName, "" );

   //:IF RESULT != zCURSOR_SET
   if ( RESULT != zCURSOR_SET )
   {
      //:CREATE ENTITY TZDBHTSO.Qual LAST
      RESULT = CreateEntity( TZDBHTSO, "Qual", zPOS_LAST );
      //:TZDBHTSO.Qual.Name = szViewName
      SetAttributeFromString( TZDBHTSO, "Qual", "Name", szViewName );
   }

   //:END

   //:DropView( TZDBHTSO )
   DropView( TZDBHTSO );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:TZDBHTSD_SaveConfiguration( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_SaveConfiguration( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:VIEW TZDBHTSOx
   zVIEW     TZDBHTSOx = 0;
   //:VIEW vTemp
   zVIEW     vTemp = 0;
   //:STRING ( 300 ) szTempDir
   zCHAR     szTempDir[ 301 ] = { 0 };
   //:STRING ( 300 ) szFileName
   zCHAR     szFileName[ 301 ] = { 0 };
   //:STRING (  30 ) szTemp
   zCHAR     szTemp[ 31 ] = { 0 };
   //:Integer nPos
   zLONG     nPos = 0;
   //:INTEGER nFlags
   zLONG     nFlags = 0;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:/* Delete the operation results list. */
   //:FOR EACH TZDBHTSO.ExecutionResult
   RESULT = SetCursorFirstEntity( TZDBHTSO, "ExecutionResult", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:DELETE ENTITY TZDBHTSO.ExecutionResult NONE
      RESULT = DeleteEntity( TZDBHTSO, "ExecutionResult", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZDBHTSO, "ExecutionResult", "" );
   }

   //:END

   //:SysGetEnvVar( szTempDir, "TEMP", 300 )
   SysGetEnvVar( szTempDir, "TEMP", 300 );

   //:CreateViewFromViewForTask( TZDBHTSOx, TZDBHTSO, vSubtask  )
   CreateViewFromViewForTask( &TZDBHTSOx, TZDBHTSO, vSubtask );

   //:// Loop through each of the views and save some.
   //:FOR EACH TZDBHTSOx.View WHERE TZDBHTSOx.View.SaveOI = "*"
   RESULT = SetCursorFirstEntityByString( TZDBHTSOx, "View", "SaveOI", "*", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:szTemp = TZDBHTSOx.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 31, TZDBHTSOx, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );

      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:nPos = GetRelativeEntityNumber( TZDBHTSOx, "View", "", 0 )
         nPos = GetRelativeEntityNumber( TZDBHTSOx, "View", "", 0 );
         //:ConvertIntegerToString( nPos, szTemp, 10 )
         ConvertIntegerToString( nPos, szTemp, 10 );
         //:szFileName = szTempDir + "\TZDBH" + szTemp + ".POR"
         ZeidonStringCopy( szFileName, 1, 0, szTempDir, 1, 0, 301 );
         ZeidonStringConcat( szFileName, 1, 0, "\\TZDBH", 1, 0, 301 );
         ZeidonStringConcat( szFileName, 1, 0, szTemp, 1, 0, 301 );
         ZeidonStringConcat( szFileName, 1, 0, ".POR", 1, 0, 301 );

         //:nFlags = zINCREMENTAL + 262144
         nFlags = zINCREMENTAL + 262144;
         //:CommitOI_ToFile( vTemp, szFileName, nFlags ) // zSAVE_CURSORS
         CommitOI_ToFile( vTemp, szFileName, nFlags );

         //:TZDBHTSOx.View.OI_WasSaved = "Y"
         SetAttributeFromString( TZDBHTSOx, "View", "OI_WasSaved", "Y" );
         //:ELSE
      }
      else
      {
         //:TZDBHTSOx.View.OI_WasSaved = "N"
         SetAttributeFromString( TZDBHTSOx, "View", "OI_WasSaved", "N" );
      }

      RESULT = SetCursorNextEntityByString( TZDBHTSOx, "View", "SaveOI", "*", "" );
      //:END
   }

   //:END

   //:DropView( TZDBHTSOx )
   DropView( TZDBHTSOx );

   //:szFileName = szTempDir + "\TZDBHTSD.POR"
   ZeidonStringCopy( szFileName, 1, 0, szTempDir, 1, 0, 301 );
   ZeidonStringConcat( szFileName, 1, 0, "\\TZDBHTSD.POR", 1, 0, 301 );
   //:nFlags = zASCII + 262144
   nFlags = zASCII + 262144;
   //:CommitOI_ToFile( TZDBHTSO, szFileName, nFlags ) // zSAVE_CURSORS
   CommitOI_ToFile( TZDBHTSO, szFileName, nFlags );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditViewName_Prebuild( VIEW vSubtask )

//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditViewName_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.View.Name = ""
   if ( CompareAttributeToString( TZDBHTSO, "View", "Name", "" ) == 0 )
   {
      //:TZDBHTSO.View.Name = "New"
      SetAttributeFromString( TZDBHTSO, "View", "Name", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditLOD_Prebuild( VIEW vSubtask )

//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditLOD_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.LOD.Name = ""
   if ( CompareAttributeToString( TZDBHTSO, "LOD", "Name", "" ) == 0 )
   {
      //:TZDBHTSO.LOD.Name = "New"
      SetAttributeFromString( TZDBHTSO, "LOD", "Name", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditQual_Prebuild( VIEW vSubtask )

//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditQual_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.Qual.Name = ""
   if ( CompareAttributeToString( TZDBHTSO, "Qual", "Name", "" ) == 0 )
   {
      //:TZDBHTSO.Qual.Name = "New"
      SetAttributeFromString( TZDBHTSO, "Qual", "Name", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditFile_Prebuild( VIEW vSubtask )

//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditFile_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.File.Name = ""
   if ( CompareAttributeToString( TZDBHTSO, "File", "Name", "" ) == 0 )
   {
      //:TZDBHTSO.File.Name = "New"
      SetAttributeFromString( TZDBHTSO, "File", "Name", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:zSetDBH_TraceLevel( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
zSetDBH_TraceLevel( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:Integer TraceLevel
   zLONG     TraceLevel = 0;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:TraceLevel = TZDBHTSO.Root.TraceLevel
   GetIntegerFromAttribute( &TraceLevel, TZDBHTSO, "Root", "TraceLevel" );
   //:SetTaskDBHandlerTraceLevel( vSubtask, TraceLevel )
   SetTaskDBHandlerTraceLevel( vSubtask, TraceLevel );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:zExecuteOperation( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
zExecuteOperation( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:VIEW KZDBHQUA BASED ON LOD KZDBHQUA
   zVIEW     KZDBHQUA = 0;
   //:VIEW vTemp
   zVIEW     vTemp = 0;
   //:VIEW vDialog
   zVIEW     vDialog = 0;
   //:SHORT   nErrIdx
   zSHORT    nErrIdx = 0;
   //:Integer nRC
   zLONG     nRC = 0;
   //:Integer nFlags
   zLONG     nFlags = 0;
   //:Integer lViewArray
   zLONG     lViewArray = 0;
   //:Integer lStartTick
   zLONG     lStartTick = 0;
   //:Integer lEndTick
   zLONG     lEndTick = 0;
   //:DECIMAL dTotalTime
   ZDecimal  dTotalTime = 0.0;

   //:STRING ( 100 ) szTemp
   zCHAR     szTemp[ 101 ] = { 0 };
   //:STRING (  32 ) szSubtaskName
   zCHAR     szSubtaskName[ 33 ] = { 0 };
   zCHAR     szTempString_0[ 255 ];
   zCHAR     szTempString_1[ 255 ];
   zCHAR     szTempString_2[ 255 ];
   zCHAR     szTempString_3[ 255 ];
   zCHAR     szTempString_4[ 255 ];
   zCHAR     szTempString_5[ 255 ];
   zCHAR     szTempString_6[ 255 ];
   zCHAR     szTempString_7[ 255 ];
   zCHAR     szTempString_8[ 255 ];

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:CREATE ENTITY TZDBHTSO.ExecutionResult LAST
   RESULT = CreateEntity( TZDBHTSO, "ExecutionResult", zPOS_LAST );
   //:TZDBHTSO.ExecutionResult.Operation = TZDBHTSO.Root.CurrentOperation
   SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "Operation", TZDBHTSO, "Root", "CurrentOperation" );

   //:IF TZDBHTSO.Application.Name != "LOCAL"
   if ( CompareAttributeToString( TZDBHTSO, "Application", "Name", "LOCAL" ) != 0 )
   {
      //:szSubtaskName = TZDBHTSO.Application.Name
      GetVariableFromAttribute( szSubtaskName, 0, 'S', 33, TZDBHTSO, "Application", "Name", "", 0 );
      //:SfCreateSubtask( vDialog, 0, szSubtaskName )
      SfCreateSubtask( &vDialog, 0, szSubtaskName );
      //:ELSE
   }
   else
   {
      //:vDialog = vSubtask
      vDialog = vSubtask;
      //:szSubtaskName = "Zeidon_Tools"
      ZeidonStringCopy( szSubtaskName, 1, 0, "Zeidon_Tools", 1, 0, 33 );
   }

   //:END

   //:lStartTick = zGetTickCount( 0 )
   lStartTick = zGetTickCount( 0 );

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromFile"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromFile" ) == 0 )
   {
      //:// If AutoDrop is specified, check to see if the current view name
      //:// already exists and if it does then drop it.
      //:IF TZDBHTSO.Root.AutoDrop = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "AutoDrop", "Y" ) == 0 )
      {
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:GET VIEW vTemp NAMED szTemp
         RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
         //:// GET VIEW vTemp NAMED TZDBHTSO.View.Name
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:DropView( vTemp )
            DropView( vTemp );
            //:TZDBHTSO.View.CurrentLOD = "?"
            SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", "?" );
         }

         //:END
      }

      //:END

      //:nFlags = 0
      nFlags = 0;

      //:IF TZDBHTSO.Root.Single = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "Single", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zSINGLE
         nFlags = nFlags + zSINGLE;
         //:ELSE
      }
      else
      {
         //:nFlags = nFlags + zMULTIPLE
         nFlags = nFlags + zMULTIPLE;
      }

      //:END

      //:IF TZDBHTSO.Root.RootOnly = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "RootOnly", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zACTIVATE_ROOTONLY
         nFlags = nFlags + zACTIVATE_ROOTONLY;
      }

      //:END

      //:nRC = ActivateOI_FromFile( vTemp,
      //:                           TZDBHTSO.LOD.Name,   /* Object Def Name */
      //:                           vDialog,            /* Subtask qual */
      //:                           TZDBHTSO.File.Name,  /* File Name */
      //:                           nFlags )             /* Control Value */
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZDBHTSO, "LOD", "Name" );
      GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TZDBHTSO, "File", "Name" );
      nRC = ActivateOI_FromFile( &vTemp, szTempString_0, vDialog, szTempString_1, nFlags );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
      //:TZDBHTSO.ExecutionResult.LOD      = TZDBHTSO.LOD.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "LOD", TZDBHTSO, "LOD", "Name" );
      //:TZDBHTSO.ExecutionResult.QualView = TZDBHTSO.Qual.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "QualView", TZDBHTSO, "Qual", "Name" );
      //:TZDBHTSO.ExecutionResult.FileName = TZDBHTSO.File.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "FileName", TZDBHTSO, "File", "Name" );

      //:IF nRC >= 0
      if ( nRC >= 0 )
      {
         //:TZDBHTSO.View.CurrentLOD = TZDBHTSO.LOD.Name
         SetAttributeFromAttribute( TZDBHTSO, "View", "CurrentLOD", TZDBHTSO, "LOD", "Name" );
         //:TZDBHTSO.View.AppName = szSubtaskName
         SetAttributeFromString( TZDBHTSO, "View", "AppName", szSubtaskName );

         //:// NAME VIEW vTemp TZDBHTSO.View.Name
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:NAME VIEW vTemp szTemp
         SetNameForView( vTemp, szTemp, 0, zLEVEL_TASK );
      }

      //:END
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromDB"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromDB" ) == 0 )
   {
      //:// If AutoDrop is specified, check to see if the current view name
      //:// already exists and if it does then drop it.
      //:szTemp = TZDBHTSO.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
      //:IF TZDBHTSO.Root.AutoDrop = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "AutoDrop", "Y" ) == 0 )
      {
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:DropView( vTemp )
            DropView( vTemp );
            //:TZDBHTSO.View.CurrentLOD = "?"
            SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", "?" );
         }

         //:END
      }

      //:END

      //:szTemp = TZDBHTSO.Qual.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "Qual", "Name", "", 0 );
      //:IF szTemp = "NONE"
      if ( ZeidonStringCompare( szTemp, 1, 0, "NONE", 1, 0, 101 ) == 0 )
      {
         //:KZDBHQUA = 0
         KZDBHQUA = 0;
         //:ELSE
      }
      else
      {
         //:GET VIEW KZDBHQUA NAMED szTemp
         RESULT = GetViewByName( &KZDBHQUA, szTemp, vSubtask, zLEVEL_TASK );
      }

      //:END

      //:nFlags = 0
      nFlags = 0;

      //:IF TZDBHTSO.Root.Single = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "Single", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zSINGLE
         nFlags = nFlags + zSINGLE;
         //:ELSE
      }
      else
      {
         //:nFlags = nFlags + zMULTIPLE
         nFlags = nFlags + zMULTIPLE;
      }

      //:END

      //:IF TZDBHTSO.Root.SingleForUpdate= "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "SingleForUpdate", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zSINGLE_FOR_UPDATE
         nFlags = nFlags + zSINGLE_FOR_UPDATE;
      }

      //:END

      //:IF TZDBHTSO.Root.RootOnly = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "RootOnly", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zACTIVATE_ROOTONLY
         nFlags = nFlags + zACTIVATE_ROOTONLY;
      }

      //:END

      //:IF TZDBHTSO.Root.ActivateContinue = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "ActivateContinue", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 8388608 // zACTIVATE_CONTINUE
         nFlags = nFlags + 8388608;
      }

      //:END

      //:nRC = ActivateObjectInstance( vTemp,
      //:                              TZDBHTSO.LOD.Name,   /* Object Def Name */
      //:                              vDialog,
      //:                              KZDBHQUA,
      //:                              nFlags )            /* Control Value */
      GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), TZDBHTSO, "LOD", "Name" );
      nRC = ActivateObjectInstance( &vTemp, szTempString_2, vDialog, KZDBHQUA, nFlags );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
      //:TZDBHTSO.ExecutionResult.LOD      = TZDBHTSO.LOD.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "LOD", TZDBHTSO, "LOD", "Name" );
      //:TZDBHTSO.ExecutionResult.QualView = TZDBHTSO.Qual.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "QualView", TZDBHTSO, "Qual", "Name" );

      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:TZDBHTSO.View.CurrentLOD = TZDBHTSO.LOD.Name
         SetAttributeFromAttribute( TZDBHTSO, "View", "CurrentLOD", TZDBHTSO, "LOD", "Name" );
         //:TZDBHTSO.View.AppName    = szSubtaskName
         SetAttributeFromString( TZDBHTSO, "View", "AppName", szSubtaskName );

         //:// NAME VIEW vTemp TZDBHTSO.View.Name
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:NAME VIEW vTemp szTemp
         SetNameForView( vTemp, szTemp, 0, zLEVEL_TASK );
      }

      //:END
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateEmpty"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateEmpty" ) == 0 )
   {
      //:// If AutoDrop is specified, check to see if the current view name
      //:// already exists and if it does then drop it.
      //:IF TZDBHTSO.Root.AutoDrop = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "AutoDrop", "Y" ) == 0 )
      {
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:GET VIEW vTemp NAMED szTemp
         RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:DropView( vTemp )
            DropView( vTemp );
            //:TZDBHTSO.View.CurrentLOD = "?"
            SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", "?" );
         }

         //:END
      }

      //:END

      //:nRC = ActivateEmptyObjectInstance( vTemp,
      //:                                   TZDBHTSO.LOD.Name,/* Object Def Name */
      //:                                   vDialog,
      //:                                   zMULTIPLE )
      GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), TZDBHTSO, "LOD", "Name" );
      nRC = ActivateEmptyObjectInstance( &vTemp, szTempString_3, vDialog, zMULTIPLE );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
      //:TZDBHTSO.ExecutionResult.LOD      = TZDBHTSO.LOD.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "LOD", TZDBHTSO, "LOD", "Name" );

      //:IF nRC >= 0
      if ( nRC >= 0 )
      {
         //:TZDBHTSO.View.CurrentLOD = TZDBHTSO.LOD.Name
         SetAttributeFromAttribute( TZDBHTSO, "View", "CurrentLOD", TZDBHTSO, "LOD", "Name" );
         //:TZDBHTSO.View.AppName    = szSubtaskName
         SetAttributeFromString( TZDBHTSO, "View", "AppName", szSubtaskName );

         //:// NAME VIEW vTemp TZDBHTSO.View.Name
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:NAME VIEW vTemp szTemp
         SetNameForView( vTemp, szTemp, 0, zLEVEL_TASK );
      }

      //:END
   }


   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromXML"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromXML" ) == 0 )
   {
      //:// If AutoDrop is specified, check to see if the current view name
      //:// already exists and if it does then drop it.
      //:IF TZDBHTSO.Root.AutoDrop = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "AutoDrop", "Y" ) == 0 )
      {
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:GET VIEW vTemp NAMED szTemp
         RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
         //:// GET VIEW vTemp NAMED TZDBHTSO.View.Name
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:DropView( vTemp )
            DropView( vTemp );
            //:TZDBHTSO.View.CurrentLOD = "?"
            SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", "?" );
         }

         //:END
      }

      //:END

      //:nFlags = 0
      nFlags = 0;

      //:nRC = DBH_ActivateOI_FromXML( vTemp,
      //:                              TZDBHTSO.LOD.Name,   /* Object Def Name */
      //:                              vDialog,            /* Subtask qual */
      //:                              TZDBHTSO.File.Name,  /* File Name */
      //:                              nFlags )             /* Control Value */
      GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), TZDBHTSO, "LOD", "Name" );
      GetStringFromAttribute( szTempString_5, sizeof( szTempString_5 ), TZDBHTSO, "File", "Name" );
      nRC = DBH_ActivateOI_FromXML( &vTemp, szTempString_4, vDialog, szTempString_5, nFlags );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
      //:TZDBHTSO.ExecutionResult.LOD      = TZDBHTSO.LOD.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "LOD", TZDBHTSO, "LOD", "Name" );
      //:TZDBHTSO.ExecutionResult.QualView = TZDBHTSO.Qual.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "QualView", TZDBHTSO, "Qual", "Name" );
      //:TZDBHTSO.ExecutionResult.FileName = TZDBHTSO.File.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "FileName", TZDBHTSO, "File", "Name" );

      //:IF nRC >= 0
      if ( nRC >= 0 )
      {
         //:TZDBHTSO.View.CurrentLOD = TZDBHTSO.LOD.Name
         SetAttributeFromAttribute( TZDBHTSO, "View", "CurrentLOD", TZDBHTSO, "LOD", "Name" );
         //:TZDBHTSO.View.AppName = szSubtaskName
         SetAttributeFromString( TZDBHTSO, "View", "AppName", szSubtaskName );

         //:// NAME VIEW vTemp TZDBHTSO.View.Name
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:NAME VIEW vTemp szTemp
         SetNameForView( vTemp, szTemp, 0, zLEVEL_TASK );
      }

      //:END
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DisplayObjDef"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DisplayObjDef" ) == 0 )
   {
      //:nRC = ActivateEmptyObjectInstance( vTemp,
      //:                                   TZDBHTSO.LOD.Name,/* Object Def Name */
      //:                                   vDialog,
      //:                                   zSINGLE )
      GetStringFromAttribute( szTempString_6, sizeof( szTempString_6 ), TZDBHTSO, "LOD", "Name" );
      nRC = ActivateEmptyObjectInstance( &vTemp, szTempString_6, vDialog, zSINGLE );

      //:nRC = DisplayObjectDef( vTemp, 0 )
      nRC = DisplayObjectDef( vTemp, 0 );

      //:          DropView( vTemp )
      DropView( vTemp );
      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.LOD        = TZDBHTSO.LOD.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "LOD", TZDBHTSO, "LOD", "Name" );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DropView"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DropView" ) == 0 )
   {
      //:szTemp = TZDBHTSO.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );

      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:DropView( vTemp )
         DropView( vTemp );
         //:nRC = 0
         nRC = 0;
         //:ELSE
      }
      else
      {
         //:nRC = -16
         nRC = -16;
      }

      //:END

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );

      //:TZDBHTSO.View.CurrentLOD = ""
      SetAttributeFromString( TZDBHTSO, "View", "CurrentLOD", "" );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "CommitToFile"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "CommitToFile" ) == 0 )
   {
      //:szTemp = TZDBHTSO.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );

      //:nFlags = zASCII
      nFlags = zASCII;

      //:IF TZDBHTSO.Root.CommitIncremental = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitIncremental", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zINCREMENTAL
         nFlags = nFlags + zINCREMENTAL;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitCompressed = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitCompressed", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 131072         // zCOMPRESSED
         nFlags = nFlags + 131072;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitCursors = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitCursors", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 262144         // zSAVE_CURSORS
         nFlags = nFlags + 262144;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitETags = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitETags", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zENTITY_TAGS
         nFlags = nFlags + zENTITY_TAGS;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitEKeys = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitEKeys", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 2097152       // zENTITY_KEYS
         nFlags = nFlags + 2097152;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitEncodeBLOBs = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitEncodeBLOBs", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 4194304       // zENCODE_BLOBS           0x00400000L
         nFlags = nFlags + 4194304;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitNoNullTerminator = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitNoNullTerminator", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 8388608       // zNO_NULL_STRING_TERM    0x00800000L
         nFlags = nFlags + 8388608;
      }

      //:END

      //:nRC = CommitOI_ToFile( vTemp,
      //:                       TZDBHTSO.File.Name,  /* File Name */
      //:                       nFlags )
      GetStringFromAttribute( szTempString_7, sizeof( szTempString_7 ), TZDBHTSO, "File", "Name" );
      nRC = CommitOI_ToFile( vTemp, szTempString_7, nFlags );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
      //:TZDBHTSO.ExecutionResult.FileName = TZDBHTSO.File.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "FileName", TZDBHTSO, "File", "Name" );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "WriteAsXML"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "WriteAsXML" ) == 0 )
   {
      //:szTemp = TZDBHTSO.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );

      //:nFlags = 0
      nFlags = 0;

      //:IF TZDBHTSO.Root.XML_WriteZeidonInfo = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "XML_WriteZeidonInfo", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 1  // zXML_ZEIDONINFO
         nFlags = nFlags + 1;
      }

      //:END

      //:IF TZDBHTSO.Root.XML_WriteIncrFlags = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "XML_WriteIncrFlags", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 2  // zXML_INCREFLAGS
         nFlags = nFlags + 2;
      }

      //:END

      //:IF TZDBHTSO.Root.XML_UpdatedOnly = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "XML_UpdatedOnly", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 4  // zXML_UPDATEDONLY
         nFlags = nFlags + 4;
      }

      //:END

      //:IF TZDBHTSO.Root.XML_EntityKeys = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "XML_EntityKeys", "Y" ) == 0 )
      {
         //:nFlags = nFlags + 8  // zXML_ENTITYKEYS
         nFlags = nFlags + 8;
      }

      //:END

      //:nRC = DBH_WriteOI_ToXML( vTemp,
      //:                         TZDBHTSO.File.Name,  /* File Name */
      //:                         nFlags )
      GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), TZDBHTSO, "File", "Name" );
      nRC = DBH_WriteOI_ToXML( vTemp, szTempString_8, nFlags );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
      //:TZDBHTSO.ExecutionResult.FileName = TZDBHTSO.File.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "FileName", TZDBHTSO, "File", "Name" );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "CommitToDB"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "CommitToDB" ) == 0 )
   {
      //:CreateViewCluster( vSubtask, lViewArray )
      CreateViewCluster( vSubtask, &lViewArray );
      //:nFlags = 0
      nFlags = 0;

      //:IF TZDBHTSO.Root.CommitKeepLocks = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitKeepLocks", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zCOMMIT_KEEPLOCKS
         nFlags = nFlags + zCOMMIT_KEEPLOCKS;
      }

      //:END

      //:IF TZDBHTSO.Root.CommitLocksOnly = "Y"
      if ( CompareAttributeToString( TZDBHTSO, "Root", "CommitLocksOnly", "Y" ) == 0 )
      {
         //:nFlags = nFlags + zCOMMIT_DROPLOCKSONLY
         nFlags = nFlags + zCOMMIT_DROPLOCKSONLY;
      }

      //:END

      //:// Add all the selected views to the array.
      //:nRC = SetCursorFirstSelectedEntity( TZDBHTSO, "View", 0 )
      nRC = SetCursorFirstSelectedEntity( TZDBHTSO, "View", 0 );
      //:LOOP WHILE ( nRC = zCURSOR_SET )
      while ( nRC == zCURSOR_SET )
      {
         //:szTemp = TZDBHTSO.View.Name
         GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
         //:GET VIEW vTemp NAMED szTemp
         RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
         //:nRC = AddViewToViewCluster( lViewArray, vTemp )
         nRC = AddViewToViewCluster( lViewArray, vTemp );

         //:nRC = SetCursorNextSelectedEntity( TZDBHTSO, "View", 0 )
         nRC = SetCursorNextSelectedEntity( TZDBHTSO, "View", 0 );
      }

      //:END

      //:nRC = CommitMultipleOIs( lViewArray, 0, nErrIdx, nFlags )
      nRC = CommitMultipleOIs( (LPVIEWCLUSTER) lViewArray, 0, &nErrIdx, nFlags );

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DisplayOI"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DisplayOI" ) == 0 )
   {
      //:szTemp = TZDBHTSO.View.Name
      GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
      //:GET VIEW vTemp NAMED szTemp
      RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );

      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:szTemp = ""
         ZeidonStringCopy( szTemp, 1, 0, "", 1, 0, 101 );
         //:DisplayObjectInstance( vTemp, szTemp, szTemp )
         DisplayObjectInstance( vTemp, szTemp, szTemp );
         //:nRC = 0
         nRC = 0;
         //:ELSE
      }
      else
      {
         //:nRC = -16
         nRC = -16;
      }

      //:END

      //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
      SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
      //:TZDBHTSO.ExecutionResult.ViewName = TZDBHTSO.View.Name
      SetAttributeFromAttribute( TZDBHTSO, "ExecutionResult", "ViewName", TZDBHTSO, "View", "Name" );
   }

   //:END

   //:lEndTick   = zGetTickCount( 0 )
   lEndTick = zGetTickCount( 0 );
   //:dTotalTime = lEndTick - lStartTick
   dTotalTime = lEndTick - lStartTick;
   //:dTotalTime = dTotalTime / 1000
   dTotalTime = dTotalTime / 1000;
   //:SetAttributeFromDecimal( TZDBHTSO, "ExecutionResult", "Time", dTotalTime )
   SetAttributeFromDecimal( TZDBHTSO, "ExecutionResult", "Time", dTotalTime );

   //:TZDBHTSO.ExecutionResult.ReturnCode = nRC
   SetAttributeFromInteger( TZDBHTSO, "ExecutionResult", "ReturnCode", nRC );
   //:SET CURSOR LAST TZDBHTSO.ExecutionResult
   RESULT = SetCursorLastEntity( TZDBHTSO, "ExecutionResult", "" );
   //:RefreshCtrl( vSubtask, "ResultsList" )
   RefreshCtrl( vSubtask, "ResultsList" );

   //:IF TZDBHTSO.Application.Name != "LOCAL"
   if ( CompareAttributeToString( TZDBHTSO, "Application", "Name", "LOCAL" ) != 0 )
   {
      //:SfDropSubtask( vDialog, 0  )
      SfDropSubtask( vDialog, 0 );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:zStartBrowser( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
zStartBrowser( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:VIEW vTemp
   zVIEW     vTemp = 0;
   //:STRING ( 100 ) szTemp
   zCHAR     szTemp[ 101 ] = { 0 };

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:szTemp = TZDBHTSO.View.Name
   GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "View", "Name", "", 0 );
   //:GET VIEW vTemp NAMED szTemp
   RESULT = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   {
      //:StageBrowser( vSubtask, vTemp )
      StageBrowser( vSubtask, vTemp );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditEntitySpec_Prebuild( VIEW vSubtask )
//:   VIEW KZDBHQUA REGISTERED AS KZDBHQUA
zOPER_EXPORT zSHORT OPERATION
EditEntitySpec_Prebuild( zVIEW     vSubtask )
{
   zVIEW     KZDBHQUA = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &KZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );

   //:IF KZDBHQUA.EntitySpec.EntityName = ""
   if ( CompareAttributeToString( KZDBHQUA, "EntitySpec", "EntityName", "" ) == 0 )
   {
      //:KZDBHQUA.EntitySpec.EntityName = "New"
      SetAttributeFromString( KZDBHQUA, "EntitySpec", "EntityName", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditQualAttrib_Prebuild( VIEW vSubtask )
//:   VIEW KZDBHQUA REGISTERED AS KZDBHQUA
zOPER_EXPORT zSHORT OPERATION
EditQualAttrib_Prebuild( zVIEW     vSubtask )
{
   zVIEW     KZDBHQUA = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &KZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );

   //:IF KZDBHQUA.QualAttrib.EntityName = "" AND KZDBHQUA.QualAttrib.Value = ""
   if ( CompareAttributeToString( KZDBHQUA, "QualAttrib", "EntityName", "" ) == 0 && CompareAttributeToString( KZDBHQUA, "QualAttrib", "Value", "" ) == 0 )
   {
      //:KZDBHQUA.QualAttrib.EntityName = "New"
      SetAttributeFromString( KZDBHQUA, "QualAttrib", "EntityName", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:TZDBHTSD_EditQualOI( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_EditQualOI( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   //:VIEW KZDBHQUA BASED ON LOD KZDBHQUA
   zVIEW     KZDBHQUA = 0;
   //:STRING ( 100 ) szTemp
   zCHAR     szTemp[ 101 ] = { 0 };

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.Qual.Name = "NONE" OR TZDBHTSO.Qual.Name = ""
   if ( CompareAttributeToString( TZDBHTSO, "Qual", "Name", "NONE" ) == 0 || CompareAttributeToString( TZDBHTSO, "Qual", "Name", "" ) == 0 )
   {
      //:SetWindowActionBehavior( vSubtask,
      //:                         zWAB_TerminateActionForError, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:szTemp = TZDBHTSO.Qual.Name
   GetVariableFromAttribute( szTemp, 0, 'S', 101, TZDBHTSO, "Qual", "Name", "", 0 );
   //:GET VIEW KZDBHQUA NAMED szTemp
   RESULT = GetViewByName( &KZDBHQUA, szTemp, vSubtask, zLEVEL_TASK );

   //:IF RESULT < 0
   if ( RESULT < 0 )
   {
      //:ActivateEmptyObjectInstance( KZDBHQUA, "KZDBHQUA",
      //:                             vSubtask, zMULTIPLE )
      ActivateEmptyObjectInstance( &KZDBHQUA, "KZDBHQUA", vSubtask, zMULTIPLE );
      //:NAME VIEW KZDBHQUA szTemp
      SetNameForView( KZDBHQUA, szTemp, 0, zLEVEL_TASK );

      //:CREATE ENTITY KZDBHQUA.EntitySpec
      RESULT = CreateEntity( KZDBHQUA, "EntitySpec", zPOS_AFTER );
      //:KZDBHQUA.EntitySpec.EntityName = "New"
      SetAttributeFromString( KZDBHQUA, "EntitySpec", "EntityName", "New" );

      //:lAddViewNameToViewList( szTemp )
      o_lAddViewNameToViewList( szTemp );
   }

   //:END

   //:NAME VIEW KZDBHQUA "KZDBHQUA"
   SetNameForView( KZDBHQUA, "KZDBHQUA", 0, zLEVEL_TASK );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:TZDBHTSD_EnableControls( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_EnableControls( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromFile"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromFile" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromDB"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromDB" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 1 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateEmpty"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateEmpty" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromXML"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromXML" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DisplayObjDef"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DisplayObjDef" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DropView"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DropView" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "CommitToFile"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "CommitToFile" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "WriteAsXML"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "WriteAsXML" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "CommitToDB"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "CommitToDB" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DisplayOI"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DisplayOI" ) == 0 )
   {
      //:SetCtrlState( vSubtask, "ViewList",   zCONTROL_STATUS_ENABLED, 1 )
      SetCtrlState( vSubtask, "ViewList", zCONTROL_STATUS_ENABLED, 1 );
      //:SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "ObjectList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "FileList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "FileList", zCONTROL_STATUS_ENABLED, 0 );
      //:SetCtrlState( vSubtask, "QualList",   zCONTROL_STATUS_ENABLED, 0 )
      SetCtrlState( vSubtask, "QualList", zCONTROL_STATUS_ENABLED, 0 );
   }

   //:END

   //:RefreshWindow( vSubtask )
   RefreshWindow( vSubtask );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:TZDBHTSD_SetOptionsFromResultLst( VIEW vSubtask )
//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_SetOptionsFromResultLst( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;
   zCHAR     szTempString_0[ 255 ];
   zCHAR     szTempString_1[ 255 ];
   zCHAR     szTempString_2[ 255 ];
   zCHAR     szTempString_3[ 255 ];
   zCHAR     szTempString_4[ 255 ];
   zCHAR     szTempString_5[ 255 ];
   zCHAR     szTempString_6[ 255 ];
   zCHAR     szTempString_7[ 255 ];
   zCHAR     szTempString_8[ 255 ];
   zCHAR     szTempString_9[ 255 ];
   zCHAR     szTempString_10[ 255 ];
   zCHAR     szTempString_11[ 255 ];
   zCHAR     szTempString_12[ 255 ];
   zCHAR     szTempString_13[ 255 ];

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:TZDBHTSO.Root.CurrentOperation = TZDBHTSO.ExecutionResult.Operation
   SetAttributeFromAttribute( TZDBHTSO, "Root", "CurrentOperation", TZDBHTSO, "ExecutionResult", "Operation" );

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromFile"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromFile" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName )
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_0 );
      //:fnAddListValue( TZDBHTSO, "LOD",
      //:                TZDBHTSO.ExecutionResult.LOD  )
      GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TZDBHTSO, "ExecutionResult", "LOD" );
      o_fnAddListValue( TZDBHTSO, "LOD", szTempString_1 );
      //:fnAddListValue( TZDBHTSO, "File",
      //:                TZDBHTSO.ExecutionResult.FileName  )
      GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), TZDBHTSO, "ExecutionResult", "FileName" );
      o_fnAddListValue( TZDBHTSO, "File", szTempString_2 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateFromDB"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateFromDB" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName  )
      GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_3 );
      //:fnAddListValue( TZDBHTSO, "LOD",
      //:                TZDBHTSO.ExecutionResult.LOD  )
      GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), TZDBHTSO, "ExecutionResult", "LOD" );
      o_fnAddListValue( TZDBHTSO, "LOD", szTempString_4 );
      //:fnAddListValue( TZDBHTSO, "Qual",
      //:                TZDBHTSO.ExecutionResult.QualView )
      GetStringFromAttribute( szTempString_5, sizeof( szTempString_5 ), TZDBHTSO, "ExecutionResult", "QualView" );
      o_fnAddListValue( TZDBHTSO, "Qual", szTempString_5 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "ActivateEmpty"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "ActivateEmpty" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName  )
      GetStringFromAttribute( szTempString_6, sizeof( szTempString_6 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_6 );
      //:fnAddListValue( TZDBHTSO, "LOD",
      //:                TZDBHTSO.ExecutionResult.LOD  )
      GetStringFromAttribute( szTempString_7, sizeof( szTempString_7 ), TZDBHTSO, "ExecutionResult", "LOD" );
      o_fnAddListValue( TZDBHTSO, "LOD", szTempString_7 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DisplayObjDef"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DisplayObjDef" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "LOD",
      //:                TZDBHTSO.ExecutionResult.LOD  )
      GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), TZDBHTSO, "ExecutionResult", "LOD" );
      o_fnAddListValue( TZDBHTSO, "LOD", szTempString_8 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DropView"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DropView" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName  )
      GetStringFromAttribute( szTempString_9, sizeof( szTempString_9 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_9 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "CommitToFile"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "CommitToFile" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName  )
      GetStringFromAttribute( szTempString_10, sizeof( szTempString_10 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_10 );
      //:fnAddListValue( TZDBHTSO, "File",
      //:                TZDBHTSO.ExecutionResult.FileName )
      GetStringFromAttribute( szTempString_11, sizeof( szTempString_11 ), TZDBHTSO, "ExecutionResult", "FileName" );
      o_fnAddListValue( TZDBHTSO, "File", szTempString_11 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "CommitToDB"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "CommitToDB" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName  )
      GetStringFromAttribute( szTempString_12, sizeof( szTempString_12 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_12 );
   }

   //:END

   //:IF TZDBHTSO.Root.CurrentOperation = "DisplayOI"
   if ( CompareAttributeToString( TZDBHTSO, "Root", "CurrentOperation", "DisplayOI" ) == 0 )
   {
      //:fnAddListValue( TZDBHTSO, "View",
      //:                TZDBHTSO.ExecutionResult.ViewName  )
      GetStringFromAttribute( szTempString_13, sizeof( szTempString_13 ), TZDBHTSO, "ExecutionResult", "ViewName" );
      o_fnAddListValue( TZDBHTSO, "View", szTempString_13 );
   }

   //:END

   //:TZDBHTSD_EnableControls( vSubtask )
   TZDBHTSD_EnableControls( vSubtask );
   return( 0 );
// END
}


//:LOCAL OPERATION
//:fnAddListValue( VIEW TZDBHTSO,
//:                STRING ( 32 ) szEntityName,
//:                STRING ( 32 ) szValue )
//:   Integer nRC
static zSHORT
o_fnAddListValue( zVIEW     TZDBHTSO,
                  zPCHAR    szEntityName,
                  zPCHAR    szValue )
{
   zLONG     nRC = 0;


   //:nRC = SetCursorFirstEntityByString( TZDBHTSO, szEntityName, "Name",
   //:                                    szValue, 0 )
   nRC = SetCursorFirstEntityByString( TZDBHTSO, szEntityName, "Name", szValue, 0 );

   //:IF nRC < zCURSOR_SET
   if ( nRC < zCURSOR_SET )
   {
      //:CreateEntity( TZDBHTSO, szEntityName, zPOS_LAST )
      CreateEntity( TZDBHTSO, szEntityName, zPOS_LAST );
      //:TZDBHTSO.szEntityName.Name = szValue
      SetAttributeFromString( TZDBHTSO, "szEntityName", "Name", szValue );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
//:EditApp_Prebuild( VIEW vSubtask )

//:   VIEW TZDBHTSO REGISTERED AS TZDBHTSO
zOPER_EXPORT zSHORT OPERATION
EditApp_Prebuild( zVIEW     vSubtask )
{
   zVIEW     TZDBHTSO = 0;
   zSHORT    RESULT;

   RESULT = GetViewByName( &TZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );

   //:IF TZDBHTSO.Application.Name = ""
   if ( CompareAttributeToString( TZDBHTSO, "Application", "Name", "" ) == 0 )
   {
      //:TZDBHTSO.Application.Name = "New"
      SetAttributeFromString( TZDBHTSO, "Application", "Name", "New" );
   }

   //:END
   return( 0 );
// END
}


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
ShowTrans_Postbuild( zVIEW     vSubtask )
{

   //:ShowTrans_Postbuild( VIEW vSubtask )

   //:NAME VIEW vSubtask "ShowTransformation"
   SetNameForView( vSubtask, "ShowTransformation", 0, zLEVEL_TASK );
   return( 0 );
// END
}


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
ShowTrans_RefreshArgs( zVIEW     vSubtask )
{

   //:ShowTrans_RefreshArgs( VIEW vSubtask )

   //:RefreshCtrl( vSubtask, "ArgumentList" )
   RefreshCtrl( vSubtask, "ArgumentList" );
   return( 0 );
// END
}


//:DIALOG OPERATION
//:TZDBHTSD_RefreshTrans( VIEW vSubtask )
//:   VIEW vTranWindow
zOPER_EXPORT zSHORT OPERATION
TZDBHTSD_RefreshTrans( zVIEW     vSubtask )
{
   zVIEW     vTranWindow = 0;
   zSHORT    RESULT;


   //:GET VIEW vTranWindow NAMED "ShowTransformation"
   RESULT = GetViewByName( &vTranWindow, "ShowTransformation", vSubtask, zLEVEL_TASK );
   //:IF vTranWindow != 0
   if ( vTranWindow != 0 )
   {
      //:RefreshWindow( vTranWindow )
      RefreshWindow( vTranWindow );
   }

   //:END
   return( 0 );
// END
}



#ifdef __cplusplus
}
#endif
