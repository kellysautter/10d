/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdib.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Device Independent Bitmap class.
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
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// ZDib - Device Independent Bitmap.
// This implementation draws bitmaps using normal Win32 API functions
// rather than DrawDib.  ZDib is derived from CBitmap, so you can use
// it with any other MFC functions that use bitmaps.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// FindDIBBits( )
//
// Parameter:
//
// zPVOID pbi - pointer to packed-DIB memory block
//
// Return Value:
//
// zPVOID - pointer to the DIB bits
//
// Description:
//
// This function calculates the address of the DIB's bits and returns a
// pointer to the DIB bits.
//
/////////////////////////////////////////////////////////////////////////////
zEXPORT zPVOID OPERATION
FindDIBBits( zPVOID pbi )
{
   return( ((zPCHAR) pbi + *((zPULONG) pbi) + ::PaletteSize( pbi )) );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   DIBWidth( zPVOID pDIB )
//
// Parameter:
//             zPVOID pDIB - pointer to DIB header (either BITMAPINFOHEADER
//                           or BITMAPCOREHEADER)
//
// Return Value:
//
// zULONG - width of the DIB
//
// Purpose:    Given a pointer to a packed-DIB memory block, returns its
//             width.  Note that it returns a DWORD (since a Win30 DIB can
//             have a DWORD in its width field), but under Win30, the
//             high order word isn't used!
//
// This function gets the width of the DIB from the BITMAPINFOHEADER
// width field if it's a Windows 3.0-style DIB or from the BITMAPCOREHEADER
// width field if it's an other-style DIB.
//
/////////////////////////////////////////////////////////////////////////////
zEXPORT zULONG OPERATION
DIBWidth( zPVOID pDib )
{
   LPBITMAPINFOHEADER pbmi;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER pbmc;  // pointer to an other-style DIB

   // Point to the header (whether Win 3.0 and old).
   pbmi = (LPBITMAPINFOHEADER) pDib;
   pbmc = (LPBITMAPCOREHEADER) pDib;

   if ( IS_WIN30_DIB( pDib ) )
   {
      // Return the DIB width if it is a Win 3.0 DIB.
      return( pbmi->biWidth );
   }
   else
   {
      // It is an other-style DIB, so return its width.
      return( (zULONG) pbmc->bcWidth );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   DIBHeight
//
// Parameter:
//             zPVOID pDIB - pointer to packed-DIB memory block (either
//                           BITMAPINFOHEADER or BITMAPCOREHEADER)
//
// Return Value:
//
// zULONG - height of the DIB
//
// Purpose:    Given a pointer to a DIB, returns its height.  Note
//             that it returns a DWORD (since a Win30 DIB can have
//             a DWORD in its height field), but under Win30, the
//             high order word isn't used!
//
// This function gets the height of the DIB from the BITMAPINFOHEADER
// height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
// height field if it is an other-style DIB.
//
/////////////////////////////////////////////////////////////////////////////
zEXPORT zULONG OPERATION
DIBHeight( zPVOID pDib )
{
   LPBITMAPINFOHEADER pbmi;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER pbmc;  // pointer to an other-style DIB

   // Point to the header (whether old or Win 3.0).
   pbmi = (LPBITMAPINFOHEADER) pDib;
   pbmc = (LPBITMAPCOREHEADER) pDib;

   if ( IS_WIN30_DIB( pDib ) )
   {
      // Return the DIB height if it is a Win 3.0 DIB.
      return( pbmi->biHeight );
   }
   else
   {
      // It is an other-style DIB, so return its height.
      return( (zULONG) pbmc->bcHeight );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION :  PaletteSize( zPVOID pbi )
//
// Parameter:
//             zPVOID pbi - pointer to packed-DIB memory block (either
//                          BITMAPINFOHEADER or BITMAPCOREHEADER)
//
// Return Value:
// zUSHORT - Size of the color palette of the DIB in number of bytes.
//
// PURPOSE  :  Calculates the palette size in bytes by multiplying the
// number of colors by the size of an RGBQUAD (for a * Windows 3.0-style
// DIB) or by the size of an RGBTRIPLE (for an other-style DIB).
//
/////////////////////////////////////////////////////////////////////////////
zEXPORT zUSHORT OPERATION
PaletteSize( zPVOID pbi )
{
   // Calculate the size required by the palette.
   if ( IS_WIN30_DIB ( pbi ) )
   {
      return( (::DIBNbrColors( pbi ) * sizeof( RGBQUAD )) );
   }
   else
   {
      return( (::DIBNbrColors( pbi ) * sizeof( RGBTRIPLE )) );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION : DIBNbrColors( zPVOID pbi )
//
// Parameter:
//            zPVOID pbi - pointer to packed-DIB memory block (either
//                         BITMAPINFOHEADER or BITMAPCOREHEADER)
//
// Return Value:
//
// zUSHORT - The number of colors in the DIB's color table
//
// PURPOSE  : Calculates the number of colors in the DIB's color table by
//            finding the bits per pixel for the DIB (whether Win3.0 or
//            other-style DIB).  If bits per pixel is 1: colors = 2,
//            if 4: colors = 16, if 8: colors = 256, if 24, no colors in
//            color table.
//
/////////////////////////////////////////////////////////////////////////////
zEXPORT zUSHORT OPERATION
DIBNbrColors( zPVOID pbi )
{
   zUSHORT uBitCnt;  // DIB bit count

   // If this is a Windows-style DIB, the number of colors in the
   // color table can be less than the number of bits per pixel
   // allows for (i.e.  pbi->biClrUsed can be set to some value).
   // If this is the case, return the appropriate value.
   if ( IS_WIN30_DIB( pbi ) )
   {
      zULONG dwClrUsed;

      dwClrUsed = ((LPBITMAPINFOHEADER) pbi)->biClrUsed;
      if ( dwClrUsed )
         return( (zUSHORT) dwClrUsed );
   }

   //  Calculate the number of colors in the color table based on
   //  the number of bits per pixel for the DIB.
   if ( IS_WIN30_DIB( pbi ) )
      uBitCnt = ((LPBITMAPINFOHEADER) pbi)->biBitCount;
   else
      uBitCnt = ((LPBITMAPCOREHEADER) pbi)->bcBitCount;

   // Return number of colors based on bits per pixel.
   switch ( uBitCnt )
   {
      case 1:
         return( 2 );

      case 4:
         return( 16 );

      case 8:
         return( 256 );

      default:
         return( 0 );   // A 24 bitcount DIB has no color table
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

const int  MAXPALCOLORS = 256;      //???

IMPLEMENT_DYNAMIC( ZDib, CBitmap )

ZDib::ZDib( )
{
   zmemset( &m_bitmap, 0, sizeof( m_bitmap ) );
   m_hdd = 0;
   m_bBitmapLoaded = FALSE;
}

ZDib::~ZDib( )
{
   DeleteObject( );
}

/////////////////////////////////////////////////////////////////////////////
//
// Delete Object. Delete DIB and palette.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZDib::DeleteObject( )
{
   m_bBitmapLoaded = FALSE;
   if ( m_palette.m_hObject )
      m_palette.DeleteObject( );

   if ( m_hdd )
   {
      ::DrawDibClose( m_hdd );
      m_hdd = 0;
   }

   zmemset( &m_bitmap, 0, sizeof( m_bitmap ) );
   return( CBitmap::DeleteObject( ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// Read DIB from file.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZDib::Load( zCPCHAR cpcPathName )
{
   if ( m_hObject )
      DeleteObject( );

   m_bBitmapLoaded =
      Attach( ::LoadImage( 0, cpcPathName, IMAGE_BITMAP, 0, 0,
              LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE ) );
   return( m_bBitmapLoaded );
}

/////////////////////////////////////////////////////////////////////////////
//
// Load bitmap resource.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZDib::Load( HINSTANCE hInst, zCPCHAR cpcResourceName )
{
   if ( m_hObject )
      DeleteObject( );

   m_bBitmapLoaded =
      Attach( ::LoadImage( hInst, cpcResourceName, IMAGE_BITMAP, 0, 0,
                           LR_CREATEDIBSECTION | LR_DEFAULTSIZE ) );
   return( m_bBitmapLoaded );
}

zBOOL
ZDib::Load( HINSTANCE hInst, UINT uResId )
{
   return( Load( hInst, MAKEINTRESOURCE( uResId ) ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// Attach is just like the CGdiObject version, except it also creates the
// palette.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZDib::Attach( HGDIOBJ hbm )
{
   if ( CBitmap::Attach( hbm ) )
   {
      if ( GetBitmap( &m_bitmap ) == 0 )     // load BITMAP for speed
         return( FALSE );

      m_palette.DeleteObject( );             // in case one is already there
      return( CreatePalette( m_palette ) );  // create palette
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
//
// Get size (width, height) of bitmap.
// This static function can be used for CBitmaps as well as ZDibs.
//
/////////////////////////////////////////////////////////////////////////////
CSize
GetBitmapSize( CBitmap *pBitmap )
{
   BITMAP bm;
   return( pBitmap->GetBitmap( &bm ) ?
                          CSize( bm.bmWidth, bm.bmHeight ) : CSize( 0, 0 ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// This static function can be used to draw CBitmaps as well as ZDibs.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
DrawBitmap( CDC& dc, CBitmap *pBitmap,
            const CRect *rectTgt, const CRect *rectSrc )
{
   // Compute rectangles where 0 specified.
   CRect rect;
   if ( rectSrc == 0 )
   {
      // If no source rect, use whole bitmap.
      rect = CRect( CPoint( 0, 0 ), GetBitmapSize( pBitmap ) );
      rectSrc = &rect;
   }

   if ( rectTgt == 0 )
   {
      // If no destination rect, use source.
      rectTgt = rectSrc;
   }

   // Create memory DC.
   CDC memDC;
   memDC.CreateCompatibleDC( &dc );
   CBitmap *pOldBmp = memDC.SelectObject( pBitmap );

   // Blast bits from memory DC to target DC.  Use StretchBlt if size
   // is different.
   zBOOL bRC = FALSE;
   if ( rectTgt->Size( ) == rectSrc->Size( ) )
   {
      bRC = dc.BitBlt( rectTgt->left, rectTgt->top,
                       rectTgt->Width( ), rectTgt->Height( ),
                       &memDC, rectSrc->left, rectSrc->top, SRCCOPY );
   }
   else
   {
      dc.SetStretchBltMode( COLORONCOLOR );
      bRC = dc.StretchBlt( rectTgt->left, rectTgt->top,
                           rectTgt->Width( ), rectTgt->Height( ),
                           &memDC,
                           rectSrc->left, rectSrc->top,
                           rectSrc->Width( ), rectSrc->Height( ), SRCCOPY );
   }

   memDC.SelectObject( pOldBmp );
   return( bRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// Draw DIB on caller's DC. Does stretching from source to destination
// rectangles.  Generally, it's OK to let the following default to zero:
//
//    bUseDrawDib = whether to use use DrawDib, default TRUE
//    pPalette    = palette, default = 0, (use DIB's palette)
//    bForeground = realize in foreground (default FALSE)
//
// If handling palette messages, use bForeground = FALSE, since the
// foreground palette will be realized in WM_QUERYNEWPALETTE.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZDib::Draw( CDC& dc, const CRect *rectTgt, const CRect *rectSrc,
            zBOOL bUseDrawDib, CPalette *pPalette, zBOOL bForeground )
{
   if ( m_hObject == 0 )
      return( FALSE );

   // Select, realize palette.
   if ( pPalette == 0 )              // no palette specified:
      pPalette = GetPalette( );      // use default

   CPalette *pOldPal = dc.SelectPalette( pPalette, !bForeground );
   dc.RealizePalette( );

   zBOOL bRC = FALSE;
   if ( bUseDrawDib )
   {
      // Compute rectangles where 0 specified.
      CRect rect( 0, 0, -1, -1 );  // default for ::DrawDibDraw
      if ( rectSrc == 0 )
         rectSrc = &rect;

      if ( rectTgt == 0 )
         rectTgt = rectSrc;

      if ( m_hdd == 0 )
         VERIFY( (m_hdd = ::DrawDibOpen( )) != 0 );

      // Get BITMAPINFOHEADER/color table.  Copy into stack object each time.
      // This doesn't seem to slow things down visibly.
      DIBSECTION ds;
      VERIFY( GetObject( sizeof( ds ), &ds ) == sizeof( ds ) );
      char buf[ sizeof( BITMAPINFOHEADER ) + MAXPALCOLORS * sizeof( RGBQUAD ) ];
      BITMAPINFOHEADER& bmih = *(BITMAPINFOHEADER *) buf;
      RGBQUAD *colors = (RGBQUAD *) (&bmih + 1);
      zmemcpy( &bmih, &ds.dsBmih, sizeof( bmih ) );
      GetColorTable( colors, MAXPALCOLORS );

      // Let DrawDib do the work!
      bRC = ::DrawDibDraw( m_hdd, dc,
                           rectTgt->left, rectTgt->top,
                           rectTgt->Width( ), rectTgt->Height( ),
                           &bmih,        // ptr to BITMAPINFOHEADER + colors
                           m_bitmap.bmBits, // bits in memory
                           rectSrc->left, rectSrc->top,
                           rectSrc->Width( ), rectSrc->Height( ),
                           bForeground ? 0 : DDF_BACKGROUNDPAL );

   }
   else
   {
      // Use normal draw function.
      bRC = DrawBitmap( dc, this, rectTgt, rectSrc );
   }

   if ( pOldPal )
      dc.SelectPalette( pOldPal, TRUE );

   return( bRC );
}

#define PALVERSION 0x300   // magic number for LOGPALETTE

/////////////////////////////////////////////////////////////////////////////
//
// Create the palette.  Use halftone palette for hi-color bitmaps.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZDib::CreatePalette( CPalette& pal )
{
   // Should not already have palette
   ASSERT( pal.m_hObject == 0 );

   zBOOL bRC = FALSE;
   RGBQUAD *colors = new RGBQUAD[ MAXPALCOLORS ];
   UINT nColors = GetColorTable( colors, MAXPALCOLORS );
   if ( nColors > 0 )
   {
      // Allocate memory for logical palette.
      int nLth = sizeof( LOGPALETTE ) + sizeof( PALETTEENTRY ) * nColors;
      LOGPALETTE *pLogPal = (LOGPALETTE *) new char[ nLth ];
      if ( pLogPal == 0 )
         return( 0 );

      // Set version and number of palette entries.
      pLogPal->palVersion = PALVERSION;
      pLogPal->palNumEntries = nColors;

      // Copy color entries.
      for ( UINT k = 0; k < nColors; k++ )
      {
         pLogPal->palPalEntry[ k ].peRed   = colors[ k ].rgbRed;
         pLogPal->palPalEntry[ k ].peGreen = colors[ k ].rgbGreen;
         pLogPal->palPalEntry[ k ].peBlue  = colors[ k ].rgbBlue;
         pLogPal->palPalEntry[ k ].peFlags = 0;
      }

      // Create the palette and destroy LOGPAL.
      bRC = pal.CreatePalette( pLogPal );
      delete [] (zPCHAR) pLogPal;
   }
   else
   {
      CWindowDC dcScreen( 0 );
      bRC = pal.CreateHalftonePalette( &dcScreen );
   }

   delete [] colors;
   return( bRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// Helper to get color table.  Does all the mem DC voodoo.
//
/////////////////////////////////////////////////////////////////////////////
UINT
ZDib::GetColorTable( RGBQUAD *colorTab, UINT nColors )
{
   CWindowDC dcScreen( 0 );
   CDC memDC;

   memDC.CreateCompatibleDC( &dcScreen );
   CBitmap *pOldBmp = memDC.SelectObject( this );
   nColors = GetDIBColorTable( memDC, 0, nColors, colorTab );
   memDC.SelectObject( pOldBmp );
   return( nColors );
}

CPalette *
ZDib::GetPalette( )
{
   return( &m_palette );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define BIG_DISTANCE 10000000L

static unsigned char masktable[] = { 0x80, 0x40, 0x20, 0x10,
                                     0x08, 0x04, 0x02, 0x01 };

ZDib2::ZDib2( HANDLE hDib, int uBits )
{
   m_pVoid = 0;
   m_pLinePtr = 0;
   m_bUseGamma = FALSE;
   m_lWidth = m_lHeight = 0;
   if ( hDib )
   {
      CreateFromHandle( hDib, uBits );
   }
}

ZDib2::~ZDib2( )
{
   DestroyDIB( );
}

void
ZDib2::DestroyDIB( )
{
   if ( m_pVoid )
      free( m_pVoid );

   m_pVoid = 0;
   if ( m_pLinePtr )
      free( m_pLinePtr );

   m_pLinePtr = 0;
}

zBOOL
ZDib2::Create( int lWidth, int lHeight, int uBits )
{
   // Free existing image.
   DestroyDIB( );
// ASSERT( lBits == 24 || lBits == 8 );

   BITMAPINFOHEADER bmInfo;

   zmemset( &bmInfo, 0, sizeof( BITMAPINFOHEADER ) );
   bmInfo.biSize = sizeof( BITMAPINFOHEADER );
   bmInfo.biWidth = lWidth;
   bmInfo.biHeight = lHeight;
   bmInfo.biPlanes = 1;
   bmInfo.biBitCount = uBits;
   bmInfo.biCompression = BI_RGB;
   return( Create( bmInfo ) );
}

zBOOL
ZDib2::Create( BITMAPINFOHEADER& bmInfo )
{
   m_nBytes = (bmInfo.biBitCount * bmInfo.biWidth) >> 3;
   m_lHeight = bmInfo.biHeight;
   m_lWidth = bmInfo.biWidth;
// bmInfo.biHeight *= -1;
   while ( m_nBytes % 4 )
      m_nBytes++;

   int nSize = sizeof( BITMAPINFOHEADER ) +
               sizeof( RGBQUAD ) * GetPaletteSize( bmInfo ) +
               m_nBytes * m_lHeight;
   m_pVoid = malloc( nSize );
   if ( m_pVoid == 0 )
      return( FALSE );

   m_pInfo = (PBITMAPINFO) m_pVoid;
   zmemcpy( &m_pInfo->bmiHeader, &bmInfo, sizeof( BITMAPINFOHEADER ) );

   m_pRGB = (RGBQUAD *) ((unsigned char *) m_pVoid +
                          sizeof( BITMAPINFOHEADER ));
   m_pBits = (unsigned char *) (m_pVoid) + sizeof( BITMAPINFOHEADER ) +
                               sizeof( RGBQUAD ) * GetPaletteSize( );

   int  k;
   BYTE **ptr;
   m_pLinePtr = (BYTE **) malloc( sizeof( BYTE * ) * m_lHeight );
   if ( m_pLinePtr == 0 )
      return( FALSE );

   for ( k = 0, ptr = m_pLinePtr; k < m_lHeight; k++, ptr++ )
   {
      //*ptr = (int) (m_pBits ) + ( k * bytes );
      //*ptr = (int) GetLinePtr( k );
      *ptr = m_pBits + (m_lHeight - k - 1 ) * m_nBytes;
   }

   m_nFlags = 0;
   return( TRUE );
}

void
ZDib2::SetPalette( unsigned char *palette )
{
   if ( palette )
   {
      int nSize = GetPaletteSize( );
      RGBQUAD *rgb = m_pRGB;

      for ( int k = 0; k < nSize; k++, rgb++, palette+= 3 )
      {
         if ( m_bUseGamma )
         {
            rgb->rgbRed = m_ucGamma[ palette[ 0 ] ];
            rgb->rgbGreen = m_ucGamma[ palette[ 1 ] ];
            rgb->rgbBlue = m_ucGamma[ palette[ 2 ] ];
         }
         else
         {
            rgb->rgbRed = palette[ 0 ];
            rgb->rgbGreen = palette[ 1 ];
            rgb->rgbBlue = palette[ 2 ];
         }

         rgb->rgbReserved = 0;
      }
   }
}

void
ZDib2::SetPalette( RGBQUAD *pRGB )
{
   if ( pRGB )
   {
      int nSize = GetPaletteSize( );
      zmemcpy( m_pRGB, pRGB, nSize * sizeof( RGBQUAD ) );
   }
}

int
ZDib2::GetPaletteSize( )
{
   return( GetPaletteSize( m_pInfo->bmiHeader ) );
}

int
ZDib2::GetPaletteSize( BITMAPINFOHEADER& rbmInfo )
{
   switch ( rbmInfo.biBitCount )
   {
      case 1:
         return( 2 );

      case 4:
         return( 16 );

      case 8:
         return( 256 );

      default:
         return( 0 );
   }
}

void
ZDib2::SetPixel( int x, int y, COLORREF clr )
{
   unsigned char *ptr;

   ASSERT( x >= 0 && y >= 0 );
   ASSERT( x < m_lWidth && y < m_lHeight );

// ptr = m_pBits + (y * bytes) + x * 3;
   ptr = (unsigned char *) m_pLinePtr[ y ];
   ptr += x * 3;
   *ptr++ = (unsigned char) GetBValue( clr );
   *ptr++ = (unsigned char) GetGValue( clr );
   *ptr++ = (unsigned char) GetRValue( clr );
}

void
ZDib2::SetPixel8( int x, int y, unsigned char color )
{
   unsigned char *ptr;
   unsigned char *aptr;

   ASSERT( x >= 0 && y >= 0 );
   ASSERT( x < m_lWidth && y < m_lHeight );

// ptr = m_pBits + (y * bytes) + x;
// ptr = (unsigned char *) m_pLinePtr[ y ];
   ptr = GetLinePtr( y );
   aptr = ptr;
   ptr +=  x;
   *ptr = color;
}

COLORREF
ZDib2::GetPixel( int x, int y )
{
   unsigned char *ptr;
   COLORREF color;

   if ( x < 0 || y < 0 )
      return( 0 );

   if ( x >= m_lWidth || y >= m_lHeight )
      return( 0 );

// ptr = m_pBits + (y * bytes) + x * 3;
   ptr = GetLinePtr( y );
   ptr += (x * 3);
   color = RGB( *(ptr + 2), *(ptr + 1), *ptr );
   return( color );
}

CBitmap *
ZDib2::GetTempBitmap( CDC& dc )
{
   HBITMAP hBitmap;
   CBitmap *pBmpTemp;

   ASSERT( m_pVoid );
   hBitmap = CreateDIBitmap( dc.m_hDC,
                             (PBITMAPINFOHEADER) m_pInfo,
                             CBM_INIT,
                             (const void *) m_pBits,
                             m_pInfo,
                             DIB_RGB_COLORS );

   if ( hBitmap == 0 )
      return( 0 );

   pBmpTemp = CBitmap::FromHandle( hBitmap );
   return( pBmpTemp );
}

CBitmap *
ZDib2::GetBitmap( CDC& dc )
{
   HBITMAP hBitmap;
   CBitmap *pBmpTemp;

   ASSERT( m_pVoid );
   hBitmap = CreateDIBitmap( dc.m_hDC,
                             (PBITMAPINFOHEADER) m_pInfo,
                             CBM_INIT,
                             (const void *) m_pBits,
                             m_pInfo,
                             DIB_RGB_COLORS );

   if ( hBitmap == 0 )
      return( 0 );

   pBmpTemp = CBitmap::FromHandle( hBitmap );
   if ( pBmpTemp )
   {
      BITMAP bmp;
      LPVOID lpVoid;
      pBmpTemp->GetBitmap( &bmp );
      lpVoid = malloc( bmp.bmWidthBytes*bmp.bmHeight );
      if ( lpVoid == 0 )
         return( 0 );

      pBmpTemp->GetBitmapBits( bmp.bmWidthBytes*bmp.bmHeight, lpVoid );
      CBitmap *newBmp = new CBitmap;
      newBmp->CreateBitmapIndirect( &bmp );
      newBmp->SetBitmapBits( bmp.bmWidthBytes*bmp.bmHeight, lpVoid );
      free( lpVoid );
      return( newBmp );
   }
   else
      return( 0 );
}

void
ZDib2::CopyLine( int nSrc, int nTgt )
{
   unsigned char *pSrc;
   unsigned char *pTgt;

   ASSERT( nSrc <= m_lHeight && nSrc >= 0 );
   ASSERT( nTgt <= m_lHeight && nTgt >= 0 );
   if ( nSrc == nTgt )
      return;

   pSrc = GetLinePtr( nSrc );
   pTgt = GetLinePtr( nTgt );
   zmemcpy( pTgt, pSrc, m_nBytes );
}

void
ZDib2::InitDIB( COLORREF color )
{
   int k, j;
   unsigned char *ptr;

   if ( m_pInfo->bmiHeader.biBitCount == 24 )
   {
      unsigned char col[ 3 ];
      col[ 0 ] = GetBValue( color );
      col[ 1 ] = GetGValue( color );
      col[ 2 ] = GetRValue( color );
      for ( k = 0, ptr = m_pBits; k < m_lHeight; k++ )
      {
         ptr = m_pBits + k * m_nBytes;
         for ( j = 0; j < m_lWidth; j++, ptr += 3 )
         {
            zmemcpy( ptr, col, 3 );
         }
      }
   }
   else
   {
      for ( k = 0, ptr = m_pBits; k < m_lHeight; k++, ptr+= m_nBytes )
      {
         zmemset( ptr, (BYTE) color, m_nBytes );
      }
   }
}

void
ZDib2::BitBlt( HDC hTgt, int nTgtX, int nTgtY,
              int lWidth, int lHeight, int nSrcX, int nSrcY )
{
   SetDIBitsToDevice( hTgt, nTgtX, nTgtY,
                      lWidth, lHeight, nSrcX, Height( ) - nSrcY - lHeight,
                      0, Height( ), m_pBits, m_pInfo, DIB_RGB_COLORS );
}

void
ZDib2::StretchBlt( HDC hTgt, int nTgtX, int nTgtY,
                  int nDWidth, int nDHeight,
                  int nSrcX, int nSrcY, int nSWidth, int nSHeight )
{
   int nErr = StretchDIBits( hTgt, nTgtX, nTgtY,
                             nDWidth, nDHeight, nSrcX, nSrcY,
                             nSWidth, nSHeight, m_pBits,
                             (CONST BITMAPINFO *) &m_pInfo->bmiHeader,
                             DIB_RGB_COLORS, SRCCOPY );
}

void
ZDib2::ExpandBlt( int nTgtX, int nTgtY,
                 int nRatioX, int nRatioY,
                 ZDib2& rDibSrc, int nSrcX, int nSrcY,
                 int nSWidth, int nSHeight )
{
   SetPalette( rDibSrc.m_pRGB );

   nSWidth = (nSrcX + nSWidth > rDibSrc.m_lWidth) ?
                                        rDibSrc.m_lWidth - nSrcX : nSWidth;
   nSHeight = (nSrcY + nSHeight > rDibSrc.m_lHeight) ?
                                        rDibSrc.m_lHeight - nSrcY : nSHeight;

   Expand( nTgtX, nTgtY, nRatioX, nRatioY, rDibSrc,
           nSrcX, nSrcY, nSWidth, nSHeight );
}

void
ZDib2::Expand( int nTgtX, int nTgtY,
              int nRatioX, int nRatioY,
              ZDib2& rDibSrc, int nSrcX, int nSrcY,
              int nSWidth, int nSHeight )
{
   int nX, nY, nErrX, nErrY;
   int nDWidth, nDHeight;

   nDWidth = nSWidth * nRatioX;
   nDHeight = nSHeight * nRatioY;

   nDWidth = nTgtX + nDWidth > m_lWidth ? m_lWidth - nTgtX : nDWidth;
   nDHeight = nTgtY + nDHeight > m_lHeight ? m_lHeight - nTgtY : nDHeight;

   nX = nDWidth / nRatioX;
   nY = nDHeight / nRatioY;
   nErrX = nDWidth % nRatioX;
   nErrY = nDHeight % nRatioY;

   unsigned char *buffer, *pSrc, *pTgt, *ptr;
   unsigned char uch;
   int k, j;

   buffer = (unsigned char *) malloc( nDWidth + 20 );
   if ( buffer == 0 )
      return;

   for ( k = 0; k < nY; k++, nSrcY++ )
   {
      pSrc = rDibSrc.GetLinePtr( nSrcY ) + nSrcX;
      ptr = buffer;
      for ( j = 0; j < nX; j++, ptr+= nRatioX )
      {
         zmemset( ptr, *(pSrc + j), nRatioX );
         uch = *(pSrc + j);
      }

      zmemset( ptr, uch, nErrX );
      for ( j = 0; j < nRatioY; j++, nTgtY++ )
      {
         pTgt = GetLinePtr( nTgtY ) + nTgtX;
         zmemcpy( pTgt, buffer, nDWidth );
      }
   }

   for ( k = 0; k < nErrY; k++, nTgtY++ )
   {
      pTgt = GetLinePtr( nTgtY ) + nTgtX;
      zmemcpy( pTgt, buffer, nDWidth );
   }

   free( buffer );
}

void
ZDib2::StretchBlt( int nTgtX, int nTgtY,
                  int nDWidth, int nDHeight,
                  ZDib2& rDibSrc, int nSrcX, int nSrcY,
                  int nSWidth, int nSHeight )
{
   SetPalette( rDibSrc.m_pRGB );
   nDWidth = nTgtX + nDWidth > m_lWidth ? m_lWidth - nTgtX : nDWidth;
   nDHeight = nTgtY + nDHeight > m_lHeight ? m_lHeight - nTgtY : nDHeight;

   nSWidth = nSrcX + nSWidth > rDibSrc.m_lWidth ?
                                       rDibSrc.m_lWidth - nSrcX : nSWidth;
   nSHeight = nSrcY + nSHeight > rDibSrc.m_lHeight ?
                                       rDibSrc.m_lHeight - nSrcY : nSHeight;

   int nDivX, nDivY;
   int nModX, nModY;

   nDivX = nDWidth / nSWidth;
   nModX = nDWidth % nSWidth;

   nDivY = nDHeight / nSHeight;
   nModY = nDHeight % nSHeight;

   if ( nModX == 0 && nModY == 0 && nDivX > 0 && nDivY > 0 )
   {
      ExpandBlt( nTgtX, nTgtY, nDivX, nDivY,
                 rDibSrc, nSrcX, nSrcY, nSWidth, nSHeight );
      return;
   }

   unsigned char *pucTemp, *pucSrcPix, *pucTgtPix, *q;

   pucTemp = (unsigned char *) malloc( nDWidth + 20 );
   int i, j, k, l, x, y, m;
   int nErrX, nErrY;

   for ( i = nErrY = m = 0; i < nSHeight; i++ )
   {
      pucSrcPix = rDibSrc.GetLinePtr( i + nSrcY ) + nSrcX;
      q = pucTemp;
      for ( j = l = nErrX = 0; j < nSWidth; j++, pucSrcPix++ )
      {
         k = nDivX;
         nErrX += nModX;
         if ( nErrX >= nSWidth )
         {
            k++;
            nErrX %= nSWidth;
         }

         x = 0;
         while ( l < nDWidth && x < k )
         {
            *q++ = *pucSrcPix;
            l++;
            x++;
         }
      }

      while ( l < nDWidth )
      {
         *q++ = *pucSrcPix;
         l++;
      }

      k = nDivY;
      nErrY += nModY;
      if ( nErrY >= nSHeight )
      {
         k++;
         nErrY %= nSHeight;
      }

      y = 0;

      while ( m < nDHeight && y < k )
      {
         pucTgtPix = GetLinePtr( m+nTgtY ) + nTgtX;
         zmemcpy( pucTgtPix, pucTemp, nDWidth );
         m++;
         y++;
      }
   }

   while ( m < nDHeight )
   {
      pucTgtPix = GetLinePtr( m + nTgtY ) + nTgtX;
      zmemcpy( pucTgtPix, pucTemp, nDWidth );
      m++;
   }

   free( pucTemp );
}

void
ZDib2::BitBlt( int nTgtX, int nTgtY,
              int lWidth, int lHeight,
              ZDib2& rDibSrc, int nSrcX, int nSrcY, BYTE *colors )
{
   SetPalette( rDibSrc.m_pRGB );
   if ( nTgtX < 0 )
   {
      nSrcX -= nTgtX;
      lWidth += nTgtX;
      nTgtX = 0;
   }

   if ( nTgtY < 0 )
   {
      nSrcY -= nTgtY;
      lHeight += nTgtY;
      nTgtY = 0;
   }

   if ( nSrcX < 0 )
   {
      nTgtX -= nSrcX;
      lWidth += nSrcX;
      nSrcX = 0;
   }

   if ( nSrcY < 0 )
   {
      nTgtY -= nSrcY;
      lHeight += nSrcY;
      nSrcY = 0;
   }

   lWidth = nTgtX + lWidth > m_lWidth ? m_lWidth - nTgtX : lWidth;
   lHeight = nTgtY + lHeight > m_lHeight ? m_lHeight - nTgtY : lHeight;

   lWidth = nSrcX + lWidth > rDibSrc.m_lWidth ?
                                       rDibSrc.m_lWidth - nSrcX : lWidth;
   lHeight = nSrcY + lHeight > rDibSrc.m_lHeight ?
                                       rDibSrc.m_lHeight - nSrcY : lHeight;

   lWidth = __max( 0, lWidth );
   lHeight = __max( 0, lHeight );

   int i, k, l, j;
   unsigned char *pSrc, *pTgt;

   if ( colors == 0 )
   {
      for ( i = 0, k = nSrcY, l = nTgtY; i < lHeight; i++, k++, l++ )
      {
         if ( k < 0 || l < 0 )
         {
            continue;
         }
         else
         {
            pSrc = rDibSrc.GetLinePtr( k );
            pTgt = GetLinePtr( l );
            zmemcpy( pTgt + nTgtX, pSrc + nSrcX, lWidth );
         }
      }
   }
   else
   {
      for ( i = 0, k = nSrcY, l = nTgtY; i < lHeight; i++, k++, l++ )
      {
         if ( k < 0 || l < 0 )
         {
            continue;
         }
         else
         {
            pSrc = rDibSrc.GetLinePtr( k )+nTgtX;
            pTgt = GetLinePtr( l ) + nSrcX;
            for ( j = 0; j < lWidth; j++, pSrc++, pTgt++ )
            {
               if ( colors[ *pSrc ] )
                  *pTgt = *pSrc;
            }
         }
      }
   }
}

unsigned char *
ZDib2::GetLinePtr( int line )
{
// unsigned char *ptr;
// ptr = m_pBits + (m_lHeight - line - 1) * m_nBytes;
// return( ptr );
   return( m_pLinePtr[ line ] );
}

zBOOL
ZDib2::CopyDIB( ZDib2& rDib )
{
   if ( Create( rDib.m_pInfo->bmiHeader ) )
   {
      SetPalette( rDib.m_pRGB );
      zmemcpy( m_pBits, rDib.m_pBits, m_lHeight * m_nBytes );
      return( TRUE );
   }

   return( FALSE );
}

void
ZDib2::ReplaceColor( unsigned char oldColor, unsigned char newColor )
{
   int i, j;
   unsigned char *ptr;

   for ( i = 0; i < m_lHeight; i++ )
   {
      ptr = GetLinePtr( i );
      for ( j = 0; j < m_lWidth; j++ )
      {
         if ( ptr[ j ] == oldColor ) ptr[ j ] = newColor;
      }
   }
}

ZDib2&
ZDib2::operator = ( ZDib2& rDib )
{
   CopyDIB( rDib );
   return( *this );
}

HANDLE
ZDib2::GetDIBits( int lStartX, int lStartY, int nCx, int nCy )
{
   if ( lStartX == -1 )
   {
      lStartX = lStartY = 0;
      nCx = m_lWidth;
      nCy = m_lHeight;
      ZDib2 rDib;
      rDib.Create( nCx, nCy, 8 );
      rDib.BitBlt( 0, 0, nCx, nCy, *this, 0, 0 );
      rDib.SetPalette( m_pRGB );
      return( rDib.DIBHandle( ) );
   }

   return( DIBHandle( ) );
}

zULONG
ZDib2::GetDIBSize( )
{
   return( sizeof( BITMAPINFOHEADER ) +
           sizeof( RGBQUAD ) * GetPaletteSize( ) + m_nBytes * m_lHeight );
}

HANDLE
ZDib2::DIBHandle( )
{
   int nSize;
   HANDLE hMem;

   nSize = sizeof( BITMAPINFOHEADER ) +
           sizeof( RGBQUAD ) * GetPaletteSize( ) + m_nBytes * m_lHeight;
   hMem = GlobalAlloc( GMEM_DDESHARE | GMEM_MOVEABLE, nSize );
   if ( hMem == 0 )
      return( 0 );

   UCHAR *lpVoid, *pBits;
   LPBITMAPINFOHEADER pHead;
   RGBQUAD *pRgb;

   lpVoid = (UCHAR *) GlobalLock( hMem );
   pHead = (LPBITMAPINFOHEADER) lpVoid;
   zmemcpy( pHead, &m_pInfo->bmiHeader, sizeof( BITMAPINFOHEADER ) );
   pRgb = (RGBQUAD *) (lpVoid + sizeof( BITMAPINFOHEADER ));
   zmemcpy( pRgb, m_pRGB, sizeof( RGBQUAD ) * GetPaletteSize( ) );
   pBits = lpVoid + sizeof( BITMAPINFOHEADER ) +
                    sizeof( RGBQUAD ) * GetPaletteSize( );
   zmemcpy( pBits, m_pBits, m_lHeight * m_nBytes );
   GlobalUnlock( lpVoid );
   return( hMem );
}

zBOOL
ZDib2::CreateFromHandle( HANDLE hBitmap, int uBits )
{
   DestroyDIB( );

   UCHAR   *pBitmap, *pBits;
   LPBITMAPINFOHEADER pbmih;
   RGBQUAD *pRgb;

   pBitmap = (UCHAR *) GlobalLock( hBitmap );
   pbmih = (LPBITMAPINFOHEADER) pBitmap;
   if ( pbmih->biCompression != BI_RGB )
   {
      GlobalUnlock( pBitmap );
      return( FALSE );
   }

   if ( pbmih->biBitCount >= 15 )
   {
      if ( pbmih->biBitCount != 24 )
      {
         GlobalUnlock( pBitmap );
         return( FALSE );
      }
   }

   m_lWidth = pbmih->biWidth;
   m_lHeight = pbmih->biHeight;
   m_nBits = pbmih->biBitCount;

   if ( Create( *pbmih ) == 0 )
   {
      GlobalUnlock( pBitmap );
      return( FALSE );
   }

   pRgb = (RGBQUAD *) (pBitmap + sizeof( BITMAPINFOHEADER ));
   zmemcpy( m_pRGB, pRgb, sizeof( RGBQUAD ) * GetPaletteSize( ) );
   pBits = pBitmap + sizeof( BITMAPINFOHEADER ) +
                     sizeof( RGBQUAD ) * GetPaletteSize( );
   zmemcpy( m_pBits, pBits, m_lHeight * m_nBytes );
   GlobalUnlock( pBitmap );
   return( TRUE );
}

void
ZDib2::UseGamma( float fg, zBOOL bUse )
{
   m_bUseGamma = bUse;
   m_fOldGamma = m_fGamma;
   m_fGamma = fg;
   CreateGammaCurve( );
}

void
ZDib2::CreateGammaCurve( )
{
   int k;

   for ( k = 0; k < 256; k++ )
   {
      m_ucGamma[ k ] = (int) (255 * pow( (double) k / 255, (double) m_fGamma ) + (double) 0.5 );
   }
}

void
ZDib2::GetPixel( UINT x, UINT y, int& pixel )
{
   ASSERT( x < (UINT) Width( ) );
   ASSERT( y < (UINT) Height( ) );
   if ( x >= (UINT) Width( ) )
      return;

   if ( y >= (UINT) Height( ) )
      return;

   pixel = (GetLinePtr( y ))[ x ];
}

zBOOL
ZDib2::Make8Bit( ZDib2& rDib )
{
   int nBits;

   ASSERT( Width( ) == rDib.Width( ) );
   ASSERT( Height( ) == rDib.Height( ) );
   nBits = rDib.GetBitCount( );
   switch ( nBits )
   {
      case 1:
         return( SwitchFromOne( rDib ) );
         break;

      case 4:
         return( SwitchFromFour( rDib ) );
         break;

      case 8:
         return( SwitchPalette( rDib ) );
         break;

      case 24:
         return( SwitchFrom24( rDib ) );
         break;

      default:
         return( FALSE );
   }

   return( FALSE );
}

/*
zBOOL
ZDib2::SwitchFrom24( ZDib2& rDib )
{
   int k, j, w, h;
   unsigned char *pSrc, *pTgt;

   w = Width( );
   h = Height( );
   zmemset( m_CachePtr, 0, sizeof( m_CachePtr ) );
   for ( k = 0; k < h; k++ )
   {
      pTgt = GetLinePtr( k );
      pSrc = rDib.GetLinePtr( k );
      for ( j = 0; j < w; j++, pTgt++, pSrc += 3 )
      {
         *pTgt = ClosestColor( (RGBQUAD *) pSrc );
      }
   }

   return( TRUE );
}
*/

zBOOL
ZDib2::SwitchFromOne( ZDib2& rDib )
{
   int k, j, w, h;
   unsigned char *pSrc, *pTgt;
   unsigned char cols[ 2 ];

   w = Width( );
   h = Height( );
   zmemset( m_CachePtr, 0, sizeof( m_CachePtr ) );
   cols[ 0 ] = (unsigned char) ClosestColor( rDib.m_pRGB );
   cols[ 1 ] = (unsigned char) ClosestColor( rDib.m_pRGB + 1 );
   for ( k = 0; k < h; k++ )
   {
      pTgt = GetLinePtr( k );
      pSrc = rDib.GetLinePtr( k );
      for ( j = 0; j < w; j++, pTgt++ )
      {
         if ( (pSrc[ j >> 3 ] & masktable[ j & 7 ]) == 0 )
            *pTgt = cols[ 0 ];
         else
            *pTgt = cols[ 1 ];
      }
   }

   return( TRUE );
}

zBOOL
ZDib2::SwitchFromFour( ZDib2& rDib )
{
   int k, n, j, w, h;
   unsigned char *pSrc, *pTgt;
   unsigned char cols[ 16 ];
   w = Width( );
   h = Height( );
   zmemset( m_CachePtr, 0, sizeof( m_CachePtr ) );
   for ( k = 0; k < 16; k++ )
   {
      cols[ k ] = (unsigned char) ClosestColor( rDib.m_pRGB + k );
   }

   for ( k = 0; k < h; k++ )
   {
      pTgt = GetLinePtr( k );
      pSrc = rDib.GetLinePtr( k );
      for ( j = 0; j < w; j++, pTgt++ )
      {
         if ( (j & 1) == 0 )
            n = (*pSrc & 0xf0) >> 4;
         else
         {
            n = *pSrc & 0x0f;
            pSrc++;
         }

         *pTgt = cols[ n ];
      }
   }

   return( TRUE );
}

zBOOL
ZDib2::SwitchPalette( ZDib2& rDib )
{
   int k, j, w, h;
   unsigned char *pSrc, *pTgt;
   unsigned char cols[ 256 ];

   w = Width( );
   h = Height( );
   zmemset( m_CachePtr, 0, sizeof( m_CachePtr ) );
   for ( k = 0; k < 256; k++ )
   {
      cols[ k ] = (unsigned char) ClosestColor( rDib.m_pRGB + k );
   }

   for ( k = 0; k < h; k++ )
   {
      pTgt = GetLinePtr( k );
      pSrc = rDib.GetLinePtr( k );
      for ( j = 0; j < w; j++, pSrc++, pTgt++ )
      {
         *pTgt = cols[ *pSrc ];
      }
   }

   return( TRUE );
}

int
ZDib2::ClosestColor( RGBQUAD *pRgb )
{
   zULONG dist = BIG_DISTANCE, d, c;
   RGBQUAD *pQuad = m_pRGB;
   int k, nSize = GetPaletteSize( );
   for ( k = 0; k < nSize; k++ )
   {
      if ( m_CachePtr[ k ] )
      {
         if ( zmemcmp( &m_rgbQuadCache[ k ], pRgb, 3 ) == 0 )
         {
            return( k );
         }
      }
   }

   for ( k = 0; k < nSize; k++, pQuad++ )
   {
      d = Distance( *pRgb, *pQuad );
      if ( d == 0 )
      {
         m_rgbQuadCache[ k ] = *pRgb;
         m_CachePtr[ k ] = 1;
         return( k );
      }

      if ( dist > d )
      {
         c = k;
         dist = d;
      }
   }

   m_rgbQuadCache[ c ] = *pRgb;
   m_CachePtr[ c ] = 1;
   return( c );
}

zULONG
ZDib2::Distance( RGBQUAD& rgb1, RGBQUAD& rgb2 )
{
   zULONG d;

   d = 3 * (unsigned) ((rgb1.rgbRed) - (rgb2.rgbRed)) *
           (unsigned) ((rgb1.rgbRed) - (rgb2.rgbRed));
   d += 4 * (unsigned) ((rgb1.rgbGreen) - (rgb2.rgbGreen)) *
            (unsigned) ((rgb1.rgbGreen) - (rgb2.rgbGreen));
   d += 2 * (unsigned) ((rgb1.rgbBlue) - (rgb2.rgbBlue)) *
            (unsigned) ((rgb1.rgbBlue) - (rgb2.rgbBlue));
   return( d );
}

zBOOL
ZDib2::OpenDIB( zCPCHAR cpcFileName )
{
   CFile file;

   if ( file.Open( cpcFileName, CFile::modeRead | CFile::typeBinary ) == 0 )
   {
      return( FALSE );
   }

   file.Close( );
   if ( OpenBMP( cpcFileName ) )
      return( TRUE );

   return( FALSE );
}

zBOOL
ZDib2::SaveDIB( zCPCHAR cpcFileName, eBitmapType eType )
{
   CFile file;

   if ( file.Open( cpcFileName, CFile::modeCreate |
                                CFile::modeWrite |
                                CFile::typeBinary ) == 0 )
   {
      return( FALSE );
   }

   file.Close( );
   switch ( eType )
   {
      case BMP:
         return( SaveBMP( cpcFileName ) );

      default:
         return( FALSE );
   }

   return( FALSE );
}

zBOOL
ZDib2::SaveBMP( zCPCHAR cpcFileName )
{
   BITMAPFILEHEADER bmfh;
   CFile file;

   if ( file.Open( cpcFileName,
                   CFile::modeCreate |
                   CFile::modeWrite |
                   CFile::typeBinary ) == 0 )
   {
      return( FALSE );
   }

   zmemset( &bmfh, 0, sizeof( bmfh ) );
   zmemcpy( &bmfh.bfType, "BM", 2 );
   bmfh.bfSize = GetDIBSize( ) + sizeof( bmfh );
   bmfh.bfOffBits = sizeof( BITMAPINFOHEADER ) +
                     GetPaletteSize( ) * sizeof( RGBQUAD ) +
                     sizeof( BITMAPFILEHEADER );
   file.Write( &bmfh, sizeof( bmfh ) );
   file.Write( m_pVoid, GetDIBSize( ) );
   file.Close( );
   return( TRUE );
}

zBOOL
ZDib2::OpenBMP( zCPCHAR cpcFileName )
{
   BITMAPFILEHEADER bmfh;
   BITMAPINFOHEADER head;
   CFile file;

   if ( file.Open( cpcFileName, CFile::modeRead | CFile::typeBinary ) == 0 )
   {
      return( FALSE );
   }

   file.Read( &bmfh, sizeof( bmfh ) );
   if ( zmemcmp( &bmfh.bfType, "BM", 2 ) )
   {
      file.Close( );
      return( FALSE );
   }

   file.Read( &head, sizeof( head ) );
   if ( Create( head ) == 0 )
   {
      file.Close( );
      return( FALSE );
   }

   file.Read( m_pRGB, sizeof( RGBQUAD ) * GetPaletteSize( ) );
   file.Seek( bmfh.bfOffBits, CFile::begin );
   file.Read( m_pBits, m_lHeight * m_nBytes );
   file.Close( );
   return( TRUE );
}

int
ZDib2::CountColors( )
{
   ASSERT( GetBitCount( ) == 8 );
   BYTE colors[ 256 ], *ptr;
   int lNbr = 0, k, j, w, d;

   w = Width( );
   d = Height( );
   zmemset( colors, 0, 256 );
   for ( k = 0; k < d; k++ )
   {
      ptr = GetLinePtr( k );
      for ( j = 0; j < w; j++, ptr++ )
      {
         if ( colors[ *ptr ] == 0 )
         {
            colors[ *ptr ] = 1;
            lNbr++;
         }
      }
   }

   return( lNbr );
}

int
ZDib2::EnumColors( BYTE *array )
{
   ASSERT( GetBitCount( ) == 8 );
   BYTE *ptr;
   int nNbr = 0, k, j, w, d;

   w = Width( );
   d = Height( );
   zmemset( array, 0, 256 );
   for ( k = 0; k < d; k++ )
   {
      ptr = GetLinePtr( k );
      for ( j = 0; j < w; j++, ptr++ )
      {
         if ( array[ *ptr ] == 0 )
         {
            array[ *ptr ] = 1;
            nNbr++;
         }
      }
   }

   return( nNbr );
}

COLORREF
ZDib2::PaletteColor( int nIndex )
{
   ASSERT( nIndex < 256 );
   RGBQUAD *pRGB = m_pRGB + nIndex;

   return( RGB( pRGB->rgbRed, pRGB->rgbGreen, pRGB->rgbBlue ) );
}

zBOOL
ZDib2::SwitchFrom24( ZDib2& rDib )
{
   int k, j, w, h, c;
   unsigned char *pSrc, *pTgt;
   BYTE *index_ptr = 0;
   RGBQUAD rgb;
   w = Width( );
   h = Height( );
   index_ptr = (BYTE *) malloc( 0x7FFF+1 );

   if ( index_ptr == 0 )
      return( FALSE );

   zmemset( m_CachePtr, 0, sizeof( m_CachePtr ) );
   for ( k = 0; k <= 0x7FFF; k++ )
   {
      rgb.rgbRed = (((k & 0x7C00) >> 10) << 3) | 0x07;
      rgb.rgbGreen = (((k & 0x3e0) >> 5) << 3) | 0x07;
      rgb.rgbBlue = ((k & 0x1F) << 3) | 0x07;
      index_ptr[ k ] = (unsigned char) ClosestColor( &rgb );
   }

   for ( k = 0; k < h; k++ )
   {
      pTgt = GetLinePtr( k );
      pSrc = rDib.GetLinePtr( k );
      for ( j = 0; j < w; j++, pTgt++, pSrc+= 3 )
      {
         c = (*pSrc >> 3) | ((*(pSrc + 1) >> 3) << 5) |
             ((*(pSrc + 2) >> 3) << 10);
         *pTgt = index_ptr[ c ];
      }
   }

   free( index_ptr );
   return( TRUE );
}
