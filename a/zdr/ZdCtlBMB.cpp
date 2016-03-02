/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlbmb.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Bitmap Button control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.28    HH
//    Improve setting of image url for http.
//
// 2002.03.18   TMV/DKS
//    Add code to load bitmaps from files if DLL load fails R056363.
//
// 2001.10.22   FH
//    Set szImageURL to lower case.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.05.31    DKS   R55199
//    Attempt to load bitmaps from source when running interpretively.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2000.03.17    TMV   WEB
//    Validate parent window's handle prior to call Create/CreateEx.
//
// 1999.05.11    DKS   Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.02.16    DKS
//    Used new option zLOADLIB_RESOURCES on SysLoadLibraryWithErrFlag.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlBmb.h"
#include "ZdCtlGbl.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define zBUTTON_HOVER                 zCONTROL_SPECIFIC_2

IMPLEMENT_DYNAMIC( ZBitmapButton, CBitmapButton )

BEGIN_MESSAGE_MAP( ZBitmapButton, CBitmapButton )
   ON_WM_CREATE( )
   ON_CONTROL_REFLECT( BN_CLICKED, BMClicked )
   ON_CONTROL_REFLECT( BN_DISABLE, BMDisable )
   ON_CONTROL_REFLECT( BN_DOUBLECLICKED, BMDoubleClicked )
   ON_CONTROL_REFLECT( BN_HILITE, BMHilite )
   ON_CONTROL_REFLECT( BN_PAINT, BMPaint )
   ON_CONTROL_REFLECT( BN_UNHILITE, BMUnhilite )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_MOUSEMOVE( )
   ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave )
   ON_MESSAGE( WM_MOUSEHOVER, OnMouseHover )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZBitmapButton

// ZBitmapButton - ctor
ZBitmapButton::ZBitmapButton( ZSubtask *pZSubtask,
                              CWnd     *pWndParent,
                              ZMapAct  *pzmaComposite,
                              zVIEW    vDialog,
                              zSHORT   nOffsetX,
                              zSHORT   nOffsetY,
                              zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
               CBitmapButton( ),
               ZMapAct( pZSubtask,                 // base class ctor
                        pzmaComposite,
                        pWndParent,
                        this,
                        vDialog,
                        nOffsetX,
                        nOffsetY,
                        pCtrlDef,
                        "BitmapBtn" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZBitmapButton::ctor ", *m_pzsTag );
#endif

   m_ulLth = 0;
   m_pDib = 0;
   m_pDibUP = 0;  // normal (up)
   m_pDibDN = 0;  // selected (down)
   m_pDibFO = 0;  // focus
   m_pDibDA = 0;  // disabled
   m_pchCtrlBOI = 0;
   m_pBMPINFO = 0;
   m_pchMsgText = 0;

   m_bHover = FALSE;
   m_bTracking = FALSE;

   GetAttributeLength( &m_ulLth, vDialog, szlCtrl, szlCtrlBOI );
   if ( m_ulLth == 0 )   // there is no CtrlBOI
      return;

   m_pchCtrlBOI = new char[ m_ulLth ];
   zmemcpy( m_pchCtrlBOI, (zPVOID) pCtrlDef->CtrlBOI, m_ulLth );

   m_ulLth = zstrlen( pCtrlDef->Text ) + 1;
   m_pchMsgText = new char[ m_ulLth ];
   strcpy_s( m_pchMsgText, m_ulLth, pCtrlDef->Text );

   // determine if this button is the default pushbutton for this window
   if ( CompareAttributeToAttribute( pZSubtask->m_vDialog, szlWnd,
                                     szlDfltBtn, vDialog, szlCtrl, szlTag ) == 0 )
   {
      pZSubtask->m_pDfltBtn = this;            // Zeidon default pushbutton
      pZSubtask->m_pCurrDfltBtn = this;
      Attr.Style |= BS_DEFPUSHBUTTON;
   }

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   Attr.Style |= BS_OWNERDRAW;

   if ( pCtrlDef->Subtype & zBUTTON_HOVER )
      m_bHoverType = TRUE;
   else
      m_bHoverType = FALSE;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZBitmapButton::CreateZ( )
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
   LoadBitmaps( );
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", *m_pzsText,
                   BS_BITMAP | Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

void
ZBitmapButton::SplitCtrlBOI_BMP( )
{
   // Use global op to do it.
   ::SplitCtrlBOI_BMP( m_pchCtrlBOI, m_ulLth, m_pBMPINFO );
}

// position after the directory path information of image file
zPCHAR
StripDir( zCPCHAR cpcImage )
{
   // Find last backslash.
   zPCHAR pch = zstrrchr( cpcImage, '\\' );
   if ( pch == 0 )
      return( (zPCHAR) cpcImage );
   else
      return( pch + 1 );
}

HINSTANCE
ZBitmapButton::GetCorrectResourceHandle( zPCHAR *ppch )
{
   LPLIBRARY hLibrary;
   HINSTANCE hInst = 0;
   zPCHAR    pch = *ppch;

   if ( m_pZSubtask->m_vInterpSubtask )
   {
      // Do not even try to load the bitmap from the DLL if running
      // interpretively.
   }
   else
   if ( pch && *pch )   // DLL_Name
   {
      if ( m_pZSubtask->m_pZTask->m_uAppState & zAPP_STATE_NT )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog, pch,
                                               zLOADLIB_RESOURCES );
         hInst = (HINSTANCE) SysGetInstance( hLibrary );
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
            hInst = (*lpfnDynRoutine)( m_pZSubtask->m_vDialog );
      }

      pch += zstrlen( pch ) + 1;  // skip past DLL name
   }
   else
   {
      zPCHAR pchDLL_Name;

      hLibrary = 0;
      if ( pch )
         pch++;      // skip past null DLL name

      // Get the DLL name.
      GetAddrForAttribute( (zPCHAR *) &pchDLL_Name, m_pZSubtask->m_vDialog,
                           "Dlg", "DLL" );
      if ( *pchDLL_Name )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               pchDLL_Name,
                                               zLOADLIB_RESOURCES );
         hInst = (HINSTANCE) SysGetInstance( hLibrary );
      }
   }

   *ppch = pch;
   return( hInst );
}

// Load a bitmap from the resources in the button ... the bitmap has to
// have 3 button states next to each other: Up/Down/Hover.
BOOL
ZBitmapButton::LoadBitmap( )
{
   zPCHAR    pchBitmap = m_pchCtrlBOI;
   HINSTANCE hInstDLL = GetCorrectResourceHandle( &pchBitmap );

   if ( hInstDLL == 0 || pchBitmap == 0 || *pchBitmap == 0 )  // nothing left in the Ctrl BOI
   {
      TraceLineS( "ZBitmapButton::LoadBitmap unable to load hover bitmap", "" );
      return( FALSE );
   }

   zPCHAR pchSemiColon = zstrchr( pchBitmap, ';' );
   zPCHAR pchReturn = pchBitmap + zstrlen( pchBitmap ) + 1;
   UINT   uBitmapId = 0;

   if ( pchSemiColon )
      pchSemiColon++;
   else
      pchSemiColon = pchBitmap;

   if ( (isdigit( *pchBitmap ) || (pchSemiColon != 0)) &&
        (uBitmapId = (zUSHORT) zatol( pchSemiColon )) != 0 )
   {
      HANDLE hImage = ::LoadImage( hInstDLL,
                                   MAKEINTRESOURCE( uBitmapId ),
                                   IMAGE_BITMAP, 0, 0,
                                   LR_LOADMAP3DCOLORS );
      if ( hImage )
      {
         m_bitmap.Attach( hImage );
         BITMAP   bitmapbits;
         m_bitmap.GetBitmap( &bitmapbits );
         m_ButtonSize.cy = bitmapbits.bmHeight;
         m_ButtonSize.cx = bitmapbits.bmWidth / 3;
         SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER );
         return( TRUE );
      }
   }

   TraceLineI( "ZBitmapButton::LoadBitmap unable to load hover bitmap Id: ", uBitmapId );
   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
