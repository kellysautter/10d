//
// MODULE NAME:  KZSCHEDT  -  Object services program
// DESCRIPTION:  This is a Windows program which initializes the
//               Object services DLL and creates a tray window
//               to manage scheduled tasks.
// ************************************************************************
// * Copyright (c) 1993-2009 QuinSoft Corporation. All rights reserved.   *
// * Confidential and Proprietary material subject to license -           *
// * do not reproduce or disclose. This material is an unpublished work   *
// * and is considered a trade secret belonging to the copyright holder.  *
// ************************************************************************
//  AUTHOR:  Doug Sorensen
//  DATE:    2009.09.16
//  API:     Windows XP
//  ENVIRONMENT: Windows
//

/* CHANGE LOG
   ==========
*/

#include <windows.h>
#include <stdio.h>
#include <sys\stat.h>

#define  KZSYSSVC_INCL
#include <kzoengaa.h>   // Object Services
#include <zdecimal.h>   // Object Services

#define NUM_THREADS 1
// int zPASCAL WinMain( HANDLE, HANDLE, LPSTR, int );
HWND       InitInstance( HANDLE hInst, int  nCmdShow, LPSTR lpCmdLine );
BOOL       InitApplication( HANDLE hInst );

long zPASCAL
MainWndProc( HWND, unsigned, WPARAM, LPARAM );

// Definitions for using SysTray.
#define zWM_NOTIFYICON       (WM_APP + 100)
#define zTRAY_MESSAGE       1
#define zTRAY_MESSAGE_SHOW  zTRAY_MESSAGE + 1

#define TITLE_BUFSIZE 1024  // buffer size for console window titles
zBOOL g_DoWork = TRUE;


HANDLE g_hInst;                       // current instance
HINSTANCE g_hInstRes;
HWND   g_hWndMain;
char   g_szClassName[ ] = "TaskSchedulerClass";
WORD   g_wClientMessage;              // allocate message
int    g_nNoUnregister;
zVIEW  g_vSubtask;
zBOOL  g_bRunningTest = FALSE;
zULONG g_ulTestCount = 0;

#ifdef __WIN32__
#include <process.h>

zBOOL  g_bContinueThreadTest = 0;
zBOOL  g_bTestThreadStarted = FALSE;
zULONG g_hTestThread = 0;
zULONG g_dwTestThreadID = 0;

#define DECLARE_TLS __declspec( thread )
DECLARE_TLS zBOOL bThreadRegistered = FALSE;


unsigned __stdcall
fnThreadProc2( zPVOID p )
{
   zVIEW  vSubtask;
   zLONG  nCount = 0;
   zVIEW  vView;
   char   szMsg[ 200 ];
   zSHORT nRC;
   zSHORT nThreadNumber = (zSHORT) p;

   TraceLineI( "Starting thread test", nThreadNumber );

   while ( g_bContinueThreadTest )
   {
      zCHAR szDateTime[ 30 ];

      if ( !bThreadRegistered )
      {
         if ( RegisterZeidonApplication( &vSubtask,
                                         (zLONG) g_hInst,
                                         (zLONG) g_hWndMain,
                                         (zLONG) g_wClientMessage,
                                         "\\\\ePamms\\", 0, 0 ) == zCALL_ERROR )
         {
            g_dwTestThreadID = 0;
            return( 0 );
         }

         bThreadRegistered = TRUE;
         TraceLineI( "Completed thread register", nThreadNumber );
      }

   // if ( nThreadNumber == 2 )
   //    Sleep( 500 );
      Sleep( 100 * ((nThreadNumber % 5) + 1) );

   // SysMutexLock( "KZSCHEDT/1", 0, 0 );
   // TraceLineI( "Mutex locked", nThreadNumber );

      SysGetDateTime( szDateTime, sizeof( szDateTime ) );
      TraceLineI( szDateTime, nThreadNumber );

      nRC = ActivateObjectInstance( &vView, "mUser", vSubtask, 0, zSINGLE );
      if ( nRC >= 0 )
         DropView( vView );
      else
         TraceLineI( "----- ERROR activating mUser --------- ", nThreadNumber );

      sprintf_s( szMsg, sizeof( szMsg ), "Thread test #%d, rc = %d", nCount++, nRC );
      TraceLineI( szMsg, nThreadNumber );

   // SysMutexUnlock( "KZSCHEDT/1" );
   // TraceLineI( "Mutex unlocked", nThreadNumber );
   }

   if ( bThreadRegistered )
   {
      UnregisterZeidonApplication( vSubtask );
      vSubtask = 0;
      bThreadRegistered = FALSE;
      TraceLineI( "Completed thread UN(!)register", nThreadNumber );
   }

   g_dwTestThreadID = 0;
   TraceLineI( "Completed thread test", nThreadNumber );

   return( 0 );
}

