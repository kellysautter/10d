// ttblbrwsDlg.h : header file
//

#if !defined(AFX_TTBLBRWSDLG_H__DBF8E2B9_D176_11D2_9C83_00608C72BD76__INCLUDED_)
#define AFX_TTBLBRWSDLG_H__DBF8E2B9_D176_11D2_9C83_00608C72BD76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTtblbrwsDlg dialog

class CTtblbrwsDlg : public CDialog
{
// Construction
public:
   CTtblbrwsDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
   //{{AFX_DATA(CTtblbrwsDlg)
   enum { IDD = IDD_TTBLBRWS_DIALOG };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTtblbrwsDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   HICON m_hIcon;

   // Generated message map functions
   //{{AFX_MSG(CTtblbrwsDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   virtual void OnOK();
   virtual void OnCancel();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTBLBRWSDLG_H__DBF8E2B9_D176_11D2_9C83_00608C72BD76__INCLUDED_)
