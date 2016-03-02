/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlbmp.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Bitmap control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.28    DKS   (HH)
//    For Remote Server, call  BMP_SetBitmapFileName in MapFromOI.
//
// 2001.10.22   FH
//    set szImageURL to lower case
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.05.31    DKS   R55199
//    Attempt to load bitmap from source when running interpretively.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2001.03.27    DKS   Helmut
//    Bitmap text can now be altered using SetCtrlText ... and it uses the
//    correct font (final answer).
//
// 2001.01.16    BL    Z10
//    Modified SetBitmapFileName: change return value from void to short
//
// 2000.12.26    BL    Z10
//    Do not set a Border with 3D look
//
// 2000.03.17    TMV   WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 2000.03.09    DKS   Z10    Helmut
//    Controls placed on the Bitmap control are now positioned correctly.
//
// 1999.05.11    DKS   Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.02.16    DKS
//    Used new option zLOADLIB_RESOURCES on SysLoadLibraryWithErrFlag.
//
// 1999.01.25    DKS
//    Bitmaps with controls with "quick keys" now work (TB 441)
//
// 1998.12.29    DKS
//    Bitmap now can be loaded with non-absolute path specification.
//
// 1998.12.04    DKS
//    Focus permitted for controls on a Bitmap.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlBmp.h"
#include "ZdCtlGbl.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[ ] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( ZBmpCtl, CWnd )

BEGIN_MESSAGE_MAP( ZBmpCtl, CWnd )
   ON_WM_CREATE( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_RBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_CTLCOLOR( )
   ON_WM_PALETTECHANGED( )
   ON_WM_QUERYNEWPALETTE( )
   ON_WM_PAINT( )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )

// ZBmpCtl - ctor
ZBmpCtl::ZBmpCtl( ZSubtask *pZSubtask,
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
                  "Bitmap" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZBmpCtl", (zLONG) this );
#endif

   m_pchCtrlBOI = 0;
   m_pchBitmapFileName = 0;
   m_pchVEAPX = 0;
   m_pBmpFile = 0;

   // determine if this button is the default pushbutton for this window
   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style |= WS_BORDER;

// if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
//    Attr.Style &= ~WS_TABSTOP;
// else
   {
      Attr.Style |= WS_TABSTOP;
      m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   }

   if ( pCtrlDef->Subtype & 0x00000001 )
      m_bFit = TRUE;
   else
      m_bFit = FALSE;

   if ( pCtrlDef->CtrlBOI == 0 )  // there is no CtrlBOI
      return;

#ifdef zREMOTE_SERVER

   ZXRA_Item *pXRA;

   if ( m_pZSubtask->m_pZSocketTask &&
        (pXRA = m_pZSubtask->m_pZSocketTask->
            m_pCacheWndList->Find( *(m_pZSubtask->m_pzsRemoteWndTag),
                                   m_pZSubtask,
                                   "ZBmpCtl::ZBmpCtl" )) != 0 &&
        pXRA->m_vXRA )
   {
      zVIEW  vXRA;
      zCHAR  szImageURL[ zMAX_FILENAME_LTH ];
      zSHORT nLth;

      fnGetImageURL( m_pZSubtask, szImageURL, zsizeof( szImageURL ) );
      nLth = zstrlen( szImageURL );
#if 0  //HH
      if ( nLth > 0 && szImageURL[ nLth - 1 ] != '/' &&
           szImageURL[ nLth - 1 ] != '\\/' )
      {
         // append backslash, if the URL does not end with back or
         // forward slash, the lightdriver will convert back slashes
         // to forward slashes, if neccessary.
         strcat_s( szImageURL, zsizeof( szImageURL ), "\\" );
         nLth++;
      }
#endif
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
#if 0 //HH
            zCHAR szBitmap[ zMAX_FILENAME_LTH ];
            SetRemoteZCtrlAttribute( this, "Ctrl", "Fix", "Y" );
            GetStringFromAttribute( szBitmap, zsizeof( szBitmap ), vXRA, "Bitmap", "URL" );
            // convert the bitmap string to lowercase, if triggered by
            //  Zeidon.ini
            // [Workstation]
            // LowerCaseURL=Y
            if ( fnGetLowerCaseUrl( m_pZSubtask ) == 'Y' )
               SysTranslateString( szBitmap, 'L' );

            strcpy_s( szImageURL + nLth, zsizeof( szImageURL - nLth ), szBitmap );
            SetRemoteZCtrlAttribute( this, "Bitmap", "URL", szImageURL );
#endif
            SetRemoteZCtrlAttribute( this, "Ctrl", "Fix", "Y" );
            GetStringFromAttribute( szImageURL + nLth, vXRA, "Bitmap", "URL" );
            SysTranslateString( szImageURL, 'L' );
            SetRemoteZCtrlAttribute( this, "Bitmap", "URL", szImageURL );
         }
      }
   }

#else

   zULONG  ulLth;

   GetAttributeLength( &ulLth, vDialog, szlCtrl, szlCtrlBOI );
   m_pchCtrlBOI = new char[ ulLth ];
   zmemcpy( m_pchCtrlBOI, (zPVOID) pCtrlDef->CtrlBOI, ulLth );

   if ( pzmaComposite &&
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) == 0 )
   {
      m_ulMapActFlag2 &= ~(zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   }
   else
#endif
   {
      CreateZ( );
   }

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );
   CreateZeidonCtrls( vSubobject, pZSubtask,
                      this, this, 0, 0, TRUE );
//x before  DKS       (zSHORT) pCtrlDef->PSDLG_X,
//x ActiveX 2000/     (zSHORT) pCtrlDef->PSDLG_Y, TRUE );
//  change  03/02     pWndParent, this, 0, 0, TRUE );
   DropView( vSubobject );
}

void
ZBmpCtl::CreateZ( )
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
         CreateEx( 0, "ZeidonCustomCtrl", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZBmpCtl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZBmpCtl::OnCreate ", *m_pzsTag );
#endif

   LPLIBRARY hLibrary;
   HINSTANCE hInst = 0;
   zPCHAR    pch = m_pchCtrlBOI;
   int       nRC;

   if ( (nRC = (int) Default( )) == -1 )
      return( -1 );

   SetFontOverride( );
   if ( (m_pchBitmapFileName == 0 || m_pchBitmapFileName[ 0 ] == 0) )
   {
      if ( pch && *pch )   // DLL_Name
      {
         if ( m_pZSubtask->m_vInterpSubtask )
         {
            // Do not even try to load the bitmap from the DLL if running
            // interpretively.
         }
         else
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

      LoadDib( hInst, pch );
   }

// mDeleteInitA( m_pchCtrlBOI );
   return( nRC );
}

BOOL
ZBmpCtl::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZBmpCtl::DestroyWindow ", *m_pzsTag );
#endif

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

// load a bitmap from a CtrlBOI according to the following rules:
//    1. if there is a res id (and hDLL_Library is non-zero) attempt to
//       load the bitmap using the res id from the DLL.  If successful,
//       quit; otherwise go to rule 2.
//    2. if there is a name (and hDLL_Library is non-zero) attempt to load
//       the bitmap using the resource name from the DLL.  If successful,
//       quit; otherwise, go to rule 3.
//    3. attempt to load the bitmap assuming the name is a fully qualified
//       bitmap file name.
//    4. attempt to load the bitmap using the application directory.
//    5. attempt to load the bitmap using ResourcePath in the Zeidon.ini file.
//
// If successful, m_pBitmap will be non-zero.  The return pointer is
// the pointer to the next bitmap specification in the Ctrl BOI.
//
zBOOL
ZBmpCtl::LoadDib( HINSTANCE hInstDLL,
                  zPCHAR   pchBitmap )
{
   zCHAR szFileName[ zMAX_FILENAME_LTH ];
   zLONG lLth;

   mDeleteInitA( m_pchBitmapFileName );
   if ( pchBitmap == 0 || *pchBitmap == 0 )  // nothing left in the Ctrl BOI
      return( FALSE );

   zPCHAR pchSemiColon = zstrchr( pchBitmap, ';' );

   if ( pchSemiColon )
      pchSemiColon++;
   else
      pchSemiColon = pchBitmap;

   if ( pchSemiColon && *pchSemiColon == '@' ) // V.E.A.P.X reference
   {
      GetVEAPX_Reference( m_pZSubtask->m_vDialog, 0, pchSemiColon,
                          szFileName, zsizeof( szFileName ) );
      if ( *szFileName )
      {
         if ( m_pchVEAPX == 0 || zstrcmp( pchSemiColon, m_pchVEAPX ) != 0 )
         {
            mDeleteInitA( m_pchVEAPX );
            lLth = zstrlen( pchSemiColon ) + 1;
            m_pchVEAPX = new char[ lLth ];
            strcpy_s( m_pchVEAPX, lLth, pchSemiColon );
         }

         pchSemiColon = szFileName;
         hInstDLL = 0;
      }
   }

   lLth = zstrlen( pchSemiColon ) + 1;
   m_pchBitmapFileName = new char[ lLth ];
   strcpy_s( m_pchBitmapFileName, lLth, pchSemiColon );

   zSHORT nFound = 0;
   UINT   uResId = 0;

   while ( hInstDLL )  // purist's goto
   {
      if ( (isdigit( *pchBitmap ) || (pchSemiColon != 0)) &&
           (uResId = (zUSHORT) zatol( pchBitmap )) != 0 )
      {
         if ( m_dib.Load( hInstDLL, uResId ) )
         {
            nFound = 1;
            break;
         }
      }

      if ( m_dib.Load( hInstDLL, pchSemiColon ) )
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
            TraceLine( "ZBmpCtl::LoadDib: Cannot access bitmap "
                         "res id: %d - Cannot access resource: %s",
                       uResId, pchSemiColon );
         }
      }
      else
      if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
      {
         if ( nFound == 1 )
         {
            TraceLineI( "ZBmpCtl::LoadDib: Bitmap loaded, "
                        "res id ", uResId );
         }
         else
      // if ( nFound == 2 )
         {
            TraceLine( "ZBmpCtl::LoadDib: Cannot access bitmap "
                         "res id: %d - Bitmap loaded from resource: %s",
                       uResId, pchSemiColon );
         }
      }
   }

   if ( nFound )
      return( TRUE );

   if ( pchSemiColon )
   {
      if ( *pchSemiColon == '.' && *(pchSemiColon + 1) == '\\' )
         pchSemiColon += 2;

      if ( m_dib.Load( pchSemiColon ) )
         return( TRUE );
      else
      if ( pchSemiColon[ 0 ] && pchSemiColon[ 1 ] != ':' &&
           (pchSemiColon[ 0 ] != '\\' && pchSemiColon[ 1 ] != '\\') )
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
               strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
               SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
               if ( m_dib.Load( szWorkString ) )
               {
                  if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "ZBmpCtl::LoadDib: Bitmap loaded, "
                                 "file in Application Dir ", szWorkString );
                  }

                  return( TRUE );
               }

               mDeleteInit( m_pBmpFile );
               m_pBmpFile = new ZBmpFile( );
               m_pBmpFile->LoadAnImageX( szWorkString );
               if ( m_pBmpFile->m_pBuf )
               {
                  if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "ZBmpCtl::LoadDib: Bitmap loaded from "
                                 "file in Application Dir ", szWorkString );
                  }

                  return( TRUE );
               }
               else
                  mDeleteInit( m_pBmpFile );

               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
               {
                  TraceLineS( "ZBmpCtl::LoadDib: Cannot access bitmap "
                              "file in Application Dir ", szWorkString );
               }
            }
         }

         SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, zsizeof( szZeidonPath ) );
         if ( szZeidonPath[ 0 ] )
         {
            SysAppendcDirSep( szZeidonPath );
            strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
            SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
            if ( m_dib.Load( szWorkString ) )
            {
               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
               {
                  TraceLineS( "ZBmpCtl::LoadDib: Bitmap loaded, file in ResourcePath ", szWorkString );
               }

               return( TRUE );
            }

            mDeleteInit( m_pBmpFile );
            m_pBmpFile = new ZBmpFile( );
            m_pBmpFile->LoadAnImageX( szWorkString );
            if ( m_pBmpFile->m_pBuf )
            {
               if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
               {
                  TraceLineS( "ZBmpCtl::LoadDib: Bitmap loaded from file in ResourcePath ", szWorkString );
               }

               return( TRUE );
            }
            else
               mDeleteInit( m_pBmpFile );

            if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
            {
               TraceLineS( "ZBmpCtl::LoadDib: Cannot access bitmap file in ResourcePath ", szWorkString );
            }
         }

         if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
         {
            TraceLineS( "ZBmpCtl::LoadDib: Cannot access bitmap file ", pchSemiColon );
         }
      }
      else
      {
         mDeleteInit( m_pBmpFile );
         m_pBmpFile = new ZBmpFile( );
         m_pBmpFile->LoadAnImageX( pchSemiColon );
         if ( m_pBmpFile->m_pBuf )
         {
            if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
            {
               TraceLineS( "ZBmpCtl::LoadDib: Bitmap loaded from ", pchSemiColon );
            }

            return( TRUE );
         }
         else
            mDeleteInit( m_pBmpFile );
      }
   }

   return( FALSE );
}

