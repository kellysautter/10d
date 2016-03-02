// MonProcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonProc.h"
#include "MonProcDlg.h"
#include "XML_PARSER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int g_nCheckTomcat = 0;

/////////////////////////////////////////////////////////////////////////////
// CMonProcDlg dialog

CMonProcDlg::CMonProcDlg( CWnd *pParent /*=NULL*/ ) :
             CDialog( CMonProcDlg::IDD, pParent )
{
   //{{AFX_DATA_INIT( CMonProcDlg )
   m_strOs = _T( "" );
   m_strProcessToCheck = _T( "" );
   //}}AFX_DATA_INIT
   m_hIcon = AfxGetApp( )->LoadIcon( IDR_MAINFRAME );
}

void
CMonProcDlg::DoDataExchange( CDataExchange *pDX )
{
   CDialog::DoDataExchange( pDX );
   //{{AFX_DATA_MAP( CMonProcDlg )
   DDX_Control( pDX, IDC_PROCESSLIST, m_ctrlProcLB );
   DDX_Text( pDX, IDC_OSMESSAGE, m_strOs );
   DDX_Text( pDX, IDC_PROCESS_NAME, m_strProcessToCheck );
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CMonProcDlg, CDialog )
   //{{AFX_MSG_MAP( CMonProcDlg )
   ON_BN_CLICKED( IDC_GETALLPROCS, OnGetAllProcs )
   ON_BN_CLICKED( IDC_CHECKIFACTIVE, OnCheckIfActive )
   ON_BN_CLICKED( IDC_CHECKTOMCAT, OnCheckTomcatButton )
   ON_BN_CLICKED( IDC_CANCELCHECKTOMCAT, OnCancelCheckTomcat )
   ON_WM_CLOSE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// CMonProcDlg message handlers

