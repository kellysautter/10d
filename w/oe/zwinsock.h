// zWinSock.h : main header file for the ZWINSOCK DLL
//

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxsock.h>        // MFC socket extensions

/////////////////////////////////////////////////////////////////////////////
// CZWinSockApp
// See zWinSock.cpp for the implementation of this class
//

class CZWinSockApp : public CWinApp
{
public:
        CZWinSockApp();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CZWinSockApp)
        public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

        //{{AFX_MSG(CZWinSockApp)
                // NOTE - the ClassWizard will add and remove member functions here.
                //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
