#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZSYSSVC_INCL
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

// LAST MESSAGE ID: WD00409

zCHAR szWorkMessage[ 300 ];
zCHAR sz[ 256 ];

zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_ZoomOut( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_HideShowActions( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_ShowGroupEvents( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_ERD_HighliteEntity( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_RelSelect( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_ZoomStandardSize( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_Print( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_FreeCell( zPSHORT pnXout,
                       zPSHORT pnYout,
                       zSHORT  nXcenter,
                       zSHORT  nYcenter,
                       zSHORT  nDlimit );
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_PositionNonPosEnts( zVIEW vSubtask,
                                 zVIEW vDlgGraph );
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_InitCellsEDM( zVIEW vGRW );

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_MarkCellEDM( zLONG   lXgrid,
                         zLONG   lYgrid );
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_UnmarkCellEDM( zLONG   lXgrid,
                           zLONG   lYgrid );
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_FreePosEDM( zPLONG  plXout,
                        zPLONG  plYout,
                        zLONG   lXcenter,
                        zLONG   lYcenter,
                        zSHORT  nDlimit,
                        zVIEW   vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_GetWeightedPosEDM( zPLONG  plXout,
                               zPLONG  plYout,
                               zVIEW   vGRW );

#define MAX_GRID_X 600       // GRID is the unit used by the ER control
#define MAX_GRID_Y 300       //
#define MAX_CELL_X 100       // 1 thru 59 are referenced
#define MAX_CELL_Y 100       // 1 thru 39 are referenced
#define GRIDS_PER_CELL_X  6
#define GRIDS_PER_CELL_Y  3
zCHAR  cCellUsage[ MAX_CELL_X] [ MAX_CELL_Y ];
zSHORT nCellsUsed;
zSHORT nD, nXincr, nYincr;
zSHORT nXmin, nYmin;


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZWDGRWD_GraphBuild
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_GraphBuild( zVIEW vSubtask )
{
   zVIEW vWindow;
   zVIEW vDlg;
   zVIEW vGRW;
   zVIEW vProfileXFER;
   zPCHAR szDlgName;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vDlg, vWindow, 0 );

   GetAddrForAttribute( &szDlgName, vDlg, "Dialog", "Tag" );
   SetWindowCaptionTitle( vSubtask, 0, szDlgName );

   oTZWDGRWO_LoadOrInitialize( &vGRW, vDlg );

   SetDrawOptions( vSubtask, "ERD",
                   zDRAW_LINES | zDRAW_TEXT | zDRAW_NOCARD );

// MessageSend( vSubtask, "WD00401", "Dialog",
//              "about to position diagram",
//              zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   RefreshDiagram( vSubtask, "ERD", 0 );

   zwfnTZWDGRWD_PositionNonPosEnts( vSubtask, vGRW );

   zwTZWDGRWD_WD_ZoomOut( vSubtask );

   zwTZWDGRWD_WD_HideShowActions( vSubtask );
   zwTZWDGRWD_ShowGroupEvents( vSubtask );

   oTZWDGRWO_Save( vGRW );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZWDGRWD_PositionWD
//
// PURPOSE:    Set the WD Diagram positions for all Entities in the ER.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_PositionWD( zVIEW vSubtask )
{
   zVIEW  vGRW;
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
   zmemset( &cCellUsage[ 0 ][ 0 ], 0, MAX_CELL_X * MAX_CELL_Y );

   // Get Access to WD Object
   nRC = GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( sz, zsizeof( sz ), "Unable to Access WD Model. Model must be opened." );
      MessageSend( vSubtask, "WD00402", "Dialog",
                   sz,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow,
                               "TZEREMDD", "WD_Diagram" );
      return( 0 );
   }

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
      strcpy_s( sz, zsizeof( sz ), "Unable to create TZERPOSO work OI." );
      MessageSend( vSubtask, "WD00403", "Dialog",
                   sz,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow,
                               "TZEREMDD", "WD_Diagram" );
      return( 0 );
   }

   SetNameForView( vTZERPOSO, "dbgPOS", vSubtask, zLEVEL_TASK );
   CreateEntity( vTZERPOSO, "RootStats", zPOS_FIRST );

   // Copy all the entities from the WD Model to the work object
   lEnts = 0;
   nRC = SetCursorFirstEntity( vGRW, "ER_Entity", "" );
   while ( nRC >= zCURSOR_SET )
   {
      // first, position at "cluster" for the entity -- currently based
      // on a possible Owning Area.
      GetStringFromAttribute( sz, zsizeof( sz ), vGRW, "ER_Entity", "OwningArea" );
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
                                   vGRW, "ER_Entity", zSET_ALL );
      if ( sz[ 1 ] == 1 )
      {  // reset OwningArea to default Cluster Name
         SetAttributeFromString( vTZERPOSO, "W_ERD_Entity",
                                 "OwningArea", sz );
      }

      lEnts++;
      AddToAttributeFromInteger( vTZERPOSO, "Cluster", "NbrEnts", 1 );
      nRC = SetCursorNextEntity( vGRW, "ER_Entity", "" );
   }

   SetAttributeFromInteger( vTZERPOSO, "RootStats", "NbrEnts", lEnts );

   // Set Cell size
   lCells = (zLONG) sqrt( (double) lEnts );
   if ( lCells * lCells !=  lEnts )
      lCells++;

   SetAttributeFromInteger( vTZERPOSO, "RootStats", "NbrCells", lCells );

   // Include all related entities from the WD Model to the work object
   CreateViewFromViewForTask( &vTZERPOS1, vTZERPOSO, 0 );
   nRC = SetCursorFirstEntity( vGRW, "ER_Entity", "" );
   if ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vGRW, "ER_Entity", "ZKey" );
      SetCursorFirstEntityByInteger( vTZERPOSO, "W_ERD_Entity",
                                    "ZKey", lZKey, "RootStats" );
   }

   while ( nRC >= zCURSOR_SET )
   {
      lRelEnts = 0;
      nRC = SetCursorFirstEntity( vGRW, "ER_RelLink", "" );
      while ( nRC >= zCURSOR_SET )
      {
         // clear any relationship positioning
         SetAttributeFromInteger( vGRW, "ER_RelType_1",
                                  "ER_DiagramBendPosX", 0 );
         SetAttributeFromInteger( vGRW, "ER_RelType_1",
                                  "ER_DiagramBendPosY", 0 );
         SetAttributeFromInteger( vGRW, "ER_RelType_1",
                                  "ER_DiagramE1PosX", 0 );
         SetAttributeFromInteger( vGRW, "ER_RelType_1",
                                  "ER_DiagramE1PosY", 0 );
         SetAttributeFromInteger( vGRW, "ER_RelType_1",
                                  "ER_DiagramE2PosX", 0 );
         SetAttributeFromInteger( vGRW, "ER_RelType_1",
                                  "ER_DiagramE2PosY", 0 );
         // get the ZKey of relationship for comparison and positioning
         GetIntegerFromAttribute( &lZKey,
                                  vGRW, "ER_Entity_Other", "ZKey" );
         if ( CompareAttributeToInteger( vGRW, "ER_Entity", "ZKey",
                                         lZKey ) != 0 )
         {  // only consider non-recursive relationships and
            // only include a related entity once(i.e. no multiple
            // relationships between the same 2 entities).
            if ( SetCursorFirstEntityByInteger( vTZERPOSO,
                        "W_ERD_RelatedEntity", "ZKey", lZKey, "" )
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
                  AddToAttributeFromInteger( vTZERPOSO, "Cluster",
                                             "NbrRelEntsInMyCluster", 1 );
                  AddToAttributeFromInteger( vTZERPOSO, "W_ERD_Entity",
                                             "NbrRelEntsInSameOA", 1 );
               }
               else
               {
                  AddToAttributeFromInteger( vTZERPOSO, "Cluster",
                                             "NbrRelEntsInOtherClusters", 1 );
               }
            }
         }
         nRC = SetCursorNextEntity( vGRW, "ER_RelLink", "" );
      }
      SetAttributeFromInteger( vTZERPOSO, "W_ERD_Entity", "NbrRelatedEnts",
                               lRelEnts );
      nRC = SetCursorNextEntity( vGRW, "ER_Entity", "" );
      GetIntegerFromAttribute( &lZKey, vGRW, "ER_Entity", "ZKey" );
      SetCursorFirstEntityByInteger( vTZERPOSO, "W_ERD_Entity",
                                    "ZKey", lZKey, "RootStats" );
   }

//  Sort Clusters, Entities, and Related Entities in prep for
//  the placement algorithm
   nXmax = nYmax = nPrevYmax = 0;
   nRC = zgSortEntityWithinParent( zDESCENDING, vTZERPOSO, "Cluster",
                                   "NbrEnts", "NbrRelEntsInMyCluster" );
   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "Cluster", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "Cluster", "" ) )
   {
      GetStringFromAttribute( sz, zsizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only
      zgSortEntityWithinParent( zDESCENDING, vTZERPOSO, "W_ERD_Entity", "NbrRelEntsInSameOA", "" );
   //  Set Cell size
      GetIntegerFromAttribute( &lEnts, vTZERPOSO, "Cluster", "NbrEnts" );
      lCells = (zLONG) sqrt( (double)lEnts );
      if ( lCells * lCells != lEnts )
         lCells++;
      SetAttributeFromInteger( vTZERPOSO, "Cluster", "NbrCells", lCells );
      nCells =(zSHORT)lCells;

   //  Set Cluster X,Y upper left corner by finding a "good"
   //  available cell "square" that will hold Cluster area
      if ( nXmax * 4 > nYmax * 5 && nCells < MAX_CELL_Y - nYmax )
         nY = nYmax + 1;
      else
         nY = 1;

      if ( nPrevYmax != 0 )
         if ( zmemcmp( &cCellUsage[ nXmax + 1 - nCells ] [ nPrevYmax + 1 ], cCellUsage[ 0 ], nCells ) == 0 )
            nY = nPrevYmax + 1;

      for ( nX = 1; nX <= MAX_CELL_X; nX++ )
      {
         if ( nX >= MAX_CELL_X )
         {
            nY++;
            if ( nY >= MAX_CELL_Y )
            {
               strcpy_s( sz, zsizeof( sz ), "No space available for Area:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ), vTZERPOSO, "Cluster", "Name" );
               strcat_s( sz, zsizeof( sz ), "'." );
               MessageSend( vSubtask, "WD00404", "Dialog",
                            sz,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               return( 0 );
            }
            else
               nX = 1;
         }
         for ( nXchk = nX; nXchk <= nX + nCells - 1; nXchk++ )
         {
            if ( zmemcmp( &cCellUsage[ nXchk ] [ nY ], cCellUsage[ 0 ], nCells ) == 0 )
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
   //  at this point nX, nY is the chosen position for the Cluster,
   //  so mark the Cells as used and set the Attrs in the POS object.
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

// MessageSend( vSubtask, "WD00405", "Dialog",
//              "Cluster positions set",
//              zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

//  Process all entites in Cluster setting stats for sorting
   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "Cluster", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "Cluster", "" ) )
   {
      GetStringFromAttribute( sz, zsizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only
      for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_Entity", "" ) )
      {
         for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" ) )
         {
            if ( CompareAttributeToAttribute( vTZERPOSO, "W_ERD_Entity", "OwningArea",
                                              vTZERPOSO, "W_ERD_RelatedEntity", "OwningArea" ) == 0 )
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
               if ( CompareAttributeToAttribute( vTZERPOSO, "W_ERD_Entity", "OwningArea",
                                                 vTZERPOS1, "W_ERD_RelatedEntity", "OwningArea" ) == 0 )
               {
                  AddToAttributeFromInteger( vTZERPOSO, "W_ERD_RelatedEntity", "NbrRelEntsInMyParentsOA", 1 );
               }
            } // End for each W_ERD_RelatedEntity of the related entity

         } // End for each W_ERD_RelatedEntity
         zgSortEntityWithinParent( zDESCENDING, vTZERPOSO, "W_ERD_RelatedEntity", "OA_MatchesParentsOA", "NbrRelEntsInMyParentsOA" );
      } // End for each W_ERD_Entity
   } // End for each Cluster
//
//   Do the Placement Algorithm
//
   nXmin = nYmin = 999;
   nCellsUsed = 0;
   zmemset( &cCellUsage[ 0 ][ 0 ], 0, MAX_CELL_X * MAX_CELL_Y );

   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "Cluster", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "Cluster", "" ) )
   {
      GetStringFromAttribute( sz, zsizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only

   //  Get Cell size
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
      if ( !(nCells & 0x0001) )
      {
         nXcenter--;
         nYcenter--;
      }

   //  Process all entites in Cluster
      for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_Entity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_Entity", "" ) )
      {
         GetStringFromAttribute( sz + 50, zsizeof( sz ) - 50, //debug only
                                 vTZERPOSO, "W_ERD_Entity", "Name" );
         if ( CompareAttributeToInteger( vTZERPOSO, "W_ERD_Entity",
                                         "ER_DgmPosX", 0 ) != 0 )
         {
            // get position
            GetIntegerFromAttribute( &lXcell, vTZERPOSO, "W_ERD_Entity", "ER_DgmPosX" );
            GetIntegerFromAttribute( &lYcell, vTZERPOSO, "W_ERD_Entity", "ER_DgmPosY" );
         }
         else
         {
            // find free cell
            nD = nXincr = nYincr = 0;
            if ( zwfnTZWDGRWD_FreeCell( &nXchk, &nYchk, nXcenter, nYcenter, nCells ) < 0 )
            {
               strcpy_s( sz, zsizeof( sz ), "No cell available for Area:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ),
                                       vTZERPOSO, "Cluster", "Name" );
               strcat_s( sz, zsizeof( sz ), "'\nEntity:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ),
                                       vTZERPOSO, "W_ERD_Entity", "Name" );
               strcat_s( sz, zsizeof( sz ), "'." );
               MessageSend( vSubtask, "WD00406", "Dialog",
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

         nX =(zSHORT)lXcell;
         nY =(zSHORT)lYcell;
         nD = nXincr = nYincr = 0;
   //  Process all related entites within the same cluster
         for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_RelatedEntity", "" ) )
         {
            GetStringFromAttribute( sz + 100, zsizeof( sz ) - 100, //debug only
                                    vTZERPOSO, "W_ERD_RelatedEntity", "Name" );
            if ( CompareAttributeToString( vTZERPOSO, "W_ERD_RelatedEntity", "OA_MatchesParentsOA", "Y" ) != 0 )
               continue;
            if ( CompareAttributeToInteger( vTZERPOSO, "W_ERD_RelatedEntity", "ER_DgmPosX", 0 ) != 0 )
               continue;

            // find next free cell
            if ( zwfnTZWDGRWD_FreeCell( &nXchk, &nYchk, nX, nY, nCells ) < 0 )
            {
               strcpy_s( sz, zsizeof( sz ), "No cell available for Area:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ),
                                       vTZERPOSO, "Cluster", "Name" );
               strcat_s( sz, zsizeof( sz ), "'\nEntity:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ),
                                       vTZERPOSO, "W_ERD_Entity", "Name" );
               strcat_s( sz, zsizeof( sz ), "'\nRelated Entity:\n\t'" );
               GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ),
                                       vTZERPOSO, "W_ERD_RelatedEntity", "Name" );
               strcat_s( sz, zsizeof( sz ), "'." );
               MessageSend( vSubtask, "WD00407", "Dialog",
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

//    MessageSend( vSubtask, "WD00408", "Dialog",
//                 "Entity positions set for Cluster.",
//                 zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   } // End for each Cluster

// DisplayObjectInstance( vTZERPOSO, "RootStats", 0 );  // debug only

//
//   Set the Placement values into the ER
//
   nXmin--;
   nYmin--;
   for ( nRC = SetCursorFirstEntity( vTZERPOSO, "W_ERD_Entity", "RootStats" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERPOSO, "W_ERD_Entity", "RootStats" ) )
   {
      GetStringFromAttribute( sz, zsizeof( sz ), vTZERPOSO, "Cluster", "Name" ); //debug only
      GetStringFromAttribute( sz + 50, zsizeof( sz ) - 50, //debug only
                              vTZERPOSO, "W_ERD_Entity", "Name" );
      // first, retrieve the cell position, adjust for white space at the
      // top and left, then calc a stagger offset
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

      // now, position on WD Entity and set position
      GetIntegerFromAttribute( &lZKey, vTZERPOSO, "W_ERD_Entity", "ZKey" );
      SetCursorFirstEntityByInteger( vGRW, "ER_Entity", "ZKey", lZKey, "" );
      SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosX", (lXcell * 30) + nStaggerX - 15 );
      SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosY", (lYcell * 20) + nStaggerY - 10 );
   }

   RefreshDiagram( vSubtask, "ERD", 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_FreeCell( zPSHORT pnXout,
                       zPSHORT pnYout,
                       zSHORT  nXcenter,
                       zSHORT  nYcenter,
                       zSHORT  nDlimit )
{

   for (;; )
   {
      if ( (zSHORT)sqrt( nXincr * nXincr + nYincr * nYincr ) <= nD )
      {
         *pnXout = nXcenter + nXincr;
         *pnYout = nYcenter + nYincr;
         if ( *pnXout > 0 && *pnXout < MAX_CELL_X &&
            *pnYout > 0 && *pnYout < MAX_CELL_Y )
         {
            if ( cCellUsage[ *pnXout ][ *pnYout] == 0 )
               break;
         }
      }
      // increment nYincr, nXincr, or nD as appropriate
      if ( nXincr > -nD )
         nXincr--;
      else
         if ( nYincr > -nD )
         {
            nYincr--;
            nXincr = nD;
         }
         else
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

   nCellsUsed++;
   cCellUsage[ *pnXout ][ *pnYout ] =(zCHAR)nCellsUsed;
   if ( nXmin > *pnXout ) nXmin = *pnXout;
   if ( nYmin > *pnYout ) nYmin = *pnYout;

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_Actions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZWDGRWD_WD_Actions( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "WD_Popup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_Focus
//
// PURPOSE:   Give DIL for ERD Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZWDGRWD_WD_Focus( zVIEW vSubtask )
{
   zVIEW  vGRW;
   zSHORT nRC;
   zPCHAR szName;

   nRC = GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetAddrForAttribute( &szName, vGRW, "ER_Entity", "Desc" );
      MB_SetMessage( vSubtask, 1, szName );
   }

// MB_SetMessage( vSubtask, 1, "Double Click -> OPTIONS" );
// MB_SetMessage( vSubtask, 2, "Right: Click -> ACTIONS; "
//                             "Double Click -> Zoom to Fit" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_NodeActions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZWDGRWD_WD_NodeActions( zVIEW vSubtask )
{
   zVIEW  vGRW;
   zPCHAR szName;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &szName, vGRW, "ER_Entity", "Desc" );
   MB_SetMessage( vSubtask, 1, szName );
   zwTZWDGRWD_ERD_HighliteEntity( vSubtask );

   CreateTrackingPopupMenu( vSubtask, "NodePopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_NodeCreate
//
// PURPOSE:   Add Entity to WD Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_NodeCreate( zVIEW vSubtask )
{
   zVIEW vGRW;

//  Get access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

// zwfnTZWDGRWD_AddEntity( vSubtask, vGRW );
// AcceptEntity( vSubtask, "ERD" );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_NodeNeighbors
//
// PURPOSE:   Zoom in on the WD Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_NodeNeighbors( zVIEW vSubtask )
{
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );

   SelectEntityNeighbors( vW, "ERD", 1, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_NodeSelect
//
// PURPOSE:   Select Entity in WD Model via Diagram( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_NodeSelect( zVIEW vSubtask )
{
   zVIEW vGRW;
   zPCHAR szName;

//  Get Access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &szName, vGRW, "ER_Entity", "Desc" );
   MB_SetMessage( vSubtask, 1, szName );

   zwTZWDGRWD_ERD_HighliteEntity( vSubtask );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_LClick
//
// PURPOSE:   Select Entity in WD Model via Diagram( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_LClick( zVIEW vSubtask )
{
   zVIEW vGRW;
// zPCHAR szName;

//  Get Access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
// If not selected, entity was toggled off. reset cursor to next selected
// entity.

   if ( GetSelectStateOfEntityForSet( vGRW, "ER_Entity", 1 ) == 0 )
   {
      SetSelectStateOfEntityForSet( vGRW, "ER_Entity", 0,2 );

      SetSelectSetForView( vGRW, 1 );
      SetCursorFirstSelectedEntity( vGRW, "ER_Entity", 0 );
      SetSelectStateOfEntityForSet( vGRW, "ER_Entity", 0,2 );

      SetSelectSetForView( vGRW, 1 );
   }
   zwTZWDGRWD_WD_NodeSelect( vSubtask );


   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_Print
//
// PURPOSE:   Print the WD Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_Print( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vW;
   zLONG    lPct;
   zLONG    lOrient;
   zLONG    lAlgorithm;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct,
                            vProfileXFER, "EMD", "ERD_PrintPercent" );
   GetIntegerFromAttribute( &lOrient,
                            vProfileXFER, "EMD", "ERD_PrintOrientation" );
   GetIntegerFromAttribute( &lAlgorithm,
                            vProfileXFER, "EMD", "ERD_PrintAlgorithm" );

   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );
   PrintDiagram( vW, "ERD",(zSHORT)lPct,
                (zSHORT)(lOrient | lAlgorithm) );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_ArcActions
//
// PURPOSE:   Show Popup Menu for ERD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT int OPERATION
zwTZWDGRWD_WD_ArcActions( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   zwTZWDGRWD_WD_RelSelect( vSubtask );   // Set DIL

   CreateTrackingPopupMenu( vSubtask, "ArcPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_RelCreate
//
// PURPOSE:   Add Relationship to WD Model via Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_RelCreate( zVIEW vSubtask )
{
#if 0
   zVIEW vGRW;

//  Get Access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   if ( !vGRW || MiGetUpdateForView( vGRW ) == 0 )
   {
      MessageSend( vSubtask, "WD00409", "Dialog",
                   "Add of relationship is only supported if the Model is updateable.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      RemoveRelationship( vSubtask, "ERD", 1, 0 );
   }
   else
   {
      zwTZWDGRWD_AddRelLink( vSubtask );
      AcceptRelationship( vSubtask, "ERD" );
      zwTZWDGRWD_WD_RelSelect( vSubtask );
   }

#endif
   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_RelDelete
//
// PURPOSE:   Delete Relationship WD Model via Diagram( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_RelDelete( zVIEW vSubtask )
{
#if 0
   zVIEW  vGRW;
   zCHAR  szMessage[ 256 ];
   zPCHAR sz;
   zSHORT nRC;

//  Get Access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByEntityCsr( vGRW, "ER_Entity",
                                    vGRW, "ER_Entity_2",
                                    "" );
   SetCursorFirstEntityByEntityCsr( vGRW, "ER_RelLink",
                                    vGRW, "ER_RelLink_2",
                                    "" );
// Prompt to confirm Delete
   szMessage[ 0 ] = 0;
   GetAddrForAttribute( &sz, vGRW, "ER_Entity", "Name" );
   strcat_s( szMessage, zsizeof( szMessage ), sz );
   strcat_s( szMessage, zsizeof( szMessage ), " " );
   GetAddrForAttribute( &sz, vGRW, "ER_RelLink", "Name" );
   strcat_s( szMessage, zsizeof( szMessage ), sz );
   strcat_s( szMessage, zsizeof( szMessage ), " " );
   GetAddrForAttribute( &sz, vGRW, "ER_Entity_Other", "Name" );
   strcat_s( szMessage, zsizeof( szMessage ), sz );

   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_ERD,
                     zENAME_ER_RELATIONSHIP, 0, 0, szMessage, 0 );

   if ( nRC == zRESPONSE_NO )
      return( 0 );

//  Invoke Object Operation to delete the relationship

   if ( oTZEREMDO_DeleteRelationship( vGRW, zREPOS_NEXT, 0 ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   zwTZWDGRWD_RefreshRllWnd( vSubtask );
#endif

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_RelNeighbors
//
// PURPOSE:   Show the two related Entities for a Relationship
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_RelNeighbors( zVIEW vSubtask )
{
   zVIEW  vGRW;
   zVIEW  vT;
   zVIEW  vW;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

//  Get Access to WD Object( and save it for later restore )
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vT, vGRW, 0 );

   SetCursorFirstEntity( vGRW, "ER_RelLink_2", "" );
   // position right side of WD to first Entity and Select/Refresh it.
   SetCursorFirstEntityByEntityCsr( vGRW, "ER_Entity",
                                    vGRW, "ER_Entity_2",
                                    "" );
   SetSelectStateOfEntityForSet( vGRW, "ER_Entity", 1, 1 );
// Refresh ERD in WD_Diagram
   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );
   RefreshEntity( vW, "ERD" );

   // construct first part of DIL for Rel
   GetAddrForAttribute( &szName, vGRW, "ER_Entity_2", "Desc" );
   MB_SetMessage( vW, 1, szName );
   GetAddrForAttribute( &szName, vGRW, "ER_Entity_2", "Name" );
   strcpy_s( sz, zsizeof( sz ), szName );
   strcat_s( sz, zsizeof( sz ), " [" );
   GetAddrForAttribute( &szName, vGRW, "ER_RelLink_2", "Name" );
   strcat_s( sz, zsizeof( sz ), szName );
   strcat_s( sz, zsizeof( sz ), "] " );

   SetCursorNextEntity( vGRW, "ER_RelLink_2", "" );
   // position right side of WD to second Entity and Select/Refresh it.
   SetCursorFirstEntityByEntityCsr( vGRW, "ER_Entity",
                                    vGRW, "ER_Entity_2",
                                    "" );
   SetSelectStateOfEntityForSet( vGRW, "ER_Entity", 1, 1 );
   RefreshEntity( vW, "ERD" );

   // construct secnod part of DIL for Rel
   GetAddrForAttribute( &szName, vGRW, "ER_Entity_2", "Name" );
   strcat_s( sz, zsizeof( sz ), szName );

   MB_SetMessage( vW, 2, sz );

   SetViewFromView( vGRW, vT );  // reset view to ER

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_RelSelect
//
// PURPOSE:   Select Relationship WD Model via Diagram( refresh DIL Msg )
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_RelSelect( zVIEW vSubtask )
{
   zVIEW  vGRW;
   zPCHAR szName;
   zCHAR  sz[ 128 ];

//  Get Access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vGRW, vGRW, 0 ); // protect current view

   SetCursorFirstEntityByEntityCsr( vGRW, "ER_Entity",
                                    vGRW, "ER_Entity_2",
                                    "" );
   SetCursorFirstEntityByEntityCsr( vGRW, "ER_RelLink",
                                    vGRW, "ER_RelLink_2",
                                    "" );
   GetAddrForAttribute( &szName, vGRW, "ER_Entity", "Name" );
   strcpy_s( sz, zsizeof( sz ), szName );
   strcat_s( sz, zsizeof( sz ), " [" );
   GetAddrForAttribute( &szName, vGRW, "ER_RelLink", "Name" );
   strcat_s( sz, zsizeof( sz ), szName );
   strcat_s( sz, zsizeof( sz ), "] " );
   GetAddrForAttribute( &szName, vGRW, "ER_Entity_Other", "Name" );
   strcat_s( sz, zsizeof( sz ), szName );

   MB_SetMessage( vSubtask, 2, sz );
   zwTZWDGRWD_ERD_HighliteEntity( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_ZoomStandardSize
//
// PURPOSE:   Zoom in on the WD Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_ZoomStandardSize( zVIEW vSubtask )
{
   zLONG  lPct;
   zVIEW  vW;

   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );

   lPct = 100;

   ZoomDiagram( vW, "ERD",(zSHORT)lPct, 1 );
   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_ZoomIn
//
// PURPOSE:   Zoom in on the WD Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_ZoomIn( zVIEW vSubtask )
{
   zLONG    lPct;
   zVIEW    vProfileXFER;
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct, vProfileXFER, "EMD", "ERD_ZoomInPercent" );

   ZoomDiagram( vW, "ERD",(zSHORT)lPct, 2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_ZoomOut
//
// PURPOSE:   Zoom in on the WD Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_ZoomOut( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zLONG    lPct;
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lPct,
                            vProfileXFER, "EMD", "ERD_ZoomOutPercent" );
   if ( lPct > 0 )
      lPct = -lPct;

   ZoomDiagram( vW, "ERD",(zSHORT) lPct, 2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZWDGRWD_WD_ZoomOutFit
//
// PURPOSE:   Zoom in on the WD Model Diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_ZoomOutFit( zVIEW vSubtask )
{
   zVIEW vW;

   GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" );
   ZoomDiagram( vW, "ERD", 0, 3 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_WD_HideShowActions( zVIEW vSubtask )
{
   zVIEW vGRW;
   zLONG  lPosX;
   zLONG  lZKey;
   zSHORT nRC;

   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstEntityByString( vGRW, "ER_Entity", "Purpose", "W", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByString( vGRW, "ER_Entity", "Purpose", "W", "" ) )
   {

      GetIntegerFromAttribute( &lPosX, vGRW, "ER_Entity", "ER_DiagramPosX" );
      if ( lPosX > 0 )
      {       // save value and set to 0 to hide it
         SetAttributeFromInteger( vGRW, "ER_Entity", "WD_HiddenPosX", lPosX );
         SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosX", 0 );
         RemoveEntity( vSubtask, "ERD", 0, 0 );
         for ( nRC = SetCursorFirstEntity( vGRW, "ER_RelLink", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vGRW, "ER_RelLink", "" ) )
         {
            GetIntegerFromAttribute( &lZKey, vGRW, "ER_RelType_1", "ZKey" );
            RemoveRelationship( vSubtask, "ERD", 0, lZKey );
         }
      }
      else
      {       // reset value
         SetAttributeFromAttribute( vGRW, "ER_Entity", "ER_DiagramPosX",
                                    vGRW, "ER_Entity", "WD_HiddenPosX" );
         SetAttributeFromInteger( vGRW, "ER_Entity", "WD_HiddenPosX", 0 );
         RefreshEntity( vSubtask, "ERD" );
         for ( nRC = SetCursorFirstEntity( vGRW, "ER_RelLink", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vGRW, "ER_RelLink", "" ) )
         {
            GetIntegerFromAttribute( &lZKey, vGRW, "ER_RelType_1", "ZKey" );
            SetCursorNextEntityByInteger( vGRW, "ER_RelType", "ZKey", lZKey, "" );
            RefreshRelationship( vSubtask, "ERD", 0 );
         }
      }
   }  // End for each ER_Entity

// RefreshDiagram( vSubtask, "ERD", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_HideShowGroupEvents( zVIEW vSubtask,
                                  zVIEW vGRW,
                                  zBOOL bHide )
{
zLONG  lZKey;
zSHORT nRC;
zCHAR  szArcType[ 2 ];

   for ( nRC = SetCursorFirstEntityByString( vGRW, "ER_Entity", "Type", "A", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByString( vGRW, "ER_Entity", "Type", "A", "" ) )
   {

      nRC = SetCursorFirstEntityByString( vGRW, "ER_RelType_1", "Type", "G", "ER_Entity" );
      if ( nRC >= zCURSOR_SET )
      {
         for ( nRC = SetCursorFirstEntity( vGRW, "ER_RelType_1", "ER_Entity" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vGRW, "ER_RelType_1", "ER_Entity" ) )
         {
            GetStringFromAttribute( szArcType, zsizeof( szArcType ), vGRW, "ER_RelType_1", "Type" );
            if ( szArcType[ 0 ] == 'G' )
            {
               GetIntegerFromAttribute( &lZKey, vGRW, "ER_RelType_1", "ZKey" );
               SetCursorFirstEntityByInteger( vGRW, "ER_RelType", "ZKey", lZKey, "" );
               if ( bHide )
               {
                  RemoveRelationship( vSubtask, "ERD", 0, 0 );
               }
               else
               {
                  RefreshRelationship( vSubtask, "ERD", 0 );
               }
            }
            else
            if ( szArcType[ 0 ] == 'E' )
            {
               GetIntegerFromAttribute( &lZKey, vGRW, "ER_RelType_1", "ZKey" );
               SetCursorFirstEntityByInteger( vGRW, "ER_RelType", "ZKey", lZKey, "" );
               if ( bHide )
               {
                  RefreshRelationship( vSubtask, "ERD", 0 );
               }
               else
               {
                  RemoveRelationship( vSubtask, "ERD", 0, 0 );
               }
            }
         }
         RefreshEntity( vSubtask, "ERD" );
      }
   }  // End for each ER_Entity

   return( 0 );

}
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_HideGroupEvents( zVIEW vSubtask )
{
zVIEW vGRW;

   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   zwfnTZWDGRWD_HideShowGroupEvents( vSubtask, vGRW, 1 );

   return( 0 );

}
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_ShowGroupEvents( zVIEW vSubtask )
{
zVIEW vGRW;

   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   zwfnTZWDGRWD_HideShowGroupEvents( vSubtask, vGRW, 0 );

   return( 0 );

}
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_TraceReport( zVIEW vSubtask )
{
zVIEW vGRW;
zSHORT nRC;
zPCHAR szDesc;
//zPCHAR szNodeType;
//zPCHAR szArcType;

   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstEntity( vGRW, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vGRW, "ER_Entity", "" ) )
   {
      GetAddrForAttribute( &szDesc, vGRW, "ER_Entity", "Desc" );
      TraceLineS( "", "" );
      TraceLineS( "", "" );
      TraceLineS( "======= ", szDesc );
      DisplayEntityInstance( vGRW, "ER_Entity" );
      for ( nRC = SetCursorFirstEntity( vGRW, "ER_RelLink", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vGRW, "ER_RelLink", "" ) )
      {
         GetAddrForAttribute( &szDesc, vGRW, "ER_RelType_1", "Fact" );
         TraceLineS( "------- ", szDesc );
         GetAddrForAttribute( &szDesc, vGRW, "ER_RelLink", "Name" );
         TraceLineS( "        ", szDesc );
         GetAddrForAttribute( &szDesc, vGRW, "ER_Entity_Other", "Name" );
         TraceLineS( "     to ", szDesc );
         GetAddrForAttribute( &szDesc, vGRW, "ER_RelLink_Other", "Name" );
         TraceLineS( "        ", szDesc );
      }  // End for each ER_Entity
   }  // End for each ER_Entity

   return( 0 );

}
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_SaveGraph( zVIEW vSubtask )
{
zVIEW vGRW;

   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   oTZWDGRWO_Save( vGRW );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwfnTZWDGRWD_PositionNonPosEnts
//
// PURPOSE:    This Entry positions any non positioned Entities for the
//             WD Diagram in the WD Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_PositionNonPosEnts( zVIEW vSubtask,
                                 zVIEW vGRW )
{
   zLONG  lXgrid, lYgrid;
   zSHORT nRC, nEnts;
   zPCHAR szName;

   nEnts = zwfnTZWDGRWD_InitCellsEDM( vGRW );
   if ( nCellsUsed == 0 )
   {
      zwTZWDGRWD_PositionWD( vSubtask );
      return( 0 );
   }

   if ( nEnts != nCellsUsed )
   {
   //  Process WD Entities, setting any 0 positions
      for ( nRC = SetEntityCursor( vGRW, "ER_Entity", "ER_DiagramPosX",
                                   zPOS_FIRST | zQUAL_ATTR_NULL,
                                   0, 0, 0, 0, 0, 0 );
            nRC >= zCURSOR_SET;
            nRC = SetEntityCursor( vGRW, "ER_Entity", "ER_DiagramPosX",
                                   zPOS_NEXT | zQUAL_ATTR_NULL,
                                   0, 0, 0, 0, 0, 0 ) )
      {
         GetAddrForAttribute( &szName, vGRW, "ER_Entity", "Name" ); // debug
         //  Set non-positioned WD Entities
         zwfnTZWDGRWD_GetWeightedPosEDM( &lXgrid, &lYgrid, vGRW );
         zwfnTZWDGRWD_FreePosEDM( &lXgrid, &lYgrid, lXgrid, lYgrid, MAX_CELL_Y, vSubtask );
         SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosX", lXgrid );
         SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosY", lYgrid );
         zwfnTZWDGRWD_MarkCellEDM( lXgrid, lYgrid );
         RefreshEntity( vSubtask, "ERD" );
//       RefreshDiagram( vSubtask, "ERD", 0 );
      } // End for each ER_Entity
   }

   RefreshDiagram( vSubtask, "ERD", 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_InitCellsEDM( zVIEW vGRW )
{
   zVIEW vGRW1;
   zSHORT nRC;
   zSHORT nEnts;
   zLONG  lXgrid, lYgrid;
// zCHAR sz[ 256 ];

   nEnts = 0;
   nCellsUsed = 0;
   zmemset( &cCellUsage[ 0 ][ 0 ], 0, MAX_CELL_X * MAX_CELL_Y );

   CreateViewFromViewForTask( &vGRW1, vGRW, 0 );

   for ( nRC = SetCursorFirstEntity( vGRW1, "ER_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vGRW1, "ER_Entity", "" ) )
   {
//    GetStringFromAttribute( sz, zsizeof( sz ), vGRW1, "ER_Entity", "Name" ); //debug only
      nEnts++;
      GetIntegerFromAttribute( &lXgrid, vGRW1, "ER_Entity", "ER_DiagramPosX" );
      if ( lXgrid != 0 )
      {
         GetIntegerFromAttribute( &lYgrid, vGRW1, "ER_Entity", "ER_DiagramPosY" );
         zwfnTZWDGRWD_MarkCellEDM( lXgrid, lYgrid );
      }
   }  // End for each ER_Entity

   return( nEnts );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_MarkCellEDM( zLONG   lXgrid,
                          zLONG   lYgrid )
{

   zSHORT nX, nY;

   if ( lXgrid != 0 )
   {
      nX =(zSHORT)lXgrid / GRIDS_PER_CELL_X;
      nY =(zSHORT)lYgrid / GRIDS_PER_CELL_Y;
      cCellUsage[ nX ][ nY ] = 1;
      nCellsUsed++;
      return( 1 );
   }
   else
      return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_UnmarkCellEDM( zLONG   lXgrid,
                            zLONG   lYgrid )
{

   zSHORT nX, nY;

   if ( lXgrid != 0 )
   {
      nX =(zSHORT)lXgrid / GRIDS_PER_CELL_X;
      nY =(zSHORT)lYgrid / GRIDS_PER_CELL_Y;
      cCellUsage[ nX ][ nY ] = 0;
      if ( nCellsUsed > 0 )
         nCellsUsed--;

      return( 1 );
   }
   else
      return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_FreePosEDM( zPLONG  plXout,
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
   nXcenter =(zSHORT)lXcenter / GRIDS_PER_CELL_X;
   nYcenter =(zSHORT)lYcenter / GRIDS_PER_CELL_Y;

   lX = lY = 0;
   if ( ValidNewEntityPosition( vSubtask, "ERD", lX, lY ) >= 0 )
   for (;; ) // loop until free cell located
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
               lX = nX * GRIDS_PER_CELL_X;
               lY = nY * GRIDS_PER_CELL_Y;
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
         if ( nYincr > -nD )
         {
            nYincr--;
            nXincr = nD;
         }
         else
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

   *plXout = lX;
   *plYout = lY;

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_GetWeightedPosEDM( zPLONG  plXout,
                               zPLONG  plYout,
                               zVIEW   vGRW )
{
   zVIEW vGRW1;
   zVIEW vGRW2;
   zSHORT nRC, nX, nY, nRels;
   zLONG  lXgrid, lYgrid;
   zCHAR sz[ 256 ];

   nX = nY = nRels = 0;

   CreateViewFromViewForTask( &vGRW1, vGRW, 0 );
   CreateViewFromViewForTask( &vGRW2, vGRW, 0 );

   for ( nRC = SetCursorFirstEntity( vGRW1, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vGRW1, "ER_RelLink", "" ) )
   {

      GetStringFromAttribute( sz, zsizeof( sz ), vGRW1, "ER_Entity", "Name" ); //debug only
      GetStringFromAttribute( sz + 50, zsizeof( sz ) - 50, vGRW1, "ER_Entity_Other", "Name" ); //debug only
      if ( SetCursorFirstEntityByEntityCsr( vGRW2, "ER_Entity", vGRW1, "ER_Entity_Other", "" ) >= zCURSOR_SET )
      {
         nRels++;
         GetIntegerFromAttribute( &lXgrid, vGRW2, "ER_Entity", "ER_DiagramPosX" );
         GetIntegerFromAttribute( &lYgrid, vGRW2, "ER_Entity", "ER_DiagramPosY" );
         nX +=(zSHORT)lXgrid;
         nY +=(zSHORT)lYgrid;
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

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwfnTZWDGRWD_RePositionEntity
//
// PURPOSE:    This Entry positions any non positioned Entities for the
//             WD Diagram in the WD Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwfnTZWDGRWD_RePositionEntity( zVIEW vSubtask,
                               zVIEW vGRW_In )
{
   zVIEW  vGRW;
// zVIEW  vProfileXFER;
   zVIEW  vWindow;
   zSHORT nRC;

   // Perserve current View
   CreateViewFromViewForTask( &vGRW, vGRW_In, 0 );

   // Clear all current postioning information for Entity
   SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosX", 0 );
   SetAttributeFromInteger( vGRW, "ER_Entity", "ER_DiagramPosY", 0 );

   for ( nRC = SetCursorFirstEntity( vGRW, "ER_RelLink", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vGRW, "ER_RelLink", "" ) )
   {
      // Clear any relationship positioning.
      SetAttributeFromInteger( vGRW, "ER_RelType_1", "ER_DiagramBendPosX", 0 );
      SetAttributeFromInteger( vGRW, "ER_RelType_1", "ER_DiagramBendPosY", 0 );
      SetAttributeFromInteger( vGRW, "ER_RelType_1", "ER_DiagramE1PosX", 0 );
      SetAttributeFromInteger( vGRW, "ER_RelType_1", "ER_DiagramE1PosY", 0 );
      SetAttributeFromInteger( vGRW, "ER_RelType_1", "ER_DiagramE2PosX", 0 );
      SetAttributeFromInteger( vGRW, "ER_RelType_1", "ER_DiagramE2PosY", 0 );
   }  // END for each ER_RelLink

   // Now simply position the non positioned Entity if update allowed
   if ( MiGetUpdateForView( vGRW ) == 1 )
   {
      zwfnTZWDGRWD_PositionNonPosEnts( vSubtask, vGRW );
   }

//  See if ERD needs refreshing
// GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
// if ( CompareAttributeToString( vProfileXFER, "EMD", "WD_View", "Y" )
//      == 0 )
// {
      GetViewByName( &vWindow, "WD_Diagram", vSubtask, zLEVEL_TASK );
      if ( vWindow )
      {  // refresh the entity and each relationship
         RefreshEntity( vWindow, "ERD" );
         for ( nRC = SetCursorFirstEntity( vGRW, "ER_RelLink", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vGRW, "ER_RelLink", "" ) )
         {
            // Position the right side of the WD Object
            SetCursorFirstEntityByEntityCsr( vGRW, "ER_RelType", vGRW, "ER_RelType_1", "" );
            RefreshRelationship( vWindow, "ERD", 0 );
         }  // END for each ER_RelLink
      }
// }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZWDGRWD_SelectPostBuild
//
// PURPOSE:    This Entry sets the Graying for the Add Buttons.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_SelectPostBuild( zVIEW vSubtask )
{
   zVIEW vGRW;
   zVIEW vList;

//  Get Access to WD Object
   GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );

   if ( vGRW )
      if ( MiGetUpdateForView( vGRW ) == 0 )
         SetCtrlState( vSubtask, "Add", zCONTROL_STATUS_ENABLED, 0 );
      else
         SetCtrlState( vSubtask, "Add", zCONTROL_STATUS_ENABLED, 1 );

   GetViewByName( &vList, "TZERENTO_List", vSubtask, zLEVEL_TASK );
// if ( SetCursorFirstEntity( vList, "ER_Entity", "" ) < zCURSOR_SET )
// {
//    SetCtrlState( vSubtask, "Select", zCONTROL_STATUS_ENABLED, 0 );
// }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZWDGRWD_ERD_HighliteEntity
//
// PURPOSE:    This Entry gets a view to the main window,
//             selects the current entity and centers it, if it is not
//             visible.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDGRWD_ERD_HighliteEntity( zVIEW vSubtask )
{
zVIEW vW;
zVIEW vGRW;
zPCHAR szName;

   if ( GetSubtaskForWindowName( vSubtask, &vW, "WD_Diagram" ) >= 0 )
   {
      GetViewByName( &vGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
      GetAddrForAttribute( &szName, vGRW, "ER_Entity", "Desc" );
      MB_SetMessage( vW, 1, szName );

      if ( !GetSelectStateOfEntityForSet( vGRW, "ER_Entity", 1 ) )
         ERD_SelectEntity( vW, "ERD", 1, 1 );
      if ( !ERD_IsEntityVisible( vW, "ERD" ) )
         CenterEntity( vW, "ERD" );
   }

   return( 0 );
}

#ifdef __cplusplus
}
#endif
