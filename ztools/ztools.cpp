// ztools.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#ifdef _DEBUG
#define DEBUG // ObjectEngine is always a little bit different
#pragma message( "compiling with DEBUG defined" )
#endif

#ifdef WIN32
#define __WIN32__ // ObjectEngine is always a little bit different
#pragma message( "compiling with __WIN32__ defined" )
#endif

#define  KZSYSSVC_INCL
#define  KZOESYS_INCL
#define  zNO_KZOEINCO // to avoid kzoeinco.h include
// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

#include "kzoengaa.h"

#ifdef __cplusplus
extern "C"
{
#endif
   #include "zeidonop.h"   //to force explicit c-linkage
#ifdef __cplusplus
}
#endif
#include "tz__oprs.h"
#include "tzlodopr.h"

#include "ztools.h"

#define MAX_TASKS           256
#include "common.h"  // header for process killing functions

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// The one and only application object

//CWinApp theApp;

using namespace std;

static zCPCHAR pszToolName = "Zeidon_Tools";
static BOOL g_bAppRegistered = FALSE;
static zCPCHAR g_pszVendor = "TOMS SOFTWARE HUT proudly presents BZE ( Build Zeidon Executables )";

int _tmain( int argc, TCHAR* argv[], TCHAR* envp[] )
{
   int nGenType = XOD;
   CString strProject;
   CString strTEName;
   CString strLogFile;
   CString strNetwork;
   CString strRemoveReadOnly = "N";
   CString strKillOE         = "Y";
   CFile   *fLog = NULL;

   int nRC = 0;
   int nError = 0;

   // initialize MFC and print and error on failure
   if ( !AfxWinInit( ::GetModuleHandle( NULL ), NULL, ::GetCommandLine(), 0 ) )
   {
      // TODO: change error code to suit your needs
      nRC = CMD_MFC_LOAD_FAILED;
   }
   else
   {
      SetConsoleCtrlHandler( ConsoleControlHandler, TRUE );

      // The command line is
      // "-G GenType -p Application" "-t TE-Name" "[-n]" ["Log-File"]
      // "GenType", "Application" and "TE-Name" are  required
      for ( int i = 1; i < argc && nRC == 0; k++ )
      {
         if ( ::stricmp( argv[ k ], "/?" ) == 0 || ::stricmp( argv[ k ], "-?" ) == 0 )
         {
            nRC = CMD_HELP_SYNTAX;
            break; // I usually don't like this
         }
         if ( i < argc )
         {
            //Check Switch
            if ( ::stricmp( argv[ k ], "-g" ) == 0 )
            {
               k++;
               if ( ::stricmp( argv[ k ], "XOD" ) == 0 )
                  nGenType = XOD;
               else
               if ( ::stricmp( argv[ k ], "XWD" ) == 0 )
                  nGenType = XWD;
               else
               if ( ::stricmp( argv[ k ], "XLP" ) == 0 )
                  nGenType = XLP;
               else
               if ( ::stricmp( argv[ k ], "XPE" ) == 0 )
                  nGenType = XPE;
               else
               if ( ::stricmp( argv[ k ], "XDM" ) == 0 )
                  nGenType = XDM;
               else
               if ( ::stricmp( argv[ k ], "DTE" ) == 0 )
                  nGenType = DTE;
               else
               if ( ::stricmp( argv[ k ], "XRP" ) == 0 )
                  nGenType = XRP;
               else
               if ( ::stricmp( argv[ k ], "ALL" ) == 0 )
                  nGenType = ALL;
               else
               {
                  nRC = CMD_INVALID_SYNTAX;
                  nGenType = -1;
               }
            }
            else
            if (::stricmp( argv[ k ], "-p" ) == 0 )
            {
               k++;
               strProject = argv[ k ];
            }
            else
            if (::stricmp( argv[ k ], "-t" ) == 0 )
            {
               k++;
               strTEName = argv[ k ];;
            }
            else
            if (::stricmp( argv[ k ], "-l" ) == 0 )
            {
               k++;
               strLogFile = argv[ k ];;
            }
            else
            if (::stricmp( argv[ k ], "-n" ) == 0 )
            {
               strNetwork = "X";
            }
            else
            if (::stricmp( argv[ k ], "-r" ) == 0 )
            {
               strRemoveReadOnly = "Y";
            }
            else
            if (::stricmp( argv[ k ], "-k" ) == 0 )
            {
               strKillOE = "Y";
            }
            else
               nRC = CMD_INVALID_SYNTAX;
         }
         else
            nRC = CMD_INVALID_ARGCOUNT;
      }
   }

   switch( nRC )
   {
   case CMD_OK:
      {
         ShowVendor();
         nRC = ProcessGentype( nGenType,
                               strProject,
                               strLogFile,
                               strTEName,
                               strNetwork,
                               strRemoveReadOnly,
                               strKillOE );
      }
      break;
   case CMD_MFC_LOAD_FAILED  :
         cerr << _T( "Fatal Error: MFC initialization failed" ) << endl;
         break;
   case CMD_INVALID_ARGCOUNT :
         cerr << _T( "Invalid number of arguments" ) << endl;
         nRC = CMD_HELP_SYNTAX;
      break;
   case CMD_INVALID_SYNTAX   :
   case CMD_HELP_SYNTAX      :
      ShowSyntax();
      break;
   default:
         cerr << _T( "Unexpected errorcode " ) << nRC << _T( "while parsing command line" ) << endl;
      break;
   }

   return ( nRC );
}

