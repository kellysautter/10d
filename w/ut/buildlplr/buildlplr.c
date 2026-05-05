/*
   buildlplr.c

   Generate XOD's

   Usage:
   buildlplr -p project -t te_name [-n] [-l logfile]

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
#include "buildlplr.h"

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, LPSTR, int);
void RunAppl( );
long APIENTRY MainWndProc(HWND   hWnd,      /* window handle       */
                UINT   message,   /* type of message               */
                WPARAM wParam,    /* additional information        */
                LPARAM lParam);   /* additional information        */
static void ProcessXods( zVIEW vSubtask,
                          zCHAR *cTE_Name, zCHAR *cLogFile );
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
    wc.lpszClassName = "buildlplr";      /* Name used in call to CreateWindow. */

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
      "buildlplr",                    /* See RegisterClass() call.          */
      "Zeidon Generate XODs",      /* Text for window title bar.         */
      WS_OVERLAPPEDWINDOW,         /* Window style.                      */
      CW_USEDEFAULT,               /* Default horizontal position.       */
      CW_USEDEFAULT,               /* Default vertical position.         */
      500, // CW_USEDEFAULT,       /*  width.                     */
      500, //CW_USEDEFAULT,         /*  height.                    */
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
         return( 0 );

      case WM_DESTROY:          /* message: window being destroyed */
         PostQuitMessage(0);
         return( 0 );

      case WM_COMMAND:
         // now the message loop is ok
         if ( wParam == CMD_RUN )
         {
            // Run the Application
            RunAppl( );
            PostQuitMessage(0);
         }
         return( 0 );

     case WM_PAINT:
         hdc = BeginPaint( hWnd, &tPaint );
         SelectObject( hdc, GetStockObject (SYSTEM_FIXED_FONT) );
         TextOut( hdc, 0, 0, szOutLine, zstrlen(szOutLine) );
         EndPaint( hWnd, &tPaint );

         return( 0 );

   }

    return DefWindowProc (hWnd, message, wParam, lParam);
} // MainWndProc

/****************************************************************************

    FUNCTION: RunAppl

    Do "everything"

****************************************************************************/

void RunAppl( )
{
   zVIEW vSubtask = NULL;
   zSHORT nRC;
   char cNet = '\0', cError = '\0' ;
   char cApplication[ 32 ], cTE_Name[ 256 ], cLogFile[ 256 ] ;
   char *pc;

   //HANDLE hInstance = (HANDLE) GetWindowLong( hWnd, GWL_HINSTANCE );
   //LPSTR  lpCmdLine = (LPSTR) GetWindowLong( hWnd, GWL_USERDATA );

   // Analyze the Command Line

   // The command line is
   // "-p Application" "-t TE-Name" "[-n]" ["Log-File"]
   // "Application" and "TE-Name" are  required


// nRC = RegisterZeidonApplication( &vSubtask, (zLONG) hInstance, (zLONG) hWnd,
//                                  WM_USER + 1, "~~Zeidon_Tools~", 0, 0 );
   nRC = ( &vSubtask, 0L, 0L,
                                    WM_USER + 1, "~~Zeidon_Tools~", 0, 0 );


   nRC= InitializeLPLR( vSubtask, "hfifusion" );

   ProcessXods( vSubtask, "hfifusion", "" );

   UnregisterZeidonApplication( vSubtask );
}

