/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlbmp.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlbmp_h__  // Sentry, use file only if it's not already included
#define __zdctlbmp_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
#include ".\jpeglib\jpeglib.h"

AFX_EXT_API
CWnd * OPERATION
BmpCtl( ZSubtask *pZSubtask,
        CWnd     *pWndParent,
        ZMapAct  *pzmaComposite,
        zVIEW    vDialog,
        zSHORT   nOffsetX,
        zSHORT   nOffsetY,
        zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

#define zBM_LCLICK        1
#define zBM_LDBLCLK       2
#define zBM_RCLICK        3
#define zBM_RDBLCLK       4
#define zBM_SETFOCUS      5
#define zBM_KILLFOCUS     6
#define zBM_POSCHANGED 9998
#define zBM_PAINT      9999

class ZDCTL_CLASS ZBmpFile;

// Container class for Entities in diagram
class ZDCTL_CLASS ZBmpCtl : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZBmpCtl )
public:
   ZBmpCtl( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~ZBmpCtl( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zBOOL  InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus );
   virtual zBOOL  InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus );

   zBOOL  IsDoubleClick( zUSHORT uMessage, CPoint ptStart );
   void   ProcessCommand( zUSHORT uNotificationCode );

   zBOOL  LoadDib( HINSTANCE hInstDLL, zPCHAR pchBitmap );
   zSHORT SetBitmapFileName( zCPCHAR cpcBitmapFileName );
   void   SetBitmapResource( zCPCHAR cpcDLL_Name, UINT uBitmapResId );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg void OnPaletteChanged( CWnd *pFocusWnd );
   afx_msg BOOL OnQueryNewPalette( );
   afx_msg void OnPaint( );

   virtual BOOL PreTranslateMessage( MSG *pMsg );

public:

   zPCHAR m_pchCtrlBOI;          // Hold the CtrlBOI
   zPCHAR m_pchBitmapFileName;   // File name for mapping
   zPCHAR m_pchVEAPX;            // Indirect file name for mapping

   ZDib     m_dib;
   ZBmpFile *m_pBmpFile;

// zLONG m_lRop;
// zBOOL  m_bUseDDB;
   zBOOL  m_bFit;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//typedef enum {FALSE, TRUE}  boolean;

#define schar               signed char
#define sshort              signed short
#define slong               signed long
#define uchar               unsigned char
#define ushort              unsigned short
#define ulong               unsigned long

#ifndef NULL
#define NULL                0
#endif
#ifndef SEEK_SET
#define SEEK_SET            0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR            1
#endif
#ifndef SEEK_END
#define SEEK_END            2
#endif

#define READ_TYPE           "rb"
#define WRITE_TYPE          "wb"

//#define IMPORT              extern
//#define EXPORT
//#define LOCAL               static
//#define GLOBAL

#define WIDTHBYTES( bits )  (((bits) + 31) / 32 * 4) // for DWORD aligning a buffer

#define BMP_HEADERSIZE (3 * 2 + 4 * 12)
#define BMP_BYTESPERLINE( width, bits ) ((((width) * (bits) + 31) / 32) * 4)
#define BMP_PIXELSIZE( width, height, bits ) (((((width) * (bits) + 31) / 32) * 4) * height)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDCTL_CLASS ZBmpFile
{
public:

   // operations

   ZBmpFile( );
   ~ZBmpFile( );

   HBITMAP LoadAnImageX( zPCHAR pchFileName );
   BYTE * LoadBMP( CString fileName, UINT *width, UINT *height );

   void SaveBMP( CString fileName,    // output path
                 BYTE * buf,          // RGB buffer
                 UINT width,          // size
                 UINT height );

   void SaveBMP( CString fileName,         // output path
                 BYTE * colormappedbuffer, // one BYTE per pixel colomapped image
                 UINT width,
                 UINT height,
                 int bitsperpixel,         // 1, 4, 8
                 int colors,               // number of colors (number of RGBQUADs)
                 RGBQUAD *colormap );      // array of RGBQUADs

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

   void OnFileOpen( CWnd *pWnd );
   void OnFileSaveAs( CWnd *pWnd );
   void OnFileSavecolormappedbmp( CWnd *pWnd );
   void OnFileSavegrayas( CWnd *pWnd );
   void OnFileGetdimensionsJPG( CWnd *pWnd );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

   int     m_bits;
   UINT    m_quantColors;
   BOOL    m_color;

public:
   // parameters
   CString m_errorText;
   DWORD m_bytesRead;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Operations
public:

   // global image params
   BYTE *m_pBuf;
   UINT m_width;
   UINT m_height;
// UINT m_widthDW;

   // draw what we've loaded
   void DrawBMP( CWnd *pWnd, BOOL bFit );

   // jpg load save
   void LoadJPG( CString fileName );
   void SaveJPG( CWnd *pWnd, CString filename, BOOL color );

   void LoadBMP( CString fileName );
   void SaveBMP24( CWnd *pWnd, CString fileName );

   // turn 24-bit to 256-color gray scale
   BYTE * MakeColormappedGrayscale( BYTE *inBuf,
                                    UINT inWidth,
                                    UINT inHeight,
                                    UINT inWidthBytes,
                                    UINT colors,
                                    RGBQUAD *colormap );
};

