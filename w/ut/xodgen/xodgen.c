/*
   xodgen.c

   Generate XOD's

   Usage:
   XODGEN -p project -t te_name [-n] [-l logfile]

   project = LPLR
   te_name = TE as named in Zeidon TE tool
   -n triggers build of network LPLR's
   logfile = Log file written

   Change Log
   ----------

   2000.02.24 RG Z2000
       Logfile schliessen eingesetzt.
   1999.12.09 RG Z2000
       Fix: SysReadZeidonIni return -1 although the function
       read a special parameter is running correct. Therefore
       we give an error message if no parameter is returned.
   1999.12.08 RG Z2000
       - Fixed: Error message and If-Statement
       - To build the network XOD's we have to read the Default
         network from zeidon.ini to position in LOD TZTEDBLO
   29.09.1998 HH
      Created new.

*/

/*----------------------------------------------------------------------
** System headers, global headers
**----------------------------------------------------------------------
*/
#include <windows.h>

#define  KZSYSSVC_INCL
#include "kzoengaa.h"   /* Object Services */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "zeidonop.h"
#include "tz__oprs.h"
#include "tzlodopr.h"
#include "xodgen.h"

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, LPSTR, int);
void RunAppl( HWND hWnd );
long APIENTRY MainWndProc(HWND   hWnd,      /* window handle       */
                UINT   message,   /* type of message               */
                WPARAM wParam,    /* additional information        */
                LPARAM lParam);   /* additional information        */
static void ProcessXods( HWND hWnd, zVIEW vSubtask,
                          zCHAR *cTE_Name, zCHAR *cLogFile, zCHAR cNet );
static char* extract_name( char *pc, char* cTarget, size_t lMax );

/*
** globals
*/
char szOutLine[ 256 ] = "Initializing...";

/*
** defines
*/
typedef zSHORT ( __stdcall *GENFUNC) ( zVIEW, zVIEW, zVIEW, char *);


/****************************************************************************

    WinMain(HANDLE, HANDLE, LPSTR, int)

   Windows recognizes this function by name as the initial entry point
   for the program.  This function calls the application initialization
   routine, if no other instance of the program is running, and always
   calls the instance initialization routine.  It then executes a message
   retrieval and dispatch loop that is the top-level control structure
   for the remainder of execution.  The loop is terminated when a WM_QUIT
   message is received, at which time this function exits the application
   instance by returning the value passed by PostQuitMessage().

   If this function must abort before entering the message loop, it
    returns the conventional value NULL.

****************************************************************************/

int PASCAL WinMain(HANDLE hInstance,       /* current instance  */
               HANDLE hPrevInstance,       /* current instance  */
               LPSTR  lpCmdLine,           /* command line             */
               int    nCmdShow)        /* show-window type (open/icon) */
{
    MSG msg;                     /* message              */

    if (!hPrevInstance)          /* Other instances of app running? */
       if (!InitApplication(hInstance)) /* Initialize shared things */
          return (FALSE);      /* Exits if unable to initialize     */

    /* Perform initializations that apply to a specific instance
     */

    if (!InitInstance(hInstance, lpCmdLine, nCmdShow))
        return (FALSE);

    /* Acquire and dispatch messages until a WM_QUIT message is received.
     */

    while (GetMessage(&msg,    /* message structure              */
        (HWND) NULL,   /* handle of window receiving the message */
        0,             /* lowest message to examine          */
        0))            /* highest message to examine         */
    {
       TranslateMessage(&msg);    /* Translates virtual key codes    */
       DispatchMessage(&msg);     /* Dispatches message to window    */
    }

    return (msg.wParam);       /* Returns the value from PostQuitMessage */
} // WinMain

/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    Initializes window data and registers window class

   This function is called at initialization time only if no other
   instances of the application are running.  This function performs
   initialization tasks that can be done once for any number of running
   instances.

   In this case, we initialize a window class by filling out a data
   structure of type WNDCLASS and calling the Windows RegisterClass()
   function.  Since all instances of this application use the same window
   class, we only need to do this when the first instance is initialized.


****************************************************************************/

BOOL InitApplication (HANDLE hInstance)
{
   WNDCLASS  wc;

   ZeroMemory( &wc, sizeof( WNDCLASS ) );

   /*
    * Fill in window class structure with parameters that describe the
    *  main window.
    */

    wc.style = CS_HREDRAW | CS_VREDRAW;  /* Class style(s).                   */
    wc.lpfnWndProc = MainWndProc;        /* Function to retrieve messages     */
                                           /* for windows of this class.      */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = (HICON) NULL;
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  NULL;
    wc.lpszClassName = "xodgen";      /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code.
     */

    return (RegisterClass(&wc));

}  // InitApplication