zSHORT
ZBmpCtl::MapFromOI( WPARAM wFlag )
{
// TraceLineS( "ZBmpCtl::MapFromOI ", *m_pzsTag );

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( (m_pchBitmapFileName == 0 || m_pchBitmapFileName[ 0 ] == 0) &&
        m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW vApp;

      if ( GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         zPCHAR pch;

         GetAddrForAttribute( &pch, vApp, *m_pzsEName, *m_pzsAName );

#ifdef zREMOTE_SERVER
         zCHAR szBitmap[ 512 ];

         strcpy_s( szBitmap, zsizeof( szBitmap ), pch );
         BMP_SetBitmapFileName( m_pZSubtask->m_vDialog, *m_pzsTag, szBitmap );
#else
         SetBitmapFileName( pch );
#endif
      }
   }

   if ( m_pchVEAPX )
   {
      if ( m_dib.m_bBitmapLoaded )
      {
         m_dib.DeleteObject( );
         m_dib.m_bBitmapLoaded = FALSE;
      }

      LoadDib( 0, m_pchVEAPX );
   }
   else
   if ( m_pchBitmapFileName && *m_pchBitmapFileName && m_dib.m_bBitmapLoaded == FALSE )
   {
      m_dib.Load( m_pchBitmapFileName );
   }

   return( 0 );
}

zSHORT
ZBmpCtl::SetBitmapFileName( zCPCHAR cpcBitmapFileName )
{
   if ( m_dib.m_bBitmapLoaded == FALSE || m_pchBitmapFileName == 0 ||
        zstrcmp( cpcBitmapFileName, m_pchBitmapFileName ) != 0 )
   {
      if ( m_dib.m_bBitmapLoaded )
      {
         m_dib.DeleteObject( );
      }

      mDeleteInitA( m_pchBitmapFileName );

      zLONG lLth = zstrlen( cpcBitmapFileName ) + 1;
      m_pchBitmapFileName = new char[ lLth ];
      strcpy_s( m_pchBitmapFileName, lLth, cpcBitmapFileName );
      if ( m_dib.Load( m_pchBitmapFileName ) )
      {
         if ( mIs_hWnd( m_hWnd ) )
            Invalidate( TRUE );
      }
      else
      {
         return( -1 );
      }
   }

   return( 0 );
}

void
ZBmpCtl::SetBitmapResource( zCPCHAR cpcDLL_Name,
                            UINT    uBitmapResId )
{
   LPLIBRARY hLibrary;
   HINSTANCE hInst = 0;

   if ( m_dib.m_bBitmapLoaded )
   {
      m_dib.DeleteObject( );
   }

   if ( m_pZSubtask->m_pZTask->m_uAppState & zAPP_STATE_NT )
   {
      if ( *cpcDLL_Name )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               cpcDLL_Name, zLOADLIB_RESOURCES );
         hInst = (HINSTANCE) SysGetInstance( hLibrary );
      }
   }
   else
   {
      zFARPROC_HINSTANCE lpfnDynRoutine = 0;

      if ( *cpcDLL_Name )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               cpcDLL_Name, 0 );

         lpfnDynRoutine = (zFARPROC_HINSTANCE)
            GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                            (LPLPLIBRARY) &hLibrary,
                                            cpcDLL_Name, "GetDLL_Instance",
                                            0 );  // no error message
         if ( lpfnDynRoutine )
            hInst = (*lpfnDynRoutine)( m_pZSubtask->m_vDialog );
      }
   }

   if ( hInst )
   {
      m_dib.Load( hInst, uBitmapResId );
   }

   Invalidate( TRUE );
}

zLONG
ZBmpCtl::SetZCtrlState( zLONG lStatusType,
                        zULONG ulValue )
{
   if ( lStatusType == zCONTROL_STATUS_ENABLED || lStatusType == zCONTROL_STATUS_VISIBLE )
   {
      ZMapAct::SetChildCtrlState( lStatusType, ulValue );
   }

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

zBOOL
ZBmpCtl::InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus )
{
// TraceLineS( "ZBmpCtl InformCompositeOfSetFocus pzma = ",
//             *(pzmaReceivingFocus->m_pzsTag) );
   return( TRUE );
}

zBOOL
ZBmpCtl::InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus )
{
// TraceLineS( "ZBmpCtl InformCompositeOfKillFocus pzma = ",
//             *(pzmaLosingFocus->m_pzsTag) );

   // Look for the next/prev sub-control that can take focus.  If there is
   // one, set focus to it, otherwise look for a control at the same level
   // as this composite and set focus to it.
   //
   zBOOL bTab = (GetKeyState( VK_TAB ) < 0);
   if ( bTab == FALSE )
      return( TRUE );

   zBOOL bShift = (GetKeyState( VK_SHIFT ) < 0);
   ZMapAct *pzmaFocus = m_pZSubtask->GetFocusCtrlWithinComposite( this, pzmaLosingFocus, !bShift );
   if ( pzmaFocus )
   {
      if ( mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }
   else
   {
      pzmaFocus = m_pZSubtask->GetFocusCtrlWithinComposite( 0, this, !bShift );
      if ( pzmaFocus && mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }

   return( TRUE );
}

void
ZBmpCtl::OnSetFocus( CWnd *pWndLostFocus )
{
// TraceLineS( "ZBmpCtl OnSetFocus", "" );
   m_pZSubtask->InformDriverOfSetFocus( this, FALSE );
// UpdatePalette( TRUE );
   Default( );              // pass it along to parent

   // Look for a sub-control that can take focus.  If there is one, set
   // focus to it, otherwise look for a control at the same level as this
   // composite and set focus to it.
   zBOOL bShift = (GetKeyState( VK_SHIFT ) < 0);
   ZMapAct *pzma = 0;

   if ( bShift && pWndLostFocus )
   {
      zSHORT k = 1;

      while ( (pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( k )) != 0 )
      {
         if ( pzma && pzma->m_pCtrl &&
              pzma->m_pCtrl->m_hWnd == pWndLostFocus->m_hWnd )
         {
            if ( pzma->m_pzmaComposite == this )
               pzma = 0;

            break;
         }

         k++;
      }
   }

   ZMapAct *pzmaFocus = m_pZSubtask->GetFocusCtrlWithinComposite( this, pzma, !bShift );
   if ( pzmaFocus )
   {
      if ( mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }
   else
   {
      pzmaFocus = m_pZSubtask->GetFocusCtrlWithinComposite( 0, this, !bShift );
      if ( pzmaFocus && mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }
}

void
ZBmpCtl::OnKillFocus( CWnd *pWndGetFocus )
{
// TraceLineS( "ZBmpCtl::OnKillFocus", "" );

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );              // pass it along to parent
}

HBRUSH
ZBmpCtl::CtlColor( CDC *pDC, UINT nCtlColor )
{
// TraceLineS( "ZBmpCtl::CtlColor: ", *m_pzsTag );
   HBRUSH hBrush = (HBRUSH) Default( );

   if ( m_clrText )
      pDC->SetTextColor( m_clrText );

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }

   return( hBrush );
}

//
// Somebody changed the palette. If it's not us, then we need to update.
//
void
ZBmpCtl::OnPaletteChanged( CWnd *pFocusWnd )
{
// if ( pFocusWnd != this )
//    UpdatePalette( TRUE ); // pass FALSE to UpdateColors( ) instead of repaint
}

//
// We need to re-realize the logical palette each time we regain focus
//
BOOL
ZBmpCtl::OnQueryNewPalette( )
{
   TraceLineS( "ZBmpCtl::OnQueryNewPalette", "" );
   return( FALSE );
// return( UpdatePalette( TRUE ) );
}

//
// Responds to an incoming Paint message by redrawing the bitmap.
// Note that we Invalidate( ) ourselves with false to avoid the background
// paint flicker.
//
void
ZBmpCtl::OnPaint( )
{
// TraceLineS( "ZBmpCtl::OnPaint", "*************************" );
   if ( m_pBmpFile )
   {
      m_pBmpFile->DrawBMP( this, m_bFit );
      return;
   }

   CPaintDC dc( this );
   CRect rectSrc;
   CRect rectTgt;

   rectTgt.top = 0;
   rectTgt.left = 0;
   rectTgt.right = Attr.W;
   rectTgt.bottom = Attr.H;

   if ( m_bFit == FALSE )
   {
      CSize size = m_dib.GetSize( );
      rectSrc.top = 0;
      rectSrc.left = 0;
      rectSrc.bottom = size.cy;
      rectSrc.right = size.cx;
   }

   if ( m_dib.m_bBitmapLoaded == FALSE ||
        m_dib.Draw( dc, m_bFit ? &rectTgt : 0, m_bFit ? 0 : &rectSrc,
                    FALSE, 0, TRUE ) == FALSE )
   {
      COLORREF clrBkSave = dc.SetBkColor( ::GetSysColor( COLOR_3DFACE ) );
      CFont    *pFont = 0;
      CFont    *pFontOld = 0;

      if ( m_pZSubtask->m_pFontCtrl )
         pFont = m_pZSubtask->m_pFontCtrl;
      else
      if ( m_pZSubtask->m_pFontWS )
         pFont = m_pZSubtask->m_pFontWS;

      if ( pFont )
         pFontOld = dc.SelectObject( pFont );

      if ( m_pzsText && *m_pzsText )
         dc.DrawText( *m_pzsText, -1, rectTgt, DT_CENTER | DT_WORDBREAK );
      else
         dc.DrawText( "Unknown BmpCtl", -1, rectTgt, DT_CENTER | DT_WORDBREAK );

      if ( pFontOld )
         dc.SelectObject( pFontOld );

      dc.SetBkColor( clrBkSave );
   }
}

void
ZBmpCtl::OnLButtonDown( UINT uModKeys, CPoint pt )
{
// UpdatePalette( TRUE );
   ProcessEvent( this, 1 );
}

void
ZBmpCtl::OnLButtonUp( UINT uModKeys, CPoint pt )
{
   ProcessEvent( this, 2 );
}

void
ZBmpCtl::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
   ProcessEvent( this, 3 );
}

void
ZBmpCtl::OnRButtonDown( UINT uModKeys, CPoint pt )
{
// UpdatePalette( TRUE );
   ProcessEvent( this, 4 );
}

void
ZBmpCtl::OnRButtonUp( UINT uModKeys, CPoint pt )
{
   ProcessEvent( this, 5 );
}

void
ZBmpCtl::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
   ProcessEvent( this, 6 );
}

