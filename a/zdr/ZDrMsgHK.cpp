/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrmsghk.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of the Message Hooking class.
// The code is derived from code written by Paul DiLascia.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// The message hook map is derived from CMapPtrToPtr, which associates
// a pointer with another pointer. It maps an HWND to a ZMsgHook, like
// the way MFC's internal maps map HWND's to CWnd's. The first hook
// attached to a window is stored in the map; all other hooks for that
// window are then chained via ZMsgHook::m_pNext.
//
class ZMsgHookMap : private CMapPtrToPtr
{
public:
   ZMsgHookMap( );
   ~ZMsgHookMap( );
   static ZMsgHookMap& GetHookMap( );
   void Add( HWND hwnd, ZMsgHook *pMsgHook );
   void Remove( ZMsgHook *pMsgHook );
   void RemoveAll( HWND hwnd );
   ZMsgHook *Lookup( HWND hwnd );
};

// This trick is used so the hook map isn't
// instantiated until someone actually requests it.
//
#define  theHookMap  ( ZMsgHookMap::GetHookMap( ) )

IMPLEMENT_DYNAMIC( ZMsgHook, CWnd );

ZMsgHook::ZMsgHook( )
{
   m_pNext = 0;
   m_pOldWndProc = 0;
   m_pWndHooked  = 0;
}

ZMsgHook::~ZMsgHook( )
{
   ASSERT( m_pWndHooked == 0 );    // can't destroy while still hooked!
   ASSERT( m_pOldWndProc == 0 );
}

/////////////////////////////////////////////////////////////////////////////
// Hook a window.
// This installs a new window proc that directs messages to the ZMsgHook.
// pWnd=0 to remove.
//
zBOOL
ZMsgHook::HookWindow( CWnd *pWnd )
{
   if ( pWnd )
   {
      // Hook the window
      ASSERT( m_pWndHooked==0 );
   // TRACE( "%s::HookWindow( %s )\n",
   //        GetRuntimeClass( )->m_lpszClassName, DbgName( pWnd ) );
      HWND hwnd = pWnd->m_hWnd;
      ASSERT( hwnd && mIs_hWnd( hwnd ) );
      theHookMap.Add( hwnd, this );       // Add to map of hooks

   }
   else
   {
      // Unhook the window
      ASSERT( m_pWndHooked!=0 );
   // TRACE( "%s::HookWindow( 0 ) [unhook 0x%04x]\n",
   //        GetRuntimeClass( )->m_lpszClassName,
   //        m_pWndHooked->GetSafeHwnd( ) );
      theHookMap.Remove( this );          // Remove from map
      m_pOldWndProc = 0;
   }

   m_pWndHooked = pWnd;
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// Window proc-like virtual function which specific ZMsgHooks will
// override to do stuff. Default passes the message to the next hook;
// the last hook passes the message to the original window.
// You MUST call this at the end of your WindowProc if you want the real
// window to get the message. This is just like CWnd::WindowProc, except that
// a ZMsgHook is not a window.
//
LRESULT
ZMsgHook::WindowProc( UINT msg, WPARAM wp, LPARAM lp )
{
   ASSERT( m_pOldWndProc );
   return( m_pNext ?
              m_pNext->WindowProc( msg, wp, lp ) :
              ::CallWindowProc( m_pOldWndProc,
                                m_pWndHooked->m_hWnd, msg, wp, lp ) );
}

/////////////////////////////////////////////////////////////////////////////
// Like calling base class WindowProc, but with no args, so individual
// message handlers can do the default thing. Like CWnd::Default
//
LRESULT
ZMsgHook::Default( )
{
   // MFC stores current MSG in thread state
   MSG& curMsg = AfxGetThreadState( )->m_lastSentMsg;

   // Note: must explicitly call ZMsgHook::WindowProc to avoid infinte
   // recursion on virtual function
   return( ZMsgHook::WindowProc( curMsg.message,
                                 curMsg.wParam, curMsg.lParam ) );
}

/////////////////////////////////////////////////////////////////////////////
// Subclassed window proc for message hooks. Replaces AfxWndProc ( or whatever
// else was there before. )
//
LRESULT CALLBACK
HookWndProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
#ifdef _USRDLL
   // If this is a DLL, need to set up MFC state
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
#endif

   // Set up MFC message state just in case anyone wants it
   // This is just like AfxCallWindowProc, but we can't use that because
   // a ZMsgHook is not a CWnd.
   //
   MSG& curMsg = AfxGetThreadState( )->m_lastSentMsg;
   MSG  oldMsg = curMsg;   // save for nesting
   curMsg.hwnd    = hwnd;
   curMsg.message = msg;
   curMsg.wParam  = wp;
   curMsg.lParam  = lp;

   // Get hook object for this window. Get from hook map
   ZMsgHook *pMsgHook = theHookMap.Lookup( hwnd );
   ASSERT( pMsgHook );

   LRESULT lr;
   if ( msg==WM_NCDESTROY )
   {
      // Window is being destroyed: unhook all hooks (for this window)
      // and pass msg to orginal window proc
      //
      WNDPROC wndproc = pMsgHook->m_pOldWndProc;
      theHookMap.RemoveAll( hwnd );
      lr = ::CallWindowProc( wndproc, hwnd, msg, wp, lp );

   }
   else
   {
      // pass to msg hook
      lr = pMsgHook->WindowProc( msg, wp, lp );
   }

   curMsg = oldMsg;        // pop state
   return( lr );
}

