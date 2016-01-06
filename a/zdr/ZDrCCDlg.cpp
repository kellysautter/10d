/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Corporation and TONBELLER AG.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrccdlg.cpp
// AUTHOR:       TMV
//
// OVERVIEW
// ========
// Source file for implementation of zdrapp initialization.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.08.07    TMV    R054917
//    Replace common Find/Replace Dialog to make use of Comboboxes
//    that store their values in the registry
//
// 2001.06.29    TMV
//    Common Dialogs.
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

/////////////////////////////////////////////////////////////////////////////
// ZRememberComboBox
#ifndef zREMOTE_SERVER
ZRememberComboBox::ZRememberComboBox( zSHORT nMaxEntries )
{
   m_nMaxEntries = nMaxEntries;
}

ZRememberComboBox::~ZRememberComboBox( )
{
}

zBOOL
ZRememberComboBox::LoadRegEntries( )
{
   HKEY  hKey;
   DWORD retVal;

   if ( (retVal = RegCreateKeyEx( HKEY_CURRENT_USER,
                                  m_csRegKey,
                                  0,
                                  NULL,
                                  0,
                                  KEY_ALL_ACCESS,
                                  NULL,
                                  &hKey,
                                  NULL )) == 0 )
   {
      DWORD dw, dwSize;
      TCHAR szValData[ 256 ];
      TCHAR szValName[ 4 ];

      for ( dw = 0; !retVal; dw++)
      {
         _stprintf( szValName, _T( "%d"), dw + 1);

         dwSize = 256 * sizeof( TCHAR);

         retVal = RegQueryValueEx( hKey,
                                  szValName,
                                  NULL,
                                  NULL,
                                  (LPBYTE) szValData,
                                  &dwSize);

         if ( !retVal )
         {
            if ( (zSHORT) dw < m_nMaxEntries && m_nMaxEntries )
               InsertString( dw, (LPCTSTR)szValData );
            else
               RegDeleteValue( hKey, szValName );
         }
      }

      RegCloseKey( hKey);
   }

   if ( retVal )
      return( FALSE );
   else
      return( TRUE );
}

zBOOL
ZRememberComboBox::LoadRegEntries( CString csRegKey )
{
   SetRegKey( csRegKey );

   return( LoadRegEntries( ) );
}

zBOOL
ZRememberComboBox::SaveRegEntries( )
{
   HKEY  hKey;
   DWORD retVal;

   if ( (retVal = RegCreateKeyEx( HKEY_CURRENT_USER,
                                  m_csRegKey,
                                  0,
                                  NULL,
                                  0,
                                  KEY_ALL_ACCESS,
                                  NULL,
                                  &hKey,
                                  NULL )) == 0 )
   {
      DWORD dw, dwSize;
      TCHAR szValName[ 4 ];
      TCHAR szValData[ 256 ];

      for ( dw = 0; dw < (DWORD) GetCount( ) && !retVal; dw++ )
      {
         _stprintf( szValName, _T( "%d"), dw + 1 );
         GetLBText( dw, szValData );
         dwSize = ( _tcslen( szValData) + 1) * sizeof( TCHAR );

         retVal = RegSetValueEx( hKey,
                                 szValName,
                                 0,
                                 REG_SZ,
                                 (BYTE *) szValData,
                                 dwSize );
      }

      RegCloseKey( hKey );
   }

   if ( retVal )
      return( FALSE );
   else
      return( TRUE );
}

void
ZRememberComboBox::SetRegKey( CString csRegKey )
{
   if ( !csRegKey.IsEmpty( ) )
      m_csRegKey = csRegKey;
}

CString
ZRememberComboBox::GetRegKey( )
{
   return( m_csRegKey );
}

void
ZRememberComboBox::SetMaxEntries( zSHORT nMaxEntries )
{
   zSHORT idx;

   if ( (idx = GetCount( )) != CB_ERR )
   {
      if ( idx > nMaxEntries && nMaxEntries )
      {
         for ( idx--; idx >= nMaxEntries; idx-- )
            DeleteString( idx );
      }

      m_nMaxEntries = nMaxEntries;
   }
}

zSHORT
ZRememberComboBox::GetMaxEntries( )
{
   return( m_nMaxEntries );
}

void
ZRememberComboBox::InsertCurEditText( )
{
   CString csText;
   zSHORT  idx;

   GetWindowText( csText );

   if ( csText && *csText )
   {
      if ( (idx = FindStringExact( -1, csText )) != CB_ERR )
         DeleteString( idx );

      InsertString( 0, csText );

      if ( (idx = GetCount( )) > m_nMaxEntries && m_nMaxEntries )
         DeleteString( idx - 1 );

      SetCurSel( 0 );
   }
}

BEGIN_MESSAGE_MAP( ZRememberComboBox, CComboBox )
   //{{AFX_MSG_MAP( ZRememberComboBox )
   ON_CONTROL_REFLECT( CBN_KILLFOCUS, OnKillFocus )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZRememberComboBox message handlers

void
ZRememberComboBox::OnKillFocus( )
{
   InsertCurEditText( );
   SaveRegEntries( );
}

BOOL
ZRememberComboBox::DestroyWindow( )
{
   InsertCurEditText( );

   SaveRegEntries( );

   return( CComboBox::DestroyWindow( ) );
}

