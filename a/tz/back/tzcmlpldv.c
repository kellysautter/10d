#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZVMLIP.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zSHORT zwTZCMLPLD_ExternalLPLR( zVIEW* );
zSHORT zwTZCMLPLD_CheckDuplicateName( zVIEW, zVIEW, zVIEW );
zSHORT zwTZCMLPLD_CheckDuplicateERD_TE( zVIEW, zVIEW, zVIEW );
zSHORT zwTZCMLPLD_CheckDuplicatePPE( zVIEW, zVIEW, zVIEW );
zSHORT zwTZCMLPLD_SearchOldXLPs( zVIEW, zVIEW, zPCHAR );
zSHORT zwTZCMLPLD_LoadRefreshListForCPL( zVIEW, zVIEW, zPCHAR );
zSHORT zwTZCMLPLD_OptionRefreshFromRepo( zVIEW, zVIEW*, zPCHAR, zSHORT );
zSHORT zwTZCMLPLD_GetZeidonApp( zVIEW, zVIEW*, zPCHAR );
zSHORT zwTZCMLPLD_UpdateWorkDirectory( zVIEW, zVIEW );
zSHORT zwTZCMLPLD_GetWorkDir( zVIEW, zPCHAR );
zSHORT zwTZCMLPLD_RebuildMetaLists( zVIEW* );

zOPER_EXPORT zSHORT OPERATION
PostbuildNewLPLR( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
PostbuildImportLPLR( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CANCEL_NewLPLR( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
INITIALIZE_UserStartingGenZKey( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
CREATE_NewLPLR( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
SAVE_NewLPLR( zVIEW     ViewToWindow );


zOPER_EXPORT zSHORT OPERATION
IMPORT_LPLR( zVIEW     ViewToWindow );


static zSHORT
o_GetZeidonApp( zVIEW     ViewToWindow,
                zPVIEW    vKZAPPLOO,
                zPCHAR    szZeidonDirectory );


static zSHORT
o_INITIALIZE_NextZKeyForObject( zVIEW     vMetaOI,
                                zPCHAR    szRootEntityName );


zOPER_EXPORT zSHORT OPERATION
CHANGE_ImportName( zVIEW     ViewToWindow );


static zSHORT
o_CopyContentsOfDirectory( zVIEW     ViewToWindow,
                           zPCHAR    szFromDir,
                           zPCHAR    szToDir,
                           zPCHAR    szCopyFiles );


//:DIALOG OPERATION
//:PostbuildNewLPLR( VIEW ViewToWindow )

//:   VIEW TZCMWKSO REGISTERED AS TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
PostbuildNewLPLR( zVIEW     ViewToWindow )
{
   zVIEW     TZCMWKSO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCMLPLO BASED ON LOD  TZCMLPLO
   zVIEW     TZCMLPLO = 0; 

   RESULT = GetViewByName( &TZCMWKSO, "TZCMWKSO", ViewToWindow, zLEVEL_TASK );

   //:// Initialize an empty LPLO object to hold the keyed in data.
   //:ACTIVATE TZCMLPLO EMPTY
   RESULT = ActivateEmptyObjectInstance( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zSINGLE );
   //:NAME VIEW TZCMLPLO "TZCMLPLO"
   SetNameForView( TZCMLPLO, "TZCMLPLO", 0, zLEVEL_TASK );
   //:CREATE ENTITY TZCMLPLO.LPLR
   RESULT = CreateEntity( TZCMLPLO, "LPLR", zPOS_AFTER );

   //:// New Empty LPLR
   //:TZCMLPLO.LPLR.LPLR_Type = 2
   SetAttributeFromInteger( TZCMLPLO, "LPLR", "LPLR_Type", 2 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:PostbuildImportLPLR( VIEW ViewToWindow )

//:   VIEW TZCMLPLO REGISTERED AS TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
PostbuildImportLPLR( zVIEW     ViewToWindow )
{
   zVIEW     TZCMLPLO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zLEVEL_TASK );

   //:// New Imported (external) LPLR
   //:TZCMLPLO.LPLR.LPLR_Type = 3
   SetAttributeFromInteger( TZCMLPLO, "LPLR", "LPLR_Type", 3 );
   //:SetCtrlState( ViewToWindow, "edName", zCONTROL_STATUS_ENABLED, FALSE )
   SetCtrlState( ViewToWindow, "edName", zCONTROL_STATUS_ENABLED, FALSE );
   //:SetCtrlState( ViewToWindow, "edDesc", zCONTROL_STATUS_ENABLED, FALSE )
   SetCtrlState( ViewToWindow, "edDesc", zCONTROL_STATUS_ENABLED, FALSE );
   //:SetCtrlState( ViewToWindow, "edBaseDir", zCONTROL_STATUS_ENABLED, FALSE )
   SetCtrlState( ViewToWindow, "edBaseDir", zCONTROL_STATUS_ENABLED, FALSE );
   //:SetCtrlState( ViewToWindow, "edExecDir", zCONTROL_STATUS_ENABLED, FALSE )
   SetCtrlState( ViewToWindow, "edExecDir", zCONTROL_STATUS_ENABLED, FALSE );
   //:SetCtrlState( ViewToWindow, "edJavaPackageName", zCONTROL_STATUS_ENABLED, FALSE )
   SetCtrlState( ViewToWindow, "edJavaPackageName", zCONTROL_STATUS_ENABLED, FALSE );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CANCEL_NewLPLR( VIEW ViewToWindow )

//:   VIEW TZCMLPLO REGISTERED AS TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
CANCEL_NewLPLR( zVIEW     ViewToWindow )
{
   zVIEW     TZCMLPLO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zLEVEL_TASK );

   //:DropObjectInstance( TZCMLPLO )
   DropObjectInstance( TZCMLPLO );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:INITIALIZE_UserStartingGenZKey( VIEW ViewToWindow )

//:   VIEW TZCMLPLO REGISTERED AS TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
INITIALIZE_UserStartingGenZKey( zVIEW     ViewToWindow )
{
   zVIEW     TZCMLPLO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCMWKSO REGISTERED AS TZCMWKSO
   zVIEW     TZCMWKSO = 0; 
   //:VIEW vMetaOI
   zVIEW     vMetaOI = 0; 
   //:VIEW vHierMetaOI
   zVIEW     vHierMetaOI = 0; 
   //:STRING ( 255 ) szFullFileName
   zCHAR     szFullFileName[ 256 ] = { 0 }; 
   //:STRING ( 10 )  szSuffix
   zCHAR     szSuffix[ 11 ] = { 0 }; 
   //:STRING ( 32 )  szMetaType
   zCHAR     szMetaType[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szRootEntityName
   zCHAR     szRootEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 500 ) szMsg
   zCHAR     szMsg[ 501 ] = { 0 }; 
   //:INTEGER        hFile
   zLONG     hFile = 0; 
   //:INTEGER        lZKey
   zLONG     lZKey = 0; 
   //:INTEGER        lMaxZKey
   zLONG     lMaxZKey = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCMWKSO, "TZCMWKSO", ViewToWindow, zLEVEL_TASK );

   //:// Initialize the Next ZKey to Assign value for each meta object to one more than the highest ZKey
   //:// Value in the object.
   //:FOR EACH TZCMLPLO.W_MetaType
   RESULT = SetCursorFirstEntity( TZCMLPLO, "W_MetaType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szSuffix = ""
      ZeidonStringCopy( szSuffix, 1, 0, "", 1, 0, 11 );
      //:IF TZCMLPLO.W_MetaType.Type = 2000
      if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2000 ) == 0 )
      { 
         //:szSuffix         = "HDR"
         ZeidonStringCopy( szSuffix, 1, 0, "HDR", 1, 0, 11 );
         //:szMetaType       = "TZOPHDRO"
         ZeidonStringCopy( szMetaType, 1, 0, "TZOPHDRO", 1, 0, 33 );
         //:szRootEntityName = "HeaderFile"
         ZeidonStringCopy( szRootEntityName, 1, 0, "HeaderFile", 1, 0, 33 );
         //:ELSE
      } 
      else
      { 
         //:IF TZCMLPLO.W_MetaType.Type = 2004
         if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2004 ) == 0 )
         { 
            //:szSuffix         = "PMD"
            ZeidonStringCopy( szSuffix, 1, 0, "PMD", 1, 0, 11 );
            //:szMetaType       = "TZEREMDO"
            ZeidonStringCopy( szMetaType, 1, 0, "TZEREMDO", 1, 0, 33 );
            //:szRootEntityName = "EntpER_Model"
            ZeidonStringCopy( szRootEntityName, 1, 0, "EntpER_Model", 1, 0, 33 );
            //:ELSE
         } 
         else
         { 
            //:IF TZCMLPLO.W_MetaType.Type = 2005
            if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2005 ) == 0 )
            { 
               //:szSuffix         = "PSA"
               ZeidonStringCopy( szSuffix, 1, 0, "PSA", 1, 0, 11 );
               //:szMetaType       = "TZERSASO"
               ZeidonStringCopy( szMetaType, 1, 0, "TZERSASO", 1, 0, 33 );
               //:szRootEntityName = "SubjectArea"
               ZeidonStringCopy( szRootEntityName, 1, 0, "SubjectArea", 1, 0, 33 );
               //:ELSE
            } 
            else
            { 
               //:IF TZCMLPLO.W_MetaType.Type = 2006
               if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2006 ) == 0 )
               { 
                  //:szSuffix         = "DTE"
                  ZeidonStringCopy( szSuffix, 1, 0, "DTE", 1, 0, 11 );
                  //:szMetaType       = "TZTENVRO"
                  ZeidonStringCopy( szMetaType, 1, 0, "TZTENVRO", 1, 0, 33 );
                  //:szRootEntityName = "TE_DB_Environ"
                  ZeidonStringCopy( szRootEntityName, 1, 0, "TE_DB_Environ", 1, 0, 33 );
                  //:ELSE
               } 
               else
               { 
                  //:IF TZCMLPLO.W_MetaType.Type = 2007
                  if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2007 ) == 0 )
                  { 
                     //:szSuffix   = "LOD"
                     ZeidonStringCopy( szSuffix, 1, 0, "LOD", 1, 0, 11 );
                     //:szMetaType = "TZZOLODO"
                     ZeidonStringCopy( szMetaType, 1, 0, "TZZOLODO", 1, 0, 33 );
                     //:szRootEntityName = "LOD"
                     ZeidonStringCopy( szRootEntityName, 1, 0, "LOD", 1, 0, 33 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:IF TZCMLPLO.W_MetaType.Type = 2009
                     if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2009 ) == 0 )
                     { 
                        //:szSuffix         = "PVR"
                        ZeidonStringCopy( szSuffix, 1, 0, "PVR", 1, 0, 11 );
                        //:szMetaType       = "TZWDVORO"
                        ZeidonStringCopy( szMetaType, 1, 0, "TZWDVORO", 1, 0, 33 );
                        //:szRootEntityName = "ViewObjRef"
                        ZeidonStringCopy( szRootEntityName, 1, 0, "ViewObjRef", 1, 0, 33 );
                        //:ELSE
                     } 
                     else
                     { 
                        //:IF TZCMLPLO.W_MetaType.Type = 2010
                        if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2010 ) == 0 )
                        { 
                           //:szSuffix         = "PPE"
                           ZeidonStringCopy( szSuffix, 1, 0, "PPE", 1, 0, 11 );
                           //:szMetaType       = "TZPESRCO"
                           ZeidonStringCopy( szMetaType, 1, 0, "TZPESRCO", 1, 0, 33 );
                           //:szRootEntityName = "PresEnvDef"
                           ZeidonStringCopy( szRootEntityName, 1, 0, "PresEnvDef", 1, 0, 33 );
                           //:ELSE
                        } 
                        else
                        { 
                           //:IF TZCMLPLO.W_MetaType.Type = 2011
                           if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2011 ) == 0 )
                           { 
                              //:szSuffix   = "PWD"
                              ZeidonStringCopy( szSuffix, 1, 0, "PWD", 1, 0, 11 );
                              //:szMetaType = "TZWDLGSO"
                              ZeidonStringCopy( szMetaType, 1, 0, "TZWDLGSO", 1, 0, 33 );
                              //:szRootEntityName = "Dialog"
                              ZeidonStringCopy( szRootEntityName, 1, 0, "Dialog", 1, 0, 33 );
                              //:ELSE
                           } 
                           else
                           { 
                              //:IF TZCMLPLO.W_MetaType.Type = 2013
                              if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2013 ) == 0 )
                              { 
                                 //:szSuffix         = "PDG"
                                 ZeidonStringCopy( szSuffix, 1, 0, "PDG", 1, 0, 11 );
                                 //:szMetaType       = "TZDGSRCO"
                                 ZeidonStringCopy( szMetaType, 1, 0, "TZDGSRCO", 1, 0, 33 );
                                 //:szRootEntityName = "DomainGroup"
                                 ZeidonStringCopy( szRootEntityName, 1, 0, "DomainGroup", 1, 0, 33 );
                                 //:ELSE
                              } 
                              else
                              { 
                                 //:IF TZCMLPLO.W_MetaType.Type = 2014
                                 if ( CompareAttributeToInteger( TZCMLPLO, "W_MetaType", "Type", 2014 ) == 0 )
                                 { 
                                    //:szSuffix         = "POG"
                                    ZeidonStringCopy( szSuffix, 1, 0, "POG", 1, 0, 11 );
                                    //:szMetaType       = "TZOGSRCO"
                                    ZeidonStringCopy( szMetaType, 1, 0, "TZOGSRCO", 1, 0, 33 );
                                    //:szRootEntityName = "GlobalOperationGroup"
                                    ZeidonStringCopy( szRootEntityName, 1, 0, "GlobalOperationGroup", 1, 0, 33 );
                                 } 

                                 //:END
                              } 

                              //:END
                           } 

                           //:END
                        } 

                        //:END
                     } 

                     //:END
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:IF szSuffix != ""
      if ( ZeidonStringCompare( szSuffix, 1, 0, "", 1, 0, 11 ) != 0 )
      { 

         //:// Process each meta within the type.
         //:FOR EACH TZCMLPLO.W_MetaDef
         RESULT = SetCursorFirstEntity( TZCMLPLO, "W_MetaDef", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:szFullFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.W_MetaDef.Name + "." + szSuffix
            GetStringFromAttribute( szFullFileName, zsizeof( szFullFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
            ZeidonStringConcat( szFullFileName, 1, 0, "\\", 1, 0, 256 );
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZCMLPLO, "W_MetaDef", "Name", "", 0 );
            ZeidonStringConcat( szFullFileName, 1, 0, szTempString_0, 1, 0, 256 );
            ZeidonStringConcat( szFullFileName, 1, 0, ".", 1, 0, 256 );
            ZeidonStringConcat( szFullFileName, 1, 0, szSuffix, 1, 0, 256 );
            //:hFile = SysOpenFile( TZCMLPLO, szFullFileName, COREFILE_READ )
            hFile = SysOpenFile( TZCMLPLO, szFullFileName, COREFILE_READ );
            //:IF hFile >= 0
            if ( hFile >= 0 )
            { 
               //:SysCloseFile( TZCMLPLO, hFile, 0 )
               SysCloseFile( TZCMLPLO, hFile, 0 );
               //:nRC = ActivateOI_FromFile ( vMetaOI, szMetaType, ViewToWindow, szFullFileName, 8192 ) // 8192 is zIGNORE_ATTRIB_ERRORS
               nRC = ActivateOI_FromFile( &vMetaOI, szMetaType, ViewToWindow, szFullFileName, 8192 );
               //:IF nRC >= 0
               if ( nRC >= 0 )
               { 
                  //:NAME VIEW vMetaOI "MetaAnalyze"
                  SetNameForView( vMetaOI, "MetaAnalyze", 0, zLEVEL_TASK );

                  //:// We have now loaded the meta into memory, so execute INITIALIZE_NextZKeyForObject to initialize the ZKey.
                  //:INITIALIZE_NextZKeyForObject( vMetaOI, szRootEntityName )
                  o_INITIALIZE_NextZKeyForObject( vMetaOI, szRootEntityName );

                  //:DropObjectInstance( vMetaOI )
                  DropObjectInstance( vMetaOI );
               } 

               //:END
            } 

            RESULT = SetCursorNextEntity( TZCMLPLO, "W_MetaDef", "" );
            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZCMLPLO, "W_MetaType", "" );
      //:END
   } 

   //:END
   return( 0 );
//    END
} 


