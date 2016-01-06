/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:  TZERSAPD  -  Data Model SAP Import
// DESCRIPTION:  This module contains the code to import SAP tables
//               and relationships into the Zeidon data model.
//               The TE is automatically generated, special logic to
//               switch foreign keys is contained.
// OPERATIONS:   all Operations for SAP import
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 QuinSoft Corporation and TONBELLER AG.       *
// All rights reserved.                                            *
// Confidential and Proprietary material subject to license -      *
// do not reproduce or disclose. This material is an unpublished   *
// work and is considered a trade secret belonging to the          *
// copyright holder.                                               *
/////////////////////////////////////////////////////////////////////////////
/*
AUTHOR:  Bianca Ludwig
DATE:    2000.10.13
CHANGE LOG
2001.07.06   BL
   Set new Control Position
2001.02.16   HH
   Move code for switching foreign keys to TZTENVRO, so that it can
   be reused by TE tool.
2000.11.19   BL
   Postbuild Import Window: Set Default Values for Import, set new error
   messages, set new View Names
2000.11.17   HH
   Improve handling if foreign key switch candidate was not found.
2000.11.13   BL
   Modified Message Text and if close SAP Import Window, do not
   delete SAP View
2000.11.10   HH
   use "lLeng" on attribute import.
   improved FK switching, function ReferenceBaseKey.
2000.11.06   BL  Z10
   new Prototypes for SAP Logoff and GetRelationship
2000.11.01   BL  Z10
   Modified SAP Import for Subject Areas and TE generation ....
2000.10.13   BL  Z10
   Create this File for import SAP-Tables to Zeidon Data Models
*/


// LAST MESSAGE ID:  SAP00005

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define  KZOESYS_INCL
#define  zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

static  LPLIBRARY  hLibrary     = 0;
static  zLONG      lIgnoreError = 0;


typedef zPCHAR ( POPERATION pfnGetLastError ) ( void );
typedef zLONG  ( POPERATION pfnSapLogon ) ( zPCHAR, zPCHAR, zPCHAR, zPCHAR );
typedef zLONG  ( POPERATION pfnSapLogoff ) ( void );
typedef zLONG  ( POPERATION pfnSapDropList ) ( void* );
typedef zLONG  ( POPERATION pfnSapGetServerList ) ( void** );
typedef zLONG  ( POPERATION pfnSapGetServer ) ( void*, long, char* );
typedef zLONG  ( POPERATION pfnSapGetTableList ) ( const char*, char, void** );
typedef zLONG  ( POPERATION pfnSapGetTable ) ( void*, long, char*, char*, char* );
typedef zLONG  ( POPERATION pfnSapGetRelList ) ( const char*, char, void** );
typedef zLONG  ( POPERATION pfnSapGetRel ) ( void*, long, char*, char*, char*,
                                          char*, char*, char*, char* );
typedef zLONG (POPERATION pfnSapGetAttrList) ( const char*, char, void** );
typedef zLONG (POPERATION pfnSapGetAttr) ( void*, long,  char*, long*, char*, char*,
                                           char*, char*, long*, char*, char*, char*,
                                           char*, char*, long*, long*, char*, char* );

#define zIgnoreError            1

#define zBuildERD               0
#define zBuildTE                1

#define zAbortImport           -200
#define zAbortBuild            -201
#define zAbortImportBuild      -202

#define zBuildRelatedEntity     0
#define zBuildRelationship      1

#define zAttributeID            0
#define zRelLinkID              1

#define zImportSelectedTables   0
#define zImportAllTables        1

#define zBuild_Tables           "T"
#define zBuild_RS_TE            "B"
#define zBuild_Tables_RS_TE     "A"
#define zBuild_TE_only          "X"

#define SAP_CHAR    'C'  // char
#define SAP_DATE    'D'  // Date JJJJMMDD FU8
#define SAP_FLOAT   'F'  // Floatingpoint
#define SAP_INT     'I'  // Integer 4
#define SAP_NUM     'N'  // numeric FU
#define SAP_PACKED  'P'  // packed
#define SAP_TIME    'T'  // Time hhmmss FU6
#define SAP_HEX     'X'  // AlphaNumeric
#define SAP_BIN     'b'  // binaer 1 bype
#define SAP_SHORT   's'  // short


zVOID
zwfnTZEREMDD_FindRelatedVisible( zVIEW vSubtask,
                                 zVIEW vTZEREMDO1,
                                 zVIEW vTZEREMDO2 );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_AddRelLinkToSA( zVIEW  vSubtask,
                           zVIEW  vTZEREMDO,
                           zVIEW  vTZERSASO );

static zBOOL
fnTZERSAPD_IsChangeLogonParam( zVIEW  vTZERSAPO );

static zSHORT
fnTZERSAPD_CheckValues( zVIEW  vSubtask,
                        zVIEW  vTZERSAPO,
                        zPCHAR szEntityName,
                        zPCHAR szAttributeName,
                        zPCHAR szControlName,
                        zSHORT szTabNumber );

static zSHORT
fnTZERSAPD_SAP_Connect( zVIEW  vSubtask,
                        zVIEW  vTZERSAPO,
                        zPCHAR szConnection );

static zSHORT
fnTZERSAPD_SAP_LoadErrorMsg( zVIEW  lpTaskView,
                             zPCHAR szOperation );

static zSHORT
fnTZERSAPD_SAP_ReadErrorMsg( zVIEW   lpTaskView,
                             zCPCHAR cpcMessage );

static zSHORT
fnTZERSAPD_SAP_LoadTableData( zVIEW   vSubtask,
                              zVIEW   vTZERSAPO,
                              zPCHAR  szSearchString,
                              zCHAR   cLanguage );

static zSHORT
fnTZERSAPD_CheckNewLogon( zVIEW  vSubtask,
                          zVIEW  vTZERSAPO );

static zSHORT
fnTZERSAPD_SAP_LoadAttributeData( zVIEW   vSubtask,
                                  zVIEW   vERD,
                                  zPCHAR  szTableName,
                                  zCHAR   cLanguage );

static zSHORT
fnTZERSAPD_SAP_AttrSetDilMsg( zVIEW   vSubtask,
                              zPCHAR  szTableName,
                              zPCHAR  szAttr,
                              zPCHAR  szCountAttr,
                              zLONG   lIndex );

static zSHORT
fnTZERSAPD_SAP_RelSetDilMsg( zVIEW   vSubtask,
                             zPCHAR  szTableName,
                             zPCHAR  szRel,
                             zPCHAR  szCountRel,
                             zLONG   lIndex );

static zSHORT
fnTZERSAPD_CreateAttribAndIdent( zVIEW          vSubtask,
                                 zVIEW          vERD,
                                 zPCHAR         szTableName,
                                 zPCHAR         szCountAttr,
                                 pfnSapGetAttr  pfnGetAttr,
                                 void*          hAttribList,
                                 zLONG          lIndex,
                                 zPSHORT        pnExistFK );

static zSHORT
fnTZERSAPD_SendMessageBox( zVIEW   vSubtask,
                           zPCHAR  szBuildFlag,
                           zPCHAR  szSubMsg,
                           zSHORT  nIcon );

static zSHORT
fnTZERSAPD_SAP_DropList( zVIEW lpTaskView,
                         void  *hList );

static zSHORT
fnTZERSAPD_CreateRelationship( zVIEW          vSubtask,
                               zVIEW          vERD,
                               zVIEW          vERD_Copy,
                               zPCHAR         szTableName,
                               zPCHAR         szCountRel,
                               pfnSapGetRel   pfnGetRel,
                               void*          hRelList,
                               zLONG          lIndex );

static zSHORT
fnTZERSAPD_CreateFirstRelLink( zVIEW   vSubtask,
                               zVIEW   vERD,
                               zVIEW   vERD_Copy,
                               zPCHAR  szFK_FieldName,
                               zPCHAR  szCard );

static zSHORT
fnTZERSAPD_CreateSecondRelLink( zVIEW   vSubtask,
                                zVIEW   vERD,
                                zPCHAR  szFK_FieldName,
                                zPCHAR  szCardLeft );

static zSHORT
fnTZERSAPD_CreateRelatedEntity( zVIEW          vSubtask,
                                zVIEW          vTZERSAPO,
                                zPCHAR         szTableName,
                                zPCHAR         szCountRel,
                                pfnSapGetRel   pfnGetRel,
                                void*          hRelList,
                                zLONG          lIndex );

static zSHORT
fnTZERSAPD_RemoveTablesFromList( zVIEW  vTZERSAPO );

static zBOOL
fnTZERSAPD_FK_HasReferToER_Attr( zVIEW  vDTE,
                                 zVIEW  vTZSapRel,
                                 zLONG  lCurrentZKey );

static zSHORT
fnTZERSAPD_CheckExistsOfName( zVIEW  vDTE,
                              zVIEW  vDTE_FieldDataRel,
                              zVIEW  vTZSapRel,
                              zPCHAR szFieldName );

static zBOOL
fnTZERSAPD_FK_HasBlankRS( zVIEW  vDTE,
                          zVIEW  vTZSapRel,
                          zPCHAR szCheckTable );

static zSHORT
fnTZERSAPD_FK_SearchBaseKey( zVIEW  vDTE_BaseKey,
                             zLONG  lZKey );

static zSHORT
fnTZERSAPD_EntityForKeyRS( zVIEW  vSubtask,
                           zPVIEW pvERD,
                           zPCHAR szFK_TableName,
                           zPCHAR szTableName,
                           zPCHAR szFK_FieldName );

static zLONG
fnTZERSAPD_CreateERD_Rel( zVIEW   vSubtask,
                          zVIEW   vTZSapRel,
                          zVIEW   vERD,
                          zVIEW   vERD_Copy,
                          zPCHAR  szTableName,
                          zPCHAR  szFK_FieldName,
                          zPCHAR  szFK_Type,
                          zPCHAR  szCard,
                          zPCHAR  szCardLeft,
                          zCHAR   cKey );

static zSHORT
fnTZERSAPD_CreateNewLogonEntity( zVIEW  vTZERSAPO );

static zSHORT
fnTZERSAPD_SwitchForeignToERAttr( zVIEW  vDTE,
                                  zVIEW  vDTE_SwitchTo );
