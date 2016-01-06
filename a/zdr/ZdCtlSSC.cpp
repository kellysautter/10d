/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlssc.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon SpreadSheet control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.09.22    DKS    Z10    QS999
//    Fixed text color problem and started memory leak analyis.
//
// 1999.03.10    DKS
//    Initialized uninitialized variables.
//
// 1999.01.18    DKS
//    Changed background color for ComboBox (Helmut's request).
//
// 1998.10.30    DKS
//    Changed background color.  Fixed edit leave/enter cell processing.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlSS.h"
#include "ZdCtlGbl.h"

#ifndef _NOANCHOR

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SpreadSheet globals
//
mGLOBAL_DECLARE( zPCHAR, szlSpinClassName, VA_SPINBTN_CLASS_NAME );
mGLOBAL_DECLARE( zPCHAR, szlCalendarClassName, VA_CALENDAR_CLASS_NAME );

zEXTERN ZListFile clfHead
#ifdef zGLOBAL_DATA
   ( "", "" );
#else
   ;
#endif

zEXTERN SS_CLASSNAME SSClassNames[ 1 ]
#ifdef zGLOBAL_DATA
 = {
     VA_GENERAL_CLASS_NAME,
     "SSwEdit",
     "SSwEditHScroll",
     "SSwEditComboBox",
     VA_PIC_CLASS_NAME,
     "SSwPic",
     VA_DATE_CLASS_NAME,
     "SSwDate",
     VA_TIME_CLASS_NAME,
     "SSwTime",
     VA_INTEGER_CLASS_NAME,
     "SSwInteger",
     VA_FLOAT_CLASS_NAME,
     "SSwFloat",
     "SSwtbSuperBtn",
     "SSwSuperBtn",
     "SSwtbComboBox",
     "SSwComboBox",
     "SSwListBox",
     VA_VIEWPICT_CLASS_NAME,
     "SSwtbCheckBox",
     "SSwCheckBox",
     "tbScrlBar",
   };
#else
   ;
#endif

mGLOBAL_DECLARE( zPCHAR, g_hColorTable, 0 );
mGLOBAL_DECLARE( zSHORT, g_nColorTableCnt, 0 );
mGLOBAL_DECLARE( zSHORT, g_nColorTableAllocCnt, 0 );
mGLOBAL_DECLARE( zPCHAR, g_hFontTable, 0 );
mGLOBAL_DECLARE( zSHORT, g_nFontTableCnt, 0 );
mGLOBAL_DECLARE( zLONG,  g_lFontTableAllocCnt, 0 );
mGLOBAL_DECLARE( zSHORT, g_nSpreadInstCnt, 0 );
mGLOBAL_DECLARE( zPCHAR, g_hCheckBoxDefPicts, 0 );
mGLOBAL_DECLARE( zSHORT, g_nCheckBoxDefPictCnt, 0 );
mGLOBAL_DECLARE( zSHORT, g_nCheckBoxDefPictLoadCnt, 0 );

//zEXTERN SSX_CALCFUNCTION SS_CalcFunctions[];

mGLOBAL_DECLARE( WNDPROC, g_lpfnDeskTopProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnSSDeskTopProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnEditProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnEditHScrollProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnPicProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnDateProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnTimeProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnIntegerProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnFloatProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnViewPictProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnButtonProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnCheckBoxProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnComboBoxProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnEditComboBoxProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnListBoxProc, 0 );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL  bShiftDown;         // The shift key is currently held down.
zSHORT nHiliteAnchor;      // Start, or anchor, position for highlighting
zSHORT nPrevious;          // Previous caret position for highlighting
zBOOL  bFirstMove;         // True if this is the first move of a hilighting
                           //    operation in an OVERSTRIKE field

zBOOL  bLocalOverstrike;   // True if this field is in OVERSTRIKE mode.
zBOOL  bGlobalOverstrike;  // True if the screen is in OVERSTRIKE mode.
                           //  (This provides the default if mode is not
                           //      specified for a particular field).

#define LEFT  1
#define RIGHT 2

#define GLOBAL   TRUE
#define LOCAL    FALSE

#define UPPERCASE 1
#define LOWERCASE 2


#define MAXFIELD 160
#define N_PRINTABLE_CHARS 256

#define BTNTEXTPICT_INC 6

#define EDIT3D_OUT          0x01  // Project the 3D out
#define EDIT3D_INSIDEBORDER 0x02  // Add a 3D inside border

#define EDIT_FIELD_CODES (DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_HASSETSEL)

#define EM_EDIT_ADJUSTLENGTH       (EM_TOOLBOXUSERMAX + 1)
#define EM_EDIT_POSITIONAFTERPAINT (EM_TOOLBOXUSERMAX + 2)
#define EM_RESETPREVLEN            (EM_TOOLBOXUSERMAX + 3)
#define EM_REPLACETEXT             (EM_TOOLBOXUSERMAX + 4)

#define CARET_BITMAP_WIDTH   8
#define CARET_BITMAP_HEIGHT  4
#define CARET_HALF_WIDTH     (CARET_BITMAP_WIDTH / 2)

#define GetDefWndProc( hWnd ) ((FARPROC) GetWindowLong( (hWnd), zGWL_EDITFLD ))

static BITMAP Bitmap = {0, CARET_BITMAP_WIDTH, CARET_BITMAP_HEIGHT, 2, 1, 1};

static BYTE    byBits[] =
{
   0x18, 0x00,       //    XX
   0x3C, 0x00,       //   XXXX
   0x7E, 0x00,       //  XXXXXX
   0xFF, 0x00        // XXXXXXXX
};

typedef struct tagGenField
{
   HWND  hParent;          // Handle of the parent window
} GENFIELD;
typedef GENFIELD *LPGENFIELD;

// Edit controls

typedef struct tagEDIT3DFORMAT
{
   WORD     wStyle;               // 3D Styles
   zSHORT   nShadowSize;          // The size of the shadow in pixels
   COLORREF  ColorShadow;         // The color of the dark portion
   COLORREF  ColorHilite;         // The color the light portion
   COLORREF  ColorInsideShadow;   // The color of the inside dark portion
   COLORREF  ColorInsideHilite; // The color the inside light portion
} EDIT3DFORMAT, *LPEDIT3DFORMAT;


typedef struct tagEditField
{
   zCHAR        szString[ MAXFIELD + 1 ];   // The contents of the field
   BYTE         nCharWidthTable[ N_PRINTABLE_CHARS - ' ' + 1 ]; // Table of
                                 //    widths for this font
   zLONG        lHeightY;        // Height (in pixels) of this font
   zLONG        tmOverhang;      // Overhang (in pixels) of this font
   zSHORT       nRightLimitX;    // Right limit (in pixels) of the field rectangle
   zSHORT       nChars;          // Number of characters in the field
   zSHORT       nCurrentPos;     // Number of chars to the left of the caret
                                 //    0 <= nCurrentPos <= nChars
   zBOOL        bModified;       // The user has modified the text
   zBOOL        bHilited;        // True if the field is currently highlighted
   zSHORT       nHiStart;        // Starting position of the current highlight
   zSHORT       nHiEnd;          // Ending position of the current highlight
   zBOOL        bOverstrike;     // True if Overstrike mode is selected
   zBOOL        bRightJustify;   // True if the field to be right justified
   zBOOL        bMouseDown;      // True if the left mouse button is down
   zCHAR        nCase;           // Indicates whether to force upper or lower case
   zCHAR        fPassWord;       // Show text as a password
   HBITMAP      hBitmapCaret;
   HFONT        hFont;           // 0 if system font is used
   zSHORT       nMaxChars;       // Maximum number of characters to be entered
   zBOOL        bCharBeingInserted;
   zBOOL        f3D;
   EDIT3DFORMAT Edit3DFormat;
   HWND         hWnd;
   zBOOL        fSuspendNotify;
} EDITFIELD, *LPEDITFIELD;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
AdjustLength( HWND hWnd );
void
EraseField( HWND hWnd, WPARAM  wParam );
LPEDITFIELD
LockField( HWND hWnd );
void
MouseDown( HWND hWnd, LPARAM  lParam );
void
MouseMove( HWND hWnd, LPARAM  lParam );
void
MouseUp( HWND hWnd );
LPGENFIELD
LockGenField( HWND hWnd );
zLONG
NotifyParent( HWND hWnd, zSHORT nNotificationCode );
void
DeleteSelectedText( HWND hWnd, LPEDITFIELD lpField );
zBOOL
GetModify( HWND hWnd );
void
SetModify( HWND hWnd, zBOOL Modify );
zSHORT
FindNearestCharacter( LPEDITFIELD lpField, zSHORT nRawPos, zBOOL bRoundUp );
zBOOL
GenCreateField( HWND hWnd, LPARAM  lParam );
zBOOL
EditGet3DParams( HWND   hWnd,
                 LPARAM  lParam );
zBOOL
EditSet3DParams( HWND    hWnd,
                 WPARAM  wParam,
                 LPARAM  lParam );
void
PaintField( HWND hWnd, HDC hDC );
void
EditPaintField( HWND hWnd, HDC hDC, LPEDITFIELD lpField );
void
EditTextOut( HWND        hWnd,
             LPEDITFIELD lpField,
             HDC         hDC,
             HBRUSH      hBrush,
             HFONT       hFont,
             RECT        Rect,
             zSHORT      x,
             zSHORT      y,
             zPCHAR      pchText,
             zSHORT      nCnt,
             zBOOL       fRightJustify,
             zSHORT      nStart,
             zSHORT      nEnd );
void
EditSuspendNotify( HWND hWnd, zBOOL fSuspend );
void
FreeGenField( HWND hWnd );
void
GenInitializeField( HWND hWnd, LPARAM  lParam );
void
GenLimitText( HWND hWnd, WPARAM  wParam );
zSHORT
GetFieldText( HWND hWnd, WPARAM  wParam, LPARAM  lParam );
void
InsertText( HWND hWnd, LPEDITFIELD lpField, zPCHAR lpNewText );
zSHORT
RightOf( LPEDITFIELD lpField, zSHORT nChar );
zSHORT
LeftOf( LPEDITFIELD lpField, zSHORT nChar );
void
SetFieldText( HWND hWnd, LPARAM  lParam );
void
ShiftUp( );
void
ShiftDown( HWND hWnd );
void
ClearSelection( HWND hWnd );
zLONG
GetSelection( HWND hWnd );
zBOOL
ReplaceSel( HWND    hWnd,
            LPARAM  lParam );
void
SetSelection( HWND hWnd, zLONG lParam );
void
DeselectText( HWND hWnd, LPEDITFIELD lpField );
zSHORT
FieldLength( LPEDITFIELD lpField );
zSHORT
EditGetRightPos( LPEDITFIELD lpField );
zSHORT
GetSingleCharWidth( LPEDITFIELD lpField, BYTE cCharacter );
void
HiliteField( HWND hWnd, LPEDITFIELD lpField );
void
InsertCharacter( HWND hWnd, LPEDITFIELD lpField, zCHAR cChar );
zBOOL
CheckFit( LPEDITFIELD lpField, BYTE cChar );
void
ReplaceCharacter( HWND hWnd, LPEDITFIELD lpField, zCHAR cChar );
zBOOL
CaretPosition( HWND hWnd, zSHORT nChar, LPARAM  lParam );
void
SetCaretPosition( HWND hWnd, zSHORT nChar );
zSHORT
GetCaretPosition( HWND hWnd );
void
SetCaretXPos( HWND hWnd, zSHORT nPos );
zSHORT
GetCaretXPos( );
zSHORT
CaretPosOfCharacter( LPEDITFIELD lpField, zSHORT nChar );
void
CreateModeCaret( HWND hWnd, LPEDITFIELD lpField );
void
UpdateSelectedRegion( HWND hWnd, LPEDITFIELD lpField );
void
SetInsertMode( HWND hWnd, zBOOL bGlobal );
zBOOL
GetOverstrikeMode( zBOOL bGlobal );
void
SetOverstrikeMode( HWND hWnd, zBOOL bGlobal );
zBOOL
GetFontWidth( HWND hWnd, HFONT hFont );
zBOOL
CreateField( HWND hWnd, LPARAM  lParam );
void
OpenField( HWND hWnd );
void
PositionCaret( HWND hWnd, LPEDITFIELD lpField, zSHORT nChar );
void
PositionCaretAfterPaint( HWND hWnd );
void
FreeField( HWND hWnd );
void
DestroyField( HWND hWnd );
void
CloseField( HWND hWnd );
void
SizeField( HWND hWnd, LPARAM  lParam );
HFONT
ProcessEditGetFont( HWND hWnd );
HFONT
ProcessEditSetFont( HWND hWnd, WPARAM  wParam, LPARAM  lParam );
void
ClipboardPaste( HWND hWnd );
void
ClipboardCut( HWND hWnd );
void
ClipboardCopy( HWND hWnd );
zBOOL
SendTextToClipboard( HWND   hWnd,
                     zPCHAR pchText,
                     zSHORT nLth );
HANDLE
GetTextFromClipboard( HWND hWnd );
zLONG
GetTextLength( HWND hWnd );
WORD
ProcessKeyUp( HWND hWnd, WPARAM  wParam );
WORD
ProcessKeystroke( HWND hWnd, WPARAM  wParam );
zBOOL
IsShiftDown( );
void
ShiftMove( HWND hWnd );
void
EndCaret( HWND hWnd, LPEDITFIELD lpField );
void
HomeCaret( HWND hWnd, LPEDITFIELD lpField );
void
LeftCaret( HWND hWnd, LPEDITFIELD lpField );
void
RightCaret( HWND hWnd, LPEDITFIELD lpField );
zBOOL
Validate( HWND hWnd, zPCHAR pchString, zLONG lPos );
zLONG OPERATION
EditFieldWndProc( HWND     hWnd,
                  WORD     uMsg,
                  WPARAM   wParam,
                  LPARAM   lParam );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Loop from the character following the substring to the end of the
// string moving each character (including the terminator) LEFT by the
// number of characters in the substring.
//
zPCHAR OPERATION
StrRemove( zPCHAR pchString, zSHORT nStart, zSHORT nEnd )
{
   TRACE_DEBUG( "<STR.CPP>", "StrRemove" );

   if ( pchString )
   {
      // if the end comes before the start, swap them.
      if ( nStart > nEnd )
      {
         zSHORT k = nStart;
         nStart = nEnd;
         nEnd = k;
      }

      strcpy_s( pchString + nStart, nEnd - nStart, pchString + nEnd + 1 );
   }

   return( pchString );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
AdjustLength( HWND hWnd )
{
   TRACE_DEBUG( "<TEXT.CPP>", "AdjustLength " );
   LPEDITFIELD lpField;

   lpField = LockField( hWnd );
   lpField->nChars = zstrlen( lpField->szString );
// DrUnlockTaskMemory( lpField );
}

LPEDITFIELD
LockField( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "LockField" );

   return( (LPEDITFIELD) GetWindowLong( hWnd, zGWL_HFIELD ) );
}

LPGENFIELD
LockGenField( HWND hWnd )
{
   TRACE_DEBUG( "<GENMGR.CPP>", "LockGenField " );
   return( (LPGENFIELD) GetWindowLong( hWnd, zGWL_HGENFLD ) );
}

zLONG
NotifyParent( HWND hWnd, zSHORT nNotificationCode )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "NotifyParent" );
   LPEDITFIELD lpField;
   zLONG       lRet = 0;

   if ( (lpField = LockField( hWnd )) != 0 )
   {
      if ( lpField->fSuspendNotify == 0 )
      {
         SendMessage( hWnd, WM_COMMAND, GetWindowLong( hWnd, GWL_ID ),
                      MAKELONG( hWnd, nNotificationCode ) );

         lRet = SendMessage( GetParent( hWnd ), WM_COMMAND,
                             GetWindowLong( hWnd, GWL_ID ),
                             MAKELONG( hWnd, nNotificationCode ) );
      }

//    DrUnlockTaskMemory( lpField );
   }

   return( lRet );
}

zSHORT
LeftOf( LPEDITFIELD lpField, zSHORT nChar )
{
   TRACE_DEBUG( "<CHAR.CPP>", "LeftOf " );
   zSHORT   nStart;
   zSHORT   nPos;
   zSHORT   nCharIdx;

   if ( lpField->bRightJustify )
      nStart = EditGetRightPos( lpField );
   else
   {
      nStart = XMARGIN;

      if ( lpField->f3D )
      {
         nStart += lpField->Edit3DFormat.nShadowSize;

         if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
            nStart += 1;
      }
   }

   for ( nCharIdx = 0, nPos = nStart; nCharIdx < nChar; ++nCharIdx )
      nPos += GetSingleCharWidth( lpField, (BYTE) lpField->szString[ nCharIdx ] );

   return( nPos );
}

int
RightOf( LPEDITFIELD lpField, int nChar )
{
   TRACE_DEBUG( "<CHAR.CPP>", "RightOf " );
   int   nStart;
   int   nPos;
   int   nCharIdx;

   if ( lpField->bRightJustify )
      nStart = EditGetRightPos( lpField );
   else
   {
      nStart = XMARGIN;

      if ( lpField->f3D )
      {
         nStart += lpField->Edit3DFormat.nShadowSize;

         if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
            nStart += 1;
      }
   }

   for ( nCharIdx = 0, nPos = nStart; nCharIdx < nChar; ++nCharIdx )
      nPos += GetSingleCharWidth( lpField, (BYTE) lpField->szString[ nCharIdx ] );

   nPos += GetSingleCharWidth( lpField, (BYTE) lpField->szString[ nCharIdx ] );

   return( nPos );
}

zBOOL
GenCreateField( zVIEW v, HWND hWnd, LPARAM  lParam )
{
   TRACE_DEBUG( "<GENMGR.CPP>", "GenCreateField " );
   LPGENFIELD  lpGen;
   zPCHAR      hGen;
   zBOOL       fRetCode = FALSE;

   if ( (DrAllocTaskMemory( &hGen, sizeof( GENFIELD ), 1000 )) != 0 )
   {
      NotifyParent( hWnd, EN_ERRSPACE );
   }
   else
   {
      SetWindowLong( hWnd, zGWL_EDITFLD, (DWORD)(FARPROC) EditFieldWndProc );
      lpGen = (LPGENFIELD) DrLockTaskMemory( hGen );
      if ( lpGen )
      {
         SetWindowLong( hWnd, zGWL_HGENFLD, (zLONG) lpGen );

         lpGen->hParent = ((LPCREATESTRUCT) lParam)->hwndParent;
         fRetCode = TRUE;
//       DrUnlockTaskMemory( lpGen );
      }
      else
         NotifyParent( hWnd, EN_ERRSPACE );
   }

   return( fRetCode );
}

void
EditSuspendNotify( HWND hWnd, zBOOL fSuspend )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditSuspendNotify" );
   LPEDITFIELD lpField;

   if ( (lpField = LockField( hWnd )) != 0 )
   {
      lpField->fSuspendNotify = fSuspend;
//    DrUnlockTaskMemory( lpField );
   }
}

zBOOL
EditSet3DParams( HWND    hWnd,
                 WPARAM  wParam,
                 LPARAM  lParam )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditSet3DParams" );
   LPEDITFIELD    lpField;
   RECT           rectClient;
   zBOOL          fRet = TRUE;

   lpField = LockField( hWnd );
   lpField->f3D = wParam;

   if ( lParam )
      zmemcpy( &lpField->Edit3DFormat, (LPEDIT3DFORMAT) lParam, sizeof( EDIT3DFORMAT ) );

   GetClientRect( hWnd, &rectClient );
   TraceRect( "EditSet3DParams ClientRect", rectClient );
   SendMessage( hWnd, WM_SIZE, 0, MAKELONG( rectClient.right, rectClient.bottom ) );

   InvalidateRect( hWnd, 0, TRUE );
   UpdateWindow( hWnd );
// DrUnlockTaskMemory( lpField );
   return( fRet );
}

zBOOL
EditGet3DParams( HWND   hWnd,
                 LPARAM  lParam )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditGet3DParams" );
   LPEDITFIELD    lpField;
   zBOOL          fRet = FALSE;

   lpField = LockField ( hWnd );
   if ( lParam )
      zmemcpy( (zPCHAR) lParam, &lpField->Edit3DFormat, sizeof( EDIT3DFORMAT ) );

   fRet = lpField->f3D;
// DrUnlockTaskMemory( lpField );
   return( fRet );
}