/////////////////////////////////////////////////////////////////////////////

ZFindReplaceDialog::ZFindReplaceDialog( CWnd *pParent /* = 0 */ ) :
                    CFindReplaceDialog( )
{
   //{{AFX_DATA_INIT( ZFindReplaceDialog )
   m_csFindWhat = _T( "" );
   m_csReplaceWith = _T( "" );
   //}}AFX_DATA_INIT
}

void
ZFindReplaceDialog::DoDataExchange( CDataExchange *pDX )
{
   CFindReplaceDialog::DoDataExchange( pDX );
   //{{AFX_DATA_MAP( ZFindReplaceDialog )
   // these are for Replace Template only
   if ( GetDlgItem( IDC_REPLACE_ALL ) )
      DDX_Control( pDX, IDC_REPLACE_ALL, m_btnReplaceAll );
   if ( GetDlgItem( IDC_REPLACE ) )
      DDX_Control( pDX, IDC_REPLACE, m_btnReplace );
   if ( GetDlgItem( IDC_REPLACE_WITH ) )
   {
      DDX_CBString( pDX, IDC_REPLACE_WITH, m_csReplaceWith );
      DDX_Text( pDX, IDC_REPLACE_WITH, m_csReplaceWith  );
      DDX_Control( pDX, IDC_REPLACE_WITH, m_cmbReplaceWith );
   }

   // these are common for Find and Replace Template
   DDX_Control( pDX, IDC_COMBOFIND_WHAT, m_cmbFindWhat );
   DDX_Control( pDX, IDOK, m_btnIDOK );
   DDX_Text( pDX, IDC_COMBOFIND_WHAT, m_csFindWhat );
   DDX_CBString( pDX, IDC_COMBOFIND_WHAT, m_csFindWhat );
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( ZFindReplaceDialog, CFindReplaceDialog )
   //{{AFX_MSG_MAP( ZFindReplaceDialog )
   ON_CBN_EDITUPDATE( IDC_COMBOFIND_WHAT, OnEditupdateCombofindWhat )
   ON_CBN_EDITUPDATE( IDC_REPLACE_WITH,   OnEditupdateComboReplace )
   ON_CBN_SELCHANGE ( IDC_COMBOFIND_WHAT, OnSelChangeCombofindWhat )
   ON_CBN_SELCHANGE ( IDC_REPLACE_WITH,   OnSelChangeComboReplace )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZFindReplaceDialog message handlers
void
ZFindReplaceDialog::OnEditupdateCombofindWhat( )
{
   // TODO: Add your control notification handler code here
   UpdateData( );
   if ( m_csFindWhat.GetLength( ) > 0 )
      m_btnIDOK.EnableWindow( TRUE );
   else
      m_btnIDOK.EnableWindow( FALSE );
}

void
ZFindReplaceDialog::OnEditupdateComboReplace( )
{
   // TODO: Add your control notification handler code here
   UpdateData( );
   if ( m_csReplaceWith.GetLength( ) > 0 )
   {
      m_btnReplace.EnableWindow( TRUE );
      m_btnReplaceAll.EnableWindow( TRUE );
   }
   else
   {
      m_btnReplace.EnableWindow( FALSE );
      m_btnReplaceAll.EnableWindow( FALSE );
   }
}

BOOL
ZFindReplaceDialog::OnInitDialog()
{
   m_csFindWhat = m_fr.lpstrFindWhat;
   m_csReplaceWith = m_fr.lpstrReplaceWith;

   CFindReplaceDialog::OnInitDialog();

   if ( mIs_hWnd( m_cmbFindWhat.m_hWnd ) )
      m_cmbFindWhat.LoadRegEntries("Software\\Zeidon Application\\Find" );

   if ( mIs_hWnd( m_cmbReplaceWith.m_hWnd ) )
      m_cmbReplaceWith.LoadRegEntries("Software\\Zeidon Application\\Replace" );

   return( TRUE );  // return TRUE unless you set the focus to a control
                    // EXCEPTION: OCX Property Pages should return FALSE
}

zBOOL
ZFindReplaceDialog::IsReplace()
{
   return( GetDlgItem( IDC_REPLACE_ALL ) ? TRUE : FALSE );
}

void
ZFindReplaceDialog::OnSelChangeCombofindWhat( )
{
   CString csHelp;
   m_cmbFindWhat.GetLBText( m_cmbFindWhat.GetCurSel(), csHelp );
   if ( csHelp.GetLength( ) > 0 )
      m_btnIDOK.EnableWindow( TRUE );
   else
      m_btnIDOK.EnableWindow( FALSE );
}

void
ZFindReplaceDialog::OnSelChangeComboReplace( )
{
   CString csHelp;
   m_cmbReplaceWith.GetLBText( m_cmbReplaceWith.GetCurSel(), csHelp );
   if ( csHelp.GetLength( ) > 0 )
   {
      m_btnReplace.EnableWindow( TRUE );
      m_btnReplaceAll.EnableWindow( TRUE );
   }
   else
   {
      m_btnReplace.EnableWindow( FALSE );
      m_btnReplaceAll.EnableWindow( FALSE );
   }
}

#endif //zREMOTE_SERVER
