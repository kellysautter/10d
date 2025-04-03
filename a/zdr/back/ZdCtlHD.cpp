/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlhd.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Hierarchical diagramming control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2000.03.17    TMV    WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 1999.07.06    DKS    Z10
//    Fixed font problem when selecting font that was not created and
//    deleting font that was selected.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.10.24    DKS
//    Fixed PrintDiagram for Windows 95 (TB 302 and TB 311)
//
// 1998.09.24    DKS
//    Fixed PrintDiagram to provide a return code to the application when
//    the print is canceled (QS 78).
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini and permit the ZoomDiagram
//    to return the current settings so that they may be stored for
//    future use.
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

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZHD_Diagram, CWnd )

BEGIN_MESSAGE_MAP( ZHD_Diagram, CWnd )
   ON_WM_CREATE( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_RBUTTONDBLCLK( )
   ON_WM_HSCROLL( )
   ON_WM_VSCROLL( )
   ON_WM_PAINT( )
   ON_WM_MOUSEMOVE( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
END_MESSAGE_MAP( )

//
//  CONSTRUCTOR: HD_Diagram
//
ZHD_Diagram::ZHD_Diagram( ZSubtask *pZSubtask,
                          CWnd     *pWndParent,
                          ZMapAct  *pzmaComposite,
                          zVIEW    vDialog,
                          zSHORT   nOffsetX,
                          zSHORT   nOffsetY,
                          zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
             CWnd( ),
             ZMapAct( pZSubtask,          // base class ctor
                      pzmaComposite,
                      pWndParent,
                      this,
                      vDialog,
                      nOffsetX,
                      nOffsetY,
                      pCtrlDef,
                      "HD" )
{
   CRect rect;
   zSHORT nIdx;

   m_pHD_Diagram       = this;     // Set a pointer to ourself for defines
   m_lCellSize         = 9;
   m_lScale            = 0;
   m_bScrolledWindow   = FALSE;
   m_bOrientation      = FALSE;
   m_bCollapsable      = TRUE;
   m_pLastPosNode      = 0;
   m_nNodeCnt = 0;
   m_NodeList.SetSize( 20, 20 );

   for ( nIdx = 0; nIdx < 16; nIdx++ )
      m_clrNormal[ nIdx ] = RGB( 255, 255, 0 );

   m_clrSelected = RGB( 255, 0, 0 );
   m_clrBox        = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrBoxU       = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrLines      = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrLinesU     = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrText       = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrTextU      = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrNoUpdate = ::GetSysColor( COLOR_GRAYTEXT );

   m_lDisplayPos    = 0;
   SetDisplayPosMax( Attr.W, Attr.H );

   m_lUpdateDisplayPos    = -1;
   m_lUpdateDisplayPosMax = -1;
   Attr.Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
                WS_HSCROLL | WS_VSCROLL;
   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
      Attr.Style |= WS_BORDER;

   m_hDC = 0;
   m_bPrintDC    = FALSE;
   m_bCapture    = FALSE;
   m_bMapped     = FALSE;
   m_bUpdateView = TRUE;
   m_lMinPosX    = 0;
   m_lMinPosY    = 0;
   m_lMaxPosX    = 0;
   m_lMaxPosY    = 0;
   m_nLDblClk    = 0;
   m_nRDblClk    = 0;

   if ( m_pzsVName == 0 )
      return;

   // Always create an HD ... should never have a parent control!
   CreateZ( );
   SetScrollBars( );

   // Finally, do output mapping
   rect.left   = Attr.X;
   rect.top    = Attr.Y;
   rect.right  = rect.left + Attr.W;
   rect.bottom = rect.top  + Attr.H;
   CalculateMinimumScale( rect );
   ZoomDiagram( 0, 3 );
}

void
ZHD_Diagram::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( WS_EX_CLIENTEDGE, "ZeidonCustomCtrl", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZHD_Diagram::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   zCHAR  szBuffer[ 40 ];
   zSHORT nLth = 256;  // some arbitrary length
   zPCHAR pchBuffer = new char[ nLth + 1 ];
   zPCHAR pch;
   zPCHAR pchComma;
   zSHORT nValues;

   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   GetWindowPreferenceString( m_pZSubtask->m_vDialog,
                              szBuffer, pchBuffer, nLth );

   nValues = 0;
   if ( *pchBuffer )
   {
      pch = pchBuffer;
      while ( pch )
      {
         pch = strchr( pch, ',' );
         if ( pch )
            pch++;

         nValues++;
      }
   }

   if ( nValues == 6 )
   {
      pch = pchBuffer;
      pchComma = strchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      m_bOrientation = (zBOOL) zatol( pch );
      pch = pchComma + 1;

      pchComma = strchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      m_bScrolledWindow = (zBOOL) zatol( pch );
      pch = pchComma + 1;

      pchComma = strchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      m_lDisplayPos = zatol( pch );
      pch = pchComma + 1;

      pchComma = strchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      m_lDisplayPosMax = zatol( pch );
      pch = pchComma + 1;

      pchComma = strchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      m_lCellSize = zatol( pch );
      pch = pchComma + 1;

      m_lScale = zatol( pch );
   }

   zVIEW vDiagram;

   if ( GetViewByName( &vDiagram, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      SetAttributeFromInteger( vDiagram, "HD_Diagram",
                               "Orientation", m_bOrientation );
   }

   mDeleteInitA( pchBuffer );

   SetFontOverride( );
   return( nRC );
}

BOOL
ZHD_Diagram::DestroyWindow( )
{
   ZHD_Node *pNode;

   zCHAR szBuffer[ 40 ]; // tag length + 7
   zLONG lMaxLth = zsizeof( "-2147483647," ) * 7;
   zPCHAR pch = new char[ lMaxLth ];
   zSHORT nLth = 0;

   _ltoa_s( m_bOrientation, pch + nLth, lMaxLth - nLth, 10 );
   nLth = (zSHORT) zstrlen( pch );
   pch[ nLth ] = ',';
   nLth++;
   _ltoa_s( m_bScrolledWindow, pch + nLth, lMaxLth - nLth, 10 );
   nLth = (zSHORT) zstrlen( pch );
   pch[ nLth ] = ',';
   nLth++;
   _ltoa_s( m_lDisplayPos, pch + nLth, lMaxLth - nLth, 10 );
   nLth = (zSHORT) zstrlen( pch );
   pch[ nLth ] = ',';
   nLth++;
   _ltoa_s( m_lDisplayPosMax, pch + nLth, lMaxLth - nLth, 10 );
   nLth = (zSHORT) zstrlen( pch );
   pch[ nLth ] = ',';
   nLth++;
   _ltoa_s( m_lCellSize, pch + nLth, lMaxLth - nLth, 10 );
   nLth = (zSHORT) zstrlen( pch );
   pch[ nLth ] = ',';
   nLth++;
   _ltoa_s( m_lScale, pch + nLth, lMaxLth - nLth, 10 );

   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );

   mDeleteInitA( pch );

   while ( m_nNodeCnt )
   {
      m_nNodeCnt--;
      pNode = m_NodeList[ m_nNodeCnt ];
      m_NodeList.RemoveAt( m_nNodeCnt );
      delete( pNode );
   }

   ReleaseDeviceContext( );

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

//
//  DESTRUCTOR: HD_Diagram
//
ZHD_Diagram::~ZHD_Diagram( )
{
// TraceLineS( "ZHD_Diagram::~ZHD_Diagram", "" );
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

void
ZHD_Diagram::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

void
ZHD_Diagram::DrawDiagram( )
{
   zSHORT   nPos;
   ZHD_Node *pNode;
   zBOOL    bDelete;

   if ( m_hDC == 0 )
   {
      GetDeviceContext( );
      bDelete = 1;
   }
   else
      bDelete = 0;

   for ( nPos = 0; nPos < m_nNodeCnt; nPos++ )
   {
      pNode = m_NodeList[ nPos ];
      if ( pNode->DrawNode( ) == 0 )
         pNode->DrawNodeLine( );
   }

   if ( bDelete )
      ReleaseDeviceContext( );
}

zSHORT
ZHD_Diagram::ZoomDiagram( zSHORT nPercent, zSHORT nOptions )
{
   CRect  rect;
   zLONG  lOldDisplaySizeX, lOldDisplaySizeY;
   zLONG  lDisplaySizeX, lDisplaySizeY;
   zLONG  lMoveX, lMoveY;
   zLONG  lNewScale;

   // Here we are going to either zoom in or zoom out, we make
   // every effort possible to keep the center of focus intact after the
   // zoom process by moving the upper left display position
   if ( nPercent == 0 && nOptions != 3 )
      return( (zSHORT) (m_lScale + 100) );

   GetClientRect( rect );

   // there are always scroll bars ... so account for them
   rect.right -= GetSystemMetrics( SM_CXVSCROLL );
   rect.bottom -= GetSystemMetrics( SM_CYHSCROLL );

   // Get the logical rect size with scaling
   CalculateDisplaySize( lOldDisplaySizeX, lOldDisplaySizeY, rect, m_lScale );

   // Calculate new scale
   if ( nOptions == 1 || nOptions == 4 )   // Absolute Zooming
      lNewScale = nPercent - 100;
   else
   if ( nOptions == 2 )   // Relative Zooming
   {
      lNewScale = ((m_lScale + 100) * nPercent) / 100;
      if ( lNewScale == 0 )
      {
         if ( nPercent > 0 )
            lNewScale = 1;
         else
            lNewScale = -1;
      }

      lNewScale += m_lScale;
   }
   else
   if ( nOptions == 3 )   // Fit to window
   {
      // Get maximum position of diagram
      SetMaxPositions( );
      lNewScale = 0;
      if ( m_nNodeCnt )
      {
         CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY,
                               rect, lNewScale );
         while ( lDisplaySizeX > (m_lMaxPosX + tzHDNODEX) &&
                 lDisplaySizeY > (m_lMaxPosY + tzHDNODEY) )
         {
            lNewScale += 10;
            CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY,
                                  rect, lNewScale );
         }

         while ( lDisplaySizeX < (m_lMaxPosX + tzHDNODEX) ||
                 lDisplaySizeY < (m_lMaxPosY + tzHDNODEY) )
         {
            if ( lNewScale == -99 )
               break;

            if ( lNewScale > -95 )
               lNewScale -= 5;
            else
               lNewScale = -99;

            CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY,
                                  rect, lNewScale );
         }

         if ( lDisplaySizeX > (m_lMaxPosX + tzHDNODEX) &&
              lDisplaySizeY > (m_lMaxPosY + tzHDNODEY) )
         {
            while ( lDisplaySizeX > (m_lMaxPosX + tzHDNODEX) &&
                    lDisplaySizeY > (m_lMaxPosY + tzHDNODEY) )
            {
               lNewScale++;
               CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY,
                                     rect, lNewScale );
            }

            lNewScale--;
         }
      }

      m_lDisplayPos = 0;
   }

   if ( lNewScale < m_lMinScale )
      lNewScale = m_lMinScale;

   if ( lNewScale > 999 )
      lNewScale = 999;

   m_lScale = lNewScale;
   CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY, rect, lNewScale );

   // Now that we have the new display size and the old display size,
   // attempt to move the display position and keep the center of
   // the display where it currently is.  If the display position is 0,
   // skip the centering process.  We do not do this for Option 4 which
   // is rubberband zooming
   if ( m_lDisplayPos && nOptions != 4 )
   {
      lMoveX = (lDisplaySizeX - lOldDisplaySizeX) / 2;
      lMoveY = (lDisplaySizeY - lOldDisplaySizeY) / 2;
      if ( lDisplaySizeX > lOldDisplaySizeX )
      {
         while ( PosX( m_lDisplayPos ) && lMoveX )
         {
            m_lDisplayPos--;
            lMoveX--;
         }

         while ( PosY( m_lDisplayPos ) && lMoveY )
         {
            m_lDisplayPos -= tzHDDIAGRAMX;
            lMoveY--;
         }
      }
      else
      {
         while ( (PosX( m_lDisplayPos ) < (tzHDDIAGRAMX - 1)) && lMoveX )
         {
            m_lDisplayPos++;
            lMoveX++;
         }

         while ( (PosY( m_lDisplayPos ) < (tzHDDIAGRAMY - 1)) && lMoveY )
         {
            m_lDisplayPos++;
            lMoveY++;
         }
      }

      while ( PosX( m_lDisplayPos ) &&
              (PosX( m_lDisplayPos ) + lDisplaySizeX) >= tzHDDIAGRAMX )
      {
         m_lDisplayPos--;
      }

      while ( PosY( m_lDisplayPos ) &&
              (PosY( m_lDisplayPos ) + lDisplaySizeY) >= tzHDDIAGRAMY )
      {
         m_lDisplayPos -= tzHDDIAGRAMX;
      }
   }

   SetDisplayBoundaries( rect );
   SetScrollBars( );
   Invalidate( TRUE );
   m_bScrolledWindow = FALSE;
   return( 0 );
}

