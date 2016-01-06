/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctledt.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon EditBox control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2003.07.10    DKS
//    Only set text if it is a change from current value.
//
// 2002.06.10    BL    R55711
//    Bugfix ED_SetCtrlMapping for a Control without Mapping in PWD.
//
// 2002.04.30    HH/DKS
//    Consistency check to recognize old garbage CtrlBOI's.
//
// 2002.04.10    DKS
//    Fixed bug reported by FH when no mapping is specified for a formatted
//    string specification.
//
// 2002.03.12    DKS
//    Added keydown events.
//
// 2002.03.06    DKS
//    Added formatting (pic).
//
// 2002.02.14    DKS
//    Added Domain translation via code.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.21    DKS   Enhance
//    Added SetCtrlMapping to remap a Control.
//
// 2001.07.14    DKS    Helmut
//    Changed background color to be retained on error.
//
// 2001.06.10    BL     WEB/Windows
//    Corrected max. input length of Editfield.
//
// 2001.05.21    DKS    Z10
//    Honor text and background color settings.
//
// 2001.05.19    BL     WEB
//    Bugfix for not mapped Editfields.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2001.01.03    DKS    Z10
//    Permit select of text in EditBox/MLE.
//
// 2000.10.23    TMV    WEB
//    Fix Bug in LimitTextLength - remote attribute belongs to EditBox
//    entity rather than Ctrl.
//
// 2000.03.17    TMV    WEB
//    Validate parent window's handle prior to call Create/CreateEx.
//
// 2000.03.13    TMV    Z10
//    Edit ctrl createable without border
//
// 1999.08.04    DKS    Z9J/Z10   FH999
//    Edit ctrl now paints properly after return from screen saver.
//
// 1999.07.22    DKS    Z10    QS999
//    Permit edit ctrl to be disabled in a "ReadOnly" mode.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlEdt.h"
#include "ZdCtlTim.h"
#include "ZdCtlGbl.h"

#define zEDITBOX_GetFocus  0x00000010
#define zEDITBOX_LoseFocus 0x00000011
#define zEDITBOX_Change    0x00000012
#define zEDITBOX_Enter     0x00000014
#define zEDITBOX_KeyDown   0x00000015
#define zEDITBOX_KeyUp     0x00000016
#define zEDITBOX_Char      0x00000017
#define zEDITBOX_Click     0x00000018

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( ZEdit, CEdit )

BEGIN_MESSAGE_MAP( ZEdit, CEdit )
   //{{AFX_MSG_MAP( ZEdit )
   ON_WM_CREATE( )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_MOUSEMOVE( )
   ON_WM_SYSKEYDOWN( )
   ON_WM_SYSKEYUP( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_WM_CHAR( )
   ON_WM_LBUTTONUP( )
   ON_WM_SIZE( )
   ON_WM_WINDOWPOSCHANGED( )
   ON_CONTROL_REFLECT( EN_CHANGE, ENChange )
   ON_CONTROL_REFLECT( EN_ERRSPACE, ENErrspace )
   ON_CONTROL_REFLECT( EN_HSCROLL, ENHscroll )
   ON_CONTROL_REFLECT( EN_KILLFOCUS, ENKillFocus )
   ON_CONTROL_REFLECT( EN_MAXTEXT, ENMaxtext )
   ON_CONTROL_REFLECT( EN_SETFOCUS, ENSetFocus )
   ON_CONTROL_REFLECT( EN_UPDATE, ENUpdate )
   ON_CONTROL_REFLECT( EN_VSCROLL, ENVscroll )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

// ZEdit - ctor
ZEdit::ZEdit( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
       CEdit( ),
       ZMapAct( pZSubtask, // base class ctor
                pzmaComposite,
                pWndParent,
                this,
                vDialog,
                nOffsetX,
                nOffsetY,
                pCtrlDef,
                "EditBox" )
{
#ifdef DEBUG_ALL
   TraceLineS( "In ctor for ZEdit: ", *m_pzsTag );
   if ( m_pzsVName )
   {
      TraceLineS( "ZEdit ViewName", *m_pzsVName );
      TraceLineS( "ZEdit Entity", *m_pzsEName );
      TraceLineS( "ZEdit Attribute", *m_pzsAName );
   }
#endif
   zULONG ulBlobLth;

   m_ulSubtype = pCtrlDef->Subtype;
   m_ulLth = 0;
   m_hBrushSystem = 0;
   m_chPic = 0;

   m_bLastKeyWasDelete = FALSE;
   m_bLastKeyWasBackspace = FALSE;
   m_chPlaceHolder = ' ';
   m_bStripSpecial = FALSE;
   m_ulLastKey = 0;

   m_pchBlobInfo = 0;
   m_lFormatPicLth = 0;
   m_lSpecialCnt = 0;
   m_cpcFormatPic = 0;
   m_cpcOverride = 0;
   m_bFixedFont = FALSE;

   if ( m_ulMapActFlags & zMAPACT_USE_SUBTASK_BRUSH )
   {
      mDeleteInit( m_pBrush );
      m_clrBk = 0;
   }

   GetAttributeLength( &ulBlobLth, vDialog, szlCtrl, szlCtrlBOI );
   if ( ulBlobLth )
   {
      // Consistency check to recognize garbage CtrlBOI's based on TZPNCTAD
      // code:  ulLth = (2 * (lFormatLth + 1)) + (3 * sizeof( zLONG ));
      m_pchBlobInfo = new char[ ulBlobLth ];
      GetBlobFromAttribute( m_pchBlobInfo, &ulBlobLth, vDialog,
                            "Ctrl", "CtrlBOI" );

      m_lFormatPicLth = (zLONG) (*((zPLONG) m_pchBlobInfo));
      if ( (m_lFormatPicLth + 1) * 2 + 3 * sizeof( zLONG ) == ulBlobLth )
      {
         m_lSpecialCnt = (*((zPLONG) (m_pchBlobInfo + sizeof( zLONG ))));
         m_cpcFormatPic = m_pchBlobInfo + (3 * sizeof( zLONG ));
         m_cpcOverride = m_cpcFormatPic + m_lFormatPicLth + 1;
         if ( m_lSpecialCnt < 0 )
         {
            m_lSpecialCnt = -m_lSpecialCnt;
            m_bStripSpecial = TRUE;
         }

         if ( *(m_pchBlobInfo + (2 * sizeof( zLONG ))) )    // 1 reserved byte
            m_chPlaceHolder = *(m_pchBlobInfo + (2 * sizeof( zLONG )));

         if ( *(m_pchBlobInfo + (2 * sizeof( zLONG )) + 2) ) // 1 reserved byte
            m_bFixedFont = TRUE;
      }
      else
      {
         // We did not pass the consistency check, so we assume that we've
         // got a garbage CtrlBOI. IGNORE IT!
         mDeleteInitA( m_pchBlobInfo );
         m_lFormatPicLth = 0;
      }
   }

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style &= ~WS_BORDER;
   else
      Attr.Style |= WS_BORDER;

   Attr.Style &= ~(ES_RIGHT | ES_CENTER);
   if ( pCtrlDef->Subtype & zCONTROL_RIGHTJUSTIFY )
      Attr.Style |= ES_RIGHT;
   else
   if ( pCtrlDef->Subtype & zCONTROL_CENTERJUSTIFY )
      Attr.Style |= ES_CENTER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   Attr.Style &= ~(WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL);
   Attr.Style |= ES_AUTOVSCROLL;
   Attr.Style |= ES_AUTOHSCROLL;

   if ( pCtrlDef->Subtype & zEDITBOX_LOWERCASE )
      Attr.Style |= ES_LOWERCASE;
   else
   if ( pCtrlDef->Subtype & zEDITBOX_UPPERCASE )
      Attr.Style |= ES_UPPERCASE;
   else
   if ( pCtrlDef->Subtype & zEDITBOX_PASSWORD )
      Attr.Style |= ES_PASSWORD;

   if ( (pCtrlDef->Subtype & zEDITBOX_PASSWORD) && pCtrlDef->CtrlBOI )
      m_chPassword = *((zPCHAR) pCtrlDef->CtrlBOI);
   else
      m_chPassword = 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      m_chPic = 0;
      CreateZ( );
   }
}

void
ZEdit::CreateZ( )
{
#if 0
   if ( zstrstr( *m_pzsTag, "Inplace" ) )
   {
      TraceLineS( "ZEdit::CreateZ: ", *m_pzsTag );
      mDeleteInit( m_pzsText );
      m_pzsText = new CString( "DKS INPLACE" );
   }
#endif

   zLONG lStyleEx;

   if ( Attr.Style & WS_BORDER )
      lStyleEx = WS_EX_CLIENTEDGE;
   else
      lStyleEx = 0;

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
   SetZCtrlText( this, *m_pzsText );
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( lStyleEx, "EDIT", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CEdit::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

#if 0
   if ( zstrstr( *m_pzsTag, "Inplace" ) )
   {
      CString cs;

      GetWindowText( cs );
      TraceLine( "ZEdit::OnCreate: %s  Text: %s", *m_pzsTag, cs );
   }
#endif

   if ( m_pFont == 0 && m_cpcFormatPic && m_bFixedFont )
   {
      m_pFont = new CFont( );

#if 1
      HFONT hFont = (HFONT) GetStockObject( SYSTEM_FIXED_FONT );
      CFont *pFont = CFont::FromHandle( hFont );
      ASSERT( pFont );

      LOGFONT lf;
      pFont->GetLogFont( &lf );
   // m_pFont->CreateFontIndirect( &lf );
   // TraceLine( "Creating Courier New for: %s  Height: %d  Width: %d",
   //            *m_pzsTag, lf.lfHeight, lf.lfWidth );

      CDC *pDC = GetDC( );

      lf.lfHeight = -MulDiv( lf.lfHeight, GetDeviceCaps( pDC->GetSafeHdc( ),
                                                         LOGPIXELSY ), 72 );
      m_pFont->CreatePointFont( (int) (lf.lfHeight * 4.5), _T( "Courier New" ) );
   // m_pFont->CreatePointFont( lf.lfWidth * 10, _T( "Courier New" ) );
#else
      m_pFont->CreatePointFont( 90, _T( "Courier New" ) );
#endif
   // TraceLineS( "Creating Courier New 8.5 for: ", *m_pzsTag );
   }

   SetFontOverride( );

   // To set password style off, call SetPasswordChar with null value.
   if ( m_chPassword )
      SetPasswordChar( m_chPassword );

   zVIEW vApp = 0;

   if ( m_pzsVName && m_pzsEName && m_pzsAName ) // painter ensures all???
   {
      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( m_ulLth == 0 )
            GetAttributeDisplayLength( &m_ulLth, vApp, *m_pzsEName,
                                       *m_pzsAName, *m_pzsContext );

         // Set TextLen in the OnCreate method to limit text. It
         // holds the size of the text buffer for static (and
         // derived) controls.  Because of the null terminator
         // on the string, the number of characters that can
         // be stored in the static control is one less than m_uLth.
      }
   }

   LimitTextLength( m_lFormatPicLth ? m_lFormatPicLth + 1 : m_ulLth );

   if ( m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
      SetZCtrlState( zCONTROL_STATUS_DISABLE_READONLY, TRUE );

   return( nRC );
}

BOOL
ZEdit::DestroyWindow( )
{
#if 0
   if ( zstrstr( *m_pzsTag, "Inplace" ) )
      TraceLineS( "ZEdit::DestroyWindow: ", *m_pzsTag );
#endif

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CEdit::DestroyWindow( ) );
}

// Default processing for a ctrl's mapping from the data OI ... if the
// View and Entity and Attribute have been specified, SetWindowText using
// the value returned by GetVariableFromAttribute returning 0, otherwise
// return -1.
zSHORT
ZEdit::MapFromOI( WPARAM wFlag )
{
#if 0
   if ( zstrstr( *m_pzsTag, "Inplace" ) )
   {
      CString cs;

      GetWindowText( cs );
      TraceLine( "ZEdit::MapFromOI: %s  Text: %s", *m_pzsTag, cs );
      return( 0 );
   }
#endif
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::MapFromOI: ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zCHAR  szMap[ zMAP_SIZE ];
      zVIEW  vApp;
      zPCHAR pch;

      m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName,
                                              *m_pzsEName, *m_pzsAName );
   // TraceLineS( "ZMapAct::MapFromOI ", *m_pzsTag );
   // TraceLineX( "ZMapAct::MapFromOI vApp: ", (zLONG) vApp );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) )
      {
         if ( m_ulLth == 0 )
         {
            GetAttributeDisplayLength( &m_ulLth, vApp, *m_pzsEName,
                                       *m_pzsAName, *m_pzsContext );
            LimitTextLength( m_lFormatPicLth ?
                                         m_lFormatPicLth + 1 : m_ulLth );
         }

         if ( m_ulLth + m_lSpecialCnt > sizeof( szMap ) - 1 )
         {
            GetAddrForAttribute( (zPCHAR *) &pch, vApp,
                                 *m_pzsEName, *m_pzsAName );
         }
         else
         {
            zSHORT nRC =
            GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                      sizeof( szMap ) - 1, vApp,
                                      *m_pzsEName,
                                      *m_pzsAName,
                                      *m_pzsContext,
                                      m_pzsContext->IsEmpty( ) ?
                                                  zUSE_DEFAULT_CONTEXT : 0 );
            if ( nRC == zVAR_NULL )
               pch = 0;
            else
               pch = szMap;
         }
      }
      else
      {
         pch = 0;
      }

      if ( pch == 0 )
      {
         szMap[ 0 ] = 0;
         pch = szMap;
      }

      TranslateValue( pch, sizeof( szMap ), TRUE, 1 );
      SetZCtrlText( this, pch );
      if ( GetFocus( ) == this )
         SetSel( 0, -1 );

      m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
      return( 0 );
   }

   return( -1 );
}

