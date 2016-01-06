/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlerd.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Entity/Relationship control
// events.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.12.06    DKS    Z10    TB861
//    Fixed Redraw problems created when zooming.
//
// 1999.11.01    DKS    Z10    TB788
//    Relationships added that cannot be drawn are deleted.
//

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
// MODES:  If the mouse is currently positioned on an Entity, move the
//         Entity. If the Mouse is currently positioned on a relationship
//         move the relationship. Otherwise, paint a new entity
//
void
ZER_Diagram::OnLButtonDown( UINT uModKeys, CPoint pt )
{
   if ( m_nLDblClk )
   {
      if ( m_nLDblClk == 1 )
         return;

      m_nLDblClk = 0;
   }

   // wait to see if it is a double-click
   if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) )
   {
      m_nLDblClk = 1;
      return;
   }

   CPoint ptStart = pt;
   MakeLogicalPoint( ptStart );
   zLONG lIdx = MousePositionToIndex( pt );

   if ( lIdx <= tzERMAXGRID && GetCapture( ) == 0 && m_bUpdateView )
   {
      m_ptMouseSelect = pt;
      m_ptLastMouseMove = pt;
      SetCapture( );
      if ( m_hDC == 0 )
         GetDeviceContext( );

      // Now we determine the type of capture:
      //    1 - Create Entity
      //    2 - Move Entity
      //    3 - Move Relationship
      if ( m_pchGrid[ lIdx ] & tziENTITY )
      {
         m_nCapture = 2;   // Moving an Entity
         while ( lIdx > -1 && (m_pchGrid[ lIdx ] & tziENTITY) )
            lIdx--;

         lIdx++;
         while ( lIdx > 0 && (m_pchGrid[ lIdx ] & tziENTITY) )
            lIdx -= tzERDIAGRAMX;

         lIdx += tzERDIAGRAMX;
         m_lMouseOffsetX = ptStart.x -
                     ((PosX( lIdx ) - PosX( m_lDisplayPos )) * tzERCELLSIZE);
         m_lMouseOffsetY = ptStart.y -
                     ((PosY( lIdx ) - PosY( m_lDisplayPos )) * tzERCELLSIZE);

         // Look up the Source entity in the entity list
         ZER_Entity *pEntity;

         m_pMoveEntity = 0;
         for ( zSHORT nItemPos = 0; nItemPos < m_nEntCnt; nItemPos++ )
         {
            pEntity = m_EntityList[ nItemPos ];
            if ( pEntity->m_lPosIdx == lIdx )
            {
               m_pMoveEntity = pEntity;
               break;
            }
         }
      }
      else
      if ( m_pchGrid[ lIdx ] & (tziTEXTHORIZ | tziTEXTVERT | tziARC) &&
           (m_pchGrid[ lIdx ] & tziTEXT) == 0 )
      {
         m_nCapture = 3;  // Moving a relationship
         m_lMouseOffsetX = ptStart.x -
                  ((PosX( lIdx ) - PosX( m_lDisplayPos )) * tzERCELLSIZE);
         m_lMouseOffsetY = ptStart.y -
                  ((PosY( lIdx ) - PosY( m_lDisplayPos )) * tzERCELLSIZE);
      }
      else
      {
         CRect  rect;

         m_nCapture = 1;  // Creating a new Entity
         m_lMouseOffsetX = 0;
         m_lMouseOffsetY = 0;

         ClientToScreen( &pt );

         zLONG lRectX = tzERENTITYX * tzERCELLSIZE;
         zLONG lRectY = tzERENTITYY * tzERCELLSIZE;
         if ( m_lScale )
         {
            lRectX = (lRectX * (100 + m_lScale)) / 100;
            lRectY = (lRectY * (100 + m_lScale)) / 100;
         }

         rect.top = pt.y - lRectY;
         rect.left = pt.x - lRectX;
         rect.bottom = pt.y + lRectY;
         rect.right = pt.x + lRectX;
         ::ClipCursor( &rect );
      }

      while ( ::ShowCursor( 0 ) > -1 )
         ;
   }
}

