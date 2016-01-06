//----------------------------------------------------------------------
//
// .Name:          KZSIRREP.C
//
// .Change state:  11.09.2001
//
// .Description:   Access of Zeidon to Siron reports
//
//----------------------------------------------------------------------
// .Extended Description:
//
//  This module sends messages to the Siron compiler SIRCOMP.EXE.
//  SIRCOMP.EXE runs the reports for Zeidon applications and
//  returns a status message.
//
//
//  Interfaces:
//   (1) Zeidon Object Service (using Library KZOENGAA)
//   (2) Siron Message Lib (using Library SIRDMSGC)
//
//----------------------------------------------------------------------
// .Change notes:
//
// 2001.09.11  RG
//    Comment in function SironDisconnect
// 2000.08.10  RG  Z2000
//    Remove compiler warnings
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// System Header
//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------
// global Header
//----------------------------------------------------------------------

#define KZSYSSVC_INCL
#include "KZOENGAA.H"

#include <hzcmsgci.h> // the interface to the Siron Reports
#include <kzsirrep.h>

//----------------------------------------------------------------------
// .typedef
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// .prototypes
//----------------------------------------------------------------------

// function prototypes
static zSHORT fnLoadMsgLib( zVIEW );
static zSHORT fnFreeMsgLib( void );
static zSHORT fnRequestReports( void );
static zVOID  fnReleaseReports( void );


// internal prototypes for Siron calls
short (PDLL_ENTRY pfnSironShowPreview)();
short (PDLL_ENTRY pfnSironConnectRDT)();
short (PDLL_ENTRY pfnSironDisconnectRDT)();
short (PDLL_ENTRY pfnSironStartRDT)();
short (PDLL_ENTRY pfnSironStartWaitForRDT)();
short (PDLL_ENTRY pfnSironStopWaitForRDT)();
short (PDLL_ENTRY pfnSironConnect)();
short (PDLL_ENTRY pfnSironDisconnect)();
short (PDLL_ENTRY pfnSironStartWait)();
short (PDLL_ENTRY pfnSironStopWait)();
short (PDLL_ENTRY pfnSironRunQuery)();
short (PDLL_ENTRY pfnSironAbortCompiler)();
short (PDLL_ENTRY pfnSironRequestStatus)();
short (PDLL_ENTRY pfnSironGetLastStatus)();
short (PDLL_ENTRY pfnSironWaitForStatus)();
short (PDLL_ENTRY pfnSironSetMessageCallback)();
short (PDLL_ENTRY pfnSironTerminate)();



//----------------------------------------------------------------------
// .global variables
//----------------------------------------------------------------------

// Timeout-Zeit for startphase or a Report
#define START_TIMEOUT 15

// State of the connection to the Siron message lib
// 0 - no connection
// 1 - connected

static zSHORT g_connected = 0;        // state of the connection
static zLONG  g_Task = 0;             // calling task
static LPLIBRARY g_hMsgLib;           // Message Lib

// ##########
// In Win31 wird ein gemeinsamer Datenbereich fuer alle Instanzen der
// DLL verwendet. Durch g_connected wird also nur einer einzigen Anwendung
// erlaubt, sich auf die Message Lib zu konnektieren.
// In OS/2 erhaelt jeder Prozess einen eigenen Datenbereich fuer globale
// Variablen. Durch g_connected wird jetzt nur verhindert, dass sich ein
// Prozess mehrmals auf die Message Lib konnektiert.
// Zusaetzlich findet in OS/2 eine Synchronisation des Workobjektes durch
// Mutex Semaphoren statt.
// ##########

// we have to store Username and Password for Siron
// the User is set per Process, but we have only one work entity to
// pass this data to the Siron Compiler. That means, the User is stored
// here and passed when starting a report.
static zCHAR  szUsername[ 33 ] = "Anwender";
static zCHAR  szPassword[ 33 ] = "Kennwort";


//./ ADD NAME=ZSiron...
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ZSiron...
//
//  PURPOSE:    Calls to the Siron operations, which are dynamically
//              loaded from a DLL
//
//  PARAMETERS: See hzcmsgci.h
//
//  RETURNS:    "
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION ZSironShowPreview(
  zPCHAR pszDatei,
  zPCHAR pszInfo,
  zLONG  lFlags)
{
  return(pfnSironShowPreview(
            pszDatei,
            pszInfo,
            lFlags));
}

zOPER_EXPORT zSHORT OPERATION ZSironConnectRDT(
  T_MSG_CALLBACK pCallback)
{
  return(pfnSironConnectRDT(
            pCallback));
}

zOPER_EXPORT zSHORT OPERATION ZSironDisconnectRDT()
{
  return(pfnSironDisconnectRDT());
}

zOPER_EXPORT zSHORT OPERATION ZSironStartRDT(
  zPCHAR pszReport,
  zPCHAR pszLPLR,
  zPCHAR pszLOD,
  zLONG  lReadWrite,
  zLONG  lInputType,
  zVIEW  vAutoDesignedView,
  zPCHAR pszSourcePath,
  zPCHAR pszExecPath,
  zPCHAR pszUsername,
  zPCHAR pszPassword )
{
  return(pfnSironStartRDT(
           pszReport,
           pszLPLR,
           pszLOD,
           lReadWrite,
           lInputType,
           vAutoDesignedView,
           pszSourcePath,
           pszExecPath,
           pszUsername,
           pszPassword ));
}

