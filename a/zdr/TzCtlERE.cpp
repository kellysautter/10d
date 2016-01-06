/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlere.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Entity/Relationship control's
// ZER_Entity helper class.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
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

// #define DEBUG_ALL

// Class Implementation for class ZER_Entity
ZER_Entity::ZER_Entity( ZER_Diagram * pER_Diagram,
                        zLONG  lPosX,
                        zLONG  lPosY,
                        zPCHAR pchText,
                        zCHAR  chType,
                        zLONG  lPosZKey,
                        zLONG  lZKey,
                        zLONG  lPortFactor,
                        zBOOL  bSelect )
{
   m_pER_Diagram = pER_Diagram;
   m_lZKey = lZKey;
   m_lPosZKey = lPosZKey;
   m_bSelected = bSelect;
   if ( lPortFactor > 1 )
   {
      m_lPortFactor = lPortFactor;
      m_lEntPortX = (lPortFactor * tzERENTITYX);
      m_lEntPortY = (lPortFactor * tzERENTITYY);
      if ( m_lEntPortX % 2 == 0 )   // force it to be odd (aesthetics)
         m_lEntPortX -= 1;

      if ( m_lEntPortY % 2 == 0 )   // force it to be odd (aesthetics)
         m_lEntPortY -= 1;
   }
   else
   {
      m_lPortFactor = 1;
      m_lEntPortX = tzERENTITYX;
      m_lEntPortY = tzERENTITYY;
   }

   // these next several lines prevent going off the end of the huge array
   if ( lPosX < 0 )
      lPosX = -lPosX;

   if ( lPosY < 0 )
      lPosY = -lPosY;

   m_lPosIdx  = (lPosY * tzERDIAGRAMX) + lPosX;
   while ( m_lPosIdx >= ((zLONG) tzERDIAGRAMX * (zLONG) tzERDIAGRAMY) )
      m_lPosIdx >>= 1;  // divide by 2

   m_bUpdated  = 0;
   if ( pchText && *pchText )
      m_csText = pchText;

   m_chType = chType;
   MarkEntity( );
}

ZER_Entity::~ZER_Entity( )
{
}

zSHORT
ZER_Entity::PosX( zLONG lIdx )
{
   if ( lIdx == -1 )
      return( (zSHORT) (m_lPosIdx % tzERDIAGRAMX) );
   else
      return( (zSHORT) (lIdx % tzERDIAGRAMX) );
}

zSHORT
ZER_Entity::PosY( zLONG lIdx )
{
   if ( lIdx == -1 )
      return( (zSHORT) (m_lPosIdx / tzERDIAGRAMX) );
   else
      return( (zSHORT) (lIdx / tzERDIAGRAMX) );
}

void
HyphenateER_Text( zCPCHAR cpcText, zPCHAR pchReturn, zLONG lMaxLth, zLONG lIdx )
{
   zBOOL bGoodBreak = FALSE;
   zLONG k;

   strcpy_s( pchReturn, lMaxLth, cpcText );
// TraceLineS( "HyphenateCheck ", cpcText );

   while ( lIdx > 5 && lIdx > (zLONG) (zstrlen( cpcText ) / 2) )  // purist's goto
   {
      zPCHAR  pchTemp;
      zPCHAR  pch;

      // check for lowercase/uppercase change
      k = lIdx + 1;
      while ( k > lIdx - 5 )
      {
         if ( pchReturn[ k ] == '_' || (isupper( pchReturn[ k ] ) && islower( pchReturn[ k - 1 ] )) )
         {
            if ( k <= lIdx )
            {
               bGoodBreak = TRUE;
               if ( pchReturn[ k ] == '_' )
                  lIdx = k;
               else
                  lIdx = k - 1;

               break;
            }
         }

         k--;
      }

      if ( k > lIdx - 5 )
         break;  // we found a good place to break

      // check for standard endings that would make a good place to break the text
      pchTemp = pchReturn + lIdx - 5;
      if ( (pch = zstrstr( pchTemp, "ing" )) != 0 ||
           (pch = zstrstr( pchTemp, "able" )) != 0 ||
           (pch = zstrstr( pchTemp, "tion" )) != 0 ||
           (pch = zstrstr( pchTemp, "bute" )) != 0 ||
           (pch = zstrstr( pchTemp, "er" )) != 0 )
      {
         if ( pch < pchReturn + lIdx )
         {
            k = pchReturn + lIdx - pch;
            lIdx -= k;
         }
      }

      break;  // get out of purist's goto (while)
   }

   k = ++lIdx;
   if ( bGoodBreak == FALSE )
      pchReturn[ lIdx++ ] = '-';

   pchReturn[ lIdx++ ] = '\n';
   strcpy_s( pchReturn + lIdx, lMaxLth - lIdx, cpcText + k );
}

