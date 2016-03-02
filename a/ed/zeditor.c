#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "..\tz\ZeidonOp.H"
#include "tz__oprs.h"
#include "tzlodopr.h"
#include "stdlib.h"
#include "exports.h"
#include "cwstart.h"
#include "cwdialog.h"
#include "tzedcwaa.h"
#include "commdlg.h"
#include <dos.h>
#include <stdio.h>

/*
CHANGE LOG

18.04.97 DGC
   Modified SaveAndParse operation to reuse SaveFile operation rather than
   have duplicate code.  This caused SaveAndParse to execute the code
   for backing up the file if it couldn't be saved.  This error was
   occurring at Landesbank and on Windows 95 machines.

18.04.97 DonC
   Modified SaveFile to set meta flag as updated when the C/VML file is
   saved.

03.12.97 GT
   For operations in language C:
   - fill in the needed includes for a new source file
   - correct prototypes for parameters and operations
*/

/* /////////////////////////////////////////////////////////////////////////

   The following functions are declared in TZEDCWZD.C

zSHORT OPERATION AEQ_ClearViews( zVIEW vSubtask );
zSHORT OPERATION AEQ_SelectView( zVIEW vSubtask );
zSHORT OPERATION AEQ_ViewPopup( zVIEW vSubtask );
zSHORT OPERATION AEQ_EntityPopup( zVIEW vSubtask );
zSHORT OPERATION AEQ_AttributePopup( zVIEW vSubtask );

///////////////////////////////////////////////////////////////////////// */

#define SYSFLAG_SHOW_TOOLBOX           SYSFLAG_SHOW_RIBBON
#define SYSFLAG_SHOW_RIBBON_SIDEBAR    SYSFLAG_SHOW_RIBBON

#define TRACE                   TRUE
#define CWDLL_STARTUP_DLL_NAME  "zcwstart.dll"
#define GetWorkView( pvView )   GetViewByName( (pvView), szlTZEDWRKO, 0, \
                                               zLEVEL_TASK )
#define GetProfileView( pv )    GetViewByName( (pv), szlProfileXFER, 0, \
                                               zLEVEL_TASK )
#define GetSubtaskView( pv )    GetViewByName( (pv), szlZeidonEditorSubtask, 0,\
                                               zLEVEL_SYSTEM )

#define MAX_TOKEN_LTH        100

#define PASTE_VIEW_VARIABLE  0x0001
#define PASTE_ENTITY         0x0002
#define PASTE_ATTRIBUTE      0x0004
#define PASTE_WITH_QUOTE     TRUE
#define PASTE_WITHOUT_QUOTE  FALSE

#define SAVEBUFFERS_CANCEL   0
#define SAVEBUFFERS_EXIT     1

typedef struct BufferSettingsRecord BufferSettings;
typedef BufferSettings *LPBUFFERSETTINGS;
struct BufferSettingsRecord
{
   zULONG  lPos;
   DWORD   dwSearchFlags;
   DWORD   dwBufSysFlags;
};

/* global variables used in this module or among more than one module        */
HINSTANCE ghInstance = 0;              // Program instance handle
HWND      ghwndFrame = 0;              // Handle to main window
HWND      ghwndMDIClient = 0;          // Handle to MDI client
HWND      ghwndMsgLine = 0;            // Handle to message line window
BOOL      gbIsCWInitialized = FALSE;   // flag to indicate when CW has been init
char      gszMsgLine[] = "zcwMsgLine"; // Class name for message line window
extern int __argc;
extern char **__argv;
char      sz[ 500 ];                   // Temp char buffer.


HWND   GetHWND_FromCW_Window( HWINDOW hWindow );
long   SYSFUNC CWrightDlgWndProc( HWND, UINT, WPARAM, LPARAM );
int    OperTemplate( zVIEW vSource );
zSHORT fnSetCurrentBuffer( zPVIEW pvEdWrk, zPCHAR szRoutine );
zSHORT fnSetSubwindowPosition( zVIEW  vSubtask, HWND hwnd, zPCHAR szName );
zSHORT fnSaveSubwindowPosition( zVIEW vSubtask, HWND hwnd, zPCHAR szName,
                                zPCHAR pszWindowActive );
zSHORT fnGetSourceMetaForBuffer( zVIEW  vEdWrk, zPVIEW pvSource );

zSHORT OPERATION SaveFile( zVIEW vSubtask );

long DLL
BackupHandler( long evntnum, long datap );

void fnDisplaySysFlags ( void )
{
   TraceLineS( "(zeditor) SysFlags ====================================", "" );
   TraceLineX( "(zeditor) SysFlags = ", SysQFlags( ) );
   if ( SysQFlags( ) & SYSFLAG_WIN_MINIMIZED )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_WIN_MINIMIZED = ", SYSFLAG_WIN_MINIMIZED );
   if ( SysQFlags( ) & SYSFLAG_WIN_MAXIMIZED )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_WIN_MAXIMIZED = ", SYSFLAG_WIN_MAXIMIZED );
   if ( SysQFlags( ) & SYSFLAG_AUTOSAVE_ENABLED )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_AUTOSAVE_ENABLED = ", SYSFLAG_AUTOSAVE_ENABLED );
   if ( SysQFlags( ) & SYSFLAG_STATE_ENABLED )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_STATE_ENABLED = ", SYSFLAG_STATE_ENABLED );
   if ( SysQFlags( ) & SYSFLAG_CMND_LINE_PROMPT )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_CMND_LINE_PROMPT = ", SYSFLAG_CMND_LINE_PROMPT );
   if ( SysQFlags( ) & SYSFLAG_TRAP_SYS_KEYS )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_TRAP_SYS_KEYS = ", SYSFLAG_TRAP_SYS_KEYS );
   if ( SysQFlags( ) & SYSFLAG_STATE_ALL_BUF )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_STATE_ALL_BUF = ", SYSFLAG_STATE_ALL_BUF );
   if ( SysQFlags( ) & SYSFLAG_STATE_ALL_WIN )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_STATE_ALL_WIN = ", SYSFLAG_STATE_ALL_WIN );
   if ( SysQFlags( ) & SYSFLAG_CLIPBOARD )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_CLIPBOARD = ", SYSFLAG_CLIPBOARD );
   if ( SysQFlags( ) & SYSFLAG_NEW_WIN_FOR_BUF )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_NEW_WIN_FOR_BUF = ", SYSFLAG_NEW_WIN_FOR_BUF );
   if ( SysQFlags( ) & SYSFLAG_ONE_BUF_PER_WIN )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_ONE_BUF_PER_WIN = ", SYSFLAG_ONE_BUF_PER_WIN );
   if ( SysQFlags( ) & SYSFLAG_MOUSE_WIN_CREATE )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_MOUSE_WIN_CREATE = ", SYSFLAG_MOUSE_WIN_CREATE );
   if ( SysQFlags( ) & SYSFLAG_RETAIN_DIR )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_RETAIN_DIR = ", SYSFLAG_RETAIN_DIR );
   if ( SysQFlags( ) & SYSFLAG_CONFIG_UPDATE )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_CONFIG_UPDATE = ", SYSFLAG_CONFIG_UPDATE );
   if ( SysQFlags( ) & SYSFLAG_SHOW_RIBBON )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_SHOW_RIBBON = ", SYSFLAG_SHOW_RIBBON );
   if ( SysQFlags( ) & SYSFLAG_IBEAM_CURSOR )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_IBEAM_CURSOR = ", SYSFLAG_IBEAM_CURSOR );
   if ( SysQFlags( ) & SYSFLAG_HIDE_CURSOR )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_HIDE_CURSOR = ", SYSFLAG_HIDE_CURSOR );
   if ( SysQFlags( ) & SYSFLAG_SHOW_FILE_LIST )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_SHOW_FILE_LIST = ", SYSFLAG_SHOW_FILE_LIST );
   if ( SysQFlags( ) & SYSFLAG_SHOW_TOOLBOX )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_SHOW_TOOLBOX = ", SYSFLAG_SHOW_TOOLBOX );
   if ( SysQFlags( ) & SYSFLAG_UNDO_PAST_SAVE )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_UNDO_PAST_SAVE = ", SYSFLAG_UNDO_PAST_SAVE );
   if ( SysQFlags( ) & SYSFLAG_CREATE_DIRS )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_CREATE_DIRS = ", SYSFLAG_CREATE_DIRS );
   if ( SysQFlags( ) & SYSFLAG_HIDE_BUTTON_MSGS )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_HIDE_BUTTON_MSGS = ", SYSFLAG_HIDE_BUTTON_MSGS );
   if ( SysQFlags( ) & SYSFLAG_AUTOSAVE_OVERWRITE )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_AUTOSAVE_OVERWRITE = ", SYSFLAG_AUTOSAVE_OVERWRITE );
   if ( SysQFlags( ) & SYSFLAG_MOUSE_ATTACH_SEL )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_MOUSE_ATTACH_SEL = ", SYSFLAG_MOUSE_ATTACH_SEL );
   if ( SysQFlags( ) & SYSFLAG_SUPPRESS_RELOAD_PROMPT )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_SUPPRESS_RELOAD_PROMPT = ", SYSFLAG_SUPPRESS_RELOAD_PROMPT );
   if ( SysQFlags( ) & SYSFLAG_CUA_TYPE_SELECTIONS )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_CUA_TYPE_SELECTIONS = ", SYSFLAG_CUA_TYPE_SELECTIONS );
   if ( SysQFlags( ) & SYSFLAG_RIBBON_BUTTON_MOVES )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_RIBBON_BUTTON_MOVES = ", SYSFLAG_RIBBON_BUTTON_MOVES );
   if ( SysQFlags( ) & SYSFLAG_RIBBON_MOVE_AUTOSAVE )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_RIBBON_MOVE_AUTOSAVE = ", SYSFLAG_RIBBON_MOVE_AUTOSAVE );
   if ( SysQFlags( ) & SYSFLAG_SHOW_RIBBON_SIDEBAR )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_SHOW_RIBBON_SIDEBAR = ", SYSFLAG_SHOW_RIBBON_SIDEBAR );
   if ( SysQFlags( ) & SYSFLAG_SAVE_ON_FOCUS_LOSS )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_SAVE_ON_FOCUS_LOSS = ", SYSFLAG_SAVE_ON_FOCUS_LOSS );
   if ( SysQFlags( ) & SYSFLAG_NON_DOS_FILENAMES )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_NON_DOS_FILENAMES = ", SYSFLAG_NON_DOS_FILENAMES );
   if ( SysQFlags( ) & SYSFLAG_WINDOWS_KEYSPEED )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_WINDOWS_KEYSPEED = ", SYSFLAG_WINDOWS_KEYSPEED );
   if ( SysQFlags( ) & SYSFLAG_PRELOAD_FILE )
      TraceLineX( "(zeditor) SysFlags SYSFLAG_PRELOAD_FILE = ", SYSFLAG_PRELOAD_FILE );
   TraceLineS( "(zeditor) SysFlags ====================================", "" );
}

LONG FAR PASCAL __export
ZCWMsgLineWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   if ( gbIsCWInitialized )
      return CWrightDlgWndProc( hwnd, msg, wParam, lParam );

   return DefWindowProc( hwnd,msg,wParam,lParam );
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : InitializeApplication ()                                   *
 *                                                                          *
 *  PURPOSE    : Sets up the class data structures and does a one-time      *
 *               initialization of the app by registering the window classes*
 *                                                                          *
 *  RETURNS    : TRUE  - If RegisterClass() was successful for both classes.*
 *               FALSE - otherwise.                                         *
 *                                                                          *
 ****************************************************************************/

BOOL FAR PASCAL
InitializeApplication( void )
{
   WNDCLASS    wc;

   /* Register the message line class */
   wc.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
   wc.lpfnWndProc   = ZCWMsgLineWndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = ghInstance;
   wc.hIcon         = NULL;
   wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
   wc.hbrBackground = (HBRUSH) ( COLOR_BTNFACE + 1 );
   wc.lpszMenuName  = (LPSTR)1;
   wc.lpszClassName = gszMsgLine;

   if ( !RegisterClass( &wc ) )
      return( FALSE );

   return( TRUE );
}

/*

   Hide any subwindows that might be around, show the message, then
   un-hide the subwindows.

*/
zSHORT OPERATION
fnEditorMsg( zPCHAR szMsg,
             zBOOL  nBeep,
             zSHORT nButtons,
             zSHORT nDefaultButton,
             zSHORT nIcon )
{
   zSHORT nRC;
   zVIEW  vEdWrk;

   GetWorkView( &vEdWrk );
   nRC = MessagePrompt( vEdWrk, "ED0001", szlZeidonEditor, szMsg, nBeep,
                        nButtons, nDefaultButton, nIcon );

   return( nRC );
} // fnEditorMsg

int SetUserFlagsOn( DWORD lFlag )
{
   DWORD lCurrentFlag;

   lCurrentFlag = SysQUserFlags ();
   lCurrentFlag |= lFlag;
   SysSetUserFlags( lCurrentFlag );

   return( 0 );
}

int SetUserFlagsOff( DWORD lFlag )
{
   DWORD lCurrentFlag;

   lFlag = ~lFlag;                      // Complement bits.
   lCurrentFlag = SysQUserFlags ();
   lCurrentFlag &= lFlag;
   SysSetUserFlags( lCurrentFlag );

   return( 0 );
}

void ReadOnlyOn( void )
{
   zVIEW vEdWrk = 0;

   fnSetCurrentBuffer( &vEdWrk, "ReadOnlyOn" );
   SetAttributeFromString( vEdWrk, szlBuffer, szlReadOnly, "Y" );
   BufSetReadOnly( TRUE );
}

void ReadOnlyOff( void )
{
   zVIEW vEdWrk = 0;

   fnSetCurrentBuffer( &vEdWrk, "ReadOnlyOff" );
   SetAttributeFromString( vEdWrk, szlBuffer, szlReadOnly, "N" );
   BufSetReadOnly( FALSE );
}

fnSetFocusEditor( void )
{
   SetFocus( WinQHCurrent() );

#if 0
   TraceLineX( "(DGC) GetActiveWindow   = ", (zLONG) GetActiveWindow() );
   TraceLineX( "(DGC) GetFocus          = ", (zLONG) GetFocus() );
   TraceLineX( "(DGC) WinQHCurrent      = ", (zLONG) WinQHCurrent() );
   TraceLineX( "(DGC) WinQCurrentWindow = ", (zLONG) WinQCurrentWindow() );
   TraceLineX( "(DGC) WinQHClient       = ", (zLONG) WinQHClient() );
   TraceLineX( "(DGC) GetHWND_FromCW_Window(...) = ",
               (zLONG) GetHWND_FromCW_Window( WinQCurrentWindow() ) );
#endif

   return( 0 );
} // fnSetFocusEditor

/*

   Called from SetupEditor, this code sets up all the event handlers.

*/
void fnRegisterEvents( zVIEW vSubtask )
{
   LibExport( "int ZEDITOR:_TZ_BufferCreated DWORD DWORD" );
   EventRegister( EVENT_BUFFER_CREATED, EVENT_NORMAL, "_TZ_BufferCreated" );

   LibExport( "int ZEDITOR:_TZ_BufferCurrent DWORD" );
   EventRegister( EVENT_BUFFER_CURRENT, EVENT_NORMAL, "_TZ_BufferCurrent" );

   LibExport( "int ZEDITOR:_TZ_BufferDelete DWORD HBUFFER" );
   EventRegister( EVENT_BUFFER_DELETE, EVENT_NORMAL, "_TZ_BufferDelete" );

   LibExport( "int ZEDITOR:_TZ_BufferSaved DWORD" );
// EventRegister( EVENT_BUFFER_SAVED, EVENT_NORMAL, "_TZ_BufferSaved" );   // ERIC 7/18/96
   EventRegister( EVENT_BUFFER_SAVE_REOPENED, EVENT_NORMAL, "_TZ_BufferSaved" );

   LibExport( "int ZEDITOR:_TZ_SaveBuffer DWORD LPSTR" );
//   LibExport( "int ZEDITOR:_TZ_Test DWORD LPSTR" );
   EventRegister( EVENT_SAVE_BUFFER, EVENT_FIRST, "_TZ_SaveBuffer" );
//   EventRegister( EVENT_READY_TO_WRITE, EVENT_FIRST, "_TZ_Test" );

   // Following was supplied by Premia in an attempt to circumvent the code
   // that saves files.
 LibExport( "long ZEDITOR:BackupHandler long long" );
 // register event and replace builtin event handler
 EventRegister( EVENT_READY_TO_WRITE, EVENT_ONLY, "BackupHandler" );

   // Following called from CWDIALOG.C when opening a file.
   LibExport( "void ZEDITOR:ZeidonCreateBuffer long long LPSTR" );

   // Following are functions called from the toolbox.
   LibExport( "void ZEDITOR:tbInsertZeidonAEQ void" );
   LibExport( "void ZEDITOR:tbVML_Insert void" );
   LibExport( "void ZEDITOR:tbOperInsert void" );
   LibExport( "void ZEDITOR:tbSaveFile void" );
   LibExport( "void ZEDITOR:tbParseSource void" );
   LibExport( "void ZEDITOR:tbGenSource void" );
   LibExport( "void ZEDITOR:tbNextError BOOL" );
}

