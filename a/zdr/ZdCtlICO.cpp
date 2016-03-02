/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlico.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Icon control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.10.22   FH
//    set szImageURL to lower case
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.05.31    DKS   R55199
//    Attempt to load icon from source when running interpretively.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2000.03.17    TMV   WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 1999.05.11    DKS   Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.02.16    DKS
//    Used new option zLOADLIB_RESOURCES on SysLoadLibraryWithErrFlag.
//    Fixed icon lookup when using file as source of icon load.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlIco.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( ZIconCtl, CWnd )

BEGIN_MESSAGE_MAP( ZIconCtl, CWnd )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_RBUTTONDBLCLK( )
   ON_WM_PAINT( )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )

zSHORT ZIconCtl::m_nIconPixelSizeX = ::GetSystemMetrics( SM_CXICON );
zSHORT ZIconCtl::m_nIconPixelSizeY = ::GetSystemMetrics( SM_CYICON );
zSHORT ZIconCtl::m_nIconEdgeX = ::GetSystemMetrics( SM_CXEDGE );
zSHORT ZIconCtl::m_nIconEdgeY = ::GetSystemMetrics( SM_CYEDGE );

// ZIconCtl - ctor
ZIconCtl::ZIconCtl( ZSubtask *pZSubtask,
                    CWnd     *pWndParent,
                    ZMapAct  *pzmaComposite,
                    zVIEW    vDialog,
                    zSHORT   nOffsetX,
                    zSHORT   nOffsetY,
                    zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
          CWnd( ),
          ZMapAct( pZSubtask,                // base class ctor
                   pzmaComposite,
                   pWndParent,
                   this,
                   vDialog,
                   nOffsetX,
                   nOffsetY,
                   pCtrlDef,
                   "Icon" )
{
   m_bFocus      = FALSE;
   m_nLDblClk    = 0;
   m_nRDblClk    = 0;

   m_hIcon = 0;
   m_nIconX = 0;
   m_nIconY = 0;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

#if 1

   Attr.W = m_nIconPixelSizeX + 2 * m_nIconEdgeX;
   Attr.H = m_nIconPixelSizeY + 2 * m_nIconEdgeY;

#else
   if ( pCtrlDef->Text[ 0 ] == 0 ||
        (Attr.W - (ZIconCtl::m_nIconPixelSizeX + m_nIconEdgeX)) >
                                  (Attr.H - ZIconCtl::m_nIconPixelSizeY) )
   {
      if ( Attr.H > ZIconCtl::m_nIconPixelSizeY )
      {
         m_nIconY = Attr.H - ZIconCtl::m_nIconPixelSizeY;
         m_nIconY /= 2;
      }

      m_rectIconText.left = ZIconCtl::m_nIconPixelSizeX + (2 * m_nIconEdgeX);
      m_rectIconText.top = 0;
      m_rectIconText.right = Attr.W;
      m_rectIconText.bottom = Attr.H;
   }

   if ( pCtrlDef->Text[ 0 ] == 0 ||
        (Attr.W - (ZIconCtl::m_nIconPixelSizeX + m_nIconEdgeX)) <=
                                  (Attr.H - ZIconCtl::m_nIconPixelSizeY) )
   {
      m_nIconX = 0;
      if ( Attr.W > ZIconCtl::m_nIconPixelSizeX )
      {
         m_nIconX = Attr.W - ZIconCtl::m_nIconPixelSizeX;
         m_nIconX /= 2;
      }

      m_rectIconText.left = 0;
      m_rectIconText.top = ZIconCtl::m_nIconPixelSizeY + m_nIconEdgeY;
      m_rectIconText.right = Attr.W;
      m_rectIconText.bottom = Attr.H;
   }
#endif

   if ( pCtrlDef->Text[ 0 ] )
      m_csText = pCtrlDef->Text;
   else
      m_csText = " ";

   zPCHAR pch = (zPCHAR) pCtrlDef->CtrlBOI;
   if ( pch == 0 )      // there is no CtrlBOI
      return;

#ifdef zREMOTE_SERVER

   ZXRA_Item *pXRA;

   if ( m_pZSubtask->m_pZSocketTask &&
        (pXRA = m_pZSubtask->m_pZSocketTask->
            m_pCacheWndList->Find( *(m_pZSubtask->m_pzsRemoteWndTag),
                                   m_pZSubtask,
                                   "ZIconCtl::ZIconCtl" )) != 0 &&
        pXRA->m_vXRA )
   {
      zVIEW  vXRA;
      zCHAR  szImageURL[ zMAX_FILENAME_LTH ];
      zSHORT nLth;

      fnGetImageURL( m_pZSubtask, szImageURL, zsizeof( szImageURL ) );
      nLth = zstrlen( szImageURL );

      CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA,
                                 m_pZSubtask->m_vDialog );
      while ( ResetViewFromSubobject( vXRA ) == 0 )
      {
      }

      if ( PositionOnRemoteCtrl( this, vXRA, FALSE ) )
      {
         GetStringFromAttribute( szImageURL + nLth, vXRA, "Ctrl", "Fix" );
         if ( szImageURL[ nLth ] != 'Y' )
         {
            SetRemoteZCtrlAttribute( this, "Ctrl", "Fix", "Y" );
            GetStringFromAttribute( szImageURL + nLth, vXRA,
                                    "Icon", "URL" );
            SysTranslateString( szImageURL, 'L' );
            SetRemoteZCtrlAttribute( this, "Icon", "URL", szImageURL );
         }
      }
   }

   CreateZ( );

#else

/////////////////////////////////////////////////////////////////////////////
//
// The LoadImage function loads an icon, cursor, or bitmap.
//
// HANDLE
// LoadImage( HINSTANCE hinst,  // the instance that contains the image
//            LPCTSTR lpszName, // name or identifier of image
//            UINT uType,       // type of image
//            int cxDesired,    // desired width
//            int cyDesired,    // desired height
//            UINT fuLoad       // load flags );
//
// Parameters
//
// hinst - Identifies an instance of the module that contains the image to
//         be loaded.  To load an OEM image, set this parameter to zero.
// lpszName - Identifies the image to load.
//
// If the hinst parameter is non-NULL and the fuLoad parameter does not
// include LR_LOADFROMFILE, lpszName is a pointer to a null-terminated
// string that contains the name of the image resource in the hinst module.
//
// If hinst is NULL and LR_LOADFROMFILE is not specified, the low-order word
// of this parameter must be the identifier of the OEM image to load.  The OEM
// image identifiers are defined in WINUSER.H and have the following prefixes:
//
// Prefix Meaning
//
//    OBM_  - OEM bitmaps
//    OIC_  - OEM icons
//    OCR_  - OEM cursors
//
// Windows 95: If the fuLoad parameter includes the LR_LOADFROMFILE value,
// lpszName is the name of the file that contains the image.
//
// Windows NT: LR_LOADFROMFILE is not supported.
//
// uType - Specifies the type of image to be loaded.  This parameter can be
// one of the following values:
//    IMAGE_BITMAP - Loads a bitmap.
//    IMAGE_CURSOR - Loads a cursor.
//    IMAGE_ICON - Loads an icon.
//
// cxDesired - Specifies the width, in pixels, of the icon or cursor.  If
// this parameter is zero and the fuLoad parameter is LR_DEFAULTSIZE, the
// function uses the SM_CXICON or SM_CXCURSOR system metric value to set
// the width.  If this parameter is zero and LR_DEFAULTSIZE is not used,
// the function uses the actual resource width.
//
// cyDesired - Specifies the height, in pixels, of the icon or cursor. If
// this parameter is zero and the fuLoad parameter is LR_DEFAULTSIZE, the
// function uses the SM_CYICON or SM_CYCURSOR system metric value to set
// the height.  If this parameter is zero and LR_DEFAULTSIZE is not used,
// the function uses the actual resource height.
//
// fuLoad - Specifies a combination of the following values:
//
// LR_DEFAULTCOLOR - The default flag; it does nothing.  All it means is
//    "not LR_MONOCHROME".
//
// LR_CREATEDIBSECTION - When the uType parameter specifies IMAGE_BITMAP,
//    causes the function to return a DIB section bitmap rather than a
//    compatible bitmap.  This flag is useful for loading a bitmap without
//    mapping it to the colors of the display device.
//
// LR_DEFAULTSIZE - Uses the width or height specified by the system metric
//    values for cursors or icons, if the cxDesired or cyDesired values are
//    set to zero.  If this flag is not specified and cxDesired and cyDesired
//    are set to zero, the function uses the actual resource size.  If the
//    resource contains multiple images, the function uses the size of the
//    first image.
//
// LR_LOADFROMFILE - Loads the image from the file specified by the lpszName
//    parameter.  If this flag is not specified, lpszName is the name of the
//    resource.
//
// LR_LOADMAP3DCOLORS - Searches the color table for the image and replaces
//    the following shades of gray with the corresponding 3D color:
//
//    Color Replaced with Dk Gray, RGB(128,128,128)
//    COLOR_3DSHADOW - Gray, RGB(192,192,192)
//    COLOR_3DFACE - Lt Gray, RGB(223,223,223)
//    COLOR_3DLIGHT - LR_LOADTRANSPARENT
//
// Retrieves the color value of the first pixel in the image and replaces
// the corresponding entry in the color table with the default window color
// (COLOR_WINDOW).  All pixels in the image that use that entry become the
// default window color.  This value applies only to images that have
// corresponding color tables.
//
// If fuLoad includes both the LR_LOADTRANSPARENT and LR_LOADMAP3DCOLORS
// values, LRLOADTRANSPARENT takes precedence. However, the color table
// entry is replaced with COLOR_3DFACE rather than COLOR_WINDOW.
//
// LR_MONOCHROME - Loads the image in black and white.
//
// LR_SHARED - Shares the image handle if the image is loaded multiple times.
//    If LR_SHARED is not set, a second call to LoadImage for the same
//    resource will load the image again and return a different handle.
//
// Do not use LR_SHARED for images that have non-standard sizes, that may
//    change after loading or that are loaded from a file.
//
// Return Values - If the function succeeds, the return value is the handle
//    of the newly loaded image.  If the function fails, the return value is
//    NULL.
//
/////////////////////////////////////////////////////////////////////////////

   LPLIBRARY hLibrary;
   HINSTANCE hInstDLL = 0;

   if ( m_pZSubtask->m_vInterpSubtask )
   {
      // Do not even try to load the bitmap from the DLL if running
      // interpretively.
   }
   else
   if ( *pch )   // DLL_Name
   {
      if ( m_pZSubtask->m_pZTask->m_uAppState & zAPP_STATE_NT )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog, pch,
                                               zLOADLIB_RESOURCES );
         hInstDLL = (HINSTANCE) SysGetInstance( hLibrary );
      }
      else
      {
         zFARPROC_HINSTANCE lpfnDynRoutine;

         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               pch, 0 );
         lpfnDynRoutine = (zFARPROC_HINSTANCE)
            GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                            (LPLPLIBRARY) &hLibrary,
                                            pch, "GetDLL_Instance",
                                            0 );  // no error message
   //                                       "(ZBmpCtrl)" );
         if ( lpfnDynRoutine )
            hInstDLL = (*lpfnDynRoutine)( m_pZSubtask->m_vDialog );
      }

      pch += zstrlen( pch ) + 1;  // skip past DLL name
   }
   else
   {
      zPCHAR pchDLL_Name;

      hLibrary = 0;
      pch++;      // skip past null DLL name

      // Get the DLL name.
      GetAddrForAttribute( (zPCHAR *) &pchDLL_Name, m_pZSubtask->m_vDialog,
                           "Dlg", "DLL" );
      if ( *pchDLL_Name )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               pchDLL_Name,
                                               zLOADLIB_RESOURCES );
         hInstDLL = (HINSTANCE) SysGetInstance( hLibrary );
      }
   }

   zPCHAR pchSemiColon = zstrchr( pch, ';' );
   if ( pch && *pch )
   {
      UINT uResId;

      if ( hInstDLL &&
           (isdigit( *pch ) || (pchSemiColon != 0)) &&
           (uResId = (zUSHORT) zatol( pch )) != 0 )
      {
      // m_hIcon = ::LoadIcon( hInstDLL, MAKEINTRESOURCE( uResId ) );
         m_hIcon = (HICON) ::LoadImage( hInstDLL, MAKEINTRESOURCE( uResId ),
                                        IMAGE_ICON, 0, 0, 0 );
      }

      if ( pchSemiColon )
         pch = pchSemiColon + 1;

      if ( m_hIcon == 0 || m_hIcon == (HICON) 1 )
      {
         if ( *pch == '.' && *(pch + 1) == '\\' )
            pch += 2;

         m_hIcon = ::ExtractIcon( hInstDLL, pch, 0 );
      }

      if ( m_hIcon == 0 || m_hIcon == (HICON) 1 )
      {
         zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
         zCHAR  szWorkString[ zMAX_FILENAME_LTH ];
         LPAPP pApp;

         if ( SfGetApplicationForSubtask( &pApp,
                                          m_pZSubtask->m_vDialog ) == 0 &&
              pApp )
         {
            strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );
            if ( szZeidonPath[ 0 ] )
            {
               SysAppendcDirSep( szZeidonPath );
               strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pch );
               SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
               HICON hIcon = ::ExtractIcon( hInstDLL, szWorkString, 0 );
               if ( hIcon && hIcon != (HICON) 1 )
                  m_hIcon = hIcon;
            }
         }

         if ( m_hIcon == 0 || m_hIcon == (HICON) 1 )
         {
            SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, zsizeof( szZeidonPath ) );
            if ( szZeidonPath[ 0 ] )
            {
               SysAppendcDirSep( szZeidonPath );
               strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pch );
               SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
               HICON hIcon = ::ExtractIcon( hInstDLL, szWorkString, 0 );
               if ( hIcon && hIcon != (HICON) 1 )
                  m_hIcon = hIcon;
            }
         }

         if ( m_hIcon == (HICON) 1 )
         {
            m_hIcon = 0;
            if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
               TraceLineS( "Invalid icon file ", pch );
         }
         else
         if ( m_hIcon == 0 )
         {
            if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
               TraceLineS( "Cannot access icon file ", pch );
         }
      }

      if ( pzmaComposite &&
           (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) == 0 )
      {
         m_ulMapActFlag2 &= ~(zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
      }
      else
      {
         m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
         CreateZ( );
      }
   }
   else
      m_ulMapActFlag2 &= ~(zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);