BOOL
ZBmpCtl::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN )
   {
#ifdef DEBUG_ALL
      TraceLineS( "ZBmpCtl - fnIsAccelerator ", "????" );
#endif
      if ( fnIsAccelerator( m_pZSubtask, pMsg ) )
      {
         return( TRUE );
      }

      ZMapAct *pzma;
      zACCEL_DEF ad;
      ad.uVK = pMsg->wParam;
      ad.fAlt = (unsigned) (GetKeyState( VK_MENU ) < 0);
      ad.fCtrl = (unsigned) (GetKeyState( VK_CONTROL ) < 0);
      ad.fShift = (unsigned) (GetKeyState( VK_SHIFT ) < 0);

      if ( ad.uVK != VK_MENU && ad.fAlt &&
           (pzma = m_pZSubtask->FindCtrlWithQuickKey( this, (char) ad.uVK )) != 0 &&
           pzma->HandleQuickKey( ad ) )
      {
         return( TRUE );
      }
      else
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZBmpCtl - fnIsAccelerator return ", "FALSE" );
#endif
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

void
ZBmpCtl::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZBmpCtl - dtor
ZBmpCtl::~ZBmpCtl( )
{
// TraceLineS( "ZBmpCtl::~ZBmpCtl", "" );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

   mDeleteInitA( m_pchCtrlBOI );
   mDeleteInitA( m_pchBitmapFileName );
   mDeleteInitA( m_pchVEAPX );
   mDeleteInit( m_pBmpFile );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ZBmpFile class to handle the reading and writing of BMP files.
//
ZBmpFile::ZBmpFile( )
{
   // Keep a single global image in memory
   m_pBuf = 0;        // where we keep our image data
   m_width = 0;       // image dimensions
   m_height = 0;
// m_widthDW = 0;

   m_bits = 8;
   m_quantColors = 64;
   m_color = TRUE;
   m_errorText = "OK";
}

ZBmpFile::~ZBmpFile( )
{
   // clean up
   mDeleteInitA( m_pBuf );
}

////////////////////////////////////////////////////////////////////////////
// load a .BMP file - 1, 4, 8, 24 bit
//
// allocates and returns an RGB buffer containing the image.
// modifies width and height accordingly - 0, 0, 0 on error

BYTE *
ZBmpFile::LoadBMP( CString csFileName,
                   UINT *width,
                   UINT *height )
{
   BITMAP inBM;

   BYTE m1,m2;
   long filesize;
   short res1,res2;
   long pixoff;
   long bmisize;
   long compression;
   unsigned long sizeimage;
   long xscale, yscale;
   long colors;
   long impcol;
   BYTE *outBuf = 0;

   // for safety
   *width = 0; *height = 0;

   // init
   m_errorText = "OK";
   m_bytesRead = 0;

   FILE *fp;

   fp = fopen( csFileName, "rb" );
   if ( fp == 0 )
   {
      CString msg;
      msg = "Can't open file for reading :\n" + csFileName;
      m_errorText = msg;
      return( 0 );
   }
   else
   {
      long rc;
      rc = fread( (BYTE *) &(m1), 1, 1, fp );
      m_bytesRead += 1;
      if ( rc == -1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (BYTE *) &(m2), 1, 1, fp );
      m_bytesRead += 1;
      if ( rc == -1 )
         m_errorText = "Read Error!";

      if ( (m1 != 'B') || (m2 != 'M') )
      {
         m_errorText = "Not a valid BMP File";
         fclose( fp );
         return( 0 );
      }

      ////////////////////////////////////////////////////////////////////////////
      //
      // read a ton of header stuff

      rc = fread( (long *) &(filesize), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (int *) &(res1), 2, 1, fp );
      m_bytesRead += 2;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (int *) &(res2), 2, 1, fp );
      m_bytesRead += 2;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(pixoff), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(bmisize), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(inBM.bmWidth), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(inBM.bmHeight), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (int *) &(inBM.bmPlanes), 2, 1, fp );
      m_bytesRead += 2;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (int *) &(inBM.bmBitsPixel), 2, 1, fp );
      m_bytesRead += 2;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(compression), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(sizeimage), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(xscale), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(yscale), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(colors), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      rc = fread( (long *) &(impcol), 4, 1, fp );
      m_bytesRead += 4;
      if ( rc != 1 )
      {
         m_errorText = "Read Error!";
         fclose( fp );
         return( 0 );
      }

      ////////////////////////////////////////////////////////////////////////////
      // I don't do RLE files

      if ( compression!= BI_RGB )
      {
         m_errorText = "This is a compressed file.";
         fclose( fp );
         return( 0 );
       }

      if ( colors == 0 )
      {
         colors = 1 << inBM.bmBitsPixel;
      }

      ////////////////////////////////////////////////////////////////////////////
      // read colormap

      RGBQUAD *colormap = 0;

      switch ( inBM.bmBitsPixel )
      {
      case 24:
         break;
         // read pallete
      case 1:
      case 4:
      case 8:
         colormap = new RGBQUAD[ colors ];
         if ( colormap == 0 )
         {
            fclose( fp );
            m_errorText = "Out of memory";
            return( 0 );
         }

         int k;
         for ( k = 0; k < colors; k++ )
         {
            BYTE r,g,b, dummy;

            rc = fread( (BYTE *) &(b), 1, 1, fp );
            m_bytesRead++;
            if ( rc != 1 )
            {
               m_errorText = "Read Error!";
               delete [] colormap;
               fclose( fp );
               return( 0 );
            }

            rc = fread( (BYTE *) &( g ),1,1,fp );
            m_bytesRead++;
            if ( rc != 1 )
            {
               m_errorText = "Read Error!";
               delete [] colormap;
               fclose( fp );
               return( 0 );
            }

            rc = fread( (BYTE *) &( r ),1,1,fp );
            m_bytesRead++;
            if ( rc != 1 )
            {
               m_errorText = "Read Error!";
               delete [] colormap;
               fclose( fp );
               return( 0 );
            }


            rc = fread( (BYTE *) &( dummy ),1,1,fp );
            m_bytesRead++;
            if ( rc != 1 )
            {
               m_errorText = "Read Error!";
               delete [] colormap;
               fclose( fp );
               return( 0 );
            }

            colormap[ k ].rgbRed = r;
            colormap[ k ].rgbGreen = g;
            colormap[ k ].rgbBlue = b;
         }
         break;
      }

      if ( (long) m_bytesRead>pixoff )
      {
         fclose( fp );
         m_errorText = "Corrupt palette";
         delete [] colormap;
         fclose( fp );
         return( 0 );
      }

      while ( (long) m_bytesRead<pixoff )
      {
         char dummy;
         fread( &dummy, 1, 1, fp );
         m_bytesRead++;
      }

      int w = inBM.bmWidth;
      int h = inBM.bmHeight;

      // set the output params
      *width = w;
      *height = h;

      long row_size = w * 3;

      long bufsize = (long) w * 3 * (long) h;

      ////////////////////////////////////////////////////////////////////////////
      // alloc our buffer

      outBuf = (BYTE *)  new BYTE [ bufsize ];
      if ( outBuf == 0 )
      {
         m_errorText = "Memory alloc Failed";
      }
      else
      {
         ////////////////////////////////////////////////////////////////////////////
         // read it

         long row = 0;
         long rowOffset = 0;

         // read rows in reverse order
         for ( row = inBM.bmHeight - 1; row >= 0; row-- )
         {

            // which row are we working on?
            rowOffset = (long unsigned) row * row_size;

            if ( inBM.bmBitsPixel == 24 )
            {
               for ( int col = 0; col < w; col++ )
               {
                  long offset = col * 3;
                  char pixel[ 3 ];

                  if ( fread( (void *) (pixel), 1, 3, fp ) == 3 )
                  {
                     // we swap red and blue here
                     *(outBuf + rowOffset + offset + 0) = pixel[ 2 ];  // r
                     *(outBuf + rowOffset + offset + 1) = pixel[ 1 ];  // g
                     *(outBuf + rowOffset + offset + 2) = pixel[ 0 ];  // b
                  }
               }

               m_bytesRead+= row_size;

               // read DWORD padding
               while ( (m_bytesRead - pixoff) & 3 )
               {
                  char dummy;
                  if ( fread( &dummy, 1, 1, fp ) != 1 )
                  {
                     m_errorText = "Read Error";
                     delete [] outBuf;
                     fclose( fp );
                     return( 0 );
                  }

                  m_bytesRead++;
               }
            }
            else
            {
               // 1, 4, or 8 bit image

               ////////////////////////////////////////////////////////////////
               // pixels are packed as 1 , 4 or 8 bit vals. need to unpack them

               int bit_count = 0;
               UINT mask = (1 << inBM.bmBitsPixel) - 1;

               BYTE inbyte = 0;

               for ( int col = 0;col<w;col++ ) {

                  int pix = 0;

                  // if we need another byte
                  if ( bit_count <= 0 )
                  {
                     bit_count = 8;
                     if ( fread( &inbyte, 1, 1, fp ) != 1 )
                     {
                        m_errorText = "Read Error";
                        delete [] outBuf;
                        delete [] colormap;
                        fclose( fp );
                        return( 0 );
                     }

                     m_bytesRead++;
                  }

                  // keep track of where we are in the bytes
                  bit_count -= inBM.bmBitsPixel;
                  pix = (inbyte >> bit_count) & mask;

                  // lookup the color from the colormap - stuff it in our buffer
                  // swap red and blue
                  *(outBuf + rowOffset + col * 3 + 2) = colormap[ pix ].rgbBlue;
                  *(outBuf + rowOffset + col * 3 + 1) = colormap[ pix ].rgbGreen;
                  *(outBuf + rowOffset + col * 3 + 0) = colormap[ pix ].rgbRed;
               }

               // read DWORD padding
               while ( (m_bytesRead - pixoff) & 3 )
               {
                  char dummy;
                  if ( fread( &dummy, 1, 1, fp ) != 1 )
                  {
                     m_errorText = "Read Error";
                     delete [] outBuf;
                     if ( colormap )
                        delete [] colormap;

                     fclose( fp );
                     return( 0 );
                  }

                  m_bytesRead++;
               }
            }
         }
      }

      if ( colormap )
      {
         delete [] colormap;
      }

      fclose( fp );
   }

   return( outBuf );
}

