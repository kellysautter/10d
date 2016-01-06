#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DIL_InheritedMessage( zVIEW     ViewToInstance,
                                LPVIEWENTITY InternalEntityStructure,
                                LPVIEWATTRIB InternalAttribStructure,
                                zSHORT    GetOrSetFlag );


zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DIL_TextMsg( zVIEW     ViewToInstance,
                       LPVIEWENTITY InternalEntityStructure,
                       LPVIEWATTRIB InternalAttribStructure,
                       zSHORT    GetOrSetFlag );


zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_NLS_ExtValueText( zVIEW     vDG,
                            LPVIEWENTITY lpEntity,
                            LPVIEWATTRIB lpAttribute,
                            zSHORT    GetOrSetFlag );


zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_NLS_DIL_Text( zVIEW     vDG,
                        LPVIEWENTITY lpEntity,
                        LPVIEWATTRIB lpAttribute,
                        zSHORT    GetOrSetFlag );


//:DERIVED ATTRIBUTE OPERATION
//:DIL_InheritedMessage( VIEW ViewToInstance BASED ON LOD TZDGSRCO,
//:                      STRING ( 32 ) InternalEntityStructure,
//:                      STRING ( 32 ) InternalAttribStructure,
//:                      SHORT GetOrSetFlag )

//:// The inherited message is the Desc attribute of the Context, if it exists,
//:// or the Desc attribute of the Domain, if the Context Desc doesn't exist.

