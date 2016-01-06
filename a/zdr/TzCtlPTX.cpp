/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlptx.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C++ classes for Zeidon Painter Tools
// standard painter controls.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.06    TMV   Z10
//    Make a GroupSet deletable even if it is a header or footer.
//
// 2001.06.28    DKS   Z10
//    Fix for overlap marking in report painter.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2000.12.29    BL
//    Modified TZIcon::TZIcon: do not assigned a value to a null pointer
//
// 2000.08.16    HH
//    in TZBitmap::OnCreate try to load the bitmap containing dll via
//    normal path, if explicitly loading from EXEC-directory fails.
//    Neccessary for Zeidon itself, where tzpntrad.dll cannot be loaded
//    from w\bin\sys directory.
//
// 2000.06.05    DKS   Z10
//    Load the Task LPLR view once (performance enhancement).
//
// 1999.12.22    DKS   Z10    HPF999
//    Zeidon.ini override of fonts shown at Design time same as at Runtime ...
//    including text fonts.
//
// 1999.05.11    DKS   Z10    ActiveX
//    New TZActiveX code.
//
// 1999.02.16    DKS
//    Used new option zLOADLIB_RESOURCES on SysLoadLibraryWithErrFlag.
//
// 1999.02.08    DKS
//    ReportTextSize default cached for reports.
//
// 1999.02.02    DKS
//    Added default font support for reports.
//
// 1999.01.26    DKS
//    Failure to create ActiveX control now traced.  Blocked out
//    GetAttributeCount code (hard-coded to the DataTable).
//
// 1999.01.21    DKS
//    Text enhancement completed Escapement implementation.
//
// 1999.01.11    DKS
//    Text for reports enhanced.  Added Escapement.
//
// 1998.12.14    DKS
//    Aligned GroupSet and Group borders.
//
// 1998.09.09    DKS
//    Newly painted Icon controls are now visible (XC 218)
//
// 1998.08.13    DKS
//    Fix to Tab control acceptance by GroupBox (now warns)
//

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "TzCtl.h"
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL
// #define zDEBUG_CTRL

IMPLEMENT_DYNAMIC( TZEdit, CEdit )
BEGIN_MESSAGE_MAP( TZEdit, CEdit )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZPush, CButton )
BEGIN_MESSAGE_MAP( TZPush, CButton )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZRadio, CButton )
BEGIN_MESSAGE_MAP( TZRadio, CButton )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZCheck, CButton )
BEGIN_MESSAGE_MAP( TZCheck, CButton )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZGraph, CEdit )
BEGIN_MESSAGE_MAP( TZGraph, CEdit )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZMLEdit, CEdit )
BEGIN_MESSAGE_MAP( TZMLEdit, CEdit )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZDil, CStatic )
BEGIN_MESSAGE_MAP( TZDil, CStatic )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZGrid, CListBox )
BEGIN_MESSAGE_MAP( TZGrid, CListBox )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZTree, CStatic )
BEGIN_MESSAGE_MAP( TZTree, CStatic )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZERD, CEdit )
BEGIN_MESSAGE_MAP( TZERD, CEdit )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZText, CStatic )
BEGIN_MESSAGE_MAP( TZText, CStatic )
   ON_WM_CREATE( )
   ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZListBox, CListBox )
BEGIN_MESSAGE_MAP( TZListBox, CListBox )
   ON_WM_CREATE( )
// ON_WM_WINDOWPOSCHANGED( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZComboBox, CComboBox )
BEGIN_MESSAGE_MAP( TZComboBox, CComboBox )
   ON_WM_CREATE( )
   ON_WM_WINDOWPOSCHANGED( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZGroupBox, CButton )
BEGIN_MESSAGE_MAP( TZGroupBox, CButton )
// ON_WM_DRAWITEM( )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZIcon, CWnd )
BEGIN_MESSAGE_MAP( TZIcon, CWnd )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZCalendar, CWnd )
BEGIN_MESSAGE_MAP( TZCalendar, CWnd )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZDayTimer, CWnd )
BEGIN_MESSAGE_MAP( TZDayTimer, CWnd )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZControlBarC, baseTZControlBarC )
BEGIN_MESSAGE_MAP( TZControlBarC, baseTZControlBarC )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_SIZE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZControlBarG, baseTZControlBarG )
BEGIN_MESSAGE_MAP( TZControlBarG, baseTZControlBarG )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_SIZE( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZBitmap, CBitmapButton )
BEGIN_MESSAGE_MAP( TZBitmap, CBitmapButton )
   ON_WM_CREATE( )
// ON_WM_DRAWITEM( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZTimer, CBitmapButton )
BEGIN_MESSAGE_MAP( TZTimer, CBitmapButton )
   ON_WM_CREATE( )
// ON_WM_DRAWITEM( )
END_MESSAGE_MAP( )

IMPLEMENT_DYNAMIC( TZShape, CWnd )
BEGIN_MESSAGE_MAP( TZShape, CWnd )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// TZText, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZText::TZText( TZPainterWindow *pPainterWindow,
                TZPainterCtrl   *pCtrlParent,
                CWnd    *pWndParent,
                CRect&  rect,
                zVIEW   vCtrl,
                zLONG   lType,
                zBOOL   bPlaceHolder,
                zLONG   lZKey,
                zLONG   lTabStopNbr,
                zLONG   lCtrlID,
                zPCHAR  pchTag,
                zPCHAR  pchText ) :
        CStatic( ),
        TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                       rect, vCtrl, this, pchTag, pchText,
                       lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZText::ctor: ", m_csTag );
#endif

   if ( pchText[ 0 ] == 0 ||
        (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) )
   {
      Attr.Style &= ~SS_LEFT;
      Attr.Style |= SS_LEFTNOWORDWRAP;
   }

// the Text font information uses the following structure:
//
//             byte  field          value
//             ----  ------      --------------------------------------------
//               0   Bold           1-on ... 0-off
//               1   Italic         1-on ... 0-off
//               2   Strikeout      1-on ... 0-off
//               3   Underline      1-on ... 0-off
//              4-7  PointSize      long != 0
//             8-11  Text Color     unsigned long RGB color value
//  not used    12   MultiLine      1-on ... 0-off
//  Justification
//              13   Top/Left       1-left ... 2-top ... 0-off
//              14   Center         1-horiz ... 2-vert ... 0-off
//              15   Bottom/Right   1-on ... 2-bottom ... 0-off
//  not used  16-19  BorderStyle (0-solid 1-dash 2-dot 3-dashdot 4-dashdotdot)
//               (second byte 1-top | 2-left | 4-bottom | 8-right)
//               (high order byte is line width)
//  not used  20-23  BorderColor    unsigned long RGB color value
//            24-27  Escapement
//            28-31  lType          // reserved
//            32-63  Font face name
//            64-83  *** reserved ***
//  New Label
//            84-87  Text Bk Color     unsigned long RGB color value
//            88-91  Background Color  unsigned long RGB color value
//            92-95  Highlight Color   unsigned long RGB color value
//              96   Border/Sunken  1-Border, 2-Sunken Border
//              97   Beveled Line   1-on ... 0-off
//              98   Vertical Text  1-on ... 0-off
//              99   Highlight      1-on ... 2-only with ToolTip ... 0-off
//          100-355  ToolTip Text (max length 256)

   m_lBorderStyle = 0;
   m_lBorderColor = 0;
   m_lEscapement = 0;
   m_lTypeText = 0;
   m_bLeft = TRUE;
   m_bCenter = FALSE;
   m_bRight = FALSE;
   m_chMultiLine = 0;
   m_uFormat = DT_LEFT; // | DT_SINGLELINE;  we want to see any word wrap

   m_szFaceName[ 0 ] = 0;
   m_bBold = FALSE;
   m_bItalic = FALSE;
   m_bStrikeout = FALSE;
   m_bUnderline = FALSE;

   if ( pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
      m_lSize = pPainterWindow->m_lReportTextSize;
   else
      m_lSize = 0;

   if ( vCtrl )
   {
      LABEL_DATA ld;
      zULONG  ulLth = sizeof( ld );

      if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
      {
         if ( m_lSubtype & zCONTROL_RIGHTJUSTIFY )
            Attr.Style |= SS_RIGHT;
         else
         if ( m_lSubtype & zCONTROL_CENTERJUSTIFY )
            Attr.Style |= SS_CENTER;
      }

      zmemset( &ld, 0, sizeof( ld ) );
      GetBlobFromAttribute( &ld, &ulLth, vCtrl, szlControl, szlCtrlBOI );
      if ( ulLth )
      {
         m_bBold = ld.chBold;
         m_bItalic = ld.chItalic;
         m_bStrikeout = ld.chStrikeout;
         m_bUnderline = ld.chUnderline;
         m_lSize = ld.lPointSize;
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              pPainterWindow->m_lUnit == 0 && m_lSize != 0 )
         {
            if ( m_lSize > 0 )
               m_lSize--;
            else
               m_lSize++;

            m_lSize *= 10;
            ld.lPointSize = m_lSize;
            SetAttributeFromBlob( vCtrl, szlControl, szlCtrlBOI, &ld, ulLth );
         }
         else
         if ( m_lSize == 0 &&
              (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) )
         {
            m_lSize = pPainterWindow->m_lReportTextSize;
         }

         m_clrText = ld.lTextColor;
         if ( ulLth >= 8 + (6 * sizeof( zLONG )) )
         {
            m_chMultiLine = ld.chMultiLine;
            m_bLeft = ld.chTopLeft & 1;
            m_bCenter = ld.chCenter & 1;
            m_bRight = ld.chBottomRight & 1;

            m_lBorderStyle = ld.lBorderStyle;
            m_lBorderColor = ld.lBorderColor;
            m_lEscapement = ld.lEscapement;
            m_lTypeText = ld.lType;
            strcpy_s( m_szFaceName, sizeof( m_szFaceName ), ld.szFontName );

            while ( m_lEscapement < 0 )
               m_lEscapement += 3600;

            while ( m_lEscapement >= 3600 )
               m_lEscapement -= 3600;

            if ( m_chMultiLine && m_lEscapement == 0 )
            {
               Attr.Style &= ~SS_LEFTNOWORDWRAP;
               // SS_NOPREFIX
               m_uFormat = DT_WORDBREAK;
            }
            else
            {
               Attr.Style &= ~SS_LEFTNOWORDWRAP;
            // m_uFormat = DT_SINGLELINE;    // we want to see any word wrap
               m_uFormat = DT_LEFT;
            }

            if ( m_bRight )
            {
               Attr.Style &= ~(SS_LEFT | SS_CENTER);
//             Attr.Style |= SS_RIGHT;
               m_uFormat |= DT_RIGHT;
            }
            else
            if ( m_bCenter )
            {
               Attr.Style &= ~(SS_LEFT | SS_RIGHT);
//             Attr.Style |= SS_CENTER;
               m_uFormat |= DT_CENTER;
            }
            else
         // if ( m_bLeft )
            {
               Attr.Style &= ~(SS_CENTER | SS_RIGHT);
//             Attr.Style |= SS_LEFT;
               m_uFormat |= DT_LEFT;
            }
         }
         else
         if ( ulLth > 4 + (2 * sizeof( zLONG )) )
            m_chMultiLine = ld.chMultiLine;
      }
   }
   else
   {
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;
   }

   if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
        m_bBold == 0 && m_bItalic == 0 && m_bStrikeout == 0 &&
        m_bUnderline == 0 && // m_lSize == 0 &&
        m_lEscapement == 0 && m_szFaceName[ 0 ] == 0 )
   {
      strcpy_s( m_szFaceName, sizeof( m_szFaceName ),
               m_pPainterWindow->m_pBar->m_szReportFaceName );
      if ( m_lSize == 0 )
      {
         if ( pPainterWindow->m_lReportTextSize == 0 )
            m_lSize = m_pPainterWindow->m_pBar->m_lReportFontSize * 10;
         else
            m_lSize = pPainterWindow->m_lReportTextSize;
      }
   }

   CreateZ( );
}

