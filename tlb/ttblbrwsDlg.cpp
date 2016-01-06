// ttblbrwsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttblbrws.h"
#include "ttblbrwsDlg.h"
#include "tlb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTtblbrwsDlg dialog

CTtblbrwsDlg::CTtblbrwsDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CTtblbrwsDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CTtblbrwsDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTtblbrwsDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTtblbrwsDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTtblbrwsDlg, CDialog)
   //{{AFX_MSG_MAP(CTtblbrwsDlg)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTtblbrwsDlg message handlers

BOOL CTtblbrwsDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hIcon, FALSE);      // Set small icon

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTtblbrwsDlg::OnPaint()
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDialog::OnPaint();
   }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTtblbrwsDlg::OnQueryDragIcon()
{
   return (HCURSOR) m_hIcon;
}

void CTtblbrwsDlg::OnOK()
{
   // TODO: Add extra validation here
   CString strFilter = "OCX (*.ocx)|*.ocx|Type libraries(*.tlb)|*.tlb|Dynamic Link Libraries(*.dll)|*.dll|All Files (*.*)|*.*||";
   CString strHeaderName, strCPPName;
   CTypeLibrary l;
   CFileDialog d(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST , strFilter, this );

   if( d.DoModal() == IDOK )
   {
      strHeaderName = d.GetPathName();
      l.LoadTypeLibrary(strHeaderName);
      l.GenerateCode(strHeaderName, "TBEDIT", "ZDrTBEdt");
   }

   CDialog::OnOK();
}

void CTtblbrwsDlg::OnCancel()
{
   // TODO: Add extra cleanup here

   AfxMessageBox("You are a coward!");
}
