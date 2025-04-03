/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrview.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZDrView and ZDrEdtView.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.13    TMV
//    Hi Doug, I completed your changes regarding print preview toolbar customization
//    look for #ifndef QUINSOFT in OnFilePrintPreview
//
// 2001.09.20    DKS
//    Fix to set number of preview pages.
//
// 2001.06.26    TMV
//    change OnCommonFind. It will do nothing in a remote server situation
//
// 2001.06.26    TMV
//    Change OperatorPromptFindReplace to end the dialog after the first event
//    occurs that is a FIND situation see OnCommonFind.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2001.04.10    TMV   2000   ActiveX
//    Overload method OnMouseActivate for ZDrView.
//
// 2001.03.28    TMV   2000   ActiveX
//    Changed ZDrView::InitAmbientProperties to get COLOR_WINDOWTEXT
//    instead of RGB( 255, 0, 0 ).
//
// 2001.03.27    DKS
//    Cleaned up memory leak.  Fixed GetSysColor call.
//
// 2001.03.26    DKS
//    Fixed problem when printing to a file.
//
// 2001.03.13    PAS
//    The "FontScale" has been moved to a printer specific section and
//    is no longer in the [Workstation] section of the INI chain
//    Now in ZPrintout::RemoteFilePrint and in ZDrView::OnBeginPrinting
//
// 2001.02.09    DKS   HPF
//    Print "Page # of #" implemented.
//
// 2000.09.19    TMV   Z10
//    reimplement Ambient Properties for ZDrView
//
// 2000.03.29    TMV   Z10
//    move
//    const VARIANT_BOOL VAR_TRUE = ~VARIANT_FALSE;
//    const VARIANT_BOOL VAR_FALSE = VARIANT_FALSE;
//    to zdr.h
//
// 1999.11.23    TMV   Z10
//    Implement Ambient Properties for ZDrView
//
// 1999.09.14    TMV   Z10
//    Basic implementation for Common Search/Replace Dialog
//
// 1999.05.11    DKS   Z10    ActiveX
//    Basic rewrite for new ActiveX implementation.
//
// 1999.02.02    DKS
//    Added default font support for reports.
//
// 1998.10.12    DKS
//    Modified Dialog conversion to set the size of the Relative Pos/Size
//    area properly.
//
// 1998.10.09    DKS/TMV
//    Investigation of the SetFocus Bug within the Editor.  Extended the
//    wrapper class to fix the SetFocus bug.
//    Refer bugtracking system for detailed information (TB 206)
//
// 1998.06.02    TMV
//    New Version of editor has different GUID
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

// defines for ZAmbientProperty
//
#ifndef VTI_EMPTY
#define VTI_EMPTY    0
#define VTI_BOOL     1
#define VTI_UI1      2
#define VTI_UI2      3
#define VTI_UI4      4
#define VTI_UI8      5
#define VTI_I1       6
#define VTI_I2       7
#define VTI_I4       8
#define VTI_I8       9
#define VTI_R4       10
#define VTI_R8       11
#define VTI_BSTR     12
#define VTI_CY       13
#define VTI_DATE     14
#define VTI_ERROR    15
#define VTI_COLOR    16
#define VTI_FONT     17
#define VTI_UNKNOWN  18
#define NUM_VARTYPES 19
#endif

const UINT g_uMsgFindString = RegisterWindowMessage( FINDMSGSTRING );

/////////////////////////////////////////////////////////////////////////////
// ZDrView
// Derive from ZDrView to provide background color to form views.
//

//IMPLEMENT_DYNAMIC( ZDrView, CFormView )
IMPLEMENT_DYNCREATE( ZDrView, CFormView )

BEGIN_MESSAGE_MAP( ZDrView, CFormView )
   ON_WM_CREATE( )
   ON_WM_HSCROLL( )
   ON_WM_VSCROLL( )
   ON_WM_DESTROY( )
   ON_WM_SETFOCUS( )
   ON_WM_SIZE( )

   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_MBUTTONDOWN( )
   ON_WM_MBUTTONUP( )
   ON_WM_MBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_RBUTTONDBLCLK( )
   ON_WM_MOUSEMOVE( )

   ON_WM_ERASEBKGND( )
   ON_WM_CTLCOLOR( )
   ON_WM_PAINT( )
   ON_COMMAND( ID_OLE_INSERT_NEW, OnInsertObject )
   ON_COMMAND( ID_CANCEL_EDIT_CNTR, OnCancelEditCntr )

   // Standard printing commands
   ON_COMMAND( ID_FILE_PRINT, CView::OnFilePrint )
   ON_COMMAND( ID_FILE_PRINT_DIRECT, CView::OnFilePrint )
   ON_COMMAND( ID_FILE_PRINT_PREVIEW, OnFilePrintPreview )

   ON_REGISTERED_MESSAGE( g_uMsgFindString, OnCommonFind )
   ON_WM_MOUSEACTIVATE( )

END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZDrView construction/destruction

CString ZDrView::m_csClassName = _T( "" );

#define zSIMILAR_VIEWCLASS  // to prevent Win9x background paint problems

#ifdef zSIMILAR_VIEWCLASS
static LPCSTR
RegisterSimilarClass( LPCSTR cpcNewClassName,
                      LPCSTR cpcOldClassName,
                      UINT   nIDResource )
{
   // Get class info for old class.
   HINSTANCE hInst = AfxGetInstanceHandle( );
   WNDCLASS  wc;

   if ( ::GetClassInfo( hInst, cpcOldClassName, &wc ) == 0 )
   {
      TraceLineS( "Can't find window class: ", cpcOldClassName );
      return( 0 );
   }

   // Register new class with same info, but different name and icon.
   wc.lpszClassName = cpcNewClassName;
// wc.hIcon = ::LoadIcon( hInst, MAKEINTRESOURCE( nIDResource ) );
   wc.hbrBackground = (HBRUSH) GetStockObject( LTGRAY_BRUSH );
   if ( AfxRegisterClass( &wc ) == 0 )
   {
      TraceLineS( "Unable to register window class: ", cpcNewClassName );
      return( 0 );
   }

   return( cpcNewClassName );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zLONG ZDrView::g_lIdNbr = 1;
ZDrView::ZDrView( ZSubtask *pZSubtask ) :
         CFormView( IDD_INPUTFORM )  // instead of CScrollView to get TABBING
{
   m_ulViewIdNbr = g_lIdNbr++;
   if ( pZSubtask )
      m_pZSubtask = pZSubtask;
   else
      m_pZSubtask = ((ZDrApp *) AfxGetApp( ))->m_pZSubtaskCurr;

#ifndef zSIMILAR_VIEWCLASS
   if ( m_csClassName.IsEmpty( ) )
   {
      m_csClassName =
         ::AfxRegisterWndClass( CS_DBLCLKS,
                                ::LoadCursor( NULL, IDC_ARROW ),
                                (HBRUSH) GetStockObject( LTGRAY_BRUSH ) );
                             // ::GetSysColorBrush( COLOR_BTNFACE ) );
   }
#endif

   if ( m_pZSubtask )
   {
      if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ), "ZDrView::ctor (0x%08x): %d Subtask: (%s) %d View: %d",
                    (zULONG) this, m_ulViewIdNbr, (zPCHAR) (*(m_pZSubtask->m_pzsWndTag)).GetString(),
                    pZSubtask->m_ulSubtaskIdNbr, pZSubtask->m_pZView ? pZSubtask->m_pZView->m_ulViewIdNbr : 0 );
         if ( pZSubtask->m_pZView )
         {
            strcat_s( szMsg, zsizeof( szMsg ), " ##### Original view: " );
            TraceLineI( szMsg, pZSubtask->m_pZView->m_ulViewIdNbr );
         }
         else
            TraceLineS( szMsg, "" );
      }

      m_pZSubtask->m_pZView = this;
      m_pBrushBk = new CBrush( m_pZSubtask->m_clrBk );
   }
   else
   {
#ifdef DEBUG_ALL
      TraceLine( "ZDrView::ctor (0x%08x): %d", this, m_ulViewIdNbr );
#endif
      m_pBrushBk = new CBrush( ::GetSysColor( COLOR_BTNFACE ) );
   }

   m_pItemWithFocus = 0;
   m_pItemWithCapture = 0;
   m_pSelection = 0;
   m_lScrollPosX = 0;
   m_lPageSizeX = 0;
   m_lScrollPosY = 0;
   m_lPageSizeY = 0;
   m_pOrigParent = 0;

   m_eQuad = eUndef;
   m_pParentSplitterView = 0;
   m_pParentSplitterWnd = 0;
   m_pPrevView = 0;
   m_pNextView = 0;

   // m_pZTask->m_pZDrApp->SetDialogBkColor( RGB( 45, 235, 255 ),
   //                                        RGB( 0, 0, 255 ) );
// pWinState->m_hDlgBkBrush = ::CreateSolidBrush( clrCtlBk );
// pWinState->m_crDlgTextClr = clrCtlText;
   m_bTraceAmbientPropertyRequest = FALSE;
   m_pFontHolder = 0;
   m_descFont.lpstrName = 0;

// m_pSplitterWnd = 0;
// m_bShouldSetXColumn = TRUE;
// m_bInitialized = FALSE;

   CString csHelp;
   zPCHAR pszStorageFile = csHelp.GetBufferSetLength( zMAX_FILENAME_LTH );
   SysReadZeidonIni( -1, "[ActiveX]",
                     "TraceAmbientPropertyRequest", pszStorageFile, zMAX_FILENAME_LTH );
   csHelp.ReleaseBuffer( );
   if ( csHelp.CompareNoCase( "Y" ) == 0 )
      m_bTraceAmbientPropertyRequest = TRUE;

   InitAmbientProperties( );
}

ZDrView::~ZDrView( )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLine( "ZDrView::dtor (0x%08x): %d  PropertyCnt: %d", this, m_ulViewIdNbr,
                 m_apList.GetSize( ) );

   if ( m_pPrevView && m_pNextView )
   {
      m_pPrevView->m_pNextView = m_pNextView;
      m_pNextView->m_pPrevView = m_pPrevView;
   }

   if ( m_pZSubtask )
   {
      m_pZSubtask->m_pZView = 0;
      m_pZSubtask = 0;
   }

// m_pSplitterWnd = 0;

   RemoveAmbientProperties( );
   mDeleteInit( m_pFontHolder );
   mDeleteInit( m_pBrushBk );
   mDeleteInitA( m_descFont.lpstrName );
}

BOOL
ZDrView::PreCreateWindow( CREATESTRUCT& cs )
{
   // TODO: Modify the Window class or styles here by modifying
   // the CREATESTRUCT cs.

   if ( CFormView::PreCreateWindow( cs ) == FALSE )
      return( FALSE );

#ifdef zSIMILAR_VIEWCLASS
   if ( *m_csClassName == 0 )
   {
      // One-time initialization: register the class
      m_csClassName = RegisterSimilarClass( "ZeidonDialogView",
                                            cs.lpszClass,
                                            IDR_MAINFRAME );
      if ( *m_csClassName == 0 )
         return( FALSE );
   }
#endif

   cs.lpszClass = m_csClassName;
   return( TRUE );
}