void
TZText::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZText::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_STATICEDGE, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZText::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZText::OnCreate: ", m_csTag );
#endif

   if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   if ( m_bBold || m_bItalic || m_bStrikeout || m_bUnderline || m_lSize ||
        m_lEscapement || m_szFaceName[ 0 ] )
   {
      CClientDC dc( this );
      zLONG     lSize;

      mDeleteInit( m_pFont );
      m_pFont = new CFont( );
      if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
         lSize = (m_lSize * dc.GetDeviceCaps( LOGPIXELSY )) / 720;
      else
         lSize = (m_lSize * dc.GetDeviceCaps( LOGPIXELSY )) / 72;

      m_pFont->CreateFont( -lSize, 0, m_lEscapement, m_lEscapement,
                           m_bBold ? FW_BOLD : FW_NORMAL,
                           m_bItalic, m_bUnderline, m_bStrikeout,
                           ANSI_CHARSET, OUT_TT_ONLY_PRECIS,
                           CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                           (BYTE) (m_lEscapement ? VARIABLE_PITCH | FF_SWISS :
                                                   DEFAULT_PITCH | FF_DONTCARE),
                           m_szFaceName[ 0 ] ? m_szFaceName :
                           m_pPainterWindow->m_pBar->m_bUseMapDlgUnits ?
                                                   "MS Sans Serif" : "Arial" );

      // DM - m_lSize is the font size which is specified in points.
      // 1 point = 1 / 72 of an inch.  So, we convert points into
      // pixels for the CFont ctor.
      //                   (mConvert256thInchToPixelY( m_lSize * 16 ) / 72),
   }
   else
   {
      SetFontOverride( );;
   }

   return( 0 );
}

HBRUSH
TZText::CtlColor( CDC *pDC, UINT nCtlColor )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZText::CtlColor: ", m_csTag );
#endif

   HBRUSH hBrush = (HBRUSH) Default( );
   if ( m_clrText )
   {
      pDC->SetTextColor( m_clrText );
   }

   return( hBrush );
}

#include "math.h"

void
TZText::OnPaint( )
{
#if 0 // def DEBUG_ALL lots of messages
   if ( zstrcmp( m_csTag, "Line" ) == 0 )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "TZText::OnPaint: %s ====>", m_csTag );
      TraceRect( szMsg, m_rectPainter );
   }
#endif

   if ( m_pFont ) //&& m_lEscapement )
   {
      // m_lEscapement is guaranteed at this point to be between 0 and 3600.

      CPaintDC dc( this );   // a paint DC calls Begin and End Paint
      CFont *pFontOld = 0;
      if ( m_pFont )
         pFontOld = dc.SelectObject( m_pFont );

      COLORREF colorOldText = dc.SetTextColor( m_clrText );
      COLORREF colorOldBk;
      COLORREF colorBk;
      CRect rectClient;
      zSHORT nQuadrant = (zSHORT) ((m_lEscapement - 1) / 900) + 1;

//    GetUpdateRect( rect, 1 );
//    CBrush brush( dc.GetBkColor( ) );
      colorBk = ::GetSysColor( COLOR_BTNFACE );
      CBrush brush( colorBk );
      colorOldBk = dc.SetBkColor( colorBk );

      GetClientRect( rectClient );
      CRect rect( rectClient );
      dc.FillRect( rect, &brush );
   // dc.TextOut( rect.left, rect.top, m_csText );

      if ( m_lEscapement )
      {
         m_uFormat = DT_LEFT | DT_SINGLELINE;
         static double pi = 3.14159265358979323846;
         CSize  sizeExtent = dc.GetTextExtent( m_csText );  // in logical units
         dc.LPtoDP( &sizeExtent );
         CSize  sizeAdjustedExtent;
         double dEscapeAngle;
         double dSin;
         double dCos;

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
               sizeAdjustedExtent.cx = (zLONG) ((sizeExtent.cx * dCos) +
                                                -(sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) ((sizeExtent.cy * dCos) +
                                                -(sizeExtent.cx * dSin * 5 / 4));
            // if ( zstrcmp( m_csTag, "Line" ) == 0 )
            //   TraceSize( "  4Extent adjusted", sizeAdjustedExtent );

//             rect.top = (zLONG) (dCos * sizeExtent.cy);
               rect.left = (zLONG) -(dSin * sizeExtent.cy);
               rect.right = rect.left + sizeAdjustedExtent.cx;
               rect.bottom = rect.top + sizeAdjustedExtent.cy;
               break;
            }

            case 3:
            {
               m_uFormat |= DT_TOP;
               sizeAdjustedExtent.cx = (zLONG) (-(sizeExtent.cx * dCos) +
                                                -(sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) (-(sizeExtent.cy * dCos) +
                                                -(sizeExtent.cx * dSin * 5 / 4));
            // if ( zstrcmp( m_csTag, "Line" ) == 0 )
            //   TraceSize( "  4Extent adjusted", sizeAdjustedExtent );

               rect.top = (zLONG) -(dCos * sizeExtent.cy);
               rect.left = 16 + sizeAdjustedExtent.cx;
               rect.right = rect.left + sizeAdjustedExtent.cx;
               rect.bottom = rect.top + sizeAdjustedExtent.cy;
               break;
            }

            case 2:
            {
               m_uFormat |= DT_BOTTOM;
               sizeAdjustedExtent.cx = (zLONG) (-(sizeExtent.cx * dCos) +
                                                (sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) (-(sizeExtent.cy * dCos) +
                                                (sizeExtent.cx * dSin * 5 / 4));
            // if ( zstrcmp( m_csTag, "Line" ) == 0 )
            //    TraceSize( "  2Extent adjusted", sizeAdjustedExtent );

               rect.top = sizeExtent.cy;
               rect.left = (zLONG) ((sizeExtent.cy) + (sizeExtent.cy * dCos) -
                                    (dCos * sizeAdjustedExtent.cx));
               rect.right = sizeAdjustedExtent.cx;
               rect.bottom = (zLONG) ((rect.top + (12 * dSin)) -
                                      (sizeExtent.cy * dCos) +
                                      (sizeExtent.cx * dSin));
               break;
            }

         // case 1:  first quadrant
            default:
            {
               m_uFormat |= DT_BOTTOM;
               sizeAdjustedExtent.cx = (zLONG) ((sizeExtent.cx * dCos) +
                                                (sizeExtent.cy * dSin * 4 / 5));
               sizeAdjustedExtent.cy = (zLONG) ((sizeExtent.cy * dCos) +
                                                (sizeExtent.cx * dSin * 5 / 4));
            // if ( zstrcmp( m_csTag, "Line" ) == 0 )
            //    TraceSize( "  1Extent adjusted", sizeAdjustedExtent );

               rect.top = sizeExtent.cy;
//             rect.left = (zLONG) (dSin * sizeExtent.cy);
               rect.right = rect.left + sizeAdjustedExtent.cx;
               rect.bottom = (zLONG) ((rect.top + (12 * dSin)) +
                                   // (sizeExtent.cy * dSin) +
                                      (sizeExtent.cx * dSin));
               break;
            }
         }
      }
      else
      {
         m_uFormat |= DT_WORDBREAK;
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

   // if ( zstrcmp( m_csTag, "Line" ) == 0 )
   //    TraceRect( "  Line Rect", rect );

      // We cannot use DT_NOPREFIX here.
      if ( m_chMultiLine )
      {
         zLONG  lLth = zstrlen( m_csText ) + 1;
         zPCHAR pchText = new char[ lLth ];
         zPCHAR pch = pchText;

         strcpy_s( pchText, lLth, m_csText );
         while ( (pch = zstrchr( pch, '\\' )) != 0 )
         {
            if ( *(pch + 1) == 'n' )
            {
               *pch = '\r';
               *(pch + 1) = '\n';
            }

            pch += 2;
         }

         dc.DrawText( pchText, -1, rect, m_uFormat | DT_NOCLIP );
         delete [] pchText;
      }
      else
         dc.DrawText( m_csText, -1, rect, m_uFormat | DT_NOCLIP );

      if ( pFontOld )
         dc.SelectObject( pFontOld );

      dc.SetTextColor( colorOldText );
      dc.SetBkColor( colorOldBk );
   }
   else
      CStatic::OnPaint( );
}

// dtor
TZText::~TZText( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZText::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZEdit, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZEdit::TZEdit( TZPainterWindow *pPainterWindow,
                TZPainterCtrl   *pCtrlParent,
                CWnd    *pWndParent,
                CRect&  rect,
                zVIEW   vCtrl,
                zLONG   lType,
                zBOOL   bPlaceHolder,
                zLONG   lZKey,
                zLONG   lTabStopNbr,
                zLONG   lCtrlID,
                zPCHAR  pchTag,
                zPCHAR  pchText ) :
        CEdit( ),
        TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                       rect, vCtrl, this, pchTag, pchText,
                       lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZEdit::ctor", m_csTag );
#endif

// Attr.Style |= SS_LEFT;
   CreateZ( );
}

void
TZEdit::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZEdit::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "EDIT", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZEdit::OnCreate: ", m_csTag );
#endif

   if ( CEdit::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZEdit::~TZEdit( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZEdit::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZCalendar, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZCalendar::TZCalendar( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CWnd( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCalendar::ctor: ", m_csTag );
#endif

   CreateZ( );
}

void
TZCalendar::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCalendar::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "SysDateTimePick32", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZCalendar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCalendar::OnCreate: ", m_csTag );
#endif

   int nRC = CWnd::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   return( nRC );
}

// dtor
TZCalendar::~TZCalendar( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCalendar::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZDayTimer, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZDayTimer::TZDayTimer( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CWnd( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDayTimer::ctor: ", m_csTag );
#endif

   CreateZ( );
}

void
TZDayTimer::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDayTimer::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "SysDateTimePick32", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZDayTimer::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDayTimer::OnCreate: ", m_csTag );
#endif

   int nRC = CWnd::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   // Create brushes.
   CBitmap bmpPattern;
   VERIFY( bmpPattern.LoadBitmap( IDS_CALENDAR_BKGND ) );
   VERIFY( m_brBack.CreatePatternBrush( &bmpPattern ) ); // background color
   VERIFY( m_penNull.CreatePen( PS_NULL, 0, (COLORREF) 0 ) );

   SetFontOverride( );
   return( nRC );
}

const static int g_nFullDayAppointmentHeight =
     ::GetSystemMetrics( SM_CXHSCROLL ) + ::GetSystemMetrics( SM_CXEDGE );

void
TZDayTimer::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZDayTimer::OnPaint: %s ====>", m_csTag );
   TraceRect( szMsg, m_rectPainter );
#endif

   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
   CRect rectClient;
   GetClientRect( &rectClient );

   // Paint the background with a pattern brush.
   CBrush *pBrushOld = dc.SelectObject( &m_brBack );
   CPen *pPenOld = dc.SelectObject( &m_penNull );
   VERIFY( dc.Rectangle( &rectClient ) );

   // Restore DC.
   dc.SelectObject( pBrushOld );
   dc.SelectObject( pPenOld );

   // Draw half-hour memo dividers.
   while ( rectClient.bottom > rectClient.top )
   {
      rectClient.bottom -= g_nFullDayAppointmentHeight;
      dc.MoveTo( rectClient.left, rectClient.bottom );
      dc.LineTo( rectClient.right, rectClient.bottom );
   }
}

// dtor
TZDayTimer::~TZDayTimer( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDayTimer::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZControlBarG, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZControlBarG::TZControlBarG( TZPainterWindow *pPainterWindow,
                              TZPainterCtrl   *pCtrlParent,
                              CWnd    *pWndParent,
                              CRect&  rect,
                              zVIEW   vCtrl,
                              zLONG   lType,
                              zBOOL   bPlaceHolder,
                              zLONG   lZKey,
                              zLONG   lTabStopNbr,
                              zLONG   lCtrlID,
                              zPCHAR  pchTag,
                              zPCHAR  pchText ) :
               baseTZControlBarG( ),
               TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                              rect, vCtrl, this, pchTag, pchText,
                              lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarG::ctor: ", m_csTag );
#endif

   CreateZ( );
}

void
TZControlBarG::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarG::CreateZ: ", m_csTag );
#endif

   Create( m_csText, m_pPainterWindow, 123 ); //, m_nIdNbr, dwStyle );
}

int
TZControlBarG::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarG::OnCreate: ", m_csTag );
#endif

   int nRC = baseTZControlBarG::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );

   SetSCBStyle( GetSCBStyle( ) | SCBS_SIZECHILD );
   SetBarStyle( GetBarStyle( ) |
                CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

   // Enable docking only on the left or right sides (or floating).
   EnableDocking( CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT );

   m_pPainterWindow->EnableDocking( CBRS_ALIGN_ANY );
   m_pPainterWindow->DockControlBar( this, AFX_IDW_DOCKBAR_LEFT );

   return( nRC );  // ControlBarG is now created
}

void
TZControlBarG::OnLButtonDown( UINT uModKeys, CPoint pt )
{
   m_pCtrlCover->OnLButtonDown( uModKeys, pt );
}

zBOOL
TZControlBarG::IsModeValid( zULONG ulMode )
{
   if ( ulMode & zCTRL_MODE_CLIENT )
      return( TRUE );

   return( TZPainterCtrl::IsModeValid( ulMode ) );
}

void
TZControlBarG::OnSize( UINT uType, int cx, int cy )
{
   baseTZControlBarG::OnSize( uType, cx, cy );
   GetWindowRect( m_rectPainter );
}

void
TZControlBarG::HideForUndo( zBOOL bHide )
{
   if ( m_pCtrlCover &&
        mIs_hWnd( m_pCtrlCover->m_hWnd ) )
   {
      m_pCtrlCover->ShowWindow( bHide ? SW_HIDE : SW_SHOW );
   }

   m_pPainterWindow->m_pBar->m_pZSubtask->m_pZFWnd->
                             ShowControlBar( this, !bHide, FALSE );
}

// dtor
TZControlBarG::~TZControlBarG( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarG::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZControlBarC, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZControlBarC::TZControlBarC( TZPainterWindow *pPainterWindow,
                              TZPainterCtrl   *pCtrlParent,
                              CWnd    *pWndParent,
                              CRect&  rect,
                              zVIEW   vCtrl,
                              zLONG   lType,
                              zBOOL   bPlaceHolder,
                              zLONG   lZKey,
                              zLONG   lTabStopNbr,
                              zLONG   lCtrlID,
                              zPCHAR  pchTag,
                              zPCHAR  pchText ) :
               baseTZControlBarC( ),
               TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                              rect, vCtrl, this, pchTag, pchText,
                              lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarC::ctor: ", m_csTag );
#endif

   CreateZ( );
}

void
TZControlBarC::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarC::CreateZ: ", m_csTag );
#endif
   CRect rect;

   Create( m_csText, m_pPainterWindow, 123 ); //, m_nIdNbr, dwStyle );
}

