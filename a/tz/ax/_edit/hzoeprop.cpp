
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

#include "stdafx.h"
#include "tbedit.h"
#include "hzoeprop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlg dialog


CPropertyDlg::CPropertyDlg( CWnd* pParent )
    : CDialog(CPropertyDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPropertyDlg)
    m_TabEdit = _T("4");
    //}}AFX_DATA_INIT

    m_crBackground = RGB( 255,255,255 );
    m_crForeground = RGB( 0,0,0 );
    m_crKeyword = RGB( 255,0,0 );
    m_crComment = RGB( 0,0,255 );

    m_pParent = pParent;
    m_bFontChange = FALSE;

    m_lTabStop = 0;
    m_bWantSave = FALSE;
}


void CPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyDlg)
    DDX_Control(pDX, IDC_KEYWORDS, m_Keyword);
    DDX_Control(pDX, IDC_FOREGROUND, m_Foreground);
    DDX_Control(pDX, IDC_COMMENT, m_Comment);
    DDX_Control(pDX, IDC_BACKGROUND, m_Background);
    DDX_Text(pDX, IDC_TABSTOP, m_TabEdit);
    //}}AFX_DATA_MAP
    DDX_ColourPicker(pDX, IDC_BACKGROUND, m_crBackground);
    DDX_ColourPicker(pDX, IDC_FOREGROUND, m_crForeground);
    DDX_ColourPicker(pDX, IDC_COMMENT, m_crComment);
    DDX_ColourPicker(pDX, IDC_KEYWORDS, m_crKeyword);
}


BEGIN_MESSAGE_MAP(CPropertyDlg, CDialog)
    //{{AFX_MSG_MAP(CPropertyDlg)
    ON_BN_CLICKED(IDC_APPLY, OnApply)
    ON_BN_CLICKED(IDC_FONT, OnFont)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlg message handlers

void CPropertyDlg::OnApply()
{
    GetDlgItem( IDC_TABSTOP )->GetWindowText( m_TabEdit );
    m_lTabStop = atol( (LPCTSTR) m_TabEdit );

    m_bWantSave = FALSE;

    CDialog::OnOK();
}

void CPropertyDlg::OnFont()
{
    CHOOSEFONT csf = {0};
    LONG yPerInch;
    HDC hdc;

    //Vorbelegung der CHOOSEFONT und LOGFONT-Strukturen fuer
    //ChooseFont-Dialog
    ZeroMemory( &m_cf, zsizeof( CHARFORMAT ) );
    m_cf.cbSize = zsizeof(CHARFORMAT);
    ZeroMemory( &m_lf, sizeof( LOGFONT ) );

    hdc = ::GetDC( m_pParent->m_hWnd );
    yPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
    ::ReleaseDC( m_pParent->m_hWnd, hdc);

    ::SendMessage( m_pParent->m_hWnd, EM_GETCHARFORMAT, (WPARAM) TRUE,
                   (LPARAM) &m_cf);

    csf.lStructSize = zsizeof(csf);
    csf.hwndOwner = m_pParent->m_hWnd;
    csf.hDC = 0;
    csf.lpLogFont = &m_lf;
    csf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT |
                CF_FORCEFONTEXIST | CF_LIMITSIZE ;
    csf.nSizeMin = 1;
    csf.nSizeMax = yHeightCharPtsMost;
    csf.rgbColors = m_cf.crTextColor;
    csf.lpszStyle = NULL;
    csf.nFontType = REGULAR_FONTTYPE | SCREEN_FONTTYPE;

    m_lf.lfHeight = -(INT) ((m_cf.yHeight * yPerInch) / 1440);
    m_lf.lfWidth = 0;
    m_lf.lfEscapement = 0;
    m_lf.lfOrientation = 0;
    m_lf.lfWeight = (m_cf.dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
    m_lf.lfItalic = (m_cf.dwEffects & CFE_ITALIC) ? TRUE : FALSE;
    m_lf.lfUnderline = (m_cf.dwEffects & CFE_UNDERLINE) ? TRUE : FALSE;
    m_lf.lfStrikeOut = (m_cf.dwEffects & CFE_STRIKEOUT) ? TRUE : FALSE;
    m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    m_lf.lfQuality = DRAFT_QUALITY;
    m_lf.lfCharSet = m_cf.bCharSet;
    m_lf.lfPitchAndFamily = m_cf.bPitchAndFamily;
    lstrcpy( m_lf.lfFaceName, m_cf.szFaceName);

    if( ChooseFont( &csf ) )
    {
        ZeroMemory( &m_cf, zsizeof(CHARFORMAT) );
        m_cf.cbSize = zsizeof(CHARFORMAT);
        m_cf.dwMask = CFM_EFFECTS | CFM_FACE | CFM_SIZE | CFM_CHARSET;
        m_cf.yHeight = (LONG) csf.iPointSize * 2;
        lstrcpy(m_cf.szFaceName, m_lf.lfFaceName);
        m_cf.dwEffects = CFM_EFFECTS;
        if( m_lf.lfWeight < FW_BOLD)
            m_cf.dwEffects &= ~CFE_BOLD;
        if(! m_lf.lfItalic)
            m_cf.dwEffects &= ~CFE_ITALIC;
        if(! m_lf.lfUnderline)
            m_cf.dwEffects &= ~CFE_UNDERLINE;
        if(! m_lf.lfStrikeOut)
            m_cf.dwEffects &= ~CFE_STRIKEOUT;
        m_cf.bCharSet = ANSI_CHARSET;
        m_cf.bPitchAndFamily = FF_DONTCARE;


        m_FontSize = csf.iPointSize * 2;
        m_bFontChange = TRUE;
    }
}

void CPropertyDlg::OnOK()
{
    GetDlgItem( IDC_TABSTOP )->GetWindowText( m_TabEdit );
    m_lTabStop = atol( (LPCTSTR) m_TabEdit );

    m_bWantSave = TRUE;

    CDialog::OnOK();
}

BOOL CPropertyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Keyword.SetSelectionMode( CP_MODE_BK );
    m_Foreground.SetSelectionMode( CP_MODE_BK );
    m_Comment.SetSelectionMode( CP_MODE_BK );
    m_Background.SetSelectionMode( CP_MODE_BK );

    CString sAuto;
    CString sMore;

    sAuto.LoadString( IDS_AUTOMATIC );
    sMore.LoadString( IDS_MORECOLOR );

    m_Keyword.SetDefaultText( (LPCTSTR)sAuto );
    m_Keyword.SetCustomText( (LPCTSTR)sMore );
    m_Foreground.SetDefaultText( (LPCTSTR)sAuto );
    m_Foreground.SetCustomText( (LPCTSTR)sMore );
    m_Comment.SetDefaultText( (LPCTSTR)sAuto );
    m_Comment.SetCustomText( (LPCTSTR)sMore );
    m_Background.SetDefaultText( (LPCTSTR)sAuto );
    m_Background.SetCustomText( (LPCTSTR)sMore );

    char cHilf[100];
    _ltoa_s( m_lTabStop, cHilf, zsizeof( cHilf ), 10 );
    GetDlgItem( IDC_TABSTOP )-> SetWindowText( cHilf );

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
