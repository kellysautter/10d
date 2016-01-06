/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdraxlst.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of classes used to browse the Registry for
// installed ActiveX components and to build event lists for the components.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.11.23  TMV   Z10    ActiveX
//   Fixing Memory leaks of CBitmaps.
//
// 1999.07.02    DKS    Z10    ActiveX
//    Win9x still cannot successfully load bitmap for ActiveX controls.
//
// 1999.07.01    TMV    Z10    ActiveX
//    Avoid retrieving the computer name of current machine in
//    ZActiveX_CtrlList because GetComputerName fails in Win9x.
//    Simply call RegConnectRegistry with null computer name.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Store visiblity attribute in ActiveX control structure.
//
// 1999.01.26    DKS
//    Blocked out GetAttributeCount code (hard-coded to the DataTable).
//
// 1998.06.03    TMV
//    more security in constructor of ZActiveX_CtrlList because of an
//    error-report of LANDESBANK Mainz
//
//    in general this module now opens and browses the registry with
//    Flag KEY_READ
//
//    FindItemByName replaced string "TypeLib" by variablename strKeyName
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

#define zBUFFSIZE  512

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZActiveX_CtrlList, CObList )
IMPLEMENT_DYNAMIC( ZActiveX_Ctrl, CObject )

ZActiveX_Ctrl::ZActiveX_Ctrl( )
{
   m_pcsToolbarBitmap = 0;
   m_pcsDescription = 0;
   m_pcsProgId = 0;
   m_pcsCLSID = 0;
   m_pcsTypeLib = 0;
   m_pBitmap = 0;
}

ZActiveX_Ctrl::ZActiveX_Ctrl( zCPCHAR cpcCLSID, zCPCHAR cpcProgId,
                              zCPCHAR cpcDescription,
                              zCPCHAR cpcToolbarBitmap,
                              zCPCHAR cpcTypeLib, zLONG lKey )
{
   m_pcsToolbarBitmap = 0;
   m_pcsDescription = 0;
   m_pcsProgId = 0;
   m_pcsCLSID = 0;
   m_pcsTypeLib = 0;
   m_lKey = 0;
   m_pBitmap = 0;

   SetKey( lKey );
   SetClassId( cpcCLSID );
   SetProgId( cpcProgId );
   SetDescription( cpcDescription );
   SetToolbarBitmap( cpcToolbarBitmap );
   SetTypeLib( cpcTypeLib );
}

ZActiveX_Ctrl::~ZActiveX_Ctrl( )
{
   mDeleteInit( m_pcsToolbarBitmap );
   mDeleteInit( m_pcsDescription );
   mDeleteInit( m_pcsProgId );
   mDeleteInit( m_pcsCLSID );
   mDeleteInit( m_pcsTypeLib );
   mDeleteInit( m_pBitmap );
}

zBOOL
ZActiveX_Ctrl::SetKey( zLONG lKey )
{
   zBOOL bReturn;
   if ( m_lKey == 0 )
   {
      m_lKey = lKey;
      bReturn = TRUE;
   }
   else
   {
   // TRACE2( "Attempt to update Key ** %d ** with new value ** %d **\n",
   //         m_lKey, lKey );
      bReturn = FALSE;
   }

   return( bReturn );
}

zBOOL
ZActiveX_Ctrl::SetClassId( zCPCHAR cpcCLSID )
{
   zBOOL bReturn;
   if ( m_pcsCLSID == 0 )
   {
      m_pcsCLSID = new CString( cpcCLSID );
      m_pcsCLSID->MakeUpper();
      bReturn = TRUE;
   }
   else
   {
   // TRACE2( "Attempt to update CLSID ** %s ** with new value ** %s **\n",
   //         *m_pcsCLSID, cpcCLSID );
      bReturn = FALSE;
   }

   return( bReturn );
}

zBOOL
ZActiveX_Ctrl::SetDescription( zCPCHAR cpcDescription )
{
   zBOOL bReturn;
   if ( m_pcsDescription == 0 )
   {
      m_pcsDescription = new CString( cpcDescription );
      bReturn = TRUE;
   }
   else
   {
    //TRACE2( "Attempt to update Description ** %s ** with new value ** %s **\n",
    //        *m_pcsDescription, cpcDescription );
      bReturn = FALSE;
   }

   return( bReturn );
}

