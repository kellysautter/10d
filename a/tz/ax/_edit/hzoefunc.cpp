/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditor
**
** .Version:       01D
**
** .Aenderung:     11.03.1999
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
    TEXTRANGE tr;
** 2002.12.10   DKS   Z10
**    In CTBEditor::GetEditLine, always initialize the TEXTRANGE structure.
** 2002.08.22   DKS   Z10
**    When sending a message for EM_GETTEXTRANGE, if the range is zero,
**    the return buffer is not set to null.  So we do this explicitly.
** 01A  =  Neuerstellung
**         (18.05.1998)  SL
** 01B  =  Fehler in Methode ProcessFindReplace behoben
**         Erweiterung fuer C-Syntax Kommentare
**         (18.09.1998)  SL
** 01C  =  - SetKeywords veraendert nicht mehr die Cursorposition.
**         - Suchdialog wird bei Bedarf verschoben um Selektion sichtbar
**           zu machen.
**         (01.03.1999)  SL
** 01D  =  SetTabStop() ermittelt jetzt besser die durchschnittliche Zeichen-
**         groesse. Methode InsertLineBreaks ist nicht mehr notwendig.
**         (11.03.1999)  SL
** 01E  =  Added HIDESELECTION message to CTBEditor::ProcessFindReplace to
**         show selection when FindReplace dialog is invoked the second time.
**         (04.11.1999)  DKS
** 01F  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
** 01G  =  Replace default EditWordBreakProc routine. Therefore 2 methods are added
**         to CTBEditor:
**             WordBreakCallback  (see bug report 050599, 050699)
**             IsDelimiter
**         define m_lTabStop as static and replace tabstops by
**         m_lTabStop number of blanks during read
**         Methods:
**            ReadCallback
**            ReadRTFFormat (see bug report 050780, 050668)
**         when pressing the TAB key m_lTabStop number of blanks are inserted
**         instead of insterting a TAB (see bug report 050668)
**         (26.10.2000)  TMV
** 01H  =  Change CTBEditor::IsDelimiter to be compilable under VC++ 5.0
**         because there is no CString::Find( TCHAR ch, int nStart ) member function in vc++ 5.0
**         change FireHelp to fire the RightMouseClickEvent, even if cursor is not positioned
**         on a word
**         remove special treatment of Returnkey in TestInput, because Returnkey is now
**         completely handled in CTBEditor::OnChar to make the undo work properly
**         Change returntype of GetIndentionString from void to CString
**         (1.11.2000)  TMV
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "afxconv.h"
#include "tbedit.h"
#include "hzoectl.h"
#include "RichEdit.h"
#include <commdlg.h>
#include <winnls.h>



extern CTBEditApp NEAR theApp;