void
EditTextOut( HWND        hWnd,
             LPEDITFIELD lpField,
             HDC         hDC,
             HBRUSH      hBrush,
             HFONT       hFont,
             RECT        Rect,
             zSHORT      x,
             zSHORT      y,
             zPCHAR      pchText,
             zSHORT      nCnt,
             zBOOL       fRightJustify,
             zSHORT      nStart,
             zSHORT      nEnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditTextOut" );
   HBRUSH  hBrushTemp;
   WORD    wFormat;
   RECT    RectTemp;
   RECT    RectTempHi;
   zBOOL   fIsEnabled;
   HFONT   hFontOld = 0;

   zmemcpy( &RectTemp, &Rect, sizeof( RECT ) );

   // Draw the text.
   if ( hFont )
      hFontOld = SelectFont( hDC, hFont );

   fIsEnabled = mIs_hWndEnabled( hWnd );

   if ( fIsEnabled || GetBkColor( hDC ) != GetSysColor( COLOR_GRAYTEXT ) )
   {
      COLORREF clrTextOld = CLR_INVALID;

      if ( fIsEnabled == 0 )
         clrTextOld = SetTxColor( hDC, GetSysColor( COLOR_GRAYTEXT ) );

      RectTemp.left += XMARGIN;
      RectTemp.right -= XMARGIN;
      RectTemp.top += YMARGIN;

      if ( fRightJustify )
         wFormat = DT_TOP | DT_SINGLELINE | DT_RIGHT | DT_NOPREFIX;
      else
         wFormat = DT_TOP | DT_SINGLELINE | DT_LEFT | DT_NOPREFIX;

      DrawText( hDC, pchText, nCnt, &RectTemp, wFormat );

      if ( clrTextOld != CLR_INVALID )
         SetTxColor( hDC, clrTextOld );

      // Draw Hilite.
      if ( GetFocus( ) == hWnd && lpField->bHilited )
      {
         COLORREF clrBkOld = CLR_INVALID;
         clrTextOld = CLR_INVALID;

         CopyRect( &RectTempHi, &RectTemp );

         nStart = mMax( lpField->nHiStart, 0 );
         nEnd = mMin( lpField->nHiEnd, lpField->nChars );

         RectTempHi.left =  LeftOf( lpField, nStart );
         RectTempHi.right = RightOf( lpField, nEnd - 1 );
         RectTempHi.bottom = Rect.top + lpField->lHeightY + 1;

         wFormat = DT_TOP | DT_SINGLELINE | DT_LEFT | DT_NOPREFIX;

         clrBkOld = SetBkColor( hDC, GetSysColor( COLOR_HIGHLIGHT ) );
         clrTextOld = SetTxColor( hDC, GetSysColor( COLOR_HIGHLIGHTTEXT ) );

         DrawText( hDC, &pchText[ nStart ], nEnd - nStart,
                   &RectTempHi, wFormat );

         if ( clrBkOld != CLR_INVALID )
            SetBkColor( hDC, clrTextOld );

         if ( clrTextOld != CLR_INVALID )
            SetTxColor( hDC, clrTextOld );
      }
   }
   else
   {
      hBrushTemp = kzCreateSolidBrush( GetTextColor( hDC ) );
      fnGrayString( hDC, hBrushTemp, 0, (DWORD) pchText, 0, x, y, 0, 0 );
      DeleteBrush( hBrushTemp );
   }

   if ( hFontOld )
      SelectFont( hDC, hFontOld );
}

void
FreeGenField( zVIEW v, HWND hWnd )
{
   TRACE_DEBUG( "<GENMGR.CPP>", "FreeGenField " );
   zPCHAR hGenField;

   if ( (hGenField = (zPCHAR) GetWindowLong( hWnd, zGWL_HGENFLD )) != 0 )
   {
      hGenField = (zPCHAR) DrUnlockTaskMemory( hGenField );
      DrFreeTaskMemory( hGenField );
   }
}

void
GenInitializeField( HWND hWnd, LPARAM  lParam )
{
   TRACE_DEBUG( "<GENMGR.CPP>", "GenInitializeField " );
   zCHAR       szGen[ 256 ];
   LPGENFIELD  lpGen = LockGenField( hWnd );

   if ( ((LPCREATESTRUCT) lParam)->lpszName )
      strncpy_s( szGen, sizeof( szGen ), (zPCHAR) ((LPCREATESTRUCT)lParam)->lpszName, sizeof( szGen ) - 1 );
   else
      szGen[ 0 ] = 0;

// DrUnlockTaskMemory( lpGen );

   SendMessage( hWnd, WM_SETTEXT, 0, (LPARAM) szGen );
}

void
GenLimitText( HWND hWnd, WPARAM  wParam )
{
   TRACE_DEBUG( "<GENMGR.CPP>", "GenLimitText" );
   LPEDITFIELD lpField = LockField( hWnd );
   if ( wParam == 0 )
      lpField->nMaxChars = -1;
   else
      lpField->nMaxChars = mMin( wParam, MAXFIELD );

// DrUnlockTaskMemory( lpField );
}

void
ShiftUp( )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "ShiftUp" );
   bShiftDown = FALSE;
   bFirstMove = FALSE;
}

// For future reference:
//    There is a small bug. If you start highlighting by one means,
//    via the mouse or WM_SETSEL for example, then try to extend the
//    highlighted region using shift and arrow keys, the highlighting
//    stuffs up.  Its because all the initial stuff: nHiliteAnchor, nPrevious,
//    bFirstMove, was never set.
//
void
ShiftDown( HWND hWnd )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "ShiftDown " );
   LPEDITFIELD lpField = LockField( hWnd );

   bShiftDown = TRUE;
   if ( lpField->bHilited == 0 )
   {
      nHiliteAnchor = lpField->nCurrentPos;
      lpField->nHiEnd = lpField->nHiStart = nHiliteAnchor;
      nPrevious = nHiliteAnchor;
      bFirstMove = TRUE;
   }

// DrUnlockTaskMemory( lpField );
}

zSHORT
FieldLength( LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<CHAR.CPP>", "FieldLength " );
   zSHORT k;
   zSHORT nLth;

   for ( k = 0, nLth = 0; k < lpField->nChars; k++ )
      nLth += GetSingleCharWidth( lpField, (BYTE) lpField->szString[ k ] );

   return( nLth );
}

zSHORT
EditGetRightPos( LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditGetRightPos" );
   zSHORT   nRC;

   nRC = lpField->nRightLimitX - XMARGIN - FieldLength( lpField );

   if ( lpField->f3D )
   {
      nRC -= lpField->Edit3DFormat.nShadowSize;

      if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
         nRC -= 1;
   }

   return( nRC );
}

zSHORT
GetSingleCharWidth( LPEDITFIELD lpField, BYTE cCharacter )
{
   TRACE_DEBUG( "<CHAR.CPP>", "GetSingleCharWidth " );
   if ( lpField->fPassWord && cCharacter != ' ' )
      cCharacter = '*';

   return( lpField->nCharWidthTable[ (zSHORT) cCharacter - (zSHORT) ' ' ] );
}

void
SetCaretPosition( HWND hWnd, zSHORT nChar )
{
   TRACE_DEBUG( "<CARET.CPP>", "SetCaretPosition " );
   LPEDITFIELD lpField = LockField( hWnd );
   PositionCaret( hWnd, lpField, nChar );
// DrUnlockTaskMemory( lpField );
}

zSHORT
GetCaretPosition( HWND hWnd )
{
   TRACE_DEBUG( "<CARET.CPP>", "GetCaretPosition " );
   zSHORT nPos;
   LPEDITFIELD lpField = LockField( hWnd );
   nPos = lpField->nCurrentPos;
// DrUnlockTaskMemory( lpField );

   return( nPos );
}

void
PositionCaret( HWND hWnd, LPEDITFIELD lpField, zSHORT nChar )
{
   TRACE_DEBUG( "<CARET.CPP>", "PositionCaret " );
   lpField->nCurrentPos = nChar;
   SetCaretXPos( hWnd, CaretPosOfCharacter( lpField, nChar ) );
}

void
SetCaretXPos( HWND hWnd, zSHORT nPos )
{
   TRACE_DEBUG( "<CARET.CPP>", "SetCaretXPos " );
   zSHORT      nExtraY = 0;
   zSHORT      nTemp;
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField->f3D )
   {
      nExtraY = lpField->Edit3DFormat.nShadowSize;

      if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
         nExtraY += 1;
   }

   if ( GetFocus( ) == hWnd )       // This field has the focus
   {
      if ( GetOverstrikeMode( LOCAL ) )
      {
         nTemp = nPos - CARET_HALF_WIDTH; // Use nTemp to avoid compiler bug!
         SetCaretPos( nTemp,
                      lpField->lHeightY + nExtraY + YMARGIN -
                                                   CARET_BITMAP_HEIGHT + 2 );
      }
      else
         SetCaretPos( nPos, YMARGIN + nExtraY + 1 );

      SendMessage( hWnd, EM_CARETPOS, lpField->nCurrentPos, 0l );
   }

// DrUnlockTaskMemory( lpField );
}

zSHORT
GetCaretXPos( )
{
   TRACE_DEBUG( "<CARET.CPP>", "GetCaretXPos " );
   POINT  pt;
   zLONG lPos;

   GetCaretPos( &pt );
   lPos = pt.x;

   if ( GetOverstrikeMode( LOCAL ) )
      lPos += CARET_HALF_WIDTH;

   return( (zSHORT) lPos );
}

zBOOL
CaretPosition( HWND hWnd, zSHORT nChar, LPARAM  lParam )
{
   TRACE_DEBUG( "<CARET.CPP>", "CaretPosition" );
   zBOOL fRet = 0;

   if ( lParam )
   {
      if ( GetOverstrikeMode( LOCAL ) )
         fRet = FALSE;
      else
         fRet = TRUE;
   }

   return( fRet );
}

zSHORT
CaretPosOfCharacter( LPEDITFIELD lpField, zSHORT nChar )
{
   TRACE_DEBUG( "<CARET.CPP>", "CaretPosOfCharacter " );
   zLONG   lStart;
   zSHORT  k;
   zLONG   lPos;
   zCHAR   chLast;

   if ( lpField->bRightJustify )
      lStart = EditGetRightPos( lpField );
   else
   {
      lStart = XMARGIN;

      if ( lpField->f3D )
      {
         lStart += lpField->Edit3DFormat.nShadowSize;

         if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
            lStart += 1;
      }
   }

   for ( k = 0, lPos = lStart; k < nChar; k++ )
      lPos += GetSingleCharWidth( lpField, (BYTE) lpField->szString[ k ] );

   if ( GetOverstrikeMode( LOCAL ) )
   {
      if ( nChar < lpField->nChars )
         chLast = lpField->szString[ nChar ];
      else
         chLast = ' ';

      lPos += GetSingleCharWidth( lpField, (BYTE) chLast ) / 2;
   }
   else
   if ( lpField->bRightJustify )
      lPos -= lpField->tmOverhang;

   return( (zSHORT) lPos );
}

void
CreateModeCaret( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<CARET.CPP>", "CreateModeCaret " );
   int      xWidth, yHeight;
   int      nPos;
   HBITMAP  hBitmap;

   DestroyCaret ( );

   if ( GetOverstrikeMode( LOCAL ) )
   {
      xWidth = CARET_BITMAP_WIDTH;
      yHeight = CARET_BITMAP_HEIGHT;

      if ( lpField->hBitmapCaret == 0 )
      {
         Bitmap.bmBits = (zPCHAR) byBits;
         lpField->hBitmapCaret = kzCreateBitmapIndirect( &Bitmap );
      }

      hBitmap = lpField->hBitmapCaret;
      if ( lpField->nChars > 0 && lpField->nCurrentPos == lpField->nChars )
         lpField->nCurrentPos--;
   }
   else
   {
      xWidth = 1;
      yHeight = lpField->lHeightY + 1;
      hBitmap = 0;
   }

   nPos = CaretPosOfCharacter( lpField, lpField->nCurrentPos );
   CreateCaret( hWnd, hBitmap, xWidth, yHeight );
   SetCaretXPos( hWnd, nPos );
   if ( GetFocus( ) == hWnd )       // This field has the focus
      ShowCaret( hWnd );
}

zBOOL
GetOverstrikeMode( zBOOL bGlobal )
{
   TRACE_DEBUG( "<OVERMODE.CPP>", "GetOverstrikeMode " );
   return( bGlobal ? bGlobalOverstrike : bLocalOverstrike );
}

void
SetOverstrikeMode( HWND hWnd, zBOOL bGlobal )
{
   TRACE_DEBUG( "<OVERMODE.CPP>", "SetOverstrikeMode " );

   bLocalOverstrike = TRUE;
   if ( bGlobal )
      bGlobalOverstrike = TRUE;

   if ( GetFocus( ) == hWnd )       // This field has the focus
   {
      LPEDITFIELD lpField = LockField( hWnd );
      CreateModeCaret ( hWnd, lpField );
//    DrUnlockTaskMemory( lpField );
   }
}

void
SetInsertMode( HWND hWnd, zBOOL bGlobal )
{
   TRACE_DEBUG( "<OVERMODE.CPP>", "SetInsertMode " );

   bLocalOverstrike = FALSE;
   if ( bGlobal )
      bGlobalOverstrike = FALSE;

   if ( GetFocus( ) == hWnd )       // This field has the focus
   {
      LPEDITFIELD lpField = LockField( hWnd );
      CreateModeCaret( hWnd, lpField );
//    DrUnlockTaskMemory( lpField );
   }
}

zBOOL
GetFontWidth( HWND hWnd, HFONT hFont )
{
   TRACE_DEBUG( "<CHAR.CPP>", "GetFontWidth" );
   HDC         hDC;
   TEXTMETRIC  tm;
   int         nCharWidths[ N_PRINTABLE_CHARS ];
   zBOOL       fRetCode = FALSE;
   zSHORT      k;
   HFONT       hFontOld = 0;

   LPEDITFIELD lpField = LockField( hWnd );
   if ( lpField )
   {
      hDC = GetDC( hWnd );
      if ( hDC )
      {
         lpField->hFont = hFont;
         if ( hFont )                 // a font is given so select it
            hFontOld = (HFONT) SelectFont( hDC, hFont );

         // else use the system font
         GetTextMetrics( hDC, &tm );
         lpField->lHeightY = tm.tmHeight;
         lpField->tmOverhang = tm.tmOverhang;

         GetCharWidth( hDC, ' ', N_PRINTABLE_CHARS, nCharWidths );
         for ( k = 0; k < N_PRINTABLE_CHARS - ' '; k++ )
         {
            lpField->nCharWidthTable[ k ] = (BYTE) nCharWidths[ k ] -
                                            (BYTE) tm.tmOverhang;
         }

         if ( hFontOld )
            SelectFont( hDC, hFontOld );

         ReleaseDC( hWnd, hDC );
         fRetCode = TRUE;
      }

//    DrUnlockTaskMemory( lpField );
   }

   return( fRetCode );
}

zLONG
GetTextLength( HWND hWnd )
{
   TRACE_DEBUG( "<TEXT.CPP>", "GetTextLength " );
   zLONG lTextLth;

   LPEDITFIELD lpField = LockField( hWnd );
   lTextLth = zstrlen( lpField->szString );
// DrUnlockTaskMemory( lpField );

   return( lTextLth );
}

zBOOL
CreateField( zVIEW v, HWND hWnd, LPARAM lParam )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "CreateField " );
   LPEDITFIELD   lpField;
   zPCHAR        hField;
   zLONG         lStyle;
   zBOOL         fRetCode = FALSE;

   if ( DrAllocTaskMemory( &hField, sizeof( EDITFIELD ), 1001 ) != 0 )
   {
      NotifyParent( hWnd, EN_ERRSPACE );
   }
   else
   {
      lpField = (LPEDITFIELD) DrLockTaskMemory( hField );
      if ( lpField )
      {
         SetWindowLong( hWnd, zGWL_HFIELD, (zLONG) lpField );

         lpField->hWnd =          hWnd;
         lpField->szString[ 0 ] = 0;
         lpField->nChars =        0;
         lpField->nMaxChars =     -1;
         lpField->bModified =     0;
         lpField->nCurrentPos =   0;
         lpField->bHilited =      FALSE;
         lpField->nHiEnd = lpField->nHiStart = 0;

         lpField->f3D = FALSE;
         lpField->Edit3DFormat.nShadowSize = 1;
         lpField->Edit3DFormat.ColorShadow = GetSysColor( COLOR_BTNSHADOW );
         lpField->Edit3DFormat.ColorInsideShadow = GetSysColor( COLOR_BTNSHADOW );
         lpField->Edit3DFormat.ColorHilite = GetSysColor( COLOR_BTNHIGHLIGHT );
         lpField->Edit3DFormat.ColorInsideShadow = GetSysColor( COLOR_BTNFACE );
         lpField->Edit3DFormat.ColorInsideHilite = GetSysColor( COLOR_BTNSHADOW );

         GetFontWidth( hWnd, 0 );

         lStyle = ((LPCREATESTRUCT) lParam)->style;

         if ( lStyle & ES_STATIC )
         {
            lStyle &= ~WS_TABSTOP;
            ((LPCREATESTRUCT) lParam)->style = lStyle;
            SetWindowLong( hWnd, GWL_STYLE, lStyle );
         }

         if ( lStyle & ES_3D )
            lpField->f3D = TRUE;

         if ( lStyle & WS_BORDER )
            lpField->nRightLimitX = ((LPCREATESTRUCT) lParam)->cx - 1;
         else
            lpField->nRightLimitX = ((LPCREATESTRUCT) lParam)->cx;

         lpField->bOverstrike = (lStyle & ES_OVERSTRIKE) != 0;

         lpField->bRightJustify = (lStyle & ES_RIGHT) != 0;
         lpField->fPassWord = (char) ((lStyle & ES_PASSWORD) != 0);

         if ( (lStyle & ES_UPPERCASE) != 0 )
            lpField->nCase = UPPERCASE;
         else
         if ( (lStyle & ES_LOWERCASE) != 0 )
            lpField->nCase = LOWERCASE;
         else
            lpField->nCase = 0;

         lpField->bMouseDown = FALSE;
         ShiftUp( );

         SetInsertMode( hWnd, GLOBAL );
//       DrUnlockTaskMemory( lpField );
         fRetCode = TRUE;
      }
      else
         NotifyParent( hWnd, EN_ERRSPACE );
   }

   return( fRetCode );
}

void
OpenField( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "OpenField " );

   NotifyParent( hWnd, EN_SETFOCUS );

   LPEDITFIELD lpField = LockField( hWnd );
   if ( lpField )
   {
      if ( lpField->bOverstrike )         // Force the field to Overstrike
         SetOverstrikeMode ( hWnd, LOCAL );
      else                                // Depends on tha state of the screen
      if ( GetOverstrikeMode( GLOBAL ) )
         SetOverstrikeMode( hWnd, LOCAL );
      else
         SetInsertMode( hWnd, LOCAL );

      PositionCaret( hWnd, lpField, lpField->nCurrentPos );
      lpField->bHilited = FALSE;

//    DrUnlockTaskMemory( lpField );
   }
}

// This routine positions the caret to the default position after
// a highlighted field is painted.  In practice this is only neccessary
// for the first field in a dialog box when it gets painted on initialization.
// In all other cases the caret gets positioned correctly by the sub-class
// on WM_SETFOCUS.  On dialog initialization, however, the WM_SETFOCUS is
// received before the WM_PAINT.  The WM_PAINT must give the subclass a
// chance to do any special caret positioning, e.g., to the decimal point
// in a float field.  (See PaintField in fieldmgr.c)
//
// This function provides the default positioning if the subclass chose
// not to process the EM_EDIT_POSITIONAFTERPAINT message.
//
void
PositionCaretAfterPaint( HWND hWnd )
{
   TRACE_DEBUG( "<CARET.CPP>", "PositionCaretAfterPaint" );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( GetOverstrikeMode( LOCAL ) )
      PositionCaret( hWnd, lpField, lpField->nHiStart );
   else
      PositionCaret( hWnd, lpField, lpField->nHiEnd );

// DrUnlockTaskMemory( lpField );
}

zSHORT
FindNearestCharacter( LPEDITFIELD lpField, zSHORT nRawPos, zBOOL bRoundUp )
{
   TRACE_DEBUG( "<CHAR.CPP>", "FindNearestCharacter " );
   zSHORT  nStart;
   zSHORT  nPos;
   zSHORT  nCharWidth;
   zSHORT  nCharIdx;
   zSHORT  nLimit;

   if ( lpField->bRightJustify )
      nStart = EditGetRightPos( lpField );
   else
   {
      nStart = XMARGIN;

      if ( lpField->f3D )
      {
         nStart += lpField->Edit3DFormat.nShadowSize;
         if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
            nStart += 1;
      }
   }

   if ( lpField->bRightJustify && GetOverstrikeMode( LOCAL ) )
      nLimit = mMax( lpField->nChars - 1, 0 );
   else
      nLimit = lpField->nChars;

   for ( nCharIdx = 0, nPos = nStart; nCharIdx < nLimit; ++nCharIdx )
   {
      nCharWidth = GetSingleCharWidth( lpField, (BYTE) lpField->szString[ nCharIdx ] );
      if ( nPos + nCharWidth  > nRawPos )
         break;

      nPos += nCharWidth;
   }

   if ( bRoundUp )     // Round up to the next character if closer to it.
   {
      if ( nRawPos - nPos > ( nPos + nCharWidth ) - nRawPos )
         nCharIdx++;
   }

   return( mMin( nCharIdx, nLimit ) );
}

void
MouseMove( HWND hWnd, LPARAM  lParam )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "MouseMove " );
   LPEDITFIELD lpField = LockField( hWnd );
   if ( lpField->bMouseDown )
   {
      lpField->nCurrentPos = FindNearestCharacter( lpField, LOWORD( lParam ),
                                                   !GetOverstrikeMode( LOCAL ) );

      if ( lpField->nCurrentPos != nPrevious )
      {
         UpdateSelectedRegion ( hWnd, lpField );
         SetCaretXPos( hWnd, CaretPosOfCharacter( lpField,
                                                  lpField->nCurrentPos ) );
      }
   }

// DrUnlockTaskMemory( lpField );
}

void
MouseDown( HWND hWnd, LPARAM  lParam )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "MouseDown " );
   int nPos;

   LPEDITFIELD lpField = LockField ( hWnd );

   if ( lpField->bHilited )
      DeselectText( hWnd, lpField );

   lpField->nCurrentPos =
      FindNearestCharacter( lpField, LOWORD( lParam ), FALSE );

   nPos = CaretPosOfCharacter( lpField, lpField->nCurrentPos );
   SetCaretXPos( hWnd, nPos );

   SetCapture( hWnd );
   lpField->bMouseDown = TRUE;

   nHiliteAnchor = lpField->nCurrentPos;
   lpField->nHiEnd = lpField->nHiStart = nHiliteAnchor;
   nPrevious = nHiliteAnchor;
   bFirstMove = TRUE;

