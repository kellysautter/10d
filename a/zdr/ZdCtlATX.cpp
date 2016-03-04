/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc. and TONBELLER AG.
// All Rights Reserved.
//
//  Confidential and Proprietary material subject to license -
//  do not reproduce or disclose. This material is an unpublished work
//  and is considered a trade secret belonging to the copyright holder.
//
/////////////////////////////////////////////////////////////////////////////
//
// Author:   TMV
// Date:     26.Mar.2001
// Overview: Source file for implementation of Zeidon ActiveX-Control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.04    DKS   Z10
//    Corrected return code check from initialization.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2001.04.04    TMV   2000   ActiveX
//    make a fix to TZActiveX::BuildLicKeyString because Mfc-TRACE-Macros
//    are limited to 512 bytes
//
// 2001.03.26    DKS   Cosmetic.
//    Thomas ... I kind of like the // comments better.  The copyright mark
//    is also a nice touch.  Does it show in the German character set?  The
//    10 character date keeps things lined up.  Three character indentation
//    should be part of the standard.
//
// 2001.03.26    TMV   2000   ActiveX
//    Retrieve license key from xwd and create control using this
//    related methods:
//       ZActiveX::ZActiveX
//       ZActiveX::CreateActiveXWithPersistProperties
//    new methods:
//       ZActiveX::BuildLicKeyString ( for debugging only )
//    see TZActiveX for further changes
//
// 2000.09.04    TMV   Z10
//    Passing ZTypelibrary pointer to any object that is needed to
//    generate ActiveX C++ wrapper code
//
// 2000.05.08    TMV   Z10
//    validate window handle before calling DestroyWindow
//
// 2000.04.10    TMV   Z10  ActiveX
//    check userdll return code against zACTIVEX_EVENT_COMPLETED
//    to determine whether Dialog actions have to be performed
//
// 2000.04.06    TMV   Z10  ActiveX
//    In ~ZActiveX call into user dll to give user a chance to delete all
//    he has allocated
//
// 2000.03.21    TMV   ActiveX
//    ZActiveX routes events directly to wrapper dll if requested
//
// 2000.03.08    DKS   ActiveX
//    ActiveX controls within a parent now retrieve their properties
//    properly.
//
// 2000.01.18    TMV   Z10
//    Avoid crash if no default Dispatchinterface is found
//    related function
//       PopulateGlobalOperations
//
// 2000.01.04    TMV   Z10
//    Avoid multiple error messages when browsing typelibraries fails.
//
// 1999.12.03    TMV   Z10   TB853
//    Generating code for ActiveX Wrapper with new Scriptroutine signature.
//
// 1999.12.02    TMV   Z10   TB853
//    fixing crash when working with multiple sources.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added capability to retrieve parameters for ActiveX events.
//
// 1999.05.11    DKS   Z10    ActiveX
//    New inheritance of ActiveX controls from OleClientItem.
//
// 1999.01.26    DKS
//    Blocked out useage of ActiveXScript (until done properly within
//    the framework of the new ActiveX implementation).
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlGbl.h"
#ifdef zREMOTE_SERVER
#include "zdctlvwg.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZActiveX, CWnd )

BEGIN_MESSAGE_MAP( ZActiveX, CWnd )
   ON_WM_MOUSEMOVE( )
   ON_NOTIFY_REFLECT( NM_KILLFOCUS, OnKillFocus  )
END_MESSAGE_MAP( )

#ifdef DEBUG_ALL
   int ZActiveX::m_iInstanceCount = 0;
#endif

ZActiveX::ZActiveX( ZSubtask *pZSubtask,
                    CWnd     *pWndParent,
                    ZMapAct  *pzmaComposite,
                    zVIEW    vDialog,
                    zSHORT   nOffsetX,
                    zSHORT   nOffsetY,
                    zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
          CWnd( ),
          ZMapAct( pZSubtask,             // base class ctor
                   pzmaComposite,
                   pWndParent,
                   this,
                   vDialog,
                   nOffsetX,
                   nOffsetY,
                   pCtrlDef,
                   "ActiveX" )
{
#ifdef DEBUG_ALL
   m_iInstanceCount++;
   CString strTrace = "+++ ZActiveX::ctor ";
   strTrace += *m_pzsTag;
   strTrace += " No. of instances ";
   TraceLineI( strTrace, m_iInstanceCount );
#endif

   zPCHAR  pch;

   m_nMapCnt = 0;
   m_hLibrary = 0;
   m_ulFlag = 0;

   // Get the persistent properties.
   GetAttributeLength( &m_ulPropertiesLth, vDialog, szlCtrl, "Properties" );
   m_pProperties = (zPCHAR) pCtrlDef->Properties;

   // Get the somehow serialized Zeidon Control properties.
   GetAttributeLength( &m_ulCtrlBOILength, vDialog, szlCtrl, "CtrlBOI" );
   m_pCtrlBOI = (zPCHAR) pCtrlDef->CtrlBOI;

   // Get the License key.
   GetAttributeLength( &m_ulLicenseKeyLength, vDialog, szlCtrl, "LKey" );
   m_bstrLicenseKey = NULL;
   if ( m_ulLicenseKeyLength )
   {
      // License Key is stored as double byte charachter
      // so the number of bytes are twice as much
      unsigned int nWCHARCount = m_ulLicenseKeyLength / 2;
      m_bstrLicenseKey = ::SysAllocStringLen( (BSTR) pCtrlDef->LKey,
                                              nWCHARCount );
#ifdef DEBUG_ALL
      BuildLicKeyString();
#endif
   }

   GetAddrForAttribute( &pch, vDialog, szlCtrl, "TagPE" );
   m_csTagPE = pch;
   m_csTagPE += "Script";
   GetAddrForAttribute( &pch, vDialog, szlCtrl, "GUID" );
   m_csClassID = pch;
   GetAddrForAttribute( &pch, vDialog, szlCtrl, "Script" );
   m_csScript = pch;
   GetAddrForAttribute( &pch, vDialog, szlCtrl, "ScriptDLL" );
   m_csScriptDLL = pch;

   CString strTempDir;
   LPSTR lpszTempDir = strTempDir.GetBufferSetLength( MAX_PATH );
   ::GetEnvironmentVariable( _T("TEMP"), lpszTempDir, MAX_PATH );
   strTempDir.ReleaseBuffer();
   m_csStorageFileName = strTempDir;
   m_csStorageFileName += '\\';
   m_csStorageFileName += m_csTagPE;
   m_csStorageFileName += ".stg";


   LoadMappingInfo( vDialog );
   //QueryEventInfo();

   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
   {
      CreateZ( );
   }
}

ZActiveX::~ZActiveX( )
{
#ifdef DEBUG_ALL
   m_iInstanceCount--;
   CString strTrace = "--- ZActiveX::dtor ";
   strTrace += *m_pzsTag;
   strTrace += " No. of instances ";
   TraceLineI( strTrace, m_iInstanceCount );
#endif

   if ( m_bstrLicenseKey )
      ::SysFreeString( m_bstrLicenseKey );

   DestroyWindow();

   // Make a last call to script - routine to give the user a chance
   // to drop locally created objects.
   ZMapping *pMapping;
   while ( m_nMapCnt )
   {
      m_nMapCnt--;
      pMapping = m_MapList[ m_nMapCnt ];
      m_MapList.RemoveAt( m_nMapCnt );
      delete( pMapping );
   }
}

//------------------------------------------------------------------
// free all interfaces and call dll routine to uninitialize
// the wrapper in the dll
//
void
ZActiveX::DestroyActiveX()
{
   // make a last call to script - routine to give the user a chance
   // to drop locally created objects
   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;

   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript,
                                        "(atx-evnt)" );

// TRACE0( "call dynamic uninit. routine\n" );
   if ( lpfnDynRoutine )
   {
      zLOUSHORT( (*lpfnDynRoutine)( this, 0, 0,
                                          zACTIVEX_UNINITIALIZE) );
   }

   mReleaseInit( m_pDispatch );
}

BOOL
ZActiveX::DestroyWindow()
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::DestroyWindow()", *m_pzsTag );
#endif
   DestroyActiveX();
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return CWnd::DestroyWindow();
}

void
ZActiveX::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::CreateZ", *m_pzsTag );
#endif

   BOOL     bCreateControl;
   HRESULT  hResult = 0;
   zSHORT   nRC = -1;

   // ActiveX Control is on the Window.
   // Now Create the Control via CreateControl.
   bCreateControl = CreateActiveXObjectWithPersistProperties( );

   if ( bCreateControl )
   {
      // Setup COleClientItem properly
      LPUNKNOWN pUnknown = GetControlUnknown( );
      HRESULT hr = S_FALSE;
      if ( pUnknown )
      {
         TRY
         {
            hr = pUnknown->QueryInterface( IID_IDispatch,
                                   (LPVOID *) &m_pDispatch );
            if ( SUCCEEDED( hr ) )
               m_pDispatch->AddRef( );
            else
               AfxThrowOleException( hr );

            // dynamic call to InitRoutine
            zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;

            // We must call the appropriate script here!
            lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
                 GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                                 (LPLPLIBRARY) &m_hLibrary,
                                                 m_csScriptDLL, m_csScript,
                                                 "(atx1)" );
            if ( lpfnDynRoutine )
            {
            // TraceLineS( "Processing ActiveX Initialization ", m_csScript );
               nRC = zLOUSHORT( (*lpfnDynRoutine)( this, 0, 0,
                                                   zACTIVEX_INITIALIZE ) );
            // TraceLineS( "Terminating ActiveX Initialization ", m_csScript );
            }
         }
         CATCH( COleException, e )
         {
            m_pDispatch = 0;
            e->ReportError( );
         }
         END_CATCH
      }
   }
   else
   {
      TraceLineS( "Unable to create ActiveX : ", *m_pzsTag );
   }

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( nRC == 0 )
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#endif
}

void
ZActiveX::ShowProperties( )
{
   CRect rect;
   LPOLEOBJECT lpObject = NULL;
   LPUNKNOWN pUnknown = GetControlUnknown( );

   if ( pUnknown )
        pUnknown->QueryInterface( IID_IOleObject,
                                (LPVOID *) &lpObject );

   if ( lpObject )
   {
      GetClientRect( &rect );
      lpObject->DoVerb( OLEIVERB_PROPERTIES, 0, 0, 0,
                                          m_pWndParent->m_hWnd, &rect );
   }

   mReleaseInit( lpObject );
}

BOOL
ZActiveX::CreateActiveXObjectWithPersistProperties( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::CreateActiveXObjectWithPersistProperties", "" );
#endif

   CRect    rect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H );
   CFile    *f = NULL;
   BOOL     bCreate = FALSE;
   BOOL     bFromStorage = FALSE;

   // This Function creates the ActiveX object using the persist properties.
   // First get the length of the blob.
   if ( m_ulPropertiesLth > 0 )
   {
      // store the properties from the BLOB in a file
      f = new CFile( m_csStorageFileName, CFile::modeCreate | CFile::modeWrite );
      f->Write( m_pProperties, m_ulPropertiesLth );
      delete f;
      // reopen readonly
      f = new CFile( m_csStorageFileName, CFile::modeRead );

      bFromStorage = TRUE;
   }

   // Now we try to create the control.
   bCreate = CreateControl( m_csClassID, 0,
                            WS_VISIBLE | WS_BORDER | WS_TABSTOP, rect,
                            m_pWndParent, -1, f, bFromStorage,
                            m_ulLicenseKeyLength ? m_bstrLicenseKey : NULL  );

   // Close the memory file.
   // Clean up.
   if ( f )
   {
      delete f;
   }


   if ( bCreate )
   {
      ZDrView *pZView = ZMapAct::GetFormView();
      if ( pZView )
      {
         ZAmbientProperty *pAP = pZView->FindAmbientProperty( DISPID_AMBIENT_MESSAGEREFLECT );
         COleVariant v;

         v.boolVal = TRUE;
         pAP->SetValue( v );
         if ( bCreate )
         {
            LPOLECONTROL pC = NULL;
            LPUNKNOWN    pU = NULL;
            pU = GetControlUnknown();
            pU->QueryInterface(IID_IOleControl, (LPVOID*)&pC);
            pC->OnAmbientPropertyChange( DISPID_AMBIENT_MESSAGEREFLECT );
            pC->Release();
         }
      }
   }

   return( bCreate );
}


