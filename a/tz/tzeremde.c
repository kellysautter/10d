/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzeremde.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
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
// LAST MESSAGE ID: ER00209
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2002.04.29  FH
   unset the readonly state, to get rid of errors on a noncheckedout meta
2001.03.07 BL
   revise ERD Cross Reference

2001.02.26 BL
   revise ERD Cross Reference

2000.11.01  BL
   Modified function for do not create or delete a Relationship
   from or to a SAP Entity, Bugfix load Relationships

2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.10.22  BL  Z10
   Modified zwTZEREMDD_ERD_RelActions, zwTZEREMDD_RLL_Popup,
   zwTZEREMDD_ERD_EntityActions zwTZEREMDD_ID_Popup for SAP Import


2000.10.24  SR  Z2000  Size of path variables
   Modified size of szFullName in function zwTZEREMDD_EntitySA_List
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

2000.08.04  BL  Z10
   Save zoom status for Bug Bug TB 50544
   Modified all operations for completely maintained ERD Tool
   (Cursor problems)

2000.06.20  BL  Z10
   Enable Actions again

2000.06.19  BL  Z10
   Modified zwTZEREMDD_ERD_RelDelete: if delete Relationship and this
   Relationship is a part of the Identifier, then delete the Relationship
   from the Identifier

2000.06.05  BL  Z10  Repository and Tools
   Modified zwTZEREMDD_RLL_Popup, zwTZEREMDD_ID_Popup, zwTZEREMDD_ATT_Popup,
   zwTZEREMDD_ENT_EntityActions, zwTZEREMDD_ERD_EntityActions,
   zwTZEREMDD_ERD_RelActions, zwTZEREMDD_SA_EntityActions:
   if ERD/Subject Area not checked out, disable menu items
   for update
   Modified zwTZEREMDD_PostBuildSA_Diagram for set check out state in
   title

2000.05.24  BL  Z10  Repository and Tools
   Modified zwTZEREMDD_PostBuildSA_Diagram for set check out state in
   title
   Modified zwTZEREMDD_ERD_EntityActions, zwTZEREMDD_ERD_RelSelect,
   zwTZEREMDD_ERD_Actions: check extists of View

2000.05.23  BL  Z10
   Modified zwTZEREMDD_SA_EntityActions and zwTZEREMDD_SA_RelActions for
   check exists Entity (Subject Areas)

1999.11.05  BL  Z10
   Added Operation zwTZEREMDD_AttributeListPreBuild: if Reset in Attribute Name
   List without search, then was this List empty

1999.10.17  BL  Z10 new menu strcture
   Modified Operation zwTZEREMDD_ERD_Focus for unselect Relationship
   Modified Operation zwTZEREMDD_ERD_RelSelect: Select one Releationship
   only

1998.07.19  DC  Z10
   Modified CreateXrefInfo to set relationship information for Relationship
   cross reference.

19.07.1997  DonC
   Changed MB_SetMessage to use a single DIL control, instead of 2 subs
   within the DIL.

11.03.1998  DonC
   Added the following operations to generate cross reference information.
      AttributeEntityList
      EntityIdentifierList
      CreateXrefInfo
      EntitySA_List

1998/09/08  DC
   Modified zwTZEREMDD_AttributeListFindName and AttributeListFindDesc to
   show search text value in DIL message. (TB 139)

1998.09.23  DKS
   Stopped crash when expanding ports on more than one entity (TB 245).

1998.09.24    DKS
   Fixed PrintDiagram to provide a return code to the application when
   the print is canceled (QS 78).
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
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

//#define  tzERDIAGRAMX           1200  // 600    now a 1-meg array!!!
//#define  tzERDIAGRAMY            900  // 300
#define MAX_GRID_X 1200 // 600   // GRID is the unit used by the ER control
#define MAX_GRID_Y 900  // 300   //

//#define  tzERENTITYX           19  // 9  # of cells in width
//#define  tzERENTITYY           11  // 5  # of cells in height
#define CELLS_PER_ENTITY_X   9  // 6
#define CELLS_PER_ENTITY_Y   5  // 3  // cells per entity???

#define MAX_CELL_X 200       // 1 thru ??? 59 are referenced
#define MAX_CELL_Y 200       // 1 thru ??? 39 are referenced

zCHAR  cCellUsage[ MAX_CELL_X ][ MAX_CELL_Y ];
zSHORT nCellsUsed;
zSHORT nD, nXincr, nYincr;
zSHORT nXmin, nYmin;
zCHAR  cCellUsageSA[ MAX_CELL_X ][ MAX_CELL_Y ];
zSHORT nCellsUsedSA;
zSHORT nD_SA, nXincrSA, nYincrSA;

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshRllWnd( zVIEW vSubtask );

