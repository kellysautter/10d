/*
**----------------------------------------------------------------------
** .Klasse:        CGotoDlg
**
** .Version:       01B
**
** .Aenderung:     11.03.1999
**
** .Beschreibung:  Goto-Dialogklasse des TBEdit Controls
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
**      Eine Vorbelegung der Werte erfolgt ueber das erste
**      Konstruktorargument ( long ).
**
**      Die Source ist MFC-generiert.
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (15.05.1998)  SL
** 01B  =  Klasse CTabDlg wird nicht mehr benoetigt. Entfernt
**         (11.03.1999)  SL
** 01C  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "TBEdit.h"
#include "hzoedlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGotoDlg dialog


CGotoDlg::CGotoDlg( long lLine, CWnd* pParent /*=NULL*/)
   : CDialog(CGotoDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CGotoDlg)
   m_lData = 0;
   //}}AFX_DATA_INIT

    m_lData = lLine;
}


void CGotoDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CGotoDlg)
   DDX_Text(pDX, IDC_GOTO, m_lData);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoDlg, CDialog)
   //{{AFX_MSG_MAP(CGotoDlg)
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGotoDlg message handlers

void CGotoDlg::OnOK()
{
   CDialog::OnOK();
}


BOOL CGotoDlg::OnInitDialog( )
{
    CDialog::OnInitDialog();

    GotoDlgCtrl( GetDlgItem( IDC_GOTO ) );
    return FALSE;
}