#if 0
   HideCaret( hWnd );             // There is a bug here: The HideCaret
                                  // seems to work only on the first
                                  // highlighting operation.
#endif
// DrUnlockTaskMemory( lpField );
}

void
MouseUp( HWND hWnd )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "MouseUp " );
   LPEDITFIELD lpField = LockField( hWnd );

   lpField->bMouseDown = FALSE;
   bFirstMove = FALSE;
   ReleaseCapture( );
   SetCaretXPos( hWnd, CaretPosOfCharacter( lpField, lpField->nCurrentPos ) );

// DrUnlockTaskMemory( lpField );
}

// Called in response to WM_KILLFOCUS only.
void
CloseField( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "CloseField" );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField )
   {
      if ( lpField->bMouseDown )
         MouseUp( hWnd );

      DestroyCaret( );
      DeselectText( hWnd, lpField );
      ShiftUp( );

//    DrUnlockTaskMemory( lpField );
   }

   NotifyParent( hWnd, EN_KILLFOCUS );
}

void
FreeField( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "FreeField" );
   LPEDITFIELD lpField;

   if ( mIs_hWnd( hWnd ) &&
        (lpField = (LPEDITFIELD) GetWindowLong( hWnd, zGWL_HFIELD )) != 0 )
   {
//    lpField = (LPEDITFIELD) DrLockTaskMemory( hField );

      if ( lpField->hBitmapCaret )
         DeleteBitmap( lpField->hBitmapCaret );

      lpField = (LPEDITFIELD) DrUnlockTaskMemory( lpField );
      DrFreeTaskMemory( lpField );
      SetWindowLong( hWnd, zGWL_HFIELD, 0 );
   }
}

void
DestroyField( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "DestroyField" );
   FreeField( hWnd );
}

void
SizeField( HWND hWnd, LPARAM  lParam )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "SizeField " );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField )
   {
      lpField->nRightLimitX = LOWORD( lParam );
//    DrUnlockTaskMemory( lpField );
   }
}

void
PaintField( HWND hWnd, HDC hDC )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "PaintField" );
   PAINTSTRUCT ps;
   HDC         hDCOrig;

   hDCOrig = hDC;
   LPEDITFIELD lpField = LockField( hWnd );

   if ( !hDCOrig )
      hDC = BeginPaint( hWnd, &ps );

   EditPaintField( hWnd, hDC, lpField );

   if ( GetFocus( ) == hWnd )       // This field has the focus
   {
      if ( lpField->bHilited )
      {
         // Give the sub-class an opportunity to do any initial caret
         // positioning required, e.g., float fields need the caret at
         // the decimal point.
         SendMessage( hWnd, EM_EDIT_POSITIONAFTERPAINT, 0, 0L );
      }
      else
         PositionCaret( hWnd, lpField, lpField->nCurrentPos );
   }

   if ( !hDCOrig )
      EndPaint( hWnd, &ps );

// DrUnlockTaskMemory( lpField );
}

void
EditPaintField( HWND hWnd, HDC hDC, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditPaintField" );
   HBITMAP hBitmap;
   HBITMAP hBitmapOld;
   HBRUSH  hBrush = 0;
   HBRUSH  hBrushOld;
   HFONT   hFontOld = 0;
   RECT    rectClient;
   RECT    Rect;
   HDC     hDCMemory;
   zCHAR   szBuffer[ MAXFIELD ];
   zSHORT  nPos;
   zUSHORT k;

   GetClientRect( hWnd, &rectClient );
   CopyRect( &Rect, &rectClient );

   // Create Bitmap to select to the memory device context.
   hBitmap = kzCreateCompatibleBitmap( hDC, Rect.right - Rect.left,
                                       Rect.bottom - Rect.top );
   hDCMemory = CreateCompatibleDC( hDC );

   // Create a memory device context.
   hBitmapOld = SelectBitmap( hDCMemory, hBitmap );
   int nMapModeOld = SetMapMode( hDCMemory, MM_TEXT );

#ifndef __WIN32__
   hBrush = (HBRUSH) SendMessage( GetParent( hWnd ), WM_CTLCOLOR,
                                  (WPARAM) hDC,
                                  MAKELONG( hWnd, CTLCOLOR_TBEDIT ) );
#endif

   COLORREF clrBkOld = SetBkColor( hDCMemory, GetBkColor( hDC ) );
   COLORREF clrTextOld = SetTxColor( hDCMemory, GetTextColor( hDC ) );

   if ( hBrush == 0 )
      hBrush = kzCreateSolidBrush( GetBkColor( hDC ) );

   hBrushOld = SelectBrush( hDCMemory, hBrush );

   // Fill the hDC background with the current background of the display.
   FillRect( hDCMemory, &Rect, hBrush );

   if ( lpField->f3D )
   {
      kzDrawShadows( hDC, &Rect, lpField->Edit3DFormat.nShadowSize,
                    lpField->Edit3DFormat.ColorShadow,
                    lpField->Edit3DFormat.ColorHilite,
                    !(lpField->Edit3DFormat.wStyle & EDIT3D_OUT), FALSE );

      kzShrinkRect( &Rect, lpField->Edit3DFormat.nShadowSize,
                    lpField->Edit3DFormat.nShadowSize );

      if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
      {
         kzDrawShadows( hDC, &Rect, 1, lpField->Edit3DFormat.ColorInsideShadow,
                        lpField->Edit3DFormat.ColorInsideHilite, TRUE, FALSE );

         kzShrinkRect( &Rect, 1, 1 );
      }

      Rect.right = lpField->nRightLimitX - lpField->Edit3DFormat.nShadowSize;

      if ( lpField->Edit3DFormat.wStyle & EDIT3D_INSIDEBORDER )
         Rect.right -= 1;
   }
   else
      Rect.right = lpField->nRightLimitX;

   if ( lpField->hFont )
      hFontOld = SelectFont( hDCMemory, lpField->hFont );

   if ( lpField->fPassWord )
   {
      for ( k = 0; k < zstrlen( lpField->szString ); k++ )
         szBuffer[ k ] = '*';

      szBuffer[ zstrlen( lpField->szString ) ] = 0;
   }
   else
      strcpy_s( szBuffer, sizeof( szBuffer ), lpField->szString );

   if ( lpField->bRightJustify )
   {
      nPos = EditGetRightPos( lpField );
      EditTextOut( hWnd, lpField, hDCMemory, hBrush, lpField->hFont,
                   Rect, nPos, YMARGIN, szBuffer, zstrlen( szBuffer ),
                   TRUE, -1, -1 );
   }
   else
   {
      EditTextOut( hWnd, lpField, hDCMemory, hBrush, lpField->hFont,
                   Rect, XMARGIN, YMARGIN, szBuffer, zstrlen( szBuffer ),
                   FALSE, -1, -1 );
   }

   // Copy the memory device context bitmap to the display.
   BitBlt( hDC, Rect.left, Rect.top,
           Rect.right - Rect.left, Rect.bottom - Rect.top,
           hDCMemory, Rect.left, Rect.top, SRCCOPY );

   // Clean up.
   if ( clrBkOld != CLR_INVALID )
      SetBkColor( hDCMemory, clrTextOld );

   if ( clrTextOld != CLR_INVALID )
      SetTxColor( hDCMemory, clrTextOld );

   SetMapMode( hDCMemory, nMapModeOld );
   SelectBrush( hDCMemory, hBrushOld );
   SelectBitmap( hDCMemory, hBitmapOld );
   if ( hFontOld )
      SelectFont( hDCMemory, hFontOld );

   DeleteDC( hDCMemory );
   DeleteBitmap( hBitmap );

   if ( hBrush )
      DeleteBrush( hBrush );
}

void
EraseField( HWND hWnd, WPARAM  wParam )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EraseField " );
}

void
HiliteField( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "HiliteField" );
   HWND hWndFocus;
   HDC  hDC;

   hWndFocus = GetFocus( );
   if ( hWndFocus == hWnd )
      HideCaret( hWnd );

   hDC = GetDC( hWnd );
   EditPaintField( hWnd, hDC, lpField );
   ReleaseDC( hWnd, hDC );

   if ( hWndFocus == hWnd )
      ShowCaret( hWnd );
}

void
DeselectText( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "DeselectText " );
   if ( lpField->bHilited )
   {
      lpField->nHiEnd = lpField->nHiStart = lpField->nCurrentPos;
      lpField->bHilited = FALSE;

      HiliteField( hWnd, lpField );
   }
   else
      lpField->nHiEnd = lpField->nHiStart = lpField->nCurrentPos;
}

zBOOL
CheckFit( LPEDITFIELD lpField, BYTE cChar )
{
   TRACE_DEBUG( "<CHAR.CPP>", "CheckFit " );
   if ( GetSingleCharWidth( lpField, cChar ) +
              FieldLength( lpField ) + (2 * XMARGIN) > lpField->nRightLimitX )
   {
      return( FALSE );
   }

   if ( lpField->nMaxChars != -1 && lpField->nChars >= lpField->nMaxChars )
      return( FALSE );

   return( TRUE );
}

zBOOL
Validate( HWND hWnd, zPCHAR pchString, zLONG lPos )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "Validate" );
   return( (zBOOL) SendMessage( hWnd, EM_VALIDATE, lPos, (zLONG) pchString ) );
}

void
HomeCaret( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<CARET.CPP>", "HomeCaret " );
   int nPos;

   lpField->nCurrentPos = 0;
   if ( GetFocus( ) == hWnd )       // This field has the focus
   {
      nPos = CaretPosOfCharacter( lpField, lpField->nCurrentPos );
      SetCaretXPos( hWnd, nPos );
   }
}

void
EndCaret( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<CARET.CPP>", "EndCaret " );
   int   nPos;

   if ( lpField->bRightJustify && GetOverstrikeMode ( LOCAL ) )
      lpField->nCurrentPos = lpField->nChars - 1;
   else
      lpField->nCurrentPos = lpField->nChars;

   if ( GetFocus( ) == hWnd )       // This field has the focus
   {
      nPos = CaretPosOfCharacter( lpField, lpField->nCurrentPos );
      SetCaretXPos( hWnd, nPos );
   }
}

void
LeftCaret( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<CARET.CPP>", "LeftCaret " );
   int   nPos;

   nPos = CaretPosOfCharacter( lpField, --lpField->nCurrentPos );
   SetCaretXPos( hWnd, nPos );

   return;
}

void
RightCaret( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<CARET.CPP>", "RightCaret " );
   int   nPos;

   // Right justified overstrike fields...
   //    ...don't let them position past the last character.
   if ( lpField->bRightJustify && GetOverstrikeMode( LOCAL ) &&
        lpField->nCurrentPos + 1 == lpField->nChars )
   {
      return;
   }

   nPos = CaretPosOfCharacter( lpField, ++lpField->nCurrentPos );
   SetCaretXPos( hWnd, nPos );
}

void
EditFieldModifed( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "EditFieldModified" );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField )
   {
      lpField->bModified = TRUE;
//    DrUnlockTaskMemory( lpField );
   }
}

void
InsertCharacter( HWND hWnd, LPEDITFIELD lpField, zCHAR cChar )
{
   TRACE_DEBUG( "<CHAR.CPP>", "InsertCharacter " );
   zCHAR szLocal[ MAXFIELD ];

   if ( !CheckFit( lpField, (BYTE) cChar ) )
      ::MessageBeep( MB_OK );              // No more room in field.
   else
   {
      strcpy_s( szLocal, sizeof( szLocal ), lpField->szString );
      StrInsertChar( cChar, szLocal, lpField->nCurrentPos );
      lpField->bCharBeingInserted = TRUE;
      if ( Validate( hWnd, szLocal, lpField->nCurrentPos ) )
      {
         NotifyParent ( hWnd, EN_UPDATE );
         strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
         lpField->nChars++;
         if ( lpField->bRightJustify == 0 )
            RightCaret ( hWnd, lpField );
         else
            lpField->nCurrentPos++;

         InvalidateRect( hWnd, 0, TRUE );
         UpdateWindow( hWnd );
         EditFieldModifed( hWnd );
         NotifyParent( hWnd, EN_CHANGE );
         if ( mIs_hWnd( hWnd ) == FALSE )
            return;
      }

      lpField->bCharBeingInserted = FALSE;
   }
}

void
ReplaceCharacter( HWND hWnd, LPEDITFIELD lpField, zCHAR cChar )
{
   TRACE_DEBUG( "<CHAR.CPP>", "ReplaceCharacter " );
   zCHAR szLocal[ MAXFIELD ];

   strcpy_s( szLocal, sizeof( szLocal ), lpField->szString );
   szLocal[ lpField->nCurrentPos ] = cChar;
   if ( Validate( hWnd, szLocal, lpField->nCurrentPos ) )
   {
      NotifyParent( hWnd, EN_UPDATE );
      strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
      RightCaret( hWnd, lpField );
      InvalidateRect( hWnd, 0, TRUE );
      UpdateWindow( hWnd );
      EditFieldModifed( hWnd );
      NotifyParent( hWnd, EN_CHANGE );
   }
}

void
ReplaceSelectedText( HWND hWnd, LPEDITFIELD lpField, zPCHAR lpNewText )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "ReplaceSelectedText " );
   zCHAR szLocal[ MAXFIELD ];
   zSHORT nCurrentPos;
   zSHORT nTextLth;

   if ( lpField->bHilited )
   {
      strcpy_s( szLocal, sizeof( szLocal ), lpNewText );
      nTextLth = zstrlen( lpNewText );

      if ( !SendMessage( hWnd, EM_REPLACETEXT, 0, (zLONG) szLocal ) )
      {
         strncpy_s( szLocal, sizeof( szLocal ), lpField->szString, lpField->nHiStart + 1 );
         szLocal[ lpField->nHiStart + 1 ] = 0;  // force null terminator
         strcat_s( szLocal, sizeof( szLocal ), lpNewText );
         strcat_s( szLocal, sizeof( szLocal ), &lpField->szString[ lpField->nHiEnd ] );

         nCurrentPos = lpField->nHiStart + nTextLth;
      }
      else
         nCurrentPos = lpField->nCurrentPos;

      SendMessage( hWnd, EM_RESETPREVLEN, 0, 0L );

      if ( Validate( hWnd, szLocal, nCurrentPos ) )
      {
         NotifyParent( hWnd, EN_UPDATE );
         strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
         lpField->nChars = zstrlen( lpField->szString );
         PositionCaret( hWnd, lpField, lpField->nHiStart + nTextLth );
         lpField->bHilited = FALSE;
         InvalidateRect( hWnd, 0, TRUE );
         UpdateWindow( hWnd );
         EditFieldModifed( hWnd );
         NotifyParent( hWnd, EN_CHANGE );
      }
   }
}

char
ProcessCharacter( HWND hWnd, WPARAM  wParam )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "ProcessCharacter" );
   zCHAR       cChar;
   zCHAR       szNewText[ 2 ];

   if ( wParam >= ' ' && wParam < N_PRINTABLE_CHARS ) // Printable character.
   {
      LPEDITFIELD lpField = LockField( hWnd );

      if ( lpField->nCase == UPPERCASE )       // Force case?
         cChar = (char)(zLONG) zstrupr( (zPCHAR) MAKELONG( wParam, 0 ) );
      else
      if ( lpField->nCase == LOWERCASE )
         cChar = (char)(zLONG) zstrlwr( (zPCHAR) MAKELONG( wParam, 0 ) );
      else
         cChar = (char) wParam;

      if ( GetOverstrikeMode( LOCAL ) )     // Overstrike mode
      {
         if ( lpField->bHilited )
            DeselectText( hWnd, lpField );

         if ( lpField->nCurrentPos < lpField->nChars )
            ReplaceCharacter( hWnd, lpField, cChar );
         else
         {
            if ( lpField->bRightJustify )
               ::MessageBeep( MB_OK );     // No you don't!
            else
               InsertCharacter( hWnd, lpField, cChar );
         }
      }
      else                 // Insert mode
      {
         if ( lpField->bHilited )                 // SubField currently selected.
         {
            szNewText[ 0 ] = cChar;
            szNewText[ 1 ] = 0;
            ReplaceSelectedText( hWnd, lpField, szNewText );
         }
         else
            InsertCharacter( hWnd, lpField, cChar );
      }

      if ( mIs_hWnd( hWnd ) == FALSE )
         return( 0 );

//    DrUnlockTaskMemory( lpField );
   }
   else                       // Unprintable!
      cChar = (char) wParam;

   return( cChar );
}

// There are three stages to this operation:
//    1. Figure out which part of the currently selected region needs
//       to be inverted
//    2. Invert it
//    3. Figure out exactly which characters are now inverted.
//
// For INSERT mode this is easy because:
//    o The caret is always to the left of the current character
//    o Therefore the pivot position is always to the left of the
//      anchor character
//    o And, we always want to highlight up to BUT not including the
//      extreem right hand, or LAST character (indexed by nEnd);
//
// For OVERSTRIKE mode it is mode difficult:
//    o The caret is underneath the current character
//    o Therefore, the pivot position is to the LEFT of the anchor
//      character if the first move was to the RIGHT, and to the RIGHT
//      of the anchor character if the first move was to the LEFT.
//    o We want to avoid inverting either the FIRST or the LAST character
//      of the new sub-region (and therefore of the whole selected region)
//      depending on various criteria.  These include which side of the
//      anchor you currently are, which side of the anchor the pivot
//      position is, and which direction you're going.
//
void
UpdateSelectedRegion( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "UpdateSelectedRegion " );
   zSHORT nStart, nEnd;
   static int nAnchorPivotSide;   // For OVERSTRIKE mode: which side of
                                  // the anchor character the pivot position is on.
   zBOOL bOverstrike;             // The field is in overstrike mode.

   // Figure out which part of the currently selected region
   // needs to be inverted.
   if ( lpField->nCurrentPos > nPrevious )
   {
      nStart = nPrevious;
      nEnd = lpField->nCurrentPos;
   }
   else
   {
      nStart = lpField->nCurrentPos;
      nEnd = nPrevious;
   }

   bOverstrike = GetOverstrikeMode( LOCAL );

   // OVERSTRIKE mode.
   // The given range usually needs trimming at one end or another.
   if ( bOverstrike )
   {
      if ( bFirstMove )          // No trimming required but register which
      {                           //    we are heading in initially.
         if ( lpField->nCurrentPos > nPrevious )
            nAnchorPivotSide = LEFT;
         else
            nAnchorPivotSide = RIGHT;

         bFirstMove = FALSE;
      }
      else
      {
         // Currently to the RIGHT of anchor.
         // Usually don't want to invert the FIRST character
         if ( lpField->nCurrentPos > nHiliteAnchor )
         {
            if ( nPrevious > nHiliteAnchor ||
                 (nPrevious == nHiliteAnchor && nAnchorPivotSide == LEFT) )
            {
               nStart++;
            }
         }
         // Currently to the LEFT of anchor.
         // Usually don't want to invert the LAST character
         else
         if ( lpField->nCurrentPos < nHiliteAnchor )
         {
            if ( nPrevious < nHiliteAnchor ||
                 (nPrevious == nHiliteAnchor && nAnchorPivotSide == RIGHT) )
            {
               nEnd--;
            }
         }
         // Currently ON the anchor. What we invert depends which
         // direction we are moving, and which side of the anchor
         // character the pivot is on.
         else
         {
            if ( lpField->nCurrentPos < nPrevious && // Moving left
                 nAnchorPivotSide == LEFT )
            {
               nStart++;
            }
            else
            if ( lpField->nCurrentPos > nPrevious && // Moving right
                 nAnchorPivotSide == RIGHT )
            {
               nEnd--;
            }
         }
      }
   }

   // INSERT mode.
   // Never include the last character.
   if ( !bOverstrike )
      nEnd--;

   // Invert the new sub-region

   // Figure out exactly which characters are now inverted and update
   // our internal records
   if ( lpField->nCurrentPos > nHiliteAnchor )
   {
      lpField->nHiStart = nHiliteAnchor;
      lpField->nHiEnd = lpField->nCurrentPos + 1;
   }
   else
   {
      lpField->nHiStart = lpField->nCurrentPos;
      lpField->nHiEnd = nHiliteAnchor + 1;
   }

   // OVERSTRIKE mode: Don't include the anchor character if we are
   // on the same side of it as the pivot position.
   if ( bOverstrike )
   {
      if ( lpField->nCurrentPos < nHiliteAnchor && nAnchorPivotSide == LEFT )
         lpField->nHiEnd--;
      else
      if ( lpField->nCurrentPos > nHiliteAnchor && nAnchorPivotSide == RIGHT )
         lpField->nHiStart++;
   }

   bOverstrike = !(zBOOL) SendMessage( hWnd, EM_CARETPOS, lpField->nHiEnd, 1 );

   // INSERT mode: Never include the last character.
   if ( !bOverstrike )
      lpField->nHiEnd--;

   lpField->bHilited = lpField->nHiEnd > lpField->nHiStart;
   HiliteField( hWnd, lpField );
   nPrevious = lpField->nCurrentPos;
}

void
ShiftMove( HWND hWnd )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "ShiftMove " );

   if ( bShiftDown )
   {
      LPEDITFIELD lpField = LockField( hWnd );
      if ( lpField->nCurrentPos != nPrevious )
         UpdateSelectedRegion( hWnd, lpField );

//    DrUnlockTaskMemory( lpField );
   }
}

zBOOL
IsShiftDown( )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "IsShiftDown" );
   return( bShiftDown );
}

