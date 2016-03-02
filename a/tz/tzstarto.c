/*+--------------------------------------------------------------------------+*/
/*|                                                                          |*/
/*| PROGRAM NAME: TZSTARTO                                                   |*/
/*| -------------                                                            |*/
/*|  Background program to start Zeidon Operations                           |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*|  Copyright (C) Ton Beller GmbH, Quinsoft Inc. 1997                       |*/
/*|                                                                          |*/
/*| DISCLAIMER OF WARRANTIES:                                                |*/
/*| -------------------------                                                |*/
/*| The following [enclosed] code is sample code created by Ton Beller       |*/
/*| GmbH.This sample code is not part of any standard TB product             |*/
/*| and is provided to you solely for the purpose of assisting you in the    |*/
/*| development of your applications.  The code is provided "AS IS",         |*/
/*| without warranty of any kind.  TB shall not be liable for any damages    |*/
/*| arising out of your use of the sample code, even if they have been       |*/
/*| advised of the possibility of such damages.                              |*/
/*|                                                                          |*/
/*| REVISION LEVEL: 1.0                                                      |*/
/*| ---------------                                                          |*/
/*|                                                                          |*/
/*| WHAT THIS PROGRAM DOES:                                                  |*/
/*| -----------------------                                                  |*/
/*|  This program does just a connect to the Zeidon OE and starts the        |*/
/*|  Global Op, which was specified in the command line.                     |*/
/*|                                                                          |*/
/*|  Syntax:                                                                 |*/
/*|                                                                          |*/
/*|    TZSTARTO.EXE LPLRName DLLName GlobalOpName                            |*/
/*|                                                                          |*/
/*|                                                                          |*/
/*| WHAT THIS PROGRAM DEMONSTRATES:                                          |*/
/*| -------------------------------                                          |*/
/*|  This program demonstrates how background processes can access the       |*/
/*|  Zeidon Object Engine without using Zeidon dialogs.                      |*/
/*|                                                                          |*/
/*|                                                                          |*/
/*|  REQUIRED FILES:                                                         |*/
/*|  ---------------                                                         |*/
/*|                                                                          |*/
/*|    TZSTARTO.C     - Source code                                          |*/
/*|                                                                          |*/
/*|    OS2.H          - APIs include file                                    |*/
/*|    STDLIB.H       - Standard library function declarations               |*/
/*|    STRING.H       - String handling function declarations                |*/
/*|    STDIO.H        - Handling IO                                          |*/
/*|    CTYPE.H        - For mapping different casing                         |*/
/*|                                                                          |*/
/*|  REQUIRED LIBRARIES:                                                     |*/
/*|  -------------------                                                     |*/
/*|                                                                          |*/
/*|    OS2386.LIB     - Presentation Manager/OS2 library                     |*/
/*|    DDE4SBS.LIB    - C Set++ Standard Library                             |*/
/*|    KZOENGAA.LIB   - Zeidon OE                                            |*/
/*|                                                                          |*/
/*|  REQUIRED PROGRAMS:                                                      |*/
/*|  ------------------                                                      |*/
/*|                                                                          |*/
/*|    IBM C Set++ Compiler                                                  |*/
/*|    IBM Linker                                                            |*/
/*|                                                                          |*/
/*|    or Borland C++ 2.0 for OS/2                                           |*/
/*|                                                                          |*/
/*|    or MSVC 1.52 for the Windows version                                  |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/

/*
CHANGE LOG:

22.07.1997  GT
   new module
*/

#include <windows.h>

#define zGLOBAL_DATA
#define  KZSYSSVC_INCL
#include <kzoengaa.h>   /* Object Services */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


extern void exit( int );

typedef zSHORT (POPERATION zGLOBAL_OP)( zVOID );

typedef struct global_op {
  zCHAR szApp[ 33 ];
  zCHAR szDLL[ 256 ];
  zCHAR szOp[ 33 ];
  zVIEW vTask;
  zBOOL bDebug;
  HINSTANCE hInst;
  HWND      hWnd;
} T_GLOBAL_OP;