typedef struct
{
   ulong r, g, b;
   ulong pixel_count;
   ulong pixels_in_cube;
   uchar children;
   uchar palette_index;
} CUBE;

typedef struct
{
   uchar  level;
   ushort index;
} FCUBE;

typedef struct
{
   uchar palette_index,
    red, green, blue;
   ulong distance;
   ulong squares[255+255+1];
} CLOSEST_INFO;

int
dl1quant( uchar *inbuf,
          uchar *outbuf,
          int width,
          int height,
          int quant_to,
          int dither,
          uchar userpal[ 3 ][ 256 ] );

void copy_pal( uchar userpal[ 3 ][ 256 ] );
void dlq_init( void );
int  dlq_start( void );
void dlq_finish( void );
int  build_table( uchar *image, ulong pixels );
void fixheap( ulong id );
void reduce_table( int num_colors );
void set_palette( int index, int level );
void closest_color( int index, int level );
int  quantize_image( uchar *in, uchar *out,
                     int width, int height, int dither );
int  bestcolor( int r, int g, int b );

// All functions here are static. There is no need to have a ZJpegFile object.
// There is actually nothing in a ZJpegFile object anyway.
//
// So, you can do this :
//
//    BOOL ok = ZJpegFile::vertFlipBuf( buf, widthbytes, height );
//
// instead of this :
//
//    ZJpegFile jpgOb;
//    BOOL ok = jpgOb.vertFlipBuf( buf, widthbytes, height );
//
/////
//
// Linking usage :
// It is sometimes necessary to set /NODEFAULTLIB:LIBC (or LIBCD) to use this
// class.
//
/////
//
// Error reporting:
// The class generates message boxes in response to JPEG errors.
//
// The ZJpegFile.cpp fn my_error_exit defines the behavior for
// fatal errors : show msg box, return to caller.
//
// Warnings are handled by jpeglib.lib - which  generates msg boxes too.
//
/////////////////////////////////////////////////////////////////////////////

/*
/////////////////////////////////////////////////////////////////////////////
// Reading Usage :

   UINT height;
   UINT width;
   BYTE *dataBuf;
   //read the file
   dataBuf = ZJpegFile::JpegFileToRGB( fileName, &width, &height );
   if ( dataBuf == NULL )
   {
      return;
   }

   // RGB -> BGR
   ZJpegFile::BGRFromRGB( dataBuf, m_width, m_height );


   BYTE *buf;
   // create a DWORD aligned buffer from the ZJpegFile object
   buf = ZJpegFile::MakeDwordAlignedBuf( dataBuf, width, height, &m_widthDW );

   // flip that buffer
   ZJpegFile::VertFlipBuf( m_pBuf, m_widthDW, m_height );

   // you now have a buffer ready to be used as a DIB

   // be sure to delete [] dataBuf; // !!!!!!!!!!
   //       delete [] buf;


   // Writing Usage


   // this assumes your data is stored as a 24-bit RGB DIB.
   // if you have a 1,4,8,15/16 or 32 bit DIB, you'll have to
   // do some work to get it into a 24-bit RGB state.

   BYTE *tmp = NULL;

   // assume buf is a DWORD-aligned BGR buffer, vertically flipped
   // as if read from a BMP file.

   // un-DWORD-align
   tmp = ZJpegFile::RGBFromDWORDAligned( buf, widthPix, widthBytes, height );

   // vertical flip
   ZJpegFile::VertFlipBuf( tmp, widthPix * 3, height );

   // reverse BGR
   ZJpegFile::BGRFromRGB( tmp, widthPix, height );

   if ( tmp == NULL )
   {
      AfxMessageBox( "~DWORD Memory Error" );
      return;
   }

   // write it
   BOOL ok = ZJpegFile::RGBToJpegFile( fileName, tmp, width, height, TRUE, 75 );
   if ( !ok )
   {
      AfxMessageBox( "Write Error" );
   }

   delete [] tmp;

/////////////////////////////////////////////////////////////////////////////
*/

#endif   // __zdctlbmp_h__ sentry.