static void
SetBaseKeyAs_PK( zVIEW vDTE_ERA, zVIEW vDTE_FK );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateNewErrorRoot( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_WriteErrorMsg( zVIEW  vSubtask,
                          zSHORT nBuildERDorTE,
                          zPCHAR szErrorMsg,
                          zSHORT nError );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetCountError( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SetEntitySelect( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_Build_SAP_RS_TE( zVIEW  vSubtask,
                            zVIEW  vERD );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_LoadTablesFromWorkLod( zVIEW  vSubtask,
                                  zVIEW  vTZERSAPO );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_LoadErrorList( zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetTabSizeForSearch
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SetTabSizeForSearch( zVIEW  vSubtask,
                                zLONG  lTabCtrlSize_X )
{
   zLONG  lCtrlSize_X;
   zLONG  lCtrlSize_Y;
   zLONG  lCtrlPos_X;
   zLONG  lCtrlPos_Y;
   zLONG  lTemp;

   GetCtrlPosition( vSubtask, "cbServer", &lCtrlPos_X, &lTemp, TRUE );
   GetCtrlSize( vSubtask, "cbServer", &lCtrlSize_X, &lCtrlSize_Y, TRUE );
   lCtrlSize_X = lTabCtrlSize_X - lCtrlPos_X;
   SetCtrlSize( vSubtask, "cbServer", lCtrlSize_X, 60, TRUE );

   GetCtrlSize( vSubtask, "cbSearchString", &lTemp, &lCtrlSize_Y, TRUE );
   SetCtrlSize( vSubtask, "cbSearchString", lCtrlSize_X, 60, TRUE );

   GetCtrlSize( vSubtask, "pbSearch", &lCtrlSize_X, &lCtrlSize_Y, TRUE );
   GetCtrlPosition( vSubtask, "pbSearch", &lCtrlPos_X, &lTemp, TRUE );
   lCtrlPos_X = lTabCtrlSize_X - lCtrlSize_X - 5;
   GetCtrlSize( vSubtask, "tcSearch", &lTemp, &lCtrlPos_Y, TRUE );
   lCtrlPos_Y = lCtrlPos_Y - lCtrlSize_Y - 9;
   SetCtrlPosition( vSubtask, "pbSearch", lCtrlPos_X, lCtrlPos_Y, TRUE );

   return( 0 );
} // zwTZERSAPD_SetTabSizeForSearch


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetTabSizeForLogon
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SetTabSizeForLogon( zVIEW  vSubtask,
                               zLONG  lTabCtrlSize_X )
{
   zLONG  lCtrlSize_X;
   zLONG  lCtrlSize_Y;
   zLONG  lCtrlPos_X;
   zLONG  lTemp;

   GetCtrlPosition( vSubtask, "edClient", &lCtrlPos_X, &lTemp, TRUE );
   GetCtrlSize( vSubtask, "edClient", &lCtrlSize_X, &lCtrlSize_Y, TRUE );
   lCtrlSize_X = lTabCtrlSize_X - lCtrlPos_X;
   SetCtrlSize( vSubtask, "edClient", lCtrlSize_X, lCtrlSize_Y, TRUE );

   GetCtrlSize( vSubtask, "edPassword", &lTemp, &lCtrlSize_Y, TRUE );
   SetCtrlSize( vSubtask, "edPassword", lCtrlSize_X, lCtrlSize_Y, TRUE );

   GetCtrlSize( vSubtask, "edUser", &lTemp, &lCtrlSize_Y, TRUE );
   SetCtrlSize( vSubtask, "edUser", lCtrlSize_X, lCtrlSize_Y, TRUE );

   return( 0 );
} // zwTZERSAPD_SetTabSizeForLogon


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetTabSize
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetTabSize( zVIEW vSubtask )
{
   zLONG  lWindowSize_X;
   zLONG  lCtrlSize_X;
   zLONG  lCtrlSize_Y;
   zLONG  lCtrlPos_X;
   zLONG  lCtrlPos_Y;
   zLONG  lTabCtrlSize_X;
   zLONG  lTemp;
   zCHAR  szControl[ 33 ];

   // Set new size for Tab Control
   GetCtrlSize( vSubtask, 0, &lWindowSize_X, &lTemp, TRUE );
   GetCtrlSize( vSubtask, "Tab", &lCtrlSize_X, &lCtrlSize_Y, TRUE );
   GetCtrlPosition( vSubtask, "Tab", &lCtrlPos_X, &lTemp, TRUE );

   lTabCtrlSize_X = lWindowSize_X - lCtrlPos_X - 70;
   if ( lTabCtrlSize_X < 200 )
      lTabCtrlSize_X = 200;

   SetCtrlSize( vSubtask, "Tab", lTabCtrlSize_X, lCtrlSize_Y, TRUE );

   // Set new size for Tab SubControl
   GetCtrlSize( vSubtask, "tcSearch", &lTemp, &lCtrlSize_Y, TRUE );
   lTabCtrlSize_X = lTabCtrlSize_X - 5;
   SetCtrlSize( vSubtask, "tcSearch", lTabCtrlSize_X, lCtrlSize_Y, TRUE );

   GetCtrlSize( vSubtask, "tcLogon", &lTemp, &lCtrlSize_Y, TRUE );
   SetCtrlSize( vSubtask, "tcLogon", lTabCtrlSize_X, lCtrlSize_Y, TRUE );

   GetCtrlSize( vSubtask, "tcOptions", &lTemp, &lCtrlSize_Y, TRUE );
   SetCtrlSize( vSubtask, "tcOptions", lTabCtrlSize_X, lCtrlSize_Y, TRUE );

   GetCtrlSize( vSubtask, "tcErrorOptions", &lTemp, &lCtrlSize_Y, TRUE );
   SetCtrlSize( vSubtask, "tcErrorOptions", lTabCtrlSize_X, lCtrlSize_Y, TRUE );

   // Save current Control with Focus
   GetCtrlWithFocus( vSubtask, szControl );

   // if 1. Tab active, set new size for Controls from 1. Tab SubContol
   if ( SetFocusToCtrl( vSubtask, "cbServer" ) >= 0 )
   {
      zwTZERSAPD_SetTabSizeForSearch( vSubtask, lTabCtrlSize_X );
   }

   // if 2. Tab active, set new size for Controls from 2. Tab SubContol
   if ( SetFocusToCtrl( vSubtask, "edClient" ) >= 0 )
   {
      zwTZERSAPD_SetTabSizeForLogon( vSubtask, lTabCtrlSize_X );
   }

   // Set new position for Buttons
   GetCtrlSize( vSubtask, "Tab", &lCtrlSize_X, &lTemp, TRUE );
   lCtrlPos_X = lCtrlSize_X + 10;
   GetCtrlPosition( vSubtask, "pbClose", &lTemp, &lCtrlPos_Y, TRUE );
   SetCtrlPosition( vSubtask, "pbClose", lCtrlPos_X, 16, TRUE );

   GetCtrlPosition( vSubtask, "pbImport", &lTemp, &lCtrlPos_Y, TRUE );
   SetCtrlPosition( vSubtask, "pbImport", lCtrlPos_X, 38, TRUE );

   GetCtrlPosition( vSubtask, "pbImportAll", &lTemp, &lCtrlPos_Y, TRUE );
   SetCtrlPosition( vSubtask, "pbImportAll", lCtrlPos_X, 54, TRUE );

   GetCtrlPosition( vSubtask, "pbErrorList", &lTemp, &lCtrlPos_Y, TRUE );
   SetCtrlPosition( vSubtask, "pbErrorList", lCtrlPos_X, 74, TRUE );

   SetFocusToCtrl( vSubtask, szControl );

   return( 0 );
} // zwTZERSAPD_SetTabSize


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ReadServerFromSirEnv
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_ReadServerFromSirEnv( zVIEW  vSubtask,
                                 zVIEW  vTZERSAPO )
{
   void    *hServerList = 0;
   zCHAR   szServerName[ 33 ];
   zLONG   lCountServer, k;

   pfnSapGetServerList  pfnGetServerList;
   pfnSapGetServer      pfnGetServer;

   // if KZSAPACC.dll does not load, load it
   if ( hLibrary == 0 )
   {
      hLibrary = SysLoadLibrary( vSubtask, "KZSAPACC" );
      if ( hLibrary == 0 )
      {
         SysMessageBox( vSubtask, "SAP Import", "Can't find library KZSAPACC.", 1 );
         return( zCALL_ERROR );
      }
   }

   // Read ServerList
   pfnGetServerList = (pfnSapGetServerList) SysGetProc( hLibrary, "ZSAP_GetSirenvServerList" );
   if ( pfnGetServerList == 0 )
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetSirenvServerList" ));

   lCountServer = (*pfnGetServerList) ( &hServerList );

   // Read Server from List
   pfnGetServer = (pfnSapGetServer) SysGetProc( hLibrary, "ZSAP_GetSirenvServerName" );
   if ( pfnGetServer == 0 )
   {
      fnTZERSAPD_SAP_DropList( vSubtask, hServerList );
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetSirenvServerName" ));
   }

   for ( k = 0; k < lCountServer; k++ )
   {
      if ( (*pfnGetServer) ( hServerList, k, szServerName ) < 0 )
      {
         fnTZERSAPD_SAP_DropList( vSubtask, hServerList );
         return( -1 );
      }

      CreateEntity( vTZERSAPO, "SirEnvServer", zPOS_AFTER );
      SetAttributeFromString( vTZERSAPO, "SirEnvServer", "Server", szServerName );
   }

   // if no Server found, create a empty item
   if ( lCountServer == 0 )
      CreateEntity( vTZERSAPO, "SirEnvServer", zPOS_AFTER );

   // Drop Server list
   fnTZERSAPD_SAP_DropList( vSubtask, hServerList );

   return( 0 );
} // zwTZERSAPD_ReadServerFromSirEnv


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_LoadWorkLod
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_LoadWorkLod( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zCHAR  szValue[ 20 ];
   zCHAR  szDecrypt[ 33 ];

   if ( GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK ) > 0 )
      return( 0 );

   // Create Work LOD
   ActivateEmptyObjectInstance( &vTZERSAPO, "TZERSAPO", vSubtask, zMULTIPLE );
   SetNameForView( vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   CreateEntity( vTZERSAPO, "SapEnvironment", zPOS_AFTER );
   CreateEntity( vTZERSAPO, "Logon", zPOS_AFTER );
   CreateEntity( vTZERSAPO, "SearchString", zPOS_AFTER );

   // read all SAP Servers from SirEnv.txt
   zwTZERSAPD_ReadServerFromSirEnv( vSubtask, vTZERSAPO );

   // set Client, User and Password from Zeidon.ini
   SysReadZeidonIni( -1, "[SAP]", "DefaultClient", szValue, sizeof( szValue ) );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "Client", szValue );

   SysReadZeidonIni( -1, "[SAP]", "DefaultUser", szValue, sizeof( szValue ) );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "User", szValue );

   SysReadZeidonIni( -1, "[SAP]", "DefaultPassword", szValue, sizeof( szValue ) );
   UfDecryptString( szDecrypt, szValue, 8 );
   SetAttributeFromVariable( vTZERSAPO, "SapEnvironment", "Password",
                             szDecrypt, zTYPE_STRING, 8, "Password", 0 );

   SetAttributeFromInteger( vTZERSAPO, "SapEnvironment", "Connection", 0 );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "LanguageType", "D" );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_Tables );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "OverwriteExistingEntity", "N" );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "ShowErrorList", "N" );
   SetAttributeFromInteger( vTZERSAPO, "SapEnvironment", "IgnoreError", zIgnoreError );
   SetAttributeFromInteger( vTZERSAPO, "SearchString", "CountTables", -1 );

   return( 0 );
} // zwTZERSAPD_LoadWorkLod

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SaveInZeidonIni
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SaveInZeidonIni( zVIEW vTZERSAPO )
{
   zCHAR  szValue[ 20 ];
   zCHAR  szEncrypt[ 33 ];

   // if checkBox "save in zeidon.ini" active, set new values in zeidon.ini
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "SaveInZeidonIni", "Y" ) == 0 )
   {
      GetStringFromAttribute( szValue, sizeof( szValue ), vTZERSAPO, "SapEnvironment", "Client" );
      SysUpdateZeidonIni( "[SAP]", "DefaultClient", szValue );

      GetStringFromAttribute( szValue, sizeof( szValue ), vTZERSAPO, "SapEnvironment", "User" );
      SysUpdateZeidonIni( "[SAP]", "DefaultUser", szValue );

      GetStringFromAttributeByContext( szValue, vTZERSAPO, "SapEnvironment", "Password", "Password", 8 );
      UfEncryptString( szEncrypt, szValue, 8 );
      SysUpdateZeidonIni( "[SAP]", "DefaultPassword", szEncrypt );
   }

   return( 0 );
} // zwTZERSAPD_SaveInZeidonIni

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CloseImport
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_CloseImport( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   // if checkBox "save in zeidon.ini" active, set new values in zeidon.ini
   zwTZERSAPD_SaveInZeidonIni( vTZERSAPO );

   return( 0 );
} // zwTZERSAPD_CloseImport

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ExitImport
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ExitImport( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;

   if ( GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   if ( hLibrary != 0 )
   {
      if ( CompareAttributeToInteger( vTZERSAPO, "SapEnvironment", "Connection", 1 ) == 0 )
      {
         // SAP Logoff
         fnTZERSAPD_SAP_Connect( vSubtask, vTZERSAPO, "ZSAP_Logoff" );
      }

      SysFreeLibrary( vSubtask, hLibrary );
      hLibrary = 0;
   }

   // if checkBox "save in zeidon.ini" active, set new values in zeidon.ini
   zwTZERSAPD_SaveInZeidonIni( vTZERSAPO );

   DropObjectInstance( vTZERSAPO );

   return( 0 );
} // zwTZERSAPD_ExitImport


static zSHORT
fnTZERSAPD_CheckValues( zVIEW  vSubtask,
                        zVIEW  vTZERSAPO,
                        zPCHAR szEntityName,
                        zPCHAR szAttributeName,
                        zPCHAR szControlName,
                        zSHORT szTabNumber )
{
   zCHAR  szWorkMessage[50];

   // check Server
   if ( CompareAttributeToString( vTZERSAPO, szEntityName,
                                  szAttributeName, "" ) == 0 )
   {
      if ( zstrcmp( szAttributeName, "SearchString" ) == 0 )
         strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "Search String for Table" );
      else
         strcpy_s( szWorkMessage, sizeof( szWorkMessage ), szAttributeName );

      if ( zstrcmp( szAttributeName, "Password" ) == 0 )
         strcat_s( szWorkMessage, sizeof( szWorkMessage ), " is required." );
      else
         strcat_s( szWorkMessage, sizeof( szWorkMessage ), " Name is required." );

      MessageSend( vSubtask, "SAP00001", "SAP Import",
                   szWorkMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      TAB_SetActiveTab( vSubtask, "Tab", szTabNumber );
      zwTZERSAPD_SetTabSize( vSubtask );
      SetFocusToCtrl( vSubtask, szControlName );

      return( -1 );
   }

   return( 0 );

} // fnTZERSAPD_CheckValues

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CheckValues
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CheckValues( zVIEW  vSubtask,
                        zVIEW  vTZERSAPO )
{
   zCHAR  szSearchString[ 257 ];

   if ( fnTZERSAPD_CheckValues( vSubtask, vTZERSAPO, "SirEnvServer", "Server", "cbServer", 1 ) < 0 )
   {
      return( -1 );
   }

   // if Build Relationship and TE only
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_RS_TE ) != 0 )
   {
      GetCtrlText( vSubtask, "cbSearchString", szSearchString, 257 );
      SetAttributeFromString( vTZERSAPO, "SapEnvironment", "SearchString", szSearchString );
      if ( fnTZERSAPD_CheckValues( vSubtask, vTZERSAPO, "SapEnvironment", "SearchString", "cbSearchString", 1 ) < 0 )
      {
         return( -1 );
      }
   }

   if ( fnTZERSAPD_CheckValues( vSubtask, vTZERSAPO, "SapEnvironment", "Client", "edClient", 2 ) < 0 )
   {
      return( -1 );
   }

   if ( fnTZERSAPD_CheckValues( vSubtask, vTZERSAPO, "SapEnvironment", "Password", "edPassword", 2 ) < 0 )
   {
      return( -1 );
   }

   if ( fnTZERSAPD_CheckValues( vSubtask, vTZERSAPO, "SapEnvironment", "User", "edUser", 2 ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
} // zwTZERSAPD_CheckValues


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetCountTable
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SetCountTable( zVIEW  vSubtask,
                          zVIEW  vTZERSAPO )
{
   zLONG     lTranspCount;
   zCHAR     szCountTables[ 10 ];
   zCHAR     szControlText[ 40 ];

   // Set Count Table to Textfield
   GetIntegerFromAttribute( &lTranspCount, vTZERSAPO, "SearchString", "CountTables" );

   if ( lTranspCount != -1 )
   {
      zltoa( lTranspCount, szCountTables, sizeof( szCountTables ) );
      strcpy_s( szControlText, sizeof( szControlText ), " " );
      strcat_s( szControlText, sizeof( szControlText ), szCountTables );
      strcat_s( szControlText, sizeof( szControlText ), " Table(s) found" );
   }
   else
   {
      strcpy_s( szControlText, sizeof( szControlText ), " Press Search Button" );
   }

   SetCtrlText( vSubtask, "txtCountTable", szControlText );

   return( 0 );
} // zwTZERSAPD_SetCountTable


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetSearchString
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetSearchString( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zCHAR  szSearchString[ 257 ];

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "cbSearchString", szSearchString, 257 );
   SetAttributeFromString( vTZERSAPO, "SearchString", "SearchString", szSearchString );

   return( 0 );
} // zwTZERSAPD_SetSearchString


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetEntityCursor
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetEntityCursor( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zCHAR  szSearchString[ 257 ];

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "cbSearchString", szSearchString, 257 );

   if ( zstrcmp( szSearchString, "" ) != 0 )
   {
      SetCursorFirstEntityByString( vTZERSAPO, "SearchString", "SearchString", szSearchString, "" );
   }
   else
   {
      SetCursorFirstEntityByInteger( vTZERSAPO, "SearchString", "CountTables", -1, "" );
   }

   zwTZERSAPD_SetCountTable( vSubtask, vTZERSAPO );
   RefreshCtrl( vSubtask, "lbTableList" );

   return( 0 );
} // zwTZERSAPD_SetEntityCursor



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateNewSearchItem
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateNewSearchItem( zVIEW vTZERSAPO )
{
   zVIEW   vTZERSAPO_Copy;

   // empty Search Item does not exists, crate it
   CreateViewFromViewForTask( &vTZERSAPO_Copy, vTZERSAPO, 0 );

   if ( SetCursorFirstEntityByInteger( vTZERSAPO_Copy, "SearchString", "CountTables", -1, "" ) < zCURSOR_SET )
   {
      CreateEntity( vTZERSAPO_Copy, "SearchString", zPOS_AFTER );
      SetAttributeFromInteger( vTZERSAPO_Copy, "SearchString", "CountTables", -1 );
   }

   DropView( vTZERSAPO_Copy );

   return( 0 );
} // zwTZERSAPD_CreateNewSearchItem


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateNewSearchString
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateNewSearchString( zVIEW  vSubtask,
                                  zVIEW  vTZERSAPO )
{
   zVIEW   vTZERSAPO_Copy;
   zCHAR   szSearchString[ 257 ];
   zCHAR   szOrgSearchString[ 257 ];

   CreateViewFromViewForTask( &vTZERSAPO_Copy, vTZERSAPO, 0 );

   // if new search string
   if ( CompareAttributeToInteger( vTZERSAPO, "SearchString", "CountTables", -1 ) == 0 )
   {
      SetAttributeFromAttribute( vTZERSAPO, "SearchString", "OriginalSearchString", vTZERSAPO, "SearchString", "SearchString" );
   }
   else
   {
      // search string is updated, create a new search string with this data and reset old search string data
      GetStringFromAttribute( szOrgSearchString, sizeof( szOrgSearchString ), vTZERSAPO, "SearchString", "OriginalSearchString" );
      GetStringFromAttribute( szSearchString, sizeof( szSearchString ), vTZERSAPO, "SearchString", "SearchString" );
      if ( zstrcmp( szSearchString, szOrgSearchString ) != 0 )
      {
         // reset old search string data
         SetAttributeFromString( vTZERSAPO, "SearchString", "SearchString", szOrgSearchString );

         // create a new search string with this data, when not exists
         if ( SetCursorFirstEntityByString( vTZERSAPO_Copy, "SearchString", "SearchString", szSearchString, "" ) < zCURSOR_SET &&
              SetCursorNextEntityByString( vTZERSAPO_Copy, "SearchString", "SearchString", szSearchString, "" ) < zCURSOR_SET )
         {
            CreateEntity( vTZERSAPO, "SearchString", zPOS_LAST );
            SetCursorLastEntity( vTZERSAPO, "SearchString", "" );
            SetAttributeFromString( vTZERSAPO, "SearchString", "SearchString", szSearchString );
            SetAttributeFromString( vTZERSAPO, "SearchString", "OriginalSearchString", szSearchString );
            SetAttributeFromInteger( vTZERSAPO, "SearchString", "CountTables", -1 );
            RefreshWindow( vSubtask );
         }
         else
         {
            SetCursorFirstEntityByString( vTZERSAPO, "SearchString", "SearchString", szSearchString, "" );
         }
      }
   }

   DropView( vTZERSAPO_Copy );

   return( 0 );
} // zwTZERSAPD_CreateNewSearchString


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ExistThisSearchString
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_ExistThisSearchString( zVIEW  vSubtask,
                                  zVIEW  vTZERSAPO )
{
   zVIEW   vTZERSAPO_Copy;
   zCHAR   szSearchString[ 257 ];

   // if update search string, create a new search string with this data
   // and reset old search string data
   zwTZERSAPD_CreateNewSearchString( vSubtask, vTZERSAPO );

   CreateViewFromViewForTask( &vTZERSAPO_Copy, vTZERSAPO, 0 );
   GetStringFromAttribute( szSearchString, sizeof( szSearchString ), vTZERSAPO, "SearchString", "SearchString" );

   // if a Table list for this search string already exists, do load it
   // (do not start a new SAP connection)
   if ( SetCursorFirstEntityByString( vTZERSAPO_Copy, "SearchString", "SearchString", szSearchString, "" ) >= zCURSOR_SET )
   {
      // remove the new search string, when this search string exists with tables
      if ( SetCursorNextEntityByString( vTZERSAPO_Copy, "SearchString", "SearchString", szSearchString, "" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToAttribute( vTZERSAPO_Copy, "SearchString", "SearchString",
                                           vTZERSAPO_Copy, "SearchString", "OriginalSearchString" ) == 0 )
         {
            SetAttributeFromString( vTZERSAPO_Copy, "SearchString", "SearchString", "" );
            SetAttributeFromString( vTZERSAPO_Copy, "SearchString", "OriginalSearchString", "" );
            SetAttributeFromInteger( vTZERSAPO_Copy, "SearchString", "CountTables", -1 );
         }
         else
         {
            SetAttributeFromAttribute( vTZERSAPO_Copy, "SearchString", "SearchString", vTZERSAPO_Copy, "SearchString", "OriginalSearchString" );
         }
      }

      SetCursorFirstEntityByString( vTZERSAPO, "SearchString", "SearchString", szSearchString, "" );
      zwTZERSAPD_SetCountTable( vSubtask, vTZERSAPO );
      RefreshWindow( vSubtask );
   }

   DropView( vTZERSAPO_Copy );

   if ( CompareAttributeToInteger( vTZERSAPO, "SearchString", "CountTables", -1 ) == 0 )
   {
      SetAttributeFromInteger( vTZERSAPO, "SearchString", "CountTables", 0 );
      return( 0 );
   }
   else
   {
      return( 1 );
   }

} // zwTZERSAPD_ExistThisSearchString



static zSHORT
fnTZERSAPD_SAP_ReadErrorMsg( zVIEW lpTaskView, zCPCHAR cpcMessage )
{
   zPCHAR            pszSAP_Msg = NULL;
   pfnGetLastError   pGetLastError;
   zCHAR             szMsg[ 512 ];

   pGetLastError = (pfnGetLastError) SysGetProc( hLibrary, "ZSAP_GetLastError" );
   if ( pGetLastError == 0 )
      return( fnTZERSAPD_SAP_LoadErrorMsg( lpTaskView, "ZSAP_GetLastError" ) );

   pszSAP_Msg = (*pGetLastError)( );

   strcpy_s( szMsg, sizeof( szMsg ), cpcMessage );
   strcat_s( szMsg, sizeof( szMsg ), ":\n\n" );
   strcat_s( szMsg, sizeof( szMsg ), pszSAP_Msg );

   SysMessageBox( lpTaskView, "SAP Import", szMsg, 1 );

   return( zCALL_ERROR );
}

static zSHORT
fnTZERSAPD_SAP_LoadErrorMsg( zVIEW lpTaskView, zPCHAR szOperation )
{
   zCHAR  szMsg[ 512 ];

   strcpy_s( szMsg, sizeof( szMsg ), "Can't find operation '" );
   strcat_s( szMsg, sizeof( szMsg ), szOperation );
   strcat_s( szMsg, sizeof( szMsg ), "' in library KZSAPACC." );

   SysMessageBox( lpTaskView, "SAP Import", szMsg, 1 );

   return( zCALL_ERROR );
}

static zSHORT
fnTZERSAPD_SAP_Connect( zVIEW   vSubtask,
                        zVIEW   vTZERSAPO,
                        zPCHAR  szConnection )
{
   zCHAR          szClient[ 4 ];
   zCHAR          szUser[ 13 ];
   zCHAR          szPassword[ 9 ];
   zCHAR          szServer[ 33 ];
   pfnSapLogon    pfnLogon;
   pfnSapLogoff   pfnLogoff;

   if ( zstrcmp( szConnection, "ZSAP_Logon" ) == 0 )
   {
      MB_SetMessage( vSubtask, 0, "SAP Logon" );
      GetStringFromAttribute( szClient, sizeof( szClient ), vTZERSAPO, "SapEnvironment", "Client" );
      GetStringFromAttribute( szUser, sizeof( szUser ), vTZERSAPO, "SapEnvironment", "User" );
      GetStringFromAttributeByContext( szPassword, vTZERSAPO, "SapEnvironment", "Password", "Password", 8 );
      GetStringFromAttribute( szServer, sizeof( szServer ), vTZERSAPO, "SirEnvServer", "Server" );

      pfnLogon = (pfnSapLogon) SysGetProc( hLibrary, szConnection );
      if ( pfnLogon == 0 )
         return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, szConnection ) );

      if ( (*pfnLogon) ( szClient, szUser, szPassword, szServer ) < 0 )
      {
         fnTZERSAPD_SAP_ReadErrorMsg( vSubtask, "Logon failure" );
         return( -1 );
      }
   }
   else
   {
      MB_SetMessage( vSubtask, 0, "SAP Logoff" );
      GetStringFromAttribute( szClient, sizeof( szClient ), vTZERSAPO, "Logon", "Client_Org" );
      GetStringFromAttribute( szUser, sizeof( szUser ), vTZERSAPO, "Logon", "User_Org" );
      GetStringFromAttributeByContext( szPassword, vTZERSAPO, "Logon", "Password_Org", "Password", 8 );
      GetStringFromAttribute( szServer, sizeof( szServer ), vTZERSAPO, "Logon", "Server_Org" );

      pfnLogoff = (pfnSapLogoff) SysGetProc( hLibrary, szConnection );
      if ( pfnLogoff == 0 )
         return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, szConnection ));

      if ( (*pfnLogoff) ( ) < 0 )
      {
         fnTZERSAPD_SAP_ReadErrorMsg( vSubtask, "Logoff failure" );
         return( -1 );
      }
   }

   if ( zstrcmp( szConnection, "ZSAP_Logon" ) == 0 )
   {
      SetAttributeFromString( vTZERSAPO, "Logon", "Client_Org", szClient );
      SetAttributeFromString( vTZERSAPO, "Logon", "User_Org", szUser );
      SetAttributeFromString( vTZERSAPO, "Logon", "Password_Org", szPassword );
      SetAttributeFromString( vTZERSAPO, "Logon", "Server_Org", szServer );
   }

   return( 0 );
} // fnTZERSAPD_SAP_Connect


static zSHORT
fnTZERSAPD_SAP_DropList( zVIEW lpTaskView,
                         void  *hList )
{
   pfnSapDropList pfnDropList;

   if ( hList != 0)
   {
      pfnDropList = (pfnSapDropList) SysGetProc( hLibrary, "ZSAP_DropList" );

      if ( pfnDropList == 0 )
         return( fnTZERSAPD_SAP_LoadErrorMsg( lpTaskView, "ZSAP_DropList" ) );

      (*pfnDropList) ( hList );
   }

   return( 0 );
} // fnTZERSAPD_SAP_DropList