BOOL
ZDrView::Create( LPCTSTR cpcClassName, LPCTSTR cpcWindowName,
                 DWORD dwRequestedStyle, const RECT& rect,
                 CWnd *pParentWnd, UINT nID, CCreateContext *pContext )
{
   BOOL bRC = CWnd::Create( cpcClassName, cpcWindowName,
                            dwRequestedStyle, rect, pParentWnd,
                            nID, pContext );
   if ( bRC == FALSE )
      return( FALSE );

// typedef struct tagSCROLLINFO
// {  // si
//
//    UINT cbSize;
//    UINT fMask;
//    int  nMin;
//    int  nMax;
//    UINT nPage;
//    int  nPos;
//    int  nTrackPos;
//
// }   SCROLLINFO;
// typedef SCROLLINFO FAR *LPSCROLLINFO;
//
// Members:
//    cbSize - Specifies the size, in bytes, of this structure.
//    fMask - Specifies the scroll bar parameters to set or retrieve.
//            This member can be a combination of the following values:
//             SIF_ALL - Combination of SIF_PAGE, SIF_POS, and SIF_RANGE.
//             SIF_DISABLENOSCROLL - This value is used only when setting
//                a scroll bar's parameters.  If the scroll bar's new
//                parameters make the scroll bar unnecessary, disable the
//                scroll bar instead of removing it.
//             SIF_PAGE - The nPage member contains the page size for a
//                proportional scroll bar.
//             SIF_POS - The nPos parameter contains the scroll box
//                position.
//             SIF_RANGE - The nMin and nMax members contain the minimum
//                and maximum values for the scrolling range.
//    nMin - Specifies the minimum scrolling position.
//    nMax - Specifies the maximum scrolling position.
//    nPage - Specifies the page size.  A scroll bar uses this value to
//           determine the appropriate size of the proportional scroll box.
//    nPos - Specifies the position of the scroll box.
//    nTrackPos - Specifies the immediate position of a scroll box that the
//           user is dragging.  An application can retrieve this value while
//           processing the SB_THUMBTRACK notification message.  An
//           application cannot set the immediate scroll position ... the
//           SetScrollInfo function ignores this member.
//
// BOOL SetScrollInfo( int nBar, LPSCROLLINFO lpScrollInfo,
//                     BOOL bRedraw = TRUE );
// Return Value:
//    If successful, the return is TRUE. Otherwise, it is FALSE.
// Parameters:
//    nBar - Specifies whether the scroll bar is a control or part of a
//           window's nonclient area.  If it is part of the nonclient area,
//           nBar also indicates whether the scroll bar is positioned
//           horizontally, vertically, or both.  It must be one of the
//           following:
//       SB_BOTH - Specifies the horizontal and vertical scroll bars of
//           the window.
//       SB_HORZ - Specifies that the window is a horizontal scroll bar.
//       SB_VERT - Specifies that the window is a vertical scroll bar.
//    lpScrollInfo - A pointer to a SCROLLINFO structure.
//    bRedraw - Specifies whether the scroll bar should be redrawn to
//              reflect the new position.  If bRedraw is TRUE, the scroll
//              bar is redrawn.  If it is FALSE, it is not redrawn.  The
//              scroll bar is redrawn by default.
// Remarks:
//    Call this member function to set the information that the SCROLLINFO
//    structure maintains about a scroll bar.  The SCROLLINFO structure
//    contains information about a scroll bar, including the minimum and
//    maximum scrolling positions, the page size, and the position of the
//    scroll box (the thumb).  See the SCROLLINFO structure topic in the
//    Win32 SDK Programmer's Reference for more information about changing
//    the structure defaults.  The MFC Windows message handlers that indicate
//    scroll-bar position, CWnd::OnHScroll and CWnd::OnVScroll, provide only
//    16 bits of position data. GetScrollInfo and SetScrollInfo provide 32
//    bits of scroll-bar position data.  Thus, an application can call
//    GetScrollInfo while processing either CWnd::OnHScroll or
//    CWnd::OnVScroll to obtain 32-bit scroll-bar position data.
//

#if 0
   SCROLLINFO si;

   si.cbSize = sizeof( SCROLLINFO );
   si.fMask = SIF_ALL;
   si.nMin = 0;
   si.nMax = 400;
   si.nPage = 40;
   si.nPos = 0;
   si.nTrackPos = 0;

   SetScrollInfo( SB_VERT, &si, FALSE );
   SetScrollInfo( SB_HORZ, &si, FALSE );
#endif

   return( bRC );
}

int
ZDrView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CFormView::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

#if 0
   // Initialize other colors not passed into ctor:
   m_clrEdit = RGB( 249, 166, 174 );   // peach
   m_clrListBox = RGB( 198, 40, 1zTAG_LTH ); // maroon purple
   m_clrText = RGB( 0, 0, 0 );         // black

   // Initialize brushes:
   if ( m_BrushFVBkgnd.CreateSolidBrush( m_clrFVBkgnd ) &&
        m_BrushEdit.CreateSolidBrush( m_clrEdit ) &&
        m_BrushListBox.CreateSolidBrush( m_clrListBox ) )
   {
      return( 0 );
   }
   else
   {
      return( -1 );
   }
#endif

   ModifyStyle( 0, WS_CLIPCHILDREN );  // this causes groupbox to not paint
                                       // background properly ???
   ModifyStyleEx( 0, WS_EX_CONTROLPARENT );

   if ( GetDocument( ) == 0 )
   {
      POSITION pos = AfxGetApp( )->GetFirstDocTemplatePosition( );
      if ( pos )
      {
         CDocTemplate *pDocTemplate =
            ((ZDrApp *) AfxGetApp( ))->GetNextDocTemplate( pos );
         pos = pDocTemplate->GetFirstDocPosition( );
         if ( pos )
         {
            CDocument *pDoc = pDocTemplate->GetNextDoc( pos );
            pDoc->AddView( this );
         }
      }
   }

   SetWindowText( "ZDrView" );
   return( 0 );
}

void
ZDrView::OnInitialUpdate( )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrView::OnInitialUpdate Start", "" );

   CFormView::OnInitialUpdate( );

   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrView::OnInitialUpdate Mid", "" );

   if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_pZFWndHidden == 0 )
      GetParentFrame( )->DelayRecalcLayout( );

   // TODO: remove this code when final selection model code is written
   m_pSelection = 0;    // initialize selection

   // Because of the structure of this app, this function can be called more
   // than once.  The following flag ensures the code is only run once.
#if 0
   if ( m_bInitialized == FALSE )
   {
      m_bInitialized = TRUE;
      // 2 vertical panes (the right pane happens to have sub-panes).
      for ( int k = 0; k < 2; k++ )
      {
         CView *pView = (CView *) m_pSplitterWnd.GetPane( 0, k );
         pView->OnInitialUpdate( );
      }
   }
#endif
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrView::OnInitialUpdate End", "" );
}

void
ZDrView::OnFilePrintPreview( )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrView::OnFilePrintPreview", "" );

   // In derived classes, implement special window handling here
   // Be sure to Unhook Frame Window close if hooked.

   // Must not create this on the frame.  Must outlive this function.
   CPrintPreviewState *pState = new CPrintPreviewState;

   // DoPrintPreview's return value does not necessarily indicate that
   // Print preview succeeded or failed, but rather what actions are necessary
   // at this point.  If DoPrintPreview returns TRUE, it means that
   // OnEndPrintPreview will be (or has already been) called and the
   // pState structure will be/has been deleted.
   // If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
   // WILL NOT be called and that cleanup, including deleting pState
   // must be done here.

   CFrameWnd *pWndSave = (CFrameWnd *) m_pZSubtask->m_pZTask->m_pZDrApp->m_pMainWnd;
   m_pZSubtask->m_pZTask->m_pZDrApp->m_pMainWnd = m_pZSubtask->m_pZFWnd;
//#ifndef QUINSOFT
   zLONG nDialogID = IDD_PREVIEW;
//#else
// zLONG nDialogID = AFX_IDD_PREVIEW_TOOLBAR;
//#endif
   if ( !DoPrintPreview( nDialogID, this,
                         RUNTIME_CLASS( ZDrPreviewView ), pState ) )
   {
      // In derived classes, reverse special window handling here for
      // Preview failure case.
      TraceLineS( "Error: DoPrintPreview failed.", "" );
      AfxMessageBox( AFX_IDP_COMMAND_FAILURE );
      delete( pState );      // preview failed to initialize, delete State now
   }

   m_pZSubtask->m_pZTask->m_pZDrApp->m_pMainWnd = pWndSave;
}

void
ZDrView::OnEndPrintPreview( CDC *pDC, CPrintInfo *pPrintInfo,
                            POINT pt, CPreviewView *pView )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrView::OnEndPrintPreview", "" );

   CFrameWnd *pWndSave =
                  (CFrameWnd *) m_pZSubtask->m_pZTask->m_pZDrApp->m_pMainWnd;
   m_pZSubtask->m_pZTask->m_pZDrApp->m_pMainWnd = m_pZSubtask->m_pZFWnd;
   CView::OnEndPrintPreview( pDC, pPrintInfo, pt, pView );
   m_pZSubtask->m_pZTask->m_pZDrApp->m_pMainWnd = pWndSave;
}

// Return false if print job has been cancelled.
BOOL
ZDrView::OnPreparePrinting( CPrintInfo *pPrintInfo )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineI( "ZDrView::OnPreparePrinting NumPreviewPages: ",
                  pPrintInfo->m_nNumPreviewPages );

   if ( m_pZSubtask == 0 || m_pZSubtask->m_pZPrintout == 0 ||
        m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine == 0 )
   {
      if ( m_pZSubtask )
         mDeleteInit( m_pZSubtask->m_pZPrintout );

      return( FALSE );
   }

   // Determine the page count here if requested.
// if ( m_pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PREPAGE )
//    m_pZSubtask->m_pZPrintout->m_uMaxPageNbr =
//       (*(m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine))
//                  ( m_pZSubtask->m_vDialog, 0, // print page 0
//                    m_pZSubtask->m_pZPrintout->m_lHorzSize256ths,
//                    m_pZSubtask->m_pZPrintout->m_lVertSize256ths );
// else
      m_pZSubtask->m_pZPrintout->m_uMaxPageNbr = 9999;

   pPrintInfo->SetMaxPage( m_pZSubtask->m_pZPrintout->m_uMaxPageNbr );

   // Default preparation.
// PRINTDLG pd;
// pd.lStructSize = (DWORD) sizeof( PRINTDLG );
   pPrintInfo->m_pPD->m_pd.lStructSize = (DWORD) sizeof( PRINTDLG );
   if ( m_pZSubtask->m_pZTask->m_pZDrApp->GetPrinterDeviceDefaults( &(pPrintInfo->m_pPD->m_pd) ) )
   {
   // zCHAR szDeviceName[ CCHDEVICENAME + 1 ];

      // Protect memory handle with ::GlobalLock and ::GlobalUnlock.
      LPDEVMODE pDevMode = (LPDEVMODE) ::GlobalLock( m_pZSubtask->m_pZTask->m_pZDrApp->m_hDevMode );

      // Set orientation to landscape.
      pDevMode->dmOrientation = (m_pZSubtask->m_pZPrintout->m_bLandscape) ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
      ::GlobalUnlock( m_pZSubtask->m_pZTask->m_pZDrApp->m_hDevMode );
   }

   if ( DoPreparePrinting( pPrintInfo ) )
   {
      pPrintInfo->m_nNumPreviewPages = 1;
#if 0
      if ( pPrintInfo->m_pPD->m_pd.hDevMode )
      {
         LPDEVMODE pDevMode =
                  (LPDEVMODE) ::GlobalLock( pPrintInfo->m_pPD->m_pd.hDevMode );
         pDevMode->dmOrientation =
            (m_pZSubtask->m_pZPrintout->m_bLandscape) ? DMORIENT_LANDSCAPE :
                                                        DMORIENT_PORTRAIT;
         ::GlobalUnlock( pPrintInfo->m_pPD->m_pd.hDevMode );

         LPDEVNAMES lpDev =
                  (LPDEVNAMES) GlobalLock( pPrintInfo->m_pPD->m_pd.hDevNames );
         zCPCHAR cpc = (LPCTSTR) lpDev + lpDev->wDeviceOffset;
         TraceLineS( "ZDrView::OnPreparePrinting: ", cpc );
         ::GlobalUnlock( pPrintInfo->m_pPD->m_pd.hDevNames );
      }
#endif
      return( TRUE );
   }

   return( FALSE );
}

void
ZDrView::OnBeginPrinting( CDC *pDC, CPrintInfo *pPrintInfo )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineI( "ZDrView::OnBeginPrinting NumPreviewPages: ",
                  pPrintInfo->m_nNumPreviewPages );

   if ( m_pZSubtask == 0 || m_pZSubtask->m_pZPrintout == 0 )
      return;

   // TODO: add extra initialization before printing
   m_pZSubtask->m_pZPrintout->m_pDC = pDC;
   m_pZSubtask->m_pZPrintout->m_pPrintInfo = pPrintInfo;
