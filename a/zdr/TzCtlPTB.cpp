/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlptb.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Painter PainterBar control (the only control made known to the driver).
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.10    BL    R55735
//    Target Specification: if TargetExecutable not selected, set selection
//    for current Meta
//
// 2001.06.25    DKS    Z10
//    Using Font=ZeidonSize in the [Workstation] .ini setting permits the
//    new fonts while retaining the old Window/Control sizes.
//
// 2001.06.14    PAS
//    Fix crash when 'build dialog' button hit and Report window is closed.
//    Also, make the invalid combos invisible when shown in Report.
//
// 2001.05.18    DKS   R54685
//    Changed algorithm to set the proper font when Workstation Font is
//    set in Zeidon.ini.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.03.19    DKS   Fonts
//    Added FontCtrl to permit override of ctrl font setting.
//
// 2001.02.12    BL    R54697
//    Modified TZPainterBar::TZPainterBar: if PPE view does not exist,
//    activate vTZPNTROO with Subtask View (for improved handling of
//    return code from InitializeLPLR).
//
// 2001.01.05    DKS   Z10
//    Modified Show/Hide Tabstops to retain the setting.
//
// 2000.12.27    BL    Z10    RAD 54317
//    Modified OnControlReflectCommand: call Target Specification without
//    opening any Window
//
// 2000.09.05    BL    Z10
//    Modified CreatePaletteButtons: îf for Toolbar without controls
//
// 2000.09.02    BL    Z10    TB 5100
//    Modified CreatePaletteButtons: Bugfix Toolbar
//    Modified EnablePainterWindows: do not repaint Zeidon Window
//
// 2000.06.05    DKS   Z10
//    Load the Task LPLR view once (performance enhancement).
//
// 2000.03.27    DKS   Z10    TB1013
//    Paint ctrls in the correct position when primary window is scrolled.
//
// 1999.12.09    DKS   Z10    HPF999
//    Override fonts shown at Design time same as at Runtime.
//
// 1999.11.30    TMV   Z10
//    avoid memory leaks
//
// 1999.02.17    DKS
//    Fixed use of new option zLOADLIB_RESOURCES.
//
// 1999.02.16    DKS
//    Used new option zLOADLIB_RESOURCES on SysLoadLibraryWithErrFlag.
//
// 1999.02.01    DKS
//    Implemented support of Abut ctrls.
//
// 1999.02.01    DKS
//    Create of ctrl when scrolled appears in correct position.
//
// 1999.01.27    DKS
//    Consolidated code for deletion of a ctrl in preparation for the
//    implementation of Undo/Redo.
//
// 1998.12.29    DKS
//    Report does not use profile to load tool box of controls.
//
// 1998.09.23    DKS
//    Select active window in WindowList (TB 217)
//
// 1998.08.13    DKS
//    Fix to Tab control acceptance by GroupBox (now warns)
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
// #define zDEBUG_CTRL

IMPLEMENT_DYNAMIC( TZPainterBar, CToolBar )
BEGIN_MESSAGE_MAP( TZPainterBar, CToolBar )
   ON_WM_CREATE( )
   ON_WM_CLOSE( )
   ON_CONTROL_REFLECT( CN_COMMAND, OnControlReflectCommand )
   ON_MESSAGE( zZEIDON_GET_TOOLTIP_TEXT, OnGetToolTipText )
END_MESSAGE_MAP( )

// These id's have to be in the range from AFX_IDW_CONTROLBAR_FIRST + 32 to
// AFX_IDW_CONTROLBAR_FIRST + 256 for ZDR to pass them on to the control.

static UINT g_PainterBar[] =
{
   // Same order as in the bitmap "painter.bmp".
   ID_ACTION_BAR_UPDATE,
      ID_SEPARATOR,
   ID_ALIGN_LEFT,
   ID_ALIGN_TOP,
   ID_ALIGN_RIGHT,
   ID_ALIGN_BOTTOM,
      ID_SEPARATOR,
   ID_SIZE_WIDTH,
   ID_SIZE_HEIGHT,
   ID_SIZE_WIDTH_HEIGHT,
      ID_SEPARATOR,
   ID_EQUAL_SPACE_HORIZONTAL,
   ID_EQUAL_SPACE_VERTICAL,
      ID_SEPARATOR,
   ID_ABUT_HORIZONTAL,
   ID_ABUT_VERTICAL,
      ID_SEPARATOR,
   ID_DELETE_SELECTED,
   ID_EQUAL_SIZE_POS_CLASS,
   ID_GROUP_SELECTED,
   ID_UNGROUP_SELECTED,
      ID_SEPARATOR,
   ID_TOOLBAR_WNDLIST,
//    ID_SEPARATOR,
   ID_TOOLBAR_ACTLIST,
//    ID_SEPARATOR,
   ID_TOOLBAR_OPERLIST,
};

//
// PainterBar ctor, load painter object and painter controls
// definition object and instance.  Then create a toolbar owner draw
// button for each control defined which has a bitmap.
//
TZPainterBar::TZPainterBar( ZSubtask *pZSubtask,
                            CWnd     *pWndParent,
                            ZMapAct  *pzmaComposite,
                            zVIEW    vSubtask,
                            zSHORT   nOffsetX,
                            zSHORT   nOffsetY,
                            zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
              m_wndPaletteBar( this )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::ctor", "" );
#endif

   // Initialize right away in case things don't go right in this ctor.
   m_pActivePainterWindow = 0;
   m_vSubtask = vSubtask;
   m_fState = zTZCB_SCROLLBARS | zTZCB_GRID;     // default to Dialog (not Report or XSLT)
   m_PainterWindowList.SetSize( 8, 4 );
   m_nWndCnt = 0;
   m_vTaskLPLR = 0;
   m_uPaletteItemCnt = 0;
   m_PaletteBarItems = 0;
   m_uSelectedPaletteItem = 0;
   m_pActiveX_CtrlList = 0;
   m_szReportFaceName[ 0 ] = 0;
   m_lReportFontSize = 0;
   m_bMouseSwapped = ::GetSystemMetrics( SM_SWAPBUTTON );
   m_bUseMapDlgUnits = FALSE;
   m_pFontWS = 0;
   m_pFontCtrl = 0;
   m_pZMIXWebCtrl = 0;

   // Set y size based on character height since most controls need
   // character alignment.  We align to the closest character size to
   // the rectangle created.
   m_pZSubtask = pZSubtask;
   CClientDC *hDC = new CClientDC( m_pZSubtask->m_pZFWnd );
   hDC->GetTextMetrics( &TextMetrics );
   delete( hDC );

// LOGFONT   TabFont;
// SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( LOGFONT ), &TabFont, 0 );
// m_fontTab.CreateFontIndirect( &TabFont );
   m_hFontTab = (HFONT) ::GetStockObject( DEFAULT_GUI_FONT );
   if ( m_hFontTab == 0 )
      m_hFontTab = (HFONT) ::GetStockObject( ANSI_VAR_FONT );

//
// Returns: Nonzero if successful; otherwise 0.
//
// Parameters:
//    pWndParent - Pointer to the window that is the toolbar's parent.
//    dwStyle - The toolbar style. Additional toolbar styles supported are:
//       CBRS_TOP - Toolbar is at top of the frame window.
//       CBRS_BOTTOM - Toolbar is at bottom of the frame window.
//       CBRS_NOALIGN - Toolbar is not repositioned when the parent is resized.
//       CBRS_TOOLTIPS - Toolbar displays tool tips.
//       CBRS_SIZE_DYNAMIC - Toolbar is dynamic.
//       CBRS_SIZE_FIXED - Toolbar is fixed.
//       CBRS_FLOATING - Toolbar is floating.
//       CBRS_FLYBY - Toolbar displays information about the button.
//       CBRS_HIDE_INPLACE - Toolbar is not displayed to the user.
//    nID - The toolbar's child-window ID.
//
// Remarks: This member function creates a Windows toolbar (a child window)
//          and associates it with the CToolBar object.  It also sets the
//          toolbar height to a default value.
//

   // Create a control bar button for each painter window action.
   Create( m_pZSubtask->m_pZFWnd,
           WS_CHILD | WS_VISIBLE | CBRS_TOP |
              CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY,
           IDW_PAINTER_BAR );
//            CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY,
//         AFX_IDW_TOOLBAR );
//         ((m_bToolTips) ? (CBRS_TOOLTIPS | CBRS_FLYBY) :0 ),

// TraceLineX( "PainterBar hWnd ", (zLONG) m_hWnd );

   //
   // All the button images in the toolbar are taken from one bitmap, which
   // must contain one image for each button. All images must be the same
   // size; the default is 16 pixels wide and 15 pixels high.  Images must
   // be side by side in the bitmap.
   //
   LoadBitmap( IDW_PAINTER_BAR );

   // Remarks: SetButtons sets each toolbar button's command ID to the value
   // specified by the corresponding element of the array lpIDArray.  If an
   // element of the array has the value ID_SEPARATOR, a separator is created
   // in the corresponding position of the toolbar.  This function also sets
   // each button's style to TBBS_BUTTON and each separator's style to
   // TBBS_SEPARATOR, and assigns an image index to each button.  The image
   // index specifies the position of the button's image within the bitmap.
   //
   // You do not need to account for separators in the bitmap because this
   // function does not assign image indexes for separators.  If your toolbar
   // has buttons at positions 0, 1, and 3 and a separator at position 2,
   // the images at positions 0, 1, and 2 in your bitmap are assigned to the
   // buttons at positions 0, 1, and 3, respectively.
   // If lpIDArray is null, this function allocates space for the number of
   // items specified by nIDCount.
   //
   SetButtons( g_PainterBar, mElementCnt( g_PainterBar ) );

#define COMBOWND_INDEX     mElementCnt( g_PainterBar ) - 3  // 5
#define COMBOACT_INDEX     mElementCnt( g_PainterBar ) - 2  // 3
#define COMBOOPER_INDEX    mElementCnt( g_PainterBar ) - 1
#define COMBOBOX_WIDTH     252
#define COMBOBOX_HEIGHT    400
#define COMBOWND_BITMAP    mElementCnt( g_PainterBar ) - 3
#define COMBOACT_BITMAP    mElementCnt( g_PainterBar ) - 2
#define COMBOOPER_BITMAP   mElementCnt( g_PainterBar ) - 1

   CRect rect( -COMBOBOX_WIDTH, -COMBOBOX_HEIGHT, 0, 0 );

   // The ID of the ComboBox is important for two reasons.  One, so you
   // can receive notifications from the control.  And also for ToolTips.
   // During hit testing, if the ToolBar sees that the mouse is on a child
   // control, the toolbar will look up the control's ID and search for a
   // string in the string table with the same ID to use for ToolTips
   // and StatusBar info.
   if ( m_wndListWindows.Create( WS_CHILD | WS_VISIBLE |
                                   CBS_DROPDOWNLIST | CBS_AUTOHSCROLL |
                                   WS_VSCROLL | CBS_HASSTRINGS | CBS_SORT,
                                 rect, this, ID_TOOLBAR_WNDLIST ) &&
        m_wndListActions.Create( WS_CHILD | WS_VISIBLE |
                                   CBS_DROPDOWNLIST | CBS_AUTOHSCROLL |
                                   WS_VSCROLL | CBS_HASSTRINGS | CBS_SORT,
                                 rect, this, ID_TOOLBAR_ACTLIST ) &&
        m_wndListOperations.Create( WS_CHILD | WS_VISIBLE |
                                      CBS_DROPDOWNLIST | CBS_AUTOHSCROLL |
                                      WS_VSCROLL | CBS_HASSTRINGS | CBS_SORT,
                                    rect, this, ID_TOOLBAR_OPERLIST ) )
   {
      SetButtonInfo( COMBOWND_INDEX, ID_TOOLBAR_WNDLIST,
                     TBBS_SEPARATOR, COMBOBOX_WIDTH );
      SetButtonInfo( COMBOACT_INDEX, ID_TOOLBAR_ACTLIST,
                     TBBS_SEPARATOR, COMBOBOX_WIDTH );
      SetButtonInfo( COMBOOPER_INDEX, ID_TOOLBAR_OPERLIST,
                     TBBS_SEPARATOR, COMBOBOX_WIDTH );

      if ( m_wndListWindows.m_hWnd )
      {
         GetItemRect( COMBOWND_INDEX, rect );
         m_wndListWindows.SetWindowPos( 0, rect.left, rect.top, 0, 0,
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS );
         m_wndListWindows.ShowWindow( SW_SHOW );
      }

      if ( m_wndListActions.m_hWnd )
      {
         GetItemRect( COMBOACT_INDEX, rect );
         m_wndListActions.SetWindowPos( 0, rect.left, rect.top, 0, 0,
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS );
         m_wndListActions.ShowWindow( SW_SHOW );
      }

      if ( m_wndListOperations.m_hWnd )
      {
         GetItemRect( COMBOOPER_INDEX, rect );
         m_wndListOperations.SetWindowPos( 0, rect.left, rect.top, 0, 0,
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS );
         m_wndListOperations.ShowWindow( SW_SHOW );
      }

      m_wndListWindows.SendMessage( WM_SETFONT, (WPARAM) m_hFontTab );
      m_wndListActions.SendMessage( WM_SETFONT, (WPARAM) m_hFontTab );
      m_wndListOperations.SendMessage( WM_SETFONT, (WPARAM) m_hFontTab );
   }

   SetWindowText( _T( "Control" ) );
   EnableDocking( CBRS_ALIGN_ANY );
//?m_pZSubtask->m_pZFWnd->LoadBarState( _T( "Control" ) );

   m_pZSubtask->AddBarTip( m_hWnd, ID_ACTION_BAR_UPDATE, ID_TOOLBAR_WNDLIST );
   m_pZSubtask->AddBarTip( m_hWnd, ID_ACTION_BAR_UPDATE, ID_TOOLBAR_ACTLIST );
   m_pZSubtask->AddBarTip( m_hWnd, ID_ACTION_BAR_UPDATE, ID_TOOLBAR_OPERLIST );

   // Create a palette button for each painter control
   m_wndPaletteBar.Create( m_pZSubtask->m_pZFWnd,
                           WS_CHILD | WS_VISIBLE | CBRS_TOP |
                             CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY,
                           IDW_PAINTER_PALETTE );
// TraceLineX( "PaletteBar ", (zLONG) m_wndPaletteBar.m_hWnd );

   // Activate the painter communication object
   zVIEW  vTZPNTROO;
   zVIEW  vTZPESRCO;

   GetViewByName( &vTZPESRCO, szlTZPESRCO, m_vSubtask, zLEVEL_TASK );
   if ( vTZPESRCO )
      ActivateEmptyObjectInstance( &vTZPNTROO, szlTZPNTROO, vTZPESRCO, zSINGLE );
   else
      ActivateEmptyObjectInstance( &vTZPNTROO, szlTZPNTROO, m_vSubtask, zSINGLE );

   CreateEntity( vTZPNTROO, szlPalette, zPOS_AFTER );

   TZPainterBar *pPainterBar = this;
   SetAttributeFromBlob( vTZPNTROO, szlPalette, szlPointer,
                         &pPainterBar, sizeof( TZPainterBar * ) );
   SetNameForView( vTZPNTROO, szlTZPNTROO, 0, zLEVEL_TASK );

}  // end of:  TZPainterBar ctor

// TZPainterBar dtor
TZPainterBar::~TZPainterBar( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar dtor", "" );
#endif

   DeleteWebCtrls( );

   mDeleteInit( m_pFontCtrl );
   mDeleteInitA( m_PaletteBarItems );
   mDeleteInit( m_pActiveX_CtrlList );
   DestroyWindow( );
   if ( m_vTaskLPLR )
      SfDropSubtask( m_vTaskLPLR, 0 );

}   // end of: TZPainterBar dtor

void
TZPainterBar::DeleteWebCtrls( )
{
   // Delete Web stuff associated with this window.
   if ( m_pZMIXWebCtrl )
   {
      TZWebCtrl *pWebCtrl;
      zLONG      k;

      for ( k = m_pZMIXWebCtrl->GetCount( ) - 1; k >= 0; k-- )
      {
         if ( *(m_pZMIXWebCtrl->m_pvNode + k) )
         {
            pWebCtrl = (TZWebCtrl *) *(m_pZMIXWebCtrl->m_pvNode + k);
            delete( pWebCtrl );
         }
      }

      mDeleteInit( m_pZMIXWebCtrl );
   }
}

int
TZPainterBar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::OnCreate", "" );
#endif

// CControlBarInfo CB_Info;
//
// ReadToolBarPlacement( m_pZSubtask, "ZDR", "ToolBar", &CB_Info );
   int nRC = CToolBar::OnCreate( lpCreateStruct );
   if ( nRC != -1 )
   {
      CreateDefaultFont( );
   }

   return( nRC );
}

void
TZPainterBar::OnClose( )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterBar::OnClose m_nWndCnt: ", m_nWndCnt );
#endif

   DeleteAllPainterWindows( );
   CToolBar::OnClose( );
}

BOOL
TZPainterBar::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterBar::DestroyWindow m_nWndCnt: ", m_nWndCnt );
#endif

   // Save the Ctrl toolbar state to the registry.
// m_wndPaletteBar.GetToolBarCtrl( ).
//                 SaveState( HKEY_CURRENT_USER,
//                            "Zeidon\\DesignPainter\\Settings\\Toolbar",
//                            "CtrlBar" );

// Turn redraw off until window is cleared.      This causes a problem
// m_pZSubtask->m_pZFWnd->SetRedraw( FALSE );    when m_pZFWnd is gone

   DeleteAllPainterWindows( );
#if 0
   TZPainterWindow *pPainterWindow;
   while ( m_nWndCnt )
   {
      pPainterWindow = m_PainterWindowList[ m_nWndCnt - 1 ];
      if ( pPainterWindow->m_pBar )
         pPainterWindow->SendMessage( WM_CLOSE, 0, 0 );
      else
      {
         m_nWndCnt--;
         m_PainterWindowList.RemoveAt( m_nWndCnt );
         pPainterWindow->DestroyWindow( );
//       pPainterWindow->m_pBar = 0;
      }
   }
#endif
   // Turn redraw back on.                          This causes a problem
// m_pZSubtask->m_pZFWnd->SetRedraw( TRUE );        when m_pZFWnd is gone

   return( CToolBar::DestroyWindow( ) );
}

