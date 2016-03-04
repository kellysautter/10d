/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlhdn.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Hierarchical diagramming node.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.07.06    DKS    Z10
//    Fixed font problem when selecting font that was not created and
//    deleting font that was selected.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlHd.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Class Implementation for class ZHD_Node
ZHD_Node::ZHD_Node( ZHD_Diagram *pHD_Diagram,
                    ZHD_Node *pParentNode,
                    zLONG  lPosX,
                    zLONG  lPosY,
                    zPCHAR pchText,
                    zPCHAR pchTitle,
                    zPCHAR pchDIL,
                    zPCHAR pchOutsideText1,
                    zPCHAR pchOutsideText2,
                    zPCHAR pchOutsideText3,
                    zPCHAR pchOutsideText4,
                    zPCHAR pchOutsideText5,
                    zPCHAR pchType,
                    zLONG  lZKey,
                    zBOOL  bSelect,
                    zLONG  lColor )
{
   m_pHD_Diagram = pHD_Diagram;
   m_pParentNode = pParentNode;
   m_lZKey = lZKey;
   m_lColor = lColor;
   m_bSelected = bSelect;
   m_lPosIdx = (lPosY * tzHDDIAGRAMX) + lPosX;
   if ( pchText && *pchText )
      m_csText = pchText;

   if ( pchTitle && *pchTitle )
      m_csTitle = pchTitle;

   if ( pchDIL && *pchDIL )
      m_csDIL = pchDIL;

   if ( pchOutsideText1 && *pchOutsideText1 )
      m_csOutsideText1 = pchOutsideText1;

   if ( pchOutsideText2 && *pchOutsideText2 )
      m_csOutsideText2 = pchOutsideText2;

   if ( pchOutsideText3 && *pchOutsideText3 )
      m_csOutsideText3 = pchOutsideText3;

   if ( pchOutsideText4 && *pchOutsideText4 )
      m_csOutsideText4 = pchOutsideText4;

   if ( pchOutsideText5 && *pchOutsideText5 )
      m_csOutsideText5 = pchOutsideText5;

   if ( pchType && *pchType )
      m_chType = *pchType;
   else
      m_chType = 0;
}

ZHD_Node::~ZHD_Node( )
{
}

void
ZHD_Node::Select( zVIEW vDiagram )
{
   zSHORT   nPos;
   ZHD_Node *pWkNode;

   if ( m_bSelected )
      return;

   for ( nPos = 0; nPos < m_pHD_Diagram->m_nNodeCnt; nPos++ )
   {
      pWkNode = m_pHD_Diagram->m_NodeList[ nPos ];
      if ( pWkNode->m_bSelected )
      {
         pWkNode->m_bSelected = 0;
         pWkNode->DrawNode( );
         vDiagram = m_pHD_Diagram->PositionOnDiagramNode( pWkNode, vDiagram );
         SetSelectStateOfEntityForSet( vDiagram, szlHD_Node, 0, 1 );
         break;
      }
   }

   vDiagram = m_pHD_Diagram->PositionOnDiagramNode( this, vDiagram );
   SetSelectStateOfEntityForSet( vDiagram, szlHD_Node, 1, 1 );
   m_bSelected = 1;
   DrawNode( );
}