///////////////////////////////////////////////////////////////////////
//
// The RequestLicenseKey function uses the IClassFactory2 interface
// to request a License key from an object specified by its classid.
//
// Parameters:
//
//   [out] BSTR& bstrLicenseKey
//         Upon return, this BSTR will contain either a valid
//         License key or an error message. It is the caller's
//         responsibility to call ::SysFreeString on this BSTR.
//
//   [in]  CString strProgID
//         Specifies the ProgID of the object from which to request the
//         License key.
//
//
// Return Value:
//
//   A BOOL specifying success or failure. If TRUE is returned,
//   the License key was retrieved successfully and the
//   bstrLicenseKey parameter contains a valid License key. If
//   FALSE is returned, the License key was not retrieved
//   successfully and the bstrLicenseKey parameter contains a
//   descriptive error string.
//
//   Regardless of the return value, it is the responsibility of
//   the caller to call ::SysFreeString on the returned bstrLicenseKey
//   parameter.
//
///////////////////////////////////////////////////////////////////////
BOOL
ZActiveX::RequestLicenseKey( BSTR &bstrLicenseKey, CString strClsid )
{
   USES_CONVERSION;

   LPCLASSFACTORY2 pClassFactory;
   CLSID clsid;
   BOOL bValidKeyReturned = FALSE;


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
            if (bstrLicenseKey == NULL)
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

void
ZActiveX::BuildLicKeyString( )
{
   BYTE  CurrentByte;
   BYTE  *pKeyByteData;
   WCHAR CurrentWCHAR;
   WCHAR *pKeyWCHARData;

   unsigned int nByteCount, nWCHARCount, nIndex;
   zCHAR Temp[ 7 ];
   CString strHumanReadable;
   CString strTempLicenseString;

   // Get a pointer to the beginning of the key data.
   pKeyWCHARData = m_bstrLicenseKey;

   // Get the size of the license key data in bytes.
   nByteCount = SysStringByteLen( m_bstrLicenseKey );

   // Get the size of the license key data in WCHAR's.
   nWCHARCount = nByteCount / 2;

   // Initialize the WCHAR array declaration string.
   strTempLicenseString = "WCHAR pwchLicenseKey[] =\r\n{\r\n\t";

   // Initialize the human readable version of the license key.
   strHumanReadable = "/*\r\n";

   // Walk through the the license key data, WCHAR by WCHAR, generating
   // hex formatted strings for each WCHAR value. Do appropriate formatting
   // with carriage return/line feeds.
   for ( nIndex = 0; nIndex < nWCHARCount; nIndex++ )
   {
      // Get the current WCHAR of data.
      CurrentWCHAR = *(pKeyWCHARData + nIndex);

      // Create a hex formatted string for the current WCHAR.
      wsprintf( Temp, "0x%04X", CurrentWCHAR );

      // Add the hex formatted string to the end of the BYTE array declaration
      //  string.
      strTempLicenseString += Temp;

      // See if this is the last WCHAR of the license key data. If it isn't,
      //  add a comma and a tab to the end of the WCHAR array declaration string.
      //  We check to see if this is the last WCHAR of data because we don't
      //  need a comma after the hex string for the last WCHAR of the license
      //  key data.
      if ( nIndex < (nWCHARCount - 1) )
         strTempLicenseString += ",\t";

      // Check to see if this is the 6th hex string on this line. If it is,
      //  add a carriage return, line feed, and a tab to the WCHAR array
      //  declaration string.
      if ( !((nIndex + 1) % 6) && (nIndex < (nWCHARCount - 1)) )
         strTempLicenseString += "\r\n\t";
   }

   // Add a final carriage return/line feed followed by the closing brace and
   //  semicolon to the WCHAR array declaration string.
   strTempLicenseString += "\r\n};";

   // Walk through the the license key data, byte by byte, generating
   // a human readable string. If the current byte is a printable
   // character or a carriage return or a line feed character, add it
   // to the human readable string. If it's not, insert a '.'
   // placeholder into the human readable string.

   // Get a pointer to the beginning of the key data.
   pKeyByteData = (BYTE *)m_bstrLicenseKey;

   for ( nIndex = 0; nIndex < nByteCount; nIndex++ )
   {
      // Get the current byte of data
      CurrentByte = *(pKeyByteData + nIndex );

      if ( isprint( (int) CurrentByte ) ||
           CurrentByte == 0x0D || CurrentByte == 0x0A )
      {
         strHumanReadable += (char) CurrentByte;
      }
      else
      if ( CurrentByte != 0 )
         strHumanReadable += ".";
   }

   // Add a final carriage return/line feed and close the comment block for the
   // human readable version of the license key.
   strHumanReadable += "\r\n*/";

   // Place the human readable string and the temp data declaration string into
   // the final license key string.
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

zSHORT
ZActiveX::GetEventParmCnt( DISPID dispid )
{
   ZEventItem *pItem = m_EventList.FindEvent( dispid );
   if ( pItem )
      return( (zSHORT) pItem->GetParmCnt( ) );

   return( -3 );
}

zSHORT
ZActiveX::GetEventParm( DISPID dispidEvent, zLONG lParmID,
                        zPCHAR pchValue, zSHORT nMaxLth )
{
   pchValue[ 0 ] = 0;
   ZEventItem *pEventItem = m_EventList.FindEvent( dispidEvent );
   if ( pEventItem )
   {
      ZParmItem *pParmItem = pEventItem->FindParm( lParmID );
      if ( pParmItem )
      {
         if ( nMaxLth > 0 )
            strncat_s( pchValue, nMaxLth, pParmItem->m_csValue, nMaxLth - 1 );

         return( (zSHORT) zstrlen( pchValue ) );
      }

      return( -4 );
   }

   return( -3 );
}

BOOL
ZActiveX::OnCmdMsg( UINT uID, int nCode,
                    void *pExtra,
                    AFX_CMDHANDLERINFO *pHandlerInfo )
{
#ifdef DEBUG_ALL
// TraceLineI( "ZActiveX::OnCmdMsg Code: ", nCode );
#endif

   AFX_EVENT *pEvent;
   zLONG  nRC = -1;

   if ( nCode == CN_EVENT )
   {
      pEvent = (AFX_EVENT *) pExtra;
      if ( pEvent )
      {
      // zVIEW vEvent = 0;
         zCTRL_EVENT ce;

         ce.m_pEvent = pEvent;
         ce.m_ulRC = 0x00000000;  // initialized to cause default processing

         if ( IsEventHandled( ce.m_pEvent->m_dispid ) )
         {
            m_pZSubtask->m_pEventParms = &ce ;

            // dynamic call into userdll
            nRC = HandleEvent( (DISPID) ce.m_pEvent->m_dispid );
         }

         // do we have to call user provided code ?
         if ( nRC != zACTIVEX_EVENT_COMPLETED )
         {
            nRC = ProcessImmediateEvent( this, pEvent->m_dispid, &ce );
         }

         if ( ce.m_ulRC & 0x00000001 )
         {
            // 0x00000000 ==> invoke default processing for event
            // 0x00000001 ==> skip default processing for event
            nRC = 0;
         }
         else
            nRC = -1;
      }
      else
         TraceLineI( "CN_EVENT = ", nCode );
   }
   else
   {
      zCHAR szMsg[ 256 ];
      sprintf_s( szMsg, zsizeof( szMsg ), "nCode %d is no Event", nCode );
      TraceLineS( szMsg, "" );
   }

   // This event has not been handled so far so call Bases Classes OnCmdMsg( ).
   if ( nRC == -1 )
   {
#if 0
      if ( nCode != CN_UPDATE_COMMAND_UI )
      {
         UINT uMsg = 0;

         uMsg = HIWORD( nCode );
      // nCode = LOWORD( nCode );
         static ZWndMsgMap msgMap;

      // if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
            TraceLineS( "ZActiveX::OnCmdMsg: ", msgMap.GetMsgName( uMsg ) );
      }
#endif
      return( CWnd::OnCmdMsg( uID, nCode, pExtra, pHandlerInfo ) );
   }
   else
      return( TRUE );
}

zSHORT
ZActiveX::PopulateEventParamEntities( ZEventItem  *pEventItem,
                                      zCTRL_EVENT ce )
{
   zSHORT  nRC = 0;
   zUSHORT k;

   if ( ce.m_pEvent->m_pDispParams &&
        ce.m_pEvent->m_pDispParams->cArgs > 0 )
   {
      // Create child entities for each Parameter.
      for ( k = 0; k < ce.m_pEvent->m_pDispParams->cArgs; k++ )
      {
         CString csParmValue;
         BOOL bCreate = FALSE;
         COleVariant v( ce.m_pEvent->m_pDispParams->rgvarg[ k ] );

         bCreate = VariantTypeToZeidonType( v, csParmValue );

         if ( bCreate )
         {
            pEventItem->AddParm( k, csParmValue );
         }
         else
         {
            TraceLineS( "ActiveX Event Parameter ",
                        "ActiveX Event passes non convertable type!" );
            TraceLineI( "ActiveX Event Parameter Dispatch ID ==> ",
                        (zLONG) ce.m_pEvent->m_dispid );
            TraceLineI( "ActiveX Event Parameter # ",  k );
         }
      }
   }

   return( nRC );
}

zBOOL
ZActiveX::VariantTypeToZeidonType( COleVariant v, CString& cs )
{
   CString csParamValue;
   zBOOL bCreate = FALSE;

   csParamValue.Empty();

   // If it is a reference, convert into a variant and call recursively.
   if ( v.vt & VT_BYREF )
   {
      VARTYPE vtI = v.vt;
      vtI -= VT_BYREF;
      switch ( vtI )
      {
         case VT_DISPATCH:
         case VT_UNKNOWN:
         case VT_ARRAY:
         case VT_BYREF:
         case VT_EMPTY: // Not specified.
         case VT_NULL:  // SQL-style Null.
         {
            bCreate = FALSE;
            break;
         }

         case VT_DATE: // Date pointer
         {
            COleVariant vRef (*(v.pdate));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_CY: // Currency.
         {
            COleVariant vRef (*(v.pcyVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_BSTR: // Automation string.
         {
            csParamValue = *(v.pbstrVal);
            bCreate = TRUE;
            break;
         }

         case VT_ERROR: // Scodes.
         {
            COleVariant vRef (*(v.pscode));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_I2: // 2-byte signed int.
         {
            COleVariant vRef (*(v.piVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_I4: // 4-byte-signed int.
         {
            COleVariant vRef (*(v.plVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_R4: // 4-byte real.
         {
            COleVariant vRef (*(v.pfltVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_R8: // 8-byte real.
         {
            COleVariant vRef (*(v.pdblVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_BOOL: // Boolean; True=-1, False=0.
         {
            COleVariant vRef (*(v.pboolVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_UI1: // Unsigned char.
         {
            COleVariant vRef (*(v.pbVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         case VT_VARIANT:
         {
            COleVariant vRef (*(v.pvarVal));
            bCreate = VariantTypeToZeidonType( vRef, csParamValue );
            break;
         }

         default:
         {
            bCreate = FALSE;
            break;
         }
      } // switch ( v.vt )
   }
   else
   {
      switch ( v.vt )
      {
         case VT_DISPATCH:
         case VT_UNKNOWN:
         case VT_ARRAY:
         case VT_BYREF:
         {
            bCreate = FALSE;
            break;
         }

         case VT_EMPTY: // Not specified.
         case VT_NULL:  // SQL-style Null.
         {
            csParamValue.Empty();
            bCreate = TRUE;
            break;
         }

         case VT_DATE:  // Date.
         {
            // Convert into Zeidon Date format YYYYMMTTHHMMSSFFF
            COleDateTime dt( v );
            CString csHelp;
            LPCSTR lpszFormat = "%.2d";
            zSHORT nDate = 0;

            csParamValue.Empty();
            // YYYY
            nDate = dt.GetYear  ();
            csHelp.Format("%.4d", nDate );
            csParamValue += csHelp;
            // MM
            nDate = dt.GetMonth ();
            csHelp.Format( lpszFormat, nDate );
            csParamValue += csHelp;
            // TT
            nDate = dt.GetDay();
            csHelp.Format( lpszFormat, nDate );
            csParamValue += csHelp;
            // HH
            nDate = dt.GetHour();
            csHelp.Format( lpszFormat, nDate );
            csParamValue += csHelp;
            // MM
            nDate = dt.GetMinute();
            csHelp.Format( lpszFormat, nDate );
            csParamValue += csHelp;
            // SS
            nDate = dt.GetSecond();
            csHelp.Format( lpszFormat, nDate );
            csParamValue += csHelp;
            // FFF Fraction not supported by COleDateTime so set it fix to 000
            csParamValue += "000";

            bCreate = TRUE;
            break;
         }

         case VT_CY:   // Currency.
         {
            COleCurrency oc( v );

            csParamValue.Empty();
            if ( oc.GetStatus() != COleCurrency::null )
            {
               v.ChangeType( VT_BSTR );
               csParamValue = v.bstrVal;
            }
            bCreate = TRUE;
            break;
         }

         case VT_BSTR: // Automation string.
         {
            csParamValue = v.bstrVal;
            bCreate = TRUE;
            break;
         }

         case VT_ERROR: // Scodes.
         case VT_I2:    // 2-byte signed int.
         case VT_I4:    // 4-byte-signed int.
         case VT_R4:    // 4-byte real.
         case VT_R8:    // 8-byte real.
         case VT_BOOL:  // Boolean; True=-1, False=0.
         case VT_UI1:   // Unsigned char.
         {
            v.ChangeType( VT_BSTR );
            csParamValue = v.bstrVal;
            bCreate = TRUE;
            break;
         }

         default:
         {
            bCreate = FALSE;
            break;
         }
      } // switch ( v.vt )
   }

   cs = csParamValue;
   return( bCreate );
}

void
ZActiveX::QueryEventInfo( )
{
   zVIEW vZDrSystem;
   zSHORT nRC = 0;
   ZActiveX_CtrlList cl;
   ZActiveX_Ctrl *c;
   ZTypeLibrary t;

   // Get the only object instance of the lod KZZDR.
   if ( GetViewByName( &vZDrSystem, "__ZDrSystem",
                       m_pZSubtask->m_vDialog, zLEVEL_SYSTEM ) > 0 )
   {
      // Determine whether the view already contains information about
      // current ActiveX.
      nRC = SetCursorFirstEntityByString( vZDrSystem, "AX", "GUID",
                                          m_csClassID, 0 );
      if ( nRC < zCURSOR_UNCHANGED )
      {
         // Create an instance to make sure that the following functions
         // are invoked only once for multiple ActiveX instances.
         CreateEntity( vZDrSystem, "AX", zPOS_AFTER );
         SetAttributeFromString( vZDrSystem, "AX", "GUID", m_csClassID );
         cl.CreateItemFromClassId( m_csClassID );

         // Now we should have a list with a single entry,
         // so pick up the last and only one.
         if ( !cl.IsEmpty() )
         {
            c = (ZActiveX_Ctrl *) cl.GetTail();
            t.LoadTypeLibrary( c->GetTypeLib(), m_csClassID );

            // Create the event entries.
            ZDispinterface *pEvents = t.GetEventInterface( m_csClassID );
            if ( pEvents )
            {
               POSITION pos = pEvents->GetMethods()->GetHeadPosition();
               while ( pos )
               {
                  ZFunction *pEvent =
                     (ZFunction *) pEvents->GetMethods()->GetAt( pos );
                  CreateEntity( vZDrSystem, "Event", zPOS_AFTER );
                  SetAttributeFromInteger( vZDrSystem, "Event", "ID",
                                           pEvent->GetMemberID());
                  SetAttributeFromString( vZDrSystem, "Event", "NAME",
                                          pEvent->GetName() );
                  CObList *pParams = pEvent->GetParameter();

                  // Create the parameter entries.
                  if ( pParams )
                  {
                     POSITION posParm = pParams->GetHeadPosition();
                     while ( posParm )
                     {
                        ZParameter *pParam =
                                   (ZParameter *) pParams->GetAt( posParm );
                        CreateEntity( vZDrSystem, "Parm", zPOS_AFTER );
                        SetAttributeFromInteger( vZDrSystem, "Parm", "ID",
                                                 pParam->GetMemberID());
                        SetAttributeFromString( vZDrSystem, "Parm", "NAME",
                                                pParam->GetName() );
                        pParams->GetNext( posParm );
                     } // end:  Create the parameter entries.
                  }

                  pEvents->GetMethods()->GetNext( pos );

               } // end:  Create the event entries.
            }
         }
      }
   }
}

// Zeidon Mapping.
zSHORT
ZActiveX::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::MapFromOI ", *m_pzsTag );
#endif

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zSHORT  nRC;

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx1)" );

   if ( lpfnDynRoutine )
   {
//    TraceLineS( "Processing ActiveX MapFromOI ", m_csScript );
      nRC = zLOUSHORT( (*lpfnDynRoutine)( this, 0, 0,
                                          zACTIVEX_TRANSFER_DATA_FROM_OI ) );
//    TraceLineS( "Terminating ActiveX MapFromOI ", m_csScript );
   }
   else
   {
      nRC = 0;
   }

   return( nRC );
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZActiveX::MapToOI( zLONG lFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::MapToOI ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zSHORT  nRC;

   // If the control is not Visible and Enabled, do not do mapping.
   if ( IsVisibleForMapToOI( ) == 0 )  // note ... don't check view name here!
      return( 0 );

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx2)" );

   if ( lpfnDynRoutine )
   {
//    TraceLineS( "Processing ActiveX MapToOI ", m_csScript );
      nRC = zLOUSHORT( (*lpfnDynRoutine)( this, lFlag, 0,
                                          zACTIVEX_TRANSFER_DATA_TO_OI ) );
//    TraceLineS( "Terminating ActiveX MapToOI ", m_csScript );
   }
   else
   {
      nRC = 0;
   }

   return( nRC );
}

void
ZActiveX::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

void
ZActiveX::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::OnKillFocus ", *m_pzsTag );
#endif

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zSHORT  nRC;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx2)" );

   if ( lpfnDynRoutine )
   {
      nRC = zLOUSHORT( (*lpfnDynRoutine)( this, 0, 0,
                                          zACTIVEX_LOSE_FOCUS ) );
   }
   else
   {
      nRC = 0;
   }

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

void
ZActiveX::OnKillFocus( NMHDR *pNotifyStruct, LRESULT *result )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::OnKillFocus ", *m_pzsTag );
#endif

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zSHORT  nRC;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx2)" );

   if ( lpfnDynRoutine )
   {
      nRC = zLOUSHORT( (*lpfnDynRoutine)( this, 0, 0,
                                          zACTIVEX_LOSE_FOCUS ) );
   }
   else
   {
      nRC = 0;
   }

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

void
ZActiveX::OnSetFocus( CWnd *pWndLooseFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZActiveX::OnSetFocus ", *m_pzsTag );
#endif

   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zSHORT  nRC;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx2)" );

   if ( lpfnDynRoutine )
   {
      nRC = zLOUSHORT( (*lpfnDynRoutine)( this, 0, 0,
                                          zACTIVEX_RECEIVE_FOCUS ) );
   }
   else
   {
      nRC = 0;
   }

   m_pZSubtask->InformDriverOfSetFocus( this, TRUE );
   Default( );             // pass it along to parent
}


zLONG
ZActiveX::SetZCtrlState( zLONG  lStatusType,
                         zULONG ulValue )
{
   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zLONG lRC;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx1)" );

   if ( lpfnDynRoutine )
   {
      lRC = (*lpfnDynRoutine)( this, lStatusType, ulValue,
                               zACTIVEX_SET_CTRL_STATE );
   }
   else
   {
      lRC = 0;
   }

   return( lRC );
}

zLONG
ZActiveX::GetZCtrlState( zLONG lStatusType )
{
   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;
   zLONG lRC;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript, "(atx1)" );

   if ( lpfnDynRoutine )
   {
      lRC = (*lpfnDynRoutine)( this, lStatusType, 0,
                               zACTIVEX_GET_CTRL_STATE );
   }
   else
   {
      lRC = 0;
   }

   return( lRC );
}

#if 0
/////////////////////////////////////////////////////////////////////////////
//
// This routine sets up an ordered list of structures representing the
// mapping for a ctrl.
//
//
// struct MapInfo{
//   zPCHARViewName,
//      char* EntityName,
//      char* AttributeName,
//      char* Context,
//       ...,
//       ...,
//   };
//
// int i = QueryMappingInfoCount( ..., ... ) // retrieves the number of
// Mapping in Zeidon
//
// long MapinfoSize = zsizeof( MapInfo ) * i;
// MapInfo *pMapInfo = (MapInfo *) new zCHAR ( MapinfoSize );
//
// QueryMappingInfo ( ........., pMapInfo, MapinfoSize ) // fills the Array of
// Mapping informations into a buffer allocated by the user
//
//
///////////////////////////////////////////////////////////////////////////////

// Comparison function required to order objects added to the list.  In
// this case it just compares the names embedded in the ZMapping type
zSHORT
fnCompareMapNodes( const ZMapping *pNode1,
                   const ZMapping *pNode2 )
{
   return( pNode1->m_csTag.Compare( pNode2->m_csTag ) );
}

// Comparison function required to seek for an object in the list.  In
// this case it just compares the tags embedded in the ZMapping type.
zSHORT
fnSeekMapNode( const CString *m_pzsTag,
               const ZMapping *pNode )
{
   return( m_pzsTag->Compare( pNode->m_csTag ) );
}

// Function to process the mappings in order.
zOPER_EXPORT void
fnOrderedMapping( const ZMapping *pNode,
                  WPARAM wParam,
                  LPARAM lParam )
{
   TraceLineS( pNode->m_csTag, pNode->m_csAName );
}

// Function to delete the mapping data.
void
fnDeleteMapObject( ZMapping *pNode,
                   WPARAM wParam,
                   LPARAM lParam )
{
   mDeleteInit( pNode );
}
#endif

zSHORT
ZActiveX::LoadMappingInfo( zVIEW vDialog )
{
   ZMapping *pMapping = 0;
// mDeleteInit( m_pZBinMap );

   while ( m_MapList.GetUpperBound( ) > -1 )
   {
      pMapping = m_MapList[ m_MapList.GetUpperBound( ) ];
      m_MapList.RemoveAt( m_MapList.GetUpperBound( ) );
      delete( pMapping );
   }

   // m_nMapCnt = 0;
   if ( CheckExistenceOfEntity( vDialog, szlCtrlMap ) > zCURSOR_UNCHANGED )
   {
      zKZWDLGXO_CtrlMap_DEF CtrlMapDef;
      //m_nMapCnt = (zSHORT) CountEntitiesForView( vDialog, szlCtrlMap );
      // m_MapList.SetSize( m_nMapCnt, 10 );

//    m_pZBinMap = new ZBinSearch( (fnCOMPARE) fnCompareMapNodes,
//                                 (fnCOMPARE) fnSeekMapNode,
//                                 (zUSHORT) lMapCnt,
//                                 (fnVISIT_INORDER) fnDeleteMapObject );

      CtrlMapDef.CheckByte = 0xFE;
      CtrlMapDef.lStructSize = sizeof( zKZWDLGXO_CtrlMap_DEF );
      zSHORT nRC = SetCursorFirstEntity( vDialog, szlCtrlMap, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         GetStructFromEntityAttrs( (zPCHAR) &CtrlMapDef, vDialog, szlCtrlMap );
         if ( CtrlMapDef.CheckByte != 0xFE )
            ::MessageBox( 0, "CtrlMapDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

         pMapping = new ZMapping( CtrlMapDef );
         //m_MapList.SetAtGrow( m_nMapCnt++, pMapping );
         m_MapList.Add( pMapping );


//       if ( m_pZBinMap->InsertNode( pMapping ) == zBINSEARCH_DUP )
//       {
//          TraceLineS( "(drvr) Mapping Error (DUPLICATE Mapping tag) ==> ",
//                      CtrlMapDef.Tag );
//          OperatorSend( 0, szlDrvr, "DUPLICATE Mapping tag!!!", TRUE );
//
#ifdef DEBUG     // should not happen ... protected by painter???
//          m_pZBinMap->VisitInorder( (fnVISIT_INORDER) fnOrderedMapping );
#endif
//          return( -1 );
//       }
//
#ifdef DEBUG_ALL
         TraceLineS( "Map Tag ", CtrlMapDef.Tag );
         TraceLineS( "ViewName ", CtrlMapDef.VN );
         TraceLineS( "Entity ", CtrlMapDef.EN );
         TraceLineS( "Attribute ", CtrlMapDef.AN );
         TraceLineS( "Context ", CtrlMapDef.Context );
#endif

         nRC = SetCursorNextEntity( vDialog, szlCtrlMap, 0 );
      }

      SetCursorFirstEntity( vDialog, szlCtrlMap, 0 );
      m_nMapCnt = m_MapList.GetSize();

      return( (zSHORT) m_nMapCnt );
   }

   return( 0 );
}

ZMapping *
ZActiveX::SeekMapItem( zCPCHAR cpcTag )
{
// if ( m_pZBinMap )
// {
//    ZMapping map( cpcTag );
//
//    return( (ZMapping *) m_pZBinMap->SeekNode( &map ) );
// }

   ZMapping *pMapping;
   zSHORT nPos = m_nMapCnt;

   while ( nPos-- )
   {
      pMapping = m_MapList[ nPos ];
      if ( zstrcmp( pMapping->m_csTag.GetString(), cpcTag ) == 0 )
         return( pMapping );
   }

   return( 0 );
}


/*
   Call into the users dll to handle that event
*/
zLONG ZActiveX::HandleEvent( DISPID dispidEvent )
{
   zLONG lReturn = 0;
   zFARPROC_ACTIVEX_SCRIPT lpfnDynRoutine;

   // We must call the appropriate script here!
   lpfnDynRoutine = (zFARPROC_ACTIVEX_SCRIPT)
        GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                        (LPLPLIBRARY) &m_hLibrary,
                                        m_csScriptDLL, m_csScript,
                                        "(atx-evnt)" );
   if ( lpfnDynRoutine )
   {
      lReturn = zLOUSHORT( (*lpfnDynRoutine)( this, dispidEvent, 0,
                                          zACTIVEX_HANDLE_EVENT ) );
   }
   return( lReturn );
}

#if 0
zSHORT
ExampleOf::MapFromOI( )
{
   zVIEW vAppList;
   CString zsViewName( "_" );
   zsViewName += *m_pzsTag;

   if ( GetViewByName( &vAppList, zsViewName,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( vAppList );
   }

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;

   ZMapping *pMapping;
   ZMapping *pMapScope = SeekMapItem( "Scope" );  // check for specific scope item
   CString  zsVName;
   CString  zsEName;

   if ( pMapScope )
   {
      zsVName = pMapScope->m_csVName;
      zsScope = pMapScope->m_csEName;
   }
   else
   if ( m_nMapCnt )
   {
      pMapping = m_MapList[ 0 ];          // default to first entity as scope
      zsVName = pMapping->m_csVName;
      zsScope = pMapping->m_csEName;
   }

   // If we have a View name and a Scoping (list) Entity name ... continue
   if ( **zsVName && **zsScope )
   {
      zVIEW vApp;

      // No painting during change
      // LockWindowUpdate( );
      SetRedraw( FALSE );
      ResetContent( );

      // If we cannot get the view ... quit.
      if ( GetViewByName( &vApp, pMapScope->VN,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ),
                   "Unable to load view: %s for %s.%s.%s",
                   *m_pzsVName, *m_pZSubtask->m_pzsDlgTag,
                   *m_pZSubtask->m_pzsWndTag, *m_pzsTag );
         TraceLineS( "Atx::MapFromOI ", szMsg );
//       UnlockWindowUpdate();
         SetRedraw( TRUE );
         return( 0 );
      }

      m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
      zLONG lTop = GetTopIndex( );

      // This count may not be accurate if there is a scoping entity
      // involved, in which case we have to determine the count by hand.
      zLONG lEntityCnt = CountEntitiesForView( vAppList, *m_pzsEName );

      // Auto-enable if this control was auto-disabled.
      if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
      {
         m_ulMapActFlags |= zMAPACT_ENABLED;
         m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
         EnableWindow( TRUE );
      }

      zLONG   k;

      zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, 1 );  // just to get the
      SetSelectSetForView( m_vApp, nPrevSelSet );             // previous selset
      while ( m_ulListLth )  // purist's goto
      {
         int iRelativeNbr;  // this must be int for use in API call

         if ( m_chListOnlyWhenSelected )
         {
            if ( m_pzsScope == 0 || m_pzsScope->IsEmpty( ) )
            {
               mDeleteInit( m_pzsScope );
               MiGetParentEntityNameForView( m_pchMap, m_vApp, *m_pzsEName );
               if ( m_pchMap[ 0 ] )
                  m_pzsScope = new CString( m_pchMap );
            }

            SetSelectSetForView( m_vApp, m_nSelectSelSet );

            // If there is no selected parent entity ... quit.
            if ( m_pzsScope == 0 || m_pzsScope->IsEmpty( ) ||
                 SetEntityCursor( m_vApp, *m_pzsScope, 0,
                                  m_ulFlag | zPOS_FIRST | zQUAL_SELECTED |
                                     zSPARENT_KEEPCHILDREN,
                                  0, 0, 0, 0, 0, 0 ) < 0 )
            {
               break;  // goto end of while
            }
         }

         if ( m_nDisplaySelSet )
         {
            // If there are no entities to list ... quit.
            SetSelectSetForView( m_vApp, m_nDisplaySelSet );
            if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                  m_ulFlag | zPOS_FIRST |
                                     zTEST_CSR_RESULT | zQUAL_SELECTED,
                                  0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
            {
               break;  // goto end of while
            }
         }
         else
         {
            // If there are no entities to list ... quit.
            if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                  m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                                  0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
            {
               break;  // goto end of while
            }
         }

         CreateViewFromViewForTask( &m_vAppList, m_vApp,
                                    m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewName,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent the OE from
                                    // automatically cleaning it up during
                                    // the deletion of an OE subtask.

//       TraceLineS( "ZListBox::MapFromOI ObjectName ", szViewObjectName );
//       TraceLineX( "ZListBox::MapFromOI creating m_vAppList ",
//                   (zLONG) m_vAppList );

         zCPCHAR cpcEntity;
         zSHORT  nTopSel = -1;
         zSHORT  nRC;
         zSHORT  nPos;

         iRelativeNbr = 0;
         nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                                *m_pzsScope, 0 );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            for ( nPos = 0; nPos < m_nMapCnt; nPos++ )
            {
               pMapping = m_MapList[ nPos ];
               if ( pMapping && pMapping != pMapScope )
               {
                  cpcEntity = pMapping->m_csEName;
                  if ( *cpcEntity == 0 )
                     cpcEntity = zsEName;  // default entity name to scope entity

                  // This is the attribute to present to the list.
                  zSHORT nRC =
                  GetVariableFromAttribute( m_pchMap, 0, zTYPE_STRING,
                                            max( 0, m_pchMapLth - nOffset ),
                                            m_vAppList, cpcEntity,
                                            pMapping->m_csAName,
                                            pMapping->m_csContext,
                                            *pMapping->m_csContext ?
                                                   zACCEPT_NULL_ENTITY :
                                             zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

                  if ( nRC == zVAR_NULL )
                     m_pchMap[ nOffset ] = 0;

                  SetText here!!!
               // if ( AddString( m_pchMap ) < 0 ||
               //      SetItemData( iRelativeNbr, (DWORD) pchMem ) < 0 )
               // {
               //    TraceLineS( "(lbx) Error adding string ", pchMem );
               //    break;
               // }
               }
            }

            if ( m_chMultiSelect )
            {
               if ( m_nDisplaySelSet )
                  SetSelectSetForView( m_vAppList, m_nSelectSelSet );

               if ( GetSelectStateOfEntity( m_vAppList, *m_pzsEName ) )
               {
                  SetSel( iRelativeNbr, TRUE );
                  if ( nTopSel == -1 )
                     nTopSel = (zSHORT) m_lEntityCnt;
               }

               if ( m_nDisplaySelSet )
                  SetSelectSetForView( m_vAppList, m_nDisplaySelSet );
            }
            else
            if ( nTopSel == -1 )
            {
               if ( GetSelectStateOfEntity( m_vAppList, *m_pzsEName ) )
               {
                  SetCurSel( m_lEntityCnt );
                  nTopSel = (zSHORT) m_lEntityCnt;
               }
            }

            iRelativeNbr++;
            m_lEntityCnt++;
            nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                   m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                                   *m_pzsScope, 0 );

         }  // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru fields


         if ( m_chHilitePosition == 'Y' && m_vApp && m_lEntityCnt ) // fix GetRel for SCOPE_OI
 //           CheckExistenceOfEntity( m_vApp, *m_pzsEName ) == zCURSOR_SET )
         {
            zLONG lOffset = GetRelativeEntityNumber( m_vApp, *m_pzsEName,
                                                     *m_pzsScope, m_ulFlag );
            if ( lOffset >= 0 )
            {
               SetTopIndex( lOffset );
               HiliteText( TRUE, -1 );
            }

//          TraceLineI( "MapFromOI TopIndex set to ", lOffset );
         }
         else
#if 0       // we will not ask the list box to re-establish position
         {
            if ( nTopSel >= 0 )
               lTop = nTopSel;

            if ( lTop < m_lEntityCnt )
            {
               {
                  SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                   zPOS_FIRST | zPOS_RELATIVE |
                                         zSPARENT_KEEPCHILDREN | m_ulFlag,
                                   0, 0, 0, lTop, *m_pzsScope, 0 );
               }

               SetTopIndex( lTop );
            }
         }
#endif
         if ( lTop < m_lEntityCnt )
         {
            SetTopIndex( lTop );
         }

         break;  // quit after the first time thru
      }

      SetSelectSetForView( m_vApp, nPrevSelSet );
      m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;

      // Repaint changes
   // UnlockWindowUpdate();
      Invalidate( );
      SetRedraw( TRUE );

#ifdef DEBUG_ALL
      TraceLineS( "ZListBox::MapFromOI term ", *m_pzsTag );
      TraceLineI( "ZListBox::MapFromOI EntityCnt ", m_lEntityCnt );
#endif

   }

   return( 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Script-Stuff
/////////////////////////////////////////////////////////////////////////////

#ifdef __DATA_TABLE__
extern "C" AFX_EXT_API
void
ActiveXScript( ZActiveX *pzActiveX, long lMappingFlag )
{
   IOleObject            *pOleObject;        // This is the Pointer to an
                                             // IOleInterface.  Without this
                                             // Pointer there is nothing to do.
   _DDatatbl             *pDispTable;        // IDispatch for the Table
   DTRowSet              *pDispRowSet;       // IDispatch for List of Rows
   DTColumnSet           *pDispColSet;       // IDispatch for List of Columns
   DTColumn              *pDispCol;          // IDispatch for single Column
   DTRow                 *pDispRow;          // IDispatch for single Row
   IDispatch             *pMainDispatch      // main IDispatch Interface
                                             // from ActiveX
   IDispatch             *pHelpDispatch      // We need this IDispatch
                                             // Interface to check return values
   CDTData               rowData;            // Class for an SAVEARRAY
   HRESULT               hr = E_FAIL;
   zLONG                 lDataType           // Information about the Datatype of a Column
   zSHORT                nDataLth            // Information about the max. Length of Data
   int                   nActiveXRowCount    // Information about the active Row
   int                   nActiveXColCounter; // Counter over all available Columns in the DataTable

   // Zeidon Variables for Mapping.
   zVIEW                 vApp;
   zKZWDLGXO_CtrlMap_DEF CtrlMapDef;
   zPCHAR                pchVariable;

   // These Function would be called if the Control has to get or set Data
   // from or to the Object Engine.
   if ( pzActiveX == 0 )
      return;

   pOleObject = pzActiveX->GetIOleObjectInterfacePtr( );
   if ( pOleObject == 0 )
      return;

   pOleObject->AddRef( );

   // First get the main IDispatch Interface.
   if ( !SUCCEEDED( pOleObject->QueryInterface( IID_IDispatch,
                                                (LPVOID *) &pMainDispatch ) ) )
      return;

   TRY
   {
      // The following lines are for Initializing the ActiveX Control for
      // SetData or GetData.  Create the main Wrapper Class Object
      ///////////////////////////////////////////////////////////////////////
      // Start of Init
      pDispTable = new _DDatatbl( pMainDispatch );
      if ( pDispTable == 0 )
         AfxThrowOleException( hr );

      // Create the RowSet Wrapper Class Object.
      pHelpDispatch = pDispTable->GetRowSet( );.
      if ( pHelpDispatch == 0 )
         AfxThrowOleException( E_NOINTERFACE );

      pDispRowSet = new DTRowSet( pHelpDispatch );
      if ( pDispRowSet == 0 )
         AfxThrowOleException( hr );

      // Create the ColumnSet Wrapper Class Object.
      pHelpDispatch = pDispTable->GetColumnSet( );
      if ( pHelpDispatch == 0 )
         AfxThrowOleException( E_NOINTERFACE );

      pDispColSet = new DTColumnSet( pHelpDispatch );
      // End of Init
      ///////////////////////////////////////////////////////////////////////

      if ( lMappingFlag == zSET_DATA_TO_CONTROL ) // Set data to the control
      {
         // 1. Set some optimization
         // We do not need a Refresh if we read our Data and Add Rows
         pDispTable->SetRedraw( FALSE );

         // Delete all rows, because we do a new initializing.
         pDispRowSet->Reset( );
      }

      // Create the SaveArray.
      rowData.CreateArray( pDispColSet->GetCount( ) );

      // Here I need information about the number of rows.
      if ( GetViewByName( &vApp, *(pzActiveX->m_pzsVName),
                          pzActiveX->m_pZSubtask->m_vDialog,
                          zLEVEL_ANY ) > 0 )
      {
         zSHORT nRC;

         // Loop thru all available rows.
         CreateViewFromViewForTask( &vApp, vApp, m_pZSubtask->m_vDialog );
         nRC = SetCursorFirstEntity( vApp, *(pzActiveX->m_pzsEName), 0 );
         while ( nRC == zCURSOR_SET )
         {
            if ( lMappingFlag == zGET_DATA_FROM_CONTROL )  // Get the Data
            {
               // We need a DTRow Interface to get the Data.
               pHelpDispatch = pDispRowSet->GetItem( nActiveXRowCount );
               if ( pHelpDispatch == 0 )
                  AfxThrowOleException( E_NOINTERFACE );
               if ( pDispRow )
               {
                  pDispRow->ReleaseDispatch( );
                  pDispRow->AttachDispatch( pHelpDispatch );
               }
               else
               {
                  pDispRow = new DTRow( pHelpDispatch );
                  if ( pDispRow == 0 )
                     AfxThrowOleException( hr );
               }

               rowData = pDispRow->GetValue( ); // This Line retrieves the Data from the ActiveX-Control
            }

            // Loop thru all columns.
            CtrlMapDef.CheckByte = 0xFE;
            CtrlMapDef.lStructSize = sizeof( zKZWDLGXO_CtrlMap_DEF );
            nActiveXColCounter = 0;
            nRC = SetCursorFirstEntity( pzActiveX->m_pZSubtask->m_vDialog, szlCtrlMap, 0 );
            while ( nRC == zCURSOR_SET && nActiveXColCounter < pDispColSet->GetCount( ) )
            {
               GetStructFromEntityAttrs( (zPCHAR) &CtrlMapDef, pzActiveX->m_pZSubtask->m_vDialog, szlCtrlMap );
               if ( CtrlMapDef.CheckByte != 0xFE )
                  MessageBox( 0, "CtrlMapDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

               // For every column get the Data Type and do the binding.
               pHelpDispatch = pDispColSet->GetItem( nActiveXColCounter );
               if ( pHelpDispatch == 0 )
                  AfxThrowOleException( E_NOINTERFACE );

               if ( pDispCol )
               {
                  pDispCol->ReleaseDispatch( );
                  pDispCol->AttachDispatch( pHelpDispatch );
               }
               else
               {
                  pDispCol = new DTColumn( pHelpDispatch );
                  if ( pDispCol == 0 )
                     AfxThrowOleException( hr );
               }

               // Now get information about the Data Type.
               lDataType = pDispCol->GetDataType( );
               nDataLth = pDispCol->GetDataLen( );

               pchVariable = new char[ nDataLth ];
               if ( lMappingFlag == zSET_DATA_TO_CONTROL )
               {
                  GetVariableFromAttribute( pchVariable, 0, zTYPE_STRING,
                                            nDataLth -1, vApp, CtrlMapDef.EN,
                                            CtrlMapDef.AN, CtrlMapDef.Context,
                                            CtrlMapDef.Context[ 0 ] ? 0 : zUSE_DEFAULT_CONTEXT );

                  // Now set the data to the ActiveX control.
                  rowData.SetData( pchVariable, lDataType,nActiveXColCounter );
               }
               else
               if ( lMappingFlag == zGET_DATA_FROM_CONTROL )
               {
                  // Get the value.
                  rowData.GetItem( pchVariable,nActiveXColCounter );
                  SetAttributeFromVariable( vApp, CtrlMapDef.EN,
                                            CtrlMapDef.AN, pchVariable,
                                            zTYPE_STRING, 0,
                                            CtrlMapDef.Context,
                                            CtrlMapDef.Context[ 0 ] ?
                                                  0 : zUSE_DEFAULT_CONTEXT );
               }

               mDeleteInitA( pchVariable );

               nRC = SetCursorNextEntity( pzActiveX->m_pZSubtask->m_vDialog,
                                          szlCtrlMap, 0 );
               nActiveXColCounter++;
            }

            if ( lMappingFlag == zSET_DATA_TO_CONTROL )
            {
               // Now set the Data
               pDispRowSet->Add( -2, rowData ); // add new row to the DataTable
            }

            nRC = SetCursorNextEntity( vApp, *(pzActiveX->m_pzsEName), 0 );
            nActiveXRowCount++;
         }
      }

      // Okay our work is done, let's allow painting to the DataTable.
      if ( lMappingFlag == zSET_DATA_TO_CONTROL )
      {
         pDispTable->SetRedraw( TRUE );
         pzActiveX->UpdateWindow( );
      }
   }
   CATCH( COleException,e )
   {
      AfxMessageBox( e->ReportError( ) );
   }
   END_CATCH

   // clean up
   mDeleteInit( pDispCol );
   mDeleteInit( pDispRow );
   mDeleteInit( pDispColSet );
   mDeleteInit( pDispRowSet );
   mDeleteInit( pDispTable );

   pOleObject->Release( );
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
//
// constants for generating C/C++ code
//
//
// formatstring for default include in cpp and c file;
LPCSTR lpszDEFAULT_INCLUDE =  "\r\n\
#define zGLOBAL_DATA\r\n\
#include \"%s\"// include Zeidon generated Header\r\n\r\n\
\r\n\
#ifdef _DEBUG\r\n\
#undef THIS_FILE\r\n  \
static char THIS_FILE[] = __FILE__;\r\n\
#endif\r\n";

// formatstring for controls header include if in append mode include in cpp and c file;
LPCSTR lpszDEFAULT_APPEND_INCLUDE =  "\r\n\
#include \"%s\"// include Zeidon generated Header\r\n\r\n\
#endif\r\n";


// standard include for DLL Extention
LPCSTR lpszMFC_EXTENSION = "\r\n\
#include \"afxdllx.h\"    // standard MFC Extension DLL routines\r\n\r\n\
static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };\r\n\
#include \"ZDr.h\"\r\n\
#include \"Zdctl.h\"\r\n\r\n";

// standard MFC includes
LPCSTR lpszDEFAULT_MFC_INCLUDE = "#define VC_EXTRALEAN\r\n\
#include \"zstdafx.h\"   // standard Zeidon MFC includes\r\n";


LPCSTR lpszDllMain = "\r\n\
extern \"C\" int APIENTRY\r\n\
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )\r\n\
{\r\n\
   UNREFERENCED_PARAMETER( lpReserved );\r\n\
\r\n\
   if ( dwReason == DLL_PROCESS_ATTACH )\r\n\
   {\r\n\
      g_hInstanceDLL = hInstance;\r\n\
      if ( AfxInitExtensionModule( extensionDLL, hInstance ) == 0 )\r\n\
         return( 0 );\r\n\
   }\r\n\
   else\r\n\
   if ( dwReason == DLL_PROCESS_DETACH )\r\n\
   {\r\n\
      AfxTermExtensionModule( extensionDLL );\r\n\
   }\r\n\
   return( 1 );   // ok\r\n\
}\r\n";

LPCSTR lpszZeidonInitRoutine = "\r\n\
extern \"C\" void WINAPI\r\n\
ZeidonInit%s( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,\r\n\
              zLONG lMapDlgCharX, zLONG lMapDlgCharY )\r\n\
{\r\n\
   new CDynLinkLibrary( extensionDLL );\r\n\
}\r\n";


LPCSTR lpszZeidonScriptInit = "\r\n\
extern \"C\" zOPER_EXPORT zSHORT OPERATION\r\n\
%s( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )\r\n\
{\r\n\
   #define ZCLASS_NAME %s \r\n\
   zSHORT nRC = 0;\r\n\
\r\n\
   if ( pzma == NULL ) // return immediate\r\n\
      return (0);\r\n\
\r\n\
   if ( lRequest == zACTIVEX_INITIALIZE )\r\n\
   {\r\n\
      // Default Zeidon Implementation\r\n\
      // Never change this code unless you want to crash!!!!!\r\n\
      IDispatch *pIDispatch = pzma->GetDispatchInterface();\r\n\
      if ( pIDispatch )\r\n\
      {\r\n\
         ZCLASS_NAME *pWrapperInstance = new ZCLASS_NAME( pIDispatch );\r\n\
         if ( pWrapperInstance )\r\n\
         {\r\n\
            pzma->SetWrapperInstance( pWrapperInstance );\r\n\
            nRC = 0;\r\n\
         }\r\n\
      \r\n\
         if ( nRC == 0 )\r\n\
         {\r\n\
            // to do:\r\n\
            // add additional initialization code here\r\n\
            // add multiple of those lines below to handle different events in this DLL\r\n\
            //( ( ZActiveX * ) pzma )->HandleEvent( this_is_the_controls_event_dispid );\r\n\
         }\r\n\
         return( 0 );\r\n\
      }\r\n\
   \r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_UNINITIALIZE )\r\n\
   {\r\n\
      // Default Zeidon Implementation\r\n\
      // Never change this code unless you want to crash!!!!!\r\n\
      // delete wrapper class instance\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance();\r\n\
      if ( pWrapperInstance )\r\n\
      {\r\n\
         delete pWrapperInstance;\r\n\
         pWrapperInstance = NULL;\r\n\
         pzma->SetWrapperInstance( pWrapperInstance );\r\n\
         return( 0 );\r\n\
      }\r\n\
   }\r\n\
   else";

LPCSTR lpszZeidonScriptMapping = "\r\n\
   if ( lRequest == zACTIVEX_TRANSFER_DATA_FROM_OI )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // add code to display OI's data with this control\r\n\
      return( 0 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_TRANSFER_DATA_TO_OI )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // add code to update OI's data with control's data\r\n\
      return( 0 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // return number of attributes to map here\r\n\
      return( 0 );\r\n\
   }\r\n\
   else";

LPCSTR lpszZeidonScriptMisc = "\r\n\
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // handle get focus here\r\n\
      return( 1 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_LOSE_FOCUS )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // handle lose focus here\r\n\
      return( 1 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_SET_CTRL_STATE )\r\n\
   {\r\n\
      // handle SetCtrlState here\r\n\
      return( pzma->SetZCtrlState( lFlags, ulParm ) );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_GET_CTRL_STATE )\r\n\
   {\r\n\
      // handle GetCtrlState here\r\n\
      return( pzma->GetZCtrlState( lFlags ) );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_HANDLE_EVENT )\r\n\
   {\r\n\
      // if we get a zACTIVEX_HANDLE_EVENT lFlags contains the dispatch id of the event. \r\n\
      // return zACTIVEX_EVENT_COMPLETED if you don't want to call user provided code\r\n\
      // return zACTIVEX_EVENT_CONTINUE  if you don't want to call user provided code in addition\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\r\n\
      switch ( lFlags )\r\n\
      {\r\n\
      case 0:\r\n\
         // add additional code to handle current event\r\n\
         nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code\r\n\
         break;\r\n\
      default:\r\n\
         nRC = zACTIVEX_EVENT_CONTINUE;\r\n\
         // uncommenting following line writes a message to Object Service window and hits performance\r\n\
         // TraceLineI(\"Userdll tries to handle unknown ActiveX Event: \", lFlags );\r\n\
         break;\r\n\
      }\r\n\
      return nRC;\r\n\
   }\r\n\
\r\n\
   return( nRC );\r\n\
}\r\n";



LPCSTR lpszZeidonDefinitions = "\r\n\
#include <kzoegdef.h>\r\n\r\n\
zEXTERN HINSTANCE  g_hInstanceDLL;";


LPCSTR lpszDISPATCH_DRIVER_BASE_CLASS  = " COleDispatchDriver";
LPCSTR lpszCPP_CLASS_DECLARATION       = "class AFX_CLASS_EXPORT %s : public %s\r\n{\r\n";
LPCSTR lpszCPP_TOKEN_PUBLIC            = "public:\r\n";
LPCSTR lpszPARAMTEMPLATE               = "param%d";

// formatstring for Invokehelper call
LPCSTR lpszINVOKE_HELPER               = "InvokeHelper( %s%s%s%s%s%s );";

// formatstring for GetProperty method
LPCSTR lpszGET_PROP_CPP = "%s %s::Get%s()\r\n{\r\n    %s result;\r\n    \
   GetProperty(0x%x, %s, (void*)&result);\r\n   return result;\r\n}";

// formatstring for GetProperty method
LPCSTR lpszSET_PROP_CPP = "void %s::Set%s(%s propVal)\r\n{\r\n    \
   SetProperty(0x%x, %s, propVal);\r\n}";

// strings for declaration of extern "C"....Methods
LPCSTR lpszExternCProlog = "\r\n\
#ifdef __cplusplus\r\n\
extern \"C\"\r\n\
{\r\n\
#endif\r\n";
LPCSTR lpszExternCEpilog = "\r\n\
#ifdef __cplusplus\r\n\
}\r\n\
#endif\r\n";

// string to look for where retrieving the last ordinal
// used in the def-file => ZEIDON_CURRENT_ORDINAL  1224
LPCSTR lpszOrdinalToken    = "ZEIDON_CURRENT_ORDINAL";
LPCSTR lpszFormatOrdinal   = "\r\n;;   %s\t%d";


// Translation table for VT_Constants to C-Stye
static TCHAR* _szVTtoC[] =
{
    _T("void"),             //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
    _T("null"),             //VT_NULL            = 1,   /* [V]        SQL style Null              */
    _T("short"),            //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
    _T("long"),             //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
    _T("float"),           //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
    _T("double"),           //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
    _T("CURRENCY"),         //VT_CY              = 6,   /* [V][T][P]  currency                    */
    _T("DATE"),             //VT_DATE            = 7,   /* [V][T][P]  date                        */
    _T("BSTR"),             //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
    _T("IDispatch*"),       //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
    _T("SCODE"),            //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
    _T("BOOL"),          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
    _T("VARIANT"),          //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
    _T("IUnknown*"),        //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
    _T("wchar_t"),          //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
    _T(""),                 //                   = 15,
    _T("char"),             //VT_I1              = 16,  /*    [T]     signed char                */
    _T("unsigned char"),             //VT_UI1             = 17,  /*    [T]     unsigned char               */
    _T("unsigned short"),           //VT_UI2             = 18,  /*    [T]     unsigned short              */
    _T("unsigned long"),            //VT_UI4             = 19,  /*    [T]     unsigned short              */
    _T("int64"),            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
    _T("uint64"),           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
    _T("int"),              //VT_INT             = 22,  /*    [T]     signed machine int          */
    _T("unsigned int"),             //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
    _T("void"),             //VT_VOID            = 24,  /*    [T]     C style void                */
    _T("HRESULT"),          //VT_HRESULT         = 25,  /*    [T]                                 */
    _T("PTR"),              //VT_PTR             = 26,  /*    [T]     pointer type                */
    _T("SAFEARRAY"),        //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
    _T("CARRAY"),           //VT_CARRAY          = 28,  /*    [T]     C style array               */
    _T("USERDEFINED"),      //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
    _T("LPSTR"),            //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
    _T("LPWSTR"),           //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
    _T(""),                 //                   = 32,
    _T(""),                 //                   = zTAG_LTH,
    _T(""),                 //                   = 34,
    _T(""),                 //                   = 35,
    _T(""),                 //                   = 36,
    _T(""),                 //                   = 37,
    _T(""),                 //                   = 38,
    _T(""),                 //                   = 39,
    _T(""),                 //                   = 40,
    _T(""),                 //                   = 41,
    _T(""),                 //                   = 42,
    _T(""),                 //                   = 43,
    _T(""),                 //                   = 44,
    _T(""),                 //                   = 45,
    _T(""),                 //                   = 46,
    _T(""),                 //                   = 47,
    _T(""),                 //                   = 48,
    _T(""),                 //                   = 49,
    _T(""),                 //                   = 50,
    _T(""),                 //                   = 51,
    _T(""),                 //                   = 52,
    _T(""),                 //                   = 53,
    _T(""),                 //                   = 54,
    _T(""),                 //                   = 55,
    _T(""),                 //                   = 56,
    _T(""),                 //                   = 57,
    _T(""),                 //                   = 58,
    _T(""),                 //                   = 59,
    _T(""),                 //                   = 60,
    _T(""),                 //                   = 61,
    _T(""),                 //                   = 62,
    _T(""),                 //                   = 63,
    _T("FILETIME"),         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
    _T("BLOB"),             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
    _T("STREAM"),           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
    _T("STORAGE"),          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
    _T("STREAMED_OBJECT"),  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
    _T("STORED_OBJECT"),    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
    _T("BLOB_OBJECT"),      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
    _T("CF"),               //VT_CF              = 71,  /*       [P]  Clipboard format            */
    _T("CLSID"),            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};


// Translation table used for DispatchDriver... Implementation
static TCHAR* _szVTtoVTDispatchCPP[] =
{
    _T("EMPTY"),         //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
    _T("NULL"),          //VT_NULL            = 1,   /* [V]        SQL style Null              */
    _T("I2"),            //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
    _T("I4"),            //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
    _T("R4"),            //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
    _T("R8"),            //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
    _T("CY"),            //VT_CY              = 6,   /* [V][T][P]  currency                    */
    _T("DATE"),          //VT_DATE            = 7,   /* [V][T][P]  date                        */
    _T("BSTR"),          //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
    _T("DISPATCH"),      //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
    _T("ERROR"),         //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
    _T("BOOL"),          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
    _T("VARIANT"),       //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
    _T("UNKNOWN"),       //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
    _T("WBSTR"),         //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
    _T(""),                 //                   = 15,
    _T("I1"),            //VT_I1              = 16,  /*    [T]     signed char                */
    _T("UI1"),           //VT_UI1             = 17,  /*    [T]     unsigned char               */
    _T("UI2"),           //VT_UI2             = 18,  /*    [T]     unsigned short              */
    _T("UI4"),           //VT_UI4             = 19,  /*    [T]     unsigned short              */
    _T("I8"),            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
    _T("UI8"),           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
    _T("INT"),           //VT_INT             = 22,  /*    [T]     signed machine int          */
    _T("UINT"),          //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
    _T("VOID"),          //VT_VOID            = 24,  /*    [T]     C style void                */
    _T("HRESULT"),       //VT_HRESULT         = 25,  /*    [T]                                 */
    _T("PTR"),           //VT_PTR             = 26,  /*    [T]     pointer type                */
    _T("SAFEARRAY"),     //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
    _T("CARRAY"),        //VT_CARRAY          = 28,  /*    [T]     C style array               */
    _T("USERDEFINED"),   //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
    _T("LPSTR"),         //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
    _T("LPWSTR"),        //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
    _T(""),                 //                   = 32,
    _T(""),                 //                   = zTAG_LTH,
    _T(""),                 //                   = 34,
    _T(""),                 //                   = 35,
    _T(""),                 //                   = 36,
    _T(""),                 //                   = 37,
    _T(""),                 //                   = 38,
    _T(""),                 //                   = 39,
    _T(""),                 //                   = 40,
    _T(""),                 //                   = 41,
    _T(""),                 //                   = 42,
    _T(""),                 //                   = 43,
    _T(""),                 //                   = 44,
    _T(""),                 //                   = 45,
    _T(""),                 //                   = 46,
    _T(""),                 //                   = 47,
    _T(""),                 //                   = 48,
    _T(""),                 //                   = 49,
    _T(""),                 //                   = 50,
    _T(""),                 //                   = 51,
    _T(""),                 //                   = 52,
    _T(""),                 //                   = 53,
    _T(""),                 //                   = 54,
    _T(""),                 //                   = 55,
    _T(""),                 //                   = 56,
    _T(""),                 //                   = 57,
    _T(""),                 //                   = 58,
    _T(""),                 //                   = 59,
    _T(""),                 //                   = 60,
    _T(""),                 //                   = 61,
    _T(""),                 //                   = 62,
    _T(""),                 //                   = 63,
    _T("FILETIME"),         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
    _T("BLOB"),             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
    _T("STREAM"),           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
    _T("STORAGE"),          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
    _T("STREAMED_OBJECT"),  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
    _T("STORED_OBJECT"),    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
    _T("BLOB_OBJECT"),      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
    _T("CF"),               //VT_CF              = 71,  /*       [P]  Clipboard format            */
    _T("CLSID"),            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};


//
// the string returned by this function is the name of the C++ variable type
// that is used when calling the function COleDispatchDriver::InvokeHelper
// or IDispatch::Invoke
//
CString
VTtoCType( VARTYPE vt )
{
    CString str;
    vt &= ~0xF000;
    if (vt <= VT_CLSID)
       str = _szVTtoC[vt];
    else
       str = "invalid Variable Type";

    return str;
}

//
// the string returned by this function is the name of the return type
// that is used when calling the function COleDispatchDriver::InvokeHelper
// or IDispatch::Invoke
//
CString
VTtoDispatchType( VARTYPE vt )
{
    CString str;
    vt &= ~0xF000;
    if (vt <= VT_CLSID)
    {
       str = "VT_";
       str += _szVTtoVTDispatchCPP[vt];
    }
    else
       str = "invalid Variable Type";

    return str;
}

//
// the string returned by this function is the name of the VARIANT type
// that is used in the array of paramater types
// when calling the function COleDispatchDriver::InvokeHelper
// or IDispatch::Invoke
//
CString
VTtoVariantType( VARTYPE vt )
{
   CString str;

   vt &= ~0xF000;

   if (vt <= VT_CLSID)
   {
      str += _szVTtoVTDispatchCPP[vt];
   }
   else
      str = "invalid Variable Type";

   return str;
}

//
// returns a string for a predefined DISPID if id is a predefined id
// usually these predefined have a negative value and reach from -500 to -804
// and are not numbered subsequently (that is the reason for this ugly implementation below)
//
CString
PredefinedDispID( DISPID id )
{
   CString strOut;
   switch ( id )
   {
      case  DISPID_AUTOSIZE :
         strOut = _T("DISPID_AUTOSIZE");
         break;
      case  DISPID_BACKCOLOR :
         strOut = _T("DISPID_BACKCOLOR");
         break;
      case  DISPID_BACKSTYLE :
         strOut = _T("DISPID_BACKSTYLE");
         break;
      case  DISPID_BORDERCOLOR :
         strOut = _T("DISPID_BORDERCOLOR");
         break;
      case  DISPID_BORDERSTYLE :
         strOut = _T("DISPID_BORDERSTYLE");
         break;
      case  DISPID_BORDERWIDTH :
         strOut = _T("DISPID_BORDERWIDTH");
         break;
      case  DISPID_DRAWMODE :
         strOut = _T("DISPID_DRAWMODE");
         break;
      case  DISPID_DRAWSTYLE :
         strOut = _T("DISPID_DRAWSTYLE");
         break;
      case  DISPID_DRAWWIDTH :
         strOut = _T("DISPID_DRAWWIDTH");
         break;
      case  DISPID_FILLCOLOR :
          strOut = _T("DISPID_FILLCOLOR");
         break;
      case  DISPID_FILLSTYLE :
          strOut = _T("DISPID_FILLSTYLE");
         break;
      case  DISPID_FONT :
         strOut = _T("DISPID_FONT");
         break;
      case  DISPID_FORECOLOR :
         strOut = _T("DISPID_FORECOLOR");
         break;
      case  DISPID_ENABLED :
         strOut = _T("DISPID_ENABLED");
         break;
      case  DISPID_HWND :
         strOut = _T("DISPID_HWND");
         break;
      case  DISPID_TABSTOP :
         strOut = _T("DISPID_TABSTOP");
         break;
      case  DISPID_TEXT :
         strOut = _T("DISPID_TEXT");
         break;
      case  DISPID_CAPTION :
         strOut = _T("DISPID_CAPTION");
         break;
      case  DISPID_BORDERVISIBLE :
         strOut = _T("DISPID_BORDERVISIBLE");
         break;
      case  DISPID_APPEARANCE :
         strOut = _T("DISPID_APPEARANCE");
         break;
      case  DISPID_MOUSEPOINTER :
         strOut = _T("DISPID_MOUSEPOINTER");
         break;
      case  DISPID_MOUSEICON :
         strOut = _T("DISPID_MOUSEICON");
         break;
      case  DISPID_PICTURE :
         strOut = _T("DISPID_PICTURE");
         break;
      case  DISPID_VALID :
         strOut = _T("DISPID_VALID");
         break;
      case  DISPID_READYSTATE :
         strOut = _T("DISPID_READYSTATE");
         break;
#ifdef DISPID_LISTINDEX
      case  DISPID_LISTINDEX :
         strOut = _T("DISPID_LISTINDEX");
         break;
      case  DISPID_SELECTED :
         strOut = _T("DISPID_SELECTED");
         break;
      case  DISPID_LIST :
         strOut = _T("DISPID_LIST");
         break;
      case  DISPID_COLUMN :
         strOut = _T("DISPID_COLUMN");
         break;
      case  DISPID_LISTCOUNT :
         strOut = _T("DISPID_LISTCOUNT");
         break;
      case  DISPID_MULTISELECT :
         strOut = _T("DISPID_MULTISELECT");
         break;
      case  DISPID_MAXLENGTH :
         strOut = _T("DISPID_MAXLENGTH");
         break;
      case  DISPID_PASSWORDCHAR :
         strOut = _T("DISPID_PASSWORDCHAR");
         break;
      case  DISPID_SCROLLBARS :
         strOut = _T("DISPID_SCROLLBARS");
         break;
      case  DISPID_WORDWRAP :
         strOut = _T("DISPID_WORDWRAP");
         break;
      case  DISPID_MULTILINE :
         strOut = _T("DISPID_MULTILINE");
         break;
      case  DISPID_NUMBEROFROWS :
         strOut = _T("DISPID_NUMBEROFROWS");
         break;
      case  DISPID_NUMBEROFCOLUMNS :
         strOut = _T("DISPID_NUMBEROFCOLUMNS");
         break;
      case  DISPID_DISPLAYSTYLE :
         strOut = _T("DISPID_DISPLAYSTYLE");
         break;
      case  DISPID_GROUPNAME :
         strOut = _T("DISPID_GROUPNAME");
         break;
      case  DISPID_IMEMODE :
         strOut = _T("DISPID_IMEMODE");
         break;
      case  DISPID_ACCELERATOR :
         strOut = _T("DISPID_ACCELERATOR");
         break;
      case  DISPID_ENTERKEYBEHAVIOR :
         strOut = _T("DISPID_ENTERKEYBEHAVIOR");
         break;
      case  DISPID_TABKEYBEHAVIOR :
         strOut = _T("DISPID_TABKEYBEHAVIOR");
         break;
      case  DISPID_SELTEXT :
         strOut = _T("DISPID_SELTEXT");
         break;
      case  DISPID_SELSTART :
         strOut = _T("DISPID_SELSTART");
         break;
      case  DISPID_SELLENGTH :
         strOut = _T("DISPID_SELLENGTH");
         break;
      case  DISPID_REFRESH :
         strOut = _T("DISPID_REFRESH");
         break;
      case  DISPID_DOCLICK :
         strOut = _T("DISPID_DOCLICK");
         break;
      case  DISPID_ABOUTBOX :
         strOut = _T("DISPID_ABOUTBOX");
         break;
      case  DISPID_ADDITEM :
         strOut = _T("DISPID_ADDITEM");
         break;
      case  DISPID_CLEAR :
         strOut = _T("DISPID_CLEAR");
         break;
      case  DISPID_REMOVEITEM :
         strOut = _T("DISPID_REMOVEITEM");
         break;
      case  DISPID_CLICK :
         strOut = _T("DISPID_CLICK");
         break;
      case  DISPID_DBLCLICK :
         strOut = _T("DISPID_DBLCLICK");
         break;
      case  DISPID_KEYDOWN :
         strOut = _T("DISPID_KEYDOWN");
         break;
      case  DISPID_KEYPRESS :
         strOut = _T("DISPID_KEYPRESS");
         break;
      case  DISPID_KEYUP :
         strOut = _T("DISPID_KEYUP");
         break;
      case  DISPID_MOUSEDOWN :
         strOut = _T("DISPID_MOUSEDOWN");
         break;
      case  DISPID_MOUSEMOVE :
         strOut = _T("DISPID_MOUSEMOVE");
         break;
      case  DISPID_MOUSEUP :
         strOut = _T("DISPID_MOUSEUP");
         break;
      case  DISPID_ERROREVENT :
         strOut = _T("DISPID_ERROREVENT");
         break;
      case  DISPID_READYSTATECHANGE :
         strOut = _T("DISPID_READYSTATECHANGE");
         break;
      case  DISPID_CLICK_VALUE :
         strOut = _T("DISPID_CLICK_VALUE");
         break;
      case  DISPID_RIGHTTOLEFT :
         strOut = _T("DISPID_RIGHTTOLEFT");
         break;
      case  DISPID_TOPTOBOTTOM :
         strOut = _T("DISPID_TOPTOBOTTOM");
         break;
      case  DISPID_THIS :
         strOut = _T("DISPID_THIS");
         break;
      case  DISPID_AMBIENT_BACKCOLOR :
         strOut = _T("DISPID_AMBIENT_BACKCOLOR");
         break;
      case  DISPID_AMBIENT_DISPLAYNAME :
         strOut = _T("DISPID_AMBIENT_DISPLAYNAME");
         break;
      case  DISPID_AMBIENT_FONT :
         strOut = _T("DISPID_AMBIENT_FONT");
         break;
      case  DISPID_AMBIENT_FORECOLOR :
         strOut = _T("DISPID_AMBIENT_FORECOLOR");
         break;
      case  DISPID_AMBIENT_LOCALEID :
         strOut = _T("DISPID_AMBIENT_LOCALEID");
         break;
      case  DISPID_AMBIENT_MESSAGEREFLECT :
         strOut = _T("DISPID_AMBIENT_MESSAGEREFLECT");
         break;
      case  DISPID_AMBIENT_SCALEUNITS :
         strOut = _T("DISPID_AMBIENT_SCALEUNITS");
         break;
      case  DISPID_AMBIENT_TEXTALIGN :
         strOut = _T("DISPID_AMBIENT_TEXTALIGN");
         break;
      case  DISPID_AMBIENT_USERMODE :
         strOut = _T("DISPID_AMBIENT_USERMODE");
         break;
      case  DISPID_AMBIENT_UIDEAD :
         strOut = _T("DISPID_AMBIENT_UIDEAD");
         break;
      case  DISPID_AMBIENT_SHOWGRABHANDLES :
         strOut = _T("DISPID_AMBIENT_SHOWGRABHANDLES");
         break;
      case  DISPID_AMBIENT_SHOWHATCHING :
         strOut = _T("DISPID_AMBIENT_SHOWHATCHING");
         break;
      case  DISPID_AMBIENT_DISPLAYASDEFAULT :
         strOut = _T("DISPID_AMBIENT_DISPLAYASDEFAULT");
         break;
      case  DISPID_AMBIENT_SUPPORTSMNEMONICS :
         strOut = _T("DISPID_AMBIENT_SUPPORTSMNEMONICS");
         break;
      case  DISPID_AMBIENT_AUTOCLIP :
         strOut = _T("DISPID_AMBIENT_AUTOCLIP");
         break;
      case  DISPID_AMBIENT_APPEARANCE :
         strOut = _T("DISPID_AMBIENT_APPEARANCE");
         break;
      case  DISPID_AMBIENT_CODEPAGE :
         strOut = _T("DISPID_AMBIENT_CODEPAGE");
         break;
      case  DISPID_AMBIENT_PALETTE :
         strOut = _T("DISPID_AMBIENT_PALETTE");
         break;
      case  DISPID_AMBIENT_CHARSET :
         strOut = _T("DISPID_AMBIENT_CHARSET");
         break;
      case  DISPID_AMBIENT_TRANSFERPRIORITY :
         strOut = _T("DISPID_AMBIENT_TRANSFERPRIORITY");
         break;
      case  DISPID_AMBIENT_RIGHTTOLEFT :
         strOut = _T("DISPID_AMBIENT_RIGHTTOLEFT");
         break;
      case  DISPID_AMBIENT_TOPTOBOTTOM :
         strOut = _T("DISPID_AMBIENT_TOPTOBOTTOM");
         break;
#endif
      case  DISPID_Name :
         strOut = _T("DISPID_Name");
         break;
      case  DISPID_Delete :
         strOut = _T("DISPID_Delete");
         break;
      case  DISPID_Object :
         strOut = _T("DISPID_Object");
         break;
      case  DISPID_Parent :
         strOut = _T("DISPID_Parent");
         break;
      default:
         strOut.Empty();
         break;
   }

   return strOut;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZTlbBaseProperties, CObject )

ZTlbBaseProperties::ZTlbBaseProperties()
{
   m_bstrName           = NULL;
   m_bstrDocumentation  = NULL;
   m_ulHelpContext      = NULL;
   m_bstrHelpFile       = NULL;
}

ZTlbBaseProperties::~ZTlbBaseProperties()
{
   if (m_bstrName)
   {
      SysFreeString(m_bstrName);
   }

   if ( m_bstrDocumentation )
   {
      SysFreeString( m_bstrDocumentation );
   }

   if ( m_bstrHelpFile )
   {
      SysFreeString( m_bstrHelpFile );
   }
}

CString
ZTlbBaseProperties::GetName()
{
   CString s;

   s.Empty();
   if ( m_bstrName )
   {
      s = m_bstrName;
   }

   return s;
}

CString
ZTlbBaseProperties::GetDocumentation()
{
   CString s;

   s.Empty();

   if ( m_bstrDocumentation )
   {
      s = m_bstrDocumentation;
   }
   return s;
}

CString
ZTlbBaseProperties::GetDocumentationAsComment()
{
   int i = 0;
   int j = 0;
   CString strHelp;

   CString strComment = GetDocumentation();
   // if empty documentation there is nothing to do
   if ( strComment.IsEmpty() )
   {
      return strHelp;
   }

   // if documentation is longer than 100 characters
   // insert some CR for formating purpose no matter
   // if there is a word or whitespace -> break it
   strHelp += "/*";
   strHelp += CRLF;
   j = strComment.GetLength();
   if ( i > 100)
   {
      while ( j > 0 )
      {
         strHelp+=strComment.Mid(i, 100);
         strHelp+=CRLF;
         i+=100;
         j-=100;
      }
   }
   else
   {
      strHelp += strComment;
   }
   strHelp += CRLF;
   strHelp += "*/";

   return strHelp;
}

CString
ZTlbBaseProperties::GetHelpfile()
{
   CString s;

   s.Empty();

   if ( m_bstrHelpFile )
   {
      s = m_bstrHelpFile;
   }
   return s;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZTypeLibrary

IMPLEMENT_DYNAMIC( ZTypeLibrary, CObject )

ZTypeLibrary::ZTypeLibrary()
{
   m_TypeLib            = NULL;
   m_bstrName           = NULL;
   m_bstrDocumentation  = NULL;
   m_bstrHelpFile       = NULL;
   m_bAppend            = FALSE;
}

ZTypeLibrary::~ZTypeLibrary()
{
   DestroyContent();
}

void
ZTypeLibrary::DestroyContent()
{
   if ( m_TypeLib )
   {
      m_TypeLib->Release();
      // Set to NULL to prevent destructor from attempting to free again
      m_TypeLib = NULL;
   }

   // destroy all dispatch type information
   while ( !m_pdispTypeInfos.IsEmpty() )
   {
      CObject *p = m_pdispTypeInfos.RemoveHead();
      delete p;
   }
   // destroy all Dispatches that are use for generating global ops.
   while ( !m_lGlobalDispatch.IsEmpty() )
   {
      CObject *p = m_lGlobalDispatch.RemoveHead();
      delete p;
   }

   // destroy all entries that are needed to display ops. in editor
   while ( !m_lGlobalOperations.IsEmpty() )
   {
      CObject *p = m_lGlobalOperations.RemoveHead();
      delete p;
   }
}

BOOL
ZTypeLibrary::LoadTypeLibrary( LPCSTR lpszLibraryName, LPCSTR lpszGUID )
{
   USES_CONVERSION;
   HRESULT hr = NULL;
   TLIBATTR *lptlAttributes;
   BOOL bReturn = TRUE;
   int j = 0;

   m_csGUID = lpszGUID;

   // Clean the object Instance so that this object instance
   // can be reused without destroying and recreating the object
   // via delete and new
   DestroyContent();

   if ( m_TypeLib )
   {
      m_TypeLib->Release();
   }

   TRY
   {
      // extract name without extension
      m_csLibraryFileName = lpszLibraryName;
      j = m_csLibraryFileName.ReverseFind('.');
      m_csLibraryFileName = m_csLibraryFileName.Mid( 0, m_csLibraryFileName.GetLength() - j );

      hr = LoadTypeLibEx( T2OLE( lpszLibraryName ),
                          REGKIND_DEFAULT, &m_TypeLib );
      if ( !SUCCEEDED( hr ) )
      {
         AfxThrowOleException(hr);
      }
      //retrieving Documentation about the Typelib -1 means documentation about
      //the lib itself
      hr = m_TypeLib->GetDocumentation( -1, &m_bstrName,
                                        &m_bstrDocumentation,
                                        &m_ulHelpContext,
                                        &m_bstrHelpFile );
      if ( !SUCCEEDED( hr ) )
      {
         AfxThrowOleException( hr );
      }

      //retrieving language and version information
      hr = m_TypeLib->GetLibAttr( &lptlAttributes );
      if ( !SUCCEEDED( hr ) )
      {
         AfxThrowOleException(hr);
      }
      m_guid = lptlAttributes->guid;
      m_lcid = lptlAttributes->lcid;
      m_wMajorVer = lptlAttributes->wMajorVerNum;
      m_wMinorVer = lptlAttributes->wMinorVerNum;
      m_TypeLib->ReleaseTLibAttr(lptlAttributes);
      m_TypeLib->AddRef();
      BrowseLibComplete();
   }
   CATCH( COleException, e )
   {
      // In VC 5.0 no messagetext is available for STG_E_FILENOTFOUND;
      // It's a boring message when there is no message, so out with it!

      //Avoid multiple error messages when browsing typelibraries fails.
   // if ( _HRESULT_TYPEDEF_( e->m_sc ) != STG_E_FILENOTFOUND )
   // {
   //    e->ReportError();
   // }

      DestroyContent();
      bReturn  = FALSE;
   }
   END_CATCH

   return bReturn;
}

long
ZTypeLibrary::BrowseLibComplete()
{
   HRESULT hr;
   ZTypeInfo      *pTypeInfo;
   ZInterface     *pInterface;
   ZDispinterface *pDispinterface;
   ZModule        *pModule;
   ZCoClass       *pCoClass;
   ZEnum          *pEnum;
   ZAlias         *pAlias;
   ZStruct        *pStruct;
   ZUnion         *pUnion;
   LPTYPEINFO ptinfo = NULL;
   unsigned int cTypeInfo;
   unsigned int n;
   TYPEKIND typekind;

   if ( m_pdispTypeInfos.IsEmpty())
   {
      TRY
      {
         cTypeInfo = m_TypeLib->GetTypeInfoCount();
         // Enumerate the typeinfos in this type library
         for ( n = 0; n < cTypeInfo; n++ )
         {
            hr = m_TypeLib->GetTypeInfo( n, &ptinfo );
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = m_TypeLib->GetTypeInfoType(n, &typekind);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }

            switch (typekind)
            {
               case TKIND_INTERFACE:
                  pInterface = new ZInterface(ptinfo, this, n);
                  m_pdispTypeInfos.AddTail(pInterface);
                  break;

               case TKIND_DISPATCH:
                  pDispinterface = new ZDispinterface(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pDispinterface);
                  break;

               case TKIND_MODULE:
                  pModule = new ZModule(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pModule);
                  break;

               case TKIND_COCLASS:
                  pCoClass = new ZCoClass(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pCoClass);
                  break;

               case TKIND_ENUM:
                  pEnum = new ZEnum(ptinfo,this );
                  m_pdispTypeInfos.AddTail(pEnum);
                  break;

               case TKIND_ALIAS:
                  pAlias = new ZAlias(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pAlias);
                  break;

               case TKIND_RECORD:
                  pStruct = new ZStruct(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pStruct);
                  break;

               case TKIND_UNION:
                  pUnion = new ZUnion(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pUnion);
                  break;

               default:
                  // Currently unsupported TKINDs
                  pTypeInfo = new ZTypeInfo(ptinfo, this );
                  m_pdispTypeInfos.AddTail(pTypeInfo);
                  break;
            }
         }

         // Work is done ... release interface
         ptinfo->Release();
         ptinfo = NULL;
      }
      CATCH(COleException,e)
      {
         e->ReportError( );
      }
      END_CATCH
   }

   return m_pdispTypeInfos.GetCount();
}

CString
ZTypeLibrary::GetGUID_AsString()
{
   LPOLESTR pszGUID = NULL;

   StringFromCLSID( m_guid, &pszGUID );

   return( pszGUID );
}

POSITION
ZTypeLibrary::GetFirstActiveX_OperationPosition( )
{
   if ( m_lGlobalOperations.GetCount() == 0 )
      PopulateGlobalOperations();

   return( m_lGlobalOperations.GetHeadPosition( ) );
}

ZOperation *
ZTypeLibrary::GetNextOperation( POSITION& pos )
{
   return( (ZOperation *) m_lGlobalOperations.GetNext( pos ) );
}

POSITION
ZTypeLibrary::GetFirstGlobalDispatchPosition()
{
   if ( m_lGlobalDispatch.GetCount() == 0 )
      PopulateGlobalOperations();

   return( m_lGlobalDispatch.GetHeadPosition() );
}

ZGlobalDispatch *
ZTypeLibrary::GetNextGlobalDispatch( POSITION& pos )
{
   return( (ZGlobalDispatch *) m_lGlobalDispatch.GetNext( pos ) );
}

POSITION
ZTypeLibrary::GetFirstExternalClassPosition()
{
   if ( m_lExternalClasses.GetCount() == 0 )
      QueryExternalClasses();

   return( m_lExternalClasses.GetHeadPosition() );
}

CString
ZTypeLibrary::GetNextExternalClass( POSITION& pos )
{
   return( m_lExternalClasses.GetNext( pos ) );
}


// This function creates a list of objects
// for which code has to be generate
long
ZTypeLibrary::CheckDependencies ()
{
   POSITION pos = NULL;
   CStringList lstrObjectNames;

   // Browse TypeLibrary for CoClasses as initial interfaces
   //
   pos = m_pdispTypeInfos.GetHeadPosition();
   while ( pos )
   {
      ZTypeInfo *t = (ZTypeInfo *) m_pdispTypeInfos.GetAt( pos );

      if ( t->GetTypeKind() == TKIND_COCLASS )
      {
         CString strGUID = ( (ZCoClass *) t)->GetDefaultDispatchInterfaceGUID();
         ZDispinterface *pDispinterface = GetDispatchOfGUID( strGUID );
         // if a Dispach interface is found loop through all Methods
         // and properties, get their return types and store them
         // in a list of strings
         if ( pDispinterface )
         {
            if ( lstrObjectNames.Find( pDispinterface->GetName() ) == NULL )
            {
               lstrObjectNames.AddTail( pDispinterface->GetName() );
            }
         }
      }
      m_pdispTypeInfos.GetNext( pos );
   }

   // assume that scanning return types is sufficiant for dependency checking
   // we now have a list of all CoClasses that have to be generated
   // Now check this list against dispatch interfaces.
   // If there is a dispatch interface their dependencies must be checked to

   pos = lstrObjectNames.GetHeadPosition();
   while ( pos )
   {
   // TRACE1("Type %s\n", lstrObjectNames.GetAt(pos) );
      //Retrieving the named dispatch interface
      ZDispinterface *pDispinterface = FindDispatchByName( lstrObjectNames.GetAt(pos) );
      if ( pDispinterface )
      {
         // this call forces the list to increase
         // but the names are unique so we should not step into an endless loop
         BuildStringListOfReturnTypes( &lstrObjectNames, pDispinterface );
      }
      lstrObjectNames.GetNext(pos);
   }

   m_lDependendInterfaces.RemoveAll();

   pos = lstrObjectNames.GetHeadPosition();
   while ( pos )
   {
      // Retrieving the named dispatch interface
      // there are a bunch of entries that are not dispatch interfaces
      // the are eliminated when FindDispatchByName returns NULL
      ZDispinterface *pDispinterface = FindDispatchByName( lstrObjectNames.GetAt(pos) );
      if ( pDispinterface )
      {
         m_lDependendInterfaces.AddTail( pDispinterface );
      }
      lstrObjectNames.GetNext(pos);
   }
   return lstrObjectNames.GetCount();
}


long
ZTypeLibrary::BuildStringListOfReturnTypes( CStringList *sl, ZDispinterface * pDispinterface  )
{
   CObList *l = pDispinterface ->GetProperties();
   POSITION pos = l->GetHeadPosition();
   while ( pos )
   {
      ZProperty *f = (ZProperty *) l->GetAt( pos );
      // get geturn type
      CString strReturnType = f->GetType()->GetCPPDefinition();
      // remove * if pointer
      int k = strReturnType .ReverseFind( '*' );
      if ( k >= 0 )
         strReturnType  = strReturnType .Mid( 0, k );

      if ( !strReturnType.IsEmpty() )
      {
         if ( sl->Find( strReturnType ) == NULL )
         {
            sl->AddTail( strReturnType );
         }
      }
      l->GetNext( pos );
   }

   l = pDispinterface->GetMethods();
   pos = l->GetHeadPosition();
   while ( pos )
   {
      ZFunction *f = (ZFunction *)l->GetAt( pos );
      CString strReturnType = f->GetType()->GetCPPDefinition();
      // remove * if pointer
      int k = strReturnType .ReverseFind( '*' );
      if ( k >= 0 )
         strReturnType  = strReturnType .Mid( 0, k );

      if ( !strReturnType.IsEmpty() )
      {
         if ( sl->Find( strReturnType ) == NULL )
         {
            sl->AddTail( strReturnType );
         }
      }
      l->GetNext( pos );
   }
   return sl->GetCount();
}

ZDispinterface *
ZTypeLibrary::FindDispatchByName( CString strName )
{
   CObject *o = NULL;
   ZDispinterface *pDI = NULL;

   // iterate all the dispatch interfaces and check the name
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   while ( pos && pDI == NULL )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(ZDispinterface)))
      {
         if ( ((ZDispinterface *)o)->GetName() == strName )
         {
            pDI = (ZDispinterface *)o;
         }
      }
      m_pdispTypeInfos.GetNext( pos );
   }

   return pDI;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZTypeInfo, ZTlbBaseProperties )

ZTypeInfo::ZTypeInfo( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib ) :
           ZTlbBaseProperties( )
{
   m_pTypeLib = pTypeLib;
   if ( ptinfo != NULL )
   {
      InitTypeInfo( ptinfo );
   }
}

ZTypeInfo::~ZTypeInfo()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Function Name  : InitTypeInfo
//                  Retrives the attributes of the current typeinfo
//
//
// Parameter      : ptinfo, I,   Pointer to a typeinfo structure
//
// Return         : TRUE when retrieving was successfull
//                  otherwise FALSE
//
/////////////////////////////////////////////////////////////////////////////
BOOL
ZTypeInfo::InitTypeInfo( LPTYPEINFO ptinfo )
{
   HRESULT hr = 0;
   LPTYPELIB ptlib = NULL;
   UINT nIndex = 0;
   LPTYPEATTR ptypeattr = NULL;
   BOOL bReturn = FALSE;

   TRY
   {
      // Get information from TYPEATTR
      hr = ptinfo->GetTypeAttr(&ptypeattr);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }
      m_typekind = ptypeattr->typekind;
      m_guid = ptypeattr->guid;
      m_wTypeFlags = ptypeattr->wTypeFlags;

      ptinfo->ReleaseTypeAttr(ptypeattr);

      hr = ptinfo->GetContainingTypeLib(&ptlib, &nIndex);
      if (!SUCCEEDED(hr))
      {
        //Supportes runtime typeinfo browsing. (Type info
        //obtained from IDispatch::GetTypeInfo does not have a containing
        //type library).
        if (GetScode(hr) == E_NOTIMPL)
           bReturn = TRUE;
        else
           AfxThrowOleException(hr);
      }
      else
      {
         hr = ptlib->GetDocumentation(nIndex, &m_bstrName, &m_bstrDocumentation,
                &m_ulHelpContext, &m_bstrHelpFile);
         if (!SUCCEEDED(hr))
              AfxThrowOleException(hr);
      }
      ptlib->Release();
   }
   CATCH( COleException,e )
   {
      e->ReportError( );
      bReturn = FALSE;
   }
   END_CATCH

   return( bReturn );
}

CString
ZTypeInfo::GetGUID_AsString()
{
   LPOLESTR pszGUID = NULL;

   StringFromCLSID( m_guid, &pszGUID );

   return( pszGUID );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZInterface, ZTypeInfo )

ZInterface::ZInterface( LPTYPEINFO ptinfo, ZTypeLibrary * pTypeLib, int nImplFlag ) :
            ZTypeInfo( ptinfo, pTypeLib )
{
   m_pdispBaseInterface = NULL;
   m_ptinfo = NULL;
   m_nImplFlag = nImplFlag;
   if ( ptinfo )
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      QueryFunctions();
      QueryBaseInterface();
   }
}

ZInterface::~ZInterface()
{
   while ( !m_pdispFunctions.IsEmpty() )
   {
      ZFunction *o = (ZFunction *) m_pdispFunctions.RemoveHead();
      delete o;
   }

   if ( m_pdispBaseInterface )
   {
      delete m_pdispBaseInterface;
   }

   if ( m_ptinfo )
   {
      m_ptinfo->Release();
   }
}

long
ZInterface::QueryFunctions()
{
   HRESULT hr = 0;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   if ( m_pdispFunctions.IsEmpty() )
   {
      TRY
      {
         // Create collection of functions in interface.
         hr = m_ptinfo->GetTypeAttr( &ptypeattr );
         if ( !SUCCEEDED( hr ) )
         {
          AfxThrowOleException( hr );
         }

         for (n=0; n<ptypeattr->cFuncs; n++)
         {
            m_pdispFunctions.AddTail( new ZFunction(m_ptinfo, n, this));
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      CATCH(COleException, e)
      {
         if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }
         if (ptypeattr)
         {
            m_ptinfo->ReleaseTypeAttr(ptypeattr);
         }
         while ( !m_pdispFunctions.IsEmpty() )
         {
            CObject *o = m_pdispFunctions.RemoveHead();
            delete o;
         }
      }
      END_CATCH
   }

   return( m_pdispFunctions.GetCount() );
}

BOOL
ZInterface::QueryBaseInterface()
{
   HRESULT hr = NULL;
   LPTYPEINFO ptinfoInterface = NULL;
   HREFTYPE hreftype;
   BOOL bReturn = TRUE;

   if (m_pdispBaseInterface == NULL)
   {
      TRY
      {
         hr = m_ptinfo->GetRefTypeOfImplType(0, &hreftype);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         hr = m_ptinfo->GetRefTypeInfo(hreftype, &ptinfoInterface);

         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         m_pdispBaseInterface = new ZInterface(ptinfoInterface, this->GetTypeLibrary() );

         ptinfoInterface->Release();
      }
      CATCH(COleException,e)
      {
         if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }
         if (ptinfoInterface)
         {
            ptinfoInterface->Release();
         }
         m_pdispBaseInterface = NULL;
         bReturn = FALSE;
      }
      END_CATCH
   }

   return( bReturn );
}

CString
ZInterface::GetCPPDeclaration()
{
   CString strOut;
   if ( GetBaseInterface() )
   {
      // Generating a definition like
      // #define ICursor IUnknown
      strOut = "#define ";
      strOut += GetName();
      strOut += " ";
      strOut += GetBaseInterface()->GetName();
   }
   return strOut;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZDispinterface, ZTypeInfo)

ZDispinterface::ZDispinterface( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib ) :
                ZTypeInfo( ptinfo, pTypeLib )
{
   m_pInterface = NULL;
   m_ptinfo = NULL;

   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadInterface();
      LoadMethods();
      LoadProperties();
   }
}


ZDispinterface::~ZDispinterface ()
{
   if ( m_pInterface )
   {
      delete (ZInterface *)m_pInterface;
   }

   while ( !m_pdispMethods.IsEmpty() )
   {
      ZFunction *o = (ZFunction *)m_pdispMethods.RemoveHead();
      delete o;
   }

   while ( !m_pdispProperties.IsEmpty() )
   {
      ZProperty *o = (ZProperty *)m_pdispProperties.RemoveHead();
      delete o;
   }

   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }
}

long
ZDispinterface::LoadProperties()
{
   HRESULT hr;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   USHORT n = 0;

   TRY
   {
      if ( m_pdispProperties.IsEmpty() )
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         // Enumerate properties and and store it in a list.
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_pdispProperties.AddTail(new ZProperty(m_ptinfo, pvardesc, GetTypeLibrary(), this ) );
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_ptinfo->ReleaseVarDesc(pvardesc);
            pvardesc = NULL;
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e )
   {
      if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }

      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      if (pvardesc)
      {
         m_ptinfo->ReleaseVarDesc(pvardesc);
      }

      while (!m_pdispProperties.IsEmpty())
      {
         CObject *o = m_pdispProperties.RemoveHead();
         delete o;
      }
   }
   END_CATCH

   return m_pdispProperties.GetCount();
}


long
ZDispinterface::LoadMethods()
{
   HRESULT hr = NULL;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   if ( m_pdispMethods.IsEmpty() )
   {
      TRY
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException( hr );
         }
         // Enumerate methods and return a collection of these.
         for (n=0; n<ptypeattr->cFuncs; n++)
         {
            m_pdispMethods.AddTail(new ZFunction(m_ptinfo, n, this));
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      CATCH( COleException, e )
      {
         if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }
         if (ptypeattr)
         {
            m_ptinfo->ReleaseTypeAttr(ptypeattr);
         }
         while ( !m_pdispMethods.IsEmpty() )
         {
            CObject *o = m_pdispMethods.RemoveHead();
            delete o;
         }
      }
      END_CATCH
   }

   return m_pdispMethods.GetCount();
}

BOOL
ZDispinterface::LoadInterface()
{
   HRESULT hr = NULL;
   LPTYPEINFO ptinfoInterface = NULL;
   HREFTYPE hreftype;
   BOOL bReturn = TRUE;

   if ( m_pInterface == NULL )
   {
      TRY
      {
         // Get interface part of dual interface
         hr = m_ptinfo->GetRefTypeOfImplType( 0xFFFFFFFF, &hreftype );
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException( hr );
         }
         hr = m_ptinfo->GetRefTypeInfo(hreftype, &ptinfoInterface);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException( hr );
         }
//         m_pInterface = new ZInterface(ptinfoInterface);
      }
      CATCH( COleException, e )
      {
         if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }

         if ( m_pInterface )
         {
            delete m_pInterface;
            m_pInterface = NULL;
         }
         bReturn = FALSE;
      }
      END_CATCH
   }

   return bReturn;
}

void
ZDispinterface::Rename( CString strNewName )
{
   ::SysFreeString( m_bstrName );
   m_bstrName = strNewName.AllocSysString();
}



IMPLEMENT_DYNAMIC( ZModule, ZTypeInfo)

ZModule::ZModule( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib ) :
         ZTypeInfo( ptinfo, pTypeLib )
{
   if ( ptinfo )
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadFunctions();
   }
}


ZModule::~ZModule()
{
   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while ( !m_pFunctions.IsEmpty() )
   {
      CObject *o = m_pFunctions.RemoveHead();
      delete o;
   }
}

LONG ZModule::LoadFunctions()
{
   HRESULT hr;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   TRY
   {
      if ( m_pFunctions.IsEmpty() )
      {
         // Create collection of functions of the interface.
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         for (n=0; n<ptypeattr->cFuncs; n++)
         {
            m_pFunctions.AddTail(new ZFunction(m_ptinfo, n, this));
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if ( e->m_sc != TYPE_E_ELEMENTNOTFOUND )
      {
         e->ReportError();
      }

      if ( ptypeattr )
      {
         m_ptinfo->ReleaseTypeAttr( ptypeattr );
      }

      while ( !m_pFunctions.IsEmpty() )
      {
         CObject *o = m_pFunctions.RemoveHead();
         delete o;
      }

   }
   END_CATCH

   return m_pFunctions.GetCount();
}


IMPLEMENT_DYNAMIC( ZCoClass, ZTypeInfo)

ZCoClass::ZCoClass( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib, int nImplFlag ) :
          ZTypeInfo( ptinfo, pTypeLib )
{
   m_ptinfo = NULL;
   m_nImplFlag = nImplFlag;

   if ( ptinfo )
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadInterfaces();
   }
}

ZCoClass::~ZCoClass()
{
   while ( !m_Interfaces.IsEmpty() )
   {
      CObject *o = m_Interfaces.RemoveHead();
      delete o;
   }

   if ( m_ptinfo )
   {
      m_ptinfo->Release();
   }
}

LONG
ZCoClass::LoadInterfaces()
{
   HRESULT hr = NULL;
   HREFTYPE hreftype;
   LPTYPEATTR ptypeattr = NULL;
   LPTYPEINFO ptinfoInterface = NULL;
   LPTYPELIB ptlib = NULL;
   UINT nIndex = 0;
   USHORT n = 0;
   TYPEKIND typekind = TKIND_INTERFACE;
   int nImplFlag = 0;


   TRY
   {
      if ( m_Interfaces.IsEmpty() )
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }

         // Enumerate interfaces/dispinterfaces in coclass and return a collection of these.
         for (n=0; n<ptypeattr->cImplTypes; n++)
         {
            hr = m_ptinfo->GetImplTypeFlags( n, &nImplFlag );
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = m_ptinfo->GetRefTypeOfImplType(n, &hreftype);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = m_ptinfo->GetRefTypeInfo(hreftype, &ptinfoInterface);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = ptinfoInterface->GetContainingTypeLib(&ptlib, &nIndex);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = ptlib->GetTypeInfoType(nIndex, &typekind);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            ptlib->Release();
            ptlib = NULL;

            switch (typekind)
            {
            case TKIND_INTERFACE:
               {
               ZInterface *i = new ZInterface(ptinfoInterface, this->GetTypeLibrary(), m_nImplFlag);
               m_Interfaces.AddHead(i);
               break;
               }

            case TKIND_DISPATCH:
               m_Interfaces.AddHead(new ZCoClass(ptinfoInterface, this->GetTypeLibrary(), nImplFlag));
               break;
            }

         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH( COleException, e )
   {
      if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      if (ptlib)
      {
         ptlib->Release();
      }

      if (ptinfoInterface)
      {
         ptinfoInterface->Release();
      }
   }
   END_CATCH

   return m_Interfaces.GetCount();
}

BOOL
ZCoClass::IsEventSource( )
{
   if ( m_nImplFlag & IMPLTYPEFLAG_FDEFAULT &&
        m_nImplFlag & IMPLTYPEFLAG_FSOURCE )
      return TRUE;
   else
      return FALSE;
}

BOOL ZCoClass::IsDefault( )
{
   if ( IsEventSource() )
      return FALSE;
   if ( m_nImplFlag & IMPLTYPEFLAG_FDEFAULT )
      return TRUE;
   else
      return FALSE;
}

CString  ZCoClass::GetDefaultDispatchInterfaceGUID()
{
   POSITION pos = m_Interfaces.GetHeadPosition( );
   CObject *o = NULL;
   CString strGUID;

   if ( IsDefault() )
   {
      strGUID = GetGUID_AsString();
   }

   // iterate all the ZInterfaces and check whether they are
   // the default interface
   while ( pos && strGUID.IsEmpty() )
   {
      o = m_Interfaces.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(ZCoClass)))
      {
         strGUID = ((ZCoClass *)o)->GetDefaultDispatchInterfaceGUID();
      }
      m_Interfaces.GetNext( pos );
   }

   return strGUID;
}

CString ZCoClass::GetEventInterfaceGUID()
{
   POSITION pos = m_Interfaces.GetHeadPosition( );
   CObject *o = NULL;
   CString strGUID;

   if ( IsEventSource() )
   {
      strGUID = GetGUID_AsString();
   }

   // iterate all the ZCoClass and check whether they are
   // the default interface
   while ( pos && strGUID.IsEmpty() )
   {
      o = m_Interfaces.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(ZCoClass)))
      {
         strGUID = ((ZCoClass *)o)->GetEventInterfaceGUID();
      }
      m_Interfaces.GetNext( pos );
   }

   return strGUID;
}


IMPLEMENT_DYNAMIC( ZEnum, ZTypeInfo )

ZEnum::ZEnum( LPTYPEINFO ptinfo, ZTypeLibrary * pTypeLib  ) :
       ZTypeInfo( ptinfo, pTypeLib )
{
   if ( ptinfo )
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadEnumElements();
   }
}

ZEnum::~ZEnum()
{
   if ( m_ptinfo )
   {
      m_ptinfo->Release();
   }

   while ( !m_pElements.IsEmpty() )
   {
      CObject *o = m_pElements.RemoveHead();
      delete o;
   }
}

LONG
ZEnum::LoadEnumElements()
{
   HRESULT hr = NULL;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   TRY
   {
      if (m_pElements.IsEmpty())
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         // Enumerate enum constants and return a collection of these.
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_pElements.AddTail(new ZConstant(m_ptinfo, GetTypeLibrary(), pvardesc));
            m_ptinfo->ReleaseVarDesc(pvardesc);
            pvardesc = NULL;
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      if (pvardesc)
      {
         m_ptinfo->ReleaseVarDesc(pvardesc);
      }
   }
   END_CATCH

   return m_pElements.GetCount();
}


CString ZEnum::GetCPPDeclaration(BOOL bTagged)
{
   POSITION pos = m_pElements.GetHeadPosition();
   LONG i = m_pElements.GetCount();
   LONG n = 0;
   ZConstant *c = NULL;
   COleVariant vValue;
   VARIANT  v;
   CString strName = GetName();

   CString strOut = "typedef enum ";
   if ( bTagged )
   {
      strOut += "tag";
      strOut += strName;
   }
   strOut+=" {";
   strOut+=CRLF;
   strOut+=TAB_SPACE;

   while (pos)
   {
      c = (ZConstant*) m_pElements.GetAt(pos);
      // generate unique element names by extending
      // enum list element names with enum name separated by an underscore
      strOut += strName;
      strOut += "_";
      strOut += c->GetName();
      strOut += " = ";
      vValue = c->GetValue();

      vValue.ChangeType(VT_BSTR);
      v = vValue.Detach();
      strOut += v.bstrVal;

      if ( n < i-1 )
      {
         strOut+= ",";
         strOut+=CRLF;
         strOut+=TAB_SPACE;
      }
      else
      {
         strOut+=CRLF;
      }
      m_pElements.GetNext(pos);
      n++;
   }

   strOut += "} ";
   strOut += strName;
   strOut += ";";

   return strOut;
}



IMPLEMENT_DYNAMIC( ZAlias, ZTypeInfo)

ZAlias::ZAlias (LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib )
            :ZTypeInfo( ptinfo, pTypeLib )
{
   HRESULT hr;
   LPTYPEATTR ptypeattr = NULL;

   // Get base type of this alias.
   hr = ptinfo->GetTypeAttr(&ptypeattr);
   if (SUCCEEDED( hr ))
   {
      m_pTypeDescBase = new ZTypeDesc(ptinfo, pTypeLib, &ptypeattr->tdescAlias);
   }

   ptinfo->ReleaseTypeAttr(ptypeattr);
}



ZAlias::~ZAlias ()
{
   if ( m_pTypeDescBase != NULL )
   {
      delete m_pTypeDescBase;
   }
}



IMPLEMENT_DYNAMIC( ZStruct, ZTypeInfo)

ZStruct::ZStruct(LPTYPEINFO ptinfo, ZTypeLibrary * pTypeLib)
            :ZTypeInfo(ptinfo, pTypeLib )
{
   m_ptinfo = NULL;
   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadElements();
   }
}



ZStruct::~ZStruct()
{
   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while (!m_Elements.IsEmpty())
   {
      CObject *o = m_Elements.RemoveHead();
      delete o;
   }
}


LONG ZStruct::LoadElements()
{
   HRESULT hr = NULL;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   USHORT n = 0;

   TRY
   {
      if ( m_Elements.IsEmpty() )
      {
         // Create a list of structure members
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_Elements.AddTail(new ZProperty(m_ptinfo, pvardesc, GetTypeLibrary() ) );
            m_ptinfo->ReleaseVarDesc(pvardesc);
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      while (!m_Elements.IsEmpty())
      {
         CObject *o = m_Elements.RemoveHead();
         delete o;
      }
   }
   END_CATCH

   return m_Elements.GetCount();
}


CString ZStruct::GetCPPDefinition(BOOL bTagged)
{
   CString strOut;
   POSITION pos = m_Elements.GetHeadPosition();
   LONG i = m_Elements.GetCount();
   LONG n = 0;
   ZProperty *c = NULL;

   strOut = "struct ";
   if ( bTagged )
   {
      strOut += "tag";
   }

   strOut += GetName();

   strOut+=CRLF;
   strOut +="{";
   strOut+=CRLF;
   strOut+=TAB_SPACE;

   // step through all elements and look for type and name
   while (pos)
   {
      c = (ZProperty*) m_Elements.GetAt(pos);
      strOut += c->GetCPPDefinition();
      if ( n < i-1 )
      {
         strOut += (",");
         strOut+=CRLF;
         strOut+=TAB_SPACE;
      }
      else
      {
         strOut+=CRLF;
      }
      m_Elements.GetNext(pos);
      n++;
   }

   strOut += "}";
   strOut += GetName();
   strOut += ";";

   return strOut;
}




IMPLEMENT_DYNAMIC( ZUnion, ZTypeInfo)

ZUnion::ZUnion(LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib)
            :ZTypeInfo(ptinfo, pTypeLib )
{
   m_ptinfo = NULL;
   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadElements();
   }
}


ZUnion::~ZUnion()
{
   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while (!m_Elements.IsEmpty())
   {
      CObject *o = m_Elements.RemoveHead();
      delete o;
   }
}


LONG ZUnion::LoadElements()
{
   HRESULT hr = NULL;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   USHORT n = 0;

   TRY
   {
      if ( m_Elements.IsEmpty() )
      {
         // Create a list of structure members
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_Elements.AddTail(new ZProperty(m_ptinfo, pvardesc, this->GetTypeLibrary() ) );
            m_ptinfo->ReleaseVarDesc(pvardesc);
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      while (!m_Elements.IsEmpty())
      {
         CObject *o = m_Elements.RemoveHead();
         delete o;
      }
   }
   END_CATCH

   return m_Elements.GetCount();
}


CString ZUnion::GetCPPDefinition(BOOL bTagged)
{
   CString strOut;
   POSITION pos = m_Elements.GetHeadPosition();
   LONG i = m_Elements.GetCount();
   LONG n = 0;
   ZProperty *c = NULL;

   strOut = "struct ";
   if ( bTagged )
   {
      strOut += "tag";
   }

   strOut += GetName();

   strOut+=CRLF;
   strOut +="{";
   strOut+=CRLF;
   strOut+=TAB_SPACE;

   // step through all elements and look for type and name
   while (pos)
   {
      c = (ZProperty*) m_Elements.GetAt(pos);
      strOut += c->GetCPPDefinition();
      if ( n < i-1 )
      {
         strOut += (",");
         strOut+=CRLF;
         strOut+=TAB_SPACE;
      }
      else
      {
         strOut+=CRLF;
      }
      m_Elements.GetNext(pos);
      n++;
   }

   strOut += "}";
   strOut += GetName();
   strOut += ";";

   return strOut;
}



IMPLEMENT_DYNAMIC(ZFunction, CObject)

ZFunction::ZFunction( LPTYPEINFO ptinfo,
                      USHORT nIndex,
                      ZInterface *pInterface )
{
   m_pTypeLib = pInterface->GetTypeLibrary();
   m_pParentInterface = pInterface;
   Initialize( ptinfo, nIndex );
}

ZFunction::ZFunction( LPTYPEINFO ptinfo,
                      USHORT nIndex,
                      ZDispinterface *pInterface )
{
   m_pTypeLib = pInterface->GetTypeLibrary();
   m_pParentInterface = pInterface;
   Initialize( ptinfo, nIndex );
}

ZFunction::ZFunction( LPTYPEINFO ptinfo,
                      USHORT nIndex,
                      ZModule *pModule )
{
   m_pTypeLib = pModule->GetTypeLibrary();
   m_pParentInterface = pModule;
   Initialize( ptinfo, nIndex );
}

BOOL
ZFunction::Initialize( LPTYPEINFO ptinfo, USHORT nIndex )
{
   HRESULT hr = NULL;
   BOOL bReturn = TRUE;

   TRY
   {
      hr = ptinfo->GetFuncDesc(nIndex, &m_pfuncdesc);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }
      // Function return type.
      m_pdispTypeDesc = new ZTypeDesc(ptinfo, GetTypeLibrary(), &m_pfuncdesc->elemdescFunc.tdesc);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }

      hr = ptinfo->GetDocumentation(m_pfuncdesc->memid, &m_bstrName, &m_bstrDocumentation,
               &m_ulHelpContext, &m_bstrHelpFile);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }
      m_ptinfoFunction = ptinfo;
      m_ptinfoFunction->AddRef();
      LoadParameter();

   }
   CATCH( COleException, e)
   {
      if (e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (m_pfuncdesc)
      {
         ptinfo->ReleaseFuncDesc(m_pfuncdesc);
      }

      if (m_ptinfoFunction)
      {
         m_ptinfoFunction->Release();
      }

      // Set to NULL to prevent destructor from attempting to free again
      m_ptinfoFunction = NULL;
      m_pfuncdesc = NULL;
      bReturn = FALSE;
   }
   END_CATCH

   return( bReturn );
}

LONG
ZFunction::LoadParameter()
{
   HRESULT hr = NULL;
   BSTR rgbstrNames[DISPATCH_MAX_PARAM];
   UINT cNames;
   USHORT n = 0;
   TYPEDESC  *pTypedesc;
   LPIDLDESC  pIDLdesc;

   for ( UINT ui = 0; ui < DISPATCH_MAX_PARAM; ui++)
      rgbstrNames[ ui ] = NULL;

   TRY
   {  // Create a list of parameters.
      if (m_pdispParameters.IsEmpty())
      {
         // Get the name of the function and its parameters
         hr = m_ptinfoFunction->GetNames(m_pfuncdesc->memid, rgbstrNames,
                           DISPATCH_MAX_PARAM, &cNames);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         SysFreeString(rgbstrNames[0]);

         // a "property put" has no name, so name int "Value"
         if (m_pfuncdesc->invkind & INVOKE_PROPERTYPUT)
            rgbstrNames[n+1] = SysAllocString(OLESTR("Value"));
         // Add parameters to the list of parameter
         for (n=0; n<m_pfuncdesc->cParams; n++)
         {
            pTypedesc = &m_pfuncdesc->lprgelemdescParam[n].tdesc;
            pIDLdesc  = &m_pfuncdesc->lprgelemdescParam[n].idldesc;
            m_pdispParameters.AddTail( new ZParameter(m_ptinfoFunction, GetTypeLibrary(),
                                               rgbstrNames[n+1],
                                               pTypedesc,
                                               pIDLdesc,
                                               n));

            SysFreeString(rgbstrNames[n+1]);
         }
      }
   }
   CATCH( COleException, e )
   {
      if (rgbstrNames)
      {  // free only those that have not already been free'd. continue with current n;
         for (; n<m_pfuncdesc->cParams; n++)
         {
            SysFreeString(rgbstrNames[n+1]);
         }
      }
   }
   END_CATCH
   return m_pdispParameters.GetCount();

}

ZFunction::~ZFunction()
{
   while ( !m_pdispParameters.IsEmpty() )
   {
      CObject *p = m_pdispParameters.RemoveHead();
      delete p;
   }

   if (m_pfuncdesc && m_ptinfoFunction)
   {
      m_ptinfoFunction->ReleaseFuncDesc(m_pfuncdesc);
   }

   if (m_ptinfoFunction)
   {
      m_ptinfoFunction->Release();
   }

   if (m_pdispTypeDesc)
   {
      delete m_pdispTypeDesc;
   }
}



CString
ZFunction::GetCPPDeclaration( zBOOL bHeader )
{
   CString strOut, strHelp;
   CObList *lpParameter = GetParameter();
   POSITION pos = NULL;
   INT i,j;

   strOut.Empty();
   i = j = 0;

   // there is nothig to do if one of those conditions are true
   if (m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }

   // write returntype
   strHelp = m_pdispTypeDesc->GetCPPDefinition();

   if ( m_pdispTypeDesc->IsDispatchPointer() )
   {
      int k = strHelp.ReverseFind( '*' );
      if ( k >= 0 )
         strHelp = strHelp.Mid( 0, k );
   }

   strOut += strHelp;
   strOut += TAB_SPACE;

   // write classname followed by ::
   if ( bHeader == FALSE )
   {
      strOut += GetClassName();
      strOut += "::";
   }

   // write methodname
   strOut += GetPropertyMethodPrefix() + GetName();

   strOut += "( ";

   // write parameter if there is any
   if (lpParameter != NULL )
   {
      pos = lpParameter->GetHeadPosition();
      i   = lpParameter->GetCount();
   }
   while ( pos )
   {
      ZParameter *p =(ZParameter *)lpParameter->GetAt(pos);
      strHelp = p->GetType()->GetCPPDefinition();
      strHelp += " ";

      CString strName = p->GetName();

      // if parameter has no name generate one that look like "param0"
      if ( strName.GetLength() < 1 )
      {
         strName.Format( lpszPARAMTEMPLATE, j );
      }

      // If current parameter is optional
      // add const and address operator
      if ( IsOptionalParameter(j) )
      {
         strName = "& " + strName;
         strHelp = "const " + strHelp;
      }

      strOut = strOut + strHelp + strName;
      if (j < i-1)
      {
         strOut += ", ";
      }
      j++;
      lpParameter->GetNext(pos);
   }
   strOut += " )";

   if ( bHeader )
   {
      strOut+=";";
   }

   return strOut;

}


CString ZFunction::GetMethodCall()
{
   CString strOut, strHelp;
   CObList *lpParameter = GetParameter();
   POSITION pos = NULL;
   INT i,j;

   strOut.Empty();
   i = j = 0;

   // there is nothig to do if one of those conditions are true
   if (m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }

   // write methodname
   strOut += GetPropertyMethodPrefix() + GetName();

   strOut += "( ";

   // write parameter if there is any
   if (lpParameter != NULL )
   {
      pos = lpParameter->GetHeadPosition();
      i   = lpParameter->GetCount();
   }
   while ( pos )
   {
      ZParameter *p =(ZParameter *)lpParameter->GetAt(pos);
      CString strName = p->GetName();

      // if parameter has no name generate one that look like "param0"
      if ( strName.GetLength() < 1 )
         strName.Format(lpszPARAMTEMPLATE, j);

      strOut = strOut + strName;
      if (j < i-1)
         strOut += ", ";

      j++;
      lpParameter->GetNext(pos);
   }
   strOut += " );";

   return strOut;
}


CString ZFunction::GetCPPDefinition()
{
   CString strOut;            //return string
   CString strHelp;           //just for help
   CString strParamNames;     //lists all parameters by name, separated by comma
   CString strParamTypes;     //lists all parameter types separated by blank
   CString strReturnType;     //Holds type of return
   CString strReturnName;     //Name of the Variable the holds the return value
   CString strMethodType;     //used to convert information about method type into string
   CString strDISPID;         //String that holds the dispatch ID of the current function
   CString strParmList;
   POSITION pos = NULL;       //list iterater
   CObList *pParams = NULL;   //points to the list of parameters
   BOOL bIsVoid = FALSE;

   strOut.Empty();
   strParamNames.Empty();

   // there is nothig to do if one of those conditions are true
   if (m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }


   // 1. write function declaration like this
   //    CString _DATX32::GetFooterFText(short whatFooter)
   strOut += GetCPPDeclaration(FALSE);
   strOut += CRLF;
   strOut += "{";
   strOut += CRLF;

   // 2. delcare a variable for return if it's not void
   strReturnType = m_pdispTypeDesc->GetCPPDefinition();
   if ( strReturnType != "void" )
   {
      strOut += TAB_SPACE;

      // if this is a dispatch pointer return class reference
      if ( m_pdispTypeDesc->IsDispatchPointer() )
      {
         int k = strReturnType.ReverseFind( '*' );
         if ( k >= 0 )
            strReturnType = strReturnType.Mid( 0, k );

         strOut += "LPDISPATCH result;";
         strOut += CRLF;
         strReturnName = "(void*)&result";
         strReturnType = "VT_DISPATCH";
      }
      else
      {
         strOut += strReturnType + " result;";
         strOut += CRLF;
         strReturnName = "(void*)&result";
         strReturnType = VTtoDispatchType( m_pdispTypeDesc->GetVartype() );
      }
   }
   else
   {
      strReturnType = "VT_EMPTY";
      strReturnName = "NULL";
      bIsVoid = TRUE;
   }

   // 3.declare the array of parametertypes we are going to pass ( if there are any )
   //   static BYTE parms[] = VTS_I2;
   //   and list the name of the parameters we use to call InvokeHelper
   pParams = GetParameter();
   int i = pParams->GetCount();
   int j = 0;

   strParamTypes.Empty();

   if ( i > 0 )
   {
      strParamTypes += TAB_SPACE;
      strParamTypes += "static BYTE parms[] =";
      strParamTypes += CRLF;
      strParamTypes += TAB_SPACE;
      strParamTypes += TAB_SPACE;

      strParmList = "parms";

      pos = pParams->GetHeadPosition();
      while ( pos )
      {
         ZParameter *p = (ZParameter *)pParams->GetAt( pos );
         strParamTypes += p->GetVariantType();
         strHelp = p->GetName();

         // if parameter has no name generate one that look like "param0"
         if ( strHelp.GetLength() < 1 )
         {
            strHelp.Format(lpszPARAMTEMPLATE, j);
         }

         // If current parameter is optional
         // add address operator
         if ( IsOptionalParameter(j) )
         {
            strHelp = "&" + strHelp;
         }

         strParamNames += strHelp;
         if ( j < i-1 )
         {
            strParamTypes += " ";          // each type is separated by a blank
            strParamNames += ", ";  // each parameter by comma
         }
         pParams->GetNext(pos);
         j++;
      }
      strParamTypes += ";";
   }
   else
   {
      strParamNames.Empty(); //Nothing, no code, not even a NULL-Value completely nothing
      strParmList = "NULL";
      strParamTypes.Empty();
   }

   strOut += strParamTypes;

   // 4. setup call InvokeHelper
   strDISPID.Empty();
   strDISPID = PredefinedDispID( GetMemberID() );
   if ( strDISPID.IsEmpty() )
   {
      strDISPID.Format("0x%x", GetMemberID());
   }

   strMethodType = GetInvokeKindString();

   strOut += CRLF;
   strOut += TAB_SPACE;

   // setup the  string so that each parameter is separated by comma
   strMethodType = ", " + strMethodType;
   strReturnType = ", " + strReturnType;
   strReturnName = ", " + strReturnName;
   strParmList   = ", " + strParmList;

   // if there are no paramaternames the InvokeHelper function
   // has only 5 parameters so we need no comma between the list
   // of parameter names and the list name (parms) of parameter types
   if ( !strParamNames.IsEmpty() )
   {
      strParamNames = ",\r\n      " + strParamNames;
   }

   strHelp.Format( lpszINVOKE_HELPER, strDISPID,
                   strMethodType, strReturnType,
                   strReturnName, strParmList, strParamNames );

   strOut += strHelp;
   // Is function type of void?
   if ( bIsVoid == FALSE )
   {
      strOut += CRLF;
      strOut += TAB_SPACE;

      // if this is a dispatch pointer return class reference
      if ( m_pdispTypeDesc->IsDispatchPointer() )
      {
         strHelp = m_pdispTypeDesc->GetCPPDefinition();
         int k = strHelp.ReverseFind( '*' );
         if ( k >= 0 )
            strHelp = strHelp.Mid( 0, k );
         strHelp = "return " + strHelp;
         strHelp += " ( result );";
         strOut += strHelp;
      }
      else
      {
         strOut += "return result;";
      }
   }

   strOut += CRLF;
   strOut += "}";
   return strOut;
}

CString ZFunction::GetInvokeKindString()
{
   CString strOut;

   switch ( GetInvokeKind() )
   {
      case INVOKE_FUNC :
         strOut = _T("DISPATCH_METHOD");
         break;
      case INVOKE_PROPERTYGET :
         strOut = _T("DISPATCH_PROPERTYGET");
         break;
      case INVOKE_PROPERTYPUT :
         strOut = _T("DISPATCH_PROPERTYPUT");
         break;
      case INVOKE_PROPERTYPUTREF :
         strOut = _T("DISPATCH_PROPERTYPUTREF");
         break;
      default: // should not occur because it is enumeration but it is still Microsoft
         strOut.Format(_T("Unknown invoke type:   %d"), GetInvokeKind());
         break;
   }
   return strOut;
}


//
// returns get or set if it is a method
// for setting or getting a property
//
CString ZFunction::GetPropertyMethodPrefix()
{
   CString strOut;

   switch ( GetInvokeKind() )
   {
      case INVOKE_PROPERTYGET :
         strOut = _T("Get");
         break;
      case INVOKE_PROPERTYPUT :
         strOut = _T("Set");
         break;
      case INVOKE_PROPERTYPUTREF :
         strOut = _T("SetRef");
         break;
      default:
         strOut = _T("");
         break;
   }
   return strOut;
}


CString ZFunction::GetClassName()
{
   CString strOut;
   ZInterface *pInterface = NULL;
   ZDispinterface *pDispinterface = NULL;
   ZModule *pModule = NULL;

   CObject *o = GetParentInterface();

   if ( o == NULL )
   {
      strOut.Empty();
   }
   else
   {  // savely cast to the specific object
      if ( o->IsKindOf(RUNTIME_CLASS( ZInterface ) ) )
      {
         pInterface = (ZInterface *) o;
         strOut = pInterface->GetName();
      }
      else
      if ( o->IsKindOf(RUNTIME_CLASS( ZDispinterface ) ) )
      {
         pDispinterface = (ZDispinterface *) o;
         strOut = pDispinterface->GetName();
      }
      else
      if ( o->IsKindOf(RUNTIME_CLASS( ZModule ) ) )
      {
         pModule = (ZModule *) o;
         strOut = pModule->GetName();
      }

   }

   return strOut;

}


IMPLEMENT_DYNAMIC(ZTypeDesc, CObject)

ZTypeDesc::ZTypeDesc(LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib, TYPEDESC FAR* ptypedesc)
{
   HRESULT hr = NULL;
   LPTYPEINFO ptinfoUserDefined;

   m_pDescription = NULL;
   m_vartype = ptypedesc->vt;
   m_pTypeLib = pTypeLib;
   switch ( m_vartype )
   {
      case VT_USERDEFINED :
      {
         hr = ptinfo->GetRefTypeInfo( ptypedesc->hreftype, &ptinfoUserDefined );
         if ( SUCCEEDED( hr ) )
            m_pDescription = new ZTypeInfo( ptinfoUserDefined, m_pTypeLib );

         break;
      }

      case VT_CARRAY :
      {
         m_pDescription = new ZArrayDesc( ptinfo, m_pTypeLib, ptypedesc->lpadesc );
         break;
      }

      case VT_PTR :
      {
         m_pDescription = new ZTypeDesc( ptinfo, m_pTypeLib, ptypedesc->lptdesc );
         break;
      }

      default:
         break;
   }
}

ZTypeDesc::~ZTypeDesc()
{
   mDeleteInit( m_pDescription );
}

CString
ZTypeDesc::GetCPPDefinition()
{
   CString str;

   if ( (GetVartype() & 0x0FFF) == VT_SAFEARRAY )
   {
      str = "SAFEARRAY( ";
   }

   if ( GetVartype() == VT_PTR )
   {
      // m_pDescribtion points to a ZTypeDesc that specifies the thing pointed to
      ZTypeDesc *o = (ZTypeDesc *)m_pDescription;

      str += o->GetCPPDefinition();
      str += "*";
   }
   else
   if ( (GetVartype() & 0x0FFF) == VT_CARRAY )
   {
      // m_pDescribtion  points to an ZArrayDesc
      ZArrayDesc *o = (ZArrayDesc *) m_pDescription;
      str = o->GetCPPDefinition();
   }
   else
   if ( GetVartype() == VT_USERDEFINED )
   {
      ZTypeInfo *o = (ZTypeInfo *) m_pDescription;
      str = o->GetName();
   }
   else
      str = VTtoCType( GetVartype() );

   if ( (GetVartype() & 0x0FFF) == VT_SAFEARRAY )
   {
      str += " )";
   }

   return str;
}

CString
ZTypeDesc::GetVariantType()
{
   CString str;

   if ( GetVartype() == VT_PTR )
   {
      // m_pDescribtion points to a ZTypeDesc that specifies the type pointed to
      ZTypeDesc *o = (ZTypeDesc *) m_pDescription;
      str += "VTS_P";
      str += VTtoVariantType( o->GetVartype() );
   }
   else
   {
      str += "VTS_";
      str += VTtoVariantType( GetVartype() );
   }

   return( str );
}

BOOL
ZTypeDesc::IsDispatchPointer()
{
   BOOL bReturn = FALSE;

   if ( GetVartype() == VT_PTR )
   {
      // m_pDescribtion points to a ZTypeDesc that specifies the type pointed to
      ZTypeDesc *o = (ZTypeDesc *) m_pDescription;
      if ( o->GetVartype() == VT_USERDEFINED )
      {
         bReturn = TRUE;
      }
   }

   return( bReturn );
}


//
// retruns the parametersize used for generation def-files
// Functionname = _FunctionName@xxx @y
// where xxx is a computed value depending on the number and type of
// of parameter passed by the caller and y is the ordinal of the function
//
USHORT ZTypeDesc::GetStackSize()
{
   if ( GetVartype() == VT_VARIANT ) // as far as I know the only exception
      return 16;
   else
      return  4;
}



IMPLEMENT_DYNAMIC(ZArrayBound, CObject)

ZArrayBound::ZArrayBound( ULONG cElements, LONG lBounds )
{
   m_ulElementCount  = cElements;
   m_lLowerBound     = lBounds;
}

ZArrayBound::~ZArrayBound()
{
}


IMPLEMENT_DYNAMIC(ZArrayDesc, CObject)

ZArrayDesc::ZArrayDesc( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib,
                        ARRAYDESC FAR *parraydesc )
{
    INT n = 0;
    m_pTypeDesc = NULL;

    if ( ptinfo == NULL )
       return;

    m_pTypeDesc = new ZTypeDesc( ptinfo, pTypeLib, &parraydesc->tdescElem );

    // Create a list of array bounds. One array bound per dimension.
    for ( n = 0; n < parraydesc->cDims; n++ )
    {
        m_ArrayBounds.AddTail( new ZArrayBound( parraydesc->rgbounds[ n ].cElements,
                                                parraydesc->rgbounds[ n ].lLbound ) );
    }
}

ZArrayDesc::~ZArrayDesc()
{
   mDeleteInit( m_pTypeDesc );
   while ( !m_ArrayBounds.IsEmpty() )
   {
      CObject *o = m_ArrayBounds.RemoveHead();
      delete o;
   }
}

CString ZArrayDesc::GetCPPDefinition()
{
   CString str;
   POSITION pos = m_ArrayBounds.GetHeadPosition();
   UINT n = 0;

   // Allocate Dimensions [ ][ ]....
   CString strTemp;

   while ( pos )
   {
      ZArrayBound * a = (ZArrayBound *)m_ArrayBounds.GetAt(pos);
      strTemp.Format( _T("[%d]"), a->GetElementCount());
      str += strTemp;
      n++;
      m_ArrayBounds.GetNext(pos);
   }

   return str;
}



IMPLEMENT_DYNAMIC(ZConstant, CObject)

ZConstant::ZConstant(LPTYPEINFO ptinfo, ZTypeLibrary* pTypeLib, LPVARDESC pvardesc)
{
   HRESULT hr;
   unsigned int cNames;

   m_memid = pvardesc->memid;
   ptinfo->GetNames(pvardesc->memid, &m_bstrName, 1, &cNames);

   // Type of constant.
   m_pTypeDesc = new ZTypeDesc(ptinfo, pTypeLib, &pvardesc->elemdescVar.tdesc);

   // Constant value.
   if (pvardesc->varkind == VAR_CONST)
   {
      m_vValue = *pvardesc->lpvarValue;

      hr = ptinfo->GetDocumentation(pvardesc->memid, NULL, &m_bstrDocumentation,
          &m_ulHelpContext, &m_bstrHelpFile);
      if (!SUCCEEDED(hr))
      {
         if (m_bstrName)
         {
            SysFreeString(m_bstrName);
         }

         if (m_bstrDocumentation)
         {
            SysFreeString(m_bstrDocumentation);
         }

         if (m_bstrHelpFile)
         {
            SysFreeString(m_bstrHelpFile);
         }

         // Set to NULL to prevent destructor from attempting to free again
         m_bstrName = NULL;
         m_bstrDocumentation = NULL;
         m_bstrHelpFile = NULL;
      }
   }
}

ZConstant::~ZConstant()
{
   if ( m_pTypeDesc )
   {
      delete m_pTypeDesc;
   }

}



IMPLEMENT_DYNAMIC(ZProperty, ZTlbBaseProperties)

ZProperty::ZProperty(LPTYPEINFO ptinfo, LPVARDESC pvardesc,
                     ZTypeLibrary * pTypeLib, ZDispinterface *pDispinterface )
{
   HRESULT hr = NULL;
   unsigned int cNames;
   m_pdispTypeDesc = NULL;
   m_pDispinterface = pDispinterface;
   m_pTypeLib = pTypeLib;

   m_memid = pvardesc->memid;
   ptinfo->GetNames(m_memid, &m_bstrName, 1, &cNames);

   // Property type
   m_pdispTypeDesc = new ZTypeDesc(ptinfo, m_pTypeLib, &pvardesc->elemdescVar.tdesc);

   hr = ptinfo->GetDocumentation(pvardesc->memid, NULL, &m_bstrDocumentation,
          &m_ulHelpContext, &m_bstrHelpFile);

   if (!SUCCEEDED(hr))
   {
      if (m_bstrName)
      {
         SysFreeString(m_bstrName);
      }

      if (m_bstrDocumentation)
      {
         SysFreeString(m_bstrDocumentation);
      }

      if (m_bstrHelpFile)
      {
         SysFreeString(m_bstrHelpFile);
      }
      // Set to NULL to prevent destructor from attempting to free again
      m_bstrName = NULL;
      m_bstrDocumentation = NULL;
      m_bstrHelpFile = NULL;
   }

}


ZProperty::~ZProperty()
{
   if ( m_pdispTypeDesc != NULL )
   {
      delete m_pdispTypeDesc;
   }

}


CString ZProperty::GetCPPDefinition()
{
   CString strOut;
   CString strGet;
   CString strSet;


   // 1. setup definition for GetMethod
   strGet.Format(lpszGET_PROP_CPP,GetType()->GetCPPDefinition(),
                                  GetClassName(),
                                  GetName(),
                                  GetType()->GetCPPDefinition(),
                                  GetMemberID(),
                                  VTtoDispatchType( GetType()->GetVartype() ));

   // 2. setup definition for SetMethod
   strSet.Format(lpszSET_PROP_CPP,GetClassName(),
                                  GetName(),
                                  GetType()->GetCPPDefinition(),
                                  GetMemberID(),
                                  VTtoDispatchType( GetType()->GetVartype() ));

   strOut = CRLF + strGet + CRLF + strSet + CRLF;

   return strOut;
}


CString ZProperty::GetCPPDeclaration()
{
   CString strOut;
   CString strType;
   CString strName;

   strType.Empty();
   strOut.Empty();
   ZTypeDesc  *t = GetType ();

   if ( t != NULL )
   {
      // this should be the return type
      strType = t->GetCPPDefinition();

      // First the Get... method
      strOut += TAB_SPACE;
      strOut += strType;
      strOut += TAB_SPACE;

      strOut += "Get";
      strName += GetName();
      strOut += strName;
      strOut += "();";
      strOut += CRLF;

      // now the Set... method
      strOut += TAB_SPACE;
      strOut += "void";  // Setting any property is type of void
      strOut += TAB_SPACE;

      strOut += "Set";
      strOut += GetName();
      strOut += "( ";
      strOut += strType;
      strOut = strOut + " " + "propVal";
      strOut += " );";

      strOut += CRLF;
   }

   return strOut;
}

CString ZProperty::GetClassName()
{
   CString strOut;

   strOut.Empty();

   if ( m_pDispinterface )
   {
      strOut = m_pDispinterface->GetName();
   }
   return strOut;
}


IMPLEMENT_DYNAMIC(ZParameter, ZTlbBaseProperties)

ZParameter::ZParameter(LPTYPEINFO ptinfo, ZTypeLibrary * pTypeLib, BSTR bstrName,
                       TYPEDESC FAR* ptypedesc, IDLDESC FAR* pidldesc, MEMBERID memid)
{
   m_pdispTypeDesc = NULL;

   m_bstrName = SysAllocString(bstrName);
   m_pTypeLib = pTypeLib;

   // Parameter type.
   m_pdispTypeDesc = new ZTypeDesc(ptinfo, m_pTypeLib, ptypedesc);
   m_usFlags = pidldesc->wIDLFlags;
   m_memid = memid; // Typelibrary does not contain a member id
}


ZParameter::~ZParameter()
{
   if ( m_pdispTypeDesc )
   {
      delete m_pdispTypeDesc;
   }
}


CString ZParameter::GetVariantType()
{
   CString strOut;
   if ( m_pdispTypeDesc )
   {
      strOut =  m_pdispTypeDesc->GetVariantType();
   }
   else
   {
      strOut.Empty();
   }

   return strOut;
}

zBOOL
ZTypeLibrary::GenerateCode( zCPCHAR cpcFileName,
                            zCPCHAR cpcScriptFunctionName,
                            zCPCHAR cpcNewDefaultName,
                            zCPCHAR cpcGUID,
                            zBOOL   bAppend )
{
   zBOOL bReturn = FALSE;
   CString strHelp = cpcFileName;
   //m_csNewDefaultClassName = cpcNewDefaultName;
   ZDispinterface *pI = GetDefaultDispatch( cpcGUID );

   m_bAppend = bAppend;
   m_csGUID = cpcGUID;

   int j = 0;

   if ( strHelp.IsEmpty() )
   {
      strHelp = m_csLibraryFileName;
   }

   // extract name without extension
   j = strHelp.ReverseFind( '.' );
   if ( j >= 0 )
      strHelp = strHelp.Mid( 0, j );

   // create Header with class definitions
   bReturn = CreateHeader( strHelp + ".h" );

   // now create C for dll generation
   // first C code so that the code that is most relevant for the user
   // is at the top of the file
   if ( bReturn )
   {
      bReturn = CreateC( strHelp + ".cpp", cpcScriptFunctionName);
   }

   // now create the C++ class implementation
   if ( bReturn )
   {
      bReturn = CreateCPP( strHelp + ".cpp");
   }

   if ( bReturn )
   {
      bReturn = CreateDEF(strHelp + ".def", cpcScriptFunctionName);
   }

   return bReturn;
}


BOOL ZTypeLibrary::CreateCPP( zCPCHAR cpcFileName )
{
   BOOL bReturn = TRUE;
   POSITION pos = NULL;
   ZDispinterface *pDispinterface = NULL;
   CString        strHelp;
   UINT           nOpenFlags      = 0;
   CString        strGUID;

   CFile *f = NULL;


   TRY
   {
      // C code generation is already done so open file for writing
      // and seek to end of file
      nOpenFlags = CFile::modeReadWrite;
      f = new CFile(cpcFileName, nOpenFlags );
      f->SeekToEnd();

      m_csCPPFileName = cpcFileName;

      strHelp += CRLF;
      f->Write(strHelp,strHelp.GetLength());

      // setup the list of neccesary dispatch interfaces
      // and store them in m_lDependendInterfaces
      if ( m_lDependendInterfaces.IsEmpty() )
      {
         CheckDependencies();
      }

      // loop for writing code for neccesary dispatch interfaces
      pos = m_lDependendInterfaces.GetHeadPosition();
      while ( pos )
      {
         pDispinterface = (ZDispinterface *) m_lDependendInterfaces.GetAt( pos );
         strHelp.Empty();
         // no code if no interface
         if ( pDispinterface )
         {
            strHelp += pDispinterface->GetCPPDefinition();
            strHelp += CRLF;
            f->Write(strHelp,strHelp.GetLength());
         }

         m_lDependendInterfaces.GetNext( pos );
      }

      strHelp = CRLF;

      f->Write(strHelp,strHelp.GetLength());
      f->Close();
      delete f;
   }
   CATCH( CFileException, e )
   {
      e->ReportError();
      bReturn = FALSE;
      if ( f )
         delete f;
   }
   END_CATCH
   return bReturn;
}

BOOL ZTypeLibrary::CreateHeader( zCPCHAR cpcFileName )
{

   BOOL bReturn = TRUE;
   POSITION pos = NULL;
   ZDispinterface *pDispinterface   = NULL;
   ZDispinterface *pEventInterface  = NULL;
   ZEnum          *pEnum            = NULL;
   ZStruct        *pStruct          = NULL;
   ZUnion         *pUnion           = NULL;
   CString        strHelp;
   CString        strGUID;
   UINT           nOpenFlags = 0;

   CFile *f = NULL;


   pEventInterface = GetEventInterface( m_csGUID );
//   strHelp = pEventInterface->GetCPPDeclaration();

   TRY
   {
      if ( m_bAppend )
         nOpenFlags = CFile::modeCreate|
                       CFile::modeNoTruncate|
                       CFile::modeReadWrite;
      else
         nOpenFlags = CFile::modeCreate|CFile::modeReadWrite;


      f = new CFile(cpcFileName, nOpenFlags );
      f->SeekToEnd();

      m_csHeaderFileName = cpcFileName;

      WriteFileHeader(f);

      // in append mode all standard setting should be already done
      // so additional includes are obsolete
      if ( !m_bAppend )
      {
         strHelp = lpszDEFAULT_MFC_INCLUDE;
         f->Write(strHelp,strHelp.GetLength());

         strHelp = lpszZeidonDefinitions;
         f->Write(strHelp,strHelp.GetLength());

         strHelp = lpszMFC_EXTENSION;
         f->Write(strHelp,strHelp.GetLength());
      }

      // 1.
      // loop for writing defines
      pos = m_pdispTypeInfos.GetHeadPosition();
      if ( pos )
      {
         strHelp = "// Definitions for Interfaces other than IDispatch";
         strHelp += CRLF;
         f->Write(strHelp,strHelp.GetLength());
      }
      while ( pos )
      {
         ZTypeInfo *t = (ZTypeInfo *) m_pdispTypeInfos.GetAt( pos );
         strHelp.Empty();
         if ( t->GetTypeKind() == TKIND_INTERFACE )
         {
            strHelp += ((ZInterface *)t)->GetCPPDeclaration();
            strHelp += CRLF;
         }
         f->Write(strHelp,strHelp.GetLength());
         m_pdispTypeInfos.GetNext( pos );
      }
      // setup the list of neccesary dispatch interfaces
      // and store them in m_lDependendInterfaces
      if ( m_lDependendInterfaces.IsEmpty() )
      {
         CheckDependencies();
      }
      // 2.
      // loop for writing forward declarations
      pos = m_lDependendInterfaces.GetHeadPosition();
      if ( pos )
      {
         strHelp = "// Forward Declarations";
         strHelp += CRLF;
         f->Write(strHelp,strHelp.GetLength());
      }
      while ( pos )
      {
         pDispinterface = (ZDispinterface *) m_lDependendInterfaces.GetAt( pos );
         strHelp.Empty();
         // no code if no interface
         if ( pDispinterface )
         {
            strHelp += pDispinterface->GetForwardDeclaration();
            strHelp += CRLF;
            f->Write(strHelp,strHelp.GetLength());
         }

         m_lDependendInterfaces.GetNext( pos );
      }

      strHelp = "// End Forward Declarations";
      strHelp += CRLF;
      strHelp += CRLF;
      f->Write(strHelp,strHelp.GetLength());

      // 3.
      // loop for writing enums
      pos = m_pdispTypeInfos.GetHeadPosition();
      while ( pos )
      {
         ZTypeInfo *t = (ZTypeInfo *) m_pdispTypeInfos.GetAt( pos );
         CString strDoc;
         strHelp.Empty();

         if ( t->GetTypeKind() == TKIND_ENUM )
         {
            strDoc = t->GetDocumentationAsComment();
            pEnum = ( ZEnum *) t;
            strHelp += pEnum->GetCPPDeclaration();
            if ( !strHelp.IsEmpty() )
            {
               strHelp += CRLF;
            }
         }

         if (!strDoc.IsEmpty())
         {
            strHelp = strDoc + CRLF + strHelp;
            strHelp += CRLF;
         }
         // write code if available
         if ( !strHelp.IsEmpty() )
         {
            f->Write(strHelp,strHelp.GetLength());
         }
         m_pdispTypeInfos.GetNext(pos);
      }


      // 4.
      // loop for writing structures
      pos = m_pdispTypeInfos.GetHeadPosition();
      while ( pos )
      {
         ZTypeInfo *t = (ZTypeInfo *) m_pdispTypeInfos.GetAt( pos );
         CString strDoc;
         strHelp.Empty();

         if ( t->GetTypeKind() == TKIND_RECORD )
         {
            strDoc = t->GetDocumentationAsComment();
            pStruct = ( ZStruct *) t;
            strHelp += pStruct->GetCPPDefinition();
            if ( !strHelp.IsEmpty() )
               strHelp += CRLF;
         }
         if (!strDoc.IsEmpty())
         {
            strHelp = strDoc + CRLF + strHelp;
            strHelp += CRLF;
         }
         // write code if available
         if ( !strHelp.IsEmpty() )
         {
            f->Write(strHelp,strHelp.GetLength());
         }
         m_pdispTypeInfos.GetNext(pos);
      }

      // 5.
      // loop for writing unions
      pos = m_pdispTypeInfos.GetHeadPosition();
      while ( pos )
      {
         ZTypeInfo *t = (ZTypeInfo *) m_pdispTypeInfos.GetAt( pos );
         CString strDoc;
         strHelp.Empty();

         if ( t->GetTypeKind() == TKIND_UNION)
         {
            strDoc = t->GetDocumentationAsComment();
            pUnion = ( ZUnion *) t;
            strHelp += pUnion->GetCPPDefinition();
            if ( !strHelp.IsEmpty() )
               strHelp += CRLF;
         }
         if (!strDoc.IsEmpty())
         {
            strHelp = strDoc + CRLF + strHelp;
            strHelp += CRLF;
         }
         // write code if available
         if ( !strHelp.IsEmpty() )
         {
            f->Write(strHelp,strHelp.GetLength());
         }
         m_pdispTypeInfos.GetNext(pos);
      }

      // 6.
      // loop for writing warnings in case of unsupported
      // TypeKinds
      pos = m_pdispTypeInfos.GetHeadPosition();
      while ( pos )
      {
         ZTypeInfo *t = (ZTypeInfo *) m_pdispTypeInfos.GetAt( pos );
         strHelp.Empty();

         if (  t->GetTypeKind() < TKIND_ENUM
            && t->GetTypeKind() >= TKIND_MAX )
         {
            // Currently unsupported TKINDs
            strHelp += "unsupported TYPEKIND contact software vendor\n";
            strHelp += "Name is:";
            strHelp += TAB_SPACE;
            strHelp += t->GetName();
         }
         // write code if available
         if ( !strHelp.IsEmpty() )
         {
            f->Write(strHelp,strHelp.GetLength());
         }
         m_pdispTypeInfos.GetNext(pos);
      }

      // 7.
      // loop for writing class declarations
      pos = m_lDependendInterfaces.GetHeadPosition();
      while ( pos )
      {
         pDispinterface = (ZDispinterface *) m_lDependendInterfaces.GetAt( pos );
         strHelp.Empty();
         strHelp = pDispinterface->GetDocumentationAsComment();
         strHelp += CRLF;

         // no code if no interface
         if ( pDispinterface )
         {
            strHelp += pDispinterface->GetCPPDeclaration();
            strHelp += CRLF;
            f->Write(strHelp,strHelp.GetLength());
         }

         m_lDependendInterfaces.GetNext( pos );
      }

      // writing prototypes for global ops.
      pos = GetFirstGlobalDispatchPosition();
      while ( pos )
      {
         strHelp = CRLF;
         strHelp += lpszExternCProlog;
         ZGlobalDispatch *p = GetNextGlobalDispatch(pos);
         strHelp += p->GetCPPDeclaration();
         strHelp += CRLF;
         strHelp += lpszExternCEpilog;
         f->Write(strHelp,strHelp.GetLength());
      }

      f->Close();
      delete f;
   }
   CATCH( CFileException, e )
   {
      e->ReportError();
      bReturn = FALSE;
      if ( f )
         delete f;
   }
   END_CATCH
   return bReturn;
}

BOOL ZTypeLibrary::CreateDEF(LPCSTR lpszFileName, LPCSTR lpszZeidonScriptOper)
{
   BOOL           bReturn = TRUE;
   CString        strOrdinalToken, strHelp;
   UINT           nOpenFlags = 0;
   DWORD dwOrdinalTokenPosition = 0;
   int iOrdinal = 1;

   CFile *fCurrent = NULL;
   CMemFile *fNew = NULL;

   TRY
   {
      // truncate file if neccessary otherwise append
      // no matter what seek to end of file
      if ( m_bAppend )
         nOpenFlags = CFile::modeCreate|
                       CFile::modeNoTruncate|
                       CFile::modeReadWrite;
      else
         nOpenFlags = CFile::modeCreate|CFile::modeReadWrite;

      fCurrent = new CFile(lpszFileName, nOpenFlags );
      fNew = new CMemFile();

      if ( m_bAppend )
      {
         iOrdinal = GetMaxOrdinal( fCurrent, &dwOrdinalTokenPosition );
         fCurrent->SeekToBegin();
         // Read everything up to the ordinal Position and write it to the memfile
         LPSTR lpszHelp = strHelp.GetBufferSetLength( dwOrdinalTokenPosition );
         fCurrent->Read( lpszHelp, dwOrdinalTokenPosition );
         fNew->Write(lpszHelp, dwOrdinalTokenPosition);
         strHelp.ReleaseBuffer();
      }

      strHelp.Empty();
      CString strScriptRoutine;
      strScriptRoutine.Format("%s = _%s@12   @%d", lpszZeidonScriptOper, lpszZeidonScriptOper, iOrdinal );
      iOrdinal++;
      strHelp += CRLF;
      strHelp += strScriptRoutine;

      // get all def-entries of Zeidon Global operations
      POSITION pos = GetFirstGlobalDispatchPosition();
      while ( pos )
      {
         strHelp += CRLF;
         // write the Script Routine
         ZGlobalDispatch *p = GetNextGlobalDispatch(pos);
         strHelp += p->GetDefEntry( &iOrdinal );
      }

      strOrdinalToken.Format( lpszFormatOrdinal, lpszOrdinalToken, iOrdinal );
      // write the new ordinal with the new token
      // comment out the old line containing the token by appending ;;
      strOrdinalToken += CRLF;
      if ( !m_bAppend )
      {
         // initialize the new file with some default stuff
         strOrdinalToken += ";; Don't change the line above";
         strOrdinalToken += CRLF;
         strOrdinalToken += "EXPORTS";
         strOrdinalToken += CRLF;
      }

      fNew->Write( strOrdinalToken, strOrdinalToken.GetLength() );
      zCHAR c;

      if ( m_bAppend )
      {
         // append content of the old file;
         // current position is the old ordinal so comment it out
         fNew->Write(";;", 2);
         while ( fCurrent->Read( &c, 1 ))
            fNew->Write( &c, 1 );
      }
      //Now write the string with new defs
      fNew->Write( strHelp, strHelp.GetLength());

      // now fNew contains the old and the new stuff so overwrite the old file
      fNew->SeekToBegin();
      fCurrent->SeekToBegin();

      while ( fNew->Read( &c, 1 ) )
         fCurrent->Write( &c, 1 );
   }
   CATCH( CFileException, e )
   {
      e->ReportError();
      bReturn = FALSE;
   }
   END_CATCH

   if ( fNew )
   {
      fNew->Close();
      delete fNew;
   }

   if ( fCurrent )
   {
      fCurrent->Close();
      delete fCurrent;
   }

   return bReturn;
}

int ZTypeLibrary::GetMaxOrdinal( CFile *f, DWORD * dwOrdinalTokenPosition )
{
   int iCurrentOrdinal = 0;
   CString strHelp;
   CString strOrdinal;// this is a real big ordinal
   TCHAR ch = 0;

   long lOrdinalStartPos = 0;

   LPSTR lpszBuffer = NULL;

   // get current position to reset it when leaving this function
   DWORD dwLength = (DWORD) f->GetLength();

   if ( dwLength == 0 )
      return( 0 );

   DWORD dwPosition = (DWORD) f->GetPosition();

   lpszBuffer = strHelp.GetBufferSetLength( dwLength );

   f->SeekToBegin();

   f->Read( lpszBuffer, dwLength );

   strHelp.ReleaseBuffer();

   // look foor the first occurence of a numeric character
   // hopefully this is the beginning of our ordinal
   lOrdinalStartPos = strHelp.Find( lpszOrdinalToken );
   *dwOrdinalTokenPosition = lOrdinalStartPos;
   lOrdinalStartPos += zstrlen( lpszOrdinalToken );

   lOrdinalStartPos++;

   ch = strHelp.GetAt( lOrdinalStartPos );

   while (!isdigit(ch))
   {
      lOrdinalStartPos++;
      ch = strHelp.GetAt( lOrdinalStartPos);
   }

   // now ch contains the first numeric character
   // determinthe length of ordinal string
   int i = 0;
   while ( isdigit(ch) )
   {
      i++;
      ch = strHelp.GetAt( lOrdinalStartPos + i );
   }

   // i contains the count of characters of the ordinal string
   // allocate a string of this length
   LPSTR lpzsOrdinal = strOrdinal.GetBufferSetLength( i );
   i = 0;
   ch = strHelp.GetAt( lOrdinalStartPos );
   while ( isdigit(ch) )
   {
      strOrdinal.SetAt (i, ch);
      i++;
      ch = strHelp.GetAt( lOrdinalStartPos + i );
   }

   strOrdinal.ReleaseBuffer();

   if ( strOrdinal.GetLength() > 0 )
       iCurrentOrdinal = atoi( strOrdinal );

   f->Seek( dwPosition, CFile::begin );

   return iCurrentOrdinal;
}



BOOL ZTypeLibrary::WriteFileHeader(CFile *f)
{
   CTime ct = CTime::GetCurrentTime();
   CString strHelp;

   BOOL bReturn = TRUE;

   // writing some default stuff
   strHelp = "/**************************************************************";
   strHelp += CRLF;
   strHelp += TAB_SPACE;
   strHelp += "Generated wrapper for ActiveX library named: ";
   strHelp +=  GetName();
   strHelp += CRLF;
   strHelp += TAB_SPACE;
   strHelp += "Generated at:";
   strHelp += TAB_SPACE;
   strHelp += ct.Format( "%A, %B %d, %Y" );
   strHelp += CRLF;
   strHelp += "**************************************************************/";
   strHelp += CRLF;
   strHelp += CRLF;
   strHelp += CRLF;
   strHelp += CRLF;

   TRY
   {
      f->Write(strHelp, strHelp.GetLength());
   }
   CATCH(CFileException, e)
   {
      e->ReportError();
      bReturn = FALSE;
   }
   END_CATCH

   return bReturn;
}

long
ZTypeLibrary::PopulateGlobalOperations()
{
   ZDispinterface * i = GetEventInterface( m_csGUID );

   POSITION pos = m_pdispTypeInfos.GetHeadPosition();
   while ( pos )
   {
      CObject *o = m_pdispTypeInfos.GetAt(pos);
/*
      if (o->IsKindOf(RUNTIME_CLASS( ZDispinterface )))
      {
         if ( i != o ) // following stuff is not need to be done in case of an event interface
         {
            PopulateGlobalOperations( (ZDispinterface *) o );
            m_lGlobalDispatch.AddTail(new ZGlobalDispatch ( (ZDispinterface *) o ) );
         }
      }
*/
      if (o->IsKindOf(RUNTIME_CLASS( ZCoClass )))
      {
         CString strGUID = ( (ZCoClass *) o)->GetDefaultDispatchInterfaceGUID();
         ZDispinterface *pDispinterface = GetDispatchOfGUID( strGUID );
         if ( pDispinterface )
         {
            PopulateGlobalOperations( pDispinterface  );
            m_lGlobalDispatch.AddTail( new ZGlobalDispatch( pDispinterface ) );
         }
      }

      m_pdispTypeInfos.GetNext( pos );
   }

   return m_lGlobalDispatch.GetCount();
}

void
ZTypeLibrary::PopulateGlobalOperations( ZDispinterface *i )
{
   CString strName;
   CString strType;
   POSITION pos;
   CObList *l;
   ZOperation* o = NULL;

   l=i->GetProperties();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZProperty *f = (ZProperty *)l->GetAt( pos );

      // populating GetMethod
      strType = f->GetType()->GetCPPDefinition();        // returntype
      strName = f->GetClassName() + "_Get" + f->GetName();  // qualified name of global op
      o = new ZOperation();
      o->SetEntry( strType, strName );
      m_lGlobalOperations.AddHead( o );

      // population set method
      strType = "void";                                     // returntype alway void with set
      strName = f->GetClassName() + "_Set" + f->GetName();  // qualified name of global op
      o = new ZOperation();
      o->SetEntry( strType, strName );
      o->AddParameter(f->GetType()->GetCPPDefinition(), f->GetName());
      m_lGlobalOperations.AddHead( o );
      l->GetNext( pos );
   }

   l = i->GetMethods();
   pos = l->GetHeadPosition();
   while ( pos )
   {
      ZFunction *f = (ZFunction *)l->GetAt( pos );
      // populating Method
      // There is nothing to do if one of these conditions is true.
      if (!(f->GetFunctionFlags() & FUNCFLAG_FRESTRICTED ) && // Exist but should not be available to macro languages
          !(f->GetFunctionFlags() & FUNCFLAG_FHIDDEN )&&     // The function should not be displayed to the user, although it exists and is bindable.
          f->m_pfuncdesc != NULL)                          // Initialization error
      {
         strType = f->GetType()->GetCPPDefinition();;        // return type
         strName = f->GetClassName() + "_"+ f->GetPropertyMethodPrefix() + f->GetName();  // qualified name of global op
         o = new ZOperation();
         o->SetEntry( strType, strName );
         m_lGlobalOperations.AddHead( o );
         POSITION posParam = f->GetParameter()->GetHeadPosition();
         while ( posParam )
         {
            ZParameter *p = (ZParameter *)f->GetParameter()->GetAt( posParam );
            o->AddParameter(p->GetType()->GetCPPDefinition(), p->GetName());
            f->GetParameter()->GetNext( posParam );
         }
      }

      l->GetNext( pos );
   }
}