// defines for MyHelp()
#define FULL_HELP 1
#define LINE_HELP 2

#define ID_WINDOW           1000

static zCHAR szClassName[] = "TZSTARTO";
static T_GLOBAL_OP GlobalOp;

///////////////////////////////
/*   Function Prototypes     */
///////////////////////////////

static zSHORT fnProcessCommandParm( int, zCHAR **, T_GLOBAL_OP *);
static zSHORT fnConnect(T_GLOBAL_OP * pGO);
static void  fnDisconnect(T_GLOBAL_OP * pGO);
static void  fnMyHelp(int );
static void fnRunOp(T_GLOBAL_OP * pGO);

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, LPSTR, int);
long CALLBACK __export MainWndProc(HWND, UINT, WPARAM, LPARAM);

//./ ADD NAME=fnMyHelp
// Source Module=tzstarto.c
////////////////////////////////////////////////
//
//  OPERATION:  fnMyHelp
//
//  PURPOSE:    Help for Command Line Parameters
//
//  PARAMETERS: vSubtask- View to a zeidon window
//
//  RETURNS:    -
//
////////////////////////////////////////////////
//./ END + 9
static void fnMyHelp(int nHelpType)
{
  zCHAR szMsg[512];

  strcpy_s( szMsg, zsizeof( szMsg ), "TZSTARTO - back ground process to start global ops\n");
  strcat_s( szMsg, zsizeof( szMsg ), "Copyright (C) Ton Beller GmbH, Quinsoft Inc. 1997.\n\n");

  if (nHelpType == LINE_HELP)
  {
    strcat_s( szMsg, zsizeof( szMsg ), "\n");
    strcat_s( szMsg, zsizeof( szMsg ), "Call Syntax:\n");
    strcat_s( szMsg, zsizeof( szMsg ), " TZSTARTO LPLR DLL Operation\n");
    strcat_s( szMsg, zsizeof( szMsg ), "\n");
  }
  else
  {
    strcat_s( szMsg, zsizeof( szMsg ), "\n");
    strcat_s( szMsg, zsizeof( szMsg ), "Call Syntax:\n");
    strcat_s( szMsg, zsizeof( szMsg ), " TZSTARTO LPLR DLL Operation\n");
    strcat_s( szMsg, zsizeof( szMsg ), " LPLR - name of the Zeidon application (LPLR)\n");
    strcat_s( szMsg, zsizeof( szMsg ), " DLL - name of the DLL with the Global Op\n");
    strcat_s( szMsg, zsizeof( szMsg ), " Operation - name of the Global Op to run\n");
    strcat_s( szMsg, zsizeof( szMsg ), "\n");
  }

  SysMessageBox( "TZSTARTO", szMsg, 0);
  return;
} // fnMyHelp


//./ ADD NAME=fnProcessCommandParm
// Source Module=tzstarto.c
////////////////////////////////////////////////
//
//  OPERATION:  fnProcessCommandParm
//
//  PURPOSE:    Parse command line parameters into pGO
//
//  PARAMETERS: argc - Number of arguments
//              argv - Array with arguments
//              pGO - Data of the global op to start
//
//  RETURNS:    0 - Connected
//              else Error
//
////////////////////////////////////////////////
//./ END + 9
static zSHORT
fnProcessCommandParm(int argc, zCHAR **argv, T_GLOBAL_OP *pGO)
{
  if ( argc == 1)
  {
    fnMyHelp(LINE_HELP);
    exit (0);
  }
  argv++;
  argc--;

  /////////////////////
  /* Process options */
  /////////////////////

  while ( argc > 0 && (**argv == '-' ||  **argv == '/' ) )
  {
    (*argv)++;
    while (**argv != 0)
    {
      switch (**argv)
      {

         case  'd':
    case  'D':
           pGO->bDebug = TRUE;
           break;

         case  'h':
         case  'H':
           fnMyHelp(FULL_HELP);
           exit(0);
           break;

         default:
           fnMyHelp(LINE_HELP);
           exit(1);
           break;
      }
      (*argv)++;
    }
    argv++;
    argc--;
  }

  // check the global op data
  if (argc != 3)
  {
    fnMyHelp(LINE_HELP);
    exit (0);
  }

  // get the global op data
  zmemset( pGO, 0, zsizeof(T_GLOBAL_OP) );
  strncpy_s( pGO->szApp, zsizeof( pGO->szApp ), argv[ 0 ], zsizeof( pGO->szApp ) - 1 );
  strncpy_s( pGO->szDLL, zsizeof( pGO->szDLL ), argv[ 1 ], zsizeof( pGO->szDLL ) - 1 );
  strncpy_s( pGO->szOp, zsizeof( pGO->szOp ), argv[ 2 ], zsizeof( pGO->szOp ) - 1 );

  return( 0 );
} // fnProcessCommandParm