void
HyphenateHD_Text( zCPCHAR cpcText, zPCHAR pchReturn, zLONG lMaxLth, zLONG lIdx )
{
   zBOOL bGoodBreak = FALSE;
   zLONG k;

   strcpy_s( pchReturn, lMaxLth, cpcText );
// if ( zstrcmp( cpcText, "M_InsertTextKeywordTitleDU" ) == 0 )
//    TraceLineS( "HyphenateCheck ", cpcText );

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
ZHD_Node::DrawNode( )
{
   CBrush   *pBrush = 0;    // but because we are a little neurotic
   CFont    *pFont = 0;     // we will do it regardless
   CPen     *pPen = 0;      // not sure we have to zero these out ...
   CRect    rect;
   CSize    Size;
   zBOOL    bRelease;
   zCHAR    szText[ 256 ];
   COLORREF colorOldText;
   zBOOL    bSpecialSelect = FALSE;

   // Now paint a rounded rectangle to represent the entity
   if ( !m_pHD_Diagram->PositionVisible( m_lPosIdx, m_lPosIdx + (tzHDNODEX - 1) + ((tzHDNODEY - 1) * tzHDDIAGRAMX) ) )
   {
      return( 0 );
   }

   if ( m_pHD_Diagram->m_hDC == 0 )
   {
      m_pHD_Diagram->GetDeviceContext( );
      bRelease = TRUE;
   }
   else
      bRelease = FALSE;

   if ( m_pHD_Diagram->m_bPrintDC )
   {
      if ( m_bSelected && m_pHD_Diagram->m_bShadeEnts )
         pBrush = new CBrush( COLORREF( tzHDCOLORSHADE ) );
      else
         pBrush = new CBrush( tzHDCOLORWHITE );

      pPen = new CPen( PS_SOLID, 1, tzHDCOLORBLACK );
      colorOldText = m_pHD_Diagram->m_hDC->SetTextColor( tzHDCOLORBLACK );
   }
   else
   {
      pPen = new CPen( PS_SOLID, 1, tzHDCOLORBOX );
      colorOldText = m_pHD_Diagram->m_hDC->SetTextColor( tzHDCOLORTEXT );
      if ( m_lColor >= 0 && m_lColor < 16 )
      {
         if ( m_bSelected )
            pBrush = new CBrush( tzHDCOLORSELECTED );
         else
            pBrush = new CBrush( tzHDCOLORNORMAL[ m_lColor ] );
      }
      else
      {
         pBrush = new CBrush( m_lColor );
         if ( m_bSelected )
            bSpecialSelect = TRUE;
      }
   }

   CPen   *pPenOld = m_pHD_Diagram->m_hDC->SelectObject( pPen );
   CBrush *pBrushOld = m_pHD_Diagram->m_hDC->SelectObject( pBrush );
   CFont  *pFontOld = 0;

   pFont = new CFont( );
   if ( pFont->CreateFont( (tzHDFONTHEIGHT * 5) / 4, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET,
                           OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, tzHDFONTFF, tzHDFONTNAME ) )
   {
      pFontOld = m_pHD_Diagram->m_hDC->SelectObject( pFont );
   }

   rect.left = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
   rect.top  = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
   if ( m_bHidden )
   {
      rect.right  = rect.left + tzHDCELLSIZE;
      rect.bottom = rect.top  + tzHDCELLSIZE;
      m_pHD_Diagram->m_hDC->Ellipse( rect.left, rect.top, rect.right, rect.bottom );
      if ( bSpecialSelect )
         m_pHD_Diagram->m_hDC->Ellipse( rect.left + 2, rect.top + 2, rect.right + 2, rect.bottom - 2 );

   }
   else
   {
      rect.right  = rect.left + ((tzHDNODEX * tzHDCELLSIZE) - 1);
      rect.bottom = rect.top  + ((tzHDNODEY * tzHDCELLSIZE) - 1);
      m_pHD_Diagram->m_hDC->RoundRect( rect.left, rect.top, rect.right, rect.bottom, tzHDARCSIZE, tzHDARCSIZE );
      if ( bSpecialSelect )
         m_pHD_Diagram->m_hDC->RoundRect( rect.left + 2, rect.top + 2, rect.right - 2, rect.bottom - 2, tzHDARCSIZE, tzHDARCSIZE );
      // Now that the rectangle is drawn, see if we need to draw the attributive or associative lines in the entity
      if ( m_chType == 'A' )
      {
         if ( m_pHD_Diagram->m_bPrintDC == FALSE )
         {
            CPen *pPen2 = pPen;
            pPen = new CPen( PS_SOLID, 1, tzHDCOLORGRAY );
            m_pHD_Diagram->m_hDC->SelectObject( pPen );
            mDeleteInit( pPen2 );
         }

         CPoint pt1;
         CPoint pt2;

         pt1.x = ((rect.left + rect.right) / 2);
         pt1.y = rect.top;
         m_pHD_Diagram->m_hDC->MoveTo( pt1.x, pt1.y );
         pt2.x = rect.left;
         pt2.y = rect.bottom - tzHDCELLSIZE;
         m_pHD_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         m_pHD_Diagram->m_hDC->MoveTo( pt1.x, pt1.y );
         pt2.x = rect.right;
         m_pHD_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
      }
      else
      if ( m_chType == 'S' )
      {
         if ( m_pHD_Diagram->m_bPrintDC == FALSE )
         {
            CPen *pPen2 = pPen;
            pPen = new CPen( PS_SOLID, 1, tzHDCOLORGRAY );
            m_pHD_Diagram->m_hDC->SelectObject( pPen );
            mDeleteInit( pPen2 );
         }

         CPoint pt1;
         CPoint pt2;

         pt1.x = ((rect.left + rect.right) / 2);
         pt1.y = rect.top;
         m_pHD_Diagram->m_hDC->MoveTo( pt1.x, pt1.y );
         pt2.x = rect.left;
         pt2.y = (rect.top + rect.bottom) / 2;
         m_pHD_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         pt2.x = pt1.x;
         pt2.y = rect.bottom;
         m_pHD_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         pt2.x = rect.right;
         pt2.y = (rect.top + rect.bottom) / 2;
         m_pHD_Diagram->m_hDC->LineTo( pt2.x, pt2.y );
         m_pHD_Diagram->m_hDC->LineTo( pt1.x, pt1.y );
      }

      int nOldBkMode = m_pHD_Diagram->m_hDC->SetBkMode( TRANSPARENT );
      Size = m_pHD_Diagram->m_hDC->GetTextExtent( m_csText );
      rect.left   += 4;
      rect.right  -= 4;
      if ( Size.cx > (rect.right - rect.left) )
      {
         rect.top    += ((tzHDCELLSIZE * 3) / 4);
         rect.bottom -= ((tzHDCELLSIZE * 3) / 4);
//       rect.top    += tzHDCELLSIZE;
//       rect.bottom -= tzHDCELLSIZE;

         zSHORT nIdx = 0;
         zSHORT nLth = m_csText.GetLength( ) - 1;
         while ( nIdx < nLth && m_csText.GetAt( nIdx ) != ' ' )
            nIdx++;

         Size = m_pHD_Diagram->m_hDC->GetTextExtent( m_csText, nIdx );
         if ( Size.cx > (rect.right - rect.left) )
         {
            while ( nIdx && Size.cx > (rect.right - rect.left) )
            {
               nIdx--;
               Size = m_pHD_Diagram->m_hDC->GetTextExtent( m_csText, nIdx );
            }

            nIdx--;
            if ( nIdx > 0 )
            {
               HyphenateHD_Text( m_csText, szText, zsizeof( szText ), nIdx );
            }

            m_pHD_Diagram->m_hDC->DrawText( szText, -1, rect, DT_LEFT | DT_WORDBREAK );
   //                                                         DT_SINGLELINE | DT_LEFT | DT_VCENTER );
         }
         else
         {
   //       rect.top += ((tzHDCELLSIZE * 3) / 4);
            m_pHD_Diagram->m_hDC->DrawText( m_csText, -1, rect, DT_CENTER | DT_WORDBREAK );
         }
      }
      else
      {
         rect.left   -= 4;
         rect.right  += 4;
         m_pHD_Diagram->m_hDC->DrawText( m_csText, -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
      }

      if ( m_pHD_Diagram->m_bCollapsable ||
           m_csTitle.IsEmpty( ) == FALSE  ||
           m_csDIL.IsEmpty( ) == FALSE    ||
           m_csOutsideText1.IsEmpty( ) == FALSE ||
           m_csOutsideText2.IsEmpty( ) == FALSE ||
           m_csOutsideText3.IsEmpty( ) == FALSE ||
           m_csOutsideText4.IsEmpty( ) == FALSE ||
           m_csOutsideText5.IsEmpty( ) == FALSE )
      {
         if ( pFontOld )
         {
            m_pHD_Diagram->m_hDC->SelectObject( pFontOld );
            pFontOld = 0;
         }

         mDeleteInit( pFont );
         pFont = new CFont( );
         if ( pFont->CreateFont( tzHDCELLSIZE - 1, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET,
                                 OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, tzHDFONTFF, tzHDFONTNAME ) )
         {
            pFontOld = m_pHD_Diagram->m_hDC->SelectObject( pFont );
         }

         if ( m_csTitle.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.right  = rect.left + (tzHDNODEX * tzHDCELLSIZE) - 1;
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.bottom = rect.top  + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csTitle, -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
         }

         if ( m_csDIL.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.right  = rect.left + (tzHDNODEX * tzHDCELLSIZE) - 1;
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.top    += ((tzHDNODEY - 1) * tzHDCELLSIZE);
            rect.bottom = rect.top + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csDIL, -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
         }

         if ( m_csOutsideText1.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.left   = rect.left + (tzHDNODEX * tzHDCELLSIZE) + 1;
            rect.right  = rect.left + (tzHDNODEX * ((tzHDCELLSIZE + 1) / 2));
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.bottom = rect.top  + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csOutsideText1, -1, rect,  DT_SINGLELINE | DT_VCENTER );
         }

         if ( m_csOutsideText2.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.left   = rect.left + (tzHDNODEX * tzHDCELLSIZE) + 1;
            rect.right  = rect.left + (tzHDNODEX * ((tzHDCELLSIZE + 1) / 2));
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.top    += tzHDCELLSIZE;
            rect.bottom = rect.top  + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csOutsideText2, -1, rect, DT_SINGLELINE | DT_VCENTER );
         }

         if ( m_csOutsideText3.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.left   = rect.left + (tzHDNODEX * tzHDCELLSIZE) + 1;
            rect.right  = rect.left + (tzHDNODEX * ((tzHDCELLSIZE + 1) / 2));
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.top    += (2 * tzHDCELLSIZE);
            rect.bottom = rect.top  + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csOutsideText3, -1, rect,  DT_SINGLELINE | DT_VCENTER );
         }

         if ( m_csOutsideText4.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.left   = rect.left + (tzHDNODEX * tzHDCELLSIZE) + 1;
            rect.right  = rect.left + (tzHDNODEX * ((tzHDCELLSIZE + 1) / 2));
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.top    += (3 * tzHDCELLSIZE);
            rect.bottom = rect.top  + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csOutsideText4, -1, rect, DT_SINGLELINE | DT_VCENTER );
         }

         if ( m_csOutsideText5.IsEmpty( ) == FALSE )
         {
            rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.left   = rect.left + (tzHDNODEX * tzHDCELLSIZE) + 1;
            rect.right  = rect.left + (tzHDNODEX * ((tzHDCELLSIZE + 1) / 2));
            rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
            rect.top    += (4 * tzHDCELLSIZE);
            rect.bottom = rect.top  + tzHDCELLSIZE;
            m_pHD_Diagram->m_hDC->DrawText( m_csOutsideText5, -1, rect, DT_SINGLELINE | DT_VCENTER );
         }
      }

      if ( m_pHD_Diagram->m_bCollapsable )
      {
         rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
         rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
         rect.right  = rect.left + tzHDCELLSIZE;
         rect.bottom = rect.top  + tzHDCELLSIZE;
         if ( m_bCollapsed )
         {
            if ( m_pHD_Diagram->m_bPrintDC == FALSE )
            {
               mDeleteInit( pBrush );
               if ( m_lColor >= 0 && m_lColor < 16 )
               {
                  if ( m_bSelected )
                     pBrush = new CBrush( tzHDCOLORNORMAL[ m_lColor ] );
                  else
                     pBrush = new CBrush( tzHDCOLORSELECTED );
               }
               else
               {
                  pBrush = new CBrush( m_lColor );
                  if ( m_bSelected )
                     bSpecialSelect = TRUE;
               }

               m_pHD_Diagram->m_hDC->SelectObject( pBrush );
               m_pHD_Diagram->m_hDC->SetBkMode( OPAQUE );
               m_pHD_Diagram->m_hDC->Ellipse( rect.left, rect.top, rect.right, rect.bottom );
               m_pHD_Diagram->m_hDC->SetBkMode( TRANSPARENT );
            }

            m_pHD_Diagram->m_hDC->DrawText( "+", -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
         }
//       else
//       {
//          m_pHD_Diagram->m_hDC->DrawText( "-", -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
//       }
      }

      m_pHD_Diagram->m_hDC->SetBkMode( nOldBkMode );
   }

   m_pHD_Diagram->m_hDC->SetTextColor( colorOldText );
   m_pHD_Diagram->m_hDC->SelectObject( pPenOld );
   if ( pFontOld )
      m_pHD_Diagram->m_hDC->SelectObject( pFontOld );

   m_pHD_Diagram->m_hDC->SelectObject( pBrushOld );
   mDeleteInit( pPen );
   mDeleteInit( pFont );
   mDeleteInit( pBrush );

   // Now that the node has been drawn, draw the line to the parent node if the node exists
   DrawNodeLine( );

   if ( bRelease )
      m_pHD_Diagram->ReleaseDeviceContext( );

   // return that entity was drawn
   return( 1 );
}