int
TZControlBarC::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarC::OnCreate: ", m_csTag );
#endif

   int nRC = baseTZControlBarC::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );

   SetSCBStyle( GetSCBStyle( ) | SCBS_SIZECHILD );
   SetBarStyle( GetBarStyle( ) |
                CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

   // Enable docking only on the left or right sides (or floating).
   EnableDocking( CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT );

   m_pPainterWindow->EnableDocking( CBRS_ALIGN_ANY );
   m_pPainterWindow->DockControlBar( this, AFX_IDW_DOCKBAR_LEFT );

   return( nRC );  // ControlBarC is now created
}

void
TZControlBarC::OnLButtonDown( UINT uModKeys, CPoint pt )
{
   m_pCtrlCover->OnLButtonDown( uModKeys, pt );
}

zBOOL
TZControlBarC::IsModeValid( zULONG ulMode )
{
   if ( ulMode & zCTRL_MODE_CLIENT )
      return( TRUE );

   return( TZPainterCtrl::IsModeValid( ulMode ) );
}

void
TZControlBarC::OnSize( UINT uType, int cx, int cy )
{
   baseTZControlBarC::OnSize( uType, cx, cy );
   GetWindowRect( m_rectPainter );
}

void
TZControlBarC::HideForUndo( zBOOL bHide )
{
   if ( m_pCtrlCover &&
        mIs_hWnd( m_pCtrlCover->m_hWnd ) )
   {
      m_pCtrlCover->ShowWindow( bHide ? SW_HIDE : SW_SHOW );
   }

   m_pPainterWindow->m_pBar->m_pZSubtask->m_pZFWnd->
                             ShowControlBar( this, !bHide, FALSE );
}

// dtor
TZControlBarC::~TZControlBarC( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZControlBarC::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZPush, class implementation
/////////////////////////////////////////////////////////////////////////////

#define zBUTTON_MULTILINE             zCONTROL_SPECIFIC_1

// ctor
TZPush::TZPush( TZPainterWindow *pPainterWindow,
                TZPainterCtrl   *pCtrlParent,
                CWnd    *pWndParent,
                CRect&  rect,
                zVIEW   vCtrl,
                zLONG   lType,
                zBOOL   bPlaceHolder,
                zLONG   lZKey,
                zLONG   lTabStopNbr,
                zLONG   lCtrlID,
                zPCHAR  pchTag,
                zPCHAR  pchText ) :
        CButton( ),
        TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                       rect, vCtrl, this, pchTag, pchText,
                       lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPush::TZPush: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF;

   CreateZ( );
}

void
TZPush::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPush::CreateZ: ", m_csTag );
#endif

   if ( m_lSubtype & zBUTTON_MULTILINE )
   {
      zPCHAR pchText;
      zPCHAR pch = m_csText.GetBuffer( m_csText.GetLength( ) );

      Attr.Style |= BS_MULTILINE;

      while ( (pchText = zstrchr( pch, '\\' )) != 0 )
      {
         if ( *(pchText + 1) == '\\' )
         {
            // Convert to one backslash.
            strcpy_s( pchText, zstrlen( pchText ), pchText + 1 );
            pchText--;  // back up so skip only one
         }
         else
         if ( *(pchText + 1) == 'n' )
         {
            *pchText = '\r';
            *(pchText + 1) = '\n';
         }

         pch = pchText + 2;
      }

      m_csText.ReleaseBuffer( );
   }

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZPush::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPush::OnCreate: ", m_csTag );
#endif

   if ( CButton::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZPush::~TZPush( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPush::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZIcon, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor

zSHORT TZIcon::m_nIconPixelSizeX = ::GetSystemMetrics( SM_CXICON );
zSHORT TZIcon::m_nIconPixelSizeY = ::GetSystemMetrics( SM_CYICON );
zSHORT TZIcon::m_nIconEdgeX = ::GetSystemMetrics( SM_CXEDGE );
zSHORT TZIcon::m_nIconEdgeY = ::GetSystemMetrics( SM_CYEDGE );

// TZIcon - ctor
TZIcon::TZIcon( TZPainterWindow *pPainterWindow,
                TZPainterCtrl   *pCtrlParent,
                CWnd    *pWndParent,
                CRect&  rect,
                zVIEW   vCtrl,
                zLONG   lType,
                zBOOL   bPlaceHolder,
                zLONG   lZKey,
                zLONG   lTabStopNbr,
                zLONG   lCtrlID,
                zPCHAR  pchTag,
                zPCHAR  pchText ) :
        CWnd( ),
        TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                       rect, vCtrl, this, pchTag, pchText,
                       lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZIcon::ctor: ", m_csTag );
#endif

   m_pchCtrlBOI = 0;
   m_pchMsgText = 0;

   m_hIcon = 0;
   m_nIconX = 0;
   m_nIconY = 0;

   Attr.W = m_nIconPixelSizeX + 2 * m_nIconEdgeX;
   Attr.H = m_nIconPixelSizeY + 2 * m_nIconEdgeY;

   if ( vCtrl )
   {
      zULONG  ulLth;

      GetAttributeLength( &ulLth, vCtrl, szlControl, szlCtrlBOI );
      if ( ulLth > 0 )
      {
         zPCHAR    pch;

         GetAddrForAttribute( &pch, vCtrl, szlControl, szlCtrlBOI );
         m_pchCtrlBOI = new char[ ulLth ];
         zmemcpy( m_pchCtrlBOI, (zPVOID) pch, ulLth );
         zLONG lLth = zstrlen( pchText ) + 1;
         m_pchMsgText = new char[ lLth ];
         strcpy_s( m_pchMsgText, lLth, pchText );
      }
   }

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
// If the hinst parameter is non-null and the fuLoad parameter does not
// include LR_LOADFROMFILE, lpszName is a pointer to a null-terminated
// string that contains the name of the image resource in the hinst module.
//
// If hinst is null and LR_LOADFROMFILE is not specified, the low-order word
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
//    null.
//
/////////////////////////////////////////////////////////////////////////////

   LPLIBRARY hLibrary;
   HINSTANCE hInst = 0;
   zPCHAR pch = m_pchCtrlBOI;
   ZSubtask *pZSubtask = m_pPainterWindow->m_pBar->m_pZSubtask;

   if ( pch && *pch )    // DLL_Name
   {
      hLibrary = SysLoadLibraryWithErrFlag( pZSubtask->m_vDialog,
                                            pch, zLOADLIB_RESOURCES );
      hInst = (HINSTANCE) SysGetInstance( hLibrary );
      pch += zstrlen( pch ) + 1;  // skip past DLL name
   }
   else
   {
      hLibrary = 0;
      if ( pch )
         pch++;      // skip past null DLL name
   }

   if ( pch && *pch )
   {
      zPCHAR pchSemiColon = zstrchr( pch, ';' );

      if ( pchSemiColon )
         pch = pchSemiColon + 1;

      if ( hInst )
      {
         m_hIcon = (HICON) ::LoadImage( hInst, pch, IMAGE_ICON,
                                        0, 0, LR_LOADFROMFILE );
      }

      if ( m_hIcon == 0 || m_hIcon == (HICON) 1 )
      {
         if ( *pch == '.' && *(pch + 1) == '\\' )
            pch += 2;

         m_hIcon = ::ExtractIcon( hInst, pch, 0 );
         if ( m_hIcon == 0 || m_hIcon == (HICON) 1 &&
              pch[ 0 ] && pch[ 1 ] != ':' )
         {
            zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
            zCHAR  szWorkString[ zMAX_FILENAME_LTH ];
            zVIEW vApp;
            LPAPP pApp;

            m_pPainterWindow->m_pBar->CreateAppView( &vApp );
            if ( vApp &&
                 SfGetApplicationForSubtask( &pApp, vApp ) == 0 && pApp )
            {
               SysConvertEnvironmentString( szZeidonPath, sizeof( szZeidonPath ), pApp->szLocalDir );
            }
            else
            {
               zPCHAR pchExecDir;
               zVIEW  vTaskLPLR;

               GetViewByName( &vTaskLPLR, "TaskLPLR", pZSubtask->m_vDialog, zLEVEL_TASK );
               GetAddrForAttribute( &pchExecDir, vTaskLPLR, "LPLR", "ExecDir" );
               SysConvertEnvironmentString( szZeidonPath, sizeof( szZeidonPath ), pchExecDir );
            }

            if ( szZeidonPath[ 0 ] )
            {
               SysAppendcDirSep( szZeidonPath );
               strcat_s( szZeidonPath, sizeof( szZeidonPath ), pch );
               HICON hIcon = ::ExtractIcon( hInst, szZeidonPath, 0 );
               if ( hIcon && hIcon != (HICON) 1 )
                  m_hIcon = hIcon;
            }

            if ( m_hIcon == 0 || m_hIcon == (HICON) 1 )
            {
               SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, sizeof( szZeidonPath ) );
               if ( szZeidonPath[ 0 ] )
               {
                  SysAppendcDirSep( szZeidonPath );
                  strcat_s( szZeidonPath, sizeof( szZeidonPath ), pch );
                  SysConvertEnvironmentString( szWorkString, sizeof( szWorkString ), szZeidonPath );
                  HICON hIcon = ::ExtractIcon( hInst, szWorkString, 0 );
                  if ( hIcon && hIcon != (HICON) 1 )
                     m_hIcon = hIcon;
               }
            }

            if ( m_hIcon == (HICON) 1 )
            {
               m_hIcon = 0;
               TraceLineS( "TZIcon::ctor: Invalid icon file ", pch );
            }
            else
            if ( m_hIcon == 0 )
            {
               TraceLineS( "TZIcon::ctor: Cannot access icon file ", pch );
            }
         }
      }
   }

   CreateZ( );
}

