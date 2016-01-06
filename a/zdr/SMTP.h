#ifndef __Smtp_H__
#define __Smtp_H__

class Smtp
{
public:
   ~Smtp();
   Smtp();

   BOOL CreateSmtpConnection( CHAR *cpSmtpServer );
   BOOL CreateSmtpMessage( CHAR *cpSmtpServer,
                           CHAR *cpFrom,
                           CHAR *cpTo[],
                           INT cTo,
                           CHAR *cpSubject,
                           INT nMimeType,      //  1 - text; 2 - html
                           CHAR *cpBody,
                           INT nHasAttachment, // 0 - No;  1 - Yes
                           CHAR *cpAttachFile );

private:
   INT    m_iCount;
   BOOL   m_bHeloNotSent;
   SOCKET m_SA;
   _TCHAR m_tcBuffer[ 2048 ];

   BOOL SendRecv( _TCHAR *cp1 );
   BOOL SendRecv( _TCHAR *cp1, _TCHAR *cp2 );
   BOOL SendRecv( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3 );
   BOOL SendRecv( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3, _TCHAR *cp4 );
   BOOL Send( _TCHAR *cp1 );
   BOOL Send( _TCHAR *cp1, _TCHAR *cp2 );
   BOOL Send( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3 );
   BOOL Send( _TCHAR *cp1, _TCHAR *cp2, _TCHAR *cp3, _TCHAR *cp4 );
   BOOL insert_end_boundry();
   BOOL insert_boundry();
   BOOL insert_text_part();
   BOOL insert_html_part();
   BOOL insert_mixed_part();
   BOOL prepare_header();
};

#endif //__Smtp_H__
