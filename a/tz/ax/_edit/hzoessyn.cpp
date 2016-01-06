/*
**----------------------------------------------------------------------
** .Klasse:        SironSyntax
**
** .Version:       01B
**
** .Aenderung:     11.03.1999
**
** .Beschreibung:  Klasse fuer das Siron Syntax Coloring
**
** .Basisklassen:  SyntaxColoring
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      SironSyntax fuehrt das Syntax Coloring fuer Sironabfragen durch.
**      Keywords werden aus den Tabellen des HZ115 extrahiert. Hinzu kommen
**      einige zusammengesetzte Woerter, die dort nicht  aufgefuehrt sind.
**
**      Die Keyword-Suche erfolgt ueber HashKeys. D.h. es gibt einen Hash-
**      Array, der Pointer auf die entsprechende Keywords enthaelt.
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  '-' und '/' werden nun immer als Wortbestandteile erkannt
**         (11.03.1999)  SL
** 01C  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "tbedit.h"
#include "hzoesyn.h"
#include "hzoectl.h"

extern "C" {
#include "hs115d.h"
}



//Hilfsarrays, die Keywords enthalten, die aus HZ115 nicht direkt ableitbar sind
static char cGerZusatz[][KEY_ARR_SIZE] = {
        "BEGINN", "BEGINN-BENUTZE",
        "ENDE-WENN", "ENDE-SONST", "ENDE-BENUTZE",
        "ENDE-GW", "ENDE-DANN", "ENDE-SCHLEIFE" };

static char cEngZusatz[][KEY_ARR_SIZE] = {
        "BEGIN-PERFORM",
        "END-IF", "END-ELSE", "END-PERFORM",
        "END-CB", "END-THEN", "END-LOOP" };



/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      Konstruktor
**
** .Beschreibung: Initialisierung der Keyword-Tabellen
**
** .Parameter:  CTBEditor* , pT        , I, Pointer auf CTBEditor Instanz
**              char*      , pData     , I, Pointer auf Puffer mit Text
**              HWND       , hWnd      , I, Handle des RichEdits
**              BOOL       , bLanguage , I, TRUE -> German, else -> English
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Aufbau des Hasharrays und der Keywordtabelle, die in Abhaengigkeit
**      von der Sprache ( D oder E ) aus dem HZ115D oder HZ115E aufgebaut wird.
**-----------------------------------------------------------------
*/
SironSyntax::SironSyntax( CTBEditor *pT, char *pData, HWND hWnd, BOOL bLanguage )
    :SyntaxColoring( pT, pData, hWnd )
{
    m_bLanguage = bLanguage; // true -> German

    m_pHash = new char* [HASH_ARR_SIZE];
    m_pKeys = new char[HASH_ARR_SIZE][KEY_ARR_SIZE];

    for( int i = 0; i < HASH_ARR_SIZE; i++ )
    {
        m_pHash[i] = NULL;
        m_pKeys[i][0] = '\0';
    }


    if( m_bLanguage ) //deutsch
        m_pArray = ( char*) hz115d();
    else
        m_pArray = (char *) hz115e();


    //Keywordtabelle aus den HZ115x aufbauen
    short iSize;
    char *p;
    long pos = 0;

    iSize = * ((short *) m_pArray);

    //Algorythmus verlaesst sich darauf, dass die wirklichen Keywords im
    //HZ115 durch einen NULL-Eintrag vom restlichen Schrott getrennt sind
    do
    {
        m_pArray += sizeof( short );
        p = m_pArray; //Anfang eines Keywords merken

        m_pArray += iSize;

        if( *m_pArray != ' ' )
        {
            lstrcpyn( m_pKeys[pos], p, iSize+1 );
            iSize = * ((short *) m_pArray);
        }
        else
        { //wenn noch Blank-> ueberlesen bis zur naechsten Laenge
            lstrcpyn( m_pKeys[pos], p, iSize+1 );
            while ( *(++m_pArray) == ' ' )
                    ;
            iSize = * ((short *) m_pArray);
        }

        int x = Hash( m_pKeys[pos] );

        while( m_pHash[x] )
            x = (x+1) % HASH_ARR_SIZE;

        m_pHash[x] = m_pKeys[pos++];

    } while( iSize );


    //Zusaetzliche Keywords einhaengen
    char (*pKey)[KEY_ARR_SIZE];
    long lAnzKey;

    if( m_bLanguage )
    {
        lAnzKey = sizeof( cGerZusatz ) / KEY_ARR_SIZE;
        pKey = & cGerZusatz[0];
    }
    else
    {
        lAnzKey = sizeof( cEngZusatz ) / KEY_ARR_SIZE;
        pKey = & cEngZusatz[0];
    }

    //Hash-Tabelle fuellen
    for( long lPos = 0; lPos < lAnzKey; lPos++ )
    {
        int x = Hash( pKey[lPos] );

        while( m_pHash[x] )
            x = (x+1) % HASH_ARR_SIZE;

        m_pHash[x] = pKey[lPos];
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      Destruktor
**
** .Beschreibung: Freigabe der Ressurcen
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
SironSyntax::~SironSyntax( void )
{
    if( m_pHash )
        delete [] m_pHash;
    if( m_pKeys )
        delete [] m_pKeys;
}


/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      ParseLine
**
** .Beschreibung: Faerben eines TextBereichs
**
** .Parameter:    char* , pData     , I, Pointer auf Puffer mit Text
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode untersucht den aktuellen Editierbereich auf Keywords
**      und Kommentare.
**-----------------------------------------------------------------
*/
void SironSyntax::ParseLine(char* pData)
{
    m_pData = pData;
    m_iWord = 0;
    m_iLine = 0;
    m_dEnde = m_cha.cpMax;
    m_dAnfang = m_cha.cpMin;

    char ups;
    long lMerk;


    while( m_pData[m_iLine] )
    {
        m_cha.cpMin = m_dAnfang + m_iLine;

        if( m_pData[m_iLine] == '\'' ) //ist es ein Textstring?
        {
            lMerk = m_iLine++;

            while( m_pData[m_iLine] && m_pData[m_iLine] != '\'' &&
                   m_pData[m_iLine] != '\r' )
                m_iLine++;

            m_cha.cpMax = m_dAnfang + m_iLine;
            SetColor( m_pT->m_crForeground );
            m_cha.cpMin = m_cha.cpMax;
        }

        while( IsCharAlphaNumeric( m_pData[m_iLine] ) || m_pData[m_iLine] == '_' ||
                                   m_pData[m_iLine] == '/' || m_pData[m_iLine] == '-' )
        {
            m_iWord++;
            m_iLine++;
        }

        if( m_iWord )
        {
            ups = m_pData[m_iLine];
            m_pData[m_iLine] = '\0';

            m_cha.cpMax = m_dAnfang + m_iLine;

            if( IsKeyWord( m_pData + m_cha.cpMin - m_dAnfang ) )
            {
                SetColor( m_pT->m_crKeyword );

                m_cha.cpMin = m_cha.cpMax;
                m_cha.cpMax += 1;
                SetColor( m_pT->m_crForeground );
            }
            else
                SetColor( m_pT->m_crForeground );

            m_pData[m_iLine] = ups;
            m_iWord = 0;
        }

        if( m_pData[m_iLine] == '*' && ( m_pData[m_iLine+1] == '%' ||
                                     m_pData[m_iLine+1] == '+' )
          )
        {
            //nur am Anfang einer Zeile beachten!
            if( m_iLine == 0 || m_pData[m_iLine-1] == '\n' ||
                                m_pData[m_iLine-1] == '\r' )
            {
                lMerk = m_iLine;
                m_iLine += 2;

                while( m_pData[m_iLine] && m_pData[m_iLine] != '\r' )
                    m_iLine++;

                m_cha.cpMin = m_dAnfang + lMerk;
                m_cha.cpMax = m_dAnfang + m_iLine;
                SetColor( m_pT->m_crComment );
            }
        }

        if( m_pData[m_iLine] )
        {
            m_cha.cpMin = m_dAnfang + m_iLine;
            m_cha.cpMax = m_cha.cpMin + 1;
            SetColor( m_pT->m_crForeground );

            m_iLine++;
        }
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      IsKeyWord
**
** .Beschreibung: Ist ein Wort ein Keywort ?
**
** .Parameter:    char*   , pWord     , I, Pointer auf zu untersuchendes Wort
**
** .Rueckgabewert: BOOL, TRUE -> es ist ein Keyword
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode hashed das Word, erhaelt so einen Pointer auf die Key-
**      tabelle und vergleicht dann ob die Worte identisch sind.
**-----------------------------------------------------------------
*/
BOOL SironSyntax::IsKeyWord( char * pWord )
{
    long x = Hash( pWord );

    while( m_pHash[x] )
    {
        if( ! lstrcmp( m_pHash[x], pWord ) )
            return TRUE;
        x = (x+1) % HASH_ARR_SIZE;
    }
    return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      Hash
**
** .Beschreibung: Zerhacken eines Wortes
**
** .Parameter:    char*   , pWord     , I, Pointer auf Wort
**
** .Rueckgabewert: long, der Hashwert
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Leider muss das Wort zuerst in Grossbuchstaben  umgesetzt werden
**-----------------------------------------------------------------
*/
inline long SironSyntax::Hash( char *pWord )
{
    unsigned long h;

    CharUpper( pWord );

    for( h=0; *pWord; pWord++ )
        h = (64*h + *pWord) % HASH_ARR_SIZE;
    return h;
}


/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      ParseAll
**
** .Beschreibung: Den gesamten Text parsen
**
** .Parameter:    char*   , pdata     , I, Pointer auf zu untersuchendes Text
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode sucht im gesamten Text nach Keywords oder Kommentaren. Sie
**      wird z.b. beim Laden eines Files aufgerufen.
**-----------------------------------------------------------------
*/
void SironSyntax::ParseAll(char *pData )
{
    m_pData = pData;
    m_iLine = 0;
    m_dEnde = m_cha.cpMax;
    m_dAnfang = m_cha.cpMin;

    long dMerk;
    char ups;

    ::SendMessage( m_hWnd, WM_SETREDRAW, FALSE, 0L );
    m_cha.cpMin = 0;
    m_cha.cpMax = -1;       //alles faerben!
    SetColor( m_pT->m_crForeground );

    do
    {
        if( m_pData[m_iLine] == '\'' ) //Textstrings ueberlesen
        {
            m_iLine++;

            while( m_pData[m_iLine] && m_pData[m_iLine] != '\'' &&
                   m_pData[m_iLine] != '\r' )
                m_iLine++;
        }

        dMerk = m_iLine;

        while( IsCharAlphaNumeric( m_pData[m_iLine] ) || m_pData[m_iLine] == '_' ||
               m_pData[m_iLine] == '-' || m_pData[m_iLine] == '/' )
        {
            m_iLine++;
        }

        if( dMerk != m_iLine )
        {
            ups = m_pData[m_iLine];
            m_pData[m_iLine] = '\0';

            if( IsKeyWord( m_pData + dMerk ) )
            {
                m_cha.cpMin = dMerk;
                m_cha.cpMax = m_iLine;

                ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
                m_pT->m_cf.crTextColor = m_pT->m_crKeyword;
                ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_WORD | SCF_SELECTION,
                                   (LPARAM) &m_pT->m_cf );
            }

            m_pData[m_iLine] = ups;
        }


        if(  m_pData[m_iLine] == '*' &&
            ( m_pData[m_iLine+1] == '%' || m_pData[m_iLine+1] == '+' ) )
        {
            if( m_iLine == 0 || m_pData[m_iLine-1] == '\n' ||
                                m_pData[m_iLine-1] == '\r' )
            {
                dMerk = m_iLine;

                while( m_pData[m_iLine] && m_pData[m_iLine] != '\r' )
                    m_iLine++;

                m_cha.cpMax = m_iLine;
                m_cha.cpMin = dMerk;
                ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
                m_pT->m_cf.crTextColor = m_pT->m_crComment;
                ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &m_pT->m_cf );
            }
        }

    } while( m_pData[m_iLine++] );

    ::SendMessage( m_hWnd, WM_SETREDRAW, TRUE, 0L );
    InvalidateRect( m_hWnd, NULL, FALSE );
}



/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      RTFParseAll
**
** .Beschreibung: Den gesamten Text parsen und in RTF - Format umwandeln
**
** .Parameter: char* , pQuelle   , I, Pointer auf zu untersuchenden Text
**             char* , pZiel     , I, Pointer auf Schreibpuffer
**             char* , pNormalFarbe   , I, Pointer auf RTF Farbstring
**             char* , pKeywordFarbe  , I, Pointer auf RTF Farbstring
**             char* , pKommentarFarbe, I, Pointer auf RTF Farbstring
**             char* , pZeile    , I, Pointer auf RTF NewLineString
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode sucht im gesamten Text nach Keywords oder Kommentaren. Sie
**      wird beim Laden eines Files aufgerufen. Sie wandelt den Text in
**      RichText Format um. Dies beschleunigt das Laden.
**-----------------------------------------------------------------
*/
void SironSyntax::RTFParseAll( char *pQuelle, char *pZiel, char *pNormalFarbe,
                                char *pKeywordFarbe, char *pKommentarFarbe,
                                char *pZeile )
{
    char *pMerk;
    char ups;
    char cHilf[KEY_ARR_SIZE+2];
    BOOL bInTextString = FALSE;

    long lKommStringLen = lstrlen( pKommentarFarbe );
    long lKeyStringLen = lstrlen( pKeywordFarbe );
    long lNormStringLen = lstrlen( pNormalFarbe );
    long lZeilStringLen = lstrlen( pZeile );


    do
    {
        if( bInTextString ) //bin ich in einem TextString
        { // -> alle Zeichen uberlesen
            while( *pQuelle && *pQuelle != '\'' && *pQuelle != '\r' )
            {
                if( *pQuelle == '{' || *pQuelle == '}' || *pQuelle == '\\' )
                    *pZiel++ = '\\';

                *pZiel++ = *pQuelle++;
            }

            if( ! *pQuelle  ) //alle anderen Faelle werden weiter unten gehandelt
                continue;
        }

        pMerk = pQuelle;

        while( IsCharAlphaNumeric( *pQuelle ) || *pQuelle == '_' ||
               *pQuelle == '-' || *pQuelle == '/' )
        {
            pQuelle++;
        }

        if( pMerk != pQuelle )
        {
            ups = *pQuelle;
            *pQuelle = '\0';
            lstrcpyn( cHilf, pMerk, KEY_ARR_SIZE +1 );

            if( IsKeyWord( cHilf ) )
            {
                lstrcpy( pZiel, pKeywordFarbe );
                pZiel += lKeyStringLen;
                lstrcpy( pZiel, pMerk );
                pZiel += lstrlen( pMerk );
                lstrcpy( pZiel, pNormalFarbe );
                pZiel += lNormStringLen;
            }
            else
            {
                lstrcpy( pZiel, pMerk );
                pZiel += lstrlen( pMerk );
            }

            *pQuelle = ups;
        }


        //ANfang eines Zeilenkommentars
        if( *pQuelle == '*' && ( *(pQuelle+1) == '%' || *(pQuelle+1) == '+' ) )
        {
            lstrcpy( pZiel, pKommentarFarbe );
            pZiel += lKommStringLen;

            do
            {
                if( *pQuelle == '{' || *pQuelle == '}' || *pQuelle == '\\' )
                    *pZiel++ = '\\';

                *pZiel++ = *pQuelle++;
            } while( *pQuelle && *pQuelle != '\r' );

            lstrcpy( pZiel, pNormalFarbe );
            pZiel += lNormStringLen;
        }

        //bin ich am Zeilenende?
        if( *pQuelle =='\r' )
        {
            bInTextString = FALSE;

            while( *pQuelle && *pQuelle == '\r' )
                pQuelle++;

            *pZiel++ = '\r';

            while( *pQuelle && *pQuelle == '\n' )
                pQuelle++;

            *pZiel++ = '\n';
            lstrcpy( pZiel, pZeile );
            pZiel += lZeilStringLen;

            pQuelle--;
            continue;
        }


        //spezielle RTF - Zeichen durch '\' abschirmen
        if( *pQuelle == '{' || *pQuelle == '}' || *pQuelle == '\\' )
        {
            *pZiel++ = '\\';
            continue;
        }

        if( *pQuelle )
        {
            if( *pQuelle == '\'' )
                bInTextString = ! bInTextString;
            *pZiel++ = *pQuelle;
        }

    } while( *pQuelle++ );
}


/*
**----------------------------------------------------------------------
** .Klasse:       SironSyntax
**
** .Methode:      SetColor
**
** .Beschreibung: Faerben eines Bereichs
**
** .Parameter:  COLORREF , cRef , I, Farbwert
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Das Setzen der Farben mit EM_SETCHARFORMAT ist sehr
**      zeitaufwendig, weshalb zuerst untersucht wird, ob schon die
**      richtige Farbe vorhanden ist.
**-----------------------------------------------------------------
*/
void SironSyntax::SetColor( COLORREF cRef )
{
    ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
    ::SendMessage( m_hWnd, EM_GETCHARFORMAT, TRUE, (LPARAM) &m_cfo );
    m_pT->m_cf.crTextColor = cRef;
    if( ( ! (m_cfo.dwMask & CFM_COLOR) ) || m_cfo.crTextColor != cRef )
        ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &m_pT->m_cf );
}