////////////////////////////////////////////////////////////////////////////
// write a 24-bit BMP file
//
// image MUST be a packed buffer (not DWORD-aligned)
// image MUST be vertically flipped !
// image MUST be BGR, not RGB !
//

void
ZBmpFile::SaveBMP( CString csFileName,    // output path
                   BYTE * buf,          // BGR buffer
                   UINT width,          // pixels
                   UINT height )
{
   short res1 = 0;
   short res2 = 0;
   long pixoff = 54;
   long compression = 0;
   long cmpsize = 0;
   long colors = 0;
   long impcol = 0;
   char m1 = 'B';
   char m2 = 'M';

   m_errorText = "OK";

   DWORD widthDW = WIDTHBYTES( width * 24 );

   long bmfsize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + widthDW * height;
   long byteswritten = 0;

   BITMAPINFOHEADER header;
   header.biSize = 40;                   // header size
   header.biWidth = width;
   header.biHeight = height;
   header.biPlanes = 1;
   header.biBitCount = 24;               // RGB encoded, 24 bit
   header.biCompression = BI_RGB;        // no compression
   header.biSizeImage = 0;
   header.biXPelsPerMeter = 0;
   header.biYPelsPerMeter = 0;
   header.biClrUsed = 0;
   header.biClrImportant = 0;

   FILE *fp;
   fp = fopen( csFileName,"wb" );
   if ( fp == 0 )
   {
      m_errorText = "Can't open file for writing";
      return;
   }

   // should probably check for write errors here...

   fwrite( (BYTE *) &( m1 ), 1, 1, fp );
   byteswritten += 1;
   fwrite( (BYTE *) &( m2 ), 1, 1, fp );
   byteswritten += 1;
   fwrite( (long *) &( bmfsize ), 4, 1, fp );
   byteswritten += 4;
   fwrite( (int *) &( res1 ), 2, 1, fp );
   byteswritten += 2;
   fwrite( (int *) &( res2 ), 2, 1, fp );
   byteswritten += 2;
   fwrite( (long *) &( pixoff ), 4, 1, fp );
   byteswritten += 4;

   fwrite( (BITMAPINFOHEADER *) &header, sizeof( BITMAPINFOHEADER ), 1, fp );
   byteswritten += sizeof( BITMAPINFOHEADER );

   long row = 0;
   long rowidx;
   long row_size;
   row_size = header.biWidth * 3;
   long rc;

   for ( row = 0;row < header.biHeight; row++ )
   {
      rowidx = (long unsigned) row * row_size;

      // write a row
      rc = fwrite( (void *) (buf + rowidx), row_size, 1, fp );
      if ( rc != 1 )
      {
         m_errorText = "fwrite error\nGiving up";
         break;
      }

      byteswritten += row_size;

      // pad to DWORD
      for ( DWORD count = row_size; count < widthDW; count++ )
      {
         char dummy = 0;
         fwrite( &dummy, 1, 1, fp );
         byteswritten++;
      }
   }

   fclose( fp );
}

////////////////////////////////////////////////////////////////////////////////////
//
// 1,4,8 bit BMP stuff
//
// if you have a color-mapped image and a color map...
//
// the BMP saving code in SaveColorMappedBMP modified from Programming
// for Graphics Files in C and C++, by John Levine.

void
ZBmpFile::SaveBMP( CString csFileName,        // output path
                   BYTE * colormappedbuffer, // one BYTE per pixel colomapped image
                   UINT width,
                   UINT height,
                   int bitsperpixel,        // 1, 4, 8
                   int colors,              // number of colors (number of RGBQUADs)
                   RGBQUAD *colormap )      // array of RGBQUADs
{
   int datasize, cmapsize, byteswritten, row, col;

   m_errorText = "OK";

   if ( bitsperpixel == 24 )
   {
      // the routines could be combined, but k don't feel like it
      m_errorText = "We don't do 24-bit files in here, sorry";
      return;
   }
   else
      cmapsize = colors * 4;

   datasize = BMP_PIXELSIZE( width, height, bitsperpixel );

   long filesize = BMP_HEADERSIZE + cmapsize + datasize;
   int res1, res2;
   res1 = res2 = 0;

   long pixeloffset = BMP_HEADERSIZE + cmapsize;

   int bmisize = 40;
   long cols = width;
   long rows = height;
   WORD planes = 1;
   long compression = 0;
   long cmpsize = datasize;
   long xscale = 0;
   long yscale = 0;
   long impcolors = colors;

   FILE *fp;
   fp = fopen( csFileName, "wb" );
   if ( fp == 0 )
   {
      m_errorText = "Can't Open";
      return;
   }

   char bm[ 2 ];
   bm[ 0 ] = 'B';
   bm[ 1 ] = 'M';

   // header stuff
   BITMAPFILEHEADER bmfh;
   bmfh.bfType = *(WORD *) &bm;
   bmfh.bfSize = filesize;
   bmfh.bfReserved1 = 0;
   bmfh.bfReserved2 = 0;
   bmfh.bfOffBits = pixeloffset;

   fwrite( &bmfh, sizeof (BITMAPFILEHEADER), 1, fp );


   BITMAPINFOHEADER bmih;
   bmih.biSize = bmisize;
   bmih.biWidth = cols;
   bmih.biHeight = rows;
   bmih.biPlanes = planes;
   bmih.biBitCount = bitsperpixel;
   bmih.biCompression = compression;
   bmih.biSizeImage = cmpsize;
   bmih.biXPelsPerMeter = xscale;
   bmih.biYPelsPerMeter = yscale;
   bmih.biClrUsed = colors;
   bmih.biClrImportant = impcolors;

   fwrite( &bmih, sizeof ( BITMAPINFOHEADER ), 1, fp );

   if ( cmapsize )
   {
      int k;
      for ( k = 0; k< colors; k++ )
      {
         putc( colormap[ k ].rgbRed, fp );
         putc( colormap[ k ].rgbGreen, fp );
         putc( colormap[ k ].rgbBlue, fp );
         putc( 0, fp ); // dummy
      }
   }

   byteswritten = BMP_HEADERSIZE + cmapsize;

   for ( row = 0; row < (int) height; row++ )
   {
      int pixbuf;
      int nbits = 0;

      for ( col = 0 ; col < (int) width; col++ )
      {
         int offset = row * width + col;  // offset into our color-mapped RGB buffer
         BYTE pval = *(colormappedbuffer + offset);

         pixbuf = (pixbuf << bitsperpixel) | pval;

         nbits += bitsperpixel;

         if ( nbits > 8 )
         {
            m_errorText = "Error : nBits > 8????";
            fclose( fp );
            return;
         }

         if ( nbits == 8 )
         {
            putc( pixbuf, fp );
            pixbuf = 0;
            nbits = 0;
            byteswritten++;
         }
      } // cols

      if ( nbits > 0 )
      {
         putc( pixbuf, fp );  // write partially filled byte
         byteswritten++;
      }

      // DWORD align
      while ( (byteswritten - pixeloffset) & 3 )
      {
         putc( 0, fp );
         byteswritten++;
      }

   }  //rows

   if ( byteswritten != filesize )
   {
      m_errorText = "byteswritten != filesize";
   }

   fclose( fp );
}

//
// This has been modified from Dennis Lee's original version
//

/*
 * DL1 Quantization
 * ================
 *
 * File: dl1quant.c
 * Author: Dennis Lee   E-mail: denlee@ecf.utoronto.ca
 *
 * Copyright (C) 1993-1997 Dennis Lee
 *
 * C implementation of DL1 Quantization.
 * DL1 Quantization is a 2-pass color quantizer optimized for speed.
 * The method was designed around the steps required by a 2-pass
 * quantizer and constructing a model that would require the least
 * amount of extra work.  The resulting method is extremely fast --
 * about half the speed of a memcpy.  That should make DL1 Quant the
 * fastest 2-pass color quantizer.
 *
 * This quantizer's quality is also among the best, slightly
 * better than Wan et al's marginal variance based quantizer.  For
 * more on DL1 Quant's performance and other related information,
 * see DLQUANT.TXT included in this distribution.
 *
 *
 * NOTES
 * =====
 *
 * The dithering code is based on code from the IJG's jpeg library.
 *
 * This source code may be freely copied, modified, and redistributed,
 * provided this copyright notice is attached.
 * Compiled versions of this code, modified or not, are free for
 * personal use.  Compiled versions used in distributed software
 * is also free, but a notification must be sent to the author.
 * An e-mail to denlee@ecf.utoronto.ca will do.
 *
 */

//#define FAST        /* improves speed but uses a lot of memory */
#define QUAL1         /* slightly improves quality */
//#define QUAL2       /* slightly improves quality */

// define *one* of the following dither options:
//#define DITHER1     /* 1-val error diffusion dither */
#define DITHER2       /* 2-val error diffusion dither */
//#define DITHER4     /* 4-val error diffusion dither ( Floyd-Steinberg ) */

#define DITHER_MAX  20

static uchar palette[ 3 ][ 256 ];
static CUBE *rgb_table[ 6 ];
static ushort r_offset[ 256 ], g_offset[ 256 ], b_offset[ 256 ];
static CLOSEST_INFO c_info;
static int tot_colors, pal_index;
static ulong *squares;
static FCUBE *heap = 0;
static short *dl_image = 0;

/* returns 1 on success, 0 on failure */
int dl1quant( uchar *inbuf,
              uchar *outbuf,
              int width,
              int height,
              int quant_to,
              int dither,
              uchar userpal[ 3 ][ 256 ] )
{
   dlq_init( );

   if ( dlq_start( ) == 0 )
   {
      dlq_finish( );
      return( 0 );
   }

   if ( build_table( inbuf, (ulong) width * (ulong) height ) == 0 )
   {
      dlq_finish( );
      return( 0 );
   }

   reduce_table( quant_to );
   set_palette( 0, 0 );

   if ( quantize_image( inbuf, outbuf, width, height, dither ) == 0 )
   {
      dlq_finish( );
      return( 0 );
   }

    dlq_finish( );
    copy_pal( userpal );

    return( 1 );
}