void
ZEdit::CWndSetWindowText( zCPCHAR cpcText )
{
   CString cs;

   CWnd::GetWindowText( cs );
   if ( zstrcmp( cpcText, cs.GetString() ) != 0 )
      CWnd::SetWindowText( cpcText );
}

void
ZEdit::SetText( zPCHAR pchText, zLONG lMaxLth )
{
   if ( m_lFormatPicLth > 0 )  // need to format string
   {
      zLONG  k;
      zLONG  j;
      zLONG  lLth = zstrlen( pchText );
      zPCHAR pchFormatText;

      pchFormatText = new char[ zstrlen( pchText ) +
                                m_lFormatPicLth + m_lSpecialCnt + 1 ];
      for ( k = 0, j = 0; k < m_lFormatPicLth; k++ )
      {
         if ( m_cpcOverride[ k ] == 'Y' )
         {
            pchFormatText[ k ] = m_cpcFormatPic[ k ];
            if ( m_bStripSpecial == FALSE )
               j++;
         }
         else
         {
            if ( j < lLth )
               pchFormatText[ k ] = pchText[ j ];
            else
               pchFormatText[ k ] = m_chPlaceHolder;

            j++;
         }
      }

      pchFormatText[ m_lFormatPicLth ] = 0;
      strcpy_s( pchText, lMaxLth, pchFormatText );
      m_csSaveText = pchFormatText;
      delete [] pchFormatText;
   }
   else
   {
      m_csSaveText = pchText;
   }
}

zSHORT
ZEdit::SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn )
{
   zLONG lLth = zstrlen( cpcText ) + m_lFormatPicLth + m_lSpecialCnt + 1 ;
   zPCHAR pchFormatText = new char[ lLth ];
   strcpy_s( pchFormatText, lLth, cpcText );
   SetText( pchFormatText, lLth );
   CWndSetWindowText( pchFormatText );
   delete [] pchFormatText;
   return( 0 );
}

zLONG
ZEdit::GetText( zPCHAR pchText, zLONG lMaxLth ) const
{
   zBOOL bStripped = FALSE;
   if ( m_bStripSpecial && m_lFormatPicLth > 0 )  // need to format string
   {
      zLONG k;
      zLONG j;
      zLONG lLth = zstrlen( pchText );
      zPCHAR pchFormatText;

      bStripped = TRUE;
      pchFormatText = new char[ lLth + m_lFormatPicLth + m_lSpecialCnt + 1 ];
      pchFormatText[ 0 ] = 0;
      for ( k = 0, j = 0; k < m_lFormatPicLth; k++ )
      {
         if ( m_cpcOverride[ k ] != 'Y' )
         {
            if ( k < lLth )
               pchFormatText[ j ] = pchText[ k ];
            else
               pchFormatText[ j ] = 0;

            j++;
            pchFormatText[ j ] = 0;
         }
      }

      strcpy_s( pchText, lMaxLth, pchFormatText );
      delete [] pchFormatText;
   }

   if ( m_lFormatPicLth > 0 )
   {
      // Need to strip place holder characters.
      if ( m_chPlaceHolder != ' ' )
      {
         zPCHAR pch = pchText;
         while ( (pch = zstrchr( pch, m_chPlaceHolder )) != 0 )
         {
            *pch = ' ';
            pch++;
         }
      }

      zLONG  lLth = zstrlen( pchText ) - 1;
      zLONG  lData = -1;
      if ( bStripped )
      {
         while ( lLth >= 0 )
         {
            if ( pchText[ lLth ] != ' ' )
            {
                  lData = lLth + 1;
                  break;
            }

            lLth--;
         }
      }
      else
      {
         while ( lLth >= 0 )
         {
            if ( lData == -1 )
            {
               if ( lLth < m_lFormatPicLth && m_cpcOverride[ lLth ] == 'Y' )
               {
                  // do nothing
               }
               else
               if ( pchText[ lLth ] != ' ' )
               {
                  lData = lLth + 1;
                  break;
               }
            }

            lLth--;
         }
      }

      if ( lData == -1 )
         lData = 0;

      pchText[ lData ] = 0;
   }

   return( zstrlen( pchText ) );
}

zSHORT
ZEdit::GetRowColText( zPCHAR pchText, zUSHORT uMaxLth,
                      zLONG lRow, zLONG lColumn )
{
   CWnd::GetWindowText( pchText, uMaxLth );
   GetText( pchText, uMaxLth );
   return( (zSHORT) zstrlen( pchText ) );
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZEdit::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::MapToOI: ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      zSHORT  nRC;
      zCHAR   szMap[ 65534 ];

      GetZCtrlText( this, szMap, sizeof( szMap ) );
   // GetText( szMap, sizeof( szMap ) );
      TranslateValue( szMap, sizeof( szMap ), FALSE );

#if 0
      if ( zstrcmp( *m_pzsAName, "Tag" ) == 0 &&
           zstrcmp( *m_pzsEName, "Control" ) == 0 )
      {
         zLONG lAbsolutePos;

         TraceLineS( "ZEdit::MapToOI Control Tag: ", zs );
         GetAbsolutePositionForEntity( &lAbsolutePos, vApp, *m_pzsEName );
         TraceLineI( "ZEdit::MapToOI Control Tag: ", lAbsolutePos );
      }
#endif

      nRC = SetAttributeFromVariable( vApp, *m_pzsEName,
                                      *m_pzsAName, szMap,
                                      zTYPE_STRING, 0,
                                      *m_pzsContext,
                                      m_pzsContext->IsEmpty( ) ?
                             (zSHORT) (lFlag | zUSE_DEFAULT_CONTEXT) :
                             (zSHORT) lFlag );
      return( nRC );
   }

   return( 0 );
}

#if 0  // not used (yet)
void
ZEdit::SetFormatPic( zCPCHAR cpcFormatPic,
                     zCPCHAR cpcOverride, zCHAR cPlaceHolder )
{
   mDeleteInitA( m_cpcFormatPic );
   mDeleteInitA( m_cpcOverride );
   if ( *cpcFormatPic )
   {
      m_lFormatPicLth = zstrlen( m_cpcOverride );
      m_cpcOverride = new char[ m_lFormatPicLth + 1 ];
      strcpy_s( m_cpcOverride, m_lFormatPicLth + 1, cpcOverride )
      m_lFormatPicLth = zstrlen( cpcFormatPic );
      m_cpcFormatPic = new char[ m_lFormatPicLth + 1 ];
      strcpy_s( m_cpcFormatPic, m_lFormatPicLth + 1, cpcFormatPic )
      m_csSaveText.Empty( );

      zLONG lIndex = 0;
      while ( lIndex < m_lFormatPicLth )
      {
         if ( IsFormatPicChar( lIndex ) )
            m_csSaveText += m_cpcFormatPic[ lIndex ];
         else
            m_csSaveText += m_chPlaceHolder;

         lIndex++;
      }
   }

   CWndSetWindowText( m_csSaveText );
}
#endif

void
ZEdit::ENChange( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::ENChange: ", *m_pzsTag );
#endif
   if ( m_ulMapActFlag2 & zMAPACT_CREATED )
   {
      zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
      if ( (ulSubtaskState & zSUBTASK_STATE_WELL_FORMED) ==
                                                 zSUBTASK_STATE_WELL_FORMED )
      {
      // if ( (m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI) == 0 )  // can't do this!
            ProcessImmediateEvent( this, zEDITBOX_Change );
      }
   }

   Default( );             // pass it along to parent
   if ( (m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI) &&
        (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SELECT_FIRST_EDIT) )
   {
      SetSel( 0, -1 );
   }
}

zBOOL
ZEdit::HandleQuickKey( zACCEL_DEF ad )
{
   if ( ad.uVK == VK_RETURN && ad.fAlt == 0 && ad.fCtrl == 0 )
   {
      zLONG lRC = ProcessImmediateEvent( this, zEDITBOX_Enter, 0 );
      if ( lRC != zNO_APPLICATION_EVENT && (zSHORT) zLOUSHORT( lRC ) < 0 )
      {
         return( TRUE );
      }
   }

   return( FALSE );
}

zLONG
ZEdit::SelectAll( )
{
   SetSel( 0, -1 );
   return( 1 );
}

zSHORT
ZEdit::SelectText( zSHORT nTextStartPos,
                   zSHORT nTextEndPos )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::SelectText: ", *m_pzsTag );
#endif
   SetSel( nTextStartPos - 1, nTextEndPos - 1 );
   return( 0 );
}

zSHORT
ZEdit::LimitTextLength( zLONG lTextLth )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::LimitTextLength: ", *m_pzsTag );
#endif
   if ( lTextLth >= 0 )
   {
      m_ulLth = lTextLth;
      if ( mIs_hWnd( m_hWnd ) )
         SetLimitText( m_ulLth );

#ifdef zREMOTE_SERVER
      zCHAR szTextLth[ 32 ];

      _ltoa_s( lTextLth, szTextLth );
      SetRemoteZCtrlAttribute( this, "EditBox", "TextLth", szTextLth );
#endif

      return( 0 );
   }

   return( 1 );
}

zLONG
ZEdit::SetZCtrlState( zLONG  lStatusType,
                      zULONG ulValue )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "FirstName", *m_pzsTag ) == 0 )
      TraceLineS( "ZEdit::SetZCtrlState: ", *m_pzsTag );
