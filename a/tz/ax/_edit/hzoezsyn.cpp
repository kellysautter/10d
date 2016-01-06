/*
**----------------------------------------------------------------------
** .Klasse:        ZeidonSyntax
**
** .Version:       01A
**
** .Aenderung:     20.05.1998
**
** .Beschreibung:  Klasse fuer das Zeidon Syntax Coloring
**
** .Basisklassen:  SyntaxColoring
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      ZeidonSyntax fuehrt das Syntax Coloring fuer Zeidonabfragen durch.
**      Keywords werden aus der Tabelle des hzoezsyn.h extrahiert.
**
**      Die Keyword-Suche erfolgt ueber HashKeys. D.h. es gibt einen Hash-
**      Array, der Pointer auf die entsprechende Keywords enthaelt.
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  Erweiterung fuer C-Syntax Kommentar
**         (23.06.1998)  SL
** 01C  =  Stop setting keywords that are part of a V.E.A from
**         being translated to uppercase.
**         (2002.11.14)  DKS
**
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
//#include <sys/timeb.h>
#include "stdafx.h"
#include "tbedit.h"
#include "hzoesyn.h"
#include "hzoectl.h"


#include "hzoezsyn.h"   // die Keyword-Tabelle





/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      Konstruktor
**
** .Beschreibung: Initialisierung der Keyword-Tabelle
**
** .Parameter:  CTBEditor* , pT        , I, Pointer auf CTBEditor Instanz
**              char*      , pData     , I, Pointer auf Puffer mit Text
**              HWND       , hWnd      , I, Handle des RichEdits
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Aufbau des Hasharrays und der Keywordtabelle.
**-----------------------------------------------------------------
*/
ZeidonSyntax::ZeidonSyntax( CTBEditor *pT, char *pData, HWND hWnd )
    :SyntaxColoring( pT, pData, hWnd )
{
//    m_pBigKeyword = NULL;
    m_pBigKeyword.Empty();
    m_chMerk.cpMin = m_chMerk.cpMax = 0;
    m_dMerkComLine = 0xFFFFFFFF;
    m_bMerkComment = FALSE;
    m_bEndWithComment = FALSE;

    m_pHash = new char* [HASH_ARR_SIZE];

    long cZeidonSize = sizeof( cZeidon ) / KEY_ARR_SIZE;
    int i;

    for( i = 0; i < HASH_ARR_SIZE; i++ )
        m_pHash[i] = NULL;

    //Hash-Tabelle fuellen
    for( i = 0; i < cZeidonSize; i++ )
    {
        int x = Hash( cZeidon[i] );

        while( m_pHash[x] )
            x = (x+1) % HASH_ARR_SIZE;

        m_pHash[x] = cZeidon[i];
    }
/*
    m_mapHashTable.InitHashTable( HASH_ARR_SIZE );
    for( i = 0; i < cZeidonSize; i++ )
    {
        m_mapHashTable[cZeidon[i]] = cZeidon[i];
    }
*/

}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      Destruktor
**
** .Beschreibung: Freigabe der Ressourcen
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
ZeidonSyntax::~ZeidonSyntax( void )
{
    if( m_pHash )
        delete  m_pHash;
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
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
inline long ZeidonSyntax::Hash( char *pWord )
{
    unsigned long h;

    CharUpper( pWord );

    for( h=0; *pWord; pWord++ )
        h = (64*h + *pWord) % HASH_ARR_SIZE;
    return h;
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
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
**      tabelle und vergleicht dann ob die Worte identisch sind. Bei
**      Identitaet wird das Keyword in m_pBigKeyword gemerkt.
**-----------------------------------------------------------------
*/
BOOL ZeidonSyntax::IsKeyWord( char * pWord )
{
//   struct _timeb tstruct;
//   _ftime( &tstruct );
//   TRACE1( "Plus milliseconds:\t\t\t%u\n", tstruct.millitm );
//   BOOL bReturn = m_mapHashTable.Lookup( pWord, m_pBigKeyword );
//   return bReturn;
   long x = Hash( pWord );

   while( m_pHash[x] )
   {
       if( ! lstrcmp( m_pHash[x], pWord ) )
       {
           m_pBigKeyword = m_pHash[x]; //Zeiger auf Keyword eintragen
           return TRUE;
       }
       x = (x+1) % HASH_ARR_SIZE;
   }
   return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      ParseLine
**
** .Beschreibung: Untersuchen des Eingabebereichs ( 1 oder mehrere Zeilen )
**
** .Parameter:  char*      , pData     , I, Pointer auf Puffer mit Text
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode untersucht den aktuelle Eingabebereich auf Keywords und
**      Kommentare. Ist der Bereich Teil eines Kommentars, wird zuerst nach
**      dem Kommentarende gesucht. Gegebenfalls  wird die Methode
**      ParseRestOfRange aufgerufen. Auf jeden Fall wird mit TestCommentBuffer
**      der nachfolgende Textbereich untersucht.
**-----------------------------------------------------------------
*/
void ZeidonSyntax::ParseLine( char *pData )
{
    m_pData = pData;
    m_dEnde = m_cha.cpMax;
    m_dAnfang = m_cha.cpMin;
    m_iLine = 0;

    if( m_pT->m_bComment ) // vorherige Zeile ist ein C-Style Kommentar
    {
        //Kommentarende in Zeile suchen
        while( m_pData[m_iLine] && ( m_pData[m_iLine] != '*' || m_pData[m_iLine+1] != '/' ) )
                m_iLine++;

        if( ! m_pData[m_iLine] ) //alles Kommentar
        {
            SetColor( m_pT->m_crComment );
            TestCommentPuffer( TRUE );
            m_bEndWithComment = TRUE;
            return;
        }
        else //C-style Kommentar ist in der Zeile zu Ende
        {
            m_iLine += 2;

            //stimmen die Farben
            m_cha.cpMax = m_dAnfang + m_iLine;
            SetColor( m_pT->m_crComment );

            //neue Anfangswerte setzen
            m_dAnfang = m_dAnfang + m_iLine;
            m_cha.cpMin = m_dAnfang;
            m_cha.cpMax = m_dEnde;
            m_pData = m_pData + m_iLine;
        }
    }

    if( ParseRestOfRange() )
    {
        m_bEndWithComment = TRUE;
        TestCommentPuffer( TRUE );
    }
    else
    {
        TestCommentPuffer( FALSE );
        m_bEndWithComment = FALSE;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
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
void ZeidonSyntax::SetColor( COLORREF cRef )
{
    ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
    ::SendMessage( m_hWnd, EM_GETCHARFORMAT, TRUE, (LPARAM) &m_cfo );
    m_pT->m_cf.crTextColor = cRef;
    if( ( ! (m_cfo.dwMask & CFM_COLOR) ) || m_cfo.crTextColor != cRef )
        ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &m_pT->m_cf );
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      TestCommentPuffer
**
** .Beschreibung: Untersuchung des Kommentarranges hinter dem Eingabebereich
**
** .Parameter:    BOOL , bComment , I, TRUE->Range beginnt innerhalb eines
**                                           Kommentars
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode behandelt den Range hinter dem Eingabebereich, um
**      auf das Oeffnen, bzw. Schliessen eines C-Style Kommentars
**      reagieren zu koennen. ( Wird im Eingabebereich ein Kommentar
**      geoeffnet, muss der nachfolgende Text entsprechend gefaerbt
**      werden ). Die Groesse des Kommentarranges wird von der KLasse
**      CTBEditor festgelegt ( abgelegt in der Registry!).
**      Um nicht jedesmal neu zu parsen und zu faerben, versucht die
**      Methode herauszufinden, ob schon richtige Verhaeltnisse
**      herrschen. Dies geschieht ueber die Faerbung der ersten beiden
**      Zeichen innerhalb der Zeilen. Zu beachten ist hierbei, dass ein
**      C++-Style Kommentar keinerlei Aussagen gestattet und deshalb
**      ueberlesen wird.
**
**      Verwendete Variablen der Klasse CTBEditor:
**          m_dComSize      -> Anzahl Zeichen im Kommentarpuffer
**          m_dComLine      -> Index der ersten erfassten Zeile
**          m_pCommentBuf   -> Pointer auf den Kommentarpuffer
**          m_dComNumberOfLines -> Anzahl Zeilen im Kommentarpuffer
**-----------------------------------------------------------------
*/
void ZeidonSyntax::TestCommentPuffer( BOOL bComment )
{
    CHARRANGE cha;
    char *p;
    DWORD dZeile = 0;

    if( m_pT->m_dComSize == 0 )
        return;

    if( m_pT->m_dComLine == m_dMerkComLine && bComment == m_bMerkComment )
        return;
    else
    {
        m_dMerkComLine = m_pT->m_dComLine;
        m_bMerkComment = bComment;
    }

    //zuerst Anfangsindex des Kommentarbuffers bestimmen
    m_cha.cpMin = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                (WPARAM)m_pT->m_dComLine, 0L );

    if( bComment ) // Beginn innerhalb eines C-Style Kommentars?
    {
        do
        {
            //characterindex der zu untersuchenden Zeile bestimmen
            cha.cpMin = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                        (WPARAM)m_pT->m_dComLine + dZeile, 0L );
            dZeile++;

            //AnfangsPosition der Zeile innerhalb des Kommentarbuffers bestimmen
            p =   m_pT->m_pCommentBuf + ( cha.cpMin - m_cha.cpMin );

            if( *p == '/' && *(p+1) == '/' ) //beginnt die Zeile mit C++ Kommentar?
            { // ja->dann suchen ob ein C-Style Kommentarende inder Zeile ist
                p += 2;
                long lZaehler = 4; //der C++-Komm. + das C-Komm.Ende
                while( *p && *p != '\r' && ( *p != '*' || *(p+1) != '/' ) )
                {
                    lZaehler ++;
                    p++;
                }
                if( (! *p ) || *p == '\r' ) //keins gefunden ->naechste Zeile untersuchen
                    continue;
                else // !! der Kommentar endet in dieser Zeile !!!
                { //dann parse den ganzen Mist einfach
                    ParseTextBuffer( m_pT->m_pCommentBuf+lZaehler, m_cha.cpMin+lZaehler );
                    return;
                }
            }

            //Kein C++! -> ist sie als Kommentar gefaerbt?
            cha.cpMax = cha.cpMin + 2;
            ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &cha );
            ::SendMessage( m_hWnd, EM_GETCHARFORMAT, TRUE, (LPARAM) &m_cfo );
            if( !(m_cfo.dwMask & CFM_COLOR) ||
                m_cfo.crTextColor != m_pT->m_crComment )
            { //nein! -> nochmals parsen
              //zuerst wird das Kommentarende gesucht. Wenn in der Zeile mit
              //Kommentarende vorher ein Zeilenkommentar oder ein Stringanfang
              //steht, muss der Rest neu gecolored werden
                BOOL bLineComment = FALSE;
                BOOL bInTextString = FALSE;

                while( *p && ( *p != '*' || *(p+1) != '/' ) )
                {
                    if( *p == '\r' ) //neue Zeile
                        bLineComment = bInTextString = FALSE;
                    else if( *p == '/' && *(p+1) == '/' )
                        bLineComment = TRUE;
                    else if( *p == '"' )
                        bInTextString =  !bInTextString;
                    p++;
                }

                DWORD dLen = p - m_pT->m_pCommentBuf;

                if( dLen >= m_pT->m_dComSize ) //wurde ein Kommentarende gefunden?
                {//nein
                    m_cha.cpMax = m_cha.cpMin + dLen;
                    SetColor( m_pT->m_crComment );
                }
                else
                { // ja -> Endeposition + 2 ( p steht auf dem '*' )
                    m_cha.cpMax = m_cha.cpMin + dLen + 2;
                    SetColor( m_pT->m_crComment );
                    //wurde in vorhergehenden WHILE etwas gravierendes gefunden
                    if( bInTextString || bLineComment )
                        ParseTextBuffer( p + 2, m_cha.cpMax ); //ab dem Kommentarende
                                                               //parsen
                }
            }
            return;
        } while( dZeile < m_pT->m_dComNumberOfLines );
    }
    else
    { //es muss festgestellt werden, ob der nachfolgende Text richtig gefaerbt ist
      //d.h. wenn er als Kommentar gefaerbt ist, muss er wieder geparst werden.
      //Dies wird ueber die Faerbung der ersten beiden Zeichen bestimmt
      //allerdings vorsicht bei C++-Style Kommentar!!
        do
        {
            //characterindex der zu untersuchenden Zeile bestimmen
            cha.cpMin = ::SendMessage( m_hWnd, EM_LINEINDEX,
                                        (WPARAM)m_pT->m_dComLine + dZeile, 0L );
            dZeile++;

            //AnfangsPosition der Zeile innerhalb des Kommentarbuffers bestimmen
            p =   m_pT->m_pCommentBuf + ( cha.cpMin - m_cha.cpMin );

            if( *p == '/' && *(p+1) == '/' ) //beginnt die Zeile mit C++ Kommentar?
                continue;
            else
            { //nein! -> ist sie als Kommentar gefaerbt?
                cha.cpMax = cha.cpMin + 2;
                ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &cha );
                ::SendMessage( m_hWnd, EM_GETCHARFORMAT, TRUE, (LPARAM) &m_cfo );
                if(  (m_cfo.dwMask & CFM_COLOR) &&
                     m_cfo.crTextColor == m_pT->m_crComment )
                { //ja! -> nochmals parsen falls kein C-Style Kommentar
                    if( *p && *p != '/' && *(p+1) != '*' )
                        ParseTextBuffer( m_pT->m_pCommentBuf, m_cha.cpMin );
                }
                break;
            }
        } while( dZeile < m_pT->m_dComNumberOfLines );
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      ParseRestOfRange
**
** .Beschreibung: Untersuchen des Eingabebereichs
**
** .Parameter:
**
** .Rueckgabewert:  BOOL-> TRUE, wenn Kommentar ueber das Ende des
**                               Bereichs reicht
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode wird von ParseLine aufgerufen um den Eingabebereich
**      oder Reste des Eingabebereiches auf notwendiges Coloring zu
**      untersuchen.
**-----------------------------------------------------------------
*/
BOOL ZeidonSyntax::ParseRestOfRange()
{
    m_iWord = 0;
    m_iLine = 0;
    char ups;
    long lMerk;

    while( m_pData[m_iLine] )
    {
        m_cha.cpMin = m_dAnfang + m_iLine;

        if( m_pData[m_iLine] == '"' )
        {
            lMerk = m_iLine++;

            while( m_pData[m_iLine] && m_pData[m_iLine] != '"' &&
                   m_pData[m_iLine] != '\r' )
                m_iLine++;

            m_cha.cpMax = m_dAnfang + m_iLine;
            SetColor( m_pT->m_crForeground );
            m_cha.cpMin = m_cha.cpMax;
        }

        while( IsCharAlphaNumeric( m_pData[m_iLine] ) || m_pData[m_iLine] == '_' )
        {
            m_iWord++;
            m_iLine++;
        }

        if( m_iWord )
        {
            ups = m_pData[m_iLine];
            m_pData[m_iLine] = '\0';

            m_cha.cpMax = m_dAnfang + m_iLine;

            // Stop setting keywords that are part of a V.E.A from
            // being translated to uppercase (DKS).
            if( (m_iLine <= m_iWord ||
                 m_pData[ m_cha.cpMin - m_dAnfang - 1 ] != '.') &&
                IsKeyWord( m_pData + m_cha.cpMin - m_dAnfang ) )
            {
                m_chMerk = m_cha;
                SetColor( m_pT->m_crKeyword );

                m_cha.cpMin = m_cha.cpMax;
                m_cha.cpMax += 1;
                SetColor( m_pT->m_crForeground );

                if( ups == ' ' && m_pT->m_chAlt.cpMin == (long) (m_dAnfang + m_iLine) )
                {
                    if( m_iLine ) //&& m_pT->m_bInsertMode )
                    {
                        ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_chMerk );
                        ::SendMessage( m_hWnd, EM_REPLACESEL, FALSE,(LPARAM) (LPCSTR)m_pBigKeyword );
                    }
                }
            }
            else
            {
                SetColor( m_pT->m_crForeground );
            }

            m_pData[m_iLine] = ups;
            m_iWord = 0;
        }

        if( ! m_pData[m_iLine] )
            return FALSE;

        if( m_pData[m_iLine] == '/' && m_pData[m_iLine+1] == '*' )
        {
            m_cha.cpMin = m_dAnfang + m_iLine;

            m_iLine += 2;

            //Zeile nach KommentarEnde durchsuchen
            while( m_pData[m_iLine] && ( m_pData[m_iLine] != '*' || m_pData[m_iLine+1] != '/' ) )
                m_iLine++;

            if( m_pData[m_iLine] ) //Kommentarende innerhalb der Zeile
            {
                m_iLine += 2;
                m_cha.cpMax = m_dAnfang + m_iLine;

                SetColor( m_pT->m_crComment );

                if( !m_pData[m_iLine] ) //Kommentarende am Ende der Zeile
                    return FALSE;
            }
            else
            {
                m_cha.cpMax = m_dEnde;
                SetColor( m_pT->m_crComment );
                return TRUE;
            }
        }

        // ist es ein C++-Syntax Kommentar
        if( m_pData[m_iLine] == '/' && m_pData[m_iLine+1] == '/' )
        {
            m_cha.cpMin = m_dAnfang + m_iLine;

            m_iLine += 2;

            while( m_pData[m_iLine] && m_pData[m_iLine] != '\r' )
                m_iLine++;

            m_cha.cpMax = m_dAnfang + m_iLine;
            SetColor( m_pT->m_crComment );

            if( ! m_pData[m_iLine] )
                return FALSE;
        }


        if( m_pData[m_iLine] )
        {
            //pruefen ob nicht-charakter Zeichen richtige Farbe  haben
            m_cha.cpMin = m_dAnfang + m_iLine;
            m_cha.cpMax = m_cha.cpMin + 1;
            SetColor( m_pT->m_crForeground );

            m_iLine++;
        }
    }

    return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      ParseAll
**
** .Beschreibung: Den gesamten Text parsen
**
** .Parameter:    char*   , pdata     , I, Pointer auf zu untersuchenden Text
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode setzt zuerst die normale Farbe fuer den gesamten Text
**      und ruft dann ParseTextBuffer auf. Sie wird beim Syntaxwechsel
**      aufgerufen, um einen schon geladenen Text umzuwandeln.
**-----------------------------------------------------------------
*/
void ZeidonSyntax::ParseAll( char *pData )
{
    ::SendMessage( m_hWnd, WM_SETREDRAW, FALSE, 0L );
    m_cha.cpMin = 0;
    m_cha.cpMax = -1;       //alles faerben!
    SetColor( m_pT->m_crForeground );
    ParseTextBuffer( pData, 0 );
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
**
** .Methode:      ParseTextBuffer
**
** .Beschreibung: Schnelles Parsen eines Textpuffers
**
** .Parameter:    char*, pdata   , I, Pointer auf zu untersuchenden Text
**                long , lVersatz, I, Versatzwert, wenn der Pufferanfang
**                                    nicht mit dem Textanfang ueberein-
**                                    stimmt ( fuer EM_EXSETSEL ! )
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Die Methode sucht den gesamten Puffer nach Keywords und Kommentaren.
**      Sie wird von ParseText unt TestCommentPuffer aufgerufen.
**-----------------------------------------------------------------
*/
void ZeidonSyntax::ParseTextBuffer( char* pData, long lVersatz )
{
    m_pData = pData;
    m_iLine = 0;
    long dMerk;
    char ups;


    ::SendMessage( m_hWnd, WM_SETREDRAW, FALSE, 0L );

    //nur gebraucht fuer TestCommentPuffer
    m_cha.cpMin = lVersatz;
    m_cha.cpMax = lVersatz + m_pT->m_dComSize;
    SetColor( m_pT->m_crForeground );

    do
    {
        if( m_pData[m_iLine] == '"' ) //Textstrings ueberlesen
        {
            m_iLine++;

            while( m_pData[m_iLine] && m_pData[m_iLine] != '"' &&
                   m_pData[m_iLine] != '\r' )
                m_iLine++;
        }

        dMerk = m_iLine;

        while( IsCharAlphaNumeric( m_pData[m_iLine] ) || m_pData[m_iLine] == '_' )
        {
            m_iLine++;
        }

        if( dMerk != m_iLine )
        {
            ups = m_pData[m_iLine];
            m_pData[m_iLine] = '\0';

            if( IsKeyWord( m_pData + dMerk ) )
            {
                m_cha.cpMin = dMerk + lVersatz;
                m_cha.cpMax = m_iLine + lVersatz;

                ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
                m_pT->m_cf.crTextColor = m_pT->m_crKeyword;
                ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION,
                                   (LPARAM) &m_pT->m_cf );
            }

            m_pData[m_iLine] = ups;
        }

        if( m_pData[m_iLine] == '/' && m_pData[m_iLine+1] == '*' )
        {
            dMerk = m_iLine;

            m_iLine += 2;

            while( m_pData[m_iLine] && ( m_pData[m_iLine] != '*' || m_pData[m_iLine+1] != '/' ) )
                m_iLine++;
            //mache Kommentar
            if( m_pData[m_iLine] )
            { //bei Textende darf m_iLine nicht mehr erhoeht werden! -> else-Fall
                m_iLine++;
                m_cha.cpMax = m_iLine+1+lVersatz;
            }
            else
                m_cha.cpMax = m_iLine+2+lVersatz;

            m_cha.cpMin = dMerk+lVersatz;
            ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
            m_pT->m_cf.crTextColor = m_pT->m_crComment;
            ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &m_pT->m_cf );
            continue;
        }

        if( m_pData[m_iLine] == '/' && m_pData[m_iLine+1] == '/' )
        {
            dMerk = m_iLine;

            m_iLine += 2;

            while( m_pData[m_iLine] && m_pData[m_iLine] != '\r' )
                m_iLine++;

            m_cha.cpMax = m_iLine+lVersatz;
            m_cha.cpMin = dMerk+lVersatz;
            ::SendMessage( m_hWnd, EM_EXSETSEL, 0,(LPARAM) &m_cha );
            m_pT->m_cf.crTextColor = m_pT->m_crComment;
            ::SendMessage( m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &m_pT->m_cf );
            continue;
        }

    } while( m_pData[m_iLine++] );

    ::SendMessage( m_hWnd, WM_SETREDRAW, TRUE, 0L );
    InvalidateRect( m_hWnd, NULL, FALSE );
}


/*
**----------------------------------------------------------------------
** .Klasse:       ZeidonSyntax
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
void ZeidonSyntax::RTFParseAll( char *pQuelle, char *pZiel, char *pNormalFarbe,
                                char *pKeywordFarbe, char *pKommentarFarbe,
                                char *pZeile )
{
    char *pMerk;
    char ups;
    char cHilf[500]; //[KEY_ARR_SIZE+2];
    BOOL bInTextString = FALSE;

    long lKommStringLen = lstrlen( pKommentarFarbe );
    long lKeyStringLen = lstrlen( pKeywordFarbe );
    long lNormStringLen = lstrlen( pNormalFarbe );
    long lZeilStringLen = lstrlen( pZeile );


    do
    {
        if( bInTextString ) //bin ich in einem TextString
        { // -> alle Zeichen uberlesen
            while( *pQuelle && *pQuelle != '"' && *pQuelle != '\r' )
            {
                if( *pQuelle == '{' || *pQuelle == '}' || *pQuelle == '\\' )
                    *pZiel++ = '\\';

                *pZiel++ = *pQuelle++;
            }

            if( ! *pQuelle  ) //alle anderen Faelle werden weiter unten gehandelt
                continue;
        }

        pMerk = pQuelle;

        while( IsCharAlphaNumeric( *pQuelle ) || *pQuelle == '_' )
        {
            pQuelle++;
        }

        if( pMerk != pQuelle )
        {
            ups = *pQuelle;
            *pQuelle = '\0';
            lstrcpyn( cHilf, pMerk, 498 ); //KEY_ARR_SIZE +1 );

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


        if( *pQuelle == '/' && *(pQuelle+1) == '*' )
        {
            lstrcpy( pZiel, pKommentarFarbe );
            pZiel += lKommStringLen;

            do
            {
                if( *pQuelle == '{' || *pQuelle == '}' || *pQuelle == '\\' )
                    *pZiel++ = '\\';

                *pZiel++ = *pQuelle++;

                if( *pQuelle == '\r' )
                {
                    while( *pQuelle && *pQuelle == '\r' )
                        pQuelle++;

                    *pZiel++ = '\r';

                    while( *pQuelle && *pQuelle == '\n' )
                        pQuelle++;

                    *pZiel++ = '\n';
                    lstrcpy( pZiel, pZeile );
                    pZiel += lZeilStringLen;
                }

            } while( *pQuelle && ( *pQuelle != '*' || *(pQuelle+1) != '/' ) );

            if( *pQuelle )
            { //noch das Kommentarende uebertragen
                *pZiel++ = *pQuelle++;
                *pZiel++ = *pQuelle++;
            }
            lstrcpy( pZiel, pNormalFarbe );
            pZiel += lNormStringLen;
        }

        //ANfang eines Zeilenkommentars
        if( *pQuelle == '/' && *(pQuelle+1) == '/' )
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
        if( *pQuelle == '\r' )
        {
            bInTextString = FALSE;

            *pZiel++ = '\r';

            if( *( pQuelle + 1 ) == '\n' )
                pQuelle++;

            *pZiel++ = '\n';
            lstrcpy( pZiel, pZeile );
            pZiel += lZeilStringLen;

            continue;
        }


        //bin ich am Zeilenende? ( Zeile nicht durch \r\n
        //gekennzeichnet sondern nur durch \n )
        if( *pQuelle =='\n' )
        {
            bInTextString = FALSE;

            *pZiel++ = '\r';
            *pZiel++ = '\n';

            lstrcpy( pZiel, pZeile );
            pZiel += lZeilStringLen;

            continue;
        }


        //spezielle RTF - Zeichen durch '\' abschirmen
        if( *pQuelle == '{' || *pQuelle == '}' || *pQuelle == '\\' )
        {
            *pZiel++ = '\\';
            *pZiel++ = *pQuelle;
            continue;
        }

        if( *pQuelle )
        {
            if( *pQuelle == '"' )
                bInTextString = ! bInTextString;
            *pZiel++ = *pQuelle;
        }

    } while( *pQuelle++ );
}