//./ ADD NAME=fnConnect
// Source Module=tzstarto.c
////////////////////////////////////////////////
//
//  OPERATION:  fnConnect
//
//  PURPOSE:    Connect to Zeidon OE
//
//  PARAMETERS: pGO - Data for connection
//
//  RETURNS:    0 - connected
//             -1 - can't connect
//
////////////////////////////////////////////////
//./ END + 9
static zSHORT
fnConnect( T_GLOBAL_OP *pGO )
{
  zCHAR szMsg[ 120 ];
  zCHAR szApp[ 60 ];

  // Initialize window for application
  // Connect to Zeidon
  strcpy_s( szApp, zsizeof( szApp ), "\\\\" );
  zstrcat_s( szApp, zsizeof( szApp ), pGO->szApp);
  zstrcat_s( szApp, zsizeof( szApp ), "\\");

  if ( RegisterZeidonApplication( &(pGO->vTask),
                                  (zLONG) pGO->hInst,
                                  (zLONG) pGO->hWnd,
                                  (zLONG) (WM_USER + 1),
                                  szApp, 0, 0 ) != 0 )
  {
    sprintf( szMsg, "Connect to Zeidon failed \n"
                    "Connect String: \"%s\"", szApp );
    SysMessageBox( "TZSTARTO", szApp, 1 );
    return( -1 );
  }

  TraceLineS( "(tzstarto) Connected to Zeidon", "" );
  sprintf( szMsg, "(tzstarto) Parameters: App = %s, DLL = %s, Op = %s ",
                  pGO->szApp, pGO->szDLL, pGO->szOp );
  TraceLineS( szMsg, "" );

  return( 0 );
} // fnConnect

//./ ADD NAME=fnDisconnect
// Source Module=tzstarto.c
////////////////////////////////////////////////
//
//  OPERATION:  fnDisconnect
//
//  PURPOSE:    Disconnect from Zeidon OE
//
//  PARAMETERS: pGO - Data for connection
//
//  RETURNS:    -
//
////////////////////////////////////////////////
//./ END + 9
static void fnDisconnect( T_GLOBAL_OP *pGO)
{
  // disconnect from Zeidon
  UnregisterZeidonApplication( );
  pGO->vTask = NULL;

} // fnDisconnect

