#include "stdafx.h"
#include "Smtp.h"
#include "FormatedDate.h"

// Default constructor
Smtp::Smtp( )
{
   WSADATA wsaData;
   WORD    wVersionRequested = MAKEWORD( 2, 2 );

   m_SA = 0;
   m_iCount = 1;
   m_bEhloNotSent = TRUE;
   WSAStartup( wVersionRequested, &wsaData );
   memset( m_tcBuffer, 0, sizeof( m_tcBuffer ) );
}

// Default destructor
Smtp::~Smtp( )
{
   CHAR cBuffer[ 256 ];
   if ( m_SA )
   {
      SendRecv( "QUIT", "\r\n" );
     ::Sleep( 1000 );
     INT iErr = shutdown( m_SA, SD_SEND );
     if ( iErr == SOCKET_ERROR )
        INT iLastError = WSAGetLastError();

     for ( ; ; )
    {
        INT iRC = recv( m_SA, cBuffer, sizeof( cBuffer ), 0 );
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
}

// Create a new smtp server connection
// Name of your smtp server
BOOL
Smtp::CreateSmtpConnection( CHAR *cpSmtpServer )
{
   // Use DNS to resolve address of smtp server
   struct hostent *heHostent = gethostbyname( cpSmtpServer );
   struct in_addr inaddr;
   struct sockaddr_in sockaddrSend;

   inaddr.s_addr = *((unsigned long*)heHostent->h_addr_list[0]);
   char *cpIpAddr = inet_ntoa( inaddr );

   // create a new socket for to connect to the Comcast's SMTP server
   m_SA = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
   sockaddrSend.sin_family = AF_INET;
   sockaddrSend.sin_addr.s_addr = inet_addr( cpIpAddr );
   sockaddrSend.sin_port = htons( 25 );

   //
   // Attempt upto 5 connections to the webserver
   INT err;
   for ( INT iRetry = 0; iRetry < 5; iRetry++ )
   {
      err = connect( m_SA, (struct sockaddr*)&sockaddrSend, sizeof( sockaddr_in  ) );
      if ( err != SOCKET_ERROR )
     {
         break;
      }
   }

   // if connect failed print message and exit
   if ( err == SOCKET_ERROR )
   {
      printf( "Connect error:%d\n", WSAGetLastError() );
      return( FALSE );
   }
   return( TRUE );

}

BOOL Smtp::prepare_header()
{
   if ( !Send( "MIME-Version: 1.0", "\r\n" ) )
      return( FALSE );
   return( TRUE );
}

BOOL Smtp::insert_mixed_part()
{
   if ( !Send( "Content-Type: multipart/mixed; boundary=\"MessagePartBoundry\"", "\r\n" ) )
      return( FALSE );
   return( TRUE );
}

BOOL Smtp::insert_html_part()
{
   if ( !Send( "Content-Type: text/html; charset=US-ASCII", "\r\n" ) )
      return( FALSE );
   return( TRUE );
}

BOOL Smtp::insert_text_part()
{
   if ( !Send( "Content-Type: text/plain; charset=US-ASCII", "\r\n" ) )
      return( FALSE );
   return( TRUE );
}

BOOL Smtp::insert_boundry()
{
   if ( !Send( "\r\n" ) )
      return( FALSE );
   if ( !Send( "--MessagePartBoundry", "\r\n" ) )
      return( FALSE );
   return( TRUE );
}
BOOL Smtp::insert_end_boundry()
{
   if ( !Send( "\r\n" ) )
      return( FALSE );
   if ( !Send( "--MessagePartBoundry--", "\r\n" ) )
      return( FALSE );
   return( TRUE );
}

/* Sample Header
MIME-Version: 1.0
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

BOOL Smtp::CreateSmtpMessage( CHAR *cpSmtpServer,
                       CHAR *cpFrom,
                       CHAR *cpTo[],
                       INT cTo,
                       CHAR *cpSubject,
                       INT nMimeType,  //  1 - text; 2 - html
                       CHAR *cpBody,
                       INT nHasAttachment, // 0 - No;  1 -yes
                       CHAR *cpAttachFile)
{
   FILE *fp1;

   CHAR szBody1[5120] = "" ;
   CHAR szString1[1024];

   m_iCount++;
   if ( m_bEhloNotSent )
   {
      if ( !SendRecv( "EHLO ", "\r\n" ) )
        return( FALSE );
      m_bEhloNotSent = FALSE;
   }
   if ( !SendRecv( "MAIL FROM: ", cpFrom, "\r\n" ) )
      return( FALSE );

   for ( INT j = 0; j < cTo; j++ )
   {
     if ( !SendRecv( "RCPT TO: ", cpTo[ j ], "\r\n" ) )
       return( FALSE );
   }
   if ( !SendRecv( "DATA", "\r\n" ) )
      return( FALSE );

   FormatedDate fd;

   _TCHAR *tcpDate = fd.GetFormatedDate();
   if ( !Send( tcpDate, "\r\n" ) )
      return( FALSE );
   delete [] tcpDate;

   if ( !Send( "FROM: ", cpFrom, "\r\n" ) )
      return( FALSE );

   for ( j = 0; j < cTo; j++ )
   {
      if ( !Send( "TO: ", cpTo[ j ], "\r\n" ) )
         return( FALSE );
   }

   if ( !Send( "SUBJECT: ", cpSubject, "\r\n" ) )
      return( FALSE );

   prepare_header();
   if ( nMimeType == 2 )
   {
      if (nHasAttachment == 1 ) // so this is HTML And Attachment
      {
           insert_mixed_part();
         insert_boundry();
      }
       insert_html_part();
   }
   else // plain text with or without attachment
   {
      if (nHasAttachment == 1 )  // this is plain text with attachment
      {
           insert_mixed_part();
         insert_boundry();
      }
       insert_text_part();
   }
   // set the encoding
   if ( !Send( "Content-Transfer-Encoding: ", "7bit", "\r\n", "\r\n" ) )
      return( FALSE );
   // do the body
   if ( !Send( cpBody ) )
      return( FALSE );
   if ( !Send( "\r\n" ) )
      return( FALSE );

   // add attachments here if necessary
   if (nHasAttachment == 1 )
   {
      //insert_end_boundry();
      insert_boundry();

      if ( !Send( "Content-Type: APPLICATION/octet-stream; name=\"", cpAttachFile,"\"", "\r\n" ) )
         return( FALSE );
      if ( !Send( "Content-Transfer-Encoding: 7bit", "\r\n" ) )
         return( FALSE );
      if ( !Send( "Content-Disposition: attachment; filename=\"", cpAttachFile,"\"", "\r\n" ) )
         return( FALSE );
      if ( !Send( "\r\n" ) )
         return( FALSE );
       /////////////////////
   strcpy( szBody1, "");
   fp1 = fopen(cpAttachFile,"r");
   while (fgets(szString1,1024,fp1))
   {
      strcat( szBody1, szString1 );
   }
   fclose(fp1);
      /////////////////////

      if ( !Send( szBody1 ) )
         return( FALSE );

      insert_end_boundry();

   }

   // end th body
   if ( !Send( "\r\n" ) )
      return( FALSE );

   if ( !SendRecv( ".", "\r\n" ) )
      return( FALSE );

   return( TRUE );
}
//////////////////////////////////////////
//////////////////////////////////////////

// Create and send a mim encoded html email message
// smtp server: Name of your smtp server
// email address of person sending the message
// array of email addresses of person/people to receive the message
// count of email addresses in the send to array
// subject line
// Body of email

/*BOOL Smtp::CreateHtmlSmtpMessage( CHAR *cpSmtpServer
,CHAR *cpFrom
,CHAR *cpTo[]
,INT cTo
,CHAR *cpSubject
,CHAR *cpBody )
{
 m_iCount++;
 if ( m_bEhloNotSent )
 {
        if ( !SendRecv( "EHLO ", "\r\n" ) ) return( FALSE );
  m_bEhloNotSent = FALSE;
 }
 if ( !SendRecv( "MAIL FROM: ", cpFrom, "\r\n" ) ) return( FALSE );
 for ( INT j = 0; j < cTo; j++ )
 {
  if ( !SendRecv( "RCPT TO: ", cpTo[ j ], "\r\n" ) )
return( FALSE );
 }
 if ( !SendRecv( "DATA", "\r\n" ) ) return( FALSE );

 FormatedDate fd;

 _TCHAR *tcpDate = fd.GetFormatedDate();
 if ( !Send( tcpDate, "\r\n" ) ) return( FALSE );
 delete [] tcpDate;
 if ( !Send( "FROM: ", cpFrom, "\r\n" ) ) return( FALSE );
 for ( j = 0; j < cTo; j++ )
 {
  if ( !Send( "TO: ", cpTo[ j ], "\r\n" ) ) return( FALSE );
 }
 if ( !Send( "Mime-Version: ", "1.0", "\r\n" ) ) return
 FALSE;
 if ( !Send( "Content-Type: ", "text/html; charset=US-ASCII", "\r\n" ) ) return
 FALSE;
 if ( !Send( "Content-Transfer-Encoding: ", "7bit", "\r\n" ) ) return
 FALSE;
 if ( !Send( "SUBJECT: ", cpSubject, "\r\n", "\r\n" ) ) return
FALSE;


 if ( !Send( cpBody ) ) return( FALSE );
 if ( !Send( "\r\n" ) ) return( FALSE );
 if ( !SendRecv( ".", "\r\n" ) ) return( FALSE );

 return( TRUE );
}
*/
//////////////////////////////////////////
//////////////////////////////////////////

// SendRecv
// Pass one character string; do a tcp send and receive on it
BOOL Smtp::SendRecv( _TCHAR *cp1 )
{
 if ( send( m_SA, cp1, (INT)_tcsclen( cp1 ), 0 ) == SOCKET_ERROR
)
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    memset( m_tcBuffer, 0, sizeof( m_tcBuffer ) );
    if ( recv( m_SA, m_tcBuffer, sizeof( m_tcBuffer ), 0 ) ==
SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

 return( TRUE );
}

// SendRecv
// Pass two character strings; concatenate them
// and do a tcp send and receive on the concatenation.
BOOL Smtp::SendRecv( _TCHAR *cp1, _TCHAR *cp2 )
{
    _tcscpy( m_tcBuffer, cp1 );
    _tcscat( m_tcBuffer, cp2 );
    if ( send( m_SA, m_tcBuffer, (INT)_tcsclen( m_tcBuffer ), 0 ) ==
SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    memset( m_tcBuffer, 0, sizeof( m_tcBuffer ) );
    if ( recv( m_SA, m_tcBuffer, sizeof( m_tcBuffer ), 0 ) ==
SOCKET_ERROR  )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

 return( TRUE );
}

// SendRecv
// Pass three character strings; concatenate them;
// do a tcp send and receive on the concatenation.
BOOL Smtp::SendRecv( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3 )
{
    _tcscpy( m_tcBuffer, cp1 );
    _tcscat( m_tcBuffer, cp2 );
    _tcscat( m_tcBuffer, cp3 );
 INT senderror;
 senderror = send( m_SA, m_tcBuffer, (INT)_tcsclen( m_tcBuffer
) ,0 );
 if ( senderror == SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
  err = WSAGetLastError();
//        return( FALSE );
 }

    memset( m_tcBuffer, 0, sizeof( m_tcBuffer ) );
    if ( recv( m_SA, m_tcBuffer, sizeof( m_tcBuffer ), 0 ) ==
SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

 return( TRUE );
}

// SendRecv
// Pass four character strings; concatenate them;
// do a tcp send and receive on the concatenation.
BOOL Smtp::SendRecv( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3, _TCHAR
*cp4 )
{
    _tcscpy( m_tcBuffer, cp1 );
    _tcscat( m_tcBuffer, cp2 );
    _tcscat( m_tcBuffer, cp3 );
    _tcscat( m_tcBuffer, cp4 );
    if ( send( m_SA, m_tcBuffer, (INT)_tcsclen( m_tcBuffer ) ,0 ) ==
SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    memset( m_tcBuffer, 0, sizeof( m_tcBuffer ) );
    if ( recv( m_SA, m_tcBuffer, sizeof( m_tcBuffer ), 0 ) ==
SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

 return( TRUE );
}

// Send
// Pass one character string; do a tcp send on it
BOOL Smtp::Send( _TCHAR *cp1 )
{
    if ( send( m_SA, cp1, (INT)_tcsclen( cp1 ) ,0 ) == SOCKET_ERROR )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    return( TRUE );
}

// Send
// Pass two character string; do a tcp send on them
BOOL Smtp::Send( _TCHAR *cp1, _TCHAR *cp2 )
{
 _tcscpy( m_tcBuffer, cp1 );
 _tcscat( m_tcBuffer, cp2 );
    if ( send( m_SA, m_tcBuffer, (INT)_tcsclen( m_tcBuffer ) ,0 ) ==
SOCKET_ERROR  )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    return( TRUE );
}

// Send
// Pass three character string; do a tcp send on them
BOOL Smtp::Send( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3 )
{
 _tcscpy( m_tcBuffer, cp1 );
 _tcscat( m_tcBuffer, cp2 );
 _tcscat( m_tcBuffer, cp3 );
    if ( send( m_SA, m_tcBuffer, (INT)_tcsclen( m_tcBuffer ) ,0 ) ==
SOCKET_ERROR  )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    return( TRUE );
}

// Send
// Pass four character string; do a tcp send on them
BOOL Smtp::Send( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3, _TCHAR *cp4 )
{
 _tcscpy( m_tcBuffer, cp1 );
 _tcscat( m_tcBuffer, cp2 );
 _tcscat( m_tcBuffer, cp3 );
 _tcscat( m_tcBuffer, cp4 );
    if ( send( m_SA, m_tcBuffer, (INT)_tcsclen( m_tcBuffer ) ,0 ) ==
SOCKET_ERROR  )
 {
  INT err = WSAGetLastError();
        return( FALSE );
 }

    return( TRUE );
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
   // TODO: Place code here.
   CHAR *cNames[] = { "dks@quinsoft.com", "arksoft@comcast.net" };
   FILE *fp;

   CHAR szBody[45120] = "" ;
   CHAR szString[1024];


   Smtp sMailMessage;
   Smtp sMailMessage2;
   Smtp sMailMessage3;
   Smtp sMailMessage4;

   strcpy( szBody, "This is a plain Text EMAIL");
   sMailMessage.CreateSmtpConnection( "www.quinsoft.com" );
   sMailMessage.CreateSmtpMessage( "www.quinsoft.com",
                                  "dks@quinsoft.com",
                           cNames,
                           1,
                           "Test",
                           1, // text
                           szBody,
                           0,
                           "" );

   strcpy( szBody, "");
   fp = fopen("C:\\Temp\\test2.html","r");
   while (fgets(szString,5120,fp))
   {
      strcat( szBody, szString );
   }
   fclose(fp);

   sMailMessage2.CreateSmtpConnection( "www.quinsoft.com" );
   sMailMessage2.CreateSmtpMessage( "www.quinsoft.com",
                                  "dks@quinsoft.com",
                           cNames,
                           1,
                           "Test",
                           2, // HTML
                           szBody,
                           0,
                           "" );

   strcpy( szBody, "This is aNOTHER plain Text EMAIL with Attachment");
   sMailMessage3.CreateSmtpConnection( "www.quinsoft.com" );
   sMailMessage3.CreateSmtpMessage( "www.quinsoft.com",
                                  "dks@quinsoft.com",
                           cNames,
                           1,
                           "Test",
                           1,
                           szBody,
                           1,
                           "C:\\temp\\text1.txt" );

   strcpy( szBody, "");
   fp = fopen("C:\\Temp\\test2.html","r");
   while (fgets(szString,1024,fp))
   {
      strcat( szBody, szString );
   }
   fclose(fp);
   sMailMessage4.CreateSmtpConnection( "www.quinsoft.com" );
   sMailMessage4.CreateSmtpMessage( "www.quinsoft.com",
                                  "dks@quinsoft.com",
                           cNames,
                           1,
                           "Test",
                           2, // HTML
                           szBody,
                           1,
                           "C:\\temp\\test1.rtf" );
   return 0;
}

