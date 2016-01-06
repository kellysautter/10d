#if !defined(AFX_HZOECTL_H__DEA843E2_DAA2_11D1_95B2_0060973B3B55__INCLUDED_)
#define AFX_HZOECTL_H__DEA843E2_DAA2_11D1_95B2_0060973B3B55__INCLUDED_


/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditor
**
** .Version:       01M
**
** .Aenderung:     02.11.2000
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
** 01A  =  Neuerstellung
**         (18.05.1998)  SL
** 01B  =  Erweiterung fuer C-Syntax Kommentare und UNDO
**         (23.07.1998)  SL
** 01C  =  Methode DeleteText gibt nun long zurueck
**         (18.12.1998)  SL, TMV
** 01D  =  Neue private Methode DoNotHideThePoint()
**         (01.03.1999)SL, diesen Eintrag ergaenzt RT
** 01E  =  Umstellung auf Version ohne eigenes Menue. Dadurch entfallen
**         alle Methoden die fuer Menubedienung notwendig waren.
**         Dafür wurden einige neue Interface-Methoden bereitgestellt.
**         (22.03.1999)  SL
** 01F  =  Einbindung des Controls in CFormView-Klassen ermoeglicht
**         durch Abfangen von WM_GETDLGCODE
**         (24.06.1999)  SL
** 01G  =  Multiple Line Indention
**         (09.03.1999)  TMV
** 01H  =  Siron spezifische Funktionen auskommentiert
**         (1999.11.04)  TMV
** 01I  =  Neue Funktion eingefügt
**         SelectRange
**         (2000.03.15)  TMV
** 01J  =  Cosmetic (so Brief can read it)
**         (2000.03.16)  DKS
** 01K  =  Replace default EditWordBreakProc routine. Therefore 2 methods are added
**         to CTBEditor:
**             WordBreakCallback  (see bug report 050599, 050699)
**             IsDelimiter
**         define m_lTabStop as static and replace tabstops by
**         m_lTabStop number of blanks during read
**         see ReadCallback and ReadRTFFormat (see bug report 050780, 050668)
**         (26.10.2000)  TMV
** 01L  =  replace TABS in call of InsertItem. Therefore implement
**         new method InsertText
**         (2000.10.31)  TMV
** 01M  =  remove obsolete variable m_bReturn
**         (2000.11.02)  TMV
**----------------------------------------------------------------------
*/



#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "hzoesyn.h"
#include "hzoeundo.h"
#include "hzoestri.h"


/*----------------------------------------------------------------------
** .typdefs
**----------------------------------------------------------------------
*/
#define LINEBUF_SIZE    64000

UINT APIENTRY FindReplaceHook( HWND hdlg, // handle to the dialog box window
                             UINT uiMsg, // message identifier
                             WPARAM wParam,// message parameter
                             LPARAM lParam ); // message parameter




class CTBEditor : public COleControl
{
   DECLARE_DYNCREATE(CTBEditor)

public:
   CTBEditor();

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTBEditor)
   public:
   virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
   virtual void DoPropExchange(CPropExchange* pPX);
   virtual void OnResetState();
   virtual DWORD GetControlFlags();
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   protected:
   virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
   //}}AFX_VIRTUAL



// Implementation
protected:
   ~CTBEditor();

   DECLARE_OLECREATE_EX(CTBEditor)    // Class factory and guid
   DECLARE_OLETYPELIB(CTBEditor)      // GetTypeInfo
   DECLARE_OLECTLTYPE(CTBEditor)       // Type name and misc status

   // Subclassed control support
   BOOL PreCreateWindow(CREATESTRUCT& cs);
   BOOL IsSubclassedControl();
   LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// Message maps
   //{{AFX_MSG(CTBEditor)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   afx_msg void OnDestroy();
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg UINT OnGetDlgCode();
   afx_msg LRESULT OnFind(WPARAM wParam, LPARAM lParam);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

// Dispatch maps
   //{{AFX_DISPATCH(CTBEditor)
   afx_msg BOOL OpenObject(BSTR FAR* FileName);
   afx_msg BOOL CloseObject();
   afx_msg long DeleteText();
   afx_msg long CutText();
   afx_msg long CopyText();
   afx_msg long PasteText();
   afx_msg void SyntaxOff();
