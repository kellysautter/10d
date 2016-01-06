/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditor
**
** .Version:       01N
**
** .Aenderung:     16.11.2000
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
** CHANGE LOG
**
** 2002.08.22   DKS   Z10
**    When sending a message for EM_GETTEXTRANGE, if the range is zero,
**    the return buffer is not set to null.  So we do this explicitly.
** 2001.02.12   BL    R54305
**    if Dialog not check out, do not insert a new line
** 1999.11.04   DKS   Z10   Cosmetic (so Brief can read it).
** 01A  =  Neuerstellung
**         (18.05.1998)  SL
** 01B  =  Erweiterung um Methode DeleteTextRange
**         (02.07.1998)  SL
** 01C  =  Erweiterung um die Methoden
**         GetTextFromLineOfIndex
**         SetSironLanguage
**         GetPositionByIndex
**         IsCommentAtIndex
**         (27.08.1998)  TMV
** 01D  =  Bug in FindTextPosition verbessert
**         (11.09.1998)  SL
** 01E  =  Methode SelectItem erweitert
**         Methode SetWrappingOn verbessert mit SelectObject
**         (27.10.1998)  SL
** 01F  =  Methode DeleteText gibt nun long zurueck
**         (18.12.1998)  SL, TMV
** 01G  =  InsertItem() rueckt nicht mehr automatisch ein.
**         (02.03.1999)  SL
** 01H  =  Neue Interface-Methoden als Ersatz fuer Menue-
**         ansteuerung
**         (22.03.1999)  SL
** 01I  =  Siron spezifische Funktionen auskommentiert
**         (1999.11.04)  TMV
** 01J  =  Neue Funktion eingefügt
**         SelectRange
**         (2000.03.15)  TMV
** 01K  =  SelectRange check minimum und maximum werte für CHARRANGE
**         (2000.10.17)  TMV
** 01L  =  replace TABS in call of InsertItem and during paste operations via clipboard.
**         Therefore implement new method: CTBEditor::InsertText( LPCSTR pDaten )
**         (2000.10.31)  TMV
** 01M  =  remove membervariable m_bReturn
**         (2000.11.2)  TMV
** 01N  =  change ZeidonSyntaxOn to avoid obsolete calls of
**            SendMessage( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &m_cf );
**          and
**            SetKeywords();
**         they are realy time consuming
**         (2000.11.16)  TMV
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
#include "hzoregex.h"
#include "hzoedlg.h"
#include "hzoeprop.h"

extern CTBEditApp NEAR theApp;


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      DeleteText
**
** .Beschreibung: Loeschen von selektierten Text
**
** .Parameter:
**
** .Rueckgabewert: long , Laenge des geloeschten Bereichs
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
long CTBEditor::DeleteText()
{
    ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &m_chAlt );

    GetSelectionBuffer();

    ::SendMessage( m_hWnd, WM_CLEAR, 0, 0L );

    FireSetLine();
    m_bPosChanged = TRUE;

    if( m_bSelection )
        m_pUndo->DeleteRange( m_chAlt.cpMin, m_tr.lpstrText );

    TestInput();

    return ( m_chAlt.cpMax - m_chAlt.cpMin );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      CutText
**
** .Beschreibung: Loeschen von selektierten Text und kopieren in die
**                Zwischenablage
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
long CTBEditor::CutText()
{
    long lErg = 0;

    if( ! m_hWnd )
        return lErg;

    ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &m_chAlt );

    lErg = m_chAlt.cpMax - m_chAlt.cpMin;

    GetSelectionBuffer();

   SendMessage( WM_CUT, 0, 0L );

    FireSetLine();
    m_bPosChanged = TRUE;

    if( m_bSelection )
        m_pUndo->DeleteRange( m_chAlt.cpMin, m_tr.lpstrText );

    TestInput();

    return lErg;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      CopyText
**
** .Beschreibung: kopieren von selektierten Text in die Zwischenablage
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
long CTBEditor::CopyText()
{
    long lErg = 0;

    if( m_hWnd )
    {
        CHARRANGE cha;

        ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &cha );
        lErg = cha.cpMax - cha.cpMin;
        ::SendMessage( m_hWnd, WM_COPY, 0, 0L );
    }
    return lErg;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      PasteText
