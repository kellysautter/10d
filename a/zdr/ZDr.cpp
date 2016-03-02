/////////////////////////////////////////////////////////////////////////////
// Project ZDr
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdr.exe - ZDr Application Rendering
// FILE:         zdr.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of the class behaviors for the application
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 1999.12.31    DKS   Z10    RemoteServer
//
// 1999.05.11    DKS    Z10    ActiveX
//    Used Afx initialization of thread.
//
// 1998.12.15    DKS
//    Process Zeidon message for shared memory properly.
//
// 1998.12.15    DKS
//    Synchronize forced close from OE.
//
// 1998.05.29 - 1998.06.09   RG!!
//    Use SfActivateSysEmptyOI to load the kzzdr.lod so that it does
//    not have to be in the application LPLR.

#include "zstdafx.h"

#include "ZDr.h"

#include <process.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

zLONG g_lMapDlgCharX = 0;
zLONG g_lMapDlgCharY = 0;
zLONG g_lDlgUnitCharX = 0;
zLONG g_lDlgUnitCharY = 0;

/////////////////////////////////////////////////////////////////////////////
// The one and only ZDrApp object
//
ZDrApp TheOneAndOnlyZDrApp;

LRESULT CALLBACK
ZeidonCommOE_WndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
   switch ( uMessage )
   {
      case zZEIDON_COMMUNICATE_OE:
      {
#ifdef DEBUG_ALL
         TraceLineI( "ZeidonCommOE COMMNUICATE_OE wParam: ", wParam );
         TraceLineI( "ZeidonCommOE COMMNUICATE_OE lParam: ", lParam );
#endif

#if 1
         if ( wParam == 0 && (lParam == 0 || lParam == -1) )
         {
            // Checking to see if we are alive.
            if ( lParam == 0 )   // is this thread still alive?
               return( -1 );
            else
            {
               // Determine if the main window is still alive.
               HWND hWndMain = (HWND) ::GetWindowLong( hWnd, GWL_USERDATA );
               if ( mIs_hWnd( hWndMain ) )
                  return( -1 );
               else
                  return( 0 );
            }
         }
         else
#endif
         {
            return( ProcessZeidonMessage( wParam, lParam ) );
         }
      }

      case zZEIDON_POSTED_CLOSE:
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZeidonCommOE POSTED_CLOSE", "" );
#endif
         HWND hWndMain = (HWND) ::GetWindowLong( hWnd, GWL_USERDATA );
         if ( hWndMain && mIs_hWnd( hWndMain ) )
         {
            UINT uMsgClose = GetRegisteredMessage( "ZDR_POSTED_CLOSE" );
            ::PostMessage( hWndMain, uMsgClose, -1, (zLONG) hWndMain );
         }

         ::SetWindowLong( hWnd, GWL_USERDATA, 0L );
//       ::PostMessage( hWnd, WM_CLOSE, 0, 0L );
         return( 0 );
      }

      case WM_CLOSE:
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZeidonCommOE WM_CLOSE", "" );
#endif
         HWND hWndMain = (HWND) ::GetWindowLong( hWnd, GWL_USERDATA );
         if ( hWndMain && mIs_hWnd( hWndMain ) )
         {
            ::SetWindowLong( hWnd, GWL_USERDATA, 0L );
            ::SendMessage( hWndMain, WM_CLOSE, 0, 0L );
         }

         AfxEndThread( 0 );

         break;
      }

      case WM_DESTROY:                  // message: window being destroyed
