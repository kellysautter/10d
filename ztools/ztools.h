
#if !defined(AFX_ZTOOLS_H__BEB44AF9_3594_11D5_8398_00500418B3C2__INCLUDED_)
#define AFX_ZTOOLS_H__BEB44AF9_3594_11D5_8398_00500418B3C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const zLONG ALL = 0;
const zLONG XLP = 1;
const zLONG XPE = 2;
const zLONG XDM = 3;
const zLONG DTE = 4;
const zLONG XOD = 5;
const zLONG XWD = 6;
const zLONG XRP = 7;
const zLONG GENTYPE_LAST = 8;
const zLONG GENTYPE_FIRST = 1;

const zLONG CMD_OK                  = 0;
const zLONG CMD_MFC_LOAD_FAILED     = 1;
const zLONG CMD_INVALID_SYNTAX      = 2;
const zLONG CMD_INVALID_ARGCOUNT    = 3;
const zLONG CMD_HELP_SYNTAX         = 4;

const UINT  TERMINATE_CTRL_C    = 100;
const UINT  TERMINATE_SHUTDOWN  = 101;


void ShowSyntax();
void ShowVendor();

BOOL WINAPI ConsoleControlHandler( DWORD dwCtrlType );


int ZeidonApplicationProlog( zPVIEW vApp,
                             zCPCHAR pszApplicationName );

int
InvokeSystemCommand( zCPCHAR pszCommand,
                     CFile *fLog );

int ProcessGentype( zLONG nGenType,
                    CString strProject,
                    CString strLogFile,
                    CString strTEName,
                    CString strNetwork,
                    CString strRemoveReadOnly,
                    CString strKillOE
                    );

int ProcessXOD( zVIEW vSubtask,
                 zCPCHAR cTE_Name,
                 CFile *fLog,
                 zCPCHAR cNet );

int ProcessXWD( zVIEW vSubtask,
                 CFile *fLog,
                 zCPCHAR cRemReadOnly);

int ProcessDTE( zVIEW vSubtask,
                CFile *fLog );

int ProcessXDM( zVIEW vSubtask,
                CFile *fLog);

int ProcessXLP( zVIEW vSubtask,
                CFile *fLog);

int ProcessXPE( zVIEW vSubtask,
                CFile *fLog,
                zCPCHAR cRemReadOnly);

int ProcessXRP( zVIEW vSubtask,
                CFile *fLog,
                zCPCHAR cRemReadOnly);

BOOL KillObjectEngineProcess();

typedef zSHORT (POPERATION pfnConsoleMessageSend)
                        ( zVIEW    vMessage,
                          zVIEW    vSubtask,
                          zCPCHAR cpcMsgId,
                          zCPCHAR cpcTitle,
                          zCPCHAR cpcMsgText,
                          zLONG    lMsgType,
                          zSHORT   bBeep );

typedef zSHORT (POPERATION pfnConsoleMessagePrompt)
                        ( zVIEW    vMessage,
                          zVIEW    vSubtask,
                          zCPCHAR cpcMsgId,
                          zCPCHAR cpcTitle,
                          zCPCHAR cpcMsgText,
                          zSHORT   bBeep,
                          zSHORT   nButtons,
                          zSHORT   nDefaultButton,
                          zSHORT   nIcon );

typedef zSHORT (POPERATION pfnConsoleMessagePromptForInput)
                        ( zVIEW    vMessage,
                          zVIEW    vSubtask,
                          zCPCHAR cpcMsgId,
                          zCPCHAR cpcTitle,
                          zCPCHAR cpcMsgText,
                          zSHORT   bBeep,
                          zPCHAR   lpchReturnBuffer,
                          zUSHORT  usBufferLth );

typedef zSHORT (POPERATION pfnConsoleMessagePresent)
                        ( zVIEW    vMessage,
                          zVIEW    vSubtask,
                          zLONG    lEventType );

zSHORT ConsoleMessageSend( zVIEW    vMessage,
                       zVIEW    vSubtask,
                       zCPCHAR cpcMsgId,
                       zCPCHAR cpcTitle,
                       zCPCHAR cpcMsgText,
                       zLONG    lMsgType,
                       zSHORT   bBeep );

zSHORT ConsoleMessagePrompt( zVIEW    vMessage,
                         zVIEW    vSubtask,
                         zCPCHAR cpcMsgId,
                         zCPCHAR cpcTitle,
                         zCPCHAR cpcMsgText,
                         zSHORT   bBeep,
                         zSHORT   nButtons,
                         zSHORT   nDefaultButton,
                         zSHORT   nIcon );

zSHORT ConsoleMessagePromptForInput( zVIEW    vMessage,
                                 zVIEW    vSubtask,
                                 zCPCHAR cpcMsgId,
                                 zCPCHAR cpcTitle,
                                 zCPCHAR cpcMsgText,
                                 zSHORT   bBeep,
                                 zPCHAR   lpchReturnBuffer,
                                 zUSHORT  usBufferLth );

zSHORT ConsoleMessagePresent( zVIEW    vMessage,
                          zVIEW    vSubtask,
                          zLONG    lEventType );

int OverwriteMessageFunc( zVIEW vApp );

LPMQINTERFACE FindMQInterface( zVIEW vSubtask, LPTASK lpTask );


#endif // !defined(AFX_ZTOOLS_H__BEB44AF9_3594_11D5_8398_00500418B3C2__INCLUDED_)