/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      DoNotHideThePoint
**
** .Beschreibung: Verschieben von DialogBoxen
**
** .Parameter:    HWND, hWndWindow, I,  Handle des Dialogs
**                long, lIndex,   I, CharIndex des Zeichens dass nicht
**                                   verdeckt werden soll
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode verschiebt Dialogboxen, um ausgewaehlte Bereiche
**      des Hauptfensters sichtbar zu lassen. Verwendung bei den
**      Suchdialogen !!!
**-----------------------------------------------------------------
*/
void CTBEditor::DoNotHideThePoint( HWND hWndWindow, long lIndex )
{
    POINT point;

    SendMessage( EM_POSFROMCHAR, (WPARAM)&point , (LPARAM) lIndex );

    ClientToScreen(&point);
    CRect rectDlg;
    ::GetWindowRect( hWndWindow, &rectDlg );
    if (rectDlg.PtInRect(point))
    {
        if (point.y > rectDlg.Height())
            rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
        else
        {
            int nVertExt = GetSystemMetrics(SM_CYSCREEN);
            if (point.y + rectDlg.Height() < nVertExt)
                rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
        }
        ::MoveWindow( hWndWindow, rectDlg.left, rectDlg.top,
            rectDlg.right-rectDlg.left, rectDlg.bottom-rectDlg.top,
            TRUE);
    }
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ChangeColors
**
** .Beschreibung: Selektion von Farben
**
** .Parameter:    COLORREF* , pCR, U, Pointer auf einen FarbWert
**
** .Rueckgabewert:  BOOL, FALSE -> User hat Dialog abgebrochen
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode nutzt den CommonDialog ChooseColor zur Auswahl
**
**-----------------------------------------------------------------
*/
BOOL CTBEditor::ChangeColors( COLORREF *pCR )
{
    CHOOSECOLOR cc;

   ZeroMemory( &cc, sizeof( CHOOSECOLOR ) );

    cc.lStructSize = sizeof( CHOOSECOLOR );
    cc.hwndOwner = m_hWnd;
    cc.rgbResult = *pCR;
    cc.Flags = CC_RGBINIT | CC_ANYCOLOR;
    cc.lpCustColors = m_crArray;

    if( ChooseColor( &cc ) )
    {
        *pCR = cc.rgbResult;
        return TRUE;
    }
    else
        return FALSE;
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      FindReplace
**
** .Beschreibung: Starten des Suchen-/Ersetzen-Dialogs
**
** .Parameter:    BOOL, fReplace, I, TRUE -> Replace
**                                   FALSE -> Search
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Startet die CommonDialoge FindText / ReplaceText. Es handelt sich
**      dabei um nichtmodale Dialoge.
**      Beim Start der Dialoge wird m_hWndModal belegt. Diese Variable
**      dient als Merker,ob ein Dialog laeuft ( siehe z.B. WindowProc ).
**-----------------------------------------------------------------
*/
void CTBEditor::FindReplace( BOOL fReplace )
{
   //FINDREPLACE
   CString strFind, strReplace;
   LPSTR lpszFind = NULL;
   LPSTR lpszReplace = NULL;
   USHORT lBuffSize = 255;

   lpszFind = strFind.GetBufferSetLength(lBuffSize);
   lpszReplace = strReplace.GetBufferSetLength(lBuffSize);

   /*
   if( ! m_sFindDlg )
       m_sFindDlg = (FINDREPLACE *)new BYTE[ sizeof(FINDREPLACE) + 2 * 80 * sizeof(TCHAR)];

   if( m_sFindDlg )
   {
      ZeroMemory( m_sFindDlg, sizeof(FINDREPLACE) + 2 * 80 * sizeof(TCHAR) );

      SendMessage( EM_HIDESELECTION, FALSE, TRUE );

      m_sFindDlg->lStructSize = sizeof(FINDREPLACE);
      m_sFindDlg->hwndOwner = this->m_hWnd; //GetSafeHwnd();
      m_sFindDlg->Flags = FR_HIDEWHOLEWORD| FR_DOWN ;//| FR_ENABLEHOOK;
      m_sFindDlg->lpstrFindWhat = lpszFind;
      m_sFindDlg->lpstrReplaceWith = lpszReplace;
      m_sFindDlg->wFindWhatLen = lBuffSize;
      m_sFindDlg->wReplaceWithLen = lBuffSize;
      //m_sFindDlg->wFindWhatLen = 80 * sizeof(TCHAR);
      //m_sFindDlg->wReplaceWithLen = 80 * sizeof(TCHAR);
      //m_sFindDlg->lpfnHook = FindReplaceHook;
      //m_sFindDlg->lpstrFindWhat = (LPSTR) (((BYTE *) m_sFindDlg) + sizeof(FINDREPLACE));
      //m_sFindDlg->lpstrReplaceWith = m_sFindDlg->lpstrFindWhat + 80;


      //Initialisieren des Suchstrings
      if( m_pSearchString )
          lstrcpyn( m_sFindDlg->lpstrFindWhat, m_pSearchString, 80 * sizeof(TCHAR) );

      // Suche starten
      if( fReplace )
        m_hWndModal = ReplaceText( m_sFindDlg );
      else
        m_hWndModal = FindText( m_sFindDlg);

      if ( m_hWndModal )
      {
        long oldStyle = GetWindowLong( m_hWndModal, GWL_STYLE );
        long lStyle = oldStyle;
        SetWindowLong(m_hWndModal, GWL_STYLE, lStyle );
      }
      else
      {
         DWORD DERROR = CommDlgExtendedError();
         TRACE0("ERROR Common Dialog Find/Replace\n");
      }
   }
*/
   CWnd *pParent = AfxGetMainWnd( );
   CFindReplaceDialog *pFindReplaceDlg = new CFindReplaceDialog;
   pFindReplaceDlg->Create(fReplace ? FALSE : TRUE,
                            lpszFind,
                            lpszReplace,
                            FR_DOWN ,
                            this);

}

UINT APIENTRY FindReplaceHook( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
   TRACE3("CTBEditor::FindReplaceHook uiMsg = %d, wParam = %d, lParam = %d \n", uiMsg, wParam, lParam );
   switch (uiMsg)
   {
   case WM_INITDIALOG :
      return TRUE;
      break;
   case WM_SETFOCUS :
      return( 0 );
      break;
   case WM_KEYDOWN :
      return( 0 );
      break;
   case WM_KEYUP :
      return( 0 );
      break;
   case WM_CHAR :
      return( 0 );
      break;
   case WM_GETDLGCODE:
      {
         UINT uiDialogCode = 0; //COleControl::OnGetDlgCode();
         uiDialogCode = uiDialogCode | DLGC_WANTALLKEYS | DLGC_HASSETSEL |
                 DLGC_WANTCHARS |  DLGC_WANTARROWS | DLGC_WANTTAB;
         return uiDialogCode;
      }

   default:
      return  DefWindowProc( hdlg, uiMsg, wParam, lParam  );
      break;
   }
}


typedef int (WINAPI* AFX_COMPARE_PROC)(LPCTSTR str1, LPCTSTR str2);

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SameAsSelected
**
** .Beschreibung: Hilfsfunktion fuer Textsuche
**
** .Parameter:    LPCTSTR , lpszCompare, I, Pointer auf Suchstring
**                BOOL    , bCase      , I, TRUE -> casesensitiv
**
** .Rueckgabewert:  BOOL, TRUE -> Strings sind gleich
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode prueft, ob die momentane Selektion im RichEdit identisch
**      ist mit dem Suchstring. Sie setzt voraus, dass m_pBigBuf den
**      aktuellen Inhalt des RichEdits besitzt.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase)
{
   CHARRANGE cha;
   // check length first
   size_t nLen = lstrlen(lpszCompare);
   unsigned long nStartChar, nEndChar;
   SendMessage( EM_EXGETSEL, 0, (LPARAM) &cha );
   nStartChar = cha.cpMin;
   nEndChar = cha.cpMax;
   if (nLen != (size_t)(nEndChar - nStartChar))
      return FALSE;

   // length is the same, check contents ;
   //Vorsicht bei Returnwert von Comparestring!
   return (bCase && CompareString(LOCALE_USER_DEFAULT,0,lpszCompare, nLen,
                                   m_pBigBuf+nStartChar,nLen) == 2) ||
      (!bCase && CompareString(LOCALE_USER_DEFAULT,NORM_IGNORECASE,lpszCompare,
                                   nLen, m_pBigBuf+nStartChar,nLen) == 2);
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SearchText
**
** .Beschreibung: Textsuche
**
** .Parameter:    LPCTSTR , lpszFind, I, Suchstring
**                BOOL    , bNext   , I, TRUE -> Richtung Textende suchen
**                                       FALSE-> Richtung Textanfang
**                BOOL    , bCase   , I, TRUE -> case-sensitiv suchen
**                CHARRANGE*,  pCha , U, Pointer auf CHARRANGE-Struktur zum
**                                       festhalten der gefundenen Position
**
** .Rueckgabewert:  BOOL, FALSE -> nichts gefunden
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode erwartet in m_pBigBuf den aktuellen Textinhalt des
**      RichEdits und in m_dBigBufSize seine Groesse.
**      Die Logik stammt weitestgehend aus MFC.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::SearchText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
                           CHARRANGE *pCha )
{
   ASSERT(lpszFind != NULL);
   if ( *lpszFind == 0 )
      return FALSE;

   //ASSERT(*lpszFind != '\0');

    LPCTSTR lpszText = m_pBigBuf;

    CHARRANGE cha;

   unsigned long nLen = m_dBigBufSize;
   unsigned long nStartChar, nEndChar;

   SendMessage( EM_EXGETSEL, 0, (LPARAM) &cha );
    nStartChar = cha.cpMin;
    nEndChar = bNext ? m_dBigBufSize : 0;
   unsigned long nStart = nStartChar;
   int iDir = bNext ? +1 : -1;

   // can't find a match before the first character
   if (nStart == 0 && iDir < 0)
      return FALSE;

   if (iDir < 0)
   {
      // always go back one for search backwards
      nStart -= (lpszText+nStart) -
         _tcsdec(lpszText, lpszText+nStart);
   }
   else if (nStartChar != nEndChar && SameAsSelected(lpszFind, bCase))
   {
      // easy to go backward/forward with SBCS
      if (_istlead(lpszText[nStart]))
         nStart++;
      nStart += iDir;
   }

   // handle search with nStart past end of buffer
   size_t nLenFind = lstrlen(lpszFind);
   if (nStart+nLenFind-1 >= nLen)
   {
      if (iDir < 0 && nLen >= nLenFind)
      {
         nStart = nLen - nLenFind;
         ASSERT(nStart+nLenFind-1 <= nLen);
      }
      else
      {
         return FALSE;
      }
   }

   // start the search at nStart
   LPCTSTR lpsz = lpszText + nStart;
   AFX_COMPARE_PROC pfnCompare = bCase ? lstrcmp : lstrcmpi;

    unsigned long nCompare;
   if (iDir < 0)
      nCompare = (UINT)(lpsz - lpszText) + 1;
   else
      nCompare = nLen - (UINT)(lpsz - lpszText) - nLenFind + 1;

   while (nCompare > 0)
   {
      ASSERT(lpsz >= lpszText);
      ASSERT(lpsz+nLenFind-1 <= lpszText+nLen-1);

      LPSTR lpch = (LPSTR)(lpsz + nLenFind);
      char chSave = *lpch;
      *lpch = '\0';
      int nResult = (*pfnCompare)(lpsz, lpszFind);
      *lpch = chSave;
      if (nResult == 0)
      {
         unsigned long n = (int)(lpsz - lpszText);

            pCha->cpMin = n;
            pCha->cpMax = n+nLenFind;
         return TRUE;
      }

      // restore character at end of search
      *lpch = chSave;

      // move on to next substring
      nCompare--;
      lpsz += iDir;
   }
   return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ReplaceAll
**
** .Beschreibung: Text ersetzen
**
** .Parameter:    LPCTSTR , lpszFind   , I, Suchstring
**                LPCTSTR , lpszReplcae, I, Suchstring
**                BOOL    , bCase      , I, TRUE -> case-sensitiv suchen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode sucht alle Vorkommen von lpszFind und ersetzt sie durch
**      lpszReplace. Wenn bCase == TRUE, dann wird casesensitiv gesucht.
**      Nach der Suche wird die Anzahl der ersetzten Stellen ausgegeben.
**-----------------------------------------------------------------
*/
void CTBEditor::ReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase)
{
    CHARRANGE cha = m_chAlt; // m_chAlt wird zwischengespeichert fuer UNDO
    long lCount = 0;
    long lLength = lstrlen( lpszReplace );

    m_chAlt.cpMin = 0;
    m_chAlt.cpMax = 0;

    SendMessage( EM_EXSETSEL, 0, (LPARAM) &m_chAlt ); // von Textanfang suchen

    while( SearchText( lpszFind, TRUE, bCase, &m_chAlt ) )
   {
        char *p;

        SendMessage( EM_EXSETSEL, 0, (LPARAM) &m_chAlt );

        //das Undo-Geschaeft zuerst regeln
        GetSelectionBuffer();
        p = m_pUndo->MakeNewBuffer( lpszReplace  );
        m_pUndo->InsertRange( m_chAlt.cpMin, p, m_tr.lpstrText );

        SendMessage( EM_REPLACESEL, FALSE,(LPARAM) lpszReplace );

        m_bPosChanged = TRUE;
        TestInput();

        lCount++;
        GetAllText( FALSE );
   }

    char rgch[10];

    wsprintf(rgch, "%ld ", lCount );
    DialogMeldung( IDS_REPLACE, MB_OK, rgch );

    m_chAlt = cha;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ReplaceSel
**
** .Beschreibung: Text ersetzen
**
** .Parameter:    LPCTSTR , lpszFind   , I, Suchstring
**                BOOL    , bNext   , I, TRUE -> Richtung Textende suchen
**                                       FALSE-> Richtung Textanfang
**                BOOL    , bCase      , I, TRUE -> case-sensitiv suchen
**                LPCTSTR , lpszReplcae, I, Suchstring
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode sucht ein Vorkommen von lpszFind und ersetzt es durch
**      lpszReplace. Wenn bCase == TRUE, dann wird casesensitiv gesucht.
**      bNext bestimmt die Suchrichtung.
**-----------------------------------------------------------------
*/
void CTBEditor::ReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
                           LPCTSTR lpszReplace)
{
    CHARRANGE cha = m_chAlt; // zuerst m_chALt zwischenspeichern
    unsigned long nStartChar, nEndChar;
    char *p;

    SendMessage( EM_EXGETSEL, 0, (LPARAM) &m_chAlt );
    nStartChar = cha.cpMin;
    nEndChar = bNext ? m_dBigBufSize : 0;

    if (nStartChar == nEndChar)
   {
      if (!SearchText( lpszFind, bNext, bCase, &m_chAlt))
      {
            DialogMeldung( IDS_SEARCH );
      }
        m_chAlt = cha;
      return;
   }

   if (!SameAsSelected( lpszFind, bCase))
   {
      if (!SearchText( lpszFind, bNext, bCase, &m_chAlt))
            DialogMeldung( IDS_SEARCH );
        m_chAlt = cha;
      return;
   }

    SendMessage( EM_EXSETSEL, 0, (LPARAM) &m_chAlt );

    GetSelectionBuffer();
    p = m_pUndo->MakeNewBuffer( lpszReplace );
    m_pUndo->InsertRange( m_chAlt.cpMin, p, m_tr.lpstrText );

    SendMessage( EM_REPLACESEL, FALSE,(LPARAM) lpszReplace);

    m_bPosChanged = TRUE;
    TestInput();

    m_chAlt = cha;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ProcessFindReplace
**
** .Beschreibung: Steuerung des Suchvorgangs
**
** .Parameter:    FINDREPLACE *, pfr, I, Datenstruktur des Suchdialogs
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird aufgerufen, wenn der nichtmodale Suchdialog
**      eine Message an das Control schickt. Diese Message kann die
**      naechste Useranforderung oder das Ende des Dialogs mitteilen.
**-----------------------------------------------------------------
*/
void CTBEditor::ProcessFindReplace( FINDREPLACE *pfr )
{
    if( pfr->Flags & FR_DIALOGTERM ) // Dialog wird beendet
   {
        SendMessage( EM_HIDESELECTION, TRUE, 0 );
        SetFocus();
        m_hWndModal = NULL;  //Merker wieder auf NULL setzen
        FireSetLine();
        m_bPosChanged = TRUE;
    }
    else //irgendeine Useranforderung -> Suchen oder Ersetzen
    {
        CHARRANGE cha;
        GetAllText(); // EditText besorgen

        SendMessage( EM_HIDESELECTION, FALSE, 0 );

        //den Suchstring und Case merken fuer F3 - Behandlung
        m_bMatchCase = pfr->Flags & FR_MATCHCASE;

        if( ! m_pSearchString )
        { //wenn noch kein Merkstring
            long lLen = lstrlen( pfr->lpstrFindWhat );
            m_pSearchString = new char [lLen+2];
            lstrcpy( m_pSearchString, pfr->lpstrFindWhat );
        }
        else if( lstrcmp( pfr->lpstrFindWhat, m_pSearchString ) )
        { //wenn die Strings ungleich sind
            delete [] m_pSearchString;
            long lLen = lstrlen( pfr->lpstrFindWhat );
            m_pSearchString = new char [lLen+2];
            lstrcpy( m_pSearchString, pfr->lpstrFindWhat );
        }

        if( pfr->Flags & FR_FINDNEXT ) //Suchen
        {
            if( SearchText( pfr->lpstrFindWhat, pfr->Flags & FR_DOWN,
                            m_bMatchCase, &cha ) )
            {
                SendMessage( EM_EXSETSEL, 0, (LPARAM) &cha );
                m_bPosChanged = TRUE;
                FireSetLine();

                //wir verschieben den Suchdialog, falls er die markierte
                //Fundstelle versteckt
                if( m_hWndModal )
                {
                    DoNotHideThePoint( m_hWndModal, cha.cpMin );
                }
            }
            else
            {
                MessageBeep(0);
                DialogMeldung( IDS_SEARCH );
            }
        }
        else if( pfr->Flags & FR_REPLACE ) // einmal ersetzen
            ReplaceSel( pfr->lpstrFindWhat, pfr->Flags & FR_DOWN,
                        m_bMatchCase, pfr->lpstrReplaceWith );
        else if( pfr->Flags & FR_REPLACEALL ) //alle ersetzen
            ReplaceAll( pfr->lpstrFindWhat, pfr->lpstrReplaceWith,
                        m_bMatchCase );

        delete [] m_pBigBuf;
        m_pBigBuf = NULL;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      TestInput
**
** .Beschreibung: Text fuer Syntax Coloring vorbereiten
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird nach jedem Tastendruck aufgerufen. Sie initiiert
**      das Syntax Coloring und die Indention von neuen Zeilen.
**-----------------------------------------------------------------
*/
void CTBEditor::TestInput()
{
   DWORD dAltLineIndex;
   DWORD dAktLineIndex;
   DWORD dCharIndex;
   CHARRANGE chAkt;
   DWORD dLength;

   if( m_enSyntax == NO_SYNTAX || m_bReadOnly )
       return;

   LockWindowUpdate( );
   ::SendMessage( m_hWnd, EM_HIDESELECTION, TRUE, 0L );
   ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &chAkt );

   dAltLineIndex = ::SendMessage( m_hWnd, EM_EXLINEFROMCHAR, 0, (LPARAM)m_chAlt.cpMin );
   dAktLineIndex = ::SendMessage( m_hWnd, EM_EXLINEFROMCHAR, 0, (LPARAM)chAkt.cpMin );

   if( dAltLineIndex > dAktLineIndex )
   {
      DWORD dHilf;
      dHilf = dAltLineIndex;
      dAltLineIndex = dAktLineIndex;
      dAktLineIndex = dHilf;
   }

   if( m_chAlt.cpMin != m_chAlt.cpMax || dAltLineIndex != dAktLineIndex )
   {
      m_bComment = BeginInComment( dAltLineIndex - 1 );
      BuildCommentBuffer( dAktLineIndex );
   }
   else
   {
      if( m_bPosChanged )
         PrepareForComments( dAktLineIndex );
   }

   m_bPosChanged = FALSE;

   if( dAltLineIndex == dAktLineIndex )
   {
      dCharIndex = ::SendMessage( m_hWnd, EM_LINEINDEX, (WPARAM)dAktLineIndex, 0L );
      dLength = ::SendMessage( m_hWnd, EM_LINELENGTH, (WPARAM)dCharIndex, 0L );
      GetEditLine( dCharIndex, dCharIndex + dLength );
      m_pSyntax->ParseLine( m_pLineBuf );
   }
   else
   {
      DWORD dAnfangIndex;
      DWORD dEndeIndex;

      dAnfangIndex = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                (WPARAM)dAltLineIndex, 0L );
      dEndeIndex = ::SendMessage( m_hWnd, EM_LINEINDEX,
                              (WPARAM)dAktLineIndex, 0L );

      DWORD dLength = ::SendMessage( m_hWnd, EM_LINELENGTH, dEndeIndex, 0L );
      dEndeIndex += dLength;

      GetEditLine( dAnfangIndex, dEndeIndex );
      m_pSyntax->ParseLine( m_pLineBuf );
   }

   ::SendMessage( m_hWnd, EM_HIDESELECTION, FALSE, 0L );
   ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &chAkt );

   UnlockWindowUpdate( );
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SetKeywords
**
** .Beschreibung: Gesamten text mit Syntax Coloring bearbeiten
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Mit dieser Methode wird der gesamte Text bearbeitet. Sie wird
**      beim Laden einer neuen Datei und beim Wechsel von Farbeinstellungen
**      aufgerufen.
**-----------------------------------------------------------------
*/
void CTBEditor::SetKeywords()
{
    if( m_enSyntax == NO_SYNTAX ) //sicher ist sicher!
        return;

    CHARRANGE cha;

    BeginWaitCursor();

    ::SendMessage( m_hWnd, EM_HIDESELECTION, TRUE,(LPARAM) 0 );

    //aktuelle Cursorposition merken
    SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );

    GetAllText(); //Text nach m_pBigBuf bringen

    LockWindowUpdate();
    m_pSyntax->ParseAll( m_pBigBuf );
    UnlockWindowUpdate();

    ::SendMessage( m_hWnd, EM_HIDESELECTION, FALSE,(LPARAM) 0 );

    SendMessage( EM_EXSETSEL, 0,(LPARAM) &cha );

    FireSetLine();
    m_bPosChanged = TRUE;

    delete [] m_pBigBuf;
    m_pBigBuf = NULL;

    EndWaitCursor();
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetAllText
**
** .Beschreibung: Kopieren des RichEdit-Inhalts in privaten Puffer
**
** .Parameter:    BOOL, bNewPuffer, I, TRUE->neuer Puffer soll angelegt werden
**                                      TRUE ist Vorbelegung
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Da es keine Moeglichkeit gibt, direkt auf den Editpuffer des
**      Richedits zuzugreifen muss u.U. der gesamte Text in einen
**      eigenen Puffer uebertragen werden. Dies geschieht z.B. bei
**      allen Suchvorgaengen.
**      Der private Puffer wird ueber m_pBigBuf adressiert, seine
**      Groesse steht in m_dBigBufSize
**-----------------------------------------------------------------
*/
void CTBEditor::GetAllText( BOOL bNewPuffer )
{
    TEXTRANGE tr;
    DWORD dSize = SendMessage( WM_GETTEXTLENGTH ); //wie gross ist der Text

    if( bNewPuffer || ( ! m_pBigBuf || m_dBigBufSize <= dSize ) )
    {
        if( m_pBigBuf )
            delete [] m_pBigBuf;
        m_pBigBuf = new char[dSize + 100];
        m_dBigBufSize = dSize+100;
    }

    ZeroMemory( m_pBigBuf, m_dBigBufSize );

    if( m_pSyntax )
    {   //wenn Syntax Coloring -> Anfang- und Endewerte fuer Verarbeitung setzen
        m_pSyntax->m_cha.cpMin = 0;
        m_pSyntax->m_cha.cpMax = dSize;
    }

    tr.chrg.cpMin = 0;
    tr.chrg.cpMax = dSize;
    tr.lpstrText = m_pBigBuf;

    ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &tr );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetEditLine
**
** .Beschreibung: Kopieren einer Textzeile in privaten Puffer
**
** .Parameter:    DWORD, dLineIndex, I, Zeilenindex der zu bearbeitenden
**                                      Zeile
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode kopiert den Inhalt der ausgewaehlten Zeile in den
**      Puffer m_pLineBuf. Seine Groesse steht in m_dLinebufSize.
**-----------------------------------------------------------------
*/
void CTBEditor::GetEditLine( DWORD dAnfangIndex, DWORD dEndeIndex )
{
    DWORD dLineLength;
    TEXTRANGE tr;

    if( m_pSyntax )
    {
       m_pSyntax->m_cha.cpMin = dAnfangIndex;
       m_pSyntax->m_cha.cpMax = dEndeIndex;
    }

    dLineLength = dEndeIndex - dAnfangIndex;

    if( dLineLength >= m_dLinebufSize )
    {
        delete [] m_pLineBuf;
        m_dLinebufSize = dLineLength + 100;
        m_pLineBuf = new char[m_dLinebufSize + 1];// one more for \0
    }

    tr.chrg.cpMin = dAnfangIndex;
    tr.chrg.cpMax = dEndeIndex;
    tr.lpstrText = m_pLineBuf;
    if ( dEndeIndex <= dAnfangIndex )
       m_pLineBuf[ 0 ] = 0;

    ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &tr );
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetIndentionString
**
** .Beschreibung: Parsen des Zeilenanfangs auf Blanks und Tabs
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode holt sich die Einrückung der vorangegangenen Zeile
**      und liefert diese Zeichenkette zurück
**-----------------------------------------------------------------
*/
CString CTBEditor::GetIndentionString( void )
{
   CString strHelp;
   LONG  dLineLength;
   LONG  dLineIndex;
   LONG  dCharIndex;
   TEXTRANGE tr;
   CHARRANGE cha;

   strHelp.Empty();
   if( ! m_hWnd )
      return strHelp;

   //aktuelle Char-Position
   SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
   //aktuellen Zeileindex
   dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)cha.cpMin );
   if( dLineIndex < 0 )
      return strHelp;

   //CharIndex der aktuellen Zeile
   dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex, 0L );
   if ( dCharIndex == cha.cpMin )
   {
      //we are a the beginning of the line, so no indention should occur
      return strHelp;
   }
   //Laenge ermitteln
   dLineLength = SendMessage( EM_LINELENGTH, (WPARAM)dCharIndex, 0L );

   tr.chrg.cpMin = dCharIndex;
   tr.chrg.cpMax = dCharIndex + dLineLength;
   tr.lpstrText = strHelp.GetBufferSetLength( dLineLength );
   if( SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr ) > 0 )
   {
      LPSTR p = tr.lpstrText;
      int i = 0;
      while ( *p && ::isspace( *p ))
      {
         i++;
         p++;
      }
      strHelp.ReleaseBuffer();
      if ( i > 0 )
         strHelp = strHelp.Mid( 0, i );
      else
         strHelp.Empty();
   }
   else
   {
      strHelp.Empty();
   }


   return strHelp;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ReadCallback
