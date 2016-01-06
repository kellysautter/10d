// MODULE NAME:  ZWINSOCK  -  Layer between Zeidon Core and Windows Sockets.
// ************************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved.   *
// * Confidential and Proprietary material subject to license -           *
// * do not reproduce or disclose. This material is an unpublished        *
// * work and is considered a trade secret belonging to the               *
// * copyright holder.                                                    *
// ************************************************************************
//  AUTHOR:  D.G. Christensen
//  DATE:    3/22/96
//  API:     Windows 3.1
//           WinSock 1.1
//  ENVIRONMENT: Windows 3.1
//
#include <windows.h>
#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRACELEVEL      1
#define zCALL_ERROR    -16

#include <zwinsckb.h>

#define BUFFER_LTH               32000
#define MAX_PENDING_CONNECTS     16
#define WSA_READ                 (WM_USER + 1)
#define WSA_ACCEPT               (WM_USER + 2)

typedef struct SocketsStruct SocketsRecord;
typedef struct SocketsStruct *LPSOCKETS;
struct SocketsStruct
{
   HGLOBAL hMemHandle;        // Mem handle for the SocketsStruct.
   HWND    hwndMain;          // hwnd for the window accepting WinSocket msgs.
   HWND    hwndTrace;         // hwnd for the trace window.
   SOCKET  sockListen;        // The listen socket.
};

typedef struct ConnectionStruct ConnectionRecord;
typedef struct ConnectionStruct *LPCONNECTION;
struct ConnectionStruct
{
   SOCKET  sock;              // Socket to send/receive.
   LPSTR   pszBuffer;         // Ptr to allocated buffer.
   LPSTR   pszNextByte;       // Ptr to next unused byte in the buffer.
   UINT    usBufferLth;       // Total # of bytes in buffer--used and unused.
   HGLOBAL hMemHandle;        // Mem handle for connection record.
   HGLOBAL hBufferMem;        // Buffer handle.
   BOOL    bLastReceive;      // True if last send/receive command was receive.
   LONG    lReceiveTickCount;
   LONG    lSendTickCount;
   int     nTraceLevel;
};

typedef struct ActivateStruct ActivateRecord;
typedef struct ActivateStruct *LPACTIVATE;
struct ActivateStruct
{
   SOCKET  sock;              // Socket to send/receive.
   LPSTR   pszBuffer;         // Ptr to allocated buffer.
   LPSTR   pszNextByte;       // Ptr to next unused byte in the buffer.
   UINT    usBufferLth;       // Total lth of buffer--used and unused.
   HGLOBAL hMemHandle;        // Buffer handle.
   LONG    lLineCount;        // Total lines sent/received.
   BOOL    bReceiveOK;        // FALSE until an OI is received OK.
};

struct ErrorMessageStruct
{
   int    iErrorNum;
   LPSTR  pszErrorMessage;
};

// Global variables.
char  szlErrorTitle[] = "zWinSock Error";
BOOL  gnNetworkCount  = 0;
HWND  ghwndTrace = 0;

int PASCAL
ReceiveBootstrapFile( LPVOID ppvConnPtr );

void TraceLineS( LPSTR psz1, LPSTR psz2 )
{
   char szMsg[ 256 ];

   strcpy( szMsg, psz1 );
   if ( psz2 && *psz2 )
      strcat( szMsg, psz2 );

   SendMessage( ghwndTrace, LB_ADDSTRING, 0, (LPARAM) szMsg );
}

void TraceLineI( LPSTR psz1, LONG l )
{
   char szL[ 10 ];

   _ltoa( l, szL, 10 );
   TraceLineS( psz1, szL );
}

void TraceLineX( LPSTR psz1, LONG l )
{
   char szL[ 10 ];

   _ltoa( l, szL, 16 );
   TraceLineS( psz1, szL );
}

