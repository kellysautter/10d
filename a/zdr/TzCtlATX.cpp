/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2000 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlatx.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C++ classes for Zeidon Painter Tools
// standard painter controls.

/* Change log most recent first order:

   2002.02.26 TMV   2000   ActiveX
      Changed WritePersistProperties to store license key only if avilable

   2001.10.23 DKS   2000   ActiveX
      Changed WritePersistProperties to create and use its own view.

   2001.4.4   TMV   2000   ActiveX
      make a fix to TZActiveX::BuildLicKeyString because Mfc-TRACE-Macros
      are limited to 512 bytes

   2001.3.26   TMV   2000   ActiveX
      Store license key in pwd and create control using this
      new method:
         ZActiveX::BuildLicKeyString ( for debugging only )

   2001.2.26   TMV   2000   ActiveX
      If painter requests attribute count we return 1000 which is stupid
      because the painter will create 1000 Mappings

   2000.09.07  TMV    Z10   ActiveX
      WritePersistProperties
      First try to save properties via IPersistStream
      IPersistStream Interface is not supported use IPersistStorage

   2000.09.06  TMV    Z10   ActiveX
      Initialize variable pMemPtr in WritePersistProperties
      to avoid crash if no memory is allocated for retrieving
      persistent properties

   2000.06.13  TMV    Z10   ActiveX
      Changed CreateActiveXWithPersistProperties
      and     WritePersistProperties
      to simplify control creation and fix the bug that shows the
      control in its initial state ( unable to add columns )

   2000.01.28  TMV    Z10   ActiveX
      removed NULL pointer check in LoadPersistProperties
      because this check prevents porperty loading

   1999.12.14  TMV    Z10   ActiveX
      check com pointers against NULL before using it
      related function: WritePersistProperties.

   1999.09.15  DKS    Z10   ActiveX
      Reorganizing code in preparation of retrieving parameters for
      ActiveX events.

   1999.07.21  TMV 10
      Add some functions to have a dependency checking prior to call
      code generation to avoid code generation for versioned interfaces

   1999.07.19  TMV 10
      Change code for generating global op's
      related class ZGlobalOperation
      related method GetCPPDefinition(...)

*/

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "TzCtl.h"
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL
// #define TZAX_TRACE_BLOBS
// #define TZAX_TRACE_OI


zLONG g_lID = 1000;  // ctrl id for ActiveX controls

//////////////////////////////////////////////////////////////////////////////////
//
// class TZAmbientProperty for handling Ambient properties
//
//////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( TZAmbientProperty, CObject, 1 );

TZAmbientProperty::TZAmbientProperty() :
                   m_dispid( 0 ),
                   m_tEnabled( FALSE )
{
}

TZAmbientProperty::TZAmbientProperty( DISPID dispid, LPCTSTR pszName,
                                      const VARIANT& var, int vti,
                                      BOOL tStock ) :
                   m_dispid( dispid ),
                   m_csName( pszName ),
                   m_varValue( var ),
                   m_vti( vti ),
                   m_tEnabled( TRUE ),
                   m_tStock( tStock )
{
}

void
TZAmbientProperty::Enable( BOOL tEnabled )
{
   m_tEnabled = tEnabled;
}

DISPID
TZAmbientProperty::GetID() const
{
   return( m_dispid );
}

CString
TZAmbientProperty::GetName() const
{
   return( m_csName );
}

const
COleVariant& TZAmbientProperty::GetValue() const
{
   return( m_varValue );
}

int
TZAmbientProperty::GetVTI() const
{
   return( m_vti );
}

BOOL
TZAmbientProperty::IsEnabled() const
{
   return( m_tEnabled );
}

BOOL
TZAmbientProperty::IsStock() const
{
   return( m_tStock );
}

void
TZAmbientProperty::SetValue( const VARIANT& varValue, int vti )
{
   m_varValue = varValue;
/*
   if ( vti == -1 )
   {
      m_vti = VTToVTI( varValue.vt );
   }
   else
   {
      ASSERT( VTIToVT( vti ) == varValue.vt );
      m_vti = vti;
   }
*/
}

void
TZAmbientProperty::Serialize( CArchive& ar )
{
   CObject::Serialize( ar );

   if ( ar.IsStoring() )
   {
     ar<<m_dispid;
     ar<<m_csName;
     ar<<m_varValue;
     ar<<m_vti;
     ar<<m_tEnabled;
     ar<<m_tStock;
   }
   else
   {
     ar>>m_dispid;
     ar>>m_csName;
     ar>>m_varValue;
     ar>>m_vti;
     ar>>m_tEnabled;
     ar>>m_tStock;
   }
}

//////////////////////////////////////////////////////////////////////////////////
//
// class TZActiveX
//
//////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC( TZActiveX, CWnd )

BEGIN_MESSAGE_MAP( TZActiveX, CWnd )
   //{{AFX_MSG_MAP(TZActiveX)
   ON_WM_RBUTTONDOWN( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

#ifdef DEBUG_ALL
   int TZActiveX::m_iInstanceCount = 0;
#endif


TZActiveX::TZActiveX( TZPainterWindow *pPainterWindow,
                      TZPainterCtrl   *pCtrlParent,
                      CWnd    *pWndParent,
                      CRect&  rect,
                      zVIEW   vCtrl,
                      zLONG   lType,
                      zBOOL   bPlaceHolder,
                      zLONG   lZKey,
                      zLONG   lTabStopNbr,
                      zLONG   lCtrlID,
                      zPCHAR  pchTag,
                      zPCHAR  pchText ) :
           CWnd( ),
           TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                          rect, vCtrl, this, pchTag, pchText,
                          lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   m_iInstanceCount++;
   CString csTrace = "+++ TZActiveX::ctor ";
   csTrace += m_csTag;
   csTrace += " No. of instances ";
   TraceLineI( csTrace, m_iInstanceCount );
#endif

   // Retrieve ini setting for debug purpose.
   m_bTraceAmbientPropertyRequest = FALSE;
   m_bTraceBlob                   = FALSE;
   m_bTraceDisplayEntityInstance  = FALSE;
   m_csStorageFileName.Empty();

   zVIEW   vTZPESRCO;

   CString strHelp;
   LPSTR pszStorageFile = strHelp.GetBufferSetLength( zMAX_FILENAME_LTH );
   SysReadZeidonIni( -1, "[ActiveX]", "TraceBlob", pszStorageFile, zMAX_FILENAME_LTH );
   strHelp.ReleaseBuffer();
   if ( strHelp.CompareNoCase( "y" ) == 0 )
   {
      m_bTraceBlob = TRUE;
   }

   pszStorageFile = strHelp.GetBufferSetLength( zMAX_FILENAME_LTH );
   SysReadZeidonIni( -1, "[ActiveX]", "TraceDisplayEntityInstance", pszStorageFile, zMAX_FILENAME_LTH );
   strHelp.ReleaseBuffer();
   if ( strHelp.CompareNoCase( "y" ) == 0 )
   {
      m_bTraceDisplayEntityInstance = TRUE;
   }

   CString strTempDir;
   LPSTR lpszTempDir = strTempDir.GetBufferSetLength( MAX_PATH );
   ::GetEnvironmentVariable( _T("TEMP"), lpszTempDir, MAX_PATH );
   strTempDir.ReleaseBuffer();
   m_csStorageFileName = strTempDir;
   m_csStorageFileName += '\\';
   m_csStorageFileName += m_csTag;
   m_csStorageFileName += ".stg";

   // Initialize control interface pointers controls ptrs.
   m_lpObject     = NULL;
   m_lpOleControl = NULL;

   // Initialize members for connectable interfaces.
   m_dwPropNotifySink = 0;
   m_bPropertiesChanged = FALSE;

   m_bSameAsLoaded  = FALSE;
   m_bstrLicenseKey = 0;
   m_bLicensed      = FALSE;

   if ( GetViewByName( &vTZPESRCO, szlTZPESRCO,
                       m_pPainterWindow->m_vDialog, zLEVEL_TASK ) > 0 &&
        SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef,
                                       szlKey, lType, 0 ) >= zCURSOR_SET )
   {
      zPCHAR  pchGUID;

      GetAddrForAttribute( &pchGUID, vTZPESRCO, szlControlDef, "GUID" );
      m_csClassID = pchGUID;
      CreateZ( );
   }
}