zBOOL
ZActiveX_Ctrl::SetProgId( zCPCHAR cpcProgId )
{
   zBOOL bReturn;
   if ( m_pcsProgId == 0 )
   {
      m_pcsProgId = new CString( cpcProgId );
      bReturn = TRUE;
   }
   else
   {
   // TRACE2( "Attempt to update ProgId ** %s ** with new value ** %s **\n",
   //         *m_pcsProgId, cpcProgId );
      bReturn = FALSE;
   }

   return( bReturn );
}

zBOOL
ZActiveX_Ctrl::SetToolbarBitmap( zCPCHAR cpcToolbarBitmap )
{
   zBOOL bReturn;
   if ( m_pcsToolbarBitmap == 0 )
   {
      m_pcsToolbarBitmap = new CString( cpcToolbarBitmap );
      bReturn = TRUE;
   }
   else
   {
   // TRACE2( "Attempt to update ToolbarBitmap ** %s ** with new value ** %s **\n",
   //         *m_pcsToolbarBitmap, cpcToolbarBitmap );
      bReturn = FALSE;
   }

   return( bReturn );
}

zBOOL
ZActiveX_Ctrl::SetTypeLib( zCPCHAR cpcTypeLib )
{
   zBOOL bReturn;
   if ( m_pcsTypeLib == 0 )
   {
      m_pcsTypeLib = new CString( cpcTypeLib );
      bReturn = TRUE;
   }
   else
   {
   // TRACE2( "Attempt to update TypeLib ** %s ** with new value ** %s **\n",
   //         *m_pcsTypeLib, cpcTypeLib );
      bReturn = FALSE;
   }

   return( bReturn );
}

zLONG
ZActiveX_Ctrl::GetKey( )
{
   return( m_lKey );
}

zCPCHAR
ZActiveX_Ctrl::GetToolbarBitmapName( )
{
   return( *m_pcsToolbarBitmap );
}

zCPCHAR
ZActiveX_Ctrl::GetDescription( )
{
   return( *m_pcsDescription );
}

zCPCHAR
ZActiveX_Ctrl::GetProgId( )
{
   return( *m_pcsProgId );
}

zCPCHAR
ZActiveX_Ctrl::GetClassId( )
{
   return( *m_pcsCLSID );
}

zCPCHAR
ZActiveX_Ctrl::GetTypeLib( )
{
   return( *m_pcsTypeLib );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZActiveX_CtrlList::ZActiveX_CtrlList( zCPCHAR cpcMachineName )
{
   HKEY    hConnect = 0;
   zBOOL   bProcessError = FALSE;
   CString csMsgTitle;
   HKEY    hClassesRoot = 0;
   zLONG   lReturn = 0;

   m_hKeyTypeLib = 0;
   m_hKeyCLSIDS = 0;

   if ( cpcMachineName == 0 )
   {
      // Connect to the registry of local machine
      lReturn = RegConnectRegistry( NULL, HKEY_LOCAL_MACHINE, &hConnect );
   }
   else
   {
      // Check machine name for leading backslashes
      CString strMachineName = cpcMachineName;
      if ( strMachineName.GetAt( 0 ) != '\\' ||
           strMachineName.GetAt( 1 ) != '\\' )
      {
         strMachineName = "\\" + strMachineName;
         strMachineName = "\\" + strMachineName;
      }

      // Connect to the registry of the given machine
      lReturn = RegConnectRegistry( (LPSTR)(LPCSTR) strMachineName,
                                    HKEY_LOCAL_MACHINE, &hConnect );
   }

   if ( lReturn != ERROR_SUCCESS )
   {
      bProcessError = TRUE;
      csMsgTitle = "ZActiveX_CtrlList Registry Connect";
   }
   else
   {
      // Getting the right entry to query CLSID's HKEY_CLASSES_ROOT\CLSID
      if ( RegOpenKeyEx( HKEY_CLASSES_ROOT, 0,
                         0, KEY_READ, &hClassesRoot ) != ERROR_SUCCESS )
      {
         bProcessError = TRUE;
         csMsgTitle = "ZActiveX_CtrlList HKEY_CLASSES_ROOT";
      }
      else
      {
         if ( RegOpenKeyEx( hClassesRoot, "CLSID",
                            0, KEY_READ, &m_hKeyCLSIDS ) != ERROR_SUCCESS )
         {
            bProcessError = TRUE;
            csMsgTitle = "ZActiveX_CtrlList CLSID";
         }

         if ( RegOpenKeyEx( hClassesRoot, "TypeLib",
                            0, KEY_READ, &m_hKeyTypeLib ) != ERROR_SUCCESS )
         {
            bProcessError = TRUE;
            csMsgTitle = "ZActiveX_CtrlList TypeLib";
         }
      }
   }

   if ( bProcessError )
   {
      zPCHAR pchMsg;
      ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
                                              FORMAT_MESSAGE_ALLOCATE_BUFFER,
                       0, GetLastError( ),
                       MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                       (zPCHAR) &pchMsg, 0, 0 );
      ::MessageBox( 0, (zCPCHAR) pchMsg, csMsgTitle,
                    MB_OK | MB_ICONINFORMATION );

      // Free the buffer.
      LocalFree( pchMsg );
   }

   if ( hConnect )
      RegCloseKey( hConnect );

   if ( hClassesRoot )
      RegCloseKey( hClassesRoot );
}