void TraceBuffer( LPSTR pszPrefix, LPVOID pvBuffer, UINT uLth )
{
   LPSTR   pszBuffer = (LPSTR) pvBuffer;
   char    szHex[ 100 ];
   char    szAscii[ 100 ];
   char    sz[ 40 ];
   UINT    i;
   int     n;
   UINT    nLongestStr = 0;

   // szHex is a buffer containing the prefix and the hex representation of
   // 16 bytes from the buffer.  szAscii is a buffer containing the ASCII
   // representation of those same 16 bytes.

   szHex[ 0 ]   = 0;
   szAscii[ 0 ] = 0;

   // Loop through each byte in the buffer and display a block of 16 bytes
   // on each line in the trace window.
   for ( i = 0; i < uLth; i++ )
   {
      // If the index is a multiple of 16, then trace the szHex and szAscii
      // buffers (if they aren't empty) and reset them.
      if ( i % 16 == 0 )
      {
         if ( szHex[ 0 ] != 0 )
         {
            TraceLineS( szHex, szAscii );

            // Keep track of the longest hex string.
            if ( nLongestStr < strlen( szHex ) )
               nLongestStr = strlen( szHex );
         }

         // Add the offset counter to pszPrefix.
         sprintf( szHex, "%s(%4X) ", pszPrefix, i );
         strcpy( szAscii, "    " );
      }
      else
      if ( i % 8 == 0 )
      {
         // The index is a multiple of 8, so add a few spaces to the buffers
         // so make the trace a little easier to read.
         strcat( szHex, "   " );
         strcat( szAscii, " " );
      }

      // Convert the current byte to a 2-char hex number.
      _ltoa( (LONG) pszBuffer[ i ], sz, 16 );
      n = strlen( sz );
      if ( n < 2 )
      {
         strcat( szHex, "0" );
         n = 0;
      }
      else
         n -= 2;

      // Add the hex number to szHex.
      strcat( szHex, &sz[ n ] );
      strcat( szHex, " " );

      // If the current byte is a printable char, then copy it to the ASCII
      // buffer.  Otherwise add a "." to the buffer.
      if ( pszBuffer[ i ] >= 32 && pszBuffer[ i ] <= 126 )
      {
         n = strlen( szAscii );
         szAscii[ n++ ] = pszBuffer[ i ];
         szAscii[ n ]   = 0;
      }
      else
         strcat( szAscii, "." );

   } // for...

   // If szHex is not empty, then the buffers contain info that needs to be
   // traced.
   if ( szHex[ 0 ] != 0 )
   {
      // Pad the current szHex string with spaces so that it matches (in lth)
      // the longest szHex string that was traced.
      if ( strlen( szHex ) < nLongestStr )
      {
         for ( i = strlen( szHex ); i < nLongestStr; i++ )
            strcat( szHex, " " );
      }

      TraceLineS( szHex, szAscii );
   }

   return;

} // TraceBuffer

