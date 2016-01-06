/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlopr.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C 'wrappers' for Zeidon Tools controls.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1998.09.24    DKS
//    Fixed PrintDiagram to provide a return code to the application when
//    the print is canceled (QS 78).
//
// 1998.09.10    DKS
//    Permit ZoomDiagram to return the current settings so that they
//    may be stored for future use.

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "TzCtl.h"
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//  OPERATION: ER_Diagram
//
//  PURPOSE:   This Operation calls the ctor for an ER Diagram.
//             It is called by the driver to create an ER Control
//
CWnd * OPERATION
ER_Diagram( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZER_Diagram( pZSubtask, pWndParent,
                            pzmaComposite, vDialog,
                            nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=ZoomDiagram
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ZoomDiagram
//
//  PURPOSE:   This Operation is used by applications to send the Zoom
//             message to the ER Diagram control.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the ER Control.
//               cpcCtrlTag - The tag of the ER Control
//               nPercent   - The percentage change or absolute percentage
//                            depending on the options parameter
//               nOptions   - Zooming options
//                            1 - Absolute Zooming (i.e. Zoom to nPercent)
//                            2 - Relative Zooming (Zoom in or out by
//                                                  nPercent. In being
//                                                  positive and out being
//                                                  negative)
//                            3 - Make the whole diagram fit into the size
//                                of the control (nPercent is ignored).
//
//  RETURNS:    -1  - Error locating window or ER control.
//              Note that if nPercent and nOptions are both 0, the return
//              value is the current Zooming Scale.  Otherwise, the return
//              value is zero.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ZoomDiagram( zVIEW    vSubtask,
             zPCHAR   cpcCtrlTag,
             zSHORT   nPercent,
             zSHORT   nOptions )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         return( pER_Diagram->ZoomDiagram( nPercent, nOptions ) );
      }

      TraceLineS( "drvr - Invalid control type for ZoomDiagram ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=PrintDiagram
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: PrintDiagram
//
//  PURPOSE:   This Operation is used by applications to send the Print
//             message to the E/R Diagram control.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the ER Control.
//               cpcCtrlTag - The tag of the ER Control
//               nPercent   - The percentage at which to print the
//                            diagram (with zPRINT_PERCENT Option)
//               nOptions   - Printing Options
//
//                            zPRINT_PAGE - Print on one page, shrink
//                                          if necessary
//                            zPRINT_PERCENT - Print the percentage value
//                                             passed.
//                            zPRINT_FILLPAGE - Fill the page, shrink OR
//                                              enlarge as necessary
//                            zPRINT_PORTRAIT - Force Portrait Orientation
//                            zPRINT_LANDSCAPE - Force Landscape Orientation
//
//                                    NOTE: No orientation will select based
//                                          on the largest percentage to
//                                          fit the page or the use of
//                                          the fewest number of pages to
//                                          print the diagram.
//
//  RETURNS:    -1 - Error locating window or E/R Diagram control.
//               0 - Print OK.
//               1 - Print Canceled.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
PrintDiagram( zVIEW    vSubtask,
              zPCHAR   cpcCtrlTag,
              zSHORT   nPercent,
              zSHORT   nOptions )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         return( pER_Diagram->PrintDiagram( nPercent, nOptions ) );
      }

      TraceLineS( "drvr - Invalid control type for PrintDiagram ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=AcceptEntity
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: AcceptEntity
//
//  PURPOSE:   This Operation is used by applications to Accept an Entity
//             painted in the ER diagram after the Create Node event is
//             sent to the application.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
AcceptEntity( zVIEW    vSubtask,
              zPCHAR   cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;
   ZER_Entity *pEntity;
   zPCHAR   pchText;
   zPCHAR   pchType;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram && pER_Diagram->m_pPendingEntity &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                             == zCURSOR_SET )
      {
         pEntity = pER_Diagram->m_pPendingEntity;
         pER_Diagram->m_pPendingEntity = 0;
         GetIntegerFromAttribute( &pEntity->m_lPosZKey,
                                  vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlZKey );
         GetIntegerFromAttribute( &pEntity->m_lZKey, vDiagram,
                                  szlER_Entity, szlZKey );
         SetAttributeFromInteger( vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlER_DiagramPosX,
                                  (zLONG) pEntity->PosX( ) );
         SetAttributeFromInteger( vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlER_DiagramPosY,
                                  (zLONG) pEntity->PosY( ) );
         GetAddrForAttribute( &pchText, vDiagram, szlER_Entity, szlName );
         pEntity->m_csText = pchText;
         GetAddrForAttribute( &pchType, vDiagram, szlER_Entity, szlPurpose );
         pEntity->m_chType = pchType[ 0 ];
         pEntity->DrawEntity( );
      }
   }

   return( -1 );
}