void
TZIcon::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZIcon::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_STATICEDGE, "ZeidonCustomCtrl", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

void
TZIcon::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZIcon::OnPaint: %s ====>", m_csTag );
   TraceRect( szMsg, m_rectPainter );
#endif

   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
// int nOldBkMode = dc.SetBkMode( TRANSPARENT );
   CRect rect;

// GetWindowRect( rect );
// TraceRect( "TZIcon::OnPaint Window rect", rect );
   GetUpdateRect( rect, 1 );
// TraceRect( "TZIcon::OnPaint Update rect", rect );
// CBrush brush( dc.GetBkColor( ) );
   CBrush brush( ::GetSysColor( COLOR_BTNFACE ) );
   dc.FillRect( rect, &brush );

   if ( m_hIcon )
   {
      dc.DrawIcon( rect.left + m_nIconEdgeX,
                   rect.top + m_nIconEdgeY, m_hIcon );
   }

// dc.SetBkMode( nOldBkMode );
}

int
TZIcon::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZIcon::OnCreate: ", m_csTag );
#endif

   if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// TZIcon - dtor
TZIcon::~TZIcon( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZIcon::dtor: ", m_csTag );
#endif

// if ( m_hIcon )
//    delete( m_hIcon );

   mDeleteInitA( m_pchCtrlBOI );
   mDeleteInitA( m_pchMsgText );

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZBitmap, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor

TZBitmap::TZBitmap( TZPainterWindow *pPainterWindow,
                    TZPainterCtrl   *pCtrlParent,
                    CWnd    *pWndParent,
                    CRect&  rect,
                    zVIEW   vCtrl,
                    zLONG   lType,
                    zBOOL   bPlaceHolder,
                    zLONG   lZKey,
                    zLONG   lTabStopNbr,
                    zLONG   lCtrlID,
                    zPCHAR  pchTag,
                    zPCHAR  pchText ) :
          CBitmapButton( ),
          TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                         rect, vCtrl, this, pchTag, pchText,
                         lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBitmap::ctor: ", m_csTag );
#endif

   m_pDib = 0;
   m_pchCtrlBOI = 0;
   m_pchMsgText = 0;

   Attr.Style |= BS_BITMAP | BS_OWNERDRAW;

   if ( vCtrl )
   {
      zULONG  ulLth;

      GetAttributeLength( &ulLth, vCtrl, szlControl, szlCtrlBOI );
      if ( ulLth > 0 )
      {
         zPCHAR    pch;

         GetAddrForAttribute( &pch, vCtrl, szlControl, szlCtrlBOI );
         m_pchCtrlBOI = new char[ ulLth ];
         zmemcpy( m_pchCtrlBOI, (zPVOID) pch, ulLth );
         zLONG lLth = zstrlen( pchText ) + 1;
         m_pchMsgText = new char[ lLth ];
         strcpy_s( m_pchMsgText, lLth, pchText );
      }
   }
   else
      m_lSubtype = zCONTROL_BORDEROFF;

   CreateZ( );
}

void
TZBitmap::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBitmap::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "BUTTON", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZBitmap::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBitmap::OnCreate: ", m_csTag );
#endif

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );

/////////////////////////////////////////////////////////////////////////////
//
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
//    If successful, m_pDib will be non-zero.  The return pointer is
//    the pointer to the next bitmap specification in the Ctrl BOI
//
/////////////////////////////////////////////////////////////////////////////

   LPLIBRARY hLibrary = 0;
   HINSTANCE hInst = 0;
   zPCHAR    pch = m_pchCtrlBOI;

   if ( pch && *pch )   // DLL_Name
   {
      // We need the correct view so that the DLL name is correctly
      // qualified!!!
      zVIEW vTaskLPLR;
      zCHAR  szDir[ 512 ];

      GetViewByName( &vTaskLPLR, "TaskLPLR", m_vCtrl, zLEVEL_TASK );
      GetStringFromAttribute( szDir, sizeof( szDir ), vTaskLPLR, "LPLR", "ExecDir" );
      zULONG  ulLth = zstrlen( szDir );
      if ( ulLth && szDir[ ulLth ] != '\\' )
         szDir[ ulLth++ ] = '\\';

      strcpy_s( szDir + ulLth, sizeof( szDir ) - ulLth, pch );
      hLibrary = SysLoadLibraryWithErrFlag( m_vCtrl, szDir, zLOADLIB_RESOURCES );
      if ( hLibrary == 0 )
      {
         // Try to load the DLL via normal path.
         hLibrary = SysLoadLibraryWithErrFlag( m_vCtrl, pch, zLOADLIB_RESOURCES );
      }

      hInst = (HINSTANCE) SysGetInstance( hLibrary );
      pch += zstrlen( pch ) + 1;  // skip past DLL name
   }
   else
   {
      hLibrary = 0;
      if ( pch )
         pch++;      // skip past null DLL name
   }

// TraceLineX( "TZBitmap::OnCreate hInst: ", (zLONG) hInst );

   pch = LoadDib( hInst, pch );
   if ( hLibrary )
      SysFreeLibrary( m_vCtrl, hLibrary );

   mDeleteInitA( m_pchCtrlBOI );
   return( nRC );
}

zPCHAR
TZBitmap::LoadDib( HINSTANCE hInstDLL,
                   zPCHAR    pchBitmap )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBitmap::LoadDib: ", m_csTag );
#endif

   if ( pchBitmap == 0 || *pchBitmap == 0 )  // nothing left in the Ctrl BOI
      return( 0 );

   m_pDib = new ZDib;
   zPCHAR pchSemiColon = zstrchr( pchBitmap, ';' );
   zPCHAR pchReturn = pchBitmap + zstrlen( pchBitmap ) + 1;

   if ( pchSemiColon )
      pchSemiColon++;
   else
      pchSemiColon = pchBitmap;

   if ( hInstDLL )
   {
      UINT uResId;

      if ( (isdigit( *pchBitmap ) || (pchSemiColon != 0)) &&
           (uResId = (zUSHORT) zatol( pchBitmap )) != 0 )
      {
         if ( m_pDib->Load( hInstDLL, uResId ) )
         {
            return( pchReturn );
         }
         else
         {
            TraceLineI( "TZBitmap::LoadDib: Cannot access bitmap res id ", uResId );
         }
      }

      if ( m_pDib->Load( hInstDLL, pchSemiColon ) )
      {
         return( pchReturn );
      }
      else
      {
        TraceLineS( "TZBitmap::LoadDib: Cannot access bitmap resource ", pchSemiColon );
      }
   }

   if ( pchSemiColon )
   {
      if ( *pchSemiColon == '.' && *(pchSemiColon + 1) == '\\' )
         pchSemiColon += 2;

      if ( m_pDib->Load( pchSemiColon ) == FALSE &&
           pchSemiColon[ 0 ] && pchSemiColon[ 1 ] != ':' )
      {
         zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
         zCHAR  szWorkString[ zMAX_FILENAME_LTH ];
         zVIEW vApp;
         LPAPP pApp;

         m_pPainterWindow->m_pBar->CreateAppView( &vApp );
         if ( vApp && SfGetApplicationForSubtask( &pApp, vApp ) == 0 && pApp )
         {
            SysConvertEnvironmentString( szZeidonPath, sizeof( szZeidonPath ), pApp->szLocalDir );
         }
         else
         {
            zPCHAR pchExecDir;
            zVIEW  vTaskLPLR;

            GetViewByName( &vTaskLPLR, "TaskLPLR", m_vCtrl, zLEVEL_TASK );
            GetAddrForAttribute( &pchExecDir, vTaskLPLR, "LPLR", "ExecDir" );
            SysConvertEnvironmentString( szZeidonPath, sizeof( szZeidonPath ), pchExecDir );
         }

         if ( szZeidonPath[ 0 ] )
         {
            SysAppendcDirSep( szZeidonPath );
            strcat_s( szZeidonPath, sizeof( szZeidonPath ), pchSemiColon );
            if ( m_pDib->Load( szZeidonPath ) )
               return( pchReturn );

            TraceLineS( "TZBitmap::LoadDib: Cannot access bitmap file in TaskLPLR ", szZeidonPath );
         }

         SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, sizeof( szZeidonPath ) );
         if ( szZeidonPath[ 0 ] )
         {
            SysAppendcDirSep( szZeidonPath );
            strcat_s( szZeidonPath, sizeof( szZeidonPath ), pchSemiColon );
            SysConvertEnvironmentString( szWorkString, sizeof( szWorkString ), szZeidonPath );
            if ( m_pDib->Load( szWorkString ) )
               return( pchReturn );

            TraceLineS( "TZBitmap::LoadDib: Cannot access bitmap file in ResourcePath ", szWorkString );
         }

         mDeleteInit( m_pDib );
         TraceLineS( "TZBitmap::LoadDib: Cannot access bitmap ile ", pchSemiColon );
      }
   }

   return( pchReturn );
}

