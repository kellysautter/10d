/********************************************************************************************/
//
// File:            kzoengwa.h
// *******************************************************************
// * Copyright (c) 2001 QuinSoft Corporation and TONBELLER AG.       *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:
// Datum:           9. April 2001
// describtion:
//
//
/* Change log most recent first order:

   2001.4.9   TMV   2000
      moved resource constants to seperate file
      add functions to hide OE Service Window or make it appear in systray


*/
/*********************************************************************************************/

//int PASCAL WinMain( HANDLE, HANDLE, LPSTR, int );
HWND       InitInstance( HANDLE hInstance, int  nCmdShow );
BOOL       InitApplication( HANDLE hInstance );

long FAR PASCAL
MainWndProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
AboutDlgProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
LogonDlgProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
SetTraceDlgProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
LimitDlgProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
FindStringDlgProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
ShowDLLDlgProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
TraceListWndProc( HWND, unsigned, UINT, LONG );
long FAR PASCAL
LogTraceDlgProc( HWND, unsigned, UINT, LONG );
void fnProcessOENG_Message( HWND, unsigned, UINT, LONG );

// functions used for systray stuff
zBOOL TrayMessage( DWORD dwMessage, UINT uID, HICON hIcon, LPCSTR pszTip );
zBOOL SetupSysTray( HANDLE hInstance );
void DetachSysTray( );
void APIENTRY HandlePopupMenu( HWND hwnd, POINT point );

#if defined( zSQLSERVER )
int fnSqlServerErrHandler( DBPROCESS *,
                                int,
                                int,
                                int,
                                zPCHAR,
                                zPCHAR );
int fnSqlServerMsgHandler( DBPROCESS *,
                                DBINT,
                                DBSMALLINT,
                                DBSMALLINT,
                                zPCHAR );
#endif