// afx_msg void SironSyntaxOn();
   afx_msg void ZeidonSyntaxOn();
   afx_msg BOOL PropertyDialog();
   afx_msg void GetCursorPosition(long FAR* lLinie, long FAR* lSpalte, long FAR* lIndex);
   afx_msg void GetLineCount(long FAR* lZeilen);
   afx_msg long InsertItem(BSTR FAR* pDaten);
   afx_msg void IsObjectChanged(BOOL FAR* bChanged);
   afx_msg void PrintObject();
   afx_msg void ReadOnlyModus(BOOL bReadOnly);
   afx_msg BOOL SaveObject();
   afx_msg BOOL SaveObjectAs(BSTR FAR* pFileName);
   afx_msg BOOL SelectItem(long lZeile, BSTR FAR* pDaten);
   afx_msg void SetCursorPositionByIndex(long lIndex);
   afx_msg void SetCursorPositionByLine(long lZeile, long lSpalte);
   afx_msg BOOL Undo();
   afx_msg BOOL Redo();
   afx_msg BOOL TempSaveObject(BSTR FAR* pFileName);
   afx_msg long FindTextPosition(BSTR FAR* pSearchString, long FAR* lTextPosition, long lOptions);
   afx_msg long GetActualTextLine(BSTR FAR* pBuffer, long lLength);
   afx_msg BOOL IsReadOnly();
   afx_msg BOOL IsSubWindowActive();
   afx_msg void WantKeystrokes(BOOL bWantKeys);
   afx_msg BOOL DeleteTextRange(long lStartIndex, long lEndIndex);
   afx_msg BOOL CloseSubWindow();
   afx_msg void SearchForward();
   afx_msg void SearchBackward();
   afx_msg void Deselect();
   afx_msg long GetTextFromLineOfIndex(BSTR FAR* pBuffer, long lLength, long lIndex);
// afx_msg long SetSironLanguage(long lLanguage);
   afx_msg void GetPositionByIndex(long lIndex, long FAR* lLine, long FAR* lColumn);
   afx_msg BOOL IsCommentAtIndex( long lIndex);
   afx_msg void SearchWindow();
   afx_msg void ReplaceWindow();
   afx_msg void SelectAll();
   afx_msg BOOL GotoWindow();
   afx_msg BOOL CanCopy();
   afx_msg BOOL CanPaste();
   afx_msg BOOL CanUndo();
   afx_msg BOOL CanRedo();
   afx_msg BOOL SelectRange(long lIndex, long lLength);
   afx_msg long GetTextFromRange ( BSTR FAR* pBuffer, long lLength, long lIndex );
   afx_msg long GetObjectName ( BSTR FAR* pBuffer );
   afx_msg long GetSelectedText( BSTR FAR* pBuffer, long lLength );
   //}}AFX_DISPATCH
   DECLARE_DISPATCH_MAP()