#endif

   // Optionally an edit ctrl can be disabled using the ReadOnly state
   // setting.  Sometimes the ReadOnly state is not sufficient since
   // the ctrl can still receive focus via a tab.

   if ( (lStatusType != zCONTROL_STATUS_DISABLE_READONLY &&
         lStatusType != zCONTROL_STATUS_ENABLED &&
         lStatusType != zCONTROL_STATUS_AUTOGRAY) ||
        ((m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == FALSE &&
         (lStatusType == zCONTROL_STATUS_ENABLED ||
          lStatusType == zCONTROL_STATUS_AUTOGRAY)) )
   {
      return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
   }

   zBOOL bEnabledPrev = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;

   if ( lStatusType == zCONTROL_STATUS_ENABLED )
   {
      // Shut off auto enabled flag ... user is overriding enabled status.
      m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
      if ( ulValue )
         m_ulMapActFlags |= zMAPACT_ENABLED;
      else
         m_ulMapActFlags &= ~zMAPACT_ENABLED;
   }
   else
   if ( lStatusType == zCONTROL_STATUS_AUTOGRAY )
   {
      if ( ulValue )
      {
         // We can't auto-enable a control that was manually disabled.
         if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
         {
            m_ulMapActFlags |= zMAPACT_ENABLED;
            m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
         }
      }
      else
      {
         // We can't auto-disable a control that was manually disabled.
         if ( m_ulMapActFlags & zMAPACT_ENABLED )
         {
            m_ulMapActFlags &= ~zMAPACT_ENABLED;
            m_ulMapActFlags |= zMAPACT_AUTO_DISABLED;
         }
      }
   }
   else
   if ( lStatusType == zCONTROL_STATUS_DISABLE_READONLY )
   {
      if ( ulValue )
         m_ulMapActFlags |= zMAPACT_DISABLE_READONLY;
      else
         m_ulMapActFlags &= ~zMAPACT_DISABLE_READONLY;

      if ( mIs_hWnd( m_hWnd ) )
      {
#ifdef zREMOTE_SERVER
         if ( ulValue )
         {
            SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", "Y" );
            if ( (m_ulMapActFlags & zMAPACT_AUTO_DISABLED) ||
                 (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
            {
               SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly", "Y" );
            }
            else
            {
               SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly", "N" );
            }
         }
         else
         {
            SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly", "N" );
            if ( (m_ulMapActFlags & zMAPACT_AUTO_DISABLED) ||
                 (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
            {
               SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", "N" );
            }
            else
            {
               SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", "Y" );
            }
         }
#else
         if ( ulValue )
         {
            EnableWindow( TRUE );
            if ( (m_ulMapActFlags & zMAPACT_AUTO_DISABLED) ||
                 (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
            {
               SetReadOnly( TRUE );
            }
            else
            {
               SetReadOnly( FALSE );
            }
         }
         else
         {
            SetReadOnly( FALSE );
            if ( (m_ulMapActFlags & zMAPACT_AUTO_DISABLED) ||
                 (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
            {
               EnableWindow( FALSE );
            }
            else
            {
               EnableWindow( TRUE );
            }
         }
#endif
      }

      return( 0 );
   }

   zBOOL bEnabled = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;
   if ( (bEnabled == TRUE && bEnabledPrev == FALSE) ||
        (bEnabled == FALSE && bEnabledPrev == TRUE) )
   {
      bEnabled = !bEnabled;     // for SetReadOnly and for return value
#ifdef zREMOTE_SERVER
      SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled",
                               bEnabled ? "N" : "Y" );
      SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly",
                               bEnabled ? "Y" : "N" );
#else
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
            SetReadOnly( bEnabled );
         else
            EnableWindow( !bEnabled );

         Invalidate( TRUE );
      }
#endif
   }

   return( bEnabled );
}

HBRUSH
ZEdit::CtlColor( CDC *pDC, UINT nCtlColor )
{
#if 0 //def DEBUG_ALL  lots of messages
   if ( zstrcmp( "Tag", *m_pzsTag ) == 0 )
   {
      CString cs;
      GetWindowText( cs );
      TraceLineS( "ZEdit::CtlColor Tag ", *m_pzsTag );
   // TraceLineS( "ZEdit::CtlColor Text ", cs );
   // TraceLineI( "ZEdit::CtlColor nCtlColor ", nCtlColor );
   // TraceLineX( "ZEdit::CtlColor colorText ", (zLONG) m_clrText );
   }
#endif

   HBRUSH hBrush;

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }
   else
      hBrush = (HBRUSH) Default( );

   if ( m_ulCtrlErr )
   {
      pDC->SetTextColor( zRGB( 255, 0, 0 ) );
//    pDC->SetTextColor( m_pZSubtask->m_rgbColorErrText );
   }
   else
   {
      if ( m_clrText )
         pDC->SetTextColor( m_clrText );
   }

   return( hBrush );
}

void
ZEdit::OnNcPaint( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "EdtNcPaint", *m_pzsTag ) == 0 )
   {
      TraceLineS( "ZEdit::OnNcPaint: ", *m_pzsTag );
   }
#endif

   if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
        mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
   {
      m_pzmaComposite->m_pCtrl->UpdateWindow( );
   }

   CEdit::OnNcPaint( );
}

#if 1
void
ZEdit::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( "EdtNcPaint", *m_pzsTag ) == 0 )
   {
      CRect rect;
      BOOL  bUpdate;

      TraceLineS( "ZEdit::OnPaint: ", *m_pzsTag );
      bUpdate = GetUpdateRect( rect, 1 );
      TraceRect( "ZEdit::OnPaint UpdateRect", rect );
      GetWindowRect( rect );
      TraceRect( "ZEdit::OnPaint WindowRect", rect );
      GetClientRect( rect );
      TraceRect( "ZEdit::OnPaint ClientRect", rect );
   }
#endif

#if 0
   HFONT hFontOld = 0;
   HDC hDC = ::GetDC( m_hWnd );
   if ( m_pFont == 0 && m_cpcFormatPic )
   {
      HFONT hFontOld =
          (HFONT) ::SelectObject( hDC, GetStockObject( SYSTEM_FIXED_FONT ) );
   }

   CEdit::OnPaint( );
   if ( hFontOld )
      ::SelectObject( hDC, hFontOld );
#else
   CEdit::OnPaint( );
#endif
}
#endif

void
ZEdit::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::OnLButtonUp", "" );
#endif

   CEdit::OnLButtonUp( uModKeys, pt );  // let base class do its thing
   ProcessImmediateEvent( this, zEDITBOX_Click );
}

void
ZEdit::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZEdit - dtor
ZEdit::~ZEdit( )
{
// TraceLineS( "ZEdit::~ZEdit", "" );
   mDeleteInitA( m_pchBlobInfo );
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

zSHORT
ZEdit::SetCtrlMapping( zCPCHAR cpcViewName, zCPCHAR cpcEntityName,
                       zCPCHAR cpcAttributeName, zCPCHAR cpcContextName,
                       zLONG lColumn )
{
   zVIEW  vApp;

   ZMapAct::SetCtrlMapping( cpcViewName, cpcEntityName,
                            cpcAttributeName, cpcContextName, lColumn );
   if ( GetViewByName( &vApp, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      GetAttributeDisplayLength( &m_ulLth, vApp, *m_pzsEName,
                                 *m_pzsAName, *m_pzsContext );
      LimitTextLength( m_lFormatPicLth ? m_lFormatPicLth + 1 : m_ulLth );
   }

   MapFromOI( );
   return( 0 );
}

zBOOL
ZEdit::IsFormatPicChar( zLONG lPos )
{
   ASSERT( lPos < m_lFormatPicLth );

   // Add new masks as needed.
   return( m_cpcFormatPic[ lPos ] != '#' && // numeric (0-9)
           m_cpcFormatPic[ lPos ] != '?' && // alpha (a-z;A-Z)
           m_cpcFormatPic[ lPos ] != '*' && // alphanumeric (a-z;A-Z;0-9)
           m_cpcFormatPic[ lPos ] != '^' && // uppercase (A-Z)
           m_cpcFormatPic[ lPos ] != '_' && // lowercase (a-z)
           m_cpcFormatPic[ lPos ] != '$' && // currency (0-9;$;.;,)
           m_cpcFormatPic[ lPos ] != '€' && // currency euro (€)
           m_cpcFormatPic[ lPos ] != '£' && // currency pound
           m_cpcFormatPic[ lPos ] != '@' ); // hex (a-f;A-F;0-9)
}

zSHORT
ZEdit::GetReplacementChar( zLONG lPos, zCHAR cWant, char& cGet )
{
   if ( lPos >= m_lFormatPicLth )
      return( -1 );

   if ( m_cpcOverride[ lPos ] == 'Y' && cWant == m_cpcFormatPic[ lPos ] )
      return( 0 );

   while ( m_cpcOverride[ lPos ] == 'Y' && cWant != m_cpcFormatPic[ lPos ] )
   {
      lPos++;
      if ( lPos >= m_lFormatPicLth )
         return( -1 );
   }

   switch ( m_cpcFormatPic[ lPos ] )
   {
      case '#':   // numeric (0-9)
         if ( isdigit( cWant ) == 0 )
            return( -1 );

         cGet = cWant;
         break;

      case '?':   // alpha (a-z;A-Z)
         if ( isalpha( cWant ) == 0 )
            return( -1 );

         cGet = cWant;
         break;

      case '*':   // alphanumeric (a-z;A-Z;0-9)
         if ( isalnum( cWant ) == 0 )
            return( -1 );

         cGet = cWant;
         break;

      case '^':   // uppercase (A-Z)
         if ( isupper( cWant ) == 0 )
            return( -1 );

         cGet = cWant;
         break;

      case '_':   // lowercase (a-z)
         if ( islower( cWant ) == 0 )
            return( -1 );

         cGet = cWant;
         break;

      case '$':   // currency (0-9;$;.;,)
         if ( isdigit( cWant ) == 0 &&
              cWant != '$' &&
              cWant != '€' &&
              cWant != '£' &&
              cWant != '.' &&
              cWant != ',' )
         {
            return( -1 );
         }

         cGet = cWant;
         break;

      case '@':   // hex (a-f;A-F;0-9)
         if ( isxdigit( cWant ) == 0 )
            return( -1 );

         cGet = cWant;
         break;

      // Add new masks as needed

      default:
         cGet = m_cpcFormatPic[ lPos ];
         break;
   }

   return( (zSHORT) (lPos + 1) );
}

zLONG
ZEdit::GoToNextEntryChar( zLONG  lPos )
{
   if ( m_bLastKeyWasBackspace )
   {
      lPos--;

      while ( lPos >= 0 && IsFormatPicChar( lPos ) )
         lPos--;

      lPos++;
   }
   else
   {
      while ( lPos < m_lFormatPicLth && IsFormatPicChar( lPos ) )
         lPos++;
   }

   return( lPos );
}

/////////////////////////////////////////////////////////////////////////////
// ZEdit message handlers

void
ZEdit::ENErrspace( )
{
   Default( );             // pass it along to parent
}

void
ZEdit::ENHscroll( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::ENHscroll: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

void
ZEdit::ENMaxtext( )
{
   Default( );             // pass it along to parent
}

void
ZEdit::ENSetFocus( )
{
//#ifdef DEBUG_ALL
   if ( zstrcmp( "DIL_Text", (*m_pzsTag).GetString() ) == 0 )
      TraceLineS( "ZEdit::ENSetFocus: ", (*m_pzsTag).GetString() );
//#endif
   ProcessImmediateEvent( this, zEDITBOX_GetFocus );
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      Default( );             // pass it along to parent
      SetSel( 0, -1 );
   }
}

void
ZEdit::ENKillFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::ENKillFocus: ", *m_pzsTag );
#endif
   ZMapAct *pzmaGetFocus = 0;
   CWnd *pWnd = GetFocus( );
   if ( pWnd && pWnd->m_hWnd != m_hWnd )
   {
      // Attempt to locate the child window using the list of Zeidon MapActs.
      zULONG k = m_pZSubtask->m_pZMIXCtrl->GetCount( );
      while ( k )
      {
         pzmaGetFocus = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( k );
         if ( pzmaGetFocus && pzmaGetFocus->m_pCtrl &&
              pzmaGetFocus->m_pCtrl->m_hWnd == pWnd->m_hWnd )
         {
            break;
         }

         k--;
      }

      if ( k == 0 )
         pzmaGetFocus = 0;
   }

   if ( pzmaGetFocus )
      ProcessImmediateEvent( this, zEDITBOX_LoseFocus );

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

int
ZEdit::ResetStartChar( CString& csText, int nStart, int nEnd )
{
   if ( nStart < nEnd && nStart < m_lFormatPicLth )
   {
      int nPos = nStart;

      while ( nPos <= nEnd && nPos < m_lFormatPicLth )
      {
         if ( m_cpcOverride[ nPos ] == 'Y' )
            csText.SetAt( nPos, m_cpcFormatPic[ nPos ] );
         else
            csText.SetAt( nPos, m_chPlaceHolder );

         nPos++;
      }
   }

   return( nStart );
}

void
ZEdit::ENUpdate( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::ENUpdate: ", *m_pzsTag );
#endif
// ProcessEvent( this, 19 );

   if ( m_lFormatPicLth > 0 )
   {
      CString csText;
      int nStart;  // must be int for GetSel
      int nEnd;

      GetWindowText( csText );
      if ( zstrcmp( csText.GetString(), m_csSaveText.GetString() ) == 0 )
         return;

      GetSel( nStart, nEnd );
      if ( m_bLastKeyWasBackspace )
      {
         nStart = ResetStartChar( csText, nStart, nEnd );
         m_csSaveText.SetAt( nStart,
                             IsFormatPicChar( nStart ) ?
                               m_cpcFormatPic[ nStart ] : m_chPlaceHolder );
         CWndSetWindowText( m_csSaveText );
         zLONG lNext = GoToNextEntryChar( nStart );
         SetSel( lNext, lNext );
      }
      else
      if ( m_bLastKeyWasDelete )
      {
         nStart = ResetStartChar( csText, nStart, nEnd );
         m_csSaveText.SetAt( nStart,
                             IsFormatPicChar( nStart ) ?
                               m_cpcFormatPic[ nStart ] : m_chPlaceHolder );
         CWndSetWindowText( m_csSaveText );
         int nNext = GoToNextEntryChar( nStart + 1 );
         SetSel( nNext, nNext );
      }
      else
      {
         if ( nStart - 1 < 0 )
            return;

         zCHAR  chWanted = csText[ nStart - 1 ];
         zCHAR  chReplace = 0;

         zSHORT nPos = GetReplacementChar( nStart - 1, chWanted, chReplace );

         if ( nPos > 0 )
         {
            if ( m_csSaveText.GetLength( ) < nPos )
               m_csSaveText.GetBufferSetLength( nPos );

            m_csSaveText.SetAt( nPos - 1, chReplace );
            m_csSaveText.ReleaseBuffer( );
         }

         CWndSetWindowText( m_csSaveText );
         zLONG lNext = GoToNextEntryChar( nPos > 0 ? nPos : nStart );
         if ( nPos < 0 && lNext > 0 )
         {
            lNext--;
            ::MessageBeep( MB_ICONQUESTION );
         }

         SetSel( lNext, lNext );
      }
   }
   else
      Default( );             // pass it along to parent
}

void
ZEdit::ENVscroll( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::ENVscroll: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

BOOL
ZEdit::PreTranslateMessage( MSG *pMsg )
{
#ifdef DEBUG_ALL
// TraceLineI( "TZPainterWindow::PreTranslateMessage: ", pMsg->message );
#endif

   // Try to dismiss the current "drag" or "new ctrl" operation.
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN )
   {
      if ( pMsg->wParam == VK_ESCAPE )
      {
         TraceLineS( "ZEdit::PreTranslateMessage ", "VK_ESCAPE" );
         if ( m_ulMapActFlag2 & zMAPACT_INPLACE_CTRL )
         {
            m_ulMapActFlags &= ~zMAPACT_VISIBLE;
            m_ulMapActFlag2 &= ~zMAPACT_INPLACE_VISIBLE;
            m_pWndLastFocus->SetFocus( );
            m_pWndLastFocus = 0;
            m_pCtrl->SetParent( m_pWndParent );
            m_pCtrl->ShowWindow( SW_HIDE );
            return( TRUE ); // the message was translated and should not be dispatched
         }
      }
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}

void
ZEdit::OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::OnSysKeyDown: ", *m_pzsTag );
#endif
   zLONG lRC = 0;

   m_ulLastKey = uKey;
   if ( uKey != VK_SHIFT && uKey != VK_CONTROL && uKey != VK_MENU )
      lRC = ProcessImmediateEvent( this, zEDITBOX_KeyDown, (zPVOID) uFlags );

   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      m_bLastKeyWasDelete = (VK_DELETE == uKey);
      m_bLastKeyWasBackspace = (VK_BACK == uKey);
      CEdit::OnSysKeyDown( uKey, uRepeatCnt, uFlags );
   }
}

void
ZEdit::OnSysKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::OnSysKeyUp: ", *m_pzsTag );
#endif
   zLONG lRC = 0;

   m_ulLastKey = uKey;
   if ( uKey != VK_SHIFT && uKey != VK_CONTROL && uKey != VK_MENU )
      lRC = ProcessImmediateEvent( this, zEDITBOX_KeyUp, (zPVOID) uFlags );

   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      CEdit::OnSysKeyUp( uKey, uRepeatCnt, uFlags );
   }
}