int
fnZeidonCreateBuffer( zVIEW  vSubtask,
                      zVIEW  vEdWrk,
                      zPCHAR pszFileName )
{
   zVIEW    vProfile;
   zPCHAR   lpszStr;
   zCHAR    szFontString[ 200 ];
   BOOL     bFileExists;
   HBUFFER  hNewBuf;
   HWND     hWndParent;
   HWND     hWnd;
   HWINDOW  hWindow;

   if ( vEdWrk == 0 )
      GetWorkView( &vEdWrk );

   if ( vSubtask == 0 )
      GetViewByName( &vSubtask, szlZeidonEditorSubtask, 0, zLEVEL_SYSTEM );

   GetProfileView( &vProfile );
   bFileExists = FileExists( pszFileName );

   hWndParent = CreateMDI_Child( vSubtask, pszFileName );
   ShowWindow( hWndParent, SW_SHOWMAXIMIZED );
   hWindow = WinCreateChild( hWndParent );

   // Set the newly created window as the current window and set any
   // Window settings.
   WinSetCurrentWindow( hWindow );
   WinSetSysFlags( WinQSysFlags() | WINDOW_HSCROLLBAR | WINDOW_VSCROLLBAR );

   // Setting colors and font styles.
   //
   // Colors and font styles can be set for each of the CW text objects
   // (text, comments, keywords, etc).  The first byte in the Color...()
   // function sets the font style. Values are:
   //
   //    Regular - 0x00   Italic - 0x01   Bold - 0x02
   //
   // The second byte is the color byte.  First nibble in the color byte is
   // backgound, second nibble is foreground.  Values are:
   //
   //    0 - Black         4 - Red        8 - DarkGray      C - LightRed
   //    1 - Blue          5 - Purple     9 - LightBlue     D - LightGray
   //    2 - BlueGreen     6 - Brown      A - LtBlueGreen   E - Yellow
   //    3 - Green         7 - Gray       B - LightGreen    F - White
   //
   // A value of -1 (0xFFFF) leaves color/font as is.  Example--the following
   // sets the color for comments to blue with white background with italics:
   //      ColorComments( 0x01F1 );
   //
   // Note that colors are also set in VML.C.

   ColorKeywords( 0xFC );              // Keywords are red.
   ColorComments( 0xF3 );

   GetStringFromAttribute( szFontString, zsizeof( szFontString ), vProfile, szlED, "FontString" );

   if ( *szFontString == 0 )
   {
      FontSelectWin( 0, 11 );  // Set font to ANSI_FIXED_FONT
   }
   else
   {
      zPCHAR psz;
      zPCHAR psz2;
      zCHAR  facename[ 100 ];
      UINT   hFont, height, charset, family, weight, italic;

      psz = szFontString;
      hFont = (UINT) strtol( psz, &psz, 10 );

      psz = strchr( psz, '\'' ) + 1;
      psz2 = strchr( psz, '\'' );
      *psz2 = 0;
      strcpy_s( facename, zsizeof( facename, psz );
      psz = psz2 + 2;

      if ( hFont )
         FontSelectWin( 0, hFont );
      else
      if ( facename && *facename )
      {
         height = (UINT) strtol( psz, &psz, 10 );
         psz++;

         charset = (UINT) strtol( psz, &psz, 10 );
         psz++;

         family = (UINT) strtol( psz, &psz, 10 );
         psz++;

         weight = (UINT) strtol( psz, &psz, 10 );
         psz++;

         italic = (UINT) strtol( psz, &psz, 10 );

         FontSelectWin( FontCreate( facename, height, 0,
                                    charset, family, italic,
                                    FALSE, weight ), 0 );
      }
   }

   hNewBuf = BufCreate( NIL, pszFileName, FALSE );

   if ( hNewBuf == 0 )
   {
      TraceLineS( "Editor file = ", pszFileName );
      SysMessageBox( "Zeidon Editor", "Couldn't open file", 0 );
      return( zCALL_ERROR );
   }

   hWnd = GetHWND_FromCW_Window( hWindow );
   SetWindowPos( hWnd, HWND_TOP, 0, 0, 0, 0,
                 SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );

   if ( !WinAttachBuf( hWindow, hNewBuf ) )
      SysMessageBox( "Zeidon Editor", "Couldn't attach buffer", 0 );

   // Set the buffer as the current buffer and set any buffer settings.
   // The BufSetCurrentBuffer command should have triggered the BUFFER_CURRENT
   // event.  This means that the current Buffer entity in vEdWrk is for the
   // new buffer.
   BufSetCurrentBuffer( hNewBuf );
   BufSetTabStr( "4 7" );           // Set tabs every three spaces.
   BufSetTabUsage( FALSE );         // Use spaces not the tab character.

   lpszStr = TransformFilename( pszFileName, "%e" );
   if ( zstrcmpi( lpszStr, ".c" ) == 0 )
      SetAttributeFromString( vEdWrk, szlBuffer,
                              szlLanguageType, szlC_File );
   else
   if ( zstrcmpi( lpszStr, ".vml" ) == 0 )
      SetAttributeFromString( vEdWrk, szlBuffer,
                              szlLanguageType, szlVML_File );
   StrFree( lpszStr );

   return( bFileExists );

} // fnZeidonCreateBuffer

int OPERATION
ZeidonCreateBuffer( zVIEW  vSubtask,
                    zVIEW  vEdWrk,
                    zPCHAR pszFileName )
{
   int nRC;

   nRC = fnZeidonCreateBuffer( vSubtask, vEdWrk, pszFileName );
   InvalidateRect( WinQHCurrent( ), 0, FALSE );  // if not working try TRUE

   CWDLLResetDlgWindow();
   CWDLLRedrawDlgWindow();
   CWDLLShowCaret();

   fnSetFocusEditor( );

   return( nRC );

} // ZeidonCreateBuffer

int
InitSession( zVIEW  vSubtask )
{
   zSHORT   nRC;
   char     szFilename[ 200 ];
   zPCHAR   lpszOperName;
   zPCHAR   lpszSourceFileEntityName;
   zPCHAR   pszInvokingTool;
   zCHAR    szOperSrch[ 65 ];
   zLONG    lMetaType;
   zCHAR    szMetaName[ 33 ];
   zLONG    lLth;
   zLONG    l;
   BOOL     bFileExists;
   zVIEW    vEdWrk;
   zVIEW    vSource;
   zVIEW    vTaskLPLR;
   zVIEW    vTempLPLR;
   zVIEW    vProfileXFER;

   GetWorkView( &vEdWrk );
   GetProfileView( &vProfileXFER );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pszInvokingTool, vProfileXFER, szlED, "InvokingTool" );

   // Retrieve the source file name and open a buffer.
   GetViewByName( &vSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
   if ( vSource == 0 )
   {
      SysMessageBox( "Zeidon Editor", "Couldn't find METAOPERATIONS", 0 );
      return( 0 );
   }

   // Clean up a little by dropping the name.
   DropNameForView( vSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );

   //
   // Create the name of the file.
   //

   if ( zstrcmp( pszInvokingTool, "GO" ) == 0 )
      lpszSourceFileEntityName = "GlobalOperationGroup";
   else
   if ( zstrcmp( pszInvokingTool, "DM" ) == 0 )
      lpszSourceFileEntityName = "DomainGroup";
   else
      lpszSourceFileEntityName = szlSourceFile;

   // Get the source dir from the lplr view.
   GetStringFromAttribute( szFilename, zsizeof( szFilename ), vTaskLPLR, szlLPLR, szlPgmSrcDir );
   SysAppendcDirSep( szFilename );

   // Get the subdir from the source file entity.  If GetString... returns < 0
   // then the subdir attribute is null, so nothing was retrieved.  If subdir
   // is set, then we need to add a '\'.
   if ( GetStringFromAttribute( szFilename + zstrlen( szFilename ),
                                vSource, lpszSourceFileEntityName, szlSubdirectory ) >= 0 )
   {
      SysAppendcDirSep( szFilename );
   }

   // Get the file name and extension.
   GetStringFromAttribute( szFilename + zstrlen( szFilename ),
                           vSource, lpszSourceFileEntityName, szlName );
   strcat_s( szFilename, zsizeof( szFileName ), "." );
   GetStringFromAttribute( szFilename + zstrlen( szFilename ),
                           vSource, lpszSourceFileEntityName, szlExtension );

#if TRACE
   TraceLineS( "(tzedcwut) Opening file ", szFilename );
#endif

   SetUserFlagsOn( zUF_OpeningZeidonFile );
   bFileExists = fnZeidonCreateBuffer( vSubtask, vEdWrk, szFilename );

   // The BufEditFile command should have triggered the BUFFER_CURRENT event.
   // This means that the current Buffer entity in vEdWrk is for the file
   // we just opened.

   SetAttributeFromInteger( vEdWrk, szlBuffer, szlSourceViewID, (zLONG) vSource );

   // Set the target executable name from the source meta.
   // Also set the MetaType and MetaName to be used further on.
   if ( zstrcmp( pszInvokingTool, "GO" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "GlobalOperationGroup", "DomainAndGlobalOpGroupName" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_GOPGRP_META );
      lMetaType = zSOURCE_GOPGRP_META;
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vSource, "GlobalOperationGroup", "Name" );
   }
   else
   if ( zstrcmp( pszInvokingTool, "DM" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "DomainGroup", "DomainAndGlobalOpGroupName" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_DOMAINGRP_META );
      lMetaType = zSOURCE_DOMAINGRP_META;
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vSource, "DomainGroup", "Name" );
   }
   else
   if ( zstrcmp( pszInvokingTool, "ZO" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "LOD", "DLL_Name" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType,
                               zREFER_LOD_META );
      lMetaType = zSOURCE_LOD_META;
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ),
                              vSource, "LOD", "Name" );
   }
   else
   if ( zstrcmp( pszInvokingTool, "WD" ) == 0 ||
        zstrcmp( pszInvokingTool, "PN" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, szlSourceFileName,
                                 vSource, szlSourceFile, szlName );

      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "Dialog", "DLL_Name" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_DIALOG_META );

      // If there is no DLL name for the window, then assume the DLL name is
      // the window name.
      if ( CompareAttributeToString( vEdWrk, szlBuffer, "TargetExecutableName",
                                     "" ) == 0 )
      {
         SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                    vSource, "Dialog", "Tag" );
      }

      lMetaType = zSOURCE_DIALOG_META;
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vSource, "Dialog", "Tag" );
   }

   // If the source meta view is not updateable, then make sure that buffer
   // is not updateable.
   // Use a temp view so as not to alter TaskLPLR.
   CreateViewFromViewForTask( &vTempLPLR, vTaskLPLR, vSubtask );
   nRC = SetCursorFirstEntityByInteger( vTempLPLR, "W_MetaType", "Type", lMetaType, 0 );
   if ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByString( vTempLPLR, "W_MetaDef", "Name", szMetaName, 0 );
   }

   // nRC is now < zCURSOR_SET if there was no updatable meta of the type
   // being edited or if the particular meta being edited was not updatable.
   if ( nRC >= zCURSOR_SET )
   {
      // Source view is updateable
      SetAttributeFromString( vEdWrk, szlBuffer, szlActiveStatus, "Y" );
      ReadOnlyOff( );
   }
   else
   {
      // Source view is NOT updateable
      SetAttributeFromString( vEdWrk, szlBuffer, szlActiveStatus, "N" );
      ReadOnlyOn( );
   }

   // Set the LanguageType for the current buffer.
   SetAttributeFromAttribute( vEdWrk, szlBuffer, szlLanguageType,
                              vSource, lpszSourceFileEntityName,
                              szlLanguageType );

   // If the file did not previously exist then init all operations in Meta
   if ( bFileExists == FALSE )
   {
      zVIEW    vTmp;
      char     szDateRaw[ 20 ];
      char     szDateFormatted[ 20 ];
      zPCHAR   pszName;


      MovTopBuf( );
      MovHome( );

      // if source file is of type "C"
      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlC_File ) == 0 )
      {
         // create a "C" source header
         GetAddrForAttribute( &pszName, vSource, lpszSourceFileEntityName, szlName );

         SysGetDateTime( szDateRaw );
         UfFormatDateTime( szDateFormatted, zsizeof( szDateFormatted ), szDateRaw, "DD-MM-YYYY" );

         BufInsertStr(
           "/*\r\n"
           "**----------------------------------------------------------------------\r\n"
           "**\r\n"
           "** .Name:         " );
         BufInsertStr( pszName );
         BufInsertStr( ".c\r\n"
           "**\r\n"
           "** .Version:      1.0\r\n"
           "**\r\n"
           "** .Last change:  " );
         BufInsertStr( szDateFormatted );
         BufInsertStr( "\r\n"
           "**\r\n"
           "** .Description:  Zeidon operations\r\n"
           "**\r\n"
           "**----------------------------------------------------------------------\r\n"
           "** .Change notes\r\n"
           "**\r\n"
           "**  1.0  = New\r\n"
           "**        (" );
         BufInsertStr( szDateFormatted );
         BufInsertStr( ") Zeidon\r\n"
           "**\r\n"
           "**----------------------------------------------------------------------\r\n"
           "*/\r\n\r\n" );

         BufInsertStr(
           "#define KZSYSSVC_INCL\r\n"
           "#include <KZOENGAA.H>\r\n"
           "#include <ZDRVROPR.H>\r\n"
           "#include \"ZEIDONOP.H\"\r\n"
           "\r\n\r\n" );
      }

      CreateViewFromViewForTask( &vTmp, vSource, vSubtask );
      for ( nRC = SetCursorFirstEntity( vTmp, szlOperation, "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTmp, szlOperation, "" ) )
      {
         OperTemplate( vTmp );
         MovEOF();
         MovDown( 2 );

      } // End for each Operation
      DropView( vTmp );
   }

   // Determine if the operation is in the file or we should create a template
   GetAddrForAttribute( &lpszOperName, vSource, szlOperation, szlName );
   strcpy_s( szOperSrch, zsizeof( szOperSrch ), "OPERATION[ \\t\\n]+" );
   strcat_s( szOperSrch, zsizeof( szOperSrch ), lpszOperName );
   strcat_s( szOperSrch, zsizeof( szOperSrch ), "[ \\t\\n\\(]+" );
   MovTopBuf( );
   MovHome( );
   if ( !SrchFind( szOperSrch,
                   SEARCH_FORWARD | SEARCH_REGEX | SEARCH_IGCASE, &lLth ) )
   {
      MovEOF();
      MovEOL();
      BufInsertStr( "\n\n" );
      OperTemplate( vSource );
      MovUp( 3 );
   }
   else
   {
      // Find end of parm list.
      SrchFind( ")", SEARCH_FORWARD, &lLth );

      // Move cursor to beginning of next line.
      MovDown( 1 );
   }

   MovHome ();
   WinScrollH( 99 );
   WinSetSysFlags( WinQSysFlags() | WINDOW_MAXIMIZED );
   CenterLine();

   // Set the search flags.
   if ( GetIntegerFromAttribute( &l, vProfileXFER, szlED, "SearchFlags" ) == 0 )
      SrchSetFlags( l );

   InvalidateRect( WinQHCurrent(), 0, FALSE );   // if not working try TRUE

   CWDLLResetDlgWindow();
   CWDLLRedrawDlgWindow();
   CWDLLShowCaret();

   fnSetFocusEditor( );

   return( 0 );
} // InitSession

/*

   Determines what Zeidon subwindows were active the last time the editor
   was shut down and displays them.

*/
int fnShowSubwindows( zVIEW  vSubtask )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;

   GetProfileView( &vProfileXFER );

   for ( nRC = SetCursorFirstEntity( vProfileXFER, szlSubwindow, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, szlSubwindow, 0 ) )
   {
      zPCHAR pszWindowName;

      if ( CompareAttributeToString( vProfileXFER, szlSubwindow, szlSubwindowActive, "Y" ) != 0 )
         continue;

      GetAddrForAttribute( &pszWindowName, vProfileXFER, szlSubwindow, szlName );
      SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow | zWAB_ProcessImmediateAction,
                               szlTZEDCWZD, pszWindowName );
   }

   return( 0 )

} // fnShowSubwindows

/*=========================================================================

   Command handlers

   The following operations are called from TZEDFRMD.DLL in response to a
   user action on one of the menu options or a Zeidon action.

===========================================================================*/

