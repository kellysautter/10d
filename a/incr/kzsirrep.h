/*
**----------------------------------------------------------------------
**
** .Name:          KZSIRREP.H
**
** .Change state:  02.04.2001
**
** .Description:   Access of Zeidon to Siron reports
**
**----------------------------------------------------------------------
** .Extended Description:
**
**  This module sends messages to the Siron compiler SIRCOMP.EXE.
**  SIRCOMP.EXE runs the reports for Zeidon applications and
**  returns a status message.
**
**
**  Interfaces:
**   (1) Zeidon Object Service (using Library KZOENGAA)
**   (2) Siron Message Lib (using Library SIRDMSGC)
**
**----------------------------------------------------------------------
** .Change notes:
**
** 2001.04.02  RG
**    inserted definition zSIRON_QUERY_EXCEL
** 2001.03.27  RG  R54667
**    Inserted missing definitions
** 2000.08.10  RG  Z2000
**    Removed compiler warnings
**
** from here old version
**
** 01A  =  new module
**         (13.06.1997) GT
** 01B  =  extern "C"
**         (15.10.1998) HH
** 01C  =  new operation: SironTerminate
**         (08.01.1999) RG
** 01D  =  insert zOPER_EXPORT to function prototypes
**         (10.04.1999) HH
**
**----------------------------------------------------------------------
*/

#ifndef _KZSIRREP_H
#define _KZSIRREP_H

// This header needs KZOENGAA.H, which is often loaded before
#ifndef  KZOENGAA_DEF
#include <kzoengaa.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif


/*----------------------------------------------------------------------
** .define
**----------------------------------------------------------------------
*/

/* if not used in siron, make compile environment compatible with Siron */
#ifndef TB_PC

#ifdef _MSC_VER
#define TB_WIN
#ifdef _WIN32
#define TB_NT
#endif
#else
#define TB_OS2
#endif

#endif


/*
** Flags for Reports
*/
#define zSIRON_QUERY_DATA     0x00000001L
#define zSIRON_QUERY_TEST     0x00000002L
#define zSIRON_QUERY_SYNTAX   0x00000004L
#define zSIRON_QUERY_EXCEL    0x00000009L
#define zSIRON_QUERY_PREVIEW  0x00000010L
#define zSIRON_QUERY_DELETE   0x00000020L
#define zSIRON_QUERY_CLOSE    0x00000040L

/*
** Flags for Compiler Status
*/
#define zSIRON_STATUS_WAITING 0x00000001L
#define zSIRON_STATUS_RUNNING 0x00000002L
#define zSIRON_STATUS_READY   0x00000004L
#define zSIRON_STATUS_ERROR   0x00000008L
#define zSIRON_STATUS_IDLE    0x00000010L
#define zSIRON_STATUS_ABORT   0x00000020L
#define zSIRON_STATUS_TIMEOUT 0x00000100L

/*
** Flags for Compiler Terminate
*/
#define zSIRON_TERM_AFTER_RUN 0x00000001L

/*
** Flags for Preview
*/
#define zSIRON_PREVIEW_ASCII     0x00000001L
#define zSIRON_PREVIEW_GRAPHIC   0x00000002L
#define zSIRON_PREVIEW_LANDSCAPE 0x00000004L
#define zSIRON_PREVIEW_DELETE    0x00000008L

/*
** Flags for RDT
*/
#define zSIRON_RDT_EMPTY      0x00000001L
#define zSIRON_RDT_REP        0x00000002L
#define zSIRON_RDT_AUTODESIGN 0x00000004L


/*----------------------------------------------------------------------
** .typedef
**----------------------------------------------------------------------
*/
#ifndef DLL_ENTRY
#ifdef TB_NT
#define DLL_EXPORT
#define DLL_ENTRY __stdcall
typedef void * (DLL_ENTRY *T_MSG_CALLBACK)( zLONG, zLONG);
#define PACKED

#elif defined (TB_WIN)
#define DLL_ENTRY _far _pascal _loadds
typedef void * (_far _pascal *T_MSG_CALLBACK)( zLONG, zLONG);
#define PACKED

#elif defined (TB_OS2)
#define DLL_ENTRY
typedef void * (* _Pascal T_MSG_CALLBACK)( zLONG, zLONG);
#endif
#endif


// typedefs for Siron prototypes
#ifdef TB_WIN
#define PDLL_ENTRY DLL_ENTRY *
#endif
#ifdef TB_OS2
#define PDLL_ENTRY * DLL_ENTRY zFAR
#endif

typedef short (PDLL_ENTRY PSOPER) ( );
#define DLL_ENTRY_S PSOPER


/*----------------------------------------------------------------------
** .prototypes
**----------------------------------------------------------------------
*/

// These are the Siron report operations
zOPER_EXPORT zSHORT OPERATION
SironWaitForStatus( zVIEW vSubtask0,
                    zPLONG lJobID1,
                    zPLONG lStatus2,
                    zLONG lTimeout3 );

zOPER_EXPORT zSHORT OPERATION
SironAbortReport( zVIEW vSubtask0,
                  zLONG lJobID1 );