// Message cracking METHOD for LButtonUp
void
ZER_Diagram::OnLButtonUp( UINT uModKeys, CPoint pt )
{
   CPoint   ptStart;
   CPoint   ptEnd;
   int      nOldROP2;
   zSHORT   nItemPos;
   zLONG    lIdx;
   ZER_Entity *pEntity;
   ZER_Relationship *pRelationship;
   zBOOL    bCtrl;

   if ( m_nLDblClk )
   {
      if ( m_nLDblClk == 2 )
         m_nLDblClk = 0;

      return;
   }

   bCtrl = (uModKeys & MK_CONTROL);

   if ( m_nCapture && m_nCapture < 4 )
   {
      if ( m_nCapture == 1 )
         ::ClipCursor( 0 );

      ReleaseCapture( );
      while ( ::ShowCursor( 1 ) < 0 )
      {
      }

      ptStart = m_ptMouseSelect;
      MakeLogicalPoint( ptStart );
      ptEnd = m_ptLastMouseMove;
      MakeLogicalPoint( ptEnd );
      nOldROP2 = m_hDC->SetROP2( R2_NOT );

      // If the last mouse position is different from the mouse capture
      // position, erase the drag rectangle.
      if ( m_ptLastMouseMove != m_ptMouseSelect )
      {
         CPoint ptWork;

         m_hDC->MoveTo( ptStart.x, ptStart.y );
         if ( m_nCapture == 1 )
         {
            // Erase the last rectangle drawn to represent the
            // painting of an Entity
            ptWork.x = ptEnd.x;
            ptWork.y = ptStart.y;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            m_hDC->LineTo( ptEnd.x, ptEnd.y );
            ptWork.x = ptStart.x;
            ptWork.y = ptEnd.y;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            m_hDC->LineTo( ptStart.x, ptStart.y );
         }
         else
         {
            ptEnd.x -= m_lMouseOffsetX;
            ptEnd.y -= m_lMouseOffsetY;
            ptWork.x = ptEnd.x;
            ptWork.y = ptEnd.y;
            m_hDC->MoveTo( ptWork.x, ptWork.y );
            if ( m_nCapture == 2 )
            {
               // Erase the last Rectangle drawn for moving an entity.
               if ( m_pMoveEntity )
               {
                  ptWork.x += (tzERCELLSIZE * m_pMoveEntity->m_lEntPortX);
                  m_hDC->LineTo( ptWork.x, ptWork.y );
                  ptWork.y += (tzERCELLSIZE * m_pMoveEntity->m_lEntPortY);
                  m_hDC->LineTo( ptWork.x, ptWork.y );
                  ptWork.x -= (tzERCELLSIZE * m_pMoveEntity->m_lEntPortX);
                  m_hDC->LineTo( ptWork.x, ptWork.y );
                  ptWork.y -= (tzERCELLSIZE * m_pMoveEntity->m_lEntPortY);
                  m_hDC->LineTo( ptWork.x, ptWork.y );
               }
            }
            else
            {
               // Erase the last Square drawn for moving a relationship
               ptWork.x += tzERCELLSIZE;
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.y += tzERCELLSIZE;
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.x -= tzERCELLSIZE;
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.y -= tzERCELLSIZE;
               m_hDC->LineTo( ptWork.x, ptWork.y );
            }
         }
      }

      m_hDC->SetROP2( nOldROP2 );

      if ( pt != m_ptMouseSelect && m_ptLastMouseMove != m_ptMouseSelect )
      {
         if ( m_nCapture == 1 )
         {
            zLONG  lWk1, lWk2;
            zLONG  lPosX, lPosY, nCollision;

            if ( ptStart.x < ptEnd.x )
            {
               lPosX =  ptStart.x / tzERCELLSIZE;
               lWk1 = ptEnd.x - ptStart.x;
            }
            else
            {
               lPosX =  ptEnd.x / tzERCELLSIZE;
               lWk1 = ptStart.x - ptEnd.x;
            }

            if ( ptStart.y < ptEnd.y )
            {
               lPosY =  ptStart.y / tzERCELLSIZE;
               lWk1 += ptEnd.y - ptStart.y;
            }
            else
            {
               lPosY =  ptEnd.y / tzERCELLSIZE;
               lWk1 += ptStart.y - ptEnd.y;
            }

            // If the box drawn was big enough, create an Entity.
            if ( (lWk1 / tzERCELLSIZE) >= tzERENTITYX )
            {
               // Add the display position to the entity.
               lPosX += PosX( tzERDISPLAYPOS );
               lPosY += PosY( tzERDISPLAYPOS );

               // Now check to see that an entity will fit in the grid.
               lIdx = ((zLONG) lPosY * tzERDIAGRAMX) + lPosX;
               lIdx -= (2 * tzERDIAGRAMX);
               lIdx -= 2;
               nCollision = 0;
               if ( lPosX <= 0 || (lPosX + tzERENTITYX) >= tzERDIAGRAMX ||
                    lPosY <= 0 || (lPosY + tzERENTITYY) >= tzERDIAGRAMY )
               {
                  nCollision = 1;
               }
               else
               {
                  for ( lWk1 = 0; lWk1 < (tzERENTITYY + 4); lWk1++ )
                  {
                     for ( lWk2 = 0; lWk2 < (tzERENTITYX + 4); lWk2++ )
                     {
                        if ( lIdx >= 0 && lIdx <= tzERMAXGRID &&
                              m_pchGrid[ lIdx ] & tziENTITY )
   //                                     (tziENTITY | tziTEXT) )
                        {
                           nCollision = 1;
                           break;
                        }

                        lIdx++;
                     }

                     if ( nCollision )
                        break;

                     lIdx -= (tzERENTITYX + 4);
                     lIdx += tzERDIAGRAMX;
                  }
               }

               if ( nCollision == 0 )
               {
                  pEntity = new ZER_Entity( this, lPosX, lPosY );
                  m_EntityList.SetAtGrow( m_nEntCnt++, pEntity );
                  pEntity->DrawEntity( );
                  m_pPendingEntity = pEntity;
                  m_nCapture = 0;
               /*y*/ReleaseDeviceContext( );
// TraceLineI("(tzerctaa) Triggering event #", 1L );
                  ProcessImmediateEvent( this, zER_NODE_CREATE );
                  return;
               }
            }
         }
         else
         if ( m_nCapture == 2 )   // Moving an entity
         {
            if ( m_pMoveEntity )  // This can be null if there are
                                  // overlapping entities at the
                                  // current position (which can only
                                  // happen as a result of a merge or
                                  // import from another E/R tool).
            {
               zLONG lPosX, lPosY;
               zLONG lMoveX, lMoveY;
               ZER_Entity *pWkEntity;

               // Calculate the new (x, y) position for the move.
               lPosX =  ptEnd.x / tzERCELLSIZE;
               lPosX += PosX( m_lDisplayPos );
               lPosY =  ptEnd.y / tzERCELLSIZE;
               lPosY += PosY( m_lDisplayPos );

               lMoveX = lPosX - PosX( m_pMoveEntity->m_lPosIdx );
               lMoveY = lPosY - PosY( m_pMoveEntity->m_lPosIdx );

               if ( bCtrl == FALSE || m_pMoveEntity->m_bSelected == FALSE )
                  m_pMoveEntity->Move( lPosX, lPosY );
               else
               {
                  for ( nItemPos = 0; nItemPos < m_nEntCnt; nItemPos++ )
                  {
                     pWkEntity = m_EntityList[ nItemPos ];
                     if ( pWkEntity->m_bSelected )
                     {
                        pWkEntity->Move( PosX( pWkEntity->m_lPosIdx ) + lMoveX,
                                         PosY( pWkEntity->m_lPosIdx ) + lMoveY,
                                         0 );
                     }
                  }

                  for ( nItemPos = 0; nItemPos < m_nEntCnt; nItemPos++ )
                  {
                     pWkEntity = m_EntityList[ nItemPos ];
                     if ( pWkEntity->m_bSelected )
                     {
                        pWkEntity->Move( PosX( pWkEntity->m_lPosIdx ),
                                         PosY( pWkEntity->m_lPosIdx ) );
                     }
                  }
               }

               m_pMoveEntity = 0;
            }
         }
         else
      // if ( m_nCapture == 3 )   // Moving a relationship
         {
            ZER_Relationship *pSrcRelationship;

            // Get the index of the Start of the line.
            lIdx = MousePositionToIndex( m_ptMouseSelect );
//          lIdx = ((zLONG) (ptStart.y / tzERCELLSIZE) *
//                      tzERDIAGRAMX) + (ptStart.x / tzERCELLSIZE);
//          lIdx += m_lDisplayPos;

            // Look up the Source relationship in the relationship list.
            for ( nItemPos = 0; nItemPos < m_nRelCnt; nItemPos++ )
            {
               pSrcRelationship = m_RelationshipList[ nItemPos ];
               if ( pSrcRelationship->m_lStartIdx == lIdx ||
                    pSrcRelationship->m_lArcIdx1 == lIdx ||
                    pSrcRelationship->m_lArcIdx2 == lIdx ||
                    pSrcRelationship->m_lArcIdx3 == lIdx ||
                    pSrcRelationship->m_lEndIdx == lIdx )
               {
                  break;
               }
            }

            if ( m_lScale && m_bScrolledWindow )
               SetRedraw( FALSE );

            pSrcRelationship->Erase( );  // Erase the relationship
            pSrcRelationship->CalculateLine( );
            if ( pSrcRelationship->m_lStartIdx == lIdx &&
                 pSrcRelationship->m_lEndIdx == lIdx &&
                 pSrcRelationship->m_pSrcEntity !=
                                       pSrcRelationship->m_pTgtEntity )
            {
               ZER_Entity *pWorkEntity;
               zLONG      lWorkIdx;
               pWorkEntity = pSrcRelationship->m_pSrcEntity;
               pSrcRelationship->m_pSrcEntity =
                                    pSrcRelationship->m_pTgtEntity;
               pSrcRelationship->m_pTgtEntity = pWorkEntity;
               pSrcRelationship->CalculateLine( );

               // After calculating the line with the source and target
               // entities reversed, reset the source and target
               // entities as well as the starting and ending positions
               // of the relationship line
               pSrcRelationship->m_pTgtEntity =
                                    pSrcRelationship->m_pSrcEntity;
               pSrcRelationship->m_pSrcEntity = pWorkEntity;
               lWorkIdx = pSrcRelationship->m_lStartIdx;
               pSrcRelationship->m_lStartIdx = pSrcRelationship->m_lEndIdx;
               pSrcRelationship->m_lEndIdx = lWorkIdx;
            }

            pSrcRelationship->DrawRelationship( );  // Redraw the rel
         }

         // if the diagram is Scaled, force a redraw of the whole window
         if ( m_lScale && m_bScrolledWindow )
         {
            SetRedraw( TRUE );
            Invalidate( TRUE );
            m_bScrolledWindow = FALSE;
         }
      }
      else
      {
         lIdx = MousePositionToIndex( m_ptMouseSelect );
         if ( (pEntity = IndexOnEntity( lIdx )) != 0 )
         {
            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
            if ( PositionOnZeidonEntity( pEntity, 0, 1, bCtrl ) == 1 )
            {
               pEntity->Select( bCtrl, !bCtrl );
// TraceLineI("(tzerctaa) Triggering event #", 2L );
               ProcessImmediateEvent( this, zER_NODE_LCLICK );
            }

            return;
         }
         else
         if ( (pRelationship = IndexOnRelationship( lIdx )) != 0 )
         {
            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
            if ( PositionOnZeidonRelationship( pRelationship,
                                               0, 1, bCtrl ) == 1 )
            {
// TraceLineI("(tzerctaa) Triggering event #
               ProcessEvent( this, zER_ARC_LCLICK );
               pRelationship->Select( bCtrl, !bCtrl );
            }

            return;
         }
         else
         {
            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
// TraceLineI("(tzerctaa) Triggering event #", 11L
            ProcessImmediateEvent( this, zER_SPACE_LCLICK );
            return;
         }
      }

      m_nCapture = 0;
   /*y*/ReleaseDeviceContext( );
   }
   else
   if ( m_nCapture == 0 )
   {
      m_ptMouseSelect = pt;
      lIdx = MousePositionToIndex( pt );
      if ( (pEntity = IndexOnEntity( lIdx )) != 0 )
      {
         if ( PositionOnZeidonEntity( pEntity, 0, 1, bCtrl ) == 1 )
         {
            pEntity->Select( bCtrl, !bCtrl );
// TraceLineI("(tzerctaa) Triggering event #", 2L
            ProcessImmediateEvent( this, zER_NODE_LCLICK );
         }

         return;
      }
      else
      if ( (pRelationship = IndexOnRelationship( lIdx )) != 0 )
      {
         if ( PositionOnZeidonRelationship( pRelationship,
                                            0, 1, bCtrl ) == 1 )
         {
// TraceLineI("(tzerctaa) Triggering even
            ProcessEvent( this, zER_ARC_LCLICK );
            pRelationship->Select( bCtrl, !bCtrl );
         }

         return;
      }
      else
      {
// TraceLineI("(tzerctaa) Triggering event #",
         ProcessImmediateEvent( this, zER_SPACE_LCLICK );
         return;
      }
   }
}