//./ ADD NAME=fnRunOp
// Source Module=tzstarto.c
////////////////////////////////////////////////
//
//  OPERATION:  fnRunOp
//
//  PURPOSE:    Execute the Global Operation
//
//  PARAMETERS: pGO - Data for connection
//
//  RETURNS:    -
//
////////////////////////////////////////////////
//./ END + 9
static void fnRunOp( T_GLOBAL_OP *pGO)
{
  zCHAR szDLL_Path[ zMAX_FILENAME_LTH ];
  zCHAR szOpUpper[ 33 ];
  LPLIBRARY hLibrary;
  zGLOBAL_OP fnGlobalOp;


  // if DLL name does not contain path
  if ( zstrchr( pGO->szDLL, '\\' ) == NULL)
  {
    /* Append LPLR LIB path to DLL name */
    GetApplDirectoryFromView( szDLL_Path, pGO->vTask,
                              zAPPL_DIR_LIB, zMAX_FILENAME_LTH );
  }
  else
  {
    szDLL_Path[ 0 ] = 0;
  }

  zstrcat_s( szDLL_Path, zsizeof( szDLL_Path ), pGO->szDLL );

  // Load the DLL
  hLibrary = SysLoadLibrary( pGO->vTask, szDLL_Path );
  if ( hLibrary == 0 )
  {
    TraceLineS( "(tzstarto) can't load DLL ", szDLL_Path );
    return;
  }

  // get the operation from the DLL
  fnGlobalOp = SysGetProc( hLibrary, pGO->szOp );
  if ( fnGlobalOp == 0 )
  {
    // if operation not found, try to look for upper case name
    strcpy_s( szOpUpper, zsizeof( szOpUpper ) pGO->szOp );
    SysTranslateString( szOpUpper, 'U' );
    fnGlobalOp = SysGetProc( hLibrary, szOpUpper );
    if ( fnGlobalOp == 0 )
    {
      TraceLineS( "(tzstarto) Global Op does not exist in DLL ", pGO->szOp );
      return;
    }
  }

  // call the operation
  TraceLineS( "(tzstarto) Calling Global Op ", pGO->szOp );
  fnGlobalOp();
  TraceLineS( "(tzstarto) Return from Global Op call ", "" );

} // fnRunOp

/////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
//
//  PURPOSE: calls initialization function, processes message loop
//
//  COMMENTS:
//
//      Windows recognizes this function by name as the initial entry point
//      for the program.  This function calls the application initialization
//      routine, if no other instance of the program is running, and always
//      calls the instance initialization routine.  It then executes a message
//      retrieval and dispatch loop that is the top-level control structure
//      for the remainder of execution.  The loop is terminated when a WM_QUIT
//      message is received, at which time this function exits the application
//      instance by returning the value passed by PostQuitMessage().
//
//      If this function must abort before entering the message loop, it
//      returns the conventional value NULL.
//
/////////////////////////////////////////////////////////////////////////////////

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance;                /* current instance         */
HANDLE hPrevInstance;            /* previous instance        */
LPSTR lpCmdLine;                 /* command line             */
int nCmdShow;                    /* show-window type (open/icon) */
{
    MSG msg;                     /* message              */

    if (!hPrevInstance)          /* Other instances of app running? */
    if (!InitApplication(hInstance)) /* Initialize shared things */
        return(FALSE);       /* Exits if unable to initialize     */

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, lpCmdLine, nCmdShow))
        return(FALSE);

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while (GetMessage(&msg,    /* message structure              */
        (HWND) NULL,   /* handle of window receiving the message */
        0,             /* lowest message to examine          */
        0))            /* highest message to examine         */
    {
    TranslateMessage(&msg);    /* Translates virtual key codes       */
    DispatchMessage(&msg);     /* Dispatches message to window       */
    }
    return(msg.wParam);        /* Returns the value from PostQuitMessage */
} // WinMain

/////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: InitApplication(HANDLE)
//
//  PURPOSE: Initializes window data and registers window class
//
//  COMMENTS:
//
//      This function is called at initialization time only if no other
//      instances of the application are running.  This function performs
//      initialization tasks that can be done once for any number of running
//      instances.
//
//      In this case, we initialize a window class by filling out a data
//      structure of type WNDCLASS and calling the Windows RegisterClass()
//      function.  Since all instances of this application use the same window
//      class, we only need to do this when the first instance is initialized.
//
//
/////////////////////////////////////////////////////////////////////////////////
BOOL
InitApplication( HANDLE hInstance )  /* current instance       */
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the       */
    /* main window.                                                           */

    wc.style = 0;                    /* Class style(s).                    */
    wc.lpfnWndProc = MainWndProc;       /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = (HICON) NULL;
    wc.hCursor = (HCURSOR) NULL;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  NULL;
    wc.lpszClassName = "TZSTARTO";      /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */

    return( RegisterClass( &wc ) );

}  // InitApplication