void
copy_pal( uchar userpal[ 3 ][ 256 ] )
{
    int k;

    for ( k = 0; k < 256; k++ ) {
      userpal[ 0 ][ k ] = palette[ 0 ][ k ];
      userpal[ 1 ][ k ] = palette[ 1 ][ k ];
      userpal[ 2 ][ k ] = palette[ 2 ][ k ];
    }
}

void
dlq_init( void )
{
   int k;

   for ( k = 0; k < 6; k++ )
   {
      rgb_table[ k ] = 0;
   }

   tot_colors = 0;
   pal_index = 0;

   heap = 0;
   dl_image = 0;

    for ( k = 0; k < 256; k++ ) {
      r_offset[ k ] = ( k & 128 ) << 7 | ( k & 64 ) << 5 | ( k & 32 ) << 3 |
              ( k & 16 ) << 1 | ( k & 8 ) >> 1;
      g_offset[ k ] = ( k & 128 ) << 6 | ( k & 64 ) << 4 | ( k & 32 ) << 2 |
              ( k & 16 ) << 0 | ( k & 8 ) >> 2;
      b_offset[ k ] = ( k & 128 ) << 5 | ( k & 64 ) << 3 | ( k & 32 ) << 1 |
              ( k & 16 ) >> 1 | ( k & 8 ) >> 3;
    }


   c_info.palette_index = 0;
   c_info.red = 0;
   c_info.green = 0;
   c_info.blue = 0;
    c_info.distance = 0;

    for ( k = ( -255 ); k <= 255; k++ )
      c_info.squares[ k+255 ] = k*k;

   for ( k = 0;k<256;k++ ) {
      palette[ 0 ][ k ] = 0;
      palette[ 1 ][ k ] = 0;
      palette[ 2 ][ k ] = 0;
   }

    squares = c_info.squares + 255;
}

/* returns 1 on success, 0 on failure */
int dlq_start( void )
{
    int k;

    rgb_table[ 0 ] = ( CUBE * ) calloc( sizeof( CUBE ), 1 );
    rgb_table[ 1 ] = ( CUBE * ) calloc( sizeof( CUBE ), 8 );
    rgb_table[ 2 ] = ( CUBE * ) calloc( sizeof( CUBE ), 64 );
    rgb_table[ 3 ] = ( CUBE * ) calloc( sizeof( CUBE ), 512 );
    rgb_table[ 4 ] = ( CUBE * ) calloc( sizeof( CUBE ), 4096 );
    rgb_table[ 5 ] = ( CUBE * ) calloc( sizeof( CUBE ), 32768 );

    for ( k = 0; k <= 5; k++ )
      if ( rgb_table[ k ] == 0 )
          return( 0 );

    pal_index = 0;
    return( 1 );
}

void
dlq_finish( void )
{
   int k;

   for ( k = 0; k < 6; k++ )
   {
      if ( rgb_table[ k ] )
      {
         free( rgb_table[ k ] );
         rgb_table[ k ] = 0;
      }
   }

   if ( heap  )
   {
      free( heap );
      heap = 0;
   }

   if ( dl_image )
   {
      free( dl_image );
      dl_image = 0;
   }

   memset( &c_info, 0, sizeof( CLOSEST_INFO ) );

   tot_colors = pal_index = 0;
}

// Returns 1 on success, 0 on failure.
int build_table( uchar *image, ulong pixels )
{
   ulong k = 0;
   ulong index = 0;
   ulong cur_count = 0;
   ulong head = 0;
   ulong tail = 0;
   slong j = 0;

   heap = (FCUBE *) malloc( sizeof( FCUBE ) * 32769 );
   if ( heap == 0 )
      return( 0 );

#ifdef FAST
    dl_image = malloc( zsizeof( short ) * pixels );
    if ( dl_image == 0 )
      return( 0 );
#endif

    for ( k = 0; k < pixels; k++ ) {
#ifdef FAST
      dl_image[ k ] = index = r_offset[ image[ 0 ] ] + g_offset[ image[ 1 ] ] + b_offset[ image[ 2 ] ];
#else
      index = r_offset[ image[ 0 ] ] + g_offset[ image[ 1 ] ] + b_offset[ image[ 2 ] ];
#endif
#ifdef QUAL1
      rgb_table[ 5 ][ index ].r += image[ 0 ];
      rgb_table[ 5 ][ index ].g += image[ 1 ];
      rgb_table[ 5 ][ index ].b += image[ 2 ];
#endif
      rgb_table[ 5 ][ index ].pixel_count++;
      image += 3;
    }

    tot_colors = 0;
      for ( k = 0; k < 32768; k++ ) {
      cur_count = rgb_table[ 5 ][ k ].pixel_count;
      if ( cur_count ) {
         heap[ ++tot_colors ].level = 5;
         heap[ tot_colors ].index = ( ushort )k;
         rgb_table[ 5 ][ k ].pixels_in_cube = cur_count;
#ifndef QUAL1
         rgb_table[ 5 ][ k ].r = cur_count * ( ( ( k & 0x4000 ) >> 7 |
               ( k & 0x0800 ) >> 5 | ( k & 0x0100 ) >> 3 |
               ( k & 0x0020 ) >> 1 | ( k & 0x0004 ) << 1 ) + 4 );
         rgb_table[ 5 ][ k ].g = cur_count * ( ( ( k & 0x2000 ) >> 6 |
               ( k & 0x0400 ) >> 4 | ( k & 0x0080 ) >> 2 |
               ( k & 0x0010 ) >> 0 | ( k & 0x0002 ) << 2 ) + 4 );
         rgb_table[ 5 ][ k ].b = cur_count * ( ( ( k & 0x1000 ) >> 5 |
               ( k & 0x0200 ) >> 3 | ( k & 0x0040 ) >> 1 |
               ( k & 0x0008 ) << 1 | ( k & 0x0001 ) << 3 ) + 4 );
#endif
         head = k;
         for ( j = 4; j >= 0; j-- ) {
            tail = head & 0x7;
            head >>= 3;
            rgb_table[ j ][ head ].pixels_in_cube += cur_count;
            rgb_table[ j ][ head ].children |= 1 << tail;
         }
      }
    }

    for ( k = tot_colors; k > 0; k-- )
      fixheap( k );

    return( 1 );
}

void fixheap( ulong id )
{
    uchar thres_level = heap[ id ].level;
    ulong thres_index = heap[ id ].index;
   ulong index = 0;
   ulong half_totc = tot_colors >> 1;
   ulong thres_val = rgb_table[ thres_level ][ thres_index ].pixels_in_cube;

    while ( id <= half_totc ) {
      index = id << 1;

      if ( index < (ulong) tot_colors )
         if ( rgb_table[ heap[ index ].level ][ heap[ index ].index ].pixels_in_cube
           > rgb_table[ heap[ index+1 ].level ][ heap[ index+1 ].index ].pixels_in_cube )
         index++;

      if ( thres_val <= rgb_table[ heap[ index ].level ][ heap[ index ].index ].pixels_in_cube )
         break;
      else {
         heap[ id ] = heap[ index ];
         id = index;
      }
    }
    heap[ id ].level = thres_level;
    heap[ id ].index = ( ushort )thres_index;
}

void reduce_table( int num_colors )
{
   while ( tot_colors > num_colors ) {

      uchar tmp_level = heap[ 1 ].level, t_level = max( 0,tmp_level - 1 );
      ulong tmp_index = heap[ 1 ].index, t_index = tmp_index >> 3;

      if ( rgb_table[ t_level ][ t_index ].pixel_count )
         heap[ 1 ] = heap[ tot_colors-- ];
      else {
         heap[ 1 ].level = t_level;
         heap[ 1 ].index = ( ushort )t_index;
      }

      rgb_table[ t_level ][ t_index ].pixel_count += rgb_table[ tmp_level ][ tmp_index ].pixel_count;
      rgb_table[ t_level ][ t_index ].r += rgb_table[ tmp_level ][ tmp_index ].r;
      rgb_table[ t_level ][ t_index ].g += rgb_table[ tmp_level ][ tmp_index ].g;
      rgb_table[ t_level ][ t_index ].b += rgb_table[ tmp_level ][ tmp_index ].b;
      rgb_table[ t_level ][ t_index ].children &= ~( 1 << ( tmp_index & 0x7 ) );

      fixheap( 1 );
    }
}

void set_palette( int index, int level )
{
    int k;

    if ( rgb_table[ level ][ index ].children ) {
      for ( k = 7; k >= 0; k-- ) {
         if ( rgb_table[ level ][ index ].children & ( 1 << k ) ) {
            set_palette( ( index << 3 ) + k, level + 1 );
         }
      }
   }

    if ( rgb_table[ level ][ index ].pixel_count ) {
      ulong r_sum, g_sum, b_sum, sum;

      rgb_table[ level ][ index ].palette_index = pal_index;

      r_sum = rgb_table[ level ][ index ].r;
      g_sum = rgb_table[ level ][ index ].g;
      b_sum = rgb_table[ level ][ index ].b;

      sum = rgb_table[ level ][ index ].pixel_count;

      palette[ 0 ][ pal_index ] = ( BYTE )( ( r_sum + ( sum >> 1 ) ) / sum );
      palette[ 1 ][ pal_index ] = ( BYTE )( ( g_sum + ( sum >> 1 ) ) / sum );
      palette[ 2 ][ pal_index ] = ( BYTE )( ( b_sum + ( sum >> 1 ) ) / sum );

      pal_index++;
    }
}

void closest_color( int index, int level )
{
    int k;

    if ( rgb_table[ level ][ index ].children ) {
      for ( k = 7; k >= 0; k-- ) {
         if ( rgb_table[ level ][ index ].children & ( 1 << k ) ) {
            closest_color( ( index << 3 ) + k, level + 1 );
         }
      }
   }

    if ( rgb_table[ level ][ index ].pixel_count ) {
      slong dist, r_dist, g_dist, b_dist;
      uchar pal_num = rgb_table[ level ][ index ].palette_index;

      // Determine if this color is "closest".
      r_dist = palette[ 0 ][ pal_num ] - c_info.red;
      g_dist = palette[ 1 ][ pal_num ] - c_info.green;
      b_dist = palette[ 2 ][ pal_num ] - c_info.blue;

      dist = squares[ r_dist ] + squares[ g_dist ] + squares[ b_dist ];

      if ( dist < ( slong )c_info.distance ) {
         c_info.distance = dist;
         c_info.palette_index = pal_num;
      }
    }
}

