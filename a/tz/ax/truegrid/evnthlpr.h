/********************************************************************************************/
//
// File:            trhlpr.h
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           5. September 2000
// describtion:     helper classes for event handling
//
//
// list of classes :
// Name:             describtion:
// ------------------------------------------------------------------------------------
// CActiveXEvent     holds single activex event with its name and its DISPID
//
//
/* Change log most recent first order:




*/
/*********************************************************************************************/
#include <afx.h>

class CActiveXEvent : public CObject
{
public:
   CActiveXEvent(){};
   CActiveXEvent(CString strName, long lID);

   ~CActiveXEvent(){};
public:
   CString m_strName;
   long m_lID;
};