// SetupEditor is called after the MDI frame has been created.
zSHORT OPERATION
SetupEditor( zVIEW vSubtask )
{
   char    szStartupFileName[ 300 ];
   char    szFileName[ 300 ];
   int     nLth;
   HWND    hwnd;
   zVIEW   vTaskLPLR;
   zVIEW   vProfileXFER;
   zVIEW   vEdWrk;
   zVIEW   vTemp;
   LPSTR   pszCurDir;

   ghInstance = (HINSTANCE) GetApplicationInstance( vSubtask );
   GetViewByName( &vTaskLPLR, "TaskLPLR", 0, zLEVEL_TASK );
   GetProfileView( &vProfileXFER );

   // First thing - create a editor work object instance.
   GetWorkView( &vEdWrk );
   if ( vEdWrk == 0 )
   {
      ActivateEmptyObjectInstance( &vEdWrk, "TZEDWRKO", vTaskLPLR, zSINGLE );

      // Create empty root.
      CreateEntity( vEdWrk, "Editor", zPOS_FIRST );
      SetAttributeFromInteger( vEdWrk, "Editor", "hWndCW", (zLONG) WinQHFrame () );

      SetNameForView( vEdWrk, "TZEDWRKO", 0, zLEVEL_TASK );
   }

   //SysSwapBlocks( 350 );

#if 0
   // Short term hack -- because we've changed the size of the toolbox,
   // delete the Subwindow entity from the profile so that the toolbox gets
   // created with the new size.
   if ( SetCursorFirstEntityByString( vProfileXFER, szlSubwindow, szlName, "Toolbox", 0 ) == zCURSOR_SET )
   {
      if ( CompareAttributeToInteger( vProfileXFER, szlSubwindow, szlHeight, 219 ) > 0 )
      {
         DeleteEntity( vProfileXFER, szlSubwindow, zREPOS_NEXT );
      }
   }
#endif

   // Keep track of the parent subtask view.
   GetViewByName( &vTemp, szlParentToolSubtaskID, 0, zLEVEL_TASK );
   SetAttributeFromInteger( vEdWrk, szlEditor, szlParentToolSubtaskID, (zLONG) vTemp );
   DropNameForView( vTemp, szlParentToolSubtaskID, 0, zLEVEL_TASK );

   // Give the Editor subtask view a name so that it can be retrieved later.
   // We name it at the system level so that other tasks can check to see
   // if the editor is already running.
   SetNameForView( vSubtask, szlZeidonEditorSubtask, 0, zLEVEL_SYSTEM );

   GetWindowHandle( &ghwndFrame, 0, vSubtask, 0 );
   fnSetSubwindowPosition( 0, ghwndFrame, "MainFrame" );

   CWDLLExtraBytes( 0 );
   CWDLLSetArgv( __argv );
   CWDLLSetArgc( __argc );

   CWDLLSetMsgFont( (HFONT)GetStockObject( SYSTEM_FONT ));

   // Startup dll is in the Zeidon Bin dir.
   SysGetLocalDirectory( szStartupFileName );
   SysAppendcDirSep( szStartupFileName );
   strcat( szStartupFileName, CWDLL_STARTUP_DLL_NAME );
   CWDLLSetStartDLLName( szStartupFileName );

   SysGetLocalDirectory( szFileName );

   pszCurDir = SysQCwd( 0 );
   SysSetCwd( szFileName );

   SysAppendcDirSep( szFileName );
   nLth = strlen( szFileName );
   SysSetMapName( "version",  "V1.00.1" );

   LibSetEnvName( "ZCWLIB" );
   SysSetMapName( "cwstart",      "zcwstart" );
   SysSetMapName( "cwstart.dll",  szStartupFileName );
   SysSetMapName( "zcwstart.dll", szStartupFileName );
   SysSetMapName( "zcwstart",     szStartupFileName );

   strcpy( &szFileName[ nLth ],   "zcwdlg.dll" );
   SysSetMapName( "cwdialog",     "zcwdlg" );
   SysSetMapName( "cwdialog.dll",  szFileName );
   SysSetMapName( "zcwdlg.dll",    szFileName );

   strcpy( &szFileName[ nLth ], "zcwcua.dll" );
   SysSetMapName( "cua.dll",      szFileName );

   strcpy( &szFileName[ nLth ], "zeditor.dll" );
   SysSetMapName( "zeditor.dll", szFileName );
   SysSetMapName( "zeditor",     szFileName );

   strcpy( &szFileName[ nLth ], "zeditor.ini" );
   SysSetMapName( "cwright.ini",  szFileName );

   SysSetMapName( "cua",          "zcwcua" );

   strcpy( &szFileName[ nLth ], "QBKeys.dll" );
   SysSetMapName( "QBKeys.dll",  szFileName );

   SysSetVariant( SysQVariant()
      | VARIANT_CHILD
      | VARIANT_CWVC
      | VARIANT_SUBCLASS
      | VARIANT_STANDARD_MDI
      | VARIANT_HOOKKEYS
      | VARIANT_NO_DOCKING
      | VARIANT_SINGLE_MSG_HWND );

   SysSetFlags( SysQFlags()
         | SYSFLAG_IBEAM_CURSOR
         | SYSFLAG_RETAIN_DIR
         | SYSFLAG_TRAP_SYS_KEYS
         | SYSFLAG_ONE_BUF_PER_WIN
         | SYSFLAG_SHOW_TOOLBOX
         | SYSFLAG_WINDOWS_KEYSPEED );

   /* If this is the first instance of the app. register window classes */
   InitializeApplication( );

   /* Since VARIANT_SINGLE_MSG_LINE was specified, these are unused
    * but listed for completeness.
    */
   // CWDLLSetLineHWnd( ghwndMsgLine );
   // CWDLLSetColumnHWnd( ghwndMsgLine );
   // CWDLLSetROHWnd( ghwndMsgLine );
   // CWDLLSetOVRHWnd( ghwndMsgLine );

   CWDLLSetFrame( ghwndFrame );
   ghwndMDIClient = GetMDI_Client( vSubtask );

   CWDLLSetMDIClient( ghwndMDIClient );
   if ( ghwndMsgLine == 0 )
   {
      ghwndMsgLine = CreateWindow( gszMsgLine,
                                   "",
                                   WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                                   0,
                                   0,
                                   0,
                                   0,
                                   ghwndFrame,
                                   NULL,
                                   ghInstance,
                                   NULL);
   }

   CWDLLSetMsgHWnd( ghwndMsgLine );

   CWDLLEntry( TRUE );
   gbIsCWInitialized = TRUE;
   CWDLLRedrawDlgWindow();
   CWDLLEntry( FALSE );

   SysSetCwd( pszCurDir );
   StrFree( pszCurDir );

   ExtDelayedColoring( FALSE );
   BufSetGlobalBackupSpec( "" );   // Turn off backups
   fnRegisterEvents( vSubtask );

#if 0
   // There's some problem with the toolbox code--the toolbox isn't sized
   // right.  The following resizes the toolbox to the size CW thinks it
   // should be.
   hwnd = ToolboxRedraw( );
   if ( !fnSetSubwindowPosition( 0, hwnd, "Toolbox" ) )
      SetWindowPos( hwnd, HWND_TOP, 100, 100, (int) ToolboxQWidth(),
                    (int) ToolboxQHeight(), SWP_SHOWWINDOW );
#endif

   InitSession( vSubtask );
   fnShowSubwindows( vSubtask );
   WinRedraw( 0 );

   return( 0 );

} // SetupEditor


zSHORT OPERATION
Postbuild( zVIEW     vSubtask )
{
   //InitSession( vSubtask );
   return( 0 )
}

zSHORT OPERATION
CleanupEditor( zVIEW     vSubtask )
{
   MSG msg;

#if 1
   if ( ghwndMsgLine )
   {
      DestroyWindow( ghwndMsgLine );
      UnregisterClass( gszMsgLine, ghInstance );
      ghwndMsgLine = 0;
   }

   CWDLLSetExiting();
   CWDLLQueryEndSession( FALSE );
   CWDLLCleanup();

   msg.hwnd = NULL;
   msg.message = WM_CWDLL_EXITING;
   msg.wParam = 0;
   msg.lParam = 0;
   CWDLLProcessMessage( &msg );
   CWDLLCloseAllFiles( );
#else
   CWDLLQueryEndSession( FALSE );
   CWDLLCleanup();

   CWDLLSetExiting();
   msg.hwnd = NULL;
   msg.message = WM_CWDLL_EXITING;
   msg.wParam = 0;
   msg.lParam = 0;
   CWDLLProcessMessage( &msg );
   CWDLLCloseAllFiles( );

   if ( ghwndMsgLine )
   {
      if ( IsWindow( ghwndMsgLine ) )
         DestroyWindow( ghwndMsgLine );

      UnregisterClass( gszMsgLine, ghInstance );
      ghwndMsgLine = 0;
   }
#endif

   return( 0 );
}

/*

   Called during exit processing, this function creates a list of files that
   need to be saved using the "BufferSaveList" entities.

   Returns:
      TRUE  - There are files that need to be saved.
      FALSE - No files need to be saved.

*/
zBOOL OPERATION
BuildSaveBufferList( vSubtask )
{
   zSHORT  nRC;
   HBUFFER hCurrentBuffer;
   zBOOL   bFilesNeedSaving = FALSE;
   zVIEW   vEdWrk;

   // Get the work object and create a new (temporary) view so we can muck
   // around with the cursors.
   GetWorkView( &vEdWrk );
   CreateViewFromViewForTask( &vEdWrk, vEdWrk, vSubtask );

   // Delete temp entities.
   for ( nRC = SetCursorFirstEntity( vEdWrk, szlBufferSaveList, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vEdWrk, szlBufferSaveList, 0 ) )
   {
      DeleteEntity( vEdWrk, szlBufferSaveList, zREPOS_NONE );
   }

   hCurrentBuffer = BufQCurrentBuffer();

   // Copy Buffer entities to Save List.
   for ( nRC = SetCursorFirstEntity( vEdWrk, szlBuffer, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vEdWrk, szlBuffer, 0 ) )
   {
      zLONG hBuffer;

      GetIntegerFromAttribute( &hBuffer, vEdWrk, szlBuffer, szlhBuffer );
      if ( !hBuffer )
         continue;

      // Set the current buffer so that we test if it's been changed.  Note
      // that we use BufSwitchBuffer so that the BUFFER_CURRENT event doesn't
      // get invoked.
      BufSwitchBuffer( (HBUFFER) hBuffer );

      // If the current buffer has been modified then create a SaveList
      // entity.
      if ( BufQModified() && !BufQIsReadOnly() )
      {
         bFilesNeedSaving = TRUE;

         CreateEntity( vEdWrk, szlBufferSaveList, zPOS_LAST );
         SetMatchingAttributesByName( vEdWrk, szlBufferSaveList,
                                      vEdWrk, szlBuffer, zSET_ALL );
      }
   }

   BufSwitchBuffer( hCurrentBuffer );

   DropView( vEdWrk );

   // Get the original work view back and set the cursor to the first buffer.
   GetWorkView( &vEdWrk );
   SetCursorFirstEntity( vEdWrk, szlBufferSaveList, 0 );

   return bFilesNeedSaving;

} // BuildSaveBufferList

void fnSystemClose( vSubtask )
{
   zVIEW  vTemp;
   zVIEW  vEdWrk;
   zVIEW  vProfileXFER;
   zSHORT nRC;

   GetWorkView( &vEdWrk );
   GetProfileView( &vProfileXFER );
   GetSubtaskView( &vTemp );
   DropNameForView( vTemp, szlZeidonEditorSubtask, 0, zLEVEL_SYSTEM );

   //fnSaveSubwindowPosition( 0, ToolboxRedraw(), "Toolbox", "N" );
   fnSaveSubwindowPosition( 0, ghwndFrame, "MainFrame", "N" );

   // Delete the buffer entities.
   for ( nRC = SetCursorFirstEntity( vEdWrk, szlBuffer, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vEdWrk, szlBuffer, 0 ) )
   {
      DeleteEntity( vEdWrk, szlBuffer, zREPOS_NONE );
   }

   SetAttributeFromInteger( vProfileXFER, szlED, "SearchFlags", (zLONG) SrchQFlags() );

} // fnSystemClose

zSHORT OPERATION
SystemClose( zVIEW vSubtask )
{
   if ( UserFlagsOn( zUF_PerformingExit ) )
   {
      SysMessageBox( "DGC", "Received SystemClose again", 1 );
      return( 0 )
   }

   CWDLLCloseAllFiles( );
   SetUserFlagsOn( zUF_PerformingExit );

   if ( BuildSaveBufferList( vSubtask ) )
   {
      // If there are any Save List entities, then a buffer needs to be saved,
      // so show window.
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               szlTZEDCWZD, szlSaveBuffers );
   }
   else
      fnSystemClose( vSubtask );

   return( 0 )

} // SystemClose