int ProcessGentype( zLONG nGenType,
                    CString strProject,
                    CString strLogFile,
                    CString strTEName,
                    CString strNetwork,
                    CString strRemoveReadOnly,
                    CString strKillOE )
{
   CFile *fLog = NULL;
   int nRC = 0;
   zLONG nCurrentGenType = nGenType;

   zVIEW vSubtask;
   if ( !strLogFile.IsEmpty() )
      fLog = new CFile( strLogFile, CFile::modeCreate | CFile::modeWrite );

   if (nGenType == ALL )
      nCurrentGenType  = GENTYPE_FIRST;
   else
      nCurrentGenType = nGenType;

   for ( nRC = 0;
         nCurrentGenType < GENTYPE_LAST && nRC == 0;
         nCurrentGenType++ )
   {
      if ( ZeidonApplicationProlog( &vSubtask, pszToolName ) == 0 )
      {
         g_bAppRegistered = TRUE;
         nRC = InitializeLPLR( vSubtask, (LPSTR)(LPCSTR)strProject );
         if ( nRC == 1 )
         {
            ::OverwriteMessageFunc( vSubtask );

            switch ( nCurrentGenType )
            {
            case XOD:
               cout << _T( "Processing LOD" ) << endl;
               nRC = ProcessXOD( vSubtask, strTEName, fLog, strNetwork );
               break;
            case XWD:
               cout << _T( "Processing dialogs" ) << endl;
               nRC = ProcessXWD( vSubtask, fLog, strRemoveReadOnly );
               break;
            case XPE:
               cout << _T( "Processing Presentation Environment" ) << endl;
               nRC = ProcessXPE( vSubtask, fLog, strRemoveReadOnly );
               break;
            case DTE:
               cout << _T( "Processing Technical Environment" ) << endl;
               nRC = ProcessDTE( vSubtask, fLog );
               break;
            case XDM:
               cout << _T( "Processing Domains" ) << endl;
               nRC = ProcessXDM( vSubtask, fLog );
               break;
            case XLP:
               cout << _T( "Processing Project/LPLR" ) << endl;
               nRC = ProcessXLP( vSubtask, fLog );
               break;
            case XRP:
               cout << _T( "Processing Reports" ) << endl;
               nRC = ProcessXRP( vSubtask, fLog, strRemoveReadOnly );
               break;
            default:
               cerr << _T( "Error: Unexpected executable Type " ) << nCurrentGenType << endl;
               nRC = CMD_HELP_SYNTAX;
               break;
            }
         }
         UnregisterZeidonApplication();
         g_bAppRegistered = FALSE;
         if (strKillOE == "Y" )
            KillObjectEngineProcess();
         if (nGenType != ALL )
            nCurrentGenType = GENTYPE_LAST;
      }
   }

   if ( fLog )
      fLog->Close();

   return( nRC );
}

void ShowSyntax()
{
   ShowVendor();
   cout << "bze version 0.9" << endl;
   cout << "Syntax of Build Zeidon Executables" << endl;
   cout << endl;
   cout << "bze -g [XOD|XWD|XLP|XDM|XPE|DTE|XRP|ALL]" << endl;
   cout << "    -p Project Name ( LPLR )" << endl;
   cout << "    [-t Name of Technical Environment]" << endl;
   cout << "    [-l Logfile]" << endl;
   cout << "    [-n](to build network XODs)" << endl;
   cout << "    [-r](to remove write protection prior to generate binaries )" << endl;
   cout << "    [-k](don't kill ObjectEngine after generate binaries )" << endl;
   cout << endl;
   cout << "Hint: The switches -t and -n are only valid with -g set to XOD." << endl;
   cout << "    They are ignored with other types." << endl;

   cout << "Examples:" << endl;
   cout << "    bze -G xod -p Account -t Account_DB2 -l C:\\temp.log" << endl;
   cout << "    to build XODs for project ( LPLR ) Account using the" << endl;
   cout << "    physical datamodel named Account_DB2 and write a logfile"<< endl;
   cout << "    C:\\temp.log"<< endl;
   cout << endl;
   cout << "    bze -G xwd Account" << endl;
   cout << "    to build XWDs for project ( LPLR ) Account" << endl;

}

void ShowVendor()
{
   cout << g_pszVendor << endl;
   cout << endl;
   cout << "EXCEPT FOR THE LIMITED WARRANTY, AND TO THE FULL EXTENT PERMITTED BY LAW, THE " << endl;
   cout << "AUTHORS PROVIDE THE SOFTWARE AS IS WITH ALL FAULTS, AND DISCLAIM ALL WARRANTIES," << endl;
   cout << "WHETHER ORAL OR WRITTEN, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT" << endl;
   cout << "LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR" << endl;
   cout << "PURPOSE, DATA ACCURACY AND COMPLETENESS, AND ANY WARRANTIES RELATING TO VIRUSES," << endl;
   cout << "ALL WITH REGARD TO THE SOFTWARE." << endl;
}

BOOL WINAPI ConsoleControlHandler(  DWORD dwCtrlType  )
{
   BOOL bReturn = FALSE;
   if (g_bAppRegistered )
      UnregisterZeidonApplication( );

   switch( dwCtrlType )
   {
   case CTRL_C_EVENT :
   case CTRL_BREAK_EVENT :
   case CTRL_CLOSE_EVENT :
      cerr << "process terminated at user request" << endl;
      ExitProcess( TERMINATE_CTRL_C  );
      break;
   case CTRL_LOGOFF_EVENT:
      cerr << "process terminated due to user logoff" << endl;
      ExitProcess( TERMINATE_SHUTDOWN );
      break;
   case CTRL_SHUTDOWN_EVENT:
      cerr << "process terminated due to system shutdown" << endl;
      ExitProcess( TERMINATE_SHUTDOWN );
      break;
   default:
      bReturn = FALSE;
   }

   return bReturn;
}

int ZeidonApplicationProlog( zPVIEW pvApp,
                             zCPCHAR pszApplicationName )
{
   int nRC = 1;
   zVIEW vSubtask = 0;

   CString strAppName = "~~";
   strAppName += pszApplicationName;
   strAppName += "~";

   nRC = RegisterZeidonApplication( pvApp, 0, 0, WM_USER + 1, strAppName, 0, 0 );

   if ( nRC == 0 )
   {
      SfCreateSubtask( &vSubtask, 0,  pszApplicationName );
      MessagePrompt( vSubtask, "1", "2", "3", 0, 0, 0, 0 );

      zVIEW vMSGOBJ;
      if ( GetViewByName( &vMSGOBJ, "__MSGQ", *pvApp, zLEVEL_TASK ) != zLEVEL_TASK )
      {
         if ( ActivateEmptyObjectInstance( &vMSGOBJ, "KZMSGQOO", *pvApp, zMULTIPLE ) >= 0 )
         {
            SetNameForView( vMSGOBJ, "__MSGQ", *pvApp, zLEVEL_TASK );
         }
      }

      if ( vSubtask )
         SfDropSubtask( vSubtask, 1 );
   }

   return( nRC );
}

int
OverwriteMessageFunc( zVIEW vAPP )
{
   LPTASK    lpTask;
// zLONG     lTaskID;
   LPMQINTERFACE  lpMQInterface = NULL;
   int nReturn = 0;

// lTaskID = SysGetTaskID( );
   lpTask = SfGetCurrentTask( );

   if ( lpTask )
      lpMQInterface = FindMQInterface( vAPP, lpTask );

   if (lpMQInterface )
   {
      lpMQInterface->pfn[ 0 ] = ConsoleMessageSend;
      lpMQInterface->pfn[ 1 ] = ConsoleMessagePrompt;
      lpMQInterface->pfn[ 2 ] = ConsoleMessagePromptForInput;
      lpMQInterface->pfn[ 3 ] = ConsoleMessagePresent;
      nReturn = 1;
   }
   else
      cerr << "unable to overwrite message handler functions" << endl;

   return nReturn;
}