// Message cracking METHOD for LButtonDblClk
void
ZER_Diagram::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
   zLONG    lIdx;
   ZER_Entity *pEntity;
   ZER_Relationship *pRelationship;
   zBOOL    bCtrl;

   if ( m_nLDblClk == 0 )
      return;

   bCtrl = (uModKeys & MK_CONTROL);
   m_nLDblClk = 2;

   m_ptMouseSelect = pt;

   // Get the grid index of the mouse position
   lIdx = MousePositionToIndex( pt );
   if ( lIdx <= tzERMAXGRID )
   {
      if ( (pEntity = IndexOnEntity( lIdx )) != 0 )
      {
         if ( PositionOnZeidonEntity( pEntity, 0, 1, bCtrl ) == 1 )
         {
            pEntity->Select( bCtrl, !bCtrl );
// TraceLineI("(tzerctaa) Triggering event #", 3L
            ProcessImmediateEvent( this, zER_NODE_LDBLCLICK );
         }

         return;
      }
      else
      if ( (pRelationship = IndexOnRelationship( lIdx )) != 0 )
      {
         if ( PositionOnZeidonRelationship( pRelationship, 0,
                                            1, bCtrl ) == 1 )
         {
// TraceLineI("(tzerctaa) Triggering event #", 8L );
            pRelationship->Select( bCtrl, !bCtrl );
            ProcessImmediateEvent( this, zER_ARC_LDBLCLICK );
         }

         return;
      }
      else
      {
// TraceLineI("(tzerctaa) Triggering event #",
         ProcessImmediateEvent( this, zER_SPACE_LDBLCLICK );
         return;
      }
   }
}

// Message cracking METHOD for RButtonDown
void
ZER_Diagram::OnRButtonDown( UINT uModKeys, CPoint pt )
{
   zBOOL bShift;
   zLONG lIdx;

   // WM_RBUTTONDOWN - Draw a relationship if the mouse is on an Entity
   // OR ZOOM in if the shift key is down
   if ( m_nRDblClk )
   {
      if ( m_nRDblClk == 1 )
         return;

      m_nRDblClk = 0;
   }

   if ( IsDoubleClick( WM_RBUTTONDBLCLK, pt ) )
   {
      m_nRDblClk = 1;
      return;
   }

   bShift = (uModKeys & MK_SHIFT);
   lIdx = MousePositionToIndex( pt );
   if ( lIdx <= tzERMAXGRID && GetCapture( ) == 0 &&
        (bShift || (m_bUpdateView && (m_pchGrid[ lIdx ] & tziENTITY))) )
   {
      m_ptMouseSelect = pt;
      m_ptLastMouseMove = pt;
      SetCapture( );
      if ( m_hDC == 0 )
         GetDeviceContext( );

      if ( bShift )
         m_nCapture = 11;
      else
         m_nCapture = 10;

      while ( ::ShowCursor( 0 ) > -1 )
      {
      }
   }
}