/*

   fnShowError

   Trace the current WinSock error.

*/
void fnShowError( LPSTR pszFuncName )
{
   int    iErr = WSAGetLastError( );
   int    i;
   char   szMsg[ 400 ];

   struct ErrorMessageStruct gErrMsgs[] = {
      WSAEINTR,              "WSAEINTR",
      WSAEBADF,              "WSAEBADF",
      WSAEACCES,             "WSAEACCES",
      WSAEFAULT,             "WSAEFAULT",
      WSAEINVAL,             "WSAEINVAL",
      WSAEMFILE,             "WSAEMFILE",
      WSAEWOULDBLOCK,        "WSAEWOULDBLOCK",
      WSAEINPROGRESS,        "WSAEINPROGRESS",
      WSAEALREADY,           "WSAEALREADY",
      WSAENOTSOCK,           "WSAENOTSOCK",
      WSAEDESTADDRREQ,       "WSAEDESTADDRREQ",
      WSAEMSGSIZE,           "WSAEMSGSIZE",
      WSAEPROTOTYPE,         "WSAEPROTOTYPE",
      WSAENOPROTOOPT,        "WSAENOPROTOOPT",
      WSAEPROTONOSUPPORT,    "WSAEPROTONOSUPPORT",
      WSAESOCKTNOSUPPORT,    "WSAESOCKTNOSUPPORT",
      WSAEOPNOTSUPP,         "WSAEOPNOTSUPP",
      WSAEPFNOSUPPORT,       "WSAEPFNOSUPPORT",
      WSAEAFNOSUPPORT,       "WSAEAFNOSUPPORT",
      WSAEADDRINUSE,         "WSAEADDRINUSE",
      WSAEADDRNOTAVAIL,      "WSAEADDRNOTAVAIL",
      WSAENETDOWN,           "WSAENETDOWN",
      WSAENETUNREACH,        "WSAENETUNREACH",
      WSAENETRESET,          "WSAENETRESET",
      WSAECONNABORTED,       "WSAECONNABORTED",
      WSAECONNRESET,         "WSAECONNRESET",
      WSAENOBUFS,            "WSAENOBUFS",
      WSAEISCONN,            "WSAEISCONN",
      WSAENOTCONN,           "WSAENOTCONN",
      WSAESHUTDOWN,          "WSAESHUTDOWN",
      WSAETOOMANYREFS,       "WSAETOOMANYREFS",
      WSAETIMEDOUT,          "WSAETIMEDOUT",
      WSAECONNREFUSED,       "WSAECONNREFUSED",
      WSAELOOP,              "WSAELOOP",
      WSAENAMETOOLONG,       "WSAENAMETOOLONG",
      WSAEHOSTDOWN,          "WSAEHOSTDOWN",
      WSAEHOSTUNREACH,       "WSAEHOSTUNREACH",
      WSAENOTEMPTY,          "WSAENOTEMPTY",
      WSAEPROCLIM,           "WSAEPROCLIM",
      WSAEUSERS,             "WSAEUSERS",
      WSAEDQUOT,             "WSAEDQUOT",
      WSAESTALE,             "WSAESTALE",
      WSAEREMOTE,            "WSAEREMOTE",
      WSASYSNOTREADY,        "WSASYSNOTREADY",
      WSAVERNOTSUPPORTED,    "WSAVERNOTSUPPORTED",
      WSANOTINITIALISED,     "WSANOTINITIALISED",
      0,                     0 };

   for ( i = 0; gErrMsgs[ i ].pszErrorMessage; i++ )
   {
      if ( gErrMsgs[ i ].iErrorNum == iErr )
         break;
   }

   if ( gErrMsgs[ i ].pszErrorMessage )
   {
      wsprintf( szMsg, "(zwinsock) WinSock error Func = %s   %s %d",
                pszFuncName, gErrMsgs[ i ].pszErrorMessage, iErr );
   }
   else
   {
      wsprintf( szMsg, "(zwinsock) Unknown WinSock error Func = %s   %d",
                pszFuncName, iErr );
   }


   TraceLineS( szMsg, "" );

} // fnShowError

//***********************************************************************
//
//  ENTRY:       zNetSend
//
//  PURPOSE:     To send a block of data over the network.
//
//  DESCRIPTION: Zeidon Core call this operation when it needs to send data
//               over the network.
//
//  PARAMETERS:  pszNetworkName - Unique internal 'Zeidon' name of the
//                                network.
//               ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//               ppvConnPtr     - Connection pointer to buffer created in
//                                zNetOpenConnection.
//               pszBuffer      - Pointer to send buffer.  If pszBuffer is 0,
//                                then flush (send) any local buffer.
//               uLth           - Length of buffer.  If uLth is 0, then
//                                pszBuffer points to a null-terminated str.
//               cDataType      - Specifies whether data is a string
//                                (zTYPE_STRING) or a blob (zTYPE_BLOB).
//                                NOTE: Ignored for WinSock implementation.
//
//  RETURNS:     0           - Data sent OK.
//               zCALL_ERROR - Error.
//
//***********************************************************************
int    PASCAL
zNetSend( LPSTR   pszNetworkName,
          LPVOID  * ppHandle,
          LPVOID  * ppvConnPtr,
          LPSTR   pszBuffer,
          UINT     uLth,
          char     cDataType )
{
   int   iBytesToSend;
   int   iBytesSent;
   LONG         lTickCount = GetTickCount( );
   LPSOCKETS    lpSockets = (LPSOCKETS) *ppHandle;
   LPCONNECTION lpConn = (LPCONNECTION) *ppvConnPtr;

   if ( lpConn->nTraceLevel > 1 )
   {
      TraceLineI( "(zwinsock) zNetSend -- uLth = ", uLth );
   }

   // If the last command was a receive (as opposed to a send) then we need
   // to reset the buffer for sending.
   if ( lpConn->bLastReceive )
   {
      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineS( "(zwinsock) Resetting buffer for sending", "" );
      }

      lpConn->bLastReceive = FALSE;
      lpConn->pszNextByte  = lpConn->pszBuffer;
      lpConn->usBufferLth  = 0;
   }

   // Check to see if we need to flush the current buffer.
   if ( ( pszBuffer == 0 && lpConn->usBufferLth > 0 ) ||
        ( lpConn->usBufferLth + uLth > BUFFER_LTH ) )
   {
      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineI( "(zwinsock) Sending buffer of length ",
                     lpConn->usBufferLth );
         TraceBuffer( "(zwinsock)",  lpConn->pszBuffer, lpConn->usBufferLth );
      }

      iBytesToSend = lpConn->usBufferLth;

      do
      {
         iBytesSent = send( lpConn->sock, lpConn->pszBuffer, iBytesToSend, 0 );
         if ( iBytesSent == SOCKET_ERROR )
         {
            TraceLineS( "(zwinsock) send -- Error!", "" );
            fnShowError( "send" );
            return( zCALL_ERROR );
         }

         if ( lpConn->nTraceLevel > 1 )
         {
            TraceLineI( "(zwinsock) Bytes sent for send ", iBytesSent );
         }

         iBytesToSend -= iBytesSent;

      } while ( iBytesToSend > 0 );

      lpConn->usBufferLth = 0;
   }

   // If pszBuffer is non-zero, then we need to add the buffer data to the
   // send buffer.
   if ( pszBuffer )
   {
      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineS( "(zwinsock) Adding bytes to send buffer", "" );
      }

      memcpy( &lpConn->pszBuffer[ lpConn->usBufferLth ], pszBuffer, uLth );
      lpConn->usBufferLth += uLth;
   }

   lpConn->lSendTickCount += GetTickCount( ) - lTickCount;

   return( 0 );

} // zNetSend

