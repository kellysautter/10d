/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditor
**
** .Version:       01Q
**
** .Aenderung:     14.04.2000
**
** .Beschreibung:  Implementation des TBEditor Controls
**
** .Basisklassen:  COleControl
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      CTBEditor implementiert das Editor Control. Durch Ableitung von
**      COleControl ( MFC ) wird OLE-Faehigkeit erlangt. Die Editor-
**      funktionalitaet ergibt sich durch Subclassing des RichEdit-Controls
**      Version 1.0.
**
**      Die Klasse implementiert auch die OLE-Interfaces des Editor:
**      - _DTBEdit
**      - _DTBEditEvents
**
**      Die Methoden der Klasse sind auf drei Files verteilt:
**      - HZOECTL.CPP   -> Konstruktor/Destruktor, MessageHandling
**      - HZOEMETH.CPP  -> Methoden des Interface _DTBEdit
**      - HZOEFUNC.CPP  -> private Methoden
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 2002.08.22   DKS   Z10
**    When sending a message for EM_GETTEXTRANGE, if the range is zero,
**    the return buffer is not set to null.  So we do this explicitly.
** 01A  =  Neuerstellung
**         (18.05.1998)  SL
** 01B  =  Erweiterung der Methode PreTranslateMessage
**         Erweiterung fuer C-Syntax Kommentare
**         (22.06.1998)  SL
** 01C  =  Registry-Eintraege befinden sich in HKEY_CURRENT_USER\Software.
**         OnUpdate-Funktionen fuer Cut und Copy verbessert
**         (20.10.1998)  SL
** 01D  =  Voreinstellungen fuer Kommentarfarbe und Schriftart geaendert
**         (18.12.1998)  SL, TMV
** 01E  =  DestroyTracker() in OnSetFocus() eingefuegt.
**         (02.03.1999)  SL
** 01F  =  Neue Version des Controls ohne Menu. Deshalb sind alle Menue-
**         bezogenen Methoden weggefallen und durch einige Interface-
**         Methoden ersetzt worden.
**         (22.03,1999)  SL
** 01G  =  Einbindung des Controls in CFormView-Klassen ermoeglicht
**         durch Abfangen von WM_GETDLGCODE
**         (24.06.1999)  SL
** 01H  =  Erweitern des Returns von WM_GETDLGCODE um DLGC_WANTARROWS
**         (09.07.1999)  TMV
** 01I  =  Shift-Insert behandeln wie CTRL-V
**         Multiple Line Indention
**         (09.03.1999)  TMV
** 01J  =  Schließen von Subwindows (Suchen/ersetzen) in On Destroy
**         um Systemabsturz zu verhindern
**         (09.10.1999)  TMV
** 01K  =  Cosmetic (so Brief can read it)
**         (1999.11.04)  DKS
** 01L  =  Siron spezifische Funktionen auskommentiert
**         (1999.11.04)  TMV
** 01M  =  Überflüssige TRACE rausgenommen
**         (1999.12.21)  TMV
** 01N  =  Neue Funktion eingefügt
**         SelectRange
**         (2000.03.15)  TMV
** 01O  =  [Alt Gr] Taste sendet 2 Key downs einen mit VK_CONTROL und einen mit VK_MENU
**         [Alt Gr] neutralisiert
**         (2000.04.14)  TMV   TB1093
** 01P  =  Neue Funktion eingefügt
**         GetTextFromRange
**         (2000.07.20)  TMV
** 01Q  =  Avoid fireing Keystroke Event when CTRL or SHIFT key is pressed
**         Change OnChar to handle Returnkey properly together with undo
**         (2000.11.02)  TMV
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/

#include "stdafx.h"
#include "tbedit.h"
#include "hzoectl.h"
#include "RichEdit.h"
#include "hzocregs.h"
#include <commctrl.h>
#include "hzoedlg.h"

#pragma warning(disable: 4996) // This function or variable may be unsafe

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#define NEW_RICHEDIT

extern CTBEditApp NEAR theApp;



IMPLEMENT_DYNCREATE(CTBEditor, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

const UINT     wm_Find = RegisterWindowMessage( FINDMSGSTRING ) ;

BEGIN_MESSAGE_MAP(CTBEditor, COleControl)
   //{{AFX_MSG_MAP(CTBEditor)
   ON_WM_CREATE()
   ON_WM_CHAR()
   ON_WM_KEYDOWN()
   ON_WM_RBUTTONDOWN()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_SETFOCUS()
   ON_WM_DESTROY()
   ON_WM_ERASEBKGND()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_KEYUP()
   ON_WM_SYSKEYDOWN()
   ON_WM_GETDLGCODE()
   ON_REGISTERED_MESSAGE( wm_Find, OnFind )
   //}}AFX_MSG_MAP
   ON_MESSAGE(OCM_COMMAND, OnOcmCommand)
   ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CTBEditor, COleControl)
    //{{AFX_DISPATCH_MAP(CTBEditor)
    DISP_FUNCTION(CTBEditor, "OpenObject", OpenObject, VT_BOOL, VTS_PBSTR)
    DISP_FUNCTION(CTBEditor, "CloseObject", CloseObject, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "DeleteText", DeleteText, VT_I4, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "CutText", CutText, VT_I4, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "CopyText", CopyText, VT_I4, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "PasteText", PasteText, VT_I4, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "SyntaxOff", SyntaxOff, VT_EMPTY, VTS_NONE)
//  DISP_FUNCTION(CTBEditor, "SironSyntaxOn", SironSyntaxOn, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "ZeidonSyntaxOn", ZeidonSyntaxOn, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "PropertyDialog", PropertyDialog, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "GetCursorPosition", GetCursorPosition, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4)
    DISP_FUNCTION(CTBEditor, "GetLineCount", GetLineCount, VT_EMPTY, VTS_PI4)
    DISP_FUNCTION(CTBEditor, "InsertItem", InsertItem, VT_I4, VTS_PBSTR)
    DISP_FUNCTION(CTBEditor, "IsObjectChanged", IsObjectChanged, VT_EMPTY, VTS_PBOOL)
    DISP_FUNCTION(CTBEditor, "PrintObject", PrintObject, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "ReadOnlyModus", ReadOnlyModus, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CTBEditor, "SaveObject", SaveObject, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "SaveObjectAs", SaveObjectAs, VT_BOOL, VTS_PBSTR)
    DISP_FUNCTION(CTBEditor, "SelectItem", SelectItem, VT_BOOL, VTS_I4 VTS_PBSTR)
    DISP_FUNCTION(CTBEditor, "SetCursorPositionByIndex", SetCursorPositionByIndex, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CTBEditor, "SetCursorPositionByLine", SetCursorPositionByLine, VT_EMPTY, VTS_I4 VTS_I4)
    DISP_FUNCTION(CTBEditor, "Undo", Undo, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "Redo", Redo, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "TempSaveObject", TempSaveObject, VT_BOOL, VTS_PBSTR)
    DISP_FUNCTION(CTBEditor, "FindTextPosition", FindTextPosition, VT_I4, VTS_PBSTR VTS_PI4 VTS_I4)
    DISP_FUNCTION(CTBEditor, "GetActualTextLine", GetActualTextLine, VT_I4, VTS_PBSTR VTS_I4)
    DISP_FUNCTION(CTBEditor, "IsReadOnly", IsReadOnly, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "IsSubWindowActive", IsSubWindowActive, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "WantKeystrokes", WantKeystrokes, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CTBEditor, "DeleteTextRange", DeleteTextRange, VT_BOOL, VTS_I4 VTS_I4)
    DISP_FUNCTION(CTBEditor, "CloseSubWindow", CloseSubWindow, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "SearchForward", SearchForward, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "SearchBackward", SearchBackward, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "Deselect", Deselect, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "GetTextFromLineOfIndex", GetTextFromLineOfIndex,VT_I4, VTS_PBSTR VTS_I4 VTS_I4)
