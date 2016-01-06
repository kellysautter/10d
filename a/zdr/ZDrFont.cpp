/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrfont.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of ZFont class (simple class to
// permit reuse of fonts by an application).

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2000.08.07  RG  Z2000
//   Changed declaration of fontsize from zSHORT to zLONG to remove
//   compiler warnings in Zeidon Tools
//
// 1999.03.10  DKS
//    Altered the TranslateColor function to remove name conflict.
//
// 1999.02.02    DKS
//    Added default font support for reports.
//
// 1999.01.11    DKS
//    Added support for Escapement to font creation.
//
// 1998.12.04    DKS
//    The "FontScale" may now reside in a user.ini file (redirected from
//    Zeidon.ini via the "LocalDir" value.
//
// 1998.09.02    DKS
//    Permit the application to scale the font for printing using the
//    "[Workstation]" "FontScale" value.
//
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
//
// Simple font class to permit an application to reuse fonts while printing
// a document.
//
//
// typedef struct tagLOGFONT {     /* lf */
//    int   lfHeight;
//    int   lfWidth;
//    int   lfEscapement;
//    int   lfOrientation;
//    int   lfWeight;
//    BYTE  lfItalic;
//    BYTE  lfUnderline;
//    BYTE  lfStrikeOut;
//    BYTE  lfCharSet;
//    BYTE  lfOutPrecision;
//    BYTE  lfClipPrecision;
//    BYTE  lfQuality;
//    BYTE  lfPitchAndFamily;
//    BYTE  lfFaceName[LF_FACESIZE];
// } LOGFONT;
//
// The LOGFONT structure defines the attributes of a font, a drawing
// object used to write text on a display surface.
//
// Member  Description
//
// lfHeight - Specifies the desired height, in logical units, for the
// font. If this value is greater than zero, it specifies the cell
// height of the font. If it is less than zero, it specifies the
// character height of the font. (Character height is the cell height
// minus the internal leading. Applications that specify font height in
// points typically use a negative number for this member.) If this
// value is zero, the font mapper uses a default height. The font
// mapper chooses the largest physical font that does not exceed the
// requested size (or the smallest font, if all the fonts exceed the
// requested size). The absolute value of the lfHeight member must not
// exceed 16,384 after it is converted to device units.
//
// lfWidth - Specifies the average width, in logical units, of
// characters in the font. If this value is zero, the font mapper
// chooses a reasonable default width for the specified font height.
// (The default width is chosen by matching the aspect ratio of the
// device against the digitization aspect ratio of the available fonts.
// The closest match is determined by the absolute value of the
// difference.) The widths of characters in TrueType fonts are scaled
// by a factor of this member divided by the width of the characters in
// the physical font (as specified by the tmAveCharWidth member of the
// TEXTMETRIC structure).
//
// lfEscapement - Specifies the angle, in tenths of degrees, between
// the base line of a character and the x-axis. The angle is measured
// in a counterclockwise direction from the x-axis for left-handed
// coordinate systems (that is, MM_TEXT, in which the y direction is
// down) and in a clockwise direction from the x-axis for right-handed
// coordinate systems (in which the y direction is up).
//
// lfOrientation - Specifies the orientation of the characters. This
// value is ignored.
//
// lfWeight - Specifies the font weight. This member can be one of the
// following values:
//
// Constant        Value
// =============== =====
// FW_DONTCARE     0
// FW_THIN 100
// FW_EXTRALIGHT   200
// FW_ULTRALIGHT   200
// FW_LIGHT        300
// FW_NORMAL       400
// FW_REGULAR      400
// FW_MEDIUM       500
// FW_SEMIBOLD     600
// FW_DEMIBOLD     600
// FW_BOLD 700
// FW_EXTRABOLD    800
// FW_ULTRABOLD    800
// FW_BLACK        900
// FW_HEAVY        900
//
// The actual appearance of the font depends on the type face. Some
// fonts have only FW_NORMAL, FW_REGULAR, and FW_BOLD weights. If
// FW_DONTCARE is specified, a default weight is used.
//
// lfItalic        Specifies an italic font if nonzero.
// lfUnderline     Specifies an underlined font if nonzero.
// lfStrikeOut     Specifies a strikeout font if nonzero.
// lfCharSet       Specifies the character set of the font. The
// following values are predefined:
//
// Constant         Value
// ================ =====
// ANSI_CHARSET         0
// DEFAULT_CHARSET      1
// SYMBOL_CHARSET       2
// SHIFTJIS_CHARSET   128
// OEM_CHARSET        255
//
// The DEFAULT_CHARSET value is not used by the font mapper. An
// application can use this value to allow the name and size of a font
// to fully describe the logical font. If the specified font name does
// not exist, a font from any character set can be substituted for the
// specified font; applications should use the DEFAULT_CHARSET value
// sparingly to avoid unexpected results.
//
// The OEM character set is system-dependent.  Fonts with other
// character sets may exist in the system. If an application uses a
// font with an unknown character set, it should not attempt to
// translate or interpret strings that are to be rendered with that
// font.
//
// lfOutPrecision - Specifies the desired output precision. The output
// precision defines how closely the output must match the height,
// width, character orientation, escapement, and pitch of the requested
// font. This member can be one of the following values:
//
// OUT_CHARACTER_PRECIS    OUT_STRING_PRECIS
// OUT_DEFAULT_PRECIS      OUT_STROKE_PRECIS
// OUT_DEVICE_PRECIS       OUT_TT_PRECIS
// OUT_RASTER_PRECIS       OUT_TT_ONLY_PRECIS
//
// Applications can use the values OUT_DEVICE_PRECIS,
// OUT_RASTER_PRECIS, and OUT_TT_PRECIS to control how the font mapper
// chooses a font when the system contains more than one font with a
// given name. For example, if a system contains a font named "Symbol"
// in raster and TrueType form, specifying OUT_TT_PRECIS would force
// the font mapper to choose the TrueType version. (Specifying
// OUT_TT_PRECIS forces the font mapper to choose a TrueType font
// whenever the specified font name matches a device or raster font,
// even when there is no TrueType font with the same name.) An
// application can use TrueType fonts exclusively by specifying
// OUT_TT_ONLY_PRECIS.  When this value is specified, the system
// chooses a TrueType font even when the name specified in the
// lfFaceName member matches a raster or vector font.
//
// lfClipPrecision Specifies the desired clipping precision. The
// clipping precision defines how to clip characters that are partially
// outside the clipping region. This member can be any one of the
// following values:
//
// CLIP_CHARACTER_PRECIS   CLIP_MASK
// CLIP_DEFAULT_PRECIS     CLIP_STROKE_PRECIS
// CLIP_EMBEDDED           CLIP_TT_ALWAYS
// CLIP_LH_ANGLES
//
// To use an embedded read-only font, applications must specify the
// CLIP_EMBEDDED value.
//
// To achieve consistent rotation of device, TrueType, and vector
// fonts, an application can use the OR operator to combine the
// CLIP_LH_ANGLES value with any of the other lfClipPrecision values.
// If the CLIP_LH_ANGLES bit is set, the rotation for all fonts is
// dependent on whether the orientation of the coordinate system is
// left-handed or right-handed. If CLIP_LH_ANGLES is not set, device
// fonts always rotate counter-clockwise, but the rotation of other
// fonts is dependent on the orientation of the coordinate system. (For
// more information about the orientation of coordinate systems, see
// the description of the lfEscapement member.)
//
// lfQuality - Specifies the output quality of the font, which defines
// how carefully the graphics device interface (GDI) must attempt to
// match the logical-font attributes to those of an actual physical
// font. This member can be one of the following values:
//
// Value            Meaning
// ================ ======================================================
// DEFAULT_QUALITY  Appearance of the font does not matter.
// DRAFT_QUALITY    Appearance of the font is less important than when the
//                  PROOF_QUALITY  value is used. For GDI raster fonts,
//                  scaling is enabled. Bold, italic, underline, and
//                  strikeout fonts are synthesized if necessary.
// PROOF_QUALITY    Character quality of the font is more important than
//                  exact
// matching of the logical-font attributes. For GDI raster fonts,
// scaling is disabled and the font closest in size is chosen.
// Bold, italic, underline, and strikeout fonts are synthesized if
// necessary.
//
// lfPitchAndFamily - Specifies the pitch and family of the font. The two
// low-order bits, which specify the pitch of the font, can be one of
// the following values:
//
// DEFAULT_PITCH   VARIABLE_PITCH   FIXED_PITCH
//
// The four high-order bits of the member, which specify the font
// family, can be one of the following values:
//
// Value           Meaning
// ==============  ========================================================
// FF_DECORATIVE   Novelty fonts. Old English is an example.
// FF_DONTCARE     Don't care or don't know.
// FF_MODERN       Fonts with constant stroke width, with or without
//                 serifs.  Pica, Elite, and Courier New are examples.
// FF_ROMAN        Fonts with variable stroke width and with serifs.
//                 Times New Roman and New Century Schoolbook are examples.
// FF_SCRIPT       Fonts designed to look like handwriting.  Script and
//                 Cursive are examples.
// FF_SWISS        Fonts with variable stroke width and without serifs.
//                 MS Sans Serif is an example.
//
// An application can specify a value for the lfPitchAndFamily member
// by using the Boolean OR operator to join a pitch constant with a
// family constant.  Font families describe the look of a font in a
// general way. They are intended for specifying fonts when the exact
// typeface desired is not available.
//
// lfFaceName - Specifies the typeface name of the font. The length of
// this string must not exceed LF_FACESIZE - 1. The EnumFontFamilies
// function can be used to enumerate the typeface names of all
// currently available fonts. If lfFaceName is 0, GDI uses a
// device-dependent typeface.
//
// Comments
//
// Applications can use the default settings for most of these members
// when creating a logical font.  The members that should always be
// given specific values are lfHeight and lfFaceName. If lfHeight and
// lfFaceName are not set by the application, the logical font that is
// created is device-dependent.
//
//  CFont fontVertical( h, 0, 2700, 2700, FW_NORMAL, 0, 0, 0,
//                      ANSI_CHARSET, OUT_TT_ONLY_PRECIS,
//                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
//                      VARIABLE_PITCH | FF_SWISS, 0);
//
//  BOOL CreateFont( int nHeight, int nWidth,
//                   int nEscapement, int nOrientation,
//                   int nWeight, BYTE bItalic,
//                   BYTE bUnderline, BYTE cStrikeOut,
//                   BYTE nCharSet, BYTE nOutPrecision,
//                   BYTE nClipPrecision, BYTE nQuality,
//                   BYTE nPitchAndFamily,
//                   LPCTSTR lpszFacename );
//
// CFont( int height, int width, int escapement = 0, int orientation = 0,
//        int weight = FW_NORMAL, UINT8 italic = false,
//        BYTE underline = false, UINT8 strikeout = false,
//        UINT8 charSet = 1, UINT8 outputPrecision = OUT_TT_ONLY_PRECIS,
//        UINT8 clipPrecision = CLIP_DEFAULT_PRECIS,
//        UINT8 quality = DEFAULT_QUALITY,
//        UINT8 pitchAndFamily = DEFAULT_PITCH|FF_DONTCARE,
//        const char far *facename = 0 );
//
// BOOL CreateFont( int nHeight, int nWidth, int nEscapement,
//                  int nOrientation, int nWeight, BYTE bItalic,
//                  BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet,
//                  BYTE nOutPrecision, BYTE nClipPrecision,
//                  BYTE nQuality, BYTE nPitchAndFamily,
//                  LPCTSTR lpszFacename );
//
/////////////////////////////////////////////////////////////////////////////
ZFont::ZFont( zVIEW vSubtask, zCPCHAR cpcTag, zLONG nPtSize, zSHORT nWeight,
              zSHORT bItalic, zSHORT bUnderscore, zSHORT bStrikeout,
              zLONG lTextColor, zLONG lTextBkColor,
              zSHORT nEscapement, zCPCHAR cpcFaceName )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZFont ctor: ", cpcTag );
#endif

   m_bSet = FALSE;
   m_nIdNbr = 0;
   zmemset( &m_Panose, 0, sizeof( PANOSE ) );
   m_byCharset = ANSI_CHARSET;
   m_byPitchFamily = 0;

   m_TagList.AddTail( cpcTag );
   m_clrText = fnTranslateColor( lTextColor );
   if ( lTextBkColor == CLR_INVALID || lTextBkColor == 0xff000000 )
      m_clrTextBk = CLR_INVALID;
   else
      m_clrTextBk = fnTranslateColor( lTextBkColor );

   m_nPointSize = nPtSize;
   m_nWeight = nWeight;
   if ( bItalic )
      m_nFlags = 1;
   else
      m_nFlags = 0;

   if ( bUnderscore )
      m_nFlags |= 2;

   if ( bStrikeout )
      m_nFlags |= 16;

   if ( m_nWeight >= FW_BOLD )
      m_nFlags |= 32;

   m_nEscapement = nEscapement;
   if ( cpcFaceName == 0 && *cpcFaceName == 0 )
   {
      ZSubtask *pZSubtask;

      if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
           pZSubtask->m_bUseMapDlgUnits )
      {
         m_csFaceName = "MS Sans Serif";
      }
      else
         m_csFaceName = "Arial";
   }
   else
      m_csFaceName = cpcFaceName;

   zmemset( &m_Panose, 0, sizeof( PANOSE ) );
   CreateFont( nPtSize, 0, nEscapement, nEscapement, nWeight,
               (BYTE) bItalic, (BYTE) bUnderscore, (BYTE) bStrikeout,
               ANSI_CHARSET, OUT_TT_ONLY_PRECIS,
               CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
               (BYTE) (nEscapement ? VARIABLE_PITCH | FF_SWISS :
                                     DEFAULT_PITCH | FF_DONTCARE),
               m_csFaceName );
}