zSHORT
ZActiveX_CtrlList::PopulateZeidonRegisteredActiveX( )
{
   CString  csClassId;
   zVIEW    vTZPESRCO;
   zLONG    lKey;
   zLONG    lVisible;
   zPCHAR   pch;
   zSHORT   nRC;

   if ( GetCount( ) > 0 )
      DeleteAllEntries( );

   GetViewByName( &vTZPESRCO, "TZPESRCO", m_v, zLEVEL_TASK );
   nRC = SetCursorFirstEntity( vTZPESRCO, "ControlDef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetAddrForAttribute( &pch, vTZPESRCO, "ControlDef", "GUID" );
      GetIntegerFromAttribute( &lVisible, vTZPESRCO,
                               "ControlDef", "Invisible" );
      if ( *pch && (lVisible == 0 || lVisible == -1) )
      {
         GetIntegerFromAttribute( &lKey, vTZPESRCO, "ControlDef", "Key" );
         csClassId = pch;
         CreateItemFromClassId( csClassId, lKey );
      }

      nRC = SetCursorNextEntity( vTZPESRCO, "ControlDef", 0 );
   }

   return( GetCount( ) );
}

zSHORT
ZActiveX_CtrlList::PopulateZeidonUnregisteredActiveX( )
{
   zLONG    lResult = ERROR_SUCCESS;
   DWORD    dwIndex = 0;               // index of subkey to enumerate
   DWORD    dwClassSize = zBUFFSIZE;   // for size of buffers
   zPCHAR   pchName = 0;               // buffer for subkey name
   zPCHAR   pchClass = 0;              // buffer for class string
   FILETIME ftLastWriteTime;
   CString  csName;
   CString  csClass;

   csName.Empty( );
   pchName = csName.GetBufferSetLength( zBUFFSIZE );
   pchClass = csClass.GetBufferSetLength( zBUFFSIZE );

   while ( lResult != ERROR_NO_MORE_ITEMS )
   {
      lResult = RegEnumKeyEx( m_hKeyCLSIDS, dwIndex,
                              pchName, &dwClassSize,
                              0, // reserved
                              pchClass, &dwClassSize,
                              &ftLastWriteTime );
      dwIndex++;
      dwClassSize = zBUFFSIZE;
      if ( KeyIsControl( m_hKeyCLSIDS, pchName ) )
      {
         CreateItemFromClassId( pchName );
      }
   }

   return( GetCount( ) );
}

zBOOL
ZActiveX_CtrlList::KeyIsControl( HKEY hKeyParent, CString csCLSID )
{
   HKEY  hKey = 0;
   zBOOL bReturn = FALSE;

   if ( csCLSID.GetLength( ) < 1 )
   {
      ; // bReturn = FALSE;
   }
   else
   {
      CString csControl = csCLSID + "\\Control";

      // Try to get the rootentry for the current CLSID.  All underlying
      // entries are children so we store the key in hKeyParent.
      if ( RegOpenKeyEx( hKeyParent, csControl, 0,
                         KEY_READ, &hKey )== ERROR_SUCCESS )
      {
         bReturn = TRUE;
         RegCloseKey( hKey );
      }
   }

   return( bReturn );
}

// Destroy current list
zSHORT
ZActiveX_CtrlList::DeleteAllEntries( )
{
   zSHORT  k = 0;
   CObject *pObject = 0;

   while ( IsEmpty( ) == FALSE )
   {
      pObject = RemoveHead( );
      if ( pObject )
      {
         delete( pObject );
         k++;
      }
   }

   return( k );
}