/************************************************************************
** Use this method to create a string that contains the  declaration of
** dispatchinterface as a C++ Class for a header
**
*/
CString ZDispinterface::GetCPPDeclaration()
{
   POSITION pos = NULL;
   CObList *l = NULL;
   CString strName = GetName();
   CString strOut;

   strOut.Format(lpszCPP_CLASS_DECLARATION,strName,lpszDISPATCH_DRIVER_BASE_CLASS);
   strOut+=lpszCPP_TOKEN_PUBLIC;

   // writing the constuctors there are three of them
   // an empty one
   strOut+=TAB_SPACE;
   strOut+=strName;
   strOut+="() {}";
   strOut+=CRLF;

   // one that take an dispatchpointer as argument
   strOut+=TAB_SPACE;
   strOut+=strName;
   strOut+="(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}";
   strOut+=CRLF;

   // and a copy constructor
   strOut+=TAB_SPACE;
   strOut+=strName;
   strOut+="(const ";
   strOut+=strName;
   strOut+="& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}";
   strOut+=CRLF;
   strOut+=CRLF;


   // write the declaration for the property Set and Get Methods
   // therefore process the list of properties
   strOut+="public: // methods for setting and getting poperties";
   strOut+= CRLF;
   strOut+= TAB_SPACE;
   l=GetProperties();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZProperty *f = (ZProperty *)l->GetAt( pos );
      strOut+= CRLF;
      strOut+=f->GetCPPDeclaration();
      l->GetNext( pos );
   }

   strOut+=CRLF;
   strOut+=CRLF;
   strOut+="public: // methods of the current Dispinterface";
   strOut+= CRLF;
   strOut+= TAB_SPACE;
   l=GetMethods();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZFunction *f = (ZFunction *)l->GetAt( pos );
      CString strHelp = f->GetCPPDeclaration();
      if ( !strHelp.IsEmpty() )
      {
         strOut+= CRLF;
         strOut+= TAB_SPACE;
         strOut+= strHelp;
      }
      l->GetNext( pos );
   }

   //----------------------------------------------------------------
   // ok closing brace an out of here
   strOut+=CRLF;
   strOut+="};";
   return strOut;
}