zBOOL
SendTextToClipboard( HWND   hWnd,
                     zPCHAR pchText,
                     zSHORT nLth )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "SendTextToClipboard" );
   HANDLE hMem;
   zPCHAR pchMem;
   zBOOL  fRet = FALSE;

   if ( (hMem = GlobalAlloc( GHND, (DWORD) (nLth + 1) )) != 0 )
   {
      pchMem = (zPCHAR) GlobalLock( hMem );
      zmemcpy( pchMem, pchText, nLth );
      GlobalUnlock( hMem );

      OpenClipboard( hWnd );
      EmptyClipboard( );

      SetClipboardData( CF_TEXT, hMem );
      CloseClipboard( );

      fRet = TRUE;
   }

   return( fRet );
}


HANDLE
GetTextFromClipboard( HWND hWnd )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "GetTextFromClipboard" );
   HANDLE hMem;
   HANDLE hText = 0;
   zPCHAR pchMem;
   zPCHAR pchText;

   OpenClipboard( hWnd );

   if ( (hMem = GetClipboardData( CF_TEXT )) != 0 )
   {
      pchMem = (zPCHAR) GlobalLock( hMem );

      if ( zstrlen( pchMem ) )
      {
         if ( (hText = GlobalAlloc( GHND, (DWORD)( zstrlen( pchMem ) + 1 ) ) ) != 0 )
         {
            pchText = (zPCHAR) GlobalLock( hText );
            strcpy_s( pchText, zstrlen( pchMem ) + 1, pchMem );
            GlobalUnlock( hText );
         }
      }

      GlobalUnlock( hMem );
   }

   CloseClipboard( );
   return( hText );
}

WORD
ProcessKeystroke( HWND hWnd, WPARAM wParam )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "ProcessKeystroke " );
   WORD        wKey;
   zLONG       lKey;
   zLONG       lSel;
   WORD        wClearField;
   zCHAR       szLocal[ MAXFIELD ];

   wKey = wParam;

   LPEDITFIELD lpField = LockField( hWnd );

   if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
        (wParam == 'x' || wParam == 'X') )
   {
      ClipboardCut( hWnd );
   }
   else
   if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
        (wParam == 'c' || wParam == 'C') )
   {
      ClipboardCopy( hWnd );
   }
   else
   if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
        (wParam == 'v' || wParam == 'V') )
   {
      ClipboardPaste( hWnd );
   }
   else
   {
      switch ( wKey )
      {
         case VK_LEFT:
         case VK_UP:
            if ( lpField->nCurrentPos > 0 )
               LeftCaret( hWnd, lpField );

            if ( IsShiftDown( ) )
               ShiftMove( hWnd );
            else
               DeselectText( hWnd, lpField );

            break;

         case VK_RIGHT:
         case VK_DOWN:
            if ( lpField->nCurrentPos < lpField->nChars )
               RightCaret( hWnd, lpField );

            if ( IsShiftDown ( ) )
               ShiftMove( hWnd );
            else
               DeselectText( hWnd, lpField );

            break;

         case VK_HOME:
            HomeCaret( hWnd, lpField );

            if ( IsShiftDown( ) )
               ShiftMove( hWnd );
            else
               DeselectText( hWnd, lpField );

            break;

         case VK_END:
            EndCaret( hWnd, lpField );

            if ( IsShiftDown( ) )
               ShiftMove( hWnd );
            else
               DeselectText( hWnd, lpField );

            break;

         case VK_BACK:
            if ( GetOverstrikeMode( LOCAL ) )   // Like VK_LEFT but ignore control key
            {
               if ( lpField->nCurrentPos > 0 )
                  LeftCaret( hWnd, lpField );

               if ( IsShiftDown( ) )
                  ShiftMove( hWnd );
               else
                  DeselectText( hWnd, lpField );

               break;
            }

            if ( lpField->bHilited )
            {
               DeleteSelectedText( hWnd, lpField );
               break;
            }

            if ( lpField->nCurrentPos > 0 )
            {
               strcpy_s( szLocal, sizeof( szLocal ), lpField->szString );
               strcpy_s( szLocal + lpField->nCurrentPos - 1, sizeof( szLocal ) - (lpField->nCurrentPos - 1), // delete 1 char
                         szLocal + lpField->nCurrentPos );
               if ( Validate( hWnd, szLocal, lpField->nCurrentPos ) )
               {
                  NotifyParent( hWnd, EN_UPDATE );
                  strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
                  lpField->nChars--;
                  LeftCaret( hWnd, lpField );
                  InvalidateRect ( hWnd, 0, TRUE );
                  UpdateWindow( hWnd );
                  EditFieldModifed( hWnd );
                  NotifyParent( hWnd, EN_CHANGE );
               }
            }

            break;

         case VK_DELETE:
            if ( IsShiftDown( ) )
               ClipboardCut( hWnd );
            else
            {
               if ( GetOverstrikeMode( LOCAL ) )
               {
                  if ( lpField->bHilited )
                  {
                     lSel = (zLONG) SendMessage( hWnd, EM_GETSEL, 0, 0l );
                     if ( LOWORD( lSel ) == 0 &&
                          HIWORD( lSel ) >= (WORD) lpField->nChars )
                     {
                        DeleteSelectedText( hWnd, lpField );
                     }
                     else
                     {
                        DeselectText( hWnd, lpField );
                        ::MessageBeep( MB_OK ); // can't do that 'ere, mate
                     }
                  }
                  else
                     ::MessageBeep( MB_OK );    // can't do that 'ere, mate

                  break;
               }

               if ( lpField->bHilited )
               {
                  DeleteSelectedText ( hWnd, lpField );
                  break;
               }

               if ( lpField->nCurrentPos < lpField->nChars )
               {
                  strcpy_s( szLocal, sizeof( szLocal ), lpField->szString );
                  strcpy_s( szLocal + lpField->nCurrentPos, sizeof( szLocal ) - lpField->nCurrentPos, // delete 1 char
                            szLocal + lpField->nCurrentPos + 1 );
                  if ( Validate( hWnd, szLocal, lpField->nCurrentPos ) )
                  {
                     NotifyParent( hWnd, EN_UPDATE );
                     strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
                     lpField->nChars--;
                     InvalidateRect( hWnd, 0, TRUE );
                     UpdateWindow( hWnd );
                     EditFieldModifed( hWnd );
                     NotifyParent( hWnd, EN_CHANGE );
                  }
               }
            }

            break;

         case VK_INSERT:
            if ( IsShiftDown( ) )
               ClipboardPaste( hWnd );
            else
            if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
               ClipboardCopy( hWnd );
            else
            {
               if ( lpField->bHilited )
                  DeselectText( hWnd, lpField );
               if ( GetOverstrikeMode ( GLOBAL ) )
                  SetInsertMode( hWnd, GLOBAL );
               else
                  SetOverstrikeMode( hWnd, GLOBAL );
            }

            break;

         case VK_SHIFT:
            ShiftDown( hWnd );
            break;

         default:
            // Check for signal to clear the field
            if ( GetToolBoxGlobal( TBG_CLEARFIELD, &lKey ) )
               wClearField = (WORD) lKey;

            if ( wKey == wClearField )
            {
               EditFieldModifed( hWnd );
               SendMessage( hWnd, WM_SETTEXT, 0, (LPARAM) "" );
            }

            break;
      }
   }

// DrUnlockTaskMemory( lpField );
   return( wKey );
}

WORD
ProcessKeyUp( HWND hWnd, WPARAM wKey )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "ProcessKeyUp" );

   LPEDITFIELD lpField = LockField( hWnd );
   switch ( wKey )
   {
      case VK_SHIFT:
         ShiftUp( );
         break;

      default:
         break;
   }

// DrUnlockTaskMemory( lpField );
   return( wKey );
}

void
InsertText( HWND hWnd, LPEDITFIELD lpField, zPCHAR lpNewText )
{
   TRACE_DEBUG( "<TEXT.CPP>", "InsertText" );
   zCHAR szLocal[ MAXFIELD ];
   zULONG lTextLth;

   zmemset( szLocal, 0, sizeof( szLocal ) );

   lTextLth = zstrlen( lpNewText );
   lTextLth = mMin( lTextLth, MAXFIELD - 1 - zstrlen( lpField->szString ) );

   zmemcpy( szLocal, lpField->szString, lpField->nCurrentPos );
   zmemcpy( szLocal + zstrlen( szLocal ), lpNewText, lTextLth );
   strcat_s( szLocal + zstrlen( szLocal ), sizeof( szLocal ) - zstrlen( szLocal ),
             lpField->szString + lpField->nCurrentPos );

   if ( lpField->nMaxChars != -1 )
      szLocal[ lpField->nMaxChars ] = 0;

   if ( Validate( hWnd, szLocal, lpField->nCurrentPos ) )
   {
      NotifyParent( hWnd, EN_UPDATE );
      strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
      lpField->nChars = zstrlen( lpField->szString );
      InvalidateRect( hWnd, 0, TRUE );
      UpdateWindow( hWnd );
      EditFieldModifed( hWnd );
      NotifyParent( hWnd, EN_CHANGE );
   }
}

void
ClearSelection( HWND hWnd )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "ClearSelection " );

   if ( !GetOverstrikeMode( LOCAL ) )
   {
      LPEDITFIELD lpField = LockField( hWnd );

      if ( lpField->bHilited )
         DeleteSelectedText( hWnd, lpField );

//    DrUnlockTaskMemory( lpField );
   }
}

zLONG
GetSelection( HWND hWnd )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "GetSelection " );
   zLONG lStartEnd;
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField->bHilited )
   {
      lStartEnd = MAKELONG( lpField->nHiStart, lpField->nHiEnd );
   }
   else
   {
      // If there is currently no selection return the caret position.
      lStartEnd = MAKELONG( lpField->nCurrentPos, lpField->nCurrentPos );
   }

// DrUnlockTaskMemory( lpField );
   return( lStartEnd );
}

zBOOL
ReplaceSel( HWND    hWnd,
            LPARAM  lParam )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "ReplaceSel" );
   if ( lParam )
   {
      LPEDITFIELD lpField = LockField( hWnd );
      ReplaceSelectedText( hWnd, lpField, (zPCHAR) lParam );
//    DrUnlockTaskMemory( lpField );
      return( TRUE );
   }

   return( FALSE );
}

void
SetSelection( HWND hWnd, LPARAM lParam )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "SetSelection " );

   LPEDITFIELD lpField = (LPEDITFIELD) LockField( hWnd );
   if ( lpField )
   {
      WORD  wStart;
      WORD  wEnd;
      WORD  wTemp;

      // isolate the extremities
      wStart = LOWORD( lParam );
      wEnd =   HIWORD( lParam );

      // limit the range to the current field contents
      if ( wEnd > (WORD) lpField->nChars )
         wEnd = lpField->nChars;

      // swap them if they are in the wrong order
      if ( wEnd < wStart )
      {
         wTemp = wStart;
         wStart = wEnd;
         wEnd = wTemp;
      }

      // if start and end are the same, turn off selection
      if ( wStart == wEnd )
      {
         DeselectText( hWnd, lpField );
         lpField->nHiStart = (int) wStart;
         lpField->nHiEnd = (int) wEnd;
      }
      else  // Store them in the field's data structure
      {
         lpField->nHiStart = (int) wStart;
         lpField->nHiEnd = (int) wEnd;

         if ( GetOverstrikeMode( LOCAL ) )
            nHiliteAnchor = lpField->nHiEnd;
         else
            nHiliteAnchor = lpField->nHiStart;
      }

      // Position the caret appropriately
      if ( GetOverstrikeMode( LOCAL ) )
         PositionCaret( hWnd, lpField, lpField->nHiStart );
      else
         PositionCaret( hWnd, lpField, lpField->nHiEnd );

      if ( lpField->nHiStart == lpField->nHiEnd )
         lpField->bHilited = FALSE;
      else
         lpField->bHilited = TRUE;

//    DrUnlockTaskMemory( lpField );
      InvalidateRect( hWnd, 0, TRUE );
      UpdateWindow( hWnd );
   }
}

void
DeleteSelectedText( HWND hWnd, LPEDITFIELD lpField )
{
   TRACE_DEBUG( "<HILIGHT.CPP>", "DeleteSelectedText " );
   zCHAR szLocal[ MAXFIELD ];

   if ( lpField->bHilited )
   {
      strcpy_s( szLocal, sizeof( szLocal ), lpField->szString );
      StrRemove( szLocal, lpField->nHiStart, mMin( zstrlen( szLocal ), (zUSHORT) lpField->nHiEnd ) - 1 );
      if ( Validate( hWnd, szLocal, lpField->nCurrentPos ) )
      {
         NotifyParent( hWnd, EN_UPDATE );
         strcpy_s( lpField->szString, sizeof( lpField->szString ), szLocal );
         lpField->nChars -= lpField->nHiEnd - lpField->nHiStart;
         lpField->nChars = mMax( lpField->nChars, 0 );
         PositionCaret( hWnd, lpField, lpField->nHiStart );
         lpField->bHilited = FALSE;
         InvalidateRect( hWnd, 0, TRUE );
         UpdateWindow( hWnd );
         EditFieldModifed( hWnd );
         NotifyParent( hWnd, EN_CHANGE );
      }
   }
}

void
ClipboardCopy( HWND hWnd )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "ClipboardCopy" );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField->bHilited )
      SendTextToClipboard( hWnd, &lpField->szString[lpField->nHiStart],
                           lpField->nHiEnd - lpField->nHiStart );

// DrUnlockTaskMemory( lpField );
}

void
ClipboardCut( HWND hWnd )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "ClipboardCut" );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField->bHilited )
   {
      if ( SendTextToClipboard( hWnd, &lpField->szString[ lpField->nHiStart ],
                                lpField->nHiEnd - lpField->nHiStart ) )
         DeleteSelectedText( hWnd, lpField );
   }

// DrUnlockTaskMemory( lpField );
}

void
ClipboardPaste( HWND hWnd )
{
   TRACE_DEBUG( "<KEYBOARD.CPP>", "ClipboardPaste" );
   zPVOID       hText;
   zPCHAR       pchText;
   LPEDITFIELD lpField = LockField( hWnd );

   if ( (hText = GetTextFromClipboard( hWnd )) != 0 )
   {
      pchText = (zPCHAR) GlobalLock( hText );

      if ( lpField->bHilited )
         ReplaceSelectedText( hWnd, lpField, pchText );
      else
         InsertText( hWnd, lpField, pchText );

      GlobalUnlock( hText );
      GlobalFree( hText );
   }

// DrUnlockTaskMemory( lpField );
}

void
SetFieldText( HWND hWnd, LPARAM  lParam )
{
   TRACE_DEBUG( "<TEXT.CPP>", "SetFieldText " );
   zPCHAR pchText;
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lParam )
      pchText = (zPCHAR) lParam;
   else
      pchText = "";

   NotifyParent( hWnd, EN_UPDATE );

   if ( lpField->bHilited )
      DeselectText( hWnd, lpField );

   strncpy_s( lpField->szString, sizeof( lpField->szString ), pchText, sizeof( lpField->szString ) - 1 );

   if ( lpField->nMaxChars != -1 )
      lpField->szString[ lpField->nMaxChars ] = '\0';

   if ( lpField->nCase == UPPERCASE )       // Force case?
      zstrupr( lpField->szString );
   else
   if ( lpField->nCase == LOWERCASE )
      zstrlwr( lpField->szString );

   lpField->nChars = zstrlen( lpField->szString );

   if ( lpField->nCurrentPos >= lpField->nChars )
      PositionCaret( hWnd, lpField, lpField->nChars );

   InvalidateRect( hWnd, 0, TRUE );

   EditFieldModifed( hWnd );
   NotifyParent( hWnd, EN_CHANGE );

// DrUnlockTaskMemory( lpField );
}

zSHORT
GetFieldText( HWND hWnd, WPARAM  wParam, LPARAM  lParam )
{
   TRACE_DEBUG( "<TEXT.CPP>", "GetFieldText" );

   if ( lParam )
   {
      LPEDITFIELD lpField = LockField( hWnd );
      zPCHAR      pchText = (zPCHAR) lParam;
      zSHORT      nLth = zstrlen( lpField->szString );
      zSHORT      nLth2 = nLth + 1;

      if ( wParam > 0 )
      {
         nLth = mMin( nLth, (zSHORT) wParam );
         nLth2 = mMin( nLth2, (zSHORT) wParam );
      }

      zmemcpy( pchText, lpField->szString, nLth2 );
//    DrUnlockTaskMemory( lpField );
   }

   return( 0 );
}

HFONT
ProcessEditSetFont( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
   TRACE_DEBUG( "<CHAR.CPP>", "ProcessEditSetFont" );
   LPEDITFIELD lpField = LockField( hWnd );
   if ( lpField )
   {
      if ( wParam || lpField->hFont )
      {
         if ( GetFontWidth( hWnd, (HFONT) wParam ) && (zBOOL) lParam )
            InvalidateRect( hWnd, 0, TRUE );    // repaint with the new font
      }

//    DrUnlockTaskMemory( lpField );
   }

   return( (HFONT) wParam );
}

HFONT
ProcessEditGetFont( HWND hWnd )
{
   TRACE_DEBUG( "<CHAR.CPP>", "ProcessEditGetFont" );
   HFONT       hFont = 0;
   LPEDITFIELD lpField = LockField( hWnd );
   if ( lpField )
   {
      hFont = lpField->hFont;
//    DrUnlockTaskMemory( lpField );
   }

   return( hFont );
}

zBOOL
GetModify( HWND hWnd )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "GetModify" );
   zBOOL       Modify = FALSE;
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField )
   {
      Modify = lpField->bModified;
//    DrUnlockTaskMemory( lpField );
   }

   return( Modify );
}

void
SetModify( HWND hWnd, zBOOL Modify )
{
   TRACE_DEBUG( "<FIELDMGR.CPP>", "SetModify" );
   LPEDITFIELD lpField = LockField( hWnd );

   if ( lpField )
   {
      lpField->bModified = Modify;
//    DrUnlockTaskMemory( lpField );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zLONG OPERATION
EditFieldWndProc( HWND     hWnd,
                  WORD     uMsg,
                  WPARAM   wParam,
                  LPARAM   lParam )
{
   TRACE_DEBUG( "<EDITFLD.CPP>", "EditFieldWndProc" );
   zLONG lRC = 0;
   zCHAR  cCharacter;
   WORD  wKey;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "EditFieldWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_NCCREATE:
         if ( CreateField( hWnd, lParam ) )
            lRC = DefWindowProc( hWnd, uMsg, wParam, lParam );
         else
            lRC = 0;
         break;

      case WM_SIZE:
         SizeField( hWnd, lParam );
         break;

      case WM_DESTROY:
         DestroyField( hWnd );
         return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );

      case WM_CHAR:
         cCharacter = ProcessCharacter( hWnd, wParam );
         break;

      case WM_KEYDOWN:
         wKey = ProcessKeystroke( hWnd, wParam );
         break;

      case WM_KEYUP:
         wKey = ProcessKeyUp( hWnd, wParam );
         break;

      case WM_ENABLE:
         InvalidateRect( hWnd, 0, TRUE );
         UpdateWindow( hWnd );
         break;

      case WM_ERASEBKGND:
         EraseField( hWnd, wParam );
         return( TRUE );

      case WM_PAINT:
         PaintField( hWnd, (HDC) wParam );
         break;

      case WM_GETDLGCODE:
         if ( GetWindowLong( hWnd, GWL_STYLE ) & ES_STATIC )
            lRC = DLGC_STATIC;
         else
            lRC = EDIT_FIELD_CODES;

         break;

      case WM_NCHITTEST:
         if ( GetWindowLong( hWnd, GWL_STYLE ) & ES_STATIC )
            return( HTTRANSPARENT );

         return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );

      case WM_SETFONT:
         ProcessEditSetFont( hWnd, wParam, lParam );
         break;

      case WM_GETFONT:
         return( (zLONG) ProcessEditGetFont( hWnd ) );

      case WM_MOUSEACTIVATE:
         if ( !(GetWindowLong( hWnd, GWL_STYLE ) & ES_STATIC) )
            SetFocus( hWnd );

         break;

      case WM_LBUTTONDOWN:
         MouseDown( hWnd, lParam );
         break;

      case WM_MOUSEMOVE:
         MouseMove( hWnd, lParam );
         break;

      case WM_LBUTTONUP:
         MouseUp( hWnd );
         break;

      case WM_SETFOCUS:
         if ( GetWindowLong( hWnd, 0 ) )
         {
            if ( !(GetWindowLong( hWnd, GWL_STYLE ) & ES_STATIC) )
               OpenField( hWnd );
         }

         break;

      case WM_KILLFOCUS:
         CloseField( hWnd );
         break;

      case WM_SETTEXT:
         SetFieldText( hWnd, lParam );
         break;

      case WM_GETTEXT:
         lRC = (zLONG) GetFieldText( hWnd, wParam, lParam );
         break;

      case WM_GETTEXTLENGTH:
         lRC = GetTextLength( hWnd );
         break;

      case WM_CLEAR:
         ClearSelection( hWnd );
         lRC = 1;
         break;

      case WM_CUT:
         ClipboardCut( hWnd );
         lRC = 1;
         break;

      case WM_COPY:
         ClipboardCopy( hWnd );
         lRC = 1;
         break;

      case WM_PASTE:
         ClipboardPaste( hWnd );
         lRC = 1;
         break;

      // EM_...   Windows dialog edit class messages.

      case EM_GETMODIFY:
         lRC = GetModify( hWnd );
         break;

      case EM_SETMODIFY:
         SetModify( hWnd, (zBOOL) wParam );
         break;

      case EM_SETSEL:
         SetSelection( hWnd, lParam );
         break;

      case EM_REPLACESEL:
         lRC = ReplaceSel( hWnd, lParam );
         break;

      case EM_GETSEL:
         lRC = GetSelection( hWnd );
         break;


      // Additional EM_... messages defined by us, in TOOLBOX.H

      case EM_VALIDATE:
         lRC = TRUE;
         break;

      case EM_CARETPOS:
         lRC = CaretPosition( hWnd, wParam, lParam );
         break;

      case EM_SETCARETPOS:
         SetCaretPosition( hWnd, wParam );
         break;

      case EM_GETCARETPOS:
         lRC = (zLONG) GetCaretPosition( hWnd );
         break;

      // Private ToolBox messages defined in LIBGEN.H

      case EM_EDIT_POSITIONAFTERPAINT:
         PositionCaretAfterPaint( hWnd );
         break;

      case EM_EDIT_ADJUSTLENGTH:
         AdjustLength( hWnd );
         break;

      case EM_GET3D:
         lRC = EditGet3DParams( hWnd, lParam );
         break;

      case EM_SET3D:
         EditSet3DParams( hWnd, wParam, lParam );
         break;

      default:
         return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
   }

   return( lRC );
}