LPMQINTERFACE
FindMQInterface( zVIEW vSubtask, LPTASK lpTask )
{
   LPAPP          lpApp;
   LPTASKAPP      lpTaskApp;
   LPMQINTERFACE  lpMQInterface = NULL;

   // If the view is a subtask view use its application, else
   // use the application of the task.
   SfGetApplicationForSubtask( &lpApp, vSubtask );

   // if a task is coming up, then it may not yet be tied to an
   // application, in this case get out fast.
   if ( !lpApp )
      return( 0 );

   // Find the appropriate TaskAppRecord
   // Scan the TaskApp chain for the matching application
   for ( lpTaskApp = (LPTASKAPP)SysGetPointerFromHandle( lpTask->hFirstTaskApp );
         lpTaskApp;
         lpTaskApp = (LPTASKAPP)SysGetPointerFromHandle( lpTaskApp->hNextTaskApp ) )
   {
      if ( SysGetPointerFromHandle( lpTaskApp->hApp ) == lpApp )
         break;
   }

   if ( !lpTaskApp  )
      lpTaskApp = (LPTASKAPP) SysGetPointerFromHandle( lpTask->hParentTask );

   if ( lpTaskApp )
   {
      // TaskAppRecord for the application was found on the chain
      lpMQInterface = (LPMQINTERFACE) SysGetPointerFromHandle( lpTaskApp->hMQInterface );
   }

   return( lpMQInterface );
}

zSHORT ConsoleMessageSend( zVIEW    vMessage,
                           zVIEW    vSubtask,
                           zCPCHAR  cpcMsgId,
                           zCPCHAR  cpcTitle,
                           zCPCHAR  cpcMsgText,
                           zLONG    lMsgType,
                           zSHORT   bBeep )
{
   zSHORT nRC = 0;
   cerr << "error:" << cpcMsgId << " " << cpcTitle << endl;
   cerr << cpcMsgText << endl;
   return( nRC );
}

zSHORT ConsoleMessagePrompt( zVIEW    vMessage,
                             zVIEW    vSubtask,
                             zCPCHAR  cpcMsgId,
                             zCPCHAR  cpcTitle,
                             zCPCHAR  cpcMsgText,
                             zSHORT   bBeep,
                             zSHORT   nButtons,
                             zSHORT   nDefaultButton,
                             zSHORT   nIcon )
{
   cerr << "error:" << cpcMsgId << " " << cpcTitle << endl;
   cerr << "\t Message Text\t= " << cpcMsgText << endl;
   return nDefaultButton;
}

zSHORT ConsoleMessagePromptForInput( zVIEW   vMessage,
                                     zVIEW   vSubtask,
                                     zCPCHAR cpcMsgId,
                                     zCPCHAR cpcTitle,
                                     zCPCHAR cpcMsgText,
                                     zSHORT  bBeep,
                                     zPCHAR  lpchReturnBuffer,
                                     zUSHORT usBufferLth )
{
   zSHORT nRC = 0;
   char strInbuff[ 512 ];
   cerr << "Waiting for input:" << cpcTitle << endl;
   cerr << "error:" << cpcMsgId << endl;
   cerr << "\t Message ID\t= " << cpcMsgId << endl;
   cerr << cpcMsgText << endl;
   cin >> strInbuff;
   ::strncpy( lpchReturnBuffer, strInbuff, __min( 512, usBufferLth ) );
   return( nRC );
}
zSHORT
ConsoleMessagePresent( zVIEW    vMessage,
                       zVIEW    vSubtask,
                       zLONG    lEventType )
{
   zSHORT nRC = 0;
   cerr << "Event occured\tEvent Type\t= " << lEventType << endl;
   return( nRC );
}

int
ProcessXWD( zVIEW vSubtask, CFile *fLog,
            zCPCHAR cRemReadOnly)
{
   zVIEW vLPLR;
   zVIEW vDialog;
   zSHORT nRC = 0;

   zCHAR szDialogName[ 32 ];
   CString strMsg;
   zPCHAR pszPath;
   zCHAR szOrder[ 512 ];
   zCHAR szTargetPath[ 128 ];
   zCHAR szSourcePath[ 128 ];

   if ( *cRemReadOnly == 'Y')
   {
      /* The program was called with parameter -r to remove the
      ** readonly attribute for all PWD-files. First we build a
      ** TMP-directory to save the original files. Then we remove
      ** the readonly attribute.
      */

      pszPath = getenv( "KZD" );
      strcpy_s( szTargetPath, pszPath );
      strcat_s( szTargetPath, ":\\" );
      pszPath = getenv( "KZV" );
      strcat_s( szTargetPath, pszPath );
      strcpy_s( szSourcePath, szTargetPath );
      strcat_s( szTargetPath, "\\savexwd" );
      strcat_s( szSourcePath, "\\a\\bin\\sys" );

      strcpy_s( szOrder, "mkdir " );
      strcat_s( szOrder, szTargetPath );

      InvokeSystemCommand( szOrder, fLog );

      strcpy_s( szOrder, "xcopy " );
      strcat_s( szOrder, szSourcePath );
      strcat_s( szOrder, "\\*.pwd " );
      strcat_s( szOrder, szTargetPath );
      strcat_s( szOrder, " /K /Q" );
      InvokeSystemCommand( szOrder, fLog );

      strcpy_s( szOrder, "attrib -r " );
      strcat_s( szOrder, szSourcePath );
      strcat_s( szOrder, "\\*.pwd" );
      InvokeSystemCommand( szOrder, fLog );
   }

   if ( nRC == 0 )
   {
      RetrieveViewForMetaList( vSubtask, vSubtask, &vLPLR, zSOURCE_DIALOG_META );
      if (!vLPLR )
      {
         strMsg = "Error generating XWD: Unable to retrieve list of Dialogs. Aborting build!";

         cerr << (LPCSTR) strMsg << endl;

         strMsg += "\n";
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         return( 1 );
      }

      nRC = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttribute( szDialogName, sizeof( szDialogName ), vLPLR, "W_MetaDef", "Name" );

         nRC = ActivateMetaOI( vSubtask, &vDialog, vLPLR, zSOURCE_DIALOG_META, zSINGLE );
         if ( nRC < 0 )
         {
            strMsg.Format( "Error generating XWD: Could not Activate Dialog: %s. Aborting build!", szDialogName );

            cerr << strMsg << endl;

            strMsg += "\n";
            if ( fLog )
               fLog->Write( strMsg, strMsg.GetLength() );

            return 2;
         }
         else
         {
            // The dialog ACCOUNT.PWD isn't ok. Therefore we get an error message
            // while saving the dialog and the releasebuilding system isn't running.
            // We don't save the dialog ACCOUNT.PWD until the bug is fixed. This dialog
            // is an example and not needed for a release.  RG, 17.12.1999
            if ( _strnicmp( szDialogName, "TZ", 2 ) == 0 )
            {
               // Send message that we are saving the XWD.
               strMsg.Format( "Saving Dialog: %s.", szDialogName );

               cout << (LPCSTR) strMsg << endl;

               strMsg += "\n";
               if ( fLog )
                  fLog->Write( strMsg, strMsg.GetLength() );

               SetCursorFirstEntity( vDialog, "Window", "" );
               CommitMetaOI( vDialog, zSOURCE_DIALOG_META );
               DropView( vDialog );
            }
            nRC = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );
         }
      }

      if ( *cRemReadOnly == 'Y' )
      {
         // After saving the PWD and XWD files we have to get
         // back the original files with readonly attribute
         strcpy_s( szOrder, sizeof( szOrder ), "xcopy " );
         strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
         strcat_s( szOrder, sizeof( szOrder ), "\\*.pwd " );
         strcat_s( szOrder, sizeof( szOrder ), szSourcePath );
         strcat_s( szOrder, sizeof( szOrder ), " /K /Q" );
         InvokeSystemCommand( szOrder, fLog );
         strcpy_s( szOrder, sizeof( szOrder ), "rmdir /S /Q " );
         strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
         InvokeSystemCommand( szOrder, fLog );
      }
   }

   return( nRC );
}