//
// DrawItem - virtual method overload required for all owner draw buttons,
//            this method draws the current button and de-selects the
//            currently selected modal button if the current button is
//            being selected (pushed) and is a modal button.
//
void
TZBitmap::DrawItem( LPDRAWITEMSTRUCT pDrawInfo )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZBitmap::DrawItem: ", m_csTag );
#endif

   // if we are not going to draw anything, return
   if ( !(pDrawInfo->itemAction & (ODA_DRAWENTIRE | ODA_SELECT | ODA_FOCUS)) )
   {
      return;
   }

   if ( m_pDib == 0 )
      return;

   // now that we have determined which state to draw in, draw the button

   // get the size of the button being drawn
   CDC *pDC = CDC::FromHandle( pDrawInfo->hDC );
   CRect rect( pDrawInfo->rcItem );
   if ( m_pDib->m_bBitmapLoaded == FALSE ||
      m_pDib->Draw( (CDC &) *pDC, &rect,
                    (m_lType == zTOOLBAR_CONTROL) ? &rect: 0,
                    FALSE, 0, TRUE ) == FALSE )
   {
      if ( m_pchMsgText && *m_pchMsgText )
         pDC->DrawText( m_pchMsgText, -1, rect, DT_CENTER | DT_WORDBREAK );
      else
         pDC->DrawText( "Unknown BmpCtl", -1, rect, DT_CENTER | DT_WORDBREAK );
   }
}

// dtor
TZBitmap::~TZBitmap( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBitmap::dtor: ", m_csTag );
#endif

   mDeleteInitA( m_pchCtrlBOI );
   mDeleteInitA( m_pchMsgText );
   mDeleteInit( m_pDib );

   DestroyWindow( );
}

BOOL
TZBitmap::OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                             VARIANT *pvar )
{
   if ( m_pPainterWindow && m_pPainterWindow->m_pPainterClient )
   {
      return( m_pPainterWindow->m_pPainterClient->OnAmbientProperty( pSite, dispid, pvar ) );
   }
   else
   {
      return( CBitmapButton::OnAmbientProperty( pSite, dispid, pvar ) );
   }
}

/////////////////////////////////////////////////////////////////////////////
// TZTimer, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor

TZTimer::TZTimer( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd    *pWndParent,
                  CRect&  rect,
                  zVIEW   vCtrl,
                  zLONG   lType,
                  zBOOL   bPlaceHolder,
                  zLONG   lZKey,
                  zLONG   lTabStopNbr,
                  zLONG   lCtrlID,
                  zPCHAR  pchTag,
                  zPCHAR  pchText ) :
         CBitmapButton( ),
         TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                        rect, vCtrl, this, pchTag, pchText,
                        lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTimer::ctor: ", m_csTag );
#endif

   m_pDib = 0;
   Attr.Style |= BS_BITMAP | BS_OWNERDRAW;
   CreateZ( );
}

void
TZTimer::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTimer::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "BUTTON", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZTimer::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTimer::OnCreate: ", m_csTag );
#endif

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

// TraceLineX( "TZTimer::OnCreate hInst: ", (zLONG) hInst );

   HINSTANCE hInst = 0;
   LPLIBRARY hLibrary = SysLoadLibraryWithErrFlag( m_vCtrl, "tzctl",
                                                   zLOADLIB_RESOURCES );
   hInst = (HINSTANCE) SysGetInstance( hLibrary );

   m_pDib = new ZDib;
   m_pDib->Load( hInst, IDB_TIMER );
   return( nRC );
}

//
// DrawItem - virtual method overload required for all owner draw buttons,
//            this method draws the current button and de-selects the
//            currently selected modal button if the current button is
//            being selected (pushed) and is a modal button.
//
void
TZTimer::DrawItem( LPDRAWITEMSTRUCT pDrawInfo )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZTimer::DrawItem: ", m_csTag );
#endif

   // if we are not going to draw anything, return
   if ( !(pDrawInfo->itemAction & (ODA_DRAWENTIRE | ODA_SELECT | ODA_FOCUS)) )
   {
      return;
   }

   if ( m_pDib == 0 )
      return;

   // now that we have determined which state to draw in, draw the button

   // get the size of the button being drawn
   CDC *pDC = CDC::FromHandle( pDrawInfo->hDC );
   CRect rect( pDrawInfo->rcItem );
   if ( m_pDib->m_bBitmapLoaded == FALSE ||
        m_pDib->Draw( (CDC &) *pDC, &rect, 0, FALSE, 0, TRUE ) == FALSE )
   {
      pDC->DrawText( m_csTag, -1, rect, DT_CENTER | DT_WORDBREAK );
   }
}

// dtor
TZTimer::~TZTimer( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTimer::dtor: ", m_csTag );
#endif

   mDeleteInit( m_pDib );
   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZRadio, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZRadio::TZRadio( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd    *pWndParent,
                  CRect&  rect,
                  zVIEW   vCtrl,
                  zLONG   lType,
                  zBOOL   bPlaceHolder,
                  zLONG   lZKey,
                  zLONG   lTabStopNbr,
                  zLONG   lCtrlID,
                  zPCHAR  pchTag,
                  zPCHAR  pchText ) :
         CButton( ),
         TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                        rect, vCtrl, this, pchTag, pchText,
                        lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZRadio::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF;

   Attr.Style |= BS_RADIOBUTTON;
   CreateZ( );
}

void
TZRadio::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZRadio::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "BUTTON", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZRadio::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZRadio::OnCreate: ", m_csTag );
#endif

   if ( CButton::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZRadio::~TZRadio( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZRadio dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZCheck, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZCheck::TZCheck( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd    *pWndParent,
                  CRect&  rect,
                  zVIEW   vCtrl,
                  zLONG   lType,
                  zBOOL   bPlaceHolder,
                  zLONG   lZKey,
                  zLONG   lTabStopNbr,
                  zLONG   lCtrlID,
                  zPCHAR  pchTag,
                  zPCHAR  pchText ) :
         CButton( ),
         TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                        rect, vCtrl, this, pchTag, pchText,
                        lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCheck::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF;

   Attr.Style |= BS_CHECKBOX;
   CreateZ( );
}

void
TZCheck::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCheck::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "BUTTON", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZCheck::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCheck::OnCreate: ", m_csTag );
#endif

   if ( CButton::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZCheck::~TZCheck( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCheck::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZComboBox, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZComboBox::TZComboBox( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CComboBox( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZComboBox::ctor: ", m_csTag );
#endif

   // first call out base class create
   Attr.Style |= CBS_DROPDOWNLIST;
   CreateZ( );
}

void
TZComboBox::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZComboBox::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "COMBOBOX", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZComboBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZComboBox::OnCreate: ", m_csTag );
#endif

   if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

void
TZComboBox::EndPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZComboBox::EndPaint: ", m_csTag );
#endif
}

void
TZComboBox::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZComboBox::OnPaint: %s ====>", m_csTag );
   TraceRect( szMsg, m_rectPainter );
#endif

   if ( m_pCtrlCover )
   {
      CRect  rect;

      m_pCtrlCover->GetClientRect( rect );
      CBrush brush( ::GetSysColor( COLOR_BTNFACE ) );
      CClientDC dc( m_pCtrlCover );
      dc.FillRect( &rect, &brush );
   }

   CComboBox::OnPaint( );
}

#if 0
BOOL
TZComboBox::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZComboBox::DestroyWindow: ", m_csTag );
#endif

   return( CComboBox::DestroyWindow( ) );
}
#endif

// dtor
TZComboBox::~TZComboBox( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZComboBox::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZListBox, class implementation
/////////////////////////////////////////////////////////////////////////////

// ctor
TZListBox::TZListBox( TZPainterWindow *pPainterWindow,
                      TZPainterCtrl   *pCtrlParent,
                      CWnd    *pWndParent,
                      CRect&  rect,
                      zVIEW   vCtrl,
                      zLONG   lType,
                      zBOOL   bPlaceHolder,
                      zLONG   lZKey,
                      zLONG   lTabStopNbr,
                      zLONG   lCtrlID,
                      zPCHAR  pchTag,
                      zPCHAR  pchText ) :
           CListBox( ),
           TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                          rect, vCtrl, this, pchTag, pchText,
                          lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZListBox::ctor: ", m_csTag );
#endif

// Attr.Style |= SS_BLACKFRAME;
   Attr.Style |= WS_VSCROLL;
   CreateZ( );
}

void
TZListBox::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZListBox::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "LISTBOX", m_csText,
             Attr.Style | LBS_NOINTEGRALHEIGHT,
             Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );

   for ( int k = 0; k < 75; k++ )
      AddString( " " );
}

int
TZListBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZListBox::OnCreate: ", m_csTag );
#endif

   if ( CListBox::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

#if 0
BOOL
TZListBox::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZListBox::DestroyWindow: ", m_csTag );
#endif

   return( CListBox::DestroyWindow( ) );
}
#endif

TZListBox::~TZListBox( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZListBox::dtor: ", m_csTag );
#endif

   // destroy the scroll bar
   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZGraph, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZGraph::TZGraph( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd    *pWndParent,
                  CRect&  rect,
                  zVIEW   vCtrl,
                  zLONG   lType,
                  zBOOL   bPlaceHolder,
                  zLONG   lZKey,
                  zLONG   lTabStopNbr,
                  zLONG   lCtrlID,
                  zPCHAR  pchTag,
                  zPCHAR  pchText ) :
         CEdit( ),
         TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                        rect, vCtrl, this, pchTag, pchText,
                        lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGraph::ctor: ", m_csTag );
#endif

   CreateZ( );
}

void
TZGraph::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGraph::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "EDIT", m_csText,
             Attr.Style | WS_BORDER, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZGraph::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGraph::OnCreate: ", m_csTag );
#endif

   if ( CEdit::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZGraph::~TZGraph( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGraph dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZGroupBox, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZGroupBox::TZGroupBox( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CButton( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupBox::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;

   Attr.Style |= BS_GROUPBOX;
   CreateZ( );
}

void
TZGroupBox::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupBox::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZGroupBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupBox::OnCreate: ", m_csTag );
#endif

   if ( CButton::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

void
TZGroupBox::RealizeCtrl( zBOOL bCreate )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZGroupBox::RealizeCtrl bCreate = ", bCreate );
#endif
   if ( bCreate )
   {
      TZPainterCtrl::RealizeCtrl( TRUE );
   // TZNotePage *pCurrNotePage = GetActivePage( );
   // if ( pCurrNotePage )
   // {
   //    ActivateTab( pCurrNotePage->m_nTabNbr );
   // }
   }

   if ( m_pWndCtrl && mIs_hWnd( m_pWndCtrl->m_hWnd ) )
      m_pWndCtrl->ShowWindow( bCreate ? SW_SHOW : SW_HIDE );
}

// dtor
TZGroupBox::~TZGroupBox( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupBox::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

#if 0
void
TZGroupBox::OnDrawItem( int nCtrlId, LPDRAWITEMSTRUCT pDrawInfo )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZGroupBox::OnDrawItem: ", m_csTag );
#endif

   if ( pDrawInfo->hwndItem != m_hWnd )
   {
      if ( ReflectLastMsg( pDrawInfo->hwndItem ) )
         return;     // eat it
   }

   Default( );
}
#endif

void
TZGroupBox::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZGroupBox::OnPaint: %s ====>", m_csTag );
   TraceRect( szMsg, m_rectPainter );
#endif

   CRect  rect;

   BOOL bUpdate = GetUpdateRect( rect, 1 );
// TraceRect( "TZGroupBox::OnPaint rect", rect );
   if ( bUpdate )
   {
      CBrush brush( ::GetSysColor( COLOR_BTNFACE ) );
      CClientDC dc( this );
      dc.FillRect( &rect, &brush );

      ClientToScreen( rect );
   // TraceRect( "ZGroupBox::OnPaint ClientToScreenRect", rect );

      CRect   rectCtrl;
      TZPainterCtrl *pCtrl;
      zSHORT  nPos;

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent == this )
         {
            if ( pCtrl->m_pWndCtrl &&
                 mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
            {
               pCtrl->m_pWndCtrl->GetWindowRect( rectCtrl );
            // TraceRect( pCtrl->m_csText, rectCtrl );
               if ( rectCtrl.IntersectRect( rectCtrl, rect ) )
                  pCtrl->m_pWndCtrl->Invalidate( );
            }
         }
      }
   }

   CButton::OnPaint( );
}

BOOL
TZGroupBox::OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                               VARIANT *pvar )
{
   if ( m_pPainterWindow && m_pPainterWindow->m_pPainterClient )
   {
      return( m_pPainterWindow->m_pPainterClient->
                                 OnAmbientProperty( pSite, dispid, pvar ) );
   }
   else
   {
      return( CButton::OnAmbientProperty( pSite, dispid, pvar ) );
   }
}