//:DIALOG OPERATION
//:CREATE_NewLPLR( VIEW ViewToWindow )

//:   VIEW TZCMWKSO    REGISTERED AS TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
CREATE_NewLPLR( zVIEW     ViewToWindow )
{
   zVIEW     TZCMWKSO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCMLPLO    REGISTERED AS TZCMLPLO
   zVIEW     TZCMLPLO = 0; 
   //:VIEW KZAPPLOO    BASED ON LOD  KZAPPLOO
   zVIEW     KZAPPLOO = 0; 
   //:VIEW TZCMLPLONew BASED ON LOD  TZCMLPLO
   zVIEW     TZCMLPLONew = 0; 
   //:VIEW TZEREMDO    BASED ON LOD  TZEREMDO
   zVIEW     TZEREMDO = 0; 
   //:VIEW TZTENVRO    BASED ON LOD  TZTENVRO
   zVIEW     TZTENVRO = 0; 
   //:STRING ( 513 ) szMsg
   zCHAR     szMsg[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szTmp
   zCHAR     szTmp[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szFileName
   zCHAR     szFileName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szFromFileName
   zCHAR     szFromFileName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szExeDir
   zCHAR     szExeDir[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szTempDir
   zCHAR     szTempDir[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szLPLRName
   zCHAR     szLPLRName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szOrigLPLRName
   zCHAR     szOrigLPLRName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szJavaPackageName
   zCHAR     szJavaPackageName[ 514 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:INTEGER lHighZKey
   zLONG     lHighZKey = 0; 
   //:INTEGER lHighPrefix
   zLONG     lHighPrefix = 0; 
   //:INTEGER nLth
   zLONG     nLth = 0; 
   //:INTEGER lHandle
   zLONG     lHandle = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_4[ 255 ]; 
   zSHORT    lTempInteger_4; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 255 ]; 
   zCHAR     szTempString_7[ 255 ]; 
   zCHAR     szTempString_8[ 255 ]; 

   RESULT = GetViewByName( &TZCMWKSO, "TZCMWKSO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zLEVEL_TASK );

   //:// KJS 05/18/15 - For right now I am not using this because we were running into errors with
   //:// the xlp, hky file not being created and zeidon.app not updated. I modified nzwTZCMLPLD_CreateNewLPLR to
   //:// create the tzcmulwo entries.

   //:// If the request is to create a new, empty LPLR, use the data entered to create a new one.
   //:// If the request is to create a new entry for an existing, external LPLR, then update the existing LPLR.
   //:szLPLRName = TZCMLPLO.LPLR.Name
   GetVariableFromAttribute( szLPLRName, 0, 'S', 514, TZCMLPLO, "LPLR", "Name", "", 0 );
   //:zSearchAndReplace( szLPLRName, 513, " ", "_" )
   zSearchAndReplace( szLPLRName, 513, " ", "_" );

   //:// Get the max zkey in tzcmwkso? This is code from zwTZCMLPLD_PrebuildNewLPLR, not sure where that was called before...
   //:// And I'm not seeing that the LPLR.ZKey in xlp is the same as the LPLR.Zkey in tzcmwkso.por. Is this a problem?
   //:lHighZKey = 0
   lHighZKey = 0;
   //:FOR EACH TZCMWKSO.LPLR
   RESULT = SetCursorFirstEntity( TZCMWKSO, "LPLR", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZCMWKSO.LPLR.ZKey > lHighZKey
      if ( CompareAttributeToInteger( TZCMWKSO, "LPLR", "ZKey", lHighZKey ) > 0 )
      { 
         //:lHighZKey = TZCMWKSO.LPLR.ZKey
         GetIntegerFromAttribute( &lHighZKey, TZCMWKSO, "LPLR", "ZKey" );
      } 

      RESULT = SetCursorNextEntity( TZCMWKSO, "LPLR", "" );
      //:END
   } 

   //:END
   //:lHighZKey = lHighZKey + 1
   lHighZKey = lHighZKey + 1;

   //:// Make sure there is a correct APPLICATION entry for the new LPLR in the ZEIDON.APP object and save it.
   //:SysGetEnvVar( szFileName, "ZEIDON", 128 )
   SysGetEnvVar( szFileName, "ZEIDON", 128 );
   //:szFileName = szFileName + "\ZEIDON.APP"
   ZeidonStringConcat( szFileName, 1, 0, "\\ZEIDON.APP", 1, 0, 514 );

   //:GET VIEW KZAPPLOO NAMED "KZAPPLOO"
   RESULT = GetViewByName( &KZAPPLOO, "KZAPPLOO", ViewToWindow, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:nRC = ActivateOI_FromFile ( KZAPPLOO, "KZAPPLOO", ViewToWindow, szFileName, 512 )
      nRC = ActivateOI_FromFile( &KZAPPLOO, "KZAPPLOO", ViewToWindow, szFileName, 512 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Configuration Management",
         //:             "An error occurred when reading the ZEIDON.APP file.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Configuration Management", "An error occurred when reading the ZEIDON.APP file.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:DropView( KZAPPLOO )
         DropView( KZAPPLOO );
         //:RETURN -1
         return( -1 );
         //:ELSE
      } 
      else
      { 
         //:NAME VIEW KZAPPLOO "KZAPPLOO"
         SetNameForView( KZAPPLOO, "KZAPPLOO", 0, zLEVEL_TASK );
      } 

      //:END
   } 

   //:END

   //:// KJS 09/12/17 - Thinking I still want to do this even if it's not new... because if I have a different directory to move to...??
   //://IF TZCMLPLO.LPLR.LPLR_Type = "2"    // "2" is new empty
   //:   // LPLR is new empty.
   //:   // Create the following.
   //:   // 1. Build Installation/Users object (TZCMULWO). It will be completed on next window.
   //:   // 2. TZCMLPLO and TZCMWKSO will be updated on next window.

   //:   // Is this right?????
   //:   TZCMLPLO.LPLR.ZKey = lHighZKey
   SetAttributeFromInteger( TZCMLPLO, "LPLR", "ZKey", lHighZKey );

   //:   // KJS 06/05/15 - If the user puts in a directory for the meta source that doesn't
   //:   // exist, do we want to create one for them?
   //:   szFileName = TZCMLPLO.LPLR.MetaSrcDir
   GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );
   //:   IF SysValidDirOrFile( szFileName, TRUE, TRUE, 512) < 0
   lTempInteger_0 = SysValidDirOrFile( szFileName, TRUE, TRUE, 512 );
   if ( lTempInteger_0 < 0 )
   { 
      //:   RETURN -1
      return( -1 );
   } 

   //:   END

   //://END

   //:// We have to create the subdirectory one directory at a time.
   //:szExeDir = TZCMLPLO.LPLR.wExecutableSubDirectory
   GetVariableFromAttribute( szExeDir, 0, 'S', 514, TZCMLPLO, "LPLR", "wExecutableSubDirectory", "", 0 );
   //:TZCMLPLO.LPLR.ExecDir = TZCMLPLO.LPLR.MetaSrcDir + "\" + szExeDir
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZCMLPLO, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szTempString_0, 1, 0, "\\", 1, 0, 255 );
   ZeidonStringConcat( szTempString_0, 1, 0, szExeDir, 1, 0, 255 );
   SetAttributeFromString( TZCMLPLO, "LPLR", "ExecDir", szTempString_0 );
   //:nRC = zSearchSubString( szExeDir, "\"  , "f", 0 )
   nRC = zSearchSubString( szExeDir, "\\", "f", 0 );
   //:LOOP WHILE nRC > 0 
   while ( nRC > 0 )
   { 
      //:ZeidonStringCopy( szTempDir, 1, 0, szExeDir, 1, nRC, 513 )
      ZeidonStringCopy( szTempDir, 1, 0, szExeDir, 1, (zLONG) nRC, 513 );
      //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + szTempDir
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( szFileName, 1, 0, szTempDir, 1, 0, 514 );
      //:IF SysValidDirOrFile( szFileName, TRUE, TRUE, 512) < 0
      lTempInteger_1 = SysValidDirOrFile( szFileName, TRUE, TRUE, 512 );
      if ( lTempInteger_1 < 0 )
      { 
         //:szMsg = "An error occured trying to create the executable directory " + szFileName
         ZeidonStringCopy( szMsg, 1, 0, "An error occured trying to create the executable directory ", 1, 0, 514 );
         ZeidonStringConcat( szMsg, 1, 0, szFileName, 1, 0, 514 );
         //:MessageSend( ViewToWindow, "", "Configuration Management",
         //:             szMsg,
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -1
         return( -1 );
      } 

      //:END
      //:nRC = zSearchSubString( szExeDir, "\"  , "f", nRC + 1)
      nRC = zSearchSubString( szExeDir, "\\", "f", nRC + 1 );
   } 

   //:END

   //:// Set the executable directory. Create it if it doesn't exist and also copy the zeidon.hky if it doesn't exist.
   //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.LPLR.wExecutableSubDirectory
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TZCMLPLO, "LPLR", "wExecutableSubDirectory", "", 0 );
   ZeidonStringConcat( szFileName, 1, 0, szTempString_1, 1, 0, 514 );
   //:SysValidDirOrFile( szFileName, TRUE, TRUE, 512)
   SysValidDirOrFile( szFileName, TRUE, TRUE, 512 );

   //:// If the user has changed the lplr name, then we will copy over the original lplr to the new directory.
   //:IF TZCMLPLO.LPLR.wOrigLPLRName != "" AND TZCMLPLO.LPLR.Name != TZCMLPLO.LPLR.wOrigLPLRName 
   if ( CompareAttributeToString( TZCMLPLO, "LPLR", "wOrigLPLRName", "" ) != 0 && CompareAttributeToAttribute( TZCMLPLO, "LPLR", "Name", TZCMLPLO, "LPLR", "wOrigLPLRName" ) != 0 )
   { 

      //:// This was for testing purposes but at the moment, it looked like the
      //:// zSearchSubString is case sensitive and I don't want it to be.
      //:szFileName = TZCMLPLO.LPLR.JavaPackageName 
      GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "JavaPackageName", "", 0 );
      //:szOrigLPLRName = TZCMLPLO.LPLR.wOrigLPLRName
      GetVariableFromAttribute( szOrigLPLRName, 0, 'S', 514, TZCMLPLO, "LPLR", "wOrigLPLRName", "", 0 );
      //:nRC = zSearchSubString( szFileName, szOrigLPLRName, "f", 0 )
      nRC = zSearchSubString( szFileName, szOrigLPLRName, "f", 0 );
      //:zSearchAndReplace( szFileName, 513, szOrigLPLRName, szLPLRName )
      zSearchAndReplace( szFileName, 513, szOrigLPLRName, szLPLRName );
      //:TZCMLPLO.LPLR.JavaPackageName = szFileName
      SetAttributeFromString( TZCMLPLO, "LPLR", "JavaPackageName", szFileName );
      //:      
      //:szFromFileName = TZCMLPLO.LPLR.wOrigMetaSrc + "\*.*"
      GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
      ZeidonStringConcat( szFromFileName, 1, 0, "\\*.*", 1, 0, 514 );

      //:lHandle = FindFileStart( szFromFileName, szMsg )
      lHandle = FindFileStart( szFromFileName, szMsg );
      //:nRC = 1
      nRC = 1;
      //:LOOP WHILE lHandle >= 0 AND nRC > 0
      while ( lHandle >= 0 && nRC > 0 )
      { 

         //://szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + szMsg
         //:// Don't copy the file if it begins with a period (like .git, .project etc)
         //:nRC = zSearchSubString( szMsg, ".", "f", 0 )
         nRC = zSearchSubString( szMsg, ".", "f", 0 );
         //:IF nRC != 0 
         if ( nRC != 0 )
         { 

            //:// I think we care about DTE, PMD, LLP (don't copy)
            //:szTempDir = TZCMLPLO.LPLR.wOrigLPLRName + ".DTE"
            GetStringFromAttribute( szTempDir, zsizeof( szTempDir ), TZCMLPLO, "LPLR", "wOrigLPLRName" );
            ZeidonStringConcat( szTempDir, 1, 0, ".DTE", 1, 0, 514 );
            //:nRC = zSearchSubString( szMsg, szTempDir, "f", 0 )
            nRC = zSearchSubString( szMsg, szTempDir, "f", 0 );
            //:IF nRC >= 0
            if ( nRC >= 0 )
            { 
               //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.LPLR.Name + ".DTE"
               GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
               ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
               GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZCMLPLO, "LPLR", "Name", "", 0 );
               ZeidonStringConcat( szFileName, 1, 0, szTempString_2, 1, 0, 514 );
               ZeidonStringConcat( szFileName, 1, 0, ".DTE", 1, 0, 514 );

               //:szFromFileName = TZCMLPLO.LPLR.wOrigMetaSrc + "\" + szMsg   
               GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
               ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 514 );
               ZeidonStringConcat( szFromFileName, 1, 0, szMsg, 1, 0, 514 );
               //:nRC = SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
               nRC = SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
               //:nRC = ActivateOI_FromFile ( TZTENVRO, "TZTENVRO", ViewToWindow, szFileName, 512 )
               nRC = ActivateOI_FromFile( &TZTENVRO, "TZTENVRO", ViewToWindow, szFileName, 512 );
               //:TZTENVRO.TE_DB_Environ.Name = TZCMLPLO.LPLR.Name
               SetAttributeFromAttribute( TZTENVRO, "TE_DB_Environ", "Name", TZCMLPLO, "LPLR", "Name" );
               //:nRC = CommitOI_ToFile( TZTENVRO, szFileName, zASCII )
               nRC = CommitOI_ToFile( TZTENVRO, szFileName, zASCII );
               //:ELSE 
            } 
            else
            { 
               //:szTempDir = TZCMLPLO.LPLR.wOrigLPLRName + ".PMD"
               GetStringFromAttribute( szTempDir, zsizeof( szTempDir ), TZCMLPLO, "LPLR", "wOrigLPLRName" );
               ZeidonStringConcat( szTempDir, 1, 0, ".PMD", 1, 0, 514 );
               //:nRC = zSearchSubString( szMsg, szTempDir, "f", 0 )
               nRC = zSearchSubString( szMsg, szTempDir, "f", 0 );
               //:IF nRC >= 0
               if ( nRC >= 0 )
               { 
                  //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.LPLR.Name + ".PMD"
                  GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
                  ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
                  GetVariableFromAttribute( szTempString_3, 0, 'S', 33, TZCMLPLO, "LPLR", "Name", "", 0 );
                  ZeidonStringConcat( szFileName, 1, 0, szTempString_3, 1, 0, 514 );
                  ZeidonStringConcat( szFileName, 1, 0, ".PMD", 1, 0, 514 );
                  //:szFromFileName = TZCMLPLO.LPLR.wOrigMetaSrc + "\" + szMsg   
                  GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
                  ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 514 );
                  ZeidonStringConcat( szFromFileName, 1, 0, szMsg, 1, 0, 514 );
                  //:nRC = SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
                  nRC = SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
                  //:nRC = ActivateOI_FromFile ( TZEREMDO, "TZEREMDO", ViewToWindow, szFileName, 512 ) 
                  nRC = ActivateOI_FromFile( &TZEREMDO, "TZEREMDO", ViewToWindow, szFileName, 512 );
                  //:TZEREMDO.EntpER_Model.Name = TZCMLPLO.LPLR.Name
                  SetAttributeFromAttribute( TZEREMDO, "EntpER_Model", "Name", TZCMLPLO, "LPLR", "Name" );
                  //://CommitMetaOI( ViewToWindow, TZEREMDO, zSOURCE_ERD_META )
                  //:nRC = CommitOI_ToFile( TZEREMDO, szFileName, zASCII )
                  nRC = CommitOI_ToFile( TZEREMDO, szFileName, zASCII );
                  //:ELSE 
               } 
               else
               { 
                  //:szTempDir = TZCMLPLO.LPLR.wOrigLPLRName + ".LLP"
                  GetStringFromAttribute( szTempDir, zsizeof( szTempDir ), TZCMLPLO, "LPLR", "wOrigLPLRName" );
                  ZeidonStringConcat( szTempDir, 1, 0, ".LLP", 1, 0, 514 );
                  //:nRC = zSearchSubString( szMsg, szTempDir, "f", 0 )
                  nRC = zSearchSubString( szMsg, szTempDir, "f", 0 );
                  //:// Don't copy if this is the llp.
                  //:IF nRC < 0
                  if ( nRC < 0 )
                  { 
                     //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + szMsg
                     GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
                     ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
                     ZeidonStringConcat( szFileName, 1, 0, szMsg, 1, 0, 514 );
                     //:szFromFileName = TZCMLPLO.LPLR.wOrigMetaSrc + "\" + szMsg
                     GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
                     ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 514 );
                     ZeidonStringConcat( szFromFileName, 1, 0, szMsg, 1, 0, 514 );
                     //:nRC = SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
                     nRC = SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 


         //:END // Check if file name starts with "."

         //:nRC = FindFileNext( lHandle, szMsg )
         nRC = FindFileNext( lHandle, szMsg );
      } 

      //:END       

      //:// Now Copy files from executable directory.
      //:szFromFileName = TZCMLPLO.LPLR.wOrigExecSrc + "\"
      GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigExecSrc" );
      ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 514 );
      //:szFileName = TZCMLPLO.LPLR.ExecDir + "\"
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "ExecDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
      //:CopyContentsOfDirectory( ViewToWindow, szFromFileName, szFileName, "Y" )
      o_CopyContentsOfDirectory( ViewToWindow, szFromFileName, szFileName, "Y" );
      //:TZCMLPLO.LPLR.wCopiedExecDir = "Y"
      SetAttributeFromString( TZCMLPLO, "LPLR", "wCopiedExecDir", "Y" );

      //:// Copy files from the JavaProject... java directory if it exists.
      //:IF TZCMLPLO.LPLR.JavaPackageName != ""
      if ( CompareAttributeToString( TZCMLPLO, "LPLR", "JavaPackageName", "" ) != 0 )
      { 

         //:szJavaPackageName = TZCMLPLO.LPLR.JavaPackageName 
         GetVariableFromAttribute( szJavaPackageName, 0, 'S', 514, TZCMLPLO, "LPLR", "JavaPackageName", "", 0 );
         //:nRC = zSearchSubString( szJavaPackageName, "."  , "f", 0 )
         nRC = zSearchSubString( szJavaPackageName, ".", "f", 0 );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:zSearchAndReplace( szJavaPackageName, 513, ".", "\" )
            zSearchAndReplace( szJavaPackageName, 513, ".", "\\" );
         } 

         //:END
         //:// At the moment we are going to assume that if you have a JavaPackageName that
         //:// you will have a \javaProject\src\main" + javaPackageName directory to store your 
         //:// java code.
         //:szTmp = "JavaProject\src\main\java\" + szJavaPackageName + "\"
         ZeidonStringCopy( szTmp, 1, 0, "JavaProject\\src\\main\\java\\", 1, 0, 514 );
         ZeidonStringConcat( szTmp, 1, 0, szJavaPackageName, 1, 0, 514 );
         ZeidonStringConcat( szTmp, 1, 0, "\\", 1, 0, 514 );
         //:nRC = zSearchSubString( szTmp, "\"  , "f", 0 )
         nRC = zSearchSubString( szTmp, "\\", "f", 0 );
         //:LOOP WHILE nRC > 0 
         while ( nRC > 0 )
         { 
            //:ZeidonStringCopy( szTempDir, 1, 0, szTmp, 1, nRC, 513 )
            ZeidonStringCopy( szTempDir, 1, 0, szTmp, 1, (zLONG) nRC, 513 );
            //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + szTempDir
            GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
            ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
            ZeidonStringConcat( szFileName, 1, 0, szTempDir, 1, 0, 514 );
            //:IF SysValidDirOrFile( szFileName, TRUE, TRUE, 512) < 0
            lTempInteger_2 = SysValidDirOrFile( szFileName, TRUE, TRUE, 512 );
            if ( lTempInteger_2 < 0 )
            { 
               //:szMsg = "An error occured trying to create the java project directory " + szFileName
               ZeidonStringCopy( szMsg, 1, 0, "An error occured trying to create the java project directory ", 1, 0, 514 );
               ZeidonStringConcat( szMsg, 1, 0, szFileName, 1, 0, 514 );
               //:MessageSend( ViewToWindow, "", "Configuration Management",
               //:             szMsg,
               //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
               SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
               //:RETURN -1
               return( -1 );
            } 

            //:END
            //:nRC = zSearchSubString( szTmp, "\"  , "f", nRC + 1)
            nRC = zSearchSubString( szTmp, "\\", "f", nRC + 1 );
         } 

         //:END

         //:szTmp = TZCMLPLO.LPLR.wOrigJavaPackage  
         GetVariableFromAttribute( szTmp, 0, 'S', 514, TZCMLPLO, "LPLR", "wOrigJavaPackage", "", 0 );
         //:IF zSearchSubString( szTmp, "."  , "f", 0 ) >= 0
         lTempInteger_3 = zSearchSubString( szTmp, ".", "f", 0 );
         if ( lTempInteger_3 >= 0 )
         { 
            //:zSearchAndReplace( szTmp, 513, ".", "\" )
            zSearchAndReplace( szTmp, 513, ".", "\\" );
         } 

         //:END

         //:TZCMLPLO.LPLR.wOrigJavaPackage = szTmp
         SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigJavaPackage", szTmp );
         //:// Copy java files
         //:szFromFileName = TZCMLPLO.LPLR.wOrigMetaSrc + "\JavaProject\src\main\java\" + szTmp + "\"
         GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
         ZeidonStringConcat( szFromFileName, 1, 0, "\\JavaProject\\src\\main\\java\\", 1, 0, 514 );
         ZeidonStringConcat( szFromFileName, 1, 0, szTmp, 1, 0, 514 );
         ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 514 );
         //:TZCMLPLO.LPLR.wOrigJavaPackage = szFromFileName
         SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigJavaPackage", szFromFileName );

         //:szTempDir = TZCMLPLO.LPLR.MetaSrcDir + "\JavaProject\src\main\java\" + szJavaPackageName + "\"
         GetStringFromAttribute( szTempDir, zsizeof( szTempDir ), TZCMLPLO, "LPLR", "MetaSrcDir" );
         ZeidonStringConcat( szTempDir, 1, 0, "\\JavaProject\\src\\main\\java\\", 1, 0, 514 );
         ZeidonStringConcat( szTempDir, 1, 0, szJavaPackageName, 1, 0, 514 );
         ZeidonStringConcat( szTempDir, 1, 0, "\\", 1, 0, 514 );
         //:szJavaPackageName = szTempDir
         ZeidonStringCopy( szJavaPackageName, 1, 0, szTempDir, 1, 0, 514 );
         //:CopyContentsOfDirectory( ViewToWindow, szFromFileName, szJavaPackageName, "Y" )
         o_CopyContentsOfDirectory( ViewToWindow, szFromFileName, szJavaPackageName, "Y" );
         //:TZCMLPLO.LPLR.wCopiedJavaDir = "Y"
         SetAttributeFromString( TZCMLPLO, "LPLR", "wCopiedJavaDir", "Y" );
      } 


      //:END // If JavaProjectName exists
      //:      
      //:// Now copy any other directories that exist.

      //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\"
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
      //:szFromFileName = TZCMLPLO.LPLR.wOrigMetaSrc + "\"
      GetStringFromAttribute( szFromFileName, zsizeof( szFromFileName ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
      ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 514 );

      //:CopyContentsOfDirectory( ViewToWindow, szFromFileName, szFileName, "N" )
      o_CopyContentsOfDirectory( ViewToWindow, szFromFileName, szFileName, "N" );
   } 

   //: 
   //:END  // If the user has changed the lplr name

   //:// Set the executable directory. Create it if it doesn't exist and also copy the zeidon.hky if it doesn't exist.
   //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.LPLR.wExecutableSubDirectory
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
   GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TZCMLPLO, "LPLR", "wExecutableSubDirectory", "", 0 );
   ZeidonStringConcat( szFileName, 1, 0, szTempString_4, 1, 0, 514 );

   //:TZCMLPLO.LPLR.ExecDir = szFileName
   SetAttributeFromString( TZCMLPLO, "LPLR", "ExecDir", szFileName );


   //:// Copy the Hotkey file "ZEIDON.HKY" to the new LPLR executable directory.
   //:szFileName = szFileName + "\ZEIDON.HKY"
   ZeidonStringConcat( szFileName, 1, 0, "\\ZEIDON.HKY", 1, 0, 514 );
   //:nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 )
   nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 );
   //:IF nRC <= 0
   if ( nRC <= 0 )
   { 
      //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
      GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
      //:szFromFileName = szFromFileName + "\ZEIDON.HKY"
      ZeidonStringConcat( szFromFileName, 1, 0, "\\ZEIDON.HKY", 1, 0, 514 );
      //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
      SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
   } 

   //:END

   //:IF TZCMLPLO.LPLR.Name != TZCMLPLO.LPLR.wOrigLPLRName 
   if ( CompareAttributeToAttribute( TZCMLPLO, "LPLR", "Name", TZCMLPLO, "LPLR", "wOrigLPLRName" ) != 0 )
   { 

      //:// If we are creating a new lplr...
      //:// Now check kzmsgqoo, and tzzolodo if the directory "NewLPLR" exists under zeidon executable directory.
      //:szFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS + "\NewLPLR"
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), KZAPPLOO, "ZEIDON", "ZEIDON_SYS" );
      ZeidonStringConcat( szFileName, 1, 0, "\\NewLPLR", 1, 0, 514 );
      //:IF TZCMLPLO.LPLR.LPLR_Type = "2" AND SysValidDirOrFile( szFileName, TRUE, FALSE, 512 ) > 0
      lTempInteger_4 = SysValidDirOrFile( szFileName, TRUE, FALSE, 512 );
      if ( CompareAttributeToString( TZCMLPLO, "LPLR", "LPLR_Type", "2" ) == 0 && lTempInteger_4 > 0 )
      { 

         //:szFileName = TZCMLPLO.LPLR.MetaSrcDir
         GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );
         //:szFileName = szFileName + "\KZMSGQOO.LOD"
         ZeidonStringConcat( szFileName, 1, 0, "\\KZMSGQOO.LOD", 1, 0, 514 );
         //:nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 )
         nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 );
         //:IF nRC <= 0
         if ( nRC <= 0 )
         { 
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\KZMSGQOO.LOD"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\KZMSGQOO.LOD", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
            //:szFileName = TZCMLPLO.LPLR.ExecDir + "\KZMSGQOO.XOD"
            GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "ExecDir" );
            ZeidonStringConcat( szFileName, 1, 0, "\\KZMSGQOO.XOD", 1, 0, 514 );
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\KZMSGQOO.XOD"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\KZMSGQOO.XOD", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
         } 

         //:END
         //:szFileName = TZCMLPLO.LPLR.MetaSrcDir
         GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );
         //:szFileName = szFileName + "\TZZOLFLO.LOD"
         ZeidonStringConcat( szFileName, 1, 0, "\\TZZOLFLO.LOD", 1, 0, 514 );
         //:nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 )
         nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 );
         //:IF nRC <= 0
         if ( nRC <= 0 )
         { 
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\TZZOLFLO.LOD"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\TZZOLFLO.LOD", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
            //:szFileName = TZCMLPLO.LPLR.ExecDir + "\TZZOLFLO.XOD"
            GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "ExecDir" );
            ZeidonStringConcat( szFileName, 1, 0, "\\TZZOLFLO.XOD", 1, 0, 514 );
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\TZZOLFLO.XOD"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\TZZOLFLO.XOD", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
         } 

         //:END
         //:szFileName = TZCMLPLO.LPLR.MetaSrcDir
         GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );
         //:szFileName = szFileName + "\TZAPDMAA.PDG"
         ZeidonStringConcat( szFileName, 1, 0, "\\TZAPDMAA.PDG", 1, 0, 514 );
         //:nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 )
         nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 );
         //:IF nRC <= 0
         if ( nRC <= 0 )
         { 
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\TZAPDMAA.PDG"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\TZAPDMAA.PDG", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
         } 

         //:END
         //:szFileName = TZCMLPLO.LPLR.MetaSrcDir
         GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );
         //:szFileName = szFileName + "\TZAPDMAB.PDG"
         ZeidonStringConcat( szFileName, 1, 0, "\\TZAPDMAB.PDG", 1, 0, 514 );
         //:nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 )
         nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 );
         //:IF nRC <= 0
         if ( nRC <= 0 )
         { 
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\TZAPDMAB.PDG"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\TZAPDMAB.PDG", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
         } 

         //:END
         //:szFileName = TZCMLPLO.LPLR.MetaSrcDir
         GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );
         //:szFileName = szFileName + "\TZAPDMAC.PDG"
         ZeidonStringConcat( szFileName, 1, 0, "\\TZAPDMAC.PDG", 1, 0, 514 );
         //:nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 )
         nRC = SysValidDirOrFile( szFileName, FALSE, FALSE, 512 );
         //:IF nRC <= 0
         if ( nRC <= 0 )
         { 
            //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
            GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
            //:szFromFileName = szFromFileName + "\NewLPLR\TZAPDMAC.PDG"
            ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\TZAPDMAC.PDG", 1, 0, 514 );
            //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
            SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
         } 

         //:END

         //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\KZMSGQOO_Object.java"
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
         ZeidonStringConcat( szFileName, 1, 0, "\\KZMSGQOO_Object.java", 1, 0, 514 );
         //:szFromFileName = KZAPPLOO.ZEIDON.ZEIDON_SYS
         GetVariableFromAttribute( szFromFileName, 0, 'S', 514, KZAPPLOO, "ZEIDON", "ZEIDON_SYS", "", 0 );
         //:szFromFileName = szFromFileName + "\NewLPLR\KZMSGQOO_Object.java"
         ZeidonStringConcat( szFromFileName, 1, 0, "\\NewLPLR\\KZMSGQOO_Object.java", 1, 0, 514 );
         //:SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE )
         SysCopyFile( ViewToWindow, szFromFileName, szFileName, TRUE );
      } 

      //:END
   } 


   //:END

   //:TZCMLPLO.LPLR.PgmSrcDir = TZCMLPLO.LPLR.MetaSrcDir
   SetAttributeFromAttribute( TZCMLPLO, "LPLR", "PgmSrcDir", TZCMLPLO, "LPLR", "MetaSrcDir" );
   //:TZCMLPLO.LPLR.TempDesc = TZCMLPLO.LPLR.Desc
   SetAttributeFromAttribute( TZCMLPLO, "LPLR", "TempDesc", TZCMLPLO, "LPLR", "Desc" );

   //:// TZCMWKSO
   //:// Make sure that the LPLR object exists in TZCMWKSO.
   //:SET CURSOR FIRST TZCMWKSO.LPLR WHERE TZCMWKSO.LPLR.Name = TZCMLPLO.LPLR.Name
   GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), TZCMLPLO, "LPLR", "Name" );
   RESULT = SetCursorFirstEntityByString( TZCMWKSO, "LPLR", "Name", szTempString_5, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CREATE ENTITY TZCMWKSO.LPLR
      RESULT = CreateEntity( TZCMWKSO, "LPLR", zPOS_AFTER );
      //:TZCMWKSO.LPLR.LPLR_Type = "2"
      SetAttributeFromString( TZCMWKSO, "LPLR", "LPLR_Type", "2" );
      //:TZCMWKSO.LPLR.Name      = TZCMLPLO.LPLR.Name
      SetAttributeFromAttribute( TZCMWKSO, "LPLR", "Name", TZCMLPLO, "LPLR", "Name" );
      //:TZCMWKSO.LPLR.ZKey = lHighZKey
      SetAttributeFromInteger( TZCMWKSO, "LPLR", "ZKey", lHighZKey );
   } 

   //:END
   //:TZCMWKSO.LPLR.Desc       = TZCMLPLO.LPLR.Desc
   SetAttributeFromAttribute( TZCMWKSO, "LPLR", "Desc", TZCMLPLO, "LPLR", "Desc" );
   //:TZCMWKSO.LPLR.MetaSrcDir = TZCMLPLO.LPLR.MetaSrcDir
   SetAttributeFromAttribute( TZCMWKSO, "LPLR", "MetaSrcDir", TZCMLPLO, "LPLR", "MetaSrcDir" );
   //:TZCMWKSO.LPLR.PgmSrcDir  = TZCMLPLO.LPLR.MetaSrcDir
   SetAttributeFromAttribute( TZCMWKSO, "LPLR", "PgmSrcDir", TZCMLPLO, "LPLR", "MetaSrcDir" );
   //:TZCMWKSO.LPLR.ExecDir    = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.LPLR.wExecutableSubDirectory
   GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZCMLPLO, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szTempString_6, 1, 0, "\\", 1, 0, 255 );
   GetVariableFromAttribute( szTempString_7, 0, 'S', 255, TZCMLPLO, "LPLR", "wExecutableSubDirectory", "", 0 );
   ZeidonStringConcat( szTempString_6, 1, 0, szTempString_7, 1, 0, 255 );
   SetAttributeFromString( TZCMWKSO, "LPLR", "ExecDir", szTempString_6 );
   //://TZCMWKSO.LPLR.MaxZKey    = TZCMULWO.User.GenerationStartZKey

   //:TZCMWKSO.RepositoryClient.DefaultLPLR_ZKey = TZCMWKSO.LPLR.ZKey
   SetAttributeFromAttribute( TZCMWKSO, "RepositoryClient", "DefaultLPLR_ZKey", TZCMWKSO, "LPLR", "ZKey" );

   //:SysGetEnvVar( szFileName, "ZEIDON", 128 )
   SysGetEnvVar( szFileName, "ZEIDON", 128 );
   //:szFileName = szFileName + "\TZCMWKS8.POR"
   ZeidonStringConcat( szFileName, 1, 0, "\\TZCMWKS8.POR", 1, 0, 514 );
   //:nRC = CommitOI_ToFile( TZCMWKSO, szFileName, zASCII )
   nRC = CommitOI_ToFile( TZCMWKSO, szFileName, zASCII );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Save New LPLR",
      //:             "An error occurred when writing the TZCMWKSO file. You will have to repeat the function.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Save New LPLR", "An error occurred when writing the TZCMWKSO file. You will have to repeat the function.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// TZCMLPLO
   //:IF TZCMLPLO.LPLR.wExecutableSubDirectory = ""
   if ( CompareAttributeToString( TZCMLPLO, "LPLR", "wExecutableSubDirectory", "" ) == 0 )
   { 
      //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + szLPLRName + ".XLP"
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( szFileName, 1, 0, szLPLRName, 1, 0, 514 );
      ZeidonStringConcat( szFileName, 1, 0, ".XLP", 1, 0, 514 );
      //:ELSE
   } 
   else
   { 
      //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMLPLO.LPLR.wExecutableSubDirectory + "\" + szLPLRName + ".XLP"
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 255, TZCMLPLO, "LPLR", "wExecutableSubDirectory", "", 0 );
      ZeidonStringConcat( szFileName, 1, 0, szTempString_8, 1, 0, 514 );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( szFileName, 1, 0, szLPLRName, 1, 0, 514 );
      ZeidonStringConcat( szFileName, 1, 0, ".XLP", 1, 0, 514 );
   } 

   //:END

   //:// KJS 06/05/15 - Do we need to create this new one, or can we just save this with the new base directory?
   //:/*
   //:nRC = ActivateOI_FromFile ( TZCMLPLONew, "TZCMLPLO", ViewToWindow, szFileName, 512 )
   //:IF nRC < 0
   //:   szMsg = "An XLP object (TZCMLPLO) does not exist in the directory path specified: " + NEW_LINE + "  " + szFileName + "."
   //:   MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
   //:   SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
   //:   DropView( TZCMULWO )
   //:   RETURN -1
   //:ELSE
   //:   TZCMLPLONew.LPLR.LPLR_Type  = TZCMLPLO.LPLR.LPLR_Type    // We will use Type on next window.
   //:   TZCMLPLONew.LPLR.MetaSrcDir = TZCMLPLO.LPLR.MetaSrcDir
   //:   TZCMLPLONew.LPLR.PgmSrcDir  = TZCMLPLO.LPLR.MetaSrcDir
   //:   TZCMLPLONew.LPLR.ExecDir    = TZCMLPLO.LPLR.MetaSrcDir + "\" + TZCMULWO.Installation.ExecutableSubDirectory
   //:   DropObjectInstance( TZCMLPLO )
   //:   NAME VIEW TZCMLPLONew "TZCMLPLO"
   //:END
   //:*/

   //:TZCMLPLO.LPLR.wOrigLPLRName = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigLPLRName", "" );
   //:TZCMLPLO.LPLR.wOrigMetaSrc = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigMetaSrc", "" );
   //:TZCMLPLO.LPLR.wOrigExecSrc = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigExecSrc", "" );
   //:TZCMLPLO.LPLR.wOrigJavaPackage = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigJavaPackage", "" );
   //:TZCMLPLO.LPLR.wCopiedExecDir = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wCopiedExecDir", "" );
   //:TZCMLPLO.LPLR.wCopiedJavaDir = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wCopiedJavaDir", "" );

   //:// Commit the XLP in the executable directory, and then commit the LLP in the base directory.
   //:CommitOI_ToFile( TZCMLPLO, szFileName, zSINGLE )
   CommitOI_ToFile( TZCMLPLO, szFileName, zSINGLE );
   //:szFileName = TZCMLPLO.LPLR.MetaSrcDir + "\" + szLPLRName + ".LLP"
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), TZCMLPLO, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( szFileName, 1, 0, szLPLRName, 1, 0, 514 );
   ZeidonStringConcat( szFileName, 1, 0, ".LLP", 1, 0, 514 );
   //:CommitOI_ToFile( TZCMLPLO, szFileName, zSINGLE )
   CommitOI_ToFile( TZCMLPLO, szFileName, zSINGLE );

   //:// Make sure the APPLICATION entry exists and is correct.
   //:SET CURSOR FIRST KZAPPLOO.APPLICATION WHERE KZAPPLOO.APPLICATION.APP_NAME = TZCMLPLO.LPLR.Name
   GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), TZCMLPLO, "LPLR", "Name" );
   RESULT = SetCursorFirstEntityByString( KZAPPLOO, "APPLICATION", "APP_NAME", szTempString_5, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CREATE ENTITY KZAPPLOO.APPLICATION
      RESULT = CreateEntity( KZAPPLOO, "APPLICATION", zPOS_AFTER );
      //:KZAPPLOO.APPLICATION.APP_NAME = TZCMLPLO.LPLR.Name
      SetAttributeFromAttribute( KZAPPLOO, "APPLICATION", "APP_NAME", TZCMLPLO, "LPLR", "Name" );
   } 

   //:END
   //:KZAPPLOO.APPLICATION.APP_DLL    = TZCMLPLO.LPLR.ExecDir
   SetAttributeFromAttribute( KZAPPLOO, "APPLICATION", "APP_DLL", TZCMLPLO, "LPLR", "ExecDir" );
   //:KZAPPLOO.APPLICATION.APP_ADOBIN = TZCMLPLO.LPLR.ExecDir
   SetAttributeFromAttribute( KZAPPLOO, "APPLICATION", "APP_ADOBIN", TZCMLPLO, "LPLR", "ExecDir" );
   //:KZAPPLOO.APPLICATION.APP_LOCAL  = KZAPPLOO.ZEIDON.ZEIDON_LOC
   SetAttributeFromAttribute( KZAPPLOO, "APPLICATION", "APP_LOCAL", KZAPPLOO, "ZEIDON", "ZEIDON_LOC" );
   //:KZAPPLOO.APPLICATION.APP_SHARED = KZAPPLOO.ZEIDON.ZEIDON_SHR
   SetAttributeFromAttribute( KZAPPLOO, "APPLICATION", "APP_SHARED", KZAPPLOO, "ZEIDON", "ZEIDON_SHR" );

   //:SysGetEnvVar( szFileName, "ZEIDON", 128 )
   SysGetEnvVar( szFileName, "ZEIDON", 128 );
   //:szFileName = szFileName + "\ZEIDON.APP"
   ZeidonStringConcat( szFileName, 1, 0, "\\ZEIDON.APP", 1, 0, 514 );

   //:// Commit the ZEIDON.APP object.
   //:nRC = CommitOI_ToFile( KZAPPLOO, szFileName, zASCII )
   nRC = CommitOI_ToFile( KZAPPLOO, szFileName, zASCII );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Save New LPLR",
      //:             "An error occurred when writing the ZEIDON.APP file. You will have to repeat the function.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Save New LPLR", "An error occurred when writing the ZEIDON.APP file. You will have to repeat the function.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Save view TZCMLPLO to view TaskLPLR because thi
   //:NAME VIEW TZCMLPLO "TaskLPLR"
   SetNameForView( TZCMLPLO, "TaskLPLR", 0, zLEVEL_TASK );

   //:RebuildMetaLists( ViewToWindow )
   RebuildMetaLists( ViewToWindow );
   //:InitializeLPLR( ViewToWindow, szLPLRName )
   InitializeLPLR( ViewToWindow, szLPLRName );
   //://CommitMetaOI( ViewToWindow, TZEREMDO, zSOURCE_ERD_META )
   //://CommitMetaOI( ViewToWindow, TZTENVRO, zSOURCE_DTE_META )
   //:RebuildXDM( ViewToWindow )
   RebuildXDM( ViewToWindow );
   return( 0 );