//./ ADD NAME=AdjustRelPortsForSelEnt
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: AdjustRelPortsForSelEnt
//
//  PURPOSE:   This Operation is used by applications to adjust the number
//             of relationship ports for a selected entity.
//
//  RETURNS:   0 - Adjustment succeeded
//             1 - no entity was selected
//             2 - more than one entity was selected
//            -1 - control not found in dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
AdjustRelPortsForSelectedEnt( zVIEW    vSubtask,
                              zPCHAR   cpcCtrlTag,
                              zSHORT   bIncrement )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      zVIEW vDiagram;
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );

      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         ZER_Entity *pEntity = 0;
         int nPos = pER_Diagram->m_nEntCnt;
         while ( nPos-- )
         {
            if ( pER_Diagram->m_EntityList[ nPos ]->m_bSelected )
            {
               if ( pEntity )
                  return( 2 );

               pEntity = pER_Diagram->m_EntityList[ nPos ];
            }
         }

         if ( pEntity )
         {
            pEntity->DrawEntity( TRUE ); // blank out entity
            pEntity->MarkEntity( TRUE ); // erase entity markings in grid
            if ( bIncrement )
               pEntity->m_lPortFactor++;
            else
            if ( pEntity->m_lPortFactor > 1 )
               pEntity->m_lPortFactor--;

            SetAttributeFromInteger( vDiagram,
                                     *(pER_Diagram->m_pzsEName),
                                     szlER_PortFactor,
                                     pEntity->m_lPortFactor );

            if ( pEntity->m_lPortFactor > 1 )
            {
               pEntity->m_lEntPortX = (pEntity->m_lPortFactor * tzERENTITYX);
               pEntity->m_lEntPortY = (pEntity->m_lPortFactor * tzERENTITYY);
               if ( pEntity->m_lEntPortX % 2 == 0 ) // force it to be odd (aesthetics)
                  pEntity->m_lEntPortX -= 1;

               if ( pEntity->m_lEntPortY % 2 == 0 ) // force it to be odd (aesthetics)
                  pEntity->m_lEntPortY -= 1;
            }
            else
            {
               pEntity->m_lEntPortX = tzERENTITYX;
               pEntity->m_lEntPortY = tzERENTITYY;
            }

         // pEntity->DrawEntity( ); drawn as a result of Move
            pEntity->Move( pEntity->PosX( ), pEntity->PosY( ), FALSE );
            RecalculateAllRelationships( vSubtask, cpcCtrlTag );
            return( 0 );
         }

         return( 1 );
      }
   }

   return( -1 );
}