/////////////////////////////////////////////////////////////////////////////
// TZMLEdit, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZMLEdit::TZMLEdit( TZPainterWindow *pPainterWindow,
                    TZPainterCtrl   *pCtrlParent,
                    CWnd    *pWndParent,
                    CRect&  rect,
                    zVIEW   vCtrl,
                    zLONG   lType,
                    zBOOL   bPlaceHolder,
                    zLONG   lZKey,
                    zLONG   lTabStopNbr,
                    zLONG   lCtrlID,
                    zPCHAR  pchTag,
                    zPCHAR  pchText ) :
          CEdit( ),
          TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                         rect, vCtrl, this, pchTag, pchText,
                         lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLEdit::ctor: ", m_csTag );
#endif

   if ( pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
      Attr.Style |= WS_BORDER;

   Attr.Style |= WS_VSCROLL | WS_HSCROLL | ES_MULTILINE;

   CreateZ( );
}

void
TZMLEdit::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLEdit::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "EDIT", m_csTag,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
// SetWindowText( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
//                "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" );
}

int
TZMLEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLEdit::OnCreate: ", m_csTag );
#endif

   if ( CEdit::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZMLEdit::~TZMLEdit( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLEdit::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZDil, class implementation
/////////////////////////////////////////////////////////////////////////////

// ctor
TZDil::TZDil( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd    *pWndParent,
              CRect&  rect,
              zVIEW   vCtrl,
              zLONG   lType,
              zBOOL   bPlaceHolder,
              zLONG   lZKey,
              zLONG   lTabStopNbr,
              zLONG   lCtrlID,
              zPCHAR  pchTag,
              zPCHAR  pchText ) :
       CStatic( ),
       TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                      rect, vCtrl, this, pchTag, pchText,
                      lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDil::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = 1;  // special case for ConvertDIL

// Attr.Style |= SS_BLACKRECT;
   CreateZ( );
}

void
TZDil::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDil::CreateZ: ", m_csTag );
#endif

// CRect rect;
//
// m_pPainterWindow->GetClientRect( rect );
   CreateEx( WS_EX_CLIENTEDGE, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZDil::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDil::OnCreate: ", m_csTag );
#endif

   if ( CStatic::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   return( 0 );
}

TZDil::~TZDil( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZDil::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZGrid, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZGrid::TZGrid( TZPainterWindow *pPainterWindow,
                TZPainterCtrl   *pCtrlParent,
                CWnd    *pWndParent,
                CRect&  rect,
                zVIEW   vCtrl,
                zLONG   lType,
                zBOOL   bPlaceHolder,
                zLONG   lZKey,
                zLONG   lTabStopNbr,
                zLONG   lCtrlID,
                zPCHAR  pchTag,
                zPCHAR  pchText ) :
        CListBox( ),
        TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                       rect, vCtrl, this, pchTag, pchText,
                       lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGrid::ctor: ", m_csTag );
#endif

   Attr.Style |= WS_VSCROLL | WS_HSCROLL;
   CreateZ( );
}

void
TZGrid::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGrid::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_CLIENTEDGE, "LISTBOX", m_csText,
             Attr.Style | LBS_NOINTEGRALHEIGHT,
             Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );

   for ( int k = 0; k < 75; k++ )
      AddString( " " );

   SetHorizontalExtent( 32000 );
}

int
TZGrid::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGrid::OnCreate: ", m_csTag );
#endif

   if ( CListBox::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

TZGrid::~TZGrid( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGrid::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZTree, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZTree::TZTree( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CStatic( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTree::ctor: ", m_csTag );
#endif

   Attr.Style |= WS_BORDER; // | SS_GRAYFRAME;
   CreateZ( );

   if ( *pchText == 0 )
      SetWindowText( pchTag );

#if 0
   zSHORT x;
   zSHORT y;

   TDC *hDC = new TClientDC( m_hWnd );

   x = rect.left + nDlgUnitCharX;
   y = rect.top + nDlgUnitCharY;

   while ( x < rect.right && y < rect.bottom )
   {
      // draw box
      hDC->MoveTo( x, y );
      hDC->LineTo( x + 2 * nDlgUnitCharX, y );
      hDC->LineTo( x + 2 * nDlgUnitCharX, y + nDlgUnitCharY );
      hDC->LineTo( x, y + nDlgUnitCharY );
      hDC->LineTo( x, y );

      // draw "-"
      hDC->MoveTo( x + nDlgUnitCharX / 2, y + nDlgUnitCharY / 2 );
      hDC->MoveTo( x + nDlgUnitCharX * 3 / 2, y + nDlgUnitCharY / 2 );

      x += 3 * nDlgUnitCharX;
      y += 2 * nDlgUnitCharY;
   }

   delete( hDC );
#endif
}

void
TZTree::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTree::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZTree::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTree::OnCreate: ", m_csTag );
#endif

   if ( CStatic::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

// dtor
TZTree::~TZTree( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTree::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZERD, class implementation
/////////////////////////////////////////////////////////////////////////////

// ctor
TZERD::TZERD( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd    *pWndParent,
              CRect&  rect,
              zVIEW   vCtrl,
              zLONG   lType,
              zBOOL   bPlaceHolder,
              zLONG   lZKey,
              zLONG   lTabStopNbr,
              zLONG   lCtrlID,
              zPCHAR  pchTag,
              zPCHAR  pchText ) :
       CEdit( ),
       TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                      rect, vCtrl, this, pchTag, pchText,
                      lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZERD::ctor: ", m_csTag );
#endif

   CreateZ( );
}

void
TZERD::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZERD::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "EDIT", m_csText,
             Attr.Style | WS_BORDER, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZERD::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZERD::OnCreate: ", m_csTag );
#endif

   if ( CEdit::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

TZERD::~TZERD( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZERD::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// the report painter forces a Report Header and Report Footer GroupSet
// to exist ... the user cannot create or delete them.  Report Header
// and Report Footers each have exactly one Group (of type Detail).
// Other GroupSets permit 1 Group Header and 1 Group Footer (there may
// be no Header or Footer) and permit as many Detail Groups as desired.
// If there are no Groups within a GroupSet, the GroupSet may be deleted.


/////////////////////////////////////////////////////////////////////////////
// TZGroupSetRpt, class implementation
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( TZGroupSetRpt, CStatic )
BEGIN_MESSAGE_MAP( TZGroupSetRpt, CStatic )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

// ctor
TZGroupSetRpt::TZGroupSetRpt( TZPainterWindow *pPainterWindow,
                              TZPainterCtrl   *pCtrlParent,
                              CWnd    *pWndParent,
                              CRect&  rect,
                              zVIEW   vCtrl,
                              zLONG   lType,
                              zBOOL   bPlaceHolder,
                              zLONG   lZKey,
                              zLONG   lTabStopNbr,
                              zLONG   lCtrlID,
                              zPCHAR  pchTag,
                              zPCHAR  pchText ) :
               CStatic( ),
               TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                              rect, vCtrl, this, pchTag, pchText,
                              lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID, 'S' )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::ctor: ", m_csTag );
#endif

   zCHAR szTemp[ 32 ];

   if ( vCtrl )
   {
      GetIntegerFromAttribute( &m_lGrpSetFlags, vCtrl, szlGroup, "Flags" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vCtrl, szlGroupSet, szlType );
      if ( szTemp[ 0 ] )
         m_chType = toupper( szTemp[ 1 ] );
      else
      {
         TraceLineS( "TZGroupSetRpt::ctor invalid null type: ", m_csTag );
         m_chType = 0;
      }
   }
   else
   {
      m_lGrpSetFlags = 0;
      m_chType = 0;
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;
   }

   if ( m_chType == 'H' )
   {
      m_csLabel = "Page Header";
   }
   else
   if ( m_chType == 'F' )
   {
      m_csLabel = "Page Footer";
   }
   else
// if ( m_chType == 'E' )
   {
      if ( m_vCtrl )
      {
         zPCHAR pch;

         GetAddrForAttribute( &pch, vCtrl, szlGroupSet, szlTag );
         if ( *pch )
            m_csLabel = pch;
         else
            m_csLabel = "GroupSet (Unknown)";
      }
      else
         m_csLabel = "GroupSet (Unknown)";

      m_chType = 'E';
   }

// Attr.Style |= WS_BORDER | SS_GRAYFRAME;
   CreateZ( );

#if 0
   zSHORT x;
   zSHORT y;

   TDC *hDC = new TClientDC( m_hWnd );

   x = rect.left + nDlgUnitCharX;
   y = rect.top + nDlgUnitCharY;

   while ( x < rect.right && y < rect.bottom )
   {
      // draw box
      hDC->MoveTo( x, y );
      hDC->LineTo( x + 2 * nDlgUnitCharX, y );
      hDC->LineTo( x + 2 * nDlgUnitCharX, y + nDlgUnitCharY );
      hDC->LineTo( x, y + nDlgUnitCharY );
      hDC->LineTo( x, y );

      // draw "-"
      hDC->MoveTo( x + nDlgUnitCharX / 2, y + nDlgUnitCharY / 2 );
      hDC->MoveTo( x + nDlgUnitCharX * 3 / 2, y + nDlgUnitCharY / 2 );

      x += 3 * nDlgUnitCharX;
      y += 2 * nDlgUnitCharY;
   }

   delete( hDC );
#endif
}

int
TZGroupSetRpt::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::OnCreate: ", m_csTag );
#endif

   if ( (int) Default( ) == -1 )
      return( -1 );

   zCHAR  szTag[ zTAG_LTH ];
   CClientDC dc( this );

   MakeShortString( &dc, szTag, sizeof( szTag ), m_csTag, m_pPainterWindow->m_lLeftMargin - 8, 0 );
   SetWindowText( szTag );

// ModifyStyleEx( 0, WS_EX_TRANSPARENT );
   return( 0 );
}

void
TZGroupSetRpt::SetupCtrlCover( )   // first chance the control has to know
{                                  // its corresponding pCtrlCover
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::SetupCtrlCover: ", m_csTag );
#endif

   if ( m_chType == 'H' )
   {
      m_pPainterWindow->m_pPainterClient->m_pPageHeader = m_pCtrlCover;
   }
   else
   if ( m_chType == 'F' )
   {
      m_pPainterWindow->m_pPainterClient->m_pPageFooter = m_pCtrlCover;
   }
   else
      m_chType = 'E';

// SetWindowText( m_csTag );
   m_pPainterWindow->m_pPainterClient->AdjustGroupSets( );
}

TZPainterCtrl *
TZGroupSetRpt::IsDeletable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::IsDeletable: ", m_csTag );
#endif

   // TMV-make GroupSet deletable even if it is a header or footer.
// if ( m_chType != 'H' && m_chType != 'F' &&
   if ( TZPainterCtrl::IsDeletable( ) )
   {
      TZPainterCtrl *pCtrl;
      zSHORT        nPos;

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent == this )
            return( 0 );   // cannot be deleted if there is a child group
      }

      return( this );
   }

   return( 0 );
}