// Event maps
   //{{AFX_EVENT(CTBEditor)
   void Fire_SetInsertMode()
       {FireEvent(eventidSetInsertMode,EVENT_PARAM(VTS_NONE));}
   void Fire_RightMouseClick(BSTR FAR* pName)
       {FireEvent(eventidRightMouseClick,EVENT_PARAM(VTS_PBSTR), pName);}
   void Fire_SetLine(long lZeile, long lSpalte)
       {FireEvent(eventidSetLine,EVENT_PARAM(VTS_I4  VTS_I4), lZeile, lSpalte);}
   void Fire_SetOverwriteMode()
       {FireEvent(eventidSetOverwriteMode,EVENT_PARAM(VTS_NONE));}
   void FireKeystroke(short sKey, BOOL FAR* bForgetIt)
       {FireEvent(eventidKeystroke,EVENT_PARAM(VTS_I2  VTS_PBOOL), sKey, bForgetIt);}
   //}}AFX_EVENT
   DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
   enum {
   //{{AFX_DISP_ID(CTBEditor)
   dispidOpenObject = 1L,
   dispidCloseObject = 2L,
   dispidDeleteText = 3L,
   dispidCutText = 4L,
   dispidCopyText = 5L,
   dispidPasteText = 6L,
   dispidSyntaxOff = 7L,
// dispidSironSyntaxOn = 8L,
   dispidZeidonSyntaxOn = 9L,
   dispidPropertyDialog = 10L,
   dispidGetCursorPosition = 11L,
   dispidGetLineCount = 12L,
   dispidInsertItem = 13L,
   dispidIsObjectChanged = 14L,
   dispidPrintObject = 15L,
   dispidReadOnlyModus = 16L,
   dispidSaveObject = 17L,
   dispidSaveObjectAs = 18L,
   dispidSelectItem = 19L,
   dispidSetCursorPositionByIndex = 20L,
   dispidSetCursorPositionByLine = 21L,
   dispidUndo = 22L,
   dispidRedo = 23L,
   dispidTempSaveObject = 24L,
   dispidFindTextPosition = 25L,
   dispidGetActualTextLine = 26L,
   dispidIsReadOnly = 27L,
   dispidIsSubWindowActive = 28L,
   dispidWantKeystrokes = 29L,
   dispidDeleteTextRange = 30L,
   dispidCloseSubWindow = 31L,
   dispidSearchForward = 32L,
   dispidSearchBackward = 33L,
   dispidDeselect = 34L,
   dispidGetTextFromLineOfIndex = 35L,
// dispidSetSironLanguage = 36L,
   dispidGetPositionByIndex = 37,
   dispidIsCommentAtIndex = 38,
   dispidSearchWindow = 39,
   dispidReplaceWindow = 40,
   dispidSelectAll = 41,
   dispidGotoWindow = 42,
   dispidCanCopy = 43,
   dispidCanPaste = 44,
   dispidCanUndo = 45,
   dispidCanRedo = 46,
   dispidSelectRange = 47,
   dispidGetTextFromRange = 48,
   dispidGetObjectName = 49,
   eventidSetInsertMode = 1L,
   eventidRightMouseClick = 2L,
   eventidSetLine = 3L,
   eventidSetOverwriteMode = 4L,
   eventidKeystroke = 5L,
   //}}AFX_DISP_ID
   };


// FriendKlassen fer die Syntaxpruefung
// friend class SironSyntax;
friend class ZeidonSyntax;


private:
   void DoNotHideThePoint( HWND hWndWindow, long lIndex );
   BOOL ChangeColors( COLORREF *pCR );
   void InitializeFileName();
   void CreateMyFont( const char *pFont, long nHeight );
   void SetTabStops( long lAnz );
   void SetKeywords();
   void FireSetLine( BOOL bChange = TRUE);
   void FireNewPosition( long lPos );
   DWORD ScrollToLine( long lLine );
   void TestInput();
   void GetEditLine( DWORD dAnfangIndex, DWORD dEndeIndex );
   CString GetIndentionString( void );
   void FireHelp( DWORD dCharIndex );
   void ProcessFindReplace( FINDREPLACE *pfr );
   void FindReplace( BOOL fReplace );
   void GetAllText( BOOL bNewBuffer = TRUE );
   BOOL SearchText( LPCTSTR, BOOL, BOOL, CHARRANGE* );
   BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);
   void ReplaceAll(LPCTSTR, LPCTSTR, BOOL bCase);
   void ReplaceSel(LPCTSTR, BOOL, BOOL, LPCTSTR);
   void BuildCommentBuffer( DWORD );
   BOOL BeginInComment( long lAktLineIndex );
   void PrepareForComments( DWORD lAktLineIndex );
   void GetSelectionBuffer();
   int DialogMeldung( long, unsigned int uOptions = MB_OK, const char *pText = NULL );
   void SaveOptions();
   long IsMultipleLineSelection(); // Sind mehrere Zeilen markiert?
   long SelectLinesComplete();     // markierung auf komplette Zeilen ausdehnen
   long MultipleLineIndention();   // mehrere Zeilen einrücken
   CString GetStringFromLineOfIndex( long lIndex );
   long InsertText( LPCSTR pDaten);

   static DWORD CALLBACK ReadCallback(DWORD, LPBYTE, LONG, LONG FAR *);
   static DWORD CALLBACK SaveCallback(DWORD, LPBYTE, LONG, LONG FAR *);
   static DWORD CALLBACK ReadRTFFormat(DWORD, LPBYTE, LONG, LONG FAR *);

   static int CALLBACK WordBreakCallback( LPTSTR lpch, int ichCurrent, int cch, int code );
   static BOOL IsDelimiter( int iChar );

