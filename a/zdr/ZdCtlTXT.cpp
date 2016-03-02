/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctltxt.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Text (static) control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.10.16    DKS
//    Fixed default text for "color picker" combobox.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.05.21    DKS    Z10
//    Honor text and background color settings.
//
// 2001.05.19    BL     WEB
//    Bugfix for not mapped Textfields.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2000.03.17    TMV    WEB
//    Validate parent window's handle prior to call Create/CreateEx.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.03.10  DKS
//    Altered the TranslateColor function to remove name conflict.
//
// 1999.01.21    DKS
//    Text enhancement completed Escapement implementation.
//
// 1998.10.14    DKS
//    Text on Tab Ctrl colored in PostBuild, switch no longer loses color
//    (TB 298).
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlTxt.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZText, CStatic )

BEGIN_MESSAGE_MAP( ZText, CStatic )
   ON_WM_CREATE( )
   ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_PAINT( )
   ON_WM_MOUSEMOVE( )

   ON_WM_ENABLE( )
   ON_WM_ERASEBKGND( )
   ON_WM_SETCURSOR( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_TIMER( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

// ZText - ctor
ZText::ZText( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
       CStatic( ),
       ZMapAct( pZSubtask,             // base class ctor
                pzmaComposite,
                pWndParent,
                this,
                vDialog,
                nOffsetX,
                nOffsetY,
                pCtrlDef,
                "Label" )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "Line" ) == 0 )
      TraceLineS( "ZText::ctor ", *m_pzsTag );
#endif

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style &= ~WS_BORDER;
   else
      Attr.StyleEx = WS_EX_CLIENTEDGE;

   Attr.Style &= ~(SS_RIGHT | SS_CENTER);
   if ( pCtrlDef->Subtype & zCONTROL_RIGHTJUSTIFY )
      Attr.Style |= SS_RIGHT;
   else
   if ( pCtrlDef->Subtype & zCONTROL_CENTERJUSTIFY )
      Attr.Style |= SS_CENTER;

   Attr.Style &= ~WS_TABSTOP;
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;

   m_fTextFlags = 0;
   m_lSize  = 0;
// m_lBorderStyle = 0;
// m_lBorderColor = 0;
   m_lEscapement = 0;
   m_lType = 0;
   m_szFaceName[ 0 ] = 0;

   if ( m_pzsVName )
   {
      SetText( "" );
   }
   else
   {
      CString cs( *m_pzsText );
      zPCHAR pchAmpersand = (zPCHAR) zstrchr( cs, '&' );
      while ( pchAmpersand )
      {
         pchAmpersand++;
         if ( *pchAmpersand == '&' )
            pchAmpersand++;
         else
         {
            m_chQuickKey = toupper( *pchAmpersand );
            break;
         }

         pchAmpersand = (zPCHAR) zstrchr( pchAmpersand, '&' );
      }

      SetText( cs );
   }

   m_clrBackColor = ::GetSysColor( COLOR_3DFACE );
   m_clrTextForeColor = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrTextBackColor = ::GetSysColor( COLOR_3DFACE );
   m_clrHighLight = ::GetSysColor( COLOR_HIGHLIGHT );
   m_hBackBrush = ::CreateSolidBrush( m_clrBackColor );
   m_bForceNoOwnerDraw = FALSE;
   m_bUseDefaultBackColor = TRUE;
   m_bUseDefaultTextBackColor = TRUE;
   m_bUseDefaultTextColor = TRUE;
   m_bLink = FALSE;
   m_bUseBevelLine = FALSE;
   m_bUseHighLighting = FALSE;
   m_bVerticalText = FALSE;
   m_bInside = FALSE;
   m_bTextFlash = FALSE;
   m_bBackFlash = FALSE;
   m_bFlashTextState = FALSE;
   m_bFlashBackState = FALSE;
   m_bUseToolTip = FALSE;
   m_bUseHighLightingOnlyWithToolTip = FALSE;
   m_bBlockMessage = FALSE;
   m_bLowerCase = FALSE;
   m_bUpperCase = FALSE;
   m_bStretchBitmap = FALSE;
   m_bFixTopLeft = FALSE;
   m_bRedrawLocal = TRUE;
   m_hCursor = 0;
// m_hCursorOld = ::LoadCursor( AfxGetInstanceHandle( ), IDC_ARROW );
   m_hBackImage = 0;
   m_ulAlignment = HORZ_LEFT | VERT_TOP;
   m_ulAngle = 0; // 0 degrees
// m_hEmptyBitmap1 = 0;
// m_hEmptyBitmap2 = 0;

   LABEL_DATA ld;
   zULONG ulLth = sizeof( ld );

   zmemset( &ld, 0, sizeof( ld ) );
   GetBlobFromAttribute( &ld, &ulLth, vDialog, "Ctrl", "CtrlBOI" );
//::MessageBox( 0, "vCtrl|Control|CtrlBOI", "Zeidon Error", MB_OK );

   if ( ulLth >= 4 + 2 * sizeof( zLONG ) )
   {
      if ( ld.chBold )
         m_fTextFlags |= zTEXT_BOLD;

      if ( ld.chItalic )
         m_fTextFlags |= zTEXT_ITALIC;

      if ( ld.chStrikeout )
         m_fTextFlags |= zTEXT_STRIKEOUT;

      if ( ld.chUnderline )
         m_fTextFlags |= zTEXT_UNDERLINE;

      m_lSize = ld.lPointSize;

      COLORREF clr;

      if ( ld.lTextColor & 0xFF000000 )
      {
         if ( ld.lTextColor == 0xFF000000 )
            clr = 0;
         else
            clr = ::GetSysColor( ld.lTextColor );
      }
      else
         clr = ld.lTextColor;

      if ( clr )
      {
         m_clrTextForeColor = clr;
         m_bUseDefaultTextColor = FALSE;
      }

      if ( ulLth >= 8 + (6 * sizeof( zLONG )) )
      {
         m_lType = ld.lType;
         Attr.Style &= ~(SS_LEFT | SS_CENTER | SS_RIGHT);

         if ( ld.chBottomRight & 1 )
         {
            Attr.Style |= SS_RIGHT;
            m_ulAlignment = HORZ_RIGHT;
         }
         else
         if ( ld.chCenter & 1 )
         {
            Attr.Style |= SS_CENTER;
            m_ulAlignment = HORZ_CENTER;
         }
         else
      // if ( ld.chTopLeft & 1 )
         {
            Attr.Style |= SS_LEFT;
            m_ulAlignment = HORZ_LEFT;
         }

         if ( ld.chBottomRight & 2 )
            m_ulAlignment |= VERT_BOTTOM;
         else
         if ( ld.chCenter & 2 )
            m_ulAlignment |= VERT_CENTER;
         else
      // if ( ld.chTopLeft & 2 )
            m_ulAlignment |= VERT_TOP;

         m_lEscapement = ld.lEscapement;
         strcpy_s( m_szFaceName, zsizeof( m_szFaceName ), ld.szFontName );

         while ( m_lEscapement < 0 )
            m_lEscapement += 3600;

         while ( m_lEscapement >= 3600 )
            m_lEscapement -= 3600;

      // SetAngle( m_lEscapement );
         m_ulAngle = m_lEscapement / 10;

         if ( ulLth == sizeof( ld ) )
         {
            Attr.StyleEx &= ~WS_EX_CLIENTEDGE;
            if ( ld.lBackgroundColor & 0x00FFFFFF )
            {
               m_clrBackColor = ld.lBackgroundColor;
               m_bUseDefaultBackColor = FALSE;
               DeleteObject( m_hBackBrush );
               m_hBackBrush = ::CreateSolidBrush( m_clrBackColor );
            }

            if ( ld.lTextBkColor & 0x00FFFFFF )
            {
               m_clrTextBackColor = ld.lTextBkColor;
               m_bUseDefaultTextBackColor = FALSE;
            }

            if ( ld.lHighlightColor & 0x00FFFFFF )
               m_clrHighLight = ld.lHighlightColor;

            m_bLink = ld.chHyperText;
            m_bUseBevelLine = ld.chBeveledLine;
            m_bVerticalText = ld.chVerticalText;
            if ( (ld.chUseToolTip & 1) && ld.szToolTip[ 0 ] )
            {
               SetToolTipText( ld.szToolTip );
               SetToolTipPosition( 1 );
               UseToolTip( TRUE );
               if ( ld.chUseToolTip & 2 )
               {
                  m_ToolTipCtrl.ShowOnlyOnMouseClickDown( TRUE );
                  if ( ld.chUseToolTip & 4 )
                     m_ToolTipCtrl.UseLeftMouseButton( TRUE );
               }
            }

            if ( ld.chHighlight & 1 )
               m_bUseHighLighting = ld.chHighlight;

            if ( ld.chHighlight & 2 )
               m_bUseHighLightingOnlyWithToolTip = TRUE;

            if ( ld.chBorder )
            {
               if ( ld.chBorder & 1 )
                  Attr.Style |= WS_BORDER;

               if ( ld.chBorder & 2 )
                  Attr.StyleEx = WS_EX_STATICEDGE;
               else
                  Attr.StyleEx = WS_EX_CLIENTEDGE;
            }
         }
      }
   }

   m_clrSaveTextColor = m_clrTextForeColor;
   m_clrText = fnTranslateColor( m_clrTextForeColor );
   if ( m_lType > 1 )
   {
      mDeleteInit( m_pFont );
      m_pFont = new CFont;
      ::GetObject( (HFONT) GetStockObject( DEFAULT_GUI_FONT ),
                                           sizeof( m_lf ), &m_lf );
      m_pFont->CreateFontIndirect( &m_lf );
   }

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZText::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZText::CreateZ ", *m_pzsTag );
#endif

   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   SetZCtrlText( this, *m_pzsText );
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( Attr.StyleEx, "STATIC", *m_pzsText,
                // (Attr.Style & ~WS_BORDER),
                   Attr.Style,
                   Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZText::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZText::OnCreate ", *m_pzsTag );
   TraceLineX( "ZText::OnCreate colorText ", (zLONG) m_clrText );
#endif

   int nRC = CStatic::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

#if 0
   BOOL CreateFont( int nHeight, int nWidth,
                    int nEscapement, int nOrientation,
                    int nWeight, BYTE bItalic,
                    BYTE bUnderline, BYTE cStrikeOut,
                    BYTE nCharSet, BYTE nOutPrecision,
                    BYTE nClipPrecision, BYTE nQuality,
                    BYTE nPitchAndFamily,
                    LPCTSTR lpszFacename );
#endif

   // Escapement - Specifies the angle, in tenths of degrees,
   // between the escapement vector and the x-axis of the device.
   // The escapement vector is parallel to the base line of a
   // row of text.

   if ( m_lType <= 1 &&
        m_fTextFlags || m_lSize || m_lEscapement || m_szFaceName[ 0 ] )
   {
      CClientDC dc( this );

      mDeleteInit( m_pFont );
      m_pFont = new CFont( );
      m_pFont->CreateFont( -((m_lSize * dc.GetDeviceCaps( LOGPIXELSY )) / 72),
                        // (mConvert16thInchToPixelY( m_lSize * 16 ) / 72),
                           0, m_lEscapement, m_lEscapement,
                           (m_fTextFlags & zTEXT_BOLD) ? FW_BOLD : FW_NORMAL,
                           (m_fTextFlags & zTEXT_ITALIC ? TRUE : FALSE),
                           (m_fTextFlags & zTEXT_UNDERLINE ? TRUE : FALSE),
                           (m_fTextFlags & zTEXT_STRIKEOUT ? TRUE : FALSE),
                           ANSI_CHARSET, OUT_TT_ONLY_PRECIS,
                           CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                           (BYTE)(m_lEscapement ? VARIABLE_PITCH | FF_SWISS :
                                                  DEFAULT_PITCH | FF_DONTCARE),
                           m_szFaceName[ 0 ] ? m_szFaceName :
                            m_pZSubtask->m_bUseMapDlgUnits ? "MS Sans Serif" :
                                                             "Arial" );
      SetFont( m_pFont, FALSE );
   }
   else
   if ( m_lType > 1 && m_pFont )
      SetFont( m_pFont, FALSE );
   else
      ZMapAct::SetFontOverride( );

   return( nRC );
}

zSHORT
ZText::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( "FolderName", *m_pzsTag ) == 0 )
      TraceLineS( "ZText::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW   vApp;

      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
         {
            zCHAR  szMap[ zMAP_SIZE ];
            zPCHAR pch;
            zULONG ulLth;

            GetAttributeDisplayLength( &ulLth, vApp, *m_pzsEName,
                                       *m_pzsAName, *m_pzsContext );
            if ( ulLth > zsizeof( szMap ) - 1 )
            {
               GetAddrForAttribute( &pch, vApp, *m_pzsEName, *m_pzsAName );
            }
            else
            {
               zSHORT nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING, zsizeof( szMap ) - 1, vApp,
                                                      *m_pzsEName, *m_pzsAName, *m_pzsContext,
                                                      m_pzsContext->IsEmpty( ) ?
                                                        zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT : zACCEPT_NULL_ENTITY );
               if ( nRC == zVAR_NULL )
                  pch = "";
               else
                  pch = szMap;
            }

            // We cannot use DT_NOPREFIX to handle the '&' as underline
            // problem since we normally call the default CStatic drawing
            // method to display text.  So we do the work ourselves.
            if ( zstrchr( pch, '&' ) )
            {
               zLONG lLth = (zstrlen( pch ) + 1) * 2;
               zPCHAR  pchBuffer = new char[ lLth ];
               zPCHAR  pchAmpersand = zstrchr( pch, '&' );
               zPCHAR  pchPos = pch;
               zLONG   lBufferLth = 0;

               while ( pchAmpersand )
               {
                  strcpy_s( pchBuffer + lBufferLth, lLth - lBufferLth, pchPos );
                  lBufferLth += pchAmpersand - pchPos + 1;
                  pchBuffer[ lBufferLth++ ] = '&';

                  pchPos = pchAmpersand + 1;
                  pchAmpersand = zstrchr( pchPos, '&' );
               }

               strcpy_s( pchBuffer + lBufferLth, lLth - lBufferLth, pchPos );
               SetZCtrlText( this, pchBuffer );
               mDeleteInitA( pchBuffer );
            }
            else
            {
               SetZCtrlText( this, pch );
            }

            return( 0 );
         }
      }

      SetZCtrlText( this, "" );  // things aren't quite ok
   }

   return( -1 );
}

zLONG
ZText::SetZCtrlProperty( zLONG   lPropertyType,
                         zULONG  ulNumericProperty,
                         zCPCHAR cpcStringProperty )
{
   switch ( lPropertyType )
   {
      case zCONTROL_PROPERTY_SUBCTRL_JUSTIFY:
      {
         zLONG lStyleAdd;
         zLONG lStyleRemove;

         Attr.Style &= ~(SS_RIGHT | SS_CENTER);
         if ( ulNumericProperty == 1 )
         {
            Attr.Style |= SS_RIGHT;  // right justify
            lStyleAdd = SS_RIGHT;
            lStyleRemove = SS_LEFT | SS_CENTER;
         }
         else
         if ( ulNumericProperty == 2 )
         {
            Attr.Style |= SS_CENTER; // center justify
            lStyleAdd = SS_CENTER;
            lStyleRemove = SS_LEFT | SS_RIGHT;
         }
         else
         {
            lStyleAdd = SS_LEFT;
            lStyleRemove = SS_CENTER | SS_RIGHT;
         }

         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            ModifyStyleEx( lStyleRemove, lStyleAdd );
            Invalidate( );
         }

         return( 0 );
      }
   }

   return( ZMapAct::SetZCtrlProperty( lPropertyType, ulNumericProperty, cpcStringProperty ) );
}

zLONG
ZText::SetZCtrlState( zLONG lStatusType,
                      zULONG ulValue )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZText::SetZCtrlState ", *m_pzsTag );
#endif

   if ( lStatusType >= 90 )
   {
      if ( lStatusType == 99 )
      {
         m_lEscapement = ulValue;
         while ( m_lEscapement < 0 )
            m_lEscapement += 3600;

         while ( m_lEscapement >= 3600 )
            m_lEscapement -= 3600;
      }
      else
      if ( lStatusType == 98 )
         m_lSize = ulValue;

      CClientDC dc( this );

      mDeleteInit( m_pFont );
      m_pFont = new CFont( );
      m_pFont->CreateFont( -((m_lSize * dc.GetDeviceCaps( LOGPIXELSY )) / 72),
                        // (mConvert16thInchToPixelY( m_lSize * 16 ) / 72),
                           0, m_lEscapement, m_lEscapement,
                           (m_fTextFlags & zTEXT_BOLD) ? FW_BOLD : FW_NORMAL,
                           (m_fTextFlags & zTEXT_ITALIC ? TRUE : FALSE),
                           (m_fTextFlags & zTEXT_UNDERLINE ? TRUE : FALSE),
                           (m_fTextFlags & zTEXT_STRIKEOUT ? TRUE : FALSE),
                           ANSI_CHARSET, OUT_TT_ONLY_PRECIS,
                           CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                           (BYTE)(m_lEscapement ? VARIABLE_PITCH | FF_SWISS : DEFAULT_PITCH | FF_DONTCARE),
                           m_szFaceName[ 0 ] ? m_szFaceName :
                            m_pZSubtask->m_bUseMapDlgUnits ? "MS Sans Serif" : "Arial" );
      SetFont( m_pFont, FALSE );
      if ( mIs_hWnd( m_hWnd ) )
         Invalidate( TRUE );

      return( 0 );
   }

   if ( lStatusType == zCONTROL_STATUS_RESET )
   {
      TrackMouse( TRUE, FALSE );
      m_bInside = FALSE;
      if ( m_bUseHighLighting )
         SetTextColor( m_clrSaveTextColor );

      return( 0 );
   }

   if ( lStatusType == zCONTROL_STATUS_ENABLED )
   {
      if ( ulValue )
      {
         m_ulMapActFlags |= zMAPACT_ENABLED;
         if ( mIs_hWnd( m_hWnd ) )
            SetDisabled( FALSE );
      }
      else
      {
         m_ulMapActFlags &= ~zMAPACT_ENABLED;
         if ( mIs_hWnd( m_hWnd ) )
            SetDisabled( TRUE );
      }

      return( 0 );
   }

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

void
ZText::ParentResize( ZMapAct *pParent,
                     CSize   *pCSize )
{
// if ( m_hEmptyBitmap1 )
// {
//    DeleteObject( m_hEmptyBitmap1 );
//    m_hEmptyBitmap1 = 0;
// }

// if ( m_hEmptyBitmap2 )
// {
//    DeleteObject( m_hEmptyBitmap2 );
//    m_hEmptyBitmap2 = 0;
// }

   ZMapAct::ParentResize( pParent, pCSize ); // default processing
}