CString ZDispinterface::GetForwardDeclaration()
{
   CString strOut = "class ";
   strOut += GetName();
   strOut += ";";

   return strOut;
}

CString ZDispinterface::GetCPPDefinition()
{
   CString strOut;
   CObList *m = NULL;
   POSITION pos = NULL;

   // Write all the properties
   m = GetProperties();

   pos = m->GetHeadPosition();

   if ( pos )
   {
      strOut =  "/*------------------------------------------------------   \r\n";
      strOut += "**   Setting and getting properties of dispatch interface   \r\n";
      strOut += "**   ";
      strOut += GetName();
      strOut += CRLF;
      strOut += "**------------------------------------------------------*/\r\n";
   }

   while ( pos )
   {
      ZProperty *f = ( ZProperty * )m->GetAt( pos );
      strOut += CRLF;
      strOut += f->GetDocumentationAsComment();
      strOut += CRLF;
      strOut += f->GetCPPDefinition();
      m->GetNext( pos );
   }

   // write all methods
   m = GetMethods();

   pos = m->GetHeadPosition();

   if ( pos )
   {
      strOut +=  "/*------------------------------------------------------   \r\n";
      strOut += "**   Wrapper for invoking methods of dispatch interface     \r\n";
      strOut += "**   ";
      strOut += GetName();
      strOut += CRLF;
      strOut += "**------------------------------------------------------*/\r\n";
   }
   while ( pos )
   {
      ZFunction *f = ( ZFunction * )m->GetAt( pos );
      strOut += CRLF;
      strOut += f->GetDocumentationAsComment();
      strOut += CRLF;
      strOut += f->GetCPPDefinition();
      m->GetNext( pos );
   }
   return strOut;
}