void
TZPainterBar::Initialization( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::Initialization", "" );
#endif

   if ( m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
   {
      zVIEW  vApp;

      CreateAppView( &vApp );
      GetWorkstationApplicationValues( vApp, "ReportFont",
                                       m_szReportFaceName,
                                       sizeof( m_szReportFaceName ),
                                       &m_lReportFontSize,
                                       0, 0, 0, 0, 0, 0, 0, 0, 0 );

      // Make the invalid ComboBoxes invisible.
      m_wndListActions.ModifyStyle( WS_VISIBLE, 0 );
      m_wndListOperations.ModifyStyle( WS_VISIBLE, 0 );
   }

#if 0
   CWindowDC dc( this );
   CPoint pt( 1000, 1000 );   // a HIENGLISH point is .001 inches

   if ( m_fState & (zTZCB_REPORT | zTZCT_XSLT) )
      m_fState &= ~(zTZCB_SCROLLBARS | zTZCB_GRID | zTZCB_SHOWTABS);
   else
      m_fState |= zTZCB_GRID;

   // Change the mapping mode to high english.
   int nOldMapMode = dc.SetMapMode( MM_HIENGLISH );

   // Convert the client rect sizes to high english.
   zLONG lMod;

   dc.LPtoDP( &pt );
   g_lPixelsPerInchX = (pt.x < 0) ? -pt.x : pt.x;
   g_lPixelsPerInchY = (pt.y < 0) ? -pt.y : pt.y;
   lMod = g_lPixelsPerInchX % 16;
   if ( lMod >= 8 )
   {
      lMod = 16 - lMod;
      g_lPixelsPerInchX += lMod;
   }
   else
      g_lPixelsPerInchX -= lMod;

   lMod = g_lPixelsPerInchY % 16;
   if ( lMod >= 8 )
   {
      lMod = 16 - lMod;
      g_lPixelsPerInchY += lMod;
   }
   else
      g_lPixelsPerInchY -= lMod;

// TraceLineI( "PixelsPerInchX = ", g_lPixelsPerInchX );
// TraceLineI( "PixelsPerInchY = ", g_lPixelsPerInchY );

   // Reset the mapping mode.
   dc.SetMapMode( nOldMapMode );
#else

   g_lPixelsPerInchX = afxData.cxPixelsPerInch;
   g_lPixelsPerInchY = afxData.cyPixelsPerInch;

#endif

   CreatePaletteButtons( );

   // The frame's EnableDocking is required for the following DockControlBar calls to work.
   m_pZSubtask->m_pZFWnd->EnableDocking( CBRS_ALIGN_ANY );
   m_pZSubtask->m_pZFWnd->DockControlBar( this, AFX_IDW_DOCKBAR_TOP );
   m_wndPaletteBar.SetWindowText( _T( "Toolbar" ) );
   m_wndPaletteBar.EnableDocking( CBRS_ALIGN_ANY );
   m_pZSubtask->m_pZFWnd->DockControlBar( &m_wndPaletteBar, AFX_IDW_DOCKBAR_RIGHT );
   m_pZSubtask->m_pZFWnd->ShowControlBar( &m_wndPaletteBar, TRUE, FALSE );

// m_pZSubtask->m_pZFWnd->UpdateWindow( );

   // Restore the Ctrl toolbar state from the registry.
// m_wndPaletteBar.GetToolBarCtrl( ).
//             RestoreState( HKEY_CURRENT_USER,
//                           "Zeidon\\DesignPainter\\Settings\\Toolbar",
//                           "CtrlBar" );
}

#if 0
const CSize COMBO_MINSIZE( 150, 25 );

/////////////////////////////////////////////////////////////////////////////
// This is the virtual function override required to add bands
//
BOOL
TZPainterBar::OnCreateBands( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::OnCreateBands", "" );
#endif

   // Create tool bar
   CCoolToolBar& tb = m_wndToolBar;
   if ( !tb.Create( this,
                    WS_CHILD | WS_VISIBLE |
                    WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                    CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC | CBRS_FLYBY ) ||
        !tb.LoadToolBar( IDR_MAINFRAME ) )
   {
      TraceLineS( "TZPainterBar::OnCreateBands", "Failed to create toolbar" );
      return( FALSE ); // failed to create
   }

   tb.ModifyStyle( 0, TBSTYLE_FLAT );
// tb.SetOwner( GetParent( ) );

   // Create combo box
   CRect rc( 0, 0, 0, 0 );
   m_wndCombo.Create( WS_VISIBLE | WS_CHILD |
                         WS_VSCROLL | CBS_DROPDOWNLIST |
                         WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                      rc, this, 1001 );

// Following is not needed since I'm not using an image list
//   CRebarInfo rbi;
// ... set stuff in rbi...
//   SetBarInfo( &rbi );

   // Get minimum size of bands
   CSize szHorz = tb.CalcDynamicLayout( -1, 0 );       // get min horz size
   CSize szVert = tb.CalcDynamicLayout( -1, LM_HORZ ); // get min vert size
   VERIFY( m_bmBackground.LoadBitmap( IDB_BITMAP1 ) ); // load background bmp

   // Band 1: Add toolbar band
   CRebarBandInfo rbbi;
   rbbi.fMask = RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_BACKGROUND | RBBIM_COLORS;
   rbbi.fStyle = RBBS_FIXEDBMP;
   rbbi.hwndChild = m_wndToolBar;
   rbbi.cxMinChild = szHorz.cx;
   rbbi.cyMinChild = szVert.cy;
   rbbi.hbmBack = m_bmBackground;
   rbbi.clrFore = ::GetSysColor( COLOR_BTNTEXT );
   rbbi.clrBack = ::GetSysColor( COLOR_BTNFACE );
   if ( !InsertBand( -1, &rbbi ) )
      return( FALSE );

   // Band 2: Add combo box band. Most settings in rbbi same from tool bar
   rbbi.fMask |= RBBIM_TEXT;
   rbbi.lpText = _T("Address:");
   rbbi.cxMinChild = COMBO_MINSIZE.cx;
   rbbi.cyMinChild = COMBO_MINSIZE.cy;
   rbbi.hwndChild = m_wndCombo;
   if ( !InsertBand( -1, &rbbi ) )
      return( FALSE );

   return( FALSE ); // OK
}
#endif

void
TZPainterBar::LoadActionList( zCPCHAR cpcWndTag )
{
   m_wndListActions.ResetContent( );
   if ( (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
   {
      zVIEW  vList;
      zPCHAR pch;
      zSHORT nRC;

      if ( GetViewByName( &vList, "TZWINDOWL", m_vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( SetCursorFirstEntityByString( vList, szlWindow, szlTag, cpcWndTag, 0 ) == 0 )
         {
            if ( CheckExistenceOfEntity( vList, "Action" ) == 0 )
            {
               nRC = SetCursorFirstEntity( vList, "Action", 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  GetAddrForAttribute( &pch, vList, "Action", "Tag" );
                  m_wndListActions.AddString( pch );

                  nRC = SetCursorNextEntity( vList, "Action", 0 );
               }
            }
         }
      }
   }
}

void
TZPainterBar::LoadWindowList( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::LoadWindowList", "" );
#endif

   zVIEW  vList;
   zVIEW  vOper;
   zPCHAR pchViewName;
   zPCHAR pchWindowPage;
   zPCHAR pchTag = 0;

   m_wndListOperations.ResetContent( );
   m_wndListWindows.ResetContent( );
   m_wndListActions.ResetContent( );

   if ( m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
   {
      pchViewName = "PAGEL";
      pchWindowPage = szlPage;
   }
   else
   {
      pchViewName = "TZWINDOWL";
      pchWindowPage = szlWindow;
   }

   if ( GetViewByName( &vList, pchViewName, m_vSubtask, zLEVEL_TASK ) > 0 )
   {
      zPCHAR pch;
      zSHORT nRC = SetCursorLastEntity( vList, pchWindowPage, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetAddrForAttribute( &pch, vList, pchWindowPage, szlTag );
         m_wndListWindows.AddString( pch );
         if ( GetSelectStateOfEntity( vList, pchWindowPage ) )
            pchTag = pch;

         nRC = SetCursorPrevEntity( vList, pchWindowPage, 0 );
      }

      if ( pchTag )
      {
         m_wndListWindows.SelectString( -1, pchTag );
//       TraceLineS( "LoadWindowList selecting: ", pchTag );
         LoadActionList( pchTag );
      }
      else
      {
         m_wndListWindows.Clear( );
//       TraceLineS( "LoadWindowList CLEAR", "" );
      }

      if ( (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
      {
         LPLIBRARY lpLibrary = 0;
         zFARPROC_DRIVER lpfnDynRoutine = (zFARPROC_DRIVER)
            GetOperationDynamicCallAddress( m_vSubtask, &lpLibrary,
                                            "tzoprusd",
                                            "wTZOPRUSD_SetUpTZOPRUSO_View",
                                            "(tzctlptb Wnd)" );
         if ( lpfnDynRoutine )
         {
            zBOOL bUpdated = (zBOOL) ObjectInstanceUpdated( vList );
            (*lpfnDynRoutine)( m_vSubtask );
            if ( bUpdated == FALSE )
               MiSetInstanceUpdateFlag( vList, FALSE );

            GetViewByName( &vList, "TZOPRUSO", m_vSubtask, zLEVEL_TASK );
            if ( CheckExistenceOfEntity( vList, "OperationList" ) == 0 )
            {
               CreateViewFromViewForTask( &vOper, vList, m_vSubtask );
               nRC = SetCursorFirstEntity( vOper, "OperationList", 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  GetAddrForAttribute( &pch, vOper,
                                       "OperationList", szlName );
                  m_wndListOperations.AddString( pch );
                  nRC = SetCursorNextEntity( vOper, "OperationList", 0 );
               }

               DropView( vOper );
            }
         }
      }
   }
// else
//    TraceLineS( "LoadWindowList EMPTY", "" );
}

void
TZPainterBar::DockControlBarLeftOf( CToolBar *pBar, CToolBar *pLeftOf )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::DockControlBarLeftOf", "" );
#endif

   CRect rect;
   DWORD dw;
   UINT  uStyle;

   // Get MFC to adjust the dimensions of all docked ToolBars
   // so that GetWindowRect will be accurate.
   m_pZSubtask->m_pZFWnd->RecalcLayout( );

   pLeftOf->GetWindowRect( &rect );
   rect.OffsetRect( 1, 0 );
   dw = pLeftOf->GetBarStyle( );
   uStyle = 0;
   uStyle = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : uStyle;
   uStyle = (dw & CBRS_ALIGN_BOTTOM && uStyle == 0) ?
                                             AFX_IDW_DOCKBAR_BOTTOM : uStyle;
   uStyle = (dw & CBRS_ALIGN_LEFT && uStyle == 0) ?
                                             AFX_IDW_DOCKBAR_LEFT : uStyle;
   uStyle = (dw & CBRS_ALIGN_RIGHT && uStyle == 0) ?
                                             AFX_IDW_DOCKBAR_RIGHT : uStyle;

   // When we take the default parameters on rect, DockControlBar will dock
   // each Toolbar on a separate line.  By calculating a rectangle, we are
   // effectively simulating a Toolbar being dragged to that location and
   // docked.
   m_pZSubtask->m_pZFWnd->DockControlBar( pBar, uStyle, &rect );
}

void
TZPainterBar::OnControlReflectCommand( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::OnControlReflectCommand", "" );
#endif

   const MSG *pMsg = CWnd::GetCurrentMessage( );
   UINT uId = LOWORD( pMsg->wParam );
   UINT uNotifyCode = HIWORD( pMsg->wParam );
   HWND hWndCtrl = (HWND) pMsg->lParam;

// TraceLineS( "TZPainterBar::OnControlReflectCommand", "" );
// TraceLineI( "TZPainterBar::OnControlReflectCommand wParam ", pMsg->wParam );
// TraceLineI( "TZPainterBar::OnControlReflectCommand lParam ", pMsg->lParam );

// TraceLineI( "TZPainterBar::OnControlReflectCommand NotifyCode = ", uNotifyCode );
// TraceLineI( "TZPainterBar::OnControlReflectCommand hWndCtrl = ", (zLONG) hWndCtrl );
// TraceLineI( "TZPainterBar::OnControlReflectCommand uId = ", uId );


   if ( m_pActivePainterWindow == 0 && uId != ID_ACTION_BAR_UPDATE )
      return;

   TZPainterWindow *pCurrPainter = m_pActivePainterWindow;
   switch ( uId )
   {
      case ID_ACTION_BAR_UPDATE:
      {
         if ( (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
         {
            LPLIBRARY lpLibrary = 0;
            zFARPROC_DRIVER lpfnDynRoutine = (zFARPROC_DRIVER)
               GetOperationDynamicCallAddress( m_vSubtask, &lpLibrary,
                                               "tzedcmpd",
                                               "BuildCompilerEntity",
                                               "(tzctlptb Act)" );
            if ( lpfnDynRoutine )
            {
               zVIEW  vTZPNTROO;

               GetViewByName( &vTZPNTROO, "TZPNTROO", m_vSubtask, zLEVEL_TASK );
               PainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vTZPNTROO, this, m_vSubtask, 0, 0 );
               EnablePainterWindows( 0 );
               (*lpfnDynRoutine)( m_vSubtask );
               ResetSelectedMouseReleaseSwap( );

               // Set selection to TargetExecutable.
               LPLIBRARY lpLib = 0;
               zFARPROC_DRIVER lpfnDynRout = (zFARPROC_DRIVER)
                        GetOperationDynamicCallAddress( m_vSubtask, &lpLib,
                                                        "tzpntrad",
                                                        "zwTZPNTRAD_TargetSpecification",
                                                        "(tzctlptb Act)" );
               (*lpfnDynRout)( m_vSubtask );
               SetWindowActionBehavior( m_vSubtask,
                                        zWAB_StartModalSubwindow,
                                        "TZEDCMPD", "TargetSpecification" );
               ProcessActionLoop( m_pZSubtask, 0, 0, 0, 0,
                                  FALSE, "(pntr build)" );
               return;
            }
         }
         else
         {
            // Fix crash when 'Build Dialog' button hit and Report window
            // is closed.
            return;
         }

         break;
      }

      case ID_ALIGN_LEFT:
         pCurrPainter->AlignSelectedCtrls( zALIGN_LEFT );
         break;

      case ID_ALIGN_TOP:
         pCurrPainter->AlignSelectedCtrls( zALIGN_TOP );
         break;

      case ID_ALIGN_RIGHT:
         pCurrPainter->AlignSelectedCtrls( zALIGN_RIGHT );
         break;

      case ID_ALIGN_BOTTOM:
         pCurrPainter->AlignSelectedCtrls( zALIGN_BOTTOM );
         break;

      case ID_SIZE_WIDTH:
         pCurrPainter->SizeSelectedCtrls( zSIZE_WIDTH );
         break;

      case ID_SIZE_HEIGHT:
         pCurrPainter->SizeSelectedCtrls( zSIZE_HEIGHT );
         break;

      case ID_SIZE_WIDTH_HEIGHT:
         pCurrPainter->SizeSelectedCtrls( zSIZE_BOTH );
         break;

      case ID_EQUAL_SPACE_HORIZONTAL:
         pCurrPainter->EqualSpaceSelectedCtrls( zEQUAL_HSPACE );
         break;

      case ID_EQUAL_SPACE_VERTICAL:
         pCurrPainter->EqualSpaceSelectedCtrls( zEQUAL_VSPACE );
         break;

      case ID_ABUT_HORIZONTAL:
         pCurrPainter->AbutSelectedCtrls( zABUT_HORIZONTAL );
         break;

      case ID_ABUT_VERTICAL:
         pCurrPainter->AbutSelectedCtrls( zABUT_VERTICAL );
         break;

      case ID_DELETE_SELECTED:
         pCurrPainter->DeleteSelectedCtrls( );
         break;

      case ID_EQUAL_SIZE_POS_CLASS:
         pCurrPainter->EqualSizePosClassCtrls( );
         break;

      case ID_GROUP_SELECTED:
         pCurrPainter->GroupBoxSelectedCtrls( );
         break;

      case ID_UNGROUP_SELECTED:
         pCurrPainter->UngroupSelectedGroupCtrls( );
         break;

      default:
         return;
   }

   pCurrPainter->SetActiveWindow( );
}

void
TZPainterBar::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterBar::OnUpdateCmdUI", "" ); // lots of messages
#endif

   // If the second parameter is FALSE, the button is not disabled,
   // otherwise, the button is disabled ... so we need to check the
   // status of the appropriate action and enable/disable appropriately.
   CToolBar::OnUpdateCmdUI( pTarget, FALSE );
}

LRESULT
TZPainterBar::OnGetToolTipText( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::OnGetToolTipText", "" );
#endif

   zPCHAR pchText = (zPCHAR) lParam;
   switch ( wParam )
   {
      case ID_ACTION_BAR_UPDATE:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Build Dialog", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_ALIGN_LEFT:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Align Left", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_ALIGN_TOP:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Align Top", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_ALIGN_RIGHT:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Align Right", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_ALIGN_BOTTOM:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Align Bottom", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_SIZE_HEIGHT:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Equal Height", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_SIZE_WIDTH:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Equal Width", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_SIZE_WIDTH_HEIGHT:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Equal Width/Height", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_EQUAL_SPACE_VERTICAL:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Equi-Space Vertical", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_EQUAL_SPACE_HORIZONTAL:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Equi-Space Horizontal", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_ABUT_HORIZONTAL:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Abut Horizontal", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_ABUT_VERTICAL:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Abut Vertical", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_DELETE_SELECTED:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Delete Selected", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_EQUAL_SIZE_POS_CLASS:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Equi-Size/Pos/Class Selected Across Windows", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_GROUP_SELECTED:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Create GroupBox around selected controls", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_UNGROUP_SELECTED:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Remove GroupBox (leaving controls in place)", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_TOOLBAR_WNDLIST:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Dialog Window List", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_TOOLBAR_ACTLIST:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Window Action List", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      case ID_TOOLBAR_OPERLIST:
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, "Dialog Operation List", zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;

      default:
         pchText[ 0 ] = 0;
         break;
   }

   return( 0 );
}

BOOL
TZPainterBar::OnCmdMsg( UINT uId, int nCode, void *pExtra,
                        AFX_CMDHANDLERINFO *pHandlerInfo )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterBar::OnCmdMsg uId ", uId );
   TraceLineI( "            OnCmdMsg nCode ", nCode );
   TraceLineI( "      ZSubtask's lMsgCmdId ", m_pZSubtask->m_lMsgCmdId );
#endif

#if 0
   if ( uId == 0 && nCode == 5176814 ) //??? what is 5176814's define???
   {
      switch ( m_pZSubtask->m_lMsgCmdId )
      {
         case ID_ACTION_BAR_UPDATE:
            MB_SetMessage( m_vSubtask, 3, "Build Dialog" );
            break;

         case ID_ALIGN_LEFT:
            MB_SetMessage( m_vSubtask, 3, "Align Left" );
            break;

         case ID_ALIGN_TOP:
            MB_SetMessage( m_vSubtask, 3, "Align Top" );
            break;

         case ID_ALIGN_RIGHT:
            MB_SetMessage( m_vSubtask, 3, "Align Right" );
            break;

         case ID_ALIGN_BOTTOM:
            MB_SetMessage( m_vSubtask, 3, "Align Bottom" );
            break;

         case ID_SIZE_HEIGHT:
            MB_SetMessage( m_vSubtask, 3, "Size Height" );
            break;

         case ID_SIZE_WIDTH:
            MB_SetMessage( m_vSubtask, 3, "Size Width" );
            break;

         case ID_SIZE_HEIGHT_WIDTH:
            MB_SetMessage( m_vSubtask, 3, "Size Height and Width" );
            break;

         case ID_EQUAL_SPACE_VERTICAL:
            MB_SetMessage( m_vSubtask, 3, "Equal Space Vertical" );
            break;

         case ID_EQUAL_SPACE_HORIZONTAL:
            MB_SetMessage( m_vSubtask, 3, "Equal Space Horizontal" );
            break;

         case ID_DELETE_SELECTED:
            MB_SetMessage( m_vSubtask, 3, "Delete Selected" );
            break;

         case ID_EQUAL_SIZE_POS_CLASS:
            MB_SetMessage( m_vSubtask, 3, "Equal Size/Pos/Class Ctrl across Windows" );
            break;

         case ID_GROUP_SELECTED:
            MB_SetMessage( m_vSubtask, 3, "GroupBox selected Ctrls" );
            break;

         default:
            MB_SetMessage( m_vSubtask, 3, "" );
            break;
      }
   }
   else
#endif

   if ( uId == ID_TOOLBAR_WNDLIST )
   {
#if 0
      switch ( nCode )
      {
         case CBN_CLOSEUP:
            TraceLineI( "CBNCloseup ", nCode );
            break;

         case CBN_DBLCLK:
            TraceLineI( "CBNDblclk ", nCode );
            break;

         case CBN_DROPDOWN:
            TraceLineI( "CBNDropdown ", nCode );
            break;

         case CBN_EDITCHANGE:
            TraceLineI( "CBNEditchange ", nCode );
            break;

         case CBN_EDITUPDATE:
            TraceLineI( "CBNEditupdate ", nCode );
            break;

         case CBN_ERRSPACE:
            TraceLineI( "CBNErrspace ", nCode );
            break;

         case CBN_SELCHANGE:
            TraceLineI( "CBNSelchange ", nCode );
            break;

         case CBN_SELENDCANCEL:
            TraceLineI( "CBNSelendcancel ", nCode );
            break;

         case CBN_SELENDOK:
            TraceLineI( "CBNSelendok ", nCode );
            break;

         case CBN_SETFOCUS:
            TraceLineI( "CBNSetFocus ", nCode );
            break;

         case CBN_KILLFOCUS:
            TraceLineI( "CBNKillFocus ", nCode );
            break;

         default:
            TraceLineI( "Unknown Combo code ", nCode );
            break;
      }
#endif

      if ( nCode == CBN_SELENDOK )
      {
         zVIEW  vDialogList;
         zVIEW  vTZPNTROO;
         zCHAR  szTag[ 34 ];
         zSHORT nIdx;
         zPCHAR pchViewName;
         zPCHAR pchWindowPage;

      // TraceLineS( "PainterBar WndList EndOK", "" );
         nIdx = m_wndListWindows.GetCurSel( );
         if ( nIdx >= 0 )
            m_wndListWindows.GetLBText( nIdx, szTag );
         else
            m_wndListWindows.GetWindowText( szTag, sizeof( szTag ) - 1 );

         if ( m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
         {
            pchViewName = "PAGEL";
            pchWindowPage = szlPage;
         }
         else
         {
            pchViewName = "TZWINDOWL";
            pchWindowPage = szlWindow;
         }

         // Get the view to the window list.
         if ( GetViewByName( &vDialogList, pchViewName,
                             m_vSubtask, zLEVEL_TASK ) > 0 &&
              SetCursorFirstEntityByString( vDialogList, pchWindowPage, szlTag,
                                            szTag, 0 ) == zCURSOR_SET )
         {
            GetViewByName( &vTZPNTROO, "TZPNTROO", m_vSubtask, zLEVEL_TASK );
            PainterCall( zMSG_UPDATEWINDOWBYNAME, vTZPNTROO, this, vDialogList, szTag, sizeof( szTag ) );
         }

         return( TRUE );
      }
   }
   else
   if ( uId == ID_TOOLBAR_ACTLIST )
   {
      if ( nCode == CBN_SELENDOK && (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
      {
         zCHAR  szWndTag[ 34 ];
         zCHAR  szActTag[ 34 ];
         zVIEW  vList;
         zSHORT nIdx;

      // TraceLineS( "PainterBar ActList EndOK", "" );
         nIdx = m_wndListWindows.GetCurSel( );
         if ( nIdx >= 0 )
            m_wndListWindows.GetLBText( nIdx, szWndTag );
         else
            m_wndListWindows.GetWindowText( szWndTag,
                                            sizeof( szWndTag ) - 1 );

         if ( szWndTag[ 0 ] &&
              GetViewByName( &vList, "TZWINDOW", m_vSubtask, zLEVEL_TASK ) > 0 )
         {
            if ( SetCursorFirstEntityByString( vList, szlWindow,
                                               szlTag, szWndTag, 0 ) == 0 )
            {
               nIdx = m_wndListActions.GetCurSel( );
               if ( nIdx >= 0 )
                  m_wndListActions.GetLBText( nIdx, szActTag );
               else
                  m_wndListActions.GetWindowText( szActTag,
                                                  sizeof( szActTag ) - 1 );

               if ( szActTag[ 0 ] &&
                    SetCursorFirstEntityByString( vList, szlAction,
                                                  szlTag, szActTag, 0 ) == 0 )
               {
                  zVIEW  vTZPNTROO;
                  zVIEW  vWork;

                  GetViewByName( &vTZPNTROO, "TZPNTROO", m_vSubtask, zLEVEL_TASK );
                  GetViewByName( &vWork, "TZPNCTWO", m_vSubtask, zLEVEL_TASK );
                  SetAttributeFromString( vWork, "TZPNCTWO",
                                          "ParentIsSelectAction", "N" );

                  PainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vTZPNTROO, this, m_vSubtask, 0, 0 );
                  EnablePainterWindows( 0 );

                  CreateTemporalSubobjectVersion( vList, "Action" );
                  ResetSelectedMouseReleaseSwap( );
                  SetWindowActionBehavior( m_vSubtask,
                                           zWAB_StartModalSubwindow,
                                           "TZPNCTAD", "UPD_ACT" );
                  ProcessActionLoop( m_pZSubtask, 0, 0, 0, 0,
                                     FALSE, "(pntr action)" );
               }
            }
         }

         return( TRUE );
      }
   }
   else
   if ( uId == ID_TOOLBAR_OPERLIST )
   {
      if ( nCode == CBN_SELENDOK && (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
      {
         zVIEW  vList;
         zCHAR  szOperTag[ 34 ];
         zSHORT nIdx;

      // TraceLineS( "PainterBar OperList EndOK", "" );
         nIdx = m_wndListOperations.GetCurSel( );
         if ( nIdx >= 0 )
            m_wndListOperations.GetLBText( nIdx, szOperTag );
         else
            m_wndListOperations.GetWindowText( szOperTag,
                                               sizeof( szOperTag ) - 1 );

         // Get the view to the window list.
         if ( szOperTag[ 0 ] &&
              GetViewByName( &vList, "TZOPRUSO", m_vSubtask, zLEVEL_TASK ) > 0 &&
              SetCursorFirstEntityByString( vList, "OperationList",
                                            szlName, szOperTag,
                                            0 ) == zCURSOR_SET )
         {
            LPLIBRARY lpLibrary = 0;
            zFARPROC_DRIVER lpfnDynRoutine = (zFARPROC_DRIVER)
               GetOperationDynamicCallAddress( m_vSubtask, &lpLibrary,
                                               "tzoprusd",
                                               "wTZOPRUSD_InvokeEditor",
                                               "(tzctlptb Oper)" );
            if ( lpfnDynRoutine )
            {
               zVIEW  vTZPNTROO;

               GetViewByName( &vTZPNTROO, "TZPNTROO", m_vSubtask, zLEVEL_TASK );
               PainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vTZPNTROO, this, m_vSubtask, 0, 0 );
               EnablePainterWindows( 0 );
               (*lpfnDynRoutine)( m_vSubtask );
               ProcessActionLoop( m_pZSubtask, 0, 0, 0, 0,
                                  FALSE, "(pntr oper)" );
            }
         }

         return( TRUE );
      }
   }

   return( FALSE );  // not handled here ... pass it on
}

void
TZPainterBar::ParentNotify( UINT message, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::ParentNotify", "" );
#endif
}

// This method is to temporarily delete all controls within a
// painter window.  They must then be brought back to life prior
// to another action taking place.  Normally there should be a
// flow of: UpdateAllPainterWindows, EmptyAllPainterWindows,
//
void
TZPainterBar::EmptyAllPainterWindows( zPCHAR pchParm )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterBar::EmptyAllPainterWindows cnt: ", m_nWndCnt );
#endif

   TZPainterWindow *pPainterWindow;
   zSHORT k;

   // Delete all the painter application windows.
   for ( k = 0; k < m_nWndCnt; k++ )
   {
      pPainterWindow = m_PainterWindowList[ k ];
      if ( pchParm == 0 || pPainterWindow != m_pActivePainterWindow )
         pPainterWindow->EmptyPainterWindow( );
   }
}

void
TZPainterBar::DeleteAllPainterWindows( )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterBar::DeleteAllPainterWindows cnt: ", m_nWndCnt );
#endif

   TZPainterWindow *pPainterWindow;

   // Delete all the painter application windows.
   while ( m_nWndCnt )
   {
      pPainterWindow = m_PainterWindowList[ m_nWndCnt - 1 ];
      if ( pPainterWindow->m_pBar )
         pPainterWindow->SendMessage( WM_CLOSE, 0, 0 );
      else
      {
         m_nWndCnt--;
         m_PainterWindowList.RemoveAt( m_nWndCnt );
//       pPainterWindow->m_pBar = 0;

         // Never explicitly call 'delete' on a CFrameWnd ... use
         // DestroyWindow instead!
         pPainterWindow->DestroyWindow( );
      }
   }

   m_pActivePainterWindow = 0;
}

//
// METHOD: EnablePainterWindows
//
//    This method enables and disables painter windows when processing sub-dialogs.
//
void
TZPainterBar::EnablePainterWindows( zBOOL bEnable )
{
   zSHORT nPos;

#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::EnablePainterWindows: ", bEnable ? "TRUE" : "FALSE" );
   if ( bEnable && m_pActivePainterWindow )
   {
      TZPainterCtrl *pCtrl;
      zCHAR szMsg[ 256 ];

      for ( nPos = 0; nPos < m_pActivePainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pActivePainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_lType == 2090 || pCtrl->m_lType == 2091 )
         {
            sprintf_s( szMsg, sizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
                      pCtrl->IsSelected( ) ? "*" : " ",
                      pCtrl->m_pCtrlParent ?
                                    pCtrl->m_pCtrlParent->m_csTag : "<null>",
                      pCtrl->m_csTag,
                      (pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd )) ? " " : "~",
                      pCtrl->m_uVisible );
            TraceRect( szMsg, pCtrl->m_rectCtrl );
         }
      }
   }
#endif

   for ( nPos = 0; nPos < m_nWndCnt; nPos++ )
   {
      m_PainterWindowList[ nPos ]->EnableWindow( bEnable );
   }

#if 0
   BL, 2000.09.01 do not repaint Zeidon Window,
   to avoid an error described as follows:
   1. Open the Action List
   2. Move Controls in Zeidon Window
   3. Close Action List
   --> on repainting the Window, the Controls move to their old Position

   Another ugly side effect is avoided: By bringing the "Active Painter
   Window" to the foreground, other windows (e.g. operation list) may
   be overlaid and the desktop changes in an unexpected way.

   // If we are enabling the painter windows, set the active window
   // to the current painter if it exists.
   if ( bEnable && m_pActivePainterWindow )
   {
      m_pActivePainterWindow->SetActiveWindow( );
      m_pActivePainterWindow->PaintZeidonWindow( );
   }
#endif
}

void
TZPainterBar::SetScrollbars( zBOOL bEnable )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::SetScrollbars", "" );
#endif

   TZPainterWindow *pPainterWindow;
   zSHORT   nPos;

   if ( bEnable )
      m_fState |= zTZCB_SCROLLBARS;
   else
      m_fState &= ~zTZCB_SCROLLBARS;

   for ( nPos = 0; nPos < m_nWndCnt; nPos++ )
   {
      pPainterWindow = m_PainterWindowList[ nPos ];
      pPainterWindow->m_pPainterClient->ScrollToPosition( CPoint( 0, 0 ) );
//    TraceLineS( "TZPainterBar::SetScrollbars ", bEnable ? "TRUE" : "FALSE" );
      if ( bEnable )
      {
         CSize size( (1024 * 3) / (2 * pPainterWindow->m_lScrollUnitsX),
                     (1024 * 3) / (2 * pPainterWindow->m_lScrollUnitsY) );

         SCROLLINFO si;

         si.cbSize = sizeof( SCROLLINFO );
         si.fMask = SIF_ALL;

         pPainterWindow->m_pPainterClient->GetScrollInfo( SB_VERT, &si );
         si.nMin = 0;
         si.nMax = 1000;   // size.cy;
         si.nPage = 200;
         si.nPos = 0;
         // si.nTrackPos = 0;  ignored anyhow
         pPainterWindow->m_pPainterClient->SetScrollInfo( SB_VERT, &si, TRUE );

         pPainterWindow->m_pPainterClient->GetScrollInfo( SB_HORZ, &si );
         si.nMin = 0;
         si.nMax = 1000;   // size.cx;
         si.nPage = 200;
         si.nPos = 0;
         // si.nTrackPos = 0;  ignored anyhow
         pPainterWindow->m_pPainterClient->SetScrollInfo( SB_HORZ, &si, TRUE );

         CRect rect;
         pPainterWindow->GetClientRect( rect );

         pPainterWindow->m_pPainterClient->
               SetScrollSizes( MM_TEXT,
                               CSize( pPainterWindow->m_lHScrollMax + 200,
                                      pPainterWindow->m_lVScrollMax + 200 ),
                               CSize( rect.right, rect.bottom ),
                               CSize( pPainterWindow->m_lScrollUnitsX,
                                      pPainterWindow->m_lScrollUnitsY ) );
      }
      else
      {
         pPainterWindow->m_pPainterClient->
                         SetScrollSizes( MM_TEXT, CSize( 0, 0 ) );
      }
   }
}

void
TZPainterBar::SetGrid( zBOOL bEnable )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::SetGrid", "" );
#endif

   TZPainterWindow *pPainterWindow;
   zSHORT   nPos;

   if ( bEnable )
      m_fState |= zTZCB_GRID;
   else
      m_fState &= ~zTZCB_GRID;

   for ( nPos = 0; nPos < m_nWndCnt; nPos++ )
   {
      pPainterWindow = m_PainterWindowList[ nPos ];
//    TraceLineS( "TZPainterBar::SetGrid ", bEnable ? "TRUE" : "FALSE" );
      pPainterWindow->Invalidate( TRUE );
   }
}

//
// Create a tool box button for each painter control defined which
// has a bitmap.
//

void
TZPainterBar::CreatePaletteButtons( )
{
// ZActiveX_CtrlList *pActiveX_CtrlList = 0; now a data item of TZPainterBar
   UINT    *pPaletteButtons = 0;
   zLONG   lKey;
   zLONG   lVisible;
   zCHAR   szBitmap[ 34 ];
   zPCHAR  pchTag;
   zVIEW   vTZPESRCO;
   zVIEW   vTZ__PRFO;
   zLONG   lFirstId;
   zLONG   lId;
   zUSHORT uButtonCnt = 0;
   zUSHORT k;
   UINT    j;

#ifdef DEBUG_ALL
   szBitmap[ 0 ] = (m_fState & zTZCB_XSLT) ? 'X' : (m_fState & zTZCB_REPORT) ? 'R' : 'D';
   szBitmap[ 1 ] = 0;

   TraceLineS( "TZPainterBar::CreatePaletteButtons Dialog-Report ",
               szBitmap );
#endif

   // Retrieve the view to the presentation environment.
   GetViewByName( &vTZPESRCO, szlTZPESRCO, m_vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZ__PRFO, szlProfileXFER, m_vSubtask, zLEVEL_TASK );

   if ( vTZPESRCO == 0 || vTZ__PRFO == 0 ||
        SetCursorFirstEntity( vTZPESRCO, szlControlDef, 0 ) < zCURSOR_SET ||
        mIs_hWnd( m_wndPaletteBar.m_hWnd ) == FALSE )
   {
      return;
   }

   m_pZSubtask->m_ulNextBarTipCmd = ID_ARROW;
   lFirstId = m_pZSubtask->m_ulNextBarTipCmd;

   // Create a palette button for each painter control.
   HINSTANCE hInstOld = AfxGetResourceHandle( );
   zCHAR     szDLL_Name[ 32 ];
   LPLIBRARY hLibrary;
   HINSTANCE hInst = 0;
   zPCHAR    pch;

   if ( (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )     // Dialog Painter
   {
      m_pActiveX_CtrlList = new ZActiveX_CtrlList( m_vSubtask );
      m_pActiveX_CtrlList->PopulateZeidonRegisteredActiveX( );
      m_uPaletteItemCnt = CountEntitiesForViewUnderParent( vTZ__PRFO, szlControlDef, 0 );
   }
   else  // Reports
   {
      m_uPaletteItemCnt = CountEntitiesForViewUnderParent( vTZPESRCO, szlControlDef, 0 );
   }

   m_uPaletteItemCnt++;  // at least one for the pointer bitmap

   // Delete old Toolbar Buttons
   CToolBarCtrl& rPaletteCtrl = m_wndPaletteBar.GetToolBarCtrl( );
   if ( m_uPaletteItemCnt )
   {
      k = rPaletteCtrl.GetButtonCount( );
      while ( k-- )
         rPaletteCtrl.DeleteButton( k );

      mDeleteInitA( m_PaletteBarItems );
   }

   m_PaletteBarItems = (zPALETTE_ITEM *)
                  new char[ sizeof( zPALETTE_ITEM ) * m_uPaletteItemCnt ];

   // Skip arrow pointer.
   for ( k = 1; k < m_uPaletteItemCnt; k++ )
      m_PaletteBarItems[ k ].chState = 0;

   // Create a pointer on the toolbar.
   m_wndPaletteBar.LoadBitmap( IDW_PAINTER_PALETTE );
   m_PaletteBarItems[ 0 ].chState = 'Y';
   m_PaletteBarItems[ 0 ].uBitmapIdx = 0;
   m_PaletteBarItems[ 0 ].lKey = 0;  // no Key for the pointer bitmap
   m_PaletteBarItems[ 0 ].uCmdId = m_pZSubtask->m_ulNextBarTipCmd;
   m_PaletteBarItems[ 0 ].szTag[ 0 ] = 0;
   m_pZSubtask->m_ulNextBarTipCmd++;

   szDLL_Name[ 0 ] = 0; // initialize to null
   SetCursorFirstEntity( vTZPESRCO, szlControlDef, 0 );
   do    // we've guaranteed position on first entity.
   {
      GetStringFromAttribute( szBitmap, sizeof( szBitmap ), vTZPESRCO, szlControlDef, szlPainterBitmap );
      GetIntegerFromAttribute( &lVisible, vTZPESRCO, szlControlDef, "Invisible" );

      // There is no Bitmap or the Control is invisible (deleted).
      if ( szBitmap[ 0 ] == 0 || lVisible == 1 )
         continue;

      // If Control visible for Dialog Painter and Control in vTZ__PRFO?
      if ( (((lVisible == 0 || lVisible == -1 || lVisible == -3 || lVisible == -5) &&
             (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 &&    // Ctrl visible for Dialog
             SetEntityCursor( vTZ__PRFO, szlControlDef, szlTag, zPOS_FIRST | zQUAL_ENTITYATTR,
                              vTZPESRCO, szlControlDef, szlTag, 0, 0, 0 ) >= zCURSOR_SET )) ||
            ((lVisible == 0 || lVisible == -2 || lVisible == -3 || lVisible == -6) &&
             (m_fState & zTZCB_REPORT)) ||                       // Ctrl visible for Report
            ((lVisible == 0 || lVisible == -4 || lVisible == -5 || lVisible == -6) &&
             (m_fState & zTZCB_XSLT)) )                          // Ctrl visible for XSLT
      {
         GetAddrForAttribute( &pch, vTZPESRCO, szlControlDef, "PainterDLL_Name" );
         if ( zstrcmp( pch, szDLL_Name ) != 0 )
         {
            hInst = 0;
            strcpy_s( szDLL_Name, sizeof( szDLL_Name ), pch );
            hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog, pch, zLOADLIB_RESOURCES );
            hInst = (HINSTANCE) SysGetInstance( hLibrary );
            if ( hInst )
            {
               AfxSetResourceHandle( hInst );
            }
         }

         if ( hInst )
         {
            GetIntegerFromAttribute( &lKey, vTZPESRCO, szlControlDef, szlKey );
            GetIntegerFromAttribute( &lId, vTZPESRCO, szlControlDef, "Id" );

            if ( lId && lKey &&
                 (j = rPaletteCtrl.AddBitmap( 1, (UINT) lId )) >= 0 )
            {
               uButtonCnt++;
               m_PaletteBarItems[ uButtonCnt ].uBitmapIdx = j;
               m_PaletteBarItems[ uButtonCnt ].uCmdId = m_pZSubtask->m_ulNextBarTipCmd;
               m_PaletteBarItems[ uButtonCnt ].lKey = lKey;
               m_pZSubtask->m_ulNextBarTipCmd++;

               GetAddrForAttribute( &pchTag, vTZPESRCO, szlControlDef, szlTag );
               strcpy_s( m_PaletteBarItems[ uButtonCnt ].szTag, sizeof( m_PaletteBarItems[ uButtonCnt ].szTag ), pchTag );
            }
         }
      }

   } while ( SetCursorNextEntity( vTZPESRCO, szlControlDef, 0 ) >= zCURSOR_SET );

   AfxSetResourceHandle( hInstOld );

   // Now that the painter controls are active, go through the controls and
   // create a toolbar button for each control which has a painting bitmap
   // defined.  Also, the following restrictions have been implemented:
   //
   //   -6 - control def is visible to the report builder and XSLT builder
   //   -5 - control def is visible to the painter and XSLT builder
   //   -4 - control def is visible only to the XLST builder
   //   -3 - control def is visible to the painter and report builder
   //   -2 - control def is visible only to the report builder
   //   -1 - control def is visible only to the painter
   //    0 - control def is visible to painter, report builder, and XSLT builder
   //    1 - control def is invisible (to be deleted)
   //

   if ( m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
      vTZ__PRFO = 0;
   else
      SetCursorFirstEntity( vTZ__PRFO, szlControlDef, 0 );

   uButtonCnt = 0;
   pPaletteButtons = (UINT *) new char[ sizeof( UINT ) * m_uPaletteItemCnt ];
   pPaletteButtons[ 0 ] = m_PaletteBarItems[ 0 ].uCmdId;
   SetCursorFirstEntity( vTZPESRCO, szlControlDef, 0 );
   for ( k = 0; ; )
   {
      if ( vTZ__PRFO == 0 ||
           CheckExistenceOfEntity( vTZ__PRFO, szlControlDef ) >= zCURSOR_SET &&
           SetEntityCursor( vTZPESRCO, szlControlDef, szlTag,
                            zPOS_FIRST | zQUAL_ENTITYATTR,
                            vTZ__PRFO, szlControlDef, szlTag,
                            0, 0, 0 ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szBitmap, sizeof( szBitmap ), vTZPESRCO, szlControlDef, szlPainterBitmap );
         GetIntegerFromAttribute( &lVisible, vTZPESRCO, szlControlDef, "Invisible" );

         if ( szBitmap[ 0 ] && lVisible <= 0 &&
              (((lVisible == 0 || lVisible == -1 || lVisible == -3 || lVisible == -5) &&
                    (m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0) ||      // Dialog
               ((lVisible == 0 || lVisible == -2 || lVisible == -3 || lVisible == -6) &&
                    (m_fState & zTZCB_REPORT))   ||          // Report
               ((lVisible == 0 || lVisible == -4 || lVisible == -5 || lVisible == -6) &&
                    (m_fState & zTZCB_XSLT))) )              // XSLT
         {
            GetIntegerFromAttribute( &lKey, vTZPESRCO, szlControlDef, szlKey );
            GetIntegerFromAttribute( &lId, vTZPESRCO, szlControlDef, "Id" );

            if ( lId && lKey && lKey < 4000 ) // not for ActiveX Controls
            {
               uButtonCnt++;
               for ( j = 1; j < m_uPaletteItemCnt; j++ )
               {
                  if ( m_PaletteBarItems[ j ].lKey == lKey )
                  {
                     k++;
                     m_PaletteBarItems[ j ].chState = 'Y';
                     pPaletteButtons[ k ] = m_PaletteBarItems[ j ].uCmdId;

                     break;  // out of inner loop
                  }
               }
            }
         }
      }

      if ( vTZ__PRFO &&
           CheckExistenceOfEntity( vTZ__PRFO, szlControlDef ) >= zCURSOR_SET &&
           SetCursorNextEntity( vTZ__PRFO, szlControlDef, 0 ) < zCURSOR_SET )
      {
         break;
      }
      else
      if ( SetCursorNextEntity( vTZPESRCO, szlControlDef, 0 ) < zCURSOR_SET )
      {
         break;
      }
   }

   // Add the Entries for ActiveX controls.
   if ( m_pActiveX_CtrlList )
   {
      // Step through the list and get the ActiveX Control associated Bitmap.
      ZActiveX_Ctrl *pAX_Ctrl;
      POSITION pos = m_pActiveX_CtrlList->GetHeadPosition( );

      while ( pos )
      {
         pAX_Ctrl = (ZActiveX_Ctrl *) m_pActiveX_CtrlList->GetAt( pos );
         if ( pAX_Ctrl )
         {
            CBitmap *pbm = pAX_Ctrl->GetControlBitmap( );

            // If you pass a pointer to a CBitmap object, you must ensure
            // that the bitmap is not destroyed until after the toolbar
            // is destroyed.  The return value is the zero-based index of
            // the first new image if successful; otherwise -1.
            lKey = pAX_Ctrl->GetKey( );
            if ( SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef, szlKey, lKey, "" ) >= zCURSOR_SET &&
                 vTZ__PRFO &&
                 SetEntityCursor( vTZ__PRFO, szlControlDef, szlTag, zPOS_FIRST | zQUAL_ENTITYATTR,
                                  vTZPESRCO, szlControlDef, szlTag, 0, 0, 0 ) >= zCURSOR_SET  &&
                 (j = rPaletteCtrl.AddBitmap( 1, pbm )) >= 0 )
            {
               uButtonCnt++;
               pPaletteButtons[ uButtonCnt ] = m_pZSubtask->m_ulNextBarTipCmd;
               m_PaletteBarItems[ uButtonCnt ].uBitmapIdx = j;
               m_PaletteBarItems[ uButtonCnt ].lKey = pAX_Ctrl->GetKey( );
               m_PaletteBarItems[ uButtonCnt ].uCmdId =
                                               m_pZSubtask->m_ulNextBarTipCmd;
               strcpy_s( m_PaletteBarItems[ uButtonCnt ].szTag, sizeof( m_PaletteBarItems[ uButtonCnt ].szTag ),
                         pAX_Ctrl->GetDescription( ) );
               m_PaletteBarItems[ uButtonCnt ].chState = 'Y';                     //?
               m_pZSubtask->m_ulNextBarTipCmd++;
            }
         }

         m_pActiveX_CtrlList->GetNext( pos );
      }
   }

   m_pZSubtask->AddBarTip( m_wndPaletteBar.m_hWnd, lFirstId, lFirstId + uButtonCnt );

   uButtonCnt++;          // at least one for the pointer bitmap
// TraceLineI( "CreatePaletteButtons: ", uButtonCnt );

   //
   // SetButtonInfo( int nIndex, UINT nID, UINT nStyle, int nImage )
   // Parameters:
   // -----------
   // nIndex - Index of the button or separator whose information is to
   //          be set.
   // nID - The value to which the button's command ID is set.
   // nStyle - The new button style. The following button styles are
   //          supported:
   //    TBBS_BUTTON - Standard pushbutton (default)
   //    TBBS_SEPARATOR - Separator
   //    TBBS_CHECKBOX - Auto check-box button
   //    TBBS_GROUP - Marks the start of a group of buttons
   //    TBBS_CHECKGROUP - Marks the start of a group of check-box buttons
   // nImage - New index for the button's image within the bitmap.
   //
   // Remarks:
   // --------
   // Call SetButtonInfo to set the button's command ID, style, and image
   // number.  For separators, which have the style TBBS_SEPARATOR, this
   // function sets the separator's width in pixels to the value stored
   // in nImage.
   //

   m_wndPaletteBar.SetButtons( pPaletteButtons, uButtonCnt );
   k = uButtonCnt;
   while ( k-- )
   {
//    m_wndPaletteBar.SetButtonStyle( k, TBBS_CHECKGROUP );

      // Call SetButtonInfo to set the button's command ID, style, and image.
      for ( j = 0; j < m_uPaletteItemCnt; j++ )
      {
         if ( pPaletteButtons[ k ] == m_PaletteBarItems[ j ].uCmdId )
         {
            m_wndPaletteBar.SetButtonInfo( k, pPaletteButtons[ k ],
                                           TBBS_CHECKGROUP,
                                           m_PaletteBarItems[ j ].uBitmapIdx );
         }
      }
   }

   rPaletteCtrl.CheckButton( m_PaletteBarItems[ 0 ].uCmdId, TRUE );
   rPaletteCtrl.AutoSize( );
   delete [] pPaletteButtons;
   GetParentFrame( )->RecalcLayout( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// DeselectModal - zero the current selected pointer and set the
//                 state of the selected modal button to non-highlighted.
void
TZPainterBar::DeselectModal( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::DeselectModal", "" );
#endif

   CToolBarCtrl& rPaletteCtrl = m_wndPaletteBar.GetToolBarCtrl( );
   if ( m_uSelectedPaletteItem )
   {
      rPaletteCtrl.
           CheckButton( m_PaletteBarItems[ m_uSelectedPaletteItem ].uCmdId,
                        FALSE );
      m_uSelectedPaletteItem = 0;
   }

   rPaletteCtrl.CheckButton( m_PaletteBarItems[ 0 ].uCmdId, TRUE );
}

// SelectModalPointer - Deselect the current modal button and select
//                      the modal pointer if one exists.
void
TZPainterBar::SelectModalPointer( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::SelectModalPointer", "" );
#endif

   CToolBarCtrl& rPaletteCtrl = m_wndPaletteBar.GetToolBarCtrl( );

   if ( m_uSelectedPaletteItem )
   {
      rPaletteCtrl.
           CheckButton( m_PaletteBarItems[ m_uSelectedPaletteItem ].uCmdId,
                        FALSE );
      m_uSelectedPaletteItem = 0;
   }

   rPaletteCtrl.CheckButton( m_PaletteBarItems[ 0 ].uCmdId, TRUE );

   // Set the cursor for the painter window to the standard pointer and
   // then set the class word for all ctrl covers
   if ( m_pActivePainterWindow &&
        m_pActivePainterWindow->m_pPainterClient )
   {
      ::SetClassLong( m_pActivePainterWindow->m_pPainterClient->m_hWnd,
                      GCL_HCURSOR, (zLONG) ::LoadCursor( 0, IDC_ARROW ) );

      if ( m_pActivePainterWindow->m_nCtrlCnt )
      {
         TZPainterCtrl *pCtrl = m_pActivePainterWindow->m_CtrlList[ 0 ];
         if ( pCtrl->m_pCtrlCover )
            ::SetClassLong( pCtrl->m_pCtrlCover->m_hWnd,
                            GCL_HCURSOR, (zLONG) ::LoadCursor( 0, IDC_ARROW ) );
      }
   }
}

//
// METHOD: PaintToolBoxCtrl
//
//    This method paints a new control into a painter window
//
zBOOL
TZPainterBar::PaintToolBoxCtrl( TZPainterWindow *pPainterWindow,
                                CPoint ptStart, CPoint ptEnd )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterBar::PaintToolBoxCtrl", "" );
#endif

   zVIEW         vCtrlDef;
   zVIEW         vParentDef;
   CRect         rectPaint;
   CRect         rectParent;
   zCHAR         szTag[ 80 ];
   zLONG         lWork;
   zLONG         lNewType;
   zLONG         lParentType;
   TZPainterCtrl *pCtrl;
   TZPainterCtrl *pNewParent;

   // If there isn't a current control for painting, then return FALSE.
   if ( m_uSelectedPaletteItem == 0 ||
        m_PaletteBarItems[ m_uSelectedPaletteItem ].lKey == 0 )
   {
      return( FALSE );
   }

   if ( (pPainterWindow->m_fWndState & zTZPW_UPDATEABLE) == 0 )
      return( FALSE );

   // Get the points for painting.  Flip the coordinates if necessary so
   // the starting point is before the ending point.
   if ( ptStart.x > ptEnd.x )
   {
      lWork = ptEnd.x;
      ptEnd.x = ptStart.x;
      ptStart.x = lWork;
   }

   if ( ptStart.y > ptEnd.y )
   {
      lWork = ptEnd.y;
      ptEnd.y = ptStart.y;
      ptStart.y = lWork;
   }

   // If the control is too small to be visible, then assume the
   // operator slipped off the mouse button too soon ... and get out fast.
   if ( (ptEnd.x - ptStart.x) < 8 || (ptEnd.y - ptStart.y) < 8 )
      return( FALSE );

   CPoint ptScrollPos =
                pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );

   // Build a rect for snapping.
   rectPaint.left   = ptStart.x;
   rectPaint.top    = ptStart.y;
   rectPaint.right  = ptEnd.x;
   rectPaint.bottom = ptEnd.y;

   // Adjust for scrolling position and reset the ScrollPosition
// if ( (pPainterWindow->m_fWndState & zTZPW_SCROLLADJUSTED) == 0 )
   {
      SetRedraw( FALSE );
      pPainterWindow->m_pPainterClient->ScrollToPosition( CPoint( 0, 0 ) );
   }
// else
// {
   // // Adjust for scroll position.
   // rectPaint.top += ptScrollPos.y;
   // rectPaint.left += ptScrollPos.x;
   // rectPaint.bottom += ptScrollPos.y;
   // rectPaint.right += ptScrollPos.x;
// }

   // Snappy snapping ... this could be optional, but since a resize
   // does not force snapping, let it happen.
   // Set y size based on character height since most controls need
   // character alignment. We align to the closest character size to
   // the rectangle created.
   pPainterWindow->SnapRect( &rectPaint, 0, -1 );

   // Now that we have the rect to paint, get the id of the select
   // paint control and paint that type of control in the rect specified.
   lNewType = m_PaletteBarItems[ m_uSelectedPaletteItem ].lKey;

   // Set tag to null, let control do tag generation.
   szTag[ 0 ] = 0;

   // If there is a parent control to the ctrl cover, make sure the
   // rectangle being painted is wholly contained within the paint parent,
   // otherwise, assume the control is not a child of the paint parent.
   pNewParent = 0;
   if ( pPainterWindow->m_pPaintParent )
   {
      pNewParent = pPainterWindow->m_pPaintParent->
                                    FindParentObject( rectPaint, 0, 1 );
   }

   // Before painting the control, ensure that the parent and child are
   // happy with each other ... Note:  this code is very similar to the
   // code in TZCtrl::ValidateParentObject (and should thereby be kept
   // in sync).
   lParentType = 0;
   pCtrl = 0;

   GetViewByName( &vCtrlDef, szlTZPESRCO, m_vSubtask, zLEVEL_TASK );
   if ( pNewParent )
   {
      // If the parent wants someone else to be the parent, give
      // it the chance to let us know now!
      pNewParent = pNewParent->GetRealCtrl( );

      lParentType = pNewParent->m_lType;
      CreateViewFromViewForTask( &vParentDef, vCtrlDef, m_vSubtask );
      SetCursorFirstEntityByInteger( vParentDef, szlControlDef, szlKey,
                                     lParentType, 0 );
   }
   else
      vParentDef = 0;

   SetCursorFirstEntityByInteger( vCtrlDef, szlControlDef,
                                  szlKey, lNewType, 0 );
   lWork = CtrlValidate( vCtrlDef, vParentDef, 0, zCTRL_VALID_PAINT );

   while ( pNewParent && lWork == 1 )
   {
      pNewParent = pNewParent->FindParentObject( rectPaint, pNewParent, 1 );
      if ( pNewParent )
      {
         SetCursorFirstEntityByInteger( vParentDef, szlControlDef, szlKey,
                                        pNewParent->m_lType, 0 );
         lWork = CtrlValidate( vCtrlDef, vParentDef, 0, zCTRL_VALID_PAINT );
      }
   }

   if ( lWork < 0 )
      lNewType = 0;

   if ( pNewParent && lNewType )
   {
      if ( pNewParent->IsChildValid( lNewType ) == FALSE )
      {
         pNewParent = 0;
         lNewType = 0;
      }
   }

   pPainterWindow->m_pPaintParent = 0;
   if ( lNewType )
   {
      // Get the ID of the control to be painted.
      GetIntegerFromAttribute( &lNewType, vCtrlDef, szlControlDef, szlKey );

      // If the parent of the control is to be the client area, then
      // zero out the candidate parent and update the paint position of
      // the control.
      if ( vParentDef )
      {
         if ( lWork == 1 )
         {
            lParentType = 0;
         }
         else
         if ( pNewParent )
         {
//          pNewParent->ParentToPainterRect( rectParent );
            pCtrl = pNewParent;
            while ( pCtrl )
               pCtrl = pCtrl->AdjustCtrlRect( rectPaint, FALSE );

//          rectPaint.left   -= rectParent.left;
//          rectPaint.top    -= rectParent.top;
//          rectPaint.right  -= rectParent.left;
//          rectPaint.bottom -= rectParent.top;
         }
      }
   }

   // Drop the view to the parent control if one was created.
   if ( vParentDef )
      DropView( vParentDef );

   if ( lNewType == 0 )
   {
      ::MessageBeep( MB_ICONQUESTION );
   }
   else
   {
      pCtrl = pPainterWindow->PaintCtrl( pNewParent, szlControl,
                                         0, lNewType, &rectPaint,
                                         szTag, szTag );
      if ( pCtrl )
      {
         zSHORT nPos = pPainterWindow->m_nCtrlCnt;
         zSHORT nIdNbr;
         zCHAR  szLabel[ 64 ];

         if ( pCtrl->GetVisibility( ) )   // not visible/selectable
         {
            if ( pCtrl->m_pCtrlParent &&
                 pCtrl->m_pCtrlParent->GetVisibility( ) ) // not visible/selectable
            {
               pCtrl->m_uVisible = 0;  // if we are painting it from a
                                       // ToolBar, it must be visible!!!
            }
         }

         while ( nPos > 0 )
         {
            nPos--;
            if ( pCtrl == pPainterWindow->m_CtrlList[ nPos ] )
            {
               if ( pPainterWindow->m_pUndoCurr )
                  nIdNbr = pPainterWindow->m_pUndoCurr->m_nIdNbr + 1;
               else
                  nIdNbr = 1;

               pPainterWindow->GenerateTag( szTag, lNewType, FALSE );
               sprintf_s( szLabel, sizeof( szLabel ), "&xxdo Create %s control\tCtrl+x", szTag );
               pPainterWindow->AddToUndo( pCtrl, nIdNbr, nPos, FALSE,
                                          TZUndoRedo::eCreate, szLabel );
               break;
            }
         }
      }

#if 0 // this doesn't seem to help
      if ( pCtrl )
      {
         // We're not fooling around anymore ... we are going to create
         // the corresponding Zeidon instance right here and now ...
         // it should not be a performance problem.
#if 1
         pPainterWindow->UpdateZeidonWindow( );
#else
         pCtrl->PositionOnZeidonCtrl( pPainterWindow->m_vDialog );
         vCtrlDef = pCtrl->m_vCtrl;

         SetAttributeFromInteger( vCtrlDef, szlControl, szlPSDLG_X, pCtrl->Attr.X );
         SetAttributeFromInteger( vCtrlDef, szlControl, szlPSDLG_Y, pCtrl->Attr.Y );
         SetAttributeFromInteger( vCtrlDef, szlControl, szlSZDLG_X, pCtrl->Attr.W );
         SetAttributeFromInteger( vCtrlDef, szlControl, szlSZDLG_Y, pCtrl->Attr.H );
         SetAttributeFromInteger( vCtrlDef, szlControl, szlSyncKey, 9999 );

         // Reset the dialog view from the owning painter
         while ( ResetViewFromSubobject( pPainterWindow->m_vDialog ) == 0 )
         {
         }
#endif
         if ( pCtrl->m_lType == 2090 || pCtrl->m_lType == 2091 )
            DisplayEntityInstance( pCtrl->m_vCtrl, "Control" );
      }
#endif

   }

   if ( (pPainterWindow->m_fWndState & zTZPW_SCROLLADJUSTED) == 0 )
   {
      SetRedraw( TRUE );
      pPainterWindow->m_pPainterClient->ScrollToPosition( ptScrollPos  );
   }

   // Return TRUE, we at least tried to paint a ctrl.
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
//
// Create a font using specifications from the Zeidon.ini file
//
// HFONT CreateFont( int nHeight, // logical height of font
//                   int nWidth, // logical average character width
//                   int nEscapement, // angle of escapement
//                   int nOrientation, // base-line orientation angle
//                   int fnWeight, // font weight
//                   DWORD fdwItalic, // italic attribute flag
//                   DWORD fdwUnderline, // underline attribute flag
//                   DWORD fdwStrikeOut, // strikeout attribute
//                   DWORD fdwCharSet, // character set identifier
//                   DWORD fdwOutputPrecision, // output precision
//                   DWORD fdwClipPrecision, // clipping precision
//                   DWORD fdwQuality, // output quality
//                   DWORD fdwPitchAndFamily, // pitch and family
//                   LPCTSTR lpszFace // pointer to typeface name string
//                 );
//
/////////////////////////////////////////////////////////////////////////////
void
TZPainterBar::CreateDefaultFont( )
{
// TraceLineS( "TZPainterBar::CreateDefaultFont", "" );
   zVIEW  vTaskLPLR;
   zCHAR  szFaceName[ 32 ];  // LF_FACENAME == 32
   zLONG  lFontSize;
   zSHORT nFont;


   CreateAppView( &vTaskLPLR );
   GetWorkstationApplicationValues( vTaskLPLR, "Font", szFaceName,
                                    sizeof( szFaceName ), &lFontSize,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0 );
   if ( szFaceName[ 0 ] == 'Z' &&
        (zstrcmp( szFaceName, "ZeidonOld" ) == 0 || zstrcmp( szFaceName, "ZeidonSize" ) == 0) )
   {
      if ( szFaceName[ 6 ] == 'S' )    // "ZeidonSize"
         nFont = -1;
      else
         nFont = 1;
   }
   else
   if ( szFaceName[ 0 ] && lFontSize )
   {
      nFont = 1;
      if ( lFontSize < 0 )
         lFontSize = -lFontSize;

      LOGFONT logFont;
      zmemset( &logFont, 0, sizeof( LOGFONT ) );
      logFont.lfCharSet = DEFAULT_CHARSET;
      logFont.lfHeight = lFontSize;
      strcpy_s( logFont.lfFaceName, sizeof( logFont.lfFaceName ), szFaceName );
      HDC hDC = ::GetDC( 0 );

      ::ReleaseDC( 0, hDC );

      m_pFontWS = new CFont;  // font override for Workstation
      if ( m_pFontWS->CreateFontIndirect( &logFont ) == 0 )
      {
         mDeleteInit( m_pFontWS );
      }
   }
   else
   {
      nFont = 0;
      m_pFontWS = 0;
   }

   GetWorkstationApplicationValues( vTaskLPLR, "FontCtrl", szFaceName,
                                    sizeof( szFaceName ), &lFontSize,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0 );
   m_pFontCtrl = new CFont;  // font override for controls
   if ( szFaceName[ 0 ] && lFontSize )
   {
      if ( m_pFontCtrl->CreatePointFont( lFontSize, szFaceName ) == 0 )
      {
         mDeleteInit( m_pFontCtrl );
      }
   }
   else
   {
      if ( nFont > 0 )
      {
         mDeleteInit( m_pFontCtrl );
      }
      else
      {
         zBOOL bRC = m_pFontCtrl->CreatePointFont( 80, _T( "MS Sans Serif" ) );
         if ( bRC == 0 )
         {
            mDeleteInit( m_pFontCtrl );
         }
         else
         if ( nFont == 0 )
         {
            m_bUseMapDlgUnits = TRUE;
         }
      }
   }

// SYSTEM_FONT
// DEFAULT_GUI_FONT
// m_hFont = (HFONT) ::GetStockObject( SYSTEM_FONT );
}

void
TZPainterBar::CreateAppView( zVIEW *pvApp )
{
// TraceLineS( "TZPainterBar::CreateAppView", "" );
   zVIEW vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", m_vSubtask, zLEVEL_TASK );
   if ( vTaskLPLR )
   {
      zPCHAR  pchName;

      GetAddrForAttribute( &pchName, vTaskLPLR, "LPLR", "Name" );
      if ( zstrcmp( pchName, m_csLPLR_Name.GetString() ) == 0 )
      {
         if ( m_vTaskLPLR == 0  )
         {
            SfCreateSubtask( pvApp, 0, pchName );
            SfLockView( m_vTaskLPLR );
            m_vTaskLPLR = *pvApp;
         }
         else
            *pvApp = m_vTaskLPLR;
      }
      else
      {
         m_csLPLR_Name = pchName;
         if ( m_vTaskLPLR )
            SfDropSubtask( m_vTaskLPLR, 0 );

         SfCreateSubtask( pvApp, m_vSubtask, pchName );
         m_vTaskLPLR = *pvApp;
      }
   }
   else
      *pvApp = 0;
}

void
TZPainterBar::ResetSelectedMouseReleaseSwap( zBOOL bDeselect )
{
   if ( bDeselect )
   {
      TZPainterWindow *pPainterWindow;
      TZPainterCtrl *pCtrl;
      zSHORT nPosWnd;
      zSHORT nPosCtrl;

      for ( nPosWnd = 0; nPosWnd < m_nWndCnt; nPosWnd++ )
      {
         pPainterWindow = m_PainterWindowList[ nPosWnd ];

         nPosCtrl = pPainterWindow->m_nCtrlCnt;
         while ( nPosCtrl-- )
         {
            pCtrl = pPainterWindow->m_CtrlList[ nPosCtrl ];
            if ( pCtrl->IsSelected( ) )
            {
               mDeleteInit( pCtrl->m_pCtrlCover->m_pSelectCover->m_pTracker );
               pCtrl->Deselect( );
               pCtrl->m_pWndCtrl->Invalidate( );
            }
         }
      }
   }

   zBOOL bRC = ReleaseCapture( );
// TraceLineS( "tzctlptb ReleaseCapture: ", bRC ? "TRUE" : "FALSE" );
   bRC = SwapMouseButton( m_bMouseSwapped );
// TraceLineS( "tzctlptb SwapMouse: ", bRC ? "TRUE" : "FALSE" );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern "C" {

zLONG OPERATION
PainterPaintControl( zSHORT nMessage,
                     TZPainterWindow *pPainterWindow,
                     zLONG  lParentChildType,
                     CWnd   *pWndParent,
                     zVIEW  vCtrl,
                     CRect& rect,
                     TZPainterCtrl *pCtrl,
                     zLONG  lType,
                     zLONG  lZKey,
                     zLONG  lCtrlID,
                     zLONG  lTabStopNbr,
                     zPCHAR pchTag,
                     zPCHAR pchText )
{
   return( 0 );
}

}  // END of C 'wrapper' message stubs

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZPaletteBar

IMPLEMENT_DYNAMIC( TZPalette, CToolBar )
BEGIN_MESSAGE_MAP( TZPalette, CToolBar )
   ON_WM_CREATE( )
   ON_CONTROL_REFLECT( CN_COMMAND, OnControlReflectCommand )
   ON_MESSAGE( zZEIDON_GET_TOOLTIP_TEXT, OnGetToolTipText )
END_MESSAGE_MAP( )


/////////////////////////////////////////////////////////////////////////////
// TZPalette construction/destruction

TZPalette::TZPalette( TZPainterBar *pPainterBar )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::TZPalette", "" );
#endif

   m_pBar = pPainterBar;
   m_uColumns = 2;
   m_cxLeftBorder = 5;
   m_cxRightBorder = 5;
   m_cyTopBorder = 5;
   m_cyBottomBorder = 5;
}

TZPalette::~TZPalette( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::dtor", "" );
#endif
}

/////////////////////////////////////////////////////////////////////////////
// TZPalette diagnostics

#ifdef _DEBUG
void
TZPalette::AssertValid( ) const
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPalette::AssertValid", "" );
#endif

   CToolBar::AssertValid( );

#ifdef DEBUG_ALL
// TraceLineS( "TZPalette::AssertValid", " Complete" );
#endif
}

void
TZPalette::Dump( CDumpContext& dc ) const
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::Dump", "" );
#endif

   CToolBar::Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TZPalette message handlers

void
TZPalette::SetColumns( zUSHORT uColumns )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::SetColumns", "" );
#endif

   m_uColumns = uColumns;
   zSHORT nCount = GetToolBarCtrl( ).GetButtonCount( );

   for ( int k = 0; k < nCount; k++ )
   {
      UINT uStyle = GetButtonStyle( k );
      BOOL bWrap = (((k + 1) % uColumns) == 0);
      if (bWrap)
         uStyle |= TBBS_WRAPPED;
      else
         uStyle &= ~TBBS_WRAPPED;

      SetButtonStyle( k, uStyle );
   }

   Invalidate( );
   GetParentFrame( )->RecalcLayout( );
}

void
TZPalette::OnControlReflectCommand( )
{
   const MSG *pMsg = CWnd::GetCurrentMessage( );
   UINT uID = LOWORD( pMsg->wParam );
   UINT uNotifyCode = HIWORD( pMsg->wParam );
   HWND hWndCtrl = (HWND) pMsg->lParam;

#ifdef DEBUG_ALL
   TraceLineI( "TZPalette::OnControlReflectCommand NotifyCode = ", uNotifyCode );
   TraceLineI( "TZPalette::OnControlReflectCommand hWndCtrl = ", (zLONG) hWndCtrl );
   TraceLineI( "TZPalette::OnControlReflectCommand uID = ", uID );
#endif

   for ( UINT k = 0; k < m_pBar->m_uPaletteItemCnt; k++ )
   {
      if ( m_pBar->m_PaletteBarItems[ k ].chState &&
           m_pBar->m_PaletteBarItems[ k ].uCmdId == uID )
      {
         m_pBar->m_uSelectedPaletteItem = k;
//       TraceLineS( "Palette selected: ", m_pBar->m_PaletteBarItems[ k ].szTag );
         MB_SetMessage( m_pBar->m_vSubtask, 3, m_pBar->m_PaletteBarItems[ k ].szTag );

         if ( m_pBar->m_pActivePainterWindow && m_pBar->m_pActivePainterWindow->m_pPainterClient )
         {
            if ( m_pBar->m_uSelectedPaletteItem )
            {
               ::SetClassLong( m_pBar->m_pActivePainterWindow->m_pPainterClient->m_hWnd,
                               GCL_HCURSOR, (zLONG) ::LoadCursor( 0, IDC_CROSS ) );
               if ( m_pBar->m_pActivePainterWindow->m_nCtrlCnt )
               {
                  TZPainterCtrl *pCtrl =
                      m_pBar->m_pActivePainterWindow->m_CtrlList[ 0 ];
                  if ( pCtrl->m_pCtrlCover )
                     ::SetClassLong( pCtrl->m_pCtrlCover->m_hWnd,
                                     GCL_HCURSOR, (zLONG) ::LoadCursor( 0, IDC_CROSS ) );
               }
            }
            else
            {
               ::SetClassLong( m_pBar->m_pActivePainterWindow->m_pPainterClient->m_hWnd,
                               GCL_HCURSOR, (zLONG) ::LoadCursor( 0, IDC_ARROW ) );
               if ( m_pBar->m_pActivePainterWindow->m_nCtrlCnt )
               {
                  TZPainterCtrl *pCtrl =
                      m_pBar->m_pActivePainterWindow->m_CtrlList[ 0 ];
                  if ( pCtrl->m_pCtrlCover )
                     ::SetClassLong( pCtrl->m_pCtrlCover->m_hWnd,
                                     GCL_HCURSOR, (zLONG) ::LoadCursor( 0, IDC_ARROW ) );
               }
            }
         }

         break;
      }
   }
}

void
TZPalette::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPalette::OnUpdateCmdUI", "" );   // lots of messages
#endif

   // If the second parameter is FALSE, the button is not disabled,
   // otherwise, the button is disabled ... so we need to check the
   // status of the appropriate action and enable/disable appropriately.
   CToolBar::OnUpdateCmdUI( pTarget, FALSE );
}

LRESULT
TZPalette::OnGetToolTipText( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::OnGetToolTipText", "" );
#endif

   zPCHAR pchText = (zPCHAR) lParam;
   pchText[ 0 ] = 0;
   for ( UINT k = 0; k < m_pBar->m_uPaletteItemCnt; k++ )
   {
      if ( m_pBar->m_PaletteBarItems[ k ].chState &&
           m_pBar->m_PaletteBarItems[ k ].uCmdId == wParam )
      {
         strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, m_pBar->m_PaletteBarItems[ k ].szTag, zZEIDON_TOOLTIP_TEXTLTH - 1 );
         break;
      }
   }

   return( 0 );
}

