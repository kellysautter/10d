/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmrptd.c - Zeidon Report dialod Operations
// DESCRIPTION:   This C file contains dialog operations for the
//                Zeidon AuditTrail display in WksAdmin and SysAdmin
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Frank Holzenthal
// DATE:          1997/05/15
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: ZO00172
//
// HISTORY:
//
//    1997/05/15: Beginning of 0.9G branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
20001.07.08 BL   Repository
   maintained Repository, part 2
2001.06.26  Bl   Repository
   maintained Repository, part 2 ( replace LOD TZCMREPO with TZCMCPL )
2001.03.11  BL   Repository
   maintained Repository
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.02.24   BL   Z10   refresh user defined
   added zwTZCNRPTD_RefreshAllComponents, zwTZCNRPTD_RefreshSelComponents
   and zwTZCNRPTD_CloseRefresh for refresh user defined
1999.08.31   BL   Z10   new Menu strurce
   insert Operation zwTZCMRPTD_RefreshStatus
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

zPCHAR pcNoCompoents  = "No Compoments";

#define zRefreshLPLR          1
#define zRefreshUserDefined   2


static zSHORT
zwfnTZCMRPTD_LoadNewAudittrail( zVIEW vSubtask, zPVIEW vTZBRAU2O );

zOPER_EXPORT zSHORT OPERATION
zwTZCMLPLD_OptionRefreshFromRepo( zVIEW   vSubtask,
                                  zPVIEW  vTZCMLPLO,
                                  zPCHAR  szName,
                                  zSHORT  nRefreshLPLR );

zOPER_EXPORT zSHORT OPERATION
zwTZCMLPLD_RefreshUserDefined( zVIEW  vSubtask );


zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_LoadAudittrail( zVIEW vSubtask )
{
   zVIEW    vTZCMRPTO;
   zVIEW    vTZBRAUDO;
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szZKey[18];
   zSHORT   nRC;

   if ( GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vTZCMRPTO, "Z_MetaDef" ) == 0 )
      {
         if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                            zSINGLE ) >= 0 )
         {
            SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
            CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
            CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
         }
         else
            return( -1 );

         GetIntegerFromAttribute(( zPLONG )&ulZKey, vTZCMRPTO, "Z_MetaDef", "ZKey" );
         SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "Z_MetaDef" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "Z_MetaDef" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
         zltoa( ulZKey, szZKey, sizeof( szZKey ) );
         SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );

         nRC = ActivateObjectInstance( &vTZBRAUDO, "TZBRAUDO", vSubtask,
                                       vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
         DropObjectInstance( vKZDBHQUA );

         if ( CheckExistenceOfEntity( vTZBRAUDO, "AuditTrail" ) == 0 )
            SetNameForView( vTZBRAUDO, "TZBRAUDO", vSubtask, zLEVEL_TASK );
         else
         {
            MessageSend( vSubtask, "CM00299", "Audit Trail:",
                         "There are no AuditTrail-Information for the component in this CPLR.",
                         zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );

         }
      }
   }
   else
      return( -1 );

   return( 0 );
}