// TraceLineX( "BeginPrinting DC ", (zLONG) pDC );
   m_pZSubtask->m_pZPrintout->m_lPageCnt = 0;

   // Default preparation.
// PRINTDLG pd;
// pd.lStructSize = (DWORD) sizeof( PRINTDLG );
   pPrintInfo->m_pPD->m_pd.lStructSize = (DWORD) sizeof( PRINTDLG );
   if ( m_pZSubtask->m_pZTask->m_pZDrApp->GetPrinterDeviceDefaults( &(pPrintInfo->m_pPD->m_pd) ) )
   {
   // zCHAR szDeviceName[ CCHDEVICENAME + 1 ];
      zCHAR szYN[ 2 ];

      // Protect memory handle with ::GlobalLock and ::GlobalUnlock.
      LPDEVMODE pDevMode = (LPDEVMODE)
         ::GlobalLock( m_pZSubtask->m_pZTask->m_pZDrApp->m_hDevMode );

      // Set the way we rotate text by getting INI setting for printer.
      GetWorkstationAppValue( m_pZSubtask->m_vDialog, (zPCHAR) pDevMode->dmDeviceName,
                              "NegateEscapement", szYN, zsizeof( szYN ) );
      if ( szYN[ 0 ] == 'y' || szYN[ 0 ] == 'Y' )
      {
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
         {
            TraceLineS( "Using printer (with NegateEscapement): ",
                        (zPCHAR) pDevMode->dmDeviceName );
         }

         m_pZSubtask->m_pZPrintout->m_nEscapementFactor = -1;
      }
      else
      {
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
            TraceLineS( "Using printer: ", (zPCHAR) pDevMode->dmDeviceName );

         m_pZSubtask->m_pZPrintout->m_nEscapementFactor = 1;
      }

      // Set any font scaling specific to printer from INI setting.
      zCHAR szFontScale[ 128 ];

      GetWorkstationAppValue( m_pZSubtask->m_vDialog,
                              (zPCHAR) pDevMode->dmDeviceName,
                              "FontScale", szFontScale,
                              zsizeof( szFontScale ) );
      if ( szFontScale[ 0 ] )
      {
         m_pZSubtask->m_pZPrintout->m_dHeight = atof( szFontScale );
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
            TraceLineS( "        =====: with FontScale factor ", szFontScale );
      }
      else
         m_pZSubtask->m_pZPrintout->m_dHeight = 0;

      ::GlobalUnlock( m_pZSubtask->m_pZTask->m_pZDrApp->m_hDevMode );
   }

   // Determine how many pixels are in a 16th of an inch.
   CPoint pt( 1000, 1000 );   // a HIENGLISH point is .001 inches

   // Change the mapping mode to high english.
   int nOldMapMode = pDC->SetMapMode( MM_HIENGLISH );

   // Convert the client rect sizes to high english.
   pDC->LPtoDP( &pt );

   // Determine the page extent in 16ths of an inch.
   // HORZRES    - Width, in pixels, of the screen.
   // VERTRES    - Height, in raster lines, of the screen.
   // LOGPIXELSX - Number of pixels per logical inch along the screen width.
   // LOGPIXELSY - Number of pixels per logical inch along the screen height.
   if ( m_pZSubtask->m_pZPrintout->m_bPrintToHTML )
   {
      CWindowDC dc( CWnd::GetDesktopWindow( ) );

      m_pZSubtask->m_pZPrintout->m_lHorzRes = dc.GetDeviceCaps( HORZRES );
      m_pZSubtask->m_pZPrintout->m_lVertRes = dc.GetDeviceCaps( VERTRES );
      m_pZSubtask->m_pZPrintout->m_nPixelsPerInchX = dc.GetDeviceCaps( LOGPIXELSX );
      m_pZSubtask->m_pZPrintout->m_nPixelsPerInchY = dc.GetDeviceCaps( LOGPIXELSY );
      m_pZSubtask->m_pZPrintout->m_nPixelsOffsetX = dc.GetDeviceCaps( PHYSICALOFFSETX );
      m_pZSubtask->m_pZPrintout->m_nPixelsOffsetY = dc.GetDeviceCaps( PHYSICALOFFSETY );
   }
   else
   {
      // Determine the page extent in 16ths of an inch.
      m_pZSubtask->m_pZPrintout->m_lHorzRes = pDC->GetDeviceCaps( HORZRES );
      m_pZSubtask->m_pZPrintout->m_lVertRes = pDC->GetDeviceCaps( VERTRES );
      m_pZSubtask->m_pZPrintout->m_nPixelsPerInchX = pDC->GetDeviceCaps( LOGPIXELSX );
      m_pZSubtask->m_pZPrintout->m_nPixelsPerInchY = pDC->GetDeviceCaps( LOGPIXELSY );
      m_pZSubtask->m_pZPrintout->m_nPixelsOffsetX = pDC->GetDeviceCaps( PHYSICALOFFSETX );
      m_pZSubtask->m_pZPrintout->m_nPixelsOffsetY = pDC->GetDeviceCaps( PHYSICALOFFSETY );
   }

   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
   {
      TraceLineS( "=====================", "" );
      TraceLineI( "DeviceCaps LOGPIXELSX ", m_pZSubtask->m_pZPrintout->m_nPixelsPerInchX );
      TraceLineI( "DeviceCaps LOGPIXELSY ", m_pZSubtask->m_pZPrintout->m_nPixelsPerInchY );
      TraceLineI( "DeviceCaps HORZRES ", m_pZSubtask->m_pZPrintout->m_lHorzRes );
      TraceLineI( "DeviceCaps VERTRES ", m_pZSubtask->m_pZPrintout->m_lVertRes );
      TraceLineI( "DeviceCaps PHYSICALWIDTH ", pDC->GetDeviceCaps( PHYSICALWIDTH ) );
      TraceLineI( "DeviceCaps PHYSICALHEIGHT ", pDC->GetDeviceCaps( PHYSICALHEIGHT ) );
      TraceLineI( "DeviceCaps PHYSICALOFFSETX ", m_pZSubtask->m_pZPrintout->m_nPixelsOffsetX );
      TraceLineI( "DeviceCaps PHYSICALOFFSETY ", m_pZSubtask->m_pZPrintout->m_nPixelsOffsetY );
      TraceLineI( "m_pZSubtask->m_pZPrintout->m_lHorzRes = ", m_pZSubtask->m_pZPrintout->m_lHorzRes );
      TraceLineI( "m_pZSubtask->m_pZPrintout->m_lVertRes = ", m_pZSubtask->m_pZPrintout->m_lVertRes );
      TraceLineS( "--------------------", "" );
      TraceLineI( "DeviceCaps DRIVERVERSION ", pDC->GetDeviceCaps( DRIVERVERSION ) );
      TraceLineI( "DeviceCaps TECHNOLOGY ", pDC->GetDeviceCaps( TECHNOLOGY ) );
      TraceLineI( "DeviceCaps HORZSIZE ", pDC->GetDeviceCaps( HORZSIZE ) );
      TraceLineI( "DeviceCaps VERTSIZE ", pDC->GetDeviceCaps( VERTSIZE ) );
      TraceLineI( "DeviceCaps TEXTCAPS ", pDC->GetDeviceCaps( TEXTCAPS ) );
      TraceLineI( "DeviceCaps CLIPCAPS ", pDC->GetDeviceCaps( CLIPCAPS ) );
      TraceLineI( "DeviceCaps COLORRES ", pDC->GetDeviceCaps( COLORRES ) );
      TraceLineI( "DeviceCaps SCALINGFACTORX ", pDC->GetDeviceCaps( SCALINGFACTORX ) );
      TraceLineI( "DeviceCaps SCALINGFACTORY ", pDC->GetDeviceCaps( SCALINGFACTORY ) );
      TraceLineS( "=====================", "" );
   }

   m_pZSubtask->m_pZPrintout->m_lHorzSize256ths =
         (m_pZSubtask->m_pZPrintout->m_lHorzRes * 256) / m_pZSubtask->m_pZPrintout->m_nPixelsPerInchX;
   m_pZSubtask->m_pZPrintout->m_lVertSize256ths =
         (m_pZSubtask->m_pZPrintout->m_lVertRes * 256) / m_pZSubtask->m_pZPrintout->m_nPixelsPerInchY;

   // Reset the mapping mode.
   pDC->SetMapMode( nOldMapMode );

   if ( m_pZSubtask && m_pZSubtask->m_pZPrintout && m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine )
   {
      (*(m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine))
                   ( m_pZSubtask->m_vDialog,
                     pPrintInfo->m_bPreview ? -2 : -1 /* flag for BeginPrint */,
                     m_pZSubtask->m_pZPrintout->m_lHorzSize256ths,
                     m_pZSubtask->m_pZPrintout->m_lVertSize256ths );
   }
}

void
ZDrView::OnPrepareDC( CDC *pDC, CPrintInfo *pPrintInfo )
{
   ASSERT_VALID( pDC );
   UNUSED( pDC ); // unused in release builds

   // Default to printing one page if doc length is not known.
   if ( pPrintInfo )
   {
      if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
         TraceLineI( "ZDrView::OnPrepareDC NumPreviewPages: ", pPrintInfo->m_nNumPreviewPages );

      if ( m_pZSubtask && m_pZSubtask->m_pZPrintout && m_pZSubtask->m_pZPrintout->m_uMaxPageNbr >= pPrintInfo->m_nCurPage )
      {
         pPrintInfo->m_bContinuePrinting = TRUE;
      }
      else
      {
         pPrintInfo->m_bContinuePrinting = FALSE;
      }
   }
}

void
ZDrView::OnPrint( CDC *pDC, CPrintInfo *pPrintInfo )
{
// if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
//    TraceLineI( "ZDrView::OnPrint ZoomState: ", m_nZoomState );

   if ( m_pZSubtask && m_pZSubtask->m_pZPrintout )
   {
      CWaitCursor wait;
      zLONG  lCurPage;
      zSHORT nRC;
      zSHORT nOldMapMode;

      m_pZSubtask->m_pZPrintout->m_pPrintInfo = pPrintInfo;
//    if ( m_pZSubtask->m_pZPrintout->m_bPrintPreview )
//       nOldMapMode = pDC->SetMapMode( MM_TEXT );
//    else
         nOldMapMode = pDC->SetMapMode( MM_HIENGLISH );

      if ( m_pZSubtask->m_pZPrintout->m_lPageCnt < (zLONG) pPrintInfo->m_nCurPage )
         m_pZSubtask->m_pZPrintout->m_lPageCnt = pPrintInfo->m_nCurPage;

      // Determine the page count here if requested.
      if ( m_pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PREPAGE )
      {
         lCurPage = pPrintInfo->m_nCurPage;
         pPrintInfo->m_nCurPage = 0;
         m_pZSubtask->m_pZTask->m_uAppState &= ~zTASK_STATE_PREPAGE;
      }
      else
      if ( m_pZSubtask->m_pZPrintout->m_bPrintToCtrl )
      {
         m_pZSubtask->m_pZPrintout->m_lVertSize256ths = 0x7FFFFFFF;
      }
      else
      {
         lCurPage = -1;
      }

      do
      {
      // TraceLineI( "ZDrView::OnPrint NumPreviewPages: ",
      //             pPrintInfo->m_nNumPreviewPages );
         nRC = (*(m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine))
                    ( m_pZSubtask->m_vDialog, pPrintInfo->m_nCurPage,
                      m_pZSubtask->m_pZPrintout->m_lHorzSize256ths,
                      m_pZSubtask->m_pZPrintout->m_lVertSize256ths );

         if ( lCurPage >= 0 )
         {
            pPrintInfo->m_nCurPage = lCurPage;
            lCurPage = -1;
         }
         else
            break;

      } while ( TRUE );

      if ( nRC > 0 )
      {
         m_pZSubtask->m_pZPrintout->m_uMaxPageNbr = pPrintInfo->m_nCurPage + 1;

         // If we set this when bPreview is TRUE, it will Disable the
         // "Prev" button in print preview.  If it is set when not in
         // preview mode, the printing of all pages is synchronous, otw,
         // the printing is asynchronous.  Since we want synchronous
         // printing, we set the minimum page to be the current page
         // when print preview is turned off.
         if ( pPrintInfo->m_bPreview == FALSE )
            pPrintInfo->SetMinPage( m_pZSubtask->m_pZPrintout->m_uMaxPageNbr );
      }
      else
      {
         m_pZSubtask->m_pZPrintout->m_uMaxPageNbr = pPrintInfo->m_nCurPage;
      }

      if ( nRC <= 0 || pPrintInfo->GetMaxPage( ) < m_pZSubtask->m_pZPrintout->m_uMaxPageNbr )
      {
         pPrintInfo->SetMaxPage( m_pZSubtask->m_pZPrintout->m_uMaxPageNbr );
      }

      // Reset the mapping mode.
      pDC->SetMapMode( nOldMapMode );
   }
   else
   if ( pPrintInfo )
      pPrintInfo->m_bContinuePrinting = FALSE;
}

