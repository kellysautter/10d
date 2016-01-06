/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlerl.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Entity/Relationship control's
// ZER_Relationship helper class.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2000.04.28    DKS    Z10    TB51282
//    Fixed endless loop for FH.
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

// Class Implementation for class ZER_Relationship
ZER_Relationship::ZER_Relationship( ZER_Diagram *pER_Diagram,
                                    ZER_Entity  *pSrcEntity,
                                    ZER_Entity  *pTgtEntity,
                                    zPCHAR      pchForwardText,
                                    zPCHAR      pchReverseText,
                                    zBOOL       bSrcReq,
                                    zBOOL       bSrcMany,
                                    zBOOL       bTgtReq,
                                    zBOOL       bTgtMany,
                                    zLONG       lPosZKey,
                                    zLONG       lStartIdx,
                                    zLONG       lEndIdx,
                                    zLONG       lArcIdx,
                                    zBOOL       bSelect,
                                    zLONG       lFlags )
{
   m_pER_Diagram = pER_Diagram;
   m_pSrcEntity = pSrcEntity;
   m_pTgtEntity = pTgtEntity;
   m_uLPOffsetF = 0;
   if ( pchForwardText && *pchForwardText )
   {
      m_csTextF = pchForwardText;
      m_uLPOffsetF = m_csTextF.GetLength( );
      while ( m_uLPOffsetF && pchForwardText[ m_uLPOffsetF ] != '(' )
         m_uLPOffsetF--;

      if ( pchForwardText[ m_uLPOffsetF ] != '(' )
         m_uLPOffsetF = (zSHORT) zstrlen( pchForwardText );
   }

   m_uLPOffsetR = 0;
   if ( pchReverseText )
   {
      m_csTextR = pchReverseText;
      m_uLPOffsetR = (zSHORT) zstrlen( pchReverseText );
      while ( m_uLPOffsetR && pchReverseText[ m_uLPOffsetR ] != '(' )
         m_uLPOffsetR--;

      if ( pchReverseText[ m_uLPOffsetR ] != '(' )
         m_uLPOffsetR = (zSHORT) zstrlen( pchReverseText );
   }

   m_bSrcReq = bSrcReq;
   m_bSrcMany = bSrcMany;
   m_bTgtReq = bTgtReq;
   m_bTgtMany = bTgtMany;
   m_lPosZKey = lPosZKey;
   m_bSelected = bSelect;
   m_bLineDrawn = 0;
   m_lStartIdx = lStartIdx;
   m_lEndIdx = lEndIdx;
   m_lER_Flags = lFlags;

   // If there is an Arc index, see if the relationship is recursive and
   // calculate the 3 arcs, otherwise simply set ArcIdx1.
   if ( lArcIdx )
   {
      if ( m_pSrcEntity == m_pTgtEntity )
      {
         // The relationship is recursive, calculate the two additional
         // arcs from the one given.
         m_lArcIdx2 = lArcIdx;
         m_nArcs = 3;

         // Upper side of entity.
         if ( PosY( lArcIdx ) < PosY( lStartIdx ) )
         {
            if ( PosX( lArcIdx ) > PosX( lStartIdx ) )
            {
               // Upper right corner.
               if ( PosX( lStartIdx ) < PosX( lEndIdx ) )
               {
                  m_lArcIdx1 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lStartIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lEndIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
               }
               else
               {
                  m_lArcIdx1 = ((zLONG) PosY( lStartIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lEndIdx );
               }
            }
            else
            {
               // Upper left corner.
               if ( PosX( lStartIdx ) < PosX( lEndIdx ) )
               {
                  m_lArcIdx1 = ((zLONG) PosY( lStartIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lEndIdx );
               }
               else
               {
                  m_lArcIdx1 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lStartIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lEndIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
               }
            }
         }
         else
         {
            // Lower side of entity.
            if ( PosX( lArcIdx ) > PosX( lStartIdx ) )
            {
               // Lower right corner.
               if ( PosX( lStartIdx ) < PosX( lEndIdx ) )
               {
                  m_lArcIdx1 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lStartIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lEndIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
               }
               else
               {
                  m_lArcIdx1 = ((zLONG) PosY( lStartIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lEndIdx );
               }
            }
            else
            {
               // Lower left corner.
               if ( PosX( lStartIdx ) < PosX( lEndIdx ) )
               {
                  m_lArcIdx1 = ((zLONG) PosY( lStartIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lEndIdx );
               }
               else
               {
                  m_lArcIdx1 = ((zLONG) PosY( lArcIdx ) * tzERDIAGRAMX) +
                               PosX( lStartIdx );
                  m_lArcIdx3 = ((zLONG) PosY( lEndIdx ) * tzERDIAGRAMX) +
                               PosX( lArcIdx );
               }
            }
         }
      }
      else
      {
         m_nArcs = 1;
         m_lArcIdx1 = lArcIdx;
         m_lArcIdx2 = 0;
         m_lArcIdx3 = 0;
      }
   }
   else
   {
      m_nArcs = 0;
      m_lArcIdx1 = 0;
      m_lArcIdx2 = 0;
      m_lArcIdx3 = 0;
   }

   // If a line has not been calculated, do it now.
   if ( lStartIdx == 0 )
      CalculateLine( );

   if ( lStartIdx )
   {
      // If a relationship has been calculated, mark the relationship in the
      // grid so other calculations will not conflict with the relationship.
      MarkRelationship( );
   }

   m_lTextIdx = 0;
   m_nTextLth = 0;
   m_nTextDir = 0;
   m_bTextDrawn = 0;
   m_bUpdated = 0;
}

ZER_Relationship::~ZER_Relationship( )
{
}

#define zX_AXIS   1
#define zY_AXIS   2

//
// METHOD: ZER_Relationship::CalculateLine
//
//
zSHORT
ZER_Relationship::CalculateLine( zBOOL bDisplayMsgBox, zSHORT nErrCnt )
{
   zSHORT nSP, nTP;
   zLONG  lSPIdx, lTPIdx;
   zLONG  lAIdx1, lAIdx2, lAIdx3;
   zSHORT nSrcAxis;
   zLONG  lImpact, lMinImpact;
   zSHORT nBestAxis;
   zLONG  lBestSP, lBestTP;
   zLONG  lBestArc1, lBestArc2, lBestArc3;
   zLONG  lPortX;
   zLONG  lPortY;

#if 0
// if ( (zstrcmp( m_pSrcEntity->m_csText, "Supplier" ) == 0 ||
//       zstrcmp( m_pTgtEntity->m_csText, "Supplier" ) == 0) &&
//      (zstrcmp( m_pSrcEntity->m_csText, "SupplierReturn" ) == 0 ||
//       zstrcmp( m_pTgtEntity->m_csText, "SupplierReturn" ) == 0) )
   {
      zCHAR szMessage[ 256 ];

      wsprintf( szMessage,
                "(TzCtlERL) CalculateLine between"
                " Src: %s  and Tgt: %s  at"
                " SrcPos(%ld,%ld) TgtPos(%ld,%ld) Ports: S%ld,%ld T%ld,%ld",
                m_pSrcEntity->m_csText,
                m_pTgtEntity->m_csText,
                PosX( m_pSrcEntity->m_lPosIdx ),
                PosY( m_pSrcEntity->m_lPosIdx ),
                PosX( m_pTgtEntity->m_lPosIdx ),
                PosY( m_pTgtEntity->m_lPosIdx ),
                m_pSrcEntity->m_lEntPortX,
                m_pSrcEntity->m_lEntPortY,
                m_pTgtEntity->m_lEntPortX,
                m_pTgtEntity->m_lEntPortY );
      TraceLineS( szMessage, " entities ..." );
   }
#endif

   // We have two entities in hand and their respective starting points,
   // now we must determine where to draw the lines. If the entities are
   // overlapping on the same X or Y plane, then we can only draw a straight
   // line between them, otherwise, we determine the best arc'd line we can
   // to draw between them.
   if ( m_bLineDrawn )
      Erase( );
   else
   if ( m_lStartIdx )
   {
      MarkRelationship( 1 );
      m_lStartIdx = 0;
   }

   // The line needs to have a bend, analyze all possible port combinations
   // to get the best ports for source and target drawing.
   lMinImpact = tzERBADPATH;
   nSrcAxis = zX_AXIS;
   if ( m_pSrcEntity == m_pTgtEntity ||
        PosY( m_pSrcEntity->m_lPosIdx ) != PosY( m_pTgtEntity->m_lPosIdx ) )
   {
      lPortX = (m_pSrcEntity->m_lEntPortX) >> 1;   // divide the number
      lPortY = (m_pTgtEntity->m_lEntPortY) >> 1;   // of ports by 2

//    for ( nSP = 0; nSP <= tzERMAXPORTX; )              DKS 3/29/96
      for ( nSP = 0; nSP <= lPortX; )
      {
//       for ( nTP = 0; nTP <= tzERMAXPORTY; )           DKS 3/29/96
         for ( nTP = 0; nTP <= lPortY; )
         {
            lImpact = AnalyzeLine( &lSPIdx, &lTPIdx,
                                   &lAIdx1, &lAIdx2, &lAIdx3,
                                   nSP, nTP, nSrcAxis, lPortX, lPortY );
            if ( lImpact < lMinImpact )
            {
               lMinImpact = lImpact;
               nBestAxis = nSrcAxis;
               lBestSP = lSPIdx;
               lBestTP = lTPIdx;
               lBestArc1 = lAIdx1;
               lBestArc2 = lAIdx2;
               lBestArc3 = lAIdx3;
            }

//          if ( lMinImpact < tzERLINEABUTSENTITY )      DKS 3/5/96
            if ( lMinImpact == 0 )
               break;

            if ( nTP > 0 )
               nTP = -nTP;
            else
            {
               nTP = -nTP;
               nTP++;
            }
         }

         if ( lMinImpact == 0 )
            break;

         if ( nSP > 0 )
            nSP = -nSP;
         else
         {
            nSP = -nSP;
            nSP++;
         }
      }
   }

   // If we have not found a clean line from the source to the target, try
   // switching the source to the closest Y side and go to the targets X side.
   if ( lMinImpact && (m_pSrcEntity == m_pTgtEntity ||
        PosX( m_pSrcEntity->m_lPosIdx ) != PosX( m_pTgtEntity->m_lPosIdx )) )
   {
      nSrcAxis = zY_AXIS;
      lPortY = (m_pSrcEntity->m_lEntPortY) >> 1;
      lPortX = (m_pTgtEntity->m_lEntPortX) >> 1;

//    for ( nSP = 0; nSP <= tzERMAXPORTY; )              DKS 3/29/96
      for ( nSP = 0; nSP <= lPortY; )
      {
//       for ( nTP = 0; nTP <= tzERMAXPORTX; )           DKS 3/29/96
         for ( nTP = 0; nTP <= lPortX; )
         {
            lImpact = AnalyzeLine( &lSPIdx, &lTPIdx,
                                   &lAIdx1, &lAIdx2, &lAIdx3,
                                   nSP, nTP, nSrcAxis, lPortX, lPortY );
            if ( lImpact < lMinImpact )
            {
               lMinImpact = lImpact;
               nBestAxis = nSrcAxis;
               lBestSP = lSPIdx;
               lBestTP = lTPIdx;
               lBestArc1 = lAIdx1;
               lBestArc2 = lAIdx2;
               lBestArc3 = lAIdx3;
            }

//          if ( lMinImpact < tzERLINEABUTSENTITY )      DKS 3/5/96
            if ( lMinImpact == 0 )
               break;

            if ( nTP > 0 )
               nTP = -nTP;
            else
            {
               nTP = -nTP;
               nTP++;
            }
         }

         if ( lMinImpact == 0 )
            break;

         if ( nSP > 0 )
            nSP = -nSP;
         else
         {
            nSP = -nSP;
            nSP++;
         }
      }
   }

   // Now convert the best positions found to X,Y.
   if ( lMinImpact < tzERBADPATH )
   {
      m_lStartIdx = lBestSP;
      m_lEndIdx = lBestTP;
      m_lArcIdx1 = lBestArc1;
      m_lArcIdx2 = lBestArc2;
      m_lArcIdx3 = lBestArc3;
      if ( m_pSrcEntity == m_pTgtEntity )
         m_nArcs = 3;
      else
      if ( PosX( m_lStartIdx ) == PosX( m_lEndIdx ) ||
           PosY( m_lStartIdx ) == PosY( m_lEndIdx ) )
      {
         m_nArcs = 0;
      }
      else
         m_nArcs = 1;

      MarkRelationship( );

      // Since a relationship was successfully calculated, invalidate
      // the text position in the event one is hanging around from
      // a turning off of text drawing.
      m_lTextIdx = 0;
      m_nTextLth = 0;
      m_nTextDir = 0;
   }
   else
   {
      // We could not find a place to put the relationship line,
      // punt and give up.
   //xzCHAR szMessage[ 256 ];

      m_lStartIdx = 0;
      m_lEndIdx = 0;
      m_lArcIdx1 = 0;
      m_lArcIdx2 = 0;
      m_lArcIdx3 = 0;
      m_nArcs = 0;
   // if ( nErrCnt == 0 )  removed DonC  2007.01.23
   // {
   //    wsprintf( szMessage,
   //              "(TzCtlERL) Error calculating relationship between"
   //              " source: %s  and target: %s",
   //              m_pSrcEntity->m_csText,
   //              m_pTgtEntity->m_csText );
   //    TraceLineS( szMessage, " entities!!!" );
   // }

   //xstrcpy_s( szMessage, sizeof( szMessage ), m_pSrcEntity->m_csText );
   //xstrcat_s( szMessage, sizeof( szMessage ), "/" );
   //xstrcat_s( szMessage, sizeof( szMessage ), m_pTgtEntity->m_csText );
   //xstrcat_s( szMessage, sizeof( szMessage ), " - Error calculating relationship!" );
   //xMB_SetMessage( m_pER_Diagram->m_pZSubtask->m_vDialog, 1, szMessage );

   // if ( bDisplayMsgBox )
   // {
   //    MessageSend( m_pER_Diagram->m_pZSubtask->m_vDialog,
   //                 "ER00XXX", "E/R Model Maintainance", szMessage,
   //                 zMSGQ_OBJECT_CONSTRAINT_ERROR, FALSE );
   // }
   }

   // Flag the relationship as updated.
   m_bUpdated = 1;
   return( (lMinImpact < tzERBADPATH) ? 0 : 1 );
}

//
// METHOD: ZER_Relationship::AnalyzeLine
//
// PURPOSE: This method analyzes a line between a source and target
//          entity and returns an impact analysis based on the number
//          of cross-overs and entities through which the line must
//          run.  If the line either arcs in a cell which has a line in
//          it OR runs through a cell in the same direction as another
//          line already in the cell, the return value is tzERBADPATH
//          (NOT A GOOD LINE).  Impact points are added as follows:
//
//             tzERLINEABUTSLINE      1
//             tzERLINEABUTSTEXT      1
//             tzERLINEBENDS          3
//             tzERLINECROSSESTEXT    100
//             tzERLINECROSSESLINE    100
//             tzERLINEABUTSENTITY    200
//             tzERLINECROSSESENTITY  300
//

#ifdef DEBUG_ALL
zPCHAR g_Collision[ ] =
   {
      "Recursive arc1",
      "Recursive Arc Limit1",
      "Arc = Src or Tgt Port1",
      "Arc in Src entity1",
      "Arc collision1a",
      "Vertical Line collision1",
      "Horizontal Line collision1",
      "Arc collision1b",
      "Line collision1",
      "Arc at Line or Arc1",
      "Recursive arc2",
      "Recursive Arc Limit2",
      "Arc = Src or Tgt Port2",
      "Arc in Src entity2",
      "Arc collision2a",
      "Vertical Line collision2",
      "Horizontal Line collision2",
      "Arc collision2b",
      "Line collision2",
      "Arc at Line or Arc2",
   };
#endif

zLONG
ZER_Relationship::AnalyzeLine( zPLONG  lpSPIdx,       //
                               zPLONG  lpTPIdx,       //
                               zPLONG  lpArcIdx1,
                               zPLONG  lpArcIdx2,
                               zPLONG  lpArcIdx3,
                               zSHORT  nSP,           // source port
                               zSHORT  nTP,           // target port
                               zSHORT  nSrcAxis,      // zX_AXIS or zY_AXIS
                               zLONG   lPortX,
                               zLONG   lPortY )
{
   zLONG  lRC;
   zLONG  lSPIdx, lTPIdx;
   zLONG  lArcIdx1, lArcIdx2, lArcIdx3;
   zLONG  lWkIdx, lNextIdx;
   zSHORT nArcsChecked;
   zBOOL  bRecursive;
   zSHORT nDirection;
   zPCHAR pchGrid = m_pER_Diagram->m_pchGrid;

   // ASSUME the line will bend (until shown it can be straight).
   lRC = tzERLINEBENDS;
   if ( nSP )
   {
      if ( nSP > 0 )
         lRC += nSP;
      else
         lRC -= nSP;

      lRC++;
   }

   if ( nTP )
   {
      if ( nTP > 0 )
         lRC += nTP;
      else
         lRC -= nTP;

      lRC++;
   }

   nArcsChecked = 0;
   bRecursive = (m_pSrcEntity == m_pTgtEntity);

   // Initialize 2nd and third Arcs.
   lArcIdx2 = 0;
   lArcIdx3 = 0;

   // Calculate the source and target port positions on the grid.
   if ( nSrcAxis == zX_AXIS )  // if source entity on X Axis
   {
      // Calculate the source ports index position on the grid.
//    lSPIdx = m_pSrcEntity->m_lPosIdx + (tzERENTITYX / 2) + nSP;
      lSPIdx = m_pSrcEntity->m_lPosIdx +
                                 (m_pSrcEntity->m_lEntPortX / 2) + nSP;
      if ( m_pTgtEntity->PosY( ) > m_pSrcEntity->PosY( ) )
         lSPIdx += ((zLONG) m_pSrcEntity->m_lEntPortY * tzERDIAGRAMX);
      else
         lSPIdx -= tzERDIAGRAMX;

      // If the line is recursive, set the Target Port to the Source Port.
      if ( bRecursive )
      {
         if ( (nSP > -2 && nSP < 2) || nSP > lPortY || nSP < -lPortY )
           // nSP > tzERMAXPORTY || nSP < -tzERMAXPORTY )   DKS 10/18/99
         {
            lRC = -1;
            goto BADPATH_tag;
         }

         if ( nSP > 0 )
            nTP = -nSP;
         else
            nTP = nSP;
      }

      // Calculate the target ports index position on the grid.
      lTPIdx = m_pTgtEntity->m_lPosIdx +
               ((zLONG) (m_pTgtEntity->m_lEntPortY / 2) * tzERDIAGRAMX) +
               ((zLONG) nTP * tzERDIAGRAMX);

      if ( m_pSrcEntity->PosX( ) > m_pTgtEntity->PosX( ) ||
           (bRecursive && nSP > 0) )
      {
         lTPIdx += m_pTgtEntity->m_lEntPortX;
      }
      else
      {
         lTPIdx--;
      }

      // If the line is recursive, calculate 3 arcs.
      if ( bRecursive )
      {
         if ( nSP > 0 )
         {
            lArcIdx1 = lSPIdx - ((zLONG) tzERRECURSSZ * tzERDIAGRAMX);
            lArcIdx2 = m_pSrcEntity->m_lPosIdx + tzERENTITYX + tzERRECURSSZ -
                               ((zLONG) (tzERRECURSSZ + 1) * tzERDIAGRAMX);
            lArcIdx3 = lTPIdx + tzERRECURSSZ;
         }
         else
         {
            lArcIdx1 = lSPIdx - ((zLONG) tzERRECURSSZ * tzERDIAGRAMX);
            lArcIdx2 = m_pSrcEntity->m_lPosIdx - ((tzERRECURSSZ + 1) +
                             ((zLONG) (tzERRECURSSZ + 1) * tzERDIAGRAMX));
            lArcIdx3 = lTPIdx - tzERRECURSSZ;
         }

         if ( lArcIdx1 < 0 || lArcIdx1 > tzERMAXGRID ||
              lArcIdx2 < 0 || lArcIdx2 > tzERMAXGRID ||
              lArcIdx3 < 0 || lArcIdx3 > tzERMAXGRID ||
              PosY( lArcIdx1 ) != PosY( lArcIdx2 ) )
         {
            lRC = -2;
            goto BADPATH_tag;
         }
      }
      else
      {
         // Calculate the arcs index position on the grid.
         lArcIdx1 = lSPIdx;
         if ( m_pSrcEntity->PosX( ) > m_pTgtEntity->PosX( ) )
         {
            while ( lArcIdx1 > lTPIdx )
               lArcIdx1 -= tzERDIAGRAMX;

            while ( lArcIdx1 < lTPIdx )
               lArcIdx1 += tzERDIAGRAMX;
         }
         else
         {
            while ( lArcIdx1 < lTPIdx )
               lArcIdx1 += tzERDIAGRAMX;

            while ( lArcIdx1 > lTPIdx )
               lArcIdx1 -= tzERDIAGRAMX;
         }
      }

      // If the arc position is the same as either the source or target
      // port, then fail the line.
      if ( lArcIdx1 == lSPIdx || lArcIdx1 == lTPIdx )
      {
         lRC = -3;
         goto BADPATH_tag;
      }

      // If the arc position is in an entity, see if it is in either the
      // source or target entity, if it is in the source entity, fail
      // the line, if it is in the target entity, convert the line to a
      // straight line for analysis.
      if ( pchGrid[ lArcIdx1 ] & tziENTITY )
      {
         if ( m_pSrcEntity->IndexInEntity( lArcIdx1 ) )
         {
            lRC = -4;
            goto BADPATH_tag;
         }
         else
         if ( m_pTgtEntity->IndexInEntity( lArcIdx1 ) )
         {
            if ( lArcIdx1 > lSPIdx )
            {
               if ( lArcIdx1 > ( lSPIdx + tzERDIAGRAMX ))
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1 -= tzERDIAGRAMX;
               }
               else
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1--;
               }
            }
            else
            {
               if ( lArcIdx1 < ( lSPIdx - tzERDIAGRAMX ))
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1 += tzERDIAGRAMX;
               }
               else
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1++;
               }
            }

            lTPIdx = lArcIdx1;
            nTP = nSP;
            lRC -= tzERLINEBENDS;  // OOPS, the line is straight
         }
      }

      // We have the source, target and arc index on the grid, now analyze
      // a line from the source to the target.
      lWkIdx = lSPIdx;
      for ( ; ; )
      {
         if ( nArcsChecked == 0 )
         {
            lNextIdx = lArcIdx1;
         }
         else
         {
            if ( lWkIdx == lArcIdx2 )
               lNextIdx = lArcIdx3;
            else
            if ( lArcIdx2 == 0 || lWkIdx == lArcIdx3 )
               lNextIdx = lTPIdx;
            else
               lNextIdx = lArcIdx2;
         }

         if ( PosX( lWkIdx ) == PosX( lNextIdx ) )
            nDirection = tzVERTICAL;
         else
            nDirection = tzHORIZONTAL;

         while ( lWkIdx > lNextIdx )
         {
            if ( pchGrid[ lWkIdx ] & tziARC )
            {
               lRC = -5;
               goto BADPATH_tag;
            }

            if ( pchGrid[ lWkIdx ] & (tziLINE | tziTEXTVERT | tziTEXTHORIZ) )
            {
               if ( (pchGrid[ lWkIdx ] & (tziVERT | tziTEXTVERT)) &&
                                             nDirection == tzVERTICAL )
               {
                  lRC = -6;
                  goto BADPATH_tag;
               }
               else
               if ( (pchGrid[ lWkIdx ] & (tziHORIZ | tziTEXTHORIZ)) &&
                                             nDirection == tzHORIZONTAL )
               {
                  lRC = -7;
                  goto BADPATH_tag;
               }
               else
               if ( pchGrid[ lWkIdx ] & tziLINE )
                  lRC += tzERLINECROSSESLINE;
               else
                  lRC += tzERLINECROSSESTEXT;
            }
            else
            if ( pchGrid[ lWkIdx ] & tziENTITY )
               lRC += tzERLINECROSSESENTITY;
            else
            {
               if ( nDirection == tzHORIZONTAL )
               {
                  if ( lWkIdx > tzERDIAGRAMX )
                  {
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziENTITY )
                        lRC += tzERLINEABUTSENTITY;
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] &
                                                         (tziLINE | tziARC) )
                     {
                        lRC += tzERLINEABUTSLINE;
                     }
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziTEXT )
                     {
                        lRC += tzERLINEABUTSTEXT;
                     }
                  }
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
               else
               {
                  if ( pchGrid[ lWkIdx - 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;

                  if ( pchGrid[ lWkIdx + 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
            }

            if ( nDirection == tzVERTICAL )
               lWkIdx -= tzERDIAGRAMX;
            else
               lWkIdx--;
         }

         while ( lWkIdx < lNextIdx )
         {
            if ( pchGrid[ lWkIdx ] & tziARC )
            {
               lRC = -8;
               goto BADPATH_tag;
            }

            if ( pchGrid[ lWkIdx ] & (tziLINE | tziTEXTVERT | tziTEXTHORIZ) )
            {
               if ( pchGrid[ lWkIdx ] & (tziVERT | tziTEXTVERT) &&
                                             nDirection == tzVERTICAL ||
                    pchGrid[ lWkIdx ] & (tziHORIZ | tziTEXTHORIZ) &&
                                             nDirection == tzHORIZONTAL )
               {
                  lRC = -9;
                  goto BADPATH_tag;
               }
               else
               if ( pchGrid[ lWkIdx ] & tziLINE )
                  lRC += tzERLINECROSSESLINE;
               else
                  lRC += tzERLINECROSSESTEXT;
            }
            else
            if ( pchGrid[ lWkIdx ] & tziENTITY )
               lRC += tzERLINECROSSESENTITY;
            else
            {
               if ( nDirection == tzHORIZONTAL )
               {
                  if ( lWkIdx > tzERDIAGRAMX )
                  {
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziENTITY )
                        lRC += tzERLINEABUTSENTITY;
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] &
                                                       (tziLINE | tziARC) )
                     {
                        lRC += tzERLINEABUTSLINE;
                     }
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziTEXT )
                     {
                        lRC += tzERLINEABUTSTEXT;
                     }
                  }

                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
               else
               {
                  if ( pchGrid[ lWkIdx - 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;

                  if ( pchGrid[ lWkIdx + 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
            }

            if ( nDirection == tzVERTICAL )
               lWkIdx += tzERDIAGRAMX;
            else
               lWkIdx++;
         }

         if ( lWkIdx == lTPIdx )
            break;

         // We should be at the arc, make sure the cell is clean.
         if ( pchGrid[ lWkIdx ] & (tziLINE | tziARC) )
         {
            lRC = -10;
            goto BADPATH_tag;
         }

         if ( pchGrid[ lWkIdx ] & tziENTITY )
            lRC += tzERLINECROSSESENTITY;

         nArcsChecked++;
      }
   }
   else
   {
      // Calculate the source ports index position on the grid.
      lSPIdx = m_pSrcEntity->m_lPosIdx +
               ((zLONG) (m_pSrcEntity->m_lEntPortY / 2) * tzERDIAGRAMX);
      if ( nSP )
         lSPIdx += ((zLONG) nSP * tzERDIAGRAMX);

      if ( m_pTgtEntity->PosX( ) > m_pSrcEntity->PosX( ) )
         lSPIdx += m_pSrcEntity->m_lEntPortX;
      else
         lSPIdx--;

      // If the line is recursive, set the Source Port to the Target
      // port and calculate 3 arcs.
      if ( bRecursive )
      {
         if ( (nSP > -2 && nSP < 2) || nSP > lPortY || nSP < -lPortY )
           // nSP > tzERMAXPORTY || nSP < -tzERMAXPORTY )   DKS 10/18/99
         {
            lRC = -11;
            goto BADPATH_tag;
         }

         nTP = nSP;
         if ( nSP > 0 )
         {
            lSPIdx += (m_pSrcEntity->m_lEntPortX + 1);
         }
         else
         {
            lSPIdx = m_pSrcEntity->m_lPosIdx +
                     ((zLONG) (m_pSrcEntity->m_lEntPortY / 2) * tzERDIAGRAMX);
            if ( nSP )
               lSPIdx += ((zLONG) -nSP * tzERDIAGRAMX);

            lSPIdx--;
         }
      }

      // Calculate the target ports index position on the grid.
      lTPIdx = m_pTgtEntity->m_lPosIdx + (m_pTgtEntity->m_lEntPortX / 2);
      lTPIdx += nTP;
      if ( bRecursive || m_pSrcEntity->PosY( ) > m_pTgtEntity->PosY( ) )
         lTPIdx += ((zLONG) m_pTgtEntity->m_lEntPortY * tzERDIAGRAMX);
      else
         lTPIdx -= tzERDIAGRAMX;

      // Calculate the 3 arc index positions on the grid
      if ( bRecursive )
      {
         if ( nSP > 0 )
         {
            lArcIdx1 = lSPIdx + tzERRECURSSZ;
            lArcIdx2 = m_pSrcEntity->m_lPosIdx + tzERENTITYX +
                    ((zLONG) (tzERENTITYY + tzERRECURSSZ) * tzERDIAGRAMX) +
                                                                tzERRECURSSZ;
            lArcIdx3 = lTPIdx + ((zLONG) tzERRECURSSZ * tzERDIAGRAMX);
         }
         else
         {
            lArcIdx1 = lSPIdx - tzERRECURSSZ;
            lArcIdx2 = (m_pSrcEntity->m_lPosIdx +
                        ((zLONG) (tzERENTITYY + tzERRECURSSZ)
                                    * tzERDIAGRAMX )) - (tzERRECURSSZ + 1);
            lArcIdx3 = lTPIdx + ((zLONG) tzERRECURSSZ * tzERDIAGRAMX);
         }

         if ( lArcIdx1 < 0 || lArcIdx1 > tzERMAXGRID ||
              lArcIdx2 < 0 || lArcIdx2 > tzERMAXGRID ||
              lArcIdx3 < 0 || lArcIdx3 > tzERMAXGRID ||
              PosY( lArcIdx2 ) != PosY( lArcIdx3 ) )
         {
            lRC = -12;
            goto BADPATH_tag;
         }
      }
      else
      {
         // Calculate the arcs index position on the grid.
         lArcIdx1 = lTPIdx;
         if ( m_pTgtEntity->PosX( ) > m_pSrcEntity->PosX( ) )
         {
            while ( lArcIdx1 > lSPIdx )
               lArcIdx1 -= tzERDIAGRAMX;

            while ( lArcIdx1 < lSPIdx )
               lArcIdx1 += tzERDIAGRAMX;
         }
         else
         {
            while ( lArcIdx1 < lSPIdx )
               lArcIdx1 += tzERDIAGRAMX;

            while ( lArcIdx1 > lSPIdx )
               lArcIdx1 -= tzERDIAGRAMX;
         }
      }

      // If the arc position is the same as either the source or target
      // port, then fail the line.
      if ( lArcIdx1 == lSPIdx || lArcIdx1 == lTPIdx )
      {
         lRC = -13;
         goto BADPATH_tag;
      }

      // If the arc position is in an entity, see if it is in either the
      // source or target entity, if it is in the source entity, fail
      // the line, if it is in the target entity, convert the line to a
      // straight line for analysis.
      if ( pchGrid[ lArcIdx1 ] & tziENTITY )
      {
         if ( m_pSrcEntity->IndexInEntity( lArcIdx1 ) )
         {
            lRC = -14;
            goto BADPATH_tag;
         }
         else
         if ( m_pTgtEntity->IndexInEntity( lArcIdx1 ) )
         {
            if ( lArcIdx1 > lSPIdx )
            {
               if ( lArcIdx1 > ( lSPIdx + tzERDIAGRAMX ))
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1 -= tzERDIAGRAMX;
               }
               else
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1--;
               }
            }
            else
            {
               if ( lArcIdx1 < ( lSPIdx - tzERDIAGRAMX ))
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1 += tzERDIAGRAMX;
               }
               else
               {
                  while ( pchGrid[ lArcIdx1 ] & tziENTITY )
                     lArcIdx1++;
               }
            }

            lTPIdx = lArcIdx1;
            nTP = nSP;
            lRC -= tzERLINEBENDS;  // OOPS, the line is straight
         }
      }

      // We have the source, target and arc index on the grid, now analyze
      // a line from the source to the target.
      lWkIdx = lSPIdx;
      for ( ; ; )
      {
         if ( nArcsChecked == 0 )
         {
            lNextIdx = lArcIdx1;
         }
         else
         {
            if ( lWkIdx == lArcIdx2 )
               lNextIdx = lArcIdx3;
            else
            if ( lArcIdx2 == 0 || lWkIdx == lArcIdx3 )
               lNextIdx = lTPIdx;
            else
               lNextIdx = lArcIdx2;
         }

         if ( PosX( lWkIdx ) == PosX( lNextIdx ) )
            nDirection = tzVERTICAL;
         else
            nDirection = tzHORIZONTAL;

         while ( lWkIdx > lNextIdx )
         {
            if ( pchGrid[ lWkIdx ] & tziARC )
            {
               lRC = -15;
               goto BADPATH_tag;
            }

            if ( pchGrid[ lWkIdx ] & (tziLINE | tziTEXTVERT | tziTEXTHORIZ) )
            {
               if ( (pchGrid[ lWkIdx ] & (tziVERT | tziTEXTVERT)) &&
                                             nDirection == tzVERTICAL )
               {
                  lRC = -16;
                  goto BADPATH_tag;
               }
               else
               if ( (pchGrid[ lWkIdx ] & (tziHORIZ | tziTEXTHORIZ)) &&
                                             nDirection == tzHORIZONTAL )
               {
                  lRC = -17;
                  goto BADPATH_tag;
               }
               else
               if ( pchGrid[ lWkIdx ] & tziLINE )
                  lRC += tzERLINECROSSESLINE;
               else
                  lRC += tzERLINECROSSESTEXT;
            }
            else
            if ( pchGrid[ lWkIdx ] & tziENTITY )
               lRC += tzERLINECROSSESENTITY;
            else
            {
               if ( nDirection == tzHORIZONTAL )
               {
                  if ( lWkIdx > tzERDIAGRAMX )
                  {
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziENTITY )
                        lRC += tzERLINEABUTSENTITY;
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] &
                                                         (tziLINE | tziARC) )
                     {
                        lRC += tzERLINEABUTSLINE;
                     }
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziTEXT )
                     {
                        lRC += tzERLINEABUTSTEXT;
                     }
                  }
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
               else
               {
                  if ( pchGrid[ lWkIdx - 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;

                  if ( pchGrid[ lWkIdx + 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
            }

            if ( nDirection == tzVERTICAL )
               lWkIdx -= tzERDIAGRAMX;
            else
               lWkIdx--;
         }

         while ( lWkIdx < lNextIdx )
         {
            if ( pchGrid[ lWkIdx ] & tziARC )
            {
               lRC = -18;
               goto BADPATH_tag;
            }

            if ( pchGrid[ lWkIdx ] & (tziLINE | tziTEXTVERT | tziTEXTHORIZ) )
            {
               if ( pchGrid[ lWkIdx ] & (tziVERT | tziTEXTVERT) &&
                                             nDirection == tzVERTICAL ||
                    pchGrid[ lWkIdx ] & (tziHORIZ | tziTEXTHORIZ) &&
                                             nDirection == tzHORIZONTAL )
               {
                  lRC = -19;
                  goto BADPATH_tag;
               }
               else
               if ( pchGrid[ lWkIdx ] & tziLINE )
                  lRC += tzERLINECROSSESLINE;
               else
                  lRC += tzERLINECROSSESTEXT;
            }
            else
            if ( pchGrid[ lWkIdx ] & tziENTITY )
               lRC += tzERLINECROSSESENTITY;
            else
            {
               if ( nDirection == tzHORIZONTAL )
               {
                  if ( lWkIdx > tzERDIAGRAMX )
                  {
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziENTITY )
                        lRC += tzERLINEABUTSENTITY;
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] &
                                                         (tziLINE | tziARC) )
                     {
                        lRC += tzERLINEABUTSLINE;
                     }
                     else
                     if ( pchGrid[ lWkIdx - tzERDIAGRAMX ] & tziTEXT )
                     {
                        lRC += tzERLINEABUTSTEXT;
                     }
                  }

                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + tzERDIAGRAMX ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
               else
               {
                  if ( pchGrid[ lWkIdx - 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx - 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;

                  if ( pchGrid[ lWkIdx + 1 ] & tziENTITY )
                     lRC += tzERLINEABUTSENTITY;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & (tziLINE | tziARC) )
                     lRC += tzERLINEABUTSLINE;
                  else
                  if ( pchGrid[ lWkIdx + 1 ] & tziTEXT )
                     lRC += tzERLINEABUTSTEXT;
               }
            }

            if ( nDirection == tzVERTICAL )
               lWkIdx += tzERDIAGRAMX;
            else
               lWkIdx++;
         }

         if ( lWkIdx == lTPIdx )
            break;

         // We should be at the arc, make sure the cell is clean.
         if ( pchGrid[ lWkIdx ] & (tziLINE | tziARC) )
         {
            lRC = -20;
            goto BADPATH_tag;
         }

         if ( pchGrid[ lWkIdx ] & tziENTITY )
            lRC += tzERLINECROSSESENTITY;

         nArcsChecked++;
      }
   }

   // Return the results of the impact analysis.
   *lpSPIdx = lSPIdx;
   *lpTPIdx = lTPIdx;
   if ( lTPIdx == lArcIdx1 )
   {
      *lpArcIdx1 = 0;
      *lpArcIdx2 = 0;
      *lpArcIdx3 = 0;
   }
   else
   {
      *lpArcIdx1 = lArcIdx1;
      *lpArcIdx2 = lArcIdx2;
      *lpArcIdx3 = lArcIdx3;
   }



BADPATH_tag:
#ifdef DEBUG_ALL
   zCHAR szMsg[ 256 ];
   sprintf_s( szMsg, sizeof( szMsg ), "(TzCtlERL) Line Analysis m_csTextF: %s"
                    "  SourcePort: %d"
                    "  TargetPort: %d"
                    "  Axis: %d"
                    "  Impact Points: %ld ",
             m_csTextF, nSP, nTP, nSrcAxis, lRC );
   if ( lRC < 0 )
   {
      TraceLineS( szMsg, g_Collision[ -(lRC + 1) ] );
      lRC = tzERBADPATH;
   }
   else
      TraceLineS( szMsg, "" );
#else
   if ( lRC < 0 )
      lRC = tzERBADPATH;
#endif

   return( lRC );
}

//
// METHOD: ZER_Relationship::DrawRelationship
//
// RETURN: 1 - line drawn successfully
//         0 - line would have been drawn if it was in view
//        -1 - line could not be drawn
//
zSHORT
ZER_Relationship::DrawRelationship( zBOOL  bErase,
                                    zBOOL  bDisplayMsgBox,
                                    zSHORT nErrCnt )
{
   CPoint   ptWork;
   zLONG    lIdx;
   zPCHAR   pchGrid;
   zSHORT   nWX, nWY;
   zSHORT   nLastDirection;
   zSHORT   nDirection;
   zSHORT   nStartPosX, nStartPosY;
   zSHORT   nEndPosX, nEndPosY;
   zSHORT   nArcsDrawn;
   zSHORT   nArc1PosX, nArc1PosY;
   zSHORT   nArc2PosX, nArc2PosY;
   zSHORT   nArc3PosX, nArc3PosY;
   zSHORT   nNextPosX, nNextPosY;
   CPen     *pPen;
   zBOOL    bRelease;

   // Calculate a line now if necessary.
   if ( m_lStartIdx == 0 )
   {
      // If the relationship line could not be calculated, punt ... any
      // messages were produced in CalculateLine.
//    if ( m_lStartIdx == 0 )    dks 4/18/96
      if ( CalculateLine( bDisplayMsgBox, nErrCnt ) > 0 )
         return( -1 );
   }

   if ( m_pER_Diagram->m_bShowLines == FALSE )
      return( 0 );

   if ( (m_lArcIdx3 &&
         m_pER_Diagram->PositionVisible( m_lArcIdx1, m_lArcIdx3 ) == 0) ||
        (m_lArcIdx3 == 0 &&
         m_pER_Diagram->PositionVisible( m_lStartIdx, m_lEndIdx ) == 0) )
   {
      if ( RelationshipTextVisible( ) )
         DrawRelationshipText( );

      return( 0 );
   }

   // Get the grid pointer from the ER_Diagram.
   pchGrid = m_pER_Diagram->m_pchGrid;
   if ( m_pER_Diagram->m_hDC == 0 )
   {
      bRelease = TRUE;
      m_pER_Diagram->GetDeviceContext( );
   }
   else
      bRelease = FALSE;

   // Set the text drawn indicator.
   if ( bErase )
   {
      if ( m_bTextDrawn )
         EraseText( );
      else
      if ( m_bLineDrawn == 0 )
      {
         if ( bRelease )
            m_pER_Diagram->ReleaseDeviceContext( );

         return( 0 );
      }
   }
   else
   {
      m_bLineDrawn = 1;
      if ( pchGrid[ m_lStartIdx ] == 0 )
         MarkRelationship( );
   }

   //////////////////////////////////////////////////////////////////////////
   //             RELATIONSHIP LINE DRAWING
   //////////////////////////////////////////////////////////////////////////
   if ( bErase )
      pPen = new CPen( PS_SOLID, 1, tzERCOLORBACKGROUND );
   else
   if ( m_pER_Diagram->m_bPrintDC )
      pPen = new CPen( PS_SOLID, 1, tzERCOLORBLACK );
   else
      pPen = new CPen( PS_SOLID, 1, tzERCOLORLINES );

   CPen *pPenOld = m_pER_Diagram->m_hDC->SelectObject( pPen );

   // Get the x and y positions of all the points
   nStartPosX = PosX( m_lStartIdx ) - PosX( tzERDISPLAYPOS );
   nStartPosY = PosY( m_lStartIdx ) - PosY( tzERDISPLAYPOS );
   nEndPosX = PosX( m_lEndIdx ) - PosX( tzERDISPLAYPOS );
   nEndPosY = PosY( m_lEndIdx ) - PosY( tzERDISPLAYPOS );
   nArc1PosX = 0;
   nArc1PosY = 0;
   nArc2PosX = 0;
   nArc2PosY = 0;
   nArc3PosX = 0;
   nArc3PosY = 0;

   if ( m_nArcs )
   {
      nArc1PosX = PosX( m_lArcIdx1 ) - PosX( tzERDISPLAYPOS );
      nArc1PosY = PosY( m_lArcIdx1 ) - PosY( tzERDISPLAYPOS );
      if ( m_nArcs > 1 )
      {
         nArc2PosX = PosX( m_lArcIdx2 ) - PosX( tzERDISPLAYPOS );
         nArc2PosY = PosY( m_lArcIdx2 ) - PosY( tzERDISPLAYPOS );
         nArc3PosX = PosX( m_lArcIdx3 ) - PosX( tzERDISPLAYPOS );
         nArc3PosY = PosY( m_lArcIdx3 ) - PosY( tzERDISPLAYPOS );
      }
   }

   // Now draw the line(s) between the entities
   ptWork.x = nStartPosX * tzERCELLSIZE;
   ptWork.y = nStartPosY * tzERCELLSIZE;

   // We are now pointing at the upper left of the starting cell, move
   // to the appropriate position in the cell.
   nLastDirection = 0;
   nArcsDrawn = 0;
   lIdx = -1;
   for ( ; ; )
   {
      if ( nLastDirection == 0 )
      {
         if ( m_nArcs )
         {
            if ( nStartPosX == nArc1PosX )  // Vertical line
            {
               nDirection = tzDOWN;
               ptWork.x += (tzERCELLSIZE / 2);
               if ( nStartPosY > nArc1PosY )
               {
                  nDirection = tzUP;
                  ptWork.y += (tzERCELLSIZE - 1);
               }
            }
            else
            {
               nDirection = tzRIGHT;
               ptWork.y += (tzERCELLSIZE / 2);
               if ( nStartPosX > nArc1PosX )
               {
                  nDirection = tzLEFT;
                  ptWork.x += (tzERCELLSIZE - 1);
               }
            }
         }
         else
         {
            if ( nStartPosX == nEndPosX )  // Vertical line
            {
               nDirection = tzDOWN;
               ptWork.x += (tzERCELLSIZE / 2);
               if ( nStartPosY > nEndPosY )
               {
                  nDirection = tzUP;
                  ptWork.y += (tzERCELLSIZE - 1);
               }
            }
            else
            {
               nDirection = tzRIGHT;
               ptWork.y += (tzERCELLSIZE / 2);
               if ( nStartPosX > nEndPosX )
               {
                  nDirection = tzLEFT;
                  ptWork.x += (tzERCELLSIZE - 1);
               }
            }
         }

         // Move the pen to the starting point for drawing.
         nWX = nStartPosX;
         nWY = nStartPosY;
         lIdx = m_lStartIdx;
      }

      nLastDirection = nDirection; // Save the last direction

      // Color the first cell in case it's selected.
      if ( nArcsDrawn == 0 )
      {
         if ( bErase )
         {
            m_pER_Diagram->ColorCell( lIdx, tzERCOLORBACKGROUND );
         }
         else
         {
            if ( m_bSelected )
            {
               if ( m_pER_Diagram->m_bPrintDC )
               {
                  if ( m_pER_Diagram->m_bShadeRels )
                     m_pER_Diagram->ColorCell( lIdx, tzERCOLORSHADE );
               }
               else
                  m_pER_Diagram->ColorCell( lIdx, tzERCOLORSELECTED );
            }

            m_pER_Diagram->DrawDiagramCell( lIdx, FALSE, this );
         }

         // Draw the starting Cell and move the index to the second cell of
         // the relationship line.
         if ( nDirection == tzLEFT )
         {
            lIdx--;
            nWX--;
            ptWork.x -= tzERCELLSIZE;
         }
         else
         if ( nDirection == tzUP )
         {
            lIdx -= tzERDIAGRAMX;
            nWY--;
            ptWork.y -= tzERCELLSIZE;
         }
         else
         if ( nDirection == tzRIGHT )
         {
            lIdx++;
            nWX++;
            ptWork.x += tzERCELLSIZE;
         }
         else
         {
            lIdx += tzERDIAGRAMX;
            nWY++;
            ptWork.y += tzERCELLSIZE;
         }

         m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
      }

      // If this is the first time through and the defined line is
      // straight, get out and draw one line only.
      if ( nArcsDrawn == 0 && m_nArcs == 0 )
      {
         break;
      }

      // If there is a bend in the line (denoted by an Arc Cell), Draw
      // a line up to the arc cell.
      if ( nWX == nArc1PosX )
      {
         while ( nWY < nArc1PosY )
         {
            ptWork.y += tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWY++;
            lIdx += tzERDIAGRAMX;
         }

         while ( nWY > nArc1PosY )
         {
            ptWork.y -= tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWY--;
            lIdx -= tzERDIAGRAMX;
         }
      }
      else
      {
         while ( nWX < nArc1PosX )
         {
            ptWork.x += tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWX++;
            lIdx++;
         }

         while ( nWX > nArc1PosX )
         {
            ptWork.x -= tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWX--;
            lIdx--;
         }
      }

      // Determine the next position to which we are going.
      if ( nArcsDrawn < 2 && ( m_lArcIdx2 || m_lArcIdx3 ) )
      {
         nNextPosX = nArc2PosX;
         nNextPosY = nArc2PosY;
      }
      else
      {
         nNextPosX = nEndPosX;
         nNextPosY = nEndPosY;
      }

      // We've made it to an Arc Cell, now draw the arc.
      if ( nArc1PosX == nNextPosX )  // Vertical line
      {
         // We were moving left or right, now it is time to go up or
         // down, move the X position to the middle of the Arc Cell.
         if ( nDirection == tzLEFT )
            ptWork.x -= (tzERCELLSIZE / 2);
         else
            ptWork.x += (tzERCELLSIZE / 2);

         if ( nArc1PosY > nNextPosY )
         {
            // We're about to go up, move the Y position out of the Arc Cell.
            ptWork.y -= ((tzERCELLSIZE / 2) + 1);
            nDirection = tzUP;
            nWY--;
         }
         else
         {
            // We're about to go down, move the Y position out of the
            // Arc Cell.
            ptWork.y += ((tzERCELLSIZE / 2) + 1);
            nDirection = tzDOWN;
            nWY++;
         }
      }
      else
      {
         // We were moving up or down, now it is time to go left or
         // right, move the Y position to the middle of the Arc Cell.
         if ( nDirection == tzUP )
            ptWork.y -= (tzERCELLSIZE / 2);
         else
            ptWork.y += (tzERCELLSIZE / 2);

         if ( nArc1PosX > nNextPosX )
         {
            // We're about to go left, move the X position out of the
            // Arc Cell.
            ptWork.x -= ((tzERCELLSIZE / 2) + 1);
            nDirection = tzLEFT;
            nWX--;
         }
         else
         {
            // We're about to go right, move the X position to the right
            // of the Arc Cell.
            ptWork.x += ((tzERCELLSIZE / 2) + 1);
            nDirection = tzRIGHT;
            nWX++;
         }
      }

      // We're at the arc cell and have moved the pointer ... now draw an Arc.
      // Now we know where the arc belongs, if the arc is not hidden by an
      // Entity, then draw or erase the Arc.
      if ( (pchGrid[ lIdx ] & tziENTITY) == 0 )
      {
         if ( bErase )
            m_pER_Diagram->DrawDiagramCell( lIdx, TRUE );
         else
         {
            if ( m_bSelected )
            {
               if ( m_pER_Diagram->m_bPrintDC )
               {
                  if ( m_pER_Diagram->m_bShadeRels )
                     m_pER_Diagram->ColorCell( lIdx, tzERCOLORSHADE );
               }
               else
                  m_pER_Diagram->ColorCell( lIdx, tzERCOLORSELECTED );
            }

            m_pER_Diagram->DrawDiagramCell( lIdx );
         }
      }

      // Move the pen around the arc just built.
      m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );

      // Finally, move the index pointer into the next cell for drawing
      if ( nDirection == tzLEFT )
         lIdx--;
      else
      if ( nDirection == tzUP )
         lIdx -= tzERDIAGRAMX;
      else
      if ( nDirection == tzRIGHT )
         lIdx++;
      else
         lIdx += tzERDIAGRAMX;

      nArcsDrawn++;
      if ( nArcsDrawn == 3 || (m_lArcIdx2 == 0 && m_lArcIdx3 == 0) )
         break;

      nArc1PosX = nArc2PosX;
      nArc1PosY = nArc2PosY;
      nArc2PosX = nArc3PosX;
      nArc2PosY = nArc3PosY;
      nArc3PosX = 0;
      nArc3PosY = 0;
   }

   // Through drawing all the arcs, now finish with the last leg of the line.
   if ( nDirection == tzUP || nDirection == tzDOWN )
   {
      if ( nDirection == tzDOWN )
      {
         while ( nWY < nEndPosY )
         {
            ptWork.y += tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWY++;
            lIdx += tzERDIAGRAMX;
         }
      }
      else
      {
         while ( nWY > nEndPosY )
         {
            ptWork.y -= tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
            {
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );
            }

            nWY--;
            lIdx -= tzERDIAGRAMX;
         }
      }
   }
   else
   {
      if ( nDirection == tzRIGHT )
      {
         while ( nWX < nEndPosX )
         {
            ptWork.x += tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWX++;
            lIdx++;
         }
      }
      else
      {
         while ( nWX > nEndPosX )
         {
            ptWork.x -= tzERCELLSIZE;
            if ( pchGrid[ lIdx ] & tziENTITY )
               m_pER_Diagram->m_hDC->MoveTo( ptWork.x, ptWork.y );
            else
               m_pER_Diagram->m_hDC->LineTo( ptWork.x, ptWork.y );

            nWX--;
            lIdx--;
         }
      }
   }

   // We're at the terminating cell, draw it.
   if ( bErase )
   {
      m_pER_Diagram->ColorCell( lIdx, tzERCOLORBACKGROUND );
   }
   else
   {
      if ( m_bSelected )
      {
         if ( m_pER_Diagram->m_bPrintDC )
         {
            if ( m_pER_Diagram->m_bShadeRels )
               m_pER_Diagram->ColorCell( lIdx, tzERCOLORSHADE );
         }
         else
            m_pER_Diagram->ColorCell( lIdx, tzERCOLORSELECTED );
      }

      m_pER_Diagram->DrawDiagramCell( lIdx, FALSE, this );
   }

   // Skip out if we were erasing a line.
   if ( bErase == FALSE )
   {
      // Now draw the text
      DrawRelationshipText( );
   }

   m_pER_Diagram->m_hDC->SelectObject( pPenOld );
   delete( pPen );
   if ( bRelease )
      m_pER_Diagram->ReleaseDeviceContext( );

   return( 1 );   // return indicating that line was drawn
}