//
// Load a bitmap from a CtrlBOI according to the following rules:
//    1. if there is a res id (and hDLL_Library is non-zero) attempt to
//       load the bitmap using the res id from the DLL.  If successful,
//       quit; otherwise go to rule 2.
//    2. if there is a name (and hDLL_Library is non-zero) attempt to load
//       the bitmap using the resource name from the DLL.  If successful,
//       quit; otherwise, go to rule 3.
//    3. attempt to load the bitmap assuming the name is a fully qualified
//       bitmap file name.
//
//    If successful, m_pDib will be non-zero.  The return pointer is
//    the pointer to the next bitmap specification in the Ctrl BOI
//
/////////////////////////////////////////////////////////////////////////////
void
ZBitmapButton::LoadBitmaps( )
{
#ifdef zREMOTE_SERVER

   ZXRA_Item *pXRA;

   if ( m_pZSubtask->m_pZSocketTask &&
        (pXRA = m_pZSubtask->m_pZSocketTask->
         m_pCacheWndList->Find( *(m_pZSubtask->m_pzsRemoteWndTag),
                                m_pZSubtask,
                                "ZBitmapButton::LoadBitmaps" )) != 0 &&
        pXRA->m_vXRA )
   {
      zVIEW  vXRA;
      zCHAR  szImageURL[ zMAX_FILENAME_LTH ];
      zCHAR  szImageUp[ zMAX_FILENAME_LTH ];
      zCHAR  szImage[ zMAX_FILENAME_LTH ];
      zSHORT nLth;
      zBOOL  bHTTP = FALSE;

      fnGetImageURL( m_pZSubtask, szImageURL, zsizeof( szImageURL ) );
      nLth = zstrlen( szImageURL );

      if ( zstrncmpi( szImageURL, "http:", 5) == 0 )
         bHTTP = TRUE;

      CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA,
                                 m_pZSubtask->m_vDialog );
      while ( ResetViewFromSubobject( vXRA ) == 0 )
      {
      }

      if ( PositionOnRemoteCtrl( this, vXRA, FALSE ) )
      {
         GetStringFromAttribute( szImageUp, zsizeof( szImageUp ), vXRA, "Ctrl", "Fix" );
         if ( szImageUp[ 0 ] != 'Y' )
         {
            SetRemoteZCtrlAttribute( this, "Ctrl", "Fix", "Y" );

            // handle UP URL, this is the default
            GetStringFromAttribute( szImageUp, zsizeof( szImageUp ), vXRA, "BitmapBtn", "URL_Up" );
            if ( bHTTP )
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, StripDir( szImage ) );
            else
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, szImageUp );

            SysTranslateString( szImageURL, 'L' );
            SetRemoteZCtrlAttribute( this, "BitmapBtn", "URL_Up", szImageURL );
            strcpy_s( szImageUp, zsizeof( szImageUp ), szImageURL ); // save to be used as default

            // handle DOWN URL
            GetStringFromAttribute( szImage, zsizeof( szImage ), vXRA, "BitmapBtn", "URL_Down" );
            if ( szImage[ 0 ] == 0 )
               strcpy_s( szImageURL, zsizeof( szImageURL ), szImageUp );
            else
            if ( bHTTP )
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, StripDir( szImage ) );
            else
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, szImage );

            SysTranslateString( szImageURL, 'L' );
            SetRemoteZCtrlAttribute( this, "BitmapBtn", "URL_Down", szImageURL );

            // handle Focus URL
            GetStringFromAttribute( szImage, zsizeof( szImage ), vXRA, "BitmapBtn", "URL_Focus" );
            if ( szImage[ 0 ] == 0 )
               strcpy_s( szImageURL, zsizeof( szImageURL ), szImageUp );
            else
            if ( bHTTP )
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, StripDir( szImage ) );
            else
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, szImage );

            SysTranslateString( szImageURL, 'L' );
            SetRemoteZCtrlAttribute( this, "BitmapBtn", "URL_Focus", szImageURL );

            // handle Disabled URL
            GetStringFromAttribute( szImage, zsizeof( szImage ), vXRA, "BitmapBtn", "URL_Disabled" );
            if ( szImage[ 0 ] == 0 )
               strcpy_s( szImageURL, zsizeof( szImageURL ), szImageUp );
            else
            if ( bHTTP )
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, StripDir( szImage ) );
            else
               strcpy_s( szImageURL + nLth, zsizeof( szImageURL ) - nLth, szImage );

            SysTranslateString( szImageURL, 'L' );
            SetRemoteZCtrlAttribute( this, "BitmapBtn", "URL_Disabled", szImageURL );
         }
      }
   }

#else

   zPCHAR    pch = m_pchCtrlBOI;
   HINSTANCE hInst = GetCorrectResourceHandle( &pch );

// TraceLineX( "ZBitmapButton::OnCreate hInst: ", (zLONG) hInst );
   pch = LoadDib( hInst, pch );
   if ( m_pDib )  // the first bitmap must be present!
   {
      m_pDibUP = m_pDib;
      pch = LoadDib( hInst, pch );
      if ( m_pDib )
      {
         m_pDibDN = m_pDib;
      }

      pch = LoadDib( hInst, pch );
      if ( m_pDib )
      {
         m_pDibFO = m_pDib;
      }

      pch = LoadDib( hInst, pch );
      if ( m_pDib )
      {
         m_pDibDA = m_pDib;
      }

      m_pDib = m_pDibUP;
   }
   else // load Bitmaps from DLL failed so load it from file
   {
      m_pBMPINFO = new zCTRLBOI_BMP;
      SplitCtrlBOI_BMP( );
      if ( m_pBMPINFO->szBMPUpOrIconName[ 0 ] )
      {
         LoadDib( 0, m_pBMPINFO->szBMPUpOrIconName );
         if ( m_pDib )  // the first bitmap must be present!
         {
            m_pDibUP = m_pDib;
         }
      }

      if ( m_pBMPINFO->szBMPDownName[ 0 ] )
      {
         LoadDib( 0, m_pBMPINFO->szBMPDownName );
         if ( m_pDib )
         {
            m_pDibDN = m_pDib;
         }
      }

      if ( m_pBMPINFO->szBMPDisabledName[ 0 ] )
      {
         LoadDib( 0, m_pBMPINFO->szBMPDisabledName );
         if ( m_pDib )
         {
            m_pDibDA = m_pDib;
         }
      }

      if ( m_pBMPINFO->szBMPFocusName[ 0 ] )
      {
         LoadDib( 0, m_pBMPINFO->szBMPFocusName );
         if ( m_pDib )
         {
            m_pDibFO = m_pDib;
         }
      }

      mDeleteInit( m_pBMPINFO );
      m_pDib = m_pDibUP;
   }
#endif

// mDeleteInitA( m_pchCtrlBOI );
}

int
ZBitmapButton::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZBitmapButton::OnCreate ", *m_pzsTag );
#endif

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   if ( m_bHoverType )
      LoadBitmap( );
   else
      LoadBitmaps( );

   return( nRC );
}

BOOL
ZBitmapButton::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZBitmapButton::DestroyWindow ", *m_pzsTag );
#endif

   if ( m_pZSubtask->m_pDfltBtn == this )
      m_pZSubtask->m_pDfltBtn = 0;

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CBitmapButton::DestroyWindow( ) );
}

