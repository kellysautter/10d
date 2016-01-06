#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
#include "tzlodopr.h" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
BuildTargetLPLR_List( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
DialogCompareDataModels( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
MigrateMeta( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SelectLPLR( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
MigrateCompilerSubobjects( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CommitTZWDLCVO( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
InitializeTZWDLCVO( zVIEW     vSubtask );


static zSHORT
o_InitializeControlSubobject( zVIEW     vTZWDLCVO,
                              zVIEW     vDialog );


static zSHORT
o_InitializeOptionSubobject( zVIEW     vTZWDLCVO,
                             zVIEW     vDialog );


zOPER_EXPORT zSHORT OPERATION
SelectNewLanguageCode( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ActivateTZWDLCVO( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ConvertTextInDialogs( zVIEW     vSubtask );


static zVOID
o_ConvertControlSubobject( zVIEW     vSubtask,
                           zVIEW     vTZWDLCVO,
                           zVIEW     vDialog );


static zVOID
o_ConvertOptionSubobject( zVIEW     vSubtask,
                          zVIEW     vTZWDLCVO,
                          zVIEW     vDialog );


zOPER_EXPORT zSHORT OPERATION
UpdateLanguageText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SelectLPLR_ForSAMerge( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
MergeSAsToER( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
RefreshListBoxes( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
DropPPE_View( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SetCheckoutState( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CleanUpVORs( zVIEW     ViewToWindow );


//:DIALOG OPERATION
//:BuildTargetLPLR_List( VIEW vSubtask )

//:   // Build the Target LPLR list from the TZCMLPLO list by eliminating the REFER members.

//:   VIEW TargetLPLR BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
BuildTargetLPLR_List( zVIEW     vSubtask )
{
   zVIEW     TargetLPLR = 0; 
   //:VIEW SourceLPLR BASED ON LOD TZCMLPLO
   zVIEW     SourceLPLR = 0; 
   //:VIEW TZCMLPLO BASED ON LOD TZCMLPLO
   zVIEW     TZCMLPLO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 


   //:GET VIEW TZCMLPLO NAMED "TZCMLPLO"
   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", vSubtask, zLEVEL_TASK );
   //:GET VIEW SourceLPLR NAMED "SourceLPLR"
   RESULT = GetViewByName( &SourceLPLR, "SourceLPLR", vSubtask, zLEVEL_TASK );
   //:GET VIEW TargetLPLR NAMED "TargetLPLR"
   RESULT = GetViewByName( &TargetLPLR, "TargetLPLR", vSubtask, zLEVEL_TASK );

   //:IF RESULT > 0
   if ( RESULT > 0 )
   { 
      //:DELETE ENTITY TargetLPLR.LPLR
      RESULT = DeleteEntity( TargetLPLR, "LPLR", zPOS_NEXT );
      //:ELSE
   } 
   else
   { 
      //:ACTIVATE TargetLPLR EMPTY
      RESULT = ActivateEmptyObjectInstance( &TargetLPLR, "TZCMLPLO", vSubtask, zSINGLE );
      //:NAME VIEW TargetLPLR "TargetLPLR"
      SetNameForView( TargetLPLR, "TargetLPLR", 0, zLEVEL_TASK );
   } 

   //:END

   //:CREATE ENTITY TargetLPLR.LPLR
   RESULT = CreateEntity( TargetLPLR, "LPLR", zPOS_AFTER );
   //:FOR EACH TZCMLPLO.W_MetaType WHERE
   //:         TZCMLPLO.W_MetaType.Type >= 2000
   RESULT = SetCursorFirstEntity( TZCMLPLO, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2000 ) >= 0 )
      { 

         //:// Do not include Domains (2003), Global Operations (2002) or
         //:// Registered Views (2009) in the list.
         //:IF TZCMLPLO.W_MetaType.Type != 2002 AND
         //:TZCMLPLO.W_MetaType.Type != 2003 AND
         //:TZCMLPLO.W_MetaType.Type != 2009
         if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2002 ) != 0 && CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2003 ) != 0 && CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2009 ) != 0 )
         { 

            //:CREATE ENTITY TargetLPLR.W_MetaType
            RESULT = CreateEntity( TargetLPLR, "W_MetaType", zPOS_AFTER );
            //:TargetLPLR.W_MetaType.Type = TZCMLPLO.W_MetaType.Type
            SetAttributeFromAttribute( TargetLPLR, "W_MetaType", "Type", TZCMLPLO, "W_MetaType", "Type" );
            //:FOR EACH TZCMLPLO.W_MetaDef
            RESULT = SetCursorFirstEntity( TZCMLPLO, "W_MetaDef", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CREATE ENTITY TargetLPLR.W_MetaDef
               RESULT = CreateEntity( TargetLPLR, "W_MetaDef", zPOS_AFTER );
               //:TargetLPLR.W_MetaDef.Name = TZCMLPLO.W_MetaDef.Name
               SetAttributeFromAttribute( TargetLPLR, "W_MetaDef", "Name", TZCMLPLO, "W_MetaDef", "Name" );
               RESULT = SetCursorNextEntity( TZCMLPLO, "W_MetaDef", "" );
            } 

            //:END
         } 

      } 

      RESULT = SetCursorNextEntity( TZCMLPLO, "W_MetaType", "" );
      //:END
   } 

   //:END

   //:// Now order both the Source and Target objects.  Do this by creating a sort
   //:// order attribute that puts domain groups and global operation groups at the
   //:// beginning.
   //:// Following this by sorting MetaDefs within each MetaType.
   //:FOR EACH TargetLPLR.W_MetaType
   RESULT = SetCursorFirstEntity( TargetLPLR, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TargetLPLR.W_MetaType.Type = 2013 OR
      //:   TargetLPLR.W_MetaType.Type = 2014
      if ( CompareAttributeToInteger( TargetLPLR, "W_MetaType", "Type", 2013 ) == 0 || CompareAttributeToInteger( TargetLPLR, "W_MetaType", "Type", 2014 ) == 0 )
      { 
         //:TargetLPLR.W_MetaType.SortOrder = TargetLPLR.W_MetaType.Type
         SetAttributeFromAttribute( TargetLPLR, "W_MetaType", "SortOrder", TargetLPLR, "W_MetaType", "Type" );
         //:else
      } 
      else
      { 
         //:TargetLPLR.W_MetaType.SortOrder = TargetLPLR.W_MetaType.Type + 20
         GetIntegerFromAttribute( &lTempInteger_0, TargetLPLR, "W_MetaType", "Type" );
         lTempInteger_1 = lTempInteger_0 + 20;
         SetAttributeFromInteger( TargetLPLR, "W_MetaType", "SortOrder", lTempInteger_1 );
      } 

      RESULT = SetCursorNextEntity( TargetLPLR, "W_MetaType", "" );
      //:END
   } 

   //:END
   //:OrderEntityForView( TargetLPLR, "W_MetaType", "SortOrder A" )
   OrderEntityForView( TargetLPLR, "W_MetaType", "SortOrder A" );
   //:FOR EACH TargetLPLR.W_MetaType
   RESULT = SetCursorFirstEntity( TargetLPLR, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:OrderEntityForView( TargetLPLR, "W_MetaDef", "Name A" )
      OrderEntityForView( TargetLPLR, "W_MetaDef", "Name A" );
      RESULT = SetCursorNextEntity( TargetLPLR, "W_MetaType", "" );
   } 

   //:END

   //:// Alter sort order for Domain Groups (2013) and Global Operation Groups (2014)
   //:// as above
   //:FOR EACH SourceLPLR.W_MetaType
   RESULT = SetCursorFirstEntity( SourceLPLR, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF SourceLPLR.W_MetaType.Type = 2013 OR
      //:   SourceLPLR.W_MetaType.Type = 2014
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2013 ) == 0 || CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2014 ) == 0 )
      { 
         //:SourceLPLR.W_MetaType.SortOrder = SourceLPLR.W_MetaType.Type
         SetAttributeFromAttribute( SourceLPLR, "W_MetaType", "SortOrder", SourceLPLR, "W_MetaType", "Type" );
         //:else
      } 
      else
      { 
         //:SourceLPLR.W_MetaType.SortOrder = SourceLPLR.W_MetaType.Type + 20
         GetIntegerFromAttribute( &lTempInteger_2, SourceLPLR, "W_MetaType", "Type" );
         lTempInteger_3 = lTempInteger_2 + 20;
         SetAttributeFromInteger( SourceLPLR, "W_MetaType", "SortOrder", lTempInteger_3 );
      } 

      RESULT = SetCursorNextEntity( SourceLPLR, "W_MetaType", "" );
      //:END
   } 

   //:END
   //:OrderEntityForView( SourceLPLR, "W_MetaType", "SortOrder A" )
   OrderEntityForView( SourceLPLR, "W_MetaType", "SortOrder A" );
   //:FOR EACH SourceLPLR.W_MetaType
   RESULT = SetCursorFirstEntity( SourceLPLR, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:OrderEntityForView( SourceLPLR, "W_MetaDef", "Name A" )
      OrderEntityForView( SourceLPLR, "W_MetaDef", "Name A" );
      RESULT = SetCursorNextEntity( SourceLPLR, "W_MetaType", "" );
   } 

   //:END

   //:// Now be sure to position at the beginning of both lists.
   //:SET CURSOR FIRST TargetLPLR.W_MetaType
   RESULT = SetCursorFirstEntity( TargetLPLR, "W_MetaType", "" );
   //:SET CURSOR FIRST SourceLPLR.W_MetaType
   RESULT = SetCursorFirstEntity( SourceLPLR, "W_MetaType", "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DialogCompareDataModels( VIEW vSubtask )

//:   // Activate the "other" LPLR and Data Model for the selected LPLR and call the
//:   // Compare Data Model operation.

//:   VIEW vCM         BASED ON LOD TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
DialogCompareDataModels( zVIEW     vSubtask )
{
   zVIEW     vCM = 0; 
   //:VIEW OtherLPLR   BASED ON LOD TZCMLPLO
   zVIEW     OtherLPLR = 0; 
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 
   //:VIEW OtherERD    BASED ON LOD TZEREMDO
   zVIEW     OtherERD = 0; 
   //:VIEW CurrentERD  BASED ON LOD TZEREMDO
   zVIEW     CurrentERD = 0; 
   //:VIEW TZERRPTO    BASED ON LOD TZERRPTO
   zVIEW     TZERRPTO = 0; 


   //:STRING (512) FileName
   zCHAR     FileName[ 513 ] = { 0 }; 
   //:STRING (32)  TruncatedName
   zCHAR     TruncatedName[ 33 ] = { 0 }; 
   //:SHORT        nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:GET VIEW vCM NAMED "TZCMWKSO"
   RESULT = GetViewByName( &vCM, "TZCMWKSO", vSubtask, zLEVEL_TASK );

   //:// Activate Other LPLR.
   //:FileName = vCM.LPLR.Name
   GetVariableFromAttribute( FileName, 0, 'S', 513, vCM, "LPLR", "Name", "", 0 );
   //:TruncatedName = FileName [1:32]
   ZeidonStringCopy( TruncatedName, 1, 0, FileName, 1, 32, 33 );
   //:FileName = vCM.LPLR.ExecDir + "\" + TruncatedName + ".XLP"
   GetStringFromAttribute( FileName, sizeof( FileName ), vCM, "LPLR", "ExecDir" );
   ZeidonStringConcat( FileName, 1, 0, "\\", 1, 0, 513 );
   ZeidonStringConcat( FileName, 1, 0, TruncatedName, 1, 0, 513 );
   ZeidonStringConcat( FileName, 1, 0, ".XLP", 1, 0, 513 );
   //:nRC = ActivateOI_FromFile( OtherLPLR, "TZCMLPLO", vSubtask, FileName, zSINGLE )
   nRC = ActivateOI_FromFile( &OtherLPLR, "TZCMLPLO", vSubtask, FileName, zSINGLE );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask, "", "Compare Data Models",
      //:             "Error activating Other LPLR", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Compare Data Models", "Error activating Other LPLR", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:NAME VIEW OtherLPLR "OtherLPLR"
   SetNameForView( OtherLPLR, "OtherLPLR", 0, zLEVEL_TASK );

   //:// Activate Other data model (2004 is zREFER_ERD_META)
   //:SET CURSOR FIRST OtherLPLR.W_MetaType WHERE OtherLPLR.W_MetaType.Type = 2004
   RESULT = SetCursorFirstEntityByInteger( OtherLPLR, "W_MetaType", "Type", 2004, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( vSubtask, "", "Compare Data Models",
      //:             "Selected LPLR does not have a Data Model", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Compare Data Models", "Selected LPLR does not have a Data Model", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:FileName = OtherLPLR.LPLR.MetaSrcDir + "\" + OtherLPLR.W_MetaDef.Name + ".PMD"
   GetStringFromAttribute( FileName, sizeof( FileName ), OtherLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( FileName, 1, 0, "\\", 1, 0, 513 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, OtherLPLR, "W_MetaDef", "Name", "", 0 );
   ZeidonStringConcat( FileName, 1, 0, szTempString_0, 1, 0, 513 );
   ZeidonStringConcat( FileName, 1, 0, ".PMD", 1, 0, 513 );
   //:nRC = ActivateOI_FromFile( OtherERD, "TZEREMDO", OtherLPLR, FileName, zSINGLE )
   nRC = ActivateOI_FromFile( &OtherERD, "TZEREMDO", OtherLPLR, FileName, zSINGLE );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask, "", "Compare Data Models",
      //:             "Error activating Other ERD", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Compare Data Models", "Error activating Other ERD", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Retrieve Current LPLR.
   //:GET VIEW CurrentLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:// Activate Current data model.
   //:SET CURSOR FIRST CurrentLPLR.W_MetaType WHERE CurrentLPLR.W_MetaType.Type = 2004
   RESULT = SetCursorFirstEntityByInteger( CurrentLPLR, "W_MetaType", "Type", 2004, "" );
   //:nRC = ActivateMetaOI( vSubtask, CurrentERD, CurrentLPLR, zSOURCE_ERD_META, zSINGLE )
   nRC = ActivateMetaOI( vSubtask, &CurrentERD, CurrentLPLR, zSOURCE_ERD_META, zSINGLE );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask, "", "Compare Data Models",
      //:             "Error activating Current ERD", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Compare Data Models", "Error activating Current ERD", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Call operation to Compare the two data models.
   //:CompareDataModels( CurrentERD, OtherERD, CurrentLPLR, OtherLPLR, vSubtask )
   oTZEREMDO_CompareDataModels( CurrentERD, OtherERD, CurrentLPLR, OtherLPLR, vSubtask );

   //:// Print the report.
   //:GET VIEW TZERRPTO NAMED "TZERRPTO"
   RESULT = GetViewByName( &TZERRPTO, "TZERRPTO", vSubtask, zLEVEL_TASK );
   //:FormatSubobjectOnDoc( TZERRPTO, "Root", vSubtask , "ERDCOMP", 0 )
   FormatSubobjectOnDoc( TZERRPTO, "Root", vSubtask, "ERDCOMP", 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:MigrateMeta( VIEW vSubtask )

//:   VIEW SourceLPLR         BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
MigrateMeta( zVIEW     vSubtask )
{
   zVIEW     SourceLPLR = 0; 
   //:VIEW SourceVOR_LPLR     BASED ON LOD TZCMLPLO
   zVIEW     SourceVOR_LPLR = 0; 
   //:VIEW TargetVOR_LPLR     BASED ON LOD TZCMLPLO
   zVIEW     TargetVOR_LPLR = 0; 
   //:VIEW OrigLPLR           BASED ON LOD TZCMLPLO
   zVIEW     OrigLPLR = 0; 
   //:VIEW CurrentLPLR        BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 
   //:VIEW NewERD             BASED ON LOD TZEREMDO
   zVIEW     NewERD = 0; 
   //:VIEW ReturnedSA         BASED ON LOD TZERSASO
   zVIEW     ReturnedSA = 0; 
   //:VIEW ReturnedTE         BASED ON LOD TZTENVRO
   zVIEW     ReturnedTE = 0; 
   //:VIEW NewLOD             BASED ON LOD TZZOLODO
   zVIEW     NewLOD = 0; 
   //:VIEW ReturnedPPE        BASED ON LOD TZPESRCO
   zVIEW     ReturnedPPE = 0; 
   //:VIEW NewDialog          BASED ON LOD TZWDLGSO
   zVIEW     NewDialog = 0; 
   //:VIEW ReturnedDialogSpec BASED ON LOD TZADCSDO
   zVIEW     ReturnedDialogSpec = 0; 
   //:VIEW ReturnedReport     BASED ON LOD TZRPSRCO
   zVIEW     ReturnedReport = 0; 
   //:VIEW ReturnedXSLT       BASED ON LOD TZXSLTSO
   zVIEW     ReturnedXSLT = 0; 
   //:VIEW NewDomainGroup     BASED ON LOD TZDGSRCO
   zVIEW     NewDomainGroup = 0; 
   //:VIEW NewOperationGroup  BASED ON LOD TZOGSRCO
   zVIEW     NewOperationGroup = 0; 
   //:VIEW ReturnedHeader     BASED ON LOD TZOPHDRO
   zVIEW     ReturnedHeader = 0; 
   //:VIEW vPE                BASED ON LOD TZPESRCO
   zVIEW     vPE = 0; 
   //:VIEW vVOR               BASED ON LOD TZWDVORO
   zVIEW     vVOR = 0; 

   //:STRING (32)  MetaName
   zCHAR     MetaName[ 33 ] = { 0 }; 
   //:STRING (32)  NewTE_Name
   zCHAR     NewTE_Name[ 33 ] = { 0 }; 
   //:STRING (250) MG_ErrorMessage
   zCHAR     MG_ErrorMessage[ 251 ] = { 0 }; 
   //:STRING (513) SourceDirectoryName       // zMAX_FILESPEC_LTH+1
   zCHAR     SourceDirectoryName[ 514 ] = { 0 }; 
   //:STRING (513) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 1 ) szDoNotMergeOperationsFlag
   zCHAR     szDoNotMergeOperationsFlag[ 2 ] = { 0 }; 
   //://INTEGER      nConvertFlag
   //:INTEGER      lZKey
   zLONG     lZKey = 0; 
   //:SHORT        nRC
   zSHORT    nRC = 0; 
   //:SHORT        nRC1
   zSHORT    nRC1 = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 


   //:GET VIEW SourceLPLR NAMED "SourceLPLR"
   RESULT = GetViewByName( &SourceLPLR, "SourceLPLR", vSubtask, zLEVEL_TASK );
   //:GET VIEW SourceVOR_LPLR NAMED "SourceVOR_LPLR"
   RESULT = GetViewByName( &SourceVOR_LPLR, "SourceVOR_LPLR", vSubtask, zLEVEL_TASK );

   //:// Process the Domain Groups and the Operation Groups first, since they must exist before
   //:// other metas can be processed, and their numbers put them at the end of the list.

   //:// For creating/updating VOR's, put the ObjectName for each VOR in the W_MetaDef entry.
   //:SourceDirectoryName = SourceLPLR.LPLR.MetaSrcDir
   GetVariableFromAttribute( SourceDirectoryName, 0, 'S', 514, SourceLPLR, "LPLR", "MetaSrcDir", "", 0 );
   //:FOR EACH SourceVOR_LPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( SourceVOR_LPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// Activate existing source meta VOR
      //:lZKey = SourceVOR_LPLR.W_MetaDef.CPLR_ZKey
      GetIntegerFromAttribute( &lZKey, SourceVOR_LPLR, "W_MetaDef", "CPLR_ZKey" );
      //:zltoxa( lZKey, MetaName )
      zltoxa( lZKey, MetaName );
      //:SourceFileName = SourceDirectoryName + "\" + MetaName + ".PVR"
      ZeidonStringCopy( SourceFileName, 1, 0, SourceDirectoryName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, MetaName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, ".PVR", 1, 0, 514 );
      //:ActivateOI_FromFile ( vVOR, "TZWDVORO", vSubtask, SourceFileName, 8192 )
      ActivateOI_FromFile( &vVOR, "TZWDVORO", vSubtask, SourceFileName, 8192 );
      //:// 8192 is zIGNORE_ATTRIB_ERRORS
      //:NAME VIEW vVOR "vVOR"
      SetNameForView( vVOR, "vVOR", 0, zLEVEL_TASK );
      //:// Set the work name as the LOD name.
      //:SourceVOR_LPLR.W_MetaDef.MigrateWorkName = vVOR.LOD.Name
      SetAttributeFromAttribute( SourceVOR_LPLR, "W_MetaDef", "MigrateWorkName", vVOR, "LOD", "Name" );
      //:DropObjectInstance( vVOR )
      DropObjectInstance( vVOR );
      RESULT = SetCursorNextEntity( SourceVOR_LPLR, "W_MetaDef", "" );
   } 

   //:END

   //://nConvertFlag = 0        // Indicate that we are not converting.
   //:nRC = SetCursorFirstSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" )
   nRC = SetCursorFirstSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 

      //:MetaName = SourceLPLR.W_MetaDef.Name
      GetVariableFromAttribute( MetaName, 0, 'S', 33, SourceLPLR, "W_MetaDef", "Name", "", 0 );

      //:// Domain Group: W_MetaType.Type = 2013 for domain group meta
      //:IF SourceLPLR.W_MetaType.Type = 2013
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2013 ) == 0 )
      { 
         //:// If the Domain Group doesn't already exist, create it.
         //:// Otherwise pass the activated DomainGroup to the migrate routine so
         //:// that Domains can be merged.
         //:nRC = ActivateMetaOI_ByName( vSubtask, NewDomainGroup, 0, zSOURCE_DOMAINGRP_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &NewDomainGroup, 0, zSOURCE_DOMAINGRP_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:ActivateEmptyMetaOI( vSubtask, NewDomainGroup, zSOURCE_DOMAINGRP_META, zSINGLE )
            ActivateEmptyMetaOI( vSubtask, &NewDomainGroup, zSOURCE_DOMAINGRP_META, zSINGLE );
         } 

         //:END

         //:MG_ErrorMessage = "Migrating Domain Group: " + MetaName
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Domain Group: ", 1, 0, 251 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
         //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
         MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
         //:nRC = DomainGrpMigrate( NewDomainGroup, MetaName, SourceLPLR, vSubtask )
         nRC = oTZDGSRCO_DomainGrpMigrate( NewDomainGroup, MetaName, SourceLPLR, vSubtask );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Domain Group(" + MetaName + ") aborted."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Domain Group(", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01001", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01001", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         } 

         //:END

         //:IF NewDomainGroup != 0
         if ( NewDomainGroup != 0 )
         { 
            //:DropMetaOI( vSubtask, NewDomainGroup )
            DropMetaOI( vSubtask, NewDomainGroup );
         } 

         //:END

         //:// Turn off the selected state flag
         //:SetSelectStateOfEntity( SourceLPLR, "W_MetaDef", 0 )
         SetSelectStateOfEntity( SourceLPLR, "W_MetaDef", 0 );
      } 


      //:END

      //:// Global Operation Group: W_MetaType.Type = 2014 for global operation group meta
      //:IF SourceLPLR.W_MetaType.Type = 2014
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2014 ) == 0 )
      { 
         //:// If the Global Operation Group doesn't already exist, create it.
         //:// Otherwise pass the activated GlobalOperationGroup to the migrate routine so
         //:// that Global Operations can be merged.
         //:nRC = ActivateMetaOI_ByName( vSubtask, NewOperationGroup, 0, zSOURCE_GOPGRP_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &NewOperationGroup, 0, zSOURCE_GOPGRP_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:ActivateEmptyMetaOI( vSubtask, NewOperationGroup, zSOURCE_GOPGRP_META, zSINGLE )
            ActivateEmptyMetaOI( vSubtask, &NewOperationGroup, zSOURCE_GOPGRP_META, zSINGLE );
         } 

         //:END

         //:MG_ErrorMessage = "Migrating Global Operation Group: " + MetaName
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Global Operation Group: ", 1, 0, 251 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
         //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
         MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
         //:nRC = GlobalOperGrpMigrate( NewOperationGroup, MetaName, SourceLPLR, vSubtask )
         nRC = oTZOGSRCO_GlobalOperGrpMigrate( NewOperationGroup, MetaName, SourceLPLR, vSubtask );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Global Operation Group(" + MetaName + ") aborted."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Global Operation Group(", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01001", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01001", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         } 

         //:END

         //:IF NewOperationGroup != 0
         if ( NewOperationGroup != 0 )
         { 
            //:DropMetaOI( vSubtask, NewOperationGroup )
            DropMetaOI( vSubtask, NewOperationGroup );
         } 

         //:END
         //:// Turn off the selected state flag
         //:SetSelectStateOfEntity( SourceLPLR, "W_MetaDef", 0 )
         SetSelectStateOfEntity( SourceLPLR, "W_MetaDef", 0 );
      } 


      //:END

      //:nRC = SetCursorNextSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" )
      nRC = SetCursorNextSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" );
   } 


   //:END

   //:// Because some migration operations need an LPLR view with Domains in it??, we will
   //:// pass the OrigLPLR view instead of the SourceLPLR view.
   //:GET VIEW OrigLPLR NAMED "OrigLPLR"
   RESULT = GetViewByName( &OrigLPLR, "OrigLPLR", vSubtask, zLEVEL_TASK );

   //:// Now loop and process the regular metas (those for the current release).
   //:nRC = SetCursorFirstSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" )
   nRC = SetCursorFirstSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 

      //:// Position on the corresponding meta in the OrigLPLR view.
      //:SET CURSOR FIRST OrigLPLR.W_MetaDef WITHIN OrigLPLR.LPLR WHERE
      //:                 OrigLPLR.W_MetaDef.CPLR_ZKey = SourceLPLR.W_MetaDef.CPLR_ZKey
      GetIntegerFromAttribute( &lTempInteger_0, SourceLPLR, "W_MetaDef", "CPLR_ZKey" );
      RESULT = SetCursorFirstEntityByInteger( OrigLPLR, "W_MetaDef", "CPLR_ZKey", lTempInteger_0, "LPLR" );

      //:// Turn off the selected state flag
      //:SetSelectStateOfEntity( SourceLPLR, "W_MetaDef", 0 )
      SetSelectStateOfEntity( SourceLPLR, "W_MetaDef", 0 );
      //:MetaName = SourceLPLR.W_MetaDef.Name
      GetVariableFromAttribute( MetaName, 0, 'S', 33, SourceLPLR, "W_MetaDef", "Name", "", 0 );

      //:// Header: W_MetaType.Type = 2000 for Header meta
      //:IF SourceLPLR.W_MetaType.Type = 2000
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2000 ) == 0 )
      { 

         //:// If the Header doesn't already exist, create it.
         //:// Otherwise return error message.
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedHeader, 0, zREFER_HDR_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedHeader, 0, zREFER_HDR_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating Header: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Header: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:nRC = HeaderMigrate( OrigLPLR, MetaName, ReturnedHeader, vSubtask )
            nRC = oTZCMLPLO_HeaderMigrate( OrigLPLR, MetaName, &ReturnedHeader, vSubtask );
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:MG_ErrorMessage = "Header (" + MetaName + ") aborted."
               ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Header (", 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
               //:MessageSend( vSubtask, "CM01005", "Configuration Management",
               //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vSubtask, "CM01005", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            } 

            //:END
            //:IF ReturnedHeader != 0
            if ( ReturnedHeader != 0 )
            { 
               //:DropMetaOI( vSubtask, ReturnedHeader )
               DropMetaOI( vSubtask, ReturnedHeader );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "Header (" + MetaName + ") already exists."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Header (", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") already exists.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01006", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01006", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:DropMetaOI( vSubtask, ReturnedHeader )
            DropMetaOI( vSubtask, ReturnedHeader );
         } 

         //:END
      } 

      //:END

      //:// ERD: W_MetaType.Type = 2004 for ERD meta
      //:IF SourceLPLR.W_MetaType.Type = 2004
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2004 ) == 0 )
      { 

         //:// If the ERD doesn't already exist, create it.
         //:// Otherwise pass the activated ERD to the migrate routine so that entities, attributes and
         //:// relationships can be merged.
         //:GET VIEW  CurrentLPLR NAMED "TaskLPLR"
         RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
         //:MetaName = CurrentLPLR.LPLR.Name
         GetVariableFromAttribute( MetaName, 0, 'S', 33, CurrentLPLR, "LPLR", "Name", "", 0 );
         //:nRC = ActivateMetaOI_ByName( vSubtask, NewERD, 0, zSOURCE_ERD_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &NewERD, 0, zSOURCE_ERD_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:ActivateEmptyMetaOI( vSubtask, NewERD, zSOURCE_ERD_META, zSINGLE )
            ActivateEmptyMetaOI( vSubtask, &NewERD, zSOURCE_ERD_META, zSINGLE );
         } 

         //:END
         //:NAME VIEW NewERD "NewERD"
         SetNameForView( NewERD, "NewERD", 0, zLEVEL_TASK );

         //:MG_ErrorMessage = "Migrating ERD: " + MetaName
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating ERD: ", 1, 0, 251 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
         //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
         MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
         //:ERD_Migrate( NewERD, OrigLPLR, vSubtask )
         oTZEREMDO_ERD_Migrate( NewERD, &OrigLPLR, vSubtask );
         //:DropMetaOI( vSubtask, NewERD )
         DropMetaOI( vSubtask, NewERD );
      } 


      //:END

      //:// Subject Area: W_MetaType.Type = 2005 for Subject Area meta
      //:IF SourceLPLR.W_MetaType.Type = 2005
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2005 ) == 0 )
      { 

         //:// If the Subject Area doesn't already exist, create it.
         //:// Otherwise return error message, as we're not going to merge subject areas at this time.
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedSA, 0, zREFER_SA_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedSA, 0, zREFER_SA_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating Subject Area: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Subject Area: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:SA_Migrate( ReturnedSA, MetaName, OrigLPLR, vSubtask )
            oTZERSASO_SA_Migrate( &ReturnedSA, MetaName, OrigLPLR, vSubtask );
            //:IF ReturnedSA != 0
            if ( ReturnedSA != 0 )
            { 
               //:DropMetaOI( vSubtask, ReturnedSA )
               DropMetaOI( vSubtask, ReturnedSA );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "Subject Area (" + MetaName + ") already exists."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Subject Area (", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") already exists.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01011", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01011", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         } 

         //:END
      } 

      //:END

      //:// Technical Environment: W_MetaType.Type = 2006 for TE meta
      //:IF SourceLPLR.W_MetaType.Type = 2006
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2006 ) == 0 )
      { 

         //:// If the TE doesn't already exist, create it.
         //:// Otherwise return error message, as we're not going to merge TEs at this time.
         //:// Note that the name of the Meta is the name of the LPLR.
         //:GET VIEW  CurrentLPLR NAMED "TaskLPLR"
         RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
         //:NewTE_Name = CurrentLPLR.LPLR.Name
         GetVariableFromAttribute( NewTE_Name, 0, 'S', 33, CurrentLPLR, "LPLR", "Name", "", 0 );
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedTE, 0, zREFER_DTE_META, zSINGLE, NewTE_Name, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedTE, 0, zREFER_DTE_META, zSINGLE, NewTE_Name, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating TE: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating TE: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:TE_Migrate( ReturnedTE, MetaName, OrigLPLR, vSubtask )
            oTZTENVRO_TE_Migrate( &ReturnedTE, MetaName, OrigLPLR, vSubtask );
            //:IF ReturnedTE != 0
            if ( ReturnedTE != 0 )
            { 
               //:DropMetaOI( vSubtask, ReturnedTE )
               DropMetaOI( vSubtask, ReturnedTE );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "Technical Environment (" + NewTE_Name + ") already exists."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Technical Environment (", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, NewTE_Name, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") already exists.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01012", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01012", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:DropMetaOI( vSubtask, ReturnedTE )
            DropMetaOI( vSubtask, ReturnedTE );
         } 

         //:END
      } 

      //:END

      //:// LOD: W_MetaType.Type = 2007 for LOD meta
      //:IF SourceLPLR.W_MetaType.Type = 2007
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2007 ) == 0 )
      { 

         //:// Copy over or merge LOD based on value of DoNotMergeFlag for the LOD.
         //:// If the DoNotMergeFlag is set but the DoNotMergeOperationsFlag is not, we will only merge in Operations.
         //:// If both are set, we will not do anything to this LOD.
         //:// If the DoNotMergeFlag is not set, we will delete the old LOD structure and recreate it in totality, including Operations.
         //:GET VIEW  CurrentLPLR NAMED "TaskLPLR"
         RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
         //:nRC = ActivateMetaOI_ByName( vSubtask, NewLOD, 0, zSOURCE_LOD_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &NewLOD, 0, zSOURCE_LOD_META, zSINGLE, MetaName, 0 );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:SET CURSOR FIRST CurrentLPLR.W_MetaType WHERE CurrentLPLR.W_MetaType.Type = 7
            RESULT = SetCursorFirstEntityByInteger( CurrentLPLR, "W_MetaType", "Type", 7, "" );
            //:SET CURSOR FIRST CurrentLPLR.W_MetaDef WHERE CurrentLPLR.W_MetaDef.Name = SourceLPLR.W_MetaDef.Name
            GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), SourceLPLR, "W_MetaDef", "Name" );
            RESULT = SetCursorFirstEntityByString( CurrentLPLR, "W_MetaDef", "Name", szTempString_0, "" );
            //:IF NewLOD.LOD.DoNotMergeFlag = "" AND NewLOD.LOD.DoNotMergeOperationsFlag = ""
            if ( CompareAttributeToString( NewLOD, "LOD", "DoNotMergeFlag", "" ) == 0 && CompareAttributeToString( NewLOD, "LOD", "DoNotMergeOperationsFlag", "" ) == 0 )
            { 
               //:DELETE ENTITY NewLOD.LOD_EntityParent
               RESULT = DeleteEntity( NewLOD, "LOD_EntityParent", zPOS_NEXT );
               //:FOR EACH NewLOD.SourceFile
               RESULT = SetCursorFirstEntity( NewLOD, "SourceFile", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:DELETE ENTITY NewLOD.SourceFile NONE
                  RESULT = DeleteEntity( NewLOD, "SourceFile", zREPOS_NONE );
                  RESULT = SetCursorNextEntity( NewLOD, "SourceFile", "" );
               } 

               //:END
               //:IF NewLOD.POD EXISTS
               lTempInteger_1 = CheckExistenceOfEntity( NewLOD, "POD" );
               if ( lTempInteger_1 == 0 )
               { 
                  //:DELETE ENTITY NewLOD.POD
                  RESULT = DeleteEntity( NewLOD, "POD", zPOS_NEXT );
               } 

               //:END
               //:IF NewLOD.LOD_ConstraintOper EXISTS
               lTempInteger_2 = CheckExistenceOfEntity( NewLOD, "LOD_ConstraintOper" );
               if ( lTempInteger_2 == 0 )
               { 
                  //:EXCLUDE NewLOD.LOD_ConstraintOper
                  RESULT = ExcludeEntity( NewLOD, "LOD_ConstraintOper", zREPOS_AFTER );
               } 

               //:END
            } 

            //:END
            //:NAME VIEW NewLOD "NewLOD"
            SetNameForView( NewLOD, "NewLOD", 0, zLEVEL_TASK );
            //:ELSE
         } 
         else
         { 
            //:ActivateEmptyMetaOI( vSubtask, NewLOD, zSOURCE_LOD_META, zSINGLE )
            ActivateEmptyMetaOI( vSubtask, &NewLOD, zSOURCE_LOD_META, zSINGLE );
         } 

         //:END
         //:NAME VIEW NewLOD "NewLOD"
         SetNameForView( NewLOD, "NewLOD", 0, zLEVEL_TASK );

         //:// Migrate LOD, unless the request is for both the Structure and Operations to not be merged.
         //:// If LOD_EntityParent doesn't exist, then this is either a new LOD or the DoNotMergeFlag was not set for an existing LOD.
         //:IF NewLOD.LOD_EntityParent DOES NOT EXIST OR szDoNotMergeOperationsFlag = ""
         lTempInteger_3 = CheckExistenceOfEntity( NewLOD, "LOD_EntityParent" );
         if ( lTempInteger_3 != 0 || ZeidonStringCompare( szDoNotMergeOperationsFlag, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //:MG_ErrorMessage = "Migrating LOD: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating LOD: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:nRC = LOD_Migrate( NewLOD, MetaName, OrigLPLR, vSubtask )
            nRC = oTZZOLODO_LOD_Migrate( NewLOD, MetaName, OrigLPLR, vSubtask );
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:MG_ErrorMessage = "LOD (" + MetaName + ") aborted."
               ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD (", 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
               //:MessageSend( vSubtask, "CM01013", "Configuration Management",
               //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vSubtask, "CM01013", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            } 

            //:END
         } 

         //:END
         //:DropMetaOI( vSubtask, NewLOD )
         DropMetaOI( vSubtask, NewLOD );
      } 

      //:END

      //:// PPE: W_MetaType.Type = 2010 for PPE meta
      //:IF SourceLPLR.W_MetaType.Type = 2010
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2010 ) == 0 )
      { 

         //:// If the PPE doesn't already exist, create it.
         //:// Otherwise ask for merge.
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedPPE, 0, zSOURCE_PENV_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedPPE, 0, zSOURCE_PENV_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating PPE: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating PPE: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:nRC = MigratePPE( ReturnedPPE, SourceLPLR, vSubtask )
            nRC = oTZPESRCO_MigratePPE( &ReturnedPPE, SourceLPLR, vSubtask );
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:IF nRC = -16  //SourceLPLR has a old PPE
               if ( nRC == -16 )
               { 
                  //:MG_ErrorMessage = "Invalid PPE in Source Project!" + NEW_LINE +
                  //:                  "New PPE required. Migrating aborted."
                  ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Invalid PPE in Source Project!", 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, NEW_LINE, 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, "New PPE required. Migrating aborted.", 1, 0, 251 );
                  //:ELSE
               } 
               else
               { 
                  //:MG_ErrorMessage = "Migrating of PPE will be aborted."
                  ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating of PPE will be aborted.", 1, 0, 251 );
               } 

               //:END
               //:MessageSend( vSubtask, "CM01013", "Configuration Management",
               //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vSubtask, "CM01013", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               //:RETURN nRC
               return( nRC );
            } 

            //:END
            //:ELSE //PPE exists
         } 
         else
         { 
            //:MG_ErrorMessage = "PPE already exists. Do you want to merge it?"
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "PPE already exists. Do you want to merge it?", 1, 0, 251 );
            //:nRC = MessagePrompt( vSubtask, "CM01014", "Configuration Management",
            //:                     MG_ErrorMessage, 1,         zBUTTONS_YESNO,
            //:                     zRESPONSE_YES  ,  0 )
            nRC = MessagePrompt( vSubtask, "CM01014", "Configuration Management", MG_ErrorMessage, 1, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
            //:IF nRC = zRESPONSE_YES
            if ( nRC == zRESPONSE_YES )
            { 
               //:MB_SetMessage( vSubtask, 1, "Merging Zeidon.PPE" )
               MB_SetMessage( vSubtask, 1, "Merging Zeidon.PPE" );
               //://if Repository Project, check check out status
               //:nRC1 = ComponentIsCheckedOut( vSubtask, ReturnedPPE, 2010 )
               nRC1 = ComponentIsCheckedOut( vSubtask, ReturnedPPE, 2010 );
               //:IF nRC1 = 0
               if ( nRC1 == 0 )
               { 
                  //:MG_ErrorMessage = "PPE is not checked out. Merging aborted."
                  ZeidonStringCopy( MG_ErrorMessage, 1, 0, "PPE is not checked out. Merging aborted.", 1, 0, 251 );
                  //:MessageSend( vSubtask, "CM01013", "Configuration Management",
                  //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( vSubtask, "CM01013", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  //:RETURN -1
                  return( -1 );
               } 

               //:END
               //:nRC = MergePPE( ReturnedPPE, SourceLPLR, vSubtask )
               nRC = oTZPESRCO_MergePPE( ReturnedPPE, SourceLPLR, vSubtask );
               //:IF nRC < 0
               if ( nRC < 0 )
               { 
                  //:IF nRC = -16  //SourceLPLR has a old PPE
                  if ( nRC == -16 )
                  { 
                     //:MG_ErrorMessage = "Invalid PPE in Source Project! " + NEW_LINE +
                     //:                  "New PPE required. Merging aborted."
                     ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Invalid PPE in Source Project! ", 1, 0, 251 );
                     ZeidonStringConcat( MG_ErrorMessage, 1, 0, NEW_LINE, 1, 0, 251 );
                     ZeidonStringConcat( MG_ErrorMessage, 1, 0, "New PPE required. Merging aborted.", 1, 0, 251 );
                     //:MessageSend( vSubtask, "CM01013", "Configuration Management",
                     //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                     MessageSend( vSubtask, "CM01013", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  } 

                  //:END
                  //:IF ReturnedPPE != 0
                  if ( ReturnedPPE != 0 )
                  { 
                     //:DropMetaOI( vSubtask, ReturnedPPE )
                     DropMetaOI( vSubtask, ReturnedPPE );
                  } 

                  //:END
                  //:RETURN nRC
                  return( nRC );
               } 

               //:END
            } 

            //:END
         } 

         //:END //END IF nRC < 0
         //:DropMetaOI( vSubtask, ReturnedPPE )
         DropMetaOI( vSubtask, ReturnedPPE );
      } 

      //:END

      //:// Dialog: W_MetaType.Type = 2011 for Dialog meta
      //:IF SourceLPLR.W_MetaType.Type = 2011
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2011 ) == 0 )
      { 

         //:// Make sure a valid Presentation Environment exists.
         //:// If not, exit the operation.
         //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zREFER_PENV_META )
         RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zREFER_PENV_META );
         //:MG_ErrorMessage = "Dialog (" + MetaName + ") aborted."
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Dialog (", 1, 0, 251 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
         //:nRC = LoadZeidonPPE( vSubtask, vPE, zREFER_PENV_META, CurrentLPLR,
         //:                     "Configuration Management", MG_ErrorMessage )
         nRC = LoadZeidonPPE( vSubtask, &vPE, zREFER_PENV_META, CurrentLPLR, "Configuration Management", MG_ErrorMessage );
         //:DropView( CurrentLPLR )
         DropView( CurrentLPLR );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:RETURN -16
            return( -16 );
         } 

         //:END

         //:// If the Dialog doesn't already exist, create it.
         //:// Otherwise return error message.
         //:nRC = ActivateMetaOI_ByName( vSubtask, NewDialog, 0, zSOURCE_DIALOG_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &NewDialog, 0, zSOURCE_DIALOG_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating Dialog: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Dialog: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:ActivateEmptyMetaOI( vSubtask, NewDialog, zSOURCE_DIALOG_META, zSINGLE )
            ActivateEmptyMetaOI( vSubtask, &NewDialog, zSOURCE_DIALOG_META, zSINGLE );
            //:NAME VIEW NewDialog "NewDialog"
            SetNameForView( NewDialog, "NewDialog", 0, zLEVEL_TASK );
            //:nRC = DialogMigrate( NewDialog, MetaName, OrigLPLR, vSubtask )
            nRC = oTZWDLGSO_DialogMigrate( NewDialog, MetaName, OrigLPLR, vSubtask );
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:MG_ErrorMessage = "Dialog (" + MetaName + ") aborted."
               ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Dialog (", 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
               //:MessageSend( vSubtask, "CM01016",
               //:             "Configuration Management",
               //:             MG_ErrorMessage,
               //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vSubtask, "CM01016", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "Migrating Dialog: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Dialog: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:nRC = DialogMigrate( NewDialog, MetaName, OrigLPLR, vSubtask )
            nRC = oTZWDLGSO_DialogMigrate( NewDialog, MetaName, OrigLPLR, vSubtask );
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:MG_ErrorMessage = "Dialog (" + MetaName + ") aborted."
               ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Dialog (", 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
               ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
               //:MessageSend( vSubtask, "CM01016",
               //:             "Configuration Management",
               //:             MG_ErrorMessage,
               //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vSubtask, "CM01016", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            } 

            //:END
         } 

         //:END
         //:DropMetaOI( vSubtask, NewDialog )
         DropMetaOI( vSubtask, NewDialog );
      } 

      //:END

      //:// UIS: W_MetaType.Type = 2012 for User Interface Spec meta
      //:IF SourceLPLR.W_MetaType.Type = 2012
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2012 ) == 0 )
      { 

         //:// If the User Spec doesn't already exist, create it.
         //:// Otherwise return error message.
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedDialogSpec, 0, zREFER_UIS_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedDialogSpec, 0, zREFER_UIS_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating User Interface Spec: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating User Interface Spec: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:nRC = DialogSpecMigrate( ReturnedDialogSpec,
            //:                         MetaName, OrigLPLR, vSubtask )
            nRC = oTZADCSDO_DialogSpecMigrate( &ReturnedDialogSpec, MetaName, OrigLPLR, vSubtask );
            //:IF ReturnedDialogSpec != 0
            if ( ReturnedDialogSpec != 0 )
            { 
               //:DropMetaOI( vSubtask, ReturnedDialogSpec )
               DropMetaOI( vSubtask, ReturnedDialogSpec );
            } 

            //:END
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:IF  nRC = -16  //PPE does not exists
               if ( nRC == -16 )
               { 
                  //:RETURN nRC
                  return( nRC );
                  //:ELSE
               } 
               else
               { 
                  //:MG_ErrorMessage = "User Interface Spec (" + MetaName + ") aborted."
                  ZeidonStringCopy( MG_ErrorMessage, 1, 0, "User Interface Spec (", 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
                  //:MessageSend( vSubtask, "CM01018",
                  //:             "Configuration Management",
                  //:             MG_ErrorMessage,
                  //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( vSubtask, "CM01018", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               } 

               //:END
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "User Interface Spec (" + MetaName + ") already exists."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "User Interface Spec (", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") already exists.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01019", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01019", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:DropMetaOI( vSubtask, ReturnedDialogSpec )
            DropMetaOI( vSubtask, ReturnedDialogSpec );
         } 

         //:END
      } 

      //:END

      //:// Report: W_MetaType.Type = 2015 for Report Definition meta
      //:IF SourceLPLR.W_MetaType.Type = 2015
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2015 ) == 0 )
      { 

         //:// If the ReportDef doesn't already exist, create it.
         //:// Otherwise return error message.
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedReport, 0, zREFER_REPORT_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedReport, 0, zREFER_REPORT_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating Report Definition: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating Report Definition: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //:nRC = ReportMigrate( ReturnedReport, MetaName, OrigLPLR, vSubtask )
            nRC = oTZRPSRCO_ReportMigrate( &ReturnedReport, MetaName, OrigLPLR, vSubtask );
            //:IF ReturnedReport != 0
            if ( ReturnedReport != 0 )
            { 
               //:DropMetaOI( vSubtask, ReturnedReport )
               DropMetaOI( vSubtask, ReturnedReport );
            } 

            //:END
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:IF  nRC = -16  //PPE does not exists
               if ( nRC == -16 )
               { 
                  //:RETURN nRC
                  return( nRC );
                  //:ELSE
               } 
               else
               { 
                  //:MG_ErrorMessage = "Report Definition (" + MetaName + ") aborted."
                  ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Report Definition (", 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
                  //:MessageSend( vSubtask, "CM01023",
                  //:             "Configuration Management",
                  //:             MG_ErrorMessage,
                  //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( vSubtask, "CM01023", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               } 

               //:END
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "Report Definition (" + MetaName + ") already exists."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Report Definition (", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") already exists.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01024", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01024", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:DropMetaOI( vSubtask, ReturnedReport )
            DropMetaOI( vSubtask, ReturnedReport );
         } 

         //:END
      } 

      //:END

      //:// XSLT: W_MetaType.Type = 2016 for XSLT Definition meta
      //:IF SourceLPLR.W_MetaType.Type = 2016
      if ( CompareAttributeToInteger( SourceLPLR, "W_MetaType", "Type", 2016 ) == 0 )
      { 

         //:// If the XSLTDef doesn't already exist, create it.
         //:// Otherwise return error message.
         //:nRC = ActivateMetaOI_ByName( vSubtask, ReturnedXSLT, 0, zREFER_XSLT_META, zSINGLE, MetaName, 0 )
         nRC = ActivateMetaOI_ByName( vSubtask, &ReturnedXSLT, 0, zREFER_XSLT_META, zSINGLE, MetaName, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MG_ErrorMessage = "Migrating XSLT Definition: " + MetaName
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "Migrating XSLT Definition: ", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            //:MB_SetMessage( vSubtask, 1, MG_ErrorMessage )
            MB_SetMessage( vSubtask, 1, MG_ErrorMessage );
            //://xnRC = XSLT_Migrate( ReturnedXSLT, MetaName, OrigLPLR, vSubtask )
            //:IF ReturnedXSLT != 0
            if ( ReturnedXSLT != 0 )
            { 
               //:DropMetaOI( vSubtask, ReturnedXSLT )
               DropMetaOI( vSubtask, ReturnedXSLT );
            } 

            //:END
            //:IF nRC < 0
            if ( nRC < 0 )
            { 
               //:IF  nRC = -16  //PPE does not exists
               if ( nRC == -16 )
               { 
                  //:RETURN nRC
                  return( nRC );
                  //:ELSE
               } 
               else
               { 
                  //:MG_ErrorMessage = "XSLT Definition (" + MetaName + ") aborted."
                  ZeidonStringCopy( MG_ErrorMessage, 1, 0, "XSLT Definition (", 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
                  ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 251 );
                  //:MessageSend( vSubtask, "CM01023",
                  //:             "Configuration Management",
                  //:             MG_ErrorMessage,
                  //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( vSubtask, "CM01023", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               } 

               //:END
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage = "XSLT Definition (" + MetaName + ") already exists."
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "XSLT Definition (", 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, MetaName, 1, 0, 251 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") already exists.", 1, 0, 251 );
            //:MessageSend( vSubtask, "CM01024", "Configuration Management",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vSubtask, "CM01024", "Configuration Management", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:DropMetaOI( vSubtask, ReturnedXSLT )
            DropMetaOI( vSubtask, ReturnedXSLT );
         } 

         //:END
      } 

      //:END

      //:nRC = SetCursorNextSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" )
      nRC = SetCursorNextSelectedEntity( SourceLPLR, "W_MetaDef", "LPLR" );
   } 


   //:END

   //:MB_SetMessage( vSubtask, 1, "Migration Complete" )
   MB_SetMessage( vSubtask, 1, "Migration Complete" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SelectLPLR( VIEW vSubtask )

//:   VIEW vCM         BASED ON LOD TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
SelectLPLR( zVIEW     vSubtask )
{
   zVIEW     vCM = 0; 
   //:VIEW SourceLPLR  BASED ON LOD TZCMLPLO
   zVIEW     SourceLPLR = 0; 
   //:VIEW OrigLPLR    BASED ON LOD TZCMLPLO
   zVIEW     OrigLPLR = 0; 
   //:VIEW VOR_LPLR    BASED ON LOD TZCMLPLO
   zVIEW     VOR_LPLR = 0; 
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 

   //:STRING ( 513 ) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 32 )  TruncatedName
   zCHAR     TruncatedName[ 33 ] = { 0 }; 
   //:INTEGER        TempType
   zLONG     TempType = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:GET VIEW vCM NAMED "TZCMWKSO"
   RESULT = GetViewByName( &vCM, "TZCMWKSO", vSubtask, zLEVEL_TASK );

   //:// Activate OI and View to the source LPLR
   //:SourceFileName = vCM.LPLR.Name
   GetVariableFromAttribute( SourceFileName, 0, 'S', 514, vCM, "LPLR", "Name", "", 0 );
   //:TruncatedName = SourceFileName [1:32]
   ZeidonStringCopy( TruncatedName, 1, 0, SourceFileName, 1, 32, 33 );
   //:SourceFileName = vCM.LPLR.ExecDir + "\" +
   //:                 TruncatedName + ".XLP"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), vCM, "LPLR", "ExecDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, TruncatedName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".XLP", 1, 0, 514 );
   //:ActivateOI_FromFile( OrigLPLR, "TZCMLPLO", vSubtask,
   //:                     SourceFileName, zSINGLE )
   ActivateOI_FromFile( &OrigLPLR, "TZCMLPLO", vSubtask, SourceFileName, zSINGLE );
   //:NAME VIEW OrigLPLR "OrigLPLR"
   SetNameForView( OrigLPLR, "OrigLPLR", 0, zLEVEL_TASK );

   //:// Create the views that will be positioned on the list of VOR's for VOR migration.
   //:CreateViewFromView( VOR_LPLR, OrigLPLR )
   CreateViewFromView( &VOR_LPLR, OrigLPLR );
   //:NAME VIEW VOR_LPLR "SourceVOR_LPLR"
   SetNameForView( VOR_LPLR, "SourceVOR_LPLR", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST VOR_LPLR.W_MetaType WHERE VOR_LPLR.W_MetaType.Type = 2009  // 2009 is the VOR.
   RESULT = SetCursorFirstEntityByInteger( VOR_LPLR, "W_MetaType", "Type", 2009, "" );
   //:GET VIEW CurrentLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   //:CreateViewFromView( VOR_LPLR, CurrentLPLR )
   CreateViewFromView( &VOR_LPLR, CurrentLPLR );
   //:NAME VIEW VOR_LPLR "TargetVOR_LPLR"
   SetNameForView( VOR_LPLR, "TargetVOR_LPLR", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST VOR_LPLR.W_MetaType WHERE VOR_LPLR.W_MetaType.Type = 2009  // 2009 is the VOR.
   RESULT = SetCursorFirstEntityByInteger( VOR_LPLR, "W_MetaType", "Type", 2009, "" );

   //:// Reposition work station view on original LPLR so that any ZKeys generated during migration will
   //:// be created using the correct next ZKey.
   //:SET CURSOR FIRST vCM.LPLR WHERE vCM.LPLR.Name = CurrentLPLR.LPLR.Name 
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), CurrentLPLR, "LPLR", "Name" );
   RESULT = SetCursorFirstEntityByString( vCM, "LPLR", "Name", szTempString_0, "" );

   //:// Copy the Orig members with Type >= 2000 to Source
   //:ACTIVATE SourceLPLR EMPTY
   RESULT = ActivateEmptyObjectInstance( &SourceLPLR, "TZCMLPLO", vSubtask, zSINGLE );
   //:NAME VIEW SourceLPLR "SourceLPLR"
   SetNameForView( SourceLPLR, "SourceLPLR", 0, zLEVEL_TASK );

   //:CREATE ENTITY SourceLPLR.LPLR
   RESULT = CreateEntity( SourceLPLR, "LPLR", zPOS_AFTER );
   //:SetMatchingAttributesByName( SourceLPLR, "LPLR", OrigLPLR, "LPLR", zSET_ALL )
   SetMatchingAttributesByName( SourceLPLR, "LPLR", OrigLPLR, "LPLR", zSET_ALL );
   //:FOR EACH OrigLPLR.W_MetaType WHERE
   //:         OrigLPLR.W_MetaType.Type >= 2000
   RESULT = SetCursorFirstEntity( OrigLPLR, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      if ( CompareAttributeToInteger( OrigLPLR, "W_MetaType", "Type", 2000 ) >= 0 )
      { 

         //:// Do not include Domains (2003), Global Operations (2002) or
         //:// Registered Views (2009) in the list.
         //:IF OrigLPLR.W_MetaType.Type != 2002 AND
         //:OrigLPLR.W_MetaType.Type != 2003 AND
         //:OrigLPLR.W_MetaType.Type != 2009
         if ( CompareAttributeToInteger( OrigLPLR, "W_MetaType", "Type", 2002 ) != 0 && CompareAttributeToInteger( OrigLPLR, "W_MetaType", "Type", 2003 ) != 0 && CompareAttributeToInteger( OrigLPLR, "W_MetaType", "Type", 2009 ) != 0 )
         { 

            //:CREATE ENTITY SourceLPLR.W_MetaType
            RESULT = CreateEntity( SourceLPLR, "W_MetaType", zPOS_AFTER );
            //:// This code has to use a work integer in setting SourceLPLR.W_MetaType.Type to
            //:// OrigLPLR.W_MetaType.Type, because the domain on Type, CM_Type, seems to always
            //:// convert a REFER value to a SOURCE value.  This is an error in the table domain
            //:// and should eventually get corrected.
            //:TempType = OrigLPLR.W_MetaType.Type
            GetIntegerFromAttribute( &TempType, OrigLPLR, "W_MetaType", "Type" );
            //:SourceLPLR.W_MetaType.Type = TempType
            SetAttributeFromInteger( SourceLPLR, "W_MetaType", "Type", TempType );
            //:FOR EACH OrigLPLR.W_MetaDef
            RESULT = SetCursorFirstEntity( OrigLPLR, "W_MetaDef", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CREATE ENTITY SourceLPLR.W_MetaDef
               RESULT = CreateEntity( SourceLPLR, "W_MetaDef", zPOS_AFTER );
               //:SetMatchingAttributesByName( SourceLPLR, "W_MetaDef", OrigLPLR, "W_MetaDef", zSET_ALL )
               SetMatchingAttributesByName( SourceLPLR, "W_MetaDef", OrigLPLR, "W_MetaDef", zSET_ALL );
               RESULT = SetCursorNextEntity( OrigLPLR, "W_MetaDef", "" );
            } 

            //:END
         } 

      } 

      RESULT = SetCursorNextEntity( OrigLPLR, "W_MetaType", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:MigrateCompilerSubobjects( VIEW vSubtask )

//:   VIEW SourceLPLR    BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
MigrateCompilerSubobjects( zVIEW     vSubtask )
{
   zVIEW     SourceLPLR = 0; 
   //:VIEW CurrentLPLR   BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 
   zSHORT    RESULT; 


   //:GET VIEW SourceLPLR  NAMED "OrigLPLR"
   RESULT = GetViewByName( &SourceLPLR, "OrigLPLR", vSubtask, zLEVEL_TASK );
   //:GET VIEW CurrentLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &CurrentLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:CompilerMigrate( CurrentLPLR, SourceLPLR, vSubtask )
   oTZCMLPLO_CompilerMigrate( CurrentLPLR, SourceLPLR, vSubtask );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CommitTZWDLCVO( VIEW vSubtask )

//:   // Commit the Language object using directory information from the LPLR.
//:   // The file name is ZEIDON.LNG.

//:   VIEW vTZWDLCVO BASED ON LOD TZWDLCVO
zOPER_EXPORT zSHORT OPERATION
CommitTZWDLCVO( zVIEW     vSubtask )
{
   zVIEW     vTZWDLCVO = 0; 
   //:VIEW vLPLR   BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 

   //:STRING ( 513 ) szFileName                // zMAX_FILESPEC_LTH+1
   zCHAR     szFileName[ 514 ] = { 0 }; 
   zSHORT    RESULT; 


   //:GET VIEW vLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   //:GET VIEW vTZWDLCVO NAMED "TZWDLCVO"
   RESULT = GetViewByName( &vTZWDLCVO, "TZWDLCVO", vSubtask, zLEVEL_TASK );

   //:// Before committing the object, remove all the SelectedNLS_Text entity
   //:// entries so that they won't take up room on the file.
   //:FOR EACH vTZWDLCVO.SelectedNLS_Text WITHIN vTZWDLCVO.OriginalLanguage
   RESULT = SetCursorFirstEntity( vTZWDLCVO, "SelectedNLS_Text", "OriginalLanguage" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE vTZWDLCVO.SelectedNLS_Text NONE
      RESULT = ExcludeEntity( vTZWDLCVO, "SelectedNLS_Text", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vTZWDLCVO, "SelectedNLS_Text", "OriginalLanguage" );
   } 

   //:END

   //:szFileName = vLPLR.LPLR.ExecDir + "\ZEIDON.LNG"
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vLPLR, "LPLR", "ExecDir" );
   ZeidonStringConcat( szFileName, 1, 0, "\\ZEIDON.LNG", 1, 0, 514 );
   //:CommitOI_ToFile( vTZWDLCVO, szFileName, zASCII )
   CommitOI_ToFile( vTZWDLCVO, szFileName, zASCII );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:InitializeTZWDLCVO( VIEW vSubtask )

//:   VIEW vTZWDLCVO BASED ON LOD TZWDLCVO
zOPER_EXPORT zSHORT OPERATION
InitializeTZWDLCVO( zVIEW     vSubtask )
{
   zVIEW     vTZWDLCVO = 0; 
   //:VIEW vDialog   BASED ON LOD TZWDLGSO
   zVIEW     vDialog = 0; 
   //:VIEW vLPLR     BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 

   //:STRING (64) szMsg
   zCHAR     szMsg[ 65 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 


   //:GET VIEW vTZWDLCVO NAMED "TZWDLCVO"
   RESULT = GetViewByName( &vTZWDLCVO, "TZWDLCVO", vSubtask, zLEVEL_TASK );
   //:RetrieveViewForMetaList( vSubtask, vLPLR, zREFER_DIALOG_META )
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_DIALOG_META );

   //:// Now loop through all the dialogs, creating new OriginalText entries if
   //:// they don't already exist.

   //:FOR EACH vLPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:  ActivateMetaOI( vSubtask, vDialog, vLPLR, zREFER_DIALOG_META, zSINGLE )
      ActivateMetaOI( vSubtask, &vDialog, vLPLR, zREFER_DIALOG_META, zSINGLE );

      //:szMsg = "Initializing Dialog: " + vDialog.Dialog.Tag
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "Dialog", "Tag", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "Initializing Dialog: ", 1, 0, 65 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 65 );
      //:MB_SetMessage( vSubtask, 1, szMsg )
      MB_SetMessage( vSubtask, 1, szMsg );

      //:FOR EACH vDialog.Window
      RESULT = SetCursorFirstEntity( vDialog, "Window", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:// First process Caption entries for each Window.
         //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
         //:    vTZWDLCVO.OriginalText.Value = vDialog.Window.Caption
         GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Window", "Caption" );
         RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
         //:IF RESULT < zCURSOR_SET AND vDialog.Window.Caption != ""
         if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Window", "Caption", "" ) != 0 )
         { 
            //:CREATE ENTITY vTZWDLCVO.OriginalText
            RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
            //:vTZWDLCVO.OriginalText.Value = vDialog.Window.Caption
            SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Window", "Caption" );
         } 

         //:END

         //:// Next process Option Text entries for each Option.  This requires
         //:// a recursive call.
         //:FOR EACH vDialog.Option
         RESULT = SetCursorFirstEntity( vDialog, "Option", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
            //:     vTZWDLCVO.OriginalText.Value = vDialog.Option.Text
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Option", "Text" );
            RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
            //:IF RESULT < zCURSOR_SET AND vDialog.Option.Text != ""
            if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Option", "Text", "" ) != 0 )
            { 
               //:CREATE ENTITY vTZWDLCVO.OriginalText
               RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
               //:vTZWDLCVO.OriginalText.Value = vDialog.Option.Text
               SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Option", "Text" );
               //:// Call recursive subobject
               //:InitializeOptionSubobject( vTZWDLCVO, vDialog )
               o_InitializeOptionSubobject( vTZWDLCVO, vDialog );
            } 

            RESULT = SetCursorNextEntity( vDialog, "Option", "" );
            //:END
         } 

         //:END

         //:// Next process Control Text entries for each Control.  This requires
         //:// a recursive call.
         //:FOR EACH vDialog.Control
         RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:// First process Text
            //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
            //:     vTZWDLCVO.OriginalText.Value =vDialog.Control.Text
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Control", "Text" );
            RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
            //:IF RESULT < zCURSOR_SET AND vDialog.Control.Text != ""
            if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Control", "Text", "" ) != 0 )
            { 
               //:CREATE ENTITY vTZWDLCVO.OriginalText
               RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
               //:vTZWDLCVO.OriginalText.Value = vDialog.Control.Text
               SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Control", "Text" );
            } 

            //:END
            //:// Next process DIL_Text
            //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
            //:     vTZWDLCVO.OriginalText.Value =vDialog.Control.DIL_Text
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Control", "DIL_Text" );
            RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
            //:IF RESULT < zCURSOR_SET AND vDialog.Control.DIL_Text != ""
            if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Control", "DIL_Text", "" ) != 0 )
            { 
               //:CREATE ENTITY vTZWDLCVO.OriginalText
               RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
               //:vTZWDLCVO.OriginalText.Value = vDialog.Control.DIL_Text
               SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Control", "DIL_Text" );
            } 

            //:END

            //:// Call recursive subobject
            //:InitializeControlSubobject( vTZWDLCVO, vDialog )
            o_InitializeControlSubobject( vTZWDLCVO, vDialog );
            RESULT = SetCursorNextEntity( vDialog, "Control", "" );
         } 

         RESULT = SetCursorNextEntity( vDialog, "Window", "" );
         //:END
      } 

      //:END
      //:DropObjectInstance( vDialog )
      DropObjectInstance( vDialog );
      RESULT = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );
   } 

   //:END

   //:// Now sort them in alphabetical order.
   //:OrderEntityForView( vTZWDLCVO, "OriginalText", "Value A" )
   OrderEntityForView( vTZWDLCVO, "OriginalText", "Value A" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
o_InitializeControlSubobject( zVIEW     vTZWDLCVO,
                              zVIEW     vDialog )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 

   //:InitializeControlSubobject( VIEW vTZWDLCVO BASED ON LOD TZWDLCVO,
   //:                         VIEW vDialog BASED ON LOD TZWDLGSO )

   //:// Initialize the OriginalText values for subcontrols.
   //:SetViewToSubobject( vDialog, "CtrlCtrl" )
   SetViewToSubobject( vDialog, "CtrlCtrl" );
   //:FOR EACH vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// First process Text
      //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
      //:      vTZWDLCVO.OriginalText.Value = vDialog.Control.Text
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialog, "Control", "Text" );
      RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET AND vDialog.Control.Text != ""
      if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Control", "Text", "" ) != 0 )
      { 
         //:CREATE ENTITY vTZWDLCVO.OriginalText
         RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
         //:vTZWDLCVO.OriginalText.Value = vDialog.Control.Text
         SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Control", "Text" );
         //:// Call recursive subobject
         //:InitializeControlSubobject( vTZWDLCVO, vDialog )
         o_InitializeControlSubobject( vTZWDLCVO, vDialog );
      } 

      //:END
      //:// Next process DIL_Text
      //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
      //:      vTZWDLCVO.OriginalText.Value =vDialog.Control.DIL_Text
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialog, "Control", "DIL_Text" );
      RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET AND vDialog.Control.DIL_Text != ""
      if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Control", "DIL_Text", "" ) != 0 )
      { 
         //:CREATE ENTITY vTZWDLCVO.OriginalText
         RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
         //:vTZWDLCVO.OriginalText.Value = vDialog.Control.DIL_Text
         SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Control", "DIL_Text" );
      } 

      RESULT = SetCursorNextEntity( vDialog, "Control", "" );
      //:END
   } 


   //:END
   //:ResetViewFromSubobject( vDialog )
   ResetViewFromSubobject( vDialog );
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
o_InitializeOptionSubobject( zVIEW     vTZWDLCVO,
                             zVIEW     vDialog )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 

   //:InitializeOptionSubobject( VIEW vTZWDLCVO BASED ON LOD TZWDLCVO,
   //:                  VIEW vDialog BASED ON LOD TZWDLGSO )

   //:// Initialize the OriginalText values for subcontrols.
   //:SetViewToSubobject( vDialog, "OptOpt" )
   SetViewToSubobject( vDialog, "OptOpt" );
   //:FOR EACH vDialog.Option
   RESULT = SetCursorFirstEntity( vDialog, "Option", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
      //:      vTZWDLCVO.OriginalText.Value = vDialog.Option.Text
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialog, "Option", "Text" );
      RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET AND vDialog.Option.Text != ""
      if ( RESULT < zCURSOR_SET && CompareAttributeToString( vDialog, "Option", "Text", "" ) != 0 )
      { 
         //:CREATE ENTITY vTZWDLCVO.OriginalText
         RESULT = CreateEntity( vTZWDLCVO, "OriginalText", zPOS_AFTER );
         //:vTZWDLCVO.OriginalText.Value = vDialog.Option.Text
         SetAttributeFromAttribute( vTZWDLCVO, "OriginalText", "Value", vDialog, "Option", "Text" );
         //:// Call recursive subobject
         //:InitializeOptionSubobject( vTZWDLCVO, vDialog )
         o_InitializeOptionSubobject( vTZWDLCVO, vDialog );
      } 

      RESULT = SetCursorNextEntity( vDialog, "Option", "" );
      //:END
   } 

   //:END
   //:ResetViewFromSubobject( vDialog )
   ResetViewFromSubobject( vDialog );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SelectNewLanguageCode( VIEW vSubtask )

