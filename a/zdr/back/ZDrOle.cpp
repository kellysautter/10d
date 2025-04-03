/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrole.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZDrOleClient
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.05.11    DKS    Z10    ActiveX
//    Basic rewrite for new ActiveX implementation.
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

// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
// ZDrOleClient implementation

IMPLEMENT_SERIAL( ZDrOleClient, COleClientItem, 0 )

BEGIN_INTERFACE_MAP( ZDrOleClient, COleClientItem )
//   INTERFACE_PART( ZDrOleClient, IID_IOleInPlaceSiteEx,
//      OleInPlaceSiteWindowless )
//   INTERFACE_PART( ZDrOleClient, IID_IOleInPlaceSiteWindowless,
//      OleInPlaceSiteWindowless )
END_INTERFACE_MAP()


ZDrOleClient::ZDrOleClient( ZDrDoc  *pContainer ) :
              COleClientItem( pContainer )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrOleClient::ZDrOleClient ctor", "" );
#endif

   // TODO: add one-time construction code here
   // Added to support TB ActiveX Editor - 1998/4/18
   m_bWindowless = FALSE;
   m_pDispatchInstance = 0;
   m_pAttr = 0;
   m_bFullClient = FALSE;

   // Added to support TB ActiveX Editor - 1998/4/18
}

ZDrOleClient::ZDrOleClient( ZDrDoc  *pContainer,
                            ZAttr   *pAttr,
                            zBOOL   bFullClient ) :
              COleClientItem( pContainer )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrOleClient::ZDrOleClient ctor", "" );
#endif

   // TODO: add one-time construction code here
   // Added to support TB ActiveX Editor - 1998/4/18
   m_bWindowless = FALSE;
   m_pDispatchInstance = 0;
   m_pAttr = pAttr;
   m_bFullClient = bFullClient;

   // Added to support TB ActiveX Editor - 1998/4/18
}

ZDrOleClient::~ZDrOleClient( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrOleClient::~ZDrOleClient dtor", "" );
#endif

   // TODO: add cleanup code here
   mDeleteInit( m_pDispatchInstance );
}

BOOL
ZDrOleClient::InterfaceConnect( IUnknown *pObj, REFIID riid,
                                IUnknown *pIUnknownSink, LPDWORD pdwConn )
{
   TraceLineS( "ZDrOleClient::InterfaceConnect", "" );
   HRESULT                   hr;
   IConnectionPointContainer *pCPointContainer = 0;
   IConnectionPoint          *pCPoint = 0;

   if ( pObj == 0 || pIUnknownSink == 0 || pdwConn == 0 )
      return( FALSE );

   hr = pObj->QueryInterface( IID_IConnectionPointContainer,
                              (LPVOID *) &pCPointContainer );
   if ( FAILED( hr ) )
      return( FALSE );

   hr = pCPointContainer->FindConnectionPoint( riid, &pCPoint );
   if ( SUCCEEDED( hr ) )
   {
      hr = pCPoint->Advise( pIUnknownSink, pdwConn );
      if ( hr == E_POINTER )
         AfxMessageBox( "Invalid Pointer!" );
      else
      if ( hr == CONNECT_E_ADVISELIMIT )
         AfxMessageBox( "No more Connections available!" );
      else
      if ( hr == CONNECT_E_CANNOTCONNECT )
         AfxMessageBox( "No Connection!" );

      pCPoint->Release( );
   }
   else
   {
       if ( hr == E_OUTOFMEMORY )
          TraceLineS( "FindConnectionPoint failed: ", "E_OUTOFMEMORY" );
       else
       if ( hr == E_UNEXPECTED )
          TraceLineS( "FindConnectionPoint failed: ", "E_UNEXPECTED" );
       else
       if ( hr == E_POINTER )
          TraceLineS( "FindConnectionPoint failed: ", "E_POINTER" );
       else
       if ( hr == CONNECT_E_NOCONNECTION )
          TraceLineS( "FindConnectionPoint failed: ", "CONNECT_E_NOCONNECTION" );
       else
       if ( hr == S_OK )
          TraceLineS( "FindConnectionPoint failed: ", "S_OK" );
       else
          TraceLineX( "FindConnectionPoint failed: ", (zLONG) hr );
   }

   pCPointContainer->Release( );
   return( SUCCEEDED( hr ) );
} // InterfaceConnect

BOOL
ZDrOleClient::InterfaceDisconnect( IUnknown *pObj, REFIID riid,
                                   LPDWORD pdwConn )
{
   HRESULT                   hr;
   IConnectionPointContainer *pCPointContainer = 0;
   IConnectionPoint          *pCPoint = 0;

   if ( pObj == 0 || pdwConn == 0 )
      return( FALSE );

   if ( *pdwConn == 0 )
      return( FALSE );

   hr = pObj->QueryInterface( IID_IConnectionPointContainer,
                              (LPVOID *) &pCPointContainer );
   if ( FAILED( hr ) )
      return( FALSE );

   hr = pCPointContainer->FindConnectionPoint( riid, &pCPoint );
   if ( SUCCEEDED( hr ) )
   {
      // This is the Point to kill m_pEditorEventSink. We are doing this
      // through Unadvise
      hr = pCPoint->Unadvise(*pdwConn);
      if ( SUCCEEDED( hr ) )
         *pdwConn = 0L;

      pCPoint->Release( );
   }

   pCPointContainer->Release( );
   return( SUCCEEDED( hr ) );
} // InterfaceDisconnect
// Added to support TB ActiveX Editor - 1998/4/18

void
ZDrOleClient::OnChange( OLE_NOTIFICATION nCode, DWORD dwParam )
{
   ASSERT_VALID( this );

   COleClientItem::OnChange( nCode, dwParam );

   // When an item is being edited (either in-place or fully open)
   // it sends OnChange notifications for changes in the state of the
   // item or visual appearance of its content.

   // TODO: invalidate the item by calling UpdateAllViews
   // (with hints appropriate to your application)

   GetDocument( )->UpdateAllViews( 0 );
   // for now just update ALL views/no hints
}

