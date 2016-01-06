/*
**----------------------------------------------------------------------
** .Klasse:        TBRegistry
**
** .Version:       01A
**
** .Aenderung:     15.05.1998
**
** .Beschreibung:  Hilfsklasse fuer NT/Win95 Registry
**
** .Basisklassen:
**
** .Querverweise:
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:  Die Klasse stellt Methoden zur Bearbeitung
**      der Windows-Registry zur Verfuegung ( Erzeugen/Lesen/Schreiben
**      neuer/bestehender Schluessel ).
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 1999.11.04   DKS   Z10   Cosmetic (so Brief can read it).
** 01A  =  Neuerstellung
**         (15.05.1998)  SL
**
**----------------------------------------------------------------------
*/



/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "hzocregs.h"




/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      Konstruktor
**
** .Beschreibung:
**
** .Parameter:  char*,  pSubKey, I, der SubKey
**              unsigned long,  hKeyAst,  I, der Reg-Ast
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**    Initialisiert Registry-Bearbeitung:
**        pSubKey -> Eintrag in einem Baum der Registry ,
**                   z.B. \\Software\\Ton Beller
**        hKeyAst -> Legt Baum fest, Standard = HKEY_LOCAL_MACHINE
**-----------------------------------------------------------------
*/
TBRegistry::TBRegistry( char *pSubKey, unsigned long hKeyAst  )
{
    hKey = NULL;
    hAst = (HKEY) hKeyAst;

    int len = lstrlen( pSubKey ) + 2;
    pRegSubKey = new char [len];
    lstrcpy( pRegSubKey, pSubKey );

    rSAM = REG_NO;
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
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
**          Freigabe der Instanzressourcen
**-----------------------------------------------------------------
*/
TBRegistry::~TBRegistry( void )
{
    if( hKey )
        RegCloseKey( hKey );

    if( pRegSubKey )
        delete [] pRegSubKey;
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      CreateKey
**
** .Beschreibung: Anlegen/Oeffnen eines Keys
**
** .Parameter:
**
** .Rueckgabewert:  BOOL -> TRUE == Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          Versucht den im Konstruktor genannten Key zu oeffnen. Existiert
**          er nicht wird er mit allen Zugriffsrechten angelegt.
**          Es ist moeglich einen Keypfad zu nennen, der bis zu 3 nicht
**          existierende Keys enthaelt.
**-----------------------------------------------------------------
*/
BOOL TBRegistry::CreateKey()
{
    DWORD dDisposition;

    if( ERROR_SUCCESS == RegCreateKeyEx( hAst, pRegSubKey, 0, REG_NONE ,
                            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
                            NULL /*SEC-Desc.*/, &hKey, &dDisposition ) )
    {
        rSAM = KEY_ALL_ACCESS;
        return TRUE;
    }
    else
    {
        hKey = NULL;
        return FALSE;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      OpenKey
**
** .Beschreibung: Oeffnen eines existierenden Keys
**
** .Parameter:      TB_REGISTRY_ENUM, enSec, I,  gewuenschte Zugriffsrechte
**
** .Rueckgabewert:  BOOL -> TRUE == Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          enSec ist mit LESEN vorinitialisiert.
**-----------------------------------------------------------------
*/
BOOL TBRegistry::OpenKey( TB_REGISTRY_ENUM enSec )
{
    if( enSec == REG_ALL )
        rSAM = KEY_ALL_ACCESS;
    else if ( enSec == REG_WRITE )
        rSAM = KEY_WRITE;
    else
        rSAM = KEY_READ;

    if( ERROR_SUCCESS == RegOpenKeyEx( hAst, pRegSubKey, 0, rSAM, &hKey ) )
        return TRUE;
    else
    {
        hKey = NULL;
        return FALSE;
    }
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      DeleteKey
**
** .Beschreibung: Loeschen eines Keys
**
** .Parameter:
**
** .Rueckgabewert:  BOOL -> TRUE == Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Loescht den im Konstruktor angegebenen Key und aller Subkeys.
**      Ist kein Open gemacht worden, geschieht dies automatisch
**-----------------------------------------------------------------
*/
BOOL TBRegistry::DeleteKey()
{
    if( ! hKey )
        if( ! OpenKey( REG_ALL ) )
            return FALSE;

    if( ERROR_SUCCESS == RegDeleteKey( hKey, pRegSubKey ) )
    {
        hKey = NULL;
        return TRUE;
    }
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      ReadValue
**
** .Beschreibung: Lesen eines Werts in einen Key
**
** .Parameter:      char*   , pValue, I, Name des Wertes
**                  DWORD*  , dTyp  , U, DatenTyp des Wertes in der Re.g.
**                  BYTE*   , pZiel , O, Puffer fuer Ergebnis
**                  DWORD*  , dSize , U, Groesse des Puffers
**
** .Rueckgabewert:  BOOL -> TRUE == Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          Bemerkung zu dem Parameter dTyp: mögliche Werte sind in windows.h
**          festgelegt, z.B:  REG_DWORD, REG_SZ ...
**
**-----------------------------------------------------------------
*/
BOOL TBRegistry::ReadValue( const char *pValue, DWORD *dTyp, BYTE *pZiel, DWORD *dSize )
{
    if( ERROR_SUCCESS == RegQueryValueEx( hKey, pValue, 0, dTyp, pZiel, dSize ) )
        return TRUE;
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      WriteValue
**
** .Beschreibung: Schreiben eines Werts in einen Key
**
** .Parameter:      char*   , pValue, I, Name des Wertes
**                  DWORD   , dTyp  , I, DatenTyp des Wertes in der Re.g.
**                  BYTE*   , pZiel , I, Zeiger auf zu schreibende Daten
**                  DWORD   , dSize , I, Groesse der Daten in Byte
**
** .Rueckgabewert:  BOOL -> TRUE == Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**          Bemerkung zu dem Parameter dTyp: mögliche Werte sind in windows.h
**          festgelegt, z.B:  REG_DWORD, REG_SZ ...
**
**-----------------------------------------------------------------
*/
BOOL TBRegistry::WriteValue( const char *pValue, DWORD dTyp, const BYTE *pData,
                             DWORD dSize )
{
    if( ERROR_SUCCESS == RegSetValueEx( hKey, pValue, 0, dTyp, pData, dSize ) )
        return TRUE;
    else
        return FALSE;
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegistry
**
** .Methode:      DeleteValue
**
** .Beschreibung: Loeschen eines Werts in einen Key
**
** .Parameter:      char*   , pValue, I, Name des Wertes
**
** .Rueckgabewert:  BOOL -> TRUE == Erfolg
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**
**-----------------------------------------------------------------
*/
BOOL TBRegistry::DeleteValue( const char *pValue )
{
    if( ERROR_SUCCESS == RegDeleteValue( hKey, pValue ) )
        return TRUE;
    else
        return FALSE;
}