/****************************************************************************

    InitInstance(HANDLE, lpCmdLine, int)

   Saves instance handle and creates main window

   This function is called at initialization time for every instance of
   this application.  This function performs initialization tasks that
   cannot be shared by multiple instances.

   In this case, we save the instance handle in a static variable and
   create and display the main program window.

****************************************************************************/

BOOL InitInstance(HANDLE hInstance, /* Current instance identifier.       */
              LPSTR  lpCmdLine, /* command line                       */
              int    nCmdShow)  /* Param for first ShowWindow() call. */
{
    HWND            hWnd;               /* Main window handle.                */

    /* Create a main window for this application instance.
    */
    hWnd = CreateWindow(
      "xodgen",                    /* See RegisterClass() call.          */
      "Zeidon Generate XODs",      /* Text for window title bar.         */
      WS_OVERLAPPEDWINDOW,         /* Window style.                      */
      CW_USEDEFAULT,               /* Default horizontal position.       */
      CW_USEDEFAULT,               /* Default vertical position.         */
      500, // CW_USEDEFAULT,       /*  width.                     */
      50, //CW_USEDEFAULT,         /*  height.                    */
      NULL,                        /* Overlapped windows have no parent. */
      NULL,                        /* Use the window class menu.         */
      hInstance,                   /* This instance owns this window.    */
      NULL                         /* Pointer not needed.                */
    );

   /* If window could not be created, return "failure"
    */
   if (!hWnd)
      return (FALSE);

   ShowWindow (hWnd, nCmdShow);
   UpdateWindow (hWnd);

   // Pass the commandline on to subsequent processing
   SetWindowLong( hWnd, GWL_USERDATA, (LONG) lpCmdLine );

   /* return "success"
    */
   return (TRUE);

} // InitInstance

/****************************************************************************

    FUNCTION: MainWndProc(HWND, UINT, WPARAM, LPARAM)

    Processes messages

****************************************************************************/
long APIENTRY MainWndProc(HWND hWnd,   /* window handle                 */
                UINT   message,   /* type of message               */
                WPARAM wParam,    /* additional information        */
                LPARAM lParam)    /* additional information        */

{
   HDC hdc;
   PAINTSTRUCT tPaint;

   switch (message)
   {
      case WM_CREATE:       /* message: command from application menu */
         // At this time the message loop is not setup.
         PostMessage( hWnd, WM_COMMAND, CMD_RUN, 0 );
         return( 0 )

      case WM_DESTROY:          /* message: window being destroyed */
         PostQuitMessage(0);
         return( 0 )

      case WM_COMMAND:
         // now the message loop is ok
         if ( wParam == CMD_RUN )
         {
            // Run the Application
            RunAppl( hWnd );
            PostQuitMessage(0);
         }
         return( 0 )

     case WM_PAINT:
         hdc = BeginPaint( hWnd, &tPaint );
         SelectObject( hdc, GetStockObject (SYSTEM_FIXED_FONT) );
         TextOut( hdc, 0, 0, szOutLine, zstrlen(szOutLine) );
         EndPaint( hWnd, &tPaint );

         return( 0 )

   }

    return DefWindowProc (hWnd, message, wParam, lParam);
} // MainWndProc

/****************************************************************************

    FUNCTION: RunAppl

    Do "everything"

****************************************************************************/