zSHORT OPERATION
FileNew( zVIEW     vSubtask )
{
#if 0
   HWND    hWndParent = CreateMDI_Child( vSubtask, "account.c" );
   HWINDOW hWindow = WinCreateChild( hWndParent );
   HBUFFER hBuffer = BufCreate( NIL, "d:\\9b\\a\\tz\\account.c", FALSE );
   HWND    hWnd = GetHWND_FromCW_Window( hWindow );

   if ( hWnd )
   {
      SetWindowPos( hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
   }

   WinAttachBuf( hWindow, hBuffer );
#endif
   HFILE h = 0;
   int   iCtl = 0;
   OFSTRUCT of;
   DWORD l;

   while ( iCtl != 0 )
   {
      switch ( iCtl )
      {
         case 1:
         {
            LPSTR sz = BufQFilename ();

            l = OpenFile( sz, &of, OF_EXIST );
            break;
         }

         case 2:
            if ( h )
               l = _lclose( h );

            break;

         case 3:
         {
            LPSTR sz = BufQFilename ();

            l = OpenFile( sz, &of, OF_REOPEN );
            break;
         }

      }

      iCtl = 0;
   }

   return( 0 );
}

zSHORT OPERATION
ZeidonSearch( zVIEW vSubtask )
{
   LibFunctionExec( "srchCommand NULL IDM_SRCH_SEARCH" );
   return( 0 );
}

zSHORT OPERATION
FileNext( zVIEW vSubtask )
{
   LibFunctionExec( "srchCommand NULL IDM_SRCH_SRCH_AGAIN" );
   return( 0 );
}

zSHORT OPERATION
FileReplace( zVIEW vSubtask )
{
   LibFunctionExec( "srchCommand NULL IDM_SRCH_REPLACE" );
   return( 0 );
}

zSHORT OPERATION
RepaintEditorWindow( zVIEW vSubtask )
{
   HWND hWinCW;

   hWinCW = WinQHCurrent();
   InvalidateRect( hWinCW, 0, FALSE );   // if not working try TRUE
   fnSetFocusEditor( );
   CWDLLResetDlgWindow();
   CWDLLRedrawDlgWindow();
   CWDLLShowCaret();

   return( 0 );
}

zSHORT OPERATION
ZeidonUndo( zVIEW vSubtask )
{
   Undo( );
   RepaintEditorWindow( vSubtask );
   return( 0 );
}

zSHORT OPERATION
ZeidonRedo( zVIEW vSubtask )
{
   Redo( );
   RepaintEditorWindow( vSubtask );
   return( 0 );
}

zSHORT OPERATION
ZeidonEditCut( zVIEW vSubtask )
{
   LibFunctionExec( "BR_cut" );
   RepaintEditorWindow( vSubtask );
   return( 0 );
}

zSHORT OPERATION
ZeidonEditCopy( zVIEW vSubtask )
{
   LibFunctionExec( "BR_copy" );
   RepaintEditorWindow( vSubtask );
   return( 0 );
}
zSHORT OPERATION
ZeidonEditPaste( zVIEW vSubtask )
{
   LibFunctionExec( "BR_CUA_deletion Paste" );
   RepaintEditorWindow( vSubtask );
   return( 0 );
}

zSHORT OPERATION
OpenZeidonFile( zVIEW vSubtask )
{
   OPENFILENAME ofn;
   char         szFile[ 256 ];

   memset( &ofn, 0, zsizeof( OPENFILENAME ) );
   *szFile = 0;

   ofn.lStructSize  = zsizeof( OPENFILENAME );
   ofn.hwndOwner    = 0;
   ofn.hInstance    = 0;
   ofn.lpstrFilter  = "Zeidon Source Files\0*.vml;*.c\0All files\0*.*\0";
   ofn.nFilterIndex = 1;
   ofn.lpstrFile    = szFile;
   ofn.nMaxFile     = zsizeof( szFile );
   ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   if ( !GetOpenFileName( &ofn ) )
   {
      DWORD err;

      err = CommDlgExtendedError( );
      TraceLineI( "(zeditor) Return code from GetOpenFileName = ", err );

      return( 0 )
   }

   ZeidonCreateBuffer( 0, 0, szFile );

   return( 0 );
}

zSHORT OPERATION
CloseZeidonFile( zVIEW vSubtask )
{
   HWINDOW hWindow;
   HBUFFER hBuffer;

   TraceLineS( "(zeditor) CloseZeidonFile - ", BufQFilename () );

   if ( BufQModified () )
   {
      zSHORT nRC;

      sprintf( sz, "File '%s' has been modified.\n"
               "Do you want to save it?", BufQFilename() );
      nRC = MessagePrompt( vSubtask, "ED0020", "Zeidon Editor", sz, TRUE,
                           zBUTTONS_YESNOCANCEL, 0, zICON_EXCLAMATION );
      switch ( nRC )
      {
         case zRESPONSE_CANCEL:
            return( 0 )          // Cancel close

         case zRESPONSE_YES:
            SaveFile( vSubtask );
            break;

         case zRESPONSE_NO:
            break;               // Don't do anything
      }
   }

   hBuffer = BufQCurrentBuffer();
   hWindow = WinQCurrentWindow();

   EditNextBuffer();

   BufDelBuffer( hBuffer );
   WinDelWindow( hWindow );

   TraceLineS( "(zeditor) End CloseZeidonFile - ", BufQFilename () );

   return( 0 );
}

// The user resized an MDI window.  We need to resize the CW buffer window.
zSHORT OPERATION
WindowResize( zVIEW vSubtask )
{
   HWND    hwndBuffer;
   HWND    hwndMDI_Child;
   RECT    rect;

   // Get the hwnd of the MDI child window and it's size.
   //hwndMDI_Child = GetFocus( );
   hwndMDI_Child = GetLastMDI_Child( vSubtask );
   if ( hwndMDI_Child == 0 )
      return( 0 );

   GetClientRect( hwndMDI_Child, &rect);

   // Get the hwnd of the child window.  We assume that there is only one
   // child window for the MDI child window and it must be the buffer window.
   hwndBuffer = GetWindow( hwndMDI_Child, GW_CHILD );
   if ( hwndBuffer )
   {
      // Now resize the buffer window to reflect it's new size.
      SetWindowPos( hwndBuffer, 0, rect.left, rect.top,
                    rect.right - rect.left, rect.bottom - rect.top,
                    SWP_NOZORDER | SWP_NOACTIVATE );

      if ( ghwndMsgLine && gbIsCWInitialized)
      {
         CWDLLResetDlgWindow();
         CWDLLRedrawDlgWindow();
      }
   }

   return( 0 );
}

zSHORT OPERATION
SaveFile( zVIEW vSubtask )
{
   HBUFFER hBuffer;
   zVIEW   vMetaLPLR;

   hBuffer = BufQCurrentBuffer ();
   if ( hBuffer == 0 )
      return( 0 );

   if ( BufQIsSystem( hBuffer ) )
      return( 0 );

   // Save source if it was modified.
   if ( BufQModified () )
   {
      if ( BufWrite () )
      {
         zCHAR  sz[ 10 ];

         SysReadZeidonIni( -1, "[zeditor]", "fix", sz );
         if ( *sz == 'y' || *sz == 'Y' )
         {
            zPCHAR pszFileName = BufQFilename();
            zPCHAR psz         = TransformFilename( pszFileName, "%b" );
            zCHAR  szNewFileName[ 255 ];
            zCHAR  szMsg[ 300 ];

            strcpy_s( szNewFileName, zsizeof( szNewFileName ), psz );
            strcat_s( szNewFileName, zsizeof( szNewFileName ), "._zz" );
            BufWriteFile( szNewFileName, FALSE );
            sprintf( szMsg, "Unable to save buffer to file.  Buffer saved to "
                     "temp file '%s'", szNewFileName );
            MsgNotify( szMsg );
            StrFree( psz );
         }

         return( -1 )
      }
   }

   // Modify the meta in the LPLR list to indicate that it has been updated
   // if the Status for the meta is not 0 (which means the meta is not
   // updateable.
   // To get the view to the correct meta list, we will have to use the
   // following names depending on meta type.
   //   Dialogs          TZDIALOGS
   //   LODs             TZZOLFLO
   //   Domain Groups    CM_ListGroup
   //   Operation Groups CM_ListGroup
   if ( GetViewByName( &vMetaLPLR, "TZDIALOGS", 0, zLEVEL_TASK ) < 0 )
      if ( GetViewByName( &vMetaLPLR, "TZZOLFLO", 0, zLEVEL_TASK ) < 0 )
         if ( GetViewByName( &vMetaLPLR, "CM_ListGroup", 0, zLEVEL_TASK ) < 0 )
            GetViewByName( &vMetaLPLR, "TaskLPLR", 0, zLEVEL_TASK );  // Use default.
   if ( CompareAttributeToInteger( vMetaLPLR, "W_MetaDef", "Status", 1 ) == 0 )
      SetAttributeFromInteger( vMetaLPLR, "W_MetaDef", "UpdateInd", 2 );

   return( 0 )

} // SaveFile

// The user wants to parse the current vml file.  Save it (if it needs it)
// and then parse it.
//
// Returns: 0 - Everything OK
//          1 - Parse errors.
//          zCALL_ERROR - Errors.
//
zSHORT OPERATION
SaveAndParse( zVIEW vSubtask )
{
   zSHORT  nRC = 0;
   zLONG   nParseAction = zPARSE_ACTION_FULL;
   zLONG   nParseLimit  = zPARSE_LIMIT_SOURCE;
   zLONG   nParseSource = zPARSE_SOURCE_FILE;
   zLONG   lTemp;
   zPCHAR  szFilename = 0;
   char    szErrorFile[ 300 ];
   zVIEW   vEdWrk;
   zVIEW   vTempWrk;
   zVIEW   vSource;
   HBUFFER hBuffer;
   HBUFFER hTempBuffer;
   zLONG   lMetaType;

   hBuffer = BufQCurrentBuffer ();
   if ( hBuffer == 0 )
      return( 0 );

   if ( BufQIsSystem( hBuffer ) )
      return( 0 );

   GetWorkView( &vEdWrk );
   if ( !vEdWrk )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at _zfParseFile." );
      fnEditorMsg( sz, 1, 0, 0, zICON_EXCLAMATION );
      return( 0 );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "Parse only valid with .VML files." );
      fnEditorMsg( sz, 1, 0, 0, zICON_EXCLAMATION );
      nRC = 1;
      goto EndOfFunction;
   }

   // Make sure that none of the buffers contains the .ERR file.
   szFilename = TransformFilename( BufQFilename (), "%b" );
   strcpy_s( szErrorFile, zsizeof( szErrorFile ), szFilename );
   strcat_s( szErrorFile, zsizeof( szErrorFile ), ".ERR" );
   StrFree( szFilename );
   szFilename = 0;
   CreateViewFromViewForTask( &vTempWrk, vEdWrk, vSubtask );
   if ( SetCursorFirstEntityByString( vTempWrk, szlBuffer, szlFileName,
                                      szErrorFile, "" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTemp, vTempWrk, szlBuffer, szlhBuffer );
      hTempBuffer = (HBUFFER) lTemp;
      BufSwitchBuffer( hTempBuffer );
      BufDelBuffer( hTempBuffer );
      BufSwitchBuffer( hBuffer );
   }
   DropView( vTempWrk );

   // Get the Source Meta for the current buffer.
   fnGetSourceMetaForBuffer( vEdWrk, &vSource );
   if ( vSource == 0 )
   {
      MessagePrompt( vEdWrk, "ED0106", szlZeidonEditor,
                     "Cannot parse VML that has been opened via File/Open",
                     TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      nRC = 1;
      goto EndOfFunction;
   }

   SetUserFlagsOn( zUF_CurrentlyParsing );

#if 1
   if ( SaveFile( vSubtask ) != 0 )
   {
      nRC = 1;
      goto EndOfFunction;
   }
#else
   // Save source if it was modified.
   if ( BufQModified () )
   {
      if ( BufWrite () )
      {
         MsgNotify( "Unable to save buffer to file." );
         nRC = 1;
         goto EndOfFunction;
      }

      BufClearUndo( );
   }
#endif

   szFilename = TransformFilename( BufQFilename (), "%r" );

   SysWaitStart( );
#if TRACE
   TraceLineS( "(tzedcwut) Parsing file = ", szFilename );
#endif

   GetIntegerFromAttribute( &lMetaType, vEdWrk, szlBuffer, szlMetaType );
   sprintf( sz, "Parsing file '%s.VML'...", szFilename );
   MsgMessage( sz );
   nRC = (zSHORT) ParseSource( vSubtask, vSource,
                               nParseAction, nParseLimit, nParseSource,
                               szFilename, "", lMetaType );
   SysWaitEnd( );

   if ( nRC < 0 )
   {
      sprintf( sz, "Error parsing file '%s.VML'", szFilename );
      MsgMessage( sz );

      // There is a parse error.  If performing exit, then show one message,
      // otherwise show message that allows user to go directly to the
      // error file.
      if ( UserFlagsOn( zUF_PerformingExit ) )
      {
         strcpy_s( sz, zsizeof( sz ), "Parse completed with Errors.\n\n"
                      "See Error File for list of errors." );
         nRC = fnEditorMsg( sz, 1, 0, 0, zICON_ERROR );
      }
      else
      {
#if 0
         ErrorSrcFile( szErrorFile );

         strcpy_s( sz, zsizeof( sz ), "Parse completed with Errors.\n\nDo you want to open the Error File?" );
         nRC = fnEditorMsg( sz, 1, zBUTTONS_YESNO, 0, zICON_ERROR );
         if ( nRC == zRESPONSE_YES )
#endif
            ErrorNext( 1, 0 );
      }

      nRC = 1;
   }
   else
   {
      strcpy_s( sz, zsizeof( sz ), "Parse completed successfully." );
      fnEditorMsg( sz, 0, 0, 0, zICON_INFORMATION );
      nRC = 0;
   }

EndOfFunction:
   if ( szFilename )
      StrFree( szFilename );

   SetUserFlagsOff( zUF_CurrentlyParsing );

   return( nRC );

} // SaveAndParse

zSHORT OPERATION
Generate( zVIEW vSubtask )
{
   zSHORT  nRC;
   zPCHAR  szFilename;
   zVIEW   vEdWrk;
   zVIEW   vSource;
   HBUFFER hBuffer;

   hBuffer = BufQCurrentBuffer ();
   if ( hBuffer == 0 )
      return( 0 );
   if ( BufQIsSystem( hBuffer ) )
      return( 0 );

   GetWorkView( &vEdWrk );
   if ( !vEdWrk )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at _zfGenerateCFile." );
      fnEditorMsg( sz, 1, 0, 0, zICON_EXCLAMATION );
      return( 0 );
   }

   // Get the Source Meta for the current buffer.
   fnGetSourceMetaForBuffer( vEdWrk, &vSource );
   if ( vSource == 0 )
   {
      MessagePrompt( vEdWrk, "ED0107", szlZeidonEditor,
                     "Cannot generate VML that has been opened via File/Open",
                     TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      nRC = 1;
      goto EndOfFunction;
   }

   SetUserFlagsOn( zUF_CurrentlyParsing );

   // Save source if it was modified.
   if ( BufQModified () )
   {
      if ( BufWrite () )
      {
         MsgNotify( "Unable to save buffer to file." );
         nRC = 1;
         goto EndOfFunction;
      }
   }

   szFilename = TransformFilename( BufQFilename (), "%r" );
#if TRACE
   TraceLineS( "(tzedcwut) Generating from file = ", szFilename );
#endif

   SysWaitStart( );
   nRC = Generate3GL( vSubtask, vSubtask, vSource, szFilename );
   SysWaitEnd( );

   StrFree( szFilename );

   if ( nRC < 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "Errors in Generate." );
      strcat_s( sz, zsizeof( sz ), "\n\nSee Messages in Error Box" );
      fnEditorMsg( sz, 1, 0, 0, zICON_ERROR );
      nRC = 1;
   }
   else
   {
      fnEditorMsg( "Generate completed successfully.",
                    0, 0, 0, zICON_INFORMATION );
      nRC = 0;
   }

EndOfFunction:

   SetUserFlagsOff( zUF_CurrentlyParsing );

   return( nRC );

} // Generate
/*

   This operation is called whenever a subwindow returns to the editor.  If
   we are currently involved in exit processing, then we have just returned
   from the SaveBuffers window.  Check to return code from the SaveBuffers
   window and possibly exit the editor.

*/
zSHORT OPERATION
ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW vEdWrk;
   zLONG nRC;

   TraceLineS( "(zeditor) Return From Subwindow", "" );
#if 1
   if ( ghwndMsgLine && gbIsCWInitialized)
   {
      CWDLLResetDlgWindow();
      CWDLLRedrawDlgWindow();
   }
#else

   InvalidateRect( WinQHCurrent(), 0, TRUE );   // if not working try TRUE

   CWDLLResetDlgWindow();
   CWDLLRedrawDlgWindow();
   CWDLLShowCaret();

   fnSetFocusEditor( );
#endif

   if ( !UserFlagsOn( zUF_PerformingExit ) )
      return( 0 )

   GetWorkView( &vEdWrk );
   GetIntegerFromAttribute( &nRC, vEdWrk, szlEditor, szlSaveBuffersReturnCode );

   // If the return code from the SaveBuffers window is SAVEBUFFERS_EXIT,
   // then the user clicked the 'Exit' button, so exit the editor.
   if ( nRC == SAVEBUFFERS_EXIT )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      fnSystemClose( vSubtask );
   }
   else
   {
      // User has cancelled the exit, so reset the flags.
      SetUserFlagsOff( zUF_PerformingExit );
   }

   return( 0 )
}

/*=========================================================================

   Local routines

===========================================================================*/

HWND
GetHWND_FromCW_Window( HWINDOW hWindow )
{
   HWINDOW hCur;
   HWND    hWnd;

   hCur = WinQCurrentWindow();

   WinSwitchWindow( hWindow );
   hWnd = WinQHCurrent();
   WinSwitchWindow( hCur );
   return( hWnd );
}


/*
   Sets the current Buffer entity in vEdWrk to correspond with the current
   CodeWright buffer.

   RETURNS: 1 - Buffer set OK.
            0 - No Buffer entity in vEdWrk.
           -1 - Current buffer is null or system buffer.
*/
zSHORT
fnSetCurrentBuffer( zPVIEW pvEdWrk, zPCHAR szRoutine )
{
   zSHORT  nRC;
   zPCHAR  szFileName;
   zPCHAR  szStr;
   HBUFFER hBuffer;

   hBuffer = BufQCurrentBuffer();

   // Ignore null buffer.
   if ( hBuffer == 0 )
      return( -1 );

   // Ignore system buffers.
   if ( BufQIsSystem( hBuffer ) )
      return( -1 );

   // If it isn't supplied, get the work object view.
   if ( *pvEdWrk == 0 )
   {
      GetWorkView( pvEdWrk );
      if ( !*pvEdWrk )
      {
         char sz[ 300 ];

         strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not found in fnSetCurrentBuffer" );
         strcat_s( sz, zsizeof( sz ), szRoutine );
         fnEditorMsg( sz, 1, 0, 0, zICON_EXCLAMATION );
         return( zCALL_ERROR );
      }
   }

   // Try to find the entity in the work object that matches the current
   // buffer.
   nRC = SetCursorFirstEntityByInteger( *pvEdWrk, szlBuffer, szlhBuffer,
                                        (zLONG) hBuffer, "" );
   if ( nRC == zCURSOR_SET )
      return( 1 );  // Entity set, so return.

   // Setup the Work Object with Buffer data
   szFileName = BufQFilename();
   CreateEntity( *pvEdWrk, szlBuffer, zPOS_LAST );
   SetAttributeFromInteger( *pvEdWrk, szlBuffer, szlhBuffer, hBuffer );
   SetAttributeFromString( *pvEdWrk, szlBuffer, szlFileName, szFileName );

   szStr = TransformFilename( szFileName, "%e" );
   if ( zstrcmpi( szStr, ".c" ) == 0 )
      SetAttributeFromString( *pvEdWrk, szlBuffer,
                              szlLanguageType, szlC_File );
   else
   if ( zstrcmpi( szStr, ".vml" ) == 0 )
      SetAttributeFromString( *pvEdWrk, szlBuffer,
                              szlLanguageType, szlVML_File );
   StrFree( szStr );

   return( 0 );
} // fnSetCurrentBuffer

/*
   Retrieve the Source meta for the current buffer.

*/
zSHORT
fnGetSourceMetaForBuffer( zVIEW  vEdWrk,
                          zPVIEW pvSource )
{
   GetIntegerFromAttribute( (zPLONG) pvSource, vEdWrk, szlBuffer,
                            szlSourceViewID );

   return( 0 )

} // fnGetSourceMetaForBuffer

/*=========================================================================

   Event handlers

===========================================================================*/

/*
   Operation: _TZ_BufferCreated
   Event:     EVENT_BUFFER_CREATED

   The following function is called after a CW buffer is created.
*/
int DLL
_TZ_BufferCreated( DWORD eventnum, DWORD hBuffer )
{
   zVIEW   vEdWrk = 0;
   HBUFFER hCurrentBuffer;

   // Ignore system buffers.
   if ( BufQIsSystem( hBuffer ) )
      return( 0 );

   // Save current buffer and then switch to new buffer.
   hCurrentBuffer = BufQCurrentBuffer ();
   BufSwitchBuffer( hBuffer );

   // Set Buffer entity in vEdWrk to point to current CW buffer.
   fnSetCurrentBuffer ( &vEdWrk, "BufferCreated" );

#if TRACE
   TraceLineS( "(tzedcwut) Event -- BufferCreated.  File name = ",
               BufQFilename () );
#endif

   // Turn on undo.
   if ( !BufQIsReadOnly () )
   {
      BufSetSysFlags( BufQSysFlags () | BUFFER_UNDO );
      BufSetAutoIndentMode( 1 );
   }

   // Reset current buffer.
   BufSwitchBuffer( hCurrentBuffer );

   // Just in case, set Buffer entity in vEdWrk back to original buffer.
   fnSetCurrentBuffer( &vEdWrk, "BufferCreated2" );

   return( 0 );
} // _TZ_BufferCreated