BOOL
TZPalette::OnCmdMsg( UINT uId, int nCode, void *pExtra,
                     AFX_CMDHANDLERINFO *pHandlerInfo )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPalette::OnCmdMsg uId ", uId );
   TraceLineI( "         OnCmdMsg nCode ", nCode );
   TraceLineI( "   ZSubtask's lMsgCmdId ", m_pBar->m_pZSubtask->m_lMsgCmdId );
#endif

#if 0
   if ( uId == 0 && nCode == 5176814 ) //??? what is 5176814's define???
   {
      for ( UINT k = 0; k < m_pBar->m_uPaletteItemCnt; k++ )
      {
         if ( m_pBar->m_PaletteBarItems[ k ].chState &&
              m_pBar->m_PaletteBarItems[ k ].uCmdId == (UINT) m_pBar->m_pZSubtask->m_lMsgCmdId )
         {
            MB_SetMessage( m_pBar->m_vSubtask, 3, m_pBar->m_PaletteBarItems[ k ].szTag );
            break;
         }
      }
   }
#endif

   return( FALSE );  // not handled here ... pass it on
}

void
TZPalette::ParentNotify( UINT message, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::ParentNotify", "" );
#endif
}

int
TZPalette::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::OnCreate", "" );
#endif

// CControlBarInfo CB_Info;
//
// ReadToolBarPlacement( m_pBar->m_pZSubtask, "ZDR", "Palette", &CB_Info );
//
   return( CToolBar::OnCreate( lpCreateStruct ) );
}

