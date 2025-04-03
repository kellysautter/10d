#ifndef __ZSEM_H__
#define __ZSEM_H__

#ifndef zOPER_EXPORT
#define zOPER_EXPORT       __declspec (dllexport)  // AFX_API_EXPORT
#define OPERATION          __stdcall
#endif

char *
ReadFileIntoBuffer( zPCHAR pchAttachFile );
void
encode64( zPCHAR pchTgt, zCPCHAR cpcSrc );

class FormattedDate
{
public:
   FormattedDate( );
   ~FormattedDate( );

   zBOOL GetFormattedDate( _TCHAR *ptcDateTime );
};

class Smtp
{
public:
   ~Smtp();
   Smtp();

   zBOOL CreateSmtpConnection( zPCHAR pchSmtpServer );
   zSHORT CreateSmtpMessage( zPCHAR pchSmtpServer,
                             zPCHAR pchSender,
                             zPCHAR pchRecipient[],
                             zLONG  lRecipientCnt,
                             zPCHAR pchSubject,
                             zLONG  lMimeType,      //  1 - text;  2 - html
                             zPCHAR pchBody,
                             zLONG  lHasAttachment, // 0 - No;  1 - Yes
                             zPCHAR pchAttachFile,
                             zPCHAR pchUserName,
                             zPCHAR pchPassword );

private:
   zLONG  m_lCount;
   BOOL   m_bEhloNotSent;
   SOCKET m_SA;
   _TCHAR *m_ptcBuffer;
   zLONG  m_lBufferLth;

