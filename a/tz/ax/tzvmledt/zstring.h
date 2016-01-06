/********************************************************************************************/
//
// File:            zstring.h
// Copyright:       TONBELLER AG 2001
// Autor:           TMV
// Date :           30. November 2001
// describtion:
//
//
// list of classes :   ZString a CString extension
// Name:             describtion:
// ------------------------------------------------------------------------------------
//
//
//
/* Change log most recent first order:




*/
/*********************************************************************************************/
#ifndef __ZSTRING_H__
#define __ZSTRING_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ZDCTL_CLASS
#define ZDCTL_CLASS AFX_EXT_CLASS
#endif

class ZDCTL_CLASS ZString : public CString
{
public:
   ZString( );
   ZString( const CString& stringSrc );
   ZString( TCHAR ch, int nRepeat = 1 );
   ZString( LPCTSTR lpch, int nLength );
   ZString( const unsigned char* psz );
   ZString( LPCWSTR lpsz );
   ZString( LPCSTR lpsz );
   ~ZString(){};

   int Tokenize( LPCTSTR lpszDelimiter, CStringArray &sar );
};


#endif //__ZSTRING_H__