// Message cracking METHOD for RButtonUp
void
ZER_Diagram::OnRButtonUp( UINT uModKeys, CPoint pt )
{
   zLONG    lIdx;
   int      nOldROP2;
   CPoint   ptStart;
   CPoint   ptEnd;
   CPoint   ptWork;
   zSHORT   nItemPos;
   zLONG    lMove;
   ZER_Entity *pEntity;
   ZER_Relationship *pRelationship;

   if ( m_nRDblClk )
   {
      if ( m_nRDblClk == 2 )
         m_nRDblClk = 0;

      return;
   }

   if ( m_nCapture && m_nCapture > 3 )
   {
      ReleaseCapture( );
      while ( ::ShowCursor( 1 ) < 0 )
      {
      }

      ptStart = m_ptMouseSelect;
      MakeLogicalPoint( ptStart );
      ptEnd = m_ptLastMouseMove;
      MakeLogicalPoint( ptEnd );
      nOldROP2 = m_hDC->SetROP2( R2_NOT );

      // If the last mouse position is different from the mouse
      // capture position, erase the drag rectangle.
      if ( m_ptLastMouseMove != m_ptMouseSelect )
      {
         if ( m_nCapture == 10 )
         {
            m_hDC->MoveTo( ptStart.x, ptStart.y );

            // Erase the last line drawn to represent the relationship.
            m_hDC->LineTo( ptEnd.x, ptEnd.y );
         }
         else
         {
            // Erase the last rectangle drawn to represent the
            // Rubberband Zoom.
            ptWork.x = ptEnd.x;
            ptWork.y = ptStart.y;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            m_hDC->LineTo( ptEnd.x, ptEnd.y );
            ptWork.x = ptStart.x;
            ptWork.y = ptEnd.y;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            m_hDC->LineTo( ptStart.x, ptStart.y );
         }
      }

      m_hDC->SetROP2( nOldROP2 );

      lMove = 0;
      if ( pt != m_ptMouseSelect )
      {
         if ( ptEnd.x > ptStart.x )
            lMove = ptEnd.x - ptStart.x;
         else
            lMove = ptStart.x - ptEnd.x;

         if ( ptEnd.y > ptStart.y )
            lMove += ptEnd.y - ptStart.y;
         else
            lMove += ptStart.y - ptEnd.y;
      }

      if ( pt != m_ptMouseSelect && lMove > (tzERCELLSIZE * 2) )
      {
         if ( m_nCapture == 10 )
         {
            ZER_Entity *pSrcEntity;
            ZER_Entity *pTgtEntity;
            zVIEW      vDiagram;

            lIdx = MousePositionToIndex( pt );
            if ( m_pchGrid[ lIdx ] & tziENTITY )  // did we end on an entity
            {
               // We started and ended a line on an entity, now
               // find the entities in the list and attempt to
               // create a relationship between the entities.
               while ( lIdx > 0 && m_pchGrid[ lIdx ] & tziENTITY )
                  lIdx--;

               lIdx++;
               while ( lIdx > 0 && m_pchGrid[ lIdx ] & tziENTITY )
                  lIdx -= tzERDIAGRAMX;

               lIdx += tzERDIAGRAMX;

               // We've found the target entity's position, now
               // look up the entity in the entity list.
               for ( nItemPos = 0; nItemPos < m_nEntCnt; nItemPos++ )
               {
                  pTgtEntity = m_EntityList[ nItemPos ];
                  if ( pTgtEntity->m_lPosIdx == lIdx )
                  {
                     break;
                  }
               }

               // Now find the source entity in the entity list.
               lIdx = ((zLONG) (ptStart.y / tzERCELLSIZE) *
                        tzERDIAGRAMX) + (ptStart.x / tzERCELLSIZE);
               lIdx += m_lDisplayPos;
               while ( lIdx > 0 && m_pchGrid[ lIdx ] & tziENTITY )
                  lIdx--;

               lIdx++;
               while ( lIdx > 0 && m_pchGrid[ lIdx ] & tziENTITY )
                  lIdx -= tzERDIAGRAMX;

               lIdx += tzERDIAGRAMX;

               // We've found the target entity's position, now
               // look up the entity in the entity list.
               for ( nItemPos = 0; nItemPos < m_nEntCnt; nItemPos++ )
               {
                  pSrcEntity = m_EntityList[ nItemPos ];
                  if ( pSrcEntity->m_lPosIdx == lIdx )
                  {
                     break;
                  }
               }

               // Now that we know the source and target entities, create
               // a relationship between the two entities.
               pRelationship = new ZER_Relationship( this, pSrcEntity,
                                                     pTgtEntity );
               AddRelationship( pRelationship );
               zSHORT nRC = pRelationship->DrawRelationship( FALSE, TRUE );
               m_nCapture = 0;

               if ( nRC < 0 ) // error drawing relationship, so let's delete it
               {
                  zSHORT  nPos;
                  for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
                  {
                     if ( pRelationship == m_RelationshipList[ nPos ] )
                     {
                        m_RelationshipList.RemoveAt( nPos );
                        delete( pRelationship );
                        m_nRelCnt--;
                        break;
                     }
                  }
               }
               else
               {
                  // Now trigger event 6 for the user of the control,
                  // BUT first, position on the source and target entities
                  // of the relationship.
                  if ( GetViewByName( &vDiagram, *m_pzsVName,
                                      m_pZSubtask->m_vDialog,
                                      zLEVEL_ANY ) > 0 )
                  {
                     PositionOnZeidonEntity( pSrcEntity, vDiagram );
                     CreateViewFromViewForTask( &vDiagram, vDiagram,
                                                m_pZSubtask->m_vDialog );
                     PositionOnZeidonEntity( pTgtEntity, vDiagram );
                     SetNameForView( vDiagram, szlTZRELTGT,
                                     m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
                     m_pPendingRelationship = pRelationship;
                  /*y*/ReleaseDeviceContext( );
// TraceLineI("(tzerctaa) Triggering event #", 6L );

                  // DisplayCells( "Before ArcCreate", 3, 9, 32, 55 ); // 77 );
                     ProcessImmediateEvent( this, zER_ARC_CREATE );
                  // DisplayCells( "After ArcCreate", 3, 9, 32, 55 );
                  }
               }

               return;
            }
         }
         else
         {
            zLONG  lSizeX1, lSizeY1, lSizeX2, lSizeY2;
            zLONG  lIdx1, lIdx2;
            zLONG  lScale;
            CRect  rect;

            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
            lIdx1 = MousePositionToIndex( m_ptMouseSelect );
            lIdx2 = MousePositionToIndex( pt );
            lSizeX1 = PosX( lIdx2 ) - PosX( lIdx1 );
            if ( lSizeX1 < 0 ) lSizeX1 = -lSizeX1;
            lSizeY1 = PosY( lIdx2 ) - PosY( lIdx1 );
            if ( lSizeY1 < 0 ) lSizeY1 = -lSizeY1;
            GetClientRect( rect );

            // Get the logical rect size with scaling
            lScale = 0;
            CalculateDisplaySize( lSizeX2, lSizeY2, rect, lScale );
            while ( lSizeX2 > lSizeX1 && lSizeY2 > lSizeY1 )
            {
               lScale += 20;
               CalculateDisplaySize( lSizeX2, lSizeY2, rect, lScale );
            }

            while ( lSizeX2 < lSizeX1 || lSizeY2 < lSizeY1 )
            {
               lScale -= 10;
               CalculateDisplaySize( lSizeX2, lSizeY2, rect, lScale );
            }

            while ( lSizeX2 > lSizeX1 && lSizeY2 > lSizeY1 )
            {
               lScale += 5;
               CalculateDisplaySize( lSizeX2, lSizeY2, rect, lScale );
            }

            while ( lSizeX2 < lSizeX1 && lSizeY2 < lSizeY1 )
            {
               lScale -= 2;
               CalculateDisplaySize( lSizeX2, lSizeY2, rect, lScale );
            }

            if ( lIdx2 < lIdx1 )
               lIdx1 = lIdx2;

            if ( lIdx1 < tzERMAXGRID )
            {
               m_lDisplayPos = lIdx1;
               ZoomDiagram( (zSHORT) (lScale + 100), 4 );
            }

            return;
         }
      }
      else
      {
         lIdx = MousePositionToIndex( m_ptMouseSelect );
         if ( (pEntity = IndexOnEntity( lIdx )) != 0 )
         {
            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
            PositionOnZeidonEntity( pEntity );
// TraceLineI("(tzerctaa) Triggering event #", 4L
            ProcessImmediateEvent( this, zER_NODE_RCLICK );
            return;
         }
         else
         if ( (pRelationship = IndexOnRelationship( lIdx )) != 0 )
         {
            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
            PositionOnZeidonRelationship( pRelationship );
// TraceLineI("(tzerctaa) Triggering event #", 9L
            ProcessImmediateEvent( this, zER_ARC_RCLICK );
            return;
         }
         else
         {
            m_nCapture = 0;
         /*y*/ReleaseDeviceContext( );
// TraceLineI("(tzerctaa) Triggering event #", 13L
            ProcessImmediateEvent( this, zER_SPACE_RCLICK );
            return;
         }
      }

      m_nCapture = 0;
   /*y*/ReleaseDeviceContext( );
   }
   else
   if ( m_nCapture == 0 )
   {
      m_ptMouseSelect = pt;
      lIdx = MousePositionToIndex( pt );
      if ( (pEntity = IndexOnEntity( lIdx )) != 0 )
      {
         PositionOnZeidonEntity( pEntity );
// TraceLineI("(tzerctaa) Triggering event #",
         ProcessImmediateEvent( this, zER_NODE_RCLICK );
         return;
      }
      else
      if ( (pRelationship = IndexOnRelationship( lIdx )) != 0 )
      {
         PositionOnZeidonRelationship( pRelationship );
// TraceLineI("(tzerctaa) Triggering event #",
         ProcessImmediateEvent( this, zER_ARC_RCLICK );
         return;
      }
      else
      {
// TraceLineI("(tzerctaa) Triggering event #",
         ProcessImmediateEvent( this, zER_SPACE_RCLICK );
         return;
      }
   }
}

// Message cracking METHOD for RButtonDblClk
void
ZER_Diagram::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
   zLONG       lIdx;
   ZER_Entity *pEntity;
   ZER_Relationship *pRelationship;

   if ( m_nRDblClk == 0 )
      return;

   m_nRDblClk = 2;
   m_ptMouseSelect = pt;

   // Get the grid index of the mouse position
   lIdx = MousePositionToIndex( pt );
   if ( lIdx <= tzERMAXGRID )
   {
      if ( (pEntity = IndexOnEntity( lIdx )) != 0 )
      {
         if ( PositionOnZeidonEntity( pEntity ) == 1 )
         {
// TraceLineI("(tzerctaa) Triggering event #", 5L
            ProcessImmediateEvent( this, zER_NODE_RDBLCLICK );
         }

         return;
      }
      else
      if ( (pRelationship = IndexOnRelationship( lIdx )) != 0 )
      {
         if ( PositionOnZeidonRelationship( pRelationship ) == 1 )
         {
// TraceLineI("(tzerctaa) Triggering event #", 10L
            ProcessImmediateEvent( this, zER_ARC_RDBLCLICK );
         }

         return;
      }
      else
      {
// TraceLineI("(tzerctaa) Triggering event #",
         ProcessImmediateEvent( this, zER_SPACE_RDBLCLICK );
         return;
      }
#if 0
      else
      {
         GetDeviceContext( );
         for ( lIdx = 0; lIdx < 40; lIdx++ )
            ColorCell( lIdx, RGB( 255, 0, 0 ) );
         for ( lIdx = 600; lIdx < 640; lIdx++ )
            ColorCell( lIdx, RGB( 255, 0, 0 ) );
         for ( lIdx = 1200; lIdx < 1240; lIdx++ )
            ColorCell( lIdx, RGB( 255, 0, 0 ) );
         for ( lIdx = 1800; lIdx < 1840; lIdx++ )
            ColorCell( lIdx, RGB( 255, 0, 0 ) );
         for ( lIdx = 0; lIdx < 40; lIdx++ )
            ColorCell( lIdx, RGB( 0, 255, 0 ) );
         for ( lIdx = 600; lIdx < 640; lIdx++ )
            ColorCell( lIdx, RGB( 0, 255, 0 ) );
         for ( lIdx = 1200; lIdx < 1240; lIdx++ )
            ColorCell( lIdx, RGB( 0, 255, 0 ) );
         for ( lIdx = 1800; lIdx < 1840; lIdx++ )
            ColorCell( lIdx, RGB( 0, 255, 0 ) );
         for ( lIdx = 0; lIdx < 40; lIdx++ )
            ColorCell( lIdx, RGB( 0, 0, 255 ) );
         for ( lIdx = 600; lIdx < 640; lIdx++ )
            ColorCell( lIdx, RGB( 0, 0, 255 ) );
         for ( lIdx = 1200; lIdx < 1240; lIdx++ )
            ColorCell( lIdx, RGB( 0, 0, 255 ) );
         for ( lIdx = 1800; lIdx < 1840; lIdx++ )
            ColorCell( lIdx, RGB( 0, 0, 255 ) );

         ReleaseDeviceContext( );
      }
#endif
   }
}

