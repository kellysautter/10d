/********************************************************************************************/
//
// File:            zviewdef.h
// *******************************************************************
// * Copyright © 2001 QuinSoft Corporation and TONBELLER AG.         *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           11. Dezember 2001
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
#ifndef __ZVIEWDEF_H__
#define __ZVIEWDEF_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxcoll.h>

class ZLod;
class ZEntity;
class ZAttribute;

typedef ZLod             * LPZLOD;
typedef ZEntity          * LPZENTITY;
typedef ZAttribute       * LPZATTRIBUTE;

// use of class CMap to create a set of somewhat
// CMap has implemented an hash-allgorithm that should gain performance

typedef CMapStringToOb ZLodSet;
typedef CMapStringToOb ZInstanceSet;
typedef CMapStringToOb ZEntitySet;
typedef CMapStringToOb ZAttributeSet;

class ZLod : public CObject{
public:
   ZLod( LPCSTR lpszName = NULL, LPCSTR lpcstrDescribtion = NULL );
   ~ZLod();
   CString m_strName;
   CString m_strDescribtion;
   ZEntitySet m_mapEntities;

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



class ZEntity : public CObject{
public:
   ZEntity( LPCSTR lpszName = NULL, LPCSTR lpcstrDescribtion = NULL );
   ~ZEntity( );
   CString m_strName;
   CString m_strDescribtion;
   ZAttributeSet m_mapAttributes;

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



class ZAttribute : public CObject{
public:
   ZAttribute(LPCSTR lpszName = NULL, LPCSTR lpcstrDescribtion = NULL);
   ~ZAttribute();
   CString m_strName;
   CString m_strDescribtion;

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

#endif //__ZVIEWDEF_H__
