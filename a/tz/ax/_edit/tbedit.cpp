/*
**----------------------------------------------------------------------
** .Klasse:        CTBEditApp
**
** .Version:       01D
**
** .Aenderung:     18.12.1998
**
** .Beschreibung:  Implementation of CTBEditApp and DLL registration
**
** .Basisklassen:  COleControlModule
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      Hauptklasse des TBEditor Controls. Uebernimmt die Initialisierung
**      des Controls und dessen Registrierung.
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (15.05.1998)  SL
** 01B  =  Erweiterung der Registry-Eintraege um Undo und C-Style Kommentare
**         (24.07.1998)  SL
** 01C  =  Registry-Eintraege nach HKEY_CURRENT_USER\Software
**         (20.10.1998)  SL
** 01D  =  Voreinstellungen fuer Kommentarfarbe und Schriftfarbe geaendert
**         (18.12.1998)  SL, TMV
** 01E  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
** 01F  =  Beibehalten von Nutzereinträgen in der Registry bei Update Installationen
**         RAD-NR.: 053856
**         (27.00.2000)  TMV
**----------------------------------------------------------------------
*/



/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "tbedit.h"
#include "hzoestri.h"
#include "hzocregs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTBEditApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
      { 0xdea843d2, 0xdaa2, 0x11d1, { 0x95, 0xb2, 0, 0x60, 0x97, 0x3b, 0x3b, 0x55 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CTBEditApp::InitInstance - DLL initialization

BOOL CTBEditApp::InitInstance()
{
   BOOL bInit = COleControlModule::InitInstance();

   return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CTBEditApp::ExitInstance - DLL termination

int CTBEditApp::ExitInstance()
{
   return COleControlModule::ExitInstance();
}

BOOL CTBEditApp::PreTranslateMessage( MSG* pMsg )
{
   TRACE1("CTBEditApp::PreTranslateMessage( MSG* pMsg = %d )\n",pMsg->message);
   return COleControlModule::PreTranslateMessage( pMsg );
}



/////////////////////////////////////////////////////////////////////////////
/*  Die Funktion wird nicht direkt aufgerufen -> hierzu gibt es
    z.B. das Tool regsvr32.exe.
    Zum Erzeugen der benutzerdefinierten Registry-Eintraege unter "HKEY_CURRENT_
    USER/SOFTWARE/Ton Beller/TBEditor" wird die Klasse TBRegistry genutzt.

    Um zu vermeiden das alte Eintrage verloren gehen wird zuerst versucht
    ein Eintrag zu lesen, bevor er geschrieben wird
*/

STDAPI DllRegisterServer(void)
{
   AFX_MANAGE_STATE(_afxModuleAddrThis);

   TBRegistry * pReg = new TBRegistry( REGISTRYKEY, (unsigned long)HKEY_CURRENT_USER );

   if( pReg->CreateKey() )
   {
      DWORD dTyp;
      DWORD dSize;
      DWORD dWert;
      CString strFont = "Courier";
      LPSTR lpszFont = NULL;
      COLORREF cr;

      //Schriftart -> Arial
      strFont = "Courier";
      dSize = strFont.GetLength() + 1;
      lpszFont = strFont.GetBufferSetLength( MAX_PATH + 1 );
      dSize = MAX_PATH;

      if( !pReg->ReadValue( REGFONT, &dTyp, (LPBYTE)lpszFont, &dSize ) )
      {
         strFont.ReleaseBuffer();
         strFont = "Courier";
         dTyp = REG_SZ;
         dSize = strFont.GetLength() + 1;
         pReg->WriteValue( REGFONT, dTyp, (LPBYTE)(LPCSTR)strFont, dSize );
      }

      //Schriftgroesse -> 9
      dTyp = REG_DWORD;
      dSize = sizeof ( dWert );
      dWert = 180;
      if ( !pReg->ReadValue( REGSIZE, &dTyp, (LPBYTE)&dWert, &dSize ) )
      {
         dWert = 180;
         dTyp = REG_DWORD;
         dSize = sizeof ( dWert );
         pReg->WriteValue( REGSIZE, dTyp, (LPBYTE)&dWert, dSize );
      }

      //Hintergrundfarbe -> Weiß
      dTyp = REG_DWORD;
      dSize = sizeof ( cr );
      if ( !pReg->ReadValue( REGBACK, &dTyp, (LPBYTE)&cr, &dSize ) )
      {
         dTyp = REG_DWORD;
         dSize = sizeof ( cr );
         cr = RGB( 255, 255, 255 );
         pReg->WriteValue( REGBACK, dTyp, (LPBYTE)&cr, dSize );
      }

      //Vordergrund -> Schwarz
      dTyp = REG_DWORD;
      dSize = sizeof ( cr );
      if ( !pReg->ReadValue( REGFORE, &dTyp, (LPBYTE)&cr, &dSize ) )
      {
         dTyp = REG_DWORD;
         dSize = sizeof ( cr );
         cr = RGB( 0,0,0 );
         pReg->WriteValue( REGFORE, dTyp, (LPBYTE)&cr, dSize );
      }

      //KeywordFarbe -> Rot
      dTyp = REG_DWORD;
      dSize = sizeof ( cr );
      if ( !pReg->ReadValue( REGWORD, &dTyp, (LPBYTE)&cr, &dSize ) )
      {
         dTyp = REG_DWORD;
         dSize = sizeof ( cr );
         cr = RGB( 255, 0, 0 );
         pReg->WriteValue( REGWORD, dTyp, (LPBYTE)&cr, dSize );
      }

      //Kommentarfarbe -> Blau
      dTyp = REG_DWORD;
      dSize = sizeof ( cr );
      if ( !pReg->ReadValue( REGCOMMENT, &dTyp, (LPBYTE)&cr, &dSize ) )
      {
         dTyp = REG_DWORD;
         dSize = sizeof ( cr );
         cr = RGB( 0, 0, 255 );
         pReg->WriteValue( REGCOMMENT, dTyp, (LPBYTE)&cr, dSize );
      }

      //Tabstop-Weite -> 4
      dTyp = REG_DWORD;
      dSize = sizeof ( dWert );
      if ( !pReg->ReadValue( REGTABSTOP, &dTyp, (LPBYTE)&dWert, &dSize ) )
      {
         dTyp = REG_DWORD;
         dWert = 4;
         pReg->WriteValue( REGTABSTOP, dTyp, (BYTE*)&dWert, dSize );
      }

      //UndoLevel -> 30
      dTyp = REG_DWORD;
      dSize = sizeof ( dWert );
      if ( !pReg->ReadValue( REGUNDOLEVEL, &dTyp, (LPBYTE)&dWert, &dSize ) )
      {
         dTyp = REG_DWORD;
         dWert = 30;
         pReg->WriteValue( REGUNDOLEVEL, dTyp, (BYTE*)&dWert, dSize );
      }

      //Anzahl Zeilen fuer Kommentarpuffer -> 100
      dTyp = REG_DWORD;
      dSize = sizeof ( dWert );
      if ( !pReg->ReadValue( REGBUFFERSIZE, &dTyp, (LPBYTE)&dWert, &dSize ) )
      {
         dTyp = REG_DWORD;
         dWert = 100;
         pReg->WriteValue( REGBUFFERSIZE, dTyp, (BYTE*)&dWert, dSize );
      }
   }

   delete pReg;

   if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
   return ResultFromScode(SELFREG_E_TYPELIB);

   if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
   return ResultFromScode(SELFREG_E_CLASS);

   return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
   AFX_MANAGE_STATE(_afxModuleAddrThis);

    TBRegistry * pReg = new TBRegistry( REGISTRYKEY, (unsigned long)HKEY_CURRENT_USER );
    pReg->DeleteKey();
    delete pReg;

   if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
      return ResultFromScode(SELFREG_E_TYPELIB);

   if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
      return ResultFromScode(SELFREG_E_CLASS);

   return NOERROR;
}
