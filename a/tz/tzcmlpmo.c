#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "TZVMLIP.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZCMLPLO_SetCompilerSpecDir( zVIEW     vSourceLPLR,
                              zVIEW     vCurrentLPLR );


static zSHORT
oTZCMLPLO_SetUniqueSpecName( zVIEW     vCurrentLPLR );


static zSHORT
oTZCMLPLO_SetCompilerPathes( zVIEW     vSource,
                             zVIEW     vTarget,
                             zPCHAR    pszEntityName,
                             zPCHAR    pszAttributeName,
                             zPCHAR    pszDirName,
                             zSHORT    sMaxLen );


//:TRANSFORMATION OPERATION
//:CompilerMigrate( VIEW vCurrentLPLR BASED ON LOD TZCMLPLO,
//:                 VIEW vSourceLPLR  BASED ON LOD TZCMLPLO,
//:                 VIEW vSubtask )

//:   STRING (512) szSrcLPLR
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CompilerMigrate( zVIEW     vCurrentLPLR,
                           zVIEW     vSourceLPLR,
                           zVIEW     vSubtask )
{
   zCHAR     szSrcLPLR[ 513 ] = { 0 }; 
   //:STRING (512) szBaseDir
   zCHAR     szBaseDir[ 513 ] = { 0 }; 
   //:STRING (512) szAktDir
   zCHAR     szAktDir[ 513 ] = { 0 }; 
   zSHORT    RESULT; 


   //:szSrcLPLR = vSourceLPLR.LPLR.Name
   GetVariableFromAttribute( szSrcLPLR, 0, 'S', 513, vSourceLPLR, "LPLR", "Name", "", 0 );
   //:szBaseDir = vSourceLPLR.LPLR.MetaSrcDir
   GetVariableFromAttribute( szBaseDir, 0, 'S', 513, vSourceLPLR, "LPLR", "MetaSrcDir", "", 0 );

   //:FOR EACH vSourceLPLR.Compiler
   RESULT = SetCursorFirstEntity( vSourceLPLR, "Compiler", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:IF vSourceLPLR.Compiler.CompilerName != ""
      if ( CompareAttributeToString( vSourceLPLR, "Compiler", "CompilerName", "" ) != 0 )
      { 

         //:CREATE ENTITY vCurrentLPLR.Compiler
         RESULT = CreateEntity( vCurrentLPLR, "Compiler", zPOS_AFTER );
         //:SetMatchingAttributesByName( vCurrentLPLR, "Compiler",
         //:                             vSourceLPLR,  "Compiler", zSET_NULL )
         SetMatchingAttributesByName( vCurrentLPLR, "Compiler", vSourceLPLR, "Compiler", zSET_NULL );
         //:SetUniqueSpecName( vCurrentLPLR )
         oTZCMLPLO_SetUniqueSpecName( vCurrentLPLR );
         //:SetCompilerSpecDir( vSourceLPLR, vCurrentLPLR )
         oTZCMLPLO_SetCompilerSpecDir( vSourceLPLR, vCurrentLPLR );

         //:// Migrate all Include Directories
         //:FOR EACH vSourceLPLR.Include
         RESULT = SetCursorFirstEntity( vSourceLPLR, "Include", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //: CREATE ENTITY vCurrentLPLR.Include
            RESULT = CreateEntity( vCurrentLPLR, "Include", zPOS_AFTER );
            //: SetMatchingAttributesByName( vCurrentLPLR, "Include",
            //:                              vSourceLPLR,  "Include", zSET_NULL )
            SetMatchingAttributesByName( vCurrentLPLR, "Include", vSourceLPLR, "Include", zSET_NULL );
            //: szAktDir = vSourceLPLR.Include.IncludeDir
            GetVariableFromAttribute( szAktDir, 0, 'S', 513, vSourceLPLR, "Include", "IncludeDir", "", 0 );
            //: SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "Include",
            //:                   "IncludeDir", "Include Dir", "Migrate Compiler Spec", 254 )
            SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "Include", "IncludeDir", "Include Dir", "Migrate Compiler Spec", 254 );
            RESULT = SetCursorNextEntity( vSourceLPLR, "Include", "" );
         } 

         //:END

         //:FOR EACH vSourceLPLR.Lib
         RESULT = SetCursorFirstEntity( vSourceLPLR, "Lib", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //: CREATE ENTITY vCurrentLPLR.Lib
            RESULT = CreateEntity( vCurrentLPLR, "Lib", zPOS_AFTER );
            //: SetMatchingAttributesByName( vCurrentLPLR, "Lib",
            //:                              vSourceLPLR,  "Lib", zSET_NULL )
            SetMatchingAttributesByName( vCurrentLPLR, "Lib", vSourceLPLR, "Lib", zSET_NULL );
            //: szAktDir = vSourceLPLR.Lib.LibDir
            GetVariableFromAttribute( szAktDir, 0, 'S', 513, vSourceLPLR, "Lib", "LibDir", "", 0 );
            //: SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "Lib", "LibDir",
            //:                   "Lib Dir", "Migrate Compiler Spec", 254 )
            SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "Lib", "LibDir", "Lib Dir", "Migrate Compiler Spec", 254 );
            RESULT = SetCursorNextEntity( vSourceLPLR, "Lib", "" );
         } 

         //:END

         //:FOR EACH vSourceLPLR.ExternalTarget
         RESULT = SetCursorFirstEntity( vSourceLPLR, "ExternalTarget", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //: CREATE ENTITY vCurrentLPLR.ExternalTarget
            RESULT = CreateEntity( vCurrentLPLR, "ExternalTarget", zPOS_AFTER );
            //: SetMatchingAttributesByName( vCurrentLPLR, "ExternalTarget",
            //:                              vSourceLPLR,  "ExternalTarget", zSET_NULL )
            SetMatchingAttributesByName( vCurrentLPLR, "ExternalTarget", vSourceLPLR, "ExternalTarget", zSET_NULL );

            //: FOR EACH vSourceLPLR.ExternalObjFile
            RESULT = SetCursorFirstEntity( vSourceLPLR, "ExternalObjFile", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:  CREATE ENTITY vCurrentLPLR.ExternalObjFile
               RESULT = CreateEntity( vCurrentLPLR, "ExternalObjFile", zPOS_AFTER );
               //:  SetMatchingAttributesByName( vCurrentLPLR, "ExternalObjFile",
               //:                              vSourceLPLR,  "ExternalObjFile", zSET_NULL )
               SetMatchingAttributesByName( vCurrentLPLR, "ExternalObjFile", vSourceLPLR, "ExternalObjFile", zSET_NULL );
               //:  szAktDir = vSourceLPLR.ExternalObjFile.Name
               GetVariableFromAttribute( szAktDir, 0, 'S', 513, vSourceLPLR, "ExternalObjFile", "Name", "", 0 );
               //:  SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "ExternalObjFile",
               //:                    "Name", "External Object Dir", "Migrate Compiler Spec", 512 )
               SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "ExternalObjFile", "Name", "External Object Dir", "Migrate Compiler Spec", 512 );
               RESULT = SetCursorNextEntity( vSourceLPLR, "ExternalObjFile", "" );
            } 

            //: END

            //: FOR EACH vSourceLPLR.ExternalLibFile
            RESULT = SetCursorFirstEntity( vSourceLPLR, "ExternalLibFile", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:  CREATE ENTITY vCurrentLPLR.ExternalLibFile
               RESULT = CreateEntity( vCurrentLPLR, "ExternalLibFile", zPOS_AFTER );
               //:  SetMatchingAttributesByName( vCurrentLPLR, "ExternalLibFile",
               //:                               vSourceLPLR,  "ExternalLibFile", zSET_NULL )
               SetMatchingAttributesByName( vCurrentLPLR, "ExternalLibFile", vSourceLPLR, "ExternalLibFile", zSET_NULL );
               //:  szAktDir = vSourceLPLR.ExternalLibFile.Name
               GetVariableFromAttribute( szAktDir, 0, 'S', 513, vSourceLPLR, "ExternalLibFile", "Name", "", 0 );
               //:  SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "ExternalLibFile",
               //:                    "Name", "External Lib Dir", "Migrate Compiler Spec", 512 )
               SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szAktDir, "ExternalLibFile", "Name", "External Lib Dir", "Migrate Compiler Spec", 512 );
               RESULT = SetCursorNextEntity( vSourceLPLR, "ExternalLibFile", "" );
            } 

            RESULT = SetCursorNextEntity( vSourceLPLR, "ExternalTarget", "" );
            //: END
         } 


         //:END
      } 

      RESULT = SetCursorNextEntity( vSourceLPLR, "Compiler", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:HeaderMigrate( VIEW        SourceLPLR BASED ON LOD TZCMLPLO,
//:               STRING (32) HeaderMetaName,
//:               VIEW        NewHeader  BASED ON LOD TZOPHDRO,
//:               VIEW        vSubtask )

//:   VIEW OldHeader   BASED ON LOD TZOPHDRO
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_HeaderMigrate( zVIEW     SourceLPLR,
                         zPCHAR    HeaderMetaName,
                         zPVIEW    NewHeader,
                         zVIEW     vSubtask )
{
   zVIEW     OldHeader = 0; 
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 

   //:STRING ( 513 ) HeaderFileName1  // size according to zMAX_FILESPEC_LTH+1
   zCHAR     HeaderFileName1[ 514 ] = { 0 }; 
   //:STRING ( 513 ) HeaderFileName2  // size according to zMAX_FILESPEC_LTH+1
   zCHAR     HeaderFileName2[ 514 ] = { 0 }; 
   zSHORT    RESULT; 


   //:// Copy the .H file, if it exists, from the source directory to the target directory.
   //:// Note that if it isn't there (it wasn't created in the source LPLR), we're going to
   //:// just ignore it; we're not going to return an error message.
   //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zSOURCE_ERD_META ) // Get view for directory info.
   RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zSOURCE_ERD_META );
   //:ResetViewFromSubobject( CurrentLPLR )
   ResetViewFromSubobject( CurrentLPLR );
   //:HeaderFileName1 = SourceLPLR.LPLR.MetaSrcDir + "\" + HeaderMetaName + ".H"
   GetStringFromAttribute( HeaderFileName1, sizeof( HeaderFileName1 ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( HeaderFileName1, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( HeaderFileName1, 1, 0, HeaderMetaName, 1, 0, 514 );
   ZeidonStringConcat( HeaderFileName1, 1, 0, ".H", 1, 0, 514 );
   //:HeaderFileName2 = CurrentLPLR.LPLR.MetaSrcDir + "\" + HeaderMetaName + ".H"
   GetStringFromAttribute( HeaderFileName2, sizeof( HeaderFileName2 ), CurrentLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( HeaderFileName2, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( HeaderFileName2, 1, 0, HeaderMetaName, 1, 0, 514 );
   ZeidonStringConcat( HeaderFileName2, 1, 0, ".H", 1, 0, 514 );
   //:SysCopyFile( vSubtask, HeaderFileName1, HeaderFileName2, TRUE )
   SysCopyFile( vSubtask, HeaderFileName1, HeaderFileName2, TRUE );
   //:DropView( CurrentLPLR )
   DropView( CurrentLPLR );

   //:// Activate existing source meta OldHeader
   //:HeaderFileName1 = SourceLPLR.LPLR.MetaSrcDir + "\" + HeaderMetaName + ".HDR"
   GetStringFromAttribute( HeaderFileName1, sizeof( HeaderFileName1 ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( HeaderFileName1, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( HeaderFileName1, 1, 0, HeaderMetaName, 1, 0, 514 );
   ZeidonStringConcat( HeaderFileName1, 1, 0, ".HDR", 1, 0, 514 );
   //:ActivateOI_FromFile( OldHeader, "TZOPHDRO", SourceLPLR,
   //:                     HeaderFileName1, 8192 )
   ActivateOI_FromFile( &OldHeader, "TZOPHDRO", SourceLPLR, HeaderFileName1, 8192 );
   //:// 8192 IS zIGNORE_ATTRIB_ERRORS
   //:NAME VIEW OldHeader "OldHeader"
   SetNameForView( OldHeader, "OldHeader", 0, zLEVEL_TASK );
   //:ActivateEmptyMetaOI( vSubtask, NewHeader, zSOURCE_HDR_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, NewHeader, zSOURCE_HDR_META, zSINGLE );
   //:NAME VIEW NewHeader "NewHeader"
   SetNameForView( *NewHeader, "NewHeader", 0, zLEVEL_TASK );

   //:CreateMetaEntity( vSubtask, NewHeader, "HeaderFile", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewHeader, "HeaderFile", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewHeader, "HeaderFile", OldHeader,
   //:                             "HeaderFile", zSET_NULL )
   SetMatchingAttributesByName( *NewHeader, "HeaderFile", OldHeader, "HeaderFile", zSET_NULL );
   //:FOR EACH OldHeader.DefinedItem
   RESULT = SetCursorFirstEntity( OldHeader, "DefinedItem", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:CreateMetaEntity( vSubtask, NewHeader, "DefinedItem", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewHeader, "DefinedItem", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewHeader, "DefinedItem", OldHeader,
      //:                             "DefinedItem", zSET_NULL )
      SetMatchingAttributesByName( *NewHeader, "DefinedItem", OldHeader, "DefinedItem", zSET_NULL );
      RESULT = SetCursorNextEntity( OldHeader, "DefinedItem", "" );
   } 

   //:END

   //:DropObjectInstance( OldHeader )
   DropObjectInstance( OldHeader );
   //:CommitMetaOI( vSubtask, NewHeader, zSOURCE_HDR_META )
   CommitMetaOI( vSubtask, *NewHeader, zSOURCE_HDR_META );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:SetCompilerSpecDir( VIEW vSourceLPLR   BASED ON LOD TZCMLPLO,
//:                    VIEW vCurrentLPLR  BASED ON LOD TZCMLPLO )

//:   STRING (512) szSrcLPLR
static zSHORT
oTZCMLPLO_SetCompilerSpecDir( zVIEW     vSourceLPLR,
                              zVIEW     vCurrentLPLR )
{
   zCHAR     szSrcLPLR[ 513 ] = { 0 }; 
   //:STRING (512) szBaseDir
   zCHAR     szBaseDir[ 513 ] = { 0 }; 
   //:STRING (512) szActDir
   zCHAR     szActDir[ 513 ] = { 0 }; 


   //:szSrcLPLR = vSourceLPLR.LPLR.Name
   GetVariableFromAttribute( szSrcLPLR, 0, 'S', 513, vSourceLPLR, "LPLR", "Name", "", 0 );
   //:szBaseDir = vSourceLPLR.LPLR.MetaSrcDir
   GetVariableFromAttribute( szBaseDir, 0, 'S', 513, vSourceLPLR, "LPLR", "MetaSrcDir", "", 0 );

   //:// set Executable Directory
   //:szActDir  = vSourceLPLR.Compiler.TargetExecutableDir
   GetVariableFromAttribute( szActDir, 0, 'S', 513, vSourceLPLR, "Compiler", "TargetExecutableDir", "", 0 );
   //:SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "TargetExecutableDir",
   //:                  "Target Executable Dir", "Migrate Compiler Spec", 254 )
   SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "TargetExecutableDir", "Target Executable Dir", "Migrate Compiler Spec", 254 );

   //:// set OBJ/LIB Directory
   //:szActDir  = vSourceLPLR.Compiler.TargetObjectDir
   GetVariableFromAttribute( szActDir, 0, 'S', 513, vSourceLPLR, "Compiler", "TargetObjectDir", "", 0 );
   //:SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "TargetObjectDir",
   //:                  "Target Object Dir", "Migrate Compiler Spec", 254 )
   SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "TargetObjectDir", "Target Object Dir", "Migrate Compiler Spec", 254 );

   //:// set Makefile Directory
   //:szActDir  = vSourceLPLR.Compiler.MakefileDir
   GetVariableFromAttribute( szActDir, 0, 'S', 513, vSourceLPLR, "Compiler", "MakefileDir", "", 0 );
   //:SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "MakefileDir",
   //:                  "Makefile Dir", "Migrate Compiler Spec", 254 )
   SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "MakefileDir", "Makefile Dir", "Migrate Compiler Spec", 254 );

   //:// set Resource Directory
   //:szActDir  = vSourceLPLR.Compiler.EnvironmentDir
   GetVariableFromAttribute( szActDir, 0, 'S', 513, vSourceLPLR, "Compiler", "EnvironmentDir", "", 0 );
   //:SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "EnvironmentDir",
   //:                  "Resource Dir", "Migrate Compiler Spec", 254 )
   SetCompSpecPaths( vCurrentLPLR, szBaseDir, szSrcLPLR, szActDir, "Compiler", "EnvironmentDir", "Resource Dir", "Migrate Compiler Spec", 254 );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:SetUniqueSpecName( VIEW vCurrentLPLR )

//:   STRING ( 32 ) szNewName
static zSHORT
oTZCMLPLO_SetUniqueSpecName( zVIEW     vCurrentLPLR )
{
   zCHAR     szNewName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szCurrentName
   zCHAR     szCurrentName[ 33 ] = { 0 }; 
   //:STRING ( 11 ) szSuffix
   zCHAR     szSuffix[ 12 ] = { 0 }; 
   //:STRING ( 10 ) szNumber
   zCHAR     szNumber[ 11 ] = { 0 }; 
   //:INTEGER       lNumber
   zLONG     lNumber = 0; 
   //:INTEGER       lSuffixLen
   zLONG     lSuffixLen = 0; 
   //:INTEGER       lNewLen
   zLONG     lNewLen = 0; 
   //:INTEGER       lCurrentLen
   zLONG     lCurrentLen = 0; 

   //:VIEW vTempName BASED ON LOD TZCMLPLO
   zVIEW     vTempName = 0; 
   zSHORT    RESULT; 


   //:CreateViewFromViewForTask( vTempName, vCurrentLPLR, 0 )
   CreateViewFromViewForTask( &vTempName, vCurrentLPLR, 0 );

   //:// set unique Name
   //:lNumber = 0
   lNumber = 0;
   //:szNewName      = vCurrentLPLR.Compiler.Name
   GetVariableFromAttribute( szNewName, 0, 'S', 33, vCurrentLPLR, "Compiler", "Name", "", 0 );
   //:szCurrentName  = vCurrentLPLR.Compiler.Name
   GetVariableFromAttribute( szCurrentName, 0, 'S', 33, vCurrentLPLR, "Compiler", "Name", "", 0 );
   //:lCurrentLen    = zstrlen( szCurrentName )
   lCurrentLen = zstrlen( szCurrentName );

   //:SET CURSOR FIRST vTempName.Compiler
   //:           WHERE vTempName.Compiler.Name = szNewName
   RESULT = SetCursorFirstEntityByString( vTempName, "Compiler", "Name", szNewName, "" );
   //:SET CURSOR NEXT  vTempName.Compiler
   //:           WHERE vTempName.Compiler.Name = szNewName
   RESULT = SetCursorNextEntityByString( vTempName, "Compiler", "Name", szNewName, "" );

   //:LOOP WHILE RESULT >= zCURSOR_SET
   while ( RESULT >= zCURSOR_SET )
   { 

      //:  lNumber = lNumber + 1
      lNumber = lNumber + 1;
      //:  zltoa( lNumber, szNumber )
      zltoa( lNumber, szNumber, sizeof( szNumber ) );

      //:  szSuffix   = "_" + szNumber
      ZeidonStringCopy( szSuffix, 1, 0, "_", 1, 0, 12 );
      ZeidonStringConcat( szSuffix, 1, 0, szNumber, 1, 0, 12 );
      //:  lSuffixLen = zstrlen( szSuffix )
      lSuffixLen = zstrlen( szSuffix );

      //:  lNewLen = lSuffixLen + lCurrentLen
      lNewLen = lSuffixLen + lCurrentLen;

      //:  IF  lNewLen <= 32
      if ( lNewLen <= 32 )
      { 
         //:   szNewName = szCurrentName + szSuffix
         ZeidonStringCopy( szNewName, 1, 0, szCurrentName, 1, 0, 33 );
         ZeidonStringConcat( szNewName, 1, 0, szSuffix, 1, 0, 33 );
         //:ELSE
      } 
      else
      { 
         //:   lNewLen   = lCurrentLen - lSuffixLen
         lNewLen = lCurrentLen - lSuffixLen;
         //:   szNewName = szCurrentName[1:lNewLen] + szSuffix
         ZeidonStringCopy( szNewName, 1, 0, szCurrentName, 1, lNewLen, 33 );
         ZeidonStringConcat( szNewName, 1, 0, szSuffix, 1, 0, 33 );
      } 

      //:  END

      //:  SET CURSOR FIRST vTempName.Compiler
      //:             WHERE vTempName.Compiler.Name = szNewName
      RESULT = SetCursorFirstEntityByString( vTempName, "Compiler", "Name", szNewName, "" );
   } 


   //:END
   //:DropView( vTempName )
   DropView( vTempName );

   //:vCurrentLPLR.Compiler.Name = szNewName
   SetAttributeFromString( vCurrentLPLR, "Compiler", "Name", szNewName );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:SetCompilerPathes( VIEW vSource,                     // Source LPLR
//:                   VIEW vTarget,                     // Target LPLR
//:                   STRING ( 254 ) pszEntityName,     // EntityName
//:                   STRING ( 254 ) pszAttributeName,  // AttributeName
//:                   STRING ( 100 ) pszDirName,        // Name of actual directory for error message
//:                   SHORT sMaxLen                     // max. Length of directory or directory and
//:                                                     // filename
//:                 )

//:   SHORT sRC
static zSHORT
oTZCMLPLO_SetCompilerPathes( zVIEW     vSource,
                             zVIEW     vTarget,
                             zPCHAR    pszEntityName,
                             zPCHAR    pszAttributeName,
                             zPCHAR    pszDirName,
                             zSHORT    sMaxLen )
{
   zSHORT    sRC = 0; 
   //:SHORT sAnfPos
   zSHORT    sAnfPos = 0; 
   //:SHORT sAnzahl
   zSHORT    sAnzahl = 0; 
   //:SHORT sAktLth
   zSHORT    sAktLth = 0; 
   //:SHORT sBaseLth
   zSHORT    sBaseLth = 0; 
   //:STRING (1024) szNewPath
   zCHAR     szNewPath[ 1025 ] = { 0 }; 
   //:STRING (10) szLPLRName
   zCHAR     szLPLRName[ 11 ] = { 0 }; 
   //:STRING (10) szLPLRNameUpper
   zCHAR     szLPLRNameUpper[ 11 ] = { 0 }; 
   //:STRING (256) szBaseDir
   zCHAR     szBaseDir[ 257 ] = { 0 }; 
   //:STRING (256) szActDir
   zCHAR     szActDir[ 257 ] = { 0 }; 
   //:STRING (256) szBaseDirUpper
   zCHAR     szBaseDirUpper[ 257 ] = { 0 }; 
   //:STRING (256) szActDirUpper
   zCHAR     szActDirUpper[ 257 ] = { 0 }; 
   //:STRING (512) szMsg
   zCHAR     szMsg[ 513 ] = { 0 }; 


   //:GetStringFromAttribute( szBaseDir, vSource, "LPLR", "MetaSrcDir" )
   GetStringFromAttribute( szBaseDir, sizeof( szBaseDir ), vSource, "LPLR", "MetaSrcDir" );
   //:GetStringFromAttribute( szActDir,  vSource, pszEntityName, pszAttributeName )
   GetStringFromAttribute( szActDir, sizeof( szActDir ), vSource, pszEntityName, pszAttributeName );
   //:zToUpper( szBaseDir, szBaseDirUpper)
   zToUpper( szBaseDir, szBaseDirUpper );
   //:zToUpper( szActDir, szActDirUpper)
   zToUpper( szActDir, szActDirUpper );
   //:// Is the Target Executable Directory relativ or fully qualified ??
   //:sRC = GenFullQualPathFromRelPath( szActDir, szBaseDir, szNewPath, sMaxLen)
   sRC = GenFullQualPathFromRelPath( szActDir, szBaseDir, szNewPath, sMaxLen );
   //:CASE sRC
   switch( sRC )
   { 
      //:  OF 0:  // The pathname was relative
      case 0 :
         //:      // We take the name like it is.
         //:      SetAttributeFromString( vTarget, pszEntityName, pszAttributeName, szActDir)
         SetAttributeFromString( vTarget, pszEntityName, pszAttributeName, szActDir );
         break ;
      //:  OF 1:  // The pathname was fully qualified
      case 1 :
         //:      // Is the LPLR-Name included in the pathname ?
         //:      GetStringFromAttribute( szLPLRName, vSource, "LPLR", "Name")
         GetStringFromAttribute( szLPLRName, sizeof( szLPLRName ), vSource, "LPLR", "Name" );
         //:      zToUpper (szLPLRName, szLPLRNameUpper)
         zToUpper( szLPLRName, szLPLRNameUpper );
         //:      sRC = zSearchSubString( szActDirUpper, szLPLRNameUpper, "f", 0)
         sRC = zSearchSubString( szActDirUpper, szLPLRNameUpper, "f", 0 );
         //:      IF sRC = -1
         if ( sRC == -1 )
         { 
            //:      // LPLR-Name not found. Take the pathname like it is
            //:      SetAttributeFromString( vTarget, pszEntityName, pszAttributeName, szActDir)
            SetAttributeFromString( vTarget, pszEntityName, pszAttributeName, szActDir );
            //:   ELSE
         } 
         else
         { 
            //:      // LPLR-Name is included in the pathname
            //:      // See if ComponentSourceDirectory is part of the pathname
            //:      sRC = zSearchSubString ( szActDirUpper, szBaseDirUpper, "f", 0 )
            sRC = zSearchSubString( szActDirUpper, szBaseDirUpper, "f", 0 );
            //:      IF sRC = -1
            if ( sRC == -1 )
            { 
               //:      // Component Source Directory isn't part of the pathname
               //:      szMsg = "Counld't change " + pszDirName + " in CompilerSpec!"
               ZeidonStringCopy( szMsg, 1, 0, "Counld't change ", 1, 0, 513 );
               ZeidonStringConcat( szMsg, 1, 0, pszDirName, 1, 0, 513 );
               ZeidonStringConcat( szMsg, 1, 0, " in CompilerSpec!", 1, 0, 513 );
               //:      TraceLineS( "Migrate Compiler Specification: ", szMsg)
               TraceLineS( "Migrate Compiler Specification: ", szMsg );
               //:   ELSE
            } 
            else
            { 
               //:      // Component Source Directory is part of the pathname
               //:      // Take the new component Source directory and add the end of
               //:      // the original pathname
               //:      sAktLth  = zstrlen (szActDirUpper)
               sAktLth = zstrlen( szActDirUpper );
               //:      sBaseLth = zstrlen (szBaseDirUpper)
               sBaseLth = zstrlen( szBaseDirUpper );
               //:      sAnzahl  = sAktLth - sBaseLth
               sAnzahl = sAktLth - sBaseLth;
               //:      sAnfPos  = sAktLth - sAnzahl + 1
               sAnfPos = sAktLth - sAnzahl + 1;
               //:      GetStringFromAttribute( szBaseDir, vTarget, "LPLR", "MetaSrcDir" )
               GetStringFromAttribute( szBaseDir, sizeof( szBaseDir ), vTarget, "LPLR", "MetaSrcDir" );
               //:      szBaseDir = szBaseDir + szActDir[sAnfPos : sAnzahl]
               ZeidonStringConcat( szBaseDir, 1, 0, szActDir, sAnfPos, sAnzahl, 257 );
               //:      SetAttributeFromString( vTarget, pszEntityName, pszAttributeName, szBaseDir)
               SetAttributeFromString( vTarget, pszEntityName, pszAttributeName, szBaseDir );
            } 

            //:      END
         } 

         //:      END
         break ;
      //:  OTHERWISE : // Error: Pathname exceeds max. length
      default  :
         //:           szMsg = "Path " + pszDirName + " exceeds maximum size !"
         ZeidonStringCopy( szMsg, 1, 0, "Path ", 1, 0, 513 );
         ZeidonStringConcat( szMsg, 1, 0, pszDirName, 1, 0, 513 );
         ZeidonStringConcat( szMsg, 1, 0, " exceeds maximum size !", 1, 0, 513 );
         //:           TraceLineS( "Migrate Compiler Specification: ", szMsg)
         TraceLineS( "Migrate Compiler Specification: ", szMsg );
         //:           return
         return( 0 );
      } 

      //:END
      return( 0 );
   } 


 
#ifdef __cplusplus
}
#endif