//:   VIEW vTZWDLCVO BASED ON LOD TZWDLCVO
zOPER_EXPORT zSHORT OPERATION
SelectNewLanguageCode( zVIEW     vSubtask )
{
   zVIEW     vTZWDLCVO = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 


   //:GET VIEW vTZWDLCVO NAMED "TZWDLCVO"
   RESULT = GetViewByName( &vTZWDLCVO, "TZWDLCVO", vSubtask, zLEVEL_TASK );

   //:// First remove all the current selected SelectedNLS_Text entries.
   //:FOR EACH vTZWDLCVO.SelectedNLS_Text WITHIN vTZWDLCVO.OriginalLanguage
   RESULT = SetCursorFirstEntity( vTZWDLCVO, "SelectedNLS_Text", "OriginalLanguage" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE vTZWDLCVO.SelectedNLS_Text NONE
      RESULT = ExcludeEntity( vTZWDLCVO, "SelectedNLS_Text", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vTZWDLCVO, "SelectedNLS_Text", "OriginalLanguage" );
   } 

   //:END

   //:// Simply return if the new Language Code is 0 (meaning it is null).
   //:IF vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode = 0
   if ( CompareAttributeToInteger( vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode", 0 ) == 0 )
   { 
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Now loop through all the original text entries and include the correct
   //:// NLS_Text entries for the selected language code.
   //:// Also create new NLS_Text entries for OriginalText entries without them.
   //:FOR EACH vTZWDLCVO.OriginalText
   RESULT = SetCursorFirstEntity( vTZWDLCVO, "OriginalText", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
      //:     vTZWDLCVO.NLS_Text.LanguageIndex  =
      //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
      GetIntegerFromAttribute( &lTempInteger_0, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
      RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_0, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CREATE ENTITY vTZWDLCVO.NLS_Text
         RESULT = CreateEntity( vTZWDLCVO, "NLS_Text", zPOS_AFTER );
         //:vTZWDLCVO.NLS_Text.LanguageIndex =
         //:              vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
         SetAttributeFromAttribute( vTZWDLCVO, "NLS_Text", "LanguageIndex", vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
      } 

      //:END
      //:INCLUDE vTZWDLCVO.SelectedNLS_Text FROM vTZWDLCVO.NLS_Text
      RESULT = IncludeSubobjectFromSubobject( vTZWDLCVO, "SelectedNLS_Text", vTZWDLCVO, "NLS_Text", zPOS_AFTER );
      RESULT = SetCursorNextEntity( vTZWDLCVO, "OriginalText", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ActivateTZWDLCVO( VIEW vSubtask )

//:   // Activate the Language object using directory information from the LPLR
//:   // or create it new.  The file name is ZEIDON.LNG.

//:   VIEW vTZWDLCVO BASED ON LOD TZWDLCVO
zOPER_EXPORT zSHORT OPERATION
ActivateTZWDLCVO( zVIEW     vSubtask )
{
   zVIEW     vTZWDLCVO = 0; 
   //:VIEW vLPLR   BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 

   //:STRING ( 513 ) szFileName                // zMAX_FILESPEC_LTH+1
   zCHAR     szFileName[ 514 ] = { 0 }; 
   //:INTEGER        nFileNo
   zLONG     nFileNo = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:szFileName = vLPLR.LPLR.ExecDir + "\ZEIDON.LNG"
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vLPLR, "LPLR", "ExecDir" );
   ZeidonStringConcat( szFileName, 1, 0, "\\ZEIDON.LNG", 1, 0, 514 );
   //:nFileNo = SysOpenFile( vSubtask, szFileName, COREFILE_READ )
   nFileNo = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   //:IF nFileNo > 0
   if ( nFileNo > 0 )
   { 
      //:  ActivateOI_FromFile( vTZWDLCVO, "TZWDLCVO", vSubtask, szFileName, zSINGLE )
      ActivateOI_FromFile( &vTZWDLCVO, "TZWDLCVO", vSubtask, szFileName, zSINGLE );
      //:ELSE
   } 
   else
   { 
      //:ACTIVATE vTZWDLCVO EMPTY
      RESULT = ActivateEmptyObjectInstance( &vTZWDLCVO, "TZWDLCVO", vSubtask, zSINGLE );
      //:CREATE ENTITY vTZWDLCVO.OriginalLanguage
      RESULT = CreateEntity( vTZWDLCVO, "OriginalLanguage", zPOS_AFTER );
   } 

   //:END

   //:NAME VIEW vTZWDLCVO "TZWDLCVO"
   SetNameForView( vTZWDLCVO, "TZWDLCVO", 0, zLEVEL_TASK );

   //:// If there is CurrentSelectedLanguageCode value, use it to initialize
   //:// SelectedNLS_Text entries.
   //:IF vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode != 0
   if ( CompareAttributeToInteger( vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode", 0 ) != 0 )
   { 
      //:SelectNewLanguageCode( vSubtask )
      SelectNewLanguageCode( vSubtask );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ConvertTextInDialogs( VIEW vSubtask )

//:   VIEW vTZWDLCVO BASED ON LOD TZWDLCVO
zOPER_EXPORT zSHORT OPERATION
ConvertTextInDialogs( zVIEW     vSubtask )
{
   zVIEW     vTZWDLCVO = 0; 
   //:VIEW vDialog   BASED ON LOD TZWDLGSO
   zVIEW     vDialog = 0; 
   //:VIEW vLPLR     BASED ON LOD TZCMLPLO
   zVIEW     vLPLR = 0; 

   //:STRING (64) szMsg
   zCHAR     szMsg[ 65 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 


   //:// First make sure that now Dialog meta is currently open.  We will
   //:// check this by seeing if there is a view named "TZWINDOWL".
   //:GET VIEW vDialog NAMED "TZWINDOWL"
   RESULT = GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:MessageSend( vSubtask, "CM01020", "Dialog",
      //:             "You cannot convert text when a dialog is open.",
      //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vSubtask, "CM01020", "Dialog", "You cannot convert text when a dialog is open.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:GET VIEW vTZWDLCVO NAMED "TZWDLCVO"
   RESULT = GetViewByName( &vTZWDLCVO, "TZWDLCVO", vSubtask, zLEVEL_TASK );
   //:RetrieveViewForMetaList( vSubtask, vLPLR, zREFER_DIALOG_META )
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_DIALOG_META );

   //:// Now loop through all the dialogs, converting Caption, Control Text,
   //:// Control DIL_Text and Option Text for each text string that matches
   //:// text in an OriginalText entity.

   //:FOR EACH vLPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:  ActivateMetaOI( vSubtask, vDialog, vLPLR, zSOURCE_DIALOG_META, zSINGLE )
      ActivateMetaOI( vSubtask, &vDialog, vLPLR, zSOURCE_DIALOG_META, zSINGLE );

      //:szMsg = "Converting Dialog: " + vDialog.Dialog.Tag
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "Dialog", "Tag", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "Converting Dialog: ", 1, 0, 65 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 65 );
      //:MB_SetMessage( vSubtask, 1, szMsg )
      MB_SetMessage( vSubtask, 1, szMsg );

      //:FOR EACH vDialog.Window
      RESULT = SetCursorFirstEntity( vDialog, "Window", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:// First process Caption entries for each Window, if there is a match on
         //:// an original text entry.
         //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
         //:    vTZWDLCVO.OriginalText.Value = vDialog.Window.Caption
         GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Window", "Caption" );
         RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:// Then position on the correct NLS_Text entry used for conversion.
            //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
            //:          vTZWDLCVO.NLS_Text.LanguageIndex =
            //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
            GetIntegerFromAttribute( &lTempInteger_0, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
            RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_0, "" );
            //:IF vTZWDLCVO.NLS_Text.Text != ""
            if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
            { 
               //:// Then create the Dialog NLS entry if it doesn't exist.
               //:SET CURSOR FIRST vDialog.CaptionNLS_Text WHERE
               //:vDialog.CaptionNLS_Text.LanguageIndex =
               //:          vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
               GetIntegerFromAttribute( &lTempInteger_1, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
               RESULT = SetCursorFirstEntityByInteger( vDialog, "CaptionNLS_Text", "LanguageIndex", lTempInteger_1, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:CreateMetaEntity( vSubtask, vDialog, "CaptionNLS_Text", zPOS_AFTER )
                  CreateMetaEntity( vSubtask, vDialog, "CaptionNLS_Text", zPOS_AFTER );
               } 

               //:END
               //:// Finally, set the text values.
               //:vDialog.CaptionNLS_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
               SetAttributeFromAttribute( vDialog, "CaptionNLS_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
               //:vDialog.CaptionNLS_Text.Text = vTZWDLCVO.NLS_Text.Text
               SetAttributeFromAttribute( vDialog, "CaptionNLS_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
            } 

            //:END
         } 

         //:END

         //:// Next process Option Text entries for each Option, if there is a match on
         //:// an original text entry.
         //:FOR EACH vDialog.Option
         RESULT = SetCursorFirstEntity( vDialog, "Option", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
            //:     vTZWDLCVO.OriginalText.Value = vDialog.Option.Text
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Option", "Text" );
            RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// Then position on the correct NLS_Text entry used for conversion.
               //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
               //:          vTZWDLCVO.NLS_Text.LanguageIndex =
               //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
               GetIntegerFromAttribute( &lTempInteger_2, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
               RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_2, "" );
               //:IF vTZWDLCVO.NLS_Text.Text != ""
               if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
               { 
                  //:// Then create the Dialog NLS entry if it doesn't exist.
                  //:SET CURSOR FIRST vDialog.OptionNLS_Text WHERE
                  //:     vDialog.OptionNLS_Text.LanguageIndex =
                  //:             vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
                  GetIntegerFromAttribute( &lTempInteger_3, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
                  RESULT = SetCursorFirstEntityByInteger( vDialog, "OptionNLS_Text", "LanguageIndex", lTempInteger_3, "" );
                  //:IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //:CreateMetaEntity( vSubtask, vDialog, "OptionNLS_Text", zPOS_AFTER )
                     CreateMetaEntity( vSubtask, vDialog, "OptionNLS_Text", zPOS_AFTER );
                  } 

                  //:END
                  //:// Finally, set the text values.
                  //:vDialog.OptionNLS_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
                  SetAttributeFromAttribute( vDialog, "OptionNLS_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
                  //:vDialog.OptionNLS_Text.Text = vTZWDLCVO.NLS_Text.Text
                  SetAttributeFromAttribute( vDialog, "OptionNLS_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
               } 

               //:END
            } 

            //:END

            //:// Call recursive subobject
            //:ConvertOptionSubobject( vSubtask, vTZWDLCVO, vDialog )
            o_ConvertOptionSubobject( vSubtask, vTZWDLCVO, vDialog );
            RESULT = SetCursorNextEntity( vDialog, "Option", "" );
         } 

         //:END

         //:// Next process Control Text and DIL_Text entries for each Control,
         //:// if there is a match on an original text entry.
         //:FOR EACH vDialog.Control
         RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 

            //:// Control Text
            //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
            //:     vTZWDLCVO.OriginalText.Value = vDialog.Control.Text
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Control", "Text" );
            RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// Then position on the correct NLS_Text entry used for conversion.
               //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
               //:          vTZWDLCVO.NLS_Text.LanguageIndex =
               //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
               GetIntegerFromAttribute( &lTempInteger_4, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
               RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_4, "" );
               //:IF vTZWDLCVO.NLS_Text.Text != ""
               if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
               { 
                  //:// Then create the Dialog NLS entry if it doesn't exist.
                  //:SET CURSOR FIRST vDialog.ControlNLS_Text WHERE
                  //:     vDialog.ControlNLS_Text.LanguageIndex =
                  //:             vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
                  GetIntegerFromAttribute( &lTempInteger_5, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
                  RESULT = SetCursorFirstEntityByInteger( vDialog, "ControlNLS_Text", "LanguageIndex", lTempInteger_5, "" );
                  //:IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //:CreateMetaEntity( vSubtask, vDialog, "ControlNLS_Text", zPOS_AFTER )
                     CreateMetaEntity( vSubtask, vDialog, "ControlNLS_Text", zPOS_AFTER );
                  } 

                  //:END
                  //:// Finally, set the text values.
                  //:vDialog.ControlNLS_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
                  SetAttributeFromAttribute( vDialog, "ControlNLS_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
                  //:vDialog.ControlNLS_Text.Text = vTZWDLCVO.NLS_Text.Text
                  SetAttributeFromAttribute( vDialog, "ControlNLS_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
               } 

               //:END
            } 

            //:END

            //:// Control DIL_Text
            //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
            //:     vTZWDLCVO.OriginalText.Value = vDialog.Control.DIL_Text
            GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vDialog, "Control", "DIL_Text" );
            RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_1, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// Then position on the correct NLS_Text entry used for conversion.
               //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
               //:          vTZWDLCVO.NLS_Text.LanguageIndex =
               //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
               GetIntegerFromAttribute( &lTempInteger_6, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
               RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_6, "" );
               //:IF vTZWDLCVO.NLS_Text.Text != ""
               if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
               { 
                  //:// Then create the Dialog NLS entry if it doesn't exist.
                  //:SET CURSOR FIRST vDialog.ControlNLS_DIL_Text WHERE
                  //:     vDialog.ControlNLS_DIL_Text.LanguageIndex =
                  //:             vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
                  GetIntegerFromAttribute( &lTempInteger_7, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
                  RESULT = SetCursorFirstEntityByInteger( vDialog, "ControlNLS_DIL_Text", "LanguageIndex", lTempInteger_7, "" );
                  //:IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //:CreateMetaEntity( vSubtask, vDialog, "ControlNLS_DIL_Text", zPOS_AFTER )
                     CreateMetaEntity( vSubtask, vDialog, "ControlNLS_DIL_Text", zPOS_AFTER );
                  } 

                  //:END
                  //:// Finally, set the text values.
                  //:vDialog.ControlNLS_DIL_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
                  SetAttributeFromAttribute( vDialog, "ControlNLS_DIL_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
                  //:vDialog.ControlNLS_DIL_Text.Text = vTZWDLCVO.NLS_Text.Text
                  SetAttributeFromAttribute( vDialog, "ControlNLS_DIL_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
               } 

               //:END
            } 

            //:END

            //:// Call recursive subobject
            //:ConvertControlSubobject( vSubtask, vTZWDLCVO, vDialog )
            o_ConvertControlSubobject( vSubtask, vTZWDLCVO, vDialog );
            RESULT = SetCursorNextEntity( vDialog, "Control", "" );
         } 

         RESULT = SetCursorNextEntity( vDialog, "Window", "" );
         //:END
      } 

      //:END

      //:// Finally, commit the Dialog and drop it.
      //:CommitMetaOI( vSubtask, vDialog, zSOURCE_DIALOG_META )
      CommitMetaOI( vSubtask, vDialog, zSOURCE_DIALOG_META );
      //:DropObjectInstance( vDialog )
      DropObjectInstance( vDialog );
      RESULT = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );
   } 


   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zVOID
o_ConvertControlSubobject( zVIEW     vSubtask,
                           zVIEW     vTZWDLCVO,
                           zVIEW     vDialog )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 

   //:ConvertControlSubobject( VIEW vSubtask,
   //:                      VIEW vTZWDLCVO BASED ON LOD TZWDLCVO,
   //:                      VIEW vDialog BASED ON LOD TZWDLGSO )

   //:// Initialize the OriginalText values for subcontrols.
   //:SetViewToSubobject( vDialog, "CtrlCtrl" )
   SetViewToSubobject( vDialog, "CtrlCtrl" );

   //:// Process Control Text and DIL_Text entries for each Control,
   //:// if there is a match on an original text entry.
   //:FOR EACH vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Control Text
      //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
      //:     vTZWDLCVO.OriginalText.Value = vDialog.Control.Text
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialog, "Control", "Text" );
      RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_0, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Then position on the correct NLS_Text entry used for conversion.
         //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
         //:          vTZWDLCVO.NLS_Text.LanguageIndex =
         //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
         GetIntegerFromAttribute( &lTempInteger_0, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
         RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_0, "" );
         //:IF vTZWDLCVO.NLS_Text.Text != ""
         if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
         { 
            //:// Then create the Dialog NLS entry if it doesn't exist.
            //:SET CURSOR FIRST vDialog.ControlNLS_Text WHERE
            //:     vDialog.ControlNLS_Text.LanguageIndex =
            //:             vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
            GetIntegerFromAttribute( &lTempInteger_1, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
            RESULT = SetCursorFirstEntityByInteger( vDialog, "ControlNLS_Text", "LanguageIndex", lTempInteger_1, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:CreateMetaEntity( vSubtask, vDialog, "ControlNLS_Text", zPOS_AFTER )
               CreateMetaEntity( vSubtask, vDialog, "ControlNLS_Text", zPOS_AFTER );
            } 

            //:END
            //:// Finally, set the text values.
            //:vDialog.ControlNLS_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
            SetAttributeFromAttribute( vDialog, "ControlNLS_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
            //:vDialog.ControlNLS_Text.Text = vTZWDLCVO.NLS_Text.Text
            SetAttributeFromAttribute( vDialog, "ControlNLS_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
         } 

         //:END
      } 

      //:END

      //:// Control DIL_Text
      //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
      //:     vTZWDLCVO.OriginalText.Value = vDialog.Control.DIL_Text
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialog, "Control", "DIL_Text" );
      RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_0, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Then position on the correct NLS_Text entry used for conversion.
         //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
         //:          vTZWDLCVO.NLS_Text.LanguageIndex =
         //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
         GetIntegerFromAttribute( &lTempInteger_2, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
         RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_2, "" );
         //:IF vTZWDLCVO.NLS_Text.Text != ""
         if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
         { 
            //:// Then create the Dialog NLS entry if it doesn't exist.
            //:SET CURSOR FIRST vDialog.ControlNLS_DIL_Text WHERE
            //:     vDialog.ControlNLS_DIL_Text.LanguageIndex =
            //:             vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
            GetIntegerFromAttribute( &lTempInteger_3, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
            RESULT = SetCursorFirstEntityByInteger( vDialog, "ControlNLS_DIL_Text", "LanguageIndex", lTempInteger_3, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:CreateMetaEntity( vSubtask, vDialog, "ControlNLS_DIL_Text", zPOS_AFTER )
               CreateMetaEntity( vSubtask, vDialog, "ControlNLS_DIL_Text", zPOS_AFTER );
            } 

            //:END
            //:// Finally, set the text values.
            //:vDialog.ControlNLS_DIL_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
            SetAttributeFromAttribute( vDialog, "ControlNLS_DIL_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
            //:vDialog.ControlNLS_DIL_Text.Text = vTZWDLCVO.NLS_Text.Text
            SetAttributeFromAttribute( vDialog, "ControlNLS_DIL_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
         } 

         //:END
      } 

      //:END

      //:// Call recursive subobject
      //:ConvertControlSubobject( vSubtask, vTZWDLCVO, vDialog )
      o_ConvertControlSubobject( vSubtask, vTZWDLCVO, vDialog );
      RESULT = SetCursorNextEntity( vDialog, "Control", "" );
   } 

   //:END
   //:ResetViewFromSubobject( vDialog )
   ResetViewFromSubobject( vDialog );
   return;
// END
} 


//:LOCAL OPERATION
static zVOID
o_ConvertOptionSubobject( zVIEW     vSubtask,
                          zVIEW     vTZWDLCVO,
                          zVIEW     vDialog )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 

   //:ConvertOptionSubobject( VIEW vSubtask, VIEW vTZWDLCVO BASED ON LOD TZWDLCVO,
   //:                     VIEW vDialog BASED ON LOD TZWDLGSO )

   //:// Initialize the OriginalText values for subcontrols.
   //:SetViewToSubobject( vDialog, "OptOpt" )
   SetViewToSubobject( vDialog, "OptOpt" );

   //:// Process Option Text entries for each Option, if there is a match on
   //:// an original text entry.
   //:FOR EACH vDialog.Option
   RESULT = SetCursorFirstEntity( vDialog, "Option", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vTZWDLCVO.OriginalText WHERE
      //:     vTZWDLCVO.OriginalText.Value = vDialog.Option.Text
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialog, "Option", "Text" );
      RESULT = SetCursorFirstEntityByString( vTZWDLCVO, "OriginalText", "Value", szTempString_0, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// Then position on the correct NLS_Text entry used for conversion.
         //:SET CURSOR FIRST vTZWDLCVO.NLS_Text WHERE
         //:          vTZWDLCVO.NLS_Text.LanguageIndex =
         //:                 vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
         GetIntegerFromAttribute( &lTempInteger_0, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
         RESULT = SetCursorFirstEntityByInteger( vTZWDLCVO, "NLS_Text", "LanguageIndex", lTempInteger_0, "" );
         //:IF vTZWDLCVO.NLS_Text.Text != ""
         if ( CompareAttributeToString( vTZWDLCVO, "NLS_Text", "Text", "" ) != 0 )
         { 
            //:// Then create the Dialog NLS entry if it doesn't exist.
            //:SET CURSOR FIRST vDialog.OptionNLS_Text WHERE
            //:     vDialog.OptionNLS_Text.LanguageIndex =
            //:             vTZWDLCVO.OriginalLanguage.CurrentSelectedLanguageCode
            GetIntegerFromAttribute( &lTempInteger_1, vTZWDLCVO, "OriginalLanguage", "CurrentSelectedLanguageCode" );
            RESULT = SetCursorFirstEntityByInteger( vDialog, "OptionNLS_Text", "LanguageIndex", lTempInteger_1, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:CreateMetaEntity( vSubtask, vDialog, "OptionNLS_Text", zPOS_AFTER )
               CreateMetaEntity( vSubtask, vDialog, "OptionNLS_Text", zPOS_AFTER );
            } 

            //:END
            //:// Finally, set the text values.
            //:vDialog.OptionNLS_Text.LanguageIndex = vTZWDLCVO.NLS_Text.LanguageIndex
            SetAttributeFromAttribute( vDialog, "OptionNLS_Text", "LanguageIndex", vTZWDLCVO, "NLS_Text", "LanguageIndex" );
            //:vDialog.OptionNLS_Text.Text = vTZWDLCVO.NLS_Text.Text
            SetAttributeFromAttribute( vDialog, "OptionNLS_Text", "Text", vTZWDLCVO, "NLS_Text", "Text" );
         } 

         //:END
      } 

      //:END

      //:// Call recursive subobject
      //:ConvertOptionSubobject( vSubtask, vTZWDLCVO, vDialog )
      o_ConvertOptionSubobject( vSubtask, vTZWDLCVO, vDialog );
      RESULT = SetCursorNextEntity( vDialog, "Option", "" );
   } 

   //:END

   //:ResetViewFromSubobject( vDialog )
   ResetViewFromSubobject( vDialog );
   return;
// END
} 


//:DIALOG OPERATION
//:UpdateLanguageText( VIEW vSubtask )

//:   // This routine simply checks to see if the SelectNLS_Text entries have been
//:   // created and creates them if necessary.

//:   VIEW vTZWDLCVO BASED ON LOD TZWDLCVO
zOPER_EXPORT zSHORT OPERATION
UpdateLanguageText( zVIEW     vSubtask )
{
   zVIEW     vTZWDLCVO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vTZWDLCVO NAMED "TZWDLCVO"
   RESULT = GetViewByName( &vTZWDLCVO, "TZWDLCVO", vSubtask, zLEVEL_TASK );

   //:IF vTZWDLCVO.SelectedNLS_Text DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( vTZWDLCVO, "SelectedNLS_Text" );
   if ( lTempInteger_0 != 0 )
   { 
      //:SelectNewLanguageCode( vSubtask )
      SelectNewLanguageCode( vSubtask );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SelectLPLR_ForSAMerge( VIEW vSubtask )

//:   // The purpose of this routine is to select a particular LPLR which will be
//:   // used as the source of an SA to ER merge.  It is very much the same as the
//:   // SelectLPLR operation above for merging components.  It is simpler, however,
//:   // since the only metas we care about are SAs.

//:   VIEW vCM BASED ON LOD TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
SelectLPLR_ForSAMerge( zVIEW     vSubtask )
{
   zVIEW     vCM = 0; 
   //:VIEW SourceLPLR BASED ON LOD TZCMLPLO
   zVIEW     SourceLPLR = 0; 
   //:VIEW OrigLPLR BASED ON LOD TZCMLPLO
   zVIEW     OrigLPLR = 0; 

   //:STRING ( 513 ) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 32 )  TruncatedName
   zCHAR     TruncatedName[ 33 ] = { 0 }; 
   zSHORT    RESULT; 


   //:GET VIEW vCM NAMED "TZCMWKSO"
   RESULT = GetViewByName( &vCM, "TZCMWKSO", vSubtask, zLEVEL_TASK );

   //:// Activate OI and View to the source LPLR
   //:SourceFileName = vCM.LPLR.Name
   GetVariableFromAttribute( SourceFileName, 0, 'S', 514, vCM, "LPLR", "Name", "", 0 );
   //:TruncatedName = SourceFileName [1:32]
   ZeidonStringCopy( TruncatedName, 1, 0, SourceFileName, 1, 32, 33 );
   //:SourceFileName = vCM.LPLR.ExecDir + "\" +
   //:                 TruncatedName + ".XLP"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), vCM, "LPLR", "ExecDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, TruncatedName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".XLP", 1, 0, 514 );
   //:ActivateOI_FromFile( OrigLPLR, "TZCMLPLO", vSubtask,
   //:                     SourceFileName, zSINGLE )
   ActivateOI_FromFile( &OrigLPLR, "TZCMLPLO", vSubtask, SourceFileName, zSINGLE );
   //:NAME VIEW OrigLPLR "OrigLPLR"
   SetNameForView( OrigLPLR, "OrigLPLR", 0, zLEVEL_TASK );

   //:// Copy the Orig members with Type = 2005 to Source (SA members)
   //:ACTIVATE SourceLPLR EMPTY
   RESULT = ActivateEmptyObjectInstance( &SourceLPLR, "TZCMLPLO", vSubtask, zSINGLE );
   //:NAME VIEW SourceLPLR "SourceLPLR"
   SetNameForView( SourceLPLR, "SourceLPLR", 0, zLEVEL_TASK );

   //:CREATE ENTITY SourceLPLR.LPLR
   RESULT = CreateEntity( SourceLPLR, "LPLR", zPOS_AFTER );
   //:SetMatchingAttributesByName( SourceLPLR, "LPLR", OrigLPLR, "LPLR", zSET_ALL )
   SetMatchingAttributesByName( SourceLPLR, "LPLR", OrigLPLR, "LPLR", zSET_ALL );

   //:SET CURSOR FIRST OrigLPLR.W_MetaType WHERE OrigLPLR.W_MetaType.Type = 2005
   RESULT = SetCursorFirstEntityByInteger( OrigLPLR, "W_MetaType", "Type", 2005, "" );

   //:CREATE ENTITY SourceLPLR.W_MetaType
   RESULT = CreateEntity( SourceLPLR, "W_MetaType", zPOS_AFTER );

   //:SourceLPLR.W_MetaType.Type = 5
   SetAttributeFromInteger( SourceLPLR, "W_MetaType", "Type", 5 );
   //:FOR EACH OrigLPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( OrigLPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY SourceLPLR.W_MetaDef
      RESULT = CreateEntity( SourceLPLR, "W_MetaDef", zPOS_AFTER );
      //:SetMatchingAttributesByName( SourceLPLR, "W_MetaDef", OrigLPLR, "W_MetaDef", zSET_ALL )
      SetMatchingAttributesByName( SourceLPLR, "W_MetaDef", OrigLPLR, "W_MetaDef", zSET_ALL );
      RESULT = SetCursorNextEntity( OrigLPLR, "W_MetaDef", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:MergeSAsToER( VIEW vSubtask )

//:   // This routine sets up the target ERD as necessary and then calls the routine
//:   // that will actually do the merge of the entities from the SA into the ERD.
//:   // If an ERD already exists, it activates it.  If it doesn't exist, it creates
//:   // an new empty one.
//:   // The SAs to be used in the merge are selected in the LPLR object named "SourceLPLR".

//:   VIEW vSourceLPLR  REGISTERED AS SourceLPLR
zOPER_EXPORT zSHORT OPERATION
MergeSAsToER( zVIEW     vSubtask )
{
   zVIEW     vSourceLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCMLPLO     REGISTERED AS TZCMLPLO
   zVIEW     TZCMLPLO = 0; 
   //:VIEW vSourceLPLR2 BASED ON LOD  TZCMLPLO
   zVIEW     vSourceLPLR2 = 0; 
   //:VIEW vCurrentLPLR BASED ON LOD  TZCMLPLO
   zVIEW     vCurrentLPLR = 0; 
   //:VIEW vTargetERD   BASED ON LOD  TZEREMDO
   zVIEW     vTargetERD = 0; 
   //:VIEW vSourceERD   BASED ON LOD  TZEREMDO
   zVIEW     vSourceERD = 0; 
   //:VIEW vSourceSA    BASED ON LOD  TZERSASO
   zVIEW     vSourceSA = 0; 
   //:VIEW vTargetSA    BASED ON LOD  TZERSASO
   zVIEW     vTargetSA = 0; 

   //:STRING ( 513 ) SourceFileName            // zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 33 ) SubjectAreaName
   zCHAR     SubjectAreaName[ 34 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_1[ 2 ]; 

   RESULT = GetViewByName( &vSourceLPLR, "SourceLPLR", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", vSubtask, zLEVEL_TASK );

   //:// First activate the original ERD.
   //:GET VIEW vSourceLPLR2 NAMED "OrigLPLR"
   RESULT = GetViewByName( &vSourceLPLR2, "OrigLPLR", vSubtask, zLEVEL_TASK );
   //:// Position on zREFER_ERD
   //:SET CURSOR FIRST vSourceLPLR2.W_MetaType WHERE vSourceLPLR2.W_MetaType.Type = 2004
   RESULT = SetCursorFirstEntityByInteger( vSourceLPLR2, "W_MetaType", "Type", 2004, "" );

   //:SourceFileName = vSourceLPLR.LPLR.MetaSrcDir + "\" + vSourceLPLR2.W_MetaDef.Name + ".PMD"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), vSourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vSourceLPLR2, "W_MetaDef", "Name", "", 0 );
   ZeidonStringConcat( SourceFileName, 1, 0, szTempString_0, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PMD", 1, 0, 514 );
   //:ActivateOI_FromFile( vSourceERD, "TZEREMDO", vSourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &vSourceERD, "TZEREMDO", vSourceLPLR, SourceFileName, 8192 );
   //:// 8192 IS zIGNORE_ATTRIB_ERRORS
   //:DropView( vSourceLPLR2 )
   DropView( vSourceLPLR2 );
   //:NAME VIEW vSourceERD "***SourceERD"
   SetNameForView( vSourceERD, "***SourceERD", 0, zLEVEL_TASK );

   //:// Then activate the Target ERD as either empty or existing.
   //:RetrieveViewForMetaList( vSubtask, vCurrentLPLR, zREFER_ERD_META )
   RetrieveViewForMetaList( vSubtask, &vCurrentLPLR, zREFER_ERD_META );
   //:IF vCurrentLPLR.W_MetaDef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vCurrentLPLR, "W_MetaDef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:ActivateMetaOI( vSubtask, vTargetERD, vCurrentLPLR, zSOURCE_ERD_META, zSINGLE )
      ActivateMetaOI( vSubtask, &vTargetERD, vCurrentLPLR, zSOURCE_ERD_META, zSINGLE );
      //:ELSE
   } 
   else
   { 
      //:ActivateEmptyMetaOI( vSubtask, vTargetERD, zSOURCE_ERD_META, zSINGLE )
      ActivateEmptyMetaOI( vSubtask, &vTargetERD, zSOURCE_ERD_META, zSINGLE );
      //:CreateMetaEntity( vSubtask, vTargetERD, "EntpER_Model", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vTargetERD, "EntpER_Model", zPOS_AFTER );
      //:vTargetERD.EntpER_Model.Name = TZCMLPLO.LPLR.Name
      SetAttributeFromAttribute( vTargetERD, "EntpER_Model", "Name", TZCMLPLO, "LPLR", "Name" );
   } 

   //:END

   //:// Name it for debug purposes.
   //:NAME VIEW vTargetERD "***TargetERD"
   SetNameForView( vTargetERD, "***TargetERD", 0, zLEVEL_TASK );

   //:nRC = SetCursorFirstSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" )
   nRC = SetCursorFirstSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 
      //:SubjectAreaName = vSourceLPLR.W_MetaDef.Name
      GetVariableFromAttribute( SubjectAreaName, 0, 'S', 34, vSourceLPLR, "W_MetaDef", "Name", "", 0 );
      //:SourceFileName  = vSourceLPLR.LPLR.MetaSrcDir + "\" + SubjectAreaName + ".PSA"
      GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), vSourceLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, SubjectAreaName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, ".PSA", 1, 0, 514 );
      //:ActivateOI_FromFile( vSourceSA, "TZERSASO", vSubtask, SourceFileName, 8192 )
      ActivateOI_FromFile( &vSourceSA, "TZERSASO", vSubtask, SourceFileName, 8192 );
      //:// 8192 is zIGNORE_ATTRIB_ERRORS
      //:NAME VIEW vSourceSA "SourceSA"
      SetNameForView( vSourceSA, "SourceSA", 0, zLEVEL_TASK );

      //:// Now actually do the merge of entities from the source SA into the target ERD.
      //:// We will pass the merge option selected to the merge operation.
      //:nRC = MergeSA_ToER( vTargetERD, vSourceERD, vSourceSA,
      //:                    vSourceLPLR.LPLR.MergeType, vSubtask )
      GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vSourceLPLR, "LPLR", "MergeType" );
      nRC = oTZEREMDO_MergeSA_ToER( vTargetERD, vSourceERD, vSourceSA, szTempString_1, vSubtask );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:// If an error occurred during merge, abort the process.  An error
         //:// message was already send by the merge routine.
         //:RETURN -1
         return( -1 );
      } 

      //:END

      //:nRC = SetCursorNextSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" )
      nRC = SetCursorNextSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" );
   } 

   //:END

   //:DropView( vCurrentLPLR )
   DropView( vCurrentLPLR );

   //:// Commit the ER itself, which will then be used for SA migration.
   //:CommitMetaOI( vSubtask, vTargetERD, zSOURCE_ERD_META )
   CommitMetaOI( vSubtask, vTargetERD, zSOURCE_ERD_META );

   //:// Lastly loop through all the selected SA's again to migrate each
   //:// subject area.
   //:nRC = SetCursorFirstSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" )
   nRC = SetCursorFirstSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" );
   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 
      //:SubjectAreaName = vSourceLPLR.W_MetaDef.Name
      GetVariableFromAttribute( SubjectAreaName, 0, 'S', 34, vSourceLPLR, "W_MetaDef", "Name", "", 0 );
      //:SourceFileName  = vSourceLPLR.LPLR.MetaSrcDir + "\" + SubjectAreaName + ".PSA"
      GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), vSourceLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, SubjectAreaName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName, 1, 0, ".PSA", 1, 0, 514 );
      //:ActivateOI_FromFile( vSourceSA, "TZERSASO", vSubtask, SourceFileName, 8192 )
      ActivateOI_FromFile( &vSourceSA, "TZERSASO", vSubtask, SourceFileName, 8192 );
      //:// 8192 is zIGNORE_ATTRIB_ERRORS
      //:NAME VIEW vSourceSA "SourceSA"
      SetNameForView( vSourceSA, "SourceSA", 0, zLEVEL_TASK );

      //:// Now actually do the merge of entities from the source SA into the target ERD,
      //:// if the SA is not already in the Current LPLR.
      //:nRC = ActivateMetaOI_ByName( vSubtask, vTargetSA, 0, zREFER_SA_META, 0,
      //:                             SubjectAreaName, 0 )
      nRC = ActivateMetaOI_ByName( vSubtask, &vTargetSA, 0, zREFER_SA_META, 0, SubjectAreaName, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:SA_MigrateWithERDs( vTargetSA, vSourceSA,
         //:                    vTargetERD, vSourceERD, vSubtask )
         oTZERSASO_SA_MigrateWithERDs( &vTargetSA, vSourceSA, vTargetERD, vSourceERD, vSubtask );

         //:CommitMetaOI( vSubtask, vTargetSA, zSOURCE_SA_META )
         CommitMetaOI( vSubtask, vTargetSA, zSOURCE_SA_META );
      } 

      //:END

      //:nRC = SetCursorNextSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" )
      nRC = SetCursorNextSelectedEntity( vSourceLPLR, "W_MetaDef", "LPLR" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
RefreshListBoxes( zVIEW     vSubtask )
{

   //:RefreshListBoxes( VIEW vSubtask )

   //:RefreshCtrl( vSubtask, "AttributeList" )
   RefreshCtrl( vSubtask, "AttributeList" );
   //:RefreshCtrl( vSubtask, "RelLinkList" )
   RefreshCtrl( vSubtask, "RelLinkList" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:DropPPE_View( VIEW vSubtask )
//:   SHORT   nRC
zOPER_EXPORT zSHORT OPERATION
DropPPE_View( zVIEW     vSubtask )
{
   zSHORT    nRC = 0; 
   //:VIEW    vPE BASED ON LOD TZPESRCO
   zVIEW     vPE = 0; 


   //:nRC = GetViewByName( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )
   nRC = GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:DropView( vPE )
      DropView( vPE );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:SetCheckoutState( VIEW vSubtask )
//:   SHORT           nEnable
zOPER_EXPORT zSHORT OPERATION
SetCheckoutState( zVIEW     vSubtask )
{
   zSHORT    nEnable = 0; 
   //:STRING ( 100 )  szTitle
   zCHAR     szTitle[ 101 ] = { 0 }; 

   //:VIEW vCurrentLPLR BASED ON LOD  TZCMLPLO
   zVIEW     vCurrentLPLR = 0; 
   //:VIEW SourceLPLR   REGISTERED AS SourceLPLR
   zVIEW     SourceLPLR = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &SourceLPLR, "SourceLPLR", vSubtask, zLEVEL_TASK );

   //:nEnable = 1
   nEnable = 1;

   //:// if ERD not checked out, disable Merge Button for Subject Areas
   //:// and set check out state in title

   //:IF SourceLPLR.W_MetaDef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( SourceLPLR, "W_MetaDef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:// Then activate the Target ERD as either empty or existing.
      //:RetrieveViewForMetaList( vSubtask, vCurrentLPLR, zREFER_ERD_META )
      RetrieveViewForMetaList( vSubtask, &vCurrentLPLR, zREFER_ERD_META );

      //:IF vCurrentLPLR.W_MetaDef EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vCurrentLPLR, "W_MetaDef" );
      if ( lTempInteger_1 == 0 )
      { 
         //:IF vCurrentLPLR.W_MetaDef.Status != 1
         if ( CompareAttributeToInteger( vCurrentLPLR, "W_MetaDef", "Status", 1 ) != 0 )
         { 
            //:nEnable = 0
            nEnable = 0;
            //:szTitle = "Merge Subject Area To Data Model    < Data Model not checked out >"
            ZeidonStringCopy( szTitle, 1, 0, "Merge Subject Area To Data Model    < Data Model not checked out >", 1, 0, 101 );
            //:SetWindowCaptionTitle( vSubtask, szTitle, "" )
            SetWindowCaptionTitle( vSubtask, szTitle, "" );
         } 

         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:nEnable = 0
      nEnable = 0;
   } 

   //:END

   //:SetCtrlState( vSubtask, "Merge", zOPTION_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "Merge", zOPTION_STATUS_ENABLED, (zLONG) nEnable );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CleanUpVORs( VIEW ViewToWindow )

//:   VIEW vCurrentLPLR  BASED ON LOD  TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
CleanUpVORs( zVIEW     ViewToWindow )
{
   zVIEW     vCurrentLPLR = 0; 
   //:VIEW vCurrentLPLRn BASED ON LOD  TZCMLPLO
   zVIEW     vCurrentLPLRn = 0; 
   //:VIEW vVOR          BASED ON LOD  TZWDVORO
   zVIEW     vVOR = 0; 
   //:STRING ( 200 ) SourceFileName
   zCHAR     SourceFileName[ 201 ] = { 0 }; 
   //:STRING ( 200 ) SourceDirectoryName
   zCHAR     SourceDirectoryName[ 201 ] = { 0 }; 
   //:STRING ( 32 ) MetaName
   zCHAR     MetaName[ 33 ] = { 0 }; 
   //:INTEGER       lZKey
   zLONG     lZKey = 0; 
   zSHORT    RESULT; 


   //:// Remove duplicate VORs.
   //:GET VIEW vCurrentLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &vCurrentLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );
   //:SourceDirectoryName = vCurrentLPLR.LPLR.MetaSrcDir
   GetVariableFromAttribute( SourceDirectoryName, 0, 'S', 201, vCurrentLPLR, "LPLR", "MetaSrcDir", "", 0 );
   //:SET CURSOR FIRST vCurrentLPLR.W_MetaType WHERE vCurrentLPLR.W_MetaType.Type = 2009   // 2009 is VOR
   RESULT = SetCursorFirstEntityByInteger( vCurrentLPLR, "W_MetaType", "Type", 2009, "" );
   //:FOR EACH vCurrentLPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( vCurrentLPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateViewFromView( vCurrentLPLRn, vCurrentLPLR )
      CreateViewFromView( &vCurrentLPLRn, vCurrentLPLR );
      //:SET CURSOR NEXT vCurrentLPLRn.W_MetaDef
      RESULT = SetCursorNextEntity( vCurrentLPLRn, "W_MetaDef", "" );
      //:IF vCurrentLPLRn.W_MetaDef.CPLR_ZKey = vCurrentLPLR.W_MetaDef.CPLR_ZKey
      if ( CompareAttributeToAttribute( vCurrentLPLRn, "W_MetaDef", "CPLR_ZKey", vCurrentLPLR, "W_MetaDef", "CPLR_ZKey" ) == 0 )
      { 
         //:IssueError( ViewToWindow,0,0, "Delete Entity" )
         IssueError( ViewToWindow, 0, 0, "Delete Entity" );
         //:DeleteEntity( vCurrentLPLRn, "W_MetaDef", zREPOS_NONE )
         DeleteEntity( vCurrentLPLRn, "W_MetaDef", zREPOS_NONE );
         //:ELSE
      } 
      else
      { 
         //:IF vCurrentLPLRn.W_MetaDef.Name = vCurrentLPLR.W_MetaDef.Name
         if ( CompareAttributeToAttribute( vCurrentLPLRn, "W_MetaDef", "Name", vCurrentLPLR, "W_MetaDef", "Name" ) == 0 )
         { 
            //:// Activate existing source meta VOR
            //:lZKey = vCurrentLPLRn.W_MetaDef.CPLR_ZKey
            GetIntegerFromAttribute( &lZKey, vCurrentLPLRn, "W_MetaDef", "CPLR_ZKey" );
            //:zltoxa( lZKey, MetaName )
            zltoxa( lZKey, MetaName );
            //:SourceFileName = SourceDirectoryName + "\" + MetaName + ".PVR"
            ZeidonStringCopy( SourceFileName, 1, 0, SourceDirectoryName, 1, 0, 201 );
            ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 201 );
            ZeidonStringConcat( SourceFileName, 1, 0, MetaName, 1, 0, 201 );
            ZeidonStringConcat( SourceFileName, 1, 0, ".PVR", 1, 0, 201 );
            //://IssueError( ViewToWindow,0,0, "Delete VOR" )
            //:DeleteMetaOI( ViewToWindow, vCurrentLPLRn, zSOURCE_VOR_META )
            DeleteMetaOI( ViewToWindow, vCurrentLPLRn, zSOURCE_VOR_META );
         } 

         //:END
      } 

      //:END
      //:DropView( vCurrentLPLRn )
      DropView( vCurrentLPLRn );
      RESULT = SetCursorNextEntity( vCurrentLPLR, "W_MetaDef", "" );
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