#endif
}

void
ZIconCtl::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( WS_EX_WINDOWEDGE, "ZeidonCustomCtrl", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZIconCtl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   // if we do not call the base class'OnCreate, the client window's
   // OnCreate( LPCREATESTRUCT lpCreateStruct ) method will not be called.
   int nRC = CWnd::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   return( nRC );
}

BOOL
ZIconCtl::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

zSHORT
ZIconCtl::MapFromOI( WPARAM wFlag )
{
// TraceLineS( "ZIconCtl::MapFromOI ", *m_pzsTag );

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   return( 0 );
}

void
ZIconCtl::OnKeyDown( UINT key, UINT uRepeatCnt, UINT uFlags )
{
   return;
}

void
ZIconCtl::OnKeyUp( UINT key, UINT uRepeatCnt, UINT uFlags )
{
   return;
}

void
ZIconCtl::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pZSubtask->InformDriverOfSetFocus( this, TRUE ) )
   {
      m_bFocus = TRUE;
      Default( );
      if ( m_csText[ 0 ] )
         InvalidateRect( m_rectIconText, 1 );
      else
         Invalidate( 1 );

      UpdateWindow( );

      // Send a GetFocus Event
      ProcessEvent( this, 5 );
   }
}

void
ZIconCtl::OnKillFocus( CWnd *pWndGetFocus )
{
   m_bFocus = FALSE;
   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );
   if ( m_csText[ 0 ] )
      InvalidateRect( m_rectIconText, 1 );
   else
      Invalidate( 1 );

   UpdateWindow( );
}