//
// METHOD : PrintDiagram
//
//
#define  zPRINT_PAGE        0x0001
#define  zPRINT_PERCENT     0x0002
#define  zPRINT_FILLPAGE    0x0004
#define  zPRINT_PORTRAIT    0x0800
#define  zPRINT_LANDSCAPE   0x0400
#define  zPRINT_SHADESELE   0x2000
#define  zPRINT_SHADESELR   0x1000

zSHORT
ZHD_Diagram::PrintDiagram( zSHORT nPercent, zSHORT nOptions )
{
   CWaitCursor wait;

   zVIEW    vDiagram;
   zPCHAR   pchPrintBox;
   zPCHAR   pchPrintTitle;
   zPCHAR   pchPrintDesc;
   zLONG    lShiftX, lShiftY;
   zLONG    lDisplayPosHold;
   zLONG    lDisplayPosMaxHold;
   zLONG    lCellSizeHold;
   zLONG    lScaleHold;
   zSHORT   nRC = zRESPONSE_YES;

   if ( GetViewByName( &vDiagram, "TZZOLODO",
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      GetAddrForAttribute( &pchPrintDesc, vDiagram, "LOD", "Desc" );
   }
   else
      pchPrintDesc = 0;

   if ( GetViewByName( &vDiagram, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      CreateViewFromViewForTask( &vDiagram, vDiagram,
                                 m_pZSubtask->m_vDialog );
      ResetView( vDiagram );
      GetAddrForAttribute( &pchPrintBox, vDiagram,
                           szlHD_Diagram, szlPrintBox );
      GetAddrForAttribute( &pchPrintTitle, vDiagram,
                           szlHD_Diagram, szlPrintTitle );
      DropView( vDiagram );
   }
   else
   {
      pchPrintBox   = 0;
      pchPrintTitle = 0;
   }

   // Set operation count timer.
   // OperationCount( 1 );

   // Save all the current window values.
   lDisplayPosHold    = m_lDisplayPos;
   lDisplayPosMaxHold = m_lDisplayPosMax;
   lCellSizeHold      = m_lCellSize;
   lScaleHold         = m_lScale;

   m_bShadeEnts = (nOptions & zPRINT_SHADESELE);
   m_bShadeRels = (nOptions & zPRINT_SHADESELR);

   // Set up the default Scaling and cellsize information.
   m_lScale = 0;

   // Now we determine the print cell size.
   m_lCellSize = tzHDPRINTCELLSIZE;

   // Get maximum position of diagram
   SetMaxPositions( );

   // Shift the maximum calculations to allow for a 4 cell border around the
   // whole diagram.
   m_lMaxPosX += (tzHDNODEX + 1) / 2;
   lShiftX = 4 - m_lMinPosX;
   TraceLineI("(zdctlhd) Shifting X position for print-> ", lShiftX );
   m_lMaxPosX += lShiftX;
   lShiftY = 4 - m_lMinPosY;
   TraceLineI("(zdctlhd) Shifting Y position for print-> ", lShiftX );
   m_lMaxPosY += lShiftY;
   m_lMaxPosX +=4;
   m_lMaxPosY +=4;

   // Get a DC to the default printer.
   CDC *pPrintDC = new CDC( );
   CPrintDialog dlg( FALSE );
   dlg.GetDefaults( );
   if ( pPrintDC->Attach( dlg.GetPrinterDC( ) ) )
   {
      ZHD_Node *pZHD_Node;
      zCHAR    szMessage[ 256 ];
      zPCHAR   pData = 0;
      CPoint   ptPageSize;
      CPoint   ptPageOffset;
      CPoint   ptPageSizeWork;
      CPoint   ptDiagramSize;
      int      nOldMapMode;
      CSize    sizeOldWExt;
      CSize    sizeOldVExt;
      zLONG    lXPercent;
      zLONG    lYPercent;
      zLONG    lPage;
      zLONG    lPages = 1;
      zLONG    lHorizPages, lHorizWork, lPrinted;
      zLONG    lPageSizeX, lPageSizeY;
      zLONG    lNodesPrinted;
      zLONG    lNodesNotPrinted;
      zLONG    lPos;

      // Get the physical page size of the printer.
      ptPageSize.x  = pPrintDC->GetDeviceCaps( HORZRES );  //??? which one ???
      ptPageSize.y  = pPrintDC->GetDeviceCaps( VERTRES );
      ptPageSize.x = pPrintDC->GetDeviceCaps( PHYSICALWIDTH );
      ptPageSize.y = pPrintDC->GetDeviceCaps( PHYSICALHEIGHT );
      ptPageOffset.x = pPrintDC->GetDeviceCaps( PHYSICALOFFSETX );
      ptPageOffset.y = pPrintDC->GetDeviceCaps( PHYSICALOFFSETY );
      ptPageSizeWork.x = ptPageSize.x - (2 * ptPageOffset.x);
      ptPageSizeWork.y = ptPageSize.y - (2 * ptPageOffset.y);

      // Now that we have all the printer information, do page analysis
      // if an orientation was not selected AND we're not forcing the
      // diagram to fit on one page.
      if ( (nOptions & (zPRINT_PAGE | zPRINT_FILLPAGE |
                        zPRINT_PORTRAIT | zPRINT_LANDSCAPE)) == 0 )
      {
         // If we're picking the orientation, select the orientation
         // which produces the fewest pages.
         // Determine the scaling necessary to get the diagram on the page
         ptDiagramSize.x = (m_lMaxPosX + tzHDNODEX) * tzHDCELLSIZE;
         ptDiagramSize.y = (m_lMaxPosY + tzHDNODEY) * tzHDCELLSIZE;

         // First Analyze the pages for portrait printing.
         lPages = AnalyzePages( lHorizPages, m_lScale,
                                lPageSizeX, lPageSizeY,
                                nPercent, nOptions | zPRINT_PORTRAIT,
                                ptDiagramSize, ptPageSizeWork );

         // Next flipflop the orientation and analyze the landscape printing.
         lPos = ptPageSizeWork.x;
         ptPageSizeWork.x = ptPageSizeWork.y;
         ptPageSizeWork.y = lPos;
         lPage  = AnalyzePages( lHorizPages, m_lScale,
                                lPageSizeX, lPageSizeY,
                                nPercent, nOptions | zPRINT_LANDSCAPE,
                                ptDiagramSize, ptPageSizeWork );

         // Reset the page size information back to portrait.
         lPos = ptPageSizeWork.x;
         ptPageSizeWork.x = ptPageSizeWork.y;
         ptPageSizeWork.y = lPos;

         // Now, if one is better than the other, update nOptions with
         // the appropriate orientation before setting the printer info.
         if ( lPages < lPage )
            nOptions |= zPRINT_PORTRAIT;
         else
         if ( lPage < lPages )
            nOptions |= zPRINT_LANDSCAPE;
      }

      DEVMODE DevMode;

      DevMode.dmSize = sizeof( DEVMODE );
      DevMode.dmDriverExtra = 0;

      if ( (nOptions & zPRINT_LANDSCAPE) ||
           ((nOptions & zPRINT_PORTRAIT) == 0 &&
           (m_lMaxPosX + tzHDNODEX) > (m_lMaxPosY + tzHDNODEY)) )
      {
         DevMode.dmOrientation = DMORIENT_LANDSCAPE;
         lPos = ptPageSizeWork.x;
         ptPageSizeWork.x = ptPageSizeWork.y;
         ptPageSizeWork.y = lPos;
         nOptions |= zPRINT_LANDSCAPE;
      }
      else
      {
         DevMode.dmOrientation = DMORIENT_PORTRAIT;
         nOptions |= zPRINT_PORTRAIT;
      }

      DevMode.dmDuplex = DMDUP_SIMPLEX;
      DevMode.dmScale  = 100;
      DevMode.dmColor  = DMCOLOR_COLOR;
      DevMode.dmFields = DM_ORIENTATION | DM_DUPLEX | DM_SCALE | DM_COLOR;

      CString cs = dlg.GetDeviceName( );

      mDeleteInit( pPrintDC );
      pPrintDC = new CDC( );
      pPrintDC->CreateDC( "", cs, "", &DevMode );
      if ( pPrintDC )
      {
         // A device context for the printer has been created ... we can now
         // write the diagram to the printer.
         CDC  *hSaveDC = m_hDC;
         m_hDC = pPrintDC;    // for use in DrawNode

         // Determine the scaling necessary to get the diagram on the page.
         ptDiagramSize.x = (m_lMaxPosX + (tzHDNODEX - 1)) * tzHDCELLSIZE;
         ptDiagramSize.y = (m_lMaxPosY + (tzHDNODEY - 1)) * tzHDCELLSIZE;

         // If we're going to print the diagram on one page, determine the
         // scaling factor required to shrink the diagram onto the page.
         if ( nOptions == 0 || (nOptions & (zPRINT_PAGE | zPRINT_FILLPAGE)) )
         {
            lPageSizeX = tzHDDIAGRAMX;
            lPageSizeY = tzHDDIAGRAMY;
            if ( ptDiagramSize.x > ptPageSizeWork.x ||
                 ptDiagramSize.y > ptPageSizeWork.y )
            {
               lXPercent = 100;
               lYPercent = 100;
               if ( ptDiagramSize.x > ptPageSizeWork.x )
               {
                  lXPercent = ptDiagramSize.x;
                  lXPercent = (lXPercent * 100) / ptPageSizeWork.x;
                  lXPercent = 10000 / lXPercent;
               }

               if ( ptDiagramSize.y > ptPageSizeWork.y )
               {
                  lYPercent = ptDiagramSize.y;
                  lYPercent = (lYPercent * 100) / ptPageSizeWork.y;
                  lYPercent = 10000 / lYPercent;
               }

               if ( lXPercent < lYPercent )
                  m_lScale = -101 + lXPercent;
               else
                  m_lScale = -101 + lYPercent;

               if ( m_lScale < -99 )
                  m_lScale = -99;
            }
            else
            if ( nOptions & zPRINT_FILLPAGE )
            {
               ptDiagramSize.x = (m_lMaxPosX + m_lMinPosX + tzHDNODEX) *
                                                                tzHDCELLSIZE;
               ptDiagramSize.y = (m_lMaxPosY + m_lMinPosY + tzHDNODEY) *
                                                                tzHDCELLSIZE;
               nOptions |= zPRINT_PERCENT;
               nPercent = 110;
               while ( (lPages =
                        AnalyzePages( lHorizPages, m_lScale,
                                      lPageSizeX, lPageSizeY,
                                      nPercent, nOptions,
                                      ptDiagramSize, ptPageSizeWork )) == 1 )
               {
                  nPercent += 10;
               }

               while ( (lPages =
                        AnalyzePages( lHorizPages, m_lScale,
                                      lPageSizeX, lPageSizeY,
                                      nPercent, nOptions,
                                      ptDiagramSize, ptPageSizeWork ) ) > 1 )
               {
                  nPercent -= 5;
               }

               while ( (lPages =
                        AnalyzePages( lHorizPages, m_lScale,
                                      lPageSizeX, lPageSizeY,
                                      nPercent, nOptions,
                                      ptDiagramSize, ptPageSizeWork ) ) == 1 )
               {
                  nPercent++;
               }

               nPercent--;
               m_lScale--;
               lPages = 1;
            }
         }
         else
         {
            lPages = AnalyzePages( lHorizPages, m_lScale,
                                   lPageSizeX, lPageSizeY,
                                   nPercent, nOptions,
                                   ptDiagramSize, ptPageSizeWork );
         }

         strcpy_s( szMessage, zsizeof( szMessage ), "Print Analysis\r\n\r\n" );
         strcat_s( szMessage, zsizeof( szMessage ), " Size: " );
         _ltoa_s( m_lScale + 100, szMessage + 240, zsizeof( szMessage ) - 240, 10 );
         strcat_s( szMessage, zsizeof( szMessage ), &szMessage[ 240 ] );
         strcat_s( szMessage, zsizeof( szMessage ), "%\r\n" );
         strcat_s( szMessage, zsizeof( szMessage ), " Orientation: " );
         if ( nOptions & zPRINT_PORTRAIT )
            strcat_s( szMessage, zsizeof( szMessage ), "Portrait\r\n" );
         else
            strcat_s( szMessage, zsizeof( szMessage ), "Landscape\r\n" );

         strcat_s( szMessage, zsizeof( szMessage ), " Pages: " );
         _ltoa_s( lPages, szMessage + 240, zsizeof( szMessage ) - 240, 10 );
         strcat_s( szMessage, zsizeof( szMessage ), &szMessage[ 240 ] );
         strcat_s( szMessage, zsizeof( szMessage ), "\r\n\r\n" );
         strcat_s( szMessage, zsizeof( szMessage ), "Continue with printing?" );

         // Reset the drawing stuff so a window paint can occur
         // while we are prompting the operator.
         zLONG lScalePrint = m_lScale;
         m_lScale = lScaleHold;
         m_lCellSize = lCellSizeHold;
         nRC = OperatorPrompt( m_pZSubtask->m_vDialog, "Diagram Print",
                               szMessage, 1, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
         m_lScale = lScalePrint;
         m_lCellSize = tzHDPRINTCELLSIZE;

         // If we succeed in starting the print, tie up the machine for
         // awhile printing the diagram.
         DOCINFO DocInfo;

         zmemset( &DocInfo, 0, sizeof( DOCINFO ) );
         DocInfo.cbSize = sizeof( DOCINFO );
         DocInfo.lpszDocName = "LOD Print";
         DocInfo.lpszOutput = 0;

         if ( nRC == zRESPONSE_YES && pPrintDC->StartDoc( &DocInfo ) > 0 )
         {
            // If the diagram needs to be shifted, do it now.
            if ( lShiftX || lShiftY )
            {
               for ( lPos = 0; lPos < m_nNodeCnt; lPos++ )
               {
                  pZHD_Node = m_NodeList[ lPos ];
                  pZHD_Node->m_lPosIdx += lShiftX;
                  pZHD_Node->m_lPosIdx += (tzHDDIAGRAMX * lShiftY);
               }
            }

            m_bPrintDC = TRUE;
            m_lDisplayPos = 0;
            m_lDisplayPosMax = lPageSizeY * tzHDDIAGRAMX;
            m_lDisplayPosMax += lPageSizeX;
            if ( m_lDisplayPosMax > tzHDMAXGRID )
               m_lDisplayPosMax = tzHDMAXGRID;

            lHorizWork = lHorizPages;
            lPrinted = 0;
            lPage = 1;
            for ( ;; )
            {
               TraceLineI( "(zdctlhd) Printing page #", lPage );
               pPrintDC->StartPage( );

               // If the diagram is to be scaled, scale the device context
               // and let windows handle the scaling through GDI.
               if ( m_lScale )
               {
                  nOldMapMode = pPrintDC->SetMapMode( MM_ISOTROPIC );
                  sizeOldWExt = pPrintDC->SetWindowExt( CSize( 100, 100 ) );
                  sizeOldVExt = pPrintDC->
                    SetViewportExt( CSize( 100 + m_lScale, 100 + m_lScale ) );
               }

               lNodesPrinted = 0;
               lNodesNotPrinted = 0;
               for ( lPos = 0; lPos < m_nNodeCnt; lPos++ )
               {
                  pZHD_Node = m_NodeList[ lPos ];
                  if ( pZHD_Node->DrawNode( ) )
                  {
                     lNodesPrinted++;
                     lPrinted++;
                  }
                  else
                  {
                     pZHD_Node->DrawNodeLine( );
                     lNodesNotPrinted++;
                  }
               }

               TraceLineI( "                 Nodes printed = ", lNodesPrinted );
               TraceLineI( "            Nodes not printed = ", lNodesNotPrinted );

               // Do not print out a page if nothing was printed.
               if ( lPrinted )
               {
                  if ( lPages > 1 && (lPage > 1 || pchPrintTitle == 0 || *pchPrintTitle == 0) )
                  {
                     zCHAR szPage[ 4 ];

                     _ltoa_s( lPage, szPage, zsizeof( szPage ), 10 );
                     pPrintDC->TextOut( 0, 0, szPage, zstrlen( szPage ) );
                  }

                  if ( lPage == 1 )
                  {
                     zSHORT k;

                     if ( pchPrintTitle && *pchPrintTitle )
                     {
                        strcpy_s( szMessage, zsizeof( szMessage ), pchPrintTitle );
                        lPos = zstrlen( szMessage );
                        szMessage[ lPos++ ] = ' ';
                        szMessage[ lPos++ ] = '[';
                     }
                     else
                     {
                     // szMessage[ 0 ] = 0;
                        lPos = 0;
                     }

                     SysGetDateTime( szMessage + 200, zsizeof( szMessage ) - 200 );
                     UfFormatDateTime( szMessage + lPos, zsizeof( szMessage ) - lPos, szMessage + 200, "YYYY/MM/DD HH:MI:SS" );
                     lPos = zstrlen( szMessage ) - 3; // drop the seconds
                     szMessage[ lPos++ ] = ']';
                     szMessage[ lPos++ ] = ' ';

                     k = 0;
                     while ( lPos < 80 && pchPrintDesc &&
                             pchPrintDesc[ k ] &&
                             pchPrintDesc[ k ] != '\n' &&
                             pchPrintDesc[ k ] != '\r' )
                     {
                        szMessage[ lPos++ ] = pchPrintDesc[ k++ ];
                     }


                     szMessage[ lPos++ ] = '~';  // check boundary test
                     szMessage[ lPos ] = 0;

                     CFont *pFont = new CFont( );
                     if ( pFont->CreateFont( (tzHDFONTHEIGHT * 5) / 4, 0,
                                             0, 0, 0, 0, 0, 0, ANSI_CHARSET,
                                             OUT_TT_ONLY_PRECIS,
                                             CLIP_DEFAULT_PRECIS,
                                             PROOF_QUALITY, tzHDFONTFF,
                                             tzHDFONTNAME ) )
                     {
                        CFont *pFontOld = pPrintDC->SelectObject( pFont );
                        pPrintDC->TextOut( 2, 2, szMessage, lPos - 1 );
                        pPrintDC->SelectObject( pFontOld );
                     }
                     else
                        pPrintDC->TextOut( 2, 2, szMessage, lPos - 1 );

                     mDeleteInit( pFont );
                  }

                  if ( pchPrintBox && pchPrintBox[ 0 ] )
                  {
                     int lOldBkMode = pPrintDC->SetBkMode( TRANSPARENT );
                     CPen *pPenOld = (CPen *) pPrintDC->SelectStockObject( BLACK_PEN );
                     CBrush *pBrushOld = (CBrush *) pPrintDC->SelectStockObject( NULL_BRUSH );

                     pPrintDC->Rectangle( 1, 1,
                                          ptDiagramSize.x - 1,
                                          ptDiagramSize.y - 1 );
                     pPrintDC->SetBkMode( lOldBkMode );
                     pPrintDC->SelectObject( pPenOld );
                     pPrintDC->SelectObject( pBrushOld ); // is this a leak???
                  }
               }

               if ( m_lScale )
               {
                  pPrintDC->SetMapMode( nOldMapMode );
                  pPrintDC->SetViewportExt( CSize( sizeOldWExt ) );
                  pPrintDC->SetViewportExt( CSize( sizeOldVExt ) );
               }

               pPrintDC->EndPage( );

               // We've printed a page, move on to the next.
               lPage++;
               if ( lPage > lPages )
                  break;

               // We have another page to print, see whether we need to move
               // the display position to the right or down.
               lHorizWork--;
               if ( lHorizWork )
               {
                  m_lDisplayPos += (lPageSizeX - 2);
               }
               else
               {
                  m_lDisplayPos -= PosX( m_lDisplayPos );
                  m_lDisplayPos += (zLONG) (lPageSizeY - 2) * tzHDDIAGRAMX;
                  lHorizWork = lHorizPages;
               }

               m_lDisplayPosMax += (zLONG) lPageSizeY * tzHDDIAGRAMX;
               m_lDisplayPosMax += lPageSizeX;
               if ( m_lDisplayPosMax > tzHDMAXGRID )
                  m_lDisplayPosMax = tzHDMAXGRID;
            }

            pPrintDC->EndDoc( );

            // If the diagram needs to be shifted, do it now.
            if ( lShiftX || lShiftY )
            {
               for ( lPos = 0; lPos < m_nNodeCnt; lPos++ )
               {
                  pZHD_Node = m_NodeList[ lPos ];
                  pZHD_Node->m_lPosIdx -= lShiftX;
                  pZHD_Node->m_lPosIdx -= (tzHDDIAGRAMX * lShiftY);
               }
            }
         }

         m_hDC = hSaveDC;
         mDeleteInit( pPrintDC );
         m_bPrintDC = FALSE;
         Invalidate( TRUE );
      }
   }
   else
   {
      TraceLineS( "(zdctlhd) Error getting Default printer", "" );
   }

   // Restore all the screen information
   m_lDisplayPos = lDisplayPosHold;
   m_lDisplayPosMax = lDisplayPosMaxHold;
   m_lCellSize = lCellSizeHold;
   m_lScale    = lScaleHold;

   // Display timings
   // OperationCount( 0 );
   // OperationCount( 3 );

   if ( nRC == zRESPONSE_YES )
      return( 0 );
   else
      return( 1 );
}

zLONG
ZHD_Diagram::AnalyzePages( zLONG& lHorizPages, zLONG& lScale,
                           zLONG& lPageSizeX, zLONG& lPageSizeY,
                           zSHORT nPercent, zSHORT nOptions,
                           CPoint ptDiagramSize, CPoint ptPageSize )
{
   zLONG  lPages;
   zLONG  lPagesX;
   zLONG  lPagesY;
   zLONG  lSizeWk;
   double dWork;

   lPages      = 1;
   lHorizPages = 1;
   if ( nOptions & zPRINT_PERCENT )
   {
      lScale = nPercent - 100;
      if ( lScale < - 99 )
         lScale = -99;
   }
   else
      lScale = 0;

   // Now inflate or deflate the diagram size based on the scale provided.
   if ( lScale )
   {
      lSizeWk = ptDiagramSize.x;
      lSizeWk *= (lScale + 100);
      lSizeWk /= 100;
      ptDiagramSize.x = lSizeWk;
      lSizeWk = ptDiagramSize.y;
      lSizeWk *= (lScale + 100);
      lSizeWk /= 100;
      ptDiagramSize.y = lSizeWk;
   }

   if ( ptDiagramSize.x <= ptPageSize.x &&
        ptDiagramSize.y <= ptPageSize.y )
   {
      lPageSizeX = tzHDDIAGRAMX;
      lPageSizeY = tzHDDIAGRAMY;
      return( lPages );
   }

   // We've determined that the diagram will not fit on one page,
   // calculate the number of pages and the page sizes in cells.
   dWork = (double) ptPageSize.x;
   dWork /= ptDiagramSize.x;
   dWork *= (m_lMaxPosX + (tzHDNODEX - 1));
   dWork++;
   lPageSizeX = (zLONG) dWork;

   dWork = (double) ptPageSize.y;
   dWork /= ptDiagramSize.y;
   dWork *= (m_lMaxPosY + (tzHDNODEY - 1));
   dWork++;
   lPageSizeY = (zLONG) dWork;

   // Now analyze the number of pages in either direction
   lPagesX = 1;
   lSizeWk = lPageSizeX - 1;
   while ( lSizeWk < (m_lMaxPosX + (tzHDNODEX - 1)) )
   {
      lPagesX++;
      lSizeWk += (lPageSizeX - 2);
   }

   lPagesY = 1;
   lSizeWk = lPageSizeY - 1;
   while ( lSizeWk < (m_lMaxPosY + (tzHDNODEY - 1)) )
   {
      lPagesY++;
      lSizeWk += (lPageSizeY - 2);
   }

   lHorizPages = lPagesX;
   lPages = lPagesX * lPagesY;

   return( lPages );
}

void
ZHD_Diagram::RecursDrawNodes( zVIEW vDiagram, ZHD_Node *pParentNode )
{
   zLONG    lZKey, lPosX, lPosY, lColor;
   zSHORT   nCollapsed, nHidden, nSelected;
   zPCHAR   pchText;
   zPCHAR   pchTitle;
   zPCHAR   pchDIL;
   zPCHAR   pchOutsideText1;
   zPCHAR   pchOutsideText2;
   zPCHAR   pchOutsideText3;
   zPCHAR   pchOutsideText4;
   zPCHAR   pchOutsideText5;
   zPCHAR   pchType;
   ZHD_Node *pZHD_Node;

   if ( SetCursorFirstEntity( vDiagram, szlHD_Node, 0 ) < zCURSOR_SET )
   {
      pParentNode->m_bCollapsed = FALSE;
      return;
   }

   do
   {
      GetIntegerFromAttribute( &lZKey, vDiagram, szlHD_Node, szlZKey );
      GetIntegerFromAttribute( &lPosX, vDiagram, szlHD_Node, szlPosX );
      GetIntegerFromAttribute( &lPosY, vDiagram, szlHD_Node, szlPosY );
      GetIntegerFromAttribute( &lColor, vDiagram, szlHD_Node, szlColor );
      GetAddrForAttribute( &pchText, vDiagram, szlHD_Node, szlText );
      GetAddrForAttribute( &pchTitle, vDiagram, szlHD_Node, szlTitle );
      GetAddrForAttribute( &pchDIL, vDiagram, szlHD_Node, szlDIL );
      GetAddrForAttribute( &pchOutsideText1, vDiagram, szlHD_Node, szlOutsideText1 );
      GetAddrForAttribute( &pchOutsideText2, vDiagram, szlHD_Node, szlOutsideText2 );
      GetAddrForAttribute( &pchOutsideText3, vDiagram, szlHD_Node, szlOutsideText3 );
      GetAddrForAttribute( &pchOutsideText4, vDiagram, szlHD_Node, szlOutsideText4 );
      GetAddrForAttribute( &pchOutsideText5, vDiagram, szlHD_Node, szlOutsideText5 );
      GetAddrForAttribute( &pchType, vDiagram, szlHD_Node, szlPurpose );
      nSelected = GetSelectStateOfEntityForSet( vDiagram, szlHD_Node, 1 );
      if ( nSelected && m_pLastPosNode )
      {
         SetSelectStateOfEntityForSet( vDiagram, szlHD_Node, 0, 1 );
         nSelected = 0;
      }

      pZHD_Node = new ZHD_Node( this, pParentNode,
                                lPosX, lPosY,
                                pchText, pchTitle, pchDIL,
                                pchOutsideText1,
                                pchOutsideText2,
                                pchOutsideText3,
                                pchOutsideText4,
                                pchOutsideText5,
                                pchType,
                                lZKey,
                                (zBOOL) nSelected,
                                lColor );
      if ( nSelected )
         m_pLastPosNode = pZHD_Node;

      pZHD_Node->m_bCollapsed = FALSE;
      pZHD_Node->m_bHidden = FALSE;
      m_NodeList.SetAtGrow( m_nNodeCnt++, pZHD_Node );
      nCollapsed = CompareAttributeToString( vDiagram, szlHD_Node, szlCollapsed, "Y" );
      nHidden = CompareAttributeToString( vDiagram, szlHD_Node, szlHidden, "Y" );
      if ( nCollapsed && nHidden )
      {
         SetViewToSubobject( vDiagram, szlHD_ChildNode );
         RecursDrawNodes( vDiagram, pZHD_Node );
         ResetViewFromSubobject( vDiagram );
      }
      else
      {
         if ( nHidden == 0 )
            pZHD_Node->m_bHidden = TRUE;

         if ( nCollapsed == 0 &&
              CheckExistenceOfEntity( vDiagram, szlHD_ChildNode ) > zCURSOR_NULL )
         {
            pZHD_Node->m_bCollapsed = TRUE;
         }
      }

   } while ( SetCursorNextEntity( vDiagram, szlHD_Node, 0 ) == zCURSOR_SET );
}

// Standard driver control methods.
zSHORT
ZHD_Diagram::MapFromOI( WPARAM wFlag )
{
// TraceLineS( "ZHD_Diagram::MapFromOI ", *m_pzsTag );

   zVIEW    vDiagram;
// zLONG    lOrientation;
   ZHD_Node *pZHD_Node;

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   m_pLastPosNode = 0;
   if ( m_nNodeCnt )
   {
      Invalidate( TRUE );
      while ( m_nNodeCnt )
      {
         m_nNodeCnt--;
         pZHD_Node = m_NodeList[ m_nNodeCnt ];
         m_NodeList.RemoveAt( m_nNodeCnt );
         delete( pZHD_Node );
      }
   }

   if ( GetViewByName( &vDiagram, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) < 1 )
   {
      return( 0 );
   }

   CreateViewFromViewForTask( &vDiagram, vDiagram, m_pZSubtask->m_vDialog );
   ResetView( vDiagram );

#if 0
   GetIntegerFromAttribute( &lOrientation, vDiagram,
                            szlHD_Diagram, szlOrientation );
   if ( lOrientation )
      m_bOrientation = TRUE;
   else
      m_bOrientation = FALSE;
#endif

   SetNodeMetrics( );
   PositionNodes( vDiagram );
   RecursDrawNodes( vDiagram, 0 );
   DropView( vDiagram );
   if ( m_bMapped == FALSE && m_nNodeCnt )
   {
      pZHD_Node = m_NodeList[ 0 ];
      pZHD_Node->CenterNode( );
      m_bMapped = TRUE;  // prevent refresh of this control from
                         // re-scrolling the nodes
   }

   return( 0 );
}

void
ZHD_Diagram::SetNodeMetrics( )
{
   for ( m_nLevel = 0; m_nLevel <= MAX_LEVELS; m_nLevel++ )
      m_nMaxNodeEdge[ m_nLevel ] = 0;

   m_nLevel = 0;
   if ( m_bOrientation )
   {
      NODE_HEIGHT     =  9;
      NODE_WIDTH      =  5;
      X_SPACING       =  2;
      HALF_X_SPACING  =  1;
      Y_SPACING       = 15;
   }
   else
   {
      NODE_HEIGHT     =  5;
      NODE_WIDTH      =  9;
      X_SPACING       =  4;
      HALF_X_SPACING  =  2;
      Y_SPACING       =  6;
   }
}

zSHORT
ZHD_Diagram::PositionNodes( zVIEW  vDiagram )
{
   zSHORT  nRC;
   zSHORT  nLeftEdgeStart;
   zSHORT  nPos = 0;
   zSHORT  nFirstChildPos = 0;
   zSHORT  nLastChildPos = 0;
   zSHORT  nChildren = 0;
   zSHORT  nNodeWidth;
   zSHORT  nNodeHeight;

   nLeftEdgeStart = m_nMaxNodeEdge[ m_nLevel ];
   m_nLevel++;

   if ( nLeftEdgeStart < m_nMaxNodeEdge[ m_nLevel ] )
   {
      nLeftEdgeStart = m_nMaxNodeEdge[ m_nLevel ];
   }
   else
   {
      m_nMaxNodeEdge[ m_nLevel ] = nLeftEdgeStart;
   }

   if ( nLeftEdgeStart < m_nMaxNodeEdge[ m_nLevel+1 ] )
   {
      nLeftEdgeStart = m_nMaxNodeEdge[ m_nLevel+1 ];
   }

   nNodeWidth  = NODE_WIDTH;
   nNodeHeight = NODE_HEIGHT;
   if ( CompareAttributeToString( vDiagram, szlHD_Node, szlHidden, "Y" ) == 0 )
   {
      nNodeWidth = 1;
   }

// nLeftEdgeStart -= nNodeWidth;

#if DEBUG_TRACE
   GetAddrForAttribute( &szName, vDiagram, szlHD_Node, szlName );
   wsprintf( szDbg, "lvl=%2d %-34s     LeftEdgeStart=%d",
             m_nLevel, szName, nLeftEdgeStart );
   TraceLineS( "PNode.1 ", szDbg );
#endif

   if ( CompareAttributeToString( vDiagram, szlHD_Node, szlCollapsed, "Y" ) != 0 &&
        CompareAttributeToString( vDiagram, szlHD_Node, szlHidden, "Y" ) != 0 )
   {
      for ( nRC = SetCursorFirstEntity( vDiagram, szlHD_ChildNode, szlHD_Node );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDiagram, szlHD_ChildNode, szlHD_Node ) )
      {
         SetViewToSubobject( vDiagram, szlHD_ChildNode );
         nPos = PositionNodes( vDiagram );
         ResetViewFromSubobject( vDiagram );

         nChildren++;
         if ( nChildren == 1 )
            nFirstChildPos = nPos;

         nLastChildPos = nPos;
      }
   }

#if DEBUG_TRACE
   wsprintf( szDbg, "lvl=%2d %-34s has %d chidren: first=%d, last=%d",
             m_nLevel, szName, nChildren, nFirstChildPos, nLastChildPos );
   TraceLineS( "PNode.. ", szDbg );
#endif
   if ( nChildren == 0 )
   {
      nPos = nLeftEdgeStart;
   }
   else
   {
      nPos = nFirstChildPos;
      nPos += (nLastChildPos - nFirstChildPos + nNodeWidth) / 2;
      nPos -= nNodeWidth / 2;
   }

   if ( m_bOrientation == FALSE )
   {
      SetAttributeFromInteger( vDiagram, szlHD_Node, szlPosX, nPos + 2 );
      SetAttributeFromInteger( vDiagram, szlHD_Node, szlPosY,
                               HALF_X_SPACING + ((m_nLevel - 1) * (nNodeHeight + Y_SPACING)));
   }
   else
   {
      SetAttributeFromInteger( vDiagram, szlHD_Node, szlPosY, nPos + 2 );
      SetAttributeFromInteger( vDiagram, szlHD_Node, szlPosX,
                               HALF_X_SPACING + ((m_nLevel - 1) * (nNodeHeight + Y_SPACING)));
   }

   m_nMaxNodeEdge[ m_nLevel ] = nPos + nNodeWidth + X_SPACING;
#if DEBUG_TRACE
   wsprintf( szDbg, "lvl=%2d %-34s at %d   with MaxEdge=%d",
             m_nLevel, szName, nPos, m_nMaxNodeEdge[ m_nLevel ] );
   TraceLineS( "PNode.9 ", szDbg );
#endif
   m_nLevel--;
   return( nPos );
}

void
ZHD_Diagram::CalculateMinimumScale( CRect& rect )
{
   zLONG  lSizeX, lSizeY;
   zLONG  lScale;

   lScale   = -80;
   CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
   while ( lSizeX < tzHDDIAGRAMX || lSizeY < tzHDDIAGRAMY )
   {
      lScale -= 10;
      if ( lScale == -100 )
      {
         lScale = -99;
         CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
         break;
      }

      CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
   }

   while ( lSizeX > tzHDDIAGRAMX && lSizeY > tzHDDIAGRAMY )
   {
      lScale += 5;
      CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
   }

   if ( lScale > -99 )
   {
      while ( lSizeX < tzHDDIAGRAMX || lSizeY < tzHDDIAGRAMY )
      {
         lScale--;
         if ( lScale == -99 )
            break;

         CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
      }
   }

   m_lMinScale = lScale;
}

void
ZHD_Diagram::CalculateDisplaySize( zLONG&  lDisplaySizeX,
                                   zLONG&  lDisplaySizeY,
                                   CRect&  rect,
                                   zLONG   lScale )
{
   zLONG lRectX, lRectY;

// TraceLineI( "CalculateDisplaySize lScale = ", lScale );

   // Get the logical rect size with scaling.
   lRectX = rect.right  - rect.left;
   lRectY = rect.bottom - rect.top;
   if ( lScale )
   {
      lRectX = (lRectX * 100) / (100 + lScale);
      lRectY = (lRectY * 100) / (100 + lScale);
   }

   // Calculate the new display size.
   lDisplaySizeX = lRectX / tzHDCELLSIZE;
   if ( lRectX % tzHDCELLSIZE )
      lDisplaySizeX++;

   lDisplaySizeY = lRectY / tzHDCELLSIZE;
   if ( lRectY % tzHDCELLSIZE )
      lDisplaySizeY++;
}

void
ZHD_Diagram::SetScrollBars( )
{
   if ( (PosY( m_lDisplayPosMax ) - PosY( m_lDisplayPos )) < (tzHDDIAGRAMY - 1) )
   {
      SetScrollRange( SB_VERT, 0, tzHDDIAGRAMY - ((PosY( m_lDisplayPosMax ) - PosY(m_lDisplayPos)) + 1), FALSE );
      SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
   }
   else
      SetScrollRange( SB_VERT, 0, 0, TRUE );

   if ( (PosX( m_lDisplayPosMax ) - PosX( m_lDisplayPos )) < (tzHDDIAGRAMX - 1) )
   {
      SetScrollRange( SB_HORZ, 0, tzHDDIAGRAMX - (((PosX(m_lDisplayPosMax) - PosX(m_lDisplayPos)) + 1)), FALSE );
      SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
   }
   else
      SetScrollRange( SB_HORZ, 0, 0, TRUE );
}

zSHORT
ZHD_Diagram::PosX( zLONG lIdx )
{
   return( (zSHORT) (lIdx % tzHDDIAGRAMX) );
}

zSHORT
ZHD_Diagram::PosY( zLONG lIdx )
{
   return( (zSHORT) (lIdx / tzHDDIAGRAMX) );
}

zSHORT
ZHD_Diagram::SetProperties( zLONG lProperties )
{
   if ( lProperties == 0 )
      m_bOrientation = FALSE;
   else
   if ( lProperties == 1 )
      m_bOrientation = TRUE;
   else
   if ( lProperties == 2 )
   {
      m_bOrientation = !m_bOrientation;
   }
   else
      return( 1 );

   return( 0 );
}

//
//  METHOD:  GetDeviceContext
//
//  PURPOSE: This method retrieves the device context for the diagram
//           and scales it to the current scale.
//
void
ZHD_Diagram::GetDeviceContext( )
{
   if ( m_hDC )
      return;

   m_hDC = new CClientDC( this );
   if ( m_lScale )
   {
      m_nOldMapMode = m_hDC->SetMapMode( MM_ISOTROPIC );
      m_sizeOldWExt = m_hDC->SetWindowExt( CSize( 100, 100 ) );
      m_sizeOldVExt = m_hDC->SetViewportExt( CSize( 100 + m_lScale, 100 + m_lScale ) );
   }
}

//
//  METHOD:  ReleaseDeviceContext
//
//  PURPOSE: This method releases the device context for the diagram.
//
void
ZHD_Diagram::ReleaseDeviceContext( )
{
   if ( m_hDC == 0 )
      return;

   if ( m_lScale )
   {
      m_nOldMapMode = m_hDC->SetMapMode( m_nOldMapMode );
      m_sizeOldWExt = m_hDC->SetViewportExt( CSize( m_sizeOldWExt ) );
      m_sizeOldVExt = m_hDC->SetViewportExt( CSize( m_sizeOldVExt ) );
   }

   mDeleteInit( m_hDC );
}

//
//  METHOD:  PositionVisible
//
//  PURPOSE: This method takes two grid positions and determines if
//           ths rectangle they represent is visible in the window.
//
zBOOL
ZHD_Diagram::PositionVisible( zLONG lStartIdx, zLONG lEndIdx )
{
   zSHORT nPos1, nPos2, nPos3, nPos4;

   if ( m_lUpdateDisplayPos == -1 )
   {
//    if ( m_bPrintDC )
//       return( 1 );

      if ( (lStartIdx < m_lDisplayPos && lEndIdx < m_lDisplayPos) ||
           (lStartIdx > m_lDisplayPosMax && lEndIdx > m_lDisplayPosMax) )
      {
         return( 0 );
      }

      nPos1 = PosX( lStartIdx );
      nPos2 = PosX( lEndIdx );
      nPos3 = PosX( m_lDisplayPos );
      if ( nPos1 < nPos3 && nPos2 < nPos3 )
         return( 0 );

      nPos4 = PosX( m_lDisplayPosMax );
      if ( nPos1 > nPos4 && nPos2 > nPos4 )
         return( 0 );

      nPos1 = PosY( lStartIdx );
      nPos2 = PosY( lEndIdx );
      nPos3 = PosY( m_lDisplayPos );
      if ( nPos1 < nPos3 && nPos2 < nPos3 )
         return( 0 );

      nPos4 = PosY( m_lDisplayPosMax );
      if ( nPos1 > nPos4 && nPos2 > nPos4 )
         return( 0 );
   }
   else
   {
      if ( (lStartIdx < m_lUpdateDisplayPos && lEndIdx < m_lUpdateDisplayPos) ||
           (lStartIdx > m_lUpdateDisplayPosMax && lEndIdx > m_lUpdateDisplayPosMax) )
      {
         return( 0 );
      }

      nPos1 = PosX( lStartIdx );
      nPos2 = PosX( lEndIdx );
      nPos3 = PosX( m_lUpdateDisplayPos );
      if ( nPos1 < nPos3 && nPos2 < nPos3 )
         return( 0 );

      nPos4 = PosX( m_lUpdateDisplayPosMax );
      if ( nPos1 > nPos4 && nPos2 > nPos4 )
         return( 0 );

      nPos1 = PosY( lStartIdx );
      nPos2 = PosY( lEndIdx );
      nPos3 = PosY( m_lUpdateDisplayPos );
      if ( nPos1 < nPos3 && nPos2 < nPos3 )
         return( 0 );

      nPos4 = PosY( m_lUpdateDisplayPosMax );
      if ( nPos1 > nPos4 && nPos2 > nPos4 )
         return( 0 );
   }

   return( 1 );   // position is visible
}


void
ZHD_Diagram::SetMaxPositions( )
{
   zSHORT    nPos;
   ZHD_Node  *pZHD_Node;

   m_lMinPosX = 0;
   m_lMaxPosX = 0;
   m_lMinPosY = 0;
   m_lMaxPosY = 0;
   m_pMinPosXNode = 0;
   m_pMaxPosXNode = 0;
   m_pMinPosYNode = 0;
   m_pMaxPosYNode = 0;
   if ( m_nNodeCnt )
   {
      m_lMinPosX = 32767;
      m_lMinPosY = 32767;
      for ( nPos = 0; nPos < m_nNodeCnt; nPos++ )
      {
         pZHD_Node = m_NodeList[ nPos ];
         if ( pZHD_Node->PosX( ) < m_lMinPosX )
         {
            m_lMinPosX = pZHD_Node->PosX( );
            m_pMinPosXNode = pZHD_Node;
         }

         if ( pZHD_Node->PosX( ) > m_lMaxPosX )
         {
            m_lMaxPosX = pZHD_Node->PosX( );
            m_pMaxPosXNode = pZHD_Node;
         }

         if ( pZHD_Node->PosY( ) < m_lMinPosY )
         {
            m_lMinPosY = pZHD_Node->PosY( );
            m_pMinPosYNode = pZHD_Node;
         }

         if ( pZHD_Node->PosY( ) > m_lMaxPosY )
         {
            m_lMaxPosY = pZHD_Node->PosY( );
            m_pMaxPosYNode = pZHD_Node;
         }
      }
   }
}

//
//  METHOD: SetDisplayBoundaries
//
//  PURPOSE: This method takes the physical rect of the window and
//           calculates the Grid positions which are the visible
//           display boundaries of the diagram so painting can
//           determine which objects are visible. The class variables
//           m_lDisplayPos and m_lDisplayPosMax are set EXCEPT in PAINT
//           situations when the lUpdateDisplayPos and lUpdateDisplayPosMax
//           are set.
//
//
void
ZHD_Diagram::SetDisplayBoundaries( CRect& rect, zBOOL bUpdate )
{
   zLONG lRectX;
   zLONG lRectY;
   zLONG lTop;
   zLONG lLeft;

   if ( bUpdate )
   {
      // Now set the update region in the class from the rect to be painted.
      lTop   = rect.top;
      lLeft  = rect.left;
      lRectX = rect.right  - rect.left;
      lRectY = rect.bottom - rect.top;
      if ( m_lScale )
      {
         lTop   = (lTop * 100) / (100 + m_lScale);
         lLeft  = (lLeft * 100) / (100 + m_lScale);
         lRectX = (lRectX * 100) / (100 + m_lScale);
         lRectY = (lRectY * 100) / (100 + m_lScale);
      }

      m_lUpdateDisplayPos = ((zLONG) PosY( m_lDisplayPos ) * tzHDDIAGRAMX) +
                            ((zLONG) (lTop / tzHDCELLSIZE) * tzHDDIAGRAMX);
      m_lUpdateDisplayPos += PosX( m_lDisplayPos ) + (lLeft / tzHDCELLSIZE);
      m_lUpdateDisplayPosMax = m_lUpdateDisplayPos;
      if ( lRectX >= (tzHDCELLSIZE * tzHDDIAGRAMX) )
         lRectX = (tzHDCELLSIZE * (tzHDDIAGRAMX - 1));

      m_lUpdateDisplayPosMax += (zLONG)
         ((lRectY / tzHDCELLSIZE) * tzHDDIAGRAMX) + (lRectX / tzHDCELLSIZE);

      // Always increase update area to ensure painting
      if ( PosX( m_lUpdateDisplayPos ) )
         m_lUpdateDisplayPos--;

      if ( PosY( m_lUpdateDisplayPos ) )
         m_lUpdateDisplayPos -= tzHDDIAGRAMX;

      m_lUpdateDisplayPosMax += tzHDDIAGRAMX;
      if ( PosX( m_lUpdateDisplayPosMax ) < (tzHDDIAGRAMX - 1) )
         m_lUpdateDisplayPosMax++;

      if ( m_lUpdateDisplayPosMax > tzHDMAXGRID )
         m_lUpdateDisplayPosMax = tzHDMAXGRID;
   }
   else
   {
      lRectX = rect.right - rect.left;
      lRectY = rect.bottom - rect.top;
      if ( m_lScale )
      {
         lRectX = (lRectX * 100) / (100 + m_lScale);
         lRectY = (lRectY * 100) / (100 + m_lScale);
      }

      if ( lRectX >= (tzHDCELLSIZE * tzHDDIAGRAMX) )
         lRectX = (tzHDCELLSIZE * (tzHDDIAGRAMX - 1));

      SetDisplayPosMax( lRectX, lRectY );
   }
}

void
ZHD_Diagram::SetDisplayPosMax( zLONG lX, zLONG lY )
{
   m_lDisplayPosMax = m_lDisplayPos + ((lY / tzHDCELLSIZE) * tzHDDIAGRAMX) + (lX / tzHDCELLSIZE);

   if ( lY % tzHDCELLSIZE )
      m_lDisplayPosMax += tzHDDIAGRAMX;

   if ( lX % tzHDCELLSIZE )
      m_lDisplayPosMax++;

   if ( m_lDisplayPosMax > tzHDMAXGRID )
      m_lDisplayPosMax = tzHDMAXGRID;
}

//
//  METHOD: MakeLogicalPoint
//
//  PURPOSE: This method takes the physical mouse position and
//           determines the logical point of the position by
//           getting the physical point value and scaling it by
//           the current zooming scale factor.
//
void
ZHD_Diagram::MakeLogicalPoint( CPoint& pt )
{
   zLONG lPosY;
   zLONG lPosX;

   // If the diagram is scaled, increase or decrease the position based
   // on the scale.
   lPosX = pt.x;
   lPosY = pt.y;
   if ( m_lScale )
   {
      pt.x = ((lPosX * 100) / (100 + m_lScale));
      pt.y = ((lPosY * 100) / (100 + m_lScale));
   }

   pt.x /= tzHDCELLSIZE;
   pt.y /= tzHDCELLSIZE;
   pt.x += PosX( m_lDisplayPos );
   pt.y += PosY( m_lDisplayPos );
}

//
//  METHOD: PositionOnDiagramNode
//
//
zVIEW
ZHD_Diagram::PositionOnDiagramNode( ZHD_Node *pZHD_Node, zVIEW vDiagram )
{
   zSHORT   nParents, nWk;
   ZHD_Node *pWkNode;

   if ( vDiagram == 0 )
   {
      if ( GetViewByName( &vDiagram, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) < 1 )
      {
         return( 0 );
      }
   }

   nParents = 0;
   pWkNode = pZHD_Node;
   while ( pWkNode->m_pParentNode )
   {
      nParents++;
      pWkNode = pWkNode->m_pParentNode;
   }

   while ( ResetViewFromSubobject( vDiagram ) == 0 )
   {
   }

   while ( nParents )
   {
      pWkNode = pZHD_Node;
      for ( nWk = nParents - 1; nWk; nWk-- )
      {
         pWkNode = pWkNode->m_pParentNode;
      }

      SetCursorFirstEntityByInteger( vDiagram, szlHD_ChildNode, szlZKey, pWkNode->m_lZKey, 0 );
      SetViewToSubobject( vDiagram, szlHD_ChildNode );
      nParents--;
   }

   return( vDiagram );
}

//
//  METHOD: PointInNode
//
//
ZHD_Node *
ZHD_Diagram::PointInNode( CPoint pt )
{
   zSHORT   nPos;
   ZHD_Node *pNode;
   CPoint   ptNode;

   // The list destructor attempts to free the memory for the items.
   // However, we do the free anyway for neurotic reasons!
   for ( nPos = 0; nPos < m_nNodeCnt; nPos++ )
   {
      pNode = m_NodeList[ nPos ];
      ptNode.x = pNode->PosX( );
      ptNode.y = pNode->PosY( );
      if ( pNode->m_bHidden )
      {
         if ( ptNode.x == pt.x && ptNode.y == pt.y )
         {
            m_pLastPosNode = pNode;
            return( pNode );
         }
      }
      else
      if ( ptNode.x <= pt.x && (ptNode.x + tzHDNODEX) > pt.x &&
           ptNode.y <= pt.y && (ptNode.y + tzHDNODEY) > pt.y )
      {
         m_pLastPosNode = pNode;
         return( pNode );
      }
   }

   return( 0 );
}

zBOOL
ZHD_Diagram::IsDoubleClick( zUSHORT uMessage, CPoint ptStart )
{
   MSG     msg;
   zULONG  ulInterval = 0;
   zLONG   lMessage = GetMessageTime( );
   zULONG  ulDblclkTime = GetDoubleClickTime( );

#ifdef DEBUG_ALL
   TraceLineI( "ZHD_Diagram::IsDoubleClick Time: ", ulDblclkTime );
   TraceLineI( "                    MessageTime: ", lMessage );
#endif

   while ( ulInterval < ulDblclkTime )
   {
      ulInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage, PM_NOREMOVE | PM_NOYIELD ) )
      {
      // TraceLineI( "ZHD_Diagram::IsDoubleClick TRUE: ", ulInterval );
         return( TRUE );
      }
      else
      if ( ::PeekMessage( &msg, m_hWnd, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE | PM_NOYIELD ) )
      {
         CPoint pt( msg.lParam );
         if ( ptStart.x - pt.x > g_lDblClkX ||
              pt.x - ptStart.x > g_lDblClkX ||
              ptStart.y - pt.y > g_lDblClkY ||
              pt.y - ptStart.y > g_lDblClkY )
         {
         // TracePoint( "ZHD_Diagram::IsDoubleClick FALSE: ", pt );
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

// Message cracking METHOD for LButtonDown
//
// MODES:  If the mouse is currently positioned on an Entity, move the
//         Entity. If the Mouse is currently positioned on a relationship
//         move the relationship. Otherwise, paint a new entity.
//
void
ZHD_Diagram::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnLButtonDown", "" );
#endif

   if ( m_nLDblClk )
   {
      if ( m_nLDblClk == 1 )
         return;

      m_nLDblClk = 0;
   }

   // Wait to see if it is a double-click.
   if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) )
   {
      m_nLDblClk = 1;
   }
}

// Message cracking METHOD for LButtonUp
void
ZHD_Diagram::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnLButtonUp", "" );
#endif

   ZHD_Node *pZHD_Node;
   zVIEW    vDiagram;

   if ( m_nLDblClk )
   {
      if ( m_nLDblClk == 2 )
         m_nLDblClk = 0;

      return;
   }

   m_ptMouseSelect = pt;
   MakeLogicalPoint( pt );
   if ( (pZHD_Node = PointInNode( pt )) != 0 )
   {
      if ( m_pHD_Diagram->m_bCollapsable && pZHD_Node->m_bHidden == FALSE &&
           pt.x == pZHD_Node->PosX( ) && pt.y == pZHD_Node->PosY( ) )
      {
         vDiagram = PositionOnDiagramNode( pZHD_Node );
         if ( pZHD_Node->m_bCollapsed )
         {
            SetAttributeFromString( vDiagram, szlHD_Node, szlCollapsed, "N" );
            if ( SetCursorFirstEntity( vDiagram, szlHD_ChildNode, 0 ) == zCURSOR_SET )
            {
               do
               {
                  SetAttributeFromString( vDiagram, szlHD_ChildNode, szlCollapsed, "Y" );
                  SetAttributeFromString( vDiagram, szlHD_ChildNode, szlHidden, "N" );
               } while ( SetCursorNextEntity( vDiagram, szlHD_ChildNode, 0 ) == zCURSOR_SET );
            }
         }
         else
         {
            SetAttributeFromString( vDiagram, szlHD_Node, szlCollapsed, "Y" );
         }

         if ( pZHD_Node->m_bCollapsed )
         {
// TraceLineS("(zdctlhd) Processing event 5","");
            ProcessEvent( m_pHD_Diagram, 5 );
         }
         else
         {
// TraceLineS("(zdctlhd) Processing event 6","");
            ProcessEvent( m_pHD_Diagram, 6 );
         }

         // Get index of node that was expanded or collapsed since MapFromOI
         // will rebuild the node list.
         zSHORT nIdx = m_nNodeCnt;
         while ( nIdx-- )
         {
            if ( pZHD_Node == m_NodeList[ nIdx ] )
               break;
         }

         MapFromOI( );
         pZHD_Node = m_NodeList[ nIdx ];
         pZHD_Node->CenterNode( );
      }
      else
      {
         pZHD_Node->Select( );
// TraceLineS("(zdctlhd) Processing event 1","");
         ProcessEvent( m_pHD_Diagram, 1 );
      }
   }
   else
   {
// TraceLineS("(zdctlhd) Processing event 7","");
      ProcessEvent( m_pHD_Diagram, 7 );
   }
}

