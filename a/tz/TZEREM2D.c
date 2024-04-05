#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zSHORT zwTZEREMDD_InitMenuForEditItems( zVIEW, zSHORT, zSHORT );
zSHORT zwTZEREMDD_InitMenuForDeleteAll( zVIEW, zVIEW, zSHORT );
zSHORT zwTZEREMDD_InitMenuUtilities( zVIEW );
zSHORT zwTZEREMDD_DisableDeleteActions( zVIEW*, zSHORT );
zSHORT zwTZEREMDD_ADW_Set_Dir_Prefix( zVIEW, zPCHAR, zLONG );
zSHORT zwTZEREMDD_DeselectRelationship( zVIEW, zPCHAR, zSHORT );
zSHORT zwfnTZEREMDD_RefreshEntitySelect( zVIEW, zSHORT );
zSHORT zwTZEREMDD_SetSelectionInSAList( zVIEW, zVIEW );
zSHORT zwTZEREMDD_InitMenuForEdit_RS( zVIEW, zVIEW, zSHORT );
zSHORT zwTZEREMDD_InitMenuForEdit_ID( zVIEW, zVIEW, zSHORT, zSHORT );
zSHORT zwTZEREMDD_IsRelLinkToSapEntity( zVIEW, zVIEW, zVIEW );
zSHORT zwTZEREMDD_AddRelLinkToSA( zVIEW, zVIEW );
zSHORT zwTZEREMDD_WriteErrorMsg( zVIEW, zVIEW, zPCHAR, zSHORT );

