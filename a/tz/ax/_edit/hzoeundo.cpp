/*
**----------------------------------------------------------------------
** .Klasse:        CUndo
**
** .Version:       01A
**
** .Aenderung:     20.05.1998
**
** .Beschreibung:  Klasse fuer Undo/Redo des TBEditors
**
** .Basisklassen:
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      Die Klasse bildet einen UNDO\REDO-Mechanismus fuer Textverarbeitung
**      Sie bietet verschiedene AKtionstypen an ( siehe Enum EDIT_ACTION ),
**      die ausreichen sollten um alle Editieraktionen abzubilden.
**
**      Die Frage, welche Aktionen abgespeichert werden, bleibt dem
**      Benutzer der Klasse ueberlassen.
**      Da die Klasse mit einem dynamischen Ringpuffer arbeitet, kann die
**      Anzahl der gemerkten Aktionen beliebig eingestellt werden.
**
**      Sollen Textbereiche abgespeichert werden, muss mit MakeNewBuffer
**      zuerst Speicher allokiert werden. Und dann der Text dorthin kopiert
**      werden.
**
**      Bedeutung einiger Pointer:
**          m_pBegin -> das erste Element der UNDO-Kette
**          m_pInsert -> das Element, das akt. Aktion aufnimmt. Erreicht er
**                       wieder m_pBegin, werden beide um eins nach vorne
**                       geschoben
**          m_pUndo -> das Element, das akt. fuer UNDO verwendet wird. Dieser
**                     Pointer lauft normalerweise mit m_pInsert mit. Wenn
**                     UNDOs ausgefuehrt werden laeuft er rueckwaerts bis
**                     zu m_pBegin
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
** 01C  =  change MakeNewBuffer to pass a string pointer and copy it's contents
**         this makes life a littel bit easier when dealing with undobuffer
**         (26.10.2000)  TMV
**
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "hzoeundo.h"



/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      Konstruktor
**
** .Beschreibung: Initialisierung des Undo-Puffers
**
** .Parameter:  HWND  , hEdit, I, Handle des RichEdits
**              long  , lAnzElem, I, Anzahl der moeglichen Undos
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Initialisiert den Ringpuffer zur Verwaltung der Undos. Es wird
**      ein doppelt verketteter Puffer aufgebaut.
**-----------------------------------------------------------------
*/
CUndo::CUndo( HWND hEdit, long lAnzElem )
{
    m_hEdit = hEdit;
    m_bStop = FALSE;

    if( lAnzElem == 0 )
        m_lAnzElem = 1;
    else
        m_lAnzElem = lAnzElem;

    //Ring aufbauen
    m_pUrsprung = new UndoElem[ m_lAnzElem ];

    UndoElem *pAkt = & (m_pUrsprung[0]);
    UndoElem *Nachfolger = pAkt + 1;
    UndoElem *Vorgaenger = & (m_pUrsprung[m_lAnzElem-1]);

    for( int i = 1; i <= m_lAnzElem; i++ )
    {
        pAkt->dAnfang = 0;
        pAkt->enAction = LEER;
        ZeroMemory( pAkt->cCharIns, 3 );
        ZeroMemory( pAkt->cCharDel, 3 );
        pAkt->pDel = NULL;
        pAkt->pIns = NULL;
        pAkt->pPrev = Vorgaenger;
        pAkt->pNext = Nachfolger;

        Vorgaenger = pAkt;
        pAkt = Nachfolger;
        Nachfolger = & (m_pUrsprung[i+1]);
    }

    Vorgaenger->pNext = m_pUrsprung; //letzten Schleifendurchlauf korrigiern
                                     //und Ring schliessen

    m_pInsert = m_pUrsprung;
    m_pUndo = m_pInsert;
    m_pBegin = m_pUrsprung;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      Destruktor
**
** .Beschreibung: Freigabe des Undo-Puffers
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
CUndo::~CUndo()
{
    //Ring loeschen
    UndoElem *p = m_pUrsprung;

    do
    {
        if( p->pDel )
            delete [] p->pDel;
        if( p->pIns )
            delete [] p->pIns;

        p = p->pNext;
    } while( p != m_pUrsprung );

    delete [] m_pUrsprung;
}



/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      NewAction
**
** .Beschreibung: Neue Undo-Aktion beginnen
**
** .Parameter:
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Verwaltungszeiger m_pInsert, m_pUndo und m_pBegin werden
**      auf neue Positionen gesetzt. Eventuell allokierter Speicher
**      wird wieder freigegeben.
**-----------------------------------------------------------------
*/
void CUndo::NewAction()
{
    if( m_pInsert == m_pUndo )
    { //das ist der NormalFall
        m_pInsert = m_pInsert->pNext;  //Zeiger eins weiter

        if( m_pInsert == m_pBegin ) //wenn am Anfang des Rings -> Anfang eins weiter
            m_pBegin = m_pBegin->pNext;

        m_pUndo = m_pInsert;  //Undo-Zeiger neu setzen
    }
    else
    { // es hat ein UNDO stattgefunden
        if( m_bStop )
        { //der komplette UNDO-Puffer wurde geleert
            m_pInsert = m_pUndo;
        }
        else
        { //nur ein Paar UNDOs wurden ausgefuehrt
            m_pInsert = m_pUndo->pNext;
            m_pUndo = m_pInsert;
        }
    }

    if( m_pInsert->pDel )       //allokierten Speicher freigeben
        delete [] m_pInsert->pDel;
    if( m_pInsert->pIns )
        delete [] m_pInsert->pIns;

    m_bStop = FALSE;

    m_pInsert->pDel = NULL;
    m_pInsert->pIns = NULL;
    ZeroMemory( m_pInsert->cCharIns, 3 );
    ZeroMemory( m_pInsert->cCharDel, 3 );
}



/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      InsertCharacter
**
** .Beschreibung: InsertAction ( ein Zeichen )
**
** .Parameter:    DWORD,  dPos , I, aktuelle Position
**                char,   cIns , I, das eingefuegte Zeichen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      \r wird gesondert behandelt, um ein sauberes CRLF zu erhalten
**-----------------------------------------------------------------
*/
void CUndo::InsertCharacter( DWORD dPos, char cIns ) //INSCHAR
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = INSCHAR;

    m_pInsert->cCharIns[0] = cIns;

    if( cIns == '\r' )
         m_pInsert->cCharIns[1] = '\n';
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      InsertCharacter
**
** .Beschreibung: InsertAction bei Overwrite ( ein Zeichen )
**
** .Parameter:    DWORD,  dPos , I, aktuelle Position
**                char,   cIns , I, das eingefuegte Zeichen
**                char,   cDel , I, das geloeschte Zeichen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::InsertCharacter( DWORD dPos, char cIns, char cDel ) //INSCHARDEL
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = INSCHARDEL;

    m_pInsert->cCharIns[0] = cIns;
    m_pInsert->cCharDel[0] = cDel;
/*
    if( cIns == '\r' )
         m_pInsert->cCharIns[1] = '\n';
    if( cDel == '\r' )
         m_pInsert->cCharDel[1] = '\n';
*/
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      InsertRange
**
** .Beschreibung: InsertAction ( mehrere Zeichen )
**
** .Parameter:    DWORD,  dPos      , I, aktuelle Position
**                char*,  pInsRange , I, Zeiger auf eingefuegte Zeichen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::InsertRange( DWORD dPos, char *pInsRange )
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = INSRANGE;

    m_pInsert->pIns = pInsRange;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      InsertRange
**
** .Beschreibung: InsertAction bei Overwrite ( mehrere Zeichen )
**
** .Parameter:    DWORD,  dPos      , I, aktuelle Position
**                char*,  pInsRange , I, Zeiger auf eingefuegte Zeichen
**                char*,  pDelrange , I, Zeiger auf geloeschten Text
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::InsertRange( DWORD dPos, char *pInsRange, char *pDelRange )
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = INSRANGEDEL;

    m_pInsert->pIns = pInsRange;
    m_pInsert->pDel = pDelRange;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      DeleteCharacter
**
** .Beschreibung: DeleteAction ( ein Zeichen )
**
** .Parameter:    DWORD,  dPos      , I, aktuelle Position
**                char ,  cDel      , I, das geloeschte Zeichen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::DeleteCharacter( DWORD dPos, char cDel )
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = DELCHAR;

    m_pInsert->cCharDel[0] = cDel;

    if( cDel == '\r' )
         m_pInsert->cCharDel[1] = '\n';
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      DeleteRange
**
** .Beschreibung: DeleteAction ( mehrere Zeichen )
**
** .Parameter:    DWORD,  dPos      , I, aktuelle Position
**                char*,  pDelRange , I, Zeiger auf geloeschte Zeichen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::DeleteRange( DWORD dPos, char *pDelRange )
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = DELRANGE;

    m_pInsert->pDel = pDelRange;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      DeleteRangeByChar
**
** .Beschreibung: DeleteAction ( mehrere Zeichen )
**
** .Parameter:    DWORD,  dPos      , I, aktuelle Position
**                char*,  pDelRange , I, Zeiger auf geloeschte Zeichen
**                char ,  cIns      , I, das eingefuegte Zeichen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::DeleteRangeByChar( DWORD dPos, char *pDelRange, char cIns )
{
    NewAction();

    m_pInsert->dAnfang = dPos;
    m_pInsert->enAction = DELRANGEBYCHAR;

    m_pInsert->pDel = pDelRange;

    m_pInsert->cCharIns[0] = cIns;

    if( cIns == '\r' )
         m_pInsert->cCharIns[1] = '\n';
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      MakeUndo
**
** .Beschreibung: Undo durchfuehren
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      In Abhaengigkeit von der UNDO-Art( enAction ) werden verschiedene
**      Behandlungsarten verwendet.
**      Der Zeiger m_pUndo laueft solange nach hinten, bis er m_pBegin
**      erreicht, dann wird m-bStop gesetzt
**-----------------------------------------------------------------
*/
DWORD CUndo::MakeUndo()
{
    if( m_bStop )
        MessageBeep(0);
    else
    {
        CHARRANGE cha;

        switch( m_pUndo->enAction )
        {
            case INSCHAR:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + 1;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharDel );
                break;

            case INSCHARDEL:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + 1;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharDel );
                break;

            case INSRANGE:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + lstrlen( m_pUndo->pIns );

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharDel );
                break;

            case INSRANGEDEL:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + lstrlen( m_pUndo->pIns );

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->pDel );
                break;

            case DELCHAR:
                cha.cpMin = cha.cpMax = m_pUndo->dAnfang;
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharDel );
                break;

            case DELRANGE:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->pDel );
                break;

            case DELRANGEBYCHAR:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + 1;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->pDel );
                break;

            default:
                MessageBeep(0);
        }


        if( m_pUndo == m_pBegin )
            m_bStop = TRUE;
        else
        {
            m_pUndo = m_pUndo->pPrev;
        }

        return m_pUndo->dAnfang;
    }

    return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      MakeRedo