// Message cracking METHOD for LButtonDblClk
void
ZHD_Diagram::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnLButtonDblClk", "" );
#endif

   ZHD_Node *pZHD_Node;

   if ( m_nLDblClk == 0 )
      return;

   m_nLDblClk = 2;

   m_ptMouseSelect = pt;
   MakeLogicalPoint( pt );
   if ( (pZHD_Node = PointInNode( pt )) != 0 )
   {
      pZHD_Node->Select( );
// TraceLineS("(zdctlhd) Processing event 2","");
      ProcessEvent( m_pHD_Diagram, 2 );
   }
   else
   {
// TraceLineS("(zdctlhd) Processing event 8","");
      ProcessEvent( m_pHD_Diagram, 8 );
   }
}

// Message cracking METHOD for RButtonDown
//
void
ZHD_Diagram::OnRButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZHD_Diagram::OnRButtonDown", pt.x );
   TraceLineI( "             OnRButtonDown", pt.y );
#endif

   if ( m_nRDblClk )
   {
      if ( m_nRDblClk == 1 )
         return;

      m_nRDblClk = 0;
   }

   if ( IsDoubleClick( WM_RBUTTONDBLCLK, pt ) )
   {
      m_nRDblClk = 1;
   }

   ZHD_Node *pZHD_Node;
   m_ptMouseSelect = pt;
   MakeLogicalPoint( pt );
   if ( (pZHD_Node = PointInNode( pt )) != 0 )
   {
      PositionOnDiagramNode( pZHD_Node );
// TraceLineS("(zdctlhd) Processing event 3","");
      ProcessEvent( m_pHD_Diagram, 3 );
   }
   else
   {
// TraceLineS("(zdctlhd) Processing event 9","");
      ProcessEvent( m_pHD_Diagram, 9 );
   }
}

