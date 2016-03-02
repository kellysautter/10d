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
// Source file for implementation of Zeidon Entity/Relationship control.
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
// 2000.01.17    HH     Z10
//    Fixed problem with Flags attribute of ER_Rellink. It is not
//    in SA_Rellink and so the code did not work for a SubjectArea.
//    As it is never used (DKS!), just replace it by 0.
//
// 1999.12.06    DKS    Z10    TB861
//    Fixed Redraw problems created when zooming.
//
// 1999.11.01    DKS    Z10    TB788
//    Relationships added that cannot be drawn are deleted.
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

IMPLEMENT_DYNAMIC( ZER_Diagram, CWnd )

BEGIN_MESSAGE_MAP( ZER_Diagram, CWnd )
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
   ON_WM_MOUSEWHEEL( )
   ON_WM_WINDOWPOSCHANGED( )
END_MESSAGE_MAP( )


CString ZER_Diagram::m_csClassName = _T( "" );

ZER_Diagram::ZER_Diagram( ZSubtask *pZSubtask,
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
                      "ER" )
{
   CRect  rect;
   zLONG  lAlloc;
   zPCHAR pchWork;
   zCHAR  szTemp[ zMAX_FILENAME_LTH ];

   if ( m_csClassName.IsEmpty( ) )
   {
      m_csClassName =
         ::AfxRegisterWndClass( CS_DBLCLKS,
                                ::LoadCursor( NULL, IDC_ARROW ),
                                ::GetSysColorBrush( COLOR_BTNFACE ) );
   }

   m_pER_Diagram = this;     // Set a pointer to ourself for defines
   m_lDisplayPos = 0;
   m_lCellSize   = 9;
   m_lScale      = 0;
   m_bScrolledWindow = FALSE;
   m_nEntCnt = 0;
   m_nRelCnt = 0;
   m_EntityList.SetSize( 100, 20 );
   m_RelationshipList.SetSize( 100, 20 );

   m_hDC = 0;
   m_bPrintDC = FALSE;
   m_nCapture = 0;
   m_bUpdateView = TRUE;
   m_bMultMap = FALSE;
   m_lMinPosX = 0;
   m_lMinPosY = 0;
   m_lMaxPosX = 0;
   m_lMaxPosY = 0;
   m_nLDblClk = 0;
   m_nRDblClk = 0;
   m_bShowLines = TRUE;
   m_bShowText = TRUE;
   m_bShowCard = TRUE;
   m_pPendingEntity = 0;
   m_pPendingRelationship = 0;

   m_nOldMapMode = 0;
   m_sizeOldWExt = 0;
   m_sizeOldVExt = 0;
   m_lMinScale = 0;
   m_lMouseOffsetX = 0;
   m_lMouseOffsetY = 0;
   m_bShadeEnts = FALSE;
   m_bShadeRels = FALSE;
   m_pchGrid = 0;
   m_hGrid = 0;

   // this will blow if any of VName, EName or AName are null pointers
   // or null strings (painter must guarantee)
   if ( m_pzsVName == 0 )
      return;

   m_clrNormal     = RGB( 255, 255, 0 );
   m_clrSelected = RGB( 255, 0, 0 );
   m_clrBox        = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrBoxU       = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrLines      = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrLinesU     = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrText       = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrTextU      = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrNoUpdate = ::GetSysColor( COLOR_GRAYTEXT );
   m_pMoveEntity = 0;

   m_lDisplayPosMax = (zLONG) (pCtrlDef->SZDLG_Y / tzERCELLSIZE) *
                        tzERDIAGRAMX + (pCtrlDef->SZDLG_X / tzERCELLSIZE);
   if ( pCtrlDef->SZDLG_Y % tzERCELLSIZE )
      m_lDisplayPosMax += tzERDIAGRAMX;

   if ( pCtrlDef->SZDLG_X % tzERCELLSIZE )
      m_lDisplayPosMax++;

   if ( m_lDisplayPosMax > tzERMAXGRID )
      m_lDisplayPosMax = tzERMAXGRID;

   m_lUpdateDisplayPos    = -1;
   m_lUpdateDisplayPosMax = -1;
   m_bRefreshERD = TRUE;

   // Check a flag in the Zeidon INI to see if we should pop up the msg.
   SysReadZeidonIni( -1, "[ZeidonTools]", "RefreshERD", szTemp, zsizeof( szTemp ) );
   if ( ((*szTemp && *(szTemp + 1) == 0) && (*szTemp == 'N' || *szTemp == 'n')) )
      m_bRefreshERD = FALSE;

   lAlloc = ((zLONG) tzERDIAGRAMX * (zLONG) tzERDIAGRAMY);
   m_hGrid = SysAllocMemory( (zCOREMEM) &m_pchGrid, lAlloc, 0, zCOREMEM_ALLOC, 0 );
   // Zero out the grid
   pchWork = m_pchGrid;
   while ( lAlloc > 16384 )
   {
      zmemset( pchWork, 0, 16384 );
      lAlloc -= 16384;
      pchWork += 16384;
   }

   if ( lAlloc )
      zmemset( pchWork, 0, (int) lAlloc );

   Attr.Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_HSCROLL | WS_VSCROLL;
   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
      Attr.Style |= WS_BORDER;

   // Always create an erd ... should never have a parent.
   CreateZ( );
   SetScrollBars( );

   // Finally, do output mapping
   rect.left   = pCtrlDef->PSDLG_X;
   rect.top    = pCtrlDef->PSDLG_Y;
   rect.right  = rect.left + pCtrlDef->SZDLG_X;
   rect.bottom = rect.top  + pCtrlDef->SZDLG_Y;
   CalculateMinimumScale( rect );
// ZoomDiagram( 0, 3 );
}

void
ZER_Diagram::CreateZ( )
{
   if ( mIs_hWnd( m_hWnd ) ||
        CreateEx( WS_EX_CLIENTEDGE, m_csClassName /*"ZeidonCustomCtrl"*/, *m_pzsText,
                  Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                  m_pWndParent->m_hWnd, (HMENU) m_nIdNbr ) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
}

int
ZER_Diagram::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = (int) Default( );
/*y*/if ( nRC != -1 )
/*y*/   GetDeviceContext( );

   return( nRC );
}

ZER_Diagram::~ZER_Diagram( )
{
// TraceLineS( "ZER_Diagram::~ZER_Diagram", "" );

   ZER_Entity *pEntity;
   ZER_Relationship *pRelationship;

   while ( m_nRelCnt )
   {
      m_nRelCnt--;
      pRelationship = m_RelationshipList[ m_nRelCnt ];
      m_RelationshipList.RemoveAt( m_nRelCnt );
      delete( pRelationship );
   }

   while ( m_nEntCnt )
   {
      m_nEntCnt--;
      pEntity = m_EntityList[ m_nEntCnt ];
      m_EntityList.RemoveAt( m_nEntCnt );
      delete( pEntity );
   }

   if ( m_hGrid )
      SysFreeMemory( m_hGrid );

   ReleaseDeviceContext( );
   DestroyWindow( );
}

void
ZER_Diagram::DrawDiagram( )
{
   zSHORT   nPos;
   ZER_Entity *pEntity;
   ZER_Relationship *pRelationship;
   zBOOL    bDelete;
   zBOOL    bDisplayMsgBox = FALSE;
   zSHORT   nCnt = 0;

   if ( m_hDC == 0 )
   {
      GetDeviceContext( );
      bDelete = TRUE;
   }
   else
      bDelete = FALSE;

   for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
   {
      pEntity = m_EntityList[ nPos ];
      pEntity->DrawEntity( );
   }

   for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
   {
      pRelationship = m_RelationshipList[ nPos ];
      if ( pRelationship->DrawRelationship( FALSE, bDisplayMsgBox, nCnt ) < 0 )
      {
         bDisplayMsgBox = FALSE;
         nCnt++;
      }
   }

   if ( bDelete )
      ReleaseDeviceContext( );

// DisplayCells( "DrawDiagram", 3, 9, 32, 55 );
}

zSHORT
ZER_Diagram::ZoomDiagram( zSHORT nPercent, zSHORT nOptions )
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
      if ( m_lMaxPosX || m_lMaxPosY )
      {
         CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY,
                               rect, lNewScale );
         while ( lDisplaySizeX > (m_lMaxPosX + tzERENTITYX ) && lDisplaySizeY > (m_lMaxPosY + tzERENTITYY ) )
         {
            lNewScale += 10;
            CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY, rect, lNewScale );
         }

         while ( lDisplaySizeX < (m_lMaxPosX + tzERENTITYX) || lDisplaySizeY < (m_lMaxPosY + tzERENTITYY) )
         {
            if ( lNewScale == -99 )
               break;

            if ( lNewScale > -95 )
               lNewScale -= 5;
            else
               lNewScale = -99;

            CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY, rect, lNewScale );
         }

         if ( lDisplaySizeX > (m_lMaxPosX + tzERENTITYX) && lDisplaySizeY > (m_lMaxPosY + tzERENTITYY) )
         {
            while ( lDisplaySizeX > (m_lMaxPosX + tzERENTITYX) && lDisplaySizeY > (m_lMaxPosY + tzERENTITYY) )
            {
               lNewScale++;
               CalculateDisplaySize( lDisplaySizeX, lDisplaySizeY, rect, lNewScale );
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
            m_lDisplayPos -= tzERDIAGRAMX;
            lMoveY--;
         }
      }
      else
      {
         while ( (PosX( m_lDisplayPos ) < (tzERDIAGRAMX - 1)) && lMoveX )
         {
            m_lDisplayPos++;
            lMoveX++;
         }

         while ( (PosY( m_lDisplayPos ) < (tzERDIAGRAMY - 1)) && lMoveY )
         {
            m_lDisplayPos++;
            lMoveY++;
         }
      }

      while ( PosX( m_lDisplayPos ) && (PosX( m_lDisplayPos ) + lDisplaySizeX) >= tzERDIAGRAMX )
      {
         m_lDisplayPos--;
      }

      while ( PosY( m_lDisplayPos ) && (PosY( m_lDisplayPos ) + lDisplaySizeY) >= tzERDIAGRAMY )
      {
         m_lDisplayPos -= tzERDIAGRAMX;
      }
   }

   SetDisplayBoundaries( rect );
   SetScrollBars( );
   Invalidate( TRUE );
   m_bScrolledWindow = FALSE;
   return( 0 );
}