zLONG OPERATION
EditGeneralWndProc( HWND     hWnd,
                    WORD     uMsg,
                    WPARAM   wParam,
                    LPARAM   lParam )
{
   TRACE_DEBUG( "<EDITGEN.CPP>", "EditGeneralWndProc" );
   zLONG lRC = 0L;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "EditGeneralWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_NCCREATE:
         if ( !GenCreateField( hWnd, lParam ) )
            return( 0 );

         lRC = CallWindowProc( (WNDPROC) GetDefWndProc( hWnd ),
                               hWnd, uMsg, wParam, lParam );
         break;

      case WM_CREATE:
         EditSuspendNotify( hWnd, TRUE );
         GenInitializeField( hWnd, lParam );
         EditSuspendNotify( hWnd, FALSE );
         break;

      case WM_DESTROY:
         FreeGenField( hWnd );
         lRC = CallWindowProc( (WNDPROC) GetDefWndProc( hWnd ),
                               hWnd, uMsg, wParam, lParam );
         break;

      case EM_LIMITTEXT:
         GenLimitText( hWnd, wParam );
         break;

      default:
         lRC = CallWindowProc( (WNDPROC) GetDefWndProc( hWnd ),
                               hWnd, uMsg, wParam, lParam );
         break;
   }

   return( lRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
SS_CheckBoxWndProc( HWND    hWnd,
                    WORD    uMsg,
                    WPARAM  wParam,
                    LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_CheckBoxWndProc" );
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_CheckBoxWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   if ( uMsg == BM_SETSTYLE )
      return( 0 );

   if ( uMsg == WM_KEYDOWN && wParam == VK_RETURN )
      SS_SendMsgCommand( GetParent( hWnd ), 0, BN_CLICKED, FALSE );

   switch ( uMsg )
   {
      case WM_KEYDOWN:
         switch ( wParam )
         {
            case VK_UP:
            case VK_DOWN:
               SendMessage( GetParent( hWnd ), WM_KEYDOWN, wParam, 0L );
               return( 0 );

            case VK_LEFT:
            case VK_RIGHT:
               SendMessage( GetParent( hWnd ), WM_KEYDOWN, wParam, 0L );
               return( 0 );
         }

         break;
   }

   return( SS_DefProc( g_lpfnCheckBoxProc, GetParent( hWnd ),
                       hWnd, uMsg, wParam, lParam ) );
}

LRESULT CALLBACK
SS_ComboBoxWndProc( HWND     hWnd,
                    WORD     uMsg,
                    WPARAM   wParam,
                    LPARAM   lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_ComboBoxWndProc" );
   LPSPREADSHEET       lpSS;
   LPMEASUREITEMSTRUCT lpmis;
   LPDRAWITEMSTRUCT    lpdis;
   SS_CLRTBLITEM       ForeClrTblItem;
   SS_CLRTBLITEM       BackClrTblItem;
   TEXTMETRIC          tm;
   COLORREF            clrBkOld;
   COLORREF            clrTextOld;
   HBRUSH              hBrush;
   zPCHAR              lpData;
   zSHORT              nShift;
   HDC                 hDC;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_ComboBoxWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_SETFOCUS:
         break;

      case WM_KEYDOWN:
         nShift = 0;
         SS_ShowEditField( GetParent( hWnd ), hWnd );

         if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            nShift |= 0x01;

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            nShift |= 0x02;

#ifdef SS_GP
         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
              (wParam == 'z' || wParam == 'Z') )
         {
            wParam = VK_ESCAPE;
         }
#endif

         switch ( wParam )
         {
            case VK_ESCAPE:
               if ( SS_SendMsg( GetParent( hWnd ), SSM_KEYDOWN,
                                GetWindowLong( GetParent( hWnd ), GWL_ID ),
                                               MAKELONG( wParam, nShift ) ) )
               {
                  return( 0 );
               }

               lpSS = SS_Lock( GetParent( hWnd ) );
               lpSS->EscapeInProgress = TRUE;

//             DrUnlockTaskMemory( lpSS );
               PostMessage( GetParent( hWnd ), SSM_SETEDITMODE, FALSE, 0L );

               return( TRUE );

            case VK_RETURN:
               if ( !SS_SendMsg( GetParent( hWnd ), SSM_KEYDOWN,
                                 GetWindowLong( GetParent( hWnd ), GWL_ID ),
                                                MAKELONG( wParam, nShift ) ) )
               {
                  PostMessage( GetParent( hWnd ), SSM_PROCESSENTERKEY, 0, 0L );
               }

               return( TRUE );

            case VK_F1:
               SendMessage( GetParent( hWnd ), WM_KEYDOWN, wParam, 0L );
               return( 0 );

            case VK_HOME:
            case VK_END:
            case VK_RIGHT:
            case VK_LEFT:
            case VK_UP:
            case VK_DOWN:
               if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
               {
                  SendMessage( GetParent( hWnd ), WM_KEYDOWN, wParam, 0L );
                  return( 0 );
               }

               if ( SS_SendMsg( GetParent( hWnd ), SSM_KEYDOWN,
                                GetWindowLong( GetParent( hWnd ), GWL_ID ),
                                               MAKELONG( wParam, nShift ) ) )
               {
                  return( 0 );
               }

               break;

            case VK_TAB:
               return( SS_EditProcessTab( GetParent( hWnd ), uMsg,
                                          wParam, lParam, nShift ) );

            default:
               if ( SS_SendMsg( GetParent( hWnd ), SSM_KEYDOWN,
                                GetWindowLong( GetParent( hWnd ), GWL_ID ),
                                               MAKELONG( wParam, nShift ) ) )
               {
                  return( 0 );
               }
         }

         return( CallWindowProc( (WNDPROC) g_lpfnComboBoxProc,
                                 hWnd, uMsg, wParam, lParam ) );

      case WM_MEASUREITEM:
         lpmis = (LPMEASUREITEMSTRUCT) lParam;

         hDC = GetDC( hWnd );
         GetTextMetrics( hDC, &tm );
         ReleaseDC( hWnd, hDC );
         lpmis->itemHeight = tm.tmHeight;
         return( 0 );

      case WM_DRAWITEM:
         lpdis = (LPDRAWITEMSTRUCT) lParam;
      // TraceLineS( "ComboBox DRAWITEM", "" );
         if ( ((lpdis->itemAction & ODA_DRAWENTIRE) ||
                (lpdis->itemAction & ODA_SELECT)) && lpdis->itemData )
         {
            lpData = (zPCHAR) lpdis->itemData;

            if ( lpdis->itemState & ODS_SELECTED )
            {
               clrBkOld = SetBkColor( lpdis->hDC,
                                      GetSysColor( COLOR_HIGHLIGHT ) );
               clrTextOld = SetTxColor( lpdis->hDC,
                                          GetSysColor( COLOR_HIGHLIGHTTEXT ) );
            }
            else
            {
               lpSS = SS_Lock( GetParent( hWnd ) );

               SS_GetClrTblItem( GetParent( hWnd ), lpSS, lpSS->Col.ssCurrAt,
                                   lpSS->Row.ssCurrAt, &BackClrTblItem,
                                   &ForeClrTblItem );

//             DrUnlockTaskMemory( lpSS );

               clrBkOld = SetBkColor( lpdis->hDC, BackClrTblItem.Color );
               clrTextOld = SetTxColor( lpdis->hDC, ForeClrTblItem.Color );
            }

            hBrush = kzCreateSolidBrush( GetBkColor( lpdis->hDC ) );
            FillRect( lpdis->hDC, &lpdis->rcItem, hBrush );
            DeleteBrush( hBrush );

            int nBkModeOld = SetBkMode( lpdis->hDC, TRANSPARENT );
            TextOut( lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top,
                     lpData, zstrlen( lpData ) );
            SetBkMode( lpdis->hDC, nBkModeOld );

            if ( clrBkOld != CLR_INVALID )
               SetBkColor( lpdis->hDC, clrBkOld );

            if ( clrTextOld != CLR_INVALID )
               SetTxColor( lpdis->hDC, clrTextOld );
         }

         if ( lpdis->itemState & ODS_SELECTED )
            DrawFocusRect( lpdis->hDC, &lpdis->rcItem );

         return( 0 );
   }

   return( SS_DefProc( g_lpfnComboBoxProc, GetParent( hWnd ),
                       hWnd, uMsg, wParam, lParam ) );
}


zLONG OPERATION
kzCheckBoxWndProc( HWND    hWnd,
                   WORD    uMsg,
                   WPARAM  wParam,
                   LPARAM  lParam )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzCheckBoxWndProc" );
   HDC            hDCMemory;
   PAINTSTRUCT    Paint;
   HBITMAP        hBitmapOld;
   LPCHECKBOX     lpCheckBox;
   HFONT          hFont;
// zPCHAR         Text;
   RECT           Rect;
   HDC            hDC;
   zLONG          lRet;
   zSHORT         nLth;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "kzCheckBoxWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_CREATE:
         CheckBoxLoadDefBitmaps( g_hInstanceDLL );
         CheckBoxCreate( hWnd, lParam );
         return( 0 );

      case WM_GETDLGCODE:
         return( DLGC_BUTTON );

      case WM_ENABLE:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            CheckBoxSetDisabled( hWnd, lpCheckBox, (zBOOL) !wParam );
//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( 0 );

      case WM_SETFOCUS:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            CheckBoxSetFocus( hWnd, lpCheckBox, TRUE );
//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( 0 );

      case WM_KILLFOCUS:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            CheckBoxSetFocus( hWnd, lpCheckBox, FALSE );
            CheckBoxSetButtonDown( hWnd, lpCheckBox, FALSE );
//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( 0 );

      case WM_KEYDOWN:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( wParam == VK_SPACE && lpCheckBox->fMouseCaptureOn == 0 &&
                 !(HIBYTE( HIWORD( lParam ) ) & 0x0040) )
            {
               CheckBoxSetButtonDown( hWnd, lpCheckBox, TRUE );
            }

//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_CHAR:
         if ( wParam != VK_SPACE && wParam != VK_TAB &&
              wParam != VK_SHIFT && wParam != VK_CONTROL )
         {
            PostMessage( GetParent( hWnd ), WM_COMMAND, GetDlgCtrlID( hWnd ),
                         MAKELONG( hWnd, BN_CHECKBOXTYPING ) );
         }

         break;

      case WM_USER + 99:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            CheckBoxSetButtonDown( hWnd, lpCheckBox, FALSE );
            if ( (lpCheckBox->lStyle & 0x0F) == BS_AUTO3STATE )
            {
               if ( lpCheckBox->fDisabled )
                  SendMessage( hWnd, BM_SETCHECK, 0, 0L );
               else
                  SendMessage( hWnd, BM_SETCHECK, lpCheckBox->fChecked + 1, 0L );
            }
            else
            if ( (lpCheckBox->lStyle & 0x0F) == BS_AUTOCHECKBOX )
               CheckBoxSetCheck( hWnd, lpCheckBox, !lpCheckBox->fChecked );

            if ( (lpCheckBox->lStyle & 0x0F) == BS_AUTORADIOBUTTON )
               SendMessage( hWnd, BM_SETCHECK, !lpCheckBox->fChecked, 0L );

            if ( lpCheckBox->fFocus )
               PostMessage( GetParent( hWnd ), WM_COMMAND, GetDlgCtrlID( hWnd ),
                            MAKELONG( hWnd, BN_CLICKED ) );

//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_KEYUP:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( wParam == VK_SPACE && lpCheckBox->fMouseCaptureOn == 0 &&
                 lpCheckBox->fButtonDown )
            {
               CheckBoxSetButtonDown( hWnd, lpCheckBox, FALSE );

               if ( ((lpCheckBox->lStyle & 0x0F) == BS_AUTOCHECKBOX) ||
                    ((lpCheckBox->lStyle & 0x0F) == BS_AUTORADIOBUTTON) )
               {
                  CheckBoxSetCheck( hWnd, lpCheckBox, !lpCheckBox->fChecked );
               }
               else
               if ( (lpCheckBox->lStyle & 0x0F) == BS_AUTO3STATE )
               {
                  if ( lpCheckBox->fDisabled )
                     SendMessage( hWnd, BM_SETCHECK, 0, 0L );
                  else
                     SendMessage( hWnd, BM_SETCHECK,
                                  lpCheckBox->fChecked + 1, 0L );
               }

               if ( lpCheckBox->fFocus )
                  PostMessage( GetParent( hWnd ), WM_COMMAND,
                               GetDlgCtrlID( hWnd ),
                               MAKELONG( hWnd, BN_CLICKED ) );
            }

//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_MOUSEACTIVATE:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            SetFocus( hWnd );
//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_LBUTTONDOWN:
      // TraceLineS( "kzCheckBoxWndProc", " WM_LBUTTONDOWN" );
      case WM_LBUTTONDBLCLK:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( lpCheckBox->fMouseCaptureOn == 0 )
            {
               SetCapture( hWnd );
               lpCheckBox->fMouseCaptureOn = TRUE;
               CheckBoxSetButtonDown( hWnd, lpCheckBox, TRUE );
            }

//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_LBUTTONUP:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( lpCheckBox->fMouseCaptureOn )
            {
               ReleaseCapture( );
               lpCheckBox->fMouseCaptureOn = FALSE;

               CheckBoxSetButtonDown( hWnd, lpCheckBox, FALSE );
               GetClientRect( hWnd, &Rect );

               POINT pt;
               pt.x = LOWORD( lParam );
               pt.y = HIWORD( lParam );
               if ( PtInRect( &Rect, pt ) )
               {
                  if ( ((lpCheckBox->lStyle & 0x0F) == BS_AUTOCHECKBOX) ||
                       ((lpCheckBox->lStyle & 0x0F) == BS_AUTORADIOBUTTON) )
                  {
                     CheckBoxSetCheck( hWnd, lpCheckBox,
                                       !lpCheckBox->fChecked );
                  }
                  else
                  if ( (lpCheckBox->lStyle & 0x0F) == BS_AUTO3STATE )
                  {
                     if ( lpCheckBox->fDisabled )
                        SendMessage( hWnd, BM_SETCHECK, 0, 0L );
                     else
                        SendMessage( hWnd, BM_SETCHECK, lpCheckBox->fChecked + 1, 0L );
                  }

                  PostMessage( GetParent( hWnd ), WM_COMMAND,
                               GetWindowLong( hWnd, GWL_ID ),
                               MAKELONG( hWnd, BN_CLICKED ) );
               }
            }

//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_MOUSEMOVE:
         if ( wParam & MK_LBUTTON )
         {
            if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
            {
               if ( lpCheckBox->fMouseCaptureOn )
               {
                  GetClientRect( hWnd, &Rect );

                  POINT pt;
                  pt.x = LOWORD( lParam );
                  pt.y = HIWORD( lParam );
                  if ( PtInRect( &Rect, pt ) )
                     CheckBoxSetButtonDown( hWnd, lpCheckBox, TRUE );
                  else
                     CheckBoxSetButtonDown( hWnd, lpCheckBox, FALSE );
               }

//             DrUnlockTaskMemory( lpCheckBox );
            }
         }

         break;

      case BM_SETPICT:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( lpCheckBox->fUseDefPicts )
            {
               zmemset( &lpCheckBox->Picts, 0, sizeof( CHECKBOXPICTS ) );
               lpCheckBox->fUseDefPicts = FALSE;
            }

            CheckBoxSetPict( hWnd, 0, &lpCheckBox->Picts, wParam, lParam );
//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( 0 );

      case BM_SETSTATE:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            CheckBoxSetCheck( hWnd, lpCheckBox, wParam );
//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( 0 );

      case BM_SETCHECK:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( ((lpCheckBox->lStyle & 0x0F) == BS_3STATE) ||
                 ((lpCheckBox->lStyle & 0x0F) == BS_AUTO3STATE) )
            {
               switch ( wParam )
               {
                  case 0:
                     CheckBoxSetDisabled( hWnd, lpCheckBox, FALSE );
                     CheckBoxSetCheck( hWnd, lpCheckBox, FALSE );
                     break;

                  case 1:
                     CheckBoxSetDisabled( hWnd, lpCheckBox, FALSE );
                     CheckBoxSetCheck( hWnd, lpCheckBox, TRUE );
                     break;

                  case 2:
                     CheckBoxSetDisabled( hWnd, lpCheckBox, TRUE );
                     break;
               }
            }
            else
               CheckBoxSetCheck( hWnd, lpCheckBox, wParam );

//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( 0 );

      case BM_GETSTATE:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            lRet = (zLONG) lpCheckBox->fChecked;
//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( lRet );

      case BM_GETCHECK:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( ((lpCheckBox->lStyle & 0x0F) == BS_3STATE) ||
                 ((lpCheckBox->lStyle & 0x0F) == BS_AUTO3STATE) )
            {
               if ( lpCheckBox->fDisabled )
                  lRet = 2;
               else
                  lRet = (zLONG) lpCheckBox->fChecked;
            }
            else
               lRet = (zLONG) lpCheckBox->fChecked;

//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( lRet );

      case BM_SETSTYLE:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            lpCheckBox->lStyle = (zLONG) wParam;
//          DrUnlockTaskMemory( lpCheckBox );

            if ( lParam )
               InvalidateRect( hWnd, 0, TRUE );
         }

         return( 0 );

      case WM_SIZE:
      // TraceLineS( "kzCheckBoxWndProc WM_SIZE", "" );
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            GetClientRect( hWnd, &Rect );

            if ( lpCheckBox->hBitmapDC )
               DeleteBitmap( lpCheckBox->hBitmapDC );

            // Create Bitmap to select to the memory device context.

            hDC = GetDC( hWnd );
            lpCheckBox->hBitmapDC =
               kzCreateCompatibleBitmap( hDC, Rect.right - Rect.left,
                                         Rect.bottom - Rect.top );
            ReleaseDC( hWnd, hDC );

//          DrUnlockTaskMemory( lpCheckBox );
         }

         break;

      case WM_CLOSE:
         DestroyWindow( hWnd );
         return( 0 );

      // This message is received when the window is destroyed,
      // either under program control or external direction.
      case WM_DESTROY:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            DeleteBrush( lpCheckBox->hBrushFocus );
            DeleteBitmap( lpCheckBox->hBitmapFocus );

            if ( lpCheckBox->hText )
            {
               lpCheckBox->hText =
                  (zPCHAR) DrUnlockTaskMemory( lpCheckBox->hText );
               DrFreeTaskMemory( lpCheckBox->hText );
               lpCheckBox->hText = 0;
            }

            if ( lpCheckBox->fUseDefPicts == 0 )
            {
               CheckBoxFreePict( &lpCheckBox->Picts.Pict );
               CheckBoxFreePict( &lpCheckBox->Picts.PictBtnDown );
               CheckBoxFreePict( &lpCheckBox->Picts.PictFocus );
               CheckBoxFreePict( &lpCheckBox->Picts.PictFocusDown );
               CheckBoxFreePict( &lpCheckBox->Picts.PictDisabled );
               CheckBoxFreePict( &lpCheckBox->Picts.PictDisabledDown );
            }

            if ( lpCheckBox->hBitmapDC )
               DeleteBitmap( lpCheckBox->hBitmapDC );

            lpCheckBox = (LPCHECKBOX) DrUnlockTaskMemory( lpCheckBox );
            DrFreeTaskMemory( lpCheckBox );
            CheckBoxSetPtr( hWnd, 0 );
         }

         CheckBoxUnloadDefBitmaps( );
         return( 0 );

      case WM_SETFONT:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            lpCheckBox->hFont = (HFONT) wParam;
//          DrUnlockTaskMemory( lpCheckBox );

            if ( lParam )
            {
               InvalidateRect( hWnd, 0, TRUE );
               UpdateWindow( hWnd );
            }
         }

         return( 0 );

      case WM_GETFONT:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( lpCheckBox->hFont )
               hFont = lpCheckBox->hFont;
            else
            {
               hDC = GetDC( hWnd );
               hFont = SelectFont( hDC, (HFONT)
                                   GetStockObject( SYSTEM_FIXED_FONT ) );
               SelectFont( hDC, hFont );
               ReleaseDC( hWnd, hDC );
            }

//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( (zLONG) hFont );

      case WM_SETTEXT:
         CheckBoxSetText( hWnd, (zPCHAR) lParam );
         return( 0 );

      case WM_GETTEXT:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( lpCheckBox->hText )
            {
//             Text = (zPCHAR) DrLockTaskMemory( lpCheckBox->hText );
               nLth = zstrlen( lpCheckBox->hText );

               nLth = mMin( nLth + 1, (zSHORT) wParam );
               zmemcpy( (zPCHAR) lParam, lpCheckBox->hText, nLth );
//             DrUnlockTaskMemory( Text );
            }
            else
               nLth = 0;