TZActiveX::~TZActiveX( )
{
#ifdef DEBUG_ALL
   m_iInstanceCount--;
   CString csTrace = "--- TZActiveX::dtor ";
   csTrace += m_csTag;
   csTrace += " No. of instances ";
   TraceLineI( csTrace, m_iInstanceCount );
#endif
   DestroyActiveX();
   if ( m_bLicensed )
      SysFreeString( m_bstrLicenseKey );
}

void
TZActiveX::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::CreateZ ", m_csTag );
   TRACE1( "*** TZActiveX::CreateZ %s\n", m_csTag );
#endif

   BOOL    bCreateControl;
   HRESULT hResult = 0;

   // ActiveX Control is on the Window.
   // Now Create the Control via CreateControl.

   bCreateControl = CreateActiveXObjectWithPersistProperties( );
   if ( bCreateControl == FALSE )
   {
      TraceLineS( "Unable to create ActiveX : ", m_csTag );
   }
}

zBOOL
TZActiveX::IsModeValid( zULONG ulMode )
{
   if ( ulMode & zCTRL_MODE_DIRECT_EDIT )
      return( TRUE );

   return( TZPainterCtrl::IsModeValid( ulMode ) );
}

// Get the number of Entity/Attribute definitions from the ActiveX control.
zSHORT
TZActiveX::GetAttributeCount( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::GetAttributeCount ", m_csTag );
#endif

   zSHORT nCount = 0;

   // We must call the appropriate script here!
   LPLIBRARY lpLibrary = 0;
   zVIEW  vTaskLPLR;
   zVIEW  vPE;
   zPCHAR pchScript;
   zPCHAR pchScriptDLL;

   GetViewByName( &vTaskLPLR, "TaskLPLR",
                  m_pPainterWindow->m_vDialog, zLEVEL_TASK );
   GetViewByName( &vPE, szlTZPESRCO, m_pPainterWindow->m_vDialog, zLEVEL_TASK );

   SetCursorFirstEntityByInteger( vPE, szlControlDef, szlKey, m_lType, 0 );
   GetAddrForAttribute( &pchScript, vPE, szlControlDef, "ActiveX_Script" );
   GetAddrForAttribute( &pchScriptDLL, vPE,
                        szlControlDef, "ActiveX_ScriptDLL" );

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
      GetOperationDynamicCallAddress( vTaskLPLR, (LPLPLIBRARY) &lpLibrary,
                                      pchScriptDLL, pchScript, "(ptx)" );
   if ( lpfnDynRoutine )
   {
      // Processing ActiveX Script to retrieve number of mapable attributes.
//    TraceLineS( "Processing ActiveX Script ", pchScript );
      nCount = (zSHORT) (*lpfnDynRoutine)( 0, 0, (ULONG) this,
                                           zACTIVEX_GET_ATTRIBUTE_COUNT );
   }

   return( nCount );
}

zSHORT
TZActiveX::CreateZeidonControl( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::GetAttributeCount ", m_csTag );
#endif

   zSHORT nRC = 0;

   // We must call the appropriate script here!
   LPLIBRARY lpLibrary = 0;
   zVIEW  vTaskLPLR;
   zVIEW  vPE;
   zPCHAR pchScript;
   zPCHAR pchScriptDLL;

   GetViewByName( &vTaskLPLR, "TaskLPLR", m_pPainterWindow->m_vDialog, zLEVEL_TASK );
   GetViewByName( &vPE, szlTZPESRCO, m_pPainterWindow->m_vDialog, zLEVEL_TASK );

   SetCursorFirstEntityByInteger( vPE, szlControlDef, szlKey, m_lType, 0 );
   GetAddrForAttribute( &pchScript, vPE, szlControlDef, "ActiveX_Script" );
   GetAddrForAttribute( &pchScriptDLL, vPE,
                        szlControlDef, "ActiveX_ScriptDLL" );

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
      GetOperationDynamicCallAddress( vTaskLPLR, (LPLPLIBRARY) &lpLibrary,
                                      pchScriptDLL, pchScript, "(ptx)" );
   if ( lpfnDynRoutine )
   {
      // Processing ActiveX Script to retrieve number
      // Zeidon Control
      nRC = (zSHORT) (*lpfnDynRoutine)( 0, 0, (ULONG) this,
                                        zACTIVEX_CREATE_ZEIDON_CONTROL );
   }

   return( nRC );
}

void
TZActiveX::ShowProperties( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::ShowProperties ", m_csTag );
#endif

   CRect rect;

   if ( GetIOleObjectInterfacePtr() )
   {
      GetClientRect( &rect );
      m_lpObject->DoVerb( OLEIVERB_PROPERTIES, 0, 0, 0,
                          m_pWndParent->m_hWnd, &rect );
      // properties are stored when object is destroyed
//      WritePersistProperties();

   }
} // ShowProperties

BOOL
TZActiveX::CreateActiveXObjectWithPersistProperties( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::CreateActiveXObjectWithPersistProperties ",
               m_csTag );