BOOL
TZPalette::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPalette::DestroyWindow", "" );
#endif

   return( CToolBar::DestroyWindow( ) );
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// TZSizingControlBar            Version 2.44
//
// Created: Jan 24, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
//
// The sources and a short version of the docs are also available at
// www.codeproject.com . Look for a "Docking Windows" section and check
// the version to be sure you get the latest one ;)
//
// Hint: These classes are intended to be used as base classes. Do not
// simply add your code to these files - instead create a new class
// derived from one of TZSizingControlBarXX classes and put there what
// you need. See CMyBar classes in the demo projects for examples.
// Modify this file only to fix bugs, and don't forget to send me a copy.
/////////////////////////////////////////////////////////////////////////
// Acknowledgements:
//  o   Thanks to Harlan R. Seymour for his continuous support during
//      development of this code.
//  o   Thanks to Dundas Software for the opportunity
//      to test this code on real-life applications.
//  o   Some ideas for the gripper came from the CToolBarEx flat toolbar
//      by Joerg Koenig. Thanks, Joerg!
//  o   Thanks to Robert Wolpow for the code on which CDockContext based
//      dialgonal resizing is based.
//  o   Thanks to the following people for various bug fixes and/or
//      enhancements: Chris Maunder, Jakawan Ratiwanich, Udo Schaefer,
//      Anatoly Ivasyuk, Peter Hauptmann, DJ(?), Pat Kusbel, Aleksey
//      Malyshev.
//  o   And, of course, many thanks to all of you who used this code,
//      for the invaluable feedback I received.
/////////////////////////////////////////////////////////////////////////