//:   STRING (254) DIL_Message
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DIL_InheritedMessage( zVIEW     ViewToInstance,
                                LPVIEWENTITY InternalEntityStructure,
                                LPVIEWATTRIB InternalAttribStructure,
                                zSHORT    GetOrSetFlag )
{
   zCHAR     DIL_Message[ 255 ] = { 0 }; 


   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :

         //:IF ViewToInstance.Context.Desc = ""
         if ( CompareAttributeToString( ViewToInstance, "Context", "Desc", "" ) == 0 )
         { 
            //:DIL_Message = ViewToInstance.Domain.Desc
            GetVariableFromAttribute( DIL_Message, 0, 'S', 255, ViewToInstance, "Domain", "Desc", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:DIL_Message = ViewToInstance.Context.Desc
            GetVariableFromAttribute( DIL_Message, 0, 'S', 255, ViewToInstance, "Context", "Desc", "", 0 );
         } 

         //:END

         //:StoreStringInRecord( ViewToInstance,
         //:                  InternalEntityStructure,
         //:                  InternalAttribStructure,
         //:                  DIL_Message )
         StoreStringInRecord( ViewToInstance, InternalEntityStructure, InternalAttribStructure, DIL_Message );
         break ;
   } 


   //:/* end zDERIVED_GET */

   //:END  /* case */
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DomainMigrate( VIEW          NewDomainGroup BASED ON LOD TZDGSRCO,
//:               STRING ( 32 ) DomainMetaName,
//:               STRING ( 32 ) DomainGroupMetaName,
//:               VIEW          SourceLPLR BASED ON LOD TZCMLPLO,
//:               VIEW          vSubtask )

//:   VIEW OldDomainGroup BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DomainMigrate( zVIEW     NewDomainGroup,
                         zPCHAR    DomainMetaName,
                         zPCHAR    DomainGroupMetaName,
                         zVIEW     SourceLPLR,
                         zVIEW     vSubtask )
{
   zVIEW     OldDomainGroup = 0; 
   //:STRING ( 513 ) SourceFileName                 // size according to zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   zSHORT    RESULT; 



   //:// Activate existing source meta OldDomain Group
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + DomainGroupMetaName + ".PDG"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, DomainGroupMetaName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PDG", 1, 0, 514 );
   //:ActivateOI_FromFile( OldDomainGroup, "TZDGSRCO", SourceLPLR,
   //:                     SourceFileName, 8192 )
   ActivateOI_FromFile( &OldDomainGroup, "TZDGSRCO", SourceLPLR, SourceFileName, 8192 );
   //:// 8192 IS zIGNORE_ATTRIB_ERRORS

   //:NAME VIEW OldDomainGroup "OldDomainGroup"
   SetNameForView( OldDomainGroup, "OldDomainGroup", 0, zLEVEL_TASK );
   //:NAME VIEW NewDomainGroup "NewDomainGroup"
   SetNameForView( NewDomainGroup, "NewDomainGroup", 0, zLEVEL_TASK );

   //:// Position on the correct Domain in the source DomainGroup
   //:SET CURSOR FIRST OldDomainGroup.Domain WHERE
   //:                 OldDomainGroup.Domain.Name = DomainMetaName
   RESULT = SetCursorFirstEntityByString( OldDomainGroup, "Domain", "Name", DomainMetaName, "" );

   //:// Call operation to actually do the copy of the Domain.
   //:DomainMigrateReus( NewDomainGroup, OldDomainGroup, vSubtask )
   oTZDGSRCO_DomainMigrateReus( NewDomainGroup, OldDomainGroup, vSubtask );

   //:// Build the executable.
   //:oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, NewDomainGroup, 1 )
   oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, NewDomainGroup, 1 );

   //:DropObjectInstance( OldDomainGroup )
   DropObjectInstance( OldDomainGroup );
   //:CommitMetaOI( vSubtask, NewDomainGroup, 13 )  // 13 is zSOURCE_DOMAINGRP_META
   CommitMetaOI( vSubtask, NewDomainGroup, 13 );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DomainMigrateReus( zVIEW     NewDomainGroup,
                             zVIEW     OldDomainGroup,
                             zVIEW     vSubtask )
{
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 

   //:DomainMigrateReus( VIEW NewDomainGroup BASED ON LOD TZDGSRCO,
   //:                VIEW OldDomainGroup BASED ON LOD TZDGSRCO,
   //:                VIEW vSubtask )

   //:// If the source Domain has an Operation subobject and the target does not, add it.
   //:IF OldDomainGroup.SelectedOperation EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( OldDomainGroup, "SelectedOperation" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SET CURSOR FIRST NewDomainGroup.Operation WHERE
      //:                 NewDomainGroup.Operation.Name = OldDomainGroup.SelectedOperation.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDomainGroup, "SelectedOperation", "Name" );
      RESULT = SetCursorFirstEntityByString( NewDomainGroup, "Operation", "Name", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR LAST NewDomainGroup.Operation
         RESULT = SetCursorLastEntity( NewDomainGroup, "Operation", "" );
         //:CreateMetaEntity( vSubtask, NewDomainGroup, "Operation", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewDomainGroup, "Operation", zPOS_AFTER );
         //:SetMatchingAttributesByName ( NewDomainGroup, "Operation",
         //:                              OldDomainGroup, "Operation", zSET_NULL )
         SetMatchingAttributesByName( NewDomainGroup, "Operation", OldDomainGroup, "Operation", zSET_NULL );
      } 

      //:END
   } 

   //:END

   //:CreateMetaEntity( vSubtask, NewDomainGroup, "Domain", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewDomainGroup, "Domain", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewDomainGroup, "Domain",
   //:                             OldDomainGroup, "Domain", zSET_NULL )
   SetMatchingAttributesByName( NewDomainGroup, "Domain", OldDomainGroup, "Domain", zSET_NULL );

   //:FOR EACH OldDomainGroup.Context
   RESULT = SetCursorFirstEntity( OldDomainGroup, "Context", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:CreateMetaEntity( vSubtask, NewDomainGroup, "Context", zPOS_AFTER )
      CreateMetaEntity( vSubtask, NewDomainGroup, "Context", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewDomainGroup, "Context",
      //:                             OldDomainGroup, "Context", zSET_NULL )
      SetMatchingAttributesByName( NewDomainGroup, "Context", OldDomainGroup, "Context", zSET_NULL );
      //:FOR EACH OldDomainGroup.TableEntry
      RESULT = SetCursorFirstEntity( OldDomainGroup, "TableEntry", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( vSubtask, NewDomainGroup, "TableEntry", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewDomainGroup, "TableEntry", zPOS_AFTER );
         //:SetMatchingAttributesByName( NewDomainGroup, "TableEntry",
         //:                             OldDomainGroup, "TableEntry", zSET_NULL )
         SetMatchingAttributesByName( NewDomainGroup, "TableEntry", OldDomainGroup, "TableEntry", zSET_NULL );
         RESULT = SetCursorNextEntity( OldDomainGroup, "TableEntry", "" );
      } 

      //:END

      //:FOR EACH OldDomainGroup.RegularExpression
      RESULT = SetCursorFirstEntity( OldDomainGroup, "RegularExpression", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( vSubtask, NewDomainGroup, "RegularExpression", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewDomainGroup, "RegularExpression", zPOS_AFTER );
         //:SetMatchingAttributesByName( NewDomainGroup, "RegularExpression",
         //:                             OldDomainGroup, "RegularExpression", zSET_NULL )
         SetMatchingAttributesByName( NewDomainGroup, "RegularExpression", OldDomainGroup, "RegularExpression", zSET_NULL );
         RESULT = SetCursorNextEntity( OldDomainGroup, "RegularExpression", "" );
      } 

      RESULT = SetCursorNextEntity( OldDomainGroup, "Context", "" );
      //:END
   } 


   //:END

   //:IF OldDomainGroup.SelectedOperation EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( OldDomainGroup, "SelectedOperation" );
   if ( lTempInteger_1 == 0 )
   { 
      //:SET CURSOR FIRST NewDomainGroup.Operation WHERE
      //:                 NewDomainGroup.Operation.Name = OldDomainGroup.SelectedOperation.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDomainGroup, "SelectedOperation", "Name" );
      RESULT = SetCursorFirstEntityByString( NewDomainGroup, "Operation", "Name", szTempString_0, "" );
      //:INCLUDE NewDomainGroup.SelectedOperation FROM NewDomainGroup.Operation
      RESULT = IncludeSubobjectFromSubobject( NewDomainGroup, "SelectedOperation", NewDomainGroup, "Operation", zPOS_AFTER );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DomainAddForMerge( VIEW           NewDomainGroup BASED ON LOD TZDGSRCO,
//:                   VIEW           SourceLPLR     BASED ON LOD TZCMLPLO,
//:                   VIEW           CurrentLPLR    BASED ON LOD TZCMLPLO,
//:                   STRING ( 513 ) SourceDirectoryName,
//:                   STRING ( 32 )  DomainName,
//:                   VIEW           vSubtask )

//:   VIEW OldDomainGroup BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DomainAddForMerge( zPVIEW    NewDomainGroup,
                             zVIEW     SourceLPLR,
                             zVIEW     CurrentLPLR,
                             zPCHAR    SourceDirectoryName,
                             zPCHAR    DomainName,
                             zVIEW     vSubtask )
{
   zVIEW     OldDomainGroup = 0; 
   //:STRING ( 513 ) SourceFileName                 // size according to zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 32 )  DomainGroupMetaName
   zCHAR     DomainGroupMetaName[ 33 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:// Activate existing source meta OldDomainGroup. To do this, we must locate the Domain and DomainGroup in the  SourceLPLR.
   //:SET CURSOR FIRST SourceLPLR.W_MetaType WHERE SourceLPLR.W_MetaType.Type = 2003    // 2003 is Domain
   RESULT = SetCursorFirstEntityByInteger( SourceLPLR, "W_MetaType", "Type", 2003, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "Invalid Source XLP."
      ZeidonStringCopy( szMsg, 1, 0, "Invalid Source XLP.", 1, 0, 201 );
      //:MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:SET CURSOR FIRST SourceLPLR.W_MetaDef WHERE SourceLPLR.W_MetaDef.Name = DomainName
   RESULT = SetCursorFirstEntityByString( SourceLPLR, "W_MetaDef", "Name", DomainName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "Domain (" + DomainName + ") was not found in Source LPLR."
      ZeidonStringCopy( szMsg, 1, 0, "Domain (", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, DomainName, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ") was not found in Source LPLR.", 1, 0, 201 );
      //:MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:DomainGroupMetaName = SourceLPLR.W_MetaDef.GroupName
   GetVariableFromAttribute( DomainGroupMetaName, 0, 'S', 33, SourceLPLR, "W_MetaDef", "GroupName", "", 0 );
   //:SourceFileName = SourceDirectoryName + "\" + DomainGroupMetaName + ".PDG"
   ZeidonStringCopy( SourceFileName, 1, 0, SourceDirectoryName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, DomainGroupMetaName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PDG", 1, 0, 514 );
   //:nRC = ActivateOI_FromFile( OldDomainGroup, "TZDGSRCO", SourceLPLR, SourceFileName, 8192 ) // 8192 IS zIGNORE_ATTRIB_ERRORS
   nRC = ActivateOI_FromFile( &OldDomainGroup, "TZDGSRCO", SourceLPLR, SourceFileName, 8192 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:szMsg = "Domain Group (" + DomainGroupMetaName + ") was not found in Source LPLR."
      ZeidonStringCopy( szMsg, 1, 0, "Domain Group (", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, DomainGroupMetaName, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ") was not found in Source LPLR.", 1, 0, 201 );
      //:MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Activate the Target Domain Group which will contain the new Domain.
   //:nRC = ActivateMetaOI_ByName( vSubtask, NewDomainGroup, 0, zSOURCE_DOMAINGRP_META, zSINGLE, DomainGroupMetaName, 0 )
   nRC = ActivateMetaOI_ByName( vSubtask, NewDomainGroup, 0, zSOURCE_DOMAINGRP_META, zSINGLE, DomainGroupMetaName, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:szMsg = "Domain Group (" + DomainGroupMetaName + ") was not found in Target LPLR."
      ZeidonStringCopy( szMsg, 1, 0, "Domain Group (", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, DomainGroupMetaName, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ") was not found in Target LPLR.", 1, 0, 201 );
      //:MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "", "Add/Merge Domain", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:NAME VIEW OldDomainGroup "OldDomainGroup"
   SetNameForView( OldDomainGroup, "OldDomainGroup", 0, zLEVEL_TASK );
   //:NAME VIEW NewDomainGroup "NewDomainGroup"
   SetNameForView( *NewDomainGroup, "NewDomainGroup", 0, zLEVEL_TASK );

   //:// Position on the correct Domain in the source DomainGroup
   //:SET CURSOR FIRST OldDomainGroup.Domain WHERE OldDomainGroup.Domain.Name = DomainName
   RESULT = SetCursorFirstEntityByString( OldDomainGroup, "Domain", "Name", DomainName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:IssueError( SourceLPLR,0,0, "Programming Error: Domain Name not in Domain Group" )
      IssueError( SourceLPLR, 0, 0, "Programming Error: Domain Name not in Domain Group" );
   } 

   //:END

   //:// Call operation to actually do the copy of the Domain.
   //:DomainMigrateReus( NewDomainGroup, OldDomainGroup, vSubtask )
   oTZDGSRCO_DomainMigrateReus( *NewDomainGroup, OldDomainGroup, vSubtask );

   //:// Prompt User for Add of Domain.
   //:szMsg = "Domain, " + DomainName + ", has been added. Do you want it commited to the new LPLR?"
   ZeidonStringCopy( szMsg, 1, 0, "Domain, ", 1, 0, 201 );
   ZeidonStringConcat( szMsg, 1, 0, DomainName, 1, 0, 201 );
   ZeidonStringConcat( szMsg, 1, 0, ", has been added. Do you want it commited to the new LPLR?", 1, 0, 201 );
   //:nRC = MessagePrompt( vSubtask, "", "Add/Merge Domain", szMsg, 1, zBUTTONS_YESNO, zRESPONSE_YES, 0 )
   nRC = MessagePrompt( vSubtask, "", "Add/Merge Domain", szMsg, 1, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
   //:IF nRC = zRESPONSE_YES
   if ( nRC == zRESPONSE_YES )
   { 
      //:CommitMetaOI( vSubtask, NewDomainGroup, 13 )  // 13 is zSOURCE_DOMAINGRP_META
      CommitMetaOI( vSubtask, *NewDomainGroup, 13 );
      //:ELSE
   } 
   else
   { 
      //:// Indicate Domain wasn't saved, so that we won't save the LOD or ER.
      //:CurrentLPLR.LPLR.wMergeComponentError = "Y"
      SetAttributeFromString( CurrentLPLR, "LPLR", "wMergeComponentError", "Y" );
   } 

   //:END

   //:DropObjectInstance( OldDomainGroup )
   DropObjectInstance( OldDomainGroup );
   return( 0 );
// END
} 


//:DERIVED ATTRIBUTE OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DIL_TextMsg( zVIEW     ViewToInstance,
                       LPVIEWENTITY InternalEntityStructure,
                       LPVIEWATTRIB InternalAttribStructure,
                       zSHORT    GetOrSetFlag )
{

   //:DIL_TextMsg( VIEW ViewToInstance BASED ON LOD TZDGSRCO,
   //:          STRING ( 32 ) InternalEntityStructure,
   //:          STRING ( 32 ) InternalAttribStructure,
   //:          SHORT GetOrSetFlag )

   //://   This derived attribute is only temporary.  It and the associated derived
   //://   attribute should be deleted when no portable files exist with values for
   //://   this attribute.

   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :

         //:StoreStringInRecord( ViewToInstance,
         //:                  InternalEntityStructure,
         //:                  InternalAttribStructure,
         //:                  "" )
         StoreStringInRecord( ViewToInstance, InternalEntityStructure, InternalAttribStructure, "" );
         break ;
   } 


   //:/* end zDERIVED_GET */

   //:END  /* case */
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DomainGrpMigrate( VIEW        NewDomainGroup BASED ON LOD TZDGSRCO,
//:                  STRING (32) DomainGroupMetaName,
//:                  VIEW        SourceLPLR BASED ON LOD TZCMLPLO,
//:                  VIEW        vSubtask )

//:   VIEW OldDomainGroup BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_DomainGrpMigrate( zVIEW     NewDomainGroup,
                            zPCHAR    DomainGroupMetaName,
                            zVIEW     SourceLPLR,
                            zVIEW     vSubtask )
{
   zVIEW     OldDomainGroup = 0; 
   //:VIEW CurrentLPLR  BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 

   //:STRING ( 513 ) SourceFileName                 // size according to zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) SourceFileName2                // size according to zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName2[ 514 ] = { 0 }; 
   //:STRING ( 513 ) Msg                            // size according to zMAX_MESSAGE_LTH+1
   zCHAR     Msg[ 514 ] = { 0 }; 
   //:STRING ( 129 ) SourceName                     // size according to zBASE_FILENAME_LTH+1
   zCHAR     SourceName[ 130 ] = { 0 }; 

   //:STRING ( 32 )  SourceMetaName
   zCHAR     SourceMetaName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  ContextName
   zCHAR     ContextName[ 33 ] = { 0 }; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 


   //:// Activate existing source meta OldDomain Group
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + DomainGroupMetaName + ".PDG"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, DomainGroupMetaName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PDG", 1, 0, 514 );
   //:ActivateOI_FromFile ( OldDomainGroup, "TZDGSRCO", SourceLPLR,
   //:                      SourceFileName, 8192 )
   ActivateOI_FromFile( &OldDomainGroup, "TZDGSRCO", SourceLPLR, SourceFileName, 8192 );
   //:// 8192 IS zIGNORE_ATTRIB_ERRORS

   //:NAME VIEW OldDomainGroup "OldDomainGroup"
   SetNameForView( OldDomainGroup, "OldDomainGroup", 0, zLEVEL_TASK );
   //:NAME VIEW NewDomainGroup "NewDomainGroup"
   SetNameForView( NewDomainGroup, "NewDomainGroup", 0, zLEVEL_TASK );

   //:IF NewDomainGroup.DomainGroup DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( NewDomainGroup, "DomainGroup" );
   if ( lTempInteger_0 != 0 )
   { 

      //:// New Domain Group
      //:CreateMetaEntity( vSubtask, NewDomainGroup, "DomainGroup", zPOS_AFTER )
      CreateMetaEntity( vSubtask, NewDomainGroup, "DomainGroup", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewDomainGroup, "DomainGroup", OldDomainGroup,
      //:                             "DomainGroup", zSET_NULL )
      SetMatchingAttributesByName( NewDomainGroup, "DomainGroup", OldDomainGroup, "DomainGroup", zSET_NULL );

      //:// Copy over the source file to the new LPLR directory, if there is a source file.
      //:// We will assume a source file exists if the SourceFile.Extension is not null.  If there
      //:// is actually no source file, the CopyFile does no harm.
      //:IF OldDomainGroup.DomainGroup.Extension != ""
      if ( CompareAttributeToString( OldDomainGroup, "DomainGroup", "Extension", "" ) != 0 )
      { 
         //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zSOURCE_ERD_META ) // Get view for directory info.
         RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zSOURCE_ERD_META );
         //:ResetViewFromSubobject( CurrentLPLR )
         ResetViewFromSubobject( CurrentLPLR );
         //:SourceMetaName = NewDomainGroup.DomainGroup.Name
         GetVariableFromAttribute( SourceMetaName, 0, 'S', 33, NewDomainGroup, "DomainGroup", "Name", "", 0 );
         //:IF OldDomainGroup.DomainGroup.Extension = "C"
         if ( CompareAttributeToString( OldDomainGroup, "DomainGroup", "Extension", "C" ) == 0 )
         { 
            //:SourceName = SourceMetaName + ".C"
            ZeidonStringCopy( SourceName, 1, 0, SourceMetaName, 1, 0, 130 );
            ZeidonStringConcat( SourceName, 1, 0, ".C", 1, 0, 130 );
            //:ELSE
         } 
         else
         { 
            //:SourceName = SourceMetaName + ".VML"
            ZeidonStringCopy( SourceName, 1, 0, SourceMetaName, 1, 0, 130 );
            ZeidonStringConcat( SourceName, 1, 0, ".VML", 1, 0, 130 );
         } 

         //:END
         //:SourceFileName = SourceLPLR.LPLR.PgmSrcDir + "\" + SourceName
         GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "PgmSrcDir" );
         ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
         ZeidonStringConcat( SourceFileName, 1, 0, SourceName, 1, 0, 514 );
         //:SourceFileName2 = CurrentLPLR.LPLR.PgmSrcDir + "\" + SourceName
         GetStringFromAttribute( SourceFileName2, sizeof( SourceFileName2 ), CurrentLPLR, "LPLR", "PgmSrcDir" );
         ZeidonStringConcat( SourceFileName2, 1, 0, "\\", 1, 0, 514 );
         ZeidonStringConcat( SourceFileName2, 1, 0, SourceName, 1, 0, 514 );
         //:SysCopyFile( vSubtask, SourceFileName, SourceFileName2, TRUE )
         SysCopyFile( vSubtask, SourceFileName, SourceFileName2, TRUE );
         //:DropMetaOI( vSubtask, CurrentLPLR )
         DropMetaOI( vSubtask, CurrentLPLR );
      } 

      //:END
   } 


   //:END

   //:FOR EACH OldDomainGroup.Operation
   RESULT = SetCursorFirstEntity( OldDomainGroup, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST NewDomainGroup.Operation WHERE NewDomainGroup.Operation.Name = OldDomainGroup.Operation.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDomainGroup, "Operation", "Name" );
      RESULT = SetCursorFirstEntityByString( NewDomainGroup, "Operation", "Name", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR LAST NewDomainGroup.Operation
         RESULT = SetCursorLastEntity( NewDomainGroup, "Operation", "" );
         //:CreateMetaEntity( vSubtask, NewDomainGroup, "Operation", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewDomainGroup, "Operation", zPOS_AFTER );
         //:SetMatchingAttributesByName( NewDomainGroup, "Operation", OldDomainGroup,
         //:                             "Operation", zSET_NULL )
         SetMatchingAttributesByName( NewDomainGroup, "Operation", OldDomainGroup, "Operation", zSET_NULL );
      } 

      RESULT = SetCursorNextEntity( OldDomainGroup, "Operation", "" );
      //:END
   } 

   //:END

   //:// Migrate each Domain/Context not in the target LPLR.
   //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zREFER_DOMAIN_META ) // Get view for domain info.
   RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zREFER_DOMAIN_META );
   //:FOR EACH OldDomainGroup.Domain
   RESULT = SetCursorFirstEntity( OldDomainGroup, "Domain", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// Add any Domains and/or Contexts in the Source LPLR not in the Target LPLR.
      //:SET CURSOR FIRST CurrentLPLR.W_MetaDef WHERE CurrentLPLR.W_MetaDef.Name = OldDomainGroup.Domain.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDomainGroup, "Domain", "Name" );
      RESULT = SetCursorFirstEntityByString( CurrentLPLR, "W_MetaDef", "Name", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CreateMetaEntity( vSubtask, NewDomainGroup, "Domain", zPOS_AFTER )
         CreateMetaEntity( vSubtask, NewDomainGroup, "Domain", zPOS_AFTER );
         //:SetMatchingAttributesByName( NewDomainGroup, "Domain", OldDomainGroup,
         //:                             "Domain", zSET_NULL )
         SetMatchingAttributesByName( NewDomainGroup, "Domain", OldDomainGroup, "Domain", zSET_NULL );
      } 

      //:END

      //:FOR EACH OldDomainGroup.Context
      RESULT = SetCursorFirstEntity( OldDomainGroup, "Context", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:ContextName = OldDomainGroup.Context.Name
         GetVariableFromAttribute( ContextName, 0, 'S', 33, OldDomainGroup, "Context", "Name", "", 0 );
         //:SET CURSOR FIRST NewDomainGroup.Context WHERE NewDomainGroup.Context.Name = ContextName
         RESULT = SetCursorFirstEntityByString( NewDomainGroup, "Context", "Name", ContextName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CreateMetaEntity( vSubtask, NewDomainGroup, "Context", zPOS_AFTER )
            CreateMetaEntity( vSubtask, NewDomainGroup, "Context", zPOS_AFTER );
            //:SetMatchingAttributesByName( NewDomainGroup, "Context", OldDomainGroup, "Context", zSET_NULL )
            SetMatchingAttributesByName( NewDomainGroup, "Context", OldDomainGroup, "Context", zSET_NULL );

            //:FOR EACH OldDomainGroup.TableEntry
            RESULT = SetCursorFirstEntity( OldDomainGroup, "TableEntry", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( vSubtask, NewDomainGroup, "TableEntry", zPOS_AFTER )
               CreateMetaEntity( vSubtask, NewDomainGroup, "TableEntry", zPOS_AFTER );
               //:SetMatchingAttributesByName( NewDomainGroup, "TableEntry", OldDomainGroup,
               //:                             "TableEntry", zSET_NULL )
               SetMatchingAttributesByName( NewDomainGroup, "TableEntry", OldDomainGroup, "TableEntry", zSET_NULL );
               RESULT = SetCursorNextEntity( OldDomainGroup, "TableEntry", "" );
            } 

            //:END

            //:FOR EACH OldDomainGroup.RegularExpression
            RESULT = SetCursorFirstEntity( OldDomainGroup, "RegularExpression", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( vSubtask, NewDomainGroup, "RegularExpression", zPOS_AFTER )
               CreateMetaEntity( vSubtask, NewDomainGroup, "RegularExpression", zPOS_AFTER );
               //:SetMatchingAttributesByName( NewDomainGroup, "RegularExpression", OldDomainGroup, "RegularExpression", zSET_NULL )
               SetMatchingAttributesByName( NewDomainGroup, "RegularExpression", OldDomainGroup, "RegularExpression", zSET_NULL );
               RESULT = SetCursorNextEntity( OldDomainGroup, "RegularExpression", "" );
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( OldDomainGroup, "Context", "" );
         //:END
      } 


      //:END

      //:IF OldDomainGroup.SelectedOperation EXISTS AND NewDomainGroup.SelectedOperation DOES NOT EXIST
      lTempInteger_1 = CheckExistenceOfEntity( OldDomainGroup, "SelectedOperation" );
      lTempInteger_2 = CheckExistenceOfEntity( NewDomainGroup, "SelectedOperation" );
      if ( lTempInteger_1 == 0 && lTempInteger_2 != 0 )
      { 
         //:SET CURSOR FIRST NewDomainGroup.Operation WHERE
         //:                 NewDomainGroup.Operation.Name = OldDomainGroup.SelectedOperation.Name
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDomainGroup, "SelectedOperation", "Name" );
         RESULT = SetCursorFirstEntityByString( NewDomainGroup, "Operation", "Name", szTempString_0, "" );
         //:INCLUDE NewDomainGroup.SelectedOperation FROM NewDomainGroup.Operation
         RESULT = IncludeSubobjectFromSubobject( NewDomainGroup, "SelectedOperation", NewDomainGroup, "Operation", zPOS_AFTER );
      } 

      //:END

      //:// Build the executable.
      //:oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, NewDomainGroup, 1 )
      oTZDMSRCO_ReplaceOrAddDomToXDM( vSubtask, NewDomainGroup, 1 );
      RESULT = SetCursorNextEntity( OldDomainGroup, "Domain", "" );
   } 


   //:END

   //:DropObjectInstance( OldDomainGroup )
   DropObjectInstance( OldDomainGroup );
   //:CommitMetaOI( vSubtask, NewDomainGroup, 13 )  // 13 is zSOURCE_DOMAINGRP_META
   CommitMetaOI( vSubtask, NewDomainGroup, 13 );
   return( 0 );
// END
} 


//:DERIVED ATTRIBUTE OPERATION
//:NLS_ExtValueText( VIEW        vDG BASED ON LOD TZDGSRCO,
//:                  STRING (32) lpEntity,
//:                  STRING (32) lpAttribute,
//:                  SHORT       GetOrSetFlag )

//:   SHORT        nLanguageCode
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_NLS_ExtValueText( zVIEW     vDG,
                            LPVIEWENTITY lpEntity,
                            LPVIEWATTRIB lpAttribute,
                            zSHORT    GetOrSetFlag )
{
   zSHORT    nLanguageCode = 0; 
   //:STRING (254) szText
   zCHAR     szText[ 255 ] = { 0 }; 
   zSHORT    RESULT; 


   //:SysGetLanguageCode( nLanguageCode )
   SysGetLanguageCode( &nLanguageCode );

   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :

         //:// This is basically the same algorithm as the NLS_Text operations in
         //:// TZWDLG2O.VML.  See that code for an explanation.

         //:SET CURSOR FIRST vDG.ExtValueNLS_Text WHERE
         //:              vDG.ExtValueNLS_Text.LanguageIndex = nLanguageCode
         RESULT = SetCursorFirstEntityByInteger( vDG, "ExtValueNLS_Text", "LanguageIndex", nLanguageCode, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szText = vDG.ExtValueNLS_Text.Text
            GetVariableFromAttribute( szText, 0, 'S', 255, vDG, "ExtValueNLS_Text", "Text", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szText = vDG.TableEntry.ExternalValue
            GetVariableFromAttribute( szText, 0, 'S', 255, vDG, "TableEntry", "ExternalValue", "", 0 );
         } 

         //:END

         //:StoreStringInRecord( vDG, lpEntity, lpAttribute, szText )
         StoreStringInRecord( vDG, lpEntity, lpAttribute, szText );
         break ;

      //:  /* end zDERIVED_GET */
      //:OF   zDERIVED_SET:
      case zDERIVED_SET :

         //:GetStringFromRecord( vDG, lpEntity, lpAttribute, szText, 254 )
         GetStringFromRecord( vDG, lpEntity, lpAttribute, szText, 254 );

         //:SET CURSOR FIRST vDG.ExtValueNLS_Text WHERE
         //:              vDG.ExtValueNLS_Text.LanguageIndex = nLanguageCode
         RESULT = SetCursorFirstEntityByInteger( vDG, "ExtValueNLS_Text", "LanguageIndex", nLanguageCode, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:vDG.ExtValueNLS_Text.Text = szText
            SetAttributeFromString( vDG, "ExtValueNLS_Text", "Text", szText );
            //:ELSE
         } 
         else
         { 
            //:vDG.TableEntry.ExternalValue = szText
            SetAttributeFromString( vDG, "TableEntry", "ExternalValue", szText );
         } 

         //:END
         break ;
   } 


   //:     /* end zDERIVED_SET */
   //:END  /* case */
   return( 0 );
// END
} 


//:DERIVED ATTRIBUTE OPERATION
//:NLS_DIL_Text( VIEW        vDG BASED ON LOD TZDGSRCO,
//:              STRING (32) lpEntity,
//:              STRING (32) lpAttribute,
//:              SHORT       GetOrSetFlag )

//:   SHORT        nLanguageCode
zOPER_EXPORT zSHORT OPERATION
oTZDGSRCO_NLS_DIL_Text( zVIEW     vDG,
                        LPVIEWENTITY lpEntity,
                        LPVIEWATTRIB lpAttribute,
                        zSHORT    GetOrSetFlag )
{
   zSHORT    nLanguageCode = 0; 
   //:STRING (254) szText
   zCHAR     szText[ 255 ] = { 0 }; 
   zSHORT    RESULT; 


   //:SysGetLanguageCode( nLanguageCode )
   SysGetLanguageCode( &nLanguageCode );

   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :

         //:// This is basically the same algorithm as the NLS_Text operations in
         //:// TZWDLG2O.VML.  See that code for an explanation.

         //:SET CURSOR FIRST vDG.DIL_NLS_Text WHERE
         //:              vDG.DIL_NLS_Text.LanguageIndex = nLanguageCode
         RESULT = SetCursorFirstEntityByInteger( vDG, "DIL_NLS_Text", "LanguageIndex", nLanguageCode, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szText = vDG.DIL_NLS_Text.Text
            GetVariableFromAttribute( szText, 0, 'S', 255, vDG, "DIL_NLS_Text", "Text", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szText = vDG.Context.DIL_Text
            GetVariableFromAttribute( szText, 0, 'S', 255, vDG, "Context", "DIL_Text", "", 0 );
         } 

         //:END

         //:StoreStringInRecord( vDG, lpEntity, lpAttribute, szText )
         StoreStringInRecord( vDG, lpEntity, lpAttribute, szText );
         break ;

      //:  /* end zDERIVED_GET */
      //:OF   zDERIVED_SET:
      case zDERIVED_SET :

         //:GetStringFromRecord( vDG, lpEntity, lpAttribute, szText, 254 )
         GetStringFromRecord( vDG, lpEntity, lpAttribute, szText, 254 );

         //:SET CURSOR FIRST vDG.DIL_NLS_Text WHERE
         //:              vDG.DIL_NLS_Text.LanguageIndex = nLanguageCode
         RESULT = SetCursorFirstEntityByInteger( vDG, "DIL_NLS_Text", "LanguageIndex", nLanguageCode, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:vDG.DIL_NLS_Text.Text = szText
            SetAttributeFromString( vDG, "DIL_NLS_Text", "Text", szText );
            //:ELSE
         } 
         else
         { 
            //:vDG.Context.DIL_Text = szText
            SetAttributeFromString( vDG, "Context", "DIL_Text", szText );
         } 

         //:END
         break ;
   } 


   //:     /* end zDERIVED_SET */
   //:END  /* case */
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
