/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzzolodd.c - Zeidon LOD dialog operations
// DESCRIPTION:   This C file contains dialog operations for the
//                Zeidon LOD application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
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
// LAST MESSAGE ID: ZO00183
//
// HISTORY:
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2002.07.11 DC
   Changed TZZOLODD.C code to create TZZOLODQ.C code for Query function.
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

#include "tzlodopr.hg"

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshAttributeList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshEntityList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteAllOperations( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteAllFiles( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteSelectedOperat( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteSourceFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_InvokeEditor( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_GoToUpdateOperation( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_AddSourceFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SetUpTZOPRUSO_View( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_LoadLODList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OpenLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EntityNext( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EntityPrev( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CreateNewLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CloseLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PrintLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CancelNewLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_InitializeDialog( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODChildEnt( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vTZZOLOD1 );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODEntity( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vTZZOLOD1 );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODAttrib( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vTZZOLOD1, zCPCHAR cpcDEBUGCHG );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODDBVChildEnt( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vTZZOLOD1, zVIEW vKZOMEROO );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODDataRec( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vKZOMEROO );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODRelRec( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vKZOMEROO,
                           zVIEW vTZZOLOD1, zVIEW vTZZOLOD2 );

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_BldXODDataField( zVIEW vSubtask, zVIEW vTZZOXODO, zVIEW vKZOMEROO );


zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshAttWnd( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshOperWnd( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshSourceWnd( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_LocateRecursiveEntity( zVIEW vLOD,
                                  zLONG lLOD_EntityZKey );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OKEntity( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ReusOKEntity( zVIEW vSubtask );

static zSHORT
zwfnTZZOLODD_CheckActionAfterSaveAs( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_AddOperation( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildEntmaint( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshEntitySelect( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZZOLODD_SetAttributeType( zVIEW vSubtask, zVIEW vTZZOLODO );

static zSHORT
zwfnTZZOLODD_SaveAsSetSourceName( zVIEW   vSubtask,
                                  zVIEW   vSaveAs,
                                  zVIEW   vLodData,
                                  zSHORT  nIndex );
static zSHORT
zwfnTZZOLODD_SaveAsCheckName( zVIEW    vSubtask,
                              zVIEW    vSaveAs,
                              zVIEW    vTZZOLODO,
                              zPCHAR   szOutName );

static zSHORT
zwfnTZZOLODD_SaveAsCheckFileName( zVIEW    vSubtask,
                                  zVIEW    vSaveAs );

static zSHORT
zwfnTZZOLODD_SaveAsCheckStatus( zVIEW    vSubtask,
                                zVIEW    vTZZOLFLO,
                                zPCHAR   szOutName );

static zSHORT
zwfnTZZOLODD_SaveAsCopyFiles( zVIEW    vSubtask,
                              zVIEW    vTZZOLODO,
                              zVIEW    vSaveAs );

static zSHORT
zwfnTZZOLODD_SaveAsGetFileName( zVIEW  vTaskLPLR,
                                zVIEW  vView,
                                zPCHAR szAttribute,
                                zPCHAR szSourceFileName );

static zSHORT
zwfnTZZOLODD_SaveAsSourceFiles( zVIEW   vSubtask,
                                zVIEW   vNewLOD,
                                zVIEW   vOldLOD );

static zSHORT
zwfnTZZOLODD_SaveAsAssignOperation( zVIEW   vNewLOD,
                                    zVIEW   vOldLOD );

static zSHORT
zwfnTZZOLODD_AddAllER_Attribs( zVIEW vSubtask,
                               zVIEW vTZZOLODO,
                               zVIEW vTZZOLOD1,
                               zVIEW vTZEREMDO_REF );

static zSHORT
zwfnTZZOLODD_ATTMAINT_SetCtrlState( zVIEW  vSubtask,
                                    zVIEW  vTZZOLODO,
                                    zSHORT nEnable,
                                    zSHORT IsCheckedOut );

static zSHORT
zwfnTZZOLODD_OrderBuild( zVIEW vDTE,
                         zVIEW vDTE_T,
                         zVIEW vLOD_T,
                         zVIEW vLOD_R,
                         zVIEW vWork );

static zSHORT
zwfnTZZOLODD_PrintPriorityList( zVIEW vSubtask,
                                zVIEW vLOD,
                                zVIEW vWork,
                                zLONG lMaxPriority );

static zSHORT
zwfnTZZOLODD_PriorityListGetLen( zVIEW   vWork,
                                 zPLONG  plmaxEntityNameLen,
                                 zPLONG  plmaxAttributeNameLen );

static zSHORT
zwfnTZZOLODD_SaveAsLOD( zVIEW  vSubtask,
                        zPCHAR szLOD_Name );

static zSHORT
zwfnTZZOLODD_IfPOD_Entity( zVIEW vLOD, zULONG ulZKey, zBOOL bDeleteIt );

static zSHORT
zwfnTZZOLODD_FoundPOD_Entity( zVIEW vLOD, zULONG ulZKey );

static zSHORT
zwfnTZZOLODD_DeletePOD_Entity( zVIEW vSubtask, zVIEW vTZZOLODO );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_QueryFrameSetUp( zVIEW vSubtask );

static zSHORT
zwfnTZZOLODD_Get_DTE( zVIEW vSubtask, zPVIEW  vDTE );

#define ATTR_CONVERSION 0          /* convert DerivedF to include Operation */

#define POSITION_NODE   1          /* test Node Positioning */
#define HORIZ_ORIENT    0
#define DEBUG_TRACE     1

#if POSITION_NODE
#if HORIZ_ORIENT
#define ORIENT             1
#define MAX_LEVELS        31
#define NODE_HEIGHT        9  //   5
#define NODE_WIDTH         5  //   9
#define HALF_NODE_WIDTH    2  //   4
#define X_SPACING          2  //   4
#define HALF_X_SPACING     1  //   2
#define Y_SPACING         15  //   6
#else
#define ORIENT             0
#define MAX_LEVELS        31
#define NODE_HEIGHT        5
#define NODE_WIDTH         9
#define HALF_NODE_WIDTH    4
#define X_SPACING          4
#define HALF_X_SPACING     2
#define Y_SPACING          6
#endif

zSHORT  nMaxNodeEdge[ MAX_LEVELS + 1 ]
                    = { 0 };
zSHORT  lLevel = 0;
#endif

#if DEBUG_TRACE
zCHAR  szDbg[512];
#endif

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HD_Print( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SelectConstraint( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SelectEntConstraint( zVIEW vSubtask );

zSHORT
zfnTZZOLODD_MergeLOD_WithDiagram( zVIEW vSubtask,
                                  zVIEW vTZZOLODO,
                                  zVIEW vTZZOHDGO );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OK_NewEntity( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EnableDBH_Info( zVIEW vSubtask );

#if POSITION_NODE
zOPER_EXPORT zSHORT OPERATION
oTZZOHDGO_CreateNode( zVIEW  vHDG,
                      zVIEW  vLOD,
                      zLONG  lOrientation )
{
   zSHORT  nRC;
   zSHORT  nTok;
   zSHORT  nOutText;
   zSHORT  nSkipDelBehave;
   zCHAR   szOutText[ 32 ];
   zCHAR   szMsg[ zLONG_MESSAGE_LTH + 1 ];

   CreateEntity( vHDG, "HD_Node", zPOS_AFTER );
   SetAttributeFromAttribute( vHDG, "HD_Node", "ZKey",
                              vLOD, "LOD_EntityParent", "ZKey" );
   SetAttributeFromAttribute( vHDG, "HD_Node", "Name",
                              vLOD, "LOD_EntityParent", "Name" );
   SetAttributeFromAttribute( vHDG, "HD_Node", "XML_Name",
                              vLOD, "LOD_EntityParent", "XML_Name" );
   SetAttributeFromAttribute( vHDG, "HD_Node", "Text",
                              vLOD, "LOD_EntityParent", "Name" );
   // ER_EntityRec doesn't exist on new LOD, so check before setting.
   if ( CheckExistenceOfEntity( vLOD, "ER_EntityRec" ) >= zCURSOR_SET )
      SetAttributeFromAttribute( vHDG, "HD_Node", "Purpose",
                                 vLOD, "ER_EntityRec", "Purpose" );

#if 1
   // Gig, 10/10 set outside text attrs
   SetAttributeFromString( vHDG, "HD_Node", "OutsideText1", "" );
   SetAttributeFromString( vHDG, "HD_Node", "OutsideText2", "" );
   SetAttributeFromString( vHDG, "HD_Node", "OutsideText3", "" );
   SetAttributeFromString( vHDG, "HD_Node", "OutsideText4", "" );
   SetAttributeFromString( vHDG, "HD_Node", "OutsideText5", "" );
   nOutText = 1;
   nSkipDelBehave = 0;
   if ( CompareAttributeToString( vLOD, "LOD_EntityParent", "Work", "Y" ) == 0 )
   {
      SetAttributeFromString( vHDG, "HD_Node", "OutsideText1", "Work" );
      nOutText = 2;
      nSkipDelBehave = 1;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent", "Derived", "Y" ) == 0 )
   {
      SetAttributeFromString( vHDG, "HD_Node", "OutsideText1", "Derived" );
      nOutText = 2;
      nSkipDelBehave = 1;
   }

   nTok = 0;
   szOutText[ 0 ] = 0;
   if ( CompareAttributeToString( vLOD, "LOD_EntityParent", "Create", "Y" )
                                                                     == 0 )
   {
      strcpy_s( szOutText, sizeof( szOutText ), "cr" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Update", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "up" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Delete", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "de" );
      nTok++;
   }

   if ( nTok )
   {
      if ( nOutText == 1 )
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText1", szOutText );
      else
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText2", szOutText );

      nOutText++;
   }

   nTok = 0;
   szOutText[ 0 ] = 0;
   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Include", "Y" ) == 0 )
   {
      strcpy_s( szOutText, sizeof( szOutText ), "inc" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Exclude", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "exc" );
      nTok++;
   }

// if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
//                                "InclSrc", "Y" ) == 0 )
// {
//    if ( nTok )
//       strcat_s( szOutText, sizeof( szOutText ), "," );
//
//    strcat_s( szOutText, sizeof( szOutText ), "isrc" );
//    nTok++;
// }

   if ( nTok )
   {
      if ( nOutText == 1 )
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText1", szOutText );
      else
      if ( nOutText == 2 )
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText2", szOutText );
      else
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText3", szOutText );

      nOutText++;
   }

   if ( !nSkipDelBehave )
   {
      if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                    "ParentDeleteBehave", "D" ) == 0 )
      {
         strcpy_s( szOutText, sizeof( szOutText ), "pdel" );
      }
      else
      if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                    "ParentDeleteBehave", "E" ) == 0 )
      {
         strcpy_s( szOutText, sizeof( szOutText ), "pexc" );
      }
      else
      {
         strcpy_s( szOutText, sizeof( szOutText ), "pres" );
      }

      if ( nOutText == 1 )
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText1", szOutText );
      else
      if ( nOutText == 2 )
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText2", szOutText );
      else
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText3", szOutText );

      nOutText++;
   }

   nTok = 0;
   szOutText[ 0 ] = 0;
   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "CreateConstraint", "Y" ) == 0 )
   {
      strcpy_s( szOutText, sizeof( szOutText ), "c" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "DeleteConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "d" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "IncludeConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "i" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "ExcludeConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "x" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "AcceptConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "a" );
      nTok++;
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "CancelConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "n" );
      nTok++;
   }

   if ( nTok )
   {
      if ( nOutText <= 2 )
      {
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText2", "cnstrnts:" );
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText3", szOutText );
      }
      else
      if ( nOutText == 3 )
      {
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText3", "cnstrnts:" );
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText4", szOutText );
      }
      else
      {
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText4", "cnstrnts:" );
         SetAttributeFromString( vHDG, "HD_Node", "OutsideText5", szOutText );
      }
   }

#endif

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Work", "Y" ) != 0 &&
        SetCursorFirstEntity( vLOD, "ER_EntityRec", 0 ) == zCURSOR_SET )
   {
      SetAttributeFromAttribute( vHDG, "HD_Node", "DIL",
                                 vLOD, "ER_EntityRec", "Name" );
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Work", "Y" ) != 0 &&
        SetCursorFirstEntity( vLOD, "ER_RelLinkRec", 0 ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szMsg, sizeof( szMsg ), vLOD, "ER_RelLinkRec", "Name" );
      strcat_s( szMsg, sizeof( szMsg ), " (" );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), sizeof( szMsg ) - zstrlen( szMsg ),
                              vLOD, "ER_RelLinkRec", "CardMin" );
      strcat_s( szMsg, sizeof( szMsg ), "," );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), sizeof( szMsg ) - zstrlen( szMsg ),
                              vLOD, "ER_RelLinkRec", "CardMax" );
      strcat_s( szMsg, sizeof( szMsg ), ")" );
      SetAttributeFromString( vHDG, "HD_Node", "Title", szMsg );
   }

   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Create", "Y" ) == 0 )
   {
      SetAttributeFromInteger( vHDG, "HD_Node", "Color", 0 );
   }
   else
   if ( CompareAttributeToString( vLOD, "LOD_EntityParent",
                                  "Include", "Y" ) == 0 )
   {
      SetAttributeFromInteger( vHDG, "HD_Node", "Color", 1 );
   }
   else
      SetAttributeFromInteger( vHDG, "HD_Node", "Color", 2 );


   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityChild", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_EntityChild", "" ) )
   {
      SetViewToSubobject( vLOD, "LOD_EntityChild" );
      SetViewToSubobject( vHDG, "HD_ChildNode" );
      oTZZOHDGO_CreateNode( vHDG, vLOD, lOrientation );
      ResetViewFromSubobject( vLOD );
      ResetViewFromSubobject( vHDG );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZZOHDGO_CreateFromLOD( zVIEW   vSubtask,
                         zPVIEW  pvHDG,
                         zVIEW   vLOD,
                         zLONG   lOrientation )
{
   if ( GetViewByName( pvHDG, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
#if 1
      zfnTZZOLODD_MergeLOD_WithDiagram( vLOD, 0, *pvHDG );
      return( 0 );
#else
      DropObjectInstance( *pvHDG );
#endif
   }

   ActivateEmptyObjectInstance( pvHDG, "TZZOHDGO", vLOD, zSINGLE );
// SetNameForView( *pvHDG, "dbgHierDgm", vLOD, zLEVEL_TASK ); // just for dbg
   CreateEntity( *pvHDG, "HD_Diagram", zPOS_AFTER );
   SetAttributeFromInteger( *pvHDG, "HD_Diagram", "Orientation",
                            lOrientation );

#if 1   // Gig, 10/10/93
   SetAttributeFromAttribute( *pvHDG, "HD_Diagram", "PrintTitle",
                              vLOD, "LOD", "Name" );
   SetAttributeFromString( *pvHDG, "HD_Diagram", "PrintBox", "Y" );
#endif

   oTZZOHDGO_CreateNode( *pvHDG, vLOD, lOrientation );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZZOLODD_PositionNode( zVIEW  vIn,
                           zPCHAR szParent,
                           zPCHAR szChild,
                           zLONG  lOrientation )
{
#if DEBUG_TRACE
   zPCHAR  szName;
#endif

   zSHORT  nLeftEdgeStart;
   zSHORT  nPos = 0;
   zSHORT  nFirstChildPos = 0;
   zSHORT  nLastChildPos = 0;
   zSHORT  nChildren = 0;
   zSHORT  nRC;

   // Gig, this code has been moved into the HD Control
   return( 0 );

   nLeftEdgeStart = nMaxNodeEdge[ lLevel ];
   lLevel++;

   if ( nLeftEdgeStart < nMaxNodeEdge[ lLevel ] )
   {
      nLeftEdgeStart = nMaxNodeEdge[ lLevel ];
   }
   else
   {
      nMaxNodeEdge[ lLevel ] = nLeftEdgeStart;
   }

   if ( nLeftEdgeStart < nMaxNodeEdge[ lLevel + 1 ] )
   {
      nLeftEdgeStart = nMaxNodeEdge[ lLevel + 1 ];
   }

// nLeftEdgeStart -= NODE_WIDTH;

#if DEBUG_TRACE
   GetAddrForAttribute( &szName, vIn, szParent, "Name" );
   zsprintf( szDbg, "lvl=%2d %-34s     LeftEdgeStart=%d",
             lLevel, szName, nLeftEdgeStart );
   TraceLineS( "PNode.1 ", szDbg );
#endif

   if ( CompareAttributeToString( vIn, szParent, "Collapsed", "Y" ) != 0 )
   {
      for ( nRC = SetCursorFirstEntity( vIn, szChild, szParent );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vIn, szChild, szParent ) )
      {
         SetViewToSubobject( vIn, szChild );
         nPos = zwfnTZZOLODD_PositionNode( vIn, szParent, szChild,
                                           lOrientation );
         ResetViewFromSubobject( vIn );

         nChildren++;
         if ( nChildren == 1 )
            nFirstChildPos = nPos;

         nLastChildPos = nPos;
      }
   }

#if DEBUG_TRACE
   zsprintf( szDbg, "lvl=%2d %-34s has %d chidren: first=%d, last=%d",
             lLevel, szName, nChildren,
             nFirstChildPos, nLastChildPos );
   TraceLineS( "PNode.. ", szDbg );
#endif
   if ( nChildren == 0 )
   {
      nPos = nLeftEdgeStart;
   }
   else
   {
      nPos = nFirstChildPos;
      nPos += ( nLastChildPos - nFirstChildPos + NODE_WIDTH ) / 2;
      nPos -= HALF_NODE_WIDTH;
   }

   if ( lOrientation == 0 )
   {
      SetAttributeFromInteger( vIn, szParent, "PosX", nPos + 2 );
      SetAttributeFromInteger( vIn, szParent, "PosY",
                               lLevel * (NODE_HEIGHT + Y_SPACING) );
   }
   else
   {
      SetAttributeFromInteger( vIn, szParent, "PosY", nPos + 2 );
      SetAttributeFromInteger( vIn, szParent, "PosX",
                               lLevel * (NODE_HEIGHT + Y_SPACING) );
   }

   nMaxNodeEdge[ lLevel ] = nPos + NODE_WIDTH + X_SPACING;
#if DEBUG_TRACE
   zsprintf( szDbg, "lvl=%2d %-34s at %d   with MaxEdge=%d",
             lLevel, szName, nPos, nMaxNodeEdge[ lLevel ] );
   TraceLineS( "PNode.9 ", szDbg );
#endif
   lLevel--;
   return( nPos );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_GraphicToLOD
// PURPOSE:    This Entry retrieves the ZKey from the Hier Graph and
//             positions on the appropriate LodEntity in the lod
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_GraphicToLOD( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zVIEW vTZZOHDGO;

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK )
                                                   == zLEVEL_TASK &&
        GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK )
                                                   == zLEVEL_TASK )
   {
      while ( ResetViewFromSubobject( vTZZOLODO ) == 0 )
      {
      }

      SetEntityCursor( vTZZOLODO, "LOD_Entity", "ZKey",
                       zPOS_FIRST | zQUAL_ENTITYATTR,
                       vTZZOHDGO, "HD_Node", "ZKey", 0, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: ofnTZZOHDGO_GraphicFromZKey
// PURPOSE:    This Entry retrieves resets the Hier Graph postion and
//             select state based on incoming ZKey
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZZOHDGO_GraphicFromZKey( zVIEW   vIn,
                             zPCHAR szParent,
                             zPCHAR szChild,
                             zLONG  lZKey )
{
   zSHORT nRC;

   // see if current node is the right one
   if ( CompareAttributeToInteger( vIn, szParent, "ZKey", lZKey ) == 0 )
   {
      SetSelectStateOfEntity( vIn, szParent, 1 );
      return( 1 );
   }

   // for each child node, call this routine
   for ( nRC = SetCursorFirstEntity( vIn, szChild, szParent );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vIn, szChild, szParent ) )
   {
      SetViewToSubobject( vIn, szChild );
      nRC = ofnTZZOHDGO_GraphicFromZKey( vIn, szParent, szChild, lZKey );
      if ( nRC == 1 )
         return( 1 );

      ResetViewFromSubobject( vIn );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: oTZZOHDGO_GraphicFromZKey
// PURPOSE:    This Entry retrieves resets the Hier Graph postion and
//             select state based on incoming ZKey
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOHDGO_GraphicFromZKey( zVIEW vSubtask,
                           zVIEW vIn,
                           zLONG lZKey )
{
   zVIEW vTZZOHDGO;

   vTZZOHDGO = vIn;
   if ( vIn == 0 )
      GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );

   if ( vTZZOHDGO )
   {
      // see if current node is the right one
      if ( CompareAttributeToInteger( vTZZOHDGO, "HD_Node",
                                      "ZKey", lZKey ) == 0 )
      {
         SetSelectStateOfEntity( vTZZOHDGO, "HD_Node", 1 );
         return( 0 );
      }
      else
         SetSelectStateOfEntity( vTZZOHDGO, "HD_Node", 0 );

      // go to top of Hier Diagram
      while ( ResetViewFromSubobject( vTZZOHDGO ) == 0 )
      {
      }

      // call routine to check top node and the recursively process child
      // nodes if need be.
      ofnTZZOHDGO_GraphicFromZKey( vTZZOHDGO, "HD_Node",
                                   "HD_ChildNode", lZKey );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_UnselectAllNodes
// PURPOSE:    This Entry retrieves the current (only possible selected)
//             node from the Hier Graph and unselects it.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_UnselectAllNodes( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zVIEW vTZZOLODO;
   zVIEW vEntityList;
   zVIEW vAttributeList;
   zBOOL  bReadOnly = FALSE;

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK )
                                                   == zLEVEL_TASK )
   {
      GetSubtaskForWindowName( vSubtask, &vEntityList, "ENTVIEW" );
      GetSubtaskForWindowName( vSubtask, &vAttributeList, "ATTVIEW" );
      if ( !vEntityList && !vAttributeList )
      {
         SetSelectStateOfEntity( vTZZOHDGO, "HD_Node", 0 );
         if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0 )
            SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 0 );

         // Unset the readonly state, because RefreshAllSubwindows triggers an
         // update (because of xPos/yPos of vTZZOHDGO):
         if ( MiGetUpdateForView( vTZZOHDGO ) == 0 )
         {
            bReadOnly = TRUE;
            SetViewUpdate( vTZZOHDGO );
         }

         RefreshWindow( vSubtask );
         if ( bReadOnly == TRUE )
            SetViewReadOnly( vTZZOHDGO );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_InitializeDialog
// PURPOSE:    This Entry prepares the environment upon entry
//             to the LOD Dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_InitializeDialog( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  wXferO;
   zVIEW  vTZZOLODO;
   zVIEW  vHierDiag;
   zVIEW  vXLP;
   zPCHAR szLPLR_Name;
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ 1000 ];
   zSHORT nRC;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "ZO", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // choose the proper LPLR to Init
   if ( CompareAttributeToString( vProfileXFER, "ZO", "StartupLPLR_Option",
                                  "D" ) == 0 )   //default LPLR
   {
      InitializeLPLR( vSubtask, "" );
   }
   else
   {
      GetAddrForAttribute( &szLPLR_Name,
                           vProfileXFER, "ZO", "StartupLPLR_Name" );
      InitializeLPLR( vSubtask, szLPLR_Name );
   }

   // If this request is for a LOD already determined in wXferO object, activate it.
   nRC = GetViewByName( &wXferO, "wXferO", vSubtask, zLEVEL_SYSTEM );
   if ( nRC >= 0 )
   {
      GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), wXferO, "QueryValues", "QueryObjectName" );
      if ( szLOD_Name[ 0 ] != 0 )
      {
         GetViewByName( &vXLP, "TaskLPLR", vSubtask, zLEVEL_TASK );
         GetStringFromAttribute( szFileName, sizeof( szFileName ), vXLP, "LPLR", "MetaSrcDir" );
         ofnTZCMWKSO_AppendSlash( szFileName );
         ZeidonStringConcat( szFileName, 1, 0, szLOD_Name, 1, 0, 1000 );
         ZeidonStringConcat( szFileName, 1, 0, ".LOD", 1, 0, 1000 );
         nRC = ActivateOI_FromFile( &vTZZOLODO, "TZZOLODO", vSubtask,
               szFileName, zSINGLE );
         SetNameForView( vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
         oTZZOHDGO_CreateFromLOD( vSubtask, &vHierDiag, vTZZOLODO, ORIENT );
                                                         // 1 Fern; 0 Hier
         if ( vHierDiag )
         {
            SetNameForView( vHierDiag, "TZZOHDGO", vSubtask, zLEVEL_TASK );
            lLevel = 0;
            zmemset( (zPCHAR) &nMaxNodeEdge[ 0 ], 0, sizeof( nMaxNodeEdge ) );
            zwfnTZZOLODD_PositionNode( vHierDiag, "HD_Node",
                                       "HD_ChildNode", ORIENT );
         }
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZZOLODQ", "QueryViewDisplay" );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DetermineDialogEntry
// PURPOSE:    This Entry prepares the environment upon entry
//             to the LOD Dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DetermineDialogEntry( zVIEW vSubtask )
{
   zSHORT   nRC;
   zVIEW    vT = NULL;
   zVIEW    vTZZOLODO;
   DWORD    dwColors[ 4 ];

   // Color 0 is Green     (Creatable)
   // Color 1 is Yellow    (Includeable)
   // Color 2 is Lt. Gray  (Includable Path)
   // Color 3 is terminator
   dwColors[ 0 ] = RGB( 0, 255, 0 );
   dwColors[ 1 ] = RGB( 255, 255, 0 );
   dwColors[ 2 ] = RGB( 192, 192, 192 );
   dwColors[ 3 ] = 0;
   HD_SetColors( vSubtask, "HierDiag1", dwColors, 0, 0, 0, 0, 0, 1 );

   // Get Access to LOD Object
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vT, zREFER_LOD_META );
      if ( vT )
      {
         SetNameForView( vT, "TZZOLFLO", vSubtask, zLEVEL_TASK );

         // XFER to New or Open Window
      // if ( SetCursorFirstEntity( vT, "W_MetaDef", "" ) < zCURSOR_SET )
         if ( CheckExistenceOfEntity( vT, "W_MetaDef" ) != 0 )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                     "TZZOLODQ", "TZZONWLD" );
         }
         else
         {
            SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                     "TZZOLODQ", "TZZOOPLD" );
         }
      }
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Query View Maintenance", "TZZOLODO",
                               vTZZOLODO, "LOD", zSOURCE_LOD_META );

   // Set Name for View to current Window (for displaying the
   // status DIL message out of the OpenLOD window.
   SetNameForView( vSubtask, "TZZOLODD", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_PostBuildQueryView
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildQueryView( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;
   DWORD    dwColors[ 4 ];

   // Color 0 is Green     (Creatable)
   // Color 1 is Yellow    (Includeable)
   // Color 2 is Lt. Gray  (Includable Path)
   // Color 3 is terminator
   dwColors[ 0 ] = RGB( 0, 255, 0 );
   dwColors[ 1 ] = RGB( 255, 255, 0 );
   dwColors[ 2 ] = RGB( 192, 192, 192 );
   dwColors[ 3 ] = 0;
   HD_SetColors( vSubtask, "HierDiag1", dwColors, 0, 0, 0, 0, 0, 1 );

   // Set position of the Hier Diagram to current Entity
   zwTZZOLODD_RefreshEntitySelect( vSubtask );

   // Automatically bring up Attribute list subwindow.
   SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow,
                            "TZZOLODQ", "QueryViewATTVIEW" );

   // Set window Title with check out state
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   SetTitleWithCheckOutState( vSubtask, "Query View - ", "TZZOLODO",
                               vTZZOLODO, "LOD", zSOURCE_LOD_META );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_InitMenuForEdit
//
// enable or disable menu items
/////////////////////////////////////////////////////////////////////////////
zSHORT /* LOCAL */  OPERATION
zwTZZOLODD_InitMenuForEdit( zVIEW  vSubtask,
                            zSHORT nIsCheckedOut,
                            zSHORT nShowMenuItemForUpdate )
{
   zVIEW    vTZZOHDGO;
   zVIEW    vTZZOLODO;
   zVIEW    vTZZOLODO1 = 0;
   zSHORT   nRC;

   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
      CreateViewFromViewForTask( &vTZZOLODO1, vTZZOLODO, 0 );

   //BL: if Entity selected, then enable menu items for add Attribute
   // and Entity.
   if ( (GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0) &&
        (GetSelectStateOfEntity( vTZZOHDGO, "HD_Node" ) == 1) )
   {
      SetOptionState( vSubtask, "UpdateEntity", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
      {
         if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) != 0 )
         {
            SetOptionState( vSubtask, "AddAllAttributes", zOPTION_STATUS_ENABLED, 1 );
            SetOptionState( vSubtask, "AddSelectedAttributes", zOPTION_STATUS_ENABLED, 1 );
         }
         if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
         {
            SetOptionState( vSubtask, "DeleteAllAttributes", zOPTION_STATUS_ENABLED, 1 );
         }
         SetOptionState( vSubtask, "AddRelatedEntity", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "AddDerivedEntity", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "AddWorkEntity", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "DeleteSelectedEntity", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "AddWorkAttributes", zOPTION_STATUS_ENABLED, 1 );
      }
   }
   // If Attribute selected (in Entity or Attribute List), then enable menu
   // items for update Attribute.
   if ( (nShowMenuItemForUpdate == 1) &&
        (GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0) )
   {
      nRC = SetCursorFirstSelectedEntity( vTZZOLODO1, "LOD_Attribute", 0 );
      if ( nRC == zCURSOR_SET )
      {
         SetCursorFirstEntityByEntityCsr( vTZZOLODO, "LOD_Attribute",
                                          vTZZOLODO1, "LOD_Attribute", 0 );
         SetOptionState( vSubtask, "UpdateAttribute", zOPTION_STATUS_ENABLED,
                         nShowMenuItemForUpdate );
         if ( nIsCheckedOut == 1 )
            SetOptionState( vSubtask, "DeleteSelectedAttributes",
                            zOPTION_STATUS_ENABLED, nShowMenuItemForUpdate );
      }
   }

   return( 0 );
} // zwTZZOLODD_InitMenuForEdit


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_InitMenuForView
//
//    enable or disable menu items
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZZOLODD_InitMenuForView( zVIEW   vSubtask,
                            zSHORT  nIsCheckedOut,
                            zPSHORT nShowMenuItemForUpdate )
{
   zVIEW    vT;
   zVIEW    vTZOPRUSO;

   //Entity Attribute List
   GetSubtaskForWindowName( vSubtask, &vT, "ATTVIEW" );
   if ( vT )
   {
      SetOptionState( vSubtask, "ViewAttribute", zCONTROL_STATUS_CHECKED, 1 );
      *nShowMenuItemForUpdate = 1;
   }
   else
   {
      SetOptionState( vSubtask, "ViewAttribute", zCONTROL_STATUS_CHECKED, 0 );
   }

   // LOD Entity List
   GetSubtaskForWindowName( vSubtask, &vT, "ENTVIEW" );
   if ( vT )
   {
      SetOptionState( vSubtask, "ViewEntitys", zCONTROL_STATUS_CHECKED, 1 );
      *nShowMenuItemForUpdate = 1;
   }
   else
   {
      SetOptionState( vSubtask, "ViewEntitys", zCONTROL_STATUS_CHECKED, 0 );
   }

   //BL: 1999.10.11 - Source File List
   GetSubtaskForWindowName( vSubtask, &vT, "SRCLIST" );
   if ( vT )
   {
      SetOptionState( vSubtask, "SourceFileList", zCONTROL_STATUS_CHECKED, 1 );
      if ( (GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0) &&
           (GetSelectStateOfEntity( vTZOPRUSO, "SourceFile" ) == 1) )
      {
         SetOptionState( vSubtask, "UpdateSourceFile", zOPTION_STATUS_ENABLED, 1 );
         if ( nIsCheckedOut == 1 )
            SetOptionState( vSubtask, "DeleteSelectedSourceFiles",
                            zOPTION_STATUS_ENABLED, 1 );
      }
   }
   else
   {
      SetOptionState( vSubtask, "SourceFileList", zCONTROL_STATUS_CHECKED, 0 );
   }

   // Ordering List For Activate
   if ( GetViewByName( &vT, "TZZOLODO", vSubtask, zLEVEL_TASK ) >= 0 )
      SetOptionState( vSubtask, "OrderingListForActivate", zOPTION_STATUS_ENABLED, 1 );

   return( 0 );
} // zwTZZOLODD_InitMenuForView


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_InitMenuDisableAll
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZZOLODD_InitMenuDisableAll( zVIEW vSubtask )
{
   zVIEW vLPLR;
   zBOOL bEnable = TRUE;

   /*SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SaveWithoutDatabase", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddRelatedEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddDerivedEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddWorkEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddAllAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddSelectedAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddWorkAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddSourceFile", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddRootEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateAttribute", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateSourceFile", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedEntity", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedSourceFiles", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSelectedOperations", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllAttributes", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllSorceFiles", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllOperations", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SetActivateOrdering", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "MoveEntityLeft", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "MoveEntityRight", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "MoveOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EditOperationscode", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "OrderingListForActivate", zOPTION_STATUS_ENABLED, 0 );*/

   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      bEnable = FALSE;

   SetOptionState( vSubtask, "New", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Open", zOPTION_STATUS_ENABLED, bEnable );

   return( 0 );
} // zwTZZOLODD_InitMenuDisableAll



/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_InitMenu
// PURPOSE:    This Entry grays the items on the Menu
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_InitMenu( zVIEW vSubtask )
{
   zVIEW    vT;
   zVIEW    vTZOPRUSO;
   zVIEW    vTZZOLODO;
   zSHORT   nShowMenuItemForUpdate;
   zSHORT   nIsCheckedOut;

   zwTZZOLODD_InitMenuDisableAll( vSubtask );

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO,
                                          zSOURCE_LOD_META );

   if ( nIsCheckedOut == 1 )
   {
      // if LOD not checked out, disable menu items for save
      /*SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "SaveWithoutDatabase", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddRootEntity", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddSourceFile", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddOperation", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "SetActivateOrdering", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "MoveEntityLeft", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "MoveEntityRight", zOPTION_STATUS_ENABLED, 1 );*/
   }
   else
      TraceLineS( "(DMUP) ", "view is read only" );

   nShowMenuItemForUpdate = 0;

   zwTZZOLODD_InitMenuForView( vSubtask, nIsCheckedOut, &nShowMenuItemForUpdate );

   if ( GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 &&
        nIsCheckedOut == 1 )
   {
      if ( CheckExistenceOfEntity( vTZOPRUSO, "SourceFile" ) >= zCURSOR_SET )
         SetOptionState( vSubtask, "DeleteAllSorceFiles", zOPTION_STATUS_ENABLED, 1 );
      if ( CheckExistenceOfEntity( vTZOPRUSO, "OperationList" ) >= zCURSOR_SET )
         SetOptionState( vSubtask, "DeleteAllOperations", zOPTION_STATUS_ENABLED, 1 );
   }

   //BL: 1999.10.11 - Operation List
   GetSubtaskForWindowName( vSubtask, &vT, "OPERLIST" );
   if ( vT )
   {
      SetOptionState( vSubtask, "OperationList", zCONTROL_STATUS_CHECKED, 1 );
      if ( GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( GetSelectStateOfEntity( vTZOPRUSO, "OperationList" ) == 1 )
         {
            SetOptionState( vSubtask, "UpdateOperation", zOPTION_STATUS_ENABLED, 1 );
            SetOptionState( vSubtask, "EditOperationscode", zOPTION_STATUS_ENABLED, 1 );
            if ( nIsCheckedOut == 1 )
            {
               SetOptionState( vSubtask, "MoveOperation", zOPTION_STATUS_ENABLED, 1 );
               SetOptionState( vSubtask, "DeleteSelectedOperations",
                               zOPTION_STATUS_ENABLED, 1 );
            }
         }
      }
   }
   else
   {
      SetOptionState( vSubtask, "OperationList", zCONTROL_STATUS_CHECKED, 0 );
   }

   //BL, 1999.10.22
   zwTZZOLODD_InitMenuForEdit( vSubtask, nIsCheckedOut, nShowMenuItemForUpdate );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_MainRtnFromSubWnd
// PURPOSE:    This Entry re-loads a LOD if a SwitchLPLR has invalidated
//             the current LOD.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZZOHDGO;
   zVIEW  vW;
   zSHORT nRC;
   zBOOL  bReadOnly = FALSE;

   // Refresh Attribute, Operation subwindows
   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) < 1 &&
        GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropObjectInstance( vTZZOHDGO );
   }

   zwTZZOLODD_GraphicToLOD( vSubtask );

   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      //BL, 1999.10.12 set View for Operation and Source File List
      SetNameForView( vTZZOLODO, "TZOPRUSO", vSubtask, zLEVEL_TASK );
      SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 1 );

      if ( !HD_IsNodeVisible( vSubtask, "HierDiag1" ) )
         HD_CenterNode( vSubtask, "HierDiag1" );

      // Refresh the window if it exists.
      if ( GetSubtaskForWindowName( vSubtask, &vW, "OPERLIST" ) >= 0 )
         //rebuild Operation List
         wTZOPRUSD_SetUpTZOPRUSO_View( vSubtask );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Query View Maintenance", "TZZOLODO",
                              vTZZOLODO, "LOD", zSOURCE_LOD_META );

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetSelectStateOfEntity( vTZZOHDGO, "HD_Node", 1 );

      // Unset the readonly state, because RefreshAllSubwindows triggers an
      // update (because of xPos/yPos of vTZZOHDGO):
      if ( MiGetUpdateForView( vTZZOHDGO ) == 0 )
      {
         bReadOnly = TRUE;
         SetViewUpdate( vTZZOHDGO );
      }

      RefreshAllSubwindows( vSubtask );

      if ( bReadOnly == TRUE )
         SetViewReadOnly( vTZZOHDGO );
   }

   GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
   if ( vW )
      HD_SelectNode( vW, "HierDiag1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_GrRefreshAttSAWnd
// PURPOSE:    This Entry refreshes the Attribute and
//             Subject Area views in the LOD Dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_GrRefreshAttSAWnd( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vW;
   zPCHAR szName;
   zPCHAR pDesc;
   zPCHAR pMsg;
   zPCHAR pDescMax;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   zwTZZOLODD_GraphicToLOD( vSubtask );

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
   if ( !HD_IsNodeVisible( vW, "HierDiag1" ) )
      HD_CenterNode( vW, "HierDiag1" );

   GetAddrForAttribute( &szName, vTZZOLODO, "LOD_Entity", "Name" );
   strcpy( szMsg, szName);
   strcat( szMsg, " :   " );

   GetAddrForAttribute( &pDesc, vTZZOLODO, "LOD_Entity", "Desc" );

   // Concat characters from szDesc until the first null or NewLine or
   // line feed or to a maximum of 60 characters.
   pMsg  = szMsg + zstrlen( szMsg );
   pDescMax = pDesc + 60;
   while ( *pDesc != 0 && *pDesc != '\n' && *pDesc != '\r' && pDesc < pDescMax )
   {
      *pMsg++ = *pDesc++;
   }

   *pMsg = 0;

   MB_SetMessage( vSubtask, 1, szMsg );
   GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" );
   if ( vW )
      RefreshWindow( vW );

   GetSubtaskForWindowName( vSubtask, &vW, "QueryViewATTVIEW" );
   if ( vW )
      RefreshWindow( vW );

   GetSubtaskForWindowName( vSubtask, &vW, "EntityDescription" );
   if ( vW )
      RefreshWindow( vW );

   GetSubtaskForWindowName( vSubtask, &vW, "SARELVIEW" );
   if ( vW )
      RefreshWindow( vW );

   GetSubtaskForWindowName( vSubtask, &vW, "SAENTVIEW" );
   if ( vW )
      RefreshWindow( vW );

   GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" );
   if ( vW )
   {
      RefreshWindow( vW );
      SetWindowCaptionTitle( vW, 0, szName );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_RefreshWndWithTitle
// PURPOSE:    This Entry sets the CaptionTitle to the LOD Entity Name and
//             then returns for a full window refresh.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshWndWithTitle( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zCHAR  szCaption[ 60 ];

//  Refresh and Set Caption for Attribute View Window
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( vTZZOLODO )
   {
      GetStringFromAttribute( szCaption, sizeof( szCaption ), vTZZOLODO, "LOD_Entity", "Name" );
      strcat_s( szCaption, sizeof( szCaption ), " Attributes" );
      SetWindowCaptionTitle( vSubtask, 0, szCaption );
   }

   RefreshCtrl( vSubtask, "lbAttributeList" );
   return( 1 );  // 1 says don't continue refresh, I've done it all
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_PostBuildER_ENTVIEW
// PURPOSE:    This Entry sets the caption for the Derived Entity Window
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildER_ENTVIEW( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZZOLODO;
   zVIEW vT;
   zSHORT nRC;
   zCHAR szName[ 33 ];

   //  Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //  Get Access to ER Object,
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      nRC = RetrieveViewForMetaList( vSubtask, &vT, zREFER_ERD_META );
      if ( ActivateMetaOI( vSubtask, &vTZEREMDO, vT, zREFER_ERD_META,
                           zSINGLE | zLEVEL_APPLICATION ) >= 0 )
      {
         SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      }

      DropView( vT );
   }

   //  Set Window Caption with Entity Name
   GetStringFromAttribute( szName, sizeof( szName ), vTZZOLODO, "LOD_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szName );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_LoadLODList
// PURPOSE:   This function Loads a File List of LOD's for the Open Window
// TRIGGERED BY:
//    WINDOW: TZZOOPMD
//    ACTION: WINDOW
//    NAME: WindowAttribute
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_LoadLODList( zVIEW vSubtask )
{
   zVIEW    vTZLODS;
   zVIEW    vTZZOLODO;
   zCHAR    szLodName[33];
   zSHORT   nRC;

   // Obtain list of LOD's and position on the current LOD if there is
   // one active.
   if ( GetViewByName( &vTZLODS, "TZZOLFLO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vTZLODS, zREFER_LOD_META );
      SetNameForView( vTZLODS, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   }

   // Create query entries as only those that start with a "q".
   ResetViewFromSubobject( vTZLODS );
   CreateEntity( vTZLODS, "QueryW_MetaType", zPOS_AFTER );
   for ( nRC = SetCursorFirstEntity( vTZLODS, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZLODS, "W_MetaDef", "" ) )
   {
      GetStringFromAttribute( szLodName, sizeof( szLodName ), vTZLODS, "W_MetaDef", "Name" );
      if ( *szLodName == 'q' )
      {
         IncludeSubobjectFromSubobject( vTZLODS, "QueryW_MetaDef",
                                        vTZLODS, "W_MetaDef", zPOS_AFTER );
      }
   }

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZZOLODO, "LOD" ) >= zCURSOR_SET )
   {
      // If LOD open, then set cursor to it.
      SetCursorFirstEntityByAttr( vTZLODS, "QueryW_MetaDef", "Name",
                                  vTZZOLODO, "LOD", "Name", "" );
   }
   else
   {
      OrderEntityForView( vTZLODS, "QueryW_MetaDef", "LastUpdateDate D" );
      SetCursorFirstEntity( vTZLODS, "QueryW_MetaDef", "" );
   }

// MessageBox( 0, "Check cursor position of TZZOLFLO", "TZZOLODD.C", MB_OK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AskForSave
// PURPOSE:   This function asks and does a save of the LOD
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: Menu Item
//    NAME: Open
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZZOLODD_AskForSave( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;
   zSHORT   nRC;
   zBOOL    nSaveAs = FALSE;
   zCHAR    szLodName[33];
   zCHAR    szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Get Access to LOD Object
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( nRC <= 0 )
      return( -1 );

   if ( ObjectInstanceUpdatedFromFile( vTZZOLODO ) == 1 )
   {
      // if LOD not checked out, then ask for save as
      if ( !ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ) )
      {
         nSaveAs = TRUE;
         GetStringFromAttribute( szLodName, sizeof( szLodName ), vTZZOLODO, "LOD", "Name" );
         strcpy_s( szMsg, sizeof( szMsg ), "LOD is not checked out, but LOD '" );
         strcat_s( szMsg, sizeof( szMsg ), szLodName );
         strcat_s( szMsg, sizeof( szMsg ), "' has been modified. \n\nWould you like to " );
         strcat_s( szMsg, sizeof( szMsg ), "save it with differend name?" );
         nRC = MessagePrompt( vSubtask, "DM00115", "Zeidon Objects Maintenance",
                              szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_YES, zICON_QUESTION );
      }
      else
         nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_LOD,
                             zREFER_LOD_META,
                             vTZZOLODO, "LOD", "Name", 0 );

      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "TZZOLODD", "TZZOLODG" );
         return( nRC );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            // if LOD not checked out and user would save it, then call
            // window save LOD as
            if ( nSaveAs )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                        "TZZOLODQ", "LOD_SaveAs" );
               return( 99 );
            }

            // Save current LOD to a file along with the XOD
            nRC = zwTZZOLODD_SaveLOD( vSubtask );
            if ( nRC < 0 )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
               return( nRC );
            }
         }
         else
            return( 0 );
      } //endif ( nRC == zRESPONSE_CANCEL )
   }
   else // no update
      nRC = 0;

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AskForSave( zVIEW vSubtask )
{
   zwfnTZZOLODD_AskForSave( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_DisplaySaveStatus
// PURPOSE:    Display status in the DIL-Line (saved with TE .../without TE ..
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DisplaySaveStatus( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zCHAR  szTE_Name[ 33 ];
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH +1 ];

   // Get Access to LOD Object
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   nRC = CheckExistenceOfEntity( vTZZOLODO, "POD" );
   strcpy(szMsg, "Status: LOD is saved with" );
   if (  nRC >= zCURSOR_SET )
   {
      strcat( szMsg, " TE: " );
      nRC = SetCursorFirstEntityByAttr( vTZZOLODO, "TE_DBMS_Source", "ZKey",
                                        vTZZOLODO, "POD", "TE_SourceZKey",
                                        "TE_DB_Environ" );
      if ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttribute( szTE_Name, sizeof( szTE_Name ), vTZZOLODO, "TE_DBMS_Source", "Name");
         strcat( szMsg, szTE_Name );
      }
      else
         strcpy(szMsg, "Status: LOD is saved without TE." );
   }
   else
      strcat( szMsg, "out TE" );

   strcat( szMsg, "." );
   MB_SetMessage( vSubtask, 1, szMsg );
   return( 0 );
}


static zSHORT
zwfnTZZOLODD_OpenLOD( zVIEW vSubtask )
{
   zVIEW       vTZZOLODO;
   zVIEW       vHierDiag;
   zVIEW       vMainWindow;
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   // set the DIL-Message in the ParentWinow
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vHierDiag, "TZZOHDGO", vSubtask, zLEVEL_TASK );

   zwTZZOLODD_DisplaySaveStatus( vTZZOLODO );

   // if attribute sequence does not exist, set it and remove update flag
   oTZZOLODO_SetAttributeSequence( vTZZOLODO );
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOLODO->hViewCsr );
   lpViewOI  = (LPVIEWOI)SysGetPointerFromHandle( lpViewCsr->hViewOI );
   lpViewOI->bUpdatedFile = FALSE;

   GetSubtaskForWindowName( vSubtask, &vMainWindow, "TZZOLODG" );

   // if LOD not checked out, set  View read only ->
   // the user cannot update the values in Detail Windows
   if ( !ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ))
   {
      SetViewReadOnly( vTZZOLODO );
      SetViewReadOnly( vHierDiag );
      EnableAction( vMainWindow, "DeleteEntity", FALSE );
   }
   else
      EnableAction( vMainWindow, "DeleteEntity", TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_OpenLOD
// PURPOSE:   This function activates a LOD selected from the list
//            of LOD's.  It will optionally refresh the LOD if the
//            ER Model has been updated.
// TRIGGERED BY:
//    WINDOW: TZLODOPMD
//    ACTION: BUTTON
//    NAME: Open
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OpenLOD( zVIEW vSubtask )
{
   zCHAR       szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zVIEW       vTZZOLODO;
   zVIEW       vTZZOLFLO;
   zVIEW       vTZZOMFLO;
   zVIEW       vTZEREMDO_REF;
   zVIEW       vHierDiag;
   zSHORT      nRC;

   // Drop View to existing LOD.
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
      nRC = DropMetaOI( vSubtask, vTZZOLODO );

   // Get Access to LOD File List Object.
   nRC = GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "OpenLOD unable to access LOD file list. Open terminated." );
      MessageSend( vSubtask, "ZO00106", "Zeidon Object Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   SetCursorFirstEntityByEntityCsr( vTZZOLFLO, "W_MetaDef", vTZZOLFLO, "QueryW_MetaDef", "" );

   // Get Access to LOD Object.
   nRC = ActivateMetaOI( vSubtask, &vTZZOLODO, vTZZOLFLO, zSOURCE_LOD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
      return( nRC );

#if ATTR_CONVERSION
   zwfnTZZOLODD_AttrConversion( vTZZOLODO );
#endif

#if POSITION_NODE
   if ( GetViewByName( &vHierDiag, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vHierDiag );

   oTZZOHDGO_CreateFromLOD( vSubtask, &vHierDiag, vTZZOLODO, ORIENT );
                                                   // 1 Fern; 0 Hier
   if ( vHierDiag )
   {
      SetNameForView( vHierDiag, "TZZOHDGO", vSubtask, zLEVEL_TASK );
      lLevel = 0;
      zmemset( (zPCHAR) &nMaxNodeEdge[ 0 ], 0, sizeof( nMaxNodeEdge ) );
      zwfnTZZOLODD_PositionNode( vHierDiag, "HD_Node",
                                 "HD_ChildNode", ORIENT );
   }
#endif

   SetNameForView( vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Get Access to ER Object
   RetrieveViewForMetaList( vSubtask, &vTZZOMFLO, zREFER_ERD_META );
   if ( CheckExistenceOfEntity( vTZZOMFLO, "W_MetaDef" ) == zCURSOR_SET )
   {
      oTZEREMDO_GetRefViewForER( vSubtask, &vTZEREMDO_REF, zCURRENT_OI );
      SetNameForView( vTZEREMDO_REF, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
   }

   // set DIL-Message, set attribute sequence
   // if LOD not checked out, set View read only
   zwfnTZZOLODD_OpenLOD( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_CreateNewLOD
// PURPOSE:   Create a new instance of a LOD by building a skeleton
//            definition with a an initial Root.
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_New...
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CreateNewLOD( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZZOLODO_New;
   zVIEW  vTZZOLFLO;
   zVIEW  vDialogLPLR;
   zVIEW  vHierDiag;
   zVIEW  vProfileXFER;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szInName[ 120 ];
   zCHAR  szOutName[ 33 ];


   // A name must be entered.
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   // Validate LOD Name is OK
   GetStringFromAttribute( szInName, sizeof( szInName ), vProfileXFER, "TZ", "Name" );
   UfCompressName( szInName, szOutName, 32, "", "", "", "", 0 );
   SetAttributeFromString( vProfileXFER, "TZ", "Name", szOutName );

   if ( CompareAttributeToString( vProfileXFER, "TZ", "Name", "" ) == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "LOD Name is required." );
      MessageSend( vSubtask, "ZO00107", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -2 );
   }

   // There must be no other LOD with same name.
   if ( GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vTZZOLFLO, zREFER_LOD_META );
      SetNameForView( vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   }
   if ( SetCursorFirstEntityByString( vTZZOLFLO,
                                      "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "A LOD by the same name already exists." );
      MessageSend( vSubtask, "ZO00108", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -2 );
   }

   // There must be no Dialog with same name.
   RetrieveViewForMetaList( vSubtask, &vDialogLPLR, zREFER_DIALOG_META );
   if ( SetCursorFirstEntityByString( vDialogLPLR,
                                      "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "A Dialog with the same name exists." );
      MessageSend( vSubtask, "ZO00165", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -2 );
   }

   // Activate an empty LOD Object
   nRC = ActivateEmptyMetaOI( vSubtask, &vTZZOLODO_New, zSOURCE_LOD_META,
                              zSINGLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwTZZOLODD_CreateNewLOD) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00109", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   // Drop View to existing LOD
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( vTZZOLODO )
      DropMetaOI( vSubtask, vTZZOLODO );

   SetNameForView( vTZZOLODO_New, "TZZOLODO", vSubtask, zLEVEL_TASK );
   vTZZOLODO = vTZZOLODO_New;

   if ( GetViewByName( &vHierDiag, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vHierDiag );

   // Create a skeleton LOD Object
   nRC = CreateMetaEntity( vSubtask, vTZZOLODO, "LOD", zPOS_AFTER );
   SetAttributeFromAttribute( vTZZOLODO, "LOD", "Name",
                               vProfileXFER, "TZ", "Name" );
   SetAttributeFromAttribute( vTZZOLODO, "LOD", "Desc",
                               vProfileXFER, "ZO", "NewDesc" );

   // Create a skeleton LOD Root Entity on the Recursive Side
   nRC = CreateMetaEntity( vSubtask, vTZZOLODO, "LOD_EntityParent", zPOS_AFTER );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Name", szInName );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "IndentName", szInName );
   nRC = SetAttributeFromInteger( vTZZOLODO, "LOD_EntityParent", "IndentLvl", 0 );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Work", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Create",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Delete",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Include",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Exclude",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "InclSrc",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Recursive",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Derived",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "ParentDeleteBehave",
                                  "D" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Update",
                                  "N" );

   // Create a skeleton LOD Root Entity on the flat side
   nRC = IncludeSubobjectFromSubobject( vTZZOLODO, "LOD_Entity",
                                        vTZZOLODO, "LOD_EntityParent",
                                        zPOS_AFTER );

   nRC = SetNameForView( vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   oTZZOHDGO_CreateFromLOD( vSubtask, &vHierDiag, vTZZOLODO, ORIENT );
   if ( vHierDiag )
      SetNameForView( vHierDiag, "TZZOHDGO", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_OkNewLOD
// PURPOSE:   Update the temporary LOD from the SA Entity Selected.
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_New...
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OkNewLOD( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZZOHDGO;
   zVIEW  vTZEREMDO_REF;
   zVIEW  vProfileXFER;
   zCHAR  szNewName[ 33 ];

   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Name must start with a "q".
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "TZ", vSubtask, zCURRENT_OI );
   GetStringFromAttribute( szNewName, sizeof( szNewName ), vProfileXFER, "TZ", "Name" );
   if ( *szNewName != 'q' )
   {
      MessageSend( vSubtask, "ZO00001", "Query LOD Maintenance",
                   "The LOD name must begin with a 'q'.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Create New LOD
   if ( zwTZZOLODD_CreateNewLOD( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Get Access to LOD Object
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwTZZOLODD_OkNewLOD) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00110", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO_REF, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access ER Model. ER must be opened." );
      MessageSend( vSubtask, "ZO00111", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // The entity is always ER.
   nRC = SetAttributeFromAttribute( vTZZOLODO, "LOD_EntityParent", "Name",
                                     vTZEREMDO_REF, "ER_Entity", "Name" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_EntityParent", "Work", "N" );

   nRC = SetAttributeFromAttribute( vTZZOLODO, "LOD_EntityParent", "IndentName",
                                     vTZEREMDO_REF, "ER_Entity", "Name" );
   nRC = SetAttributeFromAttribute( vTZZOLODO, "LOD_EntityParent", "Desc",
                                     vTZEREMDO_REF, "ER_Entity", "Desc" );

   // Include the selected SA Entity under the LOD Root Entity on the
   // Recursive side.
   nRC = IncludeSubobjectFromSubobject( vTZZOLODO, "ER_EntityRec",
                                        vTZEREMDO_REF, "ER_Entity",
                                        zPOS_AFTER );

   // Build all Atttributes from the selected ER Entity under the LOD Root Entity
   while ( nRC >= zCURSOR_SET )
   {
      nRC = CheckExistenceOfEntity( vTZEREMDO_REF, "ER_Attribute" );
      if ( nRC <  zCURSOR_SET )
         break;
      nRC = CreateMetaEntity( vSubtask, vTZZOLODO, "LOD_AttributeRec", zPOS_AFTER );
      nRC = IncludeSubobjectFromSubobject( vTZZOLODO, "ER_AttributeRec",
                                           vTZEREMDO_REF, "ER_Attribute",
                                           zPOS_AFTER );
      nRC = SetCursorNextEntity( vTZEREMDO_REF, "ER_Attribute", "" );
   }

   nRC = SetNameForView( vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );
   zfnTZZOLODD_MergeLOD_WithDiagram( vSubtask, vTZZOLODO, vTZZOHDGO );

   // if attribute sequence does not exist, set it
   oTZZOLODO_SetAttributeSequence( vTZZOLODO );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_SaveLODForTE
// PURPOSE:   Implement a LOD using a particular Tech. Environment.
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_SaveTE
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveLODForTE( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZTENVRO_REF;
   zSHORT nRC;
   zCHAR  szMsg[ zMAX_MESSAGE_LTH + 1 ];

   // Get Access to LOD
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwTZZOLODD_SaveLODForTE) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00122", "Zeidon Object Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Get Access to Tech. Environment
   nRC = GetViewByName( &vTZTENVRO_REF, "TZTENVRO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
      nRC = GetViewByName( &vTZTENVRO_REF, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access Technical Env. TE must be opened." );
      MessageSend( vSubtask, "ZO00123", "Zeidon Object Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // If LOD is empty show error and return
   nRC = CheckExistenceOfEntity( vTZZOLODO, "LOD" );
   if ( nRC < zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Save empty LOD." );
      MessageSend( vSubtask, "ZO00124", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // If LOD doesn't contain POD reference, create one
   if ( CheckExistenceOfEntity( vTZZOLODO, "POD" ) < zCURSOR_SET )
      CreateMetaEntity( vSubtask, vTZZOLODO, "POD", zPOS_FIRST );

   SetAttributeFromAttribute( vTZZOLODO, "POD", "NAME",
                              vTZZOLODO, "LOD", "Name" );

   // Execute the object constraints for TZZOLODO.
   nRC = oTZZOLODO_Constraint( vSubtask, vTZZOLODO, zOCE_COMMIT, 0 );
   if ( nRC < 0 )
      return( nRC );

   // for delete all Joind Definitions (POD_Entities), where this included
   // DataSource does not exists in the TE
   //oTZZOLODO_DeleteOldPOD_Entities( vTZZOLODO, vTZTENVRO_REF );

   // Execute routine to do the save of the LOD.
   oTZZOLODO_SaveLOD_WithTE( vSubtask, vTZZOLODO, vTZTENVRO_REF );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ActivateTE( zVIEW vSubtask )
{
   zVIEW   vT;
   zVIEW   vTE;
   zSHORT  nRC;

   // Activate TE to present list of source entries, if it's not already there, and
   // position on the Access Database definition.
   nRC = GetViewByName( &vTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = GetViewByName( &vTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         RetrieveViewForMetaList( vSubtask, &vT, zREFER_DTE_META );
         nRC = ActivateMetaOI( vSubtask, &vTE, vT, zREFER_DTE_META,
                               zSINGLE | zLEVEL_APPLICATION );
         if ( nRC >= 0 )
         {
            // Set a name for the list box and for the XOD build.
            SetNameForView( vTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
            SetNameForView( vTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
         }
      }
      else
         SetNameForView( vTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   }
   SetCursorFirstEntityByString( vTE, "TE_DBMS_Source", "Name", "ZDMA", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_SaveLOD
// PURPOSE:   Save the LOD to a file along with the associated XOD
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_Save
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveLOD( zVIEW vSubtask )
{
   zVIEW   vTZZOLFLO;
   zVIEW   vLOD;
   zVIEW   vTZZOLOD1;
   zVIEW   vTZZOLOD2;
   zVIEW   vTZTEDBLO;
   zVIEW   vDTE;
   zVIEW   vXOD;
   zSHORT  nRC;
   zUSHORT lLevel, nFirstLevel;
   zLONG   lZKey;
   zVIEW   vFlatTgt;
   zVIEW   vFlatSrc;
   zLONG   lFlatTgt;
   zLONG   lFlatSrc;
   zBOOL   bEntity;
   zBOOL   bError = FALSE;
   zCHAR   szHierEntityName[ 33 ];
   zCHAR   szLOD_Name[ 33 ];
   zCHAR   szNetworkName[ 33 ];
   zCHAR   szOrigDBMS_Name[ 33 ];
   zCHAR   szFileName[ 400 ];
   zCHAR   szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR   szQueryDBMS_Type[ 10 ];

   // Get Access to LOD Object
   nRC = GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwTZZOLODD_SaveLOD) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00116", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Get Access to LOD List Object
   nRC = GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      // Frank H. (97/04/16): If the User has tried (with an updated LOD)
      // to open another LPLR, answer NO (don't save the LOD yet),
      // then cancels out of this (the LPLR list),
      // --> the LOD_List (TZZOLFLO) is dropped.
      // (and he still has his updated LOD)
      // therefore we try to LoadLODList here for a second time.
      // If this wasn't successful, we RETURN with an error.

      zwTZZOLODD_LoadLODList( vSubtask );
      nRC = GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
      if ( nRC < 1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access LOD List. LOD List must be opened." );
         MessageSend( vSubtask, "ZO00117", "Zeidon Object Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
   }

   nRC = CheckExistenceOfEntity( vLOD, "LOD" );
   if ( nRC < zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Save empty LOD." );
      MessageSend( vSubtask, "ZO00119", "Zeidon Object Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Execute the object constraints for TZZOLODO.
   nRC = oTZZOLODO_Constraint( vSubtask, vLOD, zOCE_COMMIT, 0 );
   if ( nRC < 0 )
      return( nRC );

   //////////////////////////////////////////////////////////////////////////
   //
   // Hierarchical Validations
   //
   //////////////////////////////////////////////////////////////////////////

   // The following validations are done against the recursive LOD_EntityParent
   // and LOD_EntityChild structure.  We will navigate that side of the
   // LOD structure hierarchically and perform the following validations each
   // time we SetViewToSubobject to a LOD_EntityChild entity.

   // Validation 1: Check that the relationship (RelLink) between an entity
   // and its parent is not the reverse path (same RelType) as between the
   // parent entity and its own parent.

   // Navigate the recursive structure hierarchically and stop at each
   // LOD_EntityChild.

   lFlatTgt = 0;  // reorder flat enitities in hierarchical order
   CreateViewFromViewForTask( &vFlatTgt, vLOD, 0 );
   ResetView( vFlatTgt );
   CreateViewFromViewForTask( &vFlatSrc, vLOD, 0 );
   ResetView( vFlatSrc );

   CreateViewFromViewForTask( &vTZZOLOD1, vLOD, 0 );
   SetCursorFirstEntity( vTZZOLOD1, "LOD", "" );
   DefineHierarchicalCursor( vTZZOLOD1, "LOD_EntityParent" );
   GetIntegerFromAttribute( &lZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vFlatTgt, "LOD_Entity",
                                       "ZKey", lZKey, 0 ) == 0 )
   {
      lFlatTgt = GetRelativeEntityNumber( vFlatTgt, "LOD_Entity", 0, 0 );
   }

   nRC = SetCursorNextEntityHierarchical( &lLevel,
                                          szHierEntityName, vTZZOLOD1 );
   nFirstLevel = lLevel;
   while ( nRC >= zCURSOR_SET && lLevel >= nFirstLevel )
   {
      if ( zstrcmp( szHierEntityName, "LOD_EntityChild") == 0 ||
           zstrcmp( szHierEntityName, "LOD_EntityParent") == 0 )
      {
         bEntity = TRUE;
         GetIntegerFromAttribute( &lZKey, vTZZOLOD1,
                                  szHierEntityName, "ZKey" );
         if ( SetCursorFirstEntityByInteger( vFlatSrc, "LOD_Entity",
                                             "ZKey", lZKey, 0 ) == 0 )
         {
            lFlatSrc = GetRelativeEntityNumber( vFlatSrc, "LOD_Entity",
                                                0, 0 );
            if ( lFlatSrc != lFlatTgt + 1 )
            {
               MoveSubobject( vFlatTgt, "LOD_Entity",
                              vFlatSrc, "LOD_Entity",
                              zPOS_AFTER, zREPOS_NONE );
            }
         }

         SetCursorFirstEntityByInteger( vFlatTgt, "LOD_Entity",
                                        "ZKey", lZKey, 0 );
         lFlatTgt = GetRelativeEntityNumber( vFlatTgt, "LOD_Entity", 0, 0 );

         if ( zstrcmp( szHierEntityName, "LOD_EntityChild" ) == 0 )
            SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
      }
      else
      {
         bEntity = FALSE;
      }

      // Validation 1:  If we just got position on a LOD_EntityParent (the
      // last entity name, szHierEntityName, was either "LOD_EntityChild" or
      // "LOD_EntityParent") make sure that the RelType for the parent entity
      // is not the same as for this entity.
      if ( bEntity )
      {
         CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
         if ( CheckExistenceOfEntity( vTZZOLOD2,
                                      "ER_RelTypeRec" ) >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lZKey, vTZZOLOD2,
                                     "ER_RelTypeRec", "ZKey" );
            ResetViewFromSubobject( vTZZOLOD2 );
            if ( CheckExistenceOfEntity( vTZZOLOD2,
                                         "ER_RelTypeRec" ) >= zCURSOR_SET )
            {
               nRC = CompareAttributeToInteger( vTZZOLOD2, "ER_RelTypeRec",
                                                "ZKey", lZKey );
               // It is no error if the ZKeys match, but a WARNING !!!
               if ( nRC == 0 )
               {
                  strcpy_s( szMsg, sizeof( szMsg ), "LOD Entity, " );
                  GetStringFromAttribute( szHierEntityName, sizeof( szHierEntityName ), vTZZOLOD2,
                                          "LOD_EntityChild", "Name" );
                  strcat_s( szMsg, sizeof( szMsg ), szHierEntityName );
                  strcat_s( szMsg, sizeof( szMsg ),
                           ", is for the same relationship as its parent, " );
                  GetStringFromAttribute( szHierEntityName, sizeof( szHierEntityName ), vTZZOLOD2,
                                          "LOD_EntityParent", "Name" );
                  strcat_s( szMsg, sizeof( szMsg ), szHierEntityName );
                  strcat_s( szMsg, sizeof( szMsg ), "." );
                  MessageSend( vSubtask, "ZO00171",
                               "Zeidon Object Maintenance", szMsg,
                               zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
                  // DropView( vTZZOLOD2 );
                  // DropView( vTZZOLOD1 );
                  // return( -1 );
               }
            }
         }

         DropView( vTZZOLOD2 );
      }

      nRC = SetCursorNextEntityHierarchical( &lLevel, szHierEntityName,
                                             vTZZOLOD1 );
   }

   DropView( vTZZOLOD1 );
   DropView( vFlatTgt );
   DropView( vFlatSrc );

   // Sort Attributes in original sequence
   oTZZOLODO_SortAttributes( vLOD );
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   // Make sure the QueryFrameAttribute subobject is correctly built.
   zwTZZOLODD_QueryFrameSetUp( vSubtask );

   // Position on either the Access or SQL Server Database definition and save LOD with TE.
   // The default is Access. SQL Server requires the "DBMS=S" .INI entry. A combination of SQL Server on
   // the server and Network on the client requires the "DBMS=S" .INI entry.
   SysReadZeidonIni( -1, "[Query]", "DBMS", szQueryDBMS_Type, sizeof( szQueryDBMS_Type ) );
   zwTZZOLODD_ActivateTE( vSubtask );
   GetViewByName( &vDTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   if ( *szQueryDBMS_Type == 'S' || *szQueryDBMS_Type == 'N')
      SetCursorFirstEntityByString( vDTE, "TE_DBMS_Source", "DBMS", "SQLServer via ODBC", 0 );
   else
      SetCursorFirstEntityByString( vDTE, "TE_DBMS_Source", "DBMS", "MS Access via ODBC", 0 );
   if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
      SetAttributeFromAttribute( vLOD, "POD",            "TE_SourceZKey",
                                 vDTE, "TE_DBMS_Source", "ZKey" );
   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   if ( vTZTEDBLO == 0 )
   {
      ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl",
                           zMULTIPLE );
      SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   }

   // The following operation saves the LOD to the source LPLR and builds an XOD to the DBMS identified above.
   nRC = zwTZZOLODD_SaveLODForTE( vSubtask );

   if ( *szQueryDBMS_Type == 'N')
   {
      // If the DBMS Type is Network, then we have just build a SQL Server XOD and wrote it out to the
      // client executable directory. That XOD is still in memory by the name of "". So in this step
      // we will:
      // 1. Write out the XOD just built to the Server executable directory, identified in the .INI file.
      // 2. Rebuild and write out the XOD as a network XOD.
      SysReadZeidonIni( -1, "[Query]", "ServerExec", szFileName, sizeof( szFileName ) );
      if ( *szFileName == 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "The ServerExec entry in the ZEIDON.INI must exist for Query DBMS 'N'." );
         MessageSend( vSubtask, "", "Query View Maintenance",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
      // 1. Write of the XOD just built.
      GetViewByName( &vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ),
                              vLOD, "LOD", "Name" );
      strcat_s( szFileName, sizeof( szFileName ), "\\" );
      strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
      strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
      CommitOI_ToFile( vXOD, szFileName, zASCII );
      DropObjectInstance( vXOD );
      TraceLineS( "Query XOD Name: ", szFileName );
      // 2. Rebuild and write out the XOD as a network XOD.
      SysReadZeidonIni( -1, "[Network]", "DefaultNetwork", szNetworkName, sizeof( szNetworkName ) );
      if ( *szNetworkName == 0 )
      {
         MessageSend( vSubtask, "", "Query View Maintenance",
                      "DefaultNetwork entry not found in ZEIDON.INI.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         return( -1 );
      }
      nRC = SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "Network",
                                          szNetworkName, 0 );
      if ( nRC < zCURSOR_SET )
      {
         MessageSend( vSubtask, "", "Query View Maintenance",
                      "DefaultNetwork value from ZEIDON.INI not found in DB Handler object.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         return( -1 );
      }
      GetStringFromAttribute( szOrigDBMS_Name, sizeof( szOrigDBMS_Name ),
                              vDTE, "TE_DBMS_Source", "DBMS" );
      SetAttributeFromAttribute( vDTE,      "TE_DBMS_Source", "DBMS",
                                 vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
      SetAttributeFromAttribute( vDTE,      "TE_DBMS_Source", "Network",
                                 vTZTEDBLO, "TE_DBMS_Source", "Network" );
      if ( CheckExistenceOfEntity( vLOD,
                                   "POD" ) >= zCURSOR_SET )
         SetAttributeFromAttribute( vLOD, "POD",            "TE_SourceZKey",
                                    vDTE, "TE_DBMS_Source", "ZKey" );
      zwTZZOLODD_SaveLODForTE( vSubtask );
      SetAttributeFromString( vDTE, "TE_DBMS_Source", "DBMS", szOrigDBMS_Name );
      SetAttributeFromString( vDTE, "TE_DBMS_Source", "Network", "" );
   }

   MB_SetMessage( vSubtask, 1, "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_SaveLODWithoutTE
// PURPOSE:   Implement a LOD for portable file use only.
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_SaveTE
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveLODWithoutTE( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Get Access to LOD
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwTZZOLODD_SaveLODWithoutTE) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00127", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // If LOD contains POD reference, delete it
   if ( CheckExistenceOfEntity( vTZZOLODO, "POD" ) >= zCURSOR_SET )
   {
      DeleteEntity( vTZZOLODO, "POD", zREPOS_NONE );
   }

   // Make sure the LOD is not empty
   nRC = CheckExistenceOfEntity( vTZZOLODO, "LOD" );
   if (  nRC < zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Save empty LOD." );
      MessageSend( vSubtask, "ZO00128", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Delete all POD_Entities (Joind Definietions)
   nRC = MessagePrompt( vSubtask, "ZO00182", "Zeidon Objects Maintenance",
                        "Delete all Properties for Join Definitions?", 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
   if ( nRC == zRESPONSE_YES )
      oTZZOLODO_DeleteAllPOD_Entities( vTZZOLODO );

   // Sort Attributes in original sequence
   oTZZOLODO_SortAttributes( vTZZOLODO );
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   // Save the LOD
   nRC = CommitMetaOI( vSubtask, vTZZOLODO, zSOURCE_LOD_META );
   if ( nRC < 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Save LOD to LPLR." );
      MessageSend( vSubtask, "ZO00129", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_SaveLODForNetwork
// PURPOSE:   Implement a LOD using a particular Tech. Environment.
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_SaveTE
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveLODForNetwork( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vDTE;
   zVIEW  vTZTEDBLO;
   zVIEW  vXOD;
   zVIEW  vLPLR;
   zCHAR  szOrigDBMS_Name[ 33 ];
   zCHAR  szNetworkName[ 33 ];
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   // First execute the Save of the LOD with TE. This does two things:
   // 1. It performs validations.
   // 2. It saves the LOD with the correct POD.
   nRC = zwTZZOLODD_SaveLODForTE ( vSubtask );
   if ( nRC < 0 )
      return( nRC );

   // Set up views.
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vDTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
      GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl",
                           zMULTIPLE );
      SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   }

   // Save current DBMS value.
   GetStringFromAttribute( szOrigDBMS_Name, sizeof( szOrigDBMS_Name ), vDTE, "TE_DBMS_Source", "DBMS" );

   // Position on corresponding DBMS entry in DB Handler object.
   SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                               vDTE,      "TE_DBMS_Source", "DBMS", 0 );

   // Check if current DBMS is a Network specification. We wiil know this
   // because the attribute "Network" value will be other than null.
   if ( CompareAttributeToString( vTZTEDBLO, "TE_DBMS_Source", "Network",
                                  "" ) != 0 )
   {
      // Current specification is Network, so just use it to build the XOD.
      oTZZOXODO_SaveXOD( vSubtask, vTZZOLODO );
   }
   else
   {
      // Current specification is not Network, get Default Network value from
      // INI and then locate in DB Handler object. Don't use it if it wasn't
      // found in DB Handler object.
      SysReadZeidonIni( -1, "[Network]", "DefaultNetwork", szNetworkName, sizeof( szNetworkName ) );
      if ( szNetworkName[ 0 ] )
      {
         nRC = SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "Network",
                                             szNetworkName, 0 );
         if ( nRC < zCURSOR_SET )
         {
            MessageSend( vSubtask, "TE00426", "Technical Environment",
                         "Default Network value not found in DB Handler object; using internal default.",
                         zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            szNetworkName[ 0 ] = 0;
         }
         // The else condition means we're positioned on the Default Network.
      }

      if ( szNetworkName[ 0 ] == 0 )
      {
         // Use default network for Ton Beller (DBMS = "Network Server")
         SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                       "Network Server", 0 );
      }

      SetAttributeFromAttribute( vDTE,      "TE_DBMS_Source", "DBMS",
                                 vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
      SetAttributeFromAttribute( vDTE,      "TE_DBMS_Source", "Network",
                                 vTZTEDBLO, "TE_DBMS_Source", "Network" );

      // Build the XOD in memory
      oTZZOXODO_SaveXOD( vSubtask, vTZZOLODO );

      // Commit the XOD to LPLR file.
      GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ),
                              vLPLR, "W_MetaDef", "Name" );
      GetViewByName( &vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szFileName, sizeof( szFileName ),
                              vLPLR, "LPLR", "ExecDir" );
      ofnTZCMWKSO_AppendSlash( szFileName );
      strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
      strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
      CommitOI_ToFile( vXOD, szFileName, zSINGLE );

      // Reset values back to what they were.
      SetAttributeFromString( vDTE, "TE_DBMS_Source", "DBMS", szOrigDBMS_Name );
      SetAttributeFromString( vDTE, "TE_DBMS_Source", "Network", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_GenReportVML
// PURPOSE:   Generate First cut VML for code supporting a report of
//            an instance of any LOD Definition
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_SaveDBV
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_GenReportVML( zVIEW vSubtask )
{
   zVIEW vLOD;
   zVIEW vProfile;
   zSHORT nRC;

   // Get Access to LOD Object
   nRC = GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      // Get Access to and Commit Profile Object
      nRC = oTZ__PRFO_GetViewToProfile( &vProfile, "ZO", vSubtask, zCURRENT_OI );
      if ( nRC >= 0 )
      {
         nRC = oTZ__PRFO_CommitProfile ( vProfile );
         // Produce VML Source Code
      // nRC = oTZZOLODO_GenReportVML( vLOD, vProfile );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_BuildTEList
// PURPOSE:   Build a list of Technical Environments
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_SaveDBV
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_BuildTEList( zVIEW vSubtask )
{
   zVIEW  vT;
   zVIEW  vTE;
   zVIEW  vTZTEDBLO;
   zSHORT nRC;

   // Activate TE to present list of source entries, if it's not already there.
   nRC = GetViewByName( &vTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = GetViewByName( &vTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         RetrieveViewForMetaList( vSubtask, &vT, zREFER_DTE_META );
         nRC = ActivateMetaOI( vSubtask, &vTE, vT, zREFER_DTE_META,
                               zSINGLE | zLEVEL_APPLICATION );
         if ( nRC >= 0 )
         {
            // Set a name for the list box and for the XOD build.
            SetNameForView( vTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
            SetNameForView( vTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
         }
      }
      else
         SetNameForView( vTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   }

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   if ( vTZTEDBLO == 0 )
   {
      ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl",
                           zMULTIPLE );
      SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SummaryReport( zVIEW vTZZOLODO, zPCHAR szReportFileName );

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ReportLOD
// PURPOSE:   Perform hierarchical report of LOD
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_Print
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ReportLOD( zVIEW vSubtask )
{
   zVIEW  vLPLR;
   zVIEW  vTZZOLODO;
   zSHORT nRC;
   zCHAR  szReportFileName[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR szName;
   zCHAR  szMsg[ zMAX_FILESPEC_LTH + zSHORT_MESSAGE_LTH + 1 ];

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Get Access to LPLR for Directory
   nRC = GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetStringFromAttribute( szReportFileName, sizeof( szReportFileName ), vLPLR, "LPLR", "MetaSrcDir" );
      strcat_s( szReportFileName, sizeof( szReportFileName ), "\\" );
   }
   else
   {
      szReportFileName[ 0 ] = 0;
   }
   GetAddrForAttribute( &szName, vTZZOLODO, "LOD", "Name" );
   strcat_s( szReportFileName, sizeof( szReportFileName ), szName );
   strcat_s( szReportFileName, sizeof( szReportFileName ), ".RPT" );

   // Procuce Report file from LOD instance
   nRC = oTZZOLODO_SummaryReport( vTZZOLODO, szReportFileName );

   if ( nRC >= 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "LOD Report produced to file:\n\t\'" );
      strcat_s( szMsg, sizeof( szMsg ), szReportFileName );
      strcat_s( szMsg, sizeof( szMsg ), "\'." );
      MessageSend( vSubtask, "ZO00133", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Error opening file \'" );
      strcat_s( szMsg, sizeof( szMsg ), szReportFileName );
      strcat_s( szMsg, sizeof( szMsg ), "\' for output." );
      MessageSend( vSubtask, "ZO00134", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ExitLOD
// PURPOSE:   Exit LOD Dialog and save the current LOD
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_Exit
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ExitLOD( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;
   zVIEW    vProfileXFER;
   zVIEW    vLPLR;
   zVIEW    vSaveAs;
   zSHORT   nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   nRC = zwfnTZZOLODD_AskForSave ( vSubtask );
   if ( nRC  != 0 )
   {
      if ( vSaveAs )
      {
         if ( nRC == 99 )
            // if LOD not checked out and user change this LOD, then zeidon
            // call the window "Save LOD as"
            // We save the action after Save as in the View TZSAVEAS (e.g.
            // Create new LOD or Open an other LOD or Switch Project or
            // Exit LOD Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zExitToolAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   if ( vSaveAs )
      DropMetaOI( vSubtask, vSaveAs );

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "ZO", vSubtask, zCURRENT_OI );
   if ( CompareAttributeToString( vProfileXFER, "ZO", "StartupLPLR_Option",
                                  "L" ) == 0 )   //set Last LPLR
   {
      if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
      {
         SetAttributeFromAttribute( vProfileXFER, "ZO", "StartupLPLR_Name",
                                    vLPLR, "LPLR", "Name" );
      }
   }

   if ( vTZZOLODO )
   {
      SetAttributeFromAttribute( vProfileXFER, "ZO", "LastLOD",
                                 vTZZOLODO, "LOD", "Name" );
   }

   if ( GetViewByName( &vLPLR, "TZERSASO_REF", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromAttribute( vProfileXFER, "ZO", "LastSA",
                                 vLPLR, "SubjectArea", "Name" );
   }

   oTZ__PRFO_CommitProfile ( vProfileXFER );
   if ( vTZZOLODO )
   {
      DropMetaOI( vSubtask, vTZZOLODO );
   }

   // Go to teminate the LPLR, but then make sure the cached XLP is really gone.
   nRC = TerminateLPLR( vSubtask );
   nRC = GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      DropObjectInstance( vLPLR );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DeleteLOD
// PURPOSE:   Delete an existing instance of a LOD
// TRIGGERED BY:
//    WINDOW: TZLODOPMD
//    ACTION: BUTTON
//    NAME: &Delete
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteLOD( zVIEW vSubtask )
{
   zVIEW  vTZZOLFLO;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ 400 ];
   zCHAR   szQueryDBMS_Type[ 10 ];

   // Get Access to LOD File List Object
   nRC = GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access LOD File List. Delete aborted." );
      MessageSend( vSubtask, "ZO00135", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }
   SetCursorFirstEntityByEntityCsr( vTZZOLFLO, "W_MetaDef", vTZZOLFLO, "QueryW_MetaDef", "" );

   // Inform user of LOD being Deleted
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_LOD,
                       zREFER_LOD_META,
                       vTZZOLFLO, "W_MetaDef", "Name", 0 );
   // Delete existing LOD & XOD
   if ( nRC == zRESPONSE_YES )
   {
      SysReadZeidonIni( -1, "[Query]", "DBMS", szQueryDBMS_Type, sizeof( szQueryDBMS_Type ) );
      if ( *szQueryDBMS_Type == 'N')
      {
         // If the DBMS Type is Network, then we have to delete the XOD
         // from the Server bin directory.
         SysReadZeidonIni( -1, "[Query]", "ServerExec", szFileName, sizeof( szFileName ) );
         if ( *szFileName == 0 )
         {
            MessageSend( vSubtask, "", "Query View Maintenance",
                        "The ServerExec entry in the ZEIDON.INI must exist, for Query DBMS 'N'.",
                        zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -1 );
         }
         GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ),
                                 vTZZOLFLO, "QueryW_MetaDef", "Name" );
         strcat_s( szFileName, sizeof( szFileName ), "\\" );
         strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
         strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
         zDeleteFile( szFileName );
      }
      nRC = DeleteMetaOI( vSubtask, vTZZOLFLO, zSOURCE_LOD_META );
      SetCursorPrevEntity( vTZZOLFLO, "W_MetaDef", "" );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_SwitchLPLR
// PURPOSE:    This Entry switches to a new LPLR.  All named views to
//             the current LPLR file List must be droped first.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SwitchLPLR( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTemp;
   zVIEW  vSaveAs;

   // Ask and do save
   nRC = zwfnTZZOLODD_AskForSave( vSubtask);
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if LOD not checked out and user change this LOD, then zeidon
            // call the window "Save LOD as"
            // We save the action after Save as in the View TZSAVEAS (e.g.
            // Create new LOD or Open an other LOD or Switch Project or
            // Exit LOD Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zSwitchLPLRAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   // Drop view to ERD File List Object
   if ( GetViewByName( &vTemp, "TZZOMFLO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vTemp );

   // Drop view to SA File List Object
   if ( GetViewByName( &vTemp, "TZZOSFLO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vTemp );

   // Drop view to LOD File List Object
   if ( GetViewByName( &vTemp, "TZZOLFLO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vTemp );

   // Drop view to TE File List Object
   if ( GetViewByName( &vTemp, "TZTEDFLO", vSubtask, zLEVEL_TASK ) > 0 )
      nRC = DropView( vTemp );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_SortByName
// PURPOSE:    This Entry sorts the LOD list in the Open Dialog by Name.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SortByName( zVIEW    vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   // Get view to SA File List Object
   nRC = GetViewByName( &vT, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Sort LOD's by Name
   nRC = zgSortEntityWithinParent( zASCENDING, vT,
                                   "W_MetaDef", "Name", "" );
   SetCursorFirstEntity( vT, "W_MetaDef", "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_SortByDate
// PURPOSE:    This Entry sorts the LOD list in the Open Dialog by Last
//             Update.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SortByDate( zVIEW    vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   // Get view to SA File List Object
   nRC = GetViewByName( &vT, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Sort LOD's by Last Update
   nRC = zgSortEntityWithinParent( zDESCENDING, vT,
                                   "W_MetaDef", "LastUpdateDate", "" );
   SetCursorFirstEntity( vT, "W_MetaDef", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SortByDesc( zVIEW vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   // Get view to SA File List Object
   nRC = GetViewByName( &vT, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Sort LOD's by Last Update
   nRC = zgSortEntityWithinParent( zASCENDING, vT,
                                   "W_MetaDef", "Desc", "" );
   SetCursorFirstEntity( vT, "W_MetaDef", "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_SelectConstraint
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SelectConstraint( zVIEW vSubtask )
{
   zVIEW  vLOD;
   zVIEW  vMETAOBJECT;
   zSHORT nRC;
   zSHORT nEnable = 0;

   // Set the select set for the object constraint combo box
   CB_SetDisplaySelectSet( vSubtask, "ComboSelConstraint", 2 );

   // Get Access to LOD Object
   nRC = GetViewByName( &vMETAOBJECT, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( vMETAOBJECT && MiGetUpdateForView( vMETAOBJECT ) )
   {
      nRC = SetCursorFirstEntity( vMETAOBJECT, "LOD_ConstraintOper", "" );
      if ( nRC <  zCURSOR_SET )
      {
         nEnable = 0;
         SetAttributeFromString( vMETAOBJECT, "LOD", "ActivateConstraint", "" );
         RefreshCtrl( vSubtask, "CheckActConstraint" );
         SetAttributeFromString( vMETAOBJECT, "LOD", "ActivateEmptyConstraint", "" );
         RefreshCtrl( vSubtask, "CheckActEmptyConstraint" );
         SetAttributeFromString( vMETAOBJECT, "LOD", "CommitConstraint", "" );
         RefreshCtrl( vSubtask, "CheckCommitConstraint" );
         SetAttributeFromString( vMETAOBJECT, "LOD", "DropOIConstraint", "" );
         RefreshCtrl( vSubtask, "CheckDropConstraint" );
      }
      else
      {
         if ( ComponentIsCheckedOut( vSubtask, vMETAOBJECT, zSOURCE_LOD_META ))
            nEnable = 1;
      }
   }
   else
   {
      SetCtrlState( vSubtask, "ComboSelConstraint", zCONTROL_STATUS_ENABLED, 0 );
   }

   SetCtrlState( vSubtask, "CheckActConstraint", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CheckActEmptyConstraint", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CheckCommitConstraint", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CheckDropConstraint", zCONTROL_STATUS_ENABLED, nEnable );

   if ( CheckExistenceOfEntity( vMETAOBJECT, "LOD_ConstraintOper" ) >= zCURSOR_SET )
      SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 0 );

   nRC = GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( nRC == zLEVEL_TASK )
   {
      // Build select set 2 in vLOD for object constraint operations
      SetSelectSetForView( vLOD, 2 );
      SetAllSelectStatesForEntity( vLOD, "Operation", 0, 0 );
      nRC = SetCursorFirstEntityByString( vLOD, "Operation", "Type",
                                          "C", "LOD" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         nRC = SetSelectStateOfEntity( vLOD, "Operation", 1 );
         nRC = SetCursorNextEntityByString( vLOD, "Operation",
                                         "Type", "C", "LOD" );
      }

      SetSelectSetForView( vLOD, 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_EntityNext
// PURPOSE:   Display the next LOD Entity
// TRIGGERED BY:
//    WINDOW: LOD_ENTIT
//    ACTION: BUTTON
//    NAME: Next
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EntityNext( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vEntityList;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( zwTZZOLODD_OKEntity( vSubtask ) < 0 )
      return( -1 );

   // Reposition on next entity and create a new temporal.
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 0 );

   SetCursorNextEntity( vTZZOLODO, "LOD_Entity", "" );
   CreateTemporalSubobjectVersion( vTZZOLODO, "LOD_Entity" );

   zwTZZOLODD_PostBuildEntmaint( vSubtask );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 1 );
   SetFocusToCtrl( vSubtask, "Name" );

   GetSubtaskForWindowName( vSubtask, &vEntityList, "ENTVIEW" );
   if ( vEntityList )
      RefreshWindow( vEntityList );

   // Set position of the Hier Diagram to current Entity
   zwTZZOLODD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_EntityPrev
// PURPOSE:   Display the previous LOD Entity
// TRIGGERED BY:
//    WINDOW: LOD_ENTIT
//    ACTION: BUTTON
//    NAME: Prev
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EntityPrev( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vEntityList;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( zwTZZOLODD_OKEntity( vSubtask ) < 0 )
      return( -1 );

   // Reposition on next entity and create a new temporal.
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 0 );

   SetCursorPrevEntity( vTZZOLODO, "LOD_Entity", "" );
   CreateTemporalSubobjectVersion( vTZZOLODO, "LOD_Entity" );

   zwTZZOLODD_PostBuildEntmaint( vSubtask );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 1 );
   SetFocusToCtrl( vSubtask, "Name" );

   GetSubtaskForWindowName( vSubtask, &vEntityList, "ENTVIEW" );
   if ( vEntityList )
      RefreshWindow( vEntityList );

   // Set position of the Hier Diagram to current Entity
   zwTZZOLODD_RefreshEntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddWorkEntity
// PURPOSE:   Add LOD Entity as a Work Entity
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEnt
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddWorkEntity( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zSHORT nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Add Entity as a Work Entity
   oTZZOLODO_AddEntity( vSubtask, vTZZOLODO, "W" );

   // Initialize LOD Entity Indicators for Work Entity
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Work", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Create", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Delete", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Include", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Exclude", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "InclSrc", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Recursive", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Derived", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity",
                                 "ParentDeleteBehave", "D" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Update", "Y" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromERRel
// PURPOSE:   Insure SARELVIEW is active
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: Option
//    NAME: AddEntFromERwithRel
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromERRel( zVIEW vSubtask )
{
   zVIEW vSARELVIEW;
   zVIEW vTZZOLODO;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Get Access to SARELVIEW window
   GetSubtaskForWindowName( vSubtask, &vSARELVIEW, "SARELVIEW" );
   if ( vSARELVIEW == 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZZOLODQ", "SARELVIEW" );
   }
   else
   {
      ActivateWindow( vSARELVIEW );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromERRelAsCR
// PURPOSE:   Add LOD Entity as a ER based Entity and mark as Creatable
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEntFromER as Create
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromERRelAsCR( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW vTZZOLODO;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Add Entity as a ER based Entity through a Relationship
   oTZZOLODO_AddEntity( vSubtask, vTZZOLODO, "R" );

   // Initialize LOD Entity Indicators for ER Entity/Rel as Creatable
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Create",
                                  "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Delete",
                                  "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Include",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Exclude",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "InclSrc",
                                  "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "ParentDeleteBehave",
                                  "D" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Recursive",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Derived",
                                  "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Update",
                                  "Y" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromERRelAsINC
// PURPOSE:   Add LOD Entity as a ER based Entity and mark as Includable
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEntFromER as Includable
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromERRelAsINC( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zSHORT nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Add Entity as a ER based Entity through a Relationship
   oTZZOLODO_AddEntity( vSubtask, vTZZOLODO, "R" );

   // Initialize LOD Entity Indicators for ER Entity/Rel as Includeable
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Create", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Delete", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Include", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Exclude", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "InclSrc", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity",
                                 "ParentDeleteBehave", "E" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Recursive", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Derived", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Update", "N" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromERRelAsDSP
// PURPOSE:   Add LOD Entity as a ER based Entity and mark as Displayable
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEntFromER as Recursive
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromERRelAsDSP( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zSHORT nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Add Entity as a ER based Entity through a Relationship
   oTZZOLODO_AddEntity( vSubtask, vTZZOLODO, "R" );

   // Initialize LOD Entity Indicators for ER Entity/Rel as Recursive
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Create", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Delete", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Include", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Exclude", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "InclSrc", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Recursive", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity",
                                 "ParentDeleteBehave", "R" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Derived", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Update", "N" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromEREnt
// PURPOSE:   Insure SAENTVIEW is active
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: Option
//    NAME: AddEntFromERwithoutRel
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromEREnt( zVIEW vSubtask )
{
   zVIEW vSAENTVIEW;
   zVIEW vTZZOLODO;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Get Access to SAENTVIEW window
   GetSubtaskForWindowName( vSubtask, &vSAENTVIEW, "SAENTVIEW" );
   if ( vSAENTVIEW == 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZZOLODQ", "SAENTVIEW" );
   }
   else
   {
      ActivateWindow( vSAENTVIEW );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromEREntAsDER
// PURPOSE:   Add LOD Entity as a ER based Entity and mark as Derived
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEntFromER as Recursive
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromEREntAsDER( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vWindow;
   zSHORT nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Add Entity as a ER based Entity
   oTZZOLODO_AddEntity( vSubtask, vTZZOLODO, "E" );

   // Initialize LOD Entity Indicators for ER Entity as Derived
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Create", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Delete", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Include", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Exclude", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "InclSrc", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Recursive", "N" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Derived", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity",
                                 "ParentDeleteBehave", "D" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Update", "Y" );

   if ( zwTZZOLODD_OK_NewEntity( vSubtask ) != 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZZOLODQ", "ENTMAINT" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddEntFromERThruRel
// PURPOSE:   Add an Entity via a relationship based on CR, INC or Rec
// TRIGGERED BY:
//    WINDOW: ENTVIEW
//    ACTION: DblClick
//    NAME: AddEntFromERThruRel
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddEntFromERThruRel( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZZOLODO;
   zVIEW  vWindow;

   // Get Access to LOD Object
   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Always set the entity as "displayable".
   zwTZZOLODD_AddEntFromERRelAsDSP ( vSubtask );

   if ( zwTZZOLODD_OK_NewEntity( vSubtask ) != 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZZOLODQ", "ENTMAINT" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DeleteEntity
// PURPOSE:   Delete a LOD Entity from a LOD.   Note that all subordinate
//            entities are also deleted.
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: DelEnt
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteEntity( zVIEW vSubtask )
{
   zVIEW   vTZZOLODO;
   zVIEW   vTZZOHDGO;
   zVIEW   vW;
   zSHORT  nRC;
   zCHAR   szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( (CompareAttributeToInteger( vTZZOLODO, "LOD_Entity", "IndentLvl", 0 ) == 0) ||
        (CompareAttributeToString( vTZZOLODO, "LOD_Entity", "IndentLvl", "" ) == 0) )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Delete Root Entity in Logical Object." );
      MessageSend( vSubtask, "ZO00146", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Delete LOD Entity
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_LOD,
                       zENAME_LOD_ENTITY,
                       vTZZOLODO, "LOD_Entity", "Name", 0 );

   if ( nRC == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   DeleteEntity( vTZZOLODO, "LOD_Entity", zREPOS_PREV );

   GetSubtaskForWindowName( vSubtask, &vW, "ENTMAINT" );
   if ( vW )
   {
      CreateTemporalSubobjectVersion( vTZZOLODO, "LOD_Entity" );
      zwTZZOLODD_PostBuildEntmaint( vSubtask );
      SetFocusToCtrl( vSubtask, "Name" );
   }

   SetSelectStateOfEntity( vTZZOLODO, "LOD_Entity", 1 );
   GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" );
   if ( vW )
      RefreshWindow( vW );

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK )
                                                      == zLEVEL_TASK )
   {
      DropObjectInstance( vTZZOHDGO );
      oTZZOHDGO_CreateFromLOD( vSubtask, &vTZZOHDGO, vTZZOLODO, ORIENT );
      SetNameForView( vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );
      GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
      RefreshCtrl( vW, "HierDiag1" );
   }
   // Set position of the Hier Diagram to current Entity
   zwTZZOLODD_RefreshEntitySelect( vSubtask );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_CheckEntityType
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZZOLODD_CheckEntityType( zVIEW  vSubtask,
                            zVIEW  vTZZOLODO )
{
   zSHORT  nEnable = 1;

   if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) == 0 )
   {
      nEnable = 0;
      SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Derived", "N" );
   }

   SetCtrlState( vSubtask, "Derived", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Work", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
} // zwTZZOLODD_CheckEntityType



/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_PostBuildEntmaint
// PURPOSE:   Prevent the Delete of the Root Entity in the LOD.
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: DelEnt
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildEntmaint( zVIEW vSubtask )
{
   zVIEW   vLOD;
   zVIEW   vTZZOLODO;
   zSHORT  nEnable = 1;
   zSHORT  nRC;

   // Set the select set for the entity constraint combo box
   CB_SetDisplaySelectSet( vSubtask, "ComboSelEntConstraint", 3 );

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Disable Delete Button on LOD Entity Detail window if Entity is root.
   if ( (CompareAttributeToInteger( vTZZOLODO, "LOD_Entity", "IndentLvl", 0 ) == 0) ||
        (CompareAttributeToString( vTZZOLODO, "LOD_Entity", "IndentLvl", "" ) == 0) )
   {
      nEnable = 0;
   }
   SetCtrlState( vSubtask, "DeleteBtn", zCONTROL_STATUS_ENABLED, nEnable );

   nRC = GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( nRC == zLEVEL_TASK )
   {
      // Build select set 3 in vLOD for entity constraint operations
      SetSelectSetForView( vLOD, 3 );
      SetAllSelectStatesForEntity( vLOD, "Operation", 0, 0 );
      nRC = SetCursorFirstEntityByString( vLOD, "Operation", "Type",
                                          "E", "LOD" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         nRC = SetSelectStateOfEntity( vLOD, "Operation", 1 );
         nRC = SetCursorNextEntityByString( vLOD, "Operation", "Type",
                                            "E", "LOD" );
      }

      SetSelectSetForView( vLOD, 1 );
   }

   if ( !ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ) )
   {
      SetCtrlState( vSubtask, "ComboSelEntConstraint", zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "DeleteBtn", zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "Derived", zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "Work", zCONTROL_STATUS_ENABLED, 0 );
   }
   else
   {
      SetCtrlState( vSubtask, "ComboSelEntConstraint", zCONTROL_STATUS_ENABLED, 1 );
      zwTZZOLODD_CheckEntityType( vSubtask, vTZZOLODO );
   }

   // Disable check boxes based on Entity Constraint Oper
   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_EntityConstraintOper" ) >= zCURSOR_SET )
      SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ReusOKEntity
// PURPOSE:   Make sure the LOD Entity Name is unique within the LOD.
//            Reset IndentName in the case of a Name change.
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: OK
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ReusOKEntity( zVIEW vSubtask )
{
   zVIEW   vTZZOLODO;
   zVIEW   vTZZOLOD1;
   zVIEW   vTZZOLOD2;
   zVIEW   vWindow;
   zLONG   lIndentLvl;
   zCHAR   szRestrictEntityName[ 33 ];
   zCHAR   szHierEntityName[ 33 ];
   zCHAR   szEntityName[ 33 ];
   zCHAR   szEntityNameIndent[ 133 ];
   zSHORT  nRC;
   zUSHORT lLevel, lRestrictLevel;
   zCHAR   szMsg[ zMAX_MESSAGE_LTH + 1 ];

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( !MiGetUpdateForView( vTZZOLODO ) )
      return( 0 );

   // Reset Indent Name in the case of a renamed Entity

   // Make sure that the entity name is unique within the LOD.
   nRC = CreateViewFromViewForTask( &vTZZOLOD1, vTZZOLODO, 0 );
   nRC = GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOLODO, "LOD_Entity", "Name" );

   while ( szEntityName[ 0 ] == 0 ||
           SetCursorFirstEntityByString( vTZZOLOD1, "LOD_Entity",
                                         "Name", szEntityName,"" )
                                                           >= zCURSOR_SET &&
           SetCursorNextEntityByString( vTZZOLOD1, "LOD_Entity",
                                        "Name", szEntityName,"" )
                                                           >= zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "A Object Entity by the same name already exists." );
      MessageSend( vSubtask, "ZO00167", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   // Don't allow Parent Delete Behavior of "Restrict" on a root entity.
   // We will determine if an entity is a root by comparing its ZKey to
   // LOD_EntityParent.
   if ( CompareAttributeToAttribute( vTZZOLODO, "LOD_EntityParent", "ZKey",
                                     vTZZOLODO, "LOD_Entity", "ZKey" ) == 0 &&
        CompareAttributeToString( vTZZOLODO, "LOD_Entity", "ParentDeleteBehave",
                                  "R" ) == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Parent Delete Behavior of Restrict not allowed for root entity. " );
      MessageSend( vSubtask, "ZO00168", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // If Parent Delete Behavior of Restrict exists for an entity, we must make
   // sure that it also exists for all its children.
   // If Parent Delete Behavior is Exclude or Delete for an entity, we must
   // make sure that no parent of that entity is Restrict.
   // To determine the above, we will process the recursive side of the
   // relationship hierarchically, processing each subordinate entity down to
   // the current one.  Then if the Parent Delete Behavior is Restrict, we
   // will process the subordinate entities to make sure they are also Restrict.
   // If the Parent Delete Behavior is not Restrict, we will process each
   // parent entity to make sure that no parent is Restrict.

   GetStringFromAttribute( szRestrictEntityName, sizeof( szRestrictEntityName ),
                           vTZZOLODO, "LOD_Entity", "Name" );
   DefineHierarchicalCursor( vTZZOLOD1, "LOD" );

   // First loop to find the Restrict entity in the recursive side.
   nRC = SetCursorNextEntityHierarchical( &lLevel,
                                          szHierEntityName, vTZZOLOD1 );
   while ( nRC >= zCURSOR_SET )
   {
      if ( zstrcmp( szHierEntityName, "LOD_EntityChild" ) == 0 )
      {
         SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
         GetStringFromAttribute( szHierEntityName, sizeof( szHierEntityName ),
                                 vTZZOLOD1, "LOD_EntityParent", "Name" );
         if ( zstrcmp( szHierEntityName, szRestrictEntityName ) == 0 )
            break;
      }

      nRC = SetCursorNextEntityHierarchical( &lLevel,
                                             szHierEntityName, vTZZOLOD1 );
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "ParentDeleteBehave",
                                  "R" ) == 0 )
   {
      // Loop through all the subentities to the Restrict Entity and make
      // sure the entities have a Parent Delete Behavior of Restrict.
      lRestrictLevel = lLevel;
      nRC = SetCursorNextEntityHierarchical( &lLevel,
                                             szHierEntityName, vTZZOLOD1 );
      while ( nRC >= zCURSOR_SET && lLevel > lRestrictLevel )
      {
         if ( zstrcmp( szHierEntityName, "LOD_EntityChild" ) == 0 )
         {
            SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
            GetStringFromAttribute( szHierEntityName, sizeof( szHierEntityName ),
                                    vTZZOLOD1, "LOD_EntityParent", "Name" );
            if ( CompareAttributeToString( vTZZOLOD1, "LOD_EntityParent", "ParentDeleteBehave",
                                           "R" ) != 0 )
            {
               strcpy_s( szMsg, sizeof( szMsg ), "Parent Delete Behavior of Restrict is not allowed\n " );
               strcat_s( szMsg, sizeof( szMsg ), "if any child of the entity does not also have a\n" );
               strcat_s( szMsg, sizeof( szMsg ), "Parent Delete Behavior of Restrict." );
               MessageSend( vSubtask, "ZO00169", "Zeidon Object Maintenance",
                            szMsg,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
               return( -1 );
            }
         }

         nRC = SetCursorNextEntityHierarchical( &lLevel,
                                                szHierEntityName, vTZZOLOD1 );
      }
   }
   else
   {
      // Loop through all the parent entities to the Restrict Entity and make
      // sure none of them have Parent Delete Behavior of Restrict.
      CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
      nRC = ResetViewFromSubobject( vTZZOLOD2 );
      while ( nRC == zCURSOR_SET )
      {
         if ( CompareAttributeToString( vTZZOLOD2, "LOD_EntityParent", "ParentDeleteBehave",
                                        "R" ) == 0 )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Parent Delete Behavior must be Restrict because a \n " );
            strcat_s( szMsg, sizeof( szMsg ), "parent of this entity has Parent Delete Behavior\n" );
            strcat_s( szMsg, sizeof( szMsg ), "of Restrict." );
            MessageSend( vSubtask, "ZO00170", "Zeidon Object Maintenance",
                         szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }

         nRC = ResetViewFromSubobject( vTZZOLOD2 );
      }

      DropView( vTZZOLOD2 );
   }

   DropView ( vTZZOLOD1 );
   SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Name", szEntityName );

   // Reset Ident Name
   nRC = GetStringFromAttribute( szEntityNameIndent, sizeof( szEntityNameIndent ), vTZZOLODO, "LOD_Entity", "IndentName" );
   nRC = GetIntegerFromAttribute( &lIndentLvl, vTZZOLODO, "LOD_Entity", "IndentLvl" );
   if ( lIndentLvl > 0 )
   {
      zSHORT k;

      for ( k = 0;
            k < lIndentLvl && k < 21;
            k++ )
      {
         szEntityNameIndent[ k ] = ' ';
      }

      szEntityNameIndent[ k ] = 0;
      strcat_s( szEntityNameIndent, sizeof( szEntityNameIndent ), szEntityName );
   }
   else
      strcpy_s( szEntityNameIndent, sizeof( szEntityNameIndent ), szEntityName );

   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "IndentName", szEntityNameIndent );

   AcceptSubobject( vTZZOLODO, "LOD_Entity" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_OKEntity
// PURPOSE:   Make sure the LOD Entity Name is unique within the LOD.
//            Reset IndentName in the case of a Name change.
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: OK
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OKEntity( zVIEW vSubtask )
{
   zVIEW   vTZZOHDGO;
   zVIEW   vTZZOLODO;

   if ( zwTZZOLODD_ReusOKEntity( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if (!ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ))
      return( 0 );

   // if Entity Type = Work, set type for all Attribute to Work
   zwTZZOLODD_SetAttributeType( vSubtask, vTZZOLODO );

   // if Entity Type = Work or Derived, delete all POD_Entities (Join Informations)
   zwfnTZZOLODD_DeletePOD_Entity( vSubtask, vTZZOLODO );

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) == zLEVEL_TASK )
   {
      zVIEW vW;

      oTZZOHDGO_CreateFromLOD( vSubtask, &vTZZOHDGO, vTZZOLODO, ORIENT );
      SetNameForView( vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );

      GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
      RefreshCtrl( vW, "HierDiag1" );
      if ( !HD_IsNodeVisible( vW, "HierDiag1" ) )
         HD_CenterNode( vW, "HierDiag1" );
   }

   RefreshAllSubwindows( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_OK_NewEntity
// PURPOSE:   Make sure the LOD Entity Name is unique within the LOD.
//            Reset IndentName in the case of a Name change.
//            This differes from OKEntity in that the entity here is new
//            and errors must be handled diferently.
// TRIGGERED BY:
//    WINDOW: ENTMAINT
//    ACTION: BUTTON
//    NAME: OK
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OK_NewEntity( zVIEW vSubtask )
{
   zVIEW   vTZZOHDGO;
   zVIEW   vTZZOLODO;
   zVIEW   vTZZOLOD1;
   zLONG   lIndentLvl;
   zCHAR   szEntityName[ 33 ];
   zCHAR   szEntityNameIndent[ 133 ];
   zSHORT  nRC;
   zCHAR   szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( !MiGetUpdateForView( vTZZOLODO ) )
      return( 0 );

   // Accept the entity, in case it is temporal.
   AcceptSubobject( vTZZOLODO, "LOD_Entity" );

   // Reset Indent Name in the case of a renamed Entity
   nRC = GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOLODO, "LOD_Entity", "Name" );

   // Make the operator specify a unique Entity name, if the entity name
   // selected is not unique already.

   nRC = CreateViewFromViewForTask( &vTZZOLOD1, vTZZOLODO, 0 );
   nRC = GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOLODO, "LOD_Entity", "Name" );
   if (szEntityName[ 0 ] != 0 )
   {
      nRC = SetCursorFirstEntityByString( vTZZOLOD1, "LOD_Entity", "Name", szEntityName,"" );

      if ( nRC >= zCURSOR_SET )
         nRC = SetCursorNextEntityByString( vTZZOLOD1, "LOD_Entity", "Name", szEntityName,"" );
      while ( nRC >= zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Object Entity Name is not unique. Please reenter: " );
         nRC = MessagePromptForInput( vSubtask, "ZO00149",
                                      "Zeidon Objects Maintenance",
                                      szMsg, zBEEP,
                                      szEntityName, 32 );
         // Return if the Operator Cancels the prompt.
         if ( nRC == zRESPONSE_NO )
         {
            DeleteEntity( vTZZOLODO, "LOD_Entity", zREPOS_AFTER );
            return( 0 );
         }

         nRC = SetCursorFirstEntityByString( vTZZOLOD1, "LOD_Entity", "Name", szEntityName,"" );
      }
   }

   SetAttributeFromString( vTZZOLODO, "LOD_Entity", "Name", szEntityName );
   DropView ( vTZZOLOD1 );

   // Reset Ident Name
   nRC = GetStringFromAttribute( szEntityNameIndent, sizeof( szEntityNameIndent ), vTZZOLODO, "LOD_Entity", "IndentName" );
   nRC = GetIntegerFromAttribute( &lIndentLvl, vTZZOLODO, "LOD_Entity", "IndentLvl" );
   strcpy_s( szEntityNameIndent + lIndentLvl, sizeof( szEntityNameIndent ) - lIndentLvl, szEntityName );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Entity", "IndentName", szEntityNameIndent );

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) == zLEVEL_TASK )
   {
      zVIEW vW;

   // DropObjectInstance( vTZZOHDGO );
      oTZZOHDGO_CreateFromLOD( vSubtask, &vTZZOHDGO, vTZZOLODO, ORIENT );
      SetNameForView( vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );

      GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
      RefreshCtrl( vW, "HierDiag1" );
      if ( !HD_IsNodeVisible( vW, "HierDiag1" ) )
         HD_CenterNode( vW, "HierDiag1" );
   }

   RefreshAllSubwindows( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_SelectEntConstraint
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SelectEntConstraint( zVIEW vSubtask )
{
   zVIEW vMETAOBJECT;
   zSHORT nRC;

   //  Get Access to LOD Object
   nRC = GetViewByName( &vMETAOBJECT, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vMETAOBJECT ) )
   {
      nRC = CheckExistenceOfEntity( vMETAOBJECT, "LOD_EntityConstraintOper" );
      if ( nRC <  zCURSOR_SET )
      {
         SetAttributeFromString( vMETAOBJECT, "LOD_Entity",
                                 "CreateConstraint", "" );
         RefreshCtrl( vSubtask, "CheckCrConstraint" );
         SetCtrlState( vSubtask, "CheckCrConstraint", zCONTROL_STATUS_ENABLED, 0 );
         SetAttributeFromString( vMETAOBJECT, "LOD_Entity", "DeleteConstraint", "" );
         RefreshCtrl( vSubtask, "CheckDelConstraint" );
         SetCtrlState( vSubtask, "CheckDelConstraint", zCONTROL_STATUS_ENABLED, 0 );
         SetAttributeFromString( vMETAOBJECT, "LOD_Entity", "IncludeConstraint", "" );
         RefreshCtrl( vSubtask, "CheckIncConstraint" );
         SetCtrlState( vSubtask, "CheckIncConstraint", zCONTROL_STATUS_ENABLED, 0 );
         SetAttributeFromString( vMETAOBJECT, "LOD_Entity", "ExcludeConstraint", "" );
         RefreshCtrl( vSubtask, "CheckExcConstraint" );
         SetCtrlState( vSubtask, "CheckExcConstraint", zCONTROL_STATUS_ENABLED, 0 );
         SetAttributeFromString( vMETAOBJECT, "LOD_Entity", "AcceptConstraint", "" );
         RefreshCtrl( vSubtask, "CheckAccConstraint" );
         SetCtrlState( vSubtask, "CheckAccConstraint", zCONTROL_STATUS_ENABLED, 0 );
         SetAttributeFromString( vMETAOBJECT, "LOD_Entity", "CancelConstraint", "" );
         RefreshCtrl( vSubtask, "CheckCanConstraint" );
         SetCtrlState( vSubtask, "CheckCanConstraint", zCONTROL_STATUS_ENABLED, 0 );
         SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 0 );
      }
      else
      {
         SetCtrlState( vSubtask, "CheckCrConstraint", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "CheckDelConstraint", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "CheckIncConstraint", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "CheckExcConstraint", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "CheckAccConstraint", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "CheckCanConstraint", zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 1 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_PreBuildATTVIEW
// PURPOSE:    This Entry names a view for the Attribute
//             modeless subwindow in the LOD Dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PreBuildATTVIEW( zVIEW vSubtask )
{
   zVIEW vATTVIEW;

   // See if Modal Window already active
   GetSubtaskForWindowName( vSubtask, &vATTVIEW, "ATTVIEW" );
   if ( vATTVIEW )
   {
      ActivateWindow( vATTVIEW );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_PostBuildATTVIEW
// PURPOSE:    This Entry sets the caption for the Attribute
//             modeless subwindow in the LOD Dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildATTVIEW( zVIEW vSubtask )
{
   //zVIEW  vTZZOLODO;
   //zCHAR  szCaption[60];

   // Get Access to LOD Object
   //GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //GetStringFromAttribute( szCaption, sizeof( szCaption ), vTZZOLODO, "LOD_Entity", "Name" );
   //strcat_s( szCaption, sizeof( szCaption ), " Attributes" );
   //SetWindowCaptionTitle( vSubtask, 0, szCaption );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_PreBuildATTMAINT
// PURPOSE:   Build DomainList if not previously built.
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: PreBuild
//    NAME: PreBuildATTMAINT
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PreBuildATTMAINT( zVIEW vSubtask )
{
   zVIEW    vDomainList;
   zVIEW    vLOD;
   zVIEW    vLODCopy;
// zVIEW    vTZZOLOD1;
   zVIEW    vWindow;
   zSHORT   nRC;

   // Get Access to Domain List
   nRC = RetrieveViewForMetaList( vSubtask, &vDomainList, zREFER_DOMAIN_META );
   nRC = SetNameForView( vDomainList, "DomainList", vSubtask, zLEVEL_TASK );

   nRC = GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC == zLEVEL_TASK )
   {
      SetSelectSetForView( vLOD, 1 );
      // Attribute List is a multi selection list, delesect all Attributes
      CreateViewFromViewForTask( &vLODCopy, vLOD, 0 );
      for ( nRC = SetCursorFirstSelectedEntity( vLOD, "LOD_Attribute", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vLOD, "LOD_Attribute", "" ) )
      {
         nRC =SetSelectStateOfEntity( vLOD, "LOD_Attribute", 0 );
      }

      SetCursorFirstEntityByAttr( vLOD, "LOD_Attribute", "ZKey",
                                  vLODCopy, "LOD_Attribute", "ZKey", "" );
      DropView( vLODCopy );
      SetSelectStateOfEntity( vLOD, "LOD_Attribute", 1 );

      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ATTVIEW" ) >= 0 )
         RefreshWindow( vWindow );
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" ) >= 0 )
         RefreshWindow( vWindow );

      // if Entity Type = Work, set type Attribute to Work
      if ( ComponentIsCheckedOut( vSubtask, vLOD, zSOURCE_LOD_META ) &&
           CompareAttributeToString( vLOD, "LOD_Entity", "Work", "Y" ) == 0 )
      {
         SetAttributeFromString( vLOD, "LOD_Attribute", "Work", "Y" );
      }

      // Build select set 4 in vLOD for derived operations
      SetSelectSetForView( vLOD, 4 );
      SetAllSelectStatesForEntity( vLOD, "Operation", 0, 0 );
      nRC = SetCursorFirstEntityByString( vLOD, "Operation", "Type", "A", "LOD" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         nRC = SetSelectStateOfEntity( vLOD, "Operation", 1 );
         nRC = SetCursorNextEntityByString( vLOD, "Operation", "Type", "A", "LOD" );
      }

      SetSelectSetForView( vLOD, 1 );
   }

   // Create additional TZZOLODO view.
   /*if ( GetViewByName( &vTZZOLOD1, "TZZOLOD1", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTZZOLOD1 );
   CreateViewFromViewForTask( &vTZZOLOD1, vLOD );
   SetNameForView( vTZZOLOD1, "TZZOLOD1", vSubtask, zLEVEL_TASK );*/

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SelectAttrDerOper( zVIEW vSubtask )
{
   zVIEW    vSOURCE;
   GetViewByName( &vSOURCE, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // En-/Disable EditButton
   if ( CheckExistenceOfEntity( vSOURCE, "LOD_AttrDerivationOper" ) != 0 )
      SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 0 );
   else
      SetCtrlState( vSubtask, "EditPush", zCONTROL_STATUS_ENABLED, 1 );

   return( 0 );
}


static zSHORT
zwfnTZZOLODD_ATTMAINT_SetCtrlState( zVIEW  vSubtask,
                                    zVIEW  vTZZOLODO,
                                    zSHORT nEnable,
                                    zSHORT IsCheckedOut )
{
   SetCtrlState( vSubtask, "Name", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "PPLit", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "PPLth", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Desc", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "LPLit", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "LPLth", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "NotNull", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CaseSens", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Alth", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DomainText", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DomainCombo", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DLth", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DomDText", zCONTROL_STATUS_ENABLED, nEnable );

   if ( IsCheckedOut == 1 )
   {
      if ( CompareAttributeToString( vTZZOLODO, "LOD_Attribute", "Update", "" ) == 0 )
      {
         SetAttributeFromString( vTZZOLODO, "LOD_Attribute", "Update", "Y" );
      }

      if ( CompareAttributeToString( vTZZOLODO, "LOD_Attribute", "SeqOrder", "" ) == 0 )
      {
         SetAttributeFromString( vTZZOLODO, "LOD_Attribute", "SeqOrder", "A" );
      }
   }
   else
   {
      SetCtrlState( vSubtask, "DerivedF", zCONTROL_STATUS_ENABLED, 0 );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_PostBuildATTMAINT
// PURPOSE:   Protect ER criteria on LOD Attribute's if Attribute is
//            ER based.  Do not protect Work Attributes.
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: PostBuild
//    NAME: PostBuildATTMAINT
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildATTMAINT( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zSHORT nEnable = 0;
   zSHORT IsCheckedOut = 0;
   zCHAR  szEntityName[ 33 ];

   //  Set the select set for the derived attribute combo box
   CB_SetDisplaySelectSet( vSubtask, "DerivedF", 4 );

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   IsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META );

   // Check to see if LOD Attribute is Work
   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute") == zCURSOR_SET )
   {
      if ( CompareAttributeToString( vTZZOLODO, "LOD_Attribute", "Work", "Y" ) != 0 )
      {
         SetCtrlState( vSubtask, "DomainCombo", zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "DomainText", zCONTROL_STATUS_VISIBLE, 1 );
         nEnable = 0;
      }
      else
      {
         SetCtrlState( vSubtask, "DomainCombo", zCONTROL_STATUS_VISIBLE, 1 );
         SetCtrlState( vSubtask, "DomainText", zCONTROL_STATUS_VISIBLE, 0 );
         if ( IsCheckedOut == 1 )
            nEnable = 1;
      }

      zwfnTZZOLODD_ATTMAINT_SetCtrlState( vSubtask, vTZZOLODO, nEnable, IsCheckedOut );
   }

   // En-/Disable EditButton
   zwTZZOLODD_SelectAttrDerOper( vSubtask );

   //  Set Entity Name on caption
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOLODO, "LOD_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szEntityName );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_PreBuildENTMAINT
// PURPOSE:   Create Temporal Subobject Verstion, if this is update, and not new.
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: PreBuild
//    NAME: PreBuildATTMAINT
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PreBuildENTMAINT( zVIEW vSubtask )
{
   zVIEW vLOD;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // If this is a root entity AND
   //        there is a POD with a Multiple Root Limit,
   // then set the MultipleChildLimit from the POD and turn it off in the POD.
   if ( ComponentIsCheckedOut( vSubtask, vLOD, zSOURCE_LOD_META ) &&
        CompareAttributeToAttribute( vLOD, "LOD_Entity", "ZKey",
                                     vLOD, "LOD_EntityParent", "ZKey" ) == 0 &&
        CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET &&
        CompareAttributeToString( vLOD, "POD", "MULTI_ROOT_LIMIT", "" ) != 0 )
   {
      SetAttributeFromAttribute( vLOD, "LOD_Entity", "MultipleChildLimit",
                                 vLOD, "POD", "MULTI_ROOT_LIMIT" );
      SetAttributeFromString( vLOD, "POD", "MULTI_ROOT_LIMIT", "" );
   }

   // If this is an update of a LOD_Entity, instead of a new one, we
   // will create a temporal subobject version of LOD_Entity.
   // We will determine that it is an update by checking the Name attribute.
   // A null Name value must be a new entity.

   if ( CompareAttributeToString( vLOD, "LOD_Entity", "Name", "" ) != 0 )
   {
      CreateTemporalSubobjectVersion( vLOD, "LOD_Entity" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PreBuildLOD( zVIEW vSubtask )
{
   zVIEW vLOD;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // If the MULTI_ROOT_LIMIT of the POD is set, then set the MultipleChildLimit
   // from the POD and turn off the MULTI_ROOT_LIMIT in the POD.
   if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET &&
        CompareAttributeToString( vLOD, "POD", "MULTI_ROOT_LIMIT", "" ) != 0 )

   {
      SetAttributeFromAttribute( vLOD, "LOD_Entity", "MultipleChildLimit",
                                 vLOD, "POD",        "MULTI_ROOT_LIMIT" );
      SetAttributeFromString( vLOD, "POD", "MULTI_ROOT_LIMIT", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddAttribute
// PURPOSE:   Add a Work Attribute to a given LOD Entity
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddAtt
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddAttribute( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zVIEW vATTMAINT;
   zSHORT nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Accept the current Attribute subobject.
   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
   {
      if ( AcceptSubobject( vTZZOLODO, "LOD_Attribute" ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         SetFocusToCtrl( vSubtask, "Name" );
         return( -1 );
      }
      SetSelectSetForView( vTZZOLODO, 1 );
      SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 0 );
   }

   SetSelectSetForView( vTZZOLODO, 1 );

   // Create the LOD Attribute after its current position.
   nRC = CreateTemporalMetaEntity( vSubtask, vTZZOLODO, "LOD_Attribute", zPOS_AFTER );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Attribute", "Work", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Attribute", "Update", "Y" );
   nRC = SetAttributeFromString( vTZZOLODO, "LOD_Attribute", "SeqOrder", "Y" );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 1 );

   nRC = CreateMetaEntity( vSubtask, vTZZOLODO, "ER_Attribute", zPOS_AFTER );
   //   nRC = SetAttributeFromString( vTZZOLODO, "ER_Attribute", "Name", "New" );

   // Get Access to LOD Attribute Detail Window and set initial state
   GetSubtaskForWindowName( vSubtask, &vATTMAINT, "ATTMAINT" );
   if ( vATTMAINT )
   {
      zwTZZOLODD_PreBuildATTMAINT( vATTMAINT );
      nRC = zwTZZOLODD_PostBuildATTMAINT( vATTMAINT );
   }

   // Set the cursor focus to first edit box on window.
   SetFocusToCtrl( vSubtask, "Name" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddDerivedAttribute
// PURPOSE:   Add a Derived Attribute to a given LOD Entity
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddAtt
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddDerivedAttribute( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Add a Derived Attribute, which looks like a work entity from the ER with an Expression.
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vTZZOLODO, "LOD_Attribute", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vTZZOLODO, "ER_Attribute", zPOS_AFTER );
   SetAttributeFromString( vTZZOLODO, "LOD_Attribute", "Work", "Y" );
   SetAttributeFromString( vTZZOLODO, "ER_Attribute", "Work", "Y" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddSelectedAttribsL
// PURPOSE:   Create the list of ER Attributes for selective add.
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddSelectedAttribsL( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZEREMDO;
   zVIEW  vTZZOMFLO;

   zLONG  lTgtZKey = 0;
   zSHORT nRC;

   // Get Access to LOD & ER Objects
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vTZZOMFLO, zREFER_ERD_META );
      oTZEREMDO_GetRefViewForER( vSubtask, &vTZEREMDO, zCURRENT_OI );
      SetNameForView( vTZEREMDO, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
      DropView( vTZZOMFLO );
   }

   SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Position on corresponding ER_Entity in model. If not there, tell
   // operator this is work entity.
   if ( CheckExistenceOfEntity( vTZZOLODO, "ER_Entity" ) < zCURSOR_SET ||
        SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey",
                                    vTZZOLODO, "ER_Entity", "ZKey", 0 ))
   {
      MessageSend( vSubtask, "ZO00173", "Zeidon Object Maintenance",
                   "A Work Entity does not have ER Attributes.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddSelectedAttribs
// PURPOSE:   Add the selected ER attributes as LOD Attributes
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddSelectedAttribs( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZZOLOD1;
   zVIEW  vTZZOLOD2;
   zVIEW  vTZEREMDO;
   zLONG  lSrcZKey;
   zLONG  lTgtZKey = 0;
   zSHORT nRC;
   zUSHORT lLevel;
   zCHAR  szEntityName[ 33 ];

   // Get Access to LOD & ER Objects ... don't change position on TZZOLODO!
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );

   // Position on the Recursive side for the current LOD Entity
   GetIntegerFromAttribute( &lSrcZKey, vTZZOLODO, "LOD_Entity", "ZKey" );
   CreateViewFromViewForTask( &vTZZOLOD1, vTZZOLODO, 0 );
   SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityParent", "" );
   DefineHierarchicalCursor( vTZZOLOD1, "LOD" );
   while ( lSrcZKey != lTgtZKey )
   {
      nRC = SetCursorNextEntityHierarchical( &lLevel,
                                             szEntityName, vTZZOLOD1 );
      if ( nRC >= zCURSOR_SET_RECURSIVECHILD )
      {
         nRC = SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
         GetIntegerFromAttribute( &lTgtZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
      }

      if ( zstrcmp( szEntityName, "LOD_EntityParent" ) == 0 )
      {
         GetIntegerFromAttribute( &lTgtZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
      }
   }

   DropHierarchicalCursor( vTZZOLOD1 );
   CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLODO, 0 );

   // Position on the same Attribute in vTZZOLOD1 that has current
   // position in vTZZOLODO. This allows us to create new Attributes at that point.
   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vTZZOLOD1, "LOD_AttributeRec", "ZKey",
                                  vTZZOLODO, "LOD_Attribute", "ZKey", 0 );
   }

   // Loop through all selected ER Attributes and add them as LOD Attributes
   // if they are not already there.
   nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // Make sure the Attribute is not already there.
      if ( SetCursorFirstEntityByAttr( vTZZOLOD2, "ER_Attribute", "ZKey",
                                       vTZEREMDO, "ER_Attribute", "ZKey",
                                       "LOD_Entity" ) < zCURSOR_SET )
      {
         // Create the LOD Attribute on the Recursive Side
         CreateMetaEntity( vSubtask, vTZZOLOD1, "LOD_AttributeRec", zPOS_AFTER );
         SetAttributeFromString( vTZZOLOD1, "LOD_AttributeRec",
                                 "Update", "Y" );

         // Include the ER Attribute on the Recursive Side for the LOD Attribute
         IncludeSubobjectFromSubobject( vTZZOLOD1, "ER_AttributeRec",
                                        vTZEREMDO, "ER_Attribute",
                                        zPOS_AFTER );

         // set new Attribute Sequence
         SetCursorFirstEntityByAttr( vTZZOLOD2, "LOD_Entity", "ZKey",
                                     vTZZOLOD1, "LOD_EntityParent", "ZKey",
                                     "LOD" );
         SetCursorFirstEntityByAttr( vTZZOLOD2, "ER_Attribute", "ZKey",
                                     vTZZOLOD1, "ER_AttributeRec", "ZKey",
                                     "LOD_Entity" );
         oTZZOLODO_NewAttributeSequence( vTZZOLOD2 );

         // Deselect the ER_Entity.
         SetSelectStateOfEntity( vTZEREMDO, "ER_Attribute", 0 );
      }

      nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Attribute", 0 );
   }

   DropView( vTZZOLOD1 );
   DropView( vTZZOLOD2 );

   return( 0 );
}

static zSHORT
zwfnTZZOLODD_AddAllER_Attribs( zVIEW vSubtask,
                               zVIEW vTZZOLODO,
                               zVIEW vTZZOLOD1,
                               zVIEW vTZEREMDO_REF )
{
   zVIEW  vTZZOLOD2;
   zVIEW  vTZZOLOD3;
   zLONG  lZKey;
   zSHORT nRC;

   // Check to see if the Entity is a Work Entity
   if ( CompareAttributeToString( vTZZOLOD1, "LOD_EntityParent", "Work", "Y" ) != 0 )
   {
      CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
      CreateViewFromViewForTask( &vTZZOLOD3, vTZZOLODO, 0 );

      // Position on the same Attribute in vTZZOLOD1 that has current
      // position in vTZZOLODO. This allows us to create new Attributes at
      // that point.
      if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
         SetCursorFirstEntityByAttr( vTZZOLOD1, "LOD_AttributeRec", "ZKey",
                                     vTZZOLODO, "LOD_Attribute",    "ZKey", 0 );

      GetIntegerFromAttribute( &lZKey, vTZZOLOD1, "ER_EntityRec", "ZKey" );
      nRC = SetCursorFirstEntityByInteger ( vTZEREMDO_REF, "ER_Entity", "ZKey", lZKey, "" );
      nRC = SetCursorFirstEntity( vTZEREMDO_REF, "ER_Attribute", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         GetIntegerFromAttribute( &lZKey, vTZEREMDO_REF, "ER_Attribute", "ZKey" );
         nRC = SetCursorFirstEntityByInteger ( vTZZOLOD2, "ER_AttributeRec",
                                               "ZKey", lZKey, "LOD_EntityParent" );
         if ( nRC < zCURSOR_SET )
         {
            // Create the LOD Attribute on the Recursive Side
            nRC = CreateMetaEntity( vSubtask, vTZZOLOD1, "LOD_AttributeRec", zPOS_AFTER );
            nRC = SetAttributeFromString( vTZZOLOD1, "LOD_AttributeRec",
                                          "Update", "Y" );

            // Include the ER Attribute on the Recursive Side for the LOD Attribute.
            nRC = IncludeSubobjectFromSubobject( vTZZOLOD1, "ER_AttributeRec",
                                                 vTZEREMDO_REF, "ER_Attribute",
                                                 zPOS_AFTER );

            // set new Attribute Sequence
            SetCursorFirstEntityByAttr( vTZZOLOD3, "LOD_Entity", "ZKey",
                                        vTZZOLOD1, "LOD_EntityParent", "ZKey",
                                       "LOD" );
            SetCursorFirstEntityByAttr( vTZZOLOD3, "ER_Attribute", "ZKey",
                                        vTZZOLOD1, "ER_AttributeRec", "ZKey",
                                       "LOD_Entity" );
            oTZZOLODO_NewAttributeSequence( vTZZOLOD3 );
         }

         nRC = SetCursorNextEntity( vTZEREMDO_REF, "ER_Attribute", "" );
      }

      DropView( vTZZOLOD2 );
      DropView( vTZZOLOD3 );
   }

   return( 0 );
} // zwfnTZZOLODD_AddAllER_Attribs

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_AddAllER_Attribs
// PURPOSE:   Add all ER Attributes to a given LOD Entity
// TRIGGERED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddAtt
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddAllER_Attribs( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZZOLOD1;
   zVIEW  vTZEREMDO_REF;
   zVIEW  vTZZOMFLO;
   zSHORT nRC;
   zUSHORT lLevel;
   zCHAR  szEntityName[ 33 ];
   zLONG  lSrcZKey;
   zLONG  lTgtZKey = 0;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Check to see if the Entity is a Work Entity
   if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00173", "Zeidon Object Maintenance",
                   "A Work Entity does not have ER Attributes.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Get Access to ER Object
   nRC = GetViewByName( &vTZEREMDO_REF, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vTZZOMFLO, zREFER_ERD_META );
      oTZEREMDO_GetRefViewForER( vSubtask, &vTZEREMDO_REF, zCURRENT_OI );
      SetNameForView( vTZEREMDO_REF, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
      DropView( vTZZOMFLO );
   }

   // Position on the Recursive side for the current LOD Entity
   GetIntegerFromAttribute( &lSrcZKey, vTZZOLODO, "LOD_Entity", "ZKey" );
   CreateViewFromViewForTask( &vTZZOLOD1, vTZZOLODO, 0 );
   SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityParent", "" );
   DefineHierarchicalCursor( vTZZOLOD1, "LOD" );
   while ( lSrcZKey != lTgtZKey )
   {
      nRC = SetCursorNextEntityHierarchical( &lLevel,
                                             szEntityName, vTZZOLOD1 );
      if ( nRC >= zCURSOR_SET_RECURSIVECHILD )
      {
         nRC = SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
         GetIntegerFromAttribute( &lTgtZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
      }

      if ( zstrcmp( szEntityName, "LOD_EntityParent" ) == 0 )
         GetIntegerFromAttribute( &lTgtZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
   }

   DropHierarchicalCursor( vTZZOLOD1 );

   // Add all ER Entities
   zwfnTZZOLODD_AddAllER_Attribs( vSubtask, vTZZOLODO, vTZZOLOD1, vTZEREMDO_REF );

   DropView( vTZZOLOD1 );

   nRC = SetCursorFirstEntity( vTZZOLODO, "LOD_Attribute", "" );
   nRC = SetCursorFirstEntity( vTZZOLODO, "ER_Attribute", "" );

   // Refresh Attribute List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ParseDerivedExpr( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( oTZZOLODO_ParseDerivedExpr( vTZZOLODO, vSubtask ) < 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow,
                               "TZZOLODQ", "ShowParseErrors" );


   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DeleteAttrib
// PURPOSE:   Delete an selected LOD Attributes
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteAttrib( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_LOD,
                      zENAME_LOD_ATTRIBUTE,
                      vTZZOLODO, "ER_Attribute", "Name", 0 ) == zRESPONSE_YES )
   {
      DeleteEntity( vTZZOLODO, "LOD_Attribute", zREPOS_NONE );

      if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) == zCURSOR_NULL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      }
      else
      {
         SetCursorPrevEntity( vTZZOLODO, "LOD_Attribute", "" );
         CreateTemporalSubobjectVersion( vTZZOLODO, "LOD_Attribute" );
         zwTZZOLODD_PreBuildATTMAINT( vSubtask );
         zwTZZOLODD_PostBuildATTMAINT( vSubtask );
         SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 1 );
         SetFocusToCtrl( vSubtask, "Name" );
      }

      // Refresh Attribute and Entity List Window
      zwTZZOLODD_RefreshAttributeList( vSubtask );
      zwTZZOLODD_RefreshEntityList( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_DeleteSelectedAttrib
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_DeleteSelectedAttrib( zVIEW vSubtask )
{
   zVIEW   vTZZOLODO;
   zSHORT  nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( MessagePrompt( vSubtask, "", "Zeidon Objects Maintenance",
                       "OK to delete selected Entity Attributes", 0,
                       zBUTTONS_YESNO, zRESPONSE_NO,
                       zICON_QUESTION ) == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vTZZOLODO, "LOD_Attribute", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vTZZOLODO, "LOD_Attribute", "" ) )
      {
         DeleteEntity( vTZZOLODO, "LOD_Attribute", zREPOS_NONE );
      }

      // Refresh Attribute and Entity List Window
      zwTZZOLODD_RefreshAttributeList( vSubtask );
      zwTZZOLODD_RefreshEntityList( vSubtask );
   }

   return( 0 );
} // zwTZZOLODD_DeleteSelectedAttrib

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DeleteAllAttrib
// PURPOSE:   Delete all LOD Attributes for an Entity
// TRIGGERED BY:
//    WINDOW: ATTMAINT
//    ACTION: BUTTON
//    NAME: DelAttr
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteAllAttrib( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zSHORT nRC;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   //BL, 1999.10.24  Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "Zeidon Objects Maintenance",
                        "OK to delete all Entity Attributes", 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      //  Loop and Delete all LOD Attributes
      nRC = SetCursorFirstEntity( vTZZOLODO, "LOD_Attribute", "" );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = DeleteEntity( vTZZOLODO, "LOD_Attribute", zREPOS_NONE );
         nRC = SetCursorNextEntity( vTZZOLODO, "LOD_Attribute", "" );
      }
   }

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_SortAttByName
// PURPOSE:   Sort LOD Attributes by Name
// TRIGGERED BY:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SortAttByName( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zSHORT nRC;

   nRC = GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent4( vTZZOLODO, "ER_Attribute",
                                    "Name", zASCENDING,
                                    "", 0, "", 0, "", 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_IncludeDomain
// PURPOSE:   Include Domain for Work LOD Attribute
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_IncludeDomain( zVIEW vSubtask )
{
   zVIEW    vDomainList;
   zVIEW    vTempList;
   zVIEW    vDomain;
   zVIEW    vLOD;
   zSHORT   nRC;
   zCHAR    szAttributeName[ 33 ];
   zCHAR    szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Get Access to ER Object
   nRC = GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access LOD Object." );
      MessageSend( vSubtask, "ZO00161", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Get Access to Domain List
   nRC = GetViewByName( &vDomainList, "DomainList", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access Domain List." );
      MessageSend( vSubtask, "ZO00162", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // Get Access to SA Object
   // Use TempList, because ActivateMetaOI for a Domain alters the position
   // of the passed meta list.
   CreateViewFromViewForTask( &vTempList, vDomainList, 0 );
   nRC = ActivateMetaOI( vSubtask, &vDomain, vTempList, zREFER_DOMAIN_META,
                         zSINGLE | zLEVEL_APPLICATION );
   DropView( vTempList );
   if ( nRC < 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Unable to Access Domain to be included." );
      MessageSend( vSubtask, "ZO00163", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   if ( CheckExistenceOfEntity( vLOD, "Domain" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vLOD, "Domain", zREPOS_NONE );
   }

   nRC = IncludeSubobjectFromSubobject( vLOD, "Domain", vDomain, "Domain", zPOS_AFTER );

   // If the AttributeName is spaces, initialize it with the DomainName.
   GetCtrlText( vSubtask, "Name", szAttributeName, 33 );
   if ( szAttributeName[ 0 ] == 0 )
   {
      SetAttributeFromAttribute( vLOD, "ER_Attribute", "Name", vLOD, "Domain", "Name" );
      RefreshCtrl( vSubtask, "Name" );
   }

   RefreshCtrl( vSubtask, "MaxStrLth" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ProfileSave
// PURPOSE:   Save updated Profile.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ProfileSave( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "ZO", vSubtask, zCURRENT_OI );
   oTZ__PRFO_CommitProfile( vProfileXFER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ProfileSaveandPrint
// PURPOSE:   Save updated Profile and Print Diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ProfileSaveAndPrint( zVIEW vSubtask )
{

   zwTZZOLODD_ProfileSave( vSubtask );
   zwTZZOLODD_HD_Print( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ZoomStandard
// PURPOSE:   Zoom 100 the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ZoomStandard( zVIEW    vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" ) >= 0 )
      HD_ZoomDiagram( vW, "HierDiag1", 100, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ZoomRead
// PURPOSE:   Zoom 100 and Center the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ZoomRead( zVIEW    vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" ) >= 0 )
   {
      HD_ZoomDiagram( vW, "HierDiag1", 100, 1 );
      HD_CenterNode( vW, "HierDiag1" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ZoomIn
// PURPOSE:   Zoom In on the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ZoomIn( zVIEW    vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" ) >= 0 )
      HD_ZoomDiagram( vW, "HierDiag1", 25, 2 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ZoomOut
// PURPOSE:   Zoom Out on the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ZoomOut( zVIEW    vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" ) >= 0 )
      HD_ZoomDiagram( vW, "HierDiag1", -25, 2 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ZoomFit
// PURPOSE:   Zoom to fit on the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ZoomFit( zVIEW    vSubtask )
{
   zVIEW vW;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" ) >= 0 )
      HD_ZoomDiagram( vW, "HierDiag1", 0, 3 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_SwitchOrientation
// PURPOSE:   Switch the orientation of the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SwitchOrientation( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zBOOL  bReadOnly = FALSE;
   zBOOL  bTZZOHDGO_Valid = FALSE;

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      CreateViewFromViewForTask( &vTZZOHDGO, vTZZOHDGO, 0 );
      while ( ResetViewFromSubobject( vTZZOHDGO ) == 0 )
      {
      }

      // Unset the readonly state, because RefreshAllSubwindows triggers an
      // update (because of xPos/yPos of vTZZOHDGO):
      if ( MiGetUpdateForView( vTZZOHDGO ) == 0 )
      {
         bReadOnly = TRUE;
         SetViewUpdate( vTZZOHDGO );
      }

      if ( CompareAttributeToInteger( vTZZOHDGO, "HD_Diagram", "Orientation", 0 ) == 0 )
      {
         SetAttributeFromInteger( vTZZOHDGO, "HD_Diagram", "Orientation", 1 );
      }
      else
      {
         SetAttributeFromInteger( vTZZOHDGO, "HD_Diagram", "Orientation", 0 );
      }

      if ( bReadOnly == TRUE )
         SetViewReadOnly( vTZZOHDGO );
   }

   HD_SetProperties( vSubtask, "HierDiag1", 2 );  // switch orientation

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( bReadOnly )
        SetViewUpdate( vTZZOHDGO );

      RefreshCtrl( vSubtask, "HierDiag1" );

      if ( bReadOnly )
         SetViewReadOnly( vTZZOHDGO );
   }

   HD_SelectNode( vSubtask, "HierDiag1" );
   if ( bTZZOHDGO_Valid )
      DropView( vTZZOHDGO );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DisplayClientMenu
// PURPOSE:   To Display the client menu on the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DisplayClientMenu( zVIEW    vSubtask )
{
   POINT  pt;

   HD_GetPointerCoordinates( vSubtask, "HierDiag1", &pt.x, &pt.y );
   CreateTrackingPopupMenu( vSubtask, "ClientMenu", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_DisplayNodeMenu
// PURPOSE:   To Display the node menu on the hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DisplayNodeMenu( zVIEW    vSubtask )
{
   POINT   pt;
   zVIEW   vTZZOLODO;
   zSHORT  nIsCheckedOut = 0;
   zBOOL   bOpen         = FALSE;
   zBOOL   bNew          = FALSE;
   zBOOL   bDelete       = FALSE;

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      zwTZZOLODD_GraphicToLOD( vSubtask );
      HD_GetPointerCoordinates( vSubtask, "HierDiag1", &pt.x, &pt.y );
      zwTZZOLODD_RefreshEntitySelect( vSubtask );

      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META );

      if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Entity" ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
      {
         bNew = TRUE;
      }

      EnableAction( vSubtask, "XferToENTMAINT", bOpen );
      EnableAction( vSubtask, "FromSARel", bNew );
      EnableAction( vSubtask, "FromSAEnt", bNew );
      EnableAction( vSubtask, "AddWorkEntity", bNew );
      EnableAction( vSubtask, "ReplaceRootEntity", bNew );
      EnableAction( vSubtask, "DeleteEntity", bDelete );
      EnableAction( vSubtask, "ActivateOrdering", bDelete );
      EnableAction( vSubtask, "ShowPriorityList", bOpen );
      EnableAction( vSubtask, "LoadAttributeList", bOpen );
      EnableAction( vSubtask, "MoveEntityLeft", bDelete );
      EnableAction( vSubtask, "MoveEntityRight", bDelete );

      CreateTrackingPopupMenu( vSubtask, "NodeMenu", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_CenterNode
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CenterNode( zVIEW    vSubtask )
{
   HD_CenterNode( vSubtask, "HierDiag1" );

   RefreshCtrl( vSubtask, "HierDiag1" );
   HD_SelectNode( vSubtask, "HierDiag1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ExpandNode
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ExpandNode( zVIEW    vSubtask )
{
   zVIEW vTZZOHDGO;

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CompareAttributeToString( vTZZOHDGO, "HD_Node", "Hidden", "Y" ) == 0 )
      {
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "Hidden", "N" );
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "Collapsed", "Y" );
      }
      else
      {
       if ( MiGetUpdateForView( vTZZOHDGO ) == 0)
            SetViewUpdate( vTZZOHDGO );

         SetAttributeFromString( vTZZOHDGO, "HD_Node", "Collapsed", "N" );
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "Hidden", "N" );
         if ( SetCursorFirstEntity( vTZZOHDGO, "HD_ChildNode", 0 ) >= zCURSOR_SET )
         {
            do
            {
               SetAttributeFromString( vTZZOHDGO, "HD_ChildNode", "Collapsed", "Y" );
               SetAttributeFromString( vTZZOHDGO, "HD_ChildNode", "Hidden", "N" );
            } while ( SetCursorNextEntity( vTZZOHDGO, "HD_ChildNode", 0 ) >= zCURSOR_SET );
         }
       if ( MiGetUpdateForView( vTZZOHDGO ) == 0)
            SetViewReadOnly( vTZZOHDGO );
      }
   }

   RefreshCtrl( vSubtask, "HierDiag1" );
   HD_SelectNode( vSubtask, "HierDiag1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_ExpandAllNodes
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
void
zfnTZZOLODD_RecursExpandNodes( zVIEW    vTZZOHDGO )
{
   zBOOL  bReadOnly = FALSE;

   if ( MiGetUpdateForView( vTZZOHDGO ) == 0 )
   {
      bReadOnly = TRUE;
      SetViewUpdate( vTZZOHDGO );
   }

   SetAttributeFromString( vTZZOHDGO, "HD_Node", "Collapsed", "N" );
   SetAttributeFromString( vTZZOHDGO, "HD_Node", "Hidden", "N" );

   if ( SetCursorFirstEntity( vTZZOHDGO, "HD_ChildNode", 0 ) >= zCURSOR_SET )
   {
      do
      {
         SetViewToSubobject( vTZZOHDGO, "HD_ChildNode" );
         zfnTZZOLODD_RecursExpandNodes( vTZZOHDGO );
         ResetViewFromSubobject( vTZZOHDGO );
      } while ( SetCursorNextEntity( vTZZOHDGO, "HD_ChildNode", 0 ) >= zCURSOR_SET );
   }

   if ( bReadOnly)
      SetViewReadOnly( vTZZOHDGO );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ExpandAllNodes( zVIEW    vSubtask )
{
   zVIEW vTZZOHDGO;
   zBOOL  bReadOnly = FALSE;

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      ResetView( vTZZOHDGO );
      zfnTZZOLODD_RecursExpandNodes( vTZZOHDGO );

     if ( MiGetUpdateForView( vTZZOHDGO ) == 0 )
     {
         bReadOnly = TRUE;
         SetViewUpdate( vTZZOHDGO );
     }

      RefreshCtrl( vSubtask, "HierDiag1" );

      if ( bReadOnly)
         SetViewReadOnly( vTZZOHDGO );

      HD_SelectNode( vSubtask, "HierDiag1" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_CollapseNode
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CollapseNode( zVIEW    vSubtask )
{
   zVIEW vTZZOHDGO;
   zBOOL  bReadOnly = FALSE;

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( MiGetUpdateForView( vTZZOHDGO ) == 0 )
      {
         bReadOnly = TRUE;
         SetViewUpdate( vTZZOHDGO );
     }

      SetAttributeFromString( vTZZOHDGO, "HD_Node", "Collapsed", "Y" );
      SetAttributeFromString( vTZZOHDGO, "HD_Node", "Hidden", "N" );
   }

   RefreshCtrl( vSubtask, "HierDiag1" );

   if ( bReadOnly == TRUE )
      SetViewReadOnly( vTZZOHDGO );

   HD_SelectNode( vSubtask, "HierDiag1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_HideNode
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HideNode( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;

   if ( GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromString( vTZZOHDGO, "HD_Node", "Hidden", "Y" );
      SetAttributeFromString( vTZZOHDGO, "HD_Node", "Collapsed", "N" );
   }

   RefreshCtrl( vSubtask, "HierDiag1" );
   HD_SelectNode( vSubtask, "HierDiag1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_HD_Print
// PURPOSE:   Print the HD Diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HD_Print( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vW;
   zLONG    lPct;
   zLONG    lOrient;
   zLONG    lAlgorithm;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "ZO", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lPct, vProfileXFER, "ZO", "ZO_PrintPercent" );
   GetIntegerFromAttribute( &lOrient, vProfileXFER, "ZO", "ZO_PrintOrientation" );
   GetIntegerFromAttribute( &lAlgorithm, vProfileXFER, "ZO", "ZO_PrintAlgorithm" );

   GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
   if ( HD_PrintDiagram( vW, "HierDiag1", (zSHORT) lPct, (zSHORT)(lOrient | lAlgorithm) ) > 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_HelpAbout
// PURPOSE:    This Entry does the About
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_LOD, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, "BasicsManual" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, "SystemHelp" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, "UsersGuide" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zfnTZZOLODD_MergeLOD_WithDiagram
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnRecursMergeLOD_WithDiagram( zVIEW vSubtask,
                              zVIEW vTZZOLODO,
                              zVIEW vTZZOHDGO,
                              zBOOL bCreate )
{
   zSHORT  nRC, nRC1;
   zVIEW   vWkTZZOHDGO;
   zLONG   lZKey;
   zSHORT  nTok;
   zSHORT  nSkipDelBehave;
   zSHORT  nOutText;
   zCHAR   szOutText[ 32 ];
   zCHAR   szMsg[ zLONG_MESSAGE_LTH + 1 ];

   if ( !bCreate &&
        CompareAttributeToAttribute( vTZZOLODO, "LOD_EntityParent", "ZKey",
                                     vTZZOHDGO, "HD_Node", "ZKey" ) != 0 )
   {
      GetIntegerFromAttribute( &lZKey, vTZZOLODO, "LOD_EntityParent", "ZKey" );
      CreateViewFromViewForTask( &vWkTZZOHDGO, vTZZOHDGO, 0 );
      if ( SetCursorFirstEntityByInteger( vWkTZZOHDGO, "HD_Node", "ZKey", lZKey, 0 ) == 0 )
      {
         while ( CompareAttributeToInteger( vTZZOHDGO, "HD_Node", "ZKey", lZKey ) != 0 )
         {
            DeleteEntity( vTZZOHDGO, "HD_Node", zREPOS_NEXT );
         }
      }
      else
         bCreate = 2;

      DropView( vWkTZZOHDGO );
   }

   if ( bCreate )
   {
      if ( bCreate == 2 )
      {
         CreateEntity( vTZZOHDGO, "HD_Node", zPOS_BEFORE );
         bCreate = 1;
      }
      else
         CreateEntity( vTZZOHDGO, "HD_Node", zPOS_AFTER );
   }

   SetAttributeFromAttribute( vTZZOHDGO, "HD_Node", "ZKey", vTZZOLODO, "LOD_EntityParent", "ZKey" );
   SetAttributeFromAttribute( vTZZOHDGO, "HD_Node", "Name", vTZZOLODO, "LOD_EntityParent", "Name" );
   SetAttributeFromAttribute( vTZZOHDGO, "HD_Node", "Text", vTZZOLODO, "LOD_EntityParent", "Name" );
   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Work", "Y" ) != 0 &&
        SetCursorFirstEntity( vTZZOLODO, "ER_EntityRec", 0 ) == zCURSOR_SET )
   {
      SetAttributeFromAttribute( vTZZOHDGO, "HD_Node", "DIL", vTZZOLODO, "ER_EntityRec", "Name" );
      SetAttributeFromAttribute( vTZZOHDGO, "HD_Node", "Purpose", vTZZOLODO, "ER_EntityRec", "Purpose" );
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Work", "Y" ) != 0 &&
        SetCursorFirstEntity( vTZZOLODO, "ER_RelLinkRec", 0 ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szMsg, sizeof( szMsg ), vTZZOLODO, "ER_RelLinkRec", "Name" );
      strcat_s( szMsg, sizeof( szMsg ), " (" );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), sizeof( szMsg ) - zstrlen( szMsg ),
                              vTZZOLODO, "ER_RelLinkRec", "CardMin" );
      strcat_s( szMsg, sizeof( szMsg ), "," );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), sizeof( szMsg ) - zstrlen( szMsg ),
                              vTZZOLODO, "ER_RelLinkRec", "CardMax" );
      strcat_s( szMsg, sizeof( szMsg ), ")" );
      SetAttributeFromString( vTZZOHDGO, "HD_Node", "Title", szMsg );
   }

#if 1
   SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText1", "" );
   SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText2", "" );
   SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText3", "" );
   SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText4", "" );
   SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText5", "" );
   // Gig, 10/10 set outside text attrs
   nOutText = 1;
   nSkipDelBehave = 0;
   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Work", "Y" ) == 0 )
   {
      SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText1", "Work" );
      nOutText = 2;
      nSkipDelBehave = 1;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Derived", "Y" ) == 0 )
   {
      SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText1", "Derived" );
      nOutText = 2;
      nSkipDelBehave = 1;
   }

   nTok = 0;
   szOutText[ 0 ] = 0;
   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Create", "Y" ) == 0 )
   {
      strcpy_s( szOutText, sizeof( szOutText ), "cr" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Update", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "up" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Delete", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "de" );
      nTok++;
   }

   if ( nTok )
   {
      if ( nOutText == 1 )
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText1", szOutText );
      else
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText2", szOutText );

      nOutText++;
   }

   nTok = 0;
   szOutText[ 0 ] = 0;
   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Include", "Y" ) == 0 )
   {
      strcpy_s( szOutText, sizeof( szOutText ), "inc" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Exclude", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "exc" );
      nTok++;
   }

// if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "InclSrc", "Y" ) == 0 )
// {
//    if ( nTok )
//       strcat_s( szOutText, sizeof( szOutText ), "," );
//
//    strcat_s( szOutText, sizeof( szOutText ), "isrc" );
//    nTok++;
// }

   if ( nTok )
   {
      if ( nOutText == 1 )
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText1", szOutText );
      else
      if ( nOutText == 2 )
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText2", szOutText );
      else
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText3", szOutText );

      nOutText++;
   }

   if ( !nSkipDelBehave )
   {
      if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "ParentDeleteBehave", "D" ) == 0 )
      {
         strcpy_s( szOutText, sizeof( szOutText ), "pdel" );
      }
      else
      if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "ParentDeleteBehave", "E" ) == 0 )
      {
         strcpy_s( szOutText, sizeof( szOutText ), "pexc" );
      }
      else
      {
         strcpy_s( szOutText, sizeof( szOutText ), "pres" );
      }

      if ( nOutText == 1 )
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText1", szOutText );
      else
      if ( nOutText == 2 )
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText2", szOutText );
      else
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText3", szOutText );

      nOutText++;
   }

   nTok = 0;
   szOutText[ 0 ] = 0;
   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "CreateConstraint", "Y" ) == 0 )
   {
      strcpy_s( szOutText, sizeof( szOutText ), "c" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "DeleteConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "d" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "IncludeConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "i" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "ExcludeConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "x" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "AcceptConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "a" );
      nTok++;
   }

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "CancelConstraint", "Y" ) == 0 )
   {
      if ( nTok )
         strcat_s( szOutText, sizeof( szOutText ), "," );

      strcat_s( szOutText, sizeof( szOutText ), "n" );
      nTok++;
   }

   if ( nTok )
   {
      if ( nOutText <= 2 )
      {
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText2", "cnstrnts:" );
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText3", szOutText );
      }
      else
      if ( nOutText == 3 )
      {
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText3", "cnstrnts:" );
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText4", szOutText );
      }
      else
      {
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText4", "cnstrnts:" );
         SetAttributeFromString( vTZZOHDGO, "HD_Node", "OutsideText5", szOutText );
      }
   }

#endif

   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Create", "Y" )
                                                                     == 0 )
   {
      SetAttributeFromInteger( vTZZOHDGO, "HD_Node", "Color", 0 );
   }
   else
   if ( CompareAttributeToString( vTZZOLODO, "LOD_EntityParent", "Include", "Y" )
                                                                     == 0 )
   {
      SetAttributeFromInteger( vTZZOHDGO, "HD_Node", "Color", 1 );
   }
   else
      SetAttributeFromInteger( vTZZOHDGO, "HD_Node", "Color", 2 );

   if ( SetCursorFirstEntity( vTZZOLODO, "LOD_EntityChild", 0 ) >= zCURSOR_SET )
   {
      for (;; )
      {
         SetViewToSubobject( vTZZOLODO, "LOD_EntityChild" );
         SetViewToSubobject( vTZZOHDGO, "HD_ChildNode" );
         if ( !bCreate && CheckExistenceOfEntity( vTZZOHDGO, "HD_Node" ) == zCURSOR_NULL )
         {
            bCreate = 1;
         }

         nRC = fnRecursMergeLOD_WithDiagram( vSubtask, vTZZOLODO, vTZZOHDGO, bCreate );
         ResetViewFromSubobject( vTZZOLODO );
         ResetViewFromSubobject( vTZZOHDGO );
         nRC1 = SetCursorNextEntity( vTZZOLODO, "LOD_EntityChild", 0 );
         if ( nRC1 < zCURSOR_SET )
         {
            if ( !bCreate )
            {
               while ( SetCursorNextEntity( vTZZOHDGO, "HD_ChildNode", 0 ) >= zCURSOR_SET )
               {
                  DeleteEntity( vTZZOHDGO, "HD_ChildNode", zREPOS_NONE );
               }
            }
            break;
         }
         else
         {
            if ( SetCursorNextEntity( vTZZOHDGO, "HD_ChildNode", 0 ) < zCURSOR_SET )
            {
               bCreate = 1;
            }
         }
      }
   }
   else
   if ( SetCursorFirstEntity( vTZZOHDGO, "HD_ChildNode", 0 ) >= zCURSOR_SET )
   {
      while ( DeleteEntity( vTZZOHDGO, "HD_ChildNode", zREPOS_NEXT ) >= zCURSOR_SET );
   }

   return( nRC );
}

zSHORT
zfnTZZOLODD_MergeLOD_WithDiagram( zVIEW vSubtask,
                                  zVIEW vTZZOLODO,
                                  zVIEW vTZZOHDGO )
{
   zSHORT  nRC;

   if ( !vTZZOLODO && GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      return( 0 );
   }

   if ( !vTZZOHDGO && GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      return( 0 );
   }

   CreateViewFromViewForTask( &vTZZOLODO, vTZZOLODO, 0 );
   CreateViewFromViewForTask( &vTZZOHDGO, vTZZOHDGO, 0 );
   ResetView( vTZZOLODO );
   ResetView( vTZZOHDGO );

   if (ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_ERD_META ))
      nRC = fnRecursMergeLOD_WithDiagram( vSubtask, vTZZOLODO, vTZZOHDGO, 0 );

   DropView( vTZZOLODO );
   DropView( vTZZOHDGO );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_PostBuildENTVIEW
// PURPOSE:    This Entry sets the caption for the Entity
//             modeless subwindow in the ER Dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildENTVIEW( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vProfileXFER;
   zVIEW  vWindow;
   zLONG  lPSDLG_X;
   zLONG  lPSDLG_Y;
   zLONG  lSZDLG_X;
   zLONG  lSZDLG_Y;
   zCHAR  szEntityName[ 33 ];

   SetNameForView( vSubtask, "ENTVIEW", vSubtask, zLEVEL_TASK );

   // Set Caption for Entity name
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOLODO, "LOD_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szEntityName );

   // Set Entity Selection in Graph Control
   GetSubtaskForWindowName( vSubtask, &vWindow, "TZZOLODG" );
   if ( vWindow )
      HD_SelectNode( vWindow, "HierDiag1" );

   // Size the window appropriateley
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vProfileXFER, "ZOENTVIEW" ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lSZDLG_X, vProfileXFER, "ZOENTVIEW", "SZDLG_X" );
      if ( lSZDLG_X )
      {
         GetIntegerFromAttribute( &lSZDLG_Y, vProfileXFER, "ZOENTVIEW", "SZDLG_Y" );
         GetIntegerFromAttribute( &lPSDLG_X, vProfileXFER, "ZOENTVIEW", "PSDLG_X" );
         GetIntegerFromAttribute( &lPSDLG_Y, vProfileXFER, "ZOENTVIEW", "PSDLG_Y" );
         SetCtrlPosition( vSubtask, 0, lPSDLG_X, lPSDLG_Y, TRUE );
         SetCtrlSize( vSubtask, 0, lSZDLG_X, lSZDLG_Y, TRUE );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_RefreshEntitySelect
// PURPOSE:    This Entry refreshes hierarchical diagram
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshEntitySelect( zVIEW vSubtask )
{
   zVIEW  vTZZOHDGO;
   zVIEW  vTZZOLODO;
   zVIEW  vW;
   zLONG  lZKey;

   GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Set position of the Hier Diagram to current Entity
   GetIntegerFromAttribute( &lZKey, vTZZOLODO, "LOD_Entity", "ZKey" );
   oTZZOHDGO_GraphicFromZKey( vSubtask, vTZZOHDGO, lZKey );

   GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" );
   if ( vW == vSubtask )
      RefreshCtrl( vSubtask, "AttrList" );

   GetSubtaskForWindowName( vSubtask, &vW, "TZZOLODG" );
   if ( vW )
   {
      HD_SelectNode( vW, "HierDiag1" );
      if ( !HD_IsNodeVisible( vW, "HierDiag1" ) )
         HD_CenterNode( vW, "HierDiag1" );
   }

   GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" );
   if ( vW )
      RefreshWindow( vW );

   RefreshWindowExceptForCtrl( vSubtask, "EntityListBox" );
// SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_CloseENTVIEW
// PURPOSE:    This Entry deletes the named view to the window upon close.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CloseENTVIEW( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zLONG  lDLG_X;
   zLONG  lDLG_Y;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "ZO", vSubtask, zCURRENT_OI );
   GetCtrlSize( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   if ( CheckExistenceOfEntity( vProfileXFER, "ZOENTVIEW" ) != zCURSOR_SET )
      CreateEntity( vProfileXFER, "ZOENTVIEW", zPOS_FIRST );

   SetAttributeFromInteger( vProfileXFER, "ZOENTVIEW", "SZDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "ZOENTVIEW", "SZDLG_Y", lDLG_Y );
   GetCtrlPosition( vSubtask, 0, &lDLG_X, &lDLG_Y, TRUE );
   SetAttributeFromInteger( vProfileXFER, "ZOENTVIEW", "PSDLG_X", lDLG_X );
   SetAttributeFromInteger( vProfileXFER, "ZOENTVIEW", "PSDLG_Y", lDLG_Y );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZZOLODD_EntityPopup
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EntityPopup( zVIEW vSubtask )
{
   zVIEW   vTZZOLODO;
   zSHORT  nIsCheckedOut = 0;
   zBOOL   bOpen         = FALSE;
   zBOOL   bNew          = FALSE;
   zBOOL   bDelete       = FALSE;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META );

      if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Entity" ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "NewEntityFromSubjectArea", bNew );
      EnableAction( vSubtask, "NewDerivedEntity", bNew );
      EnableAction( vSubtask, "NewWorkEntity", bNew );
      EnableAction( vSubtask, "DeleteEntity", bDelete );
      EnableAction( vSubtask, "ShowPriorityList", bOpen );

      CreateTrackingPopupMenu( vSubtask, "EntityPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AttPopup( zVIEW    vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "AttPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AttributePopup( zVIEW    vSubtask )
{
   zVIEW   vTZZOLODO;
   zVIEW   vWork;
   zSHORT  nIsCheckedOut = 0;
   zBOOL   bOpen         = FALSE;
   zBOOL   bPaste        = FALSE;
   zBOOL   bNew          = FALSE;
   zBOOL   bNewER_Attrib = FALSE;
   zBOOL   bDelete       = FALSE;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META );

      if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
      {
         bNew = TRUE;
         if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) != 0 )
            bNewER_Attrib = TRUE;

         if ( GetViewByName( &vWork, "CopyWork", vSubtask, zLEVEL_SYSTEM ) > 0 &&
              CheckExistenceOfEntity( vWork, "LOD_AttributeCopy" ) >= zCURSOR_SET )
            bPaste = TRUE;
      }

      EnableAction( vSubtask, "AddAttribute", bNew );
      EnableAction( vSubtask, "DeleteAttribute", bDelete );
      EnableAction( vSubtask, "DeleteAllAttrib", bDelete );
      EnableAction( vSubtask, "AddAllERAttributes", bNewER_Attrib );
      EnableAction( vSubtask, "AddSelectedER_AttributesList", bNewER_Attrib );
      EnableAction( vSubtask, "ShowPriorityList", bOpen );
      EnableAction( vSubtask, "CopyAttributes", bOpen );
      EnableAction( vSubtask, "PasteAttributes", bPaste );

      CreateTrackingPopupMenu( vSubtask, "AttributePopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_NotDone( zVIEW    vSubtask )
{
   MessageSend( vSubtask, "ZO00164", "Zeidon Object Maintenance",
                "The function you have requested has not been completed. "
                "Please notify Zeidon development",
                zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PrebuildNewLOD( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vT;
   zVIEW  vProfileXFER;
   zSHORT nRC;

   // Make sure the TZEREMDO object is active and named.
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vT, zREFER_ERD_META );
      if ( CheckExistenceOfEntity( vT, "W_MetaDef" ) >= zCURSOR_SET )
      {
         ActivateMetaOI( vSubtask, &vTZEREMDO, vT, zREFER_ERD_META,
                         zSINGLE | zLEVEL_APPLICATION );
         SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      }

      DropView( vT );
   }

   // Blank out the Name and Desc values in the Profile object so that
   // previous names won't be shown.
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vProfileXFER, "TZ" ) >= zCURSOR_SET )
      SetAttributeFromString( vProfileXFER, "TZ", "Name", "" );

   if ( CheckExistenceOfEntity( vProfileXFER, "ZO" ) >= zCURSOR_SET )
   {
      SetAttributeFromString( vProfileXFER, "ZO", "NewDesc", "" );
      // set Default Radio Button
      SetAttributeFromString( vProfileXFER, "ZO", "LOD_Type", "ER" );
   }

   // Make sure we're positioned on the first entry.
   SetCursorFirstEntity( vTZEREMDO, "ER_Entity", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_PreBuildSARELVIEW
// PURPOSE:    Position on the ER_Entity in the ERD that corresponds to the
//             ER_Entity in the LOD, which has current position.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PreBuildSARELVIEW( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZEREMDO;
   zVIEW  vT;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vT, zREFER_ERD_META );
      ActivateMetaOI( vSubtask, &vTZEREMDO, vT, zREFER_ERD_META,
                      zSINGLE | zLEVEL_APPLICATION );
      SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      DropView( vT );
   }

   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                    vTZZOLODO, "ER_Entity", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZZOLODD_PostBuildSARELVIEW
// PURPOSE:    Set caption.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PostBuildSARELVIEW( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zCHAR  szName[ 100 ];

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szName, sizeof( szName ), vTZZOLODO, "LOD_Entity", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szName );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_TransferToOperDLG( zVIEW vSubtask )
{
   zVIEW vTZZOLODO;
   zVIEW vWindow;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // If no SourceFile exists, create an initial SourceFile entity by the
   // same name as the LOD.
   if ( CheckExistenceOfEntity( vTZZOLODO, "SourceFile" ) < zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vTZZOLODO, "SourceFile", zPOS_AFTER );
      SetAttributeFromAttribute( vTZZOLODO, "SourceFile", "Name",
                                 vTZZOLODO, "LOD",        "Name" );
      SetAttributeFromString( vTZZOLODO, "SourceFile", "Extension", "VML" );
      SetAttributeFromString( vTZZOLODO, "SourceFile", "LanguageType", "V" );

      // Refresh Window <Source File List>
      GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" );
      if ( vWindow )
         RefreshWindow( vWindow );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_VerifyRelatedFunction( zVIEW vSubtask )
{
   // Make sure that the entity selected is an ER entity so that it is valid
   // to go to the subwindow for selection of related entities.

   zVIEW vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Entity" ) < zCURSOR_SET ||
        CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00166", "Zeidon Object Maintenance",
                   "Related Entity invalid for Work entity",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   // SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZZOLODQ", "TZZOLODG" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow,
                               "TZZOLODQ", "TZZOLODG" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PrintReport( zVIEW vSubtask )
{
   zVIEW  vLOD;

   // This routine prints a report on the current Subject Area in memory.

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   FormatSubobjectOnDoc( vLOD, "LOD", vSubtask, "LODRPT", 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZZOLODD_TransferToEditor
// PURPOSE:
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZZOLODD_TransferToEditor( zVIEW vSubtask )
{
   zVIEW  vMETAOPERATIONS;
   zVIEW  vT;

   //  Get view to main meta (LOD or Dialog) and source, which as of
   //  Release .9A is in the same object.
   GetViewByName( &vMETAOPERATIONS, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Check to see if a view with name "TZZOLODO" exists.  If it does, then
   // the editor is being invoked from the LOD dialog.  Otherwise it is being
   // invoked from the Dialog dialog.  Pass this info to the editor.
   if ( GetViewByName( &vT, "TZZOLODO", vSubtask, zLEVEL_TASK ) >= 0 )
      TransferToEditor( vSubtask, vMETAOPERATIONS, "ZO", 0 );
   else
      TransferToEditor( vSubtask, vMETAOPERATIONS, "WD", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_InvokeEditor( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zCHAR    szName[ 34 ];

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   GetViewByName( &vSOURCE, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "LOD_ConstraintOper", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "LOD" );

   // Now go set up for the editor.
   wTZZOLODD_TransferToEditor( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_InvokeEditor2( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zCHAR    szName[ 34 ];

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   GetViewByName( &vSOURCE, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "LOD_AttrDerivationOper", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "LOD" );

   // Now go set up for the editor.
   wTZZOLODD_TransferToEditor( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_InvokeEditor3( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zCHAR    szName[ 34 ];

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   GetViewByName( &vSOURCE, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "LOD_EntityConstraintOper", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName, "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName, "LOD" );

   // Now go set up for the editor.
   wTZZOLODD_TransferToEditor( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PrintPartialReport( zVIEW vSubtask )
{
   zVIEW  vLOD;

   // This routine prints a report on the current Subject Area in memory.

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   FormatSubobjectOnDoc( vLOD, "LOD", vSubtask, "LODPART", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_LocateRecursiveEntity( zVIEW vLOD,
                                  zLONG lLOD_EntityZKey )
{
   // Process the recursive side of the LOD to position on the same entity
   // as in the flat side of the LOD.

   zSHORT  nRC;

   nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent", "ZKey", lLOD_EntityZKey ) == 0 )
      {
         return( 1 );
      }

      // Process any children.
      SetViewToSubobject( vLOD, "LOD_EntityChild" );
      nRC = zwTZZOLODD_LocateRecursiveEntity( vLOD, lLOD_EntityZKey );
      if ( nRC == 1 )
         return( nRC );

      ResetViewFromSubobject( vLOD );
      nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ShowPriorityListSub( zVIEW  vLOD_R,
                                zVIEW  vWork,
                                zPLONG lpMaxPriority )
{
   zSHORT nRC;

   // Process attributes in child entities with max cardinality of 1.
   SetViewToSubobject( vLOD_R, "LOD_EntityChild" );
   for ( nRC = SetCursorFirstEntity( vLOD_R, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_R, "LOD_EntityParent", 0 ))
   {
      if ( CheckExistenceOfEntity( vLOD_R, "ER_RelLinkRec" ) >= zCURSOR_SET &&
           CompareAttributeToString( vLOD_R, "ER_RelLinkRec", "CardMax", "1" ) == 0 )
      {
         for ( nRC = SetCursorFirstEntity( vLOD_R, "LOD_AttributeRec", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vLOD_R, "LOD_AttributeRec", 0 ))
         {
            if ( CompareAttributeToInteger( vLOD_R, "LOD_AttributeRec", "SeqPriority", 0 ) != 0 )
            {
               CreateEntity( vWork, "SelectedAttribute", zPOS_AFTER );
               SetAttributeFromAttribute( vWork, "SelectedAttribute", "EntityName",
                                          vLOD_R,"LOD_EntityParent",  "Name" );
               SetAttributeFromAttribute( vWork, "SelectedAttribute", "AttributeName",
                                          vLOD_R,"ER_AttributeRec",   "Name" );
               SetAttributeFromAttribute( vWork, "SelectedAttribute", "PrioritySequence",
                                          vLOD_R,"LOD_AttributeRec",  "SeqPriority" );

               if ( CompareAttributeToString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "D" ) == 0 )
                  SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Desc" );
               else
                  SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Asc" );
               (*lpMaxPriority)++;
            }
         }

         // Process any subentities.
         if ( CheckExistenceOfEntity( vLOD_R, "LOD_EntityChild" ) >= zCURSOR_SET )
            zwTZZOLODD_ShowPriorityListSub( vLOD_R, vWork, lpMaxPriority );
      }
   }

   ResetViewFromSubobject( vLOD_R );
   return( 0 );
}


static zSHORT
zwfnTZZOLODD_PriorityListGetLen( zVIEW   vWork,
                                 zPLONG  plmaxEntityNameLen,
                                 zPLONG  plmaxAttributeNameLen )
{
   zSHORT  nRC;
   zCHAR   szEntityName[ 32 ];
   zCHAR   szAttributeName[ 32 ];
   zLONG   lEntityNameLen;
   zLONG   lAttributeNameLen;

   // get max Length for Entity and Attribute Name
   for ( nRC = SetCursorFirstEntity( vWork, "SelectedAttribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vWork, "SelectedAttribute", 0 ) )
   {
      GetStringFromAttribute( szEntityName, sizeof( szEntityName ),
                              vWork, "SelectedAttribute", "EntityName" );
      GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ),
                              vWork, "SelectedAttribute", "AttributeName" );
      lEntityNameLen    = zstrlen( szEntityName );
      lAttributeNameLen = zstrlen( szAttributeName );

      if ( lEntityNameLen > *plmaxEntityNameLen )
         *plmaxEntityNameLen = lEntityNameLen;

      if ( lAttributeNameLen > *plmaxAttributeNameLen )
         *plmaxAttributeNameLen = lAttributeNameLen;
   }

   if ( *plmaxEntityNameLen < 12 )
      *plmaxEntityNameLen = 12;

   if ( *plmaxAttributeNameLen < 15 )
      *plmaxAttributeNameLen = 15;

   return( 0 );
}

static zSHORT
zwfnTZZOLODD_PrintPriorityList( zVIEW vSubtask,
                                zVIEW vLOD,
                                zVIEW vWork,
                                zLONG lMaxPriority )
{
   zCHAR  szEntityName[ 32 ];
   zCHAR  szAttributeName[ 32 ];
   zCHAR  szPrioritySequence[ 4 ];
   zCHAR  szSequenceType[ 10 ];
   zSHORT nRC;
   zLONG  lmaxEntityNameLen    = 0;
   zLONG  lmaxAttributeNameLen = 0;
   zLONG  lEntityNameLen;
   zLONG  lAttributeNameLen;
   zLONG  k;
   CString  cstrMsg;

   cstrMsg = "\nEntity: \t";
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vLOD, "LOD_Entity", "Name" );
   cstrMsg += szEntityName;
   cstrMsg += "\n";

   if ( lMaxPriority != 0 )
   {
      // Order the work object by sequence number.
      OrderEntityForView( vWork, "SelectedAttribute", "PrioritySequence A" );

      // get max Length for Entity and Attribute Name
      zwfnTZZOLODD_PriorityListGetLen( vWork, &lmaxEntityNameLen,
                                       &lmaxAttributeNameLen );

      cstrMsg += "\n\nPriority:\tEntity Name:";
      for ( k = 0; 12 + k <= lmaxEntityNameLen; k++ )
         cstrMsg += " ";
      cstrMsg += "\t";
      cstrMsg += "Attribute Name:";
      for ( k = 0; 14 + k <= lmaxAttributeNameLen; k++ )
         cstrMsg += " ";
      cstrMsg += "\tType:       \n\n";

      // Add work object entries to message.
      for ( nRC = SetCursorFirstEntity( vWork, "SelectedAttribute", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vWork, "SelectedAttribute", 0 ) )
      {
         GetStringFromAttribute( szPrioritySequence, sizeof( szPrioritySequence ),
                                 vWork, "SelectedAttribute", "PrioritySequence" );
         GetStringFromAttribute( szEntityName, sizeof( szEntityName ),
                                 vWork, "SelectedAttribute", "EntityName" );
         GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ),
                                 vWork, "SelectedAttribute", "AttributeName" );
         GetStringFromAttribute( szSequenceType, sizeof( szSequenceType ),
                                 vWork, "SelectedAttribute", "OrderFlag" );
         lEntityNameLen    = zstrlen( szEntityName );
         lAttributeNameLen = zstrlen( szAttributeName );

         cstrMsg += szPrioritySequence;
         cstrMsg += "\t";
         cstrMsg += szEntityName;
         for ( k = 0; lEntityNameLen + k <= lmaxEntityNameLen; k++ )
            cstrMsg += " ";
         cstrMsg += "\t";
         cstrMsg += szAttributeName;
         for ( k = 0; lAttributeNameLen + k <= lmaxAttributeNameLen; k++ )
            cstrMsg += " ";
         cstrMsg += "\t";
         cstrMsg += szSequenceType;
         cstrMsg += "\t\n";
      }

      MessageSend( vSubtask, "ZO00208", "Ordering List for Activate",
                   cstrMsg,
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }
   else
   {
      cstrMsg += "\nThere is no Attribute Ordering for this entity.";

      MessageSend( vSubtask, "ZO00208", "Ordering List for Activate",
                   cstrMsg,
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ShowPriorityList( zVIEW vSubtask )
{
   zVIEW  vLOD;
   zVIEW  vLOD_R;
   zVIEW  vWork;
   zLONG  lMaxPriority;
   zLONG  lLOD_EntityZKey;
   zSHORT nRC;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vLOD_R, vLOD, 0 );
   ActivateEmptyObjectInstance( &vWork, "TZZOWK1O", vSubtask, zSINGLE );
   SetNameForView( vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   lMaxPriority = 0;

   // Make sure the LOD_Entity being processed is not a subordinate entity
   // with max cardinality 1, since there can be no ordering of a single
   // entity instance.
   if ( CheckExistenceOfEntity( vLOD, "ER_RelLink" ) >= zCURSOR_SET &&
        CompareAttributeToString( vLOD, "ER_RelLink", "CardMax", "1" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00177", "Zeidon Object Maintenance",
                   "Activate Ordering is not valid for an entity with max cardinality of 1.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // We will start by formating the work object from the existing priority
   // settings and then using the work object to format the message.
   // First loop through all attributes in the entity and create entries
   // for those with sequence priority (the same as OrderBuild).
   // Then process all 1-to-1 child entities.

   CreateEntity( vWork, "WorkRoot", zPOS_AFTER );

   // Process attributes for current entity.
   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", 0 ))
   {
      if ( CompareAttributeToInteger( vLOD, "LOD_Attribute", "SeqPriority", 0 ) != 0 )
      {
         CreateEntity( vWork, "SelectedAttribute", zPOS_AFTER );
         SetAttributeFromAttribute( vWork, "SelectedAttribute", "EntityName",
                                    vLOD,  "LOD_Entity",         "Name" );
         SetAttributeFromAttribute( vWork, "SelectedAttribute", "AttributeName",
                                    vLOD,  "ER_Attribute",       "Name" );
         SetAttributeFromAttribute( vWork, "SelectedAttribute", "PrioritySequence",
                                    vLOD,  "LOD_Attribute",      "SeqPriority" );

         if ( CompareAttributeToString( vLOD, "LOD_Attribute", "SeqOrder", "D" ) == 0 )
            SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Desc" );
         else
            SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Asc" );
         lMaxPriority++;
      }
   }

   // Process attributes in child entities with max cardinality of 1.
   GetIntegerFromAttribute( &lLOD_EntityZKey, vLOD, "LOD_Entity", "ZKey" );
   nRC = zwTZZOLODD_LocateRecursiveEntity( vLOD_R, lLOD_EntityZKey );

   if ( CheckExistenceOfEntity( vLOD_R, "LOD_EntityChild" ) >= zCURSOR_SET )
      zwTZZOLODD_ShowPriorityListSub( vLOD_R, vWork, &lMaxPriority );

   DropView( vLOD_R );

   zwfnTZZOLODD_PrintPriorityList( vSubtask, vLOD, vWork, lMaxPriority );

   DropView( vWork );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderCreateRegEntry( zVIEW  vLOD,
                                zVIEW  vWork )
{
   // Create the Activate Ordering entry in the work object.
   // It will be created as an entry already selected or as a potential
   // attribute for selection.

   // We are positioned on the correct LOD_Entity and ER_Attribute for
   // creating the entry in the work object representing the
   // LOD_Entity.ER_Attribute combination that can be used as a key for
   // Activate Ordering.

   if ( CompareAttributeToInteger( vLOD, "LOD_Attribute", "SeqPriority", 0 ) != 0 )
   {
      CreateEntity( vWork, "SelectedAttribute", zPOS_AFTER );
      SetAttributeFromAttribute( vWork, "SelectedAttribute", "EntityName",
                                 vLOD,  "LOD_Entity",        "Name" );
      SetAttributeFromAttribute( vWork, "SelectedAttribute", "AttributeName",
                                 vLOD,  "ER_Attribute",      "Name" );
      SetAttributeFromAttribute( vWork, "SelectedAttribute", "PrioritySequence",
                                 vLOD,  "LOD_Attribute",     "SeqPriority" );

      if ( CompareAttributeToString( vLOD, "LOD_Attribute", "SeqOrder", "D" ) == 0 )
         SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Desc" );
      else
         SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Asc" );
   }
   else
   {
      CreateEntity( vWork, "PotentialAttribute", zPOS_AFTER );
      SetAttributeFromAttribute( vWork, "PotentialAttribute", "EntityName",
                                 vLOD,  "LOD_Entity",        "Name" );
      SetAttributeFromAttribute( vWork, "PotentialAttribute", "AttributeName",
                                 vLOD,  "ER_Attribute",       "Name" );

   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_LocateRecursAttr( zVIEW vLOD,
                             zLONG lER_AttributeZKey,
                             zLONG lER_RelTypeZKey )
{
   // Process the recursive side of the LOD, searching for a child Attribute.

   zSHORT  nRC;

   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", 0 ) )
   {
      for ( nRC = SetCursorFirstEntityByInteger( vLOD, "ER_AttributeRec", "ZKey",
                                                 lER_AttributeZKey, "LOD_EntityParent" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByInteger( vLOD, "ER_AttributeRec", "ZKey",
                                                lER_AttributeZKey, "LOD_EntityParent" ))
         if ( CheckExistenceOfEntity( vLOD, "ER_RelTypeRec" ) < zCURSOR_SET ||
              CompareAttributeToInteger( vLOD, "ER_RelTypeRec", "ZKey",
                                         lER_RelTypeZKey ) == 0 )
      {
         return( 1 );
      }

      // Process any children.
      SetViewToSubobject( vLOD, "LOD_EntityChild" );
      nRC = zwTZZOLODD_LocateRecursAttr( vLOD,
                                         lER_AttributeZKey,
                                         lER_RelTypeZKey );
      if ( nRC == 1 )
         return( nRC );

      ResetViewFromSubobject( vLOD );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderBuildLocateAttr( zVIEW vDTE,
                                 zPLONG plAttributeZKey,
                                 zPLONG plRelTypeZKey )
{
   // Follow the recursive TE relationship chain to locate the key attribute.

   zVIEW  vDTE_T;
   SHORT  nRC;

   CreateViewFromViewForTask( &vDTE_T, vDTE, 0 );
   nRC = SetCursorFirstEntityByAttr( vDTE_T, "TE_FieldDataRel",    "ZKey",
                                     vDTE,   "TE_FieldDataRel_PK", "ZKey",
                                     "TE_DB_Environ" );

   if ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDTE_T, "ER_Attribute" ) >= zCURSOR_SET )
      {
         // We found the attribute, so return the ZKeys.
         GetIntegerFromAttribute( plAttributeZKey,
                                  vDTE_T, "ER_Attribute", "ZKey" );
         GetIntegerFromAttribute( plRelTypeZKey,
                                  vDTE, "ER_RelType_O", "ZKey" );
         DropView( vDTE_T );
         return( 0 );
      }
      else
         // Process the next level of chain.
         nRC = zwTZZOLODD_OrderBuildLocateAttr( vDTE_T,
                                                plAttributeZKey,
                                                plRelTypeZKey );
   }
   else
   {
      // We've come to the end of the chain without finding the attribute.
      // Return an error.
      nRC = -1;
   }

   DropView( vDTE_T );
   return( nRC );
}


static zSHORT
zwfnTZZOLODD_OrderBuild( zVIEW vDTE,
                         zVIEW vDTE_T,
                         zVIEW vLOD_T,
                         zVIEW vLOD_R,
                         zVIEW vWork )
{
   zVIEW   vLOD_R2;
   zSHORT  nRC;
   zLONG   lER_AttributeZKey;
   zLONG   lER_RelTypeZKey;

   // Loop through all TE_FieldDataRel entries and process entries as
   // regular attributes or foriegn keys. Any entries that aren't either (there
   // should be none) are skipped.
   for ( nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRel", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDTE, "TE_FieldDataRel", 0 ) )
   {
      if ( CheckExistenceOfEntity( vDTE, "ER_Attribute" ) >= zCURSOR_SET )
      {
         // Process regular Attribute, but only if the ER_Attribute is in
         // the LOD.
         nRC = SetCursorFirstEntityByAttr( vLOD_T, "ER_Entity", "ZKey",
                                           vDTE,   "ER_Entity", "ZKey", "LOD" );
         if ( nRC >= zCURSOR_SET )
            nRC = SetCursorFirstEntityByAttr( vLOD_T, "ER_Attribute", "ZKey",
                                              vDTE,   "ER_Attribute", "ZKey", "LOD_Entity" );

         if ( nRC >= zCURSOR_SET )
         {
            zwTZZOLODD_OrderCreateRegEntry( vLOD_T, vWork );
         }
      }
      else
      {
         if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel_PK" ) >= zCURSOR_SET )
         {
            nRC = zwTZZOLODD_OrderBuildLocateAttr( vDTE,
                                                   &lER_AttributeZKey,
                                                   &lER_RelTypeZKey );
            if ( nRC == 0 )
            {
               // Process foreign key.
               // Only create an entry if the foreign key is an Attribute in a
               // child entity.
               CreateViewFromViewForTask( &vLOD_R2, vLOD_R, 0 );
               SetViewToSubobject( vLOD_R2, "LOD_EntityChild" );
               nRC = zwTZZOLODD_LocateRecursAttr( vLOD_R2,
                                                  lER_AttributeZKey,
                                                  lER_RelTypeZKey);
               if ( nRC > 0 )
               {
                  SetCursorFirstEntityByAttr( vLOD_T,  "LOD_Entity",       "ZKey",
                                              vLOD_R2, "LOD_EntityParent", "ZKey", 0 );
                  SetCursorFirstEntityByAttr( vLOD_T,  "LOD_Attribute",    "ZKey",
                                              vLOD_R2, "LOD_AttributeRec", "ZKey", 0 );
                  zwTZZOLODD_OrderCreateRegEntry( vLOD_T, vWork );
               }

               ResetViewFromSubobject( vLOD_R2 );
               DropView( vLOD_R2 );
            }
         }
      }
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderBuild( zVIEW vSubtask )
{
   // Build the work object containing Selected and Potential Priority
   // entries.  Physically, the potential Attributes for ordering are
   // those that make up columns in the corresponding SQL table.  These
   // are the regular attributes for an entity, but also foreign keys from
   // children with 1-to-1 cardinality.  The logic to determine the
   // foreign key entries is convoluted, but since it has already been done
   // in the TE, we will use the TE to create these entries.

   // We will thus loop through all TE_FieldDataRel entities for the
   // corresponding table in the TE. If the entry is for a regular attribute
   // (ER_Attribute exists), then we will simply position on the
   // corresponding ER_Attribute under the current LOD_Entity and process it.
   // If the entry is for a foreign key, we will first position on the
   // correct TE_FieldDataRel entry in the TE and then use it to position on
   // the corresponding ER_Attribute under the correct LOD_Entity.
   // To keep the logic the same for both conditions, we will assume we
   // always have to position on the correct LOD_Entity in the LOD.

   // One complexity is that a foreign key in the TE may not be for one
   // of the child entities in the LOD but for a parent or for an entity
   // not in the LOD at all.  Thus we will have to search the recursive
   // chain of child entities to determine this.

   zVIEW  vLOD;
   zVIEW  vLOD_T;
   zVIEW  vLOD_R;
   zVIEW  vDTE;
   zVIEW  vDTE_T;
   zVIEW  vT;
   zVIEW  vWork;
   zLONG  lLOD_EntityZKey;
   zSHORT nRC;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vLOD_T, vLOD, 0 );

   // Create view to recursive structure.
   CreateViewFromViewForTask( &vLOD_R, vLOD, 0 );
   GetIntegerFromAttribute( &lLOD_EntityZKey, vLOD, "LOD_Entity", "ZKey" );
   zwTZZOLODD_LocateRecursiveEntity( vLOD_R, lLOD_EntityZKey );

   // Get views to DTE
   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) < 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vT, zREFER_DTE_META );
      if ( ActivateMetaOI( vSubtask, &vDTE, vT, zREFER_DTE_META,
                           zSINGLE | zLEVEL_APPLICATION ) >= 0 )
      {
         SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
      }
   }

/* bl 2000.07.16  TB 50766
   // Position on correct DBMS_Source for the LOD. Else it is an error.
   if ( CheckExistenceOfEntity( vLOD, "TE_DBMS_Source" ) < zCURSOR_SET ||
        SetCursorFirstEntityByAttr( vDTE, "TE_DBMS_Source", "ZKey",
                                    vLOD, "TE_DBMS_Source", "ZKey", 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "ZO00179", "Zeidon Object Maintenance",
        "The Data Source specified in the LOD does not match a Data Source in the Physical Model.",
        zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }
*/
   // Make sure the LOD_Entity being processed is not a subordinate entity
   // with max cardinality 1, since there can be no ordering of a single
   // entity instance.
   if ( CheckExistenceOfEntity( vLOD, "ER_RelLink" ) >= zCURSOR_SET &&
        CompareAttributeToString( vLOD, "ER_RelLink", "CardMax", "1" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00177", "Zeidon Object Maintenance",
                   "Activate Ordering is not valid for an entity with max cardinality of 1.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   ActivateEmptyObjectInstance( &vWork, "TZZOWK1O", vSubtask, zSINGLE );
   SetNameForView( vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );

   CreateEntity( vWork, "WorkRoot", zPOS_AFTER );

   // Position on the corresponding table entry in the TE for the current
   // LOD_Entity.  Since it is possible that we have a TE_DBMS_Source entry
   // that is not relational, we will loop to the first relational entry.  If
   // we don't find one, we will be positioned on the last entry and will
   // simply process it, which won't give us foreign keys.
   if ( CheckExistenceOfEntity( vLOD, "ER_Entity" ) >= zCURSOR_SET )
   {
      for ( nRC = SetCursorFirstEntityByAttr( vDTE, "ER_Entity", "ZKey",
                                              vLOD, "ER_Entity", "ZKey", "TE_DB_Environ" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByAttr( vDTE, "ER_Entity", "ZKey",
                                             vLOD, "ER_Entity", "ZKey", "TE_DB_Environ" ) )
      {
         if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "Paradigm", "R" ) == 0 )
            break;
      }
   }

   if ( vDTE )
      CreateViewFromViewForTask( &vDTE_T, vDTE, 0 );
   else
   {
      DropView( vLOD_T );
      DropView( vLOD_R );
      return( -1 );
   }

   // Loop through all TE_FieldDataRel entries and process entries as
   // regular attributes or foriegn keys. Any entries that aren't either (there
   // should be none) are skipped.
   zwfnTZZOLODD_OrderBuild( vDTE, vDTE_T, vLOD_T, vLOD_R, vWork );

   // Order SelectedAttributes by Sequence Priority.
   OrderEntityForView( vWork, "SelectedAttribute", "PrioritySequence A" );
   SetAttributeFromString( vWork, "WorkRoot", "ContinueFlag", "N" );
   SetCursorFirstEntity( vWork, "SelectedAttribute", "" );
   SetCursorFirstEntity( vWork, "PotentialAttribute", "" );

   DropView( vLOD_T );
   DropView( vLOD_R );
   DropView( vDTE_T );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderCreatePostEntry( zVIEW  vLOD,
                                 zVIEW  vWork )
{
   zSHORT  nRC;

   // Loop through all attributes and create a work entry on either the
   // Selected side (if the attribute has already been selected as a
   // sequencing attribute) or on the potential side otherwise.

   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_AttributeRec", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_AttributeRec", 0 ) )
   {
      if ( CompareAttributeToInteger( vLOD, "LOD_AttributeRec", "SeqPriority", 0 ) != 0 )
      {
         CreateEntity( vWork, "SelectedAttribute", zPOS_AFTER );
         SetAttributeFromAttribute( vWork, "SelectedAttribute", "EntityName",
                                    vLOD,  "LOD_EntityParent",  "Name" );
         SetAttributeFromAttribute( vWork, "SelectedAttribute", "AttributeName",
                                    vLOD,  "ER_AttributeRec",   "Name" );
         SetAttributeFromAttribute( vWork, "SelectedAttribute", "PrioritySequence",
                                    vLOD,  "LOD_AttributeRec",  "SeqPriority" );

         if ( CompareAttributeToString( vLOD, "LOD_AttributeRec", "SeqOrder", "D" ) == 0 )
            SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Desc" );
         else
            SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Asc" );
      }
      else
      {
         // Create an attribute entry unless attribute is work.
         if ( CompareAttributeToString( vLOD, "LOD_AttributeRec", "Work", "Y" ) != 0 )
         {
            CreateEntity( vWork, "PotentialAttribute", zPOS_AFTER );
            SetAttributeFromAttribute( vWork, "PotentialAttribute", "EntityName",
                                       vLOD,  "LOD_EntityParent",   "Name" );
            SetAttributeFromAttribute( vWork, "PotentialAttribute", "AttributeName",
                                       vLOD,  "ER_AttributeRec",    "Name" );
         }
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_LocateRecursAttrPost( zVIEW vLOD,
                                 zVIEW vWork )
{
   // Process all children recursively with max cardinality 1.

   zSHORT  nRC;

   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", 0 ) )
   {
      // Only process entries with max cardinality of 1.
      if ( CheckExistenceOfEntity( vLOD, "ER_RelLinkRec" ) >= zCURSOR_SET &&
           CompareAttributeToString( vLOD, "ER_RelLinkRec", "CardMax",
                                       "1" ) == 0 )
      {
         // Go to create the work entries.
         zwTZZOLODD_OrderCreatePostEntry( vLOD, vWork );

         // Process any children with max cardinality of 1.
         SetViewToSubobject( vLOD, "LOD_EntityChild" );

         if ( CheckExistenceOfEntity( vLOD, "ER_RelLinkRec" ) >= zCURSOR_SET &&
            CompareAttributeToString( vLOD, "ER_RelLinkRec", "CardMax",
                                          "1" ) == 0 )
            zwTZZOLODD_LocateRecursAttrPost( vLOD, vWork );

         ResetViewFromSubobject( vLOD );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderBuildPostAct( zVIEW vSubtask )
{
   // Build the list of all potential sequencing attributes for Post Activate
   // processing.  This will simply be a list of all attributes, work and/or
   // ER, on the current entity or any of its children that have a 1-to-1
   // relationship to it.
   // We will thus first position on the recursive view of the current entity
   // and then process all children until we reach a leaf node or until we
   // come to a child whose max cardinality is not 1.

   zVIEW  vLOD;
   zVIEW  vLOD_R;
   zVIEW  vWork;
   zLONG  lLOD_EntityZKey;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Make sure the LOD_Entity being processed is not a subordinate entity
   // with max cardinality 1, since there can be no ordering of a single
   // entity instance.
   if ( CheckExistenceOfEntity( vLOD, "ER_RelLink" ) >= zCURSOR_SET &&
        CompareAttributeToString( vLOD, "ER_RelLink", "CardMax", "1" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00177", "Zeidon Object Maintenance",
                   "Activate Ordering is not valid for an entity with max cardinality of 1.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Create view to recursive structure.
   CreateViewFromViewForTask( &vLOD_R, vLOD, 0 );
   GetIntegerFromAttribute( &lLOD_EntityZKey, vLOD, "LOD_Entity", "ZKey" );
   zwTZZOLODD_LocateRecursiveEntity( vLOD_R, lLOD_EntityZKey );

   // Create empty work object.
   ActivateEmptyObjectInstance( &vWork, "TZZOWK1O", vSubtask, zSINGLE );
   SetNameForView( vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   CreateEntity( vWork, "WorkRoot", zPOS_AFTER );

   // Process the attributes in this entity and in all child entries with
   // max card of 1.
   zwTZZOLODD_OrderCreatePostEntry( vLOD_R, vWork );

   SetViewToSubobject( vLOD_R, "LOD_EntityChild" );
   zwTZZOLODD_LocateRecursAttrPost( vLOD_R, vWork );
   ResetViewFromSubobject( vLOD_R );

   DropView( vLOD_R );
   OrderEntityForView( vWork, "SelectedAttribute", "PrioritySequence A" );
   SetCursorFirstEntity( vWork, "SelectedAttribute", "" );
   SetCursorFirstEntity( vWork, "PotentialAttribute", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ResetAllSubSeqValues( zVIEW vLOD_R )
{
   // Go through the whole subobject and reset sequencing values for any
   // child that has max cardinality of 1.

   zSHORT nRC;

   SetViewToSubobject( vLOD_R, "LOD_EntityChild" );
   for ( nRC = SetCursorFirstEntity( vLOD_R, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_R, "LOD_EntityParent", 0 ) )
   {
      if ( CheckExistenceOfEntity( vLOD_R, "ER_RelLinkRec" ) >= zCURSOR_SET &&
           CompareAttributeToString( vLOD_R, "ER_RelLinkRec", "CardMax", "1" ) == 0 )
      {
         for ( nRC = SetCursorFirstEntity( vLOD_R, "LOD_AttributeRec", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vLOD_R, "LOD_AttributeRec", 0 ) )
         {
            SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqPriority", "" );
            SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "" );
         }
         zwTZZOLODD_ResetAllSubSeqValues( vLOD_R );
      }
   }

   ResetViewFromSubobject( vLOD_R );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SetSubSeqValue( zVIEW vLOD_R, zVIEW vWork )
{
   // We need to set the sequencing values for a work entry into the
   // proper attribute in a subobject. Process subobjects recursively
   // until the correct attribute is found and then set its sequencing
   // values from the current values in the work object entity.

   zSHORT nRC;
   zLONG  lLOD_EntityZKey;

   SetViewToSubobject( vLOD_R, "LOD_EntityChild" );

   if ( CheckExistenceOfEntity( vLOD_R, "LOD_EntityParent" ) >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByAttr( vLOD_R, "LOD_EntityParent",  "Name",
                                       vWork,  "SelectedAttribute", "EntityName", 0 );

      if ( nRC >= zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vLOD_R, "ER_AttributeRec",   "Name",
                                     vWork,  "SelectedAttribute", "AttributeName", "LOD_EntityParent" );
         SetAttributeFromAttribute( vLOD_R, "LOD_AttributeRec",  "SeqPriority",
                                    vWork,  "SelectedAttribute", "PrioritySequence" );
         if ( CompareAttributeToString( vWork, "SelectedAttribute", "OrderFlag",
                                        "Asc" ) == 0 )
            SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "A" );
         else
            SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "D" );
      }
      else
         zwTZZOLODD_SetSubSeqValue( vLOD_R, vWork );
   }
   else
   {
      ResetViewFromSubobject( vLOD_R );
      GetIntegerFromAttribute( &lLOD_EntityZKey, vWork, "SelectedAttribute", "ZKey" );
      zwTZZOLODD_LocateRecursiveEntity( vLOD_R, lLOD_EntityZKey );
      zwTZZOLODD_SetSubSeqValue( vLOD_R, vWork );
   }

   ResetViewFromSubobject( vLOD_R );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderOK( zVIEW vSubtask )
{

   // Transform the Priority selections from the work object into the
   // LOD object.

   zVIEW  vLOD;
   zVIEW  vLOD_R;
   zVIEW  vWork;
   zLONG  lLOD_EntityZKey;
   zSHORT nCount;
   zSHORT nRC;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );

   // First, reset all the SeqPriority values in the target to null.
   // We must do this for attributes in the root entity, but also for
   // attributes in any children that have max cardinality 1.
   // We will do this on the recursive side of the LOD.
   CreateViewFromViewForTask( &vLOD_R, vLOD, 0 );
   GetIntegerFromAttribute( &lLOD_EntityZKey, vLOD, "LOD_Entity", "ZKey" );
   zwTZZOLODD_LocateRecursiveEntity( vLOD_R, lLOD_EntityZKey );

   for ( nRC = SetCursorFirstEntity( vLOD_R, "LOD_AttributeRec", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_R, "LOD_AttributeRec", 0 ) )
   {
      SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqPriority", "" );
      SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "" );
   }

   zwTZZOLODD_ResetAllSubSeqValues( vLOD_R );
   DropView( vLOD_R );

   // Sequence the selected attributes in the work object.
   nCount = 0;
   for ( nRC = SetCursorFirstEntity( vWork, "SelectedAttribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vWork, "SelectedAttribute", 0 ) )
   {
      nCount++;
      SetAttributeFromInteger( vWork, "SelectedAttribute", "PrioritySequence",
                               nCount );
   }

   CreateViewFromViewForTask( &vLOD_R, vLOD, 0 );
   zwTZZOLODD_LocateRecursiveEntity( vLOD_R, lLOD_EntityZKey );

   // Set the SeqPriority values in the LOD from the work object. Use the
   // recursive side of the LOD.
   for ( nRC = SetCursorFirstEntity( vWork, "SelectedAttribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vWork, "SelectedAttribute", 0 ) )
   {
      // If the selected attribute is in the root LOD, process it there.
      // Otherwise process it on a subentity.
      nRC = SetCursorFirstEntityByAttr( vLOD_R, "LOD_EntityParent",  "Name",
                                        vWork,  "SelectedAttribute", "EntityName", 0 );
      if ( nRC >= zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vLOD_R, "ER_AttributeRec",   "Name",
                                     vWork,  "SelectedAttribute", "AttributeName", "LOD_EntityParent" );
         SetAttributeFromAttribute( vLOD_R, "LOD_AttributeRec",  "SeqPriority",
                                    vWork,  "SelectedAttribute", "PrioritySequence" );
         if ( CompareAttributeToString( vWork, "SelectedAttribute", "OrderFlag", "Asc" ) == 0 )
            SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "A" );
         else
            SetAttributeFromString( vLOD_R, "LOD_AttributeRec", "SeqOrder", "D" );
      }
      else
      {
         zwTZZOLODD_SetSubSeqValue( vLOD_R, vWork );
      }
   }

   DropView( vLOD_R );
   DropObjectInstance( vWork );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderCancel( zVIEW vSubtask )
{
   zVIEW vWork;

   // Delete the work object.
   GetViewByName( &vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   DropObjectInstance( vWork );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderSetDescending( zVIEW vSubtask )
{
   zVIEW vWork;

   // Set the select item in the work object to descending state.
   GetViewByName( &vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Desc" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderSetAscending( zVIEW vSubtask )
{
   zVIEW vWork;

   // Set the select item in the work object to ascending state.
   GetViewByName( &vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Asc" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderItemSelect( zVIEW vSubtask )
{
   zVIEW vWork;

   // Move an attribute from the potential side to the selected side of
   // the work object.
   GetViewByName( &vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vWork, "PotentialAttribute" ) >= zCURSOR_SET )
   {
      CreateEntity( vWork, "SelectedAttribute", zPOS_AFTER );
      SetMatchingAttributesByName( vWork, "SelectedAttribute",
                                 vWork, "PotentialAttribute", zSET_ALL );
      SetAttributeFromString( vWork, "SelectedAttribute", "OrderFlag", "Asc" );
      DeleteEntity( vWork, "PotentialAttribute", zREPOS_PREV );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_OrderItemDeselect( zVIEW vSubtask )
{
   zVIEW vWork;

   // Move an attribute from the selected side back to the potential side of
   // the work object.
   GetViewByName( &vWork, "TZZOWK1O", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vWork, "SelectedAttribute" ) >= zCURSOR_SET )
   {
      CreateEntity( vWork, "PotentialAttribute", zPOS_AFTER );
      SetMatchingAttributesByName( vWork, "PotentialAttribute",
                                   vWork, "SelectedAttribute", zSET_ALL );
      DeleteEntity( vWork, "SelectedAttribute", zREPOS_PREV );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CopyAttributes( zVIEW vSubtask )
{
   zVIEW  vLOD;
   zVIEW  vWork;
   zSHORT nRC;
   zBOOL  bER_AttributesExists = FALSE;

   // Copy selected attributes under the root entity.
   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vWork, "CopyWork", vSubtask, zLEVEL_SYSTEM );

   // If the work object doesn't exist, create it.
   if ( nRC < 0 )
   {
      ActivateEmptyObjectInstance( &vWork, "TZZOWK1O", vLOD, zLEVEL_SYSTEM );
      CreateEntity( vWork, "WorkRoot", zPOS_AFTER );
      SetNameForView( vWork, "CopyWork", 0, zLEVEL_SYSTEM );
   }

   // First remove any existing derived entries.
   for ( nRC = SetCursorFirstEntity( vWork, "LOD_AttributeCopy", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vWork, "LOD_AttributeCopy", 0 ) )
   {
      DeleteEntity( vWork, "LOD_AttributeCopy", zREPOS_NONE );
   }

   // Now loop through all the selected entries and copy them.
   for ( nRC = SetCursorFirstSelectedEntity( vLOD, "LOD_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vLOD, "LOD_Attribute", 0 ) )
   {
      if ( CompareAttributeToString( vLOD, "LOD_Attribute", "Work", "Y" ) != 0 )
         bER_AttributesExists = TRUE;

      CreateEntity( vWork, "LOD_AttributeCopy", zPOS_AFTER );
      SetMatchingAttributesByName( vWork, "LOD_AttributeCopy",
                                   vLOD,  "LOD_Attribute", zSET_ALL );
      CreateEntity( vWork, "ER_AttributeCopy", zPOS_AFTER );
      SetMatchingAttributesByName( vWork, "ER_AttributeCopy",
                                   vLOD,  "ER_Attribute", zSET_ALL );
      CreateEntity( vWork, "DomainCopy", zPOS_AFTER );
      SetMatchingAttributesByName( vWork, "DomainCopy",
                                   vLOD,  "Domain", zSET_ALL );
   }

   // Give a warning message if any of the attributes being copied
   // are ER_Attributes, which will become work attributes.
   if ( bER_AttributesExists )
   {
      MessageSend( vSubtask, "ZO00180", "Zeidon Object Maintenance",
         "An ER Attribute is being copied, which will be copied as a work attribute.",
         zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_PasteAttributes( zVIEW vSubtask )
{
   zVIEW  vLOD;
   zVIEW  vWork;
   zVIEW  vDomain = 0;
   zCHAR  szErrorMsg[ 254 ];
   zCHAR  szTemp[ 33 ];
   zSHORT nRC;
   zLONG  lZKey;

   // Paste selected attributes under the LOD_Entity entity.
   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vWork, "CopyWork", vSubtask, zLEVEL_SYSTEM );

   // Exit immediately if there is no Copy Work object.
   if ( nRC < 0 )
      return( -0 );

   // Loop through all LOD_AttributeCopy entities and copy them to
   // LOD_Attribute entities. The resulting LOD_Attributes are always
   // work attributes.
   for ( nRC = SetCursorFirstEntity( vWork, "LOD_AttributeCopy", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vWork, "LOD_AttributeCopy", 0 ) )
   {
      CreateMetaEntity( vSubtask, vLOD, "LOD_Attribute", zPOS_AFTER );
      SetMatchingAttributesByName( vLOD,  "LOD_Attribute",
                                   vWork, "LOD_AttributeCopy", zSET_NULL );
      SetAttributeFromString( vLOD, "LOD_Attribute", "Work", "Y" );
      CreateMetaEntity( vSubtask, vLOD, "ER_Attribute", zPOS_AFTER );
      SetMatchingAttributesByName( vLOD,  "ER_Attribute",
                                   vWork, "ER_AttributeCopy", zSET_NULL );
      GetIntegerFromAttribute( &lZKey, vWork, "DomainCopy", "ZKey" );
      // set new Attribute Sequence
      oTZZOLODO_NewAttributeSequence( vLOD );
      if ( ActivateMetaOI_ByZKey( vSubtask, &vDomain, 0, zREFER_DOMAIN_META,
                                  zSINGLE, lZKey, 0 ) < 0 )
      {
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vWork, "DomainCopy", "Name" );
         ActivateMetaOI_ByName( vSubtask, &vDomain, 0, zREFER_DOMAIN_META,
                                zSINGLE, szTemp, 0 );
      }

      if ( vDomain )
      {
         IncludeSubobjectFromSubobject( vLOD, "Domain", vDomain, "Domain", zPOS_AFTER );
         DropView( vDomain );
      }
      else
      {
         strcpy_s( szErrorMsg, sizeof( szErrorMsg ), "Attribute '" );
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vWork, "ER_AttributeCopy", "Name" );
         strcat_s( szErrorMsg, sizeof( szErrorMsg ), szTemp );
         strcat_s( szErrorMsg, sizeof( szErrorMsg ), "' cannot be created, because \nDomain ' " );
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vWork, "DomainCopy", "Name" );
         strcat_s( szErrorMsg, sizeof( szErrorMsg ), szTemp );
         strcat_s( szErrorMsg, sizeof( szErrorMsg ), "' does not exist." );
         MessageSend( vSubtask, "ZO00137", "LOD Maintenance", szErrorMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
   }

   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ReplaceRootEntityPreB( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vT;
   zVIEW  vProfileXFER;
   zSHORT nRC;

   // Get Access to ER Object,
   nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      nRC = RetrieveViewForMetaList( vSubtask, &vT, zREFER_ERD_META );
      if ( ActivateMetaOI( vSubtask, &vTZEREMDO, vT, zREFER_ERD_META,
                           zSINGLE | zLEVEL_APPLICATION ) >= 0 )
      {
         SetNameForView( vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
      }

      DropView( vT );
   }

   // set Default Radio Button
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vProfileXFER, "ZO", "LOD_Type", "ER" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_ReplaceRootEntityOK( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vProfileXFER;
   zVIEW  vLOD;
   zVIEW  vTZZOHDGO;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );

   // Delete all current entities.
   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_Entity", 0 ) )
   {
      DeleteEntity( vLOD, "LOD_Entity", zREPOS_NONE );
   }

   // Create a skeleton LOD Root Entity on the Recursive Side
   CreateMetaEntity( vSubtask, vLOD, "LOD_EntityParent", zPOS_AFTER );
   SetAttributeFromAttribute( vLOD, "LOD_EntityParent", "Name",
                              vLOD, "LOD",              "Name" );
   SetAttributeFromAttribute( vLOD, "LOD_EntityParent", "IndentName",
                              vLOD, "LOD",              "Name" );
   SetAttributeFromInteger( vLOD, "LOD_EntityParent", "IndentLvl", 0 );

   SetAttributeFromString( vLOD, "LOD_EntityParent", "Create", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Delete", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Include", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Exclude", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "InclSrc", "Y" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Recursive", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Derived", "N" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "ParentDeleteBehave", "-" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Update", "N" );

   // Create a skeleton LOD Root Entity on the flat side
   IncludeSubobjectFromSubobject( vLOD, "LOD_Entity", vLOD, "LOD_EntityParent", zPOS_AFTER );

   // Always complete as ER entity
   SetAttributeFromAttribute( vLOD, "LOD_EntityParent", "Name", vERD, "ER_Entity", "Name" );
   SetAttributeFromString( vLOD, "LOD_EntityParent", "Work", "N" );

   SetAttributeFromAttribute( vLOD, "LOD_EntityParent", "IndentName", vERD, "ER_Entity", "Name" );
   SetAttributeFromAttribute( vLOD, "LOD_EntityParent", "Desc", vERD, "ER_Entity", "Desc" );

   IncludeSubobjectFromSubobject( vLOD, "ER_EntityRec", vERD, "ER_Entity", zPOS_AFTER );

   // Build all Atttributes from the selected ER Entity under the LOD
   // Root Entity.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Attribute", 0 ) )
   {
      CreateMetaEntity( vSubtask, vLOD, "LOD_AttributeRec", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vLOD, "ER_AttributeRec", vERD, "ER_Attribute", zPOS_AFTER );
   }

   DropObjectInstance( vTZZOHDGO );
   oTZZOHDGO_CreateFromLOD( vSubtask, &vTZZOHDGO, vLOD, ORIENT );
   SetNameForView( vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );

   // Set Attribute Sequence for new Entity
   nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
   while ( nRC >= zCURSOR_SET )
   {
      oTZZOLODO_NewAttributeSequence( vLOD );
      nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
   }
   nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_AddSourceFile
//
// BL: add new source file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddSourceFile( zVIEW vSubtask )
{
   wTZOPRUSD_AddSourceFile( vSubtask );

   return( 0 );
} // zwTZZOLODD_AddSourceFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_AddOperation
//
// BL: add new Operation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddOperation( zVIEW vSubtask )
{
   zwTZZOLODD_TransferToOperDLG( vSubtask );

   wTZOPRUSD_AddOperation( vSubtask );

   return( 0 );
} // zwTZZOLODD_AddOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_UpdateOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_UpdateOperation( zVIEW vSubtask )
{
   wTZOPRUSD_GoToUpdateOperation( vSubtask );

   return( 0 );
} // zwTZZOLODD_UpdateOperation


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_RefreshAttributeList
//
// if single click in EntityList (List box Attributes),
// then refresh AttributeList
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshAttributeList( zVIEW vSubtask )
{
   zVIEW vW;

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "ATTVIEW" ) >= 0 )
      RefreshWindow( vW );

   return( 0 );
} // zwTZZOLODD_RefreshAttributeList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_RefreshEntityList
//
// if single click in Attribute List, then refresh Entity List
// (List box Attributes)
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_RefreshEntityList( zVIEW vSubtask )
{
   zVIEW vW;

   // Refresh the window if it exists.
   if ( GetSubtaskForWindowName( vSubtask, &vW, "ENTVIEW" ) >= 0 )
      RefreshCtrl( vW, "AttrList" );

   return( 0 );
} // zwTZZOLODD_RefreshEntityList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_EditOperationcode
//
// start Editor
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_EditOperationcode( zVIEW vSubtask )
{
   wTZOPRUSD_InvokeEditor( vSubtask );
   return( 0 );
} // zwTZZOLODD_EditOperationcode

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_DeleteSelectedFile
//
// delete selected Source Files
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteSelectedFile( zVIEW vSubtask )
{
   wTZOPRUSD_DeleteSourceFile( vSubtask );
   return( 0 );
} // zwTZZOLODD_DeleteSelectedFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_DeleteSelectedOperat
//
// delete selected Operations
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteSelectedOperat( zVIEW vSubtask )
{
   wTZOPRUSD_DeleteSelectedOperat( vSubtask );
   return( 0 );
} // zwTZZOLODD_DeleteSelectedOperat

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_DeleteAllSourceFiles
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteAllSourceFiles( zVIEW vSubtask )
{
   wTZOPRUSD_DeleteAllFiles( vSubtask );
   return( 0 );
} // zwTZZOLODD_DeleteAllSourceFiles

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_AcceptAttribute
//
// accept Attribute and set selection
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AcceptAttribute( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zSHORT nRC;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   nRC = AcceptSubobject( vTZZOLODO, "LOD_Attribute" );
   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   SetSelectSetForView( vTZZOLODO, 1 );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 1 );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
} // zwTZZOLODD_AcceptAttribute


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_DeleteAllOperations
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_DeleteAllOperations( zVIEW vSubtask )
{
   wTZOPRUSD_DeleteAllOperations( vSubtask );

   return( 0 );
} // zwTZZOLODD_DeleteAllOperations


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsSetSourceName
//
static zSHORT
zwfnTZZOLODD_SaveAsSetSourceName( zVIEW   vSubtask,
                                  zVIEW   vSaveAs,
                                  zVIEW   vLodData,
                                  zSHORT  nIndex )
{
   zCHAR  szNewName[33];
   zCHAR  szIndex[ 4 ];
   zSHORT nPosition = 0;
   zSHORT nRC;

   szNewName[ 0 ] = 0;

   do
   {
      nIndex++;
      GetStringFromAttribute( szNewName, sizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
      if ( nIndex > 0 )
      {
         nPosition = (zSHORT) zstrlen( szNewName );
         if ( nPosition > 6 )
            nPosition = 6;

         zltoa( nIndex, szIndex, sizeof( szIndex ) );
         if ( nIndex < 10 )
         {
            strncpy_s( szNewName + nPosition, sizeof( szNewName ) - nPosition, "_", 1 );
            strncpy_s( szNewName + 1 + nPosition, sizeof( szNewName ) - 1 - nPosition, szIndex, 2 );
         }
         else
         {
            if ( nPosition == 6 )
               nPosition--;
            strncpy_s( szNewName + nPosition, sizeof( szNewName ) - nPosition, "_", 1 );
            strncpy_s( szNewName + 1 + nPosition, sizeof( szNewName ) - 1 - nPosition, szIndex, 3 );
         }
      }
      nRC = -1;
      if ( vLodData )
         nRC = SetCursorFirstEntityByString( vLodData, "SourceFile", "Name", szNewName, "" );

   } while ( nRC >= zCURSOR_SET );


   SetAttributeFromString( vSaveAs, "SourceFile", "SaveAsName", szNewName );

   return( nIndex );
} // zwfnTZZOLODD_SaveAsSetSourceName


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SetSaveAsName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SetSaveAsName( zVIEW vSubtask )
{
   zVIEW  vTZZOLFLO;
   zVIEW  vTZZOLODO;
   zVIEW  vSaveAs;
   zVIEW  vLodData;
   zSHORT nRC;
   zSHORT nRC1      = -1;
   zSHORT nIndex    = 0;

   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "SaveAsName8",
                              vTZZOLFLO, "W_MetaDef", "Name" );

   // set Description
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) != 1 )
   {
      SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vTZZOLFLO, "W_MetaDef", "Desc" );
   }

   // set DLL Name and Source File Name
   if ( CompareAttributeToAttribute( vTZZOLFLO, "W_MetaDef", "Name",
                                     vTZZOLODO, "LOD", "Name" ) == 0 )
   {
      CreateViewFromViewForTask( &vLodData, vTZZOLODO, 0 );
   }
   else
   {
      if ( ActivateMetaOI( vSubtask, &vLodData, vTZZOLFLO, zSOURCE_LOD_META,
                           zSINGLE | zLEVEL_TASK ) != 1 )
      {
         return( -1 );
      }
   }

   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "DLL_Name",
                              vLodData, "LOD", "DLL_Name" );

   if ( CheckExistenceOfEntity( vLodData, "SourceFile" ) >= zCURSOR_SET )
      OrderEntityForView( vLodData, "SourceFile", "LanguageType D Name A" );
   else
      nIndex = -1;

   // set new source Files name
   nRC1 = SetCursorFirstEntity( vLodData, "SourceFile", "" );
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      if ( nRC1 == zCURSOR_SET &&
           CompareAttributeToAttribute( vSaveAs, "SourceFile", "LanguageType",
                          vLodData, "SourceFile", "LanguageType" ) == 0 )
      {
         SetAttributeFromAttribute( vSaveAs, "SourceFile", "SaveAsName",
                                    vLodData, "SourceFile", "Name" );
         nRC1 = SetCursorNextEntity( vLodData, "SourceFile", "" );
      }
      else
      {
         nIndex = zwfnTZZOLODD_SaveAsSetSourceName( vSubtask, vSaveAs,
                                                    vLodData, nIndex );
      }
   } // endfor ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );

   DropMetaOI( vSubtask, vLodData );
   SetFocusToCtrl( vSubtask, "edLODName" );

   return( 0 );
} // zwTZZOLODD_SetSaveAsName


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsCheckFileName
//
static zSHORT
zwfnTZZOLODD_SaveAsCheckFileName( zVIEW    vSubtask,
                                  zVIEW    vSaveAs )
{
   zVIEW    vSaveAsCopy;
   zSHORT   nRC;
   zCHAR    szNewName[33];
   zCHAR    szFileName[33];
   zCHAR    szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   CreateViewFromViewForTask( &vSaveAsCopy, vSaveAs, 0 );

   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      // Source File Name is required
      GetStringFromAttribute( szNewName, sizeof( szNewName ), vSaveAs, "SourceFile", "SaveAsName" );
      UfCompressName( szNewName, szFileName, 32, "", "", "", "", 0 );
      if ( zstrcmp( szFileName, "" ) == 0 )
      {
         MessageSend( vSubtask, "ZO00137", "LOD Maintenance",
                      "Source File Name is required.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "ssSourceFile" );
         return( -2 );
      }

      // check duplicate Source File Names
      SetCursorFirstEntityByString( vSaveAsCopy, "SourceFile", "SaveAsName",
                                    szNewName, "" );
      if ( SetCursorNextEntityByString( vSaveAsCopy, "SourceFile", "SaveAsName",
                                        szNewName, "" ) >= zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Source File Name '" );
         strcat_s( szMsg, sizeof( szMsg ), szNewName );
         strcat_s( szMsg, sizeof( szMsg ), "' is not unique." );
         MessageSend( vSubtask, "ZO00137", "LOD Maintenance",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "ssSourceFile" );
         DropView( vSaveAsCopy );
         return( -2 );
      }

   } // endfor ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );

   DropView( vSaveAsCopy );

   return( 0 );
} //zwfnTZZOLODD_SaveAsCheckFileName

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsCheckName
//
static zSHORT
zwfnTZZOLODD_SaveAsCheckName( zVIEW    vSubtask,
                              zVIEW    vSaveAs,
                              zVIEW    vTZZOLODO,
                              zPCHAR   szOutName )
{
   zVIEW  vDialog_LPLR;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szDLLName[33];
   zCHAR  szNewName[33];

   //Name is required
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "LOD Name is required." );
      MessageSend( vSubtask, "ZO00137", "Zeidon Objects Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edLODName" );
      return( -2 );
   }

   // DLL Name is required
   GetStringFromAttribute( szNewName, sizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "DLL_Name" );
   UfCompressName( szNewName, szDLLName, 32, "", "", "", "", 0 );
   if ( zstrcmp( szDLLName, "" ) == 0 &&
        CheckExistenceOfEntity( vTZZOLODO, "SourceFile" ) >= zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Source File exists and DLL Name is required." );
      MessageSend( vSubtask, "ZO00137", "LOD Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edDLLName" );
      return( -2 );
   }

   // There must be no Dialog with same name.
   RetrieveViewForMetaList( vSubtask, &vDialog_LPLR, zREFER_DIALOG_META );
   if ( SetCursorFirstEntityByString( vDialog_LPLR,
                                      "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "PN00221", "LOD Maintenance",
                   "A Dialog with the same name exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edLODName" );
      return( -2 );
   }
   DropView( vDialog_LPLR );

   if ( zwfnTZZOLODD_SaveAsCheckFileName( vSubtask, vSaveAs ) < 0 )
   {
      return( -2 );
   }

   return( 0 );
} // zwfnTZZOLODD_SaveAsCheckName

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsCheckStatus
//
static zSHORT
zwfnTZZOLODD_SaveAsCheckStatus( zVIEW    vSubtask,
                                zVIEW    vTZZOLFLO,
                                zPCHAR   szOutName )
{
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   //check the check out state
   if ( CompareAttributeToInteger( vTZZOLFLO, "W_MetaDef",
                                   "Status", 1 ) != 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "LOD '" );
      strcat_s( szMsg, sizeof( szMsg ), szOutName );
      strcat_s( szMsg, sizeof( szMsg ), "' is not checked out." );
      MessageSend( vSubtask, "ZO00137", "Zeidon Objects Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edLODName" );
      return( -1 );
   }

   return( 0 );
} // zwfnTZZOLODD_SaveAsCheckStatus

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsGetFileName
//
//    PARAMETERS:
//    szSourceFileName : size of szSourceFileName has to be at least zMAX_FILESPEC_LTH+1
//
static zSHORT
zwfnTZZOLODD_SaveAsGetFileName( zVIEW  vTaskLPLR,
                                zVIEW  vView,
                                zPCHAR szAttribute,
                                zPCHAR szSourceFileName )
{
   zCHAR    szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zCHAR    szNewName[33];
   zCHAR    szFileName[33];

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vView, "SourceFile", szAttribute );
   UfCompressName( szNewName, szFileName, 32, "", "", "", "", 0 );

   GetStringFromAttribute( szSourceFileName, sizeof( szSourceFileName ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   GetStringFromAttributeByContext(   szExtension, vView, "SourceFile",
                                    "LanguageType", "LanguageType", zMAX_EXTENSION_LTH );

   ofnTZCMWKSO_AppendSlash( szSourceFileName );
   strcat_s( szSourceFileName, sizeof( szSourceFileName ), szFileName );
   strcat_s( szSourceFileName, sizeof( szSourceFileName ), "." );
   strcat_s( szSourceFileName, sizeof( szSourceFileName ), szExtension );

   return( 0 );
} // zwfnTZZOLODD_SaveAsGetFileName

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsCopyFiles
//
static zSHORT
zwfnTZZOLODD_SaveAsCopyFiles( zVIEW    vSubtask,
                              zVIEW    vTZZOLODO,
                              zVIEW    vSaveAs )
{
   zVIEW    vTaskLPLR;
   zVIEW    vSourceFile;
   zLONG    hFile;
   zSHORT   nRC;
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR    szSourceFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szNewFileName[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   strcpy_s( szSourceFileName, sizeof( szSourceFileName ), "" );

   // Source File Name already exists
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      zwfnTZZOLODD_SaveAsGetFileName( vTaskLPLR, vSaveAs, "SaveAsName", szNewFileName );
      SetAttributeFromInteger( vSaveAs, "SourceFile", "ReplaceFile", 1 );

      hFile = SysOpenFile( vSubtask, szNewFileName, COREFILE_EXIST );
      if ( hFile == 1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Source File '" );
         strcat_s( szMsg, sizeof( szMsg ), szNewFileName );
         strcat_s( szMsg, sizeof( szMsg ), "' already exists. Replace existing File?" );
         nRC = MessagePrompt( vSubtask, "ZO00138", "LOD Maintenance",
                              szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_YES, zICON_QUESTION );

         if ( nRC == zRESPONSE_CANCEL )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            SetFocusToCtrl( vSubtask, "ssSourceFile" );
            return( -1 );
         }
         else
         {
            if ( nRC == zRESPONSE_NO )
               SetAttributeFromInteger( vSaveAs, "SourceFile", "ReplaceFile", 0 );
         }
      }
   } // endfor ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );

   // Set new Description and DLL Name
   SetAttributeFromAttribute(   vTZZOLODO, "LOD", "Desc",
                              vSaveAs, "ActionAfterSaveAS", "Desc" );
   SetAttributeFromAttribute(   vTZZOLODO, "LOD", "DLL_Name",
                              vSaveAs, "ActionAfterSaveAS", "DLL_Name" );

   // copy Source Files and set new Source File Name
   CreateViewFromViewForTask( &vSourceFile, vTZZOLODO, 0 );
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      zwfnTZZOLODD_SaveAsGetFileName( vTaskLPLR, vSaveAs, "SourceName",
                                      szSourceFileName );
      zwfnTZZOLODD_SaveAsGetFileName( vTaskLPLR, vSaveAs, "SaveAsName",
                                      szNewFileName );

      if ( CompareAttributeToInteger(   vSaveAs, "SourceFile", "ReplaceFile", 1 ) == 0 )
         SysCopyFile( vSubtask, szSourceFileName, szNewFileName, TRUE );

      // set new Source FIle Name
      if ( SetCursorFirstEntityByAttr(   vSourceFile, "SourceFile", "Name",
                                       vSaveAs, "SourceFile", "SourceName",
                                       "" ) == zCURSOR_SET )
      {
         SetAttributeFromAttribute(   vSourceFile, "SourceFile", "Name",
                                    vSaveAs, "SourceFile", "SaveAsName" );
      }
   }
   DropView( vSourceFile );

   return( 0 );
} // zwfnTZZOLODD_SaveAsCopyFiles

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveAsLOD
//
// save LOD under another name
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveAsLOD( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZZOLFLO;
   zVIEW  vTZZOLODO;
   zVIEW  vSaveAs;
   zCHAR  szNewName[ 33 ];
   zCHAR  szOutName[ 33 ];
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Validate LOD Name is OK
   GetStringFromAttribute( szNewName, sizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   //Name is required
   if ( zwfnTZZOLODD_SaveAsCheckName( vSubtask, vSaveAs,
                                      vTZZOLODO, szOutName ) < 0 )
   {
      return( -1 );
   }

   // Name must start with a "q".
   if ( *szNewName != 'q' )
   {
      MessageSend( vSubtask, "ZO00001", "Query LOD Maintenance",
                   "The LOD name must begin with a 'q'.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Name already exists
   if ( SetCursorFirstEntityByString( vTZZOLFLO, "W_MetaDef", "Name",
                                     szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "LOD already exists. Replace existing LOD?" );
      nRC = MessagePrompt( vSubtask, "ZO00138", "Zeidon Objects Maintenance",
                           szMsg, zBEEP, zBUTTONS_YESNO,
                           zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edLODName" );
         return( -1 );
      }

      //if this LOD open, then save LOD, not save as
      if ( CompareAttributeToString( vTZZOLODO, "LOD", "Name", szOutName ) == 0 )
      {
         if ( zwfnTZZOLODD_SaveAsCopyFiles( vSubtask, vTZZOLODO, vSaveAs ) < 0 )
            return( -1 );

         SetAttributeFromString( vTZZOLODO, "LOD", "Name", szOutName );
         zwTZZOLODD_SaveLOD( vSubtask );
         return( 0 );
      }
      else
      {
         // if another LOD open, then replace existing LOD
         nRC = DeleteMetaOI( vSubtask, vTZZOLFLO, zSOURCE_LOD_META );
      }
   }

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", szOutName );

   if ( zwfnTZZOLODD_SaveAsLOD( vSubtask, szOutName ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edLODName" );
      return( -1 );
   }
   zwfnTZZOLODD_CheckActionAfterSaveAs( vSubtask );

   return( 0 );
} // zwTZZOLODD_SaveAsLOD

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsAssignOperation
//
static zSHORT
zwfnTZZOLODD_SaveAsAssignOperation( zVIEW   vNewLOD,
                                    zVIEW   vOldLOD )
{
   zSHORT nRC;

   // assign Object Contraint Operation
   if ( CheckExistenceOfEntity( vOldLOD, "LOD_ConstraintOper" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByAttr( vNewLOD, "Operation", "Name",
                                    vOldLOD, "LOD_ConstraintOper", "Name",
                                    "LOD" ) >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vNewLOD, "LOD_ConstraintOper",
                                     vNewLOD, "Operation", zPOS_AFTER );
   }

   // assign Entity Contraint Operation
   nRC = SetCursorFirstEntity( vOldLOD, "LOD_Entity", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( CheckExistenceOfEntity( vOldLOD, "LOD_EntityConstraintOper" ) >= zCURSOR_SET &&
           SetCursorFirstEntityByAttr( vNewLOD, "LOD_Entity", "Name",
                                       vOldLOD, "LOD_Entity", "Name",
                                       "LOD" ) >= zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vNewLOD, "Operation", "Name",
                                     vOldLOD, "LOD_EntityConstraintOper", "Name", "LOD" );
         IncludeSubobjectFromSubobject( vNewLOD, "LOD_EntityConstraintOper",
                                        vNewLOD, "Operation", zPOS_AFTER );
      }

      // assign Derived Attribute Operation
      nRC = SetCursorFirstEntity( vOldLOD, "LOD_Attribute", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( CheckExistenceOfEntity( vOldLOD, "LOD_AttrDerivationOper" ) >= zCURSOR_SET &&
              SetCursorFirstEntityByAttr( vNewLOD, "LOD_Entity", "Name",
                                          vOldLOD, "LOD_Entity", "Name",
                                          "LOD" ) >= zCURSOR_SET &&
              SetCursorFirstEntityByAttr( vNewLOD, "ER_Attribute", "Name",
                                          vOldLOD, "ER_Attribute", "Name",
                                          "LOD_Entity" ) >= zCURSOR_SET )
         {
            SetCursorFirstEntityByAttr( vNewLOD, "Operation", "Name",
                                        vOldLOD, "LOD_AttrDerivationOper", "Name", "LOD" );
            IncludeSubobjectFromSubobject( vNewLOD, "LOD_AttrDerivationOper",
                                          vNewLOD, "Operation", zPOS_AFTER );
         }

         nRC = SetCursorNextEntity( vOldLOD, "LOD_Attribute", "" );
      }

      nRC = SetCursorNextEntity( vOldLOD, "LOD_Entity", "" );
   }

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsSourceFiles
//
static zSHORT
zwfnTZZOLODD_SaveAsSourceFiles( zVIEW   vSubtask,
                                zVIEW   vNewLOD,
                                zVIEW   vOldLOD )
{
   zSHORT  nRC;

   // Copy each SourceFile subobject, with Operation information.
   nRC = SetCursorFirstEntity( vOldLOD, "SourceFile", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateMetaEntity( vSubtask, vNewLOD, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vNewLOD, "SourceFile",
                                   vOldLOD, "SourceFile", zSET_NULL );

      nRC = SetCursorFirstEntity( vOldLOD, "Operation", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CreateMetaEntity( vSubtask, vNewLOD, "Operation", zPOS_AFTER );
         SetMatchingAttributesByName( vNewLOD, "Operation",
                                      vOldLOD, "Operation", zSET_NULL );

         nRC = SetCursorFirstEntity( vOldLOD, "Parameter", "" );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            // Abort with error message if the Parameter.ShortDesc attribute is
            // null, since ShortDesc is a required attribute and we don't want
            // to continue to migrate a Dialog with an error.
            if ( CompareAttributeToString( vOldLOD, "Parameter", "ShortDesc",
                 "" ) == 0 )
            {
               SetAttributeFromString( vOldLOD, "Parameter", "ShortDesc", "Subtask" );
            }

            CreateMetaEntity( vSubtask, vNewLOD, "Parameter", zPOS_AFTER );
            SetMatchingAttributesByName( vNewLOD, "Parameter",
                                         vOldLOD, "Parameter", zSET_NULL );

            nRC = SetCursorNextEntity( vOldLOD, "Parameter", "" );
         }

         nRC = SetCursorNextEntity( vOldLOD, "Operation", "" );
      }

      nRC = SetCursorNextEntity( vOldLOD, "SourceFile", "" );
   }

   // assignment to Operation
   zwfnTZZOLODD_SaveAsAssignOperation( vNewLOD, vOldLOD );

   return( 0 );
} // zwfnTZZOLODD_SaveAsSourceFiles

static zSHORT
zwfnTZZOLODD_Get_DTE( zVIEW vSubtask, zPVIEW  vDTE )
{
   zVIEW  vT;
   zSHORT nRC;

   nRC = GetViewByName( vDTE, "TZTENVRO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = GetViewByName( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         RetrieveViewForMetaList( vSubtask, &vT, zREFER_DTE_META );
         nRC = ActivateMetaOI( vSubtask, vDTE, vT, zREFER_DTE_META,
                               zSINGLE | zLEVEL_APPLICATION );
      }
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZZOLODD_SaveAsLOD
//
// Save LOD as
/////////////////////////////////////////////////////////////////////////////
static zSHORT
zwfnTZZOLODD_SaveAsLOD( zVIEW  vSubtask,
                        zPCHAR szLOD_Name )
{
   zVIEW  vLOD_Target;
   zVIEW  vLOD_TargetRecur;
   zVIEW  vLOD_Source;
   zVIEW  vLOD_SourceRecur;
   zVIEW  vTZZOLFLO;
   zVIEW  vTZZOLFLO_Target;
   zVIEW  vTZZOLFLO_Source;
   zVIEW  vLPLR;
   zVIEW  vERD;
   zVIEW  vDTE = 0;
   zVIEW  vTaskLPLR;
   zVIEW  vDialogLPLR;
   zVIEW  vSaveAs;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // A name must be entered.
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_Source, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // There must be no Dialog with same name.
   RetrieveViewForMetaList( vSubtask, &vDialogLPLR, zREFER_DIALOG_META );
   if ( SetCursorFirstEntityByString( vDialogLPLR,
                                      "W_MetaDef", "Name",
                                      szLOD_Name, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "A Dialog with the same name exists." );
      MessageSend( vSubtask, "ZO00165", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      DropView( vDialogLPLR );
      return( -2 );
   }

   DropView( vDialogLPLR );

   // Activate an empty LOD Object
   nRC = ActivateEmptyMetaOI( vSubtask, &vLOD_Target, zSOURCE_LOD_META,
                              zSINGLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwfnTZZOLODD_SaveAsLOD) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00109", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   // Sort Attributes in original sequence
   oTZZOLODO_SortAttributes( vLOD_Source );

   // Copy the source LOD to the new LOD.
   CreateMetaEntity( vSubtask, vLOD_Target, "LOD", zPOS_AFTER );
   SetMatchingAttributesByName( vLOD_Target, "LOD",
                                vLOD_Source, "LOD", zSET_NULL );
   SetAttributeFromAttribute( vLOD_Target, "LOD", "Name",
                              vSaveAs, "ActionAfterSaveAS", "SaveAsName8");

   // Reuse the copy routine from LPLR migrate.
   // Create the necessary extra views.
   CreateViewFromViewForTask( &vLOD_TargetRecur, vLOD_Target, 0 );
   CreateViewFromViewForTask( &vLOD_SourceRecur, vLOD_Source, 0 );
   CreateViewFromViewForTask( &vTZZOLFLO_Target, vTZZOLFLO, 0 );
   CreateViewFromViewForTask( &vTZZOLFLO_Source, vTZZOLFLO, 0 );
   zwfnTZZOLODD_Get_DTE( vSubtask, &vDTE );

   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   ActivateMetaOI( vSubtask, &vERD, vLPLR, zSOURCE_ERD_META,
                   zSINGLE | zLEVEL_APPLICATION );

   for ( nRC = SetCursorFirstEntity( vLOD_Source, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_Source, "LOD_EntityParent", 0 ) )
   {
      oTZZOLODO_LOD_EntityCopy( vLOD_Target, vLOD_TargetRecur,
                                vLOD_Source, vLOD_SourceRecur,
                                vTZZOLFLO_Source, vTZZOLFLO_Target, vERD, 0,
                                vDTE, vSubtask, "", "N" );
   }

   // If the source has a POD, copy it.
   if ( CheckExistenceOfEntity( vLOD_Source, "POD" ) >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vLOD_Target, "POD", zPOS_AFTER );
      SetMatchingAttributesByName( vLOD_Target, "POD",
                                   vLOD_Source, "POD", zSET_NULL );
      IncludeSubobjectFromSubobject( vLOD_Target, "TE_DB_Environ",
                                     vLOD_Source, "TE_DB_Environ",
                                     zPOS_AFTER );
   }

   //BL, 1999.10.26 Copy LOD Operations
   zwfnTZZOLODD_SaveAsSourceFiles( vSubtask, vLOD_Target, vLOD_Source );

   // Commit the new LOD.
   SetNameForView( vLOD_Target, "TZZOLODO", vSubtask, zLEVEL_TASK );
   zwTZZOLODD_SaveLOD( vSubtask );

   //Set Cursor to new LOD and open this LOD
   if ( SetCursorFirstEntityByString( vTZZOLFLO, "W_MetaDef", "Name",
                                      szLOD_Name, 0 ) == zCURSOR_SET )
      zwTZZOLODD_OpenLOD( vSubtask );

   // Drop views.
   DropView( vLPLR );
   DropMetaOI( vSubtask, vERD );
   DropView( vTZZOLFLO_Target );
   DropView( vTZZOLFLO_Source );
   DropMetaOI( vSubtask, vLOD_Source );

   return( 0 );
} // zwfnTZZOLODD_SaveAsLOD

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_CreateTemporalEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_CreateTemporalEntity( zVIEW vSubtask )
{
   zVIEW vLOD;

   GetViewByName( &vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vLOD, "LOD_Attribute" );

   return( 0 );
} // zwTZZOLODD_CreateTemporalEntity

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveAsPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_SaveAsPostBuild( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;
   zVIEW    vTZZOLFLO;
   zVIEW    vSaveAs;
   zVIEW    vSourceFile;
   zSHORT   nRC;

   zwTZZOLODD_LoadLODList ( vSubtask );

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZZOLFLO, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZZOLFLO, "W_MetaDef", 0 );

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );
   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "DLL_Name", "" );

   //set current Description
   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );
   SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "Desc",
                              vTZZOLODO, "LOD", "Desc" );

   // delete old Source Files
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      nRC = DeleteEntity( vSaveAs, "SourceFile", zREPOS_NONE );
   }

   // create new source Files
   CreateViewFromViewForTask( &vSourceFile, vTZZOLODO, 0 );
   for ( nRC = SetCursorFirstEntity( vSourceFile, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSourceFile, "SourceFile", "" ) )
   {
      CreateEntity( vSaveAs, "SourceFile", zPOS_AFTER );
      SetAttributeFromAttribute(   vSaveAs, "SourceFile", "SourceName",
                                 vSourceFile, "SourceFile", "Name" );
      SetAttributeFromAttribute(   vSaveAs, "SourceFile", "LanguageType",
                                 vSourceFile, "SourceFile", "LanguageType" );
   }
   DropView( vSourceFile );

   if ( CheckExistenceOfEntity( vSaveAs, "SourceFile" ) >= zCURSOR_SET )
      OrderEntityForView( vSaveAs, "SourceFile", "LanguageType D SourceName A" );

   return( 0 );
} // zwTZZOLODD_SaveAsPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: AddParentEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_AddParentEntity( zVIEW vSubtask )
{
   zVIEW    vLOD_Tgt;
   zVIEW    vLOD_Src;
   zVIEW    vLOD_TgtRecur;
   zVIEW    vLOD_SrcRecur;
   zVIEW    vTZZOLFLO;
   zVIEW    vTZZOLFLO_Tgt;
   zVIEW    vTaskLPLR;
   zVIEW    vLPLR;
   zVIEW    vERD;
   zSHORT   nRC;
   zCHAR    szMsg[ zSHORT_MESSAGE_LTH + 1 ];

   // Get the current LOD.
   nRC = GetViewByName( &vLOD_Src, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Activate an empty LOD Object
   nRC = ActivateEmptyMetaOI( vSubtask, &vLOD_Tgt, zSOURCE_LOD_META,
                              zSINGLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "(zwTZZOLODD_AddParentEntity) Unable to Access LOD. LOD must be opened." );
      MessageSend( vSubtask, "ZO00109", "Zeidon Object Maintenance",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   // Copy the source LOD root entity to the new LOD root entity.
   nRC = CreateEntity( vLOD_Tgt, "LOD", zPOS_AFTER );
   SetMatchingAttributesByName( vLOD_Tgt, "LOD",
                                vLOD_Src, "LOD", zSET_ALL );
   SetNameForView( vLOD_Tgt, "TZZOLODO", vSubtask, zLEVEL_TASK );
   nRC = CreateMetaEntity( vSubtask, vLOD_Tgt, "LOD_EntityParent", zPOS_AFTER );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent",
                           "Name", "NewParentWork" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent",
                           "Desc", "NewParent Description" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Create", "Y" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Delete", "Y" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Update", "Y" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Include", "N" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "InclSrc", "Y" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Exclude", "N" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Recursive", "N" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Derived", "N" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent", "Work", "Y" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent",
                           "ParentDeleteBehave", "D" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent",
                           "DupEntityInstance", "N" );
   SetAttributeFromString( vLOD_Tgt, "LOD_EntityParent",
                           "DupRelationshipInstance", "N" );

   nRC = IncludeSubobjectFromSubobject( vLOD_Tgt, "LOD_Entity",
                                        vLOD_Tgt, "LOD_EntityParent",
                                        zPOS_AFTER );

   // Reuse the copy routine from LPLR migrate.
   // Create the necessary extra views.
   CreateViewFromViewForTask( &vLOD_TgtRecur, vLOD_Tgt, 0 );
   SetViewToSubobject( vLOD_TgtRecur, "LOD_EntityChild" );
   CreateViewFromViewForTask( &vLOD_SrcRecur, vLOD_Src, 0 );
   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZZOLFLO_Tgt, vTZZOLFLO, 0 );
   RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );
   ActivateMetaOI( vSubtask, &vERD, vLPLR, zSOURCE_ERD_META,
                   zSINGLE | zLEVEL_APPLICATION );
   for ( nRC = SetCursorFirstEntity( vLOD_Src, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_Src, "LOD_EntityParent", 0 ) )
   {
      oTZZOLODO_LOD_EntityCopy( vLOD_Tgt, vLOD_TgtRecur,
                                vLOD_Src, vLOD_SrcRecur,
                                vTZZOLFLO, vTZZOLFLO_Tgt, vERD, 0, vSubtask, 0, "", "N" );
   }

   // If the source has a POD, copy it.
   if ( CheckExistenceOfEntity( vLOD_Src, "POD" ) >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vLOD_Tgt, "POD", zPOS_AFTER );
      SetMatchingAttributesByName( vLOD_Tgt, "POD",
                                   vLOD_Src, "POD", zSET_NULL );
      IncludeSubobjectFromSubobject( vLOD_Tgt, "TE_DB_Environ",
                                     vLOD_Src, "TE_DB_Environ",
                                     zPOS_AFTER );
   }

   //BL, 1999.10.26 Copy LOD Operations
   oTZZOLODO_LOD_OperationCopy( vSubtask, vLOD_Tgt, vLOD_Src, vTaskLPLR );

   // Drop views.
   DropView( vLPLR );
   DropMetaOI( vSubtask, vERD );
   DropView( vTZZOLFLO_Tgt );

   if ( vLOD_Src )
      DropMetaOI( vSubtask, vLOD_Src );

   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );

   return( 0 );
}

#define zCHECK_TIMESTAMP_ONLY       0x10000000

// z0100-Zeidon    TEMP     TZZOLODO 03/03/99   13:57:24 1.0a
// z10000Zeidon    ACCOUNT  TZZOXODO 04/04/00   04:49:44 1.0a
// z1000-Zeidon    ACCOUNT  TZWDLGSO 04/18/07   09:18:42 1.0a2
zSHORT
fnLocateLODDate( zPCHAR pchBuffer )
{
   zPCHAR pchColon2;
   zPCHAR pchColon1;
   zPCHAR pchSlash2;
   zPCHAR pchSlash1;
   zLONG  lLth = zstrlen( pchBuffer );
   zLONG  k;

   pchColon2 = zstrchr( pchBuffer, '\n' );
   if ( pchColon2 )
      *pchColon2 = 0;

   pchColon2 = zstrchr( pchBuffer, '\r' );
   if ( pchColon2 )
      *pchColon2 = 0;

   pchColon2 = zstrrchr( pchBuffer, ':' );
   if ( pchColon2 )
   {
      *pchColon2 = 0;
      pchColon1 = zstrrchr( pchBuffer, ':' );
      if ( pchColon1 )
      {
         pchSlash2 = zstrrchr( pchBuffer, '/' );
         if ( pchSlash2 )
         {
            *pchSlash2 = 0;
            pchSlash1 = zstrrchr( pchBuffer, '/' );
            if ( pchSlash1 && pchSlash1 + 3 == pchSlash2 &&
                 pchColon1 + 3 == pchColon2 )
            {
               *pchColon2 = ':';
               *(pchColon2 + 3) = 0;
               k = zatol( pchSlash2 + 1 );
               if ( k > 80 )
               {
                  pchBuffer[ 0 ] = '1';
                  pchBuffer[ 1 ] = '9';
               }
               else
               {
                  pchBuffer[ 0 ] = '2';
                  pchBuffer[ 1 ] = '0';
               }

               strcpy_s( pchBuffer + 2, lLth - 2, pchSlash2 + 1 );
               pchBuffer[ 4 ] = '/';
               strcpy_s( pchBuffer + 5, lLth - 5, pchSlash1 - 2 );
               pchBuffer[ 10 ] = ' ';
               strcpy_s( pchBuffer + 11, lLth - 11, pchColon1 - 2 );
               return( 0 );   // we think we did this correctly
            }
         }
      }
   }

   return( -1 );   // there was some sort of problem
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_Check_CheckoutStatus
//
//    set check out state in window title
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_Check_CheckoutStatus( zVIEW vSubtask )
{
   zVIEW   vTZZOLFLO;
   zSHORT  nEnable = 0;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZZOLFLO, "QueryW_MetaDef" ) >= zCURSOR_SET )
   {
         nEnable = CheckOutStateForCurrentWindow( vSubtask, vTZZOLFLO );
         if ( nEnable )
            bEnable = TRUE;
   }

   SetCtrlState( vSubtask, "Delete", zOPTION_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "DeleteLOD", bEnable );

   return( 0 );
} // zwTZZOLODD_Check_CheckoutStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_CheckNameForCheckOut
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_CheckNameForCheckOut( zVIEW vSubtask )
{
   zSHORT  nEnable = 1;
   zVIEW   vTZZOLFLO;
   zVIEW   vTZZOLFLO_Copy;
   zCHAR   szNewName[33];
   zCHAR   szOutName[33];

   GetCtrlText( vSubtask, "edLODName", szNewName, 33 );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZZOLFLO_Copy, vTZZOLFLO, 0 );

   if ( SetCursorFirstEntityByString( vTZZOLFLO_Copy, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      // set check out state in Title
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vTZZOLFLO_Copy );
   }
   else
   {
      SetWindowCaptionTitle( vSubtask, "Save LOD as", "" );
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable );

   DropView( vTZZOLFLO_Copy );

   return( 0 );
} // zwTZZOLODD_CheckNameForCheckOut

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveAsCancel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_SaveAsCancel( zVIEW vSubtask )
{
   zVIEW   vSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );

   return( 0 );
} // zwTZZOLODD_SaveAsCancel

static zSHORT
zwfnTZZOLODD_CheckActionAfterSaveAs( zVIEW  vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vTZZOLODO;
   zVIEW   vParentWindow;
   zLONG   lActionAfterSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );


   // Set window Title with check out state
   GetParentWindow( &vParentWindow, vSubtask );
   SetTitleWithCheckOutState( vParentWindow, "Query View Maintenance",
                               "TZZOLODO", vTZZOLODO, "LOD", zSOURCE_LOD_META );

   // if LOD not checked out and user change this LOD, then zeidon
   // call the window "Save LOD as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new LOD or Open an other LOD or Switch Project or Exit
   // LOD Tool)

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   switch ( lActionAfterSaveAs )
   {
      case zNewComponentAfterSaveAs:   // Create new LOD
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZZOLODQ", "TZZONWLD" );
         break;

      case zOpenComponentAfterSaveAs:   // Open LOD
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZZOLODQ", "TZZOOPLD" );
         break;

      case zSwitchLPLRAfterSaveAs:   // Switch Project
         zwTZZOLODD_SwitchLPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZCMSLPD", "SwitchLPLR" );
         break;

      case zExitToolAfterSaveAs:   // Exit LOD Tool
         zwTZZOLODD_ExitLOD( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, "", "" );
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_NewLOD
//
//    create new LOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_NewLOD( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwfnTZZOLODD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if LOD not checked out and user change this LOD, then zeidon
            // call the window "Save LOD as"
            // We save the action after Save as in the View TZSAVEAS (e.g.
            // Create new LOD or Open an other LOD or Switch Project or
            // Exit LOD Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNewComponentAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   return( 0 );
} // zwTZZOLODD_NewLOD

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_LodOpen
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_LodOpen( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = zwfnTZZOLODD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // if LOD not checked out and user change this LOD, then zeidon
            // call the window "Save LOD as"
            // We save the action after Save as in the View TZSAVEAS (e.g.
            // Create new LOD or Open an other LOD or Switch Project or
            // Exit LOD Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zOpenComponentAfterSaveAs );
         }
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }

      return( 0 );
   }

   return( 0 );

} // zwTZZOLODD_LodOpen

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveSelectedAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_SaveSelectedAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // if LOD not checked out, View is read only
   if ( ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ) &&
        CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
   {
      SetAttributeFromAttribute( vTZZOLODO, "LOD_Entity", "wkSelectedAttributeForDragSort",
                                 vTZZOLODO, "LOD_Attribute", "wkSequence" );
   }

   zwTZZOLODD_RefreshAttributeList( vSubtask );
   return( 0 );
} // zwTZZOLODD_SaveSelectedAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveSelAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_SaveSelAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // if LOD not checked out, View is read only
   if ( ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ) &&
        CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
   {
      SetAttributeFromAttribute( vTZZOLODO, "LOD_Entity", "wkSelectedAttributeForDragSort",
                                 vTZZOLODO, "LOD_Attribute", "wkSequence" );
   }

   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
} // zwTZZOLODD_SaveSelAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_MoveAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_MoveAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;
   zVIEW    vTZZOLODO_Src;
   zVIEW    vTZZOLODO_Pos;
   zLONG    lSourceZKey;
   zLONG    lPosZKey;
   zSHORT   nRC;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   oTZZOLODO_MoveAttribInSequence( vTZZOLODO );

   // If the attribute just moved exists as QueryFrameAttribute entry, also move it.
   GetIntegerFromAttribute( &lSourceZKey, vTZZOLODO, "LOD_Attribute", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vTZZOLODO, "QueryFrameAttribute", "ZKey", lSourceZKey, 0 );
   if ( nRC >= zCURSOR_SET )
   {
      // Make sure all attributes exist as QueryFrameAttribute entries.
      zwTZZOLODD_QueryFrameSetUp( vSubtask );
      CreateViewFromViewForTask( &vTZZOLODO_Src, vTZZOLODO, 0 );
      CreateViewFromViewForTask( &vTZZOLODO_Pos, vTZZOLODO, 0 );
      SetCursorPrevEntity( vTZZOLODO_Src, "LOD_Attribute", 0 );
      GetIntegerFromAttribute( &lPosZKey, vTZZOLODO_Src, "LOD_Attribute", "ZKey" );
      SetCursorFirstEntityByInteger( vTZZOLODO_Src, "QueryFrameAttribute", "ZKey",
                                     lSourceZKey, 0 );
      SetCursorFirstEntityByInteger( vTZZOLODO_Pos, "QueryFrameAttribute", "ZKey",
                                     lPosZKey, 0 );
      CreateEntity( vTZZOLODO_Pos, "QueryFrameAttribute", zPOS_AFTER );
      SetMatchingAttributesByName( vTZZOLODO_Pos, "QueryFrameAttribute",
                                   vTZZOLODO_Src, "QueryFrameAttribute", zSET_ALL );
      DeleteEntity( vTZZOLODO_Src, "QueryFrameAttribute", zREPOS_NEXT );
      DropView( vTZZOLODO_Src );
      DropView( vTZZOLODO_Pos );
   }

   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
} // zwTZZOLODD_MoveAttribute

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: fnTZZOLODD_MoveEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnTZZOLODD_MoveEntity( zVIEW vSubtask, zCHAR cType )
{
   zVIEW    vTZZOLODO;
   zVIEW    vTZZOLOD1;
   zVIEW    vTZZOHDGO;
   zVIEW    vCurrent;
   zVIEW    vTarget;
   zCHAR    szEntityName[ 33 ];
   zCHAR    szHierEntityName[ 33 ];
   zSHORT   nRC;
   zUSHORT  lLevel;
   zINTEGER lSeqNo;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( !MiGetUpdateForView( vTZZOLODO ) )
      return( 0 );

   // We will position on the recursive side of the LOD and then use that
   // position to move the selected entity left or right, unless the entity
   // is already the left-most or right-most entity.

   CreateViewFromViewForTask( &vTZZOLOD1, vTZZOLODO, 0 );
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ),
                           vTZZOLODO, "LOD_Entity", "Name" );
   DefineHierarchicalCursor( vTZZOLOD1, "LOD" );

   // First loop to find the selected entity in the recursive side.
   nRC = SetCursorNextEntityHierarchical( &lLevel,
                                          szHierEntityName, vTZZOLOD1 );
   while ( nRC >= zCURSOR_SET )
   {
      if ( zstrcmp( szHierEntityName, "LOD_EntityChild" ) == 0 )
      {
         SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
         GetStringFromAttribute( szHierEntityName, sizeof( szHierEntityName ),
                                 vTZZOLOD1, "LOD_EntityParent", "Name" );
         if ( zstrcmp( szHierEntityName, szEntityName ) == 0 )
            break;
      }

      nRC = SetCursorNextEntityHierarchical( &lLevel,
                                             szHierEntityName, vTZZOLOD1 );
   }

   CreateViewFromViewForTask( &vCurrent, vTZZOLOD1, 0 );
   CreateViewFromViewForTask( &vTarget, vTZZOLOD1, 0 );

   if ( cType == 'L' )
      nRC = SetCursorPrevEntity( vTarget, "LOD_EntityParent", 0 );
   else
      nRC = SetCursorNextEntity( vTarget, "LOD_EntityParent", 0 );

   if ( nRC >= zCURSOR_SET )
   {
      // Swap SeqNo values and move entity.
      GetIntegerFromAttribute( &lSeqNo,
                               vCurrent, "LOD_EntityParent", "SeqNo" );
      SetAttributeFromAttribute( vCurrent,  "LOD_EntityParent", "SeqNo",
                                 vTarget,   "LOD_EntityParent", "SeqNo" );
      SetAttributeFromInteger( vTarget, "LOD_EntityParent", "SeqNo",
                               lSeqNo );

      if ( cType == 'L' )
         MoveSubobject( vTarget,  "LOD_EntityParent",
                        vCurrent, "LOD_EntityParent",
                        zPOS_BEFORE, zREPOS_FIRST );
      else
         MoveSubobject( vTarget,  "LOD_EntityParent",
                        vCurrent, "LOD_EntityParent",
                        zPOS_AFTER, zREPOS_FIRST );
   }

   DropView( vTZZOLOD1 );
   DropView( vCurrent );
   DropView( vTarget );

   GetViewByName( &vTZZOHDGO, "TZZOHDGO", vSubtask, zLEVEL_TASK );
   zfnTZZOLODD_MergeLOD_WithDiagram( vSubtask, vTZZOLODO, vTZZOHDGO );
   RefreshCtrl( vSubtask, "HierDiag1" );
   HD_SelectNode( vSubtask, "HierDiag1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZZOLODD_MoveEntityLeft
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_MoveEntityLeft( zVIEW vSubtask )
{

   fnTZZOLODD_MoveEntity( vSubtask, 'L' );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZZOLODD_MoveEntityRight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZOLODD_MoveEntityRight( zVIEW vSubtask )
{

   fnTZZOLODD_MoveEntity( vSubtask, 'R' );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveAsSetDefaults
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_SaveAsSetDefaults( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zVIEW  vTZZOLFLO;
   zVIEW  vTZLodList;
   zCHAR  szNewName[33];
   zCHAR  szOutName[33];
   zSHORT nRC;
   zSHORT nIndex = -1;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vTZLodList, vTZZOLFLO, 0 );

   // if LOD Name not required, set default value
   GetStringFromAttribute( szNewName, sizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 32, "", "", "", "", 0 );

   //Name is required
   if ( zstrcmp( szOutName, "" ) != 0 )
   {
      SetAttributeFromString(   vSaveAs, "ActionAfterSaveAS", "DLL_Name", szOutName );

      for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
      {
         // if Lod in LodList, call zwTZPNTRAD_SetSaveAsName
         if ( SetCursorFirstEntityByString( vTZLodList, "W_MetaDef", "Name",
                                            szOutName, "" ) >= zCURSOR_SET )
         {
            zwTZZOLODD_SetSaveAsName( vSubtask );
         }
         else
         {
            // if Lod not in LodList, create all new Source
            // File Names
            nIndex = zwfnTZZOLODD_SaveAsSetSourceName( vSubtask, vSaveAs,
                                                       0, nIndex );
         }
      }
   } // endif ( zstrcmp( szOutName, "" ) != 0 )

   return( 0 );
} // zwTZZOLODD_SaveAsSetDefaults

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SaveAsKeepCurrentDesc
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zVIEW  vSaveAs;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vTZZOLODO, "LOD", "Desc" );
      RefreshCtrl( vSubtask, "edDesc" );
   }

   return( 0 );
} // zwTZZOLODD_SaveAsKeepCurrentDesc

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_LoadEntityList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_LoadEntityList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZZOLODD_LoadEntityList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_LoadAttributeList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_LoadAttributeList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "ATTVIEW" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZZOLODD_LoadAttributeList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_LoadOperationList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_LoadOperationList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZZOLODD_LoadOperationList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_LoadSourceFileList
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_LoadSourceFileList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" );
   if ( vWindow )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // zwTZZOLODD_LoadSourceFileList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_EntityAttributePopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_EntityAttributePopup( zVIEW vSubtask )
{
   zVIEW   vTZZOLODO;
   zSHORT  nIsCheckedOut = 0;
   zBOOL   bOpen         = FALSE;
   zBOOL   bNew          = FALSE;
   zBOOL   bNewER_Attrib = FALSE;
   zBOOL   bDelete       = FALSE;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META );

      if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
      {
         bNew = TRUE;
         if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) != 0 )
            bNewER_Attrib = TRUE;
      }

      EnableAction( vSubtask, "AddWorkAttribute", bNew );
      EnableAction( vSubtask, "DeleteAttribute", bDelete );
      EnableAction( vSubtask, "DeleteAllAttributes", bDelete );
      EnableAction( vSubtask, "AddAllER_Attribs", bNewER_Attrib );
      EnableAction( vSubtask, "ShowPriorityList", bOpen );

      CreateTrackingPopupMenu( vSubtask, "AttributePopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZZOLODD_EntityAttributePopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_NextAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_NextAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Accept the current Attribute subobject.

   if ( AcceptSubobject( vTZZOLODO, "LOD_Attribute" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   SetSelectSetForView( vTZZOLODO, 1 );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 0 );
   SetCursorNextEntity( vTZZOLODO, "LOD_Attribute", 0 );

   // Use a temporal subobject version for updating the Attribute subobject.
   CreateTemporalSubobjectVersion( vTZZOLODO, "LOD_Attribute" );
   zwTZZOLODD_PreBuildATTMAINT( vSubtask );
   zwTZZOLODD_PostBuildATTMAINT( vSubtask );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 1 );

   SetFocusToCtrl( vSubtask, "Name" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
} // zwTZZOLODD_NextAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_PrevAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_PrevAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   // Accept the current Attribute subobject.

   if ( AcceptSubobject( vTZZOLODO, "LOD_Attribute" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      SetFocusToCtrl( vSubtask, "Name" );
      return( -1 );
   }

   SetSelectSetForView( vTZZOLODO, 1 );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 0 );
   SetCursorPrevEntity( vTZZOLODO, "LOD_Attribute", 0 );

   // Use a temporal subobject version for updating the Attribute subobject.
   CreateTemporalSubobjectVersion( vTZZOLODO, "LOD_Attribute" );
   zwTZZOLODD_PreBuildATTMAINT( vSubtask );
   zwTZZOLODD_PostBuildATTMAINT( vSubtask );
   SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 1 );

   SetFocusToCtrl( vSubtask, "Name" );

   // Refresh Attribute and Entity List Window
   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
} // zwTZZOLODD_PrevAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_CancelAttribute
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_CancelAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOLODO;

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   SetSelectSetForView( vTZZOLODO, 1 );
   CancelSubobject( vTZZOLODO, "LOD_Attribute" );

   if ( CheckExistenceOfEntity( vTZZOLODO, "LOD_Attribute" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZZOLODO, "LOD_Attribute", 1 );

   zwTZZOLODD_RefreshAttributeList( vSubtask );
   zwTZZOLODD_RefreshEntityList( vSubtask );

   return( 0 );
} // zwTZZOLODD_CancelAttribute

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_WarningForWorkEntity
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_WarningForWorkEntity( zVIEW vSubtask )
{
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zVIEW  vTZZOLODO;
   zULONG ulZKey;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) == 0 )
   {
      GetIntegerFromAttribute( (zLONG*)&ulZKey,
                               vTZZOLODO, "LOD_Entity", "ZKey" );

      if ( zwfnTZZOLODD_IfPOD_Entity( vTZZOLODO, ulZKey, FALSE ) == 1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Setting the Entity Type to 'Work' will cause all \n" );
         strcat_s( szMsg, sizeof( szMsg ), "Attributes to be turned to Type 'Work' and \n" );
         strcat_s( szMsg, sizeof( szMsg ), "all Properites for Join Definition will be deleted.\n\n" );
         strcat_s( szMsg, sizeof( szMsg ), "If you accept the change, it will be irreverible!" );
      }
      else
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Setting the Entity Type to 'Work' will cause all \n" );
         strcat_s( szMsg, sizeof( szMsg ), "Attributes to be turned to Type 'Work'.\n\n" );
         strcat_s( szMsg, sizeof( szMsg ), "If you accept the change, it will be irreverible!" );
      }

      MessagePrompt( vSubtask, "ZO00181", "Zeidon Objects Maintenance",
                     szMsg, zBEEP, zBUTTONS_OK,
                     zRESPONSE_OK, zICON_EXCLAMATION );

   }

   return( 0 );
} // zwTZZOLODD_WarningForWorkEntity

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_SetAttributeType
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZZOLODD_SetAttributeType( zVIEW vSubtask, zVIEW vTZZOLODO )
{
   // if Entity Type = Work, set type for all Attribute to Work
   if ( ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ) &&
        CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) == 0 )
   {
      zVIEW   vTZZOLODO_Copy;
      zVIEW   vLPLR;
      zVIEW   vERD = 0;
      zSHORT  nRC;
      zULONG  ulZKey;
      zSHORT  lERD_Flag;

      CreateViewFromViewForTask( &vTZZOLODO_Copy, vTZZOLODO, 0 );
      RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_ERD_META );

      // Check if there is an ERD, because we will skip certain relinking if
      // there is not.
      if ( CheckExistenceOfEntity( vLPLR, "W_MetaDef" ) >= zCURSOR_SET )
         lERD_Flag = ActivateMetaOI( vSubtask, &vERD, vLPLR, zREFER_ERD_META, 0 );
      else
         lERD_Flag = -1;

      DropView ( vLPLR );

      if ( lERD_Flag >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zLONG*)&ulZKey,
                                  vTZZOLODO, "ER_Entity", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vERD, "ER_Entity", "ZKey", ulZKey,
                                             "" ) < zCURSOR_SET )
         {
            lERD_Flag = -1;
         }
      }

      for ( nRC = SetCursorFirstEntity( vTZZOLODO_Copy, "LOD_Attribute", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZZOLODO_Copy, "LOD_Attribute", "" ) )
      {
         SetAttributeFromString( vTZZOLODO_Copy, "LOD_Attribute", "Work", "Y" );
         if ( lERD_Flag >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( (zLONG*)&ulZKey,
                                     vTZZOLODO_Copy, "ER_Attribute", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vERD, "ER_Attribute", "ZKey", ulZKey,
                                                "" ) >= zCURSOR_SET )
            {
               ExcludeEntity( vTZZOLODO_Copy, "ER_Attribute", zREPOS_NONE );
               CreateMetaEntity( vSubtask, vTZZOLODO_Copy, "ER_Attribute", zPOS_AFTER );
               SetMatchingAttributesByName( vTZZOLODO_Copy, "ER_Attribute",
                                            vERD, "ER_Attribute", zSET_NULL );
               IncludeSubobjectFromSubobject( vTZZOLODO_Copy, "Domain",
                                              vERD, "Domain", zPOS_AFTER );
            }
         }
      }

      DropView( vTZZOLODO_Copy );
      if ( vERD )
         DropMetaOI( vSubtask, vERD );
   }

   return( 0 );
} // zwTZZOLODD_SetAttributeType

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_EnableDBH_Info
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_EnableDBH_Info( zVIEW vSubtask )
{
   zVIEW vTZTEDBLO;
   zVIEW vTZTENVRO;
   zBOOL bEnable = FALSE;

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZTENVRO, "TZTENVRO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZTENVRO, "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                       vTZTENVRO, "TE_DBMS_Source", "DBMS",
                                       0 ) >= zCURSOR_SET )
      {
         // Check to see if there is a sub-dialog called "LOD_Main".  If there is, then
         // we need to enable the "Set DBH" button.
         if ( SetCursorFirstEntityByString( vTZTEDBLO, "DBS_SubDialog",
                                            "Type", "LOD_Main", 0 ) == zCURSOR_SET )
         {
            bEnable = TRUE;
         }
      }
   }

   SetCtrlState( vSubtask, "DBH_Info", zCONTROL_STATUS_ENABLED, bEnable );

   return( 0 );
   RefreshCtrl( vSubtask, "DBH_Info" );

   return( 0 );

} // zwTZZOLODD_EnableDBH_Info

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_TransferToSetDBH
//
//    Starts up a sub-dialog to set DBH-specific info
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_TransferToSetDBH( zVIEW vSubtask )
{
   zVIEW  vTZTEDBLO;
   zCHAR  szDialogName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szWindowName[ zZEIDON_NAME_LTH + 1 ];

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szDialogName, sizeof( szDialogName ), vTZTEDBLO, "DBS_SubDialog", "Dialog" );
   GetStringFromAttribute( szWindowName, sizeof( szWindowName ), vTZTEDBLO, "DBS_SubDialog", "Window" );

   SetWindowActionBehavior( vSubtask,
                            zWAB_StartModalSubwindow + zWAB_ProcessImmediateAction,
                            szDialogName, szWindowName );

   return( 0 );
} // zwTZZOLODD_TransferToSetDBH

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_WarningForDerivEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_WarningForDerivEntity( zVIEW vSubtask )
{
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zVIEW  vTZZOLODO;
   zULONG ulZKey;

   // Get Access to LOD Object
   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Derived", "Y" ) == 0 )
   {
      GetIntegerFromAttribute( (zLONG*)&ulZKey,
                               vTZZOLODO, "LOD_Entity", "ZKey" );

      if ( zwfnTZZOLODD_IfPOD_Entity( vTZZOLODO, ulZKey, FALSE ) == 1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Setting the Entity Type to 'Derived' will delete all \n" );
         strcat_s( szMsg, sizeof( szMsg ), "Properties for Join Definition.\n\n" );
         strcat_s( szMsg, sizeof( szMsg ), "If you accept the change, it will be irreverible!" );
         MessagePrompt( vSubtask, "ZO00182", "Zeidon Objects Maintenance",
                        szMsg, zBEEP, zBUTTONS_OK, zRESPONSE_OK, zICON_EXCLAMATION );
      }
   }

   return( 0 );
} // zwTZZOLODD_WarningForDerivEntity


static zSHORT
zwfnTZZOLODD_IfPOD_Entity( zVIEW vLOD, zULONG ulZKey, zBOOL bDeleteIt )
{
   zSHORT nRC;
   zVIEW  vLOD_Copy;

   CreateViewFromViewForTask( &vLOD_Copy, vLOD, 0 );
   ResetView( vLOD_Copy );

   nRC = SetCursorFirstEntity( vLOD_Copy, "LOD_EntityParent", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = zwfnTZZOLODD_FoundPOD_Entity( vLOD_Copy, ulZKey );
      if ( nRC >= 0 )
      {
         if ( bDeleteIt && nRC == 1 )
         {
            for ( nRC = SetCursorFirstEntity( vLOD_Copy, "POD_Entity", "" );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vLOD_Copy, "POD_Entity", "" ) )
            {
               DeleteEntity( vLOD_Copy, "POD_Entity", zREPOS_NONE );
            }
         }

         DropView( vLOD_Copy );
         return( nRC );
      }

      nRC = SetCursorNextEntity( vLOD_Copy, "LOD_EntityParent", "" );

   }

   DropView( vLOD_Copy );
   return( -1 );
}



static zSHORT
zwfnTZZOLODD_FoundPOD_Entity( zVIEW vLOD, zULONG ulZKey )
{
   zSHORT nRC;

   if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent", "ZKey",
                                   ulZKey ) == 0 )
   {
      if ( CheckExistenceOfEntity( vLOD, "POD_Entity" ) >= zCURSOR_SET )
         return( 1 );
      else
         return( 0 );
   }

   // Process any LOD_EntityChild entities recursively.
   nRC = SetCursorFirstEntity( vLOD, "LOD_EntityChild", "" );
   while ( nRC >= zCURSOR_SET )
   {
      SetViewToSubobject( vLOD, "LOD_EntityChild" );
      nRC = zwfnTZZOLODD_FoundPOD_Entity( vLOD, ulZKey );
      if ( nRC >= 0 )
         return( nRC );

      ResetViewFromSubobject( vLOD );
      nRC = SetCursorNextEntity( vLOD, "LOD_EntityChild", "" );
   }

   return( -1 );
}


static zSHORT
zwfnTZZOLODD_DeletePOD_Entity( zVIEW vSubtask, zVIEW vTZZOLODO )
{
   zULONG  ulZKey;

   if ( !ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META ) )
      return( -1 );

   // if Entity Type = Work or Derived, delete all POD_Entities (Join
   // Informations)
   if ( CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Work", "Y" ) == 0 ||
        CompareAttributeToString( vTZZOLODO, "LOD_Entity", "Derived", "Y" ) == 0 )
   {
      GetIntegerFromAttribute( (zLONG *) &ulZKey,
                               vTZZOLODO, "LOD_Entity", "ZKey" );
      zwfnTZZOLODD_IfPOD_Entity( vTZZOLODO, ulZKey, TRUE );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_DisableTE_List
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_DisableTE_List( zVIEW vSubtask )
{
   zVIEW  vTZZOLODO;
   zSHORT nIsCheckedOut;

   if ( GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZZOLODO, zSOURCE_LOD_META );

   EnableAction( vSubtask, "Alt-F4", nIsCheckedOut );
   EnableAction( vSubtask, "SaveWithNetwork", nIsCheckedOut );

   zwTZZOLODD_EnableDBH_Info( vSubtask );

   return( 0 );
} // zwTZZOLODD_DisableTE_List

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_QueryFrameSetUpRecur
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZZOLODD_QueryFrameSetUpRecur( zVIEW vLOD_ViewRecurs,
                                 zVIEW vLOD_ViewRoot )
{
   // Create QueryFrameAttribute entries from the LOD_EntityParent subobject structure.
   // Note that the vLOD_ViewRoot view is pointing to the last QueryFrameAttribute for the Entity
   // just processed. If the next Entity does not already have a QueryFrameAttribute entry, the
   // next entry will be at this position.
   zVIEW  vLOD_ViewTemp;
   zCHAR  szCurrentEntityName[ 33 ];
   zCHAR  szCurrentAttributeName[ 33 ];
   zCHAR  szPrompt[ 51 ];
   zCHAR  szTitle[ 51 ];
   zSHORT nRC;
   zSHORT nEntityRC;

   CreateViewFromViewForTask( &vLOD_ViewTemp, vLOD_ViewRoot, 0 );
   SetNameForView( vLOD_ViewTemp, "vLOD_ViewTemp", vLOD_ViewRoot, zLEVEL_TASK );
   nRC = SetCursorFirstEntity( vLOD_ViewRecurs, "LOD_EntityParent", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szCurrentEntityName, sizeof( szCurrentEntityName ),
                              vLOD_ViewRecurs, "LOD_EntityParent", "Name" );
      nEntityRC = SetCursorFirstEntityByString( vLOD_ViewRoot, "QueryFrameAttribute", "EntityName",
                                                szCurrentEntityName, 0 );
      // If a QueryFrameAttribute entry exists for the current Entity, we are positioned on the first one.
      // If an entry doesn't exist, we are still positioned on the last entry added.
      // Loop through all the attributes and add any that don't already exist.
      nRC = SetCursorFirstEntity( vLOD_ViewRecurs, "LOD_AttributeRec", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttribute( szCurrentAttributeName, sizeof( szCurrentAttributeName ),
                                 vLOD_ViewRecurs, "ER_AttributeRec", "Name" );
         if ( nEntityRC < zCURSOR_SET )
            // In this case there were no entries for the entity so we will always take the else
            // condition below to add all attributes.
            nRC = -1;
         else
         {
            // There is at least one entry for the current Entity, so check if the current
            // attribute exists.
            SetCursorFirstEntityByString( vLOD_ViewTemp, "QueryFrameAttribute", "EntityName",
                                          szCurrentEntityName, 0 );
            if ( CompareAttributeToString( vLOD_ViewTemp, "QueryFrameAttribute", "AttributeName",
                                           szCurrentAttributeName ) == 0 )
            {
               // We're already positioned on the correct entry.
               nRC = 0;
               SetViewFromView( vLOD_ViewTemp, vLOD_ViewRoot );
            }
            else
               nRC = SetCursorNextEntityByString( vLOD_ViewTemp, "QueryFrameAttribute", "AttributeName",
                                                  szCurrentAttributeName, 0 );
         }
         if ( nRC >= zCURSOR_SET &&
              CompareAttributeToString( vLOD_ViewTemp, "QueryFrameAttribute", "EntityName",
                                        szCurrentEntityName ) == 0 )
         {
            // We found an entry with a match on EntityName and AttributeName, so set the
            // vLOD_ViewRoot view to this position.
            // Note that we had to use a temporary view because the last set attribute could have
            // set position on an entry with the same AttributeName but different EntityName.
            SetViewFromView( vLOD_ViewRoot, vLOD_ViewTemp );
         }
         else
         {
            // We didn't find an entry with a match on EntityName and AttributeName, so add it
            // to the current position.
            CreateEntity( vLOD_ViewRoot, "QueryFrameAttribute", zPOS_AFTER );
            SetAttributeFromAttribute( vLOD_ViewRoot,   "QueryFrameAttribute", "EntityName",
                                       vLOD_ViewRecurs, "LOD_EntityParent", "Name" );
            SetAttributeFromAttribute( vLOD_ViewRoot,   "QueryFrameAttribute", "AttributeName",
                                       vLOD_ViewRecurs, "ER_AttributeRec", "Name" );
            GetStringFromAttribute( szPrompt, sizeof( szPrompt ), vLOD_ViewRecurs, "LOD_EntityParent", "Name" );
            GetStringFromAttribute( szTitle, sizeof( szTitle ), vLOD_ViewRecurs, "ER_AttributeRec", "Name" );
            strcat_s( szPrompt, sizeof( szPrompt ), szTitle );
            AddSpacesToString( szPrompt );
            AddSpacesToString( szTitle );
            SetAttributeFromString( vLOD_ViewRoot, "QueryFrameAttribute", "Prompt", szPrompt );
            SetAttributeFromString( vLOD_ViewRoot, "QueryFrameAttribute", "Title", szTitle );
         }

         nRC = SetCursorNextEntity( vLOD_ViewRecurs, "LOD_AttributeRec", 0 );
      }
      // At the end, set the position to the last QueryFrameAttribute entry for the current Entity.
      nRC = SetCursorFirstEntityByString( vLOD_ViewRoot, "QueryFrameAttribute", "EntityName",
                                          szCurrentEntityName, 0 );
      nRC = 0;
      while ( nRC >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vLOD_ViewRoot, "QueryFrameAttribute", "EntityName",
                                        szCurrentEntityName ) != 0 )
         {
            SetCursorPrevEntity( vLOD_ViewRoot, "QueryFrameAttribute", 0 );
            nRC = -1;
         }
         else
            nRC = SetCursorNextEntity( vLOD_ViewRoot, "QueryFrameAttribute", 0 );
      }

      // We have finished adding QueryFrameAttribute entries for the current Entity. Now process any
      // child entities.
      nRC = SetCursorFirstEntity( vLOD_ViewRecurs, "LOD_EntityChild", 0 );
      if ( nRC >= zCURSOR_SET )
      {
         SetViewToSubobject( vLOD_ViewRecurs, "LOD_EntityChild" );
         zwTZZOLODD_QueryFrameSetUpRecur( vLOD_ViewRecurs, vLOD_ViewRoot );
         ResetViewFromSubobject( vLOD_ViewRecurs );
      }

      nRC = SetCursorNextEntity( vLOD_ViewRecurs, "LOD_EntityParent", 0 );
   }

   DropView( vLOD_ViewTemp );
   return( 0 );
} // zwTZZOLODD_QueryFrameSetUpRecur

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_QueryFrameSetUp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_QueryFrameSetUp( zVIEW vSubtask )
{
   // Set up the QueryFrameAttribute subobject by making sure that every attribute in the LOD is
   // has a QueryFrameAttribute entry and that no QueryFrameAttribute exists that isn't in the LOD.
   // The QueryFrameAttribute entries must be created in hierarchical order.

   zVIEW  vTZZOLODO;
   zVIEW  vLOD_ViewRecurs;
   zVIEW  vLOD_ViewRoot;
   zSHORT nRC;
   zCHAR  szEntityName[ 33 ];
   zCHAR  szAttributeName[ 33 ];

   GetViewByName( &vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vLOD_ViewRoot, vTZZOLODO, 0 );

   // First make sure all Entity/Attributes from the LOD are added to the Frame work entries.
   CreateViewFromViewForTask( &vLOD_ViewRecurs, vLOD_ViewRoot, 0 );
   SetNameForView( vLOD_ViewRecurs, "vLOD_ViewRecurs", vSubtask, zLEVEL_TASK );
   zwTZZOLODD_QueryFrameSetUpRecur( vLOD_ViewRecurs, vLOD_ViewRoot );
   DropView( vLOD_ViewRecurs );

   // Next, make sure that no Frame work entries exist that don't have a corresponding
   // Entity/Attribute in the LOD.
   nRC = SetCursorFirstEntity( vLOD_ViewRoot, "QueryFrameAttribute", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vLOD_ViewRoot, "QueryFrameAttribute", "EntityName" );
      GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ), vLOD_ViewRoot, "QueryFrameAttribute", "AttributeName" );
      nRC = SetCursorFirstEntityByString( vLOD_ViewRoot, "LOD_Entity", "Name", szEntityName, "" );
      if ( nRC >= zCURSOR_SET )
         nRC = SetCursorFirstEntityByString( vLOD_ViewRoot, "ER_Attribute", "Name", szAttributeName, "LOD_Entity" );
      if ( nRC < zCURSOR_SET )
         DeleteEntity( vLOD_ViewRoot, "QueryFrameAttribute", zREPOS_NONE );

      nRC = SetCursorNextEntity( vLOD_ViewRoot, "QueryFrameAttribute", "" );
   }

   DropView( vLOD_ViewRoot );
   return( 0 );
} // zwTZZOLODD_QueryFrameSetUp

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZZOLODD_RefreshAttributeDesc
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_RefreshAttributeDesc( zVIEW vSubtask )
{
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "AttributeDescription" );
   if ( vW )
      RefreshWindow( vW );

   return( 0 );
} // zwTZZOLODD_RefreshAttributeDesc

/*************************************************************************************************
**
**    OPERATION: zwTZZOLODD_RebuildSelectedLODs
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZZOLODD_RebuildSelectedLODs( zVIEW vSubtask )
{
// zCHAR       szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zVIEW       vTZZOLODO;
   zVIEW       vTZZOLFLO;
// zVIEW       vTZZOMFLO;
// zVIEW       vTZEREMDO_REF;
// zVIEW       vHierDiag;
   zLONG       lZKey;
   zSHORT      nRC;

   // Get Access to LOD File List Object.
   nRC = GetViewByName( &vTZZOLFLO, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Loop through each selected LOD and Activate and Save each.
   for ( nRC = SetCursorFirstEntity( vTZZOLFLO, "QueryW_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZZOLFLO, "QueryW_MetaDef", "" ) )
   {
      nRC = GetSelectStateOfEntity( vTZZOLFLO, "QueryW_MetaDef" );
      if ( nRC == 1 )
      {
         GetIntegerFromAttribute( &lZKey, vTZZOLFLO, "QueryW_MetaDef", "CPLR_ZKey" );
         SetCursorFirstEntityByInteger( vTZZOLFLO, "W_MetaDef", "CPLR_ZKey", lZKey, 0 );
         nRC = ActivateMetaOI( vSubtask, &vTZZOLODO, vTZZOLFLO, zSOURCE_LOD_META,
                               zSINGLE | zLEVEL_APPLICATION );
         SetNameForView( vTZZOLODO, "TZZOLODO", vSubtask, zLEVEL_TASK );
         nRC = zwTZZOLODD_SaveLOD( vSubtask );
         DropObjectInstance( vTZZOLODO );
         SetSelectStateOfEntity( vTZZOLFLO, "QueryW_MetaDef", 0 );
      }
   }

   return( 0 );
} // zwTZZOLODD_RebuildSelectedLODs

#ifdef __cplusplus
}
#endif