/*vml #include <KZAPPLAA.H> */
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_RelSelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_RelSelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PositionER( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AddEntity( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_AddEntity( zVIEW vSubtask,
                        zVIEW vTZEREMDO );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_DiagToRLLMaint( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshAttRelSAWnd( zVIEW vSubtask );
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
zwTZEREMDD_AddRelLink( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetModeUpdate( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_RefreshEntitySelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomStandardSize( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomStdSizeSA( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_Print( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_FreeCell( zPSHORT pnXout,
                       zPSHORT pnYout,
                       zSHORT  nXcenter,
                       zSHORT  nYcenter,
                       zSHORT  nDlimit );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_PositionNonPosEnts( zVIEW vSubtask,
                                 zVIEW vTZEREMDO );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_PosNonPosEntsSA( zVIEW vSubtask,
                              zVIEW vTZERSASO );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_InitCellsEDM( zVIEW vTZERSASO );

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_MarkCellEDM( zLONG   lXgrid,
                         zLONG   lYgrid );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_UnmarkCellEDM( zLONG   lXgrid,
                           zLONG   lYgrid );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_FreePosEDM( zPLONG  plXout,
                        zPLONG  plYout,
                        zLONG   lXcenter,
                        zLONG   lYcenter,
                        zSHORT  nDlimit,
                        zVIEW   vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_GetWeightedPosEDM( zPLONG  plXout,
                               zPLONG  plYout,
                               zVIEW   vTZERSASO );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_InitCellsSA( zVIEW vTZERSASO );

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_MarkCellSA( zLONG   lXgrid,
                         zLONG   lYgrid );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_UnmarkCellSA( zLONG   lXgrid,
                           zLONG   lYgrid );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_FreePosSA( zPLONG  plXout,
                        zPLONG  plYout,
                        zLONG   lXcenter,
                        zLONG   lYcenter,
                        zSHORT  nDlimit,
                        zVIEW   vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_GetWeightedPosSA( zPLONG  plXout,
                               zPLONG  plYout,
                               zVIEW   vTZERSASO );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_SA_ViewList( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_SA_ViewDiag( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_EntitySelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW   vSubtask,
                           zPCHAR  szToolname,
                           zPCHAR  szSAname,
                           zVIEW   vToolView,
                           zPCHAR  szEntityname,
                           zLONG   lType );
zOPER_EXPORT zSHORT OPERATION
RecalcAllRels( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_DeselectRelationship( zVIEW  vView,
                                 zPCHAR szEntityName,
                                 zSHORT nSelectInstance );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwfnTZEREMDD_RefreshEntitySelect( zVIEW vSubtask,
                                  zSHORT nDeselectRelationship );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_SetSelectionInSAList( zVIEW vTZEREMDO,
                                 zVIEW vTZERSASO );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SA_RefreshEntityList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SetRelationshipSelect( zVIEW vSubtask );

static void
zwfnTZEREMDD_CreateEntityIdentifier( zVIEW  vXfer,
                                     zVIEW  vERD );
static void
zwfnTZEREMDD_SetIdentifierEntity( zVIEW vXfer,
                                  zVIEW vERD );
static void
zwfnTZEREMDD_EntitySA_AddSA( zVIEW vSubtask,
                             zVIEW vXfer,
                             zVIEW vTemp );
static void
zwfnTZEREMDD_CreateEntity_RS( zVIEW vXfer,
                              zVIEW vERD );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PositionER
//
// PURPOSE:    Set the ER Diagram positions for all Entities in the ER.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PositionER( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vERD;
   zVIEW  vTZERPOSO;
   zVIEW  vTZERPOS1;
   zSHORT nRC, nCells, nX, nY, nXmax, nYmax, nPrevYmax, nXchk, nYchk;
   zSHORT nXcenter, nYcenter;
   zSHORT nStaggerX, nStaggerY;
   zLONG  lEnts, lCells, lRelEnts;
   zLONG  lXcell, lYcell;
   zLONG  lZKey;
   zCHAR  sz[ 256 ];
   ldiv_t ll;          // used by ldiv std c function

   nCellsUsed = 0;
   zmemset( cCellUsage, 0, (unsigned) MAX_CELL_X * (unsigned) MAX_CELL_Y );

   // Get Access to ER Object
   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZEREMDO, vERD, 0 );

   // Drop any existing position evaluation object
   GetViewByName( &vTZERPOSO, "dbgPOS", vSubtask, zLEVEL_TASK );
   if ( vTZERPOSO != 0 )
   {
      DropObjectInstance( vTZERPOSO );
   }

   // Create a new work position evaluation object
   ActivateEmptyObjectInstance( &vTZERPOSO, "TZERPOSO", vSubtask, zSINGLE );
   if ( vTZERPOSO == 0 )
   {
      strcpy_s( sz, sizeof( sz ), "Unable to create TZERPOSO work OI." );
      MessageSend( vSubtask, "ER00202", "E/R Model Maintainance",
                   sz,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow,
                               "TZEREMDD", "TZEREMDD" );
      return( 0 );
   }

   SetNameForView( vTZERPOSO, "dbgPOS", vSubtask, zLEVEL_TASK );
   CreateEntity( vTZERPOSO, "RootStats", zPOS_FIRST );

   // Copy all the entities from the ER Model to the work object
   lEnts = 0;
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", "" );
   while ( nRC >= zCURSOR_SET )
   {
      // first, position at "cluster" for the entity -- currently based
      // on a possible Owning Area.
      GetStringFromAttribute( sz, sizeof( sz ), vTZEREMDO, "ER_Entity", "OwningArea" );
      if ( sz[ 0 ] == 0 )
      {
         sz[ 0 ] = 1;  // Nice happy face...
         sz[ 1 ] = 0;
      }
      nRC = SetCursorFirstEntityByString( vTZERPOSO, "Cluster", "Name",
                                          sz, "" );
      if ( nRC <  zCURSOR_SET )
      {
         CreateEntity( vTZERPOSO, "Cluster", zPOS_LAST );
         SetAttributeFromString( vTZERPOSO, "Cluster", "Name", sz );
         SetAttributeFromInteger( vTZERPOSO, "Cluster", "NbrEnts", 0 );
         AddToAttributeFromInteger( vTZERPOSO, "RootStats",
                                    "NbrClusters", 1 );
      }

      // now, add the entity under the appropriate cluster
      CreateEntity( vTZERPOSO, "W_ERD_Entity", zPOS_AFTER );
      SetMatchingAttributesByName( vTZERPOSO, "W_ERD_Entity",
                                   vTZEREMDO, "ER_Entity",
                                   zSET_ALL );
      if ( sz[ 1 ] == 1 )
      {  // reset OwningArea to default Cluster Name
         SetAttributeFromString( vTZERPOSO, "W_ERD_Entity",
                                 "OwningArea", sz );
      }

      lEnts++;
      AddToAttributeFromInteger( vTZERPOSO, "Cluster", "NbrEnts", 1 );
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_Entity", "" );
   }

   SetAttributeFromInteger( vTZERPOSO, "RootStats", "NbrEnts", lEnts );

   // Set Cell size
   lCells = (zLONG)sqrt( (double)lEnts );
   if ( lCells * lCells !=  lEnts )
      lCells++;

   SetAttributeFromInteger( vTZERPOSO, "RootStats", "NbrCells",
                            (zLONG)lCells );

   // Include all related entities from the ER Model to the work object
   CreateViewFromViewForTask( &vTZERPOS1, vTZERPOSO, 0 );
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_Entity", "" );
   if ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vTZEREMDO, "ER_Entity", "ZKey" );
      SetCursorFirstEntityByInteger( vTZERPOSO, "W_ERD_Entity",
                                    "ZKey", lZKey, "RootStats" );
   }

   while ( nRC >= zCURSOR_SET )
   {
      lRelEnts = 0;
      nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
      while ( nRC >= zCURSOR_SET )
      {
         // clear any relationship positioning
         SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1",
                                  "ER_DiagramBendPosX", 0 );
         SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1",
                                  "ER_DiagramBendPosY", 0 );
         SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1",
                                  "ER_DiagramE1PosX", 0 );
         SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1",
                                  "ER_DiagramE1PosY", 0 );
         SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1",
                                  "ER_DiagramE2PosX", 0 );
         SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1",
                                  "ER_DiagramE2PosY", 0 );
         // get the ZKey of relationship for comparison and positioning
         GetIntegerFromAttribute( &lZKey, vTZEREMDO,
                                  "ER_Entity_Other", "ZKey" );
         if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity",
                                         "ZKey", lZKey ) != 0 )
         {  // only consider non-recursive relationships and
            // only include a related entity once (i.e. no multiple
            // relationships between the same 2 entities).
            if ( SetCursorFirstEntityByInteger( vTZERPOSO,
                                                "W_ERD_RelatedEntity",
                                                "ZKey", lZKey, "" )
                 < zCURSOR_SET )
            {
               SetCursorFirstEntityByInteger( vTZERPOS1, "W_ERD_Entity",
                                             "ZKey", lZKey, "RootStats" );
               IncludeSubobjectFromSubobject( vTZERPOSO, "W_ERD_RelatedEntity",
                                              vTZERPOS1, "W_ERD_Entity",
                                              zPOS_AFTER );
               lRelEnts++;
               if ( CompareAttributeToAttribute(
                     vTZERPOSO, "W_ERD_Entity", "OwningArea",
                     vTZERPOSO, "W_ERD_RelatedEntity", "OwningArea" ) == 0 )
               {
                  AddToAttributeFromInteger( vTZERPOSO, "Cluster", "NbrRelEntsInMyCluster", 1 );
                  AddToAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "NbrRelEntsInSameOA", 1 );
               }
               else
               {
                  AddToAttributeFromInteger( vTZERPOSO, "Cluster", "NbrRelEntsInOtherClusters", 1 );
               }
            }
         }
         nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" );
      }

      SetAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "NbrRelatedEnts", lRelEnts );
      nRC = SetCursorNextEntity( vTZEREMDO, "ER_Entity", "" );
      GetIntegerFromAttribute( &lZKey, vTZEREMDO, "ER_Entity", "ZKey" );
      SetCursorFirstEntityByInteger( vTZERPOSO, "W_ERD_Entity", "ZKey", lZKey, "RootStats" );
   }

   //  Sort Clusters, Entities, and Related Entities in prep for
   //  the placement algorithm
   nXmax = nYmax = nPrevYmax = 0;
   nRC = zgSortEntityWithinParent( zDESCENDING, vTZERPOSO, "Cluster", "NbrEnts", "NbrRelEntsInMyCluster" );
   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "Cluster", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "Cluster", "" ) )
   {
      GetStringFromAttribute( sz, sizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only
      zgSortEntityWithinParent( zDESCENDING, vTZERPOSO, "W_ERD_Entity", "NbrRelEntsInSameOA", "" );
      // Set Cell size
      GetIntegerFromAttribute( &lEnts, vTZERPOSO, "Cluster", "NbrEnts" );
      lCells = (zLONG)sqrt( (double)lEnts );
      if ( lCells * lCells != lEnts )
         lCells++;

      SetAttributeFromInteger( vTZERPOSO, "Cluster", "NbrCells", lCells );
      nCells = (zSHORT)lCells;

      // Set Cluster X,Y upper left corner by finding a "good"
      // available cell "square" that will hold Cluster area
      if ( nXmax * 4 > nYmax * 5 && nCells < MAX_CELL_Y - nYmax )
         nY = nYmax + 1;
      else
         nY = 1;

      if ( nPrevYmax != 0 )
         if ( zmemcmp( &cCellUsage[ nXmax + 1 - nCells ][ nPrevYmax + 1 ], cCellUsage[ 0 ], nCells ) == 0 )
            nY = nPrevYmax + 1;

      for ( nX = 1; nX <= MAX_CELL_X; nX++ )
      {
         if ( nX >= MAX_CELL_X )
         {
            nY++;
            if ( nY >= MAX_CELL_Y )
            {
               strcpy_s( sz, sizeof( sz ), "No space available for Area:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ), vTZERPOSO, "Cluster", "Name" );
               strcat_s( sz, sizeof( sz ), "'." );
               MessageSend( vSubtask, "ER00203", "E/R Model Maintainance",
                            sz,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               return( 0 );
            }
            else
               nX = 1;
         }

         for ( nXchk = nX; nXchk <= nX + nCells - 1; nXchk++ )
         {
            if ( zmemcmp( &cCellUsage[ nXchk ][ nY ], cCellUsage[ 0 ], nCells ) == 0 )
            {
               if ( nXchk != nX + nCells - 1 )
                  continue;
               else
               {
                  nXchk = -1;
               }
            }

            break;
         }

         if ( nXchk == -1 )
            break;
      }

      // at this point nX, nY is the chosen position for the Cluster,
      // so mark the Cells as used and set the Attrs in the POS object.
      for ( nXchk = nX; nXchk < nX + nCells; nXchk++ )
      {
         nCellsUsed++;
         zmemset( &cCellUsage[ nXchk ][ nY ], 1, nCells );
      }

      SetAttributeFromInteger( vTZERPOSO, "Cluster", "CellX", nX );
      nXchk = nX + nCells - 1;
      if ( nXchk > nXmax )
         nXmax = nXchk;

      SetAttributeFromInteger( vTZERPOSO, "Cluster", "CellY", nY );
      nYchk = nY + nCells - 1;
      if ( nYchk > nYmax )
      {
         nPrevYmax = nYmax;
         nYmax = nYchk;
      }
   }

   // Process all entites in Cluster setting stats for sorting
   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "Cluster", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "Cluster", "" ) )
   {
      GetStringFromAttribute( sz, sizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only
      for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_Entity", "" ) )
      {
         for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" ) )
         {
            if ( CompareAttributeToAttribute( vTZERPOSO, "W_ERD_Entity", "OwningArea", vTZERPOSO, "W_ERD_RelatedEntity", "OwningArea" ) == 0 )
            {
               SetAttributeFromString( vTZERPOSO, "W_ERD_RelatedEntity", "OA_MatchesParentsOA", "Y" );
            }
            // for each related entity, find the nbr of related entities
            // it has that are in the same Owning Area as its parent
            SetViewFromView( vTZERPOS1, vTZERPOSO );
            SetCursorFirstEntityByEntityCsr( vTZERPOS1, "W_ERD_Entity", vTZERPOSO, "W_ERD_RelatedEntity", "" );
            for ( nRC = SetCursorFirstEntity( vTZERPOS1, "W_ERD_RelatedEntity", "" );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vTZERPOS1, "W_ERD_RelatedEntity", "" ) )
            {
               if ( CompareAttributeToAttribute( vTZERPOSO, "W_ERD_Entity", "OwningArea", vTZERPOS1, "W_ERD_RelatedEntity", "OwningArea" ) == 0 )
               {
                  AddToAttributeFromInteger( vTZERPOSO, "W_ERD_RelatedEntity", "NbrRelEntsInMyParentsOA", 1 );
               }
            } // End for each W_ERD_RelatedEntity of the related entity

         } // End for each W_ERD_RelatedEntity
         zgSortEntityWithinParent( zDESCENDING, vTZERPOSO, "W_ERD_RelatedEntity", "OA_MatchesParentsOA", "NbrRelEntsInMyParentsOA" );
      } // End for each W_ERD_Entity
   } // End for each Cluster

   //
   // Do the Placement Algorithm
   //
   nXmin = nYmin = 999;
   nCellsUsed = 0;
   zmemset( cCellUsage, 0, (unsigned) MAX_CELL_X * (unsigned) MAX_CELL_Y );

   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "Cluster", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "Cluster", "" ) )
   {
      GetStringFromAttribute( sz, sizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only

      // Get Cell size
      GetIntegerFromAttribute( &lCells, vTZERPOSO, "Cluster", "NbrCells" );
      nCells = (zSHORT) lCells;
      GetIntegerFromAttribute( &lXcell, vTZERPOSO, "Cluster", "CellX" );
      nX = (zSHORT) lXcell;
      GetIntegerFromAttribute( &lYcell, vTZERPOSO, "Cluster", "CellY" );
      nY = (zSHORT) lYcell;
      nD = 0;
      nXincr = 0;
      nYincr = 0;
      nXcenter = (nCells / 2) + nX;
      nYcenter = (nCells / 2) + nY;
      if ( (nCells & 0x0001) == 0 )
      {
         nXcenter--;
         nYcenter--;
      }

      // Process all entites in Cluster
      for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_Entity", "" ) )
      {
         GetStringFromAttribute( sz + 50, sizeof( sz ) - 50, //debug only
                                 vTZERPOSO, "W_ERD_Entity", "Name" );
         if ( CompareAttributeToInteger( vTZERPOSO, "W_ERD_Entity", "ER_DgmPosX", 0 ) != 0 )
         {
            // get position
            GetIntegerFromAttribute( &lXcell, vTZERPOSO, "W_ERD_Entity", "ER_DgmPosX" );
            GetIntegerFromAttribute( &lYcell, vTZERPOSO, "W_ERD_Entity", "ER_DgmPosY" );
         }
         else
         {
            // find free cell
            nD = nXincr = nYincr = 0;
            if ( zwfnTZEREMDD_FreeCell( &nXchk, &nYchk, nXcenter, nYcenter, nCells ) < 0 )
            {
               strcpy_s( sz, sizeof( sz ), "No cell available for Area:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ), vTZERPOSO, "Cluster", "Name" );
               strcat_s( sz, sizeof( sz ), "'\nEntity:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ), vTZERPOSO, "W_ERD_Entity", "Name" );
               strcat_s( sz, sizeof( sz ), "'." );
               MessageSend( vSubtask, "ER00205", "E/R Model Maintainance",
                            sz,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               return( 0 );
            }

            lXcell = nXchk;
            lYcell = nYchk;

            // set position
            SetAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "ER_DgmPosX", lXcell );
            SetAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "ER_DgmPosY", lYcell );
         }

         nX = (zSHORT)lXcell;
         nY = (zSHORT)lYcell;
         nD = nXincr = nYincr = 0;

         // Process all related entites within the same cluster
         for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" ) )
         {
            GetStringFromAttribute( sz + 100, sizeof( sz ) - 100, //debug only
                                    vTZERPOSO, "W_ERD_RelatedEntity", "Name" );
            if ( CompareAttributeToString( vTZERPOSO, "W_ERD_RelatedEntity",
                                           "OA_MatchesParentsOA", "Y" ) != 0 )
               continue;
            if ( CompareAttributeToInteger( vTZERPOSO, "W_ERD_RelatedEntity", "ER_DgmPosX", 0 ) != 0 )
               continue;

            // find next free cell
            if ( zwfnTZEREMDD_FreeCell( &nXchk, &nYchk, nX, nY, nCells ) < 0 )
            {
               strcpy_s( sz, sizeof( sz ), "No cell available for Area:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ), vTZERPOSO, "Cluster", "Name" );
               strcat_s( sz, sizeof( sz ), "'\nEntity:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ), vTZERPOSO, "W_ERD_Entity", "Name" );
               strcat_s( sz, sizeof( sz ), "'\nRelated Entity:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ), vTZERPOSO, "W_ERD_RelatedEntity", "Name" );
               strcat_s( sz, sizeof( sz ), "'." );
               MessageSend( vSubtask, "ER00206", "E/R Model Maintainance",
                            sz,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               return( 0 );
            }

            lXcell = nXchk;
            lYcell = nYchk;

            // set position
            SetAttributeFromInteger( vTZERPOSO, "W_ERD_RelatedEntity", "ER_DgmPosX", lXcell );
            SetAttributeFromInteger( vTZERPOSO, "W_ERD_RelatedEntity", "ER_DgmPosY", lYcell );

         } // End for each W_ERD_RelatedEntity

      } // End for each W_ERD_Entity

   } // End for each Cluster

// DisplayObjectInstance( vTZERPOSO, "RootStats", 0 );  // debug only

   //
   // Set the Placement values into the ER
   //
   nXmin--;
   nYmin--;
   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_Entity", "RootStats" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_Entity", "RootStats" ) )
   {
      GetStringFromAttribute( sz, sizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only
      GetStringFromAttribute( sz + 50, sizeof( sz ) - 50, //debug only
                              vTZERPOSO, "W_ERD_Entity", "Name" );
      // First, retrieve the cell position, adjust for white space at the top and left, then calc a stagger offset.
      GetIntegerFromAttribute( &lXcell, vTZERPOSO, "W_ERD_Entity", "ER_DgmPosX" );
      if ( nXmin > 0 )
      {
         // reset position
         lXcell -= nXmin;
         SetAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "ER_DgmPosX", lXcell );
      }

      ll = ldiv( lXcell, 4 );
      nStaggerY = (zSHORT) ll.rem * 3;

      GetIntegerFromAttribute( &lYcell, vTZERPOSO, "W_ERD_Entity", "ER_DgmPosY" );
      if ( nYmin > 0 )
      {
         // reset position
         lYcell -= nYmin;
         SetAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "ER_DgmPosY", lYcell );
      }

      ll = ldiv( lYcell, 5 );
      nStaggerX = (zSHORT) ll.rem * 5;

      // now, position on ER Entity and set position
      GetIntegerFromAttribute( &lZKey, vTZERPOSO, "W_ERD_Entity", "ZKey" );
      SetCursorFirstEntityByInteger( vTZEREMDO, "ER_Entity", "ZKey", lZKey, "" );
      SetAttributeFromInteger( vTZEREMDO, "ER_Entity", "ER_DiagramPosX", (lXcell * 30) + nStaggerX - 15 );
      SetAttributeFromInteger( vTZEREMDO, "ER_Entity", "ER_DiagramPosY", (lYcell * 20) + nStaggerY - 10 );
   }

   DropView ( vTZEREMDO );
   RefreshDiagram( vSubtask, "ERD", 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_FreeCell( zPSHORT pnXout,
                       zPSHORT pnYout,
                       zSHORT  nXcenter,
                       zSHORT  nYcenter,
                       zSHORT  nDlimit )
{
   for ( ; ; )
   {
      if ( (zSHORT)sqrt( nXincr * nXincr + nYincr * nYincr ) <= nD )
      {
         *pnXout = nXcenter + nXincr;
         *pnYout = nYcenter + nYincr;
         if ( *pnXout > 0 && *pnXout < MAX_CELL_X && *pnYout > 0 && *pnYout < MAX_CELL_Y )
         {
            if ( cCellUsage[ *pnXout ][ *pnYout] == 0 )
               break;
         }
      }

      // increment nYincr, nXincr, or nD as appropriate
      if ( nXincr > -nD )
         nXincr--;
      else
      {
         if ( nYincr > -nD )
         {
            nYincr--;
            nXincr = nD;
         }
         else
         {
            if ( nD <= nDlimit )
            {
               nD++;
               nYincr = nD;
               nXincr = nD;
            }
            else
            {
               *pnXout = *pnYout = 0;
               return( -1 );
            }
         }
      }
   }

   nCellsUsed++;
   cCellUsage[ *pnXout ][ *pnYout ] = (zCHAR)nCellsUsed;
   if ( nXmin > *pnXout )
      nXmin = *pnXout;

   if ( nYmin > *pnYout )
      nYmin = *pnYout;

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_Actions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ERD_Actions( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   // Get Access to ER Object
   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      CreateTrackingPopupMenu( vSubtask, "ERD_Popup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_Focus
//
// PURPOSE:   Give DIL for ERD Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ERD_Focus( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vWindow;
   zPCHAR szName;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
      MB_SetMessage( vSubtask, 1, szName );

      // unselect Relationship
      if (( CheckExistenceOfEntity( vTZEREMDO, "ER_RelType" ) >= zCURSOR_SET ) &&
          ( GetSelectStateOfEntity( vTZEREMDO, "ER_RelType" ) == 1 ))
      {
         SetSelectStateOfEntity( vTZEREMDO, "ER_RelType", 0 );
         GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
         RefreshDiagram( vWindow, "ERD", 0 );
      }

      // Refresh Subject Area
      if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 )
      {
         // unselect Relationship
         if ( (CheckExistenceOfEntity( vTZERSASO, "SA_RelType" ) >= zCURSOR_SET) &&
              (GetSelectStateOfEntity( vTZERSASO, "SA_RelType" ) == 1) )
         {
            SetSelectStateOfEntity( vTZERSASO, "SA_RelType", 0 );
            GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" );
            RefreshDiagram( vWindow, "ERD", 0 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ENT_EntityActions
//
// PURPOSE:   Show Popup Menu for Entity ListBox
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ENT_EntityActions( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zPCHAR szName;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
   MB_SetMessage( vSubtask, 1, szName );

   if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
   {
      EnableAction( vSubtask, "AddEntity", FALSE );
      EnableAction( vSubtask, "DeleteEntityFromPopup", FALSE );
      EnableAction( vSubtask, "CopyEntityFromPopup", FALSE );
   }
   else
   {
      EnableAction( vSubtask, "AddEntity", TRUE );
      EnableAction( vSubtask, "DeleteEntityFromPopup", TRUE );
      EnableAction( vSubtask, "CopyEntityFromPopup", TRUE );
   }

   CreateTrackingPopupMenu( vSubtask, "EntityPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_RLL_Popup
//
// PURPOSE:   Show Popup Menu for Rel ListBox
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_RLL_Popup( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zPCHAR szName;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
      MB_SetMessage( vSubtask, 1, szName );

      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META );

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
         {
            bDelete = TRUE;

            // Exists a Relationship to a SAP Entities, disable delete and new Option
            if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity", 1 ) == 0 ||
                 CompareAttributeToInteger( vTZEREMDO, "ER_Entity_Other", "SAP_Entity", 1 ) == 0 )
            {
               bDelete = FALSE;
               bNew    = FALSE;
            }
         }
      }

      EnableAction( vSubtask, "RelNew", bNew );
      EnableAction( vSubtask, "DeleteRelLink", bDelete );
      EnableAction( vSubtask, "DeleteAllRelLinks", bDelete );

      CreateTrackingPopupMenu( vSubtask, "RelPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ATT_Popup
//
// PURPOSE:   Show Popup Menu for Rel ListBox
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ATT_Popup( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zPCHAR szName;
   zBOOL  bNew          = FALSE;
   zBOOL  bPaste        = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
      MB_SetMessage( vSubtask, 1, szName );

      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META );

      if ( CheckExistenceOfEntity( vTZEREMDO, "ER_Attribute" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
      {
         bNew = TRUE;
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_AttributeCopy" ) >= zCURSOR_SET )
            bPaste = TRUE;
      }

      EnableAction( vSubtask, "AddAttribute", bNew );
      EnableAction( vSubtask, "DeleteAttr", bDelete );
      EnableAction( vSubtask, "DeleteAllAttr", bDelete );
      EnableAction( vSubtask, "CopySelectedAttributes", bDelete );
      EnableAction( vSubtask, "PasteAttributes", bPaste );

      CreateTrackingPopupMenu( vSubtask, "AttPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ID_Popup
//
// PURPOSE:   Show Popup Menu for Identifier List Box
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ID_Popup( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zPCHAR szName;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
      MB_SetMessage( vSubtask, 1, szName );

      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZEREMDO, zSOURCE_ERD_META );

      // if SAP Entity, disable new and delete functions
      if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity",
                                      1 ) == 0 )
      {
         nIsCheckedOut = 0;
      }

      if ( CheckExistenceOfEntity( vTZEREMDO, "ER_EntIdentifier" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "AddIdentifier", bNew );
      EnableAction( vSubtask, "DeleteIdentifier", bDelete );
      EnableAction( vSubtask, "DeleteAllIdentifier", bDelete );

      CreateTrackingPopupMenu( vSubtask, "IDPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_EntityActions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ERD_EntityActions( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zPCHAR szName;
   zBOOL  bDeleteAll = FALSE;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
      MB_SetMessage( vSubtask, 1, szName );
      zwTZEREMDD_RefreshEntitySelect( vSubtask );

      if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
      {
         EnableAction( vSubtask, "AddEntity", FALSE );
         EnableAction( vSubtask, "DeleteEntity", FALSE );
         EnableAction( vSubtask, "CopyEntity", FALSE );
         EnableAction( vSubtask, "DeleteAllRelLinks", FALSE );
      }
      else
      {
         // If Entity a SAP Entities, disable Option "Delete all"
         if ( CheckExistenceOfEntity( vTZEREMDO, "ER_RelLink" ) >= zCURSOR_SET )
         {
            bDeleteAll = TRUE;
            if ( CompareAttributeToInteger( vTZEREMDO, "ER_Entity", "SAP_Entity",
                                            1 ) == 0 )
               bDeleteAll = FALSE;
         }

         EnableAction( vSubtask, "AddEntity", TRUE );
         EnableAction( vSubtask, "DeleteEntity", TRUE );
         EnableAction( vSubtask, "CopyEntity", TRUE );
         EnableAction( vSubtask, "DeleteAllRelLinks", bDeleteAll );
      }

      CreateTrackingPopupMenu( vSubtask, "EntityPopup",
                               pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_EntityCreate
//
// PURPOSE:   Add Entity to ER Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_EntityCreate( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   zwfnTZEREMDD_AddEntity( vSubtask, vTZEREMDO );
   AcceptEntity( vSubtask, "ERD" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_EntityNeighbors
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_EntityNeighbors( zVIEW vSubtask )
{
   zVIEW  vW;
   zVIEW  vT;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );
   GetViewByName( &vT, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Set up DIL message.
   strcpy_s( sz, sizeof( sz ), "Neighbors of entity (" );
   GetAddrForAttribute( &szName, vT, "ER_Entity", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), ") are selected." );

   SelectEntityNeighbors( vW, "ERD", 1, 0, 0 );

   // Set Selection in Subject Area
   if ( GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" ) >= 0 )
      SelectEntityNeighbors( vW, "ERD", 1, 0, 0 );

   MB_SetMessage( vSubtask, 1, sz );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_EntitySelect
//
// PURPOSE:   Select Entity in ER Model via Diagram (refresh DIL Msg)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_EntitySelect( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zPCHAR szName;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &szName, vTZEREMDO, "ER_Entity", "Name" );
   MB_SetMessage( vSubtask, 1, szName );

   zwTZEREMDD_RefreshEntitySelect( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_LClick
//
// PURPOSE:   Select Entity in ER Model via Diagram (refresh DIL Msg)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_LClick( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // If not selected, entity was toggled off.  Reset cursor to the next
   // selected entity.

   if ( GetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1 ) == 0 )
   {
      SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 0, 2 );

      SetSelectSetForView( vTZEREMDO, 1 );
      SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", 0 );
      SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 0, 2 );

      SetSelectSetForView( vTZEREMDO, 1 );
   }

   zwTZEREMDD_ERD_EntitySelect( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_Print
//
// PURPOSE:   Print the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_Print( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vW;
   zLONG    lPct;
   zLONG    lOrient;
   zLONG    lAlgorithm;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER,
                            "EMD", "ERD_PrintPercent" );
   GetIntegerFromAttribute( &lOrient, vProfileXFER,
                            "EMD", "ERD_PrintOrientation" );
   GetIntegerFromAttribute( &lAlgorithm, vProfileXFER,
                            "EMD", "ERD_PrintAlgorithm" );

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );
   if ( PrintDiagram( vW, "ERD", (zSHORT) lPct,
                      (zSHORT) (lOrient | lAlgorithm) ) > 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_RelActions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_ERD_RelActions( zVIEW vSubtask )
{
   zBOOL bEnable = TRUE;
   zVIEW vTZEREMDO;
   zVIEW vERD;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      zwTZEREMDD_ERD_RelSelect( vSubtask );   // Set DIL

      if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
         bEnable = FALSE;
      else
      {
         // if Relationship between 2 SAP Entities, disable "Delete" Action
         CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );
         if ( SetCursorFirstEntityByAttr( vERD, "ER_RelType_1", "ZKey",
                                          vERD, "ER_RelType", "ZKey",
                                          "EntpER_Model" ) >= zCURSOR_SET &&
              (CompareAttributeToInteger( vERD, "ER_Entity", "SAP_Entity",
                                          1 ) == 0 ||
               CompareAttributeToInteger( vERD, "ER_Entity_Other", "SAP_Entity",
                                          1 ) == 0) )
         {
            bEnable = FALSE;
         }

         DropView( vERD );
      }

      EnableAction( vSubtask, "ERD_DelRelLink", bEnable );

      CreateTrackingPopupMenu( vSubtask, "RelPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_RelCreate
//
// PURPOSE:   Add Relationship to ER Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_RelCreate( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( !vTZEREMDO || MiGetUpdateForView( vTZEREMDO ) == 0 )
   {
      MessageSend( vSubtask, "ER00208", "E/R Model Maintainance",
                   "Add of relationship is only supported if the Model is updateable.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      RemoveRelationship( vSubtask, "ERD", 1, 0 );
   }
   else
   {
      zwTZEREMDD_AddRelLink( vSubtask );
      AcceptRelationship( vSubtask, "ERD" );
      zwTZEREMDD_ERD_RelSelect( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_RelDelete
//
// PURPOSE:   Delete Relationship ER Model via Diagram ( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_RelDelete( zVIEW vSubtask )
{
   zVIEW          vTZEREMDO;
   zCHAR          szMessage[ 256 ];
   zPCHAR         sz;
   zSHORT         nRC;
   zVIEW          vERD;
   unsigned long  ulZKey;


   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                    vTZEREMDO, "ER_Entity_2", "" );
   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink",
                                    vTZEREMDO, "ER_RelLink_2", "" );
   //Prompt to confirm Delete
   szMessage[ 0 ] = 0;
   GetAddrForAttribute( &sz, vTZEREMDO, "ER_Entity", "Name" );
   strcat_s( szMessage, sizeof( szMessage ), sz );
   strcat_s( szMessage, sizeof( szMessage ), " " );
   GetAddrForAttribute( &sz, vTZEREMDO, "ER_RelLink", "Name" );
   strcat_s( szMessage, sizeof( szMessage ), sz );
   strcat_s( szMessage, sizeof( szMessage ), " " );
   GetAddrForAttribute( &sz, vTZEREMDO, "ER_Entity_Other", "Name" );
   strcat_s( szMessage, sizeof( szMessage ), sz );

   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD,
                     zENAME_ER_RELATIONSHIP, 0, 0, szMessage, 0 );

   if ( nRC == zRESPONSE_NO )
      return( 0 );

   // if delete Relationship and this Relationship is a part of the
   // Identifier, then delete the Relationship from the Identifier
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink_2", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_RelLink_2", "" ) )
   {
      SetCursorFirstEntityByEntityCsr( vERD, "ER_Entity",
                                       vERD, "ER_Entity_2", "" );
      GetIntegerFromAttribute( (zPLONG) &ulZKey, vERD,
                               "ER_RelLink_2", "ZKey" );
      for ( nRC = SetCursorFirstEntityByInteger( vERD, "ER_RelLinkIdentifier",
                  "ZKey", ulZKey, "EntpER_Model" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByInteger( vERD, "ER_RelLinkIdentifier",
                  "ZKey", ulZKey, "EntpER_Model" ))
      {
         if ( CheckExistenceOfEntity( vERD, "ER_AttributeIdentifier" ) == zCURSOR_NULL )
         {
            DeleteEntity( vERD, "ER_FactType", zREPOS_NONE );
         }

      }
   }
   DropView( vERD );

   // Invoke Object Operation to delete the relationship
   if ( oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NEXT, 0 ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   zwTZEREMDD_RefreshRllWnd( vSubtask );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_RelUpdate
//
// PURPOSE:   Position on correct relationship to go to Update Rel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_RelUpdate( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                    vTZEREMDO, "ER_Entity_2", "" );
   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink",
                                    vTZEREMDO, "ER_RelLink_2", "" );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_RelNeighbors
//
// PURPOSE:   Show the two related Entities for a Relationship
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_RelNeighbors( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vT;
   zVIEW  vW;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

   //  Get Access to ER Object ( and save it for later restore )
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vT, vTZEREMDO, 0 );

   // Set up DIL message.
   strcpy_s( sz, sizeof( sz ), "Neighbors of relationship (" );
   SetCursorFirstEntity( vT, "ER_RelLink_2", 0 );
   GetAddrForAttribute( &szName, vT, "ER_RelLink_2", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), " / " );
   SetCursorNextEntity( vT, "ER_RelLink_2", 0 );
   GetAddrForAttribute( &szName, vT, "ER_RelLink_2", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), ") are selected." );

   SetCursorFirstEntity( vTZEREMDO, "ER_RelLink_2", "" );
   // position right side of ER to first Entity and Select/Refresh it.
   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                    vTZEREMDO, "ER_Entity_2",
                                    "" );
   SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );

   SetCursorNextEntity( vTZEREMDO, "ER_RelLink_2", "" );
   // position right side of ER to second Entity and Select/Refresh it.
   SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                    vTZEREMDO, "ER_Entity_2", "" );
   SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );

   // Refresh Subject Area and set selection
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 0 );

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );
   MB_SetMessage( vW, 1, sz );

   DropView( vT );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_RelSelect
//
// PURPOSE:   Select Relationship ER Model via Diagram ( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_RelSelect( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDO1;
   zVIEW  vTZERSASO;
   zVIEW  vWindow;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

   // Get Access to ER Object
   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );
   else
      CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );

   // select one Relationship only.
   zwTZEREMDD_DeselectRelationship( vTZEREMDO, "ER_RelType", 1 );

   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   RefreshDiagram( vWindow, "ERD", 0 );

   // Get Access to Subject Area Dialog and Select Relationship
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 &&
        GetSubtaskForWindowName( vSubtask, &vWindow, "SA_Diagram" ) == 0 )
   {
      if ( SetCursorFirstEntityByAttr( vTZERSASO, "ER_RelType", "ZKey",
                                       vTZEREMDO, "ER_RelType", "ZKey",
                                       "SubjectArea" ) >= zCURSOR_SET )
         zwTZEREMDD_SA_RelSelect( vWindow );
   }

   SetCursorFirstEntityByEntityCsr( vTZEREMDO1, "ER_Entity",
                                    vTZEREMDO1, "ER_Entity_2", "" );
   SetCursorFirstEntityByEntityCsr( vTZEREMDO1, "ER_RelLink",
                                    vTZEREMDO1, "ER_RelLink_2", "" );
   GetAddrForAttribute( &szName, vTZEREMDO1, "ER_Entity", "Name" );
   strcpy_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), " [" );
   GetAddrForAttribute( &szName, vTZEREMDO1, "ER_RelLink", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), "] " );
   GetAddrForAttribute( &szName, vTZEREMDO1, "ER_Entity_Other", "Name" );
   strcat_s( sz, sizeof( sz ), szName );

   MB_SetMessage( vSubtask, 1, sz );
   DropView( vTZEREMDO1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_ZoomStandardSize
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomStandardSize( zVIEW vSubtask )
{
   zLONG    lPct;
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

// lPct = 75;     // 100 dks 3/5/96
   lPct = 100;

   ZoomDiagram( vW, "ERD", (zSHORT) lPct, 1 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_ZoomStdSizeSA
//
// PURPOSE:   Zoom in on the SA Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomStdSizeSA( zVIEW vSubtask )
{
   zLONG    lPct;
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" );

   lPct = 100;

   ZoomDiagram( vW, "ERD", (zSHORT)lPct, 1 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_ZoomIn
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomIn( zVIEW vSubtask )
{
   zLONG    lPct;
   zSHORT   nZoom;
   zVIEW    vProfileXFER;
   zVIEW    vW;

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER,
                            "EMD", "ERD_ZoomInPercent" );

   ZoomDiagram( vW, "ERD", (zSHORT)lPct, 2 );

   nZoom = ZoomDiagram( vSubtask, "ERD", 0, 0 );
   SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_ZoomOut
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomOut( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zLONG    lPct;
   zSHORT   nZoom;
   zVIEW    vW;

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER,
                            "EMD", "ERD_ZoomOutPercent" );
   if ( lPct > 0 )
      lPct = -lPct;

   ZoomDiagram( vW, "ERD", (zSHORT)lPct, 2 );

   nZoom = ZoomDiagram( vSubtask, "ERD", 0, 0 );
   SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_ERD_ZoomOutFit
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_ERD_ZoomOutFit( zVIEW vSubtask )
{
   zVIEW  vW;
   zSHORT nZoom;

   GetSubtaskForWindowName( vSubtask, &vW, "TZEREMDD" );

   nZoom = ZoomDiagram( vW, "ERD", 0, 3 );

   nZoom = ZoomDiagram( vSubtask, "ERD", 0, 0 );
   SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SA_SetSelection
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_SA_SetSelection( zVIEW vSubtask,
                            zVIEW vTZERSASO,
                            zVIEW vTZEREMDO )
{
   zSHORT nRC;
   zVIEW  vTZEREMDO1;

   CreateViewFromViewForTask( &vTZEREMDO1, vTZEREMDO, 0 );
   if ( SetCursorFirstSelectedEntity( vTZEREMDO1, "ER_RelType", "" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vTZERSASO, "ER_RelType", "ZKey",
                                       vTZEREMDO1, "ER_RelType", "ZKey",
                                       "SubjectArea" ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntity( vTZERSASO, "SA_RelType", 1 );
      }
   }
   // Now select our currently selected SA Entity (new)
   for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO1, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZEREMDO1, "ER_Entity", "" ) )
   {
      if ( SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey",
                                       vTZEREMDO1, "ER_Entity", "ZKey",
                                       "SubjectArea" ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntity( vTZERSASO, "SA_Entity", 1 );
      }
   }

   DropView( vTZEREMDO1 );
   RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
} // zwTZEREMDD_SA_SetSelection

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_PostBuildSA_Diagram
//
// PURPOSE:    Set title for SA Diagram window
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_PostBuildSA_Diagram( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZEREMDD;
   zVIEW  vTZERSASO;
   zVIEW  vProfileXFER;
   zLONG  lDrawOption;
   zLONG  lPSDLG_X;
   zLONG  lPSDLG_Y;
   zLONG  lSZDLG_X;
   zLONG  lSZDLG_Y;

   SetNameForView( vSubtask, "SAVIEW", vSubtask, zLEVEL_TASK );

   // Get Access to Views & Set Caption for Entity name
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDD, "TZEREMDD", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Subject Area", "TZERSASO", vTZERSASO,
                              "SubjectArea", zSOURCE_SA_META );

   // Size the window appropriateley
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lSZDLG_X, vProfileXFER, "Submodel", "SZDLG_X" );
   if ( lSZDLG_X )
   {
      GetIntegerFromAttribute( &lSZDLG_Y, vProfileXFER, "Submodel", "SZDLG_Y" );
      GetIntegerFromAttribute( &lPSDLG_X, vProfileXFER, "Submodel", "PSDLG_X" );
      GetIntegerFromAttribute( &lPSDLG_Y, vProfileXFER, "Submodel", "PSDLG_Y" );
      SetCtrlPosition( vSubtask, 0, (zSHORT)lPSDLG_X, (zSHORT)lPSDLG_Y, TRUE );
      SetCtrlSize( vSubtask, 0, (zSHORT)lSZDLG_X, (zSHORT)lSZDLG_Y, TRUE );
   }

   // check to see if ERD positions need setting.
   zwfnTZEREMDD_PosNonPosEntsSA( vSubtask, vTZERSASO );

   GetIntegerFromAttribute( &lDrawOption, vProfileXFER, "EMD", "SA_ShowRelOption" );
   SetDrawOptions( vSubtask, "ERD", (zUSHORT) lDrawOption );
   ZoomDiagram( vSubtask, "ERD", 0, 3 );

   zwfnTZEREMDD_SA_ViewDiag( vSubtask );
   zwTZEREMDD_SA_SetSelection( vSubtask, vTZERSASO, vTZEREMDO );

   // if SA not checked out, set Subject Area View (TZERSASO) read only ->
   // the user cannot update the values in Detail Windows (for example Window
   // Entity Detail)
   if ( !ComponentIsCheckedOut( vSubtask, vTZERSASO, zSOURCE_SA_META ))
   {
      SetViewReadOnly( vTZERSASO );
      EnableAction( vSubtask, "RemoveEntity", FALSE );
   }
   else
      EnableAction( vSubtask, "RemoveEntity", TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwfnTZEREMDD_PosNonPosEntsSA
//
// PURPOSE:    Position any non-positioned SA Entities in the ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_PosNonPosEntsSA( zVIEW vSubtask,
                              zVIEW vTZERSASO )
{
   zSHORT nRC;
   zLONG  lXgrid, lYgrid;
   zSHORT nEnts;
   zBOOL  bSomePosSet;
   zPCHAR szName;

   // check to see if ERD positions need setting.
   nEnts = zwfnTZEREMDD_InitCellsSA( vTZERSASO );
   if ( nEnts != nCellsUsedSA )
   {
      if ( nCellsUsedSA == 0 )
         bSomePosSet = 0;
      else
         bSomePosSet = 1;

      // Process SA Entities, setting any 0 positions
      for ( nRC = SetEntityCursor( vTZERSASO, "SA_Entity",
                                   "ER_DiagramPosX", zPOS_FIRST,
                                   0, 0, 0, 0, 0, 0 );
            nRC >= zCURSOR_SET;
            nRC = SetEntityCursor( vTZERSASO, "SA_Entity",
                                   "ER_DiagramPosX", zPOS_NEXT,
                                   0, 0, 0, 0, 0, 0 ) )
      {
         GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" ); // debug
         //  Set any non-positioned SA Entities
         GetIntegerFromAttribute( &lXgrid, vTZERSASO,
                                  "SA_Entity", "ER_DiagramPosX" );
         if ( lXgrid == 0 )
         {
            if ( bSomePosSet )
            {
               zwfnTZEREMDD_GetWeightedPosSA( &lXgrid, &lYgrid, vTZERSASO );
               zwfnTZEREMDD_FreePosSA( &lXgrid, &lYgrid, lXgrid, lYgrid,
                                       MAX_CELL_Y, vSubtask );
            }
            else
            {
               GetIntegerFromAttribute( &lXgrid, vTZERSASO,
                                        "ER_Entity", "ER_DiagramPosX" );
               GetIntegerFromAttribute( &lYgrid, vTZERSASO,
                                        "ER_Entity", "ER_DiagramPosY" );
            }
            if ( MiGetUpdateForView( vTZERSASO ) == 0 )
            {
               TraceLineS( "SA_PosNonPos: ====== ReadOnly View --- Can't"
                           " position ER_Entity: ", szName );  // debug only
            }
            else
            {
               SetAttributeFromInteger( vTZERSASO, "SA_Entity",
                                        "ER_DiagramPosX", lXgrid );
               SetAttributeFromInteger( vTZERSASO, "SA_Entity",
                                        "ER_DiagramPosY", lYgrid );
               zwfnTZEREMDD_MarkCellSA( lXgrid, lYgrid );
               RefreshEntity( vSubtask, "ERD" );
            }
         }
      } // End for each SA_Entity

      RefreshDiagram( vSubtask, "ERD", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_Actions
//
// PURPOSE:   Show Popup Menu for Submodel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_SA_Actions( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "ERD_Popup", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_Focus
//
// PURPOSE:   Give DIL for SA Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_SA_Focus( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;
   zVIEW  vWindow;

   zwTZEREMDD_RefreshSAWnd( vSubtask );

   if ( GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // Deselect Relationship.
      if ( (CheckExistenceOfEntity( vTZERSASO, "SA_RelType" ) >= zCURSOR_SET) &&
           (GetSelectStateOfEntity( vTZERSASO, "SA_RelType" ) == 1) )
      {
         SetSelectStateOfEntity( vTZERSASO, "SA_RelType", 0 );
         RefreshDiagram( vSubtask, "ERD", 0 );
      }

      // Refresh Data Model.
      if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 )
      {
         // Deselect Relationship.
         if ( (CheckExistenceOfEntity( vTZEREMDO, "ER_RelType" ) >= zCURSOR_SET) &&
              (GetSelectStateOfEntity( vTZEREMDO, "ER_RelType" ) == 1) )
         {
            SetSelectStateOfEntity( vTZEREMDO, "ER_RelType", 0 );
            GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
            RefreshDiagram( vWindow, "ERD", 1 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_DiagramPrint
//
// PURPOSE:   Print Subject Area ERD.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_SA_DiagramPrint( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vW;
   zLONG    lPct;
   zLONG    lOrient;
   zLONG    lAlgorithm;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER,
                            "EMD", "SA_PrintPercent" );
   GetIntegerFromAttribute( &lOrient, vProfileXFER,
                            "EMD", "SA_PrintOrientation" );
   GetIntegerFromAttribute( &lAlgorithm, vProfileXFER,
                            "EMD", "SA_PrintAlgorithm" );

   GetSubtaskForWindowName( vSubtask, &vW, "SA_Diagram" );
   if ( PrintDiagram( vW, "ERD", (zSHORT) lPct,
                      (zSHORT) (lOrient | lAlgorithm) ) > 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_SA_ZoomRead
//
// PURPOSE:    This Entry gets a view to the main window,
//             selects the current entity and centers it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_ZoomRead( zVIEW vSubtask )
{
   zVIEW vW;

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
// OPERATION: zwTZEREMDD_SA_ZoomIn
//
// PURPOSE:   Zoom in on the Submodel Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_ZoomIn( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zLONG    lPct;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER,
                            "EMD", "SA_ZoomInPercent" );

   ZoomDiagram( vSubtask, "ERD", (zSHORT) lPct, 2 );
   zwTZEREMDD_RefreshSAWnd( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_ZoomOut
//
// PURPOSE:   Zoom in on the Submodel Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_ZoomOut( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zLONG    lPct;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER,
                            "EMD", "SA_ZoomOutPercent" );
   if ( lPct > 0 )
      lPct = -lPct;

   ZoomDiagram( vSubtask, "ERD", (zSHORT) lPct, 2 );
   zwTZEREMDD_RefreshSAWnd( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_ZoomOutFit
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_ZoomOutFit( zVIEW vSubtask )
{
   ZoomDiagram( vSubtask, "ERD", 0, 3 );
   zwTZEREMDD_RefreshSAWnd( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_EntityActions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_EntityActions( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zPCHAR szName;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity(   vTZERSASO, "ER_Entity" ) < 0 )
      return( 0 );

   zwTZEREMDD_SA_EntitySelect( vSubtask );

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   MB_SetMessage( vSubtask, 1, szName );

   if ( MiGetUpdateForView( vTZERSASO ) == 0 )
      EnableAction( vSubtask, "RemoveEntity", FALSE );
   else
      EnableAction( vSubtask, "RemoveEntity", TRUE );

   CreateTrackingPopupMenu( vSubtask, "EntityPopup",
                            pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_EntityCreate
//
// PURPOSE:   Add Entity to ER Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_EntityCreate( zVIEW vSubtask )
{
   zVIEW    vTZERSASO;
   zVIEW    vTZEREMDO;
   zVIEW    vWindow;
   zVIEW    vList;
   zSHORT   nRC;

   // Get access to Object
   GetViewByName( &vList, "TZERENTO_List", vSubtask, zLEVEL_TASK );
   if ( vList == 0 )
      return( 0 );

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( vTZEREMDO == 0 || vTZERSASO == 0 )
      return( 0 );

   // Use List view to set position
   if ( SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                         vList, "ER_Entity",
                                         0 ) >= zCURSOR_SET )
   {
      oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vTZEREMDO );
      GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK );
      AcceptEntity( vWindow, "ERD" );
      for ( nRC = SetCursorFirstEntity( vTZERSASO, "ER_RelLink", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERSASO, "ER_RelLink", "" ) )
      {
         // Position the right side of the SA Object
         SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_RelType",
                                          vTZERSASO, "ER_RelType_1",
                                          "SubjectArea" );
         RefreshRelationship( vWindow, "ERD", 1 );
      }  // END for each ER_RelLink
   }

   DropObjectInstance( vList );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_EntityNeighbors
//
// PURPOSE:   Zoom in on the ER Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_EntityNeighbors( zVIEW vSubtask )
{
   zVIEW  vT;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

   GetViewByName( &vT, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Set up DIL message.
   strcpy_s( sz, sizeof( sz ), "Neighbors of entity (" );
   GetAddrForAttribute( &szName, vT, "ER_Entity", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), ") are selected." );

   SelectEntityNeighbors( vSubtask, "ERD", 1, 0, 0 );

   MB_SetMessage( vSubtask, 1, sz );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_EntitySelect
//
// PURPOSE:   Select Entity in ER Model via Diagram (refresh DIL Msg)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_EntitySelect( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZERSASO1;
   zVIEW  vTZEREMDO;
   zVIEW  vWindow;
   zPCHAR szName;
   zSHORT nRC;

   // Get Access to Objects
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   MB_SetMessage( vSubtask, 1, szName );

   // Get Access to ER Main Dialog and Select ER Entity
   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );

   // First, deselect all selected
   for ( nRC = SetCursorFirstSelectedEntity( vTZEREMDO, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZEREMDO, "ER_Entity", "" ) )
   {
      SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 0, 1 );
   }

   // Now select our currently selected SA Entity (new)
   // Frank H. (97/04/15): if the ERD is not checked out then the
   // SA is not correct linked to the ERD --> SetCursor by ZKey.
   CreateViewFromViewForTask( &vTZERSASO1, vTZERSASO, 0 );
   for ( nRC = SetCursorFirstSelectedEntity( vTZERSASO, "SA_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZERSASO, "SA_Entity", "" ) )
   {
      if ( SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey",
                                       vTZERSASO, "ER_Entity", "ZKey", "" ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntityForSet( vTZEREMDO, "ER_Entity", 1, 1 );
         zwTZEREMDD_SetSelectionInSAList( vTZEREMDO, vTZERSASO );
      }
   }

   SetCursorFirstEntityByAttr( vTZERSASO, "SA_Entity", "ZKey",
                               vTZERSASO1, "SA_Entity", "ZKey", "" );
   SetCursorFirstEntityByAttr( vTZEREMDO, "ER_Entity", "ZKey",
                               vTZERSASO, "ER_Entity", "ZKey", "" );
   DropView( vTZERSASO1 );

   // unselect Relationship
   if ( (CheckExistenceOfEntity( vTZERSASO, "SA_RelType" ) >= zCURSOR_SET) &&
        (GetSelectStateOfEntity( vTZERSASO, "SA_RelType" ) == 1) )
   {
      SetSelectStateOfEntity( vTZERSASO, "SA_RelType", 0 );
   }

   if ( (CheckExistenceOfEntity( vTZEREMDO, "ER_RelType" ) >= zCURSOR_SET) &&
        (GetSelectStateOfEntity( vTZEREMDO, "ER_RelType" ) == 1) )
   {
      SetSelectStateOfEntity( vTZEREMDO, "ER_RelType", 0 );
   }

   zwTZEREMDD_RefreshEntWnd( vSubtask );
   zwTZEREMDD_RefreshAttWnd( vSubtask );
   zwTZEREMDD_RefreshIdWnd( vSubtask );
   zwTZEREMDD_RefreshRllWnd( vSubtask );

   RefreshDiagram( vSubtask, "ERD", 0 );
   RefreshDiagram( vWindow, "ERD", 1 );
   RefreshCtrl( vSubtask, "SA_RelLink" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_EntityUpdate
//
// PURPOSE:   Select Entity in ER Model via Diagram (refresh DIL Msg)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_EntityUpdate( zVIEW vSubtask )
{
   zwTZEREMDD_SA_EntitySelect( vSubtask );

   // may need more in future
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_RelActions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZEREMDD_SA_RelActions( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity(   vTZERSASO, "ER_RelLink" ) < 0 )
      return( 0 );

   zwTZEREMDD_SA_RelSelect( vSubtask );   // Set DIL

   CreateTrackingPopupMenu( vSubtask, "RelPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_RelCreate
//
// PURPOSE:   Add Relationship to ER Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_RelCreate( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vTZRELSRC;
   zVIEW vTZRELTGT;
   zVIEW vWindow;

   RemoveRelationship( vSubtask, "ERD", 1, 0 ); // let the EMD Add the Rel
   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( !vTZEREMDO || MiGetUpdateForView( vTZEREMDO ) == 0 )
   {
      MessageSend( vSubtask, "ER00209", "E/R Model Maintainance",
                   "Add of relationship is only supported if the Model is updateable.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   {
      GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
      if ( vWindow )
      {
         GetViewByName( &vTZRELSRC, "TZERSASO", vSubtask, zLEVEL_TASK );
         SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                          vTZRELSRC, "ER_Entity", "" );
         CreateViewFromViewForTask( &vTZEREMDO, vTZEREMDO, 0 );
         GetViewByName( &vTZRELTGT, "TZRELTGT", vSubtask, zLEVEL_SUBTASK );
         SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_Entity",
                                          vTZRELTGT, "ER_Entity", "" );
         SetNameForView( vTZEREMDO, "TZRELTGT", vWindow, zLEVEL_SUBTASK );
         zwTZEREMDD_AddRelLink( vWindow );
         RefreshRelationship( vWindow, "ERD", 1 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_RelNeighbors
//
// PURPOSE:   Show the two related Entities for a Relationship
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_RelNeighbors( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vT;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

   // Get Access to SM Object ( and save it for later restore )
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vT, vTZERSASO, 0 );

   SetCursorFirstEntity( vTZERSASO, "ER_RelLink_2", "" );
   // position right side of ER to first Entity and Select/Refresh it. (old)
   //SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_Entity",
   //                                 vTZERSASO, "ER_Entity_2",
   //                                 "SubjectArea" );

   // Set up DIL message.
   strcpy_s( sz, sizeof( sz ), "Neighbors of relationship (" );
   SetCursorFirstEntity( vT, "ER_RelLink_2", 0 );
   GetAddrForAttribute( &szName, vT, "ER_RelLink_2", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), " / " );
   SetCursorNextEntity( vT, "ER_RelLink_2", 0 );
   GetAddrForAttribute( &szName, vT, "ER_RelLink_2", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), ") are selected." );

   // position right side of ER to second Entity and Select/Refresh it. (new)
   // Frank H. (97/04/15): if the ERD is not checked out then the
   // SA is not correct linked to the ERD --> SetCursor by ZKey.
   SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey",
                               vTZERSASO, "ER_Entity_2", "ZKey",
                               "SubjectArea" );


   SetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 1, 1 );
   RefreshEntity( vSubtask, "ERD" );

#if 0
   // construct first part of DIL for Rel
   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   MB_SetMessage( vSubtask, 1, szName );
   strcpy_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), " [" );
   GetAddrForAttribute( &szName, vTZERSASO, "ER_RelLink_2", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), "] " );
#endif

   SetCursorNextEntity( vTZERSASO, "ER_RelLink_2", "" );
   // position right side of ER to second Entity and Select/Refresh it. (old)
   //SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_Entity",
   //                                 vTZERSASO, "ER_Entity_2",
   //                                 "SubjectArea" );

   // position right side of ER to second Entity and Select/Refresh it. (new)
   // Frank H. (97/04/15): if the ERD is not checked out then the
   // SA is not correct linked to the ERD --> SetCursor by ZKey.
   SetCursorFirstEntityByAttr( vTZERSASO, "ER_Entity", "ZKey",
                               vTZERSASO, "ER_Entity_2", "ZKey",
                               "SubjectArea" );

   SetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 1, 1 );
   RefreshEntity( vSubtask, "ERD" );

   // Frank H. (97/04/15): also mark the ER-Entity !!!
   zwTZEREMDD_SA_EntitySelect( vSubtask );

#if 0
   // construct second part of DIL for Rel
   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
#endif

   MB_SetMessage( vSubtask, 1, sz );

   SetViewFromView( vTZERSASO, vT );  // reset view to ER

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_RelSelect
//
// PURPOSE:   Select Relationship ER Model via Diagram (refresh DIL Msg)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_RelSelect( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;
   zVIEW  vWindow;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

   // Get Access to ER Object
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );

   // Select one Relationship only.
   zwTZEREMDD_DeselectRelationship( vTZERSASO, "SA_RelType", 1 );
   RefreshDiagram( vSubtask, "ERD", 0 );

   // Get Access to Subject Area Dialog and Select Relationship
   if ( GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK ) > 0 &&
        GetSubtaskForWindowName( vSubtask, &vWindow, "TZEREMDD" ) == 0 )
   {
      if ( SetCursorFirstEntityByAttr( vTZEREMDO, "ER_RelType", "ZKey",
                                       vTZERSASO, "ER_RelType", "ZKey",
                                       "" ) >= zCURSOR_SET )
      {
         zwTZEREMDD_DeselectRelationship( vTZEREMDO, "ER_RelType", 1 );
         RefreshDiagram( vWindow, "ERD", 1 );
      }
   }

   CreateViewFromViewForTask( &vTZERSASO, vTZERSASO, 0 ); // protect current view
   SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_Entity",
                                    vTZERSASO, "ER_Entity_2",
                                    "SubjectArea" );
   SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_RelLink",
                                    vTZERSASO, "ER_RelLink_2", "" );

   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   strcpy_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), " [" );
   GetAddrForAttribute( &szName, vTZERSASO, "ER_RelLink", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   strcat_s( sz, sizeof( sz ), "] " );
   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity_Other", "Name" );
   strcat_s( sz, sizeof( sz ), szName );
   MB_SetMessage( vSubtask, 1, sz );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_RelUpdate
//
// PURPOSE:   Select Relationship ER Model via Diagram ( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_RelUpdate( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;


   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   zwTZEREMDD_SA_RelSelect( vSubtask ); // set DIL msg

   if ( GetCtrlState( vSubtask, "ERD", zCONTROL_STATUS_VISIBLE ) == 1 )
   {
      SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_RelLink",
                                       vTZERSASO, "ER_RelLink_2",
                                       "SubjectArea" );
      SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink",
                                       vTZERSASO, "ER_RelLink",
                                       "EntpER_Model" );
   }
   else
      zwTZEREMDD_SA_RefreshEntityList( vSubtask );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwfnTZEREMDD_PositionNonPosEnts
//
// PURPOSE:    This Entry positions any non positioned Entities for the
//             ER Diagram in the ER Dialog.
//
///////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_PositionNonPosEnts( zVIEW vSubtask,
                                 zVIEW vTZEREMDO )
{
   zLONG  lXgrid, lYgrid;
   zSHORT nRC, nEnts;
   zPCHAR szName;

   nEnts = zwfnTZEREMDD_InitCellsEDM( vTZEREMDO );
   if ( nCellsUsed == 0 )
   {
      zwTZEREMDD_PositionER( vSubtask );
      return( 0 );
   }

   if ( nEnts != nCellsUsed )
   {
      // Process ER Entities, setting any 0 positions
      for ( nRC = SetEntityCursor( vTZEREMDO, "ER_Entity",
                                   "ER_DiagramPosX",
                                   zPOS_FIRST | zQUAL_ATTR_NULL,
                                   0, 0, 0, 0, 0, 0 );
            nRC >= zCURSOR_SET;
            nRC = SetEntityCursor( vTZEREMDO, "ER_Entity",
                                   "ER_DiagramPosX",
                                   zPOS_NEXT | zQUAL_ATTR_NULL,
                                   0, 0, 0, 0, 0, 0 ) )
      {
         GetAddrForAttribute( &szName, vTZEREMDO,
                              "ER_Entity", "Name" ); // debug

         //  Set non-positioned ER Entities
         zwfnTZEREMDD_GetWeightedPosEDM( &lXgrid, &lYgrid, vTZEREMDO );
         zwfnTZEREMDD_FreePosEDM( &lXgrid, &lYgrid, lXgrid, lYgrid,
                                 MAX_CELL_Y, vSubtask );
         SetAttributeFromInteger( vTZEREMDO, "ER_Entity",
                                  "ER_DiagramPosX", lXgrid );
         SetAttributeFromInteger( vTZEREMDO, "ER_Entity",
                                  "ER_DiagramPosY", lYgrid );
         zwfnTZEREMDD_MarkCellEDM( lXgrid, lYgrid );
         RefreshEntity( vSubtask, "ERD" );
//       RefreshDiagram( vSubtask, "ERD", 0 );
      } // End for each ER_Entity
   }

// RefreshDiagram( vSubtask, "ERD", 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_InitCellsEDM( zVIEW vTZEREMDO )
{
   zVIEW vTZEREMD1;
   zSHORT nRC;
   zSHORT nEnts;
   zLONG  lXgrid, lYgrid;
// zCHAR sz[ 256 ];

   nEnts = 0;
   nCellsUsed = 0;
   zmemset( cCellUsage, 0, (unsigned) MAX_CELL_X * (unsigned) MAX_CELL_Y );

   CreateViewFromViewForTask( &vTZEREMD1, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstEntity( vTZEREMD1, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZEREMD1, "ER_Entity", "" ) )
   {
//    GetStringFromAttribute( sz, vTZEREMD1, "ER_Entity", "Name" ); //debug only
      nEnts++;
      GetIntegerFromAttribute( &lXgrid, vTZEREMD1, "ER_Entity", "ER_DiagramPosX" );
      if ( lXgrid != 0 )
      {
         GetIntegerFromAttribute( &lYgrid, vTZEREMD1, "ER_Entity", "ER_DiagramPosY" );
         zwfnTZEREMDD_MarkCellEDM( lXgrid, lYgrid );
      }
   }  // End for each SA_Entity

   return( nEnts );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_MarkCellEDM( zLONG   lXgrid,
                          zLONG   lYgrid )
{
   zSHORT nX, nY;

   if ( lXgrid != 0 )
   {
      nX = (zSHORT) (lXgrid / CELLS_PER_ENTITY_X);
      nY = (zSHORT) (lYgrid / CELLS_PER_ENTITY_Y);
      cCellUsage[ nX ][ nY ] = 1;
      nCellsUsed++;
      return( 1 );
   }
   else
      return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_UnmarkCellEDM( zLONG   lXgrid,
                            zLONG   lYgrid )
{

   zSHORT nX, nY;

   if ( lXgrid != 0 )
   {
      nX = (zSHORT) (lXgrid / CELLS_PER_ENTITY_X);
      nY = (zSHORT) (lYgrid / CELLS_PER_ENTITY_Y);
      cCellUsage[ nX ][ nY ] = 0;
      if ( nCellsUsed > 0 )
         nCellsUsed--;

      return( 1 );
   }
   else
      return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_FreePosEDM( zPLONG  plXout,
                         zPLONG  plYout,
                         zLONG   lXcenter,
                         zLONG   lYcenter,
                         zSHORT  nDlimit,
                         zVIEW   vSubtask )
{
   zSHORT nX, nY;
   zLONG  lX, lY;
   zSHORT nXcenter, nYcenter;

   nXincr = nYincr = nD = 0;
   nXcenter = (zSHORT) (lXcenter / CELLS_PER_ENTITY_X);
   nYcenter = (zSHORT) (lYcenter / CELLS_PER_ENTITY_Y);

   lX = lY = 0;
   if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) >= 0 )
   for ( ; ; ) // loop until free cell located
   {
      if ( (zSHORT)sqrt( nXincr * nXincr + nYincr * nYincr ) <= nD )
      {
         nX = nXcenter + nXincr;
         nY = nYcenter + nYincr;
         if ( nX > 0 && nX < MAX_CELL_X &&
            nY > 0 && nY < MAX_CELL_Y )
         {
            if ( cCellUsage[ nX ][ nY] == 0 )
            {
               lX = nX * CELLS_PER_ENTITY_X;
               lY = nY * CELLS_PER_ENTITY_Y;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) == 1 )
                  break;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+1 ) == 1 )
               {
                  lY++;
                  break;
               }
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+2 ) == 1 )
               {
                  lY++;
                  lY++;
                  break;
               }
               lX++;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) == 1 )
                  break;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+1 ) == 1 )
               {
                  lY++;
                  break;
               }
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+2 ) == 1 )
               {
                  lY++;
                  lY++;
                  break;
               }
               lX++;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) == 1 )
                  break;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+1 ) == 1 )
               {
                  lY++;
                  break;
               }
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+2 ) == 1 )
               {
                  lY++;
                  lY++;
                  break;
               }
            }
         }
      }

      // increment nYincr, nXincr, or nD as appropriate
      if ( nXincr > -nD )
         nXincr--;
      else
      {
         if ( nYincr > -nD )
         {
            nYincr--;
            nXincr = nD;
         }
         else
         {
            if ( nD <= nDlimit )
            {
               nD++;
               nYincr = nD;
               nXincr = nD;
            }
            else
            {
               *plXout = *plYout = 0;
               return( -1 );
            }
         }
      }
   }

   *plXout = lX;
   *plYout = lY;
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_GetWeightedPosEDM( zPLONG  plXout,
                                zPLONG  plYout,
                                zVIEW   vTZEREMDO )
{
   zVIEW vTZEREMD1;
   zVIEW vTZEREMD2;
   zSHORT nRC, nX, nY, nRels;
   zLONG  lXgrid, lYgrid;
   zCHAR sz[ 256 ];

   nX = nY = nRels = 0;

   CreateViewFromViewForTask( &vTZEREMD1, vTZEREMDO, 0 );
   CreateViewFromViewForTask( &vTZEREMD2, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstEntity( vTZEREMD1, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZEREMD1, "ER_RelLink", "" ) )
   {

      GetStringFromAttribute( sz, sizeof( sz ), vTZEREMD1, "ER_Entity", "Name" ); //debug only
      GetStringFromAttribute( sz + 50, sizeof( sz ) - 50, vTZEREMD1, "ER_Entity_Other", "Name" ); //debug only
      if ( SetCursorFirstEntityByEntityCsr( vTZEREMD2, "ER_Entity", vTZEREMD1, "ER_Entity_Other", "" ) >= zCURSOR_SET )
      {
         nRels++;
         GetIntegerFromAttribute( &lXgrid, vTZEREMD2, "ER_Entity", "ER_DiagramPosX" );
         GetIntegerFromAttribute( &lYgrid, vTZEREMD2, "ER_Entity", "ER_DiagramPosY" );
         nX += (zSHORT)lXgrid;
         nY += (zSHORT)lYgrid;
      }
   }  // End for each ER_ERlLink

   if ( nRels == 0 )
   {
      *plXout = 1;
      *plYout = 1;
   }
   else
   {
      *plXout = nX / nRels;
      if ( *plXout < 1 ) *plXout = 1;
      *plYout = nY / nRels;
      if ( *plYout < 1 ) *plYout = 1;
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_InitCellsSA( zVIEW vTZERSASO )
{
   zVIEW vTZERSAS1;
   zSHORT nRC;
   zSHORT nEnts;
   zLONG  lXgrid, lYgrid;
// zCHAR sz[ 256 ];

   nEnts = 0;
   nCellsUsedSA = 0;
// zmemset( &cCellUsageSA[ 0 ][ 0 ], 0, MAX_CELL_X * MAX_CELL_Y );
   zmemset( cCellUsage, 0, (unsigned) MAX_CELL_X * (unsigned) MAX_CELL_Y );

   CreateViewFromViewForTask( &vTZERSAS1, vTZERSASO, 0 );

   for ( nRC = SetCursorFirstEntity( vTZERSAS1, "SA_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSAS1, "SA_Entity", "" ) )
   {
//    GetStringFromAttribute( sz, sizeof( sz ), vTZERSAS1, "ER_Entity", "Name" ); //debug only
      nEnts++;
      GetIntegerFromAttribute( &lXgrid, vTZERSAS1, "SA_Entity", "ER_DiagramPosX" );
      if ( lXgrid != 0 )
      {
         GetIntegerFromAttribute( &lYgrid, vTZERSAS1, "SA_Entity", "ER_DiagramPosY" );
         zwfnTZEREMDD_MarkCellSA( lXgrid, lYgrid );
      }
   }  // End for each SA_Entity

   return( nEnts );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_MarkCellSA( zLONG   lXgrid,
                         zLONG   lYgrid )
{
   zSHORT nX, nY;

   if ( lXgrid != 0 )
   {
      nX = (zSHORT) (lXgrid / CELLS_PER_ENTITY_X);
      nY = (zSHORT) (lYgrid / CELLS_PER_ENTITY_Y);
      cCellUsageSA[ nX ][ nY ] = 1;
      nCellsUsedSA++;
      return( 1 );
   }
   else
      return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_UnmarkCellSA( zLONG   lXgrid,
                         zLONG   lYgrid )
{
   zSHORT nX, nY;

   if ( lXgrid != 0 )
   {
      nX = (zSHORT) (lXgrid / CELLS_PER_ENTITY_X);
      nY = (zSHORT) (lYgrid / CELLS_PER_ENTITY_Y);
      cCellUsageSA[ nX ][ nY ] = 0;
      if ( nCellsUsedSA > 0 )
         nCellsUsedSA--;

      return( 1 );
   }
   else
      return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_FreePosSA( zPLONG  plXout,
                        zPLONG  plYout,
                        zLONG   lXcenter,
                        zLONG   lYcenter,
                        zSHORT  nDlimit,
                        zVIEW   vSubtask )
{
   zSHORT nX, nY;
   zLONG  lX, lY;
   zSHORT nXcenter, nYcenter;

   nXincrSA = nYincrSA = nD_SA = 0;
   nXcenter = (zSHORT) (lXcenter / CELLS_PER_ENTITY_X);
   nYcenter = (zSHORT) (lYcenter / CELLS_PER_ENTITY_Y);

   lX = lY = 0;
   if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) >= 0 )
   for ( ; ; )
   {
      if ( (zSHORT)sqrt( nXincrSA * nXincrSA + nYincrSA * nYincrSA ) <= nD_SA )
      {
         nX = nXcenter + nXincrSA;
         nY = nYcenter + nYincrSA;
         if ( nX > 0 && nX < MAX_CELL_X &&
            nY > 0 && nY < MAX_CELL_Y )
         {
            if ( cCellUsageSA[ nX ][ nY] == 0 )
            {
               lX = nX * CELLS_PER_ENTITY_X;
               lY = nY * CELLS_PER_ENTITY_Y;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) == 1 )
                  break;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+1 ) == 1 )
               {
                  lY++;
                  break;
               }
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+2 ) == 1 )
               {
                  lY++;
                  lY++;
                  break;
               }
               lX++;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) == 1 )
                  break;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+1 ) == 1 )
               {
                  lY++;
                  break;
               }
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+2 ) == 1 )
               {
                  lY++;
                  lY++;
                  break;
               }
               lX++;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) == 1 )
                  break;
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+1 ) == 1 )
               {
                  lY++;
                  break;
               }
               if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY+2 ) == 1 )
               {
                  lY++;
                  lY++;
                  break;
               }
            }
         }
      }

      // increment nYincrSA, nXincrSA, or nD_SA as appropriate
      if ( nXincrSA > -nD_SA )
         nXincrSA--;
      else
      {
         if ( nYincrSA > -nD_SA )
         {
            nYincrSA--;
            nXincrSA = nD_SA;
         }
         else
         {
            if ( nD_SA <= nDlimit )
            {
               nD_SA++;
               nYincrSA = nD_SA;
               nXincrSA = nD_SA;
            }
            else
            {
               *plXout = *plYout = 0;
               return( -1 );
            }
         }
      }
   }

   *plXout = lX;
   *plYout = lY;

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_GetWeightedPosSA( zPLONG  plXout,
                               zPLONG  plYout,
                               zVIEW   vTZERSASO )
{
   zVIEW vTZERSAS1;
   zVIEW vTZERSAS2;
   zSHORT nRC, nX, nY, nRels;
   zLONG  lXgrid, lYgrid;
   zCHAR sz[ 256 ];

   nX = nY = nRels = 0;

   CreateViewFromViewForTask( &vTZERSAS1, vTZERSASO, 0 );
   CreateViewFromViewForTask( &vTZERSAS2, vTZERSASO, 0 );

   for ( nRC = SetCursorFirstEntity( vTZERSAS1, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSAS1, "ER_RelLink", "" ) )
   {
      GetStringFromAttribute( sz, sizeof( sz ), vTZERSAS1, "ER_Entity", "Name" ); //debug only
      GetStringFromAttribute( sz + 50, sizeof( sz ) - 50, vTZERSAS1, "ER_Entity_Other", "Name" ); //debug only
      if ( SetCursorFirstEntityByEntityCsr( vTZERSAS2, "ER_Entity",
                                            vTZERSAS1, "ER_Entity_Other",
                                            "SubjectArea" ) >= zCURSOR_SET )
      {
         nRels++;
         GetIntegerFromAttribute( &lXgrid, vTZERSAS2, "SA_Entity", "ER_DiagramPosX" );
         GetIntegerFromAttribute( &lYgrid, vTZERSAS2, "SA_Entity", "ER_DiagramPosY" );
         nX += (zSHORT)lXgrid;
         nY += (zSHORT)lYgrid;
      }
   }  // End for each SA_Entity

   if ( nRels == 0 )
   {
      *plXout = 1;
      *plYout = 1;
   }
   else
   {
      *plXout = nX / nRels - 1;
      if ( *plXout < 1 ) *plXout = 1;
      *plYout = nY / nRels - 1;
      if ( *plYout < 1 ) *plYout = 1;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_ChooseView
//
// PURPOSE:   Swap current view of SA ( ERD / List )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_ChooseView( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zPCHAR   szListView;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetAddrForAttribute( &szListView, vProfileXFER, "EMD", "SA_ViewList" );
   if ( *szListView == 'Y' )
      zwfnTZEREMDD_SA_ViewDiag( vSubtask );
   else
      zwfnTZEREMDD_SA_ViewList( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwfnTZEREMDD_SA_ViewList
//
// PURPOSE:   To make the ERD invisible and the List visible
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_SA_ViewList( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetAttributeFromString( vProfileXFER, "EMD", "SA_ViewList", "Y" );

   SetCtrlState( vSubtask, "ERD",        zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "5",          zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "6",          zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "SA_Entity",  zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "SA_RelLink", zCONTROL_STATUS_VISIBLE, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwfnTZEREMDD_SA_ViewDiag
//
// PURPOSE:   To make the ERD visible and the List invisible
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_SA_ViewDiag( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   SetAttributeFromString( vProfileXFER, "EMD", "SA_ViewList", "" );

   SetCtrlState( vSubtask, "ERD",        zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "5",          zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "6",          zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "SA_Entity",  zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "SA_RelLink", zCONTROL_STATUS_VISIBLE, 0 );

   // Refresh Subject Area and set selection
   zwfnTZEREMDD_RefreshEntitySelect( vSubtask, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwfnTZEREMDD_RePositionEntity
//
// PURPOSE:    This Entry positions any non positioned Entities for the
//             ER Diagram in the ER Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZEREMDD_RePositionEntity( zVIEW vSubtask,
                               zVIEW vTZEREMDO_In )
{
   zVIEW  vTZEREMDO;
   zVIEW  vWindow;
   zSHORT nRC;

   // Perserve current View
   CreateViewFromViewForTask( &vTZEREMDO, vTZEREMDO_In, 0 );

   // Clear all current postioning information for Entity
   SetAttributeFromInteger( vTZEREMDO, "ER_Entity", "ER_DiagramPosX", 0 );
   SetAttributeFromInteger( vTZEREMDO, "ER_Entity", "ER_DiagramPosY", 0 );

   for ( nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" ) )
   {
      // clear any relationship positioning
      SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1", "ER_DiagramBendPosX", 0 );
      SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1", "ER_DiagramBendPosY", 0 );
      SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1", "ER_DiagramE1PosX", 0 );
      SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1", "ER_DiagramE1PosY", 0 );
      SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1", "ER_DiagramE2PosX", 0 );
      SetAttributeFromInteger( vTZEREMDO, "ER_RelType_1", "ER_DiagramE2PosY", 0 );
   }  // END for each ER_RelLink

   // Now simply position the non positioned Entity if update allowed
   if ( MiGetUpdateForView( vTZEREMDO ) == 1 )
   {
      zwfnTZEREMDD_PositionNonPosEnts( vSubtask, vTZEREMDO );
   }

//  See if ERD needs refreshing
// GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
// if ( CompareAttributeToString( vProfileXFER, "EMD", "ERD_View", "Y" )
//      == 0 )
// {
      GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
      if ( vWindow )
      {  // refresh the entity and each relationship
         RefreshEntity( vWindow, "ERD" );
         for ( nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" ) )
         {
            // Position the right side of the ER Object
            SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelType", vTZEREMDO, "ER_RelType_1", "" );
            RefreshRelationship( vWindow, "ERD", 1 );
         }  // END for each ER_RelLink
      }
// }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_SelectPreBuild
//
// PURPOSE:    This Entry creates a work object to use for selection of
//             ER Entities to be added to Submodel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_SelectPreBuild( zVIEW vSubtask )
{
   zVIEW  vTZEREMDO;
   zVIEW  vTZERSASO;
   zVIEW  vEDM;
   zVIEW  vList;
   zVIEW  vSM;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_ANY );
   if ( vTZEREMDO == 0 || vTZERSASO == 0 )
      return( 0 );

   CreateViewFromViewForTask( &vEDM, vTZEREMDO, 0 );
   CreateViewFromViewForTask( &vSM, vTZERSASO, 0 );

   // Activate Empty Entity List Object and include any ER_Entities from
   // the EDM that are not allready in the Submodel
   ActivateEmptyObjectInstance ( &vList, "TZERENTO", vSubtask,
                                 zMULTIPLE | zLEVEL_TASK );

   for ( nRC = SetCursorFirstEntity( vEDM, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vEDM, "ER_Entity", "" ) )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vSM, "ER_Entity",
                                             vEDM, "ER_Entity",
                                             "SubjectArea" );
      if ( nRC < zCURSOR_SET )
         nRC = IncludeSubobjectFromSubobject( vList, "ER_Entity",
                                             vEDM, "ER_Entity",
                                             zPOS_AFTER );
   }

   if ( SetCursorFirstEntity( vList, "ER_Entity", "" ) < zCURSOR_SET )
      EnableAction( vSubtask, "AddOneEntityToSA", 0 );
   else
      EnableAction( vSubtask, "AddOneEntityToSA", 1 );

   SetNameForView( vList, "TZERENTO_List", vSubtask, zLEVEL_TASK );
   DropView( vEDM );
   DropView( vSM );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_SelectPostBuild
//
// PURPOSE:    This Entry sets the Graying for the Add Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SelectPostBuild( zVIEW vSubtask )
{
   zVIEW vTZEREMDO;
   zVIEW vList;

   // Get Access to ER Object
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( vTZEREMDO )
   {
      if ( MiGetUpdateForView( vTZEREMDO ) == 0 )
         SetCtrlState( vSubtask, "Add",     zCONTROL_STATUS_ENABLED, 0 );
      else
         SetCtrlState( vSubtask, "Add",     zCONTROL_STATUS_ENABLED, 1 );
   }

   GetViewByName( &vList, "TZERENTO_List", vSubtask, zLEVEL_TASK );
// if ( SetCursorFirstEntity( vList, "ER_Entity", "" ) < zCURSOR_SET )
// {
//    SetCtrlState( vSubtask, "Select",     zCONTROL_STATUS_ENABLED, 0 );
// }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZEREMDD_SA_CancelEntSelect
//
// PURPOSE:    This Entry deletes the work object for selection of
//             ER Entities to be added to Submodel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_CancelEntSelect( zVIEW vSubtask )
{
   zVIEW  vList;
   zVIEW  vWindow;

   GetViewByName( &vList, "TZERENTO_List", vSubtask, zLEVEL_TASK );
   DropObjectInstance( vList );

   GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK );
   RemoveEntity( vWindow, "ERD", 1, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZEREMDD_SA_CreateNewEntity
//
// PURPOSE:   Add Entity to ER Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_SA_CreateNewEntity( zVIEW vSubtask )
{
   zVIEW vTZERSASO;
   zVIEW vTZEREMDO;
   zVIEW vList;
   zVIEW vWindow;

   GetViewByName( &vList, "TZERENTO_List", vSubtask, zLEVEL_TASK );
   if ( vList == 0 )
      return( 0 );

   DropObjectInstance( vList );

   // Get access to Objects
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   if ( vTZEREMDO == 0 || vTZERSASO == 0 )
      return( 0 );

   GetViewByName( &vWindow, "TZEREMDD", vSubtask, zLEVEL_TASK );
   zwTZEREMDD_AddEntity( vWindow );

   oTZERSASO_AddOneSA_EntityFrER( vSubtask, vTZERSASO, vTZEREMDO );

   GetViewByName( &vWindow, "SAVIEW", vSubtask, zLEVEL_TASK );
#if 1
   SetNameForView( vWindow, "SA_ENTITY_ADD", vSubtask, zLEVEL_TASK );
#else
   AcceptEntity( vWindow, "ERD" );
#endif

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: IncreaseRelationshipPorts
//
// PURPOSE:   Increment the number of relationship ports for an entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
IncreaseRelationshipPorts( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW vDiagram;
   zBOOL  bReadOnly = FALSE;

   if ( GetViewByName( &vDiagram, "TZEREMDO", vSubtask, zLEVEL_ANY ) > 0 )
   {
      if ( MiGetUpdateForView( vDiagram ) == 0 )
      {
         bReadOnly = TRUE;
         SetViewUpdate( vDiagram );
      }
   }

   nRC = AdjustRelPortsForSelectedEnt( vSubtask, "ERD", TRUE );

   if (bReadOnly)
      SetViewReadOnly( vDiagram );

   if ( nRC > 1 )
   {
      MessageSend( vSubtask, "ER00XXX", "E/R Model Maintainance",
                   "Multiple Entities cannot be selected for "
                   "Increase Entity ports",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
   if ( nRC > 0 )
   {
      MessageSend( vSubtask, "ER00XXX", "E/R Model Maintainance",
                   "Entity not selected for Increase Entity ports",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: IncreaseRelationshipPortsForSA
//
// PURPOSE:   Increment the number of relationship ports for an SA entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
IncreaseRelationshipPortsForSA( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   // Get Access to SA & ER
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Increase the ports for the SA
   nRC = AdjustRelPortsForSelectedEnt( vSubtask, "ERD", TRUE );
   if ( nRC > 0 )
   {
      MessageSend( vSubtask, "ER00XXX", "E/R Model Maintainance",
                   "Entity not selected for Increase Entity ports",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: DecreaseRelationshipPorts
//
// PURPOSE:   Decrement the number of relationship ports for an entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DecreaseRelationshipPorts( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW vDiagram;
   zBOOL  bReadOnly = FALSE;

   if ( GetViewByName( &vDiagram, "TZEREMDO", vSubtask, zLEVEL_ANY ) > 0 )
   {
      if ( MiGetUpdateForView( vDiagram ) == 0 )
      {
         bReadOnly = TRUE;
         SetViewUpdate( vDiagram );
      }
   }

   nRC = AdjustRelPortsForSelectedEnt( vSubtask, "ERD", FALSE );

   if (bReadOnly)
      SetViewReadOnly( vDiagram );

   if ( nRC > 0 )
   {
      MessageSend( vSubtask, "ER00XXX", "E/R Model Maintainance",
                   "Entity not selected for Decrease Entity ports",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: DecreaseRelationshipPortsForSA
//
// PURPOSE:   Decrement the number of relationship ports for an SA entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DecreaseRelationshipPortsForSA( zVIEW vSubtask )
{
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   // Get Access to SA & ER
   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Decrease the ports for the SA
   nRC = AdjustRelPortsForSelectedEnt( vSubtask, "ERD", FALSE );
   if ( nRC > 0 )
   {
      MessageSend( vSubtask, "ER00XXX", "E/R Model Maintainance",
                   "Entity not selected for Decrease Entity ports",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: ERD_ExpandDiagram
//
// PURPOSE:   Expand distance between all entities.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ExpandDiagram( zVIEW vSubtask )
{
   return( ERD_ExpandDiagram( vSubtask, "ERD", 8 ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: ERD_ContractDiagram
//
// PURPOSE:   Contract distance between all entities.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ContractDiagram( zVIEW vSubtask )
{
   return( ERD_ContractDiagram( vSubtask, "ERD", 4 ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: RecalculateAllRelationships
//
// PURPOSE:   Recalculate all relationships for all entities
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
RecalcAllRels( zVIEW vSubtask )
{
   return( RecalculateAllRelationships( vSubtask, "ERD" ) );
}

static void
zwfnTZEREMDD_EntitySA_AddSA( zVIEW vSubtask,
                             zVIEW vXfer,
                             zVIEW vTemp )
{
   zVIEW  vSA;
   zCHAR  szMetaName[ 33 ];
   zCHAR  szFullName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   HFILE  hFile;
   zSHORT nRC;

   GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vTemp, "W_MetaDef", "Name" );
   GetStringFromAttribute( szFullName, sizeof( szFullName ), vTemp, "LPLR", "MetaSrcDir" );
   strcat_s( szFullName, sizeof( szFullName ), "\\" );
   strcat_s( szFullName, sizeof( szFullName ), szMetaName );
   strcat_s( szFullName, sizeof( szFullName ), ".PSA" );

   hFile = SysOpenFile( vSubtask, szFullName, COREFILE_READ );
   if ( hFile >= 0 )
   {
      SysCloseFile( vSubtask, hFile, 0 );
      nRC = ActivateOI_FromFile( &vSA, "TZERSASO", vSubtask,
                                 szFullName, zSINGLE );
      if ( nRC < 0 )
      {
         TraceLineS( "Error reading file: ", szFullName );
         strcpy_s( szMsg, sizeof( szMsg ), "Error in reading Subject Area: " );
         strcat_s( szMsg, sizeof( szMsg ), szMetaName );
         MessageSend( vSubtask, "ER00190", "E/R Model Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         for ( nRC = SetCursorFirstEntity( vSA, "SA_Entity", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vSA, "SA_Entity", 0 ) )
         {
            CreateEntity( vXfer, "EntitySA", zPOS_AFTER );
            SetAttributeFromAttribute( vXfer, "EntitySA",    "EntityName",
                                       vSA,   "ER_Entity",   "Name" );
            SetAttributeFromAttribute( vXfer, "EntitySA",    "SA_Name",
                                       vSA,   "SubjectArea", "Name" );
         }

         DropView( vSA );
      }
   }
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_EntitySA_List( zVIEW vSubtask )
{
   // This operation creates a cross reference list of ER_Entity to each
   // Subject Area in which the ER_Entity appears.

   zVIEW  vTaskLPLR;
   zVIEW  vTemp;
   zVIEW  vXfer;
   zVIEW  vERD;
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   if ( GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // If the cross reference object exists, use it, but delete existing
      // entries.
      for ( nRC = SetCursorFirstEntity( vXfer, "EntitySA", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "EntitySA", 0 ) )
      {
         DeleteEntity( vXfer, "EntitySA", zREPOS_NONE );
      }
   }
   else
   {
      ActivateEmptyObjectInstance( &vXfer, "TZBRLOVO", vTaskLPLR, zMULTIPLE );
      CreateEntity( vXfer, "Root", zPOS_AFTER );
      SetNameForView( vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );
   }

   // Loop through all Subject Areas.
   CreateViewFromViewForTask( &vTemp, vTaskLPLR, 0 );
   SetCursorFirstEntityByInteger( vTemp, "W_MetaType", "Type",
                                  zREFER_SA_META, 0 );

   for ( nRC = SetCursorFirstEntity( vTemp, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTemp, "W_MetaDef", 0 ) )
   {
      zwfnTZEREMDD_EntitySA_AddSA( vSubtask, vXfer, vTemp );
   }

   DropView( vTemp );

   // Now loop through all ER_Entity entities in the ERD and make sure an
   // entry exists in vXfer for every ER_Entity.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 ) )
   {
      nRC = SetCursorFirstEntityByAttr( vXfer, "EntitySA",  "EntityName",
                                        vERD,  "ER_Entity", "Name", 0 );
      if ( nRC < zCURSOR_SET )
      {
         CreateEntity( vXfer, "EntitySA", zPOS_AFTER );
         SetAttributeFromAttribute( vXfer, "EntitySA",    "EntityName",
                                    vERD,  "ER_Entity",   "Name" );
      }
   }

   // Order the list by AttributeName.
   OrderEntityForView( vXfer, "EntitySA", "EntityName A SA_Name A" );

   // At the end, position on the first entry.
   SetCursorFirstEntity( vXfer, "EntitySA", 0 );
   DropView( vERD );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_EntitySA_ListOrder( zVIEW vSubtask )
{
   // This operation simply orders the SA_EntityList by Entity Name or
   // SA Name.

   zVIEW  vXfer;

   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vXfer, "Root", "OrderType", "E" ) == 0 )
      OrderEntityForView( vXfer, "EntitySA", "EntityName A SA_Name A" );
   else
      OrderEntityForView( vXfer, "EntitySA", "SA_Name A EntityName A" );

   return( 0 );
}

static void
zwfnTZEREMDD_CreateEntity_RS( zVIEW vXfer,
                              zVIEW vERD )
{
   zSHORT nRC;

   // Loop through all the ER_Entities, creating ER_Entity entries and
   // ER_Attribute entries within them.

   for ( nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 ) )
   {
      CreateEntity( vXfer, "ER_Entity", zPOS_AFTER );
      SetMatchingAttributesByName( vXfer, "ER_Entity",
                                   vERD,  "ER_Entity", zSET_ALL );

      for ( nRC = SetCursorFirstEntity( vERD, "ER_Attribute", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vERD, "ER_Attribute", 0 ) )
      {
         CreateEntity( vXfer, "ER_Attribute", zPOS_AFTER );
         SetMatchingAttributesByName( vXfer, "ER_Attribute",
                                      vERD,  "ER_Attribute", zSET_ALL );
      }

      for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vERD, "ER_RelLink", 0 ) )
      {
         CreateEntity( vXfer, "ER_RelType", zPOS_AFTER );
         SetAttributeFromAttribute( vXfer, "ER_RelType", "RelLinkName",
                                    vERD,  "ER_RelLink", "Name" );
         SetAttributeFromAttribute( vXfer, "ER_RelType",   "ZKey",
                                    vERD,  "ER_RelType_1", "ZKey" );
         SetAttributeFromAttribute( vXfer, "ER_RelType",      "OtherEntityName",
                                    vERD,  "ER_Entity_Other", "Name" );
      }
   }
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_CreateXrefInfo( zVIEW vSubtask )
{
   // This operation creates cross reference information from components
   // in the ER. It first creates ER_Entity and ER_Attribute information
   // from the ERD. It then reads all LODs and then all Dialogs to create a
   // cross reference listing (object) of all LODs, RegisteredViews, Dialogs
   // and Controls.
   // For performance reasons, it does not use ActivateMetaOI, but reads
   // each file directly to avoid relink time and filling up memory with
   // cached LODs.

   zVIEW  vTaskLPLR;
   zVIEW  vXfer;
   zVIEW  vERD;
   zVIEW  vTZEREMDO;

   // If the cross reference object exists, just use it (ie., don't continue).
   if ( GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vXfer, "ER_Entity" ) >= zCURSOR_SET )
   {
      return( 0 );
   }

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   ActivateEmptyObjectInstance( &vXfer, "TZBRLOVO", vTaskLPLR, zMULTIPLE );
   CreateEntity( vXfer, "Root", zPOS_AFTER );
   SetNameForView( vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaType", "Type",
                                  zREFER_LOD_META, 0 );

   // Loop through all the ER_Entities, creating ER_Entity entries and
   // ER_Attribute entries within them.
   zwfnTZEREMDD_CreateEntity_RS( vXfer, vERD );

   // Process all LODs.
   LOD_DialogListSub( vSubtask, "LOD", vTaskLPLR, vXfer, vERD );

   // Order the LODs by Name.
   OrderEntityForView( vXfer, "LOD", "Name A" );

   // Process all Registered Views.
   LOD_DialogListSub( vSubtask, "PVR", vTaskLPLR, vXfer, vERD );

   // Process all Dialogs.
   LOD_DialogListSub( vSubtask, "PWD", vTaskLPLR, vXfer, vERD );

   // At the end, position on the first entry.
   SetCursorFirstEntity( vXfer, "LOD", 0 );
   SetCursorFirstEntity( vXfer, "ER_Entity", 0 );
   DropView( vERD );

   return( 0 );
}

static void
zwfnTZEREMDD_SetIdentifierEntity( zVIEW vXfer, zVIEW vERD )
{
   zCHAR  szFactTypeName[ 66 ];
   zCHAR  szOtherName[ 33 ];

   // Set the attribute Identifier.FactTypeName from either the
   // ER_AttributeIdentifier or the ER_ReLinkIdentifier and
   // ER_EntityOtherIdentifier entries.
   if ( CheckExistenceOfEntity( vERD, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
   {
      SetAttributeFromAttribute( vXfer, "IdentifierEntity",       "FactTypeName",
                                 vERD,  "ER_AttributeIdentifier", "Name" );
      SetAttributeFromString( vXfer, "IdentifierEntity", "Type",
                              "Attr" );
   }

   if ( CheckExistenceOfEntity( vERD, "ER_Entity_Other_Identifier" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szFactTypeName, sizeof( szFactTypeName ),
                              vERD, "ER_RelLinkIdentifier", "Name" );
      GetStringFromAttribute( szOtherName, sizeof( szOtherName ),
                              vERD, "ER_Entity_Other_Identifier", "Name" );
      strcat_s( szFactTypeName, sizeof( szFactTypeName ), " " );
      strcat_s( szFactTypeName, sizeof( szFactTypeName ), szOtherName );
      SetAttributeFromString( vXfer, "IdentifierEntity", "FactTypeName",
                              szFactTypeName );
      SetAttributeFromString( vXfer, "IdentifierEntity", "Type",
                              "Rel" );
   }

   // If this is System Maintained, set IdentifierEntity.Type to "Sys"
   // for what should be the only ER_FactType.
   if ( CompareAttributeToString( vERD, "ER_EntIdentifier", "SystemMaintained",
                                 "Y" ) == 0 )
   {
      SetAttributeFromString( vXfer, "IdentifierEntity", "Type",
                              "Sys" );
   }
}

static void
zwfnTZEREMDD_CreateEntityIdentifier( zVIEW vXfer, zVIEW vERD )
{
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vERD, "ER_FactType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      SetAttributeFromAttribute( vXfer, "IdentifierEntity", "EntityName",
                                 vERD,  "ER_Entity",        "Name" );
      SetAttributeFromAttribute( vXfer, "IdentifierEntity", "IdentifierName",
                                 vERD,  "ER_EntIdentifier", "Name" );

      zwfnTZEREMDD_SetIdentifierEntity( vXfer, vERD );

      nRC = SetCursorNextEntity( vERD, "ER_FactType", 0 );

      // Add entry for next ER_FactType or for blank line.
      if ( nRC >= zCURSOR_SET )
         CreateEntity( vXfer, "IdentifierEntity", zPOS_AFTER );
   }

   // If this is System Maintained without an ER_FactType, set the
   // IdentifierEntity.Type entry to "Sys".
   if ( CompareAttributeToString( vERD, "ER_EntIdentifier", "SystemMaintained",
                                  "Y" ) == 0 &&
        CheckExistenceOfEntity( vERD, "ER_FactType" ) < zCURSOR_SET )
   {
      SetAttributeFromString( vXfer, "IdentifierEntity", "Type",
                              "Sys" );
   }

   // If there was no FactType entry, flag the Identifier entry
   // accordingly (unless the Identifier is System Maintained).
   if ( CompareAttributeToString( vERD, "ER_EntIdentifier", "SystemMaintained",
                                  "Y" ) != 0 &&
        CheckExistenceOfEntity( vERD, "ER_FactType" ) < zCURSOR_SET )
   {
      SetAttributeFromString( vXfer, "IdentifierEntity", "FactTypeName",
                              "*** Missing Identifier Values ***" );
   }
}


zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_EntityIdentifierList( zVIEW vSubtask )
{
   // This operation creates cross reference information

   zVIEW  vTaskLPLR;
   zVIEW  vXfer;
   zVIEW  vTZEREMDO;
   zVIEW  vERD;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   if ( GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // If the cross reference object exists, use it, but delete existing
      // entries.
      for ( nRC = SetCursorFirstEntity( vXfer, "IdentifierEntity", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "IdentifierEntity", 0 ) )
         DeleteEntity( vXfer, "IdentifierEntity", zREPOS_NONE );
   }
   else
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      ActivateEmptyObjectInstance( &vXfer, "TZBRLOVO", vTaskLPLR, zMULTIPLE );
      CreateEntity( vXfer, "Root", zPOS_AFTER );
      SetNameForView( vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );
   }

   // Loop through all the ER_Entities, creating IdentifierEntity entries and
   // setting the work attribute values for the children of ER_EntIdentifier.
   // Create an IdentifierEntity entry for each subentity and for desirable
   // blank lines.

   for ( nRC = SetCursorFirstEntity( vERD, "ER_Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Entity", 0 ) )
   {
      CreateEntity( vXfer, "IdentifierEntity", zPOS_AFTER );
      SetAttributeFromAttribute( vXfer, "IdentifierEntity", "EntityName",
                                 vERD,  "ER_Entity",        "Name" );

      // Create the other IdentitiferEntity values.
      for ( nRC = SetCursorFirstEntity( vERD, "ER_EntIdentifier", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vERD, "ER_EntIdentifier", 0 ) )
      {
         zwfnTZEREMDD_CreateEntityIdentifier( vXfer, vERD );
      }

      // If an ER_EntIdentifier entity doesn't exist, flag the Identifier
      // entry and create the additional blank entry.
      if ( CheckExistenceOfEntity( vERD, "ER_EntIdentifier" ) < zCURSOR_SET )
      {
         SetAttributeFromString( vXfer, "IdentifierEntity", "IdentifierName",
                                 "*** Missing Identifier ***" );
      }
   }

   // At the end, position on the first entry.
   SetCursorFirstEntity( vXfer, "IdentifierEntity", 0 );
   DropView( vERD );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListBuild( zVIEW vSubtask )
{
   // This operation creates cross reference information of Attribute Name
   // to Entity Name.

   zVIEW  vTaskLPLR;
   zVIEW  vXfer;
   zVIEW  vERD;
   zVIEW  vTZEREMDO;
   zSHORT nRC;

   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   if ( GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // If the cross reference object exists, use it, but delete existing
      // entries.
      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      {
         DeleteEntity( vXfer, "AttributeList", zREPOS_NONE );
      }

      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeListCopy", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeListCopy", 0 ) )
      {
         DeleteEntity( vXfer, "AttributeListCopy", zREPOS_NONE );
      }
   }
   else
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      ActivateEmptyObjectInstance( &vXfer, "TZBRLOVO", vTaskLPLR, zMULTIPLE );
      CreateEntity( vXfer, "Root", zPOS_AFTER );
      SetNameForView( vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );
   }

   // Loop through all the ER_Attribute entities in the data model,
   // creating AttributeList entries.

   for ( nRC = SetCursorFirstEntity( vERD, "ER_Attribute", "EntpER_Model" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_Attribute", "EntpER_Model" ) )
   {
      CreateEntity( vXfer, "AttributeList", zPOS_AFTER );
      SetAttributeFromAttribute( vXfer, "AttributeList", "EntityName",
                                 vERD,  "ER_Entity",     "Name" );
      SetAttributeFromAttribute( vXfer, "AttributeList", "AttributeName",
                                 vERD,  "ER_Attribute",  "Name" );
      SetAttributeFromAttribute( vXfer, "AttributeList", "DomainName",
                                 vERD,  "Domain",        "Name" );
      SetAttributeFromAttribute( vXfer, "AttributeList", "AttributeDesc",
                                 vERD,  "ER_Attribute",  "Desc" );
   }

   // Order the list by AttributeName.
   OrderEntityForView( vXfer, "AttributeList", "AttributeName A" );

   // At the end, position on the first entry.
   SetCursorFirstEntity( vXfer, "AttributeList", 0 );
   DropView( vERD );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListFindName( zVIEW vSubtask )
{
   // This operation finds all Attribute Names containing a particular
   // string of characters.

   zVIEW  vXfer;
   zCHAR  szFindString[ 61 ];
   zSHORT nRC;

   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   // Before doing the find, we will see if AttributeListCopy entries exist
   // for resetting AttributeList entries to their original state.
   // If they don't exist, we'll create them.
   if ( CheckExistenceOfEntity( vXfer, "AttributeListCopy" ) < zCURSOR_SET )
   {
      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      {
         CreateEntity( vXfer, "AttributeListCopy", zPOS_AFTER );
         SetMatchingAttributesByName( vXfer, "AttributeListCopy",
                                      vXfer, "AttributeList", zSET_ALL );
      }

      OrderEntityForView( vXfer, "AttributeListCopy", "AttributeName A" );
   }

   szFindString[ 0 ] = 0;
   nRC = MessagePromptForInput( vSubtask, "ER00210",
                                "E/R Model Maintainance",
                                "Enter search string.",
                                zBEEP, szFindString, 60 );
   if ( nRC == 1 && szFindString[ 0 ] != 0 )
   {
      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      {
         nRC = FindTextInAttribute( szFindString, vXfer, "AttributeList", "AttributeName" );
         if ( nRC < 0 )
         {
            // The text was not found so delete the entry.
            DeleteEntity( vXfer, "AttributeList", zREPOS_NONE );
         }
      }
   }

   SetCursorFirstEntity( vXfer, "AttributeList", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListPreBuild( zVIEW vSubtask )
{
   zVIEW  vXfer;
   zSHORT nRC;

   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXfer, "AttributeListCopy" ) < zCURSOR_SET )
   {
      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      {
         CreateEntity( vXfer, "AttributeListCopy", zPOS_AFTER );
         SetMatchingAttributesByName( vXfer, "AttributeListCopy",
                                      vXfer, "AttributeList", zSET_ALL );
      }

      OrderEntityForView( vXfer, "AttributeListCopy", "AttributeName A" );
   }

   SetCursorFirstEntity( vXfer, "AttributeList", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListFindDesc( zVIEW vSubtask )
{
   // This operation finds all Attribute Desc containing a particular
   // string of characters.

   zVIEW  vXfer;
   zCHAR  szFindString[ 61 ];
   zSHORT nRC;

   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   // Before doing the find, we will see if AttributeListCopy entries exist
   // for resetting AttributeList entries to their original state.
   // If they don't exist, we'll create them.
   if ( CheckExistenceOfEntity( vXfer, "AttributeListCopy" ) < zCURSOR_SET )
   {
      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      {
         CreateEntity( vXfer, "AttributeListCopy", zPOS_AFTER );
         SetMatchingAttributesByName( vXfer, "AttributeListCopy",
                                      vXfer, "AttributeList", zSET_ALL );
      }

      OrderEntityForView( vXfer, "AttributeListCopy", "AttributeName A" );
   }

   szFindString[ 0 ] = 0;
   nRC = MessagePromptForInput( vSubtask, "ER00210",
                                "E/R Model Maintainance",
                                "Enter search string.",
                                zBEEP, szFindString, 60 );
   if ( nRC == 1 && szFindString[ 0 ] != 0 )
   {
      for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      {
         nRC = FindTextInAttribute( szFindString, vXfer, "AttributeList", "AttributeDesc" );
         if ( nRC < 0 )
         {
            // The text was not found so delete the entry.
            DeleteEntity( vXfer, "AttributeList", zREPOS_NONE );
         }
      }
   }

   SetCursorFirstEntity( vXfer, "AttributeList", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListReset( zVIEW vSubtask )
{
   // This operation resets the Attribute List to its original state with
   // all Attributes listed.

   zVIEW  vXfer;
   zSHORT nRC;

   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vXfer, "Root", "OrderType", "A" );

   // Delete existing AttributeList entries.
   for ( nRC = SetCursorFirstEntity( vXfer, "AttributeList", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vXfer, "AttributeList", 0 ) )
      DeleteEntity( vXfer, "AttributeList", zREPOS_NONE );

   // Recreate AttributeList entries from copies.
   for ( nRC = SetCursorFirstEntity( vXfer, "AttributeListCopy", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vXfer, "AttributeListCopy", 0 ) )
   {
      CreateEntity( vXfer, "AttributeList", zPOS_AFTER );
      SetMatchingAttributesByName( vXfer, "AttributeList",
                                   vXfer, "AttributeListCopy", zSET_ALL );
   }

   SetCursorFirstEntity( vXfer, "AttributeList", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListXferAttM( zVIEW vSubtask )
{
   // This operation transfers to the Attribute Maintenance window and must
   // therefore position on the correct ER_Attribute entity.

   zVIEW  vXfer;
   zVIEW  vERD;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vXfer, "Root", "AttributeListFlag", "Y" );

   SetCursorFirstEntityByAttr( vERD,  "ER_Entity",     "Name",
                               vXfer, "AttributeList", "EntityName", 0 );
   SetCursorFirstEntityByAttr( vERD,  "ER_Attribute",  "Name",
                               vXfer, "AttributeList", "AttributeName", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListM_OK( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vXfer;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   if ( AcceptSubobject( vERD, "ER_Attribute" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Set the Attribute & Domain names back in the analysis object.
   SetAttributeFromAttribute( vXfer, "AttributeList", "AttributeName",
                              vERD,  "ER_Attribute",  "Name" );
   SetAttributeFromAttribute( vXfer, "AttributeList", "DomainName",
                              vERD,  "Domain",        "Name" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListNext( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vXfer;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   if ( AcceptSubobject( vERD, "ER_Attribute" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Set the Attribute & Domain names back in the analysis object.
   SetAttributeFromAttribute( vXfer, "AttributeList", "AttributeName",
                              vERD,  "ER_Attribute",  "Name" );
   SetAttributeFromAttribute( vXfer, "AttributeList", "DomainName",
                              vERD,  "Domain",        "Name" );

   SetCursorNextEntity( vXfer, "AttributeList", "" );
   SetCursorFirstEntityByAttr( vERD,  "ER_Entity",     "Name",
                               vXfer,      "AttributeList", "EntityName", 0 );
   SetCursorFirstEntityByAttr( vERD,  "ER_Attribute",  "Name",
                               vXfer,      "AttributeList", "AttributeName", 0 );

   CreateTemporalSubobjectVersion( vERD, "ER_Attribute" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEREMDD_AttributeListPrev( zVIEW vSubtask )
{
   zVIEW  vERD;
   zVIEW  vXfer;

   GetViewByName( &vERD, "TZEREMDO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXfer, "TZBRLOVO", vSubtask, zLEVEL_TASK );

   if ( AcceptSubobject( vERD, "ER_Attribute" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Set the Attribute & Domain names back in the analysis object.
   SetAttributeFromAttribute( vXfer, "AttributeList", "AttributeName",
                              vERD,  "ER_Attribute",  "Name" );
   SetAttributeFromAttribute( vXfer, "AttributeList", "DomainName",
                              vERD,  "Domain",        "Name" );

   SetCursorPrevEntity( vXfer, "AttributeList", "" );
   SetCursorFirstEntityByAttr( vERD,  "ER_Entity",     "Name",
                               vXfer,      "AttributeList", "EntityName", 0 );
   SetCursorFirstEntityByAttr( vERD,  "ER_Attribute",  "Name",
                               vXfer,      "AttributeList", "AttributeName", 0 );

   CreateTemporalSubobjectVersion( vERD, "ER_Attribute" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_DeselectRelationship
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZEREMDD_DeselectRelationship( zVIEW  vView,
                                 zPCHAR szEntityName,
                                 zSHORT nSelectInstance )
{
   zVIEW   vCopy;
   zSHORT  nRC;

   if ( CheckExistenceOfEntity( vView, szEntityName ) < zCURSOR_SET )
      return( -1 );

   CreateViewFromViewForTask( &vCopy, vView, 0 );

   nRC = SetCursorFirstEntity( vView, szEntityName, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      SetSelectStateOfEntity( vView, szEntityName, 0 );
      nRC = SetCursorNextEntity( vView, szEntityName, 0 );
   }

   SetCursorFirstEntityByEntityCsr( vView, szEntityName,
                                    vCopy, szEntityName, 0 );

   if ( nSelectInstance == 1 )
      SetSelectStateOfEntity( vView, szEntityName, 1 );

   DropView( vCopy );

   return( 0 );
} // zwTZEREMDD_DeselectRelationship

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZEREMDD_SA_RefreshEntityList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZEREMDD_SA_RefreshEntityList( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vTZERSASO;
   zVIEW  vTZEREMDO;

   GetViewByName( &vTZERSASO, "TZERSASO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZEREMDO, "TZEREMDO", vSubtask, zLEVEL_TASK );

   if ( !GetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 1 ))
   {
      if ( CheckExistenceOfEntity( vTZERSASO, "SA_Entity" ) < zCURSOR_SET )
         return( -1 );

      SetSelectStateOfEntityForSet( vTZERSASO, "SA_Entity", 1, 1 );
      RefreshCtrl( vSubtask, "SA_Entity" );
      zwTZEREMDD_SA_EntitySelect( vSubtask );
   }

   if ( CheckExistenceOfEntity( vTZERSASO, "ER_RelLink" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelLink",
                                         vTZERSASO, "ER_RelLink",
                                         "EntpER_Model" ) >= zCURSOR_SET )
   {
      zwTZEREMDD_DeselectRelationship( vTZEREMDO, "ER_RelLink", 0 );
      SetSelectStateOfEntity( vTZEREMDO, "ER_RelLink", 1 );
      zwTZEREMDD_SetRelationshipSelect( vSubtask );

      // If single click then refresh the listbox in window RelationShipList.
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "RLLVIEW" ) >= 0 )
         RefreshCtrl( vWindow, "ER_RelLink" );

      // If single click then refresh the listbox in window EntityList.
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ENTVIEW" ) >= 0 )
         RefreshCtrl( vWindow, "RelList" );
   }

   return( 0 );
} // zwTZEREMDD_SA_RefreshEntityList


#ifdef __cplusplus
}
#endif
