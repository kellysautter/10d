/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdoc.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZDrDoc.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.05.11    DKS    Z10    ActiveX
//    Single document for all views.
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

// #include "ZDrOle.h"

//IMPLEMENT_DYNAMIC(ZDrDoc, COleDocument)
IMPLEMENT_DYNCREATE(ZDrDoc, COleDocument)

BEGIN_MESSAGE_MAP(ZDrDoc, COleDocument)
   //{{AFX_MSG_MAP(ZDrDoc)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
   // Enable default OLE container implementation
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
   ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
   ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
   ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
   ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
   ON_UPDATE_COMMAND_UI(ID_OLE_VERB_FIRST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZDrDoc ctor

ZDrDoc::ZDrDoc( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrDoc::ZDrDoc DEFAULT ctor", "" );
#endif
   // Make sure that the document won't be destroyed when the view is
   // destroyed.  m_bAutoDelete is a public, but non-documented member
   // of CDocument.
   m_bAutoDelete = FALSE;

   ZDrApp *pApp = (ZDrApp *) AfxGetApp( );
   if ( pApp->m_pDoc == 0 )
      pApp->m_pDoc = this;
   else
      ::MessageBox( 0, "Twice Doc'd", "ZDrDoc::ctor", MB_OK );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrDoc dtor
ZDrDoc::~ZDrDoc( )
{
#ifdef DEBUG_ALL
   TraceLineX( "ZDrDoc::dtor", (zLONG) this );
#endif

   // Added to support TB ActiveX Editor - 1998/4/18
// mDeleteInit( m_pDocTemplate );   // Clean up our Workaround
   // Added to support TB ActiveX Editor - 1998/4/18
}

BOOL
ZDrDoc::OnNewDocument( )
{
   if ( COleDocument::OnNewDocument( ) == 0 )
      return( FALSE );

#if 0
   // TODO: add reinitialization code here
   // (SDI documents will reuse this document)

   // Added to support TB ActiveX Editor - 1998/4/18
   // This is a Workaround for the MergeMenus Problem.  The ActiveX Editor
   // Control wants to bring up its own Menu in Merge of the existing
   // Menu from the Zeidon Application.  To do that with the MFC we need a
   // CDocTemplate Object.  The Problem is Zeidon doesn't need the entire
   // Document-View Concept, so the Developers of Zeidon made some
   // modifications and don't create the CSingleDocTemplate.
   CSingleDocTemplate *pDocTemplate =
        new CSingleDocTemplate( IDR_MAINFRAME,
                                RUNTIME_CLASS( ZDrDoc ),
                                RUNTIME_CLASS( ZDrFrame ), // main SDI frame
                                RUNTIME_CLASS( ZDrView ) );
   if ( pDocTemplate == 0 )
      return( FALSE );

   pDocTemplate->SetContainerInfo( IDR_CNTR_INPLACE );
   pDocTemplate->AddDocument( this );
   m_pDocTemplate = pDocTemplate;
   // Added to support TB ActiveX Editor - 1998/4/18
#endif

   return( TRUE );
}

// Added to support TB ActiveX Editor - 1998/4/18
void
ZDrDoc::SetDocTemplateMenu( HMENU hMenu )
{
   if ( m_pDocTemplate->m_hMenuInPlace )
      ::DestroyMenu( m_pDocTemplate->m_hMenuInPlace );

   m_pDocTemplate->m_hMenuInPlace = hMenu;
}
// Added to support TB ActiveX Editor - 1998/4/18

/////////////////////////////////////////////////////////////////////////////
// ZDrDoc serialization

void
ZDrDoc::Serialize( CArchive& ar )
{
   if ( ar.IsStoring( ) )
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }

   // Calling the base class COleDocument enables serialization
   // of the container document's COleClientItem objects.
   COleDocument::Serialize( ar );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrDoc diagnostics

#ifdef _DEBUG
void
ZDrDoc::AssertValid( ) const
{
   COleDocument::AssertValid( );
}

void
ZDrDoc::Dump( CDumpContext& dc ) const
{
   COleDocument::Dump( dc );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrDoc commands

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZParentData
//
zLONG ZParentData::g_lIdNbr = 1;
ZParentData::ZParentData( ZSplitterView *pParentSplitterView,
                          ZSplitterWnd  *pParentSplitterWnd,
                          ZSplitterView *pSplitterView,
                          zSHORT nRow, zSHORT nCol )
{
   m_ulIdNbr = g_lIdNbr++;

#ifdef DEBUG_ALL
   TraceLine( "ZParentData::ctor (0x%08x): %d", this, m_ulIdNbr );
#endif

   m_pParentSplitterView = pParentSplitterView;
   m_pParentSplitterWnd = pParentSplitterWnd;
   m_pSplitterView = pSplitterView;
   m_nRow = nRow;
   m_nCol = nCol;
}

ZParentData::~ZParentData( )
{
#ifdef DEBUG_ALL
   TraceLine( "ZParentData::dtor (0x%08x): %d", this, m_ulIdNbr );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZSplitterWnd
//
IMPLEMENT_DYNAMIC( ZSplitterWnd, CSplitterWnd )
BEGIN_MESSAGE_MAP( ZSplitterWnd, CSplitterWnd )
   //{{AFX_MSG_MAP(ZSplitterWnd)
   ON_WM_SETCURSOR( )
   ON_WM_CREATE( )
   ON_WM_SIZE( )
   ON_WM_CLOSE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

#define FOCUS_HILIGHT_COLOR_ULO RGB( 180, 75, 25 )
#define FOCUS_HILIGHT_COLOR_LRO RGB( 245, 5, 25 )
#define FOCUS_HILIGHT_COLOR_ULI RGB( 145, 95, 75 )
#define FOCUS_HILIGHT_COLOR_LRI RGB( 220, 65, 40 )

#define FOCUS_HILIGHT_SHOW TRUE

#define SPLITTER_CX   4
#define SPLITTER_CY   4
#define SPLITTER_GAPX 4
#define SPLITTER_GAPY 4

zLONG ZSplitterWnd::g_lIdNbr = 1;
ZSplitterWnd::ZSplitterWnd( ZSubtask *pZSubtask, zCPCHAR cpcSplitTag,
                            zSHORT nPercentX, zSHORT nPercentY,
                            zLONG lFlag )
{
   m_ulIdNbr = g_lIdNbr++;

#ifdef DEBUG_ALL
   TraceLine( "ZSplitterWnd::ctor (0x%08x): %d  %s  Subtask: %d  ZView: %d",
              this, m_ulIdNbr, cpcSplitTag, pZSubtask->m_ulIdNbr,
              pZSubtask->m_pZView->m_ulIdNbr );
#endif

   m_lNextViewId = 1;
   m_hWndAct = 0;
   m_bSplitterView[ 0 ][ 0 ] = FALSE;
   m_bSplitterView[ 0 ][ 1 ] = FALSE;
   m_bSplitterView[ 1 ][ 0 ] = FALSE;
   m_bSplitterView[ 1 ][ 1 ] = FALSE;

   m_nRow = 0;
   m_nCol = 0;

   m_pZTask = pZSubtask->m_pZTask;
   m_pZSubtask1 = pZSubtask;
   m_pZSubtask2 = 0;
   m_pZView = pZSubtask->m_pZView;
   m_pcsSplitTag = new CString( cpcSplitTag );
   m_nPercentX = nPercentX;
   m_nPercentY = nPercentY;
   m_lFlag = lFlag;  // 1 ==> right; 2 ==> down
   m_ulBaseSplitterPane = pZSubtask->m_pZTask->m_ulNextSplitterPane;
   pZSubtask->m_pZTask->m_ulNextSplitterPane += 4;

   m_nRows = (lFlag & 2) ? 2 : 1;
   m_nCols = (lFlag & 1) ? 2 : 1;

   m_pParentWnd = 0;

#if 0
   m_cxSplitter = SPLITTER_CX;
   m_cySplitter = SPLITTER_CY;
   m_cxSplitterGap = SPLITTER_GAPX;
   m_cySplitterGap = SPLITTER_GAPY;
#endif
}

ZSplitterWnd::~ZSplitterWnd( )
{
#ifdef DEBUG_ALL
   TraceLine( "ZSplitterWnd::dtor (0x%08x) %d: %s",
              this, m_ulIdNbr, *m_pcsSplitTag );
#endif

   m_pParentWnd = 0;
// if ( m_pZView )   // want to, but can't do this here
//    m_pZView->m_pParentSplitterWnd = 0;

// m_mapIdPaneViews.Cleanup( );
// m_mapCurrentViews.Cleanup( );
   mDeleteInit( m_pcsSplitTag );
}

int
ZSplitterWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CSplitterWnd::OnCreate( lpCreateStruct );
   if ( nRC != -1 )
   {
      m_pParentWnd = GetParent( );
   }

   return( nRC );
}

BOOL
ZSplitterWnd::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMsg )
{
   if ( m_pZTask->m_bDeletingSplitter ||
        m_pParentWnd == 0 ||
        mIs_hWnd( pWnd->m_hWnd ) == 0 )
   {
      return( TRUE );
   }

#if 0
   if ( mIs_hWnd( m_pParentWnd->m_hWnd ) == 0 )
   {
      TraceLineI( "ZSplitterWnd::OnSetCursor: ", m_ulIdNbr );
      CWnd *pWndParent = GetParent( );
      GetTopLevelFrame( );
   }
#endif

   return( CSplitterWnd::OnSetCursor( pWnd, uHitTest, uMsg ) );
}

void
ZSplitterWnd::OnSize( UINT uSizeType, int cx, int cy )
{
#ifdef DEBUG_ALL
   if ( m_pZSubtask1 &&
        (ZSubtask::GetSubtaskState( m_pZSubtask1 ) &
                                         zSUBTASK_STATE_DELETED) == 0 &&
        m_pZSubtask1->m_pzsWndTag )
   {
      TraceLineS( "ZSplitterWnd::OnSize tag1: ", *(m_pZSubtask1->m_pzsWndTag) );
   }

   if ( m_pZSubtask2 &&
        (ZSubtask::GetSubtaskState( m_pZSubtask2 ) &
                                         zSUBTASK_STATE_DELETED) == 0 &&
        m_pZSubtask2->m_pzsWndTag )
   {
      TraceLineS( "ZSplitterWnd::OnSize tag2: ", *(m_pZSubtask2->m_pzsWndTag) );
   }

   TraceLine( "ZSplitterWnd::OnSize  cx: %d  cy: %d", cx, cy );
#endif

   CSplitterWnd::OnSize( uSizeType, cx, cy );
}

/////////////////////////////////////////////////////////////////////////////
// ZSplitterWnd diagnostics
//
#ifdef _DEBUG
void
ZSplitterWnd::AssertValid( ) const
{
   CSplitterWnd::AssertValid( );
}

void
ZSplitterWnd::Dump( CDumpContext& dc ) const
{
   CSplitterWnd::Dump( dc );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void
ZSplitterWnd::RefreshSplitBars( )
{
   CRect rectInside;

   GetInsideRect( rectInside );
   DrawAllSplitBars( NULL, rectInside.right, rectInside.bottom );
}

void
ZSplitterWnd::OnDrawSplitter( CDC *pDC, ESplitType nType,
                              const CRect& rectArg )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSplitterWnd::OnDrawSplitter", "" );
#endif
#if 0
   if ( (FOCUS_HILIGHT_SHOW) && (nType == splitBorder) &&
        ((GetRowCount( ) > 1) || (GetColumnCount( ) > 1)) )
   {
      int nRow = 0;
      int nCol = 0;

      if ( rectArg.top )
         nRow = 1;

      if ( rectArg.left )
         nCol = 1;

      if ( (m_nRow == nRow) && (m_nCol == nCol) )
      {
         if ( pDC == NULL )
         {
            RedrawWindow( rectArg, NULL, RDW_INVALIDATE | RDW_NOCHILDREN );
            return;
         }

         ASSERT_VALID( pDC );
         CRect rect = rectArg;
         pDC->Draw3dRect( rect, FOCUS_HILIGHT_COLOR_ULO, FOCUS_HILIGHT_COLOR_LRO );
         rect.InflateRect( -GetSystemMetrics( SM_CXBORDER ),
                           -GetSystemMetrics( SM_CYBORDER ) );
         pDC->Draw3dRect( rect, FOCUS_HILIGHT_COLOR_ULI, FOCUS_HILIGHT_COLOR_LRI );
         return;
      }
   }

#endif
   CSplitterWnd::OnDrawSplitter( pDC, nType, rectArg );
}

#if 1
void
ZSplitterWnd::SetHighlight( )
{
  CWnd *pAct = GetActivePane( );
  if ( pAct && m_hWndAct != pAct->m_hWnd )
  {
     // Active pane changed.
     m_hWndAct = pAct->m_hWnd;

     CPaintDC dc( this );
     CRect rectInside;
     GetInsideRect( rectInside );

     // CSplitterWnd::GetInsideRect - undocumented public function
     // returns size of SplitterWnd minus size of scroll bars, if any.

     // CSplitterWnd::DrawAllSplitBars - undocumented public function
     // Called with DeviceContext, it draws the frames.
     // Called with DeviceContext = NULL, it does a RedrawWindow
     // to make the changes visible.

     DrawAllSplitBars( &dc, rectInside.right, rectInside.bottom );
     DrawAllSplitBars( NULL, rectInside.right, rectInside.bottom );

     // RedrawWindow( );  // generates too much flicker
  }
}

#else

void
ZSplitterWnd::SetHighlight( )
{
   CWnd *pAct = GetActivePane( );
   if ( pAct && m_hWndAct != pAct->m_hWnd )
   {
      m_hWndAct = pAct->m_hWnd;
      RedrawWindow( );
   }
}
#endif

void
ZSplitterWnd::SetActivePane( int nRow, int nCol, CWnd *pWnd /*= NULL*/ )
{
  CSplitterWnd::SetActivePane( nRow, nCol, pWnd );
  SetHighlight( );
}

zLONG
ZSplitterWnd::AddView( zSHORT nRow, zSHORT nCol,
                       CRuntimeClass *pViewClass,
                       CCreateContext *pContext,
                       CWnd *pNewView )
{
   // Hide the current view of the pane.
   zLONG lPreviousId = HideCurrentView( nRow, nCol );
   CWnd  *pWnd = GetDlgItem( IdFromRowCol( nRow, nCol ) );
   zLONG lPaneId = MAKELONG( nRow, nCol );
   zLONG lViewId;

   do  // purists goto
   {
      if ( pWnd && pWnd != pNewView )
      {
      // MessageBox( "Huh?", "ZSplitterWnd::AddView", MB_OK );
         TraceLineX( "ZSplitterWnd::AddView (huh?) Window: ", (zLONG) pWnd );
         m_mapIdPaneViews.Display( "PaneViews (huh?) AddView" );
         m_mapCurrentViews.Display( "CurrentViews (huh?) AddView" );
         CWnd *pParent = pWnd->SetParent( this );
         lViewId = m_lNextViewId++;
         m_mapIdPaneViews.Add( lViewId, lPaneId, (zLONG) pWnd );
         SetCurrentView( nRow, nCol, lViewId );
         lPreviousId = HideCurrentView( nRow, nCol );
      }

#if 0    // this prevents proper close
      else
      if ( pNewView )
      {
         ZPairedItem *pItem = m_mapIdPaneViews.Find( 0, 0, (zLONG) pNewView,
                                                     0, 4 );
         if ( pItem )
         {
            lViewId = pItem->m_lValue1;
            SetCurrentView( nRow, nCol, lViewId );
            break;;
         }
      }
#endif

      // Create the new view. On failure, set the previous view as current.
      if ( pNewView == 0 &&
           CreateView( nRow, nCol, pViewClass, CSize( 10, 10 ), pContext ) == 0 )
      {
         if ( lPreviousId != -1 )
            SetCurrentView( nRow, nCol, lPreviousId );

         lViewId = -1;
         break;
      }

      // Get and store the new view.
      lViewId = m_lNextViewId++;
      if ( pNewView == 0 )
         pNewView = GetPane( nRow, nCol );

      m_mapIdPaneViews.Add( lViewId, lPaneId, (zLONG) pNewView );

      // Set the new view current.
      SetCurrentView( nRow, nCol, lViewId );

   } while ( FALSE );  // end of: purists goto

   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "ZSplitterWnd::AddView(0x%08x) Row: %ld, Col: %ld, "
                    "PaneId: 0x%08x, ViewId: 0x%08x, View: 0x%08x",
             (zULONG) this, (zLONG) nRow, (zLONG) nCol, lPaneId, lViewId, (zULONG) pNewView );
   TraceLineS( szMsg, "" );
   sprintf_s( szMsg, zsizeof( szMsg ), "PaneViews After AddView: 0x%08x", (zULONG) pNewView );
   m_mapIdPaneViews.Display( szMsg );
   sprintf_s( szMsg, zsizeof( szMsg ), "CurrentViews After AddView: 0x%08x", (zULONG) pNewView );
   m_mapCurrentViews.Display( szMsg );
   RedrawWindow( );
   return( lViewId );
}

ZDrView *
ZSplitterWnd::RemoveView( zLONG lViewId )
{
   ZDrView     *pZView = 0;
   ZPairedItem *pItem1 = m_mapIdPaneViews.Find( lViewId, 0, 0, 0, 1 );
   ZPairedItem *pTemp1 = m_mapIdPaneViews.Remove( pItem1 );
   ZPairedItem *pItem2 = m_mapCurrentViews.Find( lViewId, 0, 0, 0, 1 );
   ZPairedItem *pTemp2 = m_mapCurrentViews.Remove( pItem2 );
   ASSERT( pItem1 == pTemp1 && pItem2 == pTemp2 );

   mDeleteInit( pItem2 );
   if ( pItem1 )
   {
      zLONG lPaneId = pItem1->m_lValue2;
      ZPairedItem *pItem = m_mapIdPaneViews.Find( 0, lPaneId, 0, 0, 2 );
      ZSubtask *pZSubtask = (ZSubtask *) pItem1->m_lValue4;
      if ( pZSubtask )
      {
         if ( pZSubtask->m_pZView != (ZDrView *) pItem1->m_lValue3 )
         {
            TraceLineS( "ZSplitterWnd::RemoveView Views do not match ... ",
                        "I DO NOT LIKE THIS!!!!" );
            pZView = pZSubtask->m_pZView;
            if ( pZView )
            {
               pZSubtask->m_pZView = 0;
               pZView->m_pZSubtask = 0;
               pZView->SetDlgCtrlID( 0 );
               delete( pZView );
            }
         }

         pZSubtask->m_pZView = (ZDrView *) (pItem1->m_lValue3);

         pZView = pZSubtask->m_pZView;
         if ( pZView )
            pZView->SetDlgCtrlID( 0 );

         if ( pItem && pItem->m_lValue1 )
         {
            CPoint p( lPaneId );
            zSHORT nRow = (zSHORT) p.x;
            zSHORT nCol = (zSHORT) p.y;

            SetCurrentView( nRow, nCol, pItem->m_lValue1 );
         }
      }

      delete( pItem1 );
   }

   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "PaneViews After RemoveView: 0x%08x", (zULONG) pZView );
   m_mapIdPaneViews.Display( szMsg );
   sprintf_s( szMsg, zsizeof( szMsg ), "CurrentViews After RemoveView: 0x%08x", (zULONG) pZView );
   m_mapCurrentViews.Display( szMsg );

   return( pZView );
}