#endif

   CRect    rect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H );
   BOOL     bCreate = FALSE, bFromStorage = FALSE;
   zULONG   ulRequiredMemoryLth;
   zPVOID   pMemPtr = 0;         // Pointer to the global Memory used
                                 // for Loading the Properties
   CFile *f = 0;

   // This Function creates the ActiveX object using the persist properties.
   // First get the length of the blob.
   if ( m_vCtrl == 0 )
      ulRequiredMemoryLth = 0;
   else
   {
      GetAttributeLength( &ulRequiredMemoryLth, m_vCtrl,
                          szlControl, szlProperties );
   }

   if ( ulRequiredMemoryLth  > 0 )
   {
      // Now allocate memory for loading the properties.
      pMemPtr = new BYTE[ ulRequiredMemoryLth ];
      // Read the properties from the BLOB.
      GetBlobFromAttribute( pMemPtr, &ulRequiredMemoryLth,
                            m_vCtrl, szlControl, szlProperties );

      f = new CFile( m_csStorageFileName, CFile::modeCreate | CFile::modeWrite );
      f->Write( pMemPtr, ulRequiredMemoryLth);
      delete f;
      f = new CFile( m_csStorageFileName, CFile::modeRead );

      bFromStorage = TRUE;

      if ( m_bTraceBlob )
      {
         TraceBlob( "TZActiveX::CreateActiveXObjectWithPersistProperties: ",
                    pMemPtr, ulRequiredMemoryLth );
      }

      if ( m_bTraceDisplayEntityInstance )
      {
         if ( m_vCtrl )
         {
            TraceLineS( "***","" );
            TraceLineS( "TZActiveX::CreateActiveXObjectWithPersistProperties ",
                        "DisplayEntityInstance" );
            DisplayEntityInstance( m_vCtrl, szlControl );
         }
      }
   }

   //  switch to run mode
   TZPainterClient *pc = GetPainterClient();
   TZAmbientProperty *pAP = pc->FindAmbientProperty( DISPID_AMBIENT_USERMODE );

   COleVariant v;
   v.vt = VT_BOOL;
   v.boolVal = TRUE;
   pAP->SetValue( v );

   // Get the license key.
   if( m_bstrLicenseKey )
      ::SysFreeString( m_bstrLicenseKey );

   // Get the license key.
   m_bLicensed = RequestLicenseKey( m_bstrLicenseKey, m_csClassID );
   if( !m_bLicensed )
      ::SysFreeString( m_bstrLicenseKey );

#ifdef DEBUG_ALL
   BuildLicKeyString();
#endif
   // Now we try to create the control.
   bCreate = CreateControl( m_csClassID, 0,
                            WS_VISIBLE | WS_TABSTOP, rect,
                            m_pWndParent, g_lID++, f,
                            bFromStorage,
                            m_bLicensed ? m_bstrLicenseKey : NULL );

   if ( f )
      delete f;

   if ( pMemPtr )
      delete [] pMemPtr;

   //  switch back to designMode mode
   v.boolVal = FALSE;
   pAP->SetValue( v );
   if ( bCreate )
   {
      LPOLECONTROL pC = NULL;
      LPUNKNOWN    pU = NULL;
      pU = GetControlUnknown();
      pU->QueryInterface( IID_IOleControl, (LPVOID *) &pC );
      pC->OnAmbientPropertyChange( DISPID_AMBIENT_USERMODE );
      pC->Release();
   }

   if ( bCreate )
      ConnectPropertyNotifySink();

   return( bCreate );

} //CreateActiveXObjectWithPersistProperties

///////////////////////////////////////////////////////////////////////
//
//  The RequestLicenseKey function uses the IClassFactory2 interface
//  to request a License key from an object specified by its classid.
//
//  Parameters:
//
//    [out] BSTR& bstrLicenseKey
//          Upon return, this BSTR will contain either a valid
//          License key or an error message. It is the caller's
//          responsibility to call ::SysFreeString on this BSTR.
//
//    [in]  CString strProgID
//          Specifies the ProgID of the object from which to request the
//          License key.
//
//
//  Return Value:
//
//    A BOOL specifying success or failure. If TRUE is returned,
//    the License key was retrieved successfully and the
//    bstrLicenseKey parameter contains a valid License key. If
//    FALSE is returned, the License key was not retrieved
//    successfully and the bstrLicenseKey parameter contains a
//    descriptive error string.
//
//    Regardless of the return value, it is the responsibility of
//    the caller to call ::SysFreeString on the returned bstrLicenseKey
//    parameter.
//

BOOL TZActiveX::RequestLicenseKey(BSTR &bstrLicenseKey, CString strClsid)
{
   USES_CONVERSION;

   LPCLASSFACTORY2 pClassFactory;
   CLSID clsid;
   BOOL bValidKeyReturned = FALSE;
   CString strMsg;


   OLECHAR wszCLSID[40]; // should be more than large enough
   mbstowcs(wszCLSID, strClsid, 40);

  // Get the CLSID of the specified string.
  if (NOERROR == CLSIDFromString(wszCLSID, &clsid) )
  {
    // Create an instance of the object and query it for
    //  the IClassFactory2 interface.
    if (SUCCEEDED(CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, NULL,
      IID_IClassFactory2, (LPVOID *)(&pClassFactory))))
    {
      LICINFO licinfo;

      // Check to see if this object has a runtime License key.
      if (SUCCEEDED(pClassFactory->GetLicInfo(&licinfo)))
      {
        if (licinfo.fRuntimeKeyAvail)
        {
          HRESULT hr;

          // The object has a runtime License key, so request it.
          hr = pClassFactory->RequestLicKey(0, &bstrLicenseKey);

          if (SUCCEEDED(hr))
          {
            if(bstrLicenseKey == NULL)
              bstrLicenseKey = ::SysAllocString(
                L"<Object returned a NULL license key>");
            else
              // You have the license key.
              bValidKeyReturned = TRUE;
          }
          else
            // Requesting the License key failed.
            switch (hr)
            {
              case E_NOTIMPL:
                bstrLicenseKey = ::SysAllocString(
                  L"<The object's class factory does not support"
                  L" run-time license keys>");
                break;

              case E_UNEXPECTED:
                bstrLicenseKey = ::SysAllocString(
                  L"<An unexpected error occurred when requesting the"
                  L" run-time license key>");
                break;

              case E_OUTOFMEMORY:
                bstrLicenseKey = ::SysAllocString(
                  L"<The object's class factory was unable to allocate"
                  L" the license key>");
                break;

              case CLASS_E_NOTLICENSED:
                bstrLicenseKey = ::SysAllocString(
                  L"<The object's class factory supports run-time"
                  L" licensing, but the current machine\r\nitself is"
                  L" not licensed. Thus, a run-time key is not"
                  L" available on this machine>");
                break;

              default:
                bstrLicenseKey = ::SysAllocString(
                  L"<An unknown error occurred when requesting the"
                  L" license key>");
            }
        }
        else
          bstrLicenseKey = ::SysAllocString(
            L"<The object has no runtime license key>");
      }
      else
        bstrLicenseKey = ::SysAllocString(
          L"<Unable to get the licensing capabilities of the object's"
          L" class factory>");

      // Make sure you release the reference to the class factory.
      pClassFactory->Release();
    }
    else
      bstrLicenseKey = ::SysAllocString(
        L"<Unable to get the IClassFactory2 interface from the"
        L" specified object>");
  }
  else
    bstrLicenseKey = ::SysAllocString(
      L"<Unable to get the CLSID of the specified object>");

  // Return a BOOL specifying whether or not you were able to get a
  // valid license key.
  return bValidKeyReturned;

}