//./ ADD NAME=RecalculateAllRelationships
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RecalculateAllRelationships
//
//  PURPOSE:   This Operation is used by applications to recalculate and
//             re-display all relationships.
//
//  RETURNS:   0 - recalculate succeeded
//            -1 - control not found in dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RecalculateAllRelationships( zVIEW    vSubtask,
                             zPCHAR   cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         ZER_Entity *pEntity;
         int nPos = pER_Diagram->m_nEntCnt;
         while ( nPos-- )
         {
            pEntity = pER_Diagram->m_EntityList[ nPos ];
            pEntity->DrawEntity( TRUE ); // blank out entity
            pEntity->MarkEntity( TRUE ); // erase entity markings in grid
         // pEntity->DrawEntity( ); drawn as a result of Move
            pEntity->Move( pEntity->PosX( ), pEntity->PosY( ), TRUE );
         }
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=ExpandDiagram
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ExpandDiagram
//
//  PURPOSE:   This Operation is used by applications to send the Expand
//             Diagram message to the ER Diagram control.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the ER Control.
//               cpcCtrlTag - The tag of the ER Control
//               nPercent   - The percentage change or absolute percentage
//                            depending on the options parameter
//
//               nPercent   - The percentage change
//
//  RETURNS:    -1  - Error locating window or ER control.
//               0  - Expansion OK.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ERD_ExpandDiagram( zVIEW    vSubtask,
                   zPCHAR   cpcCtrlTag,
                   zSHORT   nPercent )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram && nPercent )
      {
         pER_Diagram->ExpandContractDiagram( nPercent, 0 );
         RecalculateAllRelationships( vSubtask, cpcCtrlTag );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for ERD_ExpandDiagram ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=ERD_ContractDiagram
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ERD_ContractDiagram
//
//  PURPOSE:   This Operation is used by applications to send the Compress
//             Diagram message to the ER Diagram control.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the ER Control.
//               cpcCtrlTag - The tag of the ER Control
//               nPercent   - The percentage change
//
//  RETURNS:    -1  - Error locating window or ER control.
//               0  - Compression OK.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ERD_ContractDiagram( zVIEW    vSubtask,
                     zPCHAR   cpcCtrlTag,
                     zSHORT   nPercent )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram && nPercent )
      {
         pER_Diagram->ExpandContractDiagram( nPercent, 1 );
         RecalculateAllRelationships( vSubtask, cpcCtrlTag );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for ERD_ContractDiagram ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RefreshEntity
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RefreshEntity
//
//  PURPOSE:   This Operation is used by applications to Refresh the
//             entity text in the diagram after the application has
//             modified the entity in the Zeidon Object. This would
//             normally happen automatically in MapFromOI except that
//             MapFromOI is too expensive and is therefore ignored
//             other than the initial mapping of the control.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RefreshEntity( zVIEW    vSubtask,
               zPCHAR   cpcCtrlTag )
{
   ZSubtask  *pZSubtask;
   ZMapAct   *pzma;
   zVIEW     vDiagram;
   ZER_Entity *pEntity;
   zPCHAR    pchText;
   zPCHAR    pchType;
   zLONG     lZKey, lPosZKey, lPosX, lPosY;
   zSHORT    nPos;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                             == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlZKey );
         for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
         {
            pEntity = pER_Diagram->m_EntityList[ nPos ];
            if ( pEntity->m_lPosZKey == lZKey )
               break;
         }

         // If the entity was not found, attempt to create an entity
         if ( nPos == pER_Diagram->m_nEntCnt )
         {
            GetIntegerFromAttribute( &lPosZKey, vDiagram,
                                     *(pER_Diagram->m_pzsEName),
                                     szlZKey );
            GetIntegerFromAttribute( &lPosX, vDiagram,
                                     *(pER_Diagram->m_pzsEName),
                                     szlER_DiagramPosX );
            GetIntegerFromAttribute( &lPosY, vDiagram,
                                     *(pER_Diagram->m_pzsEName),
                                     szlER_DiagramPosY );
            if ( lPosX || lPosY )
            {
               pEntity = new ZER_Entity( pER_Diagram,
                                         (zSHORT) lPosX,
                                         (zSHORT) lPosY,
                                         0, 0, lPosZKey, lZKey, 0 );
               pER_Diagram->m_EntityList.
                     SetAtGrow( pER_Diagram->m_nEntCnt++, pEntity );
            }
         }

         // we need to retrieve the potential new item count here!
         if ( nPos < pER_Diagram->m_nEntCnt )
         {
            GetAddrForAttribute( &pchText, vDiagram, szlER_Entity, szlName );
            pEntity->m_csText = pchText;
            GetAddrForAttribute( &pchType, vDiagram,
                                 szlER_Entity, szlPurpose );
            pEntity->m_chType = pchType[ 0 ];
            pEntity->Erase( 0 );
            pEntity->m_bSelected = (zBOOL)
               GetSelectStateOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) );
            pEntity->DrawEntity( );
            return( 0 );
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=ERD_SelectEntity
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ERD_SelectEntity
//
//  PURPOSE:  This Operation is used to select/deselect an entity in the
//            ER diagram based on the current cursor position of the
//            instance used to map the diagram.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the ER Control.
//               cpcCtrlTag - The tag of the ER Control
//               bSelect    - 1 - Select it
//                            0 - Deselect it
//               bClear     - 1 - Clear all selected first
//                            0 - Do Not clear all selected first
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ERD_SelectEntity( zVIEW    vSubtask,
                  zPCHAR   cpcCtrlTag,
                  zBOOL    bSelect,
                  zBOOL    bClear )
{
   ZSubtask    *pZSubtask;
   ZMapAct     *pzma;
   zVIEW       vDiagram;
   ZER_Entity  *pEntity;
   zLONG       lZKey;
   zSHORT      nPos;

// TraceLineS( "ERD_SelectEntity ", cpcCtrlTag );
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                             == zCURSOR_SET )
      {
         if ( bClear )
         {
            SetAllSelStatesForEntityForSet( vDiagram,
                                      *(pER_Diagram->m_pzsEName), 0, 1, 0 );
         }

         GetIntegerFromAttribute( &lZKey, vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlZKey );
         for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
         {
            pEntity = pER_Diagram->m_EntityList[ nPos ];
            if ( pEntity->m_lPosZKey == lZKey )
            {
               if ( bSelect || bClear )
                  pEntity->Select( 0, bClear );
               if ( bSelect )
               {
                  SetSelectStateOfEntity( vDiagram,
                                          *(pER_Diagram->m_pzsEName),
                                          1 );
               }
               else
                  pEntity->Deselect( );

               return( 0 );
            }
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=ERD_SelectEntityNeighbors
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: SelectEntityNeighbors
//
//  PURPOSE:  This Operation is used to select/deselect neighbors in the
//            ER diagram based on the current cursor position of the
//            instance used to map the diagram.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the ER Control.
//               cpcCtrlTag - The tag of the ER Control
//               bSelect     - 1 - Select them
//                             0 - Deselect them
//               nSelectRels - 0 - Do not process relationships
//                             1 - Process relationships
//                             2 - Process relationships ONLY
//               bClear      - 1 - Clear all selected first
//                             0 - Do Not clear all selected first
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
SelectEntityNeighbors( zVIEW    vSubtask,
                       zPCHAR   cpcCtrlTag,
                       zBOOL    bSelect,
                       zSHORT   nSelectRels,
                       zBOOL    bClear )
{
   ZSubtask    *pZSubtask;
   ZMapAct     *pzma;
   zVIEW       vDiagram;
   ZER_Entity  *pEntity;
   zLONG       lZKey;
   zSHORT      nPos;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                             == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlZKey );
         for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
         {
            pEntity = pER_Diagram->m_EntityList[ nPos ];
            if ( pEntity->m_lPosZKey == lZKey )
               break;
         }
      }

      if ( nPos < pER_Diagram->m_nEntCnt )
      {
         pEntity->SelectNeighbors( bSelect, nSelectRels, bClear );
         return( 0 );
      }
   }

   return( -1 );
}