// Returns 1 on success, 0 on failure.
int
quantize_image( uchar *in, uchar *out, int width, int height, int dither )
{
   if ( !dither ) {
      ulong k = 0;
      ulong pixels = width * height;
      ushort level = 0;
      ushort index = 0;
      uchar tmp_r = 0;
      uchar tmp_g = 0;
      uchar tmp_b = 0;
      uchar cube = 0;
      uchar *lookup = 0;

      lookup = (BYTE *) malloc( sizeof( char ) * 32768 );
      if ( lookup == 0 )
         return( 0 );

      for ( k = 0; k < 32768; k++ )
         if ( rgb_table[ 5 ][ k ].pixel_count )
         {
         tmp_r = ( BYTE )( ( k & 0x4000 ) >> 7 | ( k & 0x0800 ) >> 5 |
            ( k & 0x0100 ) >> 3 | ( k & 0x0020 ) >> 1 |
            ( k & 0x0004 ) << 1 );
         tmp_g = ( BYTE )( ( k & 0x2000 ) >> 6 | ( k & 0x0400 ) >> 4 |
            ( k & 0x0080 ) >> 2 | ( k & 0x0010 ) >> 0 |
            ( k & 0x0002 ) << 2 );
         tmp_b = ( BYTE )( ( k & 0x1000 ) >> 5 | ( k & 0x0200 ) >> 3 |
            ( k & 0x0040 ) >> 1 | ( k & 0x0008 ) << 1 |
            ( k & 0x0001 ) << 3 );
   #ifdef QUAL2
         lookup[ k ] = bestcolor( tmp_r, tmp_g, tmp_b );
   #else
         c_info.red = tmp_r + 4;
         c_info.green = tmp_g + 4;
         c_info.blue = tmp_b + 4;
         level = 0;
         index = 0;
         for ( ;; ) {
            cube = ( tmp_r&128 ) >> 5 | ( tmp_g&128 ) >> 6 | ( tmp_b&128 ) >> 7;
            if ( ( rgb_table[ level ][ index ].children & ( 1 << cube ) ) == 0 )
            {
            c_info.distance = ( ULONG )~0L;
            closest_color( index, level );
            lookup[ k ] = c_info.palette_index;
            break;
            }
            level++;
            index = ( index << 3 ) + cube;
            tmp_r <<= 1;
            tmp_g <<= 1;
            tmp_b <<= 1;
         }
   #endif
         }

      for ( k = 0; k < pixels; k++ ) {
   #ifdef FAST
         out[ k ] = lookup[ dl_image[ k ] ];
   #else
         out[ k ] = lookup[ r_offset[ in[ 0 ] ] + g_offset[ in[ 1 ] ] + b_offset[ in[ 2 ] ] ];
         in += 3;
   #endif
      }

      free( lookup );

   }
   else
   { // dither

   #if defined( DITHER2 ) || defined( DITHER4 )
      slong k = 0;
      slong j = 0;
      slong r_pix = 0;
      slong g_pix = 0;
      slong b_pix = 0;
      slong offset = 0;
      slong dir = 0;
      slong two_val = 0;
      slong odd_scanline = 0;
      slong err_len = ( width + 2 ) * 3;

      uchar *range_tbl = 0;
      uchar *range = 0;

      sshort *lookup = 0;
      sshort *erowerr = 0;
      sshort *orowerr = 0;
      sshort *thisrowerr = 0;
      sshort *nextrowerr = 0;

      schar *dith_max_tbl = 0;
      schar *dith_max = 0;

      lookup = (sshort *) malloc( sizeof( short ) * 32768 );
      erowerr = (sshort *) malloc( sizeof( short ) * err_len );
      orowerr = (sshort *) malloc( sizeof( short ) * err_len );
      range_tbl = (BYTE *) malloc( 3 * 256 );
      range = range_tbl + 256;
      dith_max_tbl = (schar *) malloc( 512 );
      dith_max = dith_max_tbl + 256;

      if ( range_tbl == 0 || lookup == 0 ||
           erowerr == 0 || orowerr == 0 || dith_max_tbl == 0 )
      {
         if ( range_tbl )
         {
            free( range_tbl );
            range_tbl = 0;
         }

         if ( lookup )
         {
            free( lookup );
            lookup = 0;
         }

         if ( erowerr )
         {
            free( erowerr );
            erowerr = 0;
         }

         if ( orowerr )
         {
            free( orowerr );
            orowerr = 0;
         }

         if ( dith_max_tbl )
         {
            free( dith_max_tbl );
            dith_max_tbl = 0;
         }

         return( 0 );
      }

      for ( k = 0; k < err_len; k++ )
         erowerr[ k ] = 0;

      for ( k = 0; k < 32768; k++ )
         lookup[ k ] = -1;

      for ( k = 0; k < 256; k++ )
      {
         range_tbl[ k ] = 0;
         range_tbl[ k + 256 ] = (uchar) k;
         range_tbl[ k + 512 ] = 255;
      }

      for ( k = 0; k < 256; k++ )
      {
         dith_max_tbl[ k ] = -DITHER_MAX;
         dith_max_tbl[ k + 256 ] = DITHER_MAX;
      }

      for ( k = -DITHER_MAX; k <= DITHER_MAX; k++ )
         dith_max_tbl[ k + 256 ] = (schar) k;

      for ( k = 0 ; k < height; k++ )
      {
         if ( odd_scanline )
         {
            dir = -1;
            in  += ( width - 1 ) * 3;
            out += ( width - 1 );
            thisrowerr = orowerr + 3;
            nextrowerr = erowerr + width * 3;
         }
         else
         {
            dir = 1;
            thisrowerr = erowerr + 3;
            nextrowerr = orowerr + width * 3;
         }

         nextrowerr[ 0 ] = nextrowerr[ 1 ] = nextrowerr[ 2 ] = 0;

         for ( j = 0; j < width; j++ ) {

      #ifdef DITHER2
            r_pix = range[ ( thisrowerr[ 0 ] >> 1 ) + in[ 0 ] ];
            g_pix = range[ ( thisrowerr[ 1 ] >> 1 ) + in[ 1 ] ];
            b_pix = range[ ( thisrowerr[ 2 ] >> 1 ) + in[ 2 ] ];
      #else
            r_pix = range[ ( ( thisrowerr[ 0 ] + 8 ) >> 4 ) + in[ 0 ] ];
            g_pix = range[ ( ( thisrowerr[ 1 ] + 8 ) >> 4 ) + in[ 1 ] ];
            b_pix = range[ ( ( thisrowerr[ 2 ] + 8 ) >> 4 ) + in[ 2 ] ];
      #endif
            offset = ( r_pix&248 ) << 7 | ( g_pix&248 ) << 2 | b_pix >> 3;
            if ( lookup[ offset ] < 0 )
               lookup[ offset ] = bestcolor( r_pix, g_pix, b_pix );
            *out = ( BYTE )lookup[ offset ];
            r_pix = dith_max[ r_pix - palette[ 0 ][ lookup[ offset ] ] ];
            g_pix = dith_max[ g_pix - palette[ 1 ][ lookup[ offset ] ] ];
            b_pix = dith_max[ b_pix - palette[ 2 ][ lookup[ offset ] ] ];

      #ifdef DITHER2
            nextrowerr[ 0 ] = ( short )r_pix;
            thisrowerr[ 0+3 ] += ( short )r_pix;
            nextrowerr[ 1 ] = ( short )g_pix;
            thisrowerr[ 1+3 ] += ( short )g_pix;
            nextrowerr[ 2 ] = ( short )b_pix;
            thisrowerr[ 2+3 ] += ( short )b_pix;
      #else
            two_val = r_pix * 2;
            nextrowerr[ 0-3 ] = r_pix;
            r_pix += two_val;
            nextrowerr[ 0+3 ] += r_pix;
            r_pix += two_val;
            nextrowerr[ 0 ] += r_pix;
            r_pix += two_val;
            thisrowerr[ 0+3 ] += r_pix;
            two_val = g_pix * 2;
            nextrowerr[ 1-3 ] = g_pix;
            g_pix += two_val;
            nextrowerr[ 1+3 ] += g_pix;
            g_pix += two_val;
            nextrowerr[ 1 ] += g_pix;
            g_pix += two_val;
            thisrowerr[ 1+3 ] += g_pix;
            two_val = b_pix * 2;
            nextrowerr[ 2-3 ] = b_pix;
            b_pix += two_val;
            nextrowerr[ 2+3 ] += b_pix;
            b_pix += two_val;
            nextrowerr[ 2 ] += b_pix;
            b_pix += two_val;
            thisrowerr[ 2+3 ] += b_pix;
      #endif
            thisrowerr += 3;
            nextrowerr -= 3;
            in  += dir * 3;
            out += dir;
         }

         if ( ( k% 2) == 1 )
         {
            in  += (width + 1) * 3;
            out += (width + 1);
         }

         odd_scanline = !odd_scanline;
      }

      free( range_tbl );
      free( lookup );
      free( erowerr );
      free( orowerr );
      free( dith_max_tbl );
   #else
      slong k = 0;
      slong j = 0;
      slong r_pix = 0;
      slong g_pix = 0;
      slong b_pix = 0;
      slong r_err = 0;
      slong g_err = 0;
      slong b_err = 0;
      slong offset = 0;

      uchar *range_tbl = (BYTE *) malloc( 3 * 256 ), *range = range_tbl + 256;
      sshort *lookup = (sshort *) malloc( zsizeof( short ) * 32768 );

      if ( range_tbl == 0 || lookup == 0 )
      {
         if ( range_tbl )
            free( range_tbl );

         if ( lookup )
            free( lookup );

         return( 0 );
      }

      for ( k = 0; k < 32768; k++ )
         lookup[ k ] = -1;

      for ( k = 0; k < 256; k++ )
      {
         range_tbl[ k ] = 0;
         range_tbl[ k + 256 ] = (uchar) k;
         range_tbl[ k + 512 ] = 255;
      }

      for ( k = 0; k < height; k++ )
      {
         r_err = g_err = b_err = 0;
         for ( j = width - 1; j >= 0; j-- )
         {
            r_pix = range[ (r_err >> 1) + in[ 0 ] ];
            g_pix = range[ (g_err >> 1) + in[ 1 ] ];
            b_pix = range[ (b_err >> 1) + in[ 2 ] ];

            offset = ( r_pix&248 ) << 7 | ( g_pix&248 ) << 2 | b_pix >> 3;

            if ( lookup[ offset ] < 0 )
               lookup[ offset ] = bestcolor( r_pix, g_pix, b_pix );

            *out++= ( BYTE )lookup[ offset ];

            r_err = r_pix - palette[ 0 ][ lookup[ offset ] ];
            g_err = g_pix - palette[ 1 ][ lookup[ offset ] ];
            b_err = b_pix - palette[ 2 ][ lookup[ offset ] ];

            in += 3;
         }
      }

      free( range_tbl );
      free( lookup );
   #endif
    }

    return( 1 );
}

int bestcolor( int r, int g, int b )
{
   ulong k = 0;
   ulong bestcolor = 0;
   ulong curdist = 0;
   ulong mindist = 0;
   slong rdist = 0;
   slong gdist = 0;
   slong bdist = 0;

   r = ( r & 248 ) + 4;
   g = ( g & 248 ) + 4;
   b = ( b & 248 ) + 4;
   mindist = 200000;

   for ( k = 0; k < (ulong) tot_colors; k++ ) {
      rdist = palette[ 0 ][ k ] - r;
      gdist = palette[ 1 ][ k ] - g;
      bdist = palette[ 2 ][ k ] - b;
      curdist = squares[ rdist ] + squares[ gdist ] + squares[ bdist ];

      if ( curdist < mindist )
      {
         mindist = curdist;
         bestcolor = k;
      }
   }

   return bestcolor;
}