/////////////////////////////////////////////////////////////////////////////
// To change the text color, call the SetTextColor member function with the
// desired red, green, and blue (RGB) values.
//
// To change the background color of a single-line edit control, set the brush
// handle in both the CTLCOLOR_EDIT and CTLCOLOR_MSGBOX message codes, and call
// the CDC::SetBkColor function in response to the CTLCOLOR_EDIT code.
//
// OnCtlColor will not be called for the list box of a drop-down combo box
// because the drop-down list box is actually a child of the combo box and not
// a child of the window. To change the color of the drop-down list box, create
// a CComboBox with an override of OnCtlColor that checks for CTLCOLOR_LISTBOX
// in the nCtlColor parameter. In this handler, the SetBkColor member function
// must be used to set the background color for the text.
//
// Note:  This member function is called by the framework to allow your
// application to handle a Windows message. The parameters passed to your
// function reflect the parameters received by the framework when the message
// was received.  If you call the base-class implementation of this function,
// that implementation will use the parameters originally passed with the
// message and not the parameters you supply to the function.
//
// Handling Reflected Messages: An Example of a Reusable control
//
// This simple example creates a reusable control called CYellowEdit.  The
// control works the same as a regular edit control except that it displays
// black text on a yellow background. It would be easy to add member functions
// that would allow the CYellowEdit control to display different colors.
//
// To try this example, do the following steps:
//
// 1. Create a new dialog box in an existing application. For more information
//    see dialog editor in the Developer Studio User's Guide.
//
//    You must have an application in which to develop the reusable control.
//    If you don't have an existing application to use, create a dialog-based
//    application using AppWizard.
//
// 2. With your project loaded into Developer Studio, use ClassWizard to
//    create a new class called CYellowEdit based on CEdit. Leave the "Add
//    to Component Gallery" box checked.
// 3. Add three member variables to your CYellowEdit class.  The first two
//    will be COLORREF variables to hold the text color and the background
//    color.  The third will be a CBrush object which will hold the brush for
//    painting the background.  The CBrush object allows you to create the
//    brush once, merely referencing it after that, and to destroy the brush
//    automatically when the CYellowEdit control is destroyed.
// 4. Initialize the member variables by writing the constructor as follows:
//
// CYellowEdit::CYellowEdit( )
// {
//    m_clrText = RGB( 0, 0, 0 );
//    m_clrBkgnd = RGB( 255, 255, 0 );
//    m_brBkgnd.CreateSolidBrush( m_clrBkgnd );
// }
//
//
// 5. Using ClassWizard, add a handler for the reflected WM_CTLCOLOR message
//    to your CYellowEdit class. Note that the equal sign in front of the
//    message name in the list of messages you can handle indicates that the
//    message is reflected. This is described in Defining a Message Handler
//    for a Reflected Message in the Visual C++ Programmer's Guide.
//
//    ClassWizard adds the following message-map macro and skeleton function
//    for you:
//
//    ON_WM_CTLCOLOR_REFLECT( )
//
//    // Note: other code will be in between....
//
//    HBRUSH CYellowEdit::CtlColor(CDC *pDC, UINT nCtlColor)
//    {
//       // TODO: Change any attributes of the DC here
//       // TODO: Return a non-NULL brush if the
//       //    parent's handler should not be called
//       return( NULL );
//    }
//
//
// 6. Replace the body of the function with the following code.  The code
//    specifies the text color, the text background color, and the background
//    color for rest of the control.
//
//    pDC->SetTextColor( m_clrText );     // text
//    pDC->SetBkColor( m_clrBkgnd );      // text bkgnd
//    return( m_brBkgnd );                // ctl bkgnd
//
// 7. Create an edit control in your dialog box, then attach it to a member
//    variable by double-clicking the edit control while holding a control
//    key down.  In the Add Member Variable dialog box, finish the variable
//    name and choose "Control" for the category, then "CYellowEdit" for the
//    variable type.  Don't forget to set the tab order in the dialog box.
//    Also, be sure to include the header file for the CYellowEdit control
//    in your dialog box's header file.
// 8. Build and run your application. The edit control will have a yellow
//    background.
// 9. You can now use Component Gallery to add your CYellowEdit control class
//    to other projects.
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH
ZText::CtlColor( CDC *pDC, UINT uCtlColor )
{
#if 0 //def DEBUG_ALL lots of messages
   CString cs;
   GetWindowText( cs );
   TraceLineS( "ZText::CtlColor Tag ", *m_pzsTag );
   TraceLineS( "ZText::CtlColor Text ", cs );
   TraceLineI( "ZText::CtlColor uCtlColor ", uCtlColor );
   TraceLineX( "ZText::CtlColor colorText ", (zLONG) m_clrText );
#endif

   if ( m_lType > 1 )
   {
      if ( m_bForceNoOwnerDraw )
      {
         if ( CTLCOLOR_STATIC == uCtlColor )
         {
            pDC->SelectObject( m_pFont );
            pDC->SetTextColor( m_bUseDefaultTextColor ? ::GetSysColor( COLOR_WINDOWTEXT ) : m_clrTextForeColor );
            pDC->SetBkMode( TRANSPARENT );
         }

         return( m_hBackBrush );
      }
      else
      {
         return( 0 );
      }
   }
   else
   {
      HBRUSH hBrush;

      // Return a non-NULL brush if the parent's handler should not be called.
      // Note that we have to call the Default handler prior to setting the
      // text color since the Default handler will set the text color to the
      // default text color.
      if ( m_pBrush )
      {
         hBrush = (HBRUSH) *m_pBrush;
         pDC->SetBkColor( m_clrBk );
      }
      else
         hBrush = (HBRUSH) Default( );

      if ( m_clrText )
         pDC->SetTextColor( m_clrText );

   // TraceLineX( "ZText::CtlColor return brush ", (zLONG) hBrush );
      return( hBrush );
   }
}

#include <math.h>

void
ZText::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   if ( zstrcmp( *m_pzsTag, "Line" ) == 0 )
      TraceLineS( "ZMapAct::OnPaint tag: ", *m_pzsTag );