//
// returns whether the function parameter indexed by parameter i
// is optional or not
//
BOOL ZFunction::IsOptionalParameter(SHORT i)
{
   BOOL bReturn = FALSE;

   if ((GetNumOfOptionalParams() == -1 && i == ( GetNumOfOptionalParams() - 1)) ||
       (i >= (GetNumOfParams() - GetNumOfOptionalParams())))
   {
      bReturn = TRUE;
   }
   else
   {
      bReturn = FALSE;
   }
   return bReturn;
}

//
// returns whether the function named by parameter strParam
// is optional or not
//
BOOL ZFunction::IsOptionalParameter( CString strParam )
{
   POSITION pos = NULL;
   BOOL bFound = FALSE;
   BOOL bReturn = FALSE;
   CObList *l = GetParameter();

   int i = 0;

   pos = l->GetHeadPosition();
   while ( pos && bFound == FALSE )
   {
      ZParameter *p = (ZParameter *)l->GetAt( pos );
      if ( p->GetName() == strParam )
      {
         bFound = TRUE;
      }
      if ( !bFound )
      {
         i++;
      }
      else
      {
         bReturn = IsOptionalParameter( i );
      }
   }
   return bReturn;
}




BOOL ZTypeLibrary::CreateC( LPCSTR lpszFileName, CString strScriptFunctionName )
{
   BOOL           bReturn = TRUE;
   CString        strHelp, strClassName = "";
   CString        strDllName = lpszFileName;
   UINT           nOpenFlags = 0;
   ZDispinterface *pI = GetDefaultDispatch( m_csGUID );

   // retrieve the default class name "name of the default interface"
   if ( !m_csNewDefaultClassName.IsEmpty() )
      strClassName = m_csNewDefaultClassName;
   if (pI && m_csNewDefaultClassName.IsEmpty())
      strClassName = pI->GetName();

   // extract name without extension
   int j = strDllName.ReverseFind( '.' );
   strDllName = strDllName.Mid( 0, j );
   j = strDllName.ReverseFind( '\\' );
   strDllName = strDllName.Mid( j + 1, strDllName.GetLength() - j );

   CFile *f = NULL;

   TRY
   {
      // truncate file if neccessary otherwise append
      // no matter what seek to end of file
      if ( m_bAppend )
         nOpenFlags = CFile::modeCreate|
                       CFile::modeNoTruncate|
                       CFile::modeReadWrite;
      else
         nOpenFlags = CFile::modeCreate|CFile::modeReadWrite;

      f = new CFile(lpszFileName, nOpenFlags );
      f->SeekToEnd();

      WriteFileHeader( f );

      if ( !m_bAppend )
      {
         strHelp.Format( lpszDEFAULT_INCLUDE, m_csHeaderFileName );
         f->Write(strHelp,strHelp.GetLength());

         strHelp = lpszDllMain;
         f->Write( strHelp, strHelp.GetLength( ) );

         strHelp.Format( lpszZeidonInitRoutine, strDllName );
         f->Write( strHelp, strHelp.GetLength( ) );
      }
      else
      {
         strHelp.Format( lpszDEFAULT_APPEND_INCLUDE, m_csHeaderFileName );
         f->Write( strHelp, strHelp.GetLength( ) );
      }

      strHelp.Format( lpszZeidonScriptInit,
                      strScriptFunctionName, strClassName );
      f->Write( strHelp, strHelp.GetLength( ) );

      strHelp.Format( lpszZeidonScriptMapping );
      f->Write( strHelp, strHelp.GetLength( ) );

      strHelp.Format( lpszZeidonScriptMisc,
                      strScriptFunctionName, strClassName );
      f->Write( strHelp, strHelp.GetLength( ) );

      POSITION pos = GetFirstGlobalDispatchPosition( );
      while ( pos )
      {
         strHelp = CRLF;
         strHelp += lpszExternCProlog;
         ZGlobalDispatch *p = GetNextGlobalDispatch( pos );
         strHelp += p->GetCPPDefinition( );
         strHelp += CRLF;
         strHelp += lpszExternCEpilog;
         f->Write( strHelp, strHelp.GetLength( ) );
      }

      f->Close( );
      delete f;
   }
   CATCH( CFileException, e )
   {
      e->ReportError( );
      bReturn = FALSE;
      if ( f )
         delete f;
   }
   END_CATCH
   return( bReturn );
}