void TZActiveX::BuildLicKeyString()
{
   BYTE CurrentByte;
   BYTE * pKeyByteData;
   WCHAR CurrentWCHAR;
   WCHAR * pKeyWCHARData;

   unsigned int nByteCount, nWCHARCount, nIndex;
   zCHAR Temp[ 7 ];
   CString strHumanReadable;
   CString strTempLicenseString;

   // Get a pointer to the beginning of the key data.
   pKeyWCHARData = m_bstrLicenseKey;

   // Get the sizeof the license key data in bytes.
   nByteCount = SysStringByteLen(m_bstrLicenseKey);

   // Get the size of the license key data in WCHAR's.
   nWCHARCount = nByteCount / 2;

   // Initialize the WCHAR array declaration string.
   strTempLicenseString = "WCHAR pwchLicenseKey[] =\r\n{\r\n\t";

   // Initialize the human readable version of the license key.
   strHumanReadable = "/*\r\n";

   // Walk through the the license key data, WCHAR by WCHAR, generating
   //  hex formatted strings for each WCHAR value. Do appropriate formatting
   //  with carriage return/line feeds.
   for (nIndex = 0; nIndex < nWCHARCount; nIndex++)
   {
     // Get the current WCHAR of data.
     CurrentWCHAR = *(pKeyWCHARData + nIndex);

     // Create a hex formatted string for the current WCHAR.
     wsprintf(Temp, "0x%04X", CurrentWCHAR);

     // Add the hex formatted string to the end of the BYTE array declaration
     //  string.
     strTempLicenseString += Temp;

     // See if this is the last WCHAR of the license key data. If it isn't,
     //  add a comma and a tab to the end of the WCHAR array declaration string.
     //  We check to see if this is the last WCHAR of data because we don't
     //  need a comma after the hex string for the last WCHAR of the license
     //  key data.
     if(nIndex < (nWCHARCount - 1))
        strTempLicenseString += ",\t";

     // Check to see if this is the 6th hex string on this line. If it is,
     //  add a carriage return, line feed, and a tab to the WCHAR array
     //  declaration string.
     if(!((nIndex + 1) % 6) && (nIndex < (nWCHARCount - 1)))
        strTempLicenseString += "\r\n\t";
   }

   // Add a final carriage return/line feed followed by the closing brace and
   //  semicolon to the WCHAR array declaration string.
   strTempLicenseString += "\r\n};";

   // Walk through the the license key data, byte by byte, generating
   //  a human readable string. If the current byte is a printable
   //  character or a carriage return or a line feed character, add it
   //  to the human readable string. If it's not, insert a '.'
   //  placeholder into the human readable string.

   // Get a pointer to the beginning of the key data.
   pKeyByteData = (BYTE *)m_bstrLicenseKey;

   for (nIndex = 0; nIndex < nByteCount; nIndex++)
   {
      // Get the current byte of data
      CurrentByte = *(pKeyByteData + nIndex);

      if (isprint((int)CurrentByte) ||
          CurrentByte == 0x0D ||
          CurrentByte == 0x0A)
         strHumanReadable += (char)CurrentByte;
      else if (CurrentByte != 0)
         strHumanReadable += ".";
   }

   // Add a final carriage return/line feed and close the comment block for the
   //  human readable version of the license key.
   strHumanReadable += "\r\n*/";

   // Place the human readable string and the temp data declaration string into
   //  the final license key string.
   m_strLicenseKey = strHumanReadable;
   m_strLicenseKey += "\r\n\r\n";
   m_strLicenseKey += strTempLicenseString;
#ifdef DEBUG_ALL
   if( m_strLicenseKey.GetLength() <= 512 )// this is because TRACEXXX is limited to 512 Bytes
      TRACE0(_T( m_strLicenseKey ) );
   else
   {
      int nLoopCount = 0;
      LPCSTR lpszSrc = m_strLicenseKey;
      while ( *lpszSrc )
      {
         zCHAR szHelp[512];
         ::memset( szHelp, '\0', 512 );
         for ( int i = 0; i < 511 && *lpszSrc; i++, lpszSrc++ )
         {
            szHelp[i] = *lpszSrc;
         }
         TRACE0( _T( szHelp ) );
      }
   }
#endif
}


// unsing memory storage
BOOL
TZActiveX::LoadPersistProperties( )
{
   BOOL bReturn = TRUE;
   return bReturn;
}

BOOL
TZActiveX::QueryRequiredInterfaces()
{
   LPUNKNOWN pUnk = GetControlUnknown();
   BOOL bReturn = TRUE;
   HRESULT hr = NULL;
/*
   mReleaseInit( m_lpStorage );
   mReleaseInit( m_lpLockBytes );
   mReleaseInit( m_lpPersistStg );

   TRY
   {
      hr = pUnk->QueryInterface( IID_IPersistStorage,
                                 (LPVOID *) &m_lpPersistStg );
      if ( hr != S_OK )
      {
         AfxThrowOleException( hr );
      }

      // Creating a Storage in memory
      hr = CreateILockBytesOnHGlobal( NULL, TRUE, &m_lpLockBytes );
      if ( hr != S_OK )
      {
         AfxThrowOleException( hr );
      }

      hr = StgCreateDocfileOnILockBytes( m_lpLockBytes,
                                         STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                         0, &m_lpStorage );
      if ( hr != S_OK )
      {
         AfxThrowOleException ( hr );
      }

      m_bSameAsLoaded = FALSE;
   }
   CATCH (COleException, e )
   {
      bReturn = FALSE;
      e->ReportError();
   }
   END_CATCH
*/
   return( bReturn );
}