zPCHAR
ZBitmapButton::LoadDib( HINSTANCE hInstDLL,
                        zPCHAR    pchBitmap )
{
   if ( pchBitmap == 0 || *pchBitmap == 0 )  // nothing left in the Ctrl BOI
   {
      m_pDib = 0;
      return( 0 );
   }

   zPCHAR pchSemiColon = zstrchr( pchBitmap, ';' );
   zPCHAR pchReturn = pchBitmap + zstrlen( pchBitmap ) + 1;

   if ( (pchReturn - m_pchCtrlBOI) >= (zLONG) m_ulLth )
      pchReturn = 0;

   if ( pchSemiColon )
      pchSemiColon++;
   else
      pchSemiColon = pchBitmap;

   zSHORT nFound = 0;
   UINT   uResId = 0;

   m_pDib = new ZDib;
   while ( hInstDLL )  // purist's goto
   {
      if ( (isdigit( *pchBitmap ) || (pchSemiColon != 0)) &&
           (uResId = (zUSHORT) zatol( pchBitmap )) != 0 )
      {
         if ( m_pDib->Load( hInstDLL, uResId ) )
         {
            nFound = 1;
            break;
         }
      }

      if ( m_pDib->Load( hInstDLL, pchSemiColon ) )
      {
         nFound = 2;
         break;
      }

      break;  // end of:  purist's goto
   }

   if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
   {
      if ( nFound == 0 )
      {
         if ( m_pZSubtask == 0 ||
              (pchSemiColon == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1) )
         {
            TraceLine( "ZBitmapButton::LoadDib: Cannot access bitmap res id: %d - Cannot access resource: %s", uResId, pchSemiColon );
         }
      }
      else
      if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
      {
         if ( nFound == 1 )
         {
            TraceLineI( "ZBitmapButton::LoadDib: Bitmap loaded, res id ", uResId );
         }
         else
      // if ( nFound == 2 )
         {
            TraceLine( "ZBitmapButton::LoadDib: Cannot access bitmap res id: %d - Bitmap loaded from resource: %s", uResId, pchSemiColon );
         }
      }
   }

   if ( nFound )
      return( pchReturn );

   if ( pchSemiColon )
   {
      if ( *pchSemiColon == '.' && *(pchSemiColon + 1) == '\\' )
         pchSemiColon += 2;

      if ( m_pDib->Load( pchSemiColon ) == FALSE &&
           pchSemiColon[ 0 ] && pchSemiColon[ 1 ] != ':' )
      {
         zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
         zCHAR  szWorkString[ zMAX_FILENAME_LTH ];
         LPAPP pApp;

         if ( SfGetApplicationForSubtask( &pApp, m_pZSubtask->m_vDialog ) == 0 && pApp )
         {
            strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );
            if ( szZeidonPath[ 0 ] )
            {
               SysAppendcDirSep( szZeidonPath );
               strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
               SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
               if ( m_pDib->Load( szWorkString ) )
               {
                  if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "ZBitmapButton::LoadDib: Bitmap loaded, file in Application Dir ", szWorkString );
                  }

                  return( pchReturn );
               }

               HBITMAP hBitmap = LoadAnImage( szWorkString );
               if ( hBitmap )
               {
                  m_pDib->Attach( hBitmap );
                  if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "ZBitmapButton::LoadDib: Bitmap loaded from file in Application Dir ", szWorkString );
                  }

                  return( pchReturn );
               }

               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
               {
                  TraceLineS( "ZBitmapButton::LoadDib: Cannot access bitmap file in Application Dir ", szWorkString );
               }
            }
         }

         SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, zsizeof( szZeidonPath ) );
         if ( szZeidonPath[ 0 ] )
         {
            SysAppendcDirSep( szZeidonPath );
            strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
            SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
            if ( m_pDib->Load( szWorkString ) )
            {
               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
               {
                  TraceLineS( "ZBitmapButton::LoadDib: Bitmap loaded, file in ResourcePath ", szWorkString );
               }

               return( pchReturn );
            }

            HBITMAP hBitmap = LoadAnImage( szWorkString );
            if ( hBitmap )
            {
               m_pDib->Attach( hBitmap );
               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
               {
                  TraceLineS( "ZBitmapButton::LoadDib: Bitmap loaded from, file in ResourcePath ", szWorkString );
               }

               return( pchReturn );
            }

            if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
            {
               TraceLineS( "ZBitmapButton::LoadDib: Cannot access bitmap file in ResourcePath ", szWorkString );
            }
         }

         if ( pApp )
         {
            strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szObjectDir );
            if ( szZeidonPath[ 0 ] )
            {
               SysAppendcDirSep( szZeidonPath );
               strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
               SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
               if ( m_pDib->Load( szWorkString ) )
               {
                  if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "ZBitmapButton::LoadDib: Bitmap loaded, file in Object Dir ", szWorkString );
                  }

                  return( pchReturn );
               }

               HBITMAP hBitmap = LoadAnImage( szWorkString );
               if ( hBitmap )
               {
                  m_pDib->Attach( hBitmap );
                  if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "ZBitmapButton::LoadDib: Bitmap loaded from, file in Object Dir ", szWorkString );
                  }

                  return( pchReturn );
               }

               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
               {
                  TraceLineS( "ZBitmapButton::LoadDib: Cannot access bitmap file in Object Dir ", szWorkString );
               }
            }
         }

         mDeleteInit( m_pDib );
         if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
         {
            TraceLineS( "ZBitmapButton::LoadDib: Cannot access bitmap file ", pchSemiColon );
         }
      }
   }

   return( pchReturn );
}

zSHORT
ZBitmapButton::MapFromOI( WPARAM wFlag )
{
// TraceLineS( "ZBitmapButton::MapFromOI ", *m_pzsTag );

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   return( 0 );
}

void
ZBitmapButton::BMClicked( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZBitmapButton::BMClicked ", *m_pzsTag );
#endif

// UpdatePalette( TRUE );
   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      ProcessEvent( this, 1 );
}

void
ZBitmapButton::BMDisable( )
{
   Default( );             // pass it along to parent
}

void
ZBitmapButton::BMDoubleClicked( )
{
   Default( );             // pass it along to parent
}

void
ZBitmapButton::BMHilite( )
{
   Default( );             // pass it along to parent
}

void
ZBitmapButton::BMPaint( )
{
   Default( );             // pass it along to parent
}

void
ZBitmapButton::BMUnhilite( )
{
   Default( );             // pass it along to parent
}

void
ZBitmapButton::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pZSubtask->InformDriverOfSetFocus( this, TRUE ) )
   {
      m_pZSubtask->m_pCurrDfltBtn = this;
      Default( );             // pass it along to parent
   }
}

void
ZBitmapButton::OnKillFocus( CWnd *pWndGetFocus )
{
   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

//
// DrawItem - virtual method overload required for all owner draw buttons,
//            this method draws the current button and de-selects the
//            currently selected modal button if the current button is
//            being selected (pushed) and is a modal button.
//
void
ZBitmapButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
   if ( m_bHoverType )
   {
      // TODO: Add your code to draw the specified item

      CDC *mydc = CDC::FromHandle( lpDrawItemStruct->hDC );

      CDC *pMemDC = new CDC;
      pMemDC->CreateCompatibleDC( mydc );

      CBitmap *pOldBitmap;
      pOldBitmap = pMemDC->SelectObject( &m_bitmap );

      CPoint point( 0, 0 );

      if ( lpDrawItemStruct->itemState & ODS_SELECTED )
      {
         mydc->BitBlt( 0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx, 0, SRCCOPY );
      }
      else
      {
         if ( m_bHover )
         {
            mydc->BitBlt( 0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx * 2, 0, SRCCOPY );
         }
         else
         {
            mydc->BitBlt( 0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, 0, 0, SRCCOPY );
         }
      }

      // clean up
      pMemDC->SelectObject( pOldBitmap );
      delete( pMemDC );
   }
   else
   {
      // If we are not going to draw anything, return.
      if ( !(lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT | ODA_FOCUS)) )
      {
         return;
      }

      m_pDib = m_pDibUP;
      if ( (lpDrawItemStruct->itemState & ODS_DISABLED) == ODS_DISABLED )
      {
         if ( m_pDibDA )
            m_pDib = m_pDibDA;
      }
      else
      if ( (lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED )
      {
         if ( m_pDibDN )
            m_pDib = m_pDibDN;
      }
      else
      if ( (lpDrawItemStruct->itemState & ODS_FOCUS) == ODS_FOCUS )
      {
         if ( m_pDibFO )
            m_pDib = m_pDibFO;
      }

      if ( m_pDib == 0 )
         return;

      // Now that we have determined which state to draw in, draw the button.

      // Get the size of the button being drawn.
      CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
      CRect rect( lpDrawItemStruct->rcItem );
      if ( m_pDib->m_bBitmapLoaded == FALSE ||
           m_pDib->Draw( (CDC &) *pDC, &rect, 0, FALSE, 0, TRUE ) == FALSE )
      {
         if ( m_pchMsgText && *m_pchMsgText )
            pDC->DrawText( m_pchMsgText, -1, rect, DT_CENTER | DT_WORDBREAK );
         else
            pDC->DrawText( "Unknown BmpCtl", -1, rect, DT_CENTER | DT_WORDBREAK );
      }
   }
}