// Message cracking METHOD for MouseMove
//    m_nCapture = 1;  // Creating a new Entity
//    m_nCapture = 2;  // Moving an Entity
//    m_nCapture = 3;  // Moving a relationship
//    m_nCapture = 10  // Creating a new relationship (shift key down)
//    m_nCapture = 11  // Creating a new Entity by copy (shift key down)

void
ZER_Diagram::OnMouseMove( UINT uModKeys, CPoint pt )
{
   if ( m_nCapture )  // Are we currently capturing the mouse
   {
      CPoint ptStart;
      CPoint ptCurrent;
      CPoint ptWork;
      int   nOldROP2;

      nOldROP2 = m_hDC->SetROP2( R2_NOT );
      if ( m_ptLastMouseMove != m_ptMouseSelect )
      {
         ptStart = m_ptMouseSelect;      MakeLogicalPoint( ptStart );
         ptCurrent = m_ptLastMouseMove;  MakeLogicalPoint( ptCurrent );
         m_hDC->MoveTo( ptStart.x, ptStart.y );
         if ( m_nCapture == 1 || m_nCapture == 11 )  // creating new entity
         {
#if 0
            TraceLineI( "Erasing pt.x ", pt.x );
            TraceLineI( "Erasing pt.y ", pt.y );
            TraceLineI( "        ptStart.x ", ptStart.x );
            TraceLineI( "        ptStart.y ", ptStart.y );
            TraceLineI( "        ptCurrent.x ", ptCurrent.x );
            TraceLineI( "        ptCurrent.y ", ptCurrent.y );
            TraceLineI( "        m_ptMouseSelect.x ", m_ptMouseSelect.x );
            TraceLineI( "        m_ptMouseSelect.y ", m_ptMouseSelect.y );
            TraceLineI( "        m_ptLastMouseMove.y ", m_ptLastMouseMove.y );
#endif
            // Erase the last rectangle drawn to represent where an
            // Entity would have been positioned on the last move

            // Calculate and erase the previous rectangle
            ptWork.x = ptCurrent.x;
            ptWork.y = ptStart.y;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            m_hDC->LineTo( ptCurrent.x, ptCurrent.y );
            ptWork.x = ptStart.x;
            ptWork.y = ptCurrent.y;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            m_hDC->LineTo( ptStart.x, ptStart.y );
         }
         else
         if ( m_nCapture == 2 ) // Moving an Entity
         {
#if 0
            TraceLineI( "Moving pt.x ", pt.x );
            TraceLineI( "Moving pt.y ", pt.y );
            TraceLineI( "       ptStart.x ", ptStart.x );
            TraceLineI( "       ptStart.y ", ptStart.y );
            TraceLineI( "       ptCurrent.x ", ptCurrent.x );
            TraceLineI( "       ptCurrent.y ", ptCurrent.y );
            TraceLineI( "       m_ptMouseSelect.x ", m_ptMouseSelect.x );
            TraceLineI( "       m_ptMouseSelect.y ", m_ptMouseSelect.y );
            TraceLineI( "       m_ptLastMouseMove.y ", m_ptLastMouseMove.y );
#endif
            if ( m_pMoveEntity )
            {
               ptWork.x = ptCurrent.x - m_lMouseOffsetX;
               ptWork.y = ptCurrent.y - m_lMouseOffsetY;
               m_hDC->MoveTo( ptWork.x, ptWork.y );
               ptWork.x += (tzERCELLSIZE * m_pMoveEntity->m_lEntPortX);
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.y += (tzERCELLSIZE * m_pMoveEntity->m_lEntPortY);
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.x -= (tzERCELLSIZE * m_pMoveEntity->m_lEntPortX);
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.y -= (tzERCELLSIZE * m_pMoveEntity->m_lEntPortY);
               m_hDC->LineTo( ptWork.x, ptWork.y );
            }
         }
         else
         if ( m_nCapture == 3 ) // Moving a Relationship
         {
            ptWork.x = ptCurrent.x - m_lMouseOffsetX;
            ptWork.y = ptCurrent.y - m_lMouseOffsetY;
            m_hDC->MoveTo( ptWork.x, ptWork.y );
            ptWork.x += tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y += tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.x -= tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y -= tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
         }
         else
         if ( m_nCapture == 10 ) // Drawing a Relationship
         {
            // Erase the last line drawn to represent where
            // a relationship is going to
            m_hDC->LineTo( ptCurrent.x, ptCurrent.y );
         }
      }

      if ( pt != m_ptMouseSelect )
      {
         // Draw a new rubberband rectangle
         ptStart = m_ptMouseSelect;    MakeLogicalPoint( ptStart );
         ptCurrent = pt;               MakeLogicalPoint( ptCurrent );

         m_hDC->MoveTo( ptStart.x, ptStart.y );
         if ( m_nCapture == 1 || m_nCapture == 11 )
         {
#if 0
            TraceLineI( "New Rect pt.x ", pt.x );
            TraceLineI( "New Rect pt.y ", pt.y );
            TraceLineI( "         ptStart.x ", ptStart.x );
            TraceLineI( "         ptStart.y ", ptStart.y );
            TraceLineI( "         ptCurrent.x ", ptCurrent.x );
            TraceLineI( "         ptCurrent.y ", ptCurrent.y );
            TraceLineI( "         m_ptMouseSelect.x ", m_ptMouseSelect.x );
            TraceLineI( "         m_ptMouseSelect.y ", m_ptMouseSelect.y );
            TraceLineI( "         m_ptLastMouseMove.x ", m_ptLastMouseMove.x );
            TraceLineI( "         m_ptLastMouseMove.y ", m_ptLastMouseMove.y );
#endif
            // Draw the new rubberband rectangle.  The rectangle cannot
            // become bigger than an entity because of ClipCursor.
#if 0
            TraceLineI( "Recalibrated ptCurrent.x ", ptCurrent.x );
            TraceLineI( "             ptCurrent.y ", ptCurrent.y );
            TraceLineI( "Recalibrated pt.x ", pt.x );
            TraceLineI( "             pt.y ", pt.y );
#endif
            m_hDC->LineTo( ptCurrent.x, ptStart.y );
            m_hDC->LineTo( ptCurrent.x, ptCurrent.y );
            m_hDC->LineTo( ptStart.x, ptCurrent.y );
            m_hDC->LineTo( ptStart.x, ptStart.y );
         }
         else
         if ( m_nCapture == 2 ) // Moving an Entity
         {
#if 0
            TraceLineI( "New Rect pt.x ", pt.x );
            TraceLineI( "New Rect pt.y ", pt.y );
            TraceLineI( "         ptStart.x ", ptStart.x );
            TraceLineI( "         ptStart.y ", ptStart.y );
            TraceLineI( "         ptCurrent.x ", ptCurrent.x );
            TraceLineI( "         ptCurrent.y ", ptCurrent.y );
            TraceLineI( "         m_ptMouseSelect.x ", m_ptMouseSelect.x );
            TraceLineI( "         m_ptMouseSelect.y ", m_ptMouseSelect.y );
            TraceLineI( "         m_ptLastMouseMove.x ", m_ptLastMouseMove.x );
            TraceLineI( "         m_ptLastMouseMove.y ", m_ptLastMouseMove.y );
            TraceLineI( "         m_nMouseOffsetX ", m_nMouseOffsetX );
            TraceLineI( "         m_nMouseOffsetY ", m_nMouseOffsetY );
#endif
            if ( m_pMoveEntity )
            {
               ptWork.x = ptCurrent.x - m_lMouseOffsetX;
               ptWork.y = ptCurrent.y - m_lMouseOffsetY;
               m_hDC->MoveTo( ptWork.x, ptWork.y );
               ptWork.x += (tzERCELLSIZE * m_pMoveEntity->m_lEntPortX);
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.y += (tzERCELLSIZE * m_pMoveEntity->m_lEntPortY);
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.x -= (tzERCELLSIZE * m_pMoveEntity->m_lEntPortX);
               m_hDC->LineTo( ptWork.x, ptWork.y );
               ptWork.y -= (tzERCELLSIZE * m_pMoveEntity->m_lEntPortY);
               m_hDC->LineTo( ptWork.x, ptWork.y );
            }
         }
         else
         if ( m_nCapture == 3 ) // Moving a Relationship
         {
            ptWork.x = ptCurrent.x - m_lMouseOffsetX;
            ptWork.y = ptCurrent.y - m_lMouseOffsetY;
            m_hDC->MoveTo( ptWork.x, ptWork.y );
            ptWork.x += tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y += tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.x -= tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y -= tzERCELLSIZE;
            m_hDC->LineTo( ptWork.x, ptWork.y );
         }
         else
         if ( m_nCapture == 10 ) // Drawing a Relationship
         {
            // Draw a new line to represent where a relationship is going to
            m_hDC->LineTo( ptCurrent.x, ptCurrent.y );
         }
      }

      m_ptLastMouseMove = pt;
      m_hDC->SetROP2( nOldROP2 );
   }
}