void
ZSplitterWnd::ShowView( zLONG lViewId )
{
   if ( GetView( lViewId ) == 0 )
      return;

   // Find the pane containing the view.
   zSHORT nRow;
   zSHORT nCol;

   GetPaneFromViewId( lViewId, &nRow, &nCol );

   // Switch views.
   HideCurrentView( nRow, nCol );
   SetCurrentView( nRow, nCol, lViewId );

   RecalcLayout( );
}

ZDrView *
ZSplitterWnd::GetView( zLONG lViewId )
{
   ZPairedItem *pItem = m_mapIdPaneViews.Find( lViewId, 0, 0, 0, 1 );
   if ( pItem )
      return( (ZDrView *) pItem->m_lValue3 );

   return( 0 );
}

zLONG
ZSplitterWnd::GetView( ZSubtask *pZSubtask )
{
   ZPairedItem *pItem = m_mapIdPaneViews.Find( 0, 0, 0, (zLONG) pZSubtask, 8 );
   if ( pItem )
      return( pItem->m_lValue1 );

   return( 0 );
}

ZDrView *
ZSplitterWnd::GetCurrentView( zSHORT nRow, zSHORT nCol, zLONG *plCurrId )
{
   zLONG lPaneId = MAKELONG( nRow, nCol );
   ZPairedItem *pItem = m_mapCurrentViews.Find( 0, lPaneId, 0, 0, 2 );
   if ( pItem )
   {
      zLONG lPreviousId = pItem->m_lValue1;
      if ( plCurrId )
         *plCurrId = lPreviousId;

      return( GetView( lPreviousId ) );
   }

   return( 0 );
}