BOOL
ZBitmapButton::PreTranslateMessage( MSG *pMsg )
{
   // TODO: Add your specialized code here and/or call the base class.
   InitToolTip( );
   m_ToolTip.RelayEvent( pMsg );
   return( CButton::PreTranslateMessage( pMsg ) );
}

// Set the tooltip with a string resource.
void
ZBitmapButton::SetToolTipText( int nId, BOOL bActivate )
{
   CString csText;

   // load string resource.
   csText.LoadString( nId );

   // If string resource is not empty
   if ( csText.IsEmpty( ) == FALSE )
      SetToolTipText( &csText, bActivate );
}

void
ZBitmapButton::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   if ( m_bTracking == FALSE )
   {
      TRACKMOUSEEVENT tme;
      tme.cbSize = sizeof( tme );
      tme.hwndTrack = m_hWnd;
      tme.dwFlags = TME_LEAVE | TME_HOVER;
      tme.dwHoverTime = 1;
      m_bTracking = _TrackMouseEvent( &tme );
   }

   Default( );              // pass it along to parent
}

// ZBitmapButton - dtor
ZBitmapButton::~ZBitmapButton( )
{
// TraceLineS( "ZBitmapButton::~ZBitmapButton", "" );
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

   mDeleteInitA( m_pchCtrlBOI );
   mDeleteInitA( m_pchMsgText );
   mDeleteInit( m_pDibUP );
   mDeleteInit( m_pDibDN );
   mDeleteInit( m_pDibFO );
   mDeleteInit( m_pDibDA );
   mDeleteInit( m_pBMPINFO );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Going from a JPG/GIF/BMP File to a HBITMAP File Using Plain API.
//
// Michael Chourdakis
// June 11, 2003
//
// This code has probably been superseded by MFC or whatever classes, which
// can load any image file. However, you will most probably do it with plain
// API, and converting a JPG file using Intel's library can be very painful.
//
// Here is a simple function that loads a JPG/GIF/BMP file into memory and
// returns a HBITMAP. The function uses IPicture, an OLE interface.
// #include <olectl.h> and <ole2.h> are also needed.
//
// Function LoadAnImage: accepts a file name and returns an HBITMAP.
// On error, it returns 0.
zEXPORT HBITMAP OPERATION
LoadAnImage( zPCHAR pchFileName )
{
   // Use IPicture stuff to use JPG / GIF files
   IPicture *p;
   IStream *s;
// IPersistStream *ps;
   HGLOBAL hG;
   void *pp;
   FILE *fp;

   // Read file in memory
   fp = fopen( pchFileName, "rb" );
   if ( fp == 0 )
      return( 0 );

   fseek( fp, 0, SEEK_END );
   int fs = ftell( fp );
   fseek( fp, 0, SEEK_SET );
   hG = GlobalAlloc( GPTR, fs );
   if ( hG == 0 )
   {
      fclose( fp );
      return( 0 );
   }

   pp = (void *) hG;
   fread( pp, 1, fs, fp );
   fclose( fp );

   // Create an IStream so IPicture can ...?
   CreateStreamOnHGlobal( hG, false, &s );
   if ( s == 0 )
   {
      GlobalFree( hG );
      return( 0 );
   }

   OleLoadPicture( s, 0, false, IID_IPicture, (void **) &p );

   if ( p == 0 )
   {
      s->Release( );
      GlobalFree( hG );
      return( 0 );
   }

   s->Release( );
   GlobalFree( hG );

   HBITMAP hB = 0;
   p->get_Handle( (unsigned int *) &hB );

   // Copy the image. Necessary, because upon p's release,
   // the handle is destroyed.
   HBITMAP hBB = (HBITMAP) CopyImage( hB, IMAGE_BITMAP, 0, 0,
                                      LR_COPYRETURNORG );

   p->Release( );
   return( hBB );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// DrawGrayScale  - Draws a bitmap in gray scale
// pDC         - Pointer to target device context
// hDIB        - Handle of device-independent bitmap
//
void
DrawGrayScale( CDC *pDC, HANDLE hDIB )
{
   CPalette pal;
   CPalette *pOldPalette;

   BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;

   int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed :
                 1 << bmInfo.bmiHeader.biBitCount;

   // Create the palette if needed.
   if ( (pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE) && nColors <= 256 )
   {
      // The device supports a palette and bitmap has color table.

      // Allocate memory for a palette.
      UINT nSize = sizeof( LOGPALETTE ) + (sizeof( PALETTEENTRY ) * nColors);
      LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[ nSize ];

      pLP->palVersion = 0x300;
      pLP->palNumEntries = nColors;

      for ( int k = 0; k < nColors; k++ )
      {
         long lSquareSum = bmInfo.bmiColors[ k ].rgbRed * bmInfo.bmiColors[ k ].rgbRed +
                           bmInfo.bmiColors[ k ].rgbGreen * bmInfo.bmiColors[ k ].rgbGreen +
                           bmInfo.bmiColors[ k ].rgbBlue * bmInfo.bmiColors[ k ].rgbBlue;
         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );
         pLP->palPalEntry[ k ].peRed = nGray;
         pLP->palPalEntry[ k ].peGreen = nGray;
         pLP->palPalEntry[ k ].peBlue = nGray;
         pLP->palPalEntry[ k ].peFlags = 0;
      }

      pal.CreatePalette( pLP );

      delete[] pLP;

      // Select the palette.
      pOldPalette = pDC->SelectPalette( &pal, FALSE );
      pDC->RealizePalette( );
   }
   else
   if ( (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE) == 0 && nColors <= 256 )
   {
      // Device does not supports palettes but bitmap has a color table.
      // Modify the bitmaps color table directly.
      // Note : This ends up changing the DIB. If that is not acceptable then
      // copy the DIB and then change the copy rather than the original.

      for ( int k = 0; k < nColors; k++ )
      {
         long lSquareSum = bmInfo.bmiColors[ k ].rgbRed * bmInfo.bmiColors[ k ].rgbRed +
                           bmInfo.bmiColors[ k ].rgbGreen * bmInfo.bmiColors[ k ].rgbGreen +
                           bmInfo.bmiColors[ k ].rgbBlue * bmInfo.bmiColors[ k ].rgbBlue;
         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );
         bmInfo.bmiColors[ k ].rgbRed = nGray;
         bmInfo.bmiColors[ k ].rgbGreen = nGray;
         bmInfo.bmiColors[ k ].rgbBlue = nGray;
      }
   }


   int nWidth = bmInfo.bmiHeader.biWidth;
   int nHeight = bmInfo.bmiHeader.biHeight;


   // Draw the image
   LPVOID lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

   ::SetDIBitsToDevice( pDC->m_hDC,       // hDC
                        0,                // XDest
                        0,                // YDest
                        nWidth,           // nDestWidth
                        nHeight,          // nDestHeight
                        0,                // XSrc
                        0,                // YSrc
                        0,                // nStartScan
                        nHeight,          // nNumScans
                        lpDIBBits,        // lpBits
                        (LPBITMAPINFO) hDIB, // lpBitsInfo
                        DIB_RGB_COLORS ); // wUsage


   pDC->SelectPalette( pOldPalette, FALSE );
}

// TransparentBlt - Copies a bitmap transparently onto the destination DC
// hdcDest     - Handle to destination device context
// nXDest      - x-coordinate of destination rectangle's upper-left corner
// nYDest      - y-coordinate of destination rectangle's upper-left corner
// nWidth      - Width of destination rectangle
// nHeight     - height of destination rectangle
// hBitmap     - Handle of the source bitmap
// nXSrc    - x-coordinate of source rectangle's upper-left corner
// nYSrc    - y-coordinate of source rectangle's upper-left corner
// colorTransparent  - The transparent color
// hPal        - Logical palette to be used with bitmap. Can be NULL