//./ ADD NAME=ERD_IsEntityVisible
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ERD_IsEntityVisible
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zBOOL OPERATION
ERD_IsEntityVisible( zVIEW    vSubtask,
                     zPCHAR   cpcCtrlTag )
{
   ZSubtask    *pZSubtask;
   ZMapAct     *pzma;
   zVIEW       vDiagram;
   ZER_Entity  *pEntity;
   zLONG       lZKey;
   zSHORT      nPos;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                             == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlZKey );
         for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
         {
            pEntity = pER_Diagram->m_EntityList[ nPos ];
            if ( pEntity->m_lPosZKey == lZKey )
            {
               if ( pER_Diagram->PositionVisible( pEntity->m_lPosIdx,
                                                  pEntity->m_lPosIdx ) )
               {
                  zLONG lPos = pEntity->m_lPosIdx + (tzERENTITYX - 1) +
                               ((tzERDIAGRAMX * tzERENTITYY) - 1);
                  return( pER_Diagram->PositionVisible( lPos, lPos ) );
               }

               return( 0 );
            }
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=RemoveEntity
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RemoveEntity
//
//  PURPOSE:   This Operation is used by applications to Remove the
//             Entity from the diagram BEFORE the application actually
//             deletes the entity in the Zeidon Object. This would
//             normally happen automatically in MapFromOI except that
//             MapFromOI is too expensive and is therefore ignored
//             other than the initial mapping of the control.
//
//             If the pending flag is set, then the pending entity is removed,
//             else, if a ZKey is passed then the ZKey is used to qualify,
//             the entity to be removed. Otherwise, the ZKey is pulled from
//             the positioning entity of the Zeidon object to determine
//             which ZKey to remove
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
RemoveEntity( zVIEW    vSubtask,
              zPCHAR   cpcCtrlTag,
              zBOOL    bPending,
              zLONG    lZKey )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;
   ZER_Entity *pEntity;
   zSHORT   nPos;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           (bPending && pER_Diagram->m_pPendingEntity) ||
           lZKey ||
           (GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                           vSubtask, zLEVEL_ANY ) > 0 &&
            CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                            == zCURSOR_SET) )
      {
         if ( bPending )
         {
            for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
            {
               pEntity = pER_Diagram->m_EntityList[ nPos ];
               if ( pEntity == pER_Diagram->m_pPendingEntity )
               {
                  pER_Diagram->m_pPendingEntity = 0;
                  break;
               }
            }
         }
         else
         {
            if ( !lZKey )
            {
               GetIntegerFromAttribute( &lZKey, vDiagram,
                                        *(pER_Diagram->m_pzsEName),
                                        szlZKey );
            }

            for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
            {
               pEntity = pER_Diagram->m_EntityList[ nPos ];
               if ( pEntity->m_lPosZKey == lZKey )
                  break;
            }
         }

         if ( nPos < pER_Diagram->m_nEntCnt )
         {
            pEntity->Erase( );
            pER_Diagram->m_EntityList.RemoveAt( nPos );
            delete( pEntity );
            pER_Diagram->m_nEntCnt--;
//          while ( nPos < nItemCnt )   CArray keeps elements left justified!
//          {
//             nPos++;
//             pER_Diagram->m_EntityList[ nPos - 1 ] =
//                                        pER_Diagram->m_EntityList[ nPos ];
//          }
//
//          pER_Diagram->m_EntityList[ nItemCnt ] = 0;
            return( 0 );
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=CenterEntity
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: CenterEntity
//
//  PURPOSE:   This Operation centers an entity in the diagram
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
CenterEntity( zVIEW    vSubtask,
              zPCHAR   cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;
   ZER_Entity *pEntity;
   zLONG    lZKey;
   zSHORT   nPos;

// TraceLineS( "CenterEntity ", cpcCtrlTag );
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsEName) )
                                                             == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDiagram,
                                  *(pER_Diagram->m_pzsEName),
                                  szlZKey );
         for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
         {
            pEntity = pER_Diagram->m_EntityList[ nPos ];
            if ( pEntity->m_lPosZKey == lZKey )
            {
               pEntity->CenterEntity( );
               return( 0 );
            }
         }
      }
   }
   return( -1 );
}