//
// METHOD: ZER_Relationship::MarkRelationship
//
//
void
ZER_Relationship::MarkRelationship( zBOOL bErase )
{
   zLONG      lIdx;
   zPCHAR     pchGrid;
   zSHORT     nWX, nWY;
   zSHORT     nLastDirection;
   zSHORT     nDirection;
   zSHORT     nStartPosX, nStartPosY;
   zSHORT     nEndPosX, nEndPosY;
   zSHORT     nArcsDrawn;
   zSHORT     nArc1PosX, nArc1PosY;
   zSHORT     nArc2PosX, nArc2PosY;
   zSHORT     nArc3PosX, nArc3PosY;
   zSHORT     nNextPosX, nNextPosY;

   // If the line is not calculated, do not mark it.
   if ( m_lStartIdx == 0 )
      return;

   //////////////////////////////////////////////////////////////////////////
   //             RELATIONSHIP LINE MARKING
   //////////////////////////////////////////////////////////////////////////
   // Get the x and y positions of all the points
   nStartPosX = PosX( m_lStartIdx ) - PosX( tzERDISPLAYPOS );
   nStartPosY = PosY( m_lStartIdx ) - PosY( tzERDISPLAYPOS );
   nEndPosX = PosX( m_lEndIdx ) - PosX( tzERDISPLAYPOS );
   nEndPosY = PosY( m_lEndIdx ) - PosY( tzERDISPLAYPOS );
   nArc1PosX = 0;
   nArc1PosY = 0;
   nArc2PosX = 0;
   nArc2PosY = 0;
   nArc3PosX = 0;
   nArc3PosY = 0;
   if ( m_nArcs )
   {
      nArc1PosX = PosX( m_lArcIdx1 ) - PosX( tzERDISPLAYPOS );
      nArc1PosY = PosY( m_lArcIdx1 ) - PosY( tzERDISPLAYPOS );
      if ( m_nArcs > 1 )
      {
         nArc2PosX = PosX( m_lArcIdx2 ) - PosX( tzERDISPLAYPOS );
         nArc2PosY = PosY( m_lArcIdx2 ) - PosY( tzERDISPLAYPOS );
         nArc3PosX = PosX( m_lArcIdx3 ) - PosX( tzERDISPLAYPOS );
         nArc3PosY = PosY( m_lArcIdx3 ) - PosY( tzERDISPLAYPOS );
      }
   }

   pchGrid = m_pER_Diagram->m_pchGrid;
   nLastDirection = 0;
   nArcsDrawn = 0;
   lIdx = -1;
   for ( ;; )
   {
      if ( nLastDirection == 0 )
      {
         if ( m_nArcs )
         {
            if ( nStartPosX == nArc1PosX )  // Vertical line
            {
               nDirection = tzDOWN;
               if ( nStartPosY > nArc1PosY )
                  nDirection = tzUP;
            }
            else
            {
               nDirection = tzRIGHT;
               if ( nStartPosX > nArc1PosX )
                  nDirection = tzLEFT;
            }
         }
         else
         {
            if ( nStartPosX == nEndPosX )  // Vertical line
            {
               nDirection = tzDOWN;
               if ( nStartPosY > nEndPosY )
                  nDirection = tzUP;
            }
            else
            {
               nDirection = tzRIGHT;
               if ( nStartPosX > nEndPosX )
                  nDirection = tzLEFT;
            }
         }

         // Move the pen to the starting point for drawing.
         nWX = nStartPosX;
         nWY = nStartPosY;
         lIdx = m_lStartIdx;
      }

      nLastDirection = nDirection; // Save the last direction

      // If this is the first time through and the defined line is
      // straight, get out and draw one line only.
      if ( nArcsDrawn == 0 && m_nArcs == 0 )
      {
         break;
      }

      // If there is a bend in the line ( denoted by an Arc Cell ), Draw
      // a line up to the arc cell.
      if ( nWX == nArc1PosX )
      {
         if ( nArcsDrawn == 0 )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziTEXTVERT);
            }
            else
               pchGrid[ lIdx ] |= tziTEXTVERT;
         }

         while ( nWY < nArc1PosY )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziVERT);
               if ( (pchGrid[ lIdx ] & tziHORIZ) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziVERT;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWY++;
            lIdx += tzERDIAGRAMX;
         }

         while ( nWY > nArc1PosY )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziVERT);
               if ( (pchGrid[ lIdx ] & tziHORIZ) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziVERT;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWY--;
            lIdx -= tzERDIAGRAMX;
         }
      }
      else
      {
         if ( nArcsDrawn == 0 )
         {
            if ( bErase )
               pchGrid[ lIdx ] &= (0xFF - tziTEXTHORIZ);
            else
               pchGrid[ lIdx ] |= tziTEXTHORIZ;
         }

         while ( nWX < nArc1PosX )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziHORIZ);
               if ( (pchGrid[ lIdx ] & tziVERT) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziHORIZ;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWX++;
            lIdx++;
         }

         while ( nWX > nArc1PosX )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziHORIZ);
               if ( (pchGrid[ lIdx ] & tziVERT) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziHORIZ;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWX--;
            lIdx--;
         }
      }

      // Determine the next position to which we are going.
      if ( nArcsDrawn < 2 && (m_lArcIdx2 || m_lArcIdx3) )
      {
         nNextPosX = nArc2PosX;
         nNextPosY = nArc2PosY;
      }
      else
      {
         nNextPosX = nEndPosX;
         nNextPosY = nEndPosY;
      }

      // We've made it to an Arc Cell, now mark the arc.
      if ( nArc1PosX == nNextPosX )  // Vertical line
      {
         // We were moving left or right, now it is time to go up or down.
         if ( nArc1PosY > nNextPosY )
         {
            // We're about to go up, move the Y position out of the Arc Cell.
            nDirection = tzUP;
            nWY--;
         }
         else
         {
            // We're about to go down, move the Y position out of the
            // Arc Cell.
            nDirection = tzDOWN;
            nWY++;
         }
      }
      else
      {
         // We were moving up or down, now it is time to go left or right.
         if ( nArc1PosX > nNextPosX )
         {
            // We're about to go left, move the X position out of the
            // Arc Cell.
            nDirection = tzLEFT;
            nWX--;
         }
         else
         {
            // We're about to go right, move the X position to the right
            // of the Arc Cell.
            nDirection = tzRIGHT;
            nWX++;
         }
      }

      // Were at the arc cell and have moved the pointer now draw an Arc
      // First, mark the type of arc to be drawn.
      pchGrid[ lIdx ] |= tziARC;
      if ( nLastDirection == tzLEFT )
      {
         if ( nDirection == tzUP )
            pchGrid[ lIdx ] |= tziURIGHT;
         else
            pchGrid[ lIdx ] |= tziBRIGHT;
      }
      else
      if ( nLastDirection == tzRIGHT )
      {
         if ( nDirection == tzUP )
            pchGrid[ lIdx ] |= tziULEFT;
         else
            pchGrid[ lIdx ] |= tziBLEFT;
      }
      else
      if ( nLastDirection == tzUP )
      {
         if ( nDirection == tzLEFT )
            pchGrid[ lIdx ] |= tziBLEFT;
         else
            pchGrid[ lIdx ] |= tziBRIGHT;
      }
      else
      {
         if ( nDirection == tzLEFT )
            pchGrid[ lIdx ] |= tziULEFT;
         else
            pchGrid[ lIdx ] |= tziURIGHT;
      }

      // If we are erasing, turn off all arc bits in the cell.
      if ( bErase )
      {
         pchGrid[ lIdx ] &= (0xFF - (tziARC + tziULEFT + tziURIGHT +
                                       tziBLEFT + tziBRIGHT));
      }

      // Finally, move the index pointer into the next cell for drawing.
      if ( nDirection == tzLEFT )
         lIdx--;
      else
      if ( nDirection == tzUP )
         lIdx -= tzERDIAGRAMX;
      else
      if ( nDirection == tzRIGHT )
         lIdx++;
      else
         lIdx += tzERDIAGRAMX;

      nArcsDrawn++;
      if ( nArcsDrawn == 3 || (m_lArcIdx2 == 0 && m_lArcIdx3 == 0) )
         break;

      nArc1PosX = nArc2PosX;
      nArc1PosY = nArc2PosY;
      nArc2PosX = nArc3PosX;
      nArc2PosY = nArc3PosY;
      nArc3PosX = 0;
      nArc3PosY = 0;
   }

   // Through marking all the arcs, now finish with the last leg of the line.
   if ( nDirection == tzUP || nDirection == tzDOWN )
   {
      if ( nArcsDrawn == 0 )
      {
         if ( bErase )
            pchGrid[ lIdx ] &= (0xFF - tziTEXTVERT);
         else
            pchGrid[ lIdx ] |= tziTEXTVERT;
      }

      if ( nDirection == tzDOWN )
      {
         while ( nWY < nEndPosY )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziVERT);
               if ( (pchGrid[ lIdx ] & tziHORIZ) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziVERT;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWY++;
            lIdx += tzERDIAGRAMX;
         }

         if ( bErase )
         {
            pchGrid[ lIdx ] &= (0xFF - (tziVERT | tziTEXTVERT));
            if ( (pchGrid[ lIdx ] & tziHORIZ) == 0 )
               pchGrid[ lIdx ] &= (0xFF - tziLINE);

            if ( pchGrid[ lIdx ] )
               m_pER_Diagram->DrawDiagramCell( lIdx );
         }
         else
         {
            pchGrid[ lIdx ] |= tziVERT;
            pchGrid[ lIdx ] |= tziLINE;
         }
      }
      else
      {
         while ( nWY > nEndPosY )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziVERT);
               if ( (pchGrid[ lIdx ] & tziHORIZ) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziVERT;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWY--;
            lIdx -= tzERDIAGRAMX;
         }

         if ( bErase )
         {
            pchGrid[ lIdx ] &= (0xFF - (tziVERT | tziTEXTVERT));
            if ( (pchGrid[ lIdx ] & tziHORIZ) == 0 )
               pchGrid[ lIdx ] &= (0xFF - tziLINE);

            if ( pchGrid[ lIdx ] )
               m_pER_Diagram->DrawDiagramCell( lIdx );
         }
         else
         {
            pchGrid[ lIdx ] |= tziVERT;
            pchGrid[ lIdx ] |= tziLINE;
         }
      }

      if ( bErase == FALSE )
         pchGrid[ lIdx ] |= tziTEXTVERT;
   }
   else
   {
      if ( nArcsDrawn == 0 )
      {
         if ( bErase )
            pchGrid[ lIdx ] &= (0xFF - tziTEXTHORIZ);
         else
            pchGrid[ lIdx ] |= tziTEXTHORIZ;
      }

      if ( nDirection == tzRIGHT )
      {
         while ( nWX < nEndPosX )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziHORIZ);
               if ( (pchGrid[ lIdx ] & tziVERT) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziHORIZ;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWX++;
            lIdx++;
         }

         if ( bErase )
         {
            pchGrid[ lIdx ] &= (0xFF - (tziHORIZ | tziTEXTHORIZ));
            if ( (pchGrid[ lIdx ] & tziVERT) == 0 )
               pchGrid[ lIdx ] &= (0xFF - tziLINE);

            if ( pchGrid[ lIdx ] )
               m_pER_Diagram->DrawDiagramCell( lIdx );
         }
         else
         {
            pchGrid[ lIdx ] |= tziHORIZ;
            pchGrid[ lIdx ] |= tziLINE;
         }
      }
      else
      {
         while ( nWX > nEndPosX )
         {
            if ( bErase )
            {
               pchGrid[ lIdx ] &= (0xFF - tziHORIZ);
               if ( (pchGrid[ lIdx ] & tziVERT) == 0 )
                  pchGrid[ lIdx ] &= (0xFF - tziLINE);

               if ( pchGrid[ lIdx ] )
                  m_pER_Diagram->DrawDiagramCell( lIdx );
            }
            else
            {
               pchGrid[ lIdx ] |= tziHORIZ;
               pchGrid[ lIdx ] |= tziLINE;
            }

            nWX--;
            lIdx--;
         }

         if ( bErase )
         {
            pchGrid[ lIdx ] &= (0xFF - (tziHORIZ | tziTEXTHORIZ));
            if ( (pchGrid[ lIdx ] & tziVERT) == 0 )
               pchGrid[ lIdx ] &= (0xFF - tziLINE);

            if ( pchGrid[ lIdx ] )
               m_pER_Diagram->DrawDiagramCell( lIdx );
         }
         else
         {
            pchGrid[ lIdx ] |= tziHORIZ;
            pchGrid[ lIdx ] |= tziLINE;
         }
      }

      if ( bErase == FALSE )
         pchGrid[ lIdx ] |= tziTEXTHORIZ;
   }
   // End of relationship marking
}