/////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  InitInstance(HANDLE, lpCmdLine, int)
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
/////////////////////////////////////////////////////////////////////////////////
BOOL
InitInstance( HANDLE hInstance,  /* Current instance identifier.       */
              LPSTR  lpCmdLine,  /* command line             */
              int    nCmdShow )  /* Param for first ShowWindow() call. */
{
    HWND            hWnd;        /* Main window handle.                */
    int argc;
    zCHAR *argv[ 10 ];
    zSHORT nRC;

    // check the command line parameters
    zmemset( &GlobalOp, 0, zsizeof( T_GLOBAL_OP ) );

    argc = 1;
    argv[ 0 ] = "TZSTARTO.EXE";
    while ( *lpCmdLine && argc < 9 )
    {
      if ( *lpCmdLine )
      {
        argv[ argc ] = lpCmdLine;
        argc++;
      }

      while ( *lpCmdLine && *lpCmdLine != ' ' )
        lpCmdLine++;

      if ( *lpCmdLine == ' ' )
      {
        *lpCmdLine = 0;
        lpCmdLine++;
      }

      while ( *lpCmdLine && *lpCmdLine == ' ' )
        lpCmdLine++;
    }

    argv[ argc ] = NULL;

    nRC = fnProcessCommandParm( argc, argv, &GlobalOp );
    if ( nRC )
      exit( -1 );

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    GlobalOp.hInst = hInstance;

    /* Create a main window for this application instance.  */

    hWnd = CreateWindow(
        "TZSTARTO",                     /* See RegisterClass() call.          */
        "Start Zeidon Global Op",       /* Text for window title bar.         */
        WS_OVERLAPPEDWINDOW,            /* Window style.                      */
        CW_USEDEFAULT,                  /* Default horizontal position.       */
        CW_USEDEFAULT,                  /* Default vertical position.         */
        CW_USEDEFAULT,                  /* Default width.                     */
        CW_USEDEFAULT,                  /* Default height.                    */
        (HWND) NULL,                    /* Overlapped windows have no parent. */
        (HMENU) NULL,                   /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL );                         /* Pointer not needed.                */

    /* If window could not be created, return "failure" */

    if ( !hWnd )
        return( FALSE );

    /* Make the window visible; update its client area; and return "success" */

    if ( GlobalOp.bDebug )
    {
      ShowWindow( hWnd, nCmdShow );  /* Show the window                      */
      UpdateWindow( hWnd );          /* Sends WM_PAINT message               */
    }

    GlobalOp.hWnd = hWnd;
    return( TRUE );                /* Returns the value from PostQuitMessage */

} // InitInstance

/////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: MainWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages
//
//  MESSAGES:
//
//  WM_COMMAND    - application menu (About dialog box)
//  WM_DESTROY    - destroy window
//
//  COMMENTS:
//
//  To process the IDM_ABOUT message, call MakeProcInstance() to get the
//  current instance address of the About() function.  Then call Dialog
//  box which will create the box according to the information in your
//  generic.rc file and turn control over to the About() function.  When
//  it returns, free the intance address.
//
/////////////////////////////////////////////////////////////////////////////////
long CALLBACK __export
MainWndProc( HWND hWnd,      /* window handle                 */
             UINT message,   /* type of message               */
             WPARAM wParam,  /* additional information        */
             LPARAM lParam ) /* additional information        */
{
  zSHORT nRC;

    switch (message)
    {
        case WM_CREATE:       /* message: command from application menu */

            // Connecting to Zeidon
            GlobalOp.hWnd = hWnd;
            nRC = fnConnect( &GlobalOp );
            if ( nRC )
              exit( -1 );

            // Run the operation
            fnRunOp( &GlobalOp );

            // Disconnecting from Zeidon
            fnDisconnect( &GlobalOp );

            // We quit after running the global op
            PostQuitMessage( 0 );
            break;

        case WM_DESTROY:          /* message: window being destroyed */
            PostQuitMessage( 0 );
            break;

        default:                  /* Passes it on if unproccessed    */
            return( DefWindowProc( hWnd, message, wParam, lParam ) );
    }

    return( 0 );
} // MainWndProc