/*
   Operation: _TZ_BufferCurrent
   Event:     EVENT_BUFFER_CURRENT

   The following function is called any time a CW buffer is made current.
*/
int DLL
_TZ_BufferCurrent( DWORD eventnum )
{
   zSHORT  nRC;
   zVIEW   vEdWrk = 0;

#if TRACE
   TraceLineS( "(tzedcwut) Event -- Buffer Current. File name = ",
               BufQFilename () );
#endif

   // Set Buffer entity in vEdWrk to point to current CW buffer.
   nRC = fnSetCurrentBuffer ( &vEdWrk, "BufferCurrent" );
   if ( nRC == -1 )
      return( 0 );    // Current buffer is null or system.

   return( 0 );

} // _TZ_BufferCurrent

/*
   Operation: _TZ_BufferCreated
   Event:     EVENT_BUFFER_CREATED

   The following function is called any time a CW buffer is deleted.
*/
int DLL
_TZ_BufferDelete( DWORD eventnum, HBUFFER hBuffer )
{
   zSHORT  nRC;
   zVIEW   vEdWrk = 0;
   zPCHAR  szFileName;
   HBUFFER hCurrentBuffer = 0;

   hCurrentBuffer = BufQCurrentBuffer();
   if ( hCurrentBuffer != hBuffer )
      BufSwitchBuffer( hBuffer );
   else
      hCurrentBuffer = 0;

   szFileName = BufQFilename ();
#if TRACE
   TraceLineS( "(tzedcwut) Event -- BufferDelete.  Filename = ", szFileName );
#endif

   // Set Buffer entity in vEdWrk to point to current CW buffer.
   nRC = fnSetCurrentBuffer ( &vEdWrk, "BufferDelete" );
   if ( nRC == -1 )
   {
      if ( hCurrentBuffer )
         BufSwitchBuffer( hCurrentBuffer );
      return( 0 );    // Current buffer is null or system.
   }
   else
   if ( nRC == 0 )
   {
      char sz[ 100 ];

      // This should never happen, so flash a message if it does.
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO current buffer not found for Delete!." );
      fnEditorMsg( sz, 1, 0, 0, zICON_EXCLAMATION );
      if ( hCurrentBuffer )
         BufSwitchBuffer( hCurrentBuffer );
      return( 0 );
   }

   DeleteEntity( vEdWrk, szlBuffer, zREPOS_NONE );

   if ( hCurrentBuffer )
      BufSwitchBuffer( hCurrentBuffer );

   return( 0 );
} // _TZ_BufferDelete

/*
   The following function is called before a CW buffer is saved.
*/
int DLL
_TZ_SaveBuffer( DWORD eventnum, LPSTR szFilename )
{
   // Following line is done because argument szFilename is only set when
   // saving buffer to a different file.
   szFilename = BufQFilename ();

#if TRACE
   TraceLineS( "(tzedcwut) Event -- SaveBuffer. File = ", szFilename );
#endif

   BufClearUndo();
   CWDLLCloseAllFiles( );

   return( 0 );

} // _TZ_SaveBuffer

int DLL
_TZ_Test( DWORD eventnum, LPSTR szFilename )
{
   SysMessageBox( "DGC", "About to save buffer", 0 );
   CWDLLCloseAllFiles( );
   CWDLLCloseAllFiles( );
   CWDLLCloseAllFiles( );
   CWDLLCloseAllFiles( );
   CWDLLCloseAllFiles( );

   return( 0 );

} // _TZ_SaveBuffer

/*
   The following function is called after a CW buffer is saved.
*/
int DLL
_TZ_BufferSaved( DWORD eventnum )
{
   zSHORT  nRC;
   zVIEW   vEdWrk = 0;
   zVIEW   vTempLPLR;
   zPCHAR  szFileName;
   zCHAR   szMetaName[33];
   zLONG   lMetaType;

#if TRACE
   TraceLineS( "(tzedcwut) Event -- BufferSaved. File name = ",
               BufQFilename() );
#endif

   BufClearUndo();
   CWDLLCloseAllFiles( );

   // Set Buffer entity in vEdWrk to point to current CW buffer.
   nRC = fnSetCurrentBuffer ( &vEdWrk, "BufferSaved" );
   if ( nRC == -1 )
   {
      return( 0 );    // Current buffer is null or system.
   }

   // Set the W_MetaDef.UpdateInd attribute to make sure that the meta is flagged as
   // updated, to handle the case where it was checked out but not saved.
   // In that way, if the user does not save the meta but does save the C code,
   // then the meta will look as if it has been saved for check in purposes.
   // We will only do this if the meta is not currently flagged for update,
   // which means it doesn't have a zSOURCE_... entry with UpdateInd of 2.
   GetIntegerFromAttribute( &lMetaType,
                            vEdWrk, szlBuffer, szlMetaType );
   RetrieveViewForMetaList( vSubtask, vSubtask, &vTempLPLR, (zSHORT) lMetaType );
   GetStringFromAttribute( szMetaName, zsizeof( szMetaName ),
                           vTempLPLR, "W_MetaDef", "Name" );
   lMetaType = lMetaType - 2000;   // Convert type from REFER to SOURCE.
   nRC = SetCursorFirstEntityByString( vTempLPLR, "W_MetaDef", "Name",
                                       szMetaName, 0 );
   if ( nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToInteger( vTempLPLR, "W_MetaDef", "UpdateInd",
                                      2 ) != 0 )
      {
         // This will be true if the meta does exist for update but has not
         // yet been updated.
         // Force the flag that indicates the meta was updated.
         SetAttributeFromInteger( vTempLPLR, "W_MetaDef", "UpdateInd", 2 );
      }
   }

   DropView( vTempLPLR );

   // If we are performing exit processing, then don't worry about parsing.
   if ( UserFlagsOn( zUF_PerformingExit ) )
      return( 0 )

   // If we are not already in the middle of a Save/Parse procedure, then
   // ask the user if he wants to parse the file being saved if the file is
   // a VML file.
   if ( !UserFlagsOn( zUF_CurrentlyParsing ) &&
        CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                  szlVML_File ) == 0 )
   {
      char sz[ 300 ];

      szFileName = BufQFilename ();
      strcpy_s( sz, zsizeof( sz ), "File " );
      strcat_s( sz, zsizeof( sz ), szFileName );
      strcat_s( sz, zsizeof( sz ), " has been saved. Parse file?" );
      nRC = fnEditorMsg( sz, 1, zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
      if ( nRC == zRESPONSE_YES )
      {
         zVIEW vSubtask;

         GetSubtaskView( &vSubtask );
         SaveAndParse( vSubtask );
      }
   }

   return( 0 );
} // _TZ_BufferSaved

int OperTemplate( zVIEW vSource )
{
   zVIEW    vLOD   = 0;
   zVIEW    vEdWrk = 0;
   LPSTR    szLOD  = 0;
   LPSTR    szOperName;
   LPSTR    szType;
   LPSTR    szReturnDataType;
   LPSTR    szDataType;
   zBOOL    bPointer;
   zBOOL    bUnsigned;
   zBOOL    bFileC;
   zSHORT   nRC;
   zSHORT   nLth;
   zSHORT   nLen;
   zSHORT   nParam;
   zCHAR    szParam[ 65 ];
   zCHAR    szParamCase[ 65 ];
   zCHAR    szLine[ 65 ];

   GetWorkView( &vEdWrk );

   GetAddrForAttribute( &szOperName, vSource, szlOperation, szlName );
   GetAddrForAttribute( &szType, vSource, szlOperation, "Type" );
   GetAddrForAttribute( &szReturnDataType,
                        vSource, szlOperation, "ReturnDataType" );
   bFileC = CompareAttributeToString( vEdWrk, szlBuffer,
                                      szlLanguageType, szlC_File ) == 0;

   if ( szType )
   {
      if ( bFileC )
      {
         switch ( *szReturnDataType )
         {
            case 'V':
               BufInsertStr( "zVIEW " );
               break;
            case 'N':
               BufInsertStr( "zSHORT " );
               break;
            case 'M':
               BufInsertStr( "zDECIMAL " );
               break;
            case 'Y':
               BufInsertStr( "zCHAR " );
               break;
            case 'L':
               BufInsertStr( "zLONG " );
               break;
            case 'O':
               BufInsertStr( "zVOID " );
               break;
            default:
               BufInsertStr( "zPCHAR " );
         }
         BufInsertStr( "/* " );
      }

      switch ( *szType )
      {
           case 'D':
         bFileC ? BufInsertStr( "DIALOG */  OPERATION\r\n" )
                : BufInsertStr( "DIALOG OPERATION\r\n" );
                   break;
           case 'E':

         bFileC ? BufInsertStr( "ENTITY CONSTRAINT */  OPERATION\r\n" )
                : BufInsertStr( "ENTITY CONSTRAINT OPERATION\r\n" );
                   break;
           case 'C':
           case 'O':

         bFileC ? BufInsertStr( "OBJECT CONSTRAINT */  OPERATION\r\n" )
                : BufInsertStr( "OBJECT CONSTRAINT OPERATION\r\n" );
                   break;
           case 'T':
         bFileC ? BufInsertStr( "TRANSFORMATION */  OPERATION\r\n" )
                : BufInsertStr( "TRANSFORMATION OPERATION\r\n" );
                   break;
           case 'A':
         bFileC ? BufInsertStr( "DERIVED ATTRIBUTE */  OPERATION\r\n" )
                : BufInsertStr( "DERIVED ATTRIBUTE OPERATION\r\n" );
                   break;
           case 'M':
         bFileC ? BufInsertStr( "DOMAIN */  OPERATION\r\n" )
                : BufInsertStr( "DOMAIN OPERATION\r\n" );
                   break;
           case 'G':
         bFileC ? BufInsertStr( "GLOBAL */  OPERATION\r\n" )
                : BufInsertStr( "GLOBAL OPERATION\r\n" );
                   break;
           case 'L':
           default:
         bFileC ? BufInsertStr( "LOCAL */  OPERATION\r\n" )
                : BufInsertStr( "LOCAL OPERATION\r\n" );
           }
   }
   else
      bFileC ? BufInsertStr( "LOCAL */  OPERATION\r\n" )
             : BufInsertStr( "LOCAL OPERATION\r\n" );

   nLth = lstrlen( szOperName ) + 2;
   BufInsertStr( szOperName );
   BufInsertStr( "( " );

   // Insert parameter list.
   nParam = 0;
   for ( nRC = SetCursorFirstEntity( vSource, "Parameter", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSource, "Parameter", "" ) )
   {
      if ( nParam > 0 )
      {
         BufInsertStr( ",\r\n" );
         MovRight( nLth );
      }

      nParam++;
      bPointer  = CompareAttributeToString( vSource, "Parameter",
                                            "PFlag", "Y" ) == 0;
      bUnsigned = CompareAttributeToString( vSource, "Parameter",
                                            "UFlag", "Y" ) == 0;
      GetAddrForAttribute( &szDataType, vSource, "Parameter", "DataType" );

      // create prefix for parameter name
      strcpy_s( szParam, zsizeof( szParam ), "");
      if (bFileC)
      {
         if (bPointer)
           strcat_s( szParam, zsizeof( szParam ), "p");

         if (bUnsigned && *szDataType == 'N' || *szDataType == 'L')
           strcat_s( szParam, zsizeof( szParam ), "u");

         switch ( *szDataType )
         {
            case 'V':
               strcat_s( szParam, zsizeof( szParam ), "v");
               break;
            case 'N':
               strcat_s( szParam, zsizeof( szParam ), "n");
               break;
            case 'M':
               strcat_s( szParam, zsizeof( szParam ), "d");
               break;
            case 'Y':
               strcat_s( szParam, zsizeof( szParam ), "c");
               break;
            case 'L':
               strcat_s( szParam, zsizeof( szParam ), "l");
               break;
            case 'O':
               // no prefix for void pointers
               break;
            default:
               strcat_s( szParam, zsizeof( szParam ), "sz");
         }
      }

      // create parameter name
      nLen = (zSHORT) zstrlen( szParam );
      GetStringFromAttribute( szParam + nLen, vSource, "Parameter", "ShortDesc" );
      UfCompressName( szParam + nLen,
         szParam + nLen, 32 - nLen, "", "BL", "", "B_", 1 );

      strcpy_s( szLine, zsizeof( szLine ), "");
      if (bFileC)
      {
         // create prefix for "C"
         strcat_s( szLine, zsizeof( szLine ), "z");

         if (bPointer)
           strcat_s( szLine, zsizeof( szLine ), "P");

         if (bUnsigned)
         {
            switch ( *szDataType )
            {
//               case 'Y':
               case 'N':
               case 'L':
                  strcat_s( szLine, zsizeof( szLine ), "U");
                  break;
            }
         }
      }

      switch ( *szDataType )
      {
         case 'V':
            strcat_s( szLine, zsizeof( szLine ), "VIEW ");
            strcat_s( szLine, zsizeof( szLine ), szParam );

            if ( nParam == 1 &&
                 ( *szType == 'A' || *szType == 'E' || *szType == 'T' ||
                   *szType == 'C' || *szType == 'O' ) )
            {
               zLONG ZKey;

               if ( !szLOD )
               {
                  GetIntegerFromAttribute( &ZKey, vSource, "LOD", szlZKey );
                  nRC = ActivateMetaOI_ByZKey( vSubtask, &vLOD, vSource, zREFER_LOD_META,
                                               zSINGLE | zACTIVATE_ROOTONLY,
                                               ZKey, 0 );
                  if ( nRC < 0 )
                  {
                     fnEditorMsg( "Internal Error -- Couldn't find LOD to go "
                                  "with Source Meta", 1, 0, 0, 0 );
                     szLOD = "error";
                  }
                  else
                     GetAddrForAttribute( &szLOD, vLOD, "LOD", szlName );
               }

               if ( bFileC )
                  strcat_s( szLine, zsizeof( szLine ), " /* " );
               strcat_s( szLine, zsizeof( szLine ), " BASED ON LOD " );
               strcat_s( szLine, zsizeof( szLine ), szLOD );
               if ( bFileC )
                  strcat_s( szLine, zsizeof( szLine ), " */" );
            }
            break;
         case 'N':
            strcat_s( szLine, zsizeof( szLine ), "SHORT ");
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
         case 'M':
            strcat_s( szLine, zsizeof( szLine ), "DECIMAL ");
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
         case 'Y':
            strcat_s( szLine, zsizeof( szLine ), "CHAR ");
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
         case 'L':
            bFileC ? strcat_s( szLine, zsizeof( szLine ), "LONG " )
                   : strcat_s( szLine, zsizeof( szLine ), "INTEGER " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
         case 'O':
            bFileC ? strcat_s( szLine, zsizeof( szLine ), "VOID " )
                   : strcat_s( szLine, zsizeof( szLine ), "VOID " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
         case 'A':
         case 'E':
         default:
            bFileC ? strcat_s( szLine, zsizeof( szLine ), "PCHAR " )
                   : strcat_s( szLine, zsizeof( szLine ), "STRING ( 32 ) " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
      }
      BufInsertStr( szLine );

      // store parameters for CASE in VML function prototype
      switch ( nParam )
      {
         case 1:
            if ( *szType == 'M' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }
            break;
         case 2:
            if ( *szType == 'C' || *szType == 'O' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }
            break;
         case 3:
            if ( *szType == 'E' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }
            break;
         case 4:
            if ( *szType == 'A' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }
            break;
      }

   }  // End FOR EACH Parameter

   if (bFileC)
   {
      // use dummy parameter, to avoid compiler warnings
      if (nParam == 0)
         BufInsertStr( "void" );
      BufInsertStr( " )\r\n" );
   }
   else
   {
      BufInsertStr( " )\r\n\r\n" );
   }

   if ( !bFileC )
   {
      switch ( *szType )
      {
         case 'E':
            MovRight( 3 );
            BufInsertStr( "CASE " );
            BufInsertStr( szParamCase );
            BufInsertStr( "\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zECE_ACCEPT:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zECE_ACCEPT */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zECE_CANCEL:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zECE_CANCEL */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zECE_CREATE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zECE_CREATE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zECE_DELETE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zECE_DELETE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zECE_EXCLUDE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zECE_EXCLUDE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zECE_INCLUDE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zECE_INCLUDE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "END  /* case */\r\n" );
            break;
         case 'C':
         case 'O':
            MovRight( 3 );
            BufInsertStr( "CASE " );
            BufInsertStr( szParamCase );
            BufInsertStr( "\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zOCE_ACTIVATE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zOCE_ACTIVATE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zOCE_ACTIVATE_EMPTY:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zOCE_ACTIVATE_EMPTY */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zOCE_COMMIT:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zOCE_COMMIT */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zOCE_DROPOI:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zOCE_DROPOI */\r\n" );
            MovRight( 3 );
            BufInsertStr( "END  /* case */\r\n" );
            break;
         case 'A':
            MovRight( 3 );
            BufInsertStr( "CASE " );
            BufInsertStr( szParamCase );
            BufInsertStr( "\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDERIVED_GET:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDERIVED_GET */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDERIVED_SET:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDERIVED_SET */\r\n" );
            MovRight( 3 );
            BufInsertStr( "END  /* case */\r\n" );
            break;
         case 'M':
            MovRight( 3 );
            BufInsertStr( "CASE " );
            BufInsertStr( szParamCase );
            BufInsertStr( "\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_SET_ATTRIBUTE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_SET_ATTRIBUTE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_GET_VARIABLE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_GET_VARIABLE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_COMPARE_ATTRIBUTE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_COMPARE_ATTRIBUTE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_GET_FIRST_TBL_ENT_FOR_ATTR:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_GET_FIRST_TBL_ENT_FOR_ATTR */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_GET_NEXT_TBL_ENT_FOR_ATTR:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_GET_NEXT_TBL_ENT_FOR_ATTR */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_SET_ATTRIBUTE_VALUE_NEXT:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_SET_ATTRIBUTE_VALUE_NEXT */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_SET_ATTRIBUTE_VALUE_PREV:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_SET_ATTRIBUTE_VALUE_PREV */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_ADD_TO_ATTRIBUTE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_ADD_TO_ATTRIBUTE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_GET_COMPARE_VALUE:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_GET_COMPARE_VALUE */\r\n" );
            MovRight( 3 );
            BufInsertStr( "OF   zDME_VALIDATE_LPDATA:\r\n\r\n" );
            MovRight( 3 );
            BufInsertStr( "     /* end zDME_VALIDATE_LPDATA */\r\n" );
            MovRight( 3 );
            BufInsertStr( "END  /* case */\r\n" );
            break;
         case 'T':
         case 'G':
         case 'D':
         case 'L':
         default:
            BufInsertStr( "\r\n" );
            break;
      }
   }

   if (bFileC)
   {
      strcpy_s( szLine, zsizeof( szLine ), "{\r\n\r\n\r\n" );
      if ( *szReturnDataType == 'O')
         strcat_s( szLine, zsizeof( szLine ), "\treturn;\r\n");
      else
         strcat_s( szLine, zsizeof( szLine ), "\treturn( 0 );\r\n");
      strcat_s( szLine, zsizeof( szLine ), "} // ");
      strcat_s( szLine, zsizeof( szLine ), szOperName );
      strcat_s( szLine, zsizeof( szLine ), "\r\n" );
   }
   else
   {
      strcpy_s( szLine, zsizeof( szLine ), "\r\nEND\r\n" );
   }
   BufInsertStr( szLine );

   if ( vLOD )
      DropView( vLOD );

   return( 0 );
} // OperTemplate

/***********************************************************************

   Local functions used by dialog operations

***********************************************************************/

/*

   Save the current position of the subwindow in the Profile object.

   If vSubtask is supplied, then the window coordinates are retrieved from
   the window defined by subtask.  If HWND is supplied, then the window
   coordinates are retrieved from the Windows window.

*/
zSHORT fnSaveSubwindowPosition( zVIEW  vSubtask,
                                HWND   hwnd,
                                zPCHAR pszName,
                                zPCHAR pszWindowActive )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;

   GetProfileView( &vProfileXFER );
   nRC = SetCursorFirstEntityByString( vProfileXFER, szlSubwindow, szlName,
                                       pszName, "" );
   if ( nRC != zCURSOR_SET )
   {
      CreateEntity( vProfileXFER, szlSubwindow, zPOS_LAST );
      SetAttributeFromString( vProfileXFER, szlSubwindow, szlName, pszName );
   }

   if ( vSubtask )
   {
      zSHORT nPosX, nPosY;

      GetControlPosition( vSubtask, 0, &nPosX, &nPosY, TRUE );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosX, nPosX );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosY, nPosY );

      GetControlSize( vSubtask, 0, &nPosX, &nPosY, TRUE );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlWidth, nPosX );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlHeight, nPosY );

      SetAttributeFromString( vProfileXFER, szlSubwindow,
                              szlSubwindowActive, pszWindowActive );
   }
   else
   if ( hwnd )
   {
      RECT rect;

      GetWindowRect( hwnd, &rect );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosX, rect.left);
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosY, rect.top );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlWidth,
                               rect.right - rect.left );
      SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlHeight,
                               rect.bottom - rect.top );

      SetAttributeFromString( vProfileXFER, szlSubwindow,
                              szlSubwindowActive, pszWindowActive );
   }

   return( 0 );
}