**
** .Beschreibung: Redo durchfuehren
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      m_bStop ( zeigt an das die letzte UNDO-Aktion ausgefuehrt wurde )
**      wird gegebenfalls zurueckgesetzt. REDO-Aktionen koennen solange
**      ausgefuehrt werden, bis m_pUndo m_pInsert erreicht.
**-----------------------------------------------------------------
*/
DWORD CUndo::MakeRedo()
{
    CHARRANGE cha;

    if( m_pInsert == m_pUndo )
        MessageBeep(0);
    else
    {
        if( m_bStop )
            m_bStop = FALSE;
        else
            m_pUndo = m_pUndo->pNext;


        switch( m_pUndo->enAction )
        {
            case INSCHAR:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharIns );
                break;

            case INSCHARDEL:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + 1;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharIns );
                break;

            case INSRANGE:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->pIns );
                break;

            case INSRANGEDEL:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + lstrlen( m_pUndo->pDel );

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->pIns );
                break;

            case DELCHAR:
                cha.cpMin = m_pUndo->dAnfang;
                if( m_pUndo->cCharDel[0] == '\r' )
                    cha.cpMax = m_pUndo->dAnfang + 2;
                else
                    cha.cpMax = m_pUndo->dAnfang + 1;

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharIns );
                break;

            case DELRANGE:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + lstrlen( m_pUndo->pDel );

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharIns );
                break;


            case DELRANGEBYCHAR:
                cha.cpMin = m_pUndo->dAnfang;
                cha.cpMax = m_pUndo->dAnfang + lstrlen( m_pUndo->pDel );

                SendMessage( m_hEdit, EM_HIDESELECTION, TRUE, (LPARAM) FALSE );
                SendMessage( m_hEdit, EM_EXSETSEL, 0, (LPARAM) &cha );
                SendMessage( m_hEdit, EM_REPLACESEL, 0, ( LPARAM ) m_pUndo->cCharIns );
                break;

            default:
                MessageBeep(0);
        }

        return m_pUndo->dAnfang;
    }

    return( 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      MakeNewBuffer
**
** .Beschreibung: neuen Speicher allokieren
**
** .Parameter:    DWORD,  dAnfPos  , I, AnfangsPosition
**                DWORD,  dEndePos , I, EndePosition
**
** .Rueckgabewert:  char *,  NULL bei Fehler
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          Aus dAnfPos und dEndePos wird die Groesse des benoetigten
**          Speichers ermittelt.
**-----------------------------------------------------------------
*/
char * CUndo::MakeNewBuffer( DWORD dAnfPos, DWORD dEndePos )
{
    char *p;
    long lSize = 10;

    if( dAnfPos < dEndePos )
        lSize = dEndePos - dAnfPos;

    p = new char [lSize+5];

    return p;
}


/*
**----------------------------------------------------------------------
** .Klasse:         CUndo
**
** .Methode:        MakeNewBuffer
**
** .Beschreibung:   neuen Speicher allokieren und belegen
**
** .Parameter:      LPCSTR, lpcValue, I, input string
**
** .Rueckgabewert:  char *,  NULL bei Fehler
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          allokieren von Speicherplatz und kopieren des Eingabewertes
**-----------------------------------------------------------------
*/
char * CUndo::MakeNewBuffer( LPCTSTR lpcValue )
{
    char *p = NULL;
    long lSize = lstrlen( lpcValue );

    if ( lSize )
    {
      p = new char [ lSize + 1 ];
      if( p )
         lstrcpy( p, lpcValue );
    }
    return p;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      DeleteNewBuffer
**
** .Beschreibung: neuen Speicher wieder freigeben
**
** .Parameter:    char*,  pBuffer  , I, Zeiger auf zuvor allokierten Speicher
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          Falls der zuvor mit MakeNewBuffer allokierte Bereich doch nicht
**          benoetigt wird, kann er hiermit wieder freigegeben werden.
**-----------------------------------------------------------------
*/
void CUndo::DeleteNewBuffer( char *pBuffer )
{
    if( pBuffer )
        delete [] pBuffer;
}


/*
**----------------------------------------------------------------------
** .Klasse:       CUndo
**
** .Methode:      Initialize
**
** .Beschreibung: Neu Initialisierung der Liste
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
void CUndo::Initialize()
{
    m_pInsert = m_pBegin;

    do
    {
        m_pInsert->dAnfang = 0;
        m_pInsert->enAction = LEER;
        ZeroMemory( m_pInsert->cCharIns, 3 );
        ZeroMemory( m_pInsert->cCharDel, 3 );
        if( m_pInsert->pIns )
            delete [] m_pInsert->pIns;
        m_pInsert->pIns = NULL;
        if( m_pInsert->pDel )
            delete [] m_pInsert->pDel;
        m_pInsert->pDel = NULL;

        m_pInsert = m_pInsert->pNext;
    }
    while( m_pInsert != m_pBegin );

    m_pBegin = m_pUndo = m_pInsert;

    m_bStop = FALSE;
}