//./ ADD NAME=ValidNewEntityPosition
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ValidNewEntityPosition
//
//  PURPOSE:   This Operation validates that the X,Y coordinates passed
//             will not cause a collision with another entity
//
//  RETURNS:   1=Position is  valid
//             0=Position not valid
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ValidNewEntityPosition( zVIEW    vSubtask,
                        zPCHAR   cpcCtrlTag,
                        zLONG    lPosX,
                        zLONG    lPosY )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zPCHAR   pchGrid;
   zLONG    lIdx;
   zLONG    lWk1, lWk2;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         pchGrid = pER_Diagram->m_pchGrid;
         if ( lPosX >= tzERDIAGRAMX )
            return( 0 );

         if ( lPosY >= tzERDIAGRAMY )
            return( 0 );

         lIdx = (lPosY * tzERDIAGRAMX) + lPosX;
         lIdx -= (2 * tzERDIAGRAMX);
         lIdx -= 2;
         for ( lWk1 = 0; lWk1 < (tzERENTITYY + 4); lWk1++ )
         {
            for ( lWk2 = 0; lWk2 < (tzERENTITYX + 4); lWk2++ )
            {
               if ( lIdx >= 0 && lIdx <= tzERMAXGRID &&
                    pchGrid[ lIdx ] & tziENTITY ) //  &&
   //                            pchGrid[ lIdx ] & (tziENTITY | tziTEXT) )
               {
                  return( 0 );
               }

               lIdx++;
            }

            lIdx -= (tzERENTITYX + 4);
            lIdx += tzERDIAGRAMX;
         }

         // Call mapping to ensure all positions are updated before
         // returning success
         pER_Diagram->MapToOI( );
         return( 1 );
      }
   }

   // default is error
   return( -1 );
}

//./ ADD NAME=AcceptRelationship
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: AcceptRelationship
//
//  PURPOSE:   This Operation is used by applications to Accept a
//             Relationship in the diagram after the Create Arc event is
//             sent to the application.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
AcceptRelationship( zVIEW    vSubtask,
                    zPCHAR   cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;
   ZER_Relationship *pRelationship;
   zCHAR    szCard[ 10 ];
   zCHAR    szText[ 60 ];

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           pER_Diagram->m_pPendingRelationship &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsAName) )
                                                             == zCURSOR_SET )
      {
         pRelationship = pER_Diagram->m_pPendingRelationship;
         pER_Diagram->m_pPendingRelationship = 0;
         GetIntegerFromAttribute( &pRelationship->m_lPosZKey, vDiagram,
                                  *(pER_Diagram->m_pzsAName),
                                  szlZKey );
         if ( MiGetUpdateForView( vDiagram ) > 0 )
         {
            SetAttributeFromInteger( vDiagram,
                 *(pER_Diagram->m_pzsAName), szlER_DiagramE1PosX,
                 (zLONG) pRelationship->PosX( pRelationship->m_lStartIdx ) );
            SetAttributeFromInteger( vDiagram,
                 *(pER_Diagram->m_pzsAName), szlER_DiagramE1PosY,
                 (zLONG) pRelationship->PosY( pRelationship->m_lStartIdx ) );
            SetAttributeFromInteger( vDiagram,
                 *(pER_Diagram->m_pzsAName), szlER_DiagramE2PosX,
                 (zLONG) pRelationship->PosX( pRelationship->m_lEndIdx ) );
            SetAttributeFromInteger( vDiagram,
                 *(pER_Diagram->m_pzsAName), szlER_DiagramE2PosY,
                 (zLONG) pRelationship->PosY( pRelationship->m_lEndIdx ) );
            if ( pRelationship->m_pSrcEntity == pRelationship->m_pTgtEntity )
            {
               SetAttributeFromInteger( vDiagram,
                  *(pER_Diagram->m_pzsAName), szlER_DiagramBendPosX,
                  (zLONG) pRelationship->PosX( pRelationship->m_lArcIdx2 ) );
               SetAttributeFromInteger( vDiagram,
                  *(pER_Diagram->m_pzsAName), szlER_DiagramBendPosY,
                  (zLONG) pRelationship->PosY( pRelationship->m_lArcIdx2 ) );
            }
            else
            {
               SetAttributeFromInteger( vDiagram,
                  *(pER_Diagram->m_pzsAName), szlER_DiagramBendPosX,
                  (zLONG) pRelationship->PosX( pRelationship->m_lArcIdx1 ) );
               SetAttributeFromInteger( vDiagram,
                  *(pER_Diagram->m_pzsAName), szlER_DiagramBendPosY,
                  (zLONG) pRelationship->PosY( pRelationship->m_lArcIdx1 ) );
            }
         }

         SetCursorFirstEntity( vDiagram, szlER_RelLink_2, 0 );
         GetStringFromAttribute( szText, sizeof( szText ), vDiagram,
                                 szlER_RelLink_2, szlName );
         strcat_s( szText, sizeof( szText ), szlLPAREN );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMin );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bSrcReq = 1;
         else
            pRelationship->m_bSrcReq = 0;

         strcat_s( szText, sizeof( szText ), szlCOMMA );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMax );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bSrcMany = 0;
         else
            pRelationship->m_bSrcMany = 1;

         strcat_s( szText, sizeof( szText ), szlRPAREN );
         pRelationship->m_csTextF = szText;
         SetCursorNextEntity( vDiagram, szlER_RelLink_2, 0 );
         GetStringFromAttribute( szText, sizeof( szText ), vDiagram,
                                 szlER_RelLink_2, szlName );
         strcat_s( szText, sizeof( szText ), szlLPAREN );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMin );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bTgtReq = 1;
         else
            pRelationship->m_bTgtReq = 0;

         strcat_s( szText, sizeof( szText ), szlCOMMA );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMax );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bTgtMany = 0;
         else
            pRelationship->m_bTgtMany = 1;
         strcat_s( szText, sizeof( szText ), szlRPAREN );
         pRelationship->m_csTextR = szText;
         pRelationship->Erase( FALSE );
         pRelationship->m_bSelected = (zBOOL)
            GetSelectStateOfEntity( vDiagram,
                                    *(pER_Diagram->m_pzsAName) );
         pRelationship->DrawRelationship( );
      }
   }

   return( -1 );
}