//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( nLth );

      case WM_GETTEXTLENGTH:
         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            if ( lpCheckBox->hText )
            {
//             Text = (zPCHAR) DrLockTaskMemory( lpCheckBox->hText );
               nLth = zstrlen( lpCheckBox->hText );
//             DrUnlockTaskMemory( Text );
            }
            else
               nLth = 0;

//          DrUnlockTaskMemory( lpCheckBox );
         }

         return( nLth );

      case WM_ERASEBKGND:
         return( TRUE );

      case WM_PAINT:
         hDC = BeginPaint( hWnd, &Paint );
         GetClientRect( hWnd, &Rect );

         if ( (lpCheckBox = CheckBoxGetPtr( hWnd )) != 0 )
         {
            // Create a memory device context.
            hDCMemory = CreateCompatibleDC( hDC );
            hBitmapOld = SelectBitmap( hDCMemory, lpCheckBox->hBitmapDC );
            int nMapModeOld = SetMapMode( hDCMemory, MM_TEXT );
            int nBkModeOld = SetBkMode( hDCMemory, TRANSPARENT );

            CheckBoxPaint( hWnd, lpCheckBox, hDCMemory, &Rect, FALSE, 0 );

            // Copy the memory device context bitmap to the display.
            BitBlt( hDC, Paint.rcPaint.left, Paint.rcPaint.top,
                    Paint.rcPaint.right - Paint.rcPaint.left,
                    Paint.rcPaint.bottom - Paint.rcPaint.top,
                    hDCMemory, Paint.rcPaint.left, Paint.rcPaint.top, SRCCOPY );

            // Clean up.
            SetBkMode( hDCMemory, nBkModeOld );
            SetMapMode( hDCMemory, nMapModeOld );
            SelectBitmap( hDCMemory, hBitmapOld );
            DeleteDC( hDCMemory );

//          DrUnlockTaskMemory( lpCheckBox );
         }

         EndPaint( hWnd, &Paint );
         return( 0 );
   }

   return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

LRESULT CALLBACK
SS_EditComboBoxWndProc( HWND    hWnd,
                        WORD    uMsg,
                        WPARAM  wParam,
                        LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_COMBO.CPP>", "SS_EditComboBoxProc" );
// TraceLineI( "SS_EditComboBoxProc uMsg: ", uMsg );
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_EditComboBoxWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_SYSKEYDOWN:
         kzSSComboBoxWndProc( GetParent( hWnd ), uMsg, wParam, lParam );
         return( 0 );

      case WM_KEYDOWN:
         switch ( wParam )
         {
            case VK_DELETE:
               SendMessage( GetParent( hWnd ), CB_SETCURSEL, (WPARAM)-1, 0L );
               break;

            case VK_UP:
            case VK_DOWN:
               kzSSComboBoxWndProc( GetParent( hWnd ), uMsg, wParam, lParam );
               return( 0 );
         }

         break;

      case WM_CHAR:
         SendMessage( GetParent( hWnd ), CB_SETCURSEL, (WPARAM) -1, 0L );
         break;

      case WM_SETFOCUS:
         SS_ComboSubclassDesktop( GetParent( GetParent( hWnd ) ) );
         break;

      case WM_KILLFOCUS:
         SS_ComboUnSubclassDesktop( );
      // TraceLineS( "SS_EditComboBoxProc: ", "MoveWindow" );
         MoveWindow( hWnd, 0, 0, 0, 0, FALSE );
         ShowWindow( hWnd, SW_HIDE );
         break;
   }

   return( SS_DefProc( g_lpfnEditComboBoxProc,
                       GetParent( GetParent( hWnd ) ),
                       hWnd, uMsg, wParam, lParam ) );
}

#define SS_CB_ISDROPDOWN( lStyle ) ((lStyle & 0x03) == SS_CB_DROPDOWN)

// This WndProc is what gets called once the spread sheet makes a
// row/column into a combo box.
LRESULT CALLBACK
kzSSComboBoxWndProc( HWND    hWnd,
                     WORD    uMsg,
                     WPARAM  wParam,
                     LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_COMBO.CPP>", "kzSSComboBoxWndProc" );
// SS_CLRTBLITEM BackClrTblItem;
// SS_CLRTBLITEM ForeClrTblItem;
   LPSPREADSHEET   lpSS;
   SS_CELLTYPE     CellType;
   PAINTSTRUCT     Paint;
   zPCHAR          hText;
   HBITMAP         hBitmapOld;
   HBITMAP         hBitmapDC;
   LPSS_CELL       Cell;
   LPSS_FONT       Font;
   zPCHAR          lpText;
   SS_FONT         FontTemp;
   HDC             hDCMemory;
   HDC             hDC;
   RECT            Rect;
   HWND            hWndCtrl = 0;
   HWND            hWndEdit;
   zLONG           lCnt;
   zLONG           lSel;
   zLONG           lSelOld;
   zSHORT          nLth;
   zLONG           x;
   zLONG           y;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "kzSSComboBoxWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_CREATE:
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSListBox, 0,
                                  WS_BORDER | WS_CHILD | WS_VSCROLL,
                                  0, 0, 0, 0, GetDesktopWindow( ), 0,
                                  g_hInstanceDLL, 0 );

         if ( hWndCtrl )
         {
            SetProp( hWndCtrl, "HWNDSPREAD", GetParent( hWnd ) );
            SetProp( hWndCtrl, "HWNDCOMBO", hWnd );

            SendMessage( hWndCtrl, WM_SETFONT,
                         (WPARAM) SendMessage( hWnd, WM_GETFONT, 0, 0L ), 0L );

            if ( (hWndEdit = CreateWindow( SSClassNames[ 0 ].SSEditComboBox,
                                           0, WS_CHILD | ES_AUTOHSCROLL,
                                           0, 0, 0, 0, hWnd, 0,
                                           g_hInstanceDLL, 0 )) != 0 )
            {
               SendMessage( hWndEdit, EM_LIMITTEXT, 150, 0L );
               SetWindowLong( hWnd, GWL_HWNDEDIT, (zLONG) hWndEdit );
            }

            SetWindowLong( hWnd, GWL_LISTBOX, (zLONG) hWndCtrl );
            SetWindowLong( hWnd, GWL_BTNDOWN, FALSE );
         }

         break;

      case WM_SYSKEYDOWN:
         hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
         if ( hWndCtrl &&
              (lCnt = (zLONG) SendMessage( hWndCtrl, LB_GETCOUNT,
                                           0, 0L )) > 0 )
         {
            lpSS = SS_Lock( GetParent( hWnd ) );

            if ( ((lParam >> 29) & 0x01) &&
                  (wParam == VK_UP || wParam == VK_DOWN) )
            {
               if ( !GetWindowLong( hWnd, GWL_BTNDOWN ) )
               {
                  lpSS->EditModeTurningOn = TRUE;
                  SendMessage( hWnd, CB_SHOWDROPDOWN, TRUE, 0L );
                  lpSS->EditModeTurningOn = FALSE;
               }
               else
               {
                  SetWindowLong( hWnd, GWL_BTNDOWN, FALSE );
                  hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
                  ShowWindow( hWndCtrl, SW_HIDE );
                  InvalidateRect( hWnd, 0, FALSE );
                  UpdateWindow( hWnd );
               }
            }

//          DrUnlockTaskMemory( lpSS );
         }

         break;

      case WM_KEYDOWN:
         hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
         if ( hWndCtrl == 0 || GetWindowLong( hWnd, GWL_BTNDOWN ) )
         {
            if ( hWndCtrl )
               SendMessage( hWndCtrl, uMsg, wParam, lParam );

            return( 0 );
         }

         if ( (lCnt = (zLONG) SendMessage( hWndCtrl, LB_GETCOUNT,
                                           0, 0L )) > 0 )
         {
            lSel = SendMessage( hWndCtrl, LB_GETCURSEL, 0, 0L );
            lSelOld = lSel;

            switch ( wParam )
            {
               case VK_UP:
               case VK_LEFT:
                  if ( lSel != 0 )
                  {
                     lSel = mMax( lSel - 1, 0 );
                     SendMessage( hWndCtrl, LB_SETCURSEL, lSel, 0L );
                  }

                  break;

               case VK_DOWN:
               case VK_RIGHT:
                  if ( lSel != lCnt - 1 )
                  {
                     lSel++;
                     SendMessage( hWndCtrl, LB_SETCURSEL, lSel, 0L );
                  }

                  break;

               case VK_HOME:
                  if ( lSel != 0 )
                  {
                     lSel = 0;
                     SendMessage( hWndCtrl, LB_SETCURSEL, lSel, 0L );
                  }

                  break;

               case VK_END:
                  if ( lSel != lCnt - 1 )
                  {
                     lSel = lCnt - 1;
                     SendMessage( hWndCtrl, LB_SETCURSEL, lSel, 0L );
                  }

                  break;
            }

            if ( lSelOld != lSel )
            {
               nLth = (zSHORT) SendMessage( hWndCtrl, LB_GETTEXTLEN,
                                            lSel, 0L );

               if ( DrAllocTaskMemory( &hText, nLth + 1, 1002 ) == 0 )
               {
                  lpText = (zPCHAR) DrLockTaskMemory( hText );

                  SendMessage( hWndCtrl, LB_GETTEXT, lSel, (zLONG) lpText );
                  SendMessage( hWnd, WM_SETTEXT, 0, (zLONG) lpText );

                  lpSS = SS_Lock( GetParent( hWnd ) );

                  SS_RetrieveCellType( lpSS, &CellType, 0,
                                       lpSS->Col.ssCurrAt,
                                       lpSS->Row.ssCurrAt );
//                DrUnlockTaskMemory( lpSS );

                  if ( SS_CB_ISDROPDOWN( CellType.lStyle ) )
                  {
                     hWndEdit = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
                     SetWindowText( hWndEdit, lpText );
                  }

                  DrUnlockTaskMemory( lpText );
                  DrFreeTaskMemory( hText );
               }

               InvalidateRect( hWnd, 0, FALSE );
               UpdateWindow( hWnd );
            }
         }

         break;

      case WM_CHAR:
         lpSS = SS_Lock( GetParent( hWnd ) );
         SS_RetrieveCellType( lpSS, &CellType, 0,
                              lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
//       DrUnlockTaskMemory( lpSS );

         if ( SS_CB_ISDROPDOWN( CellType.lStyle ) )
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
         else
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );

         if ( hWndCtrl )
            SendMessage( hWndCtrl, uMsg, wParam, lParam );

         if ( SS_CB_ISDROPDOWN( CellType.lStyle ) == 0 )
         {
            SendMessage( hWnd, WM_COMMAND, 0, MAKELONG( 0, LBN_SELCHANGE ) );
            InvalidateRect( hWnd, 0, FALSE );
            UpdateWindow( hWnd );
         }

         return( 0 );

      case WM_SETFOCUS:
         lpSS = SS_Lock( GetParent( hWnd ) );
         hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
         if ( hWndCtrl )
            SetProp( hWndCtrl, "HWNDSPREAD", GetParent( hWnd ) );

         SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt,
                              lpSS->Row.ssCurrAt );

         if ( SS_CB_ISDROPDOWN( CellType.lStyle ) )
         {
            GetClientRect( hWnd, &Rect );
            Cell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt,
                                    lpSS->Row.ssCurrAt );
            Font = SS_RetrieveFont( lpSS, &FontTemp, Cell,
                                    lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
            SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt,
                               lpSS->Row.ssCurrAt );

            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
            if ( hWndCtrl )
            {
               if ( Font->hFont )
                  SendMessage( hWndCtrl, WM_SETFONT,
                               (WPARAM) Font->hFont, 0L );

               nLth = (zSHORT) DefWindowProc( hWnd, WM_GETTEXTLENGTH, 0, 0L );
               if ( DrAllocTaskMemory( &hText, nLth + 1, 1003 ) == 0 )
               {
                  lpText = (zPCHAR) DrLockTaskMemory( hText );

                  DefWindowProc( hWnd, WM_GETTEXT, nLth + 1, (LPARAM) lpText );
                  SendMessage( hWndCtrl, WM_SETTEXT, 0, (LPARAM) lpText );

                  DrUnlockTaskMemory( lpText );
                  DrFreeTaskMemory( hText );
               }

            // TraceLineS( "kzSSComboBoxWndProc: ", "MoveWindow" );
               MoveWindow( hWndCtrl, Rect.left + 2, Rect.top + 1,
                           Rect.right - lpSS->nComboButtonBitmapWidth - 2,
                           Rect.bottom - 1, TRUE );

               ShowWindow( hWndCtrl, SW_SHOWNORMAL );
               UpdateWindow( hWndCtrl );
               SetFocus( hWndCtrl );

               if ( lpSS->fEditModeReplace )
                  SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( 0, nLth ) );
               else
                  SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( nLth, nLth ) );
            }
         }
         else
            SS_ComboSubclassDesktop( GetParent( hWnd ) );

//       DrUnlockTaskMemory( lpSS );
         break;

      case WM_KILLFOCUS:
         lpSS = SS_Lock( GetParent( hWnd ) );
         if ( lpSS->EditModeTurningOn == 0 )
         {
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
            if ( hWndCtrl && GetFocus( ) != hWndCtrl )
            {
               SetWindowLong( hWnd, GWL_BTNDOWN, FALSE );
               ShowWindow( hWndCtrl, SW_HIDE );
               InvalidateRect( hWnd, 0, FALSE );
               UpdateWindow( hWnd );
            }

            SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt,
                                 lpSS->Row.ssCurrAt );

            if ( SS_CB_ISDROPDOWN( CellType.lStyle ) == 0 )
               SS_ComboUnSubclassDesktop( );
         }

//       DrUnlockTaskMemory( lpSS );
         break;

      case WM_GETTEXT:
      case WM_GETTEXTLENGTH:
         lpSS = SS_Lock( GetParent( hWnd ) );
         SS_RetrieveCellType( lpSS, &CellType, 0,
                              lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
//       DrUnlockTaskMemory( lpSS );

         if ( SS_CB_ISDROPDOWN( CellType.lStyle ) )
         {
            hWndEdit = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
            return( SendMessage( hWndEdit, uMsg, wParam, lParam ) );
         }

         break;

      case WM_DESTROY:
         hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
         if ( hWndCtrl )
         {
            RemoveProp( hWndCtrl, "HWNDSPREAD" );
            RemoveProp( hWndCtrl, "HWNDCOMBO" );
            DestroyWindow( hWndCtrl );
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
            DestroyWindow( hWndCtrl );
         }

         break;

      case WM_MOUSEACTIVATE:
         if ( GetParent( GetFocus( ) ) != hWnd )
            SetFocus( hWnd );

         return( 0 );

      case WM_LBUTTONDOWN:
      // TraceLineS( "kzSSComboBoxWndProc", " WM_LBUTTONDOWN" );
      case WM_LBUTTONDBLCLK:
         GetClientRect( hWnd, &Rect );

         x = LOWORD( lParam );
         y = HIWORD( lParam );

         lpSS = SS_Lock( GetParent( hWnd ) );

         SS_RetrieveCellType( lpSS, &CellType, 0,
                              lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

         if ( SS_CB_ISDROPDOWN( CellType.lStyle ) == 0 ||
              (x >= Rect.right - lpSS->nComboButtonBitmapWidth &&
               x < Rect.right && y > 0 && y < Rect.bottom) )
         {
            if ( !GetWindowLong( hWnd, GWL_BTNDOWN ) )
            {
               lpSS->EditModeTurningOn = TRUE;
               SendMessage( hWnd, CB_SHOWDROPDOWN, TRUE, 0L );
               lpSS->EditModeTurningOn = FALSE;
            }
            else
            {
               SetWindowLong( hWnd, GWL_BTNDOWN, FALSE );
               hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
               if ( hWndCtrl )
               {
                  ShowWindow( hWndCtrl, SW_HIDE );
                  InvalidateRect( hWnd, 0, FALSE );
                  UpdateWindow( hWnd );
               }
            }
         }

//       DrUnlockTaskMemory( lpSS );
         break;

      case WM_SHOWWINDOW:
         if ( wParam == 0 )
         {
            SetWindowLong( hWnd, GWL_BTNDOWN, FALSE );
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
            if ( hWndCtrl )
               ShowWindow( hWndCtrl, SW_HIDE );
         }

         break;

#ifndef __WIN32__
      case WM_CTLCOLOR:
         lpSS = SS_Lock( GetParent( hWnd ) );

         SS_GetClrTblItem( GetParent( hWnd ), lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, &BackClrTblItem, &ForeClrTblItem );
         SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.ShadowLightId );

//       DrUnlockTaskMemory( lpSS );

         SetBkColor( (HDC) wParam, BackClrTblItem.Color );
         SetTxColor( (HDC) wParam, ForeClrTblItem.Color );

         return( (zLONG) BackClrTblItem.hBrush );
#endif

      case CB_RESETCONTENT:
      case CB_ADDSTRING:
      case CB_GETCURSEL:
      case CB_SETCURSEL:
         hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
         if ( hWndCtrl )
         {
            if ( uMsg == CB_RESETCONTENT )
               uMsg = LB_RESETCONTENT;
            else
            if ( uMsg == CB_ADDSTRING )
               uMsg = LB_ADDSTRING;
            else
            if ( uMsg == CB_GETCURSEL )
               uMsg = LB_GETCURSEL;
            else
            if ( uMsg == CB_SETCURSEL )
               uMsg = LB_SETCURSEL;

            return( SendMessage( hWndCtrl, uMsg, wParam, lParam ) );
         }

         return( 0 );

      case CB_SHOWDROPDOWN:
      {
#if 1
         TEXTMETRIC fm;
         zLONG      lCurSel;
         HFONT      hFontOld;
         zLONG      lHeight;
         zLONG      lScreenHeight;
         zLONG      lItemCnt;
         zSHORT     nItemsShown;

         if ( GetFocus( ) != hWnd && GetFocus( ) != (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT ) )
         {
            return( 0 );
         }

         hWndEdit = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
         hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
//       SetWindowText( hWndEdit, "DKS" );
         if ( hWndCtrl &&
              (lCnt = (zLONG) SendMessage( hWndCtrl, LB_GETCOUNT, 0, 0L )) > 0 )
         {
            SetWindowLong( hWnd, GWL_BTNDOWN, TRUE );
            InvalidateRect( hWnd, 0, FALSE );
            UpdateWindow( hWnd );

            lpSS = SS_Lock( GetParent( hWnd ) );

            Cell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
            Font = SS_RetrieveFont( lpSS, &FontTemp, Cell, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
            SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
            if ( hWndCtrl == 0 )
               break;

            if ( Font->hFont )
               SendMessage( hWndCtrl, WM_SETFONT, (WPARAM) Font->hFont, 0L );

//          DrUnlockTaskMemory( lpSS );

            lItemCnt = (zLONG) SendMessage( hWndCtrl, LB_GETCOUNT, 0, 0L );
            GetWindowRect( hWnd, &Rect );

            hDC = GetDC( hWndCtrl );
            if ( Font->hFont )
               hFontOld = SelectFont( hDC, Font->hFont );
            else
               hFontOld = 0;

            GetTextMetrics( hDC, &fm );

            nItemsShown = mMin( lItemCnt, 6 );
            lHeight = ((zSHORT) fm.tmHeight * nItemsShown) + 2;
            if ( hFontOld )
               SelectFont( hDC, hFontOld );

            ReleaseDC( hWndCtrl, hDC );

            lScreenHeight = GetSystemMetrics( SM_CYSCREEN );

            if ( Rect.bottom + lHeight >= lScreenHeight )
               Rect.top = mMax( Rect.top - lHeight, 0 );
            else
               Rect.top = Rect.bottom;

            Rect.bottom = Rect.top + lHeight;

            lCurSel = (zLONG) SendMessage( hWndCtrl, LB_GETCURSEL, 0, 0L );
//          TraceLineI( "SHOWDROPDOWN lCurSel: ", lCurSel );
            if ( lCurSel <= nItemsShown )
               SendMessage( hWndCtrl, LB_SETTOPINDEX, 0, 0L );

         // TraceLineS( "kzSSComboBoxWndProc: ", "MoveWindow" );
            MoveWindow( hWndCtrl, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, TRUE );
         // TraceLineS( "kzSSComboBoxWndProc: ", "SetWindowPos" );
            SetWindowPos( hWndCtrl, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            SendMessage( hWndCtrl, LB_SETCURSEL, lCurSel, 0L );

            nLth = (zSHORT) SendMessage( hWndCtrl, LB_GETTEXTLEN, lCurSel, 0L );

            if ( DrAllocTaskMemory( &hText, nLth + 1, 1004 ) == 0 )
            {
               lpText = (zPCHAR) DrLockTaskMemory( hText );

               SendMessage( hWndCtrl, LB_GETTEXT, lCurSel, (zLONG) lpText );
//             TraceLineS( "SHOWDROPDOWN Selected Text: ", lpText );
               SendMessage( hWnd, WM_SETTEXT, 0, (zLONG) lpText );
#if 0
               SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

               if ( SS_CB_ISDROPDOWN( CellType.lStyle ) )
               {
//                hWndEdit = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
                  SetWindowText( hWndEdit, lpText );
               }

               SendMessage( hWnd, CB_SETCURSEL, lCurSel, 0 );
#endif

               DrUnlockTaskMemory( lpText );
               DrFreeTaskMemory( hText );
            }
         }

         return( 0 );
#endif
         break;
      }

      case WM_COMMAND:
         if ( HIWORD( lParam ) == LBN_SELCHANGE )
         {
            SendMessage( hWnd, WM_SETTEXT, 0, (zLONG)(zPCHAR) "" );
            hWndCtrl = (HWND) GetWindowLong( hWnd, GWL_LISTBOX );
            if ( hWndCtrl &&
                 (lSel = (zLONG) SendMessage( hWndCtrl, LB_GETCURSEL, 0, 0L )) != LB_ERR )
            {
               nLth = (zSHORT) SendMessage( hWndCtrl, LB_GETTEXTLEN, lSel, 0L );
               if ( DrAllocTaskMemory( &hText, nLth + 1, 1005 ) == 0 )
               {
                  lpText = (zPCHAR) DrLockTaskMemory( hText );

                  SendMessage( hWndCtrl, LB_GETTEXT, lSel, (zLONG) lpText );
                  SendMessage( hWnd, WM_SETTEXT, 0, (zLONG) lpText );

                  lpSS = SS_Lock( GetParent( hWnd ) );

                  SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
//                DrUnlockTaskMemory( lpSS );

                  if ( SS_CB_ISDROPDOWN( CellType.lStyle ) )
                  {
                     hWndEdit = (HWND) GetWindowLong( hWnd, GWL_HWNDEDIT );
                     SetWindowText( hWndEdit, lpText );
                  }

                  DrUnlockTaskMemory( lpText );
                  DrFreeTaskMemory( hText );
               }
            }

            InvalidateRect( hWnd, 0, TRUE );
            UpdateWindow( hWnd );
         }

         return( 0 );

      case WM_ERASEBKGND:
         return( TRUE );

      case WM_PAINT:
      {
         hDC = BeginPaint( hWnd, &Paint );
         GetClientRect( hWnd, &Rect );

         // Create a memory device context.
         hBitmapDC = kzCreateCompatibleBitmap( hDC, Rect.right - Rect.left,
                                               Rect.bottom - Rect.top );

         hDCMemory = CreateCompatibleDC( hDC );
         hBitmapOld = SelectBitmap( hDCMemory, hBitmapDC );

         SetRect( &Rect, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top );
         SS_ComboPaint( hWnd, hDCMemory, &Rect );

         // Copy the memory device context bitmap to the display.
         BitBlt( hDC, Paint.rcPaint.left, Paint.rcPaint.top,
                 Paint.rcPaint.right - Paint.rcPaint.left,
                 Paint.rcPaint.bottom - Paint.rcPaint.top,
                 hDCMemory, Paint.rcPaint.left, Paint.rcPaint.top, SRCCOPY );

         // Clean up.
         SelectBitmap( hDCMemory, hBitmapOld );
         DeleteDC( hDCMemory );
         DeleteBitmap( hBitmapDC );

         EndPaint( hWnd, &Paint );
         return( 0 );
      }
   }

   return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

void
CheckBoxSetPtr( HWND    hWnd,
                zPCHAR  hCheckBox )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetPtr" );
   SetWindowLong( hWnd, zGWL_CHECKBOX, (zLONG) hCheckBox );
}

LPCHECKBOX
CheckBoxInit( )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxInit" );
   LPCHECKBOX   lpCheckBox;
   static WORD  Bits[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};

   if ( DrAllocTaskMemory( (zCOREMEM) &lpCheckBox, (zLONG) sizeof( CHECKBOX ), 1006 ) != 0 )
   {
      return( 0 );
   }

   lpCheckBox = (LPCHECKBOX) DrLockTaskMemory( lpCheckBox );

   lpCheckBox->hBitmapFocus = kzCreateBitmap( 8, 8, 1, 1, (zPCHAR) Bits );
   lpCheckBox->hBrushFocus = kzCreatePatternBrush( lpCheckBox->hBitmapFocus );
   return( lpCheckBox );
}