   BOOL SendRecv( _TCHAR *ptc1, _TCHAR *ptc2 = "",
                  _TCHAR *ptc3 = "", _TCHAR *ptc4 = "" );
   BOOL Send( _TCHAR *ptc1, _TCHAR *ptc2 = "",
              _TCHAR *ptc3 = "", _TCHAR *ptc4 = "" );
   BOOL insert_end_boundry();
   BOOL insert_boundry();
   BOOL insert_text_part();
   BOOL insert_html_part();
   BOOL insert_mixed_part();
   BOOL prepare_header();
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*
**  SEE.H (SEE.H2)
**
**  Use for both Win16 & Win32.
**
**  For Win32 compilers supporting the "declspec" keyword.
*/

//#define STATIC_LIBRARY
#ifdef STATIC_LIBRARY
  #ifdef WIN32
    #define DLL_IMPORT_EXPORT
  #else
    #define DLL_IMPORT_EXPORT FAR PASCAL
  #endif
#else
  #ifdef WIN32
    #ifdef DLL_SOURCE_CODE
       #define DLL_IMPORT_EXPORT __declspec(dllexport) __stdcall
    #else
       #define DLL_IMPORT_EXPORT __declspec(dllimport) __stdcall
    #endif
  #else
    #define DLL_IMPORT_EXPORT FAR PASCAL
  #endif
#endif

#ifdef __cplusplus
  #define NoMangle extern "C"
#else
  #define NoMangle
#endif

#define SEE_MIN_RESPONSE_WAIT         1
#define SEE_MAX_RESPONSE_WAIT         2
#define SEE_CONNECT_WAIT              3
#define SEE_DISABLE_MIME              4
#define SEE_QUOTED_PRINTABLE          8
#define SEE_AUTO_CALL_DRIVER          9
#define SEE_FILE_PREFIX              10
#define SEE_SLEEP_TIME               13
#define SEE_DECODE_UNNAMED           14
#define SEE_SMTP_PORT                15
#define SEE_POP3_PORT                16
#define SEE_MAX_LINE_LENGTH          17
#define SEE_BLOCKING_MODE            18
#define SEE_ALLOW_8BITS              19
#define SEE_LOG_FILE                 20
#define SEE_HIDE_SAVED_MSG           21
#define SEE_HIDE_TO_ADDR             22
#define SEE_ADDRESS_DELIMITER        23
#define SEE_WSACLEANUP               24
#define SEE_PATH_DELIMITER           25
#define SEE_ATTACH_DELIMITER         26
#define SEE_ENABLE_IMAGE             27
#define SEE_RAW_MODE                 28
#define SEE_ENABLE_ESMTP             29
#define SEE_ENABLE_APOP              30
#define SEE_ATTACH_BASE_NUMBER       31
#define SEE_IGNORE_REJECTED          32
#define SEE_WRITE_CONTENT_TYPE       33
#define SEE_SET_FILE_PREFIX          34
#define SEE_HTML_CHARSET             35

#define CHARSET_BLANK                 0
#define CHARSET_US_ASCII              1
#define CHARSET_8859                  4
#define CHARSET_ISO_8859_1            4
#define CHARSET_ISO_8859_8            5
#define CHARSET_WIN_1252              6
#define CHARSET_WIN_1255              7

#define SEE_GET_ERROR_TEXT            1
#define SEE_GET_COUNTER               2
#define SEE_GET_RESPONSE              3
#define SEE_GET_SOCK_ERROR            4

#define SEE_GET_MESSAGE_BYTES_READ   10
#define SEE_GET_ATTACH_BYTES_READ    11
#define SEE_GET_TOTAL_BYTES_READ     12
#define SEE_GET_MESSAGE_BYTES_SENT   13
#define SEE_GET_ATTACH_BYTES_SENT    14
#define SEE_GET_TOTAL_BYTES_SENT     15
#define SEE_GET_VERSION              16

#define SEE_GET_MSG_COUNT            17
#define SEE_GET_MSG_SIZE             18
#define SEE_GET_BUFFER_COUNT         19

#define SEE_GET_CONNECT_STATUS       20
#define SEE_GET_REGISTRATION         21
#define SEE_GET_ATTACH_COUNT         22
#define SEE_GET_LAST_RESPONSE        23
#define SEE_GET_VERIFY_STATUS        24
#define SEE_GET_SERVER_IP            25
#define SEE_GET_BUILD                26
#define SEE_GET_SOCKET               27
#define SEE_GET_LOCAL_IP             28
#define SEE_GET_ATTACH_NAMES         29
#define SEE_GET_LAST_RECIPIENT       30

#define SEE_COPY_BUFFER              40
#define SEE_WRITE_BUFFER             41

#define SEE_SET_REPLY                50
#define SEE_SET_HEADER               51
#define SEE_WRITE_TO_LOG             52
#define SEE_SET_FROM                 53
#define SEE_SET_CONTENT_TYPE         54
#define SEE_SET_TRANSFER_ENCODING    55
#define SEE_ADD_HEADER               56
#define SEE_SET_SECRET               57
#define SEE_SET_USER                 58
#define SEE_SET_TEXT_MESSAGE         59
#define SEE_FORCE_INLINE             60
#define SEE_SET_ATTACH_CONTENT_TYPE  61
#define SEE_AUTHENTICATE_PROTOCOL    62
#define SEE_SET_CONTENT_TYPE_PREFIX  63
#define SEE_ENABLE_XMAILER           64

#define QUOTED_OFF                    0
#define QUOTED_PLAIN                  1
#define QUOTED_HTML                   2
#define QUOTED_RICH                   3
#define QUOTED_8859                   4
#define QUOTED_ISO_8859_1             4
#define QUOTED_ISO_8859_8             5
#define QUOTED_WIN_1252               6
#define QUOTED_WIN_1255               7
#define QUOTED_USER                   9

#define INLINE_TEXT_OFF               0
#define INLINE_TEXT_INLINE            1
#define INLINE_TEXT_ATTACHMENT        2

#define AUTHENTICATE_CRAM             1
#define AUTHENTICATE_LOGIN            2
#define AUTHENTICATE_PLAIN            4

#define BYTE  unsigned char
#define ULONG unsigned long


NoMangle int  DLL_IMPORT_EXPORT
seeAbort( int Chan );                        // aborts SEE session.
NoMangle int  DLL_IMPORT_EXPORT
seeAttach( int Channels, long KeyCode );     // attaches SEE
NoMangle int  DLL_IMPORT_EXPORT
seeClose( int Chan );                        // closes SMTP/POP3 Email Engine
NoMangle int  DLL_IMPORT_EXPORT
seeCommand( int Chan, LPCSTR Command );      // transmit arbitrary SMTP/POP3 command
NoMangle int  DLL_IMPORT_EXPORT
seeDebug( int Chan, int Index,
          LPSTR Buffer,
          int BufLth );                      // returns debug information
NoMangle int  DLL_IMPORT_EXPORT
seeDecodeBuffer( LPCSTR CodedBuf,
                 LPSTR ClearBuf,
                 int ClearLth );             // decodes base-64 buffer
NoMangle int  DLL_IMPORT_EXPORT
seeDecodeUTF8( LPCSTR UTF8Buffer,
               LPSTR UnicodeBuffer );        // decodes UTF-8 to 16-bit Unicode
NoMangle int  DLL_IMPORT_EXPORT
seeDecodeUU( LPCSTR CodedPtr,
             LPSTR ClearPtr );               // decodes UU-encoded text
NoMangle int  DLL_IMPORT_EXPORT
seeDeleteEmail( int Chan, int Message );     // deletes email
NoMangle int  DLL_IMPORT_EXPORT
seeDriver( int Chan );                       // executes next SEE state
NoMangle int  DLL_IMPORT_EXPORT
seeEncodeBuffer( LPCSTR ClearBuf,
                 LPSTR CodedBuf,
                 int CodedLth );             // encodes base-64 buffer
NoMangle int  DLL_IMPORT_EXPORT
seeEncodeUTF8( int UnicodeValue,
               LPSTR UTF8Buffer );           // encodes 16-bit Unicode to UTF-8
NoMangle int  DLL_IMPORT_EXPORT
seeErrorText( int Chan, int Code,
              LPSTR Buffer, int BufLth );    // get text associated with error code
NoMangle int  DLL_IMPORT_EXPORT
seeExtractLine( LPCSTR Src, int LineNbr,
                LPSTR Buffer, int BufSize ); // extracts line by line number
NoMangle int  DLL_IMPORT_EXPORT
seeExtractText( LPCSTR Src, LPCSTR Text,
                LPSTR Buffer, int BufSize ); // extracts line containing specified text
NoMangle int  DLL_IMPORT_EXPORT
seeGetEmailCount( int Chan );                // get number of emails waiting on server
NoMangle int  DLL_IMPORT_EXPORT
seeGetEmailFile( int Chan, int MsgNbr,
                 LPCSTR EmailName,
                 LPCSTR EmailDir,
                 LPCSTR AttachDir );         // read email file and save to disk
NoMangle int  DLL_IMPORT_EXPORT
seeGetEmailLines( int Chan, int MsgNbr,
                  int Lines, LPSTR Buffer,
                  int Size );                // read email lines into buffer
NoMangle long DLL_IMPORT_EXPORT
seeGetEmailSize( int Chan, int Message );    // get size of email message on server
NoMangle int  DLL_IMPORT_EXPORT
seeGetEmailUID( int Chan, int MsgNbr,
                LPSTR Buffer, int Size );    // get email message user ID string
NoMangle int  DLL_IMPORT_EXPORT
seeIntegerParam( int Chan, int ParamName,
                 ULONG ParamValue );         // sets SEE integer parameter
NoMangle int  DLL_IMPORT_EXPORT
seePop3Connect( int Chan, LPCSTR Pop3Ptr,
                LPCSTR UserPtr,
                LPCSTR PassPtr );            // connects to POP3 server
NoMangle int  DLL_IMPORT_EXPORT
seeQuoteBuffer( LPSTR String, LPSTR Buffer,
                int BufLth );                // creates ISO-8859 encoded strings
NoMangle int  DLL_IMPORT_EXPORT
seeRelease( void );                          // releases SEE
NoMangle int  DLL_IMPORT_EXPORT
seeSendEmail( int Chan, LPCSTR ToPtr,
              LPCSTR CCPtr, LPCSTR BCCPtr,
              LPCSTR SubjPtr, LPCSTR MsgPtr,
              LPCSTR AttachPtr );            // sends email and attachments
NoMangle int  DLL_IMPORT_EXPORT
seeSendHTML( int Chan, LPCSTR ToPtr,
             LPCSTR CCPtr, LPCSTR BCCPtr,
             LPCSTR SubjPtr, LPCSTR MsgPtr,
             LPCSTR ImagesPtr,
             LPCSTR AltTxtPtr,
             LPCSTR AttachPtr );             // sends HTML encoded email
NoMangle int  DLL_IMPORT_EXPORT
seeSmtpConnect( int Chan, LPCSTR SmtpPtr,
                LPCSTR FromPtr,
                LPCSTR ReplyTo );            // connects to SMTP server
NoMangle ULONG DLL_IMPORT_EXPORT
seeStatistics( int Chan, int ParamName );    // returns runtime statistics
NoMangle int  DLL_IMPORT_EXPORT
seeStringParam( int Chan, int ParamName,
                LPCSTR ParamPtr );           // sets SEE string parameter
NoMangle int  DLL_IMPORT_EXPORT
seeVerifyFormat( LPCSTR Ptr );               // check email address format
NoMangle int  DLL_IMPORT_EXPORT
seeVerifyUser( int Chan, LPCSTR UserPtr );   // check email address with SMTP server

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// fsee.h

//#include <windows.h>
//#include "see.h"

class ZSee
{
public:
   ZSee( void );
   ~ZSee( void );