/*

   Set the position of the subwindow.

   If vSubtask is supplied, then the window coordinates are sent to
   the window defined by subtask.  If HWND is supplied, then the window
   coordinates are sent to the Windows window.

   Return TRUE  if window was repositioned,
          FALSE if window was not.

*/
zSHORT fnSetSubwindowPosition( zVIEW  vSubtask,
                               HWND   hwnd,
                               zPCHAR szName )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;
   zLONG  lPosX, lPosY;

   GetProfileView( &vProfileXFER );
   nRC = SetCursorFirstEntityByString( vProfileXFER, szlSubwindow, szlName,
                                       szName, 0 );
   if ( nRC != zCURSOR_SET )
      return( FALSE );

   if ( vSubtask )
   {
      TraceLineS( "(zeditor) Setting position of window ", szName );

      GetIntegerFromAttribute( &lPosX, vProfileXFER, szlSubwindow, szlPosX );
      GetIntegerFromAttribute( &lPosY, vProfileXFER, szlSubwindow, szlPosY );
      SetControlPosition( vSubtask, 0, (zSHORT) lPosX, (zSHORT) lPosY, TRUE );

      GetIntegerFromAttribute( &lPosX, vProfileXFER, szlSubwindow, szlWidth );
      GetIntegerFromAttribute( &lPosY, vProfileXFER, szlSubwindow, szlHeight );
      SetControlSize( vSubtask, 0, (zSHORT) lPosX, (zSHORT) lPosY, TRUE );
   }
   else
   if ( hwnd )
   {
      zLONG lx;
      zLONG ly;
      zLONG lH;
      zLONG lW;

      GetIntegerFromAttribute( &lx, vProfileXFER, szlSubwindow, szlPosX );
      GetIntegerFromAttribute( &ly, vProfileXFER, szlSubwindow, szlPosY );
      GetIntegerFromAttribute( &lH, vProfileXFER, szlSubwindow, szlHeight );
      GetIntegerFromAttribute( &lW, vProfileXFER, szlSubwindow, szlWidth );
      SetWindowPos( hwnd, HWND_TOP, (int) lx, (int) ly,
                    (int) lW, (int) lH, SWP_SHOWWINDOW );
   }

   return( TRUE );
}

zSHORT fnPasteQualifier( zVIEW  vSubtask,
                         zSHORT nPasteCtl,
                         zBOOL  bInsertWithQuote )
{
   zVIEW  vEdWrk;
   zPCHAR pszEntityName;
   zPCHAR pszAttribName;
   HWND   hWinCW;

   GetWorkView( &vEdWrk );
   if ( !vEdWrk )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at PasteQualifier." );
      OperatorPrompt ( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 ) ;
      return( zCALL_ERROR );
   }

   if ( CheckExistenceOfEntity( vEdWrk, szlView ) != zCURSOR_SET )
      return( zCALL_ERROR );

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "Paste only works for .VML files." );
      OperatorPrompt ( 0, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 ) ;
      return( zCALL_ERROR );
   }

   *sz = 0;

   if ( nPasteCtl & PASTE_VIEW_VARIABLE &&
        CheckExistenceOfEntity( vEdWrk, szlView ) == zCURSOR_SET )
   {
      GetStringFromAttribute( &sz[ zstrlen( sz ) ],
                              vEdWrk, szlView, "VariableName" );
   }

   if ( nPasteCtl & PASTE_ENTITY &&
        CheckExistenceOfEntity( vEdWrk, szlEntity ) == zCURSOR_SET )
   {
      GetAddrForAttribute( &pszEntityName, vEdWrk, szlEntity, szlEntityName );
      if ( pszEntityName )
      {
         if ( *sz )
         {
            if ( bInsertWithQuote )
               strcat_s( sz, zsizeof( sz ), ", " );
            else
               strcat_s( sz, zsizeof( sz ), "." );
         }

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );

         // Get rid of any leading spaces.
         while ( *pszEntityName == ' ' )
            pszEntityName++;
         strcat_s( sz, zsizeof( sz ), pszEntityName );

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );
      }
   }

   if ( nPasteCtl & PASTE_ATTRIBUTE &&
        CheckExistenceOfEntity( vEdWrk, szlAttribute ) == zCURSOR_SET )
   {
      GetAddrForAttribute( &pszAttribName,
                           vEdWrk, szlAttribute, szlAttributeName );
      if ( pszAttribName )
      {
         if ( *sz )
         {
            if ( bInsertWithQuote )
               strcat_s( sz, zsizeof( sz ), ", " );
            else
               strcat_s( sz, zsizeof( sz ), "." );
         }

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );

         strcat_s( sz, zsizeof( sz ), pszAttribName );

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );
      }
   }

   if ( *sz )
   {
      strcat_s( sz, zsizeof( sz ), " " );
      BufInsertStr( sz );

      hWinCW = WinQHCurrent();
      InvalidateRect( hWinCW, 0, FALSE );   // if not working try TRUE

      fnSetFocusEditor( );

      CWDLLResetDlgWindow();
      CWDLLRedrawDlgWindow();
      CWDLLShowCaret();
   }

   return( 0 );

} // fnPasteQualifier

/*
   Get the current line,col position of the cursor and store it as a long.
   Note that the long variable can be compared to other positions stored as
   a long because the line value is stored in the most significant 2 bytes
   of the long variable.  For example:

      ( lPos1 > lPos2 )  is true if the cursor pos at lPos1 comes after lPos2

*/
void fnGetCursorPosition( zPULONG lpPos )
{
   zPUSHORT puCol, puLine;

   puCol  = (zPUSHORT) lpPos;
   puLine = puCol + 1;
   *puCol  = (zUSHORT) BufQCurrentCol ();
   *puLine = (zUSHORT) BufQCurrentLine ();
}

void fnSetCursorPosition( zULONG lPos )
{
   zPUSHORT puCol, puLine;

   puCol  = (zPUSHORT) &lPos;
   puLine = puCol + 1;
   MovAbs( (zLONG) *puLine, (zLONG) *puCol );
}

void fnStartBufferOperation( LPBUFFERSETTINGS lpBufferSettings )
{
   lpBufferSettings->dwSearchFlags = SrchQFlags ();
   lpBufferSettings->dwBufSysFlags = BufQSysFlags ();
   fnGetCursorPosition( &lpBufferSettings->lPos );

   // Turn off undo.
   BufSetSysFlags( BufQSysFlags () & ~BUFFER_UNDO );
   return;
}

void fnEndBufferOperation( LPBUFFERSETTINGS lpBufferSettings )
{
   fnSetCursorPosition( lpBufferSettings->lPos );
   BufSetSysFlags( lpBufferSettings->dwBufSysFlags );
   SrchSetFlags( lpBufferSettings->dwSearchFlags );
   return;
}

/*
   Operation: fnCursorInCommentOrQuote

   Returns:  1 if cursor is in comment.
             2 if cursor is in between quotes.

*/
zSHORT fnCursorInCommentOrQuote( void )
{
   zSHORT nReturn = 0;
   zULONG lPos, lPos1;
   zLONG  lLth;
   BufferSettings BufSettings;

   fnStartBufferOperation( &BufSettings );
   fnGetCursorPosition( &lPos );

   if ( _vml_in_comment( -1L ) >= 0 )
      nReturn = 1;

   // If cursor is not in a comment, check to see if it is in quotes.
   if ( nReturn == 0 )
   {
      zBOOL bOpenQuote;

      // Check to see if the original cursor position is inside quotes.  To
      // do this, we first go to the beginning of the line and then start
      // counting quotes until we get to the original cursor position.
      // Once we've done this, if the number of quotes is odd, then the
      // cursor is in quotes, otherwise it isn't.  This assumes that quoted
      // strings must be contained on one line e.g. they cannot be continued
      // on to multiple lines.

      // Reset cursor position to original position and then move to the
      // beginning of the line.
      fnSetCursorPosition( lPos );
      MovHome ();

      // Set bOpenQuote to false.  bOpenQuote is an indicator that
      // specifies whether the last quote found is an open quote.
      bOpenQuote = FALSE;
      while ( SrchFind( "\"", SEARCH_FORWARD, &lLth ) )
      {
         // If quote is before the original cursor position, then toggle
         // bOpenQuote, otherwise break out of loop.
         fnGetCursorPosition( &lPos1 );
         if ( lPos1 <= lPos )
            bOpenQuote = !bOpenQuote;
         else
            break;
         MovRight( 1 );
      }

      if ( bOpenQuote )
         nReturn = 2;
   }

   fnEndBufferOperation( &BufSettings );
   return( nReturn );
} // fnCursorInCommentOrQuote

/*
   OPERATION:  fnGetNextTokenFromBuffer

   PURPOSE:    Reads the current line in the current buffer starting at
               the current cursor position and returns the first token
               found.
*/
zSHORT fnGetNextTokenFromBuffer( zPCHAR szToken )
{
   zPCHAR szStr;
   zPCHAR szStartPtr;
   zPCHAR szEndPtr;

   // Read line.
   szStr = BufReadStr( MAX_TOKEN_LTH );

   // Skip spaces.
   szStartPtr = szStr;
   while ( *szStartPtr == ' ' )
      szStartPtr++;

   // Look for 1st terminator.
   szEndPtr = szStartPtr;
   while ( szEndPtr != 0 )
   {
      switch ( *szEndPtr )
      {
         case '\0':
         case '\n':
         case '\t':
         case ',' :
         case ')' :
         case '(' :
         case ' ' : *szEndPtr = 0;  // Terminate token with null string.
                    szEndPtr = 0;   // Indicate that token has been found.
                    break;

         default  : szEndPtr++;
                    break;
      }

      if ( szEndPtr > 0 && (szEndPtr - szStr) >= MAX_TOKEN_LTH )
      {
         OperatorSend( 0, szlZeidonEditor, "Max token length exceeded!", 1 );
         *szEndPtr = 0;
         szEndPtr = 0;
      }
   }

   strcpy_s( szToken, MAX_TOKEN_LTH, szStartPtr );
   StrFree( szStr );

   return( 0 );
}
zSHORT PasteOperation( zVIEW vSubtask, zVIEW vOp )
{
   zSHORT   nRC;
   zVIEW    vEdWrk;
   zPCHAR   szStr;
   zBOOL    bVML_File;
   char     szTemp[ 1000 ];
   char     szIndentStr[ 100 ];
   zSHORT   nIndent;
 HWND     hWinCW;

   if ( BufQCurrentBuffer == 0 )
   {
      OperatorSend( vSubtask, szlZeidonEditor, "No current buffer.", 1 );
      return( zCALL_ERROR );
   }

   GetWorkView( &vEdWrk );
   bVML_File = ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                           szlVML_File ) == 0 );

   GetAddrForAttribute( &szStr, vOp, szlOperation, szlName );
   strcpy_s( szTemp, zsizeof( szTemp ), szStr );
   strcat_s( szTemp, zsizeof( szTemp ), "( " );

   // Create Indent string.
   nIndent = zstrlen( szTemp ) + (zSHORT) BufQCurrentCol() - 1;
   if ( nIndent > 0 )
      zmemset( szIndentStr, ' ', nIndent );
   szIndentStr[ nIndent ] = 0;

   nRC = SetCursorFirstEntity( vOp, "Parameter", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &szStr, vOp, "Parameter", "PFlag" );
      if ( !bVML_File && *szStr == 'Y' )
         strcat_s( szTemp, zsizeof( szTemp ), "PTR " );

      GetAddrForAttribute( &szStr, vOp, "Parameter", "UFlag" );
      if ( !bVML_File && *szStr == 'Y' )
         strcat_s( szTemp, zsizeof( szTemp ), "UNSIGNED " );

      GetAddrForAttribute( &szStr, vOp, "Parameter", "DataType" );
      if ( *szStr )
      {
         if ( bVML_File )
         {
            switch ( *szStr )
            {
            case 'A': strcat_s( szTemp, zsizeof( szTemp ), "\"ATTRIBUTE_NAME\"" );
                        break;
            case 'E': strcat_s( szTemp, zsizeof( szTemp ), "\"ENTITY_NAME\"" );
                        break;
            case 'L': strcat_s( szTemp, zsizeof( szTemp ), "INTEGER" );
                        break;
            case 'M': strcat_s( szTemp, zsizeof( szTemp ), "DOUBLE" );
                        break;
            case 'N': strcat_s( szTemp, zsizeof( szTemp ), "INTEGER" );
                        break;
            case 'O': strcat_s( szTemp, zsizeof( szTemp ), "VOID" );
                        break;
            case 'S': strcat_s( szTemp, zsizeof( szTemp ), "STRING" );
                        break;
            case 'V': strcat_s( szTemp, zsizeof( szTemp ), "VIEW" );
                        break;
            case 'Y': strcat_s( szTemp, zsizeof( szTemp ), "STRING" );
                        break;
            default : strcat_s( szTemp, zsizeof( szTemp ), szStr );
                        break;
            } // switch
         }
         else
         {
            switch ( *szStr )
            {
            case 'A': strcat_s( szTemp, zsizeof( szTemp ), "\"ATTRIBUTE_NAME\"" );
                        break;
            case 'E': strcat_s( szTemp, zsizeof( szTemp ), "\"ENTITY_NAME\"" );
                        break;
            case 'L': strcat_s( szTemp, zsizeof( szTemp ), "zLONG" );
                        break;
            case 'M': strcat_s( szTemp, zsizeof( szTemp ), "DOUBLE" );
                        break;
            case 'N': strcat_s( szTemp, zsizeof( szTemp ), "zSHORT" );
                        break;
            case 'O': strcat_s( szTemp, zsizeof( szTemp ), "void" );
                        break;
            case 'S': strcat_s( szTemp, zsizeof( szTemp ), "zCHAR" );
                        break;
            case 'V': strcat_s( szTemp, zsizeof( szTemp ), "zVIEW" );
                        break;
            case 'Y': strcat_s( szTemp, zsizeof( szTemp ), "zCHAR" );
                        break;
            default : strcat_s( szTemp, zsizeof( szTemp ), szStr );
                        break;
            } // switch
         } // if ( bVML_File )...else...
      }

      GetAddrForAttribute( &szStr, vOp, "Parameter", "ShortDesc" );
      if ( *szStr )
      {
         strcat_s( szTemp, zsizeof( szTemp ), " /* " );
         strcat_s( szTemp, zsizeof( szTemp ), szStr );
         strcat_s( szTemp, zsizeof( szTemp ), " */" );
      }

      nRC = SetCursorNextEntity( vOp, "Parameter", "" );
      if ( nRC == zCURSOR_SET )
      {
         strcat_s( szTemp, zsizeof( szTemp ), ",\n" );
         strcat_s( szTemp, zsizeof( szTemp ), szIndentStr );
      }
      else
         strcat_s( szTemp, zsizeof( szTemp ), " " );
   }

   strcat_s( szTemp, zsizeof( szTemp ), ")" );
   if ( !bVML_File )
      strcat_s( szTemp, zsizeof( szTemp ), ";" );

   BufInsertStr( szTemp );

 hWinCW = WinQHCurrent();
 InvalidateRect( hWinCW, 0, FALSE );   // if not working try TRUE
   fnSetFocusEditor( );

   return( 0 );
} // PasteOperation