void
TZActiveX::WritePersistProperties( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::WritePersistProperties: ", m_csTag );
#endif

   HRESULT          hr               = E_FAIL;
   BYTE             *pMemPtr         = 0;
   DWORD            dwMemoryLth = 0;
   HGLOBAL          hMemProperty = 0;;
   LPPERSISTSTREAM  lpPersistStream  = NULL;
   LPPERSISTSTORAGE lpPersistStorage = NULL;
   LPSTORAGE        lpStorage        = NULL;
   LPSTREAM         lpStream         = NULL;


   // This function stores the Property-Values for the current ActiveX-Control
   // a BLOB

// if ( m_bPropertiesChanged == FALSE )       // PropertyNotifySink reports no changes
//    return S_OK;

   LPUNKNOWN pUnk = GetControlUnknown();
   if ( !pUnk )
      return;

   TRY
   {
      // First we check the Existence of IPersistStorage, because if there
      // is no IPersistStorage Interface we can't do anything.
      //hr = GetIOleObjectInterfacePtr()->QueryInterface( IID_IPersistStorage,
      //                                   (LPVOID *) &lpPersistStg );
      //if ( hr != S_OK )
      //   AfxThrowOleException( hr );

      // 1. Create an ILockBytes Object Create an "Memory-File"
      // (IStorage) on ILockBytes. As an result you get an IStorage-Pointer
      // 2. Call Save with the IStorage-Pointer and an Parameter. This means the ActiveX-Control
      //    stores it´s Information to the Place IStorage.
      // 3. Call SaveCompleted to complete the Save Action. It´s like a COMMIT.
      // 4. Release all IStorage-Pointers with a call of HandsOffStorage

      // check whether storing has to be done

      // Create a Zeidon storage sink
      OLECHAR ocName[ MAX_PATH ];
      ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, m_csStorageFileName, -1, ocName, MAX_PATH );
      hr = StgCreateDocfile( ocName,
                             STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                             0, &lpStorage );
      if ( hr != S_OK )
      {
         AfxThrowOleException ( hr );
      }

      hr = pUnk->QueryInterface( IID_IPersistStream, (LPVOID*) &lpPersistStream );
      if ( hr == E_NOINTERFACE )
      {
         hr = pUnk->QueryInterface( IID_IPersistStorage, (LPVOID*) &lpPersistStorage );
         if ( hr != S_OK )
            AfxThrowOleException ( hr );
         else
         {
            hr = lpPersistStorage->Save ( lpStorage, FALSE );
            if ( hr != S_OK )
               AfxThrowOleException ( hr );
         }
      }
      else
      {
         ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, _T("Contents"), -1, ocName, MAX_PATH );
         hr = lpStorage->CreateStream( ocName,
                                       STGM_CREATE | STGM_DIRECT | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
                                       0, 0, &lpStream );
         if ( hr != S_OK )
         {
            AfxThrowOleException( hr );
         }

         hr = lpPersistStream->Save( lpStream, FALSE );
         if ( hr != S_OK )
         {
            AfxThrowOleException( hr );
         }
      }

      mReleaseInit( lpPersistStorage );
      mReleaseInit( lpPersistStream );
      mReleaseInit( lpStorage );
      mReleaseInit( lpStream );

      CFile fStg( m_csStorageFileName, CFile::modeRead );
      dwMemoryLth = (DWORD) fStg.GetLength();

      pMemPtr = new BYTE[ dwMemoryLth ];
      if ( !pMemPtr )
      {
         hr = E_FAIL;
         AfxThrowMemoryException( );
      }

      fStg.Read( pMemPtr, dwMemoryLth );

      // Here comes the Zeidon specific Part
      // We store our Properties in a BLOB
      zVIEW vCtrl = 0;
      if ( m_pPainterWindow && MiValidView( m_pPainterWindow->m_vDialog ) )
      {
         CreateViewFromViewForTask( &vCtrl, m_pPainterWindow->m_vDialog,
                                    m_pPainterWindow->m_vDialog );
         if ( vCtrl )
         {
            if ( PositionOnZeidonCtrl( vCtrl ) == 0 )
            {
               DropView( vCtrl );
               vCtrl = 0;
            }
         }
      }

      if ( vCtrl )
      {
         if ( m_bTraceDisplayEntityInstance )
         {
            TraceLineS( "***","" );
            TraceLineS( "TZActiveX::WritePersistProperties ",
                        "DisplayEntityInstance" );
            DisplayEntityInstance( vCtrl, szlControl );
         }

         // write persistent properties to blob
         SetAttributeFromBlob( vCtrl, szlControl, szlProperties,
                               pMemPtr, dwMemoryLth );

         delete []pMemPtr;

         if ( m_bLicensed )
            dwMemoryLth = ::SysStringByteLen( m_bstrLicenseKey );
         else
            dwMemoryLth = 0;

         // write license Key to blob
         SetAttributeFromBlob( vCtrl, szlControl, szlLKey,
                               m_bstrLicenseKey, dwMemoryLth );

         if ( m_bTraceBlob )
         {
            TraceBlob( "TZActiveX::WritePersistProperties",
                       pMemPtr, dwMemoryLth );
         }

         DropView( vCtrl );
         vCtrl = 0;
      }
      else
         TraceLineS( "TZActiveX::WritePersistProperties ",
                     "Unexpected Invalid View" );

      m_bPropertiesChanged = FALSE;
   }
   CATCH_ALL( e )
   {
      // Report the Ole-Error Message to the User
      e->ReportError( );
      mReleaseInit( lpPersistStorage );
      mReleaseInit( lpPersistStream );
      mReleaseInit( lpStorage );
      mReleaseInit( lpStream );
   }
   END_CATCH_ALL

} // WritePersistProperties