void
ZDrView::OnEndPrinting( CDC *pDC, CPrintInfo *pPrintInfo )
{
   // TODO: add cleanup after printing
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineI( "ZDrView::OnEndPrinting m_bPreview: ", pPrintInfo->m_bPreview );

   if ( m_pZSubtask && m_pZSubtask->m_pZPrintout &&
        m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine )
   {
      m_pZSubtask->m_pZPrintout->m_pPrintInfo = pPrintInfo;
      (*(m_pZSubtask->m_pZPrintout->m_lpfnPrintPageRoutine))
                   ( m_pZSubtask->m_vDialog,
                     pPrintInfo->m_bPreview ? -4 : -3 /* flag for EndPrint */,
                     m_pZSubtask->m_pZPrintout->m_lHorzSize256ths,
                     m_pZSubtask->m_pZPrintout->m_lVertSize256ths );
   }

   if ( m_pZSubtask )
      mDeleteInit( m_pZSubtask->m_pZPrintout );
}

void
ZDrView::OnDestroy( )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineI( "ZDrView::OnDestroy IdNbr: ", m_ulViewIdNbr );

   if ( m_pZSubtask )
   {
      if ( m_pZSubtask->m_pZFWnd )
         ((ZDrFrame *) m_pZSubtask->m_pZFWnd)->m_pZView = 0;

      m_pZSubtask->m_pZView = 0;
   }

   CFormView::OnDestroy( );

   // Deactivate the item on destruction; this is important when a splitter
   // view is being used.
   COleDocument *pDocument = GetDocument( );
   if ( pDocument )
   {
      COleClientItem *pActiveItem = pDocument->GetInPlaceActiveItem( this );
      if ( pActiveItem && pActiveItem->GetActiveView( ) == this )
      {
         pActiveItem->Deactivate( );
         ASSERT( pDocument->GetInPlaceActiveItem( this ) == 0 );
      }
   }
}

int
ZDrView::OnMouseActivate( CWnd *pDesktopWnd, UINT nHitTest, UINT message )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrView::OnMouseActivate", "" );
#endif
   if ( m_pZSubtask && (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) )
   {
   // CWnd *pWnd = CWnd::GetFocus( );  always zero
   // pWnd = CWnd::GetActiveWindow( ); always zero
      ZSubtask *pZSubtaskMain = m_pZSubtask->GetSplitterMainSubtask( );
      if ( pZSubtaskMain == 0 )
         pZSubtaskMain = m_pZSubtask;

      if ( pZSubtaskMain->m_pZFWnd == pDesktopWnd )  // me thinks this is always true
      {
         // If there is an InPlaceEdit, don't let the focus change.
         if ( m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_INPLACEEDIT )  // dks 2007.04.19  to let focus occur more often
                                                                      // otw, clicking on "rear" window does not always activate
         {
            return( MA_NOACTIVATE );  // dks 2007.01.30  for Grid problem with InPlaceEdit erroneously
                                      // losing focus. Note that returning MA_NOACTIVATEANDEAT is too much.
         }
      }
   }

   int nResult = CWnd::OnMouseActivate( pDesktopWnd, nHitTest, message );
   if ( nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT )
      return( nResult );   // frame does not want to activate
   else
      return( MA_ACTIVATE );
}

/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL
ZDrView::IsSelected( const CObject *pDocItem ) const
{
   // The implementation below is adequate if your selection consists of
   // only ZDrOleClient objects.  To handle different selection mechanisms,
   // the implementation here should be replaced.

   // TODO: implement this function that tests for a selected OLE client item

   return( pDocItem == m_pSelection );
}

void
ZDrView::OnInsertObject( )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrView::OnInsertObject", "" );

#if 0

   // Invoke the standard Insert Object dialog box to obtain information
   // for new ZDrOleClient object.
   COleInsertDialog dlg;
   if ( dlg.DoModal( ) != IDOK )
      return;

   BeginWaitCursor( );

   ZDrOleClient *pItem = 0;
   TRY
   {
      // Create new item connected to this document.
      ZDrDoc *pDoc = GetDocument( );
      ASSERT_VALID( pDoc );
      pItem = new ZDrOleClient( pDoc );
      ASSERT_VALID( pItem );

      // Initialize the item from the dialog data.
      if ( dlg.CreateItem( pItem ) == 0 )
         AfxThrowMemoryException( ); // any exception will do
      ASSERT_VALID( pItem );

      // If item created from class list (not from file) then launch
      // the server to edit the item.
      if ( dlg.GetSelectionType( ) == COleInsertDialog::createNewItem )
         pItem->DoVerb( OLEIVERB_SHOW, this );

      ASSERT_VALID( pItem );

      // As an arbitrary user interface design, this sets the selection
      // to the last item inserted.

      // TODO: reimplement selection as appropriate for your application

      m_pSelection = pItem;   // set selection to last inserted item
      pDoc->UpdateAllViews( 0 );
   }

   CATCH( CException, e )
   {
      if ( pItem )
      {
         ASSERT_VALID( pItem );
         pItem->Delete( );
      }

      AfxMessageBox( IDP_FAILED_TO_CREATE );
   }

   END_CATCH

   EndWaitCursor( );

#endif
}

// The following command handler provides the standard keyboard
// user interface to cancel an in-place editing session.  Here,
// the container (not the server) causes the deactivation.
void
ZDrView::OnCancelEditCntr( )
{
   // Close any in-place active item on this view.
   COleClientItem *pActiveItem = GetDocument( )->GetInPlaceActiveItem( this );
   if ( pActiveItem )
   {
      pActiveItem->Close( );
   }

   ASSERT( GetDocument( )->GetInPlaceActiveItem( this ) == 0 );
}

// Special handling of OnSetFocus and OnSize are required for a container
// when an object is being edited in-place.
void
ZDrView::OnSetFocus( CWnd *pOldWnd )
{
   COleDocument *pDocument = GetDocument( );
   if ( pDocument )
   {
      COleClientItem *pActiveItem = pDocument->GetInPlaceActiveItem( this );
      if ( pActiveItem &&
           pActiveItem->GetItemState( ) == COleClientItem::activeUIState )
      {
         // Need to set focus to this item if it is in the same view.
         CWnd *pWnd = pActiveItem->GetInPlaceWindow( );
         if ( pWnd )
         {
            pWnd->SetFocus( );  // don't call the base class
            return;
         }
      }
   }

   CFormView::OnSetFocus( pOldWnd );
}

void
ZDrView::OnSize( UINT uSizeType, int cx, int cy )
{
   CFormView::OnSize( uSizeType, cx, cy );

#if 0
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
   {
      TraceLine( "ZDrView::OnSize %s  cx: %d   cy: %d",
                 *(m_pZSubtask->m_pzsWndTag), cx, cy );
   }
#endif

   if ( m_pZSubtask == 0 ||
        (ZSubtask::GetSubtaskState( m_pZSubtask ) &
                                         zSUBTASK_STATE_SPLIT_IN_PROCESS) ||
        (ZSubtask::GetSubtaskState( m_pZSubtask ) &
                                         zSUBTASK_STATE_DELETED) )
   {
      return;
   }

   // Inform all controls that a WM_SIZE has occurred.
   if ( m_pZSubtask->m_pZMIXCtrl &&
        (uSizeType == SIZEFULLSCREEN || uSizeType == SIZENORMAL) )
   {
      CRect rect;
      GetClientRect( rect );
//    TraceRect( "ZDrView::OnSize client:", rect );

//    CSize size( cx, cy );
      CSize size( rect.right - rect.left, rect.bottom - rect.top );
//    TraceSize( "ZDrFrame::OnSize:", size );

      m_pZSubtask->SendParentResize( 0, &size );
   }

#if 1
   if ( m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SCROLLBARS )
      SetScrollSizes( MM_TEXT,
                      CSize( m_pZSubtask->m_lExtentX, m_pZSubtask->m_lExtentY ),
                      CSize( cx, cy ), CSize( 32, 16 ) );
   else
      SetScrollSizes( MM_TEXT, CSize( 0, 0 ) );
#else

   SCROLLINFO si;

   si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
   si.nMin = 0;
   if ( cx < m_pZSubtask->m_lExtentX )
   {
      si.nMax = m_pZSubtask->m_lExtentX;
      m_lPageSizeX = cx;
      m_lScrollPosX = min( m_lScrollPosX, si.nMax - m_lPageSizeX );
   }
   else
   {
      si.nMax = 0;
      m_lPageSizeX = 0;
      m_lScrollPosX = 0;
   }

   si.nPos = m_lScrollPosX;
   si.nPage = m_lPageSizeX;
   SetScrollInfo( SB_HORZ, &si, TRUE );

   si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
   si.nMin = 0;
   if ( cy < m_pZSubtask->m_lExtentY )
   {
      si.nMax = m_pZSubtask->m_lExtentY;
      m_lPageSizeY = cy;
      m_lScrollPosY = min( m_lScrollPosY, si.nMax - m_lPageSizeY );
   }
   else
   {
      si.nMax = 0;
      m_lPageSizeY = 0;
      m_lScrollPosY = 0;
   }

   si.nPos = m_lScrollPosY;
   si.nPage = m_lPageSizeY;
   SetScrollInfo( SB_VERT, &si, TRUE );

// ScrollToDevicePosition( CPoint( m_lScrollPosX, m_lScrollPosY ) );
#endif

   COleDocument *pDocument = GetDocument( );
   if ( pDocument )
   {
      COleClientItem *pActiveItem = pDocument->GetInPlaceActiveItem( this );
      if ( pActiveItem )
         pActiveItem->SetItemRects( );
   }
}

void
ZDrView::OnHScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
// TraceLineI( "ZDrView::OnHScroll: ", m_ulViewIdNbr );
#if 1
   CFormView::OnHScroll( uSBCode, uThumbPos, pScrollBar );
#else
   int nDelta;
   int nMaxPos = m_pZSubtask->m_lExtentX - m_lPageSizeX;

   switch ( uSBCode )
   {
      case SB_LINELEFT:
         if ( m_lScrollPosX <= 0 )
            return;

         nDelta = -(min( 32, m_lScrollPosX ));
         break;

      case SB_PAGELEFT:
         if ( m_lScrollPosX <= 0 )
            return;

         nDelta = -(min( m_lPageSizeX, m_lScrollPosX ));
         break;

      case SB_THUMBTRACK:
         CScrollView::OnHScroll( uSBCode, uThumbPos, pScrollBar );
         return;

      case SB_THUMBPOSITION:
         nDelta = (int) uThumbPos - m_lScrollPosX;
         break;

      case SB_PAGERIGHT:
         if ( m_lScrollPosX >= nMaxPos )
            return;

         nDelta = min( m_lPageSizeX, nMaxPos - m_lScrollPosX );
         break;

      case SB_LINERIGHT:
         if ( m_lScrollPosX >= nMaxPos )
            return;

         nDelta = min( 32, nMaxPos - m_lScrollPosX );
         break;

      default: // Ignore other scroll bar messages
         return;
   }

   m_lScrollPosX += nDelta;
   SetScrollPos( SB_HORZ, m_lScrollPosX, TRUE );
   ScrollWindow( -nDelta, 0 );
#endif
}

void
ZDrView::OnVScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
// TraceLineI( "ZDrView::OnVScroll: ", m_ulViewIdNbr );
#if 1
   CFormView::OnVScroll( uSBCode, uThumbPos, pScrollBar );