zSHORT
ZER_Diagram::ExpandContractDiagram( zSHORT nPercent, zUSHORT uOptions )
{
   if ( nPercent == 0 )
      return( 0 );

   if ( uOptions & 1 )     // Contracting diagram
   {
      if ( nPercent > 0 )
         nPercent = -nPercent;
   }
   else                    // Expanding diagram
   {
      if ( nPercent < 0 )
         nPercent = -nPercent;
   }

   ZER_Entity *pEntity;
   zSHORT nPosX;
   zSHORT nPosY;

   int nPos = m_nEntCnt;
   while ( nPos-- )
   {
      pEntity = m_EntityList[ nPos ];
      nPosX = pEntity->PosX( );
      nPosY = pEntity->PosY( );
      nPosX += (zSHORT) (((zLONG) nPosX * nPercent) / 100);
      nPosY += (zSHORT) (((zLONG) nPosY * nPercent) / 100);
      pEntity->Move( nPosX, nPosY, FALSE );
   }

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
ZER_Diagram::PrintDiagram( zSHORT nPercent, zSHORT nOptions )
{
   CWaitCursor wait;

   zLONG    lDisplayPosHold;
   zLONG    lDisplayPosMaxHold;
   zLONG    lCellSizeHold;
   zLONG    lScaleHold;
   zSHORT   nRC = zRESPONSE_YES;

   // Set operation count timer
   OperationCount( m_pZSubtask->m_vDialog, 1 );

   // Save all the current window values
   lDisplayPosHold    = m_lDisplayPos;
   lDisplayPosMaxHold = m_lDisplayPosMax;
   lCellSizeHold      = m_lCellSize;
   lScaleHold         = m_lScale;

   m_bShadeEnts = (nOptions & zPRINT_SHADESELE);
   m_bShadeRels = (nOptions & zPRINT_SHADESELR);

   // Set up the default Scaling and cellsize information
   m_lScale = 0;

   // Now we determine the print cell size
   m_lCellSize = tzERPRINTCELLSIZE;

   // Get maximum position of diagram
   SetMaxPositions( );

   // Get a dc to the default printer
   CDC *pPrintDC = new CDC( );
   CPrintDialog dlg( FALSE );
   dlg.GetDefaults( );
   if ( pPrintDC->Attach( dlg.GetPrinterDC( ) ) )
   {
      ZER_Entity *pEntity;
      ZER_Relationship *pRelationship;
      zCHAR    szMessage[ 256 ];
      zPCHAR   pData = 0;
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
      zLONG    lEntitiesPrinted, lRelationshipsPrinted;
      zLONG    lEntitiesNotPrinted, lRelationshipsNotPrinted;
      CPoint   ptPageSize;
      CPoint   ptPageOffset;
      CPoint   ptPageSizeWork;
      zLONG    lPos;

      // Get the physical page size of the printer
//    ptPageSize.x  = pPrintDC->GetDeviceCaps( HORZRES );  //??? which one ???
//    ptPageSize.y  = pPrintDC->GetDeviceCaps( VERTRES );
      ptPageSize.x = pPrintDC->GetDeviceCaps( PHYSICALWIDTH );
      ptPageSize.y = pPrintDC->GetDeviceCaps( PHYSICALHEIGHT );
      ptPageOffset.x = pPrintDC->GetDeviceCaps( PHYSICALOFFSETX );
      ptPageOffset.y = pPrintDC->GetDeviceCaps( PHYSICALOFFSETY );
      ptPageSizeWork.x = ptPageSize.x - (2 * ptPageOffset.x);
      ptPageSizeWork.y = ptPageSize.y - (2 * ptPageOffset.y);

      // Now that we have all the printer information, do page analysis
      // if an orientation was not selected AND we're not forcing the
      // diagram to fit on one page
      if ( (nOptions & (zPRINT_PAGE | zPRINT_FILLPAGE | zPRINT_PORTRAIT | zPRINT_LANDSCAPE)) == 0 )
      {
         // If we're picking the orientation, select the orientation
         // which produces the fewest pages
         // Determine the scaling necessary to get the diagram on the page
         ptDiagramSize.x = (m_lMaxPosX + tzERENTITYX) * tzERCELLSIZE;
         ptDiagramSize.y = (m_lMaxPosY + tzERENTITYY) * tzERCELLSIZE;

         // First Analyze the pages for portrait printing
         lPages = AnalyzePages( lHorizPages, m_lScale, lPageSizeX, lPageSizeY,
                                nPercent, nOptions | zPRINT_PORTRAIT,
                                ptDiagramSize, ptPageSizeWork );

         // Next flipflop the orientation and analyze the landscape printing
         lPos = ptPageSizeWork.x;
         ptPageSizeWork.x = ptPageSizeWork.y;
         ptPageSizeWork.y = lPos;
         lPage  = AnalyzePages( lHorizPages, m_lScale, lPageSizeX, lPageSizeY,
                                nPercent, nOptions | zPRINT_LANDSCAPE,
                                ptDiagramSize, ptPageSizeWork );

         // Reset the page size information back to portrait
         lPos = ptPageSizeWork.x;
         ptPageSizeWork.x = ptPageSizeWork.y;
         ptPageSizeWork.y = lPos;

         // Now, if one is better than the other, update nOptions with
         // the appropriate orientation before setting the printer info
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
           ((nOptions & zPRINT_PORTRAIT) == 0 && (m_lMaxPosX + tzHDNODEX) > (m_lMaxPosY + tzHDNODEY)) )
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

         // Determine the scaling necessary to get the diagram on the page
         ptDiagramSize.x = (m_lMaxPosX + (tzERENTITYX - 1)) * tzERCELLSIZE;
         ptDiagramSize.y = (m_lMaxPosY + (tzERENTITYY - 1)) * tzERCELLSIZE;

         // If we're going to print the diagram on one page, determine the
         // scaling factor required to shrink the diagram onto the page.
         if ( nOptions == 0 || (nOptions & (zPRINT_PAGE | zPRINT_FILLPAGE)) )
         {
            lPageSizeX = tzERDIAGRAMX;
            lPageSizeY = tzERDIAGRAMY;
            if ( ptDiagramSize.x > ptPageSizeWork.x || ptDiagramSize.y > ptPageSizeWork.y )
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
               ptDiagramSize.x = (m_lMaxPosX + m_lMinPosX + tzERENTITYX) * tzERCELLSIZE;
               ptDiagramSize.y = (m_lMaxPosY + m_lMinPosY + tzERENTITYY) * tzERCELLSIZE;
               nOptions |= zPRINT_PERCENT;
               nPercent = 110;
               while ( (lPages =  AnalyzePages( lHorizPages, m_lScale, lPageSizeX, lPageSizeY,
                                                nPercent, nOptions, ptDiagramSize, ptPageSizeWork )) == 1 )
               {
                  nPercent += 10;
               }

               while ( (lPages = AnalyzePages( lHorizPages, m_lScale, lPageSizeX, lPageSizeY,
                                               nPercent, nOptions, ptDiagramSize, ptPageSizeWork ) ) > 1 )
               {
                  nPercent -= 5;
               }

               while ( (lPages = AnalyzePages( lHorizPages, m_lScale, lPageSizeX, lPageSizeY,
                                               nPercent, nOptions, ptDiagramSize, ptPageSizeWork ) ) == 1 )
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
            lPages = AnalyzePages( lHorizPages, m_lScale, lPageSizeX, lPageSizeY,
                                   nPercent, nOptions, ptDiagramSize, ptPageSizeWork );
         }

         strcpy_s( szMessage, zsizeof( szMessage ), "Print Analysis\r\n\r\n" );
         strcat_s( szMessage, zsizeof( szMessage ), " Size: " );
         _ltoa_s( m_lScale + 100, szMessage + 240, zsizeof( szMessage ) - 240, 10 );
         strcat_s( szMessage, zsizeof( szMessage ), szMessage + 240 );
         strcat_s( szMessage, zsizeof( szMessage ), "%\r\n" );
         strcat_s( szMessage, zsizeof( szMessage ), " Orientation: " );
         if ( nOptions & zPRINT_PORTRAIT )
            strcat_s( szMessage, zsizeof( szMessage ), "Portrait\r\n" );
         else
            strcat_s( szMessage, zsizeof( szMessage ), "Landscape\r\n" );

         strcat_s( szMessage, zsizeof( szMessage ), " Pages: " );
         _ltoa_s( lPages, szMessage + 240, zsizeof( szMessage ) - 240, 10 );
         strcat_s( szMessage, zsizeof( szMessage ), szMessage + 240 );
         strcat_s( szMessage, zsizeof( szMessage ), "\r\n\r\n" );
         strcat_s( szMessage, zsizeof( szMessage ), "Continue with printing?" );

         // Reset the drawing stuff so a window paint can occur
         // while we are prompting the operator
         zLONG lScalePrint = m_lScale;
         m_lScale = lScaleHold;
         m_lCellSize = lCellSizeHold;
         nRC = OperatorPrompt( m_pZSubtask->m_vDialog, "ER Print", szMessage,
                               TRUE, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
         m_lScale = lScalePrint;
         m_lCellSize = tzERPRINTCELLSIZE;

         // If we succeed in starting the print, tie up the machine for
         // awhile printing the diagram.
         DOCINFO DocInfo;

         zmemset( &DocInfo, 0, sizeof( DOCINFO ) );
         DocInfo.cbSize = sizeof( DOCINFO );
         DocInfo.lpszDocName = "LOD Print";
         DocInfo.lpszOutput = 0;

         if ( nRC == zRESPONSE_YES && pPrintDC->StartDoc( &DocInfo ) > 0 )
         {
            m_bPrintDC = TRUE;
            m_lDisplayPos = 0;
            m_lDisplayPosMax = lPageSizeY * tzERDIAGRAMX;
            m_lDisplayPosMax += lPageSizeX;
            if ( m_lDisplayPosMax > tzERMAXGRID )
               m_lDisplayPosMax = tzERMAXGRID;

            lHorizWork = lHorizPages;
            lPrinted = 0;
            lPage = 1;
            for ( ;; )
            {
               TraceLineI( "(tzctlerd) Printing page #", lPage );
               pPrintDC->StartPage( );

               // If the diagram is to be scaled, scale the device context
               // and let windows handle the scaling through GDI
               if ( m_lScale )
               {
                  nOldMapMode = pPrintDC->SetMapMode( MM_ISOTROPIC );
                  sizeOldWExt = pPrintDC->SetWindowExt( CSize( 100, 100 ) );
                  sizeOldVExt = pPrintDC->SetViewportExt( CSize( 100 + m_lScale, 100 + m_lScale ) );
               }

               lEntitiesPrinted = 0;
               lEntitiesNotPrinted = 0;
               lRelationshipsPrinted = 0;
               lRelationshipsNotPrinted = 0;
               for ( lPos = 0; lPos < m_nEntCnt; lPos++ )
               {
                  pEntity = m_EntityList[ lPos ];
                  if ( pEntity->DrawEntity( ) )
                  {
                     lEntitiesPrinted++;
                     lPrinted++;
                  }
                  else
                     lEntitiesNotPrinted++;
               }

               for ( lPos = 0; lPos < m_nRelCnt; lPos++ )
               {
                  pRelationship = m_RelationshipList[ lPos ];
                  if ( pRelationship->DrawRelationship( ) > 0 )
                  {
                     lRelationshipsPrinted++;
                     lPrinted++;
                  }
                  else
                     lRelationshipsNotPrinted++;
               }

               TraceLineI( "              Entities printed = ", lEntitiesPrinted );
               TraceLineI( "          Entities not printed = ", lEntitiesNotPrinted );
               TraceLineI( "         Relationships printed = ", lRelationshipsPrinted );
               TraceLineI( "     Relationships not printed = ", lRelationshipsNotPrinted );

               // Do not print out a page if nothing was printed.
               if ( lPrinted )
               {
                  if ( lPage > 1 )
                  {
                     _ltoa_s( lPage, szMessage, zsizeof( szMessage ), 10 );
                     lPos = zstrlen( szMessage );
                  // szMessage[ lPos++ ] = ' ';
                  // szMessage[ lPos ] = 0;     don't need to null terminate
                  }
                  else
                  {
                     zVIEW    vDiagram;
                     zPCHAR   pchPrintTitle;
                     zPCHAR   pchPrintDesc;
                     zSHORT   k;

                     if ( GetViewByName( &vDiagram, "TZEREMDO", m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
                     {
                        GetAddrForAttribute( &pchPrintTitle, vDiagram, "EntpER_Model", "Name" );
                        GetAddrForAttribute( &pchPrintDesc, vDiagram, "EntpER_Model", "Desc" );
                     }
                     else
                     {
                        pchPrintTitle = 0;
                        pchPrintDesc = 0;
                     }

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
                  }

                  if ( lPos )
                     pPrintDC->TextOut( 0, 0, szMessage, zstrlen( szMessage ) );
               }

               if ( m_lScale )
               {
                  pPrintDC->SetMapMode( nOldMapMode );
                  pPrintDC->SetViewportExt( CSize( sizeOldWExt ) );
                  pPrintDC->SetViewportExt( CSize( sizeOldVExt ) );
               }

               pPrintDC->EndPage( );

               // We've printed a page, move on to the next
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
                  m_lDisplayPos += (zLONG) (lPageSizeY - 2) * tzERDIAGRAMX;
                  lHorizWork = lHorizPages;
               }

               m_lDisplayPosMax += (zLONG) lPageSizeY * tzERDIAGRAMX;
               m_lDisplayPosMax += lPageSizeX;
               if ( m_lDisplayPosMax > tzERMAXGRID )
                  m_lDisplayPosMax = tzERMAXGRID;
            }

            pPrintDC->EndDoc( );
         }

         m_hDC = hSaveDC;
         mDeleteInit( pPrintDC );
         m_bPrintDC = FALSE;
         Invalidate( TRUE );
      }
   }
   else
   {
      TraceLineS( "(tzctlerd) Error getting Default printer", "" );
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
ZER_Diagram::AnalyzePages( zLONG& lHorizPages, zLONG& lScale,
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

   // Now inflate or deflate the diagram size based on the scale provided
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
      lPageSizeX = tzERDIAGRAMX;
      lPageSizeY = tzERDIAGRAMY;
      return( lPages );
   }

   // We've determined that the diagram will not fit on one page,
   // calculate the number of pages and the page sizes in cells
   dWork = (double) ptPageSize.x;
   dWork /= ptDiagramSize.x;
   dWork *= (m_lMaxPosX + (tzERENTITYX - 1));
   dWork++;
   lPageSizeX = (zLONG) dWork;

   dWork = (double) ptPageSize.y;
   dWork /= ptDiagramSize.y;
   dWork *= (m_lMaxPosY + (tzERENTITYY - 1));
   dWork++;
   lPageSizeY = (zLONG) dWork;

   // Now analyze the number of pages in either direction
   lPagesX = 1;
   lSizeWk = lPageSizeX - 1;
   while ( lSizeWk < (m_lMaxPosX + (tzERENTITYX - 1)) )
   {
      lPagesX++;
      lSizeWk += (lPageSizeX - 2);
   }

   lPagesY = 1;
   lSizeWk = lPageSizeY - 1;
   while ( lSizeWk < (m_lMaxPosY + (tzERENTITYY - 1)) )
   {
      lPagesY++;
      lSizeWk += (lPageSizeY - 2);
   }

   lHorizPages = lPagesX;
   lPages = lPagesX * lPagesY;

   return( lPages );
}

void
ZER_Diagram::CalculateMinimumScale( CRect& rect )
{
   zLONG lSizeX, lSizeY;
   zLONG lScale;

   lScale   = -80;
   CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
   while ( lSizeX < tzERDIAGRAMX || lSizeY < tzERDIAGRAMY )
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

   while ( lSizeX > tzERDIAGRAMX && lSizeY > tzERDIAGRAMY )
   {
      lScale += 5;
      CalculateDisplaySize( lSizeX, lSizeY, rect, lScale );
   }

   if ( lScale > -99 )
   {
      while ( lSizeX < tzERDIAGRAMX || lSizeY < tzERDIAGRAMY )
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
ZER_Diagram::CalculateDisplaySize( zLONG& lDisplaySizeX,
                                   zLONG& lDisplaySizeY,
                                   CRect&  rect,
                                   zLONG   lScale )
{
   zLONG lRectX, lRectY;

// TraceLineI( "CalculateDisplaySize lScale = ", lScale );

   // Get the logical rect size with scaling
   lRectX = rect.right  - rect.left;
   lRectY = rect.bottom - rect.top;
   if ( lScale )
   {
      lRectX = (lRectX * 100) / (100 + lScale);
      lRectY = (lRectY * 100) / (100 + lScale);
   }

   // Calculate the new display size
   lDisplaySizeX = (zSHORT) (lRectX / tzERCELLSIZE);
   if ( lRectX % tzERCELLSIZE )
      lDisplaySizeX++;

   lDisplaySizeY = (zSHORT) (lRectY / tzERCELLSIZE);
   if ( lRectY % tzERCELLSIZE )
      lDisplaySizeY++;
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
ZER_Diagram::SetDisplayBoundaries( CRect& rect, zBOOL bUpdate )
{
   zLONG lRectX;
   zLONG lRectY;
   zLONG lTop;
   zLONG lLeft;

   if ( bUpdate )
   {
      // Now set the update region in the class from the rect to be painted
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

      m_lUpdateDisplayPos = ((zLONG) PosY( m_lDisplayPos ) * tzERDIAGRAMX) +
                            ((zLONG) (lTop / tzERCELLSIZE) * tzERDIAGRAMX);
      m_lUpdateDisplayPos += PosX( m_lDisplayPos ) + (lLeft / tzERCELLSIZE);
      m_lUpdateDisplayPosMax = m_lUpdateDisplayPos;
      if ( lRectX >= (tzERCELLSIZE * tzERDIAGRAMX) )
         lRectX = (tzERCELLSIZE * (tzERDIAGRAMX - 1) );

      m_lUpdateDisplayPosMax += (zLONG)
         ((lRectY / tzERCELLSIZE) * tzERDIAGRAMX) + (lRectX / tzERCELLSIZE);

      // Always increase update area to ensure painting
      if ( PosX( m_lUpdateDisplayPos ) )
         m_lUpdateDisplayPos--;

      if ( PosY( m_lUpdateDisplayPos ) )
         m_lUpdateDisplayPos -= tzERDIAGRAMX;

      m_lUpdateDisplayPosMax += tzERDIAGRAMX;
      if ( PosX( m_lUpdateDisplayPosMax ) < (tzERDIAGRAMX - 1) )
         m_lUpdateDisplayPosMax++;

      if ( m_lUpdateDisplayPosMax > tzERMAXGRID )
         m_lUpdateDisplayPosMax = tzERMAXGRID;
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

      if ( lRectX >= (tzERCELLSIZE * tzERDIAGRAMX) )
         lRectX = (tzERCELLSIZE * (tzERDIAGRAMX - 1));

      m_lDisplayPosMax = m_lDisplayPos +
          ((lRectY / tzERCELLSIZE) * tzERDIAGRAMX) + (lRectX / tzERCELLSIZE);

      if ( lRectY % tzERCELLSIZE )
         m_lDisplayPosMax += tzERDIAGRAMX;

      if ( lRectX % tzERCELLSIZE )
         m_lDisplayPosMax++;

      if ( m_lDisplayPosMax > tzERMAXGRID )
         m_lDisplayPosMax = tzERMAXGRID;
   }
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
ZER_Diagram::MakeLogicalPoint( CPoint& pt )
{
   zLONG lPosY;
   zLONG lPosX;

   // If the diagram is scaled, increase or decrease the position based
   // on the scale
   if ( m_lScale )
   {
      CPoint ptOrig = pt;
      CPoint ptCheck;
      lPosX = pt.x;
      lPosY = pt.y;

      while ( TRUE )
      {
         pt.x = (zSHORT) ((lPosX * 100) / (100 + m_lScale));
         pt.y = (zSHORT) ((lPosY * 100) / (100 + m_lScale));
         ptCheck = pt;
         MakePhysicalLParam( ptCheck );
         if ( ptCheck.x >= ptOrig.x && ptCheck.y >= ptOrig.y )
            break;

         if ( ptCheck.x < lPosX )
            lPosX++;

         if ( ptCheck.y < lPosY )
            lPosY++;
      }
   }
}


//
//  METHOD: MakePhysicalLParam
//
//  PURPOSE: This method converts a logical point, converts it back to
//           a physical point and then turns it into an LParam. This
//           method is used for line drawing and erasing during mouse move
//           messages.
//
void
ZER_Diagram::MakePhysicalLParam( CPoint& pt )
{
   zLONG  lPosY;
   zLONG  lPosX;

   // If the diagram is scaled, increase or decrease the position based
   // on the scale
   if ( m_lScale )
   {
      lPosX = pt.x;
      lPosY = pt.y;
      pt.x = (zSHORT) ((lPosX * (100 + m_lScale)) / 100);
      pt.y = (zSHORT) ((lPosY * (100 + m_lScale)) / 100);
   }
}

//
//  METHOD: MousePositionToIndex
//
//  PURPOSE: This method converts a physical mouse position to the
//           appropriate index position on the grid for entity and
//           relationship hit determination.
//
zLONG
ZER_Diagram::MousePositionToIndex( CPoint pt )
{
   zLONG  lIdx;
   zLONG  lPosY;
   zLONG  lPosX;

   // If the diagram is scaled, increase or decrease the position based
   // on the scale
   lPosX = pt.x;
   lPosY = pt.y;
   if ( lPosX < 0 )
      lPosX = 0;

   if ( lPosY < 0 )
      lPosY = 0;

   if ( m_lScale )
   {
      lPosX = (lPosX * 100) / (100 + m_lScale);
      lPosY = (lPosY * 100) / (100 + m_lScale);
   }

   lPosX /= tzERCELLSIZE;
   lPosY /= tzERCELLSIZE;
   if ( lPosX >= tzERDIAGRAMX )
      lPosX = (tzERDIAGRAMX - 1);

   if ( lPosY >= tzERDIAGRAMY )
      lPosY = (tzERDIAGRAMY - 1);

   lIdx = ((lPosY * tzERDIAGRAMX) + lPosX);
   lIdx += m_lDisplayPos;
   return( lIdx );
}


//
//  METHOD:  IndexOnEntity
//
//  PURPOSE: This method takes a grid pointer and returns the entity
//           (if any) which the grid position represents.
//
ZER_Entity *
ZER_Diagram::IndexOnEntity( zLONG lIdx )
{
   zSHORT nPos;
   ZER_Entity *pEntity;

   if ( lIdx <= tzERMAXGRID && (m_pchGrid[ lIdx ] & tziENTITY) )
   {
      while ( lIdx > -1 && (m_pchGrid[ lIdx ] & tziENTITY) )
         lIdx--;

      lIdx++;
      while ( lIdx > 0 && (m_pchGrid[ lIdx ] & tziENTITY) )
         lIdx -= tzERDIAGRAMX;

      lIdx += tzERDIAGRAMX;
      for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
      {
         pEntity = m_EntityList[ nPos ];
         if ( pEntity->m_lPosIdx == lIdx )
         {
            break;
         }
      }

      return( pEntity );
   }

   return( 0 );
}

//
//  METHOD:  CountEntitiesAtIndex
//
//  PURPOSE: This method takes a grid pointer and returns the number
//           of entities which the grid position represents.
//
zSHORT
ZER_Diagram::CountEntitiesAtIndex( zLONG lIdx )
{
   zSHORT nEntityCnt = 0;

   if ( lIdx <= tzERMAXGRID && (m_pchGrid[ lIdx ] & tziENTITY) )
   {
      ZER_Entity *pEntity;
      zSHORT     nPos;
      zLONG      lEndPos;

      for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
      {
         pEntity = m_EntityList[ nPos ];
         lEndPos = pEntity->m_lPosIdx + (pEntity->m_lEntPortX * pEntity->m_lEntPortY);
         if ( pEntity->m_lPosIdx <= lIdx && lEndPos >= lIdx )
            nEntityCnt++;
      }
   }

   if ( nEntityCnt > 1 )
      TraceLineI( "Entities at index ", nEntityCnt );

   return( nEntityCnt );
}

//
//  METHOD:  IndexOnRelationship
//
//  PURPOSE: This method takes a grid pointer and returns the relationship
//           (if any) which the grid position represents.
//
ZER_Relationship *
ZER_Diagram::IndexOnRelationship( zLONG lIdx )
{
   zCHAR  cSetBits;
   zSHORT nPos, nOffset;
   ZER_Relationship *pRelationship;

   if ( lIdx <= tzERMAXGRID && m_bShowLines )
   {
      // if we've selected somthing belonging to a relationship
      // then find the relationship to which the something belongs
      if ( m_pchGrid[ lIdx ] & (tziLINE | tziARC | tziTEXT) )
      {
         // if we are not positioned on a line for the relationship,
         // find the line associated with the text
         if ( ( m_pchGrid[ lIdx ] & (tziLINE | tziARC)) == 0 )
         {
            cSetBits = (tziLINE | tziTEXT);
            if ( m_pchGrid[ lIdx ] & tziTEXTVERT )
            {
               cSetBits |= tziTEXTVERT;
               nOffset = 1;
            }
            else
            {
               cSetBits |= tziTEXTHORIZ;
               nOffset = tzERDIAGRAMX;
            }

            // Now find the line associated with the text
            if ( (lIdx - nOffset) < 0 ||
                 ((m_pchGrid[ lIdx - nOffset ] & cSetBits) != cSetBits) )
            {
               lIdx += nOffset;
            }
            else
               lIdx -= nOffset;
         }

         // We're on the relationship line, now traverse it until we
         // find an arc or an entity
         if ( m_pchGrid[ lIdx ] & tziLINE )
         {
            if ( m_pchGrid[ lIdx ] & tziVERT )
               nOffset = tzERDIAGRAMX;
            else
               nOffset = 1;

            while ( (m_pchGrid[ lIdx ] & (tziARC | tziENTITY )) == 0 )
               lIdx += nOffset;

            if ( m_pchGrid[ lIdx ] & tziENTITY )
               lIdx -= nOffset;
         }

         // We've found the source entity position, now look up the entity
         // in the entity list
         for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
         {
            pRelationship = m_RelationshipList[ nPos ];
            if ( pRelationship->m_lStartIdx == lIdx ||
                 pRelationship->m_lArcIdx1  == lIdx ||
                 pRelationship->m_lArcIdx2  == lIdx ||
                 pRelationship->m_lArcIdx3  == lIdx ||
                 pRelationship->m_lEndIdx   == lIdx )
            {
               break;
            }
         }

         if ( nPos < m_nRelCnt )
            return( pRelationship );
      }
   }

   return( 0 );
}


//
//  METHOD:  PositionOnZeidonEntity
//
//  PURPOSE: This method takes a C++ Entity object and positions on the
//           corresponding entity in the Zeidon object.
//
zBOOL
ZER_Diagram::
PositionOnZeidonEntity( ZER_Entity *pEntity,
                        zVIEW      vDiagram,
                        zBOOL      bSelect,
                        zBOOL      bCtrl )
{
   if ( vDiagram == 0 )
   {
      GetViewByName( &vDiagram, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
   }

   if ( vDiagram &&
        SetCursorFirstEntityByInteger( vDiagram, *m_pzsEName, szlZKey, pEntity->m_lPosZKey, 0 ) >= zCURSOR_SET )
   {
      if ( bCtrl == FALSE )
         SetAllSelStatesForEntityForSet( vDiagram, *m_pzsEName, 0, 1, 0 );
      else
      if ( bSelect && GetSelectStateOfEntityForSet( vDiagram, *m_pzsEName, 1 ) )
      {
         bSelect = FALSE;
         SetSelectStateOfEntityForSet( vDiagram, *m_pzsEName, 0, 1 );
      }

      if ( bSelect )
         SetSelectStateOfEntityForSet( vDiagram, *m_pzsEName, 1, 1 );
      return( 1 );
   }

   return( 0 );
}

//
//  METHOD:  PositionOnZeidonRelationship
//
//  PURPOSE: This method takes a C++ Relationship object and positions on the
//           corresponding relationship in the Zeidon object.
//
zBOOL
ZER_Diagram::
PositionOnZeidonRelationship( ZER_Relationship *pRelationship,
                              zVIEW       vDiagram,
                              zBOOL       bSelect,
                              zBOOL       bCtrl )
{
   if ( vDiagram == 0 )
   {
      GetViewByName( &vDiagram, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
   }
   if ( vDiagram &&
        SetCursorFirstEntityByInteger( vDiagram, *m_pzsAName, szlZKey, pRelationship->m_lPosZKey, 0 ) >= zCURSOR_SET )
   {
      if ( bCtrl == FALSE )
         SetAllSelStatesForEntityForSet( vDiagram, *m_pzsAName, 0, 1, 0 );
      else
      if ( bSelect && GetSelectStateOfEntityForSet( vDiagram, *m_pzsAName, 1 ) )
      {
         bSelect = 0;
         SetSelectStateOfEntityForSet( vDiagram, *m_pzsAName, 0, 1 );
      }

      if ( bSelect )
         SetSelectStateOfEntityForSet( vDiagram, *m_pzsAName, 1, 1 );

      return( 1 );
   }

   return( 0 );
}


//
//  METHOD:  PositionVisible
//
//  PURPOSE: This method takes two grid positions and determines if
//           the rectangle they represent is visible in the window.
//
zBOOL
ZER_Diagram::PositionVisible( zLONG lStartIdx, zLONG lEndIdx )
{
   zSHORT nPos1, nPos2, nPos3, nPos4;

   if ( m_lUpdateDisplayPos == -1 )
   {
//    if ( bPrintDC )
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

   // position is visible
   return( 1 );
}


//
//  METHOD:  AnalyzeGrid
//
//  PURPOSE: This method gives debugging info about the grid.
//
void
ZER_Diagram::AnalyzeGrid( )
{
#if 0
   zLONG    lIdx;
   zLONG    lEntityCells;
   zLONG    lLineCells;
   zLONG    lArcCells;
   zLONG    lTextCells;
   zLONG    lErrorCells;
   zLONG    lCrossedLines;
   zLONG    lErrorRels;
   zSHORT   nPos;
   ZER_Relationship *pRelationship;

   TraceLineI( "(tzctlerd) Grid Analysis - Entities = ", m_nEntCnt );
   TraceLineI( "  Relationships = ", m_nRelCnt );

   lEntityCells = 0;
   lLineCells = 0;
   lArcCells = 0;
   lTextCells = 0;
   lErrorCells = 0;
   lCrossedLines = 0;
   lErrorRels = 0;

   for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
   {
      pRelationship = m_RelationshipList[ nPos ];
      if ( pRelationship->m_lStartIdx == 0 )
         lErrorRels++;
   }

   for ( lIdx = 0; lIdx <= tzERMAXGRID; lIdx++ )
   {
      if ( m_pchGrid[ lIdx ] )
      {
         if ( m_pchGrid[ lIdx ] & tziENTITY )
         {
            lEntityCells++;
         }

         if ( m_pchGrid[ lIdx ] & tziLINE )
         {
            lLineCells++;
            if ( (m_pchGrid[ lIdx ] & (tziVERT | tziHORIZ)) == (tziVERT | tziHORIZ) )
            {
               lLineCells++;
               lCrossedLines++;
            }
         }

         if ( m_pchGrid[ lIdx ] & tziARC )
            lArcCells++;

         if ( m_pchGrid[ lIdx ] & tziTEXT )
            lTextCells++;

         if ( (m_pchGrid[ lIdx ] &
               (tziENTITY | tziLINE | tziARC | tziTEXT)) == 0 )
         {
            lErrorCells++;
         }
      }
   }

TraceLineI("                      Entity Cells  = ", lEntityCells );
if ( lEntities )
{
TraceLineI("                Entity Cell average = ",
                                         lEntityCells / m_nEntCnt );
}
TraceLineI("                      Line Cells    = ", lLineCells );
TraceLineI("                    Crossed Lines   = ", lCrossedLines );
if ( lRelationships )
{
TraceLineI("                 Avg. Line Length   = ",
                                             lLineCells / m_nRelCnt );
}
TraceLineI("                      Arc  Cells    = ", lArcCells );
TraceLineI("                      Text Cells    = ", lTextCells );
TraceLineI("           Undrawable Relationships = ", lErrorRels );
TraceLineI("                      Error Cells   = ", lErrorCells );
#endif
}


//
//  METHOD:  GetDeviceContext
//
//  PURPOSE: This method retrieves the device context for the diagram
//           and scales it to the current scale
//
void
ZER_Diagram::GetDeviceContext( )
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

// TraceLineI( "ZER_Diagram::GetDeviceContext m_hDC: ", (zLONG) m_hDC );
}

//
//  METHOD:  ReleaseDeviceContext
//
//  PURPOSE: This method releases the device context for the diagram.
//
void
ZER_Diagram::ReleaseDeviceContext( )
{
// TraceLineI( "ZER_Diagram::ReleaseDeviceContext m_hDC: ", (zLONG) m_hDC );
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

void
ZER_Diagram::RedrawDiagram( )
{
#if 1
   zSHORT   nPos;
   ZER_Relationship *pRelationship;

   Invalidate( TRUE );

   for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
   {
      pRelationship = m_RelationshipList[ nPos ];
      pRelationship->m_bLineDrawn = FALSE;
      pRelationship->m_bTextDrawn = FALSE;
   }
#else
   CRect    rect;
   HBRUSH   hBrush;
   HPEN     hPen;
   HGDIOBJ  hOldBrush;
   HGDIOBJ  hOldPen;
   zBOOL    bDelete;

   bDelete = 0;
   if ( m_hDC == 0 )
   {
      GetDeviceContext( );
      bDelete = 1;
   }

   hBrush = CreateSolidBrush( tzERCOLORBACKGROUND );
   hPen   = CreatePen( PS_SOLID, 1, tzERCOLORBACKGROUND );
   hOldPen = m_hDC->SelectObject( (HGDIOBJ) hPen );
   hOldBrush = m_hDC->SelectObject( (HGDIOBJ) hBrush );
   GetClientRect( rect );
   m_hDC->Rectangle( rect );
   m_hDC->SelectObject( hOldBrush );
   m_hDC->SelectObject( hOldPen );
   DeleteObject( hBrush );
   DeleteObject( hPen );
   DrawDiagram( );

   if ( bDelete )
      ReleaseDeviceContext( );
#endif
}

void
ZER_Diagram::DrawDiagramCell( zLONG lIdx,
                              zBOOL bErase,
                              ZER_Relationship *pRelationship )
{
   // If the cell is not in the display, exit.
   if ( PositionVisible( lIdx, lIdx ) == 0 )
      return;

   if ( bErase )
      ColorCell( lIdx, tzERCOLORBACKGROUND );

   if ( m_pchGrid[ lIdx ] == 0 )
      return;   // no lines to draw

   if ( (m_pchGrid[ lIdx ] & tziENTITY) == 0 )
   {
      if ( m_bShowLines == FALSE )
         return;

      zBOOL bDelete = 0;
      if ( m_hDC == 0 )
      {
         GetDeviceContext( );
         bDelete = 1;
      }

      // If the cell is a line terminator which abuts an entity, draw
      // the cardinality values for the entity.
      if ( m_pchGrid[ lIdx ] & (tziTEXTHORIZ | tziTEXTVERT) && (m_pchGrid[ lIdx ] & tziTEXT) == 0 )
      {
         zSHORT nPos;
         zBOOL  bDirection;
         CPoint pt1;
         CPoint pt2;
         CRect  rect;
         zSHORT nWidth;
         zBOOL  bReq, bMany;

         nPos = 0;
         if ( pRelationship == 0 )
         {
            for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
            {
               pRelationship = m_RelationshipList[ nPos ];
               if ( pRelationship->m_lStartIdx == lIdx || pRelationship->m_lEndIdx == lIdx )
               {
                  break;
               }
            }
         }

         if ( nPos < m_nRelCnt )
         {
            // Now determine the direction of the terminator
            if ( m_pchGrid[ lIdx ] & tziTEXTHORIZ )
            {
               bDirection = tzLEFT;
               if ( m_pchGrid[ lIdx + 1 ] & tziENTITY )
                  bDirection = tzRIGHT;
            }
            else
            {
               bDirection = tzUP;
               if ( m_pchGrid[ lIdx + tzERDIAGRAMX ] & tziENTITY )
                  bDirection = tzDOWN;
            }

            // Note that in determining which cardinality to draw,
            // the Start Index goes with the target entity and
            // the End Index goes with the source entity. THIS IS
            // INTENTIONAL!!!
            if ( lIdx == pRelationship->m_lStartIdx )
            {
               bReq  = pRelationship->m_bTgtReq;
               bMany = pRelationship->m_bTgtMany;
            }
            else
            {
               bReq  = pRelationship->m_bSrcReq;
               bMany = pRelationship->m_bSrcMany;
            }

            // Now that we have the direction, draw the terminator.
            // First draw the required, optional indication.
            pt1.x = (PosX( lIdx ) - PosX( m_lDisplayPos )) * tzERCELLSIZE;
            pt1.y = (PosY( lIdx ) - PosY( m_lDisplayPos )) * tzERCELLSIZE;
            nWidth = (zSHORT) ((tzERCELLSIZE + 1) / 2);
            pt1.x += (tzERCELLSIZE / 2);
            pt1.y += (tzERCELLSIZE / 2);
            m_hDC->MoveTo( pt1.x, pt1.y );

            // Point1 is sitting in the center of the cell
            if ( bReq )
            {
               if ( bDirection == tzLEFT )
               {
                  pt2.x = pt1.x + (tzERCELLSIZE + 1) / 2;
                  pt2.y = pt1.y;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.x -= (nWidth + 1) / 2;
                  pt2.y = pt1.y - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y);
                  pt2.y = pt1.y + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               if ( bDirection == tzRIGHT )
               {
                  pt2.x = pt1.x - (tzERCELLSIZE + 1) / 2;
                  pt2.y = pt1.y;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.x += (nWidth + 1) / 2;
                  pt2.y = pt1.y - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.y = pt1.y + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               if ( bDirection == tzUP )
               {
                  pt2.y = pt1.y + (tzERCELLSIZE + 1) / 2;
                  pt2.x = pt1.x;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.y -= (nWidth + 1) / 2;
                  pt2.x = pt1.x - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.x = pt1.x + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else    // DOWN
               {
                  pt2.y = pt1.y - (tzERCELLSIZE + 1) / 2;
                  pt2.x = pt1.x;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.y += (nWidth + 1) / 2;
                  pt2.x = pt1.x - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.x = pt1.x + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
            }
            else
            {
               // Draw a circle to represent minimum cardinality of 0
               if ( bDirection == tzLEFT )
               {
                  rect.left = pt1.x;
                  rect.top  = pt1.y - (nWidth / 2);
               }
               else
               if ( bDirection == tzRIGHT )
               {
                  rect.left = pt1.x - (tzERCELLSIZE / 2);
                  rect.top  = pt1.y - (nWidth / 2);
               }
               else
               if ( bDirection == tzUP )
               {
                  rect.left = pt1.x - (nWidth / 2);
                  rect.top  = pt1.y;
               }
               else
               {
                  rect.left = pt1.x - (nWidth / 2);
                  rect.top  = pt1.y - (tzERCELLSIZE / 2);
               }

               rect.right  = rect.left + (nWidth - 1);
               rect.bottom = rect.top  + (nWidth - 1);
               CBrush *pBrushOld = (CBrush *) m_hDC->SelectStockObject( NULL_BRUSH );
               m_hDC->SelectStockObject( NULL_BRUSH );
               m_hDC->Ellipse( rect.left, rect.top, rect.right, rect.bottom );
               m_hDC->SelectObject( pBrushOld );
            }

            // Now that we've drawn the required indicator, draw the 1 or many indicator.
            m_hDC->MoveTo( pt1.x, pt1.y );
            if ( bMany )
            {
               if ( bDirection == tzLEFT )
               {
                  pt2.x = pt1.x - nWidth;
                  pt2.y = pt1.y - (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.y = pt1.y;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.y = pt1.y + (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               if ( bDirection == tzRIGHT )
               {
                  pt2.x = pt1.x + nWidth;
                  pt2.y = pt1.y - (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.y = pt1.y;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.y = pt1.y + (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               if ( bDirection == tzUP )
               {
                  pt2.y = pt1.y - nWidth;
                  pt2.x = pt1.x - (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.x = pt1.x;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.x = pt1.x + (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               {
                  pt2.y = pt1.y + nWidth;
                  pt2.x = pt1.x - (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.x = pt1.x;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  m_hDC->MoveTo( pt1.x, pt1.y );
                  pt2.x = pt1.x + (nWidth - 1);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
            }
            else
            {
               // bMany is false
               if ( bDirection == tzLEFT )
               {
                  pt2.x = pt1.x - (tzERCELLSIZE + 1) / 2;
                  pt2.y = pt1.y;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.x += ( nWidth + 1 ) / 2;
                  pt2.y = pt1.y - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.y = pt1.y + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               if ( bDirection == tzRIGHT )
               {
                  pt2.x = pt1.x + (tzERCELLSIZE + 1) / 2;
                  pt2.y = pt1.y;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.x -= (nWidth + 1) / 2;
                  pt2.y = pt1.y - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.y = pt1.y + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else
               if ( bDirection == tzUP )
               {
                  pt2.y = pt1.y - (tzERCELLSIZE + 1) / 2;
                  pt2.x = pt1.x;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.y += (nWidth + 1) / 2;
                  pt2.x = pt1.x - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.x = pt1.x + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
               else    // DOWN
               {
                  pt2.y = pt1.y + (tzERCELLSIZE + 1) / 2;
                  pt2.x = pt1.x;
                  m_hDC->LineTo( pt2.x, pt2.y );
                  pt2.y -= (nWidth + 1) / 2;
                  pt2.x = pt1.x - (nWidth / 2);
                  m_hDC->MoveTo( pt2.x, pt2.y );
                  pt2.x = pt1.x + (nWidth / 2);
                  m_hDC->LineTo( pt2.x, pt2.y );
               }
            }

            return;   // return if success, else fall into line
         }
      }

      if ( m_pchGrid[ lIdx ] & tziLINE )
      {
         CPoint pt1;

         pt1.x = (PosX( lIdx ) - PosX( m_lDisplayPos )) * tzERCELLSIZE;
         pt1.y = (PosY( lIdx ) - PosY( m_lDisplayPos )) * tzERCELLSIZE;
         if ( m_pchGrid[ lIdx ] & tziHORIZ )
         {
            pt1.y += (tzERCELLSIZE / 2);
            m_hDC->MoveTo( pt1.x, pt1.y );
            pt1.x += tzERCELLSIZE;
         }
         else
         {
            pt1.x += (tzERCELLSIZE / 2);
            m_hDC->MoveTo( pt1.x, pt1.y );
            pt1.y += tzERCELLSIZE;
         }

         m_hDC->LineTo( pt1.x, pt1.y );
      }
      else
      if ( m_pchGrid[ lIdx ] & tziARC )
      {
         CRect  rect;
         CPoint pt1;
         CPoint pt2;

         // We need to actually DRAW AN ARC, to do this we construct a rect
         // with the arc quadrants corner as the middle point in the rect.
         rect.top  = (PosY( lIdx ) - PosY( m_lDisplayPos )) * tzERCELLSIZE;
         rect.left = (PosX( lIdx ) - PosX( m_lDisplayPos )) * tzERCELLSIZE;

         // Now our rect position is in the upper left corner of the CELL
         if ( m_pchGrid[ lIdx ] & tziULEFT )
         {
            rect.left--;   // Step out of cell to middle of Arc Rect
            rect.top--;
            pt1.x = rect.left;
            pt1.y = rect.top +  ((tzERCELLSIZE / 2) + 1);
            pt2.x = rect.left + ((tzERCELLSIZE / 2) + 1);
            pt2.y = rect.top;
         }
         else
         if ( m_pchGrid[ lIdx ] & tziURIGHT )
         {
            rect.left += (tzERCELLSIZE + 1);
            rect.top--;
            pt1.x = rect.left - ((tzERCELLSIZE / 2) + 1);
            pt1.y = rect.top;
            pt2.x = rect.left;
            pt2.y = rect.top + ((tzERCELLSIZE / 2) + 1 );
         }
         else
         if ( m_pchGrid[ lIdx ] & tziBLEFT )
         {
            rect.top += (tzERCELLSIZE + 1);
            rect.left--;
            pt1.x = rect.left + ((tzERCELLSIZE / 2) + 1 );
            pt1.y = rect.top;
            pt2.x = rect.left;
            pt2.y = rect.top - ((tzERCELLSIZE / 2) + 1 );
         }
         else
         {
            rect.left += (tzERCELLSIZE + 1);        // Lower Right Arc
            rect.top  += (tzERCELLSIZE + 1);
            pt1.x = rect.left;
            pt1.y = rect.top - ((tzERCELLSIZE / 2) + 1 );
            pt2.x = rect.left - ((tzERCELLSIZE / 2) + 1 );
            pt2.y = rect.top;
         }

         // Now set the rect positions by moving the left and top positions
         // from the middle of the rectangle to the upper left corner.
         rect.left  -= ((tzERCELLSIZE / 2) + 2);
         rect.top   -= ((tzERCELLSIZE / 2) + 2);
         rect.right  = rect.left + (tzERCELLSIZE + 3);
         rect.bottom = rect.top  + (tzERCELLSIZE + 3);

         m_hDC->Arc( rect.left, rect.top, rect.right, rect.bottom,
                     pt1.x, pt1.y, pt2.x, pt2.y );
      }

      if ( bDelete )
         ReleaseDeviceContext( );
   }
}

void
ZER_Diagram::DisplayCells( zCPCHAR cpcText,
                           zLONG lX1, zLONG lY1, zLONG lX2, zLONG lY2 )
{
#ifdef DEBUG_ALL
   zCHAR  szMsg[ 512 ];
   zCHAR  szVal[ 4 ];
   zLONG  k;
   zLONG  lIdx;
   zLONG  lLth;
   zUCHAR chValue;

   TraceLineS( cpcText, " Begin" );
   if ( m_pchGrid )
   {
      szVal[ 2 ] = 0;
      while ( lX1 <= lX2 )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), "Row %3ld:", lX1 );
         lLth = zstrlen( szMsg );
         lIdx = (lX1 * tzERDIAGRAMX) + lY1;
         for ( k = 0; k <= lY2 - lY1; k++ )
         {
            if ( k && (k % 100) == 0 )
            {
               TraceLineS( szMsg, "" );
               sprintf_s( szMsg, zsizeof( szMsg ), "=>> %3ld:", lX1 );
               lLth = zstrlen( szMsg );
            }

            chValue = m_pchGrid[ lIdx ];
            szVal[ 1 ] = chValue % 16;
            if ( szVal[ 1 ] < 10 )
               szVal[ 1 ] += '0';
            else
               szVal[ 1 ] += 'A' - 10;

            chValue /= 16;
            szVal[ 0 ] = chValue % 16;
            if ( szVal[ 0 ] < 10 )
               szVal[ 0 ] += '0';
            else
               szVal[ 0 ] += 'A' - 10;

            if ( szVal[ 1 ] == '(' || szVal[ 1 ] == '*' || szVal[ 1 ] == '"' ||
                 szVal[ 1 ] == '$' || szVal[ 0 ] == '+' || szVal[ 1 ] == ')' )
            {
               szVal[ 3 ] = 0;
            }

            sprintf_s( szMsg + lLth, " %02s", szVal );
            lLth = zstrlen( szMsg );
            lIdx++;
         }

         TraceLineS( szMsg, "" );
         lX1++;
      }
   }

   TraceLineS( cpcText, " End" );

#endif
}

void
ZER_Diagram::ColorCell( zLONG lIdx, COLORREF color )
{
   if ( PositionVisible( lIdx, lIdx ) == 0 )
      return;

   CRect  rect;
   CPen   *pPen = new CPen( PS_SOLID, 1, color );
   CBrush *pBrush = new CBrush( color );

   CPen *pPenOld = m_hDC->SelectObject( pPen );
   CBrush *pBrushOld = m_hDC->SelectObject( pBrush );

   rect.left   = ((PosX( lIdx ) - PosX( tzERDISPLAYPOS )) * tzERCELLSIZE);
   rect.top    = ((PosY( lIdx ) - PosY( tzERDISPLAYPOS )) * tzERCELLSIZE);
   rect.right  = rect.left + tzERCELLSIZE;
   rect.bottom = rect.top  + tzERCELLSIZE;
   rect.left--;
   rect.top--;
   m_hDC->Rectangle( rect );
   m_hDC->SelectObject( pBrushOld );
   m_hDC->SelectObject( pPenOld );
   delete( pBrush );
   delete( pPen );
}

zSHORT
ZER_Diagram::PosX( zLONG lIdx )
{
   return( (zSHORT) (lIdx % tzERDIAGRAMX) );
}

zSHORT
ZER_Diagram::PosY( zLONG lIdx )
{
   return( (zSHORT) (lIdx / tzERDIAGRAMX) );
}

void
ZER_Diagram::SetScrollBars( )
{
   if ( (PosY( m_lDisplayPosMax ) - PosY( m_lDisplayPos )) < (tzERDIAGRAMY - 1) )
   {
      SetScrollRange( SB_VERT, 0, tzERDIAGRAMY - (((PosY( m_lDisplayPosMax ) - PosY( m_lDisplayPos )) + 1)), FALSE );
      SetScrollPos( SB_VERT, PosY( m_lDisplayPos ), TRUE );
   }
   else
      SetScrollRange( SB_VERT, 0, 0, TRUE );

   if ( (PosX( m_lDisplayPosMax ) - PosX( m_lDisplayPos )) < (tzERDIAGRAMX - 1) )
   {
      SetScrollRange( SB_HORZ, 0, tzERDIAGRAMX - (((PosX( m_lDisplayPosMax ) - PosX( m_lDisplayPos )) + 1)), FALSE );
      SetScrollPos( SB_HORZ, PosX( m_lDisplayPos ), TRUE );
   }
   else
      SetScrollRange( SB_HORZ, 0, 0, TRUE );
}

void
ZER_Diagram::SetMaxPositions( )
{
   zSHORT nPos;
   ZER_Entity * pEntity;

   m_lMinPosX = 0;
   m_lMaxPosX = 0;
   m_lMinPosY = 0;
   m_lMaxPosY = 0;
   if ( m_nEntCnt == 0 )
   {
      return;
   }
   else
   {
      m_lMinPosX = 32767;
      m_lMinPosY = 32767;
      for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
      {
         pEntity = m_EntityList[ nPos ];
         if ( pEntity->PosX( ) < m_lMinPosX )
            m_lMinPosX = pEntity->PosX( );

         if ( pEntity->PosX( ) > m_lMaxPosX )
            m_lMaxPosX = pEntity->PosX( );

         if ( pEntity->PosY( ) < m_lMinPosY )
            m_lMinPosY = pEntity->PosY( );

         if ( pEntity->PosY( ) > m_lMaxPosY )
            m_lMaxPosY = pEntity->PosY( );
      }
   }
}

//./ ADD NAME=IsDoubleClick
// Source Module=tzctlerd.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: IsDoubleClick
//
//  PURPOSE:   To determine if the current click is part of a double click.
//
//  RETURNS:    TRUE  - current click is part of a double-click
//              FALSE - current click is not part of a double-click ...
//                      OR the specified window was not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL
ZER_Diagram::IsDoubleClick( zUSHORT uMessage, CPoint ptStart )
{
   MSG     msg;
   zULONG  ulInterval = 0;
   zLONG   lMessage = GetMessageTime( );
   zULONG  ulDblclkTime = GetDoubleClickTime( );

#ifdef DEBUG_ALL
   TraceLineI( "ZER_Diagram::IsDoubleClick Time: ", ulDblclkTime );
   TraceLineI( "                    MessageTime: ", lMessage );
#endif

   while ( ulInterval < ulDblclkTime )
   {
      ulInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage, PM_NOREMOVE | PM_NOYIELD ) )
      {
      // TraceLineI( "ZER_Diagram::IsDoubleClick TRUE: ", ulInterval );
         return( TRUE );
      }
      else
      if ( ::PeekMessage( &msg, m_hWnd, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE | PM_NOYIELD ) )
      {
         CPoint pt( msg.lParam );
         if ( ptStart.x - pt.x > g_lDblClkX || pt.x - ptStart.x > g_lDblClkX ||
              ptStart.y - pt.y > g_lDblClkY || pt.y - ptStart.y > g_lDblClkY )
         {
         // TracePoint( "ZER_Diagram::IsDoubleClick TRUE: ", pt );
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

void
ZER_Diagram::AddRelationship( ZER_Relationship *pNewRelationship )
{
   m_RelationshipList.SetAtGrow( m_nRelCnt++, pNewRelationship );

   if ( pNewRelationship->m_pSrcEntity == pNewRelationship->m_pTgtEntity )
   {
      // put self (recursive) relationship first in the list for the entity
      ZER_Relationship *pZER_Relationship;

      int nPos = m_nRelCnt - 1;
      while ( nPos-- )
      {
         pZER_Relationship = m_RelationshipList[ nPos ];
         if ( pZER_Relationship->m_pSrcEntity == pNewRelationship->m_pSrcEntity ||
              pZER_Relationship->m_pTgtEntity == pNewRelationship->m_pTgtEntity )
         {
            m_RelationshipList[ nPos ] = pNewRelationship;
            m_RelationshipList[ m_nRelCnt - 1 ] = pZER_Relationship;
            break;
         }
      }
   }
}

// Standard driver control methods
zSHORT
ZER_Diagram::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZER_Diagram::MapFromOI ", *m_pzsTag );
   DisplayCells( "ZER_Diagram::MapFromOI prior", 3, 9, 32, 55 );
#endif

   zLONG      lPosZKey, lZKey;
   zLONG      lPortFactor;
   zLONG      lPosX, lPosY;
   zLONG      lFlags = 0;
   zLONG      lStartIdx, lEndIdx, lArcIdx;
   zLONG      lSrcZKey, lTgtZKey;
   zVIEW      vDiagram;
   zSHORT     nPos, nCsrRC;
   zSHORT     nRels, nCalcRels;
   zPCHAR     pchText;
   zPCHAR     pchType;
   zLONG      lAlloc;
   zPCHAR     pchWork;
   zCHAR      szCard[ 10 ];
   zCHAR      szSrcText[ 60 ];
   zCHAR      szTgtText[ 60 ];
   zBOOL      bSrcReq, bSrcMany, bTgtReq, bTgtMany;
   zBOOL      bOldUpdateView;
   zBOOL      bSelect, bRelError;
   ZER_Entity *pZER_Entity;
   ZER_Entity *pSrcEntity;
   ZER_Entity *pTgtEntity;
   ZER_Relationship *pZER_Relationship;


   if ( (m_ulMapActFlags & zMAPACT_VISIBLE) == 0 ||
        (m_ulMapActFlag2 & zMAPACT_CREATED) == 0 ||
        (m_ulMapActFlags & zMAPACT_NOAUTOMAP_FROM_OI) )
   {
      return( 0 );
   }

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;

   bOldUpdateView = m_bUpdateView;
   if ( GetViewByName( &vDiagram, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      m_bUpdateView = (MiGetUpdateForView( vDiagram ) > 0) ? TRUE : FALSE;
   }
   else
   {
      m_bUpdateView = TRUE;
      vDiagram = 0;
   }

   // If the view is not updateable, gray lines, boxes and text
   if ( m_bUpdateView )
   {
      m_clrBox     = m_clrBoxU;
      m_clrLines = m_clrLinesU;
      m_clrText    = m_clrTextU;
   }
   else
   {
      m_clrBox     = m_clrNoUpdate;
      m_clrLines = m_clrNoUpdate;
      m_clrText    = m_clrNoUpdate;
   }

   // If the entity and attribute lists are instantiated, empty them NOW
   if ( m_nEntCnt )
   {
      if ( m_bMultMap == FALSE )
      {
         if ( bOldUpdateView != m_bUpdateView )
            Invalidate( TRUE );

         return( 0 );
      }

      while ( m_nRelCnt )
      {
         m_nRelCnt--;
         pZER_Relationship = m_RelationshipList[ m_nRelCnt ];
         m_RelationshipList.RemoveAt( m_nRelCnt );
         if ( pZER_Relationship != m_pPendingRelationship )
            delete( pZER_Relationship );
      }

      while ( m_nEntCnt )
      {
         m_nEntCnt--;
         pZER_Entity = m_EntityList[ m_nEntCnt ];
         m_EntityList.RemoveAt( m_nEntCnt );
         if ( pZER_Entity != m_pPendingEntity )
            delete( pZER_Entity );
      }

      // Zero out the grid
   // TraceLineS( "ZER_Diagram::MapFromOI NULLING OUT GRID!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", "" );
      pchWork = m_pchGrid;
      lAlloc = ((zLONG) tzERDIAGRAMX * (zLONG) tzERDIAGRAMY);

      while ( lAlloc > 16384 )
      {
         zmemset( pchWork, 0, 16384 );
         lAlloc -= 16384;
         pchWork += 16384;
      }

      if ( lAlloc )
         zmemset( pchWork, 0, (int) lAlloc );
   }

   nRels = 0;
   nCalcRels = 0;
   if ( vDiagram )
   {
      zSHORT nRC;

      CreateViewFromViewForTask( &vDiagram, vDiagram, m_pZSubtask->m_vDialog );
      nRC = SetCursorFirstEntity( vDiagram, *m_pzsEName, 0 );
      if ( nRC == zCURSOR_SET )
      {
         while ( nRC == zCURSOR_SET )
         {
            bSelect = (zBOOL) GetSelectStateOfEntityForSet( vDiagram, *m_pzsEName, 1 );
            GetAddrForAttribute( &pchText, vDiagram, szlER_Entity, szlName );
            GetAddrForAttribute( &pchType, vDiagram, szlER_Entity, szlPurpose );
            GetIntegerFromAttribute( &lPosZKey, vDiagram, *m_pzsEName, szlZKey );
            GetIntegerFromAttribute( &lZKey, vDiagram, szlER_Entity, szlZKey );
            GetIntegerFromAttribute( &lPosX, vDiagram,*m_pzsEName, szlER_DiagramPosX );
            GetIntegerFromAttribute( &lPosY, vDiagram, *m_pzsEName, szlER_DiagramPosY );
            GetIntegerFromAttribute( &lPortFactor, vDiagram, *m_pzsEName, szlER_PortFactor );
            if ( lPosX || lPosY )
            {
               pZER_Entity = new ZER_Entity( this, lPosX, lPosY, pchText, pchType[ 0 ],
                                             lPosZKey, lZKey, lPortFactor, bSelect );
               m_EntityList.SetAtGrow( m_nEntCnt++, pZER_Entity );
            }

            nRC = SetCursorNextEntity( vDiagram, *m_pzsEName, 0 );
         }

         nRC = SetCursorFirstEntity( vDiagram, *m_pzsAName, 0 );
         while ( nRC == zCURSOR_SET )
         {
            bRelError = 0;
            nRels++;
            bSelect = (zBOOL) GetSelectStateOfEntityForSet( vDiagram, *m_pzsAName, 1 );
            GetIntegerFromAttribute( &lPosZKey, vDiagram, *m_pzsAName, szlZKey );
            GetStringFromAttribute( szSrcText, zsizeof( szSrcText ), vDiagram, szlER_RelLink_2, szlName );
            strcat_s( szSrcText, szlLPAREN );
            GetStringFromAttribute( szCard, zsizeof( szCard ), vDiagram, szlER_RelLink_2, szlCardMin );
            strcat_s( szSrcText, szCard );
            if ( atoi( szCard ) == 1 )
               bSrcReq = 1;
            else
               bSrcReq = 0;

            strcat_s( szSrcText, szlCOMMA );
            GetStringFromAttribute( szCard, zsizeof( szCard ), vDiagram, szlER_RelLink_2, szlCardMax );
            strcat_s( szSrcText, szCard );
            if ( atoi( szCard ) == 1 )
               bSrcMany = 0;
            else
               bSrcMany = 1;

            strcat_s( szSrcText, szlRPAREN );
            if ( CheckExistenceOfEntity( vDiagram, szlER_Entity_2 ) == zCURSOR_NULL )
            {
               bRelError = 1;
               lSrcZKey = 0;
               TraceLineS("(tzerctda) - ERROR, Missing ER_Entity_2","");
               DisplayObjectInstance( vDiagram, szlER_RelType, 0 );
            }
            else
            {
               GetIntegerFromAttribute( &lSrcZKey, vDiagram, szlER_Entity_2, szlZKey );
            }

            nCsrRC = SetCursorNextEntity( vDiagram, szlER_RelLink_2, 0 );
            GetStringFromAttribute( szTgtText, zsizeof( szTgtText ), vDiagram, szlER_RelLink_2, szlName );
            strcat_s( szTgtText, szlLPAREN );
            GetStringFromAttribute( szCard, zsizeof( szCard ), vDiagram, szlER_RelLink_2, szlCardMin );
            strcat_s( szTgtText, szCard );
            if ( atoi( szCard ) == 1 )
               bTgtReq = 1;
            else
               bTgtReq = 0;

            strcat_s( szTgtText, szlCOMMA );
            GetStringFromAttribute( szCard, zsizeof( szCard ), vDiagram, szlER_RelLink_2, szlCardMax );
            strcat_s( szTgtText, szCard );
            if ( atoi( szCard ) == 1 )
               bTgtMany = 0;
            else
               bTgtMany = 1;

            strcat_s( szTgtText, szlRPAREN );
            if ( CheckExistenceOfEntity( vDiagram, szlER_Entity_2 ) == zCURSOR_NULL )
            {
               lTgtZKey = 0;
               if ( bRelError == 0 )
               {
                  bRelError = 1;
                  TraceLineS("(tzerctda) - ERROR, Missing ER_Entity_2","");
                  DisplayObjectInstance( vDiagram, szlER_RelType, 0 );
               }
            }
            else
            {
               GetIntegerFromAttribute( &lTgtZKey, vDiagram, szlER_Entity_2, szlZKey );
            }

            // If the relationship is in error, offer to delete the relationship from the object.
            if ( bRelError )
            {
               if ( OperatorPrompt( m_pZSubtask->m_vDialog,
                                    "ER Mapping Error",
                                    "A Relationship has been found "
                                    "without a source or target entity - "
                                    "Shall I Attempt to Delete it?", TRUE,
                                    zBUTTONS_YESNO, zRESPONSE_NO,
                                    0 ) == zRESPONSE_YES )
               {
                  if ( MiGetUpdateForView( vDiagram ) > 0 )
                     DeleteEntity( vDiagram, *m_pzsAName, zREPOS_NONE );
                  else
                  {
                     OperatorPrompt( m_pZSubtask->m_vDialog, "ER Relationship Delete",
                                     "The view is read-only!", TRUE, 0, 0, 0 );
                  }
               }
            }

            // Now find the source and target entities in the entity list
            pSrcEntity = 0;
            if ( lSrcZKey && nCsrRC == zCURSOR_SET )
            {
               for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
               {
                  pZER_Entity = m_EntityList[ nPos ];
                  if ( pZER_Entity->m_lZKey == lSrcZKey )
                  {
                     pSrcEntity = pZER_Entity;
                     break;
                  }
               }
            }

            if ( pSrcEntity )
            {
               pTgtEntity = 0;
               for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
               {
                  pZER_Entity = m_EntityList[ nPos ];
                  if ( pZER_Entity->m_lZKey == lTgtZKey )
                  {
                     pTgtEntity = pZER_Entity;
                     break;
                  }
               }
            }

            // If the source and target entitys of the relationship
            // were found, get the positioning information and
            // create the relationship
            if ( pSrcEntity && pTgtEntity )
            {
               // Initialize relationship positioning variables
               lStartIdx = 0;
               lEndIdx = 0;
               lArcIdx = 0;
               GetIntegerFromAttribute( &lPosX, vDiagram, *m_pzsAName, szlER_DiagramE1PosX );
               GetIntegerFromAttribute( &lPosY, vDiagram, *m_pzsAName, szlER_DiagramE1PosY );

               // If relationship positioning exists, gather the attributes
               if ( lPosX || lPosY )
               {
                  lStartIdx = (lPosY * tzERDIAGRAMX) + lPosX;
                  GetIntegerFromAttribute( &lPosX, vDiagram, *m_pzsAName, szlER_DiagramE2PosX );
                  GetIntegerFromAttribute( &lPosY, vDiagram, *m_pzsAName, szlER_DiagramE2PosY );
                  lEndIdx = (lPosY * tzERDIAGRAMX) + lPosX;
                  GetIntegerFromAttribute( &lPosX, vDiagram, *m_pzsAName, szlER_DiagramBendPosX );
                  GetIntegerFromAttribute( &lPosY, vDiagram, *m_pzsAName, szlER_DiagramBendPosY );
                  lArcIdx = (lPosY * tzERDIAGRAMX) + lPosX;
               // HH, 2000.01.17
               // GetIntegerFromAttribute( &lFlags, vDiagram, *m_pzsAName, "Flags" );
               }
               else
                  nCalcRels++;

               pZER_Relationship =
                  new ZER_Relationship( this, pSrcEntity, pTgtEntity,
                                        szSrcText, szTgtText, bSrcReq, bSrcMany,
                                        bTgtReq, bTgtMany, lPosZKey,
                                        lStartIdx, lEndIdx, lArcIdx, bSelect, 0 );
               AddRelationship( pZER_Relationship );
            }

            nRC = SetCursorNextEntity( vDiagram, *m_pzsAName, 0 );
         }

         // If all the relationships have been calculated, re-calculate
         // all the relationships to see if we can get any better results
         // with all the other relationships already marked
         if ( nRels && nRels == nCalcRels )
         {
            zSHORT nCnt = 0;

            for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
            {
               pZER_Relationship = m_RelationshipList[ nPos ];
               if ( pZER_Relationship->m_bLineDrawn )
                  pZER_Relationship->Erase( );
               else
                  pZER_Relationship->MarkRelationship( 1 );

               nCnt += pZER_Relationship->CalculateLine( FALSE, nCnt );
               pZER_Relationship->MarkRelationship( );
            }
         }
      }

      DropView( vDiagram );
   }

   if ( m_pPendingEntity )
   {
      m_EntityList.SetAtGrow( m_nEntCnt++, m_pPendingEntity );
      m_pPendingEntity->MarkEntity( );
   }

   if ( m_pPendingRelationship )
   {
      AddRelationship( m_pPendingRelationship );
      m_pPendingRelationship->MarkRelationship( );
   }

   // Invalidate the diagram to force its drawing
   Invalidate( TRUE );
// DisplayCells( "ZER_Diagram::MapFromOI post", 3, 9, 32, 55 );
   return( 0 );
}

zSHORT
ZER_Diagram::MapToOI( zLONG /* lFlag */ )
{
   zVIEW          vDiagram;
   zSHORT         nPos;
   ZER_Entity     *pZER_Entity;
   ZER_Relationship *pZER_Relationship;

   if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 || (m_ulMapActFlags & zMAPACT_VISIBLE) == 0 ||
        (m_ulMapActFlag2 & zMAPACT_CREATED) == 0 || (m_ulMapActFlags & zMAPACT_NOAUTOMAP_TO_OI) )
   {
      return( 0 );
   }

// DisplayCells( "ZER_Diagram::MapToOI prior", 3, 9, 32, 55 );

   if ( GetViewByName( &vDiagram, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 &&
        MiGetUpdateForView( vDiagram ) > 0 )
   {
      CreateViewFromViewForTask( &vDiagram, vDiagram, m_pZSubtask->m_vDialog );
      for ( nPos = 0; nPos < m_nEntCnt; nPos++ )
      {
         pZER_Entity = m_EntityList[ nPos ];
         if ( pZER_Entity->m_bUpdated )
         {
            if ( SetCursorFirstEntityByInteger( vDiagram, *m_pzsEName, szlZKey,
                                                pZER_Entity->m_lPosZKey, 0 ) == zCURSOR_SET )
            {
               SetAttributeFromInteger( vDiagram, *m_pzsEName, szlER_DiagramPosX, pZER_Entity->PosX( ) );
               SetAttributeFromInteger( vDiagram, *m_pzsEName, szlER_DiagramPosY, pZER_Entity->PosY( ) );
            }

            pZER_Entity->m_bUpdated = FALSE;
         }
      }

      for ( nPos = 0; nPos < m_nRelCnt; nPos++ )
      {
         pZER_Relationship = m_RelationshipList[ nPos ];
         if ( pZER_Relationship->m_bUpdated )
         {
            if ( SetCursorFirstEntityByInteger( vDiagram, *m_pzsAName, szlZKey,
                                                pZER_Relationship->m_lPosZKey, 0 ) == zCURSOR_SET )
            {
               SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramE1PosX,
                                        PosX( pZER_Relationship->m_lStartIdx ) );
               SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramE1PosY,
                                        PosY( pZER_Relationship->m_lStartIdx ) );
               SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramE2PosX,
                                        PosX( pZER_Relationship->m_lEndIdx ) );
               SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramE2PosY,
                                        PosY( pZER_Relationship->m_lEndIdx ) );
               if ( pZER_Relationship->m_pSrcEntity == pZER_Relationship->m_pTgtEntity )
               {
                  SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramBendPosX,
                                           PosX( pZER_Relationship->m_lArcIdx2 ) );
                  SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramBendPosY,
                                           PosY( pZER_Relationship->m_lArcIdx2 ) );
               }
               else
               {
                  SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramBendPosX,
                                           PosX( pZER_Relationship->m_lArcIdx1 ) );
                  SetAttributeFromInteger( vDiagram, *m_pzsAName, szlER_DiagramBendPosY,
                                           PosY( pZER_Relationship->m_lArcIdx1 ) );
               }
            }

            pZER_Relationship->m_bUpdated = FALSE;
         }
      }

      DropView( vDiagram );
   }

//DisplayCells( "ZER_Diagram::MapToOI post", 3, 9, 32, 55 );
  return( 0 );
}
