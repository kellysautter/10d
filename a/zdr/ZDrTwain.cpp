/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrtwain.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZTwain.
//

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

// Constructor:
//   Parameters : HWND - Window to subclass.

ZTwain::ZTwain( CFrameWnd *pZFWnd )
{
   m_hTwainDLL = 0;
   m_pDSMProc = 0;
   m_bSourceSelected = FALSE;
   m_bDSOpen = m_bDSMOpen = FALSE;
   m_bSourceEnabled = FALSE;
   m_bModalUI = TRUE;
   m_nImageCnt = TWCPP_ANYCOUNT;
   m_pZFWnd = pZFWnd;
   if ( pZFWnd )
   {
      InitTwain( pZFWnd );
   }
}

ZTwain::~ZTwain( )
{
   ReleaseTwain( 0 );
}

// Initializes TWAIN interface.  Is already called from the constructor.
// It should be called again if ReleaseTwain is called.
//
// pZFWnd is the window which has to subclassed in order to receive Twain
// messages.  Normally - this would be your main application window.

zBOOL
ZTwain::InitTwain( CFrameWnd *pZFWnd )
{
   zCHAR szTwainLibName[ 512 ];

   if ( pZFWnd == 0 || mIs_hWnd( pZFWnd->m_hWnd ) == FALSE )
   {
      ReleaseTwain( 0 );
      return( FALSE );
   }

   if ( m_pZFWnd != pZFWnd )
   {
      ReleaseTwain( 0 );
      m_pZFWnd = pZFWnd;
   }
   else
   if ( IsValidDriver( ) )
   {
      return( TRUE );
   }

   zmemset( &m_AppId, 0, sizeof( m_AppId ) );
   strcpy_s( szTwainLibName, sizeof( szTwainLibName ), "TWAIN_32.DLL" );

   m_hTwainDLL = ::LoadLibrary( szTwainLibName );
   if ( m_hTwainDLL )
   {
      if ( (m_pDSMProc = (DSMENTRYPROC)
                GetProcAddress( m_hTwainDLL, MAKEINTRESOURCE( 1 ) )) == 0 )
      {
         FreeLibrary( m_hTwainDLL );
         m_hTwainDLL = 0;
      }
   }

   if ( IsValidDriver( ) )
   {
      ((ZDrFrame *) m_pZFWnd)->m_pZTwain = this;
      GetIdentity( );
      m_bDSMOpen = CallTwainProc( &m_AppId, 0, DG_CONTROL,
                                  DAT_PARENT, MSG_OPENDSM,
                                  (TW_MEMREF) &(m_pZFWnd->m_hWnd) );
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

// Releases the twain interface.  Need not be called unless you
// want to specifically shut it down.
void
ZTwain::ReleaseTwain( CFrameWnd *pZFWnd )
{
   if ( m_pZFWnd == m_pZFWnd || m_pZFWnd == 0 )
   {
      if ( m_pZFWnd )
         ((ZDrFrame *) m_pZFWnd)->m_pZTwain = 0;

      if ( IsValidDriver( ) )
      {
         CloseDSM( );
         FreeLibrary( m_hTwainDLL );
         m_hTwainDLL = 0;
         m_pDSMProc = 0;
      }
   }
}

// Returns true if a valid driver has been loaded.
zBOOL
ZTwain::IsValidDriver( ) const
{
   return( m_hTwainDLL && m_pDSMProc );
}

// Entry point into Twain. For a complete description of this
// routine, please refer to the Twain specification 1.8.
zBOOL
ZTwain::CallTwainProc( pTW_IDENTITY pOrigin, pTW_IDENTITY pDest,
                       TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG,
                       TW_MEMREF pData )
{
   if ( IsValidDriver( ) )
   {
      USHORT uRC;

      uRC = (*m_pDSMProc)( pOrigin, pDest, DG, DAT, MSG, pData );
      m_nRC = uRC;
      if ( uRC != TWRC_SUCCESS )
      {
         (*m_pDSMProc)( pOrigin, pDest,
                        DG_CONTROL, DAT_STATUS, MSG_GET, &m_Status );
      }

      return( uRC == TWRC_SUCCESS );
   }
   else
   {
      m_nRC = TWRC_FAILURE;
      return( FALSE );
   }
}

// This function should ideally be overridden in the derived class . If only
// a few fields need to be updated, call CTawin::GetIdentity first in your
// derived class.
void
ZTwain::GetIdentity( )
{
   // Expects all the fields in m_AppId to be set except for the Id field.
   m_AppId.Id = 0; // Initialize to 0 (Source Manager will assign real value)
   m_AppId.Version.MajorNum = 3; // your app's version number
   m_AppId.Version.MinorNum = 5;
   m_AppId.Version.Language = TWLG_USA;
   m_AppId.Version.Country = TWCY_USA;
   strcpy_s( m_AppId.Version.Info, sizeof( m_AppId.Version.Info ), "3.5" );
   m_AppId.ProtocolMajor = TWON_PROTOCOLMAJOR;
   m_AppId.ProtocolMinor = TWON_PROTOCOLMINOR;
   m_AppId.SupportedGroups = DG_IMAGE | DG_CONTROL;
   strcpy_s( m_AppId.Manufacturer, sizeof( m_AppId.Manufacturer ), "MICSS" );
   strcpy_s( m_AppId.ProductFamily, sizeof( m_AppId.ProductFamily ), "Generic" );
   strcpy_s( m_AppId.ProductName, sizeof( m_AppId.ProductName ), "Zeidon" );
}

// Called to display a dialog box to select the Twain source to use.
// This can be overridden if a list of all sources is available
// to the application. These sources can be enumerated by Twain.
// it is not yet supportted by ZTwain.
zBOOL
ZTwain::SelectSource( )
{
   zmemset( &m_Source, 0, sizeof( m_Source ) );
   if ( SourceSelected( ) == FALSE )
   {
      SelectDefaultSource( );
   }

   if ( CallTwainProc( &m_AppId, 0, DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, &m_Source ) )
   {
      m_bSourceSelected = TRUE;
   }

   return( m_bSourceSelected );
}

// Called to select the default source.
zBOOL
ZTwain::SelectDefaultSource( )
{
   m_bSourceSelected = CallTwainProc( &m_AppId, 0, DG_CONTROL, DAT_IDENTITY,
                                      MSG_GETDEFAULT, &m_Source );
   return( m_bSourceSelected );
}

// Closes the Data Source.
void
ZTwain::CloseDS( )
{
   if ( DSOpen( ) )
   {
      DisableSource( );
      CallTwainProc( &m_AppId, 0, DG_CONTROL, DAT_IDENTITY,
                     MSG_CLOSEDS, (TW_MEMREF) &m_Source );
      m_bDSOpen = FALSE;
   }
}

// Closes the Data Source Manager.
void
ZTwain::CloseDSM( )
{
   if ( DSMOpen( ) )
   {
      CloseDS( );
      CallTwainProc( &m_AppId, 0, DG_CONTROL,
                     DAT_PARENT, MSG_CLOSEDSM,
                     (TW_MEMREF) &(m_pZFWnd->m_hWnd) );
      m_bDSMOpen = FALSE;
   }
}

// Returns true if the Data Source Manager is Open.
zBOOL
ZTwain::DSMOpen( ) const
{
   return( IsValidDriver( ) && m_bDSMOpen );
}

// Returns true if the Data Source is Open.
zBOOL
ZTwain::DSOpen( ) const
{
   return( IsValidDriver( ) && DSMOpen( ) && m_bDSOpen );
}

// Opens a Data Source supplied as the input parameter.
zBOOL
ZTwain::OpenSource( TW_IDENTITY *pSource )
{
   if ( pSource )
   {
      m_Source = *pSource;
   }

   if ( DSMOpen( ) )
   {
      if ( SourceSelected( ) == FALSE )
      {
         SelectDefaultSource( );
      }

      m_bDSOpen = CallTwainProc( &m_AppId, 0,
                                 DG_CONTROL, DAT_IDENTITY,
                                 MSG_OPENDS, (TW_MEMREF) &m_Source );
   }

   return( DSOpen( ) );
}

// Should be called from the main message loop of the application. Can always
// be called ... it will not process the message unless a scan is in progress.
zBOOL
ZTwain::ProcessMessage( MSG *pMsg )
{
   if ( m_bSourceEnabled )
   {
      TW_EVENT twEvent;

      twEvent.pEvent = (TW_MEMREF) pMsg;
      twEvent.TWMessage = MSG_NULL;

      CallTwainProc( &m_AppId, &m_Source, DG_CONTROL, DAT_EVENT,
                     MSG_PROCESSEVENT, (TW_MEMREF) &twEvent );
      if ( GetRC( ) != TWRC_NOTDSEVENT )
      {
         TranslateMessage( twEvent );
      }
   }

   return( FALSE );
}

// Queries the capability of the Twain Data Source.
zBOOL
ZTwain::GetCapability( TW_CAPABILITY& twCap,
                       TW_UINT16 cap,
                       TW_UINT16 conType )
{
   if ( DSOpen( ) )
   {
      twCap.Cap = cap;
      twCap.ConType = conType;
      twCap.hContainer = 0;

      if ( CallTwainProc( &m_AppId, &m_Source,
                          DG_CONTROL, DAT_CAPABILITY,
                          MSG_GET, (TW_MEMREF) &twCap ) )
      {
         return( TRUE );
      }
   }

   return( FALSE );
}

// Queries the capability of the Twain Data Source.
zBOOL
ZTwain::GetCapability( TW_UINT16 cap,
                       TW_UINT32& value )
{
   TW_CAPABILITY twCap;

   if ( GetCapability( twCap, cap ) )
   {
      pTW_ONEVALUE pVal = (pTW_ONEVALUE) GlobalLock( twCap.hContainer );
      if ( pVal )
      {
         value = pVal->Item;
         GlobalUnlock( pVal );
         GlobalFree( twCap.hContainer );
         return( TRUE );
      }
   }

   return( FALSE );
}

// Sets the capability of the Twain Data Source.
zBOOL
ZTwain::SetCapability( TW_UINT16 cap, TW_UINT16 value, zBOOL bSign )
{
   if ( DSOpen( ) )
   {
      TW_CAPABILITY twCap;
      pTW_ONEVALUE pVal;
      zBOOL bRC = FALSE;

      twCap.Cap = cap;
      twCap.ConType = TWON_ONEVALUE;

      twCap.hContainer = GlobalAlloc( GHND, sizeof( TW_ONEVALUE ) );
      if ( twCap.hContainer )
      {
         pVal = (pTW_ONEVALUE) GlobalLock( twCap.hContainer );
         pVal->ItemType = bSign ? TWTY_INT16 : TWTY_UINT16;
         pVal->Item = (TW_UINT32) value;
         GlobalUnlock( twCap.hContainer );
         bRC = SetCapability( twCap );
         GlobalFree( twCap.hContainer );
      }

      return( bRC );
   }

   return( FALSE );
}

// Sets the capability of the Twain Data Source.
zBOOL
ZTwain::SetCapability( TW_CAPABILITY& cap )
{
   if ( DSOpen( ) )
   {
      return( CallTwainProc( &m_AppId, &m_Source,
                             DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                             (TW_MEMREF) &cap ) );
   }

   return( FALSE );
}

// Sets the number of images which can be accepted by the application at
// one time.
zBOOL
ZTwain::SetImageCount( TW_INT16 nCount )
{
   if ( SetCapability( CAP_XFERCOUNT, (TW_UINT16) nCount, TRUE ) )
   {
      m_nImageCnt = nCount;
      return( TRUE );
   }
   else
   {
      if ( GetRC( ) == TWRC_CHECKSTATUS )
      {
         TW_UINT32 uCount;

         if ( GetCapability( CAP_XFERCOUNT, uCount ) )
         {
            nCount = (TW_INT16) uCount;
            if ( SetCapability( CAP_XFERCOUNT, nCount ) )
            {
               m_nImageCnt = nCount;
               return( TRUE );
            }
         }
      }
   }

   return( FALSE );
}

// Called to enable the Twain Acquire Dialog. This too can be overridden
// but is a tough job.
zBOOL
ZTwain::EnableSource( zBOOL bShowUI )
{
   if ( DSOpen( ) && m_bSourceEnabled == FALSE )
   {
      TW_USERINTERFACE twUI;
      twUI.ShowUI = bShowUI;
      twUI.hParent = (TW_HANDLE) m_pZFWnd->m_hWnd;
      if ( CallTwainProc( &m_AppId, &m_Source,
                          DG_CONTROL, DAT_USERINTERFACE,
                          MSG_ENABLEDS, (TW_MEMREF) &twUI ) )
      {
         m_bSourceEnabled = TRUE;
         m_bModalUI = (zBOOL) twUI.ModalUI;
      }
      else
      {
         m_bSourceEnabled = FALSE;
         m_bModalUI = TRUE;
      }

      return( m_bSourceEnabled );
   }

   return( FALSE );
}

// Called to acquire images from the source.  Parameter nNbrImages is the
// number of images that you an handle concurrently.
zBOOL
ZTwain::Acquire( int nNbrImages )
{
   if ( DSOpen( ) || OpenSource( ) )
   {
      if ( SetImageCount( nNbrImages ) )
      {
         if ( EnableSource( ) )
         {
            return( TRUE );
         }
      }
   }

   return( FALSE );
}

// Called to disable the source.
zBOOL
ZTwain::DisableSource( )
{
   if ( m_bSourceEnabled )
   {
      TW_USERINTERFACE twUI;

      if ( CallTwainProc( &m_AppId, &m_Source,
                          DG_CONTROL, DAT_USERINTERFACE,
                          MSG_DISABLEDS, &twUI ) )
      {
         m_bSourceEnabled = FALSE;
         return( TRUE );
      }
   }

   return( FALSE );
}

// Called by ProcessMessage to Translate a TWAIN message.
void
ZTwain::TranslateMessage( TW_EVENT& twEvent )
{
   switch ( twEvent.TWMessage )
   {
      case MSG_XFERREADY:
      {
         TransferImage( );
         break;
      }

      case MSG_CLOSEDSREQ:
      {
         if ( CanClose( ) )
            CloseDS( );

         break;
      }
   }
}

zBOOL
ZTwain::SourceSelected( ) const
{
   return( m_bSourceSelected );
}

zBOOL
ZTwain::ModalUI( ) const
{
   return( m_bModalUI );
}

TW_INT16
ZTwain::GetRC( ) const
{
   return( m_nRC );
}

TW_STATUS
ZTwain::GetStatus( ) const
{
   return( m_Status );
}

zBOOL
ZTwain::CanClose( )
{
   return( TRUE );
}

// Gets Imageinfo for an image which is about to be transferred.
zBOOL
ZTwain::GetImageInfo( TW_IMAGEINFO& info )
{
   if ( m_bSourceEnabled )
   {
      return( CallTwainProc( &m_AppId, &m_Source,
                             DG_IMAGE, DAT_IMAGEINFO,
                             MSG_GET, (TW_MEMREF) &info ) );
   }

   return( FALSE );
}

int
ZTwain::ShouldTransfer( TW_IMAGEINFO& info )
{
   return( TWCPP_DOTRANSFER );
}

// Trasnfers the image or cancels the transfer depending on the state of
// the TWAIN system.
void
ZTwain::TransferImage( )
{
   TW_IMAGEINFO info;
   zBOOL bContinue = TRUE;

   while ( bContinue )
   {
      if ( GetImageInfo( info ) )
      {
         int nPermission = ShouldTransfer( info );
         switch ( nPermission )
         {
            case TWCPP_CANCELTHIS:
               bContinue = EndTransfer( );
               break;

            case TWCPP_CANCELALL:
               CancelTransfer( );
               bContinue = FALSE;
               break;

            case TWCPP_DOTRANSFER:
               bContinue = GetImage( info );
               break;
         }
      }
   }
}

// Ends the current transfer.  Returns TRUE if the more images are pending.
zBOOL
ZTwain::EndTransfer( )
{
   TW_PENDINGXFERS twPend;

   if ( CallTwainProc( &m_AppId, &m_Source, DG_CONTROL, DAT_PENDINGXFERS,
                       MSG_ENDXFER, (TW_MEMREF) &twPend ) )
   {
      return( twPend.Count != 0 );
   }

   return( FALSE );
}

// Aborts all transfers.
void
ZTwain::CancelTransfer( )
{
   TW_PENDINGXFERS twPend;

   CallTwainProc( &m_AppId, &m_Source, DG_CONTROL, DAT_PENDINGXFERS,
                  MSG_RESET, (TW_MEMREF) &twPend );
}

// Calls TWAIN to actually get the image.
zBOOL
ZTwain::GetImage( TW_IMAGEINFO& info )
{
   HANDLE hBitmap;

   CallTwainProc( &m_AppId, &m_Source,
                  DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET,
                  &hBitmap );
   switch ( m_nRC )
   {
      case TWRC_XFERDONE:
      {
         CopyImage( hBitmap, info );
         break;
      }

      case TWRC_CANCEL:
      {
         break;
      }

      case TWRC_FAILURE:
      {
         CancelTransfer( );
         return( FALSE );
      }
   }

   GlobalFree( hBitmap );
   return( EndTransfer( ) );
}