**
** .Beschreibung: Einfuegen von Text aus der Zwischenablage
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
long CTBEditor::PasteText()
{

   if( ! m_hWnd )
      return( 0 );

   if ( !::OpenClipboard( 0 ) )
      return( 0 );

   HANDLE hClipboardData = ::GetClipboardData( CF_TEXT );
   if ( !hClipboardData )
   {
      ::CloseClipboard();
      return( 0 );
   }

   LPCSTR pDaten = (LPCSTR)::GlobalLock( hClipboardData );

   if ( !pDaten )
   {
      ::CloseClipboard();
      return( 0 );
   }

   long lReturn = InsertText( pDaten );

   ::GlobalUnlock( hClipboardData );
   ::CloseClipboard();

   return lReturn;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SyntaxOff
**
** .Beschreibung: Ausschalten der Syntax - Bearbeitung
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode schaltet die Syntaxbearbeitung aus ( "aus" ist
**      Standard ). Eventuell vorhandene Syntaxobjekte werden
**      zerstoert.
**-----------------------------------------------------------------
*/
void CTBEditor::SyntaxOff()
{
    BOOL bModify;

    if( ! m_hWnd )
        return;

    bModify = SendMessage( EM_GETMODIFY, 0, 0L );

    if( m_enSyntax != NO_SYNTAX )
    {
        if( m_pSyntax )
        {
            delete m_pSyntax;
            m_pSyntax = NULL;
        }
    }

    m_enSyntax = NO_SYNTAX;

    //alles neu einfaerben
    m_cf.crTextColor = m_crForeground;
    SendMessage( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &m_cf );

    FireSetLine();

    if( ! bModify ) //keine Aenderung
        SendMessage( EM_SETMODIFY, FALSE, 0L );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SironSyntaxOn
**
** .Beschreibung: Einschalten der Siron Syntax
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
/*
void CTBEditor::SironSyntaxOn()
{
    BOOL bModify;

    return; // siehe Änderungsvermerk 01L

    if( ! m_hWnd )
        return;

    bModify = SendMessage( EM_GETMODIFY, 0, 0L );

    if( m_enSyntax != SIRON_SYNTAX )
    {
        m_enSyntax = SIRON_SYNTAX;

        if( m_pSyntax )
            delete (ZeidonSyntax*)m_pSyntax;
        m_pSyntax = new SironSyntax( this, m_pLineBuf, m_hWnd, m_bGerman );
    }

    //zuerst grunddarstellung
    m_cf.crTextColor = m_crForeground;
    SendMessage( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &m_cf );

    SetKeywords(); //dann neue Faerbung

    if( ! bModify ) //keine Aenderung
        SendMessage( EM_SETMODIFY, FALSE, 0L );
}
*/
/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SetSironLanguage
**
** .Beschreibung: Umschalten in eine andere Sprachversion
**
**
** .Parameter:    long , lLanguage   , I, Sprachkennzeichen
**
** .Rueckgabewert:long, Kennzeichen der gesetzten Sparche
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**       Umschalten in eine andere Sprachversion.
**       gültige werte sind ....
**-----------------------------------------------------------------
*/
/*
long CTBEditor::SetSironLanguage( long lLanguage )
{
    if( lLanguage == 1 )
        m_bGerman = TRUE;
    else
        m_bGerman = FALSE;
    return m_bGerman;
}
*/
/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ZeidonSyntaxOn
**
** .Beschreibung: Zeidon Syntaxbearbeitung einschalten
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Im Unterschied zur Siron-Bearbeitung schaltet die Zeidonbearbeitung
**      auch das Keystroke-Event ein.
**
**-----------------------------------------------------------------
*/
void CTBEditor::ZeidonSyntaxOn()
{
   BOOL bModify;

   if( ! m_hWnd )
       return;

   m_bWantKeys = TRUE; // auf jeden Fall KeystrokeEvent

   bModify = SendMessage( EM_GETMODIFY, 0, 0L );

   if( m_enSyntax != ZEIDON_SYNTAX )
   {
      m_enSyntax = ZEIDON_SYNTAX;

//    if( m_pSyntax )
//       delete (SironSyntax*)m_pSyntax;
      m_pSyntax = new ZeidonSyntax( this, m_pLineBuf, m_hWnd );

      if ( m_pSyntax )
      {
         m_cf.crTextColor = m_crForeground;
         SendMessage( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &m_cf );

         SetKeywords();
      }
   }

   if( ! bModify ) //keine Aenderung
       SendMessage( EM_SETMODIFY, FALSE, 0L );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      OpenObject
**
** .Beschreibung: Oeffnet eine neue Datei zur Bearbeitung
**
** .Parameter:    BSTR*, pFileName, I, Name der Datei
**
** .Rueckgabewert:  BOOL, TRUE bei Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Enthaelt das Edit schon Text, wird gefragt ob dieser gespeichert
**      werden soll. Die Datei wird mit Win API geoeffnet und dann ueber
**      den Stream-Mechanismus des RichEdits eingelesen.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::OpenObject(BSTR FAR* pFileName)
{
    USES_CONVERSION;

    OFSTRUCT of;
    EDITSTREAM eStream;

    m_pUndo->Initialize();

    if( SendMessage( EM_GETMODIFY, 0, 0L ) )
    {
        if( DialogMeldung( IDS_SAVE, MB_ICONQUESTION | MB_YESNO )
              == IDYES )
        {
            OFSTRUCT of;
            EDITSTREAM eStream;

            if( ( m_hFile = OpenFile( m_cFileName, &of, OF_CREATE ) ) != HFILE_ERROR )
            {
                eStream.dwCookie = m_hFile;
                eStream.dwError = 0;
                eStream.pfnCallback = SaveCallback;
                SendMessage( EM_STREAMOUT, (WPARAM)SF_TEXT, (LPARAM)&eStream);
                CloseHandle((HANDLE) m_hFile);
            }
            else
            {
                DialogMeldung( IDS_CLOSE, MB_ICONEXCLAMATION | MB_OK );
                return FALSE;
            }
        }
    }

    if( pFileName )
    {
        LPSTR pSTR = W2A( *pFileName );
        lstrcpy( m_cFileName, pSTR );

        if( ( m_hFile = OpenFile( m_cFileName, &of, OF_READ) ) != HFILE_ERROR )
        {
            if( m_enSyntax == NO_SYNTAX )
            {
                eStream.dwCookie = m_hFile;
                eStream.pfnCallback = ReadCallback;
                eStream.dwError = 0;
                SendMessage(  EM_STREAMIN, (WPARAM)SF_TEXT, (LPARAM)&eStream);
            }
            else
            { //wir wollen direkt in RTF-Format mit Coloring umwandeln
              //deshalb muss der Instanz-Pointer mitgegeben werden
              // (!! Gott sei dank wird das DIRTY-BIT wieder zurueckgesetzt !! )
                eStream.dwCookie = (unsigned long)this;
                eStream.pfnCallback = ReadRTFFormat;
                eStream.dwError = 0;
                SendMessage(  EM_STREAMIN, (WPARAM)SF_RTF, (LPARAM)&eStream);
            }

            // Reset the dirty bit.
            SendMessage( EM_SETMODIFY, (WPARAM)FALSE, 0L);

            CloseHandle( (HANDLE) m_hFile);
            SetFocus();
            return TRUE;
        }
        else
            DialogMeldung( IDS_OPEN, MB_ICONEXCLAMATION | MB_OK );
    }

   return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      CloseObject
**
** .Beschreibung: Bearbeitung einer Datei beenden
**
** .Parameter:
**
** .Rueckgabewert:  BOOL, TRUE bei Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      "Vergisst" den kompletten Inhalt des RichEdits. Alle Aenderungen
**      gehen ohne Warnung verloren.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::CloseObject()
{
    InitializeFileName();

//  ::SendMessage( m_hWnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"");
// funktioniert unter MFC irgendwie nicht mehr !!??

    CHARRANGE cha;
    cha.cpMin = 0;
    cha.cpMax = -1;

    SendMessage( EM_HIDESELECTION, TRUE, 0L );
    SendMessage( EM_EXSETSEL, 0, (LPARAM) &cha );
   SendMessage( WM_CLEAR, 0, 0L );
    SendMessage( EM_HIDESELECTION, FALSE, 0L );

    m_pUndo->Initialize();

    SendMessage( EM_SETMODIFY, (WPARAM)FALSE, 0L );
    return TRUE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      PropertyDialog
**
** .Beschreibung: Anzeigen des Property Dialogs
**
** .Parameter:
**
** .Rueckgabewert:  BOOL, TRUE -> Aenderungen wurden durchgefuehrt
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Siron-Methode, die den gesamten Text in Grossbuchstaben umwandelt.
**      Ausnahme stellen Sironstrings dar, die unveraendert bleiben.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::PropertyDialog()
{
    BOOL bModify;
    BOOL bErg = FALSE;

    if( ! m_hWnd )
        return bErg;

    bModify = SendMessage( EM_GETMODIFY, 0, 0L );


    CPropertyDlg PropDlg( this );

    //Vorbelegungen
    PropDlg.m_crForeground = m_crForeground;
    PropDlg.m_crBackground = m_crBackground;
    PropDlg.m_crComment = m_crComment;
    PropDlg.m_crKeyword = m_crKeyword;
    PropDlg.m_lTabStop = m_lTabStop;

    if( PropDlg.DoModal() == TRUE )
    {
        if( PropDlg.m_lTabStop != m_lTabStop )  //TabStop geaendert?
        {
            m_lTabStop = PropDlg.m_lTabStop;
            SetTabStops( m_lTabStop );
        }

        m_crForeground = PropDlg.m_crForeground;
        m_crBackground = PropDlg.m_crBackground;
        m_crComment = PropDlg.m_crComment;
        m_crKeyword = PropDlg.m_crKeyword;

        SendMessage( EM_SETBKGNDCOLOR, FALSE, m_crBackground );
        m_cf.crTextColor = m_crForeground;
        SendMessage( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &m_cf );

        bErg = TRUE;

      if( PropDlg.m_bFontChange )
      {
         //neuen Font anlegen
         if( m_hFont )
            DeleteObject( m_hFont );

         if( m_hFont = CreateFontIndirect(& PropDlg.m_lf) )
         {   //wenn Syntax-coloring -> komplett neu darstellen
               SendMessage( WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM( FALSE, 0 ) );
               PropDlg.m_cf.crTextColor = m_crForeground; // nochmals die richtige Farbe setzen
               SendMessage( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &PropDlg.m_cf );
         }
         lstrcpy( m_FontName, PropDlg.m_lf.lfFaceName ); // Fontname und
            m_FontSize = PropDlg.m_FontSize;                // Fontgroesse merken
      }

      if( m_enSyntax != NO_SYNTAX )
         SetKeywords();

        if( PropDlg.m_bWantSave ) // Einstellungen sollen gespeichert werden
            SaveOptions();
   }

    SetFocus();

    if( ! bModify ) //keine Aenderung
        SendMessage( EM_SETMODIFY, FALSE, 0L );

    return bErg;
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetCursorPosition
**
** .Beschreibung: aktuelle Cursorposition abfragen
**
** .Parameter:    long *,  plLine   , O, zero-based Line Index
**                long *,  plSpalte , O, zero-based Spalten Index
**                long *,  plIndex  , O, zero-based Character Index
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Liefert die Position zweifach:
**          - Zeile/Spalte
**          - absoluter Character Index vom Anfang des Texts
**-----------------------------------------------------------------
*/
void CTBEditor::GetCursorPosition(long FAR* plLine, long FAR* plSpalte,
                                  long FAR* plIndex)
{
    if( !plLine || !plSpalte || ! plIndex )
        return;

    CHARRANGE cha;

    if( m_hWnd )
    {
        SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
        *plIndex = cha.cpMin;

/*      redundanter code
        *plLine = SendMessage( EM_EXLINEFROMCHAR, 0,(LPARAM) cha.cpMin );

        DWORD dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)*plLine, (LPARAM) 0 );
        *plSpalte = cha.cpMin - dCharIndex;
*/
        GetPositionByIndex( cha.cpMin, plLine, plSpalte);
    }
    else
    {
        *plLine = 0;
        *plSpalte = 0;
        *plIndex = 0;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetPositionByIndex
**
** .Beschreibung: Zeile und Spalte zu einem Index
**
** .Parameter:    long  ,  lIndex   , O, zero-based Character Index
**                long *,  plLine   , O, zero-based Line Index
**                long *,  plSpalte , O, zero-based Spalten Index
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Liefert Zeile und Spalte zu einer gegebenen Position
**-----------------------------------------------------------------
*/
void CTBEditor::GetPositionByIndex(long lIndex, long FAR* plLine, long FAR* plSpalte)
{
    DWORD dIndex = DWORD(lIndex);

   if( !plLine || !plSpalte )
        return;

    if( m_hWnd )
    {

        *plLine = SendMessage( EM_EXLINEFROMCHAR, 0,(LPARAM)dIndex );
        *plSpalte = SendMessage( EM_LINEINDEX, (WPARAM)*plLine, (LPARAM) 0 );
        *plSpalte = lIndex - *plSpalte;
    }
    else
    {
        *plLine = 0;
        *plSpalte = 0;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetLineCount
**
** .Beschreibung: Anzahl der Zeilen abfragen
**
** .Parameter:    long *, pLLines, O, Anzahl der Zeilen ( ab 1 )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
void CTBEditor::GetLineCount(long FAR* plLines)
{
    if( m_hWnd )
        *plLines = SendMessage( EM_GETLINECOUNT );
    else
        *plLines = 0;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      InsertItem
**
** .Beschreibung: Einfuegen eines Textbausteins an akt. Position
**
** .Parameter:    BSTR *, pDaten, I, Pointer auf einzufuegenden Text
**
** .Rueckgabewert:  long, Erfolg -> zero-based Character Index der akt Position
**                        Fehler -> -1
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode fügt Text an der akt. Position ein und startet dann ein
**      Syntax Coloring.
**-----------------------------------------------------------------
*/
long CTBEditor::InsertItem(BSTR FAR* pDaten )
{
   USES_CONVERSION;

   if( ! m_hWnd )
      return( -1 );

   if( !pDaten )
      return( -1 );

//   m_bReturn = FALSE;

   LPSTR pSTR = W2A( *pDaten );

   return InsertText( pSTR );
}


long CTBEditor::InsertText( LPCSTR pDaten )
{
     // replace tabs by number of blanks
   LPSTR p = NULL;
   LPSTR pInsertString = new char [ lstrlen( pDaten ) * m_lTabStop + 1];
   memset( pInsertString, 0, lstrlen( pDaten ) * m_lTabStop + 1 );

   LPSTR pSource = (LPSTR)pDaten;
   LPSTR pDest   = pInsertString;
   while ( *pSource )
   {
      if( *pSource == '\t' )
      {
         for ( int i = 0 ; i < m_lTabStop; i++, pDest++ )
         {
           *pDest = ' ';
         }
      }
      else
      {
         *pDest = *pSource;
         pDest++;
      }
      pSource++;
   }

   ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &m_chAlt );

   if ( m_bReadOnly )
      return m_chAlt.cpMax;

   //UNDO-Stuff first
   GetSelectionBuffer();
   p = m_pUndo->MakeNewBuffer( pInsertString );

   delete [] pInsertString;

   if( m_bSelection )
     m_pUndo->InsertRange( m_chAlt.cpMin, p, m_tr.lpstrText );
   else
     m_pUndo->InsertRange( m_chAlt.cpMin, p );


   ::SendMessage( m_hWnd, EM_REPLACESEL, FALSE,(LPARAM) p );

   TestInput();

   FireSetLine();
   return m_chAlt.cpMin;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      IsObjectChanged
**
** .Beschreibung: Fragen, ob Inhalt des RichEdits veraendert wurde
**
** .Parameter:    BOOL *, pbChanged, O, Anzeigen der Veraenderung
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      pbChanged wird auf TRUE gesetzt, wenn der Inhalt veraendert wurde.
**      Andernfalls FALSE.
**
**-----------------------------------------------------------------
*/
void CTBEditor::IsObjectChanged(BOOL FAR* pbChanged)
{
    if( ! m_hWnd )
    {
        *pbChanged = FALSE;
        return;
    }

    if( SendMessage( EM_GETMODIFY, 0, 0L ) )
        *pbChanged = TRUE;
    else
        *pbChanged = FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      PrintObject
**
** .Beschreibung: Drucken von Text
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode druckt den kompletten Text oder nur einen markierten
**      Bereich. In Abhaengigkeit von einer vorhandenen Selektion wird
**      im Druckdialog ( CommonDialog ) die entsprechende Auswahlmoeglichkeit
**      angeboten.
**-----------------------------------------------------------------
*/
void CTBEditor::PrintObject()
{
   long cchText;
   POINT pt;
   DOCINFO di;
   FORMATRANGE fr;
   RECT rc;
    PRINTDLG pd;
    CHARRANGE cha;

    if( ! m_hWnd )
        return;

    //Wenn etwas markiert ist -> Selektion erlauben
    DWORD dSel = SendMessage( EM_SELECTIONTYPE );
    if( dSel & SEL_MULTICHAR )
    {
        SendMessage( EM_EXGETSEL, 0, (LPARAM) &cha);
        pd.Flags = 0;
    }
    else
    {
        pd.Flags = PD_NOSELECTION;
    }

   cchText = SendMessage( WM_GETTEXTLENGTH, 0, 0);

    // Initialize the PRINTDLG structure.
    pd.lStructSize = sizeof(PRINTDLG);
    pd.hwndOwner = m_hWnd;
    pd.hDevMode = (HANDLE)NULL;
    pd.hDevNames = (HANDLE)NULL;
    pd.nFromPage = 0;
    pd.nToPage = 0;
    pd.nMinPage = 0;
    pd.nMaxPage = 0;
    pd.nCopies = 1;
    pd.hInstance = theApp.m_hInstance;
    pd.lpfnSetupHook = (LPSETUPHOOKPROC)(FARPROC)NULL;
    pd.lpSetupTemplateName = NULL;
    pd.lpfnPrintHook = (LPPRINTHOOKPROC)(FARPROC)NULL;
    pd.lpPrintTemplateName = NULL;
   pd.Flags = pd.Flags | PD_NOPAGENUMS |
            PD_USEDEVMODECOPIES | PD_COLLATE | PD_RETURNDC;

   if(!PrintDlg(&pd))
      return;

   BeginWaitCursor();

   di.cbSize = sizeof(DOCINFO);
   di.lpszDocName = m_cFileName;
   di.lpszOutput = NULL;

   fr.hdc = pd.hDC;
   fr.hdcTarget = 0;

   if(Escape(fr.hdc, GETPHYSPAGESIZE, 0, NULL, &pt) > 0)
   {
      const LONG xPerInch = GetDeviceCaps(fr.hdc, LOGPIXELSX);
      const LONG yPerInch = GetDeviceCaps(fr.hdc, LOGPIXELSY);

      rc.left = rc.top = 0;
      fr.rcPage.left = fr.rcPage.top = 0;
      rc.right = (INT) ((pt.x * 1440l) / xPerInch);
      fr.rcPage.right = rc.right;
      // leave 1.25" (1800 twips) margins if that will leave >= 1"
      if(rc.right >= 1800 + 1440 + 1800)
         rc.right -= (rc.left = 1800);
      rc.bottom = (INT) ((pt.y * 1440l) / yPerInch);
      fr.rcPage.bottom = rc.bottom;
      // leave 1" (1440 twips) margins if that will leave >= 1"
      if(rc.bottom >= 1440 + 1440 + 1440)
         rc.bottom -= (rc.top = 1440);
   }
   else
   {
      const LONG xPerInch = GetDeviceCaps(fr.hdc, LOGPIXELSX);
      const LONG yPerInch = GetDeviceCaps(fr.hdc, LOGPIXELSY);

      rc.left = rc.top = 0;
      rc.right = (INT) ((GetDeviceCaps(fr.hdc, HORZRES) * 1440l) /
                        xPerInch);
      rc.bottom = (INT) ((GetDeviceCaps(fr.hdc, VERTRES) * 1440l) /
                        yPerInch);
      fr.rcPage = rc;
   }

   if(StartDoc(fr.hdc, &di) <= 0)
      goto err;

   // tell RichEdit not to erase before rendering text
   SetBkMode(fr.hdc, TRANSPARENT);

    if( pd.Flags & PD_SELECTION )
    { //nur die Markierung drucken
        fr.chrg = cha;
    }
    else
    {
       fr.chrg.cpMin = 0;
       fr.chrg.cpMax = cchText;
    }

   do
   {
      if(StartPage(fr.hdc) <= 0)
         goto abort;
      fr.rc = rc;
      fr.chrg.cpMin = SendMessage( EM_FORMATRANGE,
                     (WPARAM) TRUE, (LPARAM) &fr);
      if(EndPage(fr.hdc) <= 0)
         goto abort;
   } while(fr.chrg.cpMin > 0 && fr.chrg.cpMin < fr.chrg.cpMax);

   if(fr.chrg.cpMin >= 0 && EndDoc(fr.hdc) > 0)
   {
      fr.chrg.cpMin = fr.chrg.cpMax = cchText;
      // free up RichEdit's format range info
      SendMessage( EM_FORMATRANGE, 0, 0);
      EndWaitCursor();
        DeleteDC( fr.hdc );
      return;
   }

abort:
   (void) AbortDoc(fr.hdc);

err:
   fr.chrg.cpMin = fr.chrg.cpMax = cchText;
   // free up RichEdit's format range info
   SendMessage( EM_FORMATRANGE, 0, 0);
   EndWaitCursor();
    DeleteDC( fr.hdc );
    DialogMeldung( IDS_PRINT_FAIL, MB_ICONSTOP | MB_OK );
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ReadOnlyModus
**
** .Beschreibung: Togglen des ReadOnly Modus
**
** .Parameter:    BOOL, bReadOnly, I, Einschalten ( TRUE ) und Ausschalten
**                                    des Modus
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
void CTBEditor::ReadOnlyModus(BOOL bReadOnly)
{
    if( m_hWnd )
    {
        if( bReadOnly )
        {

            m_bReadOnly = TRUE;
            SendMessage( EM_SETOPTIONS, (WPARAM) ECOOP_OR, (LPARAM) ECO_READONLY );
        }
        else
        { // ReadOnly-Flag auf 0 setzen
            DWORD fOpts = 0xFFFFFFFF;
            fOpts ^= ECO_READONLY;
            SendMessage( EM_SETOPTIONS, (WPARAM) ECOOP_AND, (LPARAM) fOpts );
            m_bReadOnly = FALSE;
        }
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SaveObject
**
** .Beschreibung: Speichern der editierten Datei
**
** .Parameter:
**
** .Rueckgabewert:  BOOL , TRUE bei Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode speichert den Text ueber den Stream-Mechanismus des
**      RichEdits.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::SaveObject()
{
    OFSTRUCT of;
    EDITSTREAM eStream;

    if( (m_hFile = OpenFile( m_cFileName, &of, OF_CREATE )) != HFILE_ERROR )
    {
        eStream.dwCookie = m_hFile;
        eStream.dwError = 0;
        eStream.pfnCallback = SaveCallback;
        SendMessage( EM_STREAMOUT, (WPARAM)SF_TEXT, (LPARAM)&eStream);

        CloseHandle( (HANDLE) m_hFile);
        SendMessage( EM_SETMODIFY, (WPARAM)FALSE, 0L);
        SetFocus();
        return TRUE;
    }
    else
    {
        DialogMeldung( IDS_CLOSE, MB_ICONEXCLAMATION | MB_OK );
        SetFocus();
        return FALSE;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SaveObjectAs
**
** .Beschreibung: Speichern als...
**
** .Parameter:    BSTR*, pFileName, I, neuer Name der Datei
**
** .Rueckgabewert:  BOOL, TRUE bei Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode speichert den Text des RichEdits unter einen neuen
**      Namen. Dieser wird nun als interner Dateinamen verwendet (m_cFileName).
**-----------------------------------------------------------------
*/
BOOL CTBEditor::SaveObjectAs(BSTR FAR* pFileName)
{
    BOOL bErg = FALSE;
    USES_CONVERSION;

    if( ! m_hWnd )
        return bErg;

    if( pFileName )
    {
        LPSTR pSTR = W2A( *pFileName );
        lstrcpy( m_cFileName, pSTR );

        OFSTRUCT of;
        EDITSTREAM eStream;

        if( (m_hFile = OpenFile( m_cFileName, &of, OF_CREATE ) ) != HFILE_ERROR )
        {
            eStream.dwCookie = m_hFile;
            eStream.dwError = 0;
            eStream.pfnCallback = SaveCallback;
            SendMessage( EM_STREAMOUT, (WPARAM)SF_TEXT, (LPARAM)&eStream);

            CloseHandle( (HANDLE) m_hFile);
            SendMessage( EM_SETMODIFY, (WPARAM)FALSE, 0L);
            bErg = TRUE;
        }
        else
        {
            DialogMeldung( IDS_CLOSE, MB_ICONEXCLAMATION | MB_OK );
            bErg = FALSE;
        }
    }
    SetFocus();
    return bErg;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SelectItem
**
** .Beschreibung: Markieren eines Textbereichs
**
** .Parameter:    long , nLine    , I, Zeilennummer ab 1
**                BSTR*, pDaten   , I, zu markierender Text
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode markiert ein Wort in der angegebenen Zeile. Falls
**      pDaten NULL ist, wird die ganze Zeile markiert.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::SelectItem(long nLine, BSTR FAR* pDaten)
{
    USES_CONVERSION;

    if( !m_hWnd )
        return FALSE;

    DWORD dLine = SendMessage( EM_GETLINECOUNT );
    if( dLine < (unsigned long)nLine )
    {
        MessageBeep(0);
        return FALSE;
    }


    DWORD dAnfang = ScrollToLine( nLine );

    //bestimme die Laenge der Zeile
    DWORD dLaenge = SendMessage( EM_LINELENGTH, (WPARAM)dAnfang, 0 );


    if( ! pDaten ) //d.h. ganze Zeile markieren
    {
        CHARRANGE cha;

        cha.cpMin = dAnfang;
        cha.cpMax = dAnfang + dLaenge;

        SendMessage( EM_EXSETSEL, 0, (LPARAM)&cha );

    }
    else //genau das angegebene Wort markieren
    {
        LPSTR pSTR = W2A( *pDaten );

        FINDTEXTEX ft;
        ft.lpstrText = pSTR;
        ft.chrg.cpMin = dAnfang;
        ft.chrg.cpMax = dAnfang + dLaenge;

        if( SendMessage( EM_FINDTEXTEX, FR_MATCHCASE | FR_WHOLEWORD ,
                         (LPARAM) &ft ) == -1 )
        {
            MessageBeep(0);
        }
        else
            SendMessage( EM_EXSETSEL, 0, (LPARAM)&ft.chrgText );

    }

    SendMessage( EM_HIDESELECTION, 0, 0 );
    FireSetLine();
    SetFocus();

    return TRUE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SelectRange
**
** .Beschreibung: Markieren eines Textbereichs
**
** .Parameter:    long , nIndex   , I, position ab der zu markieren ist
**                long , nLength  , I, länge der markierung
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode markiert einen Bereich ab der gegebenen Position.
** ---------------------------------------------------------------------
*/

BOOL CTBEditor::SelectRange( long nIndex, long nLength )
{

   CHARRANGE cha;
   cha.cpMin = __min ( nIndex, nIndex + nLength );
   cha.cpMax = __max ( nIndex, nIndex + nLength );

   SendMessage( EM_EXSETSEL, 0, (LPARAM)&cha );

   SendMessage( EM_HIDESELECTION, 0, 0 );
   FireSetLine();
   SetFocus();

   return TRUE;
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SetCursorPositionByIndex
**
** .Beschreibung: Cursor setzen als zero-based character Index
**
** .Parameter:    long, lIndex, I, neue Index Position
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode positioniert den Cursor an die angegebene Position
**      und scrolled an diese Stelle.
**-----------------------------------------------------------------
*/
void CTBEditor::SetCursorPositionByIndex(long lIndex)
{
    CHARRANGE cha;
    cha.cpMin = cha.cpMax = lIndex;

    if( m_hWnd )
    {
        DWORD dLineIndex;

        dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, lIndex );
        ScrollToLine( dLineIndex + 1 );

        SendMessage( EM_EXSETSEL, 0,(LPARAM) &cha );

        FireSetLine();
        m_bPosChanged = TRUE;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SetCursorPositionByLine
**
** .Beschreibung: Cursor setzen als Zeile/Spalte Angabe
**
** .Parameter:    long, lZeile  , I, neue Zeilen Position ( ab 0 )
**                long, lSpalte , I, neue Spalten Position ( ab 0 )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode positioniert den Cursor an die angegebene Position
**      und scrolled an diese Stelle.
**-----------------------------------------------------------------
*/
void CTBEditor::SetCursorPositionByLine(long lZeile, long lSpalte)
{
    CHARRANGE cha;
    DWORD dLineIndex;

    if( m_hWnd )
    {
        dLineIndex = SendMessage( EM_LINEINDEX, (WPARAM)lZeile, 0L );
        if( dLineIndex == -1 )
        {//Zeilenummer zu gross
            MessageBeep(0);
            return;
        }

        ScrollToLine( lZeile + 1 );

        cha.cpMin = cha.cpMax = dLineIndex + lSpalte;
        SendMessage( EM_EXSETSEL, 0,(LPARAM) &cha );

        FireSetLine();
        m_bPosChanged = TRUE;
    }
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      TempSaveObject
**
** .Beschreibung: Abspeichern des Texts in eine temporaere Datei
**
** .Parameter:    BSTR*, pFileName, I, Name der Datei
**
** .Rueckgabewert:  BOOL, TRUE bei Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Speichert den Inhalt des RichEdits in eine Datei, ohne den internen
**      Dateinamen zu veraendern.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::TempSaveObject(BSTR FAR* pFileName)
{
    USES_CONVERSION;

    if( !m_hWnd )
        return FALSE;

    if( pFileName )
    {
        LPSTR pSTR = W2A( *pFileName );

        OFSTRUCT of;
        EDITSTREAM eStream;

        if( ( m_hFile = OpenFile( pSTR, &of, OF_CREATE ) ) != HFILE_ERROR )
        {
            eStream.dwCookie = m_hFile;
            eStream.dwError = 0;
            eStream.pfnCallback = SaveCallback;
            SendMessage( EM_STREAMOUT, (WPARAM)SF_TEXT, (LPARAM)&eStream);

            CloseHandle( (HANDLE) m_hFile);
            SetFocus();
        }
        else
            return FALSE;
    }
    else
        return FALSE;

    return TRUE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      FindTextPosition
**
** .Beschreibung: Textsuche
**
** .Parameter:    BSTR*,  pSearchString  , I, Suchstring
**                long*,  lTextPosition  , IO, I Position ab der gesucht werden soll
**                                               wenn -1 dann ab der Cursorposition suchen
**                                             O Position des gefundenen Strings
**                                               als zero-based character Index
**                long ,  lOptions       , I, Suchoptionen
**
** .Rueckgabewert:  long, E_FAIL bei Fehler
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode bietet eine normale Fulltext- Suche und eine Regular
**      Expression Suche.
**      lOptions enthaelt Bitschalter, z.B. Bit 1 gesetzt -> Richtung Ende
**                                                auf Null ->Richtung Anfang
**      lOptions:     0x01 -> DOWN (d.h. Richtung Textende )
**                    0x02 -> MATCHCASE
**                    0x04 -> Regular Expression
**      Erlaubte Kombinationen sind ( 0x01 und 0x02 ) und ( 0x01 und 0x04 )
**-----------------------------------------------------------------
*/
long CTBEditor::FindTextPosition(BSTR FAR* pSearchString, long FAR* lTextPosition,
                                 long lOptions )
{
   USES_CONVERSION;
   LPSTR pSTR = W2A( *pSearchString );
   long lReturn = 0L;

   if( m_hWnd )
    {
        CHARRANGE cha;

        GetAllText();

        if( lOptions & 0x04 ) //RegExp Search
        {
            // Startpunkt der Suche feststellen
            if(*lTextPosition == -1)
            {
               //aktuelle Char-Position
               SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
            }
            else
            {
               cha.cpMin = *lTextPosition;
            }

            TBRegexp RegExp( pSTR, TRUE );

            if( lOptions & 0x01 ) //down search ( akt. Pos bis Ende )
            {
                *lTextPosition = RegExp.search( m_pBigBuf,
                                                (unsigned int)cha.cpMin,
                                                m_dBigBufSize );
            }
            else // up search ( akt.Pos bis Anfang )
            {
                long lFound = -1;
                long lMerk;

                //suche vom Textanfang immerwieder bis zum letzten
                //gefunden vor der akt. Pos
                do
                {
                    lMerk = lFound;

                    lFound ++ ;

                    lFound = RegExp.search( m_pBigBuf,
                                            (unsigned int)lFound,
                                            cha.cpMin - lFound );
                } while( lFound >= 0 && lFound < cha.cpMin );

                *lTextPosition = lMerk;
            }
            if( *lTextPosition > 0 )
                lReturn = RegExp.match( m_pBigBuf, *lTextPosition,
                                        m_dBigBufSize-*lTextPosition );
            else
                lReturn = 0;
        }
        else //normale Textsuche
        {
            if( SearchText( pSTR, lOptions & 0x01, lOptions & 0x02, &cha ) )
                *lTextPosition = cha.cpMin;
            else
                *lTextPosition = -1;
            lReturn = 1;
        }
   }
   else
    {
      *lTextPosition = -1;
      lReturn = -1;
   }

    delete [] m_pBigBuf;
    m_pBigBuf = NULL;
    return lReturn;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetActualTextLine
**
** .Beschreibung: Liefert die aktuelle Zeile
**
** .Parameter:    BSTR*, pBuffer  , O, Puffer fuer Zeile
**                long , lLength  , I, Groesse des Puffers
**
** .Rueckgabewert:  long, Groesse der Zeile incl. terminating NULL
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Kopiert den Inhalt der aktuell bearbeiteten Zeile in einen Puffer.
**      Reicht die Puffergroesse nicht aus, geschieht nichts!
**-----------------------------------------------------------------
*/
long CTBEditor::GetActualTextLine(BSTR FAR* pBuffer, long lLength)
{
    USES_CONVERSION;

    DWORD dLineLength;
    DWORD dLineIndex;
    DWORD dCharIndex;
    TEXTRANGE tr;
    CHARRANGE cha;

    if( ! m_hWnd )
        return( -1 );

    //aktuelle Char-Position
    SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
    //aktuellen Zeileindex
    dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)cha.cpMin );
    //CharIndex der aktuellen Zeile
    dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex, 0L );
    //Laenge ermitteln
    dLineLength = SendMessage( EM_LINELENGTH, (WPARAM)dCharIndex, 0L );


    if( !pBuffer || lLength <= 1 || dLineLength >= (unsigned long)lLength )
        return dLineLength;  //einfach zu kleiner Puffer

    if ( dLineLength == 0 )
       m_pLineBuf[ 0 ] = 0;
    else
    {
       tr.chrg.cpMin = dCharIndex;
       tr.chrg.cpMax = dCharIndex + dLineLength;
       tr.lpstrText = m_pLineBuf;
       SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr );
    }

    BSTR pStr = A2W( m_pLineBuf );

    wcscpy_s( *pBuffer, lLength, pStr );

    return dLineLength;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetTextFromLineOfIndex
**
** .Beschreibung: Liefert den Text der Zeile die durch den Index lIndex
**                referenziert wird
**
** .Parameter:    BSTR*, pBuffer  , O, Puffer fuer Zeile
**                long , lLength  , I, Groesse des Puffers
**                long , lIndex   , I, Indexposition
**
** .Rueckgabewert:  long, Groesse der Zeile incl. terminating NULL
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Kopiert den Inhalt der dur lIndex referenzierten Zeile in einen Puffer.
**      Reicht die Puffergroesse nicht aus, geschieht nichts!
**-----------------------------------------------------------------
*/
long CTBEditor::GetTextFromLineOfIndex(BSTR FAR* pBuffer, long lLength, long lIndex)
{
    USES_CONVERSION;

    DWORD dLineLength;
    DWORD dLineIndex;
    DWORD dCharIndex;
    TEXTRANGE tr;

    if( ! m_hWnd )
        return( -1 );

    if( lIndex < 0)
        return( -2 );


    dLineIndex = SendMessage( EM_EXLINEFROMCHAR, 0, (LPARAM)lIndex);
    //CharIndex der Zeile dLineIndex
    dCharIndex = SendMessage( EM_LINEINDEX, (WPARAM)dLineIndex , 0L );
    if( dCharIndex < 0 )
        return( -3 );

    //Laenge ermitteln
    dLineLength = SendMessage( EM_LINELENGTH, (WPARAM)dCharIndex, 0L );


    if( !pBuffer || lLength <= 1 || dLineLength >= (unsigned long)lLength )
        return dLineLength;  //einfach zu kleiner Puffer


    if ( dLineLength == 0 )
       m_pLineBuf[ 0 ] = 0;
    else
    {
       tr.chrg.cpMin = dCharIndex;
       tr.chrg.cpMax = dCharIndex + dLineLength;
       tr.lpstrText = m_pLineBuf;
       SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr );
    }

    BSTR pStr = A2W( m_pLineBuf );

    wcscpy_s( *pBuffer, lLength, pStr );

    return dLineLength;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      IsReadOnly
**
** .Beschreibung: Liefert den ReadOnly Status des RichEdits
**
** .Parameter:
**
** .Rueckgabewert:  BOOL, TRUE -> RichEdit ist ReadOnly
**                        FALSE -> RichEdit ist Read/Write
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
BOOL CTBEditor::IsReadOnly()
{ //true -> ReadOnly
   return m_bReadOnly;
}



/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      IsCommentAtIndex
**
** .Beschreibung: Stellt fest, ob sich der Index in einem
**                Kommentar befindet.
** .Parameter:    long , lIndex   , I, Indexposition
**
** .Rueckgabewert:  BOOL, TRUE -> in Kommentar
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode funktioniert nur vernuenftig, wenn das Syntax Coloring
**      eingeschaltet ist, da es sich an der Faerbung orientiert.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::IsCommentAtIndex(long lIndex)
{
    CHARFORMAT cfo;
    CHARRANGE chMerk;
    CHARRANGE cha;

    if( ! m_hWnd )
        return FALSE;

    // position merken
    SendMessage( EM_EXGETSEL, 0,(LPARAM) &chMerk );
    // Indexposition setzen
    SendMessage( EM_HIDESELECTION, TRUE, FALSE);

    cha.cpMin = lIndex;
    cha.cpMax = lIndex;

    SendMessage( EM_EXSETSEL, 0,(LPARAM) &cha );


    ZeroMemory( &cfo, sizeof( CHARFORMAT ) );
    cfo.cbSize = sizeof( CHARFORMAT );
    cfo.crTextColor = m_crForeground;

    SendMessage( EM_GETCHARFORMAT, TRUE, (LPARAM) &cfo );

    // Alles wieder zurücksetzen
    SendMessage( EM_EXSETSEL, 0,(LPARAM) &chMerk );
    SendMessage( EM_HIDESELECTION, FALSE, FALSE);

    if( cfo.crTextColor == m_crComment )
      return TRUE;
    else
      return FALSE;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      WantKeystrokes
**
** .Beschreibung: Togglen des Keystroke Events
**
** .Parameter:    BOOL , bWantKeys, I,  TRUE -> bitte Keystrokes schicken
**                                      FALSE -> oh nein, danke
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Legt fest ob, das Keystroke-Event verschickt werden soll. Ist
**      ZeidonSyntax aktiviert,wird das Event standardmaessig geschickt.
**-----------------------------------------------------------------
*/
void CTBEditor::WantKeystrokes(BOOL bWantKeys)
{
    m_bWantKeys = bWantKeys;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      DeleteTextRange
**
** .Beschreibung: Loeschen eines ausgewaehlten Textbereichs
**
** .Parameter:    long , lStartIndex, I, zero-based characterindex Anfang
**                long , lEndIndex  , I, zero-based characterindex Ende
**
** .Rueckgabewert: BOOL,  TRUE -> erfolgreich
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Es kann ein userdefinierter Range angegeben werden der geloescht
**      wird. lEndIndex liegt hinter(!) dem zu loeschenden Bereich!
**-----------------------------------------------------------------
*/
BOOL CTBEditor::DeleteTextRange(long lStartIndex, long lEndIndex)
{
    CHARRANGE chMerk;
    CHARRANGE cha;

    SendMessage( EM_EXGETSEL, 0,(LPARAM) &chMerk );

    if( lStartIndex < 0 || lEndIndex < lStartIndex )
        return FALSE;

    cha = m_chAlt;  //alten Wert merken

    m_chAlt.cpMin = lStartIndex;
    m_chAlt.cpMax = lEndIndex;

    SendMessage( EM_EXSETSEL, 0,(LPARAM) &m_chAlt );
    GetSelectionBuffer();
    SendMessage( WM_CLEAR, 0, 0L );

    m_bPosChanged = TRUE;

    if( m_bSelection )
        m_pUndo->DeleteRange( m_chAlt.cpMin, m_tr.lpstrText );

    m_chAlt = cha; // und wiederherstellen

    TestInput();

    SendMessage( EM_EXSETSEL, 0,(LPARAM) &chMerk );
    FireSetLine();
   return TRUE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      CloseSubWindow
**
** .Beschreibung: Schliesst einen offenen Dialog
**
** .Parameter:
**
** .Rueckgabewert:  BOOL, TRUE -> es gab einen Dialog
**                        FALSE -> es gab keinen Dialog
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
BOOL CTBEditor::CloseSubWindow()
{
    if( m_hWndModal )
    {
        ::DestroyWindow( m_hWndModal );

        SendMessage( EM_HIDESELECTION, TRUE, TRUE );
        SetFocus();
        m_hWndModal = NULL;  //Merker wieder auf NULL setzen
        m_bPosChanged = TRUE;
        return TRUE;
    }
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SearchForward
**
** .Beschreibung: Suchen in Richtung Textende
**
** .Parameter:
**
** .Rueckgabewert:
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Es wird die Suchroutine SearchText verwendet. Der Suchtext und
**      die CASE-Einstellung stammen aus der Menu-Suchfunktion.
**-----------------------------------------------------------------
*/
void CTBEditor::SearchForward()
{
    CHARRANGE cha;

    if( ! m_hWnd || ! m_pSearchString )
    {
        MessageBeep(0);
        return;
    }

    GetAllText(); // EditText besorgen

    if( SearchText( m_pSearchString, TRUE, m_bMatchCase, &cha ) )
    {
        SendMessage( EM_EXSETSEL, 0, (LPARAM) &cha );
        m_bPosChanged = TRUE;
        FireSetLine();
    }
    else
    {
        MessageBeep(0);
        DialogMeldung( IDS_SEARCH );
    }

    delete [] m_pBigBuf;
    m_pBigBuf = NULL;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SearchBackward
**
** .Beschreibung: Suchen in Richtung Dateianfang
**
** .Parameter:
**
** .Rueckgabewert:
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Es wird die Suchroutine SearchText verwendet. Der Suchtext und
**      die CASE-Einstellung stammen aus der Menu-Suchfunktion.
**-----------------------------------------------------------------
*/
void CTBEditor::SearchBackward()
{
    CHARRANGE cha;

    if( ! m_hWnd || ! m_pSearchString )
    {
        MessageBeep(0);
        return;
    }

    GetAllText(); // EditText besorgen

    if( SearchText( m_pSearchString, FALSE, m_bMatchCase, &cha ) )
    {
        SendMessage( EM_EXSETSEL, 0, (LPARAM) &cha );
        m_bPosChanged = TRUE;
        FireSetLine();
    }
    else
    {
        MessageBeep(0);
        DialogMeldung( IDS_SEARCH );
        m_bShift = FALSE; //sonst wird das SHIFT-up vom Dialog verschluckt
    }

    delete [] m_pBigBuf;
    m_pBigBuf = NULL;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      Deselect
**
** .Beschreibung: Markierung entfernen
**
** .Parameter:
**
** .Rueckgabewert:
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode entfernt eine eventuell vorhandene Markierung.
**      Fuer Container geeignet, die ESC-Taste verschlucken.
**-----------------------------------------------------------------
*/
void CTBEditor::Deselect()
{
    DWORD dErg;

    if( ! m_hWnd )
        return;

    dErg = SendMessage( EM_SELECTIONTYPE, 0, 0L );
    if( dErg & SEL_MULTICHAR )
    {
        CHARRANGE cha;
        SendMessage( EM_EXGETSEL, 0,(LPARAM) &cha );
        cha.cpMax = cha.cpMin;
        SendMessage( EM_EXSETSEL, 0,(LPARAM) &cha );
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      Undo
**
** .Beschreibung: Undo-Methode
**
** .Parameter:
**
** .Rueckgabewert:   BOOL, TRUE -> Undo durchgefuehrt
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
BOOL CTBEditor::Undo()
{
    SendMessage( EM_EXGETSEL, 0,(LPARAM) &m_chAlt );

    DWORD dErg = m_pUndo->MakeUndo();

    m_bPosChanged = TRUE;

    TestInput();
    FireSetLine();

    if( dErg != 0 )
        return TRUE;
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      Redo
**
** .Beschreibung: Redo-Methode
**
** .Parameter:
**
** .Rueckgabewert:   BOOL, TRUE -> Redo durchgefuehrt
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
BOOL CTBEditor::Redo()
{
    SendMessage( EM_EXGETSEL, 0,(LPARAM) &m_chAlt );

    DWORD dErg = m_pUndo->MakeRedo();

    m_bPosChanged = TRUE;

    TestInput();
    FireSetLine();

    if( dErg != 0 )
        return TRUE;
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SearchWindow
**
** .Beschreibung: Find-Dialog von Windows aufrufen
**
** .Parameter:
**
** .Rueckgabewert:
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CTBEditor::SearchWindow()
{
    if( m_hWnd )
        FindReplace( FALSE );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      ReplaceWindow
**
** .Beschreibung: Replace-Dialog von Windows aufrufen
**
** .Parameter:
**
** .Rueckgabewert:
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CTBEditor::ReplaceWindow()
{
    if( m_hWnd )
        FindReplace( TRUE );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      SelectAll
**
** .Beschreibung: Gesamten Text markieren
**
** .Parameter:
**
** .Rueckgabewert:
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CTBEditor::SelectAll()
{
    CHARRANGE cha;

    if( ! m_hWnd )
        return;

    cha.cpMin = 0;
    cha.cpMax = -1;

    SendMessage( EM_EXSETSEL, 0, (LPARAM) &cha );
    SendMessage( EM_HIDESELECTION, 0, 0L );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GotoWindow
**
** .Beschreibung: GoTo-Dialog aufrufen
**
** .Parameter:
**
** .Rueckgabewert:  BOOL, TRUE->Neue Tab-Einstellungen
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Ermittelt ueber CDotoDlg die neue Zeile und springt diese mit
**      ScrollToLine an.
**-----------------------------------------------------------------
*/
BOOL CTBEditor::GotoWindow()
{
    BOOL bErg = FALSE;

    if( ! m_hWnd )
        return bErg;

    long lLine = 1;

    CGotoDlg dlg( lLine );
    if( dlg.DoModal() == TRUE )
    {
        (void) ScrollToLine( dlg.m_lData );
        bErg = TRUE;
    }

    FireSetLine();
    m_bPosChanged = TRUE;
    SetFocus();

    return bErg;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      IsSubWindowActive
**
** .Beschreibung: Stellt fest, ob ein modeless Dialog aktiv ist
**
** .Parameter:
**
** .Rueckgabewert:  BOOL, TRUE -> SubWindow aktiv
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
BOOL CTBEditor::IsSubWindowActive()
{
    if( ! m_hWnd || ! m_hWndModal )
        return FALSE;


    if( m_hWnd == ::GetFocus() )
      return FALSE;
    else
        return TRUE;
}


BOOL CTBEditor::CanCopy()
{
    if( m_hWnd )
    {
        CHARRANGE cha;
        cha.cpMin = cha.cpMax = 0;

        SendMessage( EM_EXGETSEL, 0, (LPARAM)&cha );
        if( cha.cpMin == cha.cpMax )
            return FALSE;
        else
            return TRUE;
    }
    else
        return FALSE;
}

BOOL CTBEditor::CanPaste()
{
   if( IsClipboardFormatAvailable(CF_TEXT) )
        return TRUE;
    else
        return FALSE;
}


BOOL CTBEditor::CanUndo()
{
    if( m_pUndo )
        return m_pUndo->CanUndo();
    else
        return FALSE;
}

BOOL CTBEditor::CanRedo()
{
    if( m_pUndo )
        return m_pUndo->CanRedo();
    else
        return FALSE;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetTextFromRange
**
** .Beschreibung: Liefert den Text der Zeile die durch den Index lIndex
**                referenziert wird
**
** .Parameter:    BSTR*, pBuffer  , O, Puffer fuer Zeile
**                long , lStart  ,  I, Indexposition ( start )
**                long , lEnd     , I, Indexposition ( ende )
**
** .Rueckgabewert:  long, Groesse der Zeile incl. terminating NULL
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Kopiert den Inhalt der Range die durch lStart und lEnd definiert ist in einen Puffer.
**      gegebenen Puffer
**-----------------------------------------------------------------
*/
long CTBEditor::GetTextFromRange(BSTR FAR* pBuffer, long lStart, long lEnd)
{
    USES_CONVERSION;

    DWORD dLength;
    TEXTRANGE tr;

    if( ! m_hWnd )
        return( -1 );

    //Laenge ermitteln
    dLength = lEnd - lStart;
    if( dLength <= 1 )
        return( -2 );  //nothing to do

    if ( !pBuffer )
       return( -3 );

    tr.chrg.cpMin = (DWORD)lStart;
    tr.chrg.cpMax = (DWORD)lEnd;
    CString strHelp;

    tr.lpstrText = strHelp.GetBufferSetLength( ( long )dLength );
    long nReturnLength = SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr );
    strHelp.ReleaseBuffer();

    BSTR pStr = A2W( strHelp );

    wcscpy_s( *pBuffer, dLength, pStr );

    return( nReturnLength );
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetObjectName
**
** .Beschreibung: Liefert den Namen der geöffneten Datei
**
**
** .Parameter:    BSTR*, pBuffer  , O, Puffer fuer Dateinamen
**
** .Rueckgabewert:  long, länge des Dateinamens
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Liefert den Namen der geöffneten Datei
**-----------------------------------------------------------------
*/
long
CTBEditor::GetObjectName( BSTR FAR* pBuffer )
{
   USES_CONVERSION;

   long nReturnLength = strlen( m_cFileName );

   if ( pBuffer )
   {
      BSTR pStr = A2W( m_cFileName );
      wcscpy_s( *pBuffer, nReturnLength + 1, pStr );
   }

   return nReturnLength;
}

/*
**----------------------------------------------------------------------
** .Klasse:       CTBEditor
**
** .Methode:      GetSelectedText
**
** .Beschreibung: Liefert den Text der Zeile die durch den Index lIndex
**                referenziert wird
**
** .Parameter:    BSTR*, pBuffer  , O, Puffer fuer Zeile
**                long , lLength  , I, Bufferlänge
**                long , lEnd     , I, Indexposition ( ende )
**
** .Rueckgabewert:  long, Groesse des selektierten Textes incl. terminating NULL
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Kopiert den Inhalt einer Selektion in einen Puffer.
**-----------------------------------------------------------------
*/
long CTBEditor::GetSelectedText(BSTR FAR* pBuffer, long lLength )
{
   USES_CONVERSION;

   if( ! m_hWnd )
       return( -1 );

   // use a hack to make code easier to understand
   CRichEditCtrl *re = (CRichEditCtrl *)this;
   CString strHelp = re->GetSelText();

   long lErg = strHelp.GetLength();

   if( lLength > lErg && pBuffer )
   {
      BSTR pStr = A2W( strHelp );
      wcscpy_s( *pBuffer, lLength, pStr );
   }

/*
   TEXTRANGE tr;
   ::SendMessage( m_hWnd, EM_EXGETSEL, 0,(LPARAM) &tr.chrg );
   lErg = tr.chrg.cpMax - tr.chrg.cpMin;

   if( lLength > lErg && pBuffer )
   {
      CString strHelp;

      tr.lpstrText = strHelp.GetBufferSetLength( lLength );
      lErg = SendMessage( EM_GETTEXTRANGE, 0, (LPARAM) &tr );
      strHelp.ReleaseBuffer();

   }
*/
   return lErg;
}