static zSHORT
fnTZERSAPD_SAP_LoadTableData( zVIEW   vSubtask,
                              zVIEW   vTZERSAPO,
                              zPCHAR  szSearchString,
                              zCHAR   cLanguage )
{
   void*        hTableList = 0;
   zCHAR        szTableName[ 33 ];
   zCHAR        szType[ 9 ];
   zCHAR        szDesc[ 61 ];
   zCHAR        szMsg[ 50 ];
   zLONG        lTranspCount = 0;
   zLONG        lCountTables, k;

   pfnSapGetTableList  pfnGetTableList;
   pfnSapGetTable      pfnGetTable;

   MB_SetMessage( vSubtask, 0, "Read SAP Tables" );

   // Read Table Informations
   pfnGetTableList = (pfnSapGetTableList)
                     SysGetProc( hLibrary, "ZSAP_GetTableList" );
   if ( pfnGetTableList == 0 )
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetTableList" ));

   lCountTables = (*pfnGetTableList) ( szSearchString, cLanguage, &hTableList );
   if ( lCountTables < 0 )
   {
      fnTZERSAPD_SAP_ReadErrorMsg( vSubtask, "Error getting Table List" );
      SetCtrlText( vSubtask, "txtCountTable", "Error by Read Tables:" );
      return( -1 );
   }

   // Read Table Item Informations
   pfnGetTable = (pfnSapGetTable) SysGetProc( hLibrary, "ZSAP_GetTable" );
   if ( pfnGetTable == 0 )
   {
      fnTZERSAPD_SAP_DropList( vSubtask, hTableList );
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetTable" ));
   }

   for ( k = 0; k < lCountTables; k++ )
   {
      if ( (*pfnGetTable) ( hTableList, k, szTableName, szType, szDesc ) < 0 )
      {
         fnTZERSAPD_SAP_ReadErrorMsg( vSubtask, "Error getting Table Data" );
         fnTZERSAPD_SAP_DropList( vSubtask, hTableList );
         return( -1 );
      }

      if ( zstrcmp( szType, "TRANSP" ) == 0 || zstrcmp( szType, "POOL" ) == 0 ||
           zstrcmp( szType, "VIEW" ) == 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Load Table: " );
         strcat_s( szMsg, sizeof( szMsg ), szTableName );
         MB_SetMessage( vSubtask, 0, szMsg );
         lTranspCount++;
         CreateEntity( vTZERSAPO, "Table", zPOS_AFTER );
         SetAttributeFromString( vTZERSAPO, "Table", "TableName", szTableName );
         SetAttributeFromString( vTZERSAPO, "Table", "ParentName", szTableName );
         SetAttributeFromString( vTZERSAPO, "Table", "WorkName", szTableName );
         SetAttributeFromString( vTZERSAPO, "Table", "IndentName", szTableName );
         SetAttributeFromString( vTZERSAPO, "Table", "Desc", szDesc );
         SetAttributeFromInteger( vTZERSAPO, "Table", "IndentLvl", 0 );
      }
   }

   // Set Count Table to Textfield
   SetAttributeFromInteger( vTZERSAPO, "SearchString", "CountTables", lTranspCount );
   zwTZERSAPD_SetCountTable( vSubtask, vTZERSAPO );
   OrderEntityForView( vTZERSAPO, "Table", "WorkName A" );

   // Drop Table list
   fnTZERSAPD_SAP_DropList( vSubtask, hTableList );

   return( 0 );
} // fnTZERSAPD_SAP_LoadTableData

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_GetSearchString
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_GetSearchString( zVIEW   vSubtask,
                            zPCHAR  szSearchString )
{
   zCHAR   szSearchStr[ 257 ];
   zLONG   lLen, i, j, k;

   szSearchStr[ 0 ] = 0;
   lLen = zstrlen( szSearchString );

   // remove blanks from begin
   j = 0;
   for ( i = 0; i<= lLen; i++ )
   {
      if ( szSearchString[ i ] == ' ' )
         j++;
      else
         break;
   }

   k = 0;
   for ( i = j; i <= lLen; i++ )
   {
      if ( szSearchString[ i ] != ' ' && szSearchString[ i ] != ',' && szSearchString[ i ] != ';')
      {
         szSearchStr[ k ] = szSearchString[ i ];
         k++;
      }
      else
      if ( k > 0 && szSearchStr[ k - 1 ] != ';' )
      {
         szSearchStr[ k ] = ';';
         k++;
      }
   }

   // remove last semicolon, if exists
   if ( k > 1 && szSearchStr[ k - 2 ] == ';' )
      szSearchStr[ k - 2 ] = 0;

   if ( zstrlen( szSearchStr ) == 0 )
   {
      MessageSend( vSubtask, "SAP00001", "SAP Import",
                   "Search String for Tables Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      TAB_SetActiveTab( vSubtask, "Tab", 1 );
      SetFocusToCtrl( vSubtask, "cbSearchString" );
      return( -1 );
   }

   strcpy_s( szSearchString, sizeof( szSearchString ), szSearchStr );
   return( 0 );
} // zwTZERSAPD_GetSearchString


static zBOOL
fnTZERSAPD_IsChangeLogonParam( zVIEW  vTZERSAPO )
{

   if ( CompareAttributeToAttribute( vTZERSAPO, "SapEnvironment", "Client", vTZERSAPO, "Logon", "Client_Org" ) != 0 ||
        CompareAttributeToAttribute( vTZERSAPO, "SapEnvironment", "User", vTZERSAPO, "Logon", "User_Org" ) != 0 ||
        CompareAttributeToAttribute( vTZERSAPO, "SapEnvironment", "Password", vTZERSAPO, "Logon", "Password_Org" ) != 0 ||
        CompareAttributeToAttribute( vTZERSAPO, "SirEnvServer", "Server", vTZERSAPO, "Logon", "Server_Org" ) != 0 )
   {
      return( TRUE );
   }
   else
      return( FALSE );

} // fnTZERSAPD_IsChangeLogonParam



static zSHORT
fnTZERSAPD_CreateNewLogonEntity( zVIEW  vTZERSAPO )
{
   zSHORT  nRC;
   zBOOL   bLogonExists = FALSE;
   zCHAR   szClient[ 4 ];
   zCHAR   szUser[13];
   zCHAR   szPassword[9];
   zCHAR   szServer[33];

   GetStringFromAttribute( szClient, sizeof( szClient ), vTZERSAPO, "SapEnvironment", "Client" );
   GetStringFromAttribute( szUser, sizeof( szUser ), vTZERSAPO, "SapEnvironment", "User" );
   GetStringFromAttributeByContext( szPassword, vTZERSAPO, "SapEnvironment", "Password", "Password", 8 );
   GetStringFromAttribute( szServer, sizeof( szServer ), vTZERSAPO, "SirEnvServer", "Server" );

   for ( nRC = SetCursorFirstEntity( vTZERSAPO, "Logon", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSAPO, "Logon", "" ) )
   {
      if ( CompareAttributeToAttribute( vTZERSAPO, "SapEnvironment", "Client", vTZERSAPO, "Logon", "Client_Org" ) == 0 &&
           CompareAttributeToAttribute( vTZERSAPO, "SapEnvironment", "User", vTZERSAPO, "Logon", "User_Org" ) == 0 &&
           CompareAttributeToAttribute( vTZERSAPO, "SapEnvironment", "Password", vTZERSAPO, "Logon", "Password_Org" ) == 0 &&
           CompareAttributeToAttribute( vTZERSAPO, "SirEnvServer", "Server", vTZERSAPO, "Logon", "Server_Org" ) == 0 )
      {
         bLogonExists = TRUE;
         break;
      }
   }

   if ( !bLogonExists )
   {
      CreateEntity( vTZERSAPO, "Logon", zPOS_LAST );
      CreateEntity( vTZERSAPO, "SearchString", zPOS_LAST );
      SetAttributeFromInteger( vTZERSAPO, "SearchString", "CountTables", -1 );
   }

   return( 0 );
} // fnTZERSAPD_CreateNewLogonEntity


static zSHORT
fnTZERSAPD_CheckNewLogon( zVIEW  vSubtask,
                          zVIEW  vTZERSAPO )
{
   zSHORT  nRC = 0;

   // if first connection?
   if ( CompareAttributeToInteger( vTZERSAPO, "SapEnvironment", "Connection", 0 ) == 0 )
   {
      // SAP Logon
      nRC = fnTZERSAPD_SAP_Connect( vSubtask, vTZERSAPO, "ZSAP_Logon" );
      if ( nRC < 0 )
         return( nRC );

      SetAttributeFromInteger( vTZERSAPO, "SapEnvironment", "Connection", 1 );
   }
   else
   if ( fnTZERSAPD_IsChangeLogonParam( vTZERSAPO ) )
   {
      // SAP Logoff
      nRC = fnTZERSAPD_SAP_Connect( vSubtask, vTZERSAPO, "ZSAP_Logoff" );
      if ( nRC < 0 )
         return( nRC );

      // Create new Logon Entity in Work Lod
      fnTZERSAPD_CreateNewLogonEntity( vTZERSAPO );

      // SAP Logon
      nRC = fnTZERSAPD_SAP_Connect( vSubtask, vTZERSAPO, "ZSAP_Logon" );
      if ( nRC < 0 )
         return( nRC );

      SetAttributeFromInteger( vTZERSAPO, "SapEnvironment", "Connection", 1 );
   }

   return( 0 );
} // fnTZERSAPD_CheckNewLogon


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SearchSapTable
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SearchSapTable( zVIEW vSubtask )
{
   zVIEW        vTZERSAPO;
   zCHAR        szSearchString[ 257 ];
   zCHAR        szLanguage[ 2 ];
   zCHAR        cLanguage;
   zSHORT       nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   if ( zwTZERSAPD_CheckValues( vSubtask, vTZERSAPO ) < 0 )
      return( -1 );

   GetStringFromAttribute( szSearchString, sizeof( szSearchString ), vTZERSAPO, "SearchString", "SearchString" );
   if ( zwTZERSAPD_GetSearchString( vSubtask, szSearchString ) < 0 )
      return( -1);

   // new SAP Logon with other datas?
   nRC = fnTZERSAPD_CheckNewLogon( vSubtask, vTZERSAPO );
   if ( nRC < 0 )
      return( nRC );

   TranslateStringToUpperCase( szSearchString );
   SetAttributeFromString( vTZERSAPO, "SearchString", "SearchString", szSearchString );

   if ( zwTZERSAPD_ExistThisSearchString( vSubtask, vTZERSAPO ) == 1 )
   {
      zwTZERSAPD_LoadTablesFromWorkLod( vSubtask, vTZERSAPO );
      MB_SetMessage( vSubtask, 0, "Search Tables completed successfully" );
      return( 0 );
   }

   zwTZERSAPD_CreateNewSearchItem( vTZERSAPO );

   SetCtrlText( vSubtask, "txtCountTable", " Search Table(s)..." );

   // Read Table Informations and Table Data
   GetStringFromAttribute( szLanguage, sizeof( szLanguage ), vTZERSAPO, "SapEnvironment", "LanguageType" );
   cLanguage = szLanguage[ 0 ];
   nRC = fnTZERSAPD_SAP_LoadTableData( vSubtask, vTZERSAPO, szSearchString, cLanguage );
   if ( nRC < 0 )
      return( nRC );

   MB_SetMessage( vSubtask, 0, "Search Tables completed successfully" );

   return( 0 );
} // zwTZERSAPD_SearchSapTable



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetEntityPosition
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SetEntityPosition( zVIEW vERD,
                              zLONG lCount )
{
   zVIEW  vERD_Copy;
   zLONG  lMaxY = 700;
   zLONG  lLastPosX, lLastPosY;
   zLONG  lPosX, lPosY;
   zLONG  lLineCount, lEntityPerLine;
   zSHORT nRC;

   CreateViewFromViewForTask( &vERD_Copy, vERD, 0 );

   // Search for the largest PosY value for positioning entities.
   lLastPosY = -8;
   for ( nRC = SetCursorFirstEntity( vERD_Copy, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD_Copy, "ER_Entity", "" ) )
   {
      GetIntegerFromAttribute( &lPosY, vERD_Copy, "ER_Entity", "ER_DiagramPosY" );
      if ( lPosY > 0 && lPosY > lLastPosY )
         lLastPosY = lPosY;
   }

   lPosX = 1;
   lPosY = lLastPosY + 10;
   lLastPosX = 110;

//BLx
   if ( lCount > 0 )
   {
      lLineCount = (lMaxY - lPosY) / 10;
      if ( lLineCount != 0 )
         lEntityPerLine = lCount / lLineCount * 20;
      else
         lEntityPerLine = lCount * 20;

      if ( lEntityPerLine > lLastPosX )
         lLastPosX = lEntityPerLine;
   }
// ende

   for ( nRC = SetCursorFirstEntityByInteger( vERD_Copy, "ER_Entity", "ER_DiagramPosX", -1, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByInteger( vERD_Copy, "ER_Entity", "ER_DiagramPosX", -1, "" ) )
   {
      if ( CompareAttributeToInteger( vERD_Copy, "ER_Entity", "ER_DiagramPosY", -1 ) == 0 )
      {
         SetAttributeFromInteger( vERD_Copy, "ER_Entity", "ER_DiagramPosX", lPosX );
         SetAttributeFromInteger( vERD_Copy, "ER_Entity", "ER_DiagramPosY", lPosY );
         lPosX = lPosX + 20;
         if ( lPosX > lLastPosX )
         {
            lPosX = 1;
            lPosY = lPosY + 10;
         }
      }
   }

   DropView( vERD_Copy );

   return( 0 );
} // zwTZERSAPD_SetEntityPosition


static zSHORT
fnTZERSAPD_SAP_AttrSetDilMsg( zVIEW   vSubtask,
                              zPCHAR  szTableName,
                              zPCHAR  szAttr,
                              zPCHAR  szCountAttr,
                              zLONG   lIndex )
{
   zCHAR   szMsg[150];
   zCHAR   szMsg1[ 30 ];
   zCHAR   szShowIndex[ 10 ];
   zLONG   lShowIndex;

   strcpy_s( szMsg, sizeof( szMsg ), "Import Table: " );
   strcat_s( szMsg, sizeof( szMsg ), szTableName );
   strcat_s( szMsg, sizeof( szMsg ), ", Column: " );
   strcat_s( szMsg, sizeof( szMsg ), szAttr );
   MB_SetMessage( vSubtask, 0, szMsg );

   lShowIndex = 1 + lIndex;
   zltoa( lShowIndex, szShowIndex, sizeof( szShowIndex ), 10 );
   strcpy_s( szMsg1, sizeof( szMsg1 ), szShowIndex );
   strcat_s( szMsg1, sizeof( szMsg1 ), " / " );
   strcat_s( szMsg1, sizeof( szMsg1 ), szCountAttr );
   MB_SetMessage( vSubtask, 2, szMsg1 );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_IncludeDomain
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_IncludeDomain( zVIEW  vSubtask,
                          zVIEW  vERD,
                          zCHAR  cIntType,
                          zLONG  lDecimals )
{
   zVIEW   vDomain;
   zSHORT  nRC;
   zCHAR   szDomainName[ 33 ];
   zCHAR   szAttributeName[ 33 ];
   zCHAR   szMsg[ 254 ];

   if ( cIntType == SAP_SHORT || cIntType == SAP_BIN || cIntType == SAP_INT )
      strcpy_s( szDomainName, sizeof( szDomainName ), "Integer" );
   else
   if ( cIntType == SAP_CHAR )
      strcpy_s( szDomainName, sizeof( szDomainName ), "Text" );
   else
   if ( cIntType == SAP_DATE )
      strcpy_s( szDomainName, sizeof( szDomainName ), "Date" );
   else
   if ( cIntType == SAP_TIME )
      strcpy_s( szDomainName, sizeof( szDomainName ), "Time" );
   else
   if ( cIntType == SAP_HEX )
      strcpy_s( szDomainName, sizeof( szDomainName ), "AlphaNumeric" );
   else
   if ( cIntType == SAP_FLOAT || cIntType == SAP_NUM || cIntType == SAP_PACKED )
   {
      switch ( lDecimals )
      {
      case 0:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal0" );
         break;
      case 1:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal1" );
         break;
      case 2:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal2" );
         break;
      case 3:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal3" );
         break;
      case 4:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal4" );
         break;
      case 5:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal5" );
         break;
      case 6:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal6" );
         break;
      case 7:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal7" );
         break;
      case 8:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal8" );
         break;
      case 9:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal9" );
         break;
      default:
         strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal" );
      }
   }
   else
   {
      GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ), vERD, "ER_Attribute", "Name" );
      strcpy_s( szMsg, sizeof( szMsg ), "Attribute '" );
      strcat_s( szMsg, sizeof( szMsg ), szAttributeName );
      strcat_s( szMsg, sizeof( szMsg ), "' does not have Domain assigned, SAP Data Type '" );
      zstrncat( szMsg, &cIntType, 1 );
      strcat_s( szMsg, sizeof( szMsg ), "' is not implemented." );
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, szMsg, 1 );
      return( -1 );
   }

   nRC = ActivateMetaOI_ByName( vSubtask, &vDomain, 0, zREFER_DOMAIN_META, zSINGLE, szDomainName, 0 );
   if ( nRC < 1 )
   {
      GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ), vERD, "ER_Attribute", "Name" );
      strcpy_s( szMsg, sizeof( szMsg ), "Attribute '" );
      strcat_s( szMsg, sizeof( szMsg ), szAttributeName );
      strcat_s( szMsg, sizeof( szMsg ), "' does not have Domain assigned, Domain '" );
      strcat_s( szMsg, sizeof( szMsg ), szDomainName );
      strcat_s( szMsg, sizeof( szMsg ), "' was not found." );
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, szMsg, 1 );
      return( -1 );
   }
   else
   {
      IncludeSubobjectFromSubobject( vERD, "Domain", vDomain, "Domain", zPOS_AFTER );
   }

   DropMetaOI( vSubtask, vDomain );

   return( 0 );
} // zwTZERSAPD_IncludeDomain

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateIdentifier
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateIdentifier( zVIEW   vSubtask,
                             zVIEW   vERD,
                             zPCHAR  szTableName,
                             zSHORT  nIdentifierType )
{
   zCHAR  szIdentifierName[ 33 ];
   zSHORT nRC;

   // Identifier does not exists, create it
   if ( CheckExistenceOfEntity( vERD, "ER_EntIdentifier" ) < zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER );
      strcpy_s( szIdentifierName, sizeof( szIdentifierName ), szTableName );
      strcat_s( szIdentifierName, sizeof( szIdentifierName ), "_ID" );
      SetAttributeFromString( vERD, "ER_EntIdentifier", "Name", szIdentifierName );
      SetAttributeFromString( vERD, "ER_EntIdentifier", "TE_KeyName", szIdentifierName );
   }

   CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER );

   if ( nIdentifierType == zAttributeID )
      nRC = IncludeSubobjectFromSubobject( vERD, "ER_AttributeIdentifier",
                                           vERD, "ER_Attribute", zPOS_AFTER );
   else
      nRC = IncludeSubobjectFromSubobject( vERD, "ER_RelLinkIdentifier",
                                           vERD, "ER_RelLink_2", zPOS_AFTER );

   return( nRC );
} // zwTZERSAPD_CreateIdentifier