zSHORT
ZER_Entity::DrawEntity( zBOOL bErase )
{
   CPen     *pPen = 0;      // not sure we have to zero these out ...
   CBrush   *pBrush = 0;    // but because we are a little neurotic
   CFont    *pFont = 0;     // we will do it regardless
   CRect    rect;
   CSize    Size;
   zBOOL    bRelease;
   zCHAR    szText[ 256 ];
   COLORREF colorOldText;

   // Now paint a rounded rectangle to represent the entity
// if ( !m_pER_Diagram->PositionVisible( m_lPosIdx,         // DKS 4/1/96
//      m_lPosIdx + (tzERENTITYX - 1) + ((tzERENTITYY - 1) * tzERDIAGRAMX)) )
   if ( !m_pER_Diagram->PositionVisible( m_lPosIdx,
        m_lPosIdx + (m_lEntPortX - 1) + ((m_lEntPortY - 1) * tzERDIAGRAMX)) )
   {
      return( 0 );      // no sense drawing what's not in view
   }

   // If the entity has not been marked in the grid, mark it now
   if ( (m_pER_Diagram->m_pchGrid[ m_lPosIdx ] & tziENTITY) == 0 )
      MarkEntity( );

   if ( m_pER_Diagram->m_hDC == 0 )
   {
      m_pER_Diagram->GetDeviceContext( );
      bRelease = TRUE;
   }
   else
      bRelease = FALSE;

   if ( m_pER_Diagram->m_bPrintDC )
   {
      if ( m_bSelected && m_pER_Diagram->m_bShadeEnts )
         pBrush = new CBrush( tzERCOLORSHADE );
      else
         pBrush = new CBrush( tzERCOLORWHITE );

      if ( m_chType == 'D' )
         pPen = new CPen( PS_DASH, 1, tzERCOLORBLACK );
      else
      if ( m_chType == 'W' )
         pPen = new CPen( PS_DASHDOTDOT, 1, tzERCOLORBLACK );
      else
         pPen = new CPen( PS_SOLID, 1, tzERCOLORBLACK );

      colorOldText = m_pER_Diagram->m_hDC->SetTextColor( tzERCOLORBLACK );
   }
   else
   {
      if ( m_chType == 'D' )
         pPen = new CPen( PS_DASH, 1, tzERCOLORBOX );
      else
      if ( m_chType == 'W' )
         pPen = new CPen( PS_DASHDOTDOT, 1, tzERCOLORBOX );
      else
         pPen = new CPen( PS_SOLID, 1, tzERCOLORBOX );

      colorOldText = m_pER_Diagram->m_hDC->SetTextColor( tzERCOLORTEXT );
      if ( m_bSelected )
         pBrush = new CBrush( tzERCOLORSELECTED );
      else
         pBrush = new CBrush( tzERCOLORNORMAL );
   }

   CPen *pPenOld = m_pER_Diagram->m_hDC->SelectObject( pPen );
   CBrush *pBrushOld = m_pER_Diagram->m_hDC->SelectObject( pBrush );

   // NOTE, Orientation 1 used instead of 0 because of the way Truetype handles scaling of non-standard font sizes
   pFont = new CFont( );
   pFont->CreateFont( (tzERFONTHEIGHT * 5) / 4, 0, 1, 1, 0, 0, 0, 0, ANSI_CHARSET,
                      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, tzERFONTFF, tzERFONTNAME );
   CFont *pFontOld = m_pER_Diagram->m_hDC->SelectObject( pFont );

   rect.left = (PosX( ) - PosX( tzERDISPLAYPOS )) * tzERCELLSIZE;
   rect.top  = (PosY( ) - PosY( tzERDISPLAYPOS )) * tzERCELLSIZE;
   rect.right  = rect.left + ((m_lEntPortX * tzERCELLSIZE) - 1);
   rect.bottom = rect.top  + ((m_lEntPortY * tzERCELLSIZE) - 1);

#if 0
// if ( rect.left % 9 || rect.top % 9 )
// {
      TraceLineI( "tzERDISPLAYPOSX = ", PosX( tzERDISPLAYPOS ) );
      TraceLineI( "tzERDISPLAYPOSY = ", PosY( tzERDISPLAYPOS ) );
      TraceLineI( "Scale Factor = ", m_pER_Diagram->m_lScale );
      TraceLineI( "Entity rect.top = ", rect.top );
      TraceLineI( "Entity rect.left = ", rect.left );
      TraceLineI( "Entity rect.bottom = ", rect.bottom );
      TraceLineI( "Entity rect.right = ", rect.right );
// }
#endif

   if ( bErase )
   {
      if ( m_pER_Diagram->m_lScale > 0 )
      {
         rect.top--;
         rect.left--;
         rect.bottom++;
         rect.right++;
      }

      CBrush *pBrushPrev = pBrush;
      CPen   *pPenPrev = pPen;
      pPen = new CPen( PS_SOLID, 1, tzERCOLORBACKGROUND );
      pBrush = new CBrush( tzERCOLORBACKGROUND );
      m_pER_Diagram->m_hDC->SelectObject( *pBrush );
      m_pER_Diagram->m_hDC->SelectObject( *pPen );
      delete( pBrushPrev );
      delete( pPenPrev );
   }

   m_pER_Diagram->m_hDC->RoundRect( rect.left, rect.top, rect.right, rect.bottom, tzERARCSIZE * m_lPortFactor, tzERARCSIZE * m_lPortFactor );
   if ( bErase == FALSE )
   {
      // Now that the rectangle is drawn, see if we need to draw the attributive or associative lines in the entity
      if ( m_chType == 'A' )
      {
         if ( m_pER_Diagram->m_bUpdateView &&
              m_pER_Diagram->m_bPrintDC == FALSE )
         {
            CPen *pPen2 = pPen;
            pPen = new CPen( PS_SOLID, 1, tzERCOLORGRAY );
            m_pER_Diagram->m_hDC->SelectObject( *pPen );
            delete( pPen2 );
         }

         CPoint   pt1;
         CPoint   pt2;

         pt1.x = (int) (((zLONG) rect.left + rect.right) / 2);
         pt1.y = rect.top;
         m_pER_Diagram->m_hDC->MoveTo( pt1.x, pt1.y );
         pt2.x = rect.left;
         pt2.y = rect.top + ((m_lEntPortY - 1) * tzERCELLSIZE );
         m_pER_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         m_pER_Diagram->m_hDC->MoveTo( pt1.x, pt1.y );
         pt2.x = rect.right;
         m_pER_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
      }
      else
      if ( m_chType == 'S' )
      {
         if ( m_pER_Diagram->m_bUpdateView &&
              m_pER_Diagram->m_bPrintDC == FALSE )
         {
            CPen *pPen2 = pPen;
            pPen = new CPen( PS_SOLID, 1, tzERCOLORGRAY );
            m_pER_Diagram->m_hDC->SelectObject( pPen );
            delete( pPen2 );
         }

         CPoint   pt1;
         CPoint   pt2;

         pt1.x = (int) (((zLONG) rect.left + rect.right) / 2);
         pt1.y = rect.top;
         m_pER_Diagram->m_hDC->MoveTo( pt1.x, pt1.y );
         pt2.x = rect.left;
         pt2.y = (rect.top + rect.bottom) / 2;
         m_pER_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         pt2.x = pt1.x;
         pt2.y = rect.bottom;
         m_pER_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         pt2.x = rect.right;
         pt2.y = (rect.top + rect.bottom) / 2;
         m_pER_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         m_pER_Diagram->m_hDC->LineTo( pt1.x, pt1.y );
      }

      int nOldBkMode = m_pER_Diagram->m_hDC->SetBkMode( TRANSPARENT );
      Size = m_pER_Diagram->m_hDC->GetTextExtent( m_csText );
      rect.left   += 8;
      rect.right  -= 8;
      if ( Size.cx > (rect.right - rect.left) )
      {
         rect.top += ((tzERCELLSIZE * 3) / 4);

         zSHORT nIdx = 0;
         zSHORT nLth = m_csText.GetLength( ) - 1;
         while ( nIdx < nLth && m_csText.GetAt( nIdx ) != ' ' )
            nIdx++;

         Size = m_pER_Diagram->m_hDC->GetTextExtent( m_csText, nIdx );
         if ( Size.cx > (rect.right - rect.left) )
         {
            while ( nIdx && Size.cx > (rect.right - rect.left) )
            {
               nIdx--;
               Size = m_pER_Diagram->m_hDC->GetTextExtent( m_csText, nIdx );
            }

            nIdx--;
            if ( nIdx > 0 )
            {
               HyphenateER_Text( m_csText, szText, sizeof( szText ), nIdx );
            }

            m_pER_Diagram->m_hDC->DrawText( szText, -1, rect, DT_LEFT );
   //                                                         DT_LEFT | DT_WORDBREAK );
   //                                                         DT_SINGLELINE | DT_LEFT | DT_VCENTER );
         }
         else
         {
   //       rect.top += ((tzERCELLSIZE * 3) / 4);
            m_pER_Diagram->m_hDC->DrawText( m_csText, -1, rect, DT_CENTER | DT_WORDBREAK );
         }
      }
      else
      {
         m_pER_Diagram->m_hDC->DrawText( m_csText, -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
      }

      m_pER_Diagram->m_hDC->SetBkMode( nOldBkMode );
   }

   m_pER_Diagram->m_hDC->SelectObject( pPenOld );
   m_pER_Diagram->m_hDC->SelectObject( pBrushOld );
   m_pER_Diagram->m_hDC->SelectObject( pFontOld );
   m_pER_Diagram->m_hDC->SetTextColor( colorOldText );
   delete( pPen );
   delete( pBrush );
   delete( pFont );
   if ( bRelease )
      m_pER_Diagram->ReleaseDeviceContext( );

   // return that entity was drawn
   return( 1 );
}

void
ZER_Entity::MarkEntity( zBOOL bErase )
{
   zPCHAR pchWork;
   zLONG  lIdx;
   zSHORT nWk1;
   zSHORT nWk2;

   if ( m_pER_Diagram->m_pchGrid == 0 )
      return;

   // Now mark where the entity is in the grid
   lIdx = m_lPosIdx;
   pchWork = m_pER_Diagram->m_pchGrid;

   // Note: m_lEntPortX ~= (lPortFactor * tzERENTITYX);  // Factor * 9
   //       m_lEntPortY ~= (lPortFactor * tzERENTITYY);  // Factor * 5

// for ( nWk1 = 0; nWk1 < tzERENTITYY; nWk1++ )       // DKS 4/1/96 (ports)
   for ( nWk1 = 0; nWk1 < m_lEntPortY; nWk1++ )
   {
//    for ( nWk2 = 0; nWk2 < tzERENTITYX; nWk2++ )    // DKS 4/1/96 (ports)
      for ( nWk2 = 0; nWk2 < m_lEntPortX; nWk2++ )
      {
         if ( bErase )
         {
            pchWork[ lIdx ] &= (0xFF - tziENTITY);
            if ( pchWork[ lIdx ] )
               m_pER_Diagram->DrawDiagramCell( lIdx );
         }
         else
            pchWork[ lIdx ] |= tziENTITY;

         lIdx++;
      }

//    lIdx -= tzERENTITYX;
      lIdx -= m_lEntPortX;
      lIdx += tzERDIAGRAMX;
   }
}

// bUnmark defaults to TRUE
void
ZER_Entity::Erase( zBOOL bUnmark )
{
   CRect rect;

   // Erase the rectangle containing the entity
   rect.left   = (PosX( ) - PosX( tzERDISPLAYPOS )) * tzERCELLSIZE;
   rect.top    = (PosY( ) - PosY( tzERDISPLAYPOS )) * tzERCELLSIZE;
   rect.right  = rect.left + ((m_lEntPortX * tzERCELLSIZE) - 1);
   rect.bottom = rect.top  + ((m_lEntPortY * tzERCELLSIZE) - 1);

#if 1
   if ( m_pER_Diagram->m_lScale > 0 )
   {
      rect.top--;
      rect.left--;
      rect.bottom++;
      rect.right++;
//    TraceLineI( "Erase rect.top = ", rect.top );
//    TraceLineI( "Erase rect.left = ", rect.left );
//    TraceLineI( "Erase rect.bottom = ", rect.bottom );
//    TraceLineI( "Erase rect.right = ", rect.right );
   }

   // we are going to invalidate the rect so that it will be repainted
   // and also redraw the entity to erase it.
   m_pER_Diagram->InvalidateRect( rect, TRUE );
   DrawEntity( TRUE );
#else
   CBrush  *pBrush;
   CPen    *pPen;
   zBOOL   bRelease;

   if ( m_pER_Diagram->m_hDC == 0 )
   {
      m_pER_Diagram->GetDeviceContext( );
      bRelease = TRUE;
   }
   else
      bRelease = FALSE;

   CPen penOld( (HPEN) m_pER_Diagram->m_hDC->GetCurrentObject( OBJ_PEN ) );
   CBrush brushOld( (HBRUSH) m_pER_Diagram->m_hDC->GetCurrentObject( OBJ_BRUSH ) );

   // Now un-paint the rectangle where the entity was
   pBrush = new CBrush( tzERCOLORBACKGROUND );
   pPen   = new CPen( PS_SOLID, m_pER_Diagram->m_lScale > 0 ? 2 : 1,
                      tzERCOLORBACKGROUND );
   m_pER_Diagram->m_hDC->SelectObject( *pPen );
   m_pER_Diagram->m_hDC->SelectObject( *pBrush );
   m_pER_Diagram->m_hDC->Rectangle( rect );
// m_pER_Diagram->m_hDC->RoundRect( rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1, tzERARCSIZE, tzERARCSIZE );

   m_pER_Diagram->m_hDC->SelectObject( penOld );
   m_pER_Diagram->m_hDC->SelectObject( brushOld );
   delete( pBrush );
   delete( pPen );

   if ( bRelease )
      m_pER_Diagram->ReleaseDeviceContext( );
#endif

   if ( bUnmark )
   {
      // Now unmark where the entity is in the grid
      MarkEntity( 1 );
   }
}

zBOOL
ZER_Entity::IndexInEntity( zLONG lIdx )
{
   zLONG    lWkIdx;
   zSHORT   nWk;

   lWkIdx = m_lPosIdx;
   for ( nWk = 0; nWk < tzERENTITYY; nWk++ )
   {
      if ( lIdx >= lWkIdx && lIdx < (lWkIdx + tzERENTITYX) )
         return( 1 );

      lWkIdx += tzERDIAGRAMX;
   }

   return( 0 );
}

void
ZER_Entity::Move( zLONG lNewPosX, zLONG lNewPosY, zBOOL bRecalcRels )
{
   zLONG    lIdx, lNewPosIdx;
   zSHORT   nItemPos;
   zLONG    lWk1, lWk2;
   ZER_Relationship *pRelationship;
   zPCHAR   pchGrid;

   // Now check to see that an entity will fit in the grid
   if ( lNewPosX <= 0 || (lNewPosX + tzERENTITYX) >= tzERDIAGRAMX ||
        lNewPosY <= 0 || (lNewPosY + tzERENTITYY) >= tzERDIAGRAMY )
   {
      return;
   }

   pchGrid = m_pER_Diagram->m_pchGrid;
   lIdx = ((zLONG) lNewPosY * tzERDIAGRAMX) + lNewPosX;
   lIdx -= (2 * tzERDIAGRAMX);
   lIdx -= 2;
   for ( lWk1 = 0; lWk1 < (tzERENTITYY + 4); lWk1++ )
   {
      for ( lWk2 = 0; lWk2 < (tzERENTITYX + 4); lWk2++ )
      {
         if ( lIdx >= 0 && lIdx <= tzERMAXGRID && (pchGrid[ lIdx ] & tziENTITY) &&
//                                                (pchGrid[ lIdx ] & (tziENTITY | tziTEXT)) &&
              !(IndexInEntity( lIdx )) )
         {
            return;
         }

         lIdx++;
      }

      lIdx -= (tzERENTITYX + 4);
      lIdx += tzERDIAGRAMX;
   }

   lNewPosIdx = ((zLONG) lNewPosY * tzERDIAGRAMX) + lNewPosX;

   if ( PosY( lNewPosIdx ) != PosY( lNewPosIdx + (tzERENTITYX - 1) ) ||
        (lNewPosIdx + (tzERENTITYX - 1) + ((zLONG) (tzERENTITYY - 1) * tzERDIAGRAMX )) > tzERMAXGRID )
   {
      return;
   }

   CWaitCursor wait;

   if ( m_pER_Diagram->m_lScale && m_pER_Diagram->m_bScrolledWindow )
      m_pER_Diagram->SetRedraw( FALSE );

   for ( nItemPos = 0; nItemPos < m_pER_Diagram->m_nRelCnt; nItemPos++ )
   {
      pRelationship = m_pER_Diagram->m_RelationshipList[ nItemPos ];
      if ( pRelationship->m_pSrcEntity == this || pRelationship->m_pTgtEntity == this )
      {
         pRelationship->Erase( );
      }
   }

   Erase( );
   m_lPosIdx = lNewPosIdx;
   m_bUpdated = 1;
   MarkEntity( );
   DrawEntity( );

   // We do two passes with the relationships, the first calculates
   // the best lines with the other lines uncalculated, the second
   // calculates and draws the lines with the first set calculated.
   // This will re-route lines which have a better path after all
   // lines are calculated.
   if ( bRecalcRels )
   {
      zBOOL  bDisplayMsgBox = FALSE;
      zSHORT nCnt = 0;

      for ( nItemPos = 0; nItemPos < m_pER_Diagram->m_nRelCnt; nItemPos++ )
      {
         pRelationship = m_pER_Diagram->m_RelationshipList[ nItemPos ];
         if ( pRelationship->m_pSrcEntity == this || pRelationship->m_pTgtEntity == this )
         {
            nCnt += pRelationship->CalculateLine( FALSE, nCnt );
         }
      }


      for ( nItemPos = 0; nItemPos < m_pER_Diagram->m_nRelCnt; nItemPos++ )
      {
         pRelationship = m_pER_Diagram->m_RelationshipList[ nItemPos ];
         if ( pRelationship->m_pSrcEntity == this || pRelationship->m_pTgtEntity == this )
         {
            nCnt += pRelationship->CalculateLine( FALSE, nCnt );
            if ( pRelationship->DrawRelationship( FALSE, bDisplayMsgBox, nCnt ) < 0 )
            {
               bDisplayMsgBox = FALSE;
               nCnt++;
            }

            // added below dks 3/5/96
            if ( pRelationship->m_pSrcEntity != this )
               pRelationship->m_pSrcEntity->DrawEntity( FALSE );

            if ( pRelationship->m_pTgtEntity != this )
               pRelationship->m_pTgtEntity->DrawEntity( FALSE );
            // added above dks 3/5/96
         }
      }
   }

   // If the diagram is Scaled, then invalidate the diagram since erasing a scaled diagram has problems
   if ( m_pER_Diagram->m_lScale && m_pER_Diagram->m_bScrolledWindow )
   {
      m_pER_Diagram->SetRedraw( TRUE );
      m_pER_Diagram->Invalidate( TRUE );
      m_pER_Diagram->m_bScrolledWindow = FALSE;
   }
}

void
ZER_Entity::Select( zBOOL bToggle, zBOOL bClear )
{
   zSHORT      nItemPos;
   ZER_Entity  *pEntity;
   zBOOL       bRelease;

   if ( m_pER_Diagram->m_hDC == 0 )
   {
      m_pER_Diagram->GetDeviceContext( );
      bRelease = TRUE;
   }
   else
      bRelease = FALSE;

   // Deselect all selected entities if deselect requested.
   if ( bClear )
   {
      for ( nItemPos = 0; nItemPos < m_pER_Diagram->m_nEntCnt; nItemPos++ )
      {
         pEntity = m_pER_Diagram->m_EntityList[ nItemPos ];
         if ( pEntity->m_bSelected && pEntity != this )
            pEntity->Deselect( );
      }
   }
   else
   if ( m_bSelected && bToggle )
   {
      Deselect( );
      if ( bRelease )
         m_pER_Diagram->ReleaseDeviceContext( );

      return;
   }

   // First, select myself.
   m_bSelected = 1;
   DrawEntity( );

   if ( bRelease )
      m_pER_Diagram->ReleaseDeviceContext( );
}

void
ZER_Entity::Deselect( )
{
   if ( !m_bSelected )
      return;

   m_bSelected = 0;
   DrawEntity( );
}

void
ZER_Entity::SelectNeighbors( zBOOL bSelect,
                             zSHORT nSelectRels,
                             zBOOL  bClear )
{
   zSHORT   nItemPos;
   ZER_Relationship *pRelationship;
   zBOOL    bRelease;
   zVIEW    vDiagram;

   if ( m_pER_Diagram->m_hDC == 0 )
   {
      m_pER_Diagram->GetDeviceContext( );
      bRelease = TRUE;
   }
   else
      bRelease = FALSE;

   vDiagram = 0;
   GetViewByName( &vDiagram, *(m_pER_Diagram->m_pzsVName),
                  m_pER_Diagram->m_pZSubtask->m_vDialog, zLEVEL_ANY );
   if ( bClear )
   {
      if ( vDiagram )
      {
         SetAllSelStatesForEntityForSet( vDiagram, *(m_pER_Diagram->m_pzsEName), 0, 1, 0 );
         if ( nSelectRels )
         {
            SetAllSelStatesForEntityForSet( vDiagram, *(m_pER_Diagram->m_pzsAName), 0, 1, 0 );
         }
      }

      Select( );  // Select myself to clear all entity selections
      if ( nSelectRels && m_pER_Diagram->m_nRelCnt )
      {
         pRelationship = m_pER_Diagram->m_RelationshipList[ 0 ];
         pRelationship->Select( );   // Select and deselect first rel
         pRelationship->Deselect( ); // to clear all rel selections
      }
   }
   else
   {
      if ( bSelect )
         Select( 0, 0 );   // Select myself with no toggle and no clear
      else
         Deselect( );       // Clear my selection
   }

   // If deselect and clear were both specified, then there is nothing else to do but deselect ourself and return.
   if ( !bSelect && bClear )
   {
      Deselect( );
      if ( bRelease )
         m_pER_Diagram->ReleaseDeviceContext( );

      return;
   }

   // Now go through and select my neighbors
   for ( nItemPos = 0; nItemPos < m_pER_Diagram->m_nRelCnt; nItemPos++ )
   {
      pRelationship = m_pER_Diagram->m_RelationshipList[ nItemPos ];
      if ( pRelationship->m_pSrcEntity == this )
      {
         if ( nSelectRels != 2 )
         {
            if ( bSelect )
               pRelationship->m_pTgtEntity->Select( 0, 0 );
            else
               pRelationship->m_pTgtEntity->Deselect( );
            if ( vDiagram && (bSelect || !bClear) )
            {
               m_pER_Diagram->PositionOnZeidonEntity( pRelationship->m_pTgtEntity, vDiagram, 1, 1 );
               SetSelectStateOfEntity( vDiagram, *(m_pER_Diagram->m_pzsEName), bSelect );
            }
         }

         if ( nSelectRels )
         {
            if ( bSelect )
               pRelationship->Select( 0, 0 );
            else
               pRelationship->Deselect( );

            if ( vDiagram && (bSelect || !bClear) )
            {
               m_pER_Diagram->PositionOnZeidonRelationship( pRelationship, vDiagram, 1, 1 );
               SetSelectStateOfEntity( vDiagram, *(m_pER_Diagram->m_pzsAName), bSelect );
            }
         }
      }
      else
      if ( pRelationship->m_pTgtEntity == this )
      {
         if ( nSelectRels != 2 )
         {
            if ( bSelect )
               pRelationship->m_pSrcEntity->Select( 0, 0 );
            else
               pRelationship->m_pSrcEntity->Deselect( );

            if ( vDiagram )
            {
               m_pER_Diagram->PositionOnZeidonEntity( pRelationship->m_pSrcEntity, vDiagram, 1, 1 );
               SetSelectStateOfEntity( vDiagram, *(m_pER_Diagram->m_pzsEName), bSelect );
            }
         }

         if ( nSelectRels )
         {
            if ( bSelect )
               pRelationship->Select( 0, 0 );
            else
               pRelationship->Deselect( );

            if ( vDiagram && (bSelect || !bClear) )
            {
               m_pER_Diagram->PositionOnZeidonRelationship( pRelationship, vDiagram, 1, 1 );
               SetSelectStateOfEntity( vDiagram, *(m_pER_Diagram->m_pzsAName), bSelect );
            }
         }
      }
   }

   if ( bRelease )
      m_pER_Diagram->ReleaseDeviceContext( );
}

void
ZER_Entity::CenterEntity( )
{
   CRect rect;
   zLONG  lDisplaySizeX, lDisplaySizeY;
   zSHORT nNodeMiddleX, nNodeMiddleY;
   zLONG  lDisplayPosX, lDisplayPosY;


   m_pER_Diagram->GetClientRect( rect );

   // Get the logical rect size with scaling
   m_pER_Diagram->CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY, rect, m_pER_Diagram->m_lScale );

   lDisplaySizeX /= 2;
   lDisplaySizeY /= 2;

   nNodeMiddleX = PosX( ) + (tzERENTITYX / 2);
   nNodeMiddleY = PosY( ) + (tzERENTITYY / 2);

   lDisplayPosX = nNodeMiddleX - lDisplaySizeX;
   lDisplayPosY = nNodeMiddleY - lDisplaySizeY;

   if ( lDisplayPosX < 0 )
      lDisplayPosX = 0;

   if ( lDisplayPosY < 0 )
      lDisplayPosY = 0;

   m_pER_Diagram->m_lDisplayPos = lDisplayPosY;
   m_pER_Diagram->m_lDisplayPos *= tzERDIAGRAMX;
   m_pER_Diagram->m_lDisplayPos += lDisplayPosX;

   // Here we zoom the diagram to the current scale to ensure the display position is altered
   // if necessary so the right and bottom edge aren't beyond the diagram space.
   m_pER_Diagram->ZoomDiagram( (zSHORT) (m_pER_Diagram->m_lScale + 100), 1 );
}