//./ ADD NAME=RefreshRelationship
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RefreshRelationship
//
//  PURPOSE:   This Operation is used by applications to Refresh the
//             relationship text in the diagram after the application has
//             modified the relationship in the Zeidon Object. This would
//             normally happen automatically in MapFromOI except that
//             MapFromOI is too expensive and is therefore ignored
//             other than the initial mapping of the control.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RefreshRelationship( zVIEW    vSubtask,
                     zPCHAR   cpcCtrlTag,
                     zLONG    lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;
   ZER_Relationship *pRelationship;
   ZER_Entity *pEntity;
   ZER_Entity *pSrcEntity;
   ZER_Entity *pTgtEntity;
   zSHORT   nCsrRC;
   zCHAR    szCard[ 10 ];
   zCHAR    szText[ 60 ];
   zLONG    lZKey, lSrcZKey, lTgtZKey;
   zSHORT   nPos;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( (lFlag & 0x00000001) && pER_Diagram->m_bRefreshERD == FALSE )
         return( 0 );

      if ( pER_Diagram &&
           GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                          vSubtask, zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsAName) )
                                                             == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDiagram,
                                  *(pER_Diagram->m_pzsAName),
                                  szlZKey );
         for ( nPos = 0; nPos < pER_Diagram->m_nRelCnt; nPos++ )
         {
            pRelationship = pER_Diagram->m_RelationshipList[ nPos ];
            if ( pRelationship->m_lPosZKey == lZKey )
               break;
         }
      }
      else
         return( -1 );

      // If the relationship was not found, attempt to add the relationship
      // to the diagram.
      if ( nPos >= pER_Diagram->m_nRelCnt )
      {
         SetCursorPrevEntity( vDiagram, szlER_RelLink_2, 0 );
         GetIntegerFromAttribute( &lSrcZKey, vDiagram,
                                  szlER_Entity_2, szlZKey );
         nCsrRC = SetCursorNextEntity( vDiagram, szlER_RelLink_2, 0 );
         GetIntegerFromAttribute( &lTgtZKey, vDiagram,
                                  szlER_Entity_2, szlZKey );
         // Now find the source and target entity's in the entity list
         pRelationship = 0;
         pSrcEntity = 0;
         if ( nCsrRC == zCURSOR_SET )
         {
            for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
            {
               pEntity = pER_Diagram->m_EntityList[ nPos ];
               if ( pEntity->m_lZKey == lSrcZKey )
               {
                  pSrcEntity = pEntity;
                  break;
               }
            }
         }

         if ( pSrcEntity )
         {
            pTgtEntity = 0;
            for ( nPos = 0; nPos < pER_Diagram->m_nEntCnt; nPos++ )
            {
               pEntity = pER_Diagram->m_EntityList[ nPos ];
               if ( pEntity->m_lZKey == lTgtZKey )
               {
                  pTgtEntity = pEntity;
                  break;
               }
            }
         }

         if ( pSrcEntity && pTgtEntity )
         {
            pRelationship = new ZER_Relationship( pER_Diagram, pSrcEntity,
                                                  pTgtEntity, "", "",
                                                  0, 1, 0, 1, lZKey,
                                                  0, 0, 0, 0 );
            pER_Diagram->m_RelationshipList.
                         SetAtGrow( pER_Diagram->m_nEntCnt, pRelationship );
         }

         if ( !pRelationship )
            return( -1 );
      }

      // Now go through and refresh the relationship information.
      if ( nPos < pER_Diagram->m_nRelCnt )
      {
         SetCursorPrevEntity( vDiagram, szlER_RelLink_2, 0 );
         GetStringFromAttribute( szText, sizeof( szText ),
                                 vDiagram, szlER_RelLink_2, szlName );
         strcat_s( szText, sizeof( szText ), szlLPAREN );
         GetStringFromAttribute( szCard, sizeof( szCard ),
                                 vDiagram, szlER_RelLink_2, szlCardMin );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bSrcReq = 1;
         else
            pRelationship->m_bSrcReq = 0;

         strcat_s( szText, sizeof( szText ), szlCOMMA );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMax );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bSrcMany = 0;
         else
            pRelationship->m_bSrcMany = 1;

         strcat_s( szText, sizeof( szText ), szlRPAREN );
         pRelationship->m_csTextF = szText;
         SetCursorNextEntity( vDiagram, szlER_RelLink_2, 0 );
         GetStringFromAttribute( szText, sizeof( szText ), vDiagram, szlER_RelLink_2, szlName );
         strcat_s( szText, sizeof( szText ), szlLPAREN );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMin );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bTgtReq = 1;
         else
            pRelationship->m_bTgtReq = 0;

         strcat_s( szText, sizeof( szText ), szlCOMMA );
         GetStringFromAttribute( szCard, sizeof( szCard ), vDiagram,
                                 szlER_RelLink_2, szlCardMax );
         strcat_s( szText, sizeof( szText ), szCard );
         if ( atoi( szCard ) == 1 )
            pRelationship->m_bTgtMany = 0;
         else
            pRelationship->m_bTgtMany = 1;

         strcat_s( szText, sizeof( szText ), szlRPAREN );
         pRelationship->m_csTextR = szText;
         pRelationship->Erase( 0 );
         pRelationship->m_bSelected = (zBOOL)
               GetSelectStateOfEntity( vDiagram,
                                       *(pER_Diagram->m_pzsAName) );
         pRelationship->DrawRelationship( );
         return( 0 );
      }
   }

   return( -1 );
}