// return the default dispatch interface
ZDispinterface * ZTypeLibrary::GetDefaultDispatch( LPCSTR lpszGUID )
{
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   CObject *o = NULL;
   CString strInterfaceGUID;
   CString strCoClassGuid;

   // iterate all the ZCoClass and check whether they are
   // the default interface
   while ( pos && strInterfaceGUID.IsEmpty() )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(ZCoClass)))
      {
         strCoClassGuid = ((ZCoClass *)o)->GetGUID_AsString();
         if ( strCoClassGuid == lpszGUID )// make sure we are at the correct coclass
         {
            strInterfaceGUID = ((ZCoClass *)o)->GetDefaultDispatchInterfaceGUID();
         }
      }

      m_pdispTypeInfos.GetNext( pos );
   }

   if ( !strInterfaceGUID.IsEmpty() )
      return GetDispatchOfGUID( strInterfaceGUID );

   return NULL;
}

// return the event interface
ZDispinterface *ZTypeLibrary::GetEventInterface( LPCSTR lpszGUID )
{
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   CObject *o = NULL;
   CString strInterfaceGUID;
   CString strGuid;

   // iterate all the ZCoClass and check whether they are
   // the default interface
   while ( pos && strInterfaceGUID.IsEmpty() )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      CString strName = ((ZTypeInfo *)o)->GetName();
      if ( o->IsKindOf(RUNTIME_CLASS(ZCoClass)))
      {
         strGuid = ((ZCoClass *)o)->GetGUID_AsString();

         // For some reason, the registry contains a GUID with lower case
         // characters ... so better compare with no case.
         if ( strGuid == lpszGUID ) // make sure we are at the correct coclass
         {
            strInterfaceGUID = ((ZCoClass *)o)->GetEventInterfaceGUID();
         }
      }

      m_pdispTypeInfos.GetNext( pos );
   }

   if ( !strInterfaceGUID.IsEmpty() )
      return GetDispatchOfGUID( strInterfaceGUID );

   return NULL;
}