#else
   int nDelta;
   int nMaxPos = m_pZSubtask->m_lExtentY - m_lPageSizeY;

   switch ( uSBCode )
   {
      case SB_LINEUP:
         if ( m_lScrollPosY <= 0 )
            return;

         nDelta = -(min( 16, m_lScrollPosY ));
         break;

      case SB_PAGEUP:
         if ( m_lScrollPosY <= 0 )
            return;

         nDelta = -(min( m_lPageSizeY, m_lScrollPosY ));
         break;

      case SB_THUMBTRACK:
         CScrollView::OnVScroll( uSBCode, uThumbPos, pScrollBar );
         return;

      case SB_THUMBPOSITION:
         nDelta = (int) uThumbPos - m_lScrollPosY;
         break;

      case SB_PAGEDOWN:
         if ( m_lScrollPosY >= nMaxPos )
            return;

         nDelta = min( m_lPageSizeY, nMaxPos - m_lScrollPosY );
         break;

      case SB_LINEDOWN:
         if ( m_lScrollPosY >= nMaxPos )
            return;

         nDelta = min( 16, nMaxPos - m_lScrollPosY );
         break;

      default: // Ignore other scroll bar messages
         return;
   }

   m_lScrollPosY += nDelta;
   SetScrollPos( SB_VERT, m_lScrollPosY, TRUE );
   ScrollWindow( 0, -nDelta );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrView diagnostics
//
#ifdef _DEBUG
void
ZDrView::AssertValid( ) const
{
   CFormView::AssertValid( );
}

void
ZDrView::Dump( CDumpContext& dc ) const
{
   CFormView::Dump( dc );
}

#endif //_DEBUG

ZDrDoc *
ZDrView::GetDocument( )
{
//   ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( ZDrDoc ) ) );
//   return( (ZDrDoc *) m_pDocument );
   return( (ZDrDoc *) CFormView::GetDocument( ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrView painting

void
ZDrView::Repaint( ZMapAct *pzmaParent )  // force repaint of top level controls
{
   ZMapAct *pzma;
   zSHORT  k = 1;

// zBOOL bLocked = LockWindowUpdate( );

   while ( m_pZSubtask->m_pZMIXCtrl &&
           (pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( k++ )) != 0 )
   {
      if ( pzma && pzma->m_pzmaComposite == pzmaParent &&
           pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
      {
         pzma->m_pCtrl->Invalidate( );
      // pzma->m_pCtrl->RedrawWindow( 0, 0, RDW_ERASENOW | RDW_UPDATENOW |
      //                                      RDW_INTERNALPAINT );
      // Repaint( pzma );
      }
   }

// if ( bLocked )
//    UnlockWindowUpdate( );

// Invalidate( FALSE );
}

void
ZDrView::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZDrView::OnPaint", "" );
#endif
// CPaintDC( this );
   CFormView::OnPaint( );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrView drawing
void
ZDrView::OnDraw( CDC *pDC )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZDrView::OnDraw", "" );
#endif
   ZDrDoc *pDoc = GetDocument( );
   ASSERT_VALID( pDoc );

   // TODO: add draw code for native data here
   // TODO: also draw all OLE items in the document

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDCPaint;
   zBOOL    bMemDC = FALSE;
   CDC      memDC;
   CBitmap  bmp;
   CBitmap  *pOldBmp = 0;
   CRect    rectClient;

   GetClientRect( &rectClient );

   if ( m_pZSubtask &&
        m_pZSubtask->m_pZTask->m_nOS_Version >= 35 &&  // NT/2000/XP
        memDC.CreateCompatibleDC( pDC ) &&
        bmp.CreateCompatibleBitmap( pDC, rectClient.Width( ),
                                    rectClient.Height( ) ) )
   {
      // Off-screen DC successfully created. Paint to it!
      bMemDC = TRUE;
      pOldBmp = memDC.SelectObject( &bmp );
      pDCPaint = &memDC;
   }
   else
      pDCPaint = pDC;

// pDCPaint->FillRect( rectClient, &CBrush( ::GetSysColor( COLOR_BTNFACE ) ) );
// pDCPaint->FillRect( rectClient, &CBrush( pDC->GetBkColor( ) ) );

#if 0    // handling OnPrint instead

   if ( m_pSelection == 0 )
   {
      POSITION pos = pDoc->GetStartPosition( );
      m_pSelection = (ZDrOleClient *) pDoc->GetNextClientItem( pos );
   }

   if ( m_pSelection )
      m_pSelection->Draw( pDCPaint, CRect( 10, 10, 210, 210 ) );

#endif

// CFormView::OnDraw( pDCPaint );  // necessary ???

   // Draw the selection at an arbitrary position.  This code should be
   // removed once your real drawing code is implemented.  This position
   // corresponds exactly to the rectangle returned by ZDrOleClient,
   // to give the effect of in-place editing.

   // Redraw each client item as long as it is a child of the current view.
   if ( pDoc )
   {
      POSITION pos = pDoc->GetStartPosition( );
      while ( pos )
      {
         ZDrOleClient *pItem = (ZDrOleClient *) pDoc->GetNextClientItem( pos );
         if ( pItem->GetParentView( ) == this )
         {
         // if ( pDCPaint->RectVisible( pItem->GetRect( ) ) )
            {
               pItem->Draw( pDCPaint, pItem->GetRect( ) ); // Draw Item itself

               CRectTracker rt;
               pItem->SetupTracker( rt );
               rt.Draw( pDCPaint ); // draw surrounding rectangle
            }
         }
      }
   }

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
   // CRect rect( rectClient );
      CRect rect;
   // pDC->GetClipBox( rect );  // need to do this if using memory DC,
                                // otw rect is uninitialized!!!
                                // Could be initialized by rectClient (DKS)
      pDC->BitBlt( rect.left, rect.top, rect.Width( ), rect.Height( ),
                   &memDC, rect.left, rect.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }
}

BOOL
ZDrView::OnEraseBkgnd( CDC *pDC )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZDrView::OnEraseBkgnd", "" );
#endif

   CRect rect;
// GetClientRect( rect );
   pDC->GetClipBox( &rect );     // Erase the area needed.
// TraceRect( "ZDrView::OnEraseBkgnd", rect );

   CFormView::OnEraseBkgnd( pDC );

// if ( m_pZSubtask &&
//      m_pZSubtask->m_pZTask->m_nOS_Version >= 35 ) // NT/2000/XP
   {
      // Set brush to desired background color.
      // Save old brush.
      CBrush brush;
      brush.CreateSysColorBrush( COLOR_BTNFACE );

      // Select the brush into the device context.
      CBrush *pOldBrush = (CBrush *) pDC->SelectObject( &brush );

   // if ( m_pBrushBk )
   // {
   //    m_pBrushBk->UnrealizeObject( );
   //    pDC->SetBrushOrg( 0, 0 );
   //    pOldBrush = pDC->SelectObject( m_pBrushBk );
   // }

      pDC->PatBlt( rect.left, rect.top, rect.Width( ),
                   rect.Height( ), PATCOPY );

      if ( pOldBrush )
         pDC->SelectObject( pOldBrush );
   }
#ifndef zSIMILAR_VIEWCLASS
   else
   if ( m_pZSubtask )
   {
      if ( m_pBrushBk )
         pDC->FillRect( rect, m_pBrushBk );
      else
         pDC->FillRect( rect, &CBrush( m_pZSubtask->m_clrBk ) );
   }
   else
      pDC->FillRect( rect, &CBrush( ::GetSysColor( COLOR_BTNFACE ) ) );
#endif

   return( TRUE );   // the view will erase/paint itself
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

HBRUSH
ZDrView::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlColor )
{
#if 0
   switch ( uCtlColor )
   {
      case CTLCOLOR_EDIT:
         pDC->SetBkColor( m_clrEdit );
         pDC->SetTextColor( m_clrText );
         return( m_BrushEdit );

      case CTLCOLOR_LISTBOX:
         return( m_BrushListBox );

      case CTLCOLOR_SCROLLBAR:
         return( m_BrushListBox );

      // other cases you can play with:
   // case CTLCOLOR_DLG:    // Dialog box
   // case CTLCOLOR_MSGBOX: // Message box
   // case CTLCOLOR_STATIC: // Static control
   // case CTLCOLOR_BTN:    // Button control (I don't think this case works
   //                                          on Win9x, or NT4+)
   }
#endif

   if ( uCtlColor == CTLCOLOR_EDIT ||
        uCtlColor == CTLCOLOR_LISTBOX ) //||
     // uCtlColor == CTLCOLOR_BTN )
   {
      return( CFormView::OnCtlColor( pDC, pWnd, uCtlColor ) );
   }

   if ( m_pZSubtask && m_pZSubtask->m_clrBk )
      pDC->SetBkColor( m_pZSubtask->m_clrBk );

// pDC->SetTextColor( m_clrText );
   return( *m_pBrushBk );
}

/////////////////////////////////////////////////////////////////////////////

#if 0
How do I change the background color of a view?
To change the background color for a CView, CFrameWnd, or CWnd object,
process the WM_ERASEBKGND message. The following code shows how:

I solved the problem like this:

HBRUSH
ZDrView::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlColor )
{
   switch ( uCtlColor )
   {
      case CTLCOLOR_BTN:
      case CTLCOLOR_STATIC:
      {
         pDC->SetBkMode( TRANSPARENT );
      }

      case CTLCOLOR_DLG:
      {
         CBrush *back_brush;
         back_brush = new CBrush( ::GetSysColor( COLOR_BTNFACE ) );
         return( (HBRUSH) back_brush->m_hObject );
      }
   }

   return( CFormView::OnCtlColor( pDC, pWnd, nCtlColor ) );
}

/////////////////////////////////////////////////////////////////////////////

#endif

void
ZDrView::OnLButtonDown( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnLButtonDown", "" );
   CFormView::OnLButtonDown( uModKeys, pt );

   if ( m_pZSubtask &&
        (m_pZSubtask->m_ulWndOper & zWND_EVENT_LBUTTON_DOWN) ==
                                                  zWND_EVENT_LBUTTON_DOWN &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType,
                                       zWND_EVENT_LBUTTON_DOWN, 0 )
                                                   > zCURSOR_UNCHANGED )
   {
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                              zACTION_DIALOG_CODE_ONLY, TRUE,
                              "OnLButtonDown" ) > 0 )
      {
         return;
      }
   }
}

void
ZDrView::OnLButtonUp( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnLButtonUp", "" );
   CFormView::OnLButtonUp( uModKeys, pt );

   if ( m_pZSubtask )
   {
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_LEFT) ==
                                                       zMOUSE_CAPTURE_LEFT )
      {
         if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG_LEFT) ==
                                                         zMOUSE_DRAG_LEFT &&
              SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                             szlWndEvent, szlType,
                                             zWND_EVENT_LBUTTON_DRAG_DROP, 0 )
                                                        > zCURSOR_UNCHANGED )
         {
            if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                    zACTION_DIALOG_CODE_ONLY, TRUE,
                                    "OnLButtonDragDrop" ) > 0 )
            {
               m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
               return;
            }
         }
      }

      m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_LBUTTON_CLICK) ==
                                                  zWND_EVENT_LBUTTON_CLICK )
      {
         CRect rect;

         GetClientRect( rect );
         if ( rect.PtInRect( pt ) &&
              SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                             szlWndEvent, szlType,
                                             zWND_EVENT_LBUTTON_CLICK, 0 )
                                                         > zCURSOR_UNCHANGED )
         {
            if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                    zACTION_DIALOG_CODE_ONLY, TRUE,
                                    "OnLButtonClick" ) > 0 )
            {
               return;
            }
         }
      }

      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_LBUTTON_UP) ==
                                                    zWND_EVENT_LBUTTON_UP &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                          szlWndEvent, szlType,
                                          zWND_EVENT_LBUTTON_UP, 0 )
                                                      > zCURSOR_UNCHANGED )
      {
         if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                 zACTION_DIALOG_CODE_ONLY, TRUE,
                                 "OnLButtonUp" ) > 0 )
         {
            return;
         }
      }
   }
}

