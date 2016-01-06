/********************************************************************************************/
//
// File:            trhlpr.cpp
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           5. September 2000
// describtion:     Helper classes for event tracing
//
//
/* Change log most recent first order:

   2001.31.01  TMV
      adding MFC macros for debug purpose
   2000.09.05  TMV
      created


*/
/*********************************************************************************************/
#include "zstdafx.h"
#include "tdbg6.h"
#include "trhlpr.h"
//#include

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CActiveXEvent::CActiveXEvent(CString strName, long lID)
{
   m_strName = strName;
   m_lID = lID;
}