CRect
ZDrOleClient::GetRect( ) const
{
   if ( m_pAttr )
   {
      CRect rect( m_pAttr->X, m_pAttr->Y,
                  m_pAttr->X + m_pAttr->W,
                  m_pAttr->Y + m_pAttr->H );
      return( rect );
   }
   else
      return( CRect( 0, 0, 0, 0 ) );
}

BOOL
ZDrOleClient::OnChangeItemPosition( const CRect& rectPos )
{
   ASSERT_VALID( this );

   // During in-place activation ZDrOleClient::OnChangeItemPosition
   // is called by the server to change the position of the in-place
   // window.  Usually, this is a result of the data in the server
   // document changing such that the extent has changed or as a result
   // of in-place resizing.
   //
   // The default here is to call the base class, which will call
   // COleClientItem::SetItemRects to move the item
   // to the new position.
   m_pView = GetParentView();
   if ( COleClientItem::OnChangeItemPosition( rectPos ) == 0 )
      return( FALSE );

   // TODO: update any cache you may have of the item's rectangle/extent
   return( TRUE );
}

void
ZDrOleClient::OnGetItemPosition( CRect& rPosition )
{
   ASSERT_VALID( this );

   // During in-place activation, ZDrOleClient::OnGetItemPosition
   // will be called to determine the location of this item.  The default
   // implementation created from AppWizard simply returns a hard-coded
   // rectangle.  Usually, this rectangle would reflect the current
   // position of the item relative to the view used for activation.
   // You can obtain the view by calling ZDrOleClient::GetActiveView.

   // TODO: return correct rectangle (in pixels) in rPosition

   if ( m_bFullClient )
   {
      CView *pParentView = GetParentView( );
      if ( pParentView )
      {
         pParentView->GetClientRect( &rPosition );
         rPosition.left = m_pAttr->X;
         rPosition.top = m_pAttr->Y;
      }
      else
         rPosition = GetRect( );
   }
   else
   {
      rPosition = GetRect( );
   }
}

void
ZDrOleClient::OnActivate( )
{
   // Allow only one inplace activate item per frame
   COleClientItem* pItem = GetDocument( )->GetInPlaceActiveItem( GetParentView() );
   if ( pItem && pItem != this )
      pItem->Close( );

   COleClientItem::OnActivate( );
}

void
ZDrOleClient::OnActivateUI( )
{
   // Allow only one inplace activate item per frame
   ZDrOleClient * pItem;
   ZDrDoc *pDoc = GetDocument();
   POSITION pos = pDoc->GetStartPosition();

   while ( pos )
   {
      pItem = (ZDrOleClient *)pDoc->GetNextItem ( pos );
      if ( pItem->GetItemState() == activeUIState )
      {
         pItem->DeactivateUI();
      }
   }


   COleClientItem::OnActivate( );
}

void
ZDrOleClient::OnDeactivateUI( BOOL bUndoable )
{
   m_pView = GetParentView();
   COleClientItem::OnDeactivateUI( bUndoable );

   // Hide the object if it is not an outside-in object
   DWORD dwMisc = 0;
   m_lpObject->GetMiscStatus( GetDrawAspect( ), &dwMisc );
   if ( dwMisc & OLEMISC_INSIDEOUT )
      DoVerb( OLEIVERB_HIDE, 0 );
}

void
ZDrOleClient::Serialize( CArchive& ar )
{
   ASSERT_VALID( this );

   // Call base class first to read in COleClientItem data.
   // Since this sets up the m_pDocument pointer returned from
   // ZDrOleClient::GetDocument, it is a good idea to call
   // the base class Serialize first.
   COleClientItem::Serialize( ar );

   // now store/retrieve data specific to ZDrOleClient
   if ( ar.IsStoring( ) )
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}

void ZDrOleClient::OnDeactivate( BOOL bNoRedraw )
{
   if ( m_bWindowless )
   {
     if ( ((ZDrView *)GetParentView())->GetItemWithFocus() == this )
     {
       ((ZDrView *)GetParentView())->OnWindowlessReleaseFocus();
     }
     m_bWindowless = FALSE;
     m_pOleInPlaceObjectWindowless->Release();
     m_pOleInPlaceObjectWindowless = 0;
   }

   if ( !bNoRedraw )
   {
     GetDocument()->UpdateAllViews( NULL, 1, this );
   }

   COleClientItem::OnDeactivate();
}

void ZDrOleClient::OnDeactivate()
{
   OnDeactivate( FALSE );
}


BOOL
ZDrOleClient::FinishCreate( SCODE sc )
{
   HRESULT hResult = 0;
   BOOL bReturn = COleClientItem::FinishCreate( sc );

   // Determine whether we have to set the ClientSite first
   // by passing our IOleClientSite
   m_lpObject->GetMiscStatus( DVASPECT_CONTENT, &m_dwMiscStatus );
   if ( m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST )
   {
      hResult = m_lpObject->SetClientSite( GetClientSite( ) );
      if ( FAILED( hResult ) )
      {
         bReturn = FALSE;
      }
   }

   if ( bReturn )
   {
      if ( IsInvisibleAtRuntime() )
      {
         DoVerb( OLEIVERB_HIDE, GetParentView() );
      }
      else
      {
         DoVerb( OLEIVERB_SHOW, GetParentView() );
      }
   }

   return( bReturn );
}

BOOL
ZDrOleClient::IsInvisibleAtRuntime() const
{
   return( m_dwMiscStatus&OLEMISC_INVISIBLEATRUNTIME );
}

/////////////////////////////////////////////////////////////////////////////
// Setting up the surrounding rectangle of the item
//
void
ZDrOleClient::SetupTracker( CRectTracker & rTracker ) const
{
   BOOL bDesignMode = FALSE;
   rTracker.m_rect = GetRect();

   rTracker.m_nStyle = 0;

   // Evaluate desgin or runtime mode
   if ( bDesignMode )
   {
      rTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::hatchInside;
   }
   else
   {
      rTracker.m_nStyle = CRectTracker::solidLine;
   }
}

