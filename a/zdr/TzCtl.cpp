/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctl.cpp
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
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2000.01.04    TMV   Z10
//    Avoid multiple error messages when browsing typelibraries fails.
//

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "TzCtl.h"

#define zGLOBAL_DATA
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
   // TRACE0( "TZCTL.DLL Initializing!\n" );
      g_hInstanceDLL = hInstance;

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
   // TRACE0( "TZCTL.DLL Terminating!\n" );
      AfxTermExtensionModule( extensionDLL );
   }

   return( 1 );   // ok
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
ZeidonTZCTL_WndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
//   switch ( uMessage )
//   {
//      case WM_LBUTTONDBLCLK:
// #ifdef DEBUG_ALL
//         TraceLineS( "ZeidonHD LBUTTONDBLCLK", "" );
// #endif
//         break;
//   }

   return( ::DefWindowProc( hWnd, uMessage, wParam, lParam ) );
}

/////////////////////////////////////////////////////////////////////////////
// Exported DLL initialization is run in context of running application
//
extern "C" void WINAPI
ZeidonInitTZCTL( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                 zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
   // Create a new CDynLinkLibrary for this app.
// new CDynLinkLibrary( extensionDLL );

   // Initialize globals.
   g_lDlgUnitCharX = lDlgUnitCharX;
   g_lDlgUnitCharY = lDlgUnitCharY;
   g_lMapDlgCharX = lMapDlgCharX;
   g_lMapDlgCharY = lMapDlgCharY;
   g_lDblClkX = GetSystemMetrics( SM_CXDOUBLECLK );
   g_lDblClkY = GetSystemMetrics( SM_CYDOUBLECLK );

// TraceLineI( "DlgUnitCharX = ", g_lDlgUnitCharX );     // e.g.  10
// TraceLineI( "DlgUnitCharY = ", g_lDlgUnitCharY );     // e.g.  20

#if 0
   CWindowDC dc( CWnd::GetDesktopWindow( ) );
   CPoint pt( 1000, 1000 );   // a HIENGLISH point is .001 inches

   // Change the mapping mode to high english.
   int nOldMapMode = dc.SetMapMode( MM_HIENGLISH );

   // Convert the client rect sizes to high english.
   zLONG lMod;

   dc.LPtoDP( &pt );
   g_lPixelsPerInchX = (pt.x < 0) ? -pt.x : pt.x;
   g_lPixelsPerInchY = (pt.y < 0) ? -pt.y : pt.y;
   lMod = g_lPixelsPerInchX % 16;
   if ( lMod >= 8 )
   {
      lMod = 16 - lMod;
      g_lPixelsPerInchX += lMod;
   }
   else
      g_lPixelsPerInchX -= lMod;

   lMod = g_lPixelsPerInchY % 16;
   if ( lMod >= 8 )
   {
      lMod = 16 - lMod;
      g_lPixelsPerInchY += lMod;
   }
   else
      g_lPixelsPerInchY -= lMod;

// TraceLineI( "PixelsPerInchX = ", g_lPixelsPerInchX );
// TraceLineI( "PixelsPerInchY = ", g_lPixelsPerInchY );

   // Reset the mapping mode.
   dc.SetMapMode( nOldMapMode );
#else

   g_lPixelsPerInchX = afxData.cxPixelsPerInch;
   g_lPixelsPerInchY = afxData.cyPixelsPerInch;

#endif

   WNDCLASS  wc;

   wc.style         = CS_GLOBALCLASS | CS_DBLCLKS;
   wc.lpfnWndProc   = ZeidonTZCTL_WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = AfxGetInstanceHandle( );
   wc.hIcon         = 0;
   wc.hCursor       = ::LoadCursor( 0, IDC_ARROW );
   wc.hbrBackground = (HBRUSH) ::GetStockObject( LTGRAY_BRUSH );
   wc.lpszMenuName  = 0;
   wc.lpszClassName = "ZeidonDevToolsCtrl";

   if ( AfxRegisterClass( &wc ) == 0 )
   {
      TraceLineS( "Unable to register window class: ", wc.lpszClassName );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


zOPER_EXPORT zLONG OPERATION
BuildTZPEAXWO( zVIEW vTZPEAXWO, zCPCHAR cpcGUID, zBOOL bEvents )
{
   if ( MiValidView( vTZPEAXWO ) == FALSE )
      return( 0 );

   ZActiveX_CtrlList *pAX_CtrlList;
   ZActiveX_Ctrl *pAX_Ctrl;
   ZFunction     *pAX_Event;
   ZTypeLibrary  tlbCtrl; // type lib of current control
   zCPCHAR       cpcClassId;
   zLONG         lErrorCount = 0;

   POSITION posAX_Ctrl;
   POSITION posAX_Event;
   zLONG    lEntityCnt = 0;

   pAX_CtrlList = new ZActiveX_CtrlList( vTZPEAXWO );
   pAX_CtrlList->PopulateZeidonUnregisteredActiveX( );

   // Clear out any previous stuff;
   if ( cpcGUID == 0 || *cpcGUID == 0 )
   {
      cpcGUID = 0;
      while ( CheckExistenceOfEntity( vTZPEAXWO, "TZPEAXWO" ) == 0 )
         DeleteEntity( vTZPEAXWO, "TZPEAXWO", zREPOS_NEXT );

      CreateEntity( vTZPEAXWO, "TZPEAXWO", zREPOS_AFTER );
   }
   else
   {
      if ( CheckExistenceOfEntity( vTZPEAXWO, "TZPEAXWO" ) != 0 )
         CreateEntity( vTZPEAXWO, "TZPEAXWO", zREPOS_AFTER );

      if ( CheckExistenceOfEntity( vTZPEAXWO, "ActiveX" ) != 0 ||
           SetCursorFirstEntityByString( vTZPEAXWO, "ActiveX", "ClassId",
                                         cpcGUID, 0 ) < zCURSOR_SET )
      {
         lEntityCnt++;
         CreateEntity( vTZPEAXWO, "ActiveX", zREPOS_AFTER );
         SetAttributeFromString( vTZPEAXWO, "ActiveX", "ClassId", cpcGUID );
      }
   }

   // Step through the list of ActiveX controls and build the OI.
   posAX_Ctrl = pAX_CtrlList->GetHeadPosition( );
   while ( posAX_Ctrl )
   {
      pAX_Ctrl = (ZActiveX_Ctrl *) pAX_CtrlList->GetAt( posAX_Ctrl );
      if ( pAX_Ctrl )
      {
         cpcClassId = pAX_Ctrl->GetClassId( );
         if ( cpcGUID == 0 || zstrcmp( cpcGUID, cpcClassId ) == 0 )
         {
            if ( cpcGUID )
               SetCursorFirstEntityByString( vTZPEAXWO, "ActiveX",
                                             "ClassId", cpcGUID, 0 );
            else
            {
               lEntityCnt++;
               CreateEntity( vTZPEAXWO, "ActiveX", zREPOS_AFTER );
               SetAttributeFromString( vTZPEAXWO, "ActiveX", "ClassId",
                                       pAX_Ctrl->GetClassId( ) );
            }

            SetAttributeFromString( vTZPEAXWO, "ActiveX", "ProgId",
                                    pAX_Ctrl->GetProgId( ) );
            SetAttributeFromString( vTZPEAXWO, "ActiveX", "TypeLib",
                                    pAX_Ctrl->GetTypeLib( ) );
            SetAttributeFromString( vTZPEAXWO, "ActiveX", "Description",
                                    pAX_Ctrl->GetDescription( ) );
            SetAttributeFromString( vTZPEAXWO, "ActiveX", "Bitmap",
                                    pAX_Ctrl->GetToolbarBitmapName( ) );

// for testing purposes
//          CString csDescription = pAX_Ctrl->GetDescription( ) ;
//          if ( csDescription == "Calendar Control 8.0" )
//             int k = 1;

            // Load the specific type lib and detect Eventinterface
            if ( !tlbCtrl.LoadTypeLibrary( pAX_Ctrl->GetTypeLib( ),
                                           pAX_Ctrl->GetClassId( ) ) )
            {
               CString csMsg;

               lErrorCount++;
               csMsg.Format( "Unable to browse Typelibrary!\nClassId\t: "
                              "%s\nName\t: %s\n"
                              "check existence of typelibrary \"%s\"",
                              (LPCSTR)pAX_Ctrl->GetClassId( ),
                              pAX_Ctrl->GetDescription( ),
                              pAX_Ctrl->GetTypeLib( ) );
               TraceLineS( "Typelibrary: ", csMsg );
            // AfxMessageBox( csMsg );
            }

            if ( bEvents )
            {
               ZDispinterface *pEventInterface =
                        tlbCtrl.GetEventInterface( pAX_Ctrl->GetClassId( ) );
               if ( pEventInterface )
               {
                  posAX_Event =
                           pEventInterface->GetMethods()->GetHeadPosition( );
                  while ( posAX_Event )
                  {
                     pAX_Event = (ZFunction *)
                       pEventInterface->GetMethods()->GetNext( posAX_Event );
                     CreateEntity( vTZPEAXWO, "Event", zREPOS_AFTER );
                     SetAttributeFromString( vTZPEAXWO, "Event", "Name",
                                             pAX_Event->GetName( ) );
                     SetAttributeFromInteger( vTZPEAXWO, "Event",
                                              "DispatchId",
                                              pAX_Event->GetMemberID( ) );
                  }

                  //mDeleteInit( pAX_EventList );
               }
            }
         }
      }

      pAX_CtrlList->GetNext( posAX_Ctrl );
   }

   mDeleteInit( pAX_CtrlList );

   if ( lErrorCount > 0 )
   {
      CString csMsg = "Errors occurred while browsing your PC's registry.\n"
                      "See the OE Trace window for details!";
      AfxMessageBox( csMsg );
   }

   return( lEntityCnt );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