private:
   HINSTANCE m_hLibRTF;    //Handle der RichEdit-DLL
   HMENU m_hMenu;          //Handle des ServerMenus

   BOOL m_bWantKeys;       //merkt, ob Kestrokes gesendet werden sollen
   HFONT m_hFont;

   unsigned int m_msgFind;   //Notification von Find-Dialog
   FINDREPLACE* m_sFindDlg;         //Datenstruktur fuer Find-Dialog

   DWORD m_dLinebufSize;
   CHARRANGE m_chAlt;         // Position vor Insert
   CHARRANGE m_chNeu;         // Position nach Insert

   HWND m_hWndModal;           //Handle der Dialoge
   char *m_pSearchString;      //Zeiger auf Searchstring
   BOOL m_bMatchCase;          //merkt Case-Sensitivitaet fuer Suchen

//   char m_cIndent[100];        //Puffer fuer Idention
//   long m_lIndentCount;        //Anzahl Zeichen in Puffer
//   BOOL m_bReturn;             //Merker fuer Taste Return
   static long m_lTabStop;     //Tabstops
   SyntaxColoring *m_pSyntax;

   COLORREF m_crArray[17];     // Array fuer CustomColors
   BOOL m_bReadOnly;           // merkt ReadOnly-Status

   char *m_pBigBuf;            // Zeiger auf Puffer fuer gesamtText
   DWORD m_dBigBufSize;        // die Groesse des Puffers

   BOOL m_bPosChanged;         // merkt ob Eingabeposition geaendert wurde
   BOOL m_bSelection;          // merkt, ob etwas selektiert wurde
   TEXTRANGE m_tr;             // Struktur fuer Einlesen von Selektionsbereichen
   char m_cNext[2];            // das Zeichen nach der Eingabeposition

   long m_lUndoLevel;          //Anzahl der gemerkten UNDO-Operationen
   long m_lCommentBufferSize;  //max. Anzahl Zeilen im Kommentarpuffer
   HFILE m_hFile;              //Handle auf zu lesende Datei;

   BOOL m_bShift;              //merkt, ob Shift-Taste gedrueckt
   BOOL m_bCtrl;               //merkt, ob Ctrl-Taste gedrueckt

protected:
   char m_cFileName[MAX_PATH];  // Dateiname

   TCHAR m_FontName[LF_FACESIZE];     //Name des Fonts
   long m_FontSize;                   //Groesse des Fonts
   CHARFORMAT m_cf;            // Struktur fuer SyntaxCheck;
   COLORREF m_crForeground;          // normale Textfarbe
   COLORREF m_crBackground;          // Hintergrund
   COLORREF m_crComment;       // Farbe der Kommentare
   COLORREF m_crKeyword;       // Farbe der KeyWords

   char *m_pLineBuf;
   SYNTAX_SPRACHE m_enSyntax;  // Merker fuer SyntaxCheck
   BOOL m_bInsertMode;         //fuer Insert/OverwriteModus

   CUndo *m_pUndo;
   BOOL m_bGerman;             //Merker fuer Sprache True->Deutsch
   BOOL m_bComment;            //Merker fuer C-Syntax Kommentar

   char *m_pCommentBuf;        //Puffer fuer Comment-Range
   long m_lComBufSize;         //dessen Groesse
   DWORD m_dComSize;           //Anzahl der wirklich in Puffer gehaltenen Zeichen
   DWORD m_dComLine;           //LineIndex der ersten in Puffer gehaltenen Zeile
   DWORD m_dComNumberOfLines;  //Anzahl Zeilen im Puffer
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HZOECTL_H__DEA843E2_DAA2_11D1_95B2_0060973B3B55__INCLUDED)
