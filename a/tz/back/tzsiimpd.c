#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "tzlodopr.h" 
#include "ZDRVROPR.H" 
#include "TZ__OPRS.H" 
#include "TZVMLIP.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
ActivateWorkObject( zVIEW     vSubtask );


static zSHORT
o_ActivateAndImport( zVIEW     vSubtask,
                     zPCHAR    szFilename );


static zSHORT
o_ImportCreateERD( zVIEW     vWkRecur,
                   zVIEW     vERD,
                   zVIEW     vLPLRDomain,
                   zVIEW     vSubtask,
                   zPCHAR    szCatalog,
                   zSHORT    sFirst );


static zVOID
o_ImportERD_Recursive( zVIEW     vWkRecur,
                       zVIEW     vWkMain,
                       zVIEW     vERD,
                       zVIEW     vLPLRDomain,
                       zVIEW     vSubtask );


static zSHORT
o_ImportWorkOIToZeidon( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
DeleteWorkObject( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CreateKatalogList( zVIEW     vSubtask );


static zVOID
o_TransformSubFields( zVIEW     vSubtask,
                      zVIEW     Wk1,
                      zVIEW     Wk2 );


zOPER_EXPORT zSHORT OPERATION
LoadServerList( zVIEW     vSubtask );


static zSHORT
o_ShowMessage( zVIEW     vSubtask,
               zSHORT    MessageNumber );


zOPER_EXPORT zSHORT OPERATION
DropWorkViews( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
LoadCatalogList( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
RefreshCatalogList( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ImportCatalogAndWSK( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
EnableButtons( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
DropAllWorkViews( zVIEW     vSubtask );


static zSHORT
o_ChangeEntityOrAttributeName( zVIEW     vSubtask,
                               zPCHAR    szName );


//:DIALOG OPERATION
//:ActivateWorkObject( VIEW vSubtask )

//:    VIEW WkList BASED ON LOD TZSIIMLO
zOPER_EXPORT zSHORT OPERATION
ActivateWorkObject( zVIEW     vSubtask )
{
   zVIEW     WkList = 0; 
   //: INTEGER     nRC
   zLONG     nRC = 0; 
   //: STRING (76) FileName
   zCHAR     FileName[ 77 ] = { 0 }; 
   //: STRING (16) KatalogName
   zCHAR     KatalogName[ 17 ] = { 0 }; 
   //: STRING (64) DirectoryPath
   zCHAR     DirectoryPath[ 65 ] = { 0 }; 
   zSHORT    RESULT; 


   //: // Get the directory path where we will find the import work objects.
   //: nRC = GetSironWorkObjPathName( DirectoryPath, 64 )
   nRC = GetSironWorkObjPathName( DirectoryPath, 64 );

   //: IF nRC < 0
   if ( nRC < 0 )
   { 
      //: MessageSend( vSubtask, "SI00106", "Catalog Import",
      //:              "Cannot find directory path.",
      //:              zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "SI00106", "Catalog Import", "Cannot find directory path.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //: RETURN
      return( 0 );
   } 

   //: END

   //: // Use the work object list to activate the work object.
   //: // Just concatenate the directory path to the name from the list, plus a suffix.
   //: GET VIEW WkList NAMED "TZSIIMLO"
   RESULT = GetViewByName( &WkList, "TZSIIMLO", vSubtask, zLEVEL_TASK );
   //: KatalogName = WkList.KatalogFile.Name
   GetVariableFromAttribute( KatalogName, 0, 'S', 17, WkList, "KatalogFile", "Name", "", 0 );
   //: FileName = DirectoryPath + KatalogName + ".WSK"
   ZeidonStringCopy( FileName, 1, 0, DirectoryPath, 1, 0, 77 );
   ZeidonStringConcat( FileName, 1, 0, KatalogName, 1, 0, 77 );
   ZeidonStringConcat( FileName, 1, 0, ".WSK", 1, 0, 77 );

   //: // Check if the file exists or if someone deleted it
   //: nRC = SysOpenFile( vSubtask, FileName /* File Name */,
   //:                    COREFILE_READ)
   nRC = SysOpenFile( vSubtask, FileName, COREFILE_READ );
   //: IF  nRC < 0
   if ( nRC < 0 )
   { 
      //: // We cannot open the file
      //: ShowMessage( vSubtask, 582 )
      o_ShowMessage( vSubtask, 582 );

      //: // Delete the file from the list and disable OK and Delete Buttons
      //: // if the last entity was deleted
      //: DELETE ENTITY  WkList.KatalogFile NONE
      RESULT = DeleteEntity( WkList, "KatalogFile", zREPOS_NONE );
      //: EnableButtons( vSubtask )
      EnableButtons( vSubtask );

      //: RefreshWindow( vSubtask /* Subtask */ )
      RefreshWindow( vSubtask );
      //:ELSE
   } 
   else
   { 
      //: SysCloseFile( vSubtask, nRC /* File Handle */,
      //:               0 /* Owning Task */ )
      SysCloseFile( vSubtask, nRC, 0 );
      //: //Activate from File and import into the Datamodel
      //: ActivateAndImport( vSubtask, FileName )
      o_ActivateAndImport( vSubtask, FileName );
   } 

   //: END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ActivateAndImport( VIEW vSubtask, STRING (256) szFilename )

//:    VIEW Wk1    BASED ON LOD TZSIWK1O
static zSHORT
o_ActivateAndImport( zVIEW     vSubtask,
                     zPCHAR    szFilename )
{
   zVIEW     Wk1 = 0; 
   //: VIEW Wk2    BASED ON LOD TZSIWK1O
   zVIEW     Wk2 = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //: ActivateOI_FromFile( Wk1, "TZSIWK1O", vSubtask,
   //:                      szFilename, zSINGLE )
   ActivateOI_FromFile( &Wk1, "TZSIWK1O", vSubtask, szFilename, zSINGLE );
   //: NAME VIEW Wk1 "TZSIWK1O"
   SetNameForView( Wk1, "TZSIWK1O", 0, zLEVEL_TASK );

   //: // Wk2 has HD_Diagram view, while Wk1 has SironKatalog view
   //: CreateViewFromViewForTask( Wk2, Wk1, vSubtask )
   CreateViewFromViewForTask( &Wk2, Wk1, vSubtask );
   //: CreateMetaEntity( vSubtask, Wk1, "HD_Diagram", zPOS_AFTER )
   CreateMetaEntity( vSubtask, Wk1, "HD_Diagram", zPOS_AFTER );
   //: Wk1.HD_Diagram.Orientation = 0
   SetAttributeFromInteger( Wk1, "HD_Diagram", "Orientation", 0 );
   //: Wk1.HD_Diagram.PrintBox = "Y"
   SetAttributeFromString( Wk1, "HD_Diagram", "PrintBox", "Y" );
   //: Wk1.HD_Diagram.PrintTitle = Wk2.SironKatalog.Name
   SetAttributeFromAttribute( Wk1, "HD_Diagram", "PrintTitle", Wk2, "SironKatalog", "Name" );

   //: CreateMetaEntity( vSubtask, Wk1, "HD_Node", zPOS_AFTER )
   CreateMetaEntity( vSubtask, Wk1, "HD_Node", zPOS_AFTER );
   //: Wk1.HD_Node.Name = Wk2.OrigSironField.FieldName
   SetAttributeFromAttribute( Wk1, "HD_Node", "Name", Wk2, "OrigSironField", "FieldName" );
   //: Wk1.HD_Node.Text = Wk2.OrigSironField.FieldName
   SetAttributeFromAttribute( Wk1, "HD_Node", "Text", Wk2, "OrigSironField", "FieldName" );

   //: // If the group item has no fields that are themselves not group items,
   //: // we will turn the color of the entity to green (Color=2).
   //: // Otherwise, we will turn the color to yellow (Color=1).
   //: IF Wk1.NewField EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( Wk1, "NewField" );
   if ( lTempInteger_0 == 0 )
   { 
      //: Wk1.HD_Node.Color  = 1
      SetAttributeFromInteger( Wk1, "HD_Node", "Color", 1 );
      //:ELSE
   } 
   else
   { 
      //: Wk1.HD_Node.Color  = 0
      SetAttributeFromInteger( Wk1, "HD_Node", "Color", 0 );
   } 

   //: END

   //: CreateMetaEntity( vSubtask, Wk2, "NewEntity", zPOS_AFTER )
   CreateMetaEntity( vSubtask, Wk2, "NewEntity", zPOS_AFTER );
   //: SetMatchingAttributesByName( Wk2, "NewEntity", Wk1, "OrigSironField", zSET_NULL )
   SetMatchingAttributesByName( Wk2, "NewEntity", Wk1, "OrigSironField", zSET_NULL );
   //: Wk2.NewEntity.KatalogFlag = "Y"
   SetAttributeFromString( Wk2, "NewEntity", "KatalogFlag", "Y" );

   //: // Execute recursive subroutine to transform sub fields.
   //: IF Wk1.OrigSironSubField EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( Wk1, "OrigSironSubField" );
   if ( lTempInteger_1 == 0 )
   { 
      //: TransformSubFields( vSubtask, Wk1, Wk2 )
      o_TransformSubFields( vSubtask, Wk1, Wk2 );
   } 

   //: END

   //: ImportWorkOIToZeidon( vSubtask )
   o_ImportWorkOIToZeidon( vSubtask );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ImportCreateERD( VIEW vWkRecur    BASED ON LOD TZSIWK1O,
//:                 VIEW vERD        BASED ON LOD TZEREMDO,
//:                 VIEW vLPLRDomain BASED ON LOD TZCMLPLO,
//:                 VIEW vSubtask,
//:                 STRING (  9  ) szCatalog,
//:                 SHORT sFirst)

//:   VIEW vDomain BASED ON LOD TZDGSRCO
static zSHORT
o_ImportCreateERD( zVIEW     vWkRecur,
                   zVIEW     vERD,
                   zVIEW     vLPLRDomain,
                   zVIEW     vSubtask,
                   zPCHAR    szCatalog,
                   zSHORT    sFirst )
{
   zVIEW     vDomain = 0; 
   //:VIEW vLPLRDomain2 BASED ON LOD TZCMLPLO
   zVIEW     vLPLRDomain2 = 0; 


   //:STRING (256) MG_Message
   zCHAR     MG_Message[ 257 ] = { 0 }; 
   //:STRING (2)  SironType
   zCHAR     SironType[ 3 ] = { 0 }; 
   //:STRING (33) szEntityName
   zCHAR     szEntityName[ 34 ] = { 0 }; 
   //:INTEGER     nRC
   zLONG     nRC = 0; 
   //:STRING (33) szAttributeName
   zCHAR     szAttributeName[ 34 ] = { 0 }; 
   //:INTEGER     nSthk
   zLONG     nSthk = 0; 
   //:STRING (10) szDecimal
   zCHAR     szDecimal[ 11 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 


   //:// If an ER_Entity currently exists by the same name as the ER_Entity to be
   //:// added, prompt the user whether or not the existing entity should be deleted.
   //:// If OK, delete the entity and all related entities and all related relationships
   //:// for the Katalog entry and continue with the import process.
   //:// If not OK, abort the import process.

   //:szEntityName = vWkRecur.NewEntity.FieldName
   GetVariableFromAttribute( szEntityName, 0, 'S', 34, vWkRecur, "NewEntity", "FieldName", "", 0 );
   //:SET CURSOR FIRST vERD.ER_Entity WHERE
   //:                 vERD.ER_Entity.Name = szEntityName
   RESULT = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szEntityName, "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:IF sFirst = 1
      if ( sFirst == 1 )
      { 
         //:// The prompt will only be done if the actual ER_Entity is the main entity of
         //:// the siron catalog. For all dummy entities (subentities) the delete will
         //:// be done in any case
         //:MG_Message = "An Entity already exists by name: " + szEntityName +
         //:             ". Do you want to DELETE it?"
         ZeidonStringCopy( MG_Message, 1, 0, "An Entity already exists by name: ", 1, 0, 257 );
         ZeidonStringConcat( MG_Message, 1, 0, szEntityName, 1, 0, 257 );
         ZeidonStringConcat( MG_Message, 1, 0, ". Do you want to DELETE it?", 1, 0, 257 );
         //:nRC = MessagePrompt( vSubtask,
         //:                      "SI00105",
         //:                      "Catalog Import",
         //:                      MG_Message,
         //:                      0,
         //:                      zBUTTONS_YESNO,
         //:                      zRESPONSE_YES,
         //:                      zICON_QUESTION  )
         nRC = MessagePrompt( vSubtask, "SI00105", "Catalog Import", MG_Message, 0, zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
         //:IF nRC != zRESPONSE_YES
         if ( nRC != zRESPONSE_YES )
         { 
            //:MessageSend( vSubtask, "SI00106", "Catalog Import",
            //:             "Catalog Import Aborted",
            //:              zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "SI00106", "Catalog Import", "Catalog Import Aborted", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:RETURN -1  // That means, no change to the datamodel !!!
            return( -1 );
         } 

         //:END
      } 

      //:END

      //:// Now delete the entity and all its relationships.
      //:FOR EACH vERD.ER_RelLink
      RESULT = SetCursorFirstEntity( vERD, "ER_RelLink", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: SET CURSOR FIRST vERD.ER_RelType WHERE
         //:                  vERD.ER_RelType.ZKey = vERD.ER_RelType_1.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, vERD, "ER_RelType_1", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelType", "ZKey", lTempInteger_0, "" );
         //: IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //: DELETE ENTITY vERD.ER_RelType NONE
            RESULT = DeleteEntity( vERD, "ER_RelType", zREPOS_NONE );
         } 

         RESULT = SetCursorNextEntity( vERD, "ER_RelLink", "" );
         //: END
      } 

      //:END

      //:DELETE ENTITY vERD.ER_Entity
      RESULT = DeleteEntity( vERD, "ER_Entity", zPOS_NEXT );
   } 


   //:END

   //:// Create Entity and Attributes for a NewEntity
   //:CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER );
   //:szEntityName = vWkRecur.NewEntity.FieldName
   GetVariableFromAttribute( szEntityName, 0, 'S', 34, vWkRecur, "NewEntity", "FieldName", "", 0 );
   //:IF ChangeEntityOrAttributeName( vSubtask, szEntityName ) = -1
   lTempInteger_1 = o_ChangeEntityOrAttributeName( vSubtask, szEntityName );
   if ( lTempInteger_1 == -1 )
   { 
      //:RETURN
      return( 0 );
   } 

   //:END

   //:vERD.ER_Entity.Name = szEntityName
   SetAttributeFromString( vERD, "ER_Entity", "Name", szEntityName );
   //:vERD.ER_Entity.TE_TableName = vWkRecur.NewEntity.FieldName
   SetAttributeFromAttribute( vERD, "ER_Entity", "TE_TableName", vWkRecur, "NewEntity", "FieldName" );

   //:FOR EACH vWkRecur.NewField
   RESULT = SetCursorFirstEntity( vWkRecur, "NewField", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );

      //:szAttributeName = vWkRecur.NewField.FieldName
      GetVariableFromAttribute( szAttributeName, 0, 'S', 34, vWkRecur, "NewField", "FieldName", "", 0 );
      //:// Change all special characters of the attributename to '_'
      //:IF ChangeEntityOrAttributeName( vSubtask, szAttributeName ) = -1
      lTempInteger_2 = o_ChangeEntityOrAttributeName( vSubtask, szAttributeName );
      if ( lTempInteger_2 == -1 )
      { 
         //:RETURN
         return( 0 );
      } 

      //:END

      //:// Put the attributename into the Datamodel
      //:vERD.ER_Attribute.Name = szAttributeName
      SetAttributeFromString( vERD, "ER_Attribute", "Name", szAttributeName );
      //:vERD.ER_Attribute.TE_ColumnName = vWkRecur.NewField.FieldName
      SetAttributeFromAttribute( vERD, "ER_Attribute", "TE_ColumnName", vWkRecur, "NewField", "FieldName" );

      //:vERD.ER_Attribute.Lth  = vWkRecur.NewField.Length
      SetAttributeFromAttribute( vERD, "ER_Attribute", "Lth", vWkRecur, "NewField", "Length" );

      //:// Get number of decimals to set the correct decimal Domain
      //:nSthk = vWkRecur.NewField.Sthk
      GetIntegerFromAttribute( &nSthk, vWkRecur, "NewField", "Sthk" );

      //:CASE nSthk
      switch( nSthk )
      { 
         //:OF 0: szDecimal = "Decimal0"
         case 0 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal0", 1, 0, 11 );
            break ;

         //:OF 1: szDecimal = "Decimal1"
         case 1 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal1", 1, 0, 11 );
            break ;

         //:OF 2: szDecimal = "Decimal2"
         case 2 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal2", 1, 0, 11 );
            break ;

         //:OF 3: szDecimal = "Decimal3"
         case 3 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal3", 1, 0, 11 );
            break ;

         //:OF 4: szDecimal = "Decimal4"
         case 4 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal4", 1, 0, 11 );
            break ;

         //:OF 5: szDecimal = "Decimal5"
         case 5 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal5", 1, 0, 11 );
            break ;

         //:OF 6: szDecimal = "Decimal6"
         case 6 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal6", 1, 0, 11 );
            break ;

         //:OF 7: szDecimal = "Decimal7"
         case 7 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal7", 1, 0, 11 );
            break ;

         //:OF 8: szDecimal = "Decimal8"
         case 8 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal8", 1, 0, 11 );
            break ;

         //:OF 9: szDecimal = "Decimal9"
         case 9 :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal9", 1, 0, 11 );
            break ;

         //:OTHERWISE: szDecimal = "Decimal"
         default  :
            ZeidonStringCopy( szDecimal, 1, 0, "Decimal", 1, 0, 11 );
            break ;
      } 

      //:END


      //:// Set Domain based on Siron Type
      //:SironType = vWkRecur.NewField.Attribut
      GetVariableFromAttribute( SironType, 0, 'S', 3, vWkRecur, "NewField", "Attribut", "", 0 );
      //:IF SironType = "D" OR SironType = "O" OR SironType = "FE" OR SironType = "FD"
      //:    OR SironType = "DW"
      if ( ZeidonStringCompare( SironType, 1, 0, "D", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 0, "O", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 0, "FE", 1, 0, 3 ) == 0 ||
           ZeidonStringCompare( SironType, 1, 0, "FD", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 0, "DW", 1, 0, 3 ) == 0 )
      { 
         //:SET CURSOR FIRST vLPLRDomain.W_MetaDef WHERE
         //:                                  vLPLRDomain.W_MetaDef.Name = szDecimal
         RESULT = SetCursorFirstEntityByString( vLPLRDomain, "W_MetaDef", "Name", szDecimal, "" );
         //:ELSE
      } 
      else
      { 
         //:IF SironType = "FL" OR SironType = "U" OR SironType = "V" OR SironType = "FX"
         if ( ZeidonStringCompare( SironType, 1, 0, "FL", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 0, "U", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 0, "V", 1, 0, 3 ) == 0 ||
              ZeidonStringCompare( SironType, 1, 0, "FX", 1, 0, 3 ) == 0 )
         { 
            //:SET CURSOR FIRST vLPLRDomain.W_MetaDef WHERE
            //:                               vLPLRDomain.W_MetaDef.Name = szDecimal
            RESULT = SetCursorFirstEntityByString( vLPLRDomain, "W_MetaDef", "Name", szDecimal, "" );
            //:ELSE
         } 
         else
         { 
            //:IF SironType = "F" OR SironType = "H" OR SironType[1:1] = "B"
            if ( ZeidonStringCompare( SironType, 1, 0, "F", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 0, "H", 1, 0, 3 ) == 0 || ZeidonStringCompare( SironType, 1, 1, "B", 1, 0, 3 ) == 0 )
            { 
               //:SET CURSOR FIRST vLPLRDomain.W_MetaDef WHERE
               //:                            vLPLRDomain.W_MetaDef.Name = "Integer"
               RESULT = SetCursorFirstEntityByString( vLPLRDomain, "W_MetaDef", "Name", "Integer", "" );
               //:ELSE
            } 
            else
            { 
               //:IF SironType = "DT"
               if ( ZeidonStringCompare( SironType, 1, 0, "DT", 1, 0, 3 ) == 0 )
               { 
                  //:SET CURSOR FIRST vLPLRDomain.W_MetaDef WHERE
                  //:                         vLPLRDomain.W_MetaDef.Name = "DateTime"
                  RESULT = SetCursorFirstEntityByString( vLPLRDomain, "W_MetaDef", "Name", "DateTime", "" );
                  //:ELSE
               } 
               else
               { 
                  //:SET CURSOR FIRST vLPLRDomain.W_MetaDef WHERE
                  //:                         vLPLRDomain.W_MetaDef.Name = "Text"
                  RESULT = SetCursorFirstEntityByString( vLPLRDomain, "W_MetaDef", "Name", "Text", "" );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:CreateViewFromViewForTask( vLPLRDomain2, vLPLRDomain, vSubtask )
      CreateViewFromViewForTask( &vLPLRDomain2, vLPLRDomain, vSubtask );
      //:nRC = ActivateMetaOI( vSubtask, vDomain, vLPLRDomain2, zREFER_DOMAIN_META, 0 )
      nRC = ActivateMetaOI( vSubtask, &vDomain, vLPLRDomain2, zREFER_DOMAIN_META, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:MG_Message = "Siron Data Type: " + SironType + "does not have Domain"
         ZeidonStringCopy( MG_Message, 1, 0, "Siron Data Type: ", 1, 0, 257 );
         ZeidonStringConcat( MG_Message, 1, 0, SironType, 1, 0, 257 );
         ZeidonStringConcat( MG_Message, 1, 0, "does not have Domain", 1, 0, 257 );
         //:MessageSend( vSubtask, "SI00104", "Catalog Import",
         //:             MG_Message,
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( vSubtask, "SI00104", "Catalog Import", MG_Message, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:ELSE
      } 
      else
      { 
         //:INCLUDE vERD.Domain FROM vDomain.Domain
         RESULT = IncludeSubobjectFromSubobject( vERD, "Domain", vDomain, "Domain", zPOS_AFTER );
         //:DropMetaOI( vSubtask, vDomain )
         DropMetaOI( vSubtask, vDomain );
      } 

      //:END

      //:// If the Attribute is a primary key in the Katalog, create an Identifier
      //:// for the Attribute.
      //:IF vWkRecur.NewField.KeyCode = "PK"
      if ( CompareAttributeToString( vWkRecur, "NewField", "KeyCode", "PK" ) == 0 )
      { 
         //:// Create an entity 'Identifier' only if it not exists
         //:// All attributes which belong to the key should be members
         //:// of the same Identifier
         //:IF  vERD.ER_EntIdentifier DOES not EXIST
         lTempInteger_3 = CheckExistenceOfEntity( vERD, "ER_EntIdentifier" );
         if ( lTempInteger_3 != 0 )
         { 
            //:CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER )
            CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER );
            //:vERD.ER_EntIdentifier.Name = vERD.ER_Attribute.Name
            SetAttributeFromAttribute( vERD, "ER_EntIdentifier", "Name", vERD, "ER_Attribute", "Name" );
            //:vERD.ER_EntIdentifier.Desc = "Generated from Siron PK"
            SetAttributeFromString( vERD, "ER_EntIdentifier", "Desc", "Generated from Siron PK" );
         } 

         //:END
         //:// Set 'NotNull' field for all Key-attributes
         //:SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y")
         SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
         //:CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER );
         //:INCLUDE vERD.ER_AttributeIdentifier FROM vERD.ER_Attribute
         RESULT = IncludeSubobjectFromSubobject( vERD, "ER_AttributeIdentifier", vERD, "ER_Attribute", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( vWkRecur, "NewField", "" );
      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ImportERD_Recursive( VIEW vWkRecur    BASED ON LOD TZSIWK1O,
//:                     VIEW vWkMain     BASED ON LOD TZSIWK1O,
//:                     VIEW vERD        BASED ON LOD TZEREMDO,
//:                     VIEW vLPLRDomain BASED ON LOD TZCMLPLO,
//:                     VIEW vSubtask )

//:   VIEW vWkHigh   BASED ON LOD TZSIWK1O
static zVOID
o_ImportERD_Recursive( zVIEW     vWkRecur,
                       zVIEW     vWkMain,
                       zVIEW     vERD,
                       zVIEW     vLPLRDomain,
                       zVIEW     vSubtask )
{
   zVIEW     vWkHigh = 0; 
   //:VIEW vWkDim    BASED ON LOD TZSIWK1O
   zVIEW     vWkDim = 0; 

   //:VIEW vERD2     BASED ON LOD TZEREMDO
   zVIEW     vERD2 = 0; 

   //:INTEGER lZKey
   zLONG     lZKey = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:STRING (10) szDimension
   zCHAR     szDimension[ 11 ] = { 0 }; 
   //:STRING ( 9) szCatalog
   zCHAR     szCatalog[ 10 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 


   //:// Keep one view at the higher level.
   //:CreateViewFromViewForTask( vWkHigh, vWkRecur, vSubtask )
   CreateViewFromViewForTask( &vWkHigh, vWkRecur, vSubtask );
   //:NAME VIEW  vWkHigh  "vWkHigh"
   SetNameForView( vWkHigh, "vWkHigh", 0, zLEVEL_TASK );

   //:// Get Dimension of Siron Subfield
   //:CreateViewFromViewForTask( vWkDim, vWkRecur, vSubtask )
   CreateViewFromViewForTask( &vWkDim, vWkRecur, vSubtask );
   //:NAME VIEW  vWkDim  "vWkDim"
   SetNameForView( vWkDim, "vWkDim", 0, zLEVEL_TASK );

   //:SET CURSOR  FIRST vWkDim.OrigSironSubField
   //:    WHERE vWkDim.OrigSironSubField.FieldName = vWkRecur.HD_ChildNode.Name
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vWkRecur, "HD_ChildNode", "Name" );
   RESULT = SetCursorFirstEntityByString( vWkDim, "OrigSironSubField", "FieldName", szTempString_0, "" );
   //:GetStringFromAttribute( szDimension, vWkDim, "OrigSironSubField", "Dimension" )
   GetStringFromAttribute( szDimension, zsizeof( szDimension ), vWkDim, "OrigSironSubField", "Dimension" );
   //:DropView( vWkDim )
   DropView( vWkDim );

   //:// Save the catalogname to set it into fiele TableName in ERD
   //:szCatalog = vWkRecur.NewEntity.FieldName
   GetVariableFromAttribute( szCatalog, 0, 'S', 10, vWkRecur, "NewEntity", "FieldName", "", 0 );
   //:SetViewToSubobject( vWkRecur, "HD_ChildNode" )
   SetViewToSubobject( vWkRecur, "HD_ChildNode" );

   //:// Go to create Entity and Table entries, with subordinates, for
   //:// the subobject entry.
   //:nRC = ImportCreateERD( vWkRecur, vERD, vLPLRDomain, vSubtask, szCatalog, 0)
   nRC = o_ImportCreateERD( vWkRecur, vERD, vLPLRDomain, vSubtask, szCatalog, 0 );
   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:// Save the ZKey for later positioning.
      //:lZKey = vERD.ER_Entity.ZKey
      GetIntegerFromAttribute( &lZKey, vERD, "ER_Entity", "ZKey" );
      //:// Entity type is attributive.
      //:vERD.ER_Entity.Purpose = "A"
      SetAttributeFromString( vERD, "ER_Entity", "Purpose", "A" );

      //:// Stagger the X and Y positions.
      //:CreateViewFromViewForTask( vERD2, vERD, vSubtask )
      CreateViewFromViewForTask( &vERD2, vERD, vSubtask );
      //:SET CURSOR PREVIOUS vERD2.ER_Entity
      RESULT = SetCursorPrevEntity( vERD2, "ER_Entity", "" );
      //:vERD.ER_Entity.ER_DiagramPosX = vERD2.ER_Entity.ER_DiagramPosX + 11
      GetIntegerFromAttribute( &lTempInteger_0, vERD2, "ER_Entity", "ER_DiagramPosX" );
      lTempInteger_1 = lTempInteger_0 + 11;
      SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", lTempInteger_1 );
      //:vERD.ER_Entity.ER_DiagramPosY = vERD2.ER_Entity.ER_DiagramPosY + 4
      GetIntegerFromAttribute( &lTempInteger_2, vERD2, "ER_Entity", "ER_DiagramPosY" );
      lTempInteger_3 = lTempInteger_2 + 4;
      SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lTempInteger_3 );
      //:DropView( vERD2 )
      DropView( vERD2 );

      //:// Create the Relationship entry.

      //:SET CURSOR LAST vERD.ER_RelType
      RESULT = SetCursorLastEntity( vERD, "ER_RelType", "" );
      //:CreateMetaEntity( vSubtask, vERD, "ER_RelType", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vERD, "ER_RelType", zPOS_AFTER );
      //:vERD.ER_RelType.Fact = "Create by Siron import"
      SetAttributeFromString( vERD, "ER_RelType", "Fact", "Create by Siron import" );

      //:CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER );
      //:vERD.ER_RelLink_2.Name    = "has"
      SetAttributeFromString( vERD, "ER_RelLink_2", "Name", "has" );
      //:vERD.ER_RelLink_2.CardMin = "0"
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "0" );
      //:vERD.ER_RelLink_2.CardMax = szDimension
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", szDimension );
      //:SET CURSOR FIRST vERD.ER_Entity WHERE
      //:    vERD.ER_Entity.Name = vWkHigh.NewEntity.FieldName
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vWkHigh, "NewEntity", "FieldName" );
      RESULT = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szTempString_0, "" );
      //:INCLUDE vERD.ER_Entity_2 FROM vERD.ER_Entity
      RESULT = IncludeSubobjectFromSubobject( vERD, "ER_Entity_2", vERD, "ER_Entity", zPOS_AFTER );

      //:// Use a new view to save the relink path for the include under ER_Entity.
      //:CreateViewFromViewForTask( vERD2, vERD, vSubtask )
      CreateViewFromViewForTask( &vERD2, vERD, vSubtask );

      //:// Reset to last entity created
      //:SET CURSOR FIRST vERD.ER_Entity WHERE vERD.ER_Entity.ZKey = lZKey
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Entity", "ZKey", lZKey, "" );

      //:CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER );
      //:vERD.ER_RelLink_2.Name    = "for"
      SetAttributeFromString( vERD, "ER_RelLink_2", "Name", "for" );
      //:vERD.ER_RelLink_2.CardMin = "1"
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "1" );
      //:vERD.ER_RelLink_2.CardMax = "1"
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "1" );
      //:INCLUDE vERD.ER_Entity_2 FROM vERD.ER_Entity
      RESULT = IncludeSubobjectFromSubobject( vERD, "ER_Entity_2", vERD, "ER_Entity", zPOS_AFTER );

      //:// Now create the whole relationship path under ER_Entity.
      //:SET CURSOR FIRST vERD.ER_RelLink WITHIN vERD.EntpER_Model WHERE
      //:   vERD.ER_RelLink.ZKey = vERD.ER_RelLink_2.ZKey
      GetIntegerFromAttribute( &lTempInteger_4, vERD, "ER_RelLink_2", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelLink", "ZKey", lTempInteger_4, "EntpER_Model" );
      //:INCLUDE vERD.ER_RelLink_Other FROM vERD2.ER_RelLink_2
      RESULT = IncludeSubobjectFromSubobject( vERD, "ER_RelLink_Other", vERD2, "ER_RelLink_2", zPOS_AFTER );
      //:SET CURSOR FIRST vERD.ER_RelLink WITHIN vERD.EntpER_Model WHERE
      //:   vERD.ER_RelLink.ZKey = vERD2.ER_RelLink_2.ZKey
      GetIntegerFromAttribute( &lTempInteger_5, vERD2, "ER_RelLink_2", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelLink", "ZKey", lTempInteger_5, "EntpER_Model" );
      //:INCLUDE vERD.ER_RelLink_Other FROM vERD.ER_RelLink_2
      RESULT = IncludeSubobjectFromSubobject( vERD, "ER_RelLink_Other", vERD, "ER_RelLink_2", zPOS_AFTER );

      //:// Reset to last entity created
      //:SET CURSOR LAST vERD.ER_Entity
      RESULT = SetCursorLastEntity( vERD, "ER_Entity", "" );

      //:DropView( vERD2 )
      DropView( vERD2 );

      //:FOR EACH vWkRecur.HD_ChildNode
      RESULT = SetCursorFirstEntity( vWkRecur, "HD_ChildNode", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:ImportERD_Recursive( vWkRecur, vWkMain, vERD, vLPLRDomain, vSubtask )
         o_ImportERD_Recursive( vWkRecur, vWkMain, vERD, vLPLRDomain, vSubtask );
         RESULT = SetCursorNextEntity( vWkRecur, "HD_ChildNode", "" );
      } 

      //:END
   } 


   //:END

   //:DropView( vWkHigh )
   DropView( vWkHigh );
   //:ResetViewFromSubobject( vWkRecur )
   ResetViewFromSubobject( vWkRecur );
   return;
// END
} 


//:DIALOG OPERATION
//:ImportWorkOIToZeidon( VIEW vSubtask )

//:// This routine transforms the TZSIWK1O work object into entities, relationships
//:// attributes in the ER and into tables, fields and Siron entitie in the TE.

//:// It uses Configuration Management to retrieve Zeidon meta objects.

//:   VIEW vWkMain     BASED ON LOD TZSIWK1O
static zSHORT
o_ImportWorkOIToZeidon( zVIEW     vSubtask )
{
   zVIEW     vWkMain = 0; 
   //:VIEW vWkRecur    BASED ON LOD TZSIWK1O
   zVIEW     vWkRecur = 0; 
   //:VIEW vWkRecur2   BASED ON LOD TZSIWK1O
   zVIEW     vWkRecur2 = 0; 
   //:VIEW vLPLRDomain BASED ON LOD TZCMLPLO
   zVIEW     vLPLRDomain = 0; 
   //:VIEW vERD        BASED ON LOD TZEREMDO
   zVIEW     vERD = 0; 
   //:VIEW vERD_Window
   zVIEW     vERD_Window = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER PosY
   zLONG     PosY = 0; 
   //:INTEGER TempPosY
   zLONG     TempPosY = 0; 
   //:INTEGER nContinueFlag
   zLONG     nContinueFlag = 0; 
   //:SHORT   nLevel
   zSHORT    nLevel = 0; 
   //:STRING (32) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING (128) szMsg
   zCHAR     szMsg[ 129 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 34 ]; 
   zLONG     lTempInteger_1; 


   //:// Get the recursive view we want for navigating the work object.
   //:// Don't use the main one, least we mess up its position.
   //:// Also, go to the top of the structure.
   //:GET VIEW vWkMain NAMED "TZSIWK1O"
   RESULT = GetViewByName( &vWkMain, "TZSIWK1O", vSubtask, zLEVEL_TASK );
   //:NAME VIEW  vWkMain  "vWKMain"
   SetNameForView( vWkMain, "vWKMain", 0, zLEVEL_TASK );
   //:CreateViewFromViewForTask( vWkRecur, vWkMain, vSubtask )
   CreateViewFromViewForTask( &vWkRecur, vWkMain, vSubtask );
   //:NAME VIEW  vWkRecur  "vWkRecur"
   SetNameForView( vWkRecur, "vWkRecur", 0, zLEVEL_TASK );
   //:LOOP WHILE vWkRecur.NewEntity.KatalogFlag != "Y"
   while ( CompareAttributeToString( vWkRecur, "NewEntity", "KatalogFlag", "Y" ) != 0 )
   { 
      //:ResetViewFromSubobject( vWkRecur )
      ResetViewFromSubobject( vWkRecur );
   } 

   //:END

   //:// Check to see if any entities do not have attributes contained
   //:// within them.  If so, prompt the operator whether to continue or
   //:// abort.  We will do this by navigating the work object hierarchically,
   //:// examining each HD_Node entity to see if it has any NewField entities.
   //:nContinueFlag = 0
   nContinueFlag = 0;
   //:CreateViewFromViewForTask( vWkRecur2, vWkMain, vSubtask )
   CreateViewFromViewForTask( &vWkRecur2, vWkMain, vSubtask );
   //:SET CURSOR FIRST vWkRecur2.HD_Diagram
   RESULT = SetCursorFirstEntity( vWkRecur2, "HD_Diagram", "" );
   //:DefineHierarchicalCursor( vWkRecur2, "HD_Diagram" )
   DefineHierarchicalCursor( vWkRecur2, "HD_Diagram" );
   //:nRC = SetCursorNextEntityHierarchical( nLevel, szEntityName, vWkRecur2 )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &nLevel, szEntityName, vWkRecur2 );
   //:LOOP WHILE  nRC > zCURSOR_UNCHANGED
   while ( nRC > zCURSOR_UNCHANGED )
   { 
      //:IF  nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( vWkRecur2, "HD_ChildNode" )
         SetViewToSubobject( vWkRecur2, "HD_ChildNode" );
      } 

      //:END
      //:IF szEntityName = "HD_Node"
      if ( ZeidonStringCompare( szEntityName, 1, 0, "HD_Node", 1, 0, 33 ) == 0 )
      { 
         //:IF vWkRecur2.NewField DOES NOT EXIST
         lTempInteger_0 = CheckExistenceOfEntity( vWkRecur2, "NewField" );
         if ( lTempInteger_0 != 0 )
         { 
            //:szMsg = "Entity: " + vWkRecur2.HD_Node.Name +
            //:        ", will not have attributes. Do you want to import this Catalog?"
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vWkRecur2, "HD_Node", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Entity: ", 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, ", will not have attributes. Do you want to import this Catalog?", 1, 0, 129 );
            //:nRC = MessagePrompt( vSubtask,
            //:                      "SI00108",
            //:                      "Catalog Import",
            //:                      szMsg,
            //:                      0,
            //:                      zBUTTONS_YESNO,
            //:                      zRESPONSE_YES,
            //:                      zICON_QUESTION  )
            nRC = MessagePrompt( vSubtask, "SI00108", "Catalog Import", szMsg, 0, zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
            //:IF nRC = zRESPONSE_NO
            if ( nRC == zRESPONSE_NO )
            { 
               //:nContinueFlag = 1   // Set flag to abort.
               nContinueFlag = 1;
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( nLevel, szEntityName, vWkRecur2 )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &nLevel, szEntityName, vWkRecur2 );
   } 

   //:END
   //:DropHierarchicalCursor( vWkRecur2 )
   DropHierarchicalCursor( vWkRecur2 );
   //:DropView( vWkRecur2 )
   DropView( vWkRecur2 );
   //:IF nContinueFlag != 0
   if ( nContinueFlag != 0 )
   { 
      //:RETURN 0
      return( 0 );
   } 

   //:END

   //://************************************************************
   //://
   //://        Import ERD data
   //://
   //://************************************************************

   //:// Get the LPLR view for accessing domains
   //:RetrieveViewForMetaList( vSubtask, vLPLRDomain, zREFER_DOMAIN_META )
   RetrieveViewForMetaList( vSubtask, &vLPLRDomain, zREFER_DOMAIN_META );
   //:NAME VIEW vLPLRDomain "LPLRDomain"
   SetNameForView( vLPLRDomain, "LPLRDomain", 0, zLEVEL_TASK );

   //:// Use the current ERD view.
   //:GET VIEW vERD NAMED  "TZEREMDO"
   RESULT = GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   //:// Search for the largest PosY value for positioning entities.
   //:PosY = 0
   PosY = 0;
   //:FOR EACH vERD.ER_Entity
   RESULT = SetCursorFirstEntity( vERD, "ER_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TempPosY = vERD.ER_Entity.ER_DiagramPosY
      GetIntegerFromAttribute( &TempPosY, vERD, "ER_Entity", "ER_DiagramPosY" );
      //:IF TempPosY > PosY
      if ( TempPosY > PosY )
      { 
         //:PosY = TempPosY
         PosY = TempPosY;
      } 

      RESULT = SetCursorNextEntity( vERD, "ER_Entity", "" );
      //:END
   } 

   //:END

   //:// Position at end of ER_Entities.
   //:SET CURSOR LAST vERD.ER_Entity
   RESULT = SetCursorLastEntity( vERD, "ER_Entity", "" );

   //:// Create ERD entries for the main Katalog entry.
   //:nRC = ImportCreateERD( vWkRecur, vERD, vLPLRDomain, vSubtask,
   //:                        vWkRecur.NewEntity.FieldName, 1 )
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vWkRecur, "NewEntity", "FieldName" );
   nRC = o_ImportCreateERD( vWkRecur, vERD, vLPLRDomain, vSubtask, szTempString_1, 1 );
   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:// Entity type is fundamental.
      //:vERD.ER_Entity.Purpose = "F"
      SetAttributeFromString( vERD, "ER_Entity", "Purpose", "F" );
      //:// Y position is 9 greater than largest Y.
      //:// X position starts at 2.
      //:vERD.ER_Entity.ER_DiagramPosY = PosY + 9
      lTempInteger_1 = PosY + 9;
      SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lTempInteger_1 );
      //:vERD.ER_Entity.ER_DiagramPosX = 2
      SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", 2 );

      //:// Create the ERD entries for each subordinate Katalog entry.
      //:FOR EACH vWkRecur.HD_ChildNode
      RESULT = SetCursorFirstEntity( vWkRecur, "HD_ChildNode", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:ImportERD_Recursive( vWkRecur, vWkMain, vERD, vLPLRDomain, vSubtask )
         o_ImportERD_Recursive( vWkRecur, vWkMain, vERD, vLPLRDomain, vSubtask );
         RESULT = SetCursorNextEntity( vWkRecur, "HD_ChildNode", "" );
      } 

      //:END

      //:// Prompt the operator to make sure he knows we are going to commit the
      //:// ERD and give him the chance to abort.
      //:nRC = MessagePrompt( vSubtask,
      //:                      "SI00107",
      //:                      "Catalog Import",
      //:                      "Import will save the ERD. Do you want to continue?",
      //:                      0,
      //:                      zBUTTONS_YESNO,
      //:                      zRESPONSE_YES,
      //:                      zICON_QUESTION  )
      nRC = MessagePrompt( vSubtask, "SI00107", "Catalog Import", "Import will save the ERD. Do you want to continue?", 0, zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
      //:IF nRC = zRESPONSE_NO
      if ( nRC == zRESPONSE_NO )
      { 
         //:RETURN
         return( 0 );
      } 

      //:END

      //:// Commit the ERD.
      //:CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META )
      CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META );

      //:// if attribute sequence does not exist, set it and remove update flag
      //:SetAttributeSequence( vERD )
      oTZEREMDO_SetAttributeSequence( vERD );

      //:// Now make sure the stuff we have just added shows up.
      //:GET VIEW vERD_Window NAMED "TZEREMDD"
      RESULT = GetViewByName( &vERD_Window, "TZEREMDD", vSubtask, zLEVEL_TASK );
      //:RefreshDiagram( vERD_Window, "ERD", 0 )
      RefreshDiagram( vERD_Window, "ERD", 0 );
      //:RefreshAllSubwindows( vERD_Window )
      RefreshAllSubwindows( vERD_Window );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DeleteWorkObject( VIEW vSubtask )

//:    VIEW WkList BASED ON LOD TZSIIMLO
zOPER_EXPORT zSHORT OPERATION
DeleteWorkObject( zVIEW     vSubtask )
{
   zVIEW     WkList = 0; 
   //: SHORT       nRC
   zSHORT    nRC = 0; 
   //: STRING (76) FileName
   zCHAR     FileName[ 77 ] = { 0 }; 
   //: STRING (16) KatalogName
   zCHAR     KatalogName[ 17 ] = { 0 }; 
   //: STRING (64) DirectoryPath
   zCHAR     DirectoryPath[ 65 ] = { 0 }; 
   //: STRING (256) MsgText
   zCHAR     MsgText[ 257 ] = { 0 }; 
   zSHORT    RESULT; 


   //: // Get the directory path where we will find the import work objects.
   //: nRC = GetSironWorkObjPathName( DirectoryPath, 64 )
   nRC = GetSironWorkObjPathName( DirectoryPath, 64 );

   //: IF nRC < 0
   if ( nRC < 0 )
   { 
      //: MessageSend( vSubtask, "SI00106", "Catalog Import",
      //:              "Cannot find directory path.",
      //:              zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "SI00106", "Catalog Import", "Cannot find directory path.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //: RETURN
      return( 0 );
   } 

   //: END

   //: // Use the work object list to select the work object to delete.
   //: // Just concatenate the directory path to the name from the list, plus a suffix.
   //: GET VIEW WkList NAMED "TZSIIMLO"
   RESULT = GetViewByName( &WkList, "TZSIIMLO", vSubtask, zLEVEL_TASK );
   //: KatalogName = WkList.KatalogFile.Name
   GetVariableFromAttribute( KatalogName, 0, 'S', 17, WkList, "KatalogFile", "Name", "", 0 );
   //: FileName = DirectoryPath + KatalogName + ".WSK"
   ZeidonStringCopy( FileName, 1, 0, DirectoryPath, 1, 0, 77 );
   ZeidonStringConcat( FileName, 1, 0, KatalogName, 1, 0, 77 );
   ZeidonStringConcat( FileName, 1, 0, ".WSK", 1, 0, 77 );

   //: MsgText = "Do you really want to delete " + FileName
   ZeidonStringCopy( MsgText, 1, 0, "Do you really want to delete ", 1, 0, 257 );
   ZeidonStringConcat( MsgText, 1, 0, FileName, 1, 0, 257 );
   //: nRC = MessagePrompt( vSubtask, "SI00109", "Catalog Import",
   //:                      MsgText, 1, zBUTTONS_YESNO, zRESPONSE_NO,
   //:                      zICON_QUESTION )
   nRC = MessagePrompt( vSubtask, "SI00109", "Catalog Import", MsgText, 1, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   //: IF  nRC = zRESPONSE_YES
   if ( nRC == zRESPONSE_YES )
   { 
      //:  zDeleteFile( FileName )
      zDeleteFile( FileName );
      //:  // Now delete the entry from the list.
      //:  DELETE ENTITY WkList.KatalogFile
      RESULT = DeleteEntity( WkList, "KatalogFile", zPOS_NEXT );
   } 

   //: END

   //: // If the last WSK-File is deleted, disable buttons: OK and Delete
   //: EnableButtons( vSubtask )
   EnableButtons( vSubtask );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CreateKatalogList( VIEW vSubtask )

//:   SHORT       nRC
zOPER_EXPORT zSHORT OPERATION
CreateKatalogList( zVIEW     vSubtask )
{
   zSHORT    nRC = 0; 
   //:STRING (64) DirectoryPath
   zCHAR     DirectoryPath[ 65 ] = { 0 }; 
   //:VIEW vERD   BASED ON LOD TZEREMDO
   zVIEW     vERD = 0; 
   zSHORT    RESULT; 


   //:// Hier erstmal vERD laden
   //:GET VIEW vERD NAMED "TZEREMDO"
   RESULT = GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:SironCreateDataModel( vSubtask, vERD )
      SironCreateDataModel( vSubtask, &vERD );
   } 

   //:END

   //:nRC = LoadSironDLLs( vSubtask )
   nRC = LoadSironDLLs( vSubtask );
   //:IF nRC != 0
   if ( nRC != 0 )
   { 
      //:ShowMessage( vSubtask, nRC )
      o_ShowMessage( vSubtask, nRC );
      //:SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Sort Attributes in original sequence
   //:SortAttributes( vERD )
   oTZEREMDO_SortAttributes( vERD );

   //:// Create the list object made up of all import work objects in the directory.
   //:// Then transfer to a list window, allowing the operator to select one for import
   //:// processing.

   //:// Get the directory path where we will find the import work objects.
   //:nRC = GetSironWorkObjPathName( DirectoryPath, 64 )
   nRC = GetSironWorkObjPathName( DirectoryPath, 64 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask, "SI00106", "Catalog Import",
      //:             "Cannot find directory path.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "SI00106", "Catalog Import", "Cannot find directory path.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Use Windows functionality to generate the list.
   //:// The work list is given external name "TZSIIMLO"
   //:CreateSironWorkObjectList( DirectoryPath, vSubtask )
   CreateSironWorkObjectList( DirectoryPath, vSubtask );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:TransformSubFields( VIEW vSubtask,
//:                    VIEW Wk1 BASED ON LOD TZSIWK1O,
//:                    VIEW Wk2 BASED ON LOD TZSIWK1O )

//:   VIEW Wk3 BASED ON LOD TZSIWK1O
static zVOID
o_TransformSubFields( zVIEW     vSubtask,
                      zVIEW     Wk1,
                      zVIEW     Wk2 )
{
   zVIEW     Wk3 = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //:// Transform OrigSironSubFields to
   //:SetViewToSubobject( Wk1, "OrigSironSubField"  )
   SetViewToSubobject( Wk1, "OrigSironSubField" );
   //:SetViewToSubobject( Wk2, "HD_ChildNode"  )
   SetViewToSubobject( Wk2, "HD_ChildNode" );
   //:// If a sub field exists, create new entities from original fields.
   //:// If a sub field doesn't exist, create new fields from original fields.
   //:FOR EACH Wk1.OrigSironField
   RESULT = SetCursorFirstEntity( Wk1, "OrigSironField", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:IF Wk1.OrigSironSubField EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( Wk1, "OrigSironSubField" );
      if ( lTempInteger_0 == 0 )
      { 
         //:CreateMetaEntity( vSubtask, Wk2, "HD_Node", zPOS_AFTER )
         CreateMetaEntity( vSubtask, Wk2, "HD_Node", zPOS_AFTER );
         //: Wk2.HD_Node.Name = Wk1.OrigSironField.FieldName
         SetAttributeFromAttribute( Wk2, "HD_Node", "Name", Wk1, "OrigSironField", "FieldName" );
         //: Wk2.HD_Node.Text = Wk1.OrigSironField.FieldName
         SetAttributeFromAttribute( Wk2, "HD_Node", "Text", Wk1, "OrigSironField", "FieldName" );
         //: Wk2.HD_Node.Color = 1
         SetAttributeFromInteger( Wk2, "HD_Node", "Color", 1 );

         //:CreateMetaEntity( vSubtask, Wk2, "NewEntity", zPOS_AFTER )
         CreateMetaEntity( vSubtask, Wk2, "NewEntity", zPOS_AFTER );
         //: SetMatchingAttributesByName( Wk2, "NewEntity", Wk1, "OrigSironField", zSET_NULL )
         SetMatchingAttributesByName( Wk2, "NewEntity", Wk1, "OrigSironField", zSET_NULL );

         //:// Execute recursive subroutine to transform sub fields.
         //:IF Wk1.OrigSironSubField EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( Wk1, "OrigSironSubField" );
         if ( lTempInteger_1 == 0 )
         { 
            //:TransformSubFields( vSubtask, Wk1, Wk2 )
            o_TransformSubFields( vSubtask, Wk1, Wk2 );
         } 

         //:END

         //:ELSE
      } 
      else
      { 
         //:// Reset view back to last NewEntity
         //:CreateViewFromViewForTask( Wk3, Wk2, vSubtask )
         CreateViewFromViewForTask( &Wk3, Wk2, vSubtask );
         //:ResetViewFromSubobject( Wk3 )
         ResetViewFromSubobject( Wk3 );
         //:SET CURSOR LAST Wk3.NewField
         RESULT = SetCursorLastEntity( Wk3, "NewField", "" );
         //:CreateMetaEntity( vSubtask, Wk3, "NewField", zPOS_AFTER )
         CreateMetaEntity( vSubtask, Wk3, "NewField", zPOS_AFTER );
         //:SetMatchingAttributesByName( Wk3, "NewField", Wk1, "OrigSironField", zSET_NULL )
         SetMatchingAttributesByName( Wk3, "NewField", Wk1, "OrigSironField", zSET_NULL );
         //:DropView( Wk3 )
         DropView( Wk3 );
      } 

      RESULT = SetCursorNextEntity( Wk1, "OrigSironField", "" );

      //:END
   } 


   //:END

   //:ResetViewFromSubobject( Wk1 )
   ResetViewFromSubobject( Wk1 );
   //:ResetViewFromSubobject( Wk2 )
   ResetViewFromSubobject( Wk2 );
   return;
// END
} 


//:DIALOG OPERATION
//:LoadServerList( VIEW vSubtask )

//:   VIEW TZSIIMWK BASED ON LOD TZSIIMWK
zOPER_EXPORT zSHORT OPERATION
LoadServerList( zVIEW     vSubtask )
{
   zVIEW     TZSIIMWK = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:ACTIVATE TZSIIMWK MULTIPLE EMPTY
   RESULT = ActivateEmptyObjectInstance( &TZSIIMWK, "TZSIIMWK", vSubtask, zMULTIPLE );
   //:NAME VIEW TZSIIMWK "TZSIIMWK"
   SetNameForView( TZSIIMWK, "TZSIIMWK", 0, zLEVEL_TASK );

   //:nRC = GetAllServerNames( vSubtask, TZSIIMWK )
   nRC = GetAllServerNames( vSubtask, TZSIIMWK );

   //:IF nRC != 0
   if ( nRC != 0 )
   { 
      //: ShowMessage( vSubtask, nRC )
      o_ShowMessage( vSubtask, nRC );
      //:ELSE
   } 
   else
   { 
      //: OrderEntityForView( TZSIIMWK, "TZSIIMWK", "wkServerName A" )
      OrderEntityForView( TZSIIMWK, "TZSIIMWK", "wkServerName A" );
      //: SET CURSOR  FIRST TZSIIMWK.TZSIIMWK
      RESULT = SetCursorFirstEntity( TZSIIMWK, "TZSIIMWK", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ShowMessage( VIEW vSubtask, SHORT MessageNumber )

//:  STRING ( 256 ) szMessage
static zSHORT
o_ShowMessage( zVIEW     vSubtask,
               zSHORT    MessageNumber )
{
   zCHAR     szMessage[ 257 ] = { 0 }; 


   //:CASE  MessageNumber
   switch( MessageNumber )
   { 

      //:OF -16: szMessage = "Internal error !"
      case -16 :
         ZeidonStringCopy( szMessage, 1, 0, "Internal error !", 1, 0, 257 );
         break ;

      //:OF -3 : szMessage = "Function GetComputername failed !"
      case -3 :
         ZeidonStringCopy( szMessage, 1, 0, "Function GetComputername failed !", 1, 0, 257 );
         break ;

      //:OF 11 : szMessage = "Invalid Environment Handle !"
      case 11 :
         ZeidonStringCopy( szMessage, 1, 0, "Invalid Environment Handle !", 1, 0, 257 );
         break ;

      //:OF 21 : szMessage = "No memory for loading SIRENV !"
      case 21 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for loading SIRENV !", 1, 0, 257 );
         break ;

      //:OF 22 : szMessage = "File SIRENV not found !"
      case 22 :
         ZeidonStringCopy( szMessage, 1, 0, "File SIRENV not found !", 1, 0, 257 );
         break ;

      //:OF 100: szMessage = "Syntax error in SIRENV !"
      case 100 :
         ZeidonStringCopy( szMessage, 1, 0, "Syntax error in SIRENV !", 1, 0, 257 );
         break ;

      //:OF 500: szMessage = "HZVAFFZ.DLL cannot be loaded !"
      case 500 :
         ZeidonStringCopy( szMessage, 1, 0, "HZVAFFZ.DLL cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 501: szMessage = "Function hzvaOpenEnv cannot be loaded !"
      case 501 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaOpenEnv cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 502: szMessage = "Function hzvaFreeEnv cannot be loaded !"
      case 502 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaFreeEnv cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 503: szMessage = "Function hzvaGetAllServerNames cannot be loaded !"
      case 503 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetAllServerNames cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 504: szMessage = "Function hzvaGetAnzahlServerNames cannot be loaded !"
      case 504 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetAnzahlServerNames cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 505: szMessage = "Function hzvaGetAnzahlCatalogFiles cannot be loaded !"
      case 505 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetAnzahlCatalogFiles cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 506: szMessage = "Function hzvaGetAllCatalogFiles cannot be loaded !"
      case 506 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetAllCatalogFiles cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 513: szMessage = "Function hzvaGetLocalCatalogs cannot be loaded !"
      case 513 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetLocalCatalogs cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 514: szMessage = "Function hzvaReadCatalog cannot be loaded !"
      case 514 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaReadCatalog cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 515: szMessage = "Function hzvaFreeCatalogStructs cannot be loaded !"
      case 515 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaFreeCatalogStructs cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 516: szMessage = "Function hzvaWriteWSK cannot be loaded !"
      case 516 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaWriteWSK cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 517: szMessage = "Function hzvaGetPathname cannot be loaded !"
      case 517 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetPathname cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 520: szMessage = "Function hzvaGetServerCatalogs cannot be loaded !"
      case 520 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaGetServerCatalogs cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 521: szMessage = "Function hzvaReadServerCatalog cannot be loaded !"
      case 521 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaReadServerCatalog cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 522: szMessage = "Function hzvaFreeListMemory cannot be loaded !"
      case 522 :
         ZeidonStringCopy( szMessage, 1, 0, "Function hzvaFreeListMemory cannot be loaded !", 1, 0, 257 );
         break ;

      //:OF 530: szMessage = "Import aborted !"
      case 530 :
         ZeidonStringCopy( szMessage, 1, 0, "Import aborted !", 1, 0, 257 );
         break ;

      //:OF 551: szMessage = "No memory for Serverlist !"
      case 551 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for Serverlist !", 1, 0, 257 );
         break ;

      //:OF 552: szMessage = "No memory for list of Catalogfiles !"
      case 552 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for list of Catalogfiles !", 1, 0, 257 );
         break ;

      //:OF 553: szMessage = "No Catalogfiles found !"
      case 553 :
         ZeidonStringCopy( szMessage, 1, 0, "No Catalogfiles found !", 1, 0, 257 );
         break ;

      //:OF 560: szMessage = "No memory for Siron ONC !"
      case 560 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for Siron ONC !", 1, 0, 257 );
         break ;

      //:OF 561: szMessage = "No memory for Siron GENAT COMMON !"
      case 561 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for Siron GENAT COMMON !", 1, 0, 257 );
         break ;

      //:OF 562: szMessage = "Catalogfile could no be opened !"
      case 562 :
         ZeidonStringCopy( szMessage, 1, 0, "Catalogfile could no be opened !", 1, 0, 257 );
         break ;

      //:OF 563: szMessage = "Read error while reading the catalogfile !"
      case 563 :
         ZeidonStringCopy( szMessage, 1, 0, "Read error while reading the catalogfile !", 1, 0, 257 );
         break ;

      //:OF 564: szMessage = "No memory for local cataloglist !"
      case 564 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for local cataloglist !", 1, 0, 257 );
         break ;

      //:OF 570: szMessage = "Cannot load Catalog !"
      case 570 :
         ZeidonStringCopy( szMessage, 1, 0, "Cannot load Catalog !", 1, 0, 257 );
         break ;

      //:OF 571: szMessage = "No memory for Catalog data !"
      case 571 :
         ZeidonStringCopy( szMessage, 1, 0, "No memory for Catalog data !", 1, 0, 257 );
         break ;

      //:OF 581: szMessage = "First level for re-definitions is level 02 !"
      case 581 :
         ZeidonStringCopy( szMessage, 1, 0, "First level for re-definitions is level 02 !", 1, 0, 257 );
         break ;

      //:OF 582: szMessage = "WKS-File not found - File should be deleted !"
      case 582 :
         ZeidonStringCopy( szMessage, 1, 0, "WKS-File not found - File should be deleted !", 1, 0, 257 );
         break ;

      //:OF 583: szMessage = "Stringlength of Siron Attribute Name is too long"
      case 583 :
         ZeidonStringCopy( szMessage, 1, 0, "Stringlength of Siron Attribute Name is too long", 1, 0, 257 );
         break ;

      //:OF 590: szMessage = "Internal Server error while loading catalog list !"
      case 590 :
         ZeidonStringCopy( szMessage, 1, 0, "Internal Server error while loading catalog list !", 1, 0, 257 );
         break ;

      //:OF 591: szMessage = "Internal Server error while loading a single catalog!"
      case 591 :
         ZeidonStringCopy( szMessage, 1, 0, "Internal Server error while loading a single catalog!", 1, 0, 257 );
         break ;

      //:OF 592: szMessage = "Old catalog size! Please create a new catalog!"
      case 592 :
         ZeidonStringCopy( szMessage, 1, 0, "Old catalog size! Please create a new catalog!", 1, 0, 257 );
         break ;

      //:OF 593: szMessage = "Length of Catalog is incorrect !"
      case 593 :
         ZeidonStringCopy( szMessage, 1, 0, "Length of Catalog is incorrect !", 1, 0, 257 );
         break ;

      //:OF 594: szMessage = "Key-Attribute is incorrect !"
      case 594 :
         ZeidonStringCopy( szMessage, 1, 0, "Key-Attribute is incorrect !", 1, 0, 257 );
         break ;

      //:OF 595: szMessage = "Cannot connect to server !"
      case 595 :
         ZeidonStringCopy( szMessage, 1, 0, "Cannot connect to server !", 1, 0, 257 );
         break ;

      //:OF 596: szMessage = "Cannot define RPC function: GETKAT !"
      case 596 :
         ZeidonStringCopy( szMessage, 1, 0, "Cannot define RPC function: GETKAT !", 1, 0, 257 );
         break ;

      //:OF 598: szMessage = "Cannot define RPC function: GETKATANZ !"
      case 598 :
         ZeidonStringCopy( szMessage, 1, 0, "Cannot define RPC function: GETKATANZ !", 1, 0, 257 );
         break ;

      //:OF 599: szMessage = "Cannot define RPC function: GETKATLST !"
      case 599 :
         ZeidonStringCopy( szMessage, 1, 0, "Cannot define RPC function: GETKATLST !", 1, 0, 257 );
         break ;

      //:OF 601: szMessage = "GETKAT: Definition of parameter 1 is incorrect !"
      case 601 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKAT: Definition of parameter 1 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 602: szMessage = "GETKAT: Definition of parameter 2 is incorrect !"
      case 602 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKAT: Definition of parameter 2 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 603: szMessage = "GETKAT: Definition of parameter 3 is incorrect !"
      case 603 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKAT: Definition of parameter 3 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 604: szMessage = "GETKAT: Definition of parameter 4 is incorrect !"
      case 604 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKAT: Definition of parameter 4 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 605: szMessage = "GETKAT: Definition of parameter 5 is incorrect !"
      case 605 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKAT: Definition of parameter 5 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 607: szMessage = "GETKAT: Communication with server failed !"
      case 607 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKAT: Communication with server failed !", 1, 0, 257 );
         break ;

      //:OF 611: szMessage = "GETKATANZ: Definition of parameter 1 is incorrect !"
      case 611 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATANZ: Definition of parameter 1 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 612: szMessage = "GETKATANZ: Definition of parameter 2 is incorrect !"
      case 612 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATANZ: Definition of parameter 2 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 613: szMessage = "GETKATANZ: Definition of parameter 3 is incorrect !"
      case 613 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATANZ: Definition of parameter 3 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 614: szMessage = "GETKATANZ: Definition of parameter 4 is incorrect !"
      case 614 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATANZ: Definition of parameter 4 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 615: szMessage = "GETKATANZ: Definition of parameter 5 is incorrect !"
      case 615 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATANZ: Definition of parameter 5 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 616: szMessage = "GETKATANZ: Communication with server failed !"
      case 616 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATANZ: Communication with server failed !", 1, 0, 257 );
         break ;

      //:OF 621: szMessage = "GETKATLST: Definition of parameter 1 is incorrect !"
      case 621 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Definition of parameter 1 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 622: szMessage = "GETKATLST: Definition of parameter 2 is incorrect !"
      case 622 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Definition of parameter 2 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 623: szMessage = "GETKATLST: Definition of parameter 3 is incorrect !"
      case 623 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Definition of parameter 3 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 624: szMessage = "GETKATLST: Definition of parameter 4 is incorrect !"
      case 624 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Definition of parameter 4 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 625: szMessage = "GETKATLST: Definition of parameter 5 is incorrect !"
      case 625 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Definition of parameter 5 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 626: szMessage = "GETKATLST: Definition of parameter 6 is incorrect !"
      case 626 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Definition of parameter 6 is incorrect !", 1, 0, 257 );
         break ;

      //:OF 627: szMessage = "GETKATLST: Communication with server failed !"
      case 627 :
         ZeidonStringCopy( szMessage, 1, 0, "GETKATLST: Communication with server failed !", 1, 0, 257 );
         break ;

      //:OF 998: szMessage = "Function in construction !"
      case 998 :
         ZeidonStringCopy( szMessage, 1, 0, "Function in construction !", 1, 0, 257 );
         break ;

      //:OF 999: szMessage = "Function not yet supported !"
      case 999 :
         ZeidonStringCopy( szMessage, 1, 0, "Function not yet supported !", 1, 0, 257 );
         break ;

      //:otherwise: szMessage = "Internal error !"
      default  :
         ZeidonStringCopy( szMessage, 1, 0, "Internal error !", 1, 0, 257 );
         break ;
   } 


   //:END

   //:MessagePrompt( vSubtask, "VSAM0001", "Catalog Import", szMessage,
   //:              1, zBUTTONS_OK, zRESPONSE_OK, zICON_INFORMATION )
   MessagePrompt( vSubtask, "VSAM0001", "Catalog Import", szMessage, 1, zBUTTONS_OK, zRESPONSE_OK, zICON_INFORMATION );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DropWorkViews( VIEW vSubtask )

//:   VIEW TZSIIMWK REGISTERED AS TZSIIMWK
zOPER_EXPORT zSHORT OPERATION
DropWorkViews( zVIEW     vSubtask )
{
   zVIEW     TZSIIMWK = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZSIIMWK, "TZSIIMWK", vSubtask, zLEVEL_TASK );

   //:FreeHZVAFFZ( )  // Free loaded Library
   FreeHZVAFFZ( );

   //:IF  TZSIIMWK != 0
   if ( TZSIIMWK != 0 )
   { 
      //:DropView( TZSIIMWK ) // Drop WorkView
      DropView( TZSIIMWK );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:LoadCatalogList( VIEW vSubtask )

//:   VIEW TZSIIMWK REGISTERED AS TZSIIMWK
zOPER_EXPORT zSHORT OPERATION
LoadCatalogList( zVIEW     vSubtask )
{
   zVIEW     TZSIIMWK = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZSIIMWK, "TZSIIMWK", vSubtask, zLEVEL_TASK );

   //:nRC = GetAllCatalogNames( vSubtask, TZSIIMWK )
   nRC = GetAllCatalogNames( vSubtask, TZSIIMWK );

   //:IF nRC != 0
   if ( nRC != 0 )
   { 
      //: ShowMessage( vSubtask, nRC )
      o_ShowMessage( vSubtask, nRC );
      //:ELSE
   } 
   else
   { 
      //: OrderEntityForView( TZSIIMWK, "CatFiles", "wkCatFiles A" )
      OrderEntityForView( TZSIIMWK, "CatFiles", "wkCatFiles A" );
      //: SET CURSOR  FIRST TZSIIMWK.CatFiles
      RESULT = SetCursorFirstEntity( TZSIIMWK, "CatFiles", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
RefreshCatalogList( zVIEW     vSubtask )
{

   //:RefreshCatalogList( VIEW vSubtask )

   //:RefreshCtrl( vSubtask, "CatalogList" )
   RefreshCtrl( vSubtask, "CatalogList" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ImportCatalogAndWSK( VIEW vSubtask )

//:   VIEW TZSIIMWK REGISTERED AS TZSIIMWK
zOPER_EXPORT zSHORT OPERATION
ImportCatalogAndWSK( zVIEW     vSubtask )
{
   zVIEW     TZSIIMWK = 0; 
   zSHORT    RESULT; 
   //:VIEW TZSIIMLO REGISTERED AS TZSIIMLO
   zVIEW     TZSIIMLO = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:INTEGER lCtrlState
   zLONG     lCtrlState = 0; 
   //:STRING (  256  ) szFilename
   zCHAR     szFilename[ 257 ] = { 0 }; 
   zCHAR     szTempString_0[ 10 ]; 
   zCHAR     szTempString_1[ 10 ]; 
   zCHAR     szTempString_2[ 10 ]; 

   RESULT = GetViewByName( &TZSIIMWK, "TZSIIMWK", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZSIIMLO, "TZSIIMLO", vSubtask, zLEVEL_TASK );

   //:// Build WSK-File
   //:nRC = BuildWSK( vSubtask,
   //:                TZSIIMWK,
   //:                TZSIIMWK.TZSIIMWK.wkServerName,
   //:                TZSIIMWK.CatFiles.wkCatFiles,
   //:                TZSIIMWK.Catalogs.wkCatalogName,
   //:                szFilename )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZSIIMWK, "TZSIIMWK", "wkServerName" );
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZSIIMWK, "CatFiles", "wkCatFiles" );
   GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), TZSIIMWK, "Catalogs", "wkCatalogName" );
   nRC = BuildWSK( vSubtask, TZSIIMWK, szTempString_0, szTempString_1, szTempString_2, szFilename );

   //:IF  nRC != 0
   if ( nRC != 0 )
   { 
      //:ShowMessage( vSubtask, nRC )
      o_ShowMessage( vSubtask, nRC );
      //:ELSE
   } 
   else
   { 
      //:// We wrote a new WSK-File. Take it into the WorkLod for the list
      //:nRC = SetWSKInLod( vSubtask, TZSIIMLO, szFilename )
      nRC = SetWSKInLod( vSubtask, TZSIIMLO, szFilename );

      //:// Should be directly imported ????
      //:lCtrlState = GetCtrlState( vSubtask,
      //:                           "RadioImportAndWSK",
      //:                           zCONTROL_STATUS_CHECKED )
      lCtrlState = GetCtrlState( vSubtask, "RadioImportAndWSK", zCONTROL_STATUS_CHECKED );

      //:IF  lCtrlState = TRUE
      if ( lCtrlState == TRUE )
      { 
         //: // Import into the datamodel
         //: ActivateAndImport( vSubtask, szFilename )
         o_ActivateAndImport( vSubtask, szFilename );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:EnableButtons( VIEW vSubtask )

//:   VIEW TZSIIMLO REGISTERED AS TZSIIMLO
zOPER_EXPORT zSHORT OPERATION
EnableButtons( zVIEW     vSubtask )
{
   zVIEW     TZSIIMLO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZSIIMLO, "TZSIIMLO", vSubtask, zLEVEL_TASK );

   //:IF TZSIIMLO.KatalogFile EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZSIIMLO, "KatalogFile" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetCtrlState( vSubtask, "ImportWSK", zCONTROL_STATUS_ENABLED, TRUE )
      SetCtrlState( vSubtask, "ImportWSK", zCONTROL_STATUS_ENABLED, TRUE );
      //:SetCtrlState( vSubtask, "DeleteWSK", zCONTROL_STATUS_ENABLED, TRUE )
      SetCtrlState( vSubtask, "DeleteWSK", zCONTROL_STATUS_ENABLED, TRUE );
      //:ELSE
   } 
   else
   { 
      //:SetCtrlState( vSubtask, "ImportWSK", zCONTROL_STATUS_ENABLED, FALSE )
      SetCtrlState( vSubtask, "ImportWSK", zCONTROL_STATUS_ENABLED, FALSE );
      //:SetCtrlState( vSubtask, "DeleteWSK", zCONTROL_STATUS_ENABLED, FALSE )
      SetCtrlState( vSubtask, "DeleteWSK", zCONTROL_STATUS_ENABLED, FALSE );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DropAllWorkViews( VIEW vSubtask )

//:   VIEW TZSIIMLO REGISTERED AS TZSIIMLO
zOPER_EXPORT zSHORT OPERATION
DropAllWorkViews( zVIEW     vSubtask )
{
   zVIEW     TZSIIMLO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZSIIMLO, "TZSIIMLO", vSubtask, zLEVEL_TASK );

   //:// Unload Siron DLL's and Drop WorkLod: TZSIIMWK
   //:DropWorkViews( vSubtask )
   DropWorkViews( vSubtask );

   //:IF  TZSIIMLO != 0
   if ( TZSIIMLO != 0 )
   { 
      //:DropView( TZSIIMLO ) // Drop WorkView
      DropView( TZSIIMLO );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ChangeEntityOrAttributeName( VIEW vSubtask, STRING ( 32 ) szName)

//:   STRING (2)  szSpecChar
static zSHORT
o_ChangeEntityOrAttributeName( zVIEW     vSubtask,
                               zPCHAR    szName )
{
   zCHAR     szSpecChar[ 3 ] = { 0 }; 
   //:SHORT       nPosition
   zSHORT    nPosition = 0; 
   //:INTEGER     nLength
   zLONG     nLength = 0; 


   //:// Test length of Siron Attribute Name
   //:nLength = zGetStringLen( szName )
   nLength = zGetStringLen( szName );
   //:IF nLength > 32
   if ( nLength > 32 )
   { 
      //:ShowMessage( vSubtask, 583 )
      o_ShowMessage( vSubtask, 583 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Change all special characters of the attributename to '_'
   //:nPosition = 0
   nPosition = 0;
   //:LOOP WHILE nPosition != -1
   while ( nPosition != -1 )
   { 
      //:nPosition = SearchSpecialCharacter( szName, szSpecChar )
      nPosition = SearchSpecialCharacter( szName, szSpecChar );
      //:IF ( nPosition != -1 )
      if ( nPosition != -1 )
      { 
         //:zSearchAndReplace( szName /* TargetString */,
         //:                   nLength /* TargetLen */,
         //:                   szSpecChar /* SearchString */,
         //:                   "_" /* ReplaceString */ )
         zSearchAndReplace( szName, nLength, szSpecChar, "_" );
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