zSHORT
ZER_Relationship::PosX( zLONG lIdx )
{
   return( (zSHORT) (lIdx % tzERDIAGRAMX) );
}

zSHORT
ZER_Relationship::PosY( zLONG lIdx )
{
   return( (zSHORT) (lIdx / tzERDIAGRAMX) );
}

//
// METHOD: ZER_Relationship::Erase
//
//
void
ZER_Relationship::Erase( zBOOL bUnmark )
{
   DrawRelationship( TRUE );
   m_bLineDrawn = 0;
   if ( bUnmark )
   {
      MarkRelationship( TRUE );
      m_lStartIdx = 0;
   }
}

//
// METHOD: ZER_Relationship::CalculateText
//
//
void
ZER_Relationship::CalculateText( )
{
   zLONG    lIdx;
   zLONG    lIdx1;
   zLONG    lIdx2;
   zLONG    lNextIdx;
   zLONG    lBestIdx;
   zSHORT   nBestLth;
   zSHORT   nBestDir;
   zLONG    lWorkIdx;
   zSHORT   nWorkLth;
   zSHORT   nWorkDir;
   zPCHAR   pchGrid;
   zSHORT   nIncr;
   zSHORT   nOffset;

   // Initialize variables.
   pchGrid = m_pER_Diagram->m_pchGrid;
   lIdx = 0;
   lBestIdx = 0;
   nBestLth = 0;
   nBestDir = 0;

   // If text is calculated, Erase the calculated text information
   // and recalculate.
   if ( m_bTextDrawn )
      EraseText( );

   if ( !m_pER_Diagram->m_bShowText )
      return;

   // We can only calculate text if the line has been calculated and drawn.
   if ( m_lStartIdx && (pchGrid[ m_lStartIdx ] & tziLINE) )
   {
      for ( ;; )
      {
         if ( lIdx == 0 )
         {
            lIdx = m_lStartIdx;
            if ( m_lArcIdx1 )
               lNextIdx = m_lArcIdx1;
            else
               lNextIdx = m_lEndIdx;
         }
         else
         if ( lIdx == m_lEndIdx )
            break;   // the only way to get out of this forever loop
         else
         if ( lIdx == m_lArcIdx1 )
         {
            if ( m_lArcIdx2 )
            {
               if ( m_lArcIdx2 == m_lArcIdx1 ) // fix endless loop DKS
                  lNextIdx++;
               else
                  lNextIdx = m_lArcIdx2;
            }
            else
               lNextIdx = m_lEndIdx;
         }
         else
         if ( lIdx == m_lArcIdx2 )
            lNextIdx = m_lArcIdx3;
         else
            lNextIdx = m_lEndIdx;

         // Now that we have established a starting and ending point for
         // a relationship line, determine the line's direction.
         lWorkIdx = 0;
         nWorkLth = 0;
         if ( PosY( lIdx ) == PosY( lNextIdx ) )
         {
            nOffset = tzERDIAGRAMX;
            if ( lIdx > lNextIdx )
            {
               nWorkDir = tzLEFT;
               nIncr = -1;
            }
            else
            {
               nWorkDir = tzRIGHT;
               nIncr = 1;
            }
         }
         else
         {
            nOffset = 1;
            if ( lIdx > lNextIdx )
            {
               nWorkDir = tzUP;
               nIncr = -tzERDIAGRAMX;
            }
            else
            {
               nWorkDir = tzDOWN;
               nIncr = tzERDIAGRAMX;
            }
         }

         // Now that the direction and offset and increment values have
         // been determined, travel down the drawn line and find the
         // best available space for drawing the relationship text.
         if ( nWorkDir == tzUP || nWorkDir == tzDOWN )
         {
            if ( PosX( lIdx ) == 0 || PosX( lIdx ) == tzERDIAGRAMX )
               lIdx = lNextIdx;
         }
         else
         {
            if ( PosY( lIdx ) == 0 || PosY( lIdx ) == tzERDIAGRAMY )
               lIdx = lNextIdx;
         }

//       while ( lIdx != lNextIdx )  // DKS 12/2/95 ... overrun bug
         while ( lIdx < lNextIdx && nIncr > 0 ||
                 lIdx > lNextIdx && nIncr < 0 )
         {
            // If this is not a cell we can draw text in, then see if the
            // accumulated cell information is better than the current best.
            lIdx1 = lIdx + nOffset;
            lIdx2 = lIdx - nOffset;
            if ( (pchGrid[ lIdx ] &
                             (tziARC | tziENTITY | tziTEXTHORIZ | tziTEXTVERT)) ||
                 lIdx1 > tzERMAXGRID ||
                 (pchGrid[ lIdx1 ] &
                   (tziARC | tziLINE | tziENTITY | tziTEXTHORIZ | tziTEXTVERT)) ||
                 lIdx2 < 0  ||
                 (pchGrid[ lIdx2 ] &
                   (tziARC | tziLINE | tziENTITY | tziTEXTHORIZ | tziTEXTVERT)) )
            {
               if ( lWorkIdx && nWorkLth > nBestLth )
               {
                  lBestIdx = lWorkIdx;
                  nBestLth = nWorkLth;
                  nBestDir = nWorkDir;
               }

               lWorkIdx = 0;
               nWorkLth = 0;
            }
            else
            {
               if ( lWorkIdx == 0 )
                  lWorkIdx = lIdx;

               nWorkLth++;
            }

            lIdx += nIncr;

         }  // end of:  while ( lIdx != lNextIdx )

         // If we've hit an arc point, see if the buckets need to be emptied.
         if ( lWorkIdx && nWorkLth > nBestLth )
         {
            lBestIdx = lWorkIdx;
            nBestLth = nWorkLth;
            nBestDir = nWorkDir;
         }

         lWorkIdx = 0;
         nWorkLth = 0;

      }  // end of:  for ( ;; )
   }

   // Set the relationship variables when finished.
   m_lTextIdx = lBestIdx;
   m_nTextLth = nBestLth;
   m_nTextDir = nBestDir;
}