//***********************************************************************
//
//  ENTRY:       zNetReceive
//
//  PURPOSE:     To retrieve a block of data from the network.
//
//  DESCRIPTION: Zeidon Core call this operation when it is expecting data
//               from the network.
//
//               Core expects zNetReceive to create a buffer to hold the data
//               and set ppszReturnBuffer to point to the buffer.  The created
//               buffer need be valid only until the next zNet... call--
//               zNetReceive does not have to worry about keeping track of
//               data that has previously been retrieved.
//
//               uLth specifies the number of bytes Core is expecting.  If
//               there is not enough data from the network to satisfy Core,
//               then zNetReceive must return( zCALL_ERROR ).
//
//  PARAMETERS:  pszNetworkName - Unique internal 'Zeidon' name of the
//                                network.
//               ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//               ppvConnPtr     - Connection pointer to buffer created in
//                                zNetOpenConnection.
//               ppszReturnBuff - Pointer to a pointer that will point to the
//                                retrieved data.
//               uLth           - Number of bytes Core is expecting to receive.
//               cDataType      - Specifies whether data is a string
//                                (zTYPE_STRING) or a blob (zTYPE_BLOB).
//                                NOTE: Ignored for WinSock implementation.
//
//  RETURNS:     0           - Data received OK.
//               zCALL_ERROR - Error.
//
//***********************************************************************
int    PASCAL
zNetReceive( LPSTR    pszNetworkName,
             LPVOID  * ppHandle,
             LPVOID  * ppvConnPtr,
             LPSTR  * ppszReturnBuffer,
             UINT     uLth,
             char     cDataType )
{
   LONG         lTickCount = GetTickCount( );
   LPSOCKETS    lpSockets = (LPSOCKETS) *ppHandle;
   LPCONNECTION lpConn = (LPCONNECTION) *ppvConnPtr;

   if ( lpConn->nTraceLevel > 1 )
   {
      TraceLineI( "(zwinsock) zNetReceive -- uLth = ", uLth );
   }

   // Check the flag to see if the last send/receive command was a send.  If
   // the last command was a send, then the send buffer might need to be
   // flushed.
   if ( !lpConn->bLastReceive )
   {
      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineS( "(zwinsock) flushing send buffer", "" );
      }

      if ( zNetSend( pszNetworkName, ppHandle, ppvConnPtr, 0, 0, 0 ) != 0 )
         return( zCALL_ERROR );

      lpConn->bLastReceive = TRUE;

      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineS( "(zwinsock) Done flushing send buffer", "" );
      }
   }

   // If the buffer lth is 0, then we need to fill it up with network data.
   if ( lpConn->usBufferLth == 0 )
   {
      int iBytes;

      if ( lpConn->nTraceLevel > 0 )
      {
         TraceLineS( "(zwinsock) Filling empty receive buffer", "" );
      }

      iBytes = recv( lpConn->sock, lpConn->pszBuffer, BUFFER_LTH, 0 );
      if ( iBytes == SOCKET_ERROR || iBytes == 0 )
      {
         TraceLineS( "(zwinsock) Error filling receive buffer", "" );
         fnShowError( "recv" );
         return( zCALL_ERROR );
      }

      if ( lpConn->nTraceLevel > 0 )
      {
         TraceLineI( "(zwinsock) Bytes read = ", iBytes );

         if ( lpConn->nTraceLevel > 1 )
         {
            TraceLineS( "(zwinsock) Tracing buffer...", "" );
            TraceBuffer( "(zwinsock)",  lpConn->pszBuffer, iBytes );
         }
      }

      lpConn->pszNextByte = lpConn->pszBuffer;
      lpConn->usBufferLth = iBytes;
   }

   // It is possible that the incoming line didn't fit into the current buffer
   // because the buffer contained multiple lines of data.  This means that
   // we need to consolidate the data from buffer with data that still hasn't
   // been retrieved.
   if ( uLth > lpConn->usBufferLth )
   {
      int iBytes;

      if ( lpConn->nTraceLevel > 0 )
      {
         TraceLineS( "(zwinsock) Trying to consolidate receive buffer", "" );
      }

      // Copy the unused data to the beginning of the buffer.  This should
      // free up room at the end of the buffer to retrieve data off the network.
      memcpy( lpConn->pszBuffer, lpConn->pszNextByte, lpConn->usBufferLth );

      // Now retrieve data from the network, appending it to the end of the
      // data in the buffer.
      iBytes = recv( lpConn->sock, &lpConn->pszBuffer[ lpConn->usBufferLth ],
                     BUFFER_LTH - lpConn->usBufferLth, 0 );
      if ( iBytes == SOCKET_ERROR || iBytes == 0 )
      {
         fnShowError( "recv" );
         TraceLineS( "(zwinsock) Error trying to extend buffer", "" );
         return( zCALL_ERROR );
      }

      if ( lpConn->nTraceLevel > 0 )
      {
         TraceLineI( "(zwinsock) Bytes read = ", iBytes );

         if ( lpConn->nTraceLevel > 1 )
         {
            TraceLineS( "(zwinsock) Tracing buffer...", "" );
            TraceBuffer( "(zwinsock)",
                         &lpConn->pszBuffer[ lpConn->usBufferLth ], iBytes );
         }
      }

      lpConn->pszNextByte  = lpConn->pszBuffer;
      lpConn->usBufferLth += iBytes;

      // Check again to see if the buffer contains all the data.  If not,
      // set error and get out.
      if ( uLth > lpConn->usBufferLth )
      {
         TraceLineS( "(zwinsock) Error -- buffer can't hold data", "" );
         return( zCALL_ERROR );
      }
   }

   *ppszReturnBuffer = lpConn->pszNextByte;
   lpConn->pszNextByte += uLth;
   lpConn->usBufferLth -= uLth;

   lpConn->lReceiveTickCount += GetTickCount( ) - lTickCount;

   return( 0 );

} // zNetReceive