//
// The grid has dimensions of tzERDIAGRAMX (1200) by tzERDIAGRAMY (900)
//
//    0 -----------------------------------------> 1199
//  0 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//  | ³                                              ³
//  | ³    e e e e e e e e e                         ³
//  | ³    e E N T I T Y e e                         ³
//  | ³    e M A R K I N G e                         ³
//  | ³    e e e e e e e e e                         ³
//  | ³    e e e e e e e e e                         ³
//  | ³                                              ³
//  | ³                                              ³
//  | ³                                              ³
// 899ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ(1200 x 900) - 1
//
// The x-coordinate is determined from the grid index by taking the
// grid index modulus tzERDIAGRAMX.
// The y-coordinate is determined from the grid index by taking the
// grid index divided by tzERDIAGRAMX.
//
// Each coordinate is comprised of tzERCELLSIZE (m_lCellSize == 9) pixels
// when the diagram is not scaled.  A scaling factor may be positive (smaller
// relative display sizes) or negative (larger relative display sizes).
//
// Each entity is marked in the grid for its width and height by setting
// the tziENTITY bit in the appropriate grid indices.
//
// #define  tzERDISPLAYPOS         m_pER_Diagram->m_lDisplayPos
// #define  tzERCELLSIZE           m_pER_Diagram->m_lCellSize (9)
//
// An entity's position and size are determined by the following:
//
// rect.left = (PosX( ) - PosX( tzERDISPLAYPOS )) * tzERCELLSIZE;
// rect.top = (PosY( ) - PosY( tzERDISPLAYPOS )) * tzERCELLSIZE;
// rect.right = rect.left + ((m_lEntPortX * tzERCELLSIZE) - 1);
// rect.bottom = rect.top  + ((m_lEntPortY * tzERCELLSIZE) - 1);
//
// At this point, I have no idea how the above four lines work since it does
// not seem to take the scaling factor into account (DKS - 4/2/96).  Scaling
// is done without regard to the dimensions of the grid (DKS - 10/20/99).
//