ZDispinterface * ZTypeLibrary::GetDispatchOfGUID( CString strInterfaceGUID )
{
   // Get the name of that interface and look for it in the dispinterfaces
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   ZDispinterface *pD = NULL;
   BOOL bFound = FALSE;
   CObject *o = NULL;
   CString strName;

   while ( pos && !bFound )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(ZDispinterface)))
      {
         CString strGUID = ((ZDispinterface *)o)->GetGUID_AsString();
         if ( strInterfaceGUID == strGUID )
         {
            bFound = TRUE;
         }
      }
      if ( !bFound )
      {
         m_pdispTypeInfos.GetNext( pos );
      }
      else
      {
         pD = ((ZDispinterface *)o);
      }
   }
   return pD;
}

//
// scanning all ZCoClass Object whether they use IFontDisp or IPictureDisp
//
//

void ZTypeLibrary::QueryExternalClasses()
{
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   CObject *o = NULL;
   DWORD dwClsContext = CLSCTX_ALL ;
   CLSID clsid;
   LPUNKNOWN lpUnknown = NULL;
   HRESULT hr;

   OleInitialize( NULL );

   while ( pos )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS( ZCoClass )))
      {
         // Retrieve CLSID to instantiate the control
         clsid = ((ZCoClass *) o)->GetGUID();
      // TRACE1( "ClassId is :\t %s \n", ((ZCoClass *)o)->GetGUID_AsString() );

         hr = CoCreateInstance( clsid, NULL, dwClsContext, IID_IUnknown,
                                 (LPVOID *) &lpUnknown);
         if ( SUCCEEDED ( hr ) )
         {
            IPictureDisp * lpPicture = NULL;
            IFontDisp    * lpFont = NULL;
            lpUnknown->AddRef();

            hr = lpUnknown->QueryInterface( IID_IPictureDisp, (LPVOID *) &lpPicture );
            if ( SUCCEEDED ( hr ) && lpPicture != NULL )
            {
               lpPicture->AddRef();
               m_lExternalClasses.AddTail("IPictureDisp");
               lpPicture->Release();
            }

            hr = lpUnknown->QueryInterface( IID_IFontDisp, (LPVOID *) &lpFont );
            if ( SUCCEEDED ( hr ) && lpFont != NULL )
            {
               lpFont->AddRef();
               m_lExternalClasses.AddTail( "IFontDisp" );
               lpFont->Release();
            }

            lpUnknown->Release();
         }
      }

      m_pdispTypeInfos.GetNext( pos );
   }
}