zBOOL
ZER_Relationship::RelationshipTextVisible( )
{
   zLONG lIdx1, lIdx2;

   if ( m_nTextLth == 0 )
      return( 0 );

   if ( m_nTextDir == tzUP )
   {
      lIdx1 = (m_lTextIdx - ((zLONG) (m_nTextLth - 1) * tzERDIAGRAMX)) - 1;
      lIdx2 = m_lTextIdx + 1;
   }
   else
   if ( m_nTextDir == tzDOWN )
   {
      lIdx1 = m_lTextIdx - 1;
      lIdx2 = (m_lTextIdx + ((zLONG) (m_nTextLth - 1) * tzERDIAGRAMX)) + 1;
   }
   else
   if ( m_nTextDir == tzLEFT )
   {
      lIdx1 = (m_lTextIdx - (m_nTextLth - 1)) - tzERDIAGRAMX;
      lIdx2 = m_lTextIdx + tzERDIAGRAMX;
   }
   else
   {
      lIdx1 = m_lTextIdx - tzERDIAGRAMX;
      lIdx1 = (m_lTextIdx + (m_nTextLth - 1)) + tzERDIAGRAMX;
   }

   return( m_pER_Diagram->PositionVisible( lIdx1, lIdx2 ) );
}

//
// METHOD: ZER_Relationship::DrawRelationshipText
//
//
void
ZER_Relationship::DrawRelationshipText( )
{
   zLONG      lIdx;
   zLONG      lDrawDiagramCell;
   zPCHAR     pchGrid;
   zCHAR      cSetBits;
   CString    zsText;
   zLONG      lWX, lWY;
   zSHORT     nPass;
   zSHORT     nIncr;
   zSHORT     nOffset;
   zSHORT     nStartPos, nEndPos;
   zUSHORT    uTextL;
   zUSHORT    uTextC;
   CSize      Size;
   CFont      *pFont;
   zBOOL      bRelease;
   COLORREF   colorOldText;

   //////////////////////////////////////////////////////////////////////////
   //                   TEXT DRAWING
   //////////////////////////////////////////////////////////////////////////
   if ( m_pER_Diagram->m_bShowText == FALSE )
      return;

   pchGrid = m_pER_Diagram->m_pchGrid;

   // If text has not been calculated or can not be drawn, punt.
   if ( m_nTextLth == 0 )
   {
      CalculateText( );
      if ( m_nTextLth == 0 )
         return;  // we have no room for text ... no sense trying to draw it
   }

   if ( m_pER_Diagram->m_hDC == 0 )
   {
      bRelease = TRUE;
      m_pER_Diagram->GetDeviceContext( );
   }
   else
      bRelease = FALSE;

   if ( m_pER_Diagram->m_bPrintDC )
      colorOldText = m_pER_Diagram->m_hDC->SetTextColor( tzERCOLORBLACK );
   else
      colorOldText = m_pER_Diagram->m_hDC->SetTextColor( tzERCOLORTEXT );

   // Now determine the direction
   if ( m_nTextDir == tzUP || m_nTextDir == tzDOWN )
   {
      nStartPos = PosY( m_lStartIdx );
      nEndPos = PosY( m_lEndIdx );
   }
   else
   {
      nStartPos = PosX( m_lStartIdx );
      nEndPos = PosX( m_lEndIdx );
   }

   for ( nPass = 0; nPass < 2; nPass++ )
   {
      // Set the index pointer to the initial text index.
      lIdx = m_lTextIdx;

      if ( (nPass == 0 && nStartPos < nEndPos) ||
           (nPass == 1 && nEndPos < nStartPos) )
      {
         zsText = m_csTextF;
         uTextL = m_uLPOffsetF;
      }
      else
      {
         zsText = m_csTextR;
         uTextL = m_uLPOffsetR;
      }

      if ( m_pER_Diagram->m_bShowCard )
         uTextL = zsText.GetLength( );

      // Position the Index pointer in the first possible drawing cell
      // based on the text direction and whether or not we're drawing the
      // forward or reverse relationship.
      if ( uTextL )
      {
         // Set the text drawn indicator.
         m_bTextDrawn = 1;

         // If the text pointer is at the bottom of a vertical line and
         // we're going to be drawing downwards, move the pointer to the
         // top of the vertical line.
         if ( m_nTextDir == tzUP )
            lIdx -= ((zLONG) (m_nTextLth - 1) * tzERDIAGRAMX);
         else
         if ( m_nTextDir == tzLEFT )
            lIdx -= (m_nTextLth - 1);

         CFont *pFontOld = 0;
         if ( m_nTextDir == tzDOWN || m_nTextDir == tzUP )
         {
            // DRAWING TEXT DOWNWARD ON EITHER SIDE OF THE LINE!!
            // if we are going forward in the down direction or backward
            // in a up direction then rotate the text 270 degrees
            // counter clockwise (90 degrees clockwise).
            pFont = new CFont( );
            pFont->CreateFont( tzERFONTHEIGHT - 1, tzERFONTWIDTH, 2700, 2700,
                               tzERFONTWEIGHT, 0, 0, 0, ANSI_CHARSET,
                               OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
                               PROOF_QUALITY, tzERFONTFF, tzERFONTNAME );
            pFontOld = m_pER_Diagram->m_hDC->SelectObject( pFont );
            for ( ; ; )
            {
               Size = m_pER_Diagram->m_hDC->GetTextExtent( zsText, uTextL );
               uTextC = (zUSHORT) ((Size.cx / tzERCELLSIZE) + (Size.cx % tzERCELLSIZE ? 1 : 0));
               if ( m_nTextLth >= (zSHORT) uTextC )
                  break;

               uTextL--;
               if ( uTextL == 0 )
                  break;
            }

            // lIdx is positioned to the first cell of the draw line,
            // calculate the first text drawing cell.
            lDrawDiagramCell = lIdx +
                   ((zLONG) ((m_nTextLth - uTextC) / 2) * tzERDIAGRAMX);
//          lWX = ((PosX( lDrawDiagramCell ) - PosX( tzERDISPLAYPOS )) *
//                                   tzERCELLSIZE) + (tzERCELLSIZE / 2) + 2;
            lWX = (PosX( lDrawDiagramCell ) - PosX( tzERDISPLAYPOS )) * tzERCELLSIZE;
            lWY = (PosY( lDrawDiagramCell ) - PosY( tzERDISPLAYPOS )) * tzERCELLSIZE;
            if ( nPass == 0 )
            {
               // On the first pass, we draw downward on the right side
               // of the line.
               lWX += (2 * tzERCELLSIZE) - 1;
            }
            else
            {
               lWX += (tzERCELLSIZE / 2) - 1;  // Downward on left of line
            }
         }
         else
         {
            // DRAWING TEXT ACROSS ABOVE AND BELOW THE LINE!!
            // If we are writing left or right then write the text
            // rightside up either above or below the line.  NOTE: we
            // use an orientation of 1 instead of 0 because the
            // non-standard font height and width results in different
            // size text when rotated.  By rotating the level text by
            // .1 degrees, it looks more like the text rotated 90 and
            // 270 degrees.
            pFont = new CFont( );
            pFont->CreateFont( tzERFONTHEIGHT - 1, tzERFONTWIDTH,
                               1, 1, tzERFONTWEIGHT, 0, 0, 0, ANSI_CHARSET,
                               OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
                               PROOF_QUALITY, tzERFONTFF, tzERFONTNAME );
            pFontOld = m_pER_Diagram->m_hDC->SelectObject( pFont );
            for ( ;; )
            {
               Size= m_pER_Diagram->m_hDC->GetTextExtent( zsText, uTextL );
               uTextC = (zUSHORT) ((Size.cx / tzERCELLSIZE) + (Size.cx % tzERCELLSIZE ? 1 : 0));
               if ( m_nTextLth >= (zSHORT) uTextC )
                  break;

               uTextL--;
               if ( uTextL == 0 )
                  break;
            }

            lDrawDiagramCell = lIdx + ((m_nTextLth - uTextC) / 2 );
            lWX = (PosX( lDrawDiagramCell ) - PosX( tzERDISPLAYPOS )) *
                                                               tzERCELLSIZE;
            lWY = (PosY( lDrawDiagramCell ) - PosY( tzERDISPLAYPOS )) *
                                                               tzERCELLSIZE;
//          lWY = ((PosY( lDrawDiagramCell ) - PosY( tzERDISPLAYPOS )) *
//                                   tzERCELLSIZE) - (tzERCELLSIZE / 2) - 2;

            if ( nPass == 1 )
            {
               // On second pass, we draw below the line
               lWY += (tzERCELLSIZE / 2);
               if ( tzERCELLSIZE > tzERLINETEXTOFFSET )
                  lWY += tzERLINETEXTOFFSET;
            }
            else
               lWY -= tzERCELLSIZE;
         }

         if ( uTextL )
         {
            int nOldBkMode = m_pER_Diagram->m_hDC->SetBkMode( TRANSPARENT );
            m_pER_Diagram->m_hDC->TextOut( lWX, lWY, zsText, uTextL );

            // Now that we have drawn the text, mark where the text exists
            // in the grid
            if ( m_nTextDir == tzUP || m_nTextDir == tzDOWN )
            {
               cSetBits = (char) (tziTEXT | tziTEXTVERT);
               if ( nPass == 0 )
                  nOffset = 1;
               else
                  nOffset = -1;

               nIncr = tzERDIAGRAMX;
            }
            else
            {
               nIncr = 1;
               cSetBits = (char) (tziTEXT | tziTEXTHORIZ);
               if ( nPass == 0 )
                  nOffset = -tzERDIAGRAMX;
               else
                  nOffset = tzERDIAGRAMX;
            }

            while ( uTextC )
            {
               pchGrid[ lDrawDiagramCell ] |= cSetBits;
               pchGrid[ lDrawDiagramCell + nOffset ] |= cSetBits;
               lDrawDiagramCell += nIncr;
               uTextC--;
            }

            m_pER_Diagram->m_hDC->SetBkMode( nOldBkMode );
         }

         if ( pFontOld )
            m_pER_Diagram->m_hDC->SelectObject( pFontOld );

         delete( pFont );
      }
   }

   m_pER_Diagram->m_hDC->SetTextColor( colorOldText );
   if ( bRelease )
      m_pER_Diagram->ReleaseDeviceContext( );
}