//./ ADD NAME=RemoveRelationship
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RemoveRelationship
//
//  PURPOSE:   This Operation is used by applications to Remove the
//             relationship from the diagram BEFORE the application actually
//             deletes the relatiinship in the Zeidon Object. This would
//             normally happen automatically in MapFromOI except that
//             MapFromOI is too expensive and is therefore ignored
//             other than the initial mapping of the control.
//
//             If the pending flag is set, then the pending relationship is
//             removed, else, if a ZKey is passed then the ZKey is used to
//             qualify the relationship to be removed. Otherwise, the ZKey is
//             pulled from the positioning entity of the Zeidon object to
//             determine which ZKey to remove
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
RemoveRelationship( zVIEW    vSubtask,
                    zPCHAR   cpcCtrlTag,
                    zBOOL    bPending,
                    zLONG    lZKey )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;
   ZER_Relationship *pRelationship;
   zSHORT   nPos;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram &&
           (bPending && pER_Diagram->m_pPendingRelationship) ||
           lZKey ||
           (GetViewByName( &vDiagram, *(pER_Diagram->m_pzsVName),
                           vSubtask, zLEVEL_ANY ) > 0 &&
            CheckExistenceOfEntity( vDiagram, *(pER_Diagram->m_pzsAName) )
                                                            == zCURSOR_SET) )
      {
         if ( bPending )
         {
            for ( nPos = 0; nPos < pER_Diagram->m_nRelCnt; nPos++ )
            {
               pRelationship = pER_Diagram->m_RelationshipList[ nPos ];
               if ( pRelationship == pER_Diagram->m_pPendingRelationship )
               {
                  pER_Diagram->m_pPendingRelationship = 0;
                  break;
               }
            }
         }
         else
         {
            if ( !lZKey )
            {
               GetIntegerFromAttribute( &lZKey, vDiagram,
                                        *(pER_Diagram->m_pzsAName),
                                        szlZKey );
            }

            for ( nPos = 0; nPos < pER_Diagram->m_nRelCnt; nPos++ )
            {
               pRelationship = pER_Diagram->m_RelationshipList[ nPos ];
               if ( pRelationship->m_lPosZKey == lZKey )
                  break;
            }
         }

         if ( nPos < pER_Diagram->m_nRelCnt )
         {
            pRelationship->Erase( );
            pER_Diagram->m_RelationshipList.RemoveAt( nPos );
            delete( pRelationship );
            pER_Diagram->m_nRelCnt--;
//          while ( nPos < nItemCnt )   CArray keeps elements left justified!
//          {
//             nPos++;
//             pER_Diagram->m_RelationshipList[ nPos - 1 ] =
//                                  pER_Diagram->m_RelationshipList[ nPos ];
//          }
//
//          pER_Diagram->m_RelationshipList[ nItemCnt ] = 0;
            return( 0 );
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=RefreshDiagram
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RefreshDiagram
//
//  PURPOSE:   This Operation will clear the whole ER diagram and redo
//             MapFromOI when either major modifications have been made
//             OR a new diagram object instance has been activated as
//             occurs in Switch LPLR.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RefreshDiagram( zVIEW    vSubtask,
                zPCHAR   cpcCtrlTag,
                zLONG    lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;


   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         if ( (lFlag & 0x00000001) && pER_Diagram->m_bRefreshERD == FALSE )
            return( 0 );

         pER_Diagram->m_bMultMap = 1;
         pER_Diagram->MapFromOI( );
         pER_Diagram->m_bMultMap = 0;
         return( 0 );
      }
   }

   return( -1 );
}

