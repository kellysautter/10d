/////////////////////////////////////////////////////////////////////////////
// Project zdr
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdr.exe Application
// FILE:         zdr.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definition for ZDrApp (CWinApp).
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2000.03.29    TMV   Z10
//    move
//    const VARIANT_BOOL VAR_TRUE = ~VARIANT_FALSE;
//    const VARIANT_BOOL VAR_FALSE = VARIANT_FALSE;
//    to zdr.h to make it accessible in ActiveX DLLs
//
// 1999.11.23    TMV   ACTIVEX
//    include additional header "#include <afxctl.h>"
//

#ifndef __AFXWIN_H__
   #error include 'zstdafx.h' before including this file for PCH
#endif

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

#ifndef __zdr_h      // Sentry, use file only if it's not already included
#define __zdr_h

#include "zdrapp.h"   // classes exported from ZDrApp


//#ifdef ZDR_CLASS
//   #undef  AFX_DATA
//   #define AFX_DATA AFX_EXT_DATA
//#else
//   #define ZDR_CLASS
//   #define ZDR_CLASS_DEFINED
//#endif

#define zTASK_STATE_INIT            (zUSHORT) 0x0000
#define zTASK_STATE_PRINT           (zUSHORT) 0x1000
#define zTASK_STATE_CLOSE           (zUSHORT) 0x2000
#define zTASK_STATE_PREPAGE         (zUSHORT) 0x4000
#define zTASK_STATE_TERMINATED      (zUSHORT) 0x0001

const VARIANT_BOOL VAR_TRUE  = ~VARIANT_FALSE;
const VARIANT_BOOL VAR_FALSE = VARIANT_FALSE;


class ZDxView;
class ZDxDoc;

/////////////////////////////////////////////////////////////////////////////
// ZDrApp:
// See ZDr.cpp for the implementation of this class
//

class ZDR_CLASS ZDrApp : public CWinApp
{
friend class ZDrView;
public:
   ZDrApp( );
   virtual ~ZDrApp( );

// Attributes
   ZTask    *m_pZTask;
   ZDrDoc   *m_pDoc;           // pointer to the app's one document
   HWND     m_hWndCommOE;      // hWnd through which the OE
                               // communicates with DR
   zVIEW    m_vSubtask;        // Original view from RegisterZeidonApplication
   CString  *m_pzsZeidonWnd;
   CString  *m_pzsZeidonFrameOrView;
   CString  *m_pzsZeidonMDI_Frame;
   zBOOL    m_bThreadStarted;
   ZSubtask *m_pZSubtaskCurr;

   HGLOBAL  GetDevMode( ) { return( m_hDevMode ); }

// Overrides
   virtual BOOL InitInstance( );
   virtual int  ExitInstance( );
   virtual BOOL OnIdle( LONG lCount );

   void SetOrientation( zBOOL bLandscape );
   void SetDialogBkColor( COLORREF clrCtlBk = RGB( 192, 192, 192 ),
                          COLORREF clrCtlText = RGB( 0, 0, 0 ) );
   void GetCommandLineParser( CStringArray& params, CStringArray& switches );

protected:
   zLONG CheckAndCleanProcesses( );
   void  OnAppExit( );

public:
// Implementation
   //{{AFX_MSG(ZDrApp)
   afx_msg void OnAppAbout( );
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG

private:
   ZCommandLineInfo m_cmdParse;

   DECLARE_MESSAGE_MAP( )

}; // class ZDrApp

/////////////////////////////////////////////////////////////////////////////
// ZDlgUnits dialog

class ZDlgUnits : public CDialog
{
// Construction
public:
   ZDlgUnits( CWnd *pParent = NULL );   // standard constructor

// Dialog Data
   //{{AFX_DATA(ZDlgUnits)
   enum { IDD = IDD_DIALOGUNITS };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDlgUnits)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions.
   //{{AFX_MSG(ZDlgUnits)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDR_CLASS ZDxFrame : public CFrameWnd
{
protected: // create from serialization only
   ZDxFrame( );
   DECLARE_DYNCREATE( ZDxFrame )

// Attributes
protected:
   ZSplitterWnd *m_wndSplitter;
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDxFrame)
   public:
   virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext *pContext );
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   //}}AFX_VIRTUAL

   LRESULT OnRemoteSocket( WPARAM wParam, LPARAM lParam );
   LRESULT OnRemoteGetWnd( WPARAM wParam, LPARAM lParam );
   LRESULT OnRemotePrintReport( WPARAM wParam, LPARAM lParam );

// Implementation
public:
   virtual ~ZDxFrame( );
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

   ZDrApp      *m_pZDrApp;
   ZDxView     *m_pZDxView;