zBOOL
TZGroupSetRpt::IsChildValid( zLONG lChildId )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::IsChildValid: ", m_csTag );
#endif

   if ( m_chType == 'H' || m_chType == 'F' )
   {
      if ( m_nChildren > 1 )
         return( FALSE );
   }

   return( TRUE );
}

void
TZGroupSetRpt::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZGroupSetRpt::OnPaint: %s ====>", m_csTag );
   TraceRect( szMsg, m_rectPainter );
#endif

   CStatic::OnPaint( );

   if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
   {
      CClientDC dc( this );
      CRect  rect;

      GetClientRect( rect );
      rect.right = m_pPainterWindow->m_lLeftMargin - 4;

      COLORREF colorShadow( ::GetSysColor( COLOR_BTNSHADOW ) );
//    CPen penGray( PS_DASH, 1, colorShadow );
//    CPen penBlack( PS_DASH, 1, RGB( 0, 0, 0 ) );
      CPen penRed( PS_INSIDEFRAME, 3, zRGB_RED );
      CPen penBlue( PS_INSIDEFRAME, 3, zRGB_BLUE );
      CPen penGray( PS_INSIDEFRAME, 3, colorShadow );
      CPen penBlack( PS_INSIDEFRAME, 3, RGB( 0, 0, 0 ) );
      CPen *pPenOld = dc.SelectObject( &penBlack );
      dc.MoveTo( rect.right - 3, rect.top );
      dc.LineTo( rect.right - 3, rect.bottom );
      dc.SelectObject( &penGray );
      dc.MoveTo( rect.right - 2, rect.top );
      dc.LineTo( rect.right - 2, rect.bottom );

      TZPainterCtrl *pGroupSet;
      TZPainterCtrl *pGroup;
      TZPainterCtrl *pGroupCtrl;
      TZPainterCtrl *pCtrl;
      zLONG  lRangeTop;
      zLONG  lRangeBottom;
      zLONG  lTop = rect.top;
      zBOOL  bRed;
      zSHORT nPos;

      // Find this GroupSet in the list of ctrls.
      for ( nPos = 0;
            nPos < m_pPainterWindow->m_nCtrlCnt;
            nPos++ )
      {
         pGroupSet = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pGroupSet != this )   // wrong GroupSet
            continue;

         // Process each Group within this GroupSet.
         for ( nPos = nPos + 1; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
         {
            pGroupSet = m_pPainterWindow->m_CtrlList[ nPos ];
            if ( pGroupSet->m_pCtrlParent &&
                 pGroupSet->m_pCtrlParent->m_pCtrlParent == 0 )
            {
               pGroupCtrl = pGroupSet;
            }
            else
            {
               pGroupCtrl = 0;
            }

            pGroup = pGroupSet;
            while ( pGroupSet->m_pCtrlParent )
            {
               if ( pGroupSet->m_pCtrlParent->m_pCtrlParent )
                  pGroup = pGroupSet->m_pCtrlParent;

               pGroupSet = pGroupSet->m_pCtrlParent;
            }

            if ( pGroupSet != this )
            {
               nPos = m_pPainterWindow->m_nCtrlCnt;   // get out
               break;
            }

            bRed = FALSE;
            lRangeTop = -2;
            lRangeBottom = 0;

            // Mark the range for ctrls within this Group.
            while ( pGroup == pGroupCtrl &&
                    nPos < m_pPainterWindow->m_nCtrlCnt )
            {
               for ( nPos = nPos + 1;
                     nPos < m_pPainterWindow->m_nCtrlCnt;
                     nPos++ )
               {
                  pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
                  pGroupCtrl = pCtrl;
                  pGroupSet = pCtrl;
                  while ( pGroupSet->m_pCtrlParent )
                  {
                     if ( pGroupSet->m_pCtrlParent->m_pCtrlParent )
                        pGroupCtrl = pGroupSet->m_pCtrlParent;

                     pGroupSet = pGroupSet->m_pCtrlParent;
                  }

                  if ( pGroupSet != this )
                  {
                     nPos = m_pPainterWindow->m_nCtrlCnt;   // get out
                     pGroup = 0;
                     break;
                  }

                  if ( pGroup != pGroupCtrl )
                  {
                     pGroup = pGroupCtrl; // process next group
                     bRed = FALSE;
                     lRangeTop = 0;
                     lRangeBottom = 0;
                     break;
                  }

                  lTop = pGroup->m_rectCtrl.top + 1;
                  if ( pCtrl->m_lRangeTop != lRangeTop )
                  {
                     lRangeTop = pCtrl->m_lRangeTop;
                     lRangeBottom = pCtrl->m_lRangeBottom;
                     if ( bRed )
                     {
                        bRed = FALSE;
                        dc.SelectObject( &penBlue );
                        dc.MoveTo( rect.right - 4, lRangeTop + lTop );
                        dc.LineTo( rect.right - 4, lRangeBottom + lTop );
                     }
                     else
                     {
                        bRed = TRUE;
                        dc.SelectObject( &penRed );
                        dc.MoveTo( rect.right - 2, lRangeTop + lTop );
                        dc.LineTo( rect.right - 2, lRangeBottom + lTop );
                     }
                  }
               }
            }
         }
      }

      if ( pPenOld )
         dc.SelectObject( pPenOld );
   }
}

void
TZGroupSetRpt::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::CreateZ: ", m_csTag );
#endif

// WS_EX_CLIENTEDGE - Specifies that a window has a 3D look
// WS_EX_STATICEDGE - Creates a window with a three-dimensional border
// WS_EX_WINDOWEDGE - Specifies that a window has a raised border
   CreateEx( WS_EX_CLIENTEDGE, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

// dtor
TZGroupSetRpt::~TZGroupSetRpt( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupSetRpt::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}


/////////////////////////////////////////////////////////////////////////////
// TZGroupRpt, class implementation
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( TZGroupRpt, CStatic )
BEGIN_MESSAGE_MAP( TZGroupRpt, CStatic )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

// ctor
TZGroupRpt::TZGroupRpt( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CStatic( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID, 'G' )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupRpt::ctor: ", m_csTag );
#endif

   zCHAR szType[ 32 ];

   if ( vCtrl )
   {
      GetIntegerFromAttribute( &m_lGrpFlags, vCtrl, szlGroup, "Flags" );
      GetStringFromAttribute( szType, sizeof( szType ), vCtrl, szlGroup, szlType );
      m_chType = tolower( szType[ 1 ] );
   }
   else
   {
      m_lGrpFlags = 0;
      m_chType = 'a';   // detail group
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;
      if ( pCtrlParent &&
           (pCtrlParent->m_chType == 'H' ||
            pCtrlParent->m_chType == 'F') )
      {
         if ( pCtrlParent->m_nChildren > 1 )
            m_lGrpFlags = zGROUP_OVERLAY;
      }
   }

// Attr.Style |= WS_BORDER | SS_GRAYFRAME;
   CreateZ( );
}

void
TZGroupRpt::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupRpt::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_STATICEDGE, "STATIC", m_csTag,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZGroupRpt::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupRpt::OnCreate: ", m_csTag );
#endif

   if ( (int) Default( ) == -1 )
      return( -1 );

   zCHAR  szTag[ zTAG_LTH ];
   CClientDC dc( this );

   MakeShortString( &dc, szTag, sizeof( szTag ), m_csTag, m_pPainterWindow->m_lLeftMargin - 8, 0 );
   SetWindowText( szTag );

// ModifyStyleEx( 0, WS_EX_TRANSPARENT );
   return( 0 );
}

void
TZGroupRpt::SetupCtrlCover( )   // first chance the control has to know
{                               // its corresponding pCtrlCover
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupRpt::SetupCtrlCover: ", m_csTag );
#endif

   if ( m_pCtrlParent == 0 )
      return;

   if ( m_chType == 'h' )
   {
      m_csLabel = "Group Header";
   }
   else
   if ( m_chType == 'f' )
   {
      m_csLabel = "Group Footer";
   }
   else
// if ( m_chType == 'a' )
   {
      m_chType = 'a';

      // If the parent GroupSet is a page header or a page footer, set
      // the label accordingly.
      if ( m_pCtrlParent->m_chType == 'H' )
         m_csLabel = "Header Detail";
      else
      if ( m_pCtrlParent->m_chType == 'F' )
         m_csLabel = "Footer Detail";
      else
      if ( m_vCtrl )  // if there is a Zeidon instance, get its Entity name
      {
         zPCHAR pch;

         GetAddrForAttribute( &pch, m_vCtrl, szlGroup, szlTag );
         if ( *pch )
            m_csLabel = pch;
         else
            m_csLabel = "Group Detail (Unknown)";
      }
      else
         m_csLabel = "Group Detail";
   }

// SetWindowText( m_csTag );
   m_pPainterWindow->m_pPainterClient->AdjustGroupSets( );
}

TZPainterCtrl *
TZGroupRpt::IsDeletable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupRpt::IsDeletable: ", m_csTag );
#endif

   if ( TZPainterCtrl::IsDeletable( ) )
   {
   // TMV-make GroupSet deletable even if it is a header or footer.
   // if ( (m_lFlags & zGROUP_OVERLAY) == 0 && m_pCtrlParent &&
   //      (m_pCtrlParent->m_chType == 'H' ||
   //       m_pCtrlParent->m_chType == 'F') )
   // {
   //    return( 0 );   // cannot be deleted if parent is a Page Header
   //                   // or Page Footer unless it is an overlay group
   // }
   // TMV-make GroupSet deletable even if it is a header or footer.

      return( this );
   }

   return( 0 );
}

void
TZGroupRpt::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   if ( m_csTag == "PageHeaderOverlay" )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "TZGroupRpt::OnPaint: %s ====>", m_csTag );
      TraceRect( szMsg, m_rectPainter );
   }
#endif

   CStatic::OnPaint( );

   if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
   {
      CClientDC dc( this );
      CRect  rect;

      GetClientRect( rect );
      rect.right = m_pPainterWindow->m_lLeftMargin - 8 - 1;

      COLORREF colorShadow( ::GetSysColor( COLOR_BTNSHADOW ) );
//    CPen penGray( PS_DASH, 1, colorShadow );
//    CPen penBlack( PS_DASH, 1, RGB( 0, 0, 0 ) );
      CPen penRed( PS_INSIDEFRAME, 3, zRGB_RED );
      CPen penBlue( PS_INSIDEFRAME, 3, zRGB_BLUE );
      CPen penGray( PS_INSIDEFRAME, 3, colorShadow );
      CPen penBlack( PS_INSIDEFRAME, 3, RGB( 0, 0, 0 ) );
      CPen *pPenOld = dc.SelectObject( &penBlack );
      dc.MoveTo( rect.right - 3, rect.top );
      dc.LineTo( rect.right - 3, rect.bottom );
      dc.SelectObject( &penGray );
      dc.MoveTo( rect.right - 2, rect.top );
      dc.LineTo( rect.right - 2, rect.bottom );

      TZPainterCtrl *pGroup;
      TZPainterCtrl *pCtrl;
      zLONG  lRangeTop = -2;
      zLONG  lRangeBottom;
      zLONG  lTop = rect.top;
      zBOOL  bRed = FALSE;
      zSHORT nPos;

      // Mark the range for ctrls within this group.
      for ( nPos = 0;
            nPos < m_pPainterWindow->m_nCtrlCnt;
            nPos++ )
      {
         pGroup = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pGroup != this )   // wrong Group
            continue;

         for ( nPos = nPos + 1; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
         {
            pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            pGroup = pCtrl;
            while ( pGroup->m_pCtrlParent &&
                    pGroup->m_pCtrlParent->m_pCtrlParent )
            {
               pGroup = pGroup->m_pCtrlParent;
            }

            if ( pGroup == this )
            {
               lTop = pCtrl->m_rectCtrl.top;
               if ( pCtrl->m_lRangeTop != lRangeTop )
               {
                  lRangeTop = pCtrl->m_lRangeTop;
                  lRangeBottom = pCtrl->m_lRangeBottom;
                  if ( bRed )
                  {
                     bRed = FALSE;
                     dc.SelectObject( &penBlue );
                     dc.MoveTo( rect.right - 4, lRangeTop );
                     dc.LineTo( rect.right - 4, lRangeBottom );
                  }
                  else
                  {
                     bRed = TRUE;
                     dc.SelectObject( &penRed );
                     dc.MoveTo( rect.right - 2, lRangeTop );
                     dc.LineTo( rect.right - 2, lRangeBottom );
                  }
               }
            }
            else
            {
               nPos = m_pPainterWindow->m_nCtrlCnt;
            }
         }
      }

      if ( pPenOld )
         dc.SelectObject( pPenOld );
   }
}