void
ZEdit::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::OnKeyDown: ", *m_pzsTag );
#endif
   m_ulLastKey = uKey;
   zLONG lRC = ProcessImmediateEvent( this, zEDITBOX_KeyDown, (zPVOID) uFlags );
   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      m_bLastKeyWasDelete = (VK_DELETE == uKey);
      m_bLastKeyWasBackspace = (VK_BACK == uKey);
      CEdit::OnKeyDown( uKey, uRepeatCnt, uFlags );
   }
}

void
ZEdit::OnKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::OnKeyUp: ", *m_pzsTag );
#endif
   m_ulLastKey = uKey;
   zLONG lRC = ProcessImmediateEvent( this, zEDITBOX_KeyUp, (zPVOID) uFlags );
   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      CEdit::OnKeyUp( uKey, uRepeatCnt, uFlags );
   }
}

void
ZEdit::OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
   m_ulLastKey = uKey;
   zLONG lRC = ProcessImmediateEvent( this, zEDITBOX_Char, (zPVOID) uFlags );
   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      CEdit::OnChar( uKey, uRepeatCnt, uFlags );
   }
}

void
ZEdit::OnSize( UINT nType, int cx, int cy )
{
#if 0
   if ( zstrstr( *m_pzsTag, "Inplace" ) )
      TraceLine( "ZEdit::OnSize: %s  CX: %d  CY: %d", *m_pzsTag, cx, cy );
#endif

   CEdit::OnSize( nType, cx, cy );
}

void
ZEdit::OnWindowPosChanged( WINDOWPOS FAR *pWndPos )
{
#if 0
   if ( zstrstr( *m_pzsTag, "Inplace" ) )
      TraceLine( "ZEdit::OnWindowPosChanged: %s  X: %d  Y: %d  CX: %d  CY: %d",
                 *m_pzsTag, pWndPos->x, pWndPos->y, pWndPos->cx, pWndPos->cy );
#endif

   CEdit::OnWindowPosChanged( pWndPos );
}

#if 0
CString
ZEdit::GetFormatPic( )
{
   return( m_cpcFormatPic );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
Edit( ZSubtask *pZSubtask,
      CWnd     *pWndParent,
      ZMapAct  *pzmaComposite,
      zVIEW    vDialog,
      zSHORT   nOffsetX,
      zSHORT   nOffsetY,
      zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   if ( pCtrlDef->Tag[ 0 ] == '_' &&
        pCtrlDef->Tag[ 1 ] == '_' )
   {
      if ( pCtrlDef->Tag[ 2 ] == 'T' &&
           pCtrlDef->Tag[ 3 ] == 'i' &&
           pCtrlDef->Tag[ 4 ] == 'm' &&
           pCtrlDef->Tag[ 5 ] == 'e' )
      {
         return( new ZTime( pZSubtask, pWndParent,
                            pzmaComposite, vDialog,
                            nOffsetX, nOffsetY, pCtrlDef ) );
      }
   }

   return( new ZEdit( pZSubtask, pWndParent,
                      pzmaComposite, vDialog,
                      nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=ED_HiliteText
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_HiliteText
//                                              10/07/97 Modified: 10/07/97
//
//  PURPOSE:    This operation hilites an edit control's text
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nTextStartPos - The first character at which to begin
//                            hilighting the edit control's text (if zero
//                            is specified, all hilighting is set off)
//              nTextEndPos - The last character of the edit control's text
//                            to be hilighted (0 can be specified to cause
//                            the remainder of the text to be hilighted).
//
//  REMARK:     To hilight the entire text range, nTextStartPos should be
//              set to one and nTextEndPos should be set to zero.
//
//  RETURNS:    0 - Hilite set successfully
//              1 - Hilite not set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ED_HiliteText( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zSHORT  nTextStartPos,
               zSHORT  nTextEndPos )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
#ifdef zREMOTE_SERVER
      zCHAR szPos[ 32 ];

      _ltoa_s( nTextStartPos, szPos );
      SetRemoteZCtrlAttribute( pzma, "EditBox", "SelStart", szPos );
      _ltoa_s( nTextEndPos, szPos );
      SetRemoteZCtrlAttribute( pzma, "EditBox", "SelEnd", szPos );
      return( 0 );
#else
      ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
      if ( pEdit && SetFocusToCtrl( vSubtask, cpcCtrlTag ) == 0 )
         return( pEdit->SelectText( nTextStartPos, nTextEndPos ) );

      TraceLineS( "drvr - Invalid control type for ED_HiliteText ",
                  cpcCtrlTag );
#endif
   }

   return( -1 );
}

//./ ADD NAME=ED_GetSelText
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_GetSelText
//                                              10/07/97 Modified: 10/07/97
//
//  PURPOSE:    This operation gets the selected text and start position
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pchSelectedText - Return buffer for selected text.
//              nMaxTextLth - The maximum return text length including the
//                            null terminator.
//
//  RETURNS:   -1 - Error locating control
//             otw - starting position of selected text (1-based).  If no
//                   text is selected, the 1-based caret position is returned.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ED_GetSelText( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zPCHAR  pchSelectedText,
               zSHORT  nMaxTextLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   *pchSelectedText = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
#ifdef zREMOTE_SERVER
#else
      int  nStart;
      int  nEnd;

      ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
      if ( pEdit && SetFocusToCtrl( vSubtask, cpcCtrlTag ) == 0 )
      {
         CString cs;
         pEdit->GetSel( nStart, nEnd );
         if ( pchSelectedText && nStart < nEnd )
         {
            CString csText;

            nEnd = min( nEnd, nStart + nMaxTextLth - 1 );
            pEdit->GetWindowText( csText );
            zmemcpy( pchSelectedText, ((zCPCHAR) csText) + nStart, nEnd - nStart );
            pchSelectedText[ nEnd - nStart ] = 0;
         }

         return( nStart + 1 );  // 1-based index
      }

      TraceLineS( "drvr - Invalid control type for ED_GetSelText ",
                  cpcCtrlTag );
#endif
   }

   return( -1 );
}

//./ ADD NAME=ED_SetSelText
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_SetSelText
//                                              10/07/97 Modified: 10/07/97
//
//  PURPOSE:    This operation replaces the selected text or inserts the
//              specified text at the current caret position.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cpcText     - Replacement text.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
ED_SetSelText( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zPCHAR  pchText )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
#ifdef zREMOTE_SERVER
#else
      ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
      if ( pEdit && SetFocusToCtrl( vSubtask, cpcCtrlTag ) == 0 )
      {
         pEdit->ReplaceSel( pchText );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for ED_SetSelText ",
                  cpcCtrlTag );
#endif
   }

   return( -1 );
}