void
TransparentBlt( HDC hdcDest, int nXDest, int nYDest, int nWidth,
                int nHeight, HBITMAP hBitmap, int nXSrc, int nYSrc,
                COLORREF colorTransparent, HPALETTE hPal )
{
   CDC dc, memDC, maskDC, tempDC;
   dc.Attach( hdcDest );
   maskDC.CreateCompatibleDC( &dc );
   CBitmap maskBitmap;

   //add these to store return of SelectObject() calls
   CBitmap *pOldMemBmp = NULL;
   CBitmap *pOldMaskBmp = NULL;
   HBITMAP hOldTempBmp = NULL;

   memDC.CreateCompatibleDC( &dc );
   tempDC.CreateCompatibleDC( &dc );
   CBitmap bmpImage;
   bmpImage.CreateCompatibleBitmap( &dc, nWidth, nHeight );
   pOldMemBmp = memDC.SelectObject( &bmpImage );

   // Select and realize the palette
   if ( (dc.GetDeviceCaps( RASTERCAPS ) & RC_PALETTE) && hPal )
   {
      ::SelectPalette( dc, hPal, FALSE );
      dc.RealizePalette( );

      ::SelectPalette( memDC, hPal, FALSE );
   }

   hOldTempBmp = (HBITMAP) ::SelectObject( tempDC.m_hDC, hBitmap );

   memDC.BitBlt( 0,0,nWidth, nHeight, &tempDC, nXSrc, nYSrc, SRCCOPY );

   // Create monochrome bitmap for the mask
   maskBitmap.CreateBitmap( nWidth, nHeight, 1, 1, NULL );
   pOldMaskBmp = maskDC.SelectObject( &maskBitmap );
   memDC.SetBkColor( colorTransparent );

   // Create the mask from the memory DC
   maskDC.BitBlt( 0, 0, nWidth, nHeight, &memDC,
                  0, 0, SRCCOPY );

   // Set the background in memDC to black. Using SRCPAINT with black
   // and any other color results in the other color, thus making
   // black the transparent color
   memDC.SetBkColor( RGB( 0, 0, 0 ) );
   memDC.SetTextColor( RGB( 255, 255, 255 ) );
   memDC.BitBlt( 0, 0, nWidth, nHeight, &maskDC, 0, 0, SRCAND );

   // Set the foreground to black. See comment above.
   dc.SetBkColor( RGB( 255, 255, 255 ) );
   dc.SetTextColor( RGB( 0, 0, 0 ) );
   dc.BitBlt( nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND );

   // Combine the foreground with the background
   dc.BitBlt( nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT );

   if ( hOldTempBmp)
      ::SelectObject( tempDC.m_hDC, hOldTempBmp );

   if ( pOldMaskBmp )
      maskDC.SelectObject( pOldMaskBmp );

   if ( pOldMemBmp )
      memDC.SelectObject( pOldMemBmp );

   dc.Detach( );
}

//
//      DitherBlt :     Draw a bitmap dithered (3D grayed effect like disabled
//                      buttons in toolbars) into a destination DC
//      Author :        Jean-Edouard Lachand-Robert (iamwired@geocities.com), June 1997.
//
//      hdcDest :       destination DC
//      nXDest :        x coordinate of the upper left corner of the destination rectangle into the DC
//      nYDest :        y coordinate of the upper left corner of the destination rectangle into the DC
//      nWidth :        width of the destination rectangle into the DC
//      nHeight :       height of the destination rectangle into the DC
//      hbm :           the bitmap to draw (as a part or as a whole)
//      nXSrc :         x coordinates of the upper left corner of the source rectangle into the bitmap
//      nYSrc :         y coordinates of the upper left corner of the source rectangle into the bitmap
//
void
DitherBlt( HDC hdcDest, int nXDest, int nYDest, int nWidth,
           int nHeight, HBITMAP hbm, int nXSrc, int nYSrc )
{
   ASSERT( hdcDest && hbm );
   ASSERT( nWidth > 0 && nHeight > 0 );

   // Create a generic DC for all BitBlts
   HDC hDC = CreateCompatibleDC( hdcDest );
   ASSERT( hDC );

   if ( hDC )
   {
      // Create a DC for the monochrome DIB section
      HDC bwDC = CreateCompatibleDC( hDC );
      ASSERT( bwDC );
      if ( bwDC )
      {
         // Create the monochrome DIB section with a black and white palette
         struct
         {
            BITMAPINFOHEADER bmiHeader;
            RGBQUAD      bmiColors[ 2 ];
         } RGBBWBITMAPINFO =
         {
            {     // a BITMAPINFOHEADER
               sizeof( BITMAPINFOHEADER ), // biSize
               nWidth,           // biWidth;
               nHeight,          // biHeight;
               1,                // biPlanes;
               1,                // biBitCount
               BI_RGB,           // biCompression;
               0,                // biSizeImage;
               0,                // biXPelsPerMeter;
               0,                // biYPelsPerMeter;
               0,                // biClrUsed;
               0                 // biClrImportant;
            },

            {
               { 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }
            }
         };

         VOID *pbitsBW;
         HBITMAP hbmBW = CreateDIBSection( bwDC,
                                           (LPBITMAPINFO) &RGBBWBITMAPINFO,
                                           DIB_RGB_COLORS, &pbitsBW, NULL, 0 );
         ASSERT( hbmBW );
         if ( hbmBW )
         {
            // Attach the monochrome DIB section and the bitmap to the DCs
            SelectObject( bwDC, hbmBW );
            SelectObject( hDC, hbm );

            // BitBlt the bitmap into the monochrome DIB section
            BitBlt( bwDC, 0, 0, nWidth, nHeight, hDC, nXSrc, nYSrc, SRCCOPY );

            // Paint the destination rectangle in gray
            FillRect( hdcDest, CRect( nXDest, nYDest, nXDest + nWidth, nYDest +
               nHeight ), GetSysColorBrush( COLOR_3DFACE ) );

            // BitBlt the black bits in the monochrome bitmap into COLOR_3DHILIGHT bits in the destination DC
            // The magic ROP comes from the Charles Petzold's book
            HBRUSH hb = CreateSolidBrush( GetSysColor( COLOR_3DHILIGHT ) );
            HBRUSH oldBrush = (HBRUSH) SelectObject( hdcDest, hb );
            BitBlt( hdcDest, nXDest + 1, nYDest + 1, nWidth, nHeight, bwDC, 0, 0, 0xB8074A );

            // BitBlt the black bits in the monochrome bitmap into COLOR_3DSHADOW bits in the destination DC
            hb = CreateSolidBrush( GetSysColor( COLOR_3DSHADOW ) );
            DeleteObject( SelectObject( hdcDest, hb ) );
            BitBlt( hdcDest, nXDest, nYDest, nWidth, nHeight, bwDC, 0, 0, 0xB8074A );
            DeleteObject( SelectObject( hdcDest, oldBrush ) );
         }

         VERIFY( DeleteDC( bwDC ) );
      }

      VERIFY( DeleteDC( hDC ) );
   }
}