int InvokeSystemCommand( zCPCHAR pszOrder, CFile *fLog )
{
   int nRC;
   zCHAR szMsg[ 1024 ];

   nRC = system( pszOrder );

   if ( nRC == -1 )
   {
      switch (errno)
      {
         case E2BIG:
              strcpy_s( szMsg, sizeof( szMsg ), "Argument list is too big!\nOrder: " );
              strcat( szMsg, pszOrder );
              break;
         case ENOENT:
              strcpy_s( szMsg, "Command interpreter cannot be found !" );
              break;
         case ENOEXEC:
              strcpy_s( szMsg, "Command-interpreter file has invalid format and is not executable" );
              break;
         case ENOMEM:
              strcpy_s( szMsg, "Memory error: ENOMEM. See description of command 'system'" );
              break;
         default:
              strcpy_s( szMsg, "Unknown error while executing command 'system'" );
              break;
      }

      cerr << "Error running command" << szMsg << endl;

      strcat_s( szMsg, sizeof( szMsg ), "\n" );
      if ( fLog )
         fLog->Write( szMsg, strlen( szMsg ) );
   }

   return (nRC);

}

/**************************************************************************/
// Function name   : ProcessXOD
// Description     : generate Zeidon Object Executables
// Return type     : int
// Argument        : zVIEW vSubtask
// Argument        : zCPCHAR cTE_Name - Name of the technical envronment
//                                      that is use while generation XOD's
// Argument        : zCPCHAR cLogFile - logfile for errormessages
// Argument        : zCPCHAR cNet     - XOD's are generated using network handler
/**************************************************************************/
int
ProcessXOD( zVIEW vSubtask, zCPCHAR cTE_Name, CFile *fLog, zCPCHAR cNet )
{
   zVIEW vTaskLPLR = NULL;
   CString strMsg;
   zVIEW vTZTEDBLO, vDTE, vCM_List;
   zVIEW vLOD, vLOD_List, vXOD;
   zVIEW vTemp = NULL;
   zVIEW vT = NULL;
   zSHORT nRC;
   LPLIBRARY hLib = NULL;
   zCHAR szLODName[ 32 ];
   zCHAR szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR szNetwork[ zMAX_FILESPEC_LTH + 1 ];

   memset (szNetwork, 0, zMAX_FILESPEC_LTH+1 );
   SysReadZeidonIni( -1, "[Zeidon]", "DefaultNetwork", szNetwork, sizeof( szNetwork ) );
   if (*szNetwork == 0)
   {
      CString strMsg = "Error generating XOD: No default Network set! Aborting build!";

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return( 1 );
   }

   ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl", zMULTIPLE );
   SetNameForView( vTZTEDBLO, "TZTEDBLO", 0, zLEVEL_TASK );

   //oTZTENVRO_GetUpdViewForDTE_P
   nRC = RetrieveViewForMetaList( vSubtask, vSubtask, &vCM_List, zSOURCE_DTE_META );
   if (!vCM_List )
   {
      strMsg = "Error generating XOD: Unable to retrieve Techinical environment. Aborting build!";

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 2;
   }

   nRC = ActivateMetaOI( vSubtask, &vDTE, vCM_List, zSOURCE_DTE_META,
                         zSINGLE | zACTIVATE_ROOTONLY );
   if ( nRC < 0 )
   {
      strMsg = "Error generating XOD: Internal error activating TE. Aborting build!";

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 2;
   }
   SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   // Position on given DBMS Source
   nRC = SetCursorFirstEntityByString( vDTE, "TE_DBMS_Source", "Name", cTE_Name, 0 );
   if ( nRC < zCURSOR_SET )
   {
      strMsg.Format( "Error generating XOD: Technical Environment named %s does not exist. Aborting build!", cTE_Name );

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 3;
   }

   nRC = GetViewByName( &vTaskLPLR, "TaskLPLR", 0, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      strMsg.Format( "Error generating XOD: Unable to retrieve view %s. Aborting build!", "TaskLPLR" );

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 4;
   }

   // Check for network build
   if ( *cNet )
   {
      // Use default network for Ton Beller (DBMS = "Network Server")
      SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "Network", szNetwork, 0 );
      SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DBMS", vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
      SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Network", vTZTEDBLO, "TE_DBMS_Source", "Network" );
   }
   else
   {
      // Position on corresponding DBMS entry in DB Handler object.
      SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS", vDTE, "TE_DBMS_Source", "DBMS", 0 );

   }

   // Create a view that lists all LODs.
   RetrieveViewForMetaList( vSubtask, vSubtask, &vLOD_List, zREFER_LOD_META );
   if (!vLOD_List )
   {
      strMsg ="Error generating XOD: Unable to retrieve list of LODs. Aborting build!";

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 5;
   }

   OrderEntityForView( vLOD_List, "W_MetaDef", "Name A" );
   SetNameForView( vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Loop through each of the LOD's
   // WMetaType.Type= zREFER_LOD_META
   //
   for ( nRC = SetCursorFirstEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD_List, "W_MetaDef", "" ) )
   {

      GetStringFromAttribute( szLODName, sizeof( szLODName ), vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strMsg.Format( "Error generating XOD: Could not Activate LOD %s. Aborting build!", szLODName );

         cerr << strMsg << endl;

         strMsg += "\n";
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         return 6;
      }
      else
      {
         // Send message that we are building the LOD.
         strMsg.Format( "Building executable for LOD: %s", szLODName );

         cout << (LPCSTR) strMsg << endl;

         strMsg += "\n";
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         // Make sure the TE_SourceZKey attribute is set because it determines
         // what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
            SetAttributeFromAttribute( vLOD, "POD", "TE_SourceZKey",
                                       vDTE, "TE_DBMS_Source", "ZKey" );

         // Build the XOD in memory
         oTZZOXODO_SaveXOD( vLOD );
         SetNameForView( vLOD, "TZZOLODO", 0, zLEVEL_TASK );
         DropMetaOI( vLOD );

         // Commit the XOD to LPLR file.
         GetViewByName( &vXOD, "TZZOXODO", 0, zLEVEL_TASK );
         GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "ExecDir" );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, sizeof( szFileName ), szLODName );
         strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
         CommitOI_ToFile( vXOD, szFileName, zSINGLE );
      }
   }

   return( 0 );
}

