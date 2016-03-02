#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zVOID
oTZCMLPLO_AnalyzeEntity( zVIEW     vXOD,
                         zVIEW     vWk,
                         zPCHAR    ObjectName );


static zSHORT
oTZCMLPLO_wl( zLONG     nFile,
              zPCHAR    StatementLine );


//:LOCAL OPERATION
static zVOID
oTZCMLPLO_AnalyzeEntity( zVIEW     vXOD,
                         zVIEW     vWk,
                         zPCHAR    ObjectName )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 255 ]; 

   //:AnalyzeEntity( VIEW vXOD BASED ON LOD TZZOXODO,
   //:            VIEW vWk  BASED ON LOD TZCMHDWO,
   //:            STRING (32)  ObjectName )

   //:// First process the recursive sub entities.
   //:FOR EACH vXOD.CHILDENTITY
   RESULT = SetCursorFirstEntity( vXOD, "CHILDENTITY", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( vXOD, "CHILDENTITY" )
      SetViewToSubobject( vXOD, "CHILDENTITY" );
      //:AnalyzeEntity( vXOD, vWk, ObjectName )
      oTZCMLPLO_AnalyzeEntity( vXOD, vWk, ObjectName );
      //:ResetViewFromSubobject( vXOD )
      ResetViewFromSubobject( vXOD );
      RESULT = SetCursorNextEntity( vXOD, "CHILDENTITY", "" );
   } 

   //:END

   //:// Create the Object Entity Constraint work entity if necessary.
   //:IF vXOD.ENTITY.ECEOPER != ""
   if ( CompareAttributeToString( vXOD, "ENTITY", "ECEOPER", "" ) != 0 )
   { 
      //:CREATE ENTITY vWk.Object
      RESULT = CreateEntity( vWk, "Object", zPOS_AFTER );
      //:vWk.Object.Name = ObjectName + "." + vXOD.ENTITY.NAME
      ZeidonStringCopy( szTempString_0, 1, 0, ObjectName, 1, 0, 255 );
      ZeidonStringConcat( szTempString_0, 1, 0, ".", 1, 0, 255 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vXOD, "ENTITY", "NAME", "", 0 );
      ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 255 );
      SetAttributeFromString( vWk, "Object", "Name", szTempString_0 );
      //:vWk.Object.OperName = vXOD.ENTITY.ECEOPER
      SetAttributeFromAttribute( vWk, "Object", "OperName", vXOD, "ENTITY", "ECEOPER" );
      //:vWk.Object.Type = "E"
      SetAttributeFromString( vWk, "Object", "Type", "E" );
   } 

   //:END

   //:FOR EACH vXOD.ATTRIB
   RESULT = SetCursorFirstEntity( vXOD, "ATTRIB", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// Create the Derived Attribute work entity if necessary.
      //:IF vXOD.ATTRIB.DERIVEDF != ""
      if ( CompareAttributeToString( vXOD, "ATTRIB", "DERIVEDF", "" ) != 0 )
      { 
         //:CREATE ENTITY vWk.Object
         RESULT = CreateEntity( vWk, "Object", zPOS_AFTER );
         //:vWk.Object.Name = ObjectName + "." + vXOD.ENTITY.NAME + "." + vXOD.ATTRIB.NAME
         ZeidonStringCopy( szTempString_2, 1, 0, ObjectName, 1, 0, 255 );
         ZeidonStringConcat( szTempString_2, 1, 0, ".", 1, 0, 255 );
         GetVariableFromAttribute( szTempString_3, 0, 'S', 255, vXOD, "ENTITY", "NAME", "", 0 );
         ZeidonStringConcat( szTempString_2, 1, 0, szTempString_3, 1, 0, 255 );
         ZeidonStringConcat( szTempString_2, 1, 0, ".", 1, 0, 255 );
         GetVariableFromAttribute( szTempString_4, 0, 'S', 255, vXOD, "ATTRIB", "NAME", "", 0 );
         ZeidonStringConcat( szTempString_2, 1, 0, szTempString_4, 1, 0, 255 );
         SetAttributeFromString( vWk, "Object", "Name", szTempString_2 );
         //:vWk.Object.OperName  = vXOD.ATTRIB.DERIVEDF
         SetAttributeFromAttribute( vWk, "Object", "OperName", vXOD, "ATTRIB", "DERIVEDF" );
         //:vWk.Object.Type = "A"
         SetAttributeFromString( vWk, "Object", "Type", "A" );
      } 

      RESULT = SetCursorNextEntity( vXOD, "ATTRIB", "" );
      //:END
   } 

   //:END
   return;
// END
} 


//:TRANSFORMATION OPERATION
//:CheckOI_ForDupZKey( VIEW vTZCMLPLO BASED ON LOD TZCMLPLO,
//:                    VIEW vHierMetaOI_Orig,
//:                    STRING ( 32 ) szRootEntityName )

