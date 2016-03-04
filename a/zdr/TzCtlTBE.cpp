/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Control implementations
// FILE:         tzctltbe.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon ActiveX Editor control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2000.07.25    TMV
//   GetTextFromRange(LPSTR pBuffer, long lStart, long lEnd )
//   accidently calls GetTextFromLine instead of
//   GetTextFromRange(BSTR* pBuffer, long lStart, long lEnd )
//
// 2000.07.24    TMV
//   Introduced new method ZDrTbEdt::GetTextFromRange & ZDrTbEdt::GetObjectName
//
// 2000.03.16    TMV
//   Introduced new method ZDrTbEdt::SelectRange
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

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


/*
Dispatch interface for TBEdit Control
*/
/*------------------------------------------------------
**   Wrapper for invoking methods of dispatch interface
**   ZDrTBEdt
**------------------------------------------------------*/


BOOL   ZDrTBEdt::OpenObject( BSTR* FileName )
{
   BOOL result;
   static BYTE parms[] =
      VTS_PBSTR;
   InvokeHelper( 0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      FileName );
   return result;
}

BOOL   ZDrTBEdt::CloseObject( )
{
   BOOL result;

   InvokeHelper( 0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

long   ZDrTBEdt::DeleteText( )
{
   long result;

   InvokeHelper( 0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL );
   return result;
}

long   ZDrTBEdt::CutText( )
{
   long result;

   InvokeHelper( 0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL );
   return result;
}

long   ZDrTBEdt::CopyText( )
{
   long result;

   InvokeHelper( 0x5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL );
   return result;
}

long   ZDrTBEdt::PasteText( )
{
   long result;

   InvokeHelper( 0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL );
   return result;
}

void   ZDrTBEdt::SyntaxOff( )
{

   InvokeHelper( 0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
void   ZDrTBEdt::SironSyntaxOn( )
{

   InvokeHelper( 0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
*/
void   ZDrTBEdt::ZeidonSyntaxOn( )
{

   InvokeHelper( 0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

BOOL   ZDrTBEdt::PropertyDialog( )
{
   BOOL result;

   InvokeHelper( 0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ZDrTBEdt::GetCursorPosition( long* lLinie, long* lSpalte, long* lIndex )
{
   static BYTE parms[] =
      VTS_PI4 VTS_PI4 VTS_PI4;
   InvokeHelper( 0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lLinie, lSpalte, lIndex );
}

void   ZDrTBEdt::GetLineCount( long* lZeilen )
{
   static BYTE parms[] =
      VTS_PI4;
   InvokeHelper( 0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lZeilen );
}

long   ZDrTBEdt::InsertItem( BSTR* pDaten )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR;
   InvokeHelper( 0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pDaten );
   return result;
}

void   ZDrTBEdt::IsObjectChanged( BOOL* bChanged )
{
   static BYTE parms[] =
      VTS_PBOOL;
   InvokeHelper( 0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      bChanged );
}

void   ZDrTBEdt::PrintObject( )
{

   InvokeHelper( 0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ZDrTBEdt::ReadOnlyModus( BOOL bReadOnly )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      bReadOnly );
}

BOOL   ZDrTBEdt::SaveObject( )
{
   BOOL result;

   InvokeHelper( 0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::SaveObjectAs( BSTR* pFileName )
{
   BOOL result;
   static BYTE parms[] =
      VTS_PBSTR;
   InvokeHelper( 0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      pFileName );
   return result;
}

BOOL   ZDrTBEdt::SelectItem( long lZeile, BSTR* pDaten )
{
   BOOL result;
   static BYTE parms[] =
      VTS_I4 VTS_PBSTR;
   InvokeHelper( 0x13, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      lZeile, pDaten );
   return result;
}

void   ZDrTBEdt::SetCursorPositionByIndex( long lIndex )
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper( 0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lIndex );
}

void   ZDrTBEdt::SetCursorPositionByLine( long lZeile, long lSpalte )
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper( 0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lZeile, lSpalte );
}

BOOL   ZDrTBEdt::Undo( )
{
   BOOL result;

   InvokeHelper( 0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::Redo( )
{
   BOOL result;

   InvokeHelper( 0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::TempSaveObject( BSTR* pTempName )
{
   BOOL result;
   static BYTE parms[] =
      VTS_PBSTR;
   InvokeHelper( 0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      pTempName );
   return result;
}

long   ZDrTBEdt::FindTextPosition( BSTR* pSearchString, long* lTextPosition, long lOptions )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR VTS_PI4 VTS_I4;
   InvokeHelper( 0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pSearchString, lTextPosition, lOptions );
   return result;
}

long   ZDrTBEdt::GetActualTextLine( BSTR* pBuffer, long lLength )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR VTS_I4;
   InvokeHelper( 0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pBuffer, lLength );
   return result;
}

BOOL   ZDrTBEdt::IsReadOnly( )
{
   BOOL result;

   InvokeHelper( 0x1b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::IsSubWindowActive( )
{
   BOOL result;

   InvokeHelper( 0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ZDrTBEdt::WantKeystrokes( BOOL bWantKeys )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      bWantKeys );
}

BOOL   ZDrTBEdt::DeleteTextRange( long lStartIndex, long lEndIndex )
{
   BOOL result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper( 0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      lStartIndex, lEndIndex );
   return result;
}

BOOL   ZDrTBEdt::CloseSubWindow( )
{
   BOOL result;

   InvokeHelper( 0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ZDrTBEdt::SearchForward( )
{

   InvokeHelper( 0x20, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ZDrTBEdt::SearchBackward( )
{

   InvokeHelper( 0x21, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ZDrTBEdt::Deselect( )
{

   InvokeHelper( 0x22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

long   ZDrTBEdt::GetTextFromLineOfIndex( BSTR* pBuffer, long lLength, long lIndex )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR VTS_I4 VTS_I4;
   InvokeHelper( 0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pBuffer, lLength, lIndex );
   return result;
}

/*
long   ZDrTBEdt::SetSironLanguage( long lLanguage )
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper( 0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lLanguage );
   return result;
}
*/

void   ZDrTBEdt::GetPositionByIndex( long lIndexParam, long* lLine, long* lCol )
{
   static BYTE parms[] =
      VTS_I4 VTS_PI4 VTS_PI4;
   InvokeHelper( 0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lIndexParam, lLine, lCol );
}

BOOL   ZDrTBEdt::IsCommentAtIndex( long lIndex )
{
   BOOL result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper( 0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      lIndex );
   return result;
}

void   ZDrTBEdt::SearchWindow( )
{

   InvokeHelper( 0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ZDrTBEdt::ReplaceWindow( )
{

   InvokeHelper( 0x28, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ZDrTBEdt::SelectAll( )
{

   InvokeHelper( 0x29, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

BOOL   ZDrTBEdt::GotoWindow( )
{
   BOOL result;

   InvokeHelper( 0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::CanCopy( )
{
   BOOL result;

   InvokeHelper( 0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::CanPaste( )
{
   BOOL result;

   InvokeHelper( 0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::CanUndo( )
{
   BOOL result;

   InvokeHelper( 0x2d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::CanRedo( )
{
   BOOL result;

   InvokeHelper( 0x2e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL );
   return result;
}

BOOL   ZDrTBEdt::SelectRange( long lIndex, zLONG lLength )
{
   BOOL result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper( 0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      lIndex, lLength );
   return result;
}

long ZDrTBEdt::GetTextFromRange( BSTR *pBuffer, zLONG lStart, zLONG lEnd )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR VTS_I4 VTS_I4;
   InvokeHelper( 0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pBuffer, lStart, lEnd );
   return result;
}


long ZDrTBEdt::GetObjectName( BSTR *pBuffer )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR;
   InvokeHelper( 0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pBuffer);
   return result;
}

long ZDrTBEdt::GetSelectedText( BSTR *pBuffer, zLONG lLength )
{
   long result;
   static BYTE parms[] =
      VTS_PBSTR VTS_I4;
   InvokeHelper( 0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      pBuffer, lLength );
   return result;
}


/*
Event interface for TBEdit Control
*/
/*------------------------------------------------------
**   Wrapper for invoking methods of dispatch interface
**   _DTBEditEvents
**------------------------------------------------------*/

#if 0
void   _DTBEditEvents::SetInsertMode( )
{

   InvokeHelper( 0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   _DTBEditEvents::RightMouseClick( BSTR* pName )
{
   static BYTE parms[] =
      VTS_PBSTR;
   InvokeHelper( 0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      pName );
}

void   _DTBEditEvents::SetLine( long lZeile, long lSpalte )
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper( 0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lZeile, lSpalte );
}

void   _DTBEditEvents::SetOverwriteMode( )
{

   InvokeHelper( 0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   _DTBEditEvents::Keystroke( short sKey, BOOL* __MIDL_0014 )
{
   static BYTE parms[] =
      VTS_I2 VTS_PBOOL;
   InvokeHelper( 0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      sKey, __MIDL_0014 );
}
#endif
/*
TBEdit Control
*/

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Helper functions
//
BOOL
ZDrTBEdt::OpenObject( LPCSTR lpszFileName )
{
   CString csFileName = lpszFileName;
   BSTR bstrFileName = csFileName.AllocSysString( );

   BOOL bReturn = OpenObject( &bstrFileName );

   ::SysFreeString( bstrFileName );

   return( bReturn );
}

zLONG
ZDrTBEdt::InsertItem( LPCSTR pData )
{
   CString csData = pData;
   BSTR bstrData = csData.AllocSysString( );

   zLONG lReturn = InsertItem( &bstrData );

   ::SysFreeString( bstrData );

   return( lReturn );
}

BOOL
ZDrTBEdt::SaveObjectAs( LPCSTR lpszFileName )
{
   CString csFileName = lpszFileName;
   BSTR bstrFileName = csFileName.AllocSysString( );

   BOOL bReturn = SaveObjectAs( &bstrFileName );

   ::SysFreeString( bstrFileName );

   return( bReturn );
}

BOOL
ZDrTBEdt::SelectItem( zLONG lLine, LPCSTR pData )
{
   CString csData = pData;
   BSTR bstrData = csData.AllocSysString( );

   BOOL bReturn = SelectItem( lLine, &bstrData );

   ::SysFreeString( bstrData );
   return bReturn;
}



BOOL
ZDrTBEdt::TempSaveObject( LPCSTR lpszFileName )
{
   CString csFileName = lpszFileName;
   BSTR bstrFileName = csFileName.AllocSysString( );

   BOOL bReturn = TempSaveObject( &bstrFileName );

   ::SysFreeString( bstrFileName );

   return( bReturn );
}

zLONG
ZDrTBEdt::FindTextPosition( LPCSTR pSearchString,
                            zLONG  *plTextPosition,
                            zLONG  lOptions )
{
   CString csSearchString = pSearchString;
   BSTR bstrSearchString = csSearchString.AllocSysString( );

   zLONG lReturn = FindTextPosition( &bstrSearchString,
                                     plTextPosition, lOptions );

   ::SysFreeString( bstrSearchString );

   return( lReturn );
}

// the buffer must have room for the null terminator ... i.e.  the
// size of the buffer must be lLength + 1.
zLONG
ZDrTBEdt::GetActualTextLine( LPSTR pBuffer, zLONG lLength )
{
   zmemset( pBuffer, ' ', lLength );
   pBuffer[ lLength ] = 0;
   CString csBuffer = pBuffer;
   BSTR bstrBuffer = csBuffer.AllocSysString( );

   zLONG lReturn = GetActualTextLine( &bstrBuffer, lLength );

   csBuffer = bstrBuffer;

   pBuffer[ 0 ] = 0;
   if ( lReturn < lLength )
      strncat_s( pBuffer, lLength, csBuffer, lReturn ); // forces a null terminator

   ::SysFreeString( bstrBuffer );

   return( lReturn );
}


zLONG
ZDrTBEdt::GetTextFromLineOfIndex( LPSTR pBuffer, long lLength, long lLine)
{
   zmemset( pBuffer, ' ', lLength );
   pBuffer[ lLength ] = 0;
   CString csBuffer = pBuffer;
   BSTR bstrBuffer = csBuffer.AllocSysString( );

   zLONG lReturn = GetTextFromLineOfIndex( &bstrBuffer, lLength, lLine);

   csBuffer = bstrBuffer;

   pBuffer[ 0 ] = 0;
   if ( lReturn < lLength )
      strncat_s( pBuffer, lLength, csBuffer, lReturn ); // forces a null terminator

   ::SysFreeString( bstrBuffer );

   return( lReturn );
}

zLONG
ZDrTBEdt::GetTextFromRange( LPSTR pBuffer, long lStart, long lEnd )
{
   zmemset( pBuffer, ' ', lEnd - lStart );
   pBuffer[ lEnd - lStart ] = 0;
   CString csBuffer = pBuffer;
   BSTR bstrBuffer = csBuffer.AllocSysString( );

   zLONG lReturn = GetTextFromRange( &bstrBuffer, lStart, lEnd );

   csBuffer = bstrBuffer;

   pBuffer[ 0 ] = 0;
   if ( lReturn < (lEnd - lStart) )
      strncat_s( pBuffer, lReturn + 1, csBuffer, lReturn ); // forces a null terminator

   ::SysFreeString( bstrBuffer );

   return( lReturn );
}

zLONG
ZDrTBEdt::GetObjectName( LPSTR pBuffer )
{
   CString csBuffer = pBuffer;
   BSTR bstrBuffer = csBuffer.AllocSysString( );

   zLONG lReturn = GetObjectName( &bstrBuffer );

   csBuffer = bstrBuffer;

   pBuffer[ 0 ] = 0;
   if ( lReturn > 0 && lReturn < csBuffer.GetAllocLength() )
      strncat_s( pBuffer, csBuffer.GetAllocLength(), csBuffer, lReturn ); // forces a null terminator

   ::SysFreeString( bstrBuffer );

   return( lReturn );
}


zLONG
ZDrTBEdt::GetSelectedText( LPSTR pBuffer, long lLength )
{
   zmemset( pBuffer, ' ', lLength );
   pBuffer[ lLength ] = 0;
   CString csBuffer = pBuffer;
   BSTR bstrBuffer = csBuffer.AllocSysString( );

   zLONG lReturn = GetSelectedText( &bstrBuffer, lLength );

   csBuffer = bstrBuffer;

   pBuffer[ 0 ] = 0;
   if ( lReturn < lLength )
      strncat_s( pBuffer, lLength, csBuffer, lReturn ); // forces a null terminator

   ::SysFreeString( bstrBuffer );

   return( lReturn );
}


extern "C" zOPER_EXPORT zSHORT OPERATION
TBEditScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
   if ( lRequest == zACTIVEX_INITIALIZE )
   {
      // Default Zeidon Implementation
      // Never change this code unless you want to crash!!!!!
      IDispatch *pIDispatch = pzma->GetDispatchInterface();
      if ( pIDispatch )
      {
         ZDrTBEdt *pWrapperInstance = new ZDrTBEdt( pIDispatch );
         if ( pWrapperInstance )
         {
            pzma->SetWrapperInstance( pWrapperInstance );
            pWrapperInstance->ZeidonSyntaxOn( );
            return( 0 );
         }
      }

      return( -1 );
   }
   else
   if ( lRequest == zACTIVEX_UNINITIALIZE )
   {
      // Default Zeidon Implementation
      // Never change this code unless you want to crash!!!!!
      ZDrTBEdt *pWrapperInstance = (ZDrTBEdt *)pzma->GetWrapperInstance( );
      if ( pWrapperInstance )
      {
         pzma->SetWrapperInstance( NULL );
         delete pWrapperInstance;
         return( 0 );
      }
      return( -1 );
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_FROM_OI )
   {
      // to do:
      // transfer data from the object instance to the control
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_TO_OI )
   {
      // to do:
      // transfer data to the object instance from the control
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )
   {
      // For more complex controls, the determination of the number of
      // attributes may be more difficult than for this simple case.

      // to do:
      // return number of attributes to map here
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )
   {
      // to do:
      // handle receive focus if necessary
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_LOSE_FOCUS )
   {
      // to do:
      // handle lose focus if necessary
      return( 0 );
   }

   return( -1 );
}