///////////////////////////////////////////////////////////////////////////////////
// Emboss      - Creates a 3D embossed effect
// Returns     - A new bitmap containing the resulting effect
// hBitmap     - Bitmap that contains the basic text & shapes
// hbmBackGnd     - Contains the color image
// hPal        - Handle of palette associated with hbmBackGnd
// bRaised     - True if raised effect is desired. False for sunken effect
// xDest    - x coordinate - used to offset hBitmap
// yDest    - y coordinate - used to offset hBitmap
// clrHightlight  - Color used for the highlight edge
// clrShadow      - Color used for the shadow
//
// Note        - 1. Neither of the bitmap handles passed in should be selected
//         in a device context.
//         2. The pixel at 0,0 in hBitmap is considered the background color
//
zEXPORT HBITMAP OPERATION
Emboss( HBITMAP hBitmap, HBITMAP hbmBackGnd, HPALETTE hPal,
        BOOL bRaised, int xDest, int yDest,
        COLORREF clrHighlight, COLORREF clrShadow )
{
   const DWORD PSDPxax = 0x00B8074A;
   BITMAP   bmInfo;
   HBITMAP  hbmOld, hbmShadow, hbmHighlight, hbmResult, hbmOldMem;
   HBRUSH   hbrPat;
   HDC      hDC, hColorDC, hMonoDC, hMemDC;

   if ( !bRaised )
   {
      // Swap the highlight and shadow color
      COLORREF clrTemp = clrShadow;
      clrShadow = clrHighlight;
      clrHighlight = clrTemp;
   }

   // We create two monochrome bitmaps. One of them will contain the
   // highlighted edge and the other will contain the shadow. These
   // bitmaps are then used to paint the highlight and shadow on the
   // background image.

   hbmResult = NULL;
   hDC = GetDC( NULL );

   // Create a compatible DCs
   hMemDC = ::CreateCompatibleDC( hDC );
   hMonoDC = CreateCompatibleDC( hDC );
   hColorDC = CreateCompatibleDC( hDC );

   if ( hMemDC == NULL || hMonoDC == NULL || hColorDC == NULL )
   {
      if ( hMemDC ) DeleteDC( hMemDC );
      if ( hMonoDC ) DeleteDC( hMonoDC );
      if ( hColorDC ) DeleteDC( hColorDC );

      return NULL;
   }

   // Select the background image into memory DC so that we can draw it
   hbmOldMem = (HBITMAP) ::SelectObject( hMemDC, hbmBackGnd );

   // Get dimensions of the background image
   BITMAP bm;
   ::GetObject( hbmBackGnd, sizeof( bm ), &bm );



   // Create the monochrome and compatible color bitmaps
   GetObject( hBitmap, sizeof( BITMAP ), (LPSTR) &bmInfo );
   hbmShadow = CreateBitmap( bmInfo.bmWidth, bmInfo.bmHeight, 1, 1, NULL );
   hbmHighlight = CreateBitmap( bmInfo.bmWidth, bmInfo.bmHeight, 1, 1, NULL );
   hbmResult = CreateCompatibleBitmap( hDC, bm.bmWidth, bm.bmHeight );

   hbmOld = (HBITMAP) SelectObject( hColorDC, hBitmap );

   // Set background color of bitmap for mono conversion
   // We assume that the pixel in the top left corner has the background color
   SetBkColor( hColorDC, GetPixel( hColorDC, 0, 0 ) );

   // Create the highlight bitmap.
   hbmHighlight = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmHighlight );
   PatBlt( hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, WHITENESS );
   BitBlt( hMonoDC, 0, 0, bmInfo.bmWidth - 1, bmInfo.bmHeight - 1,
           hColorDC, 1, 1, SRCCOPY );
   BitBlt( hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight,
           hColorDC, 0, 0, MERGEPAINT );
   hbmHighlight = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmHighlight );


   // create the shadow bitmap
   hbmShadow = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmShadow );
   PatBlt( hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, WHITENESS );
   BitBlt( hMonoDC, 1, 1, bmInfo.bmWidth-1, bmInfo.bmHeight-1,
           hColorDC, 0, 0, SRCCOPY );
   BitBlt( hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight,
           hColorDC, 0, 0, MERGEPAINT );
   hbmShadow = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmShadow );


   // Now let's start working on the final image
   SelectObject( hColorDC, hbmResult );

   // Select and realize the palette if one is supplied
   if ( hPal && (GetDeviceCaps( hDC, RASTERCAPS ) & RC_PALETTE) )
   {
      ::SelectPalette( hColorDC, hPal, FALSE );
      ::RealizePalette( hColorDC );
   }

   // Draw the background image
   BitBlt( hColorDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0,SRCCOPY );

   // Restore the old bitmap in the hMemDC
   ::SelectObject( hMemDC, hbmOldMem );

   // Set the background and foreground color for the raster operations
   SetBkColor( hColorDC, RGB( 255, 255, 255 ) );
   SetTextColor( hColorDC, RGB( 0, 0, 0 ) );

   // blt the highlight edge
   hbrPat = CreateSolidBrush( clrHighlight );
   hbrPat = (HBRUSH) SelectObject( hColorDC, hbrPat );
   hbmHighlight = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmHighlight );
   BitBlt( hColorDC, xDest, yDest, bmInfo.bmWidth, bmInfo.bmHeight,
           hMonoDC, 0, 0, PSDPxax );
   DeleteObject( SelectObject( hColorDC, hbrPat ) );
   hbmHighlight = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmHighlight );

   // blt the shadow edge
   hbrPat = CreateSolidBrush( clrShadow );
   hbrPat = (HBRUSH) SelectObject( hColorDC, hbrPat );
   hbmShadow = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmShadow );
   BitBlt( hColorDC, xDest, yDest, bmInfo.bmWidth, bmInfo.bmHeight,
           hMonoDC, 0, 0, PSDPxax );
   DeleteObject( SelectObject( hColorDC, hbrPat ) );
   hbmShadow = (HBITMAP) SelectObject( hMonoDC, (HGDIOBJ) hbmShadow );

   // select old bitmap into color DC
   SelectObject( hColorDC, hbmOld );

   DeleteObject( (HGDIOBJ) hbmShadow );
   DeleteObject( (HGDIOBJ) hbmHighlight );

   ReleaseDC( NULL, hDC );

   return( hbmResult );
}

// A Quicker way to load *.BMP files into a CBitmap Object

// Create your own CBitmap derived class (say, CMyBitmap)
// add a "load from bitmap" method as listed below

#if 0
BOOL
CMyBitmap::LoadBitmap( LPCTSTR cpcFilename )
{
   ASSERT( cpcFilename );
   DeleteObject( );

   HBITMAP hBitmap =
      (HBITMAP) LoadImage( NULL, cpcFilename, IMAGE_BITMAP, 0, 0,
                           LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE );
   return( Attach( hBitmap ) );
}

For CBitmap objects we can use the GetBitmap() function to determine
the height and width of the bitmap.
   // The variable bitmap is a CBitmap object
   BITMAP bm;
   bitmap.GetBitmap( &bm );
   bmWidth = bm.bmWidth;
   bmHeight = bm.bmHeight;

If you have a HBITMAP, you can attach it to a CBitmap object and use
the method shown above or you can use

   // The variable hBmp is a HBITMAP
   BITMAP bm;
   ::GetObject( hBmp, zsizeof( bm ), &bm );
   bmWidth = bm.bmWidth;
   bmHeight = bm.bmHeight;

For images in a BMP file, you can use something like

   CFile file;
   // sBMPFileName is the BMP filename
   if ( !file.Open( sBMPFileName, CFile::modeRead ) )
      return;

   BITMAPFILEHEADER bmfHeader;

   // Read file header
   if ( file.Read( (LPSTR) &bmfHeader, zsizeof( bmfHeader ) ) != zsizeof( bmfHeader ))
      return;

   // File type should be 'BM'
   if ( bmfHeader.bfType != ((WORD) ('M' << 8) | 'B') )
      return;

   BITMAPINFOHEADER bmiHeader;
   if ( file.Read( (LPSTR) &bmiHeader, zsizeof( bmiHeader ) ) != zsizeof( bmiHeader ) )
      return;


   int bmWidth = bmiHeader.biWidth;
   int bmHeight = bmiHeader.biHeight;

Function 1: Fade from grayscale to color
The FadeGrayScaleToColor() draws the device-independent bitmap (DIB) onto the
device context as a grayscale image and then slowly changes it to a full color
image.  The function can display the fading effect only with DIBs with 256 or
fewer colors. This function basically handles two very distinct scenarios.
The first scenario is that the display device supports logical palettes, in
which case we can use palette animation. The second scenario is that the
display does not support logical palettes. In this case we have to repeatedly
draw a modified DIB to get the desired effect.

When we draw the initial image on a device that supports a palette, we have
the problem of retaining the color mapping. Since we start with a grayscale
image, if we just selected a grayscale palette into the device context and
drew the image onto it, then some of the colors would get remapped to
different colors since the color table is different from the colors in the
logical palette. If the colors were remapped then original red, blue or
green would get mapped to exactly the same color in the grayscale logical
palette. Any further palette manipulation will not be able to restore the
reds, greens and blues. They would all still remain the same color.