   int  Abort( int Chan );
   int  Attach( int Channels, long KeyCode );
   int  Close( int Chan );
   int  Command( int Chan, LPCSTR Command );
   int  Debug( int Chan, int Index, LPSTR Buffer, int BufLth );
   int  DecodeBuffer( LPCSTR CodedBuf, LPSTR ClearBuf, int ClearLth );
   int  DecodeUTF8( LPCSTR UTF8Buffer, LPSTR UnicodeBuffer );
   int  DecodeUU( LPCSTR CodedPtr, LPSTR ClearPtr );
   int  DeleteEmail( int Chan, int Message );
   int  Driver( int Chan );
   int  EncodeBuffer( LPCSTR ClearBuf, LPSTR CodedBuf, int CodedLth );
   int  EncodeUTF8( int UnicodeValue, LPSTR UTF8Buffer );
   int  ErrorText( int Chan, int Code, LPSTR Buffer, int BufLth );
   int  ExtractLine( LPCSTR Src, int LineNbr, LPSTR Buffer, int BufSize );
   int  ExtractText( LPCSTR Src, LPCSTR Text, LPSTR Buffer, int BufSize );
   int  GetEmailCount( int Chan );
   int  GetEmailFile( int Chan, int MsgNbr, LPCSTR EmailName,
                      LPCSTR EmailDir, LPCSTR AttachDir );
   int  GetEmailLines( int Chan, int MsgNbr, int Lines,
                       LPSTR Buffer, int Size );
   long GetEmailSize( int Chan, int Message );
   int  GetEmailUID( int Chan, int MsgNbr, LPSTR Buffer, int Size );
   int  IntegerParam( int Chan, int ParamName, ULONG ParamValue );
   int  Pop3Connect( int Chan, LPCSTR Pop3Ptr,
                     LPCSTR UserPtr, LPCSTR PassPtr );
   int  QuoteBuffer( LPSTR String, LPSTR Buffer, int BufLth );
   int  Release( void );
   int  SendEmail( int Chan, LPCSTR ToPtr, LPCSTR CCPtr, LPCSTR BCCPtr,
                   LPCSTR SubjPtr, LPCSTR MsgPtr, LPCSTR AttachPtr );
   int  SendHTML( int Chan, LPCSTR ToPtr, LPCSTR CCPtr, LPCSTR BCCPtr,
                  LPCSTR SubjPtr, LPCSTR MsgPtr, LPCSTR ImagesPtr,
                  LPCSTR AltTxtPtr, LPCSTR AttachPtr );
   int  SmtpConnect( int Chan, LPCSTR SmtpPtr,
                     LPCSTR FromPtr, LPCSTR ReplyTo );
   long Statistics( int Chan, int ParamName );
   int  StringParam( int Chan, int ParamName, LPCSTR ParamPtr );
   int  VerifyFormat( LPCSTR Ptr );
   int  VerifyUser( int Chan, LPCSTR UserPtr );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif //__ZSEM_H__