void
ZDrView::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnLButtonDblClk", "" );
   CFormView::OnLButtonDblClk( uModKeys, pt );

   if ( m_pZSubtask )
   {
      m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_LBUTTON_DBLCLK) ==
                                                 zWND_EVENT_LBUTTON_DBLCLK &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                          szlWndEvent, szlType,
                                          zWND_EVENT_LBUTTON_DBLCLK, 0 )
                                                        > zCURSOR_UNCHANGED )
      {
         m_pZSubtask->m_ZMsg.wParam = uModKeys;
         m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
         if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                 zACTION_DIALOG_CODE_ONLY, TRUE,
                                 "OnLButtonDblClk" ) > 0 )
         {
            return;
         }
      }
   }
}

void
ZDrView::OnMButtonDown( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnMButtonDown", "" );
   CFormView::OnMButtonDown( uModKeys, pt );

   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENTX_MBUTTON_DOWN) ==
                                                  zWND_EVENTX_MBUTTON_DOWN &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType,
                                       zWND_EVENTX_MBUTTON_DOWN, 0 )
                                                       > zCURSOR_UNCHANGED )
   {
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                              zACTION_DIALOG_CODE_ONLY, TRUE,
                              "OnMButtonDown" ) > 0 )
      {
         return;
      }
   }
}

void
ZDrView::OnMButtonUp( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnMButtonUp", "" );
   CFormView::OnMButtonUp( uModKeys, pt );

   if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_RIGHT) ==
                                                      zMOUSE_CAPTURE_RIGHT )
   {
      ReleaseCapture( );
      if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG_RIGHT) ==
                                                        zMOUSE_DRAG_RIGHT &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                          szlWndEvent, szlType,
                                          zWND_EVENTX_MBUTTON_DRAG_DROP, 0 )
                                                       > zCURSOR_UNCHANGED )
      {
         if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                 zACTION_DIALOG_CODE_ONLY, TRUE,
                                 "OnMButtonDragDrop" ) > 0 )
         {
            m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
            return;
         }
      }
   }

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
   m_pZSubtask->m_ZMsg.wParam = uModKeys;
   m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENTX_MBUTTON_CLICK) ==
                                                  zWND_EVENTX_MBUTTON_CLICK )
   {
      CRect rect;

      GetClientRect( rect );
      if ( rect.PtInRect( pt ) &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent,
                                          szlType, zWND_EVENTX_MBUTTON_CLICK,
                                          0 ) > zCURSOR_UNCHANGED )
      {
         if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                 zACTION_DIALOG_CODE_ONLY, TRUE,
                                 "OnMButtonClick" ) > 0 )
         {
            return;
         }
      }
   }

   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENTX_MBUTTON_UP) ==
                                                    zWND_EVENTX_MBUTTON_UP &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType,
                                       zWND_EVENTX_MBUTTON_UP, 0 )
                                                   > zCURSOR_UNCHANGED )
   {
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                              zACTION_DIALOG_CODE_ONLY, TRUE,
                              "OnMButtonUp" ) > 0 )
      {
         return;
      }
   }
}

void
ZDrView::OnMButtonDblClk( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnLButtonDblClk", "" );
   CFormView::OnMButtonDblClk( uModKeys, pt );

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENTX_MBUTTON_DBLCLK) ==
                                                zWND_EVENTX_MBUTTON_DBLCLK &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType,
                                       zWND_EVENTX_MBUTTON_DBLCLK, 0 )
                                                   > zCURSOR_UNCHANGED )
   {
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                              zACTION_DIALOG_CODE_ONLY, TRUE,
                              "OnMButtonDblClk" ) > 0 )
      {
         return;
      }
   }
}

void
ZDrView::OnRButtonDown( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnRButtonDown", "" );
   CFormView::OnRButtonDown( uModKeys, pt );

   if ( m_pZSubtask &&
        (m_pZSubtask->m_ulWndOper & zWND_EVENT_RBUTTON_DOWN) ==
                                                  zWND_EVENT_RBUTTON_DOWN &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType,
                                       zWND_EVENT_RBUTTON_DOWN, 0 )
                                                   > zCURSOR_UNCHANGED )
   {
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                              zACTION_DIALOG_CODE_ONLY, TRUE,
                              "OnRButtonDown" ) > 0 )
      {
         return;
      }
   }
}

void
ZDrView::OnRButtonUp( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnRButtonUp", "" );
   CFormView::OnRButtonUp( uModKeys, pt );

   if ( m_pZSubtask )
   {
      if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_RIGHT) ==
                                                       zMOUSE_CAPTURE_RIGHT )
      {
         ReleaseCapture( );
         if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG_RIGHT) ==
                                                         zMOUSE_DRAG_RIGHT &&
              SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                             szlWndEvent, szlType,
                                             zWND_EVENT_RBUTTON_DRAG_DROP, 0 )
                                                        > zCURSOR_UNCHANGED )
         {
            if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                    zACTION_DIALOG_CODE_ONLY, TRUE,
                                    "OnRButtonDragDrop" ) > 0 )
            {
               m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
               return;
            }
         }
      }

      m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_RBUTTON_CLICK) ==
                                                   zWND_EVENT_RBUTTON_CLICK )
      {
         CRect rect;

         GetClientRect( rect );
         if ( rect.PtInRect( pt ) &&
              SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                             szlWndEvent, szlType,
                                             zWND_EVENT_RBUTTON_CLICK,
                                             0 ) > zCURSOR_UNCHANGED )
         {
            if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                    zACTION_DIALOG_CODE_ONLY, TRUE,
                                    "OnRButtonClick" ) > 0 )
            {
               return;
            }
         }
      }

      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_RBUTTON_UP) ==
                                                     zWND_EVENT_RBUTTON_UP &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                          szlWndEvent, szlType,
                                          zWND_EVENT_RBUTTON_UP,
                                          0 ) > zCURSOR_UNCHANGED )
      {
         if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                                 zACTION_DIALOG_CODE_ONLY, TRUE,
                                 "OnRButtonUp" ) > 0 )
         {
            return;
         }
      }
   }
}

void
ZDrView::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZDrView::OnLButtonDblClk", "" );
   CFormView::OnRButtonDblClk( uModKeys, pt );

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_RBUTTON_DBLCLK) ==
                                                zWND_EVENT_RBUTTON_DBLCLK &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType,
                                       zWND_EVENT_RBUTTON_DBLCLK, 0 )
                                                   > zCURSOR_UNCHANGED )
   {
      m_pZSubtask->m_ZMsg.wParam = uModKeys;
      m_pZSubtask->m_ZMsg.lParam = (LPARAM) &pt;
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                              zACTION_DIALOG_CODE_ONLY, TRUE,
                              "OnRButtonDblClk" ) > 0 )
      {
         return;
      }
   }
}

void
ZDrView::OnMouseMove( UINT uModKeys, CPoint pt )
{
   zULONG ulSubtaskState;

// TracePoint( "ZDrView::OnMouseMove", pt );
   CFormView::OnMouseMove( uModKeys, pt );
   if ( m_pZSubtask == 0 ||
        ((ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask )) &
                                              zSUBTASK_STATE_DELETED) )
   {
      return;
   }

   CWnd    *pWnd = ChildWindowFromPoint( pt, CWP_SKIPINVISIBLE );
   ZMapAct *pzma = 0;
   ZMapAct *pzmaMin = 0;
   CWnd    *pWndMin = pWnd;

   if ( pWnd && m_pZSubtask && m_pZSubtask->m_pZMIXCtrl )
   {
      CRect   rect;
      pWnd->ClientToScreen( &pt );
      pWnd->GetWindowRect( rect );
      zLONG   lMinX = pt.x - rect.left;
      zLONG   lMinY = pt.y - rect.top;
      zSHORT  k = 1;

      while ( (pzma = (ZMapAct *)
                          m_pZSubtask->m_pZMIXCtrl->GetAt( k++ )) != 0 )
      {
         if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         {
            if ( pzma->m_pCtrl->m_hWnd == pWndMin->m_hWnd )
            {
               if ( pzmaMin == 0 )
                  pzmaMin = pzma;
            }

            pzma->m_pCtrl->GetWindowRect( rect );
            if ( rect.PtInRect( pt ) && ((pt.y - rect.top) < lMinY) ||
                 (pt.y == rect.top && (pt.x - rect.left) < lMinX) )
            {
               pzmaMin = pzma;
            }
         }
      }

      if ( pzmaMin == 0 )
      {
         m_pZSubtask->InformDriverOfFlyBy( 0 );
      }
      else
      {
      // TraceLineS( "ZDrView::OnMouseMove pzmaMin = ", *(pzmaMin->m_pzsTag) );
         m_pZSubtask->InformDriverOfFlyBy( pzmaMin );
      }

      m_pZSubtask->CheckMouseMove( uModKeys, pt );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

HRESULT
ZDrView::OnWindowlessDefWindowMessage( UINT nMessage, WPARAM wParam,
                                       LPARAM lParam, LRESULT *plResult )
{
   // Avoid warnings.
   (void) nMessage;
   (void) wParam;
   (void) lParam;

   const MSG *pMessage = GetCurrentMessage( );
   ASSERT( pMessage->message == nMessage );
   ASSERT( pMessage->wParam == wParam );
   ASSERT( pMessage->lParam == lParam );

   *plResult = Default( );
   return( S_OK );
}

LRESULT
ZDrView::OnCommonFind( WPARAM wParam, LPARAM lParam )
{
   zLONG lResult = 0;
#ifndef zREMOTE_SERVER // this will only work under windows
   zLONG lWndOpr = 0;
   zLONG lDialogEvent = 0;
   zBOOL bHandleDialogEvent = FALSE;
   FINDREPLACE *pActionfr;
   CString csFindWhat, csReplaceWith;

   FINDREPLACE *pfr = (FINDREPLACE *) lParam;
   if ( pfr->Flags & FR_DIALOGTERM ) // Dialog terminated
   {
      return( lResult );
   }
   else
   {
      // Make a copy of find replace structure because it's gone before it
      // can be handled by coded action.
      pActionfr = (FINDREPLACE *) new BYTE[ pfr->lStructSize ];
      csFindWhat = pfr->lpstrFindWhat;
      csReplaceWith = pfr->lpstrReplaceWith;
      memcpy( pActionfr, pfr, pfr->lStructSize );

      pActionfr->lpstrFindWhat =
             csFindWhat.GetBufferSetLength( csFindWhat.GetLength( ) );
      pActionfr->lpstrReplaceWith =
             csReplaceWith.GetBufferSetLength( csReplaceWith.GetLength( ) );

      lDialogEvent = zWND_EVENT_EDIT_FIND;
      lWndOpr = (m_pZSubtask->m_ulWndOper & lDialogEvent);
      if ( lWndOpr == lDialogEvent )
      {
         bHandleDialogEvent = TRUE;
         if ( pfr->Flags & FR_FINDNEXT ) // find next occurrence of string
         {
            lDialogEvent = zWND_EVENT_EDIT_NEXT;
            // close dialog TMV 26.06.2001
            ZFindReplaceDialog *pDlg = (ZFindReplaceDialog *)
                                   ZFindReplaceDialog::GetNotifier( lParam );

            if ( pDlg )
            {
               // Use pDlg as a pointer to the existing FindReplace dlg to
               // call ZFindReplaceDialog member functions.
               if ( pDlg->IsReplace( ) == FALSE )
                  pDlg->EndDialog( IDCANCEL );
            }
            //end close dialog TMV 26.06.2001
         }
         else
         {
            if ( (pfr->Flags & FR_REPLACE) || (pfr->Flags & FR_REPLACEALL) )
            {
               lDialogEvent = zWND_EVENT_EDIT_REPLACE;
            }
         }
      }

      if ( bHandleDialogEvent )
      {
         m_pZSubtask->m_pEventParms = (zPVOID) pActionfr;
         if ( SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog,
                                             szlWndEvent, szlType,
                                             lDialogEvent, 0 )
                                                        > zCURSOR_UNCHANGED )
         {
            ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0,
                               zACTION_DIALOG_CODE_ONLY, TRUE,
                               "OnCommonFind" );   // what happens on error???
         }
      }

      mDeleteInitA( pActionfr );
      lResult = 1;
   }