// Message cracking METHOD for RButtonUp
void
ZHD_Diagram::OnRButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnRButtonUp", "" );
#endif

   if ( m_nRDblClk )
   {
      if ( m_nRDblClk == 2 )
         m_nRDblClk = 0;

      return;
   }
}

// Message cracking METHOD for RButtonDblClk
void
ZHD_Diagram::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnRButtonDblClk", "" );
#endif

   ZHD_Node *pZHD_Node;

   if ( m_nRDblClk == 0 )
      return;

   m_nRDblClk = 2;
   m_ptMouseSelect = pt;
   MakeLogicalPoint( pt );
   if ( (pZHD_Node = PointInNode( pt )) != 0 )
   {
      PositionOnDiagramNode( pZHD_Node );
//    TraceLineS( "(zdctlhd) Processing event 4", "" );
      ProcessEvent( m_pHD_Diagram, 4 );
   }
   else
   {
//    TraceLineS( "(zdctlhd) Processing event 10", "" );
      ProcessEvent( m_pHD_Diagram, 10 );
   }
}

// Message cracking METHOD for HScroll
void
ZHD_Diagram::OnHScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
   zLONG  lWk;
   zLONG  lScroll;

#ifdef DEBUG_ALL
   TraceLineI( "ZHD_Diagram::HScroll uSBCode", uSBCode );
