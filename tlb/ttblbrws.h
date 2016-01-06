// ttblbrws.h : main header file for the TTBLBRWS application
//

#if !defined(AFX_TTBLBRWS_H__DBF8E2B7_D176_11D2_9C83_00608C72BD76__INCLUDED_)
#define AFX_TTBLBRWS_H__DBF8E2B7_D176_11D2_9C83_00608C72BD76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"      // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTtblbrwsApp:
// See ttblbrws.cpp for the implementation of this class
//

class CTtblbrwsApp : public CWinApp
{
public:
   CTtblbrwsApp();

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTtblbrwsApp)
   public:
   virtual BOOL InitInstance();
   //}}AFX_VIRTUAL

// Implementation

   //{{AFX_MSG(CTtblbrwsApp)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTBLBRWS_H__DBF8E2B7_D176_11D2_9C83_00608C72BD76__INCLUDED_)