//***********************************************************************
//
//  ENTRY:       zNetListen
//
//  PURPOSE:     Sets up some scheme to listen for messages from other PC's.
//
//  DESCRIPTION: This call creates a socket to listen for connections from
//               other nodes on the network.
//
//  PARAMETERS:  pszNetworkName - Unique internal 'Zeidon' name of the
//                                network.
//               ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//
//  RETURNS:     0           - Listen created successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
int    PASCAL
zNetListen( LPSTR    pszNetworkName,
            LPVOID  * ppHandle )
{
   LPSOCKETS   lpSockets = (LPSOCKETS) *ppHandle;
   SOCKET      sock;
   SOCKADDR_IN LocalSock;  /* Local socket - internet style */
   char        szHostName[ 200 ];
   int         nTraceLevel = TRACELEVEL;

   if ( nTraceLevel > 1 )
   {
      TraceLineS( "(zwinsock) Starting listen", "" );
   }

   sock = socket( AF_INET, SOCK_STREAM, 0 );
   if ( sock == INVALID_SOCKET )
   {
      fnShowError( "socket" );
      SysMessageBox( 0, "socket( ) failed", szlErrorTitle, MB_OK );
      closesocket( sock );
      return( zCALL_ERROR );
   }

   // Create the local socket information.
   LocalSock.sin_family = AF_INET;
   gethostname( szHostName, sizeof( szHostName ) );
   LocalSock.sin_addr.s_addr = INADDR_ANY;
   LocalSock.sin_port = htons( 2000 );

   // Try to bind the socket.
   if ( bind( sock, (struct sockaddr FAR *) &LocalSock,
              sizeof( LocalSock ) ) == SOCKET_ERROR )
   {
      fnShowError( "bind" );
      MessageBox( 0, "bind(sock) failed", szlErrorTitle, MB_OK );
      closesocket( sock );
      return( zCALL_ERROR );
   }

   if ( listen( sock, MAX_PENDING_CONNECTS ) < 0 )
   {
      fnShowError( "listen" );
      MessageBox( 0, "listen(sock) failed", szlErrorTitle, MB_OK );
      closesocket( sock );
      return( zCALL_ERROR );
   }

   if ( WSAAsyncSelect( sock, lpSockets->hwndMain,
                        WSA_ACCEPT, FD_ACCEPT ) == SOCKET_ERROR )
   {
      fnShowError( "WSAAsyncSelect" );
      MessageBox( 0, "WSAAsyncSelect(sock) failed", szlErrorTitle, MB_OK );
      closesocket( sock );
      return( zCALL_ERROR );
   }

   lpSockets->sockListen = sock;

   if ( nTraceLevel > 1 )
   {
      TraceLineS( "(zwinsock) Listen started OK!", "" );
   }

   return( 0 );

} // zNetListen