// Message cracking METHOD for HScroll
void
ZER_Diagram::OnHScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
   zLONG  lWk;
   zLONG  lScroll;

#ifdef DEBUG_ALL
   TraceLineI( "ZER_Diagram::HScroll uSBCode", uSBCode );
#endif

   // SB_PAGELEFT, SB_PAGERIGHT, SB_THUMBPOSITION to LOWORD( lParam ),
   // SB_LEFT and SB_RIGHT (far left and right)
   switch ( uSBCode )
   {
      case SB_LINELEFT:
//       if ( PosX( m_lDisplayPos ) >= tzERENTITYX )
         lWk = PosX( m_lDisplayPos );
         if ( lWk > 0 )
         {
            if ( lWk >= tzERENTITYX )
            {
               m_lDisplayPos -= tzERENTITYX;
               m_lDisplayPosMax -= tzERENTITYX;
               lScroll = tzERCELLSIZE * tzERENTITYX;
            }
            else
            {
               m_lDisplayPos = 0;
               m_lDisplayPosMax -= lWk;
               lScroll = tzERCELLSIZE * lWk;
            }

            if ( m_lScale )
               lScroll = (lScroll * (m_lScale + 100)) / 100;

            ScrollWindow( lScroll, 0, 0, 0 );
            SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
            UpdateWindow( );
            m_bScrolledWindow = TRUE;
         }

         break;

      case SB_PAGELEFT:
         lWk = PosX( m_lDisplayPosMax ) - PosX( m_lDisplayPos );
         if ( lWk > 2 )
            lWk -= 2;

         if ( lWk > PosX( m_lDisplayPos ) )
            lWk = PosX( m_lDisplayPos );

         m_lDisplayPos -= lWk;
         m_lDisplayPosMax -= lWk;
         lScroll = tzERCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( lScroll, 0, 0, 0 );
         SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
         break;

      case SB_LINERIGHT:
         if ( PosX( m_lDisplayPosMax ) < (tzERDIAGRAMX - (tzERENTITYX + 1)) )
         {
            m_lDisplayPos += tzERENTITYX;
            m_lDisplayPosMax += tzERENTITYX;
            lScroll = -tzERCELLSIZE * tzERENTITYX;
            if ( m_lScale )
               lScroll = (lScroll * (m_lScale + 100)) / 100;

            ScrollWindow( lScroll, 0, 0, 0 );
            SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
            UpdateWindow( );
            m_bScrolledWindow = TRUE;
         }

         break;

      case SB_PAGERIGHT:
         lWk = PosX( m_lDisplayPosMax ) - PosX( m_lDisplayPos );
         if ( lWk > 2 )
            lWk -= 2;

         if ( (lWk + PosX( m_lDisplayPosMax )) >= tzERDIAGRAMX )
            lWk = tzERDIAGRAMX - (PosX( m_lDisplayPosMax ) + 1);

         m_lDisplayPos += lWk;
         m_lDisplayPosMax += lWk;
         lScroll = -tzERCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( lScroll, 0, 0, 0 );
         SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         lWk = uThumbPos;
         lWk -= PosX( m_lDisplayPos );
         m_lDisplayPos    += lWk;
         m_lDisplayPosMax += lWk;
         lScroll = -tzERCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( lScroll, 0, 0, 0 );
         SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
         UpdateWindow( );
         m_bScrolledWindow = TRUE;
         break;

      default:
         break;
   }
}