static zSHORT
fnTZERSAPD_CreateAttribAndIdent( zVIEW          vSubtask,
                                 zVIEW          vERD,
                                 zPCHAR         szTableName,
                                 zPCHAR         szCountAttr,
                                 pfnSapGetAttr  pfnGetAttr,
                                 void*          hAttribList,
                                 zLONG          lIndex,
                                 zPSHORT        pnExistFK )
{
   zCHAR   szAttr[ 32 ];
   zCHAR   szDesc[61];
   zCHAR   szFK_TableName[ 32 ];
   zCHAR   szRefTable[ 32 ];
   zCHAR   szPrecField[ 32 ];
   zCHAR   szRefField[ 32 ];
   zCHAR   szDomName[ 32 ];
   zLONG   lPosition, nRC;
   zLONG   lIntLen, lLeng, lDecimals;
   zCHAR   cKeyFlag, cMand, cIntType, cNotNull, cDataType[ 4 ];

   // Load Attribute Datas
   nRC = (*pfnGetAttr) ( hAttribList, lIndex, szAttr, &lPosition, &cKeyFlag, &cMand,
                         szFK_TableName, &cIntType, &lIntLen, szRefTable,
                         szPrecField, szRefField, &cNotNull, &cDataType[ 0 ], &lLeng,
                         &lDecimals, szDomName, szDesc );
   if ( nRC < 0 )
   {
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, "Attributes not found.", 1 );
      if ( lIgnoreError == 1 )
         return( 0 );
      else
         return( -1 );
   }

   // in order not to create a Attribute with Name = ".INCLUDE" or ".APPEND"
   //  we skip any attribute starting with '.'
   if ( szAttr[ 0 ] == '.' )
      return( 0 );

   // Set DIL Message
   fnTZERSAPD_SAP_AttrSetDilMsg( vSubtask, szTableName, szAttr, szCountAttr, lIndex );

   // Create Attribute
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_LAST );

   SetAttributeFromString( vERD, "ER_Attribute", "Name", szAttr );
   SetAttributeFromString( vERD, "ER_Attribute", "Desc", szDesc );
   SetAttributeFromString( vERD, "ER_Attribute", "TE_ColumnName", szAttr );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", lLeng );
   SetAttributeFromInteger( vERD, "ER_Attribute", "SAP_Attribute", 1 );
   SetAttributeFromInteger( vERD, "ER_Attribute", "wkSequence", 1 + lIndex );

   if ( cNotNull == 'X' )
      SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );

   if ( cKeyFlag == 'X' )
   {
      // if Attribute a Foreign Key and this Foreign Key is a Part of the Key,
      // Entity is of type attributive
      if ( zstrcmp( szFK_TableName, "" ) != 0 )
         *pnExistFK = 1;

      SetAttributeFromString( vERD, "ER_Attribute", "Key", "Y" );
      SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
      // Create Identifier
      if ( zwTZERSAPD_CreateIdentifier( vSubtask, vERD, szTableName, zAttributeID ) < 0 )
         return( -1 );
   }

   // Include Standard Domain
   if ( zwTZERSAPD_IncludeDomain( vSubtask, vERD, cIntType, lDecimals ) < 0 && lIgnoreError == 0 )
   {
      return( -1 );
   }

   return( 0 );
} // fnTZERSAPD_CreateAttribAndIdent



static zSHORT
fnTZERSAPD_SAP_LoadAttributeData( zVIEW   vSubtask,
                                  zVIEW   vERD,
                                  zPCHAR  szTableName,
                                  zCHAR   cLanguage )
{
   void    *hAttribList = 0;
   zCHAR   szCountAttr[ 10 ];
   zLONG   lCountAttr, k;
   zSHORT  nExistFK = 0;
   zSHORT  nRC = 0;

   pfnSapGetAttrList  pfnGetAttrList;
   pfnSapGetAttr      pfnGetAttr;

   // Read Attribute Informations
   pfnGetAttrList = (pfnSapGetAttrList) SysGetProc( hLibrary, "ZSAP_GetAttrList" );
   if ( pfnGetAttrList == 0 )
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetAttrList" ));

   lCountAttr = (*pfnGetAttrList) ( szTableName, cLanguage, &hAttribList );
   if ( lCountAttr < 0 )
   {
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, "Attributes not found.", 1 );
      if ( lIgnoreError == 1 )
         return( 0 );
      else
         return( -1 );
   }

   // Read Attribute Item Informations
   pfnGetAttr = (pfnSapGetAttr) SysGetProc( hLibrary, "ZSAP_GetAttr" );
   if ( pfnGetAttr == 0 )
   {
      fnTZERSAPD_SAP_DropList( vSubtask, hAttribList );
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetAttr" ));
   }

   zltoa( lCountAttr, szCountAttr, sizeof( szCountAttr ) );
   for ( k = 0; k < lCountAttr; k++ )
   {
      nRC = fnTZERSAPD_CreateAttribAndIdent( vSubtask, vERD, szTableName, szCountAttr, pfnGetAttr, hAttribList, k, &nExistFK );
      if ( nRC < 0 )
      {
         break;
      }
   }

   // An Entity is considered to be fundamental if it does not have a foreign key as part of the key
   if ( nExistFK == 0 )
      SetAttributeFromString( vERD, "ER_Entity", "Purpose", "F" );

   // Drop Attribute list
   fnTZERSAPD_SAP_DropList( vSubtask, hAttribList );

   if ( lIgnoreError == 1 )
      return( 0 );
   else
      return( nRC );

} // fnTZERSAPD_SAP_LoadAttibuteData

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateAttribAndIdent
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateAttribAndIdent( zVIEW   vSubtask,
                                 zVIEW   vERD,
                                 zVIEW   vTZERSAPO,
                                 zPCHAR  szTableName )
{
   zSHORT  nRC;
   zCHAR   szMsg[ 50 ];
   zCHAR   szLanguage[ 2 ];
   zCHAR   cLanguage;

   // new SAP Logon with other datas?
   nRC = fnTZERSAPD_CheckNewLogon( vSubtask, vTZERSAPO );
   if ( nRC < 0 )
      return( nRC );

   // Set DIL Message
   strcpy_s( szMsg, sizeof( szMsg ), "Import Table: " );
   strcat_s( szMsg, sizeof( szMsg ), szTableName );
   MB_SetMessage( vSubtask, 0, szMsg );

   // Read Table Informations and Table Data
   GetStringFromAttribute( szLanguage, sizeof( szLanguage ), vTZERSAPO, "SapEnvironment", "LanguageType" );
   cLanguage = szLanguage[ 0 ];

   if ( fnTZERSAPD_SAP_LoadAttributeData( vSubtask, vERD, szTableName, cLanguage ) < 0 )
   {
      return( -1);
   }

   return( 0 );
} // zwTZERSAPD_CreateAttribAndIdent

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZERSAPD_DeleteEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnTZERSAPD_DeleteEntity( zVIEW   vSubtask,
                         zVIEW   vERD,
                         zPLONG  plPosY,
                         zPLONG  plPosX,
                         zPLONG  plZKey,
                         zPSHORT pnEntityInSA )
{
   zVIEW  vTZERSASO;
   zSHORT nRC;

   // get old ZKey
   GetIntegerFromAttribute( plZKey, vERD, "ER_Entity", "ZKey" );

   // get old Entity Position
   GetIntegerFromAttribute( plPosY, vERD, "ER_Entity", "ER_DiagramPosY" );
   GetIntegerFromAttribute( plPosX, vERD, "ER_Entity", "ER_DiagramPosX" );

   // if Entity in Subject Area, delete it
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 &&
        SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey", vERD, "ER_Entity", "ZKey", "SubjectArea" ) >= zCURSOR_SET )
   {
      *pnEntityInSA = 1;
      ExcludeEntity( vTZERSASO, "ER_Entity", zREPOS_NONE );
   }

   //  Delete the entity if it is involved in any relationships.
   //  Loop and Delete all ER RelLinks
   for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_RelLink", "" ) )
   {
      oTZEREMDO_DeleteRelationship( vERD, zREPOS_NONE, 1 );
   }

   DeleteEntity( vERD, "ER_Entity", zREPOS_PREV );

   return( 0 );
} // fnTZERSAPD_DeleteEntity



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZERSAPD_CheckExistsOfEntity
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnTZERSAPD_CheckExistsOfEntity( zVIEW   vSubtask,
                                zVIEW   vERD,
                                zVIEW   vTZERSAPO,
                                zPCHAR  szTableName,
                                zPLONG  plPosY,
                                zPLONG  plPosX,
                                zPLONG  plZKey,
                                zPSHORT pnEntityInSA )
{
   zVIEW    vTZERSAPO1;
   zCHAR    szMsg[ 100 ];
   zBOOL    bEntityExists = FALSE;
   zSHORT   nRC;

   // check exists of Entity in ERD
   if ( SetCursorFirstEntityByString( vERD, "ER_Entity", "TE_TableName", szTableName, "" ) >= zCURSOR_SET )
   {
      bEntityExists = TRUE;
      CreateViewFromViewForTask( &vTZERSAPO1, vTZERSAPO, 0 );
      if ( SetCursorFirstEntityByString( vTZERSAPO1, "Table", "TableName", szTableName, "" ) >= zCURSOR_SET &&
           CompareAttributeToInteger( vTZERSAPO1, "Table", "TablesIsImported", 1 ) == 0 )
      {
         // Table is imported, do not ask for replace and do not replace
         SetAttributeFromInteger( vTZERSAPO, "Table", "TablesIsImported", 1 );
         DropView( vTZERSAPO1 );
         return( 1 );
      }

      DropView( vTZERSAPO1 );
   }

   if ( bEntityExists )
   {
      // do not ask for replace
      if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "OverwriteExistingEntity", "Y" ) != 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Table '" );
         strcat_s( szMsg, sizeof( szMsg ), szTableName );
         strcat_s( szMsg, sizeof( szMsg ), "' already exists in Data Model.\n" );
         strcat_s( szMsg, sizeof( szMsg ), "Replace existing Entity?" );
         nRC = MessagePrompt( vERD, "SAP00002", "SAP Import", szMsg, zBEEP,
                              zBUTTONS_YESNOCANCEL, zRESPONSE_NO, zICON_QUESTION );
         if ( nRC == zRESPONSE_NO )
         {
            SetAttributeFromInteger( vTZERSAPO, "Table", "TablesIsImported", 1 );
            return( zRESPONSE_NO );
         }
         else
         if ( nRC == zRESPONSE_CANCEL )
         {
            zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, "Cancel Import Tables.", -1 );
            return( zAbortImport );
         }
      }

      // get old ZKey, Entity Position and delete old Entity with Relationships
      fnTZERSAPD_DeleteEntity( vSubtask, vERD, plPosY, plPosX, plZKey, pnEntityInSA );
   }

   return( 0 );
} // fnTZERSAPD_CheckExistsOfEntity


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateEntity
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateEntity( zVIEW vSubtask,
                         zVIEW vERD,
                         zVIEW vTZERSAPO )
{
   zCHAR    szTableName[ 33 ];
   zCHAR    szMsg[50];
   zVIEW    vTZERSASO;
   zLONG    lPosY = -1;
   zLONG    lPosX = -1;
   zLONG    lZKey = 0;
   zSHORT   nEntityInSA = 0;
   zSHORT   nRC;

   GetStringFromAttribute( szTableName, sizeof( szTableName ), vTZERSAPO, "Table", "TableName" );

   strcpy_s( szMsg, sizeof( szMsg ), "Import Table: " );
   strcat_s( szMsg, sizeof( szMsg ), szTableName );
   MB_SetMessage( vSubtask, 0, szMsg );

   // check exists of Entity in ERD
   nRC = fnTZERSAPD_CheckExistsOfEntity( vSubtask, vERD, vTZERSAPO, szTableName, &lPosY, &lPosX, &lZKey, &nEntityInSA );
   if ( nRC )
      return( nRC );

   SetAttributeFromInteger( vTZERSAPO, "Table", "TablesIsImported", 1 );

   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_LAST );

   // use old ZKey
   if ( lZKey != 0 )
   {
      SetAttributeFromInteger( vERD, "ER_Entity", "ZKey", lZKey );
      // Entity was in current Subject Area, include the new Entity again
      if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 && nEntityInSA == 1 )
      {
         IncludeSubobjectFromSubobject( vTZERSASO, "ER_Entity", vERD, "ER_Entity", zPOS_FIRST );
      }
   }

   SetAttributeFromString( vERD, "ER_Entity", "Name", szTableName );
   SetAttributeFromString( vERD, "ER_Entity", "TE_TableName", szTableName );
   SetAttributeFromAttribute( vERD, "ER_Entity", "Desc", vTZERSAPO, "Table", "Desc");
   SetAttributeFromString( vERD, "ER_Entity", "Purpose", "A" );
   SetAttributeFromInteger( vERD, "ER_Entity", "SAP_Entity", 1 );

   // set correctly position later (after Import Tables)
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lPosY );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", lPosX );

   // Create Attributes
   if ( zwTZERSAPD_CreateAttribAndIdent( vSubtask, vERD, vTZERSAPO, szTableName ) < 0 )
      return( -1 );

   return( 0 );
} // zwTZERSAPD_CreateEntity


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateDataModel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateDataModel( zVIEW   vSubtask,
                            zPVIEW  vERD )
{
   zVIEW  vERD_Window;
   zVIEW  vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   ActivateEmptyMetaOI( vSubtask, vERD, zSOURCE_ERD_META,
                        zSINGLE | zLEVEL_APPLICATION );

   // Build Root Instance
   CreateMetaEntity( vSubtask, *vERD, "EntpER_Model", zPOS_AFTER );
   SetAttributeFromAttribute( *vERD, "EntpER_Model", "Name", vTaskLPLR, "LPLR", "Name" );
   SetAttributeFromString( *vERD, "EntpER_Model", "Desc", "SAP Data Model" );
   SetNameForView( *vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Data Model exists, enable ERD Control
   GetParentWindow( &vERD_Window, vSubtask );
   SetCtrlState( vERD_Window, "ERD", zCONTROL_STATUS_ENABLED, TRUE );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vERD_Window, "Data Model", "TZEREMDO", *vERD, "EntpER_Model", zSOURCE_ERD_META );

   return( 0 );
} // zwTZERSAPD_CreateDataModel


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ExistStandardDomain
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_ExistStandardDomain( zVIEW vSubtask )
{
   zVIEW  vDomainList;
   zCHAR  szMsg[ 100 ];
   zCHAR  szDomainName[ 33 ];
   zBOOL  bError = FALSE;

   // Get Access to Domain List
   RetrieveViewForMetaList( vSubtask, &vDomainList, zREFER_DOMAIN_META );

   if ( SetCursorFirstEntity( vDomainList, "W_MetaDef", "" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00133", "SAP Import",
                   "Import aborted, because of missing Standard Domains",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   // check exists of Standard Domains
   else
   if ( SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", "AlphaNumeric", "" ) < zCURSOR_SET )
   {
      bError = TRUE;
      strcpy_s( szDomainName, sizeof( szDomainName ), "AlphaNumeric" );
   }
   else
   if ( SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", "Integer", "" ) < zCURSOR_SET )
   {
      bError = TRUE;
      strcpy_s( szDomainName, sizeof( szDomainName ), "Integer" );
   }
   else
   if ( SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", "Text", "" ) < zCURSOR_SET )
   {
      bError = TRUE;
      strcpy_s( szDomainName, sizeof( szDomainName ), "Text" );
   }
   else
   if ( SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", "Date", "" ) < zCURSOR_SET )
   {
      bError = TRUE;
      strcpy_s( szDomainName, sizeof( szDomainName ), "Date" );
   }
   else
   if ( SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", "Time", "" ) < zCURSOR_SET )
   {
      bError = TRUE;
      strcpy_s( szDomainName, sizeof( szDomainName ), "Time" );
   }
   else
   if ( SetCursorFirstEntityByString( vDomainList, "W_MetaDef", "Name", "Decimal", "" ) < zCURSOR_SET )
   {
      bError = TRUE;
      strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal" );
   }

   DropView ( vDomainList );

   if ( bError )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Import aborted, because Domain '" );
      strcat_s( szMsg, sizeof( szMsg ), szDomainName );
      strcat_s( szMsg, sizeof( szMsg ), "' does not exist. " );
      MessageSend( vSubtask, "ER00133", "SAP Import", szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   return( 0 );
} // zwTZERSAPD_ExistStandardDomain


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_RefreshErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_RefreshErrorList( zVIEW vSubtask )
{
   zwTZERSAPD_SetCountError( vSubtask );
   RefreshWindow( vSubtask );

   return( 0 );
} // zwTZERSAPD_RefreshErrorList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZERSAPD_Import_SAP_Tables
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnTZERSAPD_Import_SAP_Tables( zVIEW   vSubtask,
                              zVIEW   vERD,
                              zVIEW   vTZERSAPO,
                              zSHORT  nAllTables,
                              zPLONG  plCount )
{
   zSHORT  nRC, nRC1;

   // Import initial values
   for ( nRC = SetCursorFirstEntity( vTZERSAPO, "Table", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSAPO, "Table", "" ) )
   {
      SetAttributeFromInteger( vTZERSAPO, "Table", "TablesIsImported", 0 );
   }

   if ( nAllTables == zImportAllTables )
   {
      for ( nRC = SetCursorFirstEntity( vTZERSAPO, "Table", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERSAPO, "Table", "" ) )
      {
         *plCount = 1+ *plCount;
         nRC1 = zwTZERSAPD_CreateEntity( vSubtask, vERD, vTZERSAPO );
         if ( nRC1 < 0 )
         {
            return( nRC1 );
         }
      }
   }
   else
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZERSAPO, "Table", "" ) )
      {
         *plCount = 1+ *plCount;
         nRC1 = zwTZERSAPD_CreateEntity( vSubtask, vERD, vTZERSAPO );
         if ( nRC1 < 0 )
         {
            return( nRC1 );
         }
      }
   }

   return( 0 );
} // fnTZERSAPD_Import_SAP_Tables


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_WriteReasonOfAbort
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_WriteReasonOfAbort( zVIEW   vSubtask,
                               zVIEW   vTZERSAPO,
                               zVIEW   vTZSAPERR,
                               zSHORT  nReasonOfAbort )
{
   zCHAR  szMsg[ 100 ];
   zBOOL  bWriteMsg = TRUE;

   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_Tables ) == 0 )
   {
      if ( nReasonOfAbort == zAbortImport )
         bWriteMsg = FALSE;
      else
         strcpy_s( szMsg, sizeof( szMsg ), "Import Tables completed with Errors." );
   }
   else
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_RS_TE ) == 0 )
   {
      if ( nReasonOfAbort == zAbortBuild )
         strcpy_s( szMsg, sizeof( szMsg ), "Cancel Build Relationships and TE." );
      else
         strcpy_s( szMsg, sizeof( szMsg ), "TE not built." );
   }
   else
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_TE_only ) == 0 )
   {
      if ( nReasonOfAbort == zAbortBuild )
         strcpy_s( szMsg, sizeof( szMsg ), "Cancel Build TE." );
      else
         strcpy_s( szMsg, sizeof( szMsg ), "TE not built." );
   }
   else
   {
      if ( nReasonOfAbort == zAbortBuild )
         strcpy_s( szMsg, sizeof( szMsg ), "Cancel Build Relationships and TE." );
      else
      if ( nReasonOfAbort == zAbortImport )
         bWriteMsg = FALSE;
      else
      if ( nReasonOfAbort == zAbortImportBuild )
         strcpy_s( szMsg, sizeof( szMsg ), "Cancel Import Tables, Build Relationships and TE." );
      else
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Relationships and TE was not build, " );
         strcat_s( szMsg, sizeof( szMsg ), "because of Errors during Table Import." );
      }
   }

   if ( bWriteMsg )
   {
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, szMsg, -1 );
      SetCursorLastEntity( vTZSAPERR, "ErrorMsg", "" );
      SetAttributeFromString( vTZSAPERR, "ErrorMsg", "EntityName", "" );
   }

   return( 0 );
} // zwTZERSAPD_WriteReasonOfAbort



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ShowErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_ShowErrorList( zVIEW  vSubtask,
                          zVIEW  vTZERSAPO,
                          zVIEW  vTZSAPERR,
                          zSHORT nRC )
{
   zVIEW  vWindow;
   zBOOL  bShowErrorList = FALSE;

   // if Import with errors?
   if ( SetCursorFirstEntityByInteger( vTZSAPERR, "ErrorMsg", "Error", 1, "" ) >= zCURSOR_SET )
   {
      bShowErrorList = TRUE;
      if ( nRC != zAbortImport && nRC != zAbortBuild && nRC != zAbortImportBuild )
         zwTZERSAPD_WriteReasonOfAbort( vSubtask, vTZERSAPO, vTZSAPERR, nRC );
   }
   else
   if ( CompareAttributeToString( vTZSAPERR, "Import", "ShowErrorList", "Y" ) == 0 )
   {
      bShowErrorList = TRUE;
   }
   else
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 )
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );

   if ( nRC == zAbortImport || nRC == zAbortBuild || nRC == zAbortImportBuild )
      zwTZERSAPD_WriteReasonOfAbort( vSubtask, vTZERSAPO, vTZSAPERR, nRC );

   // if no Errors, delete Error Root
   if ( CheckExistenceOfEntity( vTZSAPERR, "ErrorMsg" ) < zCURSOR_SET )
      DeleteEntity( vTZSAPERR, "Import", zREPOS_NONE );

   if ( bShowErrorList )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ProcessImmediateAction | zWAB_StartOrFocusModelessWindow, "TZERSAPD", "ErrorList" );
      GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" );
      zwTZERSAPD_RefreshErrorList( vWindow );
   }

   return( 0 );
} // zwTZERSAPD_ShowErrorList