//***********************************************************************
//
//  ENTRY:       zNetStopListen
//
//  PURPOSE:     To close a socket set up to listen.
//
//  DESCRIPTION: This call closes the socket set up to listen for connections
//               from other nodes on the network.
//
//  PARAMETERS:  pszNetworkName - Unique internal 'Zeidon' name of the
//                                network.
//               ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//
//  RETURNS:     0           - Listen closed successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
int    PASCAL
zNetStopListen( LPSTR    pszNetworkName,
                LPVOID  * ppHandle )
{
   LPSOCKETS   lpSockets = (LPSOCKETS) *ppHandle;
   int         nTraceLevel = TRACELEVEL;

   if ( nTraceLevel > 1 )
   {
      TraceLineS( "(zwinsock) Stopping listen", "" );
   }

   if ( lpSockets->sockListen == INVALID_SOCKET )
   {
      TraceLineS( "(zwinsock) No Listen socket!", "" );
      return( 0 ); // Return 0 because nothing was done.
   }

   if ( closesocket( lpSockets->sockListen ) == SOCKET_ERROR )
   {
      TraceLineS( "(zwinsock) Error shutting down listen socket", "" );
      fnShowError( "closesocket" );
      return( zCALL_ERROR );
   }

   lpSockets->sockListen = INVALID_SOCKET;

   if ( nTraceLevel > 1 )
   {
      TraceLineS( "(zwinsock) Listen stopped", "" );
   }

   return( 0 );

} // zStopListen