//       AfxEndThread( 0 );
//       return( 0 );
         AfxPostQuitMessage( 0 );
         break;
   }

   return( ::DefWindowProc( hWnd, uMessage, wParam, lParam ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// We need to keep track of whether or not a local thread is its own
// Zeidon task or if it's part of a parent process.  To do this we use
// global variables that are different for each thread.

UINT
ZeidonCommOE_ThreadProc( zPVOID pParam )
{
   MSG        msg;
   ZDrApp *pDrApp = (ZDrApp *) pParam;
   HINSTANCE hInst = AfxGetInstanceHandle( );

   TraceLineS( "ZDrvrApp::InitInstance Register ZeidonMain", "" );

   WNDCLASSEX wc;

   wc.cbSize         = sizeof( WNDCLASSEX );
   wc.style          = 0;
   wc.lpfnWndProc    = ZeidonCommOE_WndProc;
   wc.cbClsExtra     = 0;
   wc.cbWndExtra     = 0;
   wc.hInstance      = hInst;
   wc.hIcon          = 0;
   wc.hCursor        = 0;
   wc.hbrBackground  = 0;
   wc.lpszMenuName   = 0;
   wc.lpszClassName = "ZDR_Communication_OE";
   wc.hIconSm        = 0;

   if ( RegisterClassEx( &wc ) == 0 )
   {
      DWORD dw = GetLastError( );
      TraceLineS( "Unable to register window class: ", wc.lpszClassName );
      SysMessageBox( pDrApp->m_vSubtask, wc.lpszClassName, "Error in Register", 0 );
      pDrApp->m_bThreadStarted = TRUE;
      return( 0 );
   }

   pDrApp->m_hWndCommOE = CreateWindowEx( WS_EX_TRANSPARENT, "ZDR_Communication_OE", "Zeidon DR Message Thread",
                                          0, 0, 0, 0, 0, 0, 0, (HINSTANCE) hInst, 0 );

   if ( pDrApp->m_hWndCommOE == 0 )
   {
      // DWORD dw = GetLastError( ); // only used for debugging?
      SysMessageBox( pDrApp->m_vSubtask, "ZDR_Communication_OE", "Error creating window", 0 );
      pDrApp->m_bThreadStarted = TRUE;
      return( 0 );
   }

   pDrApp->m_bThreadStarted = TRUE;

   // Acquire and dispatch messages until a WM_QUIT message is received
   while ( GetMessage( &msg,     // message structure
                       pDrApp->m_hWndCommOE, // handle of window receiving the message
                       0,        // lowest message to examine
                       0 ) )     // highest message to examine
   {
      if ( !TranslateAccelerator( pDrApp->m_hWndCommOE, 0, &msg ) )
      {
         TranslateMessage( &msg );  // Translate message
         DispatchMessage( &msg );   // Dispatches message to window
      }
   }

   return( msg.wParam );         // Returns the value from PostQuitMessage
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrApp

BEGIN_MESSAGE_MAP(ZDrApp, CWinApp)
   //{{AFX_MSG_MAP(ZDrApp)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
   // Standard file based document commands
   ON_COMMAND(ID_APP_EXIT, OnAppExit)
   ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrApp ctor
//
ZDrApp::ZDrApp( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrApp::ZDrApp ctor", "" );
#endif

   // TODO: add construction code here,
   // Place all significant initialization in InitInstance
   m_pZSubtaskCurr = 0;
   m_pDoc = 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrApp dtor
//
ZDrApp::~ZDrApp( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrApp::~ZDrApp dtor", "" );
#endif

   // TODO: add destruction code here,

   // Place all significant termination in ExitInstance
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrApp initialization
//
BOOL
ZDrApp::InitInstance( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrApp::InitInstance", "" );
#endif
#if 0
   m_pzsZeidonWnd = new CString( AfxRegisterWndClass( CS_GLOBALCLASS | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, IDC_ARROW ) );
   m_pzsZeidonFrameOrView = new CString( AfxRegisterWndClass( CS_GLOBALCLASS | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
                                                              IDC_ARROW, COLOR_WINDOW, AFX_IDI_STD_FRAME ) );
   m_pzsZeidonMDI_Frame = new CString( AfxRegisterWndClass( CS_GLOBALCLASS | CS_DBLCLKS,
                                                            IDC_ARROW, 0, AFX_IDI_STD_MDIFRAME ) );
#endif

   // Initialize OLE libraries
   if ( AfxOleInit( ) == 0 )
   {
      AfxMessageBox( IDP_OLE_INIT_FAILED );
      return( FALSE );
   }

   HINSTANCE hInst = AfxGetInstanceHandle( );

   ParseCommandLine( m_cmdParse );
   if ( ProcessShellCommand( m_cmdParse ) )
      return( FALSE );

   // Parse command line for standard shell commands, DDE, file open.
// ZCommandLineInfo *pCmdInfo = new ZCommandLineInfo;
// ParseCommandLine( *pCmdInfo );
   CStringArray params, switches;
   GetCommandLineParser( params, switches );
// for ( int k = 0; k < params.GetCount(); k++ )
// {
//     ::MessageBox( 0, "Command line parameter:", params.GetAt( k ), MB_OK | MB_ICONSTOP);
// }

// UINT uThreadID;
   m_hWndCommOE = 0;

   // We need to start up the message loop thread.
   m_bThreadStarted = FALSE;
   CWinThread *pThread = AfxBeginThread( ZeidonCommOE_ThreadProc, (zPVOID) this,
                                         THREAD_PRIORITY_NORMAL, 0, NULL );
   if ( pThread )
      ; // pThread->CloseHandle( (HANDLE) hThread );
   else
      return( FALSE );

   // Now wait until the thread has finished its initialization.
   while ( m_bThreadStarted == FALSE )
      Sleep( 10 );   // usually occurs only once ... so keep it short

   if ( m_hWndCommOE == 0 )
   {
      ::MessageBox( 0, "Unable to create Zeidon communication window",
                    "Zeidon Application", MB_OK | MB_ICONSTOP );
      return( FALSE );
   }

#ifndef zREMOTE_SERVER
   // Kill all driver instances if OE is not up and running.
   if ( CheckAndCleanProcesses( ) > 0 )
   {
      Sleep( 1000 );
   }
#endif

   // We need to hold on to the Subtask!!!
   if ( RegisterZeidonApplication( &m_vSubtask, (zLONG) hInst, (zLONG) m_hWndCommOE,
                                   zZEIDON_COMMUNICATE_OE, params.GetAt( 0 ), 0, 0 ) != 0 )
   {
      zCHAR szMsg[ 256 ];
      strcpy_s( szMsg, zsizeof( szMsg ), "Unable to Register Object Engine for: " );
      strcat_s( szMsg, zsizeof( szMsg ), params.GetAt( 1 ) );
      strcat_s( szMsg, zsizeof( szMsg ), "\nPlease ensure Object Engine is running" );

      // Use SysMessageBox with -1 since we have no view!
      SysMessageBox( 0, "Zeidon Application", szMsg, -1 );
   // MessageSend( 0, 0, "Zeidon Application", szMsg, zMSGQ_MODAL_ERROR, FALSE );
      ::PostMessage( m_hWndCommOE, WM_CLOSE, 0, 0L );
      return( FALSE );
//    throw xmsg( string( szMsg ) ); // throw an exception here
   }

// TraceLineS( "RegisterZeidonApplication successful for: ", pCmdInfo->m_strFileName );

   zCHAR szUseHack[ 100 ];
   SysReadZeidonIni( -1, "[Debug]", "UseDefaultTaskHack", szUseHack, zsizeof( szUseHack ) );
   if ( ztoupper( szUseHack[ 0 ] ) == 'Y' )
      SetDefaultViewForActiveTask( m_vSubtask, 1 );

   zVIEW vZDrSystem;

   if ( GetViewByName( &vZDrSystem, "__ZDrSystem", m_vSubtask, zLEVEL_SYSTEM ) <= 0 )
   {
      // Allocate lots of room for ZFindDialog structs.
      zULONG uLth = zFD_MAX * sizeof( ZFindDialog );
      zPCHAR pchSubtasks = new char[ uLth ];

   // zPLONG pl = (zPLONG) pchSubtasks;
   // *pl = 0;  // initialize to zero subtasks in list.

      // Initialize zero element in subtask list.
      zmemset( pchSubtasks, 0, sizeof( ZFindDialog ) );

      // Activate using SfActivateSysEmptyOI so that the kzzdr.lod
      // does not have to be in the application LPLR.
//    if ( ActivateEmptyObjectInstance( &vZDrSystem, "KZZDR", m_vSubtask,
//                                      zSINGLE | zLEVEL_SYSTEM ) != 0 ||
      if ( SfActivateSysEmptyOI( &vZDrSystem, "KZZDR", m_vSubtask, zSINGLE | zLEVEL_SYSTEM ) != 0 ||
           vZDrSystem == 0 ||
           CreateEntity( vZDrSystem, "System", zPOS_AFTER ) != 0 ||
#ifdef zREMOTE_SERVER
           SetAttributeFromBlob( vZDrSystem, "System", "Subtasks", pchSubtasks, uLth ) != 0 ||
           SetAttributeFromBlob( vZDrSystem, "System", "Sockets",  pchSubtasks, uLth ) != 0 )
#else
           SetAttributeFromBlob( vZDrSystem, "System", "Subtasks", pchSubtasks, uLth ) != 0 )
#endif
      {
         zCHAR szMsg[ 256 ];
         strcpy_s( szMsg, zsizeof( szMsg ), "Unable to Activate KZZDR object" );
         MessageSend( m_vSubtask, 0, "ZeidonApplication", szMsg, zMSGQ_MODAL_ERROR, FALSE );
         ::PostMessage( m_hWndCommOE, WM_CLOSE, 0, 0L );
         mDeleteInitA( pchSubtasks );
         return( FALSE );
      }

      SetNameForView( vZDrSystem, "__ZDrSystem", m_vSubtask, zLEVEL_SYSTEM );
      mDeleteInit( pchSubtasks );
   }

   AfxEnableControlContainer( );

   // Standard initialization
   // If you are not using these features and wish to reduce the size
   // of your final executable, you should remove from the following
   // the specific initialization routines you do not need.

// InitCommonControls( );    // initialize common control library
// CWinApp::InitInstance( ); // call parent class method

#ifdef _AFXDLL
   Enable3dControls( );       // Call this when using MFC in a shared DLL
#else
   Enable3dControlsStatic( ); // Call this when linking to MFC statically
#endif

   // Set the registry key under which our settings are stored.
#ifdef zREMOTE_SERVER
   SetRegistryKey( _T( "Zeidon Remote Executable Server" ) );
#else
   SetRegistryKey( _T( "Zeidon Application" ) );
#endif

   // Load standard INI file options (including MRU)
   LoadStdProfileSettings( 10 );

   CWaitCursor wait;    // put up the wait cursor while we get started

   // Initialize dialog unit globals.
   ZDlgUnits dlg;
   dlg.DoModal( );

   // Initialize DLLs - will add doc templates and make other class
   // implementations available to this application
   ZeidonInitZDRAPP( m_vSubtask, g_lDlgUnitCharX, g_lDlgUnitCharY, g_lMapDlgCharX, g_lMapDlgCharY );

#ifdef zREMOTE_SERVER

   // Register the application's document templates.  Document templates
   // serve as the connection between documents, frame windows and views.

   CSingleDocTemplate* pDocTemplate;
   pDocTemplate = new CSingleDocTemplate( IDR_MAINFRAMEX,
                                          RUNTIME_CLASS( ZDxDoc ),
                                          RUNTIME_CLASS( ZDxFrame ), // main SDI frame window
                                          RUNTIME_CLASS( ZDxView ) );
   AddDocTemplate( pDocTemplate );

   // Dispatch commands specified on the command line
   m_nCmdShow = SW_HIDE;   // prevent the window from being displayed in its
                           // default state until the ShowWindow below.
   pCmdInfo->m_nShellCommand = CCommandLineInfo::FileNew;
   if ( ProcessShellCommand( *pCmdInfo ) == FALSE )
      return( FALSE );

   // The one and only window has been initialized, so show and update it.
   ZDxFrame *pZDxFrame = (ZDxFrame *) AfxGetMainWnd( );

   pZDxFrame->m_pZDrApp = this;
   pZDxFrame->SetWindowText( "Zeidon Remote Services" );
   pZDxFrame->ShowWindow( SW_SHOW );
   pZDxFrame->UpdateWindow( );

   if ( m_pMainWnd == 0 )
      m_pMainWnd = pZDxFrame;

   ::SetWindowLong( m_hWndCommOE, GWL_USERDATA, (zLONG) m_pMainWnd->m_hWnd );

#endif   // #ifdef zREMOTE_SERVER

   // Initialize globals ...
   //
   // Create the ZSubtask for the top window.
   // ZTask is a container class for all ZSubtask's within an application.  Multiple instances of the Zeidon
   // driver may be active at once.  Each instance has exactly one main window (it can change from one window
   // to another depending upon processing flow).  When the main window is destroyed (and has not been replaced),
   // the instance terminates.
   //
   // ZSubtask is a base class containing Zeidon information for each "frame" type of window.  ZDrFrame is
   // derived from the MFC CFrameWnd class.  ZSubtask contains pointers to both the ZDrFrame and ZTask to
   // permit information to be shared among all windows within the task.

   try   // we always have to re-enable the window
   {  // begin of try block

      // Construct a one-to-one class with this class ... to minimize the interface between this .exe and the ZDrApp.dll
      m_pZTask = new ZTask( this, m_vSubtask, &m_cmdParse );

   }  // end of try block

   catch( CException *e )
   {
      zCHAR  szMsg[ 256 ];

      e->GetErrorMessage( szMsg, zsizeof( szMsg ) );
      TraceLineS( "(drvr) ZDrApp::InitInstance Exception ====> ", szMsg );
   // ::MessageBox( 0, szMsg, "Action New Exception", MB_OK );
      return( FALSE );
   }

   if ( m_pZTask->m_uAppState & zAPP_STATE_TERMINATED )
   {
      ::PostMessage( m_hWndCommOE, WM_CLOSE, 0, 0L );
      return( FALSE );
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int
ZDrApp::ExitInstance( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrApp::ExitInstance", "" );
#endif
   mDeleteInit( m_pZTask );

// TraceLineS( "ZDrApp::ExitInstance", " UnregisterZeidon" );

   if ( m_vSubtask )  // if null, Register failed, so do not Unregister!
      UnregisterZeidonApplication( m_vSubtask );

   if ( mIs_hWnd( m_hWndCommOE ) )
      ::PostMessage( m_hWndCommOE, WM_CLOSE, 0, 0L );

   mDeleteInit( m_pZTask );
   mDeleteInit( m_pDoc );

   return( CWinApp::ExitInstance( ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZDrApp::OnAppExit( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrApp::OnAppExit", "" );
// if ( m_pMainWnd != this )
//    TraceLineS( "ZDrApp::OnAppExit", " what is going on here???" );
#endif

   // Same as double-clicking on main window close box.
   if ( m_pMainWnd )
      CWinApp::OnAppExit( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define IDM_IDLE  219

BOOL
ZDrApp::OnIdle( LONG lCount )
{
   BOOL bMore = CWinApp::OnIdle( lCount );
// TraceLineI( "ZDrApp::OnIdle: ", lCount );
   if ( SysSendOE_Message( IDM_IDLE, lCount ) != 0 )
      bMore = TRUE;

   return( bMore ); // return TRUE as long as there are any more idle tasks
}

void
ZDrApp::SetOrientation( zBOOL bLandscape )
{
   PRINTDLG pd;

   pd.lStructSize = (DWORD) sizeof( PRINTDLG );
   if ( GetPrinterDeviceDefaults( &pd ) )
   {
      // protect memory handle with ::GlobalLock and ::GlobalUnlock
      LPDEVMODE pDevMode = (LPDEVMODE) ::GlobalLock( m_hDevMode );

      // set orientation to landscape or portrait
      pDevMode->dmOrientation = bLandscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
      ::GlobalUnlock( m_hDevMode );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZDrApp::SetDialogBkColor( COLORREF clrCtlBk,
                          COLORREF clrCtlText )
{
   CWinApp::SetDialogBkColor( clrCtlBk, clrCtlText );
}

void
ZDrApp::GetCommandLineParser( CStringArray& params, CStringArray& switches )
{
   m_cmdParse.GetParams( params );
   m_cmdParse.GetSwitches( switches );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDlgUnits dialog

ZDlgUnits::ZDlgUnits( CWnd *pParent ) :
           CDialog( ZDlgUnits::IDD, pParent )
{
   //{{AFX_DATA_INIT( ZDlgUnits )
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

void
ZDlgUnits::DoDataExchange( CDataExchange *pDX )
{
   CDialog::DoDataExchange( pDX );
   //{{AFX_DATA_MAP( ZDlgUnits )
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( ZDlgUnits, CDialog )
   //{{AFX_MSG_MAP( ZDlgUnits )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZDlgUnits message handlers

BOOL
ZDlgUnits::OnInitDialog( )
{
   CDialog::OnInitDialog( );

   // TODO: Add extra initialization here
   // Get Dialog Units.
   zLONG  lDlgUnits = ::GetDialogBaseUnits( );
   g_lDlgUnitCharX = LOWORD( lDlgUnits );
   g_lDlgUnitCharY = HIWORD( lDlgUnits );

   CRect rect;

   rect.top = rect.left = 0;
   rect.bottom = rect.right = 1024;
   ::MapDialogRect( m_hWnd, rect );
   g_lMapDlgCharX = rect.right;
   g_lMapDlgCharY = rect.bottom;

   EndDialog( IDCANCEL );
   return( TRUE );  // return TRUE unless you set the focus to a control
                    // EXCEPTION: OCX Property Pages should return FALSE
}

#ifdef zREMOTE_SERVER

zPCHAR g_szText[ 3 ] = { "Connection", "Dialog.Window", "State" };

/////////////////////////////////////////////////////////////////////////////
// ZDxFrame

IMPLEMENT_DYNCREATE(ZDxFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(ZDxFrame, CFrameWnd)
   //{{AFX_MSG_MAP(ZDxFrame)
   ON_WM_CREATE( )
   ON_WM_DESTROY( )
   ON_WM_SIZE( )
   ON_WM_SYSCOMMAND( )
   ON_MESSAGE( zZEIDON_REMOTE_SOCKET, OnRemoteSocket )
   ON_MESSAGE( zZEIDON_REMOTE_GETWND, OnRemoteGetWnd )
   ON_MESSAGE( zZEIDON_REMOTE_PRINT_REPORT, OnRemotePrintReport )
   ON_COMMAND( ID_HELP, CFrameWnd::OnHelp )
   ON_COMMAND( ID_APP_ABOUT, ZDrApp::OnAppAbout )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

static UINT indicators[] =
{
   ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_CAPS,
   ID_INDICATOR_NUM,
   ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// ZDxFrame construction/destruction

ZDxFrame::ZDxFrame( )
{
// TraceLineS( "ZDxFrame::ctor", "" );
   m_pZDrApp = 0;
   m_pZDxView = 0;
}

ZDxFrame::~ZDxFrame( )
{
// TraceLineS( "ZDxFrame::dtor", "" );
}

int
ZDxFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 )
      return( -1 );

   zVIEW vZDrSystem;

   if ( GetViewByName( &vZDrSystem, "__ZDrSystem", 0, zLEVEL_SYSTEM ) > 0 )
   {
      SetAttributeFromInteger( vZDrSystem, "System", "hWndRemote", (zLONG) m_hWnd );
   }

#if defined( _MSC_VER ) && _MSC_VER > 1100
   if ( m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
#else
   if ( m_wndToolBar.Create( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
#endif
                                  CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) == 0 ||
        m_wndToolBar.LoadToolBar( IDR_MAINFRAMEX ) == 0 )
   {
      TraceLineS( "Failed to create toolbar", "" );
      return( -1 );      // fail to create
   }

   if ( m_wndStatusBar.Create( this ) == 0 ||
        m_wndStatusBar.SetIndicators( indicators, zsizeof( indicators ) / zsizeof( UINT ) ) == 0 )
   {
      TraceLineS( "ZDxFrame::OnCreate Failed to create status bar", "" );
      return( -1 );      // fail to create
   }

   // TODO: Delete these three lines if you don't want the toolbar to
   // be dockable
#if defined( _MSC_VER ) && _MSC_VER > 1100
   m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
   EnableDocking( CBRS_ALIGN_ANY );
   DockControlBar( &m_wndToolBar );
#endif

   if ( mIs_hWnd( m_wndListCtrl.m_hWnd ) )
      m_wndListCtrl.SetFocus( );

   WINDOWPLACEMENT wp;
   APPBARDATA abd;
   abd.cbSize = zsizeof( abd );

   // Ignored members: hwnd, uCallbackMessage, uEdge, rc, lParam.
   // abd.rc contains the rectangular location of the taskbar in
   // screen coordinates.
   SHAppBarMessage( ABM_GETTASKBARPOS, &abd );
   UINT uEdge = GetTaskbarEdge( abd.rc );

   if ( ReadWindowPlacement( vZDrSystem, "ZDRemote", "RemoteServer", "Main", &wp ) &&
        CheckWindowPos( &wp ) )
   {
      if ( uEdge == ABE_TOP )
         wp.rcNormalPosition.top += (abd.rc.bottom - abd.rc.top);
      else
      if ( uEdge == ABE_LEFT )
         wp.rcNormalPosition.left += (abd.rc.right - abd.rc.left);

      if ( uEdge == ABE_TOP )
         wp.rcNormalPosition.bottom += (abd.rc.bottom - abd.rc.top);
      else
      if ( uEdge == ABE_LEFT )
         wp.rcNormalPosition.right += (abd.rc.right - abd.rc.left);

      SetWindowPlacement( &wp );
      SetActiveWindow( );
   }

   return( 0 );
}

BOOL
ZDxFrame::OnCreateClient( LPCREATESTRUCT lpcs,
                          CCreateContext *pContext )
{
   return( CFrameWnd::OnCreateClient( lpcs, pContext ) );
#if 0
   return( m_wndSplitter.Create( this, 2, 2,  // TODO: adjust the number of rows, columns
                                 CSize( 10, 10 ), // TODO: adjust the minimum pane size
                                 pContext ) );
#endif
}

BOOL
ZDxFrame::PreCreateWindow( CREATESTRUCT& cs )
{
// TraceLineS( "ZDxFrame::PreCreateWindow", "" );
   if ( CFrameWnd::PreCreateWindow( cs ) == FALSE )
      return( FALSE );

   // TODO: Modify the Window class or styles here by modifying
   // the CREATESTRUCT cs.

   return( TRUE );
}

void
ZDxFrame::OnDestroy( )
{
   ZDrApp *pZDrApp = (ZDrApp *) AfxGetApp( );

#ifdef DEBUG_ALL
   TraceLineS( "ZDxFrame::OnDestroy", "" );
   if ( pZDrApp && pZDrApp->m_pMainWnd != this )
      TraceLineS( "ZDxFrame::OnDestroy", " what is going on here???" );
#endif

   if ( pZDrApp->m_pZTask->GetSocketTaskCount( ) > 0 )
   {
      if ( ::MessageBox( m_hWnd, "Sockets are still active ... Exiting!",
                         "Zeidon Remote Server", MB_OKCANCEL ) != IDOK )
      {
      // return;
      }
   }

   WINDOWPLACEMENT wp;
   wp.length = zsizeof( wp );
   if ( GetWindowPlacement( &wp ) )
   {
      wp.flags = 0;
      if ( IsZoomed( ) )
         wp.flags |= WPF_RESTORETOMAXIMIZED;

      APPBARDATA abd;
      abd.cbSize = zsizeof( abd );

      // Ignored members: hwnd, uCallbackMessage, uEdge, rc, lParam
      SHAppBarMessage( ABM_GETTASKBARPOS, &abd );
      UINT uEdge = GetTaskbarEdge( abd.rc );

      // abd.rc contains the rectangular location of the taskbar in
      // screen coordinates.
      if ( uEdge == ABE_TOP )
      {
         wp.rcNormalPosition.top -= (abd.rc.bottom - abd.rc.top);
         wp.rcNormalPosition.bottom -= (abd.rc.bottom - abd.rc.top);
      }
      else
      if ( uEdge == ABE_LEFT )
      {
         wp.rcNormalPosition.left -= (abd.rc.right - abd.rc.left);
         wp.rcNormalPosition.right -= (abd.rc.right - abd.rc.left);
      }

      // Write the window placement to the .INI file.
      WriteWindowPlacement( 0, "ZDRemote", "RemoteServer", "Main", &wp );

      if ( mIs_hWnd( m_wndListCtrl.m_hWnd ) )
      {
         zPCHAR pch = new char[ zsizeof( "-2147483647," ) * 3 ];
         zSHORT nLth = 0;
         zSHORT k;

         for ( k = 0; k < 3; k++ )
         {
            _ltoa_s( m_wndListCtrl.GetColumnWidth( k ), pch + nLth );
            nLth = zstrlen( pch );
            if ( k < 3 - 1 )
            {
               pch[ nLth ] = ',';
               nLth++;
            }
         }

         SetProfileStr( 0, "ZDRemote", "RemoteServer", "TaskList", "ColumnWidth", pch );
         mDeleteInitA( pch );
      }
   }

   CFrameWnd::OnDestroy( );

   if ( pZDrApp )
   {
      ::SetWindowLong( pZDrApp->m_hWndCommOE, GWL_USERDATA, 0L );
      ::PostMessage( pZDrApp->m_hWndCommOE, WM_CLOSE, 0, 0L );
   }

   AfxPostQuitMessage( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// ZDxFrame diagnostics

#ifdef _DEBUG
void
ZDxFrame::AssertValid( ) const

{
   CFrameWnd::AssertValid( );
}

void
ZDxFrame::Dump( CDumpContext& dc ) const
{
   CFrameWnd::Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZDxFrame message handlers

void
ZDxFrame::OnSysCommand( UINT uCmdType, LPARAM lParam )
{
// TraceLineX( "ZDxFrame::OnSysCommand = ", uCmdType );

   // SC_CLOSE is 0xF060, but when a popup menu is up, the uCmdType we get
   // is 0xF063 for some reason.  Since all SC_ messages end in 0, we
   // will try the following to clear the last 4 bits.
   switch ( uCmdType & 0xFFF0 )
   {
      case SC_CLOSE:
      {
         ZDrApp *pZDrApp = (ZDrApp *) AfxGetApp( );

         if ( pZDrApp->m_pZTask->GetSocketTaskCount( ) > 0 )
         {
            if ( ::MessageBox( m_hWnd, "Sockets are still active, do you want to exit?",
                               "Zeidon Remote Server", MB_OKCANCEL ) != IDOK )
            {
               return;
            }
         }
      }
   }

   Default( ); // CWnd::OnSysCommand just calls Default
}

void
ZDxFrame::OnSize( UINT nType, int cx, int cy )
{
   CFrameWnd::OnSize( nType, cx, cy );

   // TODO: Add your message handler code here
   if ( mIs_hWnd( m_wndListCtrl.m_hWnd ) ) // &&
     // mIs_hWnd( m_wndToolBar.m_hWnd ) &&
     // mIs_hWnd( m_wndStatusBar.m_hWnd ) )
   {
      CRect rect;

      GetClientRect( rect );
      m_wndListCtrl.MoveWindow( rect, TRUE );
   }

   if ( nType == SIZE_RESTORED ||
        nType == SIZE_MAXIMIZED )
   {
      m_wndListCtrl.SetFocus( );
   }
}

/////////////////////////////////////////////////////////////////////////////
//    0 - initialization
//    1 - registration
//    2 - send XRA
//    4 - wait for XRA
//    8 - received XRA
//   16 - idle
//   -1 - termination
/////////////////////////////////////////////////////////////////////////////
LRESULT
ZDxFrame::OnRemoteSocket( WPARAM wParam, LPARAM lParam )
{
   if ( mIs_hWnd( m_wndListCtrl.m_hWnd ) )
   {
      // Protect this with a semaphore!!!
      SysMutexLock( vSubtask, "ZDs", 0, 0 );

      ZFindSocket *pSocketList = (ZFindSocket *) lParam;
      ZSocketTask *pZSocketTask;
      zLONG   lPos = wParam;
      zLONG   lCnt = (zLONG) pSocketList[ 0 ].pZSocketTask;
      zLONG   lItemCnt = m_wndListCtrl.GetItemCount( );
   // LV_ITEM lvi;
      zLONG   lItem = -1;
      CString csText;

      m_wndListCtrl.DeleteAllItems( );
      for ( lPos = 1; lPos <= lCnt; lPos++ )
      {
         if ( pSocketList[ lPos ].pZSocketTask &&
              pSocketList[ lPos ].ulSocketState != -1 )
         {
            lItem++;
            pZSocketTask = pSocketList[ lPos ].pZSocketTask;
            csText.Format( TEXT( "[S%ld.T%ld.t%ld.s%ld]" ), pZSocketTask, AfxGetThread( ),
                           pZSocketTask->m_pWinThread, pZSocketTask->m_sock );

            m_wndListCtrl.InsertItem( LVIF_TEXT | LVIF_STATE | LVIF_PARAM,
                                      lItem, csText, LVIS_SELECTED, 0, 0, (zLONG) lPos );
            m_wndListCtrl.SetItemText( lItem, 1, pZSocketTask->m_szRemoteWndTag );
            switch ( pSocketList[ lPos ].ulSocketState )
            {
               case 0:
                  m_wndListCtrl.SetItemText( lItem, 2, "Initialization" );
                  break;

               case 1:
                  m_wndListCtrl.SetItemText( lItem, 2, "Registration" );
                  break;

               case 2:
                  m_wndListCtrl.SetItemText( lItem, 2, "Send XRA" );
                  break;

               case 4:
                  m_wndListCtrl.SetItemText( lItem, 2, "Wait for XRA" );
                  break;

               case 8:
                  m_wndListCtrl.SetItemText( lItem, 2, "Received XRA" );
                  break;

               case 16:
                  m_wndListCtrl.SetItemText( lItem, 2, "Idle" );
                  break;

               default:
                  m_wndListCtrl.SetItemText( lItem, 2, "Unknown" );
                  break;
            }
         }
      }

      m_wndListCtrl.UpdateWindow( );
      SysMutexUnlock( vSubtask, "ZDs", 0 );
   }

   return( 0 );
}

LRESULT
ZDxFrame::OnRemoteGetWnd( WPARAM wParam, LPARAM lParam )
{
   if ( m_pZDxView )
   {
      m_pZDxView->m_vDialog = (zVIEW) lParam;
      return( (LPARAM) m_pZDxView->m_hWnd );
   }
   else
      return( 0 );
}

LRESULT
ZDxFrame::OnRemotePrintReport( WPARAM wParam, LPARAM lParam )
{
   zVIEW  vArgs = (zVIEW) wParam;
   zPVIEW pvViewList = (zPVIEW) SfLockTaskMemory( (zPVOID) lParam );
   zVIEW  vSubtask;
   zPCHAR pchSubobjectEntityName;
   zPCHAR pchAppName;
   zPCHAR pchReportDefName;
   zPCHAR pchReportName;
   zPCHAR pchViewNameList;
   zLONG  lFlags;
   zLONG  lRC;

   vArgs = (zVIEW) SysGetPointerFromHandle( vArgs );

   // Get value for AppName
   SetCursorFirstEntity( vArgs, "Argument", 0 );
   GetAddrForAttribute( &pchAppName, vArgs, "Argument", "Value" );

   SfCreateSubtask( &vSubtask, 0, pchAppName );

   // Get value for SubobjectEntityName
   SetCursorNextEntity( vArgs, "Argument", 0 );
   GetAddrForAttribute( &pchSubobjectEntityName, vArgs, "Argument", "Value" );

   // Get value for ReportDefName
   SetCursorNextEntity( vArgs, "Argument", 0 );
   GetAddrForAttribute( &pchReportDefName, vArgs, "Argument", "Value" );

   // Get value for ReportName
   SetCursorNextEntity( vArgs, "Argument", 0 );
   GetAddrForAttribute( &pchReportName, vArgs, "Argument", "Value" );

   // Get value for report Flags
   SetCursorNextEntity( vArgs, "Argument", 0 );
   GetIntegerFromAttribute( &lFlags, vArgs, "Argument", "Value" );

   // Get value for ViewNameList
   SetCursorNextEntity( vArgs, "Argument", 0 );
   GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );

   // Now get the name for each of the views and name them.
   zSHORT nCnt = 0;
   zPCHAR pch = (zPCHAR) pchViewNameList;
   zPCHAR pchSemi;

   while ( pch && *pch )
   {
      // Get value for data view (ignore the value).
      SetCursorNextEntity( vArgs, "Argument", 0 );

      pchSemi = zstrchr( pch, ';' );
      if ( pchSemi )
         *pchSemi = 0;

      pvViewList[ nCnt ] =
                 (zVIEW) SysGetPointerFromHandle( pvViewList[ nCnt ] );
      SetNameForView( pvViewList[ nCnt ], pch, vSubtask, zLEVEL_TASK );
      nCnt++;
      if ( pchSemi )
      {
         *pchSemi = ';';
         pchSemi++;
      }

      pch = pchSemi;
   }

   lRC = PrintReportToPDF( vSubtask, pvViewList[ 0 ], pchSubobjectEntityName,
                           pchReportDefName, pchReportName, lFlags );

   SfUnlockTaskMemory( (zPVOID) pvViewList );

   SetAttributeFromInteger( vArgs, "Operation", "ReturnValue", lRC );
   SfDropSubtask( vSubtask, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDxView

IMPLEMENT_DYNCREATE( ZDxView, CView )

BEGIN_MESSAGE_MAP( ZDxView, CView )
   //{{AFX_MSG_MAP( ZDxView )
   ON_WM_CREATE( )
   //}}AFX_MSG_MAP

   // Standard printing commands
   ON_COMMAND( ID_FILE_PRINT, CView::OnFilePrint )
   ON_COMMAND( ID_FILE_PRINT_DIRECT, CView::OnFilePrint )
   ON_COMMAND( ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZDxView construction/destruction

ZDxView::ZDxView( )
{
   m_pZDxFrame = 0;
   m_pZSubtask = 0;
   m_vDialog = 0;
}

ZDxView::~ZDxView( )
{
}

BOOL
ZDxView::PreCreateWindow( CREATESTRUCT& cs )
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs.

   return( CView::PreCreateWindow( cs ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZDxView drawing

void
ZDxView::OnDraw( CDC *pDC )
{
   ZDxDoc* pDoc = GetDocument( );
   ASSERT_VALID( pDoc );
   // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDxView diagnostics

#ifdef _DEBUG
void
ZDxView::AssertValid( ) const
{
   CView::AssertValid( );
}

void
ZDxView::Dump( CDumpContext& dc ) const
{
   CView::Dump( dc );
}

ZDxDoc *
ZDxView::GetDocument( ) // non-debug version is inline
{
   ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( ZDxDoc ) ) );
   return( (ZDxDoc *) m_pDocument );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZDxView message handlers

int
ZDxView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CView::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   // TODO: Add your specialized creation code here

   CRect rect;
   ZDxFrame *m_pZDxFrame = (ZDxFrame *) GetParentFrame( );
   m_pZDxFrame->m_pZDxView = this;

   m_pZDxFrame->GetClientRect( rect );
   if ( m_pZDxFrame->m_wndListCtrl.Create( WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS,
                                           rect, this, 14 ) == 0 )
   {
      TraceLineS( "Failed to create ListCtrl", "" );
      return( -1 );      // fail to create
   }

   // Loop processing each column.
   LV_COLUMN lvc;
   zPCHAR    pchBuffer = new char[ zsizeof( "-2147483647," ) * 3 ];
   zPCHAR    pchComma;
   zPCHAR    pch;
   zSHORT    nExtent;
   zSHORT    nCol;

   GetProfileStr( 0, "ZDRemote", "RemoteServer", "TaskList",
                  "ColumnWidth", pchBuffer, zsizeof( "-2147483647," ) * 3 );
   pch = pchBuffer;

   for ( nCol = 0; nCol < 3; nCol++ )
   {
      lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
      lvc.fmt = LVCFMT_LEFT;    // LVCFMT_RIGHT
      lvc.pszText = g_szText[ nCol ];
      lvc.iSubItem = nCol;

      pchComma = zstrchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      switch ( nCol )
      {
         case 1:
            lvc.cx = 400;
            break;

         case 2:
            lvc.cx = 256;
            break;

         default:
            lvc.cx = 128;
      }

      nExtent = (zSHORT) zatol( pch );
      if ( nExtent > 0 )
         lvc.cx = nExtent;

      if ( pchComma )
         pch = pchComma + 1;

      if ( m_pZDxFrame->m_wndListCtrl.InsertColumn( nCol, &lvc ) != nCol )
      {
         TraceLineI( "CListCtrl InsertColumn failed: ", nCol );
      }
   }

   mDeleteInitA( pchBuffer );
   m_pZDxFrame->m_wndListCtrl.ShowWindow( SW_SHOW );
   m_pZDxFrame->m_wndListCtrl.EnableWindow( TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDxDoc

IMPLEMENT_DYNCREATE(ZDxDoc, CDocument)

BEGIN_MESSAGE_MAP(ZDxDoc, CDocument)
   //{{AFX_MSG_MAP(ZDxDoc)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZDxDoc construction/destruction

ZDxDoc::ZDxDoc( )
{
   // TODO: add one-time construction code here
}

ZDxDoc::~ZDxDoc( )
{
}

BOOL
ZDxDoc::OnNewDocument( )
{
   if ( CDocument::OnNewDocument( ) == FALSE )
      return( FALSE );

   // TODO: add reinitialization code here
   // (SDI documents will reuse this document)

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZDxDoc serialization

void
ZDxDoc::Serialize(CArchive& ar)
{
   if ( ar.IsStoring( ) )
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZDxDoc diagnostics

#ifdef _DEBUG
void
ZDxDoc::AssertValid( ) const
{
   CDocument::AssertValid( );
}

void
ZDxDoc::Dump( CDumpContext& dc ) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZDxDoc commands

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
   CAboutDlg( );

// Dialog Data
   //{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOXX };
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CAboutDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   //{{AFX_MSG(CAboutDlg)
      // No message handlers
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

CAboutDlg::CAboutDlg( ) :
           CDialog( CAboutDlg::IDD )
{
   //{{AFX_DATA_INIT(CAboutDlg)
   //}}AFX_DATA_INIT
}

void
CAboutDlg::DoDataExchange( CDataExchange *pDX )
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CAboutDlg)
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   //{{AFX_MSG_MAP(CAboutDlg)
      // No message handlers
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

// App command to run the dialog
void
ZDrApp::OnAppAbout( )
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal( );
}

#else    // zREMOTE_SERVER

#include <winperf.h>   // for Windows NT
#include <tlhelp32.h>  // for Windows 95

//
// Manifest constants.
//
#define INITIAL_SIZE        51200
#define EXTEND_SIZE         25600
#define REGKEY_PERF         "software\\microsoft\\windows nt\\currentversion\\perflib"
#define REGSUBKEY_COUNTERS  "Counters"
#define PROCESS_COUNTER     "process"
#define PROCESSID_COUNTER   "id process"
#define UNKNOWN_TASK        "unknown"


// Function pointer types for accessing Toolhelp32 functions dynamically.
// By dynamically accessing these functions, we can do so only on Windows
// 95 and still run on Windows NT, which does not have these functions.

typedef BOOL (WINAPI *PROCESSWALK) ( HANDLE hSnapshot, LPPROCESSENTRY32 lppe );
typedef HANDLE (WINAPI *CREATESNAPSHOT) ( DWORD dwFlags, DWORD th32ProcessID );
typedef BOOL (CALLBACK *ENUMWINDOWSPROC) ( HWND hwnd, LONG lParam );

//./ ADD NAME=ZDrApp::CheckAndCleanProcesses
// Source Module=zdr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       ZDrApp::CheckAndCleanProcesses
//                                            1/23/2002 Modified
//  PURPOSE:    check if there are instances of the driver (zdr)
//              but no OE is running. If so, kill all drivers
//
//  PARAMETERS:
//
//  RETURN    :
/////////////////////////////////////////////////////////////////////////////
//./END + 2
zLONG
ZDrApp::CheckAndCleanProcesses( )
{
   BOOL        ForceKill = TRUE;
   CHAR        oename[ ] = { "KZOENGWA" };
   CHAR        drvname[ ] = { "ZDR" };
   TASK_LIST   tlist[ MAX_TASKS ];

   DWORD             dw;
   DWORD             numTasks;
   TASK_LIST_ENUM    te;
   int               rval = 0;
   zCHAR             tname[ PROCESS_SIZE ];
   LPSTR             p;
   DWORD             ThisPid;
   OSVERSIONINFO     verInfo = { 0 };
   LPGetTaskList     GetTaskList;
   LPEnableDebugPriv EnableDebugPriv;
   zBOOL bOERunning = FALSE;
   zLONG lKilled = 0;

   // Determine what system we're on and do the right thing.
   verInfo.dwOSVersionInfoSize = sizeof( verInfo );
   GetVersionEx( &verInfo );

   switch ( verInfo.dwPlatformId )
   {
      case VER_PLATFORM_WIN32_NT:
         GetTaskList     = GetTaskListNT;
         EnableDebugPriv = EnableDebugPrivNT;
         break;

      case VER_PLATFORM_WIN32_WINDOWS:
         GetTaskList = GetTaskList95;
         EnableDebugPriv = EnableDebugPriv95;
         break;

      default:
         return( 0 );
   }

   // Obtain the ability to manipulate other processes.
   EnableDebugPriv( );

   // Get the task list for the system.
   numTasks = GetTaskList( tlist, MAX_TASKS );

   // Enumerate all windows and try to get the window titles for each task.
   te.tlist = tlist;
   te.numtasks = numTasks;
   GetWindowTitles( &te );

   ThisPid = GetCurrentProcessId( );

   // Check if oe is running.
   for ( dw = 0; dw < numTasks && !bOERunning; dw++ )
   {
      // This prevents the user from killing this instance.
      if ( ThisPid == tlist[ dw ].dwProcessId )
      {
         continue;
      }

      tname[ 0 ] = 0;
      strcpy_s( tname, PROCESS_SIZE, (LPCSTR) tlist[ dw ].ProcessName );
      p = zstrchr( tname, '.' );
      if ( p )
      {
         p[ 0 ] = 0;
      }

      if ( MatchPattern( (UCHAR *) tname, (UCHAR *) oename ))
      {
         bOERunning = TRUE;
      }
      else
      if ( MatchPattern( tlist[ dw ].ProcessName, (UCHAR *) oename ) )
      {
         bOERunning = TRUE;
      }
      else
      if ( MatchPattern( tlist[ dw ].WindowTitle, (UCHAR *) oename ) )
      {
         bOERunning = TRUE;
      }
   }

   // Mark all Drivers except current instance as kill candidates.
   for ( dw = 0; dw < numTasks && !bOERunning; dw++ )
   {
      // This prevents the user from killing this instance.
      if ( ThisPid == tlist[ dw ].dwProcessId )
      {
         continue;
      }

      tname[ 0 ] = 0;
      strcpy_s( tname, PROCESS_SIZE, (LPCSTR) tlist[ dw ].ProcessName );
      p = zstrchr( tname, '.' );
      if ( p )
      {
         p[ 0 ] = 0;
      }

      if ( MatchPattern( (UCHAR *) tname, (UCHAR *) drvname ) )
      {
         tlist[ dw ].flags = TRUE;
      }
      else
      if ( MatchPattern( tlist[ dw ].ProcessName, (UCHAR *) drvname ) )
      {
         tlist[ dw ].flags = TRUE;
      }
      else
      if ( MatchPattern( tlist[ dw ].WindowTitle, (UCHAR *) drvname ) )
      {
         tlist[ dw ].flags = TRUE;
      }
   }

   for ( dw = 0; dw < numTasks && !bOERunning; dw++ )
   {
      if ( tlist[ dw ].flags )
      {
         KillProcess( &tlist[ dw ], ForceKill );
         lKilled++;
      }
   }

   return( lKilled );
}

//./ ADD NAME=GetTaskList95
// Source Module=zdr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       GetTaskList95
//                                            1/25/2002 Modified
//  PURPOSE:
//    Provides an API for getting a list of tasks running at the time of the
//    API call.  This function uses Toolhelp32to get the task list and is
//    therefore straight WIN32 calls that anyone can call.
//
//  PARAMETERS:  PTASK_LIST  pTask - array of tasks
//               DWORD       dwNumTasks - maximum number of tasks that the pTask
//                           array can hold
//
//  RETURN    :  number of tasks
/////////////////////////////////////////////////////////////////////////////
//./END + 3
DWORD
GetTaskList95( PTASK_LIST pTask,
               DWORD      dwNumTasks )
{
   CREATESNAPSHOT pCreateToolhelp32Snapshot = NULL;
   PROCESSWALK    pProcess32First           = NULL;
   PROCESSWALK    pProcess32Next            = NULL;

   HANDLE         hKernel        = NULL;
   HANDLE         hProcessSnap   = NULL;
   PROCESSENTRY32 pe32           = {0};
   DWORD          dwTaskCount    = 0;


   // Guarantee to the code later on that we'll enum at least one task.
   if ( dwNumTasks == 0 )
      return( 0 );

   // Obtain a module handle to KERNEL so that we can get the addresses of
   // the 32-bit Toolhelp functions we need.
   hKernel = GetModuleHandle( "KERNEL32.DLL" );

   if ( hKernel )
   {
       pCreateToolhelp32Snapshot =
         (CREATESNAPSHOT) GetProcAddress( (HMODULE) hKernel, "CreateToolhelp32Snapshot" );

       pProcess32First = (PROCESSWALK) GetProcAddress( (HMODULE) hKernel, "Process32First" );
       pProcess32Next  = (PROCESSWALK) GetProcAddress( (HMODULE) hKernel, "Process32Next" );
   }

   // make sure we got addresses of all needed Toolhelp functions.
   if ( !( pProcess32First && pProcess32Next && pCreateToolhelp32Snapshot ) )
      return( 0 );


   // Take a snapshot of all processes currently in the system.
   hProcessSnap = pCreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
   if ( hProcessSnap == (HANDLE) -1 )
       return( 0 );

   // Walk the snapshot of processes and for each process, get information
   // to display.
   dwTaskCount = 0;
   pe32.dwSize = sizeof( PROCESSENTRY32 );  // must be filled out before use
   if ( pProcess32First( hProcessSnap, &pe32 ) )
   {
      do
      {
         LPSTR pCurChar;

         // Strip path and leave executable filename splitpath.
         for ( pCurChar = (pe32.szExeFile + zstrlen( pe32.szExeFile ));
               *pCurChar != '\\' && pCurChar != pe32.szExeFile;
               pCurChar-- )
         {
            strcpy_s( (LPSTR) pTask->ProcessName, PROCESS_SIZE, pCurChar );
         }

         pTask->flags = 0;
         pTask->dwProcessId = pe32.th32ProcessID;

         dwTaskCount++;   // keep track of how many tasks we've got so far
         pTask++;         // get to next task info block.
      }
      while ( dwTaskCount < dwNumTasks && pProcess32Next( hProcessSnap, &pe32 ) );
   }
   else
   {
      dwTaskCount = 0;    // Couldn't walk the list of processes.
   }

   // Don't forget to clean up the snapshot object...
   CloseHandle( hProcessSnap );

   return( dwTaskCount );
}

void
ZeidonFormatMessage( zCPCHAR cpcMsgTitle, zBOOL bMessageBox )
{
   zCHAR  szMsgBuffer[ 4096 ];
   zPCHAR pchMsg;
   DWORD  dw = GetLastError( );

   ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                    0, dw, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (zPCHAR) &pchMsg, 0, 0 );
   sprintf_s( szMsgBuffer, "%s: (%d) %s", cpcMsgTitle, dw, pchMsg );
   TraceLineS( szMsgBuffer, "" );
   TRACE0( szMsgBuffer );
   if ( bMessageBox )
   {
      ::MessageBox( 0, (zCPCHAR) pchMsg, cpcMsgTitle, MB_OK | MB_ICONINFORMATION );
   }

   // Free the buffer.
   LocalFree( pchMsg );
}

//./ ADD NAME=GetTaskListNT
// Source Module=zdr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       GetTaskListNT
//                                            1/25/2002 Modified
//  PURPOSE:
//    Provides an API for getting a list of tasks running at the time of the
//    API call.  This function uses Toolhelp32to get the task list and is
//    therefore straight WIN32 calls that anyone can call.
//
//  PARAMETERS:  PTASK_LIST  pTask - array of tasks
//               DWORD       dwNumTasks - maximum number of tasks that the pTask
//                           array can hold
//
//  RETURN    :  number of tasks
/////////////////////////////////////////////////////////////////////////////
//./END + 3
DWORD
GetTaskListNT( PTASK_LIST  pTask,
               DWORD       dwNumTasks )
{
   DWORD                       rc;
   HKEY                        hKeyNames;
   DWORD                       dwType;
   DWORD                       dwSize;
   LPSTR                       buf = NULL;
   CHAR                        szSubKey[ 1024 ];
   LANGID                      lid;
   LPSTR                       p;
   LPSTR                       p2;
   PPERF_DATA_BLOCK            pPerf;
   PPERF_OBJECT_TYPE           pObj;
   PPERF_INSTANCE_DEFINITION   pInst;
   PPERF_COUNTER_BLOCK         pCounter;
   PPERF_COUNTER_DEFINITION    pCounterDef;
   DWORD                       dw;
   DWORD                       dwProcessIdTitle;
   DWORD                       dwProcessIdCounter;
   CHAR                        szProcessName[ MAX_PATH ];
   DWORD                       dwLimit = dwNumTasks - 1;

   // Need to initialize this because of goto logic.  Note that this code can fail if privileges are
   // not set correctly.  It can kill processes that are not owned if privileges are set to "admin",
   // since "all" process are returned.  If privileges are
   dwNumTasks = 0;

   // Look for the list of counters.  Always use the neutral English version, regardless of the local
   // language.  We are looking for some particular keys, and we are always going to do our looking in
   // English.  We are not going to show the user the counter names, so there is no need to go find the
   // corresponding name in the local language.
   lid = MAKELANGID( LANG_ENGLISH, SUBLANG_NEUTRAL );
   sprintf_s( szSubKey, "%s\\%03x", REGKEY_PERF, lid );
   rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hKeyNames );
   if ( rc != ERROR_SUCCESS )
   {
      ZeidonFormatMessage( "Open Local Machine", FALSE );
      goto exit;
   }

   // Get the buffer size for the counter names.
   rc = RegQueryValueEx( hKeyNames, REGSUBKEY_COUNTERS, NULL, &dwType, NULL, &dwSize );

   if ( rc != ERROR_SUCCESS )
   {
      ZeidonFormatMessage( "Count Subkeys", FALSE );
      goto exit;
   }

   // Allocate the counter names buffer.
   buf = (LPSTR) malloc( dwSize + 1 );
   if ( buf == NULL )
   {
      goto exit;
   }

   zmemset( buf, 0, dwSize + 1 );

   // Read the counter names from the registry.
   rc = RegQueryValueEx( hKeyNames, REGSUBKEY_COUNTERS, NULL, &dwType, (LPBYTE) buf, &dwSize );

   if ( rc != ERROR_SUCCESS )
   {
      ZeidonFormatMessage( "Load Subkeys", FALSE );
      goto exit;
   }

   // Now loop thru the counter names looking for the following counters:
   //
   //      1.  "Process"           process name
   //      2.  "ID Process"        process id
   //
   // The buffer contains multiple null terminated strings and then finally null terminated at the end.
   // The strings are in pairs of counter number and counter name.
   //
   p = buf;
   while ( *p )
   {
      if ( p > buf )
      {
         for ( p2 = p - 2; p2 >= buf && isdigit( *p2 ); p2-- )
         {
         }
      }

      if ( zstricmp( p, PROCESS_COUNTER ) == 0 )
      {
         // Look backwards for the counter number.
         for ( p2 = p - 2; p2 >= buf && isdigit( *p2 ); p2-- )
         {
         }

         strcpy_s( szSubKey, zsizeof( szSubKey ), p2 + 1 );
      }
      else
      if ( zstricmp( p, PROCESSID_COUNTER ) == 0 )
      {
         // Look backwards for the counter number.
         for ( p2 = p - 2; p2 >= buf && isdigit( *p2 ); p2-- )
         {
         }

         dwProcessIdTitle = atol( p2 + 1 );
      }

      // Next string.
      p += (zstrlen( p ) + 1);
   }

   // Free the counter names buffer.
   free( buf );

   // Allocate the initial buffer for the performance data.
   dwSize = INITIAL_SIZE;
   buf = (LPSTR) malloc( dwSize );
   if ( buf == NULL )
   {
      goto exit;
   }

   zmemset( buf, 0, dwSize );

   while ( TRUE )
   {
      rc = RegQueryValueEx( HKEY_PERFORMANCE_DATA, szSubKey, NULL, &dwType, (LPBYTE) buf, &dwSize );

      pPerf = (PPERF_DATA_BLOCK) buf;

      // Check for success and valid perf data block signature.
      if ( (rc == ERROR_SUCCESS) && (dwSize > 0) &&
           (pPerf)->Signature[ 0 ] == (WCHAR) 'P' &&
           (pPerf)->Signature[ 1 ] == (WCHAR) 'E' &&
           (pPerf)->Signature[ 2 ] == (WCHAR) 'R' &&
           (pPerf)->Signature[ 3 ] == (WCHAR) 'F' )
      {
          break;
      }

      // If buffer is not big enough, reallocate and try again.
      if ( rc == ERROR_MORE_DATA )
      {
         dwSize += EXTEND_SIZE;
         buf = (LPSTR) realloc( buf, dwSize );
         zmemset( buf, 0, dwSize );
      }
      else
      {
         goto exit;
      }
   }

   // Set the perf_object_type pointer.
   pObj = (PPERF_OBJECT_TYPE) ((DWORD) pPerf + pPerf->HeaderLength);

   // Loop thru the performance counter definition records looking
   // for the process id counter and then save its offset.
   pCounterDef = (PPERF_COUNTER_DEFINITION) ((DWORD) pObj + pObj->HeaderLength );
   for ( dw = 0; dw < (DWORD) pObj->NumCounters; dw++ )
   {
      if ( pCounterDef->CounterNameTitleIndex == dwProcessIdTitle )
      {
         dwProcessIdCounter = pCounterDef->CounterOffset;
         break;
      }

      pCounterDef++;
   }

   dwNumTasks = min( dwLimit, (DWORD) pObj->NumInstances );
   pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD) pObj + pObj->DefinitionLength);

   // Loop thru the performance instance data extracting each process name
   // and process id.
   for ( dw = 0; dw < dwNumTasks; dw++ )
   {
      // Pointer to the process name.
      p = (LPSTR)((DWORD) pInst + pInst->NameOffset);

      // Convert it to ascii.
      rc = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR) p, -1, szProcessName,
                                zsizeof( szProcessName ), NULL, NULL );
      if ( rc == 0 )
      {
         // If we can't convert the string, then use a default value.
         strcpy_s( (zPCHAR) pTask->ProcessName, PROCESS_SIZE, UNKNOWN_TASK );
      }

      if ( zstrlen( szProcessName ) + 4 <= zsizeof( pTask->ProcessName ) )
      {
         strcpy_s( (zPCHAR) pTask->ProcessName, PROCESS_SIZE, szProcessName );
         strcat_s( (zPCHAR) pTask->ProcessName, PROCESS_SIZE, ".exe" );
      }

      // Get the process id.
      pCounter = (PPERF_COUNTER_BLOCK) ((DWORD) pInst + pInst->ByteLength);
      pTask->flags = 0;
      pTask->dwProcessId = *((LPDWORD) ((DWORD) pCounter + dwProcessIdCounter));
      if ( pTask->dwProcessId == 0 )
      {
         pTask->dwProcessId = (DWORD) -2;
      }

      // Next process.
      pTask++;
      pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD) pCounter + pCounter->ByteLength );
   }

exit:
   if ( buf )
   {
      free( buf );
   }

   RegCloseKey( hKeyNames );
   RegCloseKey( HKEY_PERFORMANCE_DATA );

   return( dwNumTasks );
}

//./ ADD NAME=EnableDebugPriv95
// Source Module=zdr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       EnableDebugPriv95
//                                            1/25/2002 Modified
//  PURPOSE:    Changes the process's privilege so that kill works properly.
//
//  PARAMETERS:
//
//  RETURN    : TRUE             - success
//              FALSE            - failure
//  Comments:   Always returns TRUE
/////////////////////////////////////////////////////////////////////////////
//./END + 2
zBOOL
EnableDebugPriv95( )

{
   return( TRUE );
}

//./ ADD NAME=EnableDebugPrivNT
// Source Module=zdr.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       EnableDebugPrivNT
//                                            1/25/2002 Modified
//  PURPOSE:    Changes the process's privilege so that kill works properly.
//
//  PARAMETERS:
//
//  RETURN    : TRUE             - success
//              FALSE            - failure
/////////////////////////////////////////////////////////////////////////////
//./END + 2
zBOOL
EnableDebugPrivNT( )
{
   HANDLE hToken;
   LUID DebugValue;
   TOKEN_PRIVILEGES tkp;
   TOKEN_PRIVILEGES tkpPrev;
   DWORD dwReturnLth;

   // Retrieve a handle of the access token.
   if ( !OpenProcessToken( GetCurrentProcess( ), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
   {
   // TRACE1( "OpenProcessToken failed with %d\n", GetLastError( ) );
      ZeidonFormatMessage( "OpenProcessToken", FALSE );
      return( FALSE );
   }

   // Enable the SE_DEBUG_NAME privilege.
   if ( !LookupPrivilegeValue( 0, SE_DEBUG_NAME, &DebugValue ) )
   {
   // TRACE1( "LookupPrivilegeValue failed with %d\n", GetLastError( ) );
      ZeidonFormatMessage( "LookupPrivilegeValue", FALSE );
      return( FALSE );
   }

   tkp.PrivilegeCount = 1;
   tkp.Privileges[ 0 ].Luid = DebugValue;
   tkp.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

   AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof( TOKEN_PRIVILEGES ), &tkpPrev, &dwReturnLth );

   // The return value of AdjustTokenPrivileges can't be tested.
   if ( GetLastError( ) != ERROR_SUCCESS )
   {
   // TRACE1( "AdjustTokenPrivileges failed with %d\n", GetLastError( ) );
      ZeidonFormatMessage( "AdjustTokenPrivileges", FALSE );
      return( FALSE );
   }

   return( TRUE );
}

zBOOL
KillProcess( PTASK_LIST tlist,
             zBOOL      bForce )
{
   HANDLE hProcess;


   if ( bForce || !tlist->hwnd )
   {
      hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, tlist->dwProcessId );
      if ( hProcess )
      {
         hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, tlist->dwProcessId );
         if ( hProcess == NULL )
         {
            return( FALSE );
         }

         if ( !TerminateProcess( hProcess, 1 ) )
         {
            CloseHandle( hProcess );
            return( FALSE );
         }

         CloseHandle( hProcess );
         return( TRUE );
      }
   }

   // Kill the process.
   PostMessage( tlist->hwnd, WM_CLOSE, 0, 0 );

   return( TRUE );
}

void
GetWindowTitles( PTASK_LIST_ENUM te )
{
   // Enumerate all windows.
   EnumWindows( EnumWindowsProc, (LPARAM) te );
}

/////////////////////////////////////////////////////////////////////////////
//
// Routine Description:
//
//   Callback function for window enumeration.
//
// Arguments:
//
//   hwnd             - window handle
//   lParam           - ** not used **
//
// Return Value:
//
//   TRUE  - continues the enumeration
//
/////////////////////////////////////////////////////////////////////////////
BOOL
CALLBACK EnumWindowsProc( HWND hwnd, LONG lParam )

{
   DWORD             pid = 0;
   DWORD             dw;
   CHAR              buf[ TITLE_SIZE + 1 ];
   PTASK_LIST_ENUM   te = (PTASK_LIST_ENUM) lParam;
   PTASK_LIST        tlist = te->tlist;
   DWORD             numTasks = te->numtasks;

   // Get the processid for this window.
   if ( !GetWindowThreadProcessId( hwnd, &pid ) )
   {
      return( TRUE );
   }

   // Look for the task in the task list for this window.
   for ( dw = 0; dw < numTasks; dw++ )
   {
      if ( tlist[ dw ].dwProcessId == pid )
      {
         tlist[ dw ].hwnd = hwnd;

         // We found the task so let's try to get the window text.
         if ( GetWindowText( tlist[ dw ].hwnd, buf, zsizeof( buf ) - 1 ) )
         {
            // Got it, so let's save it.
            strcpy_s( (zPCHAR) tlist[ dw ].WindowTitle, TITLE_SIZE, buf );
         }

         break;
      }
   }

   // Continue the enumeration.
   return( TRUE );
}

zBOOL
MatchPattern( PUCHAR String,
              PUCHAR Pattern )
{
   UCHAR   c, p, l;

   for ( ; ; )
   {
      switch ( p = *Pattern++ )
      {
         case 0:                                // end of pattern
            return( *String ? FALSE : TRUE );   // if end of string TRUE

         case '*':
            while ( *String )
            {               // match zero or more char
               if ( MatchPattern ( String++, Pattern ) )
                  return( TRUE );
            }

            return( MatchPattern( String, Pattern ) );

         case '?':
            if ( *String++ == 0 )               // match any one char
               return( FALSE );                 // not end of string

            break;

         case '[':
            if ( (c = *String++) == 0 )         // match char set
               return( FALSE );                 // syntax

            c = toupper( c );
            l = 0;
            while ( p = *Pattern++ )
            {
               if ( p == ']' )                  // if end of char set, then
                  return( FALSE );              // no match found

               if ( p == '-' )
               {                // check a range of chars?
                  p = *Pattern;                 // get high limit of range
                  if ( p == 0  ||  p == ']' )
                      return( FALSE );          // syntax

                  if ( c >= l  &&  c <= p )
                      break;                    // if in range, move on
               }

               l = p;
               if ( c == p )                    // if char matches this element
                  break;                        // move on
            }

            while ( p  &&  p != ']' )           // got a match in char set
               p = *Pattern++;                  // skip to end of set

            break;

         default:
            c = *String++;
            if ( toupper( c ) != p )            // check for exact char
               return( FALSE );                 // not a match

            break;
      }
   }
}

#endif   // zREMOTE_SERVER