#endif

   // SB_PAGELEFT, SB_PAGERIGHT, SB_THUMBPOSITION to LOWORD( lParam ),
   // SB_LEFT and SB_RIGHT (far left and right)
   switch ( uSBCode )
   {
      case SB_LINELEFT:
         if ( PosX( m_lDisplayPos ) >= tzHDNODEX )
         {
            m_lDisplayPos -= tzHDNODEX;
            m_lDisplayPosMax -= tzHDNODEX;

            lScroll = tzHDCELLSIZE * tzHDNODEX;
            if ( m_lScale )
               lScroll = (lScroll * (m_lScale + 100)) / 100;

            ScrollWindow( lScroll, 0, 0, 0 );
            SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
            UpdateWindow( );
            m_bScrolledWindow = TRUE;
            break;
         }
         else
         if ( m_lDisplayPos == 0 )
         {
            break;
         }

         // otherwise INTENTIONALLY fall thru to PAGELEFT case

      case SB_PAGELEFT:
         lWk = PosX( m_lDisplayPosMax ) - PosX( m_lDisplayPos );
         if ( lWk > 2 )
            lWk -= 2;

         if ( lWk > PosX( m_lDisplayPos ) )
            lWk = PosX( m_lDisplayPos );

         m_lDisplayPos -= lWk;
         m_lDisplayPosMax -= lWk;
         lScroll = tzHDCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( lScroll, 0, 0, 0 );
         SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
         break;

      case SB_LINERIGHT:
         if ( PosX( m_lDisplayPosMax ) < (tzHDDIAGRAMX - (tzHDNODEX + 1)) )
         {
            m_lDisplayPos += tzHDNODEX;
            m_lDisplayPosMax += tzHDNODEX;
            lScroll = -tzHDCELLSIZE * tzHDNODEX;
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

         if ( (lWk + PosX( m_lDisplayPosMax )) >= tzHDDIAGRAMX )
            lWk = tzHDDIAGRAMX - (PosX( m_lDisplayPosMax ) + 1);

         m_lDisplayPos += lWk;
         m_lDisplayPosMax += lWk;
         lScroll = -tzHDCELLSIZE * lWk;
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
         lScroll = -tzHDCELLSIZE * lWk;
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
ZHD_Diagram::OnVScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
   zLONG  lWk;
   zLONG  lScroll;

#ifdef DEBUG_ALL
   TraceLineI( "ZHD_Diagram::VScroll uSBCode", uSBCode );
#endif

   // SB_PAGEDOWN, SB_PAGEUP, SB_THUMBPOSITION to LOWORD(lParam),
   // SB_THUMBTRACK to LOWORD(lParam),
   // SB_BOTTOM and SB_TOP (far top and bottom)
   switch ( uSBCode )
   {
      case SB_LINEUP:
         if ( PosY( m_lDisplayPos ) >= tzHDNODEY )
         {
            m_lDisplayPos -= (tzHDNODEY * tzHDDIAGRAMX);
            m_lDisplayPosMax -= (tzHDNODEY * tzHDDIAGRAMX);
            lScroll = tzHDCELLSIZE * tzHDNODEY;
            if ( m_lScale )
               lScroll = (lScroll * (m_lScale + 100)) / 100;

            ScrollWindow( 0, lScroll, 0, 0 );
            SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
            UpdateWindow( );
            m_bScrolledWindow = TRUE;
            break;
         }
         else
         if ( m_lDisplayPos == 0 )
         {
            break;
         }

         // otherwise INTENTIONALLY fall thru to PAGEUP case

      case SB_PAGEUP:
         lWk = PosY( m_lDisplayPosMax ) - PosY( m_lDisplayPos );
         if ( lWk > 2 )
            lWk -= 2;

         if ( lWk > PosY( m_lDisplayPos ) )
            lWk = PosY( m_lDisplayPos );

         m_lDisplayPos -= lWk * tzHDDIAGRAMX;
         m_lDisplayPosMax -= lWk * tzHDDIAGRAMX;
         lScroll = tzHDCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( 0, lScroll, 0, 0 );
         SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
         break;

      case SB_LINEDOWN:
         if ( PosY( m_lDisplayPosMax ) < (tzHDDIAGRAMY - (tzHDNODEY + 1)) )
         {
            m_lDisplayPos += (tzHDNODEY * tzHDDIAGRAMX);
            m_lDisplayPosMax += (tzHDNODEY * tzHDDIAGRAMX);
            lScroll = -tzHDCELLSIZE * tzHDNODEY;
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

         if ( lWk + PosY( m_lDisplayPosMax ) >= tzHDDIAGRAMY )
            lWk = tzHDDIAGRAMY - (PosY( m_lDisplayPosMax ) + 1);

         m_lDisplayPos += lWk * tzHDDIAGRAMX;
         m_lDisplayPosMax += lWk * tzHDDIAGRAMX;
         lScroll = -tzHDCELLSIZE * lWk;
         if ( m_lScale )
            lScroll = (lScroll * (m_lScale + 100)) / 100;

         ScrollWindow( 0, lScroll, 0, 0 );
         SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         lWk = uThumbPos;
         lWk -= PosY( m_lDisplayPos );
         m_lDisplayPos    += lWk * tzHDDIAGRAMX;
         m_lDisplayPosMax += lWk * tzHDDIAGRAMX;
         lScroll = -tzHDCELLSIZE * lWk;
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

// Message cracking METHOD for Paint
void
ZHD_Diagram::OnPaint( )
{
   CRect  rect;

   if ( GetUpdateRect( rect, 1 ) )
   {
      int    nOldMapMode;
      CSize  sizeOldWExt;
      CSize  sizeOldVExt;

      CDC *pSaveDC = m_hDC;
      m_hDC = new CPaintDC( this );

      // Now set the update region in the class from the rect to be painted
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

void
ZHD_Diagram::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnSetFocus ", *m_pzsTag );
#endif
   m_pZSubtask->InformDriverOfSetFocus( this );
}

void
ZHD_Diagram::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZHD_Diagram::OnKillFocus", *m_pzsTag );
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
HierDiagram( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZHD_Diagram( pZSubtask, pWndParent,
                            pzmaComposite, vDialog,
                            nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=HD_ZoomDiagram
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: HD_ZoomDiagram
//
//  PURPOSE:   This Operation is used by applications to send the Zoom
//             message to the Hierarchical Diagram control.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the Hierarchical Control.
//               cpcCtrlTag - The tag of the Hierarchical Control
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
//  RETURNS:    -1  - Error locating window or Hierarchical control
//              Note that if nPercent and nOptions are both 0, the return
//              value is the current Zooming Scale.  Otherwise, the return
//              value is zero.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
HD_ZoomDiagram( zVIEW    vSubtask,
                zCPCHAR  cpcCtrlTag,
                zSHORT   nPercent,
                zSHORT   nOptions )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         return( pHD_Diagram->ZoomDiagram( nPercent, nOptions ) );
      }

      TraceLineS( "drvr - Invalid control type for HD_ZoomDiagram ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=HD_PrintDiagram
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: HD_PrintDiagram
//
//  PURPOSE:   This Operation is used by applications to send the Print
//             message to the Hierarchical Diagram control.
//
//  PARAMETERS:  vSubtask - the subtask handle to the window containing
//                          the Hierarchical Control.
//               cpcCtrlTag - The tag of the Hierarchical Control
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
//  RETURNS:    -1 - Error locating window or Hierarchical control.
//               0 - Print OK.
//               1 - Print Canceled.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
HD_PrintDiagram( zVIEW    vSubtask,
                 zCPCHAR  cpcCtrlTag,
                 zSHORT   nPercent,
                 zSHORT   nOptions )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         return( pHD_Diagram->PrintDiagram( nPercent, nOptions ) );
      }

      TraceLineS( "drvr - Invalid control type for HD_PrintDiagram ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=HD_GetPointerCoordinates
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: HD_GetPointerCoordinates
//
//  PURPOSE:   This Operation returns the X and Y position of the mouse
//             when an event was positioned, the coordinates returned are
//             screen coordinates!!!
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
HD_GetPointerCoordinates( zVIEW    vSubtask,
                          zCPCHAR  cpcCtrlTag,
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
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         pt = pHD_Diagram->m_ptMouseSelect;
         pHD_Diagram->ClientToScreen( &pt );
         *plPosX = pt.x;
         *plPosY = pt.y;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for HD_GetPointerCoordinates ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=HD_SetProperties
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: HD_SetProperties
//
//  PURPOSE:   This Operation sets the orientation of the diagram to either
//             0 - vertical
//             1 - horizontal
//             2 - switch
//
//  RETURNS:    -1 - Error locating window or Hierarchical control.
//               0 - Properties set OK
//               1 - Properties not set (invalid values)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
HD_SetProperties( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zLONG   lProperties )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         return( pHD_Diagram->SetProperties( lProperties ) );
      }

      TraceLineS( "drvr - Invalid control type for HD_SetProperties ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=HD_SetColors
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: HD_SetColors
//
//  PURPOSE:   This Operation set the colors for the diagram
//             and optionally redraw the diagram.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
zOPER_EXPORT void OPERATION
HD_SetColors( zVIEW    vSubtask,
              zCPCHAR  cpcCtrlTag,
              zPULONG  pulColorNormal,
              zULONG   ulColorSelected,
              zULONG   ulColorBox,
              zULONG   ulColorLines,
              zULONG   ulColorText,
              zULONG   ulColorNoUpdate,
              zSHORT   bRedraw )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zSHORT   nIdx;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         if ( pulColorNormal && *pulColorNormal )
         {
            for ( nIdx = 0; nIdx < 16; nIdx++ )
            {
               pHD_Diagram->m_clrNormal[ nIdx ] = *pulColorNormal;
               pulColorNormal++;
               if ( !(*pulColorNormal) )
                  break;
            }
         }

         if ( ulColorSelected )
            pHD_Diagram->m_clrSelected = ulColorSelected;

         if ( ulColorBox )
            pHD_Diagram->m_clrBox = ulColorBox;

         if ( ulColorLines )
            pHD_Diagram->m_clrLines = ulColorLines;

         if ( ulColorText )
            pHD_Diagram->m_clrText = ulColorText;

         if ( ulColorNoUpdate )
            pHD_Diagram->m_clrNoUpdate = ulColorNoUpdate;

         if ( bRedraw )
         {
            pHD_Diagram->MapFromOI( );
            pHD_Diagram->Invalidate( TRUE );
         }

         return;
      }

      TraceLineS( "drvr - Invalid control type for HD_SetColors ",
                  cpcCtrlTag );
   }
}

//./ ADD NAME=HD_CenterNode
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT void OPERATION
HD_CenterNode( zVIEW    vSubtask,
               zCPCHAR  cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         if ( GetViewByName( &vDiagram, *(pzma->m_pzsVName),
                             pHD_Diagram->m_pZSubtask->m_vDialog,
                             zLEVEL_ANY ) > 0 )
         {
            ZHD_Node *pZHD_Node;
            zLONG    lRelNode;

            GetAbsolutePositionForEntity( &lRelNode, vDiagram, szlHD_Node );
            lRelNode--;
            if ( lRelNode >= 0 &&
                 lRelNode < pHD_Diagram->m_nNodeCnt )
            {
               pZHD_Node = pHD_Diagram->m_NodeList[ lRelNode ];
               pZHD_Node->CenterNode( );
            }
         }

         return;
      }

      TraceLineS( "drvr - Invalid control type for HD_CenterNode ", cpcCtrlTag );
   }
}

//./ ADD NAME=HD_SelectNode
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT void OPERATION
HD_SelectNode( zVIEW    vSubtask,
               zCPCHAR  cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         if ( GetViewByName( &vDiagram, *(pzma->m_pzsVName),
                             pHD_Diagram->m_pZSubtask->m_vDialog,
                             zLEVEL_ANY ) > 0 )
         {
            ZHD_Node *pZHD_Node;
            zLONG    lRelNode;

            GetAbsolutePositionForEntity( &lRelNode, vDiagram, szlHD_Node );
         // TraceLineI( "HD_CenterNode AbsolutePos: ", lRelNode );
            lRelNode--;
            if ( lRelNode >= 0 && lRelNode < pHD_Diagram->m_nNodeCnt )
            {
               pZHD_Node = pHD_Diagram->m_NodeList[ lRelNode ];
               pZHD_Node->Select( );
            }
         }

         return;
      }

      TraceLineS( "drvr - Invalid control type for HD_SelectNode ",
                  cpcCtrlTag );
   }
}

//./ ADD NAME=HD_IsNodeVisible
// Source Module=zdctl.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: HD_IsNodeVisible
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
HD_IsNodeVisible( zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vDiagram;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZHD_Diagram *pHD_Diagram = DYNAMIC_DOWNCAST( ZHD_Diagram, pzma->m_pCtrl );
      if ( pHD_Diagram )
      {
         if ( GetViewByName( &vDiagram, *(pzma->m_pzsVName),
                             pHD_Diagram->m_pZSubtask->m_vDialog,
                             zLEVEL_ANY ) > 0 )
         {
            ZHD_Node *pZHD_Node;
            zLONG    lRelNode;

            GetAbsolutePositionForEntity( &lRelNode, vDiagram, szlHD_Node );
            lRelNode--;
            if ( lRelNode >= 0 &&
                 lRelNode < pHD_Diagram->m_nNodeCnt )
            {
               pZHD_Node = pHD_Diagram->m_NodeList[ (zSHORT) lRelNode ];
               if ( pHD_Diagram->PositionVisible( pZHD_Node->m_lPosIdx,
                                                  pZHD_Node->m_lPosIdx ) )
               {
                  zLONG lPos = pZHD_Node->m_lPosIdx + (tzHDNODEX - 1) +
                                               ((tzHDDIAGRAMX * tzHDNODEY) - 1);
                  return( pHD_Diagram->PositionVisible( lPos, lPos ) );
               }

               return( 0 );
            }
         }
      }
      else
         TraceLineS( "drvr - Invalid control type for HD_IsNodeVisible ",
                     cpcCtrlTag );
   }

   return( -1 );
}