#endif
   return( lResult );
}

ZDrOleClient *
ZDrView::GetItemWithCapture( ) const
{
   return( m_pItemWithCapture );
}

ZDrOleClient *
ZDrView::GetItemWithFocus( ) const
{
   return( m_pItemWithFocus );
}

HRESULT
ZDrView::OnWindowlessReleaseCapture( ZDrOleClient *pItem )
{
   if ( m_pItemWithCapture == pItem )
   {
      m_pItemWithCapture = 0;
      ReleaseCapture( );
   }

   return( S_OK );
}

HRESULT
ZDrView::OnWindowlessReleaseFocus( )
{
   ASSERT( m_pItemWithFocus );

   m_pItemWithFocus = 0;
   return( S_OK );
}

HRESULT
ZDrView::OnWindowlessSetCapture( ZDrOleClient *pItem )
{
   ASSERT( pItem );
// ASSERT( m_pItemWithCapture == 0 );

   SetCapture( );
   m_pItemWithCapture = pItem;

   return( S_OK );
}

HRESULT
ZDrView::OnWindowlessSetFocus( ZDrOleClient *pItem )
{
   ASSERT( pItem );

   SetFocus( );
   m_pItemWithFocus = pItem;

   return( S_OK );
}

BOOL
ZDrView::OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                            VARIANT *pvar )
{
   ZAmbientProperty *pProp = FindAmbientProperty( dispid );
   BOOL bRC = TRUE;

   if ( pProp == 0 )
   {
      if ( m_bTraceAmbientPropertyRequest )
      {
         TraceLineI( "*** Control requests unknown Ambient Property ",
                     dispid );
      }

      bRC = CWnd::OnAmbientProperty( pSite, dispid, pvar );
   }
   else
   {
      if ( m_bTraceAmbientPropertyRequest )
      {
         TraceLineS( "*** Control requests Ambient Property ",
                     pProp->GetName( ) );
      }

      COleVariant v = pProp->GetValue( );

      if ( dispid == DISPID_AMBIENT_DISPLAYNAME )
      {
         // Get a CWND from COleControlSite *pSite
         CWnd *pCtrlToFind = pSite->m_pWndCtrl;
         ZMapAct *pCtrl = m_pZSubtask->GetMapActFromHWnd( pCtrlToFind->m_hWnd );
         if ( pCtrl )
         {
            // Get the control's tag.
            v = *(pCtrl->m_pzsTag);
         }
      }

      HRESULT hr = VariantCopy( pvar, &v );
      if ( SUCCEEDED( hr ) )
      {
         bRC = TRUE;
      }
      else
      {
         bRC = FALSE;
      }
   }

   return( bRC );
}

void
ZDrView::InitAmbientProperties( )
{
   CString csName;
   LPCOLESTR pszFaceName;
   ZAmbientProperty *pProp;

   csName.LoadString( IDS_APROPNAME_USERMODE );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_USERMODE, csName,
                                 COleVariant( VAR_TRUE, VT_BOOL ),
                                 VTI_BOOL, FALSE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_UIDEAD );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_UIDEAD, csName,
                                 COleVariant( VARIANT_FALSE, VT_BOOL ),
                                 VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_SHOWHATCHING );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_SHOWHATCHING, csName,
                                 COleVariant( VARIANT_BOOL( -1 ), VT_BOOL ),
                                 VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_SHOWGRABHANDLES );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_SHOWGRABHANDLES, csName,
                                 COleVariant( VARIANT_BOOL( -1 ), VT_BOOL ),
                                 VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_TEXTALIGN );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_TEXTALIGN, csName,
                                 COleVariant( short( 0 ), VT_I2 ),
                                 VTI_I2, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_BACKCOLOR );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_BACKCOLOR, csName,
                   COleVariant( long( GetSysColor( COLOR_WINDOW ) ), VT_I4 ),
                   VTI_COLOR, TRUE );
   m_apList.Add( pProp );

   mDeleteInit( m_pFontHolder );
   m_pFontHolder = new CFontHolder( 0 );
   if ( m_pFontHolder == 0 )
   {
     AfxThrowMemoryException( );
   }

   m_descFont.cbSizeofstruct = sizeof( m_descFont );
   pszFaceName = L"MS Sans Serif";
   mDeleteInitA( m_descFont.lpstrName );

   m_descFont.lpstrName = new OLECHAR[ ocslen( pszFaceName ) + 1 ];
   if ( m_descFont.lpstrName == 0 )
   {
      AfxThrowMemoryException( );
   }

   ocscpy( m_descFont.lpstrName, pszFaceName );
   m_descFont.cySize.Lo = 80000L;
   m_descFont.cySize.Hi = 0;
   m_descFont.sWeight = FW_NORMAL;
   m_descFont.sCharset = 0;
   m_descFont.fItalic = FALSE;
   m_descFont.fUnderline = FALSE;
   m_descFont.fStrikethrough = TRUE;
   m_pFontHolder->InitializeFont( &m_descFont );

   COleVariant varFont;
   varFont.vt = VT_DISPATCH;
   varFont.pdispVal = m_pFontHolder->GetFontDispatch( );

   csName.LoadString( IDS_APROPNAME_FONT );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_FONT, csName, varFont,
                                 VTI_FONT, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_FORECOLOR );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_FORECOLOR, csName,
               COleVariant( long( GetSysColor( COLOR_WINDOWTEXT ) ), VT_I4 ),
               VTI_COLOR, TRUE );
   m_apList.Add( pProp );

// csName.LoadString( IDS_APROPNAME_SCALEUNITS );
// pProp = new ZAmbientProperty( DISPID_AMBIENT_SCALEUNITS, csName,
//                               COleVariant( _T( "" ), VT_BSTR ),
//                               VTI_BSTR, TRUE );
// m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_DISPLAYNAME );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_DISPLAYNAME, csName,
                                 COleVariant( _T(""), VT_BSTR ),
                                 VTI_BSTR, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_LOCALEID );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_LOCALEID, csName,
                                 COleVariant( long( GetUserDefaultLCID( ) ),
                                              VT_I4 ),
                                 VTI_I4, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_MESSAGEREFLECT );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_MESSAGEREFLECT, csName,
                                 COleVariant( VARIANT_FALSE, VT_BOOL ),
                                 VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_SUPPORTSMNEMONICS );
   pProp = new ZAmbientProperty( DISPID_AMBIENT_SUPPORTSMNEMONICS, csName,
                                 COleVariant( VARIANT_BOOL( -1 ), VT_BOOL ),
                                 VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, zsizeof( szMsg ), "ZDrView::InitAmbientProperties (0x%08x) PropertyCnt: ", (zULONG) this );
      TraceLineI( szMsg, m_apList.GetSize( ) );
   }
}

ZAmbientProperty *
ZDrView::FindAmbientProperty( DISPID dispid )
{
   ZAmbientProperty *pProp = 0;

   long lMax = m_apList.GetSize( );

   for ( long lCurrent = 0; lCurrent < lMax; lCurrent++ )
   {
      if ( m_apList[ lCurrent ]->GetID( ) == dispid )
      {
         return( m_apList[ lCurrent ] );
      }
   }

   return( 0 );
}

ZAmbientProperty *
ZDrView::FindAmbientProperty( LPCOLESTR pszName )
{
   USES_CONVERSION;
   int iProp;
   LPCTSTR pszNameT;

   pszNameT = OLE2CT( pszName );
   for ( iProp = 0; iProp < m_apList.GetSize( ); iProp++ )
   {
      if ( m_apList[ iProp ]->GetName( ) == pszNameT )
      {
         return( m_apList[ iProp ] );
      }
   }

   // We didn't find the property.
   return( 0 );
}

void
ZDrView::RemoveAmbientProperties( )
{
   ZAmbientProperty *pProp = 0;

   for ( zLONG k = m_apList.GetUpperBound( ); k >= 0; k-- )
   {
      pProp = m_apList[ k ];
      m_apList.RemoveAt( k );
      delete pProp;
   }
}

BOOL
ZDrView::CheckSplitterFocus( MSG *pMsg )
{
   // TraceLine( "ZDrView::PreTranslateMessage %d, %d, %d",
   //            pMsg->message, pMsg->wParam, pMsg->lParam );
   // if ( pMsg->wParam == 85 )
   //    TraceLineI( "ZDrView::PreTranslateMessage FALSE ", pMsg->wParam );
   if ( m_pZSubtask &&
        (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) )
   {
      zBOOL bBackward = (GetKeyState( VK_SHIFT ) < 0);

      if ( fnValidTab( m_pZSubtask, bBackward ) )
      {
         ZSubtask *pZSubtaskMain = m_pZSubtask->GetSplitterMainSubtask( );

         m_pZSubtask->FindFirstLastFocus( FALSE );
         if ( m_pZSubtask->m_pzmaWithFocus &&
              ((m_pZSubtask->m_pzmaWithFocus == m_pZSubtask->m_pzmaFirstFocus &&
                bBackward == TRUE) ||
               (m_pZSubtask->m_pzmaWithFocus == m_pZSubtask->m_pzmaLastFocus &&
               bBackward == FALSE)) )
         {
            if ( bBackward &&
                 m_pZSubtask->m_pZView->m_pPrevView &&
                 m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask )
            {
               m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask->FindFirstLastFocus( FALSE );
               if ( m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask->m_pzmaLastFocus &&
                    m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask->m_pzmaLastFocus->m_pCtrl &&
                    mIs_hWnd( m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask->m_pzmaLastFocus->m_pCtrl->m_hWnd ) )
               {
                  pZSubtaskMain->m_pZFWnd->SetActiveView( m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask->m_pZView );
                  m_pZSubtask->m_pZView->m_pPrevView->m_pZSubtask->m_pzmaLastFocus->m_pCtrl->SetFocus( );
                  return( TRUE );
               }
            }
            else
            if ( bBackward == FALSE &&
                 m_pZSubtask->m_pZView->m_pNextView &&
                 m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask )
            {
               m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask->FindFirstLastFocus( FALSE );
               if ( m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask->m_pzmaFirstFocus &&
                    m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask->m_pzmaFirstFocus->m_pCtrl &&
                    mIs_hWnd( m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask->m_pzmaFirstFocus->m_pCtrl->m_hWnd ) )
               {
                  pZSubtaskMain->m_pZFWnd->SetActiveView( m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask->m_pZView );
                  m_pZSubtask->m_pZView->m_pNextView->m_pZSubtask->m_pzmaFirstFocus->m_pCtrl->SetFocus( );
                  return( TRUE );
               }
            }
         }
      }
      else
      {
         return( TRUE );
      }
   }

   return( FALSE );
 }

BOOL
ZDrView::PreTranslateMessage( MSG *pMsg )
{
#if 0
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN ||
        pMsg->message == WM_EXITSIZEMOVE )
   {
      if ( pMsg->wParam == VK_ESCAPE )
      {
         TraceLineS( "ZDrView::PreTranslateMessage: ", "VK_ESCAPE" );
      }
   }