////////////////////////////////////////////////////////////////////////////
// jpegView.cpp : implementation of the CJPegAppView class
//
//
// Note  : GIF code removed 9/23/97 pending Unisys licensing.
//
//
// This code copyright 1997 Chris Losinger, unless otherwise noted
//
// CHRISDL@PAGESZ.NET
//
// PLEASE!!! Tell me of any bugs you find!!!
//
// This code contains examples of using my ZJpegFile class, how to
// read and write 1,4,8 and 24-bit BMPs
//
// I find that this code works well for my purposes. Feel free to
// use it in your own code, but I can't assume any responsibility
// if this code fails to do what you expect.
//
// If you find any problems with this code, feel free to contact
// me for help.
//
// 24-bit to 8-bit color quantization code modified from Dennis Lee's
// DL1Quant. His source is available at ...
//

/////////////////////////////////////////////////////////////////////////////
// ZBmpFile message handlers

HBITMAP
ZBmpFile::LoadAnImageX( zPCHAR pchFileName )
{
   CString csFileName = pchFileName;
   CString ext4 = csFileName.Right( 4 );

   if ( ext4.CompareNoCase( ".BMP" ) == 0 )
      LoadBMP( csFileName );
   else
   {
      CString ext5 = csFileName.Right( 5 );
      if ( ext4.CompareNoCase( ".JPG" ) == 0 ||
           ext5.CompareNoCase( ".JPEG" ) == 0 )
      {
         LoadJPG( csFileName );
      }
   }

   return( (HBITMAP) m_pBuf );
}

void
ZBmpFile::OnFileOpen( CWnd *pWnd )
{
   CString csFileName;
   CString filt = "JPG File (*.JPG)|*.JPG|BMP (*.BMP)|*.BMP|All files (*.*)|*.*||";

   // OPENFILENAME - so i can get to its Help page easily
   CFileDialog fileDlg( TRUE, "*.JPG", "*.JPG", 0, filt, pWnd );

   fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
   fileDlg.m_ofn.lpstrTitle = "File to load";

   if ( fileDlg.DoModal( ) == IDOK )
   {
      AfxGetApp( )->DoWaitCursor( 1 );

      csFileName = fileDlg.GetPathName( );
      CString ext4 = csFileName.Right( 4 );

      if ( ext4.CompareNoCase( ".BMP" ) == 0 )
         LoadBMP( csFileName );
      else
      {
         CString ext5 = csFileName.Right( 5 );
         if ( ext4.CompareNoCase( ".JPG" ) == 0 ||
              ext5.CompareNoCase( ".JPEG" ) == 0 )
         {
            LoadJPG( csFileName );
         }
      }

      AfxGetApp( )->DoWaitCursor( -1 );
   }

   // force a redraw
   pWnd->Invalidate( TRUE );
}

void
ZBmpFile::OnFileSaveAs( CWnd *pWnd )
{
   CString csFileName;
   CString filt = "JPG File (*.JPG)|*.JPG|BMP (*.BMP)|*.BMP|All files (*.*)|*.*||";

    // OPENFILENAME - so i can get to its Help page easily
   CFileDialog fileDlg( FALSE, "*.JPG", "*.JPG", 0, filt, pWnd );

   fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
   fileDlg.m_ofn.lpstrTitle = "File to save as";

   if ( fileDlg.DoModal( ) == IDOK )
   {

      AfxGetApp( )->DoWaitCursor( 1 );

      csFileName = fileDlg.GetPathName( );
      CString ext4 = csFileName.Right( 4 );

      if ( ext4.CompareNoCase( ".BMP" ) == 0 )
         SaveBMP24( pWnd, csFileName );
      else
      {
         CString ext5 = csFileName.Right( 5 );
         if ( ext4.CompareNoCase( ".JPG" ) == 0 ||
              ext5.CompareNoCase( ".JPEG" ) == 0 )
         {
            SaveJPG( pWnd, csFileName, TRUE );
         }
      }

      AfxGetApp( )->DoWaitCursor( -1 );
   }
}

void
ZBmpFile::OnFileSavecolormappedbmp( CWnd *pWnd )
{

   if ( m_pBuf == 0 )
   {
      AfxMessageBox( "No Image!" );
      return;
   }

   ////////////////////////////////////////////////////////////////////////
   // get the file name
   CString csFileName;
   CString filt = "BMP (*.BMP)|*.BMP|All files (*.*)|*.*||";

    // OPENFILENAME - so I can get to its Help page easily
   CFileDialog fileDlg( FALSE, "*.BMP", "*.BMP", 0, filt, pWnd );
   fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
   fileDlg.m_ofn.lpstrTitle = "File to save as";

   if ( fileDlg.DoModal( ) != IDOK )
      return;

   csFileName = fileDlg.GetPathName( );

   ////////////////////////////////////////////////////////////////////////
   // fetch bits per pixel
// CBMPDlg theDlg;
// if ( theDlg.DoModal( )!= IDOK )
//    return;
// int bitsperpixel = theDlg.m_bits;
   int bitsperpixel = 8;

   AfxGetApp( )->DoWaitCursor( 1 );

   ////////////////////////////////////////////////////////////////////////
   // prepare for color-mapping

   // our palette
   RGBQUAD colormap[ 256 ];

   // num colors
   int colors = (int) pow( (double) 2, bitsperpixel );

   BYTE *colorMappedBuffer = 0;

   // if we can use the color quantizer, we will
   if ( bitsperpixel == 8 )
   {
   // CQuantDlg theDlg;
   // if ( theDlg.DoModal( )!= IDOK )
   // {
   //    return;
   // }

      // color or grayscale?
      if ( m_color )
      {
         // color !

         // allocate a buffer to colormap to
         colorMappedBuffer = (BYTE *)  new BYTE[ m_width * m_height ];

         if ( colorMappedBuffer == 0 )
         {
            AfxMessageBox( "Memory Error in OnSaveColormappedbmp!" );
            return;
         }

         BYTE tmpPal[ 3 ][ 256 ];

         // colormap it
         // generates an 8-bit color-mapped image into colorMappedBuffer
         if ( !dl1quant( m_pBuf,
                         colorMappedBuffer,   // buffers
                         m_width,
                         m_height,
                         m_quantColors,
                         TRUE,
                         tmpPal ) )
         {         // palette
            AfxMessageBox( "Quantization error" );
            delete [] colorMappedBuffer;
            return;
         }

         // copy our palette
         for ( UINT col = 0; col < 256; col++ )
         {
            if ( col > m_quantColors )
            {
               colormap[ col ].rgbRed = 0;
               colormap[ col ].rgbBlue = 0;
               colormap[ col ].rgbGreen = 0;
            }
            else
            {
               colormap[ col ].rgbRed = tmpPal[ 0 ][ col ];
               colormap[ col ].rgbGreen = tmpPal[ 1 ][ col ];
               colormap[ col ].rgbBlue = tmpPal[ 2 ][ col ];
            }
         }

      }
      else
      {
         // gray :(
         // convert to 8-bit colormapped grayscale
         colorMappedBuffer = MakeColormappedGrayscale( m_pBuf,             // RGB
                                                       (UINT) m_width,     // pixels
                                                       (UINT) m_height,
                                                       (UINT) m_width * 3, // bytes
                                                       (UINT) colors,      // colors
                                                       colormap );         // palette
      }
   }
   else
   {
      // bitsperpixel != 8
      // based on bitsperpixel, create a colormapped image
      colorMappedBuffer = MakeColormappedGrayscale( m_pBuf,
                                                    (UINT) m_width,
                                                    (UINT) m_height,
                                                    (UINT) m_width * 3,
                                                    (UINT) colors,
                                                    colormap );
   }

   ////////////////////////////////////////////////////////////////////////
   // finally, save the thing
   if ( colorMappedBuffer )
   {
      // write the BMP using our colormapped image (one byte per pixel,
      // packed), number of bits, number of total colors and a colormap
      // pixel values must be in the range [0...colors - 1]

      ZBmpFile theBmpFile;

      theBmpFile.SaveBMP( csFileName,             // path
                          colorMappedBuffer,         // image
                          m_width,             // pixels
                          m_height,
                          bitsperpixel,           // 1,4,8
                          colors,                 // num colors
                          colormap );             // palette

      if ( theBmpFile.m_errorText!= "OK" )
      {
         AfxMessageBox( theBmpFile.m_errorText, MB_ICONSTOP );
      }
      else
      {
         // load what we just saved
         LoadBMP( csFileName );
         pWnd->Invalidate( TRUE );
      }

      // toss our buffer...
      delete [] colorMappedBuffer;
   }
   else
   {
      AfxMessageBox( "Failed to allocate space for RGB buffer" );
   }

   AfxGetApp( )->DoWaitCursor( -1 );
}

void
ZBmpFile::OnFileSavegrayas( CWnd *pWnd )
{
   // note, because i'm lazy, most image data in this app
   // is handled as 24-bit images. this makes the DIB
   // conversion easier.  1, 4, 8, 15/16 and 32 bit DIBs are
   // significantly more difficult to handle.

   CString csFileName;
   CString filt = "JPG File (*.JPG)|*.JPG|All files (*.*)|*.*||";

    // OPENFILENAME - so i can get to its Help page easily
   CFileDialog fileDlg( FALSE, "*.JPG", "*.JPG", 0, filt, pWnd );
   fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
   fileDlg.m_ofn.lpstrTitle = "File to save as grayscale";

   if ( fileDlg.DoModal( ) == IDOK )
   {

      csFileName = fileDlg.GetPathName( );

      AfxGetApp( )->DoWaitCursor( 1 );

      CString ext4;
      CString ext5;
      ext4 = csFileName.Right( 4 );
      ext5 = csFileName.Right( 5 );

      if ( ext4.CompareNoCase( ".JPG" ) == 0 ||
           ext5.CompareNoCase( ".JPEG" ) == 0 )
      {
         SaveJPG( pWnd, csFileName, FALSE );
      }

      AfxGetApp( )->DoWaitCursor( -1 );
   }
}

void
ZBmpFile::DrawBMP( CWnd *pWnd, BOOL bFit )
{
   // if we don't have an image, get out of here
   if ( m_pBuf == 0 )
      return;

// TraceLineS( "ZBmpFile::DrawBMP", "==========================" );
   CPaintDC dc( pWnd );

   CRect clientRect;
   pWnd->GetClientRect( clientRect );

   ZJpegFile::DrawIt( dc.m_hDC, clientRect, m_pBuf, m_width, m_height, bFit );

#if 0
   CString info;
   info.Format( "(%d x %d)", m_width, m_height );
   dc.SetBkMode( TRANSPARENT );
   dc.SetTextColor( RGB( 0, 0, 0 ) );
   dc.TextOut( 10, 5, info );
#endif
}