/***********************************************************************

   TZEDCWZD dialog operations:

   The following operations are called from the TZEDCWZD dialog.

***********************************************************************/


zSHORT OPERATION
zwTZEDCWZD_SetFocusEditor( zVIEW vSubtask )
{
   fnSetFocusEditor( );
   return( 0 );
}

zSHORT OPERATION
AEQ_RefreshAEQ( zVIEW vSubtask )
{
   RefreshControl( vSubtask, "ViewList" );
   RefreshControl( vSubtask, "EntityList" );
   RefreshControl( vSubtask, "AttributeList" );

   return( 0 );
}

zSHORT OPERATION
AEQ_Postbuild( zVIEW vSubtask )
{
   fnSetSubwindowPosition( vSubtask, 0, szlObjectSelAQ );
   return( 0 );
}

zSHORT OPERATION
AEQ_Close( zVIEW vSubtask )
{
   if ( UserFlagsOn( zUF_PerformingExit ) )
      fnSaveSubwindowPosition( vSubtask, 0, szlObjectSelAQ, "Y" );
   else
      fnSaveSubwindowPosition( vSubtask, 0, szlObjectSelAQ, "N" );

   return( 0 );
}

zSHORT OPERATION
AEQ_GetViews( zVIEW vSubtask )
{
   zSHORT nRC;
   zSHORT nViewType;
   zVIEW  vEdWrk;
   zVIEW  vVor;
   zLONG  lLth;
   zLONG  lLastLine, lFirstLine;
   zLONG  lLineAfterOD, lColAfterOD;
   char   szObjectName[ MAX_TOKEN_LTH ];
   char   szVarName[ MAX_TOKEN_LTH ];
   zPCHAR szSearchString;

   BufferSettings BufSettings;
   static char    szOperationString[] = "[ \\t\\n]+OPERATION[ \\t\\n]+";

   GetWorkView( &vEdWrk );
   if ( !vEdWrk )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at GetViews." );
      OperatorPrompt ( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 ) ;
      return( zCALL_ERROR );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "'Get Views' only works on .VML files." );
      OperatorPrompt ( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 ) ;
      return( zCALL_ERROR );
   }

   // Save current cursor position in buffer.
   fnStartBufferOperation( &BufSettings );

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.
   do
   {
      if ( !SrchFind( szOperationString,
                      SEARCH_IGCASE | SEARCH_FORWARD | SEARCH_REGEX, &lLth ) )
      {
         // Operation not found, so set lLastLine to some ridiculously high
         //number (this number indicates the last line of the current op)
         // and stop searching
         lLastLine = 99999999;
         break;
      }
      else
         // Save current position as the last line of the operation.
         lLastLine = BufQCurrentLine( ) - 1;

      MovRight( 1 );
   } while ( fnCursorInCommentOrQuote( ) );

   // Search back for beginning of current operation.
   do
   {
      SrchSetFwd( FALSE );          // Set search to look backward.
      if ( !SrchFind( szOperationString, SEARCH_REGEX | SEARCH_IGCASE, &lLth ) )
      {
         strcpy_s( sz, zsizeof( sz ), "Couldn't find beginning of operation" );
         OperatorPrompt( 0, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 ) ;
         return( zCALL_ERROR );
      }

   } while ( fnCursorInCommentOrQuote( ) );

   // Save line # of beginning of current operation.
   lFirstLine = BufQCurrentLine( );

   // Search for 'BASED ON LOD' and 'REGISTERED AS' and retrieve the object
   // name and view variable name.
   for ( nViewType = 0; nViewType < 2; nViewType++ )
   {
      if ( nViewType == 0 )
         szSearchString = "[ ]+BASED[ ]+ON[ ]+LOD[ \\t]+\\c";
      else
      if ( nViewType == 1 )
         szSearchString = "[ ]+REGISTERED[ ]+AS[ \\t]+\\c";

      // Go to first line of current operation.
      MovAbs( lFirstLine, 1 );

      // Search for view declaration and retrieve object names.
      SrchSetFwd( TRUE );          // Set search to look forward.
      while ( SrchFind( szSearchString,
                        SEARCH_IGCASE | SEARCH_FORWARD | SEARCH_REGEX,
                        &lLth ) )
      {
         TraceLineI( "(zeditor) SrchFind length = ", lLth );
         // Stop searching once we have passed the last line of the current
         // operation.
         if ( BufQCurrentLine( ) > lLastLine )
            break;

         // If current string is commented or quoted, continue.
         if ( fnCursorInCommentOrQuote( ) )
            continue;

         fnGetNextTokenFromBuffer( szObjectName );

         // Get line/col for first char after Object name.
         lColAfterOD  = BufQCurrentCol( );
         lLineAfterOD = BufQCurrentLine( );

         // Find variable name.
         do
         {
            SrchSetFwd( FALSE );          // Set search to look backward.
            SrchFind( "[ \\t\\n]+VIEW[ \\t]+\\c",
                     SEARCH_IGCASE | SEARCH_REGEX, &lLth );
            TraceLineI( "(zeditor) SrchFind length = ", lLth );

         } while ( fnCursorInCommentOrQuote( ) );

         fnGetNextTokenFromBuffer( szVarName );

         // Move back to position after Object name.
         MovAbs( lLineAfterOD, lColAfterOD );

         // If we are searching for registered views, then szObjectName is
         // really the VOR name.  Get the object name.
         if ( nViewType == 1 )
         {
            // Add view variable and Object name to editor work object.
            // First, get the VOR by name.
            nRC = ActivateMetaOI_ByName( &vVor, vSubtask, zREFER_VOR_META,
                                         zSINGLE | zLEVEL_APPLICATION,
                                         szObjectName, 0 );
            if ( nRC < 0 )
            {
               strcpy_s( sz, zsizeof( sz ), "Can't find Registered View with name " );
               strcat_s( sz, zsizeof( sz ), szObjectName );
               OperatorPrompt ( 0, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 );
               continue;
            }

            // Get Object name from VOR.
            GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vVor, "LOD", szlName );
            DropMetaOI( vSubtask, vVor );
         }

         // Add view variable and Object name to editor work object.

         // If entity with szVarName does NOT exist, create it.
         // If the variable name is the same but the object name is different,
         // delete the old View and replace it with a new View.  If variable
         // name and object name are the same, then don't do anything.
         nRC = SetCursorFirstEntityByString( vEdWrk, szlView, szlVariableName,
                                             szVarName, "" );
         if ( nRC != zCURSOR_SET )
         {
            CreateEntity( vEdWrk, szlView, zPOS_LAST );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlObjectName, szObjectName );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlVariableName, szVarName );
         }
         else
         if ( CompareAttributeToString( vEdWrk, szlView, szlObjectName,
                                        szObjectName ) != 0 )
         {
            // We must do a delete to get rid of all Entity/Attribute entities.
            DeleteEntity( vEdWrk, szlView, zREPOS_FIRST );

            CreateEntity( vEdWrk, szlView, zPOS_LAST );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlObjectName, szObjectName );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlVariableName, szVarName );
         }
      } // while ( SrchFind... )

   } //  for ( nViewType = 0; nViewType < 2; nViewType++ )...

   fnEndBufferOperation( &BufSettings );

   AEQ_RefreshAEQ( vSubtask );

   return( 0 );
} // AEQ_GetViews

zSHORT OPERATION
AEQ_PasteView( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE,
                     PASTE_WITHOUT_QUOTE );
   return( 0 )
}

zSHORT OPERATION
AEQ_PasteViewEntity( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY,
                     PASTE_WITHOUT_QUOTE );
   return( 0 )
}

zSHORT OPERATION
AEQ_PasteViewEntity_Quoted( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY,
                     PASTE_WITH_QUOTE );
   return( 0 )
}

zSHORT OPERATION
AEQ_PasteViewEntityAttr( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY | PASTE_ATTRIBUTE,
                     PASTE_WITHOUT_QUOTE );
   return( 0 )
}

zSHORT OPERATION
AEQ_PasteViewEntityAttr_Quoted( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY | PASTE_ATTRIBUTE,
                     PASTE_WITH_QUOTE );
   return( 0 )
}

zSHORT OPERATION
SaveBuffers_Cancel( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   GetWorkView( &vEdWrk );
   SetAttributeFromInteger( vEdWrk, szlEditor, szlSaveBuffersReturnCode,
                            SAVEBUFFERS_CANCEL );
   return( 0 )
}

zSHORT OPERATION
SaveBuffers_Exit( zVIEW vSubtask )
{
   zVIEW   vEdWrk;
   HBUFFER hCurrentBuffer;
   HBUFFER hBuffer;

   GetWorkView( &vEdWrk );
   SetAttributeFromInteger( vEdWrk, szlEditor, szlSaveBuffersReturnCode,
                            SAVEBUFFERS_EXIT );

   // We are exiting the editor.  Turn off the 'modified' flag for any
   // un-saved buffers so that CW doesn't ask if they should be saved.
   hCurrentBuffer = BufQCurrentBuffer();
   hBuffer = hCurrentBuffer;

   // Loop for each buffer.
   do
   {
      // Unconditionally turn off modified flag...
      BufSetModified( FALSE );

      hBuffer = BufQNext( FALSE );        // Get next buffer.
      BufSwitchBuffer( hBuffer );         // Switch to it.

   } while ( hBuffer != hCurrentBuffer );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent |
                            zWAB_ProcessImmediateAction, 0, 0 );
   return( 0 )
}

zSHORT OPERATION
SaveBuffers_SaveFile( zVIEW vSubtask )
{
   zVIEW   vEdWrk;
   zPCHAR  pszFileName;

   GetWorkView( &vEdWrk );

   // Use BufEditFile to open the current buffer and window.
   GetAddrForAttribute( &pszFileName, vEdWrk, szlBufferSaveList, szlFileName );
   BufEditFile( pszFileName );

   SaveFile( vSubtask );

   // Rebuild the list of buffers that need to be saved.  If there are no
   // more buffers that need to be saved, then set the action behavior
   // of the Save Buffers window to return.
   if ( !BuildSaveBufferList( vSubtask ) )
   {
      SaveBuffers_Exit( vSubtask );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent |
                               zWAB_ProcessImmediateAction, 0, 0 );
   }
   else
      SetCursorFirstEntity( vEdWrk, szlBufferSaveList, 0 );

   return( 0 )

} // SaveBuffers_SaveFile

zSHORT OPERATION
SaveBuffers_SaveAll( zVIEW vSubtask )
{
   zVIEW   vEdWrk;
   HBUFFER hCurrentBuffer;
   zSHORT  nRC;

   GetWorkView( &vEdWrk );
   CreateViewFromViewForTask( &vEdWrk, vEdWrk, vSubtask );

   hCurrentBuffer = BufQCurrentBuffer();

   for ( nRC = SetCursorFirstEntity( vEdWrk, szlBufferSaveList, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vEdWrk, szlBufferSaveList, 0 ) )
   {
      zLONG hBuffer;

      GetIntegerFromAttribute( &hBuffer, vEdWrk, szlBufferSaveList, szlhBuffer );
      BufSetCurrentBuffer( (HBUFFER) hBuffer );

      SaveFile( vSubtask );
   }

   BufSetCurrentBuffer( hCurrentBuffer );

   DropView( vEdWrk );
   return( 0 )
}

zSHORT OPERATION
SaveBuffers_ParseAll( zVIEW vSubtask )
{
   zVIEW   vEdWrk;
   HBUFFER hCurrentBuffer;
   zSHORT  nRC;

   GetWorkView( &vEdWrk );
   CreateViewFromViewForTask( &vEdWrk, vEdWrk, vSubtask );

   hCurrentBuffer = BufQCurrentBuffer();

   for ( nRC = SetCursorFirstEntity( vEdWrk, szlBufferSaveList, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vEdWrk, szlBufferSaveList, 0 ) )
   {
      zLONG hBuffer;
      char  szMsg[ 200 ];

      GetIntegerFromAttribute( &hBuffer, vEdWrk, szlBufferSaveList, szlhBuffer );
      BufSetCurrentBuffer( (HBUFFER) hBuffer );

      strcpy_s( szMsg, zsizeof( szMsg ), "Parsing: " );
      GetStringFromAttribute( &szMsg[ zstrlen( szMsg ) ],
                              vEdWrk, szlBufferSaveList, szlFileName );
      MB_SetMessage( vSubtask, 0, szMsg );

      SaveAndParse( vSubtask );
   }

   BufSetCurrentBuffer( hCurrentBuffer );

   DropView( vEdWrk );
   return( 0 )
}

zSHORT OPERATION
zSetFont( zVIEW vSubtask )
{
   zVIEW  vProfile;
   zCHAR  buf[ 500 ];
   zPCHAR facename;
   LOGFONT  lf;
   UINT height, charset, family, weight, italic;
   zLONG lPointSize;

   if ( OperatorPromptForFont( vSubtask, &lf, &lPointSize, 0, FALSE ) == 1 )
   {
      facename = lf.lfFaceName;
      height   = lf.lfHeight;
      charset  = lf.lfCharSet;
      family   = lf.lfPitchAndFamily;
      italic   = lf.lfItalic;
  weight   = lf.lfWeight;

      FontSelectWin( FontCreate( facename, height, 0,
                                 charset, family, italic,
                                 FALSE, weight ), 0 );

      wsprintf( buf, "%u,'%s',%u,%u,%u,%u,%u",
         (UINT) ( (int) FontQCurrent() <= 16 ? FontQCurrent() : 0 ),
         (LPSTR) ( facename ? facename : (LPSTR) "" ),
         (UINT) height, (UINT) charset, (UINT) family,
         (UINT) weight, (UINT) italic );

      GetProfileView( &vProfile );
      SetAttributeFromString( vProfile, szlED, "FontString", buf );
   }

   return( 0 )
}

zSHORT OPERATION
zPrint( zVIEW vSubtask )
{
   DlgPrint( 0 );

   return( 0 )
}

zSHORT OPERATION
zPrinterSetup( zVIEW vSubtask )
{
   DlgPrintSetup( 0 );

   return( 0 )
}