// Message cracking METHOD for VScroll
void
ZER_Diagram::OnVScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
   zLONG  lWk;
   zLONG  lScroll;

#ifdef DEBUG_ALL
   TraceLineI( "ZER_Diagram::VScroll uSBCode", uSBCode );
#endif

   // SB_PAGEDOWN, SB_PAGEUP, SB_THUMBPOSITION to LOWORD(lParam),
   // SB_THUMBTRACK to LOWORD(lParam),
   // SB_BOTTOM and SB_TOP (far top and bottom)
   switch ( uSBCode )
   {
      case SB_LINEUP:
//       if ( PosY( m_lDisplayPos ) >= tzERENTITYY )
         lWk = PosY( m_lDisplayPos );
         if ( lWk )
         {
            if ( lWk >= tzERENTITYY )
            {
               m_lDisplayPos -= (tzERENTITYY * tzERDIAGRAMX);
               m_lDisplayPosMax -= (tzERENTITYY * tzERDIAGRAMX);
               lScroll = tzERCELLSIZE * tzERENTITYY;
            }
            else
            {
               m_lDisplayPos = 0;
               m_lDisplayPosMax -= lWk;
               lScroll = tzERCELLSIZE * lWk;
            }

            if ( m_lScale )
               lScroll = (lScroll * (m_lScale + 100)) / 100;

            ScrollWindow( 0, lScroll, 0, 0 );
            SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
            UpdateWindow( );
            m_bScrolledWindow = TRUE;
         }

         break;

      case SB_PAGEUP:
         lWk = PosY( m_lDisplayPosMax ) - PosY( m_lDisplayPos );
         if ( lWk > 2 )
            lWk -= 2;

         if ( lWk > PosY( m_lDisplayPos ) )
            lWk = PosY( m_lDisplayPos );

         m_lDisplayPos -= lWk * tzERDIAGRAMX;
         m_lDisplayPosMax -= lWk * tzERDIAGRAMX;
         lScroll = tzERCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( 0, lScroll, 0, 0 );
         SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
         break;

      case SB_LINEDOWN:
         if ( PosY( m_lDisplayPosMax ) < (tzERDIAGRAMY - (tzERENTITYY + 1)) )
         {
            m_lDisplayPos += (tzERENTITYY * tzERDIAGRAMX);
            m_lDisplayPosMax += (tzERENTITYY * tzERDIAGRAMX);
            lScroll = -tzERCELLSIZE * tzERENTITYY;
            if ( m_lScale )
               lScroll = (lScroll * (m_lScale + 100)) / 100;

            ScrollWindow( 0, lScroll, 0, 0 );
            SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
            UpdateWindow( );
            m_bScrolledWindow = TRUE;
         }

         break;

      case SB_PAGEDOWN:
         lWk = PosY( m_lDisplayPosMax ) - PosY( m_lDisplayPos );
         if ( lWk > 2 )
            lWk -= 2;

         if ( lWk + PosY( m_lDisplayPosMax ) >= tzERDIAGRAMY )
            lWk = tzERDIAGRAMY - (PosY( m_lDisplayPosMax ) + 1);

         m_lDisplayPos += lWk * tzERDIAGRAMX;
         m_lDisplayPosMax += lWk * tzERDIAGRAMX;
         lScroll = -tzERCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( 0, lScroll, 0, 0 );
         SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         lWk = uThumbPos;
         lWk -= PosY( m_lDisplayPos );
         m_lDisplayPos    += lWk * tzERDIAGRAMX;
         m_lDisplayPosMax += lWk * tzERDIAGRAMX;
         lScroll = -tzERCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( 0, lScroll, 0, 0 );
         SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
         UpdateWindow( );
         m_bScrolledWindow = TRUE;
         break;

      default:
         break;
   }
}

// Message cracking METHOD for MouseWheel
BOOL
ZER_Diagram::OnMouseWheel( UINT flags, short nDeltaIn, CPoint pt )
{
    BOOL bUp = TRUE;
    int nDelta = nDeltaIn;
    if ( nDeltaIn < 0 )
    {
        bUp = FALSE;
        nDelta = - nDelta;
    }
    UINT uWheelScrollLines;

    // get the number of wheel scroll lines from the OS. Note that Win95 doesn't support this call
    ::SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &uWheelScrollLines, 0 );

    if ( uWheelScrollLines == WHEEL_PAGESCROLL ) // scroll a page at a time
    {
        SendMessage( WM_VSCROLL,MAKEWPARAM( bUp ? SB_PAGEUP : SB_PAGEDOWN, 0 ), 0 );
    }
    else  // or scroll numlines
    {
        int nLineCnt = (nDelta * uWheelScrollLines) / WHEEL_DELTA;
        while ( nLineCnt-- )
            SendMessage( WM_VSCROLL, MAKEWPARAM( bUp ? SB_LINEUP : SB_LINEDOWN, 0 ), 0 );
    }

    return( TRUE );
}

// Message cracking METHOD for WM_WINDOWPOSCHANGED
void
ZER_Diagram::OnWindowPosChanged( WINDOWPOS *pwp )
{
   CRect  rect;

   Attr.X = pwp->x;
   Attr.Y = pwp->y;
   Attr.W = pwp->cx;
   Attr.H = pwp->cy;
   rect.left = 0;
   rect.top = 0;
   rect.right = pwp->cx;
   rect.bottom = pwp->cy;
// GetClientRect( rect );
   SetDisplayBoundaries( rect );
   CalculateMinimumScale( rect );
   SetScrollBars( );
}

void
ZER_Diagram::OnPaint( )
{
   CRect  rect;

   if ( GetUpdateRect( rect, 1 ) )
   {
   // TraceRect( "ZER_Diagram::OnPaint", rect );

      zSHORT nOldMapMode;
      CSize  sizeOldWExt;
      CSize  sizeOldVExt;

      CDC *pSaveDC = m_hDC;
      m_hDC = new CPaintDC( this );

      // Now set the update region in the class from the rect to be painted.
      SetDisplayBoundaries( rect, 1 );
      if ( m_lScale )
      {
         nOldMapMode = m_hDC->SetMapMode( MM_ISOTROPIC );
         sizeOldWExt = m_hDC->SetWindowExt( CSize( 100, 100 ) );
         sizeOldVExt = m_hDC->SetViewportExt( CSize( 100 + m_lScale,
                                                     100 + m_lScale ) );
      }

      DrawDiagram( );

      if ( m_lScale )
      {
         nOldMapMode = m_hDC->SetMapMode( nOldMapMode );
         sizeOldWExt = m_hDC->SetViewportExt( CSize( sizeOldWExt ) );
         sizeOldVExt = m_hDC->SetViewportExt( CSize( sizeOldVExt ) );
      }

      delete( m_hDC );
      m_hDC = pSaveDC;
   }

   m_lUpdateDisplayPos = -1;
   m_lUpdateDisplayPosMax = -1;
}