zOPER_EXPORT zSHORT OPERATION ZSironStartWaitForRDT()
{
  return(pfnSironStartWaitForRDT());
}

zOPER_EXPORT zSHORT OPERATION ZSironStopWaitForRDT()
{
  return(pfnSironStopWaitForRDT());
}

zOPER_EXPORT zSHORT OPERATION ZSironConnect(
  zPCHAR pszName,
  T_MSG_CALLBACK pCallback)
{
  return(pfnSironConnect(
            pszName,
            pCallback));
}

zOPER_EXPORT zSHORT OPERATION ZSironDisconnect()
{
  return(pfnSironDisconnect());
}

zOPER_EXPORT zSHORT OPERATION ZSironStartWait(zLONG lTimeout)
{
  return(pfnSironStartWait(lTimeout));
}

zOPER_EXPORT zSHORT OPERATION ZSironStopWait()
{
  return(pfnSironStopWait());
}

zOPER_EXPORT zSHORT OPERATION ZSironRunQuery(
  zCPCHAR pszQueryName,
  zCPCHAR pszOutAscii,
  zCPCHAR pszOutGraphic,
  zCPCHAR pszUsername,
  zCPCHAR pszPassword,
  zPLONG  plQID,
  zCPCHAR pszLPLR_Name,
  zCPCHAR pszViewName,
  zLONG   lFlags)
{
  return(pfnSironRunQuery(
          pszQueryName,
          pszOutAscii,
          pszOutGraphic,
          pszUsername,
          pszPassword,
          plQID,
          pszLPLR_Name,
          pszViewName,
          lFlags));
}

zOPER_EXPORT zSHORT OPERATION ZSironAbortCompiler(zLONG lJobID)
{
  return(pfnSironAbortCompiler(lJobID));
}

zOPER_EXPORT zSHORT OPERATION ZSironRequestStatus(zLONG lJobID)
{
  return(pfnSironRequestStatus(lJobID));
}

zOPER_EXPORT zSHORT OPERATION ZSironGetLastStatus(
  zPLONG  plJobID,
  zPLONG  plFlags,
  zCHAR   szCompMsg,
  zLONG   lLenCompMsg)
{
  return(pfnSironGetLastStatus(
           plJobID,
           plFlags,
           szCompMsg,
           lLenCompMsg));
}

zOPER_EXPORT zSHORT OPERATION ZSironWaitForStatus(
  zPLONG  plJobID,
  zPLONG  plFlags,
  zLONG   lTimeout)
{
  return(pfnSironWaitForStatus(
           plJobID,
           plFlags,
           lTimeout));
}

zOPER_EXPORT zSHORT OPERATION ZSironSetMessageCallback(
  T_MSG_CALLBACK pCallback)
{
  return(pfnSironSetMessageCallback(pCallback));
}


//./ ADD NAME=fnLoadMsgLib
////////////////////////////////////////////////
//
//  ENTRY:      fnLoadMsgLib
//
//  PURPOSE:    Load the SIRDMSGC.DLL (Siron message lib) and gets
//              the proc addresses
//
//  PARAMETERS: zVIEW vSubtask      (I) Task-View der Anwendung
//
//  RETURNS:    0 - Loaded
//              else error
//
//////////////////////////////////////////////////////////////////////////////
//./ END + 2
static zSHORT fnLoadMsgLib( zVIEW vSubtask )
{
  // Load the DLL (must be in the path)

  g_hMsgLib = SysLoadLibraryWithErrFlag( vSubtask, "SIRDMSGC", 0 );
  if (g_hMsgLib == 0)
    return( -1 );

  pfnSironShowPreview = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironShowPreview" );
  pfnSironConnectRDT = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironConnectRDT" );
  pfnSironDisconnectRDT = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironDisconnectRDT" );
  pfnSironStartRDT = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironStartRDT" );
  pfnSironStartWaitForRDT = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironStartWaitForRDT" );
  pfnSironStopWaitForRDT = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironStopWaitForRDT" );
  pfnSironConnect = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironConnect" );
  pfnSironDisconnect = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironDisconnect" );
  pfnSironStartWait = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironStartWait" );
  pfnSironStopWait = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironStopWait" );
  pfnSironRunQuery = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironRunQuery" );
  pfnSironAbortCompiler = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironAbortCompiler" );
  pfnSironRequestStatus = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironRequestStatus" );
  pfnSironGetLastStatus = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironGetLastStatus" );
  pfnSironWaitForStatus = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironWaitForStatus" );
  pfnSironSetMessageCallback = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironSetMessageCallback" );
  pfnSironTerminate = (DLL_ENTRY_S)
    SysGetProc( g_hMsgLib, "CSironTerminate" );

  return( 0 );
}