void RunAppl( HWND hWnd )
{
   zVIEW vSubtask = NULL;
   zSHORT nRC;
   char cNet = '\0', cError = '\0' ;
   char cApplication[ 32 ], cTE_Name[ 256 ], cLogFile[ 256 ] ;
   char *pc;

   HANDLE hInstance = (HANDLE) GetWindowLong( hWnd, GWL_HINSTANCE );
   LPSTR  lpCmdLine = (LPSTR) GetWindowLong( hWnd, GWL_USERDATA );

   // Analyze the Command Line

   // The command line is
   // "-p Application" "-t TE-Name" "[-n]" ["Log-File"]
   // "Application" and "TE-Name" are  required

   cApplication[ 0 ] = cLogFile[ 0 ] = '\0';
   pc = lpCmdLine + strspn( lpCmdLine, " "); // Blank-Eli

   do
   {

      if ( *pc != '-' )
      {
         cError = 'E';
         break;
      }
      ++pc;
      pc = pc + strspn( pc, " "); // Blank-Eli

      switch ( *pc )
      {
         case 'p':
            // Application name
            ++pc;
            pc = extract_name( pc, cApplication, sizeof( cApplication ) );
            break;

         case 't':
            // DBMS source name
            ++pc;
            pc = extract_name( pc, cTE_Name, sizeof( cTE_Name ) );
            break;

         case 'l':
            // Log file
            ++pc;
            pc = extract_name( pc, cLogFile, sizeof( cLogFile ) );
            break;

         case 'n':
            // Network switch
            cNet = 'X';
            ++pc;
            pc = pc + strspn( pc, " "); // Blank-Eli
            break;

         default:
            cError = 'E';
            break;
      }
   } while ( *pc != '\0' && cError == '\0' );

   if ( cApplication[ 0 ] == '\0' || cTE_Name[ 0 ] == '\0'|| cError != '\0' )
   {
      MessageBox( hWnd,
                  "Invalid Command Line - Application and TE-Name required",
                  "Error Running xodgen",
                  MB_OK | MB_ICONSTOP | MB_APPLMODAL );
      return;
   }

// nRC = RegisterZeidonApplication( &vSubtask, (zLONG) hInstance, (zLONG) hWnd,
//                                  WM_USER + 1, "~~Zeidon_Tools~", 0, 0 );
   nRC = RegisterZeidonApplication( &vSubtask, 0L, 0L,
                                    WM_USER + 1, "~~Zeidon_Tools~", 0, 0 );


   nRC= InitializeLPLR( vSubtask, cApplication );

   ProcessXods( hWnd, vSubtask, cTE_Name, cLogFile, cNet );

   UnregisterZeidonApplication( );
}