/*
void
TZActiveX::WritePersistProperties( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZActiveX::WritePersistProperties: ", m_csTag );
#endif

   IPersistStorage *pPersistStorage;
   IStorage        *pStorage;
   ILockBytes      *pLockBytes;
   HRESULT         hr;
   DWORD           dwMemoryLth;
   HGLOBAL         hMemProperty;
   void            *pMemPtr;

   // This function stores the Property-Values or the actual ActiveX-Control
   // state (depending on the implementation of the ActiveX-Control) in a
   // blob.

   // First of all we need IPersistProperty.
// if ( m_OleClientItem.m_lpObject == 0 )
   if ( GetIOleObjectInterfacePtr() == 0 )   // after the call of this method
      return;                                // m_lpObject should be valid

   // Get the length for the blob.
   dwMemoryLth = GetRequiredMemoryLength( );
   if ( dwMemoryLth == 0 )
      return;

   TRY
   {
      // First we check the existence of IPersistStorage, because if there is no
      // IPersistStorage Interface we can't do anything.
//    hr = m_OleClientItem.m_lpObject->QueryInterface( IID_IPersistStorage,
      hr = m_lpObject->QueryInterface( IID_IPersistStorage,
                                                       (LPVOID *) &pPersistStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      // Now allocate memory for saving the properties.
      hMemProperty = GlobalAlloc( GMEM_MOVEABLE | GMEM_NODISCARD,
                                  dwMemoryLth );
      if ( hMemProperty == 0 )
      {
         pPersistStorage->Release( );
         return;
      }

      // Get the pointer to the global memory.
      pMemPtr = GlobalLock( hMemProperty );
      if ( pMemPtr == 0 )
      {
         GlobalFree( hMemProperty );
         pPersistStorage->Release( );
         return;
      }

      // Now there are 5 Steps to do.
      // 1. Create an ILockBytes Object.
      // 2. Create an "Memory-File" on ILockBytes.  As a result you get an
      //    IStorage-Pointer.
      // 3. Call Save with the IStorage-Pointer an a parameter.  This means
      //    the ActiveX-Control stores its information in the IStorage.
      // 4. Call SaveCompleted to complete the Save Action (like a COMMIT).
      // 5. Release all IStorage-Pointers with a call of HandsOffStorage.

      hr = CreateILockBytesOnHGlobal( hMemProperty, FALSE, &pLockBytes );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      hr = StgCreateDocfileOnILockBytes( pLockBytes,
                                         STGM_CREATE | STGM_READWRITE |
                                            STGM_SHARE_EXCLUSIVE,
                                         0, &pStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      hr = pPersistStorage->Save( pStorage, m_bSameAsLoaded );
      if ( hr == E_INVALIDARG )
      {
         hr = pPersistStorage->Save( pStorage, !m_bSameAsLoaded );
         if ( hr == STG_E_MEDIUMFULL || hr == E_FAIL )
         {
            AfxThrowOleException( hr );
         }
      }

      hr = pPersistStorage->SaveCompleted( pStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      hr = pPersistStorage->HandsOffStorage( );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      pStorage->Release( );
      pLockBytes->Release( );
      pPersistStorage->Release( );

   // SetAttributeFromBlob( vCtrl, szlControl, szlCtrlBOI,
   //                       pMemPtr, dwMemoryLth );

      GlobalUnlock( hMemProperty );
      GlobalFree( hMemProperty );
   }
   CATCH( COleException,e )
   {
      // We catch only OleExceptions.  First we have to clean up.
      if ( pPersistStorage )
         pPersistStorage->Release( );

      if ( pStorage )
         pStorage->Release( );

      if ( pLockBytes )
         pLockBytes->Release( );

      if ( pMemPtr )
         GlobalUnlock( hMemProperty );

      if ( hMemProperty )
         GlobalFree( hMemProperty );

      // Report the Ole-Error message to the user.
      e->ReportError( );
   }
   END_CATCH
}*/