/////////////////////////////////////////////////////////////////////////////
// Browse the Registry!  If there is an Entry for the specified CLSID,
// find related information and create a new list entry.
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZActiveX_CtrlList::CreateItemFromClassId( CString csClassId,
                                          zLONG   lKey )
{
   HKEY   hKeyParent = 0;
   zBOOL  bReturn = FALSE;
   zLONG  lReturn = 0;
   ZActiveX_Ctrl *pActiveX = 0;

   if ( csClassId.GetLength( ) < 1 )
   {
      bReturn = FALSE;
   }
   else
   {
      // Try to get the root entry for the current CLSID.  All underlying
      // entries are child entries so we store the key in hKeyParent.
      if ( RegOpenKeyEx( m_hKeyCLSIDS, csClassId, 0,
                         KEY_READ, &hKeyParent ) == ERROR_SUCCESS )
      {
         CString csDesc;
         CString csProgId;
         CString csToolboxBitmap;
         CString csInprocServer32;
         CString csTypeLibKey;
         CString csTypeLibFile;

         QueryKeyValue( m_hKeyCLSIDS, csClassId, &csDesc );
         QueryKeyValue( hKeyParent, "ProgID", &csProgId );
         QueryKeyValue( hKeyParent, "ToolboxBitmap32", &csToolboxBitmap );
         QueryKeyValue( hKeyParent, "InprocServer32", &csInprocServer32 );
         QueryKeyValue( hKeyParent, "TypeLib", &csTypeLibKey );
         QueryTypeLibFile( csTypeLibKey, &csTypeLibFile );
         if ( csTypeLibFile.IsEmpty( ) )
            csTypeLibFile = csInprocServer32;

         pActiveX = new ZActiveX_Ctrl( csClassId, csProgId,
                                       csDesc, csToolboxBitmap,
                                       csTypeLibFile, lKey );
         AddTail( pActiveX );
         RegCloseKey( hKeyParent );
      }
   }

   return( bReturn );
}

zBOOL
ZActiveX_CtrlList::QueryKeyValue( HKEY    hKeyParent,
                                  CString csName,
                                  CString *csBuffer )
{
   zBOOL  bReturn = FALSE;
   HKEY   hKeyChild = 0;
   DWORD  dwDataLth = 256;             // data buffer size
   zPCHAR pchData = 0;                 // address of data buffer
   DWORD  dwType = REG_SZ;
   zLONG  lReturn = ERROR_MORE_DATA;   // Go into the loop at least one time

   if ( RegOpenKeyEx( hKeyParent, csName,
                      0, KEY_READ, &hKeyChild ) == ERROR_SUCCESS )
   {
      while ( lReturn == ERROR_MORE_DATA )
      {
         dwDataLth *= 2;  // double the buffer size
         pchData = csBuffer->GetBufferSetLength( dwDataLth );
         lReturn = RegQueryValueEx( hKeyChild, "",
                                    0,                // reserved must be null
                                    &dwType,          // value type
                                    (LPBYTE) pchData, // data buffer
                                    &dwDataLth );     // data buffer size
      }

      if ( lReturn == ERROR_SUCCESS )
      {
         bReturn = TRUE;
         csBuffer->ReleaseBuffer( );
      }
   }

   if ( hKeyChild )
      RegCloseKey( hKeyChild );

   return( bReturn );
}

ZActiveX_CtrlList::~ZActiveX_CtrlList( )
{
   if ( m_hKeyCLSIDS )
      RegCloseKey( m_hKeyCLSIDS );

   if ( m_hKeyTypeLib )
      RegCloseKey( m_hKeyTypeLib );

   DeleteAllEntries( );
// mDeleteInit( m_pcsMachineName );
}

ZActiveX_Ctrl *
ZActiveX_CtrlList::FindRegControl( CString csCLSID )
{
   POSITION posAX_Ctrl = 0;
   zBOOL    bFound = FALSE;
   ZActiveX_Ctrl *pAX_Ctrl;

   posAX_Ctrl = GetHeadPosition( );
   while ( posAX_Ctrl && bFound == FALSE )
   {
      pAX_Ctrl = (ZActiveX_Ctrl *) GetAt( posAX_Ctrl );
      if ( pAX_Ctrl->GetClassId( ) == csCLSID )
         bFound = TRUE;
      else
         GetNext( posAX_Ctrl );
   }

   return( pAX_Ctrl );
}

zBOOL
ZActiveX_CtrlList::QueryTypeLibFile( CString csTypeLibClsid,
                                     CString *pcsReturn )
{
   HKEY  hTypeLibCLSID = 0;
   zBOOL bReturn = TRUE;

   HRESULT hr = RegOpenKeyEx( m_hKeyTypeLib, csTypeLibClsid,
                              0, KEY_READ, &hTypeLibCLSID );
   if ( hr == ERROR_SUCCESS )
   {
      bReturn = FindItemByName( hTypeLibCLSID, "win32", pcsReturn );
   }

   if ( hTypeLibCLSID )
      RegCloseKey( hTypeLibCLSID );

   return( bReturn );
}