zOPER_EXPORT zSHORT OPERATION
SironRunReport( zVIEW vSubtask0,
                zVIEW vViewForReport1,
                zPCHAR sLPLR_Name2,
                zPCHAR sReportName3,
                zPCHAR sOutText4,
                zPCHAR sOutGraphic5,
                zLONG  lOptions6,
                zPLONG lJobID7 );

zOPER_EXPORT zSHORT OPERATION
SironSetUser( zVIEW vSubtask0,
              zPCHAR sUsername1,
              zPCHAR sPassword2 );

zOPER_EXPORT zSHORT OPERATION
SironSetMessageCallback( zVIEW vSubtask0,
                         zLONG lCallback1 );

zOPER_EXPORT zSHORT OPERATION
SironGetLastStatus( zVIEW vSubtask0,
                    zPLONG lJobID1,
                    zPLONG lStatus2,
                    zPCHAR sErrorMsg3,
                    zLONG lLenErrorMsg4 );

zOPER_EXPORT zSHORT OPERATION
SironRequestStatus( zVIEW vSubtask0,
                    zLONG lJobID1 );

zOPER_EXPORT zSHORT OPERATION
SironAbortQuery( zVIEW vSubtask0,
                 zLONG lJobID1 );

zOPER_EXPORT zSHORT OPERATION
SironRunQuery( zVIEW vSubtask0,
               zVIEW vViewForQuery1,
               zPCHAR sLPLR_Name2,
               zPCHAR sQueryName3,
               zPCHAR sOutText4,
               zPCHAR sOutGraphic5,
               zLONG lOptions6,
               zPLONG lJobID7 );

zOPER_EXPORT zSHORT OPERATION
SironDisconnect( zVIEW vSubtask0 );

zOPER_EXPORT zSHORT OPERATION
SironTerminate( zVIEW vSubtask, zLONG lOptions );

zOPER_EXPORT zSHORT OPERATION
SironConnect( zVIEW vSubtask0 );

zOPER_EXPORT zSHORT OPERATION
SironShowPreview( zVIEW vSubtask0,
                  zPCHAR sFile1,
                  zLONG lZFlags2 );

// external prototypes for direct calls to Siron
// The Siron message lib is loaded dynamically. This allows us, to
// start Zeidon applications, if no Siron is installed, and present a
// correct error message when calling Siron while it is not available.
// The disadvantage is, that we need to define this prototypes for
// function pointers.
extern zOPER_EXPORT zSHORT OPERATION ZSironShowPreview(
  zPCHAR pszDatei,
  zPCHAR pszInfo,
  zLONG  lFlags);

extern zOPER_EXPORT zSHORT OPERATION ZSironConnectRDT(
  T_MSG_CALLBACK pCallback);

extern zOPER_EXPORT zSHORT OPERATION ZSironDisconnectRDT(
  void);

extern zOPER_EXPORT zSHORT OPERATION ZSironStartRDT(
  zPCHAR pszReport,
  zPCHAR pszLPLR,
  zPCHAR pszLOD,
  zLONG  lReadWrite,
  zLONG  lInputType,
  zVIEW  vAutoDesignedView,
  zPCHAR pszSourcePath,
  zPCHAR pszExecPath,
  zPCHAR pszUsername,
  zPCHAR pszPassword );

extern zOPER_EXPORT zSHORT OPERATION ZSironStartWaitForRDT(
  void);

extern zOPER_EXPORT zSHORT OPERATION ZSironStopWaitForRDT(
  void);

extern zOPER_EXPORT zSHORT OPERATION ZSironConnect(
  zPCHAR pszName,
  T_MSG_CALLBACK pCallback);

extern zOPER_EXPORT zSHORT OPERATION ZSironDisconnect(
  void);

extern zOPER_EXPORT zSHORT OPERATION ZSironStartWait(
  long lTime);

extern zOPER_EXPORT zSHORT OPERATION ZSironStopWait(
  void);

extern zOPER_EXPORT zSHORT OPERATION ZSironRunQuery(
  zCPCHAR pszQueryName,
  zCPCHAR pszOutAscii,
  zCPCHAR pszOutGraphic,
  zCPCHAR pszUsername,
  zCPCHAR pszPassword,
  zPLONG  plQID,
  zCPCHAR pszLPLR_Name,
  zCPCHAR pszViewName,
  zLONG   lFlags
);

extern zOPER_EXPORT zSHORT OPERATION ZSironAbortCompiler(
  zLONG  lJobID);

extern zOPER_EXPORT zSHORT OPERATION ZSironRequestStatus(
  zLONG  lJobID);

extern zOPER_EXPORT zSHORT OPERATION ZSironGetLastStatus(
  zPLONG  plJobID,
  zPLONG  Flags,
  zCHAR   szCompMsg,
  zLONG   lLenCompMsg
);

extern zOPER_EXPORT zSHORT OPERATION ZSironWaitForStatus(
  zPLONG  plJobID,
  zPLONG  plFlags,
  zLONG   lTimeout
);

extern zOPER_EXPORT zSHORT OPERATION ZSironSetMessageCallback(
  T_MSG_CALLBACK pCallback);

#ifdef __cplusplus
}
#endif

#endif