TZGroupRpt::~TZGroupRpt( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZGroupRpt::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

#if 0
/////////////////////////////////////////////////////////////////////////////
// TZTextRpt, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZTextRpt::TZTextRpt( TZPainterWindow *pPainterWindow,
                      TZPainterCtrl   *pCtrlParent,
                      CWnd    *pWndParent,
                      CRect&  rect,
                      zVIEW   vCtrl,
                      zLONG   lType,
                      zBOOL   bPlaceHolder,
                      zLONG   lZKey,
                      zLONG   lTabStopNbr,
                      zLONG   lCtrlID,
                      zPCHAR  pchTag,
                      zPCHAR  pchText ) :
           CStatic( ),
           TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                          rect, vCtrl, this, pchTag, pchText,
                          lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTextRpt::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;

   Attr.Style |= WS_BORDER; // | SS_GRAYFRAME;
   CreateZ( );
}

void
TZTextRpt::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTextRpt::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

// dtor
TZTextRpt::~TZTextRpt( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTextRpt::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZMLE_Rpt, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZMLE_Rpt::TZMLE_Rpt( TZPainterWindow *pPainterWindow,
                      TZPainterCtrl   *pCtrlParent,
                      CWnd    *pWndParent,
                      CRect&  rect,
                      zVIEW   vCtrl,
                      zLONG   lType,
                      zBOOL   bPlaceHolder,
                      zLONG   lZKey,
                      zLONG   lTabStopNbr,
                      zLONG   lCtrlID,
                      zPCHAR  pchTag,
                      zPCHAR  pchText ) :
           CStatic( ),
           TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                          rect, vCtrl, this, pchTag, pchText,
                          lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLE_Rpt::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;

   Attr.Style |= WS_BORDER; // | SS_GRAYFRAME;
   CreateZ( );
}

void
TZMLE_Rpt::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLE_Rpt::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

// dtor
TZMLE_Rpt::~TZMLE_Rpt( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZMLE_Rpt::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// TZBMP_Rpt, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor
TZBMP_Rpt::TZBMP_Rpt( TZPainterWindow *pPainterWindow,
                      TZPainterCtrl   *pCtrlParent,
                      CWnd    *pWndParent,
                      CRect&  rect,
                      zVIEW   vCtrl,
                      zLONG   lType,
                      zBOOL   bPlaceHolder,
                      zLONG   lZKey,
                      zLONG   lTabStopNbr,
                      zLONG   lCtrlID,
                      zPCHAR  pchTag,
                      zPCHAR  pchText ) :
           CStatic( ),
           TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                          rect, vCtrl, this, pchTag, pchText,
                          lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBMP_Rpt::ctor: ", m_csTag );
#endif

   if ( vCtrl == 0 )
      m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;

   Attr.Style |= WS_BORDER; // | SS_GRAYFRAME;
   CreateZ( );
}

void
TZBMP_Rpt::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBMP_Rpt::CreateZ: ", m_csTag );
#endif

   CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "STATIC", m_csText,
             Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

// dtor
TZBMP_Rpt::~TZBMP_Rpt( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZBMP_Rpt::dtor: ", m_csTag );
#endif

   DestroyWindow( );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// TZShape, class implementation
/////////////////////////////////////////////////////////////////////////////
// ctor

// TZShape - ctor
TZShape::TZShape( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd    *pWndParent,
                  CRect&  rect,
                  zVIEW   vCtrl,
                  zLONG   lType,
                  zBOOL   bPlaceHolder,
                  zLONG   lZKey,
                  zLONG   lTabStopNbr,
                  zLONG   lCtrlID,
                  zPCHAR  pchTag,
                  zPCHAR  pchText ) :
         CWnd( ),
         TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                        rect, vCtrl, this, pchTag, pchText,
                        lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZShape::ctor: ", m_csTag );
#endif

   m_pchCtrlBOI = 0;
   m_bRectangle = TRUE;
   m_bRoundRect = FALSE;
   m_bEllipse = FALSE;
   m_bDiamond = FALSE;
   m_bHorizontalLine = FALSE;
   m_bVerticalLine = FALSE;
   m_bDiagonalDescend = FALSE;
   m_bDiagonalAscend = FALSE;
   m_lPenWidth = 1;
   m_lPenType = 0;
   m_lPenColor = 0;
   m_lRoundValue = 0;

   if ( vCtrl )
   {
      zULONG  ulLth;

      GetAttributeLength( &ulLth, vCtrl, szlControl, szlCtrlBOI );
      if ( ulLth >= 8 + (4 * sizeof( zLONG )) )
      {
         zPCHAR    pch;

         GetAddrForAttribute( &pch, vCtrl, szlControl, szlCtrlBOI );
         m_pchCtrlBOI = new char[ ulLth ];
         zmemcpy( m_pchCtrlBOI, pch, ulLth );
         m_bRectangle = m_pchCtrlBOI[ 0 ];
         m_bRoundRect = m_pchCtrlBOI[ 1 ];
         m_bEllipse = m_pchCtrlBOI[ 2 ];
         m_bDiamond = m_pchCtrlBOI[ 3 ];
         m_bHorizontalLine = m_pchCtrlBOI[ 4 ];
         m_bVerticalLine = m_pchCtrlBOI[ 5 ];
         m_bDiagonalDescend = m_pchCtrlBOI[ 6 ];
         m_bDiagonalAscend = m_pchCtrlBOI[ 7 ];
         m_lPenWidth = *((zPLONG) (m_pchCtrlBOI + 8));
         m_lPenType = *((zPLONG) (m_pchCtrlBOI + 8 + sizeof( zLONG )));
         m_lPenColor = *((zPLONG) (m_pchCtrlBOI + 8 + (2 * sizeof( zLONG ))));
         m_lRoundValue = *((zPLONG) (m_pchCtrlBOI + 8 + (3 * sizeof( zLONG ))));
      }
   }

   CreateZ( );
}

void
TZShape::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZShape::CreateZ: ", m_csTag );
#endif

   CreateEx( WS_EX_TRANSPARENT /* | WS_EX_STATICEDGE */, "ZeidonCustomCtrl",
             m_csText, Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

void
TZShape::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZShape::OnPaint: %s ====>", m_csTag );
   TraceRect( szMsg, m_rectPainter );
#endif

   CRect rectUpdate;
   BOOL bUpdate = GetUpdateRect( rectUpdate, 1 );

   CRect rect;
   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
   CPen *pPen = 0;
   CPen *pPenOld = 0;
   COLORREF colorBk = ::GetSysColor( COLOR_BTNFACE );
   COLORREF colorOldBk = dc.SetBkColor( colorBk );
   int nOldMode = dc.SetBkMode( TRANSPARENT );

   CBrush brush( colorBk );
   CBrush *pBrushOld = dc.SelectObject( &brush );
   GetClientRect( rect );
   dc.FillRect( rect, &brush );

   if ( m_lPenWidth || m_lPenColor || m_lPenType )
   {
      int nPenType;

      if ( m_lPenWidth == 0 )
         m_lPenWidth = 1;

      switch ( m_lPenType )
      {
         case 1:
           nPenType = PS_DASH;
           break;

         case 2:
           nPenType = PS_DOT;
           break;

         case 3:
           nPenType = PS_DASHDOT;
           break;

         case 4:
           nPenType = PS_DASHDOTDOT;
           break;

         default:
           nPenType = PS_SOLID;
           break;
      }

      pPen = new CPen( nPenType, (m_lPenWidth / 2) + 1, m_lPenColor );
      pPenOld = dc.SelectObject( pPen );
   }

   if ( m_bRectangle )
      dc.Rectangle( rect );

   if ( m_bRoundRect )
   {
      if ( m_lRoundValue <= 0 )
         m_lRoundValue = 16;

      dc.RoundRect( rect, CPoint( m_lRoundValue, m_lRoundValue ) );
   }

   if ( m_bEllipse )
      dc.Ellipse( rect );

   if ( m_bDiamond )
   {
      dc.MoveTo( (rect.left + rect.right) / 2, rect.top );
      dc.LineTo( rect.right, (rect.top + rect.bottom) / 2 );
      dc.LineTo( (rect.left + rect.right) / 2, rect.bottom );
      dc.LineTo( rect.left, (rect.top + rect.bottom) / 2 );
      dc.LineTo( (rect.left + rect.right) / 2, rect.top );
   }

   if ( m_bHorizontalLine )
   {
      dc.MoveTo( rect.left, (rect.top + rect.bottom) / 2 );
      dc.LineTo( rect.right, (rect.top + rect.bottom) / 2 );
   }

   if ( m_bVerticalLine )
   {
      dc.MoveTo( (rect.left + rect.right) / 2, rect.top );
      dc.LineTo( (rect.left + rect.right) / 2, rect.bottom );
   }

   if ( m_bDiagonalDescend )
   {
      dc.MoveTo( rect.left, rect.top );
      dc.LineTo( rect.right, rect.bottom );
   }

   if ( m_bDiagonalAscend )
   {
      dc.MoveTo( rect.left, rect.bottom );
      dc.LineTo( rect.right, rect.top );
   }

   dc.SelectObject( pBrushOld );
   dc.SetBkColor( colorOldBk );
   dc.SetBkMode( nOldMode );

   if ( pPenOld )
   {
      dc.SelectObject( pPenOld );
      delete( pPen );
   }

   if ( bUpdate )
   {
      ClientToScreen( rectUpdate );
   // TraceRect( "ZGroupBox::OnPaint ClientToScreenRect", rect );

      CRect   rectCtrl;
      TZPainterCtrl *pCtrl;
      zSHORT  nPos;

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent == this )
         {
            if ( pCtrl->m_pWndCtrl &&
                 mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
            {
               pCtrl->m_pWndCtrl->GetWindowRect( rectCtrl );
            // TraceRect( pCtrl->m_csText, rectCtrl );
               if ( rectCtrl.IntersectRect( rectCtrl, rectUpdate ) )
                  pCtrl->m_pWndCtrl->Invalidate( );
            }
         }
      }
   }
}

// TZShape - dtor
TZShape::~TZShape( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZShape::dtor: ", m_csTag );
#endif

   mDeleteInitA( m_pchCtrlBOI );
   DestroyWindow( );
}
