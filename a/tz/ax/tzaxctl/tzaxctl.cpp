/////////////////////////////////////////////////////////////////////////////
// Project TzAxCtl
//
// This is a part of the Zeidon ActiveX controls package.
// Copyright © 1995 - 2000 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    TzAxCtl.dll - ActiveX Control implementations
// FILE:         TzAxCtl.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon DR Controls.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//

#include "zstdafx.h"

#define zGLOBAL_DATA
#include "Zdr.h"
#include "TzAxGbl.h"

#include "TzAxCtl.h"
#include "TzAxCal.h"
#include "TzAxDate.h"

#define DEBUG_ALL

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialization of MFC Extension DLL

#include "afxdllx.h"    // standard MFC Extension DLL routines

static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };

extern "C" int APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
   // Remove this if you use lpReserved
   UNREFERENCED_PARAMETER( lpReserved );

   if ( dwReason == DLL_PROCESS_ATTACH )
   {
      TRACE0( "TzAxCtl.DLL Initializing!\n" );
      g_hInstanceDLL = hInstance;
//    TraceLineX( "TzAxCtl DllMain hInst = ", (zLONG) hInstance );

//??? RegisterNullWindow( hInstance );

      // Extension DLL one-time initialization - do not allocate memory here,
      // use the TRACE or ASSERT macros or call MessageBox
      if ( AfxInitExtensionModule( extensionDLL, hInstance ) == 0 )
         return( 0 );

      // Other initialization could be done here, as long as
      // it doesn't result in direct or indirect calls to AfxGetApp.
      // This extension DLL doesn't need to access the app object
      // but to be consistent with ZDrApp.dll, this DLL requires
      // explicit initialization as well (see below).

      // This allows for greater flexibility later in development.

      ///////////////////////////////////////////////////////////////////////
      //
      // We are calling this DLL from regular DLL's, so we have moved
      // the call to CDynLinkLibrary .
      //
      // Insert this DLL into the resource chain
      // NOTE: If this Extension DLL is being implicitly linked to by
      // an MFC Regular DLL (such as an ActiveX Control) instead of an
      // MFC application, then you will want to remove this line from
      // DllMain and put it in a separate function exported from this
      // Extension DLL.  The Regular DLL that uses this Extension DLL
      // should then explicitly call that function to initialize this
      // Extension DLL.  Otherwise, the CDynLinkLibrary object will not
      // be attached to the Regular DLL's resource chain, and serious
      // problems will result.
      //
      ///////////////////////////////////////////////////////////////////////

      // new CDynLinkLibrary( extensionDLL );
   }
   else
   if ( dwReason == DLL_PROCESS_DETACH )
   {
      TRACE0( "TzAxCtl.DLL Terminating!\n" );
//??? UnregisterNullWindow( hInstance );
      AfxTermExtensionModule( extensionDLL );
   }

   return( 1 );   // ok
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
ZeidonTZAXCTL_WndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
//   switch( uMessage )
//   {
//      case WM_LBUTTONDBLCLK:
//#ifdef DEBUG_ALL
//         TraceLineS( "ZeidonHD LBUTTONDBLCLK", "" );
//#endif
//         break;
//   }

   return( ::DefWindowProc( hWnd, uMessage, wParam, lParam ) );
}

/////////////////////////////////////////////////////////////////////////////
// Exported DLL initialization is run in context of running application
//
extern "C" void WINAPI
ZeidonInitTZAXCTL( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                   zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
   // Create a new CDynLinkLibrary for this app
   new CDynLinkLibrary( extensionDLL );

   // Initialize globals
   g_lDlgUnitCharX = lDlgUnitCharX;
   g_lDlgUnitCharY = lDlgUnitCharY;
   g_lMapDlgCharX = lMapDlgCharX;
   g_lMapDlgCharY = lMapDlgCharY;

// TraceLineI( "DlgUnitCharX = ", g_lDlgUnitCharX );     // e.g.  10
// TraceLineI( "DlgUnitCharY = ", g_lDlgUnitCharY );     // e.g.  20

   CWindowDC dc( CWnd::GetDesktopWindow( ) );
   CPoint pt( 1000, 1000 );   // a HIENGLISH point is .001 inches

   // change the mapping mode to high english
   int nOldMapMode = dc.SetMapMode( MM_HIENGLISH );

   // convert the client rect sizes to high english
   dc.LPtoDP( &pt );

   g_lPixelsPerInchX = (zSHORT) (pt.x < 0 ? -pt.x : pt.x);
   g_lPixelsPerInchY = (zSHORT) (pt.y < 0 ? -pt.y : pt.y);

// TraceLineI( "PixelsPerInchX = ", g_lPixelsPerInchX );
// TraceLineI( "PixelsPerInchY = ", g_lPixelsPerInchY );

   // reset the mapping mode
   dc.SetMapMode( nOldMapMode );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