// sizecbar.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar

IMPLEMENT_DYNAMIC( TZSizingControlBar, baseTZSizingControlBar );
BEGIN_MESSAGE_MAP( TZSizingControlBar, baseTZSizingControlBar )
    //{{AFX_MSG_MAP(TZSizingControlBar)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_NCPAINT()
    ON_WM_NCCALCSIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_CAPTURECHANGED()
    ON_WM_SETTINGCHANGE()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCHITTEST()
    ON_WM_CLOSE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

TZSizingControlBar::TZSizingControlBar()
{
    m_szMinHorz = CSize(zTAG_LTH, 32);
    m_szMinVert = CSize(zTAG_LTH, 32);
    m_szMinFloat = CSize(37, 32);
    m_szHorz = CSize(200, 200);
    m_szVert = CSize(200, 200);
    m_szFloat = CSize(200, 200);
    m_bTracking = FALSE;
    m_bKeepSize = FALSE;
    m_bParentSizing = FALSE;
    m_cxEdge = 5;
    m_bDragShowContent = FALSE;
    m_nDockBarID = 0;
    m_dwSCBStyle = 0;
}

TZSizingControlBar::~TZSizingControlBar()
{
}

// old creation method, still here for compatibility reasons
BOOL TZSizingControlBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
                               CSize sizeDefault, BOOL bHasGripper,
                               UINT nID, DWORD dwStyle)
{
    UNUSED_ALWAYS(bHasGripper);

    m_szHorz = m_szVert = m_szFloat = sizeDefault;
    return Create(lpszWindowName, pParentWnd, nID, dwStyle);
}