LPCHECKBOX
CheckBoxGetPtr( HWND    hWnd )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxGetPtr" );
   return( (LPCHECKBOX) GetWindowLong( hWnd, zGWL_CHECKBOX ) );
}

LPBTNPICT
CheckBoxGetPicturePtr( LPCHECKBOXPICTS lpPicts,
                       WORD            wPictType )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxGetPicturePtr" );
   LPBTNPICT  lpPict;

   switch ( wPictType )
   {
      case BT_PICT_UP:
         lpPict = &lpPicts->Pict;
         break;

      case BT_PICT_DOWN:
         lpPict = &lpPicts->PictBtnDown;
         break;

      case BT_PICT_FOCUSUP:
         lpPict = &lpPicts->PictFocus;
         break;

      case BT_PICT_FOCUSDOWN:
         lpPict = &lpPicts->PictFocusDown;
         break;

      case BT_PICT_DISABLEDUP:
         lpPict = &lpPicts->PictDisabled;
         break;

      case BT_PICT_DISABLEDDOWN:
         lpPict = &lpPicts->PictDisabledDown;
         break;
   }

   return( lpPict );
}

void
CheckBoxSetDisabled( HWND       hWnd,
                     LPCHECKBOX lpCheckBox,
                     zBOOL      fValue )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetDisabled" );
   if ( lpCheckBox->fDisabled != fValue )
   {
      lpCheckBox->fDisabled = fValue;
      InvalidateRect( hWnd, 0, FALSE );
   }
}

void
CheckBoxSetFocus( HWND       hWnd,
                  LPCHECKBOX lpCheckBox,
                  zBOOL      fValue )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetFocus" );
   if ( lpCheckBox->fFocus != fValue )
   {
      lpCheckBox->fFocus = fValue;
      InvalidateRect( hWnd, 0, FALSE );
      UpdateWindow( hWnd );
   }
}

void
CheckBoxSetButtonDown( HWND       hWnd,
                       LPCHECKBOX lpCheckBox,
                       zBOOL      fValue )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetButtonDown" );
   if ( lpCheckBox->fButtonDown != fValue )
   {
      lpCheckBox->chPaint = CHECKBOX_PAINTPICT;
      lpCheckBox->fButtonDown = fValue;
      InvalidateRect( hWnd, 0, FALSE );
   }
}

void
CheckBoxSetCheck( HWND       hWnd,
                  LPCHECKBOX lpCheckBox,
                  zBOOL      fValue )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetCheck" );
   if ( lpCheckBox->fChecked != fValue )
   {
      lpCheckBox->chPaint = CHECKBOX_PAINTPICT;
      lpCheckBox->fChecked = fValue;
      InvalidateRect( hWnd, 0, FALSE );
   }
}

void
CheckBoxLoadDefBitmaps( HINSTANCE  hInst )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxLoadDefBitmaps" );
   if ( g_nCheckBoxDefPictLoadCnt == 0 )
   {
      kzCheckBoxLoadDef( hInst, BT_BITMAP | BT_PICT_UP,
                         (zLONG) "CB_Up" );
      kzCheckBoxLoadDef( hInst, BT_BITMAP | BT_PICT_DOWN,
                         (zLONG) "CB_Down" );
      kzCheckBoxLoadDef( hInst, BT_BITMAP | BT_PICT_FOCUSUP,
                         (zLONG) "CB_FocusUp" );
      kzCheckBoxLoadDef( hInst, BT_BITMAP | BT_PICT_FOCUSDOWN,
                         (zLONG) "CB_FocusDown" );
      kzCheckBoxLoadDef( hInst, BT_BITMAP | BT_PICT_DISABLEDUP,
                         (zLONG) "CB_DisabledUp" );
      kzCheckBoxLoadDef( hInst, BT_BITMAP | BT_PICT_DISABLEDDOWN,
                         (zLONG) "CB_DisabledDown" );
   }

   g_nCheckBoxDefPictLoadCnt++;
}

void
CheckBoxUnloadDefBitmaps( )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxUnloadDefBitmaps" );
   if ( g_nCheckBoxDefPictLoadCnt > 0 )
   {
      g_nCheckBoxDefPictLoadCnt--;

      if ( g_nCheckBoxDefPictLoadCnt == 0 )
         kzCheckBoxUnloadDefs( g_hInstanceDLL );
   }
}

zLONG
CheckBoxCreate( HWND hWnd, zLONG lParam )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxCreate" );
   LPCREATESTRUCT lpcsData;
   LPCHECKBOX     lpCheckBox;
// zPCHAR         hCheckBox;
   RECT           Rect;
   HDC            hDC;

   lpCheckBox = CheckBoxInit( );
   CheckBoxSetPtr( hWnd, (zPCHAR) lpCheckBox );

   kzCheckBoxGetPict( g_hInstanceDLL, &lpCheckBox->Picts );
   kzCheckBoxGetPict( (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ), &lpCheckBox->Picts );

   lpCheckBox->fUseDefPicts = TRUE;
   lpCheckBox->nCtlColorCode = CTLCOLOR_TBCHECKBOX;

   lpCheckBox->lStyle = GetWindowLong( hWnd, GWL_STYLE );

   lpcsData = (LPCREATESTRUCT) lParam;
   CheckBoxSetText( hWnd, (zPCHAR) lpcsData->szName );
   GetClientRect( hWnd, &Rect );

   // Create Bitmap to select to the memory device context.
   hDC = GetDC( hWnd );
   lpCheckBox->hBitmapDC = kzCreateCompatibleBitmap( hDC, Rect.right - Rect.left, Rect.bottom - Rect.top );
   ReleaseDC( hWnd, hDC );
   if ( ((LPCREATESTRUCT) lParam)->style & WS_DISABLED )
       lpCheckBox->fDisabled = TRUE;

// DrUnlockTaskMemory( lpCheckBox );
   return( 0 );
}

zBOOL
CheckBoxSetText( HWND  hWnd,
                 zPCHAR Text )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetText" );
   zPCHAR        hTextTemp;
   LPCHECKBOX    lpCheckBox;
   zPCHAR        TextTemp;

   lpCheckBox = CheckBoxGetPtr( hWnd );
   if ( lpCheckBox == 0 )
      return( FALSE );

   if ( Text == 0 )
      Text = "";

   if ( DrAllocTaskMemory( &hTextTemp, zstrlen( Text ) + 1, 1007 ) != 0 )
   {
      return( FALSE );
   }

   if ( lpCheckBox->hText )
   {
      TextTemp = (zPCHAR) DrUnlockTaskMemory( lpCheckBox->hText );
      DrFreeTaskMemory( TextTemp );
   }

   TextTemp = (zPCHAR) DrLockTaskMemory( hTextTemp );
   strcpy_s( TextTemp, zstrlen( Text ) + 1, Text );
// DrUnlockTaskMemory( TextTemp );

   lpCheckBox->hText = TextTemp;

// DrUnlockTaskMemory( lpCheckBox );
   return( TRUE );
}

void
CheckBoxSetPict( HWND            hWnd,
                 HINSTANCE       hInst,
                 LPCHECKBOXPICTS lpPicts,
                 WPARAM          wParam,
                 LPARAM          lParam )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxSetPict" );
   LPBTNPICT  lpPict;
   WORD       wPictType;

   lpPict = CheckBoxGetPicturePtr( lpPicts, wParam & (BT_PICT_UP | BT_PICT_DOWN | BT_PICT_FOCUSUP | BT_PICT_FOCUSDOWN | BT_PICT_DISABLEDUP | BT_PICT_DISABLEDDOWN) );

   wPictType = 0;

   if ( wParam & BT_BITMAP )
      wPictType |= SUPERBTN_PICT_BITMAP;

   if ( wParam & BT_ICON )
      wPictType |= SUPERBTN_PICT_ICON;

   if ( wParam & BT_HANDLE )
      wPictType |= SUPERBTN_PICT_HANDLE;

   SuperBtnSetPict( hWnd, hInst, lpPict, wPictType, lParam );

   lpPicts->lMaxPictWidth = 0;
   if ( lpPicts->Pict.chPictType )
      lpPicts->lMaxPictWidth = mMax( lpPicts->lMaxPictWidth, lpPicts->Pict.lPictWidth );

   if ( lpPicts->PictBtnDown.chPictType )
      lpPicts->lMaxPictWidth = mMax( lpPicts->lMaxPictWidth, lpPicts->PictBtnDown.lPictWidth );

   if ( lpPicts->PictFocus.chPictType )
      lpPicts->lMaxPictWidth = mMax( lpPicts->lMaxPictWidth, lpPicts->PictFocus.lPictWidth );

   if ( lpPicts->PictFocusDown.chPictType )
      lpPicts->lMaxPictWidth = mMax( lpPicts->lMaxPictWidth, lpPicts->PictFocusDown.lPictWidth );

   if ( lpPicts->PictDisabled.chPictType )
      lpPicts->lMaxPictWidth = mMax( lpPicts->lMaxPictWidth, lpPicts->PictDisabled.lPictWidth );

   if ( lpPicts->PictDisabledDown.chPictType )
      lpPicts->lMaxPictWidth = mMax( lpPicts->lMaxPictWidth, lpPicts->PictDisabledDown.lPictWidth );
}

void
CheckBoxFreePict( LPBTNPICT Pict )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxFreePict" );
   if ( Pict->hPict &&
        Pict->chPictType == SUPERBTN_PICT_BITMAP &&
        !Pict->HandleProvided )
   {
      DeleteObject( Pict->hPict );
   }
}

void
CheckBoxPaint( HWND       hWnd,
               LPCHECKBOX lpCheckBox,
               HDC        hDC,
               LPRECT     lpRect,
               zBOOL      fUseDib,
               LPRECT     lpRectOrig )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxPaint" );
   SS_CLRTBLITEM ClrTblItem;
   SS_CLRTBLITEM ClrTblItemLight;
   RECT            rectTemp;
   RECT            rectShrink;

   HWND hWndParent = GetParent( hWnd );
   LPSPREADSHEET lpSS = SS_Lock( hWndParent );

   // Fill the hDCMemory background with the current background of the display.
// SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.BackgroundId );
   SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowColorId );

#ifdef __WIN32__
   HBRUSH  hBrush = ClrTblItem.hBrush;
#else
   HBRUSH  hBrush = (HBRUSH) SendMessage( hWndParent, WM_CTLCOLOR, (WPARAM) hDC, MAKELONG( hWnd, lpCheckBox->nCtlColorCode ) );
#endif

   zmemcpy( &rectTemp, lpRect, sizeof( RECT ) );
   zmemcpy( &rectShrink, lpRect, sizeof( RECT ) );

   FillRect( hDC, lpRect, ClrTblItem.hBrush );

   SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowDarkId );
   SS_GetColorItem( lpSS, &ClrTblItemLight, lpSS->Color.ShadowLightId );

   SS_DrawShadows( hDC, lpRect, 1, ClrTblItem.hBrush,
                   ClrTblItemLight.hBrush, FALSE );
   SS_ShrinkRect( &rectShrink, 4, 0 );
//xSS_ShrinkRect( &rectTemp, 4, 0 );
   rectTemp.left = (lpRect->left + lpRect->right) / 2;
   if ( rectTemp.left > 5 )
      rectTemp.left -= 4;

// DrUnlockTaskMemory( lpSS );

   CheckBoxPaintPict( hWnd, lpCheckBox, &rectTemp, hDC, hDC, hBrush, FALSE, fUseDib, &rectShrink );
//xCheckBoxPaintText( hWnd, lpCheckBox, lpRect, &rectTemp, hDC, hDC, hBrush, FALSE, fUseDib, &rectShrink );
   lpCheckBox->chPaint = CHECKBOX_PAINTALL;
}

void
CheckBoxPaintPict( HWND       hWnd,
                   LPCHECKBOX lpCheckBox,
                   LPRECT     lpRect,
                   HDC        hDCDev,
                   HDC        hDC,
                   HBRUSH     hBrush,
                   zBOOL      fPaint,
                   zBOOL      fUseDib,
                   LPRECT     lpRectOrig )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxPaintPict" );
   zBTNPICT   Pict;
// zPCHAR     lpText;
   HANDLE     hDIB;
   HPALETTE   hPalette;
   HBITMAP    hPict;
   RECT       rectControl;
   RECT       rectPict;
   RECT       rectTemp;
   HFONT      hFontOld = 0;
   zBOOL      fDeletePict = FALSE;
   zLONG      lPictWidth;
   zLONG      lPictHeight;
   zLONG      lPictWidthOrig;
   zLONG      lPictHeightOrig;
   zSHORT     nTextLth;
   zSHORT     nOrigX;
   zSHORT     nOrigY;
   zLONG      lWidth;
   zLONG      x;
   zLONG      y;
   zLONG      dxScaleX;
   zLONG      dxScaleY;
   int        nMapModeOld;

   if ( lpCheckBox->hFont )
      hFontOld = SelectFont( hDC, lpCheckBox->hFont );

   zmemcpy( &rectPict, lpRect, sizeof( RECT ) );
   zmemcpy( &rectControl, lpRect, sizeof( RECT ) );

   // Determine picture to use.
   CheckBoxGetPicture( hWnd, lpCheckBox, &Pict );

   if ( fUseDib )
   {
      dxScaleX = (lpRect->right - lpRect->left) / (lpRectOrig->right - lpRectOrig->left);

      dxScaleY = (lpRect->bottom - lpRect->top ) / (lpRectOrig->bottom - lpRectOrig->top);

      lPictWidth = Pict.lPictWidth * dxScaleX;
      lPictHeight = Pict.lPictHeight * dxScaleY;
   }

   else
   {
      lPictWidth = Pict.lPictWidth;
      lPictHeight = Pict.lPictHeight;
   }

   if ( lpCheckBox->lStyle & BS_CENTER )
   {
      zmemcpy( &rectTemp, lpRect, sizeof( RECT ) );

      if ( lpCheckBox->hText )
      {
//       lpText = (zPCHAR) DrLockTaskMemory( lpCheckBox->hText );
         DrawText( hDC, lpCheckBox->hText, -1, &rectTemp,
                   DT_TOP | DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
//       DrUnlockTaskMemory( lpText );
         lWidth = rectTemp.right - rectTemp.left;
      }
      else
         lWidth = 0;

      if ( fUseDib )
         lWidth += (zSHORT)((Pict.lPictWidth + (lWidth ? BTNTEXTPICT_INC : 0)) *
                           dxScaleX );
      else
         lWidth += Pict.lPictWidth + (lWidth ? BTNTEXTPICT_INC : 0);

      rectPict.left += mMax( (rectPict.right - rectPict.left) - lWidth, 0 ) / 2;
      rectPict.right = mMin( rectPict.right, rectPict.left + lWidth );
   }

   zmemcpy( lpRect, &rectPict, sizeof( RECT ) );

   if ( lpCheckBox->lStyle & BS_LEFTTEXT )
      rectPict.left = rectPict.right - lPictWidth;
   else
      rectPict.right = rectPict.left + lPictWidth;

   // Draw Picture.
   if ( Pict.chPictType )
   {
      x = rectPict.left + (rectPict.right - rectPict.left - lPictWidth) / 2;
      y = rectPict.top + (rectPict.bottom - rectPict.top - lPictHeight) / 2;

      if ( fUseDib )
      {
         hPict = 0;

         if ( Pict.hPict )
            switch ( Pict.chPictType )
            {
               case SUPERBTN_PICT_ICON:
                  SetRect( &rectTemp, 0, 0, GetSystemMetrics( SM_CXICON ), GetSystemMetrics( SM_CYICON ) );
                  hPict = (HBITMAP) kzCreateStretchedIcon( hDC, &rectTemp, GetBkColor( hDC ), (HICON) Pict.hPict );
                  fDeletePict = TRUE;
                  break;

               case SUPERBTN_PICT_BITMAP:
                  hPict = (HBITMAP) Pict.hPict;
                  break;
            }

         hPalette = GetSystemPalette( );
         hDIB = BitmapToDIB( hPict, hPalette );
         DeleteObject( hPalette );

         SetStretchBltMode( hDC, COLORONCOLOR );
         nMapModeOld = SetMapMode( hDC, MM_TEXT );

         x = mMax( rectPict.left, x );
         y = mMax( rectPict.top, y );

         lPictWidthOrig = mMin( Pict.lPictWidth, (zSHORT)((rectPict.right - x) / dxScaleX) );
         lPictHeightOrig = mMin( Pict.lPictHeight, (zSHORT)((rectPict.bottom - y) / dxScaleY) );

         nOrigX = mMax( (Pict.lPictWidth - (zSHORT)((rectPict.right - x) / dxScaleX)) / 2, 0 );
         nOrigY = mMax( (Pict.lPictHeight - (zSHORT)((rectPict.bottom - y) / dxScaleY)) / 2, 0 );

         StretchDibBlt( hDC, x, y, mMin( rectPict.right - x, lPictWidth ),
                        mMin( rectPict.bottom - y, lPictHeight ), hDIB, nOrigX, nOrigY,
                        lPictWidthOrig, lPictHeightOrig, SRCCOPY );

         if ( fDeletePict )
            DeleteObject( hPict );

         SetMapMode( hDC, nMapModeOld );
      }
      else
      {
         switch ( Pict.chPictType )
         {
            case SUPERBTN_PICT_ICON:
               if ( Pict.hPict )
                  kzDrawIcon( hDC, &rectPict, GetBkColor( hDC ), x, y, (HICON) Pict.hPict );

               break;

            case SUPERBTN_PICT_BITMAP:
               if ( Pict.hPict )
                  kzDrawBitmap( hDC, (HBITMAP) Pict.hPict, &rectPict, x, y );

               break;
         }

         nTextLth = 0;
         if ( lpCheckBox->hText )
         {
//          lpText = (zPCHAR) DrLockTaskMemory( lpCheckBox->hText );
            nTextLth = zstrlen( lpCheckBox->hText );
//          DrUnlockTaskMemory( lpText );
         }

         // If no text and we have focus, then draw the focus rect.
         if ( lpCheckBox->fFocus && !nTextLth )
         {
            rectTemp.left = mMax( rectPict.left - 2, rectControl.left );
            rectTemp.top = mMax( ((rectPict.bottom - rectPict.top -
                                  lPictHeight) / 2) - 2, rectControl.top );
            rectTemp.right = mMin( rectPict.right + 2, rectControl.right );
            rectTemp.bottom = mMin( rectPict.bottom + 2, rectControl.bottom );
            FrameRect( hDC, &rectTemp, lpCheckBox->hBrushFocus );
         }
      }
   }

   if ( hFontOld )
      SelectFont( hDC, hFontOld );

   // Copy the memory device context bitmap to the display.
   if ( fPaint )
   {
      BitBlt( hDCDev, rectPict.left, rectPict.top,
              rectPict.right - rectPict.left,
              rectPict.bottom - rectPict.top,
              hDC, rectPict.left, rectPict.top, SRCCOPY );
   }
}