//
// METHOD: ZER_Relationship::EraseText
//
//
void
ZER_Relationship::EraseText( )
{
   zPCHAR   pchGrid;
   zCHAR    cSetBits;
   zSHORT   nIncr;
   zSHORT   nOffset;
   zSHORT   nLth;
   zLONG    lIdx;

   if ( m_lTextIdx == 0 || m_bTextDrawn == 0 )
      return;

   m_bTextDrawn = 0;
   pchGrid = m_pER_Diagram->m_pchGrid;

   // Get length of the maximum text drawing area.
   nLth = m_nTextLth;
   lIdx = m_lTextIdx;
   if ( m_nTextDir == tzUP )
   {
      nOffset = 1;
      nIncr = -tzERDIAGRAMX;
      cSetBits = (char) (tziTEXT | tziTEXTVERT);
   }
   else
   if ( m_nTextDir == tzDOWN )
   {
      nOffset = 1;
      nIncr = tzERDIAGRAMX;
      cSetBits = (char) (tziTEXT | tziTEXTVERT);
   }
   else
   if ( m_nTextDir == tzLEFT )
   {
      nOffset = tzERDIAGRAMX;
      nIncr = -1;
      cSetBits = (tziTEXT | tziTEXTHORIZ);
   }
   else
   {
      nOffset = tzERDIAGRAMX;
      nIncr = 1;
      cSetBits = (tziTEXT | tziTEXTHORIZ);
   }

   // Now go through the text drawing area and erase all cells which have
   // text in them.
   while ( nLth )
   {
      if ( (pchGrid[ lIdx ] & cSetBits) == cSetBits )
      {
         pchGrid[ lIdx ] &= (0xFF - cSetBits);
         if ( (pchGrid[ lIdx ] & tziENTITY) == 0 )
            m_pER_Diagram->DrawDiagramCell( lIdx, TRUE );

         if ( (pchGrid[ lIdx + nOffset ] & cSetBits) == cSetBits )
         {
            pchGrid[ lIdx + nOffset ] &= (0xFF - cSetBits);
// DKS???   if ( (pchGrid[ lIdx ] & tziENTITY) == 0 )
            if ( (pchGrid[ lIdx + nOffset ] & tziENTITY) == 0 )
               m_pER_Diagram->DrawDiagramCell( lIdx + nOffset, TRUE );
         }

         if ( (pchGrid[ lIdx - nOffset ] & cSetBits) == cSetBits )
         {
            pchGrid[ lIdx - nOffset ] &= (0xFF - cSetBits);
// DKS???   if ( (pchGrid[ lIdx ] & tziENTITY) == 0 )
            if ( (pchGrid[ lIdx - nOffset ] & tziENTITY) == 0 )
               m_pER_Diagram->DrawDiagramCell( lIdx - nOffset, TRUE );
         }
      }

      lIdx += nIncr;
      nLth--;
   }

   // Force recalculation of text after erasure
   m_lTextIdx = 0;
   m_nTextLth = 0;
   m_nTextDir = 0;
}