/*

   Dialog procedure to handle messages posted by WinSock stack.  Only used
   for sockets set up to Listen.

*/
LRESULT WINAPI fnWinsockDlgProc( HWND   hWnd,
                                 UINT   message,
                                 WPARAM wParam,
                                 LPARAM lParam )
{
   switch ( message )
   {
      case WM_CREATE:
         TraceLineI( "(zwinsock) Window created! hWnd = ", (LONG) hWnd );
         break;

      case WSA_READ:
         if ( WSAGETSELECTEVENT( lParam ) == FD_READ )
         {
            SOCKET sock;
            long   lBlockOff = FALSE;
            ConnectionRecord cr;
            LPVOID  pv;

            sock = (SOCKET) wParam;

            // We have received data from the client.  We must now
            // make the socket a blocking socket so that we can send AND
            // RECEIVE the data synchronously. ( If the sending socket is
            // not blocking, then the receiving socket is not blocking)
            //
            // To make the socket blocking we must turn off asynchronous
            // messages (via WSAASyncSelect) and then turn on blocking
            // (via ioctlsocket).
            if ( WSAAsyncSelect( sock, hWnd, 0, 0 ) == SOCKET_ERROR )
            {
               fnShowError( "WSAAsyncSelect" );
               closesocket( sock );
               return( 0 );
            }

            if ( ioctlsocket( sock, FIONBIO, &lBlockOff ) == SOCKET_ERROR )
            {
               fnShowError( "ioctlsocket" );
               MessageBox( 0, "ioctlsocket( ) failed", szlErrorTitle, MB_OK );
               closesocket( sock );
               return( 0 );
            }

            // We can now process the message synchronously.
            memset( &cr, 0, sizeof( ConnectionRecord ) );

            // Set up a buffer to receive network traffic.
            cr.hBufferMem = GlobalAlloc( GPTR, BUFFER_LTH );
            if ( cr.hBufferMem == 0 )
            {
               MessageBox( 0, "Out of memory", szlErrorTitle, MB_OK );
               return( 0 );
            }
            cr.pszBuffer = GlobalLock( cr.hBufferMem );

            // Init connection info.
            cr.sock        = sock;
            cr.pszNextByte = cr.pszBuffer;
            cr.usBufferLth = 0;
            cr.nTraceLevel = TRACELEVEL;

            if ( cr.nTraceLevel > 0 )
            {
               TraceLineS( "(zwinsock) A message has been received", "" );
            }

            pv = &cr;

            // HANDLING OF MESSAGE GOES HERE!!!!!!
            ReceiveBootstrapFile( &pv );

            GlobalUnlock( cr.hBufferMem );
            GlobalFree( cr.hBufferMem );

            closesocket( sock );
         }
         else
         {
            // Closing connection...
            TraceLineS( "(zwinsock) Closing connection.", "" );
         }

         return( 0 );

      case WSA_ACCEPT:
         if ( WSAGETSELECTERROR( lParam ) == 0 )
         {
            SOCKADDR_IN AcceptSocket;     // Accept socket addr - internet style
            int         iAcceptSocketLth; // Accept socket addr length
            SOCKET      sock;

            sock = (SOCKET) wParam;

            /*
            *   Accept the incoming connection.
            */
            iAcceptSocketLth = sizeof( AcceptSocket );
            sock = accept( sock, (struct sockaddr FAR *) &AcceptSocket,
                           (int FAR *) &iAcceptSocketLth );

            if ( sock < 0 )
            {
               fnShowError( "accept" );
               MessageBox( 0, "accept(sock) failed", szlErrorTitle, MB_OK );
               return( 0 ); // WSA_ACCEPT
            }

            /*
            *   Send main window a WSA_READ when either data is pending on
            *   the socket (FD_READ) or the connection is closed (FD_CLOSE)
            */
            if ( WSAAsyncSelect( sock, hWnd, WSA_READ,
                                 FD_READ | FD_CLOSE ) == SOCKET_ERROR )
            {
               fnShowError( "WSAAsyncSelect" );
               MessageBox( 0, "Select(sock) failed", szlErrorTitle, MB_OK );
               closesocket( sock );
               return( 0 );
            }

            TraceLineS( "(zwinsock) Connection accepted OK!", "" );
         }
         else
         {
            SOCKET sock = (SOCKET) wParam;

            fnShowError( "accept" );
            MessageBox( 0, "accept failed", szlErrorTitle, MB_OK );

            /*
            *   Cancel any further notifications.
            */
            WSAAsyncSelect( sock, hWnd, 0, 0 );
         }

         return( 0 ); // WSA_ACCEPT

   } // switch ( message )...

   return DefWindowProc( hWnd, message, wParam, lParam );

} // fnWinsockDlgProc