int ProcessDTE( zVIEW vSubtask, CFile *fLog )
{
   zVIEW  vDTE;  // Physical Datamodel
   zVIEW  vEMD;  // Datamodel
   zVIEW  vTZTEDBLO;
   zCHAR  szMsg[ 256 ];
   zSHORT nRC;
   zCHAR  szDataSource[ 33 ];
   zCHAR  szTableName[ 33 ];

   /* First open the ZeidonDTE
   */
   oTZTENVRO_GetUpdViewForDTE_P( &vDTE );
   if ( vDTE )
   {
      SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   }
   else
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Error generating dte: Zeidon DTE could not be loaded. Aborting Build!" );
      cerr << szMsg << endl;

      strcat_s( szMsg, sizeof( szMsg ), "\n" );
      if ( fLog )
         fLog->Write( szMsg, strlen( szMsg ) );
      return( 1 );
   }

   /* Schleife ueber alle DataSources */
   nRC = SetCursorFirstEntity( vDTE, "TE_DBMS_Source", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szDataSource, sizeof( szDataSource ), vDTE, "TE_DBMS_Source", "Name" );

      strcpy_s( szMsg, sizeof( szMsg ), "Deleting/Initializing Tables from Datasource: " );
      strcat_s( szMsg, sizeof( szMsg ), szDataSource );

      cout << szMsg << endl;

      strcat_s( szMsg, sizeof( szMsg ), "\n" );
      if ( fLog )
         fLog->Write( szMsg, strlen( szMsg ) );

      /* Delete all tables from the DataSource */
      nRC = SetCursorFirstEntity( vDTE, "TE_TablRec", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttribute( szTableName, sizeof( szTableName ), vDTE, "TE_TablRec", "Name" );
         strcpy_s( szMsg, sizeof( szMsg ), "Table: " );
         strcat_s( szMsg, sizeof( szMsg ), szTableName );

         cout << szMsg << endl;

         strcat_s( szMsg, sizeof( szMsg ), "\n");
         if ( fLog )
            fLog->Write( szMsg, strlen( szMsg ) );
         nRC = DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
         nRC = SetCursorFirstEntity( vDTE, "TE_TablRec", "" );
      }

      /* InitTables */
      if (CompareAttributeToString( vDTE, "TE_DBMS_Source", "Paradigm", "S") == 0)
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Can't add Table Records or Relationships for Remote Servers" );

         cout << szMsg << endl;

         strcat_s( szMsg, sizeof( szMsg ), "\n" );
         if ( fLog )
            fLog->Write( szMsg, zstrlen( szMsg ) );
      }
      else
      {
         oTZEREMDO_GetRefViewForER( &vEMD, zCURRENT_OI );
         if ( vEMD )
         {
            if ( CountEntitiesForView( vDTE, "TE_DBMS_Source" ) > 1 )
               oTZTENVRO_BuildTablRecsFromEMD( vDTE, vEMD, "TE_DBMS_Source" );

            /* Init RelationShips
            ** hierfuer wird TZTEDBLO benoetigt.
            */
            ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl", zMULTIPLE );
            SetNameForView( vTZTEDBLO, "TZTEDBLO", 0, zLEVEL_TASK );
            nRC = oTZTENVRO_BuildRelationsFromEMD( vDTE, vEMD );

            /* Sichern der Aenderungen */
            nRC = CommitMetaOI( vDTE, zSOURCE_DTE_META );
            if ( nRC < 0 )
            {
               strcpy_s( szMsg, sizeof( szMsg ), "Unable to save Physical Environment. Aborting Build" );

               cout << szMsg << endl;

               strcat_s( szMsg, sizeof( szMsg ), "\n" );
               if ( fLog )
                  fLog->Write( szMsg, zstrlen( szMsg ) );
               return 2;
            }
         }
      }

      // Auf naechste DataSource positionieren
      nRC = SetCursorNextEntity (vDTE, "TE_DBMS_Source", "");
   }

   DropView (vDTE);
   DropView (vEMD);
   return( 0 );
}


int
ProcessXDM( zVIEW vSubtask, CFile *fLog )
{
   zSHORT   nRC = zCURSOR_NULL;
   zVIEW    vDomainGrp = 0;
   zVIEW    vCM_List = 0;
   zVIEW    vCM_List2 = 0;
   char     szName[255];
   zSHORT   RESULT = zCURSOR_NULL;

   nRC = RetrieveViewForMetaList( vSubtask, vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if (!vCM_List )
   {
      CString strMsg = "Error generating XDM: Unable to retrieve list of Domains. Aborting build!";

      cerr << strMsg << endl;

      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );

      return( 1 );
   }

   RESULT = SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );
   while ( RESULT >= zCURSOR_SET )
   {
      GetStringFromAttribute( szName, sizeof( szName ), vCM_List, "W_MetaDef", "Name" );

      cout << "Processing domain: " <<  szName << endl;;

      // Use a second CM_List view for Activate since Activate will alter
      // its position.
      CreateViewFromView( &vCM_List2, vCM_List );
      nRC = ActivateMetaOI( vSubtask, &vDomainGrp, vCM_List2,
                            zREFER_DOMAIN_META, zSINGLE | zLEVEL_APPLICATION );
      if ( nRC >= 0 )
      {
         if ( RESULT >= zCURSOR_SET )
            oTZDMSRCO_ReplaceOrAddDomToXDM( vDomainGrp, 0 );
         else
            oTZDMSRCO_ReplaceOrAddDomToXDM( vDomainGrp, 1 );
         DropMetaOI( vDomainGrp );
      }
      else
      {
         CString strMsg = "Error generating XDM: ActivateMetaOI failed for domain: ";
         strMsg += szName ;

         cerr << strMsg << endl;
         strMsg += "\n";
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );
      }

      DropView( vCM_List2 ); //drop temporary view

      RESULT = SetCursorNextEntity( vCM_List, "W_MetaDef", "" );
   }

   return( 0 );

}