zSHORT
ZHD_Node::DrawNodeLine( zBOOL bErase )
{
   CPen   *pPen;
   CRect  rect;
   zBOOL  bRelease;
   CPoint point1;

   // Now that the node has been drawn, draw the line to the parent node if the node exists
   if ( m_pParentNode )
   {
      if ( m_pHD_Diagram->m_hDC == 0 )
      {
         m_pHD_Diagram->GetDeviceContext( );
         bRelease = TRUE;
      }
      else
         bRelease = FALSE;

      if ( m_pHD_Diagram->m_bPrintDC )
         pPen = new CPen( PS_SOLID, 1, tzHDCOLORBLACK );
      else
      if ( bErase )
         pPen = new CPen( PS_SOLID, 1, tzHDCOLORBACKGROUND );
      else
         pPen = new CPen( PS_SOLID, 1, tzHDCOLORLINES );

      rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
      rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
      rect.right  = rect.left + (tzHDNODEX * tzHDCELLSIZE) - 1;
      rect.bottom = rect.top  + (tzHDNODEY * tzHDCELLSIZE) - 1;
      if ( m_pHD_Diagram->m_bOrientation == 0 )
      {
         if ( m_bHidden )
            point1.x = rect.left + (tzHDCELLSIZE / 2);
         else
            point1.x = (rect.left + rect.right) / 2;

         point1.y = rect.top;
      }
      else
      {
         point1.x = rect.left;
         if ( m_bHidden )
            point1.y = rect.top + (tzHDCELLSIZE / 2);
         else
            point1.y = (rect.top + rect.bottom) / 2;
      }

      m_pHD_Diagram->m_hDC->MoveTo( point1.x, point1.y );
      rect.left = (m_pParentNode->PosX( ) - m_pParentNode->PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
      rect.top  = (m_pParentNode->PosY( ) - m_pParentNode->PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
      rect.right  = rect.left + (tzHDNODEX * tzHDCELLSIZE) - 1;
      rect.bottom = rect.top  + (tzHDNODEY * tzHDCELLSIZE) - 1;
      if ( m_pHD_Diagram->m_bOrientation == 0 )
      {
         point1.x = ( rect.left + rect.right ) / 2;
         point1.y = rect.bottom;
      }
      else
      {
         point1.x = rect.right;
         point1.y = ( rect.bottom + rect.top ) / 2;
      }

      m_pHD_Diagram->m_hDC->LineTo( point1.x, point1.y );
      mDeleteInit( pPen );
      if ( bRelease )
         m_pHD_Diagram->ReleaseDeviceContext( );
   }

   // return that entity was drawn
   return( 1 );
}

void
ZHD_Node::EraseNode( )
{
   CRect    rect;
   CBrush   *pBrush;
   CPen     *pPen;
   zBOOL    bRelease;

   // Erase the rectangle containing the entity
   rect.left   = (PosX( ) - PosX( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
   rect.top    = (PosY( ) - PosY( tzHDDISPLAYPOS )) * tzHDCELLSIZE;
   rect.right  = rect.left + (tzHDNODEX * tzHDCELLSIZE) - 1;
   rect.bottom = rect.top  + (tzHDNODEY * tzHDCELLSIZE) - 1;

   if ( m_pHD_Diagram->m_hDC == 0 )
   {
      m_pHD_Diagram->GetDeviceContext( );
      bRelease = TRUE;
   }
   else
      bRelease = FALSE;

   // Unpaint the nodes line
   DrawNodeLine( 1 );

   // Now un-paint the rectangle where the entity was
   pBrush = new CBrush( tzHDCOLORBACKGROUND );
   pPen   = new CPen( PS_SOLID, 1, tzHDCOLORBACKGROUND );
   CPen *pPenOld = m_pHD_Diagram->m_hDC->SelectObject( pPen );
   CBrush *pBrushOld = m_pHD_Diagram->m_hDC->SelectObject( pBrush );

   m_pHD_Diagram->m_hDC->Rectangle( rect );

   m_pHD_Diagram->m_hDC->SelectObject( pPenOld );
   m_pHD_Diagram->m_hDC->SelectObject( pBrushOld );
   mDeleteInit( pBrush );
   mDeleteInit( pPen );

   if ( bRelease )
      m_pHD_Diagram->ReleaseDeviceContext( );
}

void
ZHD_Node::CenterNode( )
{
   CRect  rect;
   zLONG  lDisplaySizeX, lDisplaySizeY;
   zLONG  lNodeMiddleX, lNodeMiddleY;
   zLONG  lDisplayPosX, lDisplayPosY;

   m_pHD_Diagram->GetClientRect( rect );

   // Get the logical rect size with scaling
   m_pHD_Diagram->CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY, rect, m_pHD_Diagram->m_lScale );

   lDisplaySizeX /= 2;
   lDisplaySizeY /= 2;

   lNodeMiddleX = PosX( ) + (tzHDNODEX / 2);
   lNodeMiddleY = PosY( ) + (tzHDNODEY / 2);

   lDisplayPosX = lNodeMiddleX - lDisplaySizeX;
   lDisplayPosY = lNodeMiddleY - lDisplaySizeY;

   if ( lDisplayPosX < 0 )
      lDisplayPosX = 0;

   if ( lDisplayPosY < 0 )
      lDisplayPosY = 0;

   m_pHD_Diagram->m_lDisplayPos = lDisplayPosY;
   m_pHD_Diagram->m_lDisplayPos *= tzHDDIAGRAMX;
   m_pHD_Diagram->m_lDisplayPos += lDisplayPosX;

   // Here we zoom the diagram to the current scale to ensure the display position is altered if necessary
   // so the right and bottom edge aren't beyond the diagram space
   m_pHD_Diagram->ZoomDiagram( (zSHORT) m_pHD_Diagram->m_lScale + 100, 1 );
}

zSHORT
ZHD_Node::PosX( zLONG lIdx )
{
   if ( lIdx == -1 )
      return( (zSHORT) (m_lPosIdx % tzHDDIAGRAMX) );
   else
      return( (zSHORT) (lIdx % tzHDDIAGRAMX) );
}

zSHORT
ZHD_Node::PosY( zLONG lIdx )
{
   if ( lIdx == -1 )
      return( (zSHORT) (m_lPosIdx / tzHDDIAGRAMX) );
   else
      return( (zSHORT) (lIdx / tzHDDIAGRAMX) );
}