DWORD
TZActiveX::GetRequiredMemoryLength( )
{
   IPersistStorage *pPersistStorage;
   IStorage        *pStorage;
   HRESULT         hr;
   CString         csFileName = "C:\\ActiveX.tmp";
   BSTR            bstrFileName = 0;
   DWORD           dwMemoryLth = 0;

   // This Function retrieve the required Memory to store the Properties for
   // the ActiveX Control.
   // To do this we must save the Properties in a File and then get the
   // length of the File.

   // First of all we need IPersistProperty
   if ( GetIOleObjectInterfacePtr() == 0 )
      return( dwMemoryLth );

   TRY
   {
      hr = m_lpObject->QueryInterface( IID_IPersistStorage,
                                       (zPVOID *) &pPersistStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      // Check if saving is required
      if ( ! (pPersistStorage->IsDirty( ) == S_OK) )
      {
         pPersistStorage->Release( );
         return( dwMemoryLth );
      }

      // Create the IStorage to save the properties in Storage-Mode.
      bstrFileName = csFileName.AllocSysString( );
      hr = StgCreateDocfile( bstrFileName,
                             STGM_CREATE | STGM_READWRITE |
                                STGM_SHARE_EXCLUSIVE, 0, &pStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      ::SysFreeString( bstrFileName );
      bstrFileName = 0;

      // Now save the Object to File
      hr = pPersistStorage->Save( pStorage, m_bSameAsLoaded );
      if ( hr == E_INVALIDARG )
      {
         hr = pPersistStorage->Save( pStorage, !m_bSameAsLoaded );
         if ( hr == STG_E_MEDIUMFULL || hr == E_FAIL )
         {
            AfxThrowOleException( hr );
         }
      }

      // Complete the save.
      hr = pPersistStorage->SaveCompleted( pStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      // Release all Pointers to IStorage Objects.
      hr = pPersistStorage->HandsOffStorage( );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      // Clean up.
      pStorage->Release( );
      pPersistStorage->Release( );

      // Now get the Length of the saved Data.
      CFile fileTemp;
      CFileException f;
      DWORD dwFileLength = 0;
      if ( fileTemp.Open( csFileName, CFile::modeRead, &f ) )
      {
         dwMemoryLth = (DWORD) fileTemp.GetLength( );
         fileTemp.Close( );
         DeleteFile( csFileName );
      }
      else
         AfxMessageBox( f.ReportError( ) );
   }
   CATCH( COleException, e )
   {
      if ( pPersistStorage )
         pPersistStorage->Release( );

      if ( pStorage )
         pStorage->Release( );

      if ( bstrFileName )
         ::SysFreeString( bstrFileName );

      e->ReportError( );
   }
   END_CATCH

   return( dwMemoryLth );
}


IOleObject *
TZActiveX::GetIOleObjectInterfacePtr( )
{
   LPUNKNOWN pUnknown = NULL;

   if ( m_lpObject == NULL )
   {
      pUnknown = GetControlUnknown();
      if ( pUnknown )
      {
         HRESULT hr = pUnknown->QueryInterface(IID_IOleObject, (LPVOID *)&m_lpObject);
         if (!SUCCEEDED (hr))
            m_lpObject = NULL;
      }
   }

   return( m_lpObject );
}

POSITION
TZActiveX::GetFirstEventPosition()
{
   CString strTypeLibGuid = m_TypeLibrary.GetGUID_AsString();
   ZDispinterface *pEvents = NULL;

   if ( strTypeLibGuid.IsEmpty( ) ) // if guid is empty typelib has not been loaded
   {
      // Get name of the typelibrary
      ZActiveX_CtrlList lRegisteredCtrl;
      ZActiveX_Ctrl * pCurrentControl;
      lRegisteredCtrl.PopulateZeidonRegisteredActiveX();
      pCurrentControl = lRegisteredCtrl.FindRegControl( m_csClassID );

      // Load the type lib
      if ( pCurrentControl )
      {
         if ( !m_TypeLibrary.LoadTypeLibrary( pCurrentControl->GetTypeLib( ), m_csClassID ) )
         {
            return( NULL );
         }
      }
   }

   pEvents = m_TypeLibrary.GetEventInterface( m_csClassID );
   if ( !pEvents ) // no event interface
   {
      return( NULL );
   }

   // events are always methods so return the head position of the
   // list of methods within the event interface
   return( pEvents->GetMethods()->GetHeadPosition() );
}

ZFunction *
TZActiveX::GetNextEvent( POSITION& pos )
{
   CString strTypeLibGuid = m_TypeLibrary.GetGUID_AsString();
   ZDispinterface *pEvents = NULL;

   ASSERT( strTypeLibGuid.IsEmpty() == FALSE );
   ASSERT( pos );

   if ( strTypeLibGuid.IsEmpty( ) ) // if guid is empty typelib has not been loaded
   {                               // assume there was no call of GetFirstEventPosition()
      return NULL;
   }

   pEvents = m_TypeLibrary.GetEventInterface( m_csClassID );
   if ( !pEvents ) // no event interface
   {
      return NULL;
   }

   // events are always methods so return the function
   // referenced by POSITION iterator
   return( (ZFunction *) pEvents->GetMethods()->GetNext( pos ) );
}

void
TZActiveX::OnRButtonDown( UINT uModKeys, CPoint pt )
{
   WPARAM wParam = uModKeys;
   LPARAM lParam = MAKELONG(pt.x, pt.y);
   ::SendMessage( GetSafeHwnd(), WM_RBUTTONDOWN, wParam, lParam);
}

void
TZActiveX::OnMenuCommand( UINT uCmd )
{
   TraceLineI( "TZActiveX::OnMenuCommand: ", uCmd );
}

BOOL
TZActiveX::DestroyWindow()
{
   DestroyActiveX();
   return CWnd::DestroyWindow();
}

//----------------------------------------------------------------------
// Free all interfaces global memory etc, to be prepared for subsequent
// calls to CreateZ
//
// this function is called by DestroyWindow and the destructor
//
//----------------------------------------------------------------------
void
TZActiveX::DestroyActiveX()
{
#ifdef DEBUG_ALL
   CString csTrace = "--- DestroyActiveX() ";
   csTrace += m_csTag;
   TraceLineS( csTrace, "");
#endif

   WritePersistProperties();

   if ( m_dwPropNotifySink )
   {
      // We don't have to kill the Sink Object manually,
      // because InterfaceDisconnect is doing this for us.
      InterfaceDisconnect( GetControlUnknown(),
                           IID_IPropertyNotifySink, &m_dwPropNotifySink );
   }

   RemoveVerbs();
/*
   mReleaseInit( m_lpPersistStg );

   if ( m_lpLockBytes )
   {
      HRESULT hr = GetHGlobalFromILockBytes( m_lpLockBytes, &m_hMemProperty );
   }

   GlobalFree( m_hMemProperty );

   mReleaseInit( m_lpLockBytes );
   mReleaseInit( m_lpStorage );
*/
   mReleaseInit( m_lpObject );
   mReleaseInit( m_lpOleControl );

}


void
TZActiveX::RemoveVerbs()
{
   TZAxMenuVerb *pVerb;

   for ( zLONG k = m_VerbList.GetUpperBound(); k >= 0; k-- )
   {
      pVerb = m_VerbList[ k ];
      m_VerbList.RemoveAt( k );
      delete( pVerb );
   }
}

LPOLECONTROL
TZActiveX::GetIOleControlInterfacePtr()
{
   LPUNKNOWN pUnknown = NULL;

   if ( m_lpOleControl == NULL )
   {
      pUnknown = GetControlUnknown();
      if ( pUnknown )
      {
         HRESULT hr = pUnknown->QueryInterface( IID_IOleControl,
                                                (LPVOID *) &m_lpOleControl );
         if ( !SUCCEEDED( hr ) )
            m_lpOleControl = NULL;
      }
   }

   return( m_lpOleControl );
}

BOOL
TZActiveX::ConnectPropertyNotifySink( )
{
   BOOL bReturn = FALSE;

   bReturn = InterfaceConnect( GetControlUnknown(),IID_IPropertyNotifySink,
                        &m_xPropertyNotifySink, &m_dwPropNotifySink );

   return( bReturn );
} // ConnectEditorEventSink

BOOL
TZActiveX::InterfaceConnect(  LPUNKNOWN pObj, REFIID riid,
                                IUnknown *pIUnknownSink, LPDWORD pdwConn )
{
   HRESULT                   hr;
   IConnectionPointContainer *pConnectionPointContainer= 0;
   IConnectionPoint          *pConnectionPoint = 0;
   BOOL bReturn = TRUE;

   if ( pObj == 0 || pIUnknownSink == 0 || pdwConn == 0 )
      return( FALSE );

   hr = pObj->QueryInterface( IID_IConnectionPointContainer,
                              (LPVOID *) &pConnectionPointContainer );
   if ( FAILED( hr ) )
      return( FALSE );

   TRY
   {
      hr = pConnectionPointContainer->FindConnectionPoint( riid,
                                                           &pConnectionPoint );
      if ( !SUCCEEDED( hr ) )
         AfxThrowOleException( hr );

      hr = pConnectionPoint->Advise( pIUnknownSink, pdwConn );
      if ( !SUCCEEDED( hr ) )
         AfxThrowOleException( hr );

      mReleaseInit( pConnectionPoint );
   }
   CATCH( COleException, e )
   {
      e->ReportError();
      bReturn = FALSE;
   }
   END_CATCH

   mReleaseInit( pConnectionPointContainer );
   return( bReturn );
} // InterfaceConnect

BOOL
TZActiveX::InterfaceDisconnect( LPUNKNOWN pObj, REFIID riid,
                                   LPDWORD pdwConn )
{
   HRESULT                   hr;
   IConnectionPointContainer *pCPointContainer = 0;
   IConnectionPoint          *pCPoint = 0;

   if ( pObj == 0 || pdwConn == 0 )
      return( FALSE );

   if ( *pdwConn == 0 )
      return( FALSE );

   hr = pObj->QueryInterface( IID_IConnectionPointContainer,
                              (LPVOID *) &pCPointContainer );
   if ( FAILED( hr ) )
      return( FALSE );

   hr = pCPointContainer->FindConnectionPoint( riid, &pCPoint );
   if ( SUCCEEDED( hr ) )
   {
      // This is the Point to kill the Sink. We are doing this
      // through Unadvise
      hr = pCPoint->Unadvise(*pdwConn);
      if ( SUCCEEDED( hr ) )
         *pdwConn = 0L;

      mReleaseInit ( pCPoint );
   }

   mReleaseInit( pCPointContainer );
   return( SUCCEEDED( hr ) );
} // InterfaceDisconnect

/////////////////////////////////////////////////////////////////////////////
// TZActiveX COM interface implementation


BEGIN_INTERFACE_MAP( TZActiveX, CWnd )
   INTERFACE_PART( TZActiveX, IID_IPropertyNotifySink, PropertyNotifySink )
END_INTERFACE_MAP( )

///////////////////////////////////////////////////////////////////////////////
// TZActiveX::XPropertyNotifySink
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) TZActiveX::XPropertyNotifySink::AddRef()
{
   METHOD_PROLOGUE( TZActiveX, PropertyNotifySink )

   return( pThis->ExternalAddRef() );
}

STDMETHODIMP_( ULONG ) TZActiveX::XPropertyNotifySink::Release()
{
   METHOD_PROLOGUE( TZActiveX, PropertyNotifySink )

   return( pThis->ExternalRelease() );
}

STDMETHODIMP TZActiveX::XPropertyNotifySink::QueryInterface(
   REFIID iid, LPVOID* ppvObj )
{
   METHOD_PROLOGUE( TZActiveX, PropertyNotifySink )

   return( pThis->ExternalQueryInterface( &iid, ppvObj ) );
}

STDMETHODIMP TZActiveX::XPropertyNotifySink::OnChanged(
   DISPID dispid )
{

   METHOD_PROLOGUE( TZActiveX, PropertyNotifySink )

   // Whenever a property is chanded
   pThis->m_bPropertiesChanged = TRUE;
   return( S_OK );
}

STDMETHODIMP TZActiveX::XPropertyNotifySink::OnRequestEdit(
   DISPID dispid )
{

   METHOD_PROLOGUE( TZActiveX, PropertyNotifySink )

   // Allow to change any property
   HRESULT hrResponse = S_OK;

   return( hrResponse );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
GenerateActiveX_Code( zVIEW vSubtask, zCPCHAR szTypeLib,
                      zCPCHAR szScriptName, zCPCHAR szClassName,
                      zCPCHAR szSourceFileName, zCPCHAR szPathName,
                      zCPCHAR szGUID )
{
   ZTypeLibrary l;
   CString strFileName = szPathName;
   BOOL bAppend = FALSE;

   strFileName += "\\";
   strFileName += szSourceFileName;
   CString strSourceFileName = strFileName;
   CString strHeaderFileName = strFileName;
   strSourceFileName += ".cpp";
   strHeaderFileName += ".h";

   if ( l.LoadTypeLibrary( szTypeLib, szGUID ) == 0 )
      return( 1 );

   if ( SysOpenFile( vSubtask, strSourceFileName, COREFILE_EXIST ) != -1 ||
        SysOpenFile( vSubtask, strHeaderFileName, COREFILE_EXIST ) != -1 )
   {
      zSHORT nRC = MessageBox( 0, "Source File(s) exist ...\n"
                                  "replace (Yes), append (No), cancel",
                               "ZDr ActiveX Code Generation", MB_YESNOCANCEL );
      if ( nRC == IDCANCEL )
         return( 2 );

      if ( nRC == IDYES )
         bAppend = FALSE;
      else
         bAppend = TRUE;
   }

   if ( l.GenerateCode( strFileName, szScriptName, szClassName,
                        szGUID, bAppend ) == 0 )
   {
      return( 3 );
   }

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
BuildTZEDWRKO( zVIEW  vTZEDWRKO,
               zPCHAR pchOperation,
               zSHORT nMaxOperLth )
{
   if ( MiValidView( vTZEDWRKO ) == FALSE &&
        (pchOperation == 0 || nMaxOperLth <= 0) )
   {
      return( 0 );
   }

   ZActiveX_CtrlList AX_CtrlList;
   AX_CtrlList.PopulateZeidonRegisteredActiveX( );

   ZActiveX_Ctrl *pAX_Ctrl;
   ZTypeLibrary typeLibrary;
   ZOperation *pOperation;
   POSITION posAX_Oper;
   zLONG    lOperCnt = 0;
   CString  cs;

   POSITION pos = AX_CtrlList.GetHeadPosition( );
   while ( pos )
   {
      pAX_Ctrl = (ZActiveX_Ctrl *) AX_CtrlList.GetAt( pos );
      if ( pAX_Ctrl )
      {
         CString csTypeLib = pAX_Ctrl->GetTypeLib( );
         CString csClassId = pAX_Ctrl->GetClassId( );
         typeLibrary.LoadTypeLibrary( csTypeLib, csClassId );
         posAX_Oper = typeLibrary.GetFirstActiveX_OperationPosition( );
         while ( posAX_Oper )
         {
            lOperCnt++;
            pOperation = typeLibrary.GetNextOperation( posAX_Oper );
            cs = pOperation->GetName( );
            if ( pchOperation )
            {
               if ( zstrcmp( pchOperation, cs.GetString() ) == 0 )
               {
                  ZOperEntry *pParmEntry;
                  POSITION posAX_Parm;
                  zBOOL    bFirstParm = TRUE;

                  cs = pOperation->GetType( );
                  cs += " ";
                  cs += pOperation->GetName( );
                  cs += "(";
                  posAX_Parm = pOperation->GetFirstParameterPosition( );
                  while ( posAX_Parm )
                  {
                     pParmEntry = pOperation->GetNextParameter( posAX_Parm );
                     if ( bFirstParm )
                     {
                        cs += " ";
                        bFirstParm = FALSE;
                     }
                     else
                     {
                        cs += ", ";
                     }

                     cs += pParmEntry->GetType( );
                     cs += " ";
                     cs += pParmEntry->GetName( );
                  }

                  cs += " )";
                  pchOperation[ 0 ] = 0;
                  strncat_s( pchOperation, nMaxOperLth, cs, nMaxOperLth - 1 );
                  typeLibrary.DestroyContent( );
                  return( 1 );
               }
            }
            else
            {
               CreateEntity( vTZEDWRKO, "Oper", zPOS_LAST );
               SetAttributeFromString( vTZEDWRKO, "Oper", "Name", cs );
            }
         }

         typeLibrary.DestroyContent( );
      }

      AX_CtrlList.GetNext( pos );
   }

   if ( pchOperation )
      pchOperation[ 0 ] = 0;

   return( lOperCnt );
}

#ifdef DEBUG_ALL
#undef DEBUG_ALL
#endif
