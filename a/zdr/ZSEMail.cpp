// ZSEM.cpp : Defines the initialization routines for the DLL.
//

#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDrvrOpr.h"

#include <winsock2.h> // includes windows.h
#include <time.h>
#include <iostream>
#include <io.h>
#include <tchar.h>
#include "zsemail.h"

#pragma warning(disable: 4996) // This function or variable may be unsafe

// Default empty constructor.
FormattedDate::FormattedDate( )
{
}

// Default empty destructor.
FormattedDate::~FormattedDate( )
{
}

// Returns _TCHAR * string formatted as follows:
// Date: Sun, 22 Feb, 2004 16:21:45 -0700
zBOOL
FormattedDate::GetFormattedDate( _TCHAR *ptcDateTime )
{
   SYSTEMTIME systemtime;
   SYSTEMTIME localtime;
// _TCHAR *ptcDateTime = new _TCHAR[ 128 ];
   _TCHAR *ptcMonth[] = { "No Month", "Jan", "Feb", "Mar",
                          "Apr", "May", "Jun", "Jul",
                          "Aug", "Sep", "Oct", "Nov", "Dec" };
   _TCHAR *ptcDay[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

// memset( ptcDateTime, 0, zsizeof( _TCHAR ) * 128 );
// "Date: Tue, 30 Dec 2003 23:59:00 +0000
   ptcDateTime[ 0 ] = 0;

   GetSystemTime( &systemtime );
   zBOOL bRC = SystemTimeToTzSpecificLocalTime( NULL, &systemtime, &localtime );

   _stprintf( ptcDateTime, _T( "Date: %s, %02d %s, %4d %02d:%02d:%02d -0700" ),
              ptcDay[ localtime.wDayOfWeek ], localtime.wDay,
              ptcMonth[ localtime.wMonth ], localtime.wYear,
              localtime.wHour, localtime.wMinute, localtime.wSecond );
// return( ptcDateTime );
   return( bRC );
}

zOPER_EXPORT zLONG OPERATION
CreateSmtpConnection( zPCHAR pchSmtpServer )
{
   Smtp *pSmtp = new Smtp;

   if ( pSmtp->CreateSmtpConnection( pchSmtpServer ) == FALSE )
   {
      delete( pSmtp );
      pSmtp = 0;
   }

   return( (zLONG) pSmtp );
}

zOPER_EXPORT zSHORT OPERATION
CreateSmtpMessage( zLONG  lSmtpConnection,
                   zPCHAR pchSmtpServer,
                   zPCHAR pchSender,
                   zPCHAR pchRecipient,
                   zLONG  lRecipientCnt,
                   zPCHAR pchSubject,
                   zLONG  lMimeType,      // 1 - text;  2 - html
                   zPCHAR pchBody,
                   zLONG  lHasAttachment, // 0 - No;  1 - Yes
                   zPCHAR pchAttachFile,
                   zPCHAR pchUserName,
                   zPCHAR pchPassword )
{
   TraceLine( "CreateSmtpMessage Server: %s   Sender: %s   Recipient: %s"
              "   Recipient Cnt: %d   Subject: %s   Mime Type: %d"
              "   User: %s   Password %s",
              pchSmtpServer, pchSender, pchRecipient, lRecipientCnt,
              pchSubject, lMimeType, pchUserName, "******" ); // pchPassword );
   Smtp *pSmtp = (Smtp *) lSmtpConnection;
   if ( pSmtp )
   {
      zPCHAR *ppchToArray = (zPCHAR *) new char[ sizeof( zPCHAR ) * lRecipientCnt ];
      if ( lRecipientCnt == 1 )
      {
         ppchToArray[ 0 ] = pchRecipient;
      }
      else
      {
         zPCHAR pch = pchRecipient;
         zLONG  lCnt = 0;

         while ( lCnt < lRecipientCnt )
         {
            ppchToArray[ lCnt ] = pch;
            pch += zstrlen( pch ) + 1;
         }
      }

      zSHORT nRC = pSmtp->CreateSmtpMessage( pchSmtpServer,
                                             pchSender,
                                             ppchToArray,
                                             lRecipientCnt,
                                             pchSubject,
                                             lMimeType, // text
                                             pchBody,
                                             lHasAttachment,
                                             pchAttachFile,
                                             pchUserName,
                                             pchPassword );
      delete [] ppchToArray;
      return( nRC );
   }
   else
   {
      return( -1 );
   }
}

zOPER_EXPORT zVOID OPERATION
CloseSmtpConnection( zLONG lSmtpConnection )
{
   Smtp *pSmtp = (Smtp *) lSmtpConnection;

   if ( pSmtp )
      delete( pSmtp );
}

/////////////////////////////////////////////////////////////////////////////
//
// URL NOTATION
// Summary of the separator characters and other special notation that can
// appear in a URL.
//
// Special Notation in a URL Notation Meaning
//    ? Appears just after the email address, separating it fromthe first
//      name/value pair.
//    = Separates each name and value, in the form name=value.
//    & Separates name/value pairs, as in name1=value1&name2=value2.
//    % Precedes an ASCII character code in hexadecimal, in the form %xx, as
//      a way of representing that character. For example,%0A represents a
//      newline (line feed) character. See more on hex encoding below.
//    + Another way to represent a space. For example, the value Bjorn Free
//      could appear in the URL as Bjorn+Free.
//
// In JavaScript, you can hex-encode a string - that is, substitute the
// %xx notation for all characters other than letters and numbers (and
// a few special characters) - by using the escape() function, as follows:
//
// var myString = "Odds & ends for tonight!"
// var myEncodedText = escape( myString );
//
// This code puts the following value in the variable myEncodedText:
//
// Odds%20%26%20ends%20for%20tonight%21
//
// Notice that the spaces, ampersand, and exclamation point have been
// converted to their ASCII character code equivalents: %20,%26,and %21,
// respectively. Without hex encoding, this string couldnot be used as
// a subject value in a URL, because the ampersand would be interpreted
// as a separator between name/value pairs.
//
// Be sure to hex-encode only the subject and message body values in a
// mailto URL. Although the escape() function will not encode letters or
// numbers (or the characters * @ - _ + . /), it will encode the? and &
// separator characters, which would mess up your URL.
//
/////////////////////////////////////////////////////////////////////////////

void
encode64( zPCHAR pchTgt, zCPCHAR cpcSrc )
{
   static zPCHAR Base64Digits =
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   int nIdx = 0;
   int nLth = zstrlen( cpcSrc );
   int k;

   for ( k = 0 ; k < nLth ; k += 3 )
   {
      unsigned int uBuffer;
      uBuffer = ((unsigned char *) cpcSrc)[ k ] << 16;
      if ( k + 1 < nLth )
         uBuffer |= ((unsigned char *) cpcSrc)[ k + 1 ] << 8;

      if ( k + 2 < nLth )
         uBuffer |= ((unsigned char *) cpcSrc)[ k + 2 ];

      pchTgt[ nIdx++ ] = Base64Digits[ (uBuffer >> 18) % 64 ];
      pchTgt[ nIdx++ ] = Base64Digits[ (uBuffer >> 12) % 64 ];
      if ( k + 1 < nLth )
         pchTgt[ nIdx++ ] = Base64Digits[ (uBuffer >> 6) % 64 ];
      else
         pchTgt[ nIdx++ ] = '=';

      if ( k + 2 < nLth )
         pchTgt[ nIdx++ ] = Base64Digits[ uBuffer % 64 ];
      else
         pchTgt[ nIdx++ ] = '=';
   }

   pchTgt[ nIdx++ ] = 0;
}

// Default constructor
Smtp::Smtp( )
{
   WSADATA wsaData;
   WORD    wVersionRequested = MAKEWORD( 2, 2 );

   m_SA = 0;
   m_lCount = 1;
   m_bEhloNotSent = TRUE;
   WSAStartup( wVersionRequested, &wsaData );
   m_lBufferLth = 32768;
   m_ptcBuffer = new char[ sizeof( _TCHAR ) * m_lBufferLth ];
}

// Default destructor
Smtp::~Smtp( )
{
   if ( m_SA )
   {
      SendRecv( "QUIT", "\r\n" );
      ::Sleep( 1000 );
      INT iErr = shutdown( m_SA, SD_SEND );
      if ( iErr == SOCKET_ERROR )
         INT iLastError = WSAGetLastError();

      for ( ; ; )
      {
         INT iRC = recv( m_SA, m_ptcBuffer, m_lBufferLth, 0 );

         if ( iRC == 0 || iRC == SOCKET_ERROR )
            break;
      }

      iErr = closesocket( m_SA );
      if ( iErr == SOCKET_ERROR )
         INT iLastError = WSAGetLastError();
   }

   WSACleanup( );
   m_SA = 0;
   m_bEhloNotSent = FALSE;
   mDeleteInitA( m_ptcBuffer );
}

// Create a new smtp server connection
// Name of your smtp server
zBOOL
Smtp::CreateSmtpConnection( zPCHAR pchSmtpServer )
{
   // Use DNS to resolve address of smtp server.
   struct hostent *heHostent = gethostbyname( pchSmtpServer );
   struct in_addr inaddr;
   struct sockaddr_in sockaddrSend;

   inaddr.s_addr = *((unsigned long *) heHostent->h_addr_list[ 0 ]);
   zPCHAR cpIpAddr = inet_ntoa( inaddr );

   // create a new socket for to connect to the Comcast's SMTP server
   m_SA = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
   sockaddrSend.sin_family = AF_INET;
   sockaddrSend.sin_addr.s_addr = inet_addr( cpIpAddr );
   sockaddrSend.sin_port = htons( 25 );

   // Attempt up to 5 connections to the webserver.
   INT err;
   for ( zLONG lRetry = 0; lRetry < 5; lRetry++ )
   {
      err = connect( m_SA, (struct sockaddr *) &sockaddrSend, sizeof( sockaddr_in ) );
      if ( err != SOCKET_ERROR )
      {
         break;
      }
   }

   // If connect failed print message and exit.
   if ( err == SOCKET_ERROR )
   {
   // printf( "Connect error: %d\n", WSAGetLastError() );
      TraceLineI( "Smtp::CreateSmtpConnection Connect Error: ", WSAGetLastError() );
      return( FALSE );
   }

   return( TRUE );
}

BOOL
Smtp::prepare_header()
{
   if ( !Send( "MIME-nVersion: 1.0", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}

BOOL
Smtp::insert_mixed_part()
{
   if ( !Send( "Content-Type: multipart/mixed; boundary=\"MessagePartBoundry\"", "\r\n" ) )
   {
      return( FALSE );
   }

   return( TRUE );
}

BOOL
Smtp::insert_html_part()
{
   if ( !Send( "Content-Type: text/html; charset=US-ASCII", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}

BOOL
Smtp::insert_text_part()
{
   if ( !Send( "Content-Type: text/plain; charset=US-ASCII", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}

BOOL
Smtp::insert_boundry()
{
   if ( !Send( "\r\n" ) )
      return( FALSE );

   if ( !Send( "--MessagePartBoundry", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}

BOOL
Smtp::insert_end_boundry()
{
   if ( !Send( "\r\n" ) )
      return( FALSE );

   if ( !Send( "--MessagePartBoundry--", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}

/* Sample Header
MIME-nVersion: 1.0
Content-Type: MULTIPART/MIXED; BOUNDARY="-559023410-851401618-1076640626=:18781"

---559023410-851401618-1076640626=:18781
Content-Type: TEXT/PLAIN; charset=US-ASCII
Hi There

 ---559023410-851401618-1076640626=:18781
Content-Type: APPLICATION/octet-stream; name="submitted.tgz"
Content-Transfer-Encoding: BASE64
Content-ID: <Pine.GSO.4.58.0402122150260.18781@ruchbah.ccs.neu.edu>
Content-Description:
Content-Disposition: attachment; filename="submitted.tgz"

  ---559023410-851401618-1076640626=:18781--

*/

zSHORT
Smtp::CreateSmtpMessage( zPCHAR pchSmtpServer,
                         zPCHAR pchSender,
                         zPCHAR pchRecipient[],
                         zLONG  lRecipientCnt,
                         zPCHAR pchSubject,
                         zLONG  lMimeType,      // 1 - text;  2 - html
                         zPCHAR pchBody,
                         zLONG  lHasAttachment, // 0 - No;  1 - Yes
                         zPCHAR pchAttachFile,
                         zPCHAR pchUserName,
                         zPCHAR pchPassword )
{
   CHAR  szBody[ 65534 ] = "";
   CHAR  szUName[ 4096 ] = "";
   CHAR  szPassword[ 4096 ] = "";
   zLONG j;

   m_lCount++;
   if ( m_bEhloNotSent )
   {
      if ( !SendRecv( "EHLO ", "\r\n" ) )  // EHLO
      {
         TraceLineS( "CreateSmtpMessage cannot send: ", "EHLO" );
         return( -2 );
      }

      m_bEhloNotSent = FALSE;

      if ( *pchUserName )
      {
         if ( !SendRecv( "AUTH LOGIN ", "\r\n" ) )
         {
            TraceLineS( "CreateSmtpMessage cannot send: ", "AUTH LOGIN" );
            return( -3 );
         }

         encode64( szUName, pchUserName );
         if ( !SendRecv( szUName, "\r\n" ) )  //
         {
            TraceLineS( "CreateSmtpMessage cannot send: ", "USER NAME" );
            return( -4 );
         }

         encode64( szPassword, pchPassword );
         if ( !SendRecv( szPassword, "\r\n" ) )  //
         {
            TraceLineS( "CreateSmtpMessage cannot send: ", "PASSWORD" );
            return( -5 );
         }
      }
   }

   if ( !SendRecv( "MAIL FROM: ", pchSender, "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "MAIL FROM" );
      return( -6 );
   }

   for ( j = 0; j < lRecipientCnt; j++ )
   {
      if ( !SendRecv( "RCPT TO: ", pchRecipient[ j ], "\r\n" ) )
      {
         TraceLineS( "CreateSmtpMessage cannot send: ", "RCPT TO" );
         return( -7 );
      }
   }

   if ( !SendRecv( "DATA", "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "DATA" );
      return( -8 );
   }

   FormattedDate fd;
   _TCHAR tcDate[ 128 ];

   BOOL bRC = fd.GetFormattedDate( tcDate );
   if ( !Send( tcDate, "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "Date" );
      return( -9 );
   }

   if ( !Send( "FROM: ", pchSender, "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "FROM" );
      return( -10 );
   }

   for ( j = 0; j < lRecipientCnt; j++ )
   {
      if ( !Send( "TO: ", pchRecipient[ j ], "\r\n" ) )
      {
         TraceLineS( "CreateSmtpMessage cannot send: ", "TO" );
         return( -11 );
      }
   }

   if ( !Send( "SUBJECT: ", pchSubject, "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "SUBJECT" );
      return( -12 );
   }

   prepare_header();
   if ( lMimeType == 2 )
   {
      if ( lHasAttachment == 1 ) // so this is HTML And Attachment
      {
         insert_mixed_part();
         insert_boundry();
      }

      insert_html_part();
   }
   else // plain text with or without attachment
   {
      if ( lHasAttachment == 1 )  // this is plain text with attachment
      {
         insert_mixed_part();
         insert_boundry();
      }

      insert_text_part();
   }

   // Set the encoding.
   if ( !Send( "Content-Transfer-Encoding: ", "7bit", "\r\n", "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "Content-Transfer-Encoding" );
      return( -13 );
   }

   // Do the body.
   if ( !Send( pchBody ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "Body" );
      return( -14 );
   }

   if ( !Send( "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "<CR>" );
      return( -15 );
   }

   // Add attachments here if necessary.
   if ( lHasAttachment == 1 )
   {
   // insert_end_boundry();
      insert_boundry();

      if ( !Send( "Content-Type: APPLICATION/octet-stream; name=\"",
                  pchAttachFile, "\"", "\r\n" ) )
      {
         TraceLineS( "CreateSmtpMessage cannot send: ", "APPLICATION" );
         return( -16 );
      }

      if ( !Send( "Content-Transfer-Encoding: 7bit", "\r\n" ) )
      {
         TraceLineS( "CreateSmtpMessage cannot send: ", "Encoding" );
         return( -17 );
      }

      if ( !Send( "Content-Disposition: attachment; filename=\"",
                  pchAttachFile, "\"", "\r\n" ) )
      {
         TraceLineS( "CreateSmtpMessage cannot send attachment: ", pchAttachFile );
         return( -18 );
      }

      if ( !Send( "\r\n" ) )
      {
         TraceLineS( "CreateSmtpMessage cannot send: ", "<CR>" );
         return( -19 );
      }

      FILE *fh = fopen( pchAttachFile, "r" );
      if ( fh )
      {
         int  f = _fileno( fh );
         long lLth = _filelength( f );

         zPCHAR pchBody;

         if ( lLth > zsizeof( szBody ) )
            pchBody = new char[ lLth ];
         else
            pchBody = szBody;

         fread( pchBody, sizeof( zCHAR ), lLth, fh );
         fclose( fh );

         if ( !Send( pchBody ) )
         {
            if ( pchBody )
               delete [] pchBody;

            TraceLineS( "CreateSmtpMessage cannot send: ", "Body" );
            return( -20 );
         }

         if ( pchBody != szBody )
            delete [] pchBody;
      }

      insert_end_boundry();
   }

   // end the body
   if ( !Send( "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", "End Body" );
      return( -21 );
   }

   if ( !SendRecv( ".", "\r\n" ) )
   {
      TraceLineS( "CreateSmtpMessage cannot send: ", ".<CR>" );
      return( -22 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
// Create and send an email message
// smtp server: Name of your smtp server
// email address of person sending the message
// array of email addresses of person/people to receive the message
// count of email addresses in the send to array
// subject line
// Body of email
BOOL
Smtp::CreateSmtpMessage( zPCHAR pchSmtpServer, zPCHAR pchSender,
                         zPCHAR pchRecipient[], zLONG lRecipientCnt,
                         zPCHAR pchSubject, zPCHAR pchBody )
{
   zLONG j;

   m_iCount++;
   if ( m_bEhloNotSent )
   {
      if ( !SendRecv( "EHLO ", "\r\n" ) )
         return( FALSE );

      m_bEhloNotSent = FALSE;
   }

   if ( !SendRecv( "MAIL FROM: ", pchSender, "\r\n" ) )
      return( FALSE );

   for ( j = 0; j < lRecipientCnt; j++ )
   {
      if ( !SendRecv( "RCPT TO: ", pchRecipient[ j ], "\r\n" ) )
         return( FALSE );
   }

   if ( !SendRecv( "DATA", "\r\n" ) )
      return FALSE;

   FormattedDate fd;
   _TCHAR tcDate[ 128 ];

   BOOL bRC = fd.GetFormattedDate( tcDate );
   if ( !Send( tcDate, "\r\n" ) )
      return( FALSE );

   if ( !Send( "FROM: ", pchSender, "\r\n" ) )
      return( FALSE );

   for ( j = 0; j < lRecipientCnt; j++ )
   {
      if ( !Send( "TO: ", pchRecipient[ j ], "\r\n" ) )
         return( FALSE );
   }

   if ( !Send( "SUBJECT: ", pchSubject, "\r\n", "\r\n" ) )
      return( FALSE );

   if ( !Send( pchBody ) )
      return( FALSE );

   if ( !Send( "\r\n" ) )
      return( FALSE );

   if ( !SendRecv( ".", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Create and send a mim encoded html email message
// smtp server: Name of your smtp server
// email address of person sending the message
// array of email addresses of person/people to receive the message
// count of email addresses in the send to array
// subject line
// Body of email

/*BOOL Smtp::CreateHtmlSmtpMessage( zPCHAR pchSmtpServer, zPCHAR pchSender,
                                    zPCHAR pchRecipient[], zLONG lRecipientCnt,
                                    zPCHAR pchSubject, zPCHAR pchBody )
{
   zLONG j;

   m_iCount++;
   if ( m_bEhloNotSent )
   {
      if ( !SendRecv( "EHLO ", "\r\n" ) )
         return( FALSE );

      m_bEhloNotSent = FALSE;
   }

   if ( !SendRecv( "MAIL FROM: ", pchSender, "\r\n" ) )
      return( FALSE );

   for ( j = 0; j < lRecipientCnt; j++ )
   {
      if ( !SendRecv( "RCPT TO: ", pchRecipient[ j ], "\r\n" ) )
         return( FALSE );
   }

   if ( !SendRecv( "DATA", "\r\n" ) )
      return( FALSE );

   FormatedDate fd;

   _TCHAR ptcDate = fd.GetFormatedDate();
   if ( !Send( ptcDate, "\r\n" ) )
      return( FALSE );

   delete [] ptcDate;
   if ( !Send( "FROM: ", pchSender, "\r\n" ) )
      return( FALSE );

   for ( j = 0; j < lRecipientCnt; j++ )
   {
      if ( !Send( "TO: ", pchRecipient[ j ], "\r\n" ) )
         return( FALSE );
   }

   if ( !Send( "Mime-nVersion: ", "1.0", "\r\n" ) )
      return( FALSE );

   if ( !Send( "Content-Type: ", "text/html; charset=US-ASCII", "\r\n" ) )
      return( FALSE );

   if ( !Send( "Content-Transfer-Encoding: ", "7bit", "\r\n" ) )
      return( FALSE );

   if ( !Send( "SUBJECT: ", pchSubject, "\r\n", "\r\n" ) )
      return( FALSE );

   if ( !Send( pchBody ) )
      return( FALSE );

   if ( !Send( "\r\n" ) )
      return( FALSE );

   if ( !SendRecv( ".", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}
*/

#if 0

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*
 * All Windows Sockets error constants are biased by WSABASEERR from
 * the "normal"
 */
#define WSABASEERR              10000
/*
 * Windows Sockets definitions of regular Microsoft C error constants
 */
#define WSAEINTR                (WSABASEERR+4)
#define WSAEBADF                (WSABASEERR+9)
#define WSAEACCES               (WSABASEERR+13)
#define WSAEFAULT               (WSABASEERR+14)
#define WSAEINVAL               (WSABASEERR+22)
#define WSAEMFILE               (WSABASEERR+24)

/*
 * Windows Sockets definitions of regular Berkeley error constants
 */
#define WSAEWOULDBLOCK          (WSABASEERR+35)
#define WSAEINPROGRESS          (WSABASEERR+36)
#define WSAEALREADY             (WSABASEERR+37)
#define WSAENOTSOCK             (WSABASEERR+38)
#define WSAEDESTADDRREQ         (WSABASEERR+39)
#define WSAEMSGSIZE             (WSABASEERR+40)
#define WSAEPROTOTYPE           (WSABASEERR+41)
#define WSAENOPROTOOPT          (WSABASEERR+42)
#define WSAEPROTONOSUPPORT      (WSABASEERR+43)
#define WSAESOCKTNOSUPPORT      (WSABASEERR+44)
#define WSAEOPNOTSUPP           (WSABASEERR+45)
#define WSAEPFNOSUPPORT         (WSABASEERR+46)
#define WSAEAFNOSUPPORT         (WSABASEERR+47)
#define WSAEADDRINUSE           (WSABASEERR+48)
#define WSAEADDRNOTAVAIL        (WSABASEERR+49)
#define WSAENETDOWN             (WSABASEERR+50)
#define WSAENETUNREACH          (WSABASEERR+51)
#define WSAENETRESET            (WSABASEERR+52)
#define WSAECONNABORTED         (WSABASEERR+53)
#define WSAECONNRESET           (WSABASEERR+54)
#define WSAENOBUFS              (WSABASEERR+55)
#define WSAEISCONN              (WSABASEERR+56)
#define WSAENOTCONN             (WSABASEERR+57)
#define WSAESHUTDOWN            (WSABASEERR+58)
#define WSAETOOMANYREFS         (WSABASEERR+59)
#define WSAETIMEDOUT            (WSABASEERR+60)
#define WSAECONNREFUSED         (WSABASEERR+61)
#define WSAELOOP                (WSABASEERR+62)
#define WSAENAMETOOLONG         (WSABASEERR+63)
#define WSAEHOSTDOWN            (WSABASEERR+64)
#define WSAEHOSTUNREACH         (WSABASEERR+65)
#define WSAENOTEMPTY            (WSABASEERR+66)
#define WSAEPROCLIM             (WSABASEERR+67)
#define WSAEUSERS               (WSABASEERR+68)
#define WSAEDQUOT               (WSABASEERR+69)
#define WSAESTALE               (WSABASEERR+70)
#define WSAEREMOTE              (WSABASEERR+71)

#define WSAEDISCON              (WSABASEERR+101)

/*
 * Extended Windows Sockets error constant definitions
 */
#define WSASYSNOTREADY          (WSABASEERR+91)
#define WSAVERNOTSUPPORTED      (WSABASEERR+92)
#define WSANOTINITIALISED       (WSABASEERR+93)

/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (when using the resolver). Note that these errors are
 * retrieved via WSAGetLastError() and must therefore follow
 * the rules for avoiding clashes with error numbers from
 * specific implementations or language run-time systems.
 * For this reason the codes are based at WSABASEERR+1001.
 * Note also that [WSA]NO_ADDRESS is defined only for
 * compatibility purposes.
 */

#define h_errno         WSAGetLastError()

/* Authoritative Answer: Host not found */
#define WSAHOST_NOT_FOUND       (WSABASEERR+1001)
#define HOST_NOT_FOUND          WSAHOST_NOT_FOUND

/* Non-Authoritative: Host not found, or SERVERFAIL */
#define WSATRY_AGAIN            (WSABASEERR+1002)
#define TRY_AGAIN               WSATRY_AGAIN

/* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define WSANO_RECOVERY          (WSABASEERR+1003)
#define NO_RECOVERY             WSANO_RECOVERY

/* Valid name, no data record of requested type */
#define WSANO_DATA              (WSABASEERR+1004)
#define NO_DATA                 WSANO_DATA

/* no address, look for MX record */
#define WSANO_ADDRESS           WSANO_DATA
#define NO_ADDRESS              WSANO_ADDRESS

#endif

// SendRecv
// Pass four character strings; concatenate them;
// do a tcp send and receive on the concatenation.
BOOL
Smtp::SendRecv( _TCHAR *ptc1, _TCHAR *ptc2, _TCHAR *ptc3, _TCHAR *ptc4 )
{
   zLONG lLth = (zLONG) _tcsclen( ptc1 ) + (zLONG) _tcsclen( ptc2 ) +
                (zLONG) _tcsclen( ptc3 ) + (zLONG) _tcsclen( ptc4 ) + 1;

   if ( lLth > m_lBufferLth )
   {
      mDeleteInitA( m_ptcBuffer );
      m_lBufferLth = lLth;
      m_ptcBuffer = new char[ sizeof( _TCHAR ) * m_lBufferLth ];
   }

   _tcscpy( m_ptcBuffer, ptc1 );
   _tcscat( m_ptcBuffer, ptc2 );
   _tcscat( m_ptcBuffer, ptc3 );
   _tcscat( m_ptcBuffer, ptc4 );
   lLth = (zLONG) _tcsclen( m_ptcBuffer );
   if ( send( m_SA, m_ptcBuffer, lLth, 0 ) == SOCKET_ERROR )
   {
      INT err = WSAGetLastError();
      return( FALSE );
   }

   memset( m_ptcBuffer, 0, m_lBufferLth );
   if ( recv( m_SA, m_ptcBuffer, m_lBufferLth, 0 ) == SOCKET_ERROR )
   {
      INT err = WSAGetLastError();
      return( FALSE );
   }

   return( TRUE );
}

// Send
// Pass four character string; do a tcp send on them.
BOOL
Smtp::Send( _TCHAR *ptc1, _TCHAR *ptc2, _TCHAR *ptc3, _TCHAR *ptc4 )
{
   zLONG lLth = (zLONG) _tcsclen( ptc1 ) + (zLONG) _tcsclen( ptc2 ) +
                (zLONG) _tcsclen( ptc3 ) + (zLONG) _tcsclen( ptc4 ) + 1;

   if ( lLth > m_lBufferLth )
   {
      mDeleteInitA( m_ptcBuffer );
      m_lBufferLth = lLth;
      m_ptcBuffer = new char[ sizeof( _TCHAR ) * m_lBufferLth ];
   }

   _tcscpy( m_ptcBuffer, ptc1 );
   _tcscat( m_ptcBuffer, ptc2 );
   _tcscat( m_ptcBuffer, ptc3 );
   _tcscat( m_ptcBuffer, ptc4 );
   lLth = (zLONG) _tcsclen( m_ptcBuffer );
   if ( send( m_SA, m_ptcBuffer, lLth, 0 ) == SOCKET_ERROR  )
   {
      INT err = WSAGetLastError();
      return( FALSE );
   }

   return( TRUE );
}

#if 0
int APIENTRY
WinMain( HINSTANCE hInstance,
         HINSTANCE hPrevInstance,
         LPSTR     lpCmdLine,
         int       nCmdShow )
{
   // TODO: Place code here.
   zPCHAR pchNames[] = { "dks@quinsoft.com", "arksoft@comcast.net" };
   FILE *fp;

   CHAR szBody[ 45120 ] = "" ;
   CHAR szString[ 1024 ];

   Smtp smtpMailMessage;
   Smtp smtpMailMessage2;
   Smtp smtpMailMessage3;
   Smtp smtpMailMessage4;

   strcpy_s( szBody, zsizeof( szBody ), "This is a plain Text EMAIL" );
   smtpMailMessage.CreateSmtpConnection( "www.quinsoft.com" );
   smtpMailMessage.CreateSmtpMessage( "www.quinsoft.com",
                                      "dks@quinsoft.com",
                                      pchNames, 1, "Test",  1, // text
                                      szBody, 0, "" );

   strcpy_s( szBody, zsizeof( szBody ), "" );
   fp = fopen( "C:\\Temp\\test2.html", "r" );
   while ( fgets( szString,5120,fp ) )
   {
      strcat_s( szBody, zsizeof( szBody ), szString );
   }

   fclose( fp );

   smtpMailMessage2.CreateSmtpConnection( "www.quinsoft.com" );
   smtpMailMessage2.CreateSmtpMessage( "www.quinsoft.com",
                                    "dks@quinsoft.com",
                                    pchNames, 1, "Test", 2, // HTML
                                    szBody, 0, "" );

   strcpy_s( szBody, zsizeof( szBody ), "This is aNOTHER plain Text EMAIL with Attachment");
   smtpMailMessage3.CreateSmtpConnection( "www.quinsoft.com" );
   smtpMailMessage3.CreateSmtpMessage( "www.quinsoft.com",
                                    "dks@quinsoft.com",
                                    pchNames, 1, "Test", 1,
                                    szBody, 1,
                                    "C:\\temp\\text1.txt" );

   strcpy_s( szBody, zsizeof( szBody ), "" );
   fp = fopen( "C:\\Temp\\test2.html", "r" );
   while ( fgets( szString, 1024, fp ) )
   {
      strcat_s( szBody, zsizeof( szBody ), szString );
   }

   fclose( fp );
   smtpMailMessage4.CreateSmtpConnection( "www.quinsoft.com" );
   smtpMailMessage4.CreateSmtpMessage( "www.quinsoft.com",
                                    "dks@quinsoft.com",
                                    pchNames, 1, "Test", 2, // HTML
                                    szBody, 1,
                                    "C:\\temp\\test1.rtf" );
   return( 0 );
}
#endif


#if 0
/////////////////////////////////////////////////////////////////////////////
// MIMEMessage.cpp: implementation of the CMIMEMessage class.
// Author: Wes Clyburn (clyburnw@enmu.edu)
/////////////////////////////////////////////////////////////////////////////

#include "MIMEMessage.h"
#include "TextPlain.h"
#include "AppOctetStream.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Static Member Initializers
CMIMEMessage::CMIMETypeManager CMIMEMessage::m_MIMETypeManager;

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////

CMIMEMessage::CMIMEMessage()
{
   m_sMIMEContentType = _T( "multipart/mixed" );
   m_sPartBoundary = _T( "WC_MAIL_PaRt_BoUnDaRy_05151998" );
   m_sNoMIMEText = _T( "This is a multi-part message in MIME format." );

   // Register the MIME types handled by this class
   //
   // These objects are deleted by CMIMTypeManager's destructor
   CMIMEContentAgent *pType = new CTextPlain( TEXT_PLAIN, GetCharsPerLine() );
   register_mime_type( pType );
   pType = new CAppOctetStream( APPLICATION_OCTETSTREAM );
   register_mime_type( pType );
}

CMIMEMessage::~CMIMEMessage()
{
}

// This implementation adds the part to the part-list used
//  to build the body.
BOOL
CMIMEMessage::AddMIMEPart( LPCTSTR szContent,
                           int nContentType,
                           LPCTSTR szParameters,
                           int nEncoding,
                           BOOL bPath )
{
   CMIMEPart part;
   part.m_nContentType = nContentType;
   part.m_sParameters = szParameters;
   part.m_nEncoding = nEncoding;
   part.m_bPath = bPath;
   part.m_sContent = szContent;
   part.m_sContent.TrimLeft();
   part.m_sContent.TrimRight();
   if ( nContentType == TEXT_PLAIN )
      m_MIMEPartList.AddHead( part );
   else
      m_MIMEPartList.AddTail( part );

   return TRUE;
}

void
CMIMEMessage::prepare_header()
{
   CString csTemp;

   // Let the base class add its headers
   CMailMessage::prepare_header();

   add_header_line( _T( "MIME-nVersion: 1.0" ) );
   csTemp.Format( _T( "Content-Type: %s; boundary=%s" ),
                 (LPCTSTR) m_sMIMEContentType,
                 (LPCTSTR) m_sPartBoundary );
   add_header_line( (LPCTSTR) csTemp );
}

void
CMIMEMessage::prepare_body()
{
   // Class user may have assigned body text directly.
   // Convert it to just another MIME part to be processed.
   // If this default Content-Type isn't good enough for the
   // class user, he or she should have used AddMIMEPart() instead.
   if ( m_sBody != _T( "" ) )
      AddMIMEPart( (LPCTSTR) m_sBody, TEXT_PLAIN, "", _7BIT, FALSE );

   // Initialize the body (replace current contents).
   m_sBody = m_sNoMIMEText;
   m_sBody += _T( "\r\n\r\n" );
   append_mime_parts();
   insert_message_end( m_sBody );

   // Let the base class take me to Funky Town
   CMailMessage::prepare_body();
}

void
CMIMEMessage::insert_boundary( CString& csText )
{
   CString csTemp;
   if ( csText.Right( 2 ) != _T( "\r\n" ) )
      csText += _T( "\r\n" );

   csTemp.Format( _T( "--%s\r\n" ), (LPCTSTR)m_sPartBoundary );
   csText += csTemp;
}

void
CMIMEMessage::insert_message_end( CString& csText )
{
   CString csTemp;
   if ( csText.Right( 2 ) != _T( "\r\n" ) )
      csText += _T( "\r\n" );

   csTemp.Format( _T( "--%s--\r\n" ), (LPCTSTR)m_sPartBoundary );
   csText += csTemp;
}

void
CMIMEMessage::register_mime_type( CMIMEContentAgent *pMIMEType )
{
   ASSERT( pMIMEType );
   if ( pMIMEType == NULL )
      return;

   m_MIMETypeManager.RegisterMIMEType( pMIMEType );
}


void
CMIMEMessage::append_mime_parts()
{
   POSITION part_position;
   CMIMEPart *pMIMEPart = NULL;
   CMIMEContentAgent *pMIMEType = NULL;

   part_position = m_MIMEPartList.GetHeadPosition();
   // Get each part from the list, retrieve a handler for it,
   //  and let the handler do its thing.
   while ( part_position != NULL )
   {
      pMIMEPart = & m_MIMEPartList.GetNext( part_position );
      pMIMEType = m_MIMETypeManager.GetHandler( pMIMEPart->m_nContentType );
      if ( pMIMEType )
      {
         insert_boundary( m_sBody );
         pMIMEType->AppendPart( pMIMEPart->m_sContent,
                                pMIMEPart->m_sParameters,
                                pMIMEPart->m_nEncoding,
                                pMIMEPart->m_bPath,
                                m_sBody );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CMIMETypeManager Implementation
/////////////////////////////////////////////////////////////////////////////

CMIMEMessage::CMIMETypeManager::CMIMETypeManager()
{
}

CMIMEMessage::CMIMETypeManager::~CMIMETypeManager()
{
   POSITION pos;
   CMIMEContentAgent *p;
   m_csAccess.Lock();
   pos = m_MIMETypeList.GetHeadPosition();
   while ( pos )
   {
      p = m_MIMETypeList.GetNext( pos );
      delete p;
   }
}

void
CMIMEMessage::CMIMETypeManager::RegisterMIMEType( CMIMEContentAgent *pMIMEType )
{
   ASSERT( pMIMEType );
   if ( pMIMEType == NULL )
      return;

   m_csAccess.Lock( );
   m_MIMETypeList.AddTail( pMIMEType );
}

CMIMEContentAgent *
CMIMEMessage::CMIMETypeManager::GetHandler( int nContentType )
{
   POSITION pos;
   CMIMEContentAgent* pType = NULL;

   m_csAccess.Lock();
   pos = m_MIMETypeList.GetHeadPosition();
   while ( pos )
   {
      pType = m_MIMETypeList.GetNext( pos );
      if ( pType->QueryType( nContentType ) == TRUE )
         break;
   }

   return pType;
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
DisplaySeeError( int nSeeCode )
{
   zCHAR szErrorMsg[ zMAX_FILESPEC_LTH + 1 ];

   nSeeCode = seeErrorText( 0, nSeeCode, szErrorMsg, zsizeof( szErrorMsg ) );
   TraceLine( "SEE Error %d: %s", nSeeCode, szErrorMsg );
}

zPCHAR
PrefixFilenameWithAt( zCPCHAR cpcFileName )
{
   zLONG lLth = zstrlen( cpcFileName ) + 2;
   zPCHAR pchAtFileName = new char[ lLth ];
   if ( *cpcFileName == '@' )
      strcpy_s( pchAtFileName, lLth, cpcFileName );
   else
   {
      pchAtFileName[ 0 ] = '@';
      strcpy_s( pchAtFileName + 1, lLth - 1, cpcFileName );
   }

   return( pchAtFileName );
}

zPCHAR
SurroundWithAngleBrackets( zCPCHAR cpcRecipient )
{
   zLONG lLth = zstrlen( cpcRecipient ) + 3;
   zPCHAR pchAngleRecipient = new char[ lLth ];

   if ( (zstrchr( cpcRecipient, '<' ) == 0) || (zstrchr( cpcRecipient, '>' ) == 0) )
   {
      // Surround with angle brackets.
      pchAngleRecipient[ 0 ] = '<';
      strcpy_s( pchAngleRecipient + 1, lLth - 1, cpcRecipient );
      strcat_s( pchAngleRecipient, lLth, ">" );
   }
   else
   {
      strcpy_s( pchAngleRecipient, lLth, cpcRecipient );
   }

   return( pchAngleRecipient );
}

#define SEE_KEY_CODE 314056830

zOPER_EXPORT zLONG OPERATION
CreateSeeConnection( zPCHAR pchSmtpServer,
                     zPCHAR pchSenderEmailAddr,
                     zPCHAR pchSmtpUserName,
                     zPCHAR pchSmtpPassword )
{
   int nSeeCode = seeAttach( 1, SEE_KEY_CODE );
   if ( nSeeCode < 0 )
   {
      TraceLineS( "seeAttach failure: Check key code (SEE_KEY_CODE)", "" );
      return( 0 );
   }

   // Enable diagnostics log file.
   seeStringParam( 0, SEE_LOG_FILE, (LPSTR) "MailHTML.log" );

   int nVersion = seeStatistics( 0, SEE_GET_VERSION );
   int nBuild = seeStatistics( 0, SEE_GET_BUILD );
   TraceLine( "SEE32 Version: %1d.%1d.%1d Build %d   Smtp Server: %s   "
                "Sender: %s   User: %s   Password: %s",
              0x0f & (nVersion >> 8), 0x0f & (nVersion >> 4),
              0x0f & nVersion, nBuild,
              pchSmtpServer, pchSenderEmailAddr,
              pchSmtpUserName ? pchSmtpUserName : "", "" );
           // pchSmtpPassword ? pchSmtpPassword : "" );

   // Enable ESMTP authentication.
   seeIntegerParam( 0, SEE_ENABLE_ESMTP, 1 );

   // Specify user name and password for authentication.
   if ( pchSmtpUserName && *pchSmtpUserName )
      seeStringParam( 0, SEE_SET_USER, pchSmtpUserName );

   if ( pchSmtpPassword && *pchSmtpPassword )
      seeStringParam( 0, SEE_SET_SECRET, pchSmtpPassword );

   // Connect to ESMTP server.
   zPCHAR pchAngleSender = SurroundWithAngleBrackets( pchSenderEmailAddr );
   TraceLineS( "Connecting to ESMTP server: ", pchSmtpServer );
   nSeeCode = seeSmtpConnect( 0,                  // channel
                              pchSmtpServer,      // ESMTP server
                              pchAngleSender,     // return email address
                              pchAngleSender );   // Reply-To header
   delete [] pchAngleSender;

   // SMTP_USER_NAME and SMTP_PASSWORD are used for ESMTP connection.
   if ( nSeeCode < 0 )
   {
      // Server may be SMTP rather than ESMTP! (see MailHTML.log)
      DisplaySeeError( nSeeCode );
      seeRelease();
      return( 0 );
   }

   // If seeSmtpConnect (or seePop3Connect) fails, do not call seeClose.

   TraceLineS( "ESMTP Connection successful!", "" );
   ZSee *pSee = new ZSee;
   return( (zLONG) pSee );
}

// 4.1  Static Libraries   in see_4c.doc

zOPER_EXPORT zSHORT OPERATION
CreateSeeMessage( zLONG  lSeeConnection,
                  zPCHAR pchSmtpServer,
                  zPCHAR pchSender,
                  zPCHAR pchRecipient,
                  zPCHAR pchCC,
                  zPCHAR pchBCC,
                  zPCHAR pchSubject,
                  zLONG  lMimeType,      // 1 - text;  2 - html
                  zPCHAR pchBody,        // if pchBody or pchAltText start
                                         // with '@', a file name is expected
                  zPCHAR pchAltText,     // valid only when sending html
                  zPCHAR pchEmbeddedImages,
                  zLONG  lHasAttachment, // 0 - No;  1 - Yes
                  zPCHAR pchAttachFile,
                  zPCHAR pchUserName,
                  zPCHAR pchPassword )
{
   TraceLine( "CreateSeeMessage Server: %s   Sender: %s   Recipient: %s"
              "   Subject: %s   Mime Type: %d"
              "   User: %s   Password %s",
              pchSmtpServer, pchSender, pchRecipient,
              pchSubject, lMimeType, pchUserName, "******" ); // pchPassword );
   ZSee *pSee = (ZSee *) lSeeConnection;
   zPCHAR pchAngleRecipient = 0;
   zPCHAR pchAngleSender = 0;
   zSHORT nRC = -1;
   int nSeeCode = 0;

   while ( pSee )  // purist's goto
   {
      // Verify that we have all strings we need.
      if ( pchSmtpServer == 0 || *pchSmtpServer == 0 )
      {
         TraceLineS( "ERROR: Missing SMTP server name", "" );
         break;
      }

      // Check "From:" address.
      if ( pchSender == 0 || *pchSender == 0 )
      {
         TraceLineS( "ERROR: 'From' address is missing", "" );
         break;
      }

      // Check "To:" address.
      if ( pchRecipient == 0 || *pchRecipient == 0 )
      {
         TraceLineS( "ERROR: 'To' address is missing", "" );
         break;
      }

      if ( pchSubject == 0 || *pchSubject == 0 )
      {
         TraceLineS( "ERROR: Missing 'Subject:'", "" );
         break;
      }

      if ( pchBody == 0 || *pchBody == 0 )
      {
         if ( lMimeType == 2 )  // html
            TraceLineS( "ERROR: Missing HTML file", "" );
         else
            TraceLineS( "ERROR: Missing message", "" );

         break;
      }

      pchAngleRecipient = SurroundWithAngleBrackets( pchRecipient );
      pchAngleSender = SurroundWithAngleBrackets( pchSender );

      if ( (nSeeCode = seeVerifyFormat( pchAngleRecipient )) < 0 ||
           (nSeeCode = seeVerifyFormat( pchAngleSender )) < 0 )
      {
         break;
      }

      // Display To: and From:
      TraceLine( "To: '%s'   From: '%s'", pchAngleRecipient, pchAngleSender );

      // Connect to SMTP server.
   // TraceLine( "Connecting to server '%s'", SmtpString );
   // nSeeCode = seeSmtpConnect( 0, (LPSTR) SmtpString,
   //                           (LPSTR) FromString, (LPSTR) NULL );
   // if ( nSeeCode < 0 )
   // {
   //    DisplaySeeError( nSeeCode );
   //    break;
   // }

      // Prefix alternate text filename with '@'.
   // PrefixFilenameWithAt( (LPSTR) pchAltText );

      if ( lMimeType == 2 )  // html
      {
         // Ensure HTML filename is prefixed with '@'.
      // zPCHAR pchAtBody = PrefixFilenameWithAt( pchBody );
         zPCHAR pchAtBody = pchBody;

         // Send email.
         TraceLine( "Sending html mail ... Recipient: %s   Subject: %s   "
                       "Body: %s   Embedded Images: %s    "
                       "Alternate Text: %s   Attach File: %s",
                    pchAngleRecipient, pchSubject, pchAtBody,
                    pchEmbeddedImages, pchAltText, pchAttachFile );
         nSeeCode = seeSendHTML( 0,                   // channel 0
                                 pchAngleRecipient,   // To list
                                 pchCC,               // CC list
                                 pchBCC,              // BCC list
                                 pchSubject,          // Subject
                                 pchAtBody,           // Message text
                                 pchEmbeddedImages,   // Embedded images
                                 pchAltText,          // Alternate text
                                 pchAttachFile );     // MIME attachment
      // delete [] pchAtBody;
      }
      else
      {
         nSeeCode = seeSendEmail( 0,                  // channel 0
                                  pchAngleRecipient,  // To list
                                  pchCC,              // CC list
                                  pchBCC,             // BCC list
                                  pchSubject,         // Subject
                                  pchBody,            // Message text
                                  pchAttachFile );    // MIME attachment
      }

      break;
   }

   if ( pchAngleRecipient )
      delete [] pchAngleRecipient;

   if ( pchAngleSender )
      delete [] pchAngleSender;

   if ( nSeeCode < 0 )
   {
      nRC = -1;
      DisplaySeeError( nSeeCode );
      seeClose( 0 );
      return( -1 );
   }
   else
      nRC = 0;

   TraceLineI( "Email has been sent ... RC: ", nRC );
   return( nRC );
}

#if 0

      zPCHAR *ppchToArray = (zPCHAR *) new char[ sizeof( zPCHAR ) * lRecipientCnt ];
      if ( lRecipientCnt == 1 )
      {
         ppchToArray[ 0 ] = pchRecipient;
      }
      else
      {
         zPCHAR pch = pchRecipient;
         zLONG  lCnt = 0;

         while ( lCnt < lRecipientCnt )
         {
            ppchToArray[ lCnt ] = pch;
            pch += zstrlen( pch ) + 1;
         }
      }

      zSHORT nRC = pSmtp->CreateSmtpMessage( pchSmtpServer,
                                             pchSender,
                                             ppchToArray,
                                             lRecipientCnt,
                                             pchSubject,
                                             nMimeType, // text
                                             pchBody,
                                             nHasAttachment,
                                             pchAttachFile,
                                             pchUserName,
                                             pchPassword );
      delete [] ppchToArray;

#endif

zOPER_EXPORT zVOID OPERATION
CloseSeeConnection( zLONG lSeeConnection )
{
   TraceLineS( "Closing Smpt Connection ...", "" );
   ZSee *pSee = (ZSee *) lSeeConnection;

   if ( pSee )
      delete( pSee );
}

zOPER_EXPORT zSHORT OPERATION
ValidateEmailAddressFormat( zCPCHAR cpcEmailAddress )
{
   zPCHAR  pchAngleEmailAddress = SurroundWithAngleBrackets( cpcEmailAddress );
   if ( pchAngleEmailAddress )
   {
      int nSeeCode = seeVerifyFormat( pchAngleEmailAddress );
      delete [] pchAngleEmailAddress;
      if ( nSeeCode >= 0 )
         return( TRUE );

   // DisplaySeeError( nSeeCode );
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//
// fsee.cpp
//
// C++ class wrapper for SEE. See the HELLO.CPP example program.
//

//#include <windows.h>
//#include <iostream.h>

//#include "fsee.h"

// constructor
ZSee::ZSee( void )
{
};

// destructor
ZSee::~ZSee( void )
{
   Close( 0 );
   Release( );
};

// see32 functions
int
ZSee::Abort( int Chan )
{
   return( seeAbort( Chan ) );
}

int
ZSee::Attach( int Channels, long KeyCode )
{
   return( seeAttach( Channels, KeyCode ) );
}

int
ZSee::Close( int Chan )
{
   return( seeClose( Chan ) );
}

int
ZSee::Command( int Chan, LPCSTR Command )
{
   return( seeCommand( Chan, Command ) );
}

int
ZSee::Debug( int Chan, int Index, LPSTR Buffer, int BufLth )
{
   return( seeDebug( Chan, Index, Buffer, BufLth ) );
}

int
ZSee::DecodeBuffer( LPCSTR CodedBuf, LPSTR ClearBuf, int ClearLth )
{
   return( seeDecodeBuffer( CodedBuf, ClearBuf, ClearLth ) );
}

int
ZSee::DeleteEmail( int Chan, int Message )
{
   return( seeDeleteEmail( Chan, Message ) );
}

int
ZSee::Driver( int Chan )
{
   return( seeDriver( Chan ) );
}

int
ZSee::EncodeBuffer( LPCSTR ClearBuf, LPSTR CodedBuf, int CodedLth )
{
   return( seeEncodeBuffer( ClearBuf, CodedBuf, CodedLth ) );
}

int
ZSee::ErrorText( int Chan, int nSeeCode, LPSTR Buffer, int BufLth )
{
   return( seeErrorText( Chan, nSeeCode, Buffer, BufLth ) );
}

int
ZSee::ExtractLine( LPCSTR Src, int LineNbr, LPSTR Buffer, int BufSize )
{
   return( seeExtractLine( Src, LineNbr, Buffer, BufSize ) );
}

int
ZSee::ExtractText( LPCSTR Src, LPCSTR Text, LPSTR Buffer, int BufSize )
{
   return( seeExtractText( Src, Text, Buffer, BufSize ) );
}

int
ZSee::GetEmailCount( int Chan )
{
   return( seeGetEmailCount( Chan ) );
}

int
ZSee::GetEmailFile( int Chan, int MsgNbr, LPCSTR EmailName,
                    LPCSTR EmailDir, LPCSTR AttachDir )
{
   return( seeGetEmailFile( Chan, MsgNbr, EmailName, EmailDir, AttachDir ) );
}

int
ZSee::GetEmailLines( int Chan, int MsgNbr, int Lines, LPSTR Buffer, int Size )
{
   return( seeGetEmailLines( Chan, MsgNbr, Lines, Buffer, Size ) );
}

long
ZSee::GetEmailSize( int Chan, int Message )
{
   return( seeGetEmailSize( Chan, Message ) );
}

int
ZSee::GetEmailUID( int Chan, int MsgNbr, LPSTR Buffer, int Size )
{
   return( seeGetEmailUID( Chan, MsgNbr, Buffer, Size ) );
}

int
ZSee::IntegerParam( int Chan, int ParamName, ULONG ParamValue )
{
   return( seeIntegerParam( Chan, ParamName, ParamValue ) );
}

int
ZSee::Pop3Connect( int Chan, LPCSTR Pop3Ptr, LPCSTR UserPtr, LPCSTR PassPtr )
{
   return( seePop3Connect( Chan, Pop3Ptr, UserPtr, PassPtr ) );
}

int
ZSee::Release( void )
{
   return( seeRelease( ) );
}

int
ZSee::SendEmail( int Chan, LPCSTR ToPtr, LPCSTR CCPtr, LPCSTR BCCPtr,
                 LPCSTR SubjPtr, LPCSTR MsgPtr, LPCSTR AttachPtr )
{
   return( seeSendEmail( Chan, ToPtr, CCPtr, BCCPtr,
                         SubjPtr, MsgPtr, AttachPtr ) );
}

int
ZSee::SendHTML( int Chan, LPCSTR ToPtr, LPCSTR CCPtr, LPCSTR BCCPtr,
                LPCSTR SubjPtr, LPCSTR MsgPtr, LPCSTR ImagesPtr,
                LPCSTR AltTxtPtr, LPCSTR AttachPtr )
{
   return( seeSendHTML( Chan, ToPtr, CCPtr, BCCPtr, SubjPtr,
                        MsgPtr, ImagesPtr, AltTxtPtr, AttachPtr ) );
}

int
ZSee::SmtpConnect( int Chan, LPCSTR SmtpPtr, LPCSTR FromPtr, LPCSTR ReplyTo )
{
   return( seeSmtpConnect( Chan, SmtpPtr, FromPtr, ReplyTo ) );
}

long
ZSee::Statistics( int Chan, int ParamName )
{
   return( seeStatistics( Chan, ParamName ) );
}

int
ZSee::StringParam( int Chan, int ParamName, LPCSTR ParamPtr )
{
   return( seeStringParam( Chan, ParamName, ParamPtr ) );
}

int
ZSee::VerifyFormat( LPCSTR Ptr )
{
   return( seeVerifyFormat( Ptr ) );
}

int
ZSee::VerifyUser( int Chan, LPCSTR UserPtr )
{
   return( seeVerifyUser( Chan, UserPtr ) );
}

int
ZSee::QuoteBuffer( LPSTR String, LPSTR Buffer, int BufLth )
{
   return( seeQuoteBuffer( String, Buffer, BufLth ) );
}

int
ZSee::DecodeUU( LPCSTR CodedPtr, LPSTR ClearPtr )
{
   return( seeDecodeUU( CodedPtr, ClearPtr ) );
}

int
ZSee::EncodeUTF8( int UnicodeValue, LPSTR UTF8Buffer )
{
   return( seeEncodeUTF8( UnicodeValue, UTF8Buffer ) );
}

int
ZSee::DecodeUTF8( LPCSTR UTF8Buffer, LPSTR UnicodeBuffer )
{
   return( seeDecodeUTF8( UTF8Buffer, UnicodeBuffer ) );
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