To avoid this problem, we first draw the image onto a memory device context
using a color palette. Once the image is on the DC we use palette animation
to change only the logical palette. We apply the same palette operations on
the physical device context resulting in identical palettes for both the
device contexts. Now we can safely BitBlt the image onto the physical device
context without getting the colors remapped. Once we have the image in the
target device, we simply animate the palette till all the entries in the
palette are the colors needed to display the image properly.

Of course, the above applies only if the target device supports palette
operations. If the device supports more than 256 colors then it will not
support a logical palette. In this case we repeatedly change the color
information in the DIB and display it again with the modified color table.
This requires lot more processing power than palette animation.


// FadeGrayScaleToColor - Draws a bitmap in gray scale and slowly turns it to color
// pDC            - Pointer to target device context
// hDIB           - Handle of device-independent bitmap
// xDest          - x-coordinate of upper-left corner of dest. rect.
// yDest          - y-coordinate of upper-left corner of dest. rect.
// nLoops         - How many loops to fade the image into color
// nDelay         - Delay in milli-seconds between each loop
//
void
FadeGrayScaleToColor( CDC *pDC, HANDLE hDIB, int xDest, int yDest,
                      int nLoops, int nDelay )
{
   CPalette pal;
   CPalette *pOldPalette;
   PALETTEENTRY peAnimate[ 256 ];
   PALETTEENTRY peGray[ 256 ];
   PALETTEENTRY peOriginal[ 256 ];

   BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;

   int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed :
               1 << bmInfo.bmiHeader.biBitCount;

   int nReservedColors = nColors > 236 ? 236 : nReservedColors;
   int nWidth = bmInfo.bmiHeader.biWidth;
   int nHeight = bmInfo.bmiHeader.biHeight;

   // Create the palette if needed
   if ( (pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE) && nColors <= 256 )
   {
      // The device supports a palette and bitmap has color table

      HPALETTE hPal = CreateReservedPalette( hDIB );
      pal.Attach( hPal );

      // Now save the original colors and get the grayscale colors
      pal.GetPaletteEntries( 0, nReservedColors, (LPPALETTEENTRY) &peGray );
      for ( int k = 0; k < nReservedColors; k++)
      {
         // Save original colors
         peOriginal[ k ].peRed = peGray[ k ].peRed;
         peOriginal[ k ].peGreen = peGray[ k ].peGreen;
         peOriginal[ k ].peBlue = peGray[ k ].peBlue;

         long lSquareSum = peGray[ k ].peRed * peGray[ k ].peRed +
                           peGray[ k ].peGreen * peGray[ k ].peGreen +
                           peGray[ k ].peBlue * peGray[ k ].peBlue;
         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );

         peGray[ k ].peRed = nGray;
         peGray[ k ].peGreen = nGray;
         peGray[ k ].peBlue = nGray;
      }

      // Select the palette
      pOldPalette = pDC->SelectPalette( &pal, FALSE );
      pDC->RealizePalette( );

      // We need to draw the image without any color mapping
      // We use a memory DC to achieve this.
      CDC memDC;
      memDC.CreateCompatibleDC( pDC );
      CBitmap bmp;
      bmp.CreateCompatibleBitmap( pDC, nWidth, nHeight );
      CBitmap *pOldBitmap = memDC.SelectObject( &bmp );
      CPalette *pOldMemPalette = memDC.SelectPalette( &pal, FALSE );
      memDC.RealizePalette( );

      // Draw the image to memDC
      LPVOID lpDIBBits = (LPVOID) (bmInfo.bmiColors + nColors);

      ::SetDIBitsToDevice( memDC.m_hDC, 0, 0, nWidth, nHeight, 0, 0, 0,
                           nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                           DIB_RGB_COLORS );

      // Set the color to grayscale
      AnimatePalette( hPal, 0, nColors, (LPPALETTEENTRY) &peGray );

      // Now copy the image from the memory DC
      // Since the palettes in memDC and pDC are the same, no color mapping
      // takes place. The image will appear in the start color
      pDC->BitBlt( xDest, yDest, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );

      // Get the grayscale entries in the animation palette
      pal.GetPaletteEntries( 0, nColors, (LPPALETTEENTRY) &peAnimate );

      // Reselect old objects into mem DC
      memDC.SelectPalette( pOldMemPalette, FALSE );
      memDC.SelectObject( pOldBitmap );

      // Now animate palette to bring the image into color
      for ( k = 1; k <= nLoops; k++ )
      {
         for ( int j = 0; j < nColors; j++ )
         {
            peAnimate[ j ].peRed = peGray[ j ].peRed -
              ((peGray[ j ].peRed - peOriginal[ j ].peRed) * k) / nLoops;
            peAnimate[ j ].peGreen = peGray[ j ].peGreen -
              ((peGray[ j ].peGreen - peOriginal[ j ].peGreen) * k) / nLoops;
            peAnimate[ j ].peBlue = peGray[ j ].peBlue  -
              ((peGray[ j ].peBlue - peOriginal[ j ].peBlue) * k) / nLoops;
            peAnimate[ j ].peFlags = peGray[ j ].peFlags;
         }

         pal.AnimatePalette( 0, nColors, (LPPALETTEENTRY)&peAnimate );

         // Delay...
         Sleep( nDelay );
      }

      // Select the old palette back
      pDC->SelectPalette( pOldPalette, FALSE );
   }
   else
   if ( (pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE) == 0 && nColors <= 256 )
   {
      // Device does not supports palettes but bitmap has a color table
      // Modify the bitmaps color table directly

      for ( int k = 0; k < nColors; k++ )
      {
         // First save original colors
         peOriginal[ k ].peRed =   bmInfo.bmiColors[ k ].rgbRed;
         peOriginal[ k ].peGreen = bmInfo.bmiColors[ k ].rgbGreen;
         peOriginal[ k ].peBlue =  bmInfo.bmiColors[ k ].rgbBlue;

         long lSquareSum = bmInfo.bmiColors[ k ].rgbRed * bmInfo.bmiColors[ k ].rgbRed +
                           bmInfo.bmiColors[ k ].rgbGreen * bmInfo.bmiColors[ k ].rgbGreen +
                           bmInfo.bmiColors[ k ].rgbBlue * bmInfo.bmiColors[ k ].rgbBlue;
         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );
         bmInfo.bmiColors[ k ].rgbRed = nGray;
         bmInfo.bmiColors[ k ].rgbGreen = nGray;
         bmInfo.bmiColors[ k ].rgbBlue = nGray;

         // Create a reference color table
         peGray[ k ].peRed = nGray;
         peGray[ k ].peGreen = nGray;
         peGray[ k ].peBlue = nGray;
      }

      // Let's draw the grayscale image
      LPVOID lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
      ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0, 0, 0,
                           nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                           DIB_RGB_COLORS );

      // Now color the image
      for ( k = 1; k <= nLoops; k++ )
      {
         for ( int j = 0; j< nColors; j++)
         {
            bmInfo.bmiColors[ j ].rgbRed = peGray[ j ].peRed  -
               ((peGray[ j ].peRed - peOriginal[ j ].peRed) * k) / nLoops;
            bmInfo.bmiColors[ j ].rgbGreen = peGray[ j ].peGreen -
               ((peGray[ j ].peGreen - peOriginal[ j ].peGreen) * k) / nLoops;
            bmInfo.bmiColors[ j ].rgbBlue  = peGray[ j ].peBlue  -
               ((peGray[ j ].peBlue - peOriginal[ j ].peBlue) * k) / nLoops;
         }

         // Draw the image again
         ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0,
                              0, 0, nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                              DIB_RGB_COLORS );

         // Delay...
         Sleep( nDelay );
      }
   }
   else
   {
      // Any other scenario, simply draw the image
      LPVOID lpDIBBits = (LPVOID) (bmInfo.bmiColors + nColors);
      ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0, 0, 0,
                           nHeight, lpDIBBits, (LPBITMAPINFO)hDIB,
                           DIB_RGB_COLORS );
   }
}