#endif

   // moved from ZDrFrame::PreTranslateMessage  // 01/29/03 dks
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN )
   {
      ZSubtask *pZSubtask;
   // TraceLine( "ZDrView::PreTranslateMessage %d, %d",
   //            pMsg->wParam, pMsg->lParam );

      if ( pMsg->message == WM_KEYDOWN )
      {
         if ( pMsg->wParam == VK_TAB )
         {
            if ( CheckSplitterFocus( pMsg ) )
               return( TRUE );
         }
      }

      if (pMsg->wParam == VK_ESCAPE ||
          (pMsg->wParam == VK_MENU && (unsigned) (GetKeyState( VK_MENU ) < 0)) )
      {
         pZSubtask = m_pZSubtask->GetSplitterMainSubtask( );
         if ( pZSubtask == 0 )
            pZSubtask = m_pZSubtask;
      }
      else
         pZSubtask = m_pZSubtask;

      if ( fnIsAccelerator( pZSubtask, pMsg ) )
      {
      // TraceLineI( "ZDrView::PreTranslateMessage TRUE ", pMsg->wParam );
         return( TRUE );
      }
   }

   return( CFormView::PreTranslateMessage( pMsg ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define IDM_GETPRINTPREVIEW  206

IMPLEMENT_DYNAMIC( ZMappedBitmapButton, CButton )

/////////////////////////////////////////////////////////////////////////////
// ZMappedBitmapButton

ZMappedBitmapButton::ZMappedBitmapButton( )
{
   m_idResource = (UINT) -1;
}

ZMappedBitmapButton::~ZMappedBitmapButton( )
{
}

BEGIN_MESSAGE_MAP( ZMappedBitmapButton, CButton )
   //{{AFX_MSG_MAP( ZMappedBitmapButton )
   ON_WM_SYSCOLORCHANGE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

BOOL
ZMappedBitmapButton::LoadBitmap( UINT idBitmapResource )
{
   m_image.DeleteImageList( );
   m_idResource = idBitmapResource;

   CBitmap bitImage;
   if ( bitImage.LoadMappedBitmap( idBitmapResource ) == FALSE )
   {
      ASSERT( FALSE ); // failed to load bitmap. Does this resource ID exist?
      return( FALSE );
   }

   BITMAP bm;
   if ( bitImage.GetBitmap( &bm ) == 0 )
      return( FALSE );

   // We don't use a mask for the bitmaps in this case.
   UINT uFlags = 0;
   if ( bm.bmBitsPixel <= 4 )
      uFlags |= ILC_COLOR4;
   else
   if ( bm.bmBitsPixel <= 8 )
      uFlags |= ILC_COLOR8;
   else
   if ( bm.bmBitsPixel <= 16 )
      uFlags |= ILC_COLOR16;
   else
      uFlags |= ILC_COLOR24;

   // image should contain EXACTLY four subimages.
   ASSERT( bm.bmWidth % 4 == 0 );

   if ( m_image.Create( bm.bmWidth / 4, bm.bmHeight, uFlags, 4, 0 ) == 0 )
      return( FALSE );

   // the mask bitmap will be ignored here, so use the bitImage for both.
   if ( m_image.Add( &bitImage, &bitImage ) == -1 )
      return( FALSE );

   return( TRUE );
}

// SizeToContent will resize the button to the size of the bitmap.
void
ZMappedBitmapButton::SizeToContent( )
{
   ASSERT( m_image.GetImageCount( ) == 4 );

   IMAGEINFO info;
   VERIFY( m_image.GetImageInfo( 0, &info ) );
   VERIFY( SetWindowPos( 0, -1, -1, info.rcImage.right, info.rcImage.bottom,
               SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE ) );
}

// Autoload will load the bitmap resources.
BOOL
ZMappedBitmapButton::AutoLoad( UINT nID, CWnd *pParent,
                               UINT idBitmapResource )
{
   // First attach the ZMappedBitmapButton to the dialog control
   if ( SubclassDlgItem( nID, pParent ) == 0 )
      return( FALSE );

   if ( LoadBitmap( idBitmapResource ) == 0 )
      return( FALSE );

   SizeToContent( );
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZMappedBitmapButton message handlers

void
ZMappedBitmapButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
   ASSERT( lpDrawItemStruct );
   ASSERT( m_image.GetImageCount( ) == 4 );

   // Images are:
   // 0 = up
   // 1 = down
   // 2 = focused
   // 3 = disabled
   int image = 0;
   UINT state = lpDrawItemStruct->itemState;
   if ( state & ODS_SELECTED )
      image = 1;
   else
   if ( state & ODS_FOCUS )
      image = 2;   // third image for focused
   else
   if ( state & ODS_DISABLED )
      image = 3;   // last image for disabled

   // Draw the whole button.
   CRect rect;
   rect.CopyRect( &lpDrawItemStruct->rcItem );

   CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
   VERIFY( m_image.Draw( pDC, image, rect.TopLeft( ), ILD_NORMAL ) );
}

void
ZMappedBitmapButton::OnSysColorChange( )
{
   CButton::OnSysColorChange( );

   if ( m_image.GetImageCount( ) == 4 )
   {
      LoadBitmap( m_idResource );
   }

   Invalidate( );
}

IMPLEMENT_DYNCREATE( ZDrPreviewView, CPreviewView )

BEGIN_MESSAGE_MAP( ZDrPreviewView, CPreviewView )
   //{{AFX_MSG_MAP(ZDrPreviewView)
//#ifndef QUINSOFT
   ON_WM_CREATE( )
   ON_WM_SIZE( )
   ON_WM_LBUTTONDOWN( )
   ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
   ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
   ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMIN, OnUpdateZoomIn)
   ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMOUT, OnUpdateZoomOut)
//#endif
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZDrPreviewView::~ZDrPreviewView( )
{
   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
      TraceLineS( "ZDrPreviewView::~ZDrPreviewView dtor", "" );
}

ZDrPreviewView::ZDrPreviewView( )
{
//#ifndef QUINSOFT
   zCHAR szEnableZooming[ MAX_PATH ];

   SysReadZeidonIni( -1, "[Debug]", "UseReportFixZooming", szEnableZooming, zsizeof( szEnableZooming ) );
   if ( szEnableZooming[ 0 ] == 'Y' || szEnableZooming[ 0 ] == 'y' )
   {
      m_bEnableZooming = FALSE;
      m_nZoomState = ZOOM_OUT;
   }
   else
   {
      m_bEnableZooming = TRUE;
//#endif
      m_pZSubtask = ((ZDrApp *) AfxGetApp( ))->m_pZSubtaskCurr;
      m_nZoomState = SysSendOE_Message( IDM_GETPRINTPREVIEW, 0 );
      if ( m_nZoomState == '1' )
         m_nZoomState = ZOOM_MIDDLE;
      else
      if ( m_nZoomState == '2' )
         m_nZoomState = ZOOM_IN;
      else
         m_nZoomState = ZOOM_OUT;
//#ifndef QUINSOFT
   }
//#endif
}

//#ifndef QUINSOFT
void
ZDrPreviewView::OnUpdateZoomIn( CCmdUI *pCmdUI )
{
   if ( m_bEnableZooming )
      pCmdUI->Enable( m_nZoomState != ZOOM_IN );
   else
      pCmdUI->Enable( FALSE );
}

void
ZDrPreviewView::OnUpdateZoomOut( CCmdUI *pCmdUI )
{
   if ( m_bEnableZooming )
      pCmdUI->Enable( m_nZoomState != ZOOM_OUT );
   else
      pCmdUI->Enable( FALSE );
}

void
ZDrPreviewView::OnLButtonDown( UINT lFlags, CPoint pt )
{
   UINT nPage;
   if ( m_bEnableZooming == FALSE ||
        FindPageRect( pt, nPage ) == FALSE ) // didn't click on a page
   {
      return;
   }

   // Set new zoom state.
   SetZoomState( (m_nZoomState == ZOOM_IN) ? ZOOM_OUT : m_nZoomState + 1,
                 nPage, pt );
}

void
ZDrPreviewView::OnDisplayPageNumber( UINT nPage, UINT nPagesDisplayed )
{
   CPreviewView::OnDisplayPageNumber( nPage, nPagesDisplayed );
}

void
ZDrPreviewView::OnPreviewPrint( )
{
   CPreviewView::OnPreviewPrint( );
}

void
ZDrPreviewView::OnSize( UINT nType, int cx, int cy )
{
   CPreviewView::OnSize( nType, cx, cy );
   CRect rect;
   GetWindowRect( &rect );
// MoveWindow( rect, TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrPreviewView drawing

/////////////////////////////////////////////////////////////////////////////
// ZDrPreviewView diagnostics

#ifdef _DEBUG
void
ZDrPreviewView::AssertValid( ) const
{
   CPreviewView::AssertValid( );
}

void
ZDrPreviewView::Dump( CDumpContext& dc ) const
{
   CPreviewView::Dump( dc );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZDrPreviewView message handlers

void
ZDrPreviewView::OnUpdateNumPageChange( CCmdUI *pCmdUI )
{
   UINT nPages = m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages;

   if ( m_bOne )
   {
      if ( nPages == 1 )
      {
         // need to swap to show 2 pages
         m_onetwo.LoadBitmap( IDB_PREV_TWO );
         m_bOne = FALSE;
         m_onetwo.Invalidate( );
      }
   }
   else
   {
      if ( nPages != 1 )
      {
         // need to swap to show 1 page
         m_onetwo.LoadBitmap( IDB_PREV_ONE );
         m_bOne = TRUE;
         m_onetwo.Invalidate( );
      }
   }

   // enable it only if valid to display another page and not zoomed
   pCmdUI->Enable( m_nZoomState == ZOOM_OUT && m_nMaxPages != 1 &&
                   (m_pPreviewInfo->GetMaxPage( ) > 1 || m_nPages > 1) );
}

int
ZDrPreviewView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{

   if ( m_bEnableZooming == FALSE )
   {
      lpCreateStruct->cx = 400;
      lpCreateStruct->cy = 350;
      lpCreateStruct->x  = 0;
      lpCreateStruct->y  = 50;
   }

   if (CPreviewView::OnCreate(lpCreateStruct) == -1)
      return( -1 );

   m_pToolBar->EnableToolTips( TRUE );

   m_bOne = FALSE;  // the default is to show 2 pages, set in the dialog text

   m_print.AutoLoad( AFX_ID_PREVIEW_PRINT, m_pToolBar, IDB_PREV_PRINT );
   m_next.AutoLoad( AFX_ID_PREVIEW_NEXT, m_pToolBar, IDB_PREV_NEXT );
   m_previous.AutoLoad( AFX_ID_PREVIEW_PREV, m_pToolBar, IDB_PREV_PREVIOUS );
   m_onetwo.AutoLoad( AFX_ID_PREVIEW_NUMPAGE, m_pToolBar, IDB_PREV_TWO );
   m_zoomIn.AutoLoad( AFX_ID_PREVIEW_ZOOMIN, m_pToolBar, IDB_PREV_ZOOMIN );
   m_zoomOut.AutoLoad( AFX_ID_PREVIEW_ZOOMOUT, m_pToolBar, IDB_PREV_ZOOMOUT );

   return( 0 );
}
//#endif

IMPLEMENT_SERIAL( ZAmbientProperty, CObject, 1 );

ZAmbientProperty::ZAmbientProperty( ) :
                  m_dispid( 0 ),
                  m_tEnabled( FALSE )
{
}

ZAmbientProperty::ZAmbientProperty( DISPID dispid, LPCTSTR pszName,
                                    const VARIANT& var, int vti,
                                    BOOL tStock ) :
                  m_dispid( dispid ),
                  m_csName( pszName ),
                  m_varValue( var ),
                  m_vti( vti ),
                  m_tEnabled( TRUE ),
                  m_tStock( tStock )
{
}

void
ZAmbientProperty::Enable( BOOL tEnabled )
{
   m_tEnabled = tEnabled;
}

DISPID
ZAmbientProperty::GetID( ) const
{
   return( m_dispid );
}

CString
ZAmbientProperty::GetName( ) const
{
   return( m_csName );
}

const
COleVariant& ZAmbientProperty::GetValue( ) const
{
   return( m_varValue );
}

int
ZAmbientProperty::GetVTI( ) const
{
   return( m_vti );
}

BOOL
ZAmbientProperty::IsEnabled( ) const
{
   return( m_tEnabled );
}

BOOL
ZAmbientProperty::IsStock( ) const
{
   return( m_tStock );
}

void
ZAmbientProperty::SetValue( const VARIANT& varValue, int vti )
{
   m_varValue = varValue;
/*
   if ( vti == -1 )
   {
     m_vti = VTToVTI( varValue.vt );
   }
   else
   {
     ASSERT( VTIToVT( vti ) == varValue.vt );
     m_vti = vti;
   }
*/
}

void
ZAmbientProperty::Serialize( CArchive& ar )
{
   CObject::Serialize( ar );

   if ( ar.IsStoring( ) )
   {
     ar << m_dispid;
     ar << m_csName;
     ar << m_varValue;
     ar << m_vti;
     ar << m_tEnabled;
     ar << m_tStock;
   }
   else
   {
     ar >> m_dispid;
     ar >> m_csName;
     ar >> m_varValue;
     ar >> m_vti;
     ar >> m_tEnabled;
     ar >> m_tStock;
   }
}
