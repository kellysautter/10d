/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzeremdd.c - Zeidon E/R dialog operations
// DESCRIPTION:   This C file contains dialog operations for the
//                Zeidon E/R application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: ER00200
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2002.11.20  DGC
   Added CreateKeyListEntity().
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2002.04.16 FH
   after "Duplicate TE Table Name in Data Model" allow to return to parent window
2002.03.12 FH
   avoid "ViewIsReadOnly" if a meta is not checked out.
2001.07.31 HH
   fix warning with DimSetLevelByZKeys (Phil).
2001.06.15 PAS
   Added additional functions to support Qubon dimension hierarchies.
2001.03.06 BL
   revise ERD Cross Reference
2001.02.26 BL
   revise ERD Cross Reference
2001.02.18 BL   R54627
   Save ERD with Subject Area
2001.02.17 BL   R54267
   Exists a Identifier without Attribute or Relationship, send a Error
   List Window
2001.02.16 BL
   Fix for load ERD
2001.02.13 BL  R54697
   improve handling of return code from InitializeLPLR
2000.11.13 BL
   Drop SAP Views by Switch Project
2000.11.07 RG
   Enable 'Import Siron Catalogs' if there is no datamodel created
   in a new LPLR
2000.11.02 BL
   Do not create or delete a Relationship from or to a SAP Entity,
   Bugfix load Relationship
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.14  BL  Z10
   Modified Functions for Import SAP
2000.09.26  DC
   Modified DeleteAttrib so that an Identifier for the attribute would be deleted if the
   attribute were deleted.
2000.09.20  BL  Z10
   New Handling for Generate TE Names
2000.08.31  BL
   Modified zwTZEREMDD_MainRtnFromSubWnd, zwTZEREMDD_OKNewModel and
   zwTZEREMDD_CancelNewModel: if Data Model does not exists, disable ERD
   Control, do not create Entity
2000.08.22  RG  Z2000
   Set the right attribute as identifier when copying an entity
2000.08.04  BL
   Modified all operations for completely maintained ERD Tool
   (Cursor problems)
2000.08.03  BL  Z10  TB 51060, TB 50544, TB 51372
   Modified zwTZEREMDD_AddSelectedEntsToSA for Bug TB51060 and
   zwTZEREMDD_InitTool2 and Added zwTZEREMDD_ZoomDiagram for Bug TB 50544
   Added zwTZEREMDD_LoadEntityList, zwTZEREMDD_LoadRelationshipList,
   zwTZEREMDD_LoadAttributeList, zwTZEREMDD_LoadIdentifierList for
   Bug TB51372
2000.07.13  BL  Z10
   Added zwTZEREMDD_SaveAsKeepCurrentDesc and modified other save as
   functions for Keep Current Description
2000.06.20  BL  Z10
   enable Actions again
2000.06.15  BL  Z10
   Modified zwTZEREMDD_DeleteRelLink for delete Relationship in
   Entity Detail Window
2000.06.10  BL  Z10  ADW Export
   Added zwTZEREMDD_ADW_Exp_Browse, zwTZEREMDD_ADW_Exp_OK,
   zwTZEREMDD_ADW_Exp_PreBuild, zwTZEREMDD_ADW_Exp_Cancel
2000.06.09  BL  Z10  ADW Import
   Added zwTZEREMDD_ADW_Browse and zwTZEREMDD_ADW_Set_Dir_Prefix for
   Browse by ADW Import and Export and modified zwTZEREMDD_ADW_OK
2000.06.07  BL  Z10  Bug TB151007
   Modified zwTZEREMDD_CopyEntity: copy Entity with Identifier
2000.06.06  BL  Z10 Repository and Tools
   Added zwTZEREMDD_DisableDeleteActions: if switch LPLR and the new ERD
   not checked out, then disable the Delete Action for Attribute List,
   Relationship List, Identifier List and Entity List
   Modified zwTZEREMDD_InitTool2, zwTZEREMDD_MainRtnFromSubWnd,
   zwTZEREMDD_SaveSelectedAttribute, zwTZEREMDD_PostBuildATTR,
   zwTZEREMDD_PostBuildRLL, zwTZEREMDD_RelLinkOK, zwTZEREMDD_PostBuildID,
   zwTZEREMDD_PostBuildIDMAINT:
   if ERD not checked out, set Data Model View (TZEREMDO) read only ->
   the user cannot update the values in Detail Windows (f.e.Window
   Entity Detail)
   Modified zwTZEREMDD_InitMenuForSA, zwTZEREMDD_InitMenuForEditItems,
   zwTZEREMDD_InitMenu, zwTZEREMDD_InitTool2
   and added zwTZEREMDD_InitMenuForDeleteAll, zwTZEREMDD_InitMenuUtilities:
   if ERD not checked out, disable menu items for update
2000.05.24  BL  Z10
   Modified CreateGenkeyEntityLOD, CreateERD_Attribute and CreateLockingEntity
   for set attribute sequence
2000.05.24  BL  Z10  Repository and Tools
   Modified zwTZEREMDD_InitTool2, zwTZEREMDD_MainRtnFromSubWnd,
   zwTZEREMDD_OKNewModel for setcheck out state in window title
   Modified zwTZEREMDD_InitMenuForSA, zwTZEREMDD_InitMenuForEditItems,
   zwTZEREMDD_RefreshSAWnd:
   if data model not checked out, disable all menu items and functions
   for update data model or subject area
   Added zwTZEREMDD_Check_CheckoutStatus: set check out state in window
   open subject area and disable/enable Delete and New Button
   Modified zwTZEREMDD_LoadSAList: set cursor on curent SA
2000.05.18  BL  Z10
   Added zwTZEREMDD_SetRSSelection,zwTZEREMDD_RefreshRSWndForSort,
   zwTZEREMDD_RefEntityListForSort, zwTZEREMDD_RefreshAttWndForSort,
   zwTZEREMDD_RefEntListForSort for Synchronize Entity List with
   Attribute and Relationship List
   Modified zwTZEREMDD_InitTool2, zwTZEREMDD_MainRtnFromSubWnd,
   zwTZEREMDD_SetRelationshipSelect, zwTZEREMDD_MoveAttribute,
   zwTZEREMDD_SaveSelectedAttribute, zwTZEREMDD_PasteAttributes,
   zwTZEREMDD_RefreshEntityList, zwTZEREMDD_RefreshAttributeWnd
   for show original attribute sequence
2000.05.11  BL  Z10  51328
   Bug fix 51328 delete Relationship in window Entity Detail
2000.01.21  BL  Z10
   Modified zwTZEREMDD_DeleteRelLink for Bug TB944
*/

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

// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

#include "tzctlopr.h"
#include "tzlodopr.hg"

zCHAR szWorkMessage[ 300 ];
zCHAR sz[ 256 ];

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetRelationshipSelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ValidateEntity( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntAttRelWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttRelSAWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttRelWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshIdWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshRllWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshSAWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SaveModel( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_InitMenu( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ViewModel( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SwitchLPLR( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_Print( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_DiagramPrint( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AskForSave( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_AskForSave( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntitySelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_HighliteEntity( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_CenterEntity( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomStandardSize( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomStdSizeSA( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseModelessWindows( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseENTVIEW( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseATTVIEW( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseRLLVIEW( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseIDVIEW( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_RePositionEntity( zVIEW vSubtask, zVIEW vTZEREMDO );

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_PositionNonPosEnts( zVIEW vSubtask, zVIEW vTZEREMDO );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_PosNonPosEntsSA( zVIEW vSubtask, zVIEW vTZERSASO );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDO_SetEntityState( zVIEW vSubtask, zVIEW vTZEREMDO, zBOOL bSelect );
zSHORT
zwfnTZEREMDD_RedirectRelationship( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_AddEntity( zVIEW vSubtask, zVIEW vTZEREMDO );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_BuildWorkOI( zVIEW vSubtask, zVIEW vADW_Work );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_GrayIdentifierActions( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetDIL_Message( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_SetAttributeSequence( zVIEW vTZEREMDO );

zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_SortAttributes( zVIEW vTZEREMDO );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SaveSelectedAttribute( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttWndForSort( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefEntListForSort( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW   vSubtask, zPCHAR  szToolname, zPCHAR  szERDname, zVIEW   vToolView, zPCHAR  szEntityname, zLONG   lType );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DisableDeleteActions( zVIEW  vSubtask, zSHORT nEnable );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ZoomDiagram( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_AcceptSubobject( zVIEW  vSubtask, zPCHAR szEntityName, zPCHAR szMessageText, zPCHAR szTE_Attribute, zPCHAR szTE_MessText );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshIdentifierWnd( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_PreBuildIDATTADD( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_PreBuildIDRLLADD( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_DeselectRelationship( zVIEW  vView, zPCHAR szEntityName, zSHORT nSelectInstance );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_DeselectAll( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildATTR( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_DisableCtrlForRLL( zVIEW   vSubtask, zVIEW   vTZEREMDO );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_BuildRelationships_TE( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERSAPD_ExitImport( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LoadErrorList( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_CreateNewErrorRoot( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshErrorList( zVIEW vSubtask );

static void
zwfnTZEREMDD_CheckRelLinkIdentifier( zVIEW vSubtask, zVIEW vERD2 );
static void
zwfnTZEREMDD_CheckDomains( zVIEW vSubtask, zVIEW vERD2 );
static void
zwfnTZEREMDD_CheckRelationships( zVIEW vSubtask, zVIEW vERD2 );
static zSHORT
zwfnTZEREMDD_LoadErrorList( zVIEW vSubtask );
static zSHORT
zwfnTZEREMDD_CommitERD( zVIEW vSubtask, zVIEW vTZEREMDO );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RebuildTablesRels( zVIEW vSubtask );



zVOID
zwfnTZEREMDD_FindRelatedVisible( zVIEW vSubtask, zVIEW vTZEREMDO1, zVIEW vTZEREMDO2 )
{
   if ( SetCursorFirstEntity( vTZEREMDO1, "ER_Entity_Other", "ER_Entity" ) >= zCURSOR_SET )
   {
      do
      {
         SetCursorFirstEntityByEntityCsr( vTZEREMDO2, "ER_Entity", vTZEREMDO1, "ER_Entity_Other", 0 );
         if ( CompareAttributeToAttribute( vTZEREMDO1,"ER_Entity", "ZKey", vTZEREMDO2,"ER_Entity", "ZKey" ) != 0 && ERD_IsEntityVisible( vSubtask, "ERD" ) > 0 )
         {
            return;
         }
      } while ( SetCursorNextEntity( vTZEREMDO1, "ER_Entity_Other", "ER_Entity" ) >= zCURSOR_SET );
   }

   SetCursorFirstEntity( vTZEREMDO2, "ER_Entity", 0 );
   do
   {
      if ( CompareAttributeToAttribute( vTZEREMDO1,"ER_Entity", "ZKey", vTZEREMDO2,"ER_Entity", "ZKey" ) != 0 && ERD_IsEntityVisible( vSubtask, "ERD" ) > 0 )
      {
         return;
      }

   } while ( SetCursorNextEntity( vTZEREMDO2, "ER_Entity", 0 ) >= zCURSOR_SET );

   SetCursorFirstEntity( vTZEREMDO2, "ER_Entity", 0 );
   if ( CompareAttributeToAttribute( vTZEREMDO1,"ER_Entity", "ZKey", vTZEREMDO2,"ER_Entity", "ZKey" ) == 0 )
   {
      SetCursorNextEntity( vTZEREMDO2, "ER_Entity", 0 );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_InitTool1
// PURPOSE:    This Entry prepares the environment upon entry
//             to the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_InitTool1( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zLONG  l;
   zPCHAR pchLPLR_Name;

// SfSetApplicationTask( vSubtask, 0 );
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // Choose the proper LPLR to initialize.
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupLPLR_Option", "D" ) == 0 )   // default LPLR
   {
      InitializeLPLR( vSubtask, "" );
   }
   else
   {
      GetAddrForAttribute( &pchLPLR_Name, vProfileXFER, "EMD", "StartupLPLR_Name" );
      InitializeLPLR( vSubtask, pchLPLR_Name );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "ERD_ZoomAlgorithm", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ZoomAlgorithm", 2 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "ERD_ZoomInPercent", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ZoomInPercent", 25 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "ERD_ZoomOutPercent", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ZoomOutPercent", 25 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "ERD_PrintAlgorithm", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_PrintAlgorithm", 1 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "ERD_PrintPercent", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_PrintPercent", 100 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "ERD_PrintOrientation", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_PrintOrientation", 0 );
   }

   if ( GetIntegerFromAttribute( &l, vProfileXFER, "EMD", "ERD_ShowRelOption" ) == -1 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ShowRelOption", 3 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "SA_ZoomAlgorithm", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_ZoomAlgorithm", 2 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "SA_ZoomInPercent", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_ZoomInPercent", 25 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "SA_ZoomOutPercent", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_ZoomOutPercent", 25 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "SA_PrintAlgorithm", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_PrintAlgorithm", 1 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "SA_PrintPercent", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_PrintPercent", 100 );
   }

   if ( CompareAttributeToInteger( vProfileXFER, "EMD", "SA_PrintOrientation", 0 ) == 0 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_PrintOrientation", 0 );
   }

   if ( GetIntegerFromAttribute( &l, vProfileXFER, "EMD", "SA_ShowRelOption" ) == -1 )
   {
      SetAttributeFromInteger( vProfileXFER, "EMD", "SA_ShowRelOption", 3 );
   }

   SetNameForView( vSubtask, "TZEREMDD", vSubtask, zLEVEL_TASK );

   return( 0 );
}

#if 0  // no longer used
void
MigrateER_Model_1_0a1( zVIEW vER_Model, zPCHAR pchNewRelease )
{
   zLONG  lPos;
   zSHORT nRC = SetCursorFirstEntity( vER_Model, "ER_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_Entity", "ER_DiagramPosX" );
      SetAttributeFromInteger( vER_Model, "ER_Entity", "ER_DiagramPosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_Entity", "ER_DiagramPosY" );
      SetAttributeFromInteger( vER_Model, "ER_Entity", "ER_DiagramPosY", lPos * 2 );

      nRC = SetCursorNextEntity( vER_Model, "ER_Entity", 0 );
   }

   nRC = SetCursorFirstEntity( vER_Model, "ER_RelType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_RelType", "ER_DiagramBendPosX" );
      SetAttributeFromInteger( vER_Model, "ER_RelType", "ER_DiagramBendPosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_RelType", "ER_DiagramBendPosY" );
      SetAttributeFromInteger( vER_Model, "ER_RelType", "ER_DiagramBendPosY", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_RelType", "ER_DiagramE1PosX" );
      SetAttributeFromInteger( vER_Model, "ER_RelType", "ER_DiagramE1PosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_RelType", "ER_DiagramE1PosY" );
      SetAttributeFromInteger( vER_Model, "ER_RelType", "ER_DiagramE1PosY", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_RelType", "ER_DiagramE2PosX" );
      SetAttributeFromInteger( vER_Model, "ER_RelType", "ER_DiagramE2PosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vER_Model, "ER_RelType", "ER_DiagramE2PosY" );
      SetAttributeFromInteger( vER_Model, "ER_RelType", "ER_DiagramE2PosY", lPos * 2 );

      nRC = SetCursorNextEntity( vER_Model, "ER_RelType", 0 );
   }

   MiSetOI_ReleaseForView( vER_Model, pchNewRelease );
   nRC = SetCursorFirstEntity( vER_Model, "ER_Entity", 0 );
}

void
MigrateSA_1_0a1( zVIEW vSA, zPCHAR pchNewRelease )
{
   zLONG  lPos;
   zSHORT nRC = SetCursorFirstEntity( vSA, "SA_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lPos, vSA, "SA_Entity", "ER_DiagramPosX" );
      SetAttributeFromInteger( vSA, "SA_Entity", "ER_DiagramPosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vSA, "SA_Entity", "ER_DiagramPosY" );
      SetAttributeFromInteger( vSA, "SA_Entity", "ER_DiagramPosY", lPos * 2 );

      nRC = SetCursorNextEntity( vSA, "SA_Entity", 0 );
   }

   nRC = SetCursorFirstEntity( vSA, "SA_RelType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lPos, vSA, "SA_RelType", "ER_DiagramBendPosX" );
      SetAttributeFromInteger( vSA, "SA_RelType", "ER_DiagramBendPosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vSA, "SA_RelType", "ER_DiagramBendPosY" );
      SetAttributeFromInteger( vSA, "SA_RelType", "ER_DiagramBendPosY", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vSA, "SA_RelType", "ER_DiagramE1PosX" );
      SetAttributeFromInteger( vSA, "SA_RelType", "ER_DiagramE1PosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vSA, "SA_RelType", "ER_DiagramE1PosY" );
      SetAttributeFromInteger( vSA, "SA_RelType", "ER_DiagramE1PosY", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vSA, "SA_RelType", "ER_DiagramE2PosX" );
      SetAttributeFromInteger( vSA, "SA_RelType", "ER_DiagramE2PosX", lPos * 2 );
      GetIntegerFromAttribute( &lPos, vSA, "SA_RelType", "ER_DiagramE2PosY" );
      SetAttributeFromInteger( vSA, "SA_RelType", "ER_DiagramE2PosY", lPos * 2 );

      nRC = SetCursorNextEntity( vSA, "SA_RelType", 0 );
   }

   MiSetOI_ReleaseForView( vSA, pchNewRelease );
   nRC = SetCursorFirstEntity( vSA, "ER_Entity", 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_InitTool2
// PURPOSE:    This Entry switches to a NEW screen if no EMD loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_InitTool2( zVIEW vSubtask )
{
   zVIEW       vTZERSASO;
   zVIEW       vProfileXFER;
   zVIEW       vT = NULL;
   zSHORT      nRC;
   zLONG       lDrawOption;
   zPCHAR      szLastSM;
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   // Always have a Named List of EMD's (even though there can only be One!). This is done to be consistent
   // with other dialogs.  The only time that the named List will not exist is during a SwitchLPLR.  This
   // fact is used a "flag" to the ReturnFromSubWnd logic.
   nRC = RetrieveViewForMetaList( vSubtask, &vT, zREFER_ERD_META );
   if ( vT )
      SetNameForView( vT, "TZERMFLO", vSubtask, zLEVEL_TASK );

   // If there is not an ER in LPLR, go to NEW window.
   if ( GetViewByName( &vT, "TZERMFLO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // Set window Title with check out state
      SetTitleWithCheckOutState( vSubtask, "Data Model", "TZEREMDO", 0, "EntpER_Model", zSOURCE_ERD_META );

      if ( SetCursorFirstEntity( vT, "W_MetaDef", 0 ) < zCURSOR_SET )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZEREMDD", "TZERNWMD" );
         return( 0 );
      }
   }
   else
   {
      // Set window Title with check out state
      SetTitleWithCheckOutState( vSubtask, "Data Model", "TZEREMDO", 0, "EntpER_Model", zSOURCE_ERD_META );
      return( -1 );
   }

   // Always have an ER view available for mapping if this is not the first time that the LPLR is loaded.
   oTZEREMDO_GetUpdViewForER( vSubtask, &vT, zCURRENT_OI );
   if ( vT )
   {
      nRC = SetNameForView( vT, "TZEREMDO", vSubtask, zLEVEL_TASK );
      nRC = SetCursorFirstEntity( vT, "ER_Entity", "" );
      //BL, 1999.10.18
      SetSelectStateOfEntity( vT, "ER_Entity", 1 );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Data Model", "TZEREMDO", vT, "EntpER_Model", zSOURCE_ERD_META );

   // Set drawing option from profile
   GetIntegerFromAttribute( &lDrawOption, vProfileXFER, "EMD", "ERD_ShowRelOption" );
   SetDrawOptions( vSubtask, "ERD", (zUSHORT) lDrawOption );

   zwTZEREMDD_ZoomDiagram( vSubtask );

   // Check previous state for each possible startup option
   // Identifier startup - show window if it was there on last exit
   //                      (also reset size to "default" for the next time
   //                      the window is shown if startup option is "No")
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupIdOption", "Y" ) == 0 )
   {
      if ( CompareAttributeToString( vProfileXFER, "EMD", "IdView", "Y" ) == 0 )   //test only
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow | zWAB_ProcessImmediateAction, "TZERIDND", "IDVIEW" );
      }
   }
   else
      SetAttributeFromInteger( vProfileXFER, "IDVIEW", "SZDLG_X", 0 );

   // Attribute startup - show window if it was there on last exit
   //                     (also reset size to "default" for the next time
   //                     the window is shown if startup option is "No")
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupAttrOption", "Y" ) == 0 )
   {
      if ( CompareAttributeToString( vProfileXFER, "EMD", "AttrView", "Y" ) == 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow | zWAB_ProcessImmediateAction, "TZERATRD", "ATTVIEW" );
      }
   }
   else
      SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "SZDLG_X", 0 );

   // Entity view startup - show window if it was there on last exit
   //                       (also reset size to "default" for the next time
   //                       the window is shown if startup option is "No")
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupEntOption", "Y" ) == 0 )
   {
      if ( CompareAttributeToString( vProfileXFER, "EMD", "EntView", "Y" ) == 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow | zWAB_ProcessImmediateAction, "TZEREMDD", "ENTVIEW" );
      }
   }
   else
      if ( CheckExistenceOfEntity( vProfileXFER, "ENTVIEW" ) >= zCURSOR_SET )
      {
         SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "SZDLG_X", 0 );
      }

   // Relationship startup - show window if it was there on last exit
   //                        (also reset size to "default" for the next time
   //                        the window is shown if startup option is "No")
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupRelOption", "Y" ) == 0 )
   {
      if ( CompareAttributeToString( vProfileXFER, "EMD", "RelView", "Y" ) == 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow | zWAB_ProcessImmediateAction, "TZERRELD", "RLLVIEW" );
      }
   }
   else
      SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "SZDLG_X", 0 );

   // Model startup - show window as full size or comressed based on the way it was on last exit.
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupEMD_Option", "Y" ) == 0 )
   {
      if ( CompareAttributeToString( vProfileXFER, "EMD", "EDM_View", "Y" ) == 0 )
      {
         SetAttributeFromString( vProfileXFER, "EMD", "EDM_View", "N" );
      }
      else
      {
         SetAttributeFromString( vProfileXFER, "EMD", "EDM_View", "Y" );
      }

      zwTZEREMDD_ViewModel( vSubtask );
   }
   else
   {
      SetAttributeFromString( vProfileXFER, "EMD", "EDM_View", "Y" );
      SetOptionState( vSubtask, "ViewModel", zOPTION_STATUS_CHECKED, 1 );
   }

   // Submodel startup - if the Re-establish indicator is set then the last
   //                    submodel active is re-activated.  if the submodel
   //                    window was active (at last exit), then the window is shown.
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupSM_Option", "Y" ) == 0 )
   {
      GetAddrForAttribute( &szLastSM, vProfileXFER, "EMD", "LastSubmodel" );
      ActivateMetaOI_ByName( vSubtask, &vTZERSASO, 0, zSOURCE_SA_META, zSINGLE | zLEVEL_APPLICATION, szLastSM, zCURRENT_OI );
      if ( vTZERSASO )
      {
         SetNameForView( vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
         if ( CompareAttributeToString( vProfileXFER, "EMD", "SM_View", "Y" ) == 0 )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow, "TZERSAMD", "SA_Diagram" );
            return( 0 );
         }
      }
   }
   else
      SetAttributeFromInteger( vProfileXFER, "Submodel", "SZDLG_X", 0 );

   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   // if attribute sequence does not exist, set it and remove update flag
   oTZEREMDO_SetAttributeSequence( vT );
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vT->hViewCsr );
   lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   lpViewOI->bUpdatedFile = FALSE;

   // if ERD not checked out, set Data Model View (TZEREMDO) read only ->
   // the user cannot update the values in Detail Windows (for example Window Entity Detail)
   if ( !ComponentIsCheckedOut( vSubtask, vT, zSOURCE_ERD_META ))
   {
      SetViewReadOnly( vT );
      EnableAction( vSubtask, "DeleteEntity", FALSE );
   }
   else
      EnableAction( vSubtask, "DeleteEntity", TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_InitMenuForSA
//
// enable or disable menu items for Subject Area
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_InitMenuForSA( zVIEW vSubtask )
{
   zVIEW  vTZERMFLO  = NULL;
   zVIEW  vTZERMFLO1 = NULL;
   zVIEW  vTZEREMDO  = NULL;
   zVIEW  vSA;
   zSHORT nEnable = 0;
   zSHORT nSAIsCheckedOut = 0;

   if ( GetViewByName( &vTZERMFLO, "TZERMFLO", vSubtask, zLEVEL_TASK ) >= 0 )
      CreateViewFromViewForTask( &vTZERMFLO1, vTZERMFLO, 0 );

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
      nSAIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META );

   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, nSAIsCheckedOut );

   // if Data Model committed?
   if ( vTZERMFLO1 && SetCursorFirstEntityByInteger( vTZERMFLO1, "W_MetaType", "Type", zSOURCE_ERD_META, "" ) >= zCURSOR_SET && CheckExistenceOfEntity( vTZERMFLO1, "W_MetaDef" ) >= zCURSOR_SET )
   {
      SetOptionState( vSubtask, "NewSA", zOPTION_STATUS_ENABLED, nSAIsCheckedOut );
   }

   //BL, 1999.10.15 menu <File>, menu items for Subject Area
   if ( GetViewByName( &vSA, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 )
      nEnable = 1;
   else
      nSAIsCheckedOut = 0;

   SetOptionState( vSubtask, "SubjectAreaOpt", zOPTION_STATUS_ENABLED, nSAIsCheckedOut );
   SetOptionState( vSubtask, "CloseSA", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "SaveSA", zOPTION_STATUS_ENABLED, nSAIsCheckedOut );
   SetOptionState( vSubtask, "SaveAS_SA", zOPTION_STATUS_ENABLED, nSAIsCheckedOut );
   SetOptionState( vSubtask, "PropertiesSA", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "PrintSA", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "PrintSAReport", zOPTION_STATUS_ENABLED, nEnable );

   // menu item <Open Subject Area> and <New Subject Area>
   if ( vTZERMFLO1 && SetCursorFirstEntityByInteger( vTZERMFLO1, "W_MetaType", "Type", zREFER_SA_META, "" ) >= zCURSOR_SET && CheckExistenceOfEntity( vTZERMFLO1, "W_MetaDef" ) >= zCURSOR_SET )
   {
      SetOptionState( vSubtask, "OpenSA", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( vTZERMFLO1 )
      DropView( vTZERMFLO1 );

   return( 0 );
} // zwTZEREMDD_InitMenuForSA

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_InitMenuForDeleteAll
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_InitMenuForDeleteAll( zVIEW  vSubtask, zVIEW  vTZEREMDO, zSHORT nERD_IsCheckedOut )
{
   zSHORT nRC;
   zBOOL  bSAP_Entity = FALSE;
   zSHORT nDeleteAllRelFromSelEntities = 0;
   zSHORT nDeleteAllAttFromSelEntities = 0;
   zSHORT nDeleteAllIdFromSelEntities  = 0;

   //menu item <Delete all from selected Entities>
   if ( nERD_IsCheckedOut )
   {
      nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", "" );

      while ( nRC >= zCURSOR_SET && ( !nDeleteAllRelFromSelEntities || !nDeleteAllAttFromSelEntities || !nDeleteAllIdFromSelEntities || !bSAP_Entity ))
      {
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink" ) >= zCURSOR_SET )
            nDeleteAllRelFromSelEntities = 1;

         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) >= zCURSOR_SET )
            nDeleteAllAttFromSelEntities = 1;

         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_EntIdentifier" ) >= zCURSOR_SET )
            nDeleteAllIdFromSelEntities = 1;

         if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 )
            bSAP_Entity = TRUE;

         nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", "" );
      }

      SetOptionState( vSubtask, "DeleteAllRelationships", zOPTION_STATUS_ENABLED, nDeleteAllRelFromSelEntities );
      SetOptionState( vSubtask, "DeleteAllAttributes", zOPTION_STATUS_ENABLED, nDeleteAllAttFromSelEntities );
      SetOptionState( vSubtask, "DeleteAllIdentifiers", zOPTION_STATUS_ENABLED, nDeleteAllIdFromSelEntities );

      // if SAP Entity, do not delete Identifiers and Relationships
      if ( bSAP_Entity )
      {
         SetOptionState( vSubtask, "DeleteAllIdentifiers", zOPTION_STATUS_ENABLED, 0 );
         SetOptionState( vSubtask, "DeleteAllRelationships", zOPTION_STATUS_ENABLED, 0 );
      }
   }

   return( 0 );
} // zwTZEREMDD_InitMenuForDeleteAll

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DisableMenuItems
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_DisableMenuItems( zVIEW vSubtask )
{
   SetOptionState( vSubtask, "NewSA", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "OpenSA", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddRelationship", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddAttribute", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddIdentifier", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateRelationship", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateAttribute", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateIdentifier", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedEntities", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedRelationship", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedIdentifiers", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllRelationships", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllIdentifiers", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SelectRelatedEntities", zOPTION_STATUS_ENABLED, 0 );

   return( 0 );
} // zwTZEREMDD_DisableMenuItems

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_InitMenuForEdit_RS
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_InitMenuForEdit_RS( zVIEW  vSubtask, zVIEW  vTZEREMDO, zSHORT nERD_IsCheckedOut )
{
   zVIEW  vTZEREMDO1;

   //menu item <Update Relationship> and <Delete Selected Relationship>

   if ( GetSelectStateOfEntity( vTZEREMDO, "ER_RelLink" ) == 1  || GetSelectStateOfEntity( vTZEREMDO, "ER_RelType" ) == 1 )
   {
      SetOptionState( vSubtask, "SelectRelatedEntities", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "UpdateRelationship", zOPTION_STATUS_ENABLED, 1 );

      if ( nERD_IsCheckedOut )
      {
         CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );

         // if Relationship between 2 SAP Entities, disable "Delete" Option
         if ( SetCursorFirstEntityByAttr( vTZEREMDO1, "ER_RelType_1", "ZKey", vTZEREMDO1, "ER_RelType", "ZKey", "EntpER_Model" ) >= zCURSOR_SET &&
              (CompareAttributeToInteger( vTZEREMDO1, "ER_Entity", "SAP_Entity", 1 ) == 0 || CompareAttributeToInteger( vTZEREMDO1, "ER_Entity_Other", "SAP_Entity", 1 ) == 0) )
         {
            SetOptionState( vSubtask, "DeleteSelectedRelationship", zOPTION_STATUS_ENABLED, 0 );
         }
         else
         {
            SetOptionState( vSubtask, "DeleteSelectedRelationship", zOPTION_STATUS_ENABLED, 1 );
         }

         DropView( vTZEREMDO1 );
      }
   }

   return( 0 );
} // zwTZEREMDD_InitMenuForEdit_RS

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_InitMenuForEdit_ID
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_InitMenuForEdit_ID( zVIEW  vSubtask, zVIEW  vTZEREMDO, zSHORT nERD_IsCheckedOut, zSHORT nShowIdentifierUpdate )
{
   zSHORT nEnable = 0;

   //menu item <Update Identifier> and <Delete Selected Identifiers>
   if ( nShowIdentifierUpdate )
   {
      if ( GetSelectStateOfEntity ( vTZEREMDO, "ER_EntIdentifier" ) == 1 )
         nEnable = 1;
   }

   SetOptionState( vSubtask, "UpdateIdentifier", zOPTION_STATUS_ENABLED, nEnable );

   // if SAP Entity, do not delete Identifier
   if ( nERD_IsCheckedOut && CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) != 0 )
   {
      SetOptionState( vSubtask, "DeleteSelectedIdentifiers", zOPTION_STATUS_ENABLED, nEnable );
   }

   return( 0 );
} // zwTZEREMDD_InitMenuForEdit_ID

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_InitMenuForEditItems
//
// enable or disable Edit menu items
//
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZEREMDD_InitMenuForEditItems( zVIEW vSubtask, zSHORT nShowAttributeUpdate, zSHORT nShowIdentifierUpdate)
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDO1;
   zSHORT nRC;
   zSHORT nEnable = 0;
   zSHORT nERD_IsCheckedOut = 0;

   // Disable all Menu Items
   zwTZEREMDD_DisableMenuItems( vSubtask );

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );
   nERD_IsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META );

   //menu items for Add
   if ( nERD_IsCheckedOut )
   {
      SetOptionState( vSubtask, "AddEntity", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddAttribute", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DeleteSelectedEntities", zOPTION_STATUS_ENABLED, 1 );

      // if SAP Entity, do not added Identifier
      if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) != 0 )
      {
         SetOptionState( vSubtask, "AddIdentifier", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "AddRelationship", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   //menu item <Update Relationship> and <Delete Selected Relationship>
   zwTZEREMDD_InitMenuForEdit_RS( vSubtask, vTZEREMDO, nERD_IsCheckedOut );

   //menu item <Update Attribute> and <Delete Selected Attributes>
   if ( nShowAttributeUpdate )
   {
      nRC = SetCursorFirstSelectedEntity( vTZEREMDO1, "ER_Attribute", 0 );
      if ( nRC == zCURSOR_SET )
      {
         SetOptionState( vSubtask, "UpdateAttribute", zOPTION_STATUS_ENABLED, 1 );
         SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Attribute", vTZEREMDO1, "ER_Attribute", 0 );

         if ( nERD_IsCheckedOut )
            SetOptionState( vSubtask, "DeleteSelectedAttributes", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   //menu item <Update Identifier> and <Delete Selected Identifiers>
   zwTZEREMDD_InitMenuForEdit_ID( vSubtask, vTZEREMDO, nERD_IsCheckedOut, nShowIdentifierUpdate );

   //menu item <Delete all from selected Entities>
   zwTZEREMDD_InitMenuForDeleteAll( vSubtask, vTZEREMDO1, nERD_IsCheckedOut );

   DropView( vTZEREMDO1 );
   return( 0 );
} // zwTZEREMDD_InitMenuForEditItems

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_InitMenuUtilities
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zPCHAR /*LOCAL */  OPERATION
zwTZEREMDD_InitMenuUtilities( zVIEW vSubtask )
{
   zVIEW  vTZERMFLO;
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDO_Copy;
   zSHORT nERD_IsCheckedOut = 0;
   zSHORT nImport           = 1;
   zSHORT nSAP_BuildTE      = 0;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nERD_IsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META );
      if ( nERD_IsCheckedOut == 0 )
         nImport = 0;
      else
      {
         CreateViewFromViewForTask( &vTZEREMDO_Copy, vTZEREMDO, 0 );
         if ( SetCursorFirstEntityByInteger( vTZEREMDO_Copy, "ER_Entity", "SAP_Entity", 1, "" ) >= zCURSOR_SET )
         {
            nSAP_BuildTE = 1;
         }
         DropView( vTZEREMDO_Copy );
      }
   }

   if ( GetViewByName( &vTZERMFLO, "TZERMFLO", vSubtask, zLEVEL_TASK ) < 0 )
      nImport = 0;

   SetOptionState( vSubtask, "ImportSiron", zOPTION_STATUS_ENABLED, nImport );
   SetOptionState( vSubtask, "ImportADW", zOPTION_STATUS_ENABLED, nImport );
   SetOptionState( vSubtask, "SAP_Tables", zOPTION_STATUS_ENABLED, nImport );
   SetOptionState( vSubtask, "SAP_RS_and_TE", zOPTION_STATUS_ENABLED, nSAP_BuildTE );
   SetOptionState( vSubtask, "CreateGenkey", zOPTION_STATUS_ENABLED, nERD_IsCheckedOut );
   SetOptionState( vSubtask, "CreateDynamicDomain", zOPTION_STATUS_ENABLED, nERD_IsCheckedOut );
   SetOptionState( vSubtask, "CreateLocking", zOPTION_STATUS_ENABLED, nERD_IsCheckedOut );

   return( 0 );
} // zwTZEREMDD_InitMenuUtilities

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_InitMenu
// PURPOSE:    This Entry ssets menu graying
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_InitMenu( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vProfileXFER;
   zLONG  lShowRelOption;
   zSHORT nEnable;
   zSHORT nShowAttributeUpdate    = 0;
   zSHORT nShowIdentifierUpdate   = 0;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   nEnable = 0;
   if ( GetViewByName( &vWindow, "ENTVIEW", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      nEnable = 1;
      nShowAttributeUpdate = 1;
   }

   SetOptionState( vSubtask, "ViewEntitys", zOPTION_STATUS_CHECKED, nEnable );

   nEnable = 0;
   if ( GetViewByName( &vWindow, "ATTVIEW", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      nEnable = 1;
      nShowAttributeUpdate = 1;
   }

   SetOptionState( vSubtask, "ViewAttributes", zOPTION_STATUS_CHECKED, nEnable );

   nEnable = 0;
   if ( GetViewByName( &vWindow, "RLLVIEW", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      nEnable = 1;
   }

   SetOptionState( vSubtask, "ViewRels", zOPTION_STATUS_CHECKED, nEnable );

   nEnable = 0;
   if ( GetViewByName( &vWindow, "IDVIEW", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      nEnable = 1;
      nShowIdentifierUpdate = 1;
   }

   SetOptionState( vSubtask, "ViewIdent", zOPTION_STATUS_CHECKED, nEnable );

   //BL, 1999.10.16 menu item <Show Line and Text>, <Show line only>, <Hide Line>
   SetOptionState( vSubtask, "ShowLineAndText", zOPTION_STATUS_CHECKED, 0 );
   SetOptionState( vSubtask, "ShowLineOnly", zOPTION_STATUS_CHECKED, 0 );
   SetOptionState( vSubtask, "HideLine", zOPTION_STATUS_CHECKED, 0 );

   GetIntegerFromAttribute( &lShowRelOption, vProfileXFER, "EMD", "ERD_ShowRelOption" );
   if ( lShowRelOption == 3 )
      SetOptionState( vSubtask, "ShowLineAndText", zOPTION_STATUS_CHECKED, 1 );
   else
   {
      if ( lShowRelOption == 1 )
         SetOptionState( vSubtask, "ShowLineOnly", zOPTION_STATUS_CHECKED, 1 );
      else
         if ( lShowRelOption == 0 )
            SetOptionState( vSubtask, "HideLine", zOPTION_STATUS_CHECKED, 1 );
   }

   //BL, 1999.10.18 menu <Edit>
   zwTZEREMDD_InitMenuForEditItems( vSubtask, nShowAttributeUpdate, nShowIdentifierUpdate );

   //BL, 1999.10.16 menu item for Subject Area
   zwTZEREMDD_InitMenuForSA( vSubtask );

   // menu <Utilities>
   zwTZEREMDD_InitMenuUtilities( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_HelpAbout
// PURPOSE:    This Entry does the About
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_ERD, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_HelpVML
// PURPOSE:    This Entry transfers to VML help
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HelpVML( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, PROG_REF );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_HelpCoreOprs
// PURPOSE:    This Entry transfers to VML help
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HelpCoreOprs( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, OPER_REF );
   return( 0 );
}

#if 0
/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_CloseSAP_ErrorWindows
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseSAP_ErrorWindows( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vView;

   // Close ERD Error List Window
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ERD_ErrorList" ) >= 0 )
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );

   // Drop View from ERD Error List Windwo
   if ( GetViewByName( &vView, "TZERDERR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   // Close SAP Windows
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 )
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SAP_Import" ) >= 0 )
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );

   // Drop View from SAP Import
   if ( GetViewByName( &vView, "TZSAPERR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZSapRel", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   zwTZERSAPD_ExitImport( vSubtask );

   return( 0 );
} // zwTZEREMDD_CloseSAP_ErrorWindows
#endif

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_MainRtnFromSubWnd
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated the current EMD.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW       vT = NULL;
   zSHORT      nRC;
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   // If ERM List is not available, get one and open the model if it exists
   nRC = GetViewByName( &vT, "TZERMFLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      nRC = RetrieveViewForMetaList( vSubtask, &vT, zREFER_ERD_META );
      if ( vT )
         nRC = SetNameForView( vT, "TZERMFLO", vSubtask, zLEVEL_TASK );

      // If there is not an ER in LPLR, go to NEW window.
      if ( GetViewByName( &vT, "TZERMFLO", vSubtask, zLEVEL_TASK ) < 0 )
      {
         zwTZEREMDD_CloseModelessWindows( vSubtask );
         SetTitleWithCheckOutState( vSubtask, "Data Model", "TZEREMDO", 0, "EntpER_Model", zSOURCE_ERD_META );
         return( -1 );
      }
      else
      if ( SetCursorFirstEntity( vT, "W_MetaDef", 0 ) < zCURSOR_SET )
      {
         zwTZEREMDD_CloseModelessWindows( vSubtask );
         zwTZEREMDD_ZoomDiagram( vSubtask );
         SetTitleWithCheckOutState( vSubtask, "Data Model", "TZEREMDO", 0, "EntpER_Model", zSOURCE_ERD_META );
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZEREMDD", "TZERNWMD" );
         return( 0 );
      }

      // Re-Establish the EMD view.  If a view cannot be found, there is a serious error. Show the LPLR window
      nRC = GetViewByName( &vT, "TZEREMDO", vSubtask, zLEVEL_TASK );
      if ( vT == 0 )
      {
         oTZEREMDO_GetUpdViewForER( vSubtask, &vT, zCURRENT_OI );
         if ( vT )
         {
            // Data Model exists, enable ERD Control
            SetCtrlState( vSubtask, "ERD", zCONTROL_STATUS_ENABLED, TRUE );

            SetNameForView( vT, "TZEREMDO", vSubtask, zLEVEL_TASK );
            // Set window Title with check out state
            SetTitleWithCheckOutState( vSubtask, "Data Model", "TZEREMDO", vT, "EntpER_Model", zSOURCE_ERD_META );
            zwTZEREMDD_ZoomDiagram( vSubtask );
            // if attribute sequence does not exist, set it and remove update flag
            oTZEREMDO_SetAttributeSequence( vT );
            lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vT->hViewCsr );
            lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
            lpViewOI->bUpdatedFile = FALSE;

#if 0
            // Close SAP and Error Windows
            zwTZEREMDD_CloseSAP_ErrorWindows( vSubtask );
#endif
             // if ERD not checked out, set Data Model View (TZEREMDO) read only ->
            // the user cannot update the values in Detail Windows (for example Window Entity Detail)
            if ( !ComponentIsCheckedOut( vSubtask, vT, zSOURCE_ERD_META ))
            {
               SetViewReadOnly( vT );
               EnableAction( vSubtask, "DeleteEntity", FALSE );
               zwTZEREMDD_DisableDeleteActions( vSubtask, 0 );
            }
            else
            {
               EnableAction( vSubtask, "DeleteEntity", TRUE );
               zwTZEREMDD_DisableDeleteActions( vSubtask, 1 );
            }

            zwTZEREMDD_RefreshEntitySelect( vSubtask );
         }
         else
         {
            zwTZEREMDD_CloseModelessWindows( vSubtask );
            zwTZEREMDD_ZoomDiagram( vSubtask );
            zwTZEREMDD_SwitchLPLR( vSubtask );
            SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZCMSLPD", "SwitchLPLR");
            return( 0 );
         }
      }
   }

//BL RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CreateNewModel
// PURPOSE:   Activate an Empty ER Model and initialize with a root
//            instance.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_New...
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CreateNewModel( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vT;
   zSHORT nRC;

   // Get Access to current Enterprise Model Object and prompt for Save
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( zwTZEREMDD_AskForSave( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // Activate an empty Enterprise Model Object
   nRC = ActivateEmptyMetaOI( vSubtask, &vTZEREMDO, zSOURCE_ERD_META, zSINGLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Activate empty Data Model." );
      MessageSend( vSubtask, "ER00101", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Build Root Instance
   CreateMetaEntity( vSubtask, vTZEREMDO, "EntpER_Model", zPOS_AFTER );
   GetViewByName( &vT, "TaskLPLR", vSubtask, zLEVEL_TASK );
   SetAttributeFromAttribute( vTZEREMDO, "EntpER_Model", "Name", vT, "LPLR", "Name" );
   SetAttributeFromAttribute( vTZEREMDO, "EntpER_Model", "Desc", vT, "LPLR", "Desc" );
   SetNameForView( vTZEREMDO, "newTZEREMDO", vSubtask, zLEVEL_TASK );

   // Add a single entity to mode.  The model must always have one entity.
   zwfnTZEREMDD_AddEntity( vSubtask, vTZEREMDO );

   SetAttributeFromAttribute( vTZEREMDO, "ER_Entity", "Name", vT, "LPLR", "Name" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CancelNewModel
// PURPOSE:   Cancel the create of a new ER model.  Because one is
//            initially created we must first delete and then return.
// TRIGGERED BY:
//    WINDOW: TZERNWMD
//    ACTION: BUTTON
//    NAME: &Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CancelNewModel( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vParentWindow;
   zSHORT nRC;

   // Get Access to New Enterprise Model Object
   nRC = GetViewByName( &vTZEREMDO, "newTZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access new Data Model. Model must be opened to Cancel New." );
      MessageSend( vSubtask, "ER00102", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Drop skeleton instance created on New action
   DropMetaOI( vSubtask, vTZEREMDO );

   // Data Model does not exists, disable ERD Control
   GetParentWindow( &vParentWindow, vSubtask );
   SetCtrlState( vParentWindow, "ERD", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_OKNewModel
// PURPOSE:   OK the create of a new ER model.  Because one is
//            initially created we must rename the view and return.
// TRIGGERED BY:
//    WINDOW: TZERNWMD
//    ACTION: BUTTON
//    NAME: &Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_OKNewModel( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vW;

   // Get Access to New Enterprise Model Object.
   GetViewByName( &vTZEREMDO, "newTZEREMDO", vSubtask, zLEVEL_TASK );
   if ( !vTZEREMDO )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access new Data Model. Model must be opened to Cancel New." );
      MessageSend( vSubtask, "ER00103", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Make new ER current in mapping view.
   SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vW, "Data Model", "TZEREMDO", vTZEREMDO, "EntpER_Model", zSOURCE_ERD_META );

   zwfnTZEREMDD_PositionNonPosEnts( vW, vTZEREMDO );

   // Data Model exists, enable ERD Control
   SetCtrlState( vW, "ERD", zCONTROL_STATUS_ENABLED, TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_WriteErrorMsg
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_WriteErrorMsg( zVIEW  vSubtask, zVIEW  vERD, zPCHAR szErrorMsg, zSHORT nError )
{
   zVIEW  vTZERDERR;
   zCHAR  szMsg[ 255 ];

   if ( vERD == 0 )
      return( -1 );

   if ( GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZEREMDD_LoadErrorList( vSubtask );
      GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK );
   }

   CreateEntity( vTZERDERR, "ErrorMsg", zPOS_LAST );
   SetAttributeFromAttribute( vTZERDERR, "ErrorMsg", "EntityName", vERD, "ER_Entity", "Name" );
   if ( nError == 1 )
      strcpy_s( szMsg, zsizeof( szMsg ), "Error: " );
   else
   if ( nError == 0 )
      strcpy_s( szMsg, zsizeof( szMsg ), "Warning: " );

   strcat_s( szMsg, zsizeof( szMsg ), szErrorMsg );

   SetAttributeFromString( vTZERDERR, "ErrorMsg", "ErrorText", szMsg );
   SetAttributeFromInteger( vTZERDERR, "ErrorMsg", "Error", nError );

   return( 0 );
} // zwTZEREMDD_WriteErrorMsg

static void
zwfnTZEREMDD_CheckRelLinkIdentifier( zVIEW vSubtask, zVIEW vERD2 )
{
   zSHORT nRC;
   zCHAR  szName[ 33 ];

   nRC= SetCursorFirstEntity( vERD2, "ER_EntIdentifier", "EntpER_Model" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vERD2, "ER_FactType" ) < zCURSOR_SET )
      {
         strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Identifier '" );
         GetStringFromAttribute( szName, zsizeof( szName ), vERD2, "ER_EntIdentifier", "Name" );
         strcat_s( szWorkMessage, zsizeof( szWorkMessage ), szName );
         strcat_s( szWorkMessage, zsizeof( szWorkMessage ), "' does not have an Attribute or Relationship assigned. " );
         zwTZEREMDD_WriteErrorMsg( vSubtask, vERD2, szWorkMessage, 0 );
      }
      else
      {
         // Delete any dangling ER_FactTypes (FactTypes without children). There
         // seems to be a bug in the system that we can't duplicate where
         // dangling FactTypes are created. 2/12/97 DonC
         // Also make sure there are not two ER_RelLinkIdentifier entities under
         // one ER_FactType. This could occur after migration if there were
         // duplicate ZKeys in the original ER.

         nRC= SetCursorFirstEntity( vERD2, "ER_FactType", "" );
         while ( nRC >= zCURSOR_SET )
         {
            // Delete Dangling ER_FactTypes.
            if ( CheckExistenceOfEntity( vERD2, "ER_AttributeIdentifier" ) < zCURSOR_SET && CheckExistenceOfEntity( vERD2, "ER_RelLinkIdentifier" ) < zCURSOR_SET )
            {
               DeleteEntity( vERD2, "ER_FactType", zREPOS_NONE );
            }

            // Delete extra ER_RelLinkIdentifier entries.
            if ( CheckExistenceOfEntity( vERD2, "ER_RelLinkIdentifier" ) >= zCURSOR_SET )
            {
               nRC = SetCursorNextEntity( vERD2, "ER_RelLinkIdentifier", 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  zwTZEREMDD_WriteErrorMsg( vSubtask, vERD2, "Deleting extra ER_RelLinkIdentifier entry.", 0 );
                  ExcludeEntity( vERD2, "ER_RelLinkIdentifier", zREPOS_NONE );
                  nRC = SetCursorNextEntity( vERD2, "ER_RelLinkIdentifier", 0 );
               }
            }

            nRC= SetCursorNextEntity( vERD2, "ER_FactType", "" );
         } // end while ( nRC >= zCURSOR_SET ) for ER_FactType
      } // end else if ( CheckExistenceOfEntity( vERD2, "ER_FactType" ) < zCURSOR_SET )

      nRC= SetCursorNextEntity( vERD2, "ER_EntIdentifier", "EntpER_Model" );
   } // endwhile ( nRC >= zCURSOR_SET ) for ER_FactType
}

static void
zwfnTZEREMDD_CheckDomains( zVIEW vSubtask, zVIEW vERD2 )
{
   zCHAR  szName[ 33 ];
   zSHORT nRC;

   // Make sure that nothing has happened so that we have ER_Attribute entities
   // without Domains.

   nRC= SetCursorFirstEntity( vERD2, "ER_Attribute", "EntpER_Model" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vERD2, "Domain" ) < zCURSOR_SET )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vERD2, "ER_Attribute", "Name" );
         strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Attribute '" );
         strcat_s( szWorkMessage, zsizeof( szWorkMessage ), szName );
         strcat_s( szWorkMessage, zsizeof( szWorkMessage ), "' does not have Domain assigned." );
         zwTZEREMDD_WriteErrorMsg( vSubtask, vERD2, szWorkMessage, 1 );
      }
      nRC= SetCursorNextEntity( vERD2, "ER_Attribute", "EntpER_Model" );
   }
}

static void
zwfnTZEREMDD_CheckRelationships( zVIEW vSubtask, zVIEW vERD2 )
{
   zVIEW  vERD3;
   zSHORT nRC;
   zPCHAR pchEntityName1;
   zPCHAR pchEntityName2;
   zPCHAR pchRelName1;
   zPCHAR pchRelName2;

   // Make sure that there are no duplicate relationships for an entity.
   // First loop through all the ER_Entities.
   nRC = SetCursorFirstEntity( vERD2, "ER_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // Next loop through all the ER_RelLinks within an ER_Entity.
      nRC = SetCursorFirstEntity( vERD2, "ER_RelLink", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetAddrForAttribute( &pchEntityName1, vERD2, "ER_Entity", "Name" );
         GetAddrForAttribute( &pchRelName1, vERD2, "ER_RelLink", "Name" );
         GetAddrForAttribute( &pchRelName2, vERD2, "ER_RelLink_Other", "Name" );
         GetAddrForAttribute( &pchEntityName2, vERD2, "ER_Entity_Other", "Name" );

         CreateViewFromViewForTask( &vERD3, vERD2, 0 );

         // Finally loop through the rest of the ER_RelLinks under the ER_Entity.
         nRC = SetCursorNextEntity( vERD3, "ER_RelLink", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            if ( (CompareAttributeToString( vERD3, "ER_Entity", "Name", pchEntityName1 ) == 0) &&
                 (CompareAttributeToString( vERD3, "ER_RelLink", "Name", pchRelName1 ) == 0) &&
                 (CompareAttributeToString( vERD3, "ER_RelLink_Other", "Name", pchRelName2 ) == 0) &&
                 (CompareAttributeToString( vERD3, "ER_Entity_Other", "Name", pchEntityName2) == 0) )
            {
               strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Duplicate Relationship: '" );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), pchEntityName1 );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), " " );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), pchRelName1 );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), " " );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), pchEntityName2 );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), "', '" );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), pchEntityName2 );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), " " );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), pchRelName2 );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), " " );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), pchEntityName1 );
               strcat_s( szWorkMessage, zsizeof( szWorkMessage ), "'." );
               zwTZEREMDD_WriteErrorMsg( vSubtask, vERD2, szWorkMessage, 1 );
            }

            nRC = SetCursorNextEntity( vERD3, "ER_RelLink", 0 );
         }

         DropView( vERD3 );
         nRC = SetCursorNextEntity( vERD2, "ER_RelLink", 0 );
      }

      nRC = SetCursorNextEntity( vERD2, "ER_Entity", 0 );
   }
}

static zSHORT
zwfnTZEREMDD_LoadErrorList( zVIEW vSubtask )
{
   zVIEW  vTZERDERR;
   zVIEW  vWindow;

   if ( GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vTZERDERR, "ErrorMsg" ) >= zCURSOR_SET )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ProcessImmediateAction | zWAB_StartOrFocusModelessSubwindow, "TZEREMDD", "ERD_ErrorList" );
         GetSubtaskForWindowName( vSubtask, &vWindow, "ERD_ErrorList" );
         zwTZEREMDD_RefreshErrorList( vWindow );

         // if Save with Errors?
         if ( SetCursorFirstEntityByInteger( vTZERDERR, "ErrorMsg", "Error", 1, "" ) >= zCURSOR_SET )
         {
            return( 1 );
         }
      }
      else
      {
         if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ERD_ErrorList" ) >= 0 )
            InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );

         DropObjectInstance( vTZERDERR );
      }
   }

   return( 0 );
}

static zSHORT
zwfnTZEREMDD_CommitERD( zVIEW vSubtask, zVIEW vTZEREMDO )
{
   zVIEW  vDTE;

   if ( CommitMetaOI( vSubtask, vTZEREMDO, zSOURCE_ERD_META ) <= 0 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to save Data Model." );
      MessageSend( vSubtask, "ER00107", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   {
      // if the "Model Physical Mapping" TE doesn't exist create it
      RetrieveViewForMetaList( vSubtask, &vDTE, zREFER_DTE_META );
      if ( SetCursorFirstEntity( vDTE, "W_MetaDef", "" ) < zCURSOR_SET )
      {
         oTZTENVRO_GetUpdViewForDTE_P( vSubtask, &vDTE );

         if ( !vDTE )
         {
            strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Activate empty Physical Model." );
            MessageSend( vSubtask, "ER00108", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -2 );
         }
         else
         {
            CommitMetaOI( vSubtask, vDTE, zSOURCE_DTE_META );
            DropMetaOI( vSubtask, vDTE );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SaveModel
// PURPOSE:   Save ER Model through a Commit.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_Save
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SaveModel( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vERD2;
   zCHAR  szTimestamp[ 33 ];

   // Get Access to Enterprise Model Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Make sure object is not empty
   if ( CheckExistenceOfEntity( vTZEREMDO, "EntpER_Model" ) < zCURSOR_SET )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Data Model is empty and cannot be Saved." );
      MessageSend( vSubtask, "ER00106", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   zwTZEREMDD_CreateNewErrorRoot( vSubtask );
   CreateViewFromViewForTask( &vERD2, vTZEREMDO, 0 );

   // Delete any dangling ER_FactTypes (FactTypes without children). There
   // seems to be a bug in the system that we can't duplicate where
   // dangling FactTypes are created. 2/12/97 DonC
   // Also make sure there are not two ER_RelLinkIdentifier entities under
   // one ER_FactType. This could occur after migration if there were
   // duplicate ZKeys in the original ER.
   zwfnTZEREMDD_CheckRelLinkIdentifier( vSubtask, vERD2 );

   // Make sure that nothing has happened so that we have ER_Attribute entities without Domains.
   zwfnTZEREMDD_CheckDomains( vSubtask, vERD2 );

   // Make sure that there are no duplicate relationships for an entity.  First loop through all the ER_Entities.
   zwfnTZEREMDD_CheckRelationships( vSubtask, vERD2 );

   DropView( vERD2 );

   // if Errors?
   if ( zwfnTZEREMDD_LoadErrorList( vSubtask ) == 1 )
      return( -1 );

   // Save instance to file/database
   // Now simply position the non positioned Entity if update allowed
   if ( MiGetUpdateForView( vTZEREMDO ) == 1 )
      zwfnTZEREMDD_PositionNonPosEnts( vSubtask, vTZEREMDO );

   // Set LastSyncDate for Data Model.
   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( vTZEREMDO, "EntpER_Model", "LastSyncDate", szTimestamp );

   // Sort Attributes in original sequence
   oTZEREMDO_SortAttributes( vTZEREMDO );
   zwTZEREMDD_RefreshAttWndForSort( vSubtask );
   zwTZEREMDD_RefEntListForSort( vSubtask );

   if ( zwfnTZEREMDD_CommitERD( vSubtask, vTZEREMDO ) < 0 )
      return( -1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_PrintModel
// PURPOSE:   Print contents of ER model hierarchically
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_Print
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PrintModel( zVIEW vSubtask )
{
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ProfileSave
// PURPOSE:   Save updated Profile.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ProfileSave( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  vWindow;
   zLONG  lDrawOption;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   oTZ__PRFO_CommitProfile( vProfileXFER );

   // reset the line style for Model ERD
   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   if ( vWindow )
   {
      GetIntegerFromAttribute( &lDrawOption, vProfileXFER, "EMD", "ERD_ShowRelOption" );
      SetDrawOptions( vWindow, "ERD", (zUSHORT)lDrawOption );
   }

   // reset the line style for Submodel ERD
   GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK );
   if ( vWindow )
   {
      GetIntegerFromAttribute( &lDrawOption, vProfileXFER, "EMD", "SA_ShowRelOption" );
      SetDrawOptions( vWindow, "ERD", (zUSHORT)lDrawOption );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ProfileSaveandPrint
// PURPOSE:   Save updated Profile AND Print Data Model
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ProfileSaveAndPrint( zVIEW vSubtask )
{
   zwTZEREMDD_ProfileSave( vSubtask );
   zwTZEREMDD_ERD_Print( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_ProfileSavePrint
// PURPOSE:   Save updated Profile AND Print ER Model
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_ProfileSavePrint( zVIEW vSubtask )
{
   zwTZEREMDD_ProfileSave( vSubtask );
   zwTZEREMDD_SA_DiagramPrint( vSubtask );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AskForSave
// PURPOSE:   Prompt and Save Model
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AskForSave( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to current Enterprise Model Object and prompt for Save
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( nRC > 0 && MiGetUpdateForView( vTZEREMDO ) == 1 && ObjectInstanceUpdatedFromFile( vTZEREMDO ) == 1 )
   {
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_ERD, zREFER_ERD_META, vTZEREMDO, "EntpER_Model", "Name", 0 );
      if ( nRC == zRESPONSE_CANCEL )
      {
         return( -1 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            nRC = zwTZEREMDD_SaveModel( vSubtask );
            if ( nRC )
               return( -2 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ExitModel
// PURPOSE:   Leave ER Dialog with an implied save.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_Exit
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ExitModel( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vView;
   zVIEW  vProfileXFER;
   zVIEW  LPLR_View;
   zVIEW  vT;
   zSHORT nRC;
   zLONG  lDLG_X;
   zLONG  lDLG_Y;
   zSHORT nZoom;

   nZoom = ZoomDiagram( vSubtask, "ERD", 0, 0 );
   SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );

   // Get Access to current Enterprise Model Object and prompt for Save
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Prompt for Subject Area Object Save
   if ( zwTZEREMDD_SA_AskForSave( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // Prompt for Model Save
   if ( zwTZEREMDD_AskForSave( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // Save the current state to the Profile
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   if ( CompareAttributeToString( vProfileXFER, "EMD", "StartupLPLR_Option", "L" ) == 0 )   //set Last LPLR
   {
      GetViewByName( &vT, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vT )
      {
         SetAttributeFromAttribute( vProfileXFER, "EMD", "StartupLPLR_Name", vT, "LPLR", "Name" );
      }
   }

   // Set ENTLIST Profile
// if ( GetViewByName( &vT, "ENTVIEW", vSubtask, zLEVEL_TASK ) > 0 )
   if ( GetSubtaskForWindowName( vSubtask, &vT, "ENTVIEW" ) >= 0 )
   {
      SetAttributeFromString( vProfileXFER, "EMD", "EntView", "Y" );
      GetCtrlSize( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "SZDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "SZDLG_Y", lDLG_Y );
      GetCtrlPosition( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "PSDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "PSDLG_Y", lDLG_Y );
   }
   else
      SetAttributeFromString( vProfileXFER, "EMD", "EntView", "N" );

   if ( GetViewByName( &vT, "ATTVIEW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromString( vProfileXFER, "EMD", "AttrView", "Y" );
      GetCtrlSize( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "SZDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "SZDLG_Y", lDLG_Y );
      GetCtrlPosition( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "PSDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "PSDLG_Y", lDLG_Y );
   }
   else
      SetAttributeFromString( vProfileXFER, "EMD", "AttrView", "N" );


   if ( GetViewByName( &vT, "RLLVIEW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromString( vProfileXFER, "EMD", "RelView", "Y" );
      GetCtrlSize( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "SZDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "SZDLG_Y", lDLG_Y );
      GetCtrlPosition( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "PSDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "PSDLG_Y", lDLG_Y );
   }
   else
      SetAttributeFromString( vProfileXFER, "EMD", "RelView", "N" );

   if ( GetViewByName( &vT, "IDVIEW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromString( vProfileXFER, "EMD", "IdView", "Y" );
      GetCtrlSize( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "IDVIEW", "SZDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "IDVIEW", "SZDLG_Y", lDLG_Y );
      GetCtrlPosition( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "IDVIEW", "PSDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "IDVIEW", "PSDLG_Y", lDLG_Y );
   }
   else
      SetAttributeFromString( vProfileXFER, "EMD", "IdView", "N" );

   if ( GetViewByName( &vT, "SAVIEW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromString( vProfileXFER, "EMD", "SM_View", "Y" );
      GetCtrlSize( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "Submodel", "SZDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "Submodel", "SZDLG_Y", lDLG_Y );
      GetCtrlPosition( vT, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "Submodel", "PSDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "Submodel", "PSDLG_Y", lDLG_Y );
   }
   else
      SetAttributeFromString( vProfileXFER, "EMD", "SM_View", "N" );

   if ( vTZERSASO )
      SetAttributeFromAttribute( vProfileXFER, "EMD", "LastSubmodel", vTZERSASO, "SubjectArea", "Name" );

   oTZ__PRFO_CommitProfile( vProfileXFER );

   // Drop Error View
   if ( GetViewByName( &vView, "TZERDERR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   // Drop view to TZBRLOVO (Cross Reference)
   if ( GetViewByName( &vView, "TZBRLOVO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropObjectInstance( vView );

   // Drop View from SAP Import
   if ( GetViewByName( &vView, "TZSAPERR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );
   if ( GetViewByName( &vView, "TZSapRel", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &LPLR_View, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = TerminateLPLR( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteModel
// PURPOSE:   Delete an existing ER model.
// TRIGGERED BY:
//    WINDOW: TZEROPMD
//    ACTION: BUTTON
//    NAME: &Delete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteModel( zVIEW vSubtask )
{
   zVIEW vTZERMFLO;
   zSHORT nRC;

   // Get Access to ERD File List Object
   nRC = GetViewByName( &vTZERMFLO, "TZERMFLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model List. Model list must be opened to Delete." );
      MessageSend( vSubtask, "ER00110", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Prompt to confirm Delete
   GetStringFromAttribute( sz, zsizeof( sz ), vTZERMFLO, "W_MetaDef", "Name" );
   strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Delete Data Model\n\n" );
   strcat_s( szWorkMessage, zsizeof( szWorkMessage ), sz );
   nRC = MessagePrompt( vSubtask, "ER10011", "ER Data Model Maintenance",
                        szWorkMessage, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

   // Delete Data Model Object
   if ( nRC == zRESPONSE_YES )
      nRC = DeleteMetaOI( vSubtask, vTZERMFLO, zSOURCE_ERD_META );
   else
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ViewModel
// PURPOSE:   To make the Model visible or invisible (toggle)
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   ViewModel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ViewModel( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zPCHAR   cpModel;
   zSHORT   nX = 292;
   zSHORT   nY;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetAddrForAttribute( &cpModel, vProfileXFER, "EMD", "EDM_View" );
   if ( *cpModel == 'Y' )
   {
      SetAttributeFromString( vProfileXFER, "EMD", "EDM_View", "N" );
      nY = 23;
      SetOptionState( vSubtask, "ViewModel", zOPTION_STATUS_CHECKED, 0 );
   }
   else
   {
      SetAttributeFromString( vProfileXFER, "EMD", "EDM_View", "Y" );
      nY = 212;
      SetOptionState( vSubtask, "ViewModel", zOPTION_STATUS_CHECKED, 1 );
   }

   SetCtrlSize( vSubtask, 0, nX, nY, TRUE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortEntName
// PURPOSE:   Sort ER Entities by Name
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   SortEntName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortEntName( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent4( vTZEREMDO, "ER_Entity", "Name", zASCENDING, "", 0, "", 0, "", 0 );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );  // ????
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortEntOwner
// PURPOSE:   Sort ER Entities by Owner, Name
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   SortEntOwner
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortEntOwner( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent4( vTZEREMDO, "ER_Entity", "OwningArea", zASCENDING, "Name", zASCENDING, "", 0, "", 0 );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );  // ????

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortEntZKey
// PURPOSE:   Sort ER Entities by ZKey
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   SortEntZKey
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortEntZKey( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent4( vTZEREMDO, "ER_Entity", "ZKey", zASCENDING, "", 0, "", 0, "", 0 );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );  // ????

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortAttName
// PURPOSE:   Sort ER Attributes by Name
// TRIGGERED BY:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortAttName( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent4( vTZEREMDO, "ER_Attribute", "Name", zASCENDING, "", 0, "", 0, "", 0 );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );  // ????

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortByName
// PURPOSE:   Sort ER FileList by Name
// TRIGGERED BY:
//    WINDOW: TZEROPMD
//    ACTION: BUTTON
//    NAME:   SortByName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortByName( zVIEW vSubtask )
{
   zVIEW vTZERMFLO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZERMFLO, "TZERMFLO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vTZERMFLO, "W_MetaDef", "Name", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortByDate
// PURPOSE:   Sort ER FileList by Last Update Date
// TRIGGERED BY:
//    WINDOW: TZEROPMD
//    ACTION: BUTTON
//    NAME:   SortByDate
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortByDate( zVIEW vSubtask )
{
   zVIEW vTZERMFLO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZERMFLO, "TZERMFLO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zDESCENDING, vTZERMFLO, "W_MetaDef", "LastUpdateDate", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SaveAsModel
// PURPOSE:   Save Data Model under a different name.
// TRIGGERED BY:
//    WINDOW: TZERSAMD
//    ACTION: BUTTON
//    NAME: &OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SaveAsModel( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened to Save As." );
      MessageSend( vSubtask, "ER00112", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Save ER Object under new name
   zwTZEREMDD_SaveModel( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_CloseModelessWindows
// PURPOSE:    This Entry refreshes the Entity, Attribute, Relationship,
//             and SA lists and the ER Diagram
//
///////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseModelessWindows( zVIEW vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" ) >= 0 )
      InvokeWindowActionBehavior( vW, zWAB_ReturnToParent, 0, 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" ) >= 0 )
      InvokeWindowActionBehavior( vW, zWAB_ReturnToParent, 0, 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vW, "RLLVIEW" ) >= 0 )
      InvokeWindowActionBehavior( vW, zWAB_ReturnToParent, 0, 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vW, "IDVIEW" ) >= 0 )
      InvokeWindowActionBehavior( vW, zWAB_ReturnToParent, 0, 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" ) >= 0 )
      InvokeWindowActionBehavior( vW, zWAB_ReturnToParent, 0, 0 );

#if 0
   zwTZEREMDD_CloseSAP_ErrorWindows( vSubtask );
#endif

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetSelectionInSAList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_SetSelectionInSAList( zVIEW vTZEREMDO, zVIEW vTZERSASO )
{
   if ( SetCursorFirstSelectedEntity( vTZEREMDO, "ER_RelLink", "" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vTZERSASO, "ER_RelLink", "ZKey", vTZEREMDO, "ER_RelLink", "ZKey", "" ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntity( vTZERSASO, "ER_RelLink", 1 );
      }
      else
      {
         if ( CheckExistenceOfEntity( vTZERSASO, "ER_RelLink" ) >= zCURSOR_SET )
            SetSelectStateOfEntity( vTZERSASO, "ER_RelLink", 0 );
      }
   }

   return( 0 );
} // zwTZEREMDD_SetSelectionInSAList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZEREMDD_RefreshEntitySelect
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwfnTZEREMDD_RefreshEntitySelect( zVIEW  vSubtask, zSHORT nDeselectRelationship )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDO1;
   zVIEW  vTZERSASO;
   zVIEW  vWindow;
   zVIEW  vMainWindow;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" );
   GetSubtaskForWindowName( vSubtask, &vMainWindow, "TZEREMDD" );

   nRC = zwTZEREMDD_RefreshEntWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshAttWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshSAWnd( vSubtask );

   // unselect Relationship
   if ( nDeselectRelationship == 1 && CheckExistenceOfEntity( vTZEREMDO, "ER_RelType" ) >= zCURSOR_SET &&
        GetSelectStateOfEntity( vTZEREMDO, "ER_RelType" ) == 1 )
   {
      SetSelectStateOfEntity( vTZEREMDO, "ER_RelType", 0 );
   }

   // Get Access to Subject Area
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 && vWindow )
   {
      // Deselection Relationship
      if ( nDeselectRelationship == 1 && CheckExistenceOfEntity( vTZERSASO, "SA_RelType" ) >= zCURSOR_SET &&
           GetSelectStateOfEntity( vTZERSASO, "SA_RelType" ) == 1 )
      {
         SetSelectStateOfEntity( vTZERSASO, "SA_RelType", 0 );
      }

      // First, deselect all selected Entities
      for ( nRC = SetCursorFirstSelectedEntity( vTZERSASO, "SA_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZERSASO, "SA_Entity", "" ) )
      {
         SetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 0, 1 );
         zwTZEREMDD_DeselectRelationship( vTZERSASO, "ER_RelLink", 0 );
      }  // END for all Selected ER_Entity

      // Now select our currently selected SA Entity (new)
      CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );

      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", "" ) )
      {
         if ( SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity", "ZKey", "SubjectArea" ) >= zCURSOR_SET )
         {
            SetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 1, 1 );
            zwTZEREMDD_SetSelectionInSAList( vTZEREMDO, vTZERSASO );
         }
         else
            zwTZEREMDD_DeselectRelationship( vTZERSASO, "ER_RelLink", 0 );

      }
      SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZEREMDO1, "ER_Entity", "ZKey", "" );
      SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity", "ZKey", "SubjectArea" );
      DropView( vTZEREMDO1 );
      RefreshDiagram( vWindow, "ERD", 0 );
      RefreshCtrl( vWindow, "SA_Entity" );
      RefreshCtrl( vWindow, "SA_RelLink" );
   } // endif ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 &&...

   zwTZEREMDD_ERD_HighliteEntity( vMainWindow );
   RefreshDiagram( vMainWindow, "ERD", 0 );

   return( 0 );
} // zwfnTZEREMDD_RefreshEntitySelect

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshEntitySelect
// PURPOSE:    This Entry refreshes the Entity, Attribute, Relationship,
//             and SA lists and the ER Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntitySelect( zVIEW vSubtask )
{
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetEntitySelect
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SetEntitySelect( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetSelectSetForView( vTZEREMDO, 1 );

   zwTZEREMDD_ERD_DeselectAll( vSubtask );
   SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );

   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );

   return( 0 );
} // zwTZEREMDD_SetEntitySelect


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshEntAttRelWnd
// PURPOSE:    This Entry refreshes the Entity, Attribute, and Relationship
//             lists in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntAttRelWnd( zVIEW vSubtask )
{
   zSHORT nRC;

   nRC = zwTZEREMDD_RefreshEntWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshAttWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );
   return( 0 );
}

#if( 1 )

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshAttRelSAWnd
// PURPOSE:    This Entry refreshes the Entity, Attribute, and Relationship
//             lists in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttRelSAWnd( zVIEW vSubtask )
{
   zSHORT nRC;

   nRC = zwTZEREMDD_RefreshAttWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshSAWnd( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshAttRelWnd
// PURPOSE:    This Entry refreshes the Attribute and Relationship
//             lists in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttRelWnd( zVIEW vSubtask )
{
   zSHORT nRC;

   nRC = zwTZEREMDD_RefreshAttWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );
   nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );

   return( 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_ERD_HighliteEntity
// PURPOSE:    This Entry gets a view to the main window,
//             selects the current entity and centers it, if it is not
//             visible.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_HighliteEntity( zVIEW vSubtask )
{
   zVIEW  vW;
   zVIEW  vTZEREMDO;
   zVIEW  vERD;
   zVIEW  vTZERSASO;
   zVIEW  vTemp;
   zSHORT nRC;
   zPCHAR szName;
   zPCHAR pDesc;
   zPCHAR pDescMax;
   zPCHAR pMsg;
   zCHAR  szMsg[ 100 ];

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" ) >= 0 )
   {
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );

      strcpy_s( szMsg, zsizeof( szMsg ), szName);
      strcat_s( szMsg, zsizeof( szMsg ), " : " );

      GetAddrForAttribute( &pDesc, vTZEREMDO, "ER_Entity", "Desc" );

      // Concat characters from szDesc until the first null or NewLine or line feed or to a maximum of 60 characters.
      pMsg  = szMsg + zstrlen( szMsg );
      pDescMax = pDesc + 60;
      while ( *pDesc != 0 && *pDesc != '\n' && *pDesc != '\r' && pDesc < pDescMax )
      {
         *pMsg++ = *pDesc++;
      }
      *pMsg = 0;

      MB_SetMessage( vW, 1, szMsg );

      if ( !GetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1 ) )
         ERD_SelectEntity( vW, "ERD", 1, 1 );

      if ( !ERD_IsEntityVisible( vW, "ERD" ) )
         CenterEntity( vW, "ERD" );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" ) >= 0 )
   {
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

      // Use temp view so as not to alter cursor position.
      GetViewByName( &vTemp, "TZERSASO", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vTZERSASO, vTemp, 0 );
      CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

      for ( nRC = SetCursorFirstSelectedEntity( vTZERSASO, "SA_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZERSASO, "SA_Entity", "" ) )
      {

         // Now select our currently selected SA Entity
         // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.
         nRC = SetCursorFirstEntityByAttr( vERD, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", "" );

         if ( nRC >= zCURSOR_SET )
         {
            if ( !GetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 1 ) )
               ERD_SelectEntity( vW, "ERD", 1, 1 );

            if ( !ERD_IsEntityVisible( vW, "ERD" ) )
               CenterEntity( vW, "ERD" );
         }
         else
            ERD_SelectEntity( vW, "ERD", 0, 0);
      }
      DropView( vTZERSASO );
      DropView( vERD );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_ERD_CenterEntity
// PURPOSE:    This Entry gets a view to the main window,
//             selects the current entity and centers it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_CenterEntity( zVIEW vSubtask )
{
   zVIEW  vW;
   zSHORT nZoom;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" ) >= 0 )
   {
      nZoom = zwTZEREMDD_ERD_ZoomStandardSize( vW );
      nZoom = CenterEntity( vW, "ERD" );
      nZoom = ZoomDiagram( vSubtask, "ERD", 0, 0 );
      SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" ) >= 0 )
   {
      zwTZEREMDD_ERD_ZoomStdSizeSA( vW );
      CenterEntity( vW, "ERD" );
      zwTZEREMDD_RefreshSAWnd( vW );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_ERD_DeselectAll
// PURPOSE:    This Entry deselects all entities in the ER and SA diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_DeselectAll( zVIEW vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" ) >= 0 )
   {
      ERD_SelectEntity( vW, "ERD", 0, 1 );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" ) >= 0 )
   {
      ERD_SelectEntity( vW, "ERD", 0, 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshEntWnd
// PURPOSE:    This Entry refreshes the Entity List in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntWnd( zVIEW vSubtask )
{
   zVIEW vW;
   zVIEW vTZEREMDO;
   zVIEW vTZERSASO;
   zCHAR szEntityName[ 256 ];

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" ) >= 0 )
   {
      if ( vSubtask != vW )
         RefreshWindow( vW );
      else
      {
         RefreshCtrl( vW, "AttrList");
         RefreshCtrl( vW, "RelList");
         RefreshCtrl( vW, "EntityDesc");
      }
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      if ( CheckExistenceOfEntity ( vTZEREMDO, "ER_Entity" ) == zCURSOR_SET )
      {
         GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
         SetWindowCaptionTitle( vW, 0, szEntityName );
      }
      else
         SetWindowCaptionTitle( vW, 0, "");


      // Get Access to SA Object to set position in SA
      GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
      if ( vTZERSASO )
      {
         if ( CheckExistenceOfEntity( vTZERSASO, "SA_Entity" ) >= zCURSOR_SET )
         {
            // Now select our currently selected SA Entity
            // Frank H. (97/04/15): if the ERD is not checked out then the
            // SA is not correct linked to the ERD --> SetCursor by ZKey.
            SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity", "ZKey", "" );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshAttWnd
// PURPOSE:    This Entry refreshes the Attribute List in the
//             ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttWnd( zVIEW vSubtask )
{
   zVIEW vW;
   zVIEW vTZEREMDO;
   zCHAR szEntityName[ 33 ];

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" ) >= 0 )
   {
      RefreshWindow( vW );
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
      SetWindowCaptionTitle( vW, 0, szEntityName );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" ) >= 0 )
      RefreshCtrl( vW, "AttrList");

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTMAINT" ) >= 0 )
      RefreshCtrl( vW, "lbAttribute");

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshIdWnd
// PURPOSE:    This Entry refreshes the Identifier List in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshIdWnd( zVIEW vSubtask )
{
   zVIEW vW;
   zVIEW vTZEREMDO;
   zCHAR szEntityName[ 33 ];

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "IDVIEW" ) >= 0 )
   {
      RefreshWindow( vW );
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
      SetWindowCaptionTitle( vW, 0, szEntityName );
   }


   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshRllWnd
// PURPOSE:    This Entry refreshes the Relationship List in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshRllWnd( zVIEW vSubtask )
{
   zVIEW vW;
   zVIEW vTZEREMDO;
   zCHAR szEntityName[ 33 ];

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "RLLVIEW" ) >= 0 )
   {
      RefreshWindow( vW );
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
      SetWindowCaptionTitle( vW, 0, szEntityName );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" ) >= 0 )
      RefreshCtrl( vW, "RelList");

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTMAINT" ) >= 0 )
      RefreshCtrl( vW, "lbRelationship");

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_RefreshSAWnd
// PURPOSE:    This Entry refreshes the Subject Area List in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshSAWnd( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDO_Copy;
   zVIEW  vTZERSASO;
   zVIEW  vTemp;
   zVIEW  vW;
   zSHORT nRC;
   zPCHAR szName;
   zPCHAR pDesc;
   zPCHAR pDescMax;
   zPCHAR pMsg;
   zCHAR  szMsg[ 100 ];

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" ) >= 0 )
   {
      MB_SetMessage( vW, 1, "    " );

      // Get Access to ER Object
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vTZEREMDO_Copy, vTZEREMDO, 0 );

      // Use temporary view so as not to alter cursor position.
      GetViewByName( &vTemp, "TZERSASO", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vTZERSASO, vTemp, 0 );

      // Now select our currently selected SA Entity
      // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.

      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", 0 ) )
      {
         nRC = SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity", "ZKey", "SubjectArea" );

         if ( nRC >= zCURSOR_SET )
         {
            GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
            strcpy( szMsg, szName);
            GetAddrForAttribute( &pDesc, vTZERSASO, "ER_Entity", "Desc" );
            strcat( szMsg, " :   " );

            // Concat characters from szDesc until the first null or NewLine or to a maximum of 60 characters.
            pDescMax = pDesc + 60;
            pMsg  = szMsg + zstrlen( szMsg );
            while ( *pDesc != 0 && *pDesc != '\n' && pDesc < pDescMax )
            {
               *pMsg++ = *pDesc++;
            }
            *pMsg = 0;

            MB_SetMessage( vW, 1, szMsg );
         }
      }

      SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity", vTZEREMDO_Copy, "ER_Entity", 0 );
      DropView( vTZEREMDO_Copy );

      if ( vW != vSubtask )
      {
         RefreshCtrl( vW, "SA_Entity");
         RefreshCtrl( vW, "SA_RelLink");
      }

      DropView( vTZERSASO );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PreBuildENTVIEW
// PURPOSE:    This Entry names a view for the Entity
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildENTVIEW( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;

   // Make sure the ERD is sorted by ER_Entity Name.
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vTZEREMDO, "ER_Entity", "Name A" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildENTVIEW
// PURPOSE:    This Entry sets the caption for the Entity
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildENTVIEW( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vProfileXFER;
   zLONG  lPSDLG_X;
   zLONG  lPSDLG_Y;
   zLONG  lSZDLG_X;
   zLONG  lSZDLG_Y;
   zCHAR  szEntityName[ 33 ];

   SetNameForView( vSubtask, "ENTVIEW", vSubtask, zLEVEL_TASK );

   // Set Caption for Entity name
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szEntityName );

   // Size the window appropriateley
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lSZDLG_X, vProfileXFER, "ENTVIEW", "SZDLG_X" );
   if ( lSZDLG_X )
   {
      GetIntegerFromAttribute( &lSZDLG_Y, vProfileXFER, "ENTVIEW", "SZDLG_Y" );
      GetIntegerFromAttribute( &lPSDLG_X, vProfileXFER, "ENTVIEW", "PSDLG_X" );
      GetIntegerFromAttribute( &lPSDLG_Y, vProfileXFER, "ENTVIEW", "PSDLG_Y" );
      SetCtrlPosition( vSubtask, 0, (zSHORT)lPSDLG_X, (zSHORT)lPSDLG_Y, TRUE );
      SetCtrlSize( vSubtask, 0, (zSHORT)lSZDLG_X, (zSHORT)lSZDLG_Y, TRUE );
   }

   LB_SetSelectSelectSet( vSubtask, "EntityListBox", 2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_CloseENTVIEW
// PURPOSE:    This Entry deletes the named view to the window upon close.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseENTVIEW( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vProfileXFER;
   zLONG  lDLG_X;
   zLONG  lDLG_Y;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   if ( CheckExistenceOfEntity( vProfileXFER, "ENTVIEW" ) >= zCURSOR_SET )
   {
      GetCtrlSize( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "SZDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "SZDLG_Y", lDLG_Y );
      GetCtrlPosition( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "PSDLG_X", lDLG_X );
      SetAttributeFromInteger( vProfileXFER, "ENTVIEW", "PSDLG_Y", lDLG_Y );
   }

   GetViewByName( &vWindow, "ENTVIEW", vSubtask, zLEVEL_TASK );
   if ( vWindow )
      DropNameForView( vWindow, "ENTVIEW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildENTCREATE
// PURPOSE:    This Entry sets the Graying for the MORE Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildENTCREATE( zVIEW vSubtask )
{
   zwTZEREMDD_ERD_DeselectAll( vSubtask );

   // Refresh Data Model, Subject Area and set selection
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_PreBuildENT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_PreBuildENT( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;

   zwTZEREMDD_ERD_DeselectAll( vSubtask );

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );

   // Refresh Data Model, Subject Area and set selection
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );

   return( 0 );
} // zwTZEREMDD_PreBuildENT

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_Set_SAP_CtrlState
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_Set_SAP_CtrlState( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nNoSAP_Entity = 1;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // if SAP Entity, disable TE Table Name and TE Table Owner
   if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 )
   {
      nNoSAP_Entity = 0;
   }

   SetCtrlState( vSubtask, "edTE_TableName",  zCONTROL_STATUS_ENABLED, nNoSAP_Entity );
   SetCtrlState( vSubtask, "edTE_TableOwner",  zCONTROL_STATUS_ENABLED, nNoSAP_Entity );

   return( 0 );
} // zwTZEREMDD_Set_SAP_CtrlState

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildENT
// PURPOSE:    This Entry sets the Graying for the Add & Delete Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildENT( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vTemp;
   zSHORT nRC;
   zSHORT nSA_RC;
   zCHAR  szEntityPurpose[ 2 ];

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nSA_RC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   // Disable Next and Previous buttons differently depending whether we came from the ERD window or the subject area window.
   if ( nSA_RC >= zCURSOR_SET && CheckExistenceOfEntity( vTZERSASO, "SA_Entity" ) >= zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vTemp, vTZERSASO, 0 );
      nRC =SetCursorNextEntity( vTemp, "SA_Entity", 0 );
      if ( nRC >= zCURSOR_SET )
         SetCtrlState( vSubtask, "Next",  zCONTROL_STATUS_ENABLED, 1 );
      else
         SetCtrlState( vSubtask, "Next",  zCONTROL_STATUS_ENABLED, 0 );
      DropView( vTemp );

      CreateViewFromViewForTask( &vTemp, vTZERSASO, 0 );
      nRC =SetCursorPrevEntity( vTemp, "SA_Entity", 0 );
      if ( nRC >= zCURSOR_SET )
         SetCtrlState( vSubtask, "Prev",  zCONTROL_STATUS_ENABLED, 1 );
      else
         SetCtrlState( vSubtask, "Prev",  zCONTROL_STATUS_ENABLED, 0 );
      DropView( vTemp );
   }
   else
   {
      CreateViewFromViewForTask( &vTemp, vTZEREMDO, 0 );
      nRC =SetCursorNextEntity( vTemp, "ER_Entity", 0 );
      if ( nRC >= zCURSOR_SET )
         SetCtrlState( vSubtask, "Next",  zCONTROL_STATUS_ENABLED, 1 );
      else
         SetCtrlState( vSubtask, "Next",  zCONTROL_STATUS_ENABLED, 0 );
      DropView( vTemp );

      CreateViewFromViewForTask( &vTemp, vTZEREMDO, 0 );
      nRC =SetCursorPrevEntity( vTemp, "ER_Entity", 0 );
      if ( nRC >= zCURSOR_SET )
         SetCtrlState( vSubtask, "Prev",  zCONTROL_STATUS_ENABLED, 1 );
      else
         SetCtrlState( vSubtask, "Prev",  zCONTROL_STATUS_ENABLED, 0 );
      DropView( vTemp );
   }

   // only enable dimension hierarchy button for dimensions
   GetStringFromAttribute( szEntityPurpose, zsizeof( szEntityPurpose ), vTZEREMDO, "ER_Entity", "Purpose" );
   if ( *szEntityPurpose == '2' )
      SetCtrlState( vSubtask, "DimHierBtn",  zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "DimHierBtn",  zCONTROL_STATUS_ENABLED, 0 );

   if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
   {
      SetCtrlState( vSubtask, "AddBtn",  zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "Delete",  zCONTROL_STATUS_ENABLED, 0 );
   }
   else
   {
      SetCtrlState( vSubtask, "AddBtn",  zCONTROL_STATUS_ENABLED, 1 );
      SetCtrlState( vSubtask, "Delete",  zCONTROL_STATUS_ENABLED, 1 );
      // if SAP Entity, disable TE Table Name and TE Table Owner
      zwTZEREMDD_Set_SAP_CtrlState( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ValidateEntity
// PURPOSE:   Validate the entity attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ValidateEntity( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zCHAR szEntityName[ 255 ];
   //zCHAR szOutName[ 255 ];
   zCHAR szMsg[ 256 ];
   zSHORT nRC;

   // Check that the entity name is unique.
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
   if ( CheckDuplicateEntityByString( vTZEREMDO, "ER_Entity", "Name", szEntityName, 0 ) < 0 )
   {
      MessageSend( vSubtask, "ER00179", "ER Data Model Maintenance", "Duplicate Entity Name in Data Model.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "Name" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Check TE Table Name
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "TE_TableName" );
   //UfCompressName( szEntityName, szOutName, 32, "", "", "", "", 0 );

   //if ( zstrcmp( szOutName, "" ) == 0 )
   if ( zstrcmp( szEntityName, "" ) == 0 )
   {
     // The following lines were commented out by DonC on 7/11/2014 because check out is no longer being used and the operation
     // code was somehow initializing the attribute to garbage values.
     //if (ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META ))
     //    SetAttributeFromString( vTZEREMDO, "ER_Entity", "TE_TableName", szOutName );
   }
   else
   {
      if ( CheckDuplicateEntityByString( vTZEREMDO, "ER_Entity", "TE_TableName", szEntityName, 0 ) < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Duplicate TE Table Name in Data Model. Entity/TE Table: ");
         strcat_s( szMsg, zsizeof( szMsg ), szEntityName );
         strcat_s( szMsg, zsizeof( szMsg ), NEW_LINE );
         strcat_s( szMsg, zsizeof( szMsg ), "Do you want to keep the duplicate names?" );
         nRC = MessagePrompt( vSubtask, "ER00179", "ER Data Model Maintenance", szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

         if ( nRC != zRESPONSE_YES )
         {
            SetFocusToCtrl( vSubtask, "edTE_TableName" );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_EntityOK
// PURPOSE:   Accept Entity changes
// TRIGGERED BY:
//    WINDOW: ER_ENTIT
//    ACTION: BUTTON
//    NAME:   OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_EntityOK( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW vTZEREMDO;
   zVIEW vTZERSASO;

   // Validate the entity.
   if ( zwTZEREMDD_ValidateEntity( vSubtask ) < 0 )
      return( -1 );

   // Validate subobject.
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( AcceptSubobject( vTZEREMDO, "ER_Entity" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Refresh the Entity in the diagrams
   if ( GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK ) > 0 )
   {
      RefreshEntity( vWindow, "ERD" );
   }

   if ( GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) >= 1 )
      {
         // Now select our currently selected SA Entity
         // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.
         SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity", "ZKey", "SubjectArea" );
         if ( CheckExistenceOfEntity( vTZERSASO, "ER_Entity" ) == zCURSOR_SET )
            SetSelectStateOfEntityForSet( vTZERSASO, "ER_Entity", 1, 1 );
         RefreshEntity( vWindow, "ERD" );
      }
   }

   zwTZEREMDD_RefreshEntitySelect( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_NewEntityOK
// PURPOSE:   Accept a New Entity painted in the ER or Subject Area
// TRIGGERED BY:
//    WINDOW: ER_ENTIT
//    ACTION: BUTTON
//    NAME:   OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_NewEntityOK( zVIEW vSubtask )
{
   zVIEW  vWindow;

   // Validate the entity.
   if ( zwTZEREMDD_ValidateEntity( vSubtask ) < 0 )
      return( -1 );

   if ( GetViewByName( &vWindow, "SA_ENTITY_ADD", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropNameForView( vWindow, "SA_ENTITY_ADD", vSubtask, zLEVEL_TASK );
      AcceptEntity( vWindow, "ERD" );
   }
   else
   {
      GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
      AcceptEntity( vWindow, "ERD" );
   }

   zwTZEREMDD_EntityOK( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_NewEntityCancel
// PURPOSE:   Accept a New Entity painted in the ER or Subject Area
// TRIGGERED BY:
//    WINDOW: ER_ENTIT
//    ACTION: BUTTON
//    NAME:   OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_NewEntityCancel( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW vTZEREMDO;
   zVIEW vTZERSASO;

   if ( GetViewByName( &vWindow, "SA_ENTITY_ADD", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropNameForView( vWindow, "SA_ENTITY_ADD", vSubtask, zLEVEL_TASK );
      RemoveEntity( vWindow, "ERD", 1, 0 );
      GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
      DeleteEntity( vTZERSASO, "SA_Entity", zREPOS_PREV );
   }

   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   RemoveEntity( vWindow, "ERD", 0, 0 );

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   DeleteEntity( vTZEREMDO, "ER_Entity", zREPOS_PREV );

   if ( CheckExistenceOfEntity ( vTZEREMDO, "ER_Entity" ) >= zCURSOR_SET )
   {
      SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );
      // Refresh Data Model, Subject Area and set selection
      zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_EntityNext
// PURPOSE:   Position on next entity in ER Model
// TRIGGERED BY:
//    WINDOW: ER_ENTIT
//    ACTION: BUTTON
//    NAME: Next
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_EntityNext( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zSHORT nSA_RC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nSA_RC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   // Validate the entity.
   if ( zwTZEREMDD_ValidateEntity( vSubtask ) < 0 )
      return( -1 );

   zwTZEREMDD_EntityOK( vSubtask );
   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 0 );

   // Process differently when coming directly from ER window versus coming from Subject Area window.
   if ( nSA_RC >= zCURSOR_SET && CheckExistenceOfEntity( vTZERSASO, "SA_Entity" ) >= zCURSOR_SET )
   {
      SetCursorNextEntity( vTZERSASO, "SA_Entity", "" );
      SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", 0 );
   }
   else
   {
      SetCursorNextEntity( vTZEREMDO, "ER_Entity", "" );
   }

   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 1 );
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );
   zwTZEREMDD_PostBuildENT( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_EntityPrev
// PURPOSE:   Position on previous entity in Data Model
// TRIGGERED BY:
//    WINDOW: ER_ENTIT
//    ACTION: BUTTON
//    NAME: Prev
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_EntityPrev( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zSHORT nSA_RC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nSA_RC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   // Validate the entity.
   if ( zwTZEREMDD_ValidateEntity( vSubtask ) < 0 )
      return( -1 );

   zwTZEREMDD_EntityOK( vSubtask );
   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 0 );

   // Process differently when coming directly from ER window versus coming from Subject Area window.
   if ( nSA_RC >= zCURSOR_SET && CheckExistenceOfEntity( vTZERSASO, "SA_Entity" ) >= zCURSOR_SET )
   {
      SetCursorPrevEntity( vTZERSASO, "SA_Entity", "" );
      SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", 0 );
   }
   else
   {
      SetCursorPrevEntity( vTZEREMDO, "ER_Entity", "" );
   }

   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 1 );
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );
   zwTZEREMDD_PostBuildENT( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwfnTZEREMDD_AddEntity
// PURPOSE:   Add Entity to ER Model
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_AddEntity( zVIEW vSubtask, zVIEW vTZEREMDO )
{
   zSHORT nRC;

   // Create and initialize ER Entity
   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 0 );
   nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_Entity", zPOS_AFTER );
#if( 0 )
   GetIntegerFromAttribute( &lZKey, vTZEREMDO, "ER_Entity", "ZKey" );
   strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "New_" );
   zltoxa( lZKey, &szWorkMessage[ 4 ] );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_Entity", "Name", szWorkMessage );
#endif
   SetCtrlState( vSubtask, "edTE_TableName",  zCONTROL_STATUS_ENABLED, 1 );
   SetCtrlState( vSubtask, "edTE_TableOwner",  zCONTROL_STATUS_ENABLED, 1 );

   nRC = SetAttributeFromString( vTZEREMDO, "ER_Entity", "Purpose", "F" );
   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddEntity
// PURPOSE:   Add Entity to Data Model via Option or Button
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddEntity( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vWindow;
   zSHORT nRC;

   // Get access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate subobject.
   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Entity" ) >= zCURSOR_SET )
   {
      if ( zwTZEREMDD_ValidateEntity( vSubtask ) < 0 )
         return( -1 );
   }

   zwfnTZEREMDD_AddEntity( vSubtask, vTZEREMDO );

   // See if ERD needs refreshing
   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   if ( vWindow )
   {
      zwfnTZEREMDD_PositionNonPosEnts( vWindow, vTZEREMDO );
      RefreshDiagram( vWindow, "ERD", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CopyEntity
// PURPOSE:   Copy Entity to ER Model
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: Copy
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CopyEntity( zVIEW vSubtask )
{
   zVIEW  vW;
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMD1;
   zSHORT nRC;
   zCHAR  szEntityName[ 35 ];
   zCHAR  szName[ 35 ];

   // Get access ERD Subtaskid
   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

   // Get access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Create and initialize ER Entity

   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 0 );
   CreateViewFromViewForTask( &vTZEREMD1, vTZEREMDO, 0 );
   nRC = CreateMetaEntity( vSubtask, vTZEREMD1, "ER_Entity", zPOS_LAST );
   zwfnTZEREMDD_PositionNonPosEnts( vW, vTZEREMD1 );
   SetMatchingAttributesByName( vTZEREMD1, "ER_Entity", vTZEREMDO, "ER_Entity", zSET_NULL );
   SetAttributeFromInteger( vTZEREMD1, "ER_Entity", "SAP_Entity", 0 );
   strcpy_s( szEntityName, zsizeof( szEntityName ), "new_" );
   GetStringFromAttribute( szEntityName + 4, zsizeof( szEntityName ) - 4, vTZEREMDO, "ER_Entity", "Name" );
   SetAttributeFromString( vTZEREMD1, "ER_Entity", "Name", szEntityName );
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Attribute", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = CreateMetaEntity( vSubtask, vTZEREMD1, "ER_Attribute", zPOS_LAST );
      SetMatchingAttributesByName( vTZEREMD1, "ER_Attribute", vTZEREMDO, "ER_Attribute", zSET_NULL | zSET_NOTNULL );
      SetAttributeFromInteger( vTZEREMD1, "ER_Attribute", "SAP_Attribute", 0 );
      nRC = IncludeSubobjectFromSubobject( vTZEREMD1, "Domain", vTZEREMDO, "Domain", zPOS_AFTER );
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_Attribute", "" );
   }

   // Create Identifier
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_EntIdentifier", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = CreateMetaEntity( vSubtask, vTZEREMD1, "ER_EntIdentifier", zPOS_LAST );
      SetMatchingAttributesByName( vTZEREMD1, "ER_EntIdentifier", vTZEREMDO, "ER_EntIdentifier", zSET_NULL | zSET_NOTNULL );

      nRC = SetCursorFirstEntity( vTZEREMDO, "ER_FactType", "" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_AttributeIdentifier" ) == zCURSOR_SET)
         {
            CreateMetaEntity( vSubtask, vTZEREMD1, "ER_FactType", zPOS_LAST );
            SetMatchingAttributesByName( vTZEREMD1, "ER_FactType", vTZEREMDO, "ER_FactType", zSET_NULL | zSET_NOTNULL );
            GetStringFromAttribute( szName, zsizeof( szName ), vTZEREMDO, "ER_AttributeIdentifier", "Name" );
            SetCursorFirstEntityByString( vTZEREMD1, "ER_Attribute", "Name", szName, 0 );
            IncludeSubobjectFromSubobject( vTZEREMD1, "ER_AttributeIdentifier", vTZEREMD1, "ER_Attribute", zPOS_AFTER );
         }
         nRC = SetCursorNextEntity( vTZEREMDO, "ER_FactType", "" );
      }
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_EntIdentifier", "" );
   }

   SetViewFromView( vTZEREMDO, vTZEREMD1 );
   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 1 );
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Attribute", "" );

   nRC = RefreshEntity( vW, "ERD" );
   nRC = zwTZEREMDD_RefreshEntAttRelWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CopyEntityWithRel
// PURPOSE:   Copy Entity to ER Model along with it's corresponding Rels
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: Copy
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CopyEntityWithRel( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZEREMD1;
   zVIEW vW;
   zVIEW vRelFwd;
   zVIEW vRelInv;
   zSHORT nRC;

   // Get access ERD Subtaskid
   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

   // Get access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vTZEREMD1, vTZEREMDO, 0 );
   nRC = zwTZEREMDD_CopyEntity( vSubtask );

   nRC = SetCursorFirstEntity( vTZEREMD1, "ER_RelLink", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vTZEREMD1, "ER_RelType", vTZEREMD1, "ER_RelType_1", "" );
      nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_RelType", zPOS_LAST );
      SetMatchingAttributesByName( vTZEREMDO, "ER_RelType", vTZEREMD1, "ER_RelType", zSET_NULL );

      nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
      SetMatchingAttributesByName( vTZEREMDO, "ER_RelLink_2", vTZEREMD1, "ER_RelLink_2", zSET_NULL | zSET_NOTNULL );
      nRC = CreateViewFromViewForTask( &vRelFwd, vTZEREMDO, 0 );

      nRC = SetCursorNextEntity( vTZEREMD1, "ER_RelLink_2", "" );
      nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
      SetMatchingAttributesByName( vTZEREMDO, "ER_RelLink_2", vTZEREMD1, "ER_RelLink_2", zSET_NULL | zSET_NOTNULL );
      nRC = CreateViewFromViewForTask( &vRelInv, vTZEREMDO, 0 );

      nRC = SetCursorFirstEntityByEntityCsr( vTZEREMD1, "ER_Entity_2", vTZEREMD1, "ER_Entity", "ER_RelType" );
      if ( nRC >= zCURSOR_SET )
      {
         nRC = SetCursorNextEntityByEntityCsr( vTZEREMD1, "ER_Entity_2", vTZEREMD1, "ER_Entity", "ER_RelType" );
         if ( nRC >= zCURSOR_SET )
         {
            nRC = IncludeSubobjectFromSubobject( vRelFwd, "ER_Entity_2", vTZEREMDO, "ER_Entity", zPOS_AFTER );
            nRC = IncludeSubobjectFromSubobject( vRelInv, "ER_Entity_2", vTZEREMDO, "ER_Entity", zPOS_AFTER );
         }
         else
         {
            nRC = SetCursorNextEntity( vTZEREMD1, "ER_RelLink_2", "" );
            if ( nRC >= zCURSOR_SET )
            {
               nRC = IncludeSubobjectFromSubobject( vRelFwd, "ER_Entity_2", vTZEREMDO, "ER_Entity", zPOS_AFTER );
               nRC = SetCursorFirstEntityByEntityCsr( vRelInv, "ER_Entity", vTZEREMD1, "ER_Entity_2", "" );
               nRC = IncludeSubobjectFromSubobject( vRelInv, "ER_Entity_2", vRelInv, "ER_Entity", zPOS_AFTER );
            }
            else
            {
               nRC = IncludeSubobjectFromSubobject( vRelInv, "ER_Entity_2", vTZEREMDO, "ER_Entity", zPOS_AFTER );
               nRC = SetCursorFirstEntity( vTZEREMD1, "ER_RelLink_2", "" );
               nRC = SetCursorFirstEntityByEntityCsr( vRelFwd, "ER_Entity", vTZEREMD1, "ER_Entity_2", "" );
               nRC = IncludeSubobjectFromSubobject( vRelFwd, "ER_Entity_2", vRelFwd, "ER_Entity", zPOS_AFTER );
            }
         }
      }
      nRC = SetCursorFirstEntityByEntityCsr( vRelFwd, "ER_RelLink", vRelFwd, "ER_RelLink_2", "EntpER_Model" );
      nRC = IncludeSubobjectFromSubobject( vRelFwd, "ER_RelLink_Other", vRelInv, "ER_RelLink_2", zPOS_AFTER );

      nRC = SetCursorFirstEntityByEntityCsr( vRelInv, "ER_RelLink", vRelInv, "ER_RelLink_2", "EntpER_Model" );

      nRC = IncludeSubobjectFromSubobject( vRelInv, "ER_RelLink_Other", vRelFwd, "ER_RelLink_2", zPOS_AFTER );
      nRC = RefreshRelationship( vW, "ERD", 0 );


      nRC = SetCursorNextEntity( vTZEREMD1, "ER_RelLink", "" );
   }
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Attribute", "" );
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );

   zwfnTZEREMDD_RePositionEntity( vW, vTZEREMDO );
   nRC = zwTZEREMDD_RefreshEntAttRelWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteEntity
// PURPOSE:   Delete All Selected Entities from the Data Model
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: DelEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteEntity( zVIEW vSubtask )
{
   zSHORT  nRC;
   zCHAR   szMsg[ 256 ];
   zVIEW   vTZEREMDO;
   zVIEW   vTZEREMDO1;
   zVIEW   vW;

   // Get access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( CountEntitiesForView( vTZEREMDO, "ER_Entity" ) == 1 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Delete Bypassed. Data Model must have at least one Entity." );
      MessageSend( vSubtask, "ER00116", "ER Data Model Maintenance", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 0 );
   }

   // Prompt to confirm Delete
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD, zENAME_ER_ENTITY, vTZEREMDO, "ER_Entity", "Name", 0 );

   if ( nRC == zRESPONSE_NO )
      return( 0 );

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );
   CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );
   zwfnTZEREMDD_FindRelatedVisible( vW, vTZEREMDO1, vTZEREMDO );

   SetNameForView( vTZEREMDO1, "TZEREMDO", vSubtask, zLEVEL_TASK );
   oTZEREMDO_DeleteEntity( vTZEREMDO1, zREPOS_NONE );
   if ( CheckExistenceOfEntity( vTZEREMDO1, "ER_Entity" ) == zCURSOR_SET )
      SetViewFromView( vTZEREMDO, vTZEREMDO1 );
   DropView( vTZEREMDO1 );

   SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 1 );

   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteSelectedEnts
// PURPOSE:   Delete All Selected Entities from the ER Model
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: DelEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteSelectedEnts( zVIEW vSubtask )
{
   zVIEW   vTZEREMDO;
   zVIEW   vTZEREMDO1;
   zVIEW   vW;
   zSHORT  nRC;
   zCHAR szMsg[ 256 ];

   // Get access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", 0 );
   if ( nRC <  zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00118", "ER Data Model Maintenance", "No Selected Entites to Delete.", zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 0 );
   }

   // Loop thru TZEREMDO1 and delete all selected entities, Set TZEREMDO to the proper entity to highlite.
   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );
   CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );

   //BL, 1999.10.19 Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all selected Entities", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      do
      {
         if ( CountEntitiesForView( vTZEREMDO1, "ER_Entity" ) == 1 )
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "Delete bypassed. Data Model must have at least one Entity." );
            MessageSend( vSubtask, "ER00119", "ER Data Model Maintenance", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            break;
         }
         else
         {
            // Determine which one to highlite in TZEREMDO
            zwfnTZEREMDD_FindRelatedVisible( vW, vTZEREMDO1, vTZEREMDO );

            // Delete current entity
            SetNameForView( vTZEREMDO1, "TZEREMDO", vSubtask, zLEVEL_TASK );
            oTZEREMDO_DeleteEntity( vTZEREMDO1, zREPOS_NONE );
            SetNameForView( vTZEREMDO1, "TZEREMDO1", vSubtask, zLEVEL_TASK );
            SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
         }

      } while ( SetCursorNextSelectedEntity( vTZEREMDO1, "ER_Entity", 0 ) >= zCURSOR_SET );
   }

   DropView( vTZEREMDO1 );

   zwfnTZEREMDO_SetEntityState( vSubtask, vTZEREMDO, 1 );

   nRC = zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RelLinkSetFKPrefix
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_RelLinkSetFKPrefix( zVIEW vSubtask, zVIEW vERD, zVIEW vProfileXfer )
{
   zSHORT   nEnableFKPrefix = 1;
   zSHORT   nEnableFKPrefix_Other = 1;

   if ( MiGetUpdateForView( vERD ) == 0 )
   {
      nEnableFKPrefix = 0;
      nEnableFKPrefix_Other = 0;
   }

   if ( CheckExistenceOfEntity( vERD, "ER_RelLink" ) >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink1TE_NoForeignKeyPrefix", "Y" ) == 0 )
      {
         nEnableFKPrefix = 0;
      }

      if ( CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink2TE_NoForeignKeyPrefix", "Y" ) == 0 )
      {
         nEnableFKPrefix_Other = 0;
      }

      // if Relationship between 2 SAP Entities, disable for update
      if ( CompareAttributeToInteger( vERD, "ER_Entity", "SAP_Entity", 1 ) == 0 || CompareAttributeToInteger( vERD, "ER_Entity_Other", "SAP_Entity", 1 ) == 0 )
      {
         nEnableFKPrefix = 0;
         nEnableFKPrefix_Other = 0;
      }
   }

   SetCtrlState( vSubtask, "Prefix", zCONTROL_STATUS_ENABLED, nEnableFKPrefix );
   SetCtrlState( vSubtask, "Prefix_Other", zCONTROL_STATUS_ENABLED, nEnableFKPrefix_Other );

   return( 0 );
} // zwTZEREMDD_RelLinkSetFKPrefix

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetUpTempRelLink( zVIEW  vSubtask, zVIEW  vERD )
{
   // Set up the TempRelLink entity in the profile object to hold temporary
   // RelLink entity values for Relationship detail window mapping. This
   // is necessary because we can't use a temporal subobject version with
   // the subobject structure.

   zVIEW    vProfileXfer;

   GetViewByName( &vProfileXfer, "ProfileXFER", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vProfileXfer, "ZO" ) < zCURSOR_SET )
      CreateEntity( vProfileXfer, "ZO", zPOS_AFTER );

   if ( CheckExistenceOfEntity( vProfileXfer, "TempRelLink" ) >= zCURSOR_SET )
      DeleteEntity( vProfileXfer, "TempRelLink", zPOS_AFTER );

   CreateEntity( vProfileXfer, "TempRelLink", zPOS_AFTER );

   //BL, 1999.10.19
   if ( CheckExistenceOfEntity( vERD, "ER_RelLink" ) >= zCURSOR_SET )
   {
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1Name", vERD, "ER_RelLink", "Name" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1CardMin", vERD, "ER_RelLink", "CardMin" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1CardMax", vERD, "ER_RelLink", "CardMax" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1AutoSeq", vERD, "ER_RelLink", "AutoSeq" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1HangingForeignKeyEntity", vERD, "ER_RelLink", "HangingForeignKeyEntity" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1TE_ForeignKeyPrefix", vERD, "ER_RelLink", "TE_ForeignKeyPrefix" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink1TE_NoForeignKeyPrefix", vERD, "ER_RelLink", "TE_NoForeignKeyPrefix" );

      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2Name", vERD, "ER_RelLink_Other", "Name" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2CardMin", vERD, "ER_RelLink_Other", "CardMin" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2CardMax", vERD, "ER_RelLink_Other", "CardMax" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2AutoSeq", vERD, "ER_RelLink_Other", "AutoSeq" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2HangingForeignKeyEntity", vERD, "ER_RelLink_Other", "HangingForeignKeyEntity" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2TE_ForeignKeyPrefix", vERD, "ER_RelLink_Other", "TE_ForeignKeyPrefix" );
      SetAttributeFromAttribute( vProfileXfer, "TempRelLink", "RelLink2TE_NoForeignKeyPrefix", vERD, "ER_RelLink_Other", "TE_NoForeignKeyPrefix" );

      zwTZEREMDD_DisableCtrlForRLL( vSubtask, vERD );
   }

   zwTZEREMDD_RelLinkSetFKPrefix( vSubtask, vERD, vProfileXfer );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CopyTempRelLink( zVIEW vSubtask, zVIEW vERD )
{
   // Copy the TempRelLink values to the ERD RelLink entities.

   zVIEW vProfileXfer;

   GetViewByName( &vProfileXfer, "ProfileXFER", vSubtask, zLEVEL_TASK );

   SetAttributeFromAttribute( vERD, "ER_RelLink", "Name", vProfileXfer, "TempRelLink", "RelLink1Name" );
   SetAttributeFromAttribute( vERD, "ER_RelLink", "CardMin", vProfileXfer, "TempRelLink", "RelLink1CardMin" );
   SetAttributeFromAttribute( vERD, "ER_RelLink", "CardMax", vProfileXfer, "TempRelLink", "RelLink1CardMax" );
   SetAttributeFromAttribute( vERD, "ER_RelLink", "AutoSeq", vProfileXfer, "TempRelLink", "RelLink1AutoSeq" );
   SetAttributeFromAttribute( vERD, "ER_RelLink", "HangingForeignKeyEntity", vProfileXfer, "TempRelLink", "RelLink1HangingForeignKeyEntity" );
   SetAttributeFromAttribute( vERD, "ER_RelLink", "TE_ForeignKeyPrefix", vProfileXfer, "TempRelLink", "RelLink1TE_ForeignKeyPrefix" );
   SetAttributeFromAttribute( vERD, "ER_RelLink", "TE_NoForeignKeyPrefix", vProfileXfer, "TempRelLink", "RelLink1TE_NoForeignKeyPrefix" );

   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "Name", vProfileXfer, "TempRelLink", "RelLink2Name" );
   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "CardMin", vProfileXfer, "TempRelLink", "RelLink2CardMin" );
   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "CardMax", vProfileXfer, "TempRelLink", "RelLink2CardMax" );
   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "AutoSeq", vProfileXfer, "TempRelLink", "RelLink2AutoSeq" );
   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "HangingForeignKeyEntity", vProfileXfer, "TempRelLink", "RelLink2HangingForeignKeyEntity" );
   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "TE_ForeignKeyPrefix", vProfileXfer, "TempRelLink", "RelLink2TE_ForeignKeyPrefix" );
   SetAttributeFromAttribute( vERD, "ER_RelLink_Other", "TE_NoForeignKeyPrefix", vProfileXfer, "TempRelLink", "RelLink2TE_NoForeignKeyPrefix" );

   DeleteEntity( vProfileXfer, "TempRelLink", zREPOS_NONE );

   return( 0 );
}

/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_BuildEntityView
// PURPOSE:   Build a list of all Entities in the model to be used
//            as candidates for the targeting of a relationship being
//            modified.
// TRIGGERED BY:
//    WINDOW: RLLMAINT
//    ACTION: BUTTON
//    NAME: ChgEntTgt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_BuildEntityView( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERENLO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Create Entity list view for Entity selection.
   CreateViewFromViewForTask( &vTZERENLO, vTZEREMDO, 0 );

   OrderEntityForView( vTZERENLO, "ER_Entity", "Name A" );
   SetNameForView( vTZERENLO, "TZERENLO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByAttr( vTZERENLO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity_Other", "ZKey", "" );

   // Set up special mapping entity.
   zwTZEREMDD_SetUpTempRelLink( vSubtask, vTZEREMDO );

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PreBuildATTVIEW
// PURPOSE:    This Entry names a view for the Attribute
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildATTVIEW( zVIEW vSubtask )
{
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildATTVIEW
// PURPOSE:    This Entry sets the caption for the Attribute
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildATTVIEW( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vProfileXFER;
   zLONG  lPSDLG_X;
   zLONG  lPSDLG_Y;
   zLONG  lSZDLG_X;
   zLONG  lSZDLG_Y;
   zCHAR  szEntityName[ 33 ];

   SetNameForView( vSubtask, "ATTVIEW", vSubtask, zLEVEL_TASK );

   // Set Caption for Entity name
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szEntityName );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lSZDLG_X, vProfileXFER, "ATTVIEW", "SZDLG_X" );
   if ( lSZDLG_X )
   {
      GetIntegerFromAttribute( &lSZDLG_Y, vProfileXFER, "ATTVIEW", "SZDLG_Y" );
      GetIntegerFromAttribute( &lPSDLG_X, vProfileXFER, "ATTVIEW", "PSDLG_X" );
      GetIntegerFromAttribute( &lPSDLG_Y, vProfileXFER, "ATTVIEW", "PSDLG_Y" );
      SetCtrlPosition( vSubtask, 0, (zSHORT)lPSDLG_X, (zSHORT)lPSDLG_Y, TRUE );
      SetCtrlSize( vSubtask, 0, (zSHORT)lSZDLG_X, (zSHORT)lSZDLG_Y, TRUE );
   }

   if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
   {
      EnableAction( vSubtask, "DeleteAttr", FALSE );
   }
   else
   {
      EnableAction( vSubtask, "DeleteAttr", TRUE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_CloseATTVIEW
// PURPOSE:    This Entry deletes the named view to the window upon close.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseATTVIEW( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW  vProfileXFER;
   zLONG  lDLG_X;
   zLONG  lDLG_Y;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   GetCtrlSize( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "SZDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "SZDLG_Y", lDLG_Y );
   GetCtrlPosition( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "PSDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "ATTVIEW", "PSDLG_Y", lDLG_Y );

   GetViewByName( &vWindow, "ATTVIEW", vSubtask, zLEVEL_TASK );
   if ( vWindow )
      DropNameForView( vWindow, "ATTVIEW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AttribOK
// PURPOSE:   OK on ER_Attribute for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_ATTRI
//    ACTION: BUTTON
//    NAME: OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttribOK( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate the attribute.
   if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_Attribute", "Attribute", "TE_ColumnName", "Column Name for Attribute" ) < 0 )
      return( -1 );

   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );
   zwTZEREMDD_RefreshAttWnd( vSubtask );

   return( 0 );
}
/* END OF OPERATION */


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_AttribCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_AttribCancel( zVIEW vSubtask )
{
   zVIEW    vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   CancelSubobject( vTZEREMDO, "ER_Attribute" );

   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );

   zwTZEREMDD_RefreshAttWnd( vSubtask );

   return( 0 );
} // zwTZEREMDD_AttribCancel

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AttribNext
// PURPOSE:   Position on next ER_Attribute for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_ATTRI
//    ACTION: BUTTON
//    NAME: Next
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttribNext( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate the attribute.
   if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_Attribute", "Attribute", "TE_ColumnName", "Column Name for Attribute" ) < 0 )
      return( -1 );

   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 0 );
   SetCursorNextEntity( vTZEREMDO, "ER_Attribute", "" );

   CreateTemporalSubobjectVersion( vTZEREMDO, "ER_Attribute" );
   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );

   zwTZEREMDD_PostBuildATTR( vSubtask );

   SetFocusToCtrl( vSubtask, "Name" );

   // Refresh Attribute and Entity List Window
   zwTZEREMDD_RefreshAttWnd( vSubtask );

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AttribPrev
// PURPOSE:   Position on previous ER_Attribute for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_ATTRI
//    ACTION: BUTTON
//    NAME: Prev
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttribPrev( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate the attribute.
   if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_Attribute", "Attribute", "TE_ColumnName", "Column Name for Attribute" ) < 0 )
      return( -1 );

   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 0 );
   SetCursorPrevEntity( vTZEREMDO, "ER_Attribute", "" );

   zwTZEREMDD_PostBuildATTR( vSubtask );

   CreateTemporalSubobjectVersion( vTZEREMDO, "ER_Attribute" );
   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );

   SetFocusToCtrl( vSubtask, "Name" );

   // Refresh Attribute and Entity List Window
   zwTZEREMDD_RefreshAttWnd( vSubtask );

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddAttribute
// PURPOSE:   Add ER_Attribute for ER_Entity
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//
//   NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddAttribute( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate subobject.
   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) >= zCURSOR_SET )
   {
      if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_Attribute", "Attribute", "TE_ColumnName", "Column Name for Attribute" ) < 0 )
      {
         return( -1 );
      }
      SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 0 );
   }

   // Create skeleton ER_Attribute entity
   CreateTemporalMetaEntity( vSubtask, vTZEREMDO, "ER_Attribute", zPOS_AFTER );
   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );

   // if SAP Entity create Work Attribute only
   if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 )
      SetAttributeFromString( vTZEREMDO, "ER_Attribute", "Work", "Y" );

   zwTZEREMDD_PostBuildATTR( vSubtask );

   // Set the cursor focus to first edit box on window.
   SetFocusToCtrl( vSubtask, "Name" );

   zwTZEREMDD_RefreshAttWnd( vSubtask );

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteAttrib
// PURPOSE:   Delete ER_Attribute for ER_Entity
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//
//   NAME: DelAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteAttrib( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vW;
   zSHORT nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vSubtask, &vW, "ATTMAINT" );

   if ( vW )
   {
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD, zENAME_ER_ATTRIBUTE, vTZEREMDO, "ER_Attribute", "Name", 0 );
      SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );
   }
   else
      //BL, 1999.10.19 Delete all selected Identifier:
      //Prompt to confirm Delete only one
      nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete selected Attributes", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   // Delete all selected ER Attributes
   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Attribute", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Attribute", "" ) )
      {
         // DC 2000.09.26
         // If there is an Identifier for this attribute, delete it.
         nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_AttributeIdentifier", vTZEREMDO, "ER_Attribute", "ER_Entity" );
         if ( nRC >= zCURSOR_SET && CountEntitiesForView( vTZEREMDO, "ER_FactType" ) == 1 )
         {
            DeleteEntity( vTZEREMDO, "ER_EntIdentifier", zREPOS_NONE );
         }

         // Then delete the attribute itself.
         nRC = DeleteEntity( vTZEREMDO, "ER_Attribute", zREPOS_NONE );
      }

      if ( vW && CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) == zCURSOR_NULL )
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      else
      if ( vW )
      {
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) == zCURSOR_UNDEFINED )
            SetCursorPrevEntity( vTZEREMDO, "ER_Attribute", "" );

         CreateTemporalSubobjectVersion( vTZEREMDO, "ER_Attribute" );
         SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );
         zwTZEREMDD_PostBuildATTR( vSubtask );
         SetFocusToCtrl( vSubtask, "Name" );
      }

      zwTZEREMDD_RefreshAttWnd( vSubtask );
   }

   return( 0 );
}
/* END OF OPERATION */


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteAllAttribs
// PURPOSE:   Delete all ER_Attributes for ER_Entity
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteAllAttribs( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all Attributes", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      // Loop and Delete all ER Attributes
      nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Attribute", "" );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = DeleteEntity( vTZEREMDO, "ER_Attribute", zREPOS_NONE );
         nRC = SetCursorNextEntity( vTZEREMDO, "ER_Attribute", "" );
      }
   }

   zwTZEREMDD_RefreshAttWnd( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DiagToRLLMaint
// PURPOSE:   When transferring from thr Diagram to relationship
//            maintenence, we need to position on the relationship which
//            is to be updated
// TRIGGERED BY:
//
//   WINDOW: ERD
//    ACTION: ArcDblClk
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DiagToRLLMaint( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vTZEREMDO, "ER_RelLink_2", "EntpER_Model" );
      if ( CompareAttributeToAttribute( vTZEREMDO, "ER_Entity", "ER_DiagramPosX", vTZEREMDO, "ER_Entity_Other", "ER_DiagramPosX" ) > 0 ||
           (CompareAttributeToAttribute( vTZEREMDO, "ER_Entity", "ER_DiagramPosX", vTZEREMDO, "ER_Entity_Other", "ER_DiagramPosX" ) == 0 &&
            CompareAttributeToAttribute( vTZEREMDO, "ER_Entity", "ER_DiagramPosX", vTZEREMDO, "ER_Entity_Other", "ER_DiagramPosX" ) > 0) )
      {
         if ( SetCursorNextEntity( vTZEREMDO, "ER_RelLink_2", "" ) < zCURSOR_SET )
         {
            SetCursorPrevEntity( vTZEREMDO, "ER_RelLink_2", "" );
         }

         SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vTZEREMDO, "ER_RelLink_2", "EntpER_Model" );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PreBuildRLLVIEW
// PURPOSE:    This Entry names a view for the Relationship
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildRLLVIEW( zVIEW vSubtask )
{
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildRLLVIEW
// PURPOSE:    This Entry sets the caption for Relationship
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildRLLVIEW( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW  vProfileXFER;
   zLONG  lPSDLG_X;
   zLONG  lPSDLG_Y;
   zLONG  lSZDLG_X;
   zLONG  lSZDLG_Y;
   zCHAR szEntityName[ 33 ];

   SetNameForView( vSubtask, "RLLVIEW", vSubtask, zLEVEL_TASK );

   // Set Caption for Entity name
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szEntityName );

   // Size the window appropriateley
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lSZDLG_X, vProfileXFER, "RLLVIEW", "SZDLG_X" );
   if ( lSZDLG_X )
   {
      GetIntegerFromAttribute( &lSZDLG_Y, vProfileXFER, "RLLVIEW", "SZDLG_Y" );
      GetIntegerFromAttribute( &lPSDLG_X, vProfileXFER, "RLLVIEW", "PSDLG_X" );
      GetIntegerFromAttribute( &lPSDLG_Y, vProfileXFER, "RLLVIEW", "PSDLG_Y" );
      SetCtrlPosition( vSubtask, 0, (zSHORT)lPSDLG_X, (zSHORT)lPSDLG_Y, TRUE );
      SetCtrlSize( vSubtask, 0, (zSHORT)lSZDLG_X, (zSHORT)lSZDLG_Y, TRUE );
   }

   if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
      EnableAction( vSubtask, "DeleteRelLink", FALSE );
   else
      EnableAction( vSubtask, "DeleteRelLink", TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_CloseRLLVIEW
// PURPOSE:    This Entry deletes the named view to the window upon close.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseRLLVIEW( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW  vProfileXFER;
   zLONG  lDLG_X;
   zLONG  lDLG_Y;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   GetCtrlSize( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "SZDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "SZDLG_Y", lDLG_Y );
   GetCtrlPosition( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "PSDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "RLLVIEW", "PSDLG_Y", lDLG_Y );

   GetViewByName( &vWindow, "RLLVIEW", vSubtask, zLEVEL_TASK );
   if ( vWindow )
      DropNameForView( vWindow, "RLLVIEW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_IsRelToSapEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_IsRelToSapEntity( zVIEW vSubtask, zVIEW vTZEREMDO )
{
   zVIEW  vERD;
   zCHAR  szEntity_Other[ 33 ];

   // Do not create a Relationship to a SAP Entity
   if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) != 0 )
   {
      GetCtrlText( vSubtask, "EntOther", szEntity_Other, zsizeof( szEntity_Other ) );
      CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );
      if ( SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szEntity_Other, "" ) >= zCURSOR_SET && CompareAttributeToInteger( vERD, "ER_Entity", "SAP_Entity", 1 ) == 0 )
      {
         SysMessageBox( vSubtask, "ER Data Model Maintenance", "It is not allowed to create a Relationship to a SAP Entity.", 1 );
         DropView( vERD );
         SetFocusToCtrl( vSubtask, "EntOther" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
      DropView( vERD );
   }

   return( 0 );
} // zwTZEREMDD_IsRelToSapEntity


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_RelLinkOK
// PURPOSE:   OK on ER_RelLink for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_RELLI
//    ACTION: BUTTON
//    NAME: OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RelLinkOK( zVIEW vSubtask )
{
   zVIEW  vProfileXfer;
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDO2;
   zLONG  lER_RelinkID;
   zLONG  lER_EntityOtherID;
   zLONG  lER_RelinkID2;
   zLONG  lER_EntityOtherID2;
   zSHORT nErrorFlag;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXfer, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // Do not create a Relationship to a SAP Entity.
   if ( zwTZEREMDD_IsRelToSapEntity( vSubtask, vTZEREMDO ) < 0 )
      return( -1 );

   // If HangingForeignKeyEntity is requested for either ER_RelLink or
   // ER_RelLink_Other, then make sure the max cardinality is 1 for that
   // RelLink entry.
   if ( CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink1HangingForeignKeyEntity", "Y" ) == 0 &&
        CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink1CardMax", "1" ) != 0 )
   {
      MessageSend( vSubtask, "ER00191", "ER Data Model Maintenance", "Max cardinality must be 1 when 'Hanging FK Entity' is specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "MaxCard" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink2HangingForeignKeyEntity", "Y" ) == 0 &&
        CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink2CardMax", "1" ) != 0 )
   {
      MessageSend( vSubtask, "ER00191", "ER Data Model Maintenance", "Max cardinality must be 1 when 'Hanging FK Entity' is specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "InvCardMax" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META ))
   {
      // Now that all validations have been passed, copy the attributes from the
      // work entity to the correct RelLink entities.
      zwTZEREMDD_CopyTempRelLink( vSubtask, vTZEREMDO );
      nRC = zwfnTZEREMDD_RedirectRelationship( vSubtask );

      // Check if there is another relationship between these two entities and if
      // so, give a warning message to the operator to let him know that foreign
      // keys might be generated incorrectly.
      CreateViewFromView( &vTZEREMDO2, vTZEREMDO );
      nErrorFlag = 0;
      GetIntegerFromAttribute( &lER_RelinkID, vTZEREMDO, "ER_RelLink", "ZKey" );
      GetIntegerFromAttribute( &lER_EntityOtherID, vTZEREMDO, "ER_Entity_Other", "ZKey" );
      nRC = SetCursorFirstEntity( vTZEREMDO2, "ER_RelLink", "" );
      while ( nRC >= zCURSOR_SET && nErrorFlag == 0 )
      {
         GetIntegerFromAttribute( &lER_RelinkID2, vTZEREMDO2, "ER_RelLink", "ZKey" );
         GetIntegerFromAttribute( &lER_EntityOtherID2, vTZEREMDO2, "ER_Entity_Other", "ZKey" );
         if ( lER_RelinkID2 != lER_RelinkID )
         {
            if ( lER_EntityOtherID2 == lER_EntityOtherID )
            {
               MessageSend( vSubtask, "ER00000", "ER Data Model Maintenance", "Another relationship exists between these two entities.\n"
                                                 "You may want to use a foreign key prefix to generate correct keys.", zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
               nErrorFlag = 1;    // Only give one such message.
            }
         }

         nRC = SetCursorNextEntity( vTZEREMDO2, "ER_RelLink", "" );
      }

      DropView( vTZEREMDO2 );
   }

   nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );

   // Select the Relationship in ER_Diagram / Subject Area and unselect
   // the other Relationships.
   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_RelLinkCancel
// PURPOSE:   Cancel ER_RelLink for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_RELLI
//    ACTION: BUTTON
//    NAME: Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RelLinkCancel( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vNew;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vNew, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NEXT, 0 );
      nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );
   }

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
//
// OPERATION: zwTZEREMDD_RelLinkNext
// PURPOSE:   Position on next ER_RelLink for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_RELLI
//    ACTION: BUTTON
//   NAME: Next
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RelLinkNext( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   nRC = zwTZEREMDD_RelLinkOK( vSubtask );
   if ( nRC < 0 )
      return( nRC );

   // Drop the name which indicates NEW is happening
   if ( GetViewByName( &vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      DropNameForView( vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK );
   }

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 0 );
   SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" );

   // Set up special mapping entity.
   zwTZEREMDD_SetUpTempRelLink( vSubtask, vTZEREMDO );
   SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 1 );

   SetFocusToCtrl( vSubtask, "RelName" );
   zwTZEREMDD_RefreshRllWnd( vSubtask );

   // select the Relationship in ER_Diagram / Subject Area and unselect
   // the other Relationships
   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_RelLinkPrev
// PURPOSE:   Position on previous ER_RelLink for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_RELLI
//    ACTION: BUTTON
//    NAME: Prev
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RelLinkPrev( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   nRC = zwTZEREMDD_RelLinkOK( vSubtask );
   if ( nRC < 0 )
      return( nRC );

   // Drop the name which indicates NEW is happening
   if ( GetViewByName( &vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      DropNameForView( vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK );
   }

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 0 );
   SetCursorPrevEntity( vTZEREMDO, "ER_RelLink", "" );

   // Set up special mapping entity.
   zwTZEREMDD_SetUpTempRelLink( vSubtask, vTZEREMDO );
   SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 1 );

   SetFocusToCtrl( vSubtask, "RelName" );
   zwTZEREMDD_RefreshRllWnd( vSubtask );

   // select the Relationship in ER_Diagram / Subject Area and unselect
   // the other Relationships
   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_IsRelLinkToSapEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_IsRelLinkToSapEntity( zVIEW  vSubtask, zVIEW  vTZRELTGT, zVIEW  vTZEREMDO )
{
   zVIEW  vWindow;

   // Do not create a Relationship from or to a SAP Entity
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "RLLMAINT" ) < 0 &&
        (CompareAttributeToInteger( vTZRELTGT, "ER_Entity", "SAP_Entity", 1 ) == 0 || CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0) )
   {
      SysMessageBox( vSubtask, "ER Data Model Maintenance", "It is not allowed to create a Relationship to a SAP Entity.", 1 );
      RemoveRelationship( vSubtask, "ERD", 1, 0 );
      return( 1 );
   }

   return( 0 );
} // zwTZEREMDD_IsRelLinkToSapEntity

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_AddRelLinkToSA
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_AddRelLinkToSA( zVIEW  vSubtask, zVIEW  vTZEREMDO, zVIEW  vTZERSASO )
{
   zLONG lZKey1, lZKey2;
   zVIEW vER2;
   zVIEW vSA2;
   zVIEW vSA3;

   GetIntegerFromAttribute( &lZKey1, vTZEREMDO, "ER_RelType", "ZKey" );

   if ( SetCursorFirstEntityByInteger( vTZERSASO, "ER_RelType", "ZKey", lZKey1, "SubjectArea" ) < zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vER2, vTZEREMDO, 0 );
      CreateViewFromViewForTask( &vSA2, vTZERSASO, 0 );
      CreateViewFromViewForTask( &vSA3, vTZERSASO, 0 );

      SetCursorPrevEntity( vTZEREMDO, "ER_RelLink_2", 0 );
      GetIntegerFromAttribute( &lZKey1, vTZEREMDO, "ER_Entity_2", "ZKey" );
      SetViewFromView( vER2, vTZEREMDO );
      SetCursorNextEntity( vER2, "ER_RelLink_2", 0 );
      GetIntegerFromAttribute( &lZKey2, vER2, "ER_Entity_2", "ZKey" );

      if ( SetCursorFirstEntityByInteger( vSA2, "ER_Entity", "ZKey", lZKey1, "SubjectArea" ) >= zCURSOR_SET &&
           SetCursorFirstEntityByInteger( vSA3, "ER_Entity", "ZKey", lZKey2, "SubjectArea" ) >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vTZERSASO, "SA_RelType", zPOS_LAST );
         IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelType", vTZEREMDO, "ER_RelType", zPOS_BEFORE );
         SetCursorLastEntity( vSA2, "ER_RelLink", 0 );
         IncludeSubobjectFromSubobject( vSA2, "ER_RelLink", vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vSA2, "ER_RelLink_Other", vER2, "ER_RelLink_2", zPOS_AFTER );
         SetCursorLastEntity( vSA3, "ER_RelLink", 0 );
         IncludeSubobjectFromSubobject( vSA3, "ER_RelLink", vER2, "ER_RelLink_2", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vSA3, "ER_RelLink_Other", vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
      }

      DropView( vER2 );
      DropView( vSA2 );
      DropView( vSA3 );
   }

   return( 0 );
} // zwTZEREMDD_AddRelLinkToSA

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddRelLink
// PURPOSE:   Add an ER_RelLink for ER_Entity
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddRelLink( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZRELTGT;
   zVIEW  vRelFwd;
   zVIEW  vRelInv;
   zVIEW  vTZERSASO;
   zVIEW  vTZERENLO;
   zVIEW  vWindow;
   zSHORT nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Get View to Target Entity by determining if diagram is active
   GetViewByName( &vTZRELTGT, "TZRELTGT", vSubtask, zLEVEL_SUBTASK );
   if ( vTZRELTGT )
   {
      DropNameForView( vTZRELTGT, "TZRELTGT", vSubtask, zLEVEL_SUBTASK );
   }
   else
   {
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "RLLMAINT" ) >= 0 )
         SetNameForView( vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK );
      else
         SetNameForView( vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_TASK );

      GetViewByName( &vTZERENLO, "TZERENLO", vSubtask, zLEVEL_TASK );
      if ( vTZERENLO )
         CreateViewFromViewForTask( &vTZRELTGT, vTZERENLO, 0 );
      else
         CreateViewFromViewForTask( &vTZRELTGT, vTZEREMDO, 0 );
   }

   // Do not create a Relationship from or to a SAP Entity
   if ( zwTZEREMDD_IsRelLinkToSapEntity( vSubtask, vTZRELTGT, vTZEREMDO ) == 1 )
      return( 0 );

   // unselect Relationship
   zwTZEREMDD_DeselectRelationship( vTZEREMDO, "ER_RelType", 0 );
   zwTZEREMDD_DeselectRelationship( vTZEREMDO, "ER_RelLink", 0 );

   zwTZEREMDD_RefreshRllWnd( vSubtask );
   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   RefreshRelationship( vWindow, "ERD", 0 );

   // Obtain views to position on both sides of a relationship
   nRC = CreateViewFromViewForTask( &vRelFwd, vTZEREMDO, 0 );
   nRC = CreateViewFromViewForTask( &vRelInv, vTZEREMDO, 0 );

   // Create ER_RelType on Right Side( side representing relationships )
   nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_RelType", zPOS_AFTER );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelType", "Fact", "Added thru ER Dialog" );

   // Create first ER_RelLink_2 on Right Side( side representing relationships )
   nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelLink_2", "Name", "has" );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelLink_2", "CardMin", "0" );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelLink_2", "CardMax", "m" );

   // Obtain view to position on Forward side of a relationship just created
   nRC = SetViewFromView( vRelFwd, vTZEREMDO );

   // Create second ER_RelLink_2 on Right Side( side representing relationships )
   nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_RelLink_2", zPOS_AFTER );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelLink_2", "Name", "for" );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelLink_2", "CardMin", "1" );
   nRC = SetAttributeFromString( vTZEREMDO, "ER_RelLink_2", "CardMax", "1" );

   // Obtain view to position on Inverse side of a relationship just created
   nRC = SetViewFromView( vRelInv, vTZEREMDO );

   // Create first ER_Entity_2 on Right Side( side representing relationships )
   // by Including ER_Entity from the Left side( side representing entities ).
   // This also creates the ER_RelLink and ER_RelType_1 under ER_Entity on the
   // Left side.  This stops on ER_RelType_1 due to DERIVED being set on it's child
   // ER_RelLink_Other.  The base view of vTZEREMDO is used as the source of the
   // include and the View created above which is positioned on the Forward side
   // of the relationship on the Right side.
   nRC = IncludeSubobjectFromSubobject( vRelFwd, "ER_Entity_2", vTZEREMDO, "ER_Entity", zPOS_AFTER );

   // Create second ER_Entity_2 on Right Side( side representing relationships )
   // by Including ER_Entity from the Left side( side representing entities ).
   // This also creates the ER_RelLink and ER_RelType_1 under ER_Entity on the
   // Left side.  This stops on ER_RelType_1 due to DERIVED being set on it's child
   // ER_RelLink_Other.  The base view of vTZEREMDO is used as the source of the
   // include and the View created above which is positioned on the Inverse side
   // of the relationship on the Right side.
   // ///////////////  N O T E  //////////////////////////////////
   // The relationship is initially created as recursive( same Entity
   // used as source and target of the relationship.  The user must use
   // the Change Entity Push Button on the Relationship detail window to
   // target the appropriate Entity.
   nRC = IncludeSubobjectFromSubobject( vRelInv, "ER_Entity_2", vTZRELTGT, "ER_Entity", zPOS_AFTER );

   // Position on the Left( side representing entities ) inorder to
   // complete the representation of a Relationship under each ER_Entity.  The two
   // entities to create under ER_Entity are ER_RelLink_Other and ER_Entity_Other
   // and note from above that this is due to DERIVED being set on ER_RelLink_Other
   nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vRelInv, "ER_RelLink_2", "EntpER_Model" );

   // This is done through an Include using ER_RelLink_2 as the Source of the
   // Include and ER_RelLink_Other as the target of the Include.  The base View
   // of vTZEREMDO( the View we just postioned on ) is used as the target of the
   // Include.  The View created earlier which is positioned on the Forward side
   // of the relationship on the Right side is used as the source of the Include.
   nRC = IncludeSubobjectFromSubobject( vTZEREMDO, "ER_RelLink_Other", vRelFwd, "ER_RelLink_2", zPOS_AFTER );

   // Position on the Left( side representing entities ) inorder to
   // complete the representation of a Relationship under each ER_Entity.  The two
   // entities to create under ER_Entity are ER_RelLink_Other and ER_Entity_Other
   // and note from above that this is due to DERIVED being set on ER_RelLink_Other
   nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vRelFwd, "ER_RelLink_2", "EntpER_Model" );

   // This is done through an Include using ER_RelLink_2 as the Source of the
   // Include and ER_RelLink_Other as the target of the Include.  The base View
   // of vTZEREMDO( the View we just postioned on ) is used as the target of the
   // Include.  The View created earlier which is positioned on the Inverse side
   // of the relationship on the Right side is used as the source of the Include.
   nRC = IncludeSubobjectFromSubobject( vTZEREMDO, "ER_RelLink_Other", vRelInv, "ER_RelLink_2", zPOS_AFTER );

   // Now we can check for SA
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( vTZERSASO )
   {
      // unselect Relationship
      zwTZEREMDD_DeselectRelationship( vTZERSASO, "SA_RelType", 0 );
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" ) >= 0 )
         RefreshRelationship( vWindow, "ERD", 0 );

      // Add RelLink to Subject Area
      zwTZEREMDD_AddRelLinkToSA( vSubtask, vTZEREMDO, vTZERSASO );
   }

   // Set up special mapping entity.
   zwTZEREMDD_SetUpTempRelLink( vSubtask, vTZEREMDO );

   SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 1 );
   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   zwTZEREMDD_RefreshRllWnd( vSubtask );
   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   RefreshRelationship( vWindow, "ERD", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_NewRelLink
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_NewRelLink( zVIEW vSubtask )
{
   if ( zwTZEREMDD_RelLinkOK( vSubtask ) < 0 )
      return( -1 );

   zwTZEREMDD_AddRelLink( vSubtask );

   return( 0 );
} // zwTZEREMDD_NewRelLink

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteRelLink
// PURPOSE:   Delete an existing ER_RelLink for an ER_Entity.  Causing a
//            a Delete of all representations of the forward and inverse
//            sides of the relationship.
// TRIGGERED BY:
//    WINDOW: RLLMAINT
//    ACTION: BUTTON
//    NAME: DelRelLink
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteRelLink( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vW;
   zVIEW  vWindow;
   zSHORT nRC;
   zSHORT nRelType = 0;
   zCHAR  szMessage[ 256 ];
   zCHAR  szEntityName[ 33 ];
   zPCHAR sz;


   // Drop the name which indicates NEW is happening
   if ( GetViewByName( &vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      DropNameForView( vTZEREMDO, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK );
   }

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTMAINT" );
   if ( vSubtask )
      strcpy_s( szEntityName, zsizeof( szEntityName ), "ER_RelLink" );
   else
   {
      strcpy_s( szEntityName, zsizeof( szEntityName ), "ER_RelType" );
      nRelType = 1;
   }

   GetSubtaskForWindowName( vSubtask, &vW, "RLLMAINT" );

   if ( vW )
   {
      //Prompt to confirm Delete
      szMessage[ 0 ] = 0;
      GetAddrForAttribute( &sz, vTZEREMDO, "ER_Entity", "Name" );
      strcat_s( szMessage, zsizeof( szMessage ), sz );
      strcat_s( szMessage, zsizeof( szMessage ), " " );
      GetAddrForAttribute( &sz, vTZEREMDO, "ER_RelLink", "Name" );
      strcat_s( szMessage, zsizeof( szMessage ), sz );
      strcat_s( szMessage, zsizeof( szMessage ), " " );
      GetAddrForAttribute( &sz, vTZEREMDO, "ER_Entity_Other", "Name" );
      strcat_s( szMessage, zsizeof( szMessage ), sz );

      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD, zENAME_ER_RELATIONSHIP, 0, 0, szMessage, 0 );
      SetSelectStateOfEntity( vTZEREMDO, szEntityName, 1 );
   }
   else
      //BL, 1999.10.19 delete all selected Relationships:
      //Prompt to confirm Delete only one
      nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete selected Relationships", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, szEntityName, "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, szEntityName, "" ) )

      {
         if ( nRelType == 1 )
         {
            SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity", vTZEREMDO, "ER_Entity_2", "" );
            SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vTZEREMDO, "ER_RelLink_2", "" );
         }

         oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NONE, 0 );
      }

      if ( vW && CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink" ) == zCURSOR_NULL )
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
      else
      if ( vW )
      {
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink" ) == zCURSOR_UNDEFINED )
            SetCursorPrevEntity( vTZEREMDO, "ER_RelLink", "" );

         zwTZEREMDD_BuildEntityView( vSubtask );
         SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 1 );
         zwTZEREMDD_SetRelationshipSelect( vSubtask );
         SetFocusToCtrl( vSubtask, "RelName" );
      }
   }

   zwTZEREMDD_RefreshRllWnd( vSubtask );
   return( 0 );
}
/* END OF OPERATION */

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteAllRelLinks
// PURPOSE:   Delete all ER_RelLinks for ER_Entity
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteAllRelLinks( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   //BL, 1999.10.19 Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all Relationships", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      // Loop and Delete all ER RelLinks
      for ( nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" ) )
         oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NONE, 0 );
   }

   nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_PositionOnTgtEntity
// PURPOSE:   Position ER listboxes to display information regarding an
//            Entity which is the target of a selected Relationship.
// TRIGGERED BY:
//    WINDOW: RLLMAINT
//    ACTION: BUTTON
//    NAME: PosTgtEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PositionOnTgtEntity( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vW;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Positon on ER_Entity on Left Side( side representing entities )
   // using ER_Entity_Other on the left side the target of the ER_RelLink.
   if ( SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vTZEREMDO, "ER_RelLink_Other", "EntpER_Model" ) >= zCURSOR_SET )
   {
      if ( GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" ) >= 0 )
         ERD_SelectEntity( vW, "ERD", 1, 1 );

      // Refresh all windows and set selection
      zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwfnTZEREMDD_RedirectRelationship
// PURPOSE:   This function is used to change the target entity of
//            a relationship. To change the target, we delete the
//            original relationship and create a new relationship.
// TRIGGERED BY:
//    WINDOW:
//    ACTION:
//    NAME:
//
////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZEREMDD_RedirectRelationship( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERENLO;
   zSHORT nRC;
   zLONG  lZKey;
   zCHAR  szRelNameFwd[ 33 ];
   zCHAR  szMinCardFwd[ 9 ];
   zCHAR  szMaxCardFwd[ 9 ];
   zCHAR  szRelNameInv[ 33 ];
   zCHAR  szMinCardInv[ 9 ];
   zCHAR  szMaxCardInv[ 9 ];

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00129", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }
   // Get Access to Entity List Object
   nRC = GetViewByName( &vTZERENLO, "TZERENLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Entity List for changing Target of Relationship." );
      MessageSend( vSubtask, "ER00130", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // See if a new 'target' entity has been selected for this relationship
   if ( GetSelectStateOfEntityForSet( vTZERENLO, "ER_Entity", 1 ) == 0 ||
        CompareAttributeToAttribute( vTZERENLO, "ER_Entity", "ZKey", vTZEREMDO, "ER_Entity_Other", "ZKey" ) == 0 )
   {
      return( 0 );
   }

   // Capture Relationship settings prior to Delete
   GetStringFromAttribute( szRelNameFwd, zsizeof( szRelNameFwd ), vTZEREMDO, "ER_RelLink", "Name" );
   GetStringFromAttribute( szMinCardFwd, zsizeof( szMinCardFwd ), vTZEREMDO, "ER_RelLink", "CardMin" );
   GetStringFromAttribute( szMaxCardFwd, zsizeof( szMaxCardFwd ), vTZEREMDO, "ER_RelLink", "CardMax" );
   GetStringFromAttribute( szRelNameInv, zsizeof( szRelNameInv ), vTZEREMDO, "ER_RelLink_Other", "Name" );
   GetStringFromAttribute( szMinCardInv, zsizeof( szMinCardInv ), vTZEREMDO, "ER_RelLink_Other", "CardMin" );
   GetStringFromAttribute( szMaxCardInv, zsizeof( szMaxCardInv ), vTZEREMDO, "ER_RelLink_Other", "CardMax" );
   GetIntegerFromAttribute( &lZKey, vTZEREMDO, "ER_Entity", "ZKey" );

   // Invoke Object Operation to delete the relationship
   if ( oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NONE, 1 ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   // Re-Add the relationship
   nRC = zwTZEREMDD_AddRelLink( vSubtask );
   // Set Captured Relationship settings prior to Delete
   SetCursorFirstEntityByInteger( vTZEREMDO, "ER_Entity_2", "ZKey", lZKey, "ER_RelType" );
   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink", vTZEREMDO, "ER_RelLink_2", "EntpER_Model" );
   SetAttributeFromString( vTZEREMDO, "ER_RelLink", "Name", szRelNameFwd );
   SetAttributeFromString( vTZEREMDO, "ER_RelLink", "CardMin", szMinCardFwd );
   SetAttributeFromString( vTZEREMDO, "ER_RelLink", "CardMax", szMaxCardFwd );
   SetAttributeFromString( vTZEREMDO, "ER_RelLink_Other", "Name", szRelNameInv );
   SetAttributeFromString( vTZEREMDO, "ER_RelLink_Other", "CardMin", szMinCardInv );
   SetAttributeFromString( vTZEREMDO, "ER_RelLink_Other", "CardMax", szMaxCardInv );


   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_SwitchLPLR
// PURPOSE:    This Entry switches to a new LPLR.  All named views to
//             the current LPLR file List must be droped first.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SwitchLPLR( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTemp;
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;

   // Get Access to current Enterprise Model Object and prompt for Save
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC > 0  &&  zwTZEREMDD_AskForSave( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }
   // Drop view to ERD File List Object
   if ( GetViewByName( &vTemp, "TZERMFLO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vTemp );

   // Get Access to current Subject Area Object and prompt for Save
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 && zwTZEREMDD_SA_AskForSave( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // Drop instance of SA Object
   if ( GetViewByName( &vTemp, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTemp );

   // Drop view to SA File List Object
   if ( GetViewByName( &vTemp, "TZERSFLO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vTemp );

   // Drop view to TZBRLOVO.
   if ( GetViewByName( &vTemp, "TZBRLOVO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropObjectInstance( vTemp );

   // Drop view to SAVIEW Window
   if ( GetViewByName( &vTemp, "SAVIEW", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vTemp, zWAB_ReturnToParentWithRefresh | zWAB_ProcessImmediateAction, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SaveAllLODs
// PURPOSE:   Save all LOD's after a Meta Model Change.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: UtilitiesSaveAllLODs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SaveAllLODs( zVIEW vSubtask )
{
   zSHORT   nRC;

   if ( MessagePrompt( vSubtask, "ER10031", "ER Data Model Maintenance", "Data Model and All LODs will be saved. Continue?",
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_YES )
   {
      nRC = zwTZEREMDD_SaveModel( vSubtask );
      if ( nRC == 0 )
         oTZZOLODO_SaveAllLODs( vSubtask, "POR" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_LoadDomainList
// PURPOSE:    This Entry prepares for the Display of a list of Domains
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_LoadDomainList( zVIEW vSubtask )
{
   zVIEW    vDomainList;
   zVIEW    vTZEREMDO;
   zVIEW    vCopy;
   zSHORT   nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Get Access to Domain List
   nRC = RetrieveViewForMetaList( vSubtask, &vDomainList, zREFER_DOMAIN_META );
   nRC = SetNameForView( vDomainList, "DomainList", vSubtask, zLEVEL_TASK );

   // Attribute List is a multi selection list, deselect all Attributes
   CreateViewFromViewForTask( &vCopy, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Attribute", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Attribute", "" ) )
   {
      nRC =SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 0 );
   }

   SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Attribute", "ZKey", vCopy, "ER_Attribute", "ZKey", "" );
   DropView( vCopy );
   SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 1 );

   zwTZEREMDD_RefreshAttWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_IncludeDomain
// PURPOSE:    This Entry includes a selected domain to an ER Attribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_IncludeDomain( zVIEW vSubtask )
{
   zVIEW    vDomainList;
   zVIEW    vTempDomainList;
   zVIEW    vDomain;
   zVIEW    vER;
   zCHAR    szAttributeName[ 33 ];
   zSHORT   nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vER, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access ER Object." );
      MessageSend( vSubtask, "ER00132", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Get Access to Domain List
   nRC = GetViewByName( &vDomainList, "DomainList", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Domain List." );
      MessageSend( vSubtask, "ER00133", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Get Access to SA Object
   CreateViewFromViewForTask( &vTempDomainList, vDomainList, 0 );
   nRC = ActivateMetaOI( vSubtask, &vDomain, vTempDomainList, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION );
   DropView( vTempDomainList );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Domain to be included." );
      MessageSend( vSubtask, "ER00134", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }
   if ( CheckExistenceOfEntity( vER, "Domain" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vER, "Domain", zREPOS_NONE );
   }

   nRC = IncludeSubobjectFromSubobject( vER, "Domain", vDomain, "Domain", zPOS_AFTER );

   // If the AttributeName is spaces, initialize it with the DomainName.
   GetCtrlText( vSubtask, "Name", szAttributeName, zsizeof( szAttributeName ) );
   if ( szAttributeName[ 0 ] == 0 )
   {
      SetAttributeFromAttribute( vER, "ER_Attribute", "Name", vER, "Domain", "Name" );
      RefreshCtrl( vSubtask, "Name" );
   }

   RefreshCtrl( vSubtask, "MaxStrLth" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_LoadSAList
// PURPOSE:   Load File List of Active Subject Areas to present in the
//            subject Area Open sub-window.
// TRIGGERED BY:
//    WINDOW: TZEROPSD
//    ACTION: WINDOW
//    NAME: WindowAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_LoadSAList(     zVIEW vSubtask )
{
   zVIEW  vT;
   zVIEW  vTZERSASO;
   zSHORT nRC;

   // Retrieve and name Subject Area File List Object
   nRC = RetrieveViewForMetaList( vSubtask, &vT, zREFER_SA_META );
   nRC = SetNameForView( vT, "TZERSFLO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) >= 0 && CheckExistenceOfEntity( vTZERSASO, "SubjectArea" ) >= zCURSOR_SET )
   {
      // if Subject Area open, then set cursor of it
      SetCursorFirstEntityByAttr( vT, "W_MetaDef", "Name", vTZERSASO, "SubjectArea", "Name", "" );
   }
   else
      SetCursorFirstEntity( vT, "W_MetaDef", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SaveSA
// PURPOSE:   Save current Subject Area.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_Save
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SaveSA( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vSA;
   zSHORT nRC;

   // Get access to ER and SA Object
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   //BL, 1999.10.21 if you move an Entity in a small Subject Area,
   //Zeidon does not ask you for save
   //this is a workaround for this problem
   GetViewByName( &vSA, "SAVIEW", vSubtask, zLEVEL_TASK );
   MapCtrl( vSA, "ERD" );

   // Make sure current SA is not empty
   if ( vTZERSASO == 0 || CheckExistenceOfEntity( vTZERSASO, "SubjectArea" ) < zCURSOR_SET )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to save empty Subject Area." );
      MessageSend( vSubtask, "ER00172", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Get access to ER Model
   // Rebuild Data Model reference in Subject Area

   nRC = SetCursorFirstEntity( vTZERSASO, "EntpER_Model", "" );
   if ( nRC >= zCURSOR_SET )
   {
      nRC = ExcludeEntity( vTZERSASO, "EntpER_Model", zREPOS_NONE );
   }

   nRC = IncludeSubobjectFromSubobject( vTZERSASO, "EntpER_Model", vTZEREMDO, "EntpER_Model", zPOS_AFTER );

   // Store the Subject Area
   nRC = CommitMetaOI( vSubtask, vTZERSASO, zSOURCE_SA_META );
   if ( nRC <= 0 )
   {
      MessageSend( vSubtask, "ER00135", "ER Data Model Maintenance", "Unable to save Subject Area.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }
   else
   {
      zwTZEREMDD_SaveModel( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_AskForSave
// PURPOSE:   Prompt before saving subject area
// TRIGGERED BY:
//    WINDOW: TZERERMDD
//    ACTION: Menu Item
//    NAME: Open SA
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_AskForSave( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vSA;
   zCHAR  szName[33];
   zCHAR  szMsg[ 254 ];
   zSHORT nRC;

   // Get Access to current SA Object and prompt for Save
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   //BL, 1999.10.21 if you move an Entity in a small Subject Area,
   //Zeidon does not ask you for save
   //this is a workaround for this problem
   GetViewByName( &vSA, "SAVIEW", vSubtask, zLEVEL_TASK );
   MapCtrl( vSA, "ERD" );

   if ( nRC > 0 && MiGetUpdateForView( vTZERSASO ) == 1 && ObjectInstanceUpdatedFromFile( vTZERSASO ) == 1 )
   {
      GetStringFromAttribute( szName, zsizeof( szName ), vTZERSASO, "SubjectArea", "Name" );
      strcpy_s( szMsg, zsizeof( szMsg ), "Subject Area '" );
      strcat_s( szMsg, zsizeof( szMsg ), szName );
      strcat_s( szMsg, zsizeof( szMsg ), "' has been modified, save it with Data Model?" );
      nRC = MessagePrompt( vSubtask, "", "Data Model Maintenance", szMsg, 0, zBUTTONS_YESNOCANCEL, zRESPONSE_YES, zICON_QUESTION );

      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "TZEREMDD", "TZEREMDD" );
         return( -1 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            nRC = zwTZEREMDD_SaveSA( vSubtask );
            if ( nRC )
               return( -2 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_OpenSA
// PURPOSE:   Activate a particular Subject Area and its related ER.
// TRIGGERED BY:
//    WINDOW: TZEROPSD
//    ACTION: BUTTON
//    NAME: Open
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_OpenSA( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZERSASO_New;
   zVIEW  vTZERSFLO;
   zVIEW  vWindow;
   zSHORT nRC;

   // Get Access to SA File List Object
   nRC = GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00173", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Activate new Subject Area
   nRC = ActivateMetaOI( vSubtask, &vTZERSASO_New, vTZERSFLO, zSOURCE_SA_META, zSINGLE | zLEVEL_APPLICATION );
   if ( nRC >= 0 )
   {
      nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
      if ( vTZERSASO )
         DropMetaOI( vSubtask, vTZERSASO );

      SetNameForView( vTZERSASO_New, "TZERSASO", vSubtask, zLEVEL_TASK );
      vTZERSASO = vTZERSASO_New;

   }
   else
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Open Subject Area." );
      MessageSend( vSubtask, "ER00136", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   //  See if Modeless Window already active
   nRC = GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" );
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModelessWindow, "TZERSAMD", "SA_Diagram" );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModelessWindow, "TZERSAMD", "SA_Diagram" );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CreateNewSA
// PURPOSE:   Create new Subject Area and build skeleton root instance.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_New...
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_CreateNewSA( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zVIEW vTZERSFLO;
   zVIEW vTZERSASO_New;
   zVIEW vProfileXFER;
   zVIEW vTZEREMDO;
   zSHORT nRC;
   zCHAR  szInName[ 120 ];
   zCHAR  szOutName[33];

   // Get access to opened ER Model and error if a ER model is not opened
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate Subject Area Name is OK
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   if (CompareAttributeToString ( vProfileXFER, "TZ", "Name", "") == 0 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Subject Area Name is required." );
      MessageSend( vSubtask, "ER00137", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   GetStringFromAttribute( szInName, zsizeof( szInName ), vProfileXFER, "TZ", "Name" );
   UfCompressName( szInName, szOutName, 8, "", "", "", "", 0 );
   SetAttributeFromString( vProfileXFER, "TZ", "Name", szOutName );

   nRC = GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 && SetCursorFirstEntityByString( vTZERSFLO, "W_MetaDef", "Name", szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Subject Area already exists." );
      MessageSend( vSubtask, "ER00138", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }


   // Activate an empty SA Object
   nRC = ActivateEmptyMetaOI( vSubtask, &vTZERSASO_New, zSOURCE_SA_META, zSINGLE | zLEVEL_APPLICATION );
   if ( nRC >= 0 )
   {
      nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
      if ( vTZERSASO )
         DropMetaOI( vSubtask, vTZERSASO );

      SetNameForView( vTZERSASO_New, "TZERSASO", vSubtask, zLEVEL_TASK );
      vTZERSASO = vTZERSASO_New;
   }
   else
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Open Subject Area." );
      MessageSend( vSubtask, "ER00139", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Build skeleton SA root Entity
   nRC = CreateMetaEntity( vSubtask, vTZERSASO, "SubjectArea", zPOS_AFTER );

   SetAttributeFromAttribute( vTZERSASO, "SubjectArea", "Name", vProfileXFER, "TZ", "Name" );
   SetAttributeFromAttribute( vTZERSASO,"SubjectArea", "Desc", vProfileXFER, "TZ", "Title" );

   // Include current Data Model into Subject Area
   nRC = IncludeSubobjectFromSubobject( vTZERSASO, "EntpER_Model", vTZEREMDO, "EntpER_Model", zPOS_AFTER );

   // Name SA view for mapping support and future reference
   nRC = SetNameForView( vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );


   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_NewSA_OK
// PURPOSE:   Set focaus for new SA
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_New...
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_NewSA_OK( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zVIEW vWindow;
   zSHORT nRC;

   if ( zwTZEREMDD_CreateNewSA ( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }


   //  Get access to SA object
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   //  See if Modeless Window already active
   nRC = GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" );
      InvokeWindowActionBehavior( vWindow, zWAB_ReturnToParent, 0, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModelessWindow, "TZERSAMD", "SA_Diagram" );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModelessWindow, "TZERSAMD", "SA_Diagram" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ExitSA
// PURPOSE:   Exit Subject Area Dialog by first saving the current SA.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME: File_Exit
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ExitSA( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zSHORT nRC;

   // Get access to current SA
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00140", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   zwTZEREMDD_SaveSA( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteSA
// PURPOSE:
// TRIGGERED BY:
//    WINDOW: TZEROPMD
//    ACTION: BUTTON
//    NAME: &Delete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_DeleteSA( zVIEW vSubtask )
{
   zVIEW vTZERSFLO;
   zSHORT nRC;

   // Get Access to SA File List Object
   nRC = GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );

   // Prompt to confirm Delete
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD, zREFER_SA_META, vTZERSFLO, "W_MetaDef", "Name", 0 );

   // Delete Data Model Object
   if ( nRC == zRESPONSE_YES )
   {
      nRC = DeleteMetaOI( vSubtask, vTZERSFLO, zREFER_SA_META );
      SetCursorPrevEntity( vTZERSFLO, "W_MetaDef", "" );
   }

   else
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SaveAsSA
// PURPOSE:   Save Subject Area under a new name
// TRIGGERED BY:
//    WINDOW: TZERSAMD
//    ACTION: BUTTON
//    NAME: &OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_SaveAsSA( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zSHORT nRC;

   // Get access to SA
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00141", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Save SA
   zwTZEREMDD_SaveSA( vSubtask );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddAllEntitiesToSA
// PURPOSE:   Create an SA_Entity and it's subordinates for every ER_Entity
//            in the current Data Model.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAllEntitiesToSA
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_AddAllEntitiesToSA( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERSASO;
   zVIEW vSAVIEW;
   zSHORT nRC;

   // Get access to Data Model
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00142", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Get access to Subject Area
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00143", "ER Data Model Maintenance", "Unable to Access Subject Area.  SA must be opened.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK ); //Get SAVIEW if wnd open

   // Add SA_Entity and Include ER_Entity for each ER_Entity in the ER_Model
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vTZEREMDO );
      nRC = SetCursorNextEntity( vTZERSASO, "SA_Entity", "" );
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_Entity", "" );
   }

   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", "" );
   nRC = SetCursorFirstEntity( vTZERSASO, "SA_Entity", "" );

   if ( vSAVIEW )
   {
      zwfnTZEREMDD_PosNonPosEntsSA( vSAVIEW, vTZERSASO );
      zwTZEREMDD_ZoomDiagram( vSAVIEW );
   }

   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddSelectedEntsToSA
// PURPOSE:   Create an SA_Entity and it's subordinates for every ER_Entity
//            in the current Data Model.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddSelectedEntsToSA
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_AddSelectedEntsToSA( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERSASO;
   zVIEW vSAVIEW;
   zSHORT nRC;

   // Get access to ER Model
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00144", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Get access to Subject Area
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00145", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }
   GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK ); //Get SAVIEW if wnd open

   // Add SA_Entity and Include ER_Entity for each ER_Entity in the ER_Model
   for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", 0 ) )
   {
      nRC = oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vTZEREMDO );
   }  // END for all Selected ER_Entity

   nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", 0 );

   if ( vSAVIEW )
   {
      zwfnTZEREMDD_PosNonPosEntsSA( vSAVIEW, vTZERSASO );
   }

   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddOneEntityToSA
// PURPOSE:   Create an SA_Entity and it's subordinates for a ER_Entity
//            in the current ER Model.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddOneEntityToSA
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_AddOneEntityToSA( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERSASO;
   zVIEW vSAVIEW;
   zSHORT nRC;

   // Get access to ER Model
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00146", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Get access to Subject Area
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00147", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }
   GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK ); //Get SAVIEW if wnd open

   oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vTZEREMDO );

   nRC = SetCursorNextEntity( vTZERSASO, "SA_Entity", "" );

   // Now select our currently selected SA Entity (new)
   // nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity", vTZERSASO, "ER_Entity", "" );

   // Now select our currently selected SA Entity (new)
   // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.
   nRC = SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", "" );

   if ( vSAVIEW )
   {
      zwfnTZEREMDD_PosNonPosEntsSA( vSAVIEW, vTZERSASO );
   }
   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ExcludeOneEntityFrSA
// PURPOSE:   Delete an SA_Entity and it's subordinates.
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: ExcludeOneEntityFrSA
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ExcludeOneEntityFrSA( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00148", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Get Access to SA
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00149", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Exclude ER_Entity by deleting the SA_Entity
   // Prompt to confirm Delete
   for ( nRC = SetCursorFirstSelectedEntity( vTZERSASO, "SA_Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZERSASO, "SA_Entity", 0 ) )
   {
         oTZERSASO_RemoveOneSA_Entity( vTZERSASO );
   }

   if ( SetCursorFirstSelectedEntity( vTZERSASO, "SA_Entity", 0 ) != zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vTZERSASO, "SA_Entity" ) >= zCURSOR_SET )
      {
         // Now select our currently selected SA Entity
         // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.
         nRC = SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", "" );
      }
   }

   zwTZEREMDD_RefreshEntitySelect( vSubtask );
   RefreshCtrl( vSubtask, "SA_Entity");

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddAllEntsViaRels
// PURPOSE:   Create an SA_Entity and it's subordinates for every ER_Entity
//            that is the target of a Rel from the current ER_Entity
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAllEntitiesToSA
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_AddAllEntsViaRels( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vT;
   zVIEW  vTZERSASO;
   zVIEW  vSAVIEW;
   zLONG  lEntityZKey;
   zSHORT nRC;


   // Get access to ER Model
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00150", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }
   nRC = CreateViewFromViewForTask( &vT, vTZEREMDO, 0 );

   // Get access to Subject Area
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00151", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Add SA_Entity and Include ER_Entity for each ER_Entity in the ER_Model
   oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vTZEREMDO );

   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = GetIntegerFromAttribute( &lEntityZKey, vTZEREMDO, "ER_Entity_Other", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vT, "ER_Entity", "ZKey", lEntityZKey, "" );
      if ( nRC > zCURSOR_UNCHANGED )
      {
         oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vT );
         nRC = SetCursorNextEntity( vTZERSASO, "SA_Entity", "" );
      }

      nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" );
   }
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );

   // Now select our currently selected SA Entity
   // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.
   nRC = SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", "" );

   GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK ); //Get SAVIEW if wnd open
   if ( vSAVIEW )
   {
      zwfnTZEREMDD_PosNonPosEntsSA( vSAVIEW, vTZERSASO );
   }
   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddOneEntViaRel
// PURPOSE:
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddOneEntViaRel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_AddOneEntViaRel( zVIEW vSubtask )
{

   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vT;
   zVIEW  vSAVIEW;
   zLONG  lEntityZKey;
   zSHORT nRC;

   // Get access to Data Model
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00152", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   nRC = CreateViewFromViewForTask( &vT, vTZEREMDO, 0 );

   // Get access to Subject Area
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. SA must be opened." );
      MessageSend( vSubtask, "ER00154", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Add SA_Entity and based on ER_Entity_Other
   nRC = GetIntegerFromAttribute( &lEntityZKey, vT, "ER_Entity_Other", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vT, "ER_Entity", "ZKey", lEntityZKey, "" );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vT );
   }

   nRC = SetCursorNextEntity( vTZERSASO, "SA_Entity", "" );

   // Now select our currently selected SA Entity
   // Frank H. (97/04/15): if the ERD is not checked out then the SA is not correct linked to the ERD --> SetCursor by ZKey.
   nRC = SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey", vTZERSASO, "ER_Entity", "ZKey", "" );

   GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK ); //Get SAVIEW if wnd open
   if ( vSAVIEW )
   {
      zwfnTZEREMDD_PosNonPosEntsSA( vSAVIEW, vTZERSASO );
   }

   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortSAEntName
// PURPOSE:   Sort SA Entities by Name
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   SortSAEntName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortSAEntName( zVIEW vSubtask )
{
   zVIEW vTZERSASO;

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   zgSortEntityWithinGParent4( vTZERSASO, "SA_Entity", "ER_Entity", "Name", zASCENDING, "", 0, "", 0, "", 0 );
   SetCursorFirstEntity( vTZERSASO, "SA_Entity", "" );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortSAEntOwner
// PURPOSE:   Sort SA Entities by Owner, Name
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   SortSAEntOwner
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortSAEntOwner( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinGParent4( vTZERSASO, "SA_Entity", "ER_Entity", "OwningArea", zASCENDING, "Name", zASCENDING, "", 0, "", 0 );
   nRC = SetCursorFirstEntity( vTZERSASO, "SA_Entity", "" );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SortSAEntZKey
// PURPOSE:   Sort SA Entities by ZKey
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: OPTION
//    NAME:   SortSAEntZKey
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SortSAEntZKey( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent4( vTZERSASO, "SA_Entity", "ZKey", zASCENDING, "", 0, "", 0, "", 0 );
   nRC = SetCursorFirstEntity( vTZERSASO, "SA_Entity", "" );
   zwTZEREMDD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SASortByName( zVIEW    vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   nRC = GetViewByName( &vT, "TZERSFLO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vT, "W_MetaDef", "Name", "" );
   nRC = SetCursorFirstSelectedEntity( vT, "W_MetaDef", 0 );
   if ( nRC <  zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vT, "W_MetaDef", "" );
      nRC = SetSelectStateOfEntityForSet( vT, "W_MetaDef", 1, 1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SASortByDate( zVIEW    vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   nRC = GetViewByName( &vT, "TZERSFLO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zDESCENDING, vT, "W_MetaDef", "LastUpdateDate", "" );
   nRC = SetCursorFirstSelectedEntity( vT, "W_MetaDef", 0 );
   if ( nRC <  zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vT, "W_MetaDef", "" );
      nRC = SetSelectStateOfEntityForSet( vT, "W_MetaDef", 1, 1 );
   }

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SASortByDesc( zVIEW vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   nRC = GetViewByName( &vT, "TZERSFLO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vT, "W_MetaDef", "Desc", "" );
   nRC = SetCursorFirstSelectedEntity( vT, "W_MetaDef", 0 );
   if ( nRC <  zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vT, "W_MetaDef", "" );
      nRC = SetSelectStateOfEntityForSet( vT, "W_MetaDef", 1, 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PreBuildIDVIEW
// PURPOSE:    This Entry names a view for the Identifier
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildIDVIEW( zVIEW vSubtask )
{
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildIDVIEW
// PURPOSE:    This Entry sets the caption for the Identifier
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildIDVIEW( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW  vProfileXFER;
   zLONG  lPSDLG_X;
   zLONG  lPSDLG_Y;
   zLONG  lSZDLG_X;
   zLONG  lSZDLG_Y;

   zCHAR szEntityName[ 33 ];

   SetNameForView( vSubtask, "IDVIEW", vSubtask, zLEVEL_TASK );

   // Set Caption for Entity name
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szEntityName );

   // Size the window appropriateley
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lSZDLG_X, vProfileXFER, "IDVIEW", "SZDLG_X" );
   if ( lSZDLG_X )
   {
      GetIntegerFromAttribute( &lSZDLG_Y, vProfileXFER, "IDVIEW", "SZDLG_Y" );
      GetIntegerFromAttribute( &lPSDLG_X, vProfileXFER, "IDVIEW", "PSDLG_X" );
      GetIntegerFromAttribute( &lPSDLG_Y, vProfileXFER, "IDVIEW", "PSDLG_Y" );
      SetCtrlPosition( vSubtask, 0, (zSHORT)lPSDLG_X, (zSHORT)lPSDLG_Y, TRUE );
      SetCtrlSize( vSubtask, 0, (zSHORT)lSZDLG_X, (zSHORT)lSZDLG_Y, TRUE );
   }

   if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
      EnableAction( vSubtask, "DeleteIdentifier", FALSE );
   else
      EnableAction( vSubtask, "DeleteIdentifier", FALSE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_CloseIDVIEW
// PURPOSE:    This Entry deletes the named view to the window upon close.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseIDVIEW( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW vProfileXFER;
   zLONG lDLG_X;
   zLONG lDLG_Y;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   GetCtrlSize( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "IDVIEW", "SZDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "IDVIEW", "SZDLG_Y", lDLG_Y );
   GetCtrlPosition( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "IDVIEW", "PSDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "IDVIEW", "PSDLG_Y", lDLG_Y );

   GetViewByName( &vWindow, "IDVIEW", vSubtask, zLEVEL_TASK );
   if ( vWindow )
      DropNameForView( vWindow, "IDVIEW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_IdentifierOK
// PURPOSE:   OK on ER_EntIdentifier for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_ATTRI
//    ACTION: BUTTON
//    NAME: OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_IdentifierOK( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Validate ER_Attribute.
   // Accept ER_EntIdentifer, if one already exists.
   if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_EntIdentifier", "Identifier", "TE_KeyName", "TE Index Name" ) < 0 )
      return( -1 );

   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );
   zwTZEREMDD_RefreshIdentifierWnd( vSubtask );

   return( 0 );
}
/* END OF OPERATION */


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_IdentifierCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_IdentifierCancel( zVIEW vSubtask )
{
   zVIEW    vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   CancelSubobject( vTZEREMDO, "ER_EntIdentifier" );

   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_EntIdentifier" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );

   zwTZEREMDD_RefreshIdentifierWnd( vSubtask );

   return( 0 );
} // zwTZEREMDD_IdentifierCancel

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_IdentifierNext
// PURPOSE:   Position on next ER_EntIdentifier for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_ATTRI
//    ACTION: BUTTON
//    NAME: Next
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_IdentifierNext( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Accept ER_EntIdentifer, if one already exists.
   if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_EntIdentifier", "Identifier", "TE_KeyName", "TE Index Name" ) < 0 )
   {
      return( -1 );
   }

   // Create skeleton ER_Identifier entity
   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 0 );
   SetCursorNextEntity( vTZEREMDO, "ER_EntIdentifier", "" );

   CreateTemporalSubobjectVersion( vTZEREMDO, "ER_EntIdentifier" );
   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );

   zwTZEREMDD_PreBuildIDATTADD( vSubtask );
   zwTZEREMDD_PreBuildIDRLLADD( vSubtask );

   SetFocusToCtrl( vSubtask, "Name" );
   zwTZEREMDD_RefreshIdentifierWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_IdentifierPrev
// PURPOSE:   Position on previous ER_EntIdentifier for ER_Entity
// TRIGGERED BY:
//    WINDOW: ER_ATTRI
//    ACTION: BUTTON
//    NAME: Prev
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_IdentifierPrev( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Accept ER_EntIdentifer, if one already exists.
   if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_EntIdentifier", "Identifier", "TE_KeyName", "TE Index Name" ) < 0 )
   {
      return( -1 );
   }

   // Create skeleton ER_Identifier entity
   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 0 );
   SetCursorPrevEntity( vTZEREMDO, "ER_EntIdentifier", "" );

   CreateTemporalSubobjectVersion( vTZEREMDO, "ER_EntIdentifier" );
   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );

   zwTZEREMDD_PreBuildIDATTADD( vSubtask );
   zwTZEREMDD_PreBuildIDRLLADD( vSubtask );

   SetFocusToCtrl( vSubtask, "Name" );
   zwTZEREMDD_RefreshIdentifierWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_PreBuildIDATTADD
// PURPOSE:   Prebuild of Attributes for Identifier selection
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildIDATTADD( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERATLO;
   zVIEW vTemp;
   zSHORT nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Activate Empty Attribute List Object
   if ( GetViewByName( &vTZERATLO, "TZERATLO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZERATLO );

   ActivateEmptyObjectInstance( &vTZERATLO, "TZERATLO", vTZEREMDO, zMULTIPLE );
   CreateEntity( vTZERATLO, "TZERATLO", zPOS_AFTER );
   SetAttributeFromString( vTZERATLO, "TZERATLO", "Name", "Work" );
   SetNameForView( vTZERATLO, "TZERATLO", vSubtask, zLEVEL_TASK );

   // Include ER_Attribute to ER_Attribute
   nRC = CreateViewFromViewForTask( &vTemp, vTZEREMDO, 0 );
   nRC = SetCursorFirstEntity( vTemp, "ER_Attribute", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vTemp, "ER_AttributeIdentifier", vTemp, "ER_Attribute", "ER_EntIdentifier" );
      if ( nRC < zCURSOR_SET )
      {
         CreateEntity( vTZERATLO, "ER_Attribute", zPOS_AFTER );
         SetMatchingAttributesByName( vTZERATLO, "ER_Attribute", vTemp, "ER_Attribute", zSET_ALL );
      }
      nRC = SetCursorNextEntity( vTemp, "ER_Attribute", "" );
   }
   nRC = SetCursorFirstEntity( vTZERATLO, "ER_Attribute", "" );

   DropView( vTemp );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_PreBuildIDRLLADD
// PURPOSE:   Prebuild of Relationships for Identifier selection
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildIDRLLADD( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZEREMDO_RLL;
   zVIEW vTZERRLLO;
   zVIEW vTemp;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   nRC = CreateViewFromViewForTask( &vTZEREMDO_RLL, vTZEREMDO, 0 );
   SetNameForView( vTZEREMDO_RLL, "TZEREMDO_RLL", vSubtask, zLEVEL_TASK );

   // Activate Empty Relationship List Object
   if ( GetViewByName( &vTZERRLLO, "TZERRLLO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZERRLLO );

   nRC = ActivateEmptyObjectInstance( &vTZERRLLO, "TZERRLLO", vTZEREMDO, zMULTIPLE );
   CreateEntity( vTZERRLLO, "TZERRLLO", zPOS_AFTER );
   SetNameForView( vTZERRLLO, "TZERRLLO", vSubtask, zLEVEL_TASK );

   // Include ER_RelLink_Other to ER_RelLink_Other in List Object
   nRC = CreateViewFromViewForTask( &vTemp, vTZEREMDO, 0 );
   nRC = SetCursorFirstEntity( vTemp, "ER_RelLink", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vTemp, "ER_RelLinkIdentifier", vTemp, "ER_RelLink_Other", "ER_EntIdentifier" );
      if (( nRC < zCURSOR_SET ) && ( CompareAttributeToString( vTemp, "ER_RelLink", "CardMax", "1" ) == 0 ))
      {
         nRC = CreateEntity( vTZERRLLO, "ER_RelLink_Other", zPOS_AFTER );
         SetMatchingAttributesByName( vTZERRLLO, "ER_RelLink_Other", vTemp, "ER_RelLink_Other", zSET_ALL );
         nRC = CreateEntity( vTZERRLLO, "ER_Entity_Other", zPOS_AFTER );
         SetMatchingAttributesByName( vTZERRLLO, "ER_Entity_Other", vTemp, "ER_Entity_Other", zSET_ALL );
      }
      nRC = SetCursorNextEntity( vTemp, "ER_RelLink", "" );
   }
   nRC = SetCursorFirstEntity( vTZERRLLO, "ER_RelLink_Other", "" );

   DropView( vTemp );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddIdentifier
// PURPOSE:   Add ER_EntIdentifier for ER_Entity
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddIdentifier( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Accept ER_EntIdentifer, if one already exists.
   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_EntIdentifier" ) >= zCURSOR_SET )
   {
      if ( zwTZEREMDD_AcceptSubobject( vSubtask, "ER_EntIdentifier", "Identifier", "TE_KeyName", "TE Index Name" ) < 0 )
      {
         return( -1 );
      }
      SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 0 );
   }

   // Create skeleton ER_Identifier entity
   CreateTemporalMetaEntity( vSubtask, vTZEREMDO, "ER_EntIdentifier", zPOS_AFTER );
   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );

   zwTZEREMDD_PreBuildIDATTADD( vSubtask );
   zwTZEREMDD_PreBuildIDRLLADD( vSubtask );

   SetFocusToCtrl( vSubtask, "Name" );
   zwTZEREMDD_RefreshIdentifierWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddAttrToIdentifier
// PURPOSE:   Add ER_Attribute to ER_EntIdentifier
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddAttrToIdentifier( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERATLO;
   zVIEW vTemp;
   zSHORT nRC;
   zLONG  lZKey;
   zPCHAR szName;
   zCHAR  szValue[ 2 ];

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTemp, vTZEREMDO, 0 );

   nRC = GetViewByName( &vTZERATLO, "TZERATLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      return( -1 );
   }

   // Include ER_Attribute under ER_EntIdentifier entity
   nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_FactType", zPOS_AFTER );
   GetIntegerFromAttribute( &lZKey, vTZERATLO, "ER_Attribute", "ZKey" );
   SetCursorFirstEntityByInteger( vTemp, "ER_Attribute", "ZKey", lZKey, 0 );

   nRC = IncludeSubobjectFromSubobject( vTZEREMDO, "ER_AttributeIdentifier", vTemp, "ER_Attribute", zPOS_AFTER );
   nRC = DeleteEntity( vTZERATLO, "ER_Attribute", zREPOS_NEXT );

   // Set Name if it is null from attribute just selected
   GetCtrlText( vSubtask, "Name", szValue, zsizeof( szValue ) );
   if (szValue[ 0 ] == 0 )
   {
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_AttributeIdentifier", "Name" );
      SetCtrlText( vSubtask, "Name", szName );
   }

   MapWindow( vSubtask );
   DropView( vTemp );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CancelAddAttrToId
// PURPOSE:   Cancel of add ER_Attribute to ER_EntIdentifier
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CancelAddAttrToId( zVIEW vSubtask )
{
   zVIEW vTZERATLO;
   zSHORT nRC;

   // Get Access to Attribute List Object
   nRC = GetViewByName( &vTZERATLO, "TZERATLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
      return( 0 );

   nRC = DropView( vTZERATLO );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddIdViaAttr
// PURPOSE:   Add ER_EntIdentifier for ER_Entity based on Attribute
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddIdViaAttr( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00159", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Create skeleton ER_Identifier entity
   nRC = zwTZEREMDD_AddIdentifier( vSubtask );
   nRC = zwTZEREMDD_AddAttrToIdentifier( vSubtask );

   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DfltAddIdViaAttr
// PURPOSE:   Add ER_EntIdentifier for each ER_Entity based on Attribute
//            being not null and participating in identification(KEY=Y)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DfltAddIdViaAttr( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00160", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", "" );
   while ( nRC >= zCURSOR_SET )
   {
      //  Setup Name for Default Identifier
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "ID_" );
      GetStringFromAttribute( szWorkMessage + 3, zsizeof( szWorkMessage ) - 3, vTZEREMDO, "ER_Entity", "Name" );
      UfCompressName( szWorkMessage, szWorkMessage, 32, "_", "", "B", "A_", 0 );

      nRC = SetCursorFirstEntityByString( vTZEREMDO, "ER_Attribute", "Key", "Y", "" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vTZEREMDO, "ER_Attribute", "NotNull", "Y" ) == 0 )
         {  // check to see if default exists
            if ( SetCursorFirstEntityByString( vTZEREMDO, "ER_EntIdentifier", "Name", szWorkMessage, "" ) <  zCURSOR_SET )
            {  //  No, Create skeleton ER_Identifier entity
               CreateMetaEntity( vSubtask, vTZEREMDO, "ER_EntIdentifier", zPOS_AFTER );
               SetAttributeFromString( vTZEREMDO, "ER_EntIdentifier", "Name", szWorkMessage );
            }
            // check to see if Attribute exists( handle multi-part keys )
            GetStringFromAttribute( szWorkMessage + 64, zsizeof( szWorkMessage ) - 64, vTZEREMDO, "ER_Attribute", "Name" );
            if ( SetCursorFirstEntityByString( vTZEREMDO, "ER_AttributeIdentifier", "Name", szWorkMessage + 64, "" ) <    zCURSOR_SET )
            {
               CreateMetaEntity( vSubtask, vTZEREMDO, "ER_FactType", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vTZEREMDO, "ER_AttributeIdentifier", vTZEREMDO, "ER_Attribute", zPOS_AFTER );
            }
         }
         nRC = SetCursorNextEntityByString( vTZEREMDO, "ER_Attribute", "Key", "Y", "" );
      }
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_Entity", "" );
   }

   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", "" );
   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddRelLinkToId
// PURPOSE:   Add ER_RelLink to ER_EntIdentifier
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddRelLinkToId( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERRLLO;
   zVIEW vTemp;
   zSHORT nRC;
   zLONG lZKey;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTemp, vTZEREMDO, 0 );

   // Get Access to ER Object
   nRC = GetViewByName( &vTZERRLLO, "TZERRLLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      return( -1 );
   }

   // Include ER_RelLink_Other under ER_EntIdentifier entity
   nRC = CreateMetaEntity( vSubtask, vTZEREMDO, "ER_FactType", zPOS_AFTER );

   GetIntegerFromAttribute( &lZKey, vTZERRLLO, "ER_RelLink_Other", "ZKey" );
   SetCursorFirstEntityByInteger( vTemp, "ER_RelLink_2", "ZKey", lZKey, "EntpER_Model" );
   nRC = IncludeSubobjectFromSubobject( vTZEREMDO, "ER_RelLinkIdentifier", vTemp, "ER_RelLink_2", zPOS_AFTER );
   nRC = DeleteEntity( vTZERRLLO, "ER_RelLink_Other", zREPOS_NEXT );

   MapWindow( vSubtask );
   DropView( vTemp );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_CancelAddRelLinkToId
// PURPOSE:   Cancel of add ER_RelLink to ER_EntIdentifier
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CancelAddRelLinkToId( zVIEW vSubtask )
{
   zVIEW vTZERRLLO;
   zSHORT nRC;

   // Get Access to Attribute List Object
   nRC = GetViewByName( &vTZERRLLO, "TZERRLLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
      return( 0 );

   nRC = DropView( vTZERRLLO );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_AddIdViaRel
// PURPOSE:   Add ER_EntIdentifier for ER_Entity based on Relationship
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddAtt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddIdViaRel( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00161", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Make sure relationship is valid relationship.
   if ( CompareAttributeToString( vTZEREMDO, "ER_RelLink", "CardMax", "1" ) == 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Relationship being added has a Max Cardinality\n" );
      strcat_s( szWorkMessage, zsizeof( szWorkMessage ), "on the inverse side that is not equal to 1.\n\n" );
      strcat_s( szWorkMessage, zsizeof( szWorkMessage ), "Select another Relationship." );
      MessageSend( vSubtask, "ER00162", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Create skeleton ER_Identifier entity
   nRC = zwTZEREMDD_AddIdentifier( vSubtask );
   nRC = zwTZEREMDD_AddRelLinkToId( vSubtask );

   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteIdentifier
// PURPOSE:   Delete ER_EntIdentifier for ER_Entity
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteIdentifier( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vW;
   zSHORT nRC;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vSubtask, &vW, "IDMAINT" );

   if ( vW )
   {
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD, zENAME_ER_IDENTIFIER, vTZEREMDO, "ER_EntIdentifier", "Name", 0 );
      SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );
   }
   else
      //BL, 1999.10.19 Delete all selected Identifier:
      //Prompt to confirm Delete only one
      nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete selected Identifiers", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   // Delete all delected ER Identifiers
   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_EntIdentifier", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_EntIdentifier", "" ) )
      {
         nRC = DeleteEntity( vTZEREMDO, "ER_EntIdentifier", zREPOS_NONE );
      }

      if ( vW && CheckExistenceOfEntity( vTZEREMDO, "ER_EntIdentifier" ) == zCURSOR_NULL )
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      else
      if ( vW )
      {
         SetCursorPrevEntity( vTZEREMDO, "ER_EntIdentifier", "" );
         CreateTemporalSubobjectVersion( vTZEREMDO, "ER_EntIdentifier" );
         SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );
         zwTZEREMDD_PreBuildIDATTADD( vSubtask );
         zwTZEREMDD_PreBuildIDRLLADD( vSubtask );
         SetFocusToCtrl( vSubtask, "Name" );
      }

      zwTZEREMDD_RefreshIdentifierWnd( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteAllIdentifiers
// PURPOSE:   Delete all ER_EntIdentifiers for ER_Entity
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteAllIdentifiers( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00163", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all Identifiers", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      // Loop and Delete all ER EntIdentifiers
      nRC = SetCursorFirstEntity( vTZEREMDO, "ER_EntIdentifier", "" );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = DeleteEntity( vTZEREMDO, "ER_EntIdentifier", zREPOS_NONE );
         nRC = SetCursorNextEntity( vTZEREMDO, "ER_EntIdentifier", "" );
      }
   }

   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_DeleteFactTypeFromId
// PURPOSE:   Delete ER_FactType for ER_AttributeIdentifier
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DeleteFactTypeFromId( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZERATLO;
   zVIEW vTZERRLLO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
   {
      // FactType is Attribute.
      // Add attribute as candidate to ERATRLO object
      nRC = GetViewByName( &vTZERATLO, "TZERATLO", vSubtask, zLEVEL_TASK );
      if ( nRC > 0 )
      {
         CreateEntity( vTZERATLO, "ER_Attribute", zPOS_AFTER );
         SetMatchingAttributesByName( vTZERATLO, "ER_Attribute", vTZEREMDO, "ER_AttributeIdentifier", zSET_ALL );
      }
   }
   else
   {
      // FactType is Relationship.
      // Add rel back as cndidate to ERRLLO object
      nRC = GetViewByName( &vTZERRLLO, "TZERRLLO", vSubtask, zLEVEL_TASK );
      if ( nRC > 0 )
      {
         CreateEntity( vTZERRLLO, "ER_RelLink_Other", zPOS_AFTER );
         SetMatchingAttributesByName( vTZERRLLO, "ER_RelLink_Other", vTZEREMDO, "ER_RelLinkIdentifier", zSET_ALL );
         CreateEntity( vTZERRLLO, "ER_Entity_Other", zPOS_AFTER );
         SetMatchingAttributesByName( vTZERRLLO, "ER_Entity_Other", vTZEREMDO, "ER_Entity_Other_Identifier", zSET_ALL );
      }
   }

   nRC = DeleteEntity( vTZEREMDO, "ER_FactType", zREPOS_NEXT );
   nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwfnTZEREMDO_SetEntityState
// PURPOSE:   Add Entity to Data Model
// TRIGGERED BY:
//    WINDOW: TZEREMDD
//    ACTION: BUTTON
//    NAME: AddEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDO_SetEntityState( zVIEW vSubtask, zVIEW vTZEREMDO, zBOOL bSelect )
{
   zVIEW vDialog;

   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Entity" ) < zCURSOR_SET )
      return( 0 );

   SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", bSelect, 1 );

   GetViewByName( &vDialog, "TZEREMDD", vSubtask, zLEVEL_TASK );
   if ( vDialog )
      RefreshEntity( vDialog, "ERD" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildATTR
// PURPOSE:    This Entry sets the Graying for the Add & Delete
//             Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildATTR( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nEnable;
   zSHORT nEnableWork = 1;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( !vTZEREMDO || MiGetUpdateForView( vTZEREMDO ) == 0 )
   {
      nEnable = 0;
      nEnableWork = 0;
   }
   else
   {
      nEnable = 1;
      // if SAP Attribute, disable TE Attribute Fields
      if ( CompareAttributeToInteger( vTZEREMDO, "ER_Attribute", "SAP_Attribute", 1 ) == 0 )
      {
         SetCtrlState( vSubtask, "edTE_ColumnNameForAttibute", zCONTROL_STATUS_ENABLED, 0 );
         SetCtrlState( vSubtask, "edTE_ColumnNameForFK", zCONTROL_STATUS_ENABLED, 0 );
      }
      else
      {
         SetCtrlState( vSubtask, "edTE_ColumnNameForAttibute", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "edTE_ColumnNameForFK", zCONTROL_STATUS_ENABLED, 1 );
         // if SAP Entity and not SAP Attribute, Attribute is a Work Attribute, disable Work CheckBox
         if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 )
            nEnableWork = 0;
      }
   }

   SetCtrlState( vSubtask, "AddBtn", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Domain", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Work", zCONTROL_STATUS_ENABLED, nEnableWork );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildID
// PURPOSE:    This Entry sets the Graying for the Add & Delete
//             Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildID( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object.
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // If Data Model not checked out or Entity is a SAP Entity, disable all functions for update.
   if ( !vTZEREMDO || MiGetUpdateForView( vTZEREMDO ) == 0 || CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 )
   {
      SetCtrlState( vSubtask, "AddBtn",      zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "Delete",      zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "SysMaint",    zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "edIndexName", zCONTROL_STATUS_ENABLED, 0 );
      EnableAction( vSubtask, "AddAttributeToIdentifier", FALSE );
      EnableAction( vSubtask, "RemoveFactTypeFromIdentifier", FALSE );
      EnableAction( vSubtask, "AddRelLinkToIdentifier", FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DisableCtrlForRLL
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_DisableCtrlForRLL( zVIEW   vSubtask, zVIEW   vTZEREMDO )
{

   zSHORT nEnable   = 1;

   if ( !vTZEREMDO || MiGetUpdateForView( vTZEREMDO ) == 0 )
      nEnable = 0;

   if ( nEnable == 1 )
   {
      // If Relationship to a SAP Entity, disable for update.
      if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 || CompareAttributeToInteger( vTZEREMDO, "ER_Entity_Other", "SAP_Entity", 1 ) == 0 )
      {
         nEnable = 0;
      }
   }

   SetCtrlState( vSubtask, "AddBtn", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nEnable );

   SetCtrlState( vSubtask, "RelName", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "MinCard", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "MaxCard", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AutoSeq", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "HangingFKEntity", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Prefix", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "NoTEFKeyPrefix", zCONTROL_STATUS_ENABLED, nEnable );

   SetCtrlState( vSubtask, "EntOther", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "edManyToMany", zCONTROL_STATUS_ENABLED, nEnable );

   SetCtrlState( vSubtask, "InvRelName", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "InvMinCard", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "InvCardMax", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AutoSeq_Other", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "HangingFKEntity_Other", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Prefix_Other", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "NoTEFKeyPrefix_Other", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
} // zwTZEREMDD_DisableCtrlForRLL

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildRLL
// PURPOSE:    This Entry sets the Graying for the Add & Delete
//             Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildRLL( zVIEW vSubtask )
{
   zVIEW    vTZEREMDO;
   zVIEW    vProfileXfer;
   zVIEW    vNew;
   zVIEW    vCopy;
   zSHORT   nRC;

   // Get Access to ER Object.
   if ( GetViewByName( &vNew, "RLL_ENTITY_ADD", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropNameForView( vNew, "RLL_ENTITY_ADD", vSubtask, zLEVEL_TASK );
      SetNameForView( vNew, "RLL_ENTITY_ADD", vSubtask, zLEVEL_SUBTASK );
   }

   GetViewByName( &vProfileXfer, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   zwTZEREMDD_DisableCtrlForRLL( vSubtask, vTZEREMDO );
   zwTZEREMDD_RelLinkSetFKPrefix( vSubtask, vTZEREMDO, vProfileXfer );

   // Relationship List is a multi selection list, delesect all Relationships.
   CreateViewFromViewForTask( &vCopy, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_RelLink", "" ) )
   {
      nRC =SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 0 );
   }

   SetCursorFirstEntityByAttr( vTZEREMDO, "ER_RelLink", "ZKey", vCopy, "ER_RelLink", "ZKey", "" );
   DropView( vCopy );
   SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 1 );

   zwTZEREMDD_RefreshRllWnd( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_PostBuildIDMAINT
// PURPOSE:   Prebuild of Attributes & Relationships for Identifier selection
// TRIGGERED BY:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildIDMAINT( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vCopy;
   zSHORT nRC;

   zwTZEREMDD_PreBuildIDATTADD( vSubtask );
   zwTZEREMDD_PreBuildIDRLLADD( vSubtask );
   zwTZEREMDD_PostBuildID( vSubtask );

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Identifier List is a multi selection list, deselect all Identifiers
   CreateViewFromViewForTask( &vCopy, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_EntIdentifier", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_EntIdentifier", "" ) )
   {
      nRC = SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 0 );
   }

   SetCursorFirstEntityByAttr( vTZEREMDO, "ER_EntIdentifier", "ZKey", vCopy, "ER_EntIdentifier", "ZKey", "" );
   DropView( vCopy );
   SetSelectStateOfEntity( vTZEREMDO, "ER_EntIdentifier", 1 );

   zwTZEREMDD_RefreshIdentifierWnd( vSubtask );

   return( 0 );
}

#if( 1 )
/////////////////////////////////////////////////////////////////////////////
//
// NOT USED ANYMORE?????
//
// OPERATION: zwTZEREMDD_OpenModel
// PURPOSE:   Activate selected ER Model and empty SA if ER is being switched from SA Dialog.
// TRIGGERED BY:
//    WINDOW: TZEROPMD
//    ACTION: BUTTON
//    NAME: Open
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_OpenModel( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   // Get Access to current Enterprise Model Object and prompt for Save
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( zwTZEREMDD_AskForSave( vSubtask ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }
   // Re - Activate ER Object
   oTZEREMDO_GetUpdViewForER( vSubtask, &vTZEREMDO, zFORCE_RELOAD );
   SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Empty SA Object if Activated - (Switch ER in SA Dialog)
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      nRC=CompareAttributeToAttribute( vTZEREMDO, "EntpER_Model", "ZKey", vTZERSASO, "EntpER_Model", "ZKey" );
      if ( nRC )
      {
         nRC=DropMetaOI( vSubtask, vTZERSASO );
         nRC = ActivateEmptyMetaOI( vSubtask, &vTZERSASO, zSOURCE_SA_META, zSINGLE | zLEVEL_APPLICATION );
         if ( nRC < 0 )
         {
            strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Activate empty Subject Area." );
            MessageSend( vSubtask, "ER00164", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( 0 );
         }

         nRC = CreateMetaEntity( vSubtask, vTZERSASO, "SubjectArea", zPOS_AFTER );
         nRC = SetAttributeFromString( vTZERSASO, "SubjectArea", "Name", "New" );

         nRC = IncludeSubobjectFromSubobject( vTZERSASO, "EntpER_Model", vTZEREMDO, "EntpER_Model", zPOS_AFTER );

         nRC = SetNameForView( vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
      }
   }

   return( 0 );

}
#endif

#if( 1 )
/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PreBuildSAVIEW
// PURPOSE:    This Entry names a view for the Subject Area
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PreBuildSAVIEW( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZEREMDD;
   zVIEW vTZERSASO;
   zVIEW vSAVIEW;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Data Model. Model must be opened." );
      MessageSend( vSubtask, "ER00165", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // Get Access to SA Object
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. Subject Area must be opened." );
      MessageSend( vSubtask, "ER00166", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // Get Access to ER Main Dialog
   nRC = GetViewByName( &vTZEREMDD, "TZEREMDD", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access ER Dialog." );
      MessageSend( vSubtask, "ER00168", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   // See if Modeless Window already active
   nRC = GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      SetWindowActionBehavior( vSAVIEW, zWAB_ReturnToParent | zWAB_ProcessImmediateAction, 0, 0 );
   }

   SetNameForView( vSubtask, "SAVIEW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildSAVIEW
// PURPOSE:    This Entry sets the caption for the Subject Area
//             modeless subwindow in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildSAVIEW( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zSHORT nRC;
   zCHAR szSAName[ 33 ];

   // Get Access to SA Object
   nRC = GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Unable to Access Subject Area. Subject Area must be opened." );
      MessageSend( vSubtask, "ER00169", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   GetStringFromAttribute( szSAName, zsizeof( szSAName ), vTZERSASO, "SubjectArea", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szSAName );

   return( 0 );
}
#endif

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_OK( zVIEW vSubtask )
{
   zVIEW  vADW_Work;
   zVIEW  vADW_Work2;
   zVIEW  vERM;
   zVIEW  vERD_Window;
   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ 512 ];
   zCHAR  szDirectory[ 512 ];
   zCHAR  szPrefix[ 512 ];
   zLONG  hFile;
   zSHORT nRC;

   GetViewByName( &vADW_Work, "TZERADWO", vSubtask, zLEVEL_TASK );

   // Retrieve base for file names.
   GetStringFromAttribute( szDirectory, zsizeof( szDirectory ), vADW_Work, "ADW_Work", "Directory" );
   GetStringFromAttribute( szPrefix, zsizeof( szPrefix ), vADW_Work, "ADW_Work", "ExportPrefix" );

   if ( zstrcmp( szDirectory, "" ) == 0 )
   {
      MessageSend( vSubtask, "ER00192", "ADW Import", "Directory Name is required.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edDirectory" );
      return( -1 );
   }

   // Make sure the Text and Integer Domains exist.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type", zREFER_DOMAIN_META, 0 );
   if ( SetCursorFirstEntityByString( vTaskLPLR, "W_MetaDef", "Name", "Text", 0 ) < zCURSOR_SET ||
        SetCursorFirstEntityByString( vTaskLPLR, "W_MetaDef", "Name", "Integer", 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00188", "ER Data Model Maintenance", "Text and Integer Domains do not exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Look to see if a work file has been saved using the Directory and Prefix entered.  If so, activate it and copy
   // over the InfoTypeDef entries.  Import may want to use the InfoType information saved in the work object.
   strcpy_s( szFileName, zsizeof( szFileName ), szDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), ".ADW" );
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile >= 0 )
   {
      SysCloseFile( vSubtask, hFile, 0 );
      nRC = ActivateOI_FromFile( &vADW_Work2, "TZERADWO", vADW_Work, szFileName, zSINGLE );
      if ( nRC >= 0 )
      {
         nRC = SetCursorFirstEntity( vADW_Work2, "InfoTypeDef", "" );
         while ( nRC >= zCURSOR_SET )
         {
            CreateEntity( vADW_Work, "InfoTypeDef", zPOS_LAST );
            SetMatchingAttributesByName( vADW_Work, "InfoTypeDef", vADW_Work2, "InfoTypeDef", zSET_ALL );
            nRC = SetCursorNextEntity( vADW_Work2, "InfoTypeDef", "" );
         }

         DropObjectInstance( vADW_Work2 );
      }
   }

   if ( zwTZEREMDD_ADW_BuildWorkOI( vSubtask, vADW_Work ) < 0 )
      return( 0 );

   // Get Data Model Window.
   GetViewByName( &vERD_Window, "TZEREMDD", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vERM, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyMetaOI( vSubtask, &vERM, zSOURCE_ERD_META, zSINGLE | zLEVEL_APPLICATION );
      // Build Root Instance
      CreateMetaEntity( vSubtask, vERM, "EntpER_Model", zPOS_AFTER );
      SetAttributeFromAttribute( vERM, "EntpER_Model", "Name", vTaskLPLR, "LPLR", "Name" );
      SetAttributeFromAttribute( vERM, "EntpER_Model", "Desc", vTaskLPLR, "LPLR", "Desc" );
      SetNameForView( vERM, "TZEREMDO", vSubtask, zLEVEL_TASK );
      // Set window Title with check out state
      SetTitleWithCheckOutState( vERD_Window, "Data Model", "TZEREMDO", vERM, "EntpER_Model", zSOURCE_ERD_META );
      SetCtrlState( vERD_Window, "ERD", zCONTROL_STATUS_ENABLED, TRUE );
   }

   oTZEREMDO_ImportADW_Model( vSubtask, vERM, vADW_Work );

   // Now make sure the stuff we have just added shows up.
   RefreshDiagram( vERD_Window, "ERD", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_Init( zVIEW vSubtask )
{
   zVIEW vADW_Work;

   ActivateEmptyObjectInstance( &vADW_Work, "TZERADWO", vSubtask, zSINGLE );
   CreateEntity( vADW_Work, "ADW_Work", zPOS_AFTER );
   SetNameForView( vADW_Work, "TZERADWO", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_BuildWorkOI( zVIEW vSubtask, zVIEW vADW_Work )
{
   zCHAR  szFileName[ 512 ];
   zCHAR  szDirectory[ 512 ];
   zCHAR  szPrefix[ 512 ];
   zPCHAR lpszLine;
   zPCHAR lpsz1;
   zPCHAR lpsz2;
   zLONG  hFile;
   zLONG  lKey1;
   zLONG  lType;

   // Retrieve base for file names.
   GetStringFromAttribute( szDirectory, zsizeof( szDirectory ), vADW_Work, "ADW_Work", "Directory" );
   GetStringFromAttribute( szPrefix, zsizeof( szPrefix ), vADW_Work, "ADW_Work", "ExportPrefix" );

   // Process OI file.
   strcpy_s( szFileName, zsizeof( szFileName ), szDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), "OI.EXP" );
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile == -1 )
   {
      TraceLineS( "ADW Import File Name: ", szFileName );
      MessageSend( vSubtask, "ER00175", "ADW Import", "OI Export file not found.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   else
   {
      while ( SysReadLine( vSubtask, (zCOREMEM) &lpszLine, hFile ) == 1 )
      {
         // Retrieve key1.
         lpsz1 = lpszLine + 2;            // Skip first 2 chars.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lKey1 = zatol( lpsz1 );          // Convert Key1 to integer

         // Retrieve type.
         lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lType = zatol (lpsz1);           // Convert Type to integer

         // Create only selected entries for later performance.
         if ( lType == 10007 || lType == 10094 )
         {
            CreateEntity( vADW_Work, "OI", zPOS_LAST );
            SetAttributeFromInteger( vADW_Work, "OI", "Key1", lKey1 );
            SetAttributeFromInteger( vADW_Work, "OI", "Type", lType );

            // Retrieve text
            lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
            lpsz1++;                         // Skip double quote.
            lpsz2 = &lpsz1[ zstrlen( lpsz1 ) ];  // Find end of string.
            while ( lpsz2 > lpsz1 && *lpsz2 != '\"' ) // Find last quote.
               lpsz2--;

            lpsz2--;                         // Skip quote.
            while( lpsz2 > lpsz1 && *lpsz2 == ' ' )  // Skip trailing spaces.
               lpsz2--;

            lpsz2++;                         // lpsz2 points after last char.
            *lpsz2 = 0;                      // Null-terminate.
            SetAttributeFromString( vADW_Work, "OI", "Text", lpsz1 );
         }

      } // while ( SysReadLine... )...

      SysCloseFile( vSubtask, hFile, 0 );

   } // if ( lpszFileName && *lpszFileName )...

   // Process AI file.
   strcpy_s( szFileName, zsizeof( szFileName ), szDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), "AI.EXP" );
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile == -1 )

   {
      TraceLineS( "ADW Import File Name: ", szFileName );
      MessageSend( vSubtask, "ER00176", "ADW Import", "AI Export file not found.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   {
      while ( SysReadLine( vSubtask, (zCOREMEM) &lpszLine, hFile ) == 1 )
      {
         // Retrieve key1.
         lpsz1 = lpszLine + 2;            // Skip first 2 chars.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lKey1 = zatol (lpsz1);           // Convert Key1 to integer

         // Retrieve type.
         lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lType = zatol (lpsz1);           // Convert Type to integer

         // Create only selected entries for later performance.
         if ( lType == 20007 || lType == 20450 || lType == 20044 )
         {
            CreateEntity( vADW_Work, "AI", zPOS_LAST );
            SetAttributeFromInteger( vADW_Work, "AI", "Key1", lKey1 );
            SetAttributeFromInteger( vADW_Work, "AI", "Type", lType );


            // Retrieve key2
            lpsz1 = lpsz2 + 3;               // Set lpsz1 to point to next token.
            lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
            *lpsz2 = 0;                      // Change delimiter to null-term.
            SetAttributeFromString( vADW_Work, "AI", "Key2", lpsz1 );

            // Retrieve key2
            lpsz1 = lpsz2 + 3;               // Set lpsz1 to point to next token.
            SetAttributeFromString( vADW_Work, "AI", "Key3", lpsz1 );
         }
      } // while ( SysReadLine... )...

      SysCloseFile( vSubtask, hFile, 0 );

   } // if ( lpszFileName && *lpszFileName )...

   // Process TI file.
   strcpy_s( szFileName, zsizeof( szFileName ), szDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), "TI.EXP" );
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile == -1 )
   {
      TraceLineS( "ADW Import File Name: ", szFileName );
      MessageSend( vSubtask, "ER00177", "ADW Import", "TI Export file not found.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   {
      while ( SysReadLine( vSubtask, (zCOREMEM) &lpszLine, hFile ) == 1 )
      {
         // Retrieve key1.
         lpsz1 = lpszLine + 2;            // Skip first 2 chars.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lKey1 = zatol( lpsz1 );          // Convert Key1 to integer

         // Retrieve type.
         lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lType = zatol( lpsz1 );          // Convert Type to integer

         // Create only selected entries for later performance.
         if ( lType == 30076 )
         {
            CreateEntity( vADW_Work, "TI", zPOS_LAST );
            SetAttributeFromInteger( vADW_Work, "TI", "Key1", lKey1 );
            SetAttributeFromInteger( vADW_Work, "TI", "Type", lType );


            // Retrieve TextEntrySeqNo.
            lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
            lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
            *lpsz2 = 0;                      // Change delimiter to null-term.
            SetAttributeFromString( vADW_Work, "TI", "TextEntrySeqNo", lpsz1 );

            // Retrieve text
            lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
            lpsz1++;                         // Skip double quote.
            lpsz2 = &lpsz1[ zstrlen( lpsz1 ) ];  // Find end of string.
            while ( lpsz2 > lpsz1 && *lpsz2 != '\"' ) // Find last quote.
               lpsz2--;

            lpsz2--;                         // Skip quote.
            while( lpsz2 > lpsz1 && *lpsz2 == ' ' )  // Skip trailing spaces.
               lpsz2--;

            lpsz2++;                         // lpsz2 points after last char.
            *lpsz2 = 0;                      // Null-terminate.
            SetAttributeFromString( vADW_Work, "TI", "Text", lpsz1 );
         }

      } // while ( SysReadLine... )...

      SysCloseFile( vSubtask, hFile, 0 );

   } // if ( lpszFileName && *lpszFileName )...

   // Process PI file.
   strcpy_s( szFileName, zsizeof( szFileName ), szDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), "PI.EXP" );
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile == -1 )
   {
      TraceLineS( "ADW Import File Name: ", szFileName );
      MessageSend( vSubtask, "ER00178", "ADW Import", "PI Export file not found.", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   {
      while ( SysReadLine( vSubtask, (zCOREMEM) &lpszLine, hFile ) == 1 )
      {
         // Retrieve key1.
         lpsz1 = lpszLine + 2;            // Skip first 2 chars.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lKey1 = zatol( lpsz1 );          // Convert Key1 to integer

         // Retrieve type.
         lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
         lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.
         *lpsz2 = 0;                      // Change delimiter to null-term.
         lType = zatol (lpsz1);           // Convert Type to integer

         // Create only selected entries for later performance.
         if ( lType == 30011 || lType == 30034 || lType == 30035 || lType == 30036 || lType == 30037 || lType == 30038 || lType == 30039 )
         {
            CreateEntity( vADW_Work, "PI", zPOS_LAST );
            SetAttributeFromInteger( vADW_Work, "PI", "Key1", lKey1 );
            SetAttributeFromInteger( vADW_Work, "PI", "Type", lType );

            // Skip next token.
            lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
            lpsz2 = zstrchr( lpsz1, ',' );   // Find delimiter.

            // Retrieve text
            lpsz1 = lpsz2 + 1;               // Set lpsz1 to point to next token.
            lpsz1++;                         // Skip double quote.
            lpsz2 = &lpsz1[ zstrlen( lpsz1 ) ];  // Find end of string.
            while ( lpsz2 > lpsz1 && *lpsz2 != '\"' ) // Find last quote.
               lpsz2--;

            lpsz2--;                         // Skip quote.
            while( lpsz2 > lpsz1 && *lpsz2 == ' ' )  // Skip trailing spaces.
               lpsz2--;

            lpsz2++;                         // lpsz2 points after last char.
            *lpsz2 = 0;                      // Null-terminate.
            SetAttributeFromString( vADW_Work, "PI", "Text", lpsz1 );
         }

      } // while ( SysReadLine... )...

      SysCloseFile( vSubtask, hFile, 0 );

   } // if ( lpszFileName && *lpszFileName )...

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_InfoTypeInit( zVIEW vSubtask )
{
   zVIEW  vADW_Work;
   zVIEW  vCM_List;
   zCHAR  szFileName[ 512 ];
   zPCHAR lpszDirectory;
   zPCHAR lpszPrefix;
   zLONG  hFile;
   zSHORT nRC;

   GetViewByName( &vADW_Work, "TZERADWO", vSubtask, zLEVEL_TASK );

   // Retrieve base for file names.
   GetAddrForAttribute( &lpszDirectory, vADW_Work, "ADW_Work", "Directory" );
   GetAddrForAttribute( &lpszPrefix, vADW_Work, "ADW_Work", "ExportPrefix" );

   // Look to see if a work file has been saved using the Directory and Prefix entered.  If so, Activate it
   // and proceed.  If not, initialize the current work object with InfoTypes from the Export files.
   strcpy_s( szFileName, zsizeof( szFileName ), lpszDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), lpszPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), ".ADW" );
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile >= 0 )
   {
      SysCloseFile( vSubtask, hFile, 0 );
      nRC = ActivateOI_FromFile( &vADW_Work, "TZERADWO", vADW_Work, szFileName, zSINGLE );
      if ( nRC >= 0 )
         SetNameForView( vADW_Work, "TZERADWO", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // Execute the routine to build the work object and then loop through through all the InfoType
      // entries (OI.Type 10094), creating InfoType entities in the work object.
      nRC = zwTZEREMDD_ADW_BuildWorkOI( vSubtask, vADW_Work );

      // Don't continue if we couldn't find the OI export file.
      if ( nRC < 0 )
         return( 0 );

      nRC = SetCursorFirstEntityByInteger( vADW_Work, "OI", "Type", 10094, "" );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vADW_Work, "InfoTypeDef", zPOS_AFTER );
         SetAttributeFromString( vADW_Work, "InfoTypeDef", "DomainName", "Text" );
         SetAttributeFromAttribute( vADW_Work, "InfoTypeDef", "Mask", vADW_Work, "OI", "Text" );
         nRC = SetCursorNextEntityByInteger( vADW_Work, "OI", "Type", 10094, "" );
      }

      // Save the work area as a file in the same directory as the ADW export files.
      CommitOI_ToFile( vADW_Work, szFileName, zSINGLE );
   }

   // Create the View for listing Domains
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_InfoTypeOK( zVIEW vSubtask )
{
   zVIEW  vADW_Work;
   zCHAR  szFileName[ 512 ];
   zPCHAR lpszDirectory;
   zPCHAR lpszPrefix;

   // Commit the work object to a file using the ADW Directory and Prefix
   // information.

   GetViewByName( &vADW_Work, "TZERADWO", vSubtask, zLEVEL_TASK );

   // Retrieve base for file names.
   GetAddrForAttribute( &lpszDirectory, vADW_Work, "ADW_Work", "Directory" );
   GetAddrForAttribute( &lpszPrefix, vADW_Work, "ADW_Work", "ExportPrefix" );

   // Look to see if a work file has been saved using the Directory and Prefix entered.
   // If so, Activate it and proceed.  If not, return with an error.
   strcpy_s( szFileName, zsizeof( szFileName ), lpszDirectory );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), lpszPrefix );
   strcat_s( szFileName, zsizeof( szFileName ), ".ADW" );
   CommitOI_ToFile( vADW_Work, szFileName, zSINGLE );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ADW_SetDomain( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vADW_Work;

   // This processes the combo box selection by copying the meta name
   // for entry selected (CM_List) to the InforType entry.
   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vADW_Work, "TZERADWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromAttribute( vADW_Work, "InfoTypeDef", "DomainName", vCM_List, "W_MetaDef", "Name" );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_GrayIdentifierActions( zVIEW vSubtask )
{
   zVIEW vIdentifiers;

   // Disable the Add and Remove Actions as necessary.
   GetViewByName( &vIdentifiers, "TZEREMDO_RLL", vSubtask, zLEVEL_TASK );
   if ( vIdentifiers && SetCursorFirstEntity( vIdentifiers, "ER_AttributeIdentifier", "ER_EntIdentifier" ) >= zCURSOR_SET )
   {
      // There is an attribute identifier so enable the action.
      EnableAction( vSubtask, "RemoveAttributeFromIdentifier", TRUE );
   }
   else
   {
      // There is no attribute identifier so disable the action.
      EnableAction( vSubtask, "RemoveAttributeFromIdentifier", FALSE );
   }

   if ( vIdentifiers && SetCursorFirstEntity( vIdentifiers, "ER_RelLinkIdentifier", "ER_EntIdentifier" ) >= zCURSOR_SET )
   {
      // There is a relationship identifier so enable the action.
      EnableAction( vSubtask, "RemoveRelLinkFromIdentifier", TRUE );
   }
   else
   {
      // There is no relationship identifier so disable the action.
      EnableAction( vSubtask, "RemoveRelLinkFromIdentifier", FALSE );
   }

   return( 0 );
}

zSHORT LOCALOPER
fnCreateGenkeyLOD( zVIEW vSubtask, zVIEW vERD )
{
   // This routine generates a Genkey LOD.  It is called from the Dialog operation below, CreateGenkeyEntityLOD.

   zVIEW  vLOD;
   zVIEW  vLPLR;
   zVIEW  vERD2;
   zSHORT nRC;

   // Make sure the Genkey LOD doesn't already exist.
   nRC = ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META, zSINGLE | zLEVEL_APPLICATION, "GENKEYWO", zCURRENT_OI );
   if ( nRC >= zCURSOR_SET )
   {
      DropMetaOI( vSubtask, vLOD );
      MessageSend( vSubtask, "ER00184", "ER Data Model Maintenance", "Genkey LOD already exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // We are forcing an ERD REFER OI to be created, so that we can drop it later.
   // If we let CM create the REFER OI, which it will do on the relink for
   // the LOD we are building, it will leave it hanging in Zeidon Object
   // Services, which will not let the task do any more saves of the ERD.
   // By creating that REFER OI ourselves, we can force it to be dropped at
   // the end of the routine and additional ERD saves will be allowed.
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   ActivateMetaOI( vSubtask, &vERD2, vLPLR, zREFER_ERD_META, zSINGLE | zLEVEL_APPLICATION );

   // Start with an empty LOD, adding the LOD entity.
   ActivateEmptyMetaOI( vSubtask, &vLOD, zSOURCE_LOD_META, zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( vLOD, "GenkeyLOD", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vLOD, "LOD", zPOS_AFTER );
   SetAttributeFromString( vLOD, "LOD", "Name", "GENKEYWO" );
   SetAttributeFromString( vLOD, "LOD", "Desc", "Zeidon Genkey Object" );

   // Add the LOD_Entity and include the ER_Entity.
   CreateMetaEntity( vSubtask, vLOD, "LOD_EntityParent", zPOS_AFTER );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Name", "ZeidonGenkeyTable" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Create", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Delete", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Update", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "InclSrc", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Include", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Exclude", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Derived", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Work", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Recursive", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "ParentDeleteBehave", "D" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Desc", "Used for System Generated Keys" );
   IncludeSubobjectFromSubobject( vLOD, "ER_EntityRec", vERD, "ER_Entity", zPOS_AFTER );

   // Include the flat side of the ER_Entity in the LOD.
   IncludeSubobjectFromSubobject( vLOD, "LOD_Entity", vLOD, "LOD_EntityParent", zPOS_AFTER );

   // Add the two LOD_Attributes and include the ER_Attributes.
   SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );
   SetCursorNextEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );

   // Commit the LOD.
   CommitMetaOI( vSubtask, vLOD, zSOURCE_LOD_META );

   MessageSend( vSubtask, "ER00184", "ER Data Model Maintenance", "Note that LOD must be saved with TE after TE table entry is made.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );

   DropMetaOI( vSubtask, vLOD );
   DropMetaOI( vSubtask, vLPLR );

   // We are dropping the view (not DropMetaOI) because we need to get rid of the refer view in Zeidon Object Services for later saves to be allowed.
   DropObjectInstance( vERD2 );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
CreateGenkeyEntityLOD( zVIEW vSubtask )
{
   // This routine generates an Entity for generated keys, along with the associated LOD for generated keys.

   zVIEW  vERD;
   zVIEW  vDomainGrp;
   zLONG  lPosY;
   zLONG  lTempPosY;
   zSHORT nRC;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Make sure the operator knows the ERD will be saved.
   nRC = MessagePrompt( vSubtask, "ER00183", "ER Data Model Maintenance", "Data Model will be saved.\nOK to continue?", 0, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
   if ( nRC == zRESPONSE_NO )
      return( 0 );

   // Make sure a ZeidonGenkeyTable entity doesn't already exist.
   nRC = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", "ZeidonGenkeyTable", 0 );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00182", "ER Data Model Maintenance", "Genkey entity already exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // Determine the largest PosY value for positioning the Genkey entity.
   lPosY = 0;
   nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTempPosY, vERD, "ER_Entity", "ER_DiagramPosY" );
      if ( lTempPosY > lPosY )
         lPosY = lTempPosY;
      nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 );
   }

   // Position at end of ER_Entities.
   SetCursorLastEntity( vERD, "ER_Entity", 0 );

   // Create the Genkey entity, itself.
   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lPosY + 9 );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", 2 );
   SetAttributeFromString( vERD, "ER_Entity", "Name", "ZeidonGenkeyTable" );
   SetAttributeFromString( vERD, "ER_Entity", "Purpose", "F" );
   SetAttributeFromString( vERD, "ER_Entity", "Desc", "Used for System Generated Keys" );

   // Create the two attributes for the Genkey entity.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "CurrentGenkey" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "Integer", zCURRENT_OI );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );
   DropMetaOI( vSubtask, vDomainGrp );

   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", 32 );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "TableName" );
   SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "Text", zCURRENT_OI );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );
   DropMetaOI( vSubtask, vDomainGrp );

   // Create the Identifier entry.  We are already positioned on the correct ER_Attribute for the include.
   CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "Name", "TableName" );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "SystemMaintained", "N" );
   CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vERD, "ER_AttributeIdentifier", vERD, "ER_Attribute", zPOS_AFTER );

   // Sort Attributes in original sequence
   oTZEREMDO_SortAttributes( vERD );

   // Commit the ERD.
   CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META );

   // Go to also create the LOD.
   fnCreateGenkeyLOD( vSubtask, vERD );

   // Finally refresh the diagram to show the new entity.
   RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
}

zSHORT LOCALOPER
fnCreateLockingLOD( zVIEW vSubtask, zVIEW vERD )
{
   // This routine generates a Locking LOD.  It is called from the Dialog operation below, CreateLockingEntityLOD.

   zVIEW  vLOD;
   zVIEW  vLPLR;
   zVIEW  vERD2;
   zSHORT nRC;

   // Make sure the Locking LOD doesn't already exist.
   nRC = ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META, zSINGLE | zLEVEL_APPLICATION, "ZPLOCKO", zCURRENT_OI );
   if ( nRC >= zCURSOR_SET )
   {
      DropMetaOI( vSubtask, vLOD );
      MessageSend( vSubtask, "ER00184", "ER Data Model Maintenance", "Locking LOD already exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // We are forcing an ERD REFER OI to be created, so that we can drop it later.
   // If we let CM create the REFER OI, which it will do on the relink for
   // the LOD we are building, it will leave it hanging in Zeidon Object
   // Services, which will not let the task do any more saves of the ERD.
   // By creating that REFER OI ourselves, we can force it to be dropped at
   // the end of the routine and additional ERD saves will be allowed.
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   ActivateMetaOI( vSubtask, &vERD2, vLPLR, zREFER_ERD_META, zSINGLE | zLEVEL_APPLICATION );

   // Start with an empty LOD, adding the LOD entity.
   ActivateEmptyMetaOI( vSubtask, &vLOD, zSOURCE_LOD_META, zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( vLOD, "LockingLOD", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vLOD, "LOD", zPOS_AFTER );
   SetAttributeFromString( vLOD, "LOD", "Name", "ZPLOCKO" );
   SetAttributeFromString( vLOD, "LOD", "Desc", "Zeidon Locking Object" );

   // Add the LOD_Entity and include the ER_Entity.
   CreateMetaEntity( vSubtask, vLOD, "LOD_EntityParent", zPOS_AFTER );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Name", "ZeidonLock" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Create", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Delete", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Update", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "InclSrc", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Include", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Exclude", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Derived", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Work", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Recursive", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "ParentDeleteBehave", "D" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Desc", "Used for Automatic locking." );
   IncludeSubobjectFromSubobject( vLOD, "ER_EntityRec", vERD, "ER_Entity", zPOS_AFTER );

   // Include the flat side of the ER_Entity in the LOD.
   IncludeSubobjectFromSubobject( vLOD, "LOD_Entity", vLOD, "LOD_EntityParent", zPOS_AFTER );

   // Add the LOD_Attributes and include the ER_Attributes.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Attribute", 0 ) )
   {
      CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );
   }

   // Commit the LOD.
   CommitMetaOI( vSubtask, vLOD, zSOURCE_LOD_META );

   MessageSend( vSubtask, "ER00184", "ER Data Model Maintenance", "Note that LOD must be saved with TE after TE table entry is made.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );

   DropMetaOI( vSubtask, vLOD );
   DropMetaOI( vSubtask, vLPLR );

   // We are dropping the view (not DropMetaOI) because we need to get rid of the refer view in Zeidon Object Services for later saves to be allowed.
   DropObjectInstance( vERD2 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CreateLockingEntity( zVIEW vSubtask )
{
   // This routine generates an Entity for pessimistic locking, along with the associated LOD for pessimistic locking.

   zVIEW  vERD;
   zVIEW  vDomainGrp;
   zLONG  lPosY;
   zLONG  lTempPosY;
   zSHORT nRC;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Make sure the operator knows the ERD will be saved.
   nRC = MessagePrompt( vSubtask, "ER00183", "ER Data Model Maintenance", "Data Model will be saved.\nOK to continue?", 0, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
   if ( nRC == zRESPONSE_NO )
      return( 0 );

   // Make sure a ZeidonLockingTable entity doesn't already exist.
   nRC = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", "ZeidonLocking", 0 );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00182", "ER Data Model Maintenance", "Locking entity already exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // Determine the largest PosY value for positioning the Locking entity.
   lPosY = 0;
   nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTempPosY, vERD, "ER_Entity", "ER_DiagramPosY" );
      if ( lTempPosY > lPosY )
         lPosY = lTempPosY;
      nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 );
   }

   // Position at end of ER_Entities.
   SetCursorLastEntity( vERD, "ER_Entity", 0 );

   // Create the Locking entity, itself.
   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lPosY + 9 );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", 2 );
   SetAttributeFromString( vERD, "ER_Entity", "Name", "ZeidonLocking" );
   SetAttributeFromString( vERD, "ER_Entity", "Purpose", "F" );
   SetAttributeFromString( vERD, "ER_Entity", "Desc", "Used for Automatic database locking" );

   //
   // Create the attributes for the Locking entity.
   //

   // Load the Text domain.
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "Text", zCURRENT_OI );

   // LOD name.  We make the length of the attribute 35 instead of the usual 32 because we need the ability to prepend a few characters.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "LOD_Name" );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", 35 );
   SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   // Create the Identifier entry.  We are already positioned on the correct ER_Attribute for the include.
   CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "Name", "LockingKey" );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "SystemMaintained", "N" );

   CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vERD, "ER_AttributeIdentifier", vERD, "ER_Attribute", zPOS_AFTER );

   // Key value.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "KeyValue" );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", 200 );
   SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vERD, "ER_AttributeIdentifier", vERD, "ER_Attribute", zPOS_AFTER );

   // User name.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "UserName" );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", 32 );
   SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   // AllowRead.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "AllowRead" );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", 1 );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   DropMetaOI( vSubtask, vDomainGrp );

   // Load the DateTime domain.
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "DateTime", zCURRENT_OI );

   // Timestamp of lock.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "Timestamp" );
   SetAttributeFromString( vERD, "ER_Attribute", "NotNull", "Y" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   DropMetaOI( vSubtask, vDomainGrp );

   // Load the Integer domain.
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "Integer", zCURRENT_OI );

   // ID for future use.
   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "ID" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   DropMetaOI( vSubtask, vDomainGrp );

   // Sort Attributes in original sequence
   oTZEREMDO_SortAttributes( vERD );

   // Commit the ERD.
   CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META );

   // Go to also create the LOD.
   fnCreateLockingLOD( vSubtask, vERD );

   // Finally refresh the diagram to show the new entity.
   RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CreateKeyListEntity( zVIEW vSubtask )
{
   // This routine generates an Entity used for storing keys.

   zVIEW  vERD;
   zVIEW  vDomainGrp;
   zLONG  lPosY;
   zLONG  lTempPosY;
   zSHORT nRC;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Make sure the operator knows the ERD will be saved.
   nRC = MessagePrompt( vSubtask, "ER00183", "ER Data Model Maintenance", "Data Model will be saved.\nOK to continue?", 0, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
   if ( nRC == zRESPONSE_NO )
      return( 0 );

   // Make sure a ZeidonLockingTable entity doesn't already exist.
   nRC = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", "ZeidonKeyList", 0 );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00182", "ER Data Model Maintenance", "KeyList entity already exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // Determine the largest PosY value for positioning the entity.
   lPosY = 0;
   nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTempPosY, vERD, "ER_Entity", "ER_DiagramPosY" );
      if ( lTempPosY > lPosY )
         lPosY = lTempPosY;
      nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 );
   }

   // Position at end of ER_Entities.
   SetCursorLastEntity( vERD, "ER_Entity", 0 );

   // Create the Locking entity, itself.
   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lPosY + 9 );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", 2 );
   SetAttributeFromString( vERD, "ER_Entity", "Name", "ZeidonKeyList" );
   SetAttributeFromString( vERD, "ER_Entity", "Purpose", "F" );
   SetAttributeFromString( vERD, "ER_Entity", "Desc", "Used for when selecting and entity by key list" );

   //
   // Create the attributes for the Locking entity.
   //

   // Load the Text domain.
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "Text", zCURRENT_OI );

   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "StringValue" );
   SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", 200 );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   // Load the DateTime domain.
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "DateTime", zCURRENT_OI );

   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "TimestampValue" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   DropMetaOI( vSubtask, vDomainGrp );

   // Load the Integer domain.
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, "Integer", zCURRENT_OI );

   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "IntValue" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", "TaskID" );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );

   DropMetaOI( vSubtask, vDomainGrp );

   // Sort Attributes in original sequence
   oTZEREMDO_SortAttributes( vERD );

   // Commit the ERD.
   CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META );

   // Finally refresh the diagram to show the new entity.
   RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
fnCreateDomainLOD( zVIEW vSubtask, zVIEW vERD )
{
   // This routine generates a Dynamic Domain LOD, "DOMAINT".  It is called from the Dialog operation below, CreateDynamicDomain.

   zVIEW  vLOD;
   zVIEW  vLOD_Parent;
   zVIEW  vLPLR;
   zVIEW  vERD2;
   zSHORT nRC;
   zLONG  lZKey;

   // Make sure the DOMAINT LOD doesn't already exist.
   nRC = ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META, zSINGLE | zLEVEL_APPLICATION, "DOMAINT", zCURRENT_OI );
   if ( nRC >= zCURSOR_SET )
   {
      DropMetaOI( vSubtask, vLOD );
      MessageSend( vSubtask, "ER00186", "ER Data Model Maintenance", "DOMAINT LOD already exists.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // We are forcing an ERD REFER OI to be created, so that we can drop it later.
   // If we let CM create the REFER OI, which it will do on the relink for
   // the LOD we are building, it will leave it hanging in Zeidon Object
   // Services, which will not let the task do any more saves of the ERD.
   // By creating that REFER OI ourselves, we can force it to be dropped at
   // the end of the routine and additional ERD saves will be allowed.
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   ActivateMetaOI( vSubtask, &vERD2, vLPLR, zREFER_ERD_META, zSINGLE | zLEVEL_APPLICATION );

   // Start with an empty LOD, adding the LOD entity.
   ActivateEmptyMetaOI( vSubtask, &vLOD, zSOURCE_LOD_META, zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( vLOD, "DOMAINT_LOD", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vLOD, "LOD", zPOS_AFTER );
   SetAttributeFromString( vLOD, "LOD", "Name", "DOMAINT" );
   SetAttributeFromString( vLOD, "LOD", "Desc", "Zeidon Dynamic Table Domain Object" );

   // Add the Domain LOD_Entity and include the ER_Entity.
   CreateMetaEntity( vSubtask, vLOD, "LOD_EntityParent", zPOS_AFTER );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Name", "Domain" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "IndentName", "Domain" );
   SetAttributeFromInteger( vLOD, "LOD_EntityParent", "IndentLvl",    0 );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Create", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Delete", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Update", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "InclSrc", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Include", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Exclude", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Derived", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Work", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Recursive", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "ParentDeleteBehave", "D" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Desc", "One instance exists for each Dynamic Domain Table." );
   IncludeSubobjectFromSubobject( vLOD, "ER_EntityRec", vERD, "ER_Entity", zPOS_AFTER );

   // Include the flat side of the ER_Entity in the LOD.
   IncludeSubobjectFromSubobject( vLOD, "LOD_Entity", vLOD, "LOD_EntityParent", zPOS_AFTER );

   // Add the two Domain LOD_Attributes and include the ER_Attributes.
   SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );
   SetCursorNextEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );

   // Add the DomainValue LOD_Entity and include the ER_Entity and ER_RelLinkRec.
   // We will use a new LOD view to keep position at top as we step down one subobject level.
   // Also set the vERD ER_Entity forward once to DomainValue entity.
   SetCursorNextEntity( vERD, "ER_Entity", 0 );
   CreateViewFromViewForTask( &vLOD_Parent, vLOD, 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_EntityChild", zPOS_AFTER );
   SetViewToSubobject( vLOD, "LOD_EntityChild" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Name", "DomainValue" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "IndentName", "   DomainValue" );
   SetAttributeFromInteger( vLOD, "LOD_EntityParent", "IndentLvl", 3 );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "RelLinkIndicator", "L" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Create", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Delete", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Update", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "InclSrc", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Include", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Exclude", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Derived", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Work", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Recursive", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "ParentDeleteBehave", "D" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Desc", "One instance exists for each Dynamic Domain Table." );
   IncludeSubobjectFromSubobject( vLOD, "ER_EntityRec", vERD, "ER_Entity", zPOS_AFTER );
   GetIntegerFromAttribute( &lZKey, vERD, "ER_RelLink_Other", "ZKey" );
   SetCursorFirstEntityByInteger( vERD, "ER_RelLink_2", "ZKey", lZKey, "EntpER_Model" );
   IncludeSubobjectFromSubobject( vLOD, "ER_RelLinkRec", vERD, "ER_RelLink_2", zPOS_AFTER );

   // Include the flat side of the ER_Entity in the LOD.
   IncludeSubobjectFromSubobject( vLOD_Parent, "LOD_Entity", vLOD, "LOD_EntityParent", zPOS_AFTER );

   // Add the three DomainValue LOD_Attributes and include the ER_Attributes.
   SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );
   SetCursorNextEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );
   SetCursorNextEntity( vERD, "ER_Attribute", 0 );
   CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );

   // Reset the LOD view back up.
   ResetViewFromSubobject( vLOD );

   // Commit the LOD.
   CommitMetaOI( vSubtask, vLOD, zSOURCE_LOD_META );

   MessageSend( vSubtask, "ER00184", "ER Data Model Maintenance", "Note that LOD must be saved with TE after TE table entry is made.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );

   DropMetaOI( vSubtask, vLOD );
   DropMetaOI( vSubtask, vLPLR );

   // We are dropping the view (not DropMetaOI) because we need to get rid of
   // the refer view in Zeidon Object Services for later saves to be allowed.
   DropObjectInstance( vERD2 );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
CreateERD_Attribute( zVIEW  vSubtask, zVIEW  vERD, zPCHAR szAttributeName, zPCHAR szDomainName, zLONG  lLth, zPCHAR szNullInd )
{
   zVIEW  vDomainGrp;

   // This routine creates an Attribute entity.

   CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_Attribute", "Name", szAttributeName );
   // set new Attribute Sequence
   oTZEREMDO_NewAttributeSequence( vERD );
   ActivateMetaOI_ByName( vSubtask, &vDomainGrp, 0, zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION, szDomainName, zCURRENT_OI );
   if ( lLth != 0 )
      SetAttributeFromInteger( vERD, "ER_Attribute", "Lth", lLth );
   if ( szNullInd != 0 && szNullInd[ 0 ] != 0 )
      SetAttributeFromString( vERD, "ER_Attribute", "NotNull", szNullInd );
   IncludeSubobjectFromSubobject( vERD, "Domain", vDomainGrp, "Domain", zPOS_AFTER );
   DropMetaOI( vSubtask, vDomainGrp );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
CreateDynamicDomain( zVIEW vSubtask )
{
   // This routine generates two related entities for dynamic Table
   // Domains, along with the associated LOD.

   zVIEW  vERD;
   zVIEW  vERD2;
   zLONG  lPosY;
   zLONG  lTempPosY;
   zSHORT nRC;
   zSHORT nRC2;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Make sure the operator knows the ERD will be saved.
   nRC = MessagePrompt( vSubtask, "ER00183", "ER Data Model Maintenance", "Data Model will be saved.\nOK to continue?", 0, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
   if ( nRC == zRESPONSE_NO )
      return( 0 );

   // Make sure the two entities don't already exist.
   nRC  = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", "Domain", 0 );
   nRC2 = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", "DomainValue", 0 );
   if ( nRC >= zCURSOR_SET || nRC2 >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "ER00182", "ER Data Model Maintenance", "Dynamic Domain entities already exist.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      return( 0 );
   }

   // Determine the largest PosY value for positioning the Genkey entity.
   lPosY = 0;
   nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTempPosY, vERD, "ER_Entity", "ER_DiagramPosY" );
      if ( lTempPosY > lPosY )
         lPosY = lTempPosY;
      nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 );
   }

   // Position at end of ER_Entities.
   SetCursorLastEntity( vERD, "ER_Entity", 0 );

   // Create the Domain entity.
   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lPosY + 9 );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", 2 );
   SetAttributeFromString( vERD, "ER_Entity", "Name", "Domain" );
   SetAttributeFromString( vERD, "ER_Entity", "Purpose", "F" );
   SetAttributeFromString( vERD, "ER_Entity", "Desc", "Domain table for dynamic Table Domains" );

   // Create the two attributes for the Domain entity.
   CreateERD_Attribute( vSubtask, vERD, "Name", "AlphaNumeric", 32, "Y" );
   CreateERD_Attribute( vSubtask, vERD, "Desc", "Text", 0, 0 );
   SetCursorFirstEntity( vERD, "ER_Attribute", 0 );

   // Create the Identifier entry.  We are already positioned on the correct
   // ER_Attribute for the include.
   CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "Name", "Name" );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "SystemMaintained", "N" );
   CreateMetaEntity( vSubtask, vERD, "ER_FactType", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vERD, "ER_AttributeIdentifier", vERD, "ER_Attribute", zPOS_AFTER );

   // Save the position on the Domain entity.
   CreateViewFromViewForTask( &vERD2, vERD, 0 );

   // Create the DomainValue entity.
   CreateMetaEntity( vSubtask, vERD, "ER_Entity", zPOS_AFTER );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosY", lPosY + 18 );
   SetAttributeFromInteger( vERD, "ER_Entity", "ER_DiagramPosX", 2 );
   SetAttributeFromString( vERD, "ER_Entity", "Name", "DomainValue" );
   SetAttributeFromString( vERD, "ER_Entity", "Purpose", "A" );
   SetAttributeFromString( vERD, "ER_Entity", "Desc", "Individual table values" );

   // Create the three attributes for the DomainValue entity.
   CreateERD_Attribute( vSubtask, vERD, "InternalIntegerValue", "Integer", 0, 0 );
   CreateERD_Attribute( vSubtask, vERD, "InternalStringValue", "Text", 64, 0 );
   CreateERD_Attribute( vSubtask, vERD, "ExternalDescription", "Text", 64, 0 );

   // Create the Identifier entry.  It is simply a system maintained
   // Identifier.
   // ER_Attribute for the include.
   CreateMetaEntity( vSubtask, vERD, "ER_EntIdentifier", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "Name", "DomainValueID" );
   SetAttributeFromString( vERD, "ER_EntIdentifier", "SystemMaintained", "Y" );

   // Create the Relationship between the two entities.
   CreateMetaEntity( vSubtask, vERD, "ER_RelType", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_RelType", "Fact", "Added automatically." );
   CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_RelLink_2", "Name", "has" );
   SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "1" );
   SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "m" );
   IncludeSubobjectFromSubobject( vERD, "ER_Entity_2", vERD2, "ER_Entity", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vERD, "ER_RelLink_2", zPOS_AFTER );
   SetAttributeFromString( vERD, "ER_RelLink_2", "Name", "for" );
   SetAttributeFromString( vERD, "ER_RelLink_2", "CardMin", "1" );
   SetAttributeFromString( vERD, "ER_RelLink_2", "CardMax", "1" );
   IncludeSubobjectFromSubobject( vERD, "ER_Entity_2", vERD, "ER_Entity", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vERD2, "ER_RelLink_Other", vERD, "ER_RelLink_2", zPOS_AFTER );
   SetCursorPrevEntity( vERD, "ER_RelLink_2", 0 );
   IncludeSubobjectFromSubobject( vERD, "ER_RelLink_Other", vERD, "ER_RelLink_2", zPOS_AFTER );

   // Sort Attributes in original sequence
   oTZEREMDO_SortAttributes( vERD );

   // Commit the ERD.
   CommitMetaOI( vSubtask, vERD, zSOURCE_ERD_META );

   // Go to also create the LOD.  Pass vERD2, since it is positioned on the Domain entity.
   fnCreateDomainLOD( vSubtask, vERD2 );

   // Finally refresh the diagram to show the new entity.
   RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DomainAttributeList( zVIEW vSubtask )
{
   // This routine creates a list of all Domains and what attributes use
   // them.  It creates the list in the browse Domain object, but will
   // later create an HD_Diagram object for displaying the information
   // as a hierarchical diagram.

   zVIEW  vERD;
   zVIEW  vTZEREMDO;
   zVIEW  vDomainList;
   zSHORT nRC;
   zCHAR  szDomainName[ 33 ];
   zCHAR  szAttributeName[ 33 ];

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   if ( GetViewByName( &vDomainList, "TZBRDMAO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // If the view already exists, delete the entries.
      nRC = SetCursorFirstEntity( vDomainList, "Domain", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         DeleteEntity( vDomainList, "Domain", zREPOS_NONE );
         nRC = SetCursorNextEntity( vDomainList, "Domain", 0 );
      }
   }
   else
   {
      // Otherwise create the object instance empty.
      ActivateEmptyObjectInstance( &vDomainList, "TZBRDMAO", vSubtask, zMULTIPLE );
      SetNameForView( vDomainList, "TZBRDMAO", vSubtask, zLEVEL_TASK );
      CreateEntity( vDomainList, "Root", zPOS_AFTER );
   }

   // Loop through all the Domains and create entries in the DomainList.
   nRC = SetCursorFirstEntity( vERD, "Domain", "EntpER_Model" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szDomainName, zsizeof( szDomainName ), vERD, "Domain", "Name" );
      GetStringFromAttribute( szAttributeName, zsizeof( szAttributeName ), vERD, "ER_Attribute", "Name" );
      nRC = SetCursorFirstEntityByString( vDomainList, "Domain", "Name", szDomainName, 0 );
      if ( nRC < zCURSOR_SET )
      {
         CreateEntity( vDomainList, "Domain", zPOS_AFTER );
         SetAttributeFromString( vDomainList, "Domain", "Name", szDomainName );
         SetAttributeFromAttribute( vDomainList, "Domain", "Desc", vERD, "Domain", "Desc" );
      }

      IncludeSubobjectFromSubobject( vDomainList, "ER_Attribute", vERD, "ER_Attribute", zPOS_AFTER );

      nRC = SetCursorNextEntity( vERD, "Domain", "EntpER_Model" );
   }

   // Finish by order the Domains by Name.
   OrderEntityForView( vDomainList, "Domain", "Name A" );

   SetCursorFirstEntity( vDomainList, "Domain", 0 );
   DropView( vERD );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttrList( zVIEW vSubtask )
{
   // This routine refreshes the listbox of Attributes underneath the Domain selected in the other litbox.

   RefreshCtrl( vSubtask, "AttributeList" );
   RefreshCtrl( vSubtask, "edDescription" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CreateDomainAttrGrph( zVIEW vSubtask )
{
   // This routine creates a hierarchical diagram for a Domain and the Attributes which use it.
   // It also brings up the Graph window if it isn't already up, or refreshes it if it is up.

   zVIEW  vDiagram;
   zVIEW  vDomainList;
   zVIEW  vGraphWindow;
   zSHORT nRC;

   GetViewByName( &vDomainList, "TZBRDMAO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDiagram, "TZZOHDGO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // If the view already exists, delete the entries.
      nRC = SetCursorFirstEntity( vDiagram, "HD_Node", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         DeleteEntity( vDiagram, "HD_Node", zREPOS_NONE );
         nRC = SetCursorNextEntity( vDiagram, "HD_Node", 0 );
      }
   }
   else
   {
      // Otherwise create the object instance with the root entity and one
      // HD_Node entity for the selected Domain.
      ActivateEmptyObjectInstance( &vDiagram, "TZZOHDGO", vSubtask, zMULTIPLE );
      CreateEntity( vDiagram, "HD_Diagram", zPOS_AFTER );
      SetNameForView( vDiagram, "TZZOHDGO", vSubtask, zLEVEL_TASK );
   }

   // Create the top node for the Domain
   CreateEntity( vDiagram, "HD_Node", zPOS_AFTER );
   SetAttributeFromAttribute( vDiagram, "HD_Node", "Name", vDomainList, "Domain", "Name" );
   SetAttributeFromAttribute( vDiagram, "HD_Node", "Text", vDomainList, "Domain", "Name" );
   SetAttributeFromString( vDiagram, "HD_Node", "DIL", "Domain" );

   // Loop through all the ER_Attributes underneath the Domain at which the cursor is positioned and create HD_ChildNode entries.
   SetViewToSubobject( vDiagram, "HD_ChildNode" );
   nRC = SetCursorFirstEntity( vDomainList, "ER_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vDiagram, "HD_Node", zPOS_AFTER );
      SetAttributeFromAttribute( vDiagram, "HD_Node", "Name", vDomainList, "ER_Attribute", "Name" );
      SetAttributeFromAttribute( vDiagram, "HD_Node", "Text", vDomainList, "ER_Attribute", "Name" );
      SetAttributeFromString( vDiagram, "HD_Node", "DIL", "Attribute" );
      CreateEntity( vDiagram, "HD_ChildNode", zPOS_AFTER );
      SetAttributeFromAttribute( vDiagram, "HD_ChildNode", "Name", vDomainList, "ER_Entity", "Name" );
      SetAttributeFromAttribute( vDiagram, "HD_ChildNode", "Text", vDomainList, "ER_Entity", "Name" );
      SetAttributeFromString( vDiagram, "HD_ChildNode", "DIL", "Entity" );

      nRC = SetCursorNextEntity( vDomainList, "ER_Attribute", 0 );
   }
   ResetViewFromSubobject( vDiagram );

   // Determine if Graph window is already up.  If so, refresh it.  If not,
   // transfer to it.
   nRC = GetViewByName( &vGraphWindow, "DomainAttributeGraphWindow", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      RefreshCtrl( vGraphWindow, "DomainAttributeGraph" );
   else
      SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow, "TZERATRD", "DomainAttributeGraph" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_NameGraphWindowView( zVIEW vSubtask )
{
   // This routine names the view of the Domain/Attribute Graph window so that the parent window can refresh it.

   SetNameForView( vSubtask, "DomainAttributeGraphWindow", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CloseSA( zVIEW vSubtask )
{
   // Ask to save the SA and then drop the TZERSASO view.
   zVIEW   vSA;

   if ( zwTZEREMDD_SA_AskForSave( vSubtask ) < 0 )
      return( -1 );

   if ( GetViewByName( &vSA, "TZERSASO", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vSA );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetDIL_Message( zVIEW vSubtask )
{
   zCHAR szReturnCtrlTag[ 33 ];
   zCHAR szWorkMessage[ 300 ];
   zCHAR szParentEntityName[ 33 ];
   zCHAR szChildEntityName[ 33 ];
   zVIEW  vERD;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );


   GetCtrlWithFocus( vSubtask, szReturnCtrlTag );

   if ( zstrcmp( szReturnCtrlTag, "AutoSeq" ) == 0 )
   {
      GetStringFromAttribute( szChildEntityName, zsizeof( szChildEntityName ), vERD, "ER_Entity", "Name" );
      GetStringFromAttribute( szParentEntityName, zsizeof( szParentEntityName ), vERD, "ER_Entity_Other", "Name" );
   }
   else
      if ( zstrcmp( szReturnCtrlTag, "AutoSeq_Other" ) == 0 )

      {
         GetStringFromAttribute( szParentEntityName, zsizeof( szParentEntityName ), vERD, "ER_Entity", "Name" );
         GetStringFromAttribute( szChildEntityName, zsizeof( szChildEntityName ), vERD, "ER_Entity_Other", "Name" );
      }

   if ( zstrcmp( szReturnCtrlTag, "AutoSeq" ) == 0 || zstrcmp( szReturnCtrlTag, "AutoSeq_Other" ) == 0 )
   {
      strcpy_s( szWorkMessage, "Maintain the last order (positions) of '" );
      strcat_s( szWorkMessage, szParentEntityName );
      strcat_s( szWorkMessage, "' under '" );
      strcat_s( szWorkMessage, szChildEntityName );
      strcat_s( szWorkMessage, "'." );

      MB_SetMessage( vSubtask, 1, szWorkMessage );
   }


   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PrintERD_Report( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vLPLR;
   zCHAR  szTimestamp[ 22 ];

   // This routine prints a report on the current ERD in memory.

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Set up current DateTime for report.
   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( vLPLR, "LPLR", "CurrentDateTime", szTimestamp );

   FormatSubobjectOnDoc( vERD, "EntpER_Model", vSubtask, "ERDRPT1", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PrintSAReport( zVIEW vSubtask )
{

   zVIEW  vSA;
   zVIEW  vLPLR;
   zCHAR  szTimestamp[ 22 ];

   // This routine prints a report on the current Subject Area in memory.

   GetViewByName( &vSA, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Set up current DateTime for report.
   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( vLPLR, "LPLR", "CurrentDateTime", szTimestamp );

   FormatSubobjectOnDoc( vSA, "SubjectArea", vSubtask, "ERSARPT", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PrintSelectedDomains( zVIEW vSubtask )
{
   zVIEW  vDomainList;
   zVIEW  vSelectedList;
   zVIEW  vLPLR;
   zCHAR  szTimestamp[ 22 ];
   zSHORT nRC;

   // This routine prints a report on the selected domain in object
   // TZBRDMAO.

   GetViewByName( &vDomainList, "TZBRDMAO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Set up current DateTime for report.
   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( vLPLR, "LPLR", "CurrentDateTime", szTimestamp );

   // Set up a work object with only selected Domain entries.
   ActivateEmptyObjectInstance( &vSelectedList, "TZBRDMAO", vSubtask, zMULTIPLE );
   SetNameForView( vSelectedList, "***SelectedList", vSubtask, zLEVEL_TASK );
   CreateEntity( vSelectedList, "Root", zPOS_AFTER );
   // Loop through all selected Domains and create entries in the SelectedList.
   for ( nRC = SetCursorFirstSelectedEntity( vDomainList, "Domain", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDomainList, "Domain", "" ) )
   {
      CreateEntity( vSelectedList, "Domain", zPOS_AFTER );

      SetMatchingAttributesByName( vSelectedList, "Domain", vDomainList, "Domain", zSET_NULL );

      IncludeSubobjectFromSubobject( vSelectedList, "ER_Attribute", vDomainList, "ER_Attribute", zPOS_AFTER );
   }

   if ( SetCursorFirstEntity( vSelectedList, "Domain", "" ) >= zCURSOR_SET )
   {
      FormatSubobjectOnDoc( vSelectedList, "Domain", vSubtask, "DOMRPT", 0 );
   }

//   DropView( vSelectedList );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PrintERD_PartialReport( zVIEW vSubtask )
{

   zVIEW  vERD;
   zVIEW  vLPLR;
   zCHAR  szTimestamp[ 22 ];

   // This routine prints a report on the current ERD in memory.

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Set up current DateTime for report.
   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   SetAttributeFromString( vLPLR, "LPLR", "CurrentDateTime", szTimestamp );

   FormatSubobjectOnDoc( vERD, "EntpER_Model", vSubtask, "ERDPART", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RS_LodListRefreshDesc
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RS_LodListRefreshDesc( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "edLodDesc" );

   return( 0 );
} // zwTZEREMDD_RS_LodListRefreshDesc

zOPER_EXPORT zSHORT OPERATION
RefreshLOD_ListSubwindow( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "LodList" );
   RefreshCtrl( vSubtask, "edLodDesc" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_EntityLOD_Refresh
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_EntityLOD_Refresh( zVIEW vSubtask )
{

   RefreshCtrl( vSubtask, "AttributeList" );
   RefreshCtrl( vSubtask, "edDescription" );

    return( 0 );
} // zwTZEREMDD_EntityLOD_Refresh

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetCursorAttr( zVIEW vSubtask )
{

   zVIEW  vERD;
   zVIEW  vDomainList;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetViewByName( &vDomainList, "TZBRDMAO", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstEntityByAttr( vERD, "ER_Attribute", "ZKey", vDomainList, "ER_Attribute", "ZKey", "EntpER_Model" ) >= zCURSOR_SET )
      CreateTemporalSubobjectVersion( vERD, "ER_Attribute" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CopyAttributes( zVIEW vSubtask )
{
   // This operation copies all the selected attributes (actually includes them) to the derived relationship ER_AttributeCopy.

   zVIEW  vERD;
   zSHORT nRC;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // First remove any existing derived entries.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_AttributeCopy", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_AttributeCopy", 0 ) )
   {
      ExcludeEntity( vERD, "ER_AttributeCopy", zREPOS_NONE );

   }

   // Now loop through all the selected entries and copy them.
   for ( nRC = SetCursorFirstSelectedEntity( vERD, "ER_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vERD, "ER_Attribute", 0 ) )
   {
      IncludeSubobjectFromSubobject( vERD, "ER_AttributeCopy", vERD, "ER_Attribute", zPOS_AFTER );
      SetSelectStateOfEntity( vERD, "ER_Attribute", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PasteAttributes( zVIEW vSubtask )
{
   // This operation copies all the selected attributes (actually includes them) to the derived relationship ER_AttributeCopy.

   zVIEW  vERD;
   zSHORT nRC;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Loop through all ER_AttributeCopy entities and copy them to ER_Attribute entities, include the correct Domain.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_AttributeCopy", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_AttributeCopy", 0 ) )
   {
      CreateMetaEntity( vSubtask, vERD, "ER_Attribute", zPOS_AFTER );
      SetMatchingAttributesByName( vERD, "ER_Attribute", vERD, "ER_AttributeCopy", zSET_NULL );
      // set new Attribute Sequence
      oTZEREMDO_NewAttributeSequence( vERD );
      IncludeSubobjectFromSubobject( vERD, "Domain", vERD, "DomainCopy", zPOS_AFTER );
   }

   zwTZEREMDD_RefreshAttWndForSort( vSubtask );
   zwTZEREMDD_RefEntListForSort( vSubtask );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_MenuItemCloseSA
//
// close Subject Area about menu item
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_MenuItemCloseSA( zVIEW vSubtask )
{
   zSHORT nRC;

   nRC = zwTZEREMDD_CloseSA( vSubtask);

   //close window Subject Area
   if ( nRC == 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceDialogWithDialog, "TZEREMDD", "TZEREMDD" );

   return( 0 );
} // zwTZEREMDD_MenuItemCloseSA


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ShowLineAndText
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ShowLineAndText( zVIEW vSubtask )
{
   zVIEW vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ShowRelOption", 3 );

   zwTZEREMDD_ProfileSave( vSubtask );

   return( 0 );
} // zwTZEREMDD_ShowLineAndText


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ShowLineOnly
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ShowLineOnly( zVIEW vSubtask )
{
   zVIEW vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ShowRelOption", 1 );

   zwTZEREMDD_ProfileSave( vSubtask );

   return( 0 );
} // zwTZEREMDD_ShowLineOnly


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_HideLine
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_HideLine( zVIEW vSubtask )
{
   zVIEW vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   SetAttributeFromInteger( vProfileXFER, "EMD", "ERD_ShowRelOption", 0 );

   zwTZEREMDD_ProfileSave( vSubtask );

   return( 0 );
} // zwTZEREMDD_HideLine

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshEntityList
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntityList( zVIEW vSubtask )
{
   zVIEW vWindow;

   //save selected Attribute for Auto Drag Sort
   zwTZEREMDD_SaveSelectedAttribute( vSubtask );

   //if single click in window AttributeList, then refresh the listbox
   //in window EntityList
   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" );
   if ( vWindow )
      RefreshCtrl( vWindow, "AttrList" );

   return( 0 );
} // zwTZEREMDD_RefreshEntityList

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RLLRefreshEntityList
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RLLRefreshEntityList( zVIEW vSubtask )
{
   zVIEW vWindow;

   //if single click in window RelationshipList, then refresh the listbox
   //in window EntityList
   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" );
   if ( vWindow )
      RefreshCtrl( vWindow, "RelList" );

   //select the Relationship in ER_Diagram and unselect the other Relationships
   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   return( 0 );
} // zwTZEREMDD_RLLRefreshEntityList


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshAttributeWnd
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttributeWnd( zVIEW vSubtask )
{
   zVIEW vW;
   zVIEW vTZEREMDO;
   zCHAR szEntityName[ 33 ];

   //save selected Attribute for Auto Drag Sort
   zwTZEREMDD_SaveSelectedAttribute( vSubtask );

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" ) >= 0 )
   {
      RefreshWindow( vW );
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
      SetWindowCaptionTitle( vW, 0, szEntityName );
   }

   return( 0 );
} // zwTZEREMDD_RefreshAttributeWnd


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshRelatshipWnd
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshRelatshipWnd( zVIEW vSubtask )
{
   zVIEW vW;
   zVIEW vTZEREMDO;
   zCHAR szEntityName[ 33 ];

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "RLLVIEW" ) >= 0 )
   {
      RefreshWindow( vW );
      GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTZEREMDO, "ER_Entity", "Name" );
      SetWindowCaptionTitle( vW, 0, szEntityName );
   }

   //select the Relationship in ER_Diagram and unselect the other Relationships
   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   return( 0 );
} // zwTZEREMDD_RefreshRelatshipWnd


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetRelationshipSelect
//
// select the currrent Relationship and unselect the other Relationships
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetRelationshipSelect( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   //unselect the other Relationships
   zwTZEREMDD_DeselectRelationship( vTZEREMDO, "ER_RelType", 0 );

   //select the Relationship in ER_Diagram
   nRC = SetCursorFirstEntityByAttr( vTZEREMDO, "ER_RelType", "ZKey", vTZEREMDO, "ER_RelType_1", "ZKey", 0 );
   if ( nRC >= zCURSOR_SET )
      nRC = SetSelectStateOfEntity( vTZEREMDO, "ER_RelType", 1 );

   nRC = GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   nRC = RefreshDiagram( vWindow, "ERD", 0 );

   // Get Access to Subject Area
   GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" );
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 && vWindow )
   {
      //unselect the other Relationships
      zwTZEREMDD_DeselectRelationship( vTZERSASO, "SA_RelType", 0 );
      nRC = SetCursorFirstEntityByAttr( vTZERSASO, "ER_RelType", "ZKey", vTZEREMDO, "ER_RelType", "ZKey", "SubjectArea" );
      if ( nRC >= zCURSOR_SET )
         SetSelectStateOfEntity( vTZERSASO, "SA_RelType", 1 );

      RefreshDiagram( vWindow, "ERD", 0 );

      zwTZEREMDD_DeselectRelationship( vTZERSASO, "ER_RelLink", 0 );
      zwTZEREMDD_SetSelectionInSAList( vTZEREMDO, vTZERSASO );
      RefreshCtrl( vWindow, "SA_RelLink" );
   }

   return( 0 );
} // zwTZEREMDD_SetRelationshipSelect


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetRSSelection
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SetRSSelection( zVIEW vSubtask )
{
   zVIEW  vWindow;

   zwTZEREMDD_SetRelationshipSelect( vSubtask );

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "RLLVIEW" ) >= 0 )
      RefreshCtrl( vWindow, "ER_RelLink" );

   return( 0 );
} // zwTZEREMDD_SetRSSelection

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SelectAllEntities
//
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SelectAllEntities( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      nRC = SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_Entity", 0 );
   }

   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", 0 );

   // Refresh Subject Area and set selection
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 1 );

   return( 0 );
}// zwTZEREMDD_SelectAllEntities


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DelAllRelFromSelEnt
//
// delete all Relationships from selected Entities
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DelAllRelFromSelEnt( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all Relationships from selected Entities", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", "" ) )
      {
         // delete Relationships
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink" ) >= zCURSOR_SET )
         {
            // Loop and Delete all ER RelLinks
            for ( nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" ) )
            {
               oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NONE, 0 );
            }
         }
      }
      nRC = zwTZEREMDD_RefreshRllWnd( vSubtask );
   }

   return( 0 );
} // zwTZEREMDD_DelAllRelFromSelEnt

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DelAllAttFromSelEnt
//
// delete all Attributes from selected Entities
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DelAllAttFromSelEnt( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all Attributes from selected Entities", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {

      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", "" ) )
      {
         // delete Attributes
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) >= zCURSOR_SET )
         {
            // Loop and Delete all ER Attributes
            nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Attribute", "" );
            while ( nRC >= zCURSOR_SET )
            {
               nRC = DeleteEntity( vTZEREMDO, "ER_Attribute", zREPOS_NONE );
               nRC = SetCursorNextEntity( vTZEREMDO, "ER_Attribute", "" );
            }
         }
      }
      zwTZEREMDD_RefreshAttWnd( vSubtask );
   }

   return( 0 );
} // zwTZEREMDD_DelAllAttFromSelEnt




////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DelAllIdFromSelEnt
//
// delete all Identifier from selected Entities
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DelAllIdFromSelEnt( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   nRC = MessagePrompt( vSubtask, "", "ER Data Model Maintenance", "OK to delete all Identifiers from selected Enities", 0, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", "" ) )
      {
         // delete Identifiers
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_EntIdentifier" ) >= zCURSOR_SET )
         {
            // Loop and Delete all ER EntIdentifiers
            nRC = SetCursorFirstEntity( vTZEREMDO, "ER_EntIdentifier", "" );
            while ( nRC >= zCURSOR_SET )
            {
               nRC = DeleteEntity( vTZEREMDO, "ER_EntIdentifier", zREPOS_NONE );
               nRC = SetCursorNextEntity( vTZEREMDO, "ER_EntIdentifier", "" );
            }
         }
      }
      nRC = zwTZEREMDD_RefreshIdWnd( vSubtask );
   }

   return( 0 );
} // zwTZEREMDD_DelAllIdFromSelEnt

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SA_SetNewName
//
// if Single click in ListBox, then set Name in Editfeld
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_SetNewName( zVIEW vSubtask )
{
   zVIEW  vTZERSFLO;
   zVIEW  vTZERSASO;
   zCHAR  szSAName[33];
   zCHAR  szDescription[32000];

   GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szSAName, zsizeof( szSAName ), vTZERSFLO, "W_MetaDef", "Name" );
   SetAttributeFromString( vTZERSASO, "SubjectArea", "SaveAsName", szSAName );

   // set Description
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED ) != 1 )
   {
      GetStringFromAttribute( szDescription, zsizeof( szDescription ), vTZERSFLO, "W_MetaDef", "Desc" );
      SetCtrlText( vSubtask, "edDesc", szDescription );
      RefreshCtrl( vSubtask, "edDesc" );
   }

   RefreshCtrl( vSubtask, "edSAName" );
   SetFocusToCtrl( vSubtask, "edSAName" );

   return( 0 );
} // zwTZEREMDD_SA_SetNewName

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SA_LoadListForSaveAs
//
// load all SubjectAreas and set Cursor
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_LoadListForSaveAs( zVIEW vSubtask )
{
   zVIEW  vTZERSFLO;
   zVIEW  vTZERSASO;
   zCHAR  szDescription[32000];

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   // Retrieve and name Subject Area File List Object
   RetrieveViewForMetaList( vSubtask, &vTZERSFLO, zREFER_SA_META );
   SetNameForView( vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );

   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );
   SetAttributeFromString( vTZERSASO, "SubjectArea", "SaveAsName", "" );

   if ( CheckExistenceOfEntity(vTZERSFLO, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZERSFLO, "W_MetaDef", 0 );

   //set current Description
   GetStringFromAttribute( szDescription, zsizeof( szDescription ), vTZERSASO, "SubjectArea", "Desc" );
   SetCtrlText( vSubtask, "edDesc", szDescription );

   return( 0 );
} // zwTZEREMDD_SA_LoadListForSaveAs

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SA_SaveAs
//
// save Subject Area with a other Name
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_SaveAs( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vTZERSFLO;
   zVIEW  vTZERSASO_New;
   zVIEW  vSAView;
   zCHAR  szNewName[33];
   zCHAR  szOutName[33];
   zCHAR  szDescription[32000];

   // Validate Subject Area Name is OK
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSAView, "SAVIEW", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "edDesc", szDescription, 32000 );

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vTZERSASO, "SubjectArea", "SaveAsName" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   //Name is required
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Subject Area Name is required." );
      MessageSend( vSubtask, "ER00137", "ER Data Model Maintenance", szWorkMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edNewName" );
      return( -2 );
   }

   //Name already exists
   if ( SetCursorFirstEntityByString( vTZERSFLO, "W_MetaDef", "Name", szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkMessage, zsizeof( szWorkMessage ), "Subject Area already exists. Replace existing Subject Area?" );
      nRC = MessagePrompt( vSubtask, "ER00138", "ER Data Model Maintenance", szWorkMessage, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edNewName" );
         return( -1 );
      }
      else
      {
         //if this Subject Area open, then save Subject Area, not save as
         if ( CompareAttributeToString( vTZERSASO, "SubjectArea", "Name", szOutName ) == 0 )
         {
            SetAttributeFromString( vTZERSASO, "SubjectArea", "Name", szOutName );
            SetAttributeFromString( vTZERSASO, "SubjectArea", "Desc", szDescription );
            zwTZEREMDD_SaveSA( vSubtask );
            SetWindowCaptionTitle( vSAView, 0, szOutName );
            return( 0 );
         }
         else
            //if an other Subject Area open, then replace existing Subject Area
            nRC = DeleteMetaOI( vSubtask, vTZERSFLO, zREFER_SA_META );
      }
   }

   // Load new Subject Area Data.
   MapCtrl( vSAView, "ERD" );
   oTZERSASO_SA_MigrateWithERDs( &vTZERSASO_New, vTZERSASO, vTZEREMDO, vTZERSASO, vSubtask );

   SetAttributeFromString( vTZERSASO_New, "SubjectArea", "Name", szOutName );
   SetAttributeFromString( vTZERSASO_New, "SubjectArea", "Desc", szDescription );

   DropMetaOI( vSubtask, vTZERSASO );
   SetNameForView( vTZERSASO_New, "TZERSASO", vSubtask, zLEVEL_TASK );
   vTZERSASO = vTZERSASO_New;

   zwTZEREMDD_SaveSA( vSubtask );

   GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( vTZERSFLO, "W_MetaDef", "CPLR_ZKey", vTZERSASO, "SubjectArea", "ZKey", "" );

   SetWindowCaptionTitle( vSAView, 0, szOutName );

   return( 0 );
} // zwTZEREMDD_SA_SaveAs


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_MoveAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_MoveAttribute( zVIEW vSubtask )
{
   zVIEW    vTZEREMDO;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META ))
   {
      oTZEREMDO_MoveAttribInSequence( vTZEREMDO );
   }

   zwTZEREMDD_RefreshAttWndForSort( vSubtask );
   zwTZEREMDD_RefEntListForSort( vSubtask );

   return( 0 );
} // zwTZEREMDD_MoveAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SaveSelectedAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SaveSelectedAttribute( zVIEW vSubtask )
{
   zVIEW    vTZEREMDO;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META ))
   {
      SetAttributeFromAttribute( vTZEREMDO, "ER_Entity", "wkSelectedAttributeForDragSort", vTZEREMDO, "ER_Attribute", "wkSequence" );
   }

   return( 0 );
} // zwTZEREMDD_SaveSelectedAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshRSWndForSort
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshRSWndForSort( zVIEW vSubtask )
{
   zVIEW  vWindow;

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "RLLVIEW" ) >= 0 )
      RefreshCtrl( vWindow, "ER_RelLink" );

   return( 0 );
} // zwTZEREMDD_RefreshRSWndForSort

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefEntityListForSort
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefEntityListForSort( zVIEW vSubtask )
{
   zVIEW vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" );
   if ( vWindow )
      RefreshCtrl( vWindow, "RelList" );

   return( 0 );
} // zwTZEREMDD_RefEntityListForSort


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshAttWndForSort
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshAttWndForSort( zVIEW vSubtask )
{
   zVIEW vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ATTVIEW" );
   if ( vWindow )
      RefreshCtrl( vWindow, "ER_Attribute" );

   return( 0 );
} // zwTZEREMDD_RefreshAttWndForSort


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefEntListForSort
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefEntListForSort( zVIEW vSubtask )
{
   zVIEW vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" );
   if ( vWindow )
      RefreshCtrl( vWindow, "AttrList" );

   return( 0 );
} // zwTZEREMDD_RefEntListForSort


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_Check_CheckoutStatus
//
//    set check out state in window title
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_Check_CheckoutStatus( zVIEW vSubtask )
{
   zVIEW   vTZERSFLO;
   zSHORT  nEnable = 0;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vTZERSFLO, "TZERSFLO", vSubtask, zLEVEL_TASK ) >= 0 && CheckExistenceOfEntity( vTZERSFLO, "W_MetaDef" ) >= zCURSOR_SET )
   {
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vTZERSFLO );
      if ( nEnable )
         bEnable = TRUE;
   }

   SetCtrlState( vSubtask, "New", zOPTION_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Delete", zOPTION_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "DeleteSA", bEnable );

   return( 0 );
} // zwTZEREMDD_Check_CheckoutStatus


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DisableDeleteActions
//
//    if switch LPLR and the new ERD not checked out, then disable the Delete Action
//    for Attribute List, Relationship List, Identifier List and Entity List
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DisableDeleteActions( zVIEW  vSubtask, zSHORT nEnable )
{
   zVIEW  vW;

   // Attribute List
   if ( GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" ) >= 0 )
   {
      EnableAction( vW, "DeleteAttr", nEnable );
   }

   // Relationship List
   if ( GetSubtaskForWindowName( vSubtask, &vW, "RLLVIEW" ) >= 0 )
   {
      EnableAction( vW, "DeleteRelLink", nEnable );
   }

   // Identifier List
   if ( GetSubtaskForWindowName( vSubtask, &vW, "IDVIEW" ) >= 0 )
   {
      EnableAction( vW, "DeleteIdentifier", nEnable );
   }

   return( 0 );
} // zwTZEREMDD_DisableDeleteActions

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ADW_Set_Dir_Prefix
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_ADW_Set_Dir_Prefix( zVIEW vSubtask, zPCHAR szFileName, zLONG  Suffix )
{
   zCHAR   szDirectory[512];
   zCHAR   szPrefix[512];
   zCHAR   szExtention[ 3 ];
   zLONG   lStringLen = 0;
   zLONG   lIndex     = 0;
   zLONG   Backslash  = 0;

   szDirectory[ 0 ] = 0;
   szPrefix[ 0 ]  = 0;
   szExtention[ 0 ] = 0;

   lStringLen = zstrlen( szFileName );

   lIndex = lStringLen;
   while ( lIndex > 0 && Backslash == 0 )
   {
      if ( szFileName[ lIndex ] == '\\' )
         Backslash = 1;
      else
         lIndex--;
   }

   if ( Backslash )
   {
      strncpy_s( szDirectory, zsizeof( szDirectory ), szFileName, lIndex );
      szDirectory[ lIndex ] = 0;
      strncpy_s( szPrefix, zsizeof( szPrefix ), szFileName + 1 + lIndex, lStringLen - 4 - 1 - Suffix - lIndex );
      szPrefix[ lStringLen - 4 - 1 - Suffix - lIndex ] = 0;
      strncpy_s( szExtention, zsizeof( szExtention ), szFileName + lStringLen - 3, 3 );
      szExtention[ 3 ] = 0;
   }

   SetCtrlText( vSubtask, "edDirectory", szDirectory );
   SetCtrlText( vSubtask, "edPrefix", szPrefix );
   SetCtrlText( vSubtask, "edExtention", szExtention );

   return( 0 );
} // zwTZEREMDD_ADW_Set_Dir_Prefix


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ADW_Browse
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ADW_Browse( zVIEW vSubtask )
{
   zCHAR   szFileName[512];

   szFileName[ 0 ] = 0;
   if ( OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ), "Import Files (*.EXP)|*.exp|", "EXP", zOFN_HIDEREADONLY | zOFN_FILEMUSTEXIST ) == 1 )

   {
      zwTZEREMDD_ADW_Set_Dir_Prefix( vSubtask, szFileName, 2 );
   }

   return( 0 );
} // zwTZEREMDD_ADW_Browse


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ADW_Exp_Browse
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ADW_Exp_Browse( zVIEW vSubtask )
{
   zCHAR   szFileName[512];
   zLONG   lSuffix = 0;
   zLONG   lFileHandle = 0;

   szFileName[ 0 ] = 0;
   if ( OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ), "Export Files (*.EXP)|*.exp|", "EXP", zOFN_CREATEPROMPT | zOFN_SAVEAS ) == 1 )

   {
      // File exists, do not show the suffix (AI, OI, TI or PI).
      lFileHandle = SysOpenFile( vSubtask, szFileName, COREFILE_EXIST );
      if ( lFileHandle == 1 )
        lSuffix = 2;

      zwTZEREMDD_ADW_Set_Dir_Prefix( vSubtask, szFileName, lSuffix );
   }

   return( 0 );
} // zwTZEREMDD_ADW_Exp_Browse

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ADW_Exp_OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ADW_Exp_OK( zVIEW vSubtask )
{
   /*zVIEW  vTZERADWE;
   zVIEW  vTZEREMDO;
   zCHAR  szDirectory[512];
   zSHORT nRC;
   GetViewByName( &vTZERADWE, "TZERADWE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szDirectory, zsizeof( szDirectory ), vTZERADWE, "ADW_Work_Export", "Directory" );
   if ( zstrcmp( szDirectory, "" ) == 0 )
   {
      MessageSend( vSubtask, "ER00193", "ADW Export",
                   "Directory Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edDirectory" );
      return( -1 );
   }
   // Sort Attributes in original sequence
   oTZEREMDO_SortAttributes( vTZEREMDO );
   zwTZEREMDD_RefreshAttWndForSort( vSubtask );
   zwTZEREMDD_RefEntListForSort( vSubtask );
   nRC = oTZEREMDO_ADW_Export( vSubtask, vTZERADWE, vTZEREMDO );
   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( nRC );
   }
   DropObjectInstance( vTZERADWE );*/

   return( 0 );
} // zwTZEREMDD_ADW_Exp_OK


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ADW_Exp_PreBuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ADW_Exp_PreBuild( zVIEW vSubtask )
{
   zVIEW  vTZERADWE;

   ActivateEmptyObjectInstance( &vTZERADWE, "TZERADWE", vSubtask, zSINGLE );
   SetNameForView( vTZERADWE, "TZERADWE", vSubtask, zLEVEL_TASK );

   CreateEntity( vTZERADWE, "ADW_Work_Export", zPOS_AFTER );

   SetAttributeFromString( vTZERADWE, "ADW_Work_Export", "Extention", "exp" );

   return( 0 );
} // zwTZEREMDD_ADW_Exp_PreBuild


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ADW_Exp_Cancel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ADW_Exp_Cancel( zVIEW vSubtask )
{
   zVIEW vTZERADWE;

   GetViewByName( &vTZERADWE, "TZERADWE", vSubtask, zLEVEL_TASK );

   DropObjectInstance( vTZERADWE );

   return( 0 );
} // zwTZEREMDD_ADW_Exp_Cancel


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SaveAsKeepCurrentDesc
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zCHAR  szDescription[32000];

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      GetStringFromAttribute( szDescription, zsizeof( szDescription ), vTZERSASO, "SubjectArea", "Desc" );
      SetCtrlText( vSubtask, "edDesc", szDescription );
      RefreshCtrl( vSubtask, "edDesc" );
   }

   return( 0 );
} // zwTZEREMDD_SaveAsKeepCurrentDesc


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_LoadEntityList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LoadEntityList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZEREMDD_LoadEntityList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_LoadRelationshipList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LoadRelationshipList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "RLLVIEW" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZEREMDD_LoadRelationshipList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_LoadAttributeList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LoadAttributeList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ATTVIEW" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZEREMDD_LoadAttributeList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_LoadIdentifierList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LoadIdentifierList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "IDVIEW" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZEREMDD_LoadIdentifierList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ZoomDiagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ZoomDiagram( zVIEW vSubtask )
{
   zSHORT  nZoom;

   nZoom = (zSHORT) GetWindowPreferenceInteger( vSubtask, "Zoom" );

   RefreshDiagram( vSubtask, "ERD", 0 );

   if ( nZoom == 100 )
      ZoomDiagram( vSubtask, "ERD", 100, 1 );  // Zoom to Read
   else
      ZoomDiagram( vSubtask, "ERD", 0, 3 );    // Zoom to Fit

   return( 0 );
} // zwTZEREMDD_ZoomDiagram

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_LodDialogExpandAll
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LodDialogExpandAll( zVIEW vSubtask )
{
   OL_SetCtrlState( vSubtask, "LodMapping", zCONTROL_STATUS_EXPANDALL, TRUE );

   return( 0 );
} // zwTZEREMDD_LodDialogExpandAll


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshWindowCtrlList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshWindowCtrlList( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW  vView;
   zVIEW  vXfer;
   zBOOL  bInivible    = FALSE;
   zBOOL  bInivibleLOD = FALSE;

   SetCtrlText( vSubtask, "txtNoInfos", "No Info's" );

   if ( GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK ) < zCURSOR_SET )
   {
      SetCtrlText( vSubtask, "txtTitle", " No Mapping" );
   }
   else
   {
       szEntityName[ 0 ] = 0;

      if ( OL_GetCurrentEntityName( vSubtask, "LodMapping", &vView, szEntityName ) >= 0 )
      {
         if ( zstrcmp( szEntityName, "LOD" ) == 0 )
         {
            SetCtrlText( vSubtask, "txtTitle", " LOD Mapping" );
            bInivibleLOD = TRUE;
         }
         else
         if ( zstrcmp( szEntityName, "ViewObjRef" ) == 0 )
         {
            SetCtrlText( vSubtask, "txtTitle", " Registered View Mapping" );
            if ( vXfer && CheckExistenceOfEntity( vXfer, "Dialog" ) < zCURSOR_SET )
              SetCtrlText( vSubtask, "txtNoInfos", "No Mapping" );
         }
         else
         if ( zstrcmp( szEntityName, "Dialog" ) == 0 )
         {
            SetCtrlText( vSubtask, "txtTitle", " Dialog Mapping" );
            bInivible = TRUE;
          }
         else
         {
            SetCtrlText( vSubtask, "txtTitle", " No Mapping" );
         }
      }
      else
      {
         SetCtrlText( vSubtask, "txtTitle", " No Mapping" );
      }
   }

   SetCtrlState( vSubtask, "gbWindowControl", zCONTROL_STATUS_VISIBLE, bInivible );
   SetCtrlState( vSubtask, "gbLodDesc", zCONTROL_STATUS_VISIBLE, bInivibleLOD );
// SetCtrlState( vSubtask, "edLodDesc", zCONTROL_STATUS_VISIBLE, bInivibleLOD );

   if ( bInivibleLOD )
   {
      SetCtrlState( vSubtask, "gbNoInfos", zCONTROL_STATUS_VISIBLE, FALSE );
   // SetCtrlState( vSubtask, "txtNoInfos", zCONTROL_STATUS_VISIBLE, FALSE );
   }
   else
   {
      SetCtrlState( vSubtask, "gbNoInfos", zCONTROL_STATUS_VISIBLE, !bInivible );
   // SetCtrlState( vSubtask, "txtNoInfos", zCONTROL_STATUS_VISIBLE, !bInivible );
   }

   RefreshWindowExceptForCtrl( vSubtask, "LodMapping" );
/*
   RefreshCtrl( vSubtask, "txtTitle" );
   RefreshCtrl( vSubtask, "gbWindowControl" );
   RefreshCtrl( vSubtask, "lbWindowControl" );
   RefreshCtrl( vSubtask, "gbNoInfos" );
   RefreshCtrl( vSubtask, "gbLodDesc" );
*/
   return( 0 );
} // zwTZEREMDD_RefreshWindowCtrlList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshIdentifierWnd
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshIdentifierWnd( zVIEW vSubtask )
{
   zVIEW vW;

   zwTZEREMDD_RefreshIdWnd( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTMAINT" ) >= 0 )
      RefreshCtrl( vW, "lbIdentifier" );

   return( 0 );
} // zwTZEREMDD_RefreshIdentifierWnd


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_AcceptSubobject
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_AcceptSubobject( zVIEW  vSubtask, zPCHAR szEntityName, zPCHAR szMessageText, zPCHAR szTE_Attribute, zPCHAR szTE_MessText )
{
   zVIEW vTZEREMDO;
   zCHAR szName[ 33 ];
   zCHAR szOutName[ 33 ];
   zCHAR szMsg[ 100 ];

   // Check that the attribute name is unique.
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szName, zsizeof( szName ), vTZEREMDO, szEntityName, "Name" );
   if ( CheckDuplicateEntityByString( vTZEREMDO, szEntityName, "Name", szName, 0 ) < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Duplicate " );
      strcat_s( szMsg, zsizeof( szMsg ), szMessageText );
      strcat_s( szMsg, zsizeof( szMsg ), " Name for this Entity." );
      MessageSend( vSubtask, "ER00180", "ER Data Model Maintenance", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SetFocusToCtrl( vSubtask, "Name" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Check TE Column Name for Attribute / TE Index Name
   GetStringFromAttribute( szName, zsizeof( szName ), vTZEREMDO, szEntityName, szTE_Attribute );
   UfCompressName( szName, szOutName, 32, "", "", "", "", 0 );

   if ( zstrcmp( szOutName, "" ) == 0 )
   {
     if (ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META ))
         SetAttributeFromString( vTZEREMDO, szEntityName, szTE_Attribute, szOutName );
   }
   else
   {
      if ( CheckDuplicateEntityByString( vTZEREMDO, szEntityName, szTE_Attribute, szName, 0 ) < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Duplicate " );
         strcat_s( szMsg, zsizeof( szMsg ), szTE_MessText );
         strcat_s( szMsg, zsizeof( szMsg ), " for this Entity." );
         MessageSend( vSubtask, "ER00180", "ER Data Model Maintenance", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

         if ( zstrcmp( szTE_Attribute, "TE_ColumnName" ) == 0 )
            SetFocusToCtrl( vSubtask, "edTE_ColumnNameForAttibute" );
         else
            SetFocusToCtrl( vSubtask, "edIndexName" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   if ( AcceptSubobject( vTZEREMDO, szEntityName ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
} // zwTZEREMDD_AcceptSubobject
/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetFKeyPrefix1State
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SetFKeyPrefix1State( zVIEW vSubtask )
{
   zVIEW  vProfileXfer;
   zSHORT nEnableFKPrefix = 1;

   GetViewByName( &vProfileXfer, "ProfileXFER", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink1TE_NoForeignKeyPrefix", "Y" ) == 0 )
   {
      SetAttributeFromString( vProfileXfer, "TempRelLink", "RelLink1TE_ForeignKeyPrefix", "" );
      nEnableFKPrefix = 0;
   }

   SetCtrlState( vSubtask, "Prefix", zCONTROL_STATUS_ENABLED, nEnableFKPrefix );
   RefreshCtrl( vSubtask, "Prefix" );

   return( 0 );
} // zwTZEREMDD_SetFKeyPrefix1State


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetFKeyPrefix2State
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SetFKeyPrefix2State( zVIEW vSubtask )
{
   zVIEW  vProfileXfer;
   zSHORT nEnableFKPrefix = 1;

   GetViewByName( &vProfileXfer, "ProfileXFER", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vProfileXfer, "TempRelLink", "RelLink2TE_NoForeignKeyPrefix", "Y" ) == 0 )
   {
      SetAttributeFromString( vProfileXfer, "TempRelLink", "RelLink2TE_ForeignKeyPrefix", "" );
      nEnableFKPrefix = 0;
   }

   SetCtrlState( vSubtask, "Prefix_Other", zCONTROL_STATUS_ENABLED, nEnableFKPrefix );
   RefreshCtrl( vSubtask, "Prefix_Other" );

   return( 0 );
} // zwTZEREMDD_SetFKeyPrefix2State


#if 0
/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SAP_LoadLogonWindow
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SAP_LoadLogonWindow( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SAP_Import" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow, "TZERSAPD", "Logon" );
   }
   else
   {
      MapWindow( vWindow );
      zwTZERSAPD_BuildRelationships_TE( vWindow );
   }

   return( 0 );
} // zwTZEREMDD_SAP_LoadLogonWindow


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DelFactTypeFromSapId
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_DelFactTypeFromSapId( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERRLLO;
   zSHORT nRC;

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLinkIdentifier" ) >= zCURSOR_SET && CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 )
   {
      // FactType is Relationship.
      // Add rel back as cndidate to ERRLLO object
      if ( GetViewByName( &vTZERRLLO, "TZERRLLO", vSubtask, zLEVEL_TASK ) > 0 )
      {
         nRC = MessagePrompt( vSubtask, "ER00193", "ER Data Model Maintenance", "This is a internal Function for SAP Import.\n\n" "Do you want to remove the selected Relationship from Identifier?", zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
         if ( nRC == zRESPONSE_YES )
         {
            CreateEntity( vTZERRLLO, "ER_RelLink_Other", zPOS_AFTER );
            SetMatchingAttributesByName( vTZERRLLO, "ER_RelLink_Other", vTZEREMDO, "ER_RelLinkIdentifier", zSET_ALL );
            CreateEntity( vTZERRLLO, "ER_Entity_Other", zPOS_AFTER );
            SetMatchingAttributesByName( vTZERRLLO, "ER_Entity_Other", vTZEREMDO, "ER_Entity_Other_Identifier", zSET_ALL );
            DeleteEntity( vTZEREMDO, "ER_FactType", zREPOS_NEXT );
            zwTZEREMDD_RefreshIdWnd( vSubtask );
         }
      }
   }

   return( 0 );
} // zwTZEREMDD_DelFactTypeFromSapId
#endif


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SetCountError
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SetCountError( zVIEW vSubtask )
{
   zVIEW     vTZERDERR;
   zLONG     lCountError;
   zLONG     lCountWarning;
   zSHORT    nRC;
   zCHAR     szCount[ 10 ];
   zCHAR     szControlText[ 100 ];

   GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZERDERR, "Import" ) >= zCURSOR_SET )
   {
      // Set Count Errors to Textfield
      lCountError   = 0;
      lCountWarning = 0;
      for ( nRC = SetCursorFirstEntity( vTZERDERR, "ErrorMsg", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERDERR, "ErrorMsg", "" ) )
      {
         if ( CompareAttributeToInteger( vTZERDERR, "ErrorMsg", "Error", 1 ) == 0 )
            lCountError++;
         else
            lCountWarning++;
      }

      zltoa( lCountError, szCount, zsizeof( szCount ) );
      strcpy_s( szControlText, zsizeof( szControlText ), " " );
      strcat_s( szControlText, zsizeof( szControlText ), szCount );
      strcat_s( szControlText, zsizeof( szControlText ), " Error(s) " );

      if ( lCountWarning > 0 )
      {
         zltoa( lCountWarning, szCount, zsizeof( szCount ) );
         strcat_s( szControlText, zsizeof( szControlText ), ", " );
         strcat_s( szControlText, zsizeof( szControlText ), szCount );
         strcat_s( szControlText, zsizeof( szControlText ), " Warning(s) " );
      }

      strcat_s( szControlText, zsizeof( szControlText ), "found" );
   }
   else
   {
      strcpy_s( szControlText, zsizeof( szControlText ), " No Errors" );
   }

   SetCtrlText( vSubtask, "txtCountError", szControlText );

   return( 0 );
} // zwTZEREMDD_SetCountError


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_RefreshErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_RefreshErrorList( zVIEW vSubtask )
{
   zwTZEREMDD_SetCountError( vSubtask );
   RefreshWindow( vSubtask );

    return( 0 );
} // zwTZEREMDD_RefreshErrorList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_LoadErrorList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_LoadErrorList( zVIEW vSubtask )
{
   zVIEW  vTZERDERR;

   if ( GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // Create Work LOD for Error List
      ActivateEmptyObjectInstance( &vTZERDERR, "TZSAPERR", vSubtask, zMULTIPLE );
      SetNameForView( vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK );
   }

   zwTZEREMDD_SetCountError( vSubtask );

   return( 0 );
} // zwTZEREMDD_LoadErrorList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_CreateNewErrorRoot
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_CreateNewErrorRoot( zVIEW vSubtask )
{
   zVIEW  vTZERDERR;
   zVIEW  vTZEREMDO;
   zCHAR  szCurrentDateTime[ 20 ];

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZEREMDD_LoadErrorList( vSubtask );
      GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK );
   }

   SysGetDateTime( szCurrentDateTime, zsizeof( szCurrentDateTime ) );
   CreateEntity( vTZERDERR, "Import", zPOS_LAST );
   SetAttributeFromString( vTZERDERR, "Import", "SearchDateTime", szCurrentDateTime );

   return( 0 );
} // zwTZEREMDD_CreateNewErrorRoot



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ErrorListSelectEntity
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ErrorListSelectEntity( zVIEW vSubtask )
{
   zVIEW  vTZERDERR;
   zVIEW  vERD;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( SetCursorFirstEntityByAttr( vERD, "ER_Entity", "Name", vTZERDERR, "ErrorMsg", "EntityName", "" ) >= zCURSOR_SET )
      {
         zwTZEREMDD_SetEntitySelect( vSubtask );
      }
   }

   return( 0 );
} // zwTZEREMDD_ErrorListSelectEntity


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ErrorListSaveToFile
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ErrorListSaveToFile( zVIEW vSubtask )
{
   zVIEW  vTZERDERR;
   zULONG ulZFlags = 0;
   zLONG  FileHandle;
   zSHORT nRC;
   zCHAR  szMsg[ 300 ];
   zCHAR  szTemp[ 255 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];

   GetViewByName( &vTZERDERR, "TZERDERR", vSubtask, zLEVEL_TASK );

   szFileName[ 0 ] = 0;
   ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT |
              zOFN_SAVEAS | zOFN_HIDEREADONLY;

   if ( OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ), "Text Files (*.txt)|*.txt|", "txt", ulZFlags ) == 1 )
   {
      FileHandle = SysOpenFile( vSubtask, szFileName, COREFILE_WRITE );

      if ( FileHandle == -1 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Unable to open " );
         strcat_s( szMsg, zsizeof( szMsg ), szFileName );
         MessageSend( vSubtask, "FEH001", "SAP Import", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         return( -1 );
      }

      strcpy_s( szMsg, zsizeof( szMsg ), "Zeidon: Data Model Error List from " );
      GetStringFromAttributeByContext( szTemp, vTZERDERR, "Import", "SearchDateTime", "DD.MM.YYYY HH:MM:SS", 20 );
      strcat_s( szMsg, zsizeof( szMsg ), szTemp );
      SysWriteLine( vSubtask, FileHandle, szMsg );
      SysWriteLine( vSubtask, FileHandle, "" );

      for ( nRC = SetCursorFirstEntity( vTZERDERR, "ErrorMsg", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERDERR, "ErrorMsg", "" ) )
      {
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZERDERR, "ErrorMsg", "EntityName" );
         if ( zstrcmp( szTemp, "" ) != 0 )
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "Entity: " );
            strcat_s( szMsg, zsizeof( szMsg ), szTemp );
         }

         strcat_s( szMsg, zsizeof( szMsg ), "\t" );
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTZERDERR, "ErrorMsg", "ErrorText" );
         strcat_s( szMsg, zsizeof( szMsg ), szTemp );
         SysWriteLine( vSubtask, FileHandle, szMsg );
      }

      SysWriteLine( vSubtask, FileHandle, "" );
      SysWriteLine( vSubtask, FileHandle, "End of Error List" );
      SysCloseFile( vSubtask, FileHandle, 0 );
   }

   return( 0 );
} // zwTZEREMDD_ErrorListSaveToFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: EntityPurposeChange
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
EntityPurposeChange( zVIEW vSubtask )
{
   zVIEW  vERD;
   zCHAR  szEntityPurpose[ 2 ];

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // only enable dimension hierarchy button for dimensions
   GetStringFromAttribute( szEntityPurpose, zsizeof( szEntityPurpose ), vERD, "ER_Entity", "Purpose" );
   if ( *szEntityPurpose == '2' )
      SetCtrlState( vSubtask, "DimHierBtn",  zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "DimHierBtn",  zCONTROL_STATUS_ENABLED, 0 );

   return( 0 );
} // EntityPurposeChange

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DIMHIER_Prebuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DIMHIER_Prebuild( zVIEW vSubtask )
{

   zVIEW  vERD;
   zVIEW  vDimHier;
   zSHORT nRC;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_RLL", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( vDimHier );
   }
   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( vDimHier );
   }

   nRC = oTZEREMDO_DimHierInitLevels( vSubtask, vERD );

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // remove named view if it exists
   if ( GetViewByName( &vDimHier, "TZEREMDO_RLL", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( vDimHier );
   }

   CreateViewFromViewForTask( &vDimHier, vERD, 0 );
   SetNameForView( vDimHier, "TZEREMDO_RLL", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDimHier, vERD, 0 );
   SetNameForView( vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK );

   return( 0 );
} // DIMHIER_Prebuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelsListRefresh
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelsListRefresh( zVIEW vSubtask )
{

   zVIEW  vERD;
   zVIEW  vDimHier;
   zSHORT nRC;
   zLONG  lZKey;
   zLONG  lParentZKey;
   zSHORT  bFound;

   RefreshWindowExceptForCtrl( vSubtask, "DimLevelsList" );

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vERD, "ER_DimensionLevel" ) != 0 )
      return( 0 );

   // set recursive side to current flat side
   ResetView( vDimHier );
   SetViewFromView( vDimHier, vERD );
   GetIntegerFromAttribute( &lZKey, vERD, "ER_DimensionLevel", "ZKey" );
   GetIntegerFromAttribute( &lParentZKey, vERD, "ER_DimensionLevelParent", "ZKey" );
   bFound = 0;
   nRC = oTZEREMDO_DimSetLevelByZKeys( vDimHier, lZKey, lParentZKey, &bFound, vSubtask );

   return( 0 );
} // DimLevelsListRefresh

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelListTabLeave
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelListTabLeave( zVIEW vSubtask )
{
/* This is no longer needed, since the listbox always keeps the recursive view
   at the correct position.
   zVIEW  vERD;
   zVIEW  vDimHier;
   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );
   // remove named view if it exists
   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( vDimHier );
   }
   CreateViewFromViewForTask( &vDimHier, vERD );
   SetNameForView( vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK );
   // Should set position to subobject based on flat side
   //IssueError( vSubtask, 0, 0, "Should set position to subobject based on flat side" );
*/
   return( 0 );
} // DimLevelListTabLeave

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelOutlineTabLeave
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelOutlineTabLeave( zVIEW vSubtask )
{
   // this should be done as part of the outline control
   zVIEW  vERD;
   zVIEW  vDimHier;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // remove named view if it exists
   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   //IssueError( vSubtask, 0, 0, "Before - Set position of flat side based on current subobject" );
   // Set position of flat side based on current subobject
   SetCursorFirstEntityByEntityCsr( vERD, "ER_DimensionLevel", vDimHier, "ER_DimHierLevel", 0 );
   //IssueError( vSubtask, 0, 0, "After - Set position of flat side based on current subobject" );

   return( 0 );
} // DimLevelListTabLeave

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelsOutlinePopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelsOutlinePopup( zVIEW vSubtask )
{
   zVIEW vWork;
   zCHAR szEntityName[ 34 ];
   zCHAR sz[ 340 ];

   OL_GetCurrentEntityName( vSubtask, "DimLevelsOutline", &vWork, szEntityName );

   TraceLineS( "DimLevelsOutlinePopup EntityName=", szEntityName );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Name" );
   TraceLineS( "DimLevelsOutlinePopup Name=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "ZKey" );
   TraceLineS( "DimLevelsOutlinePopup ZKey=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Desc" );
   TraceLineS( "DimLevelsOutlinePopup Desc=", sz );

   //IssueError( vSubtask, 0, 0, "DimLevelsOutlinePopup" );

   CreateTrackingPopupMenu( vSubtask, "OutlinePopup", -1, -1, FALSE, FALSE );

   return( 0 );
} // DimLevelsOutlinePopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelsOutlinePreSelect
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelsOutlinePreSelect( zVIEW vSubtask )
{
   zVIEW vWork;
   zCHAR szEntityName[ 34 ];
   zCHAR sz[ 340 ];

   OL_GetCurrentEntityName( vSubtask, "DimLevelsOutline", &vWork, szEntityName );

   TraceLineS( "DimLevelsOutlinePreSelect EntityName=", szEntityName );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Name" );
   TraceLineS( "DimLevelsOutlinePreSelect Name=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "ZKey" );
   TraceLineS( "DimLevelsOutlinePreSelect ZKey=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Desc" );
   TraceLineS( "DimLevelsOutlinePreSelect Desc=", sz );

   //IssueError( vSubtask, 0, 0, "DimLevelsOutlinePreSelect" );

   return( 0 );
} // DimLevelsOutlinePreSelect

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelsOutlineSelect
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelsOutlineSelect( zVIEW vSubtask )
{
   zVIEW vWork;
   zCHAR szEntityName[ 34 ];
   zCHAR sz[ 340 ];
   zVIEW  vDimHier;

   OL_GetCurrentEntityName( vSubtask, "DimLevelsOutline", &vWork, szEntityName );

   TraceLineS( "DimLevelsOutlineSelect EntityName=", szEntityName );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Name" );
   TraceLineS( "DimLevelsOutlineSelect Name=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "ZKey" );
   TraceLineS( "DimLevelsOutlineSelect ZKey=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Desc" );
   TraceLineS( "DimLevelsOutlineSelect Desc=", sz );

   // set recursive and flat views to current selection
   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      SetViewFromView( vDimHier, vWork );
      GetViewByName( &vWork, "TZEREMDO", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntityByEntityCsr( vWork, "ER_DimensionLevel", vDimHier, "ER_DimHierLevel", 0 );
   }

   //IssueError( vSubtask, 0, 0, "DimLevelsOutlineSelect" );

   return( 0 );
} // DimLevelsOutlineSelect

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DIMTEST_includeDrillDown
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DIMTEST_includeDrillDown( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHier;
   zVIEW  vWork;
   zSHORT nRC;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vWork, vERD, 0 );

   nRC = MoveSubobject( vWork, "ER_DimHierLevel", vDimHier, "ER_DimHierLevel", zPOS_AFTER, zREPOS_BEFORE );

   nRC = oTZEREMDO_DimHierSyncLevelsR2F( vSubtask, vDimHier, vERD );

   return( 0 );
} // DIMTEST_includeDrillDown

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimAttribGridTest
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimAttribGridTest( zVIEW vSubtask )
{
   zCHAR sz[ 340 ];

   GetActionName( vSubtask, sz );
   TraceLineS( "Test Action=", sz );

   //if ( zstrcmp( sz, "OutlinePopupInit" ) == 0 )
   //{
   //}

   return( 0 );
} // DimAttribGridTest

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelNewSibling
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelNewSibling( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHier;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   oTZEREMDO_DimHierAddSiblingLevel( vSubtask, vDimHier, vERD );

   SetCursorFirstEntityByEntityCsr( vERD, "ER_DimensionLevel", vDimHier, "ER_DimHierLevel", 0 );

   return( 0 );
} // DimLevelNewSibling

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelDeleteLevel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelDeleteLevel( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHier;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   oTZEREMDO_DimHierDeleteLevel( vSubtask, vDimHier, vERD );

   SetCursorFirstEntityByEntityCsr( vERD, "ER_DimensionLevel", vDimHier, "ER_DimHierLevel", 0 );

   return( 0 );
} // DimLevelDeleteLevel

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelNewDrillDown
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelNewDrillDown( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHier;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   SetCursorFirstEntityByEntityCsr( vDimHier, "ER_DimensionLevelChild", vERD, "ER_DimHierLevelChild", 0 );

   oTZEREMDO_DimHierAddChildLevel( vSubtask, vDimHier, vERD );

   SetCursorFirstEntityByEntityCsr( vERD, "ER_DimensionLevel", vDimHier, "ER_DimHierLevel", 0 );

   return( 0 );
} // DimLevelNewDrillDown

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelRemoveDrillDown
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelRemoveDrillDown( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHier;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   SetCursorFirstEntityByEntityCsr( vERD, "ER_DimHierLevelChild", vERD, "ER_DimensionLevelChild", 0 );
   SetCursorFirstEntityByEntityCsr( vDimHier, "ER_DimHierLevelChild", vERD, "ER_DimensionLevelChild", 0 );

   oTZEREMDO_DimHierRemoveChildLvl( vSubtask, vDimHier, vERD );

   SetCursorFirstEntityByEntityCsr( vERD, "ER_DimensionLevel", vDimHier, "ER_DimHierLevel", 0 );

   return( 0 );
} // DimLevelRemoveDrillDown

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelNewAttr
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelNewAttr( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHier;

   if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDimHier, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   oTZEREMDO_DimHierAddAttribute( vDimHier, TRUE, FALSE, vERD, vSubtask );

   return( 0 );
} // DimLevelNewAttr

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimOutlinePopupInit
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimOutlinePopupInit( zVIEW vSubtask )
{
   zCHAR sz[ 340 ];
   zLONG lRC;

   GetActionName( vSubtask, sz );
   TraceLineS( "Test Action=", sz );

   lRC = RemoveOptionFromMenu( vSubtask, "Opt1");
   strcpy_s( sz, zsizeof( sz ), "Move Up One Level" );
   lRC = SetOptionState( vSubtask, "Opt2", zOPTION_STATUS_TEXT, (zULONG) sz );
   lRC = AddOptionToMenu( vSubtask, "", "", "OptMoveUp", "Move To &Top", "DIMTEST_Move", 514 );
   lRC = AddOptionToMenu( vSubtask, "", "", "OptMoveUp2", "Move 2 &Top", "DIMTEST_Move", 2 );

   return( 0 );
} // DimOutlinePopupInit

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimTargetOutlineSelect
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimTargetOutlineSelect( zVIEW vSubtask )
{
   zVIEW vWork;
   zCHAR szEntityName[ 34 ];
   zCHAR sz[ 340 ];
   zVIEW  vDimHier;

   OL_GetCurrentEntityName( vSubtask, "DimTargetOutline", &vWork, szEntityName );

   TraceLineS( "DimTargetOutlineSelect EntityName=", szEntityName );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Name" );
   TraceLineS( "DimTargetOutlineSelect Name=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "ZKey" );
   TraceLineS( "DimTargetOutlineSelect ZKey=", sz );
   GetStringFromAttribute( sz, zsizeof( sz ), vWork, szEntityName, "Desc" );
   TraceLineS( "DimTargetOutlineSelect Desc=", sz );

   // set recursive and flat views to current selection
   if ( GetViewByName( &vDimHier, "TZEREMDO_tgt", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      SetSelectStateOfEntity( vDimHier, "ER_DimHierLevel", 0 );
      SetViewFromView( vDimHier, vWork );
      SetSelectStateOfEntity( vDimHier, "ER_DimHierLevel", 1 );
   }

   return( 0 );
} // DimTargetOutlineSelect

zSHORT /* LOCAL */  OPERATION
fnDimConversSetup( zVIEW   vSubtask, zPVIEW  pvERD, zPVIEW  pvDimHierSrc, zPVIEW  pvDimHierTgt, zPVIEW  pvDimHierOutlineT )
{
   if ( GetViewByName( pvERD, "TZEREMDO_RLL", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( pvDimHierSrc, "TZEREMDO_sub", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( pvDimHierOutlineT, "TZEREMDO_OutlineT", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( *pvDimHierOutlineT );
   }

   if ( GetViewByName( pvDimHierTgt, "TZEREMDO_tgt", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropView( *pvDimHierTgt );
   }

   CreateViewFromViewForTask( pvDimHierOutlineT, *pvERD, 0 );
   SetNameForView( *pvDimHierOutlineT, "TZEREMDO_OutlineT", vSubtask, zLEVEL_TASK );
   SetSelectStateOfEntity( *pvDimHierOutlineT, "ER_DimHierLevel", 0 );
   while ( ResetViewFromSubobject( *pvDimHierOutlineT ) == 0 );
   SetSelectStateOfEntity( *pvDimHierOutlineT, "ER_DimHierLevel", 1 );

   CreateViewFromViewForTask( pvDimHierTgt, *pvDimHierSrc, 0 );
   SetNameForView( *pvDimHierTgt, "TZEREMDO_tgt", vSubtask, zLEVEL_TASK );
   SetSelectStateOfEntity( *pvDimHierTgt, "ER_DimHierLevel", 0 );
   while ( ResetViewFromSubobject( *pvDimHierTgt ) == 0 );
   SetSelectStateOfEntity( *pvDimHierTgt, "ER_DimHierLevel", 1 );

   return( 0 );
} // fnDimConversSetup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelStructCopyTo
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelStructCopyTo( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHierSrc;
   zVIEW  vDimHierTgt;
   zVIEW  vDimHierOutlineT;
   zCHAR  szTag[ 65 ];
   zSHORT nRC;

   nRC = fnDimConversSetup( vSubtask, &vERD, &vDimHierSrc, &vDimHierTgt, &vDimHierOutlineT );
   if ( nRC < 0 )
      return( nRC );

   nRC = OperatorConversationalDialog( vSubtask, szTag, "TZEREMDD", "DimHierTargetSelect" );

   if ( zstrcmp( szTag, "TOP" ) == 0 )
   {
       while ( ResetViewFromSubobject( vDimHierTgt ) == 0 )
         ;

       oTZEREMDO_DimHierCopyLevelSub( vDimHierTgt, vDimHierSrc, vERD, FALSE, vSubtask );
   }
   else
   if ( zstrcmp( szTag, "SELECT" ) == 0 )
   {
       oTZEREMDO_DimHierCopyLevelSub( vDimHierTgt, vDimHierSrc, vERD, TRUE, vSubtask );
   }

   return( 0 );

} // DimLevelStructCopyTo

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelCopyTo
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelCopyTo( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHierSrc;
   zVIEW  vDimHierTgt;
   zVIEW  vDimHierOutlineT;
   zSHORT nRC;
   zCHAR  szTag[ 65 ];

   nRC = fnDimConversSetup( vSubtask, &vERD, &vDimHierSrc, &vDimHierTgt, &vDimHierOutlineT );
   if ( nRC < 0 )
      return( nRC );

   nRC = OperatorConversationalDialog( vSubtask, szTag, "TZEREMDD", "DimHierTargetSelect" );

   if ( zstrcmp( szTag, "TOP" ) == 0 )
   {
      while ( ResetViewFromSubobject( vDimHierTgt ) == 0 )
         ;

      oTZEREMDO_DimHierCopyLevel( vDimHierTgt, vDimHierSrc, FALSE, vSubtask );

      if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
         return( -1 );

      oTZEREMDO_DimHierSyncLevelsR2F( vSubtask, vDimHierSrc, vERD );
   }
   else
   if ( zstrcmp( szTag, "SELECT" ) == 0 )
   {
      if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
         return( -1 );

      oTZEREMDO_DimHierRemoveFlatSide( vSubtask, vERD );

      oTZEREMDO_DimHierCopyLevel( vDimHierTgt, vDimHierSrc, TRUE, vSubtask );

      if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
         return( -1 );

      oTZEREMDO_DimHierSyncLevelsR2F( vSubtask, vDimHierSrc, vERD );
   }

   return( 0 );

} // DimLevelCopyTo

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimLevelStructMoveTo
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimLevelStructMoveTo( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vDimHierSrc;
   zVIEW  vDimHierTgt;
   zVIEW  vDimHierOutlineT;
   zSHORT nRC;
   zCHAR  szTag[ 65 ];

   nRC = fnDimConversSetup( vSubtask, &vERD, &vDimHierSrc, &vDimHierTgt, &vDimHierOutlineT );
   if ( nRC < 0 )
      return( nRC );

   nRC = OperatorConversationalDialog( vSubtask, szTag, "TZEREMDD", "DimHierTargetSelect" );

   if ( zstrcmp( szTag, "TOP" ) == 0 )
   {
       while ( ResetViewFromSubobject( vDimHierTgt ) == 0 );
       oTZEREMDO_DimHierRemoveFlatSide( vSubtask, vDimHierTgt );

       MoveSubobject( vDimHierTgt, "ER_DimHierLevel", vDimHierSrc, "ER_DimHierLevel", zPOS_AFTER, zREPOS_BEFORE );

       if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
          return( -1 );

       oTZEREMDO_DimHierSyncLevelsR2F( vSubtask, vDimHierSrc, vERD );
   }
   else
   if ( zstrcmp( szTag, "SELECT" ) == 0 )
   {
       if ( GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
          return( -1 );

       oTZEREMDO_DimHierRemoveFlatSide( vSubtask, vERD );

       MoveSubobject( vDimHierTgt, "ER_DimHierLevelChild", vDimHierSrc, "ER_DimHierLevel", zPOS_AFTER, zREPOS_BEFORE );

       oTZEREMDO_DimHierSyncLevelsR2F( vSubtask, vDimHierSrc, vERD );
   }

   return( 0 );

} // DimLevelStructMoveTo

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_KeepCurrentOrder( zVIEW vSubtask )
{
   // This operation resets the sequence numbers of the attributes to keep the current order.

   zVIEW  vERD;
   zSHORT nRC;
   zSHORT nSequenceNumber;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Loop through all ER_Attribute entities and renumber the sequence numbers.
   nSequenceNumber = 0;
   for ( nRC = SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Attribute", 0 ) )
   {
      nSequenceNumber = nSequenceNumber + 1;
      SetAttributeFromInteger( vERD, "ER_Attribute", "wkSequence", nSequenceNumber );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimHierTargetPrebuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimHierTargetPrebuild( zVIEW vSubtask )
{
   IssueError( vSubtask, 0, 0, "DimHierTargetPrebuild" );
   return( 0 );

} // DimHierTargetPrebuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DimHierTargetPostbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DimHierTargetPostbuild( zVIEW vSubtask )
{
   IssueError( vSubtask, 0, 0, "DimHierTargetPostbuild" );
   return( 0 );

} // DimHierTargetPostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_CLOSE_MergeSelected
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_CLOSE_MergeSelected( zVIEW vSubtask )
{


   return( 0 );
} // zwTZEREMDD_CLOSE_MergeSelected

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_CompareERDs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_CompareERDs( zVIEW vSubtask )
{
   zVIEW  vSourceERD;
   zVIEW  vTargetERD;
   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ 500 ];
   zCHAR  szERD_Name[ 33 ];
   zCHAR  szSourceLPLR_Name[ 33 ];
   zSHORT nRC;

   // Compare the Target ERD, the ERD we are currently updating, to a ERD of the same name
   // that is contained in the specified Directory Structure.
   // If an LPLR name has been entered, use it to get the XLP file and any dependent Domains.

   // Get view of current ERD to be Target and view of current LPLR.
   GetViewByName( &vTargetERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   SetNameForView( vTargetERD, "TargetERD", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Get the Source ERD Name. If a Source LPLR name is specified, then that is the name of the ERD.
   // Otherwise, the ERD Name for the Source is considered to be the same as in the Target.
   GetStringFromAttribute( szSourceLPLR_Name, zsizeof( szSourceLPLR_Name ), vTaskLPLR, "LPLR", "wMergeSourceLPLR_Name" );
   if ( zstrcmp( szSourceLPLR_Name, "" ) != 0 )
      strcpy_s( szERD_Name, zsizeof( szERD_Name ), szSourceLPLR_Name );
   else
   {
      SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type", 4, "" );     // 4 is ERD Meta Type
      GetStringFromAttribute( szERD_Name, zsizeof( szERD_Name ), vTaskLPLR, "W_MetaDef", "Name" );
   }

   // Activate Source view. We are not necessarily positioned on the ER meta entry.
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "LPLR", "wFullyQualifiedFileName" );
   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szERD_Name );
   strcat_s( szFileName, zsizeof( szFileName ), ".PMD" );
   TraceLineS( "Source File Name: ", szFileName );
   nRC = ActivateOI_FromFile( &vSourceERD, "TZEREMDO", vSubtask, szFileName, zSINGLE );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "", "Compare ERD's", "Invalid File Name", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   SetNameForView( vSourceERD, "SourceERD", vSubtask, zLEVEL_TASK );

   // Call operation to compare ERD's.
   oTZEREMDO_ERD_Compare( vTargetERD, vSourceERD );

   return( 0 );

} // zwTZEREMDD_CompareERDs

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_ERD_MergeSelected
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_ERD_MergeSelected( zVIEW vSubtask )
{
   zVIEW  vSourceERD;
   zVIEW  vTargetERD;

   // Go to Merge selected entries.

   // Get view of current ERD to be Target.
   GetViewByName( &vSourceERD, "SourceERD", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTargetERD, "TargetERD", vSubtask, zLEVEL_TASK );
   oTZEREMDO_ERD_Merge( vTargetERD, vSourceERD, vSubtask );

   return( 0 );
} // zwTZEREMDD_LOD_MergeSelected

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SelectCompEntries
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SelectCompEntries( zVIEW vSubtask )
{
   zVIEW  vTargetERD;
   zSHORT nRC;

   // Set the Selected Attribute to "Y" for each selected entry.
   GetViewByName( &vTargetERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstEntity( vTargetERD, "DisplayCompareResult", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTargetERD, "DisplayCompareResult", "" ) )
   {
      nRC = GetSelectStateOfEntity( vTargetERD, "DisplayCompareResult" );
      if ( nRC == 1 )
      {
         SetAttributeFromString( vTargetERD, "DisplayCompareResult", "SelectedFlag", "Y" );
         SetSelectStateOfEntity( vTargetERD, "DisplayCompareResult", 0 );
      }
   }

   // Set selected line to italic and red. (0-all columns, 255-text color, -1-background color, 2-italic)
   LB_SetColorAttributeName( vSubtask, "LBResultList", "SelectedFlag", 0 );
   LB_SetColorAssociation( vSubtask, "LBResultList", 0, "Y", 255, -1, 0 );

   return( 0 );
} // zwTZEREMDD_SelectCompEntries

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_UnselectCompEntries
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_UnselectCompEntries( zVIEW vSubtask )
{
   zVIEW  vTargetERD;
   zSHORT nRC;

   // Set the Selected Attribute to "Y" for each selected entry.
   GetViewByName( &vTargetERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstEntity( vTargetERD, "DisplayCompareResult", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTargetERD, "DisplayCompareResult", "" ) )
   {
      nRC = GetSelectStateOfEntity( vTargetERD, "DisplayCompareResult" );
      if ( nRC == 1 )
      {
         SetAttributeFromString( vTargetERD, "DisplayCompareResult", "SelectedFlag", "" );
      }
   }

   // Set selected line to italic and red. (0-all columns, 255-text color, -1-background color, 2-italic)
   LB_SetColorAttributeName( vSubtask, "LBResultList", "SelectedFlag", 0 );
   LB_SetColorAssociation( vSubtask, "LBResultList", 0, "Y", 255, -1, 0 );

   return( 0 );
} // zwTZEREMDD_UnselectCompEntries

/*************************************************************************************************
**
**    OPERATION: zwTZEREMDD_AnalyzeDuplicateZKeys
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_AnalyzeDuplicateZKeys( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTaskLPLR;

   // Refresh the hierarchical diagram.

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) == zLEVEL_TASK )
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

      // Identify Entities that should not be part of duplicate check, since they are the same instances
      // of other Entities in the object.
      if ( CheckExistenceOfEntity( vTaskLPLR, "DuplicateCheckEntity" ) < 0 )
      {
         // No Duplicate Entities have not been defined for a LOD, so do it.
         CreateEntity( vTaskLPLR, "DuplicateCheckEntity", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DuplicateCheckEntity", "EntityName", "ER_Entity" );
         CreateEntity( vTaskLPLR, "DuplicateCheckEntity", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DuplicateCheckEntity", "EntityName", "ER_Attribute" );
         CreateEntity( vTaskLPLR, "DuplicateCheckEntity", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DuplicateCheckEntity", "EntityName", "ER_RelLink" );
         CreateEntity( vTaskLPLR, "DuplicateCheckEntity", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DuplicateCheckEntity", "EntityName", "ER_EntIdentifier" );
         CreateEntity( vTaskLPLR, "DuplicateCheckEntity", zPOS_AFTER );
         SetAttributeFromString( vTaskLPLR, "DuplicateCheckEntity", "EntityName", "ER_RelType" );
      }

      // Go to check for duplicates.
      oTZCMLPLO_CheckOI_ForDupZKey( vTaskLPLR, vTZEREMDO, "EntpER_Model" );
   }

   return( 0 );
} // zwTZEREMDD_AnalyzeDuplicateZKeys

// KJS 08/25/14 - I am adding this because we'd like to rebuild the TE when saving the ER. I am copying this code from tzteupdd.c which
// makes me think that maybe I should add the code to TZTENVRO.c but I'm not sure...
// ******************************************************

/*
   This function finds the ZKey of the ER attribute that corresponds to the
   current physical key (TE_FieldDataRel_PK).
   We pass in two views for performance reasons.  This way we don't have to
   create/drop a new view everytime we are called.
   NOTE: This function changes the cursor positions of vDTE.
*/
zLONG LOCALOPER
fnFindER_ZKeyForPhysicalKey( zVIEW vDTE_Orig, zVIEW vDTE )
{
   zLONG   lZKey = -1;
   zSHORT  nRC;

   SetViewFromView( vDTE, vDTE_Orig );

   // Keep looping until we find the ER attribute.
   do
   {
      // Get the ZKey of the physical key.
      GetIntegerFromAttribute( &lZKey, vDTE, "TE_FieldDataRel_PK", "ZKey" );

      // Now find the Field with the corresponding ZKey.
      nRC = SetCursorFirstEntityByInteger( vDTE, "TE_FieldDataRel", "ZKey", lZKey, "TE_DBMS_Source" );

   } while ( nRC >= zCURSOR_SET && CheckExistenceOfEntity( vDTE, "ER_Attribute" ) < zCURSOR_SET );

   if ( nRC >= zCURSOR_SET )
      GetIntegerFromAttribute( &lZKey, vDTE, "ER_Attribute", "ZKey" );

   return( lZKey );
}

zSHORT LOCALOPER
fnRebuildTables( zVIEW vSubtask,
                 zVIEW vDTE,
                 zVIEW vEMD,
                 zPCHAR pszTE_EntityName )
{
   zVIEW  vDTE_Old;
   zVIEW  vDTE_OldTemp;
   zVIEW  vDTE_NewTemp;
   zVIEW  vTZTEDBLO;
   zSHORT nRC;

   zCHAR szSironDB_Type[ 2 ];

   // Drop Error List View
   //zwTZTEUPDD_ClearErrorWindow( vSubtask );

   // Used later in oTZTENVRO_BuildRelationsFromEMD().
   ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl", zMULTIPLE );
   SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   // First create a copy of the TE.
   ActivateOI_FromOI_ForTask( &vDTE_Old, vDTE, 0, zMULTIPLE );
   SetCursorFirstEntityByAttr( vDTE_Old, "TE_DBMS_Source", "ZKey", vDTE, "TE_DBMS_Source", "ZKey", 0 );

   CreateViewFromViewForTask( &vDTE_OldTemp, vDTE_Old, 0 );
   CreateViewFromViewForTask( &vDTE_NewTemp, vDTE, 0 );

   // Get the SironDB_Type because we only have to rebuild the tables for Siron catalogs
   GetStringFromAttribute( szSironDB_Type, zsizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

   // Run through all the tables and columns and delete them.  Don't delete the tables for Siron Catalogs
   if ( *szSironDB_Type != 'F' )
   {
      for ( nRC = SetCursorFirstEntity( vDTE, "TE_TablRec", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE, "TE_TablRec", 0 ) )
      {
         DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
      }
   }

   // Rebuild the entities
   if ( oTZTENVRO_BuildTablRecsFromEMD( vSubtask, vDTE, vEMD, pszTE_EntityName ) < 0 )
   {
      DropView( vDTE_Old );
      DropView( vDTE_OldTemp );
      DropView( vDTE_NewTemp );
      return( -1 );
   }

   // Do not Rebuild relationships for Siron Catalogs
   // We don't need to reset physical information from the copy
   // for Siron catalogs
   if ( *szSironDB_Type != 'F' )
   {
      // Rebuild the relationships.
      oTZTENVRO_BuildRelationsFromEMD( vSubtask, vDTE, vEMD );

      // Reset some of the physical information from the copy.
      for ( nRC = SetCursorFirstEntity( vDTE_Old, "TE_TablRec", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE_Old, "TE_TablRec", 0 ) )
      {
         // If the TablRec in the TE copy has the flag set and if the entity is in the new TE copy physical attributes.

         if ( CheckExistenceOfEntity( vDTE_Old, "ER_Entity" ) == zCURSOR_SET &&
              SetCursorFirstEntityByAttr( vDTE, "ER_Entity", "ZKey", vDTE_Old, "ER_Entity", "ZKey", "TE_DBMS_Source" ) >= zCURSOR_SET ||
              CheckExistenceOfEntity( vDTE_Old, "ER_RelType" ) == zCURSOR_SET &&
              SetCursorFirstEntityByAttr( vDTE, "ER_RelType", "ZKey", vDTE_Old, "ER_RelType", "ZKey", "TE_DBMS_Source" ) >= zCURSOR_SET )
         {
            SetAttributeFromAttribute( vDTE, "TE_TablRec", "KeepPhysCharsFlagForAllColumns", vDTE_Old, "TE_TablRec", "KeepPhysCharsFlagForAllColumns" );

            if ( CompareAttributeToString( vDTE_Old, "TE_TablRec", "KeepPhysicalCharacteristicsFlag", "Y" ) == 0 )
            {
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "Name", vDTE_Old, "TE_TablRec", "Name" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "SQL_TableQual", vDTE_Old, "TE_TablRec", "SQL_TableQual" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "SQL_TableOwner", vDTE_Old, "TE_TablRec", "SQL_TableOwner" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "Desc", vDTE_Old, "TE_TablRec", "Desc" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "KeepPhysicalCharacteristicsFlag", vDTE_Old, "TE_TablRec", "KeepPhysicalCharacteristicsFlag" );
            }
         }

         // Do the same thing with the TE fields.
         for ( nRC = SetCursorFirstEntityByString( vDTE_Old, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", "Y", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByString( vDTE_Old, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", "Y", 0 ) )
         {
            // Make sure attr exists in new copy.
            if ( CheckExistenceOfEntity( vDTE_Old, "ER_Attribute" ) == zCURSOR_SET )
            {
               if ( SetCursorFirstEntityByAttr( vDTE, "ER_Attribute", "ZKey", vDTE_Old, "ER_Attribute", "ZKey", "TE_TablRec" ) < zCURSOR_SET )
               {
                  // This ER_Attribute no longer exists so skip it.
                  continue;
               }
            } // if ER_Attribute exists...
            else
            {
               zBOOL bFound = FALSE;
               zLONG lOldZKey;

               // Get the ZKey of the originating ER Attribute.
               lOldZKey = fnFindER_ZKeyForPhysicalKey( vDTE_Old, vDTE_OldTemp );

               // We have to find the FK in the new TE that matches the old
               // TE in both relationship and originating parent key.
               for ( nRC = SetCursorFirstEntityByAttr( vDTE, "ER_RelLink", "ZKey", vDTE_Old, "ER_RelLink", "ZKey", "TE_TablRec" );
                     nRC >= zCURSOR_SET;
                     nRC = SetCursorNextEntityByAttr( vDTE, "ER_RelLink", "ZKey", vDTE_Old, "ER_RelLink", "ZKey", "TE_TablRec" ) )
               {
                  // Check to see if the ZKey for the ER attribute in the new TE matches the one for the old TE.
                  // If they do then we've found our man.
                  if ( fnFindER_ZKeyForPhysicalKey( vDTE, vDTE_NewTemp ) == lOldZKey )
                  {
                     bFound = TRUE;
                     break;
                  }
               }

               if ( !bFound )
                  continue; // This Relationshiop no longer exists so skip it.
            }

            // Copy physical information.
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Name", vDTE_Old, "TE_FieldDataRel", "Name" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Desc", vDTE_Old, "TE_FieldDataRel", "Desc" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Key", vDTE_Old, "TE_FieldDataRel", "Key" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_SCALE", vDTE_Old, "TE_FieldDataRel", "SQL_SCALE" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_NULLS", vDTE_Old, "TE_FieldDataRel", "SQL_NULLS" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_LABEL", vDTE_Old, "TE_FieldDataRel", "SQL_LABEL" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_REMARKS", vDTE_Old, "TE_FieldDataRel", "SQL_REMARKS" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", vDTE_Old, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "DataType", vDTE_Old, "TE_FieldDataRel", "DataType" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Length", vDTE_Old, "TE_FieldDataRel", "Length" );
         }
      }
   }

   DropView( vDTE_Old );
   DropView( vDTE_OldTemp );
   DropView( vDTE_NewTemp );

   // if field sequence does not exist, set it
   oTZTENVRO_SetFieldSequence( vDTE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RebuildTablesRels( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vEMD;
   zVIEW    vErrorList;
   zVIEW    vTE_Work;
   zSHORT   nRC;
   zCHAR    szSystemApp[ 65 ] = { 0 };
   zCHAR    szTranslateColumnUnderscore[ 2 ] = { 0 };
   zCHAR    szLPLR_Name[ 33 ] = { 0 };
   zCHAR    szER_UpdateFlag[ 2 ];

   // KJS 08/26/14 - I have copied this code from the TE. Unless I save the ER, this doesn't create the TE with the latest changes.
   // Should I prompt for save? Or save ER automatically? Or use the view that is already in existence for the ER?
   // Also, when we have finished, there are two l... views that were created (TZCMLPLO) that are not deleted. Does this matter?


   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY ) < 0 )
   {
      oTZTENVRO_GetUpdViewForDTE_P( vSubtask, &vDTE );
      if ( vDTE )
      {
         oTZTENVRO_SetFieldSequence( vDTE );
         // KJS 08/25/14 - I'm not sure... do I need to do all of the following if I'm in the ER?
         // not sure yet about the SetFieldSequence, not sure what that does.
         /*
         SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
         // Set window Title with check out state
         SetTitleWithCheckOutState( vSubtask, "Physical Data Model", "TE_DB_Environ", vDTE, "TE_DB_Environ", zSOURCE_DTE_META );
         // if field sequence does not exist, set it and remove update flag
         oTZTENVRO_SetFieldSequence( vDTE );
         lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vDTE->hViewCsr );
         lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
         lpViewOI->bUpdatedFile = FALSE;
         // if PE not checked out, set TE View read only ->
         // the user cannot update the values in Detail Windows
         if ( !ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
            SetViewReadOnly( vDTE );
         if ( GetViewByName( &vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vDTE_Old );
         */
      }
      else
         return( -1 );   }

   SetNameForView( vDTE, "vDTE_Init", vSubtask, zLEVEL_TASK );

   if ( oTZEREMDO_GetRefViewForER( vSubtask, &vEMD, zCURRENT_OI ) < 0 )
      return( -1 );

   // We can't init table recs with a Remote Server DBMS.
   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "Paradigm", "S" ) == 0 )
   {
      MessageSend( vSubtask, "TE00423", "Physical Data Model",
                   "Can't add Table Records for Remote Servers",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Rebuilding all data sources.
   fnRebuildTables( vSubtask, vDTE, vEMD, "TE_DB_Environ" );

   // Send Error List Window
   if ( GetViewByName( &vErrorList, "TZTEERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      //RefreshWindow( vSubtask );    // Refresh Main Window
      //SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow, "TZTEUPDD", "ErrorList" );
      //GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ),
      //RefreshWindow( vWindow );    // Refresh Error List
   }
   else
   {
      // Save the TE. This code is copied from zwTZTEUPDD_SaveModelDTE.  Make sure object is not empty
      nRC = CheckExistenceOfEntity( vDTE, "TE_DB_Environ" );
      if (  nRC < zCURSOR_SET )
      {
         return( -1 );
      }

      // Sort Attributes in original sequence
      oTZTENVRO_SortFields( vDTE, vSubtask );

      nRC = CommitMetaOI( vSubtask, vDTE, zSOURCE_DTE_META );
      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "TE00420", "Physical Data Model",
                      "Unable to save Physical Environment.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
      else
      {
         // save XDM here in the future..................
      }

      // KJS 08/26/14 - Do we want the following TZTEWRKO code???

      // Save the ER if it was updated as a part of Identifier or Relationship Syncronization.
      nRC = GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
      if ( nRC >= 0 )
      {
         GetStringFromAttribute( szER_UpdateFlag, zsizeof( szER_UpdateFlag ), vTE_Work, "TE_Work", "ER_UpdateFlag" );
         if ( szER_UpdateFlag[ 0 ] == 'Y' )
         {
            // Get the ERD View
            nRC = GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
            CommitMetaOI( vSubtask, vEMD, zSOURCE_ERD_META );
            DropView (vEMD);
         }
         DropView (vTE_Work);
      }

      // KJS 08/26/14 - This is needed because of the oTZEREMDO_GetRefViewForER.
      DropMetaOI( vSubtask, vEMD );

   }

   DropMetaOI( vSubtask, vDTE );

   return( 0 );
}

/*************************************************************************************************
**
**    OPERATION: GenerateID_Identifiers
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateID_Identifiers( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   // Execute routine to convert ZKey Domains.
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      oTZEREMDO_GenerateID_Identifiers( vTZEREMDO );

   return( 0 );

} // GenerateZKeyDomains

/*************************************************************************************************
**
**    OPERATION: GenerateZKeyDomains
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateZKeyDomains( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   // Execute routine to convert ZKey Domains.
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      oTZEREMDO_ConvertZKeyDomains( vTZEREMDO );

   return( 0 );

} // GenerateZKeyDomains


#ifdef __cplusplus
}
#endif