static zSHORT
fnTZERSAPD_SendMessageBox( zVIEW   vSubtask,
                           zPCHAR  szBuildFlag,
                           zPCHAR  szSubMsg,
                           zSHORT  nIcon )
{
   zCHAR  szMsg[ 100 ];

   if ( zstrcmp( szBuildFlag, zBuild_Tables ) == 0 )
      strcpy_s( szMsg, sizeof( szMsg ), "Import Tables completed " );
   else
   if ( zstrcmp( szBuildFlag, zBuild_RS_TE ) == 0 )
      strcpy_s( szMsg, sizeof( szMsg ), "Build Relationships and TE \ncompleted " );
   else
   if ( zstrcmp( szBuildFlag, zBuild_Tables_RS_TE ) == 0 )
      strcpy_s( szMsg, sizeof( szMsg ), "Import Tables, Build Relationships and TE \ncompleted " );
   else
   if ( zstrcmp( szBuildFlag, zBuild_TE_only ) == 0 )
      strcpy_s( szMsg, sizeof( szMsg ), "Build TE only completed " );

   strcat_s( szMsg, sizeof( szMsg ), szSubMsg );
   strcat_s( szMsg, sizeof( szMsg ), "." );

   MB_SetMessage( vSubtask, 0, szMsg );

   if ( nIcon == zICON_ERROR )
      SysMessageBox( vSubtask, "SAP Import", szMsg, 1 );
   else
      MessagePrompt( vSubtask, "SAP00003", "SAP Import",
                     szMsg, zBEEP, 0, 0, nIcon );

   return( 0 );
} // fnTZERSAPD_SendMessageBox


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_Build_AskForSave
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_Build_AskForSave( zVIEW vSubtask,
                             zVIEW vTZERSAPO )
{
   zSHORT   nRC;
   zCHAR    szMsg[150];

   // if Option Import Tables only, do not ask for save
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_Tables ) == 0 )
   {
      return( 0 );
   }

   strcpy_s( szMsg, sizeof( szMsg ), "Build Relationships and TE will cause \n" );
   strcat_s( szMsg, sizeof( szMsg ), "the ERD and the TE to be saved. \n\n" );
   strcat_s( szMsg, sizeof( szMsg ), "Do you want to continue?" );

   nRC = MessagePrompt( vSubtask, "SAP00004", "SAP Import", szMsg, zBEEP,
                        zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );

   if ( nRC == zRESPONSE_NO )
      return( zAbortBuild );
   else
      return( 0 );

} // zwTZERSAPD_Build_AskForSave

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_RefreshAllWindows
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_RefreshAllWindows( zVIEW  vSubtask,
                              zVIEW  vTZERSAPO,
                              zVIEW  vTZSAPERR,
                              zSHORT nReasonOfAbort )
{
   zVIEW  vWindow;

   // Refresh Erorr List Window
   zwTZERSAPD_ShowErrorList( vSubtask, vTZERSAPO, vTZSAPERR, nReasonOfAbort );

   // Refresh Data Model Window
   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   RefreshDiagram( vWindow, "ERD", 0 );

   // Refresh Subject Areas Window
   if ( GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshDiagram( vWindow, "ERD", 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SAP_Import" ) < 0 )
      zwTZERSAPD_CloseImport( vSubtask );

   return( 0 );
} // zwTZERSAPD_RefreshAllWindows

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_Import_SAP_Tables
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_Import_SAP_Tables( zVIEW   vSubtask,
                              zVIEW   vTZERSAPO,
                              zSHORT  nAllTables )
{
   zVIEW  vTZSAPERR;
   zVIEW  vERD = 0;
   zLONG  lCount = 0;
   zSHORT nRC;

   // if Domain does not exist, return with error
   if ( zwTZERSAPD_ExistStandardDomain( vSubtask ) < 0 )
      return( -1 );

   // added search string to Error List and load Error List View
   zwTZERSAPD_CreateNewErrorRoot( vSubtask );
   GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );

   if ( zwTZERSAPD_Build_AskForSave( vSubtask, vTZERSAPO ) < 0 )
   {
      // Refresh Erorr List Window and ERD
      zwTZERSAPD_RefreshAllWindows( vSubtask, vTZERSAPO, vTZSAPERR, zAbortImportBuild );
      return( 0 );
   }

   // if data model does not exists, create it
   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      zwTZERSAPD_CreateDataModel( vSubtask, &vERD );

   // Import Tables
   GetIntegerFromAttribute( &lIgnoreError, vTZERSAPO, "SapEnvironment", "IgnoreError" );
   nRC = fnTZERSAPD_Import_SAP_Tables( vSubtask, vERD, vTZERSAPO, nAllTables, &lCount );

   // Set new Entity Position
   MB_SetMessage( vSubtask, 2, "" );
   MB_SetMessage( vSubtask, 0, "Set new Entity Position" );
   zwTZERSAPD_SetEntityPosition( vERD, lCount );

   // if Import Tables without Errors?
   if ( SetCursorFirstEntityByInteger( vTZSAPERR, "ErrorMsg", "Error", 1, "" ) < zCURSOR_SET )
   {
      MB_SetMessage( vSubtask, 0, "Import Tables completed successfully" );
      // if Option Import Tables, Build Relationships and TE active?
      if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_Tables_RS_TE ) == 0 )
      {
         // Build Relationships and TE
         nRC = zwTZERSAPD_Build_SAP_RS_TE( vSubtask, vERD );
         if ( nRC < 0 )
            fnTZERSAPD_SendMessageBox( vSubtask, zBuild_Tables_RS_TE, "with Errors", zICON_ERROR );
         else
            fnTZERSAPD_SendMessageBox( vSubtask, zBuild_Tables_RS_TE, "successfully", zICON_INFORMATION );
      }
      else
         fnTZERSAPD_SendMessageBox( vSubtask, zBuild_Tables, "successfully", zICON_INFORMATION );
   }

   // Refresh Erorr List Window and ERD
   zwTZERSAPD_RefreshAllWindows( vSubtask, vTZERSAPO, vTZSAPERR, nRC );

   return( 0 );
} // zwTZERSAPD_Import_SAP_Tables



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_DeleteAll_SAP_Rels
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_DeleteAll_SAP_Rels( zVIEW vERD )
{
   zSHORT  nRC;

   // delete all Relationships between SAP Entity only
   for ( nRC = SetCursorFirstEntity( vERD, "ER_RelType", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_RelType", "" ) )
   {
      if ( CompareAttributeToInteger( vERD, "ER_Entity_2", "SAP_Entity", 1 ) == 0 )
      {
         SetCursorFirstEntityByEntityCsr( vERD, "ER_Entity", vERD, "ER_Entity_2", "" );
         SetCursorFirstEntityByEntityCsr( vERD, "ER_RelLink", vERD, "ER_RelLink_2", "" );
         if ( CompareAttributeToInteger( vERD, "ER_Entity_Other", "SAP_Entity", 1 ) == 0 )
         {
            oTZEREMDO_DeleteRelationship( vERD, zREPOS_NONE, 0 );
         }
      }
   }

   return( 0 );
} // zwTZERSAPD_DeleteAll_SAP_Rels



static zSHORT
fnTZERSAPD_SAP_RelSetDilMsg( zVIEW   vSubtask,
                             zPCHAR  szTableName,
                             zPCHAR  szFK_TableName,
                             zPCHAR  szCountRel,
                             zLONG   lIndex )
{
   zCHAR   szMsg[150];
   zCHAR   szMsg1[ 30 ];
   zCHAR   szShowIndex[ 10 ];
   zLONG   lShowIndex;

   strcpy_s( szMsg, sizeof( szMsg ), "Build Relationships, from Entity: " );
   strcat_s( szMsg, sizeof( szMsg ), szTableName );
   strcat_s( szMsg, sizeof( szMsg ), ", to Entity: " );
   strcat_s( szMsg, sizeof( szMsg ), szFK_TableName );
   MB_SetMessage( vSubtask, 0, szMsg );

   lShowIndex = 1 + lIndex;
   zltoa( lShowIndex, szShowIndex, sizeof( szShowIndex ) );
   strcpy_s( szMsg1, sizeof( szMsg1 ), szShowIndex );
   strcat_s( szMsg1, sizeof( szMsg1 ), " / " );
   strcat_s( szMsg1, sizeof( szMsg1 ), szCountRel );
   MB_SetMessage( vSubtask, 2, szMsg1 );

   return( 0 );
} // fnTZERSAPD_SAP_RelSetDilMsg



static zSHORT
fnTZERSAPD_CreateSecondRelLink( zVIEW   vSubtask,
                                zVIEW   vERD,
                                zPCHAR  szFK_FieldName,
                                zPCHAR  szCardLeft )
{
   zCHAR   szName[ 33 ];
   zSHORT  nRC;

   // Create First RelLink and include ER_Entity
   CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER );

   strcpy_s( szName, sizeof( szName ), szFK_FieldName );
   _strlwr( szName ); // convert to lowercase

   if ( zstrlen( szFK_FieldName ) > 28 )
      strcpy_s( szName + 28, sizeof( szName ) - 28, " for" );
   else
      strcat_s( szName, sizeof( szName ), " for" );

   SetAttributeFromString( vERD, "ER_RelLink_2", "Name", szName );

   if ( zstrcmp( szCardLeft, "C" ) == 0 )
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "0" );
   else
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "1" );

   SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "1" );

   nRC = IncludeSubobjectFromSubobject( vERD, "ER_Entity_2", vERD, "ER_Entity", zPOS_AFTER );

   return( nRC );
} // fnTZERSAPD_CreateSecondRelLink

static zSHORT
fnTZERSAPD_CreateFirstRelLink( zVIEW   vSubtask,
                               zVIEW   vERD,
                               zVIEW   vERD_Copy,
                               zPCHAR  szFK_FieldName,
                               zPCHAR  szCard )
{
   zCHAR   szName[ 33 ];
   zSHORT  nRC;

   // Create second RelLink and include ER_Entity
   CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER );

   // do not generate a Foreign Key Prefix
   SetAttributeFromString( vERD, "ER_RelLink_2", "TE_NoForeignKeyPrefix", "Y" );

   strcpy_s( szName, sizeof( szName ), "has " );
   zstrncat( szName, szFK_FieldName, 28 );
   _strlwr( szName ); // convert to lowercase

   SetAttributeFromString( vERD, "ER_RelLink_2", "Name", szName );

   if ( zstrcmp( szCard, "1" ) == 0 )
   {
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "1" );
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "1" );
   }
   else
   if ( zstrcmp( szCard, "C" ) == 0 )
   {
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "0" );
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "1" );
   }
   else
   if ( zstrcmp( szCard, "N" ) == 0 )
   {
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "1" );
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "m" );
   }
   else
   if ( zstrcmp( szCard, "CN" ) == 0 )
   {
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "0" );
      SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "m" );
   }

   nRC = IncludeSubobjectFromSubobject( vERD, "ER_Entity_2", vERD_Copy, "ER_Entity", zPOS_AFTER );

   return( nRC );
} // fnTZERSAPD_CreateFirstRelLink

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_RemoveAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_RemoveAttribute( zVIEW  vERD,
                            zPCHAR szFK_FieldName )
{
   // If Attribute implements a Relationship, delete it
   if ( SetCursorFirstEntityByString( vERD, "ER_Attribute", "TE_ColumnName", szFK_FieldName, "" ) >= zCURSOR_SET &&
        CompareAttributeToInteger( vERD, "ER_Attribute", "SAP_Attribute", 1 ) == 0 )
   {
      DeleteEntity( vERD, "ER_Attribute", zREPOS_PREV );
   }

   return( 0 );
} // zwTZERSAPD_RemoveAttribute



static zSHORT
fnTZERSAPD_EntityForKeyRS( zVIEW  vSubtask,
                           zPVIEW pvERD,
                           zPCHAR szFK_TableName,
                           zPCHAR szTableName,
                           zPCHAR szFK_FieldName )
{
   zVIEW vTZERSAPO;
   zVIEW vTZSAPERR;
   zCHAR szMsg[ 255 ];
   zCHAR szName[ 33 ];
   zLONG lZKey;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lZKey, *pvERD, "ER_Entity", "ZKey" );

   // Write Warning in Error List Window
   strcpy_s( szName, sizeof( szName ), "has " );
   zstrncat( szName, szFK_FieldName, 28 );
   _strlwr( szName );
   strcpy_s( szMsg, sizeof( szMsg ), "Automatically imported as Target of a Key Relationship '" );
   strcat_s( szMsg, sizeof( szMsg ), szName );
   strcat_s( szMsg, sizeof( szMsg ), "' to Entity '" );
   strcat_s( szMsg, sizeof( szMsg ), szTableName );
   strcat_s( szMsg, sizeof( szMsg ), "'." );
   zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, szMsg, 0 );

   // Create ER Entity
   SetCursorLastEntity( *pvERD, "ER_Entity", "" );
   CreateMetaEntity( vSubtask, *pvERD, "ER_Entity", zPOS_LAST );

   SetAttributeFromString( *pvERD, "ER_Entity", "Name", szFK_TableName );
   SetAttributeFromString( *pvERD, "ER_Entity", "TE_TableName", szFK_TableName );
   SetAttributeFromString( *pvERD, "ER_Entity", "Purpose", "A" );
   SetAttributeFromInteger( *pvERD, "ER_Entity", "SAP_Entity", 1 );

   // set correctly position later (after Import Tables)
   SetAttributeFromInteger( *pvERD, "ER_Entity", "ER_DiagramPosY", -1 );
   SetAttributeFromInteger( *pvERD, "ER_Entity", "ER_DiagramPosX", -1 );

   // Set Entity Position
   zwTZERSAPD_SetEntityPosition( *pvERD, 1 );

   // Create Attributes
   if ( zwTZERSAPD_CreateAttribAndIdent( vSubtask, *pvERD, vTZERSAPO, szFK_TableName ) < 0 )
      return( -1 );

   SetCursorFirstEntityByInteger( *pvERD, "ER_Entity", "ZKey", lZKey, "" );

   return( 0 );
} // fnTZERSAPD_EntityForKeyRS



static zLONG
fnTZERSAPD_CreateERD_Rel( zVIEW   vSubtask,
                          zVIEW   vTZSapRel,
                          zVIEW   vERD,
                          zVIEW   vERD_Copy,
                          zPCHAR  szTableName,
                          zPCHAR  szFK_FieldName,
                          zPCHAR  szFK_Type,
                          zPCHAR  szCard,
                          zPCHAR  szCardLeft,
                          zCHAR   cKey )
{
   zLONG   lRC = 0;

   CreateMetaEntity( vSubtask, vERD, "ER_RelType", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_RelType", "Fact", "Created by SAP Import" );

   // Create First RelLink and include ER_Entity
   fnTZERSAPD_CreateFirstRelLink( vSubtask, vERD, vERD_Copy, szFK_FieldName, szCard );
   SetAttributeFromAttribute( vTZSapRel, "SAP_Rel", "ZKey", vERD, "ER_RelLink_2", "ZKey" );

   // if Relationship a Key, craete new ER_FactType
   if ( cKey == 'X' )
      lRC = zwTZERSAPD_CreateIdentifier( vSubtask, vERD, szTableName, zRelLinkID );

   // Create second RelLink and include ER_Entity
   fnTZERSAPD_CreateSecondRelLink( vSubtask, vERD, szFK_FieldName, szCardLeft );

   return( lRC );
} // fnTZERSAPD_CreateERD_Rel

static zSHORT
fnTZERSAPD_CreateRelationship( zVIEW          vSubtask,
                               zVIEW          vERD,
                               zVIEW          vERD_Copy,
                               zPCHAR         szTableName,
                               zPCHAR         szCountRel,
                               pfnSapGetRel   pfnGetRel,
                               void*          hRelList,
                               zLONG          lIndex )
{
   zVIEW   vTZSapRel;
   zLONG   lRC;
   zCHAR   szFK_FieldName[ 32 ];
   zCHAR   szFK_TableName[ 32 ];
   zCHAR   szFK_Type[ 5 ];
   zCHAR   szCardLeft[ 3 ];
   zCHAR   szCard[ 2 ];
   zCHAR   szDesc[ 61 ];
   zCHAR   cKey;

   GetViewByName( &vTZSapRel, "TZSapRel", vSubtask, zLEVEL_TASK );

   // Load Relationship Datas
   lRC = (*pfnGetRel) ( hRelList, lIndex, szFK_FieldName, szFK_TableName, szFK_Type, szCardLeft, szCard, szDesc, &cKey );
   if ( lRC < 0 )
   {
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, "Relationships not found.", 1 );
      return( -1 );
   }

   // Set DIL Message
   fnTZERSAPD_SAP_RelSetDilMsg( vSubtask, szTableName, szFK_TableName, szCountRel, lIndex );

   // if Relationship a Key Relationship and the related Entity is not in ERD, import the SAP Table automatically
   if ( cKey == 'X'  &&
        SetCursorFirstEntityByString( vERD_Copy, "ER_Entity", "TE_TableName", szFK_TableName, "" ) < zCURSOR_SET )
   {
      if ( fnTZERSAPD_EntityForKeyRS( vSubtask, &vERD, szFK_TableName, szTableName, szFK_FieldName ) < 0 )
         return( -1 );
   }

   lRC = 0;
   // Exists related Entity in Data Model as SAP Entity?
   if ( SetCursorFirstEntityByString( vERD_Copy, "ER_Entity", "TE_TableName", szFK_TableName, "" ) >= zCURSOR_SET &&
        CompareAttributeToInteger( vERD_Copy, "ER_Entity", "SAP_Entity", 1 ) == 0 )
   {
      // save all SAP Relationships in Worklod for Switch FK
      CreateEntity( vTZSapRel, "SAP_Rel", zPOS_LAST );
      SetAttributeFromString( vTZSapRel, "SAP_Rel", "FieldName", szFK_FieldName );
      SetAttributeFromString( vTZSapRel, "SAP_Rel", "FK_Art", szFK_Type );
      SetAttributeFromString( vTZSapRel, "SAP_Rel", "CheckTable", szFK_TableName );

      // If Attribute implements a Relationship, delete it
      zwTZERSAPD_RemoveAttribute( vERD, szFK_FieldName );

      if ( zstrcmp( szFK_Type, "" ) != 0 )
      {
         lRC = fnTZERSAPD_CreateERD_Rel( vSubtask, vTZSapRel, vERD, vERD_Copy, szTableName,
                                         szFK_FieldName, szFK_Type, szCard, szCardLeft, cKey );
      }
   }

   return( (zSHORT) lRC );

} // fnTZERSAPD_CreateRelationship