zOPER_EXPORT zSHORT OPERATION
GOTO_TieRelationshipToTE_Field( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CANCEL_TieFieldsToRelationship( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
EXEC_TieFieldsToRelationship( zVIEW     vSubtask );


//:DIALOG OPERATION
//:GOTO_TieRelationshipToTE_Field( VIEW vSubtask )

//:   VIEW vEMD      REGISTERED AS TZEREMDO 
zOPER_EXPORT zSHORT OPERATION
GOTO_TieRelationshipToTE_Field( zVIEW     vSubtask )
{
   zVIEW     vEMD = 0; 
   zSHORT    RESULT; 
   //:VIEW vDTE      BASED ON LOD  TZTENVRO
   zVIEW     vDTE = 0; 
   //:VIEW vLPLR     BASED ON LOD  TZCMLPLO
   zVIEW     vLPLR = 0; 
   //:VIEW vTaskLPLR REGISTERED AS TaskLPLR
   zVIEW     vTaskLPLR = 0; 
   //:STRING ( 2 )  szMaxCard
   zCHAR     szMaxCard[ 3 ] = { 0 }; 
   //:STRING ( 50 ) szTargetEntity
   zCHAR     szTargetEntity[ 51 ] = { 0 }; 
   //:STRING ( 90 ) szMsg
   zCHAR     szMsg[ 91 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER n1Count
   zLONG     n1Count = 0; 
   //:INTEGER nmCount
   zLONG     nmCount = 0; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 

   RESULT = GetViewByName( &vEMD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:// Make sure that we can activate the DTE for update.
   //:CreateViewFromView( vLPLR, vTaskLPLR )
   CreateViewFromView( &vLPLR, vTaskLPLR );
   //:SET CURSOR FIRST vLPLR.W_MetaType WHERE vLPLR.W_MetaType.Type = 2006 
   RESULT = SetCursorFirstEntityByInteger( vLPLR, "W_MetaType", "Type", 2006, "" );
   //:nRC = ActivateMetaOI( vSubtask, vDTE, vLPLR, zSOURCE_DTE_META, 0 )
   nRC = ActivateMetaOI( vSubtask, &vDTE, vLPLR, zSOURCE_DTE_META, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask, "", "Tie Relationship to Field",
      //:             "The DTE is already open for Update.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Tie Relationship to Field", "The DTE is already open for Update.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW vDTE "TE_DB_Environ"
   SetNameForView( vDTE, "TE_DB_Environ", 0, zLEVEL_TASK );

   //:// Make sure that the relationship is 1-to-m or 0-to-m by checking that there is one side with
   //:// Max Card 1 and the other with Max Card m.

   //:nmCount = 0
   nmCount = 0;
   //:n1Count = 0
   n1Count = 0;
   //:FOR EACH vEMD.ER_RelLink_2 
   RESULT = SetCursorFirstEntity( vEMD, "ER_RelLink_2", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szMaxCard = vEMD.ER_RelLink_2.CardMax
      GetVariableFromAttribute( szMaxCard, 0, 'S', 3, vEMD, "ER_RelLink_2", "CardMax", "", 0 );
      //:IF szMaxCard = "m"
      if ( ZeidonStringCompare( szMaxCard, 1, 0, "m", 1, 0, 3 ) == 0 )
      { 
         //:nmCount = nmCount + 1
         nmCount = nmCount + 1;
         //:ELSE
      } 
      else
      { 
         //:n1Count = n1Count + 1
         n1Count = n1Count + 1;
      } 

      RESULT = SetCursorNextEntity( vEMD, "ER_RelLink_2", "" );
      //:END
   } 

   //:END
   //:IF n1Count != 1 OR nmCount != 1
   if ( n1Count != 1 || nmCount != 1 )
   { 
      //:MessageSend( vSubtask, "", "Tie Relationship to Field",
      //:             "The Relationship cardinality must be 1-to-m or 0-to-m.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Tie Relationship to Field", "The Relationship cardinality must be 1-to-m or 0-to-m.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure neither side of the Relationship is a work Entity.
   //:SET CURSOR FIRST vEMD.ER_Entity_2 WITHIN vEMD.ER_RelType WHERE vEMD.ER_Entity_2.Purpose = "W"
   RESULT = SetCursorFirstEntityByString( vEMD, "ER_Entity_2", "Purpose", "W", "ER_RelType" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:MessageSend( vSubtask, "", "Tie Relationship to Field",
      //:             "Neither Entity within the Relationship can be a Work Entity.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Tie Relationship to Field", "Neither Entity within the Relationship can be a Work Entity.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Position on the side the of Relationship with Max Card of "1" to get Entity/Table with the necessary foreign key.
   //:SET CURSOR FIRST vEMD.ER_RelLink_2 WHERE vEMD.ER_RelLink_2.CardMax = "1" 
   RESULT = SetCursorFirstEntityByString( vEMD, "ER_RelLink_2", "CardMax", "1", "" );

   //:// Position on corresponding ER_Entity and sub relationship.
   //:SetCursorFirstEntityByEntityCsr( vEMD, "ER_Entity",
   //:                                 vEMD, "ER_Entity_2", "" )
   SetCursorFirstEntityByEntityCsr( vEMD, "ER_Entity", vEMD, "ER_Entity_2", "" );
   //:SetCursorFirstEntityByEntityCsr( vEMD, "ER_RelLink",
   //:                                 vEMD, "ER_RelLink_2", "" )
   SetCursorFirstEntityByEntityCsr( vEMD, "ER_RelLink", vEMD, "ER_RelLink_2", "" );

   //:// Position on Table in TE for the ER_Entity on which we just set position.
   //:SET CURSOR FIRST vDTE.ER_Entity WITHIN vDTE.TE_DB_Environ 
   //:           WHERE vDTE.ER_Entity.ZKey = vEMD.ER_Entity.ZKey 
   GetIntegerFromAttribute( &lTempInteger_0, vEMD, "ER_Entity", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vDTE, "ER_Entity", "ZKey", lTempInteger_0, "TE_DB_Environ" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The ER_Entity, " + vEMD.ER_Entity.Name + ", for the Relationship does not exist in the DTE."
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vEMD, "ER_Entity", "Name", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "The ER_Entity, ", 1, 0, 91 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 91 );
      ZeidonStringConcat( szMsg, 1, 0, ", for the Relationship does not exist in the DTE.", 1, 0, 91 );
      //:MessageSend( vSubtask, "", "Tie Relationship to Field", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Tie Relationship to Field", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Build ER work attribute containing Relation and Entity text.
   //:szTargetEntity = vEMD.ER_Entity_2.Name 
   GetVariableFromAttribute( szTargetEntity, 0, 'S', 51, vEMD, "ER_Entity_2", "Name", "", 0 );
   //:SET CURSOR FIRST vEMD.ER_RelLink_2 WHERE vEMD.ER_RelLink_2.CardMax = "m"
   RESULT = SetCursorFirstEntityByString( vEMD, "ER_RelLink_2", "CardMax", "m", "" );
   //:vEMD.ER_RelType.wRelationshipEntityText = vEMD.ER_Entity_2.Name + "  " + vEMD.ER_RelLink_2.Name + "  " + szTargetEntity
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vEMD, "ER_Entity_2", "Name" );
   ZeidonStringConcat( szTempString_1, 1, 0, "  ", 1, 0, 255 );
   GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vEMD, "ER_RelLink_2", "Name", "", 0 );
   ZeidonStringConcat( szTempString_1, 1, 0, szTempString_2, 1, 0, 255 );
   ZeidonStringConcat( szTempString_1, 1, 0, "  ", 1, 0, 255 );
   ZeidonStringConcat( szTempString_1, 1, 0, szTargetEntity, 1, 0, 255 );
   SetAttributeFromString( vEMD, "ER_RelType", "wRelationshipEntityText", szTempString_1 );
   //:SET CURSOR FIRST vEMD.ER_RelLink_2 WHERE vEMD.ER_RelLink_2.CardMax = "1"
   RESULT = SetCursorFirstEntityByString( vEMD, "ER_RelLink_2", "CardMax", "1", "" );

   //:// Build TE work attribute containing Relation and Entity text.
   //:FOR EACH vDTE.TE_FieldDataRel 
   RESULT = SetCursorFirstEntity( vDTE, "TE_FieldDataRel", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vDTE.ER_RelLink EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vDTE, "ER_RelLink" );
      if ( lTempInteger_1 == 0 )
      { 
         //:vDTE.ER_RelLink.wRelationshipEntityText = vDTE.ER_EntityRelLink.Name + "  " + vDTE.ER_RelLink.Name + "  " + vDTE.ER_Entity.Name 
         GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), vDTE, "ER_EntityRelLink", "Name" );
         ZeidonStringConcat( szTempString_3, 1, 0, "  ", 1, 0, 255 );
         GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDTE, "ER_RelLink", "Name", "", 0 );
         ZeidonStringConcat( szTempString_3, 1, 0, szTempString_4, 1, 0, 255 );
         ZeidonStringConcat( szTempString_3, 1, 0, "  ", 1, 0, 255 );
         GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDTE, "ER_Entity", "Name", "", 0 );
         ZeidonStringConcat( szTempString_3, 1, 0, szTempString_5, 1, 0, 255 );
         SetAttributeFromString( vDTE, "ER_RelLink", "wRelationshipEntityText", szTempString_3 );
      } 

      RESULT = SetCursorNextEntity( vDTE, "TE_FieldDataRel", "" );
      //:END 
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_TieFieldsToRelationship( VIEW ViewToWindow )

//:   VIEW vEMD REGISTERED AS TZEREMDO 
zOPER_EXPORT zSHORT OPERATION
CANCEL_TieFieldsToRelationship( zVIEW     ViewToWindow )
{
   zVIEW     vEMD = 0; 
   zSHORT    RESULT; 
   //:VIEW vDTE REGISTERED AS TE_DB_Environ
   zVIEW     vDTE = 0; 

   RESULT = GetViewByName( &vEMD, "TZEREMDO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &vDTE, "TE_DB_Environ", ViewToWindow, zLEVEL_TASK );

   //:DropObjectInstance( vDTE )
   DropObjectInstance( vDTE );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:EXEC_TieFieldsToRelationship( VIEW vSubtask )

//:   VIEW vEMD      REGISTERED AS TZEREMDO 
zOPER_EXPORT zSHORT OPERATION
EXEC_TieFieldsToRelationship( zVIEW     vSubtask )
{
   zVIEW     vEMD = 0; 
   zSHORT    RESULT; 
   //:VIEW vDTE      REGISTERED AS TE_DB_Environ
   zVIEW     vDTE = 0; 
   //:STRING ( 1 )  szSelectFlag
   zCHAR     szSelectFlag[ 2 ] = { 0 }; 
   //:STRING ( 90 ) szMsg
   zCHAR     szMsg[ 91 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &vEMD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   //:// Make sure that at least one entry was selected and that each entry selected is not already tied
   //:// to a relationship.
   //:szSelectFlag = ""
   ZeidonStringCopy( szSelectFlag, 1, 0, "", 1, 0, 2 );
   //:FOR EACH vDTE.TE_FieldDataRel 
   RESULT = SetCursorFirstEntity( vDTE, "TE_FieldDataRel", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = GetSelectStateOfEntity( vDTE, "TE_FieldDataRel" )
      nRC = GetSelectStateOfEntity( vDTE, "TE_FieldDataRel" );
      //:IF nRC = 1
      if ( nRC == 1 )
      { 
         //:szSelectFlag = "Y"
         ZeidonStringCopy( szSelectFlag, 1, 0, "Y", 1, 0, 2 );
         //:IF vDTE.ER_RelLink EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( vDTE, "ER_RelLink" );
         if ( lTempInteger_0 == 0 )
         { 
            //:szMsg = "DTE Field, " + vDTE.TE_TablRec.Name + ", is already tied to a Relationship."
            GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDTE, "TE_TablRec", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "DTE Field, ", 1, 0, 91 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 91 );
            ZeidonStringConcat( szMsg, 1, 0, ", is already tied to a Relationship.", 1, 0, 91 );
            //:MessageSend( vSubtask, "", "Tie Relationship to Field", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "", "Tie Relationship to Field", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vDTE, "TE_FieldDataRel", "" );
      //:END
   } 

   //:END
   //:IF szSelectFlag = ""
   if ( ZeidonStringCompare( szSelectFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:MessageSend( vSubtask, "", "Tie Relationship to Field",
      //:             "No Column entry was selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Tie Relationship to Field", "No Column entry was selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Prompt operator that both DTE and ERD will be saved to DB.
   //:nRC = MessagePrompt( vSubtask, "", "Tie Relationship to Field",
   //:                     "Note that both ER and TE will be saved with this function. Do you want to continue?",
   //:                     0, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION )
   nRC = MessagePrompt( vSubtask, "", "Tie Relationship to Field", "Note that both ER and TE will be saved with this function. Do you want to continue?", 0, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION );
   //:IF nRC = zRESPONSE_YES
   if ( nRC == zRESPONSE_YES )
   { 
      //:FOR EACH vDTE.TE_FieldDataRel 
      RESULT = SetCursorFirstEntity( vDTE, "TE_FieldDataRel", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:nRC = GetSelectStateOfEntity( vDTE, "TE_FieldDataRel" )
         nRC = GetSelectStateOfEntity( vDTE, "TE_FieldDataRel" );
         //:IF nRC = 1
         if ( nRC == 1 )
         { 
            //:// Add the Relationship under the DTE Field and delete the ERD Attribute.
            //:// Position on ER RelLink_2 entry with CardMax of m and on ER_Attribute that matches the ER_Attribute in TE.
            //:SET CURSOR FIRST vEMD.ER_RelLink_2 WHERE vEMD.ER_RelLink_2.CardMax = "m"
            RESULT = SetCursorFirstEntityByString( vEMD, "ER_RelLink_2", "CardMax", "m", "" );
            //:SET CURSOR FIRST vEMD.ER_Attribute WHERE vEMD.ER_Attribute.Name = vDTE.ER_Attribute.Name  
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vDTE, "ER_Attribute", "Name" );
            RESULT = SetCursorFirstEntityByString( vEMD, "ER_Attribute", "Name", szTempString_1, "" );
            //:RelinkInstanceToInstance( vDTE, "ER_Attribute", vEMD, "ER_Attribute" ) 
            RelinkInstanceToInstance( vDTE, "ER_Attribute", vEMD, "ER_Attribute" );
            //:vDTE.TE_FieldDataRel.DataOrRelfieldOrSet = "R"    // Switch designation to Relationship/Foreign Key
            SetAttributeFromString( vDTE, "TE_FieldDataRel", "DataOrRelfieldOrSet", "R" );
            //:INCLUDE vDTE.ER_RelLink FROM vEMD.ER_RelLink_2 
            RESULT = IncludeSubobjectFromSubobject( vDTE, "ER_RelLink", vEMD, "ER_RelLink_2", zPOS_AFTER );
            //:DELETE ENTITY vEMD.ER_Attribute 
            RESULT = DeleteEntity( vEMD, "ER_Attribute", zPOS_NEXT );
            //:CommitMetaOI( vSubtask, vDTE, zSOURCE_DTE_META )
            CommitMetaOI( vSubtask, vDTE, zSOURCE_DTE_META );
            //:CommitMetaOI( vSubtask, vEMD, zSOURCE_ERD_META )
            CommitMetaOI( vSubtask, vEMD, zSOURCE_ERD_META );
         } 

         RESULT = SetCursorNextEntity( vDTE, "TE_FieldDataRel", "" );
         //:END
      } 

      //:END
   } 

   //:END

   //:DropObjectInstance( vDTE )
   DropObjectInstance( vDTE );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