void
ZSplitterWnd::SetCurrentView( zSHORT nRow, zSHORT nCol, zLONG lViewId )
{
   zLONG lPaneId = MAKELONG( nRow, nCol );
   ZPairedItem *pItem = m_mapCurrentViews.Find( 0, lPaneId, 0, 0, 2 );
   if ( pItem )
      pItem->m_lValue1 = lViewId;
   else
      m_mapCurrentViews.Add( lViewId, lPaneId );

   ZDrView *pZView = GetView( lViewId );
   if ( pZView->GetParent( ) != this )
      pZView->m_pOrigParent = pZView->SetParent( this );

   pZView->SetDlgCtrlID( IdFromRowCol( nRow, nCol ) );
   pZView->ShowWindow( SW_SHOW );

   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "ZSplitterWnd::SetCurrentView(0x%08x) Row: %d Col: %d View: 0x%08x",
             (zULONG) this, nRow, nCol, (zULONG) pZView );
   TraceLineS( szMsg, "" );
}

zLONG
ZSplitterWnd::HideCurrentView( zSHORT nRow, zSHORT nCol )
{
   zLONG lPrevId;
   CWnd *pCurrView = GetCurrentView( nRow, nCol, &lPrevId );
   if ( pCurrView == 0 || mIs_hWnd( pCurrView->m_hWnd ) == 0 )
   {
      return( -1 );
   }
   else
   {
      pCurrView->SetDlgCtrlID( 0 );
      pCurrView->ShowWindow( SW_HIDE );
      return( lPrevId );
   }
}