BOOL
CMonProcDlg::OnInitDialog( )
{
   CDialog::OnInitDialog( );

   SetIcon( m_hIcon, TRUE );     // Set big icon
   SetIcon( m_hIcon, FALSE );    // Set small icon

   if ( !m_win32proc.Init( ) )
      AfxMessageBox( m_win32proc.GetLastError( ) );

   m_strOs = m_win32proc.IsWinNT( ) ? "Win NT/XP" : "Win 95";
   m_strOs += "  v1.0.0.2";

   UpdateData( FALSE );

#if 0
   // Initialize COM
   ::CoInitialize( NULL );

   HRESULT hr = m_plDomDocument.CreateInstance( MSXML::CLSID_DOMDocument );
   if ( FAILED( hr ) )
   {
      _com_error er( hr );
      AfxMessageBox( er.ErrorMessage( ) );
   // EndDialog( 1);
   }

   m_MonitorTomcat.LoadXML_Document( "c:\\10d\\a\\zdr\\MonitorProcess\\MonProc.xml" );

#else

   char szBuffer[ 4096 ];
   char szCurrentDir[ 256 + 1 ];
   int  nLth;

   GetCurrentDirectory( zsizeof( szCurrentDir ), szCurrentDir );
   nLth = strlen( szCurrentDir );
   if ( nLth && szCurrentDir[ nLth - 1 ] != '\\' )
      szCurrentDir[ nLth++ ] = '\\';

   strcpy( szCurrentDir + nLth, "MonProc.ini" );

   GetPrivateProfileString( "Process",          // section name
                            "Name",             // key name
                            "java",             // default string
                            szBuffer,           // destination buffer
                            zsizeof( szBuffer ), // size of destination buffer
                            szCurrentDir );     // ini filename
   m_csProcess = szBuffer;

   GetPrivateProfileString( "WriteLog",  "Executable",  "kzoengwa",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_csWriteLog = szBuffer;
   GetPrivateProfileString( "WriteLog",  "Message",  "1025",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_nWriteLogMessage = atoi( szBuffer );
   GetPrivateProfileString( "QueryAlive",  "Message",  "1026",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_nQueryAliveMessage = atoi( szBuffer );
   GetPrivateProfileString( "WriteLog",  "WindowTitle",  "Zeidon Object Services",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_csWindowTitle = szBuffer;
   GetPrivateProfileString( "WriteLog",  "WindowClass",  "KZOENGWAWClass",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_csWindowClass = szBuffer;

   GetPrivateProfileString( "KillApp",  "Command",  "C:\\B\\bin\\KillTomcat.cmd",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_csKillApp = szBuffer;
   GetPrivateProfileString( "KillApp",  "Delay",  "2000",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_nKillAppDelay = atoi( szBuffer );

   GetPrivateProfileString( "Startup",  "Command",  "C:\\Program Files\\Apache Group\\Tomcat 6.0\\bin\\startup.bat",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_csStartup = szBuffer;
   GetPrivateProfileString( "Shutdown",  "Command",  "C:\\Program Files\\Apache Group\\Tomcat 6.0\\bin\\shutdown.bat",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_csShutdown = szBuffer;
   GetPrivateProfileString( "Startup",  "Delay",  "20000",
                            szBuffer, zsizeof( szBuffer ), szCurrentDir );
   m_nStartupDelay = atoi( szBuffer );

   sprintf( szBuffer, "Using Ini File: %s   Process: %s   Engine: %s   WriteLog Message: WM_USER+%d\n"
                      "   QueryAlive Message: WM_USER+%d   WindowTitle: %s   WindowClass: %s\n"
                      "   KillAppCommand: %s   KillAppDelay: %d\n"
                      "   StartupCommand: %s   StartupDelay: %d\n"
                      "   ShutdownCommand: %s",
            szCurrentDir, m_csProcess, m_csWriteLog, m_nWriteLogMessage, m_nQueryAliveMessage, m_csWindowTitle,
            m_csWindowClass, m_csKillApp, m_nKillAppDelay, m_csStartup, m_nStartupDelay, m_csShutdown );
   ::MessageBox( m_hWnd, szBuffer, "MonProc Parameters", MB_OK );
#endif

   return( TRUE );
}

void
CMonProcDlg::OnGetAllProcs( )
{
   // Clear the list box...
   m_ctrlProcLB.ResetContent( );
   CWaitCursor wait;
   if ( !m_win32proc.EnumAllProcesses( ) )
   {
      AfxMessageBox( m_win32proc.GetLastError( ) );
      return;
   }

   // Populate the list box.
   int size = m_win32proc.GetAllProcessesNames( )->GetSize( );
   for ( int k = 0; k < size; k++ )
      m_ctrlProcLB.AddString( m_win32proc.GetAllProcessesNames( )->GetAt( k ) );
}

void
CMonProcDlg::OnCheckIfActive( )
{
   CWaitCursor wait;
   bool bStatus;

   UpdateData( TRUE );
   if ( m_strProcessToCheck == "" )
   {
      AfxMessageBox( "You must supply a process name to check for!" );
      return;
   }

   if ( !m_win32proc.GetProcessStatus( &m_strProcessToCheck, &bStatus ) )
   {
      AfxMessageBox( m_win32proc.GetLastError( ) );
      return;
   }

   if ( bStatus )
      AfxMessageBox( m_strProcessToCheck + " is running!" );
   else
      AfxMessageBox( m_strProcessToCheck + " is NOT running!" );
}

void
PumpMessages( )
{
   MSG     msg;
   while ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
   {
      ::TranslateMessage( &msg );
      ::DispatchMessage( &msg );
   }
}

#define DEBUG_TRACE

void
CMonProcDlg::OnCheckTomcatButton( )
{
   CString csTomcat = "java";
   CWnd    *pWnd = 0;
   bool    bStatus;
#ifdef DEBUG_TRACE
   bool    bTrace = FALSE;
#endif
   int     nCnt;
   int     nStartupDelay = m_nStartupDelay / 20;
   long    lRC;

   if ( nStartupDelay < 100 )
      nStartupDelay = 100;

   CButton *pCheckButton = (CButton *) GetDlgItem( IDC_CHECKTOMCAT );
   CButton *pCancelButton = (CButton *) GetDlgItem( IDC_CANCELCHECKTOMCAT );

   pCheckButton->EnableWindow( FALSE );
   PumpMessages( );

   g_nCheckTomcat = 1;  // going to begin checking Tomcat
   while ( g_nCheckTomcat )
   {
      if ( !m_win32proc.GetProcessStatus( &csTomcat, &bStatus ) )
      {
         AfxMessageBox( m_win32proc.GetLastError( ) );
         return;
      }

      if ( pWnd == 0 && m_csWindowTitle.IsEmpty( ) == FALSE )
         pWnd = FindWindow( m_csWindowClass, m_csWindowTitle );

      if ( bStatus == FALSE ) // Tomcat not running
      {
         if ( m_nWriteLogMessage )
         {
            if ( pWnd && ::IsWindow( pWnd->m_hWnd ) )
            {
               ::SendMessage( pWnd->m_hWnd, WM_USER + m_nWriteLogMessage, 0, 0 );
               PumpMessages( );
            }
         }

         WinExec( m_csKillApp, SW_SHOW );
         pWnd = 0;
#ifdef DEBUG_TRACE
         TRACE0( "Tomcat not running\n" );
         bTrace = FALSE;
#endif
         Sleep( m_nKillAppDelay );  // two-second delay to let OE go down
         WinExec( m_csStartup, SW_SHOW );
#ifdef DEBUG_TRACE
         TRACE0( "WinExec Tomcat startup\n" );
#endif

         nCnt = 0;
         while ( g_nCheckTomcat && nCnt++ < 20 )
         {
            PumpMessages( );
            Sleep( nStartupDelay );  // twenty-second delay to let Tomcat start
         }
      }
      else
      {
#ifdef DEBUG_TRACE
         if ( bTrace == FALSE )
         {
            TRACE0( "Tomcat is running\n" );
            bTrace = TRUE;
         }
#endif
         if ( pWnd )
         {
            if ( ::IsWindow( pWnd->m_hWnd ) )
            {
               lRC = ::SendMessage( pWnd->m_hWnd, WM_USER + m_nQueryAliveMessage, 0, 0 );
               PumpMessages( );
            }
            else
               lRC = 0;
         }
         else
            lRC = -1;

         if ( lRC != -1 )
         {
         // ::SendMessage( pWnd->m_hWnd, WM_USER + m_nWriteLogMessage, 0, 0 );  doesn't exist
         // PumpMessages( );
            WinExec( m_csKillApp, SW_SHOW );   // stop OE
            pWnd = 0;
#ifdef DEBUG_TRACE
            TRACE0( "Zeidon not running\n" );
            bTrace = FALSE;
#endif
            WinExec( m_csShutdown, SW_SHOW );  // stop Tomcat
            Sleep( m_nKillAppDelay );  // two-second delay to let OE go down
            WinExec( m_csStartup, SW_SHOW );
#ifdef DEBUG_TRACE
            TRACE0( "WinExec Tomcat2 startup\n" );
#endif
         }
      }

      if ( g_nCheckTomcat == 1 )  // beginning to check Tomcat
      {
         g_nCheckTomcat = 2;  // check for Tomcat now in full swing
         pCheckButton->ShowWindow( SW_HIDE );
         pCancelButton->ShowWindow( SW_SHOW );
      }

      PumpMessages( );
      Sleep( 1000 );  // one-second delay to let other apps work
   }
}

void
CMonProcDlg::CancelCheckTomcat( )
{
   g_nCheckTomcat = 0;
}

void
CMonProcDlg::OnCancelCheckTomcat( )
{
   CButton *pCheckButton = (CButton *) GetDlgItem( IDC_CHECKTOMCAT );
   CButton *pCancelButton = (CButton *) GetDlgItem( IDC_CANCELCHECKTOMCAT );

   pCheckButton->EnableWindow( TRUE );
   pCheckButton->ShowWindow( SW_SHOW );
   pCancelButton->ShowWindow( SW_HIDE );
   PumpMessages( );
   CancelCheckTomcat( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
void
ZProcess::ParseXML_Document( )
{
   // For each node we store data

   // If it's a Process Node, and the Root node
   if ( IsRoot( ) && IsTag( "<PROCESS>" ) )
   {
      // Handle Attributes
      if ( IsValidAttribute( "name" ) )
         SetProcessName( GetAttributeValue( ) );
   }

   // If it's the WriteLog Node in a Process node
   if ( IsTag( "<WRITELOG>" ) && IsChildOf( "<PROCESS>" ) )
   {
      // Set the WriteLog File Name
      SetWriteLogValue( GetTextValue( ) );

      // Handle Attributes
      if ( IsValidAttribute( "name" ) )
         this->SetWriteLogName( GetAttributeValue( ) );
   }

   // If it's the KillApp Node in a Process node
   if ( IsTag("<SHUTDOWN>") && IsChildOf( "<PROCESS>") )
   {
      // Set the KillApp command name
      SetKillAppValue( GetTextValue( ) );

      // Handle Attributes
      if ( IsValidAttribute( "delay" ) )
      {
         int delay;
         sscanf( GetAttributeValue( ) , "%d" , &delay );
         SetKillAppDelay( delay );
      }
   }

   // If it's the Startup Node in a Process node
   if ( IsTag("<STARTUP>") && IsChildOf( "<PROCESS>") )
   {
      // Set the Startup command name
      SetStartupValue( GetTextValue( ) );

      // Handle Attributes
      if ( IsValidAttribute( "delay" ) )
      {
         int delay;
         sscanf( GetAttributeValue( ) , "%d" , &delay );
         SetStartupDelay( delay );
      }
   }

   // If it's the NOTE node in a PROCESS node
   if ( IsTag("<NOTE>") && IsChildOf( "<PROCESS>") )
   {
      // do nothing ... just checking
   }

   // If it's the CDATA section on NOTE node
   if ( IsChildOf( "<NOTE>" ) && IsCDataSection( ) )
   {
      // Set the Note value
      CString val;

      val = GetTextValue( );
      SetNote( val );
   }
}

void
ZProcess::SetProcessName( LPCTSTR Name )
{
   m_csProcessName = Name;
}

void
ZProcess::SetWriteLogName( LPCTSTR Name )
{
   m_csWriteLogName = Name;
}

void
ZProcess::SetWriteLogValue( LPCTSTR Value )
{
   m_csWriteLogValue = Value;
}

void
ZProcess::SetKillAppName( LPCTSTR Name )
{
   m_csKillAppName = Name;
}

void
ZProcess::SetKillAppValue( LPCTSTR Value )
{
   m_csKillAppValue = Value;
}
void
ZProcess::SetKillAppDelay( int nDelay )
{
   m_lKillAppDelay = nDelay;
}

void
ZProcess::SetStartupName( LPCTSTR Name )
{
   m_csStartupName = Name;
}

void
ZProcess::SetStartupValue( LPCTSTR Value )
{
   m_csStartupValue = Value;
}

void
ZProcess::SetStartupDelay( int nDelay )
{
   m_lStartupDelay = nDelay;
}

void
ZProcess::SetNote( LPCTSTR Note )
{
   m_csNote = Note;
}

void
ZProcess::GetNote( CString& csNote )
{
   csNote = m_csNote;
}
#endif

#if 0

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
   Save_XML_Document( "SpiderMan.xml" );

   Reset_XML_Document( );  // Reset Content of XML Document
   SetHeader( "xml", "encoding", "UTF-" ); // Set the encoding property

   // Create the root node.
   // ( We use AddFirstChildNode because we have no node;
   // it creates the root node. )
   AddFirstChildNode( "MOVIE" );
   SetAttribute( "filename", "SpiderMan.avi" );

   // Now the current node is the <MOVIE> Node ( the root node ).
   // To move into the document, start by creating the <ACTORS>
   // node.
   AddFirstChildNode( "ACTORS" );

   // Now the current node is the <ACTORS> node.
   AddLastChildNode( "ACTOR" );
   SetTextValue( "Willem Dafoe" );

   // Now the current node is the <ACTOR> node.
   // We can't use AddFirstChildNode or AddLastChildNode to
   // create the second <ACTOR> node because it's not a child
   // node for the first <ACTOR> that was just created.
   // We have two ways to accomplish this: Move to the parent Node
   // <ACTORS> or create directly before the current node.
   //

   // First way -->
   GoToParent( "ACTORS" );
   AddFirstChildNode( "ACTOR" );
   SetTextValue( "Tobey Maguire" );

   // Second way -->
   AddNodeBefore( "ACTOR" ); // Add a new node before the current node
                            // ( both have same parent ).
   SetText( "Tobey Maguire" );

   // Add <NAME> && <LENGTH> nodes
   GoToRoot( );
   AddFirstChildNode( "LENGTH" );
   SetAttribute( "value", "110" );
   GoToRoot( );
   AddFirstChildNode( "Name" );
   SetTextValue( "SpiderMan" );

   // Add the Director Node. We move to the ACTORS node, and
   // we create the DIRECTOR node after this.
   Go_Forward( "ACTORS" );
   AddNodeAfter( "DIRECTOR" );
   SetTextValue( "Sam Raimi" );

   // Add both Subtitle node, at least two ways are possible:

   // First way -->
   GoToRoot( );
   AddLastChildNode( "SUBTITLE" );
   SetTextValue( "French" );
   GoToRoot( );
   AddLastChildNode( "SUBTITLE" );
   SetTextValue( "English" );

   // Second way -->
   AddNodeAfter( "SUBTITLE" );
   SetTextValue( "French" );
   AddNodeAfter( "SUBTITLE" );
   SetTextValue( "English" );

   // Third way -->
   AddNodeAfter( "SUBTITLE" );
   SetTextValue( "English" );
   AddNodeBefore( "SUBTITLE" );
   SetTextValue( "French" );

#endif

void
CMonProcDlg::OnOK( )
{
   if ( g_nCheckTomcat )
   {
      char szMsg[ 256 ];

      sprintf( szMsg, "Do you want to cancel the monitor of process: %s", m_csProcess );

      int nRC = MessageBox( szMsg, "Monitor Process", MB_OKCANCEL );
      if ( nRC != IDOK )
         return;
   }

   CancelCheckTomcat( );
   CDialog ::OnOK( );
}

void
CMonProcDlg::OnClose( )
{
   CancelCheckTomcat( );
   CDialog ::OnClose( );
}