//./ ADD NAME=ED_LimitTextLength
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_LimitTextLength
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the maximum length for an edit control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nTextLth    - The maximum length for the control's text
//                            (without the null terminator).
//
//  RETURNS:    0 - Limit set successfully
//              1 - Limit not set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
ED_LimitTextLength( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag,
                    zSHORT  nTextLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
      if ( pEdit )
         return( pEdit->LimitTextLength( nTextLth ) );

      TraceLineS( "drvr - Invalid control type for ED_LimitTextLength ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=ED_SetCtrlMapping
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_SetCtrlMapping
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets a new Mapping for a Edit Control
//
//  PARAMETERS: vSubtask         - The subtask view for the window containing
//                                 the control to be disabled.
//              cpcCtrlTag       - The unique name of the control.
//              cpcViewName      - new View Name to be mapped
//              cpcEntityName    - new Entity Name to be mapped
//              cpcAttributeName - new Attribute Name to be mapped
//              cpcContextName   - new Context Name to be mapped
//
//  RETURNS:    0 - New Mapping set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
ED_SetCtrlMapping( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zCPCHAR cpcViewName,
                   zCPCHAR cpcEntityName,
                   zCPCHAR cpcAttributeName,
                   zCPCHAR cpcContextName )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zVIEW    vApp;
   zULONG   ulLth;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      zSHORT nRC = pzma->SetCtrlMapping( cpcViewName, cpcEntityName,
                                         cpcAttributeName, cpcContextName );

      if ( GetViewByName( &vApp, cpcViewName,
                          pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         GetAttributeDisplayLength( &ulLth, vApp, cpcEntityName,
                                    cpcAttributeName, cpcContextName );
         ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
         if ( pEdit )
            pEdit->LimitTextLength( (zSHORT) ulLth );
      }

      pzma->MapFromOI( );
      return( nRC );
   }

   return( -1 );
}

//./ ADD NAME=ED_GetLastKey
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_GetLastKey
//                                          03/07/2002 Modified: 03/07/2002
//
//  PURPOSE:    This operation gets the last keystroke for an Edit Control.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the control.
//              cpcCtrlTag - The unique name of the control.
//
//  RETURNS:    0 - New Mapping set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zULONG OPERATION
ED_GetLastKey( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
      if ( pEdit )
         return( pEdit->m_ulLastKey );

      TraceLineS( "drvr - Invalid control type for ED_GetLastKey ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=ED_SetPlaceHolder
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_SetPlaceHolder
//                                          03/07/2002 Modified: 03/07/2002
//
//  PURPOSE:    This operation sets the place holder character for a
//              formatted Edit Control.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the control.
//              cpcCtrlTag - The unique name of the control.
//              chPlaceHolder - Place holder character.
//
//  RETURNS:    0 - New placer holder character set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
ED_SetPlaceHolder( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zCPCHAR cpcPlaceHolder )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEdit *pEdit = DYNAMIC_DOWNCAST( ZEdit, pzma->m_pCtrl );
      if ( pEdit && cpcPlaceHolder && *cpcPlaceHolder )
      {
         pEdit->m_chPlaceHolder = cpcPlaceHolder[ 0 ];
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for ED_SetPlaceHolder ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=ED_FloatSetRange
// Source Module=zdctledt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ED_FloatSetRange
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the minimum and maximum values for
//              a float (money) type of edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              dfMin       - The minimum value for the control
//              dfMax       - The maximum value for the control
//
//  RETURNS:    0 - Range not set
//              1 - Range set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ED_FloatSetRange( zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag,
                  zDECIMAL dxMin,
                  zDECIMAL dxMax )
{
//??? ZSubtask *pZSubtask;
//??? ZMapAct  *pzma;

//??? if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
//???    return( ((ZEdit *) pzma->m_pCtrl)->SetRange( dxMin, dxMax ) );

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 1
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//           Abstract (Pure Virtual) Class:  ZMFE
//
//                Multi-Field Edit Control
//
//                Version: 2.0    Date:  04/16/00
//
//
//                Developed By: George Chastain,
//                              Huntsville, Alabama.
//                              e-mail: gcoutpost@hotmail.com
//
//
// ZMFE.cpp : implementation file
//
// With this changes and additions made to the support classes for
// ZMFE, this source file has become somewhat large.  Ordinarily
// you would want to split this up into separate source files to make it more
// manageable.  However, I wanted to make it as easy as possible to add
// this class to a project so I kept it and its support classes in a single
// .h and .cpp.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
#include <math.h>

ZMFE::ZMFE( )
{
   m_bValid         = FALSE;
   m_NumberOfFields = 0;
   m_SelectedField  = -1;
   m_FieldSpecs     = 0;
}

ZMFE::~ZMFE( )
{
   mDeleteInitA( m_FieldSpecs );

   // The vector objects possess a constructor that will be automatically
   // called to destory those objects.
}

BEGIN_MESSAGE_MAP( ZMFE, _PARENT_CLASS )
   //{{AFX_MSG_MAP( ZMFE )
   ON_WM_KEYDOWN( )
   ON_WM_SETFOCUS( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_NCPAINT( )
   ON_WM_MOUSEMOVE( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_CHAR( )
   ON_WM_RBUTTONUP( )
   ON_WM_CREATE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

int
ZMFE::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CGCColorEdit::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   // I want to turn off the auto horizontal scroll for this Multi-Field Edit
   // Control.
   ModifyStyle( ES_AUTOHSCROLL, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This procedure updates the display of the Multi-Field Edit Control.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZMFE::FormatAndDisplay( )
{
   ASSERT( m_FieldSpecs );
   ASSERT( m_FieldWidths.size( ) != 0 );

   CString displayval;

   // Format the data.
   zSHORT k = 0;

   // Build the string to display in the edit box.
   if ( !m_Prefix.IsEmpty( ) )
      displayval = m_Prefix;   // Start with the Prefix if there is one.

   if ( m_FieldSeparators.size( ) == 1 )
   {
      // Build this way if we are using the same Field Separator to separate
      // all Fields.
      for ( k = 0; k < m_NumberOfFields; k++ )
      {
         displayval = displayval + m_FieldContents[ k ];
         if ( k < (m_NumberOfFields - 1) )
            displayval = displayval + m_FieldSeparators[ 0 ];
      }
   }
   else
   {
      // Build this way if we are potentially using a different Field
      // Separator to separate each Field.
      for ( k = 0; k < m_NumberOfFields; k++ )
      {
         displayval = displayval + m_FieldContents[ k ];
         if ( k < ( m_NumberOfFields - 1 ) )
            displayval = displayval + m_FieldSeparators[ k ];
      }
   }

   // Append the Postfix if there is one.
   if ( !m_Postfix.IsEmpty( ) )
      displayval = displayval + m_Postfix;

   // Now that I've formatted the text to display, I can
   // privately call parent class's SetWindowText( ) method.
   CGCColorEdit::SetWindowText( displayval );

   UpdateData( FALSE );             // Update the display.

   // Highlight the appropriate field as being selected.
   SetSel( m_FieldSpecs[ m_SelectedField ].start,
           m_FieldSpecs[ m_SelectedField ].stop );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This handler provides the functionality to allow the operator to
// navigate to each field in the Multi-Field Edit Control using the
// left/right cursor control (arrow) keys and the space bar.  If the
// developer overrides this handler in the derived class, the developer
// should call this handler to ensure that the navigation functionality
// is available.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZMFE::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
   // We do NOT call the OnKeyDown handler inherited from CEdit!!
   switch ( nChar )
   {
      case VK_SPACE:
      case VK_RIGHT:
         // Move selection to next field to the right when right arrow or
         // space bar selected.  If we are at the end of the control,
         // cycle around and select the first field in the control.
         if ( m_SelectedField < int( m_NumberOfFields - 1 ) )
            m_SelectedField += 1;
         else
            m_SelectedField = 0;

         break;

      case VK_LEFT:
         // Move selection to next field to the left when left arrow
         // selected.  If we are at the first field of the control, cycle
         // around and select the last field in the control.
         if ( m_SelectedField > 0 )
            m_SelectedField -= 1;
         else
            m_SelectedField = m_NumberOfFields - 1;

         break;
   }

   // Highlight the appropriate field.
   SetSel( m_FieldSpecs[ m_SelectedField ].start,
         m_FieldSpecs[ m_SelectedField ].stop );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This handler provides the functionality to ensure that the default field
// (the first one) is selected when input focus is moved to this control using
// the tab key or some other mechanism other than clicking with the mouse.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZMFE::OnSetFocus( CWnd* pOldWnd )
{
   // Call base class handler.
   CGCColorEdit::OnSetFocus( pOldWnd );

   // Default to selecting the first field (hemisphere field) if the focus
   // is being set by some mechanism other than clicking on the field.
   ASSERT( m_FieldSpecs );

   // Highlight the appropriate field as being selected.
   SetSel( m_FieldSpecs[ m_SelectedField ].start,
         m_FieldSpecs[ m_SelectedField ].stop );

   HideCaret( );     // Hide the caret.
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This procedure is provided to allow the derived class to update the
// contents of the fields.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZMFE::Update( const vector<CString>& FieldContents )
{
   ASSERT( int( FieldContents.size( ) ) >= m_NumberOfFields );

   if ( int( FieldContents.size( ) ) < m_NumberOfFields )
      return( FALSE );

   zSHORT k = 0;

   for ( k = 0; k < int( m_NumberOfFields ); k++ )
      m_FieldContents[ k ] = FieldContents[ k ].Left( m_FieldWidths[ k ] );

   FormatAndDisplay( );
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This procedure is provided to allow the derived class ( or other part of
// the application ) to programmatically select a field in the control.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZMFE::SelectField( const zSHORT FieldNumber )
{
   // Validate specified field.

   ASSERT( ( FieldNumber <= m_NumberOfFields ) && ( FieldNumber >= 0 ) );
   if ( FieldNumber > m_NumberOfFields ) return( FALSE );

   m_SelectedField = FieldNumber;  // Store the selected field.

   ASSERT( m_FieldSpecs );

   // Highlight the appropriate field as selected.
   SetSel( m_FieldSpecs[ FieldNumber ].start,
           m_FieldSpecs[ FieldNumber ].stop );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This procedure is provided to allow the derived class (or other part of
// the application) to programmatically determine which field in the control
// is currently selected.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZMFE::GetSelectedField( ) const
{
   return m_SelectedField; // return currently selected field. A 0-based index.
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This handler provides the functionality to ensure that the
// the appropriate field is selected when the operator clicks in the control
// with the left mouse button.  If the developer overrides this in the
// derived class, the developer must be sure and call this version so that
// the selection of a field by clicking with the mouse will be available.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZMFE::OnLButtonDown( UINT nFlags, CPoint pt )
{
   ASSERT( m_FieldSpecs );

   // This message handler would need to be modified if the control that
   // you are creating has more or less than four fields.
   CGCColorEdit::OnLButtonDown( nFlags, pt );

   // Where was the mouse when the user clicked in this control?
   zSHORT pos = this->CharFromPos( pt );

   // Determine the location of that pt relative to the individual
   // fields within the control.  Set the selection for that field.
   zSHORT k = 0;

   if ( pos <= ( m_FieldSpecs[ 0 ].stop ) )
   {
      // If the operator clicked before or at the first character of the
      //first field...
      m_SelectedField = 0;
   }
   else
   if ( pos >= m_FieldSpecs[ m_FieldContents.size( ) - 1 ].stop )
   {
      // Else, if the operator clicked at or after the last character of the
      // last field...
      m_SelectedField = (zSHORT) m_FieldContents.size( ) - 1;
   }
   else
   {
      // Else, the operator didn't click before the first field or after the
      // last one. Determine between which fields the operator clicked.  If
      // the operator clicked at or after the last character of Field A but
      // before the first character of Field B, then Field B will be selected.
      for ( k = 1; k <  m_NumberOfFields; k++ )
      {
         if ( (pos >= m_FieldSpecs[ k - 1 ].stop) &&
              (pos <= m_FieldSpecs[ k ].stop) )
         {
            m_SelectedField = k;
         }
      }
   }

   // Highlight the appropriate field as being selected.
   SetSel( m_FieldSpecs[ m_SelectedField ].start,
           m_FieldSpecs[ m_SelectedField ].stop );

   HideCaret( );  // Hide the caret.
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZMFE::OnNcPaint( )
// This handler provides the functionality to update the display when the is
// control first is first displayed and to ensure that the caret is hidden.
// If the developer overrides this in the derived class, the developer
// must be sure and call this version.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZMFE::OnNcPaint( )
{
   ASSERT( m_bValid );
   // Use the OnNcPaint( ) handler as a mechanism to prevent the
   // caret from being shown when the control is first displayed on screen
   // and the control has input focus.

   // Also, use the OnNcPaint( ) handler as a mechanism to automatically
   // initialize the displayed contents of the control.
   CGCColorEdit::OnNcPaint( );   // Be sure and call the parent class.
   FormatAndDisplay( );          // Update the display.
   HideCaret( );                 // Hide the caret
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZMFE::Format( )
// This procedure is provided to allow the derived class to format the
// Multi-Field Edit Control.  Calling this method allows the derived class
// to change the number of fields, the width of the fields, and the field
// separators to use.  The derived class should call Update( ) to update the
// contents of the fields as needed after the control has been formatted
// by calling Format( ).
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZMFE::Format( const vector<CString> FieldContents,
              const vector<CString> FieldSeparators,
              const CString Prefix,
              const CString Postfix )

{
   zSHORT k = 0;

   // You have to specify something in the FieldContents and FieldSeparators
   // parameters.

   // There must be at least 2 entries in FieldContents.
   ASSERT ( FieldContents.size( ) > 1 );

   // There must be at least 1 entry in the FieldSeparators.
   ASSERT( FieldSeparators.size( ) >= 1 );

   if ( FieldContents.size( ) < 2 )
      return( FALSE );

   if ( FieldSeparators.size( ) < 1 )
      return( FALSE );

   // Verify that either a single field separator was specified to be used
   // to separate all fields or that enough field separators were specified
   // for each separation pt.

   // Debug validation.
   ASSERT ( (FieldSeparators.size( ) == 1) ||
            ((FieldSeparators.size( ) > 1) &&
             (FieldSeparators.size( ) == (FieldContents.size( ) - 1))) );

   // Runtime (release version) validation.
   if ( (FieldSeparators.size( ) != 1) &&
        (FieldSeparators.size( ) != (FieldContents.size( ) - 1)) )
   {
      return( FALSE );
   }

   // There must be a non-zero string in each entry of FieldContents.
   for ( k = 0; k < int( FieldContents.size( ) ); k++ )
   {
      ASSERT ( FieldContents[ k ].GetLength( ) > 0 );
      if ( FieldContents[ k ].GetLength( ) < 1 )
         return( FALSE );
   }

   // Throw away all previously stored stuff.
   m_Prefix = Prefix;
   m_Postfix = Postfix;

   // Delete the stored vector of Field Contents.
   if ( m_FieldContents.size( ) > 0 )
   {
      m_FieldContents.erase( m_FieldContents.begin( ),
                             m_FieldContents.end( ) );
      m_NumberOfFields = 0;
   }

   // Delete the stored vector of Field Separators.
   if ( m_FieldSeparators.size( ) > 0 )
   {
      m_FieldSeparators.erase( m_FieldSeparators.begin( ),
                               m_FieldSeparators.end( ) );
   }

   // Delete the stored vector of Field Widths.
   if ( m_FieldWidths.size( ) > 0 )
   {
      m_FieldWidths.erase( m_FieldWidths.begin( ),
                           m_FieldWidths.end( ) );
   }

   // Delete the stored array of Field Specs.
   if ( m_FieldSpecs )
   {
      delete [] m_FieldSpecs;
      m_FieldSpecs = 0;
   }

   // Reformat the Multi-Field Edit Box.
   m_NumberOfFields = (zSHORT) FieldContents.size( );
   m_SelectedField  = 0;

   // Store the new Field Separators.
   for ( k = 0; k < int( FieldSeparators.size( ) ); k++ )
      m_FieldSeparators.push_back( FieldSeparators[ k ] );

   // Store the new Field Contents.
   for ( k = 0; k < m_NumberOfFields; k++ )
      m_FieldContents.push_back( FieldContents[ k ] );

   // Store the new Field Widths.
   for ( k = 0; k < m_NumberOfFields; k++ )
      m_FieldWidths.push_back( FieldContents[ k ].GetLength( ) );

   // Allocate memory for the new Field Specs.
   m_FieldSpecs = new FIELD_SPEC_TYPE[ m_NumberOfFields ];

   // Define the new start and end positions within the edit box for each
   // field.

   // Start with determining the start and stop positions of the first field
   // on the left of the control.
   if ( Prefix.IsEmpty( ) )
   {
      // No Prefix is being used.
      m_FieldSpecs[ 0 ].start = 0;
      m_FieldSpecs[ 0 ].stop = m_FieldWidths[ 0 ];
   }
   else
   {
      // A Prefix is being used.  Take its length into account when
      // calculating the start/end positions of the first field.
      m_FieldSpecs[ 0 ].start = Prefix.GetLength( );
      m_FieldSpecs[ 0 ].stop = m_FieldSpecs[ 0 ].start + m_FieldWidths[ 0 ];
   }

   // When calculating the start and end positions of the remaining fields,
   // we must take into account the lengths of each field separator in turn.
   if ( m_FieldSeparators.size( ) == 1 )
   {
      for ( k = 1; k < m_NumberOfFields; k++ )
      {
         m_FieldSpecs[ k ].start = m_FieldSpecs[ k - 1 ].stop +
                                   m_FieldSeparators[ 0 ].GetLength( );
         m_FieldSpecs[ k ].stop = m_FieldSpecs[ k ].start +
                                  m_FieldWidths[ k ];
      }
   }
   else
   {
      for ( k = 1; k < m_NumberOfFields; k++ )
      {
         m_FieldSpecs[ k ].start = m_FieldSpecs[ k - 1 ].stop +
                                   m_FieldSeparators[ k - 1 ].GetLength( );
         m_FieldSpecs[ k ].stop = m_FieldSpecs[ k ].start +
                                  m_FieldWidths[ k ];
      }
   }

   // If the window exists, update the displayed contents.
   if ( this->m_hWnd )
      FormatAndDisplay( );

   m_bValid = TRUE;   // The control has been formatted.
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                           DO NOTHING OVERRIDES
//
// The following overrides are provided to block access to certain
// functionality.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZMFE::SetWindowText( zCPCHAR lpszString )
{
   // SetWindowText( ) - Disabled.  Calling this override of the parent
   //                    class's SetWindowText( ) has no effect.  The
   //                    client must call Format( ) and Update( ) to change
   //                    the contents of the control.
}

void
ZMFE::OnMouseMove( UINT nFlags, CPoint pt )
{
   // We want to override the OnMouseMove handler so that the operator
   // can not click-and-drag to select multiple fields in the control.
   // This "do nothing" override should remain unless you desire that the
   // user be able to select the entire contents.
   // However, this is usually not recommended as such an action would
   // not fit with the logical structure of the Multi-Field Edit Control.
}

void
ZMFE::OnLButtonDblClk( UINT nFlags, CPoint pt )
{
   // We want to override the OnLButtonDblClk handler so that the
   // operator can not select the entire contents of the control by
   // double clicking.
   //
   // This "do nothing" override should remain unless you desire that the
   // user be able to select the entire contents.
   // However, there is no need to do this unless you add a "copy" option
   // to the control's context menu.
   //
   // We definitely do not want to implement a PASTE option for the entire
   // edit control because ZMFE could not control the format of the display.
}

void
ZMFE::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
   // This "do nothing" override is to ensure that if the client
   // calls the inherited WM_CHAR handler ( i.e. makes a call to
   // "ZMFE::OnChar( )", that it has no effect on
   // the display.  If the WM_CHAR message isn't trapped here,
   // making a call to the OnChar( ) inherited from CEdit can corrupt
   // the display depending on when the client makes the call.
}

void
ZMFE::OnRButtonUp( UINT nFlags, CPoint pt )
{
   // This "do nothing" override is to ensure that the inherited context
   // menu is never displayed.
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CGCColorEdit

BEGIN_MESSAGE_MAP(CGCColorEdit, CEdit)
   //{{AFX_MSG_MAP(CGCColorEdit)
   ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_NCPAINT( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CGCColorEdit::CGCColorEdit( )
{
   m_CurrentBackColor = DEFAULT_BACKGROUND_COLOR;
   m_CurrentTextColor = DEFAULT_TEXT_COLOR;
   m_BackBrush.CreateSolidBrush( m_CurrentBackColor );
   m_bBold = FALSE;
   m_bItalic = FALSE;
   m_bUnderline = FALSE;
   m_bStatic = FALSE;
   m_bModal = FALSE;
   m_bFlat = FALSE;
}

CGCColorEdit::~CGCColorEdit( )
{
   m_BackBrush.DeleteObject( );
}

/////////////////////////////////////////////////////////////////////////////
// CGCColorEdit message handlers

COLORREF
CGCColorEdit::SetBackgroundColor( const COLORREF bkColor )
{
   COLORREF prevColor = m_CurrentBackColor; // Prepare to return previous color.
   m_CurrentBackColor = bkColor;            // Store new background color.

   m_BackBrush.DeleteObject( );             // Delete previous background brush.
   m_BackBrush.CreateSolidBrush( bkColor ); // Create new background brush.

   this->Invalidate( TRUE );                // Force edit window to repaint.

   return( prevColor );                     // Return previous background color.
}

COLORREF
CGCColorEdit::SetTextColor( const COLORREF textColor )
{
   COLORREF prevColor = m_CurrentTextColor; // Prepare to return previous text color.
   m_CurrentTextColor = textColor;          // Store new text color.

   this->Invalidate( TRUE );                // Force edit window to repaint.

   return( prevColor );                     // Return previous text color.
}

COLORREF
CGCColorEdit::GetBackgroundColor( ) const
{
   return( m_CurrentBackColor );
}

COLORREF
CGCColorEdit::GetTextColor( ) const
{
   return( m_CurrentTextColor );
}

void
CGCColorEdit::GetBackgroundColorRGB( BYTE& Red, BYTE& Green, BYTE& Blue ) const
{
   Red = GetRValue( m_CurrentBackColor );
   Green = GetGValue( m_CurrentBackColor );
   Blue = GetBValue( m_CurrentBackColor );
}

void
CGCColorEdit::GetTextColorRGB( BYTE& Red, BYTE& Green, BYTE& Blue ) const
{
   Red = GetRValue( m_CurrentTextColor );
   Green = GetGValue( m_CurrentTextColor );
   Blue = GetBValue( m_CurrentTextColor );
}

/////////////////////////////////////////////////////////////////////////////
//
// CGCColorEdit::CtlColor()
// Handles the appropriate colors when the control is repainted on screen
//
//                         REMEMBER
//
// If you handle WM_CTLCOLOR messages in your parent window (the window that
// contains this control) you must be sure to call the parent window's base
// class OnCtlColor() handler if the parent's OnCtlColor() handler is being
// called for this control.  Otherwise, the OnCtlColor() handler in the
// parent window will OVERRIDE the OnCtlColor() handler for this control.
//
// Windows is designed that way to give the parent a shot at handling the
// message before reflecting it back to this control.
/////////////////////////////////////////////////////////////////////////////

HBRUSH
CGCColorEdit::CtlColor( CDC *pDC, UINT nCtlColor )
{
   pDC->SetTextColor( m_CurrentTextColor );
   pDC->SetBkMode( TRANSPARENT );
   return static_cast<HBRUSH>( m_BackBrush.GetSafeHandle( ) );
}

zBOOL
CGCColorEdit::SetPointFont( const int PointSize, const zCPCHAR cpcFontName )
{
   zBOOL bResult = FALSE;

   CDC *pDC = this->GetDC( ); // get device context for this edit box
   m_Font.DeleteObject( );    // delete previous CGdiObject and free its memory

   // Create a new Font object.
   bResult = m_Font.CreatePointFont( PointSize * 10, cpcFontName, pDC );

   if ( bResult == FALSE )
      return( bResult );

   CEdit::SetFont( &m_Font, TRUE ); // set the new font for this edit box
   ReleaseDC( pDC ); // release the device context
   return( bResult );
}

void
CGCColorEdit::GetFontName( CString& rcsFontName ) const
{
   LOGFONT lf;
   CFont   *pFont = 0;

   pFont = CEdit::GetFont();      // get a pointer to the current font
   pFont->GetLogFont( &lf );      // get the current logical font

   rcsFontName = lf.lfFaceName;   // return the face name of current font
}

zBOOL
CGCColorEdit::SetFontIndirect( const LOGFONT *lpLogFont )
{
   zBOOL bResult = FALSE;

   m_Font.DeleteObject( );  // delete previous CGdiObject and free its memory

   // Create a new Font object.
   bResult = m_Font.CreateFontIndirect( lpLogFont );

   if ( bResult == FALSE )
      return( bResult );

   CEdit::SetFont( &m_Font, TRUE ); // set the new font for this edit box
   return bResult;
}

zBOOL
CGCColorEdit::SetPointFontIndirect( const LOGFONT *lpLogFont )
{
   zBOOL bResult;

   // Delete previous CGdiObject and free its memory.
   m_Font.DeleteObject();

   // Get device context for this edit box.
   CDC *pDC = this->GetDC();

   // Create a new Font object.
   bResult = m_Font.CreatePointFontIndirect( lpLogFont, pDC );

   if ( bResult == FALSE )
   {
      ReleaseDC( pDC ); // release the device context
      return( bResult );
   }

   // Set the new font for this edit box.
   CEdit::SetFont( &m_Font, TRUE );

   ReleaseDC( pDC ); // release the device context
   return( bResult );
}

LOGFONT
CGCColorEdit::GetLogFont( ) const
{
   LOGFONT lf;
   CFont   *pFont = 0;

   pFont = this->GetFont( );
   pFont->GetLogFont( &lf );
   return( lf );      // Return the logical font structure.
}

zBOOL
CGCColorEdit::SetUnderline( const zBOOL bOn )
{
   if ( m_bUnderline == bOn )
      return( m_bUnderline );

   zBOOL prev = m_bUnderline;     // Prepare to return previous underline state.
   m_bUnderline = bOn;            // Set the new state.

   // Get the current logical font so that we will only change the underline state.
   LOGFONT lf = this->GetLogFont( );
   lf.lfUnderline = (BYTE) bOn;   // Set the new state.
   SetFontIndirect( &lf );        // Set the font with the new underline state.

   return( prev );                // Return previous underline state.
}

zBOOL
CGCColorEdit::SetItalic( const zBOOL bOn )
{
   if ( m_bItalic == bOn )
      return( m_bItalic );

   zBOOL prev = m_bItalic;              // Prepare to return previous italic state.
   m_bItalic = bOn;                 // Store the new state.

   LOGFONT lf = this->GetLogFont( );    // Get the current logical font.
   lf.lfItalic = (BYTE) bOn;      // Set the new italic state.
   SetFontIndirect( &lf );        // Set the font with the new italic state.
   return( prev );                     // Return previous italic state.
}

zBOOL
CGCColorEdit::SetBold( const zBOOL bOn )
{
   if ( m_bBold == bOn )
      return( m_bBold );

   zBOOL prev = m_bBold;          // Prepare to return previous bold state.
   m_bBold = bOn;                 // Store the new state.

   LOGFONT lf;
   lf = this->GetLogFont( );      // Get the current logical font.

   switch ( bOn )
   {
      case TRUE:
         lf.lfWeight = 700;       // Standard weight for bold face.
         break;

      case FALSE:
         lf.lfWeight = 400;       // Standard weight for normal text.
   }

   SetFontIndirect( &lf );        // Set the font with new weight.
   return( prev );                // Return previous bold state.
}

void
CGCColorEdit::SetFontWeight( const int Weight )
{
   if ( (Weight < 1) || (Weight > 1000) )
      return;

   LOGFONT lf = this->GetLogFont();  // Get the current logical font.
   lf.lfWeight = Weight;          // Set the font weight as specified.
   SetFontIndirect( &lf );        // Set the font with the new weight.
}

zBOOL
CGCColorEdit::SetModalFrame( const zBOOL bOn )
{
   if ( m_bModal == bOn )
      return( m_bModal );

   m_bModal = bOn;

   zBOOL Result = FALSE;

   if ( bOn )
      Result = ModifyStyleEx( 0,WS_EX_DLGMODALFRAME, SWP_DRAWFRAME );
   else
      Result = ModifyStyleEx( WS_EX_DLGMODALFRAME, 0, SWP_DRAWFRAME );

   this->Invalidate( );
   return( Result );
}

zBOOL
CGCColorEdit::SetStaticFrame( const zBOOL bOn )
{
   if (m_bStatic == bOn)
      return( m_bStatic );

   zBOOL Result = FALSE;
   m_bStatic = bOn;

   if ( bOn )
      Result = ModifyStyleEx( 0, WS_EX_STATICEDGE, SWP_DRAWFRAME );
   else
      Result = ModifyStyleEx( WS_EX_STATICEDGE, 0, SWP_DRAWFRAME );

   this->Invalidate( );
   return( Result );
}

COLORREF
CGCColorEdit::SetBackgroundPattern( CBitmap *pattern )
{
   COLORREF prev = m_CurrentBackColor;
   m_CurrentBackColor = 0;
   m_BackBrush.DeleteObject( );               // Delete previous brush.
   m_BackBrush.CreatePatternBrush( pattern ); // Create pattern brush.
   this->Invalidate( );                       // Force a redraw.
   return( prev );
}

void
CGCColorEdit::OnNcPaint( )
{
   if ( m_bFlat == FALSE )
      CEdit::OnNcPaint( );
}

zBOOL
CGCColorEdit::SetFlat( const zBOOL mbFlat )
{
   BOOL Prev = m_bFlat;
   m_bFlat = mbFlat;

   CWnd *pParent = GetParent( );
   pParent->Invalidate( );
   return( Prev );
}

//#pragma warning( default : 4100 )    // Re-enable C4100 compiler warnings.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//             Class Implementation for support class CGCIntegerField
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef GCMFESUPPBASE

#if 0  // DKS
zBOOL
IsField( CGCFieldBase *ptr )
{
   return( dynamic_cast<CGCFieldBase *>( ptr ) != 0 );
}

CGCFieldBase *
ConvertPtr( CGCFieldBase *ptr )
{
   return dynamic_cast<CGCFieldBase *>( ptr );
}
#endif

#endif

#ifdef GCMFESUPP_INT

#ifdef GCMFESUPPNBASE

#if 0  // DKS
zBOOL
IsNumField( CGCNumBase *ptr )
{
   return( dynamic_cast<CGCNumBase *>( ptr ) != 0 );
}
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
CGCIntegerField::CGCIntegerField( )
{
   m_lValue = 0;
   m_lMinValue = 0;
   m_lMaxValue = 0;
   m_lInterval = 0;
   m_bAllowNegatives = FALSE;
   m_bIsNegative = FALSE;

   // The CString member variable m_csValue is initialized by CString ctor.
}

/////////////////////////////////////////////////////////////////////////////
CGCIntegerField::CGCIntegerField( zLONG lMinValue,
                                  zLONG lMaxValue,
                                  zLONG lInterval )
{
   m_lValue          = 0;
   m_lMinValue       = lMinValue;
   m_lMaxValue       = lMaxValue;
   m_lInterval       = lInterval;
   if ( m_lMinValue < 0 )
      m_bAllowNegatives = TRUE;
   else
      m_bAllowNegatives = FALSE;

   m_bIsNegative = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
CGCIntegerField&
CGCIntegerField::operator=( const long Val )
{
   _ASSERTE( Val >= m_lMinValue );
   _ASSERTE( Val <= m_lMaxValue );

   if ( (Val < m_lMinValue) || (Val > m_lMaxValue) )
      return( *this );

   m_lValue = Val;

   if ( m_lValue == 0 )
      m_bIsNegative = FALSE;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling "="( ) breaks the logical flow of the
   // data entry process.
   m_csValue.Empty( );
   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator>( const CGCIntegerField& Val )
{
   return( m_lValue > Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator>( const long Val )
{
   return( m_lValue > Val );
}

/////////////////////////////////////////////////////////////////////////////

zBOOL
CGCIntegerField::operator>( const double& Val ) const
{
   return( m_lValue > Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator>=( const CGCIntegerField& Val )
{
   return( m_lValue >= Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator>=( const long Val )
{
   return( m_lValue >= Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator>=( const double& Val ) const
{
   return( m_lValue >= Val );
}

/////////////////////////////////////////////////////////////////////////////
CGCIntegerField&
CGCIntegerField::operator-( )
{
   _ASSERTE( m_lMinValue < 0 );
   if ( m_lMinValue >= 0 )
      return( *this );  // do not change because the minimum range
                       // doesn't allow negatives.

   m_lValue *= -1;
   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator<( const CGCIntegerField& Val )
{
   return( m_lValue < Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator<( const long Val )
{
   return( m_lValue < Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator<( const double& Val ) const
{
   return( m_lValue < Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator<=( const CGCIntegerField& Val )
{
   return( m_lValue <= Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator<=( const long Val )
{
   return( m_lValue <= Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator<=( const double& Val ) const
{
   return( m_lValue <= Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator==( const CGCIntegerField& Val )
{
   return( m_lValue == Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator==( const long Val )
{
   return( m_lValue == Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::operator==( const double& Val ) const
{
   return( m_lValue == Val );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCIntegerField::Increment( )
{
   m_lValue += m_lInterval;  // increase the value by the specified Interval

   if ( m_lValue > m_lMaxValue )
      m_lValue = m_lMinValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Increment( ) breaks the logical flow of
   // the data entry process.
   m_csValue.Empty( );
}

/////////////////////////////////////////////////////////////////////////////
zLONG
CGCIntegerField::Increment( const zLONG lInterval )
{
   m_lValue += lInterval;  // increase the value by the specified Interval

   if ( m_lValue > m_lMaxValue )
      m_lValue = m_lMinValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Increment( ) breaks the logical flow of
   // the data entry process.
   m_csValue.Empty( );
   return( m_lValue );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCIntegerField::Decrement( )
{
   m_lValue -= m_lInterval; // decrement the number by the specified Interval

   if ( m_lValue < m_lMinValue )
      m_lValue = m_lMaxValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Decrement( ) breaks the logical flow of
   // the data entry process.
   m_csValue.Empty( );
}

/////////////////////////////////////////////////////////////////////////////
zLONG
CGCIntegerField::Decrement( const zLONG lInterval )
{
   m_lValue -= lInterval;  // decrement the number by the specified Interval

   if ( m_lValue < m_lMinValue )
      m_lValue = m_lMaxValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Decrement( ) breaks the logical flow of
   // the data entry process.
   m_csValue.Empty( );

   return( m_lValue );
}

/////////////////////////////////////////////////////////////////////////////
double
CGCIntegerField::GetValue( ) const
{
   return( double( m_lValue ) );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCIntegerField::GetValue( long& Val ) const
{
   Val = m_lValue;
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCIntegerField::Construct( const UINT nChar )
{
   if ( (nChar < 48) || (nChar > 57) )
   {
      if ( (nChar == 45 /* '-' */) && m_bAllowNegatives )
      {
         // A '-' key was selected. Toggle sign of current value and return.

         m_bIsNegative = !m_bIsNegative;
         if ( m_bAllowNegatives )
         {
            m_lValue *= -1;
            return( TRUE );
         }
      }
      else
         return( FALSE );
   }

   // Append the newly entered character to the current string representation
   // of the value.
   m_csValue += char( nChar );

   m_lValue = atol( m_csValue );

   if ( m_lValue > m_lMaxValue )
   {
      // If the newly entered character specifies a value that is larger than
      // the allowed maximum, throw away everything entered before and start
      // a new string representation of the number using the newly entered
      // character.
      m_csValue.Empty( );
      m_csValue += zCHAR ( nChar );
   }

   m_lValue = atol( m_csValue );

   // Make sure that the sign is correct after throwing away ( possibly ) the
   // original value.
   if ( m_bIsNegative )
      m_lValue *= -1;

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
#if 0  // DKS
zBOOL
IsIntegerField( CGCFieldBase *ptr )
{
   return( dynamic_cast<CGCIntegerField*>( ptr ) != 0 );
}
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//             Class Implementation for support class CGCDoubleField
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef GCMFESUPP_DBL


CGCDoubleField::CGCDoubleField( double MinValue,
                                double MaxValue,
                                UINT MaxPrecision,
                                double Interval )
{
   m_dValue          = 0.0;
   m_MinValue        = MinValue;
   m_MaxValue        = MaxValue;
   m_uiMaxPrecision  = MaxPrecision;
   m_dInterval       = Interval;

   if ( m_MinValue < 0.0 )
      m_bAllowNegatives = TRUE;
   else
      m_bAllowNegatives = FALSE;

   m_bIsNegative = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
CGCDoubleField&
CGCDoubleField::operator=( const double Val )
{
   _ASSERTE( Val >= m_MinValue );
   _ASSERTE( Val <= m_MaxValue );

   if ( (Val < m_MinValue) || (Val > m_MaxValue) )
      return( *this );

   m_dValue = Val;
   RoundNTrunc( );   // Throw away double-precision lack of precision. :- )

   if ( m_dValue == 0.0 )
      m_bIsNegative = FALSE;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling "="( ) breaks the logical flow of the
   // data entry process.
   m_csValue.Empty( );
   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
CGCDoubleField&
CGCDoubleField::operator-( )
{
   _ASSERTE( m_MinValue < 0.0 );
   if ( m_MinValue >= 0.0 )
      return( *this );   // do not change because the minimum range
                         // doesn't allow negatives.

   m_dValue *= -1.0;
   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator>( const CGCDoubleField& Val )
{
   return( m_dValue > Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator>( const double& Val ) const
{
   return( m_dValue > Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator>=( const CGCDoubleField& Val )
{
   return( m_dValue >= Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator>=( const double& Val ) const
{
   return( m_dValue >= Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator<( const CGCDoubleField& Val )
{
   return( m_dValue < Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator<( const double& Val ) const
{
   return( m_dValue < Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator<=( const CGCDoubleField& Val )
{
   return( m_dValue <= Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator<=( const double& Val ) const
{
   return( m_dValue <= Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator==( const CGCDoubleField& Val )
{
   return( m_dValue == Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::operator==( const double& Val ) const
{
   return( m_dValue == Val );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCDoubleField::Increment( )
{
   m_dValue += m_dInterval;
   RoundNTrunc( );   // Throw away double-precision lack of precision. :- )

   if ( m_dValue > m_MaxValue )
      m_dValue = m_MinValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Increment breaks the logical flow of the
   // data entry process.
   m_csValue.Empty( );
}

/////////////////////////////////////////////////////////////////////////////
double
CGCDoubleField::Increment( const double Interval )
{
   m_dValue += Interval;
   RoundNTrunc( );   // Throw away double-precision lack of precision. :- )


   if ( m_dValue > m_MaxValue )
      m_dValue = m_MinValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Increment( ) breaks the logical flow of
   // the data entry process.
   m_csValue.Empty( );
   return( m_dValue );
}

/////////////////////////////////////////////////////////////////////////////
double
CGCDoubleField::Decrement( const double Interval )
{
   m_dValue -= Interval;
   RoundNTrunc( );   // Throw away double-precision lack of precision. :- )

   if ( m_dValue < m_MinValue ) m_dValue = m_MaxValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Decrement breaks the logical flow of the
   // data entry process.
   m_csValue.Empty( );
   return( m_dValue );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCDoubleField::Decrement( )
{
   m_dValue -= m_dInterval;
   RoundNTrunc( );   // Throw away double-precision lack of precision. :- )
   if ( m_dValue < m_MinValue )
      m_dValue = m_MaxValue;

   // My design is to throw away any previous operator entered numbers when
   // calling this method.  Calling Decrement( ) breaks the logical flow of
   // the data entry process.
   m_csValue.Empty( );
}

/////////////////////////////////////////////////////////////////////////////
double
CGCDoubleField::GetValue( ) const
{
   return( m_dValue );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCDoubleField::GetValue( double& Val ) const
{
   Val = m_dValue;
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCDoubleField::Construct( const UINT nChar )
{

   if ( ( (nChar < 48) || (nChar > 57) ) && (nChar != 46 /* '.' */) )
   {
      if ( (nChar == 45 /* '-' */) && m_bAllowNegatives )
      {
         m_bIsNegative = !m_bIsNegative;
         if ( m_bAllowNegatives )
         {
            m_dValue *= -1.0;
            return( TRUE );
         }
      }
      else
         return( FALSE );
   }

   // If a decimal pt is already in the string representation, and nChar
   // is a decimal pt, then ignore it.
   if ( (m_csValue.Find( ".",0 ) >= 0) && (nChar == '.') )
      return( FALSE );  // Unchanged!

   m_csValue += zCHAR ( nChar );
   m_dValue = atof( m_csValue );
   if ( m_dValue > m_MaxValue )
   {
      m_csValue.Empty( );
      m_csValue += char( nChar );
   }
   else
   {
      // Make sure that we have not exceeded the allowable max precision.
      zSHORT pos = m_csValue.Find( ".",0 );
      if ( pos >= 0 )
      {
         if ( (m_csValue.GetLength( ) - (pos + 1)) > int( m_uiMaxPrecision ) )
         {
            m_csValue.Empty( );
            m_csValue += char( nChar );
         }
      }
   }

   m_dValue = atof( m_csValue );
   if ( m_bIsNegative )
      m_dValue *= -1.0;

   RoundNTrunc( );   // Throw away double-precision lack of precision. :- )
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCDoubleField::RoundNTrunc( )
{
   double factor = (pow( 10.0, (int) m_uiMaxPrecision ));
   m_dValue = m_dValue * factor + 0.5;
   m_dValue = floor( m_dValue );
   m_dValue /= factor;
}

/////////////////////////////////////////////////////////////////////////////
#if 0  // DKS
zBOOL
IsDoubleField( CGCFieldBase *ptr )
{
   return( dynamic_cast<CGCDoubleField *>( ptr ) != 0 );
}
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//           Class Implementation for support class CGCTextField
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef GCMFESUPP_TXT

CGCTextField::CGCTextField( UINT MaxCharacters,
                            CString UnallowedCharacters )
{
   m_uiMaxCharacters = MaxCharacters;
   m_csUnallowed = UnallowedCharacters;
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::Construct( const UINT nChar )
{
   // Handle backspace to remove the last character from the string.
   if ( nChar == VK_BACK )
   {
      m_csValue = m_csValue.Left( m_csValue.GetLength( ) - 1 );
      return( TRUE );
   }

   // Make sure that the input is allowed to be added to the string.
   if ( (!m_csUnallowed.IsEmpty( )) && m_csUnallowed.Find( char( nChar ) ) )
      return( FALSE );  // Unchanged!

   // If the string is already at its maximum length, we will clear it and
   // start a new string.
   if ( m_csValue.GetLength( ) == int( m_uiMaxCharacters ) )
      m_csValue.Empty( );

   // Append the new character.
   m_csValue += char( nChar );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
CGCTextField&
CGCTextField::operator+( const CString& Val )
{
   _ASSERTE( m_csValue.GetLength( ) < int( m_uiMaxCharacters ) );

   m_csValue += Val;     // Concatenate.
   m_csValue = m_csValue.Left( m_uiMaxCharacters ); // truncate excess characters
   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
CGCTextField&
CGCTextField::operator=( const CString& Val )
{
   // My design is to throw away any previous operator entered characters
   // when calling this method.  Calling "="( ) breaks the logical flow of
   // the data entry process.  So, I just replace any previous contents
   // with the newly specified string, truncating it to the maximum allowed
   // number of characters.
   m_csValue = Val.Left( m_uiMaxCharacters );
   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
char
CGCTextField::operator[]( const UINT ui ) const
{
   _ASSERTE( int( ui ) <= m_csValue.GetLength( ) );
   if ( int( ui ) > m_csValue.GetLength( ) )
      return( 0 );

   return( m_csValue.GetAt( ui ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::operator>( const CGCTextField& Val )
{
   return( m_csValue > Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::operator>( const CString& Val )
{
   return( m_csValue > Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::operator<( const CGCTextField& Val )
{
   return( m_csValue < Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::operator<( const CString& Val )
{
   return( m_csValue < Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::operator==( const CGCTextField& Val )
{
   return( m_csValue == Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCTextField::operator==( const CString& Val )
{
   return( m_csValue == Val );
}

/////////////////////////////////////////////////////////////////////////////
CString
CGCTextField::GetValue( ) const
{
   return m_csValue;
}

/////////////////////////////////////////////////////////////////////////////
void
CGCTextField::Reset( )
{
   m_csValue.Empty( );
}

/////////////////////////////////////////////////////////////////////////////
#if 0  // DKS
zBOOL
IsTextField( CGCFieldBase *ptr )
{
   return( dynamic_cast<CGCTextField *>( ptr ) != 0 );
}
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//           Class Implementation for support class CGCStringListField
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef GCMFESUPP_TLS

CGCStringListField::CGCStringListField( )
{
   m_lIndex = -1;                 // Currently, none is selected because the list is empty.
}

/////////////////////////////////////////////////////////////////////////////
CGCStringListField::~CGCStringListField( )
{
}

/////////////////////////////////////////////////////////////////////////////
CString
CGCStringListField::Increment( )
{
   // List is empty.  Return null string for selected item.
   if ( m_List.size( ) == 0 )
      return( "" );

   // Select the next item down in the list.
   m_lIndex += 1;

   // Loop around to the beginning of the list if we are at the end of the list
   // when this method is called.
   if ( m_lIndex > (long( m_List.size( ) ) - 1) )
      m_lIndex = 0;

   // Return the string representing the newly-selected item in the list.
   return( m_List[ m_lIndex ] );
}

/////////////////////////////////////////////////////////////////////////////
CString
CGCStringListField::Decrement( )
{
   // List is empty.  Return null string for selected item.
   if ( m_List.size( ) == 0 )
      return( "" );

   // Select the previous item down in the list.
   m_lIndex -= 1;

   // Loop around to the end of the list if we are at the beginning of the list
   // when this method is called.
   if ( m_lIndex < 0 )
      m_lIndex = m_List.size( ) - 1;

   // Return the string representing the newly-selected item in the list.
   return( m_List[ m_lIndex ] );
}

/////////////////////////////////////////////////////////////////////////////
void
CGCStringListField::Clear( )
{
   // Throw away all items in the list.
   m_List.clear( );
   m_lIndex = -1;
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::Construct( const UINT nChar )
{
   if ( m_List.size( ) == 0 )
      return( FALSE );

   // Add the new character to the characters already entered ( if any ).
   m_InputString += char( nChar );

   CString tmp;
   zSHORT k;

   // Search for an item in the list that starts with the characters entered
   // so far...
   for ( k = 0; k <= (int( m_List.size( )) - 1); k++ )
   {
      tmp = m_List[ k ].Left( m_InputString.GetLength( ) );

      if ( tmp.CompareNoCase( m_InputString ) == 0 )
      {
         m_lIndex = k;         // Found it, store the index.
         return( TRUE );
      }
   }

   // The current input string was not found.  In case the operator has
   // entered a valid first character for another selection, we will reconstruct
   // the input string and search again.
   m_InputString.Empty( );
   m_InputString += zCHAR ( nChar );

   for ( k = 0; k <= (int( m_List.size( )) - 1); k++ )
   {
      tmp = m_List[ k ].Left( m_InputString.GetLength( ) );

      if ( tmp.CompareNoCase( m_InputString ) == 0 )
      {
         m_lIndex = k;         // Found it, store the index.
         return( TRUE );
      }
   }

   // Still not found, throw away everything.

   m_InputString.Empty( );
   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
CString
CGCStringListField::operator+( const CString& NewString )
{
   if ( m_lIndex < 0 )
      m_lIndex = 0;     // Keep selected item set at the first item in list.

   m_List.push_back( NewString );   // Add the specified string to the list.

   return( m_List[ m_lIndex ] );
}

/////////////////////////////////////////////////////////////////////////////
CGCStringListField&
CGCStringListField::operator=( const CString& Val )
{

   // I will not assert that Val is non-null to allow client to assign
   // a null string to the field if desired.

   // When adding a new entry to the list, this logically breaks the data entry
   // process.  So, I throw away any previously entered characters.
   m_InputString.Empty( );

   // If the list is empty, then we can set the field to the value specified in Val.
   if ( m_List.size( ) <= 0 )
      return( *this );

   zSHORT k;

   // Search for the item in the list that matches the specified value completely.
   for ( k = 0; k <= ( int( m_List.size( ) ) - 1 ); k++ )
   {
      if ( m_List[ k ] == Val )
      {
         m_lIndex = k;      // Set the selection to the specified value.
         return( *this );
      }
   }

   // Notify when in debug build that the input Val is not one of the
   // previously defined list of values.
   _ASSERT( !"Input Val not in range of legal values for this control." );

   return( *this );  // Return current setting unchanged.
}

/////////////////////////////////////////////////////////////////////////////
CString
CGCStringListField::operator[ ]( const UINT ui ) const
{
   if ( ui > m_List.size( ) )
      return( "" );

   return( m_List[ ui ] );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::operator>( const CGCStringListField& Val )
{
   _ASSERTE( m_lIndex >= 0 );
   return( m_List[ m_lIndex ] > Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::operator>( const CString& Val )
{
   _ASSERTE( m_lIndex >= 0 );
   return( m_List[ m_lIndex ] > Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::operator<( const CGCStringListField& Val )
{
   _ASSERTE( m_lIndex >= 0 );
   return( m_List[ m_lIndex ] < Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::operator<( const CString& Val )
{
   _ASSERTE( m_lIndex >= 0 );
   return( m_List[ m_lIndex ] < Val );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::operator==( const CGCStringListField& Val )
{
   _ASSERTE( m_lIndex >= 0 );
   return( m_List[ m_lIndex ] == Val.GetValue( ) );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::operator==( const CString& Val )
{
   _ASSERTE( m_lIndex >= 0 );
   return( m_List[ m_lIndex ] == Val );
}

/////////////////////////////////////////////////////////////////////////////
CString
CGCStringListField::GetValue( ) const
{
   if ( m_List.size( ) > 0 )
      return( m_List[ m_lIndex ] );     // Return the currently selected item.
   else
      return( "" );                 // Return null string.
}

/////////////////////////////////////////////////////////////////////////////
zLONG
CGCStringListField::GetEntryPos( const CString& Str )
{
   long Result = -1;
   if ( m_List.size( ) == 0 )
      return( Result );

   zSHORT k = 0;

   for ( k = 0; (k < (zSHORT) m_List.size( )) && (Result < 0); k++ )
   {
      if ( m_List[ k ].CompareNoCase( Str ) )
         Result = k;
   }

   return( Result );
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
CGCStringListField::DeleteEntry( const UINT Index )
{
   BOOL Result = FALSE;
   if ( (m_List.size( ) == 0) || (Index > (m_List.size( ) - 1)) )
      return( FALSE );

   zSHORT k = 0;
   if ( Index == m_lIndex )
      Result = TRUE; // The item being deleted is the one currently selected.

   m_List.erase( m_List.begin( ) + Index );

   // Adjust m_lIndex if needed.
   if ( m_List.size( ) == 0 )
      m_lIndex = -1;  // List is now empty.
   else
   if ( Index <= (UINT) m_lIndex )
   {
      // The item we deleted came before or was the item currently selected.
      // We need to adjust m_lIndex to the new offset.  However, we only need to
      // do this if m_lIndex was not pting to the first element ( 0 ).  If
      // it was pting to the first element ( 0 ), then we can leave it unchanged
      // because we deleted the first one and the new first element is still index 0.
      if ( m_lIndex > 0 )
         m_lIndex -= 1;
   }

   return( Result );
}

/////////////////////////////////////////////////////////////////////////////
#if 0  // DKS
zBOOL
IsStringListField( CGCFieldBase *ptr )
{
   return( dynamic_cast<CGCStringListField *>( ptr ) != 0 );
}
#endif

#endif

#endif
