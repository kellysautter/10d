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
oTZZOLODO_fnRelinkWorkAttrib( zVIEW     vLOD,
                              zVIEW     DomainLPLR,
                              zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DIL_NLS_Text( zVIEW     vLOD,
                        LPVIEWENTITY lpEntity,
                        LPVIEWATTRIB lpAttribute,
                        zSHORT    GetOrSetFlag );


static zSHORT
oTZZOLODO_fnLocateParentName( zVIEW     vRecursLOD,
                              zLONG     RelationshipZKey,
                              zLONG     ChildEntityZKey,
                              zPCHAR    ReturnedParentName );


static zSHORT
oTZZOLODO_fnRelinkAttributes( zVIEW     vSubtask,
                              zVIEW     vLOD,
                              zVIEW     vERD,
                              zVIEW     DomainLPLR );


static zSHORT
oTZZOLODO_fnRelinkRelationships( zVIEW     vSubtask,
                                 zVIEW     vLOD,
                                 zVIEW     vERD );


static zSHORT
oTZZOLODO_GetRelationshipName( zVIEW     SourceLPLR,
                               zVIEW     OldLOD,
                               zPCHAR    RelationshipName,
                               zPCHAR    ParentName );


static zSHORT
oTZZOLODO_GetERAttributeName( zVIEW     SourceLPLR,
                              zVIEW     OldLOD,
                              zPCHAR    ERAttributeName );


static zSHORT
oTZZOLODO_fnCopyPOD_Entity( zVIEW     vSubtask,
                            zVIEW     NewLOD,
                            zVIEW     OldLOD,
                            zVIEW     NewDTE );


zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_dAttributeTextMapName( zVIEW     TZZOLODO,
                                 LPVIEWENTITY InternalEntityStructure,
                                 LPVIEWATTRIB InternalAttribStructure,
                                 zSHORT    GetOrSetFlag );


static zSHORT
oTZZOLODO_LOD_CompareReus( zVIEW     TargetLOD,
                           zVIEW     SourceLOD,
                           zVIEW     TargetLOD_Root,
                           zPCHAR    szParentNames );


static zSHORT
oTZZOLODO_LOD_CompareOperations( zVIEW     TargetLOD,
                                 zVIEW     SourceLOD );


static zSHORT
oTZZOLODO_LOD_CompareEntity( zVIEW     TargetLOD,
                             zVIEW     SourceLOD,
                             zVIEW     TargetLOD_Root );


static zSHORT
oTZZOLODO_LOD_MergeEntity( zVIEW     TargetLOD,
                           zVIEW     SourceLOD,
                           zVIEW     TargetLOD_Root,
                           zVIEW     SourceLOD_Root,
                           zVIEW     SourceLPLR,
                           zVIEW     CurrentLPLR,
                           zVIEW     NewERD,
                           zVIEW     SourceERD,
                           zVIEW     vSubtask );


//:TRANSFORMATION OPERATION
//:LOD_EntityCopy( VIEW        NewMainLOD      BASED ON LOD TZZOLODO,
//:                VIEW        NewRecursiveLOD BASED ON LOD TZZOLODO,
//:                VIEW        OldMainLOD      BASED ON LOD TZZOLODO,
//:                VIEW        OldRecursiveLOD BASED ON LOD TZZOLODO,
//:                VIEW        SourceLPLR      BASED ON LOD TZCMLPLO,
//:                VIEW        CurrentLPLR     BASED ON LOD TZCMLPLO,
//:                VIEW        NewERD          BASED ON LOD TZEREMDO,
//:                VIEW        SourceERD       BASED ON LOD TZEREMDO,
//:                VIEW        NewDTE          BASED ON LOD TZTENVRO,
//:                VIEW        vSubtask,
//:                STRING (32) ParentName,
//:                STRING ( 1 ) szAddER_EntityFlag )

//:/*  This is actually the main part of LOD Migration that copies the Entity subobject.  */

//:   VIEW NewERD2   BASED ON LOD TZEREMDO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_EntityCopy( zVIEW     NewMainLOD,
                          zVIEW     NewRecursiveLOD,
                          zVIEW     OldMainLOD,
                          zVIEW     OldRecursiveLOD,
                          zVIEW     SourceLPLR,
                          zVIEW     CurrentLPLR,
                          zVIEW     NewERD,
                          zVIEW     SourceERD,
                          zVIEW     NewDTE,
                          zVIEW     vSubtask,
                          zPCHAR    ParentName,
                          zPCHAR    szAddER_EntityFlag )
{
   zVIEW     NewERD2 = 0; 
   //:VIEW TempLOD   BASED ON LOD TZZOLODO
   zVIEW     TempLOD = 0; 

   //:SHORT      ErrorFlag
   zSHORT    ErrorFlag = 0; 
   //:SHORT      FoundFlag
   zSHORT    FoundFlag = 0; 
   //:SHORT      CreateIdentifiersFlag
   zSHORT    CreateIdentifiersFlag = 0; 
   //:SHORT      n
   zSHORT    n = 0; 
   //:SHORT      nRC
   zSHORT    nRC = 0; 
   //:STRING (254) szMsg
   zCHAR     szMsg[ 255 ] = { 0 }; 
   //:STRING (32)  OrigParentName
   zCHAR     OrigParentName[ 33 ] = { 0 }; 
   //:STRING (32)  RelationshipName
   zCHAR     RelationshipName[ 33 ] = { 0 }; 
   //:STRING (32)  EREntityName
   zCHAR     EREntityName[ 33 ] = { 0 }; 
   //:STRING (32)  ERAttributeName
   zCHAR     ERAttributeName[ 33 ] = { 0 }; 
   //:STRING (32)  DomainName
   zCHAR     DomainName[ 33 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    lTempInteger_6; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zSHORT    lTempInteger_7; 
   zSHORT    lTempInteger_8; 
   zCHAR     szTempString_8[ 33 ]; 
   zSHORT    lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zSHORT    lTempInteger_11; 
   zSHORT    lTempInteger_12; 
   zSHORT    lTempInteger_13; 
   zSHORT    lTempInteger_14; 


   //:ErrorFlag = 0
   ErrorFlag = 0;

   //:SET CURSOR LAST NewRecursiveLOD.LOD_EntityParent
   RESULT = SetCursorLastEntity( NewRecursiveLOD, "LOD_EntityParent", "" );
   //:CreateMetaEntity( vSubtask, NewRecursiveLOD, "LOD_EntityParent", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewRecursiveLOD, "LOD_EntityParent", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewRecursiveLOD, "LOD_EntityParent", OldRecursiveLOD,
   //:                             "LOD_EntityParent", zSET_NULL )
   SetMatchingAttributesByName( NewRecursiveLOD, "LOD_EntityParent", OldRecursiveLOD, "LOD_EntityParent", zSET_NULL );

   //:INCLUDE NewMainLOD.LOD_Entity FROM NewRecursiveLOD.LOD_EntityParent
   RESULT = IncludeSubobjectFromSubobject( NewMainLOD, "LOD_Entity", NewRecursiveLOD, "LOD_EntityParent", zPOS_AFTER );

   //:// Copy POD_Entities (Join Definitions), if there is a DTE.
   //:IF NewDTE != 0
   if ( NewDTE != 0 )
   { 
      //:fnCopyPOD_Entity( vSubtask, NewRecursiveLOD, OldRecursiveLOD, NewDTE )
      oTZZOLODO_fnCopyPOD_Entity( vSubtask, NewRecursiveLOD, OldRecursiveLOD, NewDTE );
   } 

   //:END

   //:// Process ER Entity for LOD, if it exists.
   //:IF OldRecursiveLOD.ER_EntityRec EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_EntityRec" );
   if ( lTempInteger_0 == 0 )
   { 

      //:// Always position on the ER_Entity in the Source ERD, if it exists. Otherwise skip the code.
      //:IF SourceERD != 0
      if ( SourceERD != 0 )
      { 

         //:// Position on entity in Source ERD.
         //:EREntityName = OldRecursiveLOD.ER_EntityRec.Name
         GetVariableFromAttribute( EREntityName, 0, 'S', 33, OldRecursiveLOD, "ER_EntityRec", "Name", "", 0 );
         //:SET CURSOR FIRST SourceERD.ER_Entity WHERE SourceERD.ER_Entity.Name = EREntityName
         RESULT = SetCursorFirstEntityByString( SourceERD, "ER_Entity", "Name", EREntityName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:szMsg = "ER Entity (" + EREntityName +
            //:        ") does not exist in source ERD." +
            //:        NEW_LINE + "LOD (" + OldMainLOD.LOD.Name +
            //:        ") will not be migrated."
            ZeidonStringCopy( szMsg, 1, 0, "ER Entity (", 1, 0, 255 );
            ZeidonStringConcat( szMsg, 1, 0, EREntityName, 1, 0, 255 );
            ZeidonStringConcat( szMsg, 1, 0, ") does not exist in source ERD.", 1, 0, 255 );
            ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
            ZeidonStringConcat( szMsg, 1, 0, "LOD (", 1, 0, 255 );
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, OldMainLOD, "LOD", "Name", "", 0 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 255 );
            ZeidonStringConcat( szMsg, 1, 0, ") will not be migrated.", 1, 0, 255 );
            //:MessageSend( vSubtask, "ZO00404", "Entity Copy",
            //:             szMsg,
            //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "ZO00404", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:DELETE ENTITY NewRecursiveLOD.LOD_EntityParent NONE
            RESULT = DeleteEntity( NewRecursiveLOD, "LOD_EntityParent", zREPOS_NONE );
            //:RETURN 0
            return( 0 );
         } 

         //:END

         //:// If the source ER_Entity is not in the target, we will add the Entity and Relationship to the ER.
         //:// Note that we won't do this if we didn't pass in a source ER.
         //:SET CURSOR FIRST NewERD.ER_Entity WHERE NewERD.ER_Entity.Name = EREntityName
         RESULT = SetCursorFirstEntityByString( NewERD, "ER_Entity", "Name", EREntityName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 

            //:// Create ER Entity from LOD Entity. Create entity in alphabetical order.
            //:SET CURSOR FIRST NewERD.ER_Entity WHERE NewERD.ER_Entity.Name > EREntityName
            RESULT = SetCursorFirstEntity( NewERD, "ER_Entity", "" );
            if ( RESULT > zCURSOR_UNCHANGED )
            { 
               while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( NewERD, "ER_Entity", "Name", EREntityName ) <= 0 ) )
               { 
                  RESULT = SetCursorNextEntity( NewERD, "ER_Entity", "" );
               } 

            } 

            //:CreateMetaEntity( vSubtask, NewERD, "ER_Entity", zPOS_BEFORE )
            CreateMetaEntity( vSubtask, NewERD, "ER_Entity", zPOS_BEFORE );
            //:SetMatchingAttributesByName ( NewERD,    "ER_Entity",
            //:                              SourceERD, "ER_Entity", zSET_NULL )
            SetMatchingAttributesByName( NewERD, "ER_Entity", SourceERD, "ER_Entity", zSET_NULL );
            //:FOR EACH OldRecursiveLOD.LOD_AttributeRec
            RESULT = SetCursorFirstEntity( OldRecursiveLOD, "LOD_AttributeRec", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF OldRecursiveLOD.LOD_AttributeRec.Work != "Y" AND
               //:   OldRecursiveLOD.ER_AttributeRec.Work  != "Y"
               if ( CompareAttributeToString( OldRecursiveLOD, "LOD_AttributeRec", "Work", "Y" ) != 0 && CompareAttributeToString( OldRecursiveLOD, "ER_AttributeRec", "Work", "Y" ) != 0 )
               { 

                  //:// This is an ER Attribute so add it to ER.
                  //:nRC = LOD_CreateER_Attribute( OldRecursiveLOD,
                  //:                              SourceLPLR,
                  //:                              CurrentLPLR,
                  //:                              NewERD,
                  //:                              vSubtask )
                  nRC = oTZZOLODO_LOD_CreateER_Attribute( OldRecursiveLOD, SourceLPLR, CurrentLPLR, NewERD, vSubtask );
                  //:IF nRC < 0
                  if ( nRC < 0 )
                  { 
                     //:RETURN nRC
                     return( nRC );
                  } 

                  //:END
               } 

               RESULT = SetCursorNextEntity( OldRecursiveLOD, "LOD_AttributeRec", "" );
               //:END
            } 

            //:END

            //:// Set flag to create Identifiers.
            //:// Since we created the ER Entity, we must also create Identifiers. However, these need to be created AFTER the
            //:// ER Relationship is created because an Identifier can reference a relationship. But a Relationship may need to
            //:// be added even though the ER Entity is not added, so that code exists further down below. Thus, instead of
            //:// creating the Identifiers here, we will only set a flag and process the Identifier code after the
            //:// Relationship code.
            //:CreateIdentifiersFlag = 1
            CreateIdentifiersFlag = 1;

            //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = "Y"    // Indicate that the ER has been modified.
            SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "Y" );
            //:ELSE
         } 
         else
         { 
            //:// Since there is no Source ERD and the source ER_Entity is not in the target, we will give an
            //:// error message and ignore the Entity.
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:szMsg = "ER Entity (" + OldRecursiveLOD.ER_EntityRec.Name +
               //:        ") does not exist in new ERD." +
               //:        NEW_LINE + "LOD (" + OldMainLOD.LOD.Name +
               //:        ") will not be migrated."
               GetVariableFromAttribute( szTempString_1, 0, 'S', 33, OldRecursiveLOD, "ER_EntityRec", "Name", "", 0 );
               ZeidonStringCopy( szMsg, 1, 0, "ER Entity (", 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, ") does not exist in new ERD.", 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, "LOD (", 1, 0, 255 );
               GetVariableFromAttribute( szTempString_2, 0, 'S', 33, OldMainLOD, "LOD", "Name", "", 0 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 255 );
               ZeidonStringConcat( szMsg, 1, 0, ") will not be migrated.", 1, 0, 255 );
               //:MessageSend( vSubtask, "ZO00404", "Entity Copy",
               //:             szMsg,
               //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vSubtask, "ZO00404", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               //:DELETE ENTITY NewRecursiveLOD.LOD_EntityParent NONE
               RESULT = DeleteEntity( NewRecursiveLOD, "LOD_EntityParent", zREPOS_NONE );
               //:RETURN 0
               return( 0 );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// Process ER Relationship for LOD, if it exists.
   //:// If the LOD Entity is part of an ER relationship, make sure that relationship is also in the new ER.
   //:// We will use the relationship in the LOD to locate the relationship in the ER and then copy the relationship from the ER.
   //:// Note that we might also have to create the ER Entity at the other end of the relationship. If we do, we
   //:// won't create attributes here, as they will be added when that entity is processed for the LOD.
   //:// We will call the ERD_RelationshipCopy operation in TZEREM2O to actually create the relationship, as the
   //:// process is very complicated.
   //:IF OldRecursiveLOD.ER_RelLinkRec EXISTS AND SourceERD != 0
   lTempInteger_1 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_RelLinkRec" );
   if ( lTempInteger_1 == 0 && SourceERD != 0 )
   { 
      //:RelationshipName = OldRecursiveLOD.ER_RelLinkRec.Name
      GetVariableFromAttribute( RelationshipName, 0, 'S', 33, OldRecursiveLOD, "ER_RelLinkRec", "Name", "", 0 );

      //:// To locate a match on the relationship, we will need to find a match on the relationship name and
      //:// on both the source and target entity of the relationship.
      //:FoundFlag = 0
      FoundFlag = 0;
      //:RelationshipName = OldRecursiveLOD.ER_RelLinkRec.Name
      GetVariableFromAttribute( RelationshipName, 0, 'S', 33, OldRecursiveLOD, "ER_RelLinkRec", "Name", "", 0 );
      //:SET CURSOR FIRST NewERD.ER_RelType
      RESULT = SetCursorFirstEntity( NewERD, "ER_RelType", "" );
      //:LOOP WHILE FoundFlag = 0 AND RESULT >= zCURSOR_SET
      while ( FoundFlag == 0 && RESULT >= zCURSOR_SET )
      { 
         //:// Now make sure the entity on the other side of the RelLink is correct.
         //:// If not, continue searching.

         //:// First look for match on relationship name and parent Entity name.
         //:SET CURSOR FIRST NewERD.ER_RelLink_2
         //:           WHERE NewERD.ER_RelLink_2.Name = RelationshipName AND
         //:                 NewERD.ER_Entity_2.Name  = ParentName
         RESULT = SetCursorFirstEntity( NewERD, "ER_RelLink_2", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( NewERD, "ER_RelLink_2", "Name", RelationshipName ) != 0 || CompareAttributeToString( NewERD, "ER_Entity_2", "Name", ParentName ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( NewERD, "ER_RelLink_2", "" );
            } 

         } 

         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:// There was a match, so see if the other RelLink is for he entity itself.
            //:CreateViewFromViewForTask( NewERD2, NewERD, 0 )
            CreateViewFromViewForTask( &NewERD2, NewERD, 0 );
            //:SET CURSOR NEXT NewERD2.ER_RelLink_2
            RESULT = SetCursorNextEntity( NewERD2, "ER_RelLink_2", "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:SET CURSOR PREVIOUS NewERD2.ER_RelLink_2
               RESULT = SetCursorPrevEntity( NewERD2, "ER_RelLink_2", "" );
            } 

            //:END
            //:IF NewERD2.ER_Entity_2.Name = EREntityName
            if ( CompareAttributeToString( NewERD2, "ER_Entity_2", "Name", EREntityName ) == 0 )
            { 
               //:FoundFlag = 1
               FoundFlag = 1;
            } 

            //:END
            //:DropView ( NewERD2 )
            DropView( NewERD2 );
         } 

         //:END
         //:IF FoundFlag = 0
         if ( FoundFlag == 0 )
         { 
            //:// Continue to next RelType if we have no yet found a match.
            //:SET CURSOR NEXT NewERD.ER_RelType
            RESULT = SetCursorNextEntity( NewERD, "ER_RelType", "" );
         } 

         //:END
      } 

      //:END
      //:IF FoundFlag = 0
      if ( FoundFlag == 0 )
      { 

         //:// The relationship is NOT in the ER.
         //:// Note that the Entity at the other end of the Relationship is already in the ER, either because
         //:// it is not new with this LOD or it was added earlier when the Parent Entity was processed.

         //:// We need to position on the relationship in the SourceERD that we are to copy. We are already positioned
         //:// on the entity.
         //:SET CURSOR FIRST SourceERD.ER_RelLink_Other WITHIN SourceERD.ER_Entity
         //:           WHERE SourceERD.ER_RelLink_Other.Name = RelationshipName
         //:             AND SourceERD.ER_Entity_Other.Name  = ParentName
         RESULT = SetCursorFirstEntity( SourceERD, "ER_RelLink_Other", "ER_Entity" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( SourceERD, "ER_RelLink_Other", "Name", RelationshipName ) != 0 || CompareAttributeToString( SourceERD, "ER_Entity_Other", "Name", ParentName ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( SourceERD, "ER_RelLink_Other", "ER_Entity" );
            } 

         } 


         //:// Go to create the relationship.
         //:ERD_RelationshipCopy( NewERD, SourceERD, vSubtask )
         oTZEREMDO_ERD_RelationshipCopy( NewERD, SourceERD, vSubtask );

         //:// We now need to position on the correct relationship from the Source LOD, which is the "other"
         //:// relationship from that in the LOD, which is RelationshipName.
         //:SET CURSOR FIRST NewERD.ER_RelLink_2 WHERE NewERD.ER_RelLink_2.Name != RelationshipName
         RESULT = SetCursorFirstEntity( NewERD, "ER_RelLink_2", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( NewERD, "ER_RelLink_2", "Name", RelationshipName ) == 0 ) )
            { 
               RESULT = SetCursorNextEntity( NewERD, "ER_RelLink_2", "" );
            } 

         } 


         //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = "Y"    // Indicate that the ER has been modified.
         SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "Y" );
      } 

      //:END

      //:// Whether we added the RelLink or it was already there, include it in the LOD.
      //:SET CURSOR FIRST NewERD.ER_Entity WHERE NewERD.ER_Entity.Name = OldRecursiveLOD.ER_EntityRec.Name
      GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), OldRecursiveLOD, "ER_EntityRec", "Name" );
      RESULT = SetCursorFirstEntityByString( NewERD, "ER_Entity", "Name", szTempString_3, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:IssueError( NewRecursiveLOD,0,0, "Programming Include Error 1" )
         IssueError( NewRecursiveLOD, 0, 0, "Programming Include Error 1" );
         //:RETURN -1
         return( -1 );
      } 

      //:END
      //:INCLUDE NewRecursiveLOD.ER_EntityRec FROM NewERD.ER_Entity
      RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_EntityRec", NewERD, "ER_Entity", zPOS_AFTER );

      //:IF OldRecursiveLOD.ER_RelLinkRec EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_RelLinkRec" );
      if ( lTempInteger_2 == 0 )
      { 
         //:// It's complex to position on the correct RelType in the Target ERD, as there is no name.
         //:// Thus, we'll first position on the RelType in the Source ERD, and then position on the Target ERD by
         //:// RelLink Name and other ER_Entity Name.
         //:SET CURSOR FIRST SourceERD.ER_Entity WHERE SourceERD.ER_Entity.Name = OldRecursiveLOD.ER_EntityRec.Name
         GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), OldRecursiveLOD, "ER_EntityRec", "Name" );
         RESULT = SetCursorFirstEntityByString( SourceERD, "ER_Entity", "Name", szTempString_3, "" );
         //:SET CURSOR FIRST SourceERD.ER_RelType_1 WITHIN SourceERD.ER_Entity
         //:           WHERE SourceERD.ER_RelType_1.ZKey = OldRecursiveLOD.ER_RelTypeRec.ZKey
         GetIntegerFromAttribute( &lTempInteger_3, OldRecursiveLOD, "ER_RelTypeRec", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( SourceERD, "ER_RelType_1", "ZKey", lTempInteger_3, "ER_Entity" );
         //:SET CURSOR FIRST NewERD.ER_Entity_Other WITHIN NewERD.ER_Entity
         //:           WHERE NewERD.ER_Entity_Other.Name  = SourceERD.ER_Entity_Other.Name
         //:             AND NewERD.ER_RelLink_Other.Name = SourceERD.ER_RelLink_Other.Name
         RESULT = SetCursorFirstEntity( NewERD, "ER_Entity_Other", "ER_Entity" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( NewERD, "ER_Entity_Other", "Name", SourceERD, "ER_Entity_Other", "Name" ) != 0 ||
                    CompareAttributeToAttribute( NewERD, "ER_RelLink_Other", "Name", SourceERD, "ER_RelLink_Other", "Name" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( NewERD, "ER_Entity_Other", "ER_Entity" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( NewRecursiveLOD,0,0, "Programming Include Error 2" )
            IssueError( NewRecursiveLOD, 0, 0, "Programming Include Error 2" );
            //:RETURN -1
            return( -1 );
         } 

         //:END
         //:INCLUDE NewRecursiveLOD.ER_RelLinkRec FROM NewERD.ER_RelLink_Other
         RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_RelLinkRec", NewERD, "ER_RelLink_Other", zPOS_AFTER );
         //:IF NewRecursiveLOD.ER_RelTypeRec DOES NOT EXIST
         lTempInteger_4 = CheckExistenceOfEntity( NewRecursiveLOD, "ER_RelTypeRec" );
         if ( lTempInteger_4 != 0 )
         { 
            //:INCLUDE NewRecursiveLOD.ER_RelTypeRec FROM NewERD.ER_RelType_1
            RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_RelTypeRec", NewERD, "ER_RelType_1", zPOS_AFTER );
         } 

         //:END
      } 

      //:END
   } 


   //:END

   //:// If Identifiers were flagged above to be created, create them here.
   //:IF CreateIdentifiersFlag = 1
   if ( CreateIdentifiersFlag == 1 )
   { 
      //:FOR EACH SourceERD.ER_EntIdentifier
      RESULT = SetCursorFirstEntity( SourceERD, "ER_EntIdentifier", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( vSubtask, NewERD, "ER_EntIdentifier", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewERD, "ER_EntIdentifier", zPOS_AFTER );
         //:SetMatchingAttributesByName( NewERD, "ER_EntIdentifier", SourceERD, "ER_EntIdentifier", zSET_NULL )
         SetMatchingAttributesByName( NewERD, "ER_EntIdentifier", SourceERD, "ER_EntIdentifier", zSET_NULL );
         //:FOR EACH SourceERD.ER_FactType
         RESULT = SetCursorFirstEntity( SourceERD, "ER_FactType", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( vSubtask, NewERD, "ER_FactType", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewERD, "ER_FactType", zPOS_AFTER );
            //:SetMatchingAttributesByName( NewERD, "ER_FactType", SourceERD, "ER_FactType", zSET_NULL )
            SetMatchingAttributesByName( NewERD, "ER_FactType", SourceERD, "ER_FactType", zSET_NULL );
            //:IF SourceERD.ER_AttributeIdentifier EXISTS
            lTempInteger_5 = CheckExistenceOfEntity( SourceERD, "ER_AttributeIdentifier" );
            if ( lTempInteger_5 == 0 )
            { 
               //:SET CURSOR FIRST NewERD.ER_Attribute WHERE NewERD.ER_Attribute.Name = SourceERD.ER_AttributeIdentifier.Name
               GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), SourceERD, "ER_AttributeIdentifier", "Name" );
               RESULT = SetCursorFirstEntityByString( NewERD, "ER_Attribute", "Name", szTempString_3, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:szMsg = "ER Entity (" + EREntityName +
                  //:        ") Identifier (" + SourceERD.ER_AttributeIdentifier.Name + ") does not exist in new ERD." +
                  //:        NEW_LINE + "LOD (" + OldMainLOD.LOD.Name +
                  //:        ") will not be migrated."
                  ZeidonStringCopy( szMsg, 1, 0, "ER Entity (", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, EREntityName, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") Identifier (", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_3, 0, 'S', 33, SourceERD, "ER_AttributeIdentifier", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_3, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") does not exist in new ERD.", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, "LOD (", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_4, 0, 'S', 33, OldMainLOD, "LOD", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_4, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") will not be migrated.", 1, 0, 255 );
                  //:MessageSend( vSubtask, "ZO00404", "Entity Copy",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "ZO00404", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:DELETE ENTITY NewRecursiveLOD.LOD_EntityParent NONE
                  RESULT = DeleteEntity( NewRecursiveLOD, "LOD_EntityParent", zREPOS_NONE );
                  //:RETURN 0
                  return( 0 );
               } 

               //:END
               //:INCLUDE NewERD.ER_AttributeIdentifier FROM NewERD.ER_Attribute
               RESULT = IncludeSubobjectFromSubobject( NewERD, "ER_AttributeIdentifier", NewERD, "ER_Attribute", zPOS_AFTER );
            } 

            //:END
            //:IF SourceERD.ER_RelLinkIdentifier EXISTS
            lTempInteger_6 = CheckExistenceOfEntity( SourceERD, "ER_RelLinkIdentifier" );
            if ( lTempInteger_6 == 0 )
            { 
               //:SET CURSOR FIRST NewERD.ER_RelLink_Other WITHIN NewERD.ER_Entity
               //:           WHERE NewERD.ER_RelLink_Other.Name = SourceERD.ER_RelLinkIdentifier.Name
               //:             AND NewERD.ER_Entity_Other.Name  = SourceERD.ER_Entity_Other_Identifier.Name
               RESULT = SetCursorFirstEntity( NewERD, "ER_RelLink_Other", "ER_Entity" );
               if ( RESULT > zCURSOR_UNCHANGED )
               { 
                  while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( NewERD, "ER_RelLink_Other", "Name", SourceERD, "ER_RelLinkIdentifier", "Name" ) != 0 ||
                        CompareAttributeToAttribute( NewERD, "ER_Entity_Other", "Name", SourceERD, "ER_Entity_Other_Identifier", "Name" ) != 0 ) )
                  { 
                     RESULT = SetCursorNextEntity( NewERD, "ER_RelLink_Other", "ER_Entity" );
                  } 

               } 

               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:szMsg = "ER Entity (" + EREntityName +
                  //:        ") Identifier (" + SourceERD.ER_RelLinkIdentifier.Name + "-" + SourceERD.ER_Entity_Other_Identifier.Name +
                  //:        ") does not exist in new ERD." +
                  //:        NEW_LINE + "LOD (" + OldMainLOD.LOD.Name +
                  //:        ") will not be migrated."
                  ZeidonStringCopy( szMsg, 1, 0, "ER Entity (", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, EREntityName, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") Identifier (", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_5, 0, 'S', 33, SourceERD, "ER_RelLinkIdentifier", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_5, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, "-", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_6, 0, 'S', 33, SourceERD, "ER_Entity_Other_Identifier", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_6, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") does not exist in new ERD.", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, "LOD (", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_7, 0, 'S', 33, OldMainLOD, "LOD", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_7, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") will not be migrated.", 1, 0, 255 );
                  //:MessageSend( vSubtask, "ZO00404", "Entity Copy",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "ZO00404", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:DELETE ENTITY NewRecursiveLOD.LOD_EntityParent NONE
                  RESULT = DeleteEntity( NewRecursiveLOD, "LOD_EntityParent", zREPOS_NONE );
                  //:RETURN 0
                  return( 0 );
               } 

               //:END
               //:INCLUDE NewERD.ER_AttributeIdentifier FROM NewERD.ER_Attribute
               RESULT = IncludeSubobjectFromSubobject( NewERD, "ER_AttributeIdentifier", NewERD, "ER_Attribute", zPOS_AFTER );
            } 

            RESULT = SetCursorNextEntity( SourceERD, "ER_FactType", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( SourceERD, "ER_EntIdentifier", "" );
         //:END
      } 

      //:END
   } 

   //:END

   //:// Include the ER Entity and ER_RelLink into the new LOD, if it wasn't done above.
   //:IF OldRecursiveLOD.ER_EntityRec EXISTS
   lTempInteger_7 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_EntityRec" );
   if ( lTempInteger_7 == 0 )
   { 
      //:// We will check if we've already included the ER_EntityRec, as it may have been included above when the
      //:// RelLink was included.
      //:IF NewRecursiveLOD.ER_EntityRec DOES NOT EXIST
      lTempInteger_8 = CheckExistenceOfEntity( NewRecursiveLOD, "ER_EntityRec" );
      if ( lTempInteger_8 != 0 )
      { 
         //:SET CURSOR FIRST NewERD.ER_Entity WHERE NewERD.ER_Entity.Name = OldRecursiveLOD.ER_EntityRec.Name
         GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldRecursiveLOD, "ER_EntityRec", "Name" );
         RESULT = SetCursorFirstEntityByString( NewERD, "ER_Entity", "Name", szTempString_8, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( NewRecursiveLOD,0,0, "Programming Include Error 1" )
            IssueError( NewRecursiveLOD, 0, 0, "Programming Include Error 1" );
            //:RETURN -1
            return( -1 );
         } 

         //:END
         //:INCLUDE NewRecursiveLOD.ER_EntityRec FROM NewERD.ER_Entity
         RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_EntityRec", NewERD, "ER_Entity", zPOS_AFTER );

         //:IF OldRecursiveLOD.ER_RelLinkRec EXISTS
         lTempInteger_9 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_RelLinkRec" );
         if ( lTempInteger_9 == 0 )
         { 
            //:// It's complex to position on the correct RelType in the Target ERD, as there is no name.
            //:// Thus, we'll first position on the RelType in the Source ERD, and then position on the Target ERD by
            //:// RelLink Name and other ER_Entity Name.
            //:SET CURSOR FIRST SourceERD.ER_Entity WHERE SourceERD.ER_Entity.Name = OldRecursiveLOD.ER_EntityRec.Name
            GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldRecursiveLOD, "ER_EntityRec", "Name" );
            RESULT = SetCursorFirstEntityByString( SourceERD, "ER_Entity", "Name", szTempString_8, "" );
            //:SET CURSOR FIRST SourceERD.ER_RelType_1 WITHIN SourceERD.ER_Entity
            //:           WHERE SourceERD.ER_RelType_1.ZKey = OldRecursiveLOD.ER_RelTypeRec.ZKey
            GetIntegerFromAttribute( &lTempInteger_10, OldRecursiveLOD, "ER_RelTypeRec", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( SourceERD, "ER_RelType_1", "ZKey", lTempInteger_10, "ER_Entity" );
            //:SET CURSOR FIRST NewERD.ER_Entity_Other WITHIN NewERD.ER_Entity
            //:           WHERE NewERD.ER_Entity_Other.Name  = SourceERD.ER_Entity_Other.Name
            //:             AND NewERD.ER_RelLink_Other.Name = SourceERD.ER_RelLink_Other.Name
            RESULT = SetCursorFirstEntity( NewERD, "ER_Entity_Other", "ER_Entity" );
            if ( RESULT > zCURSOR_UNCHANGED )
            { 
               while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( NewERD, "ER_Entity_Other", "Name", SourceERD, "ER_Entity_Other", "Name" ) != 0 ||
                       CompareAttributeToAttribute( NewERD, "ER_RelLink_Other", "Name", SourceERD, "ER_RelLink_Other", "Name" ) != 0 ) )
               { 
                  RESULT = SetCursorNextEntity( NewERD, "ER_Entity_Other", "ER_Entity" );
               } 

            } 

            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:IssueError( NewRecursiveLOD,0,0, "Programming Include Error 2" )
               IssueError( NewRecursiveLOD, 0, 0, "Programming Include Error 2" );
               //:RETURN -1
               return( -1 );
            } 

            //:END
            //:INCLUDE NewRecursiveLOD.ER_RelLinkRec FROM NewERD.ER_RelLink_Other
            RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_RelLinkRec", NewERD, "ER_RelLink_Other", zPOS_AFTER );
            //:IF NewRecursiveLOD.ER_RelTypeRec DOES NOT EXIST
            lTempInteger_11 = CheckExistenceOfEntity( NewRecursiveLOD, "ER_RelTypeRec" );
            if ( lTempInteger_11 != 0 )
            { 
               //:INCLUDE NewRecursiveLOD.ER_RelTypeRec FROM NewERD.ER_RelType_1
               RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_RelTypeRec", NewERD, "ER_RelType_1", zPOS_AFTER );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //://  Create each LOD Attribute
   //:FOR EACH OldRecursiveLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( OldRecursiveLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:LOD_AttributeCopy( vSubtask,
      //:                   NewMainLOD,
      //:                   NewRecursiveLOD,
      //:                   OldMainLOD,
      //:                   OldRecursiveLOD,
      //:                   SourceLPLR,
      //:                   CurrentLPLR,
      //:                   NewERD,
      //:                   "Y" )
      oTZZOLODO_LOD_AttributeCopy( vSubtask, NewMainLOD, NewRecursiveLOD, OldMainLOD, OldRecursiveLOD, SourceLPLR, CurrentLPLR, NewERD, "Y" );
      RESULT = SetCursorNextEntity( OldRecursiveLOD, "LOD_AttributeRec", "" );
   } 


   //:END

   //:// Loop again through old LOD_Attribute entities and process DB_ER_AttributeRec entities.
   //:FOR EACH OldRecursiveLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( OldRecursiveLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// If DB_ER_AttributeRec exists in the old, include it in the new.
      //:IF  OldRecursiveLOD.DB_ER_AttributeRec EXISTS
      lTempInteger_12 = CheckExistenceOfEntity( OldRecursiveLOD, "DB_ER_AttributeRec" );
      if ( lTempInteger_12 == 0 )
      { 
         //:CreateViewFromViewForTask( TempLOD, NewRecursiveLOD, 0 )
         CreateViewFromViewForTask( &TempLOD, NewRecursiveLOD, 0 );
         //:SET CURSOR FIRST NewRecursiveLOD.ER_AttributeRec WITHIN NewRecursiveLOD.LOD_EntityParent WHERE
         //:     NewRecursiveLOD.ER_AttributeRec.Name = OldRecursiveLOD.ER_AttributeRec.Name
         GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldRecursiveLOD, "ER_AttributeRec", "Name" );
         RESULT = SetCursorFirstEntityByString( NewRecursiveLOD, "ER_AttributeRec", "Name", szTempString_8, "LOD_EntityParent" );
         //:SET CURSOR FIRST TempLOD.ER_AttributeRec WITHIN TempLOD.LOD_EntityParent WHERE
         //:     TempLOD.ER_AttributeRec.Name = OldRecursiveLOD.DB_ER_AttributeRec.Name
         GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldRecursiveLOD, "DB_ER_AttributeRec", "Name" );
         RESULT = SetCursorFirstEntityByString( TempLOD, "ER_AttributeRec", "Name", szTempString_8, "LOD_EntityParent" );
         //:INCLUDE NewRecursiveLOD.DB_ER_AttributeRec FROM TempLOD.ER_AttributeRec
         RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "DB_ER_AttributeRec", TempLOD, "ER_AttributeRec", zPOS_AFTER );
         //:DropView ( TempLOD )
         DropView( TempLOD );
      } 

      RESULT = SetCursorNextEntity( OldRecursiveLOD, "LOD_AttributeRec", "" );
      //:END
   } 

   //:END

   //://  If OldRecursiveLOD.LOD_EntityConstraintOperRec exists, create a relationship to the
   //://  corresponding operation in the new LOD.
   //://  If the operation doesn't exist in the new LOD, ignore the LOD_EntityConstraintOperRec.
   //:IF OldRecursiveLOD.LOD_EntityConstraintOperRec EXISTS
   lTempInteger_13 = CheckExistenceOfEntity( OldRecursiveLOD, "LOD_EntityConstraintOperRec" );
   if ( lTempInteger_13 == 0 )
   { 
      //:SET CURSOR FIRST NewMainLOD.Operation WITHIN NewMainLOD.LOD WHERE
      //:                 NewMainLOD.Operation.Name = OldRecursiveLOD.LOD_EntityConstraintOperRec.Name
      GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldRecursiveLOD, "LOD_EntityConstraintOperRec", "Name" );
      RESULT = SetCursorFirstEntityByString( NewMainLOD, "Operation", "Name", szTempString_8, "LOD" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:INCLUDE NewRecursiveLOD.LOD_EntityConstraintOperRec FROM NewMainLOD.Operation
         RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "LOD_EntityConstraintOperRec", NewMainLOD, "Operation", zPOS_AFTER );
      } 

      //:END
   } 

   //:END

   //:// Process any LOD_EntityChild entities recursively.
   //:// We need to make sure children are processed after parents are completed, because the children may need
   //:// the ER Entities processed by the parents.
   //:FOR EACH OldRecursiveLOD.LOD_EntityChild
   RESULT = SetCursorFirstEntity( OldRecursiveLOD, "LOD_EntityChild", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF OldRecursiveLOD.ER_EntityRec EXISTS
      lTempInteger_14 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_EntityRec" );
      if ( lTempInteger_14 == 0 )
      { 
         //:OrigParentName = OldRecursiveLOD.ER_EntityRec.Name
         GetVariableFromAttribute( OrigParentName, 0, 'S', 33, OldRecursiveLOD, "ER_EntityRec", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:OrigParentName = ""
         ZeidonStringCopy( OrigParentName, 1, 0, "", 1, 0, 33 );
      } 

      //:END
      //:SetViewToSubobject( OldRecursiveLOD, "LOD_EntityChild" )
      SetViewToSubobject( OldRecursiveLOD, "LOD_EntityChild" );
      //:SetViewToSubobject( NewRecursiveLOD, "LOD_EntityChild" )
      SetViewToSubobject( NewRecursiveLOD, "LOD_EntityChild" );

      //:nRC = LOD_EntityCopy( NewMainLOD,
      //:                      NewRecursiveLOD,
      //:                      OldMainLOD,
      //:                      OldRecursiveLOD,
      //:                      SourceLPLR,
      //:                      CurrentLPLR,
      //:                      NewERD,
      //:                      SourceERD,
      //:                      NewDTE,
      //:                      vSubtask,
      //:                      OrigParentName,
      //:                      szAddER_EntityFlag )
      nRC = oTZZOLODO_LOD_EntityCopy( NewMainLOD, NewRecursiveLOD, OldMainLOD, OldRecursiveLOD, SourceLPLR, CurrentLPLR, NewERD, SourceERD, NewDTE, vSubtask, OrigParentName, szAddER_EntityFlag );

      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
         //:ELSE
      } 
      else
      { 
         //:IF nRC != 0        // Pass along Domain errors
         if ( nRC != 0 )
         { 
            //:ErrorFlag = 1
            ErrorFlag = 1;
         } 

         //:END
      } 

      //:END

      //:ResetViewFromSubobject( OldRecursiveLOD )
      ResetViewFromSubobject( OldRecursiveLOD );
      //:ResetViewFromSubobject( NewRecursiveLOD )
      ResetViewFromSubobject( NewRecursiveLOD );
      RESULT = SetCursorNextEntity( OldRecursiveLOD, "LOD_EntityChild", "" );
   } 

   //:END

   //:Return ErrorFlag
   return( ErrorFlag );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_CreateER_Attribute( VIEW OldRecursiveLOD BASED ON LOD TZZOLODO,
//:                        VIEW SourceLPLR      BASED ON LOD TZCMLPLO,
//:                        VIEW CurrentLPLR     BASED ON LOD TZCMLPLO,
//:                        VIEW NewERD          BASED ON LOD TZEREMDO,
//:                        VIEW vSubtask )

//:   VIEW NewDomain BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_CreateER_Attribute( zVIEW     OldRecursiveLOD,
                                  zVIEW     SourceLPLR,
                                  zVIEW     CurrentLPLR,
                                  zVIEW     NewERD,
                                  zVIEW     vSubtask )
{
   zVIEW     NewDomain = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:STRING ( 254)  szMsg
   zCHAR     szMsg[ 255 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    RESULT; 


   //:// First make sure that the Domain exists.
   //:nRC = ActivateMetaOI_ByName( vSubtask, NewDomain, 0, zREFER_DOMAIN_META, zSINGLE, OldRecursiveLOD.DomainRec.Name, 0 )
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldRecursiveLOD, "DomainRec", "Name" );
   nRC = ActivateMetaOI_ByName( vSubtask, &NewDomain, 0, zREFER_DOMAIN_META, zSINGLE, szTempString_0, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:// The Domain was not found, so prompt User whether or not to add it.
      //:IF SourceLPLR != 0
      if ( SourceLPLR != 0 )
      { 
         //:// There is a Source LPLR, so add the Domain
         //:DomainAddForMerge( NewDomain, SourceLPLR, CurrentLPLR,
         //:                   CurrentLPLR.LPLR.wFullyQualifiedFileName,
         //:                   OldRecursiveLOD.DomainRec.Name, vSubtask )
         GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), CurrentLPLR, "LPLR", "wFullyQualifiedFileName" );
         GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldRecursiveLOD, "DomainRec", "Name" );
         oTZDGSRCO_DomainAddForMerge( &NewDomain, SourceLPLR, CurrentLPLR, szTempString_1, szTempString_2, vSubtask );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:RETURN nRC
            return( nRC );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The Domain was not found, so give error and don't copy attribute.
         //:szMsg = "Domain (" + OldRecursiveLOD.DomainRec.Name + ") does not exist. Attribute (" +
         //:        OldRecursiveLOD.ER_AttributeRec.Name + ") will not be copied."
         GetVariableFromAttribute( szTempString_3, 0, 'S', 33, OldRecursiveLOD, "DomainRec", "Name", "", 0 );
         ZeidonStringCopy( szMsg, 1, 0, "Domain (", 1, 0, 255 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_3, 1, 0, 255 );
         ZeidonStringConcat( szMsg, 1, 0, ") does not exist. Attribute (", 1, 0, 255 );
         GetVariableFromAttribute( szTempString_4, 0, 'S', 33, OldRecursiveLOD, "ER_AttributeRec", "Name", "", 0 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_4, 1, 0, 255 );
         ZeidonStringConcat( szMsg, 1, 0, ") will not be copied.", 1, 0, 255 );
         //:MessageSend( vSubtask, "ZO00403", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "ZO00403", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:RETURN -1   // Return indicating an error has occurred so the attribute will be dropped later.
         return( -1 );
      } 

      //:END
   } 

   //:END

   //:// Since the Domain was either found or created, continue with add of ER Attribute
   //:// Create ER Attribute from LOD Attribute. Create as last Attribute for Entity.
   //:SET CURSOR LAST NewERD.ER_Attribute
   RESULT = SetCursorLastEntity( NewERD, "ER_Attribute", "" );
   //:CreateMetaEntity( vSubtask, NewERD, "ER_Attribute", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewERD, "ER_Attribute", zPOS_AFTER );
   //:SetMatchingAttributesByName ( NewERD,          "ER_Attribute",
   //:                              OldRecursiveLOD, "ER_AttributeRec", zSET_NULL )
   SetMatchingAttributesByName( NewERD, "ER_Attribute", OldRecursiveLOD, "ER_AttributeRec", zSET_NULL );
   //:INCLUDE NewERD.Domain FROM NewDomain.Domain
   RESULT = IncludeSubobjectFromSubobject( NewERD, "Domain", NewDomain, "Domain", zPOS_AFTER );
   //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = "Y"    // Indicate that the ER has been modified.
   SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "Y" );
   //:DropMetaOI( vSubtask, NewDomain )
   DropMetaOI( vSubtask, NewDomain );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_AttributeCopy( VIEW        vSubtask,
//:                   VIEW        NewMainLOD      BASED ON LOD TZZOLODO,
//:                   VIEW        NewRecursiveLOD BASED ON LOD TZZOLODO,
//:                   VIEW        OldMainLOD      BASED ON LOD TZZOLODO,
//:                   VIEW        OldRecursiveLOD BASED ON LOD TZZOLODO,
//:                   VIEW        SourceLPLR      BASED ON LOD TZCMLPLO,
//:                   VIEW        CurrentLPLR     BASED ON LOD TZCMLPLO,
//:                   VIEW        NewERD          BASED ON LOD TZEREMDO,
//:                   STRING ( 1 ) szAddER_AttributeFlag )

//:   VIEW NewDomain BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_AttributeCopy( zVIEW     vSubtask,
                             zVIEW     NewMainLOD,
                             zVIEW     NewRecursiveLOD,
                             zVIEW     OldMainLOD,
                             zVIEW     OldRecursiveLOD,
                             zVIEW     SourceLPLR,
                             zVIEW     CurrentLPLR,
                             zVIEW     NewERD,
                             zPCHAR    szAddER_AttributeFlag )
{
   zVIEW     NewDomain = 0; 

   //:SHORT      ErrorFlag
   zSHORT    ErrorFlag = 0; 
   //:SHORT      FoundFlag
   zSHORT    FoundFlag = 0; 
   //:SHORT      n
   zSHORT    n = 0; 
   //:SHORT      nRC
   zSHORT    nRC = 0; 
   //:STRING ( 254)  szMsg
   zCHAR     szMsg[ 255 ] = { 0 }; 
   //:STRING ( 32 )  RelationshipName
   zCHAR     RelationshipName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  ERAttributeName
   zCHAR     ERAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  DomainName
   zCHAR     DomainName[ 33 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_2; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_3; 


   //:CreateMetaEntity( vSubtask, NewRecursiveLOD, "LOD_AttributeRec", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewRecursiveLOD, "LOD_AttributeRec", zPOS_AFTER );
   //:    NewRecursiveLOD.LOD_AttributeRec.Work             = OldRecursiveLOD.LOD_AttributeRec.Work
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "Work", OldRecursiveLOD, "LOD_AttributeRec", "Work" );
   //:    NewRecursiveLOD.LOD_AttributeRec.Update           = OldRecursiveLOD.LOD_AttributeRec.Update
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "Update", OldRecursiveLOD, "LOD_AttributeRec", "Update" );
   //:    NewRecursiveLOD.LOD_AttributeRec.Init             = OldRecursiveLOD.LOD_AttributeRec.Init
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "Init", OldRecursiveLOD, "LOD_AttributeRec", "Init" );
   //:    NewRecursiveLOD.LOD_AttributeRec.SeqPriority      = OldRecursiveLOD.LOD_AttributeRec.SeqPriority
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "SeqPriority", OldRecursiveLOD, "LOD_AttributeRec", "SeqPriority" );
   //:    NewRecursiveLOD.LOD_AttributeRec.Hidden           = OldRecursiveLOD.LOD_AttributeRec.Hidden
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "Hidden", OldRecursiveLOD, "LOD_AttributeRec", "Hidden" );
   //:    NewRecursiveLOD.LOD_AttributeRec.SeqOrder         = OldRecursiveLOD.LOD_AttributeRec.SeqOrder
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "SeqOrder", OldRecursiveLOD, "LOD_AttributeRec", "SeqOrder" );
   //:    NewRecursiveLOD.LOD_AttributeRec.DIL_Text         = OldRecursiveLOD.LOD_AttributeRec.DIL_Text
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "DIL_Text", OldRecursiveLOD, "LOD_AttributeRec", "DIL_Text" );
   //:    NewRecursiveLOD.LOD_AttributeRec.SeqNo_LOD_Entity = OldRecursiveLOD.LOD_AttributeRec.SeqNo_LOD_Entity
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "SeqNo_LOD_Entity", OldRecursiveLOD, "LOD_AttributeRec", "SeqNo_LOD_Entity" );
   //:    NewRecursiveLOD.LOD_AttributeRec.DB_DerivedValue  = OldRecursiveLOD.LOD_AttributeRec.DB_DerivedValue
   SetAttributeFromAttribute( NewRecursiveLOD, "LOD_AttributeRec", "DB_DerivedValue", OldRecursiveLOD, "LOD_AttributeRec", "DB_DerivedValue" );

   //:IF OldRecursiveLOD.ER_AttributeRec EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( OldRecursiveLOD, "ER_AttributeRec" );
   if ( lTempInteger_0 == 0 )
   { 

      //:IF OldRecursiveLOD.LOD_AttributeRec.Work = "Y" OR
      //:   OldRecursiveLOD.LOD_EntityParent.Work = "Y"
      if ( CompareAttributeToString( OldRecursiveLOD, "LOD_AttributeRec", "Work", "Y" ) == 0 || CompareAttributeToString( OldRecursiveLOD, "LOD_EntityParent", "Work", "Y" ) == 0 )
      { 

         //:CreateMetaEntity( vSubtask, NewRecursiveLOD, "ER_AttributeRec", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewRecursiveLOD, "ER_AttributeRec", zPOS_AFTER );
         //:SetMatchingAttributesByName ( NewRecursiveLOD, "ER_AttributeRec", OldRecursiveLOD,
         //:                              "ER_AttributeRec", zSET_NULL )
         SetMatchingAttributesByName( NewRecursiveLOD, "ER_AttributeRec", OldRecursiveLOD, "ER_AttributeRec", zSET_NULL );
         //:IF OldRecursiveLOD.DomainRec EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( OldRecursiveLOD, "DomainRec" );
         if ( lTempInteger_1 == 0 )
         { 
            //:ErrorFlag = 1  // Initialize an error for Domain not found. If it is found, we'll reset it to 0.
            ErrorFlag = 1;

            //:NAME VIEW CurrentLPLR "DomainCurrentLPLR"
            SetNameForView( CurrentLPLR, "DomainCurrentLPLR", 0, zLEVEL_TASK );
            //:SET CURSOR FIRST CurrentLPLR.W_MetaType
            //:           WHERE CurrentLPLR.W_MetaType.Type = 2003
            RESULT = SetCursorFirstEntityByInteger( CurrentLPLR, "W_MetaType", "Type", 2003, "" );

            //:// If there is a SourceLPLR, get the Domain Name from there.
            //:// Otherwise, use the Domain Name from the LOD.
            //:IF SourceLPLR = 0
            if ( SourceLPLR == 0 )
            { 
               //:DomainName = OldRecursiveLOD.DomainRec.Name
               GetVariableFromAttribute( DomainName, 0, 'S', 33, OldRecursiveLOD, "DomainRec", "Name", "", 0 );
               //:SET CURSOR FIRST CurrentLPLR.W_MetaDef
               //:           WHERE CurrentLPLR.W_MetaDef.Name = DomainName
               RESULT = SetCursorFirstEntityByString( CurrentLPLR, "W_MetaDef", "Name", DomainName, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:ErrorFlag = 0  // Indicate Domain found.
                  ErrorFlag = 0;
               } 

               //:END
               //:ELSE
            } 
            else
            { 

               //:NAME VIEW SourceLPLR "DomainSourceLPLR"
               SetNameForView( SourceLPLR, "DomainSourceLPLR", 0, zLEVEL_TASK );
               //:SET CURSOR FIRST SourceLPLR.W_MetaType
               //:           WHERE SourceLPLR.W_MetaType.Type = 2003
               RESULT = SetCursorFirstEntityByInteger( SourceLPLR, "W_MetaType", "Type", 2003, "" );
               //:SET CURSOR FIRST SourceLPLR.W_MetaDef
               //:           WHERE SourceLPLR.W_MetaDef.CPLR_ZKey = OldRecursiveLOD.DomainRec.ZKey
               GetIntegerFromAttribute( &lTempInteger_2, OldRecursiveLOD, "DomainRec", "ZKey" );
               RESULT = SetCursorFirstEntityByInteger( SourceLPLR, "W_MetaDef", "CPLR_ZKey", lTempInteger_2, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:DomainName = SourceLPLR.W_MetaDef.Name
                  GetVariableFromAttribute( DomainName, 0, 'S', 33, SourceLPLR, "W_MetaDef", "Name", "", 0 );
                  //:SET CURSOR FIRST CurrentLPLR.W_MetaDef
                  //:           WHERE CurrentLPLR.W_MetaDef.Name = DomainName
                  RESULT = SetCursorFirstEntityByString( CurrentLPLR, "W_MetaDef", "Name", DomainName, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:ErrorFlag = 0  // Indicate Domain found.
                     ErrorFlag = 0;
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
            //:IF ErrorFlag = 0
            if ( ErrorFlag == 0 )
            { 
               //:ActivateMetaOI( vSubtask, NewDomain, CurrentLPLR, zREFER_DOMAIN_META, 0 )
               ActivateMetaOI( vSubtask, &NewDomain, CurrentLPLR, zREFER_DOMAIN_META, 0 );
               //:INCLUDE NewRecursiveLOD.DomainRec FROM NewDomain.Domain
               RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "DomainRec", NewDomain, "Domain", zPOS_AFTER );
               //:DropMetaOI( vSubtask, NewDomain )
               DropMetaOI( vSubtask, NewDomain );

               //:ELSE
            } 
            else
            { 
               //:IF szAddER_AttributeFlag = "Y" AND SourceLPLR != 0
               if ( ZeidonStringCompare( szAddER_AttributeFlag, 1, 0, "Y", 1, 0, 2 ) == 0 && SourceLPLR != 0 )
               { 
                  //:// The request is to add ER entities and Domains as necessary.
                  //:DomainAddForMerge( NewDomain, SourceLPLR, CurrentLPLR,
                  //:                   CurrentLPLR.LPLR.wFullyQualifiedFileName,
                  //:                   DomainName, vSubtask )
                  GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), CurrentLPLR, "LPLR", "wFullyQualifiedFileName" );
                  oTZDGSRCO_DomainAddForMerge( &NewDomain, SourceLPLR, CurrentLPLR, szTempString_0, DomainName, vSubtask );
                  //:ELSE
               } 
               else
               { 
                  //:szMsg = "Domain (" + DomainName + ") does not exist."
                  ZeidonStringCopy( szMsg, 1, 0, "Domain (", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, DomainName, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") does not exist.", 1, 0, 255 );
                  //:MessageSend( vSubtask, "ZO00403", "Entity Copy",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "ZO00403", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:szMsg = "LOD (" + OldMainLOD.LOD.Name +
                  //:        ") will not be migrated."
                  GetVariableFromAttribute( szTempString_1, 0, 'S', 33, OldMainLOD, "LOD", "Name", "", 0 );
                  ZeidonStringCopy( szMsg, 1, 0, "LOD (", 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 255 );
                  ZeidonStringConcat( szMsg, 1, 0, ") will not be migrated.", 1, 0, 255 );
                  //:MessageSend( vSubtask, "ZO00404", "Entity Copy",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "ZO00404", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               } 

               //:END
            } 

            //:END
         } 

         //:END

         //:ELSE
      } 
      else
      { 
         //:// Include the ER Attr by name
         //://  might fail if the ER attr was renamed without re-save of the LOD
         //:n = 0
         n = 0;
         //:SET CURSOR FIRST NewERD.ER_Attribute
         //:           WHERE NewERD.ER_Attribute.Name = OldRecursiveLOD.ER_AttributeRec.Name
         GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldRecursiveLOD, "ER_AttributeRec", "Name" );
         RESULT = SetCursorFirstEntityByString( NewERD, "ER_Attribute", "Name", szTempString_2, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// If the request is to add the ER Attribute, do so here.
            //:IF szAddER_AttributeFlag = "Y"
            if ( ZeidonStringCompare( szAddER_AttributeFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:// This is an ER Attribute so add it to ER.
               //:nRC = LOD_CreateER_Attribute( OldRecursiveLOD,
               //:                              SourceLPLR,
               //:                              CurrentLPLR,
               //:                              NewERD,
               //:                              vSubtask )
               nRC = oTZZOLODO_LOD_CreateER_Attribute( OldRecursiveLOD, SourceLPLR, CurrentLPLR, NewERD, vSubtask );
               //:IF nRC < 0
               if ( nRC < 0 )
               { 
                  //:RETURN nRC
                  return( nRC );
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:IF SourceLPLR = 0
               if ( SourceLPLR == 0 )
               { 
                  //:// There is no SourceLPLR, so skip this step and indicate attribute was not found.
                  //:n = 2    // 1 and 2 are not found. 0 is found.
                  n = 2;
                  //:ELSE
               } 
               else
               { 
                  //:n = GetERAttributeName( SourceLPLR, OldRecursiveLOD, ERAttributeName )
                  n = oTZZOLODO_GetERAttributeName( SourceLPLR, OldRecursiveLOD, ERAttributeName );
               } 

               //:END
               //:IF n = 0
               if ( n == 0 )
               { 
                  //:SET CURSOR FIRST NewERD.ER_Attribute
                  //:           WHERE NewERD.ER_Attribute.Name = ERAttributeName
                  RESULT = SetCursorFirstEntityByString( NewERD, "ER_Attribute", "Name", ERAttributeName, "" );
                  //:IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //:n = 1
                     n = 1;
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //:IF n = 0
         if ( n == 0 )
         { 
            //:INCLUDE NewRecursiveLOD.ER_AttributeRec FROM NewERD.ER_Attribute
            RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "ER_AttributeRec", NewERD, "ER_Attribute", zPOS_AFTER );
            //:ELSE
         } 
         else
         { 
            //:// If the ER AttributeRec in the Source does not exist in the target, drop the LOD_AttributeRec entry.
            //:DELETE ENTITY NewRecursiveLOD.LOD_AttributeRec NONE
            RESULT = DeleteEntity( NewRecursiveLOD, "LOD_AttributeRec", zREPOS_NONE );
         } 

         //:   //szMsg = "The following attribute in the Source E/R was not found in the Target E/R." +
         //:   //  NEW_LINE + NEW_LINE + OldRecursiveLOD.ER_AttributeRec.Name
         //:   //MessageSend( vSubtask, "ZO00406", "Entity Copy",
         //:   //             szMsg,
         //:   //             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         //:   //ErrorFlag = 1
         //:END
      } 

      //:END
   } 

   //:END

   //://  If OldRecursiveLOD.LOD_AttrDerivationOperRec exists, create a relationship to the
   //://  corresponding operation in the new LOD.
   //://  If the operation doesn't exist in the new LOD, ignore the LOD_AttrDerivationOperRec.
   //:IF OldRecursiveLOD.LOD_AttrDerivationOperRec EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( OldRecursiveLOD, "LOD_AttrDerivationOperRec" );
   if ( lTempInteger_3 == 0 )
   { 

      //:SET CURSOR FIRST NewMainLOD.Operation WITHIN NewMainLOD.LOD WHERE
      //:                 NewMainLOD.Operation.Name = OldRecursiveLOD.LOD_AttrDerivationOperRec.Name
      GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldRecursiveLOD, "LOD_AttrDerivationOperRec", "Name" );
      RESULT = SetCursorFirstEntityByString( NewMainLOD, "Operation", "Name", szTempString_2, "LOD" );
      //:IF RESULT >= 0
      if ( RESULT >= 0 )
      { 
         //:INCLUDE NewRecursiveLOD.LOD_AttrDerivationOperRec FROM NewMainLOD.Operation
         RESULT = IncludeSubobjectFromSubobject( NewRecursiveLOD, "LOD_AttrDerivationOperRec", NewMainLOD, "Operation", zPOS_AFTER );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_OperationCopy( VIEW vSubtask,
//:                   VIEW NewMainLOD BASED ON LOD TZZOLODO,
//:                   VIEW OldMainLOD BASED ON LOD TZZOLODO,
//:                   VIEW SourceLPLR BASED ON LOD TZCMLPLO )

//:   VIEW DirLPLR BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_OperationCopy( zVIEW     vSubtask,
                             zVIEW     NewMainLOD,
                             zVIEW     OldMainLOD,
                             zVIEW     SourceLPLR )
{
   zVIEW     DirLPLR = 0; 

   //:STRING ( 40 )  SourceName
   zCHAR     SourceName[ 41 ] = { 0 }; 
   //:STRING ( 32 )  MetaName
   zCHAR     MetaName[ 33 ] = { 0 }; 
   //:STRING ( 9 )   ExtensionName
   zCHAR     ExtensionName[ 10 ] = { 0 }; 
   //:STRING ( 513 ) SourceFileName1           // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName1[ 514 ] = { 0 }; 
   //:STRING ( 513 ) SourceFileName2           // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName2[ 514 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 


   //:FOR EACH OldMainLOD.SourceFile
   RESULT = SetCursorFirstEntity( OldMainLOD, "SourceFile", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// If there is no structure underneath the SourceFile entity in the
      //:// old LOD, ignore it..
      //:// If there is a structure underneath the SourceFile entity in the old LOD,
      //:// simply copy it over.

      //:IF OldMainLOD.Operation EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( OldMainLOD, "Operation" );
      if ( lTempInteger_0 == 0 )
      { 

         //:ExtensionName = OldMainLOD.SourceFile.Extension
         GetVariableFromAttribute( ExtensionName, 0, 'S', 10, OldMainLOD, "SourceFile", "Extension", "", 0 );
         //:MetaName = OldMainLOD.SourceFile.Name
         GetVariableFromAttribute( MetaName, 0, 'S', 33, OldMainLOD, "SourceFile", "Name", "", 0 );

         //:CreateMetaEntity( vSubtask, NewMainLOD, "SourceFile", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewMainLOD, "SourceFile", zPOS_AFTER );
         //:SetMatchingAttributesByName ( NewMainLOD, "SourceFile",
         //:                              OldMainLOD, "SourceFile", zSET_NULL )
         SetMatchingAttributesByName( NewMainLOD, "SourceFile", OldMainLOD, "SourceFile", zSET_NULL );

         //:FOR EACH OldMainLOD.Operation
         RESULT = SetCursorFirstEntity( OldMainLOD, "Operation", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 

            //:CreateMetaEntity( vSubtask, NewMainLOD, "Operation", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewMainLOD, "Operation", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewMainLOD, "Operation",
            //:                              OldMainLOD, "Operation", zSET_NULL )
            SetMatchingAttributesByName( NewMainLOD, "Operation", OldMainLOD, "Operation", zSET_NULL );
            //:FOR EACH OldMainLOD.Parameter
            RESULT = SetCursorFirstEntity( OldMainLOD, "Parameter", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( vSubtask, NewMainLOD, "Parameter", zPOS_AFTER )
               CreateMetaEntity( vSubtask, NewMainLOD, "Parameter", zPOS_AFTER );
               //:SetMatchingAttributesByName ( NewMainLOD, "Parameter",
               //:                              OldMainLOD, "Parameter", zSET_NULL )
               SetMatchingAttributesByName( NewMainLOD, "Parameter", OldMainLOD, "Parameter", zSET_NULL );
               RESULT = SetCursorNextEntity( OldMainLOD, "Parameter", "" );
            } 

            RESULT = SetCursorNextEntity( OldMainLOD, "Operation", "" );
            //:END
         } 


         //:END
      } 


      //:END

      //:// Copy the .C file or the .VML file, if it exists, from the source directory to the
      //:// target directory.
      //:// Note that if it isn't there (it wasn't created in the source LPLR), we're going to
      //:// just ignore it; we're not going to return an error message.
      //:RetrieveViewForMetaList( vSubtask, DirLPLR, zSOURCE_ERD_META ) // Get a view for directory info.
      RetrieveViewForMetaList( vSubtask, &DirLPLR, zSOURCE_ERD_META );
      //:ResetViewFromSubobject( DirLPLR ) // Get visibility to root.
      ResetViewFromSubobject( DirLPLR );
      //:IF ExtensionName = "C"
      if ( ZeidonStringCompare( ExtensionName, 1, 0, "C", 1, 0, 10 ) == 0 )
      { 
         //:SourceName = MetaName + ".C"
         ZeidonStringCopy( SourceName, 1, 0, MetaName, 1, 0, 41 );
         ZeidonStringConcat( SourceName, 1, 0, ".C", 1, 0, 41 );
         //:ELSE
      } 
      else
      { 
         //:SourceName = MetaName + ".VML"
         ZeidonStringCopy( SourceName, 1, 0, MetaName, 1, 0, 41 );
         ZeidonStringConcat( SourceName, 1, 0, ".VML", 1, 0, 41 );
      } 

      //:END
      //:SourceFileName1 = SourceLPLR.LPLR.PgmSrcDir + "\" + SourceName
      GetStringFromAttribute( SourceFileName1, sizeof( SourceFileName1 ), SourceLPLR, "LPLR", "PgmSrcDir" );
      ZeidonStringConcat( SourceFileName1, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName1, 1, 0, SourceName, 1, 0, 514 );
      //:SourceFileName2 = DirLPLR.LPLR.PgmSrcDir + "\" + SourceName
      GetStringFromAttribute( SourceFileName2, sizeof( SourceFileName2 ), DirLPLR, "LPLR", "PgmSrcDir" );
      ZeidonStringConcat( SourceFileName2, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName2, 1, 0, SourceName, 1, 0, 514 );
      //:SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE )
      SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE );
      //:DropView( DirLPLR )
      DropView( DirLPLR );

      //:IF OldMainLOD.LOD_ConstraintOper EXISTS AND
      lTempInteger_1 = CheckExistenceOfEntity( OldMainLOD, "LOD_ConstraintOper" );
      //:   OldMainLOD.SourceFileForOCOper.Name = OldMainLOD.SourceFile.Name
      if ( lTempInteger_1 == 0 && CompareAttributeToAttribute( OldMainLOD, "SourceFileForOCOper", "Name", OldMainLOD, "SourceFile", "Name" ) == 0 )
      { 

         //:SET CURSOR FIRST NewMainLOD.Operation WHERE
         //:       NewMainLOD.Operation.Name = OldMainLOD.LOD_ConstraintOper.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldMainLOD, "LOD_ConstraintOper", "Name" );
         RESULT = SetCursorFirstEntityByString( NewMainLOD, "Operation", "Name", szTempString_0, "" );

         //:INCLUDE NewMainLOD.LOD_ConstraintOper FROM NewMainLOD.Operation
         RESULT = IncludeSubobjectFromSubobject( NewMainLOD, "LOD_ConstraintOper", NewMainLOD, "Operation", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( OldMainLOD, "SourceFile", "" );

      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:ENTITY CONSTRAINT OPERATION
//:AttributeConstraints( VIEW   vLOD BASED ON LOD TZZOLODO,
//:                      STRING ( 32 ) sEntityName,
//:                      SHORT Event,
//:                      SHORT State )

//:   STRING (32) szValue
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_AttributeConstraints( zVIEW     vLOD,
                                zPCHAR    sEntityName,
                                zSHORT    Event,
                                zSHORT    State )
{
   zCHAR     szValue[ 33 ] = { 0 }; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 


   //:CASE Event
   switch( Event )
   { 
      //:OF   zECE_ACCEPT:
      case zECE_ACCEPT :

         //:TraceLineS( "DGC Accept constraint entity = ", sEntityName )
         TraceLineS( "DGC Accept constraint entity = ", sEntityName );

         //:// Validate that there are no two attributes with the same name.
         //:GetStringFromAttribute ( szValue, vLOD, "ER_Attribute", "Name" )
         GetStringFromAttribute( szValue, sizeof( szValue ), vLOD, "ER_Attribute", "Name" );
         //:nRC = CheckDuplicateEntityByString( vLOD, "ER_Attribute", "Name", szValue, "LOD_Entity" )
         nRC = CheckDuplicateEntityByString( vLOD, "ER_Attribute", "Name", szValue, "LOD_Entity" );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MessageSend( vLOD, "ZO00415", "Zeidon Object Maintenance",
            //:            "Duplicate Attribute for Entity",
            //:            zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vLOD, "ZO00415", "Zeidon Object Maintenance", "Duplicate Attribute for Entity", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:RETURN -1
            return( -1 );
         } 

         //:END

         //:// Don't allow a derived operation on an attribute that was
         //:// derived at the ER level (ie., has a DerivedExpression value that
         //:// isn't null.
         //:IF vLOD.ER_Attribute.DerivedExpression != "" AND
         //:  vLOD.LOD_AttrDerivationOper EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( vLOD, "LOD_AttrDerivationOper" );
         if ( CompareAttributeToString( vLOD, "ER_Attribute", "DerivedExpression", "" ) != 0 && lTempInteger_0 == 0 )
         { 

            //:MessageSend( vLOD, "ZO00416", "Zeidon Object Maintenance",
            //:            "A Derived Operation is not valid if ER Entity is a Derived Expression.",
            //:            zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vLOD, "ZO00416", "Zeidon Object Maintenance", "A Derived Operation is not valid if ER Entity is a Derived Expression.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:RETURN -1
            return( -1 );
         } 

         //:END

         //:// If insert new Attribute, then set new Attribute Sequence
         //:NewAttributeSequence( vLOD )
         oTZZOLODO_NewAttributeSequence( vLOD );
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

         //:// If delete Attribute, then set new Attribute Sequence
         //:DelAttribFromSequence( vLOD )
         oTZZOLODO_DelAttribFromSequence( vLOD );
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


//:LOCAL OPERATION
//:fnRelinkWorkAttrib( VIEW vLOD       BASED ON LOD TZZOLODO,
//:                    VIEW DomainLPLR BASED ON LOD TZCMLPLO,
//:                    VIEW vSubtask )

//:   VIEW  DomainLPLR2  BASED ON LOD TZCMLPLO
static zSHORT
oTZZOLODO_fnRelinkWorkAttrib( zVIEW     vLOD,
                              zVIEW     DomainLPLR,
                              zVIEW     vSubtask )
{
   zVIEW     DomainLPLR2 = 0; 
   //:VIEW  vDomain      BASED ON LOD TZDGSRCO
   zVIEW     vDomain = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// Relink Domains for attributes under a work entity.
   //:IF vLOD.Domain  EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vLOD, "Domain" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SET CURSOR FIRST DomainLPLR.W_MetaDef WHERE
      //:    DomainLPLR.W_MetaDef.CPLR_ZKey = vLOD.Domain.ZKey
      GetIntegerFromAttribute( &lTempInteger_1, vLOD, "Domain", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( DomainLPLR, "W_MetaDef", "CPLR_ZKey", lTempInteger_1, "" );
      //:IF RESULT = zCURSOR_SET
      if ( RESULT == zCURSOR_SET )
      { 
         //:CreateViewFromViewForTask( DomainLPLR2, DomainLPLR, 0 )
         CreateViewFromViewForTask( &DomainLPLR2, DomainLPLR, 0 );
         //:ActivateMetaOI( vSubtask, vDomain, DomainLPLR2, zREFER_DOMAIN_META, 0 )
         ActivateMetaOI( vSubtask, &vDomain, DomainLPLR2, zREFER_DOMAIN_META, 0 );
         //:DropView ( DomainLPLR2 )
         DropView( DomainLPLR2 );
         //:RelinkInstanceToInstance( vLOD, "Domain", vDomain, "Domain" )
         RelinkInstanceToInstance( vLOD, "Domain", vDomain, "Domain" );
         //:DropMetaOI( vSubtask, vDomain )
         DropMetaOI( vSubtask, vDomain );

         //:ELSE
      } 
      else
      { 
         //:SET CURSOR FIRST DomainLPLR.W_MetaDef WHERE
         //:    DomainLPLR.W_MetaDef.Name = vLOD.Domain.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vLOD, "Domain", "Name" );
         RESULT = SetCursorFirstEntityByString( DomainLPLR, "W_MetaDef", "Name", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR FIRST DomainLPLR.W_MetaDef WHERE
            //:    DomainLPLR.W_MetaDef.Name = "Text"
            RESULT = SetCursorFirstEntityByString( DomainLPLR, "W_MetaDef", "Name", "Text", "" );
         } 

         //:END
         //:IF RESULT = zCURSOR_SET
         if ( RESULT == zCURSOR_SET )
         { 
            //:CreateViewFromViewForTask( DomainLPLR2, DomainLPLR, 0 )
            CreateViewFromViewForTask( &DomainLPLR2, DomainLPLR, 0 );
            //:ActivateMetaOI( vSubtask, vDomain, DomainLPLR2, zREFER_DOMAIN_META, 0 )
            ActivateMetaOI( vSubtask, &vDomain, DomainLPLR2, zREFER_DOMAIN_META, 0 );
            //:DropView( DomainLPLR2 )
            DropView( DomainLPLR2 );
            //:EXCLUDE vLOD.Domain
            RESULT = ExcludeEntity( vLOD, "Domain", zREPOS_AFTER );
            //:INCLUDE vLOD.Domain FROM vDomain.Domain
            RESULT = IncludeSubobjectFromSubobject( vLOD, "Domain", vDomain, "Domain", zPOS_AFTER );
            //:DropMetaOI( vSubtask, vDomain )
            DropMetaOI( vSubtask, vDomain );
            //:ELSE
         } 
         else
         { 
            //:// If the Domain isn't in LPLR, it must have been deleted.  Thereby
            //:// exclude the Domain entity.  We'll leave the LOD_Attribute without
            //:// any Domain.
            //:MessageSend( vSubtask, "ZO00412", "LOD Relink",
            //:             "Excluding Domain",
            //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "ZO00412", "LOD Relink", "Excluding Domain", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:EXCLUDE vLOD.Domain
            RESULT = ExcludeEntity( vLOD, "Domain", zREPOS_AFTER );
         } 

         //:END
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DERIVED ATTRIBUTE OPERATION
//:DIL_NLS_Text( VIEW          vLOD BASED ON LOD TZZOLODO,
//:              STRING ( 32 ) lpEntity,
//:              STRING ( 32 ) lpAttribute,
//:              SHORT         GetOrSetFlag )

//:// LAST MESSAGE ID: ZO00416

//:   SHORT        nLanguageCode
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DIL_NLS_Text( zVIEW     vLOD,
                        LPVIEWENTITY lpEntity,
                        LPVIEWATTRIB lpAttribute,
                        zSHORT    GetOrSetFlag )
{
   zSHORT    nLanguageCode = 0; 
   //:STRING (254) szDIL_Text
   zCHAR     szDIL_Text[ 255 ] = { 0 }; 
   zSHORT    RESULT; 


   //:SysGetLanguageCode ( nLanguageCode )
   SysGetLanguageCode( &nLanguageCode );

   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :

         //:// Determine DIL text to use by checking the NLS Language Code for a match on
         //:// a DIL_NLS_Text entity.  If there is a match, use it.  Otherwise, use the
         //:// LOD_Attribute.DIL_Text attribute.

         //:SET CURSOR FIRST vLOD.DIL_NLS_Text  WHERE
         //:              vLOD.DIL_NLS_Text.LanguageIndex  = nLanguageCode
         RESULT = SetCursorFirstEntityByInteger( vLOD, "DIL_NLS_Text", "LanguageIndex", nLanguageCode, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szDIL_Text = vLOD.DIL_NLS_Text.Text
            GetVariableFromAttribute( szDIL_Text, 0, 'S', 255, vLOD, "DIL_NLS_Text", "Text", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szDIL_Text = vLOD.LOD_Attribute.DIL_Text
            GetVariableFromAttribute( szDIL_Text, 0, 'S', 255, vLOD, "LOD_Attribute", "DIL_Text", "", 0 );
         } 

         //:END

         //:StoreStringInRecord ( vLOD, lpEntity, lpAttribute, szDIL_Text )
         StoreStringInRecord( vLOD, lpEntity, lpAttribute, szDIL_Text );
         break ;

      //:  /* end zDERIVED_GET */
      //:OF   zDERIVED_SET:
      case zDERIVED_SET :

         //:// As for zDERIVED_GET above, use the Language Code to store the data entered into
         //:// the derived attribute.  If there is DIL_NLS_Text entity that matches the
         //:// Language Code, then set the data there.  If not, set it into the
         //:// LOD_Attribute.DIL_Text attribute.

         //:GetStringFromRecord ( vLOD, lpEntity, lpAttribute, szDIL_Text, 254 )
         GetStringFromRecord( vLOD, lpEntity, lpAttribute, szDIL_Text, 254 );

         //:SET CURSOR FIRST vLOD.DIL_NLS_Text  WHERE
         //:              vLOD.DIL_NLS_Text.LanguageIndex  = nLanguageCode
         RESULT = SetCursorFirstEntityByInteger( vLOD, "DIL_NLS_Text", "LanguageIndex", nLanguageCode, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:vLOD.DIL_NLS_Text.Text = szDIL_Text
            SetAttributeFromString( vLOD, "DIL_NLS_Text", "Text", szDIL_Text );
            //:ELSE
         } 
         else
         { 
            //:vLOD.LOD_Attribute.DIL_Text = szDIL_Text
            SetAttributeFromString( vLOD, "LOD_Attribute", "DIL_Text", szDIL_Text );
         } 

         //:END
         break ;
   } 


   //:     /* end zDERIVED_SET */
   //:END  /* case */
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_Migrate( VIEW          NewMainLOD BASED ON LOD TZZOLODO,
//:             STRING ( 32 ) LOD_Name,
//:             VIEW          SourceLPLR BASED ON LOD TZCMLPLO,
//:             VIEW          vSubtask )

//:   VIEW SourceLPLR2      BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_Migrate( zVIEW     NewMainLOD,
                       zPCHAR    LOD_Name,
                       zVIEW     SourceLPLR,
                       zVIEW     vSubtask )
{
   zVIEW     SourceLPLR2 = 0; 
   //:VIEW NewRecursiveLOD  BASED ON LOD TZZOLODO
   zVIEW     NewRecursiveLOD = 0; 
   //:VIEW OldMainLOD       BASED ON LOD TZZOLODO
   zVIEW     OldMainLOD = 0; 
   //:VIEW OldRecursiveLOD  BASED ON LOD TZZOLODO
   zVIEW     OldRecursiveLOD = 0; 
   //:VIEW NewERD           BASED ON LOD TZEREMDO
   zVIEW     NewERD = 0; 
   //:VIEW SourceERD        BASED ON LOD TZEREMDO
   zVIEW     SourceERD = 0; 
   //:VIEW CurrentLPLR      BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 
   //:VIEW TempLPLR         BASED ON LOD TZCMLPLO
   zVIEW     TempLPLR = 0; 
   //:VIEW SourceVOR_LPLR   BASED ON LOD TZCMLPLO
   zVIEW     SourceVOR_LPLR = 0; 
   //:VIEW TargetVOR_LPLR   BASED ON LOD TZCMLPLO
   zVIEW     TargetVOR_LPLR = 0; 
   //:VIEW ReturnedTE       BASED ON LOD TZTENVRO
   zVIEW     ReturnedTE = 0; 
   //:VIEW OldTE            BASED ON LOD TZTENVRO
   zVIEW     OldTE = 0; 
   //:VIEW NewVOR           BASED ON LOD TZWDVORO
   zVIEW     NewVOR = 0; 

   //:STRING ( 513 ) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) TargetFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     TargetFileName[ 514 ] = { 0 }; 
   //:STRING ( 40 )  SourceName
   zCHAR     SourceName[ 41 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 200 ) szFileName
   zCHAR     szFileName[ 201 ] = { 0 }; 
   //:STRING ( 1 )   ModifyFlag
   zCHAR     ModifyFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   MergeOperationsFlag
   zCHAR     MergeOperationsFlag[ 2 ] = { 0 }; 
   //:STRING ( 9 )   ExtensionName
   zCHAR     ExtensionName[ 10 ] = { 0 }; 

   //:INTEGER        ErrorFlag
   zLONG     ErrorFlag = 0; 
   //:INTEGER        ERD_Flag
   zLONG     ERD_Flag = 0; 
   //:INTEGER        Offset
   zLONG     Offset = 0; 
   //:SHORT          StringLength
   zSHORT    StringLength = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zSHORT    lTempInteger_8; 
   zSHORT    lTempInteger_9; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 


   //:ErrorFlag = 0
   ErrorFlag = 0;
   //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zREFER_ERD_META );
   //:ResetViewFromSubobject( CurrentLPLR ) // Get visibility to root.
   ResetViewFromSubobject( CurrentLPLR );

   //:// Activate existing source meta OldMainLOD
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + LOD_Name + ".LOD"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, LOD_Name, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".LOD", 1, 0, 514 );
   //:ActivateOI_FromFile ( OldMainLOD, "TZZOLODO", CurrentLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &OldMainLOD, "TZZOLODO", CurrentLPLR, SourceFileName, 8192 );
   //:// 8192 is zIGNORE_ATTRIB_ERRORS
   //:NAME VIEW OldMainLOD "OldMainLOD"
   SetNameForView( OldMainLOD, "OldMainLOD", 0, zLEVEL_TASK );

   //:// Activate existing source ERD.
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + SourceLPLR.LPLR.Name + ".PMD"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, SourceLPLR, "LPLR", "Name", "", 0 );
   ZeidonStringConcat( SourceFileName, 1, 0, szTempString_0, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PMD", 1, 0, 514 );
   //:ActivateOI_FromFile ( SourceERD, "TZEREMDO", CurrentLPLR, SourceFileName, 8192 )   // 8192 is zIGNORE_ATTRIB_ERRORS
   ActivateOI_FromFile( &SourceERD, "TZEREMDO", CurrentLPLR, SourceFileName, 8192 );
   //:NAME VIEW SourceERD "SourceERD"
   SetNameForView( SourceERD, "SourceERD", 0, zLEVEL_TASK );

   //:// Activate the current ERD.  If it doesn't exist, it is an error.
   //:// It is also an error, if the LOD references a Domain that is not in the LPLR, but
   //:// this is checked inside the code of LOD_EntityCopy.
   //:GET VIEW NewERD NAMED "NewERD"
   RESULT = GetViewByName( &NewERD, "NewERD", NewMainLOD, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:IF CurrentLPLR.W_MetaDef EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( CurrentLPLR, "W_MetaDef" );
      if ( lTempInteger_0 == 0 )
      { 
         //:nRC = ActivateMetaOI( vSubtask, NewERD, CurrentLPLR, zREFER_ERD_META, 0 )
         nRC = ActivateMetaOI( vSubtask, &NewERD, CurrentLPLR, zREFER_ERD_META, 0 );
         //:NAME VIEW NewERD "NewERD"
         SetNameForView( NewERD, "NewERD", 0, zLEVEL_TASK );
         //:ELSE
      } 
      else
      { 
         //:nRC = -1
         nRC = -1;
      } 

      //:END

      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:// Unless the LOD is a work lod, it is an error to migrate it without an
         //:// ERD. The only way we know this is to loop through all the entities
         //:// to see if any of them are against the ERD.
         //:ERD_Flag = 0
         ERD_Flag = 0;
         //:FOR EACH OldMainLOD.LOD_Entity
         RESULT = SetCursorFirstEntity( OldMainLOD, "LOD_Entity", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:IF OldMainLOD.LOD_Entity.Work != "Y"
            if ( CompareAttributeToString( OldMainLOD, "LOD_Entity", "Work", "Y" ) != 0 )
            { 
               //:ERD_Flag = 1
               ERD_Flag = 1;
            } 

            RESULT = SetCursorNextEntity( OldMainLOD, "LOD_Entity", "" );
            //:END
         } 

         //:END
         //:IF ERD_Flag = 1
         if ( ERD_Flag == 1 )
         { 
            //:MessageSend( vSubtask, "ZO00401", "LOD Migration",
            //:             "No current Data Model exists.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "ZO00401", "LOD Migration", "No current Data Model exists.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:RETURN -1
            return( -1 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// If the LOD entity doesn't exist, this is a new LOD and we must create it.
   //:IF NewMainLOD.LOD DOES NOT EXIST
   lTempInteger_1 = CheckExistenceOfEntity( NewMainLOD, "LOD" );
   if ( lTempInteger_1 != 0 )
   { 
      //:CreateMetaEntity( vSubtask, NewMainLOD, "LOD", zPOS_AFTER )
      CreateMetaEntity( vSubtask, NewMainLOD, "LOD", zPOS_AFTER );
      //:  // Can't use SetMatchingAttributes operation because of derived attribute from a
      //:  // different LPLR.
      //:  NewMainLOD.LOD.LastSyncDate            = OldMainLOD.LOD.LastSyncDate
      SetAttributeFromAttribute( NewMainLOD, "LOD", "LastSyncDate", OldMainLOD, "LOD", "LastSyncDate" );
      //:  NewMainLOD.LOD.Name                    = OldMainLOD.LOD.Name
      SetAttributeFromAttribute( NewMainLOD, "LOD", "Name", OldMainLOD, "LOD", "Name" );
      //:  NewMainLOD.LOD.Desc                    = OldMainLOD.LOD.Desc
      SetAttributeFromAttribute( NewMainLOD, "LOD", "Desc", OldMainLOD, "LOD", "Desc" );
      //:  NewMainLOD.LOD.ReLinked                = OldMainLOD.LOD.ReLinked
      SetAttributeFromAttribute( NewMainLOD, "LOD", "ReLinked", OldMainLOD, "LOD", "ReLinked" );
      //:  NewMainLOD.LOD.ActivateConstraint      = OldMainLOD.LOD.ActivateConstraint
      SetAttributeFromAttribute( NewMainLOD, "LOD", "ActivateConstraint", OldMainLOD, "LOD", "ActivateConstraint" );
      //:  NewMainLOD.LOD.ActivateEmptyConstraint = OldMainLOD.LOD.ActivateEmptyConstraint
      SetAttributeFromAttribute( NewMainLOD, "LOD", "ActivateEmptyConstraint", OldMainLOD, "LOD", "ActivateEmptyConstraint" );
      //:  NewMainLOD.LOD.CommitConstraint        = OldMainLOD.LOD.CommitConstraint
      SetAttributeFromAttribute( NewMainLOD, "LOD", "CommitConstraint", OldMainLOD, "LOD", "CommitConstraint" );
      //:  NewMainLOD.LOD.DropOIConstraint        = OldMainLOD.LOD.DropOIConstraint
      SetAttributeFromAttribute( NewMainLOD, "LOD", "DropOIConstraint", OldMainLOD, "LOD", "DropOIConstraint" );
      //:  NewMainLOD.LOD.DLL_Name                = OldMainLOD.LOD.DLL_Name
      SetAttributeFromAttribute( NewMainLOD, "LOD", "DLL_Name", OldMainLOD, "LOD", "DLL_Name" );
      //:  NewMainLOD.LOD.CacheNumberPerEntity    = OldMainLOD.LOD.CacheNumberPerEntity
      SetAttributeFromAttribute( NewMainLOD, "LOD", "CacheNumberPerEntity", OldMainLOD, "LOD", "CacheNumberPerEntity" );
   } 

   //:END

   //:// If the LOD_EntityParent entity exists, then we are merging Operations into a current LOD and not changing
   //:// any of the Entities themselves.
   //:// If the LOD_EntityParent entity doesn't exist, then we are either replacing the whole LOD or creating a new one,
   //:// and all data from the source LOD is recreated in the target LOD.
   //:IF NewMainLOD.LOD_EntityParent DOES NOT EXIST
   lTempInteger_2 = CheckExistenceOfEntity( NewMainLOD, "LOD_EntityParent" );
   if ( lTempInteger_2 != 0 )
   { 

      //:CreateViewFromViewForTask( NewRecursiveLOD, NewMainLOD, 0 )
      CreateViewFromViewForTask( &NewRecursiveLOD, NewMainLOD, 0 );
      //:CreateViewFromViewForTask( OldRecursiveLOD, OldMainLOD, 0 )
      CreateViewFromViewForTask( &OldRecursiveLOD, OldMainLOD, 0 );
      //:NAME VIEW NewRecursiveLOD "NewRecursiveLOD"
      SetNameForView( NewRecursiveLOD, "NewRecursiveLOD", 0, zLEVEL_TASK );
      //:NAME VIEW OldRecursiveLOD "OldRecursiveLOD"
      SetNameForView( OldRecursiveLOD, "OldRecursiveLOD", 0, zLEVEL_TASK );

      //:// Go to copy Source Files and Operations.
      //:LOD_OperationCopy( vSubtask, NewMainLOD, OldMainLOD, SourceLPLR )
      oTZZOLODO_LOD_OperationCopy( vSubtask, NewMainLOD, OldMainLOD, SourceLPLR );

      //:// Force the activation of the TE, because it is needed by the XOD build routine,
      //:// which requires that the TE and ER have been linked.
      //:// Also get the old TE, for retrieving up-to-date POD information.
      //:GET VIEW ReturnedTE NAMED "TE_DB_Environ"
      RESULT = GetViewByName( &ReturnedTE, "TE_DB_Environ", NewMainLOD, zLEVEL_TASK );
      //:IF RESULT < 0
      if ( RESULT < 0 )
      { 
         //:RetrieveViewForMetaList( vSubtask, TempLPLR, zREFER_DTE_META )
         RetrieveViewForMetaList( vSubtask, &TempLPLR, zREFER_DTE_META );
         //:IF TempLPLR.W_MetaDef EXISTS
         lTempInteger_3 = CheckExistenceOfEntity( TempLPLR, "W_MetaDef" );
         if ( lTempInteger_3 == 0 )
         { 
            //:ActivateMetaOI( vSubtask, ReturnedTE, TempLPLR, zREFER_DTE_META, 0 )
            ActivateMetaOI( vSubtask, &ReturnedTE, TempLPLR, zREFER_DTE_META, 0 );
            //:// Name for building of XOD.
            //:NAME VIEW ReturnedTE "TE_DB_Environ"
            SetNameForView( ReturnedTE, "TE_DB_Environ", 0, zLEVEL_TASK );
            //:// Get old TE or set view to zero.
            //:CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, 0 )
            CreateViewFromViewForTask( &SourceLPLR2, SourceLPLR, 0 );
            //:SET CURSOR FIRST SourceLPLR2.W_MetaType WHERE SourceLPLR2.W_MetaType.Type = 2006
            RESULT = SetCursorFirstEntityByInteger( SourceLPLR2, "W_MetaType", "Type", 2006, "" );
            //:IF SourceLPLR2.W_MetaDef EXISTS
            lTempInteger_4 = CheckExistenceOfEntity( SourceLPLR2, "W_MetaDef" );
            if ( lTempInteger_4 == 0 )
            { 
               //:SourceFileName = SourceLPLR2.LPLR.MetaSrcDir + "\" + SourceLPLR2.W_MetaDef.Name + ".DTE"
               GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR2, "LPLR", "MetaSrcDir" );
               ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
               GetVariableFromAttribute( szTempString_1, 0, 'S', 33, SourceLPLR2, "W_MetaDef", "Name", "", 0 );
               ZeidonStringConcat( SourceFileName, 1, 0, szTempString_1, 1, 0, 514 );
               ZeidonStringConcat( SourceFileName, 1, 0, ".DTE", 1, 0, 514 );
               //:ActivateOI_FromFile ( OldTE, "TZTENVRO", CurrentLPLR, SourceFileName, 8192 ) // 8192 is zIGNORE_ATTRIB_ERRORS
               ActivateOI_FromFile( &OldTE, "TZTENVRO", CurrentLPLR, SourceFileName, 8192 );
               //:NAME VIEW OldTE "OldTE"
               SetNameForView( OldTE, "OldTE", 0, zLEVEL_TASK );
               //:DropView ( TempLPLR )
               DropView( TempLPLR );
               //:ELSE
            } 
            else
            { 
               //:OldTE = 0
               OldTE = 0;
            } 

            //:END
            //:DropView( SourceLPLR2 )
            DropView( SourceLPLR2 );
            //:ELSE
         } 
         else
         { 
            //:ReturnedTE = 0
            ReturnedTE = 0;
            //:OldTE = 0
            OldTE = 0;
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:GET VIEW OldTE NAMED "OldTE"
         RESULT = GetViewByName( &OldTE, "OldTE", NewMainLOD, zLEVEL_TASK );
      } 

      //:END

      //:CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, 0 )
      CreateViewFromViewForTask( &SourceLPLR2, SourceLPLR, 0 );
      //:// If the old LOD was saved with TE and there is a TE in new LPLR, copy POD.
      //:IF OldMainLOD.POD EXISTS AND OldTE != 0
      lTempInteger_5 = CheckExistenceOfEntity( OldMainLOD, "POD" );
      if ( lTempInteger_5 == 0 && OldTE != 0 )
      { 

         //:// Try to set cursor on DBMS_Source which corresponds to the old POD.  If a
         //:// match, continue to process the POD.  Otherwise position on first DBMS_Source.
         //:SET CURSOR  FIRST OldMainLOD.TE_DBMS_Source WITHIN OldMainLOD.POD WHERE
         //:           OldMainLOD.TE_DBMS_Source.ZKey = OldMainLOD.POD.TE_SourceZKey
         GetIntegerFromAttribute( &lTempInteger_6, OldMainLOD, "POD", "TE_SourceZKey" );
         RESULT = SetCursorFirstEntityByInteger( OldMainLOD, "TE_DBMS_Source", "ZKey", lTempInteger_6, "POD" );
         //:SET CURSOR FIRST OldTE.TE_DBMS_Source WHERE
         //:           OldTE.TE_DBMS_Source.ZKey = OldMainLOD.TE_DBMS_Source.ZKey
         GetIntegerFromAttribute( &lTempInteger_7, OldMainLOD, "TE_DBMS_Source", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( OldTE, "TE_DBMS_Source", "ZKey", lTempInteger_7, "" );
         //:SET CURSOR FIRST ReturnedTE.TE_DBMS_Source WHERE
         //:           ReturnedTE.TE_DBMS_Source.Name = OldTE.TE_DBMS_Source.Name
         GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldTE, "TE_DBMS_Source", "Name" );
         RESULT = SetCursorFirstEntityByString( ReturnedTE, "TE_DBMS_Source", "Name", szTempString_2, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// Try to position on first TE.
            //:SET CURSOR FIRST ReturnedTE.TE_DBMS_Source
            RESULT = SetCursorFirstEntity( ReturnedTE, "TE_DBMS_Source", "" );
         } 

         //:END
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:CreateMetaEntity( vSubtask, NewMainLOD, "POD", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewMainLOD, "POD", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewMainLOD, "POD", OldMainLOD, "POD", zSET_NULL )
            SetMatchingAttributesByName( NewMainLOD, "POD", OldMainLOD, "POD", zSET_NULL );
            //:INCLUDE NewMainLOD.TE_DB_Environ FROM ReturnedTE.TE_DB_Environ
            RESULT = IncludeSubobjectFromSubobject( NewMainLOD, "TE_DB_Environ", ReturnedTE, "TE_DB_Environ", zPOS_AFTER );
            //:NewMainLOD.POD.TE_SourceZKey = ReturnedTE.TE_DBMS_Source.ZKey
            SetAttributeFromAttribute( NewMainLOD, "POD", "TE_SourceZKey", ReturnedTE, "TE_DBMS_Source", "ZKey" );
         } 

         //:END
      } 

      //:END

      //:FOR EACH OldRecursiveLOD.LOD_EntityParent
      RESULT = SetCursorFirstEntity( OldRecursiveLOD, "LOD_EntityParent", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:nRC = LOD_EntityCopy( NewMainLOD,
         //:                      NewRecursiveLOD,
         //:                      OldMainLOD,
         //:                      OldRecursiveLOD,
         //:                      SourceLPLR2,
         //:                      CurrentLPLR,
         //:                      NewERD,
         //:                      SourceERD,
         //:                      ReturnedTE,
         //:                      vSubtask,
         //:                      "",
         //:                      "" )
         nRC = oTZZOLODO_LOD_EntityCopy( NewMainLOD, NewRecursiveLOD, OldMainLOD, OldRecursiveLOD, SourceLPLR2, CurrentLPLR, NewERD, SourceERD, ReturnedTE, vSubtask, "", "" );

         //:IF nRC != 0  // Pass along any errors
         if ( nRC != 0 )
         { 
            //:ErrorFlag = 1
            ErrorFlag = 1;
         } 

         RESULT = SetCursorNextEntity( OldRecursiveLOD, "LOD_EntityParent", "" );
         //:END
      } 


      //:END
      //:DropView( SourceLPLR2 )
      DropView( SourceLPLR2 );
      //:DropView( NewRecursiveLOD )
      DropView( NewRecursiveLOD );
      //:DropView( OldRecursiveLOD )
      DropView( OldRecursiveLOD );

      //:ELSE
   } 
   else
   { 

      //:// The LOD already exists, so we are only merging Operations (including source code) into the LOD.
      //:// Note that the following code does not handle the situation where Operations in the Source LPLR
      //:// have been moved from one Source File to another. It assumes that they're in the same Source File.
      //:FOR EACH OldMainLOD.SourceFile
      RESULT = SetCursorFirstEntity( OldMainLOD, "SourceFile", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// Initialize SourceFile.
         //:SET CURSOR FIRST NewMainLOD.SourceFile WHERE NewMainLOD.SourceFile.Name = OldMainLOD.SourceFile.Name
         GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldMainLOD, "SourceFile", "Name" );
         RESULT = SetCursorFirstEntityByString( NewMainLOD, "SourceFile", "Name", szTempString_2, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// Copy over the whole Source File, including Operations and Code.
            //:CreateMetaEntity( vSubtask, NewMainLOD, "SourceFile", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewMainLOD, "SourceFile", zPOS_AFTER );
            //:SetMatchingAttributesByName ( NewMainLOD, "SourceFile", OldMainLOD, "SourceFile", zSET_NULL )
            SetMatchingAttributesByName( NewMainLOD, "SourceFile", OldMainLOD, "SourceFile", zSET_NULL );

            //:// Set up Source and Target file names for Operations.
            //:ExtensionName = OldMainLOD.SourceFile.Extension
            GetVariableFromAttribute( ExtensionName, 0, 'S', 10, OldMainLOD, "SourceFile", "Extension", "", 0 );
            //:SourceFileName = SourceLPLR.LPLR.PgmSrcDir + "\" + OldMainLOD.SourceFile.Name + "." + ExtensionName
            GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "PgmSrcDir" );
            ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
            GetVariableFromAttribute( szTempString_2, 0, 'S', 33, OldMainLOD, "SourceFile", "Name", "", 0 );
            ZeidonStringConcat( SourceFileName, 1, 0, szTempString_2, 1, 0, 514 );
            ZeidonStringConcat( SourceFileName, 1, 0, ".", 1, 0, 514 );
            ZeidonStringConcat( SourceFileName, 1, 0, ExtensionName, 1, 0, 514 );
            //:TargetFileName = CurrentLPLR.LPLR.PgmSrcDir + "\" + NewMainLOD.SourceFile.Name + "." + ExtensionName
            GetStringFromAttribute( TargetFileName, sizeof( TargetFileName ), CurrentLPLR, "LPLR", "PgmSrcDir" );
            ZeidonStringConcat( TargetFileName, 1, 0, "\\", 1, 0, 514 );
            GetVariableFromAttribute( szTempString_3, 0, 'S', 33, NewMainLOD, "SourceFile", "Name", "", 0 );
            ZeidonStringConcat( TargetFileName, 1, 0, szTempString_3, 1, 0, 514 );
            ZeidonStringConcat( TargetFileName, 1, 0, ".", 1, 0, 514 );
            ZeidonStringConcat( TargetFileName, 1, 0, ExtensionName, 1, 0, 514 );

            //:// Copy the .C file or the .VML file, if it exists, from the source directory to the
            //:// target directory.
            //:SysCopyFile( vSubtask, SourceFileName, TargetFileName, TRUE )
            SysCopyFile( vSubtask, SourceFileName, TargetFileName, TRUE );

            //:MergeOperationsFlag = "N"
            ZeidonStringCopy( MergeOperationsFlag, 1, 0, "N", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 

            //:// Set up Source and Target file names for Operations.
            //:// Only set up merge Source for VML.
            //:ExtensionName = OldMainLOD.SourceFile.Extension
            GetVariableFromAttribute( ExtensionName, 0, 'S', 10, OldMainLOD, "SourceFile", "Extension", "", 0 );
            //:IF ExtensionName = "VML"
            if ( ZeidonStringCompare( ExtensionName, 1, 0, "VML", 1, 0, 10 ) == 0 )
            { 
               //:SourceFileName = SourceLPLR.LPLR.PgmSrcDir + "\" + OldMainLOD.SourceFile.Name + ".VML"
               GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "PgmSrcDir" );
               ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
               GetVariableFromAttribute( szTempString_4, 0, 'S', 33, OldMainLOD, "SourceFile", "Name", "", 0 );
               ZeidonStringConcat( SourceFileName, 1, 0, szTempString_4, 1, 0, 514 );
               ZeidonStringConcat( SourceFileName, 1, 0, ".VML", 1, 0, 514 );
               //:TargetFileName = CurrentLPLR.LPLR.PgmSrcDir + "\" + NewMainLOD.SourceFile.Name + ".VML"
               GetStringFromAttribute( TargetFileName, sizeof( TargetFileName ), CurrentLPLR, "LPLR", "PgmSrcDir" );
               ZeidonStringConcat( TargetFileName, 1, 0, "\\", 1, 0, 514 );
               GetVariableFromAttribute( szTempString_5, 0, 'S', 33, NewMainLOD, "SourceFile", "Name", "", 0 );
               ZeidonStringConcat( TargetFileName, 1, 0, szTempString_5, 1, 0, 514 );
               ZeidonStringConcat( TargetFileName, 1, 0, ".VML", 1, 0, 514 );
               //:ELSE
            } 
            else
            { 
               //:szMsg = "LOD Operations in Source, " + OldMainLOD.SourceFile.Name + ", will not be merged because the Source code is not VML."
               GetVariableFromAttribute( szTempString_6, 0, 'S', 33, OldMainLOD, "SourceFile", "Name", "", 0 );
               ZeidonStringCopy( szMsg, 1, 0, "LOD Operations in Source, ", 1, 0, 201 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_6, 1, 0, 201 );
               ZeidonStringConcat( szMsg, 1, 0, ", will not be merged because the Source code is not VML.", 1, 0, 201 );
               //:MessageSend( vSubtask, "ZO", "LOD Merge", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vSubtask, "ZO", "LOD Merge", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               //:SourceFileName = ""
               ZeidonStringCopy( SourceFileName, 1, 0, "", 1, 0, 514 );
               //:TargetFileName = ""
               ZeidonStringCopy( TargetFileName, 1, 0, "", 1, 0, 514 );
            } 

            //:END
            //:MergeOperationsFlag = "Y"
            ZeidonStringCopy( MergeOperationsFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END

         //:// Merge each Operation and related code, if requested.
         //:FOR EACH OldMainLOD.Operation
         RESULT = SetCursorFirstEntity( OldMainLOD, "Operation", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:ModifyFlag = "Y"
            ZeidonStringCopy( ModifyFlag, 1, 0, "Y", 1, 0, 2 );
            //:SET CURSOR FIRST NewMainLOD.Operation WHERE NewMainLOD.Operation.Name = OldMainLOD.Operation.Name
            GetStringFromAttribute( szTempString_7, sizeof( szTempString_7 ), OldMainLOD, "Operation", "Name" );
            RESULT = SetCursorFirstEntityByString( NewMainLOD, "Operation", "Name", szTempString_7, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:IF NewMainLOD.Operation.DoNotMergeFlag = "Y"
               if ( CompareAttributeToString( NewMainLOD, "Operation", "DoNotMergeFlag", "Y" ) == 0 )
               { 
                  //:ModifyFlag = "N"
                  ZeidonStringCopy( ModifyFlag, 1, 0, "N", 1, 0, 2 );
                  //:ELSE
               } 
               else
               { 
                  //:DELETE ENTITY NewMainLOD.Operation NONE
                  RESULT = DeleteEntity( NewMainLOD, "Operation", zREPOS_NONE );
               } 

               //:END
            } 

            //:END
            //:IF ModifyFlag = "Y"    // Either the Operation is new or it is to replace the old Operation.
            if ( ZeidonStringCompare( ModifyFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:CreateMetaEntity( vSubtask, NewMainLOD, "Operation", zPOS_AFTER )
               CreateMetaEntity( vSubtask, NewMainLOD, "Operation", zPOS_AFTER );
               //:SetMatchingAttributesByName ( NewMainLOD, "Operation", OldMainLOD, "Operation", zSET_NULL )
               SetMatchingAttributesByName( NewMainLOD, "Operation", OldMainLOD, "Operation", zSET_NULL );
               //:FOR EACH OldMainLOD.Parameter
               RESULT = SetCursorFirstEntity( OldMainLOD, "Parameter", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:CreateMetaEntity( vSubtask, NewMainLOD, "Parameter", zPOS_AFTER )
                  CreateMetaEntity( vSubtask, NewMainLOD, "Parameter", zPOS_AFTER );
                  //:SetMatchingAttributesByName ( NewMainLOD, "Parameter", OldMainLOD, "Operation", zSET_NULL )
                  SetMatchingAttributesByName( NewMainLOD, "Parameter", OldMainLOD, "Operation", zSET_NULL );
                  RESULT = SetCursorNextEntity( OldMainLOD, "Parameter", "" );
               } 

               //:END

               //:// Merge in the Operation code, unless it was copied in for a new SourceFile above.
               //:IF MergeOperationsFlag = "Y"
               if ( ZeidonStringCompare( MergeOperationsFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:CopyOperationCode( NewMainLOD, TargetFileName, SourceFileName, NewMainLOD.Operation.Name )
                  GetStringFromAttribute( szTempString_7, sizeof( szTempString_7 ), NewMainLOD, "Operation", "Name" );
                  CopyOperationCode( NewMainLOD, TargetFileName, SourceFileName, szTempString_7 );
               } 

               //:END
            } 

            RESULT = SetCursorNextEntity( OldMainLOD, "Operation", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( OldMainLOD, "SourceFile", "" );
         //:END
      } 

      //:END

      //:// If there is an ObjectConstraint file on the SourceFile, make sure it is also on the Target.
      //:IF OldMainLOD.LOD_ConstraintOper EXISTS
      lTempInteger_8 = CheckExistenceOfEntity( OldMainLOD, "LOD_ConstraintOper" );
      if ( lTempInteger_8 == 0 )
      { 
         //:IF NewMainLOD.LOD_ConstraintOper EXISTS
         lTempInteger_9 = CheckExistenceOfEntity( NewMainLOD, "LOD_ConstraintOper" );
         if ( lTempInteger_9 == 0 )
         { 
            //:EXCLUDE NewMainLOD.LOD_ConstraintOper
            RESULT = ExcludeEntity( NewMainLOD, "LOD_ConstraintOper", zREPOS_AFTER );
         } 

         //:END
         //:SET CURSOR FIRST NewMainLOD.Operation WITHIN OldMainLOD.LOD
         //:           WHERE NewMainLOD.Operation.Name = OldMainLOD.LOD_ConstraintOper.Name
         GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldMainLOD, "LOD_ConstraintOper", "Name" );
         RESULT = SetCursorFirstEntityByString( NewMainLOD, "Operation", "Name", szTempString_8, "LOD" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:INCLUDE NewMainLOD.LOD_ConstraintOper FROM NewMainLOD.Operation
            RESULT = IncludeSubobjectFromSubobject( NewMainLOD, "LOD_ConstraintOper", NewMainLOD, "Operation", zPOS_AFTER );
         } 

         //:END
      } 

      //:END
   } 


   //:END

   //:DropObjectInstance( OldMainLOD )
   DropObjectInstance( OldMainLOD );

   //:// Don't migrate any LOD without a root entity.
   //:SET CURSOR FIRST NewMainLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( NewMainLOD, "LOD_EntityParent", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "LOD (" + NewMainLOD.LOD.Name + ") is empty and will not be migrated."
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, NewMainLOD, "LOD", "Name", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "LOD (", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_8, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ") is empty and will not be migrated.", 1, 0, 201 );
      //:MessageSend( vSubtask, "", "Entity Copy",
      //:             szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vSubtask, "", "Entity Copy", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// If the ERD was modified during LOD copy, prompt the user for save of both the LOD and ERD together. If the LOD
   //:// is to be saved, the ERD must also be saved. We will not do this if component errors have occurred during the copy.
   //:IF CurrentLPLR.LPLR.wMergeComponentError = "Y"
   if ( CompareAttributeToString( CurrentLPLR, "LPLR", "wMergeComponentError", "Y" ) == 0 )
   { 
      //:MessageSend( vSubtask, "", "LOD Migrate",
      //:             "An error occurred during the copy of the LOD. Migrating the LOD will be aborted.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "LOD Migrate", "An error occurred during the copy of the LOD. Migrating the LOD will be aborted.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:CurrentLPLR.LPLR.wMergeComponentError = ""
      SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeComponentError", "" );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:IF CurrentLPLR.LPLR.wMergeER_ModifiedFlag = "Y"
   if ( CompareAttributeToString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "Y" ) == 0 )
   { 
      //:nRC = MessagePrompt( vSubtask, "", "LOD Migrate",
      //:                     "The ER was modified in the process of migrating the LOD. Do you want to save both the ER and the LOD?",
      //:                     0, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION )
      nRC = MessagePrompt( vSubtask, "", "LOD Migrate", "The ER was modified in the process of migrating the LOD. Do you want to save both the ER and the LOD?", 0, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION );
      //:IF nRC = zRESPONSE_YES
      if ( nRC == zRESPONSE_YES )
      { 
         //:// Commit the ER.
         //:// We will not use CommitMetaOI because it requires getting an updatable version of the ER, which creates
         //:// all sorts of meta problems because we already have a LOD which is referencing a "referencable" version
         //:// of the ER. We thus have made our changes to the "referencable" version (GET VIEW NewERD NAMED "TZEREMDO_REF" above)
         //:// and will commit it here.
         //:szFileName = CurrentLPLR.LPLR.MetaSrcDir + "\" +
         //:             CurrentLPLR.LPLR.Name + ".PMD"
         GetStringFromAttribute( szFileName, sizeof( szFileName ), CurrentLPLR, "LPLR", "MetaSrcDir" );
         ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 201 );
         GetVariableFromAttribute( szTempString_9, 0, 'S', 33, CurrentLPLR, "LPLR", "Name", "", 0 );
         ZeidonStringConcat( szFileName, 1, 0, szTempString_9, 1, 0, 201 );
         ZeidonStringConcat( szFileName, 1, 0, ".PMD", 1, 0, 201 );
         //:nRC = CommitOI_ToFile( NewERD, szFileName, zASCII )
         nRC = CommitOI_ToFile( NewERD, szFileName, zASCII );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MessageSend( vSubtask, "", "LOD Merge",
            //:             "An error occurred during the save of the ER. Merging the LOD will be aborted.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "", "LOD Merge", "An error occurred during the save of the ER. Merging the LOD will be aborted.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:CurrentLPLR.LPLR.wMergeComponentError  = ""
            SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeComponentError", "" );
            //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = ""
            SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "" );
            //:RETURN -1
            return( -1 );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The User response was no, so warn him that the LOD was not copied.
         //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = ""
         SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "" );
         //:MessageSend( vSubtask, "", "LOD Migrate",
         //:             "Migrating the LOD has been aborted.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( vSubtask, "", "LOD Migrate", "Migrating the LOD has been aborted.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:CurrentLPLR.LPLR.wMergeComponentError  = ""
         SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeComponentError", "" );
         //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = ""
         SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "" );
         //:RETURN -1
         return( -1 );
      } 

      //:END
      //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag = ""
      SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "" );
   } 

   //:END
   //:DropView( CurrentLPLR )
   DropView( CurrentLPLR );

   //:IF ErrorFlag = 0
   if ( ErrorFlag == 0 )
   { 
      //:CommitMetaOI( vSubtask, NewMainLOD, 7 )
      CommitMetaOI( vSubtask, NewMainLOD, 7 );
      //:ELSE
   } 
   else
   { 
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //://CommitMetaOI( vSubtask, NewMainLOD, 7 )

   //:// For every VOR for the original source LOD in the original LPLR, either update or create the
   //:// corresponding VOR in the new LPLR.
   //:GET VIEW SourceVOR_LPLR NAMED "SourceVOR_LPLR"
   RESULT = GetViewByName( &SourceVOR_LPLR, "SourceVOR_LPLR", NewMainLOD, zLEVEL_TASK );
   //:GET VIEW TargetVOR_LPLR NAMED "TargetVOR_LPLR"
   RESULT = GetViewByName( &TargetVOR_LPLR, "TargetVOR_LPLR", NewMainLOD, zLEVEL_TASK );
   //:SourceName = NewMainLOD.LOD.Name
   GetVariableFromAttribute( SourceName, 0, 'S', 41, NewMainLOD, "LOD", "Name", "", 0 );
   //:FOR EACH SourceVOR_LPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( SourceVOR_LPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF SourceVOR_LPLR.W_MetaDef.MigrateWorkName = SourceName
      if ( CompareAttributeToString( SourceVOR_LPLR, "W_MetaDef", "MigrateWorkName", SourceName ) == 0 )
      { 
         //:SET CURSOR FIRST TargetVOR_LPLR.W_MetaDef WHERE TargetVOR_LPLR.W_MetaDef.Name = SourceVOR_LPLR.W_MetaDef.Name
         GetStringFromAttribute( szTempString_10, sizeof( szTempString_10 ), SourceVOR_LPLR, "W_MetaDef", "Name" );
         RESULT = SetCursorFirstEntityByString( TargetVOR_LPLR, "W_MetaDef", "Name", szTempString_10, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:ActivateMetaOI( vSubtask, NewVOR, TargetVOR_LPLR, zSOURCE_VOR_META, zSINGLE )
            ActivateMetaOI( vSubtask, &NewVOR, TargetVOR_LPLR, zSOURCE_VOR_META, zSINGLE );
            //:NAME VIEW NewVOR "NewVOR"
            SetNameForView( NewVOR, "NewVOR", 0, zLEVEL_TASK );
            //:EXCLUDE NewVOR.LOD
            RESULT = ExcludeEntity( NewVOR, "LOD", zREPOS_AFTER );
            //:INCLUDE NewVOR.LOD FROM NewMainLOD.LOD
            RESULT = IncludeSubobjectFromSubobject( NewVOR, "LOD", NewMainLOD, "LOD", zPOS_AFTER );
            //:CommitMetaOI( vSubtask, NewVOR, 9 )
            CommitMetaOI( vSubtask, NewVOR, 9 );
            //:ELSE
         } 
         else
         { 
            //:ActivateEmptyMetaOI( vSubtask, NewVOR, zSOURCE_VOR_META, zSINGLE )
            ActivateEmptyMetaOI( vSubtask, &NewVOR, zSOURCE_VOR_META, zSINGLE );
            //:CreateMetaEntity( vSubtask, NewVOR, "ViewObjRef", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewVOR, "ViewObjRef", zPOS_AFTER );
            //:NewVOR.ViewObjRef.Name = SourceVOR_LPLR.W_MetaDef.Name
            SetAttributeFromAttribute( NewVOR, "ViewObjRef", "Name", SourceVOR_LPLR, "W_MetaDef", "Name" );
            //:INCLUDE NewVOR.LOD FROM NewMainLOD.LOD
            RESULT = IncludeSubobjectFromSubobject( NewVOR, "LOD", NewMainLOD, "LOD", zPOS_AFTER );
            //:CommitMetaOI( vSubtask, NewVOR, 9 )
            CommitMetaOI( vSubtask, NewVOR, 9 );
         } 

         //:END
         //:DropMetaOI( vSubtask, NewVOR )
         DropMetaOI( vSubtask, NewVOR );
      } 

      RESULT = SetCursorNextEntity( SourceVOR_LPLR, "W_MetaDef", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_RelinkDelete( VIEW vLOD BASED ON LOD TZZOLODO, VIEW vSubtask )

//:// The code below has some relink code commented out.  At this point I do not totally understand why.
//:// The original comment said it existed for test purposes, but this doesn't make sense to me now.
//:// We need to re-evaluate the commented code at some point.  I think it has something to do with the
//:// incomplete functionality or performance of RelinkInstanceToInstance. (DonC 8/11/95)

//:   VIEW        vERD         BASED ON LOD TZEREMDO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_RelinkDelete( zVIEW     vLOD,
                            zVIEW     vSubtask )
{
   zVIEW     vERD = 0; 
   //:VIEW        DomainLPLR   BASED ON LOD TZCMLPLO
   zVIEW     DomainLPLR = 0; 
   //:VIEW        vLPLR        BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 
   //:VIEW        vDTE         BASED ON LOD TZTENVRO
   zVIEW     vDTE = 0; 

   //:INTEGER     DeleteFlag
   zLONG     DeleteFlag = 0; 
   //:INTEGER     nERD_Flag
   zLONG     nERD_Flag = 0; 
   //:INTEGER     nDTE_Flag
   zLONG     nDTE_Flag = 0; 
   //:STRING (200) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING (32)  szSourceName
   zCHAR     szSourceName[ 33 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 


   //:DeleteFlag = 0
   DeleteFlag = 0;
   //:RetrieveViewForMetaList( vSubtask, vLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   //:NAME VIEW vLPLR "vERD_LPLR"
   SetNameForView( vLPLR, "vERD_LPLR", 0, zLEVEL_TASK );

   //:// Check if there is an ERD, because we will skip certain relinking if
   //:// there is not.
   //:IF vLPLR.W_MetaDef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vLPLR, "W_MetaDef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:nERD_Flag = ActivateMetaOI( vSubtask, vERD, vLPLR, zREFER_ERD_META, 0 )
      nERD_Flag = ActivateMetaOI( vSubtask, &vERD, vLPLR, zREFER_ERD_META, 0 );
      //:ELSE
   } 
   else
   { 
      //:nERD_Flag = -1
      nERD_Flag = -1;
   } 

   //:END

   //:DropView ( vLPLR )
   DropView( vLPLR );

   //:// Note that we are not relinking the vLOD.TE_DB_Environ entity, as this
   //:// is only necessary at XOD build, where it is already activated.

   //:// Relink subobjects against ERD.  This code should go away when a RelinkInstanceToInstance
   //:// operation is created that can reset include flags.
   //:IF nERD_Flag >= zCURSOR_SET
   if ( nERD_Flag >= zCURSOR_SET )
   { 
      //:RelinkAllSubobjectsForOI ( vLOD, "ER_RelLinkRec", vERD, "ER_RelLink_2" )
      RelinkAllSubobjectsForOI( vLOD, "ER_RelLinkRec", vERD, "ER_RelLink_2" );
      //:RelinkAllSubobjectsForOI ( vLOD, "ER_AttributeRec", vERD, "ER_Attribute" )
      RelinkAllSubobjectsForOI( vLOD, "ER_AttributeRec", vERD, "ER_Attribute" );
   } 

   //:END

   //:RetrieveViewForMetaList( vSubtask, DomainLPLR, zREFER_DOMAIN_META )
   RetrieveViewForMetaList( vSubtask, &DomainLPLR, zREFER_DOMAIN_META );
   //:NAME VIEW DomainLPLR "DomainLPLR"
   SetNameForView( DomainLPLR, "DomainLPLR", 0, zLEVEL_TASK );

   //:// Relink Each LOD Entity and subordinate entities
   //:FOR EACH vLOD.LOD_Entity
   RESULT = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vLOD.ER_Entity EXISTS AND nERD_Flag >= zCURSOR_SET
      lTempInteger_1 = CheckExistenceOfEntity( vLOD, "ER_Entity" );
      if ( lTempInteger_1 == 0 && nERD_Flag >= zCURSOR_SET )
      { 
         //:// The following relink is only done if there is an ERD
         //:SET CURSOR FIRST vERD.ER_Entity WHERE
         //:    vERD.ER_Entity.ZKey = vLOD.ER_Entity.ZKey
         GetIntegerFromAttribute( &lTempInteger_2, vLOD, "ER_Entity", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Entity", "ZKey", lTempInteger_2, "" );

         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:RelinkInstanceToInstance( vLOD, "ER_Entity", vERD, "ER_Entity" )
            RelinkInstanceToInstance( vLOD, "ER_Entity", vERD, "ER_Entity" );

            //:// Attributes
            //:fnRelinkAttributes( vSubtask, vLOD, vERD, DomainLPLR )
            oTZZOLODO_fnRelinkAttributes( vSubtask, vLOD, vERD, DomainLPLR );

            //:// Relationship
            //:fnRelinkRelationships( vSubtask, vLOD, vERD )
            oTZZOLODO_fnRelinkRelationships( vSubtask, vLOD, vERD );

            //:ELSE
         } 
         else
         { 
            //:// No match on ER_Entity ZKey
            //:IF vLOD.LOD_Entity.Work = "Y"
            if ( CompareAttributeToString( vLOD, "LOD_Entity", "Work", "Y" ) == 0 )
            { 
               //:FOR EACH vLOD.LOD_Attribute
               RESULT = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:// Relink Domains for attributes under a work entity.
                  //:fnRelinkWorkAttrib( vLOD, DomainLPLR, vSubtask )
                  oTZZOLODO_fnRelinkWorkAttrib( vLOD, DomainLPLR, vSubtask );
                  RESULT = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:// If we are here, this must be an ER type LOD_Attribute with
               //:// no match on ER_Attribute ZKey.
               //:// Since there is no match on ZKey, try to find a match on
               //:// ER_Attribute name, in case the Attribute had been deleted
               //:// and recreated.
               //:SET CURSOR FIRST vERD.ER_Entity WHERE
               //:                 vERD.ER_Entity.Name = vLOD.ER_Entity.Name
               GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vLOD, "ER_Entity", "Name" );
               RESULT = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szTempString_0, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 

                  //:// First reinclude the ER_Entity
                  //:EXCLUDE vLOD.ER_Entity
                  RESULT = ExcludeEntity( vLOD, "ER_Entity", zREPOS_AFTER );
                  //:INCLUDE vLOD.ER_Entity FROM vERD.ER_Entity
                  RESULT = IncludeSubobjectFromSubobject( vLOD, "ER_Entity", vERD, "ER_Entity", zPOS_AFTER );

                  //:// Attributes.
                  //:fnRelinkAttributes( vSubtask, vLOD, vERD, DomainLPLR )
                  oTZZOLODO_fnRelinkAttributes( vSubtask, vLOD, vERD, DomainLPLR );

                  //:// Relationships
                  //:fnRelinkRelationships( vSubtask, vLOD, vERD )
                  oTZZOLODO_fnRelinkRelationships( vSubtask, vLOD, vERD );

                  //:ELSE
               } 
               else
               { 
                  //:szMsg = "Deleting LOD Entity: " + vLOD.LOD_Entity.Name
                  GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLOD, "LOD_Entity", "Name", "", 0 );
                  ZeidonStringCopy( szMsg, 1, 0, "Deleting LOD Entity: ", 1, 0, 201 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 201 );
                  //:MessageSend( vSubtask, "ZO00413", "LOD Relink",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "ZO00413", "LOD Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:DELETE ENTITY vLOD.LOD_Entity NONE
                  RESULT = DeleteEntity( vLOD, "LOD_Entity", zREPOS_NONE );
                  //:DeleteFlag = 1
                  DeleteFlag = 1;
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF vLOD.LOD_Entity.Work = "Y"
         if ( CompareAttributeToString( vLOD, "LOD_Entity", "Work", "Y" ) == 0 )
         { 
            //:FOR EACH vLOD.LOD_Attribute
            RESULT = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:// Relink Domains for attributes under a work entity.
               //:fnRelinkWorkAttrib( vLOD, DomainLPLR, vSubtask )
               oTZZOLODO_fnRelinkWorkAttrib( vLOD, DomainLPLR, vSubtask );
               RESULT = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
            } 

            //:END

            //:ELSE
         } 
         else
         { 
            //:// Since this is not a work entity, it is an error to have a
            //:// LOD_Entity without a corresponding ER_Entity.
            //:// If this happens, there is some kind of Zeidon error.
            //:szMsg = "Deleting LOD Entity for logic error.| Entity: " +
            //:        vLOD.LOD_Entity.Name
            GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vLOD, "LOD_Entity", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Deleting LOD Entity for logic error.\nEntity: ", 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 201 );
            //:MessageSend( vSubtask, "ZO00414", "LOD Relink",
            //:             szMsg,
            //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "ZO00414", "LOD Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:DELETE ENTITY vLOD.LOD_Entity NONE
            RESULT = DeleteEntity( vLOD, "LOD_Entity", zREPOS_NONE );
            //:DeleteFlag = 1
            DeleteFlag = 1;
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vLOD, "LOD_Entity", "" );
      //:END
   } 

   //:   // IF vLOD.ER_RelLink EXISTS
   //:   //    relink ER_RelLink & ER_RelType
   //:   // END
   //:END
   //:DropView ( DomainLPLR )
   DropView( DomainLPLR );
   //:IF nERD_Flag >= zCURSOR_SET
   if ( nERD_Flag >= zCURSOR_SET )
   { 
      //:DropMetaOI( vSubtask, vERD )
      DropMetaOI( vSubtask, vERD );
   } 

   //:END


   //://BL, 2000.01.03 Relink TE
   //:RetrieveViewForMetaList( vSubtask, vLPLR, zREFER_DTE_META )
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_DTE_META );
   //:NAME VIEW vLPLR "vDTE_LPLR"
   SetNameForView( vLPLR, "vDTE_LPLR", 0, zLEVEL_TASK );

   //:// Check if there is an DTE, because we will skip certain relinking if
   //:// there is not.
   //:IF vLPLR.W_MetaDef EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( vLPLR, "W_MetaDef" );
   if ( lTempInteger_3 == 0 )
   { 
      //:nDTE_Flag = ActivateMetaOI( vSubtask, vDTE, vLPLR, zREFER_DTE_META, 0 )
      nDTE_Flag = ActivateMetaOI( vSubtask, &vDTE, vLPLR, zREFER_DTE_META, 0 );
      //:ELSE
   } 
   else
   { 
      //:nDTE_Flag = -1
      nDTE_Flag = -1;
   } 

   //:END

   //:DropView ( vLPLR )
   DropView( vLPLR );

   //:// Relink subobjects against DTE.  This code should go away when a RelinkInstanceToInstance
   //:// operation is created that can reset include flags.
   //:IF nDTE_Flag >= zCURSOR_SET
   if ( nDTE_Flag >= zCURSOR_SET )
   { 
      //:// The relink below may cause the vLOD.POD.TE_SourceZKey value to become invalid.
      //:// This could occur because a DBMS_Source entry was deleted and recreated. We will
      //:// thus save the DBMS_Source name for the vLOD.POD.TE_SourceZKey value to alter the
      //:// value as necessary.
      //:IF vLOD.POD EXISTS
      lTempInteger_4 = CheckExistenceOfEntity( vLOD, "POD" );
      if ( lTempInteger_4 == 0 )
      { 
         //:SET CURSOR FIRST vLOD.TE_DBMS_Source
         //:    WHERE vLOD.TE_DBMS_Source.ZKey = vLOD.POD.TE_SourceZKey
         GetIntegerFromAttribute( &lTempInteger_5, vLOD, "POD", "TE_SourceZKey" );
         RESULT = SetCursorFirstEntityByInteger( vLOD, "TE_DBMS_Source", "ZKey", lTempInteger_5, "" );
         //:szSourceName = vLOD.TE_DBMS_Source.Name
         GetVariableFromAttribute( szSourceName, 0, 'S', 33, vLOD, "TE_DBMS_Source", "Name", "", 0 );
         //:RelinkAllSubobjectsForOI ( vLOD, "TE_DB_Environ", vDTE, "TE_DB_Environ" )
         RelinkAllSubobjectsForOI( vLOD, "TE_DB_Environ", vDTE, "TE_DB_Environ" );
         //:SET CURSOR FIRST vLOD.TE_DBMS_Source
         //:    WHERE vLOD.TE_DBMS_Source.ZKey = vLOD.POD.TE_SourceZKey
         GetIntegerFromAttribute( &lTempInteger_6, vLOD, "POD", "TE_SourceZKey" );
         RESULT = SetCursorFirstEntityByInteger( vLOD, "TE_DBMS_Source", "ZKey", lTempInteger_6, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR FIRST vLOD.TE_DBMS_Source
            //:    WHERE vLOD.TE_DBMS_Source.Name = szSourceName
            RESULT = SetCursorFirstEntityByString( vLOD, "TE_DBMS_Source", "Name", szSourceName, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:vLOD.POD.TE_SourceZKey = vLOD.TE_DBMS_Source.ZKey
               SetAttributeFromAttribute( vLOD, "POD", "TE_SourceZKey", vLOD, "TE_DBMS_Source", "ZKey" );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:IF nDTE_Flag >= zCURSOR_SET
   if ( nDTE_Flag >= zCURSOR_SET )
   { 
      //:DropMetaOI( vSubtask, vDTE )
      DropMetaOI( vSubtask, vDTE );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:fnLocateParentName( VIEW vRecursLOD BASED ON LOD TZZOLODO,
//:                    INTEGER RelationshipZKey,
//:                    INTEGER ChildEntityZKey,
//:                    STRING ( 32 ) ReturnedParentName )

//:   VIEW vParentLOD BASED ON LOD TZZOLODO
static zSHORT
oTZZOLODO_fnLocateParentName( zVIEW     vRecursLOD,
                              zLONG     RelationshipZKey,
                              zLONG     ChildEntityZKey,
                              zPCHAR    ReturnedParentName )
{
   zVIEW     vParentLOD = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //:// Recursive routine to locate the parent entity name for a particular RelationshipName and
   //:// ChildEntityName.
   //:// Keep searching recursively until there is a match on entity name and relationship name.
   //:// When a match is found, get the entity name of the parent and return.
   //:FOR EACH vRecursLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( vRecursLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Evaluate each LOD_EntityParent
      //:IF vRecursLOD.ER_RelLinkRec EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vRecursLOD, "ER_RelLinkRec" );
      if ( lTempInteger_0 == 0 )
      { 
         //:IF vRecursLOD.LOD_EntityParent.ZKey = ChildEntityZKey AND
         //:   vRecursLOD.ER_RelLinkRec.ZKey    = RelationshipZKey
         if ( CompareAttributeToInteger( vRecursLOD, "LOD_EntityParent", "ZKey", ChildEntityZKey ) == 0 && CompareAttributeToInteger( vRecursLOD, "ER_RelLinkRec", "ZKey", RelationshipZKey ) == 0 )
         { 

            //:CreateViewFromViewForTask( vParentLOD, vRecursLOD, 0 )
            CreateViewFromViewForTask( &vParentLOD, vRecursLOD, 0 );
            //:ResetViewFromSubobject( vParentLOD )
            ResetViewFromSubobject( vParentLOD );
            //:ReturnedParentName = vParentLOD.ER_EntityRec.Name
            GetVariableFromAttribute( ReturnedParentName, 0, 'S', 33, vParentLOD, "ER_EntityRec", "Name", "", 0 );
            //:DropView( vParentLOD )
            DropView( vParentLOD );
            //:RETURN 0
            return( 0 );
         } 

         //:END
      } 

      //:END

      //:// Process LOD_EntityChild subobjects
      //:IF vRecursLOD.LOD_EntityChild EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vRecursLOD, "LOD_EntityChild" );
      if ( lTempInteger_1 == 0 )
      { 
         //:SetViewToSubobject( vRecursLOD, "LOD_EntityChild" )
         SetViewToSubobject( vRecursLOD, "LOD_EntityChild" );
         //:fnLocateParentName( vRecursLOD,
         //:                    RelationshipZKey,
         //:                    ChildEntityZKey,
         //:                    ReturnedParentName )
         oTZZOLODO_fnLocateParentName( vRecursLOD, RelationshipZKey, ChildEntityZKey, ReturnedParentName );
         //:ResetViewFromSubobject( vRecursLOD )
         ResetViewFromSubobject( vRecursLOD );
         //:IF ReturnedParentName != ""
         if ( ZeidonStringCompare( ReturnedParentName, 1, 0, "", 1, 0, 33 ) != 0 )
         { 
            //:RETURN 0
            return( 0 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vRecursLOD, "LOD_EntityParent", "" );
      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:fnRelinkAttributes( VIEW vSubtask,
//:                    VIEW vLOD       BASED ON LOD TZZOLODO,
//:                    VIEW vERD       BASED ON LOD TZEREMDO,
//:                    VIEW DomainLPLR BASED ON LOD TZCMLPLO )

//:   INTEGER      DeleteFlag
static zSHORT
oTZZOLODO_fnRelinkAttributes( zVIEW     vSubtask,
                              zVIEW     vLOD,
                              zVIEW     vERD,
                              zVIEW     DomainLPLR )
{
   zLONG     DeleteFlag = 0; 
   //:INTEGER      nRemapFlag
   zLONG     nRemapFlag = 0; 
   //:STRING (200) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:FOR EACH vLOD.LOD_Attribute
   RESULT = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vLOD.ER_Attribute EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vLOD, "ER_Attribute" );
      if ( lTempInteger_0 == 0 )
      { 
         //:SET CURSOR FIRST vERD.ER_Attribute WHERE
         //:        vERD.ER_Attribute.ZKey = vLOD.ER_Attribute.ZKey
         GetIntegerFromAttribute( &lTempInteger_1, vLOD, "ER_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vERD, "ER_Attribute", "ZKey", lTempInteger_1, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //://RelinkInstanceToInstance( vLOD, "ER_Attribute", vERD, "ER_Attribute" )
            //://RelinkInstanceToInstance( vLOD, "Domain", vERD, "Domain" )
            //:ELSE
         } 
         else
         { 
            //:IF vLOD.LOD_Attribute.Work = "Y"
            if ( CompareAttributeToString( vLOD, "LOD_Attribute", "Work", "Y" ) == 0 )
            { 
               //:// Relink Domains for work attributes.
               //:fnRelinkWorkAttrib( vLOD, DomainLPLR, vSubtask )
               oTZZOLODO_fnRelinkWorkAttrib( vLOD, DomainLPLR, vSubtask );

               //:ELSE
            } 
            else
            { 
               //:// Since there was no match on ZKey, try to find a match on ER Entity
               //:// name and ER Attribute name, in case the Attribute had been deleted
               //:// and recreated.  In this case, re-include the Attribute.
               //:SET CURSOR FIRST vERD.ER_Entity WHERE
               //:                 vERD.ER_Entity.Name = vLOD.ER_Entity.Name
               GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vLOD, "ER_Entity", "Name" );
               RESULT = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szTempString_0, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:SET CURSOR FIRST vERD.ER_Attribute WHERE
                  //:                 vERD.ER_Attribute.Name = vLOD.ER_Attribute.Name
                  GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vLOD, "ER_Attribute", "Name" );
                  RESULT = SetCursorFirstEntityByString( vERD, "ER_Attribute", "Name", szTempString_0, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:nRemapFlag = 0
                     nRemapFlag = 0;
                     //:EXCLUDE vLOD.ER_Attribute
                     RESULT = ExcludeEntity( vLOD, "ER_Attribute", zREPOS_AFTER );
                     //:INCLUDE vLOD.ER_Attribute FROM vERD.ER_Attribute
                     RESULT = IncludeSubobjectFromSubobject( vLOD, "ER_Attribute", vERD, "ER_Attribute", zPOS_AFTER );
                     //:ELSE
                  } 
                  else
                  { 
                     //:nRemapFlag = -1
                     nRemapFlag = -1;
                  } 

                  //:END
                  //:ELSE
               } 
               else
               { 
                  //:nRemapFlag = -1
                  nRemapFlag = -1;
               } 

               //:END
               //:IF nRemapFlag = -1
               if ( nRemapFlag == -1 )
               { 
                  //:szMsg = "Deleting LOD Entity/Attribute: " +
                  //:        vLOD.LOD_Entity.Name + "/" + vLOD.ER_Attribute.Name
                  GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLOD, "LOD_Entity", "Name", "", 0 );
                  ZeidonStringCopy( szMsg, 1, 0, "Deleting LOD Entity/Attribute: ", 1, 0, 201 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 201 );
                  ZeidonStringConcat( szMsg, 1, 0, "/", 1, 0, 201 );
                  GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vLOD, "ER_Attribute", "Name", "", 0 );
                  ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 201 );
                  //:MessageSend( vSubtask, "ZO00410", "LOD Relink",
                  //:             szMsg,
                  //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "ZO00410", "LOD Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:DELETE ENTITY vLOD.LOD_Attribute NONE
                  RESULT = DeleteEntity( vLOD, "LOD_Attribute", zREPOS_NONE );
                  //:DeleteFlag = 1
                  DeleteFlag = 1;
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:fnRelinkRelationships( VIEW vSubtask,
//:                       VIEW vLOD BASED ON LOD TZZOLODO,
//:                       VIEW vERD BASED ON LOD TZEREMDO )

//:   STRING (32) ReturnedParentName
static zSHORT
oTZZOLODO_fnRelinkRelationships( zVIEW     vSubtask,
                                 zVIEW     vLOD,
                                 zVIEW     vERD )
{
   zCHAR     ReturnedParentName[ 33 ] = { 0 }; 
   //:INTEGER     RelationshipZKey
   zLONG     RelationshipZKey = 0; 
   //:INTEGER     ChildEntityZKey
   zLONG     ChildEntityZKey = 0; 
   //:STRING (200) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 


   //:FOR EACH vLOD.ER_RelLink
   RESULT = SetCursorFirstEntity( vLOD, "ER_RelLink", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vERD.ER_RelLink_Other WITHIN vERD.ER_Entity WHERE
      //:    vERD.ER_RelLink_Other.ZKey = vLOD.ER_RelLink.ZKey
      GetIntegerFromAttribute( &lTempInteger_0, vLOD, "ER_RelLink", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelLink_Other", "ZKey", lTempInteger_0, "ER_Entity" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// There is no match on ZKey, so try to match on Relationship name.
         //:// If a match, re-include. If not, give error message.
         //:// This is going to require locating the parent entity name by looking
         //:// through the recursive subobject structure.
         //:RelationshipZKey = vLOD.ER_RelLink.ZKey
         GetIntegerFromAttribute( &RelationshipZKey, vLOD, "ER_RelLink", "ZKey" );
         //:ChildEntityZKey  = vLOD.LOD_Entity.ZKey
         GetIntegerFromAttribute( &ChildEntityZKey, vLOD, "LOD_Entity", "ZKey" );
         //:SetViewToSubobject( vLOD, "LOD_EntityChild" )
         SetViewToSubobject( vLOD, "LOD_EntityChild" );
         //:ReturnedParentName = ""
         ZeidonStringCopy( ReturnedParentName, 1, 0, "", 1, 0, 33 );
         //:fnLocateParentName( vLOD,
         //:                    RelationshipZKey,
         //:                    ChildEntityZKey,
         //:                    ReturnedParentName )
         oTZZOLODO_fnLocateParentName( vLOD, RelationshipZKey, ChildEntityZKey, ReturnedParentName );
         //:ResetViewFromSubobject( vLOD )
         ResetViewFromSubobject( vLOD );
         //:SET CURSOR FIRST vERD.ER_RelLink_Other WITHIN vERD.ER_Entity WHERE
         //:    vERD.ER_RelLink_Other.Name = vLOD.ER_RelLink.Name AND
         //:    vERD.ER_Entity_Other.Name  = ReturnedParentName
         RESULT = SetCursorFirstEntity( vERD, "ER_RelLink_Other", "ER_Entity" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( vERD, "ER_RelLink_Other", "Name", vLOD, "ER_RelLink", "Name" ) != 0 || CompareAttributeToString( vERD, "ER_Entity_Other", "Name", ReturnedParentName ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( vERD, "ER_RelLink_Other", "ER_Entity" );
            } 

         } 

         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:SET CURSOR FIRST vERD.ER_RelLink_2 WITHIN vERD.EntpER_Model
            //:     WHERE vERD.ER_RelLink_2.ZKey = vERD.ER_RelLink_Other.ZKey
            GetIntegerFromAttribute( &lTempInteger_1, vERD, "ER_RelLink_Other", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vERD, "ER_RelLink_2", "ZKey", lTempInteger_1, "EntpER_Model" );
            //:EXCLUDE vLOD.ER_RelLink
            RESULT = ExcludeEntity( vLOD, "ER_RelLink", zREPOS_AFTER );
            //:INCLUDE vLOD.ER_RelLink FROM vERD.ER_RelLink_2
            RESULT = IncludeSubobjectFromSubobject( vLOD, "ER_RelLink", vERD, "ER_RelLink_2", zPOS_AFTER );
            //:ELSE
         } 
         else
         { 
            //:szMsg = "There is no entity/relationship name match in the ERD for LOD " +
            //:        vLOD.LOD.Name + ", with relationship: " + NEW_LINE +
            //:        ReturnedParentName + "  " +
            //:        vLOD.ER_RelLink.Name + "  " +
            //:        vLOD.ER_Entity.Name + NEW_LINE +
            //:        "The LOD Entity will be deleted."
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "There is no entity/relationship name match in the ERD for LOD ", 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, ", with relationship: ", 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, ReturnedParentName, 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, "  ", 1, 0, 201 );
            GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vLOD, "ER_RelLink", "Name", "", 0 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, "  ", 1, 0, 201 );
            GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vLOD, "ER_Entity", "Name", "", 0 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 201 );
            ZeidonStringConcat( szMsg, 1, 0, "The LOD Entity will be deleted.", 1, 0, 201 );
            //:MessageSend( vSubtask, "ZO00410", "LOD Relink",
            //:             szMsg,
            //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "ZO00410", "LOD Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:DELETE ENTITY vLOD.LOD_Entity
            RESULT = DeleteEntity( vLOD, "LOD_Entity", zPOS_NEXT );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vLOD, "ER_RelLink", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SetAttributeSequence( VIEW vTZZOLODO BASED ON LOD TZZOLODO )

//:  VIEW vTZZOLODO_Copy BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SetAttributeSequence( zVIEW     vTZZOLODO )
{
   zVIEW     vTZZOLODO_Copy = 0; 
   //:INTEGER lSequence
   zLONG     lSequence = 0; 
   zSHORT    RESULT; 


   //:CreateViewFromViewForTask( vTZZOLODO_Copy, vTZZOLODO, 0 )
   CreateViewFromViewForTask( &vTZZOLODO_Copy, vTZZOLODO, 0 );

   //:FOR EACH vTZZOLODO_Copy.LOD_Entity
   RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:lSequence = 1
      lSequence = 1;
      //:FOR EACH vTZZOLODO_Copy.LOD_Attribute
      RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: FOR EACH vTZZOLODO_Copy.ER_Attribute
         RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "ER_Attribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:  vTZZOLODO_Copy.LOD_Attribute.wkSequence = lSequence
            SetAttributeFromInteger( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence", lSequence );
            //:  lSequence = lSequence + 1
            lSequence = lSequence + 1;
            RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "ER_Attribute", "" );
         } 

         RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
         //: END
      } 

      RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Entity", "" );
      //:END
   } 

   //:END

   //:DropView( vTZZOLODO_Copy )
   DropView( vTZZOLODO_Copy );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:NewAttributeSequence( VIEW vTZZOLODO BASED ON LOD TZZOLODO )
//:   VIEW    vTZZOLODO_Copy BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_NewAttributeSequence( zVIEW     vTZZOLODO )
{
   zVIEW     vTZZOLODO_Copy = 0; 
   //:INTEGER lSequenceNo
   zLONG     lSequenceNo = 0; 
   //:INTEGER lNewSequenceNo
   zLONG     lNewSequenceNo = 0; 
   zSHORT    RESULT; 


   //:IF vTZZOLODO.LOD_Attribute.wkSequence = ""
   if ( CompareAttributeToString( vTZZOLODO, "LOD_Attribute", "wkSequence", "" ) == 0 )
   { 

      //:CreateViewFromViewForTask( vTZZOLODO_Copy, vTZZOLODO, 0 )
      CreateViewFromViewForTask( &vTZZOLODO_Copy, vTZZOLODO, 0 );

      //:// set cursor on previous attribute and get the sequence number
      //:SET CURSOR PREVIOUS vTZZOLODO_Copy.LOD_Attribute
      RESULT = SetCursorPrevEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
      //:IF RESULT = zCURSOR_SET
      if ( RESULT == zCURSOR_SET )
      { 
         //:lSequenceNo = vTZZOLODO_Copy.LOD_Attribute.wkSequence
         GetIntegerFromAttribute( &lSequenceNo, vTZZOLODO_Copy, "LOD_Attribute", "wkSequence" );
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

      //:// set new sequence number for the attribute with greater number
      //:FOR EACH vTZZOLODO_Copy.LOD_Attribute
      RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: IF vTZZOLODO_Copy.LOD_Attribute.wkSequence >= lSequenceNo
         if ( CompareAttributeToInteger( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence", lSequenceNo ) >= 0 )
         { 
            //: lNewSequenceNo = vTZZOLODO_Copy.LOD_Attribute.wkSequence
            GetIntegerFromAttribute( &lNewSequenceNo, vTZZOLODO_Copy, "LOD_Attribute", "wkSequence" );
            //: lNewSequenceNo = lNewSequenceNo + 1
            lNewSequenceNo = lNewSequenceNo + 1;
            //: vTZZOLODO_Copy.LOD_Attribute.wkSequence = lNewSequenceNo
            SetAttributeFromInteger( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence", lNewSequenceNo );
         } 

         RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
         //:END
      } 

      //:END

      //:// set sequence number for new attribute
      //:vTZZOLODO.LOD_Attribute.wkSequence = lSequenceNo
      SetAttributeFromInteger( vTZZOLODO, "LOD_Attribute", "wkSequence", lSequenceNo );

      //:DropView( vTZZOLODO_Copy )
      DropView( vTZZOLODO_Copy );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DelAttribFromSequence( VIEW vTZZOLODO BASED ON LOD TZZOLODO )
//:   VIEW    vTZZOLODO_Copy BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DelAttribFromSequence( zVIEW     vTZZOLODO )
{
   zVIEW     vTZZOLODO_Copy = 0; 
   //:INTEGER lSequenceNo
   zLONG     lSequenceNo = 0; 
   //:INTEGER lNewSequenceNo
   zLONG     lNewSequenceNo = 0; 
   zSHORT    RESULT; 


   //:IF vTZZOLODO.LOD_Attribute.wkSequence > 0
   if ( CompareAttributeToInteger( vTZZOLODO, "LOD_Attribute", "wkSequence", 0 ) > 0 )
   { 

      //:CreateViewFromViewForTask( vTZZOLODO_Copy, vTZZOLODO, 0 )
      CreateViewFromViewForTask( &vTZZOLODO_Copy, vTZZOLODO, 0 );

      //:lSequenceNo = vTZZOLODO.LOD_Attribute.wkSequence
      GetIntegerFromAttribute( &lSequenceNo, vTZZOLODO, "LOD_Attribute", "wkSequence" );

      //:// set new sequence number for the attribute with greater number
      //:FOR EACH vTZZOLODO_Copy.LOD_Attribute
      RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: IF vTZZOLODO_Copy.LOD_Attribute.wkSequence > lSequenceNo
         if ( CompareAttributeToInteger( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence", lSequenceNo ) > 0 )
         { 
            //: lNewSequenceNo = vTZZOLODO_Copy.LOD_Attribute.wkSequence
            GetIntegerFromAttribute( &lNewSequenceNo, vTZZOLODO_Copy, "LOD_Attribute", "wkSequence" );
            //: lNewSequenceNo = lNewSequenceNo - 1
            lNewSequenceNo = lNewSequenceNo - 1;
            //: vTZZOLODO_Copy.LOD_Attribute.wkSequence = lNewSequenceNo
            SetAttributeFromInteger( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence", lNewSequenceNo );
         } 

         RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
         //:END
      } 

      //:END

      //:DropView( vTZZOLODO_Copy )
      DropView( vTZZOLODO_Copy );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:SortAttributes( VIEW vTZZOLODO BASED ON LOD TZZOLODO )

//:   VIEW vTZZOLODO_Copy BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SortAttributes( zVIEW     vTZZOLODO )
{
   zVIEW     vTZZOLODO_Copy = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:CreateViewFromViewForTask( vTZZOLODO_Copy, vTZZOLODO, 0 )
   CreateViewFromViewForTask( &vTZZOLODO_Copy, vTZZOLODO, 0 );
   //:FOR EACH vTZZOLODO_Copy.LOD_Entity
   RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: IF vTZZOLODO.LOD_Attribute EXISTS AND vTZZOLODO.LOD_Attribute.wkSequence > 0
      lTempInteger_0 = CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" );
      if ( lTempInteger_0 == 0 && CompareAttributeToInteger( vTZZOLODO, "LOD_Attribute", "wkSequence", 0 ) > 0 )
      { 
         //: OrderEntityForView( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence A" )
         OrderEntityForView( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence A" );
      } 

      RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Entity", "" );
      //: END
   } 

   //:END
   //:DropView( vTZZOLODO_Copy )
   DropView( vTZZOLODO_Copy );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:MoveAttribInSequence( VIEW vTZZOLODO BASED ON LOD TZZOLODO )
//:   VIEW    vTZZOLODO_Copy BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_MoveAttribInSequence( zVIEW     vTZZOLODO )
{
   zVIEW     vTZZOLODO_Copy = 0; 
   //:INTEGER lNewSequenceNo
   zLONG     lNewSequenceNo = 0; 
   zSHORT    RESULT; 


   //:CreateViewFromViewForTask( vTZZOLODO_Copy, vTZZOLODO, 0 )
   CreateViewFromViewForTask( &vTZZOLODO_Copy, vTZZOLODO, 0 );
   //:// tmv
   //:lNewSequenceNo = 1
   lNewSequenceNo = 1;
   //:// set new sequence number for the attribute with greater number
   //:FOR EACH vTZZOLODO_Copy.LOD_Attribute
   RESULT = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: vTZZOLODO_Copy.LOD_Attribute.wkSequence = lNewSequenceNo
      SetAttributeFromInteger( vTZZOLODO_Copy, "LOD_Attribute", "wkSequence", lNewSequenceNo );
      //: lNewSequenceNo = lNewSequenceNo + 1
      lNewSequenceNo = lNewSequenceNo + 1;
      RESULT = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
   } 

   //:END

   //:DropView( vTZZOLODO_Copy )
   DropView( vTZZOLODO_Copy );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:GetRelationshipName( VIEW SourceLPLR BASED ON LOD TZCMLPLO,
//:                     VIEW OldLOD BASED ON LOD TZZOLODO,
//:                     STRING ( 32 ) RelationshipName,
//:                     STRING( 32) ParentName )

//:   VIEW vOldER      BASED ON LOD TZEREMDO
static zSHORT
oTZZOLODO_GetRelationshipName( zVIEW     SourceLPLR,
                               zVIEW     OldLOD,
                               zPCHAR    RelationshipName,
                               zPCHAR    ParentName )
{
   zVIEW     vOldER = 0; 
   //:VIEW SourceLPLR2 BASED ON LOD TZCMLPLO
   zVIEW     SourceLPLR2 = 0; 
   //:STRING ( 513 ) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, 0 )
   CreateViewFromViewForTask( &SourceLPLR2, SourceLPLR, 0 );
   //:SET CURSOR FIRST SourceLPLR2.W_MetaType
   //:  WHERE SourceLPLR2.W_MetaType.Type = 2004 // zREFER_ERD_META
   RESULT = SetCursorFirstEntityByInteger( SourceLPLR2, "W_MetaType", "Type", 2004, "" );

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:DropView( SourceLPLR2 )
      DropView( SourceLPLR2 );
      //:RETURN 1
      return( 1 );
   } 

   //:END

   //:SourceFileName = SourceLPLR2.LPLR.MetaSrcDir + "\" + SourceLPLR2.W_MetaDef.Name + ".PMD"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR2, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, SourceLPLR2, "W_MetaDef", "Name", "", 0 );
   ZeidonStringConcat( SourceFileName, 1, 0, szTempString_0, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PMD", 1, 0, 514 );
   //:ActivateOI_FromFile ( vOldER, "TZEREMDO", SourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &vOldER, "TZEREMDO", SourceLPLR, SourceFileName, 8192 );
   //:DropView( SourceLPLR2 )
   DropView( SourceLPLR2 );

   //:SET CURSOR FIRST vOldER.ER_Entity_2 WITHIN vOldER.EntpER_Model WHERE
   //:        vOldER.ER_Entity_2.Name = ParentName AND
   //:        vOldER.ER_RelLink_2.ZKey = OldLOD.ER_RelLinkRec.ZKey
   RESULT = SetCursorFirstEntity( vOldER, "ER_Entity_2", "EntpER_Model" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( vOldER, "ER_Entity_2", "Name", ParentName ) != 0 || CompareAttributeToAttribute( vOldER, "ER_RelLink_2", "ZKey", OldLOD, "ER_RelLinkRec", "ZKey" ) != 0 ) )
      { 
         RESULT = SetCursorNextEntity( vOldER, "ER_Entity_2", "EntpER_Model" );
      } 

   } 


   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:DropObjectInstance( vOldER )
      DropObjectInstance( vOldER );
      //:RETURN 2
      return( 2 );
   } 

   //:END

   //:RelationshipName = vOldER.ER_RelLink_2.Name
   GetVariableFromAttribute( RelationshipName, 0, 'S', 33, vOldER, "ER_RelLink_2", "Name", "", 0 );
   //:DropObjectInstance( vOldER )
   DropObjectInstance( vOldER );
   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:GetERAttributeName( VIEW SourceLPLR BASED ON LOD TZCMLPLO,
//:                    VIEW OldLOD BASED ON LOD TZZOLODO,
//:                    STRING ( 32 ) ERAttributeName )

//:   VIEW vOldER      BASED ON LOD TZEREMDO
static zSHORT
oTZZOLODO_GetERAttributeName( zVIEW     SourceLPLR,
                              zVIEW     OldLOD,
                              zPCHAR    ERAttributeName )
{
   zVIEW     vOldER = 0; 
   //:VIEW SourceLPLR2 BASED ON LOD TZCMLPLO
   zVIEW     SourceLPLR2 = 0; 
   //:STRING ( 513 ) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_0; 


   //:CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, 0 )
   CreateViewFromViewForTask( &SourceLPLR2, SourceLPLR, 0 );
   //:SET CURSOR FIRST SourceLPLR2.W_MetaType
   //:  WHERE SourceLPLR2.W_MetaType.Type = 2004 // zREFER_ERD_META
   RESULT = SetCursorFirstEntityByInteger( SourceLPLR2, "W_MetaType", "Type", 2004, "" );

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:DropView( SourceLPLR2 )
      DropView( SourceLPLR2 );
      //:RETURN 1
      return( 1 );
   } 

   //:END

   //:SourceFileName = SourceLPLR2.LPLR.MetaSrcDir + "\" + SourceLPLR2.W_MetaDef.Name + ".PMD"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR2, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, SourceLPLR2, "W_MetaDef", "Name", "", 0 );
   ZeidonStringConcat( SourceFileName, 1, 0, szTempString_0, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PMD", 1, 0, 514 );
   //:ActivateOI_FromFile ( vOldER, "TZEREMDO", SourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &vOldER, "TZEREMDO", SourceLPLR, SourceFileName, 8192 );
   //:DropView( SourceLPLR2 )
   DropView( SourceLPLR2 );

   //:SET CURSOR FIRST vOldER.ER_Attribute WITHIN vOldER.EntpER_Model WHERE
   //:        vOldER.ER_Attribute.ZKey = OldLOD.ER_AttributeRec.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, OldLOD, "ER_AttributeRec", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vOldER, "ER_Attribute", "ZKey", lTempInteger_0, "EntpER_Model" );

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:DropObjectInstance( vOldER )
      DropObjectInstance( vOldER );
      //:RETURN 2
      return( 2 );
   } 

   //:END

   //:ERAttributeName = vOldER.ER_Attribute.Name
   GetVariableFromAttribute( ERAttributeName, 0, 'S', 33, vOldER, "ER_Attribute", "Name", "", 0 );
   //:DropObjectInstance( vOldER )
   DropObjectInstance( vOldER );
   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:fnCopyPOD_Entity( VIEW vSubtask,
//:                  VIEW NewLOD  BASED ON LOD TZZOLODO,
//:                  VIEW OldLOD  BASED ON LOD TZZOLODO,
//:                  VIEW NewDTE  BASED ON LOD TZTENVRO )

//:   SHORT  nFoundDataSource
static zSHORT
oTZZOLODO_fnCopyPOD_Entity( zVIEW     vSubtask,
                            zVIEW     NewLOD,
                            zVIEW     OldLOD,
                            zVIEW     NewDTE )
{
   zSHORT    nFoundDataSource = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// Copy POD_Entities ( Join Definitions )
   //:IF NewDTE = 0
   if ( NewDTE == 0 )
   { 
      //:RETURN 0
      return( 0 );
   } 

   //:END

   //:IF NewDTE.TE_DBMS_Source EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( NewDTE, "TE_DBMS_Source" );
   //:   AND NewLOD.LOD_EntityParent.Work != "Y"
   //:   AND NewLOD.LOD_EntityParent.Derived != "Y"
   if ( lTempInteger_0 == 0 && CompareAttributeToString( NewLOD, "LOD_EntityParent", "Work", "Y" ) != 0 && CompareAttributeToString( NewLOD, "LOD_EntityParent", "Derived", "Y" ) != 0 )
   { 

      //:FOR EACH OldLOD.POD_Entity
      RESULT = SetCursorFirstEntity( OldLOD, "POD_Entity", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //: nFoundDataSource = 0
         nFoundDataSource = 0;
         //: SET CURSOR FIRST NewDTE.TE_DBMS_Source
         //:            WHERE NewDTE.TE_DBMS_Source.ZKey = OldLOD.TE_DBMS_SourceForEntity.ZKey
         GetIntegerFromAttribute( &lTempInteger_1, OldLOD, "TE_DBMS_SourceForEntity", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( NewDTE, "TE_DBMS_Source", "ZKey", lTempInteger_1, "" );

         //: IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //: nFoundDataSource = 1
            nFoundDataSource = 1;
            //:ELSE
         } 
         else
         { 
            //: SET CURSOR FIRST NewDTE.TE_DBMS_Source
            //:            WHERE NewDTE.TE_DBMS_Source.Name = OldLOD.TE_DBMS_SourceForEntity.Name
            GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldLOD, "TE_DBMS_SourceForEntity", "Name" );
            RESULT = SetCursorFirstEntityByString( NewDTE, "TE_DBMS_Source", "Name", szTempString_0, "" );

            //: LOOP WHILE RESULT >= zCURSOR_SET AND nFoundDataSource = 0
            while ( RESULT >= zCURSOR_SET && nFoundDataSource == 0 )
            { 

               //:   IF NewDTE.TE_DBMS_Source.DBMS = OldLOD.TE_DBMS_SourceForEntity.DBMS
               if ( CompareAttributeToAttribute( NewDTE, "TE_DBMS_Source", "DBMS", OldLOD, "TE_DBMS_SourceForEntity", "DBMS" ) == 0 )
               { 
                  //:   nFoundDataSource = 1
                  nFoundDataSource = 1;
                  //:ELSE
               } 
               else
               { 
                  //:   SET CURSOR NEXT  NewDTE.TE_DBMS_Source
                  //:              WHERE NewDTE.TE_DBMS_Source.Name = OldLOD.TE_DBMS_SourceForEntity.Name
                  GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldLOD, "TE_DBMS_SourceForEntity", "Name" );
                  RESULT = SetCursorNextEntityByString( NewDTE, "TE_DBMS_Source", "Name", szTempString_0, "" );
               } 

               //:   END
            } 

            //: END
         } 

         //: END

         //: IF nFoundDataSource = 1
         if ( nFoundDataSource == 1 )
         { 
            //: CreateMetaEntity( vSubtask, NewLOD, "POD_Entity", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewLOD, "POD_Entity", zPOS_AFTER );
            //: SetMatchingAttributesByName ( NewLOD, "POD_Entity",
            //:                               OldLOD, "POD_Entity", zSET_NULL )
            SetMatchingAttributesByName( NewLOD, "POD_Entity", OldLOD, "POD_Entity", zSET_NULL );
            //: INCLUDE NewLOD.TE_DBMS_SourceForEntity FROM NewDTE.TE_DBMS_Source
            RESULT = IncludeSubobjectFromSubobject( NewLOD, "TE_DBMS_SourceForEntity", NewDTE, "TE_DBMS_Source", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( OldLOD, "POD_Entity", "" );
         //: END
      } 

      //:END
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


//:DERIVED ATTRIBUTE OPERATION
//:dAttributeTextMapName( VIEW TZZOLODO BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) InternalEntityStructure,
//:                       STRING ( 32 ) InternalAttribStructure,
//:                       SHORT GetOrSetFlag )

//:   STRING ( 50 ) szTextMapName
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_dAttributeTextMapName( zVIEW     TZZOLODO,
                                 LPVIEWENTITY InternalEntityStructure,
                                 LPVIEWATTRIB InternalAttribStructure,
                                 zSHORT    GetOrSetFlag )
{
   zCHAR     szTextMapName[ 51 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :

         //:szTextMapName = "[Z:" +
         //:          TZZOLODO.LOD_Entity.Name + "." +
         //:          TZZOLODO.ER_Attribute.Name + "]"
         GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZZOLODO, "LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( szTextMapName, 1, 0, "[Z:", 1, 0, 51 );
         ZeidonStringConcat( szTextMapName, 1, 0, szTempString_0, 1, 0, 51 );
         ZeidonStringConcat( szTextMapName, 1, 0, ".", 1, 0, 51 );
         GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZZOLODO, "ER_Attribute", "Name", "", 0 );
         ZeidonStringConcat( szTextMapName, 1, 0, szTempString_1, 1, 0, 51 );
         ZeidonStringConcat( szTextMapName, 1, 0, "]", 1, 0, 51 );
         //:StoreStringInRecord( TZZOLODO,
         //:               InternalEntityStructure,
         //:               InternalAttribStructure,
         //:               szTextMapName )
         StoreStringInRecord( TZZOLODO, InternalEntityStructure, InternalAttribStructure, szTextMapName );
         break ;

      //:  /* end zDERIVED_GET */
      //:OF   zDERIVED_SET:
      case zDERIVED_SET :
         break ;
   } 


   //:     /* end zDERIVED_SET */
   //:END  /* case */
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_Compare( VIEW TargetLOD BASED ON LOD TZZOLODO,
//:             VIEW SourceLOD BASED ON LOD TZZOLODO )

//:   VIEW TargetLOD_Root BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_Compare( zVIEW     TargetLOD,
                       zVIEW     SourceLOD )
{
   zVIEW     TargetLOD_Root = 0; 
   //:STRING ( 1 ) szSecondLineFlag
   zCHAR     szSecondLineFlag[ 2 ] = { 0 }; 
   //:INTEGER      AttributeCount
   zLONG     AttributeCount = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zCHAR     szTempString_6[ 255 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 255 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 255 ]; 
   zCHAR     szTempString_13[ 33 ]; 
   zCHAR     szTempString_14[ 33 ]; 


   //:// Compare the Source LOD to the Target LOD, formatting the difference in the Target LOD.

   //:// First make sure we delete any previous work entries.
   //:FOR EACH TargetLOD.CompareEntityResult
   RESULT = SetCursorFirstEntity( TargetLOD, "CompareEntityResult", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TargetLOD.CompareEntityResult NONE
      RESULT = DeleteEntity( TargetLOD, "CompareEntityResult", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TargetLOD, "CompareEntityResult", "" );
   } 

   //:END
   //:FOR EACH TargetLOD.CompareOperationResult
   RESULT = SetCursorFirstEntity( TargetLOD, "CompareOperationResult", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TargetLOD.CompareOperationResult NONE
      RESULT = DeleteEntity( TargetLOD, "CompareOperationResult", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TargetLOD, "CompareOperationResult", "" );
   } 

   //:END

   //:// Go to process LOD_Entity/LOD_Attribute differences.
   //:CreateViewFromView( TargetLOD_Root, TargetLOD )
   CreateViewFromView( &TargetLOD_Root, TargetLOD );
   //:NAME VIEW TargetLOD_Root "TargetLOD_Root"
   SetNameForView( TargetLOD_Root, "TargetLOD_Root", 0, zLEVEL_TASK );

   //:LOD_CompareReus( TargetLOD, SourceLOD, TargetLOD_Root, "" )
   oTZZOLODO_LOD_CompareReus( TargetLOD, SourceLOD, TargetLOD_Root, "" );

   //:DropView( TargetLOD_Root )
   DropView( TargetLOD_Root );

   //:// Compare Operations and Parameters.
   //:LOD_CompareOperations( TargetLOD, SourceLOD )
   oTZZOLODO_LOD_CompareOperations( TargetLOD, SourceLOD );

   //:// Remove any Result entries that do not show a difference.
   //:FOR EACH TargetLOD.CompareEntityResult
   RESULT = SetCursorFirstEntity( TargetLOD, "CompareEntityResult", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:FOR EACH TargetLOD.CompareAttributeResult
      RESULT = SetCursorFirstEntity( TargetLOD, "CompareAttributeResult", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TargetLOD.CompareAttributeResult.DifferenceFlag = ""
         if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "DifferenceFlag", "" ) == 0 )
         { 
            //:// Delete Attribute entries that don't show a difference.
            //:DELETE ENTITY TargetLOD.CompareAttributeResult NONE
            RESULT = DeleteEntity( TargetLOD, "CompareAttributeResult", zREPOS_NONE );
         } 

         RESULT = SetCursorNextEntity( TargetLOD, "CompareAttributeResult", "" );
         //:END
      } 

      //:END
      //:SET CURSOR FIRST TargetLOD.CompareAttributeResult
      RESULT = SetCursorFirstEntity( TargetLOD, "CompareAttributeResult", "" );
      //:IF RESULT < zCURSOR_SET AND TargetLOD.CompareEntityResult.DifferenceFlag = ""
      if ( RESULT < zCURSOR_SET && CompareAttributeToString( TargetLOD, "CompareEntityResult", "DifferenceFlag", "" ) == 0 )
      { 
         //:// Delete Entity entries that don't show a difference and don't have Attributes with a difference.
         //:DELETE ENTITY TargetLOD.CompareEntityResult NONE
         RESULT = DeleteEntity( TargetLOD, "CompareEntityResult", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( TargetLOD, "CompareEntityResult", "" );
      //:END
   } 

   //:END

   //:// Build the Display entries for Entity and Attribute.
   //:// First delete current entries.
   //:FOR EACH TargetLOD.DisplayCompareResult
   RESULT = SetCursorFirstEntity( TargetLOD, "DisplayCompareResult", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TargetLOD.DisplayCompareResult NONE
      RESULT = DeleteEntity( TargetLOD, "DisplayCompareResult", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TargetLOD, "DisplayCompareResult", "" );
   } 

   //:END
   //:// Build new entries.
   //:FOR EACH TargetLOD.CompareEntityResult
   RESULT = SetCursorFirstEntity( TargetLOD, "CompareEntityResult", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Build Entity Differences
      //:CREATE ENTITY TargetLOD.DisplayCompareResult
      RESULT = CreateEntity( TargetLOD, "DisplayCompareResult", zPOS_AFTER );
      //:TargetLOD.DisplayCompareResult.EntityAttributeType = "E"
      SetAttributeFromString( TargetLOD, "DisplayCompareResult", "EntityAttributeType", "E" );
      //:TargetLOD.DisplayCompareResult.MetaID = TargetLOD.CompareEntityResult.EntityID
      SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "MetaID", TargetLOD, "CompareEntityResult", "EntityID" );
      //:IF TargetLOD.CompareEntityResult.DifferenceFlag = "T"
      if ( CompareAttributeToString( TargetLOD, "CompareEntityResult", "DifferenceFlag", "T" ) == 0 )
      { 
         //:// Entity exists in Target, but not in Source.
         //:TargetLOD.DisplayCompareResult.TargetEntityName = TargetLOD.CompareEntityResult.EntityName
         SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "TargetEntityName", TargetLOD, "CompareEntityResult", "EntityName" );
         //:TargetLOD.DisplayCompareResult.Difference       = "Target Entity not in Source"
         SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", "Target Entity not in Source" );
         //:ELSE
      } 
      else
      { 
         //:IF TargetLOD.CompareEntityResult.DifferenceFlag = "S"
         if ( CompareAttributeToString( TargetLOD, "CompareEntityResult", "DifferenceFlag", "S" ) == 0 )
         { 
            //:// Entity exists in Source, but not in Target.
            //:TargetLOD.DisplayCompareResult.SourceEntityName = TargetLOD.CompareEntityResult.EntityName
            SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "SourceEntityName", TargetLOD, "CompareEntityResult", "EntityName" );
            //:TargetLOD.DisplayCompareResult.Difference       = "Source Entity not in Target"
            SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", "Source Entity not in Target" );
            //:ELSE
         } 
         else
         { 
            //:// If we get to here, always display Source Entity Name.
            //:TargetLOD.DisplayCompareResult.SourceEntityName = TargetLOD.CompareEntityResult.EntityName
            SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "SourceEntityName", TargetLOD, "CompareEntityResult", "EntityName" );
            //:IF TargetLOD.CompareEntityResult.DifferenceFlag = "D"
            if ( CompareAttributeToString( TargetLOD, "CompareEntityResult", "DifferenceFlag", "D" ) == 0 )
            { 
               //:// There is a difference in the two Entities.
               //:IF TargetLOD.CompareEntityResult.TargetDifferentName != ""
               if ( CompareAttributeToString( TargetLOD, "CompareEntityResult", "TargetDifferentName", "" ) != 0 )
               { 
                  //:// There is a difference in names.
                  //:TargetLOD.DisplayCompareResult.TargetEntityName = TargetLOD.CompareEntityResult.TargetDifferentName
                  SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "TargetEntityName", TargetLOD, "CompareEntityResult", "TargetDifferentName" );
                  //:TargetLOD.DisplayCompareResult.Difference = "Name Change"
                  SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", "Name Change" );
               } 

               //:END
               //:IF TargetLOD.CompareEntityResult.SourceDifferentPermissions != "" OR
               //:   TargetLOD.CompareEntityResult.TargetDifferentPermissions != ""
               if ( CompareAttributeToString( TargetLOD, "CompareEntityResult", "SourceDifferentPermissions", "" ) != 0 || CompareAttributeToString( TargetLOD, "CompareEntityResult", "TargetDifferentPermissions", "" ) != 0 )
               { 

                  //:TargetLOD.DisplayCompareResult.Difference = "Permissions: " +
                  //:                                            TargetLOD.CompareEntityResult.SourceDifferentPermissions + " : " +
                  //:                                            TargetLOD.CompareEntityResult.TargetDifferentPermissions
                  GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TargetLOD, "CompareEntityResult", "SourceDifferentPermissions", "", 0 );
                  ZeidonStringCopy( szTempString_0, 1, 0, "Permissions: ", 1, 0, 255 );
                  ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 255 );
                  ZeidonStringConcat( szTempString_0, 1, 0, " : ", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_2, 0, 'S', 255, TargetLOD, "CompareEntityResult", "TargetDifferentPermissions", "", 0 );
                  ZeidonStringConcat( szTempString_0, 1, 0, szTempString_2, 1, 0, 255 );
                  SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", szTempString_0 );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:// Build Attribute Differences.
      //:AttributeCount = 0
      AttributeCount = 0;
      //:FOR EACH TargetLOD.CompareAttributeResult
      RESULT = SetCursorFirstEntity( TargetLOD, "CompareAttributeResult", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:AttributeCount = AttributeCount + 1
         AttributeCount = AttributeCount + 1;
         //:IF AttributeCount > 1 OR TargetLOD.DisplayCompareResult.Difference != ""
         if ( AttributeCount > 1 || CompareAttributeToString( TargetLOD, "DisplayCompareResult", "Difference", "" ) != 0 )
         { 
            //:// Don't create display entry for first attribute, unless the Entity had a difference.
            //:// This will put attribute data on entity display entry when there are only attribute differences.
            //:CREATE ENTITY TargetLOD.DisplayCompareResult
            RESULT = CreateEntity( TargetLOD, "DisplayCompareResult", zPOS_AFTER );
         } 

         //:END
         //:TargetLOD.DisplayCompareResult.EntityAttributeType   = "A"
         SetAttributeFromString( TargetLOD, "DisplayCompareResult", "EntityAttributeType", "A" );
         //:TargetLOD.DisplayCompareResult.MetaID = TargetLOD.CompareAttributeResult.AttributeID
         SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "MetaID", TargetLOD, "CompareAttributeResult", "AttributeID" );

         //:IF TargetLOD.CompareAttributeResult.DifferenceFlag = "T"
         if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "DifferenceFlag", "T" ) == 0 )
         { 
            //:// Attribute exists in Target, but not in Source.
            //:TargetLOD.DisplayCompareResult.TargetAttributeName = TargetLOD.CompareAttributeResult.AttributeName
            SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "TargetAttributeName", TargetLOD, "CompareAttributeResult", "AttributeName" );
            //:TargetLOD.DisplayCompareResult.Difference          = "Target Attribute not in Source"
            SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", "Target Attribute not in Source" );
            //://TargetLOD.DisplayCompareResult.TargetEntityName    = TargetLOD.CompareEntityResult.EntityName
            //:ELSE
         } 
         else
         { 
            //://TargetLOD.DisplayCompareResult.SourceEntityName = TargetLOD.CompareEntityResult.EntityName
            //:IF TargetLOD.CompareAttributeResult.DifferenceFlag = "S"
            if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "DifferenceFlag", "S" ) == 0 )
            { 
               //:// Attribute exists in Source, but not in Target.
               //:TargetLOD.DisplayCompareResult.SourceAttributeName = TargetLOD.CompareAttributeResult.AttributeName
               SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "SourceAttributeName", TargetLOD, "CompareAttributeResult", "AttributeName" );
               //:TargetLOD.DisplayCompareResult.Difference          = "Source Attribute not in Target"
               SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", "Source Attribute not in Target" );
               //:ELSE
            } 
            else
            { 
               //:// If we get to here, always display Source Entity Name.
               //:TargetLOD.DisplayCompareResult.SourceAttributeName = TargetLOD.CompareAttributeResult.AttributeName
               SetAttributeFromAttribute( TargetLOD, "DisplayCompareResult", "SourceAttributeName", TargetLOD, "CompareAttributeResult", "AttributeName" );

               //:// The current line will only reflect one type of difference. If there are other differences, they will each
               //:// generate an additional line without Source Name or Target Name.
               //:// Permissions
               //:IF TargetLOD.CompareAttributeResult.SourceDifferentPermissions != "" OR
               //:   TargetLOD.CompareAttributeResult.TargetDifferentPermissions != ""
               if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "SourceDifferentPermissions", "" ) != 0 || CompareAttributeToString( TargetLOD, "CompareAttributeResult", "TargetDifferentPermissions", "" ) != 0 )
               { 

                  //:TargetLOD.DisplayCompareResult.Difference = "Permissions: " +
                  //:                                            TargetLOD.CompareAttributeResult.SourceDifferentPermissions + " : " +
                  //:                                            TargetLOD.CompareAttributeResult.TargetDifferentPermissions
                  GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TargetLOD, "CompareAttributeResult", "SourceDifferentPermissions", "", 0 );
                  ZeidonStringCopy( szTempString_3, 1, 0, "Permissions: ", 1, 0, 255 );
                  ZeidonStringConcat( szTempString_3, 1, 0, szTempString_4, 1, 0, 255 );
                  ZeidonStringConcat( szTempString_3, 1, 0, " : ", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_5, 0, 'S', 255, TargetLOD, "CompareAttributeResult", "TargetDifferentPermissions", "", 0 );
                  ZeidonStringConcat( szTempString_3, 1, 0, szTempString_5, 1, 0, 255 );
                  SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", szTempString_3 );
                  //:szSecondLineFlag = "Y"
                  ZeidonStringCopy( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 );
               } 

               //:END
               //:// Operation
               //:IF TargetLOD.CompareAttributeResult.SourceDifferentOperationName != "" OR
               //:   TargetLOD.CompareAttributeResult.TargetDifferentOperationName != ""
               if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "SourceDifferentOperationName", "" ) != 0 || CompareAttributeToString( TargetLOD, "CompareAttributeResult", "TargetDifferentOperationName", "" ) != 0 )
               { 

                  //:IF szSecondLineFlag = "Y"
                  if ( ZeidonStringCompare( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 
                     //:CREATE ENTITY TargetLOD.DisplayCompareResult
                     RESULT = CreateEntity( TargetLOD, "DisplayCompareResult", zPOS_AFTER );
                  } 

                  //:END
                  //:TargetLOD.DisplayCompareResult.Difference = "Operation: " +
                  //:                                            TargetLOD.CompareAttributeResult.SourceDifferentOperationName + " : " +
                  //:                                            TargetLOD.CompareAttributeResult.TargetDifferentOperationName
                  GetVariableFromAttribute( szTempString_7, 0, 'S', 33, TargetLOD, "CompareAttributeResult", "SourceDifferentOperationName", "", 0 );
                  ZeidonStringCopy( szTempString_6, 1, 0, "Operation: ", 1, 0, 255 );
                  ZeidonStringConcat( szTempString_6, 1, 0, szTempString_7, 1, 0, 255 );
                  ZeidonStringConcat( szTempString_6, 1, 0, " : ", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_8, 0, 'S', 33, TargetLOD, "CompareAttributeResult", "TargetDifferentOperationName", "", 0 );
                  ZeidonStringConcat( szTempString_6, 1, 0, szTempString_8, 1, 0, 255 );
                  SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", szTempString_6 );
                  //:szSecondLineFlag = "Y"
                  ZeidonStringCopy( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 );
               } 

               //:END
               //:// Domain
               //:IF TargetLOD.CompareAttributeResult.SourceDifferentDomainName != "" OR
               //:   TargetLOD.CompareAttributeResult.TargetDifferentDomainName != ""
               if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "SourceDifferentDomainName", "" ) != 0 || CompareAttributeToString( TargetLOD, "CompareAttributeResult", "TargetDifferentDomainName", "" ) != 0 )
               { 

                  //:IF szSecondLineFlag = "Y"
                  if ( ZeidonStringCompare( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 
                     //:CREATE ENTITY TargetLOD.DisplayCompareResult
                     RESULT = CreateEntity( TargetLOD, "DisplayCompareResult", zPOS_AFTER );
                  } 

                  //:END
                  //:TargetLOD.DisplayCompareResult.Difference = "Domain: " +
                  //:                                            TargetLOD.CompareAttributeResult.SourceDifferentDomainName + " : " +
                  //:                                            TargetLOD.CompareAttributeResult.TargetDifferentDomainName
                  GetVariableFromAttribute( szTempString_10, 0, 'S', 33, TargetLOD, "CompareAttributeResult", "SourceDifferentDomainName", "", 0 );
                  ZeidonStringCopy( szTempString_9, 1, 0, "Domain: ", 1, 0, 255 );
                  ZeidonStringConcat( szTempString_9, 1, 0, szTempString_10, 1, 0, 255 );
                  ZeidonStringConcat( szTempString_9, 1, 0, " : ", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_11, 0, 'S', 33, TargetLOD, "CompareAttributeResult", "TargetDifferentDomainName", "", 0 );
                  ZeidonStringConcat( szTempString_9, 1, 0, szTempString_11, 1, 0, 255 );
                  SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", szTempString_9 );
                  //:szSecondLineFlag = "Y"
                  ZeidonStringCopy( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 );
               } 

               //:END
               //:// String Length
               //:IF TargetLOD.CompareAttributeResult.SourceDifferentStringLength != "" OR
               //:   TargetLOD.CompareAttributeResult.TargetDifferentStringLength != ""
               if ( CompareAttributeToString( TargetLOD, "CompareAttributeResult", "SourceDifferentStringLength", "" ) != 0 || CompareAttributeToString( TargetLOD, "CompareAttributeResult", "TargetDifferentStringLength", "" ) != 0 )
               { 

                  //:IF szSecondLineFlag = "Y"
                  if ( ZeidonStringCompare( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 
                     //:CREATE ENTITY TargetLOD.DisplayCompareResult
                     RESULT = CreateEntity( TargetLOD, "DisplayCompareResult", zPOS_AFTER );
                  } 

                  //:END
                  //:TargetLOD.DisplayCompareResult.Difference = "Str Lth: " +
                  //:                                            TargetLOD.CompareAttributeResult.SourceDifferentStringLength + " : " +
                  //:                                            TargetLOD.CompareAttributeResult.TargetDifferentStringLength
                  GetVariableFromAttribute( szTempString_13, 0, 'S', 33, TargetLOD, "CompareAttributeResult", "SourceDifferentStringLength", "", 0 );
                  ZeidonStringCopy( szTempString_12, 1, 0, "Str Lth: ", 1, 0, 255 );
                  ZeidonStringConcat( szTempString_12, 1, 0, szTempString_13, 1, 0, 255 );
                  ZeidonStringConcat( szTempString_12, 1, 0, " : ", 1, 0, 255 );
                  GetVariableFromAttribute( szTempString_14, 0, 'S', 33, TargetLOD, "CompareAttributeResult", "TargetDifferentStringLength", "", 0 );
                  ZeidonStringConcat( szTempString_12, 1, 0, szTempString_14, 1, 0, 255 );
                  SetAttributeFromString( TargetLOD, "DisplayCompareResult", "Difference", szTempString_12 );
                  //:szSecondLineFlag = "Y"
                  ZeidonStringCopy( szSecondLineFlag, 1, 0, "Y", 1, 0, 2 );
               } 

               //:END
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( TargetLOD, "CompareAttributeResult", "" );
         //:END
      } 

      RESULT = SetCursorNextEntity( TargetLOD, "CompareEntityResult", "" );


      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:LOD_CompareReus( VIEW TargetLOD      BASED ON LOD TZZOLODO,
//:                 VIEW SourceLOD      BASED ON LOD TZZOLODO,
//:                 VIEW TargetLOD_Root BASED ON LOD TZZOLODO,
//:                 STRING ( 1000 ) szParentNames )

//:   STRING ( 1000 ) szNewParentNames
static zSHORT
oTZZOLODO_LOD_CompareReus( zVIEW     TargetLOD,
                           zVIEW     SourceLOD,
                           zVIEW     TargetLOD_Root,
                           zPCHAR    szParentNames )
{
   zCHAR     szNewParentNames[ 1001 ] = { 0 }; 
   //:STRING ( 1 )    szER_EntityFound
   zCHAR     szER_EntityFound[ 2 ] = { 0 }; 
   //:STRING ( 32 )   szSourceEntityName
   zCHAR     szSourceEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szTargetEntityName
   zCHAR     szTargetEntityName[ 33 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zSHORT    lTempInteger_7; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// Compare LOD's at current level.

   //:// First try to match the Source entries to the Target Entries.
   //:FOR EACH SourceLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( SourceLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Create compare entry
      //:CREATE ENTITY TargetLOD_Root.CompareEntityResult
      RESULT = CreateEntity( TargetLOD_Root, "CompareEntityResult", zPOS_AFTER );
      //:TargetLOD_Root.CompareEntityResult.EntityName = SourceLOD.LOD_EntityParent.Name
      SetAttributeFromAttribute( TargetLOD_Root, "CompareEntityResult", "EntityName", SourceLOD, "LOD_EntityParent", "Name" );
      //:TargetLOD_Root.CompareEntityResult.EntityID   = SourceLOD.LOD_EntityParent.ZKey
      SetAttributeFromAttribute( TargetLOD_Root, "CompareEntityResult", "EntityID", SourceLOD, "LOD_EntityParent", "ZKey" );

      //:// If the Source Entity is a work entity (ie., not tied to an ER_Entity), look for a match on name.
      //:// If the Source Entity is an ER Entity, find a match on ER Entity Name.
      //:IF SourceLOD.ER_EntityRec EXISTS AND SourceLOD.ER_RelLinkRec EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( SourceLOD, "ER_EntityRec" );
      lTempInteger_1 = CheckExistenceOfEntity( SourceLOD, "ER_RelLinkRec" );
      if ( lTempInteger_0 == 0 && lTempInteger_1 == 0 )
      { 
         //:// This is an ER Entity. We can't do a simple set cursor so we'll loop through entries for a match.
         //:szER_EntityFound = ""
         ZeidonStringCopy( szER_EntityFound, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST TargetLOD.LOD_EntityParent
         RESULT = SetCursorFirstEntity( TargetLOD, "LOD_EntityParent", "" );
         //:LOOP WHILE RESULT >= zCURSOR_SET AND szER_EntityFound = ""
         while ( RESULT >= zCURSOR_SET && ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //:// Compare ER_Entities.
            //:IF TargetLOD.ER_EntityRec EXISTS AND TargetLOD.ER_RelLinkRec EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( TargetLOD, "ER_EntityRec" );
            lTempInteger_3 = CheckExistenceOfEntity( TargetLOD, "ER_RelLinkRec" );
            if ( lTempInteger_2 == 0 && lTempInteger_3 == 0 )
            { 
               //:IF TargetLOD.ER_EntityRec.Name  = SourceLOD.ER_EntityRec.Name AND
               //:   TargetLOD.ER_RelLinkRec.Name = SourceLOD.ER_RelLinkRec.Name
               if ( CompareAttributeToAttribute( TargetLOD, "ER_EntityRec", "Name", SourceLOD, "ER_EntityRec", "Name" ) == 0 && CompareAttributeToAttribute( TargetLOD, "ER_RelLinkRec", "Name", SourceLOD, "ER_RelLinkRec", "Name" ) == 0 )
               { 

                  //:szER_EntityFound = "Y"
                  ZeidonStringCopy( szER_EntityFound, 1, 0, "Y", 1, 0, 2 );
               } 

               //:END
            } 

            //:END
            //:// Set next cursor if we haven't found a match.
            //:IF szER_EntityFound = ""
            if ( ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
            { 
               //:SET CURSOR NEXT TargetLOD.LOD_EntityParent
               RESULT = SetCursorNextEntity( TargetLOD, "LOD_EntityParent", "" );
            } 

            //:END
         } 

         //:END
         //:IF szER_EntityFound = "Y"
         if ( ZeidonStringCompare( szER_EntityFound, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:// Set szSourceEntityName and szTargetEntityName, if the LOD Entity Name has changed.
            //:IF TargetLOD.LOD_EntityParent.Name = SourceLOD.LOD_EntityParent.Name
            if ( CompareAttributeToAttribute( TargetLOD, "LOD_EntityParent", "Name", SourceLOD, "LOD_EntityParent", "Name" ) == 0 )
            { 
               //:szSourceEntityName = ""
               ZeidonStringCopy( szSourceEntityName, 1, 0, "", 1, 0, 33 );
               //:szTargetEntityName = ""
               ZeidonStringCopy( szTargetEntityName, 1, 0, "", 1, 0, 33 );
               //:ELSE
            } 
            else
            { 
               //:szSourceEntityName = SourceLOD.LOD_EntityParent.Name
               GetVariableFromAttribute( szSourceEntityName, 0, 'S', 33, SourceLOD, "LOD_EntityParent", "Name", "", 0 );
               //:szTargetEntityName = TargetLOD.LOD_EntityParent.Name
               GetVariableFromAttribute( szTargetEntityName, 0, 'S', 33, TargetLOD, "LOD_EntityParent", "Name", "", 0 );
               //:TargetLOD_Root.CompareEntityResult.DifferenceFlag = "D"
               SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "DifferenceFlag", "D" );
            } 

            //:END

            //:RESULT = 0     // This forces the equal case below.
            RESULT = 0;
            //:ELSE
         } 
         else
         { 
            //:RESULT = -1
            RESULT = -1;
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// This is a work entity so just do set cursor on LOD Entity Name.
         //:SET CURSOR FIRST TargetLOD.LOD_EntityParent
         //:           WHERE TargetLOD.LOD_EntityParent.Name = SourceLOD.LOD_EntityParent.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), SourceLOD, "LOD_EntityParent", "Name" );
         RESULT = SetCursorFirstEntityByString( TargetLOD, "LOD_EntityParent", "Name", szTempString_0, "" );
         //:// We don't use szSourceEntityName or szTargetEntityName in this case, so just null them.
         //:szSourceEntityName = ""
         ZeidonStringCopy( szSourceEntityName, 1, 0, "", 1, 0, 33 );
         //:szTargetEntityName = ""
         ZeidonStringCopy( szTargetEntityName, 1, 0, "", 1, 0, 33 );
      } 

      //:END
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 

         //:// Source entity is found as Target entity, so compare detail.
         //:LOD_CompareEntity( TargetLOD, SourceLOD, TargetLOD_Root )
         oTZZOLODO_LOD_CompareEntity( TargetLOD, SourceLOD, TargetLOD_Root );

         //:// Set Source and Target names to capture any differences determined above.
         //:TargetLOD_Root.CompareEntityResult.SourceDifferentName = szSourceEntityName
         SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "SourceDifferentName", szSourceEntityName );
         //:TargetLOD_Root.CompareEntityResult.TargetDifferentName = szTargetEntityName
         SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "TargetDifferentName", szTargetEntityName );

         //:// Next process any subobjects.
         //:IF szParentNames = ""
         if ( ZeidonStringCompare( szParentNames, 1, 0, "", 1, 0, 1001 ) == 0 )
         { 
            //:szNewParentNames = TargetLOD.LOD_EntityParent.Name
            GetVariableFromAttribute( szNewParentNames, 0, 'S', 1001, TargetLOD, "LOD_EntityParent", "Name", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szNewParentNames = szParentNames + "/" + TargetLOD.LOD_EntityParent.Name
            ZeidonStringCopy( szNewParentNames, 1, 0, szParentNames, 1, 0, 1001 );
            ZeidonStringConcat( szNewParentNames, 1, 0, "/", 1, 0, 1001 );
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TargetLOD, "LOD_EntityParent", "Name", "", 0 );
            ZeidonStringConcat( szNewParentNames, 1, 0, szTempString_0, 1, 0, 1001 );
         } 

         //:END

         //:SetViewToSubobject( SourceLOD, "LOD_EntityChild" )
         SetViewToSubobject( SourceLOD, "LOD_EntityChild" );
         //:SetViewToSubobject( TargetLOD, "LOD_EntityChild" )
         SetViewToSubobject( TargetLOD, "LOD_EntityChild" );
         //:LOD_CompareReus( TargetLOD, SourceLOD, TargetLOD_Root, szNewParentNames )
         oTZZOLODO_LOD_CompareReus( TargetLOD, SourceLOD, TargetLOD_Root, szNewParentNames );
         //:ResetViewFromSubobject( SourceLOD )
         ResetViewFromSubobject( SourceLOD );
         //:ResetViewFromSubobject( TargetLOD )
         ResetViewFromSubobject( TargetLOD );

         //:ELSE
      } 
      else
      { 
         //:// Source entity does not exist as Target entity.
         //:TargetLOD_Root.CompareEntityResult.DifferenceFlag = "S"
         SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "DifferenceFlag", "S" );
         //:TargetLOD_Root.CompareEntityResult.ParentsConcatenatedName = szParentNames
         SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "ParentsConcatenatedName", szParentNames );
      } 

      RESULT = SetCursorNextEntity( SourceLOD, "LOD_EntityParent", "" );
      //:END
   } 

   //:END

   //:// Next, try to match the Target entries to the Source Entries.
   //:FOR EACH TargetLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( TargetLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// If the Target Entity is a work entity (ie., not tied to an ER_Entity, look for a match on name.
      //:// If the Target Entity is an ER Entity, find a match on ER Entity Name.
      //:IF TargetLOD.ER_EntityRec EXISTS AND TargetLOD.ER_RelLinkRec EXISTS
      lTempInteger_4 = CheckExistenceOfEntity( TargetLOD, "ER_EntityRec" );
      lTempInteger_5 = CheckExistenceOfEntity( TargetLOD, "ER_RelLinkRec" );
      if ( lTempInteger_4 == 0 && lTempInteger_5 == 0 )
      { 
         //:// This is an ER Entity. We can't do a simple set cursor so we'll loop through entries for a match.
         //:szER_EntityFound = ""
         ZeidonStringCopy( szER_EntityFound, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST SourceLOD.LOD_EntityParent
         RESULT = SetCursorFirstEntity( SourceLOD, "LOD_EntityParent", "" );
         //:LOOP WHILE RESULT >= zCURSOR_SET AND szER_EntityFound = ""
         while ( RESULT >= zCURSOR_SET && ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //:// Compare ER_Entities.
            //:IF SourceLOD.ER_EntityRec EXISTS AND SourceLOD.ER_RelLinkRec EXISTS
            lTempInteger_6 = CheckExistenceOfEntity( SourceLOD, "ER_EntityRec" );
            lTempInteger_7 = CheckExistenceOfEntity( SourceLOD, "ER_RelLinkRec" );
            if ( lTempInteger_6 == 0 && lTempInteger_7 == 0 )
            { 
               //:IF SourceLOD.ER_EntityRec.Name  = TargetLOD.ER_EntityRec.Name AND
               //:   SourceLOD.ER_RelLinkRec.Name = TargetLOD.ER_RelLinkRec.Name
               if ( CompareAttributeToAttribute( SourceLOD, "ER_EntityRec", "Name", TargetLOD, "ER_EntityRec", "Name" ) == 0 && CompareAttributeToAttribute( SourceLOD, "ER_RelLinkRec", "Name", TargetLOD, "ER_RelLinkRec", "Name" ) == 0 )
               { 

                  //:szER_EntityFound = "Y"
                  ZeidonStringCopy( szER_EntityFound, 1, 0, "Y", 1, 0, 2 );
               } 

               //:END
            } 

            //:END
            //:// Set next cursor if we haven't found a match.
            //:IF szER_EntityFound = ""
            if ( ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
            { 
               //:SET CURSOR NEXT SourceLOD.LOD_EntityParent
               RESULT = SetCursorNextEntity( SourceLOD, "LOD_EntityParent", "" );
            } 

            //:END
         } 

         //:END
         //:IF szER_EntityFound = "Y"
         if ( ZeidonStringCompare( szER_EntityFound, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:// We skip the match condition here because it was processed above.
            //:RESULT = 0     // This forces the equal case below.
            RESULT = 0;
            //:ELSE
         } 
         else
         { 
            //:RESULT = -1
            RESULT = -1;
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// This is a work entity so just do set cursor on LOD Entity Name.
         //:SET CURSOR FIRST SourceLOD.LOD_EntityParent
         //:           WHERE SourceLOD.LOD_EntityParent.Name = TargetLOD.LOD_EntityParent.Name
         GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TargetLOD, "LOD_EntityParent", "Name" );
         RESULT = SetCursorFirstEntityByString( SourceLOD, "LOD_EntityParent", "Name", szTempString_1, "" );
      } 

      //:END
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// Target Entity  does not exist as Source entity.
         //:// This is the only case we  care about in this pass.
         //:CREATE ENTITY TargetLOD_Root.CompareEntityResult
         RESULT = CreateEntity( TargetLOD_Root, "CompareEntityResult", zPOS_AFTER );
         //:TargetLOD_Root.CompareEntityResult.EntityName = TargetLOD.LOD_EntityParent.Name
         SetAttributeFromAttribute( TargetLOD_Root, "CompareEntityResult", "EntityName", TargetLOD, "LOD_EntityParent", "Name" );
         //:TargetLOD_Root.CompareEntityResult.EntityID   = TargetLOD.LOD_EntityParent.ZKey
         SetAttributeFromAttribute( TargetLOD_Root, "CompareEntityResult", "EntityID", TargetLOD, "LOD_EntityParent", "ZKey" );
         //:TargetLOD_Root.CompareEntityResult.DifferenceFlag = "T"
         SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "DifferenceFlag", "T" );
         //:TargetLOD_Root.CompareEntityResult.ParentsConcatenatedName = szParentNames
         SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "ParentsConcatenatedName", szParentNames );
      } 

      RESULT = SetCursorNextEntity( TargetLOD, "LOD_EntityParent", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
oTZZOLODO_LOD_CompareOperations( zVIEW     TargetLOD,
                                 zVIEW     SourceLOD )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 65 ]; 

   //:LOD_CompareOperations( VIEW TargetLOD BASED ON LOD TZZOLODO,
   //:                    VIEW SourceLOD BASED ON LOD TZZOLODO )

   //:// First try to match the Source entries to the Target Entries.
   //:FOR EACH SourceLOD.SourceFile
   RESULT = SetCursorFirstEntity( SourceLOD, "SourceFile", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:FOR EACH SourceLOD.Operation
      RESULT = SetCursorFirstEntity( SourceLOD, "Operation", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// Create compare entry
         //:CREATE ENTITY TargetLOD.CompareOperationResult
         RESULT = CreateEntity( TargetLOD, "CompareOperationResult", zPOS_AFTER );
         //:TargetLOD.CompareOperationResult.OperationName = SourceLOD.Operation.Name
         SetAttributeFromAttribute( TargetLOD, "CompareOperationResult", "OperationName", SourceLOD, "Operation", "Name" );

         //:SET CURSOR FIRST TargetLOD.Operation WITHIN TargetLOD.LOD
         //:           WHERE TargetLOD.Operation.Name = SourceLOD.Operation.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), SourceLOD, "Operation", "Name" );
         RESULT = SetCursorFirstEntityByString( TargetLOD, "Operation", "Name", szTempString_0, "LOD" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:// Source Operation is found as Target Operation, so compare Parameters.
            //:// First look for Source in Target.
            //:FOR EACH SourceLOD.Parameter
            RESULT = SetCursorFirstEntity( SourceLOD, "Parameter", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:SET CURSOR FIRST TargetLOD.Parameter WHERE TargetLOD.Parameter.ShortDesc = SourceLOD.Parameter.ShortDesc
               GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), SourceLOD, "Parameter", "ShortDesc" );
               RESULT = SetCursorFirstEntityByString( TargetLOD, "Parameter", "ShortDesc", szTempString_1, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:// Source Parameter is not found in Target.
                  //:CREATE ENTITY TargetLOD.CompareParameterResult
                  RESULT = CreateEntity( TargetLOD, "CompareParameterResult", zPOS_AFTER );
                  //:TargetLOD.CompareParameterResult.ParameterName  = SourceLOD.Parameter.ShortDesc
                  SetAttributeFromAttribute( TargetLOD, "CompareParameterResult", "ParameterName", SourceLOD, "Parameter", "ShortDesc" );
                  //:TargetLOD.CompareParameterResult.Difference     = "Parameter Only in Source"
                  SetAttributeFromString( TargetLOD, "CompareParameterResult", "Difference", "Parameter Only in Source" );
               } 

               RESULT = SetCursorNextEntity( SourceLOD, "Parameter", "" );
               //:END
            } 

            //:END
            //:// Next look for Target in Source
            //:FOR EACH TargetLOD.Parameter
            RESULT = SetCursorFirstEntity( TargetLOD, "Parameter", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:SET CURSOR FIRST SourceLOD.Parameter WHERE SourceLOD.Parameter.ShortDesc = TargetLOD.Parameter.ShortDesc
               GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), TargetLOD, "Parameter", "ShortDesc" );
               RESULT = SetCursorFirstEntityByString( SourceLOD, "Parameter", "ShortDesc", szTempString_1, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:// Source Parameter is not found in Target.
                  //:CREATE ENTITY TargetLOD.CompareParameterResult
                  RESULT = CreateEntity( TargetLOD, "CompareParameterResult", zPOS_AFTER );
                  //:TargetLOD.CompareParameterResult.ParameterName  = TargetLOD.Parameter.ShortDesc
                  SetAttributeFromAttribute( TargetLOD, "CompareParameterResult", "ParameterName", TargetLOD, "Parameter", "ShortDesc" );
                  //:TargetLOD.CompareParameterResult.Difference     = "Parameter Only in Target"
                  SetAttributeFromString( TargetLOD, "CompareParameterResult", "Difference", "Parameter Only in Target" );
               } 

               RESULT = SetCursorNextEntity( TargetLOD, "Parameter", "" );
               //:END
            } 

            //:END
            //:// Remove Results entry if no Parameter difference.
            //:SET CURSOR FIRST TargetLOD.CompareParameterResult
            RESULT = SetCursorFirstEntity( TargetLOD, "CompareParameterResult", "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:DELETE ENTITY TargetLOD.CompareOperationResult
               RESULT = DeleteEntity( TargetLOD, "CompareOperationResult", zPOS_NEXT );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:// Source Operation does not exist as Target Operation. Create CompareParameterResult parameter for
            //:// displaying result.
            //:CREATE ENTITY TargetLOD.CompareParameterResult
            RESULT = CreateEntity( TargetLOD, "CompareParameterResult", zPOS_AFTER );
            //:TargetLOD.CompareParameterResult.Difference = "Operation Only in Source"
            SetAttributeFromString( TargetLOD, "CompareParameterResult", "Difference", "Operation Only in Source" );
         } 

         RESULT = SetCursorNextEntity( SourceLOD, "Operation", "" );
         //:END
      } 

      RESULT = SetCursorNextEntity( SourceLOD, "SourceFile", "" );
      //:END
   } 

   //:END

   //:// Next, try to match Target to Source.
   //:FOR EACH TargetLOD.SourceFile
   RESULT = SetCursorFirstEntity( TargetLOD, "SourceFile", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:FOR EACH TargetLOD.Operation
      RESULT = SetCursorFirstEntity( TargetLOD, "Operation", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST SourceLOD.Operation WITHIN SourceLOD.LOD
         //:           WHERE SourceLOD.Operation.Name = TargetLOD.Operation.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), TargetLOD, "Operation", "Name" );
         RESULT = SetCursorFirstEntityByString( SourceLOD, "Operation", "Name", szTempString_0, "LOD" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// The only result we care about here is when the Target Operation is not in the Source.
            //:CREATE ENTITY TargetLOD.CompareOperationResult
            RESULT = CreateEntity( TargetLOD, "CompareOperationResult", zPOS_AFTER );
            //:TargetLOD.CompareOperationResult.OperationName = TargetLOD.Operation.Name
            SetAttributeFromAttribute( TargetLOD, "CompareOperationResult", "OperationName", TargetLOD, "Operation", "Name" );
            //:CREATE ENTITY TargetLOD.CompareParameterResult
            RESULT = CreateEntity( TargetLOD, "CompareParameterResult", zPOS_AFTER );
            //:TargetLOD.CompareParameterResult.Difference = "Operation Only in Target"
            SetAttributeFromString( TargetLOD, "CompareParameterResult", "Difference", "Operation Only in Target" );
         } 

         RESULT = SetCursorNextEntity( TargetLOD, "Operation", "" );
         //:END
      } 

      RESULT = SetCursorNextEntity( TargetLOD, "SourceFile", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:LOD_CompareEntity( VIEW TargetLOD      BASED ON LOD TZZOLODO,
//:                   VIEW SourceLOD      BASED ON LOD TZZOLODO,
//:                   VIEW TargetLOD_Root BASED ON LOD TZZOLODO )

//:   STRING ( 50 ) szPermissionsTarget
static zSHORT
oTZZOLODO_LOD_CompareEntity( zVIEW     TargetLOD,
                             zVIEW     SourceLOD,
                             zVIEW     TargetLOD_Root )
{
   zCHAR     szPermissionsTarget[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szPermissionsSource
   zCHAR     szPermissionsSource[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szOperationTarget
   zCHAR     szOperationTarget[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szOperationSource
   zCHAR     szOperationSource[ 51 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //:// Compare LOD Entities.
   //:// Compare add/update permissions of form, "Create/Update/Delete/Include/Exclude" for comparison.

   //:// Set Target Permissions
   //:IF TargetLOD.LOD_EntityParent.Create = "Y"
   if ( CompareAttributeToString( TargetLOD, "LOD_EntityParent", "Create", "Y" ) == 0 )
   { 
      //:IF szPermissionsTarget = ""
      if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsTarget = "Create"
         ZeidonStringCopy( szPermissionsTarget, 1, 0, "Create", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsTarget = szPermissionsTarget + "/Create"
         ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Create", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF TargetLOD.LOD_EntityParent.Update = "Y"
   if ( CompareAttributeToString( TargetLOD, "LOD_EntityParent", "Update", "Y" ) == 0 )
   { 
      //:IF szPermissionsTarget = ""
      if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsTarget = "Update"
         ZeidonStringCopy( szPermissionsTarget, 1, 0, "Update", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsTarget = szPermissionsTarget + "/Update"
         ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Update", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF TargetLOD.LOD_EntityParent.Delete = "Y"
   if ( CompareAttributeToString( TargetLOD, "LOD_EntityParent", "Delete", "Y" ) == 0 )
   { 
      //:IF szPermissionsTarget = ""
      if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsTarget = "Delete"
         ZeidonStringCopy( szPermissionsTarget, 1, 0, "Delete", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsTarget = szPermissionsTarget + "/Delete"
         ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Delete", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF TargetLOD.LOD_EntityParent.Include = "Y"
   if ( CompareAttributeToString( TargetLOD, "LOD_EntityParent", "Include", "Y" ) == 0 )
   { 
      //:IF szPermissionsTarget = ""
      if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsTarget = "Include"
         ZeidonStringCopy( szPermissionsTarget, 1, 0, "Include", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsTarget = szPermissionsTarget + "/Include"
         ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Include", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF TargetLOD.LOD_EntityParent.Exclude = "Y"
   if ( CompareAttributeToString( TargetLOD, "LOD_EntityParent", "Exclude", "Y" ) == 0 )
   { 
      //:IF szPermissionsTarget = ""
      if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsTarget = "Exclude"
         ZeidonStringCopy( szPermissionsTarget, 1, 0, "Exclude", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsTarget = szPermissionsTarget + "/Exclude"
         ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Exclude", 1, 0, 51 );
      } 

      //:END
   } 

   //:END

   //:// Set Source Permissions
   //:IF SourceLOD.LOD_EntityParent.Create = "Y"
   if ( CompareAttributeToString( SourceLOD, "LOD_EntityParent", "Create", "Y" ) == 0 )
   { 
      //:IF szPermissionsSource = ""
      if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsSource = "Create"
         ZeidonStringCopy( szPermissionsSource, 1, 0, "Create", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsSource = szPermissionsSource + "/Create"
         ZeidonStringConcat( szPermissionsSource, 1, 0, "/Create", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF SourceLOD.LOD_EntityParent.Update = "Y"
   if ( CompareAttributeToString( SourceLOD, "LOD_EntityParent", "Update", "Y" ) == 0 )
   { 
      //:IF szPermissionsSource = ""
      if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsSource = "Update"
         ZeidonStringCopy( szPermissionsSource, 1, 0, "Update", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsSource = szPermissionsSource + "/Update"
         ZeidonStringConcat( szPermissionsSource, 1, 0, "/Update", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF SourceLOD.LOD_EntityParent.Delete = "Y"
   if ( CompareAttributeToString( SourceLOD, "LOD_EntityParent", "Delete", "Y" ) == 0 )
   { 
      //:IF szPermissionsSource = ""
      if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsSource = "Delete"
         ZeidonStringCopy( szPermissionsSource, 1, 0, "Delete", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsSource = szPermissionsSource + "/Delete"
         ZeidonStringConcat( szPermissionsSource, 1, 0, "/Delete", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF SourceLOD.LOD_EntityParent.Include = "Y"
   if ( CompareAttributeToString( SourceLOD, "LOD_EntityParent", "Include", "Y" ) == 0 )
   { 
      //:IF szPermissionsSource = ""
      if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsSource = "Include"
         ZeidonStringCopy( szPermissionsSource, 1, 0, "Include", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsSource = szPermissionsSource + "/Include"
         ZeidonStringConcat( szPermissionsSource, 1, 0, "/Include", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:IF SourceLOD.LOD_EntityParent.Exclude = "Y"
   if ( CompareAttributeToString( SourceLOD, "LOD_EntityParent", "Exclude", "Y" ) == 0 )
   { 
      //:IF szPermissionsSource = ""
      if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szPermissionsSource = "Exclude"
         ZeidonStringCopy( szPermissionsSource, 1, 0, "Exclude", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szPermissionsSource = szPermissionsSource + "/Exclude"
         ZeidonStringConcat( szPermissionsSource, 1, 0, "/Exclude", 1, 0, 51 );
      } 

      //:END
   } 

   //:END

   //:// Compare Permissions
   //:IF szPermissionsTarget != szPermissionsSource
   if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, szPermissionsSource, 1, 0, 51 ) != 0 )
   { 
      //:TargetLOD_Root.CompareEntityResult.DifferenceFlag = "D"
      SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "DifferenceFlag", "D" );
      //:TargetLOD_Root.CompareEntityResult.SourceDifferentPermissions = szPermissionsSource
      SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "SourceDifferentPermissions", szPermissionsSource );
      //:TargetLOD_Root.CompareEntityResult.TargetDifferentPermissions = szPermissionsTarget
      SetAttributeFromString( TargetLOD_Root, "CompareEntityResult", "TargetDifferentPermissions", szPermissionsTarget );
   } 

   //:END

   //:// First try to match the Source Attributes to the Target Attributes.
   //:FOR EACH SourceLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( SourceLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Create compare entry.
      //:CREATE ENTITY TargetLOD_Root.CompareAttributeResult
      RESULT = CreateEntity( TargetLOD_Root, "CompareAttributeResult", zPOS_AFTER );
      //:TargetLOD_Root.CompareAttributeResult.AttributeName = SourceLOD.ER_AttributeRec.Name
      SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "AttributeName", SourceLOD, "ER_AttributeRec", "Name" );
      //:TargetLOD_Root.CompareAttributeResult.AttributeID   = SourceLOD.LOD_AttributeRec.ZKey
      SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "AttributeID", SourceLOD, "LOD_AttributeRec", "ZKey" );

      //:SET CURSOR FIRST TargetLOD.LOD_AttributeRec WHERE TargetLOD.ER_AttributeRec.Name = SourceLOD.ER_AttributeRec.Name
      RESULT = SetCursorFirstEntity( TargetLOD, "LOD_AttributeRec", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TargetLOD, "ER_AttributeRec", "Name", SourceLOD, "ER_AttributeRec", "Name" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( TargetLOD, "LOD_AttributeRec", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Source Attribute is found in Target, so compare other data.

         //:// Check Work/Update Permissions.
         //:// Target
         //:szPermissionsTarget = ""
         ZeidonStringCopy( szPermissionsTarget, 1, 0, "", 1, 0, 51 );
         //:IF TargetLOD.LOD_AttributeRec.Work  = "Y"
         if ( CompareAttributeToString( TargetLOD, "LOD_AttributeRec", "Work", "Y" ) == 0 )
         { 
            //:IF szPermissionsTarget = ""
            if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
            { 
               //:szPermissionsTarget = "Work"
               ZeidonStringCopy( szPermissionsTarget, 1, 0, "Work", 1, 0, 51 );
               //:ELSE
            } 
            else
            { 
               //:szPermissionsTarget = szPermissionsTarget + "/Work"
               ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Work", 1, 0, 51 );
            } 

            //:END
         } 

         //:END
         //:IF TargetLOD.LOD_AttributeRec.Update  = "Y"
         if ( CompareAttributeToString( TargetLOD, "LOD_AttributeRec", "Update", "Y" ) == 0 )
         { 
            //:IF szPermissionsTarget = ""
            if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, "", 1, 0, 51 ) == 0 )
            { 
               //:szPermissionsTarget = "Update"
               ZeidonStringCopy( szPermissionsTarget, 1, 0, "Update", 1, 0, 51 );
               //:ELSE
            } 
            else
            { 
               //:szPermissionsTarget = szPermissionsTarget + "/Update"
               ZeidonStringConcat( szPermissionsTarget, 1, 0, "/Update", 1, 0, 51 );
            } 

            //:END
         } 

         //:END
         //:// Source
         //:szPermissionsSource = ""
         ZeidonStringCopy( szPermissionsSource, 1, 0, "", 1, 0, 51 );
         //:IF SourceLOD.LOD_AttributeRec.Work  = "Y"
         if ( CompareAttributeToString( SourceLOD, "LOD_AttributeRec", "Work", "Y" ) == 0 )
         { 
            //:IF szPermissionsSource = ""
            if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
            { 
               //:szPermissionsSource = "Work"
               ZeidonStringCopy( szPermissionsSource, 1, 0, "Work", 1, 0, 51 );
               //:ELSE
            } 
            else
            { 
               //:szPermissionsSource = szPermissionsSource + "/Work"
               ZeidonStringConcat( szPermissionsSource, 1, 0, "/Work", 1, 0, 51 );
            } 

            //:END
         } 

         //:END
         //:IF SourceLOD.LOD_AttributeRec.Update  = "Y"
         if ( CompareAttributeToString( SourceLOD, "LOD_AttributeRec", "Update", "Y" ) == 0 )
         { 
            //:IF szPermissionsSource = ""
            if ( ZeidonStringCompare( szPermissionsSource, 1, 0, "", 1, 0, 51 ) == 0 )
            { 
               //:szPermissionsSource = "Update"
               ZeidonStringCopy( szPermissionsSource, 1, 0, "Update", 1, 0, 51 );
               //:ELSE
            } 
            else
            { 
               //:szPermissionsSource = szPermissionsSource + "/Update"
               ZeidonStringConcat( szPermissionsSource, 1, 0, "/Update", 1, 0, 51 );
            } 

            //:END
         } 

         //:END
         //:// Compare Permissions
         //:IF szPermissionsTarget != szPermissionsSource
         if ( ZeidonStringCompare( szPermissionsTarget, 1, 0, szPermissionsSource, 1, 0, 51 ) != 0 )
         { 
            //:TargetLOD_Root.CompareAttributeResult.DifferenceFlag = "D"
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "DifferenceFlag", "D" );
            //:TargetLOD_Root.CompareAttributeResult.SourceDifferentPermissions = szPermissionsSource
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "SourceDifferentPermissions", szPermissionsSource );
            //:TargetLOD_Root.CompareAttributeResult.TargetDifferentPermissions = szPermissionsTarget
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "TargetDifferentPermissions", szPermissionsTarget );
         } 

         //:END

         //:// Check Domain
         //:IF TargetLOD.DomainRec.Name != SourceLOD.DomainRec.Name
         if ( CompareAttributeToAttribute( TargetLOD, "DomainRec", "Name", SourceLOD, "DomainRec", "Name" ) != 0 )
         { 
            //:TargetLOD_Root.CompareAttributeResult.DifferenceFlag = "D"
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "DifferenceFlag", "D" );
            //:TargetLOD_Root.CompareAttributeResult.SourceDifferentDomainName = SourceLOD.DomainRec.Name
            SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "SourceDifferentDomainName", SourceLOD, "DomainRec", "Name" );
            //:TargetLOD_Root.CompareAttributeResult.TargetDifferentDomainName = TargetLOD.DomainRec.Name
            SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "TargetDifferentDomainName", TargetLOD, "DomainRec", "Name" );
         } 

         //:END

         //:// Check String Length
         //:IF TargetLOD.ER_AttributeRec.Lth != SourceLOD.ER_AttributeRec.Lth
         if ( CompareAttributeToAttribute( TargetLOD, "ER_AttributeRec", "Lth", SourceLOD, "ER_AttributeRec", "Lth" ) != 0 )
         { 
            //:TargetLOD_Root.CompareAttributeResult.DifferenceFlag = "D"
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "DifferenceFlag", "D" );
            //:TargetLOD_Root.CompareAttributeResult.SourceDifferentStringLength = SourceLOD.ER_AttributeRec.Lth
            SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "SourceDifferentStringLength", SourceLOD, "ER_AttributeRec", "Lth" );
            //:TargetLOD_Root.CompareAttributeResult.TargetDifferentStringLength = TargetLOD.ER_AttributeRec.Lth
            SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "TargetDifferentStringLength", TargetLOD, "ER_AttributeRec", "Lth" );
         } 

         //:END

         //:// Check Operation
         //:// Target
         //:IF TargetLOD.LOD_AttrDerivationOperRec EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TargetLOD, "LOD_AttrDerivationOperRec" );
         if ( lTempInteger_0 == 0 )
         { 
            //:szOperationTarget = TargetLOD.LOD_AttrDerivationOperRec.Name
            GetVariableFromAttribute( szOperationTarget, 0, 'S', 51, TargetLOD, "LOD_AttrDerivationOperRec", "Name", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szOperationTarget = ""
            ZeidonStringCopy( szOperationTarget, 1, 0, "", 1, 0, 51 );
         } 

         //:END
         //:// Source
         //:IF SourceLOD.LOD_AttrDerivationOperRec EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( SourceLOD, "LOD_AttrDerivationOperRec" );
         if ( lTempInteger_1 == 0 )
         { 
            //:szOperationSource = SourceLOD.LOD_AttrDerivationOperRec.Name
            GetVariableFromAttribute( szOperationSource, 0, 'S', 51, SourceLOD, "LOD_AttrDerivationOperRec", "Name", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szOperationSource = ""
            ZeidonStringCopy( szOperationSource, 1, 0, "", 1, 0, 51 );
         } 

         //:END
         //:// Compare Operations
         //:IF szOperationTarget != szOperationSource
         if ( ZeidonStringCompare( szOperationTarget, 1, 0, szOperationSource, 1, 0, 51 ) != 0 )
         { 
            //:TargetLOD_Root.CompareAttributeResult.DifferenceFlag = "D"
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "DifferenceFlag", "D" );
            //:TargetLOD_Root.CompareAttributeResult.SourceDifferentOperationName = szOperationSource
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "SourceDifferentOperationName", szOperationSource );
            //:TargetLOD_Root.CompareAttributeResult.TargetDifferentOperationName = szOperationTarget
            SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "TargetDifferentOperationName", szOperationTarget );
         } 

         //:END

         //:ELSE
      } 
      else
      { 
         //:// Source is not found in Target.
         //:TargetLOD_Root.CompareAttributeResult.DifferenceFlag = "S"
         SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "DifferenceFlag", "S" );
      } 

      RESULT = SetCursorNextEntity( SourceLOD, "LOD_AttributeRec", "" );
      //:END
   } 

   //:END

   //:// Next try to match the Target Attributes to the Source Attributes.
   //:FOR EACH TargetLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( TargetLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:SET CURSOR FIRST SourceLOD.LOD_AttributeRec WHERE SourceLOD.ER_AttributeRec.Name = TargetLOD.ER_AttributeRec.Name
      RESULT = SetCursorFirstEntity( SourceLOD, "LOD_AttributeRec", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( SourceLOD, "ER_AttributeRec", "Name", TargetLOD, "ER_AttributeRec", "Name" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( SourceLOD, "LOD_AttributeRec", "" );
         } 

      } 

      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// Target Attribute is not found in Source.
         //:// This is the only case we  care about in this pass.
         //:CREATE ENTITY TargetLOD_Root.CompareAttributeResult
         RESULT = CreateEntity( TargetLOD_Root, "CompareAttributeResult", zPOS_AFTER );
         //:TargetLOD_Root.CompareAttributeResult.AttributeName = TargetLOD.ER_AttributeRec.Name
         SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "AttributeName", TargetLOD, "ER_AttributeRec", "Name" );
         //:TargetLOD_Root.CompareAttributeResult.AttributeID   = TargetLOD.LOD_AttributeRec.ZKey
         SetAttributeFromAttribute( TargetLOD_Root, "CompareAttributeResult", "AttributeID", TargetLOD, "LOD_AttributeRec", "ZKey" );
         //:TargetLOD_Root.CompareAttributeResult.DifferenceFlag = "T"
         SetAttributeFromString( TargetLOD_Root, "CompareAttributeResult", "DifferenceFlag", "T" );
      } 

      RESULT = SetCursorNextEntity( TargetLOD, "LOD_AttributeRec", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:LOD_Merge( VIEW TargetLOD BASED ON LOD TZZOLODO,
//:           VIEW SourceLOD BASED ON LOD TZZOLODO,
//:           VIEW vSubtask )

//:   VIEW TargetLOD_Root BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_LOD_Merge( zVIEW     TargetLOD,
                     zVIEW     SourceLOD,
                     zVIEW     vSubtask )
{
   zVIEW     TargetLOD_Root = 0; 
   //:VIEW SourceLOD_Root BASED ON LOD TZZOLODO
   zVIEW     SourceLOD_Root = 0; 
   //:VIEW CurrentLPLR    BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 
   //:VIEW SourceLPLR     BASED ON LOD TZCMLPLO
   zVIEW     SourceLPLR = 0; 
   //:VIEW TempLPLR       BASED ON LOD TZCMLPLO
   zVIEW     TempLPLR = 0; 
   //:VIEW NewERD         BASED ON LOD TZEREMDO
   zVIEW     NewERD = 0; 
   //:VIEW SourceERD      BASED ON LOD TZEREMDO
   zVIEW     SourceERD = 0; 
   //:STRING ( 1 )   szSecondLineFlag
   zCHAR     szSecondLineFlag[ 2 ] = { 0 }; 
   //:STRING ( 200 ) szFileName
   zCHAR     szFileName[ 201 ] = { 0 }; 
   //:INTEGER        AttributeCount
   zLONG     AttributeCount = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 


   //:// Merge the SourceLOD into the TargetLOD based on the selection specified in the
   //:// CompareEntityResult and CompareOperationResult subobjects in TargetLOD.

   //:// If an LPLR Name has been specified, activate the XLP and pass the File Directory structure to any
   //:// sub-operations so that necessary Domains can be imported.
   //:// Also activate the Source ERD.
   //:GET VIEW CurrentLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", TargetLOD, zLEVEL_TASK );
   //:IF CurrentLPLR.LPLR.wMergeSourceLPLR_Name != ""
   if ( CompareAttributeToString( CurrentLPLR, "LPLR", "wMergeSourceLPLR_Name", "" ) != 0 )
   { 
      //:szFileName = CurrentLPLR.LPLR.wFullyQualifiedFileName + "\bin\" +
      //:             CurrentLPLR.LPLR.wMergeSourceLPLR_Name + ".XLP"
      GetStringFromAttribute( szFileName, sizeof( szFileName ), CurrentLPLR, "LPLR", "wFullyQualifiedFileName" );
      ZeidonStringConcat( szFileName, 1, 0, "\\bin\\", 1, 0, 201 );
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, CurrentLPLR, "LPLR", "wMergeSourceLPLR_Name", "", 0 );
      ZeidonStringConcat( szFileName, 1, 0, szTempString_0, 1, 0, 201 );
      ZeidonStringConcat( szFileName, 1, 0, ".XLP", 1, 0, 201 );
      //:nRC = ActivateOI_FromFile( SourceLPLR, "TZCMLPLO", TargetLOD, szFileName, zSINGLE )
      nRC = ActivateOI_FromFile( &SourceLPLR, "TZCMLPLO", TargetLOD, szFileName, zSINGLE );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:NAME VIEW SourceLPLR "SourceLPLR"
         SetNameForView( SourceLPLR, "SourceLPLR", 0, zLEVEL_TASK );

         //:// Activate Source ERD.
         //:szFileName = CurrentLPLR.LPLR.wFullyQualifiedFileName + "\" +
         //:             CurrentLPLR.LPLR.wMergeSourceLPLR_Name + ".PMD"
         GetStringFromAttribute( szFileName, sizeof( szFileName ), CurrentLPLR, "LPLR", "wFullyQualifiedFileName" );
         ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 201 );
         GetVariableFromAttribute( szTempString_1, 0, 'S', 255, CurrentLPLR, "LPLR", "wMergeSourceLPLR_Name", "", 0 );
         ZeidonStringConcat( szFileName, 1, 0, szTempString_1, 1, 0, 201 );
         ZeidonStringConcat( szFileName, 1, 0, ".PMD", 1, 0, 201 );
         //:nRC = ActivateOI_FromFile( SourceERD, "TZEREMDO", TargetLOD, szFileName, zSINGLE )
         nRC = ActivateOI_FromFile( &SourceERD, "TZEREMDO", TargetLOD, szFileName, zSINGLE );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:NAME VIEW SourceERD "SourceERD"
            SetNameForView( SourceERD, "SourceERD", 0, zLEVEL_TASK );
            //:ELSE
         } 
         else
         { 
            //:SourceERD = 0
            SourceERD = 0;
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:MessageSend( TargetLOD, "", "LOD Merge",
         //:             "The LPLR Name specified does not have an XLP file. No Source ERD will be used.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TargetLOD, "", "LOD Merge", "The LPLR Name specified does not have an XLP file. No Source ERD will be used.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SourceLPLR = 0
         SourceLPLR = 0;
         //:SourceERD  = 0
         SourceERD = 0;
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:MessageSend( TargetLOD, "", "LOD Merge",
      //:             "Note that NO LPLR Name was specified, so NO Source ERD will be used during the merge.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TargetLOD, "", "LOD Merge", "Note that NO LPLR Name was specified, so NO Source ERD will be used during the merge.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SourceLPLR = 0
      SourceLPLR = 0;
      //:SourceERD  = 0
      SourceERD = 0;
   } 

   //:END

   //:// Set up views needed for Merge and Copy processing.
   //:CreateViewFromView( TargetLOD_Root, TargetLOD )
   CreateViewFromView( &TargetLOD_Root, TargetLOD );
   //:NAME VIEW TargetLOD_Root "TargetLOD_Root"
   SetNameForView( TargetLOD_Root, "TargetLOD_Root", 0, zLEVEL_TASK );
   //:CreateViewFromView( SourceLOD_Root, SourceLOD )
   CreateViewFromView( &SourceLOD_Root, SourceLOD );
   //:NAME VIEW SourceLOD_Root "SourceLOD_Root"
   SetNameForView( SourceLOD_Root, "SourceLOD_Root", 0, zLEVEL_TASK );
   //:GET VIEW NewERD NAMED "NewERD"
   RESULT = GetViewByName( &NewERD, "NewERD", TargetLOD, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:GET VIEW NewERD NAMED "TZEREMDO_REF"
      RESULT = GetViewByName( &NewERD, "TZEREMDO_REF", TargetLOD, zLEVEL_TASK );
      //:NAME VIEW NewERD "NewERD"
      SetNameForView( NewERD, "NewERD", 0, zLEVEL_TASK );
   } 

   //:END

   //:// Initialize flags prior to merge processing.
   //:CurrentLPLR.LPLR.wMergeComponentError     = ""
   SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeComponentError", "" );
   //:CurrentLPLR.LPLR.wModifiedER_NotSavedFlag = ""
   SetAttributeFromString( CurrentLPLR, "LPLR", "wModifiedER_NotSavedFlag", "" );
   //:CurrentLPLR.LPLR.wMergeER_ModifiedFlag    = ""
   SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "" );

   //:// Go to merge LOD entities.
   //:LOD_MergeEntity( TargetLOD,
   //:                 SourceLOD,
   //:                 TargetLOD_Root,
   //:                 SourceLOD_Root,
   //:                 SourceLPLR,
   //:                 CurrentLPLR,
   //:                 NewERD,
   //:                 SourceERD,
   //:                 vSubtask )
   oTZZOLODO_LOD_MergeEntity( TargetLOD, SourceLOD, TargetLOD_Root, SourceLOD_Root, SourceLPLR, CurrentLPLR, NewERD, SourceERD, vSubtask );

   //:DropView( TargetLOD_Root )
   DropView( TargetLOD_Root );

   //:// If the ER has been modified, prompt the User to see if it should be saved. Don't do this if
   //:// component errors have occurred.
   //:IF CurrentLPLR.LPLR.wMergeER_ModifiedFlag = "Y" AND CurrentLPLR.LPLR.wMergeComponentError != "Y"
   if ( CompareAttributeToString( CurrentLPLR, "LPLR", "wMergeER_ModifiedFlag", "Y" ) == 0 && CompareAttributeToString( CurrentLPLR, "LPLR", "wMergeComponentError", "Y" ) != 0 )
   { 
      //:nRC = MessagePrompt( vSubtask, "", "LOD Merge",
      //:                     "The ER was modified in the process of merging the LOD's. Do you want to save the ER?",
      //:                     0, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION )
      nRC = MessagePrompt( vSubtask, "", "LOD Merge", "The ER was modified in the process of merging the LOD's. Do you want to save the ER?", 0, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION );
      //:IF nRC = zRESPONSE_YES
      if ( nRC == zRESPONSE_YES )
      { 
         //:// Commit the ER.
         //:// We will not use CommitMetaOI because it requires getting an updatable version of the ER, which creates
         //:// all sorts of meta problems because we already have a LOD which is referencing a "referencable" version
         //:// of the ER. We thus have made our changes to the "referencable" version (GET VIEW NewERD NAMED "TZEREMDO_REF" above)
         //:// and will commit it here.
         //:szFileName = CurrentLPLR.LPLR.MetaSrcDir + "\" +
         //:             CurrentLPLR.LPLR.Name + ".PMD"
         GetStringFromAttribute( szFileName, sizeof( szFileName ), CurrentLPLR, "LPLR", "MetaSrcDir" );
         ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 201 );
         GetVariableFromAttribute( szTempString_2, 0, 'S', 33, CurrentLPLR, "LPLR", "Name", "", 0 );
         ZeidonStringConcat( szFileName, 1, 0, szTempString_2, 1, 0, 201 );
         ZeidonStringConcat( szFileName, 1, 0, ".PMD", 1, 0, 201 );
         //:nRC = CommitOI_ToFile( NewERD, szFileName, zASCII )
         nRC = CommitOI_ToFile( NewERD, szFileName, zASCII );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MessageSend( vSubtask, "", "LOD Merge",
            //:             "An error occurred during the save of the ER. Merging the LOD will be aborted.",
            //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "", "LOD Merge", "An error occurred during the save of the ER. Merging the LOD will be aborted.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:CurrentLPLR.LPLR.wMergeComponentError     = ""
            SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeComponentError", "" );
            //:RETURN -1
            return( -1 );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// Since changes were made to the ER that were not saved, mark the LOD so that it can NOT be saved,
         //:// because saving a LOD with ER changes without saving the ER will create inconsistencies in the LPLR.
         //:CurrentLPLR.LPLR.wModifiedER_NotSavedFlag = "Y"
         SetAttributeFromString( CurrentLPLR, "LPLR", "wModifiedER_NotSavedFlag", "Y" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:LOD_MergeEntity( VIEW TargetLOD      BASED ON LOD TZZOLODO,
//:                 VIEW SourceLOD      BASED ON LOD TZZOLODO,
//:                 VIEW TargetLOD_Root BASED ON LOD TZZOLODO,
//:                 VIEW SourceLOD_Root BASED ON LOD TZZOLODO,
//:                 VIEW SourceLPLR     BASED ON LOD TZCMLPLO,
//:                 VIEW CurrentLPLR    BASED ON LOD TZCMLPLO,
//:                 VIEW NewERD         BASED ON LOD TZEREMDO,
//:                 VIEW SourceERD      BASED ON LOD TZEREMDO,
//:                 VIEW vSubtask )

//:   VIEW TargetLOD2 BASED ON LOD TZZOLODO
static zSHORT
oTZZOLODO_LOD_MergeEntity( zVIEW     TargetLOD,
                           zVIEW     SourceLOD,
                           zVIEW     TargetLOD_Root,
                           zVIEW     SourceLOD_Root,
                           zVIEW     SourceLPLR,
                           zVIEW     CurrentLPLR,
                           zVIEW     NewERD,
                           zVIEW     SourceERD,
                           zVIEW     vSubtask )
{
   zVIEW     TargetLOD2 = 0; 
   //:STRING ( 1 )  szER_EntityFound
   zCHAR     szER_EntityFound[ 2 ] = { 0 }; 
   //:STRING ( 1 )  szAddAttributeFlag
   zCHAR     szAddAttributeFlag[ 2 ] = { 0 }; 
   //:STRING ( 32 ) ParentName
   zCHAR     ParentName[ 33 ] = { 0 }; 
   //:SHORT         nRC
   zSHORT    nRC = 0; 
   //:INTEGER       Count
   zLONG     Count = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 


   //:// Loop through each Source LOD Entity, checking to see if there is a corresponding LOD Entity in
   //:// the Target LOD and if the User has requested that it be merged as follows.
   //:// 1. If the LOD Entity does NOT exist in the Target, copy it and all Attributes and LOD Entity subobjects
   //://    to the Target.
   //:// 2, If the LOD Entity DOES exist in the Target, set the all attributes from the Source into the Target.
   //://    We will not do any further processing of attributes and LOD Entity subobjects, unless those entries have
   //://    been selected for merge processing.

   //:// Check if Source LOD Entity is in Target.
   //:// If the Source Entity is a work entity (ie., not tied to an ER_Entity), look for a match on name.
   //:// If the Source Entity is an ER Entity, find a match on ER Entity Name.
   //:szER_EntityFound = ""
   ZeidonStringCopy( szER_EntityFound, 1, 0, "", 1, 0, 2 );
   //:IF SourceLOD.ER_EntityRec EXISTS AND SourceLOD.ER_RelLinkRec EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( SourceLOD, "ER_EntityRec" );
   lTempInteger_1 = CheckExistenceOfEntity( SourceLOD, "ER_RelLinkRec" );
   if ( lTempInteger_0 == 0 && lTempInteger_1 == 0 )
   { 
      //:// This is an ER Entity. We can't do a simple set cursor so we'll loop through entries for a match.
      //:SET CURSOR FIRST TargetLOD.LOD_EntityParent
      RESULT = SetCursorFirstEntity( TargetLOD, "LOD_EntityParent", "" );
      //:LOOP WHILE RESULT >= zCURSOR_SET AND szER_EntityFound = ""
      while ( RESULT >= zCURSOR_SET && ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:// Compare ER_Entities.
         //:IF TargetLOD.ER_EntityRec EXISTS AND TargetLOD.ER_RelLinkRec EXISTS
         lTempInteger_2 = CheckExistenceOfEntity( TargetLOD, "ER_EntityRec" );
         lTempInteger_3 = CheckExistenceOfEntity( TargetLOD, "ER_RelLinkRec" );
         if ( lTempInteger_2 == 0 && lTempInteger_3 == 0 )
         { 
            //:IF TargetLOD.ER_EntityRec.Name  = SourceLOD.ER_EntityRec.Name AND
            //:   TargetLOD.ER_RelLinkRec.Name = SourceLOD.ER_RelLinkRec.Name
            if ( CompareAttributeToAttribute( TargetLOD, "ER_EntityRec", "Name", SourceLOD, "ER_EntityRec", "Name" ) == 0 && CompareAttributeToAttribute( TargetLOD, "ER_RelLinkRec", "Name", SourceLOD, "ER_RelLinkRec", "Name" ) == 0 )
            { 

               //:szER_EntityFound = "Y"
               ZeidonStringCopy( szER_EntityFound, 1, 0, "Y", 1, 0, 2 );
            } 

            //:END
         } 

         //:END
         //:// Set next cursor if we haven't found a match.
         //:IF szER_EntityFound = ""
         if ( ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //:SET CURSOR NEXT TargetLOD.LOD_EntityParent
            RESULT = SetCursorNextEntity( TargetLOD, "LOD_EntityParent", "" );
         } 

         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// This is a work entity or the root of the LOD, so just do set cursor on LOD Entity Name.
      //:SET CURSOR FIRST TargetLOD.LOD_EntityParent
      //:           WHERE TargetLOD.LOD_EntityParent.Name = SourceLOD.LOD_EntityParent.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), SourceLOD, "LOD_EntityParent", "Name" );
      RESULT = SetCursorFirstEntityByString( TargetLOD, "LOD_EntityParent", "Name", szTempString_0, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szER_EntityFound = "Y"
         ZeidonStringCopy( szER_EntityFound, 1, 0, "Y", 1, 0, 2 );
      } 

      //:END
   } 

   //:END

   //:// Check if request is to merge.
   //:SET CURSOR FIRST TargetLOD_Root.DisplayCompareResult
   //:           WHERE TargetLOD_Root.DisplayCompareResult.MetaID       = SourceLOD.LOD_EntityParent.ZKey
   //:             AND TargetLOD_Root.DisplayCompareResult.SelectedFlag = "Y"
   RESULT = SetCursorFirstEntity( TargetLOD_Root, "DisplayCompareResult", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TargetLOD_Root, "DisplayCompareResult", "MetaID", SourceLOD, "LOD_EntityParent", "ZKey" ) != 0 ||
              CompareAttributeToString( TargetLOD_Root, "DisplayCompareResult", "SelectedFlag", "Y" ) != 0 ) )
      { 
         RESULT = SetCursorNextEntity( TargetLOD_Root, "DisplayCompareResult", "" );
      } 

   } 

   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Request is to merge Entity.
      //:IF szER_EntityFound = ""
      if ( ZeidonStringCompare( szER_EntityFound, 1, 0, "", 1, 0, 2 ) == 0 )
      { 

         //:// Case 1: The LOD Entity does NOT exist in the Target. Copy entity and subobjects.

         //:// Get the Parent Name of the entity, if there is on
         //:CreateViewFromView( TargetLOD2, TargetLOD )
         CreateViewFromView( &TargetLOD2, TargetLOD );
         //:nRC = ResetViewFromSubobject( TargetLOD2 )
         nRC = ResetViewFromSubobject( TargetLOD2 );
         //:IF nRC = 0
         if ( nRC == 0 )
         { 
            //:IF TargetLOD2.ER_EntityRec EXISTS
            lTempInteger_4 = CheckExistenceOfEntity( TargetLOD2, "ER_EntityRec" );
            if ( lTempInteger_4 == 0 )
            { 
               //:ParentName = TargetLOD2.ER_EntityRec.Name    // Parent Name is used in LOD_EntityCopy.
               GetVariableFromAttribute( ParentName, 0, 'S', 33, TargetLOD2, "ER_EntityRec", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:ParentName = ""
               ZeidonStringCopy( ParentName, 1, 0, "", 1, 0, 33 );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:ParentName = ""
            ZeidonStringCopy( ParentName, 1, 0, "", 1, 0, 33 );
         } 

         //:END
         //:DropView( TargetLOD2 )
         DropView( TargetLOD2 );

         //:LOD_EntityCopy( TargetLOD_Root,
         //:                TargetLOD,
         //:                SourceLOD_Root,
         //:                SourceLOD,
         //:                SourceLPLR,
         //:                CurrentLPLR,
         //:                NewERD,
         //:                SourceERD,
         //:                0,                // There is no target DTE for Merge.
         //:                vSubtask,
         //:                ParentName,
         //:                "Y" )             // The request is to add ER entities and relationships as necessary.
         oTZZOLODO_LOD_EntityCopy( TargetLOD_Root, TargetLOD, SourceLOD_Root, SourceLOD, SourceLPLR, CurrentLPLR, NewERD, SourceERD, 0, vSubtask, ParentName, "Y" );
         //:RETURN     // After a copy, we're done, so we'll just go back to process next entity.
         return( 0 );
         //:ELSE
      } 
      else
      { 

         //:// Case 2: The LOD Entity DOES exist in the Target. Set LOD Entity values.

         //:SetMatchingAttributesByName( TargetLOD, "LOD_EntityParent",
         //:                             SourceLOD, "LOD_EntityParent", zSET_NOTNULL )
         SetMatchingAttributesByName( TargetLOD, "LOD_EntityParent", SourceLOD, "LOD_EntityParent", zSET_NOTNULL );
      } 

      //:END
   } 

   //:END

   //:// Whether or not we merged the Entity, continue to process Attributes and LOD subobjects.

   //:// Position on ER Entity for the LOD_Attribute about to be processed.
   //:IF TargetLOD.ER_EntityRec EXISTS
   lTempInteger_5 = CheckExistenceOfEntity( TargetLOD, "ER_EntityRec" );
   if ( lTempInteger_5 == 0 )
   { 
      //:SET CURSOR FIRST NewERD.ER_Entity WHERE NewERD.ER_Entity.ZKey = TargetLOD.ER_EntityRec.ZKey
      GetIntegerFromAttribute( &lTempInteger_6, TargetLOD, "ER_EntityRec", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( NewERD, "ER_Entity", "ZKey", lTempInteger_6, "" );
   } 

   //:END

   //:szAddAttributeFlag = ""
   ZeidonStringCopy( szAddAttributeFlag, 1, 0, "", 1, 0, 2 );
   //:FOR EACH SourceLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( SourceLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Merge Attribute as requested.
      //:SET CURSOR FIRST TargetLOD_Root.DisplayCompareResult
      //:           WHERE TargetLOD_Root.DisplayCompareResult.MetaID       = SourceLOD.LOD_AttributeRec.ZKey
      //:             AND TargetLOD_Root.DisplayCompareResult.SelectedFlag = "Y"
      RESULT = SetCursorFirstEntity( TargetLOD_Root, "DisplayCompareResult", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TargetLOD_Root, "DisplayCompareResult", "MetaID", SourceLOD, "LOD_AttributeRec", "ZKey" ) != 0 ||
                 CompareAttributeToString( TargetLOD_Root, "DisplayCompareResult", "SelectedFlag", "Y" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( TargetLOD_Root, "DisplayCompareResult", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST TargetLOD.ER_AttributeRec WITHIN TargetLOD.LOD_EntityParent
         //:           WHERE TargetLOD.ER_AttributeRec.Name = SourceLOD.ER_AttributeRec.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), SourceLOD, "ER_AttributeRec", "Name" );
         RESULT = SetCursorFirstEntityByString( TargetLOD, "ER_AttributeRec", "Name", szTempString_0, "LOD_EntityParent" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:// The attribute exist in the Target. We are going to delete it and re-add it.
            //:DELETE ENTITY TargetLOD.LOD_AttributeRec NONE
            RESULT = DeleteEntity( TargetLOD, "LOD_AttributeRec", zREPOS_NONE );
         } 

         //:END
         //:SET CURSOR LAST TargetLOD.LOD_AttributeRec
         RESULT = SetCursorLastEntity( TargetLOD, "LOD_AttributeRec", "" );
         //:szAddAttributeFlag = "Y"
         ZeidonStringCopy( szAddAttributeFlag, 1, 0, "Y", 1, 0, 2 );

         //:// Go to copy the Source Attribute.
         //:LOD_AttributeCopy( vSubtask,
         //:                   TargetLOD_Root,
         //:                   TargetLOD,
         //:                   SourceLOD_Root,
         //:                   SourceLOD,
         //:                   SourceLPLR,
         //:                   CurrentLPLR,
         //:                   NewERD,
         //:                   "Y" )
         oTZZOLODO_LOD_AttributeCopy( vSubtask, TargetLOD_Root, TargetLOD, SourceLOD_Root, SourceLOD, SourceLPLR, CurrentLPLR, NewERD, "Y" );
      } 

      RESULT = SetCursorNextEntity( SourceLOD, "LOD_AttributeRec", "" );
      //:END
   } 

   //:END

   //:// If we added an attribute, reset sequence numbers.
   //:IF szAddAttributeFlag = "Y"
   if ( ZeidonStringCompare( szAddAttributeFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:Count = 1
      Count = 1;
      //:SET CURSOR FIRST TargetLOD_Root.LOD_Entity WHERE TargetLOD_Root.LOD_Entity.ZKey = TargetLOD.LOD_EntityParent.ZKey
      GetIntegerFromAttribute( &lTempInteger_7, TargetLOD, "LOD_EntityParent", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( TargetLOD_Root, "LOD_Entity", "ZKey", lTempInteger_7, "" );
      //:FOR EACH TargetLOD_Root.LOD_Attribute
      RESULT = SetCursorFirstEntity( TargetLOD_Root, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:TargetLOD_Root.LOD_Attribute.wkSequence = Count
         SetAttributeFromInteger( TargetLOD_Root, "LOD_Attribute", "wkSequence", Count );
         //:Count = Count + 1
         Count = Count + 1;
         RESULT = SetCursorNextEntity( TargetLOD_Root, "LOD_Attribute", "" );
      } 

      //:END
   } 

   //:END

   //:// Process any LOD Entity subobjects recursively.
   //:SetViewToSubobject( SourceLOD, "LOD_EntityChild" )
   SetViewToSubobject( SourceLOD, "LOD_EntityChild" );
   //:SetViewToSubobject( TargetLOD, "LOD_EntityChild" )
   SetViewToSubobject( TargetLOD, "LOD_EntityChild" );
   //:FOR EACH SourceLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( SourceLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Process merge for LOD Entity.
      //:LOD_MergeEntity( TargetLOD,
      //:                 SourceLOD,
      //:                 TargetLOD_Root,
      //:                 SourceLOD_Root,
      //:                 SourceLPLR,
      //:                 CurrentLPLR,
      //:                 NewERD,
      //:                 SourceERD,
      //:                 vSubtask )
      oTZZOLODO_LOD_MergeEntity( TargetLOD, SourceLOD, TargetLOD_Root, SourceLOD_Root, SourceLPLR, CurrentLPLR, NewERD, SourceERD, vSubtask );
      RESULT = SetCursorNextEntity( SourceLOD, "LOD_EntityParent", "" );
   } 


   //:END
   //:ResetViewFromSubobject( SourceLOD )
   ResetViewFromSubobject( SourceLOD );
   //:ResetViewFromSubobject( TargetLOD )
   ResetViewFromSubobject( TargetLOD );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
