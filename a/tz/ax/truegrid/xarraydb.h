/********************************************************************************************/
//
// File:            xarraydb.h
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           15. September 2000
// describtion:     Machine generated IDispatch wrapper class created with ClassWizard
//
//
// list of classes :
// Name:             describtion:
// ------------------------------------------------------------------------------------
// IXArrayDB         wrapper class
//
//
/* Change log most recent first order:

   2001.7.6   TMV   2000   ActiveX


*/
/*********************************************************************************************/

#ifndef __XARRAYDB_H__
#define __XARRAYDB_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// IXArrayDB wrapper class

class IXArrayDB : public COleDispatchDriver
{
public:
   IXArrayDB() {}      // Calls COleDispatchDriver default constructor
   IXArrayDB(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   IXArrayDB(const IXArrayDB& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
// Operations
public:
   HRESULT CreateInstanceLic( REFCLSID clsid, LPUNKNOWN pUnkOuter,
                     DWORD dwClsCtx, REFIID iid, LPVOID* ppv );
   BOOL CreateDispatch(REFCLSID clsid, COleException* pError = NULL );
   double GetPrecision();
   void SetPrecision(double newValue);
   BOOL GetNotify();
   void SetNotify(BOOL bNewValue);
   VARIANT GetValue(long iRow, long iColumn);
   void SetValue(long iRow, long iColumn, const VARIANT& newValue);
   void Clear();
   void ReDim(long rowLB, long rowUB, long columnLB, long columnUB);
   void Set(long iRow, long iColumn, const VARIANT& newVal);
   void Get(long iRow, long iColumn, VARIANT* pVal);
   long InsertColumns(long iColumn, long cColumns);
   long DeleteColumns(long iColumn, long cColumns);
   long AppendColumns(long cColumns);
   long InsertRows(long iRow, long cRows);
   long DeleteRows(long iRow, long cRows);
   long AppendRows(long cRows);
   long Find(long iRowStart, long iColumn, const VARIANT& varSval, long direct, long compType, long varType);
   // method 'QuickSort' not emitted because of invalid return type or parameter type
   long GetCount(short nDim);
   long GetLowerBound(short nDim);
   long GetUpperBound(short nDim);
   void Insert(short nDim, long index);
   void Delete(short nDim, long index);
   long GetDefaultColumnType(long iColumn);
   void SetDefaultColumnType(long iColumn, long nNewValue);
   void AboutBox();
   void LoadRows(const VARIANT& array, BOOL transpose);

// specific implementation
   CLSID GetCLSID()
   {
      CString strCLSID = "{0D62353B-DBA2-11D1-B5DF-0060976089D0}";
      LPOLESTR lpoleCLSID;
      CLSID clsid;

      lpoleCLSID = strCLSID.AllocSysString();

      CLSIDFromString( lpoleCLSID, &clsid );
      return clsid;
   }

};

#endif //__XARRAYDB_H__