//  DISP_FUNCTION(CTBEditor, "SetSironLanguage", SetSironLanguage, VT_I4, VTS_I4)
    DISP_FUNCTION(CTBEditor, "GetPositionByIndex", GetPositionByIndex, VT_EMPTY, VTS_I4 VTS_PI4 VTS_PI4)
    DISP_FUNCTION(CTBEditor, "IsCommentAtIndex", IsCommentAtIndex, VT_BOOL, VTS_I4)
    DISP_FUNCTION(CTBEditor, "SearchWindow", SearchWindow, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "ReplaceWindow", ReplaceWindow, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "SelectAll", SelectAll, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "GotoWindow", GotoWindow, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "CanCopy", CanCopy, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "CanPaste", CanPaste, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "CanUndo", CanUndo, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "CanRedo", CanRedo, VT_BOOL, VTS_NONE)
    DISP_FUNCTION(CTBEditor, "SelectRange", SelectRange, VT_BOOL, VTS_I4 VTS_I4)
    DISP_FUNCTION(CTBEditor, "GetTextFromRange", GetTextFromRange,VT_I4, VTS_PBSTR VTS_I4 VTS_I4)
    DISP_FUNCTION(CTBEditor, "GetObjectName", GetObjectName,VT_I4, VTS_PBSTR )
    DISP_FUNCTION(CTBEditor, "GetSelectedText", GetSelectedText, VT_I4, VTS_PBSTR VTS_I4)
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CTBEditor, COleControl)
    //{{AFX_EVENT_MAP(CTBEditor)
    EVENT_CUSTOM("SetInsertMode", Fire_SetInsertMode, VTS_NONE)
    EVENT_CUSTOM("RightMouseClick", Fire_RightMouseClick, VTS_PBSTR)
    EVENT_CUSTOM("SetLine", Fire_SetLine, VTS_I4  VTS_I4)
    EVENT_CUSTOM("SetOverwriteMode", Fire_SetOverwriteMode, VTS_NONE)
    EVENT_CUSTOM("Keystroke", FireKeystroke, VTS_I2  VTS_PBOOL)
    //}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTBEditor, "TBEDIT.TBEditCtrl.1",
    0xdea843d5, 0xdaa2, 0x11d1, 0x95, 0xb2, 0, 0x60, 0x97, 0x3b, 0x3b, 0x55)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CTBEditor, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DTBEdit =
        { 0xdea843d3, 0xdaa2, 0x11d1, { 0x95, 0xb2, 0, 0x60, 0x97, 0x3b, 0x3b, 0x55 } };