**
** .Beschreibung: Lesefunktion des RichEdit-Controls
**
** .Parameter:    DWORD    , dwCookie , I, Handle des Files
**                LPBYTE   , pbBuff   , U, Zielpuffer
**                LONG     , cb       , I, Anzahl zu lesende Zeichen
**                LONG FAR*, pcb      , O, Anzahl uebertragener Zeichen
**
** .Rueckgabewert:   DWORD, 0 -> alles o.k.
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode ist eine statische Callback-Methode dem vom Editstream
**      des Richedits aufgerufen wird, um Daten aus einer Datei ins Control
**      zu uebertragen.
**      Sie wird nur verwendet, wenn keine Syntax-Pruefung eingeschaltet ist.
**-----------------------------------------------------------------
*/
DWORD CALLBACK CTBEditor::ReadCallback( DWORD dwCookie, LPBYTE pbBuff, LONG cb,
                                        LONG FAR *pcb )
{
   DWORD result = 1;
   CString strHelp( (char) 0x20, (int) m_lTabStop);// creating a string containing
                                       // a blank per tabstop size
   BYTE b;
   unsigned long lReadCount = 1;
   long lBuffCount = 0;
   LPBYTE pCurrentBuffer = pbBuff;

   // read byte per byte to replace TABS
   while ( result && lBuffCount < cb && lReadCount > 0)
   {
      result = ReadFile((HANDLE)dwCookie, &b, sizeof( b ), &lReadCount, NULL);
      if( result && lReadCount > 0 ) // no EOF so far
      {
         if( b == '\t' )
         {
            memcpy( (LPVOID) pCurrentBuffer, ( LPVOID )( LPCSTR )strHelp, strHelp.GetLength() );
            pCurrentBuffer += strHelp.GetLength();
            lBuffCount += strHelp.GetLength();
         }
         else
         {
            *pCurrentBuffer = b;
            pCurrentBuffer++;
            lBuffCount++;
         }
      }
   }

   *pcb = lBuffCount;
   if (!result)
       return GetLastError ();
   return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SaveCallback
**
** .Beschreibung: Schreibfunktion des RichEdit-Controls
**
** .Parameter:    DWORD    , dwCookie , I, Handle des Files
**                LPBYTE   , pbBuff   , U, Sendepuffer
**                LONG     , cb       , I, Anzahl zu schreibender Zeichen
**                LONG FAR*, pcb      , O, Anzahl uebertragener Zeichen
**
** .Rueckgabewert:   DWORD, 0 -> alles o.k.
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode ist eine statische Callback-Methode dem vom Editstream
**      des Richedits aufgerufen wird, um Daten in eine Datei
**      zu uebertragen.
**-----------------------------------------------------------------
*/
DWORD CALLBACK CTBEditor::SaveCallback( DWORD dwCookie, LPBYTE pbBuff, LONG cb,
                                        LONG FAR *pcb)
{
    WriteFile((HANDLE)dwCookie, pbBuff, cb, (unsigned long *)pcb, NULL );
    return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      WordBreakCallback
**
** .Beschreibung: Markierungsfunktion des RichEdit-Controls
**
** .Parameter:    LPTSTR   , lpch      , I, textstring
**                int      , ichCurrent, I, startindex
**                int      , cch       , I, textlänge
**                int      , code      , I, aktion
**
** .Rueckgabewert:   DWORD, 0 -> alles o.k.
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode ist eine statische Callback-Methode dem vom
**      des Richedit aufgerufen wird, worte zu markieren
**
**-----------------------------------------------------------------
*/
int CALLBACK CTBEditor::WordBreakCallback(  LPTSTR lpch,
                                            int ichCurrent,
                                            int cch,
                                            int code )
{
   LPTSTR lpCurrentChar;
   int  nIndex;
   BOOL bCharFound = FALSE;

   switch ( code )
   {
      case WB_ISDELIMITER:

      // Windows sends this code so that the wordbreak function can
      // check to see if the current character is the delimiter.
         if ( isspace( lpch[ichCurrent] ) )
            return TRUE;
         else
            return FALSE;

         break;


      case WB_MOVEWORDLEFT:
         lpCurrentChar = lpch + ichCurrent - 1;
         nIndex = ichCurrent;

         //if current character is a delimiter step right to the beginnig of the next
         // word
         if ( *(lpCurrentChar) == 0x0A )
         {
            lpCurrentChar=CharPrev(lpch, lpCurrentChar);
            nIndex--;
         }

         bCharFound = FALSE;
         if( IsDelimiter( *lpCurrentChar ) )
            bCharFound = FALSE;
         else
            bCharFound = TRUE;

         if ( bCharFound )
         {
            while ( nIndex > 0 &&  *(lpCurrentChar) != 0x0A )
            {
               if ( IsDelimiter ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharPrev(lpch, lpCurrentChar);
               nIndex--;
            }
         }
         else
         {
            while ( nIndex > 0 &&  *(lpCurrentChar) != 0x0A)
            {
               if ( !IsDelimiter ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharPrev(lpch, lpCurrentChar);
               nIndex--;
            }
         }

         return nIndex;
         break;


      case WB_MOVEWORDRIGHT:

         lpCurrentChar = lpch + ichCurrent;
         nIndex = ichCurrent;

         //if current character is a delimiter step right to the beginnig of the next
         // word
         if ( *(lpCurrentChar) == 0x0D )
         {
            lpCurrentChar=CharNext( lpCurrentChar );
            nIndex++;
         }

         if ( *(lpCurrentChar) == 0x0A )
         {
            lpCurrentChar=CharNext( lpCurrentChar );
            nIndex++;
         }

         bCharFound = FALSE;
         if( IsDelimiter( *lpCurrentChar ) )
            bCharFound = FALSE;
         else
            bCharFound = TRUE;

         if ( bCharFound )
         {
            while ( nIndex < cch &&  *(lpCurrentChar) != 0x0A )
            {
               if ( IsDelimiter ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharNext(lpCurrentChar);
               nIndex++;
            }
         }
         else
         {
            while ( nIndex < cch &&  *(lpCurrentChar) != 0x0A )
            {
               if ( !IsDelimiter ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharNext(lpCurrentChar);
               nIndex++;
            }
         }

         return nIndex;
         break;
      case WB_LEFTBREAK:

         lpCurrentChar = lpch + ichCurrent - 1;
         nIndex = ichCurrent;

         //if current character is a delimiter step right to the beginnig of the next
         // word
         if ( *(lpCurrentChar) == 0x0A )
         {
            lpCurrentChar=CharPrev(lpch, lpCurrentChar);
            nIndex--;
         }

         if ( *(lpCurrentChar) == 0x0D )
         {
            lpCurrentChar=CharPrev(lpch, lpCurrentChar);
            nIndex--;
         }

         bCharFound = FALSE;
         if( isspace( *lpCurrentChar ) )
            bCharFound = FALSE;
         else
            bCharFound = TRUE;

         if ( bCharFound )
         {
            while ( nIndex > 0 &&  *(lpCurrentChar) != 0x0A )
            {
               if ( isspace ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharPrev(lpch, lpCurrentChar);
               nIndex--;
            }
         }
         else
         {
            while ( nIndex > 0 &&  *(lpCurrentChar) != 0x0A )
            {
               if ( !isspace ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharPrev(lpch, lpCurrentChar);
               nIndex--;
            }
         }

         return nIndex;
         break;

      case WB_RIGHTBREAK:

         lpCurrentChar = lpch + ichCurrent;
         nIndex = ichCurrent;

         //if current character is a delimiter step right to the beginnig of the next
         // word
         if ( *(lpCurrentChar) == 0x0D )
         {
            lpCurrentChar=CharNext( lpCurrentChar );
            nIndex++;
         }

         if ( *(lpCurrentChar) == 0x0A )
         {
            lpCurrentChar=CharNext( lpCurrentChar );
            nIndex++;
         }

         bCharFound = FALSE;
         if( IsDelimiter( *lpCurrentChar ) )
            bCharFound = FALSE;
         else
            bCharFound = TRUE;

         if ( bCharFound )
         {
            while ( nIndex < cch &&  *(lpCurrentChar) != 0x0A )
            {
               if ( IsDelimiter ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharNext(lpCurrentChar);
               nIndex++;
            }
         }
         else
         {
            while ( nIndex < cch &&  *(lpCurrentChar) != 0x0A )
            {
               if ( !IsDelimiter ( *( lpCurrentChar ) ) )
                  return nIndex;
               lpCurrentChar=CharNext(lpCurrentChar);
               nIndex++;
            }
         }

         return nIndex;
         break;

      case WB_CLASSIFY:

         lpCurrentChar = lpch + ichCurrent;
         if ( isalnum( *lpCurrentChar ) )
            return WBF_BREAKAFTER;
         else
         if ( *lpCurrentChar == 0x0D )
            return WBF_BREAKLINE;
         else
         if( isspace( *lpCurrentChar ) )
            return WBF_ISWHITE;
         else
         if ( *lpCurrentChar == 0x0A )
            return WBF_ISWHITE;
         if ( IsDelimiter( *lpCurrentChar ) )
            return WBF_BREAKLINE;
         else
            return WBF_BREAKLINE;
         break;

      default:
         break;

   }
   return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditor
**
** .Methode:       IsDelimiter
**
** .Beschreibung:  Überprüft ob ein Zeichen ein Wortbegrenzer ist
**
** .Parameter:     int     , iChar  , I, zu überprüfendes Zeichen
**
** .Rueckgabewert: BOOL, TRUE wenn das Zeichen einen Wortbegrenzer darstellt,
**                       ansonsten FALSE
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
BOOL CTBEditor::IsDelimiter( int iChar )
{
   BOOL bReturn = FALSE;

   CString strHelp = "\\!\"§$%&/()=?'+*<>,;.:-";

   // check for spaces
   bReturn = isspace( iChar );

   // check for CRLF
   if (!bReturn )
      if ( iChar == 0x0A || iChar == 0x0D )
         bReturn = TRUE;

   // check for operators, parentesis, etc.
   if (!bReturn )
      //if( strHelp.Find( iChar, 0 ) >= 0 ) there is no CString::Find( TCHAR ch, int nStart )
      //                                    member function in vc++ 5.0
      if (  strHelp.Find( iChar ) >= 0 )
         bReturn = TRUE;

   return bReturn;
}
/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ScrollToLine
**
** .Beschreibung: Scrollfunktion
**
** .Parameter:    LONG     , lLine  , I, Nummer der neuen akt. Zeile
**
** .Rueckgabewert:   DWORD, neuer zero-based character index
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode scrollt zur angegebenen Zeilen, wobei die Nummerierung
**      der Zeilen mit 1 beginnt.
**-----------------------------------------------------------------
*/
DWORD CTBEditor::ScrollToLine( long lLine )
{ // scrolled zur angegebenen Zeile ( 1.Zeile = 1 )
    DWORD dNewLine;
    DWORD dLineIndex;
    DWORD dMaxLine;
    CHARRANGE cha;

      dMaxLine = SendMessage( EM_GETLINECOUNT );
    dMaxLine --; //um auf zero-based Index zu kommen

    //aktuelle Zeile bestimmen
    SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
    dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM) cha.cpMin );

    if( lLine > 0 )
        lLine -- ;
    else lLine = 0;

    if( (unsigned long)lLine > dMaxLine )
        lLine = dMaxLine;

    if ( lLine < 3 )
        dNewLine = 0 - dLineIndex;
    else
        dNewLine = lLine - dLineIndex - 2; //mindestens 3 Zeilen

    SendMessage( EM_LINESCROLL, 0, dNewLine );

    //Cursor auf die neue Zeile positionieren
      DWORD dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM) lLine, 0 );
    cha.cpMin = cha.cpMax = dCharIndex;
    SendMessage( EM_EXSETSEL, 0,(LPARAM) &cha );

    return dCharIndex;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      InitializeFileName
**
** .Beschreibung: internen Standarddateinamen setzen
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      CTBEditor merkt sich intern den Namen der bearbeiteten Datei.
**      Ist keine Datei in Bearbeitung, wird ein Standardnamen verwendet,
**      in Abhaengigkeit von den Spracheinstellungen des Betriebssystems.
**-----------------------------------------------------------------
*/
void CTBEditor::InitializeFileName()
{
    if( m_bGerman )
        lstrcpy(m_cFileName, DATEIGER );
    else
        lstrcpy(m_cFileName, DATEIENG );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SetTabStops
**
** .Beschreibung: Setzen der Tabstop-Positionen des RichEdits
**
** .Parameter:    long  , lAnz  , I, Einheiten zwischen einzelnen Stops
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Das RichEdit verwendet eine feste Positionierung von Tabstops, deren
**      Abstand in irgendwelchen Units festgelegt wird. Dieser Abstand ist
**      unabhaengig vom verwendeten Font.
**-----------------------------------------------------------------
*/
void CTBEditor::SetTabStops( long lAnz )
{
    CHARRANGE cha, chMerk;
    long lPos = 0;
    PARAFORMAT pf;
    long lSize;

    lAnz++;

    cha.cpMin = 0;  //alles auswaehlen
    cha.cpMax = -1;

    ZeroMemory( &pf, sizeof( PARAFORMAT ) );
    pf.cbSize = sizeof( PARAFORMAT );

    //Breite der Zeichen bestimmen
   HDC hdc;
    TEXTMETRIC tm;
    hdc = ::GetDC( m_hWnd );
    HGDIOBJ ups = ::SelectObject( hdc, m_hFont );
    GetTextMetrics( hdc, &tm );
    ::ReleaseDC( m_hWnd, hdc );

    lSize = tm.tmAveCharWidth; //durchschnittliche Breite eines Zeichen

    SendMessage( EM_HIDESELECTION, TRUE, 0L );

    SendMessage( EM_EXGETSEL, 0, (LPARAM) &chMerk ); //Position merken
    SendMessage( EM_EXSETSEL, 0, (LPARAM) &cha );    //alles markieren


    //PARAFORMAT-STruktur fuellen
    pf.dwMask = PFM_TABSTOPS;
    pf.cTabCount  = MAX_TAB_STOPS;
    for( int i = 0; i < MAX_TAB_STOPS; i++ )
    {
        lPos += lAnz * lSize * 10;
        pf.rgxTabs[i] = lPos;
    }

    SendMessage( EM_SETPARAFORMAT, 0, (LPARAM)&pf );
    InvalidateRect( NULL );

    SendMessage( EM_EXSETSEL, 0, (LPARAM) &chMerk );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      FireSetLine
**
** .Beschreibung: Ausloesen des SetLine - Events
**
** .Parameter:  BOOL, bChange, I, TRUE -> neues m_chAlt setzen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode ermittelt die aktuelle Zeile und Spalte( ab 1 ) und
**      loest das SetLine - Event aus
**
**-----------------------------------------------------------------
*/
void CTBEditor::FireSetLine( BOOL bChange )
{
    DWORD dLineIndex;
    DWORD dCharIndex;
    CHARRANGE cha;

    ::SendMessage( m_hWnd, EM_EXGETSEL, 0, (LPARAM) &cha );
    dLineIndex = ::SendMessage( m_hWnd, EM_EXLINEFROMCHAR, 0, (LPARAM)cha.cpMin );

    dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex, (LPARAM) 0 );

    if( bChange )
        m_chAlt = cha;

    Fire_SetLine( dLineIndex + 1, (cha.cpMin - dCharIndex) + 1 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      FireNewPosition
**
** .Beschreibung: Ausloesen des SetLine - Events
**
** .Parameter:      long, lPos, I, der aktuelle character-Index
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode ermittelt die aktuelle Zeile und Spalte( ab 1 ) und
**      loest das SetLine - Event aus
**
**-----------------------------------------------------------------
*/
void CTBEditor::FireNewPosition( long lPos )
{
    DWORD dLineIndex;
    DWORD dCharIndex;

    dLineIndex = ::SendMessage( m_hWnd, EM_EXLINEFROMCHAR, 0, (LPARAM)lPos );

    dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex, (LPARAM) 0 );

    Fire_SetLine( dLineIndex + 1, ( lPos - dCharIndex ) + 1 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      FireHelp
**
** .Beschreibung: Ausloesen des RightMouseClick- Events
**
** .Parameter:   DWORD, dCharIndex, I, Clickposition
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode erhaelt die Position des Mausklicks und emittelt daraus
**      das angeklickte Wort. Dieses wird mit dem Event RightMouseClick.
**-----------------------------------------------------------------
*/
void CTBEditor::FireHelp( DWORD dCharIndex )
{
    USES_CONVERSION;

    CString strHelp;
    strHelp.Empty();

    TEXTRANGE tr;
    tr.chrg.cpMin = 0;
    tr.chrg.cpMax = 0;
    tr.lpstrText = NULL;

    if( ! SendMessage( EM_FINDWORDBREAK, WB_ISDELIMITER, (LPARAM) dCharIndex ) )
    {   //pruefen, ob an der gewaehlten Position ein Wort ist
       //Wortanfang finden mit WordBreak-Funktion
       DWORD dAnfang = SendMessage( EM_FINDWORDBREAK, WB_MOVEWORDLEFT,
                                              (LPARAM) dCharIndex + 1 );

       DWORD dEnde = SendMessage( EM_FINDWORDBREAK, WB_MOVEWORDRIGHT,
                                              (LPARAM) dCharIndex );

       tr.chrg.cpMin = dAnfang;
       tr.chrg.cpMax = dEnde;
       tr.lpstrText = strHelp.GetBufferSetLength( dEnde - dAnfang );

       SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr );
    }


    BSTR x = A2BSTR( tr.lpstrText ); //umwandeln in BSTR
    Fire_RightMouseClick( &x );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      CreateMyFont
**
** .Beschreibung: Erzeugen eines Fonts fuer RichEdit
**
** .Parameter:   char*, pFont  , I, Name des Fonts
**               long , nHeight, I, Fontgroesse
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode erzeugt einen neuen Fonthandle fuer das RichEdit.
**-----------------------------------------------------------------
*/
void CTBEditor::CreateMyFont( const char *pFont, long nHeight )
{
    LOGFONT lf;
   LONG yPerInch;
   HDC hdc;

    hdc = ::GetDC( m_hWnd );
   yPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
    ::ReleaseDC( m_hWnd, hdc );

    ZeroMemory( &lf, sizeof( LOGFONT ) );

    lf.lfHeight = -(INT) ((nHeight * yPerInch) / 1440);

    lf.lfWidth = 0;
   lf.lfEscapement = 0;
   lf.lfOrientation = 0;
   lf.lfWeight = FW_NORMAL;
   lf.lfItalic = FALSE;
   lf.lfUnderline = FALSE;
   lf.lfStrikeOut = FALSE;
   lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
   lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
   lf.lfQuality = DRAFT_QUALITY;
   lf.lfCharSet = ANSI_CHARSET;
   lf.lfPitchAndFamily = FF_DONTCARE;
    lstrcpy( lf.lfFaceName, pFont );

    if( m_hFont )
        DeleteObject( m_hFont );

    m_hFont = CreateFontIndirect( &lf );
}


/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditor
**
** .Methode:       DialogMeldung
**
** .Beschreibung:  Ausgabe von Programmeldungen
**
** .Parameter:     long,   lID    , I, ID der Stringkonstanten
**                 unsigned int, uOptions, I, Ausgabeoptionen fuer MessageBox
**                 char *      , pText   , I, zusaetzlicher Ausgabetext
**
** .Rueckgabewert: int, Ergebnis der Funktion MessageBox
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode kopiert einen String aus der String-Ressource in das
**      Feld m_cAusgabe, um Ausgaben ueber MessageBox() zu machen.
**      uOptions ist mit MB_OK vorbelegt.
**      pText ist mit NULL vorbelegt. Falls zusaetzlicher Text ausgegeben
**      werden soll, wird er vor den Ressourcenstring gesetzt.
**-----------------------------------------------------------------
*/
int CTBEditor::DialogMeldung( long lID, unsigned int uOptions, const char *pText )
{
    int iErg;
    char p[100];


    if( ! LoadString( theApp.m_hInstance, lID, p, sizeof( p ) * sizeof(TCHAR) ) )
        p[0] = '\0';

    if( m_hWndModal ) //gibt es einen nicht-modalen Dialog?
    { //Disable
        ::EnableWindow( m_hWndModal, FALSE );
    }

    if( pText )
    {
        char p1[150];

        lstrcpy( p1, pText );
        lstrcat( p1, p );
        iErg = MessageBox( p1, PROGNAME, uOptions );
    }
    else
        iErg = MessageBox( p, PROGNAME, uOptions );

    if( m_hWndModal ) //gibt es einen nicht-modalen Dialog?
    { //Enable
        ::EnableWindow( m_hWndModal, TRUE );
    }

    return iErg;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      PrepareForComments
**
** .Beschreibung: Vorbereitung fuer das Fuellen des Textpuffers
**                ( hinter der Eingabezeile) und die Untersuchung, ob die
**                aktuelle Zeile Teil eines Kommentars ist
**
** .Parameter:   DWORD, dLineIndex,  I ,  aktueller Zeilenindex
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          Aufruf von BeginInComment und BuildCommentBuffer
**-----------------------------------------------------------------
*/
void CTBEditor::PrepareForComments( DWORD dLineIndex )
{
    //untersuchen, ob vorhergehende Zeile Teil eines C-Syntax Kommentars ist
    if( dLineIndex == 0 )
        m_bComment = FALSE;
    else
        m_bComment = BeginInComment( dLineIndex - 1 );

    //Kommentarpuffer aufbauen
    BuildCommentBuffer( dLineIndex );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      BuildCommentBuffer
**
** .Beschreibung: Erstellen eines Textpuffers ( hinter der Eingabezeile)
**
** .Parameter:   DWORD, dAktLine, I, aktueller Zeilenindex
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird fuer die Bearbeitung des C-Style Kommentars be-
**      noetigt. Sie aktualisiert einen Textbuffer, der den Text "hinter"
**      der aktuell bearbeiteten Zeile enthaelt. Dieser Puffer wird ver-
**      wendet, um das Ende eines Kommentars zu finden. Er stellt prinzipiell
**      den Bereich dar, der beim Kommentieren "eingefaerbt" werden kann.
**-----------------------------------------------------------------
*/
void CTBEditor::BuildCommentBuffer( DWORD dAktLineIndex )
{
    DWORD dLineLength;
    DWORD dIndexAnfang;
    DWORD dIndexEnde;
    DWORD dMaxLines;

    if( m_enSyntax != ZEIDON_SYNTAX )
    {
        m_dComSize = 0;
        m_dComNumberOfLines = 0;
        m_pCommentBuf[0] = '\0';
        return;
    }

    //Puffer neu laden m_lCommentBufferSize Zeilen
    dAktLineIndex++;
    m_dComLine = dAktLineIndex;

    dMaxLines = ::SendMessage( m_hWnd, EM_GETLINECOUNT, 0, 0L );

    dIndexAnfang = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                (WPARAM)dAktLineIndex, 0L );

    if( dIndexAnfang == -1 ) //diese Zeile gibt es nicht, d.h wir schreiben
                             //am Textende
    {
        m_dComSize = 0;
        m_dComNumberOfLines = 0;
        m_pCommentBuf[0] = '\0';
    }
    else
    {
        if( dMaxLines <= dAktLineIndex + m_lCommentBufferSize )
        {
            m_dComNumberOfLines = dMaxLines - dAktLineIndex; // Anzahl Zeilen im Puffer
            dAktLineIndex = dMaxLines -1;
            dIndexEnde = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                        (WPARAM)dAktLineIndex, 0L );
            dLineLength = ::SendMessage( m_hWnd, EM_LINELENGTH,
                                         (WPARAM)dIndexEnde, 0L );
            dIndexEnde += dLineLength;
        }
        else
        {
            m_dComNumberOfLines = m_lCommentBufferSize;
            dAktLineIndex += m_lCommentBufferSize;
            dIndexEnde = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                    (WPARAM)dAktLineIndex, 0L );
        }

        long dSize = dIndexEnde - dIndexAnfang + 50;

        TEXTRANGE tr;

        if( dSize >= m_lComBufSize )
        {
            delete [] m_pCommentBuf;
            m_lComBufSize = dSize + 100;
            m_pCommentBuf = new char[m_lComBufSize];
        }

        tr.chrg.cpMin = dIndexAnfang;
        tr.chrg.cpMax = dIndexEnde;
        tr.lpstrText = m_pCommentBuf;

        m_dComSize = ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr );
    }
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      BeginInComment
**
** .Beschreibung: Feststellen, ob akt. Zeile innerhalb eines Kommentars
**
** .Parameter:   long, lLineIndex, I, aktueller Zeilenindex
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode versucht zu bestimmen, ob man sich in der
**      aktuellen Zeile innerhalb eines Kommentars befindet, d.h.
**      ob die vorhergehende Zeile Bestandteil eines C-Style Kommentars
**      ist.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::BeginInComment( long lLineIndex )
{
    TEXTRANGE tr;
    DWORD dCharIndex;
    DWORD dMerkIndex;
    CHARRANGE chFarbe;
    CHARFORMAT cfo;
    char *p;
    BOOL bInComment = FALSE;


    if( m_enSyntax != ZEIDON_SYNTAX )
        return FALSE;

    ZeroMemory( &cfo, sizeof( CHARFORMAT ) );
    cfo.cbSize = sizeof( CHARFORMAT );

    //Ende des zu untersuchenden Bereichs bestimmen = Anfang der akt. Zeile
    dMerkIndex = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                (WPARAM)lLineIndex + 1, 0L );

    while( lLineIndex >= 0 )
    {
        //ZeilenIndex
        dCharIndex = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                   (WPARAM)lLineIndex, 0L );

        //Char-Position setzen und Farbe abfragen
        chFarbe.cpMin = dCharIndex;
        chFarbe.cpMax = dCharIndex + 1;

        ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &chFarbe );
        ::SendMessage( m_hWnd, EM_GETCHARFORMAT, TRUE, (LPARAM) &cfo );

        if( (cfo.dwMask & CFM_COLOR) && cfo.crTextColor == m_crComment )
        {
            tr.chrg.cpMin = dCharIndex;
            tr.chrg.cpMax = dCharIndex+2;
            tr.lpstrText = m_pLineBuf;

            ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr );

            if( m_pLineBuf[0] == '/' && m_pLineBuf[1] == '/' )
            {
                lLineIndex --;
                continue;
            }
            if( m_pLineBuf[0] == '\r' || m_pLineBuf[0] == '\n' )
            {
                lLineIndex --;
                continue;
            }
            bInComment = TRUE;
        }
        break;
    }


    long lSize = dMerkIndex - dCharIndex;

    if( lSize >= m_lComBufSize )
    {
        delete [] m_pCommentBuf;
        m_lComBufSize = lSize + 100;
        m_pCommentBuf = new char[m_lComBufSize];
    }

    tr.chrg.cpMin = dCharIndex;
    tr.chrg.cpMax = dMerkIndex;
    tr.lpstrText = m_pCommentBuf;

    ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr );


    p = m_pCommentBuf;

    long lZustand; // 0->kein Kommentar, 1->C-Style, 2->C++-Style

    if( bInComment )
        lZustand = 1;
    else
        lZustand = 0;


    while( *p )
    {  //nach Kommentarende suchen, wenn in Kommentar
        if( lZustand == 1 )
        {
            if( *p == '/' && *(p+1) == '*' ) // wir koennen zuerst auf Anfang
                                             //eines Kommentars stehen -> ueberlesen
                p += 2;

            while( *p && ( *p != '*' || *(p+1) != '/' ) )
            {
                p++;
            }

            if( *p )
            {
                lZustand = 0;
                p += 2;
            }
            else
                break;
        }


        while( *p && ( *p != '/' || ( *(p+1) != '*' && *(p+1) != '/' ) ) )
        {
            p++;
            // tmv "\*****" forces invalid syntax coloring
            if( *p == '"' ) // this is a string in quotes
            {               // so move on to the next quote
               p++;
               while ( *p && *p != '"' )
               {
                  p++;
               }
            }
        }

        if( *p )
        {
            p += 2;

            if( *(p-1) == '*' )
            { //Beginn eines C-Style Kommentars -> Ende suchen
                lZustand = 1;
                while( *p && ( *p != '*' || *(p+1) != '/' ) )
                {
                    p++;
                }
                if( *p )
                {
                    lZustand = 0;
                    p += 2;
                }
            }
            else
            { //Beginn eines C++-Style Kommentars -> Ende suchen
                lZustand = 2;
                while( *p &&  *p != '\r' )
                {
                    p++;
                }
                if( *p )
                {
                    lZustand = 0;
                    p++;
                }
            }
        }
    }
    if( lZustand == 1 )
        return TRUE;
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetSelectionBuffer
**
** .Beschreibung: Bestimmung eines selektierten Bereichs, bzw. des
**                naechsten Zeichens
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird fuer den UNDO-Mechanismus benoetigt. Wenn ein
**      Bereich selektiert ist, dh m_chAlt.cpMin != cpMax, wird dieser
**      Bereich in den neu angelegten Puffer m_tr.lpstrText gelegt.
**      Andernfalls wird das Zeichen an der aktuellen Schreibposition
**      nach m_cNext gebracht.
**
**-----------------------------------------------------------------
*/
void CTBEditor::GetSelectionBuffer()
{
    if( m_chAlt.cpMin != m_chAlt.cpMax )
    {
        m_bSelection = TRUE;

        m_tr.chrg.cpMin = m_chAlt.cpMin;
        m_tr.chrg.cpMax = m_chAlt.cpMax;
        m_tr.lpstrText = m_pUndo->MakeNewBuffer( m_chAlt.cpMin, m_chAlt.cpMax );
        m_tr.lpstrText[ 0 ] = 0;
    }
    else
    {
        m_bSelection = FALSE;

        m_tr.chrg.cpMin = m_chAlt.cpMin;
        m_tr.chrg.cpMax = m_chAlt.cpMin + 1;
        m_tr.lpstrText = m_cNext;
    }

    ::SendMessage( m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &m_tr );
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ReadRTFFormat
**
** .Beschreibung: Lesefunktion des RichEdit-Controls
**
** .Parameter:    DWORD    , dwCookie , I, Zeiger auf CTBEditor-Instanz
**                LPBYTE   , pbBuff   , U, Zielpuffer
**                LONG     , cb       , I, Anzahl zu lesende Zeichen
**                LONG FAR*, pcb      , O, Anzahl uebertragener Zeichen
**
** .Rueckgabewert:   DWORD, 0 -> alles o.k.
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode ist eine statische Callback-Methode, die vom Editstream
**      des Richedits aufgerufen wird, um Daten aus einer Datei ins Control
**      zu uebertragen.
**      Sie wird nur verwendet, wenn Syntax-Pruefung eingeschaltet ist.
**      Sie setzt Plaintext in RichtextFormat um, um das Laden zu be-
**      schleunigen. RTF-Format Spezifikation in MSDN!
**
**      Hinweis: Die Methode wird u.U. mehrmals hintereinander aufgerufen
**      ( Lesen in Bloecken zu 4000 Byte (meist)). Und zwar solange bis
**      der letzte Aufruf 0 als Anzahl der uebertragenen Bytes liefert!
**-----------------------------------------------------------------
*/
/*
DWORD CALLBACK CTBEditor::ReadRTFFormat( DWORD dwCookie, LPBYTE pbBuff,
                                         long cb, long FAR *pcb )
{
   char cSchrift[6] = "\\fs" ;
   char cRed[10] = "\\red" ;
   char cGreen[10] = "\\green" ;
   char cBlue[10] = "\\blue" ;
   char cColorTable[100] = "{\\colortbl" ;
   char cEinleitung[] = "{\\rtf1\\ansi\r\n" ;
   char cEnde[] = "}" ;
   char cZeile[] = "\\par " ;
   char cKommentarFarbe[] = "\\cf2 " ;
   char cNormalFarbe[] = "\\cf0 " ;
   char cKeywordFarbe[] = "\\cf1 " ;
   char cBegrenzer[] = ";" ;
   char cColTabEnde[] = "}\r\n" ;

   char *pDatei;
   char cHilf[20];

   static bEnde = FALSE;
   static bFirst = TRUE;
   static char *pRTF;
   static char *q;

   // beim ersten Aufruf: den RTF-Header aufbauen
   //                     die benoetigten Speicherbereiche allokieren
   //                     das RTF-Parsen initiieren
   if( bFirst )
   {
      BYTE lRed, lGreen, lBlue;

      bFirst = FALSE;

      //Colortable richtig aufbauen -> Normal,Keyword,Kommentar
      lRed = GetRValue( ((CTBEditor *)dwCookie)->m_crForeground );
      lGreen = GetGValue( ((CTBEditor *)dwCookie)->m_crForeground );
      lBlue = GetBValue( ((CTBEditor *)dwCookie)->m_crForeground );

      lstrcat( cColorTable, cRed );
      wsprintf( cHilf, "%d", lRed );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cGreen );
      wsprintf( cHilf, "%d", lGreen );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBlue );
      wsprintf( cHilf, "%d", lBlue );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBegrenzer );


      lRed = GetRValue( ((CTBEditor *)dwCookie)->m_crKeyword );
      lGreen = GetGValue( ((CTBEditor *)dwCookie)->m_crKeyword );
      lBlue = GetBValue( ((CTBEditor *)dwCookie)->m_crKeyword );

      lstrcat( cColorTable, cRed );
      wsprintf( cHilf, "%d", lRed );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cGreen );
      wsprintf( cHilf, "%d", lGreen );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBlue );
      wsprintf( cHilf, "%d", lBlue );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBegrenzer );


      lRed = GetRValue( ((CTBEditor *)dwCookie)->m_crComment );
      lGreen = GetGValue( ((CTBEditor *)dwCookie)->m_crComment );
      lBlue = GetBValue( ((CTBEditor *)dwCookie)->m_crComment );

      lstrcat( cColorTable, cRed );
      wsprintf( cHilf, "%d", lRed );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cGreen );
      wsprintf( cHilf, "%d", lGreen );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBlue );
      wsprintf( cHilf, "%d", lBlue );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBegrenzer );

      lstrcat( cColorTable, cColTabEnde );


      //dann noch Schriftgroesse eintragen
      wsprintf( cHilf, "%d", ((CTBEditor *)dwCookie)->m_FontSize / 10 );
      lstrcat( cSchrift, cHilf );

      //Datei in eigenen Puffer lesen
      DWORD dXLSize;
      DWORD dResult;
      unsigned long lResultat;

      DWORD dSize = GetFileSize( (HANDLE)((CTBEditor *)dwCookie)->m_hFile, &dXLSize );
      pDatei = new char [dSize + 100 ];
      if( ! pDatei )
      {
         *pcb = 0;
         return GetLastError ();
      }
      ZeroMemory( pDatei, dSize+100 );

      dResult = ReadFile( (HANDLE)((CTBEditor *)dwCookie)->m_hFile, pDatei, dSize, &lResultat, NULL );

      if( !dResult || lResultat != dSize )
      {
         *pcb = 0;
         delete [] pDatei;
         return GetLastError ();
      }

      q = pDatei;

      pRTF = new char [(2 * dSize) + 1000 ];
      if( ! pRTF )
      {
         *pcb = 0;
         delete [] pDatei;
         return GetLastError ();
      }
      ZeroMemory( pRTF, (2*dSize) + 1000 );


      //RTF - Kopf aufbauen -> ColorTable und Schriftgroesse festlegen
      lstrcpy( pRTF, cEinleitung );
      lstrcat( pRTF, cColorTable );
      lstrcat( pRTF, cSchrift );
      lstrcat( pRTF, cNormalFarbe );

      //und Parsen
      ((CTBEditor *)dwCookie)->pSyntax->RTFParseAll( pDatei, pRTF + lstrlen( pRTF ),
                             cNormalFarbe, cKeywordFarbe,
                             cKommentarFarbe, cZeile );

      delete [] pDatei;
      q = pRTF;
   }


   //Anforderung des Richedits erfuellen
   if( ! bEnde ) //gibt es noch etwas zu lesen?
   { //ja!
      long lAnz = 0;
      pDatei = q;

      //Anzahl der Bytes bestimmen, die uebertragen werden koennen
      while( *q && lAnz < cb )
      {
         q++;
         lAnz++;
      }

      memcpy( pbBuff, pDatei, lAnz );
      *pcb = lAnz;

      if( lAnz < cb )
         bEnde = TRUE;
   }
   else
   { //nein -> neu initialisieren
      *pcb = 0;
      delete [] pRTF;
      bEnde = FALSE;
      bFirst = TRUE;
   }
   return( 0 );
}
*/

DWORD CALLBACK CTBEditor::ReadRTFFormat( DWORD dwCookie, LPBYTE pbBuff,
                                         long cb, long FAR *pcb )
{
   char cSchrift[6] = "\\fs" ;
   char cRed[10] = "\\red" ;
   char cGreen[10] = "\\green" ;
   char cBlue[10] = "\\blue" ;
   char cColorTable[100] = "{\\colortbl" ;
   char cEinleitung[] = "{\\rtf1\\ansi\r\n" ;
   char cEnde[] = "}" ;
   char cZeile[] = "\\par " ;
   char cKommentarFarbe[] = "\\cf2 " ;
   char cNormalFarbe[] = "\\cf0 " ;
   char cKeywordFarbe[] = "\\cf1 " ;
   char cBegrenzer[] = ";" ;
   char cColTabEnde[] = "}\r\n" ;

   char *pDatei;
   char cHilf[20];

   static int bEnde = FALSE;
   static int bFirst = TRUE;
   static char *pRTF;
   static char *q;

   // beim ersten Aufruf: den RTF-Header aufbauen
   //                     die benoetigten Speicherbereiche allokieren
   //                     das RTF-Parsen initiieren
   if( bFirst )
   {
      BYTE lRed, lGreen, lBlue;

      bFirst = FALSE;

      //Colortable richtig aufbauen -> Normal,Keyword,Kommentar
      lRed = GetRValue( ((CTBEditor *)dwCookie)->m_crForeground );
      lGreen = GetGValue( ((CTBEditor *)dwCookie)->m_crForeground );
      lBlue = GetBValue( ((CTBEditor *)dwCookie)->m_crForeground );

      lstrcat( cColorTable, cRed );
      wsprintf( cHilf, "%d", lRed );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cGreen );
      wsprintf( cHilf, "%d", lGreen );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBlue );
      wsprintf( cHilf, "%d", lBlue );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBegrenzer );


      lRed = GetRValue( ((CTBEditor *)dwCookie)->m_crKeyword );
      lGreen = GetGValue( ((CTBEditor *)dwCookie)->m_crKeyword );
      lBlue = GetBValue( ((CTBEditor *)dwCookie)->m_crKeyword );

      lstrcat( cColorTable, cRed );
      wsprintf( cHilf, "%d", lRed );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cGreen );
      wsprintf( cHilf, "%d", lGreen );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBlue );
      wsprintf( cHilf, "%d", lBlue );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBegrenzer );


      lRed = GetRValue( ((CTBEditor *)dwCookie)->m_crComment );
      lGreen = GetGValue( ((CTBEditor *)dwCookie)->m_crComment );
      lBlue = GetBValue( ((CTBEditor *)dwCookie)->m_crComment );

      lstrcat( cColorTable, cRed );
      wsprintf( cHilf, "%d", lRed );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cGreen );
      wsprintf( cHilf, "%d", lGreen );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBlue );
      wsprintf( cHilf, "%d", lBlue );
      lstrcat( cColorTable, cHilf );

      lstrcat( cColorTable, cBegrenzer );

      lstrcat( cColorTable, cColTabEnde );


      //dann noch Schriftgroesse eintragen
      wsprintf( cHilf, "%d", ((CTBEditor *)dwCookie)->m_FontSize / 10 );
      lstrcat( cSchrift, cHilf );

      //Datei in eigenen Puffer lesen
      DWORD dXLSize;
      DWORD dResult;
      long lResultat;

      DWORD dSize = GetFileSize( (HANDLE)((CTBEditor *)dwCookie)->m_hFile, &dXLSize );
      // assuming all chars in the file are tabs we need dSize * m_lTabstops as buffersize
      dSize = dSize * m_lTabStop;
      pDatei = new char [dSize + 100 ];
      if( ! pDatei )
      {
         *pcb = 0;
         return GetLastError ();
      }
      ZeroMemory( pDatei, dSize+100 );

      // now read bytewise and replace tabstops
      dResult = ReadCallback( ((CTBEditor *)dwCookie)->m_hFile, (LPBYTE)pDatei, dSize, &lResultat );

      dSize = lResultat;

      if( dResult > 0 )
      {
         *pcb = 0;
         delete [] pDatei;
         return GetLastError ();
      }

      q = pDatei;

      pRTF = new char [(2 * dSize) + 1000 ];
      if( ! pRTF )
      {
         *pcb = 0;
         delete [] pDatei;
         return GetLastError ();
      }
      ZeroMemory( pRTF, (2*dSize) + 1000 );


      //RTF - Kopf aufbauen -> ColorTable und Schriftgroesse festlegen
      lstrcpy( pRTF, cEinleitung );
      lstrcat( pRTF, cColorTable );
      lstrcat( pRTF, cSchrift );
      lstrcat( pRTF, cNormalFarbe );

      //und Parsen
      ((CTBEditor *)dwCookie)->m_pSyntax->RTFParseAll( pDatei, pRTF + lstrlen( pRTF ),
                             cNormalFarbe, cKeywordFarbe,
                             cKommentarFarbe, cZeile );

      delete [] pDatei;
      q = pRTF;
   }


   //Anforderung des Richedits erfuellen
   if( ! bEnde ) //gibt es noch etwas zu lesen?
   { //ja!
      long lAnz = 0;
      pDatei = q;

      //Anzahl der Bytes bestimmen, die uebertragen werden koennen
      while( *q && lAnz < cb )
      {
         q++;
         lAnz++;
      }

      memcpy( pbBuff, pDatei, lAnz );
      *pcb = lAnz;

      if( lAnz < cb )
         bEnde = TRUE;
   }
   else
   { //nein -> neu initialisieren
      *pcb = 0;
      delete [] pRTF;
      bEnde = FALSE;
      bFirst = TRUE;
   }
   return( 0 );
}