//./ ADD NAME=fnFreeMsgLib
////////////////////////////////////////////////
//
//  ENTRY:      fnFreeMsgLib
//
//  PURPOSE:    Free the SIRDMSGC.DLL
//
//  PARAMETERS: -
//
//  RETURNS:    0 - Freed
//              else error
//
//////////////////////////////////////////////////////////////////////////////
//./ END + 2
static zSHORT fnFreeMsgLib( LPTASK lpTask )
{
  return( (zSHORT) SysFreeLibrary( lpTask, g_hMsgLib ) );
}

//./ ADD NAME=fnRequestReports
////////////////////////////////////////////////
//
//  ENTRY:      fnRequestReports
//
//  PURPOSE:    Request the semaphore for report work data
//
//  PARAMETERS: NONE
//
//  RETURNS:    0 - OK
//             zCALL_ERROR - Can't get semaphore
//
//////////////////////////////////////////////////////////////////////////////
//./ END + 2
static zSHORT fnRequestReports( void )
{
  return( 0 );
}

static zVOID fnReleaseReports( void )
{
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironConnect
//
// .Beschreibung:  Anmelden bei der Message Lib
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW vSubtask      (I) Task-View der Anwendung
//
// .Funktionswert: zSHORT, 0 - OK
//                         <0 - Error
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// - Globale Variable werden initialisiert
// - Es wird eine Work-View fuer die Uebergabe von Zeidon-Parametern
//   an Siron erzeugt
// - Die Zeidon-Anwendung wird bei der Message Lib angemeldet
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironConnect( zVIEW vSubtask )
{
  zVIEW vSirPara;
  zCHAR szSirwork[ 33 ];
  zSHORT nRC;


  /* pruefe, ob wir schon angemeldet sind */
  if ( g_connected != 0 )
  {
/*
      MessageSend( vSubtask, "", "kzsirrep",
                   "Only one connection to Siron allowed",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
*/
      TraceLineS( "(kzsirrep) Only one connection to Siron allowed", "");
      return( -1 );
  }

  // Load the Siron Msg Lib
  if ( fnLoadMsgLib( vSubtask ) != 0 )
  {
/*
      MessageSend( vSubtask, "", "kzsirrep",
                   "Can't load Siron message lib SIRDMSGC.DLL",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
*/
      TraceLineS( "(kzsirrep) Can't load Siron message lib SIRDMSGC.DLL", "");
      return( -1 );
  }

  // Lock work area
  if ( fnRequestReports( ) )
     return( -1 );

  /* Create Work View for Siron parameters */
  nRC = SfActivateSysEmptyOI( &vSirPara, "KZSIRRPO", vSubtask,
                              zSINGLE | zLEVEL_SYSTEM );
  if ( nRC < 0 )
  {
    fnReleaseReports( );
    return( nRC );
  }

  CreateEntity( vSirPara, "SirPara", zPOS_LAST );
  SetAttributeFromInteger( vSirPara, "SirPara", "vSubtask", (zLONG) vSubtask );
  SetAttributeFromString( vSirPara, "SirPara", "ViewName", "" );

// sprintf( szSirwork, "SIRON$%ld", SysGetTaskFromView( vSubtask ) );
   sprintf( szSirwork, "SIRON$RDT" );
   nRC = SetNameForView( vSirPara, szSirwork, vSubtask, zLEVEL_SYSTEM );
   // Unlock work area
   fnReleaseReports( );

   if ( nRC < 0 )
      return( nRC );

  /* Connect to the message lib */
  nRC = pfnSironConnect( szSirwork, NULL );
  if ( nRC < 0 )
    return( nRC );

  /* Start message receiver */
  pfnSironStartWait( 200L );
  g_connected = 1;
  g_Task = SysGetTaskFromView( vSubtask );
  return( 0 );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironDisconnect
//
// .Beschreibung:  Abmelden von der Message Lib
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW vSubtask      (I) Task-View der Anwendung
//
// .Funktionswert: zSHORT, 0 - OK
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
// - Die Anwendung wird bei der Message Lib abgemeldet
// - Die Work-View fuer die Uebergabe von Zeidon-Parametern
//   an Siron wird freigegeben.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironDisconnect( zVIEW vSubtask )
{
//BL  zVIEW vSirPara;
//BL  zCHAR szSirwork[ 33 ];
   zSHORT nRC;

   /* pruefe, ob wir schon angemeldet sind */
   if ( g_connected == 0 )
   {
       MessageSend( vSubtask, "", "kzsirrep",
                    "No connection to Siron",
                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
       return( -1 );
   }

   /* pruefe, ob wir die angemeldete Task sind */
   if ( g_Task != SysGetTaskFromView( vSubtask ) )
   {
       MessageSend( vSubtask, "", "kzsirrep",
                    "No connection to Siron",
                    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
       return( -1 );
   }

   /* Merke, das wir uns abmelden */
   g_Task =0;
   g_connected = 0;

   /* Stop message receiver */
   pfnSironStopWait( );

   /* disconect from message lib */
   nRC = pfnSironDisconnect( );

   /* free the Siron message lib */
   fnFreeMsgLib( );

   // Drop parameter view
   // Don't do it, cause perhaps a report is running from an earlier session
   // Do not drop the instance because this location is reached before
   // the Siron compiler has finished. If we drop the instance we'll get
   // error messages from the Siron compiler.
   // 11.09.2001, RG
// sprintf( szSirwork, "SIRON$RDT" );
// nRC = GetViewByName( &vSirPara,
//                      szSirwork,
//                      vSubtask,
//                      zLEVEL_SYSTEM );
// if ( nRC == zLEVEL_SYSTEM )
// {
//     DropObjectInstance( vSirPara );
// }

   return( 0 );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironTerminate
//
// .Beschreibung:  Stop the SironCompiler
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW vSubtask      (I) Task-View der Anwendung
//
// .Funktionswert: zSHORT, 0 - OK
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironTerminate( zVIEW vSubtask, zLONG lOptions )
{
  zSHORT nRC;

  /* pruefe, ob wir schon angemeldet sind */
  if ( g_connected == 0 )
  {
      MessageSend( vSubtask, "", "kzsirrep",
                   "No connection to Siron",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      return( -1 );
  }

  /* pruefe, ob wir die angemeldete Task sind */
  if ( g_Task != SysGetTaskFromView( vSubtask ) )
  {
      MessageSend( vSubtask, "", "kzsirrep",
                   "No connection to Siron",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      return( -1 );
  }

  /* Stop the Siron Compiler */
  nRC = pfnSironTerminate( lOptions );

  return( 0 );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironShowPreview
//
// .Beschreibung:  Show a file in the Siron Print Preview
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zPCHAR szFile       (I) Name eines Textfiles
//                                         oder einer Starview Datei
//                 zLONG  lZFlags      (I) Flags for output
//                         0x00000001L: format is Graphic file
//                                      else Text file
//                         0x00000002L: Orientation Landscape
//                                      else Portrait
//                         0x00000004L: Delete when preview closed
//                         0x00000008L: Print file, do not show
//
// .Funktionswert: zSHORT, 0 - OK
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
//  Eine Ausgabemeldung wird an den Preview gesendet.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironShowPreview( zVIEW vSubtask,
                  zPCHAR szFile,
                  zLONG lZFlags)
{
  zSHORT nRC;
  zCHAR szFilename[ zMAX_FILENAME_LTH ];

  /* pruefe, ob wir angemeldet sind */
/* Preview kann direkt gestartet werden, ohne auf Antwort zu warten
  if (g_connected == 0)
  {
      MessageSend( vSubtask, "", "kzsirrep",
                   "No connection to Siron",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      return( -1 );
  }
*/

  /* if no file name given, use env variable for file name */
  if ( szFile != NULL && *szFile != 0 )
    strcpy( szFilename, szFile );
  else
  {
  if (lZFlags && 0x00000001L)
    SysGetEnvVar( szFilename, "ADATEI3", sizeof( szFilename ) );
  else
    SysGetEnvVar( szFilename, "ADATEI1", sizeof( szFilename ) );
  }

  TraceLineS( "(kzsirrep) Start of Preview: ", szFilename);

  /* start it */
  nRC = pfnSironShowPreview(
          szFilename,
          "Zeidon Report",
          (T_Flags)lZFlags);
  if (nRC != 0)
  {
    TraceLineI( "(ziron) Preview returned ", nRC);
    MessageSend( vSubtask, "", "kzsirrep",
                 "Start of Preview failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  return( 0 );
}


//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironRunReport
//
// .Beschreibung:  Ausfuehren einer Abfrage ueber SirComp
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask      (I) Task-View der Anwendung
//                 zVIEW  vViewForReport(I) View, die in der Abfrage
//                                          ausgegeben werden soll
//                                          (oder NULL fuer Abfrage mit
//                                          Daten aus DB)
//                 zPCHAR szLPLR_Name (I) Name des LPLRs (oder "" fuer
//                                          aktuellen LPLR).
//                 zPCHAR szReportName (I) Name der Abfrage
//                 zPCHAR szOutText     (I) Name der Ausgabedatei fuer
//                                          Textausgabe (oder "")
//                 zPCHAR szOutGraphic (I) Name der Ausgabedatei fuer
//                                          grafische Ausgabe (im
//                                          Starview Meta Format)
//                                          oder ""
//                 zLONG lOptions       (I) 1 = Abfrage ausfuehren und
//                                              Preview aufschalten
//                                          9 = Abfrage ausfuehren und
//                                              Abfrage drucken
//                 zPLONG pJobID        (O) ID des Jobs fuer
//                                          Statusabfrage
//
// .Funktionswert: zSHORT, 0 - Abfrage wurde an Ausfuehrungseinheit
//                             uebergeben
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Ueber die Message Lib wird ein Ausfuehrungsbefehl an SirComp gesendet.
//
// Falls vViewForReport eine View enthaelt, dann wird dieser View ein
// systemweit eindeutiger Name gegeben. Dieser Name wird dann der
// Ausfuehrungseinheit mitgegeben.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironRunReport( zVIEW  vSubtask,
                zVIEW  vViewForReport,
                zPCHAR pszLPLR_Name,
                zPCHAR pszReportName,
                zPCHAR pszOutAscii,
                zPCHAR pszOutGraphic,
                zLONG  lOptions,
                zPLONG pJobID)
{
  zVIEW vSirPara;
  zVIEW vData;
  zCHAR szSirwork[ 33 ];
  zCHAR szReportName[ 33 ];
  zCHAR szReportPath[ zMAX_FILENAME_LTH ];
  zCHAR szViewName[ 33 ];
  zCHAR szCurrentTime[ 20 ];
//BL  zCHAR szTimeout[ 20 ];
//BL  zPCHAR pszTimeout;
  zSHORT nRC;
//BL  zLONG iValue;
  zLONG l;
  zBOOL bFree;
  zLONG lExecState;

  *pJobID =0;
  strcpy( szReportName, pszReportName );

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
       "No connection to Siron",
       zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  TraceLineI( "(kzsirrep) Try to start Report, Task = ", (zLONG) g_Task);

  // Lock work area
  if (fnRequestReports() != 0)
     return( -1 );

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) SironRunReport: Siron is not initialized!", "");
    fnReleaseReports();
    return( -1 );
  }

  // check, if another report is running
  // we can't start a second one, perhaps the report runs over an active
  // view, and we are not allowed to set another active report view!
  bFree = FALSE;
  for ( l = 0; l < START_TIMEOUT; l++ )
  {
    GetIntegerFromAttribute( &lExecState, vSirPara, "SirPara", "ExecState" );
    if (lExecState == 2L)
    {
/*
       fnReleaseReports();
       TraceLineI( "(kzsirrep) A report is running, trial no ", l);
       SysWait( 1000 );
       fnRequestReports();
       continue;
*/
       break;
    }

    if (lExecState == 1L)
    {
      // for Win: We have only one connection to Siron
      // we do not need to sync multiple connections
      bFree = TRUE;
/*
     SysGetDateTime( szCurrentTime, sizeof( szCurrentTime ) );
      GetStringFromAttribute( szTimeout, sizeof( szTimeout ), vSirPara, "SirPara", "TimeoutTime");

      // if report is in starting phase
      if ( zstrcmp(szCurrentTime, szTimeout) <= 0) {
         fnReleaseReports();
         TraceLineI( "(kzsirrep) A report is starting, trial no ", l);
         SysWait( 1000 );
         fnRequestReports();
         continue;
      }
      else
      {
         TraceLineS( "(kzsirrep) Report discarded, cause it was not started before Timeout: ", szCurrentTime);
         bFree = TRUE;
      }
*/
      break;
    }
    bFree = TRUE;
    break;
  }

  if (!bFree)
  {
    TraceLineS( "(kzsirrep) Wait until another report is activated", "");
    SysWait(5000);
    bFree = TRUE;
  }
/*
  if ( bFree == FALSE )
  {
      MessageSend( vSubtask, "", "kzsirrep",
                   "Another report is still running!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
     fnReleaseReports();
     return( -1 );
  }
*/
  SetAttributeFromInteger( vSirPara, "SirPara", "ExecState", 0L );

  /* check the parameters */
  if (pszReportName == NULL || pszReportName[ 0 ] == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "Report name is missing!",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    fnReleaseReports();
    return( zCALL_ERROR );
  }

  /* Append LPLR meta path to report name */
  GetApplDirectoryFromView( szReportPath, vSubtask,
                            zAPPL_DIR_OBJECT, zMAX_FILENAME_LTH );
  strcat( szReportPath, szReportName);
  strcat( szReportPath, ".XSQ");

  /* Set LPLR name (if missing) */
  if (pszLPLR_Name == NULL || *pszLPLR_Name == 0)
  {
    AppRecord *pApp;

    SfGetApplicationForSubtask( &pApp, vSubtask );
    pszLPLR_Name = pApp->szName;
  }

    // drop old Named View, if it exists
// sprintf( szViewName, "SIRON$%ld$View", SysGetTaskFromView( vSubtask ) );
   sprintf( szViewName, "SIRON$RDT$View" );
   nRC = GetViewByName( &vData, szViewName, vSubtask, zLEVEL_SYSTEM );
   if ( nRC == zLEVEL_SYSTEM )
   {
      DropView( vData );
   // DropObjectInstance( vData );
   }

  /* is it a query over an active View?
  */
  if ( vViewForReport != NULL )
  {
    // copy data view for query
    nRC = ActivateOI_FromOI_ForTask( &vData,
                             vViewForReport, 0,
                             zLEVEL_SYSTEM /* | zREADONLY */ | zMULTIPLE);
    if ( nRC < 0 )
    {
      TraceLineI( "(kzsirrep) Start Query: Can't copy view, rc = ", nRC);
      fnReleaseReports();
      return( -1 );
    }

    TraceLineI( "(kzsirrep) Data object instance copied, Task = ", (zLONG) g_Task);


    /* give it a global name */
    nRC = SetNameForView( vData, szViewName, vSubtask, zLEVEL_SYSTEM );
   if ( nRC < 0 )
   {
      TraceLineI( "(kzsirrep) Start Query: Can't name copied view, rc = ", nRC);
      fnReleaseReports();
      return( -1 );
   }

   nRC = GetViewByName( &vData, szViewName, vSubtask, zLEVEL_SYSTEM );
   if ( nRC != zLEVEL_SYSTEM )
   {
      TraceLineI( "(kzsirrep) Start Query: Problem with named view, rc = ", nRC);
      fnReleaseReports();
      return( -1 );
   }
  }
  else
  {
    szViewName[ 0 ] = 0;
  }

  /* set parameter view */
  SetAttributeFromString( vSirPara, "SirPara", "LPLR_Name", pszLPLR_Name );
  SetAttributeFromString( vSirPara, "SirPara", "ViewName", szViewName );

  // set ExecState to starting and Timeout to now+START_TIMEOUT [s]
  SetAttributeFromInteger( vSirPara, "SirPara", "ExecState", 1L );
  SetAttributeFromInteger( vSirPara, "SirPara", "TaskID", SysGetTaskFromView( vSubtask ) );
 SysGetDateTime( szCurrentTime, sizeof( szCurrentTime ) );
  SetAttributeFromString( vSirPara, "SirPara", "CurrentTime", szCurrentTime );
  SetAttributeFromString( vSirPara, "SirPara", "TimeoutTime", szCurrentTime );
  SetAttributeFromString( vSirPara, "SirPara", "ErrorCode", "" );
/*
  iValue = START_TIMEOUT;
  nRC = AddToAttributeFromVariable( vSirPara, "SirPara", "TimeoutTime",
                                    &iValue, zTYPE_INTEGER, 20, "Second");
  if (nRC < 0)
  {
     TraceLineI( "(kzsirrep) Add Timeout failed, rc = ", (zLONG) nRC );
  }
*/
/*
  GetAddrForAttribute( &pszTimeout, vSirPara, "SirPara", "TimeoutTime" );
  UfAddToDateTime( (LPDATETIME) pszTimeout, START_TIMEOUT, zDT_SECOND );
*/
  TraceLineS( "(kzsirrep) Report started at ", szCurrentTime );
  GetStringFromAttribute( szCurrentTime, sizeof( szCurrentTime ), vSirPara, "SirPara", "TimeoutTime" );
//  TraceLineS( "(kzsirrep)        Timeout is ", szCurrentTime );

  TraceLineS( "(kzsirrep) LPLR = ", pszLPLR_Name );
  TraceLineS( "(kzsirrep) report = ", szReportPath );
  TraceLineS( "(kzsirrep) view = ", szViewName );

  /* start it */
  nRC = pfnSironRunQuery( szReportPath,
                          pszOutAscii,
                          pszOutGraphic,
                          szUsername,
                          szPassword,
                          (T_JobID *) pJobID,
                          pszLPLR_Name,
                          szViewName,
                          (T_Flags) lOptions );
  if ( nRC )
  {
    TraceLineI( "(ziron) SironRunReport returned ", nRC );
    MessageSend( vSubtask, "", "kzsirrep",
                 "Start of Report failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    fnReleaseReports( );
    return( -1 );
  }

  // Unlock work area
  fnReleaseReports( );
  TraceLineI( "(kzsirrep) Report started, Task = ", (zLONG) g_Task );

  *pJobID = 1L;
  return( 0 );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironRunQuery
//
// .Beschreibung:  Senden einer Abfrage an SirComp
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zVIEW  vViewForQuery(I) View, die in der Abfrage
//                                         ausgegeben werden soll
//                                         (oder NULL fuer Abfrage mit
//                                         Daten aus DB)
//                 zPCHAR szLPLR_Name (I) Name des LPLRs (oder "" fuer
//                                         aktuellen LPLR).
//                 zPCHAR szQueryName (I) Name der Abfrage
//                 zPCHAR szOutAscii (I) Name der Ausgabedatei fuer
//                                         Textausgabe (oder "")
//                 zPCHAR szOutGraphic (I) Name der Ausgabedatei fuer
//                                         grafische Ausgabe (im
//                                         Starview Meta Format)
//                                         oder ""
//                 zLONG lOptions      (I) 1 = Preview aufschalten
//                 zLONG *pJobID       (O) ID des Jobs fuer
//                                         Statusabfrage
//
// .Funktionswert: zSHORT, 0 - Abfrage wurde an Ausfuehrungseinheit
//                             uebergeben
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Ueber die Message Lib wird ein Ausfuehrungsbefehl an SirComp gesendet.
//
// Falls vViewForQuery eine View enthaelt, dann wird dieser View ein
// systemweit eindeutiger Name gegeben. Dieser Name wird dann der
// Ausfuehrungseinheit mitgegeben.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironRunQuery( zVIEW  vSubtask,
               zVIEW  vViewForQuery,
               zPCHAR pszLPLR_Name,
               zPCHAR pszQueryName,
               zPCHAR pszOutAscii,
               zPCHAR pszOutGraphic,
               zLONG  lOptions,
               zLONG *pJobID)
{
// SironRunQuery is now replaced by SironRunReport
  return SironRunReport( vSubtask, vViewForQuery,
                         pszLPLR_Name, pszQueryName,
                         pszOutAscii, pszOutGraphic,
                         lOptions, pJobID);
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironAbortReport
//
// .Beschreibung:  Ausfuehren eines Reports in SirComp abbrechen
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zLONG  lJobID       (O) ID des Jobs von
//                                         SironStartReport()
//
// .Funktionswert: zSHORT, 0 - Nachricht wurde gesendet
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Ueber die Message Lib wird ein Abbruchsbefehl an SirComp gesendet.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironAbortReport( zVIEW vSubtask,
                 zLONG lJobID)
{
  zVIEW vSirPara;
  zCHAR szSirwork[ 33 ];
  zSHORT nRC;

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "No connection to Siron",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
        return( -1 );
  }

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) SironAbortReport: Siron is not initialized!", "");
    return( -1 );
  }

  /* check the parameters */
  if (lJobID == 0L)
  {
    TraceLineS( "(kzsirrep) Invalid JobID!", "");
    return( zCALL_ERROR );
  }

  nRC = pfnSironAbortCompiler((T_JobID) lJobID);
  if (nRC != 0)
  {
    TraceLineI( "(ziron) SironAbortReport returned ", nRC);
    MessageSend( vSubtask, "", "kzsirrep",
                 "Abort of Report failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

#ifdef TB_OS2
  SysWait( 1000 );

  // Lock work area
  if (fnRequestReports() != 0)
     return( -1 );

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) SironRunReport: Siron is not initialized!", "");
    fnReleaseReports();
    return( -1 );
  }

  // reset ExecState, just for the case, SirComp fails
  SetAttributeFromInteger( vSirPara, "SirPara", "ExecState", 0L );

  fnReleaseReports();
#endif

  return( 0 );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironAbortQuery
//
// .Beschreibung:  Ausfuehren einer Abfrage in SirComp abbrechen
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zLONG  lJobID       (O) ID des Jobs von
//                                         SironStartQuery()
//
// .Funktionswert: zSHORT, 0 - Nachricht wurde gesendet
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Ueber die Message Lib wird ein Abbruchsbefehl an SirComp gesendet.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironAbortQuery( zVIEW vSubtask,
                 zLONG lJobID)
{
// the old SironAbortQuery is replaced by SironAbortReport
  return SironAbortReport( vSubtask, lJobID );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironRequestStatus
//
// .Beschreibung:  Statusabfrage von SirComp
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zLONG  lJobID       (I) ID des Jobs von
//                                         SironStartQuery() fuer
//                                         Jobstatus oder 0 fuer
//                                         Compilerstatus
//
// .Funktionswert: zSHORT, 0 - Nachricht wurde gesendet
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Ueber die Message Lib wird eine Statusabfrage an SirComp gesendet.
//
// Falls eine JobID angegeben wurde, wird der Status des entsprechenden
// Jobs zurueckgegeben, andernfalls der Status von SirComp.
//
// Die Antwort von SirComp erfolgt asynchron, ueber die Callbackfunktion,
// die beim Anmelden mitgegeben wurde.
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironRequestStatus( zVIEW vSubtask,
                    zLONG lJobID )
{
  zVIEW vSirPara;
  zCHAR szSirwork[ 33 ];
  zSHORT nRC;
  zLONG lStatus;
  zLONG *plStatus = &lStatus;

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "No connection to Siron",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) Start Query: Siron is not initialized!", "");
    return( -1 );
  }

  /* check the parameters */
  if (lJobID == 0L)
  {
  }

  nRC = pfnSironRequestStatus( (T_JobID) lJobID );
  if (nRC != 0)
  {
    TraceLineI( "(ziron) SironRequestStatus returned ", nRC);
    MessageSend( vSubtask, "", "kzsirrep",
                 "Request for status failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  return( 0 );
}


//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironGetLastStatus
//
// .Beschreibung:  Liefert letzten Status, der von SirComp gemeldet wurde
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zLONG  *plJobID     (O) ID des Jobs fuer
//                                         Jobstatus oder 0 fuer
//                                         Compilerstatus
//                 zLONG  *plStatus    (O) Status (von Compiler oder Job)
//                 zCHAR  *pszErrorMsg (O) Puffer fuer Fehlermeldung
//                 zLONG  lErrorMsg    (O) Laenge des Puffers
//
// .Funktionswert: zSHORT, 0 - Status wurde zurueckgegeben
//                         sonst: Fehler (nicht angemeldet)
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Die letzte Statusmeldung von SirComp wird zurueckgegeben.
//
// Falls eine JobID angegeben wurde, wird der Status des entsprechenden
// Jobs zurueckgegeben, andernfalls der Status von SirComp.
//
// Die Antwort von SirComp erfolgt nach einer Anforderung, also
// SironRunQuery(), SironRequestStatus() oder SironAbortQuery() .
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironGetLastStatus( zVIEW vSubtask,
                    zLONG *plJobID,
                    zLONG *plStatus,
                    zCHAR *pszErrorMsg,
                    zLONG lErrorMsg )
{
  zVIEW vSirPara;
  zCHAR szSirwork[ 33 ];
  zSHORT nRC;

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "No connection to Siron",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
        return( -1 );
  }

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) Start Query: Siron is not initialized!", "");
    return( -1 );
  }

  nRC = pfnSironGetLastStatus( (T_JobID *) plJobID,
                             (T_Flags *) plStatus,
                             pszErrorMsg, lErrorMsg);
  if (nRC != 0)
  {
    TraceLineI( "(ziron) SironGetLastStatus returned ", nRC);
    return( -1 );
  }

  return( 0 );
}


//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironSetMessageCallback
//
// .Beschreibung:  Setze Callback Funktion fuer Siron Status Messages
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask    (I) Task-View der Anwendung
//                 zLONG  lCallback   (I) Pointer auf Callbackfunktion
//
// .Funktionswert: zSHORT, 0 - Callback wurde gesetzt
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironSetMessageCallback( zVIEW vSubtask,
                         zLONG lCallback )
{
  zSHORT nRC;

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "No connection to Siron",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  nRC = pfnSironSetMessageCallback( (T_MSG_CALLBACK) lCallback );

  return( nRC );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironWaitForStatus
//
// .Beschreibung:  Warte auf Statusantwort von SirComp
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask     (I) Task-View der Anwendung
//                 zLONG  *plJobID     (I) ID des Jobs von
//                                         SironStartQuery() fuer
//                                         Jobstatus oder 0 fuer
//                                         Compilerstatus
//                 zLONG  *plStatus    (O) Status
//                 zLONG  lTimeout     (I) Timeout fuer Warten
//
// .Funktionswert: zSHORT, 0 - Nachricht wurde gesendet
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
// Warte auf Status von Sircomp
//
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironWaitForStatus( zVIEW vSubtask,
                    zLONG *plJobID,
                    zLONG *plStatus,
                    zLONG lTimeout)
{
  zVIEW vSirPara;
  zCHAR szSirwork[ 33 ];
  zSHORT nRC;

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "No connection to Siron",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
        return( -1 );
  }

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) Start Query: Siron is not initialized!", "");
    return( -1 );
  }

  /* check the parameters */
//  if (lJobID == 0L)
//  {
//  }

  nRC = pfnSironRequestStatus( (T_JobID) 0L );
  if (nRC != 0)
  {
    TraceLineI( "(ziron) SironRequestStatus returned ", nRC);
    MessageSend( vSubtask, "", "kzsirrep",
                 "Request for status failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  nRC = pfnSironWaitForStatus( (T_JobID *) plJobID, (T_Flags *) plStatus, lTimeout);
  if (nRC != 0)
  {
    TraceLineI( "(ziron) SironWaitForStatus returned ", nRC);
    MessageSend( vSubtask, "", "kzsirrep",
                 "Request for status failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
    return( -1 );
  }

  return( 0 );
}

//----------------------------------------------------------------------
//
// .Funktionsname: >>>SironSetUser
//
// .Beschreibung:  Setze Benutzer fuer Siron-Zugriffe
//
// .Descriptor:    .........
//
// .Parameter:     zVIEW  vSubtask    (I) Task-View der Anwendung
//                 zLONG  lCallback   (I) Pointer auf Callbackfunktion
//
// .Funktionswert: zSHORT, 0 - Callback wurde gesetzt
//                         sonst: Fehler beim Ausfuehren
//
// ---------------------------------------------------------------------
//
// .Funktionsbeschreibung:
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SironSetUser( zVIEW vSubtask,
              zPCHAR pszUsername,
              zPCHAR pszPassword )
{
  zVIEW vSirPara;
  zCHAR szSirwork[ 33 ];
  zSHORT nRC;

  /* check, if we are connected */
  if (g_connected == 0)
  {
    MessageSend( vSubtask, "", "kzsirrep",
                 "No connection to Siron",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
        return( -1 );
  }

  // Lock work area
  if (fnRequestReports() != 0)
     return( -1 );

  /* Get paramter view */
  sprintf( szSirwork, "SIRON$RDT" );
  nRC = GetViewByName( &vSirPara,
                       szSirwork,
                       vSubtask,
                       zLEVEL_SYSTEM);
  if (nRC != zLEVEL_SYSTEM)
  {
    TraceLineS( "(kzsirrep) Start Query: Siron is not initialized!", "");
    fnReleaseReports();
    return( -1 );
  }

  // Set the values
  strcpy_s( szUsername, sizeof( szUsername ), pszUsername );
  strcpy_s( szPassword, sizeof( szPassword ), pszPassword );
  SetAttributeFromString( vSirPara,
                          "SirPara", "Username", pszUsername );
  SetAttributeFromString( vSirPara,
                          "SirPara", "Password", pszPassword );

  // Unlock work area
  fnReleaseReports();

  return( 0 );
}