static zSHORT
fnTZERSAPD_CreateRelatedEntity( zVIEW          vSubtask,
                                zVIEW          vTZERSAPO,
                                zPCHAR         szTableName,
                                zPCHAR         szCountRel,
                                pfnSapGetRel   pfnGetRel,
                                void*          hRelList,
                                zLONG          lIndex )
{
   zVIEW   vTZERSAPO_Copy;
   zLONG   lRC;
   zCHAR   szFK_FieldName[ 32 ];
   zCHAR   szFK_TableName[ 32 ];
   zCHAR   szFK_Type[ 5 ];
   zCHAR   szCardLeft[ 3 ];
   zCHAR   szCard[ 2 ];
   zCHAR   szIndentName[41];
   zCHAR   szWorkName[65];
   zCHAR   szDesc[61];
   zCHAR   cKey;

   // Load Relationship Datas
   lRC = (*pfnGetRel)( hRelList, lIndex, szFK_FieldName, szFK_TableName, szFK_Type, szCardLeft, szCard, szDesc, &cKey );
   if ( lRC < 0 )
   {
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, "Relationships not found.", 1 );
      return( -1 );
   }

   // show related table only one
   strcpy_s( szWorkName, sizeof( szWorkName ), szTableName );
   strcat_s( szWorkName, sizeof( szWorkName ), " " );
   strcat_s( szWorkName, sizeof( szWorkName ), szFK_TableName );

   CreateViewFromViewForTask( &vTZERSAPO_Copy, vTZERSAPO, 0 );

   if ( SetCursorFirstEntityByString( vTZERSAPO_Copy, "Table", "WorkName", szWorkName, "" ) < zCURSOR_SET )
   {
      CreateEntity( vTZERSAPO, "Table", zPOS_AFTER );
      SetAttributeFromString( vTZERSAPO, "Table", "TableName", szFK_TableName );
      SetAttributeFromString( vTZERSAPO, "Table", "ParentName", szTableName );
      SetAttributeFromString( vTZERSAPO, "Table", "WorkName", szWorkName );

      if ( cKey == 'X' )
         strcpy_s( szIndentName, sizeof( szIndentName ), " * " );
      else
         strcpy_s( szIndentName, sizeof( szIndentName ), " " );

      strcat_s( szIndentName, sizeof( szIndentName ), szFK_TableName );
      SetAttributeFromString( vTZERSAPO, "Table", "IndentName", szIndentName );
      SetAttributeFromString( vTZERSAPO, "Table", "Desc", szDesc );
      SetAttributeFromInteger( vTZERSAPO, "Table", "IndentLvl", 1 );
   }

   DropView( vTZERSAPO_Copy );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_CreateRelationship
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateRelationship( zVIEW  vSubtask,
                               zVIEW  vView,   // ERD View or Work-LOD
                               zPCHAR szTableName,
                               zSHORT nCreateRelatedEntity )
{
   zVIEW    vView_Copy;
   void*    hRelList = 0;
   zLONG    lCountRel, k;
   zSHORT   nRC = 0;
   zCHAR    szCountRel[ 10 ];
   zCHAR    szLanguage[ 2 ];
   zCHAR    cLanguage;

   pfnSapGetRelList pfnGetRelList;
   pfnSapGetRel     pfnGetRel;

   // Read Relationship Informations
   pfnGetRelList = (pfnSapGetRelList) SysGetProc( hLibrary, "ZSAP_GetRelationshipList" );
   if ( pfnGetRelList == 0 )
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask, "ZSAP_GetRelationshipList" ) );

   // Set Language Type for Description
   if ( nCreateRelatedEntity == zBuildRelatedEntity )
   {
      GetStringFromAttribute( szLanguage, sizeof( szLanguage ), vView, "SapEnvironment", "LanguageType" );
      cLanguage = szLanguage[ 0 ];
   }
   else
      cLanguage = 0;

   lCountRel = (*pfnGetRelList) ( szTableName, cLanguage, &hRelList );
   if ( lCountRel < 0 )
   {
      zwTZERSAPD_WriteErrorMsg( vSubtask, zBuildERD, "Relationships not found.", 1 );
      return( -1 );
   }

   if ( nCreateRelatedEntity == zBuildRelatedEntity )
      SetAttributeFromInteger( vView, "Table", "LoadRelatedTables", 0 );

   // Read Relationship Item Informations
   pfnGetRel = (pfnSapGetRel) SysGetProc( hLibrary, "ZSAP_GetRelationship" );
   if ( pfnGetRel == 0 )
   {
      fnTZERSAPD_SAP_DropList( vSubtask, hRelList );
      return( fnTZERSAPD_SAP_LoadErrorMsg( vSubtask,  "ZSAP_GetRelationship" ) );
   }

   zltoa( lCountRel, szCountRel, sizeof( szCountRel ) );
   CreateViewFromViewForTask( &vView_Copy, vView, 0 );
   for ( k = 0; k < lCountRel; k++ )
   {
      if ( nCreateRelatedEntity == zBuildRelatedEntity )
         nRC = fnTZERSAPD_CreateRelatedEntity( vSubtask, vView, szTableName, szCountRel, pfnGetRel, hRelList, k );
      else
         nRC = fnTZERSAPD_CreateRelationship( vSubtask, vView, vView_Copy, szTableName, szCountRel, pfnGetRel, hRelList, k );
      if ( nRC < 0 )
      {
         break;
      }
   }

   DropView( vView_Copy );

   // Drop Relationship list
   fnTZERSAPD_SAP_DropList( vSubtask, hRelList );

   return( nRC );
} // zwTZERSAPD_CreateRelationship

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_Build_SAP_RS
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_Build_SAP_RS( zVIEW  vSubtask,
                         zVIEW  vERD,
                         zVIEW  vTZSapRel )
{
   zVIEW    vTZERSASO;
   zSHORT   nRC;
   zSHORT   nError = 0;
   zCHAR    szTableName[ 255 ];
   zBOOL    bSA_Exists = FALSE;

   // Create Relationship from SAP
   for ( nRC = SetCursorFirstEntityByInteger( vERD, "ER_Entity", "SAP_Entity", 1, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByInteger( vERD, "ER_Entity", "SAP_Entity", 1, "" ) )
   {
      GetStringFromAttribute( szTableName, sizeof( szTableName ), vERD, "ER_Entity", "TE_TableName" );

      // Create Entity in Worklod to be save all SAP Relationships for Switch FK
      CreateEntity( vTZSapRel, "SAP_Enitiy", zPOS_LAST );
      SetAttributeFromString( vTZSapRel, "SAP_Enitiy", "TE_TableName", szTableName );

      if ( zwTZERSAPD_CreateRelationship( vSubtask, vERD, szTableName, zBuildRelationship ) < 0 )
      {
         nError = 1;
         break;
      }
   }

   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) >= 0 )
      bSA_Exists = TRUE;

   // Go back and complete the relationship path under each ER_RelLink entity.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink", "EntpER_Model" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_RelLink", "EntpER_Model" ) )
   {
      if ( CheckExistenceOfEntity( vERD, "ER_RelLink_Other" ) < zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vERD, "ER_RelLink_2", "ZKey", vERD, "ER_RelLink", "ZKey", "EntpER_Model" );
         // Set cursor to the other side of the relationship.
         if ( SetCursorNextEntity( vERD, "ER_RelLink_2", "" ) < zCURSOR_SET )
            SetCursorPrevEntity( vERD, "ER_RelLink_2", "" );

         IncludeSubobjectFromSubobject( vERD, "ER_RelLink_Other", vERD, "ER_RelLink_2", zPOS_AFTER );

         // if Subject Area open, add RelLink to Subject Area
         if ( bSA_Exists )
            zwTZEREMDD_AddRelLinkToSA( vSubtask, vERD, vTZERSASO );
      }
   }

   return( nError );
} // zwTZERSAPD_Build_SAP_RS

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_UpdateDataSource
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_UpdateDataSource( zVIEW  vSubtask,
                             zVIEW  vDTE,
                             zVIEW  vTZTEDBLO )
{
   zSHORT  nRC;

   SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "SironDB_Type", "S", "" );

   // Delete all SAP Data Sources
   for ( nRC = SetCursorFirstEntityByString( vDTE, "TE_DBMS_Source", "SironDB_Type", "S", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByString( vDTE, "TE_DBMS_Source", "SironDB_Type", "S", "" ) )
   {
      DeleteEntity( vDTE, "TE_DBMS_Source", zREPOS_NONE );
   }

   CreateTemporalMetaEntity( vSubtask, vDTE, "TE_DBMS_Source", zPOS_LAST );

   SetAttributeFromString( vDTE, "TE_DBMS_Source", "Name", "SAP R3" );
   SetAttributeFromString( vDTE, "TE_DBMS_Source", "Desc", "Created by SAP Import" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Paradigm", vTZTEDBLO, "TE_DBMS_Source", "Paradigm" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DBMS", vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "GenerateExecutable", vTZTEDBLO, "TE_DBMS_Source", "GenerateExecutable" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "ConectionType", vTZTEDBLO, "TE_DBMS_Source", "ConectionType" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "SironDB_Type", vTZTEDBLO, "TE_DBMS_Source", "SironDB_Type" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "SironDB_Subtype", vTZTEDBLO, "TE_DBMS_Source", "SironDB_Subtype" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Executable", vTZTEDBLO, "TE_DBMS_Source", "Executable" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DisplayOwner", vTZTEDBLO, "TE_DBMS_Source", "DisplayOwner" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DisplayDBMS_ID", vTZTEDBLO, "TE_DBMS_Source", "DisplayDBMS_ID" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Network", vTZTEDBLO, "TE_DBMS_Source", "Network" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "GenKeyExecutable", vTZTEDBLO, "TE_DBMS_Source", "GenKeyExecutable" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "MaxFixedAttrLth", vTZTEDBLO, "TE_DBMS_Source", "MaxFixedAttrLth" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "OverflowAttrLth", vTZTEDBLO, "TE_DBMS_Source", "OverflowAttrLth" );

   return( 0 );
} // zwTZERSAPD_UpdateDataSource

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SaveTE_ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SaveTE_ERD( zVIEW  vSubtask,
                       zVIEW  vDTE,
                       zVIEW  vERD )
{
   zVIEW   vCM_List;
   zVIEW   vTaskLPLR;
   zSHORT  nRC;
   zCHAR   szName[ 33 ];
   zCHAR   szFileName[ zMAX_FILENAME_LTH ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zSOURCE_DTE_META );

   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   ofnTZCMWKSO_AppendSlash( szFileName );
   GetStringFromAttribute( szName, sizeof( szName ), vCM_List, "W_MetaDef", "Name" );
   strcat_s( szFileName, sizeof( szFileName ), szName );
   strcat_s( szFileName, sizeof( szFileName ), ".dte" );

   nRC = CommitOI_ToFile( vDTE, szFileName, zSINGLE );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "ER00107", "SAP Import",
                   "Unable to save Physical Environment.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   {
      nRC = CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META );
      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "ER00107", "SAP Import",
                      "Unable to save Data Model.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
   }

   return( nRC );
} // zwTZERSAPD_SaveTE_ERD


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_LoadTE
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_LoadTE( zVIEW  vSubtask,
                   zPVIEW pvDTE )
{
   zVIEW  vDTE;
   zVIEW  vTaskLPLR;
   zVIEW  vCM_List;
   zSHORT nRC;
   zCHAR  szName[ 33 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH ];

   *pvDTE = 0;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zSOURCE_DTE_META );

   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) < zCURSOR_SET )
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DTE_META );

   // TE does not exists, create it
   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) < zCURSOR_SET )
   {
      GetStringFromAttribute( szName, sizeof( szName ), vTaskLPLR, "LPLR", "Name" );
      ActivateEmptyMetaOI( vSubtask, &vDTE, zSOURCE_DTE_META, zSINGLE | zLEVEL_APPLICATION );
      if ( !vDTE )
         return( -2 );

      CreateMetaEntity( vSubtask, vDTE, "TE_DB_Environ", zPOS_AFTER );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Name", szName );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Desc", "Create By SAP Import" );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Type", "P" );

      nRC = CommitMetaOI( vSubtask, vDTE, zSOURCE_DTE_META );
      DropMetaOI( vSubtask, vDTE );
      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "ER00107", "SAP Import",
                      "Unable to save Physical Environment.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( nRC );
      }
   }

   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   ofnTZCMWKSO_AppendSlash( szFileName );
   GetStringFromAttribute( szName, sizeof( szName ), vCM_List, "W_MetaDef", "Name" );
   strcat_s( szFileName, sizeof( szFileName ), szName );
   strcat_s( szFileName, sizeof( szFileName ), ".dte" );

   nRC = ActivateOI_FromFile( &vDTE, "TZTENVRO", vSubtask, szFileName, zSINGLE | zIGNORE_ATTRIB_ERRORS );
   if ( nRC < 0 )
      return( -2 );

   *pvDTE = vDTE;

   return( 0 );
} // zwTZERSAPD_LoadTE


// vDTE is pointing to the TE_FieldDataRel which is the foreign key being switched off (old).
// vDTE_Copy1 is pointing to a TE_FieldDataRel which is the target of the switch (new).
// It is implemented as an ER attribute.
static zSHORT
fnTZERSAPD_SwitchForeignToERAttr( zVIEW  vDTE,
                                  zVIEW  vDTE_SwitchTo )
{
   // the FK to be switched away should NOT be part of the key. we do not check that here.

   // As the Target is an ER attribute, we must provide a TE_FieldDataRel_PK.
   //  That should be the Base Key of vDTE.
   SetBaseKeyAs_PK( vDTE_SwitchTo, vDTE );

   if ( SetCursorFirstEntityByAttr( vDTE_SwitchTo, "ER_RelLink", "ZKey", vDTE, "ER_RelLink", "ZKey", "" ) < zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vDTE_SwitchTo, "ER_RelLink", vDTE, "ER_RelLink", zPOS_AFTER );
   }

   ExcludeEntity( vDTE, "ER_RelLink", zREPOS_NONE );

   // set new field sequence
   oTZTENVRO_DelFieldFromSequence( vDTE );

   // Delete the original foreign key.
   DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );

   return( 0 );

} // fnTZERSAPD_SwitchForeignToERAttr



// vDTE_ERA is pointing to a an ER attribute (TE_FileDataRel) which is to be turned to a foreign key.
// It should have the Base Key of the current FK (pointed to by vDTE_FK ).
// Include the ER attr as FK under the "Base Key".
static void
SetBaseKeyAs_PK( zVIEW vDTE_ERA, zVIEW vDTE_FK )
{
   zVIEW  vDTE2;
   zLONG  lZKey, lCurrentZKey, lBaseZKey;
   zSHORT nRC;

   CreateViewFromViewForTask( &vDTE2, vDTE_FK, 0 );

   // get the ZKey of current FK.
   GetIntegerFromAttribute( &lCurrentZKey, vDTE2, "TE_FieldDataRel", "ZKey" );
   // let vDTE2 point to the Base Key TE_FieldDataRel
   GetIntegerFromAttribute( &lZKey, vDTE2, "TE_FieldDataRel_PK", "ZKey" );
   fnTZERSAPD_FK_SearchBaseKey( vDTE2, lZKey );

   // get the ZKey of the Base Key
   GetIntegerFromAttribute( &lBaseZKey, vDTE2, "TE_FieldDataRel", "ZKey" );
   // Exclude the current FK, wherever it occurs as FK under a PK
   nRC = SetCursorFirstEntityByInteger( vDTE2, "TE_FieldDataRelAsFK", "ZKey", lCurrentZKey, "TE_DBMS_Source" );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vDTE2, "TE_FieldDataRelAsFK", zREPOS_NONE );
      nRC = SetCursorNextEntityByInteger( vDTE2, "TE_FieldDataRelAsFK", "ZKey", lCurrentZKey, "TE_DBMS_Source" );
   }
   // include the ER attr as FK under the Base Key
   nRC = SetCursorFirstEntityByInteger( vDTE2, "TE_FieldDataRelKey", "ZKey", lBaseZKey, "TE_DBMS_Source" );
   // is it possible that the FK is already here?
   //  probably not, but to be sure ...
   nRC = SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRelAsFK", vDTE_ERA, "TE_FieldDataRel", 0 );
   if ( nRC < zCURSOR_SET )
   {
      // not there, include it
      nRC = SetCursorLastEntity( vDTE2, "TE_FieldDataRelAsFK", 0 );
      IncludeSubobjectFromSubobject( vDTE2, "TE_FieldDataRelAsFK", vDTE_ERA, "TE_FieldDataRel", zPOS_AFTER );
   }

   DropView( vDTE2 );
} // SetBaseKeyAs_PK