zSHORT  /* LOCAL */  OPERATION
zwTZCMRPTD_LoadDesc_AUX( zVIEW vSubtask,
                         zLONG CPLR_ZKey,
                         zPCHAR pszTimeStamp )
{

   zVIEW    vTZBRAU3O;
   zVIEW    vKZDBHQUA;
   zCHAR    szZKey[18];
   zSHORT   nRC;

   if ( GetViewByName( &vTZBRAU3O, "TZBRAU3O", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZBRAU3O );

   if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                    zSINGLE ) >= 0 )
   {
      SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
      CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
      CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   }
   else
      return( -1 );

   // QualAttrib for "WHERE CPLR.ZKey = CPLR_ZKey"
   SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "AuditTrail" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   zltoa( CPLR_ZKey, szZKey, sizeof( szZKey ) );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );

   // QualAttrib for "AND"
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "AND" );

   // QualAttrib for "WHERE AuditTrail.TimeStamp = pszTimeStamp"
   CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "AuditTrail" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "TimeStamp" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
   SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", pszTimeStamp );

   nRC = ActivateObjectInstance( &vTZBRAU3O, "TZBRAU3O", vSubtask,
                                 vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );

   DropObjectInstance( vKZDBHQUA );

   if (CheckExistenceOfEntity( vTZBRAU3O, "AuditTrail" ) == 0 )
      SetNameForView( vTZBRAU3O, "TZBRAU3O", vSubtask, zLEVEL_TASK );
   else
   {
      MessageSend( vSubtask, "CM00299", "Audit Trail:",
                   "There are no AuditTrail-Information for the component.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_LoadDesc_AU2O( zVIEW vSubtask )
{

   zLONG  CPLR_ZKey;
   zCHAR  szTimeStamp[ 20 ];
   zVIEW  vTZBRAU2O;

   if ( GetViewByName( &vTZBRAU2O, "TZBRAU2O", vSubtask, zLEVEL_TASK ) > 0 )
   {
     GetIntegerFromAttribute( &CPLR_ZKey, vTZBRAU2O, "CPLR", "ZKey");
     GetStringFromAttribute( szTimeStamp, sizeof( szTimeStamp ), vTZBRAU2O, "AuditTrail", "TimeStamp");
     zwTZCMRPTD_LoadDesc_AUX( vSubtask, CPLR_ZKey, szTimeStamp );
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_LoadDesc_AUDO( zVIEW vSubtask )
{
   zLONG  CPLR_ZKey;
   zCHAR  szTimeStamp[ 20 ];
   zVIEW  vTZBRAUDO;

   if ( GetViewByName( &vTZBRAUDO, "TZBRAUDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
     GetIntegerFromAttribute( &CPLR_ZKey, vTZBRAUDO, "CPLR", "ZKey");
     GetStringFromAttribute( szTimeStamp, sizeof( szTimeStamp ), vTZBRAUDO, "AuditTrail", "TimeStamp");
     zwTZCMRPTD_LoadDesc_AUX( vSubtask, CPLR_ZKey, szTimeStamp );
   }

   return( 0 );
}

zSHORT  /* LOCAL */  OPERATION
zwTZCMRPTD_ExportTextAUX( zVIEW vSubtask,
                          zPCHAR pszFileName )
{
   zVIEW   vTZBRAU3O;
   zLONG   lfFile;
   zCHAR   szMsg[ 256 ];
   zCHAR   szText[ 1000 ];
   zPCHAR  szAttr;

   if ( GetViewByName( &vTZBRAU3O, "TZBRAU3O", vSubtask, zLEVEL_TASK ) > 0 )
   {
      lfFile = SysOpenFile( vSubtask, pszFileName, COREFILE_WRITE );

      if ( lfFile < 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Error writing the File '" );
         strcat_s( szMsg, sizeof( szMsg ), pszFileName );
         strcat_s( szMsg, sizeof( szMsg ), "'." );
         MessageSend( vSubtask, "CM00xxx", "Configuration Management",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      strcpy_s( szText, sizeof( szText ), "AuditTrail Print created with (c) Zeidon 1997" );
      SysWriteLine( vSubtask, lfFile, szText );

      strcpy_s( szText, sizeof( szText ), "---------------------------------------------" );
      SysWriteLine( vSubtask, lfFile, szText );

      // save the CPLR-Name:
      strcpy_s( szText, sizeof( szText ), "CPLR:\t\t" );
      GetStringFromAttribute( szMsg, sizeof( szMsg ), vTZBRAU3O, "CPLR", "Name" );
      strcat_s( szText, sizeof( szText ), szMsg );
      SysWriteLine( vSubtask, lfFile, szText );

      // save the DateTime:
      strcpy_s( szText, sizeof( szText ), "Date:\t\t" );
      GetStringFromAttributeByContext( szMsg, vTZBRAU3O, "AuditTrail", "TimeStamp",
                                       "DD.MM.YYYY HH:MM:SS", 20);
      strcat_s( szText, sizeof( szText ), szMsg );
      SysWriteLine( vSubtask, lfFile, szText );

      // save the User:
      strcpy_s( szText, sizeof( szText ), "User:\t\t" );
      GetStringFromAttribute( szMsg, sizeof( szMsg ), vTZBRAU3O, "AuditTrail", "UserName" );
      strcat_s( szText, sizeof( szText ), szMsg );
      strcat_s( szText, sizeof( szText ), " " );
      GetStringFromAttribute( szMsg, sizeof( szMsg ), vTZBRAU3O, "AuditTrail", "UserDesc" );
      strcat_s( szText, sizeof( szText ), szMsg );
      SysWriteLine( vSubtask, lfFile, szText );

      // save the ShortDesc:
      strcpy_s( szText, sizeof( szText ), "ShortDesc:\t" );
      GetStringFromAttribute( szMsg, sizeof( szMsg ), vTZBRAU3O, "AuditTrail", "ShortDesc" );
      strcat_s( szText, sizeof( szText ), szMsg );
      SysWriteLine( vSubtask, lfFile, szText );

      strcpy_s( szText, sizeof( szText ), "---------------------------------------------" );
      SysWriteLine( vSubtask, lfFile, szText );

      GetAddrForAttribute( &szAttr, vTZBRAU3O, "AuditTrail", "Desc" );
      SysWriteLine( vSubtask, lfFile, szAttr );

      // Close the template file.
      SysCloseFile( vSubtask, lfFile, 0 );
   }
   else
   {
      IssueError( vSubtask, 0, 0,
                  "Object TZCMAU3O does not exist (zwTZCMRPTD_ExportTextAUX)" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_ExportTextFile( zVIEW vSubtask )
{

   zCHAR   szFileName[ 256 ];
   zCHAR   szMsg[ 256 ];
   zSHORT  nRC;

   GetSystemDirectory(szFileName, sizeof(szFileName));

   while ( TRUE )
   {
      nRC = OperatorPromptForFile( vSubtask, szFileName, sizeof( szFileName ),
                              "Text Files (*.TXT)|*.txt|", "TXT", FALSE );

      // Operator pressed cancel --> get outta here:
      if ( nRC != 1 )
        break;

      // if File does not exist --> ok
      if ( SysOpenFile( vSubtask, szFileName, COREFILE_EXIST ) < 0 )
        break;

      strcpy_s( szMsg, sizeof( szMsg ), "Override the current File '" );
      strcat_s( szMsg, sizeof( szMsg ), szFileName );
      strcat_s( szMsg, sizeof( szMsg ), "' ?" );

      // overwrite the existing file ? --> also get out of here:
      if ( MessagePrompt( vSubtask, "CM00xxx",
                          "Configuration Management",
                          szMsg, zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_YES,  0 ) == zRESPONSE_YES )
      {
         break;
      }
   }

   if ( nRC == 1 )
      zwTZCMRPTD_ExportTextAUX( vSubtask, szFileName );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_RefreshAllComponents
//
// Refresh Components from
// CPLR

zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_RefreshAllComponents( zVIEW vSubtask )
{
   zVIEW    vTZCMLPLO;
   zVIEW    vTZCMRPTO;
   zVIEW    vCompList;
   zCHAR    szName[ 33 ];
   zCHAR    szERR_Msg[ 254 ];

   if ( GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRPTO );

   if ( zwTZCMLPLD_OptionRefreshFromRepo( vSubtask, &vTZCMLPLO,
                                          szName, zRefreshLPLR ) < 0 )
   {
      return( -1 );
   }

   strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Project '" );
   GetStringFromAttribute( szERR_Msg + zstrlen( szERR_Msg ), sizeof( szERR_Msg ) - zstrlen( szERR_Msg ), vTZCMLPLO, "LPLR", "Name" );
   strcat_s( szERR_Msg, sizeof( szERR_Msg ), "' has been successfully refreshed." );
   MessageSend( vSubtask, "CM00279", "Configuration Management",
                szERR_Msg,
                zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );

   if ( GetViewByName( &vCompList, "CompList", vSubtask, zLEVEL_TASK ) >= 0 )
      RefreshWindow( vCompList );

   return( 0 );
} // zwTZCMRPTD_RefreshAllComponents

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_CloseRefresh
//

zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_CloseRefresh( zVIEW vSubtask )
{
   zVIEW  vView;

   if ( GetViewByName( &vView, "TZCMRPTO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZCMRPTO_New", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   return( 0 );
} // zwTZCMRPTD_CloseRefresh

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_RefreshSelComponents
//
// refresh selected Components for current selected
// CPLR

zOPER_EXPORT zSHORT OPERATION
zwTZCMRPTD_RefreshSelComponents( zVIEW vSubtask )
{
   zVIEW    vTZCMLPLO;
   zVIEW    vTZCMRPTO;
   zVIEW    vCompList;
   zVIEW    vTZCMRPTO_New;
   zCHAR    szName[ 33 ];
   zCHAR    szERR_Msg[ 254 ];

   SetNameForView( vSubtask,"TZCM_ChkInWindow", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstSelectedEntity( vTZCMRPTO, "ReportLine",
                                      "" ) < zCURSOR_SET )
   {
      strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "No Component selected to Refresh." );
      MessageSend( vSubtask, "CM00241", "Configuration Management",
                   szERR_Msg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 0 );
   }

   if ( zwTZCMLPLD_OptionRefreshFromRepo( vSubtask, &vTZCMLPLO,
                                          szName, zRefreshUserDefined ) < 0 )
   {
      return( -1 );
   }

   GetViewByName( &vTZCMRPTO_New, "TZCMRPTO_New", vSubtask, zLEVEL_TASK );

   // if component refreshed?
   if ( CompareAttributeToInteger( vTZCMRPTO_New, "CPLR",
                                   "ComponentRefreshed", 1 ) == 0 )
   {
      strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Project '" );
      GetStringFromAttribute( szERR_Msg + zstrlen( szERR_Msg ), sizeof( szERR_Msg ) - zstrlen( szERR_Msg ), vTZCMLPLO,
                              "LPLR", "Name" );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), "' has been successfully refreshed \nfrom CPLR '" );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), szName );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), "'." );
      MessageSend( vSubtask, "CM00279", "Configuration Management",
                   szERR_Msg,
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   if ( GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZCMRPTO );

   // Build new Refresh List
   if ( zwTZCMLPLD_RefreshUserDefined( vSubtask ) < 1 )
      return( -1 );

   RefreshWindow( vSubtask );

   if ( GetViewByName( &vCompList, "CompList", vSubtask, zLEVEL_TASK ) >= 0 )
      RefreshWindow( vCompList );

   return( 0 );
} // zwTZCMRPTD_RefreshSelComponents

static zSHORT
zwfnTZCMRPTD_LoadNewAudittrail( zVIEW vSubtask, zPVIEW vTZBRAU2O )
{
   zVIEW    vTZCMCPL;
   zVIEW    vKZDBHQUA;
   zULONG   ulZKey;
   zCHAR    szZKey[18];
   zSHORT   nRC;

   if ( GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vTZCMCPL, "CPLR" ) == zCURSOR_SET )
   {

      if ( ActivateEmptyObjectInstance( &vKZDBHQUA, "KZDBHQUA", vSubtask,
                                        zSINGLE ) >= 0 )
      {
         SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
         CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
         CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
      }
      else
         return( -1 );

      GetIntegerFromAttribute( (zPLONG)&ulZKey, vTZCMCPL, "CPLR", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName", "CPLR" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName", "CPLR" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
      zltoa( ulZKey, szZKey, sizeof( szZKey ) );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );


      nRC = ActivateObjectInstance( vTZBRAU2O, "TZBRAU2O", vSubtask,
                                    vKZDBHQUA, zSINGLE | zLEVEL_APPLICATION );
      DropObjectInstance( vKZDBHQUA );
      SetNameForView( *vTZBRAU2O, "TZBRAU2O", vSubtask, zLEVEL_TASK );
   }
   else
   {
      MessageSend( vSubtask, "CM00299", "Audit Trail:",
                   "There are no Audit Trail Information for a non-repository LPLR.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
} // zwfnTZCMRPTD_LoadNewAudittrail

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_LoadNewAudittrail
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMRPTD_LoadNewAudittrail( zVIEW vSubtask )
{
   zVIEW   vTZCMCPL;
   zVIEW   vTZBRAU2O;
   zVIEW   vParentWindow;
   zULONG  ulZKey;
   zULONG  ulZKeyCPLR;
   zCHAR   szName[ 33 ];
   zCHAR   szControlText[40];

   GetViewByName( &vTZCMCPL, "TZCMCPL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMCPL, "CPLR", "ZKey" );
   GetStringFromAttribute( szName, sizeof( szName ), vTZCMCPL, "CPLR", "Name" );

   if ( GetViewByName( &vTZBRAU2O, "TZBRAU2O", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( (zPLONG) &ulZKeyCPLR, vTZBRAU2O, "CPLR", "ZKey" );
      if ( ulZKey == ulZKeyCPLR )
         return( 0 );
      else
         DropObjectInstance( vTZBRAU2O );
   }

   if ( zwfnTZCMRPTD_LoadNewAudittrail( vSubtask, &vTZBRAU2O ) < 0 )
   {
      return( -1 );
   }

   GetStringFromAttribute( szName, sizeof( szName ), vTZCMCPL, "CPL", "Name" );
   strcpy_s( szControlText, sizeof( szControlText ), " CPL: " );
   strcat_s( szControlText, sizeof( szControlText ), szName );
   SetCtrlText( vSubtask, "txtCPL", szControlText );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshCtrl( vParentWindow, "lbCPLRList" );

   return( 0 );
} // zwTZCMRPTD_LoadNewAudittrail

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_DropViewForAudittrail
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMRPTD_DropViewForAudittrail( zVIEW vSubtask )
{
   zVIEW vTZBRAUDO;

   if ( GetViewByName( &vTZBRAUDO, "TZBRAUDO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZBRAUDO );

   if ( GetViewByName( &vTZBRAUDO, "TZBRAU2O", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZBRAUDO );

   if ( GetViewByName( &vTZBRAUDO, "TZBRAU3O", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZBRAUDO );

   return( 0 );
} // zwTZCMRPTD_DropViewForAudittrail

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_SetCPLR_Desc
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMRPTD_SetCPLR_Desc( zVIEW vSubtask )
{
   zVIEW   vTZCMRPTO;
   zCHAR   szDesc[ 255 ];
   zCHAR   szControlText[270];

   if ( GetViewByName( &vTZCMRPTO, "TZCMRPTO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetStringFromAttribute( szDesc, sizeof( szDesc ), vTZCMRPTO, "CPLR", "Desc" );
   strcpy_s( szControlText, sizeof( szControlText ), " Desc: " );
   strcat_s( szControlText, sizeof( szControlText ), szDesc );

   SetCtrlText( vSubtask, "txtDesc", szControlText );

   return( 0 );
} // zwTZCMRPTD_SetCPLR_Desc


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZCMRPTD_SetMinWindowSize
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZCMRPTD_SetMinWindowSize( zVIEW vSubtask )
{
   zLONG  lSize_X;
   zLONG  lSize_Y;
   zLONG  lCtrlPos_X;

   // Set new position for Buttons
   GetCtrlSize( vSubtask, "lbRefreshList", &lSize_X, &lSize_Y, TRUE );
   lCtrlPos_X = lSize_X + 10;

   SetCtrlPosition( vSubtask, "pbClose", lCtrlPos_X, 5, TRUE );
   SetCtrlPosition( vSubtask, "pbRefresh", lCtrlPos_X, 26, TRUE );
   SetCtrlPosition( vSubtask, "pbRefreshAll", lCtrlPos_X, 42, TRUE );
   SetCtrlPosition( vSubtask, "pbDetail", lCtrlPos_X, 65, TRUE );
   SetCtrlPosition( vSubtask, "pbAuditTrail", lCtrlPos_X, 81, TRUE );

   return( 0 );
} // zwTZCMRPTD_SetMinWindowSize
