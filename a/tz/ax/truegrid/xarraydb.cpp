/********************************************************************************************/
//
// File:            xarraydb.cpp
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           15. September 2000
// describtion:     Machine generated IDispatch wrapper class(es) created with ClassWizard
//
//
/* Change log most recent first order:




*/
/*********************************************************************************************/

#include "zstdafx.h"
#include "tdbg6.h"
#include "xarraydb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static WCHAR LKey[] =
{
   0x0038,   0x0045,   0x0043,   0x0035,   0x0033,   0x0031,
   0x0041,   0x0031,   0x002D,   0x0035,   0x0038,   0x0038,
   0x0030,   0x002D,   0x0031,   0x0031,   0x0064,   0x0032,
   0x002D,   0x0041,   0x0039,   0x0034,   0x0046,   0x002D,
   0x0030,   0x0030,   0x0036,   0x0030,   0x0030,   0x0038,
   0x0043,   0x0030,   0x0045,   0x0031,   0x0046,   0x0045
};

/////////////////////////////////////////////////////////////////////////////
// IXArrayDB properties

/////////////////////////////////////////////////////////////////////////////
// IXArrayDB operations

HRESULT IXArrayDB::CreateInstanceLic( REFCLSID clsid, LPUNKNOWN pUnkOuter,
   DWORD dwClsCtx, REFIID iid, LPVOID* ppv )
{
   HRESULT hr;

   LPCLASSFACTORY2 pClassFactory = NULL;
   BSTR bstrLKey = ::SysAllocStringLen( LKey, sizeof(LKey)/sizeof(WCHAR));

   if (SUCCEEDED(hr = CoGetClassObject(clsid, dwClsCtx, NULL,
      IID_IClassFactory2, (void**)&pClassFactory)))
   {
      ASSERT(pClassFactory != NULL);
      hr = pClassFactory->CreateInstanceLic( pUnkOuter, NULL, iid,
         bstrLKey , ppv);
      pClassFactory->Release();
   }

   ::SysFreeString( bstrLKey );
   return hr;
}


BOOL IXArrayDB::CreateDispatch(REFCLSID clsid, COleException* pError )
{
   ASSERT(m_lpDispatch == NULL);

   m_bAutoRelease = TRUE;  // good default is to auto-release

   // create an instance of the object
   LPUNKNOWN lpUnknown = NULL;
   HRESULT sc = CreateInstanceLic( this->GetCLSID(), NULL,
                                   CLSCTX_ALL | CLSCTX_REMOTE_SERVER,
                                   IID_IUnknown, (zPVOID*)&lpUnknown);
   if (sc == E_INVALIDARG)
   {
      // may not support CLSCTX_REMOTE_SERVER, so try without
      sc = CreateInstanceLic(this->GetCLSID(), NULL, CLSCTX_ALL & ~CLSCTX_REMOTE_SERVER,
         IID_IUnknown, (zPVOID*)&lpUnknown);
   }
   if (FAILED(sc))
      goto Failed;

   // make sure it is running
   sc = OleRun(lpUnknown);
   if (FAILED(sc))
      goto Failed;

   // query for IDispatch interface
   sc = lpUnknown->QueryInterface( IID_IDispatch, (zPVOID*) &m_lpDispatch);
   if (m_lpDispatch == NULL)
      goto Failed;

   lpUnknown->Release();
   ASSERT(m_lpDispatch != NULL);
   return TRUE;

Failed:
   lpUnknown->Release();
   if (pError != NULL)
      pError->m_sc = sc;
   TRACE1("Warning: CreateDispatch returning scode = %s.\n",
      AfxGetFullScodeString(sc));
   return FALSE;
}


double IXArrayDB::GetPrecision()
{
   double result;
   InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void IXArrayDB::SetPrecision(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

BOOL IXArrayDB::GetNotify()
{
   BOOL result;
   InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void IXArrayDB::SetNotify(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

VARIANT IXArrayDB::GetValue(long iRow, long iColumn)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      iRow, iColumn);
   return result;
}

void IXArrayDB::SetValue(long iRow, long iColumn, const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       iRow, iColumn, &newValue);
}

void IXArrayDB::Clear()
{
   InvokeHelper(0x6b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IXArrayDB::ReDim(long rowLB, long rowUB, long columnLB, long columnUB)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       rowLB, rowUB, columnLB, columnUB);
}

void IXArrayDB::Set(long iRow, long iColumn, const VARIANT& newVal)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_VARIANT;
   InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       iRow, iColumn, &newVal);
}

void IXArrayDB::Get(long iRow, long iColumn, VARIANT* pVal)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_PVARIANT;
   InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       iRow, iColumn, pVal);
}

long IXArrayDB::InsertColumns(long iColumn, long cColumns)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x6f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iColumn, cColumns);
   return result;
}

long IXArrayDB::DeleteColumns(long iColumn, long cColumns)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x70, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iColumn, cColumns);
   return result;
}

long IXArrayDB::AppendColumns(long cColumns)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x71, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      cColumns);
   return result;
}

long IXArrayDB::InsertRows(long iRow, long cRows)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x72, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iRow, cRows);
   return result;
}

long IXArrayDB::DeleteRows(long iRow, long cRows)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x73, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iRow, cRows);
   return result;
}

long IXArrayDB::AppendRows(long cRows)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x74, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      cRows);
   return result;
}

long IXArrayDB::Find(long iRowStart, long iColumn, const VARIANT& varSval, long direct, long compType, long varType)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x77, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iRowStart, iColumn, &varSval, direct, compType, varType);
   return result;
}

long IXArrayDB::GetCount(short nDim)
{
   long result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
      nDim);
   return result;
}

long IXArrayDB::GetLowerBound(short nDim)
{
   long result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
      nDim);
   return result;
}

long IXArrayDB::GetUpperBound(short nDim)
{
   long result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
      nDim);
   return result;
}

void IXArrayDB::Insert(short nDim, long index)
{
   static BYTE parms[] =
      VTS_I2 VTS_I4;
   InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       nDim, index);
}

void IXArrayDB::Delete(short nDim, long index)
{
   static BYTE parms[] =
      VTS_I2 VTS_I4;
   InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       nDim, index);
}

long IXArrayDB::GetDefaultColumnType(long iColumn)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
      iColumn);
   return result;
}

void IXArrayDB::SetDefaultColumnType(long iColumn, long nNewValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       iColumn, nNewValue);
}

void IXArrayDB::AboutBox()
{
   InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IXArrayDB::LoadRows(const VARIANT& array, BOOL transpose)
{
   static BYTE parms[] =
      VTS_VARIANT VTS_BOOL;
   InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &array, transpose);
}
