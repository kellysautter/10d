// MonProc.h : main header file for the MonProc application
//

#if !defined( AFX_MonProc_H__3F3A24B5_FFCD_11D1_A30F_0000B481E689__INCLUDED_ )
#define AFX_MonProc_H__3F3A24B5_FFCD_11D1_A30F_0000B481E689__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"      // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMonProcApp:
// See MonProc.cpp for the implementation of this class
//

class CMonProcApp : public CWinApp
{
public:
   CMonProcApp( );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CMonProcApp )
   public:
   virtual BOOL InitInstance( );
   //}}AFX_VIRTUAL

// Implementation

   //{{AFX_MSG( CMonProcApp )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined( AFX_MonProc_H__3F3A24B5_FFCD_11D1_A30F_0000B481E689__INCLUDED_ )