ZDrDoc *
ZDrOleClient::GetDocument( )
{
   return( (ZDrDoc *) COleClientItem::GetDocument( ) );
}

ZDrView *
ZDrOleClient::GetActiveView( )
{
   return( (ZDrView *) COleClientItem::GetActiveView( ) );
}

void
ZDrOleClient::SetParentView( CView *p )
{
   m_pParentView = p;
}

CView *
ZDrOleClient::GetParentView( )
{
   return( m_pParentView );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrOleClient diagnostics

#ifdef _DEBUG
void
ZDrOleClient::AssertValid( ) const
{
   COleClientItem::AssertValid( );
}

void
ZDrOleClient::Dump( CDumpContext& dc ) const
{
   COleClientItem::Dump( dc );
}
#endif


STDMETHODIMP_( ULONG )
ZDrOleClient::XOleInPlaceSiteWindowless::AddRef()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless )

   return( pThis->ExternalAddRef() );
}

STDMETHODIMP_( ULONG ) ZDrOleClient::XOleInPlaceSiteWindowless::
   Release()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless )

   return( pThis->ExternalRelease() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::QueryInterface(
   REFIID iid, LPVOID* ppvObj )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless )

   return( pThis->ExternalQueryInterface( &iid, ppvObj ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   ContextSensitiveHelp( BOOL tEnterMode )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.ContextSensitiveHelp( tEnterMode ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::GetWindow(
   HWND* phWnd )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.GetWindow( phWnd ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   CanInPlaceActivate()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.CanInPlaceActivate() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   DeactivateAndUndo()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.DeactivateAndUndo() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   DiscardUndoState()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.DiscardUndoState() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::GetWindowContext(
   IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT prectPos,
   LPRECT prectClip, LPOLEINPLACEFRAMEINFO pFrameInfo )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.GetWindowContext( ppFrame, ppDoc, prectPos,
     prectClip, pFrameInfo ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   OnInPlaceActivate()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.OnInPlaceActivate() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   OnInPlaceDeactivate()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.OnInPlaceDeactivate() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::OnPosRectChange(
   LPCRECT prectPos )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.OnPosRectChange( prectPos ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless:: OnUIActivate()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.OnUIActivate() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::OnUIDeactivate(
   BOOL tUndoable )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.OnUIDeactivate( tUndoable ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::Scroll(
   SIZE sizeExtent )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->m_xOleIPSite.Scroll( sizeExtent ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   OnInPlaceActivateEx( BOOL* ptNoRedraw, DWORD dwFlags )
{
   HRESULT hResult;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   ASSERT( ptNoRedraw != NULL );

   ASSERT( pThis->m_bWindowless == FALSE );

   if ( dwFlags&ACTIVATE_WINDOWLESS )
   {
     pThis->m_bWindowless = TRUE;
     hResult = pThis->m_lpObject->QueryInterface(
       IID_IOleInPlaceObjectWindowless,
       (void**)pThis->m_pOleInPlaceObjectWindowless );
     ASSERT( SUCCEEDED( hResult ) );
   }

   if ( ptNoRedraw != NULL )
   {
     if ( pThis->GetActiveView()->GetUpdateRect( NULL ) )
     {
       *ptNoRedraw = FALSE;
     }
     else
     {
       *ptNoRedraw = TRUE;
     }
   }

   return( pThis->m_xOleIPSite.OnInPlaceActivate() );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   OnInPlaceDeactivateEx( BOOL tNoRedraw )
{
   (void)tNoRedraw;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   pThis->OnDeactivate( tNoRedraw );

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   RequestUIActivate()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::AdjustRect(
   LPRECT prect )
{
   (void)prect;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( E_NOTIMPL );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   CanWindowlessActivate()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::GetCapture()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   if ( pThis->GetActiveView()->GetItemWithCapture() == pThis )
   {
     return( S_OK );
   }
   else
   {
     return( S_FALSE );
   }
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::GetDC(
   LPCRECT prect, DWORD dwFlags, HDC* phDC )
{
   CView *pView;
   CRgn  rgn;
   CRect rect;

   (void)dwFlags;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   ASSERT( phDC != NULL );
   *phDC = NULL;

   if ( pThis->m_pDC != NULL )
   {
     return( E_FAIL );
   }

   pView = pThis->GetActiveView();
   ASSERT( pView );

   pThis->m_pDC = pView->GetDC();
   if ( pThis->m_pDC == NULL )
   {
     return( E_FAIL );
   }

   if ( prect == NULL )
   {
     // The clipping rectangle is the object's extent rectangle.
     rect = pThis->GetRect( );
   }
   else
   {
     // The clipping rectangle is the object's extent rectangle intersected
     // with the rectangle requested by the object.
     rect.IntersectRect( &pThis->GetRect( ), prect );
   }

   rgn.CreateRectRgnIndirect( &rect );
   pThis->m_pDC->SelectClipRgn( &rgn, RGN_AND );

   *phDC = pThis->m_pDC->GetSafeHdc();

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::GetFocus()
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   if ( pThis->GetActiveView()->GetItemWithFocus() == pThis )
   {
     return( S_OK );
   }
   else
   {
     return( S_FALSE );
   }
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::InvalidateRect(
   LPCRECT prect, BOOL tErase )
{
   CRect rect;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   if ( prect == NULL )
   {
     rect = pThis->GetRect( );
   }
   else
   {
     // Intersect the given rectangle with the bounds of the object.
     rect.IntersectRect( prect, &pThis->GetRect( ) );
   }

   if ( !rect.IsRectEmpty() )
   {
     pThis->GetActiveView()->InvalidateRect( &rect, tErase );
     pThis->GetActiveView()->UpdateWindow();
   }

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::InvalidateRgn(
   HRGN hRgn, BOOL tErase )
{
   CRgn rgn;
   CRgn* pRgn;
   CRect rectRgn;
   CRect rect;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   if ( hRgn == NULL )
   {
     // The object wants its full extent invalidated.  It's much easier to do
     // this with a rect than a region.
     return( InvalidateRect( NULL, tErase ) );
   }

   pRgn = CRgn::FromHandle( hRgn );
   pRgn->GetRgnBox( &rectRgn );
   rect.IntersectRect( &rectRgn, &pThis->GetRect( ) );
   if ( rect != rectRgn )
   {
     return( InvalidateRect( NULL, tErase ) );
   }

   pThis->GetActiveView()->InvalidateRgn( pRgn, tErase );

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::
   OnDefWindowMessage( UINT nMessage, WPARAM wParam, LPARAM lParam,
   LRESULT* plResult )
{
   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( pThis->GetActiveView()->OnWindowlessDefWindowMessage( nMessage,
     wParam, lParam, plResult ) );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::ReleaseDC( HDC hDC )
{
   CView *pView;

   UNUSED( hDC );

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   pView = pThis->GetActiveView();
   ASSERT( pView );

   ASSERT( hDC == pThis->m_pDC->GetSafeHdc() );

   pView->ReleaseDC( pThis->m_pDC );

   pThis->m_pDC = NULL;

   return( S_OK );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::ScrollRect(
   int dx, int dy, LPCRECT prectScroll, LPCRECT prectClip )
{
   (void)dx;
   (void)dy;
   (void)prectScroll;
   (void)prectClip;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   return( E_NOTIMPL );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::SetCapture( BOOL tCapture )
{
   HRESULT hResult;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   if ( tCapture )
   {
     hResult = pThis->GetActiveView()->OnWindowlessSetCapture( pThis );
     if ( FAILED( hResult ) )
     {
       return( hResult );
     }
   }
   else
   {
     hResult = pThis->GetActiveView()->OnWindowlessReleaseCapture( pThis );
     if ( FAILED( hResult ) )
     {
       return( hResult );
     }
   }

   return( hResult );
}

STDMETHODIMP ZDrOleClient::XOleInPlaceSiteWindowless::SetFocus( BOOL tFocus )
{
   HRESULT hResult;

   METHOD_PROLOGUE( ZDrOleClient, OleInPlaceSiteWindowless );

   if ( tFocus )
   {
      hResult = pThis->GetActiveView()->OnWindowlessSetFocus( pThis );
      if ( FAILED( hResult ) )
      {
         return( hResult );
      }
   }
   else
   {
      hResult = pThis->GetActiveView()->OnWindowlessReleaseFocus();
      if ( FAILED( hResult ) )
      {
         return( hResult );
      }
   }

   return( hResult );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Translate V.E.A.P.X reference
zOPER_EXPORT void OPERATION
GetVEAPX_Reference( zVIEW vSubtask, zVIEW vAppOI, zCPCHAR cpcVEAPX,
                    zPCHAR pchFileName, zLONG lMaxLth )
{
   zVIEW  vApp = 0;
   zLONG  lLth = zstrlen( cpcVEAPX ) + 1;
   zPCHAR pchVEA = new char[ lLth ];
   zPCHAR pchView;
   zPCHAR pchEntity;
   zPCHAR pchAttribute;
   zPCHAR pchImagePath;

   *pchFileName = 0;
   strcpy_s( pchVEA, lLth, cpcVEAPX );
   pchView = pchVEA + 1;
   pchEntity = zstrchr( pchView, '.' );
   cpcVEAPX = 0;
   if ( pchEntity )
   {
      *pchEntity = 0;
      pchEntity++;
      pchAttribute = zstrchr( pchEntity, '.' );
      if ( pchAttribute )
      {
         zPCHAR pchExt = 0;

         *pchAttribute = 0;
         pchAttribute++;
         pchImagePath = zstrchr( pchAttribute, '.' );
         if ( pchImagePath )
         {
            *pchImagePath = 0;
            pchImagePath++;
            pchExt = zstrchr( pchImagePath, '.' );
            if ( pchExt )
            {
               *pchExt = 0;
               pchExt++;
            }

            // Get Resource Image and URL names if specified.
            GetAppOrWorkstationValue( vSubtask, pchImagePath, pchFileName, (zSHORT) lMaxLth );
         }

         if ( *pchView )
            GetViewByName( &vApp, pchView, vSubtask, zLEVEL_TASK );

         if ( vApp == 0 )
            vApp = vAppOI;

         if ( vApp )
         {
            lLth = zstrlen( pchFileName );

            GetStringFromAttribute( pchFileName + lLth, lMaxLth - lLth, vApp, pchEntity, pchAttribute );
         // strcpy_s( pchFileName, "c:\\10d\\rrkk.jpg" );
            if ( pchFileName[ lLth ] )
            {
               if ( pchExt && *pchExt )
               {
                  lLth = zstrlen( pchFileName );
                  pchFileName[ lLth++ ] = '.';
                  strcpy_s( pchFileName + lLth, lMaxLth - lLth, pchExt );
               }
            }
         }
      }
   }

   delete [] pchVEA;
}

zOPER_EXPORT BYTE * OPERATION
LoadBitmapFromJPG( CString csFileName, UINT *pnWidth, UINT *pnHeight )
{
   BYTE *pBmp = ZJpegFile::JpegFileToRGB( csFileName, pnWidth, pnHeight );

   //////////////////////
   // set up for display

   // do this before DWORD-alignment!!!
   // this works on packed (not DWORD-aligned) buffers
   // swap red and blue for display
   ZJpegFile::BGRFromRGB( pBmp, *pnWidth, *pnHeight );

   // vertical flip for display
   ZJpegFile::VertFlipBuf( pBmp, *pnWidth * 3, *pnHeight );
   return( pBmp );
}

// <setjmp.h> is used for the optional error recovery mechanism shown in
// the second part of the example.

#include <setjmp.h>

// error handler, to avoid those pesky exit( 0 )'s

struct my_error_mgr
{
   struct jpeg_error_mgr pub;  /* "public" fields */

   jmp_buf setjmp_buffer;   /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF( void ) my_error_exit( j_common_ptr cinfo );

//
// to handle fatal errors.
// the original JPEG code will just exit( 0 ). can't really
// do that in Windows....
//

METHODDEF( void ) my_error_exit ( j_common_ptr cinfo )
{
   /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
   my_error_ptr myerr = (my_error_ptr) cinfo->err;

   char buffer[ JMSG_LENGTH_MAX ];

   /* Create the message */
   (*cinfo->err->format_message)( cinfo, buffer );

   /* Always display the message. */
   TraceLineS( "JPEG Fatal Error: ", buffer );
// MessageBox( 0, buffer, "JPEG Fatal Error", MB_ICONSTOP );

   /* Return control to the setjmp point */
   longjmp( myerr->setjmp_buffer, 1 );
}

// store a scanline to our data buffer
void
j_putRGBScanline( BYTE *jpegline,
                  int  widthPix,
                  BYTE *outBuf,
                  int  row );

void
j_putGrayScanlineToRGB( BYTE *jpegline,
                        int  widthPix,
                        BYTE *outBuf,
                        int  row );

/////////////////////////////////////////////////////////////////////////////
//
// constructor doesn't do much - there's no real class here...
//

ZJpegFile::ZJpegFile( )
{
}

ZJpegFile::~ZJpegFile( )
{
}

// Read a JPEG file
BYTE *
ZJpegFile::JpegFileToRGB( CString csFileName,
                          UINT *width,
                          UINT *height )
{
   // basic code from IJG Jpeg Code v6 example.c

   *width = 0;
   *height = 0;

   // This struct contains the JPEG decompression parameters and pointers to
   // working space (which is allocated as needed by the JPEG library).
   struct jpeg_decompress_struct cinfo;

   // We use our private extension JPEG error handler.
   // Note that this struct must live as long as the main JPEG parameter
   // struct, to avoid dangling-pointer problems.
   struct my_error_mgr jerr;

   // More stuff.
   FILE *infile = 0;    // source file

   JSAMPARRAY buffer;   // Output row buffer
   int row_stride;      // physical row width in output buffer
// zCHAR buf[ 250 ];

   // In this example we want to open the input file before doing anything else,
   // so that the setjmp( ) error recovery below can assume the file is open.
   // VERY IMPORTANT: use "b" option to fopen( ) if you are on a machine that
   // requires it in order to read binary files.

   if ( (infile = fopen( csFileName, "rb" )) == 0 )
   {
      TraceLineS( "JPEG: Can't open file: ", csFileName );
   // sprintf( buf, "JPEG :\nCan't open %s\n", csFileName );
   // AfxMessageBox( buf );
      return( 0 );
   }

   // Step 1: allocate and initialize JPEG decompression object.

   // We set up the normal JPEG error routines, then override error_exit.
   cinfo.err = jpeg_std_error( &jerr.pub );
   jerr.pub.error_exit = my_error_exit;

   // Establish the setjmp return context for my_error_exit to use.
   if ( setjmp( jerr.setjmp_buffer ) )
   {
      // If we get here, the JPEG code has signaled an error.  We need to
      // clean up the JPEG object, close the input file, and return.
      jpeg_destroy_decompress( &cinfo );

      if ( infile )
         fclose( infile );

      return( 0 );
   }

   // Now we can initialize the JPEG decompression object.
   jpeg_create_decompress( &cinfo );

   // Step 2: specify data source (e.g. a file)

   jpeg_stdio_src( &cinfo, infile );

   // Step 3: read file parameters with jpeg_read_header( ).
   (void) jpeg_read_header( &cinfo, TRUE );

   // We can ignore the return value from jpeg_read_header since
   //   (a) suspension is not possible with the stdio data source, and
   //   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   // See libjpe.g.doc for more info.

   // Step 4: set parameters for decompression.

   // In this example, we don't need to change any of the defaults set by
   // jpeg_read_header( ), so we do nothing here.

   // Step 5: Start decompressor.

   (void) jpeg_start_decompress( &cinfo );

   // We can ignore the return value since suspension is not possible
   // with the stdio data source.

   // We may need to do some setup of our own at this point before reading
   // the data.  After jpeg_start_decompress( ) we have the correct scaled
   // output image dimensions available, as well as the output colormap
   // if we asked for color quantization.
   // In this example, we need to make an output work buffer of the right size.

   // get our buffer set to hold data
   BYTE *dataBuf = (BYTE *) new BYTE[ cinfo.output_width * 3 * cinfo.output_height ];
   if ( dataBuf == 0 )
   {
      AfxMessageBox( "ZJpegFile :\nOut of memory", MB_ICONSTOP );
      jpeg_destroy_decompress( &cinfo );
      fclose( infile );
      return( 0 );
   }

   // how big is this thing gonna be?
   *width = cinfo.output_width;
   *height = cinfo.output_height;

   // JSAMPLEs per row in output buffer.
   row_stride = cinfo.output_width * cinfo.output_components;

   // Make a one-row-high sample array that will go away when done with image.
   buffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr) &cinfo,
                                        JPOOL_IMAGE, row_stride, 1 );

   // Step 6: while ( scan lines remain to be read )
   //           jpeg_read_scanlines( ... );

   // Here we use the library's state variable cinfo.output_scanline as the
   // loop counter, so that we don't have to keep track ourselves.
   while ( cinfo.output_scanline < cinfo.output_height )
   {
      // jpeg_read_scanlines expects an array of pointers to scanlines.
      // Here the array is only one element long, but you could ask for
      // more than one scanline at a time if that's more convenient.
      (void) jpeg_read_scanlines( &cinfo, buffer, 1 );

      // Assume put_scanline_someplace wants a pointer and sample count.

      // assume all 3-components are RGBs
      if ( cinfo.out_color_components == 3 )
      {
         j_putRGBScanline( buffer[ 0 ], *width, dataBuf, cinfo.output_scanline - 1 );
      }
      else
      if ( cinfo.out_color_components == 1 )
      {
         // assume all single component images are grayscale
         j_putGrayScanlineToRGB( buffer[ 0 ], *width, dataBuf, cinfo.output_scanline - 1 );
      }
   }

   // Step 7: Finish decompression.

   (void) jpeg_finish_decompress( &cinfo );
   // We can ignore the return value since suspension is not possible
   // with the stdio data source.

   // Step 8: Release JPEG decompression object.

   // This is an important step since it will release a good deal of memory.
   jpeg_destroy_decompress( &cinfo );

   // After finish_decompress, we can close the input file.
   // Here we postpone it until after no more JPEG errors are possible,
   // so as to simplify the setjmp error logic above.  (Actually, I don't
   // think that jpeg_destroy can do an error exit, but why assume anything ...)
   fclose( infile );

   // At this point you may want to check to see whether any corrupt-data
   // warnings occurred (test whether jerr.pub.num_warnings is nonzero).

   return( dataBuf );
}

BOOL
ZJpegFile::GetJPGDimensions( CString csFileName,
                             UINT *width,
                             UINT *height )

{
   // basic code from IJG Jpeg Code v6 example.c

   // This struct contains the JPEG decompression parameters and pointers to
   // working space (which is allocated as needed by the JPEG library).
   struct jpeg_decompress_struct cinfo;

   // We use our private extension JPEG error handler.
   // Note that this struct must live as long as the main JPEG parameter
   // struct, to avoid dangling-pointer problems.
   struct my_error_mgr jerr;

   // More stuff.
   FILE *infile = 0;     // source file
// zCHAR buf[ 250 ];

   // In this example we want to open the input file before doing anything else,
   // so that the setjmp( ) error recovery below can assume the file is open.
   // VERY IMPORTANT: use "b" option to fopen( ) if you are on a machine that
   // requires it in order to read binary files.
   if ( (infile = fopen( csFileName, "rb" )) == 0 )
   {
   // sprintf( buf, "JPEG :\nCan't open %s\n", csFileName );
      TraceLineS( "JPEG Can't open file: ", csFileName );
   // AfxMessageBox( buf );
      return( FALSE );
   }

   // Step 1: allocate and initialize JPEG decompression object.

   // We set up the normal JPEG error routines, then override error_exit.
   cinfo.err = jpeg_std_error( &jerr.pub );
   jerr.pub.error_exit = my_error_exit;

   // Establish the setjmp return context for my_error_exit to use.
   if ( setjmp( jerr.setjmp_buffer ) )
   {
      // If we get here, the JPEG code has signaled an error.  We need to
      // clean up the JPEG object, close the input file, and return.
      jpeg_destroy_decompress( &cinfo );

      if ( infile )
         fclose( infile );

      return( FALSE );
   }

   // Now we can initialize the JPEG decompression object.
   jpeg_create_decompress( &cinfo );

   // Step 2: specify data source (e.g. a file).

   jpeg_stdio_src( &cinfo, infile );

   // Step 3: read file parameters with jpeg_read_header( ).
   (void) jpeg_read_header( &cinfo, TRUE );

   // We can ignore the return value from jpeg_read_header since
   //   (a) suspension is not possible with the stdio data source, and
   //   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   // See libjpe.g.doc for more info.

   // How big is this thing?
   *width = cinfo.image_width;
   *height = cinfo.image_height;

   // Step 8: Release JPEG decompression object.

   // This is an important step since it will release a good deal of memory.
   jpeg_destroy_decompress( &cinfo );

   // After finish_decompress, we can close the input file.
   // Here we postpone it until after no more JPEG errors are possible, so as
   // to simplify the setjmp error logic above.  (Actually, I don't think
   // that jpeg_destroy can do an error exit, but why assume anything ...)
   fclose( infile );

   // At this point you may want to check to see whether any corrupt-data
   // warnings occurred (test whether jerr.pub.num_warnings is nonzero).

   return( TRUE );
}

BYTE *
ZJpegFile::RGBFromDWORDAligned( BYTE *inBuf,
                                UINT widthPix,
                                UINT widthBytes,
                                UINT height )
{
   if ( inBuf == 0 )
      return( 0 );

   BYTE *tmp;
   tmp = (BYTE *) new BYTE[ height * widthPix * 3 ];
   if ( tmp == 0 )
      return( 0 );

   UINT uRow;

   for ( uRow = 0; uRow < height; uRow++ )
   {
      memcpy( (tmp + uRow * widthPix * 3), (inBuf + uRow * widthBytes), widthPix * 3 );
   }

   return( tmp );
}

BOOL
ZJpegFile::RGBToJpegFile( CString csFileName,
                          BYTE *dataBuf,
                          UINT widthPix,
                          UINT height,
                          BOOL color,
                          int quality )
{
   if ( dataBuf == 0 )
      return( FALSE );

   if ( widthPix == 0 )
      return( FALSE );

   if ( height == 0 )
      return( FALSE );

   LPBYTE tmp;
   if ( color == 0 )
   {
      tmp = (BYTE *) new BYTE[ widthPix * height ];
      if ( tmp == 0 )
      {
         AfxMessageBox( "Memory error" );
         return( FALSE );
      }

      UINT uRow, uCol;
      for ( uRow = 0; uRow < height; uRow++ )
      {
         for ( uCol = 0; uCol < widthPix; uCol++ )
         {
            LPBYTE pRed, pGrn, pBlu;
            pRed = dataBuf + uRow * widthPix * 3 + uCol * 3;
            pGrn = dataBuf + uRow * widthPix * 3 + uCol * 3 + 1;
            pBlu = dataBuf + uRow * widthPix * 3 + uCol * 3 + 2;

            // luminance
            int lum = (int) (.299 * (double) (*pRed) +
                             .587 * (double) (*pGrn) +
                             .114 * (double) (*pBlu) );
            LPBYTE pGray;
            pGray = tmp + uRow * widthPix + uCol;
            *pGray = (BYTE) lum;
         }
      }
   }

   struct jpeg_compress_struct cinfo;
   FILE *outfile = 0;      // target file
   int row_stride;         // physical row widthPix in image buffer

   struct my_error_mgr jerr;

   // Step 1: allocate and initialize JPEG compression object
   cinfo.err = jpeg_std_error( &jerr.pub );
   jerr.pub.error_exit = my_error_exit;

   // Establish the setjmp return context for my_error_exit to use.
   if ( setjmp( jerr.setjmp_buffer ) )
   {
      // If we get here, the JPEG code has signaled an error.  We need to
      // clean up the JPEG object, close the input file, and return.

      jpeg_destroy_compress( &cinfo );

      if ( outfile )
         fclose( outfile );

      if ( color == 0 )
      {
         delete [] tmp;
      }

      return( FALSE );
   }

   // Now we can initialize the JPEG compression object.
   jpeg_create_compress( &cinfo );

   // Step 2: specify data destination (e.g. a file)
   // Note: steps 2 and 3 can be done in either order.

   if ( (outfile = fopen( csFileName, "wb" )) == 0 )
   {
   // zCHAR buf[ 250 ];
   // sprintf( buf, "ZJpegFile :\nCan't open %s\n", csFileName );
      TraceLineS( "ZJpegFile Can't open file: ", csFileName );
   // AfxMessageBox( buf );
      return( FALSE );
   }

   jpeg_stdio_dest( &cinfo, outfile );

   // Step 3: set parameters for compression.

   // First we supply a description of the input image.
   // Four fields of the cinfo struct must be filled in:
   cinfo.image_width = widthPix;    /* image widthPix and height, in pixels */
   cinfo.image_height = height;
   if ( color )
   {
      cinfo.input_components = 3;      /* # of color components per pixel */
      cinfo.in_color_space = JCS_RGB;  /* colorspace of input image */
   }
   else
   {
      cinfo.input_components = 1;      /* # of color components per pixel */
      cinfo.in_color_space = JCS_GRAYSCALE;  /* colorspace of input image */
   }

   // Now use the library's routine to set default compression parameters.
   // (You must set at least cinfo.in_color_space before calling this,
   // since the defaults depend on the source color space.)
   jpeg_set_defaults( &cinfo );
   // Now you can set any non-default parameters you wish to.
   // Here we just illustrate the use of quality (quantization table) scaling:
   jpeg_set_quality( &cinfo, quality, TRUE /* limit to baseline-JPEG values */ );

   // Step 4: Start compressor.

   // TRUE ensures that we will write a complete interchange-JPEG file.
   // Pass TRUE unless you are very sure of what you're doing.
   jpeg_start_compress( &cinfo, TRUE );

   // Step 5: while (scan lines remain to be written)
   //           jpeg_write_scanlines( ... );

   // Here we use the library's state variable cinfo.next_scanline as the
   // loop counter, so that we don't have to keep track ourselves.
   // To keep things simple, we pass one scanline per call; you can pass
   // more if you wish, though.
   row_stride = widthPix * 3;  /* JSAMPLEs per row in image_buffer */

   while ( cinfo.next_scanline < cinfo.image_height )
   {
      // jpeg_write_scanlines expects an array of pointers to scanlines.
      // Here the array is only one element long, but you could pass
      // more than one scanline at a time if that's more convenient.
      LPBYTE outRow;
      if ( color )
      {
         outRow = dataBuf + (cinfo.next_scanline * widthPix * 3);
      }
      else
      {
         outRow = tmp + (cinfo.next_scanline * widthPix);
      }

      (void) jpeg_write_scanlines( &cinfo, &outRow, 1 );
  }

  // Step 6: Finish compression.

  jpeg_finish_compress( &cinfo );

  // After finish_compress, we can close the output file.
  fclose( outfile );

  // Step 7: release JPEG compression object.

  // This is an important step since it will release a good deal of memory.
  jpeg_destroy_compress( &cinfo );

  if ( color == 0 )
     delete [] tmp;

  // And we're done!

  return( TRUE );
}

//
// stash a scanline
//

void
j_putRGBScanline( BYTE *jpegline,
                  int widthPix,
                  BYTE *outBuf,
                  int row )
{
   int offset = row * widthPix * 3;
   int count;
   for ( count = 0; count < widthPix; count++ )
   {
      *(outBuf + offset + count * 3 + 0) = *(jpegline + count * 3 + 0);
      *(outBuf + offset + count * 3 + 1) = *(jpegline + count * 3 + 1);
      *(outBuf + offset + count * 3 + 2) = *(jpegline + count * 3 + 2);
   }
}

// Stash a gray scanline.
void
j_putGrayScanlineToRGB( BYTE *jpegline,
                        int widthPix,
                        BYTE *outBuf,
                        int row )
{
   int offset = row * widthPix * 3;
   int count;
   for ( count = 0; count < widthPix; count++ )
   {

      BYTE iGray;

      // get our grayscale value
      iGray = *(jpegline + count);

      *(outBuf + offset + count * 3 + 0) = iGray;
      *(outBuf + offset + count * 3 + 1) = iGray;
      *(outBuf + offset + count * 3 + 2) = iGray;
   }
}

// Copies BYTE buffer into DWORD-aligned BYTE buffer.
// Return addr of new buffer.
BYTE *
ZJpegFile::MakeDwordAlignedBuf( BYTE *dataBuf,
                                UINT widthPix,            // pixels!!
                                UINT height,
                                UINT *uiOutWidthBytes )   // bytes!!!
{
   //////////////////////////////////////////////////////////////////////////
   // what's going on here? this certainly means trouble
   if ( dataBuf == 0 )
      return( 0 );

   //////////////////////////////////////////////////////////////////////////
   // how big is the smallest DWORD-aligned buffer that we can use?
   UINT uiWidthBytes;
   uiWidthBytes = WIDTHBYTES( widthPix * 24 );

   DWORD dwNewsize = (DWORD) ((DWORD) uiWidthBytes * (DWORD) height);
   BYTE *pNew;

   //////////////////////////////////////////////////////////////////////////
   // alloc and open our new buffer
   pNew = (BYTE *) new BYTE[ dwNewsize ];
   if ( pNew == 0 )
   {
      return( 0 );
   }

   //////////////////////////////////////////////////////////////////////////
   // copy row-by-row
   UINT uiInWidthBytes = widthPix * 3;
   UINT uiCount;
   for ( uiCount = 0;uiCount < height;uiCount++ )
   {
      BYTE * bpInAdd;
      BYTE * bpOutAdd;
      ULONG lInOff;
      ULONG lOutOff;

      lInOff = uiInWidthBytes * uiCount;
      lOutOff = uiWidthBytes * uiCount;

      bpInAdd = dataBuf + lInOff;
      bpOutAdd = pNew + lOutOff;

      memcpy( bpOutAdd,bpInAdd,uiInWidthBytes );
   }

   *uiOutWidthBytes = uiWidthBytes;
   return pNew;
}

// Vertically flip a buffer
// note, this operates on a buffer of widthBytes bytes, not pixels!!!
BOOL
ZJpegFile::VertFlipBuf( BYTE * inbuf,
                       UINT widthBytes,
                       UINT height )
{
   BYTE  *tb1;
   BYTE  *tb2;

   if ( inbuf == 0 )
      return( FALSE );

   UINT bufsize;

   bufsize = widthBytes;

   tb1 = (BYTE *) new BYTE[ bufsize ];
   if ( tb1 == 0 )
   {
      return( FALSE );
   }

   tb2 = (BYTE *) new BYTE [ bufsize ];
   if ( tb2 == 0 )
   {
      delete [] tb1;
      return( FALSE );
   }

   UINT row_cnt;
   ULONG off1 = 0;
   ULONG off2 = 0;

   for ( row_cnt = 0; row_cnt < (height + 1) / 2; row_cnt++ )
   {
      off1 = row_cnt*bufsize;
      off2 = ((height - 1) - row_cnt) * bufsize;

      memcpy( tb1,inbuf+off1,bufsize );
      memcpy( tb2,inbuf+off2,bufsize );
      memcpy( inbuf+off1,tb2,bufsize );
      memcpy( inbuf+off2,tb1,bufsize );
   }

   delete [] tb1;
   delete [] tb2;

   return( TRUE );
}

// Swap Rs and Bs
//
// Note! this does its stuff on buffers with a whole number of pixels
// per data row!!
BOOL
ZJpegFile::BGRFromRGB( BYTE *buf, UINT widthPix, UINT height )
{
   if ( buf == 0 )
      return( FALSE );

   UINT uRow, uCol;
   for ( uRow = 0; uRow < height; uRow++ )
   {
      for ( uCol = 0; uCol < widthPix; uCol++ )
      {
         LPBYTE pRed, pGrn, pBlu;
         pRed = buf + uRow * widthPix * 3 + uCol * 3;
         pGrn = buf + uRow * widthPix * 3 + uCol * 3 + 1;
         pBlu = buf + uRow * widthPix * 3 + uCol * 3 + 2;

         // swap red and blue
         BYTE tmp;
         tmp = *pRed;
         *pRed = *pBlu;
         *pBlu = tmp;
      }
   }

   return( TRUE );
}

// Note! this does its stuff on buffers with a whole number of pixels
// per data row!!
BOOL
ZJpegFile::MakeGrayScale( BYTE *buf, UINT widthPix, UINT height )
{
   if ( buf == 0 )
      return( FALSE );

   UINT uRow, uCol;
   for ( uRow = 0; uRow < height; uRow++ )
   {
      for ( uCol = 0; uCol < widthPix; uCol++ )
      {
         LPBYTE pRed, pGrn, pBlu;
         pRed = buf + uRow * widthPix * 3 + uCol * 3;
         pGrn = buf + uRow * widthPix * 3 + uCol * 3 + 1;
         pBlu = buf + uRow * widthPix * 3 + uCol * 3 + 2;

         // luminance
         int lum = (int) (.299 * (double) (*pRed) + .587 * (double) (*pGrn) + .114 * (double) (*pBlu));

         *pRed = (BYTE) lum;
         *pGrn = (BYTE) lum;
         *pBlu = (BYTE) lum;
      }
   }

   return( TRUE );
}

void
ZJpegFile::DrawIt( HDC hDC, CRect& rect, BYTE *pBmp,
                   UINT nWidth, UINT nHeight, zBOOL bFit )
{
   UINT nWidthDW = 0;  // just a throwaway

   // Center It
   int left = bFit ? rect.left : (rect.Width( ) - (int) nWidth) / 2;
   int top = bFit ? rect.top : (rect.Height( ) - (int) nHeight) / 2;

   // A 24-bit DIB is DWORD-aligned, vertically flipped and
   // has Red and Blue bytes swapped. we already did the
   // RGB->BGR and the flip when we read the images, now do
   // the DWORD-align.

   // DWORD-align for display
   BYTE *tmp = ZJpegFile::MakeDwordAlignedBuf( pBmp, nWidth, nHeight,
                                               &nWidthDW );

   // set up a DIB
   BITMAPINFOHEADER bmiHeader;
   bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
   bmiHeader.biWidth = nWidth;
   bmiHeader.biHeight = nHeight;
   bmiHeader.biPlanes = 1;
   bmiHeader.biBitCount = 24;
   bmiHeader.biCompression = BI_RGB;
   bmiHeader.biSizeImage = 0;
   bmiHeader.biXPelsPerMeter = 0;
   bmiHeader.biYPelsPerMeter = 0;
   bmiHeader.biClrUsed = 0;
   bmiHeader.biClrImportant = 0;

   // Now blast it to the CDC passed in.
   // nLines returns the number of lines actually displayed
   ::SetStretchBltMode( hDC, COLORONCOLOR );
   int nLines = ::StretchDIBits( hDC,
                                 left, top,
                                 bFit ? rect.Width( ) : bmiHeader.biWidth,
                                 bFit ? rect.Height( ) : bmiHeader.biHeight,
                                 0, 0,
                                 bmiHeader.biWidth,
                                 bmiHeader.biHeight,
                                 tmp,
                                 (LPBITMAPINFO) &bmiHeader,
                                 DIB_RGB_COLORS,
                                 SRCCOPY );

   delete [] tmp;
}