zBOOL
ZIconCtl::IsDoubleClick( zUSHORT uMessage, CPoint ptStart )
{
   MSG     msg;
   zULONG  ulInterval = 0;
   zLONG   lMessage = GetMessageTime( );
   zULONG  ulDblclkTime = GetDoubleClickTime( );

#ifdef DEBUG_ALL
   TraceLineI( "ZIconCtl::IsDoubleClick Time: ", ulDblclkTime );
   TraceLineI( "                 MessageTime: ", lMessage );
#endif

   while ( ulInterval < ulDblclkTime )
   {
      ulInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage,
                          PM_NOREMOVE | PM_NOYIELD ) )
      {
      // TraceLineI( "ZIconCtl::IsDoubleClick TRUE: ", ulInterval );
         return( TRUE );
      }
      else
      if ( ::PeekMessage( &msg, m_hWnd, WM_MOUSEMOVE, WM_MOUSEMOVE,
                          PM_REMOVE | PM_NOYIELD ) )
      {
         CPoint pt( msg.lParam );
         if ( ptStart.x - pt.x > g_lDblClkX ||
              pt.x - ptStart.x > g_lDblClkX ||
              ptStart.y - pt.y > g_lDblClkY ||
              pt.y - ptStart.y > g_lDblClkY )
         {
         // TracePoint( "ZIconCtl::IsDoubleClick FALSE: ", pt );
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

void
ZIconCtl::OnLButtonDown( UINT uModKeys, CPoint pt )
{
   if ( m_bFocus == FALSE )
      SetFocus( );

   if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) )
   {
      m_nLDblClk = 1;
      return;
   }
}

