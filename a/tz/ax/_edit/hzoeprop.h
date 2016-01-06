/*
**----------------------------------------------------------------------
** .Klasse:        CPropertyDlg
**
** .Version:       01A
**
** .Aenderung:     22.03.1999
**
** .Beschreibung:  Property Dialogklasse des TBEdit Controls
**
** .Basisklassen:  CDialog
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      CPropertyDlg erzeugt den Propertydialog des Edit-Controls zum
**      festlegen der Farben, des Fonts und der Tabstops
**
**      Die Source ist MFC-generiert.
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (22.03.1999)  SL
** 01B  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**----------------------------------------------------------------------
*/

#if !defined(AFX_PROPERTYDLG_H__65E17623_D62F_11D2_8C3A_004005512552__INCLUDED_)
#define AFX_PROPERTYDLG_H__65E17623_D62F_11D2_8C3A_004005512552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "colpick.h"



class CPropertyDlg : public CDialog
{
// Construction
public:
   CPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CPropertyDlg)
   enum { IDD = IDD_PROPERTY };
   CColourPicker  m_Keyword;
   CColourPicker  m_Foreground;
   CColourPicker  m_Comment;
   CColourPicker  m_Background;
   CString  m_TabEdit;
   //}}AFX_DATA

    COLORREF m_crBackground;
    COLORREF m_crForeground;
    COLORREF m_crComment;
    COLORREF m_crKeyword;
   BOOL m_bFontChange;         //TRUE->Font wurde gewechselt/Veraendert
    long m_FontSize;            //Groesse des Fonts
   CHARFORMAT m_cf;            //neues CHARFORMAT
    LOGFONT m_lf;
    long m_lTabStop;            //Tabstops
    BOOL m_bWantSave;           //User will Werte gespeichert haben


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CPropertyDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   CWnd *m_pParent;

   // Generated message map functions
   //{{AFX_MSG(CPropertyDlg)
   afx_msg void OnApply();
   afx_msg void OnFont();
   virtual void OnOK();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLG_H__65E17623_D62F_11D2_8C3A_004005512552__INCLUDED_)