//***********************************************************************
//
//  ENTRY:       zNetStart
//
//  PURPOSE:     To initialize a network protocol.
//
//  DESCR:       This operation allocates a buffer to keep track of WinSock
//               information and calls WSAStartup to start the network.
//
//  PARAMETERS:  pszNetworkName - Unique internal 'Zeidon' name of the
//                                network.
//               ppHandle       - Void pointer that has been initialized to
//                                null.  The value of this pointer is passed
//                                to all later network calls.  This allows
//                                initialization data to be passed to later
//                                calls.
//
//  RETURNS:     0           - Network started OK.
//               zCALL_ERROR - Error starting network.
//
//***********************************************************************
int    PASCAL
zNetStart( LPSTR     pszNetworkName,
           LPVOID *   ppHandle,
           HINSTANCE hInstance,
           HWND      hwndTrace )
{
   WSADATA   WSAData;
   int       status;
   HGLOBAL   hMemHandle;
   LPSOCKETS lpSockets;
   HWND      hwnd;

   ghwndTrace = hwndTrace;
   *ppHandle = 0;

   if ( (status = WSAStartup( 0x0101, &WSAData ) ) != 0 )
   {
      char szTemp[80];

      wsprintf(szTemp, "Error loading starting WinSock: %d", status);
      MessageBox( 0, szTemp, szlErrorTitle, MB_OK );
      return( zCALL_ERROR );
   }

   TraceLineX( "(zwinsock) Task ID             = ", (LONG) hInstance );
   TraceLineX( "(zwinsock) WinSock Version     = ", WSAData.wVersion );
   TraceLineX( "(zwinsock) WinSock HighVersion = ", WSAData.wHighVersion );
   TraceLineS( "(zwinsock) WinSock Description = ", WSAData.szDescription );
   TraceLineS( "(zwinsock) System status       = ", WSAData.szSystemStatus );
   TraceLineI( "(zwinsock) Max sockets         = ", WSAData.iMaxSockets );
   TraceLineI( "(zwinsock) Max UPD datagram    = ", WSAData.iMaxUdpDg );

   if ( gnNetworkCount == 0 )
   {
      WNDCLASS  wc;

      memset( &wc, 0, sizeof( wc ) );
      wc.style         = 0;
      wc.lpfnWndProc   = fnWinsockDlgProc;
      wc.hInstance     = hInstance;
      wc.lpszClassName = "WinsockClientWindow";
      if ( !RegisterClass( &wc ) )
      {
         MessageBox( 0, "Error trying to create WinsockClientWindow class",
                     szlErrorTitle, MB_OK );
         WSACleanup( );
         return( zCALL_ERROR );
      }
   }

   gnNetworkCount++;

   hwnd = CreateWindow( "WinsockClientWindow",
                        "Winsock Client Window",
                        0,                         // Window style.
                        0, 0, 100, 100,            // Pos & size.
                        HWND_DESKTOP,              // Parent,
                        0,                         // Menu bar.
                        hInstance,                 //
                        0 );
   if ( hwnd == 0 )
   {
      MessageBox( 0, "Error create Winsock Client Window", szlErrorTitle, MB_OK );
      UnregisterClass( "WinsockClientWindow", hInstance );
      WSACleanup( );
      return( zCALL_ERROR );
   }

   hMemHandle = GlobalAlloc( GPTR, sizeof( SocketsRecord ) );
   if ( hMemHandle == 0 )
   {
      char szTemp[ 80 ];

      UnregisterClass( "WinsockClientWindow", hInstance );
      DestroyWindow( hwnd );
      WSACleanup( );
      wsprintf( szTemp, "Out of memory starting network '%s'", pszNetworkName );
      MessageBox( 0, szTemp, szlErrorTitle, MB_OK );
      return( zCALL_ERROR );
   }
   *ppHandle = GlobalLock( hMemHandle );

   lpSockets = *ppHandle;
   lpSockets->hMemHandle = hMemHandle;
   lpSockets->hwndMain   = hwnd;
   lpSockets->hwndTrace  = hwndTrace;
   lpSockets->sockListen = INVALID_SOCKET;

   return( 0 );

} // zNetStart

//***********************************************************************
//
//  ENTRY:       zNetClose
//
//  PURPOSE:     To shut down everything needed for winsock networking.
//
//  DESCRIPTION: This call closes the Listen socket and then calls WSACleanup
//               to close winsock stuff.
//
//  PARAMETERS:  pszNetworkName - Unique internal 'Zeidon' name of the
//                                network.
//               ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//
//  RETURNS:     0           - Network closed successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
int    PASCAL
zNetClose( LPSTR     pszNetworkName,
           LPVOID *  ppHandle,
           HINSTANCE hInstance )
{
   LPSOCKETS lpSockets = (LPSOCKETS) *ppHandle;

   if ( TRACELEVEL > 0 )
   {
      TraceLineS( "(zwinsock) Shutting down network zWinSock", "" );
   }

   WSACleanup( );
   DestroyWindow( lpSockets->hwndMain );

   gnNetworkCount--;

   if ( gnNetworkCount == 0 )
      UnregisterClass( "WinsockClientWindow", hInstance );

   // After the following call, lpSockets is no longer valid.
   GlobalUnlock( lpSockets->hMemHandle );
   GlobalFree( lpSockets->hMemHandle );

   *ppHandle = 0;

   return( 0 );

} // zNetClose