void
ZIconCtl::OnLButtonUp( UINT uModKeys, CPoint pt )
{
   if ( m_nLDblClk )
   {
      if ( m_nLDblClk == 1 )
         m_nLDblClk = 2;
      else
      {
         m_nLDblClk = 0;
// TraceLineS("(kzdcicaa) Processing Event ", "2" );
         ProcessEvent( this, 2 );
      }
   }
   else
   {
// TraceLineS("(kzdcicaa) Processing Event ", "1" );
      ProcessEvent( this, 1 );
   }
}

void
ZIconCtl::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
}

void
ZIconCtl::OnRButtonDown( UINT uModKeys, CPoint pt )
{
   if ( m_bFocus == FALSE )
      SetFocus( );

   if ( IsDoubleClick( WM_RBUTTONDBLCLK, pt ) )
   {
      m_nRDblClk = 1;
      return;
   }
}

void
ZIconCtl::OnRButtonUp( UINT uModKeys, CPoint pt )
{
   if ( m_nRDblClk )
   {
      if ( m_nRDblClk == 1 )
         m_nRDblClk = 2;
      else
      {
         m_nRDblClk = 0;
// TraceLineS("(kzdcicaa) Processing Event ", "4" );
         ProcessEvent( this, 4 );
      }
   }
   else
   {
// TraceLineS("(kzdcicaa) Processing Event ", "3" );
      ProcessEvent( this, 3 );
   }
}