// lCurrentZKey references the TE_FieldDataRel_PK for FK
static zBOOL
fnTZERSAPD_FK_HasReferToER_Attr( zVIEW  vDTE,
                                 zVIEW  vTZSapRel,
                                 zLONG  lCurrentZKey )
{
   zVIEW  vDTE_ER;
   zSHORT nRC;
   zBOOL  bHasReferToER_Attr = FALSE;

   // If the Foreign Key has a direct reference to an ER_Attribute (the
   // TE_FieldDataRel_PK is an ER attribute), then we get the foreign key's
   // original name from the SAP relationship work object( FieldName ).

   CreateViewFromViewForTask( &vDTE_ER, vDTE, 0 );

   nRC = SetCursorFirstEntityByInteger( vDTE_ER, "TE_FieldDataRel", "ZKey", lCurrentZKey, "TE_DBMS_Source" );

   if ( nRC >= zCURSOR_SET &&
        CheckExistenceOfEntity( vDTE_ER, "ER_Attribute" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByAttr( vTZSapRel, "SAP_Enitiy", "TE_TableName", vDTE, "TE_TablRec", "Name", "" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByAttr( vTZSapRel, "SAP_Rel", "ZKey", vDTE, "ER_RelLink", "ZKey", "" ) >= zCURSOR_SET )

   {
      SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Name", vTZSapRel, "SAP_Rel", "FieldName" );
      bHasReferToER_Attr = TRUE;
   }

   DropView( vDTE_ER );

   return( bHasReferToER_Attr );
} // fnTZERSAPD_FK_HasReferToER_Attr



static zSHORT
fnTZERSAPD_CheckExistsOfName( zVIEW  vDTE,
                              zVIEW  vDTE_FieldDataRel,
                              zVIEW  vTZSapRel,
                              zPCHAR szFieldName )
{
   zSHORT  nRC;

   // check, whether a TE_FieldDataRel with the name given already exists.
   nRC = SetCursorFirstEntityByString( vDTE_FieldDataRel, "TE_FieldDataRel", "Name", szFieldName, "" );
   if ( nRC >= zCURSOR_SET &&
        CompareAttributeToAttribute( vDTE_FieldDataRel, "TE_FieldDataRel", "ZKey", vDTE, "TE_FieldDataRel", "ZKey" ) == 0 )
   {
      nRC = SetCursorNextEntityByString( vDTE_FieldDataRel, "TE_FieldDataRel", "Name", szFieldName, "" );
   }

   return( nRC );
} // fnTZERSAPD_CheckExistsOfName



static zBOOL
fnTZERSAPD_FK_HasBlankRS( zVIEW  vDTE,
                          zVIEW  vTZSapRel,
                          zPCHAR szCheckTable )
{
   zVIEW   vDTE_FieldDataRel;
   zSHORT  nRC;
   zCHAR   szFieldName[ 33 ];
   zBOOL   bSwitchFK = TRUE;

   // If a relationship is a "blank" relationship, then we just want to change
   //  the name of the FK (TE_FieldDataRel). The new name is gotten from
   //  the SAP relationship work object, unless there is already a
   //  TE_FieldDataRel with this name.

   // a "blank" relationship is used to indicate, that a foreign key was
   //  inherited from an entity, where no direct relationship exists.

   CreateViewFromViewForTask( &vDTE_FieldDataRel, vDTE, 0 );
   nRC = SetCursorFirstEntityByString( vTZSapRel, "SAP_Rel", "CheckTable",
                                       szCheckTable, "" );
   while ( bSwitchFK && nRC >= zCURSOR_SET )
   {
      // Test, ob Blank Relationship vorliegt
      if ( CompareAttributeToString( vTZSapRel, "SAP_Rel", "FK_Art", "" ) == 0 )
      {
         // check, whether TE_FieldDataRel with this name already exists.
         GetStringFromAttribute( szFieldName, sizeof( szFieldName ), vTZSapRel, "SAP_Rel", "FieldName" );
         nRC = fnTZERSAPD_CheckExistsOfName( vDTE, vDTE_FieldDataRel, vTZSapRel, szFieldName );
         if ( nRC < zCURSOR_SET )
         {
            SetAttributeFromString( vDTE, "TE_FieldDataRel", "Name", szFieldName );
            bSwitchFK = FALSE;
         }
         else // Relationship is NOT a "Blank" Relationship, switch Foreign Key
         if ( nRC >= zCURSOR_SET )
         {
            oTZTENVRO_SwitchForeignKeys( vDTE, vDTE_FieldDataRel );
            bSwitchFK = FALSE;
         }
      } // endif
      nRC = SetCursorNextEntityByString( vTZSapRel, "SAP_Rel", "CheckTable", szCheckTable, "" );
   } // endwhile ( nRC >= zCURSOR_SET )

   // if we found the ZKey of ER_RelLink in the SAP Relationship Work Object,
   // then just set the name for TE_FieldDataRel.
   if ( bSwitchFK &&
        SetCursorFirstEntityByAttr( vTZSapRel, "SAP_Rel", "ZKey", vDTE, "ER_RelLink", "ZKey", "" ) >= zCURSOR_SET )
   {
      // check, whether TE_FieldDataRel with this name already exists.
      GetStringFromAttribute( szFieldName, sizeof( szFieldName ), vTZSapRel, "SAP_Rel", "FieldName" );
      nRC = fnTZERSAPD_CheckExistsOfName( vDTE, vDTE_FieldDataRel, vTZSapRel, szFieldName );
      if ( nRC < zCURSOR_SET )
      {
         SetAttributeFromString( vDTE, "TE_FieldDataRel", "Name", szFieldName );
         bSwitchFK = FALSE;
      }
   }
   DropView( vDTE_FieldDataRel );

   return( bSwitchFK );
} // fnTZERSAPD_FK_HasBlankRS


// the vDTE_Copy is positioned on the TE_FielDataRel, which is the Base Key
//  for a given Foreign Key (Input lZKey).
static zSHORT
fnTZERSAPD_FK_SearchBaseKey( zVIEW  vDTE_BaseKey,
                             zLONG  lZKey )
{
   zSHORT  nRC;

   nRC = SetCursorFirstEntityByInteger( vDTE_BaseKey, "TE_FieldDataRel", "ZKey", lZKey, "TE_DBMS_Source" );

   while ( nRC >= zCURSOR_SET &&
           CompareAttributeToString( vDTE_BaseKey, "TE_FieldDataRel", "DataOrRelfieldOrSet", "D" ) != 0 )
   {
      GetIntegerFromAttribute( &lZKey, vDTE_BaseKey, "TE_FieldDataRel_PK", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vDTE_BaseKey, "TE_FieldDataRel", "ZKey", lZKey, "TE_DBMS_Source" );
   }

   return( 0 );
} // fnTZERSAPD_FK_SearchBaseKey


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SwitchForeignKeys
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_SwitchForeignKeys( zVIEW  vDTE,
                              zVIEW  vTZSapRel )
{
   zVIEW   vDTE_BaseKey;
   zVIEW   vDTE_SwitchTo;
   zLONG   lZKey;
   zLONG   lBaseZKey;
   zBOOL   bSwitchFK;
   zSHORT  nRC;
   zCHAR   szCheckTable[ 33 ];
   zCHAR   szMsg[150];
   zCHAR   szBaseKeyName[ 33 ];

   CreateViewFromViewForTask( &vDTE_BaseKey, vDTE, 0 );

   for ( nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRel_PK", "TE_DBMS_Source" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDTE, "TE_FieldDataRel_PK", "TE_DBMS_Source" ) )
   {
      bSwitchFK = TRUE;
      GetIntegerFromAttribute( &lZKey, vDTE, "TE_FieldDataRel_PK", "ZKey" );

      // Foreign Key directy references an ER_Attribute
      if ( fnTZERSAPD_FK_HasReferToER_Attr( vDTE, vTZSapRel, lZKey ) )
         continue;

      // Foreign Key references another Foreign Key
      // Search the Key (BaseKey) referencing an ER_Attribute and NOT another
      //  Foreign Key (DataOrRelfieldOrSet = D )
      fnTZERSAPD_FK_SearchBaseKey( vDTE_BaseKey, lZKey );

      // check, whether Foreign Key with ER_Attribute exists
      if ( CheckExistenceOfEntity( vDTE_BaseKey, "ER_Attribute" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lBaseZKey, vDTE_BaseKey, "TE_FieldDataRel", "ZKey" );
         GetStringFromAttribute( szCheckTable, sizeof( szCheckTable ), vDTE_BaseKey, "TE_TablRec", "Name" );
         GetStringFromAttribute( szBaseKeyName, sizeof( szBaseKeyName ), vDTE_BaseKey, "TE_FieldDataRel", "Name" );

         // Position Root of Relationship-Workobject to the current table.
         if ( SetCursorFirstEntityByAttr( vTZSapRel, "SAP_Enitiy", "TE_TableName", vDTE, "TE_TablRec", "Name", "" ) >= zCURSOR_SET )
         {
            // if Relationship is a "Blank" Relationship, only set the correct Name for
            //  the current TE_FieldDataRel. If the TE_FieldDataRel could not be
            //  renamed, the the function returns TRUE, to indicate, that the
            //  Foreign key must be "switched" to another TE_FieldDataRel.
            bSwitchFK = fnTZERSAPD_FK_HasBlankRS( vDTE, vTZSapRel, szCheckTable );
         }

         // Switch Foreign Key
         if ( bSwitchFK )
         {
            CreateViewFromViewForTask( &vDTE_SwitchTo, vDTE, 0 );
            if ( SetCursorFirstEntityByInteger( vDTE_SwitchTo, "TE_FieldDataRel_PK", "ZKey", lBaseZKey, "TE_TablRec" ) >= zCURSOR_SET )
            {
               oTZTENVRO_SwitchForeignKeys( vDTE, vDTE_SwitchTo );
            }
            else
            {
               // we did not find a candidate foreign key for the switch,
               //  which (normally) is an error situation.
               // However, if we find an ER attribute with the same name (of the
               //  base key), then we want to use the ER attribute to implement the
               //  relationship. This is a violation of the Zeidon paradigm.
               //  Nevertheless, the SAP data model is not correct with respect
               //  of T000, MANDT. We do not ave another choice.
               //
               // get the name of the base key
               if ( SetCursorFirstEntityByString( vDTE_SwitchTo, "TE_FieldDataRel", "Name",
                       szBaseKeyName, "TE_TablRec" ) >= zCURSOR_SET )
               {
                  fnTZERSAPD_SwitchForeignToERAttr( vDTE, vDTE_SwitchTo );
                  strcpy_s( szMsg, sizeof( szMsg ), "ER Attribute '" );
                  strcat_s( szMsg, sizeof( szMsg ), szBaseKeyName );
                  strcat_s( szMsg, sizeof( szMsg ), "' used as Foreign Key." );
                  zwTZERSAPD_WriteErrorMsg( vDTE, zBuildTE, szMsg, 0 );
               }
               else
               {
                  // now, we see no more way to get around the error.
                  GetStringFromAttribute( szCheckTable, sizeof( szCheckTable ), vDTE, "ER_RelLink", "Name" );
                  strcpy_s( szMsg, sizeof( szMsg ), "Internal TE build error. Missing Foreign Key for ");
                  strcat_s( szMsg, sizeof( szMsg ), "Relationship '" );
                  strcat_s( szMsg, sizeof( szMsg ), szCheckTable );
                  strcat_s( szMsg, sizeof( szMsg ), "'." );
                  zwTZERSAPD_WriteErrorMsg( vDTE, zBuildTE, szMsg, 1 );
                  DropView( vDTE_SwitchTo );
                  DropView( vDTE_BaseKey );
                  return( -1 );
               }
            }
            DropView( vDTE_SwitchTo );
         }

      } // endif ( CheckExistenceOfEntity( vDTE_BaseKey, "ER_Attribute" ) >= zCURSOR_SET )
   }  // endfor ( nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRel_PK"...

   DropView( vDTE_BaseKey );

   return( 0 );
} // zwTZERSAPD_SwitchForeignKeys



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_DropTE_ErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_DropTE_ErrorList( zVIEW vSubtask )
{
   zVIEW  vTZTEERR;
   zVIEW  vTZSAPERR;
   zVIEW  vWindow;
   zCHAR  szMsg[ 255 ];
   zCHAR  szMsgText[ 255 ];
   zSHORT nRC;

   if ( GetViewByName( &vTZTEERR, "TZTEERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( CheckExistenceOfEntity( vTZTEERR, "ErrorMsg" ) >= zCURSOR_SET )
      {
         if ( GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK ) < 0 )
         {
            zwTZERSAPD_LoadErrorList( vSubtask );
            GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );
         }

         for ( nRC = SetCursorFirstEntity( vTZTEERR, "ErrorMsg", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZTEERR, "ErrorMsg", "" ) )
         {
            CreateEntity( vTZSAPERR, "ErrorMsg", zPOS_LAST );
            strcpy_s( szMsgText, sizeof( szMsgText ), "Error: " );
            GetStringFromAttribute( szMsg, sizeof( szMsg ), vTZTEERR, "ErrorMsg", "MessageText" );
            strcat_s( szMsgText, sizeof( szMsgText ), szMsg );
            SetAttributeFromString( vTZSAPERR, "ErrorMsg", "ErrorText", szMsgText );
            SetAttributeFromAttribute( vTZSAPERR, "ErrorMsg", "EntityName", vTZTEERR, "ErrorMsg", "EntityName" );
            SetAttributeFromInteger( vTZSAPERR, "ErrorMsg", "Error", 1 );
         }

         if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 &&
              CompareAttributeToString( vTZSAPERR, "Import", "ShowErrorList", "Y" ) == 0 )
         {
            zwTZERSAPD_RefreshErrorList( vWindow );
         }
      } // endif ( CheckExistenceOfEntity( vTZTEERR, "ErrorMsg" ) ...

      DropObjectInstance( vTZTEERR );
   }

   return( 0 );
} // zwTZERSAPD_DropTE_ErrorList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ExistTE_ErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ExistTE_ErrorList( zVIEW vSubtask )
{
   zVIEW   vTZTEERR;

   if ( GetViewByName( &vTZTEERR, "TZTEERR", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZTEERR, "ErrorMsg" ) >= zCURSOR_SET )
   {
      zwTZERSAPD_DropTE_ErrorList( vSubtask );
      return( 1 );
   }

   return( 0 );
} // zwTZERSAPD_ExistTE_ErrorList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_Build_SAP_TE
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_Build_SAP_TE( zVIEW  vSubtask,
                         zVIEW  vERD,
                         zVIEW  vTZSapRel )
{
   zVIEW   vDTE = 0;
   zVIEW   vTZTEDBLO;
   zSHORT  nRC, nTE_Error;

   // Load TE, if TE does not exists, create it
   nRC = zwTZERSAPD_LoadTE( vSubtask, &vDTE );
   if ( vDTE == 0 )
   {
      SysMessageBox( vSubtask, "SAP Import", "Can't load TE.", 1 );
      return( nRC );
   }

   SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   // Load List of supported databases/db-handlers
   ActivateOI_FromFile( &vTZTEDBLO, "TZTEDBLO", vSubtask, "tztedblo.xdl", zMULTIPLE );
   SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   // Create a new SAP Data Source, exists a SAP Data Source delete it
   zwTZERSAPD_UpdateDataSource( vSubtask, vDTE, vTZTEDBLO );

   // Build TE for SAP Data Source
   nRC = oTZTENVRO_BuildTablRecsFromEMD( vSubtask, vDTE, vERD, "TE_DBMS_Source" );
   nTE_Error = zwTZERSAPD_ExistTE_ErrorList( vSubtask );

   if ( nRC >= 0 && nTE_Error == 0 )
   {
      nRC = oTZTENVRO_BuildRelationsFromEMD( vSubtask, vDTE, vERD );
      nTE_Error = zwTZERSAPD_ExistTE_ErrorList( vSubtask );

      if ( nRC >= 0 && nTE_Error == 0 )
      {
         // switch Foreign Keys
         nRC = zwTZERSAPD_SwitchForeignKeys( vDTE, vTZSapRel );
      }
   }

   // Save TE and Data Model
   if ( nRC >= 0 && nTE_Error == 0 )
      nRC = zwTZERSAPD_SaveTE_ERD( vSubtask, vDTE, vERD );

   zwTZERSAPD_DropTE_ErrorList( vSubtask );
   DropView( vTZTEDBLO );
   DropView( vDTE );

   return( nRC );
} // zwTZERSAPD_Build_SAP_TE

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_LoadRelShipWorkLod
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_LoadRelShipWorkLod( zVIEW   vSubtask,
                               zPVIEW  pvTZSapRel )
{

   if ( GetViewByName( pvTZSapRel, "TZSapRel", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( *pvTZSapRel );

   // Create Work LOD to be saved all SAP Relationships
   ActivateEmptyObjectInstance( pvTZSapRel, "TZSAPRS", vSubtask, zMULTIPLE );
   SetNameForView( *pvTZSapRel, "TZSapRel", vSubtask, zLEVEL_TASK );

   return( 0 );
} // zwTZERSAPD_LoadRelShipWorkLod



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_Build_SAP_RS_TE
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_Build_SAP_RS_TE( zVIEW  vSubtask,
                            zVIEW  vERD )
{
   zVIEW    vTZSAPERR;
   zVIEW    vTZSapRel = 0;
   zSHORT   nRC;

   // delete all Relationships between SAP Entity only
   MB_SetMessage( vSubtask, 0, "Delete all SAP Relationships" );
   zwTZERSAPD_DeleteAll_SAP_Rels( vERD );

   // Create Worklod to be save all SAP Relationships
   zwTZERSAPD_LoadRelShipWorkLod( vSubtask, &vTZSapRel );

   // Create Relationship from SAP
   MB_SetMessage( vSubtask, 0, "Build Relationships" );
   nRC = zwTZERSAPD_Build_SAP_RS( vSubtask, vERD, vTZSapRel );

   if ( nRC >= 0 )
   {
      // Create TE
      MB_SetMessage( vSubtask, 0, "Build TE" );
      nRC = zwTZERSAPD_Build_SAP_TE( vSubtask, vERD, vTZSapRel );
   }

   // if Error in Error List
   if ( nRC >= 0 &&
        GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK ) >= 0 &&
        SetCursorFirstEntityByInteger( vTZSAPERR, "ErrorMsg", "Error", 1, "" ) >= zCURSOR_SET )
   {
      nRC = -1;
   }

   return( nRC );
} // zwTZERSAPD_Build_SAP_RS_TE


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_BuildRelationships_TE
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_BuildRelationships_TE( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vTZERSAPO;
   zVIEW  vTZSAPERR;
   zSHORT nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "ImportFlags",
                           zBuild_RS_TE );

   if ( zwTZERSAPD_CheckValues( vSubtask, vTZERSAPO ) < 0 )
      return( -1 );

   // added search string to Error List and load Error List View
   zwTZERSAPD_CreateNewErrorRoot( vSubtask );
   GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );

   if ( zwTZERSAPD_Build_AskForSave( vSubtask, vTZERSAPO ) < 0 )
   {
      // Refresh Erorr List Window and ERD
      zwTZERSAPD_RefreshAllWindows( vSubtask, vTZERSAPO, vTZSAPERR, zAbortBuild );
      return( 0 );
   }

   // new SAP Logon with other datas?
   nRC = fnTZERSAPD_CheckNewLogon( vSubtask, vTZERSAPO );
   if ( nRC < 0 )
      return( nRC );

   MB_SetMessage( vSubtask, 0, "Build Relationships" );

   // Build Relationships and TE
   nRC = zwTZERSAPD_Build_SAP_RS_TE( vSubtask, vERD );
   if ( nRC < 0 )
      fnTZERSAPD_SendMessageBox( vSubtask, zBuild_RS_TE, "with Errors", zICON_ERROR );
   else
      fnTZERSAPD_SendMessageBox( vSubtask, zBuild_RS_TE, "successfully", zICON_INFORMATION );

   // Refresh Erorr List Window and ERD
   zwTZERSAPD_RefreshAllWindows( vSubtask, vTZERSAPO, vTZSAPERR, nRC );

   return( 0 );
} // zwTZERSAPD_BuildRelationships_TE


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ImportAllTables
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ImportAllTables( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zVIEW  vWindow;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZEREMDD" ) >= 0 )
      MapWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" ) >= 0 )
      MapWindow( vWindow );

   // if Build Relationship and TE only
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_RS_TE ) == 0 )
   {
      zwTZERSAPD_BuildRelationships_TE( vSubtask );
      return( 0 );
   }

   // if Table exist?
   if ( SetCursorFirstEntity( vTZERSAPO, "Table", "" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "OP00819", "SAP Import",
                   "No Tables available for Import.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   if ( zwTZERSAPD_Import_SAP_Tables( vSubtask, vTZERSAPO, zImportAllTables ) < 0 )
      return( -1 );

   return( 0 );
} // zwTZERSAPD_ImportAllTables



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ImportTables
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ImportTables( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zVIEW  vWindow;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZEREMDD" ) >= 0 )
      MapWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" ) >= 0 )
      MapWindow( vWindow );

   // if Build Relationship and TE
   if ( CompareAttributeToString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_RS_TE ) == 0 )
   {
      zwTZERSAPD_BuildRelationships_TE( vSubtask );
      return( 0 );
   }

   // if Table selected?
   if ( SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "OP00819", "SAP Import", "No Table selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   if ( zwTZERSAPD_Import_SAP_Tables( vSubtask, vTZERSAPO, zImportSelectedTables ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
} // zwTZERSAPD_ImportTables


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetCtrlStateForImport
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetCtrlStateForImport( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vERD_Copy;
   zSHORT nEnable = 1;

   // if 3. Tab active, set Control State from Import Options
   if ( SetFocusToCtrl( vSubtask, "rbGerman" ) >= 0 )
   {
      if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      {
         nEnable = 0;
      }
      else
      {
         CreateViewFromViewForTask( &vERD_Copy, vERD, 0 );
         if ( SetCursorFirstEntityByInteger( vERD_Copy, "ER_Entity", "SAP_Entity", 1, "" ) < zCURSOR_SET )
         {
            nEnable = 0;
         }
         DropView( vERD_Copy );
      }

      SetCtrlState( vSubtask, "rbBuildRelsAndTE", zCONTROL_STATUS_ENABLED, nEnable );
   }

   zwTZERSAPD_SetTabSize( vSubtask );

   return( 0 );
} // zwTZERSAPD_SetCtrlStateForImport



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetCountError
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetCountError( zVIEW vSubtask )
{
   zVIEW     vTZSAPERR;
   zLONG     lCountError;
   zLONG     lCountWarning;
   zSHORT    nRC;
   zCHAR     szCount[ 10 ];
   zCHAR     szControlText[ 100 ];

   GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZSAPERR, "Import" ) >= zCURSOR_SET )
   {
      // Set Count Errors to Textfield
      lCountError   = 0;
      lCountWarning = 0;
      for ( nRC = SetCursorFirstEntity( vTZSAPERR, "ErrorMsg", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZSAPERR, "ErrorMsg", "" ) )
      {
         if ( CompareAttributeToInteger( vTZSAPERR, "ErrorMsg", "Error", 1 ) == 0 )
            lCountError++;
         else
            lCountWarning++;
      }

      if ( lCountWarning > 0 )
         lCountWarning--;

      zltoa( lCountError, szCount, sizeof( szCount ) );
      strcpy_s( szControlText, sizeof( szControlText ), " " );
      strcat_s( szControlText, sizeof( szControlText ), szCount );
      strcat_s( szControlText, sizeof( szControlText ), " Error(s) " );

      if ( lCountWarning > 0 )
      {
         zltoa( lCountWarning, szCount, sizeof( szCount ) );
         strcat_s( szControlText, sizeof( szControlText ), ", " );
         strcat_s( szControlText, sizeof( szControlText ), szCount );
         strcat_s( szControlText, sizeof( szControlText ), " Warning(s) " );
      }

      strcat_s( szControlText, sizeof( szControlText ), "found" );
   }
   else
   {
      strcpy_s( szControlText, sizeof( szControlText ), " No Errors" );
   }

   SetCtrlText( vSubtask, "txtCountError", szControlText );

   return( 0 );
} // zwTZERSAPD_SetCountError



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_LoadErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_LoadErrorList( zVIEW vSubtask )
{
   zVIEW  vTZSAPERR;

   if ( GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // Create Work LOD for Error List
      ActivateEmptyObjectInstance( &vTZSAPERR, "TZSAPERR", vSubtask, zMULTIPLE );
      SetNameForView( vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );
   }

   zwTZERSAPD_SetCountError( vSubtask );

   return( 0 );
} // zwTZERSAPD_LoadErrorList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPS_CreateNewErrorRoot
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_CreateNewErrorRoot( zVIEW vSubtask )
{
   zVIEW  vTZSAPERR;
   zVIEW  vTZERSAPO;
   zVIEW  vWindow;
   zCHAR  szCurrentDateTime[ 20 ];

   if ( GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZERSAPD_LoadErrorList( vSubtask );
      GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );
   }

  SysGetDateTime( szCurrentDateTime, sizeof( szCurrentDateTime ) );
   CreateEntity( vTZSAPERR, "Import", zPOS_LAST );
   SetAttributeFromString( vTZSAPERR, "Import", "SearchDateTime", szCurrentDateTime );
   SetAttributeFromAttribute( vTZSAPERR, "Import", "ShowErrorList", vTZERSAPO, "SapEnvironment", "ShowErrorList" );

   if ( CompareAttributeToString( vTZSAPERR, "Import", "ShowErrorList", "Y" ) == 0 )
   {
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ProcessImmediateAction | zWAB_StartOrFocusModelessWindow, "TZERSAPD", "ErrorList" );
         GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" );
      }

      zwTZERSAPD_RefreshErrorList( vWindow );
   }

   return( 0 );
} // zwTZERSAPS_CreateNewErrorRoot


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_WriteErrorMsg
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_WriteErrorMsg( zVIEW  vSubtask,
                          zSHORT nBuildERDorTE,
                          zPCHAR szErrorMsg,
                          zSHORT nError )
{
   zVIEW  vTZSAPERR;
   zVIEW  vERD;
   zVIEW  vDTE;
   zVIEW  vWindow;
   zCHAR  szMsg[ 255 ];

   if ( GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZERSAPD_LoadErrorList( vSubtask );
      GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );
   }

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateEntity( vTZSAPERR, "ErrorMsg", zPOS_LAST );

   if ( nBuildERDorTE == zBuildERD )
   {
      SetAttributeFromAttribute( vTZSAPERR, "ErrorMsg", "EntityName", vERD, "ER_Entity", "Name" );
   }
   else
   {
      GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
      SetAttributeFromAttribute( vTZSAPERR, "ErrorMsg", "EntityName", vDTE, "TE_TablRec", "Name" );
   }

   if ( nError == 1 )
      strcpy_s( szMsg, sizeof( szMsg ), "Error: " );
   else
   if ( nError == 0 )
      strcpy_s( szMsg, sizeof( szMsg ), "Warning: " );
   else
      strcpy_s( szMsg, sizeof( szMsg ), "*** " );

   strcat_s( szMsg, sizeof( szMsg ), szErrorMsg );

   if ( nError == -1 )
      strcat_s( szMsg, sizeof( szMsg ), " ***" );

   SetAttributeFromString( vTZSAPERR, "ErrorMsg", "ErrorText", szMsg );
   SetAttributeFromInteger( vTZSAPERR, "ErrorMsg", "Error", nError );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 &&
        CompareAttributeToString( vTZSAPERR, "Import", "ShowErrorList", "Y" ) == 0 )
   {
      zwTZERSAPD_RefreshErrorList( vWindow );
   }

   return( 0 );
} // zwTZERSAPD_WriteErrorMsg


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ErrorListSelectEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ErrorListSelectEntity( zVIEW vSubtask )
{
   zVIEW  vTZSAPERR;
   zVIEW  vERD;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( SetCursorFirstEntityByAttr( vERD, "ER_Entity", "Name", vTZSAPERR, "ErrorMsg", "EntityName", "" ) >= zCURSOR_SET )
      {
         zwTZEREMDD_SetEntitySelect( vSubtask );
      }
   }

   return( 0 );
} // zwTZERSAPD_ErrorListSelectEntity

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ErrorListSaveToFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ErrorListSaveToFile( zVIEW vSubtask )
{
   zVIEW  vTZSAPERR;
   zULONG ulZFlags = 0;
   zLONG  FileHandle;
   zCHAR  szMsg[300];
   zCHAR  szTemp[ 255 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zSHORT nRC;

   GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );

   szFileName[ 0 ] = 0;
   ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT |
              zOFN_SAVEAS | zOFN_HIDEREADONLY;

   if ( OperatorPromptForFile( vSubtask, szFileName, sizeof( szFileName ),
                               "Text Files (*.txt)|*.txt|", "txt", ulZFlags ) == 1 )
   {
      FileHandle = SysOpenFile( vSubtask, szFileName, COREFILE_WRITE );

      if ( FileHandle == -1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Unable to open " );
         strcat_s( szMsg, sizeof( szMsg ), szFileName );
         MessageSend( vSubtask, "FEH001", "SAP Import", szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         return( -1 );
      }

      strcpy_s( szMsg, sizeof( szMsg ), "Zeidon: SAP Import Error List from " );
      GetStringFromAttributeByContext( szTemp, vTZSAPERR, "Import", "SearchDateTime", "DD.MM.YYYY HH:MM:SS", 20 );
      strcat_s( szMsg, sizeof( szMsg ), szTemp );
      SysWriteLine( vSubtask, FileHandle, szMsg );
      SysWriteLine( vSubtask, FileHandle, "" );

      for ( nRC = SetCursorFirstEntity( vTZSAPERR, "ErrorMsg", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZSAPERR, "ErrorMsg", "" ) )
      {
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZSAPERR, "ErrorMsg", "EntityName" );
         if ( zstrcmp( szTemp, "" ) != 0 )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Entity: " );
            strcat_s( szMsg, sizeof( szMsg ), szTemp );
         }
         else
            strcpy_s( szMsg, sizeof( szMsg ), " \t " );

         strcat_s( szMsg, sizeof( szMsg ), "\t" );
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZSAPERR, "ErrorMsg", "ErrorText" );
         strcat_s( szMsg, sizeof( szMsg ), szTemp );
         SysWriteLine( vSubtask, FileHandle, szMsg );
      }

      SysWriteLine( vSubtask, FileHandle, "" );
      SysWriteLine( vSubtask, FileHandle, "End of Error List" );
      SysCloseFile( vSubtask, FileHandle, 0 );
   }

   return( 0 );
} // zwTZERSAPD_ErrorListSaveToFile