// preferred creation method
BOOL TZSizingControlBar::Create(LPCTSTR lpszWindowName,
                               CWnd* pParentWnd, UINT nID,
                               DWORD dwStyle)
{
    // must have a parent
    ASSERT_VALID(pParentWnd);
    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

    m_dwStyle = dwStyle & CBRS_ALL; // save the control bar styles

    // register and create the window - skip CControlBar::Create()
    CString wndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        ::GetSysColorBrush(COLOR_BTNFACE), 0);

    dwStyle &= ~CBRS_ALL; // keep only the generic window styles
    dwStyle |= WS_CLIPCHILDREN; // prevents flashing
    if ( !CWnd::Create( wndclass, lpszWindowName, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID ) )
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar operations
#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
void TZSizingControlBar::EnableDocking(DWORD dwDockStyle)
{
    // must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
    // cannot have the CBRS_FLOAT_MULTI style
    ASSERT((dwDockStyle & CBRS_FLOAT_MULTI) == 0);
    // the bar must have CBRS_SIZE_DYNAMIC style
    ASSERT((m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    m_dwDockStyle = dwDockStyle;
    if (m_pDockContext == NULL)
        m_pDockContext = new TZSCBDockContext(this);

    // permanently wire the bar's owner to its current parent
    if (m_hWndOwner == NULL)
        m_hWndOwner = ::GetParent(m_hWnd);
}
#endif

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar message handlers

int TZSizingControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (baseTZSizingControlBar::OnCreate(lpCreateStruct) == -1)
        return( -1 );

    // query SPI_GETDRAGFULLWINDOWS system parameter
    // OnSettingChange() will update m_bDragShowContent
    m_bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bDragShowContent, 0);

    // uncomment this line if you want raised borders
//    m_dwSCBStyle |= SCBS_SHOWEDGES;

    return( 0 );
}


LRESULT TZSizingControlBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);

    LRESULT lResult = CWnd::Default();

    if (IsFloating() &&
        GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
    {
        m_pDockBar->SetWindowText((LPCTSTR) lParam); // update dockbar
        GetParentFrame()->DelayRecalcLayout(); // refresh miniframe
    }

    return( lResult );
}

const BOOL TZSizingControlBar::IsFloating() const
{
    return !IsHorzDocked() && !IsVertDocked();
}

const BOOL TZSizingControlBar::IsHorzDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP || m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

const BOOL TZSizingControlBar::IsVertDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT || m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT);
}

const BOOL TZSizingControlBar::IsSideTracking() const
{
    // don't call this when not tracking
    ASSERT(m_bTracking && !IsFloating());

    return (m_htEdge == HTLEFT || m_htEdge == HTRIGHT) ?
        IsHorzDocked() : IsVertDocked();
}

CSize TZSizingControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    if (bStretch) // the bar is stretched (is not the child of a dockbar)
        if (bHorz)
            return CSize(32767, m_szHorz.cy);
        else
            return CSize(m_szVert.cx, 32767);

    // dirty cast - we need access to protected CDockBar members
    TZSCBDockBar* pDockBar = (TZSCBDockBar*) m_pDockBar;

    // force imediate RecalcDelayShow() for all sizing bars on the row
    // with delayShow/delayHide flags set to avoid IsVisible() problems
    TZSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = pDockBar->m_bLayoutQuery ?
        NULL : ::BeginDeferWindowPos(arrSCBars.GetSize());
    for (int i = 0; i < arrSCBars.GetSize(); i++)
        if (arrSCBars[i]->m_nStateFlags & (delayHide|delayShow))
            arrSCBars[i]->RecalcDelayShow(&layout);
    if (layout.hDWP != NULL)
        ::EndDeferWindowPos(layout.hDWP);

    // get available length
    CRect rc = pDockBar->m_rectLayout;
    if (rc.IsRectEmpty())
        m_pDockSite->GetClientRect(&rc);
    int nLengthTotal = bHorz ? rc.Width() + 2 : rc.Height() - 2;

    if (IsVisible() && !IsFloating() &&
        m_bParentSizing && arrSCBars[0] == this)
        if (NegotiateSpace(nLengthTotal, (bHorz != FALSE)))
            AlignControlBars();

    m_bParentSizing = FALSE;

    if (bHorz)
        return CSize(max(m_szMinHorz.cx, m_szHorz.cx),
                     max(m_szMinHorz.cy, m_szHorz.cy));

    return CSize(max(m_szMinVert.cx, m_szVert.cx),
                 max(m_szMinVert.cy, m_szVert.cy));
}

CSize TZSizingControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) // docked ?
    {
        if (nLength == -1)
            m_bParentSizing = TRUE;

        return baseTZSizingControlBar::CalcDynamicLayout(nLength, dwMode);
    }

    if (dwMode & LM_MRUWIDTH) return m_szFloat;
    if (dwMode & LM_COMMIT) return m_szFloat; // already committed

#ifndef _SCB_REPLACE_MINIFRAME
    // check for dialgonal resizing hit test
    int nHitTest = m_pDockContext->m_nHitTest;
    if (IsFloating() &&
        (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT ||
        nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT))
    {
        CPoint ptCursor;
        ::GetCursorPos(&ptCursor);

        CRect rFrame, rBar;
        GetParentFrame()->GetWindowRect(&rFrame);
        GetWindowRect(&rBar);

        if (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT)
        {
            m_szFloat.cx = rFrame.left + rBar.Width() - ptCursor.x;
            m_pDockContext->m_rectFrameDragHorz.left =
                min(ptCursor.x, rFrame.left + rBar.Width() - m_szMinFloat.cx);
        }

        if (nHitTest == HTTOPLEFT || nHitTest == HTTOPRIGHT)
        {
            m_szFloat.cy = rFrame.top + rBar.Height() - ptCursor.y;
            m_pDockContext->m_rectFrameDragHorz.top =
                min(ptCursor.y, rFrame.top + rBar.Height() - m_szMinFloat.cy);
        }

        if (nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT)
            m_szFloat.cx = rBar.Width() + ptCursor.x - rFrame.right;

        if (nHitTest == HTBOTTOMLEFT || nHitTest == HTBOTTOMRIGHT)
            m_szFloat.cy = rBar.Height() + ptCursor.y - rFrame.bottom;
    }
    else
#endif //_SCB_REPLACE_MINIFRAME
        ((dwMode & LM_LENGTHY) ? m_szFloat.cy : m_szFloat.cx) = nLength;

    m_szFloat.cx = max(m_szFloat.cx, m_szMinFloat.cx);
    m_szFloat.cy = max(m_szFloat.cy, m_szMinFloat.cy);

    return m_szFloat;
}

void TZSizingControlBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    // force non-client recalc if moved or resized
    lpwndpos->flags |= SWP_FRAMECHANGED;

    baseTZSizingControlBar::OnWindowPosChanging(lpwndpos);

    // find on which side are we docked
    m_nDockBarID = GetParent()->GetDlgCtrlID();

    if (!IsFloating())
        if (lpwndpos->flags & SWP_SHOWWINDOW)
            m_bKeepSize = TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void TZSizingControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
        CWnd::OnLButtonDown(nFlags, point);
}

void TZSizingControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_pDockBar != NULL)
    {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
        CWnd::OnLButtonDblClk(nFlags, point);
}

void TZSizingControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    UNUSED_ALWAYS(point);

    if (m_bTracking || IsFloating())
        return;

    if ((nHitTest >= HTSIZEFIRST) && (nHitTest <= HTSIZELAST))
        StartTracking(nHitTest, point); // sizing edge hit
}

void TZSizingControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    baseTZSizingControlBar::OnLButtonUp(nFlags, point);
}

void TZSizingControlBar::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    baseTZSizingControlBar::OnRButtonDown(nFlags, point);
}

void TZSizingControlBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bTracking)
    {
        CPoint ptScreen = point;
        ClientToScreen(&ptScreen);

        OnTrackUpdateSize(ptScreen);
    }

    baseTZSizingControlBar::OnMouseMove(nFlags, point);
}

void TZSizingControlBar::OnCaptureChanged(CWnd *pWnd)
{
    if (m_bTracking && (pWnd != this))
        StopTracking();

    baseTZSizingControlBar::OnCaptureChanged(pWnd);
}

void TZSizingControlBar::OnNcCalcSize(BOOL bCalcValidRects,
                                     NCCALCSIZE_PARAMS FAR* lpncsp)
{
    UNUSED_ALWAYS(bCalcValidRects);

#ifndef _SCB_REPLACE_MINIFRAME
    // Enable diagonal resizing for floating miniframe
    if (IsFloating())
    {
        CFrameWnd* pFrame = GetParentFrame();
        if (pFrame != NULL &&
            pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
        {
            DWORD dwStyle = ::GetWindowLong(pFrame->m_hWnd, GWL_STYLE);
            if ((dwStyle & MFS_4THICKFRAME) != 0)
            {
                pFrame->ModifyStyle(MFS_4THICKFRAME, 0); // clear
                GetParent()->ModifyStyle(0, WS_CLIPCHILDREN);
            }
        }
    }
#endif _SCB_REPLACE_MINIFRAME

    // compute the the client area
    m_dwSCBStyle &= ~SCBS_EDGEALL;

    // add resizing edges between bars on the same row
    if (!IsFloating() && m_pDockBar != NULL)
    {
        TZSCBArray arrSCBars;
        int nThis;
        GetRowSizingBars(arrSCBars, nThis);

        BOOL bHorz = IsHorzDocked();
        if (nThis > 0)
            m_dwSCBStyle |= bHorz ? SCBS_EDGELEFT : SCBS_EDGETOP;

        if (nThis < arrSCBars.GetUpperBound())
            m_dwSCBStyle |= bHorz ? SCBS_EDGERIGHT : SCBS_EDGEBOTTOM;
    }

    NcCalcClient(&lpncsp->rgrc[0], m_nDockBarID);
}

void TZSizingControlBar::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    CRect rc(pRc);

    rc.DeflateRect(3, 5, 3, 3);
    if (nDockBarID != AFX_IDW_DOCKBAR_FLOAT)
        rc.DeflateRect(2, 0, 2, 2);

    switch (nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        m_dwSCBStyle |= SCBS_EDGEBOTTOM;
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        m_dwSCBStyle |= SCBS_EDGETOP;
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        m_dwSCBStyle |= SCBS_EDGERIGHT;
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        m_dwSCBStyle |= SCBS_EDGELEFT;
        break;
    }

    // make room for edges only if they will be painted
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rc.DeflateRect(
            (m_dwSCBStyle & SCBS_EDGELEFT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGETOP) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGERIGHT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGEBOTTOM) ? m_cxEdge : 0);

    *pRc = rc;
}

void TZSizingControlBar::OnNcPaint()
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this); // the HDC will be released by the destructor

    CRect rcClient, rcBar;
    GetClientRect(rcClient);
    ClientToScreen(rcClient);
    GetWindowRect(rcBar);
    rcClient.OffsetRect(-rcBar.TopLeft());
    rcBar.OffsetRect(-rcBar.TopLeft());

    CDC mdc;
    mdc.CreateCompatibleDC(&dc);

    CBitmap bm;
    bm.CreateCompatibleBitmap(&dc, rcBar.Width(), rcBar.Height());
    CBitmap* pOldBm = mdc.SelectObject(&bm);

    // draw borders in non-client area
    CRect rcDraw = rcBar;
    DrawBorders(&mdc, rcDraw);

    // erase the NC background
    mdc.FillRect(rcDraw, CBrush::FromHandle(
        (HBRUSH) GetClassLong(m_hWnd, GCL_HBRBACKGROUND)));

    if (m_dwSCBStyle & SCBS_SHOWEDGES)
    {
        CRect rcEdge; // paint the sizing edges
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                mdc.Draw3dRect(rcEdge, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                    ::GetSysColor(COLOR_BTNSHADOW));
    }

    NcPaintGripper(&mdc, rcClient);

    // client area is not our bussiness :)
    dc.IntersectClipRect(rcBar);
    dc.ExcludeClipRect(rcClient);

    dc.BitBlt(0, 0, rcBar.Width(), rcBar.Height(), &mdc, 0, 0, SRCCOPY);

    mdc.SelectObject(pOldBm);
    bm.DeleteObject();
    mdc.DeleteDC();
}

void TZSizingControlBar::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    UNUSED_ALWAYS(pDC);
    UNUSED_ALWAYS(rcClient);
}

void TZSizingControlBar::OnPaint()
{
    // overridden to skip border painting based on clientrect
    CPaintDC dc(this);
}

LRESULT TZSizingControlBar::OnNcHitTest(CPoint point)
{
    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);

    if (!IsFloating())
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                if (rcEdge.PtInRect(point))
                    return GetEdgeHTCode(i);

    return HTCLIENT;
}

void TZSizingControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    baseTZSizingControlBar::OnSettingChange(uFlags, lpszSection);

    m_bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
        &m_bDragShowContent, 0); // update
}

void TZSizingControlBar::OnSize(UINT nType, int cx, int cy)
{
    UNUSED_ALWAYS(nType);

    if ((m_dwSCBStyle & SCBS_SIZECHILD) != 0)
    {
        // automatic child resizing - only one child is allowed
        CWnd* pWnd = GetWindow(GW_CHILD);
        if (pWnd != NULL)
        {
            pWnd->MoveWindow(0, 0, cx, cy);
        //  ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
        }
    }
}

void TZSizingControlBar::OnClose()
{
    // do nothing: protection against accidentally destruction by the
    //   child control (i.e. if user hits Esc in a child editctrl)
}

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar implementation helpers