//./ ADD NAME=ERD_IsEntityVisible
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: GetPointerCoordinates
//
//  PURPOSE:   This Operation returns the X and Y position of the mouse
//             when an event was positioned, the coordinates returned are
//             screen coordinates!!!
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetPointerCoordinates( zVIEW    vSubtask,
                       zPCHAR   cpcCtrlTag,
                       zPLONG   plPosX,
                       zPLONG   plPosY )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   CPoint   pt;

   *plPosX = 0;
   *plPosY = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         pt = pER_Diagram->m_ptMouseSelect;
         pER_Diagram->ClientToScreen( &pt );
         *plPosX = pt.x;
         *plPosY = pt.y;
         return( 0 );
      }
   }

   return( -1 );
}

#define zDRAW_LINES   0x0001
#define zDRAW_TEXT    0x0002
#define zDRAW_NOCARD  0x0004

//./ ADD NAME=SetDrawOptions
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: SetDrawOptions
//
//  PURPOSE:   This set draw options (e.g. lines and text) for
//             the ER diagram.
//
//  OPTIONS:   zDRAW_LINES
//             zDRAW_TEXT
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SetDrawOptions( zVIEW    vSubtask,
                zPCHAR   cpcCtrlTag,
                zUSHORT  usOptions )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         if ( usOptions & zDRAW_LINES )
            pER_Diagram->m_bShowLines = 1;
         else
            pER_Diagram->m_bShowLines = 0;
         if ( usOptions & zDRAW_TEXT )
            pER_Diagram->m_bShowText = 1;
         else
            pER_Diagram->m_bShowText = 0;
         if ( usOptions & zDRAW_NOCARD )
            pER_Diagram->m_bShowCard = 0;
         else
            pER_Diagram->m_bShowCard = 1;
         pER_Diagram->RedrawDiagram( );
         return( 0 );
      }
   }

   return( -1 );
}

//./ ADD NAME=SetColors
// Source Module=tzctlopr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: SetColors
//
//  PURPOSE:   This Operation set the colors for the ER diagram
//             and optionally redraw the diagram.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
zOPER_EXPORT void OPERATION
SetColors( zVIEW    vSubtask,
           zPCHAR   cpcCtrlTag,
           zULONG   ulColorNormal,
           zULONG   ulColorSelected,
           zULONG   ulColorBox,
           zULONG   ulColorLines,
           zULONG   ulColorText,
           zULONG   ulColorNoUpdate,
           zBOOL    bRedraw )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZER_Diagram *pER_Diagram = DYNAMIC_DOWNCAST( ZER_Diagram, pzma->m_pCtrl );
      if ( pER_Diagram )
      {
         pER_Diagram->m_clrNormal = ulColorNormal;
         pER_Diagram->m_clrSelected = ulColorSelected;
         pER_Diagram->m_clrBox = ulColorBox;
         pER_Diagram->m_clrLines = ulColorLines;
         pER_Diagram->m_clrText = ulColorText;
         pER_Diagram->m_clrNoUpdate = ulColorNoUpdate;
         if ( bRedraw )
         {
            pER_Diagram->MapFromOI( );
            pER_Diagram->Invalidate( TRUE );
         }
      }
   }
}