zBOOL
ZSplitterWnd::GetPaneFromViewId( zLONG lViewId, zPSHORT pnRow, zPSHORT pnCol )
{
   ZPairedItem *pItem = m_mapIdPaneViews.Find( lViewId, 0, 0, 0, 1 );
   if ( pItem )
   {
      zLONG lPaneId = pItem->m_lValue2;
      CPoint p( lPaneId );
      *pnRow = (zSHORT) p.x;
      *pnCol = (zSHORT) p.y;
      return( TRUE );
   }
   else
   {
      *pnRow = -1;
      *pnCol = -1;
      return( FALSE );
   }
}

zBOOL
ZSplitterWnd::SetSubtaskForViewId( zLONG lViewId, ZSubtask *pZSubtask )
{
   ZPairedItem *pItem = m_mapIdPaneViews.Find( lViewId, 0, 0, 0, 1 );
   if ( pItem )
   {
      pItem->m_lValue4 = (zLONG) pZSubtask;

      if ( pZSubtask->m_pZView != (ZDrView *) pItem->m_lValue3 )
         ::MessageBox( 0, "Views do not match", "SetSubtaskForViewId", MB_OK );

#if 1
      zCHAR szMsg[ 256 ];

      CPoint p( pItem->m_lValue2 );
      sprintf_s( szMsg, zsizeof( szMsg ), "ZSplitterWnd::SetSubtaskForViewId Row: %ld, Col: %ld, "
                       "PaneId: 0x%08x, ViewId: 0x%08x, ZSubtask: 0x%08x  ZView: 0x%08x",
                p.x, p.y, pItem->m_lValue2, lViewId, (zULONG) pZSubtask,
                (zULONG) (pZSubtask ? pZSubtask->m_pZView : 0) );
      TraceLineS( szMsg, "" );



      ZDrView *pZView;
      zLONG    lPaneId;

      for ( zSHORT nCol = 0; nCol < 2; nCol++ )
      for ( zSHORT nRow = 0; nRow < 2; nRow++ )
      {
         pZView = GetCurrentView( nRow, nCol, &lViewId );
         if ( pZView )
         {
            pZSubtask = GetSubtaskForPane( nRow, nCol );
            lPaneId = MAKELONG( nRow, nCol );
            sprintf_s( szMsg, zsizeof( szMsg ), "ZSplitterWnd::DispView(0x%08x) Row: %ld, Col: %ld, "
                             "PaneId: 0x%08x, ViewId: 0x%08x, View: 0x%08x, ZSubtask: 0x%08x",
                      (zULONG) this, (zLONG) nRow, (zLONG) nCol, lPaneId, lViewId,
                  (zULONG) pZView, (zULONG) pZSubtask );
            TraceLineS( szMsg, "" );
         }
      }
#endif

      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

ZSubtask *
ZSplitterWnd::GetSubtaskForPane( zSHORT nRow, zSHORT nCol )
{
   zLONG lViewId;

   GetCurrentView( nRow, nCol, &lViewId );
   ZPairedItem *pItem = m_mapIdPaneViews.Find( lViewId, 0, 0, 0, 1 );
   if ( pItem )
      return( (ZSubtask *) pItem->m_lValue4 );

   return( 0 );
}

zLONG
ZSplitterWnd::GetLastView( )
{
   zLONG lViewId = -1;
   ZPairedItem *pItem = m_mapIdPaneViews.m_pHeadItem;
   while ( pItem )
   {
      // If it is not pane( 0, 0 ) and not a default view, not last view.
      if ( pItem->m_lValue2 && pItem->m_lValue4 )
         return( -1 );

      // If it is pane( 0, 0 ) and not a default view, this is the one
      // we are looking for.  If no other non-default views are found,
      // return this ViewId.
      if ( pItem->m_lValue2 == 0 && pItem->m_lValue4 )
         lViewId = pItem->m_lValue1;

      pItem = pItem->m_pNextItem;
   }

   return( lViewId );
}

ZPairedItem *
ZSplitterWnd::GetFirstPaneView( )
{
   return( m_mapIdPaneViews.m_pHeadItem );
}

void
ZSplitterWnd::OnClose( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSplitterWnd::OnClose", "" );
#endif
   m_pParentWnd = 0;
   CSplitterWnd::OnClose( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZSplitterView
//
IMPLEMENT_DYNCREATE( ZSplitterView, CView )
BEGIN_MESSAGE_MAP( ZSplitterView, CView )
   //{{AFX_MSG_MAP(ZSplitterView)
   ON_WM_CREATE( )
   ON_WM_SIZE( )
   ON_MESSAGE( zZEIDON_POSTED_SPLITTER_CLOSE, OnPostedSplitterClose )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

zLONG ZSplitterView::g_lIdNbr = 1;
ZSplitterView::ZSplitterView( )
{
   m_ulIdNbr = g_lIdNbr++;

#ifdef DEBUG_ALL
   TraceLine( "ZSplitterView::ctor (0x%08x): %d", this, m_ulIdNbr );
#endif

   m_pZSubtask = 0;
   m_pSplitterWnd = 0;
   m_bInitSize = TRUE;
   m_bInitialized = FALSE;
   m_lFlag = 0;  // 1 ==> right; 2 ==> down; 3 ==> both
}

ZSplitterView::~ZSplitterView( )
{
#ifdef DEBUG_ALL
   TraceLine( "ZSplitterView::dtor (0x%08x): %d", this, m_ulIdNbr );
#endif

   ZDrApp *pApp = (ZDrApp *) AfxGetApp( );
   if ( m_pZSubtask )
   {
      m_pZSubtask->m_pSplitterView = 0;
      if ( m_pZSubtask->m_pZView )
         m_pZSubtask->m_pZView->m_pParentSplitterWnd = 0;
   }

   if ( m_pSplitterWnd )
   {
      m_pSplitterWnd->DestroyWindow( );
      mDeleteInit( m_pSplitterWnd );
   }

   mDeleteInit( m_pParentData );
}

/////////////////////////////////////////////////////////////////////////////
void
ZSplitterView::OnDraw( CDC *pDC )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSplitterView::OnDraw", "" );
#endif
   // TODO: add draw code here
}
/////////////////////////////////////////////////////////////////////////////
// ZSplitterView diagnostics
#ifdef _DEBUG
void
ZSplitterView::AssertValid( ) const
{
   CView::AssertValid( );
}

void
ZSplitterView::Dump( CDumpContext& dc ) const
{
   CView::Dump( dc );
}
#endif //_DEBUG

int
ZSplitterView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSplitterView::OnCreate", "" );
#endif
   lpCreateStruct->style |= WS_OVERLAPPED;
   return( CView::OnCreate( lpCreateStruct ) );
}

void
ZSplitterView::OnSize( UINT nType, int cx, int cy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSplitterView::OnSize", "" );
#endif
   CView::OnSize( nType, cx, cy );
   if ( m_pSplitterWnd && mIs_hWnd( m_pSplitterWnd->m_hWnd ) )
   {
      m_pSplitterWnd->MoveWindow( 0, 0, cx, cy );

      // We just want to set the row/column sizes upon creation of the view.
      // The user can then move the splitter bar to how they like it and still
      // resize the frame window without it snapping back.
      if ( m_bInitSize && (cx || cy) )
      {
         zSHORT k;
         zSHORT nPercent;

         m_bInitSize = FALSE;
         nPercent = m_pSplitterWnd->m_nPercentX;
         for ( k = 0; k < m_pSplitterWnd->m_nCols; k++ )
         {
           m_pSplitterWnd->SetColumnInfo( 0, cx * nPercent / 100, 0 );
           nPercent = 100 - nPercent;
         }

         nPercent = m_pSplitterWnd->m_nPercentY;
         for ( k = 0; k < m_pSplitterWnd->m_nRows; k++ )
         {
            m_pSplitterWnd->SetRowInfo( 0, cy * nPercent / 100, 0 );
            nPercent = 100 - nPercent;
         }
      }

      m_pSplitterWnd->RecalcLayout( );
   }
}

void
ZSplitterView::OnInitialUpdate( )
{
   // Because of the structure of this app, this function can be called more than once.
   // The following flag ensures the code after is only run once:
   if ( m_bInitialized )
   {
      ::MessageBox( 0, "Called more than once",
                    "ZSplitterView::OnInitialUpdate", MB_OK );
      return;
   }

   m_bInitialized = TRUE;
   CView  *pView;
   zSHORT k;
   zSHORT j;

   for ( k = 0; k < m_pSplitterWnd->m_nCols; k++ )
   {
      for ( j = 0; j < m_pSplitterWnd->m_nRows; j++ )
      {
         pView = (CView *) m_pSplitterWnd->GetPane( j, k );
         pView->OnInitialUpdate( );
      }
   }

#ifdef DEBUG_ALL
   TraceLineS( "ZSplitterView::OnInitialUpdate", "" );
#endif
}

// This is called from pZView->m_pZSubtask->m_pZFWnd->DestroyWindow( ).
LRESULT
ZSplitterView::OnPostedSplitterClose( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineX( "ZSplitterView::OnPostedSplitterClose: ", (zLONG) this );
#endif

   ZSubtask *pZSubtask = (ZSubtask *) lParam;
   if ( pZSubtask && m_pSplitterWnd )
   {
      zLONG lViewId = m_pSplitterWnd->GetView( pZSubtask );
      ZDrView *pZView = m_pSplitterWnd->GetView( lViewId );
      if ( pZView == pZSubtask->m_pZView )
      {
         m_pSplitterWnd->RemoveView( lViewId );
         if ( pZView->m_pOrigParent )
            pZView->SetParent( pZView->m_pOrigParent );


         // If the only "non-default" view left is at pane( 0, 0 ), then
         // it is time to remove the splitter and all the default views.
         lViewId = m_pSplitterWnd->GetLastView( );
         if ( lViewId >= 0 )
         {
            pZView = m_pSplitterWnd->RemoveView( lViewId );
            if ( pZView )
            {
               if ( pZView->m_pOrigParent )
                  pZView->SetParent( pZView->m_pOrigParent );

               if ( pZView->m_pZSubtask->m_pZParent &&
                    pZView->m_pZSubtask->m_pZParent->m_pSplitterView )
               {
                  pZView->m_pOrigParent =
                     pZView->SetParent( pZView->m_pZSubtask->m_pZParent->
                                          m_pSplitterView->m_pSplitterWnd );
               }
            }

            ZPairedItem *pItem = m_pSplitterWnd->GetFirstPaneView( );
            while ( pItem )
            {
               pZView = m_pSplitterWnd->RemoveView( pItem->m_lValue1 );
               if ( pZView && mIs_hWnd( pZView->m_hWnd ) )
                  pZView->DestroyWindow( );

               pItem = m_pSplitterWnd->GetFirstPaneView( );
            }

            SendMessage( WM_CLOSE );
         }
      }

      while ( pZSubtask->m_pZParent )
         pZSubtask = pZSubtask->m_pZParent;

      pZSubtask->m_pZFWnd->SetActiveView( pZSubtask->m_pZView );
      pZSubtask->m_pZFWnd->RecalcLayout( );
   }

   return( 0 );
}

#if 0
/////////////////////////////////////////////////////////////////////////////
//This function displays a view that is chosen from the toolbar or menu. There
//are five types to choose from: FormView1, FormView2, EditView, ListView, and SplitterView
void CMainFrame::OnChooseView(UINT nCmdID)
{
   if ( nCmdID == m_nCurrentView )
      return; //view is already selected, so don't need to destroy it and then create it again

   ZDrDoc *pDoc = ((ZDrApp *)AfxGetApp())->m_pDoc;
   if ( pDoc == 0 )  //most likely a problem reading in file
   {
      AfxMessageBox( "A problem occurred. Try to open a new file. If that"
                     "doesn't work, please close the application and run it again" );
      return;
   }

   CWnd *pWnd;
   CWnd *pWndToDelete;

   //The use of EnumChildWindows below is to find a child window that is derived from CView.
   //GetActiveView() will return NULL if no view has focus, so we can't use it here.
   //GetActiveDocument() also can't be used because it calls GetActiveView().
   ::EnumChildWindows( m_hWnd, MyWndEnumProc, (LPARAM) &pWnd );

   if ( pWnd == NULL )
   {
      AfxMessageBox( "Problem: CMainFrame::OnChooseView can not find any views!" );
      return;
   }

   //Now that we've found a view, search up the parent tree until we come to the CMainFrame
   //window. Then set pWndToDelete to the CWnd that is the immediate child of CMainFrame.
   //This is done to find the most senior view (which may or may not have children views).
   //CMainFrame also has children, such as the toolbar, that we don't want to delete.
   while ( zstrcmp(pWnd->GetRuntimeClass( )->m_lpszClassName, "CMainFrame" ) )
   {
      pWndToDelete = pWnd;
      pWnd = pWnd->GetParent( );
   }

   CRuntimeClass *pNewViewRTClass;
   switch ( nCmdID )
   {
      case ID_VIEW_FORMVIEW1:
         pNewViewRTClass = RUNTIME_CLASS( CFormView1 );
         break;

      case ID_VIEW_FORMVIEW2:
         pNewViewRTClass = RUNTIME_CLASS( CFormView2 );
         break;

      case ID_VIEW_EDITVIEW:
         pNewViewRTClass = RUNTIME_CLASS( CMyEditView );
         break;

      case ID_VIEW_LISTVIEW:
         pNewViewRTClass = RUNTIME_CLASS( CMyListView );
         break;

      case ID_VIEW_SPLITTER:
         pNewViewRTClass = RUNTIME_CLASS( ZSplitterView );
         break;

      default:
         ASSERT( 0 );
         return;
   }

   //Create the new view:
   CCreateContext context;
   context.m_pNewViewClass = pNewViewRTClass;
   context.m_pCurrentDoc = pDoc;

   m_pNewView = STATIC_DOWNCAST( CView, CreateView( &context ) );
   if ( m_pNewView )
   {
      m_nCurrentView = nCmdID;

      //Make sure that the document won't be destroyed when the view is destroyed.
      //m_bAutoDelete is a public, but non-documented member of CDocument.
      pDoc->m_bAutoDelete = FALSE;

      //destroy the old view:
      pWndToDelete->DestroyWindow( );

      //OnInitialUpdate() is not called as a result of calling CreateView() above.
      //It is not always called by the framework, so it is called here:
      m_pNewView->OnInitialUpdate( );

      //Show and activate view:
      m_pNewView->ShowWindow( SW_SHOW );
      SetActiveView( m_pNewView );
      RecalcLayout( );
   }
   else
   {
      AfxMessageBox( "problem creating a view in CMainFrame::OnChooseView( )" );
   }
}
#endif