static void ProcessXods( HWND hWnd, zVIEW vSubtask,
                          zCHAR *cTE_Name, zCHAR *cLogFile, zCHAR cNet )
{
   FILE *fLog=NULL;
   zVIEW vTaskLPLR = NULL;
   zCHAR szMsg[ 256 ];
   zVIEW vTZTEDBLO, vDTE, vCM_List;
   zVIEW vLOD, vLOD_List, vXOD;
   zVIEW vTemp = NULL;
   zVIEW vT=NULL;
   zSHORT nRC;
   RECT rect;
   LPLIBRARY hLib = NULL;
   zCHAR szLODName[ 32 ];
   zCHAR szFileName[ 256 ];
   zCHAR szNetwork[ 256 ];

   if ( cLogFile && cLogFile[0] )
      fLog = fopen( cLogFile, "w");

   memset (szNetwork, 0, 256);
   SysReadZeidonIni( -1, "[Zeidon]", "DefaultNetWork", szNetwork );
   if (*szNetwork == 0)
   {
      strcpy( szMsg, "No default Network set" );
      MessageBox( hWnd, szMsg,
                  "Error Running xodgen",
                  MB_OK | MB_ICONSTOP | MB_APPLMODAL );
      strcat_s( szMsg, sizeof( szMsg ), "\n" );
      if ( fLog )
      {
         fputs( szMsg,fLog );
         fclose (fLog);
      }
      return;
   }

   ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl",
                        zMULTIPLE );
   SetNameForView( vTZTEDBLO, "TZTEDBLO", 0, zLEVEL_TASK );

   //oTZTENVRO_GetUpdViewForDTE_P
   nRC = RetrieveViewForMetaList( vSubtask, vSubtask, &vCM_List, zSOURCE_DTE_META );

   nRC = ActivateMetaOI( &vDTE, vCM_List, zSOURCE_DTE_META,
                            zSINGLE | zACTIVATE_ROOTONLY );
   if ( nRC < 0 )
   {
      strcpy( szMsg, "Internal error activating TE" );
      MessageBox( hWnd, szMsg,
                  "Error Running xodgen",
                  MB_OK | MB_ICONSTOP | MB_APPLMODAL );
      strcat_s( szMsg, sizeof( szMsg ), "\n" );
      if ( fLog )
      {
         fputs( szMsg,fLog );
         fclose (fLog);
      }
      return;
   }
   SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   // Position on given DBMS Source
   nRC = SetCursorFirstEntityByString( vDTE,
               "TE_DBMS_Source", "Name", cTE_Name, 0 );
   if ( nRC < zCURSOR_SET )
   {
      strcpy( szMsg, "Invalid TE Name :");
      strcat( szMsg, cTE_Name );
      MessageBox( hWnd, szMsg,
                  "Error Running xodgen",
                  MB_OK | MB_ICONSTOP | MB_APPLMODAL );
      strcat_s( szMsg, sizeof( szMsg ), "\n" );
      if ( fLog )
      {
         fputs( szMsg,fLog );
         fclose (fLog);
      }
      return;
   }

   nRC = GetViewByName( &vTaskLPLR, "TaskLPLR", 0, zLEVEL_TASK );
   assert(nRC >= 0 ) ;

   // Check for network build
   if ( cNet )
   {
      // Use default network for Ton Beller (DBMS = "Network Server")
      SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "Network",
                                    szNetwork, 0 );
      SetAttributeFromAttribute( vDTE,      "TE_DBMS_Source", "DBMS",
                                 vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
      SetAttributeFromAttribute( vDTE,      "TE_DBMS_Source", "Network",
                                 vTZTEDBLO, "TE_DBMS_Source", "Network" );
   }
   else
   {
      // Position on corresponding DBMS entry in DB Handler object.
      SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                 vDTE, "TE_DBMS_Source", "DBMS", 0 );

   }


   //zwTZTEUPDD_BuildXODsOnLPLR( zVIEW vSubtask )

   // Create a view that lists all LODs.

   RetrieveViewForMetaList( vSubtask, vSubtask, &vLOD_List, zREFER_LOD_META );
   OrderEntityForView( vLOD_List, "W_MetaDef", "Name A" );
   SetNameForView( vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Loop through each of the LOD's
   // WMetaType.Type= zREFER_LOD_META
   //
   for ( nRC = SetCursorFirstEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_List, "W_MetaDef", "" ) )
   {

      GetStringFromAttribute( szLODName, sizeof( szLODName ),
                              vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Could not Activate LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLODName );
         strcat_s( szMsg, sizeof( szMsg ), ".\nAborting Build" );

         MessageBox( hWnd,
                  szMsg,
                  "Error Running xodgen",
                  MB_OK | MB_ICONSTOP | MB_APPLMODAL );

         strcat_s( szMsg, sizeof( szMsg ), "\n" );
         if ( fLog )
         {
            fputs( szMsg,fLog );
            fclose (fLog);
         }

         return ;
      }
      else
      {
         // Send message that we are building the LOD.
         strcpy( szMsg, "Building executable for LOD: " );
         strcat( szMsg, szLODName );
         strcat( szMsg, "." );
         strcpy( szOutLine, szMsg );
         GetClientRect( hWnd, &rect );
         InvalidateRect( hWnd, &rect, TRUE );
         UpdateWindow( hWnd );

         if ( fLog )
         {
           strcat_s( szMsg, sizeof( szMsg ), "\n" );
           fputs( szMsg,fLog );
         }
         else
            TraceLineS( szMsg, "" );

         // Make sure the TE_SourceZKey attribute is set because it determines
         // what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
            SetAttributeFromAttribute ( vLOD, "POD", "TE_SourceZKey",
                                       vDTE, "TE_DBMS_Source", "ZKey" );

         // Build the XOD in memory
         oTZZOXODO_SaveXOD( vLOD );
         SetNameForView( vLOD, "TZZOLODO", 0, zLEVEL_TASK );
         DropMetaOI( vSubtask, vLOD );

         // Commit the XOD to LPLR file.
         GetViewByName( &vXOD, "TZZOXODO", 0, zLEVEL_TASK );
         GetStringFromAttribute( szFileName, sizeof( szFileName ),
                                 vTaskLPLR, "LPLR", "ExecDir" );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, sizeof( szFileName ), szLODName );
         strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
         CommitOI_ToFile( vXOD, szFileName, zSINGLE );
      }
   }

   if (fLog)
      fclose (fLog);
   else
      TraceLineS( "Completed XOD build", "" );

   return;

}

static char* extract_name( char *pc, char* cTarget, size_t lMax )
{
   char * pc1;
   size_t len;

   pc = pc + strspn( pc, " "); // Blank-Eli
   pc1 = strchr( pc, ' ');

   if ( pc1 )
   {
      len = pc1 - pc;
   }
   else
   {
      len = strlen( pc );
      pc1 = pc + len;
   }

   if ( len < lMax -1 )
   {
      memcpy( cTarget, pc, len );
      cTarget[ len ] = '\0';
   }
   else
   {
      // truncate if space not sufficient
      memcpy( cTarget, pc, lMax-1 );
      cTarget[ lMax-1 ] = '\0';
   }

   pc1 = pc1 + strspn( pc1, " "); // Blank-Eli
   return pc1;
}
