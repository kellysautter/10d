#ifndef _HZOCREGS_H_
#define _HZOCREGS_H_


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
**      Die Klasse kann momentan nur auf einen Key und seine Werte
**      zugreifen!
**
**      Aufrufschritte: 1. Konstruktur legt Key fest
**                      2. Open oder Create des Keys
**                      3. ReadValue/WriteValue
**                      4. Destruktor schlieﬂt Key
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
#include <windows.h>


/*----------------------------------------------------------------------
** .typdefs
**----------------------------------------------------------------------
*/
enum TB_REGISTRY_ENUM { REG_NO, REG_READ, REG_WRITE, REG_ALL };



class TBRegistry
{
private:
    HKEY hAst;              //der Handle des Registry-Asts
    HKEY hKey;              //Handle des geoeffneten Keys innerhalb des Asts
    char *pRegSubKey;       //Name des Keys
    REGSAM rSAM;            //die Access-Methode ( siehe TB_REGISTRY_ENUM )

public:
    TBRegistry( char *pSubKey, unsigned long hKeyAst =(unsigned long) HKEY_LOCAL_MACHINE  );
    ~TBRegistry();

    BOOL CreateKey();
    BOOL OpenKey( TB_REGISTRY_ENUM enSec = REG_READ );
    BOOL DeleteKey();
    BOOL ReadValue( const char *pValue, DWORD *dTyp, BYTE *pZiel, DWORD *dSize );
    BOOL WriteValue( const char *pValue, DWORD dTyp, const BYTE *pData, DWORD dSize );
    BOOL DeleteValue( const char *pValue );
};

#endif
