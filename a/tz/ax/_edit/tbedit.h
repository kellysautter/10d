/*
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**----------------------------------------------------------------------
*/

#if !defined(AFX_TBEDIT_H__DEA843DB_DAA2_11D1_95B2_0060973B3B55__INCLUDED_)
#define AFX_TBEDIT_H__DEA843DB_DAA2_11D1_95B2_0060973B3B55__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if !defined( __AFXCTL_H__ )
   #error include 'afxctl.h' before including this file
#endif

#include "hzoectlr.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CTBEditApp : See TBEdit.cpp for implementation.

class CTBEditApp : public COleControlModule
{
public:
   BOOL InitInstance();
   int ExitInstance();
   virtual BOOL PreTranslateMessage( MSG* pMsg );
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBEDIT_H__DEA843DB_DAA2_11D1_95B2_0060973B3B55__INCLUDED)
