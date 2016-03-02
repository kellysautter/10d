#ifndef _HZOESYN_H_
#define _HZOESYN_H_

/*
**----------------------------------------------------------------------
** .Klasse:        SyntaxColoring, ZeidonSyntax, SironSyntax
**
** .Version:       01B
**
** .Aenderung:     25.06.1998
**
** .Beschreibung:  Klassen fuer das Syntax Coloring
**
** .Basisklassen:
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      SyntaxColoring ist die abstrakte Basisklasse von der die beiden
**      anderen Klassen abgeleitet sind. Diese Konstruktion wurde gewählt,
**      um eine einheitliche Ansprache aus dem Control heraus sicherzustellen.
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  Erweiterung fuer C-Syntax Kommentare
**         (25.06.1998)  SL
** 01C  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**----------------------------------------------------------------------
*/


/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include <windows.h>
#include "richedit.h"


/*----------------------------------------------------------------------
** .typdefs
**----------------------------------------------------------------------
*/
enum SYNTAX_SPRACHE { NO_SYNTAX, SIRON_SYNTAX, ZEIDON_SYNTAX };

#define HASH_ARR_SIZE  241    //dieser Wert wird fuer den HASH-Algo. verwendet

//#define HASH_ARR_SIZE  2311    //dieser Wert wird fuer den HASH-Algo. verwendet
                              //es sollte sich um eine Primzahl handeln
                              //die Zahl sollte wesentlich groesser sein, als die
                              //Anzahl der Keywords
#define KEY_ARR_SIZE   30


class CTBEditor; //forward declaration


class SyntaxColoring
{
public:
    SyntaxColoring( CTBEditor *pT, char *pData, HWND hWnd )
        {   m_pT = pT;     m_pData = pData;     m_hWnd = hWnd;
            m_cfo.cbSize = zsizeof( CHARFORMAT );
        };

    virtual ~SyntaxColoring() {};
   virtual void ParseLine(char *) = 0;
    virtual void ParseAll( char *) = 0;
    virtual void RTFParseAll( char *, char*, char*, char*, char*, char* ) = 0;
    virtual BOOL EndsLineInComment() { return FALSE; };

protected:

    CTBEditor *m_pT;
    char *m_pData;
    HWND m_hWnd;

    CHARFORMAT m_cfo;   //Struktur fuer Getcharformat
    DWORD m_dAnfang;    //Merker fuer Anfang der Zeile als zero-based CharIndex
    DWORD m_dEnde;      //Merker fuer Ende der Zeile als zero-based CharIndex
    int m_iWord;        //WordCounter
    int m_iLine;        //Index innerhalb der Zeile
public:
    CHARRANGE m_cha;    //Anfang und Ende der Zeile als zero-based CharIndex
                        // wird von aussen gesetzt!!
};



class ZeidonSyntax: public SyntaxColoring
{
public:
    ZeidonSyntax( CTBEditor *pT, char *pData, HWND hWnd );

    virtual ~ZeidonSyntax();
    virtual void ParseLine(char*);
    virtual void ParseAll(char*);
    virtual void RTFParseAll( char *, char*, char*, char*, char*, char* );
    virtual BOOL EndsLineInComment() { return m_bEndWithComment; };

private:
    char **m_pHash;         //Zeiger auf Hashtabelle
//    char *m_pBigKeyword;    //Zeiger auf das aktuell gefundene Keyword
    CHARRANGE m_chMerk;     //merkt die Position des letzen Keywords
    BOOL m_bMerkComment;    //merkt den letzten Zustand des Kommentarpuffers
    DWORD m_dMerkComLine;   //merkt den letzten ZeilenIndex des Kommentarbuffers
    BOOL m_bEndWithComment; //merkt, ob die letzte akt. Zeile mit offenen Kommentar endet

    inline long Hash( char * );
    inline void SetColor( COLORREF );
    BOOL ParseRestOfRange();
    void TestCommentPuffer( BOOL bComment );
    void ParseTextBuffer( char*, long );
    inline BOOL IsKeyWord( char * pWord );
    CMapStringToString m_mapHashTable;
    CString m_pBigKeyword;
};



class SironSyntax: public SyntaxColoring
{
public:
    SironSyntax( CTBEditor *pT, char *pData, HWND hWnd, BOOL bLanguage );

    virtual ~SironSyntax();
    virtual void ParseLine(char*);
    virtual void ParseAll(char*);
    virtual void RTFParseAll( char *, char*, char*, char*, char*, char* );

private:
    BOOL m_bLanguage; // TRUE->deutsch , else englisch
    char *m_pArray;       // Zeiger auf HZ115x - Tabelle
    char (*m_pKeys)[KEY_ARR_SIZE];    // Zeiger auf Keyword-Tabelle
    char **m_pHash;       //Zeiger auf Hashtabelle

    inline long Hash( char * );
    inline BOOL IsKeyWord( char * pWord );
    inline void SetColor( COLORREF );
};


#endif