//
// METHOD: ZER_Relationship::Select
//
//
void
ZER_Relationship::Select( zBOOL bToggle, zBOOL bClear )
{
   zBOOL  bRelease;
   CPen   *pPen;
   CPen   *pPenOld;
   zSHORT nItemPos;
   ZER_Relationship *pRelationship;

   if ( m_pER_Diagram->m_hDC == 0 )
   {
      m_pER_Diagram->GetDeviceContext( );
      bRelease = TRUE;
      pPen = new CPen( PS_SOLID, 1, tzERCOLORLINES );
      pPenOld = m_pER_Diagram->m_hDC->SelectObject( pPen );
   }
   else
      bRelease = FALSE;

   // Deselect all selected entities if deselect requested.
   if ( bClear )
   {
      for ( nItemPos = 0; nItemPos < m_pER_Diagram->m_nRelCnt; nItemPos++ )
      {
         pRelationship = m_pER_Diagram->m_RelationshipList[ nItemPos ];
         if ( pRelationship->m_bSelected && pRelationship != this )
            pRelationship->Deselect( );
      }
   }
   else
   if ( m_bSelected && bToggle )
   {
      Deselect( );
      if ( bRelease )
      {
         m_pER_Diagram->m_hDC->SelectObject( pPenOld );
         delete( pPen );
         m_pER_Diagram->ReleaseDeviceContext( );
      }

      return;
   }

   m_bSelected = 1;
   if ( m_lStartIdx )
   {
      m_pER_Diagram->ColorCell( m_lStartIdx, tzERCOLORSELECTED );
      m_pER_Diagram->DrawDiagramCell( m_lStartIdx );
      if ( m_lArcIdx1 )
      {
         m_pER_Diagram->ColorCell( m_lArcIdx1, tzERCOLORSELECTED );
         m_pER_Diagram->DrawDiagramCell( m_lArcIdx1 );
      }

      if ( m_lArcIdx2 )
      {
         m_pER_Diagram->ColorCell( m_lArcIdx2, tzERCOLORSELECTED );
         m_pER_Diagram->DrawDiagramCell( m_lArcIdx2 );
      }

      if ( m_lArcIdx3 )
      {
         m_pER_Diagram->ColorCell( m_lArcIdx3, tzERCOLORSELECTED );
         m_pER_Diagram->DrawDiagramCell( m_lArcIdx3 );
      }

      m_pER_Diagram->ColorCell( m_lEndIdx, tzERCOLORSELECTED );
      m_pER_Diagram->DrawDiagramCell( m_lEndIdx );
   }

   if ( bRelease )
   {
      m_pER_Diagram->m_hDC->SelectObject( pPenOld );
      delete( pPen );
      m_pER_Diagram->ReleaseDeviceContext( );
   }
}