void TZSizingControlBar::StartTracking(UINT nHitTest, CPoint point)
{
    SetCapture();

    // make sure no updates are pending
    if (!m_bDragShowContent)
        RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

    m_htEdge = nHitTest;
    m_bTracking = TRUE;

    BOOL bHorz = IsHorzDocked();
    BOOL bHorzTracking = m_htEdge == HTLEFT || m_htEdge == HTRIGHT;

    m_nTrackPosOld = bHorzTracking ? point.x : point.y;

    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);
    GetEdgeRect(rcBar, m_htEdge, rcEdge);
    m_nTrackEdgeOfs = m_nTrackPosOld -
        (bHorzTracking ? rcEdge.CenterPoint().x : rcEdge.CenterPoint().y);

    TZSCBArray arrSCBars;
    int nThis;
    GetRowSizingBars(arrSCBars, nThis);

    m_nTrackPosMin = m_nTrackPosMax = m_nTrackPosOld;
    if (!IsSideTracking())
    {
        // calc minwidth as the max minwidth of the sizing bars on row
        int nMinWidth = bHorz ? m_szMinHorz.cy : m_szMinVert.cx;
        for (int i = 0; i < arrSCBars.GetSize(); i++)
            nMinWidth = max(nMinWidth, bHorz ?
                arrSCBars[i]->m_szMinHorz.cy :
                arrSCBars[i]->m_szMinVert.cx);
        int nExcessWidth = (bHorz ? m_szHorz.cy : m_szVert.cx) - nMinWidth;

        // the control bar cannot grow with more than the width of
        // remaining client area of the mainframe
        CRect rcT;
        m_pDockSite->RepositionBars(0, 0xFFFF, AFX_IDW_PANE_FIRST,
            reposQuery, &rcT, NULL, TRUE);
        int nMaxWidth = bHorz ? rcT.Height() - 2 : rcT.Width() - 2;

        BOOL bTopOrLeft = m_htEdge == HTTOP || m_htEdge == HTLEFT;

        m_nTrackPosMin -= bTopOrLeft ? nMaxWidth : nExcessWidth;
        m_nTrackPosMax += bTopOrLeft ? nExcessWidth : nMaxWidth;
    }
    else
    {
        // side tracking:
        // max size is the actual size plus the amount the other
        // sizing bars can be decreased until they reach their minsize
        if (m_htEdge == HTBOTTOM || m_htEdge == HTRIGHT)
            nThis++;

        for (int i = 0; i < arrSCBars.GetSize(); i++)
        {
            TZSizingControlBar* pBar = arrSCBars[i];

            int nExcessWidth = bHorz ?
                pBar->m_szHorz.cx - pBar->m_szMinHorz.cx :
                pBar->m_szVert.cy - pBar->m_szMinVert.cy;

            if (i < nThis)
                m_nTrackPosMin -= nExcessWidth;
            else
                m_nTrackPosMax += nExcessWidth;
        }
    }

    OnTrackInvertTracker(); // draw tracker
}

void TZSizingControlBar::StopTracking()
{
    OnTrackInvertTracker(); // erase tracker

    m_bTracking = FALSE;
    ReleaseCapture();

    m_pDockSite->DelayRecalcLayout();
}

void TZSizingControlBar::OnTrackUpdateSize(CPoint& point)
{
    ASSERT(!IsFloating());

    BOOL bHorzTrack = m_htEdge == HTLEFT || m_htEdge == HTRIGHT;

    int nTrackPos = bHorzTrack ? point.x : point.y;
    nTrackPos = max(m_nTrackPosMin, min(m_nTrackPosMax, nTrackPos));

    int nDelta = nTrackPos - m_nTrackPosOld;

    if (nDelta == 0)
        return; // no pos change

    OnTrackInvertTracker(); // erase tracker

    m_nTrackPosOld = nTrackPos;

    BOOL bHorz = IsHorzDocked();

    CSize sizeNew = bHorz ? m_szHorz : m_szVert;
    switch (m_htEdge)
    {
    case HTLEFT:    sizeNew -= CSize(nDelta, 0); break;
    case HTTOP:     sizeNew -= CSize(0, nDelta); break;
    case HTRIGHT:   sizeNew += CSize(nDelta, 0); break;
    case HTBOTTOM:  sizeNew += CSize(0, nDelta); break;
    }

    TZSCBArray arrSCBars;
    int nThis;
    GetRowSizingBars(arrSCBars, nThis);

    if (!IsSideTracking())
        for (int i = 0; i < arrSCBars.GetSize(); i++)
        {
            TZSizingControlBar* pBar = arrSCBars[i];
            // make same width (or height)
            (bHorz ? pBar->m_szHorz.cy : pBar->m_szVert.cx) =
                bHorz ? sizeNew.cy : sizeNew.cx;
        }
    else
    {
        int nGrowingBar = nThis;
        BOOL bBefore = m_htEdge == HTTOP || m_htEdge == HTLEFT;
        if (bBefore && nDelta > 0)
            nGrowingBar--;
        if (!bBefore && nDelta < 0)
            nGrowingBar++;
        if (nGrowingBar != nThis)
            bBefore = !bBefore;

        // nGrowing is growing
        nDelta = abs(nDelta);
        TZSizingControlBar* pBar = arrSCBars[nGrowingBar];
        (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) += nDelta;

        // the others are shrinking
        int nFirst = bBefore ? nGrowingBar - 1 : nGrowingBar + 1;
        int nLimit = bBefore ? -1 : arrSCBars.GetSize();

        for (int i = nFirst; nDelta != 0 && i != nLimit; i += (bBefore ? -1 : 1))
        {
            TZSizingControlBar* pBar = arrSCBars[i];

            int nDeltaT = min(nDelta,
                (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) -
                (bHorz ? pBar->m_szMinHorz.cx : pBar->m_szMinVert.cy));

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) -= nDeltaT;
            nDelta -= nDeltaT;
        }
    }

    OnTrackInvertTracker(); // redraw tracker at new pos

    if (m_bDragShowContent)
        m_pDockSite->DelayRecalcLayout();
}

void TZSizingControlBar::OnTrackInvertTracker()
{
    ASSERT(m_bTracking);

    if (m_bDragShowContent)
        return; // don't show tracker if DragFullWindows is on

    BOOL bHorz = IsHorzDocked();
    CRect rc, rcBar, rcDock, rcFrame;
    GetWindowRect(rcBar);
    m_pDockBar->GetWindowRect(rcDock);
    m_pDockSite->GetWindowRect(rcFrame);
    VERIFY(GetEdgeRect(rcBar, m_htEdge, rc));
    if (!IsSideTracking())
        rc = bHorz ?
            CRect(rcDock.left + 1, rc.top, rcDock.right - 1, rc.bottom) :
            CRect(rc.left, rcDock.top + 1, rc.right, rcDock.bottom - 1);

    BOOL bHorzTracking = m_htEdge == HTLEFT || m_htEdge == HTRIGHT;
    int nOfs = m_nTrackPosOld - m_nTrackEdgeOfs;
    nOfs -= bHorzTracking ? rc.CenterPoint().x : rc.CenterPoint().y;
    rc.OffsetRect(bHorzTracking ? nOfs : 0, bHorzTracking ? 0 : nOfs);
    rc.OffsetRect(-rcFrame.TopLeft());

    CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    CBrush* pBrush = CDC::GetHalftoneBrush();
    CBrush* pBrushOld = pDC->SelectObject(pBrush);

    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATINVERT);

    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

BOOL TZSizingControlBar::GetEdgeRect(CRect rcWnd, UINT nHitTest,
                                    CRect& rcEdge)
{
    rcEdge = rcWnd;
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rcEdge.DeflateRect(1, 1);
    BOOL bHorz = IsHorzDocked();

    switch (nHitTest)
    {
    case HTLEFT:
        if (!(m_dwSCBStyle & SCBS_EDGELEFT)) return FALSE;
        rcEdge.right = rcEdge.left + m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTTOP:
        if (!(m_dwSCBStyle & SCBS_EDGETOP)) return FALSE;
        rcEdge.bottom = rcEdge.top + m_cxEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cxEdge, 0);
        break;
    case HTRIGHT:
        if (!(m_dwSCBStyle & SCBS_EDGERIGHT)) return FALSE;
        rcEdge.left = rcEdge.right - m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTBOTTOM:
        if (!(m_dwSCBStyle & SCBS_EDGEBOTTOM)) return FALSE;
        rcEdge.top = rcEdge.bottom - m_cxEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cxEdge, 0);
        break;
    default:
        ASSERT(FALSE); // invalid hit test code
    }
    return TRUE;
}

UINT TZSizingControlBar::GetEdgeHTCode(int nEdge)
{
    if (nEdge == 0) return HTLEFT;
    if (nEdge == 1) return HTTOP;
    if (nEdge == 2) return HTRIGHT;
    if (nEdge == 3) return HTBOTTOM;
    ASSERT(FALSE); // invalid edge code
    return HTNOWHERE;
}

void TZSizingControlBar::GetRowInfo(int& nFirst, int& nLast, int& nThis)
{
    ASSERT_VALID(m_pDockBar); // verify bounds

    nThis = m_pDockBar->FindBar(this);
    ASSERT(nThis != -1);

    int i, nBars = m_pDockBar->m_arrBars.GetSize();

    // find the first and the last bar in row
    for (nFirst = -1, i = nThis - 1; i >= 0 && nFirst == -1; i--)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nFirst = i + 1;
    for (nLast = -1, i = nThis + 1; i < nBars && nLast == -1; i++)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nLast = i - 1;

    ASSERT((nLast != -1) && (nFirst != -1));
}

void TZSizingControlBar::GetRowSizingBars(TZSCBArray& arrSCBars)
{
    int nThis; // dummy
    GetRowSizingBars(arrSCBars, nThis);
}

void TZSizingControlBar::GetRowSizingBars(TZSCBArray& arrSCBars, int& nThis)
{
    arrSCBars.RemoveAll();

    int nFirstT, nLastT, nThisT;
    GetRowInfo(nFirstT, nLastT, nThisT);

    nThis = -1;
    for (int i = nFirstT; i <= nLastT; i++)
    {
        TZSizingControlBar* pBar =
            (TZSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        if (pBar->IsKindOf(RUNTIME_CLASS(TZSizingControlBar)))
        {
            if (pBar == this)
                nThis = arrSCBars.GetSize();

            arrSCBars.Add(pBar);
        }
    }
}

BOOL TZSizingControlBar::NegotiateSpace(int nLengthTotal, BOOL bHorz)
{
    ASSERT(bHorz == IsHorzDocked());

    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    int nLengthAvail = nLengthTotal;
    int nLengthActual = 0;
    int nLengthMin = 2;
    int nWidthMax = 0;
    TZSizingControlBar* pBar;
    int i;

    for (i = nFirst; i <= nLast; i++)
    {
        pBar = (TZSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        BOOL bIsSizingBar =
            pBar->IsKindOf(RUNTIME_CLASS(TZSizingControlBar));

        int nLengthBar; // minimum length of the bar
        if (bIsSizingBar)
            nLengthBar = bHorz ? pBar->m_szMinHorz.cx - 2 :
                pBar->m_szMinVert.cy - 2;
        else
        {
            CRect rcBar;
            pBar->GetWindowRect(&rcBar);
            nLengthBar = bHorz ? rcBar.Width() - 2 : rcBar.Height() - 2;
        }

        nLengthMin += nLengthBar;
        if (nLengthMin > nLengthTotal)
        {
            // split the row after fixed bar
            if (i < nThis)
            {
                m_pDockBar->m_arrBars.InsertAt(i + 1,
                    (CControlBar*) NULL);
                return FALSE;
            }

            // only this sizebar remains on the row, adjust it to minsize
            if (i == nThis)
            {
                if (bHorz)
                    m_szHorz.cx = m_szMinHorz.cx;
                else
                    m_szVert.cy = m_szMinVert.cy;

                return TRUE; // the dockbar will split the row for us
            }

            // we have enough bars - go negotiate with them
            m_pDockBar->m_arrBars.InsertAt(i, (CControlBar*) NULL);
            nLast = i - 1;
            break;
        }

        if (bIsSizingBar)
        {
            nLengthActual += bHorz ? pBar->m_szHorz.cx - 2 :
                pBar->m_szVert.cy - 2;
            nWidthMax = max(nWidthMax, bHorz ? pBar->m_szHorz.cy :
                pBar->m_szVert.cx);
        }
        else
            nLengthAvail -= nLengthBar;
    }

    TZSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    int nNumBars = arrSCBars.GetSize();
    int nDelta = nLengthAvail - nLengthActual;

    // return faster when there is only one sizing bar per row (this one)
    if (nNumBars == 1)
    {
        ASSERT(arrSCBars[0] == this);

        if (nDelta == 0)
            return TRUE;

        m_bKeepSize = FALSE;
        (bHorz ? m_szHorz.cx : m_szVert.cy) += nDelta;

        return TRUE;
    }

    // make all the bars the same width
    for ( i = 0; i < nNumBars; i++)
    {
        if (bHorz)
            arrSCBars[i]->m_szHorz.cy = nWidthMax;
        else
            arrSCBars[i]->m_szVert.cx = nWidthMax;
    }

    // distribute the difference between the bars,
    // but don't shrink them below their minsizes
    while (nDelta != 0)
    {
        int nDeltaOld = nDelta;
        for ( i = 0; i < nNumBars; i++)
        {
            pBar = arrSCBars[i];
            int nLMin = bHorz ?
                pBar->m_szMinHorz.cx : pBar->m_szMinVert.cy;
            int nL = bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy;

            if ((nL == nLMin) && (nDelta < 0) || // already at min length
                pBar->m_bKeepSize) // or wants to keep its size
                continue;

            // sign of nDelta
            int nDelta2 = (nDelta == 0) ? 0 : ((nDelta < 0) ? -1 : 1);

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) += nDelta2;
            nDelta -= nDelta2;
            if (nDelta == 0) break;
        }
        // clear m_bKeepSize flags
        if ((nDeltaOld == nDelta) || (nDelta == 0))
        {
            for ( i = 0; i < nNumBars; i++)
                arrSCBars[i]->m_bKeepSize = FALSE;
        }
    }

    return TRUE;
}

void TZSizingControlBar::AlignControlBars()
{
    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    BOOL bHorz = IsHorzDocked();
    BOOL bNeedRecalc = FALSE;
    int nAlign = bHorz ? -2 : 0;

    CRect rc, rcDock;
    m_pDockBar->GetWindowRect(&rcDock);

    for (int i = nFirst; i <= nLast; i++)
    {
        TZSizingControlBar* pBar =
            (TZSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;

        pBar->GetWindowRect(&rc);
        rc.OffsetRect(-rcDock.TopLeft());

        if (pBar->IsKindOf(RUNTIME_CLASS(TZSizingControlBar)))
            rc = CRect(rc.TopLeft(),
                bHorz ? pBar->m_szHorz : pBar->m_szVert);

        if ((bHorz ? rc.left : rc.top) != nAlign)
        {
            if (!bHorz)
                rc.OffsetRect(0, nAlign - rc.top - 2);
            else if (m_nDockBarID == AFX_IDW_DOCKBAR_TOP)
                rc.OffsetRect(nAlign - rc.left, -2);
            else
                rc.OffsetRect(nAlign - rc.left, 0);
            pBar->MoveWindow(rc);
            bNeedRecalc = TRUE;
        }
        nAlign += (bHorz ? rc.Width() : rc.Height()) - 2;
    }

    if (bNeedRecalc)
        m_pDockSite->DelayRecalcLayout();
}

void TZSizingControlBar::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);
}

void TZSizingControlBar::LoadState(LPCTSTR lpszProfileName)
{
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd()); // must be called after Create()

#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
    // compensate the caption miscalculation in CFrameWnd::SetDockState()
    CDockState state;
    state.LoadState(lpszProfileName);

    UINT nID = GetDlgCtrlID();
    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        if (!pInfo->m_bFloating)
            continue;

        // this is a floating dockbar - check the ID array
        for (int j = 0; j < pInfo->m_arrBarID.GetSize(); j++)
            if ((DWORD) pInfo->m_arrBarID[j] == nID)
            {
                // found this bar - offset origin and save settings
                pInfo->m_pointPos.x++;
                pInfo->m_pointPos.y +=
                    ::GetSystemMetrics(SM_CYSMCAPTION) + 1;
                pInfo->SaveState(lpszProfileName, i);
            }
    }
