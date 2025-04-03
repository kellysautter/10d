// Z2MP.h : main header file for the Z2MP interface
//

#if !defined(AFX_Z2MP_H__9FF5BDDD_D189_4BCC_B377_435DD5B2EE46__INCLUDED_)
#define AFX_Z2MP_H__9FF5BDDD_D189_4BCC_B377_435DD5B2EE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"      // main symbols

/////////////////////////////////////////////////////////////////////////////
// ZMSWord2kApp
// See Z2MP.cpp for the implementation of this class
//

class ZMapPointApp : public CWinApp
{
public:
   ZMapPointApp();
   ~ZMapPointApp();

   _Application * GetMapPointApplication();
   SHORT GetMapPointState();
   void SetHoldState( int bHold );
   void QuitMapPointApplication();
   void QuitMapPointApplication( int bHold );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZMapPointApp)
   public:
   virtual BOOL InitInstance();
   virtual void OnAppExit( );

   //}}AFX_VIRTUAL

   //{{AFX_MSG(ZMapPointApp)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
private:
public:
   _Application* m_pMapPointApp;
private:
   int m_nFailedOLE;
   int m_nMapPointOnHold;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Z2MP_H__9FF5BDDD_D189_4BCC_B377_435DD5B2EE46__INCLUDED_)