//
// METHOD: ZER_Relationship::Deselect
//
//
void
ZER_Relationship::Deselect( )
{
   zBOOL bRelease;
   CPen  *pPen;
   CPen  *pPenOld;

   if ( m_bSelected == 0 )
     return;

   m_bSelected = 0;
   if ( m_lStartIdx )
   {
      if ( m_pER_Diagram->m_hDC == 0 )
      {
         m_pER_Diagram->GetDeviceContext( );
         bRelease = TRUE;
         pPen = new CPen( PS_SOLID, 1, tzERCOLORLINES );
         pPenOld = m_pER_Diagram->m_hDC->SelectObject( pPen );
      }
      else
         bRelease = FALSE;

      m_pER_Diagram->ColorCell( m_lStartIdx, tzERCOLORBACKGROUND );
      m_pER_Diagram->DrawDiagramCell( m_lStartIdx );
      if ( m_lArcIdx1 )
      {
         m_pER_Diagram->ColorCell( m_lArcIdx1, tzERCOLORBACKGROUND );
         m_pER_Diagram->DrawDiagramCell( m_lArcIdx1 );
      }

      if ( m_lArcIdx2 )
      {
         m_pER_Diagram->ColorCell( m_lArcIdx2, tzERCOLORBACKGROUND );
         m_pER_Diagram->DrawDiagramCell( m_lArcIdx2 );
      }

      if ( m_lArcIdx3 )
      {
         m_pER_Diagram->ColorCell( m_lArcIdx3, tzERCOLORBACKGROUND );
         m_pER_Diagram->DrawDiagramCell( m_lArcIdx3 );
      }

      m_pER_Diagram->ColorCell( m_lEndIdx, tzERCOLORBACKGROUND );
      m_pER_Diagram->DrawDiagramCell( m_lEndIdx );

      if ( bRelease )
      {
         m_pER_Diagram->m_hDC->SelectObject( pPenOld );
         delete( pPen );
         m_pER_Diagram->ReleaseDeviceContext( );
      }
   }
}

//
// METHOD: ZER_Relationship::ToggleSelect
//
//
void
ZER_Relationship::ToggleSelect( )
{
   if ( m_bSelected )
      Deselect( );
   else
      Select( );
}