////////////////////////////////////////////////////////////////////////////
// read a JPG to our global buffer
void
ZBmpFile::LoadJPG( CString csFileName )
{
   // m_pBuf is the global buffer
   mDeleteInitA( m_pBuf );
   m_pBuf = LoadBitmapFromJPG( csFileName, &m_width, &m_height );

#if 0
   // read to buffer tmp
   m_pBuf = ZJpegFile::JpegFileToRGB( csFileName, &m_width, &m_height );

   //////////////////////
   // set up for display

   // do this before DWORD-alignment!!!
   // this works on packed (not DWORD-aligned) buffers
   // swap red and blue for display
   ZJpegFile::BGRFromRGB( m_pBuf, m_width, m_height );

   // vertical flip for display
   ZJpegFile::VertFlipBuf( m_pBuf, m_width * 3, m_height );
#endif
}

////////////////////////////////////////////////////////////////////////////
// read a BMP to our global buffer
void
ZBmpFile::LoadBMP( CString csFileName )
{
   mDeleteInitA( m_pBuf );

   ZBmpFile theBmpFile;

   m_pBuf = theBmpFile.LoadBMP( csFileName, &m_width, &m_height );

   if ( (m_pBuf == 0) || (theBmpFile.m_errorText != "OK") )
   {
      AfxMessageBox( theBmpFile.m_errorText );
      m_pBuf = 0;
      return;
   }

   //////////////////////
   // set up for display

   // do this before DWORD-alignment!!!
   // this works on packed (not DWORD-aligned) buffers
   // swap red and blue for display
   ZJpegFile::BGRFromRGB( m_pBuf, m_width, m_height );

   // vertical flip for display
   ZJpegFile::VertFlipBuf( m_pBuf, m_width * 3, m_height );
}

////////////////////////////////////////////////////////////////////////////
// save a JPG

void
ZBmpFile::SaveJPG( CWnd *pWnd, CString csFileName, BOOL bColor )
{
   // note, because i'm lazy, most image data in this app
   // is handled as 24-bit images. this makes the DIB
   // conversion easier.  1, 4, 8, 15/16 and 32 bit DIBs are
   // significantly more difficult to handle.

   if ( m_pBuf == 0 )
   {
      AfxMessageBox( "No Image!" );
      return;
   }

   // we vertical flip for display. undo that.
   ZJpegFile::VertFlipBuf( m_pBuf, m_width * 3, m_height );

   // we swap red and blue for display, undo that.
   ZJpegFile::BGRFromRGB( m_pBuf, m_width, m_height );

   // save RGB packed buffer to JPG
   BOOL bOK = ZJpegFile::RGBToJpegFile( csFileName, m_pBuf,
                                        m_width, m_height,
                                        bColor, 75 );   // quality value 1-100.
   if ( bOK == FALSE )
   {
      AfxMessageBox( "Write Error" );
   }
   else
   {
      // load what we just saved
      LoadJPG( csFileName );
      pWnd->Invalidate( TRUE );
   }
}

////////////////////////////////////////////////////////////////////////////
//
// use the ZBmpFile class to write a 24-bit BMP file
//
void
ZBmpFile::SaveBMP24( CWnd *pWnd, CString csFileName )
{
   // Note, because i'm lazy, most image data in this app
   // is handled as 24-bit images. this makes the DIB
   // conversion easier. 1,4,8, 15/16 and 32 bit DIBs are
   // significantly more difficult to handle.

   if ( m_pBuf == 0 )
   {
      AfxMessageBox( "No Image!" );
      return;
   }

   // Image in m_pBuf is already BGR and vertically flipped, so we don't need
   // to do that for this function.

   // I really should make an RGB ==> BMP function.

   ZBmpFile theBmpFile;
   theBmpFile.SaveBMP( csFileName, m_pBuf, m_width, m_height );

   if ( theBmpFile.m_errorText != "OK" )
      AfxMessageBox( theBmpFile.m_errorText, MB_ICONSTOP );
   else
   {
      LoadBMP( csFileName );   // load what we just saved
      pWnd->Invalidate( TRUE );
   }
}

// Get JPG dimensions
void
ZBmpFile::OnFileGetdimensionsJPG( CWnd *pWnd )
{
   CString csFileName;
   CString filt = "JPG File (*.JPG)|*.JPG|All files (*.*)|*.*||";

   // OPENFILENAME - so i can get to its Help page easily
   CFileDialog fileDlg( TRUE, "*.JPG", "*.JPG", 0, filt, pWnd );

   fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
   fileDlg.m_ofn.lpstrTitle = "File to examine";

   if ( fileDlg.DoModal( ) == IDOK )
   {
      csFileName = fileDlg.GetPathName( );

      UINT width, height;
      if ( ZJpegFile::GetJPGDimensions( csFileName, &width, &height ) )
      {
         zCHAR buf[ 200 ];
         sprintf( buf, "%d %d", width, height );
         AfxMessageBox( buf );
      }
      else
         AfxMessageBox( "JPEG Error" );
   }
}

////////////////////////////////////////////////////////////////////////
// Instead of creating a good palette for the colormapped images
// this just graymaps them.
//
BYTE *
ZBmpFile::MakeColormappedGrayscale( BYTE *inBuf,
                                    UINT inWidth,
                                    UINT inHeight,
                                    UINT inWidthBytes,
                                    UINT colors,
                                    RGBQUAD *colormap )
{
   ////////////////////////////////////////////////////////////////////////
   // Allocate a buffer to colormap
   BYTE *tmp = (BYTE *) new BYTE[ inWidth * inHeight ];
   if ( tmp == 0 )
      return( 0 );

   // force our image to use a stupid gray scale
   UINT uColor;
   for ( uColor = 0; uColor < colors; uColor++ )
   {
      colormap[ uColor ].rgbRed = uColor * 256 / colors;
      colormap[ uColor ].rgbGreen = uColor * 256 / colors;
      colormap[ uColor ].rgbBlue = uColor * 256 / colors;
   }


   UINT col, row;
   for ( row = 0; row < inHeight; row++ )
   {
      for ( col = 0;col <inWidth; col++ )
      {
         BYTE inRed, inBlue, inGreen;

         // src pixel
         long in_offset = row * inWidthBytes + col * 3;
         inRed = *(inBuf + in_offset + 0);
         inGreen = *(inBuf + in_offset + 1);
         inBlue = *(inBuf + in_offset + 2);

         // luminance
         int lum = (int) (.299 * (double) (inRed) +
                          .587 * (double) (inGreen) +
                          .114 * (double) (inBlue));

         // Force luminance value into our range of colors.
         lum = colors * lum / 256;

         // dest pixel
         long out_offset = row * inWidth + col;
         *(tmp + out_offset) = (BYTE) lum;
      }
   }

   return( tmp );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
BmpCtl( ZSubtask *pZSubtask,
        CWnd     *pWndParent,
        ZMapAct  *pzmaComposite,
        zVIEW    vDialog,
        zSHORT   nOffsetX,
        zSHORT   nOffsetY,
        zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZBmpCtl( pZSubtask, pWndParent,
                        pzmaComposite, vDialog,
                        nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=BMP_SetBitmapFileName
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: BMP_SetBitmapFileName
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To specify a different bitmap image to show within
//             a bitmap control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the bitmap control
//              cpcBitmapFileName - the fully qualified file name containing
//                            the bitmap to show in the bitmap control.
//
//  RETURNS:    -1  - Error locating window or bitmap control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
BMP_SetBitmapFileName( zVIEW    vSubtask,
                       zCPCHAR  cpcCtrlTag,
                       zCPCHAR  cpcBitmapFileName )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zSHORT   nRC;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
#ifdef zREMOTE_SERVER

      zCHAR  szImage[ zMAX_FILENAME_LTH ];
      zSHORT nLth = zstrlen( cpcBitmapFileName ) + zMAX_FILENAME_LTH + 1;
      zPCHAR pchName = new char[ nLth ];

      GetAppOrWorkstationValue( vSubtask, "ExecutableRemoteImage", szImage, zsizeof( szImage ) );
      strcpy_s( pchName, cpcBitmapFileName );
      if ( szImage[ 0 ] )
      {
         zPCHAR pchBMP;
         zPCHAR pch = new char[ nLth ];

         strcpy_s( pch, nLth, pchName );
         SysTranslateString( pch, 'L' );
         if ( (pchBMP = zstrstr( pch, ".bmp" )) != 0 )
         {
            strcpy_s( pchName + (pchBMP - pch + 1), szImage );
            strcat_s( pchName, pchBMP + 4 );
         }

         delete [] pch;
      }

      GetAppOrWorkstationValue( vSubtask, "ExecutableRemoteImageStrip", szImage, zsizeof( szImage ) );
      if ( szImage[ 0 ] )
      {
         zPCHAR pch = new char[ nLth ];

         strcpy_s( pch, nLth, szImage );
         SysConvertEnvironmentString( szImage, zsizeof( szImage ), pch );
         SysTranslateString( szImage, 'L' );

         strcpy_s( pch, nLth, pchName );
         SysTranslateString( pch, 'L' );

         nLth = zstrlen( szImage );
         if ( zstrncmp( pch, szImage, nLth ) == 0 )
            strcpy_s( pchName, nLth, pchName + nLth );

         zPCHAR pchColon = zstrchr( pchName, ':' );
         if ( pchColon )
            strcpy_s( pchName, nLth, pchColon + 1 );

         delete [] pch;
      }

      nRC = SetRemoteZCtrlAttribute( pzma, "Bitmap", "URL", pchName );
      delete [] pchName;
      return( nRC );
#else
      ZBmpCtl *pBmp = DYNAMIC_DOWNCAST( ZBmpCtl, pzma->m_pCtrl );
      if ( pBmp )
      {
         nRC = pBmp->SetBitmapFileName( cpcBitmapFileName );
         return( nRC );
      }
#endif

      TraceLineS( "drvr - Invalid control type for BMP_SetBitmapFileName ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=BMP_SetBitmapResource
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: BMP_SetBitmapResource
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To specify a different bitmap image to show within
//             a bitmap control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the bitmap control
//              cpcDLL_Name - The name (may be fully qualified if not in the
//                            path at execution time) containing the bitmap
//                            resource.
//              nBitmapResId - The resource number of the bitmap within
//                             the resource DLL.
//
//  RETURNS:    -1  - Error locating window or bitmap control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
BMP_SetBitmapResource( zVIEW    vSubtask,
                       zCPCHAR  cpcCtrlTag,
                       zCPCHAR  cpcDLL_Name,
                       zSHORT   nBitmapResId )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZBmpCtl *pBmp = DYNAMIC_DOWNCAST( ZBmpCtl, pzma->m_pCtrl );
      if ( pBmp )
      {
         pBmp->SetBitmapResource( cpcDLL_Name, nBitmapResId );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for BMP_SetBitmapResource ",
                  cpcCtrlTag );
   }

   return( -1 );
}