#endif

   if ( m_lType <= 1 )
   {
      if ( m_pFont && m_lEscapement )
      {
         // At this point m_lEscapement is forced to be between 0 and 3600.
         CPaintDC dc( this );   // a paint DC calls Begin and End Paint
         CFont *pFontOld = dc.SelectObject( m_pFont );
         COLORREF colorOldText = dc.SetTextColor( m_clrText );
         COLORREF colorBk;
         CRect rectClient;
         zSHORT nQuadrant = (zSHORT) ((m_lEscapement - 1) / 900) + 1;

//       GetUpdateRect( rect, 1 );
//       CBrush brush( dc.GetBkColor( ) );
         colorBk = ::GetSysColor( COLOR_BTNFACE );
         CBrush brush( colorBk );
         COLORREF colorOldBk = dc.SetBkColor( colorBk );

         GetClientRect( rectClient );
         CRect rect( rectClient );
         dc.FillRect( rect, &brush );
      // dc.TextOut( rect.left, rect.top, m_csText );

         m_uFormat = DT_LEFT | DT_SINGLELINE | DT_EXPANDTABS;

         static double pi = 3.14159265358979323846;
         CSize  sizeExtent = dc.GetTabbedTextExtent( *m_pzsText, zstrlen( *m_pzsText ), 0, 0 );
         dc.LPtoDP( &sizeExtent );
         CSize  sizeAdjustedExtent;
         double dEscapeAngle;
         double dSin;
         double dCos;

//       if ( zstrcmp( *m_pzsText, "Vertical" ) == 0 )
//       {
//          TraceLineS( "Vertical: ", "Vertical" );
//          TraceSize( "   Extent", sizeExtent );
//          TraceRect( "     Rect", rect );
//          TraceLineI( "   Escape ", m_lEscapement );
//       }

         dEscapeAngle = (pi * m_lEscapement) / 1800;
         dSin = sin( dEscapeAngle );
         dCos = cos( dEscapeAngle );

         rect.top = 0;
         rect.left = 0;
         switch ( nQuadrant )
         {
            case 4:
            {
               m_uFormat |= DT_TOP;
               sizeAdjustedExtent.cx = (zLONG) ((sizeExtent.cx * dCos) + -(sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) ((sizeExtent.cy * dCos) + -(sizeExtent.cx * dSin * 5 / 4));
//             if ( zstrcmp( *m_pzsText, "Vertical" ) == 0 )
//               TraceSize( "  4Extent adjusted", sizeAdjustedExtent );

//             rect.top = (zLONG) (dCos * sizeExtent.cy);
               rect.left = (zLONG) -(dSin * sizeExtent.cy);
               rect.right = rect.left + sizeAdjustedExtent.cx;
               rect.bottom = rect.top + sizeAdjustedExtent.cy;
               break;
            }

            case 3:
            {
               m_uFormat |= DT_TOP;
               sizeAdjustedExtent.cx = (zLONG) (-(sizeExtent.cx * dCos) + -(sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) (-(sizeExtent.cy * dCos) + -(sizeExtent.cx * dSin * 5 / 4));
//             if ( zstrcmp( *m_pzsText, "Vertical" ) == 0 )
//               TraceSize( "  4Extent adjusted", sizeAdjustedExtent );

               rect.top = (zLONG) -(dCos * sizeExtent.cy);
               rect.left = 16 + sizeAdjustedExtent.cx;
               rect.right = rect.left + sizeAdjustedExtent.cx;
               rect.bottom = rect.top + sizeAdjustedExtent.cy;
               break;
            }

            case 2:
            {
               m_uFormat |= DT_BOTTOM;
               sizeAdjustedExtent.cx = (zLONG) (-(sizeExtent.cx * dCos) + (sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) (-(sizeExtent.cy * dCos) + (sizeExtent.cx * dSin * 5 / 4));
//             if ( zstrcmp( *m_pzsText, "Vertical" ) == 0 )
//                TraceSize( "  2Extent adjusted", sizeAdjustedExtent );

               rect.top = sizeExtent.cy;
               rect.left = (zLONG) ((sizeExtent.cy) + (sizeExtent.cy * dCos) - (dCos * sizeAdjustedExtent.cx));
               rect.right = sizeAdjustedExtent.cx;
               rect.bottom = (zLONG) ((rect.top + (12 * dSin)) - (sizeExtent.cy * dCos) + (sizeExtent.cx * dSin));
               break;
            }

         // case 1:  first quadrant
            default:
            {
               m_uFormat |= DT_BOTTOM;
               sizeAdjustedExtent.cx = (zLONG) ((sizeExtent.cx * dCos) + (sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) ((sizeExtent.cy * dCos) + (sizeExtent.cx * dSin * 5 / 4));
//             if ( zstrcmp( *m_pzsText, "Vertical" ) == 0 )
//                TraceSize( "  1Extent adjusted", sizeAdjustedExtent );

               rect.top = sizeExtent.cy;
//             rect.left = (zLONG) (dSin * sizeExtent.cy);
               rect.right = rect.left + sizeAdjustedExtent.cx;
               rect.bottom = (zLONG) ((rect.top + (12 * dSin)) + (sizeExtent.cx * dSin));
               break;
            }
         }

         if ( m_bCenter )
         {
            ;
         }
         else
         if ( m_bRight )
         {
            ;
         }
         else
      // if ( m_bLeft )
         {
            ;
         }

      // if ( zstrcmp( *m_pzsText, "Line" ) == 0 )
      //    TraceRect( "  Line Rect", rect );

         // We cannot use DT_NOPREFIX to handle the '&' as underline
         // problem since we normally call the default CStatic drawing
         // method to display text.  So we do the work ourselves.
         dc.DrawText( *m_pzsText, -1, rect, m_uFormat | DT_NOCLIP );
         dc.SetBkColor( colorOldBk );
         dc.SetTextColor( colorOldText );
         dc.SelectObject( pFontOld );
      }
      else
      {
         ZMapAct::SetFontOverride( );
         CStatic::OnPaint( );
      }
   }
   else
   {
      CPaintDC dc( this ); // device context for painting
      CRect    rectClient;
      CString  csText = m_bVerticalText ? MakeVerticalText( *m_pzsText ) :
                                          *m_pzsText;
      if ( m_bLowerCase )
         csText.MakeLower( );
      else
      if ( m_bUpperCase )
         csText.MakeUpper( );

      GetClientRect( &rectClient );

      // Get Horz & Vert Alignment.
      zULONG ulHorzAlignment;
      zULONG ulVertAlignment;

      if ( (m_ulAlignment & HORZ_CENTER) == HORZ_CENTER )
         ulHorzAlignment = DT_CENTER;
      else
      if ( (m_ulAlignment & HORZ_RIGHT) == HORZ_RIGHT )
         ulHorzAlignment = DT_RIGHT;
      else
         ulHorzAlignment = DT_LEFT;

      if ( (m_ulAlignment & VERT_CENTER) == VERT_CENTER )
         ulVertAlignment = DT_VCENTER;
      else
      if ( (m_ulAlignment & VERT_BOTTOM) == VERT_BOTTOM )
         ulVertAlignment = DT_BOTTOM;
      else
         ulVertAlignment = DT_TOP;

      // Set current font and background mode.
      CFont *pOldFont = dc.SelectObject( m_pFont );
      int   nOldMode = dc.SetBkMode( TRANSPARENT );

      // Init Text rotation class.
      ZTextRotator TextRotator( this, dc.GetSafeHdc( ), csText,
                                ulHorzAlignment, ulVertAlignment );

      TextRotator.SetTextColor( m_clrTextForeColor );
      TextRotator.SetDisabledText( (GetStyle( ) & WS_DISABLED) == WS_DISABLED );
      TextRotator.DrawBevelLine( m_bUseBevelLine );
      TextRotator.SetBackgroundColor( !m_bUseDefaultTextBackColor ?
                        m_clrTextBackColor : m_bUseDefaultBackColor ? ::GetSysColor( COLOR_3DFACE ) : m_clrBackColor );

      // Erase background.
      CBrush brush( ::GetSysColor( COLOR_3DFACE ) );

      if ( m_bUseDefaultBackColor == FALSE )
      {
         brush.DeleteObject( );
         brush.CreateSolidBrush( m_clrBackColor );
      }

      if ( m_bBackFlash && m_bFlashBackState )
      {
         COLORREF clr = m_bUseDefaultBackColor ? m_clrBackColor / 2 : ::GetSysColor( COLOR_3DFACE );
         brush.DeleteObject( );
         brush.CreateSolidBrush( clr );

         TextRotator.SetBackgroundColor( clr );
      }

      if ( m_hBackImage )
      {
         BITMAP   Bitmap;
         CDC      memDC;
         CBitmap  *pBackImageDC;

         GetObject( m_hBackImage, sizeof( Bitmap ), &Bitmap );
         memDC.CreateCompatibleDC( &dc );
         pBackImageDC = memDC.SelectObject( CBitmap::FromHandle( m_hBackImage ) );

         if ( m_bStretchBitmap )
         {
            dc.StretchBlt( 0, 0, rectClient.Width( ), rectClient.Height( ),
                           &memDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY );
         }
         else
         {
            int X = (rectClient.Width( ) - Bitmap.bmWidth) / 2;
            int Y = (rectClient.Height( ) - Bitmap.bmHeight) / 2;
            int W = rectClient.Width( );
            int H = rectClient.Height( );

            // The following lines are used if it is desirable to keep the top-left edge of the bitmap fixed.
            // Otherwise, the center of the bitmap remains "relatively fixed".
            if ( m_bFixTopLeft )
            {
               if ( X < 0 )
                  X = 0;

               if ( Y < 0 )
                  Y = 0;
            }

            if ( X > 0 )
            {
               CRect rectFill( 0, 0, X, H );
               dc.FillRect( rectFill, &brush );
               rectFill.left = X + Bitmap.bmWidth;
               rectFill.right = X + W;
               dc.FillRect( rectFill, &brush );
            }

            if ( Y > 0 )
            {
               CRect rectFill( 0, 0, W, Y );
               dc.FillRect( rectFill, &brush );
               rectFill.top = Y + Bitmap.bmHeight;
               rectFill.bottom = Y + H;
               dc.FillRect( rectFill, &brush );
            }

            if ( X < 0 )
               W -= X;

            if ( Y < 0 )
               H -= Y;

            dc.BitBlt( X, Y, W, H, &memDC, 0, 0, SRCCOPY );
         }

         memDC.SelectObject( pBackImageDC );
         memDC.DeleteDC( );
      }
      else
      {
        dc.FillRect( rectClient, &brush );
      }

      brush.DeleteObject( );

      // Rotate Text.
      zSHORT nMultiLine = IsMultiLineString( csText );
      zULONG ulAlignment = GetVertAlignment( );

   // if ( zstrcmp( *m_pzsTag, "Line" ) == 0 )
   //    TraceRect( "  Line Rect", rectClient );

      if ( m_hBackImage && m_ulAngle == 0 ) // &&
      //   (nMultiLine == 0 ||
      //    (nMultiLine && ulAlignment == VERT_TOP)) )
      {
         COLORREF clrOld = dc.SetTextColor( m_bUseDefaultTextColor ?
                                            ::GetSysColor( COLOR_WINDOWTEXT ) :
                                            m_clrTextForeColor );
         DrawAlignedText( &dc, csText, rectClient, m_ulAlignment );
         dc.SetTextColor( clrOld );
      }
      else
      {
         HDC hRotatedMemDC = TextRotator.RotateText( rectClient, m_ulAngle );
         if ( hRotatedMemDC )
         {
         // CRect rectRotated = TextRotator.GetRotatedRect( );

            // Draw Text.
            TextRotator.BitBltText( hRotatedMemDC, rectClient, m_ulAngle );

            if ( TextRotator.m_hRotatedBitmapMemDC )
            {
               HBITMAP hEmptyBitmap2 =
               (HBITMAP) ::SelectObject( hRotatedMemDC, TextRotator.m_hRotatedBitmapMemDC );
               ::DeleteObject( hEmptyBitmap2 ); // Delete temporary empty bitmap 2
               TextRotator.m_hRotatedBitmapMemDC = 0;
            }

            ::DeleteDC( hRotatedMemDC );
         }

      // TextRotator.m_rectRotated.SetRectEmpty( );
      // TextRotator.m_ulAngle = 0;
      }

      // Restore font and mode.
      dc.SelectObject( pOldFont );
      dc.SetBkMode( nOldMode );
   }
}

zBOOL
ZText::HandleQuickKey( zACCEL_DEF ad )
{
   if ( ad.fAlt && ad.uVK == m_chQuickKey )
   {
      ZMapAct *pzma = this;
      zULONG  ulIdx = m_pZSubtask->m_pZMIXCtrl->GetNodeIndex( pzma );
      if ( ulIdx > 0 )
      {
         pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( ulIdx + 1 );
         if ( pzma && pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) &&
              (pzma->m_ulMapActFlags & zMAPACT_VISIBLE) &&
              (pzma->m_ulMapActFlags & zMAPACT_ENABLED) &&
              (pzma->m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) )
         {
            pzma->m_pCtrl->SetFocus( );
         }
      }

      return( TRUE );
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZText::DrawAlignedText( CDC *pDC, zCPCHAR cpcText,
                        RECT Rect, zULONG ulAlignment )
{
   zULONG ulFormat = DT_EXPANDTABS;

   if ( (m_ulAlignment & HORZ_LEFT) == HORZ_LEFT  )
      ulFormat |= DT_LEFT;

   if ( (m_ulAlignment & HORZ_CENTER) == HORZ_CENTER )
      ulFormat |= DT_CENTER;

   if ( (m_ulAlignment & HORZ_RIGHT) == HORZ_RIGHT )
      ulFormat |= DT_RIGHT;

   zSHORT nMultiLine = IsMultiLineString( cpcText );
   zBOOL bIsVTop = ((m_ulAlignment & VERT_TOP) == VERT_TOP);
   zBOOL bIsVCenter = ((m_ulAlignment & VERT_CENTER) == VERT_CENTER);
   zBOOL bIsVBottom = ((m_ulAlignment & VERT_BOTTOM) == VERT_BOTTOM);
   CRect rect = Rect;

   if ( nMultiLine == 0 )
      ulFormat |= DT_SINGLELINE;
   else
   {
      CRect rectExtent = Rect;
      rectExtent.bottom = rectExtent.top + 10;
      pDC->DrawText( cpcText, -1, &rectExtent, ulFormat | DT_CALCRECT );
      zLONG lBottom = (Rect.bottom - rectExtent.bottom);
      if ( lBottom > 0 )
      {
         if ( bIsVCenter )
         {
            lBottom /= 2;
            rect.top += lBottom;
            rect.bottom = rectExtent.bottom + lBottom;
         }
         else
         if ( bIsVBottom )
         {
            rect.top = rect.bottom - (rectExtent.bottom - rectExtent.top);
         }
      }
   }

// if ( nMultiLine == 0 || (nMultiLineLine && bIsVTop) )
   {
   // if ( bIsVCenter )
   //    ulFormat |= DT_VCENTER;

   // if ( bIsVBottom )
   // {
   //    CSize size = pDC->GetTabbedTextExtent( cpcText, zstrlen( cpcText ), 0, 0 );
   //    rect.top += (Rect.bottom - Rect.top) - size.cy;
   // }

      pDC->DrawText( cpcText, -1, &rect, ulFormat );
      return;
   }

   // MultiLine Drawing (TOP or BOTTOM).
   pDC->DrawText( "**********", -1, &Rect, ulFormat );
}

int
ZText::GetWindowText( zPCHAR pchStringBuf, int nMaxCount )
{
   CString cs;

   GetWindowText( cs );
   pchStringBuf[ 0 ] = 0;
   strncat_s( pchStringBuf, nMaxCount, cs, nMaxCount - 1 );
   return( zstrlen( pchStringBuf ) );
}

void
ZText::GetWindowText( CString& rString )
{
   m_bBlockMessage = TRUE;
   CStatic::GetWindowText( rString );
   m_bBlockMessage = FALSE;
}

zSHORT
ZText::IsMultiLineString( zCPCHAR cpcText )
{
   zSHORT nCnt = 0;
   zPCHAR pch = (zPCHAR) cpcText;
   while ( (pch = zstrchr( pch, '\n' )) != 0 )
   {
      nCnt++;
      pch++;
   }

   return( nCnt );
}

CString
ZText::MakeVerticalText( zCPCHAR cpcText )
{
   CString csSrc, csTgt;

   csSrc = (cpcText ? cpcText : *m_pzsText);
   for ( int k = 0; k < csSrc.GetLength( ); k++ )
   {
      if ( k > 0 )
         csTgt += '\n';

      csTgt += csSrc[ k ];
   }

   return( csTgt );
}

zBOOL
ZText::ReconstructFont( )
{
   m_pFont->DeleteObject( );
   zBOOL bRC = m_pFont->CreateFontIndirect( &m_lf );
   RedrawWindowLocal( );
   return( bRC );
}

void
ZText::SetWindowText( zCPCHAR cpcString )
{
   m_bBlockMessage = TRUE;
   CStatic::SetWindowText( cpcString );
   m_bBlockMessage = FALSE;
}

void
ZText::TrackMouse( zBOOL bHover, zBOOL bLeave )
{
   TRACKMOUSEEVENT tme;

   tme.cbSize = sizeof( tme );
   tme.dwFlags = 0;
   tme.hwndTrack = GetSafeHwnd( );
   tme.dwHoverTime = 50;

   if ( bHover )
      tme.dwFlags |= TME_HOVER;

// if ( bHover == FALSE )
//    stTME.dwFlags |= TME_HOVER | TME_CANCEL;

   if ( bLeave )
      tme.dwFlags |= TME_LEAVE;

// if ( bLeave == FALSE )
//    stTME.dwFlags |= TME_LEAVE | TME_CANCEL;

   _TrackMouseEvent( &tme );
}

ZText&
ZText::SetBkColor( COLORREF clr )
{
   m_bUseDefaultBackColor = FALSE;
   m_clrBackColor = clr;

   if ( m_hBackBrush )
      DeleteObject( m_hBackBrush );

   m_hBackBrush = ::CreateSolidBrush( m_clrBackColor );
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetDefaultBkColor( )
{
   m_bUseDefaultBackColor = TRUE;
   m_clrBackColor = ::GetSysColor( COLOR_3DFACE );
   if ( m_hBackBrush )
      DeleteObject( m_hBackBrush );

   m_hBackBrush = ::CreateSolidBrush( m_clrBackColor );
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetDefaultTextBackColor( )
{
   m_bUseDefaultTextBackColor = TRUE;
   m_clrTextBackColor = ::GetSysColor( COLOR_3DFACE );
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetDefaultTextColor( )
{
   m_bUseDefaultTextColor = TRUE;
   m_clrTextForeColor = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrSaveTextColor = m_clrTextForeColor;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetHighLightColor( COLORREF clr )
{
   m_clrHighLight = clr;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetTextBackColor( COLORREF clr )
{
   m_bUseDefaultTextBackColor = FALSE;
   m_clrTextBackColor = clr;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetTextColor( COLORREF clr )
{
   m_bUseDefaultTextColor = FALSE;
   m_clrTextForeColor = clr;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetFontBold( zBOOL bBold )
{
   m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
   ReconstructFont( );
   return( *this );
}

ZText&
ZText::SetFontItalic( zBOOL bItalic )
{
   m_lf.lfItalic = bItalic;
   ReconstructFont( );
   return( *this );
}

ZText&
ZText::SetFontName( zCPCHAR cpcFaceName )
{
   strcpy_s( m_lf.lfFaceName, zsizeof( m_lf.lfFaceName ), cpcFaceName );
   ReconstructFont( );
   return( *this );
}

ZText&
ZText::SetFontSize( zLONG lSize )
{
   m_lf.lfHeight = lSize;
   ReconstructFont( );
   return( *this );
}

ZText&
ZText::SetFontStrikeout( zBOOL bStrikeout )
{
   m_lf.lfStrikeOut = bStrikeout;
   ReconstructFont( );
   return( *this );
}

ZText&
ZText::SetFontUnderline( zBOOL bUnderline )
{
   m_lf.lfUnderline = bUnderline;
   ReconstructFont( );
   return( *this );
}

ZText&
ZText::FlashBackground( zBOOL bActivate, zULONG ulTime )
{
   if ( m_bBackFlash )
      KillTimer( 2 );

   if ( bActivate )
      SetTimer( 2, ulTime, 0 );
   else
      InvalidateRect( 0, FALSE );

   m_bBackFlash = bActivate;
   return( *this );
}

ZText&
ZText::FlashText( zBOOL bActivate, zULONG ulTime )
{
   if ( m_bTextFlash )
   {
      SetWindowText( *m_pzsText );
      KillTimer( 1 );
   }

   if ( bActivate )
   {
      SetTimer( 1, ulTime, 0 );
   }
   else
      InvalidateRect( 0, FALSE );

   m_bTextFlash = bActivate;
   return( *this );
}

void
ZText::ForceNoOwnerDraw( zBOOL bForce )
{
   m_bForceNoOwnerDraw = bForce;
   RedrawWindowLocal( );
}

zBOOL
ZText::GetBorder( )
{
   return( ((GetStyle( ) & WS_BORDER) == WS_BORDER) ? TRUE : FALSE );
}

zULONG
ZText::GetHorzAlignment( )
{
   if ( (m_ulAlignment & HORZ_LEFT) == HORZ_LEFT )
      return( HORZ_LEFT );

   if ( (m_ulAlignment & HORZ_CENTER) == HORZ_CENTER )
      return( HORZ_CENTER );

   if ( (m_ulAlignment & HORZ_RIGHT) == HORZ_RIGHT )
      return( HORZ_RIGHT );

  return( 0 );
}

zULONG
ZText::GetHorzAlignmentIndex( )
{
   if ( (m_ulAlignment & HORZ_LEFT) == HORZ_LEFT )
      return( 0 );

   if ( (m_ulAlignment & HORZ_CENTER) == HORZ_CENTER )
      return( 1 );

   if ( (m_ulAlignment & HORZ_RIGHT) == HORZ_RIGHT )
      return( 2 );

   return( 0 );
}

zBOOL
ZText::GetSunken( )
{
   return( ((GetExStyle( ) & WS_EX_STATICEDGE) == WS_EX_STATICEDGE) ?
                                                              TRUE : FALSE );
}

CString
ZText::GetText( )
{
   return( *m_pzsText );
}

zULONG
ZText::GetVertAlignment( )
{
   if ( (m_ulAlignment & VERT_TOP) == VERT_TOP )
      return( VERT_TOP );

   if ( (m_ulAlignment & VERT_CENTER) == VERT_CENTER )
      return( VERT_CENTER );

   if ( (m_ulAlignment & VERT_BOTTOM) == VERT_BOTTOM )
      return( VERT_BOTTOM );

   return( 0 );
}

zULONG
ZText::GetVertAlignmentIndex( )
{
   if ( (m_ulAlignment & VERT_TOP) == VERT_TOP )
      return( 0 );

   if ( (m_ulAlignment & VERT_CENTER) == VERT_CENTER )
      return( 1 );

   if ( (m_ulAlignment & VERT_BOTTOM) == VERT_BOTTOM )
      return( 2 );

   return( 0 );
}

ZText&
ZText::SetAlignment( zULONG ulAlign )
{
   DWORD dwRemove = 0;
   DWORD dwHorz = 0;
   DWORD dwVert = 0;

   if ( (ulAlign & HORZ_RIGHT) == HORZ_RIGHT )
   {
      dwHorz = SS_RIGHT;
      dwRemove = SS_LEFT | SS_CENTER;
      m_ulAlignment = HORZ_RIGHT;
   }

   if ( (ulAlign & HORZ_CENTER) == HORZ_CENTER )
   {
      dwHorz = SS_CENTER;
      dwRemove = SS_LEFT | SS_RIGHT;
      m_ulAlignment = HORZ_CENTER;
   }

   if ( (ulAlign & HORZ_LEFT) == HORZ_LEFT )
   {
      dwHorz = SS_LEFT;
      dwRemove = SS_RIGHT | SS_CENTER;
      m_ulAlignment = HORZ_LEFT;
   }

   ModifyStyle( dwRemove, dwHorz );

   if ( (ulAlign & VERT_TOP) == VERT_TOP )
      dwVert = VERT_TOP;

   if ( (ulAlign & VERT_CENTER) == VERT_CENTER )
      dwVert = VERT_CENTER;

   if ( (ulAlign & VERT_BOTTOM) == VERT_BOTTOM )
      dwVert = VERT_BOTTOM;

   m_ulAlignment |= dwVert;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetAngle( zULONG ulAngle )
{
   m_ulAngle = ulAngle / 10;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetBackImage( HINSTANCE hInst, zULONG ulResId )
{
   if ( m_hBackImage )
   {
      DeleteObject( m_hBackImage );
      m_hBackImage = 0;
   }

   if ( ulResId != -1 )
   {
   // HINSTANCE hInst = AfxGetInstanceHandle( );
   // ASSERT( m_pZSubtask->m_hInstRes == hInst );
      m_hBackImage = LoadBitmap( hInst, MAKEINTRESOURCE( ulResId ) );
   }

   InvalidateRect( 0, TRUE );
   return( *this );
}

ZText&
ZText::SetBorder( zBOOL bSet )
{
   ModifyStyle( bSet ? 0 : WS_BORDER, bSet ? WS_BORDER : 0, SWP_DRAWFRAME );
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetCursor( HCURSOR hCursor )
{
   m_hCursor = hCursor;
   return( *this );
}

ZText&
ZText::SetCursor( zULONG ulCursorID )
{
   m_hCursor = LoadCursor( AfxGetInstanceHandle( ),
                           MAKEINTRESOURCE( ulCursorID ) );
   return( *this );
}

ZText&
ZText::SetDisabled( zBOOL bSet )
{
   ModifyStyle( bSet ? 0 : WS_DISABLED, bSet ? WS_DISABLED : 0, 0 );
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetLink( zBOOL bSet )
{
   m_bLink = bSet;
   return( *this );
}

ZText&
ZText::SetLinkCursor( HCURSOR hCursor )
{
   m_hCursor = hCursor;
   return( *this );
}

ZText&
ZText::SetLinkCursor( zULONG ulCursorID )
{
   m_hCursor = LoadCursor( AfxGetInstanceHandle( ),
                           MAKEINTRESOURCE( ulCursorID ) );
   return( *this );
}

ZText&
ZText::SetLowerCase( zBOOL bSet )
{
   m_bLowerCase = bSet;
   m_bUpperCase = FALSE;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetText( zCPCHAR szText )
{
   mDeleteInit( m_pzsText );
   m_pzsText = new CString( szText );

   zPCHAR pchBackSlash = (zPCHAR) zstrchr( *m_pzsText, '\\' );
   while ( pchBackSlash )
   {
      if ( *(pchBackSlash + 1) == '\\' )
         pchBackSlash++;
      else
      if ( *(pchBackSlash + 1) == 'n' )
      {
         *pchBackSlash = '\n';
         strcpy_s( pchBackSlash + 1, zstrlen( pchBackSlash ), pchBackSlash + 2 );
      }

      pchBackSlash = (zPCHAR) zstrchr( pchBackSlash + 1, '\\' );
   }

   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( m_bVerticalText )
      {
         SetWindowText( MakeVerticalText( *m_pzsText ) );
      }
      else
         SetWindowText( *m_pzsText );

      RedrawWindowLocal( );
   }

   return( *this );
}

ZText&
ZText::SetSunken( zBOOL bSet )
{
   ModifyStyleEx( bSet ? 0 : WS_EX_STATICEDGE,
                  bSet ? WS_EX_STATICEDGE : 0,
                  SWP_DRAWFRAME );
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetToolTipPosition( int iPosition )
{
   m_ToolTipCtrl.SetPosition( iPosition );
   return( *this );
}

ZText&
ZText::SetToolTipText( zCPCHAR cpcText )
{
   m_ToolTipCtrl.SetText( cpcText );
   return( *this );
}

ZText&
ZText::SetUpperCase( zBOOL bSet )
{
   m_bLowerCase = FALSE;
   m_bUpperCase = bSet;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::SetVerticalText( zBOOL bSet )
{
   if ( bSet && m_bVerticalText == FALSE )
     SetWindowText( MakeVerticalText( *m_pzsText ) );
   else
   if ( m_bVerticalText )
      SetWindowText( *m_pzsText );

   m_bVerticalText = bSet;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::ShowToolTipOnlyOnMouseClick( zBOOL bUse )
{
   m_ToolTipCtrl.ShowOnlyOnMouseClickDown( bUse );
   return( *this );
}

ZText&
ZText::StretchBitmap( zBOOL bStretch )
{
   m_bStretchBitmap = bStretch;
   if ( m_hBackImage )
      RedrawWindowLocal( );

   return( *this );
}

ZText&
ZText::UseBevelLine( zBOOL bUse )
{
   m_bUseBevelLine = bUse;
   RedrawWindowLocal( );
   return( *this );
}

ZText&
ZText::UseHighLighting( zBOOL bUse )
{
   m_bUseHighLighting = bUse;

   if ( bUse == FALSE )
      SetTextColor( m_clrSaveTextColor );

   return( *this );
}

ZText&
ZText::UseHighLightingOnlyWithToolTip( zBOOL bUse )
{
   m_bUseHighLightingOnlyWithToolTip = bUse;
   return( *this );
}

ZText&
ZText::UseLeftMouseButtonForToolTip( zBOOL bUse )
{
   m_ToolTipCtrl.UseLeftMouseButton( bUse );
   return( *this );
}

ZText&
ZText::UseToolTip( zBOOL bUse )
{
   m_bUseToolTip = bUse;

   if ( bUse )
   {
      m_ToolTipCtrl.Attach( this );
   }
   else
   {
      m_ToolTipCtrl.Detach( );
   }

   return( *this );
}

/////////////////////////////////////////////////////////////////////////////
// ZText message handlers

BOOL
ZText::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CStatic::DestroyWindow( ) );
}

void
ZText::PreSubclassWindow( )
{
   CStatic::PreSubclassWindow( );

   ModifyStyle( 0, SS_NOTIFY );

   if ( (GetStyle( ) & SS_CENTER) == SS_CENTER )
      m_ulAlignment = GetVertAlignment( ) | HORZ_CENTER;

   if ( (GetStyle( ) & SS_RIGHT) == SS_RIGHT )
      m_ulAlignment = GetVertAlignment( ) | HORZ_RIGHT;

   if ( (GetStyle( ) & SS_CENTERIMAGE) == SS_CENTERIMAGE )
      m_ulAlignment = GetHorzAlignment( ) | VERT_CENTER;

// GetWindowText( *m_pzsText );
}

LRESULT
ZText::WindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   if ( uMsg == WM_MOUSEHOVER )
   {
   // m_hCursorOld = ::GetCursor( );
   // SetLinkCursor( IDC_HANDX );
   // SetCursor( (zULONG) IDC_CROSS );
      if ( m_bUseHighLighting && (m_ulMapActFlags & zMAPACT_ENABLED) )
      {
      // m_clrSaveTextColor = m_clrTextForeColor;  I don't think we ever want to do this
         CString cs = m_ToolTipCtrl.GetText( );

         if ( m_bUseHighLightingOnlyWithToolTip == FALSE ||
              (m_bUseHighLightingOnlyWithToolTip &&
               m_bUseToolTip && cs.IsEmpty( ) == FALSE) )
         {
            SetTextColor( m_clrHighLight );
         }
      }
   }
   else
   if ( uMsg == WM_MOUSELEAVE )
   {
   // SetCursor( m_hCursorOld );

      // Determine if mouse pointer is within control when tooltip is used
      // because when mouse goes over tooltip, this message is sent.
      TrackMouse( TRUE, FALSE );
      m_bInside = FALSE;
      if ( m_bUseHighLighting )
         SetTextColor( m_clrSaveTextColor );
   }
   else
   if ( uMsg == WM_SETTEXT )
   {
      if ( m_bBlockMessage == FALSE )
      {
         mDeleteInit( m_pzsText );
         m_pzsText = new CString( (zCPCHAR) lParam );
      }
   }
   else
   if ( uMsg == WM_GETTEXT )
   {
      if ( m_bBlockMessage == FALSE )
      {
         int iCount = (int) wParam;

         zmemset( (zPCHAR) lParam, 0, iCount + 1 );
         zmemcpy( (zPCHAR) lParam, *m_pzsText, iCount );
         return( TRUE );
      }
   }

   return( CStatic::WindowProc( uMsg, wParam, lParam ) );
}

void
ZText::OnEnable( BOOL bEnable )
{
   if ( bEnable == FALSE && m_bLink == FALSE )
      EnableWindow( );  // do not disable (non-hotlink) text ... 2005.03.31
   else
   {
      SetRedraw( FALSE );
      CStatic::OnEnable( bEnable );
      SetRedraw( TRUE );
      Invalidate( );
   }
}

BOOL
ZText::OnEraseBkgnd( CDC *pDC )
{
   return( CStatic::OnEraseBkgnd( pDC ) );
}

void
ZText::OnLButtonDown( UINT uFlags, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZText::OnLButtonDown", "" );
#endif
   if ( m_bLink && (m_ulMapActFlags & zMAPACT_ENABLED) )
      ProcessEvent( this, 1 );

// Can't do this in Conversational dialog.
// CStatic::OnLButtonDown( uFlags, pt );
#ifdef DEBUG_ALL
   TraceLineS( "ZText::OnLButtonDown", " End" );
#endif
}

void
ZText::OnMouseMove( UINT uModKeys, CPoint pt )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZText::OnMouseMove", "" );
#endif

   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   if ( m_bInside == FALSE )
   {
      m_bInside = TRUE;
      TrackMouse( TRUE, TRUE );
   }

   CStatic::OnMouseMove( uModKeys, pt );
}

BOOL
ZText::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMsg )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZText::OnSetCursor", "" );
#endif
   if ( m_hCursor )
   {
      ::SetCursor( m_hCursor );
      return( TRUE );
   }

   return( CStatic::OnSetCursor( pWnd, uHitTest, uMsg ) );
}

void
ZText::OnTimer( UINT uIDEvent )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZText::OnTimer", "" );
#endif
   switch ( uIDEvent )
   {
      case 1: // TEXT FLASH
         if ( m_bTextFlash == FALSE )
            break;

         m_bFlashTextState = !m_bFlashTextState;

         SetWindowText( m_bFlashTextState ? "" : *m_pzsText );

         if ( m_bFlashTextState == FALSE )
            InvalidateRect( 0, FALSE );

         if ( m_bUseDefaultBackColor == FALSE &&
              m_clrBackColor != ::GetSysColor( COLOR_3DFACE ) )
         {
           InvalidateRect( 0, FALSE );
         }

         break;

      case 2: // BACK FLASH
         if ( m_bBackFlash == FALSE )
            break;

         m_bFlashBackState = !m_bFlashBackState;

         InvalidateRect( 0, TRUE );
         break;
   }

   CStatic::OnTimer( uIDEvent );
}

COLORREF
ZText::GetBkColor( )
{
   return( m_clrBackColor );
}

COLORREF
ZText::GetHighLightColor( )
{
   return( m_clrHighLight );
}

COLORREF
ZText::GetTextBackColor( )
{
   return( m_clrTextBackColor );
}

COLORREF
ZText::GetTextColor( )
{
   return( m_clrTextForeColor );
}

zBOOL
ZText::IsDefaultBkColor( )
{
   return( m_bUseDefaultBackColor );
}

zBOOL
ZText::IsDefaultTextBackColor( )
{
   return( m_bUseDefaultTextBackColor );
}

zBOOL
ZText::IsDefaultTextColor( )
{
   return( m_bUseDefaultTextColor );
}

zBOOL
ZText::GetFontBold( )
{
   return( ((m_lf.lfWeight & FW_BOLD) == FW_BOLD) ? TRUE : FALSE );
}

zBOOL
ZText::GetFontItalic( )
{
   return( m_lf.lfItalic );
}

CString
ZText::GetFontName( )
{
   return( m_lf.lfFaceName );
}

zLONG
ZText::GetFontSize( )
{
   return( m_lf.lfHeight );
}

zBOOL
ZText::GetFontUnderline( )
{
   return( m_lf.lfUnderline );
}

zULONG
ZText::GetAlignment( )
{
   return( m_ulAlignment );
}

zULONG
ZText::GetAngle( )
{
   return( m_ulAngle );
}

zBOOL
ZText::GetBevelLine( )
{
   return( m_bUseBevelLine );
}

zBOOL
ZText::GetLink( )
{
   return( m_bLink );
}

zBOOL
ZText::GetVerticalText( )
{
   return( m_bVerticalText );
}

zBOOL
ZText::IsBitmapStretched( )
{
   return( m_bStretchBitmap );
}

zBOOL
ZText::IsDisabled( )
{
   return( ((GetStyle( ) & WS_DISABLED) == WS_DISABLED) );
}

zBOOL
ZText::IsLowerCase( )
{
   return( m_bLowerCase );
}

zBOOL
ZText::IsToolTipUsed( )
{
   return( m_bUseToolTip );
}

zBOOL
ZText::IsUpperCase( )
{
   return( m_bUpperCase );
}

zBOOL
ZText::UseHighLightingOnlyWithToolTip( )
{
   return( m_bUseHighLightingOnlyWithToolTip );
}

zBOOL
ZText::UseToolTipOnLeftMouseClick( )
{
   return( m_ToolTipCtrl.IsLeftMouseButtonUsed( ) );
}

zBOOL
ZText::UseToolTipWithMouseClick( )
{
   return( m_ToolTipCtrl.ShowOnlyOnMouseClickDown( ) );
}

zBOOL
ZText::SetRedrawLocal( zBOOL bRedraw )
{
   zBOOL bRedrawPrev = m_bRedrawLocal;
   m_bRedrawLocal = bRedraw;
   return( bRedrawPrev );
}

void
ZText::RedrawWindowLocal( )
{
   if ( m_bRedrawLocal )
      RedrawWindow( );
}

// ZText - dtor
ZText::~ZText( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZText::dtor ", *m_pzsTag );
#endif

// if ( m_hEmptyBitmap1 )
// {
//    DeleteObject( m_hEmptyBitmap1 );
//    m_hEmptyBitmap1 = 0;
// }

// if ( m_hEmptyBitmap2 )
// {
//    DeleteObject( m_hEmptyBitmap2 );
//    m_hEmptyBitmap2 = 0;
// }

   if ( m_hBackImage )
      DeleteObject( m_hBackImage );

   if ( m_pFont )
   {
      m_pFont->DeleteObject( );
      mDeleteInit( m_pFont );
   }

   if ( m_hBackBrush )
   {
      DeleteObject( m_hBackBrush );
      m_hBackBrush = 0;
   }

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define MAX_COLORS       100

ColorTableEntry g_clrColors[ ] =
{
   { RGB( 0x00, 0x00, 0x00 ), _T( "Black" )          },
   { RGB( 0xA5, 0x2A, 0x00 ), _T( "Brown" )          },
   { RGB( 0x00, 0x40, 0x40 ), _T( "Dark Olive Green" ) },
   { RGB( 0x00, 0x55, 0x00 ), _T( "Dark Green" )     },
   { RGB( 0x00, 0x00, 0x5E ), _T( "Dark Teal" )      },
   { RGB( 0x00, 0x00, 0x8B ), _T( "Dark blue" )      },
   { RGB( 0x4B, 0x00, 0x82 ), _T( "Indigo" )         },
   { RGB( 0x28, 0x28, 0x28 ), _T( "Dark gray" )      },

   { RGB( 0x8B, 0x00, 0x00 ), _T( "Dark red" )       },
   { RGB( 0xFF, 0x68, 0x20 ), _T( "Orange" )         },
   { RGB( 0x8B, 0x8B, 0x00 ), _T( "Dark yellow" )    },
   { RGB( 0x00, 0x93, 0x00 ), _T( "Green" )          },
   { RGB( 0x38, 0x8E, 0x8E ), _T( "Teal" )           },
   { RGB( 0x00, 0x00, 0xFF ), _T( "Blue" )           },
   { RGB( 0x7B, 0x7B, 0xC0 ), _T( "Blue-gray" )      },
   { RGB( 0x66, 0x66, 0x66 ), _T( "Gray - 40" )      },

   { RGB( 0xFF, 0x00, 0x00 ), _T( "Red" )            },
   { RGB( 0xFF, 0xAD, 0x5B ), _T( "Light orange" )   },
   { RGB( 0x32, 0xCD, 0x32 ), _T( "Lime" )           },
   { RGB( 0x3C, 0xB3, 0x71 ), _T( "Sea green" )      },
   { RGB( 0x7F, 0xFF, 0xD4 ), _T( "Aqua" )           },
   { RGB( 0x7D, 0x9E, 0xC0 ), _T( "Light blue" )     },
   { RGB( 0x80, 0x00, 0x80 ), _T( "Violet" )         },
   { RGB( 0x7F, 0x7F, 0x7F ), _T( "Gray - 50" )      },

   { RGB( 0xFF, 0xC0, 0xCB ), _T( "Pink" )           },
   { RGB( 0xFF, 0xD7, 0x00 ), _T( "Gold" )           },
   { RGB( 0xFF, 0xFF, 0x00 ), _T( "Yellow" )         },
   { RGB( 0x00, 0xFF, 0x00 ), _T( "Bright green" )   },
   { RGB( 0x40, 0xE0, 0xD0 ), _T( "Turquoise" )      },
   { RGB( 0xC0, 0xFF, 0xFF ), _T( "Skyblue" )        },
   { RGB( 0x48, 0x00, 0x48 ), _T( "Plum" )           },
   { RGB( 0xC0, 0xC0, 0xC0 ), _T( "Light gray" )     },

   { RGB( 0xFF, 0xE4, 0xE1 ), _T( "Rose" )           },
   { RGB( 0xD2, 0xB4, 0x8C ), _T( "Tan" )            },
   { RGB( 0xFF, 0xFF, 0xE0 ), _T( "Light yellow" )   },
   { RGB( 0x98, 0xFB, 0x98 ), _T( "Pale green " )    },
   { RGB( 0xAF, 0xEE, 0xEE ), _T( "Pale turquoise" ) },
   { RGB( 0x68, 0x83, 0x8B ), _T( "Pale blue" )      },
   { RGB( 0xE6, 0xE6, 0xFA ), _T( "Lavender" )       },
   { RGB( 0xFF, 0xFF, 0xFF ), _T( "White" )          }
};

/////////////////////////////////////////////////////////////////////////////
//
// ColorPicker.cpp : implementation file
//
// ColorPicker is a drop-in color picker control. Check out the
// header file or the accompanying HTML doc file for details.
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included. If
// the source code in this file is used in any commercial application
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
//
// Please use and enjoy. Please let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into this
// file.
//
// Updated 16 May 1998
//         31 May 1998 - added Default text (CJM)
//         9 Jan 1999 - minor vis update
//
/////////////////////////////////////////////////////////////////////////////

#if 0
/////////////////////////////////////////////////////////////////////////////
// ZClrPicker window

void AFXAPI
DDX_ColorPicker( CDataExchange *pDX, int nIDC, COLORREF& clrColor );

void AFXAPI
DDX_ColorPicker( CDataExchange *pDX, int nIDC, COLORREF& clrColor )
{
   HWND hWndCtrl = pDX->PrepareCtrl( nIDC );
   ASSERT ( hWndCtrl );

   ZClrPicker *pColorPicker = (ZClrPicker *) CWnd::FromHandle( hWndCtrl );
   if ( pDX->m_bSaveAndValidate )
   {
      clrColor = pColorPicker->GetColor( );
   }
   else // initializing
   {
      pColorPicker->SetColor( clrColor );
   }
}
#endif

/////////////////////////////////////////////////////////////////////////////
// ZClrPicker

IMPLEMENT_DYNAMIC( ZClrPicker, CButton )

BEGIN_MESSAGE_MAP( ZClrPicker, CButton )
   //{{AFX_MSG_MAP( ZClrPicker )
   ON_CONTROL_REFLECT_EX( BN_CLICKED, OnClicked )
   ON_WM_CREATE( )
   //}}AFX_MSG_MAP
   ON_MESSAGE( CPN_SELENDOK, OnSelEndOK )
   ON_MESSAGE( CPN_SELENDCANCEL, OnSelEndCancel )
   ON_MESSAGE( CPN_SELCHANGE, OnSelChange )
END_MESSAGE_MAP( )

// ZClrPicker - ctor
ZClrPicker::ZClrPicker( ZSubtask *pZSubtask,
                        CWnd     *pWndParent,
                        ZMapAct  *pzmaComposite,
                        zVIEW    vDialog,
                        zSHORT   nOffsetX,
                        zSHORT   nOffsetY,
                        zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
            CButton( ),
            ZMapAct( pZSubtask,          // base class ctor
                     pzmaComposite,
                     pWndParent,
                     this,
                     vDialog,
                     nOffsetX,
                     nOffsetY,
                     pCtrlDef,
                     "ColorPick" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZClrPicker::ctor ", *m_pzsTag );
#endif

   m_nNbrColors = zsizeof( g_clrColors ) / sizeof( ColorTableEntry );
   ASSERT( m_nNbrColors <= MAX_COLORS );
   if ( m_nNbrColors > MAX_COLORS )
      m_nNbrColors = MAX_COLORS;

   SetBkColor( ::GetSysColor( COLOR_3DFACE ) );
   SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

   m_bTrackSelection = FALSE;
   m_nSelectionMode = CP_MODE_BK;
   m_bActive = FALSE;

   m_csDefaultText = _T( "Automatic" );
   m_csCustomText = _T( "More Colors..." );

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZClrPicker::CreateZ( )
{
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
}

ZClrPicker::~ZClrPicker( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ZClrPicker message handlers

LONG
ZClrPicker::OnSelEndOK( WPARAM wParam, LPARAM lParam )
{
   COLORREF clrNewColor = (COLORREF) lParam;
   zBOOL    bChange = (clrNewColor != GetColor( ));

   m_bActive = FALSE;
   SetColor( clrNewColor );

   CWnd *pParent = GetParent( );
   if ( pParent )
   {
      pParent->SendMessage( CPN_CLOSEUP, (WPARAM) GetDlgCtrlID( ), lParam );
      pParent->SendMessage( CPN_SELENDOK, (WPARAM) GetDlgCtrlID( ), lParam );
   }

   if ( bChange )
   {
      if ( pParent )
      {
         pParent->SendMessage( CPN_SELCHANGE,
                               (WPARAM) GetDlgCtrlID( ), lParam );
      }

      ProcessEvent( this, 1 );
   }

   return( TRUE );
}

zLONG
ZClrPicker::OnSelEndCancel( WPARAM wParam, LPARAM lParam )
{
   m_bActive = FALSE;
   SetColor( (COLORREF) lParam );

   CWnd *pParent = GetParent( );
   if ( pParent )
   {
      pParent->SendMessage( CPN_CLOSEUP, (WPARAM) GetDlgCtrlID( ), lParam );
      pParent->SendMessage( CPN_SELENDCANCEL, (WPARAM) GetDlgCtrlID( ), lParam );
   }

   return( TRUE );
}

zLONG
ZClrPicker::OnSelChange( WPARAM wParam, LPARAM lParam )
{
   if ( m_bTrackSelection )
      SetColor( (COLORREF) lParam );

   CWnd *pParent = GetParent( );
   if ( pParent )
      pParent->SendMessage( CPN_SELCHANGE, (WPARAM) GetDlgCtrlID( ), lParam );

   return( TRUE );
}

int
ZClrPicker::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CButton::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetWindowSize( );   // resize appropriately
   return( 0 );
}

// On mouse click, create and show a ZClrPopup window for color selection.
BOOL
ZClrPicker::OnClicked( )
{
   m_bActive = TRUE;
   CRect rect;
   GetWindowRect( rect );
   new ZClrPopup( CPoint( rect.left, rect.bottom ), // point to display popup
                  GetColor( ),                      // selected color
                  this,                             // parent
                  m_csDefaultText,                  // "default" text area
                  m_csCustomText );                 // custom text

   CWnd *pParent = GetParent( );
   if ( pParent )
      pParent->SendMessage( CPN_DROPDOWN, (WPARAM) GetDlgCtrlID( ),
                            (LPARAM) GetColor( ) );

   // Doc says I should return FALSE to keep the parent from also getting
   // the message.  HA! What a joke.
   return( TRUE );
}

void
ZClrPicker::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
   ASSERT( lpDIS );

   CDC   *pDC = CDC::FromHandle( lpDIS->hDC );
   CRect rect = lpDIS->rcItem;
   UINT  state = lpDIS->itemState;
   CString csText;

   CSize Margins( ::GetSystemMetrics( SM_CXEDGE ),
                  ::GetSystemMetrics( SM_CYEDGE ) );

   // Draw arrow
   if ( m_bActive )
      state |= ODS_SELECTED;

   pDC->DrawFrameControl( &m_rectArrow, DFC_SCROLL, DFCS_SCROLLDOWN |
                          ((state & ODS_SELECTED) ? DFCS_PUSHED : 0) |
                          ((state & ODS_DISABLED) ? DFCS_INACTIVE : 0) );

   pDC->DrawEdge( rect, EDGE_SUNKEN, BF_RECT );

   // Must reduce the size of the "client" area of the button due to edge
   // thickness.
   rect.DeflateRect( Margins.cx, Margins.cy );

   // Fill remaining area with color.
   rect.right -= m_rectArrow.Width( );

   CBrush brush( ((state & ODS_DISABLED) || m_clrColorBk == CLR_DEFAULT) ?
                  ::GetSysColor( COLOR_3DFACE ) : m_clrColorBk );
   CBrush *pOldBrush = (CBrush *) pDC->SelectObject( &brush );
   pDC->SelectStockObject( NULL_PEN );
   pDC->Rectangle( rect );
   pDC->SelectObject( pOldBrush );

   BYTE byRed = GetRValue( m_clrColorBk );
   BYTE byGreen = GetGValue( m_clrColorBk );
   BYTE byBlue = GetBValue( m_clrColorBk );
   COLORREF clrText;

   // Based upon the "darkness" of the selected color, use a white or black
   // text color on the background (selected color).
   if ( byRed < 192 && byGreen < 192 && byBlue <= 255 ||
        byRed < 192 && byGreen < 254 && byBlue < 192  ||
        byRed < 254 && byGreen < 192 && byBlue < 192 )
   {
      clrText = RGB( 255, 255, 255 );  // white
   }
   else
      clrText = 0;                     // black

   // Draw the window text (if any).
   GetWindowText( csText );
   if ( csText.GetLength( ) == 0 )
   {
      if ( m_clrColorBk == CLR_DEFAULT )
      {
         csText = m_csDefaultText;
      }
      else
      {
         zSHORT k;

         for ( k = 0; k < m_nNbrColors; k++ )
         {
            if ( m_clrColorBk == g_clrColors[ k ].clrColor )
            {
               csText = g_clrColors[ k ].pchName;
               break;
            }
         }

         if ( k >= m_nNbrColors )
         {
            csText.Format( "r%.1d.g%.1d.b%.1d",
                           (int) byRed, (int) byGreen, (int) byBlue );
         }
      }
   }

   {
      HFONT hFont = (HFONT) GetStockObject( DEFAULT_GUI_FONT );
      if ( hFont == 0 )
         hFont = (HFONT) GetStockObject( ANSI_VAR_FONT );

      HFONT hOldFont = (HFONT) pDC->SelectObject( hFont );
      int nOldMode = pDC->SetBkMode( TRANSPARENT );
      if ( state & ODS_DISABLED )
      {
         rect.OffsetRect( 1, 1 );
         COLORREF clrOld = pDC->SetTextColor( ::GetSysColor( COLOR_3DHILIGHT ) );
         pDC->DrawText( csText, rect,
                        DT_CENTER | DT_SINGLELINE | DT_VCENTER );
         rect.OffsetRect( -1, -1 );
         pDC->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
         pDC->DrawText( csText, rect,
                        DT_CENTER | DT_SINGLELINE | DT_VCENTER );
         pDC->SetTextColor( clrOld );
      }
      else
      {
         COLORREF clrOld = pDC->SetTextColor( clrText );
         pDC->DrawText( csText, rect,
                        DT_CENTER | DT_SINGLELINE | DT_VCENTER );
         pDC->SetTextColor( clrOld );
      }

      pDC->SetBkMode( nOldMode );
      pDC->SelectObject( hOldFont );
   }

   // Draw focus rect
   if ( state & ODS_FOCUS )
   {
      rect.DeflateRect( 1, 1 );
      pDC->DrawFocusRect( rect );
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZClrPicker overrides

void
ZClrPicker::PreSubclassWindow( )
{
   ModifyStyle( 0, BS_OWNERDRAW );      // make it owner drawn
   CButton::PreSubclassWindow( );
   SetWindowSize( );                    // resize appropriately
}

/////////////////////////////////////////////////////////////////////////////
// ZClrPicker attributes

COLORREF
ZClrPicker::GetColor( )
{
   return( (m_nSelectionMode == CP_MODE_TEXT) ? GetTextColor( ) :
                                                GetBkColor( ) );
}

void
ZClrPicker::SetColor( COLORREF clrColor )
{
   (m_nSelectionMode == CP_MODE_TEXT) ? SetTextColor( clrColor ) :
                                        SetBkColor( clrColor );
}

void
ZClrPicker::SetBkColor( COLORREF clrColorBk )
{
   m_clrColorBk = clrColorBk;
   if ( mIs_hWnd( m_hWnd ) )
      RedrawWindow( );
}

void
ZClrPicker::SetTextColor( COLORREF clrColorText )
{
   m_clrColorText = clrColorText;
   if ( mIs_hWnd( m_hWnd ) )
      RedrawWindow( );
}

void
ZClrPicker::SetDefaultText( zCPCHAR cpcDefaultText )
{
   m_csDefaultText = (cpcDefaultText) ? cpcDefaultText : _T( "Automatic" );
}

void
ZClrPicker::SetCustomText( zCPCHAR cpcCustomText )
{
   m_csCustomText = (cpcCustomText) ? cpcCustomText : _T( "More Colors..." );
}

/////////////////////////////////////////////////////////////////////////////
// ZClrPicker implementation

void
ZClrPicker::SetWindowSize( )
{
   // Get size dimensions of edges.
   CSize sizeMargin( ::GetSystemMetrics( SM_CXEDGE ),
                     ::GetSystemMetrics( SM_CYEDGE ) );

   // Get size of dropdown arrow.
   zLONG lArrowWidth = max( ::GetSystemMetrics( SM_CXHTHUMB ),
                            sizeMargin.cx * 5 );
   zLONG lArrowHeight = max( ::GetSystemMetrics( SM_CYVTHUMB ),
                             sizeMargin.cy * 5 );
   CSize sizeArrow( max( lArrowWidth, lArrowHeight ),
                    max( lArrowWidth, lArrowHeight ) );

   // Get window size.
   CRect rect;
   GetWindowRect( rect );

   CWnd *pParent = GetParent( );
   if ( pParent )
      pParent->ScreenToClient( rect );

   // Set window size at least as wide as 2 arrows, and as high as
   // arrow + margins.
   zLONG lWidth = max( rect.Width( ),
                       (2 * sizeArrow.cx) + (2 * sizeMargin.cx) );
   MoveWindow( rect.left, rect.top, lWidth,
               sizeArrow.cy + (2 * sizeMargin.cy), TRUE );

   // Get the new coords of this window.
   GetWindowRect( rect );
   ScreenToClient( rect );

   // Get the rect where the arrow goes, and convert to client coords.
   m_rectArrow.SetRect( rect.right - sizeArrow.cx - sizeMargin.cx,
                        rect.top + sizeMargin.cy, rect.right - sizeMargin.cx,
                        rect.bottom - sizeMargin.cy );
}

void
ZClrPicker::SetTrackSelection( zBOOL bTracking )
{
   m_bTrackSelection = bTracking;
}

zBOOL
ZClrPicker::GetTrackSelection( )
{
   return( m_bTrackSelection );
}

void
ZClrPicker::SetSelectionMode( UINT nMode )
{
   m_nSelectionMode = nMode;
}

UINT
ZClrPicker::GetSelectionMode( )
{
   return( m_nSelectionMode );
};

COLORREF
ZClrPicker::GetBkColor( )
{
   return( m_clrColorBk );
}

COLORREF
ZClrPicker::GetTextColor( )
{
   return( m_clrColorText );
}

zSHORT
ZClrPicker::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZClrPicker::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW   vApp;

      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
         {
            zLONG  lColor = -1;
            zSHORT nRC =
               GetVariableFromAttribute( &lColor, 0, zTYPE_INTEGER,
                                         sizeof( zLONG ),
                                         vApp, *m_pzsEName,
                                         *m_pzsAName, *m_pzsContext,
                                         m_pzsContext->IsEmpty( ) ?
                                zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT :
                                zACCEPT_NULL_ENTITY );
         // if ( nRC == zVAR_NULL )
         //    lColor = -1;

            SetColor( lColor );
            return( 0 );
         }
      }
   }

   return( -1 );
}

// Get the specified color and map it to the data OI.
zSHORT
ZClrPicker::MapToOI( zLONG lFlag )
{
   zVIEW  vApp;
   zSHORT nRC = 0;

#ifdef DEBUG_ALL
   TraceLineS( "ZClrPicker::MapToOI: ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      zLONG  lColor;

      lColor = GetColor( );
      nRC = SetAttributeFromVariable( vApp, *m_pzsEName,
                                      *m_pzsAName, &lColor,
                                      zTYPE_INTEGER, 0,
                                      *m_pzsContext,
                                      m_pzsContext->IsEmpty( ) ?
                             (zSHORT) (lFlag | zUSE_DEFAULT_CONTEXT) :
                             (zSHORT) lFlag );
   }

   return( nRC );
}

zLONG
ZClrPicker::GetZCtrlProperty( zLONG   lPropertyType,
                              zPULONG pulNumericProperty,
                              zPCHAR  pchStringProperty,
                              zLONG   lMaxLth )
{
   switch ( lPropertyType )
   {
      case zCONTROL_PROPERTY_INTEGER_DATA:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            *pulNumericProperty = GetColor( );
            return( 0 );
         }

         *pulNumericProperty = 0;
         return( -1 );
      }
   }

   return( ZMapAct::GetZCtrlProperty( lPropertyType,
                                      pulNumericProperty,
                                      pchStringProperty,
                                      lMaxLth ) );
}

zLONG
ZClrPicker::SetZCtrlProperty( zLONG   lPropertyType,
                              zULONG  ulNumericProperty,
                              zCPCHAR cpcStringProperty )
{
   switch ( lPropertyType )
   {
      case zCONTROL_PROPERTY_INTEGER_DATA:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            SetColor( ulNumericProperty );
            return( 0 );
         }

         return( -1 );
      }
   }

   return( ZMapAct::SetZCtrlProperty( lPropertyType,
                                      ulNumericProperty,
                                      cpcStringProperty ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// ColorPopup.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// Updated 30 May 1998 to allow any number of colors, and to
//                     make the appearance closer to Office 97.
//                     Also added "Default" text area.         (CJM)
//
//         13 June 1998 Fixed change of focus bug (CJM)
//         30 June 1998 Fixed bug caused by focus bug fix (D'oh!!)
//                      Solution suggested by Paul Wilkerson.
//
// ColorPopup is a helper class for the color picker control
// ZClrPicker. Check out the header file or the accompanying
// HTML doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Expect bugs.
//
// Please use and enjoy. Please let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into this
// file.
//
/////////////////////////////////////////////////////////////////////////////

#define DEFAULT_BOX_VALUE -3
#define CUSTOM_BOX_VALUE  -2
#define INVALID_COLOR     -1

/////////////////////////////////////////////////////////////////////////////
// ZClrPopup

BEGIN_MESSAGE_MAP( ZClrPopup, CWnd )
   //{{AFX_MSG_MAP( ZClrPopup )
   ON_WM_NCDESTROY( )
   ON_WM_LBUTTONUP( )
   ON_WM_PAINT( )
   ON_WM_MOUSEMOVE( )
   ON_WM_KEYDOWN( )
   ON_WM_QUERYNEWPALETTE( )
   ON_WM_PALETTECHANGED( )
   ON_WM_KILLFOCUS( )
   ON_WM_ACTIVATEAPP( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZClrPopup::ZClrPopup( )
{
   Initialize( );
}

ZClrPopup::ZClrPopup( CPoint p, COLORREF clrColor, CWnd *pParentWnd,
                      zCPCHAR cpcDefaultText /* = 0 */,
                      zCPCHAR cpcCustomText  /* = 0 */ )
{
   Initialize( );

   m_clrColor = m_clrInitialColor = clrColor;
   m_pParent = pParentWnd;
   m_csDefaultText = cpcDefaultText ? cpcDefaultText : _T( "" );
   m_csCustomText = cpcCustomText ? cpcCustomText : _T( "" );

   ZClrPopup::Create( p, clrColor, pParentWnd, cpcDefaultText, cpcCustomText );
}

void
ZClrPopup::Initialize( )
{
   m_nNbrColors = zsizeof( g_clrColors ) / sizeof( ColorTableEntry );
   ASSERT( m_nNbrColors <= MAX_COLORS );
   if ( m_nNbrColors > MAX_COLORS )
      m_nNbrColors = MAX_COLORS;

   m_lNbrCols = 0;
   m_lNbrRows = 0;
   m_nBoxSize = 18;
   m_nMargin = ::GetSystemMetrics( SM_CXEDGE );
   m_lCurrentSel = INVALID_COLOR;
   m_lChosenColorSel = INVALID_COLOR;
   m_pParent = 0;
   m_clrColor = m_clrInitialColor = RGB( 0,0,0 );

   // Idiot check: Make sure the color square is at least 5 x 5.
   if ( m_nBoxSize - 2 * m_nMargin - 2 < 5 )
      m_nBoxSize = 5 + 2 * m_nMargin + 2;

   // Create the font.
   NONCLIENTMETRICS ncm;
   ncm.cbSize = sizeof( NONCLIENTMETRICS );
   VERIFY( SystemParametersInfo( SPI_GETNONCLIENTMETRICS,
                                 sizeof( NONCLIENTMETRICS ), &ncm, 0 ) );
   m_Font.CreateFontIndirect( &(ncm.lfMessageFont) );

   // Create the palette.
   struct
   {
      LOGPALETTE   LogPalette;
      PALETTEENTRY PalEntry[ MAX_COLORS ];
   } pal;

   LOGPALETTE *plp = (LOGPALETTE *) &pal;
   plp->palVersion = 0x300;
   plp->palNumEntries = (WORD) m_nNbrColors;

   for ( zSHORT k = 0; k < m_nNbrColors; k++ )
   {
      plp->palPalEntry[ k ].peRed = GetRValue( g_clrColors[ k ].clrColor );
      plp->palPalEntry[ k ].peGreen = GetGValue( g_clrColors[ k ].clrColor );
      plp->palPalEntry[ k ].peBlue = GetBValue( g_clrColors[ k ].clrColor );
      plp->palPalEntry[ k ].peFlags = 0;
   }

   m_Palette.CreatePalette( plp );
}

ZClrPopup::~ZClrPopup( )
{
   m_Font.DeleteObject( );
   m_Palette.DeleteObject( );
}

BOOL
ZClrPopup::Create( CPoint p, COLORREF clrColor, CWnd *pParentWnd,
                   zCPCHAR cpcDefaultText /* = 0 */,
                   zCPCHAR cpcCustomText  /* = 0 */ )
{
   ASSERT( pParentWnd && mIs_hWnd( pParentWnd->GetSafeHwnd( ) ) );
   ASSERT( pParentWnd->IsKindOf( RUNTIME_CLASS( ZClrPicker ) ) );

   m_pParent = pParentWnd;
   m_clrColor = m_clrInitialColor = clrColor;

   // Get the class name and create the window.
   CString csClassName =
     AfxRegisterWndClass( CS_CLASSDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW,
                          0, (HBRUSH) (COLOR_BTNFACE + 1), 0 );

   if ( CWnd::CreateEx( 0, csClassName, _T( "" ), WS_VISIBLE | WS_POPUP,
                        p.x, p.y, 100, 100, // size updated soon
                        pParentWnd->GetSafeHwnd( ), 0, 0 ) == 0 )
   {
      return( FALSE );
   }

   // Store the Custom text.
   if ( cpcCustomText )
      m_csCustomText = cpcCustomText;

   // Store the Default Area text
   if ( cpcDefaultText )
      m_csDefaultText = cpcDefaultText;

   // Set the window size.
   SetWindowSize( );

   // Create the tooltips.
   CreateToolTips( );

   // Find which cell (if any) corresponds to the initial color.
   FindCellFromColor( clrColor );

   // Capture all mouse events for the life of this window.
   SetCapture( );

   return( TRUE );
}

COLORREF
ZClrPopup::GetColor( zLONG lIndex )
{
   return( g_clrColors[ lIndex ].clrColor );
}

zCPCHAR
ZClrPopup::GetColorName( zLONG lIndex )
{
   return( g_clrColors[ lIndex ].pchName );
}

/////////////////////////////////////////////////////////////////////////////
// ZClrPopup message handlers

// For tooltips
BOOL
ZClrPopup::PreTranslateMessage( MSG *pMsg )
{
   m_ToolTip.RelayEvent( pMsg );
   return( CWnd::PreTranslateMessage( pMsg ) );
}

// If an arrow key is pressed, then move the selection
void
ZClrPopup::OnKeyDown( UINT uChar, UINT uRepeatCnt, UINT uFlags )
{
   zLONG lRow = GetRow( m_lCurrentSel );
   zLONG lCol = GetColumn( m_lCurrentSel );

   if ( uChar == VK_DOWN )
   {
      if ( lRow == DEFAULT_BOX_VALUE )
         lRow = lCol = 0;
      else
      if ( lRow == CUSTOM_BOX_VALUE )
      {
         if ( m_csDefaultText.GetLength( ) )
            lRow = lCol = DEFAULT_BOX_VALUE;
         else
            lRow = lCol = 0;
      }
      else
      {
         lRow++;
         if ( GetIndex( lRow, lCol ) < 0 )
         {
            if ( m_csCustomText.GetLength( ) )
               lRow = lCol = CUSTOM_BOX_VALUE;
            else
            if ( m_csDefaultText.GetLength( ) )
               lRow = lCol = DEFAULT_BOX_VALUE;
            else
               lRow = lCol = 0;
         }
      }

      ChangeSelection( GetIndex( lRow, lCol ) );
   }

   if ( uChar == VK_UP )
   {
      if ( lRow == DEFAULT_BOX_VALUE )
      {
         if ( m_csCustomText.GetLength( ) )
            lRow = lCol = CUSTOM_BOX_VALUE;
         else
         {
            lRow = GetRow( m_nNbrColors - 1 );
            lCol = GetColumn( m_nNbrColors - 1 );
         }
      }
      else
      if ( lRow == CUSTOM_BOX_VALUE )
      {
         lRow = GetRow( m_nNbrColors - 1 );
         lCol = GetColumn( m_nNbrColors - 1 );
      }
      else
      if ( lRow > 0 )
         lRow--;
      else /* lRow == 0 */
      {
         if ( m_csDefaultText.GetLength( ) )
            lRow = lCol = DEFAULT_BOX_VALUE;
         else
         if ( m_csCustomText.GetLength( ) )
            lRow = lCol = CUSTOM_BOX_VALUE;
         else
         {
            lRow = GetRow( m_nNbrColors - 1 );
            lCol = GetColumn( m_nNbrColors - 1 );
         }
      }

      ChangeSelection( GetIndex( lRow, lCol ) );
   }

   if ( uChar == VK_RIGHT )
   {
      if ( lRow == DEFAULT_BOX_VALUE )
         lRow = lCol = 0;
      else
      if ( lRow == CUSTOM_BOX_VALUE )
      {
         if ( m_csDefaultText.GetLength( ) )
            lRow = lCol = DEFAULT_BOX_VALUE;
         else
            lRow = lCol = 0;
      }
      else
      if ( lCol < m_lNbrCols - 1 )
         lCol++;
      else
      {
         lCol = 0;
         lRow++;
      }

      if ( GetIndex( lRow,lCol ) == INVALID_COLOR )
      {
         if ( m_csCustomText.GetLength( ) )
            lRow = lCol = CUSTOM_BOX_VALUE;
         else
         if ( m_csDefaultText.GetLength( ) )
            lRow = lCol = DEFAULT_BOX_VALUE;
         else
            lRow = lCol = 0;
      }

      ChangeSelection( GetIndex( lRow, lCol ) );
   }

   if ( uChar == VK_LEFT )
   {
      if ( lRow == DEFAULT_BOX_VALUE )
      {
         if ( m_csCustomText.GetLength( ) )
            lRow = lCol = CUSTOM_BOX_VALUE;
         else
         {
            lRow = GetRow( m_nNbrColors - 1 );
            lCol = GetColumn( m_nNbrColors - 1 );
         }
      }
      else
      if ( lRow == CUSTOM_BOX_VALUE )
      {
         lRow = GetRow( m_nNbrColors - 1 );
         lCol = GetColumn( m_nNbrColors - 1 );
      }
      else
      if ( lCol > 0 )
         lCol--;
      else /* lCol == 0 */
      {
         if ( lRow > 0 )
         {
            lRow--;
            lCol = m_lNbrCols - 1;
         }
         else
         {
            if ( m_csDefaultText.GetLength( ) )
               lRow = lCol = DEFAULT_BOX_VALUE;
            else
            if ( m_csCustomText.GetLength( ) )
               lRow = lCol = CUSTOM_BOX_VALUE;
            else
            {
               lRow = GetRow( m_nNbrColors - 1 );
               lCol = GetColumn( m_nNbrColors - 1 );
            }
         }
      }

      ChangeSelection( GetIndex( lRow, lCol ) );
   }

   if ( uChar == VK_ESCAPE )
   {
      m_clrColor = m_clrInitialColor;
      EndSelection( CPN_SELENDCANCEL );
      return;
   }

   if ( uChar == VK_RETURN || uChar == VK_SPACE )
   {
      EndSelection( CPN_SELENDOK );
      return;
   }

   CWnd::OnKeyDown( uChar, uRepeatCnt, uFlags );
}

// auto-deletion
void
ZClrPopup::OnNcDestroy( )
{
#if 0
   // Delete the tool for each cell.
   for ( zSHORT k = 0; k < m_nNbrColors; k++ )
   {
      CRect rect;
      if ( GetCellRect( k, rect ) == 0 )
         continue;

      m_ToolTip.DelTool( this, k + 1 );
   }
#endif

   CWnd::OnNcDestroy( );
   delete( this );
}

void
ZClrPopup::OnPaint( )
{
   CPaintDC dc( this ); // device context for painting

   // Draw the Default Area text
   if ( m_csDefaultText.GetLength( ) )
      DrawCell( &dc, DEFAULT_BOX_VALUE );

   // Draw color cells
   for ( zSHORT k = 0; k < m_nNbrColors; k++ )
      DrawCell( &dc, k );

   // Draw custom text
   if ( m_csCustomText.GetLength( ) )
       DrawCell( &dc, CUSTOM_BOX_VALUE );

   // Draw raised window edge (ex-window style WS_EX_WINDOWEDGE is supposed
   // to do this, but for some reason isn't.
   CRect rect;
   GetClientRect( rect );
   dc.DrawEdge( rect, EDGE_RAISED, BF_RECT );
}

void
ZClrPopup::OnMouseMove( UINT nFlags, CPoint pt )
{
   zLONG lNewSelection = INVALID_COLOR;

   // Translate points to be relative raised window edge.
   pt.x -= m_nMargin;
   pt.y -= m_nMargin;

   // First check we aren't in text box.
   if ( m_csCustomText.GetLength( ) && m_CustomTextRect.PtInRect( pt ) )
      lNewSelection = CUSTOM_BOX_VALUE;
   else
   if ( m_csDefaultText.GetLength( ) && m_DefaultTextRect.PtInRect( pt ) )
      lNewSelection = DEFAULT_BOX_VALUE;
   else
   {
      // Take the text box into account.
      if ( m_csDefaultText.GetLength( ) )
         pt.y -= m_DefaultTextRect.Height( );

      // Get the row and column.
      lNewSelection = GetIndex( pt.y / m_nBoxSize, pt.x / m_nBoxSize );

      // In range? If not, default and exit.
      if ( lNewSelection < 0 || lNewSelection >= m_nNbrColors )
      {
         CWnd::OnMouseMove( nFlags, pt );
         return;
      }
   }

   // OK - we have the row and column of the current selection (may be
   // CUSTOM_BOX_VALUE).  Has the row/col selection changed?  If so, then
   // redraw old and new cells.
   if ( lNewSelection != m_lCurrentSel )
      ChangeSelection( lNewSelection );

   CWnd::OnMouseMove( nFlags, pt );
}

// End selection on LButtonUp.
void
ZClrPopup::OnLButtonUp( UINT nFlags, CPoint pt )
{
   CWnd::OnLButtonUp( nFlags, pt );

   pt = CPoint( GetMessagePos( ) );

   if ( m_WindowRect.PtInRect( pt ) )
      EndSelection( CPN_SELENDOK );
   else
      EndSelection( CPN_SELENDCANCEL );
}

/////////////////////////////////////////////////////////////////////////////
// ZClrPopup implementation

zLONG
ZClrPopup::GetIndex( zLONG lRow, zLONG lCol ) const
{
   if ( (lRow == CUSTOM_BOX_VALUE || lCol == CUSTOM_BOX_VALUE) &&
        m_csCustomText.GetLength( ) )
   {
      return( CUSTOM_BOX_VALUE );
   }
   else
   if ( (lRow == DEFAULT_BOX_VALUE || lCol == DEFAULT_BOX_VALUE) &&
        m_csDefaultText.GetLength( ) )
   {
      return( DEFAULT_BOX_VALUE );
   }
   else
   if ( lRow < 0 || lCol < 0 || lRow >= m_lNbrRows || lCol >= m_lNbrCols )
      return( INVALID_COLOR );
   else
   {
      if ( lRow * m_lNbrCols + lCol >= m_nNbrColors )
         return( INVALID_COLOR );
      else
         return( lRow * m_lNbrCols + lCol );
   }
}

zLONG
ZClrPopup::GetRow( zLONG lIndex ) const
{
   if ( lIndex == CUSTOM_BOX_VALUE && m_csCustomText.GetLength( ) )
      return( CUSTOM_BOX_VALUE );
   else
   if ( lIndex == DEFAULT_BOX_VALUE && m_csDefaultText.GetLength( ) )
      return( DEFAULT_BOX_VALUE );
   else
   if ( lIndex < 0 || lIndex >= m_nNbrColors )
      return( INVALID_COLOR );
   else
      return( lIndex / m_lNbrCols );
}

zLONG
ZClrPopup::GetColumn( zLONG lIndex ) const
{
   if ( lIndex == CUSTOM_BOX_VALUE && m_csCustomText.GetLength( ) )
      return( CUSTOM_BOX_VALUE );
   else
   if ( lIndex == DEFAULT_BOX_VALUE && m_csDefaultText.GetLength( ) )
      return( DEFAULT_BOX_VALUE );
   else
   if ( lIndex < 0 || lIndex >= m_nNbrColors )
      return( INVALID_COLOR );
   else
      return( lIndex % m_lNbrCols );
}

void
ZClrPopup::FindCellFromColor( COLORREF clrColor )
{
   if ( clrColor == CLR_DEFAULT && m_csDefaultText.GetLength( ) )
   {
      m_lChosenColorSel = DEFAULT_BOX_VALUE;
      return;
   }

   for ( zSHORT k = 0; k < m_nNbrColors; k++ )
   {
      if ( GetColor( k ) == clrColor )
      {
         m_lChosenColorSel = k;
         return;
      }
   }

   if ( m_csCustomText.GetLength( ) )
      m_lChosenColorSel = CUSTOM_BOX_VALUE;
   else
      m_lChosenColorSel = INVALID_COLOR;
}

// Gets the dimensions of the color cell given by row/col.
zBOOL
ZClrPopup::GetCellRect( zLONG lIndex, const LPRECT& rect )
{
   if ( lIndex == CUSTOM_BOX_VALUE )
   {
      ::SetRect( rect,
                 m_CustomTextRect.left,  m_CustomTextRect.top,
                 m_CustomTextRect.right, m_CustomTextRect.bottom );
      return( TRUE );
   }
   else
   if ( lIndex == DEFAULT_BOX_VALUE )
   {
      ::SetRect( rect,
                 m_DefaultTextRect.left,  m_DefaultTextRect.top,
                 m_DefaultTextRect.right, m_DefaultTextRect.bottom );
      return( TRUE );
   }

   if ( lIndex < 0 || lIndex >= m_nNbrColors )
      return( FALSE );

   rect->left = GetColumn( lIndex ) * m_nBoxSize + m_nMargin;
   rect->top = GetRow( lIndex ) * m_nBoxSize + m_nMargin;

   // Move everything down if we are displaying a default text area
   if ( m_csDefaultText.GetLength( ) )
      rect->top += (m_nMargin + m_DefaultTextRect.Height( ));

   rect->right = rect->left + m_nBoxSize;
   rect->bottom = rect->top + m_nBoxSize;

   return( TRUE );
}

// Works out an appropriate size and position of this window
void
ZClrPopup::SetWindowSize( )
{
   CSize TextSize;

   // If we are showing a custom or default text area, get the font and
   // text size.
   if ( m_csCustomText.GetLength( ) || m_csDefaultText.GetLength( ) )
   {
      CClientDC dc( this );
      CFont *pOldFont = (CFont *) dc.SelectObject( &m_Font );

      // Get the size of the custom text (if there IS custom text).
      TextSize = CSize( 0, 0 );
      if ( m_csCustomText.GetLength( ) )
         TextSize = dc.GetTextExtent( m_csCustomText );

      // Get the size of the default text (if there IS default text).
      if ( m_csDefaultText.GetLength( ) )
      {
         CSize DefaultSize = dc.GetTextExtent( m_csDefaultText );
         if ( DefaultSize.cx > TextSize.cx )
            TextSize.cx = DefaultSize.cx;

         if ( DefaultSize.cy > TextSize.cy )
            TextSize.cy = DefaultSize.cy;
      }

      dc.SelectObject( pOldFont );
      TextSize += CSize( 2 * m_nMargin, 2 * m_nMargin );

      // Add even more space to draw the horizontal line.
      TextSize.cy += 2 * m_nMargin + 2;
   }

   // Get the number of columns and rows.
   //m_lNbrCols = (int) sqrt( (double) m_nNbrColors ); // for a square window (yuk)
   m_lNbrCols = 8;
   m_lNbrRows = m_nNbrColors / m_lNbrCols;
   if ( m_nNbrColors % m_lNbrCols )
      m_lNbrRows++;

   // Get the current window position, and set the new size.
   CRect rect;
   GetWindowRect( rect );

   m_WindowRect.SetRect( rect.left, rect.top,
                         rect.left + m_lNbrCols * m_nBoxSize + 2 * m_nMargin,
                         rect.top + m_lNbrRows * m_nBoxSize + 2 * m_nMargin );

   // If custom text, then expand window if necessary, and set text width as
   // window width.
   if ( m_csDefaultText.GetLength( ) )
   {
      if ( TextSize.cx > m_WindowRect.Width( ) )
           m_WindowRect.right = m_WindowRect.left + TextSize.cx;

      TextSize.cx = m_WindowRect.Width( )-2 * m_nMargin;

      // Work out the text area.
      m_DefaultTextRect.SetRect( m_nMargin, m_nMargin,
                                 m_nMargin + TextSize.cx,
                                 2 * m_nMargin + TextSize.cy );
      m_WindowRect.bottom += m_DefaultTextRect.Height( ) + 2 * m_nMargin;
   }

   // If custom text, then expand window if necessary, and set text width as
   // window width.
   if ( m_csCustomText.GetLength( ) )
   {
      if ( TextSize.cx > m_WindowRect.Width( ) )
         m_WindowRect.right = m_WindowRect.left + TextSize.cx;

      TextSize.cx = m_WindowRect.Width( ) - 2 * m_nMargin;

      // Work out the text area
      m_CustomTextRect.SetRect( m_nMargin, m_WindowRect.Height( ),
                                m_nMargin+TextSize.cx,
                                m_WindowRect.Height( ) +
                                                   m_nMargin + TextSize.cy );
      m_WindowRect.bottom += m_CustomTextRect.Height( ) + 2 * m_nMargin;
   }

   // Need to check it'll fit on screen: Too far right?
   CSize ScreenSize( ::GetSystemMetrics( SM_CXSCREEN ),
                     ::GetSystemMetrics( SM_CYSCREEN ) );
   if ( m_WindowRect.right > ScreenSize.cx )
      m_WindowRect.OffsetRect( -(m_WindowRect.right - ScreenSize.cx), 0 );

   // Too far left?
   if ( m_WindowRect.left < 0 )
      m_WindowRect.OffsetRect( -m_WindowRect.left, 0 );

   // Bottom falling out of screen?
   if ( m_WindowRect.bottom > ScreenSize.cy )
   {
      CRect ParentRect;
      m_pParent->GetWindowRect( ParentRect );
      m_WindowRect.OffsetRect( 0,
                               -(ParentRect.Height( ) +
                                                m_WindowRect.Height( )) );
   }

   // Set the window size and position.
   MoveWindow( m_WindowRect, TRUE );
}

void
ZClrPopup::CreateToolTips( )
{
   // Create the tool tip.
   if ( m_ToolTip.Create( this ) == 0 )
      return;

   // Add a tool for each cell.
   for ( zSHORT k = 0; k < m_nNbrColors; k++ )
   {
      CRect rect;
      if ( GetCellRect( k, rect ) == 0 )
         continue;

      m_ToolTip.AddTool( this, GetColorName( k ), rect, k + 1 );
   }
}

void
ZClrPopup::ChangeSelection( zLONG lIndex )
{
   CClientDC dc( this );      // device context for drawing

   if ( lIndex > m_nNbrColors )
      lIndex = CUSTOM_BOX_VALUE;

   if ( (m_lCurrentSel >= 0 && m_lCurrentSel < m_nNbrColors) ||
        m_lCurrentSel == CUSTOM_BOX_VALUE ||
        m_lCurrentSel == DEFAULT_BOX_VALUE )
   {
       // Set Current selection as invalid and redraw old selection (this way
       // the old selection will be drawn unselected).
       zLONG lOldSel = m_lCurrentSel;
       m_lCurrentSel = INVALID_COLOR;
       DrawCell( &dc, lOldSel );
   }

   // Set the current selection as row/col and draw (it will be drawn
   // selected).
   m_lCurrentSel = lIndex;
   DrawCell( &dc, m_lCurrentSel );

   // Store the current color.
   if ( m_lCurrentSel == CUSTOM_BOX_VALUE )
       m_pParent->SendMessage( CPN_SELCHANGE, 0, (LPARAM) m_clrInitialColor );
   else
   if ( m_lCurrentSel == DEFAULT_BOX_VALUE )
   {
      m_clrColor = CLR_DEFAULT;
      m_pParent->SendMessage( CPN_SELCHANGE, 0, (LPARAM) CLR_DEFAULT );
   }
   else
   {
      m_clrColor = GetColor( m_lCurrentSel );
      m_pParent->SendMessage( CPN_SELCHANGE, 0, (LPARAM) m_clrColor );
   }
}

void
ZClrPopup::EndSelection( zSHORT nMessage )
{
   ReleaseCapture( );

   // If custom text selected, perform a custom color selection.
   if ( nMessage != CPN_SELENDCANCEL && m_lCurrentSel == CUSTOM_BOX_VALUE )
   {
      CColorDialog dlg( m_clrInitialColor, CC_FULLOPEN | CC_ANYCOLOR, this );

      if ( dlg.DoModal( ) == IDOK )
         m_clrColor = dlg.GetColor( );
      else
         nMessage = CPN_SELENDCANCEL;
   }

   if ( nMessage == CPN_SELENDCANCEL )
      m_clrColor = m_clrInitialColor;

   m_pParent->SendMessage( nMessage, 0, (LPARAM) m_clrColor );

   DestroyWindow( );
}

void
ZClrPopup::DrawCell( CDC *pDC, zLONG lIndex )
{
   // For the Custom Text area.
   if ( m_csCustomText.GetLength( ) && lIndex == CUSTOM_BOX_VALUE )
   {
      // The extent of the actual text button.
      CRect rectTextButton = m_CustomTextRect;
      rectTextButton.top += 2 * m_nMargin;

      // Fill background.
      pDC->FillSolidRect( rectTextButton, ::GetSysColor( COLOR_3DFACE ) );

      // Draw horizontal line.
      pDC->FillSolidRect( m_CustomTextRect.left + 2 * m_nMargin,
                          m_CustomTextRect.top,
                          m_CustomTextRect.Width( ) - 4 * m_nMargin, 1,
                          ::GetSysColor( COLOR_3DSHADOW ) );
      pDC->FillSolidRect( m_CustomTextRect.left + 2 * m_nMargin,
                          m_CustomTextRect.top + 1,
                          m_CustomTextRect.Width( ) - 4 * m_nMargin, 1,
                          ::GetSysColor( COLOR_3DHILIGHT ) );

      rectTextButton.DeflateRect( 1, 1 );

      // Fill background.
      if ( m_lChosenColorSel == lIndex && m_lCurrentSel != lIndex )
         pDC->FillSolidRect( rectTextButton, ::GetSysColor( COLOR_3DLIGHT ) );
      else
         pDC->FillSolidRect( rectTextButton, ::GetSysColor( COLOR_3DFACE ) );

      // Draw button.
      if ( m_lCurrentSel == lIndex )
         pDC->DrawEdge( rectTextButton, BDR_RAISEDINNER, BF_RECT );
      else
      if ( m_lChosenColorSel == lIndex )
         pDC->DrawEdge( rectTextButton, BDR_SUNKENOUTER, BF_RECT );

      // Draw custom text.
      CFont *pOldFont = (CFont *) pDC->SelectObject( &m_Font );
      int nOldMode = pDC->SetBkMode( TRANSPARENT );
      pDC->DrawText( m_csCustomText, rectTextButton,
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE );
      pDC->SetBkMode( nOldMode );
      pDC->SelectObject( pOldFont );
      return;
   }

   // For the Default Text area.
   if ( m_csDefaultText.GetLength( ) && lIndex == DEFAULT_BOX_VALUE )
   {
      // Fill background.
      pDC->FillSolidRect( m_DefaultTextRect, ::GetSysColor( COLOR_3DFACE ) );

      // The extent of the actual text button.
      CRect rectTextButton = m_DefaultTextRect;
      rectTextButton.DeflateRect( 1, 1 );

      // fill background.
      if ( m_lChosenColorSel == lIndex && m_lCurrentSel != lIndex )
         pDC->FillSolidRect( rectTextButton, ::GetSysColor( COLOR_3DLIGHT ) );
      else
         pDC->FillSolidRect( rectTextButton, ::GetSysColor( COLOR_3DFACE ) );

      // Draw thin line around text.
      CRect LineRect = rectTextButton;
      LineRect.DeflateRect( 2 * m_nMargin, 2 * m_nMargin );
      CPen pen( PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW ) );
      CPen *pOldPen = pDC->SelectObject( &pen );
      pDC->SelectStockObject( NULL_BRUSH );
      pDC->Rectangle( LineRect );
      pDC->SelectObject( pOldPen );

      // Draw button.
      if ( m_lCurrentSel == lIndex )
         pDC->DrawEdge( rectTextButton, BDR_RAISEDINNER, BF_RECT );
      else
      if ( m_lChosenColorSel == lIndex )
         pDC->DrawEdge( rectTextButton, BDR_SUNKENOUTER, BF_RECT );

      // Draw custom text.
      CFont *pOldFont = (CFont *) pDC->SelectObject( &m_Font );
      int nOldMode = pDC->SetBkMode( TRANSPARENT );
      pDC->DrawText( m_csDefaultText, rectTextButton,
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE );
      pDC->SetBkMode( nOldMode );
      pDC->SelectObject( pOldFont );

      return;
   }

   CRect rect;
   if ( GetCellRect( lIndex, rect ) == FALSE )
      return;

   // Select and realize the palette.
   CPalette *pOldPalette = 0;
   if ( pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE )
   {
      pOldPalette = pDC->SelectPalette( &m_Palette, FALSE );
      pDC->RealizePalette( );
   }

   // Fill background.
   if ( m_lChosenColorSel == lIndex && m_lCurrentSel != lIndex )
      pDC->FillSolidRect( rect, ::GetSysColor( COLOR_3DHILIGHT ) );
   else
      pDC->FillSolidRect( rect, ::GetSysColor( COLOR_3DFACE ) );

   // Draw button.
   if ( m_lCurrentSel == lIndex )
      pDC->DrawEdge( rect, BDR_RAISEDINNER, BF_RECT );
   else
   if ( m_lChosenColorSel == lIndex )
      pDC->DrawEdge( rect, BDR_SUNKENOUTER, BF_RECT );

   CBrush brush( PALETTERGB( GetRValue( GetColor( lIndex ) ),
                             GetGValue( GetColor( lIndex ) ),
                             GetBValue( GetColor( lIndex ) ) ) );
   CPen   pen;
   pen.CreatePen( PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW ) );

   CBrush *pOldBrush = (CBrush *) pDC->SelectObject( &brush );
   CPen *pOldPen = (CPen *) pDC->SelectObject( &pen );

   // Draw the cell color.
   rect.DeflateRect( m_nMargin + 1, m_nMargin+1 );
   pDC->Rectangle( rect );

   // Restore DC and cleanup.
   pDC->SelectObject( pOldBrush );
   pDC->SelectObject( pOldPen );
   brush.DeleteObject( );
   pen.DeleteObject( );

   if ( pOldPalette && pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE )
      pDC->SelectPalette( pOldPalette, FALSE );
}

BOOL
ZClrPopup::OnQueryNewPalette( )
{
   Invalidate( );
   return( CWnd::OnQueryNewPalette( ) );
}

void
ZClrPopup::OnPaletteChanged( CWnd *pFocusWnd )
{
   CWnd::OnPaletteChanged( pFocusWnd );

   if ( pFocusWnd->GetSafeHwnd( ) != GetSafeHwnd( ) )
      Invalidate( );
}

void
ZClrPopup::OnKillFocus( CWnd *pNewWnd )
{
   CWnd::OnKillFocus( pNewWnd );

   ReleaseCapture( );
// DestroyWindow( ); - causes crash when Custom color dialog appears.
}

// KillFocus problem fix suggested by Paul Wilkerson.
void
ZClrPopup::OnActivateApp( BOOL bActive, DWORD hTask )
{
   CWnd::OnActivateApp( bActive, hTask );

   // If Deactivating App, cancel this selection
   if ( bActive == FALSE )
      EndSelection( CPN_SELENDCANCEL );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Label.cpp
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Date of creation  : 2000.07.20
// Last modification : 2000.08.07
/////////////////////////////////////////////////////////////////////////////

#define BEVELLINE_SPACE   6

/////////////////////////////////////////////////////////////////////////////
// class ZTextRotator

ZTextRotator::ZTextRotator( ZText *pText, HDC hSrcDC, zCPCHAR cpcText,
                            zULONG ulHorzAlignment, zULONG ulVertAlignment )
{
   m_pText = pText;
   m_hSrcDC = hSrcDC;
// m_hRotatedMemDC = 0;
   m_hRotatedBitmapMemDC = 0;
   m_ulHorzAlignment = ulHorzAlignment;
   m_ulVertAlignment = ulVertAlignment;
   m_bDisabled = FALSE;
   m_bDrawBevelLine = FALSE;
// m_ulAngle = 0;
   m_csText = cpcText;
   m_clrBackground = ::GetSysColor( COLOR_3DFACE );
   m_clrText = ::GetSysColor( COLOR_WINDOWTEXT );
}

ZTextRotator::~ZTextRotator( )
{
// Clear( );
}

void
ZTextRotator::AngleBlt( HDC hSrcDC, HDC hTgtDC,
                        RECT rectSrc, zULONG ulAngle )
{
   zLONG lSrcWidth = rectSrc.right - rectSrc.left;
   zLONG lSrcHeight = rectSrc.bottom - rectSrc.top;

   if ( ulAngle == 0 )  // Don't need to rotate text
   {
      BitBlt( hTgtDC, 0, 0, lSrcWidth, lSrcHeight,
              hSrcDC, 0, 0, SRCCOPY );
      return;
   }

   if ( m_pText->m_pZSubtask->m_pZTask->m_nOS_Version >= 35 ) // NT/2000/XP
   {
      POINT Points[ 3 ];

      switch ( ulAngle )
      {
         case 90:
            Points[ 0 ].x = lSrcHeight;
            Points[ 0 ].y = 0;
            Points[ 1 ].x = lSrcHeight;
            Points[ 1 ].y = lSrcWidth;
            Points[ 2 ].x = 0;
            Points[ 2 ].y = 0;
            break;

         case 180:
            Points[ 0 ].x = lSrcWidth;
            Points[ 0 ].y = lSrcHeight;
            Points[ 1 ].x = 0;
            Points[ 1 ].y = lSrcHeight;
            Points[ 2 ].x = lSrcWidth;
            Points[ 2 ].y = 0;
            break;

         case 270:
            Points[ 0 ].x = 0;
            Points[ 0 ].y = lSrcWidth;
            Points[ 1 ].x = 0;
            Points[ 1 ].y = 0;
            Points[ 2 ].x = lSrcHeight;
            Points[ 2 ].y = lSrcWidth;
            break;

         default:
            Points[ 0 ].x = 0;
            Points[ 0 ].y = 0;
            Points[ 1 ].x = lSrcWidth;
            Points[ 1 ].y = 0;
            Points[ 2 ].x = 0;
            Points[ 2 ].y = lSrcHeight;
            break;

      } // switch ( uiAngle )

      // Rotate the memory DC (Works only on Windows NT)
      PlgBlt( hTgtDC, (const POINT *) &Points, hSrcDC, 0, 0,
              lSrcWidth, lSrcHeight, 0, 0, 0 );
      return;
   }

   // Windows 95/98.
   int k, j;

   switch ( ulAngle )
   {
      case 90:
         for ( k = 0; k < lSrcWidth; k++ )
         {
            for ( j = 0; j < lSrcHeight; j++ )
            {
               SetPixel( hTgtDC, lSrcHeight - j - 1, k,
                         GetPixel( hSrcDC, k, j ) );
            }
         }

         break;

      case 180:
         for ( k = 0; k < lSrcWidth; k++ )
         {
            for ( j = 0; j < lSrcHeight; j++ )
            {
               SetPixel( hTgtDC, k, j,
                         GetPixel( hSrcDC, lSrcWidth - k - 1,
                                   lSrcHeight - j - 1 ) );
            }
         }

         break;

      case 270:
         for ( k = 0; k < lSrcWidth; k++ )
         {
            for ( j = 0; j < lSrcHeight; j++ )
            {
               SetPixel( hTgtDC, j, k,
                         GetPixel( hSrcDC, lSrcWidth - k - 1, j ) );
            }
         }

         break;

      default:
         for ( k = 0; k < lSrcWidth; k++ )
         {
            for ( j = 0; j < lSrcHeight; j++ )
            {
               SetPixel( hTgtDC, k, j, GetPixel( hSrcDC, k, j ) );
            }
         }

         break;

   } // switch ( ulAngle )
}

#if 0
void
ZTextRotator::Clear( )
{
   if ( m_hRotatedMemDC )
   {
      if ( m_hRotatedBitmapMemDC )
      {
         HBITMAP hEmptyBitmap2 =
            (HBITMAP) ::SelectObject( m_hRotatedMemDC, m_hRotatedBitmapMemDC );
         ::DeleteObject( hEmptyBitmap2 ); // Delete temporary empty bitmap 2
         m_hRotatedBitmapMemDC = 0;
      }

      ::DeleteDC( m_hRotatedMemDC );
      m_hRotatedMemDC = 0;
   }

   m_rectRotated.SetRectEmpty( );
// m_ulAngle = 0;
}
#endif

CSize
ZTextRotator::GetLargestTextSize( HDC hDC, CStringArray *parrayText )
{
   CSize sizeMax( 0, 0 );

   for ( zSHORT k = 0; k < parrayText->GetSize( ); k++ )
   {
      CString cs = parrayText->GetAt( k );
      CSize   size;

      if ( ::GetTextExtentPoint32( hDC, cs, cs.GetLength( ), &size ) == 0 )
         continue;

      if ( sizeMax.cx < size.cx )
         sizeMax.cx = size.cx;

      if ( sizeMax.cy < size.cy )
         sizeMax.cy = size.cy;
   }

   return( sizeMax );
}

zBOOL
ZTextRotator::BitBltText( HDC hRotatedMemDC, CRect rectSrc, zULONG ulAngle )
{
   if ( hRotatedMemDC == 0 )
      return( FALSE );

   CRect rectRotatedSave = m_rectRotated;

   // Change rotated rectangle size to fit in source rectangle.
   if ( rectSrc.Width( ) < m_rectRotated.Width( ) )
      m_rectRotated.right = rectSrc.Width( );

   if ( rectSrc.Height( ) < m_rectRotated.Height( ) )
      m_rectRotated.bottom = rectSrc.Height( );

   // Calculate left/top position to draw the text block.
   int X = 0;
   int Y = 0;

   if ( m_ulHorzAlignment == DT_CENTER )
   {
      X = (rectSrc.Width( ) - m_rectRotated.Width( )) / 2;
   }
   else
   if ( m_ulHorzAlignment == DT_RIGHT )
   {
      X = rectSrc.Width( ) - m_rectRotated.Width( );
   }

   if ( m_ulVertAlignment == DT_VCENTER )       // CENTER
   {
      Y = (rectSrc.Height( ) - m_rectRotated.Height( )) / 2;
   }
   else
   if ( m_ulVertAlignment == DT_BOTTOM )        // BOTTOM
   {
      Y = rectSrc.Height( ) - m_rectRotated.Height( );
   }

   zBOOL bRC = BitBlt( m_hSrcDC, X, Y, m_rectRotated.Width( ),
                       m_rectRotated.Height( ), hRotatedMemDC,
                       0, 0, SRCCOPY );

   if ( m_bDrawBevelLine ) // DRAW BEVEL LINES (if requested)
   {
      RECT Rect;

      if ( ulAngle == 0 || ulAngle == 180 )
      {
         if ( m_ulVertAlignment == DT_TOP )     // TOP
         {
            Rect.top = Rect.bottom = m_rectRotated.Height( ) / 2;
         }
         else
         if ( m_ulVertAlignment == DT_VCENTER ) // CENTER
         {
            Rect.top = Rect.bottom = rectSrc.Height( ) / 2;
         }
         else
         if ( m_ulVertAlignment == DT_BOTTOM )  // BOTTOM
         {
            Rect.top = Rect.bottom =
                     rectSrc.Height( ) - m_rectRotated.Height( ) / 2;
         }

         if ( m_ulHorzAlignment == DT_LEFT )
         {
            Rect.left = m_rectRotated.Width( ) + BEVELLINE_SPACE;
            Rect.right = rectSrc.Width( ) - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_TOP );
         }
         else
         if ( m_ulHorzAlignment == DT_CENTER )
         {
            Rect.left = BEVELLINE_SPACE;
            Rect.right = rectSrc.Width( ) / 2 -
                                m_rectRotated.Width( ) / 2 - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_TOP );
            Rect.left = rectSrc.Width( ) / 2 +
                                m_rectRotated.Width( ) / 2 + BEVELLINE_SPACE;
            Rect.right = rectSrc.Width( ) - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_TOP );
         }
         else
         if ( m_ulHorzAlignment == DT_RIGHT )
         {
            Rect.left = BEVELLINE_SPACE;
            Rect.right = rectSrc.Width( ) -
                                    m_rectRotated.Width( ) - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_TOP );
         }
      }
      else
      {
         if ( m_ulHorzAlignment == DT_LEFT )
         {
            Rect.left = Rect.right = m_rectRotated.Width( ) / 2;
         }
         else
         if ( m_ulHorzAlignment == DT_CENTER )
         {
            Rect.left = Rect.right = rectSrc.Width( ) / 2;
         }
         else
         if ( m_ulHorzAlignment == DT_RIGHT )
         {
            Rect.left = Rect.right =
                               rectSrc.Width( ) - m_rectRotated.Width( ) / 2;
         }

         if ( m_ulVertAlignment == DT_TOP )     // TOP
         {
            Rect.top = m_rectRotated.Height( ) + BEVELLINE_SPACE;
            Rect.bottom = rectSrc.Height( ) - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_LEFT );
         }
         else
         if ( m_ulVertAlignment == DT_VCENTER ) // CENTER
         {
            Rect.top = BEVELLINE_SPACE;
            Rect.bottom = rectSrc.Height( ) / 2 -
                               m_rectRotated.Height( ) / 2 - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_LEFT );
            Rect.top = rectSrc.Height( ) / 2 +
                               m_rectRotated.Height( ) / 2 + BEVELLINE_SPACE;
            Rect.bottom = rectSrc.Height( ) - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_LEFT );
         }
         else
         if ( m_ulVertAlignment == DT_BOTTOM )  // BOTTOM
         {
            Rect.top = BEVELLINE_SPACE;
            Rect.bottom = rectSrc.Height( ) -
                                   m_rectRotated.Height( ) - BEVELLINE_SPACE;
            DrawEdge( m_hSrcDC, &Rect, EDGE_ETCHED, BF_LEFT );
         }
      }
   }

   m_rectRotated = rectRotatedSave;
   return( bRC );
}

// void
// ZTextRotator::GetRotatedDC( CDC *pDC )
// {
//    if ( pDC )
//       pDC->m_hDC = m_hRotatedMemDC;
// }

// void
// ZTextRotator::GetRotatedRect( RECT *pRect )
// {
//    RECT rect = m_rectRotated;
//
//    if ( pRect )
//       zmemcpy( pRect, &rect, zsizeof( RECT ) );
// }

//
// DT_LEFT (0)    Aligns text flush-left.
// DT_CENTER (1)  Centers text horizontally.
// DT_RIGHT (2)   Aligns text flush-right.
// DT_TOP (0)     Specifies top-justified text (single line only).
// DT_VCENTER (4) Specifies vertically centered text (single line only).
// DT_BOTTOM (8)  Specifies bottom-justified text. This value must be
//                combined with DT_SINGLELINE.
// DT_SINGLELINE  Specifies single line only. Carriage returns and linefeeds
//         (0x20) do not break the line.
// DT_WORDBREAK   Specifies word-breaking. Lines are automatically broken
//         (0x10) between words if a word would extend past the edge of the
//                rectangle specified by lpRect. A carriage return/linefeed
//                sequence will also break the line.
//
HDC
ZTextRotator::RotateText( CRect& rectClient, zULONG ulAngle )
{
// Clear( ); // Reset all previous memory DC's

// m_ulAngle = ulAngle;

   zLONG lLth = m_csText.GetLength( ) + 1;
   zPCHAR pchText = new char[ lLth ];
   if ( pchText == 0 )
      return( 0 );

   // Split text into a list of text lines (for multiline support).
   CStringArray arrayText;
   CRect  rectFull;
   CSize  size;
   CSize  sizeExtent;
   zLONG  lLineCnt = 0;
   zPCHAR pchNewLine;
   zPCHAR pchSpace;
   zPCHAR pch;
   zCHAR  ch;

   if ( ulAngle == 90 || ulAngle == 270 )
   {
      size.cx = rectClient.Height( );
      size.cy = rectClient.Width( );
   }
   else
   {
      size.cx = rectClient.Width( );
      size.cy = rectClient.Height( );
   }

   strcpy_s( pchText, lLth, m_csText );
   pch = pchText;
   while ( pch && *pch )
   {
      if ( (pchNewLine = zstrchr( pch, '\n' )) != 0 )
         *pchNewLine = 0;

      lLth = zstrlen( pch );
      ch = 0;
      while ( lLth > 0 )
      {
         sizeExtent = GetTabbedTextExtent( m_hSrcDC, pch, lLth, 0, 0 );
      // ::GetTextExtentPoint32( m_hSrcDC, pch, lLth, &sizeExtent );
         if ( lLth <= 1 || sizeExtent.cx < size.cx )
         {
            if ( ch && ch != ' ' )
            {
               pchSpace = zstrrchr( pch, ' ' );
               if ( pchSpace )
               {
                  pch[ lLth ] = ch;
                  *pchSpace = 0;
                  lLth = pchSpace - pch;
                  ch = ' ';
               }
            }

            lLineCnt++;
            arrayText.Add( pch );
            pch[ lLth ] = ch;
            pch = pch + lLth;
            while ( *pch == ' ' )
               pch++;

            lLth = zstrlen( pch );
            ch = 0;
         }
         else
         {
            pch[ lLth ] = ch;
            lLth--;
            ch = pch[ lLth ];
            pch[ lLth ] = 0;
         }
      }

      pch[ lLth ] = ch;
      if ( pchNewLine )
         pch = pchNewLine + 1;
   }

#if 0
   pch = strtok( pchText, "\n" );
   while ( pch )
   {
      arrayText.Add( pch );
      lLineCnt++;
      pch = strtok( 0, "\n" );
   }
#endif

   delete( pchText );

   // Calculate the size of the lengthy text of the text list.
   CSize sizeText = GetLargestTextSize( m_hSrcDC, &arrayText );

   // Set Full rectangle size.
   rectFull.left = 0;
   rectFull.top = 0;
   rectFull.right = sizeText.cx;
   rectFull.bottom = sizeText.cy * lLineCnt;

   // Create an empty memory DC for rotation operation.
   zLONG lMax = rectFull.Width( );

   if ( lMax < rectFull.Height( ) )
      lMax = rectFull.Height( );

#if 0

   if ( m_pText->m_hEmptyBitmap1 == 0 )
   {
      m_pText->m_hEmptyBitmap1 = CreateCompatibleBitmap( m_hSrcDC,
                                                         lMax, lMax );
      if ( m_pText->m_hEmptyBitmap1 == 0 )
         return( 0 );
   }

   if ( m_pText->m_hEmptyBitmap2 == 0 )
   {
      m_pText->m_hEmptyBitmap2 = CreateCompatibleBitmap( m_hSrcDC,
                                                         lMax, lMax );
      if ( m_pText->m_hEmptyBitmap2 == 0 )
         return( 0 );
   }

   HDC hMemDC = CreateCompatibleDC( m_hSrcDC );
   if ( hMemDC == 0 )
      return( 0 );

#else

   // If an application sets the nWidth or nHeight parameters to zero,
   // CreateCompatibleBitmap returns the handle to a 1-by-1 pixel,
   // monochrome bitmap.  This in turn is the same bitmap originally
   // selected into hMemDC, so BoundsChecker complains that a resource
   // is still selected into the DC.  To prevent BoundsCheck from
   // complaining, if lMax is 0, we will force lMax to be 1 ... which
   // causes a "real" bitmap to be created.
   if ( lMax == 0 )
      lMax = 1;

   HBITMAP hEmptyBitmap1 = CreateCompatibleBitmap( m_hSrcDC, lMax, lMax );
   if ( hEmptyBitmap1 == 0 )
      return( 0 );

   HBITMAP hEmptyBitmap2 = CreateCompatibleBitmap( m_hSrcDC, lMax, lMax );
   if ( hEmptyBitmap2 == 0 )
   {
      DeleteObject( hEmptyBitmap1 );
   // m_pText->m_hEmptyBitmap1 = 0;
      return( 0 );
   }

   HDC hMemDC = CreateCompatibleDC( m_hSrcDC );
   if ( hMemDC == 0 )
   {
      DeleteObject( hEmptyBitmap1 );
   // m_pText->m_hEmptyBitmap1 = 0;
      DeleteObject( hEmptyBitmap2 );
      return( 0 );
   }

#endif

   HDC hRotatedMemDC = CreateCompatibleDC( m_hSrcDC );
   if ( hRotatedMemDC == 0 )
   {
   // ::DeleteObject( hRotatedMemDC );
      ::DeleteDC( hMemDC );
      ::DeleteObject( hEmptyBitmap2 );
      ::DeleteObject( hEmptyBitmap1 );
   // m_pText->m_hEmptyBitmap1 = 0;
      return( 0 );
   }

   // Put the empty bitmap in memory DC.
   HBITMAP hBitmapMemDC = (HBITMAP) ::SelectObject( hMemDC, hEmptyBitmap1 );

   // Put the empty bitmap in rotated memory DC.
   m_hRotatedBitmapMemDC = (HBITMAP) ::SelectObject( hRotatedMemDC,
                                                     hEmptyBitmap2 );

   // Select same font as source DC.
   HFONT hOldFont = (HFONT) ::SelectObject( hMemDC,
                                            GetCurrentObject( m_hSrcDC,
                                                              OBJ_FONT ) );

   int nOldMode = ::SetBkMode( hMemDC, TRANSPARENT );

   HBRUSH hBrush = CreateSolidBrush( m_clrBackground );
   HBRUSH hBrushMemDC = (HBRUSH) ::SelectObject( hMemDC, hBrush );
   ::FillRect( hMemDC, rectFull, hBrush );
   ::FillRect( hRotatedMemDC, rectFull, hBrush );
   COLORREF clrOld = ::SetTextColor( hMemDC, m_clrText );

   // Draw the list of text into memory DC.
   for ( zLONG k = 0; k < arrayText.GetSize( ); k++ )
   {
      RECT Rect;

      Rect.left = 0;
      Rect.top = k * sizeText.cy;
      Rect.right = Rect.left + sizeText.cx;
      Rect.bottom = Rect.top + sizeText.cy;

      if ( m_bDisabled )
      {
         CRect rect = Rect;

         ::SetTextColor( hMemDC, ::GetSysColor( COLOR_3DHIGHLIGHT ) );
         rect.left += 1;
         rect.top += 1;
         rect.right += 1;
         rect.bottom += 1;

         ::DrawText( hMemDC, arrayText.GetAt( k ), -1, &rect,
                     DT_TOP | DT_LEFT | DT_WORDBREAK | DT_EXPANDTABS | m_ulHorzAlignment );
      //             DT_SINGLELINE | DT_VCENTER | m_ulHorzAlignment );

         ::SetTextColor( hMemDC, ::GetSysColor( COLOR_3DSHADOW ) );
      }

      ::DrawText( hMemDC, arrayText.GetAt( k ), -1, &Rect,
                  DT_TOP | DT_LEFT | DT_WORDBREAK | DT_EXPANDTABS | m_ulHorzAlignment );
               // DT_SINGLELINE | DT_VCENTER | m_ulHorzAlignment );
   }

   AngleBlt( hMemDC, hRotatedMemDC, rectFull, ulAngle );

   // Reset DC's.
   ::SelectObject( hMemDC, hOldFont );
   ::SelectObject( hMemDC, hBrushMemDC );
   ::SelectObject( hMemDC, hBitmapMemDC );
   ::DeleteObject( hEmptyBitmap1 ); // Delete temporary empty bitmap 1
   ::SetTextColor( hMemDC, clrOld );
   ::SetBkMode( hMemDC, nOldMode );
   ::DeleteDC( hMemDC );

// m_pText->m_hEmptyBitmap1 = 0;
   ::DeleteObject( hBrush );

   m_rectRotated.left = 0;
   m_rectRotated.top = 0;
   if ( ulAngle == 90 || ulAngle == 270 )
   {
      m_rectRotated.right = rectFull.Height( );
      m_rectRotated.bottom = rectFull.Width( );
   }
   else
   {
      m_rectRotated.right = rectFull.Width( );
      m_rectRotated.bottom = rectFull.Height( );
   }

   return( hRotatedMemDC );
}

// HDC
// ZTextRotator::GetRotatedDC( )
// {
//    return( m_hRotatedMemDC );
// }

// CRect
// ZTextRotator::GetRotatedRect( )
// {
//    return( m_rectRotated );
// }

void
ZTextRotator::DrawBevelLine( zBOOL bDraw )
{
   m_bDrawBevelLine = bDraw;
}

void
ZTextRotator::SetBackgroundColor( COLORREF clr )
{
   m_clrBackground = clr;
}

void
ZTextRotator::SetDisabledText( zBOOL bDisable )
{
   m_bDisabled = bDisable;
}

void
ZTextRotator::SetHorzAlignment( zULONG ulHorzAlignment )
{
   m_ulHorzAlignment = ulHorzAlignment;
}

void
ZTextRotator::SetText( zCPCHAR cpcText )
{
   m_csText = cpcText;
}

void
ZTextRotator::SetTextColor( COLORREF clr )
{
   m_clrText = clr;
}

void
ZTextRotator::SetVertAlignment( zULONG ulVertAlignment )
{
   m_ulVertAlignment = ulVertAlignment;
}

/////////////////////////////////////////////////////////////////////////////

#define CONTROLTOOLTIP_CLASSNAME  "ZeidonToolTip"

/////////////////////////////////////////////////////////////////////////////
// CControlToolTip

CControlToolTip::CControlToolTip( )
{
   m_pControl = 0;
   m_iPosition = 0;
   m_clrText = ::GetSysColor( COLOR_INFOTEXT );
   m_clrBkgnd = ::GetSysColor( COLOR_INFOBK );
   m_bUseBgndColor = FALSE;
   m_bShowOnlyOnMouseClickDown = TRUE;
   m_bUseLeftButton = TRUE;

   RegisterWindowClass( ); // Register window class if not already registered.
}

CControlToolTip::~CControlToolTip( )
{
   Detach( );
}

CSize
CControlToolTip::GetMultiLineTextExtent( CClientDC *pDC,
                                         zCPCHAR cpcText )
{
   CSize sizeExtent = pDC->GetTextExtent( cpcText );
   zLONG lLineCnt = GetNumberOfLines( cpcText );

   if ( lLineCnt == 1 )
      return( sizeExtent );

   sizeExtent.cx = 0;
   sizeExtent.cy *= lLineCnt;

   zCHAR  szTemp[ 1024 ];
   CSize size;

   strcpy_s( szTemp, zsizeof( szTemp ), cpcText );
   zPCHAR pch = strtok( szTemp, "\n" );

   while ( pch )
   {
      size = pDC->GetTextExtent( pch );
      if ( sizeExtent.cx < size.cx )
         sizeExtent.cx = size.cx;

      pch = strtok( 0, "\n" );
   }

   return( sizeExtent );
}

int
CControlToolTip::GetNumberOfLines( zCPCHAR cpcText )
{
   int     iLines = 1;
   CString str = cpcText ? cpcText : m_csText;

   for ( int k = 0; k < str.GetLength( ); k++ )
   {
      if ( str[ k ] == '\n' )
         iLines++;
   }

   return( iLines );
}

zBOOL
CControlToolTip::IsMouseInControl( )
{
   if ( m_pControl == 0 || mIs_hWnd( m_pControl->m_hWnd ) )
      return( FALSE );

   CRect rect;

   m_pControl->GetWindowRect( &rect );

   POINT stPoint;

   GetCursorPos( &stPoint ); // Get Mouse Position (in screen coordinates)

   if ( rect.left <= stPoint.x && stPoint.x <= rect.right &&
        rect.top  <= stPoint.y && stPoint.y <= rect.bottom )
   {
      return( TRUE );
   }

   return( FALSE );
}

void
CControlToolTip::Show( )
{
   if ( m_csText.IsEmpty( ) )
   {
      ShowWindow( SW_HIDE );
      return;
   }

   CRect rectControl;

   m_pControl->GetClientRect( &rectControl );

   CPoint pt( 0, 0 );

   if ( m_iPosition == 1 )
      pt.y = rectControl.Height( );

   if ( m_iPosition == 2 )
      pt.y = -m_rectText.Height( );

   m_pControl->ClientToScreen( &pt );
   SetWindowPos( &wndTop, pt.x, pt.y, m_rectText.Width( ),
                 m_rectText.Height( ), SWP_SHOWWINDOW | SWP_NOACTIVATE );
   RedrawWindow( );
}

void
CControlToolTip::RegisterWindowClass( )
{
   WNDCLASS  WndClass;
   HINSTANCE hInst = AfxGetInstanceHandle( );

   if ( (::GetClassInfo( hInst, CONTROLTOOLTIP_CLASSNAME, &WndClass )) == 0 )
   {
      WndClass.style = CS_SAVEBITS | CS_DBLCLKS;
      WndClass.lpfnWndProc = ::DefWindowProc;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 0;
      WndClass.hInstance = hInst;
      WndClass.hIcon = 0;
      WndClass.hCursor = LoadCursor( hInst, IDC_ARROW );
      WndClass.hbrBackground = (HBRUSH) (COLOR_INFOBK + 1);
      WndClass.lpszMenuName = 0;
      WndClass.lpszClassName = CONTROLTOOLTIP_CLASSNAME;

      if ( AfxRegisterClass( &WndClass ) == 0 )
         AfxThrowResourceException( );
   }
}

zBOOL
CControlToolTip::Attach( CWnd *pControl )
{
   if ( m_hWnd )
   {
      if ( Detach( ) == FALSE )
         return( FALSE ); // Failed to detach preview control
   }

   m_pControl = pControl;
   zBOOL bRC = CreateEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
                         CONTROLTOOLTIP_CLASSNAME, 0,
                         WS_BORDER | WS_POPUP, 0, 0, 0, 0, 0, 0, 0 );
  if ( bRC == FALSE )
   return( FALSE );

  SetText( m_csText );
  SetTimer( 1, 100, 0 );
  return( TRUE );
}

zBOOL
CControlToolTip::Detach( )
{
   if ( m_hWnd == 0 )
      return( TRUE );

   ShowWindow( SW_HIDE );
   KillTimer( 1 );
   m_pControl = 0;
   return( DestroyWindow( ) );
}

void
CControlToolTip::SetBkgndColor( zBOOL bUse, COLORREF clr )
{
   m_bUseBgndColor = bUse;
   if ( bUse )
      m_clrBkgnd = clr;

   if ( mIs_hWndVisible( m_hWnd ) == FALSE )
      return;

  RedrawWindow( );
}

void
CControlToolTip::SetPosition( int iPosition )
{
   if ( iPosition < 0 || iPosition > 2 )
      return;

   m_iPosition = iPosition;

   if ( m_hWnd == 0 )
      return;

   if ( mIs_hWndVisible( m_hWnd ) == FALSE )
      return;

   Show( );
}

void
CControlToolTip::SetText( zCPCHAR cpcText )
{
   m_csText = cpcText;
   if ( m_hWnd == 0 )
      return;

   CClientDC dc( this );
   int       iSaveDC = dc.SaveDC( );

   dc.SelectObject( (HFONT) GetStockObject( DEFAULT_GUI_FONT ) );
   CSize sizeText = GetMultiLineTextExtent( &dc, m_csText );
   dc.RestoreDC( iSaveDC );

   m_rectText.left = 0;
   m_rectText.top = 0;
   m_rectText.right = m_rectText.left + sizeText.cx + 4;
   m_rectText.bottom = m_rectText.top  + sizeText.cy + 4;

   if ( mIs_hWndVisible( m_hWnd ) == FALSE )
      return;

   Show( );
}

void
CControlToolTip::SetTextColor( COLORREF clr )
{
   m_clrText = clr;

   if ( m_bUseBgndColor == FALSE || mIs_hWndVisible( m_hWnd ) == FALSE )
      return;

   RedrawWindow( );
}

void
CControlToolTip::ShowOnlyOnMouseClickDown( zBOOL bShow )
{
   m_bShowOnlyOnMouseClickDown = bShow;
}

void
CControlToolTip::UseLeftMouseButton( zBOOL bUse )
{
  m_bUseLeftButton = bUse;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP( CControlToolTip, CWnd )
   //{{AFX_MSG_MAP( CControlToolTip )
   ON_WM_TIMER( )
   ON_WM_PAINT( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// CControlToolTip message handlers

void
CControlToolTip::OnTimer( UINT nIDEvent )
{
   if ( nIDEvent == 1 && m_pControl )
   {
      CWnd *pFocusWnd = GetFocus( );

      if ( pFocusWnd )
         pFocusWnd = pFocusWnd->GetParent( );

      zBOOL bClick = TRUE;

      if ( m_bShowOnlyOnMouseClickDown )
      {
         zSHORT nVal;

         nVal = GetKeyState( m_bUseLeftButton ? VK_LBUTTON : VK_RBUTTON );
         bClick = (nVal >> 1) != 0;
      }

      if ( IsMouseInControl( ) && bClick )
      {
         CWnd *pParent = m_pControl->GetParent( );
         zBOOL bVisible = mIs_hWndVisible( m_hWnd );

         if ( bVisible == FALSE && pParent == pFocusWnd )
            Show( );

         if ( bVisible && pParent != pFocusWnd )
            ShowWindow( SW_HIDE );
      }
      else
      {
         if ( mIs_hWndVisible( m_hWnd ) )
            ShowWindow( SW_HIDE );
      }
   }

   CWnd::OnTimer( nIDEvent );
}

void
CControlToolTip::OnPaint( )
{
   COLORREF clrOld = CLR_INVALID;
   CPaintDC dc( this ); // device context for painting

   HFONT hOldFont = (HFONT)
               dc.SelectObject( (HFONT) GetStockObject( DEFAULT_GUI_FONT ) );
   int nOldMode = dc.SetBkMode( TRANSPARENT );
   if ( m_bUseBgndColor )
   {
      dc.FillSolidRect( m_rectText, m_clrBkgnd );
      clrOld = dc.SetTextColor( m_clrText );
   }

   dc.SetBkMode( nOldMode );
   dc.DrawText( m_csText, -1, m_rectText, DT_LEFT );
   if ( clrOld != CLR_INVALID )
      dc.SetTextColor( clrOld );

   dc.SelectObject( hOldFont );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
Text( ZSubtask *pZSubtask,
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
      if ( pCtrlDef->Tag[ 2 ] == 'C' &&
           pCtrlDef->Tag[ 3 ] == 'o' &&
           pCtrlDef->Tag[ 4 ] == 'l' &&
           pCtrlDef->Tag[ 5 ] == 'o' &&
           pCtrlDef->Tag[ 6 ] == 'r' )
      {
         return( new ZClrPicker( pZSubtask, pWndParent,
                                 pzmaComposite, vDialog,
                                 nOffsetX, nOffsetY, pCtrlDef ) );
      }
   }

   return( new ZText( pZSubtask, pWndParent,
                      pzmaComposite, vDialog,
                      nOffsetX, nOffsetY, pCtrlDef ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TX_SetTextProperties( zVIEW      vSubtask,
                      zCPCHAR    cpcCtrlTag,
                      LABEL_DATA *pLD )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZText *pText = DYNAMIC_DOWNCAST( ZText, pzma->m_pCtrl );
      if ( pText )
      {
         pText->SetRedrawLocal( FALSE );
         pText->m_lType = pLD->lType;
         if ( pLD->chBold )
         {
            if ( (pText->m_fTextFlags & zTEXT_BOLD) == 0 )
            {
               pText->m_fTextFlags |= zTEXT_BOLD;
               pText->SetFontBold( TRUE );
            }
         }
         else
         {
            if ( pText->m_fTextFlags & zTEXT_BOLD )
            {
               pText->m_fTextFlags &= ~zTEXT_BOLD;
               pText->SetFontBold( FALSE );
            }
         }

         if ( pLD->chItalic )
         {
            if ( (pText->m_fTextFlags & zTEXT_ITALIC) == 0 )
            {
               pText->m_fTextFlags |= zTEXT_ITALIC;
               pText->SetFontItalic( TRUE );
            }
         }
         else
         {
            if ( pText->m_fTextFlags & zTEXT_ITALIC )
            {
               pText->m_fTextFlags &= ~zTEXT_ITALIC;
               pText->SetFontItalic( FALSE );
            }
         }

         if ( pLD->chStrikeout )
         {
            if ( (pText->m_fTextFlags & zTEXT_STRIKEOUT) == 0 )
            {
               pText->m_fTextFlags |= zTEXT_STRIKEOUT;
               pText->SetFontStrikeout( TRUE );
            }
         }
         else
         {
            if ( pText->m_fTextFlags & zTEXT_STRIKEOUT )
            {
               pText->m_fTextFlags &= ~zTEXT_STRIKEOUT;
               pText->SetFontStrikeout( FALSE );
            }
         }

         if ( pLD->chUnderline )
         {
            if ( (pText->m_fTextFlags & zTEXT_UNDERLINE) == 0 )
            {
               pText->m_fTextFlags |= zTEXT_UNDERLINE;
               pText->SetFontUnderline( TRUE );
            }
         }
         else
         {
            if ( pText->m_fTextFlags & zTEXT_UNDERLINE )
            {
               pText->m_fTextFlags &= ~zTEXT_UNDERLINE;
               pText->SetFontUnderline( FALSE );
            }
         }

         if ( pLD->lPointSize && pText->m_lSize != pLD->lPointSize )
         {
            pText->m_lSize = pLD->lPointSize;
            pText->SetFontSize( pText->m_lSize );
         }

         COLORREF clr;

         if ( pLD->lTextColor == 0xFF000000 )
            clr = CLR_INVALID;
         else
         if ( pLD->lTextColor & 0xFF000000 )
            clr = ::GetSysColor( pLD->lTextColor );
         else
            clr = pLD->lTextColor;

         if ( clr != CLR_INVALID )
         {
            if ( pText->m_clrTextForeColor != clr )
            {
               pText->m_clrSaveTextColor = clr;
               pText->SetTextColor( clr );
            }
         }
         else
         {
            pText->SetDefaultTextColor( );
         }

         zULONG ulAlign;
         if ( pLD->chBottomRight & 1 )
            ulAlign = ZText::HORZ_RIGHT;
         else
         if ( pLD->chCenter & 1 )
            ulAlign = ZText::HORZ_CENTER;
         else
//       if ( pLD->chTopLeft & 1 )
            ulAlign = ZText::HORZ_LEFT;

         if ( pLD->chBottomRight & 2 )
            ulAlign |= ZText::VERT_BOTTOM;
         else
         if ( pLD->chCenter & 2 )
            ulAlign |= ZText::VERT_CENTER;
         else
      // if ( pLD->chTopLeft & 2 )
            ulAlign |= ZText::VERT_TOP;

         pText->SetAlignment( ulAlign );

         if ( zstrcmp( pText->m_szFaceName, pLD->szFontName ) )
         {
            strcpy_s( pText->m_szFaceName, zsizeof( pText->m_szFaceName ), pLD->szFontName );
            pText->SetFontName( pText->m_szFaceName );
         }

         if ( pText->m_lEscapement != pLD->lEscapement )
         {
            pText->m_lEscapement = pLD->lEscapement;
            while ( pText->m_lEscapement < 0 )
               pText->m_lEscapement += 3600;

            while ( pText->m_lEscapement >= 3600 )
               pText->m_lEscapement -= 3600;

            pText->SetAngle( pText->m_lEscapement );
         }

         if ( pLD->lBackgroundColor == 0xFF000000 )
            clr = CLR_INVALID;
         else
         if ( pLD->lBackgroundColor & 0xFF000000 )
            clr = ::GetSysColor( pLD->lBackgroundColor );
         else
            clr = pLD->lBackgroundColor;

         if ( clr != CLR_INVALID )
         {
            if ( pText->m_clrBackColor != clr )
            {
               DeleteObject( pText->m_hBackBrush );
               pText->m_hBackBrush = ::CreateSolidBrush( clr );
               pText->SetBkColor( clr );
            }
         }
         else
         {
            DeleteObject( pText->m_hBackBrush );
            pText->m_hBackBrush = 0;
            pText->SetDefaultBkColor( );
         }

         if ( pLD->lTextBkColor == 0xFF000000 )
            clr = CLR_INVALID;
         else
         if ( pLD->lTextBkColor & 0xFF000000 )
            clr = ::GetSysColor( pLD->lTextBkColor );
         else
            clr = pLD->lTextBkColor;

         if ( clr != CLR_INVALID )
         {
            if ( pText->m_clrTextBackColor != clr )
               pText->SetTextBackColor( clr );
         }
         else
         {
            pText->SetDefaultTextBackColor( );
         }

         if ( pLD->lHighlightColor == 0xFF000000 )
            clr = ::GetSysColor( COLOR_HIGHLIGHT );
         else
         if ( pLD->lHighlightColor & 0xFF000000 )
            clr = ::GetSysColor( pLD->lHighlightColor );
         else
            clr = pLD->lHighlightColor;

         if ( pText->m_clrHighLight != clr )
         {
            pText->SetHighLightColor( clr );
         }

         if ( pText->m_bUseBevelLine != pLD->chBeveledLine )
            pText->UseBevelLine( pLD->chBeveledLine );

         if ( pText->m_bVerticalText != pLD->chVerticalText )
         {
            pText->m_bVerticalText = pLD->chVerticalText;
            pText->SetVerticalText( pText->m_bVerticalText );
         }

         if ( pLD->chUseToolTip && pLD->szToolTip[ 0 ] )
            pText->UseToolTip( TRUE );
         else
            pText->UseToolTip( FALSE );

         if ( pLD->chHighlight & 1 )
            pText->m_bUseHighLighting = pLD->chHighlight;

         if ( pLD->chHighlight & 2 )
            pText->m_bUseHighLightingOnlyWithToolTip = TRUE;

         if ( pLD->chBorder & 1 )
         {
            pText->Attr.Style |= WS_BORDER;
            pText->SetBorder( TRUE );
         }
         else
         {
            pText->Attr.Style &= ~WS_BORDER;
            pText->SetBorder( FALSE );
         }

         if ( pLD->chBorder & 2 )
         {
            pText->Attr.StyleEx = WS_EX_STATICEDGE;
            pText->SetSunken( TRUE );
         }
         else
         {
            pText->Attr.StyleEx = WS_EX_CLIENTEDGE;
            pText->SetSunken( FALSE );
         }

         pText->SetLink( pLD->chHyperText );
         pText->SetToolTipPosition( 1 );
         pText->SetToolTipText( pLD->szToolTip );
         pText->SetVerticalText( pLD->chVerticalText );
         pText->ShowToolTipOnlyOnMouseClick( pLD->chUseToolTip & 1 );

         if ( pText->m_bUseBevelLine != pLD->chBeveledLine )
            pText->UseBevelLine( pLD->chBeveledLine );

         if ( pText->m_bUseHighLighting != (pLD->chHighlight & 1) )
            pText->UseHighLighting( pLD->chHighlight & 1 );

         pText->UseHighLightingOnlyWithToolTip( pLD->chHighlight & 2 );
         pText->UseLeftMouseButtonForToolTip( pLD->chUseToolTip & 2 );
         pText->SetRedrawLocal( TRUE );
         pText->RedrawWindowLocal( );

         return( 0 );
      }
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  The Text font information using the following structure for reports:
//
//            byte  field          value
//            ----  ------      --------------------------------------------
//              0   Bold           1-on ... 0-off
//              1   Italic         1-on ... 0-off
//              2   Strikeout      1-on ... 0-off
//              3   Underline      1-on ... 0-off
//             4-7  PointSize      long != 0
//            8-11  Text Color     unsigned long RGB color value
//             12   MultiLine      1-on ... 0-off
//  Justification
//             13   Top/Left     (Offset)  1-left ... -1 left + offset  | 2-top
//             14   Center       1-horizontal ... 2-vertical
//             15   Bottom/Right (Offset) 1-right ... -1 right - offset | 2-bottom
//           16-19  BorderStyle (0-solid 1-dash 2-dot 3-dashdot 4-dashdotdot)
//               (second byte 1-top | 2-left | 4-bottom | 8-right)
//               (high order byte is line width)
//           20-23  BorderColor    unsigned long RGB color value
//           24-27  Escapement
//           28-31  lType
//           32-63  Font face name
//           64-355 *** reserved ... used in online ***
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TX_GetTextRptCtrlBOI( zVIEW    vCtrl,
                      zPSHORT  pbMultiLine,
                      zPSHORT  pbBold,
                      zPSHORT  pbItalic,
                      zPSHORT  pbStrikeout,
                      zPSHORT  pbUnderline,
                      zPLONG   plJustify,
                      zPLONG   plSize,
                      zPLONG   plTextColor,
                      zPLONG   plTextBkColor,
                      zPLONG   plBorderStyle,
                      zPLONG   plBorderColor,
                      zPLONG   plEscapement,
                      zPLONG   plType,
                      zPCHAR   pchFaceName )
{
   LABEL_DATA ld;
   zULONG  ulLth = sizeof( ld ); // = 84; for reports

   GetAttributeLength( &ulLth, vCtrl, "Control", "CtrlBOI" );
   if ( ulLth > sizeof( ld ) )
      ulLth = sizeof( ld );

   zmemset( &ld, 0, sizeof( ld ) );
   GetBlobFromAttribute( &ld, &ulLth, vCtrl, "Control", "CtrlBOI" );

   zPCHAR pch;
   GetAddrForAttribute( &pch, vCtrl, "Control", "Tag" );
//::MessageBox( 0, "vCtrl|Control|CtrlBOI", "Zeidon Error", MB_OK );

   *plJustify = 0;
   *plBorderStyle = 0;
   *plBorderColor = 0;
   *plEscapement = 0;
   *plType = 0;         // reserved
   if ( pchFaceName )
      pchFaceName[ 0 ] = 0;

   if ( ulLth )
   {
      *pbBold = ld.chBold;
      *pbItalic = ld.chItalic;
      *pbStrikeout = ld.chStrikeout;
      *pbUnderline = ld.chUnderline;
      *plSize = ld.lPointSize;
      *plTextColor = ld.lTextColor;
      if ( ulLth > 64 )
         *plTextBkColor = ld.lTextBkColor;
      else
         *plTextBkColor = CLR_INVALID;

      *pbMultiLine = ld.chMultiLine;
      if ( ulLth > 5 + 2 * sizeof( zLONG ) )
      {
         if ( ld.chCenter & 1 )
         {
            *plJustify = 2;
         }
         else
         if ( ld.chBottomRight < 0 )
         {
            *plJustify = 4 + 0x00000040;  // offset text
         }
         else
         if ( ld.chBottomRight & 1 )
         {
            *plJustify = 4;
         }
         else
         if ( ld.chTopLeft < 0 )
         {
            *plJustify = 1 + 0x00000040;  // offset text
         }
         else
         {
            *plJustify = 1;
         }

         if ( ld.chBottomRight < 0 )
            ld.chBottomRight *= -1;

         if ( ld.chCenter & 2 )
            *plJustify |= 0x00000100;  // vertical center
         else
         if ( ld.chBottomRight & 2 )
            *plJustify |= 0x00000200;  // bottom
      }

      if ( pchFaceName && (ulLth > 7 + (2 * sizeof( zLONG ))) )
      {
         *plBorderStyle = ld.lBorderStyle;
         *plBorderColor = ld.lBorderColor;
         *plEscapement = ld.lEscapement;
         *plType = ld.lType;
         strcpy_s( pchFaceName, 32, ld.szFontName );
      }
   }
   else
   {
      *pbBold = FALSE;
      *pbItalic = FALSE;
      *pbStrikeout = FALSE;
      *pbUnderline = FALSE;
      *plSize = 0;
      *plTextColor = CLR_INVALID;
      *plTextBkColor = CLR_INVALID;
      *pbMultiLine = 0;
   }

   return( (zSHORT) ulLth );
}

zOPER_EXPORT zSHORT OPERATION
TX_SetTextRptCtrlBOI( zVIEW   vCtrl,
                      zSHORT  nMultiLine,
                      zSHORT  bBold,
                      zSHORT  bItalic,
                      zSHORT  bStrikeout,
                      zSHORT  bUnderline,
                      zLONG   lJustify,
                      zLONG   lSize,
                      zLONG   lTextColor,
                      zLONG   lTextBkColor,
                      zLONG   lBorderStyle,
                      zLONG   lBorderColor,
                      zLONG   lEscapement,
                      zLONG   lType,
                      zCPCHAR cpcFaceName )
{
   LABEL_DATA ld;
   zULONG  ulLth;

   zmemset( &ld, 0, sizeof( ld ) );
   if ( nMultiLine || bBold || bItalic || bStrikeout ||
        bUnderline || lSize || lTextColor || lTextBkColor || lJustify ||
        lBorderStyle || lBorderColor || lEscapement || lType ||
        (cpcFaceName && *cpcFaceName) )
   {
      zCHAR  ch;

      ulLth = 84;  // for reports

      ld.chBold = (char) bBold;
      ld.chItalic = (char) bItalic;
      ld.chStrikeout = (char) bStrikeout;
      ld.chUnderline = (char) bUnderline;

      ld.lPointSize = lSize;
      ld.lTextColor = lTextColor;
      ld.lTextBkColor = lTextBkColor;
      ld.chMultiLine = (char) nMultiLine;

      ld.chTopLeft = 0;
      ld.chCenter = 0;
      ld.chBottomRight = 0;
      if ( lJustify & 0x00000040 )  // offset text?
      {
         lJustify &= ~0x00000040;
         ch = -1;
      }
      else
         ch = 1;

      if ( lJustify & 2 )
      {
         ld.chTopLeft = 0;
         ld.chCenter = ch;
         ld.chBottomRight = 0;
      }
      else
      if ( lJustify == 4 )
      {
         ld.chTopLeft = 0;
         ld.chCenter = 0;
         ld.chBottomRight = ch;
      }
      else
      {
         ld.chTopLeft = ch;
         ld.chCenter = 0;
         ld.chBottomRight = 0;
      }

      if ( lJustify & 0x00000100 )  // vertical center
         ld.chCenter |= 2;
      else
      if ( lJustify & 0x00000200 )  // bottom
      {
         if ( ld.chBottomRight < 0 )
         {
            ld.chBottomRight *= -1;
            ld.chBottomRight |= 2;
            ld.chBottomRight *= -1;
         }
         else
            ld.chBottomRight |= 2;
      }

      ld.lBorderStyle = lBorderStyle;
      ld.lBorderColor = lBorderColor;
      ld.lEscapement = lEscapement;
      ld.lType = lType;
      if ( cpcFaceName && *cpcFaceName )
         strcpy_s( ld.szFontName, zsizeof( ld.szFontName ), cpcFaceName );
      else
         ld.szFontName[ 0 ] = 0;
   }
   else
   {
      ulLth = 0;
   }

   SetAttributeFromBlob( vCtrl, "Control", "CtrlBOI", &ld, ulLth );
   return( (zSHORT) ulLth );
}

//./ ADD NAME=TX_SetBackgroundImage
// Source Module=zdctltxt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TX_SetBackgroundImage
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the background image for a text control.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the text control.
//              cpcCtrlTag - The unique name of the control.
//              ulResId    - Resource id of the background image (-1 to
//                           remove image).
//              lFlag      - 0 ==> do not stretch bitmap
//                           1 ==> stretch bitmap
//                           2 ==> hold top-left of image fixed if not
//                             stretched and image is bigger than client.
//
//  RETURNS:    0 - Colors set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
TX_SetBackgroundImage( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zULONG  ulResId,
                       zLONG   lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZText *pText = DYNAMIC_DOWNCAST( ZText, pzma->m_pCtrl );
      if ( pText )
      {
      // HINSTANCE hInst = AfxGetResourceHandle( );
      // AfxSetResourceHandle( pZSubtask->m_hInstRes );
         pText->m_bStretchBitmap = (lFlag & 0x00000001) ? TRUE : FALSE;
         pText->m_bFixTopLeft = (lFlag & 0x00000002) ? TRUE : FALSE;
         pText->SetBackImage( pZSubtask->m_hInstRes, ulResId );
      // AfxSetResourceHandle( hInst );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for TX_SetBackgroundImage ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TX_SetColors
// Source Module=zdctltxt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TX_SetColors
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets a text ctrl color
//
//  PARAMETERS: vSubtask      - The subtask view for the window containing
//                              the text control.
//              cpcCtrlTag    - The unique name of the control.
//              clr           - Color (-1 ==> default)
//              lFlag         - Set color:  1 - Text; 2 - Text Background;
//                                          4 - Text Hilite; 8 - Background
//                              Set to Text Color: 0x40000000
//                              Set to Invert Text Color: 0x80000000
//
//  RETURNS:    0 - Color set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
TX_SetColors( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zLONG   clr,
              zLONG   lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZText *pText = DYNAMIC_DOWNCAST( ZText, pzma->m_pCtrl );
      if ( pText )
      {
         if ( lFlag & 0xC0000000 )
         {
            COLORREF clrText = pText->GetTextColor( );
            if ( lFlag & 0x80000000 )
            {
               clr = zRGB( 255 - GetRValue( clrText ),  // invert text color
                           255 - GetGValue( clrText ),
                           255 - GetBValue( clrText ) );
            }
            else
               clr = clrText;
         }

         if ( lFlag & 0x00000001 )
         {
            if ( clr == -1 )
               pText->SetDefaultTextColor( );
            else
               pText->SetTextColor( clr );
         }

         if ( lFlag & 0x00000002 )
         {
            if ( clr == -1 )
               pText->SetDefaultTextBackColor( );
            else
               pText->SetTextBackColor( clr );
         }

         if ( lFlag & 0x00000004 )
         {
            if ( clr == -1 )
               pText->UseHighLighting( FALSE );
            else
            {
               pText->UseHighLighting( TRUE );
               pText->SetHighLightColor( clr );
            }
         }

         if ( lFlag & 0x00000008 )
         {
            if ( clr == -1 )
               pText->SetDefaultBkColor( );
            else
               pText->SetBkColor( clr );
         }

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for TX_SetBackgroundImage ",
                  cpcCtrlTag );
   }

   return( -1 );
}
