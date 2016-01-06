#ifndef _HZOEDLG_H_
#define _HZOEDLG_H_

/*
**----------------------------------------------------------------------
** .Klasse:        CGotoDlg
**
** .Version:       01B
**
** .Aenderung:     11.03.1999
**
** .Beschreibung:  Dialogklassen des TBEdit Controls
**
** .Basisklassen:  CDialog
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      Klassen fuer die modalen Dialoge des Controls:
**      CGotoDlg -> Gehe zu... Position einlesen
**
**      Die eingelesenen Werte sind jeweils ueber das Attribut
**      m_lData abfragbar.
**
**      Die Source ist MFC-generiert
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 1999.11.04   DKS   Z10   Cosmetic (so Brief can read it).
** 01A  =  Neuerstellung
**         (15.05.1998)  SL
** 01B  =  Klasse CTabDlg wird nicht mehr benoetigt. Entfernt
**         (11.03.1999)  SL
** 01C  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**
**----------------------------------------------------------------------
*/


class CGotoDlg : public CDialog
{
// Construction
public:
   CGotoDlg(long lData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CGotoDlg)
   enum { IDD = IDD_GOTO };
   long  m_lData;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CGotoDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CGotoDlg)
   virtual void OnOK();
    virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#endif
