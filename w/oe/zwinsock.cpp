// MODULE NAME:  ZWINSOCK  -  Layer between Zeidon Core and Windows Sockets.
// *******************************************************************
// * Copyright © 1996 - 2016 QuinSoft, Inc. All rights reserved.     *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
//  AUTHOR:  D.G. Christensen
//  DATE:    3/22/96
//  API:     Windows 3.1
//           WinSock 1.1
//  ENVIRONMENT: Windows 3.1
//
/*
CHANGE LOG

2006.01.11  DKS  All
   Added a message to get the timestamp.

2002.04.15  DGC  All
   BoundsChecker reported a memory overwrite error when we called WSAStartup().
   This may not really be an error but an easy change is to add dummy bytes in
   front of and behind the data used by WSAStartup(), so I did this.

2002.02.20  DGC  All
   Added code to time gethostbyname() and connect().

2001.07.30  DGC  All
   Added Application Update code.

2001.07.05  DGC  All
   Changed return code of OpenConn to return -1 if connect failed.
   Made changes to coincide with changes in kzoeneta.c

2000.03.14  US   All
   Workaround of syncronizing the server access to avoid multithreading
   problems disabled, but not totally removed. It is possible to enable
   it again in zeidon ini with following entry:
   [zWinSock]
   SyncServerAccess=1
   default the syncronization is DISABLED!

2000.01.26  US   All
   Workaround to avoid multithreading problems. Only one client at a time.
   this workaround should be removed as soon as possible, but first the
   server and all DB-handlers have to be multithreading save!

1999.09.08  DGC  All
   Moved call to gethostbyname() into zNetGetHostAddress() so that it's only
   called when needed.

1999.08.01  DGC  All
   Rewrote winsock handler to eliminate MFC.

*/
#ifdef __UNIX__
   #include <pthread.h>
   #include <sys/types.h>
   #include <netinet/in.h>
   #include <netdb.h>
   #include <errno.h>
   #include <unistd.h>
   #include <sys/socket.h>
   #include <sys/wait.h>
   #include <sys/time.h>
   #include <stdio.h>

   #define  KZOESYS_INCL
   #include <kzoengaa.h>     // Object Services public prototypes

   // Some definitions to make cross-porting easier.
   #define Sleep              sleep
   #define SOCKET             int
   #define INVALID_SOCKET     -1
   #define SOCKET_ERROR       -1
   #define SOCKADDR_IN        struct sockaddr_in
   #define closesocket        close
   #define PHOSTENT           struct hostent *
   #define PSOCKADDR          struct sockaddr *
   #define THREADPROC         void *
   #define SHOWERROR( s )     fnShowError( s, __LINE__ )

   char    szlErrorTitle[] = "Sockets Error";
#else
   #ifndef __WIN32__
      #define __WIN32__       TRUE
   #endif

   #include <windows.h>
   #include <winsock.h>
   #include <process.h>
   #include <stdio.h>

   #define  KZOESYS_INCL
   #include <kzoengaa.h>     // Object Services public prototypes

   #define THREADPROC         unsigned __stdcall
   #define SHOWERROR( s )     fnShowError( s )
   #define socklen_t          int

   // Global variables.
   zCHAR  szlErrorTitle[] = "zWinSock Error";
   zULONG g_hListenThread        = 0;
   UINT   g_dwListenThreadID     = 0;

#endif

zBOOL  g_bListenThreadStarted = FALSE;
zBOOL  g_bClosingListenSocket = FALSE;

#define TRACE 1

// Following define whether certain operations are to be compiled.
#define WITH_BUFFEREND     FALSE
#define WITH_SENDPACKET    FALSE

#define BUFFER_LTH               32000
#define MAX_PENDING_CONNECTS     16

typedef struct SocketsStruct
{
   zCHAR  szNetworkName[ 60 ];
   zLONG  hMemHandle;         // Mem handle for the SocketsStruct.
   SOCKET sockListen;         // The listen socket.

#ifdef __UNIX__
   pthread_t  tidListen;      // TID for thread performing listen.
#else
   HWND   hwndMain;           // hwnd for the window accepting WinSocket msgs.
#endif

} SocketsRecord, * LPSOCKETS;

typedef struct ConnectionStruct
{
   SOCKET  sock;              // Socket to send/receive.
   zPCHAR  pchBuffer;         // Ptr to allocated buffer.
   zPCHAR  pchNextByte;       // Ptr to next unused byte in the buffer.
   zPCHAR  pchPeerName;       // Name/address of remote node during connection.
   zULONG  ulUnusedLth;       // # of bytes in buf read from net but not sent to app.
   zULONG  ulBufferLth;       // Physical length of buffer.
   zULONG  ulTotalBytes;      // Total # of bytes sent/received.
   zLONG   hMemHandle;        // Mem handle for connection record.
   zLONG   hBufferMem;        // Buffer handle.
   zLONG   hPeerNameMem;      // Peer name handle.
   zSHORT  nTraceLevel;       // Trace level for connection.
   zBOOL   bLastReceive;      // True if last send/receive command was receive.
} ConnectionRecord, * LPCONNECTION;

typedef struct DataPacketStruct
{
   char  cPacketType;
   long  lPacketData;
} zDataPacket, * LPPACKET;

struct ErrorMessageStruct
{
   int    nErrorNbr;
   zPCHAR pchErrorMessage;
};

#ifdef __UNIX__

/*
   fnShowError

   Trace the current WinSock error.
*/
extern int errno;

void
fnShowError( zPCHAR pchFuncName, int nLineNbr )
{
   char   szMsg[ 400 ];
   int    nErr = 0;
   int    k;

   sprintf( szMsg, "(zwinsock) Socket Error in func %s (line %d): (%d) %s\n",
            pchFuncName, nLineNbr, errno, strerror( errno ) );

   TraceLineS( szMsg, "" );
   perror( "Error from perror => " );

} // fnShowError

#else