void
ZIconCtl::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
}

void
ZIconCtl::OnPaint( )
{
   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
// int nOldBkMode = dc.SetBkMode( TRANSPARENT );
   CRect rect;

// GetWindowRect( rect );
// TraceRect( "ZIconCtrl::OnPaint Window rect", rect );
   GetUpdateRect( rect, 1 );
// TraceRect( "ZIconCtrl::OnPaint Update rect", rect );
// CBrush brush( dc.GetBkColor( ) );
   CBrush brush( COLOR_BTNFACE );
   dc.FillRect( rect, &brush );

   if ( m_hIcon )
   {
      dc.DrawIcon( rect.left + m_nIconEdgeX,
                   rect.top + m_nIconEdgeY, m_hIcon );
   }

#if 0
   if ( m_csText[ 0 ] && m_rectIconText.bottom > m_rectIconText.top )
   {
      LOGFONT IconFont;

      ::SystemParametersInfo( SPI_GETICONTITLELOGFONT,
                              sizeof( LOGFONT ), &IconFont, 0 );
      CFont fontIcon;
      fontIcon.CreateFontIndirect( &IconFont );
      CFont *pFontOld = dc.SelectObject( &fontIcon );

      if ( m_rectIconText.left )
         dc.DrawText( m_csText, -1, m_rectIconText,
                      DT_SINGLELINE | DT_VCENTER );
      else
         dc.DrawText( m_csText, -1, m_rectIconText,
                      DT_CENTER | DT_WORDBREAK );

      dc.SelectObject( pFontOld );
   }
   else
   if ( m_bFocus )
   {
      CPen pen( PS_SOLID, ::GetSystemMetrics( SM_CXBORDER ),
                RGB( 255, 255, 255 ) );
      CPen *pPenOld = dc.SelectObject( &pen );
      dc.MoveTo( 0, 0 );
      dc.LineTo( Attr.W - 1, 0 );
      dc.LineTo( Attr.W - 1, Attr.H - 1 );
      dc.LineTo( 0, Attr.H - 1 );
      dc.LineTo( 0, 0 );
      dc.SelectObject( pPenOld );
   }
#endif

// dc.SetBkMode( nOldBkMode );
}

void
ZIconCtl::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZIconCtl - dtor
ZIconCtl::~ZIconCtl( )
{
// TraceLineS( "ZIconCtl::~ZIconCtl", "" );

   if ( m_hIcon )
      ::DestroyIcon( m_hIcon );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
IconCtl( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZIconCtl( pZSubtask, pWndParent,
                         pzmaComposite, vDialog,
                         nOffsetX, nOffsetY, pCtrlDef ) );
}