void
CheckBoxPaintText( HWND         hWnd,
                   LPCHECKBOX   lpCheckBox,
                   LPRECT       lpRect,
                   LPRECT       lpRectText,
                   HDC          hDCDev,
                   HDC          hDC,
                   HBRUSH       hBrush,
                   zBOOL        fPaint,
                   zBOOL        fUseDib,
                   LPRECT       lpRectOrig )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxPaintText" );
   zPCHAR        hTextTemp;
   TEXTMETRIC    fm;
   HFONT         hFontOld = 0;
   COLORREF      clrTextOld;
   HBRUSH        hBrushTemp;
   RECT          rectTemp;
   RECT          rectText;
// zPCHAR        Text;
   zPCHAR        TextTemp;
   zLONG         lPictWidth;
   zLONG         lTextWidth;
   zLONG         lTextHeight;
   zLONG         lAt;
   zLONG         x;
   zLONG         y;
   zSHORT        nLth;
   zUSHORT       k;


   clrTextOld = SetTxColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );

   if ( lpCheckBox->hFont )
      hFontOld = SelectFont( hDC, lpCheckBox->hFont );

   // Determine Picture and Text size.
   if ( lpCheckBox->hText )
   {
//    Text = (zPCHAR) DrLockTaskMemory( lpCheckBox->hText );
      nLth = zstrlen( lpCheckBox->hText );
   }
   else
      nLth = 0;

   zmemcpy( &rectText, lpRectText, sizeof( RECT ) );

   GetTextMetrics( hDC, &fm );

   if ( fUseDib )
      lPictWidth = ((lpCheckBox->Picts.lMaxPictWidth + BTNTEXTPICT_INC) *
                              ((lpRect->right - lpRect->left) / (lpRectOrig->right - lpRectOrig->left)));
   else
      lPictWidth = lpCheckBox->Picts.lMaxPictWidth + BTNTEXTPICT_INC;

   if ( nLth )
   {
      zmemcpy( &rectTemp, &rectText, sizeof( RECT ) );
      DrawText( hDC, lpCheckBox->hText, -1, &rectTemp, DT_TOP | DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
      lTextWidth = rectTemp.right - rectTemp.left;
   }
   else
      lTextWidth = 0;

   if ( lpCheckBox->lStyle & BS_LEFTTEXT )
   {
      rectText.right = rectText.right - lPictWidth;
      rectText.left = mMax( rectText.left, rectText.right - lTextWidth );
   }
   else
   {
      rectText.left = rectText.left + lPictWidth;
      rectText.right = mMin( rectText.right, rectText.left + lTextWidth );
   }

   // Draw Text.
   int nOldBkMode = SetBkMode( hDC, TRANSPARENT );
   if ( nLth )
   {
      zmemcpy( &rectTemp, &rectText, sizeof( RECT ) );
      lTextHeight = (zSHORT)fm.tmHeight;

      x = rectText.left + mMax( (rectText.right - rectText.left - lTextWidth) / 2, 0 );
      y = rectText.top + mMax( (rectText.bottom - rectText.top - lTextHeight) / 2, 0 );

      rectText.left = x;
      rectText.top = y;
      rectText.right = mMin( x + lTextWidth, rectText.right );
      rectText.bottom = mMin( y + lTextHeight, rectText.bottom );

      if ( lpCheckBox->fDisabled == FALSE )
      {
         DrawText( hDC, lpCheckBox->hText, -1, &rectText,
                   DT_TOP | DT_LEFT | DT_SINGLELINE );
      }
      else
      {
         if ( DrAllocTaskMemory( &hTextTemp, zstrlen( lpCheckBox->hText ) + 1L, 1008 ) == 0 )
         {
            TextTemp = (zPCHAR) DrLockTaskMemory( hTextTemp );

            for ( k = 0, lAt = 0; k < zstrlen( lpCheckBox->hText ); k++ )
            {
               if ( lpCheckBox->hText[ k ] != '&' )
                  TextTemp[ lAt++ ] = lpCheckBox->hText[ k ];
            }

            hBrushTemp = kzCreateSolidBrush( GetTextColor( hDC ) );
            fnGrayString( hDC, hBrushTemp, 0, (DWORD) TextTemp, 0, x, y, 0, 0 );
            DeleteBrush( hBrushTemp );

            DrUnlockTaskMemory( TextTemp );
            DrFreeTaskMemory( hTextTemp );
         }
      }
   }

   SetBkMode( hDC, nOldBkMode );

// if ( lpCheckBox->hText )
//    DrUnlockTaskMemory( lpCheckBox->hText );

   if ( lpCheckBox->fFocus && nLth )
   {
      rectText.bottom += 1;
      rectText.left -= 1;
      rectText.right += 1;
      FrameRect( hDC, &rectText, lpCheckBox->hBrushFocus );
   }

   if ( hFontOld )
      SelectFont( hDC, hFontOld );

   if ( clrTextOld != CLR_INVALID )
      SetTxColor( hDC, clrTextOld );

   // Copy the memory device context bitmap to the display.
   if ( fPaint )
      BitBlt( hDCDev, rectText.left, rectText.top, rectText.right - rectText.left,
              rectText.bottom - rectText.top, hDC, rectText.left, rectText.top, SRCCOPY );
}

// Called only when the column becomes a combo box
void
SS_ComboPaint( HWND     hWnd,
               HDC      hDC,
               LPRECT   lpRect )
{
   TRACE_DEBUG( "<SS_COMBO.CPP>", "SS_ComboPaint" );
   LPSPREADSHEET   lpSS;
   SS_CLRTBLITEM BackClrTblItem;
   SS_CELLTYPE     CellType;

   lpSS = SS_Lock( GetParent( hWnd ) );
// TraceLineI( "SS_ComboPaint Row: ", lpSS->Row.ssCurrAt );

   SS_GetClrTblItem( GetParent( hWnd ), lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, &BackClrTblItem, 0 );

   SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.BackgroundId );
   FillRect( hDC, lpRect, BackClrTblItem.hBrush );

   SS_DrawComboBtn( hWnd, lpSS, hDC, lpRect, lpSS->nComboButtonBitmapWidth, (zBOOL) GetWindowLong( hWnd, GWL_BTNDOWN ) );

   SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
   if ( SS_CB_ISDROPDOWN( CellType.lStyle ) == 0 )
   {
      zPCHAR     lpText;
      zPCHAR     hText;
      HBRUSH     hBrush;
      SS_FONT    FontTemp;
      HFONT      hFontOld;
      LPSS_FONT  Font;
      LPSS_CELL  Cell;
      zSHORT     nLth;

      nLth = (zSHORT) SendMessage( hWnd, WM_GETTEXTLENGTH, 0, 0L );

      if ( DrAllocTaskMemory( &hText, nLth + 1, 1009 ) == 0 )
      {
         lpText = (zPCHAR) DrLockTaskMemory( hText );
         SendMessage( hWnd, WM_GETTEXT, nLth + 1, (zLONG) lpText );

         Cell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
         Font = SS_RetrieveFont( lpSS, &FontTemp, Cell, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
         SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

         hFontOld = SelectFont( hDC, Font->hFont );

         COLORREF clrBkOld = SetBkColor( hDC, GetSysColor( COLOR_HIGHLIGHT ) );
         COLORREF clrTextOld = SetTxColor( hDC, GetSysColor( COLOR_HIGHLIGHTTEXT ) );

         lpRect->left += lpSS->nMarginX;
         lpRect->top += lpSS->nMarginY;
         lpRect->right -= lpSS->nComboButtonBitmapWidth;

         hBrush = kzCreateSolidBrush( GetSysColor( COLOR_HIGHLIGHT ) );
         FillRect( hDC, lpRect, hBrush );
         DeleteBrush( hBrush );

      // TraceLineS( "SS_ComboPaint Draw: ", lpText );
         DrawText( hDC, lpText, -1, lpRect, DT_LEFT | DT_NOPREFIX );

         if ( clrBkOld != CLR_INVALID )
            SetBkColor( hDC, clrBkOld );

         if ( clrTextOld != CLR_INVALID )
            SetTxColor( hDC, clrTextOld );

         SelectFont( hDC, hFontOld );

         DrUnlockTaskMemory( lpText );
         DrFreeTaskMemory( hText );
      }
   }

// DrUnlockTaskMemory( lpSS );
}

void
CheckBoxGetPicture( HWND        hWnd,
                    LPCHECKBOX  lpCheckBox,
                    LPBTNPICT   lpPict )
{
   WORD  wPictType;

   if ( lpCheckBox->fDisabled && (((lpCheckBox->lStyle & 0x0F) == BS_3STATE) || ((lpCheckBox->lStyle & 0x0F) == BS_AUTO3STATE)) )
   {
      if ( lpCheckBox->fChecked )
         wPictType = BT_PICT_DISABLEDDOWN;
      else
         wPictType = BT_PICT_DISABLEDUP;
   }
   else
   if ( lpCheckBox->fButtonDown )
   {
      if ( lpCheckBox->fChecked )
         wPictType = BT_PICT_FOCUSDOWN;
      else
         wPictType = BT_PICT_FOCUSUP;
   }
   else
   {
      if ( lpCheckBox->fChecked )
         wPictType = BT_PICT_DOWN;
      else
         wPictType = BT_PICT_UP;
   }

   CheckBoxPickPicture( lpCheckBox, lpPict, wPictType );
}


void
CheckBoxPickPicture( LPCHECKBOX  lpCheckBox,
                     LPBTNPICT   lpPict,
                     WORD        wPictType )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxPickPicture" );
   WORD       *lpwPriority;
   zBOOL      fFound;
   zSHORT     nPriorityCnt;
   zSHORT     k;
   static WORD  wPriorityUp[]=
   {
      BT_PICT_UP,
      BT_PICT_FOCUSUP,
      BT_PICT_DISABLEDUP,
   };
   static WORD  wPriorityDown[]=
   {
      BT_PICT_UP,
      BT_PICT_DOWN,
      BT_PICT_FOCUSUP,
      BT_PICT_FOCUSDOWN,
      BT_PICT_DISABLEDUP,
      BT_PICT_DISABLEDDOWN,
   };

   if ( lpCheckBox->fChecked )
   {
      lpwPriority = wPriorityDown;
      nPriorityCnt = sizeof( wPriorityDown ) / sizeof(WORD);
   }
   else
   {
      lpwPriority = wPriorityUp;
      nPriorityCnt = sizeof( wPriorityUp ) / sizeof(WORD);
   }

   for ( k = nPriorityCnt - 1, fFound = FALSE; k >= 0; k-- )
   {
      if ( lpwPriority[ k ] == wPictType )
         fFound = TRUE;

      if ( fFound )
         if ( CheckBoxChoosePicture( lpCheckBox, lpPict, lpwPriority[ k ] ) )
            break;
   }

   if ( fFound < 0 )
      zmemcpy( lpPict, &lpCheckBox->Picts.Pict, sizeof( zBTNPICT ) );
}

zBOOL
CheckBoxChoosePicture( LPCHECKBOX  lpCheckBox,
                       LPBTNPICT   lpPict,
                       WORD        wPictType )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "CheckBoxChoosePicture" );
   LPBTNPICT  lpPictTemp;

   lpPictTemp = CheckBoxGetPicturePtr( &lpCheckBox->Picts, wPictType );

   if ( lpPictTemp->chPictType )
   {
      zmemcpy( lpPict, lpPictTemp, sizeof( zBTNPICT ) );
      return( TRUE );
   }

   zmemset( lpPict, 0, sizeof( zBTNPICT ) );
   return( FALSE );
}

zBOOL
kzCheckBoxLoadDef( HINSTANCE  hInst,
                   WORD  wButtonType,
                   zLONG lPict )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzCheckBoxLoadDef" );
   return( kzxCheckBoxLoadDef( &g_hCheckBoxDefPicts, &g_nCheckBoxDefPictCnt, hInst, wButtonType, lPict ) );
}

zBOOL
kzCheckBoxUnloadDefs( HINSTANCE  hInst )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzCheckBoxUnloadDefs" );
   return( kzxCheckBoxUnloadDefs( &g_hCheckBoxDefPicts, &g_nCheckBoxDefPictCnt, hInst ) );
}

zBOOL
kzCheckBoxGetPict( HINSTANCE       hInst,
                   LPCHECKBOXPICTS lpDefPict )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzCheckBoxGetPict" );
   return( kzxCheckBoxGetPict( &g_hCheckBoxDefPicts, &g_nCheckBoxDefPictCnt, hInst, lpDefPict ) );
}

zBOOL
kzxCheckBoxLoadDef( zPCHAR     *lphDefPicts,
                    zPSHORT    pnDefPictCnt,
                    HINSTANCE  hInst,
                    WORD       wButtonType,
                    zLONG      lPict )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzxCheckBoxLoadDef" );
   LPCHECKBOXPICTS lpDefPicts;
   zBOOL           fRet = TRUE;
   zSHORT          nFound = -1;
   zSHORT          k;

   if ( *pnDefPictCnt )
   {
//    lpDefPicts = (LPCHECKBOXPICTS) DrLockTaskMemory( *lphDefPicts );
      lpDefPicts = (LPCHECKBOXPICTS) *lphDefPicts;

      for ( k = 0; nFound == -1 && k < *pnDefPictCnt; k++ )
         if ( lpDefPicts[ k ].hInst == hInst )
            nFound = k;

      if ( nFound == -1 )
      {
         *lphDefPicts = (zPCHAR) DrUnlockTaskMemory( lpDefPicts );
         if ( DrReAllocTaskMemory( lphDefPicts, (zLONG)(sizeof( CHECKBOXPICTS ) * (*pnDefPictCnt + 1)) ) != 0 )
         {
            *lphDefPicts = (zPCHAR) DrLockTaskMemory( lpDefPicts );
            return( FALSE );
         }

         *lphDefPicts = (zPCHAR) DrLockTaskMemory( *lphDefPicts );
//       lpDefPicts = (LPCHECKBOXPICTS) *lphDefPicts;
         nFound = *pnDefPictCnt;
         (*pnDefPictCnt)++;
      }
   }
   else
   {
      if ( DrAllocTaskMemory( lphDefPicts, sizeof( CHECKBOXPICTS ), 1011 ) != 0 )
      {
         return( FALSE );
      }

      *lphDefPicts = (zPCHAR) DrLockTaskMemory( *lphDefPicts );
      nFound = *pnDefPictCnt;
      (*pnDefPictCnt)++;
   }

// lpDefPicts = (LPCHECKBOXPICTS) DrLockTaskMemory( *lphDefPicts );
   lpDefPicts = (LPCHECKBOXPICTS) *lphDefPicts;
   lpDefPicts[ nFound ].hInst = hInst;
   CheckBoxSetPict( 0, hInst, &lpDefPicts[ nFound ], wButtonType, lPict );
// DrUnlockTaskMemory( lpDefPicts );
   return( fRet );
}

zBOOL
kzxCheckBoxUnloadDefs( zPCHAR     *lphDefPicts,
                       zPSHORT    pnDefPictCnt,
                       HINSTANCE  hInst )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzxCheckBoxUnloadDefs" );
   LPCHECKBOXPICTS lpDefPicts;
   zSHORT          nFound = -1;
   zSHORT          k;

   if ( *pnDefPictCnt && *lphDefPicts )
   {
//    lpDefPicts = (LPCHECKBOXPICTS) DrLockTaskMemory( *lphDefPicts );
      lpDefPicts = (LPCHECKBOXPICTS) *lphDefPicts;

      for ( k = 0; nFound == -1 && k < *pnDefPictCnt; k++ )
         if ( lpDefPicts[ k ].hInst == hInst )
            nFound = k;

      if ( nFound != -1 )
      {
         CheckBoxFreePict( &lpDefPicts[ nFound ].Pict );
         CheckBoxFreePict( &lpDefPicts[ nFound ].PictBtnDown );
         CheckBoxFreePict( &lpDefPicts[ nFound ].PictFocus );
         CheckBoxFreePict( &lpDefPicts[ nFound ].PictFocusDown );
         CheckBoxFreePict( &lpDefPicts[ nFound ].PictDisabled );
         CheckBoxFreePict( &lpDefPicts[ nFound ].PictDisabledDown );

         for ( k = nFound; k < *pnDefPictCnt - 1; k++ )
            zmemcpy( &lpDefPicts[ nFound ], &lpDefPicts[ nFound + 1 ], sizeof( CHECKBOXPICTS ) );

//       DrUnlockTaskMemory( lpDefPicts );
         *lphDefPicts = (zPCHAR) DrUnlockTaskMemory( lpDefPicts );
         if ( (*pnDefPictCnt) == 1 )
         {
            DrFreeTaskMemory( *lphDefPicts );
            *lphDefPicts = 0;
         }
         else
         if ( DrReAllocTaskMemory( lphDefPicts, (sizeof( CHECKBOXPICTS ) * (*pnDefPictCnt - 1)) ) != 0 )
         {
            return( FALSE );
         }

         (*pnDefPictCnt)--;
      }
//    else
//       DrUnlockTaskMemory( lpDefPicts );
   }

   return( TRUE );
}

zBOOL
kzxCheckBoxGetPict( zPCHAR          *lphDefPicts,
                    zPSHORT         pnDefPictCnt,
                    HINSTANCE       hInst,
                    LPCHECKBOXPICTS lpDefPict )
{
   TRACE_DEBUG( "<CHECKBOX.CPP>", "kzxCheckBoxGetPict" );
   LPCHECKBOXPICTS lpDefPicts;
   zBOOL           fRet = TRUE;
   zSHORT          nFound = -1;
   zSHORT          k;

   if ( *pnDefPictCnt )
   {
//    lpDefPicts = (LPCHECKBOXPICTS) DrLockTaskMemory( *lphDefPicts );
      lpDefPicts = (LPCHECKBOXPICTS) *lphDefPicts;

      for ( k = 0; nFound == -1 && k < *pnDefPictCnt; k++ )
         if ( lpDefPicts[ k ].hInst == hInst )
            nFound = k;

      if ( nFound != -1 )
      {
         zmemcpy( lpDefPict, &lpDefPicts[ nFound ], sizeof( CHECKBOXPICTS ) );
         fRet = TRUE;
      }
      else
         zmemset( lpDefPict, 0, sizeof( CHECKBOXPICTS ) );

//    DrUnlockTaskMemory( lpDefPicts );
   }

   return( fRet );
}

void
SS_ComboSubclassDesktop( HWND hWnd )
{
   TRACE_DEBUG( "<SS_COMBO.CPP>", "SS_ComboSubclassDesktop" );
   HWND hWndDesktop;

   // subclass the desktop window
   hWndDesktop = GetDesktopWindow( );

   g_lpfnSSDeskTopProc = (WNDPROC) MakeProcInstance( (FARPROC) SS_DeskTopWndProc, g_hInstanceDLL );
   g_lpfnDeskTopProc = (WNDPROC) GetWindowLong( hWndDesktop, GWL_WNDPROC );
   SetWindowLong( hWndDesktop, GWL_WNDPROC, (zLONG) g_lpfnSSDeskTopProc );
   SetProp( hWndDesktop, "HWNDSPREAD", hWnd );
}


void
SS_ComboUnSubclassDesktop( )
{
   TRACE_DEBUG( "<SS_COMBO.CPP>", "SS_ComboUnSubclassDesktop" );
   HWND hWndDesktop;

   hWndDesktop = GetDesktopWindow( );
   SetWindowLong( hWndDesktop, GWL_WNDPROC, (zLONG) g_lpfnDeskTopProc );
   RemoveProp( hWndDesktop, "HWNDSPREAD" );
}

LRESULT CALLBACK
SS_DeskTopWndProc( HWND    hWnd,
                   WORD    uMsg,
                   WPARAM  wParam,
                   LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_DeskTopProc" );
// LPSPREADSHEET    lpSS;
// SS_CLRTBLITEM    ForeClrTblItem;
// SS_CLRTBLITEM    BackClrTblItem;
// HWND             hWndSpread;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_DeskTopWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

#ifndef __WIN32__
   switch ( uMsg )
   {
      case WM_CTLCOLOR:
         TraceLineI( "SS_DeskTopProc CTLCOLOR ", HIWORD( lParam ) );

         if ( (hWndSpread = (HWND) GetProp( hWnd, "HWNDSPREAD" )) != 0 )
         {
            lpSS = SS_Lock( hWndSpread );
            SS_GetClrTblItem( hWndSpread, lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, &BackClrTblItem, &ForeClrTblItem );

//          DrUnlockTaskMemory( lpSS );
            SetTxColor( (HDC) wParam, ForeClrTblItem.Color );

            SetBkColor( (HDC) wParam, BackClrTblItem.Color );
            return( (zLONG) BackClrTblItem.hBrush );
         }

         break;
   }
#endif

   return( CallWindowProc( (WNDPROC) g_lpfnDeskTopProc, hWnd, uMsg, wParam, lParam ) );
}

#endif