/*
   fnShowError

   Trace the current WinSock error.
*/
void
fnShowError( zPCHAR pchFuncName )
{
   char   szMsg[ 400 ];
   int    nErr = WSAGetLastError( );
   int    k;

   struct ErrorMessageStruct gErrMsgs[] =
   {
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

   for ( k = 0; gErrMsgs[ k ].pchErrorMessage; k++ )
   {
      if ( gErrMsgs[ k ].nErrorNbr == nErr )
         break;
   }

   if ( gErrMsgs[ k ].pchErrorMessage )
   {
      wsprintf( szMsg, "(zwinsock) WinSock error Func = %s   %s %d",
                pchFuncName, gErrMsgs[ k ].pchErrorMessage, nErr );
   }
   else
   {
      wsprintf( szMsg, "(zwinsock) Unknown WinSock error Func = %s   %d",
                pchFuncName, nErr );
   }

   TraceLineS( szMsg, "" );

} // fnShowError

#endif // __UNIX__...else...

extern "C"
{

//***********************************************************************
//
//  ENTRY:       zNetSend
//
//  PURPOSE:     To send a block of data over the network.
//
//  DESCRIPTION: Zeidon Core call this operation when it needs to send data
//               over the network.
//
//  PARAMETERS:  ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//               ppvConnPtr     - Connection pointer to buffer created in
//                                zNetOpenConnection.
//               pchBuffer      - Pointer to send buffer.  If pchBuffer is 0,
//                                then flush (send) any local buffer.
//               uLth           - Length of buffer.  If uLth is 0, then
//                                pchBuffer points to a null-terminated str.
//               cDataType      - Specifies whether data is a string
//                                (zTYPE_STRING) or a blob (zTYPE_BLOB).
//                                NOTE: Ignored for WinSock implementation.
//
//  RETURNS:     0           - Data sent OK.
//               zCALL_ERROR - Error.
//
//***********************************************************************
zSHORT OPERATION
zNetSend( zPPVOID  ppHandle,
          zPPVOID  ppvConnPtr,
          zPCHAR   pchBuffer,
          zULONG   uLth,
          zCHAR    cDataType )
{
   LPSOCKETS      lpSockets = (LPSOCKETS) *ppHandle;
   LPCONNECTION   lpConn = (LPCONNECTION) *ppvConnPtr;
   int            iBytesToSend;
   int            iBytesSent;

   if ( lpConn->nTraceLevel > 1 )
      TraceLineI( "(zwinsock) zNetSend -- uLth = ", uLth );

   // If the last command was a receive (as opposed to a send) then we need
   // to reset the buffer for sending.
   if ( lpConn->bLastReceive )
   {
      if ( lpConn->nTraceLevel > 0 && lpConn->ulTotalBytes > 0 )
         TraceLineI( "(zwinsock) Total bytes received = ", lpConn->ulTotalBytes );

      lpConn->bLastReceive = FALSE;
      lpConn->pchNextByte  = lpConn->pchBuffer;
      lpConn->ulUnusedLth  = 0;
      lpConn->ulTotalBytes = 0;
   }

   // Check to see if we need to flush the current buffer.
   if ( (pchBuffer == 0 && lpConn->ulUnusedLth > 0) ||
        (lpConn->ulUnusedLth + uLth > lpConn->ulBufferLth) )
   {
      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineI( "(zwinsock) Sending buffer of length ",
                     lpConn->ulUnusedLth );
         TraceBuffer( "(zwinsock)",  lpConn->pchBuffer, lpConn->ulUnusedLth );
      }

      iBytesToSend = (int) lpConn->ulUnusedLth;

      do
      {
         iBytesSent = send( lpConn->sock, lpConn->pchBuffer, iBytesToSend, 0 );
         if ( iBytesSent == SOCKET_ERROR )
         {
            TraceLineS( "(zwinsock) send -- Error!", "" );
            SHOWERROR( "send" );
            return( zCALL_ERROR );
         }

         if ( lpConn->nTraceLevel > 1 )
            TraceLineI( "(zwinsock) Bytes sent for send ", iBytesSent );

         iBytesToSend -= iBytesSent;

      } while ( iBytesToSend > 0 );

      lpConn->ulTotalBytes += lpConn->ulUnusedLth;
      lpConn->ulUnusedLth = 0;
   }

   // If the incoming buffer is 0 then we only needed to flush the buffer.
   if ( pchBuffer == 0 )
      return( 0 );

   // If the incoming buffer length is greater than the send buffer we'll just
   // send the incoming buffer now.
   if ( uLth > lpConn->ulBufferLth )
   {
      if ( lpConn->nTraceLevel > 1 )
      {
         TraceLineI( "(zwinsock) Sending incoming buffer of length ", uLth );
         TraceBuffer( "(zwinsock)",  pchBuffer, uLth );
      }

      iBytesToSend = (int) uLth;

      do
      {
         iBytesSent = send( lpConn->sock, pchBuffer, iBytesToSend, 0 );
         if ( iBytesSent == SOCKET_ERROR )
         {
            TraceLineS( "(zwinsock) send -- Error!", "" );
            SHOWERROR( "send" );
            return( zCALL_ERROR );
         }

         if ( lpConn->nTraceLevel > 1 )
            TraceLineI( "(zwinsock) Bytes sent for send ", iBytesSent );

         iBytesToSend -= iBytesSent;

      } while ( iBytesToSend > 0 );

      lpConn->ulTotalBytes += uLth;

      return( 0 );
   }

   // Add the buffer data to the send buffer.
   if ( lpConn->nTraceLevel > 1 )
      TraceLineS( "(zwinsock) Adding bytes to send buffer", "" );

   zmemcpy( &lpConn->pchBuffer[ lpConn->ulUnusedLth ], pchBuffer,
            (unsigned int) uLth );
   lpConn->ulUnusedLth += uLth;

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
//               and set ppchReturnBuffer to point to the buffer.  The created
//               buffer need be valid only until the next zNet... call--
//               zNetReceive does not have to worry about keeping track of
//               data that has previously been retrieved.
//
//               uLth specifies the number of bytes Core is expecting.  If
//               there is not enough data from the network to satisfy Core,
//               then zNetReceive must return zCALL_ERROR.
//
//  PARAMETERS:  ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//               ppvConnPtr     - Connection pointer to buffer created in
//                                zNetOpenConnection.
//               ppchReturnBuff - Pointer to a pointer that will point to the
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
zSHORT OPERATION
zNetReceive( zPPVOID  ppHandle,
             zPPVOID  ppvConnPtr,
             zPPCHAR  ppchReturnBuffer,
             zULONG   uLth,
             zCHAR    cDataType )
{
   LPSOCKETS      lpSockets = (LPSOCKETS) *ppHandle;
   LPCONNECTION   lpConn = (LPCONNECTION) *ppvConnPtr;
   zULONG         ulUsed;
   unsigned       uMaxRead;

   if ( lpConn->nTraceLevel > 1 )
      TraceLineI( "(zwinsock) zNetReceive -- uLth = ", uLth );

   // Check the flag to see if the last send/receive command was a send.  If
   // the last command was a send, then the send buffer might need to be
   // flushed.
   if ( lpConn->bLastReceive == FALSE )
   {
      if ( lpConn->nTraceLevel > 1 )
         TraceLineS( "(zwinsock) Flushing send buffer", "" );

      if ( zNetSend( ppHandle, ppvConnPtr, 0, 0, 0 ) != 0 )
         return( zCALL_ERROR );

      lpConn->bLastReceive = TRUE;
      lpConn->ulTotalBytes = 0;

      if ( lpConn->nTraceLevel > 0 )
         TraceLineI( "(zwinsock) Total bytes sent = ", lpConn->ulTotalBytes );
   }

   // Check to see if we have enough "unused" bytes in the read buffer.
   // "Unused" bytes are bytes that have been received from the network but
   // haven't been sent back to core.
   if ( lpConn->ulUnusedLth >= uLth )
   {
      // We have enough "unused" bytes in the buffer to handle the request.
      // Set pointers and return.
      *ppchReturnBuffer = lpConn->pchNextByte;
      lpConn->pchNextByte += uLth;
      lpConn->ulUnusedLth -= uLth;

      return( 0 );
   }

   ulUsed = (zULONG) (lpConn->pchNextByte - lpConn->pchBuffer);

   // Check to see if there is enough space left in the buffer to hold the
   // data needed.
   if ( ulUsed + lpConn->ulUnusedLth + uLth > lpConn->ulBufferLth )
   {
      zLONG  hNewBuff;
      zPCHAR pchNewBuff = 0;

      // If we get here then there is not enough space at the end of the
      // buffer to hold the requested data.  This means we have to "move up"
      // the "unused" bytes to the beginning of the read buffer.  It's also
      // possible that the requested bytes is more than the buffer can hold.
      // In this case we'll allocate a new, bigger buffer before we move the
      // data around.

      if ( lpConn->nTraceLevel > 0 )
         TraceLineS( "(zwinsock) Trying to consolidate receive buffer", "" );

      // Check to see if the buffer is big enough to hold the necessary data.
      if ( lpConn->ulBufferLth < uLth )
      {
         if ( lpConn->nTraceLevel > 0 )
            TraceLineS( "(zwinsock) Allocating bigger buffer", "" );

         // We need to allocate a bigger buffer.
         hNewBuff = (zLONG) malloc( uLth + 1000 );
         pchNewBuff = (zPCHAR) hNewBuff;
         if ( pchNewBuff == 0 )
         {
            SysMessageBox( 0, szlErrorTitle, "Out of memory", -1 );
            return( zCALL_ERROR );
         }

         // Copy unused data from old buffer to new buffer.
         zmemcpy( pchNewBuff, lpConn->pchNextByte,
                  (unsigned int) lpConn->ulUnusedLth );

         // Free the old buffer.
         free( (zPVOID) lpConn->hBufferMem );

         // Set up connection info with new buffer.
         lpConn->ulBufferLth = uLth + 1000;
         lpConn->hBufferMem  = hNewBuff;
         lpConn->pchBuffer   = pchNewBuff;
      }
      else
      {
         // Copy the unused data to the beginning of the buffer.  This should
         // free up room at the end of the buffer to retrieve data off the network.
         zmemcpy( lpConn->pchBuffer, lpConn->pchNextByte,
                  (unsigned int) lpConn->ulUnusedLth );
      }

      lpConn->pchNextByte = lpConn->pchBuffer;
      ulUsed = 0;
   }

   // Determine how much space is left at the end of the buffer.  This is
   // the max number of bytes we can get without overflowing the buffer.
   uMaxRead = lpConn->ulBufferLth - (ulUsed + lpConn->ulUnusedLth);

   // Keep reading from the network until we have all the data we need.
   while ( uLth > lpConn->ulUnusedLth )
   {
      int iBytes;

      // Now retrieve data from the network, appending it to the end of the
      // data in the buffer.
      iBytes = recv( lpConn->sock, &lpConn->pchNextByte[ lpConn->ulUnusedLth ],
                     uMaxRead, 0 );
      if ( iBytes == SOCKET_ERROR || iBytes == 0 )
      {
         SHOWERROR( "recv" );
         TraceLineI( "(zwinsock) Error trying to extend buffer.  RC = ", iBytes );
         return( zCALL_ERROR );
      }

      if ( lpConn->nTraceLevel > 0 )
      {
         TraceLineI( "(zwinsock) Bytes read = ", iBytes );

         if ( lpConn->nTraceLevel > 1 )
         {
            TraceLineI( "(zwinsock) Tracing buffer ... ", iBytes );
            TraceBuffer( "(zwinsock)",
                         &lpConn->pchNextByte[ lpConn->ulUnusedLth ],
                         (zUSHORT) iBytes );
         }
      }

      lpConn->ulUnusedLth  += iBytes;
      lpConn->ulTotalBytes += iBytes;
      uMaxRead             -= iBytes;

      if ( lpConn->nTraceLevel > 0 )
      {
         if ( uLth > lpConn->ulUnusedLth )
            TraceLineS( "(zwinsock) Buffer still doens't have enough data", "" );
      }

   } // while...

   *ppchReturnBuffer = lpConn->pchNextByte;
   lpConn->pchNextByte += uLth;
   lpConn->ulUnusedLth -= uLth;

   return( 0 );

} // zNetReceive

u_short g_nApplPort = -1;

//***********************************************************************
//
//  ENTRY:       zNetOpenConnection
//
//  PURPOSE:     Called from Core to open a connection to the server node.
//               It is assumed that Core will then send/receive data.
//
//  DESCRIPTION: This call opens a socket to the server.
//
//               NOTE: Until further notice, the server address has been
//               hardcoded.
//
//  PARAMETERS:  ppHandle    - Network pointer to buffer created in
//                             zNetStartup.
//               ppvConnPtr  - Void pointer that has been initialized to
//                             null.  The value of this pointer is passed
//                             to later receive and send calls.  This
//                             allows connection data to be passed to these
//                             calls.
//               pchAddress  - Address of server.
//               vConnGrp    - View to OI with ConnectionGroup entity.
//                             ConnectionGroup contains info to make a
//                             connection between the current client and
//                             a server.  If vConnGrp is 0 then the
//                             info will be retrieved from ZEIDON.INI.
//
//  RETURNS:     0           - Connection opened successfully.
//               -1          - Couldn't connect to server.
//               zCALL_ERROR - Unrecoverable Error.
//
//***********************************************************************
zSHORT OPERATION
zNetOpenConnection( zPPVOID  ppHandle,
                    zPPVOID  ppvConnPtr,
                    zPCHAR   pchAddress,
                    zVIEW    lpView )
{
   LPSOCKETS      lpSockets = (LPSOCKETS) *ppHandle;
   LPCONNECTION   lpConn;
   SOCKET         sock = INVALID_SOCKET;
   SOCKADDR_IN    *DestSocket;
   PHOSTENT       pHost;
   zCHAR          szServerAddress[ 256 ];
   zPCHAR         pch;
   zLONG          hMem;
   u_long         lBlockOff = 0;
   u_short        nPort;
   zSHORT         nRC = zCALL_ERROR;

   // Set up a buffer to receive network traffic.
   hMem = (zLONG) calloc( 1, sizeof( ConnectionRecord ) );
   lpConn = (LPCONNECTION) hMem;
   if ( lpConn == 0 )
   {
      SysMessageBox( 0, szlErrorTitle, "Out of memory", -1 );
      return( zCALL_ERROR );
   }

   // Since we may manipulate the server address let's copy it to a temp var.
   strcpy_s( szServerAddress, zsizeof( szServerAddress ), pchAddress );

   // Check to see if the port number is part of the address.  If it is, then
   // copy it to szPort.
   pch = zstrchr( szServerAddress, ':' );
   if ( pch )
   {
      // Change ':' to null-term.
      *pch++ = 0;

      // Convert port number.
      nPort = (u_short) zatol( pch );
   }
   else
   if ( g_nApplPort != (u_short) -1 )
   {
      nPort = g_nApplPort;
   }
   else
   {
      zCHAR szPort[ 20 ];

      // The port value wasn't specified so get the default port from the Ini file.
      SysReadZeidonIni( -1, "[zWinSock]", "ApplPort", szPort, zsizeof( szPort ) );
      nPort = (u_short) zatol( szPort );
      g_nApplPort = nPort;
   }

   lpConn->hMemHandle = hMem;

   // Set up a buffer to receive network traffic.
   lpConn->hBufferMem = (zLONG) malloc( BUFFER_LTH );
   lpConn->pchBuffer  = (zPCHAR) lpConn->hBufferMem;
   if ( lpConn->pchBuffer == 0 )
   {
      SysMessageBox( 0, szlErrorTitle, "Out of memory", -1 );
      goto EndOfFunction;
   }

   lpConn->ulBufferLth = BUFFER_LTH;

   sock = socket( AF_INET, SOCK_STREAM, 0 );
   if ( sock == INVALID_SOCKET )
   {
      SHOWERROR( "socket" );
      SysMessageBox( 0, szlErrorTitle, "socket() failed", -1 );
      goto EndOfFunction;
   }

#ifdef __WIN32__
   if ( ioctlsocket( sock, FIONBIO, &lBlockOff ) == SOCKET_ERROR )
   {
      SHOWERROR( "ioctlsocket" );
      SysMessageBox( 0, szlErrorTitle, "ioctlsocket() failed", -1 );
      goto EndOfFunction;
   }
#endif

   while ( TRUE )
   {
      zCHAR szMsg[ 500 ];
      zLONG lStart, lEnd;

      lStart = SysGetTickCount();
      pHost = gethostbyname( szServerAddress );
      lEnd = SysGetTickCount();

      // Trace an error if over 5 seconds.
      if ( lEnd - lStart > 5 * zTICKS_PER_SECOND )
      {
         char sz[ 200 ];

         sprintf_s( sz, zsizeof( sz ), "%s took %lf seconds.", szServerAddress,
                    (double) (lEnd - lStart) / zTICKS_PER_SECOND );
         TraceLineS( "(zWinSock) WARNING gethostbyname() for ", sz );
      }

      if ( pHost )
         break;  // Got the info, so break.

      SHOWERROR( "gethostbyname" );
      sprintf_s( szMsg, zsizeof( szMsg ), "Couldn't resolve network address '%s'. Make sure the server "
                 "address is correct and that the Zeidon Server is running.",
                 szServerAddress );

#ifdef __UNIX__
      nRC = -1;
      goto EndOfFunction;
#else
      if ( SysMessageBox( lpView, szlErrorTitle, szMsg, MB_RETRYCANCEL ) == IDCANCEL )
      {
         nRC = -1;
         goto EndOfFunction;
      }
#endif
   }

   // Create the destination socket so we can call 'connect'.  Since the
   // destination socket structure has a variable length we'll temporarily
   // use our buffer that we created.
   DestSocket = (SOCKADDR_IN *) &lpConn->pchBuffer[ BUFFER_LTH / 2 ];
   zmemcpy( &(DestSocket->sin_addr), pHost->h_addr, pHost->h_length );
   DestSocket->sin_port   = htons( nPort );
   DestSocket->sin_family = AF_INET;

   // Try to connect to the server.  We'll keep trying until:
   //  1) We get through.
   //  2) User says to quit trying.
   while ( TRUE )
   {
      zCHAR szMsg[ 500 ];
      zLONG lStart, lEnd;

      lStart = SysGetTickCount();
      if ( connect( sock, (PSOCKADDR) DestSocket, sizeof( *DestSocket ) ) == 0 )
      {
         lEnd = SysGetTickCount();

         // Trace an error if over 5 seconds.
         if ( lEnd - lStart > 5 * zTICKS_PER_SECOND )
         {
            char sz[ 200 ];

            sprintf_s( sz, zsizeof( sz ), "%s took %lf seconds.", szServerAddress,
                       (double) ( lEnd - lStart ) / zTICKS_PER_SECOND );
            TraceLineS( "(zWinSock) WARNING connect() for ", sz );
         }

         break;  // Got a good connection, break loop.
      }

      SHOWERROR( "connect" );

      sprintf_s( szMsg, zsizeof( szMsg ), "Connection to server '%s' failed. Make sure the server "
                 "address is correct and that the server is running the Zeidon "
                 "Object Engine.", pchAddress );

#ifdef __UNIX__
      nRC = -1;
      goto EndOfFunction;
#else
      if ( SysMessageBox( lpView, szlErrorTitle, szMsg, MB_RETRYCANCEL ) == IDCANCEL )
      {
         nRC = -1;
         goto EndOfFunction;
      }
#endif
   }

   // Everything is OK, so set nRC.
   nRC = 0;

   // Init connection info.
   lpConn->sock         = sock;
   lpConn->pchNextByte  = lpConn->pchBuffer;
   lpConn->ulUnusedLth  = 0;
   lpConn->nTraceLevel  = NetGetTraceLevel( lpView, "zWinSock" );
   lpConn->bLastReceive = FALSE;

   if ( lpConn->nTraceLevel > 0 )
      TraceLineS( "(zwinsock) Connection to server made!", "" );

   // Set up a buffer to hold the peer name.
   lpConn->hPeerNameMem = (zLONG) malloc( zstrlen( szServerAddress ) + 1 );
   lpConn->pchPeerName = (zPCHAR) lpConn->hPeerNameMem;
   if ( lpConn->pchPeerName == 0 )
   {
      SysMessageBox( 0, szlErrorTitle, "Out of memory", -1 );
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

   strcpy_s( lpConn->pchPeerName, 256, szServerAddress );

EndOfFunction:
   if ( nRC == zCALL_ERROR )
   {
      // We're ending with an error, so clean up.
      if ( lpConn->hBufferMem )
         free( (zPVOID) lpConn->hBufferMem );

      if ( lpConn->hPeerNameMem )
         free( (zPVOID) lpConn->hPeerNameMem );

      if ( lpConn->hMemHandle )
         free( (zPVOID) lpConn->hMemHandle );

      if ( sock != INVALID_SOCKET )
         closesocket( sock );

      lpConn->hBufferMem   = 0;
      lpConn->hPeerNameMem = 0;
      lpConn->hMemHandle   = 0;
   }
   else
      *ppvConnPtr = lpConn;

   return( nRC );

} // zNetOpenConnection

//***********************************************************************
//
//  ENTRY:       zNetCloseConnection
//
//  PURPOSE:     To close a connection to the server.
//
//  DESCRIPTION: This call closes the socket that was opened by
//               zNetOpenConnection.
//
//  PARAMETERS:  ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//               ppvConnPtr     - Connection pointer to buffer created in
//                                zNetOpenConnection.
//
//  RETURNS:     0           - Connection closed successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
zSHORT OPERATION
zNetCloseConnection( zPPVOID  ppHandle,
                     zPPVOID  ppvConnPtr )
{
   LPSOCKETS      lpSockets = (LPSOCKETS) *ppHandle;
   LPCONNECTION   lpConn = (LPCONNECTION) *ppvConnPtr;

   // Check the flag to see if the last send/receive command was a send.  If
   // the last command was a send, then the send buffer might need to be
   // flushed.
   if ( lpConn )
   {
      if ( lpConn->bLastReceive == FALSE )
      {
         zNetSend( ppHandle, ppvConnPtr, 0, 0, 0 );

         if ( lpConn->nTraceLevel > 0 && lpConn->ulTotalBytes > 0 )
            TraceLineI( "(zwinsock) Total bytes sent = ", lpConn->ulTotalBytes );
      }
      else
      {
         if ( lpConn->nTraceLevel > 0 && lpConn->ulTotalBytes > 0 )
            TraceLineI( "(zwinsock) Total bytes received = ", lpConn->ulTotalBytes );
      }

      if ( closesocket( lpConn->sock ) == SOCKET_ERROR )
         SHOWERROR( "closesocket" );

      free( (zPVOID) lpConn->hBufferMem );
      free( (zPVOID) lpConn->hPeerNameMem );
      free( (zPVOID) lpConn->hMemHandle );

      *ppvConnPtr = 0;
   }

   return( 0 );

} // zNetCloseConnection


typedef struct tagSOCKTASK
{
   LPTASK    hTask;
   SOCKET    sock;

} zSOCKTASK;
typedef zSOCKTASK *LPSOCKTASK;

typedef struct ListenInfo *LPLISTENINFO;
struct ListenInfo
{
   LPTASK    hTask;
   LPSOCKETS lpSockets;
   zSHORT    nRC;
};

THREADPROC
fnProcessConnectionThreadProc( zPVOID p )
{
   LPSOCKTASK         pSockTask = (LPSOCKTASK) p;
// LPTASK             lpTask = (LPTASK) zGETPTR( pSockTask->hTask );
   LPTASK             lpTask = (LPTASK) SysGetPointerFromHandle( pSockTask->hTask );
   SOCKET             sock = pSockTask->sock;
   zVIEW              vSubtask = 0;
   long               lBlockOff = FALSE;
   zPVOID             pv;
   ConnectionRecord   cr = { 0 };
   zSHORT             nRC = zCALL_ERROR;
   zPVOID pp = 0;  // debug code

   delete( pSockTask );

#ifdef __WIN32__
   static HANDLE      hMutexObject = 0;
   static int         DoSync = 0;
   // Valid values for DoSync:  0 = zeidon.ini not read yet
   //                          -1 = dont use sync mechanisms
   //                           1 = use sync Mechanism (Mutex)

   if ( DoSync == 0 )
   {
      char szBuf[ 80 ];

      SysReadZeidonIni( -1, "[zWinSock]", "SyncServerAccess", szBuf, zsizeof( szBuf ) );
      DoSync = atoi( szBuf );
      if ( DoSync == 0 )
         DoSync = -1;   // atoi returns 0 if string was empty
   }

   if ( DoSync > 0 )
   {
      if ( hMutexObject == 0 )
         hMutexObject = CreateMutex( 0, FALSE, "WINSOCK_WORKAROUND" );

      if ( hMutexObject == 0 )
         goto EndOfFunction;

      WaitForSingleObject( hMutexObject, INFINITE );
   }
#else
   TraceLineI( "New thread task id = ", SysGetTaskID( ) );
#endif

   if ( RegisterZeidonApplication( &vSubtask, 0, 0, 0,
                                   "\\\\Zeidon System\\", 0, 0 ) != 0 )
      goto EndOfFunction;

   // Debug code - do a quick test to make sure that vSubtask is a valid
   // pointer.
   pp = (zPVOID) vSubtask->hTask;

   // Set up a buffer to receive network traffic.
   cr.hBufferMem = (zLONG) malloc( BUFFER_LTH );
   cr.pchBuffer = (zPCHAR) cr.hBufferMem;
   if ( cr.pchBuffer == 0 )
   {
      SysMessageBox( 0, szlErrorTitle, "Out of memory", -1 );
      goto EndOfFunction;
   }

   // Init connection info.
   cr.sock        = sock;
   cr.ulBufferLth = BUFFER_LTH;
   cr.pchNextByte = cr.pchBuffer;
   cr.ulUnusedLth = 0;
   cr.nTraceLevel = NetGetTraceLevel( vSubtask, "zWinSock" );

   if ( cr.nTraceLevel > 0 )
      TraceLineS( "(zwinsock) A message has been received from ",
                  cr.pchPeerName );

   pv = &cr;

   // If the trace level is > 0 then we'll call Core without a try block so
   // that we can catch crashes in the debugger.
   // In the production environment, TraceLevel should be set to zero so
   // that the errors will be handled and processing will continue.
#if 1
   nRC = NetProcessMessage( vSubtask, "zWinSock", &pv );
#else
   if ( cr.nTraceLevel > 0 )
      nRC = NetProcessMessage( vSubtask, "zWinSock", &pv );
   else
   {
      try
      {
         nRC = NetProcessMessage( vSubtask, "zWinSock", &pv );
      }
      catch ( ... )
      {
         zCHAR szMsg[ 200 ];
         zCHAR szDateTime[ 50 ];
         zCHAR szSysDateTime[ 30 ];

         SysGetDateTime( szSysDateTime );
         UfFormatDateTime( szDateTime, zsizeof( szDateTime ), szSysDateTime, "YYYY-MM-DD HH:MI:SS" );
         zsprintf( szMsg, "(%s) Client %s created an exception in NetProcessMessage",
                   szDateTime, cr.pchPeerName );
         TraceLineS( "EXCEPTION >>>>>>>>>>>>>>>>>>>>>>>> zWinsock: ", szMsg );
      // SysMessageBox( 0, "Exception in zWinsock", szMsg, -1 );
      }
   }
#endif

EndOfFunction:

   // Close the connection
   zNetCloseConnection( (zPPVOID) &pv, (zPPVOID) &pv );

   if ( vSubtask )
      UnregisterZeidonApplication( vSubtask );

   #ifdef __WIN32__
      if ( DoSync > 0 )
         ReleaseMutex( hMutexObject );
   #endif

   return( 0 );

} // fnProcessConnectionThreadProc

short g_nListenPort = -1;

// Following proc is used when user wants a listen started on the network.
// A thread is started that waits for a connection on the network.
THREADPROC
fnListenThreadProc( zPVOID p )
{
   zSHORT       nTraceLevel = 1; // NetGetTraceLevel( lpView, "zWinSock" );
   char         szPort[ 20 ];
   LPLISTENINFO lpListenInfo = (LPLISTENINFO) p;
   zVIEW        vSubtask = 0;
// SOCKET       NewSocket;
   LPTASK       hTask = lpListenInfo->hTask;

#ifdef __UNIX__

   if ( RegisterZeidonApplication( &vSubtask, 0, 0, 0,
                                   "\\\\Zeidon System\\", 0, 0 ) != 0 )
   {
      TraceLineS( "ERROR Registering app ERROR", "" );
      return( 0 );
   }

   TraceLineI( "New thread task id = ", SysGetTaskID( ) );

#endif

   // We'll assume that we return an error until proven otherwise.
   lpListenInfo->nRC = zCALL_ERROR;

   // Get port number.
   if ( g_nListenPort == -1 )
   {
      SysReadZeidonIni( -1, "[zWinSock]", "ListenPort", szPort, zsizeof( szPort ) );
      g_nListenPort = (short) zatol( szPort );
   }

   //
   // Create a TCP/IP stream socket to "listen" with
   //
   SOCKET  listenSocket;

   listenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
   if ( listenSocket == INVALID_SOCKET )
   {
      SHOWERROR( "socket" );
      SysMessageBox( 0, szlErrorTitle, "socket() failed", -1 );

      g_bListenThreadStarted = TRUE;
      return( 0 );
   }

   //
   // Fill in the address structure
   //
   SOCKADDR_IN saServer;

   saServer.sin_family = AF_INET;
   saServer.sin_addr.s_addr = INADDR_ANY;  // Let WinSock supply address
   saServer.sin_port = htons( (short) g_nListenPort );

   //
   // bind the name to the socket
   //
   int nRet;

   nRet = bind( listenSocket, (PSOCKADDR) &saServer, sizeof( struct sockaddr ) );
   if ( nRet == SOCKET_ERROR )
   {
      SHOWERROR( "bind" );
      SysMessageBox( 0, szlErrorTitle, "bind() failed", -1 );
      g_bListenThreadStarted = TRUE;
      closesocket( listenSocket );
      return( 0 );
   }

   nRet = listen( listenSocket, MAX_PENDING_CONNECTS );
   if ( nRet == SOCKET_ERROR )
   {
      SHOWERROR( "listen" );
      SysMessageBox( 0, szlErrorTitle, "listen() failed", -1 );
      g_bListenThreadStarted = TRUE;
      closesocket( listenSocket );
      return( 0 );
   }

   // Listen started OK so set return.
   lpListenInfo->nRC = 0;
   lpListenInfo->lpSockets->sockListen = listenSocket;

   // At this point everything's good so let's start waiting for connections.
   // Setting this flag allow the parent thread to continue.
   g_bListenThreadStarted = TRUE;

   TraceLineS( "(zWinSock) Waiting for client connections...", "" );

   zSOCKTASK    *pSockTask;

   // Loop until we the listen socket is closed.
   while ( TRUE )
   {
      if ( g_bClosingListenSocket )
         TraceLineS( "fnListenThreadProc trying to close ListenSocket", "" );

#ifdef __UNIX__x
      fd_set         ready;
      struct timeval sTimeVal;

      // Call select() to see if there are any connections waiting.
      FD_ZERO( &ready );
      FD_SET( listenSocket, &ready );

      if ( select( listenSocket + 1, &ready, 0, 0, 0 ) < 0 )
      {
         SHOWERROR( "select" );
         break;
      }

      // If socket doesn't have any connections waiting, then continue loop.
      if ( !FD_ISSET( listenSocket, &ready ) )
         continue;
#endif

      // On a successful connection, fnProcessConnectionThreadProc is
      // responsible for deleting pSockTask!
      pSockTask = new zSOCKTASK;
      pSockTask->hTask = hTask;
      pSockTask->sock = accept( listenSocket, 0, 0 );
      if ( pSockTask->sock == INVALID_SOCKET )
      {
         // It's possible we received the error because the socket was closed
         // by another thread.  In this case we'll ignore the error and just
         // return.
         if ( g_bClosingListenSocket )
         {
            // Turn the flag back off so we can re-use it.  This also tells
            // the closing thread that we're done.
            g_bClosingListenSocket = FALSE;
            delete( pSockTask );
            return( 0 );
         }

         SHOWERROR( "accept" );
         SysMessageBox( 0, szlErrorTitle, "accept(sock) failed for listen", -1 );
         closesocket( listenSocket );
         delete( pSockTask );
         return( 0 );
      }

      // Fire off another thread to handle this connection.
#ifdef __UNIX__
      pthread_t NewThread;

      if ( pthread_create( &NewThread, 0, fnProcessConnectionThreadProc,
                           (zPVOID) pSockTask ) != 0 )
      {
         TraceLineS( "Error starting process connection thread", "" );
         delete( pSockTask );
         return( 0 );
      }

      TraceLineI( "Thread created = ", NewThread );

#else
      HANDLE hConnThread;
      UINT   dwThreadID;

      hConnThread = (HANDLE) _beginthreadex( 0, 0,  // security, stacksize,
                                             fnProcessConnectionThreadProc, // start address
                                             (zPVOID) pSockTask, 0,  // arglist, initial state
                                             &dwThreadID );  // thread identifier
      if ( hConnThread )
         CloseHandle( (HANDLE) hConnThread );
      else
         delete( pSockTask );
#endif

   } // while ( TRUE )...

   if ( vSubtask )
      UnregisterZeidonApplication( vSubtask );

   return( 0 );
}

//***********************************************************************
//
//  ENTRY:       zNetListen
//
//  PURPOSE:     Sets up some scheme to listen for messages from other PC's.
//
//  DESCRIPTION: This call creates a socket to listen for connections from
//               other nodes on the network.
//
//  PARAMETERS:  ppHandle    - Network pointer to buffer created in
//                             zNetStartup.
//               pchAddress  - Address of server.
//
//  RETURNS:     0           - Listen created successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
zSHORT OPERATION
zNetListen( zPPVOID  ppHandle,
            zPCHAR   pchAddress,
            zVIEW    lpView )
{
   LPSOCKETS         lpSockets = (LPSOCKETS) *ppHandle;
// struct ListenInfo LisnInfo = { 0 };
   struct ListenInfo LisnInfo;

   // Set up info we need to pass to new thread.
   LisnInfo.hTask = lpView->hTask;
   LisnInfo.lpSockets = lpSockets;
   LisnInfo.nRC = 0;
   g_bListenThreadStarted = FALSE;

#ifdef __UNIX__

   zSHORT      nTraceLevel = NetGetTraceLevel( lpView, "zwinsock" );
   pthread_t   NewThread;

   if ( nTraceLevel > 1 )
   {
      TraceLineS( "(zwinsock) Starting listen", "" );
   }

   // Create a thread to perform the listen.  This keeps the current
   // thread from being blocked.
   if ( pthread_create( &NewThread, 0, fnListenThreadProc, &LisnInfo ) != 0 )
   {
      TraceLineS( "Error starting Listen thread", "" );
      return( zCALL_ERROR );
   }

   TraceLineI( "Thread created = ", NewThread );

   lpSockets->tidListen = NewThread;

#else

   // Start up a thread to create the socket and wait for connections.
   g_hListenThread = _beginthreadex( 0, 0,  // security, stacksize
                                     fnListenThreadProc,
                                     (zPVOID) &LisnInfo, 0, // arglist, initflag
                                     &g_dwListenThreadID );
   if ( g_hListenThread )
      CloseHandle( (HANDLE) g_hListenThread );

#endif

   // Now wait until the thread has finished its initialization.
   while ( g_bListenThreadStarted == FALSE )
      Sleep( 10 );

   return( LisnInfo.nRC );

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
//  PARAMETERS:  ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//
//  RETURNS:     0           - Listen closed successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
zSHORT OPERATION
zNetStopListen( zVIEW lpView, zPPVOID ppHandle )
{
   LPSOCKETS   lpSockets = (LPSOCKETS) *ppHandle;
   zSHORT      nTraceLevel = NetGetTraceLevel( lpView, "zWinSock" );
   zLONG       lSleepCnt;

   if ( nTraceLevel > 1 )
      TraceLineS( "(zwinsock) Stopping listen", "" );

   if ( lpSockets->sockListen == INVALID_SOCKET )
   {
      TraceLineS( "(zwinsock) No Listen socket!", "" );
      return( 0 ); // return 0 because nothing was done.
   }

#ifdef __UNIX__

   closesocket( lpSockets->sockListen );

#else

   // Set a flag indicating that we're closing the socket.  This allows the
   // thread handling the accept() call to ignore the error code returned
   // from accept( ).
   g_bClosingListenSocket = TRUE;

   if ( closesocket( lpSockets->sockListen ) == SOCKET_ERROR )
   {
      TraceLineS( "(zwinsock) Error shutting down listen socket", "" );
      SHOWERROR( "closesocket" );
      return( zCALL_ERROR );
   }

   // Now we'll wait until the thread handling the socket is finished.
   lSleepCnt = 0;
   while ( g_bClosingListenSocket )
   {
      lSleepCnt++;
      if ( lSleepCnt > 10000 )
      {
         SysMessageBox( 0, szlErrorTitle,
                        "NetStopListen wait for socket to finish ... TERMINATED", -1 );
         g_bClosingListenSocket = FALSE;
      }

      Sleep( 100 );
   }
#endif

   lpSockets->sockListen = INVALID_SOCKET;

   if ( nTraceLevel > 1 )
      TraceLineS( "(zwinsock) Listen stopped", "" );

   return( 0 );

} // zStopListen

//***********************************************************************
//
//  ENTRY:       zNetGetLocalHostAddress
//
//  PURPOSE:     To return the address of the local host.
//
//  DESCRIPTION: This call closes the socket set up to listen for connections
//               from other nodes on the network.
//
//  PARAMETERS:  ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//               ppvConnPtr     - Connection pointer.  If ppvConnPtr is 0,
//                                then we just want the local host address.
//                                If ppvConnPtr is not 0, then we want the
//                                address for one of the hosts on the conn.
//               cHost          - Used only if ppvConnPtr is not 0.  If cHost
//                                is 'R' then we return the address for the
//                                remote host.  If cHost is 'L' then we return
//                                the address for the local host.
//               pchHostAddress - Buffer to contain the host address.
//
//  RETURNS:     0           - Address retrieved OK.
//               zCALL_ERROR - Error.
//
//***********************************************************************
zSHORT OPERATION
zNetGetHostAddress( zVIEW   lpView,
                    zPPVOID ppHandle,
                    zPPVOID ppvConnPtr,
                    zCHAR   cHost,
                    zPCHAR  pchHostAddress )
{
   LPSOCKETS    lpSockets = (LPSOCKETS) *ppHandle;
   zSHORT       nTraceLevel = NetGetTraceLevel( lpView, "zWinSock" );

   if ( nTraceLevel > 1 )
      TraceLineS( "(zwinsock) Getting local host address.", "" );

   if ( ppvConnPtr && cHost == 'R' )
   {
      LPCONNECTION lpConn = (LPCONNECTION) *ppvConnPtr;

      // Check to see if we've already retrieved the name of the client.
      if ( lpConn->pchPeerName == 0 )
      {
         socklen_t          iLen;
         PHOSTENT           pHost;
         struct sockaddr_in stRmtAddr;

         // Get the host address of the client.
         iLen = sizeof( struct sockaddr );
         if ( getpeername( lpConn->sock, (struct sockaddr *) &stRmtAddr,
                           &iLen ) == SOCKET_ERROR )
         {
            SHOWERROR( "getpeername" );
            SysMessageBox( 0, szlErrorTitle, "getpeername() failed", -1 );
            return( zCALL_ERROR );
         }

         pHost = gethostbyaddr( (const char *) &stRmtAddr.sin_addr,
                                4, AF_INET );
         if ( pHost == 0 )
         {
            SHOWERROR( "gethostbyaddr" );
            SysMessageBox( 0, szlErrorTitle, "gethostbyaddr() failed", -1 );
            return( zCALL_ERROR );
         }

         // Set up peer name.
         lpConn->hPeerNameMem = (zLONG) malloc( zstrlen( pHost->h_name ) + 1 );
         lpConn->pchPeerName = (zPCHAR) lpConn->hPeerNameMem;
         if ( lpConn->pchPeerName == 0 )
         {
            SysMessageBox( 0, szlErrorTitle, "Out of memory", -1 );
            return( zCALL_ERROR );
         }

         strcpy_s( lpConn->pchPeerName, 256, pHost->h_name );
      }

      strcpy_s( pchHostAddress, 256, lpConn->pchPeerName );
   }
   else
   {
      gethostname( pchHostAddress, 300 );

      // For non-connections we get the address from the ZEIDON.INI
   // SysReadZeidonIni( -1, "[Zeidon]", "HostAddress", pchHostAddress );
   }

   if ( nTraceLevel > 1 )
      TraceLineS( "(zwinsock) Local host address = ", pchHostAddress );

   return( 0 );

} // zStopListen

//***********************************************************************
//
//  ENTRY:       zNetStart
//
//  PURPOSE:     To initialize a network protocol.
//
//  DESCR:       This operation allocates a buffer to keep track of WinSock
//               information and calls WSAStartup to start the network.
//
//  PARAMETERS:  pchNetworkName - Unique internal 'Zeidon' name of the
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
zSHORT OPERATION
zNetStart( zPCHAR pchNetworkName, zPPVOID  ppHandle )
{
   zLONG     hMemHandle;
   LPSOCKETS lpSockets;

#ifdef __WIN32__

   char      dummy1[ 10 ]; // BoundsChecker reported an error that said
   WSADATA   WSAData;      // WSAStartup() was writing past WSAData.  Dummy
   char      dummy2[ 10 ]; // vars are to protect stack.
   int       status;

   // Following is to stop compiler warnings:
   *dummy1 = *dummy2 = 0;

   *ppHandle = 0;

   if ( (status = WSAStartup( 0x0101, &WSAData )) != 0 )
   {
      char szTemp[ 80 ];

      sprintf_s( szTemp, zsizeof( szTemp ), "Error loading starting WinSock: %d", status );
      SysMessageBox( 0, szlErrorTitle, szTemp, -1 );
      return( zCALL_ERROR );
   }

// TraceLineX( "(zwinsock) Task ID             = ", (zLONG) SysGetTaskID( ) );
   TraceLineX( "(zwinsock) WinSock Version     = ", WSAData.wVersion );
   TraceLineX( "(zwinsock) WinSock HighVersion = ", WSAData.wHighVersion );
   TraceLineS( "(zwinsock) WinSock Description = ", WSAData.szDescription );
   TraceLineS( "(zwinsock) System status       = ", WSAData.szSystemStatus );
   TraceLineI( "(zwinsock) Max sockets         = ", WSAData.iMaxSockets );
   TraceLineI( "(zwinsock) Max UPD datagram    = ", WSAData.iMaxUdpDg );

#endif // __WIN32__

   hMemHandle = (zLONG) malloc( sizeof( SocketsRecord ) );
   *ppHandle = (zPCHAR) hMemHandle;
   if ( *ppHandle == 0 )
   {
      char szTemp[ 80 ];

#ifdef __WIN32__
      WSACleanup();
      wsprintf( szTemp, "Out of memory starting network '%s'", pchNetworkName );
      SysMessageBox( 0, szlErrorTitle, szTemp, -1 );
#endif

      return( zCALL_ERROR );
   }

   lpSockets = (LPSOCKETS) *ppHandle;
   strcpy_s( lpSockets->szNetworkName, 60, pchNetworkName );
   lpSockets->hMemHandle = hMemHandle;
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
//  PARAMETERS:  ppHandle       - Network pointer to buffer created in
//                                zNetStartup.
//
//  RETURNS:     0           - Network closed successfully.
//               zCALL_ERROR - Error.
//
//***********************************************************************
zSHORT OPERATION
zNetClose( zVIEW lpView, zPPVOID ppHandle )
{
   LPSOCKETS lpSockets = (LPSOCKETS) *ppHandle;

   if ( NetGetTraceLevel( lpView, "zWinSock" ) > 0 )
      TraceLineS( "(zwinsock) Shutting down network zWinSock", "" );

   if ( lpSockets->sockListen != INVALID_SOCKET )
      zNetStopListen( lpView, ppHandle );

#if !defined(  __UNIX__ )
   WSACleanup();
#endif

   // After the following call, lpSockets is no longer valid.
   free( (zPVOID) lpSockets->hMemHandle );

   *ppHandle = 0;

   return( 0 );

} // zNetClose

#if WITH_BUFFEREND

zSHORT OPERATION
zNetBufferEnd( zPPVOID  ppHandle,
               zPPVOID  ppvConnPtr )
{
   return zNetSend( ppHandle, ppvConnPtr, 0, 0, zTYPE_BLOB );
}

#endif // WITH_BUFFEREND

#if WITH_SENDPACKET

zSHORT OPERATION
zNetSendPacket( zPPVOID  ppHandle,
                zPPVOID  ppvConnPtr,
                zCHAR    cPacketType,
                zLONG    lPacketData )
{
   zDataPacket Packet;

   Packet.cPacketType = cPacketType;
   Packet.lPacketData = htonl( lPacketData );

   return zNetSend( ppHandle, ppvConnPtr, (zPCHAR) &Packet, zsizeof( Packet ),
                    zTYPE_BLOB );
}

zSHORT OPERATION
zNetReceivePacket( zPPVOID  ppHandle,
                   zPPVOID  ppvConnPtr,
                   zPCHAR   pcPacketType,
                   zPLONG   plPacketData )
{
   LPPACKET lpPacket;

   if ( zNetReceive( ppHandle, ppvConnPtr, (zPPCHAR) &lpPacket,
                     zsizeof( zDataPacket ), zTYPE_BLOB ) != 0 )
   {
      return( zCALL_ERROR );
   }

   *pcPacketType = lpPacket->cPacketType;
   *plPacketData = ntohl( lpPacket->lPacketData );

   return( 0 );
}

#endif // WITH_SENDPACKET

#ifdef __WIN32__
/*

   Creates an OI containing the directory of all files in the executable
   directory for an application.

*/
zSHORT OPERATION
GetDirectoryList( zPVIEW pvList, zVIEW vAppList )
{
   WIN32_FIND_DATA FileData;
   HANDLE          hFind;
   zVIEW           vList;
   zCHAR           szFileName[ zMAX_FILENAME_LTH + 1 ];

   if ( *pvList == 0 )
   {
      SfActivateSysEmptyOI( pvList, "KZFXFERO", 0, zMULTIPLE | zLEVEL_SYSTEM );
      CreateEntity( *pvList, "KZFXFERO", zPOS_FIRST );
      SetAttributeFromAttribute( *pvList, "KZFXFERO", "ApplicationName",
                                 vAppList, "APPLICATION", "APP_NAME" );
   }

   vList = *pvList;  // Just so it's easier.

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vAppList, "APPLICATION", "APP_ADOBIN" );
   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "*" );

   hFind = FindFirstFile( szFileName, &FileData );
   if ( hFind != INVALID_HANDLE_VALUE )
   {
      do
      {
         if ( ( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
         {
            zCHAR      szTimestamp[ 50 ];
            SYSTEMTIME lpTime;

            CreateEntity( vList, "File", zPOS_LAST );
            SetAttributeFromString( vList, "File", "FileName",
                                    FileData.cFileName );

            FileTimeToSystemTime( &FileData.ftLastWriteTime, &lpTime );
            sprintf_s( szTimestamp, zsizeof( szTimestamp ), "%d%02d%02d%02d%02d%02d%d",
                       lpTime.wYear, lpTime.wMonth, lpTime.wDay, lpTime.wHour,
                       lpTime.wMinute, lpTime.wSecond, lpTime.wMilliseconds );
            SetAttributeFromString( vList, "File", "ModDate", szTimestamp );
            SetAttributeFromInteger( vList, "File", "FileSize",
                                     FileData.nFileSizeLow );
         }

      } while ( FindNextFile( hFind, &FileData ) );

      FindClose( hFind );
   }

   // Last thing...sort files by name so we can more quickly compare them.
   OrderEntityForView( vList, "File", "FileName" );

   return( 0 );
}

/*

   RETURNS: 0 - Files shipped down.
            1 - Files shipped but more coming.

*/
zSHORT OPERATION
CompareFiles( zPVIEW pvList )
{
   static zLONG lMaxTotalFileSize = 0;

   zSHORT nRC = 0;
   zSHORT nServerRC;
   zSHORT nClientRC;
   zSHORT nFileCount = 0;
   zLONG  lTotalFileSize = 0;
   zLONG  lFileSize;
   zSHORT iNameIdx;
   zVIEW  vServerList = 0;
   zVIEW  vAppList;
   zVIEW  vClientList = *pvList;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];

   // Activate the object containing the app list.
   SysGetEnvVar( szFileName, "ZEIDON", zsizeof( szFileName ) );
   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "ZEIDON.APP" );
   SfActivateSysOI_FromFile( &vAppList, "KZAPPLOO", 0, szFileName, 0 );

   // Make sure the application exists for this server.
   if ( SetCursorFirstEntityByAttr( vAppList, "APPLICATION", "APP_NAME",
                                    vClientList, "KZFXFERO", "ApplicationName",
                                    0 ) < zCURSOR_SET )
   {
      SysMessageBox( *pvList, "Application Update Error",
                     "Error: Couldn't find application", 0 );
      return( zCALL_ERROR );
   }

   // Get the list of executable files for the app server.
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vAppList, "APPLICATION", "APP_NAME" );
   if ( lMaxTotalFileSize == 0 )
   {
      zCHAR szTemp[ 100 ];

      SysReadZeidonIni( -1, szFileName, "MaxTotalFileSize", szTemp, zsizeof( szTemp ) );
      if ( *szTemp )
         lMaxTotalFileSize = zatol( szTemp );

      if ( lMaxTotalFileSize <= 0 )
         lMaxTotalFileSize = 1000000;
   }

   TraceLineS( "Performing Application Update for app: ", szFileName );

   strcat_s( szFileName, zsizeof( szFileName ), "_ExecutableSourceList" );
   GetViewByName( &vServerList, szFileName, 0, zLEVEL_SYSTEM );
   if ( vServerList == 0 )
   {
      GetDirectoryList( &vServerList, vAppList );
      SetNameForView( vServerList, szFileName, 0, zLEVEL_SYSTEM );
   }

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vAppList, "APPLICATION", "APP_ADOBIN" );
   SysAppendcDirSep( szFileName );
   iNameIdx = (zSHORT) zstrlen( szFileName );

   nClientRC = SetCursorFirstEntity( vClientList, "File", 0 );
   nServerRC = SetCursorFirstEntity( vServerList, "File", 0 );
   while ( nServerRC >= zCURSOR_SET )
   {
      zCHAR szServerFile[ zMAX_FILENAME_LTH + 1 ];
      zCHAR szClientFile[ zMAX_FILENAME_LTH + 1 ];
      zSHORT nCmprResult;

      if ( nClientRC >= zCURSOR_SET )
      {
         GetStringFromAttribute( szServerFile, zsizeof( szServerFile ), vServerList, "File", "FileName" );
         GetStringFromAttribute( szClientFile, zsizeof( szClientFile ), vClientList, "File", "FileName" );

         nCmprResult = zstrcmpi( szServerFile, szClientFile );
      }
      else
         nCmprResult = -1;

      if ( nCmprResult != 0 )
      {
         // File names don't match.  Either a file exists on the server that
         // doesn't exist on the client or vice versa.  If the client file is
         // less than the server file then the file doesn't exist on the server.
         if ( nCmprResult > 0 )  // If client file is "lesser"
         {
            nClientRC = SetCursorNextEntity( vClientList, "File", 0 );
            continue;
         }

         // A file exists on the server that doesn't exist on the client.
         // Add a new entity to the client list.
         CreateEntity( vClientList, "File", zPOS_PREV );
         SetMatchingAttributesByName( vClientList, "File",
                                      vServerList, "File", zSET_ALL );
      }
      else
      if ( CompareAttributeToAttribute( vClientList, "File", "ModDate",
                                        vServerList, "File", "ModDate" ) == 0 )
      {
         // File mod dates match so we don't have to do anything.  Go on to next files.
         nClientRC = SetCursorNextEntity( vClientList, "File", 0 );
         nServerRC = SetCursorNextEntity( vServerList, "File", 0 );
         continue;
      }

      // Keep track of the number of files we are downloading.
      nFileCount++;

      GetStringFromAttribute( szFileName + iNameIdx, zsizeof( szFileName ) - iNameIdx, vClientList, "File", "FileName" );
      SetAttributeFromString( vClientList, "File", "Updated", "Y" );
      SetAttributeFromAttribute( vClientList, "File", "ModDate", vServerList, "File", "ModDate" );
      SetBlobFromFile( vClientList, "File", "Data", szFileName );

      // We only take so much at a time.
      GetIntegerFromAttribute( &lFileSize, vServerList, "File", "FileSize" );
      lTotalFileSize += lFileSize;
      if ( lTotalFileSize > lMaxTotalFileSize )
      {
         nRC = 1;
         break;
      }

      nClientRC = SetCursorNextEntity( vClientList, "File", 0 );
      nServerRC = SetCursorNextEntity( vServerList, "File", 0 );
   }

   // Now delete all entities that don't have the Updated flag set.
   while ( SetCursorFirstEntityByString( vClientList, "File", "Updated", "",
                                         0 ) >= zCURSOR_SET )
   {
      DeleteEntity( vClientList, "File", zREPOS_FIRST );
   }

   return( nRC );
}

zSHORT OPERATION
_zCall_CompareFiles( zVIEW  vArgs, zPVIEW pvViewList[] )
{
   zSHORT   ReturnValue;

   ReturnValue = CompareFiles( pvViewList[ 0 ] );
   SetAttributeFromInteger( vArgs, "Operation", "ReturnValue", ReturnValue );

   return( 0 );
}

zSHORT OPERATION
PerformApplicationUpdate( zVIEW lpView )
{
   zSHORT nRC;
   zSHORT iFileNameIdx;
   zSHORT nMore;
   zVIEW  vList = 0;
   zVIEW  vAppList;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zPCHAR pchAppName;

   // Activate the object containing the app list.
   SysGetEnvVar( szFileName, "ZEIDON", zsizeof( szFileName ) );
   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "ZEIDON.APP" );
   SfActivateSysOI_FromFile( &vAppList, "KZAPPLOO", lpView, szFileName, 0 );

   for ( nRC = SetCursorFirstEntity( vAppList, "APPLICATION", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vAppList, "APPLICATION", 0 ) )
   {
      zCHAR szSourceServer[ 256 ];

      GetAddrForAttribute( &pchAppName, vAppList, "APPLICATION", "APP_NAME" );

      // Check to see if a source server is specified for this app.  If not,
      // then skip it.
      SysReadZeidonIni( -1, pchAppName, "SourceServer", szSourceServer, zsizeof( szSourceServer ) );
      if ( szSourceServer[ 0 ] == 0 )
         continue;

      TraceLineS( "Performing application update for app: ", pchAppName );

      // Call an operation on the server to get the files.  Since it's possible
      // that the call will only return a partial list, we need to loop until
      // we get all the files.
      do
      {
         vList = 0;
         GetDirectoryList( &vList, vAppList );

         // Call the operation on the server that sends down the latest files.
         NetCallOperation( "zWinSock", szSourceServer, vList, FALSE,
                           "zWinSock", "CompareFiles", &nMore, "NpV", &vList );

         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vAppList, "APPLICATION", "APP_ADOBIN" );
         SysAppendcDirSep( szFileName );
         iFileNameIdx = (zSHORT) zstrlen( szFileName );

         TraceLineS( "===== Files that being downloaded are: ======", "" );
         for ( nRC = SetCursorFirstEntity( vList, "File", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vList, "File", 0 ) )
         {
            zCHAR szFileDateTime[ 40 ];

            GetStringFromAttribute( szFileName + iFileNameIdx, zsizeof( szFileName ) - iFileNameIdx, vList, "File", "FileName" );
            TraceLineS( "FileName: ", szFileName );
            WriteBlobToFile( vList, "File", "Data", szFileName );

            GetStringFromAttribute( szFileDateTime, zsizeof( szFileDateTime ), vList, "File", "ModDate" );
            SysSetFileTime( szFileName, szFileDateTime, 0 );
         }

         DropView( vList );

      } while ( nMore == 1 );

   } // For each APPLICATION...

   DropView( vAppList );
   return( 0 );
}

#if 0
__declspec (dllexport)
int CALLBACK dummy( int dummy2 )
{
   char abc[ 10 ];

   strcpy( abc, "abc" );
   return( 0 );
}

int WINAPI
DllMain( HANDLE hDLL,
         DWORD  dwReason,
         LPVOID lpReserved )
{

// SysMessageBox( "DGC", "zwinsock", 1 );
   return( TRUE );
}

int
main( void )
{
   return( 0 );
}
#endif

#endif // #ifdef __WIN32__

} // extern "C"