int ProcessXLP( zVIEW vSubtask, CFile *fLog )
{
   HFILE  hFile;

   zVIEW  vZeidonCM;
   zVIEW  vTZCMWKSO;
   zVIEW  LPLR_View;
   zVIEW  vTZCMSLPL;

   zCHAR  szLPLR_FileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szDirectorySpec[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szLPLR_Name[ 33 ];
   zCHAR  szLPLR_FileName[ 10 ];
   zCHAR  szDetachSpec[ zMAX_FILESPEC_LTH + 1 ];
   CString strMsg;

   long   TaskUseCount;
   unsigned long ulCurrentZKey;
   unsigned long ulZKey;
   int    nRC, LPLR_Activated;

   SetNameForView( vSubtask, "TZCM_RebuildLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &vTZCMWKSO, "TZCMWKSO", vZeidonCM, zLEVEL_SUBTASK );
   if ( vTZCMWKSO == 0 )  // View isn't there
   {
      strMsg = "Error generating XLP: Work Station Object TZCMWKSO not found! Aborting Build\n";

      cerr << strMsg;
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return( 1 );
   }

   nRC = CreateViewFromView( &vTZCMSLPL, vTZCMWKSO );
   SetNameForView( vTZCMSLPL, "TZCMSLPL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZCMSLPL, "LPLR", "ZKey" );
   GetIntegerFromAttribute( (zPLONG) &ulCurrentZKey, vTZCMWKSO, "LPLR", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vTZCMWKSO, "LPLR", "ZKey", ulZKey, "" );
   GetStringFromAttribute( szLPLR_Name, sizeof( szLPLR_Name ), vTZCMWKSO, "LPLR", "Name" );
   LPLR_Activated = GetViewByName( &LPLR_View, szLPLR_Name, vZeidonCM, zLEVEL_SUBTASK );
   GetStringFromAttribute( szLPLR_FileSpec, sizeof( szLPLR_FileSpec ), vTZCMWKSO, "LPLR", "ExecDir" );
   ofnTZCMWKSO_AppendSlash( szLPLR_FileSpec );
   zstrncpy( szLPLR_FileName, szLPLR_Name, 9 );
   for ( nRC = 0; nRC < 8; nRC++ )
   {
      if ( !( szLPLR_FileName[ nRC ] ) )
         break;
      if ( szLPLR_FileName[ nRC ] == ' ' )
         szLPLR_FileName[ nRC ] = '_';
   }

   szLPLR_FileName[ nRC ] = 0;
   strcat_s( szLPLR_FileSpec, sizeof( szLPLR_FileSpec ), szLPLR_FileName );
   strcat_s( szLPLR_FileSpec, sizeof( szLPLR_FileSpec ), ".XLP" );
   if ( LPLR_Activated < 1 ) // LPLR currently not activated
   {
      hFile = (HFILE) SysOpenFile( szLPLR_FileSpec, COREFILE_READ );
      if ( hFile > 0 )
      {
         // Activate LPLR for rebuild only
         SysCloseFile( hFile, 0 );
         nRC = ActivateOI_FromFile( &LPLR_View, "TZCMLPLO", vSubtask, szLPLR_FileSpec, zSINGLE );

         // Send message that we are building LPLR Metas
         strMsg = "Building LPLR Metas\n";

         cout << (LPCSTR) strMsg;

         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         nRC = BuildLPLR_MetaTypes( LPLR_View, 1 );
      }
      else
      {
         if ( ActivateEmptyObjectInstance( &LPLR_View, "TZCMLPLO", vSubtask, zSINGLE ) != 0 )
         {
            strMsg = "Error generating XLP: Failed activating empty LPLR Instance! Aborting Build\n";

            cerr << strMsg;
            if ( fLog )
               fLog->Write( strMsg, strMsg.GetLength() );

            return 2;
         }

         CreateMetaEntity( LPLR_View, "LPLR", zPOS_AFTER );
         SetAttributeFromAttribute( vTZCMWKSO, "LPLR", "ZKey", LPLR_View, "LPLR", "ZKey" );
         SetMatchingAttributesByName( LPLR_View, "LPLR", vTZCMWKSO, "LPLR", zSET_NOTNULL );
         if ( CheckExistenceOfEntity( vTZCMWKSO, "CorrespondingCPLR" ) >= zCURSOR_SET )
              IncludeSubobjectFromSubobject( LPLR_View, "CorrespondingCPLR",
                                             vTZCMWKSO, "CorrespondingCPLR", zPOS_AFTER );
         // Send message that we are building LPLR Metas
         strMsg = "Building LPLR Metas\n";
         cout << (LPCSTR) strMsg;
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         nRC = BuildLPLR_MetaTypes( LPLR_View, 1 );
      }
   }
   else  // LPLR is currently active
   {
      if ( ulZKey != ulCurrentZKey )
      {
         strMsg = "Error generating XLP: LPLR is currently active in another task.\n";
         strMsg += "                      Rebuilding the LPLR may corrupt the meta list.\n";
         strMsg += "                      Rebuilding of the LPLR is therefore cancelled!\n";
         strMsg += "Aborting Build\n";

         cerr << strMsg;

         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );
         return 3;
      }
      else
      {
         GetIntegerFromAttribute( &TaskUseCount, LPLR_View, "LPLR", "TaskUseCount" );
         if ( TaskUseCount > 1 )
         {
            strMsg =  "Error generating XLP: LPLR is currently active in more than one task.\n";
            strMsg += "                      Rebuilding the LPLR may corrupt the meta list.\n";
            strMsg += "                      Rebuilding of the LPLR is therefore cancelled!\n";
            strMsg += "Aborting Build\n";

            cerr << strMsg;

            if ( fLog )
               fLog->Write( strMsg, strMsg.GetLength() );
            return 3;
         }
      }

      // Send message that we are building LPLR Metas
      strMsg =  "Building Project Metas.\n";

      cout << (LPCSTR) strMsg;

      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );

      nRC = BuildLPLR_MetaTypes( LPLR_View, 1 );
   }

   if ( nRC < 0 )
   {
      strMsg =  "Error generating XLP: Build of MetaTypes failed! Aborting Build.\n";

      cerr << strMsg;

      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 4;
   }

   /* Test to see if we are indeed writing the correct O to file */
   GetStringFromAttribute( szDirectorySpec, sizeof( szDirectorySpec ), LPLR_View, "LPLR", "Name" );
   if ( zstrcmpi( szDirectorySpec, szLPLR_Name ) != 0 )
   {
      CString strHelp = "Error generating XLP: LPLR Name %s doesn't match name of OI file:\n %s\n";
      strHelp += "Commit of LPLR aborted! Aborting Build\n";

      strMsg.Format( strHelp, szLPLR_Name, szLPLR_FileSpec );

      cerr << strMsg;

      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 5;
   }
   nRC = zgSortEntityWithinParent( zASCENDING, LPLR_View, "W_MetaType", "Type", "" );
   if ( CommitOI_ToFile( LPLR_View, szLPLR_FileSpec, zSINGLE ) == 0 )
   {
      strMsg.Format( "LPLR named %s  has been rebuilt in\n %s \n", szLPLR_Name, szLPLR_FileSpec );

      cout << (LPCSTR) strMsg ;

      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
   }
   if ( LPLR_Activated < 1 )  // Application LPLR View not found
   {
      // Drop the OI because it wasn't activated before
      DropObjectInstance( LPLR_View );
   }
   if ( SysGetEnvVar( szDetachSpec, "ZEIDON", zMAX_FILENAME_LTH + 1 ) == 0 )
   {
      ofnTZCMWKSO_AppendSlash( szDetachSpec );
      strcat_s( szDetachSpec, sizeof( szDetachSpec ), "TZCM.DET" );
      hFile = (HFILE)SysOpenFile( szDetachSpec, COREFILE_DELETE );
   }
   nRC = SetCursorFirstEntityByInteger( vTZCMWKSO, "LPLR", "ZKey", ulCurrentZKey, "" );

   return( 0 );
}


int ProcessXPE( zVIEW vSubtask,
                CFile *fLog,
                zCPCHAR cRemReadOnly )
{
   zPCHAR pszPath;
   zCHAR  szOrder[ 512 ];
   zCHAR  szTargetPath[ 128 ];
   zCHAR  szPPEPath[ 128 ];
   zSHORT nRC;
   zVIEW  vCM_List;
   zVIEW  vPresEnv;

   if ( *cRemReadOnly == 'Y')
   {
      /* The program was called with parameter -r to remove the
      ** readonly attribute from PPE-file. First we build a
      ** TMP-directory to save the original file. Then we remove
      ** the readonly attribute.
      */

      pszPath = getenv( "KZD" );
      strcpy_s( szTargetPath, pszPath );
      strcat_s( szTargetPath, ":\\" );
      pszPath = getenv( "KZV" );
      strcat_s( szTargetPath, pszPath );
      strcpy_s( szPPEPath, szTargetPath );
      strcat_s( szTargetPath, "\\savexpe" );
      strcat_s( szPPEPath, "\\a\\bin\\sys" );

      strcpy_s( szOrder, "mkdir " );
      strcat_s( szOrder, szTargetPath );

      if ( InvokeSystemCommand( szOrder, fLog ) == -1 )
      {
         return( 1 );
      }

      strcpy_s( szOrder, "xcopy " );
      strcat_s( szOrder, szPPEPath );
      strcat_s( szOrder, "\\*.ppe " );
      strcat_s( szOrder, szTargetPath );
      strcat_s( szOrder, " /K /Q" );
      if ( InvokeSystemCommand( szOrder, fLog ) == -1 )
      {
         return( 1 );
      }

      strcpy_s( szOrder, "attrib -r " );
      strcat_s( szOrder, szPPEPath );
      strcat_s( szOrder, "\\*.ppe" );
      if (InvokeSystemCommand( szOrder, fLog ) == -1)
      {
         return( 1 );
      }
   }

   // Get file list of Presentation Environments (only one).
   nRC = RetrieveViewForMetaList( vSubtask, vSubtask, &vCM_List, zSOURCE_PENV_META );
   if (!vCM_List )
   {
      CString strMsg = "Unable to retrieve list of Presentation Environments. Aborting build!";

      cerr << strMsg << endl;

      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 2;
   }

   // Always have a Named List of PPE's (even though there can only be One!)
   // This is done to be consistent with other dialogs.  The only time the
   // that the named List will not exist is during a SwithLPLR.
   // This fact is used a "flag" to the ReturnFromSubWnd logic.
   nRC = SetNameForView( vCM_List, "CM_PPE_List", 0, zLEVEL_TASK );

   // see if one exists
   nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );

   // if not, create one, else open it
   if ( nRC != zCURSOR_NULL )
   {
      nRC = ActivateMetaOI( vSubtask, &vPresEnv, vCM_List, zSOURCE_PENV_META, zSINGLE );
      SetNameForView( vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

      ResetView( vPresEnv );

      OrderEntityForView( vPresEnv, "Color", "Tag A" );
      OrderEntityForView( vPresEnv, "ControlDef", "Tag A" );
      AssignZKeyToMetaOI( vPresEnv, zSOURCE_PENV_META, 0 );

      nRC = CommitMetaOI( vPresEnv, zSOURCE_PENV_META );
   }
   else
   {
      // Fehlermeldung
      CString strMsg = "PPE-List could not be opened. Aborting Build";

      cerr << strMsg << endl;
      strMsg += "\n";
      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );
      return 2;
   }

   if (*cRemReadOnly == 'Y')
   {
      /* After saving of XPE we have to get back the original
      ** files with readonly attribute
      */
      strcpy_s( szOrder, sizeof( szOrder ), "xcopy " );
      strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
      strcat_s( szOrder, sizeof( szOrder ), "\\*.ppe " );
      strcat_s( szOrder, sizeof( szOrder ), szPPEPath );
      strcat_s( szOrder, sizeof( szOrder ), " /K /Q" );

      InvokeSystemCommand( szOrder, fLog );

      strcpy_s( szOrder, sizeof( szOrder ), "rmdir /S /Q " );
      strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
      InvokeSystemCommand( szOrder, fLog );
   }
   return( 0 );
}


