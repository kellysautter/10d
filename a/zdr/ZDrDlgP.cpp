/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdlgp.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality for printing available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1998.09.07    DKS
//    Complete rewrite. (XC 236)
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


//./ ADD NAME=PrintActiveWindow
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PrintActiveWindow
//                                              05/20/93 Modified: 09/07/98
//
//  PURPOSE:   To print the window associated with the specified subtask.
//
//  PARAMETERS: vSubtask  - The subtask view for the window to be printed
//
//  RETURNS:    0  - print was successful
//             -1  - Error locating window
//             otw - other printing error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PrintActiveWindow( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;
   zSHORT   nRC;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zCHAR szMessage[ 1024 ];
      zCHAR szTitle[ 512 ];

      pZSubtask->m_pZFWnd->GetWindowText( szTitle, zsizeof( szTitle ) - 1 );
      if ( pZSubtask->m_pzmaWithFocus &&
           pZSubtask->m_pzmaWithFocus->PrintZCtrl( TRUE ) )
      {
         zCHAR szCaption[ 256 ];

         sprintf_s( szCaption, zsizeof( szCaption ), "Print Window or %s",
                   pZSubtask->m_pzmaWithFocus->m_csERemote.GetString());
         sprintf_s( szMessage, zsizeof( szMessage ), "OK to print %s control:\n"
                              "for Dialog: %s.%s.%s\n"
                              "for Window: %s?",
                   pZSubtask->m_pzmaWithFocus->m_csERemote.GetString(),
                   (*(pZSubtask->m_pzsDlgTag)).GetString(), (*(pZSubtask->m_pzsWndTag)).GetString(),
                   (*(pZSubtask->m_pzmaWithFocus->m_pzsTag)).GetString(), szTitle );

         szTitle[ 0 ] = 0;
         zLONG lFlags = 0x000F0000;

         nRC = ConstructZeidonPrompt( vSubtask, szCaption,
                                      szMessage, szTitle,
                                      zsizeof( szTitle ), &lFlags,
                                      "Report Title:;"
                                        "Print Control;"
                                        "Print Window;"
                                        "Separator Lines;"
                                        "Landscape;"
                                        "Preview;"
                                        "Select Printer",
                                      0, IDD_INPUTDIALOG );
         if ( nRC == IDOK )
         {
            return( pZSubtask->m_pzmaWithFocus->PrintZCtrl( FALSE, szTitle,
                                                            lFlags ) );
         }
         else
         if ( nRC == IDNO )
         {
            PumpMessages( vSubtask );  // let repainting occur to remove prompt
            strcpy_s( szMessage, zsizeof( szMessage ), *(pZSubtask->m_pzsDlgTag) );
            strcat_s( szMessage, zsizeof( szMessage ), "." );
            strcat_s( szMessage, zsizeof( szMessage ), *(pZSubtask->m_pzsWndTag) );
            return( PrintWnd( vSubtask, szMessage ) );
         }
      }
      else
      {
         zSHORT nButtons = zBUTTONS_YESNO;
#ifdef _DEBUG
         SysReadZeidonIni( -1, "[Debug]", "BombZDr", szMessage, zsizeof( szMessage ) );
         if ( szMessage[ 0 ] == 'Y' )
            nButtons = zBUTTONS_YESNOCANCEL;
#endif
         sprintf_s( szMessage, zsizeof( szMessage ), "OK to print Window: %s (%s.%s)?",
                    szTitle, (*(pZSubtask->m_pzsDlgTag)).GetString(),
                    (*(pZSubtask->m_pzsWndTag)).GetString() );
         nRC = OperatorPrompt( vSubtask, "Print Window", szMessage, 0, nButtons, zRESPONSE_YES, 0 );
         if ( nRC == zRESPONSE_YES )
         {
            PumpMessages( vSubtask );  // let repainting occur to remove prompt
            szMessage[ zstrlen( szMessage ) - 1 ] = 0;  // knock out the '?'
            return( PrintWnd( vSubtask, szMessage + 19 ) );
         }
#ifdef _DEBUG
         else
         if ( nRC == zRESPONSE_CANCEL )
         {
            ASSERT( FALSE );
         }
#endif
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=SaveActiveCtrlToCSV
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SaveActiveCtrlToCSV
//                                              05/20/93 Modified: 09/07/98
//
//  PURPOSE:   To print the window associated with the specified subtask.
//
//  PARAMETERS: vSubtask  - The subtask view for the window to be printed
//
//  RETURNS:    0  - print was successful
//             -1  - Error locating window
//             otw - other printing error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SaveActiveCtrlToCSV( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;
   zSHORT   nRC;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      LPAPP lpApp;
      zCHAR szMessage[ 1024 ];
      zCHAR szTitle[ 512 ];
      zCHAR szFileName[ zMAX_FILESPEC_LTH + 1 ];
      zLONG lFlags = 0x00000000;

      pZSubtask->m_pZFWnd->GetWindowText( szTitle, zsizeof( szTitle ) - 1 );
      if ( pZSubtask->m_pzmaWithFocus &&
           pZSubtask->m_pzmaWithFocus->PrintZCtrl( TRUE ) )
      {
         zCHAR szCaption[ 256 ];

         sprintf_s( szCaption, zsizeof( szCaption ), "Create CSV" );
         sprintf_s( szMessage, zsizeof( szMessage ), "OK to create CSV file for %s control:\n"
                               "for Dialog: %s.%s.%s\n"
                               "for Window: %s?",
                    pZSubtask->m_pzmaWithFocus->m_csERemote.GetString(),
                    (*(pZSubtask->m_pzsDlgTag)).GetString(), (*(pZSubtask->m_pzsWndTag)).GetString(),
                    (*(pZSubtask->m_pzmaWithFocus->m_pzsTag)).GetString(), szTitle );

         // USERPROFILE=C:\Documents and Settings\Doug Sorensen
         szFileName[ 0 ] = 0;
         if ( SysGetEnvVar( szFileName, "USERPROFILE", zsizeof( szFileName ) - 8 ) == 0 )
         {
            strcat_s( szFileName, zsizeof( szFileName ), "\\desktop\\" );
         }
         else
         {
            SfGetApplicationForSubtask( &lpApp, pZSubtask->m_vDialog );
            if ( lpApp )
            {
               strcpy_s( szFileName, zsizeof( szFileName ), lpApp->szLocalDir );
            }
         }

         strcat_s( szFileName, zsizeof( szFileName ), *(pZSubtask->m_pzmaWithFocus->m_pzsTag) );
         strcat_s( szFileName, zsizeof( szFileName ), ".csv" );
         zstrlower( szFileName );

         nRC = ConstructZeidonPrompt( vSubtask, szCaption,
                                      szMessage, szFileName,
                                      zsizeof( szTitle ), &lFlags,
                                      "CSV File Name:;"
                                        "Create CSV;"
                                        "Create CSV As...",
                                      0, IDD_INPUTDIALOG );
         if ( nRC == IDOK || nRC == IDNO )  // Create CSV or Create CSV As...
         {
            if ( nRC == IDNO )  // Create CSV As...
            {
               if ( OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ),
                                           "CSV Files (*.csv)|*.csv|", "csv",
                                           zOFN_OVERWRITEPROMPT ) != 1 )
               {
                  return( 0 );
               }
            }

            zstrlower( szFileName );
            zPCHAR pch = zstrrchr( szFileName, '.' );
            if ( zstrchr( pch, '\\' ) == 0 && zstrcmp( pch, ".csv" ) == 0 )
               *pch = 0;

            lFlags = 0x00000010; // ==> zREPORT_PRINT2CSV
            return( pZSubtask->m_pzmaWithFocus->PrintZCtrl( FALSE, szFileName, lFlags ) );
         }
         else
         {
            PumpMessages( vSubtask );  // let repainting occur to remove prompt
            return( 0 );
         }
      }
      else
      {
         zSHORT nButtons = zBUTTONS_YESNO;
#ifdef _DEBUG
         SysReadZeidonIni( -1, "[Debug]", "BombZDr", szMessage, zsizeof( szMessage ) );
         if ( szMessage[ 0 ] == 'Y' )
            nButtons = zBUTTONS_YESNOCANCEL;
#endif
         sprintf_s( szMessage, zsizeof( szMessage ), "OK to print Window: %s (%s.%s)?",
                    szTitle, (*(pZSubtask->m_pzsDlgTag)).GetString(),
                    (*(pZSubtask->m_pzsWndTag)).GetString() );
         nRC = OperatorPrompt( vSubtask, "Print Window", szMessage, 0, nButtons, zRESPONSE_YES, 0 );
         if ( nRC == zRESPONSE_YES )
         {
            szMessage[ zstrlen( szMessage ) - 1 ] = 0;  // knock out the '?'
            return( PrintWnd( vSubtask, szMessage + 19 ) );
         }
#ifdef _DEBUG
         else
         if ( nRC == zRESPONSE_CANCEL )
         {
            ASSERT( FALSE );
         }
#endif
      }

      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION : InitPrinting( CDC& dc, HWND hWnd, HANDLE hInst, zPCHAR msg )