zBOOL
ZActiveX_CtrlList::FindItemByName( HKEY    hKeyParent,
                                   CString csKeyName,
                                   CString *pcsReturn )
{
   zLONG    lResult = ERROR_SUCCESS;
   DWORD    dwIndex = 0;               // index of subkey to enumerate
   DWORD    dwClassSize = zBUFFSIZE;   // for size of buffers
   zPCHAR   pchName = 0;               // buffer for subkey name
   zPCHAR   pchClass = 0;              // buffer for class string
   FILETIME ftLastWriteTime;
   CString  csName;
   CString  csClass;
   zBOOL    bFound = FALSE;

   csName.Empty( );
   pchName = csName.GetBufferSetLength( zBUFFSIZE );
   pchClass = csClass.GetBufferSetLength( zBUFFSIZE );

   while ( lResult != ERROR_NO_MORE_ITEMS && bFound == FALSE )
   {
      lResult = RegEnumKeyEx( hKeyParent, dwIndex,
                              pchName, &dwClassSize,
                              0, // reserved
                              pchClass, &dwClassSize,
                              &ftLastWriteTime );
      dwIndex++;
      dwClassSize = zBUFFSIZE;
      if ( csKeyName == pchName )
      {
         bFound = TRUE;
         QueryKeyValue( hKeyParent, csKeyName, pcsReturn );
      }
      else
      {
         // if current value is not the right one
         // open subkey and call Function recursively
         HKEY hChild = 0;
         if ( RegOpenKeyEx( hKeyParent, csKeyName,
                            0, KEY_READ, &hChild ) == ERROR_SUCCESS )
         {
            bFound = FindItemByName( hChild, csKeyName, pcsReturn );
            RegCloseKey( hChild );
         }
      }
   }

   return( bFound );
}

CBitmap *
ZActiveX_Ctrl::GetControlBitmap( )
{
   HINSTANCE hInstActiveX = 0;
   CBitmap   *pBitmap = 0;
   CString   csFileName;
   CString   csIndex;
   CString   csBitmapName;
   zSHORT    nComma;
   zSHORT    nIdx = 0;

   if ( m_pBitmap )
      return( m_pBitmap );

   // Extracting Filename and Index of the bitmap
   csBitmapName = GetToolbarBitmapName( );
   nComma = csBitmapName.ReverseFind( ',' );
   if ( nComma >= 0 )
   {
      csFileName = csBitmapName.Left( nComma );
      csIndex = csBitmapName.Right( csBitmapName.GetLength( ) - nComma - 1 );
      csIndex.TrimLeft( );
      nIdx = atoi( csIndex );
   }

   hInstActiveX = LoadLibraryEx( csFileName, 0, LOAD_LIBRARY_AS_DATAFILE );
   if ( hInstActiveX )
   {
#if 1
      HBITMAP hIcon = ::LoadBitmap( hInstActiveX,
                                    MAKEINTRESOURCE( (WORD) nIdx ) );
#else
      HBITMAP hIcon = (HBITMAP) ::LoadImage( hInstActiveX, MAKEINTRESOURCE( (WORD) nIdx ),
                                             IMAGE_BITMAP, 0, 0,
                                             LR_DEFAULTSIZE | LR_SHARED );
      if ( hIcon == 0 )
      {
         zPCHAR  pchMsg;
         DWORD   dwError = GetLastError( );
         CString csMsgTitle = "Load Image Failed ";
         csMsgTitle += csBitmapName;
         ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
                                              FORMAT_MESSAGE_ALLOCATE_BUFFER,
                          0, dwError,
                          MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                          (zPCHAR) &pchMsg, 0, 0 );
         ::MessageBox( 0, (zCPCHAR) pchMsg, csMsgTitle,
                       MB_OK | MB_ICONINFORMATION );

         // Free the buffer.
         LocalFree( pchMsg );
      }
#endif
      if ( hIcon == 0 )
         hIcon = (HBITMAP)::ExtractIcon( hInstActiveX, csFileName, 1 );

      if ( hIcon )
      {
         pBitmap = new CBitmap;
         pBitmap->Attach( hIcon );
      }

      FreeLibrary( hInstActiveX );
   }

   // In the case where something has gone wrong, load default bitmap.
   if ( pBitmap == 0 )
   {
      pBitmap = new CBitmap;
      pBitmap->LoadBitmap( IDB_ACTIVEX );
   }

   m_pBitmap = pBitmap;
   return( pBitmap );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