IMPLEMENT_DYNAMIC(ZGlobalOperation, CObject)

CString
ZGlobalOperation::GetCPPDefinition( zBOOL bUsedForHeader )
{
   CString strOut, strHelp, strMethodName;
   CObList *lpParameter = m_pFunction->GetParameter();
   POSITION pos = NULL;
   INT i,j;

   LPCSTR lpszMethodProlog = "\r\n\
{\r\n\
   %s\r\n\
   %s * pInstance  = (%s*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );\r\n\
   if ( pInstance == NULL )\r\n\
      return %s;\r\n\
   %s pInstance->%s\r\n\
   return %s;\r\n\
}\r\n";


   strOut.Empty();
   i = j = 0;

   // there is nothing to do if one of those conditions are true
   if ( m_pFunction->m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
        m_pFunction->m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
        m_pFunction->m_pdispTypeDesc == NULL )                         // Initialization error
   {
      strOut.Empty();
      return( strOut );
   }

   // write returntype
   strOut = "zOPER_EXPORT ";
   strOut += m_pFunction->m_pdispTypeDesc->GetCPPDefinition();

   if ( m_pFunction->m_pdispTypeDesc->IsDispatchPointer() )
   {
      int k = strOut.ReverseFind( '*' );
      if ( k >= 0 )
         strOut = strOut.Mid( 0, k );
   }

   strOut.TrimLeft();
   strOut += " OPERATION ";

   // write classname followed by _ to have unique names
   strMethodName.Empty();
   strMethodName += m_pFunction->GetClassName();
   strMethodName += "_";

   // append methodname
   strMethodName += m_pFunction->GetPropertyMethodPrefix() + m_pFunction->GetName();
   CStringList *pMethodNames = m_pParentDispatch->GetObjectNames();
   int iCounter = 0;
   CString strCounter;
   strHelp = strMethodName;
   while ( pMethodNames->Find( strMethodName ) != NULL )
   {
      i++;
      strCounter.Format("%d", iCounter);
      strMethodName = strHelp + strCounter;
   }
   pMethodNames->AddTail( strMethodName );
   strOut += strMethodName;

   strOut += "( zVIEW vSubtask, zCPCHAR  cpcCtrlTag ";

   // write parameter if there is any
   if (lpParameter != NULL )
   {
      pos = lpParameter->GetHeadPosition();
      i   = lpParameter->GetCount();
   }

   while ( pos )
   {
      ZParameter *p = (ZParameter *)lpParameter->GetAt( pos );
      // use strHelp to setup parametertype and name separated by leading comma
      CString strParamHelp;
      strParamHelp += p->GetType()->GetCPPDefinition();
      strParamHelp += " ";
      CString strName = p->GetName();

      // if parameter has no name generate one that look like "param0"
      if ( strName.GetLength() < 1 )
      {
         strName.Format( lpszPARAMTEMPLATE, j );
      }

      // If current parameter is optional
      // add const and address operator
      if ( m_pFunction->IsOptionalParameter( j ) )
      {
         strParamHelp = " const " + strParamHelp;
         strName = "& " + strName;
      }

      strOut += ", ";
      strOut = strOut + strParamHelp + strName;
      lpParameter->GetNext( pos );
      j++;
   }

   strOut += " )";

   if ( bUsedForHeader )
      return strOut;

   CString strReturnType = m_pFunction->m_pdispTypeDesc->GetCPPDefinition();

   if ( m_pFunction->m_pdispTypeDesc->IsDispatchPointer() )
   {
      int k = strReturnType.ReverseFind( '*' );
      if ( k >= 0 )
         strReturnType = strReturnType.Mid( 0, k );
   }

   CString strReturnType2 = strReturnType;
   CString strReturnVariable = " ReturnValue ";
   CString strReturnVarDeclaration = strReturnType + " " + strReturnVariable + ";";
   if ( strReturnType == "void" )
   {
      strReturnType.Empty();
      strReturnType2.Empty();
      strReturnVariable.Empty();
      strReturnVarDeclaration.Empty();
   }
   else
   {
//      strReturnType = "( " + strReturnType + ") (0)";
      strReturnType = "( " + strReturnVariable + ")";
//      strReturnType2 = strReturnType2 + strReturnVariable + " = ";
      strReturnType2 = strReturnVariable + " = ";
   }

// format method
   strHelp.Format( lpszMethodProlog, strReturnVarDeclaration,
                                     m_pFunction->GetClassName(),
                                     m_pFunction->GetClassName(),
                                     strReturnType,
                                     strReturnType2,
                                     m_pFunction->GetMethodCall(),
                                     strReturnVariable );
   strOut += strHelp;
   strOut += CRLF;
   return( strOut );
}

CString ZGlobalOperation::GetCPPDeclaration( )
{
   CString strOut = GetCPPDefinition( TRUE );
   if ( !strOut.IsEmpty() )
   {
      strOut += ";";
   }
   return strOut;
}

CString ZGlobalOperation::GetDefEntry( int *piOrdinal)
{
   CString strOut;
   CString strOrdinal;
   CString strStackPart;
   CString strMethodName;
   CObList *lpParameter = m_pFunction->GetParameter();
   POSITION pos = NULL;

   // there is nothing to do if one of those conditions are true
   if ( m_pFunction->m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
        m_pFunction->m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
        m_pFunction->m_pdispTypeDesc == NULL )                         // Initialization error
   {
      strOut.Empty();
      return( strOut );
   }

   // write classname followed by _ to have unique names
   strMethodName.Empty();
   strMethodName += m_pFunction->GetClassName();
   strMethodName += "_";
   // append methodname
   strMethodName += m_pFunction->GetPropertyMethodPrefix() + m_pFunction->GetName();
   // write classname followed by _ to have unique names
   // Create a string that looks like MethodName = _MethodName@StackPart @iOrdinal
   strOut.Empty();
   strOut += strMethodName;
   strOut += " = _";
   strOut += strMethodName;

   // Get Parameters and compute the stackpart
   pos = lpParameter->GetHeadPosition();
   int iStackPart = 8;
   while ( pos )
   {
      ZParameter *p =(ZParameter *)lpParameter->GetNext(pos);
      iStackPart += p->GetType()->GetStackSize();
   }

   strStackPart.Format("@%d", iStackPart );
   strOut += strStackPart;

   // now set up the ordinal
   strOrdinal.Format( "   @%d", *piOrdinal );
   (*piOrdinal)++;

   strOut += strOrdinal;
   strOut += CRLF;

   return( strOut );
}


IMPLEMENT_DYNAMIC(ZGlobalDispatch, CObject)

ZGlobalDispatch::~ZGlobalDispatch()
{
   while ( !m_lOperations.IsEmpty() )
   {
      CObject *o = m_lOperations.RemoveHead();
      delete o;
   }
}

void
ZGlobalDispatch::SetInterface( ZDispinterface *i )
{
   m_pDispinterface = i;

   if ( i )
   {
      POSITION pos;
      CObList *l = i->GetMethods();
      for (pos = l->GetHeadPosition(); pos; l->GetNext(pos))
      {
         m_lOperations.AddHead( new ZGlobalOperation( (ZFunction *)l->GetAt( pos ),
                                                      this ) );
      }

      l = i->GetProperties();
      for (pos = l->GetHeadPosition(); pos; l->GetNext(pos))
      {
         m_lProperties.AddHead( new ZGlobalProperty( (ZProperty *)l->GetAt( pos ),
                                                     this ) );
      }
   }
}

CString
ZGlobalDispatch::GetCPPDefinition()
{
   CString strOut;
   POSITION pos;
   CObList *l;

   // delete all entries of objectnames to avoid duplicates
   // GetCPPDefinition of ZGlobalProperty and ZGlobalOperation
   // will fill this list again;
   GetObjectNames()->RemoveAll();

   l = &m_lProperties;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZGlobalProperty *f = (ZGlobalProperty *)l->GetAt( pos );
      strOut += CRLF;
      strOut += f->GetCPPDefinition();
      l->GetNext( pos );
   }

   l=&m_lOperations;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZGlobalOperation *f = (ZGlobalOperation *)l->GetAt( pos );
      CString strHelp = f->GetCPPDefinition();
      if ( !strHelp.IsEmpty() )
      {
         strOut+= CRLF;
         strOut+= TAB_SPACE;
         strOut+= strHelp;
      }

      l->GetNext( pos );
   }

   return( strOut );
}

CString
ZGlobalDispatch::GetCPPDeclaration()
{
   CString strOut;
   POSITION pos;
   CObList *l;

   // delete all entries of objectnames to avoid duplicates
   // GetCPPDeclaration of ZGlobalProperty and ZGlobalOperation
   // will fill this list again;
   GetObjectNames()->RemoveAll();

   l = &m_lProperties;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZGlobalProperty *f = (ZGlobalProperty *)l->GetAt( pos );
      strOut += CRLF;
      strOut += f->GetCPPDeclaration();
      l->GetNext( pos );
   }

   l = &m_lOperations;
   pos = l->GetHeadPosition();
   while ( pos )
   {
      ZGlobalOperation *f = (ZGlobalOperation *)l->GetAt( pos );
      CString strHelp = f->GetCPPDeclaration();
      if ( !strHelp.IsEmpty() )
      {
         strOut+= CRLF;
         strOut+= TAB_SPACE;
         strOut+= strHelp;
      }

      l->GetNext( pos );
   }

   return( strOut );
}

CString
ZGlobalDispatch::GetDefEntry( int *piOrdinal )
{
   CString strOut;
   POSITION pos;
   CObList *l;


   // first browse all the properties an get the Def. Entries
   l = &m_lProperties;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZGlobalProperty *f = (ZGlobalProperty *)l->GetNext( pos );
      strOut += f->GetDefEntry( piOrdinal );
   }

   // now browse all the operations an get the Def. Entries
   l=&m_lOperations;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      ZGlobalOperation *f = (ZGlobalOperation *)l->GetNext( pos );
      strOut += f->GetDefEntry( piOrdinal );
   }

   return( strOut );
}

IMPLEMENT_DYNAMIC(ZGlobalProperty, CObject)

CString
ZGlobalProperty::GetCPPDefinition( zBOOL bUsedForHeader )
{
   CString strOut;
   CString strType;
   CString strName;
   CString strHelp;
   LPCSTR lpszGetProlog = "\r\n\
{\r\n\
   %s ReturnValue; // to do: initialize as necessary\r\n\
   %s * pInstance  = (%s*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );\r\n\
   if ( pInstance == NULL )\r\n\
      return( ReturnValue );\r\n\r\n\
   return pInstance->Get%s();\r\n\
}\r\n";
   LPCSTR lpszSetProlog = "\r\n\
{\r\n\
   %s * pInstance  = (%s*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );\r\n\
   if ( pInstance == NULL )\r\n\
      return;\r\n\r\n\
   pInstance->Set%s( propVal );\r\n\
}\r\n";

   strType.Empty();
   strOut.Empty();
   ZTypeDesc  *t = m_pProperty->GetType ();

   if ( t )
   {
      // this should be the return type
      strType = t->GetCPPDefinition();

      // First the Get... method
      strOut += "zOPER_EXPORT ";
      strOut += strType;
      strOut += " OPERATION ";
      strOut += TAB_SPACE;

      // write classname followed by _ to have unique names
      CString strMethodName;
      strMethodName.Empty();
      strMethodName += m_pProperty->GetClassName();
      strMethodName += "_";
      strMethodName += "Get" + m_pProperty->GetName();

      // append methodname
      CStringList *pMethodNames = m_pParentDispatch->GetObjectNames();
      int iCounter = 0;
      CString strCounter;
      strHelp = strMethodName;
      while ( pMethodNames->Find( strMethodName ) != NULL )
      {
         iCounter++;
         strCounter.Format("%d", iCounter);
         strMethodName = strHelp + strCounter;
      }
      pMethodNames->AddTail( strMethodName );

      strOut += strMethodName;
      strOut += "( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )";
      if ( bUsedForHeader == FALSE )
      {
         strHelp.Format( lpszGetProlog,
                         strType,
                         m_pProperty->GetClassName(),
                         m_pProperty->GetClassName(),
                         m_pProperty->GetName() );
         strOut += strHelp;
         strOut += CRLF;
      }
      else
      {
         strOut += ";";
         strOut += CRLF;
      }

      // now the Set... method
      //strOut += "void";  // Setting any property is type of void
      strOut += "zOPER_EXPORT void OPERATION ";
      strOut += TAB_SPACE;

      // write classname followed by _ to have unique names
      strMethodName.Empty();
      strMethodName += m_pProperty->GetClassName();
      strMethodName += "_";
      strMethodName += "Set" + m_pProperty->GetName();

      // append methodname
      iCounter = 0;
      strCounter.Empty();
      strHelp = strMethodName;
      while ( pMethodNames->Find( strMethodName ) != NULL )
      {
         iCounter++;
         strCounter.Format("%d", iCounter);
         strMethodName = strHelp + strCounter;
      }
      pMethodNames->AddTail( strMethodName );

      strOut += strMethodName;
      strOut += "( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, ";
      strOut += strType;
      strOut += " propVal )";
      if ( bUsedForHeader == FALSE )
      {
         strHelp.Format( lpszSetProlog,
                         m_pProperty->GetClassName(),
                         m_pProperty->GetClassName(),
                         m_pProperty->GetName() );
         strOut += strHelp;
      }
      else
      {
         strOut += ";";
      }

      strOut += CRLF;
   }

   return( strOut );
}

CString ZGlobalProperty::GetCPPDeclaration()
{
   return( GetCPPDefinition( TRUE ) );
}

CString ZGlobalProperty::GetDefEntry( int *piOrdinal)
{
   CString strOut;
   CString strType;
   CString strOrdinal;
   CString strStackPart;

   // Get... Method
   // write classname followed by _ to have unique names
   CString strMethodName;
   strMethodName.Empty();
   strMethodName += m_pProperty->GetClassName();
   strMethodName += "_";
   strMethodName += "Get" + m_pProperty->GetName();

   // Create a string that looks like MethodName = _MethodName@StackPart @iOrdinal
   strOut.Empty();

   strOut += strMethodName;
   strOut += " = _";
   strOut += strMethodName;
   // Get Methods are always have no parameter so the stackpart is 0
   // but to identify the control we need 2 parameters zVIEW, szCtrlTag
   // so the stackpart is 8
   strOut += "@8";
   //now setup the ordinal
   strOrdinal.Format( "   @%d", *piOrdinal );
   (*piOrdinal)++;

   strOut += strOrdinal;
   strOut += CRLF;

   // now the Set... method
   strMethodName.Empty();
   strMethodName += m_pProperty->GetClassName();
   strMethodName += "_";
   strMethodName += "Set" + m_pProperty->GetName();
   strOut += strMethodName;
   strOut += " = _";
   strOut += strMethodName;
   //get the stackpart
   int iStackPart = 8;
   iStackPart += m_pProperty->GetType()->GetStackSize();
   strStackPart.Format("@%d", iStackPart);
   strOut += strStackPart;
   //now setup the ordinal
   strOrdinal.Format( "   @%d", *piOrdinal );
   (*piOrdinal)++;
   strOut += strOrdinal;
   strOut += CRLF;


   return( strOut );
}

IMPLEMENT_DYNAMIC(ZOperEntry, CObject)

IMPLEMENT_DYNAMIC(ZOperation, ZOperEntry)

ZOperation::~ZOperation()
{
   while ( !m_lParamList.IsEmpty() )
   {
      ZOperEntry *o = (ZOperEntry *)m_lParamList.RemoveHead( );
      delete o;
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZActiveX COM interface implementation

#if 0
BEGIN_INTERFACE_MAP( ZActiveX, CWnd )
   INTERFACE_PART( ZActiveX, IID_IOleClientSite, OleClientSite )
END_INTERFACE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// Implementation of IOleClientSite

STDMETHODIMP_( ULONG )
ZActiveX::XOleClientSite::AddRef( )
{
   METHOD_PROLOGUE_EX_( ZActiveX, OleClientSite )
   return( pThis->ExternalAddRef( ) );
}

STDMETHODIMP_( ULONG )
ZActiveX::XOleClientSite::Release( )
{
   METHOD_PROLOGUE_EX_( ZActiveX, OleClientSite )
   return( pThis->ExternalRelease( ) );
}

STDMETHODIMP
ZActiveX::XOleClientSite::QueryInterface( REFIID iid, LPVOID *ppvObj )
{
   METHOD_PROLOGUE_EX_( ZActiveX, OleClientSite )
   return( pThis->ExternalQueryInterface( &iid, ppvObj ) );
}

STDMETHODIMP
ZActiveX::XOleClientSite::SaveObject( )
{
   IPersistStorage   *pPersistStorage;
   SCODE             sc = S_OK;
   HRESULT           hr;

   METHOD_PROLOGUE_EX( ZActiveX, OleClientSite )
   ASSERT_VALID( pThis );

   if ( pThis->m_lpObject == 0 )
      return( E_FAIL );

   TRY
   {
      hr = pThis->m_lpObject->QueryInterface( IID_IPersistStorage,
                                                (LPVOID *) &pPersistStorage );
      if ( hr != S_OK )
         AfxThrowOleException( hr );

      // Check if we must save the Object
      if ( pPersistStorage->IsDirty( ) == S_OK )
      {
         // if the return parameter = S_OK then the object is dirty!
         /* sc = ::OleSave( pPersistStorage, pThis->m_lpStorage, TRUE );
         if ( sc != S_OK )
            sc = lpPersistStorage->SaveCompleted( 0 ); */
      }

      pPersistStorage->Release( );
   }
   CATCH( COleException, e )
   {

   }
   END_CATCH

   return( sc );

} // SaveObject

STDMETHODIMP
ZActiveX::XOleClientSite::GetMoniker( DWORD dwAssign,
                                      DWORD dwWhichMoniker,
                                      LPMONIKER *ppMoniker )
{
   METHOD_PROLOGUE_EX( ZActiveX, OleClientSite )
   ASSERT_VALID( pThis );
   return( E_NOTIMPL );
}

STDMETHODIMP
ZActiveX::XOleClientSite::GetContainer( LPOLECONTAINER *ppContainer )
{
#ifdef _DEBUG
   METHOD_PROLOGUE_EX( ZActiveX, OleClientSite )
#else
   METHOD_PROLOGUE_EX_( ZActiveX, OleClientSite )
#endif

   // We have no IOleItemContainer, so we must return a NULL.
   ppContainer = NULL;
   return( E_NOINTERFACE );
}

STDMETHODIMP
ZActiveX::XOleClientSite::ShowObject( )
{
   METHOD_PROLOGUE_EX( ZActiveX, OleClientSite )
   ASSERT_VALID( pThis );
   return( S_OK );

} // ShowObject

STDMETHODIMP
ZActiveX::XOleClientSite::OnShowWindow( BOOL fShow )
{
   METHOD_PROLOGUE_EX( ZActiveX, OleClientSite )
   ASSERT_VALID( pThis );
   return( S_OK );
}

STDMETHODIMP
ZActiveX::XOleClientSite::RequestNewObjectLayout( )
{
   return( E_NOTIMPL );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Create An ActiveX Control
AFX_EXT_API
CWnd * OPERATION
ActiveX( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
#ifdef zREMOTE_SERVER
   // Check if it is a truegrid
   CString        strTagPE;

   zPCHAR pch;
   GetAddrForAttribute( &pch, vDialog, szlCtrl, "TagPE" );
   strTagPE = pch;

   if ( strTagPE == "TrueGrid" )
      return( new ZWebGrid( pZSubtask, pWndParent,
                             pzmaComposite, vDialog,
                             nOffsetX, nOffsetY, pCtrlDef ) );
   else
      return( 0 );
#else
   return( new ZActiveX( pZSubtask, pWndParent,
                         pzmaComposite, vDialog,
                         nOffsetX, nOffsetY, pCtrlDef ) );
#endif
}