ZFont::ZFont( zVIEW vSubtask, zCPCHAR cpcTag, zCPCHAR cpcFaceName,
              int nPtSize, CDC *pDC, zLONG lTextColor )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZFont ctor2: ", cpcTag );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout &&
        pZSubtask->m_pZPrintout->m_dHeight >= 0.1 &&
        pZSubtask->m_pZPrintout->m_dHeight <= -0.1 )
   {
      // Scale the font as specified in Zeidon.ini
      nPtSize = (int) ((double) nPtSize * pZSubtask->m_pZPrintout->m_dHeight);
   }

   CreatePointFont( nPtSize, cpcFaceName, pDC );
   m_TagList.AddTail( cpcTag );
   m_clrText = fnTranslateColor( lTextColor );
}

ZFont::~ZFont( )
{
#ifdef DEBUG_ALL
   if ( m_TagList.m_pHeadName )
      TraceLineS( "ZFont dtor: ", m_TagList.m_pHeadName->m_csName );
   else
      TraceLineS( "ZFont dtor: ", "(Null)" );
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ZFontUI handles the user interface for changing font sizes, as well
// as saving/restoring font info in the application profile.
//
ZFontUI::ZFontUI( )
{
   m_nFontPtSizeMin = 4;
   m_nFontPtSizeMax = 120;
}

ZFontUI::~ZFontUI( )
{
}

/////////////////////////////////////////////////////////////////////////////
// Get font point size. Convert device units to points.
// There are 72 points per inch.
//
int
ZFontUI::GetFontPointSize( CFont& font, CDC& dc )
{
   LOGFONT lf;
   font.GetLogFont( &lf );
   return( MulDiv( -lf.lfHeight, 72, dc.GetDeviceCaps( LOGPIXELSY ) ) );
}

/////////////////////////////////////////////////////////////////////////////
// Set font point size. Convert points to device units
// There are 72 points per inch.
//
zBOOL
ZFontUI::SetFontPointSize( CFont& font, CDC& dc, int pts )
{
   LOGFONT lf;
   font.GetLogFont( &lf );
   lf.lfHeight = MulDiv( -pts, dc.GetDeviceCaps( LOGPIXELSY ), 72 );
   font.DeleteObject( );
   return( font.CreateFontIndirect( &lf ) );
}

/////////////////////////////////////////////////////////////////////////////
// Main UI function.
//    op = 0  ==> run common font dialog
//    op < 0  ==> font size smaller
//    op < 0  ==> font size bigger
//
// Returns zBOOL, whether changed or not, and CFont has new font.
//
zBOOL
ZFontUI::OnChangeFont( CFont& font, int op, CWnd *pWnd, DWORD dwFlags )
{
   ASSERT( font.m_hObject );
   if ( op == 0 )
   {
      // Run common font dialog
      LOGFONT logfont;
      font.GetLogFont( &logfont );
      CFontDialog dlg( &logfont, dwFlags, 0, pWnd );
      dlg.m_cf.nSizeMin = m_nFontPtSizeMin;
      dlg.m_cf.nSizeMax = m_nFontPtSizeMax;
      if ( dlg.DoModal( ) != IDOK )
         return( FALSE );

      // Change the font
      font.DeleteObject( );
      return( font.CreateFontIndirect( &logfont ) );
   }

   // Grow or shrink
   CWindowDC dc( 0 );                   // use screen DC
   int pts = GetFontPointSize( font, dc ); // get point size
   pts = GrowFontSize( pts, op );          // grow ( or shrink )
   if ( pts < m_nFontPtSizeMin || pts > m_nFontPtSizeMax )
      return( FALSE );

   return( SetFontPointSize( font, dc, pts ) );
}

/////////////////////////////////////////////////////////////////////////////
// Increment or decrement font point size based on current size.
// Algorithm:
// If point size is
//       <= 12    incr = 1 pts
//       10..32   incr = 2 pts
//       32..48   incr = 4 pts
//       >= 48    incr = 8 pts
// Derived classes can override this virtual function to change this.
// Return zero to disallow changing size.
//
int
ZFontUI::GrowFontSize( int ptSize, int dir )
{
   int incr = ptSize <= 12 ? 1 : ptSize < 32 ? 2 : ptSize < 48 ? 4 : 8;
   ptSize += dir>0 ? incr : -incr;;
   return( ptSize );
}

/////////////////////////////////////////////////////////////////////////////
// Create font from info in the application profile.  Reads info in the form
// facename, ptsize, weight, italic
//
zBOOL
ZFontUI::GetProfileFont( zCPCHAR cpcKey,
                         zCPCHAR cpcVal,
                         CFont&  font,
                         CDC     *pDC )
{
   CWinApp *pApp = AfxGetApp( );
   ASSERT_VALID( pApp );
   CString zs = pApp->GetProfileString( cpcKey, cpcVal );
   if ( zs.IsEmpty( ) )
      return( FALSE );

   LOGFONT lf;
   zmemset( &lf, 0, sizeof( LOGFONT ) );
   lf.lfCharSet = DEFAULT_CHARSET;
   int bItalic;
   int nPtSize;

   // scanf is overkill, but I'm lazy
   if ( sscanf( (zCPCHAR) zs, "%[a-zA-Z ],%d,%d,%d",
                  lf.lfFaceName, &nPtSize, &lf.lfWeight, &bItalic ) != 4 )
   {
      return( FALSE );
   }

   lf.lfHeight = MulDiv( -nPtSize,  // convert ptsize to logical units
       ::GetDeviceCaps( pDC ? pDC->m_hDC : ::GetDC( 0 ), LOGPIXELSY ), 72 );

   lf.lfItalic = bItalic;     // because lf.lfItalic is a BYTE
   font.DeleteObject( );      // bye
   return( font.CreateFontIndirect( &lf ) );
}

/////////////////////////////////////////////////////////////////////////////
// Write font to app profile in the form "facename,ptsize,weight,italic"
//
zBOOL
ZFontUI::WriteProfileFont( zCPCHAR cpcKey,
                           zCPCHAR cpcVal,
                           CFont&  font,
                           CDC     *pDC )
{
   CWinApp *pApp = AfxGetApp( );
   ASSERT_VALID( pApp );
   LOGFONT lf;
   font.GetLogFont( &lf );
   int nPtSize = MulDiv( -lf.lfHeight, 72,
      ::GetDeviceCaps( pDC ? pDC->m_hDC : ::GetDC( 0 ), LOGPIXELSY ) );
   CString zs;
   zs.Format( "%s,%d,%d,%d", lf.lfFaceName, nPtSize, lf.lfWeight, lf.lfItalic );
   return( pApp->WriteProfileString( cpcKey, cpcVal, zs ) );
}