static void ProcessXods( zVIEW vSubtask,
                          zCHAR *cTE_Name, zCHAR *cLogFile )RegisterZeidonApplication
{
   FILE *fLog=NULL;
   zVIEW vTaskLPLR = NULL;
   zCHAR szMsg[ 1000 ];
   zVIEW vTZTEDBLO, vDTE, vCM_List;
   zVIEW vLOD, vLOD_List, vXOD;
   zVIEW vTemp = NULL;
   zVIEW vT=NULL;
   zSHORT nRC;
   RECT rect;
   LPLIBRARY hLib = NULL;
   zCHAR szLODName[ 33 ];
   zCHAR szFileName[ 256 ];
   zCHAR szNetwork[ 256 ];
   zCHAR szTemp[zMAX_FILESPEC_LTH + 1];

   if ( cLogFile && cLogFile[0] )
      fLog = fopen( cLogFile, "w");


   ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl",
                        zMULTIPLE );
   SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   //oTZTENVRO_GetUpdViewForDTE_P
   //nRC = RetrieveViewForMetaList( vSubtask, vSubtask, &vCM_List, zSOURCE_DTE_META );
   nRC = RetrieveViewForMetaList(vSubtask, &vCM_List, zSOURCE_DTE_META);
   nRC = ActivateMetaOI( vSubtask, &vDTE, vCM_List, zSOURCE_DTE_META,
                            zSINGLE | zACTIVATE_ROOTONLY );
   if ( nRC < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Internal error activating TE" );
      strcat_s( szMsg, zsizeof( szMsg ), "\n" );
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
      strcpy_s( szMsg, zsizeof( szMsg ), "Invalid TE Name :");
      strcat_s( szMsg, zsizeof( szMsg ), cTE_Name );
      strcat_s( szMsg, zsizeof( szMsg ), "\n" );
      if ( fLog )
      {
         fputs( szMsg,fLog );
         fclose (fLog);
      }
      return;
   }

   nRC = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   assert(nRC >= 0 ) ;

   // Check for network build
   /*
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
   */
	   // Position on corresponding DBMS entry in DB Handler object.
      SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                  vDTE, "TE_DBMS_Source", "DBMS", 0 );

   //}
   // KJS 03/17/26 - This is working with saving all of the xods.
   // We'd like to be able to perform more than just xod. 
   nRC = RebuildMetaLists(vSubtask);
   nRC = RebuildXDM(vSubtask);
   //zwTZTEUPDD_BuildXODsOnLPLR( zVIEW vSubtask )

   // Create a view that lists all LODs.
   RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
   OrderEntityForView( vLOD_List, "W_MetaDef", "Name A" );
   SetNameForView( vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Loop through each of the LOD's
   // WMetaType.Type= zREFER_LOD_META
   //
   for ( nRC = SetCursorFirstEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_List, "W_MetaDef", "" ) )
   {

      GetStringFromAttribute( szLODName, zsizeof( szLODName ),
                              vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Could not Activate LOD: " );
         strcat_s( szMsg, zsizeof( szMsg ), szLODName );
         strcat_s( szMsg, zsizeof( szMsg ), ".\nAborting Build" );


         strcat_s( szMsg, zsizeof( szMsg ), "\n" );
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
         strcpy_s( szMsg, zsizeof( szMsg ), "Building executable for LOD: " );
         strcat_s( szMsg, zsizeof( szMsg ), szLODName );
         strcat_s( szMsg, zsizeof( szMsg ), "." );
         strcpy_s( szOutLine, zsizeof( szOutLine ), szMsg );
         //GetClientRect( hWnd, &rect );
         //InvalidateRect( hWnd, &rect, TRUE );
         //UpdateWindow( hWnd );

         if ( fLog )
         {
           strcat_s( szMsg, zsizeof( szMsg ), "\n" );
           fputs( szMsg,fLog );
         }
         else
            TraceLineS( szMsg, "" );

         // Make sure the TE_SourceZKey attribute is set because it determines
         // what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
		 {
			 // KJS 08/17/21 - Automatically setting the xto1 flag for LODs (if they are saved with a database).
			 //SQL_DBH_SetX_ToOneLoc(vLOD, vSubtask);
			 SetAttributeFromAttribute(vLOD, "POD", "TE_SourceZKey", vDTE, "TE_DBMS_Source", "ZKey");
		 }

         // Build the XOD in memory
		 SetNameForView(vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK);
		 nRC = oTZZOXODO_SaveXOD( vSubtask, vLOD );
		 TraceLineI("oTZZOXODO_SaveXOD ", nRC);
		 GetViewByName(&vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK);
		 // 09/08/15 - We think we do not want to keep the vLOD in cached memory so instead
		 // of doing a DropMetaOI, we are going to do a DropObjectInstance
		 //DropMetaOI( vSubtask, vLOD );
		 DropObjectInstance(vLOD);
		 /*
         // Commit the XOD to LPLR file.
         GetViewByName( &vXOD, "TZZOXODO", 0, zLEVEL_TASK );
         GetStringFromAttribute( szFileName, zsizeof( szFileName ),
                                 vTaskLPLR, "LPLR", "ExecDir" );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, zsizeof( szFileName ), szLODName );
         strcat_s( szFileName, zsizeof( szFileName ), ".XOD" );
         CommitOI_ToFile( vXOD, szFileName, zSINGLE );
		 */

		 // Commit the XOD to LPLR file.
		 GetStringFromAttribute(szTemp, zsizeof(szTemp), vTaskLPLR, "LPLR", "ExecDir");
		 SysConvertEnvironmentString(szFileName, zsizeof(szFileName), szTemp);
		 ofnTZCMWKSO_AppendSlash(szFileName);
		 strcat_s(szFileName, zsizeof(szFileName), szLODName);
		 strcat_s(szFileName, zsizeof(szFileName), ".XOD");
		 TraceLineS("*** Committing workstation file: ", szFileName);
		 // DisplayObjectInstance( vXOD, "", "" );
		 CommitOI_ToFile(vXOD, szFileName, zSINGLE);
		 // 09/08/15 - Adding drop of the xod view.
		 DropObjectInstance(vXOD);

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