// Function 2: Fade from color to grayscale
// The FadeColorToGrayScale () draws the device-independent bitmap (DIB) onto
// the device context as a full color image and then slowly changes it to a
// grayscale image. Since this function starts by drawing the image with a
// color palette it doesn't have to worry about two very different colors
// getting mapped to the same color as can happen if we start with a grayscale
// palette instead. This, of course, applies only displays that support a palette.

// FadeColorToGrayScale - Draws a bitmap in color slowly turns it to grayscale
// pDC            - Pointer to target device context
// hDIB           - Handle of device-independent bitmap
// xDest       - x-coordinate of upper-left corner of dest. rect.
// yDest       - y-coordinate of upper-left corner of dest. rect.
// nLoops         - How many loops to fade the image into color
// nDelay         - Delay in milli-seconds between each loop
//
void
FadeColorToGrayScale( CDC *pDC, HANDLE hDIB, int xDest, int yDest,
                      int nLoops, int nDelay )
{
   CPalette pal;
   CPalette *pOldPalette;
   PALETTEENTRY peAnimate[ 256 ];
   PALETTEENTRY peGray[ 256 ];
   PALETTEENTRY peOriginal[ 256 ];

   BITMAPINFO &bmInfo = *(LPBITMAPINFO) hDIB;

   int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed :
               1 << bmInfo.bmiHeader.biBitCount;

   int nReservedColors = nColors > 236 ? 236 : nReservedColors;
   int nWidth = bmInfo.bmiHeader.biWidth;
   int nHeight = bmInfo.bmiHeader.biHeight;

   // Create the palette if needed
   if ( (pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE) && nColors <= 256 )
   {
      // The device supports a palette and bitmap has color table

      HPALETTE hPal = CreateReservedPalette( hDIB );
      pal.Attach( hPal );

      // Now save the original colors and get the grayscale colors
      pal.GetPaletteEntries( 0, nReservedColors, (LPPALETTEENTRY) &peOriginal );
      for ( int k = 0; k < nReservedColors; k++ )
      {
         long lSquareSum = peOriginal[ k ].peRed * peOriginal[ k ].peRed +
                           peOriginal[ k ].peGreen * peOriginal[ k ].peGreen +
                           peOriginal[ k ].peBlue * peOriginal[ k ].peBlue;
         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );

         peGray[ k ].peRed = nGray;
         peGray[ k ].peGreen = nGray;
         peGray[ k ].peBlue = nGray;
      }

      // Select the palette
      pOldPalette = pDC->SelectPalette( &pal, FALSE );
      pDC->RealizePalette( );

      // Draw the image
      LPVOID lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

      ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0, 0, 0,
                           nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                           DIB_RGB_COLORS);

      // Now animate palette to set the image to grayscale
      for ( k = 1; k <= nLoops; k++ )
      {
         for ( int j = 0; j < nColors; j++ )
         {
            peAnimate[ j ].peRed = peOriginal[ j ].peRed -
               ((peOriginal[ j ].peRed -peGray[ j ].peRed) *k ) / nLoops;
            peAnimate[ j ].peGreen = peOriginal[ j ].peGreen -
               ((peOriginal[ j ].peGreen-peGray[ j ].peGreen) * k) / nLoops;
            peAnimate[ j ].peBlue = peOriginal[ j ].peBlue -
               ((peOriginal[ j ].peBlue -peGray[ j ].peBlue) * k) / nLoops;

            peAnimate[ j ].peFlags = peOriginal[ j ].peFlags;
         }

         pal.AnimatePalette( 0, nColors, (LPPALETTEENTRY) &peAnimate );

         // Delay...
         Sleep( nDelay );
      }

      // Select the old palette back
      pDC->SelectPalette( pOldPalette, FALSE );
   }
   else
   if ( (pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE) == 0 && nColors <= 256 )
   {
      // Device does not supports palettes but bitmap has a color table
      // Modify the bitmaps color table directly

      // Get the original colors and the grayscale colors
      for ( int k = 0; k < nColors; k++ )
      {
         // First save original colors
         peOriginal[ k ].peRed =   bmInfo.bmiColors[ k ].rgbRed;
         peOriginal[ k ].peGreen = bmInfo.bmiColors[ k ].rgbGreen;
         peOriginal[ k ].peBlue =  bmInfo.bmiColors[ k ].rgbBlue;

         long lSquareSum = bmInfo.bmiColors[ k ].rgbRed * bmInfo.bmiColors[ k ].rgbRed +
                           bmInfo.bmiColors[ k ].rgbGreen * bmInfo.bmiColors[ k ].rgbGreen +
                           bmInfo.bmiColors[ k ].rgbBlue * bmInfo.bmiColors[ k ].rgbBlue;
         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );

         // Create a reference color table
         peGray[ k ].peRed = nGray;
         peGray[ k ].peGreen = nGray;
         peGray[ k ].peBlue = nGray;
      }

      // Let's draw the color image
      LPVOID lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
      ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0,
                           0, 0, nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                           DIB_RGB_COLORS );

      // Now change the image to grayscale
      for ( k = 1; k <= nLoops; k++ )
      {
         for ( int j = 0; j< nColors; j++ )
         {
            bmInfo.bmiColors[ j ].rgbRed   = peOriginal[ j ].peRed -
               ((peOriginal[ j ].peRed -peGray[ j ].peRed) * k) / nLoops;
            bmInfo.bmiColors[ j ].rgbGreen = peOriginal[ j ].peGreen -
               ((peOriginal[ j ].peGreen-peGray[ j ].peGreen) * k) /
                  nLoops;
            bmInfo.bmiColors[ j ].rgbBlue  = peOriginal[ j ].peBlue  -
               ((peOriginal[ j ].peBlue -peGray[ j ].peBlue) * k) / nLoops;
         }

         // Draw the image again
         ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0,
                              0, 0, nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                              DIB_RGB_COLORS );

         // Delay...
         Sleep( nDelay );
      }
   }
   else
   {
      // Any other scenario, simply draw the image
      LPVOID lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
      ::SetDIBitsToDevice( pDC->m_hDC, xDest, yDest, nWidth, nHeight, 0,
                           0, 0, nHeight, lpDIBBits, (LPBITMAPINFO) hDIB,
                           DIB_RGB_COLORS );
   }
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
 //   ZBitmapButton message handlers

// Set the tooltip with a CString
void
ZBitmapButton::SetToolTipText( CString *pcsTipText, BOOL bActivate )
{
   // We cannot accept NULL pointer.
   if ( pcsTipText == NULL )
      return;

   // Initialize ToolTip.
   InitToolTip( );

   // If there is no tooltip defined then add it.
   if ( m_ToolTip.GetToolCount( ) == 0 )
   {
      CRect rectBtn;
      GetClientRect( rectBtn );
      m_ToolTip.AddTool( this, (LPCTSTR) *pcsTipText, rectBtn, 1 );
   }

   // Set text for tooltip.
   m_ToolTip.UpdateTipText( (LPCTSTR) *pcsTipText, this, 1 );
   m_ToolTip.Activate( bActivate );
}

void
ZBitmapButton::InitToolTip( )
{
   if ( m_ToolTip.m_hWnd == NULL )
   {
      // Create ToolTip control.
      m_ToolTip.Create( this );

      // Create inactive.
      m_ToolTip.Activate( FALSE );
   }
} // End of InitToolTip

// Activate the tooltip.
void
ZBitmapButton::ActivateToolTip( BOOL bActivate )
{
   // If there is no tooltip then do nothing..
   if ( m_ToolTip.GetToolCount( ) == 0 )
      return;

   // Activate tooltip.
   m_ToolTip.Activate( bActivate );

} // End of EnableTooltip

LRESULT
ZBitmapButton::OnMouseHover( WPARAM wParam, LPARAM lParam )
{
   // TODO: Add your message handler code here and/or call default
   m_bHover = TRUE;
   Invalidate( );
   return( 0 );
}

LRESULT
ZBitmapButton::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
   m_bTracking = FALSE;
   m_bHover = FALSE;
   Invalidate( );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
BitmapButton( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZBitmapButton( pZSubtask, pWndParent,
                              pzmaComposite, vDialog,
                              nOffsetX, nOffsetY, pCtrlDef ) );
}