protected:  // control bar embedded members
   CStatusBar  m_wndStatusBar;
   CToolBar    m_wndToolBar;

public:
   CListCtrl   m_wndListCtrl;

// Generated message map functions
protected:
   //{{AFX_MSG(ZDxFrame)
   afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSize( UINT nType, int cx, int cy );
   afx_msg void OnDestroy( );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDR_CLASS ZDxDoc : public CDocument
{
protected: // create from serialization only
   ZDxDoc( );
   DECLARE_DYNCREATE( ZDxDoc )

// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDxDoc)
   public:
   virtual BOOL OnNewDocument( );
   virtual void Serialize( CArchive& ar );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZDxDoc( );
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

   ZDxFrame  *m_pZDxFrame;

protected:

// Generated message map functions
protected:
   //{{AFX_MSG(ZDxDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDR_CLASS ZDxView : public CView
{
protected: // create from serialization only
   ZDxView( );
   DECLARE_DYNCREATE( ZDxView )

public:
   virtual ~ZDxView( );
   ZDxDoc* GetDocument( );

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDxView)
   public:
   virtual void OnDraw( CDC *pDC );  // overridden to draw this view
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

   protected:

   //}}AFX_VIRTUAL

// Generated message map functions
protected:
   //{{AFX_MSG( ZDxView )
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   //}}AFX_MSG

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

   zVIEW     m_vDialog;

// Attributes
protected:
   ZDxFrame  *m_pZDxFrame;
   ZSubtask  *m_pZSubtask;           // Pointer to related ZSubtask

   DECLARE_MESSAGE_MAP( )
};

#ifndef _DEBUG  // debug version in ZDxView.cpp
inline ZDxDoc * ZDxView::GetDocument( )
   { return (ZDxDoc *) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zWEB_COMMUNICATION_BUFFER_SIZE 8000

typedef struct ConnectionStruct ConnectionRecord;
typedef struct ConnectionStruct *LPCONN;
struct ConnectionStruct
{
   SOCKET  sock;           // Socket to send/receive.
   LPSTR   pszBuffer;      // Ptr to allocated buffer.
   zULONG  ulBufferLth;    // Total # of bytes in buffer--used and unused.
   zULONG  ulBlockLth;     // Total # of bytes in buffer--used and unused.
   zLONG   lFileHandle;    // File handle if also writing XML to file
   zCHAR   cMore;
};

struct ErrorMessageStruct
{
   int    iErrorNum;
   zPCHAR pszErrorMessage;
};

void
PrintHelp( );
void
fnShowError( zPCHAR pszFuncName );
zSHORT OPERATION
fnSend( zULONG lControl,
        zPCHAR pszText,
        zULONG uLth,
        zSHORT nLevel,
        zPVOID lpData );
zSHORT
fnSendXML( SOCKET sock, zBOOL bContinue, zPCHAR pszXML );
zSHORT OPERATION
fnRead( zPPCHAR ppszReturnBuffer,
        zPULONG pulReturnLth,
        zPVOID  lpData );

#else    // #ifdef zREMOTE_SERVER

#define TITLE_SIZE          64
#define PROCESS_SIZE        MAX_PATH
#define MAX_TASKS           256

//
// task list structure
//
typedef struct _TASK_LIST
{
   DWORD       dwProcessId;
   DWORD       dwInheritedFromProcessId;
   BOOL        flags;
   HWND        hwnd;
   UCHAR       ProcessName[ PROCESS_SIZE ];
   UCHAR       WindowTitle[ TITLE_SIZE ];
} TASK_LIST, *PTASK_LIST;

typedef struct _TASK_LIST_ENUM
{
    PTASK_LIST  tlist;
    DWORD       numtasks;
} TASK_LIST_ENUM, *PTASK_LIST_ENUM;

// Function pointer types for accessing platform-specific functions.
typedef DWORD (*LPGetTaskList)( PTASK_LIST, DWORD );
typedef zBOOL (*LPEnableDebugPriv)( );

// Function prototypes.
DWORD
GetTaskList95( PTASK_LIST pTask,
               DWORD      dwNumTasks );

DWORD
GetTaskListNT( PTASK_LIST pTask,
               DWORD      dwNumTasks );


zBOOL
EnableDebugPriv95( );

zBOOL
EnableDebugPrivNT( );

zBOOL
KillProcess( PTASK_LIST tlist,
             zBOOL      bForce );

void
GetWindowTitles( PTASK_LIST_ENUM te );

zBOOL
MatchPattern( PUCHAR String,
              PUCHAR Pattern );

BOOL CALLBACK EnumWindowsProc( HWND  hWnd,
                               LONG  lParam );

#endif   // #ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif   // __zdr_h sentry.
