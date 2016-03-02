#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZERSASO_SetCursorNewRelType( zVIEW     OldERD,
                               zVIEW     NewERD );


//:TRANSFORMATION OPERATION
//:SA_Migrate( VIEW          NewSA      BASED ON LOD TZERSASO,
//:            STRING ( 32 ) SubjectAreaName,
//:            VIEW          SourceLPLR BASED ON LOD TZCMLPLO,
//:            VIEW          vSubtask )

//:   VIEW NewERD      BASED ON LOD TZEREMDO
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_SA_Migrate( zPVIEW    NewSA,
                      zPCHAR    SubjectAreaName,
                      zVIEW     SourceLPLR,
                      zVIEW     vSubtask )
{
   zVIEW     NewERD = 0; 
   //:VIEW OldERD      BASED ON LOD TZEREMDO
   zVIEW     OldERD = 0; 
   //:VIEW OldSA       BASED ON LOD TZERSASO
   zVIEW     OldSA = 0; 
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 

   //:STRING ( 513 ) SourceFileName  // size according to zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 


   //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zREFER_ERD_META );

   //:GET VIEW NewERD NAMED "NewERD"
   RESULT = GetViewByName( &NewERD, "NewERD", SourceLPLR, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:IF CurrentLPLR.W_MetaDef EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( CurrentLPLR, "W_MetaDef" );
      if ( lTempInteger_0 == 0 )
      { 
         //:// Activate the current ERD.  If it doesn't exist, it is an error.
         //:nRC = ActivateMetaOI( vSubtask, NewERD, CurrentLPLR, zREFER_ERD_META, 0 )
         nRC = ActivateMetaOI( vSubtask, &NewERD, CurrentLPLR, zREFER_ERD_META, 0 );
         //:ELSE
      } 
      else
      { 
         //:nRC = -1
         nRC = -1;
      } 

      //:END

      //:IF nRC < 1
      if ( nRC < 1 )
      { 
         //:MessageSend( vSubtask, "ER00401", "Configuration Management",
         //:             "No current Data Model exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "ER00401", "Configuration Management", "No current Data Model exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:RETURN -1
         return( -1 );
      } 

      //:END
      //:NAME VIEW  NewERD "NewERD"
      SetNameForView( NewERD, "NewERD", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Activate existing source meta OldSA
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + SubjectAreaName + ".PSA"
   GetStringFromAttribute( SourceFileName, zsizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, SubjectAreaName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PSA", 1, 0, 514 );

   //:ActivateOI_FromFile ( OldSA, "TZERSASO", CurrentLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &OldSA, "TZERSASO", CurrentLPLR, SourceFileName, 8192 );
   //:// 8192 is zIGNORE_ATTRIB_ERRORS
   //:NAME VIEW OldSA "OldSA"
   SetNameForView( OldSA, "OldSA", 0, zLEVEL_TASK );

   //:// Also activate original ERD that goes with the SA
   //:GET VIEW OldERD NAMED "OldERD"
   RESULT = GetViewByName( &OldERD, "OldERD", SourceLPLR, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:SET CURSOR FIRST SourceLPLR.W_MetaType WHERE SourceLPLR.W_MetaType.Type = 2004
      RESULT = SetCursorFirstEntityByInteger( SourceLPLR, "W_MetaType", "Type", 2004, "" );
      //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + SourceLPLR.W_MetaDef.Name + ".PMD"
      GetStringFromAttribute( SourceFileName, zsizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, SourceLPLR, "W_MetaDef", "Name", "", 0 );
      ZeidonStringConcat( SourceFileName, 1, 0, szTempString_0, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, ".PMD", 1, 0, 514 );
      //:ActivateOI_FromFile ( OldERD, "TZEREMDO", SourceLPLR, SourceFileName, 8192 )
      ActivateOI_FromFile( &OldERD, "TZEREMDO", SourceLPLR, SourceFileName, 8192 );
      //:// 8192 is zIGNORE_ATTRIB_ERRORS
      //:NAME VIEW OldERD "OldERD"
      SetNameForView( OldERD, "OldERD", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Perform the code that actually builds the new SA.
   //:nRC = SA_MigrateWithERDs( NewSA,
   //:                          OldSA,
   //:                          NewERD,
   //:                          OldERD,
   //:                          vSubtask )
   nRC = oTZERSASO_SA_MigrateWithERDs( NewSA, OldSA, NewERD, OldERD, vSubtask );

   //:// Then clean up and commit the SA.
   //:DropObjectInstance( OldSA )
   DropObjectInstance( OldSA );
   //:DropMetaOI( vSubtask, CurrentLPLR )
   DropMetaOI( vSubtask, CurrentLPLR );

   //:IF nRC = 0
   if ( nRC == 0 )
   { 
      //:CommitMetaOI( vSubtask, NewSA, 5 )
      CommitMetaOI( vSubtask, *NewSA, 5 );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SA_MigrateWithERDs( VIEW NewSA  BASED ON LOD TZERSASO,
//:                    VIEW OldSA  BASED ON LOD TZERSASO,
//:                    VIEW NewERD BASED ON LOD TZEREMDO,
//:                    VIEW OldERD BASED ON LOD TZEREMDO,
//:                    VIEW vSubtask )

//:   STRING (32)  szEntityName
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_SA_MigrateWithERDs( zPVIEW    NewSA,
                              zVIEW     OldSA,
                              zVIEW     NewERD,
                              zVIEW     OldERD,
                              zVIEW     vSubtask )
{
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING (32)  szSA_Name
   zCHAR     szSA_Name[ 33 ] = { 0 }; 
   //:STRING (254) MG_ErrorMessage
   zCHAR     MG_ErrorMessage[ 255 ] = { 0 }; 
   //:SHORT        nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 


   //:ActivateEmptyMetaOI( vSubtask, NewSA, zSOURCE_SA_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, NewSA, zSOURCE_SA_META, zSINGLE );
   //:NAME VIEW NewSA "NewSA"
   SetNameForView( *NewSA, "NewSA", 0, zLEVEL_TASK );

   //:IF NewSA.SubjectArea DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( *NewSA, "SubjectArea" );
   if ( lTempInteger_0 != 0 )
   { 
      //:CreateMetaEntity( vSubtask, NewSA, "SubjectArea", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewSA, "SubjectArea", zPOS_AFTER );
      //:SetMatchingAttributesByName ( NewSA, "SubjectArea", OldSA,
      //:                              "SubjectArea", zSET_NULL )
      SetMatchingAttributesByName( *NewSA, "SubjectArea", OldSA, "SubjectArea", zSET_NULL );
   } 

   //:END

   //:FOR EACH OldSA.SA_Entity
   RESULT = SetCursorFirstEntity( OldSA, "SA_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:SET CURSOR FIRST OldERD.ER_Entity WHERE
      //:    OldERD.ER_Entity.ZKey = OldSA.ER_Entity.ZKey
      GetIntegerFromAttribute( &lTempInteger_1, OldSA, "ER_Entity", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_Entity", "ZKey", lTempInteger_1, "" );

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST NewSA.ER_Entity WITHIN NewSA.SubjectArea WHERE
         //:       NewSA.ER_Entity.Name = OldERD.ER_Entity.Name
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldERD, "ER_Entity", "Name" );
         RESULT = SetCursorFirstEntityByString( *NewSA, "ER_Entity", "Name", szTempString_0, "SubjectArea" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CreateMetaEntity( vSubtask, NewSA, "SA_Entity", zPOS_AFTER )
            CreateMetaEntity( vSubtask, *NewSA, "SA_Entity", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewSA, "SA_Entity", OldSA,
            //:                              "SA_Entity", zSET_NULL )
            SetMatchingAttributesByName( *NewSA, "SA_Entity", OldSA, "SA_Entity", zSET_NULL );

            //:SET CURSOR FIRST NewERD.ER_Entity WHERE
            //:    NewERD.ER_Entity.Name = OldERD.ER_Entity.Name
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldERD, "ER_Entity", "Name" );
            RESULT = SetCursorFirstEntityByString( NewERD, "ER_Entity", "Name", szTempString_0, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:szEntityName = OldERD.ER_Entity.Name
               GetVariableFromAttribute( szEntityName, 0, 'S', 33, OldERD, "ER_Entity", "Name", "", 0 );
               //:szSA_Name    = OldSA.SubjectArea.Name
               GetVariableFromAttribute( szSA_Name, 0, 'S', 33, OldSA, "SubjectArea", "Name", "", 0 );
               //:MG_ErrorMessage = "Migration of Subject Area (" + szSA_Name + ") skipped, " +
               //:                  "because Entity (" + szEntityName + ") was not found in " +
               //:                  "Target ERD."
               ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migration of Subject Area (", 1, 0, 255 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, szSA_Name, 1, 0, 255 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") skipped, ", 1, 0, 255 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, "because Entity (", 1, 0, 255 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, szEntityName, 1, 0, 255 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") was not found in ", 1, 0, 255 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, "Target ERD.", 1, 0, 255 );
               //:MessageSend( vSubtask, "ER00402", "Configuration Management",
               //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vSubtask, "ER00402", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               //:RETURN -1
               return( -1 );
            } 

            //:END
            //:INCLUDE NewSA.ER_Entity FROM NewERD.ER_Entity
            RESULT = IncludeSubobjectFromSubobject( *NewSA, "ER_Entity", NewERD, "ER_Entity", zPOS_AFTER );
            //:FOR EACH OldSA.ER_RelLink
            RESULT = SetCursorFirstEntity( OldSA, "ER_RelLink", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 

               //:SET CURSOR FIRST OldERD.ER_RelType WHERE
               //:   OldERD.ER_RelType.ZKey = OldSA.ER_RelType_1.ZKey
               GetIntegerFromAttribute( &lTempInteger_2, OldSA, "ER_RelType_1", "ZKey" );
               RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelType", "ZKey", lTempInteger_2, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:SET CURSOR FIRST OldERD.ER_RelLink_2 WHERE
                  //:    OldERD.ER_RelLink_2.ZKey = OldSA.ER_RelLink.ZKey
                  GetIntegerFromAttribute( &lTempInteger_3, OldSA, "ER_RelLink", "ZKey" );
                  RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelLink_2", "ZKey", lTempInteger_3, "" );

                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:nRC = SetCursorNewRelType( OldERD, NewERD )
                     nRC = oTZERSASO_SetCursorNewRelType( OldERD, NewERD );

                     //:IF nRC = 0
                     if ( nRC == 0 )
                     { 
                        //:INCLUDE NewSA.ER_RelLink FROM NewERD.ER_RelLink_2
                        RESULT = IncludeSubobjectFromSubobject( *NewSA, "ER_RelLink", NewERD, "ER_RelLink_2", zPOS_AFTER );

                        //:// Now position on other ER_RelLink_2
                        //:SET CURSOR NEXT NewERD.ER_RelLink_2
                        RESULT = SetCursorNextEntity( NewERD, "ER_RelLink_2", "" );
                        //:IF RESULT < zCURSOR_SET
                        if ( RESULT < zCURSOR_SET )
                        { 
                           //:SET CURSOR PREVIOUS NewERD.ER_RelLink_2
                           RESULT = SetCursorPrevEntity( NewERD, "ER_RelLink_2", "" );
                        } 

                        //:END

                        //:INCLUDE NewSA.ER_RelLink_Other FROM NewERD.ER_RelLink_2
                        RESULT = IncludeSubobjectFromSubobject( *NewSA, "ER_RelLink_Other", NewERD, "ER_RelLink_2", zPOS_AFTER );
                     } 

                     //:END
                     //:ELSE
                  } 
                  else
                  { 
                     //:szEntityName = OldERD.ER_Entity.Name
                     GetVariableFromAttribute( szEntityName, 0, 'S', 33, OldERD, "ER_Entity", "Name", "", 0 );
                     //:szSA_Name    = OldSA.SubjectArea.Name
                     GetVariableFromAttribute( szSA_Name, 0, 'S', 33, OldSA, "SubjectArea", "Name", "", 0 );
                     //:MG_ErrorMessage = "Migration of Subject Area (" + szSA_Name +
                     //:                  ") skipped, because Relationship for Entity (" +
                     //:                  szEntityName + ") was not found in Target ERD."
                     ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migration of Subject Area (", 1, 0, 255 );
                     ZeidonStringConcat( MG_ErrorMessage, 1, 0, szSA_Name, 1, 0, 255 );
                     ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") skipped, because Relationship for Entity (", 1, 0, 255 );
                     ZeidonStringConcat( MG_ErrorMessage, 1, 0, szEntityName, 1, 0, 255 );
                     ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") was not found in Target ERD.", 1, 0, 255 );
                     //:MessageSend( vSubtask, "ER00402", "Configuration Management",
                     //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vSubtask, "ER00402", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:RETURN -1
                     return( -1 );
                  } 

                  //:END
               } 

               RESULT = SetCursorNextEntity( OldSA, "ER_RelLink", "" );

               //:END
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( OldSA, "SA_Entity", "" );

      //:END
   } 


   //:END

   //:FOR EACH OldSA.SA_RelType
   RESULT = SetCursorFirstEntity( OldSA, "SA_RelType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:SET CURSOR FIRST OldERD.ER_RelType WHERE
      //:    OldERD.ER_RelType.ZKey = OldSA.ER_RelType.ZKey
      GetIntegerFromAttribute( &lTempInteger_4, OldSA, "ER_RelType", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelType", "ZKey", lTempInteger_4, "" );

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 

         //:SET CURSOR FIRST OldERD.ER_RelLink_2 WHERE
         //:    OldERD.ER_RelLink_2.ZKey = OldSA.ER_RelLink_2.ZKey
         GetIntegerFromAttribute( &lTempInteger_5, OldSA, "ER_RelLink_2", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelLink_2", "ZKey", lTempInteger_5, "" );

         //:nRC = SetCursorNewRelType( OldERD, NewERD )
         nRC = oTZERSASO_SetCursorNewRelType( OldERD, NewERD );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:CreateMetaEntity( vSubtask, NewSA, "SA_RelType", zPOS_AFTER )
            CreateMetaEntity( vSubtask, *NewSA, "SA_RelType", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewSA, "SA_RelType", OldSA,
            //:                               "SA_RelType", zSET_NULL )
            SetMatchingAttributesByName( *NewSA, "SA_RelType", OldSA, "SA_RelType", zSET_NULL );
            //:INCLUDE NewSA.ER_RelType FROM NewERD.ER_RelType
            RESULT = IncludeSubobjectFromSubobject( *NewSA, "ER_RelType", NewERD, "ER_RelType", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( OldSA, "SA_RelType", "" );
      //:END
   } 


   //:END

   //:INCLUDE NewSA.EntpER_Model FROM NewERD.EntpER_Model
   RESULT = IncludeSubobjectFromSubobject( *NewSA, "EntpER_Model", NewERD, "EntpER_Model", zPOS_AFTER );

   //:FOR EACH OldSA.OwnedER_Entity
   RESULT = SetCursorFirstEntity( OldSA, "OwnedER_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:SET CURSOR FIRST OldERD.ER_Entity WHERE
      //:    OldERD.ER_Entity.ZKey = OldSA.OwnedER_Entity.ZKey
      GetIntegerFromAttribute( &lTempInteger_6, OldSA, "OwnedER_Entity", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_Entity", "ZKey", lTempInteger_6, "" );

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST NewERD.ER_Entity WHERE
         //:    NewERD.ER_Entity.Name = OldERD.ER_Entity.Name
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldERD, "ER_Entity", "Name" );
         RESULT = SetCursorFirstEntityByString( NewERD, "ER_Entity", "Name", szTempString_0, "" );
         //:INCLUDE NewSA.OwnedER_Entity FROM NewERD.ER_Entity
         RESULT = IncludeSubobjectFromSubobject( *NewSA, "OwnedER_Entity", NewERD, "ER_Entity", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( OldSA, "OwnedER_Entity", "" );
      //:END
   } 


   //:END

   //:FOR EACH OldSA.OwnedER_RelType
   RESULT = SetCursorFirstEntity( OldSA, "OwnedER_RelType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:SET CURSOR FIRST OldERD.ER_RelType WHERE
      //:    OldERD.ER_RelType.ZKey = OldSA.OwnedER_RelType.ZKey
      GetIntegerFromAttribute( &lTempInteger_7, OldSA, "OwnedER_RelType", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelType", "ZKey", lTempInteger_7, "" );

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 

         //:nRC = SetCursorNewRelType( OldERD, NewERD )
         nRC = oTZERSASO_SetCursorNewRelType( OldERD, NewERD );
         //:IF nRC = 0
         if ( nRC == 0 )
         { 
            //:INCLUDE NewSA.OwnedER_RelType FROM NewERD.ER_RelType
            RESULT = IncludeSubobjectFromSubobject( *NewSA, "OwnedER_RelType", NewERD, "ER_RelType", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( OldSA, "OwnedER_RelType", "" );

      //:END
   } 


   //:END
   //:RETURN 0
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SA_RelinkDelete( VIEW vSA BASED ON LOD TZERSASO, VIEW vSubtask )

//:   VIEW    vERD   BASED ON LOD TZEREMDO
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_SA_RelinkDelete( zVIEW     vSA,
                           zVIEW     vSubtask )
{
   zVIEW     vERD = 0; 
   //:VIEW    vLPLR  BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 

   //:INTEGER DeleteFlag
   zLONG     DeleteFlag = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 

   //://   STRING (64) MG_ErrorMessage

   //:DeleteFlag = 0
   DeleteFlag = 0;
   //:RetrieveViewForMetaList( vSubtask, vLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   //:ActivateMetaOI( vSubtask, vERD, vLPLR, zREFER_ERD_META, 0 )
   ActivateMetaOI( vSubtask, &vERD, vLPLR, zREFER_ERD_META, 0 );
   //:DropMetaOI( vSubtask, vLPLR )
   DropMetaOI( vSubtask, vLPLR );

   //:FOR EACH vSA.SA_Entity
   RESULT = SetCursorFirstEntity( vSA, "SA_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vERD.ER_Entity WHERE
      //:    vERD.ER_Entity.ZKey = vSA.ER_Entity.ZKey
      GetIntegerFromAttribute( &lTempInteger_0, vSA, "ER_Entity", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Entity", "ZKey", lTempInteger_0, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Relink ER_Entity.  Will this create a problem for ER_RelLink entries?
         //:// Do we need to relink ER_RelLink entries & ER_RelLink_Other entries?
         //:// Or will this be taken care of under SA_RelType processing below?
         //:FOR EACH vSA.ER_RelLink
         RESULT = SetCursorFirstEntity( vSA, "ER_RelLink", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:SET CURSOR FIRST vERD.ER_RelType WHERE
            //:    vERD.ER_RelType.ZKey = vSA.ER_RelType_1.ZKey
            GetIntegerFromAttribute( &lTempInteger_1, vSA, "ER_RelType_1", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelType", "ZKey", lTempInteger_1, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// ??
               //:ELSE
            } 
            else
            { 
               //://               MG_ErrorMessage = "Excluding SA RelLink: " + vSA.ER_RelLink.Name
               //://               MessageSend( vSubtask, "ER00405", "E/R Model Maintainance",
               //://                            MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               //:EXCLUDE vSA.ER_RelLink NONE
               RESULT = ExcludeEntity( vSA, "ER_RelLink", zREPOS_NONE );
            } 

            RESULT = SetCursorNextEntity( vSA, "ER_RelLink", "" );

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //://         MG_ErrorMessage = "Deleting SA Entity: " + vSA.ER_Entity.Name
         //://         MessageSend( vSubtask, "ER00406", "E/R Model Maintainance",
         //://                      MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         //:DELETE ENTITY vSA.SA_Entity NONE
         RESULT = DeleteEntity( vSA, "SA_Entity", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( vSA, "SA_Entity", "" );
      //:END
   } 

   //:END

   //:FOR EACH vSA.SA_RelType
   RESULT = SetCursorFirstEntity( vSA, "SA_RelType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vERD.ER_RelType WHERE
      //:    vERD.ER_RelType.ZKey = vSA.ER_RelType.ZKey
      GetIntegerFromAttribute( &lTempInteger_2, vSA, "ER_RelType", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelType", "ZKey", lTempInteger_2, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Relink ER_RelType
         //:ELSE
      } 
      else
      { 
         //://         MG_ErrorMessage = "Deleting SA RelType for: " + vSA.ER_RelLink_2.Name
         //://         MessageSend( vSubtask, "ER00407", "E/R Model Maintainance",
         //://                      MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         //:DELETE ENTITY vSA.SA_RelType NONE
         RESULT = DeleteEntity( vSA, "SA_RelType", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( vSA, "SA_RelType", "" );
      //:END
   } 

   //:END

   //:FOR EACH vSA.OwnedER_Entity
   RESULT = SetCursorFirstEntity( vSA, "OwnedER_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vERD.ER_Entity WHERE
      //:    vERD.ER_Entity.ZKey = vSA.OwnedER_Entity.ZKey
      GetIntegerFromAttribute( &lTempInteger_3, vSA, "OwnedER_Entity", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Entity", "ZKey", lTempInteger_3, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Relink OwnedER_Entity
         //:ELSE
      } 
      else
      { 
         //://         MG_ErrorMessage = "Excluding SA Owned Entity: " + vSA.OwnedER_Entity.Name
         //://         MessageSend( vSubtask, "ER00408", "E/R Model Maintainance",
         //://                      MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         //:EXCLUDE vSA.OwnedER_Entity NONE
         RESULT = ExcludeEntity( vSA, "OwnedER_Entity", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( vSA, "OwnedER_Entity", "" );
      //:END
   } 

   //:END

   //:FOR EACH vSA.OwnedER_RelType
   RESULT = SetCursorFirstEntity( vSA, "OwnedER_RelType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vERD.ER_RelType WHERE
      //:    vERD.ER_RelType.ZKey = vSA.OwnedER_RelType.ZKey
      GetIntegerFromAttribute( &lTempInteger_4, vSA, "OwnedER_RelType", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelType", "ZKey", lTempInteger_4, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Relink OwnedER_RelType
         //:ELSE
      } 
      else
      { 
         //://         MG_ErrorMessage = "Excluding SA Owned RelType"
         //://         MessageSend( vSubtask, "ER00409", "E/R Model Maintainance",
         //://                      MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         //:EXCLUDE vSA.OwnedER_RelType NONE
         RESULT = ExcludeEntity( vSA, "OwnedER_RelType", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( vSA, "OwnedER_RelType", "" );
      //:END
   } 

   //:END

   //:DropMetaOI( vSubtask, vERD )
   DropMetaOI( vSubtask, vERD );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:SetCursorNewRelType( VIEW OldERD BASED ON LOD TZEREMDO,
//:                     VIEW NewERD BASED ON LOD TZEREMDO )

//:   VIEW NewERD2 BASED ON LOD TZEREMDO
static zSHORT
oTZERSASO_SetCursorNewRelType( zVIEW     OldERD,
                               zVIEW     NewERD )
{
   zVIEW     NewERD2 = 0; 
   //:VIEW OldERD2 BASED ON LOD TZEREMDO
   zVIEW     OldERD2 = 0; 

   //:INTEGER FoundFlag
   zLONG     FoundFlag = 0; 
   zSHORT    RESULT; 


   //:// Locate the ER_RelType in the NewERD, based on the corresponding ER_RelType
   //:// in the OldERD.  This is kind of convoluted, since the match has to
   //:// consider both sides of the RelLink under the RelType.
   //:// A return code of -1 is returned if the ER_RelType was not located.
   //:// This is a valid condition when doing an SA_merge.

   //:FoundFlag = 0
   FoundFlag = 0;
   //:SET CURSOR FIRST NewERD.ER_Entity_2 WITHIN NewERD.EntpER_Model WHERE
   //:    NewERD.ER_Entity_2.Name  = OldERD.ER_Entity_2.Name AND
   //:    NewERD.ER_RelLink_2.Name = OldERD.ER_RelLink_2.Name
   RESULT = SetCursorFirstEntity( NewERD, "ER_Entity_2", "EntpER_Model" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( NewERD, "ER_Entity_2", "Name", OldERD, "ER_Entity_2", "Name" ) != 0 || CompareAttributeToAttribute( NewERD, "ER_RelLink_2", "Name", OldERD, "ER_RelLink_2", "Name" ) != 0 ) )
      { 
         RESULT = SetCursorNextEntity( NewERD, "ER_Entity_2", "EntpER_Model" );
      } 

   } 

   //:LOOP WHILE FoundFlag = 0
   while ( FoundFlag == 0 )
   { 
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Now make sure the entity on the other side of the RelLink is correct.
         //:// If not, continue searching.
         //:CreateViewFromViewForTask( OldERD2, OldERD, 0 )
         CreateViewFromViewForTask( &OldERD2, OldERD, 0 );
         //:CreateViewFromViewForTask( NewERD2, NewERD, 0 )
         CreateViewFromViewForTask( &NewERD2, NewERD, 0 );
         //:SET CURSOR NEXT OldERD2.ER_RelLink_2
         RESULT = SetCursorNextEntity( OldERD2, "ER_RelLink_2", "" );
         //:SET CURSOR NEXT NewERD2.ER_RelLink_2
         RESULT = SetCursorNextEntity( NewERD2, "ER_RelLink_2", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR PREVIOUS OldERD2.ER_RelLink_2
            RESULT = SetCursorPrevEntity( OldERD2, "ER_RelLink_2", "" );
            //:SET CURSOR PREVIOUS NewERD2.ER_RelLink_2
            RESULT = SetCursorPrevEntity( NewERD2, "ER_RelLink_2", "" );
         } 

         //:END
         //:IF NewERD2.ER_Entity_2.Name  = OldERD2.ER_Entity_2.Name AND
         //:   NewERD2.ER_RelLink_2.Name = OldERD2.ER_RelLink_2.Name
         if ( CompareAttributeToAttribute( NewERD2, "ER_Entity_2", "Name", OldERD2, "ER_Entity_2", "Name" ) == 0 && CompareAttributeToAttribute( NewERD2, "ER_RelLink_2", "Name", OldERD2, "ER_RelLink_2", "Name" ) == 0 )
         { 
            //:FoundFlag = 1
            FoundFlag = 1;
            //:ELSE
         } 
         else
         { 
            //:SET CURSOR NEXT NewERD.ER_Entity_2 WITHIN NewERD.EntpER_Model WHERE
            //:    NewERD.ER_Entity_2.Name  = OldERD.ER_Entity_2.Name AND
            //:    NewERD.ER_RelLink_2.Name = OldERD.ER_RelLink_2.Name
            RESULT = SetCursorNextEntity( NewERD, "ER_Entity_2", "EntpER_Model" );
            if ( RESULT > zCURSOR_UNCHANGED )
            { 
               while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( NewERD, "ER_Entity_2", "Name", OldERD, "ER_Entity_2", "Name" ) != 0 ||
                       CompareAttributeToAttribute( NewERD, "ER_RelLink_2", "Name", OldERD, "ER_RelLink_2", "Name" ) != 0 ) )
               { 
                  RESULT = SetCursorNextEntity( NewERD, "ER_Entity_2", "EntpER_Model" );
               } 

            } 

         } 

         //:END
         //:DropView( OldERD2 )
         DropView( OldERD2 );
         //:DropView( NewERD2 )
         DropView( NewERD2 );
         //:ELSE
      } 
      else
      { 
         //:RETURN -1
         return( -1 );
      } 

      //:END
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