int ProcessXRP( zVIEW vSubtask, CFile *fLog, zCPCHAR cRemReadOnly)
{
   zVIEW vLPLR;
   zVIEW vReport;
   zVIEW vReportConvert;
   zSHORT nRC;

   zCHAR szReportName[ 33 ];
   CString strMsg;
   zPCHAR pszPath;
   zCHAR szOrder[ 512 ];
   zCHAR szTargetPath[ 128 ];
   zCHAR szSourcePath[ 128 ];

   if ( *cRemReadOnly == 'Y' )
   {
      /* The program was called with parameter -r to remove the
      ** readonly attribute for all PWD-files. First we build a
      ** TMP-directory to save the original files. Then we remove
      ** the readonly attribute.
      */

      pszPath = getenv ("KZD");
      strcpy_s( szTargetPath, sizeof( szTargetPath ), pszPath );
      strcat_s( szTargetPath, sizeof( szTargetPath ), ":\\" );
      pszPath = getenv( "KZV" );
      strcat_s( szTargetPath, sizeof( szTargetPath ), pszPath );
      strcpy_s( szSourcePath, sizeof( szTargetPath ), szTargetPath );
      strcat_s( szTargetPath, sizeof( szTargetPath ), "\\savexrp" );
      strcat_s( szSourcePath, sizeof( szTargetPath ), "\\a\\bin\\sys" );

      strcpy_s( szOrder, sizeof( szOrder ), "mkdir " );
      strcat_s( szOrder, sizeof( szOrder ), szTargetPath );

      if ( InvokeSystemCommand( szOrder, fLog ) == -1)
      {
         return( 1 );
      }

      strcpy_s( szOrder, sizeof( szOrder ), "xcopy " );
      strcat_s( szOrder, sizeof( szOrder ), szSourcePath );
      strcat_s( szOrder, sizeof( szOrder ), "\\*.prp " );
      strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
      strcat_s( szOrder, sizeof( szOrder ), " /K /Q" );
      if ( InvokeSystemCommand( szOrder, fLog ) == -1)
      {
         return( 1 );
      }

      strcpy_s( szOrder, sizeof( szOrder ), "attrib -r " );
      strcat_s( szOrder, sizeof( szOrder ), szSourcePath );
      strcat_s( szOrder, sizeof( szOrder ), "\\*.prp" );
      if ( InvokeSystemCommand( szOrder, fLog ) == -1 )
      {
         return( 1 );
      }
   }

   RetrieveViewForMetaList( vSubtask, vSubtask, &vLPLR, zSOURCE_REPORT_META );
   if (!vLPLR )
   {
      strMsg = "Unable to retrieve list of Reports. Aborting build!";

      cerr << strMsg << endl;

      if ( fLog )
         fLog->Write( strMsg, strMsg.GetLength() );

      return 2;
   }

   nRC = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szReportName, sizeof( szReportName ), vLPLR, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vReport, vLPLR, zSOURCE_REPORT_META, zSINGLE );
      if ( nRC < 0 )
      {
         strMsg = "Could not Activate Report: ";
         strMsg += szReportName;
         strMsg += ". Aborting Build";

         cerr << strMsg << endl;

         strMsg += "\n";
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         return( 1 );
      }
      else
      {
         // Send message that we are saving the XRP.
         strMsg = "Saving Report: ";
         strMsg += szReportName;

         cout << (LPCSTR) strMsg << endl;

         strMsg += "\n";
         if ( fLog )
            fLog->Write( strMsg, strMsg.GetLength() );

         // Positions in the report def are interpreted in 2 ways.
         // In the painter, they determine the position on the screen.
         // However, at print time, they are not accurate if one of the
         // controls within a group are a multiline text control. The size
         // of a multi line text control can NOT be determined until it is
         // printed. For this reason, the routines responsible for printing
         // will have to adjust the positions of any control that follows
         // a multiline control. The loop in the printing routine will loop
         // thru all controls for a group. For things to be printed right,
         // it needs to encounter the controls sorted by position, otherwise,
         // the adjustment for multiline controls will be nonsense. This
         // sorting can be done at print time. For efficiency sake, we
         // will do it here at save time.

         // This sorts lexicographically by psdlg_x in ascending order
         // and psdlg_y in ascending order.
         OrderEntityForView( vReport, "Control", "PSDLG_Y A PSDLG_X A" );

         //fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0 );

         CreateViewFromView( &vReportConvert, vReport );
         AssignZKeyToMetaOI( vReportConvert, zSOURCE_REPORT_META, 0 );

         ResetView( vReportConvert );

         if ( CommitMetaOI( vReportConvert, zSOURCE_REPORT_META ) < 0 )
         {
            strMsg = "Unable to save Report: ";
            strMsg += szReportName;

            cerr << "Error: " << strMsg << endl;

            strMsg += "\n";
            if ( fLog )
               fLog->Write( strMsg, strMsg.GetLength() );
         }

         DropView( vReportConvert );
         DropView( vReport );
      }

      nRC = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );
   }

   if ( *cRemReadOnly == 'Y' )
   {
      // After saving the PWD and XWD files we have to get
      // back the original files with readonly attribute
      strcpy_s( szOrder, sizeof( szOrder ), "xcopy " );
      strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
      strcat_s( szOrder, sizeof( szOrder ), "\\*.prp " );
      strcat_s( szOrder, sizeof( szOrder ), szSourcePath );
      strcat_s( szOrder, sizeof( szOrder ), " /K /Q" );
      InvokeSystemCommand( szOrder, fLog );

      strcpy_s( szOrder, sizeof( szOrder ), "rmdir /S /Q " );
      strcat_s( szOrder, sizeof( szOrder ), szTargetPath );
      InvokeSystemCommand( szOrder, fLog );
   }

   return( 0 );
}

