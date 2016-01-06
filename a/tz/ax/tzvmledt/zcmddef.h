/********************************************************************************************/
//
// File:            zcmddef.h
// *******************************************************************
// * Copyright © 2002 QuinSoft Corporation and TONBELLER AG.         *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           20. März 2002
// describtion:
//
// list of classes :
// Name:             describtion:
// ------------------------------------------------------------------------------------
//
//
//
/* Change log most recent first order:




*/
/*********************************************************************************************/
#ifndef __ZCMDDEF_H__
#define __ZCMDDEF_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ZVMLCommand;

typedef ZVMLCommand      * LPZVMLCOMMAND;

// use of class CMap to create a set of somewhat
// CMap has implemented an hash-allgorithm that should gain performance

typedef CMapStringToOb ZVMLCommandSet;

class ZVMLCommand : public CObject{
public:
   ZVMLCommand();

   ZVMLCommand( zCPCHAR strText,
                zCPCHAR strType,
                zCPCHAR strInsertText,
                zCPCHAR strInsertTextFull,
                zCPCHAR strCC_Code,
                zBOOL   bCC_FirstChar );
   ~ZVMLCommand();
   CString m_strText;
   CString m_strInsertText;
   CString m_strInsertTextFull;
   CString m_strCC_Code;
   CString m_strType;
   BOOL    m_bCC_FirstChar;   //command must be at the beginning of the line

#ifdef _DEBUG
   void AssertValid()
   {
      CObject::AssertValid();
   }

   void Dump(CDumpContext& dc)
   {
      CObject::Dump(dc);
   }
#endif //_DEBUG
};

#endif //__ZCMDDEF_H__