#endif //_SCB_REPLACE_MINIFRAME && !_SCB_MINIFRAME_CAPTION

    CWinApp* pApp = AfxGetApp();

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-SCBar-%d"), lpszProfileName,
        GetDlgCtrlID());

    m_szHorz.cx = max(m_szMinHorz.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeHorzCX"), m_szHorz.cx));
    m_szHorz.cy = max(m_szMinHorz.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeHorzCY"), m_szHorz.cy));

    m_szVert.cx = max(m_szMinVert.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeVertCX"), m_szVert.cx));
    m_szVert.cy = max(m_szMinVert.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeVertCY"), m_szVert.cy));

    m_szFloat.cx = max(m_szMinFloat.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeFloatCX"), m_szFloat.cx));
    m_szFloat.cy = max(m_szMinFloat.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeFloatCY"), m_szFloat.cy));
}

void TZSizingControlBar::SaveState(LPCTSTR lpszProfileName)
{
    // place your SaveState or GlobalSaveState call in
    // CMainFrame's OnClose() or DestroyWindow(), not in OnDestroy()
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd());

    CWinApp* pApp = AfxGetApp();

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-SCBar-%d"), lpszProfileName,
        GetDlgCtrlID());

    pApp->WriteProfileInt(szSection, _T("sizeHorzCX"), m_szHorz.cx);
    pApp->WriteProfileInt(szSection, _T("sizeHorzCY"), m_szHorz.cy);

    pApp->WriteProfileInt(szSection, _T("sizeVertCX"), m_szVert.cx);
    pApp->WriteProfileInt(szSection, _T("sizeVertCY"), m_szVert.cy);

    pApp->WriteProfileInt(szSection, _T("sizeFloatCX"), m_szFloat.cx);
    pApp->WriteProfileInt(szSection, _T("sizeFloatCY"), m_szFloat.cy);
}

void TZSizingControlBar::GlobalLoadState(CFrameWnd* pFrame,
                                        LPCTSTR lpszProfileName)
{
    POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        TZSizingControlBar* pBar =
            (TZSizingControlBar*) pFrame->m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
        if (pBar->IsKindOf(RUNTIME_CLASS(TZSizingControlBar)))
            pBar->LoadState(lpszProfileName);
    }
}

void TZSizingControlBar::GlobalSaveState(CFrameWnd* pFrame,
                                        LPCTSTR lpszProfileName)
{
    POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        TZSizingControlBar* pBar =
            (TZSizingControlBar*) pFrame->m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
        if (pBar->IsKindOf(RUNTIME_CLASS(TZSizingControlBar)))
            pBar->SaveState(lpszProfileName);
    }
}

#ifdef _SCB_REPLACE_MINIFRAME
#ifndef _SCB_MINIFRAME_CAPTION
/////////////////////////////////////////////////////////////////////////////
// TZSCBDockContext Drag Operations

static void AdjustRectangle(CRect& rect, CPoint pt)
{
    int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
                    (pt.x > rect.right) ? (pt.x - rect.right) : 0;
    int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
                    (pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
    rect.OffsetRect(nXOffset, nYOffset);
}

void TZSCBDockContext::StartDrag(CPoint pt)
{
    ASSERT_VALID(m_pBar);
    m_bDragging = TRUE;

    InitLoop();

    ASSERT((m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    // get true bar size (including borders)
    CRect rect;
    m_pBar->GetWindowRect(rect);
    m_ptLast = pt;
    CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
    CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
    CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

    m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
    m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

    // calculate frame dragging rectangle
    m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);

#ifdef _MAC
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz,
        WS_THICKFRAME, WS_EX_FORCESIZEBOX);
#else
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz, WS_THICKFRAME);
#endif
    m_rectFrameDragHorz.DeflateRect(2, 2);
    m_rectFrameDragVert = m_rectFrameDragHorz;

    // adjust rectangles so that point is inside
    AdjustRectangle(m_rectDragHorz, pt);
    AdjustRectangle(m_rectDragVert, pt);
    AdjustRectangle(m_rectFrameDragHorz, pt);
    AdjustRectangle(m_rectFrameDragVert, pt);

    // initialize tracking state and enter tracking loop
    m_dwOverDockStyle = CanDock();
    Move(pt);   // call it here to handle special keys
    Track();
}
#endif //_SCB_MINIFRAME_CAPTION

/////////////////////////////////////////////////////////////////////////////
// TZSCBMiniDockFrameWnd

IMPLEMENT_DYNCREATE(TZSCBMiniDockFrameWnd, baseTZSCBMiniDockFrameWnd);

BEGIN_MESSAGE_MAP(TZSCBMiniDockFrameWnd, baseTZSCBMiniDockFrameWnd)
    //{{AFX_MSG_MAP(TZSCBMiniDockFrameWnd)
    ON_WM_NCLBUTTONDOWN()
    ON_WM_GETMINMAXINFO()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL TZSCBMiniDockFrameWnd::Create(CWnd* pParent, DWORD dwBarStyle)
{
    // set m_bInRecalcLayout to avoid flashing during creation
    // RecalcLayout will be called once something is docked
    m_bInRecalcLayout = TRUE;

    DWORD dwStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|MFS_MOVEFRAME|
        MFS_4THICKFRAME|MFS_SYNCACTIVE|MFS_BLOCKSYSMENU|
        FWS_SNAPTOBARS;

    if (dwBarStyle & CBRS_SIZE_DYNAMIC)
        dwStyle &= ~MFS_MOVEFRAME;

    DWORD dwExStyle = 0;
#ifdef _MAC
    if (dwBarStyle & CBRS_SIZE_DYNAMIC)
        dwExStyle |= WS_EX_FORCESIZEBOX;
    else
        dwStyle &= ~(MFS_MOVEFRAME|MFS_4THICKFRAME);
#endif

    if (!CMiniFrameWnd::CreateEx(dwExStyle,
        NULL, &afxChNil, dwStyle, rectDefault, pParent))
    {
        m_bInRecalcLayout = FALSE;
        return FALSE;
    }
    dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT) ?
        CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
    dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    //pSysMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
    CString strHide;
    if (strHide.LoadString(AFX_IDS_HIDE))
    {
        pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
        pSysMenu->AppendMenu(MF_STRING|MF_ENABLED, SC_CLOSE, strHide);
    }

    // must initially create with parent frame as parent
    if (!m_wndDockBar.Create(pParent, WS_CHILD | WS_VISIBLE | dwStyle,
        AFX_IDW_DOCKBAR_FLOAT))
    {
        m_bInRecalcLayout = FALSE;
        return FALSE;
    }

    // set parent to CMiniDockFrameWnd
    m_wndDockBar.SetParent(this);
    m_bInRecalcLayout = FALSE;

    return TRUE;
}

void TZSCBMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    if (nHitTest == HTCAPTION || nHitTest == HTCLOSE)
    {
        baseTZSCBMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
        return;
    }

    if (GetSizingControlBar() != NULL)
        CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
    else
        baseTZSCBMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
}

TZSizingControlBar* TZSCBMiniDockFrameWnd::GetSizingControlBar()
{
    CWnd* pWnd = GetWindow(GW_CHILD); // get the dockbar
    if (pWnd == NULL)
        return NULL;

    pWnd = pWnd->GetWindow(GW_CHILD); // get the controlbar
    if (pWnd == NULL)
        return NULL;

    if (!pWnd->IsKindOf(RUNTIME_CLASS(TZSizingControlBar)))
        return NULL;

    return (TZSizingControlBar*) pWnd;
}

void TZSCBMiniDockFrameWnd::OnSize(UINT nType, int cx, int cy)
{
    TZSizingControlBar* pBar = GetSizingControlBar();
    if ((pBar != NULL) && (GetStyle() & MFS_4THICKFRAME) == 0
        && pBar->IsVisible() &&
        cx + 4 >= pBar->m_szMinFloat.cx &&
        cy + 4 >= pBar->m_szMinFloat.cy)
        pBar->m_szFloat = CSize(cx + 4, cy + 4);

    baseTZSCBMiniDockFrameWnd::OnSize(nType, cx, cy);
}

void TZSCBMiniDockFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    baseTZSCBMiniDockFrameWnd::OnGetMinMaxInfo(lpMMI);

    TZSizingControlBar* pBar = GetSizingControlBar();
    if (pBar != NULL)
    {
        CRect r(CPoint(0, 0), pBar->m_szMinFloat - CSize(4, 4));
#ifndef _SCB_MINIFRAME_CAPTION
        CMiniFrameWnd::CalcBorders(&r, WS_THICKFRAME);
#else
        CMiniFrameWnd::CalcBorders(&r, WS_THICKFRAME|WS_CAPTION);
#endif //_SCB_MINIFRAME_CAPTION
        lpMMI->ptMinTrackSize.x = r.Width();
        lpMMI->ptMinTrackSize.y = r.Height();
    }
}

void TZSCBMiniDockFrameWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    if ((GetStyle() & MFS_4THICKFRAME) != 0)
    {
        TZSizingControlBar* pBar = GetSizingControlBar();
        if (pBar != NULL)
        {
            lpwndpos->flags |= SWP_NOSIZE; // don't size this time
            // prevents flicker
            pBar->m_pDockBar->ModifyStyle(0, WS_CLIPCHILDREN);

            // enable diagonal resizing
            DWORD dwStyleRemove = MFS_4THICKFRAME;
#ifndef _SCB_MINIFRAME_CAPTION
            // remove caption
            dwStyleRemove |= WS_SYSMENU|WS_CAPTION;
#endif
            ModifyStyle(dwStyleRemove, 0);

            DelayRecalcLayout();
            pBar->PostMessage(WM_NCPAINT);
        }
    }

    CMiniFrameWnd::OnWindowPosChanging(lpwndpos);
}

#endif //_SCB_REPLACE_MINIFRAME

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// TZSizingControlBarG           Version 2.44
//
// Created: Jan 24, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

// sizecbar.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBarG

IMPLEMENT_DYNAMIC(TZSizingControlBarG, baseTZSizingControlBarG);

TZSizingControlBarG::TZSizingControlBarG()
{
    m_cyGripper = 12;
}

TZSizingControlBarG::~TZSizingControlBarG()
{
}

BEGIN_MESSAGE_MAP(TZSizingControlBarG, baseTZSizingControlBarG)
    //{{AFX_MSG_MAP(TZSizingControlBarG)
    ON_WM_NCLBUTTONUP()
    ON_WM_NCHITTEST()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBarG message handlers

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//

void TZSizingControlBarG::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    if (nHitTest == HTCLOSE)
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide

    baseTZSizingControlBarG::OnNcLButtonUp(nHitTest, point);
}

void TZSizingControlBarG::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    CRect rcBar(pRc); // save the bar rect

    // subtract edges
    baseTZSizingControlBarG::NcCalcClient(pRc, nDockBarID);

    if (!HasGripper())
        return;

    CRect rc(pRc); // the client rect as calculated by the base class

    BOOL bHorz = (nDockBarID == AFX_IDW_DOCKBAR_TOP) ||
                 (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);

    if (bHorz)
        rc.DeflateRect(m_cyGripper, 0, 0, 0);
    else
        rc.DeflateRect(0, m_cyGripper, 0, 0);

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn;
    if (bHorz)
        ptOrgBtn = CPoint(rc.left - 13, rc.top);
    else
        ptOrgBtn = CPoint(rc.right - 12, rc.top - 13);

    m_biHide.Move(ptOrgBtn - rcBar.TopLeft());

    *pRc = rc;
}

void TZSizingControlBarG::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if (!HasGripper())
        return;

    // paints a simple "two raised lines" gripper
    // override this if you want a more sophisticated gripper
    CRect gripper = rcClient;
    CRect rcbtn = m_biHide.GetRect();
    BOOL bHorz = IsHorzDocked();

    gripper.DeflateRect(1, 1);
    if (bHorz)
    {   // gripper at left
        gripper.left -= m_cyGripper;
        gripper.right = gripper.left + 3;
        gripper.top = rcbtn.bottom + 3;
    }
    else
    {   // gripper at top
        gripper.top -= m_cyGripper;
        gripper.bottom = gripper.top + 3;
        gripper.right = rcbtn.left - 3;
    }

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    gripper.OffsetRect(bHorz ? 3 : 0, bHorz ? 0 : 3);

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    m_biHide.Paint(pDC);
}

LRESULT TZSizingControlBarG::OnNcHitTest(CPoint point)
{
    CRect rcBar;
    GetWindowRect(rcBar);

    UINT nRet = baseTZSizingControlBarG::OnNcHitTest(point);
    if (nRet != HTCLIENT)
        return nRet;

    CRect rc = m_biHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTCLOSE;

    return HTCLIENT;
}

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBarG implementation helpers

void TZSizingControlBarG::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);

    if (!HasGripper())
        return;

    BOOL bNeedPaint = FALSE;

    CPoint pt;
    ::GetCursorPos(&pt);
    BOOL bHit = (OnNcHitTest(pt) == HTCLOSE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;

    bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

/////////////////////////////////////////////////////////////////////////
// TZSCBButton

TZSCBButton::TZSCBButton()
{
    bRaised = FALSE;
    bPushed = FALSE;
}

void TZSCBButton::Paint(CDC* pDC)
{
    CRect rc = GetRect();

    if (bPushed)
        pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW),
            ::GetSysColor(COLOR_BTNHIGHLIGHT));
    else
        if (bRaised)
            pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                ::GetSysColor(COLOR_BTNSHADOW));

    COLORREF clrOldTextColor = pDC->GetTextColor();
    pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
    font.CreatePointFont(pointsize, _T("Marlett"));
    CFont* oldfont = pDC->SelectObject(&font);

    pDC->TextOut(ptOrg.x + 2, ptOrg.y + 2, CString(_T("r"))); // x-like

    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
}

BOOL TZSizingControlBarG::HasGripper() const
{
#if defined(_SCB_MINIFRAME_CAPTION) || !defined(_SCB_REPLACE_MINIFRAME)
    // if the miniframe has a caption, don't display the gripper
    if (IsFloating())
        return FALSE;
#endif //_SCB_MINIFRAME_CAPTION

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// TZSizingControlBarCF          Version 2.44
//
// Created: Dec 21, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBarCF

IMPLEMENT_DYNAMIC(TZSizingControlBarCF, baseTZSizingControlBarCF);

int CALLBACK EnumFontFamProc(ENUMLOGFONT FAR *lpelf,
                             NEWTEXTMETRIC FAR *lpntm,
                             int FontType,
                             LPARAM lParam)
{
    UNUSED_ALWAYS(lpelf);
    UNUSED_ALWAYS(lpntm);
    UNUSED_ALWAYS(FontType);
    UNUSED_ALWAYS(lParam);

    return( 0 );
}

TZSizingControlBarCF::TZSizingControlBarCF()
{
    m_bActive = FALSE;

    CDC dc;
    dc.CreateCompatibleDC(NULL);

    m_sFontFace = (::EnumFontFamilies(dc.m_hDC,
        _T("Tahoma"), (FONTENUMPROC) EnumFontFamProc, 0) == 0) ?
        _T("Tahoma") : _T("Arial");

    dc.DeleteDC();

}

BEGIN_MESSAGE_MAP(TZSizingControlBarCF, baseTZSizingControlBarCF)
    //{{AFX_MSG_MAP(TZSizingControlBarCF)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

void TZSizingControlBarCF::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    baseTZSizingControlBarCF::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

    if (!HasGripper())
        return;

    BOOL bNeedPaint = FALSE;

    CWnd* pFocus = GetFocus();
    BOOL bActiveOld = m_bActive;

    m_bActive = (pFocus->GetSafeHwnd() && IsChild(pFocus));

    if (m_bActive != bActiveOld)
        bNeedPaint = TRUE;

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

// gradient defines (if not already defined)
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28
#define SPI_GETGRADIENTCAPTIONS         0x1008
#endif

void TZSizingControlBarCF::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if (!HasGripper())
        return;

    // compute the caption rectangle
    BOOL bHorz = IsHorzDocked();
    CRect rcGrip = rcClient;
    CRect rcBtn = m_biHide.GetRect();
    if (bHorz)
    {   // right side gripper
        rcGrip.left -= m_cyGripper + 1;
        rcGrip.right = rcGrip.left + 11;
        rcGrip.top = rcBtn.bottom + 3;
    }
    else
    {   // gripper at top
        rcGrip.top -= m_cyGripper + 1;
        rcGrip.bottom = rcGrip.top + 11;
        rcGrip.right = rcBtn.left - 3;
    }
    rcGrip.InflateRect(bHorz ? 1 : 0, bHorz ? 0 : 1);

    // draw the caption background
    //CBrush br;
    COLORREF clrCptn = m_bActive ?
        ::GetSysColor(COLOR_ACTIVECAPTION) :
        ::GetSysColor(COLOR_INACTIVECAPTION);

    // query gradient info (usually TRUE for Win98/Win2k)
    BOOL bGradient = FALSE;
    ::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS, 0, &bGradient, 0);

    if (!bGradient)
        pDC->FillSolidRect(&rcGrip, clrCptn); // solid color
    else
    {
        // gradient from left to right or from bottom to top
        // get second gradient color (the right end)
        COLORREF clrCptnRight = m_bActive ?
            ::GetSysColor(COLOR_GRADIENTACTIVECAPTION) :
            ::GetSysColor(COLOR_GRADIENTINACTIVECAPTION);

        // this will make 2^6 = 64 fountain steps
        int nShift = 6;
        int nSteps = 1 << nShift;

        for (int i = 0; i < nSteps; i++)
        {
            // do a little alpha blending
            int nR = (GetRValue(clrCptn) * (nSteps - i) +
                      GetRValue(clrCptnRight) * i) >> nShift;
            int nG = (GetGValue(clrCptn) * (nSteps - i) +
                      GetGValue(clrCptnRight) * i) >> nShift;
            int nB = (GetBValue(clrCptn) * (nSteps - i) +
                      GetBValue(clrCptnRight) * i) >> nShift;

            COLORREF cr = RGB(nR, nG, nB);

            // then paint with the resulting color
            CRect r2 = rcGrip;
            if (bHorz)
            {
                r2.bottom = rcGrip.bottom -
                    ((i * rcGrip.Height()) >> nShift);
                r2.top = rcGrip.bottom -
                    (((i + 1) * rcGrip.Height()) >> nShift);
                if (r2.Height() > 0)
                    pDC->FillSolidRect(r2, cr);
            }
            else
            {
                r2.left = rcGrip.left +
                    ((i * rcGrip.Width()) >> nShift);
                r2.right = rcGrip.left +
                    (((i + 1) * rcGrip.Width()) >> nShift);
                if (r2.Width() > 0)
                    pDC->FillSolidRect(r2, cr);
            }
        }
    }

    // draw the caption text - first select a font
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(85, 96, ppi); // 8.5 points at 96 ppi

    LOGFONT lf;
    BOOL bFont = font.CreatePointFont(pointsize, m_sFontFace);
    if (bFont)
    {
        // get the text color
        COLORREF clrCptnText = m_bActive ?
            ::GetSysColor(COLOR_CAPTIONTEXT) :
            ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);

        int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
        COLORREF clrOldText = pDC->SetTextColor(clrCptnText);

        if (bHorz)
        {
            // rotate text 90 degrees CCW if horizontally docked
            font.GetLogFont(&lf);
            font.DeleteObject();
            lf.lfEscapement = 900;
            font.CreateFontIndirect(&lf);
        }

        CFont* pOldFont = pDC->SelectObject(&font);
        CString sTitle;
        GetWindowText(sTitle);

        CPoint ptOrg = bHorz ?
            CPoint(rcGrip.left - 1, rcGrip.bottom - 3) :
            CPoint(rcGrip.left + 3, rcGrip.top - 1);

        pDC->ExtTextOut(ptOrg.x, ptOrg.y,
            ETO_CLIPPED, rcGrip, sTitle, NULL);

        pDC->SelectObject(pOldFont);
        pDC->SetBkMode(nOldBkMode);
        pDC->SetTextColor(clrOldText);
    }

    // draw the button
    m_biHide.Paint(pDC);
}

LRESULT TZSizingControlBarCF::OnSetText(WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = baseTZSizingControlBarCF::OnSetText(wParam, lParam);

    SendMessage( WM_NCPAINT );

    return( lResult );
}
