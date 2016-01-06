#ifndef _HZOEUNDO_H_
#define _HZOEUNDO_H_

/*
**----------------------------------------------------------------------
** .Klasse:        CUndo
**
** .Version:       01B
**
** .Aenderung:     11.03.1999
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
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  Neue Methoden CanUndo() und CanRedo()
**         (11.03.1999)  SL
** 01C  =  Cosmetic (so Brief can read it)
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
#include "richedit.h"



/*----------------------------------------------------------------------
** .typdefs
**----------------------------------------------------------------------
*/


enum EDIT_ACTION { LEER=0, INSCHAR, DELCHAR, INSCHARDEL, INSRANGE,
                   DELRANGE, INSRANGEDEL, DELRANGEBYCHAR };


struct _UndoElem{
    DWORD dAnfang;          //char-index
    EDIT_ACTION enAction;   //abgespeicherte Editieraktion
    char cCharIns[3];       //eingefuegter Buchstabe
    char cCharDel[3];       //geloeschter Buchstabe
    char *pIns;             //Zeiger auf eingefuegten TextRange
    char *pDel;             //Zeiger auf geloeschten TextRange
    _UndoElem *pPrev;
    _UndoElem *pNext;
};


typedef struct _UndoElem UndoElem;



class CUndo
{
private:
    HWND m_hEdit;           //das EditControl
    UndoElem *m_pUrsprung;  //der Anker

    long m_lAnzElem;        //Anzahl der Ringelemente
    BOOL m_bStop;           //merkt, ob UNDO auf Beginn-Zeiger steht
    UndoElem *m_pInsert;    //aktuelle InsertPosition
    UndoElem *m_pUndo;      //aktuelle Undo/Redo - Position
    UndoElem *m_pBegin;     //aktuelle Anfangsposition des Rings

public:
    CUndo( HWND hEdit, long lAnzElem = 30 );
    ~CUndo();

private:
    void NewAction( void );

public:
    DWORD MakeUndo();
    DWORD MakeRedo();

    void Initialize();

    char *MakeNewBuffer( DWORD dAnfPos, DWORD dEndePos );
    char *MakeNewBuffer( LPCTSTR lpcValue );
    void DeleteNewBuffer( char* pBuffer );

    void InsertCharacter( DWORD dPos, char cIns ); //INSCHAR
    void InsertCharacter( DWORD dPos, char cIns, char cDel ); //INSCHARDEL

    void InsertRange( DWORD dPos, char *pInsRange ); //INSRANGE
    void InsertRange( DWORD dPos, char *pInsRange, char *pDelRange ); //INSRANGEDEL

    void DeleteCharacter( DWORD dPos, char cDel ); //DELCHAR
    void DeleteRange( DWORD dPos, char *pDelRange ); //DELRANGE
    void DeleteRangeByChar( DWORD dPos, char *pDelRange, char cIns ); //DELRANGEBYCHAR

    BOOL CanUndo()  { if ( m_bStop || m_pUndo->enAction == LEER )
                         return FALSE;
                      else
                         return TRUE;
                    }
    BOOL CanRedo()  { return m_pInsert == m_pUndo ? FALSE : TRUE; }
};

#endif