zSHORT OPERATION
OBJ_PasteObjectName( zVIEW vSubtask )
{
   zVIEW  vList;
   zVIEW  vProfile;
   zPCHAR szOption;
   zPCHAR szName;
   HWND   hWinCW;

   GetViewByName( &vList, szlTZCMLPLO_List, 0, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vList, szlW_MetaDef ) != zCURSOR_SET )
      return( 0 );

   GetProfileView( &vProfile );
   GetAddrForAttribute( &szOption, vProfile, szlED, szlPasteObjectOption );
   GetAddrForAttribute( &szName, vList, szlW_MetaDef, szlName );

   switch ( *szOption )
   {
      case 'N':
         BufInsertStr( szName );
         break;

      case 'B':
         strcpy_s( sz, zsizeof( sz ), "VIEW " );
         strcat_s( sz, zsizeof( sz ), szName );
         strcat_s( sz, zsizeof( sz ), " BASED ON LOD " );
         strcat_s( sz, zsizeof( sz ), szName );
         BufInsertStr( sz );
         break;

      case 'G':
         strcpy_s( sz, zsizeof( sz ), "GET VIEW " );
         strcat_s( sz, zsizeof( sz ), szName );
         strcat_s( sz, zsizeof( sz ), " NAMED \"" );
         strcat_s( sz, zsizeof( sz ), szName );
         strcat_s( sz, zsizeof( sz ), "\"" );
         BufInsertStr( sz );
         break;

      case 'S':
         strcpy_s( sz, zsizeof( sz ), "NAME VIEW " );
         strcat_s( sz, zsizeof( sz ), szName );
         strcat_s( sz, zsizeof( sz ), " \"" );
         strcat_s( sz, zsizeof( sz ), szName );
         strcat_s( sz, zsizeof( sz ), "\"" );
         BufInsertStr( sz );
         break;
   }

   DropView( vList );

   hWinCW = WinQHCurrent();
   InvalidateRect( hWinCW, 0, FALSE );   // if not working try TRUE

   fnSetFocusEditor( );

   CWDLLResetDlgWindow();
   CWDLLRedrawDlgWindow();
   CWDLLShowCaret();

   return( 0 );
} // OBJ_PasteObjectName


zSHORT OPERATION
VOR_PasteName( zVIEW vSubtask )
{
   zVIEW  vList;
   zVIEW  vProfile;
   zPCHAR szOption;
   zPCHAR pszName;
   HWND   hWinCW;

   GetViewByName( &vList, szlTZCMLPLO_List, 0, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vList, szlW_MetaDef ) != zCURSOR_SET )
      return( 0 );

   GetProfileView( &vProfile );
   GetAddrForAttribute( &szOption, vProfile, szlED, szlPasteVorOption );
   GetAddrForAttribute( &pszName, vList, szlW_MetaDef, szlName );

   switch ( *szOption )
   {
      case 'N':
         strcpy_s( sz, zsizeof( sz ), pszName );
         break;

      case 'R':
         sprintf( sz, "VIEW %s REGISTERED AS %s", pszName, pszName );
         break;

      case 'G':
         sprintf( sz, "GET VIEW %s NAMED \"%s\"", pszName, pszName );
         break;

      case 'S':
         sprintf( sz, "NAME VIEW %s \"%s\"", pszName, pszName );
         break;

      case 'B':
      {
         zVIEW  vVOR;
         zPCHAR pszLOD_Name;

         ActivateMetaOI_ByName( &vVOR, vSubtask, zREFER_VOR_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                pszName, zCURRENT_OI );

         GetAddrForAttribute( &pszLOD_Name, vVOR, "LOD", "Name" );
         sprintf( sz, "VIEW %s BASED ON LOD %s", pszName, pszLOD_Name );

         DropMetaOI( vSubtask, vVOR );
         break;
      }
   }

   BufInsertStr( sz );
   DropView( vList );

   hWinCW = WinQHCurrent();
   InvalidateRect( hWinCW, 0, FALSE );   // if not working try TRUE

   fnSetFocusEditor( );

   CWDLLResetDlgWindow();
   CWDLLRedrawDlgWindow();
   CWDLLShowCaret();

   return( 0 );
} // VOR_PasteName

zSHORT OPERATION
OpIns_Postbuild( zVIEW vSubtask )
{
   fnSetSubwindowPosition( vSubtask, 0, szlOperationsList );
   return( 0 );
}

zSHORT OPERATION
OpIns_Close( zVIEW vSubtask )
{
   if ( UserFlagsOn( zUF_PerformingExit ) )
      fnSaveSubwindowPosition( vSubtask, 0, szlOperationsList, "Y" );
   else
      fnSaveSubwindowPosition( vSubtask, 0, szlOperationsList, "N" );

   return( 0 );
}

zSHORT OPERATION
OpIns_InsertOperation( zVIEW vSubtask )
{
   zCHAR  szType[ 5 ];
   zVIEW  vEdWrk;

   GetWorkView( &vEdWrk );
   GetStringFromAttribute( szType, zsizeof( szType ), vEdWrk, szlOperListType, szlType );
//   SetCursorFirstSelectedEntity( vEdWrk, szlOper, 0 );
   switch ( *szType )
   {
      case 'Z':
      {
         zVIEW vTZOPSIGK;

         // User wants to insert a Zeidon operation.  Retrieve the view that
         // contains the list of Zeidon operations and past the operation.
         GetViewByName( &vTZOPSIGK, szlTZOPSIGK, 0, zLEVEL_TASK );
         SetCursorFirstEntityByAttr( vTZOPSIGK, szlOperation, szlName,
                                     vEdWrk, szlOper, szlName, 0 );
         PasteOperation( vSubtask, vTZOPSIGK );

         break;
      }

      case 'G':
      {
         zVIEW vOp;
         char  szOpName[ 255 ];

         // User wants to insert a Global operation.  Retrieve the Global
         // Operations Group view that contains the operation.

         GetStringFromAttribute( szOpName, zsizeof( szOpName ), vEdWrk, szlOper, szlName );
         ActivateMetaOI_ByName( &vOp, vSubtask, zREFER_GO_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                szOpName, zCURRENT_OI );
         PasteOperation( vSubtask, vOp );
         break;
      }

      case 'L':
      {
         zVIEW vMeta;
         zLONG lMetaType;

         // User wants to insert a "Local"operation.  Retrieve the meta
         // and paste.

         GetIntegerFromAttribute( (zPLONG) &vMeta, vEdWrk, szlBuffer,
                                  szlSourceViewID );
         GetIntegerFromAttribute( &lMetaType, vEdWrk, szlBuffer, szlMetaType );

         // Create a temp view so that we can muck around with cursors.
         CreateViewFromViewForTask( &vMeta, vMeta, vSubtask );

         if ( lMetaType == zREFER_DIALOG_META ||
              lMetaType == zREFER_LOD_META )
         {
            // Dialog and LOD meta's can have multiple source files for one
            // meta.  Make sure we are dealing with the opertions for the
            // correct source file.
            SetCursorFirstEntityByAttr( vMeta, szlSourceFile, szlName,
                                        vEdWrk, szlBuffer,
                                        szlSourceFileName, 0 );
         }

         SetCursorFirstEntityByAttr( vMeta, szlOperation, szlName,
                                     vEdWrk, szlOper, szlName, 0 );
         PasteOperation( vSubtask, vMeta );
         DropView( vMeta );
         break;

      } // case 'L'...

      case 'O':
      {
         zVIEW  vObj;
         char   szObjectName[ 50 ];
         char   szOperName[ 255 ];
         zPCHAR psz;

         // User wants to insert an Object Operation.
         GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vEdWrk, szlOper, szlName );

         // Look for the period between the Object name and the Oper name.
         for ( psz = szObjectName; *psz && *psz != '.'; psz++ )
            ; // Nothing needs to be done here.

         // Change the period to a null-term and bump up the pointer to
         // point to the oper name.
         *psz++ = 0;

         // Copy the oper name.
         strcpy_s( szOperName, zsizeof( szOperName ), psz );

         // Activate the object meta.
         ActivateMetaOI_ByName( vSubtask, &vObj, vSubtask, zREFER_LOD_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                szObjectName, zFORCE_RELOAD );

         // Set the cursor to the operation.
         SetCursorFirstEntityByString( vObj, szlOperation, szlName,
                                       szOperName, szlLOD );
         PasteOperation( vSubtask, vObj );
         DropMetaOI( vSubtask, vObj );
         break;

      } // case 'O'...

      default:
         SysMessageBox( "Zeidon Internal",
                        "This operation type not yet supported", 0 );
         break;

   } // switch ( *szType )...

   return( 0 )

} // OpIns_InsertOperation

zSHORT OPERATION
VML_Postbuild( zVIEW vSubtask )
{
   fnSetSubwindowPosition( vSubtask, 0, szlVML_Insert );
   return( 0 );
}

zSHORT OPERATION
VML_Close( zVIEW vSubtask )
{
   if ( UserFlagsOn( zUF_PerformingExit ) )
      fnSaveSubwindowPosition( vSubtask, 0, szlVML_Insert, "Y" );
   else
      fnSaveSubwindowPosition( vSubtask, 0, szlVML_Insert, "N" );

   return( 0 );
}

zSHORT OPERATION
VML_InsertText( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  vEdWrk;
   char   szInsertString[ 500 ];
   zPCHAR pszStrBegin;
   zPCHAR pszStrEnd;
   zLONG  lCurrentCol;
   zBOOL  bPositionCursor;

   GetWorkView( &vEdWrk );
   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                  szlC_File ) == 0 )
   {
      MessagePrompt( vEdWrk, "ED0103", szlZeidonEditor,
                     "Can't insert VML into C file.", TRUE,
                     zBUTTONS_OK, 0, zICON_ERROR );
      return( 0 )
   }

   GetProfileView( &vProfileXFER );
   *szInsertString = 0;

   if ( CompareAttributeToString( vProfileXFER,
                                  szlED, szlVML_InsertFull, "Y" ) == 0 )
   {
      GetStringFromAttribute( szInsertString, zsizeof( szInsertString ), vProfileXFER,
                              szlVML_Text, szlInsertTextFull );
   }

   if ( *szInsertString == 0 )
   {
      GetStringFromAttribute( szInsertString, zsizeof( szInsertString ), vProfileXFER,
                              szlVML_Text, szlInsertText );
   }

   if ( *szInsertString == 0 )
   {
      GetStringFromAttribute( szInsertString, zsizeof( szInsertString ), vProfileXFER,
                              szlVML_Text, szlText );
   }

   if ( !*szInsertString )
      return( 0 )

   bPositionCursor = ( zstrchr( szInsertString, '&' ) != NULL );
   lCurrentCol     = BufQCurrentCol();

   // Insert each line seperately.
   for ( pszStrBegin = szInsertString; *pszStrBegin; pszStrBegin = pszStrEnd )
   {
      zBOOL bMoreLines;

      // Find the end of the current line by looking for the '\n' char or
      // the null terminator.
      for ( pszStrEnd = pszStrBegin;
            *pszStrEnd != '\n' && *pszStrEnd != '\r' && *pszStrEnd != 0;
            pszStrEnd++ )
         ;  // Nothing needs to be done here.

      // If we found a newline char, then change it to a null-terminator and
      // bump up pszStrEnd to point to the next char.
      if ( *pszStrEnd )
      {
         // If we found a carriage return, then a line feed is sure to follow
         // so we need to consider both chars.
         if ( *pszStrEnd == '\r' )
            *pszStrEnd++ = 0;

         *pszStrEnd++ = 0;
         bMoreLines = TRUE;
      }
      else
         bMoreLines = FALSE;

      BufInsertStr( pszStrBegin );
      if ( bMoreLines )
      {
         BufInsertEOL( );
         MovAbs( BufQCurrentLine(), lCurrentCol );
      }

   } // for...

   if ( bPositionCursor )
   {
      zLONG lLth;
      DWORD dwSrch;

      dwSrch = SrchSetFwd( FALSE );          // Set search to look backward.
      SrchFind( "&", SEARCH_IGCASE, &lLth );
      SrchSetFwd( (BOOL) dwSrch );           // Set back to previous value.
      BufDelChar( 1 );                       // Delete the '&'.
   }

   RepaintEditorWindow( vSubtask );

   return( 0 )

} // VML_InsertText

/*=========================================================================

   Toolbox operations

   The following operations are called from the CW toolbox.
   As a standard they all start with "tb".

===========================================================================*/
void OPERATION tbInsertZeidonAEQ( void )
{
   zVIEW vSubtask;

   GetSubtaskView( &vSubtask );
   SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow |
                                zWAB_ProcessImmediateAction,
                            szlTZEDCWZD, szlObjectSelAQ );
   return;
}

void OPERATION tbVML_Insert( void )
{
   zVIEW vSubtask;

   GetSubtaskView( &vSubtask );
   SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow |
                                zWAB_ProcessImmediateAction,
                            szlTZEDCWZD, szlVML_Insert );
   return;
}

void OPERATION tbOperInsert( void )
{
   zVIEW vSubtask;

   GetSubtaskView( &vSubtask );
   SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow |
                                zWAB_ProcessImmediateAction,
                            szlTZEDCWZD, szlOperationsList );
   return;
}

void OPERATION tbSaveFile( void )
{
   zVIEW vSubtask;

   GetSubtaskView( &vSubtask );
   SaveFile( vSubtask );
   return;
}

void OPERATION tbParseSource( void )
{
   zVIEW vSubtask;

   GetSubtaskView( &vSubtask );
   SaveAndParse( vSubtask );
   return;
}

void OPERATION tbGenSource( void )
{
   zVIEW vSubtask;

   GetSubtaskView( &vSubtask );
   Generate( vSubtask );
   return;
}

void OPERATION tbNextError( BOOL bShowErrorFile )
{
   zVIEW vEdWrk;
   LPSTR pszFilename;
   char  szErrorFile[ 300 ];

   GetWorkView( &vEdWrk );

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                  szlVML_File ) != 0 )
   {
      MessagePrompt( vEdWrk, "ED0104", szlZeidonEditor,
                     "Source file is not a VML file.", TRUE,
                     zBUTTONS_OK, 0, zICON_ERROR );
      return;
   }

   // Make sure that the error file exists.
   pszFilename = TransformFilename( BufQFilename (), "%b" );
   strcpy_s( szErrorFile, zsizeof( szErrorFile ), pszFilename );
   strcat_s( szErrorFile, zsizeof( szErrorFile ), ".ERR" );
   StrFree( pszFilename );
   if ( !FileExists( szErrorFile ) )
   {
      MessagePrompt( vEdWrk, "ED0105", szlZeidonEditor,
                     "Error file does not exist.\n"
                     "File must be parsed.", TRUE,
                     zBUTTONS_OK, 0, zICON_ERROR );
      return;
   }

   ErrorSrcFile( szErrorFile );
   ErrorNext( bShowErrorFile, 0 );

   return;

} // tbNextError

LPSTR
_StrNewPrintf(LPSTR s1, LPSTR s2, LPSTR s3)
{
 char buf[128];
 if (s3)
  wsprintf( buf, s1, s2, s3 );
 else
  wsprintf( buf, s1, s2 );
 return StrNew( buf );
}

/*
 ** BackupHandler
 *
 * Event handler for backing up files, attached to the 'EVENT_READY_TO_WRITE' event.
 *
 */
long DLL
BackupHandler( long evntnum, long data )
{
   int stat = 0;
   LPSTR file = 0;
   LPSTR spec = 0;
   LPSTR backup = 0;
   LPSTR p = 0;
   LPSTR msg = 0;
   LPSTR dir = 0;

   /* pull out the name of the file that may need to be backed up */
   if (data)
      file = *(LPSTR XFAR *)data;

   /* no need for backup unless the file already exists */
   if ( (file == 0) || !FileExists( file ) )
      goto done;

   /* get the backup specification in case it's needed */
   if ( (spec = BufQBackupSpec( )) == 0 )
      spec = BufQGlobalBackupSpec( );

   /* check for the backups being enabled */
   if ( (BufQSysFlags( ) & BUFFER_BACKUP) && (spec != 0) &&
        ((backup = TransformFilename( file, spec )) != 0) )
   {
      /* have backup filename, normalize it */
      backup = StrNormalizePath( p = backup );
      StrFree( p );
      p = 0;

      /* check for the backup file being the same as the original */
      if ( backup == 0 )
         msg = StrNew( "Invalid backup specification." );
      else
      if ( STRCMP( file, backup ) == 0 )
         msg = _StrNewPrintf( "The backup file cannot have the same name as the file being saved, \"%s\".", backup, 0 );

      /* check for the backup file name being used already */
      else
      if ( BufFindBuffer( backup, FALSE, TRUE ) )
         msg = _StrNewPrintf( "Can't use backup file \"%s\" because it is being edited.", backup, 0 );
      else
      if ( FileExists( backup ) && ( FileDelete( backup ) != 0 ) )
         msg = _StrNewPrintf( "Can't delete the existing backup file \"%s\".", backup, 0 );

      /* make sure directories along the path exist */
      else
      if ( ((dir = TransformFilename( backup, "%v%d" ) ) != 0) &&
           (FileCreateDirs( dir ) != 0) )
         msg = _StrNewPrintf( "Can't create directories for backup file \"%s\".", backup, 0 );

      /* move file to destination */
      else
      if ( FileMove( backup, file ) != 0 )
         msg = _StrNewPrintf( "Can't copy/move file \"%s\" to backup file \"%s\".", file, backup );
   }
   else
   if ( FileDelete( file ) != 0 )
      /* couldn't delete the file */
      msg = _StrNewPrintf( "Can't delete the existing file \"%s\".", file, 0 );

   /* display message box if necessary */
   if ( msg )
   {
   // MessageBox( WinQHFrame( ), msg, SysQAppName( ), MB_OK | CW_MODALFLAG | MB_ICONINFORMATION );
      MessageBox( WinQHFrame( ), msg, SysQAppName( ), MB_OK | MB_TASKMODAL | MB_ICONINFORMATION );
      stat = -1;
   }

done:
   StrFree( backup );
   StrFree( msg );
   StrFree( dir );
   return( stat );
}