//:   VIEW vHierMetaOI
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CheckOI_ForDupZKey( zVIEW     vTZCMLPLO,
                              zVIEW     vHierMetaOI_Orig,
                              zPCHAR    szRootEntityName )
{
   zVIEW     vHierMetaOI = 0; 
   //:STRING ( 32 )   szCurrentEntityName
   zCHAR     szCurrentEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szPreviousEntityName
   zCHAR     szPreviousEntityName[ 33 ] = { 0 }; 
   //:STRING ( 2000 ) szMsg
   zCHAR     szMsg[ 2001 ] = { 0 }; 
   //:STRING ( 20 )   szZKey
   zCHAR     szZKey[ 21 ] = { 0 }; 
   //:INTEGER         lZKey
   zLONG     lZKey = 0; 
   //:INTEGER         lLastZKey
   zLONG     lLastZKey = 0; 
   //:SHORT           nHierRC
   zSHORT    nHierRC = 0; 
   //:SHORT           nReturnLevel
   zSHORT    nReturnLevel = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 


   //:// Check the vHierMetaOI object instance for a duplicate ZKey. Do this by creating a work
   //:// subobject in vHierMetaOI of all ZKey values in the vHierMetaOI, then sorting and comparing them.

   //:FOR EACH vTZCMLPLO.EntityZKey 
   RESULT = SetCursorFirstEntity( vTZCMLPLO, "EntityZKey", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY vTZCMLPLO.EntityZKey NONE 
      RESULT = DeleteEntity( vTZCMLPLO, "EntityZKey", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vTZCMLPLO, "EntityZKey", "" );
   } 

   //:END
   //:FOR EACH vTZCMLPLO.DuplicateZKey 
   RESULT = SetCursorFirstEntity( vTZCMLPLO, "DuplicateZKey", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY vTZCMLPLO.DuplicateZKey NONE 
      RESULT = DeleteEntity( vTZCMLPLO, "DuplicateZKey", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vTZCMLPLO, "DuplicateZKey", "" );
   } 

   //:END

   //:CreateViewFromView( vHierMetaOI, vHierMetaOI_Orig )
   CreateViewFromView( &vHierMetaOI, vHierMetaOI_Orig );
   //:NAME VIEW vHierMetaOI "vHierMetaOIDuplicates"
   SetNameForView( vHierMetaOI, "vHierMetaOIDuplicates", 0, zLEVEL_TASK );
   //:SetCursorFirstEntity( vHierMetaOI, szRootEntityName, "" )
   SetCursorFirstEntity( vHierMetaOI, szRootEntityName, "" );
   //:DefineHierarchicalCursor( vHierMetaOI, szRootEntityName )
   DefineHierarchicalCursor( vHierMetaOI, szRootEntityName );
   //:nHierRC = zCURSOR_SET
   nHierRC = zCURSOR_SET;
   //:// Create Root ZKey.
   //:GetIntegerFromAttribute( lZKey, vHierMetaOI, szRootEntityName, "ZKey" )
   GetIntegerFromAttribute( &lZKey, vHierMetaOI, szRootEntityName, "ZKey" );
   //:CREATE ENTITY vTZCMLPLO.EntityZKey 
   RESULT = CreateEntity( vTZCMLPLO, "EntityZKey", zPOS_AFTER );
   //:vTZCMLPLO.EntityZKey.ZKey       = lZKey
   SetAttributeFromInteger( vTZCMLPLO, "EntityZKey", "ZKey", lZKey );
   //:vTZCMLPLO.EntityZKey.EntityName = szRootEntityName
   SetAttributeFromString( vTZCMLPLO, "EntityZKey", "EntityName", szRootEntityName );
   //:LOOP WHILE nHierRC >= zCURSOR_SET
   while ( nHierRC >= zCURSOR_SET )
   { 
      //:nHierRC = SetCursorNextEntityHierarchical( nReturnLevel,
      //:                                           szCurrentEntityName,
      //:                                           vHierMetaOI )
      nHierRC = SetCursorNextEntityHierarchical( (zPUSHORT) &nReturnLevel, szCurrentEntityName, vHierMetaOI );
      //:IF nHierRC >= zCURSOR_SET
      if ( nHierRC >= zCURSOR_SET )
      { 
         //:// Create ZKey, unless Entity is identified as No Duplicate Check.
         //:SET CURSOR FIRST vTZCMLPLO.DuplicateCheckEntity WHERE vTZCMLPLO.DuplicateCheckEntity.EntityName = szCurrentEntityName
         RESULT = SetCursorFirstEntityByString( vTZCMLPLO, "DuplicateCheckEntity", "EntityName", szCurrentEntityName, "" );
         //:IF RESULT >= zCURSOR_SET 
         if ( RESULT >= zCURSOR_SET )
         { 
            //:GetIntegerFromAttribute( lZKey, vHierMetaOI, szCurrentEntityName, "ZKey" )
            GetIntegerFromAttribute( &lZKey, vHierMetaOI, szCurrentEntityName, "ZKey" );
            //:CREATE ENTITY vTZCMLPLO.EntityZKey 
            RESULT = CreateEntity( vTZCMLPLO, "EntityZKey", zPOS_AFTER );
            //:vTZCMLPLO.EntityZKey.ZKey       = lZKey
            SetAttributeFromInteger( vTZCMLPLO, "EntityZKey", "ZKey", lZKey );
            //:vTZCMLPLO.EntityZKey.EntityName = szCurrentEntityName
            SetAttributeFromString( vTZCMLPLO, "EntityZKey", "EntityName", szCurrentEntityName );

            //:// For recursive subentity, step down a level.
            //:IF nHierRC = zCURSOR_SET_RECURSIVECHILD
            if ( nHierRC == zCURSOR_SET_RECURSIVECHILD )
            { 
               //:SetViewToSubobject( vHierMetaOI, szCurrentEntityName )
               SetViewToSubobject( vHierMetaOI, szCurrentEntityName );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END
   //:DropView( vHierMetaOI )
   DropView( vHierMetaOI );

   //:// Sort ZKey entries and loop through each entry looking for duplicates.
   //:OrderEntityForView( vTZCMLPLO, "EntityZKey", "ZKey A" )
   OrderEntityForView( vTZCMLPLO, "EntityZKey", "ZKey A" );
   //:SET CURSOR FIRST vTZCMLPLO.EntityZKey 
   RESULT = SetCursorFirstEntity( vTZCMLPLO, "EntityZKey", "" );
   //:lLastZKey = 0
   lLastZKey = 0;
   //:FOR EACH vTZCMLPLO.EntityZKey 
   RESULT = SetCursorFirstEntity( vTZCMLPLO, "EntityZKey", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:lZKey = vTZCMLPLO.EntityZKey.ZKey 
      GetIntegerFromAttribute( &lZKey, vTZCMLPLO, "EntityZKey", "ZKey" );
      //:IF lLastZKey = lZKey
      if ( lLastZKey == lZKey )
      { 
         //:CREATE ENTITY vTZCMLPLO.DuplicateZKey 
         RESULT = CreateEntity( vTZCMLPLO, "DuplicateZKey", zPOS_AFTER );
         //:vTZCMLPLO.DuplicateZKey.ZKey        = vTZCMLPLO.EntityZKey.ZKey 
         SetAttributeFromAttribute( vTZCMLPLO, "DuplicateZKey", "ZKey", vTZCMLPLO, "EntityZKey", "ZKey" );
         //:vTZCMLPLO.DuplicateZKey.EntityName1 = vTZCMLPLO.EntityZKey.EntityName 
         SetAttributeFromAttribute( vTZCMLPLO, "DuplicateZKey", "EntityName1", vTZCMLPLO, "EntityZKey", "EntityName" );
         //:vTZCMLPLO.DuplicateZKey.EntityName2 = szPreviousEntityName
         SetAttributeFromString( vTZCMLPLO, "DuplicateZKey", "EntityName2", szPreviousEntityName );
      } 

      //:END
      //:lLastZKey            = lZKey
      lLastZKey = lZKey;
      //:szPreviousEntityName = vTZCMLPLO.EntityZKey.EntityName
      GetVariableFromAttribute( szPreviousEntityName, 0, 'S', 33, vTZCMLPLO, "EntityZKey", "EntityName", "", 0 );
      RESULT = SetCursorNextEntity( vTZCMLPLO, "EntityZKey", "" );
   } 

   //:END

   //:// If a duplicate ZKey was found, issue a message.
   //:IF vTZCMLPLO.DuplicateZKey EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vTZCMLPLO, "DuplicateZKey" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szMsg = "The following ZKeys and Entities are duplicates: " + NEW_LINE
      ZeidonStringCopy( szMsg, 1, 0, "The following ZKeys and Entities are duplicates: ", 1, 0, 2001 );
      ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 2001 );
      //:FOR EACH vTZCMLPLO.DuplicateZKey 
      RESULT = SetCursorFirstEntity( vTZCMLPLO, "DuplicateZKey", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:szZKey = vTZCMLPLO.DuplicateZKey.ZKey 
         GetVariableFromAttribute( szZKey, 0, 'S', 21, vTZCMLPLO, "DuplicateZKey", "ZKey", "", 0 );
         //:szMsg = szMsg + "   " + szZKey + " / " + vTZCMLPLO.DuplicateZKey.EntityName1 + " / " + vTZCMLPLO.DuplicateZKey.EntityName2 + NEW_LINE
         ZeidonStringConcat( szMsg, 1, 0, "   ", 1, 0, 2001 );
         ZeidonStringConcat( szMsg, 1, 0, szZKey, 1, 0, 2001 );
         ZeidonStringConcat( szMsg, 1, 0, " / ", 1, 0, 2001 );
         GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vTZCMLPLO, "DuplicateZKey", "EntityName1", "", 0 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 2001 );
         ZeidonStringConcat( szMsg, 1, 0, " / ", 1, 0, 2001 );
         GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vTZCMLPLO, "DuplicateZKey", "EntityName2", "", 0 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 2001 );
         ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 2001 );
         RESULT = SetCursorNextEntity( vTZCMLPLO, "DuplicateZKey", "" );
      } 

      //:END
      //:MessageSend( vTZCMLPLO, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vTZCMLPLO, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:ELSE
   } 
   else
   { 
      //:MessageSend( vTZCMLPLO, "", "Configuration Management", "There were no duplicate keys in object.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vTZCMLPLO, "", "Configuration Management", "There were no duplicate keys in object.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   } 

   //:END

   //:// Remove temporary work entities.
   //:FOR EACH vTZCMLPLO.EntityZKey 
   RESULT = SetCursorFirstEntity( vTZCMLPLO, "EntityZKey", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY vTZCMLPLO.EntityZKey NONE 
      RESULT = DeleteEntity( vTZCMLPLO, "EntityZKey", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vTZCMLPLO, "EntityZKey", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateCallHeader( VIEW vLPLR BASED ON LOD TZCMLPLO, VIEW vSubtask )

//:   VIEW vXDM BASED ON LOD TZDMXGPO
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_GenerateCallHeader( zVIEW     vLPLR,
                              zVIEW     vSubtask )
{
   zVIEW     vXDM = 0; 
   //:VIEW vXOD BASED ON LOD TZZOXODO
   zVIEW     vXOD = 0; 
   //:VIEW vWk  BASED ON LOD TZCMHDWO
   zVIEW     vWk = 0; 

   //:STRING ( 513 ) FileName       // size according to zMAX_FILESPEC_LTH+1
   zCHAR     FileName[ 514 ] = { 0 }; 
   //:INTEGER        f
   zLONG     f = 0; 
   //:STRING ( 513 ) XDM_FileName   // size according to zMAX_FILESPEC_LTH+1
   zCHAR     XDM_FileName[ 514 ] = { 0 }; 
   //:STRING ( 254 ) Sl
   zCHAR     Sl[ 255 ] = { 0 }; 
   //:STRING ( 2 )   Quote
   zCHAR     Quote[ 3 ] = { 0 }; 
   //:INTEGER        nFirstTime
   zLONG     nFirstTime = 0; 
   //:STRING ( 513 ) XOD_FileName   // size according to zMAX_FILESPEC_LTH+1
   zCHAR     XOD_FileName[ 514 ] = { 0 }; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zCHAR     szTempString_6[ 255 ]; 
   zCHAR     szTempString_7[ 255 ]; 


   //:Quote = "\"
   ZeidonStringCopy( Quote, 1, 0, "\\", 1, 0, 3 );
   //:FileName = vLPLR.LPLR.PgmSrcDir
   GetVariableFromAttribute( FileName, 0, 'S', 514, vLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:FileName = FileName + "\kzoehdaa.c"
   ZeidonStringConcat( FileName, 1, 0, "\\kzoehdaa.c", 1, 0, 514 );
   //:f = SysOpenFile( vLPLR, FileName, COREFILE_WRITE )
   f = SysOpenFile( vLPLR, FileName, COREFILE_WRITE );

   //:/**************************************************************
   //:Generate a Domain header that will be linked with all Domain
   //:operation code for environments for which dynamically calling
   //:routines is difficult.  The header has a case statement for every
   //:Domain with an operation, which has a call to the corresponding
   //:Domain operation.  OE operations use their normal SysDomainCall
   //:interface but end up calling the generated SysDomainCall routine
   //:instead of the one normally in kzoesyaa.c.  The SysGetDomainProc
   //:routine returns an integer for the case statement instead of the
   //:Domain routine address.
   //:The code is generated by navigating through all the Domain entities
   //:in the TZDMXGPO object instance named ZEIDON.XDM.
   //:**************************************************************/

   //:XDM_FileName = vLPLR.LPLR.ExecDir
   GetVariableFromAttribute( XDM_FileName, 0, 'S', 514, vLPLR, "LPLR", "ExecDir", "", 0 );
   //:XDM_FileName = XDM_FileName + "\zeidon.xdm"
   ZeidonStringConcat( XDM_FileName, 1, 0, "\\zeidon.xdm", 1, 0, 514 );
   //:ActivateOI_FromFile ( vXDM, "TZDMXGPO", vLPLR, XDM_FileName, zSINGLE )
   ActivateOI_FromFile( &vXDM, "TZDMXGPO", vLPLR, XDM_FileName, zSINGLE );
   //:NAME VIEW vXDM "vXDM"
   SetNameForView( vXDM, "vXDM", 0, zLEVEL_TASK );

   //:Sl = "#include " + Quote + "kzoephdr.h" + Quote
   ZeidonStringCopy( Sl, 1, 0, "#include ", 1, 0, 255 );
   ZeidonStringConcat( Sl, 1, 0, Quote, 1, 0, 255 );
   ZeidonStringConcat( Sl, 1, 0, "kzoephdr.h", 1, 0, 255 );
   ZeidonStringConcat( Sl, 1, 0, Quote, 1, 0, 255 );
   //:wl( f, Sl )
   oTZCMLPLO_wl( f, Sl );
   //:Sl = "#include " + Quote + "ZeidonOp.h" + Quote
   ZeidonStringCopy( Sl, 1, 0, "#include ", 1, 0, 255 );
   ZeidonStringConcat( Sl, 1, 0, Quote, 1, 0, 255 );
   ZeidonStringConcat( Sl, 1, 0, "ZeidonOp.h", 1, 0, 255 );
   ZeidonStringConcat( Sl, 1, 0, Quote, 1, 0, 255 );
   //:wl( f, Sl )
   oTZCMLPLO_wl( f, Sl );
   //:wl( f, "// " )
   oTZCMLPLO_wl( f, "// " );
   //:wl( f, "// MODULE NAME:  KZOEHDAA  -  Object services Library Resolution " )
   oTZCMLPLO_wl( f, "// MODULE NAME:  KZOEHDAA  -  Object services Library Resolution " );
   //:wl( f, "// DESCRIPTION:  This is the source file which returns function pointers " )
   oTZCMLPLO_wl( f, "// DESCRIPTION:  This is the source file which returns function pointers " );
   //:wl( f, "//               for Domain, Object and Transformation operations. " )
   oTZCMLPLO_wl( f, "//               for Domain, Object and Transformation operations. " );
   //:wl( f, " " )
   oTZCMLPLO_wl( f, " " );

   //:ACTIVATE vWk EMPTY
   RESULT = ActivateEmptyObjectInstance( &vWk, "TZCMHDWO", vLPLR, zSINGLE );
   //:CREATE ENTITY vWk.Root
   RESULT = CreateEntity( vWk, "Root", zPOS_AFTER );

   //:FOR EACH vXDM.Domain
   RESULT = SetCursorFirstEntity( vXDM, "Domain", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST vWk.Executable
      //:    WHERE vWk.Executable.Name = vXDM.Domain.DLL_Name
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vXDM, "Domain", "DLL_Name" );
      RESULT = SetCursorFirstEntityByString( vWk, "Executable", "Name", szTempString_0, "" );

      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CREATE ENTITY vWk.Executable
         RESULT = CreateEntity( vWk, "Executable", zPOS_AFTER );
         //:vWk.Executable.Name = vXDM.Domain.DLL_Name
         SetAttributeFromAttribute( vWk, "Executable", "Name", vXDM, "Domain", "DLL_Name" );
      } 

      //:END

      //:IF vXDM.Domain.OperName != ""
      if ( CompareAttributeToString( vXDM, "Domain", "OperName", "" ) != 0 )
      { 
         //:CREATE ENTITY vWk.Object
         RESULT = CreateEntity( vWk, "Object", zPOS_AFTER );
         //:vWk.Object.Name = vXDM.Domain.Name
         SetAttributeFromAttribute( vWk, "Object", "Name", vXDM, "Domain", "Name" );
         //:vWk.Object.OperName = vXDM.Domain.OperName
         SetAttributeFromAttribute( vWk, "Object", "OperName", vXDM, "Domain", "OperName" );
         //:vWk.Object.Type = "D"
         SetAttributeFromString( vWk, "Object", "Type", "D" );
      } 

      RESULT = SetCursorNextEntity( vXDM, "Domain", "" );
      //:END
   } 

   //:END

   //:DropObjectInstance( vXDM )
   DropObjectInstance( vXDM );

   //:// Build work object for Object Constraints, Entity Constraints and Derived Attributes.

   //:SET CURSOR FIRST vLPLR.W_MetaType WHERE vLPLR.W_MetaType.Type = 2007
   RESULT = SetCursorFirstEntityByInteger( vLPLR, "W_MetaType", "Type", 2007, "" );
   //:FOR EACH vLPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:XOD_FileName = vLPLR.LPLR.ExecDir
      GetVariableFromAttribute( XOD_FileName, 0, 'S', 514, vLPLR, "LPLR", "ExecDir", "", 0 );
      //:XOD_FileName = XOD_FileName + "\" + vLPLR.W_MetaDef.Name + ".XOD"
      ZeidonStringConcat( XOD_FileName, 1, 0, "\\", 1, 0, 514 );
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLPLR, "W_MetaDef", "Name", "", 0 );
      ZeidonStringConcat( XOD_FileName, 1, 0, szTempString_0, 1, 0, 514 );
      ZeidonStringConcat( XOD_FileName, 1, 0, ".XOD", 1, 0, 514 );
      //:nRC = ActivateOI_FromFile( vXOD, "TZZOXODO", vLPLR, XOD_FileName, zSINGLE )
      nRC = ActivateOI_FromFile( &vXOD, "TZZOXODO", vLPLR, XOD_FileName, zSINGLE );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:SET CURSOR FIRST vWk.Executable
         //:    WHERE vWk.Executable.Name = vXOD.OBJECT.OPER_LIBNM
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vXOD, "OBJECT", "OPER_LIBNM" );
         RESULT = SetCursorFirstEntityByString( vWk, "Executable", "Name", szTempString_1, "" );

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY vWk.Executable
            RESULT = CreateEntity( vWk, "Executable", zPOS_AFTER );
            //:vWk.Executable.Name = vXOD.OBJECT.OPER_LIBNM
            SetAttributeFromAttribute( vWk, "Executable", "Name", vXOD, "OBJECT", "OPER_LIBNM" );
         } 

         //:END

         //:// Create the Object Entity Constraint work entity if necessary.
         //:IF vXOD.OBJECT.OCEOPER != ""
         if ( CompareAttributeToString( vXOD, "OBJECT", "OCEOPER", "" ) != 0 )
         { 
            //:CREATE ENTITY vWk.Object
            RESULT = CreateEntity( vWk, "Object", zPOS_AFTER );
            //:vWk.Object.Name = vXOD.OBJECT.NAME
            SetAttributeFromAttribute( vWk, "Object", "Name", vXOD, "OBJECT", "NAME" );
            //:vWk.Object.OperName = vXOD.OBJECT.OCEOPER
            SetAttributeFromAttribute( vWk, "Object", "OperName", vXOD, "OBJECT", "OCEOPER" );
            //:vWk.Object.Type = "O"
            SetAttributeFromString( vWk, "Object", "Type", "O" );
         } 

         //:END

         //:AnalyzeEntity( vXOD, vWk, vXOD.OBJECT.NAME )
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vXOD, "OBJECT", "NAME" );
         oTZCMLPLO_AnalyzeEntity( vXOD, vWk, szTempString_1 );
      } 

      //:END

      //:DropObjectInstance( vXOD )
      DropObjectInstance( vXOD );
      RESULT = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );
   } 


   //:END

   //:FOR EACH vWk.Executable
   RESULT = SetCursorFirstEntity( vWk, "Executable", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nFirstTime = 1
      nFirstTime = 1;

      //:FOR EACH vWk.Object
      RESULT = SetCursorFirstEntity( vWk, "Object", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:IF nFirstTime = 1
         if ( nFirstTime == 1 )
         { 
            //:nFirstTime = 0
            nFirstTime = 0;
            //:wl( f, "zPVOID " )
            oTZCMLPLO_wl( f, "zPVOID " );
            //:Sl = vWk.Executable.Name + "( zPCHAR pszFunctionName )"
            GetStringFromAttribute( Sl, zsizeof( Sl ), vWk, "Executable", "Name" );
            ZeidonStringConcat( Sl, 1, 0, "( zPCHAR pszFunctionName )", 1, 0, 255 );
            //:wl( f, Sl )
            oTZCMLPLO_wl( f, Sl );
            //:wl( f, "{" )
            oTZCMLPLO_wl( f, "{" );
         } 

         //:END

         //:IF vWk.Object.Type = "D"
         if ( CompareAttributeToString( vWk, "Object", "Type", "D" ) == 0 )
         { 
            //:Sl = "   // Operation Calls for Domain: " + vWk.Object.Name
            GetVariableFromAttribute( szTempString_2, 0, 'S', 255, vWk, "Object", "Name", "", 0 );
            ZeidonStringCopy( Sl, 1, 0, "   // Operation Calls for Domain: ", 1, 0, 255 );
            ZeidonStringConcat( Sl, 1, 0, szTempString_2, 1, 0, 255 );
         } 

         //:END

         //:IF vWk.Object.Type = "O"
         if ( CompareAttributeToString( vWk, "Object", "Type", "O" ) == 0 )
         { 
            //:Sl = "   // Object Constraint for Object: " + vWk.Object.Name
            GetVariableFromAttribute( szTempString_3, 0, 'S', 255, vWk, "Object", "Name", "", 0 );
            ZeidonStringCopy( Sl, 1, 0, "   // Object Constraint for Object: ", 1, 0, 255 );
            ZeidonStringConcat( Sl, 1, 0, szTempString_3, 1, 0, 255 );
         } 

         //:END

         //:IF vWk.Object.Type = "E"
         if ( CompareAttributeToString( vWk, "Object", "Type", "E" ) == 0 )
         { 
            //:Sl = "   // Entity Constraint for Object.Entity: " + vWk.Object.Name
            GetVariableFromAttribute( szTempString_4, 0, 'S', 255, vWk, "Object", "Name", "", 0 );
            ZeidonStringCopy( Sl, 1, 0, "   // Entity Constraint for Object.Entity: ", 1, 0, 255 );
            ZeidonStringConcat( Sl, 1, 0, szTempString_4, 1, 0, 255 );
         } 

         //:END

         //:IF vWk.Object.Type = "A"
         if ( CompareAttributeToString( vWk, "Object", "Type", "A" ) == 0 )
         { 
            //:Sl = "   // Derived Attribute for Object.Entity.Attr: " + vWk.Object.Name
            GetVariableFromAttribute( szTempString_5, 0, 'S', 255, vWk, "Object", "Name", "", 0 );
            ZeidonStringCopy( Sl, 1, 0, "   // Derived Attribute for Object.Entity.Attr: ", 1, 0, 255 );
            ZeidonStringConcat( Sl, 1, 0, szTempString_5, 1, 0, 255 );
         } 

         //:END

         //:wl( f, Sl )
         oTZCMLPLO_wl( f, Sl );

         //:Sl = "   if ( zstrcmp( pszFunctionName, " + Quote + vWk.Object.OperName + Quote + " ) == 0 )"
         ZeidonStringCopy( Sl, 1, 0, "   if ( zstrcmp( pszFunctionName, ", 1, 0, 255 );
         ZeidonStringConcat( Sl, 1, 0, Quote, 1, 0, 255 );
         GetVariableFromAttribute( szTempString_6, 0, 'S', 255, vWk, "Object", "OperName", "", 0 );
         ZeidonStringConcat( Sl, 1, 0, szTempString_6, 1, 0, 255 );
         ZeidonStringConcat( Sl, 1, 0, Quote, 1, 0, 255 );
         ZeidonStringConcat( Sl, 1, 0, " ) == 0 )", 1, 0, 255 );
         //:wl( f, Sl )
         oTZCMLPLO_wl( f, Sl );
         //:Sl = "      return( (zPVOID) " + vWk.Object.OperName + " );"
         GetVariableFromAttribute( szTempString_7, 0, 'S', 255, vWk, "Object", "OperName", "", 0 );
         ZeidonStringCopy( Sl, 1, 0, "      return( (zPVOID) ", 1, 0, 255 );
         ZeidonStringConcat( Sl, 1, 0, szTempString_7, 1, 0, 255 );
         ZeidonStringConcat( Sl, 1, 0, " );", 1, 0, 255 );
         //:wl( f, Sl )
         oTZCMLPLO_wl( f, Sl );
         //:wl( f, " " )
         oTZCMLPLO_wl( f, " " );
         RESULT = SetCursorNextEntity( vWk, "Object", "" );
      } 


      //:END

      //:IF nFirstTime = 0
      if ( nFirstTime == 0 )
      { 
         //:wl( f, "   return( 0 ); " )
         oTZCMLPLO_wl( f, "   return( 0 ); " );
         //:wl( f, " " )
         oTZCMLPLO_wl( f, " " );
         //:wl( f, "}" )
         oTZCMLPLO_wl( f, "}" );
         //:wl( f, " " )
         oTZCMLPLO_wl( f, " " );
      } 

      RESULT = SetCursorNextEntity( vWk, "Executable", "" );
      //:END
   } 


   //:END

   //:vWk = 0
   vWk = 0;
   //:SysCloseFile( vLPLR, f, 0 )
   SysCloseFile( vLPLR, f, 0 );

   //:GenerateTransShell( vLPLR, vSubtask )
   oTZCMLPLO_GenerateTransShell( vLPLR, vSubtask );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:wl( INTEGER       nFile,
//:    STRING ( 32 ) StatementLine )

//:   VIEW vSubtask
static zSHORT
oTZCMLPLO_wl( zLONG     nFile,
              zPCHAR    StatementLine )
{
   zVIEW     vSubtask = 0; 


   //:vSubtask = GetDefaultViewForActiveTask( )
   vSubtask = GetDefaultViewForActiveTask( );
   //:SysWriteLine( vSubtask, nFile, StatementLine )
   SysWriteLine( vSubtask, nFile, StatementLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateTransShell( VIEW vLPLR BASED ON LOD TZCMLPLO, VIEW vSubtask )
//:   VIEW vLOD BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_GenerateTransShell( zVIEW     vLPLR,
                              zVIEW     vSubtask )
{
   zVIEW     vLOD = 0; 
   //:INTEGER f
   zLONG     f = 0; 
   //:INTEGER l
   zLONG     l = 0; 
   //:INTEGER lArgCount
   zLONG     lArgCount = 0; 
   //:INTEGER lViewCount
   zLONG     lViewCount = 0; 
   //:INTEGER nFirstTime
   zLONG     nFirstTime = 0; 
   //:STRING ( 10 )  szl
   zCHAR     szl[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szViewCount
   zCHAR     szViewCount[ 11 ] = { 0 }; 
   //:STRING ( 2 )   Quote
   zCHAR     Quote[ 3 ] = { 0 }; 
   //:STRING ( 513 ) sz            // size according to zMAX_FILESPEC_LTH + 1
   zCHAR     sz[ 514 ] = { 0 }; 
   //:STRING ( 513 ) sz2           // size according to zMAX_FILESPEC_LTH + 1
   zCHAR     sz2[ 514 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 65 ]; 
   zCHAR     szTempString_5[ 65 ]; 
   zCHAR     szTempString_6[ 65 ]; 
   zCHAR     szTempString_7[ 65 ]; 
   zCHAR     szTempString_8[ 65 ]; 
   zCHAR     szTempString_9[ 65 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 65 ]; 
   zCHAR     szTempString_13[ 65 ]; 


   //:Quote = "\"
   ZeidonStringCopy( Quote, 1, 0, "\\", 1, 0, 3 );
   //://Quote = '"'
   //:// Set meta type for LODs
   //:SET CURSOR FIRST vLPLR.W_MetaType WHERE vLPLR.W_MetaType.Type = 2007
   RESULT = SetCursorFirstEntityByInteger( vLPLR, "W_MetaType", "Type", 2007, "" );
   //:FOR EACH vLPLR.W_MetaDef
   RESULT = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:ActivateMetaOI( vSubtask, vLOD, vLPLR, 2007, 0 )
      ActivateMetaOI( vSubtask, &vLOD, vLPLR, 2007, 0 );
      //:FOR EACH vLOD.SourceFile WHERE vLOD.SourceFile.LanguageType = "V"
      RESULT = SetCursorFirstEntityByString( vLOD, "SourceFile", "LanguageType", "V", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:sz = vLPLR.LPLR.PgmSrcDir
         GetVariableFromAttribute( sz, 0, 'S', 514, vLPLR, "LPLR", "PgmSrcDir", "", 0 );
         //:sz = sz + "\" + vLOD.SourceFile.Name + ".c"
         ZeidonStringConcat( sz, 1, 0, "\\", 1, 0, 514 );
         GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLOD, "SourceFile", "Name", "", 0 );
         ZeidonStringConcat( sz, 1, 0, szTempString_0, 1, 0, 514 );
         ZeidonStringConcat( sz, 1, 0, ".c", 1, 0, 514 );
         //:f = SysOpenFile( vLPLR, sz, 448 ) // 448 = COREFILE_APPEND
         f = SysOpenFile( vLPLR, sz, 448 );
         //:IF f != 0
         if ( f != 0 )
         { 
            //:wl( f, " " )
            oTZCMLPLO_wl( f, " " );
            //:wl( f, "zLONG OPERATION" )
            oTZCMLPLO_wl( f, "zLONG OPERATION" );
            //:sz = "o" + vLOD.LOD.Name + "_CallTransformation( "
            GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
            ZeidonStringCopy( sz, 1, 0, "o", 1, 0, 514 );
            ZeidonStringConcat( sz, 1, 0, szTempString_1, 1, 0, 514 );
            ZeidonStringConcat( sz, 1, 0, "_CallTransformation( ", 1, 0, 514 );
            //:l = GetStringLength( sz )
            l = GetStringLength( sz );
            //:sz = sz + "zPCHAR pszOperName,"
            ZeidonStringConcat( sz, 1, 0, "zPCHAR pszOperName,", 1, 0, 514 );
            //:wl( f, sz )
            oTZCMLPLO_wl( f, sz );
            //:sz = ""
            ZeidonStringCopy( sz, 1, 0, "", 1, 0, 514 );
            //:LOOP WHILE l > 0
            while ( l > 0 )
            { 
               //:sz = sz + " "
               ZeidonStringConcat( sz, 1, 0, " ", 1, 0, 514 );
               //:l = l - 1
               l = l - 1;
            } 

            //:END
            //:sz2 = sz + "zVIEW  vArgs,"
            ZeidonStringCopy( sz2, 1, 0, sz, 1, 0, 514 );
            ZeidonStringConcat( sz2, 1, 0, "zVIEW  vArgs,", 1, 0, 514 );
            //:wl( f, sz2 )
            oTZCMLPLO_wl( f, sz2 );
            //:sz2 = sz + "zPVIEW pvViewList[] )"
            ZeidonStringCopy( sz2, 1, 0, sz, 1, 0, 514 );
            ZeidonStringConcat( sz2, 1, 0, "zPVIEW pvViewList[] )", 1, 0, 514 );
            //:wl( f, sz2 )
            oTZCMLPLO_wl( f, sz2 );
            //:wl( f, "{" )
            oTZCMLPLO_wl( f, "{" );
            //:wl( f, "   zLONG lRC;" )
            oTZCMLPLO_wl( f, "   zLONG lRC;" );
            //:wl( f, " " )
            oTZCMLPLO_wl( f, " " );

            //:FOR EACH vLOD.Operation WHERE vLOD.Operation.Type = "T"
            RESULT = SetCursorFirstEntityByString( vLOD, "Operation", "Type", "T", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:sz = "   if ( zstrcmp( pszOperName, " + Quote
               ZeidonStringCopy( sz, 1, 0, "   if ( zstrcmp( pszOperName, ", 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
               //:IF vLOD.SourceFile.LanguageType = "V"
               if ( CompareAttributeToString( vLOD, "SourceFile", "LanguageType", "V" ) == 0 )
               { 
                  //:sz = sz + "o" + vLOD.LOD.Name + "_"
                  ZeidonStringConcat( sz, 1, 0, "o", 1, 0, 514 );
                  GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
                  ZeidonStringConcat( sz, 1, 0, szTempString_2, 1, 0, 514 );
                  ZeidonStringConcat( sz, 1, 0, "_", 1, 0, 514 );
               } 

               //:END
               //:sz = sz + vLOD.Operation.Name + Quote + " ) == 0 )"
               GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vLOD, "Operation", "Name", "", 0 );
               ZeidonStringConcat( sz, 1, 0, szTempString_3, 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, " ) == 0 )", 1, 0, 514 );
               //:wl( f, sz )
               oTZCMLPLO_wl( f, sz );
               //:wl( f, "   {" )
               oTZCMLPLO_wl( f, "   {" );

               //:// Set up local variables for each non-view argument.
               //:l = 0
               l = 0;
               //:nFirstTime = 1
               nFirstTime = 1;
               //:FOR EACH vLOD.Parameter
               RESULT = SetCursorFirstEntity( vLOD, "Parameter", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:l = l + 1
                  l = l + 1;
                  //:ConvertIntegerToString( l, szl, 10 )
                  ConvertIntegerToString( l, szl, 10 );

                  //:IF vLOD.Parameter.DataType = "S"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "S" ) == 0 )
                  { 
                     //:sz = "      zPCHAR   pszArg" + szl + ";" +
                     //:   "  // " + vLOD.Parameter.ShortDesc
                     ZeidonStringCopy( sz, 1, 0, "      zPCHAR   pszArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ";", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "  // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_4, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_4, 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "Y"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "Y" ) == 0 )
                  { 
                     //:IF nFirstTime = 1
                     if ( nFirstTime == 1 )
                     { 
                        //:nFirstTime = 0
                        nFirstTime = 0;
                        //:wl( f, "      zLONG    lTemp;" )
                        oTZCMLPLO_wl( f, "      zLONG    lTemp;" );
                     } 

                     //:END
                     //:sz = "      zCHAR    cArg" + szl + ";" +
                     //:   "  // " + vLOD.Parameter.ShortDesc
                     ZeidonStringCopy( sz, 1, 0, "      zCHAR    cArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ";", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "  // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_5, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_5, 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "M"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "M" ) == 0 )
                  { 
                     //:sz = "      zDECIMAL dArg" + szl + ";" +
                     //:   "  // " + vLOD.Parameter.ShortDesc
                     ZeidonStringCopy( sz, 1, 0, "      zDECIMAL dArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ";", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "  // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_6, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_6, 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "N"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "N" ) == 0 )
                  { 
                     //:IF nFirstTime = 1
                     if ( nFirstTime == 1 )
                     { 
                        //:nFirstTime = 0
                        nFirstTime = 0;
                        //:wl( f, "      zLONG    lTemp;" )
                        oTZCMLPLO_wl( f, "      zLONG    lTemp;" );
                     } 

                     //:END
                     //:sz = "      zSHORT   nArg" + szl + ";" +
                     //:   "  // " + vLOD.Parameter.ShortDesc
                     ZeidonStringCopy( sz, 1, 0, "      zSHORT   nArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ";", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "  // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_7, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_7, 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "L"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "L" ) == 0 )
                  { 
                     //:sz = "      zLONG    lArg" + szl + ";" +
                     //:   "  // " + vLOD.Parameter.ShortDesc
                     ZeidonStringCopy( sz, 1, 0, "      zLONG    lArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ";", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "  // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_8, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_8, 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  RESULT = SetCursorNextEntity( vLOD, "Parameter", "" );
                  //:END
               } 

               //:END

               //:lArgCount = l
               lArgCount = l;

               //:wl( f, " " )
               oTZCMLPLO_wl( f, " " );
               //:sz = "      SetCursorFirstEntity( vArgs, " + Quote + "Argument" +
               //:                Quote + ", 0 );"
               ZeidonStringCopy( sz, 1, 0, "      SetCursorFirstEntity( vArgs, ", 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, ", 0 );", 1, 0, 514 );
               //:wl( f, sz )
               oTZCMLPLO_wl( f, sz );
               //:nFirstTime = 1
               nFirstTime = 1;

               //:// Gen code to get each argument from the argument list.
               //:l = 0
               l = 0;
               //:FOR EACH vLOD.Parameter
               RESULT = SetCursorFirstEntity( vLOD, "Parameter", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:wl( f, " " )
                  oTZCMLPLO_wl( f, " " );
                  //:sz = "      // Get value for " + vLOD.Parameter.ShortDesc
                  GetVariableFromAttribute( szTempString_9, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                  ZeidonStringCopy( sz, 1, 0, "      // Get value for ", 1, 0, 514 );
                  ZeidonStringConcat( sz, 1, 0, szTempString_9, 1, 0, 514 );
                  //:wl( f, sz )
                  oTZCMLPLO_wl( f, sz );
                  //:IF nFirstTime = 1
                  if ( nFirstTime == 1 )
                  { 
                     //:nFirstTime = 0
                     nFirstTime = 0;
                     //:ELSE
                  } 
                  else
                  { 
                     //:sz = "      SetCursorNextEntity( vArgs, " + Quote + "Argument" +
                     //:                Quote + ", 0 );"
                     ZeidonStringCopy( sz, 1, 0, "      SetCursorNextEntity( vArgs, ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", 0 );", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:l = l + 1
                  l = l + 1;
                  //:ConvertIntegerToString( l, szl, 10 )
                  ConvertIntegerToString( l, szl, 10 );

                  //:IF vLOD.Parameter.DataType = "S"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "S" ) == 0 )
                  { 
                     //:sz = "      GetAddrForAttribute( &pszArg" + szl + ", vArgs, " +
                     //:  Quote + "Argument" + Quote + ", " + Quote + "Value" +
                     //:  Quote + " );"
                     ZeidonStringCopy( sz, 1, 0, "      GetAddrForAttribute( &pszArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", vArgs, ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Value", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " );", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "N"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "N" ) == 0 )
                  { 
                     //:sz = "      GetIntegerFromAttribute( &lTemp, vArgs, " +
                     //:  Quote + "Argument" + Quote + ", " + Quote + "Value" +
                     //:  Quote + " );"
                     ZeidonStringCopy( sz, 1, 0, "      GetIntegerFromAttribute( &lTemp, vArgs, ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Value", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " );", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                     //:sz = "      nArg" + szl + " = (zSHORT) lTemp;"
                     ZeidonStringCopy( sz, 1, 0, "      nArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " = (zSHORT) lTemp;", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "Y"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "Y" ) == 0 )
                  { 
                     //:sz = "      GetIntegerFromAttribute( &lTemp, vArgs, " +
                     //:  Quote + "Argument" + Quote + ", " + Quote + "Value" +
                     //:  Quote + " );"
                     ZeidonStringCopy( sz, 1, 0, "      GetIntegerFromAttribute( &lTemp, vArgs, ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Value", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " );", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                     //:sz = "      cArg" + szl + " = (zCHAR) lTemp;"
                     ZeidonStringCopy( sz, 1, 0, "      cArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " = (zCHAR) lTemp;", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "L"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "L" ) == 0 )
                  { 
                     //:sz = "      GetIntegerFromAttribute( &lArg" + szl + ", vArgs, " +
                     //:  Quote + "Argument" + Quote + ", " + Quote + "Value" +
                     //:  Quote + " );"
                     ZeidonStringCopy( sz, 1, 0, "      GetIntegerFromAttribute( &lArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", vArgs, ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Value", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " );", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "M"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "M" ) == 0 )
                  { 
                     //:sz = "      GetDecimalFromAttribute( &dArg" + szl + ", vArgs, " +
                     //:  Quote + "Argument" + Quote + ", " + Quote + "Value" +
                     //:  Quote + " );"
                     ZeidonStringCopy( sz, 1, 0, "      GetDecimalFromAttribute( &dArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", vArgs, ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Argument", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, ", ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, "Value", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, Quote, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " );", 1, 0, 514 );
                     //:wl( f, sz )
                     oTZCMLPLO_wl( f, sz );
                  } 

                  RESULT = SetCursorNextEntity( vLOD, "Parameter", "" );
                  //:END
               } 


               //:END

               //:wl( f, " " )
               oTZCMLPLO_wl( f, " " );
               //:sz = "      lRC = (zLONG) o" + vLOD.LOD.Name + "_" +
               //:      vLOD.Operation.Name + "( "
               GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
               ZeidonStringCopy( sz, 1, 0, "      lRC = (zLONG) o", 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, szTempString_10, 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, "_", 1, 0, 514 );
               GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vLOD, "Operation", "Name", "", 0 );
               ZeidonStringConcat( sz, 1, 0, szTempString_11, 1, 0, 514 );
               ZeidonStringConcat( sz, 1, 0, "( ", 1, 0, 514 );
               //:l = GetStringLength( sz )
               l = GetStringLength( sz );
               //:sz2 = ""
               ZeidonStringCopy( sz2, 1, 0, "", 1, 0, 514 );
               //:LOOP WHILE l > 0
               while ( l > 0 )
               { 
                  //:sz2 = sz2 + " "
                  ZeidonStringConcat( sz2, 1, 0, " ", 1, 0, 514 );
                  //:l = l - 1
                  l = l - 1;
               } 

               //:END
               //:l = 0
               l = 0;
               //:lViewCount = 0
               lViewCount = 0;
               //:FOR EACH vLOD.Parameter
               RESULT = SetCursorFirstEntity( vLOD, "Parameter", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:l = l + 1
                  l = l + 1;
                  //:ConvertIntegerToString( l, szl, 10 )
                  ConvertIntegerToString( l, szl, 10 );

                  //:IF vLOD.Parameter.DataType = "V"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "V" ) == 0 )
                  { 
                     //:IF vLOD.Parameter.PFlag != "Y"
                     if ( CompareAttributeToString( vLOD, "Parameter", "PFlag", "Y" ) != 0 )
                     { 
                        //:sz = sz + "*"
                        ZeidonStringConcat( sz, 1, 0, "*", 1, 0, 514 );
                     } 

                     //:END

                     //:ConvertIntegerToString( lViewCount, szViewCount, 10 )
                     ConvertIntegerToString( lViewCount, szViewCount, 10 );
                     //:sz= sz + "pvViewList[ " + szViewCount + " ]"
                     ZeidonStringConcat( sz, 1, 0, "pvViewList[ ", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szViewCount, 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, " ]", 1, 0, 514 );
                     //:lViewCount = lViewCount + 1
                     lViewCount = lViewCount + 1;
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "S"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "S" ) == 0 )
                  { 
                     //:sz = sz + "pszArg" + szl
                     ZeidonStringConcat( sz, 1, 0, "pszArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "N"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "N" ) == 0 )
                  { 
                     //:sz = sz + "nArg" + szl
                     ZeidonStringConcat( sz, 1, 0, "nArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "Y"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "Y" ) == 0 )
                  { 
                     //:sz = sz + "cArg" + szl
                     ZeidonStringConcat( sz, 1, 0, "cArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "L"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "L" ) == 0 )
                  { 
                     //:sz = sz + "lArg" + szl
                     ZeidonStringConcat( sz, 1, 0, "lArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                  } 

                  //:END

                  //:IF vLOD.Parameter.DataType = "M"
                  if ( CompareAttributeToString( vLOD, "Parameter", "DataType", "M" ) == 0 )
                  { 
                     //:sz = sz + "dArg" + szl
                     ZeidonStringConcat( sz, 1, 0, "dArg", 1, 0, 514 );
                     ZeidonStringConcat( sz, 1, 0, szl, 1, 0, 514 );
                  } 

                  //:END

                  //:IF l != lArgCount
                  if ( l != lArgCount )
                  { 
                     //:sz = sz + ",     // " + vLOD.Parameter.ShortDesc
                     ZeidonStringConcat( sz, 1, 0, ",     // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_12, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_12, 1, 0, 514 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:sz = sz + " );   // " + vLOD.Parameter.ShortDesc
                     ZeidonStringConcat( sz, 1, 0, " );   // ", 1, 0, 514 );
                     GetVariableFromAttribute( szTempString_13, 0, 'S', 65, vLOD, "Parameter", "ShortDesc", "", 0 );
                     ZeidonStringConcat( sz, 1, 0, szTempString_13, 1, 0, 514 );
                  } 

                  //:END

                  //:wl( f, sz )
                  oTZCMLPLO_wl( f, sz );
                  //:sz = sz2
                  ZeidonStringCopy( sz, 1, 0, sz2, 1, 0, 514 );
                  RESULT = SetCursorNextEntity( vLOD, "Parameter", "" );
               } 

               //:END

               //:wl( f, "      return( lRC );" )
               oTZCMLPLO_wl( f, "      return( lRC );" );
               //:wl( f, "   }" )
               oTZCMLPLO_wl( f, "   }" );
               //:wl( f, " " )
               oTZCMLPLO_wl( f, " " );
               RESULT = SetCursorNextEntityByString( vLOD, "Operation", "Type", "T", "" );
            } 


            //:END // FOR EACH vLOD.Operation...

            //:wl( f, "   return( -1 );" )
            oTZCMLPLO_wl( f, "   return( -1 );" );
            //:wl( f, "}" )
            oTZCMLPLO_wl( f, "}" );
            //:SysCloseFile( vLPLR, f, 0 )
            SysCloseFile( vLPLR, f, 0 );
         } 

         RESULT = SetCursorNextEntityByString( vLOD, "SourceFile", "LanguageType", "V", "" );

         //:END // if f != 0...
      } 

      RESULT = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );

      //:END  // FOR EACH vLOD.SourceFile
   } 


   //:END // FOR EACH vLPLR.W_MetaDef
   return( 0 );
// END // GenTransShell
} 


 
#ifdef __cplusplus
}
#endif