BOOL
KillObjectEngineProcess( )
{
   BOOL bReturn = FALSE;

   BOOL        ForceKill = TRUE;
   CHAR        pname[] = {"KZOENGWA"};
   TASK_LIST   tlist[ MAX_TASKS ];

   DWORD             k;
   DWORD             numTasks;
   TASK_LIST_ENUM    te;
   int               rval = 0;
   char              tname[ PROCESS_SIZE ];
   LPSTR             p;
   DWORD             ThisPid;
   OSVERSIONINFO     verInfo = { 0 };
   LPGetTaskList     GetTaskList;
   LPEnableDebugPriv EnableDebugPriv;

   //
   // Determine what system we're on and do the right thing
   //
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
         cerr << "Killing the Object Engine requires Windows NT or Windows 95" << endl;
         return( 1 );
   }


   // Obtain the ability to manipulate other processes
   EnableDebugPriv();

   /*
   if (pid)
   {
       tlist[0].dwProcessId = pid;
       if (KillProcess( tlist, TRUE )) {
           printf( "process #%d killed\n", pid );
           return( 0 );
       } else {
           printf( "process #%d could not be killed\n", pid );
           return( 1 );
       }
   }
   */
   //
   // get the task list for the system
   //
   numTasks = GetTaskList( tlist, MAX_TASKS );

   //
   // enumerate all windows and try to get the window
   // titles for each task
   //
   te.tlist = tlist;
   te.numtasks = numTasks;
   GetWindowTitles( &te );

   ThisPid = GetCurrentProcessId();

   for ( k = 0; k < numTasks; k++ ) {
       //
       // this prevents the user from killing KILL.EXE and
       // it's parent cmd window too
       //
       if ( ThisPid == tlist[ k ].dwProcessId ) {
           continue;
       }
       if ( MatchPattern( tlist[ k ].WindowTitle, (UCHAR *)"*KILL*" ) ) {
           continue;
       }

       tname[ 0 ] = 0;
       strcpy( tname, (LPCSTR)tlist[ k ].ProcessName );
       p = strchr( tname, '.' );
       if ( p ) {
           p[ 0 ] = '\0';
       }
       if ( MatchPattern( (UCHAR*)tname, (UCHAR*)pname ) ) {
            tlist[ k ].flags = TRUE;
       } else if ( MatchPattern( tlist[ k ].ProcessName, (UCHAR*)pname ) ) {
           tlist[ k ].flags = TRUE;
       } else if ( MatchPattern( tlist[ k ].WindowTitle, (UCHAR*)pname ) ) {
           tlist[ k ].flags = TRUE;
       }
   }

   for ( k = 0; k < numTasks; k++ ) {
       if ( tlist[ k ].flags ) {
           if ( !KillProcess( &tlist[ k ], ForceKill ) )
           {
               printf( "process #%d [%s] could not be killed\n", tlist[ k ].dwProcessId, tlist[ k ].ProcessName );
               rval = 1;
           }
       }
   }

   return rval;
}
