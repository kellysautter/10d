// MonProcDlg.h : header file
//

#if !defined( AFX_MonProcDLG_H__3F3A24B7_FFCD_11D1_A30F_0000B481E689__INCLUDED_ )
#define AFX_MonProcDLG_H__3F3A24B7_FFCD_11D1_A30F_0000B481E689__INCLUDED_

#include "Win32Process.h"  // Added by ClassView
#include "XML_PARSER.h"  // Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if 0
/////////////////////////////////////////////////////////////////////////////
// ZProcess class

class ZProcess: public ZXmlParser
{
public:
   void ParseXML_Document( );   // NEEDED to PARSE the LOADED Document

   void SetProcessName( LPCTSTR Name );

   void SetWriteLogName( LPCTSTR Name );
   void SetWriteLogValue( LPCTSTR Value );

   void SetKillAppName( LPCTSTR Name );
   void SetKillAppValue( LPCTSTR Value );
   void SetKillAppDelay( int nDelay );

   void SetStartupName( LPCTSTR Name );
   void SetStartupValue( LPCTSTR Value );
   void SetStartupDelay( int nDelay );

   void SetNote( LPCTSTR Note );
   void GetNote( CString& csNote );

private:
   CString m_csProcessName;
   CString m_csWriteLogName;
   CString m_csWriteLogValue;
   CString m_csKillAppName;
   CString m_csKillAppValue;
   long    m_lKillAppDelay;
   CString m_csStartupName;
   CString m_csStartupValue;
   long    m_lStartupDelay;
   CString m_csNote;
};
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonProcDlg dialog

class CMonProcDlg : public CDialog
{
// Construction
public:
   CMonProcDlg( CWnd *pParent = NULL );   // standard constructor
   void CancelCheckTomcat( );

// ZProcess m_MonitorTomcat;

// Dialog Data
   //{{AFX_DATA( CMonProcDlg )
   enum { IDD = IDD_MonProc_DIALOG };
   CListBox m_ctrlProcLB;
   CString  m_strOs;
   CString  m_strProcessToCheck;
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CMonProcDlg )
   protected:
   virtual void DoDataExchange( CDataExchange *pDX ); // DDX/DDV support
   //}}AFX_VIRTUAL

//protected:
//	IXMLDOMDocumentPtr m_plDomDocument;
//	IXMLDOMElementPtr m_pDocRoot;

private:
   CString m_csProcess;
   CString m_csWriteLog;
   int     m_nWriteLogMessage;
   int     m_nQueryAliveMessage;
   CString m_csKillApp;
   int     m_nKillAppDelay;
   CString m_csStartup;
   CString m_csShutdown;
   int     m_nStartupDelay;
   CString m_csWindowClass;
   CString m_csWindowTitle;


// Implementation
protected:
   Win32Process m_win32proc;
   HICON m_hIcon;

   // Generated message map functions
   //{{AFX_MSG( CMonProcDlg )
   virtual BOOL OnInitDialog( );
   afx_msg void OnGetAllProcs( );
   afx_msg void OnCheckIfActive( );
   afx_msg void OnCheckTomcatButton( );
   afx_msg void OnCancelCheckTomcat( );
	virtual void OnOK( );
	afx_msg void OnClose( );
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined( AFX_MonProcDLG_H__3F3A24B7_FFCD_11D1_A30F_0000B481E689__INCLUDED_ )
