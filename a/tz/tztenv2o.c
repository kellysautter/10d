#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zSHORT oTZTENVRO_WriteErrorMsgToList( zVIEW, zPCHAR );

static zSHORT
oTZTENVRO_SetCursorNewRelType( zVIEW     OldERD,
                               zVIEW     NewERD );


static zSHORT
oTZTENVRO_TE_MigrateOrgSironSubF( zVIEW     vSubtask,
                                  zVIEW     NewTE,
                                  zVIEW     OldTE,
                                  zVIEW     NewWorkTE,
                                  zVIEW     OldWorkTE );


static zVOID
oTZTENVRO_SyncTablRecKeyRecurs( zVIEW     vDTE,
                                zVIEW     vDTE_Recurs,
                                zVIEW     vERD );


//:TRANSFORMATION OPERATION
//:SyncTablRecKey( VIEW vDTE BASED ON LOD TZTENVRO,
//:                VIEW vERD BASED ON LOD TZEREMDO,
//:                VIEW vSubtask )

//:   VIEW        vDTE_Recurs BASED ON LOD TZTENVRO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_SyncTablRecKey( zVIEW     vDTE,
                          zVIEW     vERD,
                          zVIEW     vSubtask )
{
   zVIEW     vDTE_Recurs = 0; 
   //:STRING (32) szIdentifierName
   zCHAR     szIdentifierName[ 33 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 


   //:// Create key entries from identifiers, for those identifiers that are not
   //:// already associated with key entries.  This must follow a recursive
   //:// path, if there is a chain of identifiers that are defined using relationships.
   //:// Note that we are processing only the first ER_EntIdentifier, assuming there is only
   //:// one per entity.

   //:FOR EACH vERD.ER_EntIdentifier WITHIN vERD.EntpER_Model
   RESULT = SetCursorFirstEntity( vERD, "ER_EntIdentifier", "EntpER_Model" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vDTE.ER_EntIdentifier WITHIN vDTE.TE_DB_Environ WHERE
      //:                 vDTE.ER_EntIdentifier.ZKey = vERD.ER_EntIdentifier.ZKey
      GetIntegerFromAttribute( &lTempInteger_0, vERD, "ER_EntIdentifier", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vDTE, "ER_EntIdentifier", "ZKey", lTempInteger_0, "TE_DB_Environ" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// The identifier has not yet been created in the TE.
         //:// Position on the correct entity
         //:SET CURSOR FIRST vDTE.ER_Entity WITHIN vDTE.TE_DB_Environ  WHERE
         //:                 vDTE.ER_Entity.ZKey = vERD.ER_Entity.ZKey
         GetIntegerFromAttribute( &lTempInteger_1, vERD, "ER_Entity", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vDTE, "ER_Entity", "ZKey", lTempInteger_1, "TE_DB_Environ" );
         //:CreateMetaEntity( vSubtask, vDTE, "TE_TablRecKey", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vDTE, "TE_TablRecKey", zPOS_AFTER );
         //:szIdentifierName = vERD.ER_EntIdentifier.Name
         GetVariableFromAttribute( szIdentifierName, 0, 'S', 33, vERD, "ER_EntIdentifier", "Name", "", 0 );
         //:TranslateStringToUpperCase( szIdentifierName )
         TranslateStringToUpperCase( szIdentifierName );
         //:vDTE.TE_TablRecKey.Name =       szIdentifierName
         SetAttributeFromString( vDTE, "TE_TablRecKey", "Name", szIdentifierName );
         //:// The Category first equals 2 (Attributes Only), assuming that the key is not for relationships.
         //:// Later, we will change it to 3 (Attributes & Relationships), if necessary.
         //:vDTE.TE_TablRecKey.Category = "2"
         SetAttributeFromString( vDTE, "TE_TablRecKey", "Category", "2" );
         //:INCLUDE vDTE.ER_EntIdentifier FROM vERD.ER_EntIdentifier
         RESULT = IncludeSubobjectFromSubobject( vDTE, "ER_EntIdentifier", vERD, "ER_EntIdentifier", zPOS_AFTER );
         //:// Include each field that is a part of the key
         //:FOR EACH vERD.ER_AttributeIdentifier WITHIN vERD.ER_EntIdentifier
         RESULT = SetCursorFirstEntity( vERD, "ER_AttributeIdentifier", "ER_EntIdentifier" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:SET CURSOR FIRST vDTE.ER_Attribute WITHIN vDTE.TE_TablRec WHERE
            //:                 vDTE.ER_Attribute.ZKey = vERD.ER_AttributeIdentifier.ZKey
            GetIntegerFromAttribute( &lTempInteger_2, vERD, "ER_AttributeIdentifier", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vDTE, "ER_Attribute", "ZKey", lTempInteger_2, "TE_TablRec" );
            //:vDTE.TE_FieldDataRel.Key = "Y"
            SetAttributeFromString( vDTE, "TE_FieldDataRel", "Key", "Y" );
            //:INCLUDE vDTE.TE_FieldDataRelKey FROM vDTE.TE_FieldDataRel
            RESULT = IncludeSubobjectFromSubobject( vDTE, "TE_FieldDataRelKey", vDTE, "TE_FieldDataRel", zPOS_AFTER );
            RESULT = SetCursorNextEntity( vERD, "ER_AttributeIdentifier", "ER_EntIdentifier" );
         } 

         //:END
         //:// Now follow any relationship paths to get what will be imbedded keys.
         //:CreateViewFromViewForTask( vDTE_Recurs, vDTE, 0 )
         CreateViewFromViewForTask( &vDTE_Recurs, vDTE, 0 );
         //:FOR EACH vERD.ER_RelLinkIdentifier WITHIN vERD.ER_EntIdentifier
         RESULT = SetCursorFirstEntity( vERD, "ER_RelLinkIdentifier", "ER_EntIdentifier" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:vDTE.TE_TablRecKey.Category = "3"
            SetAttributeFromString( vDTE, "TE_TablRecKey", "Category", "3" );
            //:SyncTablRecKeyRecurs( vDTE, vDTE_Recurs, vERD )
            oTZTENVRO_SyncTablRecKeyRecurs( vDTE, vDTE_Recurs, vERD );
            RESULT = SetCursorNextEntity( vERD, "ER_RelLinkIdentifier", "ER_EntIdentifier" );
         } 

         //:END
         //:DropView( vDTE_Recurs )
         DropView( vDTE_Recurs );
      } 

      RESULT = SetCursorNextEntity( vERD, "ER_EntIdentifier", "EntpER_Model" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:SetCursorNewRelType( VIEW OldERD BASED ON LOD TZEREMDO,
//:                     VIEW NewERD BASED ON LOD TZEREMDO )

//:   VIEW NewERD2 BASED ON LOD TZEREMDO
static zSHORT
oTZTENVRO_SetCursorNewRelType( zVIEW     OldERD,
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
         //:IF NewERD2.ER_Entity_2.Name = OldERD2.ER_Entity_2.Name
         if ( CompareAttributeToAttribute( NewERD2, "ER_Entity_2", "Name", OldERD2, "ER_Entity_2", "Name" ) == 0 )
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
         //:MessageSend( NewERD, "TE00507", "Technical Environment",
         //:             "System Error: No ERD RelType Match",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( NewERD, "TE00507", "Technical Environment", "System Error: No ERD RelType Match", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
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


//:TRANSFORMATION OPERATION
//:TE_Migrate( VIEW          NewTE      BASED ON LOD TZTENVRO,
//:            STRING ( 32 ) TE_MetaName,
//:            VIEW          SourceLPLR BASED ON LOD TZCMLPLO,
//:            VIEW          vSubtask )

//:   VIEW NewERD      BASED ON LOD TZEREMDO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_TE_Migrate( zPVIEW    NewTE,
                      zPCHAR    TE_MetaName,
                      zVIEW     SourceLPLR,
                      zVIEW     vSubtask )
{
   zVIEW     NewERD = 0; 
   //:VIEW NewWorkERD  BASED ON LOD TZEREMDO
   zVIEW     NewWorkERD = 0; 
   //:VIEW OldERD      BASED ON LOD TZEREMDO
   zVIEW     OldERD = 0; 

   //:VIEW OldTE       BASED ON LOD TZTENVRO
   zVIEW     OldTE = 0; 
   //:VIEW OldWorkTE   BASED ON LOD TZTENVRO
   zVIEW     OldWorkTE = 0; 
   //:VIEW OldTE2      BASED ON LOD TZTENVRO
   zVIEW     OldTE2 = 0; 
   //:VIEW NewTE2      BASED ON LOD TZTENVRO
   zVIEW     NewTE2 = 0; 
   //:VIEW NewWorkTE   BASED ON LOD TZTENVRO
   zVIEW     NewWorkTE = 0; 
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 

   //:STRING ( 513 ) SourceFileName              // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 257 ) szMsg                       // zSHORT_MESSAGE_LTH+1
   zCHAR     szMsg[ 258 ] = { 0 }; 
   //:INTEGER        FoundFlag
   zLONG     FoundFlag = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_7; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zLONG     lTempInteger_8; 


   //:// In addition, TE_TablRecAccessed was not included because it did not look to be
   //:// processed by workable code (references were in dialogs that did not look used) and
   //:// when used, it looked like it would be a work area for dialogs.

   //:// Note that the below code must be changed to support multiple ER_Entity's per table.
   //:// The existing code for ER_Attribute and ER_EntIdentifier assumes one ER_Entity per
   //:// table.

   //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zREFER_ERD_META );

   //:GET VIEW NewERD NAMED "NewERD"
   RESULT = GetViewByName( &NewERD, "NewERD", SourceLPLR, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// Activate the current ERD.  If it doesn't exist, it is an error.
      //:nRC = ActivateMetaOI( vSubtask, NewERD, CurrentLPLR, zREFER_ERD_META, 0 )
      nRC = ActivateMetaOI( vSubtask, &NewERD, CurrentLPLR, zREFER_ERD_META, 0 );
      //:IF nRC < 1
      if ( nRC < 1 )
      { 
         //:MessageSend( vSubtask, "TE00501", "Technical Environment",
         //:             "No current Data Model exists.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( vSubtask, "TE00501", "Technical Environment", "No current Data Model exists.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:RETURN 0
         return( 0 );
      } 

      //:END
      //:NAME VIEW NewERD "NewERD"
      SetNameForView( NewERD, "NewERD", 0, zLEVEL_TASK );
   } 

   //:END

   //:SET CURSOR FIRST SourceLPLR.W_MetaType WHERE SourceLPLR.W_MetaType.Type = 2004
   RESULT = SetCursorFirstEntityByInteger( SourceLPLR, "W_MetaType", "Type", 2004, "" );
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + SourceLPLR.W_MetaDef.Name + ".PMD"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, SourceLPLR, "W_MetaDef", "Name", "", 0 );
   ZeidonStringConcat( SourceFileName, 1, 0, szTempString_0, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PMD", 1, 0, 514 );
   //:ActivateOI_FromFile ( OldERD, "TZEREMDO", SourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &OldERD, "TZEREMDO", SourceLPLR, SourceFileName, 8192 );
   //:// 8192 is zIGNORE_ATTRIB_ERRORS

   //:// Activate existing source meta OldTE
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + TE_MetaName + ".DTE"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, TE_MetaName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".DTE", 1, 0, 514 );

   //:ActivateOI_FromFile( OldTE, "TZTENVRO", CurrentLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &OldTE, "TZTENVRO", CurrentLPLR, SourceFileName, 8192 );
   //:NAME VIEW OldTE "OldTE"
   SetNameForView( OldTE, "OldTE", 0, zLEVEL_TASK );

   //:ActivateEmptyMetaOI( vSubtask, NewTE, zSOURCE_DTE_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, NewTE, zSOURCE_DTE_META, zSINGLE );
   //:NAME VIEW NewTE "NewTE"
   SetNameForView( *NewTE, "NewTE", 0, zLEVEL_TASK );

   //:CreateMetaEntity( vSubtask, NewTE, "TE_DB_Environ", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewTE, "TE_DB_Environ", zPOS_AFTER );
   //:SetMatchingAttributesByName ( NewTE, "TE_DB_Environ", OldTE,
   //:                              "TE_DB_Environ", zSET_NULL )
   SetMatchingAttributesByName( *NewTE, "TE_DB_Environ", OldTE, "TE_DB_Environ", zSET_NULL );

   //:// The TE name will be the same as the LPLR.
   //:ResetViewFromSubobject( CurrentLPLR )
   ResetViewFromSubobject( CurrentLPLR );
   //:NewTE.TE_DB_Environ.Name = CurrentLPLR.LPLR.Name
   SetAttributeFromAttribute( *NewTE, "TE_DB_Environ", "Name", CurrentLPLR, "LPLR", "Name" );

   //:FOR EACH OldTE.TE_DBMS_Source
   RESULT = SetCursorFirstEntity( OldTE, "TE_DBMS_Source", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( vSubtask, NewTE, "TE_DBMS_Source", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewTE, "TE_DBMS_Source", zPOS_AFTER );
      //:SetMatchingAttributesByName ( NewTE, "TE_DBMS_Source", OldTE,
      //:                              "TE_DBMS_Source", zSET_NULL )
      SetMatchingAttributesByName( *NewTE, "TE_DBMS_Source", OldTE, "TE_DBMS_Source", zSET_NULL );

      //:FOR EACH OldTE.TE_TablRec
      RESULT = SetCursorFirstEntity( OldTE, "TE_TablRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( vSubtask, NewTE, "TE_TablRec", zPOS_AFTER )
         CreateMetaEntity( vSubtask, *NewTE, "TE_TablRec", zPOS_AFTER );
         //:SetMatchingAttributesByName ( NewTE, "TE_TablRec", OldTE,
         //:                              "TE_TablRec", zSET_NULL )
         SetMatchingAttributesByName( *NewTE, "TE_TablRec", OldTE, "TE_TablRec", zSET_NULL );

         //:FOR EACH OldTE.ER_Entity
         RESULT = SetCursorFirstEntity( OldTE, "ER_Entity", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:SET CURSOR FIRST NewERD.ER_Entity WHERE
            //:   NewERD.ER_Entity.Name = OldTE.ER_Entity.Name
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), OldTE, "ER_Entity", "Name" );
            RESULT = SetCursorFirstEntityByString( NewERD, "ER_Entity", "Name", szTempString_1, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:INCLUDE NewTE.ER_Entity FROM NewERD.ER_Entity
               RESULT = IncludeSubobjectFromSubobject( *NewTE, "ER_Entity", NewERD, "ER_Entity", zPOS_AFTER );

               //:ELSE
            } 
            else
            { 
               //:szMsg = "ER_Entity (" + OldTE.ER_Entity.Name + ") not found in new ERD."
               GetVariableFromAttribute( szTempString_1, 0, 'S', 33, OldTE, "ER_Entity", "Name", "", 0 );
               ZeidonStringCopy( szMsg, 1, 0, "ER_Entity (", 1, 0, 258 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 258 );
               ZeidonStringConcat( szMsg, 1, 0, ") not found in new ERD.", 1, 0, 258 );
               //:MessageSend( vSubtask, "TE00502", "Technical Environment",
               //:            szMsg,
               //:            zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vSubtask, "TE00502", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            } 

            RESULT = SetCursorNextEntity( OldTE, "ER_Entity", "" );

            //:END
         } 

         //:END

         //:IF OldTE.ER_RelType EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( OldTE, "ER_RelType" );
         if ( lTempInteger_0 == 0 )
         { 
            //:// Locate the ER_RelType in the NewERD, based on the corresponding ER_RelType
            //:// in the OldERD.  This is kind of convoluted, since the match has to
            //:// consider both sides of the RelLink under the RelType.

            //:SET CURSOR FIRST OldERD.ER_RelType WHERE
            //:   OldERD.ER_RelType.ZKey = OldTE.ER_RelType.ZKey
            GetIntegerFromAttribute( &lTempInteger_1, OldTE, "ER_RelType", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelType", "ZKey", lTempInteger_1, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 

               //:nRC = SetCursorNewRelType( OldERD, NewERD )
               nRC = oTZTENVRO_SetCursorNewRelType( OldERD, NewERD );
               //:IF nRC = 0
               if ( nRC == 0 )
               { 
                  //:INCLUDE NewTE.ER_RelType FROM NewERD.ER_RelType
                  RESULT = IncludeSubobjectFromSubobject( *NewTE, "ER_RelType", NewERD, "ER_RelType", zPOS_AFTER );
                  //:ELSE
               } 
               else
               { 
                  //:RETURN -1
                  return( -1 );
               } 

               //:END

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

         //:// Note that the ER_Attribute loop has to follow the ER_Entity loop, so that the
         //:// Attribute is found under the correct entity.
         //:FOR EACH OldTE.TE_FieldDataRel
         RESULT = SetCursorFirstEntity( OldTE, "TE_FieldDataRel", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( vSubtask, NewTE, "TE_FieldDataRel", zPOS_AFTER )
            CreateMetaEntity( vSubtask, *NewTE, "TE_FieldDataRel", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewTE, "TE_FieldDataRel", OldTE,
            //:                              "TE_FieldDataRel", zSET_NULL )
            SetMatchingAttributesByName( *NewTE, "TE_FieldDataRel", OldTE, "TE_FieldDataRel", zSET_NULL );

            //:FOR EACH OldTE.ER_Attribute
            RESULT = SetCursorFirstEntity( OldTE, "ER_Attribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:SET CURSOR FIRST NewERD.ER_Attribute WHERE
               //:    NewERD.ER_Attribute.Name = OldTE.ER_Attribute.Name
               GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldTE, "ER_Attribute", "Name" );
               RESULT = SetCursorFirstEntityByString( NewERD, "ER_Attribute", "Name", szTempString_2, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:INCLUDE NewTE.ER_Attribute FROM NewERD.ER_Attribute
                  RESULT = IncludeSubobjectFromSubobject( *NewTE, "ER_Attribute", NewERD, "ER_Attribute", zPOS_AFTER );

                  //:ELSE
               } 
               else
               { 
                  //:szMsg = "ER_Attribute (" + OldTE.ER_Attribute.Name +
                  //:        ") not found in new ERD."
                  GetVariableFromAttribute( szTempString_2, 0, 'S', 33, OldTE, "ER_Attribute", "Name", "", 0 );
                  ZeidonStringCopy( szMsg, 1, 0, "ER_Attribute (", 1, 0, 258 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 258 );
                  ZeidonStringConcat( szMsg, 1, 0, ") not found in new ERD.", 1, 0, 258 );
                  //:MessageSend( vSubtask, "TE00503", "Technical Environment",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "TE00503", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               } 

               RESULT = SetCursorNextEntity( OldTE, "ER_Attribute", "" );

               //:END
            } 


            //:END

            //:IF OldTE.ER_RelLink EXISTS AND
            lTempInteger_2 = CheckExistenceOfEntity( OldTE, "ER_RelLink" );
            //:   ( OldTE.ER_Entity EXISTS OR OldTE.ER_RelType EXISTS )
            lTempInteger_3 = CheckExistenceOfEntity( OldTE, "ER_Entity" );
            lTempInteger_4 = CheckExistenceOfEntity( OldTE, "ER_RelType" );
            if ( lTempInteger_2 == 0 && ( lTempInteger_3 == 0 || lTempInteger_4 == 0 ) )
            { 
               //:// Locate the ER_RelLink entity in the E/R object corresponding to the
               //:// ER_RelLink in the Old TE.
               //:SET CURSOR FIRST OldERD.ER_RelLink_2 WITHIN OldERD.EntpER_Model WHERE
               //:    OldERD.ER_RelLink_2.ZKey = OldTE.ER_RelLink.ZKey
               GetIntegerFromAttribute( &lTempInteger_5, OldTE, "ER_RelLink", "ZKey" );
               RESULT = SetCursorFirstEntityByInteger( OldERD, "ER_RelLink_2", "ZKey", lTempInteger_5, "EntpER_Model" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:nRC = SetCursorNewRelType( OldERD, NewERD )
                  nRC = oTZTENVRO_SetCursorNewRelType( OldERD, NewERD );
                  //:IF nRC = 0
                  if ( nRC == 0 )
                  { 
                     //:INCLUDE NewTE.ER_RelLink FROM NewERD.ER_RelLink_2
                     RESULT = IncludeSubobjectFromSubobject( *NewTE, "ER_RelLink", NewERD, "ER_RelLink_2", zPOS_AFTER );
                     //:ELSE
                  } 
                  else
                  { 
                     //:RETURN -1
                     return( -1 );
                  } 

                  //:END
                  //:ELSE
               } 
               else
               { 
                  //:RETURN -1
                  return( -1 );
               } 

               //:END
            } 

            RESULT = SetCursorNextEntity( OldTE, "TE_FieldDataRel", "" );

            //:END
         } 


         //:END

         //:FOR EACH OldTE.TE_TablRecKey
         RESULT = SetCursorFirstEntity( OldTE, "TE_TablRecKey", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( vSubtask, NewTE, "TE_TablRecKey", zPOS_AFTER )
            CreateMetaEntity( vSubtask, *NewTE, "TE_TablRecKey", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewTE, "TE_TablRecKey", OldTE,
            //:                             "TE_TablRecKey", zSET_NULL )
            SetMatchingAttributesByName( *NewTE, "TE_TablRecKey", OldTE, "TE_TablRecKey", zSET_NULL );

            //:// Note that the ER_EntIdentifier loop has to follow the ER_Entity loop, so that the
            //:// ER_Identifier is found under the correct entity.
            //:IF OldTE.ER_EntIdentifier EXISTS
            lTempInteger_6 = CheckExistenceOfEntity( OldTE, "ER_EntIdentifier" );
            if ( lTempInteger_6 == 0 )
            { 
               //:SET CURSOR FIRST NewERD.ER_EntIdentifier WHERE
               //:   NewERD.ER_EntIdentifier.Name = OldTE.ER_EntIdentifier.Name
               GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), OldTE, "ER_EntIdentifier", "Name" );
               RESULT = SetCursorFirstEntityByString( NewERD, "ER_EntIdentifier", "Name", szTempString_3, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:INCLUDE NewTE.ER_EntIdentifier FROM NewERD.ER_EntIdentifier
                  RESULT = IncludeSubobjectFromSubobject( *NewTE, "ER_EntIdentifier", NewERD, "ER_EntIdentifier", zPOS_AFTER );

                  //:ELSE
               } 
               else
               { 
                  //:szMsg = "ER_EntIdentifier (" + OldTE.ER_EntIdentifier.Name +
                  //:       ") not found in new ERD."
                  GetVariableFromAttribute( szTempString_3, 0, 'S', 33, OldTE, "ER_EntIdentifier", "Name", "", 0 );
                  ZeidonStringCopy( szMsg, 1, 0, "ER_EntIdentifier (", 1, 0, 258 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_3, 1, 0, 258 );
                  ZeidonStringConcat( szMsg, 1, 0, ") not found in new ERD.", 1, 0, 258 );
                  //:MessageSend( vSubtask, "TE00504", "Technical Environment",
                  //:            szMsg,
                  //:            zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "TE00504", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               } 


               //:END

               //:// If this is for a secondary Identifier (any EntIdentifer after the first),
               //:// we will try to include the T_FieldDataRelKey entry from the attributes under
               //:// the current table.
               //:CreateViewFromViewForTask( NewWorkERD, NewERD, 0 )
               CreateViewFromViewForTask( &NewWorkERD, NewERD, 0 );
               //:SET CURSOR FIRST NewWorkERD.ER_EntIdentifier
               RESULT = SetCursorFirstEntity( NewWorkERD, "ER_EntIdentifier", "" );
               //:IF NewWorkERD.ER_EntIdentifier.ZKey != NewERD.ER_EntIdentifier.ZKey AND
               //:  OldTE.TE_FieldDataRelKey EXISTS
               lTempInteger_7 = CheckExistenceOfEntity( OldTE, "TE_FieldDataRelKey" );
               if ( CompareAttributeToAttribute( NewWorkERD, "ER_EntIdentifier", "ZKey", NewERD, "ER_EntIdentifier", "ZKey" ) != 0 && lTempInteger_7 == 0 )
               { 
                  //:// This is not the first EntIdentifier entry and the one from the OldTE
                  //:// has a TE_FieldDataRelKey entitiy.
                  //:CreateViewFromViewForTask( NewWorkTE, NewTE, 0 )
                  CreateViewFromViewForTask( &NewWorkTE, *NewTE, 0 );
                  //:SET CURSOR FIRST NewWorkTE.TE_FieldDataRel WHERE
                  //:       NewWorkTE.TE_FieldDataRel.Name = OldTE.TE_FieldDataRelKey.Name
                  GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), OldTE, "TE_FieldDataRelKey", "Name" );
                  RESULT = SetCursorFirstEntityByString( NewWorkTE, "TE_FieldDataRel", "Name", szTempString_4, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:NewWorkTE.TE_FieldDataRel.Key = "Y"
                     SetAttributeFromString( NewWorkTE, "TE_FieldDataRel", "Key", "Y" );
                     //:INCLUDE NewTE.TE_FieldDataRelKey FROM NewWorkTE.TE_FieldDataRel
                     RESULT = IncludeSubobjectFromSubobject( *NewTE, "TE_FieldDataRelKey", NewWorkTE, "TE_FieldDataRel", zPOS_AFTER );
                  } 

                  //:END
                  //:DropView ( NewWorkTE )
                  DropView( NewWorkTE );
               } 

               //:END
               //:DropView ( NewWorkERD )
               DropView( NewWorkERD );
            } 

            RESULT = SetCursorNextEntity( OldTE, "TE_TablRecKey", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( OldTE, "TE_TablRec", "" );
         //:END
      } 

      //:END

      //:// Now loop through again and migrate Siron Katalogs
      //:SET CURSOR FIRST NewTE.TE_TablRec
      RESULT = SetCursorFirstEntity( *NewTE, "TE_TablRec", "" );
      //:FOR EACH OldTE.TE_TablRec
      RESULT = SetCursorFirstEntity( OldTE, "TE_TablRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:// Migrate the Siron Katalog information.
         //:FOR EACH OldTE.SironKatalog
         RESULT = SetCursorFirstEntity( OldTE, "SironKatalog", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( vSubtask, NewTE, "SironKatalog", zPOS_AFTER )
            CreateMetaEntity( vSubtask, *NewTE, "SironKatalog", zPOS_AFTER );
            //:SetMatchingAttributesByName( NewTE, "SironKatalog",
            //:                            OldTE, "SironKatalog", zSET_NULL )
            SetMatchingAttributesByName( *NewTE, "SironKatalog", OldTE, "SironKatalog", zSET_NULL );
            //:FOR EACH OldTE.KatalogComment
            RESULT = SetCursorFirstEntity( OldTE, "KatalogComment", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( vSubtask, NewTE, "KatalogComment", zPOS_AFTER )
               CreateMetaEntity( vSubtask, *NewTE, "KatalogComment", zPOS_AFTER );
               //:SetMatchingAttributesByName( NewTE, "KatalogComment",
               //:                            OldTE, "KatalogComment", zSET_NULL )
               SetMatchingAttributesByName( *NewTE, "KatalogComment", OldTE, "KatalogComment", zSET_NULL );
               RESULT = SetCursorNextEntity( OldTE, "KatalogComment", "" );
            } 

            //:END
            //:FOR EACH OldTE.OrigSironField
            RESULT = SetCursorFirstEntity( OldTE, "OrigSironField", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( vSubtask, NewTE, "OrigSironField", zPOS_AFTER )
               CreateMetaEntity( vSubtask, *NewTE, "OrigSironField", zPOS_AFTER );
               //:SetMatchingAttributesByName( NewTE, "OrigSironField",
               //:                            OldTE, "OrigSironField", zSET_NULL )
               SetMatchingAttributesByName( *NewTE, "OrigSironField", OldTE, "OrigSironField", zSET_NULL );

               //:INCLUDE NewTE.SironGroup FROM NewTE.OrigSironField
               RESULT = IncludeSubobjectFromSubobject( *NewTE, "SironGroup", *NewTE, "OrigSironField", zPOS_AFTER );

               //:CreateViewFromViewForTask( NewWorkTE, NewTE, 0 )
               CreateViewFromViewForTask( &NewWorkTE, *NewTE, 0 );
               //:CreateViewFromViewForTask( OldWorkTE, OldTE, 0 )
               CreateViewFromViewForTask( &OldWorkTE, OldTE, 0 );

               //:FOR EACH OldTE.FieldComment
               RESULT = SetCursorFirstEntity( OldTE, "FieldComment", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:CreateMetaEntity( vSubtask, NewTE, "FieldComment", zPOS_AFTER )
                  CreateMetaEntity( vSubtask, *NewTE, "FieldComment", zPOS_AFTER );
                  //:SetMatchingAttributesByName( NewTE, "FieldComment",
                  //:                            OldTE, "FieldComment", zSET_NULL )
                  SetMatchingAttributesByName( *NewTE, "FieldComment", OldTE, "FieldComment", zSET_NULL );
                  RESULT = SetCursorNextEntity( OldTE, "FieldComment", "" );
               } 

               //:END
               //:FOR EACH OldTE.OrigSironSubField
               RESULT = SetCursorFirstEntity( OldTE, "OrigSironSubField", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:TE_MigrateOrgSironSubF( vSubtask, NewTE, OldTE, NewWorkTE, OldWorkTE )
                  oTZTENVRO_TE_MigrateOrgSironSubF( vSubtask, *NewTE, OldTE, NewWorkTE, OldWorkTE );
                  RESULT = SetCursorNextEntity( OldTE, "OrigSironSubField", "" );
               } 

               //:END

               //:DropView( NewWorkTE )
               DropView( NewWorkTE );
               //:DropView( OldWorkTE )
               DropView( OldWorkTE );
               RESULT = SetCursorNextEntity( OldTE, "OrigSironField", "" );
            } 

            RESULT = SetCursorNextEntity( OldTE, "SironKatalog", "" );
            //:END
         } 

         //:END
         //:SET CURSOR NEXT NewTE.TE_TablRec
         RESULT = SetCursorNextEntity( *NewTE, "TE_TablRec", "" );
         RESULT = SetCursorNextEntity( OldTE, "TE_TablRec", "" );
      } 

      //:END

      //:// Now loop through again and do the includes for TE_FieldDataRelKey.

      //:// For both includes from TE_FieldDataRel, we have a problem because there
      //:// is no unique logical key for TE_FieldDataRel (only the ZKey is unique).  This
      //:// makes it difficult to position on the correct entity to include.  (Similar to
      //:// the ER_RelLinkIdentifier include problem in ERD_Migrate)  We thus have to use the
      //:// relative position of TE_FieldDataRel within TE_DBMS_Source to determine
      //:// position.
      //:CreateViewFromViewForTask( OldTE2, OldTE, 0 )
      CreateViewFromViewForTask( &OldTE2, OldTE, 0 );
      //:NAME VIEW OldTE2 "OldTE2"
      SetNameForView( OldTE2, "OldTE2", 0, zLEVEL_TASK );
      //:CreateViewFromViewForTask( NewTE2, NewTE, 0 )
      CreateViewFromViewForTask( &NewTE2, *NewTE, 0 );
      //:NAME VIEW NewTE2 "NewTE2"
      SetNameForView( NewTE2, "NewTE2", 0, zLEVEL_TASK );

      //:FOR EACH OldTE.TE_TablRec
      RESULT = SetCursorFirstEntity( OldTE, "TE_TablRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST NewTE.TE_TablRec WHERE
         //:    NewTE.TE_TablRec.Name = OldTE.TE_TablRec.Name
         GetStringFromAttribute( szTempString_5, sizeof( szTempString_5 ), OldTE, "TE_TablRec", "Name" );
         RESULT = SetCursorFirstEntityByString( *NewTE, "TE_TablRec", "Name", szTempString_5, "" );

         //:// Process only the first TE_TablRecKey entry, which represents the first Identifier,
         //:// which is the main key access to the entity.  If there are other TE_TablRecKey
         //:// entities, they are for Identifiers that are used only to create indexes and will
         //:// be handled further down.
         //:FOR EACH OldTE.TE_FieldDataRelKey
         RESULT = SetCursorFirstEntity( OldTE, "TE_FieldDataRelKey", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:// Loop through both OldTE2 and NewTE2 until position on the correct
            //:// TE_FieldDataRel
            //:SET CURSOR FIRST NewTE2.TE_FieldDataRel WITHIN NewTE2.TE_DBMS_Source
            RESULT = SetCursorFirstEntity( NewTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
            //:SET CURSOR FIRST OldTE2.TE_FieldDataRel WITHIN OldTE2.TE_DBMS_Source
            RESULT = SetCursorFirstEntity( OldTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
            //:FoundFlag = 0
            FoundFlag = 0;
            //:LOOP WHILE FoundFlag = 0 AND RESULT >= zCURSOR_SET
            while ( FoundFlag == 0 && RESULT >= zCURSOR_SET )
            { 
               //:IF OldTE2.TE_FieldDataRel.ZKey = OldTE.TE_FieldDataRelKey.ZKey
               if ( CompareAttributeToAttribute( OldTE2, "TE_FieldDataRel", "ZKey", OldTE, "TE_FieldDataRelKey", "ZKey" ) == 0 )
               { 
                  //:FoundFlag = 1
                  FoundFlag = 1;
                  //:ELSE
               } 
               else
               { 
                  //:SET CURSOR NEXT NewTE2.TE_FieldDataRel WITHIN NewTE2.TE_DBMS_Source
                  RESULT = SetCursorNextEntity( NewTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
                  //:SET CURSOR NEXT OldTE2.TE_FieldDataRel WITHIN OldTE2.TE_DBMS_Source
                  RESULT = SetCursorNextEntity( OldTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
               } 

               //:END
            } 

            //:END

            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:NewTE2.TE_FieldDataRel.Key = "Y"
               SetAttributeFromString( NewTE2, "TE_FieldDataRel", "Key", "Y" );
               //:INCLUDE NewTE.TE_FieldDataRelKey FROM NewTE2.TE_FieldDataRel
               RESULT = IncludeSubobjectFromSubobject( *NewTE, "TE_FieldDataRelKey", NewTE2, "TE_FieldDataRel", zPOS_AFTER );

               //:FOR EACH OldTE.TE_FieldDataRelAsFK
               RESULT = SetCursorFirstEntity( OldTE, "TE_FieldDataRelAsFK", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:// Loop on OldTE2 and NewTE2 as above
                  //:SET CURSOR FIRST NewTE2.TE_FieldDataRel WITHIN NewTE2.TE_DBMS_Source
                  RESULT = SetCursorFirstEntity( NewTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
                  //:SET CURSOR FIRST OldTE2.TE_FieldDataRel WITHIN OldTE2.TE_DBMS_Source
                  RESULT = SetCursorFirstEntity( OldTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
                  //:FoundFlag = 0
                  FoundFlag = 0;
                  //:LOOP WHILE FoundFlag = 0 AND RESULT >= zCURSOR_SET
                  while ( FoundFlag == 0 && RESULT >= zCURSOR_SET )
                  { 
                     //:IF OldTE2.TE_FieldDataRel.ZKey = OldTE.TE_FieldDataRelAsFK.ZKey
                     if ( CompareAttributeToAttribute( OldTE2, "TE_FieldDataRel", "ZKey", OldTE, "TE_FieldDataRelAsFK", "ZKey" ) == 0 )
                     { 
                        //:FoundFlag = 1
                        FoundFlag = 1;
                        //:ELSE
                     } 
                     else
                     { 
                        //:SET CURSOR NEXT NewTE2.TE_FieldDataRel WITHIN NewTE2.TE_DBMS_Source
                        RESULT = SetCursorNextEntity( NewTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
                        //:SET CURSOR NEXT OldTE2.TE_FieldDataRel WITHIN OldTE2.TE_DBMS_Source
                        RESULT = SetCursorNextEntity( OldTE2, "TE_FieldDataRel", "TE_DBMS_Source" );
                     } 

                     //:END
                  } 

                  //:END

                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:// We will only include the entry if it isn't already there.
                     //:SET CURSOR FIRST NewTE.TE_FieldDataRelAsFK WHERE
                     //:   NewTE.TE_FieldDataRelAsFK.ZKey = NewTE2.TE_FieldDataRel.ZKey
                     GetIntegerFromAttribute( &lTempInteger_8, NewTE2, "TE_FieldDataRel", "ZKey" );
                     RESULT = SetCursorFirstEntityByInteger( *NewTE, "TE_FieldDataRelAsFK", "ZKey", lTempInteger_8, "" );
                     //:IF RESULT < zCURSOR_SET
                     if ( RESULT < zCURSOR_SET )
                     { 
                        //:SET CURSOR LAST NewTE.TE_FieldDataRelAsFK
                        RESULT = SetCursorLastEntity( *NewTE, "TE_FieldDataRelAsFK", "" );
                        //:INCLUDE NewTE.TE_FieldDataRelAsFK FROM NewTE2.TE_FieldDataRel
                        RESULT = IncludeSubobjectFromSubobject( *NewTE, "TE_FieldDataRelAsFK", NewTE2, "TE_FieldDataRel", zPOS_AFTER );
                     } 

                     //:END

                     //:ELSE
                  } 
                  else
                  { 
                     //:// The following condition should not occur unless there is a
                     //:// logic error in this code.
                     //:MessageSend( vSubtask, "TE00505", "Technical Environment",
                     //:             "TE_FieldDataRelAsFK not found in new ERD.",
                     //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                     MessageSend( vSubtask, "TE00505", "Technical Environment", "TE_FieldDataRelAsFK not found in new ERD.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  } 

                  RESULT = SetCursorNextEntity( OldTE, "TE_FieldDataRelAsFK", "" );
                  //:END
               } 


               //:END

               //:ELSE
            } 
            else
            { 
               //:// The following condition should not occur unless there is a
               //:// logic error in this code.
               //:MessageSend( vSubtask, "TE00506", "Technical Environment",
               //:             "TE_FieldDataRelKey not found in new ERD.",
               //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vSubtask, "TE00506", "Technical Environment", "TE_FieldDataRelKey not found in new ERD.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            } 

            RESULT = SetCursorNextEntity( OldTE, "TE_FieldDataRelKey", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( OldTE, "TE_TablRec", "" );

         //:END
      } 


      //:   // Now process the rest of the TE_TablRecKey entries. For these, we will skip them
      //:   // if they already have a TE_FieldDataRelKey child, and if they don't, we will
      //:   // search the TE_FieldDataRel entries for the parent table to find a match. This is
      //:   // because any Identifier after the first should only reference an attribute in

      //:END

      //:DropView( NewTE2 )
      DropView( NewTE2 );
      //:DropView( OldTE2 )
      DropView( OldTE2 );
      RESULT = SetCursorNextEntity( OldTE, "TE_DBMS_Source", "" );
   } 


   //:END

   //:DropObjectInstance( OldTE )
   DropObjectInstance( OldTE );
   //:DropObjectInstance( OldERD )
   DropObjectInstance( OldERD );
   //:DropView( CurrentLPLR )
   DropView( CurrentLPLR );
   //:CommitMetaOI( vSubtask, NewTE, 6 )
   CommitMetaOI( vSubtask, *NewTE, 6 );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:TE_RelinkDelete( VIEW vTE  BASED ON LOD TZTENVRO, VIEW vSubtask )

//:   VIEW vERD    BASED ON LOD TZEREMDO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_TE_RelinkDelete( zVIEW     vTE,
                           zVIEW     vSubtask )
{
   zVIEW     vERD = 0; 
   //:VIEW vLPLR   BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 

   //:SHORT nDeleteFlag
   zSHORT    nDeleteFlag = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:STRING (254) szMsg
   zCHAR     szMsg[ 255 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zSHORT    lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zCHAR     szTempString_3[ 255 ]; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zCHAR     szTempString_5[ 33 ]; 
   zSHORT    lTempInteger_10; 
   zLONG     lTempInteger_11; 
   zCHAR     szTempString_6[ 33 ]; 


   //:nDeleteFlag = 0
   nDeleteFlag = 0;

   //:RetrieveViewForMetaList( vSubtask, vLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   //:// KJS 06/19/15 - I am getting an error in Relink code when vERD does not exist (which would be rare but can happen when a new project
   //:// is created but the ER has not yet been created.
   //:IF vLPLR.W_MetaDef DOES NOT EXIST 
   lTempInteger_0 = CheckExistenceOfEntity( vLPLR, "W_MetaDef" );
   if ( lTempInteger_0 != 0 )
   { 
      //:DropView( vLPLR ) 
      DropView( vLPLR );
      //:RETURN ( nDeleteFlag )
      return( ( nDeleteFlag ) );
   } 

   //:END

   //:nRC = ActivateMetaOI( vSubtask, vERD, vLPLR, zREFER_ERD_META, 0 )
   nRC = ActivateMetaOI( vSubtask, &vERD, vLPLR, zREFER_ERD_META, 0 );
   //:DropView( vLPLR )
   DropView( vLPLR );

   //:FOR EACH vTE.TE_DBMS_Source
   RESULT = SetCursorFirstEntity( vTE, "TE_DBMS_Source", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:FOR EACH vTE.TE_TablRec
      RESULT = SetCursorFirstEntity( vTE, "TE_TablRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:IF vTE.ER_Entity EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( vTE, "ER_Entity" );
         if ( lTempInteger_1 == 0 )
         { 
            //:SET CURSOR FIRST vERD.ER_Entity WHERE
            //:    vERD.ER_Entity.ZKey = vTE.ER_Entity.ZKey
            GetIntegerFromAttribute( &lTempInteger_2, vTE, "ER_Entity", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Entity", "ZKey", lTempInteger_2, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// Relink ER_Entity
               //:RelinkInstanceToInstance( vTE, "ER_Entity", vERD, "ER_Entity" )
               RelinkInstanceToInstance( vTE, "ER_Entity", vERD, "ER_Entity" );
               //:ELSE
            } 
            else
            { 
               //:SET CURSOR FIRST vERD.ER_Entity WHERE
               //:    vERD.ER_Entity.Name = vTE.ER_Entity.Name
               GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTE, "ER_Entity", "Name" );
               RESULT = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szTempString_0, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:EXCLUDE vTE.ER_Entity
                  RESULT = ExcludeEntity( vTE, "ER_Entity", zREPOS_AFTER );
                  //:INCLUDE vTE.ER_Entity FROM vERD.ER_Entity
                  RESULT = IncludeSubobjectFromSubobject( vTE, "ER_Entity", vERD, "ER_Entity", zPOS_AFTER );
                  //:ELSE
               } 
               else
               { 
                  //:szMsg = "The Entity, " + vTE.ER_Entity.Name + ", in the ER" +
                  //:        " for TE table, " + vTE.TE_TablRec.Name + ", has been" +
                  //:        " deleted." + NEW_LINE +
                  //:        "The TE table is thus being deleted."
                  GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vTE, "ER_Entity", "Name", "", 0 );
                  ZeidonStringCopy( szMsg, 1, 0, "The Entity, ", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ", in the ER", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, " for TE table, ", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_2, 0, 'S', 255, vTE, "TE_TablRec", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ", has been", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, " deleted.", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, "The TE table is thus being deleted.", 1, 0, 255 );
                  //:MessageSend( vTE, "TE00508", "Technical Environment",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vTE, "TE00508", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:DELETE ENTITY vTE.TE_TablRec NONE
                  RESULT = DeleteEntity( vTE, "TE_TablRec", zREPOS_NONE );
                  //:nDeleteFlag = 1
                  nDeleteFlag = 1;
               } 

               //:END
            } 

            //:END
         } 

         //:END

         //:IF vTE.ER_RelType EXISTS
         lTempInteger_3 = CheckExistenceOfEntity( vTE, "ER_RelType" );
         if ( lTempInteger_3 == 0 )
         { 
            //:SET CURSOR FIRST vERD.ER_RelType WHERE
            //:    vERD.ER_RelType.ZKey = vTE.ER_RelType.ZKey
            GetIntegerFromAttribute( &lTempInteger_4, vTE, "ER_RelType", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelType", "ZKey", lTempInteger_4, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// Relink ER_RelType
               //:RelinkInstanceToInstance( vTE, "ER_RelType", vERD, "ER_RelType" )
               RelinkInstanceToInstance( vTE, "ER_RelType", vERD, "ER_RelType" );
               //:ELSE
            } 
            else
            { 
               //:szMsg = "There is no relationship match in the ERD for " +
               //:        "Relalationship Table " + vTE.TE_TablRec.Name + "." +
               //:        NEW_LINE + "The Relationship Table is thus being deleted."
               ZeidonStringCopy( szMsg, 1, 0, "There is no relationship match in the ERD for ", 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, "Relalationship Table ", 1, 0, 255 );
               GetVariableFromAttribute( szTempString_3, 0, 'S', 255, vTE, "TE_TablRec", "Name", "", 0 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_3, 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, ".", 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, "The Relationship Table is thus being deleted.", 1, 0, 255 );
               //:MessageSend( vTE, "TE00509", "Technical Environment",
               //:             szMsg,
               //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vTE, "TE00509", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               //:DELETE ENTITY vTE.TE_TablRec NONE
               RESULT = DeleteEntity( vTE, "TE_TablRec", zREPOS_NONE );
               //:nDeleteFlag = 1
               nDeleteFlag = 1;
            } 

            //:END
         } 

         //:END

         //:IF vTE.TE_TablRec EXISTS
         lTempInteger_5 = CheckExistenceOfEntity( vTE, "TE_TablRec" );
         if ( lTempInteger_5 == 0 )
         { 

            //:FOR EACH vTE.TE_FieldDataRel
            RESULT = SetCursorFirstEntity( vTE, "TE_FieldDataRel", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 

               //:IF vTE.ER_Attribute EXISTS
               lTempInteger_6 = CheckExistenceOfEntity( vTE, "ER_Attribute" );
               if ( lTempInteger_6 == 0 )
               { 
                  //:SET CURSOR FIRST vERD.ER_Attribute WHERE
                  //:    vERD.ER_Attribute.ZKey = vTE.ER_Attribute.ZKey
                  GetIntegerFromAttribute( &lTempInteger_7, vTE, "ER_Attribute", "ZKey" );
                  RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Attribute", "ZKey", lTempInteger_7, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:// Relink ER_Attribute - Currently handled at end
                     //://RelinkInstanceToInstance( vTE, "ER_Attribute", vERD, "ER_Attribute" )
                     //://RelinkInstanceToInstance( vTE, "Domain", vERD, "Domain" )
                     //:ELSE
                  } 
                  else
                  { 
                     //:SET CURSOR FIRST vERD.ER_Attribute WHERE
                     //:    vERD.ER_Attribute.Name = vTE.ER_Attribute.Name
                     GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTE, "ER_Attribute", "Name" );
                     RESULT = SetCursorFirstEntityByString( vERD, "ER_Attribute", "Name", szTempString_0, "" );
                     //:IF RESULT >= zCURSOR_SET
                     if ( RESULT >= zCURSOR_SET )
                     { 
                        //:EXCLUDE vTE.ER_Attribute
                        RESULT = ExcludeEntity( vTE, "ER_Attribute", zREPOS_AFTER );
                        //:INCLUDE vTE.ER_Attribute FROM vERD.ER_Attribute
                        RESULT = IncludeSubobjectFromSubobject( vTE, "ER_Attribute", vERD, "ER_Attribute", zPOS_AFTER );
                        //:ELSE
                     } 
                     else
                     { 
                        //:szMsg = "The Attribute, " + vTE.ER_Attribute.Name + ", in the ER" +
                        //:        " for TE column, " + vTE.TE_FieldDataRel.Name + ", has been" +
                        //:        " deleted." + NEW_LINE +
                        //:        "The TE column is thus being deleted."
                        GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vTE, "ER_Attribute", "Name", "", 0 );
                        ZeidonStringCopy( szMsg, 1, 0, "The Attribute, ", 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, ", in the ER", 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, " for TE column, ", 1, 0, 255 );
                        GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vTE, "TE_FieldDataRel", "Name", "", 0 );
                        ZeidonStringConcat( szMsg, 1, 0, szTempString_4, 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, ", has been", 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, " deleted.", 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
                        ZeidonStringConcat( szMsg, 1, 0, "The TE column is thus being deleted.", 1, 0, 255 );
                        //:MessageSend( vTE, "TE00510", "Technical Environment",
                        //:             szMsg,
                        //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                        MessageSend( vTE, "TE00510", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                        //:DELETE ENTITY vTE.TE_FieldDataRel NONE
                        RESULT = DeleteEntity( vTE, "TE_FieldDataRel", zREPOS_NONE );
                        //:nDeleteFlag = 1
                        nDeleteFlag = 1;
                     } 

                     //:END
                  } 

                  //:END
               } 

               //:END

               //:IF vTE.ER_RelLink EXISTS
               lTempInteger_8 = CheckExistenceOfEntity( vTE, "ER_RelLink" );
               if ( lTempInteger_8 == 0 )
               { 
                  //:SET CURSOR FIRST vERD.ER_RelLink_2 WITHIN vERD.EntpER_Model WHERE
                  //:    vERD.ER_RelLink_2.ZKey = vTE.ER_RelLink.ZKey
                  GetIntegerFromAttribute( &lTempInteger_9, vTE, "ER_RelLink", "ZKey" );
                  RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelLink_2", "ZKey", lTempInteger_9, "EntpER_Model" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:// Relink ER_RelLink - Currently handled at end
                     //://RelinkInstanceToInstance( vTE, "ER_RelLink", vERD, "ER_RelLink_2" )
                     //://RelinkInstanceToInstance( vTE, "ER_RelType_O", vERD, "ER_RelType" )
                     //://RelinkInstanceToInstance( vTE, "ER_EntityRelLink", vERD, "ER_Entity_2" )
                     //:ELSE
                  } 
                  else
                  { 
                     //:szMsg = "There is no relationship match in the ERD for " +
                     //:        "Foreign Key " + vTE.TE_FieldDataRel.Name + "." +
                     //:        NEW_LINE + "The Foreign Key is thus being deleted."
                     ZeidonStringCopy( szMsg, 1, 0, "There is no relationship match in the ERD for ", 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, "Foreign Key ", 1, 0, 255 );
                     GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vTE, "TE_FieldDataRel", "Name", "", 0 );
                     ZeidonStringConcat( szMsg, 1, 0, szTempString_5, 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, ".", 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, "The Foreign Key is thus being deleted.", 1, 0, 255 );
                     //:        MessageSend( vTE, "TE00511", "Technical Environment",
                     //:                     szMsg,
                     //:                     zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vTE, "TE00511", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:DELETE ENTITY vTE.TE_FieldDataRel NONE
                     RESULT = DeleteEntity( vTE, "TE_FieldDataRel", zREPOS_NONE );
                     //:nDeleteFlag = 1
                     nDeleteFlag = 1;
                  } 

                  //:END
               } 

               RESULT = SetCursorNextEntity( vTE, "TE_FieldDataRel", "" );
               //:END
            } 


            //:END

            //:FOR EACH vTE.TE_TablRecKey
            RESULT = SetCursorFirstEntity( vTE, "TE_TablRecKey", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 

               //:IF vTE.ER_EntIdentifier EXISTS
               lTempInteger_10 = CheckExistenceOfEntity( vTE, "ER_EntIdentifier" );
               if ( lTempInteger_10 == 0 )
               { 
                  //:SET CURSOR FIRST vERD.ER_EntIdentifier WHERE
                  //:    vERD.ER_EntIdentifier.ZKey = vTE.ER_EntIdentifier.ZKey
                  GetIntegerFromAttribute( &lTempInteger_11, vTE, "ER_EntIdentifier", "ZKey" );
                  RESULT = SetCursorFirstEntityByInteger( vERD, "ER_EntIdentifier", "ZKey", lTempInteger_11, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:// Relink ER_EntIdentifier
                     //:RelinkInstanceToInstance( vTE, "ER_EntIdentifier", vERD, "ER_EntIdentifier" )
                     RelinkInstanceToInstance( vTE, "ER_EntIdentifier", vERD, "ER_EntIdentifier" );
                     //:ELSE
                  } 
                  else
                  { 
                     //:szMsg = "The Identifier, " + vTE.ER_EntIdentifier.Name + ", in the ER" +
                     //:        ", has been deleted." + NEW_LINE +
                     //:        "The corresponding key field in the TE is thus being deleted."
                     GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vTE, "ER_EntIdentifier", "Name", "", 0 );
                     ZeidonStringCopy( szMsg, 1, 0, "The Identifier, ", 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, szTempString_6, 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, ", in the ER", 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, ", has been deleted.", 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
                     ZeidonStringConcat( szMsg, 1, 0, "The corresponding key field in the TE is thus being deleted.", 1, 0, 255 );
                     //:MessageSend( vTE, "TE00512", "Technical Environment",
                     //:             szMsg,
                     //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vTE, "TE00512", "Technical Environment", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:DELETE ENTITY vTE.TE_TablRecKey NONE
                     RESULT = DeleteEntity( vTE, "TE_TablRecKey", zREPOS_NONE );
                     //:nDeleteFlag = 1
                     nDeleteFlag = 1;
                  } 

                  //:END
               } 

               RESULT = SetCursorNextEntity( vTE, "TE_TablRecKey", "" );
               //:END
            } 


            //:END
         } 

         RESULT = SetCursorNextEntity( vTE, "TE_TablRec", "" );

         //:END
      } 

      RESULT = SetCursorNextEntity( vTE, "TE_DBMS_Source", "" );

      //:END
   } 


   //:END

   //:// Now relink the ER_Attribute and ER_RelLink subobjects.  They have to be handled
   //:// with the RelinkAllSubobjectsForOI to correctly reset the include flags.  In the future
   //:// a version of RelinkInstanceToInstance may be created that can also reset the
   //:// include flag.  If so, the following will be deleted.

   //:RelinkAllSubobjectsForOI( vTE, "ER_Attribute", vERD, "ER_Attribute" )
   RelinkAllSubobjectsForOI( vTE, "ER_Attribute", vERD, "ER_Attribute" );
   //:RelinkAllSubobjectsForOI( vTE, "ER_RelLink", vERD, "ER_RelLink" )
   RelinkAllSubobjectsForOI( vTE, "ER_RelLink", vERD, "ER_RelLink" );

   //:DropMetaOI( vSubtask, vERD )
   DropMetaOI( vSubtask, vERD );

   //:RETURN( nDeleteFlag )
   return( ( nDeleteFlag ) );
// END
} 


//:LOCAL OPERATION
//:TE_MigrateOrgSironSubF( VIEW vSubtask,
//:                        VIEW NewTE BASED ON LOD TZTENVRO,
//:                        VIEW OldTE BASED ON LOD TZTENVRO,
//:                        VIEW NewWorkTE BASED ON LOD TZTENVRO,
//:                        VIEW OldWorkTE BASED ON LOD TZTENVRO )
//:   SHORT nRC
static zSHORT
oTZTENVRO_TE_MigrateOrgSironSubF( zVIEW     vSubtask,
                                  zVIEW     NewTE,
                                  zVIEW     OldTE,
                                  zVIEW     NewWorkTE,
                                  zVIEW     OldWorkTE )
{
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 


   //:SetViewToSubobject( OldTE, "OrigSironSubField" )
   SetViewToSubobject( OldTE, "OrigSironSubField" );
   //:SetViewToSubobject( NewTE, "OrigSironSubField" )
   SetViewToSubobject( NewTE, "OrigSironSubField" );

   //:// Duplicate OrigSironField and substructure.
   //:CreateMetaEntity( vSubtask, NewTE, "OrigSironField", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewTE, "OrigSironField", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewTE, "OrigSironField",
   //:                             OldTE, "OrigSironField", zSET_NULL )
   SetMatchingAttributesByName( NewTE, "OrigSironField", OldTE, "OrigSironField", zSET_NULL );
   //:FOR EACH OldTE.FieldComment
   RESULT = SetCursorFirstEntity( OldTE, "FieldComment", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: CreateMetaEntity( vSubtask, NewTE, "FieldComment", zPOS_AFTER )
      CreateMetaEntity( vSubtask, NewTE, "FieldComment", zPOS_AFTER );
      //: SetMatchingAttributesByName( NewTE, "FieldComment",
      //:                              OldTE, "FieldComment", zSET_NULL )
      SetMatchingAttributesByName( NewTE, "FieldComment", OldTE, "FieldComment", zSET_NULL );
      RESULT = SetCursorNextEntity( OldTE, "FieldComment", "" );
   } 

   //:END

   //:nRC = SetCursorFirstEntityByEntityCsr( OldWorkTE,
   //:                                       "SironField",
   //:                                       OldTE,
   //:                                       "OrigSironField",
   //:                                       "TE_DBMS_Source" )
   nRC = SetCursorFirstEntityByEntityCsr( OldWorkTE, "SironField", OldTE, "OrigSironField", "TE_DBMS_Source" );

   //:IF nRC >= zCURSOR_SET
   if ( nRC >= zCURSOR_SET )
   { 
      //:// Include the SironField under the correct FieldDataRel
      //:SET CURSOR FIRST NewWorkTE.TE_TablRec WHERE
      //:        NewWorkTE.TE_TablRec.Name = OldWorkTE.TE_TablRec.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldWorkTE, "TE_TablRec", "Name" );
      RESULT = SetCursorFirstEntityByString( NewWorkTE, "TE_TablRec", "Name", szTempString_0, "" );
      //:SET CURSOR FIRST NewWorkTE.TE_FieldDataRel WHERE
      //:        NewWorkTE.TE_FieldDataRel.Name = OldWorkTE.TE_FieldDataRel.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldWorkTE, "TE_FieldDataRel", "Name" );
      RESULT = SetCursorFirstEntityByString( NewWorkTE, "TE_FieldDataRel", "Name", szTempString_0, "" );

      //:INCLUDE NewWorkTE.SironField FROM NewTE.OrigSironField
      RESULT = IncludeSubobjectFromSubobject( NewWorkTE, "SironField", NewTE, "OrigSironField", zPOS_AFTER );
      //:ELSE
   } 
   else
   { 
      //:// Include the SironGroup under the correct TablRec
      //:nRC = SetCursorFirstEntityByEntityCsr( OldWorkTE,
      //:                                       "SironGroup",
      //:                                       OldTE,
      //:                                       "OrigSironField",
      //:                                       "TE_DBMS_Source" )
      nRC = SetCursorFirstEntityByEntityCsr( OldWorkTE, "SironGroup", OldTE, "OrigSironField", "TE_DBMS_Source" );
      //:IF nRC >= zCURSOR_SET
      if ( nRC >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST NewWorkTE.TE_TablRec WHERE
         //:        NewWorkTE.TE_TablRec.Name = OldWorkTE.TE_TablRec.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldWorkTE, "TE_TablRec", "Name" );
         RESULT = SetCursorFirstEntityByString( NewWorkTE, "TE_TablRec", "Name", szTempString_0, "" );

         //:INCLUDE NewWorkTE.SironGroup FROM NewTE.OrigSironField
         RESULT = IncludeSubobjectFromSubobject( NewWorkTE, "SironGroup", NewTE, "OrigSironField", zPOS_AFTER );
      } 

      //:END
   } 

   //:END

   //:FOR EACH OldTE.OrigSironSubField
   RESULT = SetCursorFirstEntity( OldTE, "OrigSironSubField", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TE_MigrateOrgSironSubF( vSubtask, NewTE, OldTE, NewWorkTE, OldWorkTE )
      oTZTENVRO_TE_MigrateOrgSironSubF( vSubtask, NewTE, OldTE, NewWorkTE, OldWorkTE );
      RESULT = SetCursorNextEntity( OldTE, "OrigSironSubField", "" );
   } 

   //:END

   //:ResetViewFromSubobject( NewTE )
   ResetViewFromSubobject( NewTE );
   //:ResetViewFromSubobject( OldTE )
   ResetViewFromSubobject( OldTE );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SyncTablRecKeyRecurs( VIEW vDTE        BASED ON LOD TZTENVRO,
//:                      VIEW vDTE_Recurs BASED ON LOD TZTENVRO,
//:                      VIEW vERD        BASED ON LOD TZEREMDO )

//:   VIEW vDTE_Recurs2 BASED ON LOD TZTENVRO
static zVOID
oTZTENVRO_SyncTablRecKeyRecurs( zVIEW     vDTE,
                                zVIEW     vDTE_Recurs,
                                zVIEW     vERD )
{
   zVIEW     vDTE_Recurs2 = 0; 
   //:VIEW vERD2       BASED ON LOD TZEREMDO
   zVIEW     vERD2 = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 


   //:// This routine creates TE_FieldDataRelKey entries for each relationship defined in
   //:// an identifier.
   //:// Position on the source of the relationship and use its keys.

   //:// Create new vDTE and vERD views, so we don't alter the caller's vDTE_Recurs and vERD.
   //:CreateViewFromViewForTask( vERD2, vERD, 0 )
   CreateViewFromViewForTask( &vERD2, vERD, 0 );
   //:CreateViewFromViewForTask( vDTE_Recurs2, vDTE_Recurs, 0 )
   CreateViewFromViewForTask( &vDTE_Recurs2, vDTE_Recurs, 0 );

   //:SET CURSOR FIRST vERD2.ER_Entity WHERE
   //:                 vERD2.ER_Entity.ZKey = vERD.ER_Entity_Other_Identifier.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vERD, "ER_Entity_Other_Identifier", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vERD2, "ER_Entity", "ZKey", lTempInteger_0, "" );

   //:// Include each field that is a part of the key
   //:FOR EACH vERD2.ER_AttributeIdentifier WITHIN vERD2.ER_EntIdentifier
   RESULT = SetCursorFirstEntity( vERD2, "ER_AttributeIdentifier", "ER_EntIdentifier" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vDTE_Recurs2.ER_Attribute WITHIN vDTE_Recurs2.TE_DB_Environ WHERE
      //:                 vDTE_Recurs2.ER_Attribute.ZKey = vERD2.ER_AttributeIdentifier.ZKey
      GetIntegerFromAttribute( &lTempInteger_1, vERD2, "ER_AttributeIdentifier", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vDTE_Recurs2, "ER_Attribute", "ZKey", lTempInteger_1, "TE_DB_Environ" );
      //:vDTE_Recurs2.TE_FieldDataRel.Key = "Y"
      SetAttributeFromString( vDTE_Recurs2, "TE_FieldDataRel", "Key", "Y" );
      //:INCLUDE vDTE.TE_FieldDataRelKey FROM vDTE_Recurs2.TE_FieldDataRel
      RESULT = IncludeSubobjectFromSubobject( vDTE, "TE_FieldDataRelKey", vDTE_Recurs2, "TE_FieldDataRel", zPOS_AFTER );
      RESULT = SetCursorNextEntity( vERD2, "ER_AttributeIdentifier", "ER_EntIdentifier" );
   } 

   //:END

   //:// Now follow any relationship paths to get what will be foreign keys.
   //:FOR EACH vERD2.ER_RelLinkIdentifier WITHIN vERD2.ER_EntIdentifier
   RESULT = SetCursorFirstEntity( vERD2, "ER_RelLinkIdentifier", "ER_EntIdentifier" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SyncTablRecKeyRecurs( vDTE, vDTE_Recurs2, vERD2 )
      oTZTENVRO_SyncTablRecKeyRecurs( vDTE, vDTE_Recurs2, vERD2 );
      RESULT = SetCursorNextEntity( vERD2, "ER_RelLinkIdentifier", "ER_EntIdentifier" );
   } 

   //:END

   //:DropView( vDTE_Recurs2 )
   DropView( vDTE_Recurs2 );
   //:DropView( vERD2 )
   DropView( vERD2 );
   return;
// END
} 


//:TRANSFORMATION OPERATION
//:SetFieldSequence( VIEW vTZTENVRO BASED ON LOD TZTENVRO )
//:  VIEW vTZTENVRO_Copy BASED ON LOD TZTENVRO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_SetFieldSequence( zVIEW     vTZTENVRO )
{
   zVIEW     vTZTENVRO_Copy = 0; 
   //:INTEGER lSequence
   zLONG     lSequence = 0; 
   zSHORT    RESULT; 


   //:CreateViewFromViewForTask( vTZTENVRO_Copy, vTZTENVRO, 0 )
   CreateViewFromViewForTask( &vTZTENVRO_Copy, vTZTENVRO, 0 );

   //:FOR EACH vTZTENVRO_Copy.TE_DBMS_Source
   RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_DBMS_Source", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:FOR EACH vTZTENVRO_Copy.TE_TablRec
      RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_TablRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: lSequence = 1
         lSequence = 1;
         //: FOR EACH vTZTENVRO_Copy.TE_FieldDataRel
         RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:  vTZTENVRO_Copy.TE_FieldDataRel.wkSequence = lSequence
            SetAttributeFromInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lSequence );
            //:  lSequence = lSequence + 1
            lSequence = lSequence + 1;
            RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
         } 

         RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_TablRec", "" );
         //: END
      } 

      RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_DBMS_Source", "" );
      //:END
   } 

   //:END

   //:DropView( vTZTENVRO_Copy )
   DropView( vTZTENVRO_Copy );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SortFields( VIEW vTZTENVRO BASED ON LOD TZTENVRO, VIEW vSubtask )
//:   VIEW vWindow
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_SortFields( zVIEW     vTZTENVRO,
                      zVIEW     vSubtask )
{
   zVIEW     vWindow = 0; 
   //:VIEW vTZTENVRO_Copy BASED ON LOD TZTENVRO
   zVIEW     vTZTENVRO_Copy = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:CreateViewFromViewForTask( vTZTENVRO_Copy, vTZTENVRO, 0 )
   CreateViewFromViewForTask( &vTZTENVRO_Copy, vTZTENVRO, 0 );
   //:FOR EACH vTZTENVRO_Copy.TE_DBMS_Source
   RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_DBMS_Source", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: FOR EACH vTZTENVRO_Copy.TE_TablRec
      RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_TablRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:  IF vTZTENVRO.TE_FieldDataRel EXISTS AND vTZTENVRO.TE_FieldDataRel.wkSequence > 0
         lTempInteger_0 = CheckExistenceOfEntity( vTZTENVRO, "TE_FieldDataRel" );
         if ( lTempInteger_0 == 0 && CompareAttributeToInteger( vTZTENVRO, "TE_FieldDataRel", "wkSequence", 0 ) > 0 )
         { 
            //:  OrderEntityForView( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence A" )
            OrderEntityForView( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence A" );
         } 

         RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_TablRec", "" );
         //:  END
      } 

      RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_DBMS_Source", "" );
      //: END
   } 

   //:END
   //:DropView( vTZTENVRO_Copy )
   DropView( vTZTENVRO_Copy );

   //:GetSubtaskForWindowName( vSubtask, vWindow, "TableRecord" )
   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   //:IF vWindow > 0
   if ( vWindow > 0 )
   { 
      //:RefreshCtrl( vWindow, "lbColumnList" )
      RefreshCtrl( vWindow, "lbColumnList" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:NewFieldSequence( VIEW vTZTENVRO BASED ON LOD TZTENVRO )
//:   VIEW    vTZTENVRO_Copy BASED ON LOD TZTENVRO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_NewFieldSequence( zVIEW     vTZTENVRO )
{
   zVIEW     vTZTENVRO_Copy = 0; 
   //:INTEGER lSequenceNo
   zLONG     lSequenceNo = 0; 
   //:INTEGER lNewSequenceNo
   zLONG     lNewSequenceNo = 0; 
   zSHORT    RESULT; 


   //:IF vTZTENVRO.TE_FieldDataRel.wkSequence = ""
   if ( CompareAttributeToString( vTZTENVRO, "TE_FieldDataRel", "wkSequence", "" ) == 0 )
   { 

      //:CreateViewFromViewForTask( vTZTENVRO_Copy, vTZTENVRO, 0 )
      CreateViewFromViewForTask( &vTZTENVRO_Copy, vTZTENVRO, 0 );

      //:// set cursor on previous field and get the sequence number
      //:SET CURSOR PREVIOUS vTZTENVRO_Copy.TE_FieldDataRel
      RESULT = SetCursorPrevEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
      //:IF RESULT = zCURSOR_SET
      if ( RESULT == zCURSOR_SET )
      { 
         //:lSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
         GetIntegerFromAttribute( &lSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
         //:lSequenceNo = lSequenceNo + 1
         lSequenceNo = lSequenceNo + 1;
         //:ELSE
      } 
      else
      { 
         //:lSequenceNo = 1
         lSequenceNo = 1;
      } 

      //:END

      //:// set new sequence number for the field with greater number
      //:FOR EACH vTZTENVRO_Copy.TE_FieldDataRel
      RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: IF vTZTENVRO_Copy.TE_FieldDataRel.wkSequence >= lSequenceNo
         if ( CompareAttributeToInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lSequenceNo ) >= 0 )
         { 
            //: lNewSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
            GetIntegerFromAttribute( &lNewSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
            //: lNewSequenceNo = lNewSequenceNo + 1
            lNewSequenceNo = lNewSequenceNo + 1;
            //: vTZTENVRO_Copy.TE_FieldDataRel.wkSequence = lNewSequenceNo
            SetAttributeFromInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lNewSequenceNo );
         } 

         RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
         //:END
      } 

      //:END

      //:// set sequence number for new attribute
      //:vTZTENVRO.TE_FieldDataRel.wkSequence  = lSequenceNo
      SetAttributeFromInteger( vTZTENVRO, "TE_FieldDataRel", "wkSequence", lSequenceNo );

      //:DropView( vTZTENVRO_Copy )
      DropView( vTZTENVRO_Copy );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DelFieldFromSequence( VIEW vTZTENVRO BASED ON LOD TZTENVRO )
//:   VIEW    vTZTENVRO_Copy BASED ON LOD TZTENVRO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_DelFieldFromSequence( zVIEW     vTZTENVRO )
{
   zVIEW     vTZTENVRO_Copy = 0; 
   //:INTEGER lSequenceNo
   zLONG     lSequenceNo = 0; 
   //:INTEGER lNewSequenceNo
   zLONG     lNewSequenceNo = 0; 
   zSHORT    RESULT; 


   //:IF vTZTENVRO.TE_FieldDataRel.wkSequence > 0
   if ( CompareAttributeToInteger( vTZTENVRO, "TE_FieldDataRel", "wkSequence", 0 ) > 0 )
   { 

      //:CreateViewFromViewForTask( vTZTENVRO_Copy, vTZTENVRO, 0 )
      CreateViewFromViewForTask( &vTZTENVRO_Copy, vTZTENVRO, 0 );

      //:lSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
      GetIntegerFromAttribute( &lSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
      //:// set new sequence number for the field with greater number
      //:FOR EACH vTZTENVRO_Copy.TE_FieldDataRel
      RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: IF vTZTENVRO_Copy.TE_FieldDataRel.wkSequence > lSequenceNo
         if ( CompareAttributeToInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lSequenceNo ) > 0 )
         { 
            //: lNewSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
            GetIntegerFromAttribute( &lNewSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
            //: lNewSequenceNo = lNewSequenceNo - 1
            lNewSequenceNo = lNewSequenceNo - 1;
            //: vTZTENVRO_Copy.TE_FieldDataRel.wkSequence = lNewSequenceNo
            SetAttributeFromInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lNewSequenceNo );
         } 

         RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
         //:END
      } 

      //:END

      //:DropView( vTZTENVRO_Copy )
      DropView( vTZTENVRO_Copy );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:MoveFieldInSequence( VIEW vTZTENVRO BASED ON LOD TZTENVRO )
//:   VIEW    vTZTENVRO_Copy BASED ON LOD TZTENVRO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_MoveFieldInSequence( zVIEW     vTZTENVRO )
{
   zVIEW     vTZTENVRO_Copy = 0; 
   //:INTEGER lSequenceNo
   zLONG     lSequenceNo = 0; 
   //:INTEGER lOldSequenceNo
   zLONG     lOldSequenceNo = 0; 
   //:INTEGER lNewSequenceNo
   zLONG     lNewSequenceNo = 0; 
   zSHORT    RESULT; 


   //:lOldSequenceNo = vTZTENVRO.TE_TablRec.wkSelectedFieldForDragSort
   GetIntegerFromAttribute( &lOldSequenceNo, vTZTENVRO, "TE_TablRec", "wkSelectedFieldForDragSort" );

   //:IF lOldSequenceNo > 0
   if ( lOldSequenceNo > 0 )
   { 

      //:CreateViewFromViewForTask( vTZTENVRO_Copy, vTZTENVRO, 0 )
      CreateViewFromViewForTask( &vTZTENVRO_Copy, vTZTENVRO, 0 );

      //:// set cursor on previous field and get the sequence number
      //:SET CURSOR PREVIOUS vTZTENVRO_Copy.TE_FieldDataRel
      RESULT = SetCursorPrevEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
      //:IF RESULT = zCURSOR_SET
      if ( RESULT == zCURSOR_SET )
      { 
         //:lNewSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
         GetIntegerFromAttribute( &lNewSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
         //:IF lNewSequenceNo <= lOldSequenceNo
         if ( lNewSequenceNo <= lOldSequenceNo )
         { 
            //:lNewSequenceNo = lNewSequenceNo + 1
            lNewSequenceNo = lNewSequenceNo + 1;
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:lNewSequenceNo = 1
         lNewSequenceNo = 1;
      } 

      //:END

      //:// set new sequence number for the field with greater number
      //:FOR EACH vTZTENVRO_Copy.TE_FieldDataRel
      RESULT = SetCursorFirstEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF lNewSequenceNo < lOldSequenceNo
         //:   AND vTZTENVRO_Copy.TE_FieldDataRel.wkSequence >= lNewSequenceNo
         //:   AND vTZTENVRO_Copy.TE_FieldDataRel.wkSequence <= lOldSequenceNo
         if ( lNewSequenceNo < lOldSequenceNo && CompareAttributeToInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lNewSequenceNo ) >= 0 && CompareAttributeToInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lOldSequenceNo ) <= 0 )
         { 
            //:lSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
            GetIntegerFromAttribute( &lSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
            //:lSequenceNo = lSequenceNo + 1
            lSequenceNo = lSequenceNo + 1;
            //:vTZTENVRO_Copy.TE_FieldDataRel.wkSequence = lSequenceNo
            SetAttributeFromInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lSequenceNo );
            //:ELSE
         } 
         else
         { 
            //:IF lNewSequenceNo > lOldSequenceNo
            //:   AND vTZTENVRO_Copy.TE_FieldDataRel.wkSequence >= lOldSequenceNo
            //:   AND vTZTENVRO_Copy.TE_FieldDataRel.wkSequence <= lNewSequenceNo
            if ( lNewSequenceNo > lOldSequenceNo && CompareAttributeToInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lOldSequenceNo ) >= 0 && CompareAttributeToInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lNewSequenceNo ) <= 0 )
            { 
               //:lSequenceNo = vTZTENVRO_Copy.TE_FieldDataRel.wkSequence
               GetIntegerFromAttribute( &lSequenceNo, vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence" );
               //:lSequenceNo = lSequenceNo - 1
               lSequenceNo = lSequenceNo - 1;
               //:vTZTENVRO_Copy.TE_FieldDataRel.wkSequence = lSequenceNo
               SetAttributeFromInteger( vTZTENVRO_Copy, "TE_FieldDataRel", "wkSequence", lSequenceNo );
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( vTZTENVRO_Copy, "TE_FieldDataRel", "" );
         //:END
      } 

      //:END

      //:// set sequence number for new field
      //:vTZTENVRO.TE_FieldDataRel.wkSequence = lNewSequenceNo
      SetAttributeFromInteger( vTZTENVRO, "TE_FieldDataRel", "wkSequence", lNewSequenceNo );

      //:DropView( vTZTENVRO_Copy )
      DropView( vTZTENVRO_Copy );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SetFieldDataRelKeyFlag( VIEW vTZTENVRO BASED ON LOD TZTENVRO )

//:   VIEW vTZTENVRO1 BASED ON LOD TZTENVRO
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_SetFieldDataRelKeyFlag( zVIEW     vTZTENVRO )
{
   zVIEW     vTZTENVRO1 = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 


   //:CreateViewFromViewForTask( vTZTENVRO1, vTZTENVRO, 0 )
   CreateViewFromViewForTask( &vTZTENVRO1, vTZTENVRO, 0 );

   //:FOR EACH vTZTENVRO1.TE_FieldDataRel
   RESULT = SetCursorFirstEntity( vTZTENVRO1, "TE_FieldDataRel", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //: SET CURSOR FIRST vTZTENVRO1.TE_FieldDataRelKey WITHIN vTZTENVRO1.TE_TablRec
      //:     WHERE vTZTENVRO1.TE_FieldDataRelKey.ZKey = vTZTENVRO1.TE_FieldDataRel.ZKey
      GetIntegerFromAttribute( &lTempInteger_0, vTZTENVRO1, "TE_FieldDataRel", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vTZTENVRO1, "TE_FieldDataRelKey", "ZKey", lTempInteger_0, "TE_TablRec" );

      //: IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //: vTZTENVRO1.TE_FieldDataRel.Key = "Y"
         SetAttributeFromString( vTZTENVRO1, "TE_FieldDataRel", "Key", "Y" );
         //:ELSE
      } 
      else
      { 
         //: // If FieldType = Attribute (D), set Key=No, else empty
         //: IF vTZTENVRO1.TE_FieldDataRel.DataOrRelfieldOrSet = "D"
         if ( CompareAttributeToString( vTZTENVRO1, "TE_FieldDataRel", "DataOrRelfieldOrSet", "D" ) == 0 )
         { 
            //: vTZTENVRO1.TE_FieldDataRel.Key = "N"
            SetAttributeFromString( vTZTENVRO1, "TE_FieldDataRel", "Key", "N" );
            //:ELSE
         } 
         else
         { 
            //: vTZTENVRO1.TE_FieldDataRel.Key = ""
            SetAttributeFromString( vTZTENVRO1, "TE_FieldDataRel", "Key", "" );
         } 

         //: END
      } 

      RESULT = SetCursorNextEntity( vTZTENVRO1, "TE_FieldDataRel", "" );
      //: END
   } 

   //:END // ENDFOR vTZTENVRO1.TE_FieldDataRel

   //:DropView( vTZTENVRO1 )
   DropView( vTZTENVRO1 );
   return( 0 );
// END
} 


//:ENTITY CONSTRAINT OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_Accept_FieldDataRelKey( zVIEW     vTZTENVRO,
                                  zPCHAR    szEntityName,
                                  zSHORT    Event,
                                  zSHORT    State )
{

   //:Accept_FieldDataRelKey( VIEW vTZTENVRO BASED ON LOD TZTENVRO,
   //:                     STRING ( 32 )  szEntityName,
   //:                     SHORT          Event,
   //:                     SHORT          State )

   //:CASE Event
   switch( Event )
   { 
      //:OF   zECE_ACCEPT:
      case zECE_ACCEPT :

         //:SetFieldDataRelKeyFlag( vTZTENVRO )
         oTZTENVRO_SetFieldDataRelKeyFlag( vTZTENVRO );
         break ;

      //:  /* end zECE_ACCEPT */
      //:OF   zECE_CANCEL:
      case zECE_CANCEL :
         break ;

      //:  /* end zECE_CANCEL */
      //:OF   zECE_CREATE:
      case zECE_CREATE :
         break ;

      //:  /* end zECE_CREATE */
      //:OF   zECE_DELETE:
      case zECE_DELETE :
         break ;

      //:  /* end zECE_DELETE */
      //:OF   zECE_EXCLUDE:
      case zECE_EXCLUDE :
         break ;

      //:  /* end zECE_EXCLUDE */
      //:OF   zECE_INCLUDE:
      case zECE_INCLUDE :
         break ;
   } 


   //:     /* end zECE_INCLUDE */
   //:END  /* case */
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