//
// PURPOSE  : Makes preliminary driver calls to set up print job.
//
// RETURNS  : TRUE  - if successful.
//            FALSE - otherwise.
//
/////////////////////////////////////////////////////////////////////////////
BOOL
InitPrinting( CDC& dc,
           // HWND hWnd, HANDLE hInst,
              zCPCHAR cpcMsg )
{
   DOCINFO      DocInfo;

//x   bError     = FALSE;     // no errors yet
//x   bUserAbort = FALSE;     // user hasn't aborted
//x   hWndParent = hWnd;      // save for Enable at Term time

//x   lpfnPrintDlgProc = (DLGPROC) MakeProcInstance( PrintDlgProc, hInst );
//x   lpfnAbortProc = (WNDPROC) MakeProcInstance( AbortProc, hInst );

//x   hDlgPrint = CreateDialog( hInst, "PRTDLG", hWndParent,
//x                             (DLGPROC) lpfnPrintDlgProc );

//x   if ( hDlgPrint == 0 )
//x      return( FALSE );

//x   SetWindowText( hDlgPrint, msg );
//x   EnableWindow( hWndParent, FALSE );     /* disable parent */

   //
   // Use new printing APIs...Petrus Wong 12-May-1993
   //
//x   if ( dc.SetAbortProc( (ABORTPROC) lpfnAbortProc ) <= 0 )
//x   {
//x      bError = TRUE;
//x      return( FALSE );
//x   }

   zmemset( &DocInfo, 0, sizeof( DOCINFO ) );
   DocInfo.cbSize   = sizeof( DOCINFO );
   DocInfo.lpszDocName = cpcMsg;
   DocInfo.lpszOutput  = NULL;

   if ( dc.StartDoc( &DocInfo ) <= 0 )
   {
//x      bError = TRUE;
      return( FALSE );
   }

//x   bError = FALSE;

   // Might want to call the abort proc here to allow the user to
   // abort just before printing begins.
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   FindDIBBits
//
//             LPSTR lpbi - pointer to packed-DIB memory block (either
//                          BITMAPINFOHEADER or BITMAPCOREHEADER)
//
// Purpose:    Given a pointer to a DIB, calculates the address of the
//             DIBS's bits adnd returns a pointer to the DIB's bitmap bits.
//
/////////////////////////////////////////////////////////////////////////////
LPSTR WINAPI
FindDIBBits( LPSTR lpbi )
{
   return( lpbi + *((LPDWORD) lpbi) + ::PaletteSize( lpbi ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// PaintDIB()
//
// Parameters:
//
// HDC hDC          - DC to do output to
//
// LPRECT lpDCRect  - rectangle on DC to do output to
//
// HDIB hDib        - handle to global memory with a DIB spec
//                    in it followed by the DIB bits
//
// LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
//
// CPalette* pPal   - pointer to CPalette containing DIB's palette
//
// Return Value:
//
// BOOL             - TRUE if DIB was drawn, FALSE otherwise
//
// Description:
//   Painting routine for a DIB.  Calls StretchDIBits() or
//   SetDIBitsToDevice() to paint the DIB.  The DIB is
//   output to the specified DC, at the coordinates given
//   in lpDCRect.  The area of the DIB to be output is
//   given by lpDIBRect.
//
/////////////////////////////////////////////////////////////////////////////
BOOL WINAPI
PaintDIB( HDC      hDC,
          LPRECT   lpDCRect,
          HDIB     hDib,
          LPRECT   lpDIBRect,
          CPalette *pPal )
{
   LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
   LPSTR    lpDIBBits;           // Pointer to DIB bits
   BOOL     bSuccess = FALSE;    // Success/fail flag
   HPALETTE hPal = NULL;         // Our DIB's palette
   HPALETTE hOldPal = NULL;      // Previous palette

   // Check for valid DIB handle
   if ( hDib == 0 )
      return( FALSE );

   // Lock the DIB, and get a pointer to the beginning of the bit buffer.
   lpDIBHdr  = (LPSTR) ::GlobalLock( (HGLOBAL) hDib );
   lpDIBBits = ::FindDIBBits( lpDIBHdr );

   // Get the DIB's palette, then select it into DC
   if ( pPal )
   {
      hPal = (HPALETTE) pPal->m_hObject;

      // Select as background since we have already realized in forground
      // if needed.
      hOldPal = ::SelectPalette( hDC, hPal, TRUE );
   }

   // Use the stretching mode best for color pictures.
   ::SetStretchBltMode( hDC, COLORONCOLOR );

   // Determine whether to call StretchDIBits( ) or SetDIBitsToDevice( ).
   if ( (RECTWIDTH( lpDCRect )  == RECTWIDTH( lpDIBRect )) &&
        (RECTHEIGHT( lpDCRect ) == RECTHEIGHT( lpDIBRect )) )
   {
      bSuccess =
         ::SetDIBitsToDevice( hDC,                             // hDC
                              lpDCRect->left,                  // DestX
                              lpDCRect->top,                   // DestY
                              RECTWIDTH( lpDCRect ),           // nDestWidth
                              RECTHEIGHT( lpDCRect ),          // nDestHeight
                              lpDIBRect->left,                 // SrcX
                              (int) ::DIBHeight( lpDIBHdr ) -
                                lpDIBRect->top -
                                RECTHEIGHT( lpDIBRect ),       // SrcY
                              0,                               // nStartScan
                              (WORD) ::DIBHeight( lpDIBHdr ),  // nNumScans
                              lpDIBBits,                       // lpBits
                              (LPBITMAPINFO) lpDIBHdr,         // lpBitsInfo
                              DIB_RGB_COLORS );                // wUsage
   }
   else
   {
     bSuccess =
         ::StretchDIBits( hDC,                                 // hDC
                          lpDCRect->left,                      // DestX
                          lpDCRect->top,                       // DestY
                          RECTWIDTH( lpDCRect ),               // nDestWidth
                          RECTHEIGHT( lpDCRect ),              // nDestHeight
                          lpDIBRect->left,                     // SrcX
                          lpDIBRect->top,                      // SrcY
                          RECTWIDTH( lpDIBRect ),              // wSrcWidth
                          RECTHEIGHT( lpDIBRect ),             // wSrcHeight
                          lpDIBBits,                           // lpBits
                          (LPBITMAPINFO) lpDIBHdr,             // lpBitsInfo
                          DIB_RGB_COLORS,                      // wUsage
                          SRCCOPY );                           // dwROP
   }

   ::GlobalUnlock( (HGLOBAL) hDib );

   // Reselect old palette.
   if ( hOldPal )
      ::SelectPalette( hDC, hOldPal, TRUE );

   return( bSuccess );
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION : TermPrinting( CDC& dc )
//
// PURPOSE  : Terminates print job.
//
/////////////////////////////////////////////////////////////////////////////
void
TermPrinting( CDC& dc )
{
   //
   // Use new printing APIs...Petrus Wong 12-May-1993
   //
//x   if ( bError == FALSE )
      dc.EndDoc( );

//x   if ( bUserAbort )
//x      dc.AbortDoc( );
//x   else
//x   {
//x      EnableWindow( hWndParent, TRUE );
//x      DestroyWindow( hDlgPrint );
//x   }

//x   FreeProcInstance( lpfnAbortProc );
//x   FreeProcInstance( lpfnPrintDlgProc );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   InitBitmapInfoHeader
//
// Purpose:    Does a "standard" initialization of a BITMAPINFOHEADER,
//             given the Width, Height, and Bits per Pixel for the
//             DIB.
//
//             By standard, I mean that all the relevant fields are set
//             to the specified values.  biSizeImage is computed, the
//             biCompression field is set to "no compression," and all
//             other fields are 0.
//
//             Note that DIBs only allow BitsPixel values of 1, 4, 8, or
//             24.  This routine makes sure that one of these values is
//             used (whichever is most appropriate for the specified
//             nBPP).
//
// Parms:      lpBmInfoHdr == Far pointer to a BITMAPINFOHEADER structure
//                            to be filled in.
//             dwWidth     == Width of DIB (not in Win 3.0 & 3.1, high
//                            word MUST be 0).
//             dwHeight    == Height of DIB (not in Win 3.0 & 3.1, high
//                            word MUST be 0).
//             nBPP        == Bits per Pixel for the DIB.
//
// History:   Date      Reason
//            11/07/91  Created
//
/////////////////////////////////////////////////////////////////////////////
void
InitBitmapInfoHeader( LPBITMAPINFOHEADER lpBmInfoHdr,
                      zULONG dwWidth,
                      zULONG dwHeight,
                      zSHORT nBPP )
{
   zmemset( lpBmInfoHdr, 0, sizeof( BITMAPINFOHEADER ) );

   lpBmInfoHdr->biSize      = sizeof( BITMAPINFOHEADER );
   lpBmInfoHdr->biWidth     = dwWidth;
   lpBmInfoHdr->biHeight    = dwHeight;
   lpBmInfoHdr->biPlanes    = 1;

   if ( nBPP <= 1 )
      nBPP = 1;
   else
   if ( nBPP <= 4 )
      nBPP = 4;
   else
   if ( nBPP <= 8 )
      nBPP = 8;
   else
      nBPP = 24;

   lpBmInfoHdr->biBitCount  = nBPP;
   lpBmInfoHdr->biSizeImage = WIDTHBYTES( dwWidth * nBPP ) * dwHeight;
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   PalEntriesOnDevice
//
// Purpose:    Returns the number of colors a device supports.
//
// Parms:      hDC == DC for the device we want # of colors for.
//
// Returns:    # of colors that the given device can represent.
//
// History:   Date      Reason
//            06/01/91  Created
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
PalEntriesOnDevice( HDC hDC )
{
   zSHORT nColors;

   // Find out the number of palette entries on this
   //  defice.
   nColors = GetDeviceCaps( hDC, SIZEPALETTE );

   // For non-palette devices, we'll use the # of system
   //  colors for our palette size.
   if ( !nColors )
      nColors = GetDeviceCaps( hDC, NUMCOLORS );

   return( nColors );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   BitmapToDIB
//
// Purpose:    Given a device dependent bitmap and a palette, returns
//             a handle to global memory with a DIB spec in it.  The
//             DIB is rendered using the colors of the palette passed in.
//
//             Stolen almost verbatim from ShowDIB.
//
// Parms:      hBitmap == Handle to device dependent bitmap compatible
//                        with default screen display device.
//             hPal    == Palette to render the DDB with.  If it's 0,
//                        use the default palette.
//
// History:   Date      Reason
//            06/01/91  Created
//
/////////////////////////////////////////////////////////////////////////////
HANDLE
BitmapToDIB( HBITMAP hBitmap,
             HPALETTE hPal )
{
   BITMAP             Bitmap;
   BITMAPINFOHEADER   bmInfoHdr;
   LPBITMAPINFOHEADER lpbmInfoHdr;
   zPCHAR             lpBits;
   HDC                hMemDC;
   HANDLE             hDib;
   HPALETTE           hOldPal = 0;

   // Do some setup -- make sure the Bitmap passed in is valid,
   //  get info on the bitmap (like its height, width, etc.),
   //  then setup a BITMAPINFOHEADER.

   if ( hBitmap == 0 )
      return( 0 );

   if ( !GetObject( hBitmap, sizeof( Bitmap ), (zPCHAR) &Bitmap ) )
      return( 0 );

   InitBitmapInfoHeader( &bmInfoHdr, Bitmap.bmWidth, Bitmap.bmHeight,
                         Bitmap.bmPlanes * Bitmap.bmBitsPixel );

   // Now allocate memory for the DIB.  Then, set the BITMAPINFOHEADER
   // into this memory, and find out where the bitmap bits go.
   hDib = GlobalAlloc( GHND, sizeof( BITMAPINFOHEADER ) +
                       ::PaletteSize( &bmInfoHdr ) +
                       bmInfoHdr.biSizeImage );

   if ( hDib == 0 )
      return( 0 );

   lpbmInfoHdr  = (LPBITMAPINFOHEADER) GlobalLock( hDib );
   *lpbmInfoHdr = bmInfoHdr;
   lpBits       = FindDIBBits( (zPCHAR) lpbmInfoHdr );

   // Now, we need a DC to hold our bitmap.  If the app passed us
   // a palette, it should be selected into the DC.

   hMemDC = GetDC( 0 );

   if ( hPal )
   {
      hOldPal = SelectPalette( hMemDC, hPal, FALSE );
      RealizePalette( hMemDC );
   }

   // We're finally ready to get the DIB.  Call the driver and let
   // it party on our bitmap.  It will fill in the color table,
   // and bitmap bits of our global memory block.

   if ( GetDIBits( hMemDC, hBitmap, 0, Bitmap.bmHeight, lpBits,
                   (LPBITMAPINFO) lpbmInfoHdr, DIB_RGB_COLORS ) == 0 )
   {
      GlobalUnlock( hDib );
      GlobalFree( hDib );
      hDib = 0;
   }
   else
      GlobalUnlock( hDib );

   // Finally, clean up and return.
   if ( hOldPal )
      SelectPalette( hMemDC, hOldPal, FALSE );

   ReleaseDC( 0, hMemDC );
   return( hDib );
}

/////////////////////////////////////////////////////////////////////////////
//
// CopyScreenToBitmap( )
//
// Parameter:
//
// LPRECT lpRect    - specifies the window
//
// Return Value:
//
// HANDLE             - identifies the device-dependent bitmap
//
// Description:
//
// This function copies the specified part of the screen to a device-
// dependent bitmap.
//
/////////////////////////////////////////////////////////////////////////////
HBITMAP
CopyScreenToBitmap( LPRECT lpRect )
{
   HDC     hScrDC;               // screen DC
   HDC     hMemDC;               // memory DC
   HBITMAP hBitmap, hOldBitmap;  // handles to device-dependent bitmaps
   zLONG   lX, lY, lX2, lY2;     // coordinates of rectangle to grab
   zLONG   lWidth, lHeight;      // DIB width and height
   zLONG   lSrcX, lSrcY;         // screen resolution

   // Check for an empty rectangle
   if ( IsRectEmpty( lpRect ) )
     return( 0 );

   // Create a DC for the screen and create a memory DC compatible
   // to screen DC
   hScrDC = CreateDC( "DISPLAY", 0, 0, 0 );
   hMemDC = CreateCompatibleDC( hScrDC );

   // Get points of rectangle to grab
   lX = lpRect->left;
   lY = lpRect->top;
   lX2 = lpRect->right;
   lY2 = lpRect->bottom;

   // Get screen resolution
   lSrcX = GetDeviceCaps( hScrDC, HORZRES );
   lSrcY = GetDeviceCaps( hScrDC, VERTRES );

   // Make sure bitmap rectangle is visible
   if ( lX < 0 )
      lX = 0;

   if ( lY < 0 )
      lY = 0;

   if ( lX2 > lSrcX )
      lX2 = lSrcX;

   if ( lY2 > lSrcY )
      lY2 = lSrcY;

   lWidth = lX2 - lX;
   lHeight = lY2 - lY;

   // Create a bitmap compatible with the screen DC
   hBitmap = CreateCompatibleBitmap( hScrDC, lWidth, lHeight );

   // Select new bitmap into memory DC
   hOldBitmap = (HBITMAP) SelectObject( hMemDC, (HGDIOBJ) hBitmap );

   // BitBlt screen DC to memory DC
   BitBlt( hMemDC, 0, 0, lWidth, lHeight, hScrDC, lX, lY, SRCCOPY );

   // Select old bitmap back into memory DC and get handle to bitmap of
   // the screen.
   hBitmap = (HBITMAP) SelectObject( hMemDC, (HGDIOBJ) hOldBitmap );

   // Clean up
   DeleteDC( hScrDC );
   DeleteDC( hMemDC );

   // Return handle to the bitmap
   return( hBitmap );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:   GetSystemPalette
//
// Purpose:    This routine returns a handle to a palette which represents
//             the system palette (each entry is an offset into the system
//             palette instead of an RGB with a flag of PC_EXPLICIT).
//             Useful for dumping the system palette.
//
// Parms:      None
//
// Returns:    Handle to a palette consisting of the system palette
//             colors.
//
// History:   Date      Reason
//            06/01/91  Created
//
/////////////////////////////////////////////////////////////////////////////
HPALETTE
GetSystemPalette( )
{
   HDC           hDC;
   HPALETTE      hPal = 0;
   HANDLE        hLogPal;
   LPLOGPALETTE  lpLogPal;
   zSHORT        k, nColors;


   // Find out how many palette entries we want.
   hDC = GetDC( 0 );
   if ( hDC == 0 )
      return( 0 );

   nColors = PalEntriesOnDevice( hDC );
   ReleaseDC( 0, hDC );

   // Allocate room for the palette and lock it.
   hLogPal = GlobalAlloc( GHND, sizeof( LOGPALETTE ) + nColors * sizeof( PALETTEENTRY ) );

   if ( hLogPal == 0 )
      return( 0 );

   lpLogPal = (LPLOGPALETTE) GlobalLock( hLogPal );

   lpLogPal->palVersion    = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   for ( k = 0;  k < nColors;  k++ )
   {
      lpLogPal->palPalEntry[k].peBlue  = 0;
      *((zPUSHORT) (&lpLogPal->palPalEntry[k].peRed)) = k;
      lpLogPal->palPalEntry[k].peFlags = PC_EXPLICIT;
   }

   // Go ahead and create the palette.  Once it's created,
   // we no longer need the LOGPALETTE, so free it.
   hPal = CreatePalette( lpLogPal );
   GlobalUnlock( hLogPal );
   GlobalFree( hLogPal );

   return( hPal );
}

/////////////////////////////////////////////////////////////////////////////
//
// CopyScreenToDIB( )
//
// Parameter:
//
// LPRECT lpRect    - specifies the window
//
// Return Value:
//
// HDIB             - identifies the device-independent bitmap
//
// Description:
//
// This function copies the specified part of the screen to a device-
// independent bitmap.
//
/////////////////////////////////////////////////////////////////////////////
HDIB
CopyScreenToDIB( LPRECT lpRect )
{
  HBITMAP  hBitmap;   // handle to device-dependent bitmap
  HPALETTE hPalette;  // handle to palette
  HDIB     hDib = 0;  // handle to DIB

  // Get the device-dependent bitmap in lpRect by calling
  // CopyScreenToBitmap and passing it the rectangle to grab.
  hBitmap = CopyScreenToBitmap( lpRect );

  // Check for a valid bitmap handle
  if ( hBitmap == 0 )
    return( 0 );

  // Get the current palette
  hPalette = GetSystemPalette( );

  // Convert the bitmap to a DIB
  hDib = BitmapToDIB( hBitmap, hPalette );

  // Clean up
  DeleteObject( hBitmap );
  DeleteObject( hPalette );

  // Return handle to the packed-DIB
  return( hDib );
}

/////////////////////////////////////////////////////////////////////////////
//
// CopyWindowToDIB( )
//
// Parameters:
//
// HWND hWnd        - specifies the window
//
// Return Value:
//
// HDIB             - identifies the device-independent bitmap
//
// Description:
//
// This function copies the entire window to a device-independent bitmap.
//
/////////////////////////////////////////////////////////////////////////////
HDIB
CopyWindowToDIB( HWND hWnd )
{
   HDIB hDib = 0;  // handle to DIB
   RECT rect;

   // Check for a valid window handle
   if ( mIs_hWnd( hWnd ) == 0 )
      return( 0 );

   // Get the window rectangle
   GetWindowRect( hWnd, &rect );

   // Get the DIB of the window by calling CopyScreenToDIB and passing
   // it the window rect
   hDib = CopyScreenToDIB( &rect );

   // Return the handle to the DIB
   return( hDib );
}

//./ ADD NAME=PrintWnd
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PrintWnd
//                                              05/20/93 Modified: 09/07/98
//
//  PURPOSE:   To print the window image to a printer or bitmap or both.
//
//  PARAMETERS: vSubtask  - The subtask view for the window to be printed
//              cpcTitle  - Title with which to print the window
//
//  RETURNS:     0  - print was successful
//              -1  - Error locating window
//              -2  - printing error (call ShowPrintError( ) to display
//                    all the errors)
//
// void Print( CDC *pDC, DWORD dwFlags ) const;
// Parameters:
//    pDC - A pointer to a device context.
//    dwFlags - Specifies the drawing options.  This parameter can
//       be one or more of these flags:
//        PRF_CHECKVISIBLE - Draw the window only if it is visible.
//        PRF_CHILDREN - Draw all visible children windows.
//        PRF_CLIENT - Draw the client area of the window.
//        PRF_ERASEBKGND - Erase the background before drawing
//                          the window.
//        PRF_NONCLIENT - Draw the nonclient area of the window.
//        PRF_OWNED - Draw all owned windows.
// Remarks:
//    Call this member function to draw the current window in the
//    specified device context, which is most commonly in a printer
//    device context.
//
//    CWnd::DefWindowProc function processes this message based on which
//    drawing option is specified:
//     If PRF_CHECKVISIBLE is specified and the window is not visible,
//      do nothing.
//     If PRF_NONCLIENT is specified, draw the nonclient area in the
//      given device context.
//     If PRF_ERASEBKGND is specified, send the window a WM_ERASEBKGND
//      message.
//     If PRF_PRINTCLIENT is specified, send the window a
//      WM_PRINTCLIENT message.
//     If PRF_PRINTCHILDREN is set, send each visible child window a
//      WM_PRINT message.
//     If PRF_OWNED is set, send each visible owned window a WM_PRINT
//      message.
//
// Capturing Screen Images:
//
// Use the PRINT SCREEN key to capture an image of the entire screen.
// This image is placed in the Clipboard.  If all you want is the
// active window, use the ALT+PRINT SCREEN combination.  Simply paste
// this into your favorite bitmap editor (such as Microsoft Paintbrush),
// and trim the image you want.
//
// Using ALT+PRINT SCREEN to capture dropped menus may be a little
// tricky because the menu manager uses the ALT key.  But if you
// follow these simple steps, you can do it easily:
//
//    1. Make the program active.
//    2. Hold down the ALT key.
//    3. Press the keyboard accelerator key for the menu desired
//       (without releasing ALT).
//    4. Move the selection with the arrow keys (keeping ALT down).
//    5. Press the PRINT SCREEN key.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
PrintWnd( zVIEW   vSubtask,
          zCPCHAR cpcTitle )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pZFWnd && mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
      {
         // Get a dc to the default printer
         CDC printDC;
         CPrintDialog dlg( FALSE );
         dlg.GetDefaults( );
         if ( printDC.Attach( dlg.GetPrinterDC( ) ) )
         {
            CWaitCursor wait;

            HDIB hDib = CopyWindowToDIB( *(pZSubtask->m_pZFWnd) );
            if ( hDib )
            {
               LPSTR lpDIB = (LPSTR) ::GlobalLock( (HGLOBAL) hDib );
               int cxDIB = (int) ::DIBWidth( lpDIB );   // Size of DIB - x
               int cyDIB = (int) ::DIBHeight( lpDIB );  // Size of DIB - y
               ::GlobalUnlock( (HGLOBAL) hDib );

               CRect rcDIB;
               rcDIB.top = rcDIB.left = 0;
               rcDIB.right = cxDIB;
               rcDIB.bottom = cyDIB;
               CRect rectDest;

               // get size of printer page (in pixels)
               int cxPage = printDC.GetDeviceCaps( HORZRES );
               int cyPage = printDC.GetDeviceCaps( VERTRES );

               // get printer pixels per inch
               int cxInch = printDC.GetDeviceCaps( LOGPIXELSX );
               int cyInch = printDC.GetDeviceCaps( LOGPIXELSY );

               // Best Fit case -- create a rectangle which preserves
               // the DIB's aspect ratio, and fills the page horizontally.
               //
               // The formula in the "->bottom" field below calculates the Y
               // position of the printed bitmap, based on the size of the
               // bitmap, the width of the page, and the relative size of
               // a printed pixel (cyInch / cxInch).
               rectDest.top = rectDest.left = 0;
               rectDest.bottom = (int)(((double) cyDIB * cxPage * cyInch) / ((double) cxDIB * cxInch));
               rectDest.right = cxPage;

               if ( InitPrinting( printDC, cpcTitle ) )
               {
                  printDC.StartPage( );
                  ::PaintDIB( printDC.m_hDC, &rectDest, hDib, &rcDIB, 0 );

                  zSHORT nLth = (zSHORT) zstrlen( cpcTitle );
                  CSize size = printDC.GetTextExtent( cpcTitle, nLth );
                  printDC.TextOut( 16, rectDest.bottom + size.cy + 16, cpcTitle, nLth );

                  // Signal to the driver to begin translating the drawing
                  // commands to printer output.
                  printDC.EndPage( );
                  TermPrinting( printDC );
                  return( 0 );
               }
            }

            return( -4 );
         }
      }

      return( -2 );
   }

   return( -1 );
}

   //////////////////////////////////////////////////////////////////////////
   //
   // virtual BOOL CreateDC( LPCTSTR lpszDriverName,
   //                        LPCTSTR lpszDeviceName,
   //                        LPCTSTR lpszOutput,
   //                        const void* lpInitData );
   //
   // Return Value:  Nonzero if the function is successful; otherwise 0.
   //
   // Parameters:
   //    lpszDriverName - Points to a null-terminated string that specifies
   //       the filename (without extension) of the device driver (for
   //       example, "EPSON").  You can also pass a CString object for this
   //       parameter.
   //
   //    lpszDeviceName - Points to a null-terminated string that specifies
   //       the name of the specific device to be supported (for example,
   //       "EPSON FX-80").  The lpszDeviceName parameter is used if the
   //       module supports more than one device.  You can also pass a
   //       CString object for this parameter.
   //
   //    lpszOutput - Points to a null-terminated string that specifies the
   //       file or device name for the physical output medium (file or
   //       output port).  You can also pass a CString object for this
   //       parameter.
   //
   //    lpInitData - Points to a DEVMODE structure containing device-
   //       specific initialization data for the device driver.  The
   //       Windows DocumentProperties function retrieves this structure
   //       filled in for a given device.  The lpInitData parameter must be
   //       NULL if the device driver is to use the default initialization
   //       (if any) specified by the user through the Control Panel.
   //
   // Remarks:
   //    Creates a device context for the specified device.  The PRINT.H
   //    header file is required if the DEVMODE structure is used.
   //    Device names follow these conventions: an ending colon (:) is
   //    recommended, but optional.  Windows strips the terminating colon
   //    so that a device name ending with a colon is mapped to the same
   //    port as the same name without a colon.  The driver and port names
   //    must not contain leading or trailing spaces.  GDI output functions
   //    cannot be used with information contexts.
   //
   //////////////////////////////////////////////////////////////////////////
   //
   // DEVMODE Structure:
   //
   // typedef struct _devicemode
   // {
   //    TCHAR  dmDeviceName[ 32 ];
   //    WORD   dmSpecVersion;
   //    WORD   dmDriverVersion;
   //    WORD   dmSize;
   //    WORD   dmDriverExtra;
   //    DWORD  dmFields;
   //    short  dmOrientation;
   //    short  dmPaperSize;
   //    short  dmPaperLength;
   //    short  dmPaperWidth;
   //    short  dmScale;
   //    short  dmCopies;
   //    short  dmDefaultSource;
   //    short  dmPrintQuality;
   //    short  dmColor;
   //    short  dmDuplex;
   //    short  dmYResolution;
   //    short  dmTTOption;
   //    short  dmCollate;
   //    TCHAR  dmFormName[32];
   //    WORD   dmUnusedPadding;
   //    USHORT dmBitsPerPel;
   //    DWORD  dmPelsWidth;
   //    DWORD  dmPelsHeight;
   //    DWORD  dmDisplayFlags;
   //    DWORD  dmDisplayFrequency;
   // } DEVMODE;
   //
   // The DEVMODE data structure contains information about the device
   // initialization and environment of a printer.
   //
   // Members:
   // dmDeviceName - Specifies the name of the device the driver supports;
   //    for example, PCL/HP LaserJet in the case of PCL/HP LaserJetr.  This
   //    string is unique among device drivers.
   // dmSpecVersion - Specifies the version number of the initialization data
   //    specification on which the structure is based.
   // dmDriverVersion - Specifies the printer driver version number assigned
   //    by the printer driver developer.
   // dmSize - Specifies the size, in bytes, of the DEVMODE structure except
   //    the dmDriverData (device-specific) member.  If an application
   //    manipulates only the driver-independent portion of the data, it can
   //    use this member to determine the length of the structure without
   //    having to account for different versions.
   // dmDriverExtra - Contains the number of bytes of private driver-data
   //    that follow this structure. If a device driver does not use device-
   //    specific information, set this member to zero.
   // dmFields - Specifies which of the remaining members in the DEVMODE
   //    structure have been initialized.  Bit 0 (defined as DM_ORIENTATION)
   //    corresponds to dmOrientation; bit 1 (defined as DM_PAPERSIZE)
   //    specifies dmPaperSize, and so on. A printer driver supports only
   //    those members that are appropriate for the printer technology.
   // dmOrientation - Selects the orientation of the paper. This member can
   //    be either DMORIENT_PORTRAIT (1) or DMORIENT_LANDSCAPE (2).
   // dmPaperSize - Selects the size of the paper to print on. This member
   //    can be set to zero if the length and width of the paper are both
   //    set by the dmPaperLength and dmPaperWidth members. Otherwise, the
   //    dmPaperSize member can be set to one of the following predefined
   //    values:
   //       DMPAPER_LETTER - Letter, 8 1/2 by 11 inches
   //       MPAPER_LEGAL - Legal, 8 1/2 by 14 inches
   //       DMPAPER_A4 - A4 Sheet, 210 by 297 millimeters
   //       DMPAPER_CSHEET - C Sheet, 17 by 22 inches
   //       DMPAPER_DSHEET - D Sheet, 22 by 34 inches
   //       DMPAPER_ESHEET - E Sheet, 34 by 44 inches
   //       DMPAPER_LETTERSMALL - Letter Small, 8 1/2 by 11 inches
   //       DMPAPER_TABLOID - Tabloid, 11 by 17 inches
   //       DMPAPER_LEDGER - Ledger, 17 by 11 inches
   //       DMPAPER_STATEMENT - Statement, 5 1/2 by 8 1/2 inches
   //       DMPAPER_EXECUTIVE - Executive, 7 1/4 by 10 1/2 inches
   //       DMPAPER_A3 - A3 sheet, 297 by 420 millimeters
   //       DMPAPER_A4SMALL - A4 small sheet, 210 by 297 millimeters
   //       DMPAPER_A5 - A5 sheet, 148 by 210 millimeters
   //       DMPAPER_B4 - B4 sheet, 250 by 354 millimeters
   //       DMPAPER_B5 - B5 sheet, 182-by-257-millimeter paper
   //       DMPAPER_FOLIO - Folio, 8-1/2-by-13-inch paper
   //       DMPAPER_QUARTO - Quarto, 215-by-275-millimeter paper
   //       DMPAPER_10X14 - 10-by-14-inch sheet
   //       DMPAPER_11X17 - 11-by-17-inch sheet
   //       DMPAPER_NOTE - Note, 8 1/2 by 11 inches
   //       DMPAPER_ENV_9 - #9 Envelope, 3 7/8 by 8 7/8 inches
   //       DMPAPER_ENV_10 - #10 Envelope, 4 1/8 by 9 1/2 inches
   //       DMPAPER_ENV_11 - #11 Envelope, 4 1/2 by 10 3/8 inches
   //       DMPAPER_ENV_12 - #12 Envelope, 4 3/4 by 11 inches
   //       DMPAPER_ENV_14 - #14 Envelope, 5 by 11 1/2 inches
   //       DMPAPER_ENV_DL - DL Envelope, 110 by 220 millimeters
   //       DMPAPER_ENV_C5 - C5 Envelope, 162 by 229 millimeters
   //       DMPAPER_ENV_C3 - C3 Envelope, 324 by 458 millimeters
   //       DMPAPER_ENV_C4 - C4 Envelope, 229 by 324 millimeters
   //       DMPAPER_ENV_C6 - C6 Envelope, 114 by 162 millimeters
   //       DMPAPER_ENV_C65 - C65 Envelope, 114 by 229 millimeters
   //       DMPAPER_ENV_B4 - B4 Envelope, 250 by 353 millimeters
   //       DMPAPER_ENV_B5 - B5 Envelope, 176 by 250 millimeters
   //       DMPAPER_ENV_B6 - B6 Envelope, 176 by 125 millimeters
   //       DMPAPER_ENV_ITALY - Italy Envelope, 110 by 230 millimeters
   //       DMPAPER_ENV_MONARCH - Monarch Envelope, 3 7/8 by 7 1/2 inches
   //       DMPAPER_ENV_PERSONAL - 6 3/4 Envelope, 3 5/8 by 6 1/2 inches
   //       DMPAPER_FANFOLD_US - US Std Fanfold, 14 7/8 by 11 inches
   //       DMPAPER_FANFOLD_STD_GERMAN - German Std Fanfold, 8 1/2 by 12
   //          inches
   //       DMPAPER_FANFOLD_LGL_GERMAN - German Legal Fanfold, 8 1/2 by
   //          13 inches
   // dmPaperLength - Overrides the length of the paper specified by the
   //    dmPaperSize member, either for custom paper sizes or for devices
   //    such as dot-matrix printers, which can print on a page of arbitrary
   //    length.  These values, along with all other values in this structure
   //    that specify a physical length, are in tenths of a millimeter.
   // dmPaperWidth - Overrides the width of the paper specified by the
   //    dmPaperSize member.
   // dmScale - Specifies the factor by which the printed output is to be
   //    scaled.  The apparent page size is scaled from the physical page
   //    size by a factor of dmScale/100.  For example, a letter-sized page
   //    with a dmScale value of 50 would contain as much data as a page of
   //    17 by 22 inches because the output text and graphics would be half
   //    their original height and width.
   // dmCopies - Selects the number of copies printed if the device supports
   //    multiple-page copies.
   // dmDefaultSource - Reserved; must be zero.
   // dmPrintQuality - Specifies the printer resolution. There are four
   //    predefined device-independent values:
   //       DMRES_HIGH
   //       DMRES_MEDIUM
   //       DMRES_LOW
   //       DMRES_DRAFT
   //    If a positive value is given, it specifies the number of dots per
   //    inch (DPI) and is therefore device dependent.
   // dmColor - Switches between color and monochrome on color printers.
   //    Following are the possible values:
   //       DMCOLOR_COLOR
   //       DMCOLOR_MONOCHROME
   // dmDuplex - Selects duplex or double-sided printing for printers
   //    capable of duplex printing.  Following are the possible values:
   //       DMDUP_SIMPLEX
   //       DMDUP_HORIZONTAL
   //       DMDUP_VERTICAL
   // dmYResolution - Specifies the y-resolution, in dots per inch, of the
   //    printer.  If the printer initializes this member, the dmPrintQuality
   //    member specifies the x-resolution, in dots per inch, of the printer.
   // dmTTOption - Specifies how TrueTyper fonts should be printed.  This
   //    member can be one of the following values:
   // DMTT_BITMAP - Prints TrueType fonts as graphics.  This is the default
   //    action for dot-matrix printers.
   // DMTT_DOWNLOAD - Downloads TrueType fonts as soft fonts.  This is the
   //    default action for Hewlett-Packard printers that use Printer Control
   //    Language (PCL).
   // DMTT_SUBDEV - Substitute device fonts for TrueType fonts. This is the
   //    default action for PostScriptr printers.
   // dmCollate - Specifies whether collation should be used when printing
   //    multiple copies.  Using DMCOLLATE_FALSE provides faster, more
   //    efficient output, since the data is sent to a page printer just
   //    once, no matter how many copies are required.  The printer is told
   //    to simply print the page again.  This member can be be one of the
   //    following values:
   //       DMCOLLATE_TRUE - Collate when printing multiple copies.
   //       DMCOLLATE_FALSE - Do NOT collate when printing multiple copies.
   // dmFormName - Specifies the name of the form to use; for example,
   //    Letter or Legal. A complete set of names can be retrieved through
   //    the Windows EnumForms function.
   // dmUnusedPadding - Used to align the structure to a DWORD boundary.
   //    This should not be used or referenced.  Its name and usage is
   //    reserved, and can change in future releases.
   // dmBitsPerPel - Specifies in bits per pixel the color resolution of the
   //    display device. For example: 4 bits for 16 colors, 8 bits for 256
   //    colors, or 16 bits for 65,536 colors.
   // dmPelsWidth - Specifies the width, in pixels, of the visible device
   //    surface.
   // dmPelsHeight - Specifies the height, in pixels, of the visible device
   //    surface.
   // dmDisplayFlags - Specifies the device's display mode. The following
   //    are valid flags:
   // DM_GRAYSCALE - Specifies that the display is a non-color device.  If
   //    this flag is not set, color is assumed.
   // DM_INTERLACED - Specifies that the display mode is interlaced. If the
   //    flag is not set, non-interlaced is assumed.
   // dmDisplayFrequency - Specifies the frequency, in hertz (cycles per
   //    second), of the display device in a particular mode.
   //
   // Comments:  A device driver's private data will follow the dmDisplayMode
   //    member.  The number of bytes of private data is specified by the
   //    dmDriverExtra member.
   //
   //////////////////////////////////////////////////////////////////////////
   //
   // The CPrintDialog class encapsulates the services provided by the
   // Windows common dialog box for printing. Common print dialog boxes
   // provide an easy way to implement Print and Print Setup dialog boxes
   // in a manner consistent with Windows standards.
   //
   // If you wish, you can rely on the framework to handle many aspects of
   // the printing process for your application. In this case, the
   // framework automatically displays the Windows common dialog box for
   // printing. You can also have the framework handle printing for your
   // application but override the common Print dialog box with your own
   // print dialog box. For more information on using the framework to
   // handle printing tasks, see the article Printing in Programming with
   // MFC.
   //
   // If you want your application to handle printing without the
   // framework's involvement, you can use the CPrintDialog class "as is"
   // with the constructor provided, or you can derive your own dialog
   // class from CPrintDialog and write a constructor to suit your needs.
   // In either case, these dialog boxes will behave like standard MFC
   // dialog boxes because they are derived from class CCommonDialog.
   //
   // To use a CPrintDialog object, first create the object using the
   // CPrintDialog constructor. Once the dialog box has been constructed,
   // you can set or modify any values in the m_pd structure to initialize
   // the values of the dialog box's controls.  The m_pd structure is of
   // type PRINTDLG. For more information on this structure, see the Win32
   // SDK documentation.
   //
   // If you do not supply your own handles in m_pd for the hDevMode and
   // hDevNames members, be sure to call the Windows function GlobalFree
   // for these handles when you are done with the dialog box. When using
   // the framework's Print Setup implementation provided by
   // CWinApp::OnFilePrintSetup, you do not have to free these handles.
   // The handles are maintained by CWinApp and are freed in CWinApp's
   // destructor. It is only necessary to free these handles when using
   // CPrintDialog stand-alone.
   //
   // After initializing the dialog box controls, call the DoModal member
   // function to display the dialog box and allow the user to select
   // print options. DoModal returns whether the user selected the OK
   // (IDOK) or Cancel (IDCANCEL) button.
   //
   // If DoModal returns IDOK, you can use one of CPrintDialog's member
   // functions to retrieve the information input by the user.
   //
   // The CPrintDialog::GetDefaults member function is useful for
   // retrieving the current printer defaults without displaying a dialog
   // box. This member function requires no user interaction.
   //
   // To customize the dialog box, derive a class from CPrintDialog,
   // provide a custom dialog template, and add a message map to process
   // the notification messages from the extended controls. Any
   // unprocessed messages should be passed on to the base class.
   // Customizing the hook function is not required.
   //
   // To process the same message differently depending on whether the
   // dialog box is Print or Print Setup, you must derive a class for each
   // dialog box. You must also override the Windows AttachOnSetup
   // function, which handles the creation of a new dialog box when the
   // Print Setup button is selected within a Print dialog box.
   //
   //////////////////////////////////////////////////////////////////////////
   //
   // The PRINTDLG structure contains information that the PrintDlg
   // function uses to initialize the Print common dialog box. After the
   // user closes the dialog box, the system returns information about the
   // user-defined print selections in this structure.
   //
   // typedef struct tagPD
   // {
   //     DWORD     lStructSize;
   //     HWND      hwndOwner;
   //     HANDLE    hDevMode;
   //     HANDLE    hDevNames;
   //     HDC       hDC;
   //     DWORD     Flags;
   //     WORD      nFromPage;
   //     WORD      nToPage;
   //     WORD      nMinPage;
   //     WORD      nMaxPage;
   //     WORD      nCopies;
   //     HINSTANCE hInstance;
   //     DWORD     lCustData;
   //     LPPRINTHOOKPROC lpfnPrintHook;
   //     LPSETUPHOOKPROC lpfnSetupHook;
   //     LPCTSTR   lpPrintTemplateName;
   //     LPCTSTR   lpSetupTemplateName;
   //     HANDLE    hPrintTemplate;
   //     HANDLE    hSetupTemplate;
   // } PRINTDLG;
   //
   // Members:
   // lStructSize - Specifies the structure size, in bytes.
   //
   // hwndOwner - Identifies the window that owns the dialog box. This member
   //    can be any valid window handle, or it can be NULL if the dialog box
   //    has no owner.
   //
   // hDevMode - Identifies a movable global memory object that contains a
   //    DEVMODE structure. Before a call to the PrintDlg function, the
   //    structure members may contain data used to initialize the dialog
   //    controls.  When PrintDlg returns, the structure members specify
   //    the state of the dialog box controls.
   //
   //    If you use the structure to initialize the dialog box controls, you
   //    must allocate space for and create the DEVMODE structure. (You
   //    should allocate a movable block of memory.)
   //
   //    If you do not use the structure to initialize the dialog box
   //    controls, hDevMode may be NULL. In this case, PrintDlg allocates
   //    memory for the structure, initializes its members, and returns a
   //    handle that identifies it.
   //
   //    If the device driver for the specified printer does not support
   //    extended device modes, hDevMode is NULL when PrintDlg returns.
   //
   //    If the device name (specified by the dmDeviceName member of the
   //    DEVMODE structure) does not appear in the [devices] section of
   //    WIN.INI, PrintDlg returns an error.
   //
   //    Because this structure is a movable global memory object, the value
   //    of hDevMode may change during the execution of PrintDlg.
   //
   //    For a discussion of how the system resolves a possible data
   //    collision between values specified by the hDevMode and hDevNames
   //    members, see the following Remarks section.
   //
   // hDevNames - Identifies a movable global memory object that contains a
   //    DEVNAMES structure. This structure contains three strings that
   //    specify the driver name, the printer name, and the output port name.
   //    Before the call to PrintDlg, the structure members contain strings
   //    used to initialize dialog box controls. When PrintDlg returns, the
   //    structure members contain the strings typed by the user. The calling
   //    application uses these strings to create a device context or an
   //    information context.
   //
   //    If you use the structure to initialize the dialog box controls, you
   //    must allocate space for and create the DEVNAMES structure. (You
   //    should allocate a movable block of global memory.)
   //
   //    If you do not use the structure to initialize the dialog box
   //    controls, hDevNames may be NULL. In this case, PrintDlg allocates
   //    memory for the structure, initializes its members (by using the
   //    printer name specified in the DEVMODE structure), and returns a
   //    handle that identifies it. PrintDlg uses the first port name that
   //    appears in the [devices] section of WIN.INI when it initializes the
   //    members in the DEVNAMES structure. For example, the function uses
   //    “LPT1:” as the port name if the following string appears in the
   //    [devices] section:
   //
   //    PCL / HP LaserJet=HPPCL,LPT1:,LPT2:
   //
   //    If both hDevMode and hDevNames are NULL, PrintDlg initializes
   //    hDevNames using the current default printer.
   //
   //    Because this structure is a movable global memory object, the value
   //    of hDevNames may change during the execution of PrintDlg.
   //
   //    For a discussion of how the system resolves a possible data
   //    collision between values specified by hDevNames and hDevMode, see
   //    the Remarks section later in this topic.
   //
   // hDC - Identifies a device context or an information context, depending
   //    on whether the Flags member specifies the PD_RETURNDC or PC_RETURNIC
   //    flag. If neither flag is specified, the value of this member is
   //    undefined. If both flags are specified, PD_RETURNDC has priority.
   //
   // Flags - A set of bit flags that you can use to initialize the Print
   //    common dialog box. When the dialog box returns, it sets these flags
   //    to indicate the user’s input. This member can be a combination of
   //    the following flags:
   //
   //    PD_ALLPAGES -
   //    The default flag that indicates that the All radio button is
   //    initially selected. This flag is used as a placeholder to indicate
   //    that the PD_PAGENUMS and PD_SELECTION flags are not specified.
   //
   //    PD_COLLATE -
   //    Places a checkmark in the Collate check box when set on input.  When
   //    the PrintDlg function returns, this flag indicates that the user
   //    selected the Collate option and the printer driver does not support
   //    collation. In this case, the application must provide collation.  If
   //    PrintDlg sets the PD_COLLATE flag on return, the dmCollate member of
   //    the DEVMODE structure is undefined.
   //
   //    PD_DISABLEPRINTTOFILE - Disables the Print to File check box.
   //
   //    PD_ENABLEPRINTHOOK -
   //    Enables the hook procedure specified in the lpfnPrintHook member.
   //    This enables the hook procedure for the Print dialog box.
   //
   //    PD_ENABLEPRINTTEMPLATE -
   //    Indicates that the hInstance and lpPrintTemplateName members specify
   //    a dialog box template to use in place of the default template for
   //    the Print dialog box.
   //
   //    PD_ENABLEPRINTTEMPLATEHANDLE -
   //    Indicates that the hPrintTemplate member identifies a data block
   //    that contains a preloaded dialog box template. The system uses this
   //    template in place of the default template for the Print dialog box.
   //    The system ignores the lpPrintTemplateName member if this flag is
   //    specified.
   //
   //    PD_ENABLESETUPHOOK -
   //    Enables the hook procedure specified in the lpfnSetupHook member.
   //    This enables the hook procedure for the Print Setup dialog box.
   //
   //    PD_ENABLESETUPTEMPLATE -
   //    Indicates that the hInstance and lpSetupTemplateName members specify
   //    a dialog box template to use in place of the default template for
   //    the Print Setup dialog box.
   //
   //    PD_ENABLESETUPTEMPLATEHANDLE -
   //    Indicates that the hSetupTemplate member identifies a data block
   //    that contains a preloaded dialog box template. The system uses this
   //    template in place of the default template for the Print Setup dialog
   //    box. The system ignores the lpSetupTemplateName member if this flag
   //    is specified.
   //
   //    PD_HIDEPRINTTOFILE - Hides the Print to File check box.
   //
   //    PD_NONETWORKBUTTON - Hides and disables the Network button.
   //
   //    PD_NOPAGENUMS -
   //    Disables the Pages radio button and the associated edit controls.
   //
   //    PD_NOSELECTION - Disables the Selection radio button.
   //
   //    PD_NOWARNING -
   //    Prevents the warning message from being displayed when there is no
   //    default printer.
   //
   //    PD_PAGENUMS -
   //    Causes the Pages radio button to be in the selected state when the
   //    dialog box is created. When PrintDlg returns, this flag is set if
   //    the Pages radio button is in the selected state.
   //
   //    PD_PRINTSETUP -
   //    Causes the system to display the Print Setup dialog box rather than
   //    the Print dialog box.
   //
   //    PD_PRINTTOFILE -
   //    Causes the Print to File check box to be checked when the dialog box
   //    is created.
   //
   //    When PrintDlg returns, this flag is set if the check box is
   //    checked.  In this case, the offset indicated by the wOutputOffset
   //    member of the DEVNAMES structure contains the string “FILE:”. When
   //    you call the StartDoc function to start the printing operation,
   //    specify this “FILE:” string in the lpszOutput member of the DOCINFO
   //    structure.  Specifying this string causes the print subsystem to
   //    query the user for the name of the output file.
   //
   //    PD_RETURNDC -
   //    Causes PrintDlg to return a device context matching the selections
   //    the user made in the dialog box. The device context is returned in
   //    hDC.
   //
   //    PD_RETURNDEFAULT -
   //    The PrintDlg function does not display the dialog box. Instead, it
   //    sets the hDevNames and hDevMode members to handles to DEVMODE and
   //    DEVNAMES structures that are initialized for the system default
   //    printer.  Both hDevNames or hDevMode must be NULL, or PrintDlg
   //    returns an error. If the system default printer is supported by an
   //    old printer driver (earlier than Windows version 3.0), only
   //    hDevNames is returned; hDevMode is NULL.
   //
   //    PD_RETURNIC -
   //    Similar to the PD_RETURNDC flag, except that this flag returns an
   //    information context rather than a device context. If neither
   //    PD_RETURNDC nor PD_RETURNIC is specified, hDC is undefined on
   //    output.
   //
   //    PD_SELECTION -
   //    Causes the Selection radio button to be in the selected state when
   //    the dialog box is created. When PrintDlg returns, this flag is
   //    specified if the Selection radio button is selected. If neither
   //    PD_PAGENUMS nor PD_SELECTION is set, the All radio button is
   //    selected.
   //
   //    PD_SHOWHELP -
   //    Causes the dialog box to display the Help button. The hwndOwner
   //    member must specify the window to receive the HELPMSGSTRING
   //    registered messages that the dialog box sends when the user clicks
   //    the Help button.
   //
   //    PD_USEDEVMODECOPIES -
   //    Same as PD_USEDEVMODECOPIESANDCOLLATE
   //
   //    PD_USEDEVMODECOPIESANDCOLLATE -
   //    Disables the Copies edit control if the printer driver does not
   //    support multiple copies, and disables the Collate checkbox if the
   //    printer driver does not support collation. If this flag is not
   //    specified, PrintDlg stores the user selections for the Copies and
   //    Collate options in the dmCopies and dmCollate members of the DEVMODE
   //    structure.
   //
   //    If this flag isn't set, the copies and collate information is
   //    returned in the DEVMODE structure if the driver supports multiple
   //    copies and collation. If the driver doesn’t support multiple copies
   //    and collation, the information is returned in the PRINTDLG
   //    structure.  This means that an application only has to look at
   //    nCopies and PD_COLLATE to determine how many copies it needs to
   //    render and whether it needs to print them collated.
   //
   // nFromPage - Specifies the initial value for the starting page edit
   // control.  When the PrintDlg function returns, nFromPage specifies the
   // starting page specified by the user. This value is valid only if the
   // PD_PAGENUMS flag is specified.
   //
   // nToPage - Specifies the initial value for the ending page edit control. When
   // the PrintDlg function returns, nToPage specifies the ending page
   // specified by the user. This value is valid only if the PD_PAGENUMS
   // flag is specified.
   //
   // nMinPage - Specifies the minimum value for the range of pages specified
   // in the From and To page edit controls.
   //
   // nMaxPage - Specifies the maximum value for the range of pages specified
   // in the From and To page edit controls.
   //
   // nCopies - Contains the initial number of copies for the Copies edit
   // control if hDevMode is NULL; otherwise, the dmCopies member of the
   // DEVMODE structure contains the initial value. When PrintDlg returns,
   // this member contains the actual number of copies to print.  If the
   // printer driver does not support multiple copies, this value may be
   // greater than one and the application must print all requested
   // copies. If the PD_USEDEVMODECOPIESANDCOLLATE value is set in the
   // Flags member, nCopies is always set to 1 on return and the dmCopies
   // member in the DEVMODE receives the actual number of copies to
   // print.
   //
   // hInstance - If the PD_ENABLEPRINTTEMPLATE or PD_ENABLESETUPTEMPLATE
   // flag is set in the Flags member, hInstance is the handle of the
   // application or module instance that contains the dialog box template
   // named by the lpPrintTemplateName or lpSetupTemplateName member.
   //
   // lCustData - Specifies application-defined data that the system passes
   // to the hook procedure identified by the lpfnPrintHook or lpfnSetupHook
   // member.  When the system sends the WM_INITDIALOG message to the hook
   // procedure, the message’s lParam parameter is a pointer to the
   // PRINTDLG structure specified when the dialog was created.  The hook
   // procedure can use this pointer to get the lCustData value.
   //
   // lpfnPrintHook - Pointer to a PrintHookProc hook procedure that can
   // process messages intended for the Print dialog box. This member is
   // ignored unless the PD_ENABLEPRINTHOOK flag is set in the Flags member.
   //
   // lpfnSetupHook - Pointer to a SetupHookProc hook procedure that can
   // process messages intended for the Print Setup dialog box. This member
   // is ignored unless the PD_ENABLESETUPHOOK flag is set in the Flags
   // member.
   //
   // lpPrintTemplateName - Pointer to a null-terminated string that names
   // a dialog box template resource in the module identified by the
   // hInstance member. This template is substituted for the standard Print
   // dialog box template.  This member is ignored unless the
   // PD_ENABLEPRINTTEMPLATE flag is set in the Flags member.
   //
   // lpSetupTemplateName - Pointer to a null-terminated string that names
   // a dialog box template resource in the module identified by the
   // hInstance member. This template is substituted for the standard Print
   // Setup dialog box template. This member is ignored unless the
   // PD_ENABLESETUPTEMPLATE flag is set in the Flags member.
   //
   // hPrintTemplate - If the PD_ENABLEPRINTTEMPLATEHANDLE flag is set in
   // the Flags member, hPrintTemplate is the handle of a memory object
   // containing a dialog box template. This template is substituted for
   // the standard Print dialog box template.
   //
   // hSetupTemplate - If the PD_ENABLESETUPTEMPLATEHANDLE flag is set in
   // the Flags member, hSetupTemplate is the handle of a memory object
   // containing a dialog box template. This template is substituted for
   // the standard Print Setup dialog box template.
   //
   // Remarks:
   // There might be data collision between the values specified by
   // hDevNames and hDevMode because the wDeviceOffset member of the
   // DEVNAMES structure is supposed to be identical to the dmDeviceName
   // member of the DEVMODE structure. If it is not, the system resolves
   // the data collision by using the value of wDeviceOffset.
   //
   // If the user checks the Collate check box in the common Print dialog
   // box, but the printer driver does not support collation, the Flags
   // member of the PRINTDLG structure returned by PrintDlg includes the
   // PD_COLLATE flag. In addition, the dmCollate member of the DEVMODE
   // structure identified by the hDevMode member is set to COLLATE_TRUE.
   // If the PD_COLLATE flag is set, the application must simulate
   // collation.
   //
   // If the PD_USEDEVMODECOPIESANDCOLLATE flag is set, the Collate check
   // box is grayed out if the printer driver does not support collation.
   // This ensures that the user can only select one copy, and nCopies
   // will be one. If the driver does support multiple copies, dmCopies
   // will contain the number of copies, and nCopies will be one.
   //
   // On entry to PrintDlg, the nCopies member of the PRINTDLG structure
   // specifies the number of copies if PD_USEDEVMODECOPIESANDCOLLATE is
   // not set. On exit from PrintDlg, nCopies contains the number of
   // copies if the printer driver does not support multiple copies.
   // Otherwise it is set to one. If nCopies is greater than one on exit
   // from PrintDlg, the application must print multiple copies of the
   // document.
   //
   //////////////////////////////////////////////////////////////////////////
   //
   // The Escape function allows applications to access capabilities of a
   // particular device not directly available through GDI. Escape calls
   // made by an application are translated and sent to the driver.
   //
   // int Escape( HDC    hdc,       // handle to device context
   //             int    nEscape,   // escape function
   //             int    cbInput,   // number of bytes in input structure
   //             LPCSTR lpvInData, // pointer to input structure
   //             LPVOID lpvOutData // pointer to output structure
   //           );
   //
   // Parameters:
   //    hdc - Identifies the device context.
   //
   //    nEscape - Specifies the escape function to be performed. This
   //    parameter must be one of the predefined escape values. Use
   //    the ExtEscape function if your application defines a private
   //    escape value.
   //
   //    cbInput - Specifies the number of bytes of data pointed to by
   //    the lpvInData parameter.
   //
   //    lpvInData - Points to the input structure required for the specified
   //    escape.
   //
   //    lpvOutData - Points to the structure that receives output from this
   //    escape. This parameter should be NULL if no data is returned.
   //
   // Return Values:
   //
   // If the function succeeds, the return value is greater than zero, except
   // with the QUERYESCSUPPORT printer escape, which checks for
   // implementation only. If the escape is not implemented, the return
   // value is zero.
   // If the function fails, the return value is an error.  To get extended
   // error information, call GetLastError.
   //
   // Errors:  If the function fails, the return value is one of the
   // following values:
   //  Value             Meaning
   // SP_ERROR -         General error. If SP_ERROR is returned, Escape may
   //                    set the last error code to:
   //                       ERROR_INVALID_PARAMETER
   //                       ERROR_DISK_FULL
   //                       ERROR_NOT_ENOUGH_MEMORY
   //                       ERROR_PRINT_CANCELLED
   //
   // SP_OUTOFDISK -     Not enough disk space is currently available for
   //                    spooling, and no more space will become available.
   //
   // SP_OUTOFMEMORY -   Not enough memory is available for spooling.
   //
   // SP_USERABORT -     The user terminated the job through Windows Print
   //                    Manager.
   //
   // Remarks:
   // The Win32 API provides six new functions that supersede some printer
   // escapes:
   //
   // Function                Description
   //
   // AbortDoc        Terminates a print job. Supersedes the ABORTDOC escape.
   //
   // EndDoc          Ends a print job. Supersedes the ENDDOC escape.
   //
   // EndPage         Ends a page. Supersedes the NEWFRAME escape.
   //                 Unlike NEWFRAME, this function is always called
   //                 after printing a page.
   //
   // SetAbortProc    Sets the abort function for a print job. Supersedes
   //                 the SETABORTPROC escape.
   //
   // StartDoc        Starts a print job. Supersedes the STARTDOC escape.
   //
   // StartPage       Prepares printer driver to receive data.
   //
   // The Win32 API provides six new indexes for the GetDeviceCaps function
   // that supersede some printer escapes:
   //
   // Index                              Description
   //
   // PHYSICALWIDTH -   For printing devices: the width of the physical page,
   //                   in device units.  For example, a printer set to print at
   //                   600 dpi on 8.5"x11" paper has a physical width value
   //                   of 5100 device units.  Note that the physical page is
   //                   almost always greater than the printable area of the
   //                   page, and never smaller.
   //
   // PHYSICALHEIGHT -  For printing devices: the height of the physical page,
   //                   in device units.  For example, a printer set to print at
   //                   600 dpi on 8.5"x11" paper has a physical height value
   //                   of 6600 device units.  Note that the physical page is
   //                   almost always greater than the printable area of the
   //                   page, and never smaller.
   //
   // PHYSICALOFFSETX - For printing devices: the distance from the left edge
   //                   of the physical page to the left edge of the printable
   //                   area, in device units.  For example, a printer set to
   //                   print at 600 dpi on 8.5"x11" paper, that cannot print
   //                   on the leftmost 0.25" of paper, has a horizontal physical
   //                   offset of 150 device units.
   //
   // PHYSICALOFFSETY - For printing devices: the distance from the top edge
   //                   of the physical page to the top edge of the printable
   //                   area, in device units. For example, a printer set to
   //                   print at 600 dpi on 8.5"x11" paper, that cannot print
   //                   on the topmost 0.5" of paper, has a vertical physical
   //                   offset of 300 device units.
   //
   // Of the 64 original printer escapes, only the QUERYESCSUPPORT and the
   // PASSTHROUGH escapes can be used by Win32-based applications.
   //
   // QUERYYESCSUPPORT - Determines whether a particular escape is
   //                    implemented by the device driver.
   //
   //////////////////////////////////////////////////////////////////////////
   //
   // CString GetDeviceName( );
   //
   // Return Value: The name of the currently selected printer.
   // Remarks: Call this function after calling DoModal to retrieve the name
   //    of the currently selected printer.
   //
   //////////////////////////////////////////////////////////////////////////
   //
   // CString GetDriverName( );
   //
   // Return Value: The name of the currently selected printer device driver.
   // Remarks: Call this function after calling DoModal to retrieve the name
   //   of the currently selected printer device driver.
   //
   //////////////////////////////////////////////////////////////////////////

#if 0
   DEVMODE DevMode;

   DevMode.dmSize = zsizeof( DEVMODE );
   DevMode.dmDriverExtra = 0;
   DevMode.dmFields = DM_ORIENTATION;
   DevMode.dmOrientation = DMORIENT_LANDSCAPE; // or ...
   DevMode.dmOrientation = DMORIENT_PORTRAIT;

   DOCINFO DocInfo;

   hdcPrinter = CreateDC( "", szPrinterName, "", &DevMode );
   if ( hdcPrinter == 0 )
       return( FALSE );

   zmemset( &DocInfo, 0, zsizeof( DOCINFO ) );
   DocInfo.cbSize = zsizeof( DOCINFO );
   DocInfo.lpszDocName = pPrtData->info.CaptionBarText;
   DocInfo.lpszOutput = NULL;
   StartDoc( hdcPrinter, &DocInfo );
   StartPage( hdcPrinter);
   fnDrawDIB( hdcPrinter, &(pPrtData->info), 0, 0,
              GetDeviceCaps( hdcPrinter, HORZRES ),
              GetDeviceCaps( hdcPrinter, VERTRES ) );

   EndPage( hdcPrinter );
   EndDoc( hdcPrinter );

   DeleteDC( hdcPrinter );
   return( TRUE );
#endif

//./ ADD NAME=PrintCtrl
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PrintCtrl
//                                              12/27/01 Modified: 12/27/01
//
//  PURPOSE:   To print the specified ctrl.
//
//  PARAMETERS: vSubtask   - The subtask view for the window in which the
//                           ctrl exists.
//              cpcTitle   - Title for the report.
//              cpcCtrlTag - Tag of the Ctrl to be printed.
//              lFlags     - Flags specific to the control.
//              nFontSize  - Font size * 10 (defaults to 80).
//              cpcFontFace - Font face name (defaults to Arial).
//
//  RETURNS:    0  - print was successful
//             -1  - Error locating window
//             otw - other printing error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
PrintCtrl( zVIEW   vSubtask,
           zCPCHAR cpcTitle,
           zCPCHAR cpcCtrlTag,
           zLONG   lFlags,
           zSHORT  nFontSize,
           zCPCHAR cpcFontFace )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      if ( nFontSize <= 0 )
         nFontSize = 80;

      return( pzma->PrintZCtrl( FALSE, cpcTitle, lFlags, nFontSize,
                                cpcFontFace && *cpcFontFace ? cpcFontFace :
                                                              "Arial" ) );
   }

   return( -1 );
}

//./ ADD NAME=PrintCtrlPage
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PrintCtrlPage
//                                              12/24/01 Modified: 12/24/01
//
//  PURPOSE:   To print the specified page of the ctrl in the specified
//             subtask.
//
//  PARAMETERS: vSubtask  - The subtask view for the window to be printed.
//              nPageNbr  - Page number to be printed.
//              nPageWidth - Width of the page (in 256ths of an inch).
//              nPageHeight - Height of the page (in 256ths of an inch).
//
//  RETURNS:    0  - print was successful
//             -1  - Error locating window
//             otw - other printing error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
PrintCtrlPage( zVIEW  vSubtask,
               zLONG  lPageNbr,
               zLONG  lPageWidth,
               zLONG  lPageHeight )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout )
   {
      if ( pZSubtask->m_pZTask->m_pZSubtaskPrint )
         vSubtask = pZSubtask->m_pZTask->m_pZSubtaskPrint->m_vDialog;

      if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask,
                             pZSubtask->m_pZPrintout->m_csReportDefName ) == 0 )
      {
         return( pzma->PrintZPage( lPageNbr, lPageWidth, lPageHeight ) );
      }
   }

   return( 0 );
}

//./ ADD NAME=PrintTextFile
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PrintTextFile
//                                              05/20/93 Modified: 09/07/98
//
//  PURPOSE:   To print a text stream or text file to a printer
//
//  PARAMETERS: vSubtask    - Subtask view for the stream/file to be printed
//              cpcTitle    - Print job title
//              cpcFileName - File name (or text stream) to print.
//              cpcFontFaceName - Font override
//              lFontPointSize  - Font override
//              lFlag       - 0 : Printing text stream
//                          - 1 : Printing text file
//
//  RETURNS:     0  - print was successful
//              -1  - Error locating file
//              -2  - Error reading file
//              -3  - Error locating window
//              -4  - printing error (call ShowPrintError( ) to display
//                    all the errors)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
PrintTextFile( zVIEW   vSubtask,
               zCPCHAR cpcTitle,
               zCPCHAR cpcFileName,
               zCPCHAR cpcFontFaceName,
               zLONG   lFontPointSize,
               zLONG   lFlag )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zPCHAR   pchPrintBuffer;
      zULONG   ulBufferLth;
      zLONG    hFile = -1;

      if ( lFlag & 0x00000001 )  // printing Text File
      {
         if ( (hFile = SysOpenFile( vSubtask, cpcFileName, COREFILE_READ )) == -1 )
            return( -1 );

         ulBufferLth = SysGetFileSize( hFile );
         pchPrintBuffer = new char[ ulBufferLth + 1 ];
         if ( SysReadFile( vSubtask, hFile,
                           pchPrintBuffer, ulBufferLth ) != ulBufferLth )
         {
            delete [] pchPrintBuffer;
            return( -2 );
         }

         SysCloseFile( vSubtask, hFile, 0 );
      }
      else
      {
         ulBufferLth = zstrlen( cpcFileName );
         pchPrintBuffer = new char[ ulBufferLth + 1 ];
         zmemcpy( pchPrintBuffer, cpcFileName, ulBufferLth );
      }

      pchPrintBuffer[ ulBufferLth ] = 0;  // ensure final null terminator

      // Get a DC to the default printer.
      CDC printDC;
      CPrintDialog dlg( FALSE );
      dlg.GetDefaults( );
      if ( printDC.Attach( dlg.GetPrinterDC( ) ) )
      {
         CWaitCursor wait;
         CRect rectDest;
         CFont *pFont = 0;
         CFont *pFontOld = 0;
         zPCHAR pchText = pchPrintBuffer;
         zPCHAR pchNewLine;
         zPCHAR pchNewPage;
         int nOldMapMode = printDC.SetMapMode( MM_TEXT );

         if ( cpcFontFaceName[ 0 ] && lFontPointSize )
         {
            pFont = new CFont;  // font override
            if ( pFont->CreatePointFont( lFontPointSize,
                                         cpcFontFaceName, &printDC ) == 0 )
            {
               mDeleteInit( pFont );
            }
         }

         if ( pFont )
            pFontOld = printDC.SelectObject( pFont );

         CSize size = printDC.GetTextExtent( "JjQqWwYy", 8 );
         ::GetTextExtentPoint32( printDC, "JjQqWwYy", 8, &size );

         // Get size of printer page (in pixels).
         int cxPage = printDC.GetDeviceCaps( HORZRES );
         int cyPage = printDC.GetDeviceCaps( VERTRES );

         // Get printer pixels per inch.
      // int cxInch = printDC.GetDeviceCaps( LOGPIXELSX );
      // int cyInch = printDC.GetDeviceCaps( LOGPIXELSY );

         rectDest.left = 0;
         rectDest.right = cxPage;

         if ( InitPrinting( printDC, cpcTitle ) )
         {
            // Here we go doing our work ...
            do
            {
               printDC.StartPage( );

               rectDest.top = 2 * size.cy;
               rectDest.bottom = 3 * size.cy;
               pchNewPage = 0;

               while ( pchText && rectDest.bottom < cyPage )
               {
                  if ( pchText[ 0 ] == '' )
                  {
                     pchText++;
                     break;  // out of inner loop
                  }

                  // Remove CRLF.
                  pchNewLine = zstrchr( pchText, '\n' );
                  if ( pchNewLine )
                  {
                     *pchNewLine = 0;
                     pchNewLine++;
                     if ( *(pchNewLine - 2) == '\r' )
                        *(pchNewLine - 2) = 0;

                     if ( *pchNewLine == '\r' )
                     {
                        *pchNewLine = 0;
                        pchNewLine++;
                     }
                  }

                  if ( (pchNewPage = zstrchr( pchText, '' )) != 0 )
                  {
                     *pchNewPage = 0;
                  }

                  printDC.DrawText( pchText, zstrlen( pchText ), rectDest,
                                    DT_LEFT | DT_WORDBREAK |
                                      DT_NOPREFIX | DT_EXPANDTABS |
                                      DT_SINGLELINE | DT_NOCLIP );

                  rectDest.top = rectDest.bottom;
                  rectDest.bottom += size.cy;
                  pchText = pchNewLine;
                  if ( pchNewPage )
                     break;  // out of inner loop
               }

               // Signal the driver to begin translating the drawing commands
               // to printer output.
               printDC.EndPage( );

            } while ( pchText );

            // Done with the work ... let's clean up!

            TermPrinting( printDC );
            if ( pFontOld )
               printDC.SelectObject( pFontOld );

            printDC.SetMapMode( nOldMapMode );
            delete [] pchPrintBuffer;
            return( 0 );
         }

         if ( pFontOld )
            printDC.SelectObject( pFontOld );

         printDC.SetMapMode( nOldMapMode );
      }

      delete [] pchPrintBuffer;
      return( -4 );
   }

   return( -3 );
}