#endif // __WIN32__

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: TrayMessage
//  PURPOSE:   Add/modify or remove Icons or messages to systray
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
TrayMessage( DWORD dwMessage, UINT uID, HICON hIcon, LPCSTR pszTip )
{
   NOTIFYICONDATA tnd;
   zBOOL bRC;

   tnd.cbSize      = sizeof( NOTIFYICONDATA );
   tnd.hWnd        = g_hWndMain;
   tnd.uID         = uID;
   tnd.uFlags      = NIF_MESSAGE | NIF_ICON | NIF_TIP;
   tnd.uCallbackMessage = zWM_NOTIFYICON;  // notification message for message loop
   tnd.hIcon      = hIcon;
   if ( pszTip )
   {
      strncpy_s( tnd.szTip, sizeof( tnd.szTip ), pszTip, sizeof( tnd.szTip ) - 1 );
   }
   else
   {
      tnd.szTip[ 0 ] = 0;
   }

   bRC = Shell_NotifyIcon( dwMessage, &tnd );

   if ( hIcon )
       DestroyIcon( hIcon );

   return( bRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: SetupSysTray
//  PURPOSE:   To setup Systray for Zeidon Monitor.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
SetupSysTray( HINSTANCE hInst )
{
   zBOOL bReturn = FALSE;
   HICON hIcon = NULL;
   char szTip[ 128 ];

   hIcon = LoadIcon( hInst, "ZEIDON_MONITOR" );
   GetWindowText( g_hWndMain, szTip, 127 );
   bReturn = TrayMessage( NIM_ADD, zTRAY_MESSAGE_SHOW, hIcon, szTip );

   return( bReturn );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: DetachSysTray
//  PURPOSE:   Remove setting from systray.
//
/////////////////////////////////////////////////////////////////////////////
void
DetachSysTray( )
{
   TrayMessage( NIM_DELETE, zTRAY_MESSAGE_SHOW, NULL, NULL );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
PostBuild( zVIEW vSubtask )
{
   HMODULE   hLibrary;
   HINSTANCE hInst;
// zULONG ulWindowHandle;

// g_hWndMain = GetConsoleWindow( );

// GetWindowHandle( &ulWindowHandle, 0, vSubtask, 0 );
// g_hWndMain = (HWND) ulWindowHandle;

   hInst = (HINSTANCE) GetWindowLong( g_hWndMain, GWL_HINSTANCE );

   ShowWindow( g_hWndMain, SW_HIDE );

   hLibrary = LoadLibraryEx( "KZSchedT.exe", 0, LOAD_LIBRARY_AS_DATAFILE );
   g_hInstRes = (HINSTANCE) hLibrary;

   SetupSysTray( g_hInstRes );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: Cleanup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
Cleanup( zVIEW vSubtask )
{
   DetachSysTray( ); // remove Icon from systray

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: Exit
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
Exit( zVIEW vSubtask )
{
   g_DoWork = FALSE;
   Cleanup( vSubtask );

   return( 0 );
}

#define ID_SYSTRAY_MNU                  323

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       HandlePopupMenu
//
//  PURPOSE:     Create a popup menu for on systray notification
//
/////////////////////////////////////////////////////////////////////////////
void APIENTRY
HandlePopupMenu( HWND hWnd, POINT point )
{

   HMENU hMenu;
   HMENU hMenuTrackPopup;
   BOOL  bIsVisible;

   // Get the menu for the windows.
   hMenu = LoadMenu( g_hInstRes, MAKEINTRESOURCE( ID_SYSTRAY_MNU ) );

   if ( hMenu == 0 )
      return;

   // Get the first menu in it which we will use for the call to TrackPopup( ).
   // This could also have been created on the fly using CreatePopupMenu and
   // then we could have used InsertMenu( ) or AppendMenu.
   hMenuTrackPopup = GetSubMenu( hMenu, 0 );

   // This is required when using a notify icon -- see KB article
   // PRB: Menus for Notification Icons Don't Work Correctly
   SetForegroundWindow( hWnd );

   bIsVisible = IsWindowVisible( hWnd );

   TrackPopupMenu( hMenuTrackPopup, TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
                   point.x, point.y, 0, hWnd, NULL );

   // This is required when using a notify icon -- see KB article
   // PRB: Menus for Notification Icons Don't Work Correctly
   PostMessage( hWnd, WM_USER, 0, 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     WinMain
//  PURPOSE:   Establish message queue and create object
//             services window
//
/////////////////////////////////////////////////////////////////////////////

int zPASCAL
WinMain( HINSTANCE hInst,         // current instance
         HINSTANCE hPrevInstance, // previous instance
         LPSTR     lpCmdLine,     // command line
         int       nCmdShow )     // show-window type (open/icon)
{
   MSG msg;                       // message
   HWND hWnd;                     // main window handle

   if ( hPrevInstance == 0 )      // other instances of app running?
   {
      if ( InitApplication( hInst ) == 0 ) // initialize shared things
         return( FALSE );                  // exit if unable to initialize
   }

   // Perform initializations that apply to a specific instance.

   if ( (hWnd = InitInstance( hInst, nCmdShow, lpCmdLine )) == 0 )
      return( FALSE );

   // Set g_nNoUnregister to 0.
   g_nNoUnregister = 0;

   // Acquire and dispatch messages until a WM_QUIT message is received.
   while ( GetMessage( &msg,    // message structure
                       0,       // handle of window receiving the message
                       0,       // lowest message to examine
                       0 ) )    // highest message to examine
   {
      TranslateMessage( &msg ); // translate message
      DispatchMessage( &msg );  // dispatches message to window
   }

   return( msg.wParam );        // returns the value from PostQuitMessage
}

VOID CALLBACK
TimerProc( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
   zCHAR  szMsg[ 100 ];
#if 0
   zVIEW  vView;
   zVIEW  vLocalSubtask;

   SfCreateSubtask( &vLocalSubtask, g_vSubtask, "ePamms" );
   if ( vLocalSubtask )
   {
      zCHAR szDateTime[ 30 ];

      SysGetDateTime( szDateTime, sizeof( szDateTime ) );
      TraceLineI( szDateTime, 0 );

      ActivateObjectInstance( &vView, "mMedCode", vLocalSubtask, 0, zSINGLE );
      DropView( vView );
      SfDropSubtask( vLocalSubtask, 0 );
   }
#endif

   // DoWork( vSubtask );
   g_ulTestCount++;

   sprintf_s( szMsg, sizeof( szMsg ), "Done with test %u", g_ulTestCount );
   TraceLineS( szMsg, "" );
}

/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: MainWndProc(HWND, unsigned, WPARAM, LPARAM )
//
//  PURPOSE:  Processes messages for Task Scheduler
//
/////////////////////////////////////////////////////////////////////////////
long zPASCAL
MainWndProc( HWND     hWnd,       // window handle
             unsigned uMsg,       // type of message
             WPARAM   wParam,     // additional information
             LPARAM   lParam )    // additional information
{
   switch ( uMsg )
   {
      case WM_CLOSE:
         // Check to see if the network is up.
         if ( g_bRunningTest )
         {
            SysMessageBox( 0, "Test In Progress", "Close running test", -1 );
            return( 0 );
         }

         KillTimer( hWnd, 14 );  // 14 just picked out of the hat
         Exit( g_vSubtask );

         if ( g_vSubtask && NetStatus( g_vSubtask, "zWinSock" ) != 0 )
            NetClose( g_vSubtask, "zWinSock" );

         // It's OK to leave, shut down Object Services.
         if ( g_nNoUnregister != 99 && g_vSubtask )
            UnregisterZeidonApplication( g_vSubtask );

         return( DefWindowProc( hWnd, uMsg, wParam, lParam ));

      case WM_DESTROY:         // window being destroyed
         PostQuitMessage( 0 );
         break;

      case WM_COMMAND:
         if ( wParam == 114 )  // exit
         {
            PostMessage( hWnd, WM_CLOSE, 0, 0L );
         }
         else
         if ( wParam == 115 )  // ID_TEST
         {
            if ( g_bRunningTest )
            {
               g_bRunningTest = FALSE;
               g_bContinueThreadTest = FALSE;
            }
            else
            {
               int k;

               g_bRunningTest = TRUE;
            // SetTimer( hWnd, 0, 100, (TIMERPROC) TimerProc );

               g_bContinueThreadTest = TRUE;

               for ( k = 1; k <= NUM_THREADS; k++ )
               {
                  g_hTestThread = _beginthreadex( NULL, 0, fnThreadProc2,
                                                  (void *) k, 0, &g_dwTestThreadID );
                  if ( g_hTestThread )
                     CloseHandle( (HANDLE) g_hTestThread );
               }
            }
         }

         break;

      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
         if ( wParam == VK_F3 )
            PostMessage( hWnd, WM_CLOSE, 0, 0L );

         break;

      // entry for systray notifications
      case zWM_NOTIFYICON:
         if ( lParam == WM_RBUTTONDOWN )
         {
            POINT point;

            GetCursorPos( &point );
            HandlePopupMenu( hWnd, point );
         }

         break;

      default:
         if ( uMsg == g_wClientMessage )
            ProcessZeidonMessage( wParam, lParam );

         break;
   }

   return( DefWindowProc( hWnd, uMsg, wParam, lParam ));
}

/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  InitApplication( HANDLE hInatsnce )
//
//  PURPOSE:  Saves instance handle and creates main window
//
/////////////////////////////////////////////////////////////////////////////
BOOL
InitApplication( HANDLE hInst )     // current instance
{
   WNDCLASS  wc;

   // Fill in window class structure with parameters that describe the main window.

   wc.style = 0;                       // Class style(s).
   wc.lpfnWndProc = MainWndProc;       // Function to retrieve messages for
                                       // windows of this class.
   wc.cbClsExtra = 0;                  // No per-class extra data.
   wc.cbWndExtra = 0;                  // No per-window extra data.
   wc.hInstance = hInst;           // Application that owns the class.
   wc.hIcon = LoadIcon( hInst, "KZSCHEDT" );
   wc.hCursor = LoadCursor( 0, IDC_ARROW );
   wc.hbrBackground = GetStockObject( WHITE_BRUSH );
   wc.lpszMenuName =  0;               // Name of menu resource in .RC file.
   wc.lpszClassName = g_szClassName;     // Name used in call to CreateWindow.

   // Register the window class and return success/failure code.
   return( RegisterClass( &wc ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  InitInstance( HANDLE, int, LPSTR )
//
//  PURPOSE:  Saves instance handle and creates main window
//
//  COMMENTS:
//
//      This function is called at initialization time for every instance of
//      this application.  This function performs initialization tasks that
//      cannot be shared by multiple instances.
//
//      In this case, we save the instance handle in a static variable and
//      create and display the main program window.
//
/////////////////////////////////////////////////////////////////////////////
HWND
InitInstance( HANDLE hInst,       // current instance identifier.
              int    nCmdShow,    // param for first ShowWindow( ) call
              LPSTR lpCmdLine )   // command line
{
   int   nHeight, nPosY;
   int   nWidth, nPosX;

   // Save the instance handle in static variable, which will be used in many subsequent
   // calls from this application to Windows.

   g_hInst = hInst;

   // Get the dimensions of the screen.
   nWidth = GetSystemMetrics( SM_CXSCREEN );
   nHeight = GetSystemMetrics( SM_CYSCREEN );
   nWidth /= 4;
   nHeight /= 4;
   nWidth -= 40;
   nHeight -= 20;
   nPosX = nWidth / 2;
   nPosY = nHeight / 2;
   nPosX -= 8; // so it doesn't occupy same space as object services
   nPosY -= 8;

   // Create a main window for this application instance.
   g_hWndMain = CreateWindow( g_szClassName,                // see RegisterClass( ) call
                            "Task Scheduler",               // text for window title bar
                            WS_OVERLAPPEDWINDOW,            // window style
                            nPosX,                          // default horizontal position
                            nPosY,                          // default vertical position
                            nWidth,                         // default width
                            nHeight,                        // default height
                            0,                              // overlapped windows have no parent
                            LoadMenu( hInst, "KZSCHEDT" ),  // menu for task scheduler window
                         // 0,                              // use default menu
                            hInst,                          // this instance owns this window
                            0 );                            // pointer not needed


   if ( g_hWndMain == 0 )  // if window could not be created, return "failure"
      return( FALSE );

   g_wClientMessage = RegisterWindowMessage( "KZSCHEDT" );

   if ( RegisterZeidonApplication( &g_vSubtask, (zLONG) g_hInst,
                                   (zLONG) g_hWndMain,
                                   (zLONG) g_wClientMessage,
                                   "\\\\ePamms\\", 0, 0 ) != 0 )
                                   //lpCmdLine, 0, 0 ) != 0 )
   {
      DestroyWindow( g_hWndMain );
      return( FALSE );
   }

   // Make the window visible ... will be undone in PostBuild.
   ShowWindow( g_hWndMain, SW_SHOW );  // show the window
   TraceLineS( "Done registering", "" );

// PostBuild( g_vSubtask );
   SetTimer( g_hWndMain, 14, 5000, (TIMERPROC) TimerProc );    // 14 just picked out of the hat ... 5 second timeout

   return( g_hWndMain );
}