//    // Right now we run into an error in RebuildXODs because we don't have the TE set up.
//    // It's looking for view "TE_DB_Environ" and entity "TE_DBMS_Source".
//    // So I will come back to this, do we want to set up an initial te here?
//    //RebuildXODs( ViewToWindow )
// END
} 


//:DIALOG OPERATION
//:SAVE_NewLPLR( VIEW ViewToWindow )

//:   VIEW TZCMWKSO    REGISTERED AS TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
SAVE_NewLPLR( zVIEW     ViewToWindow )
{
   zVIEW     TZCMWKSO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCMLPLO    REGISTERED AS TZCMLPLO
   zVIEW     TZCMLPLO = 0; 
   //:VIEW KZAPPLOO    BASED ON LOD  KZAPPLOO
   zVIEW     KZAPPLOO = 0; 
   //:STRING (513) szFileName
   zCHAR     szFileName[ 514 ] = { 0 }; 
   //:STRING (513) szDirectoryName
   zCHAR     szDirectoryName[ 514 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 

   RESULT = GetViewByName( &TZCMWKSO, "TZCMWKSO", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zLEVEL_TASK );
   return( 0 );
//    // Make sure that a subdirectory is specified and then update and save the three objects above.
//    /*IF TZCMULWO.Installation.ExecutableSubDirectory = ""
//       MessageSend( ViewToWindow, "", "Save New LPLR",
//                    "A Subdirectory value must be specified.",
//                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
//       SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
//       RETURN -1
//    END*/
//    // Save the XLP object.
//    // KJS 06/10/15 - Already done in CreateNewLPLR.
//    /*
//    szFileName = TZCMLPLO.LPLR.ExecDir + "\" + TZCMLPLO.LPLR.Name  + ".XLP"
//    nRC = CommitOI_ToFile( TZCMLPLO, szFileName, zASCII )
//    IF nRC < 0
//       MessageSend( ViewToWindow, "", "Save New LPLR",
//                    "An error occurred when writing the TZCMLPLO file. You will have to repeat the function.",
//                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
//       SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
//       RETURN -1
//    END
//    // Save the TZCMWKSO object.
//    SysGetEnvVar( szDirectoryName, "ZEIDON", 128 )
//    szFileName = szDirectoryName + "\TZCMWKS8.POR"
// TraceLineS( "*** WKS File: ", szDirectoryName )
//    nRC = CommitOI_ToFile( TZCMWKSO, szFileName, zASCII )
//    IF nRC < 0
//       MessageSend( ViewToWindow, "", "Save New LPLR",
//                    "An error occurred when writing the TZCMWKSO file. You will have to repeat the function.",
//                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
//       SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
//       RETURN -1
//    END
//    // Make sure there is a correct APPLICATION entry for the new LPLR in the ZEIDON.APP object and save it.
//    SysGetEnvVar( szDirectoryName, "ZEIDON", 128 )
//    szFileName = szDirectoryName + "\ZEIDON.APP"
//    nRC = ActivateOI_FromFile ( KZAPPLOO, "KZAPPLOO", ViewToWindow, szFileName, 512 )
//    IF nRC < 0
//       MessageSend( ViewToWindow, "", "Configuration Management",
//                    "An error occurred when reading the ZEIDON.APP file.",
//                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
//       SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
//       DropView( KZAPPLOO )
//       RETURN -1
//    ELSE
//       NAME VIEW KZAPPLOO "KZAPPLOO"
//    END
// TraceLineS( "*** APP File: ", szDirectoryName )
//    // Make sure the APPLICATION entry exists and is correct.
//    SET CURSOR FIRST KZAPPLOO.APPLICATION WHERE KZAPPLOO.APPLICATION.APP_NAME = TZCMLPLO.LPLR.Name
//    IF RESULT < zCURSOR_SET
//       CREATE ENTITY KZAPPLOO.APPLICATION
//       KZAPPLOO.APPLICATION.APP_NAME = TZCMLPLO.LPLR.Name
//    END
//    KZAPPLOO.APPLICATION.APP_DLL    = TZCMLPLO.LPLR.ExecDir
//    KZAPPLOO.APPLICATION.APP_ADOBIN = TZCMLPLO.LPLR.ExecDir
//    KZAPPLOO.APPLICATION.APP_LOCAL  = KZAPPLOO.ZEIDON.ZEIDON_LOC
//    KZAPPLOO.APPLICATION.APP_SHARED = KZAPPLOO.ZEIDON.ZEIDON_SHR
//    // Commit the ZEIDON.APP object.
//    nRC = CommitOI_ToFile( KZAPPLOO, szFileName, zASCII )
//    IF nRC < 0
//       MessageSend( ViewToWindow, "", "Save New LPLR",
//                    "An error occurred when writing the ZEIDON.APP file. You will have to repeat the function.",
//                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
//       SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
//       RETURN -1
//    END
// */
// END
} 


//:DIALOG OPERATION
//:IMPORT_LPLR( VIEW ViewToWindow )

//:   VIEW TZCMWKSO    REGISTERED AS TZCMWKSO
zOPER_EXPORT zSHORT OPERATION
IMPORT_LPLR( zVIEW     ViewToWindow )
{
   zVIEW     TZCMWKSO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCMLPLO    BASED ON LOD  TZCMLPLO
   zVIEW     TZCMLPLO = 0; 
   //:VIEW TZCMLPLONew BASED ON LOD  TZCMLPLO
   zVIEW     TZCMLPLONew = 0; 
   //:VIEW KZAPPLOO    BASED ON LOD  KZAPPLOO
   zVIEW     KZAPPLOO = 0; 
   //:VIEW KZXMLPGO    BASED ON LOD  KZXMLPGO
   zVIEW     KZXMLPGO = 0; 
   //:STRING ( 513 ) szMsg
   zCHAR     szMsg[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szXLPFileName
   zCHAR     szXLPFileName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szStr
   zCHAR     szStr[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szFileName
   zCHAR     szFileName[ 514 ] = { 0 }; 
   //:STRING ( 1 )   szSlash
   zCHAR     szSlash[ 2 ] = { 0 }; 
   //:STRING ( 30 )  szOperatorArea
   zCHAR     szOperatorArea[ 31 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:SHORT nLth
   zSHORT    nLth = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 

   RESULT = GetViewByName( &TZCMWKSO, "TZCMWKSO", ViewToWindow, zLEVEL_TASK );

   //:szFileName = "*.*"
   ZeidonStringCopy( szFileName, 1, 0, "*.*", 1, 0, 514 );
   //:IF ( OperatorPromptForFile( ViewToWindow,
   //:                             szXLPFileName,
   //:                             500,
   //:                             "LLP files (*.LLP)||*.LLP||||",
   //:                             "CSV",
   //:                             9224 ) = 2 )
   lTempInteger_0 = OperatorPromptForFile( ViewToWindow, szXLPFileName, 500, "LLP files (*.LLP)|*.LLP||", "CSV", 9224 );
   if ( lTempInteger_0 == 2 )
   { 
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END


   //:nRC = ActivateOI_FromFile ( TZCMLPLO, "TZCMLPLO", ViewToWindow, szXLPFileName, 512 )
   nRC = ActivateOI_FromFile( &TZCMLPLO, "TZCMLPLO", ViewToWindow, szXLPFileName, 512 );

   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:szMsg = "Could not open the LLP file specified: " + NEW_LINE + "  " + szXLPFileName + "."
      ZeidonStringCopy( szMsg, 1, 0, "Could not open the LLP file specified: ", 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, "  ", 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, szXLPFileName, 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, ".", 1, 0, 514 );
      //:MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:DropView( TZCMLPLO )
      DropView( TZCMLPLO );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:NAME VIEW TZCMLPLO "TZCMLPLO"
   SetNameForView( TZCMLPLO, "TZCMLPLO", 0, zLEVEL_TASK );

   //:nLth = zstrlen( szXLPFileName )
   nLth = zstrlen( szXLPFileName );
   //:// We want to get just the directory selected without the filename.
   //:nRC = zSearchSubString( szXLPFileName, "\"  , "b", nLth )
   nRC = zSearchSubString( szXLPFileName, "\\", "b", nLth );
   //://nRC = zSearchSubString( szXLPFileName, "\"  , "b", nRC - 1 )

   //:// Keep track of original MetSrcDir
   //:szStr = TZCMLPLO.LPLR.MetaSrcDir
   GetVariableFromAttribute( szStr, 0, 'S', 514, TZCMLPLO, "LPLR", "MetaSrcDir", "", 0 );

   //:nRC = ZeidonStringCopy( szFileName, 1, 0, szXLPFileName, 1, nRC, 513 )
   nRC = ZeidonStringCopy( szFileName, 1, 0, szXLPFileName, 1, (zLONG) nRC, 513 );

   //:// KJS 06/05/15 - Probably not here, but if the user puts in a directory for the meta source that doesn't
   //:// exist, do we want to create one for them?
   //:TZCMLPLO.LPLR.MetaSrcDir = szFileName
   SetAttributeFromString( TZCMLPLO, "LPLR", "MetaSrcDir", szFileName );
   //:TZCMLPLO.LPLR.wOrigMetaSrc = szFileName
   SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigMetaSrc", szFileName );
   //://TZCMLPLO.LPLR.wOrigExecSrc = TZCMLPLO.LPLR.ExecDir
   //:TZCMLPLO.LPLR.wOrigLPLRName = TZCMLPLO.LPLR.Name 
   SetAttributeFromAttribute( TZCMLPLO, "LPLR", "wOrigLPLRName", TZCMLPLO, "LPLR", "Name" );
   //:TZCMLPLO.LPLR.wOrigJavaPackage = TZCMLPLO.LPLR.JavaPackageName
   SetAttributeFromAttribute( TZCMLPLO, "LPLR", "wOrigJavaPackage", TZCMLPLO, "LPLR", "JavaPackageName" );

   //:// KJS 06/16/15 - Should we see if there is an ExecutableSubDirecory, and if not, create one based on the ExecDir.
   //:IF TZCMLPLO.LPLR.ExecDir != ""
   if ( CompareAttributeToString( TZCMLPLO, "LPLR", "ExecDir", "" ) != 0 )
   { 
      //:szFileName = TZCMLPLO.LPLR.ExecDir
      GetVariableFromAttribute( szFileName, 0, 'S', 514, TZCMLPLO, "LPLR", "ExecDir", "", 0 );
      //:// Get the part of the filename that is different between the Meta and Exec dirs.
      //:zSearchAndReplace( szFileName, 513, szStr, "" )
      zSearchAndReplace( szFileName, 513, szStr, "" );
      //:nRC = zSearchSubString( szFileName, "\"  , "f", 0 )
      nRC = zSearchSubString( szFileName, "\\", "f", 0 );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:// Now replace the starting "\" if there is one.
         //:nRC = ZeidonStringCopy( szFileName, 1, 0, szFileName, 2, 0, 513 )
         nRC = ZeidonStringCopy( szFileName, 1, 0, szFileName, 2, 0, 513 );
      } 

      //:END
      //:// Do we need to get rid of the first /?
      //:TZCMLPLO.LPLR.wExecutableSubDirectory = szFileName
      SetAttributeFromString( TZCMLPLO, "LPLR", "wExecutableSubDirectory", szFileName );
   } 

   //:END

   //:TZCMLPLO.LPLR.wOrigExecSrc = TZCMLPLO.LPLR.wOrigMetaSrc + "\" + TZCMLPLO.LPLR.wExecutableSubDirectory
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZCMLPLO, "LPLR", "wOrigMetaSrc" );
   ZeidonStringConcat( szTempString_0, 1, 0, "\\", 1, 0, 255 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TZCMLPLO, "LPLR", "wExecutableSubDirectory", "", 0 );
   ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 255 );
   SetAttributeFromString( TZCMLPLO, "LPLR", "wOrigExecSrc", szTempString_0 );
   //:TZCMLPLO.LPLR.wCopiedExecDir = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wCopiedExecDir", "" );
   //:TZCMLPLO.LPLR.wCopiedJavaDir = ""
   SetAttributeFromString( TZCMLPLO, "LPLR", "wCopiedJavaDir", "" );
   return( 0 );
//   //zstrncpy( szFileName, szXLPFileName, nRC, nLth )
// END
} 


//:LOCAL OPERATION
//:GetZeidonApp( VIEW ViewToWindow,
//:              VIEW vKZAPPLOO,
//:              STRING ( 32 ) szZeidonDirectory )
//:   STRING ( 513 ) szMsg
static zSHORT
o_GetZeidonApp( zVIEW     ViewToWindow,
                zPVIEW    vKZAPPLOO,
                zPCHAR    szZeidonDirectory )
{
   zCHAR     szMsg[ 514 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nLth
   zLONG     nLth = 0; 


   //:SysGetEnvVar( szZeidonDirectory, "ZEIDON",  513 )
   SysGetEnvVar( szZeidonDirectory, "ZEIDON", 513 );
   //:nLth = zstrlen( szZeidonDirectory )
   nLth = zstrlen( szZeidonDirectory );
   //:nRC = zSearchSubString( szZeidonDirectory, "\"  , "f", nLth )
   nRC = zSearchSubString( szZeidonDirectory, "\\", "f", nLth );

   //:szZeidonDirectory = szZeidonDirectory + "ZEIDON.APP"
   ZeidonStringConcat( szZeidonDirectory, 1, 0, "ZEIDON.APP", 1, 0, 33 );
   //:nRC = ActivateOI_FromFile ( vKZAPPLOO, "KZAPPLOO", ViewToWindow, szZeidonDirectory, 512)
   nRC = ActivateOI_FromFile( vKZAPPLOO, "KZAPPLOO", ViewToWindow, szZeidonDirectory, 512 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:szMsg = "Could not open: " + szZeidonDirectory + "."
      ZeidonStringCopy( szMsg, 1, 0, "Could not open: ", 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, szZeidonDirectory, 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, ".", 1, 0, 514 );
      //:MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:DropView( vKZAPPLOO )
      DropView( *vKZAPPLOO );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:INITIALIZE_NextZKeyForObject( VIEW vMetaOI,
//:                              STRING ( 32 )  szRootEntityName )

//:   VIEW vHierMetaOI
static zSHORT
o_INITIALIZE_NextZKeyForObject( zVIEW     vMetaOI,
                                zPCHAR    szRootEntityName )
{
   zVIEW     vHierMetaOI = 0; 
   //:STRING ( 32 )  szCurrentEntityName
   zCHAR     szCurrentEntityName[ 33 ] = { 0 }; 
   //:STRING ( 500 ) szMsg
   zCHAR     szMsg[ 501 ] = { 0 }; 
   //:STRING ( 50 )  szDateTimeStamp
   zCHAR     szDateTimeStamp[ 51 ] = { 0 }; 
   //:STRING ( 20 )  szGeneratedKey
   zCHAR     szGeneratedKey[ 21 ] = { 0 }; 
   //:INTEGER        lZKey
   zLONG     lZKey = 0; 
   //:INTEGER        lMaxZKey
   zLONG     lMaxZKey = 0; 
   //:INTEGER        lGeneratedKey
   zLONG     lGeneratedKey = 0; 
   //:SHORT          nHierRC
   zSHORT    nHierRC = 0; 
   //:SHORT          nReturnLevel
   zSHORT    nReturnLevel = 0; 


   //:SysGetDateTime( szDateTimeStamp )
   SysGetDateTime( szDateTimeStamp, zsizeof( szDateTimeStamp ) );
   //:TraceLineS( "*** szDateTimeStamp: ", szDateTimeStamp )
   TraceLineS( "*** szDateTimeStamp: ", szDateTimeStamp );

   //:// Initialize the NextZKeyToAssign value in the object, which is done in two different ways.
   //:// 1. If the current entity being created in CreateMetaEntity is the root entity, then we will use the
   //://    date/time stamp as a random number generator.
   //:// 2. Otherwise, we will set NextZKeyToAssign to one more than the highest ZKey in the object.

   //:IF szRootEntityName != szCurrentEntityName
   if ( ZeidonStringCompare( szRootEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 )
   { 

      //:// Initialize the Next ZKey to Assign value for the meta object to one more than the highest ZKey
      //:// Value in the object.
      //:// Process each entity hierarchically, looking for highest ZKey value.
      //:CreateViewFromView( vHierMetaOI, vMetaOI )
      CreateViewFromView( &vHierMetaOI, vMetaOI );
      //:DefineHierarchicalCursor( vHierMetaOI, szRootEntityName )
      DefineHierarchicalCursor( vHierMetaOI, szRootEntityName );
      //:szCurrentEntityName = szRootEntityName
      ZeidonStringCopy( szCurrentEntityName, 1, 0, szRootEntityName, 1, 0, 33 );

      //:GetIntegerFromAttribute( lMaxZKey, vHierMetaOI, szCurrentEntityName, "ZKey" )
      GetIntegerFromAttribute( &lMaxZKey, vHierMetaOI, szCurrentEntityName, "ZKey" );
      //:nHierRC = zCURSOR_SET
      nHierRC = zCURSOR_SET;
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

            //:// Check ZKey in current entity against current Max ZKey.
            //:GetIntegerFromAttribute( lZKey, vHierMetaOI, szCurrentEntityName, "ZKey" )
            GetIntegerFromAttribute( &lZKey, vHierMetaOI, szCurrentEntityName, "ZKey" );
            //:IF lZKey > lMaxZKey
            if ( lZKey > lMaxZKey )
            { 
               //:lMaxZKey =lZKey
               lMaxZKey = lZKey;
            } 

            //:END

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
      //:lMaxZKey = lMaxZKey + 1
      lMaxZKey = lMaxZKey + 1;

      //:DropView( vHierMetaOI )
      DropView( vHierMetaOI );
      //:ELSE
   } 
   else
   { 
      //:// This is the Root of the object, so we will use the date/time stamp as a random number generator
      //:// for generating the ZKey of the root entity and set the NextZKeyToAssign to 1000.
      //:// DateTimeStamp is of form YYYYMMDDHHMMSSTTT.
      //:// We will use 9 middle digits, DDHHMMSST, forming DDH,HMM,SST, which would generate a key up to 312,459,599.
      //:SysGetDateTime( szDateTimeStamp )
      SysGetDateTime( szDateTimeStamp, zsizeof( szDateTimeStamp ) );
      //:szGeneratedKey = szDateTimeStamp[6:9]
      ZeidonStringCopy( szGeneratedKey, 1, 0, szDateTimeStamp, 6, 9, 21 );
      //:lGeneratedKey = zStringToInteger( szGeneratedKey )
      lGeneratedKey = zStringToInteger( szGeneratedKey );
      //:SetAttributeFromInteger( vMetaOI, szRootEntityName, "ZKey", lGeneratedKey )
      SetAttributeFromInteger( vMetaOI, szRootEntityName, "ZKey", lGeneratedKey );
      //:lMaxZKey = 1000
      lMaxZKey = 1000;
   } 

   //:   END

   //:// Set the NextZKeyToAssign attribute
   //:SetAttributeFromInteger( vMetaOI, szRootEntityName, "NextZKeyToAssign", lMaxZKey )
   SetAttributeFromInteger( vMetaOI, szRootEntityName, "NextZKeyToAssign", lMaxZKey );
   //:szMsg = "*** Root Attribute " + szRootEntityName + ": "
   ZeidonStringCopy( szMsg, 1, 0, "*** Root Attribute ", 1, 0, 501 );
   ZeidonStringConcat( szMsg, 1, 0, szRootEntityName, 1, 0, 501 );
   ZeidonStringConcat( szMsg, 1, 0, ": ", 1, 0, 501 );
   //:TraceLineI( szMsg, lMaxZKey )
   TraceLineI( szMsg, lMaxZKey );
   return( 0 );
//    END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
CHANGE_ImportName( zVIEW     ViewToWindow )
{

   //:CHANGE_ImportName( VIEW ViewToWindow )

   //:SetCtrlState( ViewToWindow, "edName", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( ViewToWindow, "edName", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( ViewToWindow, "edDesc", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( ViewToWindow, "edDesc", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( ViewToWindow, "edBaseDir", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( ViewToWindow, "edBaseDir", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( ViewToWindow, "edExecDir", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( ViewToWindow, "edExecDir", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( ViewToWindow, "edJavaPackageName", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( ViewToWindow, "edJavaPackageName", zCONTROL_STATUS_ENABLED, TRUE );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:CopyContentsOfDirectory( VIEW ViewToWindow, 
//:                         STRING ( 1000 ) szFromDir,
//:                         STRING ( 1000 ) szToDir, 
//:                         STRING( 1 )     szCopyFiles )
//:                         
//:   VIEW TZCMLPLO    REGISTERED AS TZCMLPLO
static zSHORT
o_CopyContentsOfDirectory( zVIEW     ViewToWindow,
                           zPCHAR    szFromDir,
                           zPCHAR    szToDir,
                           zPCHAR    szCopyFiles )
{
   zVIEW     TZCMLPLO = 0; 
   zSHORT    RESULT; 
   //:STRING ( 1000 ) szFromDirSub                          
   zCHAR     szFromDirSub[ 1001 ] = { 0 }; 
   //:STRING ( 1000 ) szToDirSub                          
   zCHAR     szToDirSub[ 1001 ] = { 0 }; 
   //:STRING ( 1000 ) szFromFileName                          
   zCHAR     szFromFileName[ 1001 ] = { 0 }; 
   //:STRING ( 1000 ) szToFileName                          
   zCHAR     szToFileName[ 1001 ] = { 0 }; 
   //:STRING ( 100 ) szTmp 
   zCHAR     szTmp[ 101 ] = { 0 }; 
   //:STRING ( 1 ) szAlreadyCopied                         
   zCHAR     szAlreadyCopied[ 2 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:INTEGER lHandle
   zLONG     lHandle = 0; 

   RESULT = GetViewByName( &TZCMLPLO, "TZCMLPLO", ViewToWindow, zLEVEL_TASK );

   //:szAlreadyCopied = ""
   ZeidonStringCopy( szAlreadyCopied, 1, 0, "", 1, 0, 2 );

   //:szFromDirSub = TZCMLPLO.LPLR.wOrigExecSrc + "\"
   GetStringFromAttribute( szFromDirSub, zsizeof( szFromDirSub ), TZCMLPLO, "LPLR", "wOrigExecSrc" );
   ZeidonStringConcat( szFromDirSub, 1, 0, "\\", 1, 0, 1001 );
   //:IF szFromDir = szFromDirSub AND TZCMLPLO.LPLR.wCopiedExecDir = "Y"
   if ( ZeidonStringCompare( szFromDir, 1, 0, szFromDirSub, 1, 0, 1001 ) == 0 && CompareAttributeToString( TZCMLPLO, "LPLR", "wCopiedExecDir", "Y" ) == 0 )
   { 
      //:szAlreadyCopied = "Y"
      ZeidonStringCopy( szAlreadyCopied, 1, 0, "Y", 1, 0, 2 );
      //:TraceLineS("executabledirectory", "yyy")
      TraceLineS( "executabledirectory", "yyy" );
   } 

   //:END
   //:szFromDirSub = TZCMLPLO.LPLR.wOrigJavaPackage
   GetVariableFromAttribute( szFromDirSub, 0, 'S', 1001, TZCMLPLO, "LPLR", "wOrigJavaPackage", "", 0 );
   //:IF szFromDir = szFromDirSub AND TZCMLPLO.LPLR.wCopiedJavaDir = "Y"
   if ( ZeidonStringCompare( szFromDir, 1, 0, szFromDirSub, 1, 0, 1001 ) == 0 && CompareAttributeToString( TZCMLPLO, "LPLR", "wCopiedJavaDir", "Y" ) == 0 )
   { 
      //:szAlreadyCopied = "Y"
      ZeidonStringCopy( szAlreadyCopied, 1, 0, "Y", 1, 0, 2 );
      //:TraceLineS("javadirectoryname", "yyy")
      TraceLineS( "javadirectoryname", "yyy" );
   } 

   //:END

   //:IF szAlreadyCopied = "" 
   if ( ZeidonStringCompare( szAlreadyCopied, 1, 0, "", 1, 0, 2 ) == 0 )
   { 

      //:SysValidDirOrFile( szToDir, TRUE, TRUE, 512)
      SysValidDirOrFile( szToDir, TRUE, TRUE, 512 );

      //:// Recursive function to copy directories and files from one lplr to another.
      //:szFromFileName = szFromDir + "*.*"
      ZeidonStringCopy( szFromFileName, 1, 0, szFromDir, 1, 0, 1001 );
      ZeidonStringConcat( szFromFileName, 1, 0, "*.*", 1, 0, 1001 );
      //:lHandle = FindFileStart( szFromFileName, szTmp )
      lHandle = FindFileStart( szFromFileName, szTmp );
      //:nRC = 1
      nRC = 1;
      //:LOOP WHILE lHandle >= 0 AND nRC > 0
      while ( lHandle >= 0 && nRC > 0 )
      { 

         //:// Don't copy the file if it begins with a period (like .git, .project etc)
         //:nRC = zSearchSubString( szTmp, ".", "f", 0 )
         nRC = zSearchSubString( szTmp, ".", "f", 0 );
         //:// If there is no period in the name, then we will assume this is a directory.
         //:IF nRC < 0 
         if ( nRC < 0 )
         { 
            //:// We want to create the directory, and copy files from the directory
            //:szFromDirSub = szFromDir + szTmp + "\"
            ZeidonStringCopy( szFromDirSub, 1, 0, szFromDir, 1, 0, 1001 );
            ZeidonStringConcat( szFromDirSub, 1, 0, szTmp, 1, 0, 1001 );
            ZeidonStringConcat( szFromDirSub, 1, 0, "\\", 1, 0, 1001 );
            //:szToDirSub = szToDir + szTmp + "\"
            ZeidonStringCopy( szToDirSub, 1, 0, szToDir, 1, 0, 1001 );
            ZeidonStringConcat( szToDirSub, 1, 0, szTmp, 1, 0, 1001 );
            ZeidonStringConcat( szToDirSub, 1, 0, "\\", 1, 0, 1001 );
            //://SysValidDirOrFile( szToDirSub, TRUE, TRUE, 512)
            //:CopyContentsOfDirectory( ViewToWindow, szFromDirSub, szToDirSub, "Y" )
            o_CopyContentsOfDirectory( ViewToWindow, szFromDirSub, szToDirSub, "Y" );
            //:ELSE
         } 
         else
         { 
            //:IF nRC != 0 AND szCopyFiles = "Y"
            if ( nRC != 0 && ZeidonStringCompare( szCopyFiles, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:szToFileName = szToDir + "\" + szTmp
               ZeidonStringCopy( szToFileName, 1, 0, szToDir, 1, 0, 1001 );
               ZeidonStringConcat( szToFileName, 1, 0, "\\", 1, 0, 1001 );
               ZeidonStringConcat( szToFileName, 1, 0, szTmp, 1, 0, 1001 );
               //:szFromFileName = szFromDir + "\" + szTmp
               ZeidonStringCopy( szFromFileName, 1, 0, szFromDir, 1, 0, 1001 );
               ZeidonStringConcat( szFromFileName, 1, 0, "\\", 1, 0, 1001 );
               ZeidonStringConcat( szFromFileName, 1, 0, szTmp, 1, 0, 1001 );
               //:nRC = SysCopyFile( ViewToWindow, szFromFileName, szToFileName, TRUE )
               nRC = SysCopyFile( ViewToWindow, szFromFileName, szToFileName, TRUE );
            } 

            //:END
         } 

         //:END
         //:nRC = FindFileNext( lHandle, szTmp )
         nRC = FindFileNext( lHandle, szTmp );
      } 

      //:END 
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