//./ ADD NAME=PrintTextBlob
// Source Module=zdrdlgp.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PrintTextBlob
//                                              05/20/93 Modified: 09/07/98
//
//  PURPOSE:   To print a text stream.
//
//  PARAMETERS: vSubtask    - Subtask view for the stream to be printed
//              cpcTitle    - Print job title
//              vOI         - View to the instance containing the blob
//              cpcEntityName - Entity Name containing the blob.
//              cpcAttribName - Attribute name of the blob print.
//              cpcFontFaceName - Font override
//              lFontPointSize  - Font override
//
//  RETURNS:     0  - print was successful
//              -1  - Error locating blob atttribute
//              -2  - Error reading file
//              -3  - Error locating window
//              -4  - printing error (call ShowPrintError( ) to display
//                    all the errors)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
PrintTextBlob( zVIEW   vSubtask,
               zCPCHAR cpcTitle,
               zVIEW   vOI,
               zCPCHAR cpcEntityName,
               zCPCHAR cpcAttribName,
               zCPCHAR cpcFontFaceName,
               zLONG   lFontPointSize )
{
   zPCHAR   pchPrintBlob;
   zULONG   ulBlobLth;

   GetAttributeLength( &ulBlobLth, vOI, cpcEntityName, cpcAttribName );
   GetAddrForAttribute( &pchPrintBlob, vOI, cpcEntityName, cpcAttribName );
   return( PrintTextFile( vSubtask, cpcTitle, pchPrintBlob,
                          cpcFontFaceName, lFontPointSize, 0 ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef zREMOTE_SERVER  // only available under windows

///////////////////////////////////////////////////////////////////////////////
// ZPinnableDlg.cpp : Implementation of the ZPinnableDlg class.
//
// Copyright 1999 David Hill - dhill@PrincipiaMathematica.com.
//
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(ZPinnableDlg, CDialog)
   //{{AFX_MSG_MAP(ZPinnableDlg)
   ON_WM_CLOSE()
   ON_MESSAGE( WM_ACTIVATE, OnActivate )
   ON_WM_SYSCOMMAND()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

ZPinnableDlg::ZPinnableDlg( UINT nDlgID,
                            CWnd *pParent ) :
              CDialog( nDlgID, pParent )
{
   //{{AFX_DATA_INIT( ZPinnableDlg )
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   m_hPinned             = AfxGetApp( )->LoadIcon( IDI_PINNED );
   m_hUnpinned           = AfxGetApp( )->LoadIcon( IDI_UNPINNED );
   m_nTemplateID         = nDlgID;
   m_bPinned             = FALSE;
   m_bDeactivateAsCancel = TRUE;
   ASSERT( m_hPinned );
   ASSERT( m_hUnpinned );
}

BOOL
ZPinnableDlg::Show( BOOL bPinned )
{
   BOOL bRC = TRUE;

   // Show the dialog.
   if ( GetSafeHwnd( ) == 0 )
   {
      bRC = CDialog::Create( m_nTemplateID );
   }

   // Set the pinned state icon.
   if ( bRC )
   {
      m_bPinned = bPinned;
      UpdateIcon( );
   }

   return( bRC );
}

BOOL
ZPinnableDlg::Hide( )
{
   BOOL bRC = TRUE;

   if ( GetSafeHwnd( ) != 0 )
   {
      bRC = DestroyWindow( );
   }

   return( bRC );
}

BOOL
ZPinnableDlg::Visible( ) const
{
   return( GetSafeHwnd( ) != 0 );
}

void
ZPinnableDlg::DeactivateAsCancel( BOOL bCancel )
{
   m_bDeactivateAsCancel = bCancel;
}

void
ZPinnableDlg::OnClose( )
{
   if ( GetParent( ) )
   {
      // Inform the parent.
      GetParent( )->PostMessage( WM_PINUPCLOSE, IDCANCEL, 0 );
   }
   else
   {
      // Standard modal behavior.
      CDialog::OnCancel( );
   }
}

void
ZPinnableDlg::OnCancel( )
{
   if ( GetParent( ) )
   {
      // Inform the parent.
      GetParent( )->PostMessage( WM_PINUPCLOSE, IDCANCEL, 0 );
   }
   else
   {
      // Standard modal behavior.
      CDialog::OnCancel( );
   }
}

void
ZPinnableDlg::OnOK( )
{
   if ( GetParent( ) )
   {
      // Inform the parent.
      UpdateData( TRUE );
      GetParent( )->PostMessage( WM_PINUPCLOSE, IDOK, 0 );
   }
   else
   {
      // Standard modal behavior.
      CDialog::OnOK( );
   }
}

LRESULT
ZPinnableDlg::OnActivate( WPARAM wParam, LPARAM lParam )
{
   lParam;     // Unused.
   WORD fActive = LOWORD( wParam );
   CWnd *pParentWnd;

   if ( !m_bPinned && fActive == WA_INACTIVE &&
        (pParentWnd = GetParent( )) != 0 )
   {
      // Inform the parent.
      UpdateData( TRUE );
      pParentWnd->PostMessage( WM_PINUPCLOSE,
                               m_bDeactivateAsCancel ? IDCANCEL : IDOK, 0 );
   }

   return( 0L );
}

BOOL
ZPinnableDlg::OnInitDialog( )
{
   CDialog::OnInitDialog( );
   UpdateIcon( );

   return( TRUE );  // Return TRUE unless you set the focus to a control
                    // EXCEPTION: OCX Property Pages should return FALSE
}

void
ZPinnableDlg::UpdateIcon( )
{
   if ( m_bPinned )
   {
      SetIcon( m_hPinned, TRUE );
      SetIcon( m_hPinned, FALSE );
   }
   else
   {
      SetIcon( m_hUnpinned, TRUE );
      SetIcon( m_hUnpinned, FALSE );
   }
}

void
ZPinnableDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
   // Prevent the system menu from being shown and the dialog
   // from closing when it's double clicked.
   UINT nCommand = nID & 0xFFF0;
   if ( nCommand == SC_MOUSEMENU || nCommand == SC_DEFAULT )
   {
      // Toggle the pinned state.
      m_bPinned = !m_bPinned;
      UpdateIcon( );
   }
   else
   {
      CDialog::OnSysCommand( nID, lParam );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif // #ifndef zREMOTE_SERVER  // only available under windows