const IID BASED_CODE IID_DTBEditEvents =
        { 0xdea843d4, 0xdaa2, 0x11d1, { 0x95, 0xb2, 0, 0x60, 0x97, 0x3b, 0x3b, 0x55 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwTBEditOleMisc =
    OLEMISC_ACTIVATEWHENVISIBLE |
    OLEMISC_SETCLIENTSITEFIRST |
    OLEMISC_INSIDEOUT |
    OLEMISC_CANTLINKINSIDE |
    OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CTBEditor, IDS_TBEDIT, _dwTBEditOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::CTBEditorFactory::UpdateRegistry -
// Adds or removes system registry entries for CTBEditor

BOOL CTBEditor::CTBEditorFactory::UpdateRegistry(BOOL bRegister)
{
    if (bRegister)
        return AfxOleRegisterControlClass(
            AfxGetInstanceHandle(),
            m_clsid,
            m_lpszProgID,
            IDS_TBEDIT,
            IDB_TBEDIT,
            afxRegInsertable | afxRegApartmentThreading,
            _dwTBEditOleMisc,
            _tlid,
            _wVerMajor,
            _wVerMinor);
    else
        return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

long CTBEditor::m_lTabStop = 4;  //alle 4 Zeichen


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      Konstruktor
**
** .Beschreibung:
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Initialisiert die Attribute der Klasse.
**      Es wird zunaechst nur die Version 1.0 des RichEdits verwendet, da
**      die 2.0 Probleme beim Subclassing macht. ( Unter ATL 2.1 gibt
**      es Absturz; unter MFC 4.2 werden die Zeichen nicht richtig
**      dargestellt.
**
**-----------------------------------------------------------------
*/
CTBEditor::CTBEditor()
{
    InitializeIIDs(&IID_DTBEdit, &IID_DTBEditEvents);

#ifdef NEW_RICHEDIT
    m_hLibRTF = LoadLibrary(_T("RICHED20.DLL")); // die neue Version
#else
    m_hLibRTF = LoadLibrary(_T("RICHED32.DLL")); // die alte Version
#endif
    m_hMenu = NULL;

    m_bWantKeys = FALSE;
    m_bReadOnly = FALSE;
    m_msgFind = 0;
    m_sFindDlg = NULL;
    m_pUndo = NULL;

    m_bShift = FALSE;
    m_bCtrl = FALSE;

    m_chNeu.cpMin = 0;
    m_chNeu.cpMax = 0;

    m_pLineBuf = new char[LINEBUF_SIZE+1];
    m_dLinebufSize = LINEBUF_SIZE;

    m_lComBufSize = 1000;
    m_pCommentBuf = new char[1000+1];
    m_dComSize = 0;
    m_dComLine = 0;
    m_dComNumberOfLines = 0;

    m_bPosChanged = FALSE;
    m_bSelection = FALSE;

    m_pBigBuf = NULL;

    m_bComment = FALSE;
    m_hWndModal = NULL;
    m_pSearchString = NULL;
    m_bMatchCase = FALSE;

    m_bInsertMode = TRUE;
    m_hFont = NULL;

    ZeroMemory( &m_cf, sizeof(CHARFORMAT) );
    m_cf.cbSize = sizeof(CHARFORMAT);
    m_cf.dwMask = CFM_COLOR;
    m_cf.dwEffects = 0;

    m_pSyntax = NULL;

    //Sprache bestimmen
    LANGID lg = PRIMARYLANGID( GetUserDefaultLangID() );
    if( lg == LANG_GERMAN )
        m_bGerman = TRUE;
    else if( lg == LANG_ENGLISH )
        m_bGerman = FALSE;
    else
        m_bGerman = FALSE;


    //FarbArray mit Weiss initialisieren; Wird von Farb-Dialog genutzt, um
    //selbstdefinierte Farbwerte zu hinterlegen
    for( int i=0; i < 17; i++ )
        m_crArray[i] = RGB( 255, 255, 255 );

    //Registry-Eintraege
    //zuerst zur Sicherheit vorbelegen
    m_crForeground = RGB( 0, 0, 0 );
    m_crBackground = RGB( 255, 255, 255 );
    m_crComment = RGB( 0, 0, 255 );
    m_crKeyword = RGB( 255, 0, 0 );
    lstrcpy( m_FontName, "Courier" );
    m_FontSize = 180;
    m_enSyntax = NO_SYNTAX;
    m_lUndoLevel = 30;
    m_lCommentBufferSize = 100;

    TBRegistry * pReg = new TBRegistry( REGISTRYKEY, (unsigned long)HKEY_CURRENT_USER );
    if( pReg->OpenKey() )
    {
        DWORD dTyp;
        DWORD dSize;

        dTyp = REG_SZ;
        dSize = LF_FACESIZE;
        pReg->ReadValue( REGFONT, &dTyp, (BYTE*)m_FontName, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_FontSize );
        pReg->ReadValue( REGSIZE, &dTyp, (BYTE*)&m_FontSize, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_crBackground );
        pReg->ReadValue( REGBACK, &dTyp, (BYTE*)&m_crBackground, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_crForeground );
        pReg->ReadValue( REGFORE, &dTyp, (BYTE*)&m_crForeground, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_crKeyword );
        pReg->ReadValue( REGWORD, &dTyp, (BYTE*)&m_crKeyword, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_crComment );
        pReg->ReadValue( REGCOMMENT, &dTyp, (BYTE*)&m_crComment, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_lTabStop );
        pReg->ReadValue( REGTABSTOP, &dTyp, (BYTE*)&m_lTabStop, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_lUndoLevel );
        pReg->ReadValue( REGUNDOLEVEL, &dTyp, (BYTE*)&m_lUndoLevel, &dSize );

        dTyp = REG_DWORD;
        dSize = sizeof ( m_lCommentBufferSize );
        pReg->ReadValue( REGBUFFERSIZE, &dTyp, (BYTE*)&m_lCommentBufferSize, &dSize );
    }

    delete pReg;

    InitializeFileName();
    InitCommonControls();   //die CommonDialog- Bibliothek initialisieren
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      Destruktor
**
** .Beschreibung:
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Freigabe der allokierten Ressourcen und der CommonDialog
**      Bibliothek.
**
**-----------------------------------------------------------------
*/

CTBEditor::~CTBEditor()
{
    if( m_hLibRTF )
    {
        FreeLibrary( m_hLibRTF );
        m_hLibRTF = NULL;
    }

    if( m_pSyntax )
    {
        delete m_pSyntax;
        m_pSyntax = NULL;
    }

    delete [] m_pLineBuf;
    delete [] m_pCommentBuf;

    if( m_hFont )
    {
        DeleteObject( m_hFont );
        m_hFont = NULL;
    }

    if( m_pUndo )
    {
        delete m_pUndo;
        m_pUndo = NULL;
    }

    if( m_pBigBuf )
    {
        delete [] m_pBigBuf;
        m_pBigBuf = NULL;
    }

    if( m_pSearchString )
    {
        delete [] m_pSearchString;
        m_pSearchString = NULL;
    }

}


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::OnDraw - Drawing function

void CTBEditor::OnDraw( CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
//  DoSuperclassPaint(pdc, rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::DoPropExchange - Persistence support

void CTBEditor::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CTBEditor::GetControlFlags()
{
    DWORD dwFlags = COleControl::GetControlFlags();

    dwFlags |= windowlessActivate;
    return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::OnResetState - Reset control to default state

void CTBEditor::OnResetState()
{
    COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::PreCreateWindow - Modify parameters for CreateWindowEx

BOOL CTBEditor::PreCreateWindow(CREATESTRUCT& cs)
{
#ifdef NEW_RICHEDIT
    cs.lpszClass = _T("RichEdit20A");//neue Version
#else
    cs.lpszClass = RICHEDIT_CLASS; //alte Version
#endif
    cs.style = cs.style|ES_MULTILINE|ES_SELECTIONBAR|ES_AUTOVSCROLL|ES_AUTOHSCROLL|
                    ES_NOHIDESEL| WS_VSCROLL|WS_HSCROLL;

    return COleControl::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CTBEditor::IsSubclassedControl - This is a subclassed control

BOOL CTBEditor::IsSubclassedControl()
{
    return TRUE;
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnChar
**
** .Beschreibung: Handler fuer WM_CHAR - Event
**
** .Parameter:    UINT, nChar   , I,  das eingegebene Zeichen
**                UINT, nRepCnt , I,  RepeatCounter der Taste
**                UINT, nFlags  , I,  Flags ( ALT, CRTL ... )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Hier kommen nur abdruckbare Zeichen an, die fuer das RichEdit
**      bestimmt sind. Die Abarbeitungsreihenfolge:
**      - Ueberreste der Ctrl-Tastenkombinationen verschlucken
**      - wenn Grosschreibung eingestellt -> alle Zeichen umsetzen
**      - Undo-Geschaeft regeln vor RichEdit
**      - das Zeichen an das RichEdit weitergeben
**      - Nacharbeiten fuer Undo-Geschaeft
**-----------------------------------------------------------------
*/
void CTBEditor::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   CString strHelp;
   LPSTR p = NULL;
   // alle non printable characters abfangen
   if( ! ( nChar >= VK_SPACE ||
           nChar == VK_RETURN ||  //Carriage Return
           nChar == VK_TAB ||  //Tabulator
           nChar == VK_BACK ) ) //Backspace
       return;


   if ( nChar != VK_TAB && nChar != VK_RETURN )
   {
      GetSelectionBuffer();
      COleControl::OnChar(nChar, nRepCnt, nFlags);

      ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &m_chNeu );

      FireNewPosition( m_chNeu.cpMin );
   }

   switch ( nChar )
   {
      case VK_BACK:
         if( m_chNeu.cpMin < m_chAlt.cpMin - 1 )
         {
            m_bPosChanged = TRUE;
         }
         TestInput();
         break;
      case VK_TAB:
         if ( IsMultipleLineSelection() > 1 )
         {
            SelectLinesComplete();
            MultipleLineIndention();
         }
         else
         {  // UndoBuffer bauen
            GetSelectionBuffer();
            // invoke Controls default behavior but with a number of blanks instead of tab
            for ( int i = 0; i < m_lTabStop; i++ )
            {
               COleControl::OnChar( VK_SPACE, nRepCnt, nFlags);
               strHelp += (unsigned char) VK_SPACE;
            }

            p = m_pUndo->MakeNewBuffer( strHelp );
            if( m_bSelection )
            {
               m_pUndo->DeleteRange( m_chAlt.cpMin, p );
            }
            else
            {
               if( m_bInsertMode )
               {
                  m_pUndo->InsertRange( m_chAlt.cpMin, p );
               }
               else
               {
                  m_pUndo->InsertRange( m_chAlt.cpMin, p, m_cNext );
               }
            }
         }
         ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &m_chNeu );
         FireNewPosition( m_chNeu.cpMin );
         TestInput();
         break;

      case VK_RETURN:
         {
            if ( !IsReadOnly() )
            {
               DWORD dLineLength;
               DWORD dLineIndex;
               DWORD dCharIndex;
               CHARRANGE cha;

               //aktuelle Char-Position
               SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
               //aktuellen Zeileindex
               dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)cha.cpMin );
               //CharIndex der aktuellen Zeile
               dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex, 0L );
               //Laenge ermitteln
               dLineLength = SendMessage( EM_LINELENGTH, (WPARAM)dCharIndex, 0L );

               GetEditLine( dCharIndex, dCharIndex + dLineLength );
               GetSelectionBuffer();
               strHelp = "\r\n";
               strHelp += GetIndentionString();

               InsertText( strHelp );
            }
         }

         break;

      default:
         if( m_bSelection )
            m_pUndo->DeleteRangeByChar( m_chAlt.cpMin, m_tr.lpstrText, nChar );
         else
         {
            if( m_bInsertMode )
               m_pUndo->InsertCharacter( m_chAlt.cpMin, nChar );
            else
               m_pUndo->InsertCharacter( m_chAlt.cpMin, nChar, m_cNext[0] );
         }
         TestInput();
         break;
   }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnKeyDown
**
** .Beschreibung: Eventhandler fuer WM_KEYDOWN
**
** .Parameter:    UINT, nChar   , I,  die gedrueckte Taste
**                UINT, nRepCnt , I,  RepeatCounter der Taste
**                UINT, nFlags  , I,  Flags ( ALT, CRTL ... )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Hier kommen alle Tastendruecke an, d.h. Methode wird vor OnChar
**      aufgerufen.
**      Arbeitsschriite:
**      - Crtl-Tasten abfangen, sonst macht es das RichEdit
**      - Shift-Insert abfangen, sonst macht es das RichEdit
**      - den Keystroke an den Container weiterreichen mit FireKeystroke
**        ( nur wenn ueber WantKeystrokes oder SYNTAX == ZEIDON gesetzt )
**      - Sonderbehandlungen z.B. fuer BACKSPACE und DELETE beim Undo
**      - den Keystroke an RichEdit weitergeben
**      - VK_DELETE und VK_INSERT gesondert behandeln
**      - die Cursorbewegungen registrieren
**
**-----------------------------------------------------------------
*/

void CTBEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   BOOL bMerkPOSandHOME = FALSE;

   BOOL bFireKeyStroke = TRUE;

   if( m_bCtrl || m_bShift )
      bFireKeyStroke = FALSE;

   if( m_bCtrl ) //ein Paar Tasten sollen nicht ans Richedit gehen !
   {
      switch( nChar )
      {
         case 0x5A:    //Ctrl-Z
            Undo();
            return;
         case 0x59:    //Ctrl-Y
            Redo();
            return;
         case 0x43:    //Ctrl-C
         case 0x2D:    //Ctrl-INS
            CopyText();
            return;
         case 0x56:    //Ctrl-V
            PasteText();
            return;
         case 0x58:    //Ctrl-X
            CutText();
            return;
         case 0x24:
         case 0x23:  //Ctrl-POS1 und -ENDE Tasten
            bMerkPOSandHOME = TRUE;
      }
   }

   if( m_bShift )
   {
      switch( nChar )
      {
         case 0x2D:    //Shift-INS
            CopyText();
            return;
         default:
            break;
      }
   }

   if( m_bWantKeys )
   {
      BOOL bForget = FALSE;
      if ( bFireKeyStroke )
         FireKeystroke( nChar, &bForget );

      if( bForget )
         return;
   }


   ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &m_chAlt );

   switch( nChar )
   {
       case VK_CONTROL:
           m_bCtrl = TRUE;
           break;
       // Alt Gr Taste sendet 2 Key downs
       // einen mit VK_CONTROL und einen mit VK_MENU
       // hiermit wird Alt Gr neutralisiert
       case VK_MENU:
           m_bCtrl = FALSE;
           break;
       case VK_SHIFT:
           m_bShift = TRUE;
           break;
       case VK_F3:
           if( m_bShift )
               SearchBackward();
           else
               SearchForward();
           break;
       case VK_INSERT:
           if( m_bShift )
           { //Shift-Ins == CTRL-V
               PasteText();
               return;
           }
           break;
       case VK_DELETE:
           if( m_bShift )
           { //Shift-Del == CTRL-X
               CutText();
               return;
           }
           else
               GetSelectionBuffer();
               if( m_bSelection )
                  m_pUndo->DeleteRange( m_chAlt.cpMin, m_tr.lpstrText );
               else
                  m_pUndo->DeleteCharacter( m_chAlt.cpMin, m_cNext[0] );
           break;
       case VK_BACK:
           if( m_chAlt.cpMin != m_chAlt.cpMax )
           {
               GetSelectionBuffer();
               m_pUndo->DeleteRange( m_chAlt.cpMin, m_tr.lpstrText );
           }
           else if( m_chAlt.cpMin )
           {
               m_chAlt.cpMin -- ;
               m_tr.chrg.cpMin = m_chAlt.cpMin;
               m_tr.chrg.cpMax = m_chAlt.cpMin + 1;
               m_tr.lpstrText = m_cNext;

               ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &m_tr );

               m_pUndo->DeleteCharacter( m_chAlt.cpMin, m_cNext[0] );
               m_chAlt.cpMin++;
           }
           break;
   }

   COleControl::OnKeyDown(nChar, nRepCnt, nFlags);

   if( nChar == VK_INSERT )
   {
       m_bInsertMode = ! m_bInsertMode;
       if( m_bInsertMode )
           Fire_SetInsertMode();
       else
           Fire_SetOverwriteMode();
   }
   else if( nChar == VK_DOWN || nChar == VK_UP ||
            nChar == VK_PRIOR || nChar == VK_NEXT ||
            nChar == VK_END || nChar == VK_HOME ||
            bMerkPOSandHOME )
   {
       FireSetLine();
       m_bPosChanged = TRUE;
   }
   else if ( nChar == VK_LEFT || nChar == VK_RIGHT )
       FireSetLine( FALSE );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnKeyUp
**
** .Beschreibung: Eventhandler fuer WM_KEYUP
**
** .Parameter:    UINT, nChar   , I,  die gedrueckte Taste
**                UINT, nRepCnt , I,  RepeatCounter der Taste
**                UINT, nFlags  , I,  Flags ( ALT, CRTL ... )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode setzt die Merker fuer Shift und Ctrl-Tasten auf
**      FALSE. Das Gegenstueck befindet sich in WM_KEYDOWN
**-----------------------------------------------------------------
*/
void CTBEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( nChar == VK_SHIFT )
        m_bShift = FALSE;

    if( nChar == VK_CONTROL )
        m_bCtrl = FALSE;

    COleControl::OnKeyUp(nChar, nRepCnt, nFlags);
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnSysKeyDown
**
** .Beschreibung: Eventhandler fuer WM_SYSKEYDOWN
**
** .Parameter:    UINT, nChar   , I,  die gedrueckte Taste
**                UINT, nRepCnt , I,  RepeatCounter der Taste
**                UINT, nFlags  , I,  Flags ( ALT, CRTL ... )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode soll ALT-BACKSPACE abfangen. ( == CTRL-Z )
**      Tastenkombinationen mit ALT laufen nicht ueber WM_KEYDOWN !!!
**-----------------------------------------------------------------
*/
void CTBEditor::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( nChar == VK_BACK )
    {
        Undo();
        return;
    }
    COleControl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnRButtonDown
**
** .Beschreibung: Eventhandler fuer WM_RBUTTONDOWN
**
** .Parameter:    UINT  , nFlags  , I,  Flags ( SHIFT ... )
**                CPOINT, point   , I,  Position des Mausklicks
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode loest das Event FireHelp aus, das angibt auf welchem
**      Wort geklickt wurde.
**-----------------------------------------------------------------
*/

void CTBEditor::OnRButtonDown(UINT nFlags, CPoint point)
{
    POINT p;
    DWORD dCharIndex;
    WPARAM wParam = 0;
    LPARAM lParam = 0;

    p.x = point.x;
    p.y = point.y;

    //die Doku stimmt ueberhaupt nicht !!!
    // EM_CHARFROMPOS erhaelt einen Zeiger auf POINT nicht auf POINTS
    // liefert einen DWORD zero-based Charindex zurueck
    dCharIndex = SendMessage(EM_CHARFROMPOS, 0, (LPARAM) &p);

    FireHelp( dCharIndex );
    lParam = MAKELONG(point.x, point.y);
    wParam = (WPARAM) m_hWnd;
//hwnd = (HWND) wParam; xPos = LOWORD(lParam); yPos = HIWORD(lParam);
    COleControl::OnRButtonDown(nFlags, point);
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnLButtonDown
**
** .Beschreibung: Eventhandler fuer WM_LBUTTONDOWN
**
** .Parameter:    UINT  , nFlags  , I,  Flags ( SHIFT ... )
**                CPOINT, point   , I,  Position des Mausklicks
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode loest das Event FireSetline aus, das angibt auf welche
**      Zeile geklickt wurde.
**-----------------------------------------------------------------
*/
void CTBEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
    COleControl::OnLButtonDown(nFlags, point);

    FireSetLine();
    m_bPosChanged = TRUE;
}

void CTBEditor::OnLButtonUp(UINT nFlags, CPoint point)
{
    COleControl::OnLButtonUp(nFlags, point);
}

void CTBEditor::OnMouseMove(UINT nFlags, CPoint point)
{
   /* this doesn't help bomb on drag ... dks 2007.03.02
    if ( nFlags & MK_LBUTTON )
    {
        MSG  msg;

        if ( ::PeekMessage( &msg, m_hWnd, WM_LBUTTONUP, WM_LBUTTONUP,
                           PM_NOREMOVE | PM_NOYIELD ) )
        {
           return;
        }
    }
   */
    COleControl::OnMouseMove(nFlags, point);
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnLButtonDblClk
**
** .Beschreibung: Eventhandler fuer WM_LBUTTONDBLCLK
**
** .Parameter:    UINT  , nFlags  , I,  Flags ( SHIFT ... )
**                CPOINT, point   , I,  Position des Mausklicks
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode verhindert das Weitergeben des Doppelklicks an das
**      RichEdit.
**      Fuer UNDO notwendig, da Version 2.0 des RichEdits sich anders
**      verhaelt als Version 1.0. Ein Blank hinter dem Wort wird zwar
**      markiert, aber nicht bearbeitet.
**      Der UNDO verlaesst sich aber auf die Korrektheit der Markierung.
**-----------------------------------------------------------------
*/
void CTBEditor::OnLButtonDblClk(UINT nFlags, CPoint point)
{
   POINT p;
   DWORD dAbsoluteIndex;
   WPARAM wParam = 0;
   LPARAM lParam = 0;
   CString strHelp;
   strHelp.Empty();

   DWORD dLineLength;
   DWORD dLineIndex;
   DWORD dBeginOfLineIndex;


   TEXTRANGE tr;
   tr.chrg.cpMin = 0;
   tr.chrg.cpMax = 0;
   tr.lpstrText = NULL;

   p.x = point.x;
   p.y = point.y;

   //die Doku stimmt ueberhaupt nicht !!!
   // EM_CHARFROMPOS erhaelt einen Zeiger auf POINT nicht auf POINTS
   // liefert einen DWORD zero-based Charindex zurueck
   dAbsoluteIndex = SendMessage(EM_CHARFROMPOS, 0, (LPARAM) &p);
   if( dAbsoluteIndex < 0)
       return;

   dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)dAbsoluteIndex );
   if( dLineIndex  < 0)
       return;
   // Char Index vom beginn der Zeile dLineIndex
   dBeginOfLineIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex , 0L );
   if( dBeginOfLineIndex < 0)
       return;
   //Laenge ermitteln
   dLineLength = SendMessage( EM_LINELENGTH, (WPARAM)dBeginOfLineIndex, 0L );

   tr.chrg.cpMin = dBeginOfLineIndex;
   tr.chrg.cpMax = dBeginOfLineIndex + dLineLength;
   tr.lpstrText  = strHelp.GetBufferSetLength( dLineLength );
   if ( SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr ) > 0 )
   {
      // position to the character near click position
      // this is the index of the character relative to the BOF
      // minus index of the character the represents the begin of line
      DWORD dClickPosChar = dAbsoluteIndex - dBeginOfLineIndex;

      if ( dClickPosChar < 0 )
         return;
      // looking for the first charater left from current position thats a delimiter
      LPSTR lpszText = tr.lpstrText + dClickPosChar;
      DWORD dPosLeft = dClickPosChar;
      DWORD dPosRight = dClickPosChar;
      while ( lpszText > tr.lpstrText
         && !IsDelimiter( *lpszText ) )
      {
         dPosLeft--;
         lpszText--;
      }
      if( IsDelimiter( *lpszText ) )
         dPosLeft++;
      // looking for the first charater rigth from current position thats a delimiter
      lpszText = tr.lpstrText + dClickPosChar;
      while ( lpszText < ( tr.lpstrText + dLineLength )
         && !IsDelimiter( *lpszText ) )
      {
         dPosRight++;
         lpszText++;
      }

      tr.chrg.cpMin = dBeginOfLineIndex + dPosLeft;
      tr.chrg.cpMax = dBeginOfLineIndex + dPosRight;
      SendMessage( EM_EXSETSEL, 0,(LPARAM) &tr.chrg );
   }

/*
#ifndef NEW_RICHEDIT
   COleControl::OnLButtonDblClk(nFlags, point);
#endif
   */
   return;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnSetFocus
**
** .Beschreibung: Eventhandler fuer WM_SETFOCUS
**
** .Parameter:    CWnd* , pOldwnd , I,  das Window, das den Focus hatte
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode loest die Events fuer den Container aus.
**-----------------------------------------------------------------
*/
void CTBEditor::OnSetFocus(CWnd* pOldWnd)
{
    COleControl::OnSetFocus(pOldWnd);

    /* Bugbehebung: Die Tracker wurden u.U. ( Oeffnen eines Dialogs,
       Aktivieren eines anderen MDI-Childs ... ) sichtbar. Sie fuehren
       dann zu haesslichen Schmierzeichen. Deshalb zerstoeren wir die
       Tracker sofort.
       Der mehrmalige Aufruf von DestroyTracker() stoert nicht, da die
       Methode intern zuerst prueft, ob es etwas zu zerstoeren gibt.
    */

    if( COleControl::m_bUIActive )  //vermeidet ASSERT in MFC
        DestroyTracker();

    SetFocus();
    if( m_bInsertMode )
        Fire_SetInsertMode();
    else
        Fire_SetOverwriteMode();
    FireSetLine( FALSE );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnDestroy
**
** .Beschreibung: Eventhandler fuer WM_DESTROY
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird aufgerufen, wenn das RichEdit - Window zerstoert
**      wird. Zur Sicherheit wird nochmals ueberprueft, ob alle Ressourcen
**      freigegeben sind.
**-----------------------------------------------------------------
*/
void CTBEditor::OnDestroy()
{
   CloseSubWindow();
   COleControl::OnDestroy();

    if( m_sFindDlg )
        delete [] m_sFindDlg;

    if( m_pSyntax )
    {
        delete m_pSyntax;
        m_pSyntax = NULL;
    }

    if( m_hFont )
    {
        DeleteObject( m_hFont );
        m_hFont = NULL;
    }

    if( m_pUndo )
    {
        delete m_pUndo;
        m_pUndo = NULL;
    }

    if( m_pBigBuf )
    {
        delete m_pBigBuf;
        m_pBigBuf = NULL;
    }

    if( m_pSearchString )
    {
        delete [] m_pSearchString;
        m_pSearchString = NULL;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      PreTranslateMessage
**
** .Beschreibung: Bearbeitung von Messages
**
** .Parameter:    MSG*  , pMsg  , I,  Pointer aus Messagestruktur
**
** .Rueckgabewert:  BOOL : TRUE -> Message wurde abgearbeitet
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird aufgerufen bevor die jeweilige Message an ihr
**      Fenster weitergeleitet wird. Dadurch ist es moeglich, bestimmte
**      Messages abzufangen.
**      Dies geschieht fuer Tastatur-Messages, wenn der nichtmodale
**      Suchdialog laeuft, da sonst TAB und ESC nicht an den Dialog
**      geleitet werden.
**      Das ESC wird prinzipiell abgefangen, da MFC-Container in ihrer
**      Standardimplementation daraufhin den INpLace-Server deleten.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::PreTranslateMessage(MSG* pMsg)
{
   if( pMsg->message >= WM_KEYFIRST && pMsg->message <=  WM_KEYLAST )
   {
      if ( GetFocus() != this )
      {
         if( m_hWndModal ) //wenn nicht-modale Dialoge laufen
         {
            BOOL bProcessed;

            bProcessed = ::IsDialogMessage( m_hWndModal, pMsg ); //weiterleiten der Tasten
            if( bProcessed )
                return TRUE;
            else
                return COleControl::PreTranslateMessage(pMsg);
         }
      }
      else if( VK_ESCAPE == pMsg->wParam ) //ESCAPE abfangen
      { //das ESC soll eine bestehende Textmarkierung aufheben
         Deselect();
         return TRUE;
      }
   }

   return COleControl::PreTranslateMessage(pMsg);
}

LRESULT CTBEditor::OnFind(WPARAM wParam, LPARAM lParam)
{
   ProcessFindReplace( (FINDREPLACE *) lParam );
   return( 1 );
}
/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      WindowProc
**
** .Beschreibung: Behandlung von Window-Messages
**
** .Parameter:    UINT  , msg     , I,  WM_...-Nachricht
**                WPARAM, wParam  , I,  WPARAM
**                LPARAM, lParam  , I,  LPARAM
**
** .Rueckgabewert:  LRESULT  1 -> Nachricht wurde abgearbeitet
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode haengt in der MessageLoop des MFC-Frameworks. Sie
**      wird vor dem MFC-MessageHandler aufgerufen und erlaubt das
**      Abfangen von Nachrichten, bevor das Framework sie bearbeitet.
**
**      Sie wird genutzt, um Die registrierte Nachricht des Suchdialogs
**      ( in OnCreate registriert ) abzufangen.
**-----------------------------------------------------------------
*/
LRESULT CTBEditor::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
   if( msg == WM_PAINT )
   {
      ::CallWindowProc( *GetSuperWndProcAddr(), m_hWnd, WM_PAINT,
                       wParam, lParam );
      return TRUE;
   }

   return COleControl::WindowProc(msg, wParam, lParam);
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnOcmCommand
**
** .Beschreibung: Behandlung von Command-Messages
**
** .Parameter:    WPARAM, wParam, I, Notification
**                LPARAM, lParam, I, Zusatzinfos in Abhaengig. von wParam
**
** .Rueckgabewert:  LRESULT -> immer 0
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Es werden Notifications des RichEdits abgefangen:
**      - EN_ERRSPACE wird gesendet, wenn kein Speicher mehr allokierbar
**        ist -> absoluter Systemsystemfehler
**      - EN_MAXTEXT wird gesendet, wenn die urspruenglich erlaubte max.
**        Textgroesse ueberschritten wird ( in OnCreate auf 200000 Byte
**        festgelegt ). In diesem Fall wird die Groesse einfach ver-
**        doppelt.
**-----------------------------------------------------------------
*/
LRESULT CTBEditor::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN32
   WORD wNotifyCode = HIWORD(wParam);
#else
   WORD wNotifyCode = HIWORD(lParam);
#endif

   switch ( wNotifyCode )
   {
   case EN_ERRSPACE:  // kein zusaetzliches Memory verfuegbar
      DialogMeldung( IDS_MEMORY, MB_ICONEXCLAMATION | MB_OK );
      break;

   case EN_MAXTEXT:        //RichEdit hat keinen Platz mehr
      {
         DWORD d = SendMessage( EM_GETLIMITTEXT, 0, 0 );
         SendMessage( EM_EXLIMITTEXT, 0, (LPARAM) 2*d );
         break;
      }
   default:
      break;
   }
   return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnCreate
**
** .Beschreibung: Initialisierung des TBEditor-Controls
**
** .Parameter:    LPCREATESTRUCT, lpCreateStruct, I, Infos des Frameworks
**
** .Rueckgabewert:  int ->  0 = alles o.k
**                          -1 = Fehler
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird vom Framework aufgerufen, wenn das OleControl
**      erzeugt wird.
**      Hier finden sich RichEdit-spezifische Initialisierungen:
**      - WindowMessage des CommonDialog Suchdialogs registrieren
**      - max. Textlaenge, Font, Fontgroesse, Hintergrund- und
**        Vordergrundfarbe des Richedits
**      - SyntaxColoring einschalten
**      - Drag 'n Drop einschalten
**-----------------------------------------------------------------
*/
int CTBEditor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (COleControl::OnCreate(lpCreateStruct) == -1)
        return( -1 );

    if( m_hWnd )
    {
        //Message fuer Find/Replace-Dialog registrieren lassen
//        m_msgFind = RegisterWindowMessage( FINDMSGSTRING );

        //Initialisierungen
        SendMessage( EM_SETUNDOLIMIT, 0, 0L ); //kein eigenes Undo
        SendMessage( EM_EXLIMITTEXT, 0, 500000 );  //Textlimit
        SendMessage( EM_SETBKGNDCOLOR, FALSE, m_crBackground );
//        SendMessage( EM_SETWORDBREAKPROC ,  (WPARAM) 0,  (LPARAM)WordBreakCallback );

//        SendMessage( EM_SETOPTIONS, (WPARAM)ECOOP_OR, ECO_NOHIDESEL );
//        SendMessage( EM_SETOPTIONS, (WPARAM)ECOOP_XOR, ECO_AUTOWORDSELECTION );
//        SendMessage( EM_SETOPTIONS, (WPARAM)ECOOP_XOR, ECO_SAVESEL );

        //Syntax einschalten
        if( m_enSyntax == ZEIDON_SYNTAX )
            m_pSyntax = new ZeidonSyntax( this, m_pLineBuf, m_hWnd );
/* siehe Änderungsvermerk 01L
        else if( m_enSyntax == SIRON_SYNTAX )
            pSyntax = new SironSyntax( this, m_pLineBuf, m_hWnd, m_bGerman );
*/
        else
            m_pSyntax = NULL;

        //UndoRedo initialisieren
        m_pUndo = new CUndo( m_hWnd, m_lUndoLevel );

        //CharFormat einstellen
        m_cf.crTextColor = m_crForeground;
        CreateMyFont( m_FontName, m_FontSize );
        SendMessage( WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM( TRUE, 0 ) );

        //Tabs einstellen
        SetTabStops( m_lTabStop );

        //Enable Drag and Drop  // does not help drag/drop bomb when enabled 2008.09.24 dks
        //::DragAcceptFiles( m_hWnd, TRUE );
        //SendMessage( EM_SETEVENTMASK, 0, (LPARAM)ENM_DROPFILES);

        SendMessage( EM_SETMODIFY, FALSE, 0L );

        Fire_SetInsertMode();
    }

    return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnEraseBkgnd
**
** .Beschreibung: Ausloesen des Repaints
**
** .Parameter:    CDC*, pDC, I, DeviceContext Klasse
**
** .Rueckgabewert:  BOOL, TRUE-> wurde abgearbeitet
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Das Ueberladen der Methode ist notwendig, da die MFC das Repainten
**      des RichEdits nicht korrekt initialisiert. Der uebergebene
**      DC beinhaltet die SelectionBar des RichEdits nicht!!
**      So wird ein eigener DC aufgebaut und direkt an die Window-
**      Prozedur des RichEdits uebergeben.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::OnEraseBkgnd(CDC* pDC)
{
    BOOL bErg;
    HDC hDC = ::GetDC( m_hWnd );
    bErg = ::CallWindowProc( *GetSuperWndProcAddr(), m_hWnd,
                             WM_ERASEBKGND, (WPARAM)hDC, 0L );
    ::ReleaseDC( m_hWnd, hDC );

    return bErg;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OnGetDlgCode
**
** .Beschreibung: Beschreibung des Verhaltens in Dialogen
**
** .Parameter:
**
** .Rueckgabewert:  UINT, gewuenschter Input
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      In Dialogen werden gewisse Tasten ( Return, Tab, ESC ) speziell
**      behandelt. Um weiterhin diese Tasten im Editor zu verwenden,
**      muss das Control dem Dialog mitteilen welchen Input es
**      verarbeiten will. Dies geschieht ueber den Returnwert der
**      Message WM_GETDLGCODE.
**-----------------------------------------------------------------
*/
UINT CTBEditor::OnGetDlgCode()
{
   return DLGC_WANTALLKEYS | DLGC_HASSETSEL | DLGC_WANTMESSAGE |
           DLGC_WANTCHARS |  DLGC_WANTARROWS | DLGC_WANTTAB;
}




/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SaveOptions
**
** .Beschreibung: Editoreinstellungen speichern
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Speichert die persistenten Einstellungen des Controls in der
**      Registry ab.
**-----------------------------------------------------------------
*/
void CTBEditor::SaveOptions()
{
    TBRegistry * pReg = new TBRegistry( REGISTRYKEY, (unsigned long)HKEY_CURRENT_USER );
    if( pReg->OpenKey( REG_WRITE ) )
    {
        DWORD dTyp;
        DWORD dSize;

        dTyp = REG_SZ;      //Fontname
        dSize = lstrlen( m_FontName ) + 1;
        pReg->WriteValue( REGFONT, dTyp, (BYTE*)m_FontName, dSize );

        dTyp = REG_DWORD;   //Schriftgroesse
        dSize = sizeof ( m_FontSize );
        pReg->WriteValue( REGSIZE, dTyp, (BYTE*)&m_FontSize, dSize );

        dTyp = REG_DWORD;   //Hintergrundfarbe
        dSize = sizeof ( m_crBackground );
        pReg->WriteValue( REGBACK, dTyp, (BYTE*)&m_crBackground, dSize );

        dTyp = REG_DWORD;   //Vordergrundfarbe
        dSize = sizeof ( m_crForeground );
        pReg->WriteValue( REGFORE, dTyp, (BYTE*)&m_crForeground, dSize );

        dTyp = REG_DWORD;   //Keywordfarbe
        dSize = sizeof ( m_crKeyword );
        pReg->WriteValue( REGWORD, dTyp, (BYTE*)&m_crKeyword, dSize );

        dTyp = REG_DWORD;   //K0mmentarfarbe
        dSize = sizeof ( m_crComment );
        pReg->WriteValue( REGCOMMENT, dTyp, (BYTE*)&m_crComment, dSize );

        dTyp = REG_DWORD;   //Tabulator-Positionen
        dSize = sizeof ( m_lTabStop );
        pReg->WriteValue( REGTABSTOP, dTyp, (BYTE*)&m_lTabStop, dSize );
    }

    delete pReg;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      IsMultipleLineSelection
**
** .Beschreibung: Stellt fest ob der markierte Text mehrere Zeilen
**                umfasst
**
** .Parameter:
**
** .Rueckgabewert: Anzahl der markierten Zeilen
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Stellt fest ob der markierte Text mehrere Zeilen umfasst
**-----------------------------------------------------------------
*/
long CTBEditor::IsMultipleLineSelection()
{
   DWORD dFirstLine;
   DWORD dLastLine;
   CHARRANGE cha;

   if( ! m_hWnd )
      return( -1 );

   //aktuelle Char-Position
   SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
   //aktuellen Zeile
   dFirstLine = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)cha.cpMin );
   dLastLine  = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)cha.cpMax );

   return ( long )( dLastLine - dFirstLine + 1 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SelectLinesComplete
**
** .Beschreibung: markiert Zeilen vom ersten bis zum letzten Zeichen
**
** .Parameter:
**
** .Rueckgabewert: Anzahl der Zeichen
**
** ---------------------------------------------------------------------
*/
long CTBEditor::SelectLinesComplete()
{
   DWORD dFirstLine;
   DWORD dLastLine;
   LONG dFirstChar;
   LONG dLastChar;

   if( ! m_hWnd )
      return( -1 );

   TEXTRANGE tr;
   //aktuelle Char-Position
   SendMessage( EM_EXGETSEL, 0,(LPARAM) &tr.chrg );
   //aktuellen Zeile
   dFirstLine = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)tr.chrg.cpMin );
   dLastLine  = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)tr.chrg.cpMax );

   // absolute position des 1. characters der ersten und der letzten Zeile holen
   dFirstChar = SendMessage( EM_LINEINDEX, dFirstLine, 0);
   dLastChar  = SendMessage( EM_LINEINDEX, dLastLine, 0);

   // Zeilenlänge zu dLastchrgr addieren um alle Zeichen zu selektieren
   dLastChar  += SendMessage( EM_LINELENGTH, dLastChar, 0);

   // erweiterte range selektieren
   tr.chrg.cpMin = dFirstChar;
   tr.chrg.cpMax = dLastChar;
   SendMessage( EM_EXSETSEL, 0,(LPARAM) &tr.chrg );

   return ( long )( dLastChar - dFirstChar + 1 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      MultipleLineIndention
**
** .Beschreibung: rückt mehrere Zeile ein
**
** .Parameter:
**
** .Rueckgabewert: Anzahl der Zeichen
**
** ---------------------------------------------------------------------
*/
long CTBEditor::MultipleLineIndention()   // mehrere Zeilen einrücken
{
   TEXTRANGE tr;
   CString strBuffer;
   CString strNewBuffer;
   CString strTab;
   BOOL bInsertBlanks = TRUE;
   int iLFPosition = 0;
   int iOffset = 0;
   int iLength = 0;
   long lLineCount = 0;

   if( ! m_hWnd )
      return( -1 );

   if ( bInsertBlanks )
   {
      CString strHelp(_T(' '), m_lTabStop);
      strTab = strHelp;
   }
   else
   {
      strTab = _T('\t');
   }

   //aktuelle Char-Position
   SendMessage( EM_EXGETSEL, 0,(LPARAM) &tr.chrg );
   // Buffer holen
   tr.lpstrText = strBuffer.GetBufferSetLength( tr.chrg.cpMax - tr.chrg.cpMin );
   SendMessage( EM_GETTEXTRANGE, 0,(LPARAM) &tr );
   strBuffer.ReleaseBuffer();

   strNewBuffer.Empty();
   // Shift Tab
   if ( m_bShift )
   {
      LPCSTR pSource = strBuffer;
      CString strHelp;
      LPSTR pHelp;
      while( *pSource )
      {
         pHelp = strHelp.GetBufferSetLength( strlen( pSource ) + 1 );
         // 1. Neuen Indention String aufbauen
         CString strNewIndent;
         LPSTR pNewIndent = strNewIndent.GetBufferSetLength( strBuffer.GetLength() );
         LPSTR pI = pNewIndent;
         // such char der kein space oder tab is
         // und baue indention string.
         BOOL bHasTabs = FALSE;
         while (*pSource && ( *pSource == '\t' || *pSource == ' ' ))
         {
            if (*pSource == '\t' )
            {
               bHasTabs = TRUE;
            }
            *pNewIndent++ = *pSource++;
         }
         *pNewIndent = '\0';
         pNewIndent = pI;
         // Wenn Tabs vorhanden ein Tab raus
         if ( bHasTabs )
         {
            while ( *pNewIndent && *pNewIndent != '\t' )
            {
               pNewIndent++;
            }
            if ( *pNewIndent == '\t' )
            {
               strcpy (pNewIndent, ( pNewIndent + 1 ));
            }
         }
         else // wenn nur spaces dann Entsprechende Anzahl rausnehmen.
         {
            pNewIndent += strlen (pNewIndent);
            int ic = 0;
            while ( pNewIndent >= pI && ic <= m_lTabStop )
            {
               *pNewIndent-- = '\0';
               ic++;
            }
         }
         // Zurückpositionieren und zuweisen
         pNewIndent = pI;
         strNewBuffer += pNewIndent;

         // 2. kopieren bis zum Line feed
         while (*pSource && *pSource != '\n')
         {
            *pHelp++ = *pSource++;
         }
         if ( *pSource == '\n' )
         {
            *pHelp++ = '\n';
         }
         *pHelp = '\0';
         strHelp.ReleaseBuffer();
         strNewBuffer += strHelp;
         if (*pSource)
         {
            pSource++;
         }
      }
   }
   else
   {
      lLineCount = IsMultipleLineSelection();
      long lBuffLength = lLineCount * strTab.GetLength() + strBuffer.GetLength();
      LPCSTR pSource = strBuffer;
      LPSTR pTarget = strNewBuffer.GetBufferSetLength( lBuffLength  + 1 );
      strcpy(pTarget, strTab);
      pTarget += strTab.GetLength();
      for(; *pSource; pSource++)
      {
         *pTarget = *pSource;
         pTarget++;
         if ( *pSource == '\n' ) // nach jedem Linefeed ein tab einfügen
         {
            *pTarget = '\0';
            strcat( pTarget, strTab);
            pTarget += strTab.GetLength();
         }
      }
      *pTarget = '\0';
   }
   strNewBuffer.ReleaseBuffer();

   // Markierten Text ersetzen
   SendMessage( EM_REPLACESEL , (WPARAM)FALSE, (LPARAM)(LPCTSTR) strNewBuffer );
   // Text neu markieren
   tr.chrg.cpMax = tr.chrg.cpMin + strNewBuffer.GetLength();
   SendMessage( EM_EXSETSEL, 0,(LPARAM) &tr.chrg );

   char *pDelRange = m_pUndo->MakeNewBuffer( strBuffer );
   char *pInsRange = m_pUndo->MakeNewBuffer( strNewBuffer );

   m_pUndo->InsertRange( tr.chrg.cpMin, pInsRange, pDelRange );

   // do the sysntax coloring line by line
   long lFirstLine = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)tr.chrg.cpMin );
   long lLastLine = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)tr.chrg.cpMax );
   while ( lFirstLine <= lLastLine && m_pSyntax )
   {
      // absolute position des 1. characters der ersten und der letzten Zeile holen
      long lFirstChar = SendMessage( EM_LINEINDEX, lFirstLine, 0);
      long lLength    = SendMessage( EM_LINELENGTH,lFirstLine, 0);
      this->GetEditLine( lFirstChar, lFirstChar + lLength );
      m_pSyntax->ParseLine( m_pLineBuf );
      lFirstLine++;
   }
   SendMessage( EM_EXSETSEL, 0,(LPARAM) &tr.chrg );

   return strNewBuffer.GetLength();
}

CString CTBEditor::GetStringFromLineOfIndex( long lIndex )
{
   CString strHelp;
   DWORD dLineLength;
   DWORD dLineIndex;
   DWORD dCharIndex;
   TEXTRANGE tr;

   if( ! m_hWnd || lIndex < 0)
       return strHelp;

   dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)lIndex);
   //CharIndex der Zeile dLineIndex
   dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex , 0L );
   if( dCharIndex < 0)
      return strHelp;

   //Laenge ermitteln
   dLineLength = SendMessage( EM_LINELENGTH, (WPARAM)dCharIndex, 0L );

   if ( dLineLength > 0 )
   {
      tr.chrg.cpMin = dCharIndex;
      tr.chrg.cpMax = dCharIndex + dLineLength;
      tr.lpstrText = strHelp.GetBufferSetLength( dLineLength );
      SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr );
      strHelp.ReleaseBuffer();
   }

   return strHelp;
}