static zSHORT
fnTZERSAPD_RemoveTablesFromList( zVIEW  vTZERSAPO )
{
   CreateEntity( vTZERSAPO, "RemovedTable", zPOS_AFTER );
   SetMatchingAttributesByName( vTZERSAPO, "RemovedTable", vTZERSAPO, "Table", zSET_NULL );

   DeleteEntity( vTZERSAPO, "Table", zREPOS_NONE );

   return( 0 );
} // fnTZERSAPD_RemoveTablesFromList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_RemoveTablesFromList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_RemoveTablesFromList( zVIEW vSubtask )
{
   zVIEW   vTZERSAPO;
   zCHAR   szTableName[ 33 ];
   zSHORT  nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZERSAPO, "Table", "" ) )
   {
      // if selected Table a SAP Table, remove all related Tables
      if ( CompareAttributeToInteger( vTZERSAPO, "Table", "IndentLvl", 0 ) == 0 )
      {
         GetStringFromAttribute( szTableName, sizeof( szTableName ), vTZERSAPO, "Table", "ParentName" );
         for ( nRC = SetCursorFirstEntityByString( vTZERSAPO, "Table", "ParentName", szTableName, "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByString( vTZERSAPO, "Table", "ParentName", szTableName, "" ) )
         {
            fnTZERSAPD_RemoveTablesFromList( vTZERSAPO );
         }
      }
      else
      {
         fnTZERSAPD_RemoveTablesFromList( vTZERSAPO );
      }
   }

   if ( CheckExistenceOfEntity( vTZERSAPO, "Table" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZERSAPO, "Table", 1 );

   RefreshCtrl( vSubtask, "lbTableList" );

   return( 0 );
} // zwTZERSAPD_RemoveTablesFromList



static zSHORT
fnTZERSAPD_ShowRelatedTables( zVIEW vSubtask,
                              zVIEW vTZERSAPO )
{
   zSHORT  nRC;
   zCHAR  szTableName[ 33 ];

   // if selected Table a SAP Table, not a related Table, read from Work-Lod,
   // not from SAP
   if ( CompareAttributeToInteger( vTZERSAPO, "Table", "IndentLvl", 0 ) == 0 )
   {
      GetStringFromAttribute( szTableName, sizeof( szTableName ), vTZERSAPO, "Table", "ParentName" );

      // Exists the related Tables in Entity "RemovedTable"
      if ( SetCursorFirstEntityByString( vTZERSAPO, "RemovedTable", "ParentName", szTableName, "" ) >= zCURSOR_SET )
      {
         for ( nRC = SetCursorFirstEntityByString( vTZERSAPO, "RemovedTable", "ParentName", szTableName, "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByString( vTZERSAPO, "RemovedTable", "ParentName", szTableName, "" ) )
         {
            CreateEntity( vTZERSAPO, "Table", zPOS_AFTER );
            SetMatchingAttributesByName( vTZERSAPO, "Table", vTZERSAPO, "RemovedTable", zSET_NULL );
            SetAttributeFromInteger( vTZERSAPO, "Table", "LoadRelatedTables", 0 );
            DeleteEntity( vTZERSAPO, "RemovedTable", zREPOS_NONE );
         }
      }
      else
      {
         // LoadRelatedTables = 0 means, related Tables does not exists for this SAP Tables or is loaded
         if ( CompareAttributeToInteger( vTZERSAPO, "Table", "LoadRelatedTables", 1 ) == 0 )
         {
            // read related Table from SAP
            if ( zwTZERSAPD_CreateRelationship( vSubtask, vTZERSAPO, szTableName, zBuildRelatedEntity ) < 0 )
            {
               return( -1 );
            }
            SetAttributeFromInteger( vTZERSAPO, "Table", "LoadRelatedTables", 0 );
         }
      }
   }

   return( 0 );
} // fnTZERSAPD_ShowRelatedTables


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ShowRelatedTables
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ShowRelatedTables( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zSHORT nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZERSAPO, "Table", "" ) )
   {
      if ( fnTZERSAPD_ShowRelatedTables( vSubtask, vTZERSAPO ) < 0 )
         break;
   }

   OrderEntityForView( vTZERSAPO, "Table", "WorkName A" );
   RefreshCtrl( vSubtask, "lbTableList" );

   return( 0 );
} // zwTZERSAPD_ShowRelatedTables



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_ShowRelatedForAllTabl
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ShowRelatedForAllTabl( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zSHORT nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstEntity( vTZERSAPO, "Table", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSAPO, "Table", "" ) )
   {
      if ( fnTZERSAPD_ShowRelatedTables( vSubtask, vTZERSAPO ) < 0 )
         break;
   }

   OrderEntityForView( vTZERSAPO, "Table", "WorkName A" );
   SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" );
   RefreshCtrl( vSubtask, "lbTableList" );

   return( 0 );
} // zwTZERSAPD_ShowRelatedForAllTabl


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_RemoveRelatedTables
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_RemoveRelatedTables( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zSHORT nRC;
   zCHAR  szTableName[ 33 ];
   zLONG  lCount = 0;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZERSAPO, "Table", "" ) )
   {
      // if selected Table a related Table, not a SAP Table
      if ( CompareAttributeToInteger( vTZERSAPO, "Table", "IndentLvl", 1 ) == 0 )
      {
         lCount++;
         fnTZERSAPD_RemoveTablesFromList( vTZERSAPO );
      }
   }

   // if SAP Table selected, not related Table selected, remove all related Tables
   if ( lCount == 0 )
   {
      GetStringFromAttribute( szTableName, sizeof( szTableName ), vTZERSAPO, "Table", "ParentName" );
      for ( nRC = SetCursorFirstEntityByString( vTZERSAPO, "Table", "ParentName", szTableName, "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByString( vTZERSAPO, "Table", "ParentName", szTableName, "" ) )
      {
         // if selected Table a related Table, not a SAP Table
         if ( CompareAttributeToInteger( vTZERSAPO, "Table", "IndentLvl", 1 ) == 0 )
         {
            fnTZERSAPD_RemoveTablesFromList( vTZERSAPO );
         }
      }
   }

   SetSelectStateOfEntity( vTZERSAPO, "Table", 1 );
   RefreshCtrl( vSubtask, "lbTableList" );

   return( 0 );
} // zwTZERSAPD_RemoveRelatedTables



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_RemoveAllRelatedTabl
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_RemoveAllRelatedTabl( zVIEW vSubtask )
{
   zVIEW  vTZERSAPO;
   zCHAR  szTableName[ 33 ];
   zSHORT nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTableName, sizeof( szTableName ), vTZERSAPO, "Table", "ParentName" );

   // all related Tables, not SAP Tables
   for ( nRC = SetCursorFirstEntityByInteger( vTZERSAPO, "Table", "IndentLvl", 1, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByInteger( vTZERSAPO, "Table", "IndentLvl", 1, "" ) )
   {
      fnTZERSAPD_RemoveTablesFromList( vTZERSAPO );
   }

   // set new cursor position
   if ( SetCursorFirstEntityByString( vTZERSAPO, "Table", "TableName", szTableName, "" ) >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTZERSAPO, "Table", 1 );
   }

   RefreshCtrl( vSubtask, "lbTableList" );

   return( 0 );
} // zwTZERSAPD_RemoveAllRelatedTabl



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_LoadTablesFromWorkLod
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERSAPD_LoadTablesFromWorkLod( zVIEW  vSubtask,
                                  zVIEW  vTZERSAPO )
{
   zSHORT  nRC;

   // load all removed SAP Tables, not related Tables
   for ( nRC = SetCursorFirstEntityByInteger( vTZERSAPO, "RemovedTable", "IndentLvl", 0, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByInteger( vTZERSAPO, "RemovedTable", "IndentLvl", 0, "" ) )
   {
      CreateEntity( vTZERSAPO, "Table", zPOS_AFTER );
      SetMatchingAttributesByName( vTZERSAPO, "Table", vTZERSAPO, "RemovedTable", zSET_NULL );
      SetAttributeFromInteger( vTZERSAPO, "Table", "LoadRelatedTables", 0 );
      DeleteEntity( vTZERSAPO, "RemovedTable", zREPOS_NONE );
   }

   // remove all related Tables
   if ( SetCursorFirstEntity( vTZERSAPO, "Table", "" ) >= zCURSOR_SET )
   {
      SetCursorFirstSelectedEntity( vTZERSAPO, "Table", "" );
      zwTZERSAPD_RemoveAllRelatedTabl( vSubtask );
      OrderEntityForView( vTZERSAPO, "Table", "WorkName A" );
      RefreshCtrl( vSubtask, "lbTableList" );
   }

   return( 0 );
} // zwTZERSAPD_LoadTablesFromWorkLod



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetButtonTextForBuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetButtonTextForBuild( zVIEW vSubtask )
{
   SetCtrlText( vSubtask, "pbImport", "&Build" );

   // disable Button "Import all"
   SetCtrlState( vSubtask, "pbImportAll", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // zwTZERSAPD_SetButtonTextForBuild


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_SetButtonTextForImp
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_SetButtonTextForImp( zVIEW vSubtask )
{
   SetCtrlText( vSubtask, "pbImport", "&Import" );

   // enable Button "Import all"
   SetCtrlState( vSubtask, "pbImportAll", zCONTROL_STATUS_ENABLED, TRUE );

   return( 0 );
} // zwTZERSAPD_SetButtonTextForImp


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_BuildTE_Only
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_BuildTE_Only( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vTZERSAPO;
   zVIEW  vTZSAPERR;
   zVIEW  vTZSapRel;
   zSHORT nRC;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTZSapRel, "TZSapRel", vSubtask, zLEVEL_TASK ) < 0 )
   {
      SysMessageBox( vSubtask,
                     "SAP Import", "Internal work object not found.\n"
                     "Please start Build of Relationships and TE.", 1 );
      return( -1 );
   }

   if ( MessagePrompt( vSubtask, "ER00193", "SAP Import",
                       "This is a internal Function for SAP Import.\n\n"
                       "Do you want to create the TE without building Relationships?",
                        zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                        zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_TE_only );

   // added search string to Error List and load Error List View
   zwTZERSAPD_CreateNewErrorRoot( vSubtask );
   GetViewByName( &vTZSAPERR, "TZSAPERR", vSubtask, zLEVEL_TASK );

   if ( zwTZERSAPD_Build_AskForSave( vSubtask, vTZERSAPO ) < 0 )
   {
      // Refresh Erorr List Window and ERD
      zwTZERSAPD_RefreshAllWindows( vSubtask, vTZERSAPO, vTZSAPERR, zAbortBuild );
      return( 0 );
   }

   MB_SetMessage( vSubtask, 0, "Build TE" );
   nRC = zwTZERSAPD_Build_SAP_TE( vSubtask, vERD, vTZSapRel );

   if ( nRC < 0 )
      fnTZERSAPD_SendMessageBox( vSubtask, zBuild_TE_only, "with Errors", zICON_ERROR );
   else
      fnTZERSAPD_SendMessageBox( vSubtask, zBuild_TE_only, "successfully", zICON_INFORMATION );

   // Refresh Erorr List Window and ERD
   zwTZERSAPD_RefreshAllWindows( vSubtask, vTZERSAPO, vTZSAPERR, nRC );

   return( 0 );
} // zwTZERSAPD_BuildTE_Only


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZERSAPD_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_PostBuild( zVIEW vSubtask )
{
   zVIEW vTZERSAPO;

   GetViewByName( &vTZERSAPO, "TZERSAPO", vSubtask, zLEVEL_TASK );

   // Set Defaults
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "LanguageType", "D" );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "ImportFlags", zBuild_Tables );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "OverwriteExistingEntity", "N" );
   SetAttributeFromString( vTZERSAPO, "SapEnvironment", "ShowErrorList", "N" );
   SetAttributeFromInteger( vTZERSAPO, "SapEnvironment", "IgnoreError", zIgnoreError );

   return( 0 );
} // zwTZERSAPD_PostBuild


#ifdef __cplusplus
}
#endif