/////////////////////////////////////////////////////////////////////////////
// ZMsgHookMap implementation

ZMsgHookMap::ZMsgHookMap( )
{
}

ZMsgHookMap::~ZMsgHookMap( )
{
   ASSERT( IsEmpty( ) );   // all hooks should be removed!
}

/////////////////////////////////////////////////////////////////////////////
// Get the one and only global hook map
//
ZMsgHookMap& ZMsgHookMap::GetHookMap( )
{
   // By creating theMap here, C++ doesn't instantiate it until/unless
   // it's ever used!  This is a good trick to use in C++, to
   // instantiate/initialize a static object the first time it's used.
   //
   static ZMsgHookMap theMap;
   return( theMap );
}

/////////////////////////////////////////////////////////////////////////////
// Add hook to map; i.e., associate hook with window
//
void
ZMsgHookMap::Add( HWND hwnd, ZMsgHook *pMsgHook )
{
   ASSERT( hwnd && mIs_hWnd( hwnd ) );

   // Add to front of list
   pMsgHook->m_pNext = Lookup( hwnd );
   SetAt( hwnd, pMsgHook );

   if ( pMsgHook->m_pNext == 0 )
   {
      // If this is the first hook added, subclass the window
      pMsgHook->m_pOldWndProc =
         (WNDPROC) SetWindowLong( hwnd, GWL_WNDPROC, (DWORD) HookWndProc );

   }
   else
   {
      // just copy wndproc from next hook
      pMsgHook->m_pOldWndProc = pMsgHook->m_pNext->m_pOldWndProc;
   }

   ASSERT( pMsgHook->m_pOldWndProc );
}

/////////////////////////////////////////////////////////////////////////////
// Remove hook from map
//
void
ZMsgHookMap::Remove( ZMsgHook *pUnHook )
{
   HWND hwnd = pUnHook->m_pWndHooked->GetSafeHwnd( );
   ASSERT( hwnd && mIs_hWnd( hwnd ) );

   ZMsgHook *pHook = Lookup( hwnd );
   ASSERT( pHook );
   if ( pHook == pUnHook )
   {
      // hook to remove is the one in the hash table: replace w/next
      if ( pHook->m_pNext )
         SetAt( hwnd, pHook->m_pNext );
      else
      {
         // This is the last hook for this window: restore wnd proc
         RemoveKey( hwnd );
         SetWindowLong( hwnd, GWL_WNDPROC, (DWORD) pHook->m_pOldWndProc );
      }
   }
   else
   {
      // Hook to remove is in the middle: just remove from linked list
      while ( pHook->m_pNext != pUnHook )
         pHook = pHook->m_pNext;

      ASSERT( pHook && pHook->m_pNext == pUnHook );
      pHook->m_pNext = pUnHook->m_pNext;
   }
}

/////////////////////////////////////////////////////////////////////////////
// Remove all the hooks for a window
//
void
ZMsgHookMap::RemoveAll( HWND hwnd )
{
   ZMsgHook *pMsgHook;
   while ( (pMsgHook = Lookup( hwnd )) != 0 )
      pMsgHook->HookWindow( 0 ); // (unhook)
}

/////////////////////////////////////////////////////////////////////////////
// Find first hook associate with window
//
ZMsgHook *
ZMsgHookMap::Lookup( HWND hwnd )
{
   ZMsgHook *pFound = 0;
   if ( !CMapPtrToPtr::Lookup( hwnd, (void*&) pFound ) )
      return( 0 );

   ASSERT_KINDOF( ZMsgHook, pFound );
   return( pFound );
}
