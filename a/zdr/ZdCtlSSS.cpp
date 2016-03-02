/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlsss.cpp
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
// 1999.11.01    DKS    Z10    FH999
//    SetLock holds across MapFromOI.
//
// 1999.09.22    DKS    Z10    QS999
//    Fixed text color problem and started memory leak analyis.
//
// 1998.12.15    DKS
//    Fix to support LockColumn operations.
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

#define DKS_VIRT
// #define DKS_TEST

zBOOL
RegisterLibClass( HINSTANCE  hInst );
LRESULT CALLBACK
EditGeneralWndProc( HWND     hWnd,
                    WORD     uMsg,
                    WPARAM   wParam,
                    LPARAM   lParam );

zBOOL
RegisterSS( HINSTANCE  hInstance )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "RegisterSS" );
   WNDCLASS wc;
   zCHAR    szErrorClass[ 32 ];

   szErrorClass[ 0 ] = 0;

   do  // purist's goto
   {
      zmemset( &wc, 0, zsizeof( WNDCLASS ) );

//    wc.style         = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW |
//                       CS_DBLCLKS | CS_NOKEYCVT;
      wc.style         = CS_GLOBALCLASS | CS_DBLCLKS;
      wc.lpfnWndProc   = (WNDPROC) SpreadSheetWndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = 2 * zsizeof( void * );
      wc.hInstance     = hInstance;
      wc.hIcon         = 0;
      wc.hCursor       = 0;
//    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_SPREAD_CLASS_NAME;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_SPREAD_CLASS_NAME );
         break;
      }

      wc.style         = CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) EditGeneralWndProc;
      wc.cbWndExtra    = 2 * zsizeof( HANDLE ) + zsizeof( FARPROC );
      wc.hInstance     = hInstance;
      wc.hCursor       = LoadCursor( 0, IDC_IBEAM );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_GENERAL_CLASS_NAME;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_GENERAL_CLASS_NAME );
         break;
      }

      if ( GetClassInfo( 0, SSClassNames[ 0 ].TBEdit, &wc ) )
      {
         g_lpfnEditProc = (WNDPROC) wc.lpfnWndProc;

         wc.style |= CS_GLOBALCLASS;
         wc.lpfnWndProc = (WNDPROC) SS_EditWndProc;
         wc.hInstance = hInstance;
         wc.lpszClassName = SSClassNames[ 0 ].SSEdit;
         if ( AfxRegisterClass( &wc ) == 0 )
         {
            strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].SSEdit );
            break;
         }
      }

      if ( GetClassInfo( 0, "Edit", &wc ) )
      {
         g_lpfnEditHScrollProc = (WNDPROC) wc.lpfnWndProc;

         wc.style |= CS_GLOBALCLASS;
         wc.lpfnWndProc = (WNDPROC) SS_EditHScrollWndProc;
         wc.hInstance = hInstance;
         wc.lpszClassName = SSClassNames[ 0 ].SSEditHScroll;
         if ( AfxRegisterClass( &wc ) == 0 )
         {
            strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].SSEditHScroll );
            break;
         }
      }

//    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS;
      wc.style         = CS_DBLCLKS | CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) kzCheckBoxWndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = 16;
      wc.hInstance     = hInstance;
      wc.hIcon         = 0;
      wc.hCursor       = LoadCursor( 0, IDC_ARROW );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1 );
      wc.lpszMenuName  = 0;
      wc.lpszClassName = SSClassNames[ 0 ].TBCheckBox;

      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].TBCheckBox );
         break;
      }

      if ( GetClassInfo( 0, SSClassNames[ 0 ].TBCheckBox, &wc ) )
      {
         g_lpfnCheckBoxProc = (WNDPROC) wc.lpfnWndProc;
         wc.style |= CS_GLOBALCLASS;
         wc.lpfnWndProc = (WNDPROC) SS_CheckBoxWndProc;
         wc.hInstance = hInstance;
         wc.lpszClassName = SSClassNames[ 0 ].SSCheckBox;

         if ( AfxRegisterClass( &wc ) == 0 )
         {
            strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].SSCheckBox );
            break;
         }
      }

      if ( GetClassInfo( 0, "ListBox", &wc ) )
      {
         g_lpfnListBoxProc = (WNDPROC) wc.lpfnWndProc;

         wc.style |= CS_GLOBALCLASS | CS_PARENTDC;
         wc.lpfnWndProc = (WNDPROC) SS_ListBoxWndProc;
         wc.hInstance = hInstance;
         wc.lpszClassName = SSClassNames[ 0 ].SSListBox;
         if ( AfxRegisterClass( &wc ) == 0 )
         {
            strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].SSListBox );
            break;
         }
      }

//    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS;
      wc.style         = CS_DBLCLKS | CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) kzSSComboBoxWndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = GWL_MAX;
      wc.hInstance     = hInstance;
      wc.hIcon         = 0;
      wc.hCursor       = LoadCursor( 0, IDC_ARROW );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = SSClassNames[ 0 ].TBComboBox;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].TBComboBox );
         break;
      }

      if ( GetClassInfo( 0, "Edit", &wc ) )
      {
         g_lpfnEditComboBoxProc = (WNDPROC) wc.lpfnWndProc;

         wc.style |= CS_GLOBALCLASS;
         wc.lpfnWndProc = (WNDPROC) SS_EditComboBoxWndProc;
         wc.hInstance = hInstance;
         wc.lpszClassName = SSClassNames[ 0 ].SSEditComboBox;
         if ( AfxRegisterClass( &wc ) == 0 )
         {
            strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].SSEditComboBox );
            break;
         }
      }

      if ( GetClassInfo( 0, SSClassNames[ 0 ].TBComboBox, &wc ) )
      {
         g_lpfnComboBoxProc = (WNDPROC)wc.lpfnWndProc;

         wc.style |= CS_GLOBALCLASS;
         wc.lpfnWndProc = (WNDPROC) SS_ComboBoxWndProc;
         wc.hInstance = hInstance;
         wc.lpszClassName = SSClassNames[ 0 ].SSComboBox;

         if ( AfxRegisterClass( &wc ) == 0 )
         {
            strcpy_s( szErrorClass, zsizeof( szErrorClass ), SSClassNames[ 0 ].SSComboBox );
            break;
         }
      }

#if 0
#ifndef SS_NOCT_DATE
      wc.style         = CS_GLOBALCLASS | CS_DBLCLKS;
      wc.lpfnWndProc   = (WNDPROC) EditDateWndProc;
      wc.cbWndExtra    = 2 * zsizeof( HANDLE ) + zsizeof( FARPROC );
      wc.hInstance     = hInstance;
      wc.hCursor       = LoadCursor( 0, IDC_IBEAM );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_DATE_CLASS_NAME;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_DATE_CLASS_NAME );
         break;
      }
#endif

#ifndef SS_NOCT_TIME
      wc.style         = CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) EditTimeWndProc;
      wc.hInstance     = hInstance;
      wc.hCursor       = LoadCursor( 0, IDC_IBEAM );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_TIME_CLASS_NAME;
      wc.cbWndExtra    = 2 * zsizeof( HANDLE ) + zsizeof( FARPROC );
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_TIME_CLASS_NAME );
         break;
      }
#endif

#ifndef SS_NOCT_INT
      wc.style         = CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) EditIntegerWndProc;
      wc.hInstance     = hInstance;
      wc.hCursor       = LoadCursor( 0, IDC_IBEAM );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_INTEGER_CLASS_NAME;
      wc.cbWndExtra    = 2 * zsizeof( HANDLE ) + zsizeof( FARPROC );
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_INTEGER_CLASS_NAME );
         break;
      }
#endif

#ifndef SS_NOCT_FLOAT
      wc.style         = CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) EditFloatWndProc;
      wc.cbWndExtra    = 2 * zsizeof( HANDLE ) + zsizeof( FARPROC );
      wc.hInstance     = hInstance;
      wc.hCursor       = LoadCursor( 0, IDC_IBEAM );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_FLOAT_CLASS_NAME;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_FLOAT_CLASS_NAME );
         break;
      }
#endif

#ifndef SS_NOCT_PIC
      wc.style         = CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) EditPicWndProc;
      wc.cbWndExtra    = 2 * zsizeof( HANDLE ) + zsizeof( FARPROC );
      wc.hInstance     = hInstance;
      wc.hCursor       = LoadCursor( 0, IDC_IBEAM );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_PIC_CLASS_NAME;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_PIC_CLASS_NAME );
         break;
      }
#endif

      // The Calendar Class:
#ifndef SS_NOCALENDAR
#ifndef SS_NOCT_DATE
//    wc.style         = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW | CS_GLOBALCLASS;
      wc.style         = CS_DBLCLKS | CS_GLOBALCLASS;
      wc.lpfnWndProc   = (WNDPROC) CalendarWndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = zsizeof( HANDLE );
      wc.hInstance     = hInstance;
      wc.hIcon         = 0;
      wc.hCursor       = LoadCursor( 0, IDC_ARROW );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wc.lpszMenuName  = 0;
      wc.lpszClassName = VA_CALENDAR_CLASS_NAME;
      if ( AfxRegisterClass( &wc ) == 0 )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_CALENDAR_CLASS_NAME );
         break;
      }
#endif
#endif

      if ( RegisterViewPict( hInstance, VA_VIEWPICT_CLASS_NAME ) == FALSE )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_VIEWPICT_CLASS_NAME );
         break;
      }

#ifndef SSP_NO_TYPESPIN
      if ( RegisterSpinBtn( hInstance, VA_SPINBTN_CLASS_NAME ) == FALSE )
      {
         strcpy_s( szErrorClass, zsizeof( szErrorClass ), VA_SPINBTN_CLASS_NAME );
         break;
      }
#endif

#endif
   }  while ( FALSE );   // end of:  do  // purist's goto


   if ( szErrorClass[ 0 ] == 0 )
   {
//    TraceLineS( "RegisterLibClass successful", "" );
      return( TRUE );
   }
   else
   {
      TraceLineS( "Could not register class ", szErrorClass );
      return( FALSE );
   }
}

struct tagToolBoxGlobal
{
   int ClearField;
   int CurrentDateTimeKey;
   int PopupCalendarKey;
} ToolBoxGlobal = {VK_F2, VK_F3, VK_F4};

zBOOL OPERATION
SetToolBoxGlobal( int nID, zLONG lValue )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "SetToolBoxGlobal" );
   switch ( nID )
   {
      case TBG_CLEARFIELD:
         ToolBoxGlobal.ClearField = (int) lValue;
         return( TRUE );

      case TBG_CURRENTDATETIME:
         ToolBoxGlobal.CurrentDateTimeKey = (int) lValue;
         return( TRUE );

      case TBG_POPUPCALENDAR:
         ToolBoxGlobal.PopupCalendarKey = (int) lValue;
         return( TRUE );

      default:
         return( FALSE );
   }
}

zBOOL OPERATION
GetToolBoxGlobal( int nID, zPLONG lplValue )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "GetToolBoxGlobal " );
   switch ( nID )
   {
      case TBG_CLEARFIELD:
         *lplValue = (zLONG) ToolBoxGlobal.ClearField;
         return( TRUE );

      case TBG_CURRENTDATETIME:
         *lplValue = (zLONG) ToolBoxGlobal.CurrentDateTimeKey;
         return( TRUE );

      case TBG_POPUPCALENDAR:
         *lplValue = (zLONG) ToolBoxGlobal.PopupCalendarKey;
         return( TRUE );

      default:
         return( FALSE );
   }
}

void
SpreadInit( HWND hWnd )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "SpreadInit" );
}

#if 0
zSHORT
ViewGifLoadImage( HWND hWnd, LPVIEWPICT_PICTURE Picture,
                  zPCHAR szGIFFile, zPCHAR GifResData,
                  zLONG lGifResDataSize )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "ViewGifLoadImage" );
   return( 0 );
}


zSHORT
ViewPcxLoadImage( HWND hWnd, LPVIEWPICT_PICTURE Picture,
                  zPCHAR FileName, zPCHAR PcxResourceData,
                  zLONG lPcxResourceDataSize )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "ViewPcxLoadImage" );
   return( 0 );
}
#endif

LRESULT CALLBACK
DialogBoxProc( HINSTANCE  hInstance, zCPCHAR lpszDlg,
               HWND hWnd, DLGPROC DlgProc )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "DialogBoxProc" );
   return( DialogBox( hInstance, lpszDlg, hWnd, DlgProc ) );
}


#if ( defined( SS_NOCALENDAR ) || defined( SS_NOCT_DATE ) )
zBOOL OPERATION
PopupCalendar( HWND hDateWnd )
{
   TRACE_DEBUG( "<ZDSS.CPP>", "PopupCalendar" );
   return( 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
SpreadSheetWndProc( HWND    hWnd,
                    WORD    uMsg,
                    WPARAM  wParam,
                    LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SpreadSheetWndProc" );
   zPCHAR          hSpread;
   LPSPREADSHEET   lpSS;
// SS_CELLTYPE     CellTypeTemp;
// LPSS_CELLTYPE   lpCellType;
   HCURSOR         hCursor;
   HWND            hWndFocus;
   HDC             hDC;
   RECT            rect;
   RECT            rectClient;
   POINT           pt;
   POINT           ptClient;
   HFONT           hFont;
// SS_CLRTBLITEM   BackClrTblItem;
// SS_CLRTBLITEM   ForeClrTblItem;
   SS_RBUTTON      RButton;
   BITMAP          Bitmap;
   DWORD           dwTime;
   WORD            wShift = 0;
   zBOOL           fNoSetCapture = FALSE;
   zBOOL           fButtonDown;
   zBOOL           fNoHilite;
   zBOOL           fComboBtn = FALSE;
   zBOOL           fDone = FALSE;
   zBOOL           fMakeTransparent;
   zBOOL           fHiliteOnOld;
   zBOOL           fRedraw;
   zBOOL           fMoveActiveCell;
   zBOOL           fSingleSel;
   zLONG           dxVal;
   zLONG           lRet;
   zLONG           lStyle;
   zSHORT          nRC = -1;
   zLONG           lMouseX;
   zLONG           lMouseY;
   zLONG           lHeight;
   zLONG           lWidth;
   zLONG           lWidthTemp;
   SS_COORD        ssRow;
   SS_COORD        ssRow2;
   SS_COORD        ss;
   SS_COORD        ssCol;
   SS_COORD        ssCol2;
   zSHORT          nType;
   zLONG           x;
   zLONG           y;
   SIZE            size;

   static WORD  Bits[ ]       = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
   static WORD  Bits2[ ] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SpreadSheetWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_NCCREATE:
         ((LPCREATESTRUCT) lParam )->style &= ~(WS_VSCROLL | WS_HSCROLL);
         SetWindowLong( hWnd, GWL_STYLE, ((LPCREATESTRUCT) lParam)->style );
         break;

      case WM_CREATE:
         if ( g_nSpreadInstCnt == 0 )
         {
            // initialize memory pools ... this is an artifact of a time
            // when using SmartHeap ... now using Core's memory management
         }

         g_nSpreadInstCnt++;
         if ( DrAllocTaskMemory( (zCOREMEM) &hSpread,
                                 zsizeof( SPREADSHEET), 1050 ) == 0 )
         {
            lpSS = (LPSPREADSHEET) DrLockTaskMemory( hSpread );
         }
         else
         {
            lpSS = 0;  // this will hopefully never happen
            break;
         }

         SetWindowLong( hWnd, zGWL_HSS, (zLONG) lpSS );
         lpSS->hWnd = hWnd;
         lStyle = GetWindowLong( hWnd, GWL_STYLE );
         hDC = GetDC( hWnd );

         lpSS->nPixelsPerInchX = GetDeviceCaps( hDC, LOGPIXELSX );
         lpSS->nPixelsPerInchY = GetDeviceCaps( hDC, LOGPIXELSY );

         lpSS->hWndHScroll = CreateWindow( "ScrollBar", 0,
                                           WS_CHILD | WS_VISIBLE | SBS_HORZ,
                                           0, 0, 0, 0,
                                           hWnd, 0, g_hInstanceDLL, 0 );

         lpSS->hWndVScroll = CreateWindow( "ScrollBar", 0,
                                           WS_CHILD | WS_VISIBLE | SBS_VERT,
                                           0, 0, 0, 0,
                                           hWnd, 0, g_hInstanceDLL, 0 );

         lpSS->nVScrollWidth = GetSystemMetrics( SM_CXVSCROLL );
         lpSS->nHScrollHeight = GetSystemMetrics( SM_CYHSCROLL );

         // Obtain font information.
         hFont = SelectFont( hDC,
                             (HFONT) GetStockObject( SYSTEM_FIXED_FONT ) );
         lpSS->DefaultFontId = SS_InitFont( hWnd, hFont, FALSE, FALSE, 0 );
         GetTextExtentPoint( hDC, "1", 1, &size );
         lpSS->nStandardFontWidth = (zSHORT) size.cx;
         SelectFont( hDC, hFont );

         ReleaseDC( hWnd, hDC );

         // Load the button to be used for the combo box.
#define OBM_COMBO           32738
         lpSS->hBitmapComboBtn = LoadBitmap( 0, (zPCHAR) OBM_COMBO );

         GetObject( lpSS->hBitmapComboBtn, zsizeof( BITMAP ), (zPCHAR) &Bitmap );
         lpSS->nComboButtonBitmapWidth = Bitmap.bmWidth * 2;

         lpSS->hCursorPointer = SS_CreateCursorPointer( g_hInstanceDLL );
         lpSS->hCursorResizeH = SS_CreateCursorResizeh( g_hInstanceDLL );
         lpSS->hCursorResizeV = SS_CreateCursorResizev( g_hInstanceDLL );
         lpSS->hBitmapLines = kzCreateBitmap( 8, 8, 1, 1, (zPCHAR) Bits );
         lpSS->hBitmapLines2 = kzCreateBitmap( 8, 8, 1, 1, (zPCHAR) Bits2 );
         lpSS->hBrushLines = kzCreatePatternBrush( lpSS->hBitmapLines );
         lpSS->hBrushLines2 = kzCreatePatternBrush( lpSS->hBitmapLines2 );

         lpSS->Redraw = FALSE;
         SS_InitSpread( hWnd, lpSS );
         SSSetBool( hWnd, SSB_REDRAW, TRUE );

//       DrUnlockTaskMemory( lpSS );
         return( 0 );

// #define SWP_NOSIZE          0x0001
// #define SWP_NOMOVE          0x0002
// #define SWP_NOZORDER        0x0004
// #define SWP_NOREDRAW        0x0008
// #define SWP_NOACTIVATE      0x0010
// #define SWP_FRAMECHANGED    0x0020
// #define SWP_SHOWWINDOW      0x0040
// #define SWP_HIDEWINDOW      0x0080
// #define SWP_NOCOPYBITS      0x0100
// #define SWP_NOOWNERZORDER   0x0200
// #define SWP_NOSENDCHANGING  0x0400
//
// #define SWP_DRAWFRAME       SWP_FR
// #define SWP_NOREPOSITION    SWP_NO
//
// #if(WINVER >= 0x0400)
// #define SWP_DEFERERASE      0x2000
// #define SWP_ASYNCWINDOWPOS  0x4000

      case WM_WINDOWPOSCHANGING:
      {
         LPWINDOWPOS lpwp = (LPWINDOWPOS) lParam; // points to size and position data
         break;
      }

      case WM_WINDOWPOSCHANGED:
      {
         LPWINDOWPOS lpwp = (LPWINDOWPOS) lParam; // points to size and position data
         break;
      }

      case WM_NCCALCSIZE:
      {
         BOOL fCalcValidRects = (BOOL) wParam;      // valid area flag
         LPNCCALCSIZE_PARAMS lpncsp;
         LPRECT lpRect;

         if ( fCalcValidRects )
            lpncsp = (LPNCCALCSIZE_PARAMS) lParam;  // size calculation data
         else
            lpRect = (LPRECT) lParam;               // new window coordinates

         break;
      }

      case WM_CHILDACTIVATE:
         return( 0 );

      case WM_MOVE:
      {
         int xPos = (int)(short) LOWORD( lParam );    // horizontal position
         int yPos = (int)(short) HIWORD( lParam );    // vertical position

         break;
      }

      case WM_GETDLGCODE:
         lpSS = SS_Lock( hWnd );

         lRet = DLGC_WANTARROWS | DLGC_WANTCHARS;

         if ( lpSS->fProcessTab )
            lRet |= DLGC_WANTTAB;

         if ( lpSS->wEnterAction != SS_ENTERACTION_NONE )
            lRet = DLGC_WANTALLKEYS;

//       DrUnlockTaskMemory( lpSS );
         return( lRet );

#ifndef __WIN32__

      case WM_CTLCOLOR:
// #define CTLCOLOR_MSGBOX     0
// #define CTLCOLOR_EDIT       1
// #define CTLCOLOR_LISTBOX    2
// #define CTLCOLOR_BTN        3
// #define CTLCOLOR_DLG        4
// #define CTLCOLOR_SCROLLBAR  5
// #define CTLCOLOR_STATIC     6
// #define CTLCOLOR_TBEDIT     (101)
// #define CTLCOLOR_TBCHECKBOX (107)
         lpSS = SS_Lock( hWnd );

         if ( HIWORD( lParam ) == CTLCOLOR_SCROLLBAR )
            SS_GetColorItem( lpSS, &BackClrTblItem,
                             lpSS->ScrollBarColorId );
         else
//       if ( HIWORD( lParam ) == CTLCOLOR_TBCHECKBOX )
//          SS_GetColorItem( lpSS, &BackClrTblItem,
//                           lpSS->Color.ShadowColorId );
//       else
         {
            lpCellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, 0,
                                              lpSS->CurVisCell.ssCol,
                                              lpSS->CurVisCell.ssRow );

            if ( lpCellType && lpCellType->chType == SS_TYPE_STATICTEXT &&
                 (lpCellType->lStyle & SS_TEXT_SHADOW) )
            {
               SS_GetColorItem( lpSS, &BackClrTblItem,
                                lpSS->Color.ShadowColorId );
            }
            else
            {
               SS_GetClrTblItem( hWnd, lpSS, lpSS->CurVisCell.ssCol,
                                   lpSS->CurVisCell.ssRow, &BackClrTblItem,
                                   &ForeClrTblItem );

               if ( (lpSS->LockColor.BackgroundId ||
                     lpSS->LockColor.BackgroundId) &&
                    SS_GetLock( hWnd, lpSS, lpSS->CurVisCell.ssCol,
                                lpSS->CurVisCell.ssRow) )
               {
                  if ( lpSS->LockColor.BackgroundId )
                     SS_GetColorItem( lpSS, &BackClrTblItem,
                                      lpSS->LockColor.BackgroundId );

                  if ( lpSS->LockColor.ForegroundId )
                     SS_GetColorItem( lpSS, &ForeClrTblItem,
                                      lpSS->LockColor.ForegroundId );
               }

               SetTxColor( (HDC) wParam, ForeClrTblItem.Color );
            }
         }

//       DrUnlockTaskMemory( lpSS );

         SetBkColor( (HDC) wParam, BackClrTblItem.Color );
         return( (zLONG) BackClrTblItem.hBrush );
#endif

      case WM_SIZE:
         lpSS = SS_Lock( hWnd );
         SS_GetClientRect( hWnd, &lpSS->rectClient );
         SS_AutoSize( hWnd, TRUE );

         if ( (zSHORT) LOWORD( lParam ) > lpSS->nClientWidth )
         {
            SetRect( &rect, lpSS->nClientWidth, 0,
                     LOWORD( lParam ), HIWORD( lParam ) );
            InvalidateRect( hWnd, &rect, TRUE );
         }

         if ( (zSHORT) HIWORD( lParam ) > lpSS->nClientHeight )
         {
            SetRect( &rect, 0, lpSS->nClientHeight,
                     LOWORD( lParam ), HIWORD( lParam ) );
            InvalidateRect( hWnd, &rect, TRUE );
         }

#ifndef SS_NOCHILDSCROLL
         if ( lpSS->fVScrollVisible && lpSS->fHScrollVisible )
         {
            SetRect( &rect, LOWORD( lParam ) - lpSS->nVScrollWidth,
                     HIWORD( lParam ) - lpSS->nHScrollHeight,
                     LOWORD( lParam ), HIWORD( lParam ) );
            InvalidateRect( hWnd, &rect, TRUE );
            SetRect( &rect, lpSS->nClientWidth - lpSS->nVScrollWidth,
                     lpSS->nClientHeight - lpSS->nHScrollHeight,
                     lpSS->nClientWidth, lpSS->nClientHeight );
            InvalidateRect( hWnd, &rect, TRUE );
         }
#endif

         lpSS->nClientWidth = LOWORD( lParam );
         lpSS->nClientHeight = HIWORD( lParam );

//       DrUnlockTaskMemory( lpSS );
         break;

   case WM_NCHITTEST:
         lpSS = SS_Lock( hWnd );
         GetWindowRect( hWnd, &rect );
         SS_GetClientRect( hWnd, &rectClient );

         lMouseX = LOWORD( lParam );
         lMouseY = HIWORD( lParam );

         if ( lMouseX >= rect.left && lMouseX <= rect.left + rectClient.right &&
              lMouseY >= rect.top && lMouseY <= rect.top + rectClient.bottom )
         {
            pt.x = lMouseX;
            pt.y = lMouseY;

            ScreenToClient( hWnd, &pt );

            lMouseX = pt.x;
            lMouseY = pt.y;

            if ( SS_IsMouseInHorizResize( lpSS, &rect, &rectClient,
                                          lMouseX, lMouseY, 0 ) != -1 )
               lpSS->hCursorCurrent = (HICON) SS_CURSOR_DEFROWRESIZE;

            else
            if ( SS_IsMouseInVertResize( lpSS, &rect, &rectClient,
                                         lMouseX, lMouseY, 0 ) != -1 )
               lpSS->hCursorCurrent = (HICON) SS_CURSOR_DEFCOLRESIZE;

#if 0 // SS_NODRAGDROP
            else
            if ( SS_IsMouseInDragDrop( hWnd, lpSS, &rectClient,
                                       lMouseX, lMouseY, 0, 0) )
               lpSS->hCursorCurrent = (HICON) SS_CURSOR_ARROW;
#endif

            else
            if ( SS_IsMouseInGrayArea( lpSS, &rectClient, lMouseX, lMouseY ) )
            {
               lpSS->hCursorCurrent = lpSS->hCursorGrayArea;
#ifdef SS_MAKETRANSPARENT
//             DrUnlockTaskMemory( lpSS );
               return( HTTRANSPARENT );
#endif
            }
            else
            if ( lpSS->wOpMode == SS_OPMODE_READONLY ||
                 lpSS->wOpMode == SS_OPMODE_SINGLESEL ||
                 lpSS->wOpMode == SS_OPMODE_MULTISEL ||
                 lpSS->wOpMode == SS_OPMODE_EXTSEL )
               lpSS->hCursorCurrent = (HICON) SS_CURSOR_ARROW;
            else
            {
               SS_GetCellFromPixel( hWnd, lpSS, &ssCol, &ssRow, 0, 0,
                                    lMouseX, lMouseY );

            fMakeTransparent = TRUE;
            if ( ssCol < lpSS->Col.nHeaderCnt )
               lpSS->hCursorCurrent = lpSS->hCursorRowHeader;
            else
            if ( ssRow < lpSS->Row.nHeaderCnt )
               lpSS->hCursorCurrent = lpSS->hCursorColHeader;
            else
            if ( SS_GetLock( hWnd, lpSS, ssCol, ssRow) )
               lpSS->hCursorCurrent = lpSS->hCursorLockedCell;
            else
            if ( (lpSS->RestrictCols && ssCol > lpSS->Col.DataCnt) ||
                 (lpSS->RestrictRows && ssRow > lpSS->Row.DataCnt) )
               lpSS->hCursorCurrent = lpSS->hCursorLockedCell;
            else
            if ( SS_IsMouseInButton( lpSS, &rectClient, lMouseX, lMouseY ) )
            {
               fMakeTransparent = FALSE;
               lpSS->hCursorCurrent = lpSS->hCursorButton;
            }
            else
            {
               lpSS->hCursorCurrent = SS_CURSOR_DEFAULT;
               fMakeTransparent = FALSE;
            }

#ifdef SS_MAKETRANSPARENT
            if ( fMakeTransparent )
            {
//             DrUnlockTaskMemory( lpSS );
               return( HTTRANSPARENT );
            }
#endif
         }
      }
      else
      {
         lpSS->hCursorCurrent = (HICON) SS_CURSOR_ARROW;
      }

//    DrUnlockTaskMemory( lpSS );
      break;

   case WM_SYSKEYDOWN:
      if ( wParam == VK_BACK )
      {
         lpSS = SS_Lock( hWnd );

         if ( lpSS->fAllowUndo )
            SS_Undo( hWnd, lpSS );

         lpSS->wMsgLast = 0;
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }

      break;

   case WM_KEYDOWN:
      lpSS = SS_Lock( hWnd );

      if ( lpSS->fMsgSentToVB )
      {
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }

//    DrUnlockTaskMemory( lpSS );
      lRet = SS_KeyDown( hWnd, wParam, lParam );

      if ( lRet != -1 )
      {
         return( lRet );
      }

      break;

   case WM_KEYUP:
      lpSS = SS_Lock( hWnd );

      if ( lpSS->fMsgSentToVB )
      {
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }

//    DrUnlockTaskMemory( lpSS );

      if ( wParam == VK_SHIFT && !HIBYTE( GetKeyState( VK_SHIFT ) ) &&
           !(HIBYTE( GetKeyState( VK_LBUTTON ) )) )
      {
         lpSS = SS_Lock( hWnd );
         lpSS->ModeBlock = FALSE;

         if ( lpSS->BlockCellUL.ssRow != lpSS->BlockCellULPrev.ssRow ||
              lpSS->BlockCellUL.ssCol != lpSS->BlockCellULPrev.ssCol ||
              lpSS->BlockCellLR.ssRow != lpSS->BlockCellLRPrev.ssRow ||
              lpSS->BlockCellLR.ssCol != lpSS->BlockCellLRPrev.ssCol )
         {
            SS_SendMsgCommand( hWnd, 0, SSN_BLOCKSELECTED, FALSE );
         }

//       DrUnlockTaskMemory( lpSS );
      }

      break;

   case WM_CHAR:
      lpSS = SS_Lock( hWnd );

      if ( lpSS->fMsgSentToVB )
      {
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }

      if ( lpSS->fProhibitTypingWhenSel && SS_IsBlockSelected( lpSS ) &&
           !(wParam == 3 || wParam == 22 || wParam == 24 || wParam == 26) )
         SS_SendMsgCommand( hWnd, 0, SSN_TYPINGPROHIBITED, FALSE );
      else
      if ( lpSS->wOpMode == SS_OPMODE_MULTISEL )
      {
#ifndef SS_NOLISTBOX
         if ( wParam == 32 )           // Space bar
            SS_SelectRow( hWnd, lpSS );
#endif
      }
      else
      if ( wParam == 32 &&
          (HIBYTE( GetKeyState( VK_CONTROL ) ) ||
           HIBYTE( GetKeyState( VK_SHIFT ) )) )
      {
         ;
      }
      else
      if ( lpSS->wOpMode == SS_OPMODE_SINGLESEL ||
           lpSS->wOpMode == SS_OPMODE_MULTISEL ||
           lpSS->wOpMode == SS_OPMODE_EXTSEL )
      {
         ;
      }

      // Check to see if Ctrl-c, Ctrl-v or Ctrl-x were pressed.
      // If so, eat the key.
      else
      if ( HIBYTE( GetKeyState( VK_CONTROL ) ) && (wParam == 'c' - 'a' + 1 ||
                   wParam == 'v' - 'a' + 1 || wParam == 'x' - 'a' + 1) )
      {
         ;
      }
      else
      if ( wParam != '\t' && wParam != '\r' && mIs_hWndEnabled( hWnd ) &&
           mIs_hWndVisible( hWnd ) &&
           (lpSS->fEditModePermanent == 0 ||
            (lpSS->wOpMode == SS_OPMODE_ROWMODE &&
             lpSS->fRowModeEditing == 0)) )
      {
         SS_ResetBlock( hWnd, TRUE );
         lpSS->ModeBlock = FALSE;
         SS_CellEditModeOn( hWnd, lpSS, uMsg, wParam, lParam );
      }

//    DrUnlockTaskMemory( lpSS );
      return( 0 );

   // Any horizontal scroll (with mouse) causes this message.
   case WM_HSCROLL:
   // TraceLineS( "SpreadSheetWndProc WM_", "HSCROLL" );
      return( 0 );

   // A vertical scroll action occurred.
   case WM_VSCROLL:
   // TraceLineS( "SpreadSheetWndProc WM_", "VSCROLL" );
      return( 0 );

   case WM_SETFOCUS:
#ifdef SS_NOPROCESSFOCUS
      if ( lpSS = SS_Lock( hWnd ) )
      {
         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 &&
              lpSS->fProcessingLButtonDown == 0 &&
              lpSS->fRetainSelBlock == 0 )
         {
            SS_DrawSelBlock( hWnd, 0, lpSS );
            lpSS->fSelBlockInvisible = FALSE;
         }

//       DrUnlockTaskMemory( lpSS );
      }

      break;
#endif
   case SSM_SETFOCUSINTERNAL:
      if ( (lpSS = SS_Lock( hWnd )) != 0 )
      {
         lpSS->fSetFocusReceived = TRUE;
         lpSS->fProcessSetFocus = FALSE;

         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 &&
              lpSS->fProcessingLButtonDown == 0 )
         {
            lpSS->fProcessSetFocus = TRUE;

#ifndef SS_NOPROCESSFOCUS
            if ( lpSS->fRetainSelBlock == 0 )
            {
               lpSS->fSelBlockInvisible = FALSE;
            }

            if ( lpSS->EditModeTurningOn == 0 &&
                 lpSS->EditModeTurningOff == 0 &&
                 lpSS->wMessageBeingSent == 0 &&
                 lpSS->fProcessingLButtonDown == 0 )
            {
               if ( GetFocus( ) == hWnd )
               {
                  if ( lpSS->fEditModePermanent && lpSS->hWndFocusPrev )
                  {
                     SetFocus( lpSS->hWndFocusPrev );
                     lpSS->hWndFocusPrev = 0;
                  }
               }
            }
#endif

#ifndef SS_NOPROCESSFOCUS
            PostMessage( hWnd, SSM_PROCESSSETFOCUS,
                         GetWindowLong( hWnd, GWL_ID ),
                         MAKELONG( hWnd, 0) );
#else
            SendMessage( hWnd, SSM_PROCESSSETFOCUS,
                         GetWindowLong( hWnd, GWL_ID ),
                         MAKELONG( hWnd, 0) );
#endif
         }
         else
         if ( lpSS->fProcessingLButtonDown )
         {
            lpSS->fProcessSetFocus = TRUE;
            SendMessage( hWnd, SSM_PROCESSSETFOCUS,
                         GetWindowLong( hWnd, GWL_ID ),
                         MAKELONG( hWnd, 0) );
         }
         else
         if ( lpSS->wMessageBeingSent )
         {
#ifndef SS_NOPROCESSFOCUS
            PostMessage( hWnd, SSM_PROCESSSETFOCUS,
                         GetWindowLong( hWnd, GWL_ID ),
                         MAKELONG( hWnd, 0) );
#else
            lpSS->fSetFocusWhileMsgBeingSent = TRUE;
#endif
         }

//       DrUnlockTaskMemory( lpSS );
      }

      break;

#ifdef SS_NOPROCESSFOCUS
   case SSM_SETFOCUSPOSTPROCESS:
      if ( lpSS = SS_Lock( hWnd ) )
         {
         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 &&
              lpSS->fProcessingLButtonDown == 0 )
         {
            if ( lpSS->fSetFocusReceived == 0 && GetFocus( ) == hWnd )
            {
               if ( lpSS->fEditModePermanent && lpSS->hWndFocusPrev )
               {
                  SetFocus( lpSS->hWndFocusPrev );

                  lpSS->hWndFocusPrev = 0;
               }
               else
               if ( lpSS->fEditModePermanent == 0 )
               {
                  if ( SS_USESINGLESELBAR( lpSS ) )
                  {
                     if ( lpSS->fKillFocusReceived )
                     {
                        fHiliteOnOld = lpSS->HiliteOn;

                        if ( lpSS->HiliteOn )
                        {
                           lpSS->HiliteOn = FALSE;
                           lpSS->fProcessingKillFocus = TRUE;
                        }

                        SS_HiliteCell( hWnd, TRUE );

                        if ( fHiliteOnOld )
                        {
                           lpSS->HiliteOn = TRUE;
                           lpSS->fProcessingKillFocus = FALSE;
                        }

                        lpSS->fKillFocusReceived = FALSE;
                     }
                  }
                  else
                  if ( lpSS->EditModeOn == 0 )
                     SS_HiliteCell( hWnd, TRUE );
               }
            }
         }

//       DrUnlockTaskMemory( lpSS );
      }

      break;
#endif

   case WM_KILLFOCUS:
#ifdef SS_NOPROCESSFOCUS
      if ( lpSS = SS_Lock( hWnd ) )
      {
         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 &&
              lpSS->fRetainSelBlock == 0 &&
              !SS_USESELBAR( lpSS ) &&
              lpSS->fSelBlockInvisible == 0 )
         {
            SS_DrawSelBlock( hWnd, 0, lpSS );
            lpSS->fSelBlockInvisible = TRUE;
            if ( lpSS->fEditModePermanent == 0 )
               SS_HiliteCell( hWnd, FALSE );
         }

//       DrUnlockTaskMemory( lpSS );
      }

      break;
#endif
   case SSM_KILLFOCUSINTERNAL:
      if ( (lpSS = SS_Lock( hWnd )) != 0 )
      {
         lpSS->fProcessKillFocus = FALSE;
         lpSS->ModeBlock = FALSE;

         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 )
         {
#ifndef SS_NOPROCESSFOCUS
            if ( lpSS->fRetainSelBlock == 0 && !SS_USESELBAR( lpSS ) )
            {
               SS_DrawSelBlock( hWnd, 0, lpSS );
               lpSS->fSelBlockInvisible = TRUE;
               if ( lpSS->fEditModePermanent == 0 )
                  SS_HiliteCell( hWnd, FALSE );
            }
#endif

            lpSS->fProcessKillFocus = TRUE;
#ifndef SS_NOPROCESSFOCUS
            PostMessage( hWnd, SSM_PROCESSKILLFOCUS, wParam,
                        MAKELONG( hWnd, 0) );
#else
            SendMessage( hWnd, SSM_PROCESSKILLFOCUS, wParam,
                        MAKELONG( hWnd, 0) );
#endif
         }
#ifndef SS_NOPROCESSFOCUS
         else
         if ( lpSS->wMessageBeingSent )
            SendMessage( hWnd, SSM_PROCESSKILLFOCUS,
                         wParam, MAKELONG( hWnd, 0 ) );
#endif

         lpSS->fKillFocusReceived = TRUE;
//       DrUnlockTaskMemory( lpSS );
      }

      break;

   case WM_MOUSEACTIVATE:
      if ( HIWORD( lParam ) == WM_LBUTTONDOWN )
      {
         HWND hWndFocus = GetFocus( );
         if ( hWndFocus == 0 ||
              (GetParent( hWndFocus ) != hWnd &&
               (GetParent( hWndFocus ) == 0 ||
                GetParent( GetParent( hWndFocus ) ) != hWnd)) )
         {
            lpSS = SS_Lock( hWnd );
            lpSS->fProcessingLButtonDown = TRUE;

//          DrUnlockTaskMemory( lpSS );
         }
      }

      break;

   case WM_LBUTTONDOWN:
   // TraceLineS( "SpreadSheetWndProc", " WM_LBUTTONDOWN" );
   case WM_LBUTTONDBLCLK:
   // TraceLineS( "SpreadSheetWndProc DblClk", "" );
      lpSS = SS_Lock( hWnd );

      lpSS->fProcessingLButtonDown = FALSE;
      if ( lpSS->fNoProcessButtonDown )
      {
         lpSS->fNoProcessButtonDown = FALSE;

         hWndFocus = GetFocus( );
         if ( hWndFocus != hWnd && GetParent( hWndFocus ) != hWnd )
            SetFocus( hWnd );

//       DrUnlockTaskMemory( lpSS );
         break;
      }

      hWndFocus = GetFocus( );
      if ( hWndFocus != hWnd && GetParent( hWndFocus ) != hWnd )
      {
         lpSS->fProcessingLButtonDown = TRUE;
         SetFocus( hWnd );

         if ( SS_USESELBAR( lpSS ) )
         {
            if ( lpSS->HiliteOn )
            {
               lpSS->HiliteOn = FALSE;
               lpSS->fProcessingKillFocus = TRUE;
               SS_HiliteCell( hWnd, TRUE );
               lpSS->HiliteOn = TRUE;
               lpSS->fProcessingKillFocus = FALSE;
            }
         }

         if ( lpSS->fMoveActiveOnFocus == 0 && lpSS->fNoProcessButtonDown )
         {
            lpSS->fProcessingLButtonDown = FALSE;
            lpSS->fNoProcessButtonDown = FALSE;
//          DrUnlockTaskMemory( lpSS );
            break;
         }
      }

      PostMessage( hWnd, uMsg == WM_LBUTTONDOWN ? SSM_PROCESSLBUTTONDOWN :
                   SSM_PROCESSLBUTTONDBLCLK, LOWORD( lParam ),
                   MAKELONG( HIWORD( lParam ), 0 ) );

//    DrUnlockTaskMemory( lpSS );
      break;

   case WM_LBUTTONUP:
      lpSS = SS_Lock( hWnd );

      if ( lpSS->ModeResize )
      {
         ReleaseCapture( );
         SS_Resize( hWnd, lpSS, SS_RESIZEMODE_END, lParam );

         if ( lpSS->ModeResize == SS_MODERESIZE_HORIZ ) // ssRow
         {
            y = lpSS->lResizeCurrentPos;
            lHeight = SS_GetRowHeight( lpSS, lpSS->ssResizeCoord );

            if ( lpSS->BlockCellUL.ssRow != -1 &&
                 lpSS->BlockCellUL.ssCol == 0 &&
                 lpSS->BlockCellLR.ssCol == -1 )
            {
               ssRow = lpSS->BlockCellUL.ssRow;
               ssRow2 = lpSS->BlockCellLR.ssRow;
            }
            else
            {
               ssRow = lpSS->ssResizeCoord;
               ssRow2 = lpSS->ssResizeCoord;
            }

            fRedraw = lpSS->Redraw;
            SSSetBool( hWnd, SSB_REDRAW, FALSE );
            for ( ss = ssRow; ss <= ssRow2; ss++ )
            {
               if ( SSGetUserResizeRow( hWnd, ss ) != SS_RESIZE_OFF )
               {
                  SS_CalcPixelsToRowHeight( lpSS, ss,
                                            lHeight + (y - lpSS->lResizeStartPos),
                                            &dxVal );
                  SSSetRowHeight( hWnd, ss - (lpSS->Row.nHeaderCnt - 1),
                                  dxVal );
               }
            }

            SSSetBool( hWnd, SSB_REDRAW, fRedraw );

            if ( lpSS->Row.ssCurrAt >= ssRow && lpSS->Row.ssCurrAt <= ssRow2 &&
                 lHeight + (y - lpSS->lResizeStartPos) == 0 )
            {
               SS_ScrollLineLR( hWnd, lpSS,
                                SS_F_SCROLL_DOWN | SS_F_MOVEACTIVECELL );
               if ( lpSS->Row.ssCurrAt >= ssRow && lpSS->Row.ssCurrAt <= ssRow2 )
                  SS_ScrollLineUL( hWnd, lpSS,
                                   SS_F_SCROLL_UP | SS_F_MOVEACTIVECELL );
            }

            SS_SendMsgCoords( hWnd, SSM_ROWHEIGHTCHANGE,
                              GetWindowLong( hWnd, GWL_ID ), ssRow, ssRow2 );
            SS_HiliteCell( hWnd, TRUE );
         }
         else                          // ssColumn being resized
         {
            x = lpSS->lResizeCurrentPos;
            lWidth = SS_GetColWidth( lpSS, lpSS->ssResizeCoord );
            if ( lpSS->BlockCellUL.ssCol != -1 &&
                 lpSS->BlockCellUL.ssRow == 0 &&
                 lpSS->BlockCellLR.ssRow == -1 )
            {
               ssCol = lpSS->BlockCellUL.ssCol;
               ssCol2 = lpSS->BlockCellLR.ssCol;
            }
            else
            {
               ssCol = lpSS->ssResizeCoord;
               ssCol2 = lpSS->ssResizeCoord;
            }

            fRedraw = lpSS->Redraw;
            SSSetBool( hWnd, SSB_REDRAW, FALSE );

            lWidth += x - lpSS->lResizeStartPos;

            for ( ss = ssCol; ss <= ssCol2; ss++ )
            {
               if ( SSGetUserResizeCol( hWnd, ss ) != SS_RESIZE_OFF )
               {
                  lWidthTemp = lWidth;

                  SS_CalcPixelsToColWidth( lpSS, ss, lWidthTemp, &dxVal );
                  SSSetColWidth( hWnd, ss - (lpSS->Col.nHeaderCnt - 1), dxVal );
               }
            }

            SSSetBool( hWnd, SSB_REDRAW, fRedraw );

            if ( lpSS->Col.ssCurrAt >= ssCol &&
                 lpSS->Col.ssCurrAt <= ssCol2 && lWidth == 0 )
            {
               SS_ScrollLineLR( hWnd, lpSS,
                                SS_F_SCROLL_RIGHT | SS_F_MOVEACTIVECELL );
               if ( lpSS->Col.ssCurrAt >= ssCol && lpSS->Col.ssCurrAt <= ssCol2 )
                  SS_ScrollLineUL( hWnd, lpSS,
                                   SS_F_SCROLL_LEFT | SS_F_MOVEACTIVECELL );
            }

            SS_SendMsgCoords( hWnd, SSM_COLWIDTHCHANGE,
                              GetWindowLong( hWnd, GWL_ID ), ssCol, ssCol2 );
            SS_HiliteCell( hWnd, TRUE );
         }

         lpSS->ModeResize = SS_MODERESIZE_OFF;
      }
#if 0 // SS_NODRAGDROP
      else
      if ( lpSS->fModeDragDrop )
      {
         ReleaseCapture( );
         lpSS->fModeDragDrop = FALSE;

         SS_DragDropFinish( hWnd, lpSS );
      }
#endif
      else
      if ( lpSS->ModeBlock )
      {
         ReleaseCapture( );
         lpSS->ModeBlock = FALSE;

         if ( lpSS->fMultipleBlocksSelected ||
              lpSS->BlockCellUL.ssCol != lpSS->BlockCellLR.ssCol ||
              lpSS->BlockCellUL.ssRow != lpSS->BlockCellLR.ssRow )
         {
            if ( lpSS->fMultipleBlocksSelected )
               SS_MultiSelBlockAddItem( lpSS, &lpSS->BlockCellUL,
                                        &lpSS->BlockCellLR );

            SS_SendMsgCommand( hWnd, 0, SSN_BLOCKSELECTED, FALSE );
         }
      }
      else
      if ( lpSS->ModeSelect )
      {
         ReleaseCapture( );
         lpSS->ModeSelect = FALSE;

         if ( lpSS->Row.ssCurrAt != lpSS->Row.PrevAt )
            if ( !SS_LeaveCell( hWnd, lpSS, lpSS->Col.PrevAt, lpSS->Col.PrevUL,
                                lpSS->Row.PrevAt, lpSS->Row.PrevUL, 0) )
            {
               SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
               InvalidateRect( hWnd, 0, TRUE );
            }
      }

//    DrUnlockTaskMemory( lpSS );
      break;

   case WM_RBUTTONDOWN:
   case WM_RBUTTONUP:
   case WM_RBUTTONDBLCLK:
      PostMessage( hWnd, SSM_PROCESSRBUTTON, uMsg, lParam );
      break;

   case WM_MOUSEMOVE:
      PostMessage( hWnd, SSM_PROCESSMOUSEMOVE, wParam, lParam );
      break;


   case WM_SETCURSOR:
      if ( SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ) )
         return( 1 );

      lpSS = SS_Lock( hWnd );

      hCursor = lpSS->hCursorCurrent;

      switch ( (WORD) hCursor )
      {
         case SS_CURSOR_DEFAULT:
            hCursor = lpSS->hCursorDefault;
            break;

         case SS_CURSOR_ARROW:
            hCursor = LoadCursor( 0, IDC_ARROW );
            break;

         case SS_CURSOR_DEFCOLRESIZE:
            hCursor = lpSS->hCursorResizeV;
            break;

         case SS_CURSOR_DEFROWRESIZE:
            hCursor = lpSS->hCursorResizeH;
            break;
      }

      switch ( (WORD) hCursor )
      {
         case SS_CURSOR_DEFAULT:
            hCursor = lpSS->hCursorPointer;
            break;

         case SS_CURSOR_ARROW:
            hCursor = LoadCursor( 0, IDC_ARROW );
            break;

         case SS_CURSOR_DEFCOLRESIZE:
            hCursor = lpSS->hCursorResizeV;
            break;

         case SS_CURSOR_DEFROWRESIZE:
            hCursor = lpSS->hCursorResizeH;
            break;
      }

      SetClassLong( hWnd, GCL_HCURSOR, (UINT) hCursor );
      SetCursor( hCursor );
//    DrUnlockTaskMemory( lpSS );
      return( 0 );

   case SSM_SETEDITMODE:
      lpSS = SS_Lock( hWnd );
      SS_ResetBlock( hWnd, TRUE );

      if ( wParam )
         SS_CellEditModeOn( hWnd, lpSS, 0, 0, 0L );
      else
      {
         if ( SS_CellEditModeOff( hWnd, lpSS, 0) )
            SS_HiliteCell( hWnd, TRUE );
      }

//    DrUnlockTaskMemory( lpSS );
      return( 0 );

   case SSM_NEXTCELL:
      SS_ResetBlock( hWnd, TRUE );
      SS_ScrollNextCell( hWnd );
      return( 0 );

   case SSM_NEXTROW:
      SS_ResetBlock( hWnd, TRUE );
      SSx_ScrollNextCell( hWnd, FALSE, TRUE );
      return( 0 );

   case SSM_PREVCELL:
      SS_ResetBlock( hWnd, TRUE );
      SS_ScrollPrevCell( hWnd );
      return( 0 );

   case SSM_PROCESSENTERKEY:
      SS_ProcessEnterKey( hWnd );
      return( 0 );

   case SSM_PROCESSTAB:
      lpSS = SS_Lock( hWnd );

      if ( SS_USESELBAR( lpSS ) )
      {
         fMoveActiveCell = FALSE;
         fSingleSel = TRUE;

         lpSS->Col.ssCurrAt = lpSS->Col.UL;
      }
      else
      if ( lpSS->wOpMode == SS_OPMODE_READONLY )
         fMoveActiveCell = FALSE;
      else
         fMoveActiveCell = TRUE;

      if ( lpSS->fProcessTab && fMoveActiveCell )
      {
         SS_ResetBlock( hWnd, TRUE );

         if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            SS_ScrollPrevCell( hWnd );
         else
            SS_ScrollNextCell( hWnd );
      }

      if ( mIs_hWnd( hWnd ) == FALSE )
         return( FALSE );

//    DrUnlockTaskMemory( lpSS );
      UpdateWindow( hWnd );
      return( 0 );

   case SSM_PROCESSKILLFOCUS:
      if ( (lpSS = SS_Lock( hWnd )) != 0 )
      {
         lpSS->hWndFocusPrev = 0;

#ifndef SS_NOPROCESSFOCUS
         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 )
#else
         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 )
#endif
         {
            UpdateWindow( hWnd );
            lpSS->fProcessingKillFocus = TRUE;

            if ( wParam == (WORD) GetParent( hWnd ) &&
                 GetFocus( ) != (HWND) wParam )
            {
               wParam = (WORD) GetFocus( );
            }
            else
            if ( wParam == 0 )
              wParam = (WORD) -1;

            SS_CellEditModeOff( hWnd, lpSS, (HWND) wParam );

            if ( mIs_hWnd( hWnd ) == FALSE )
               return( FALSE );

            lpSS->fProcessingKillFocus = FALSE;

#if 0
            if ( lpSS->fRetainSelBlock == 0 && !SS_USESELBAR( lpSS ) )
            {
               SS_DrawSelBlock( hWnd, 0, lpSS );
               lpSS->fSelBlockInvisible = TRUE;
            }
#endif

            SS_LeaveCell( hWnd, lpSS, -1, -1, -1, -1, zLEAVE_CELL_KILLFOCUS );

            if ( mIs_hWnd( hWnd ) == FALSE )
               return( FALSE );

            if ( SS_USESELBAR( lpSS ) )
            {
               lpSS->fProcessingKillFocus = TRUE;
               SS_HiliteCell( hWnd, FALSE );
               lpSS->fProcessingKillFocus = FALSE;
            }
            else
            if ( lpSS->fRetainSelBlock == 0 )
               SS_HiliteCell( hWnd, FALSE );

            SS_SendMsgCommand( hWnd, 0, SSN_KILLFOCUS, FALSE );

            lpSS->fFocusOn = FALSE;
         }

//       DrUnlockTaskMemory( lpSS );
      }

      break;

   case SSM_PROCESSSETFOCUS:
      if ( (lpSS = SS_Lock( hWnd )) != 0 )
      {
         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 )
         {
            fButtonDown = FALSE;
            if ( lpSS->fProcessingLButtonDown )
               fButtonDown = TRUE;

            if ( fButtonDown )
            {
               GetCursorPos( &pt );
               GetWindowRect( hWnd, &rect );

               if ( PtInRect( &rect, pt ) == FALSE )
                  fButtonDown = FALSE;
            }

            fNoHilite = FALSE;

            if ( lpSS->fFocusOn == 0 &&
                 lpSS->fMoveActiveOnFocus == 0 && fButtonDown )
            {
               lpSS->fNoProcessButtonDown = TRUE;
            }
            else
            if ( lpSS->fFocusOn == 0 &&
                 lpSS->fMoveActiveOnFocus && fButtonDown )
            {
               fNoHilite = TRUE;
            }
            if ( SS_IsBlockSelected( lpSS ) )
            {
               if ( lpSS->fRetainSelBlock == 0 )
               {
                  if ( lpSS->fSelBlockInvisible )
                     SS_DrawSelBlock( hWnd, 0, lpSS );

                  SS_HiliteCell( hWnd, TRUE );
               }
            }
            else
            if ( !fNoHilite )
            {
               if ( lpSS->fInvalidated )
                  UpdateWindow( hWnd );

               if ( SS_USESELBAR( lpSS ) )
               {
                  if ( lpSS->fKillFocusReceived )
                  {
                     fHiliteOnOld = lpSS->HiliteOn;

                     if ( lpSS->HiliteOn )
                     {
                        lpSS->HiliteOn = FALSE;
                        lpSS->fProcessingKillFocus = TRUE;
                     }

                     if ( SS_HiliteCell( hWnd, TRUE) )
                        lpSS->fKillFocusReceived = FALSE;

                     if ( fHiliteOnOld )
                     {
                        lpSS->HiliteOn = TRUE;
                        lpSS->fProcessingKillFocus = FALSE;
                     }

                  }
               }
               else
               if ( lpSS->EditModeOn && lpSS->fEditModePermanent == 0 )
                  ;
               else
                  SS_HiliteCell( hWnd, TRUE );
            }

            if ( mIs_hWnd( hWnd ) == FALSE )
               return( 0 );

            SS_SendMsgCommand( hWnd, 0, SSN_SETFOCUS, FALSE );

            lpSS->fFocusOn = TRUE;
         }
         else
         if ( lpSS->wMessageBeingSent )
            lpSS->fSetFocusWhileMsgBeingSent = TRUE;

//       DrUnlockTaskMemory( lpSS );
      }

      break;

   case SSM_PROCESSLBUTTONDBLCLK:
   // TraceLineS( "SSM_LBUTTONDBLCLK", "" );
   case SSM_PROCESSLBUTTONDOWN:
   // TraceLineS( "SSM_PROCESSLBUTTONDOWN", " WM_LBUTTONDOWN" );
      return( SS_ProcessLButton( hWnd, uMsg, wParam, lParam) );

   case SSM_PROCESSMOUSEMOVE:
      if ( (lpSS = SS_Lock( hWnd ) ) != 0 )
      {
         if ( lpSS->ModeResize )
            SS_Resize( hWnd, lpSS, SS_RESIZEMODE_UPDATE, lParam );
         else
         if ( wParam & MK_LBUTTON && (lpSS->ModeBlock ||
                  lpSS->ModeSelect || lpSS->fModeDragDrop ||
                  lpSS->bModeScrollArrow) )
         {
            SS_GetClientRect( hWnd, &rectClient );

            ptClient.x = 0;
            ptClient.y = 0;

            ClientToScreen( hWnd, &ptClient );

            dwTime = GetTickCount( );
            while ( dwTime + SS_BLOCKMODE_INITIALDELAY > GetTickCount() )
               ;

            for ( ;; )
            {
               if ( GetSystemMetrics( SM_SWAPBUTTON) )
               {
                  if ( (GetAsyncKeyState( VK_RBUTTON ) & 0x8000 ) == 0 )
                     break;
               }
               else
               if ( (GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) == 0 )
                  break;

               GetCursorPos( &pt );

               pt.x -= ptClient.x;
               pt.y -= ptClient.y;

               if ( lpSS->fModeDragDrop )
               {
#if 0 // SS_NODRAGDROP
                  SS_CELLCOORD BlockCellLR;
                  SS_CELLCOORD NewPos;

                  BlockCellLR.ssCol = lpSS->DragDropCurrentCol;
                  BlockCellLR.ssRow = lpSS->DragDropCurrentRow;
                  if ( SS_ScrollWithMouse( hWnd, lpSS, &BlockCellLR,
                                           &BlockCellLR, &NewPos,
                                           pt.x, pt.y) )
                  {
                     if ( lpSS->DragDropBlockUL.ssCol +
                          (NewPos.ssCol - lpSS->DragDropCurrentCol) >=
                                                      lpSS->Col.nHeaderCnt &&
                          lpSS->DragDropBlockUL.ssCol +
                            (lpSS->BlockCellLR.ssCol - lpSS->BlockCellUL.ssCol) +
                            (NewPos.ssCol - lpSS->DragDropCurrentCol) <=
                                                         lpSS->Col.Max + 1 )
                     {
                        lpSS->DragDropBlockUL.ssCol +=
                                       NewPos.ssCol - lpSS->DragDropCurrentCol;
                     }

                     if ( lpSS->DragDropBlockUL.ssRow +
                          (NewPos.ssRow - lpSS->DragDropCurrentRow ) >=
                                                      lpSS->Row.nHeaderCnt &&
                          lpSS->DragDropBlockUL.ssRow +
                          (lpSS->BlockCellLR.ssRow - lpSS->BlockCellUL.ssRow) +
                          (NewPos.ssRow - lpSS->DragDropCurrentRow) <=
                                                      lpSS->Row.Max + 1 )
                     {
                        lpSS->DragDropBlockUL.ssRow +=
                                       NewPos.ssRow - lpSS->DragDropCurrentRow;
                     }

                     lpSS->DragDropCurrentCol = NewPos.ssCol;
                     lpSS->DragDropCurrentRow = NewPos.ssRow;

                     SS_DragDropDrawBox( hWnd, lpSS, TRUE );
                  }
#endif
               }
               else
               if ( SS_USESELBAR( lpSS ) )
               {
#ifndef SS_NOLISTBOX
                  if ( lpSS->fSingleSelRowFound )
                  {
                     SS_CELLCOORD BlockCellLR;
                     SS_CELLCOORD NewPos;

                     BlockCellLR.ssCol = lpSS->Col.ssCurrAt;
                     BlockCellLR.ssRow = lpSS->Row.ssCurrAt;
                     if ( SS_ScrollWithMouse( hWnd, lpSS, &BlockCellLR,
                                              &BlockCellLR, &NewPos,
                                              pt.x, pt.y) )
                     {
                        if ( NewPos.ssRow != lpSS->Row.ssCurrAt )
                        {
                           if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                              SSx_SelectRow( hWnd, lpSS, NewPos.ssRow );

                           SS_HiliteCell( hWnd, FALSE );
                           lpSS->Row.ssCurrAt = NewPos.ssRow;
                           SS_HiliteCell( hWnd, TRUE );
                        }
                     }
                  }
#endif
               }
               else
                  SS_SelectBlock( hWnd, lpSS, pt.x, pt.y );
            }

            if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            {
               LPSS_ROW lpRowTemp;

               for ( ss = lpSS->BlockCellUL.ssRow;
                     ss <= lpSS->BlockCellLR.ssRow; ss++ )
               {
                  if ( (lpRowTemp = SS_LockRowItem( lpSS, ss )) != 0 )
                  {
                     if ( lpRowTemp->fRowSelected == 3 )
                        lpRowTemp->fRowSelected = TRUE;
                     else
                     if ( lpRowTemp->fRowSelected == 2 )
                        lpRowTemp->fRowSelected = FALSE;

                     SS_UnlockRowItem( lpSS, ss );
                  }
               }
            }
         }

         lpSS->wMsgLast = 0;
//       DrUnlockTaskMemory( lpSS );
      }

      return( 0 );

   case SSM_PROCESSRBUTTON:
      uMsg = wParam;
      lpSS = SS_Lock( hWnd );

      if ( uMsg == WM_RBUTTONUP )
      {
         if ( lpSS->fRButtonDown == 0 )
         {
//          DrUnlockTaskMemory( lpSS );
            return( 0 );
         }

         lpSS->fRButtonDown = FALSE;
      }
      else
         lpSS->fRButtonDown = TRUE;

      if ( uMsg == WM_RBUTTONUP )
         RButton.nRButtonType = RBUTTONTYPE_UP;
      else
      if ( uMsg == WM_RBUTTONDOWN )
         RButton.nRButtonType = RBUTTONTYPE_DOWN;
      else
      if ( uMsg == WM_RBUTTONDBLCLK )
         RButton.nRButtonType = RBUTTONTYPE_DBLCLK;

      RButton.nPosX = LOWORD( lParam );
      RButton.nPosY = HIWORD( lParam );

      SS_GetCellFromPixel( hWnd, lpSS, &RButton.ssCol, &RButton.ssRow,
                           &x, &y, RButton.nPosX, RButton.nPosY );

      if ( RButton.ssCol != -2 && RButton.ssRow != -2 )
      {
         if ( RButton.ssCol == -1 )
            RButton.ssCol = SS_HEADER;

         if ( RButton.ssRow == -1 )
            RButton.ssRow = SS_HEADER;

         SS_SendMsg( hWnd, SSM_RBUTTON, GetWindowLong( hWnd, GWL_ID ),
                     (LPARAM) &RButton );
      }

//    DrUnlockTaskMemory( lpSS );
      return( 0 );

   case WM_COMMAND:
      switch ( HIWORD( lParam) )
      {
         case BN_CLICKED:
            lpSS = SS_Lock( hWnd );

//          if ( lpSS->EditModeTurningOn == 0 &&
//               (nType = SS_TypeGetControlType( lpSS, (zSHORT) wParam )) != -1 )

            if ( lpSS->fIgnoreBNClicked == 0 &&
                 (nType = SS_TypeGetControlType( lpSS, (zSHORT) wParam )) != -1 )
            {
               lpSS->CurCellSendingMsg.ssCol = lpSS->Col.ssCurrAt;
               lpSS->CurCellSendingMsg.ssRow = lpSS->Row.ssCurrAt;

               if ( nType == SS_TYPE_BUTTON )
               {
                  if ( SendMessage( (HWND) LOWORD( lParam ), SBM_GETBUTTONSTATE, 0, 0L ) )
                     SS_SendMsgCommand( hWnd, lpSS, SSN_BUTTONDOWN, TRUE );
                  else
                     SS_SendMsgCommand( hWnd, lpSS, SSN_BUTTONUP, TRUE );

                  if ( mIs_hWnd( hWnd ) == FALSE )
                     return( 0 );
               }
               else
               if ( nType == SS_TYPE_CHECKBOX )
               {
                  if ( SendMessage( (HWND) LOWORD( lParam ), BM_GETSTATE, 0, 0L) )
                     SS_SendMsgCommand( hWnd, lpSS, SSN_BUTTONDOWN, TRUE );
                  else
                     SS_SendMsgCommand( hWnd, lpSS, SSN_BUTTONUP, TRUE );
               }
            }

//          DrUnlockTaskMemory( lpSS );
            return( 0 );

         case BN_CHECKBOXTYPING:
            SS_SendMsgCommand( hWnd, 0, SSN_CHECKBOXTYPING, FALSE );
            break;

         case EN_INVALIDDATA:
            SS_SendMsgCommand( hWnd, 0, SSN_INVALIDDATA, FALSE );
            break;
      }

      break;

   case WM_ERASEBKGND:
      return( TRUE );   // we erase our own background

   // Paint Spreadsheet
   case WM_PAINT:
      if ( !SS_Paint( hWnd, (HDC) wParam ) )
         return( FALSE );

      break;

   case WM_DESTROY:
      if ( (lpSS = SS_Lock( hWnd )) != 0 )
      {
#ifndef SS_NOCT_CHECK
         if ( lpSS->fCheckBoxDefBitmapsLoaded )
            CheckBoxUnloadDefBitmaps( );
#endif

         SS_CellEditModeOff( hWnd, lpSS, (HWND) -1 );
//       DrUnlockTaskMemory( lpSS );
      }

      if ( mIs_hWnd( hWnd ) == FALSE )
         return( FALSE );

      SS_Free( hWnd );

      break;
   }

   return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zLONG
SS_KeyDown( HWND    hWnd,
            WPARAM  wParam,
            LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_KeyDown" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_CELLCOORD  BlockCellUL;
   SS_CELLCOORD  BlockCellLR;
   SS_COORD      ssRowAtTemp;
   SS_COORD      ssRowAtOld;
   SS_COORD      ssColAtOld;
   SS_COORD      ssRowAtNew;
   SS_COORD      ssColAtNew;
   SS_COORD      ssColAtTemp;
   SS_COORD      ssColLeftOld;
   SS_COORD      ssRowTopOld;
   SS_COORD      BlockCellCurrentCol;
   SS_COORD      BlockCellCurrentRow;
   SS_COORD      ssRowCnt;
   SS_COORD      ssColCnt;
   SS_COORD      ssRowAt;
   SS_COORD      ssColAt;
   RECT          rect;
   WORD          wShift = 0;
   zLONG         lRet;
   zBOOL         fMoveActiveCell;
   zBOOL         fSingleSel;
   zBOOL         fRedrawOld;
   zLONG         lRC = -1;
   zLONG         lWidth;
   zLONG         lHeight;
   zLONG         x;
   zLONG         y;

   if ( GetKeyState( VK_SHIFT ) & 0x8000 )
   {
      wShift |= 0x01;

      if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
         lpSS->ModeBlock = TRUE;

      else
      if ( SS_IsBlockSelected( lpSS ) )
         lpSS->ModeBlock = TRUE;
      else
      if ( (lpSS->wSelBlockOption & SS_SELBLOCK_BLOCKS) &&
           lpSS->ModeBlock == 0 && lpSS->Col.Max > 0 &&
           lpSS->Row.Max > 0 )
      {
         lpSS->Col.BlockCellCurrentPos = lpSS->Col.ssCurrAt;
         lpSS->Row.BlockCellCurrentPos = lpSS->Row.ssCurrAt;
         lpSS->BlockCellUL.ssCol = lpSS->Col.ssCurrAt;
         lpSS->BlockCellUL.ssRow = lpSS->Row.ssCurrAt;
         lpSS->BlockCellLR.ssCol = lpSS->Col.ssCurrAt;
         lpSS->BlockCellLR.ssRow = lpSS->Row.ssCurrAt;
         lpSS->ModeBlock = TRUE;
      }
   }
   else
   if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
      lpSS->fExtSelHilite = TRUE;

   if ( GetKeyState( VK_CONTROL ) & 0x8000 )
      wShift |= 0x02;

   lpSS->Col.ULPrev = lpSS->Col.UL;
   lpSS->Row.ULPrev = lpSS->Row.UL;
   lRet = SS_SendMsg( hWnd, SSM_KEYDOWN, GetWindowLong( hWnd, GWL_ID ),
                      MAKELONG( wParam, wShift) );

   if ( lRet )
   {
//    DrUnlockTaskMemory( lpSS );
      return( 0 );
   }

   fSingleSel = FALSE;

   if ( SS_USESELBAR( lpSS ) )
   {
      fMoveActiveCell = FALSE;
      fSingleSel = TRUE;

      lpSS->Col.ssCurrAt = lpSS->Col.UL;
   }
   else
   if ( lpSS->wOpMode == SS_OPMODE_READONLY )
      fMoveActiveCell = FALSE;
   else
      fMoveActiveCell = TRUE;

   lpSS->wMsgLast = WM_KEYDOWN;

   if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
   {
      if ( wParam == 'x' || wParam == 'X' )
      {
         if ( lpSS->fAutoClipboard )
            SSClipboardCut( hWnd );

         lpSS->wMsgLast = 0;
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }
      else
      if ( wParam == 'c' || wParam == 'C' )
      {
         if ( lpSS->fAutoClipboard )
            SSClipboardCopy( hWnd );

         lpSS->wMsgLast = 0;
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }
      else
      if ( wParam == 'v' || wParam == 'V' )
      {
         if ( lpSS->fAutoClipboard )
            SSClipboardPaste( hWnd );

         lpSS->wMsgLast = 0;
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }
   }

   if ( (HIBYTE( GetKeyState( VK_CONTROL ) ) &&
         (wParam == 'z' || wParam == 'Z')) ||
        ((lParam & 0x20000000L) && wParam == VK_BACK) )
   {
      if ( lpSS->fAllowUndo )
         SS_Undo( hWnd, lpSS );

      lpSS->wMsgLast = 0;
//    DrUnlockTaskMemory( lpSS );
      return( 0 );
   }

#ifdef SS_GP
   if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
        (wParam == 'z' || wParam == 'Z') )
      wParam = VK_ESCAPE;
#endif

   lpSS->wScrollDirection = 0;

   if ( lpSS->nVKeyIns && wParam == (WORD) lpSS->nVKeyIns &&
        !HIBYTE( GetKeyState( VK_CONTROL ) ) &&
        !HIBYTE( GetKeyState( VK_SHIFT ) ) )
   {
      if ( SS_IsBlockSelected( lpSS ) )
      {
         SS_COORD InsCnt;

         fRedrawOld = lpSS->Redraw;
         lpSS->Redraw = FALSE;

         if ( lpSS->BlockCellUL.ssCol == -1 || lpSS->BlockCellLR.ssCol == -1 )
         {
            InsCnt = lpSS->BlockCellLR.ssRow - lpSS->BlockCellUL.ssRow + 1;
            if ( InsCnt > 0 )
            {
               if ( !SS_SendMsgInsDel( hWnd, SS_INSDEL_TYPE_INS,
                                       SS_INSDEL_OP_KEY, SS_INSDEL_ROW,
                                       lpSS->BlockCellUL.ssRow,
                                       lpSS->BlockCellLR.ssRow) )
               {
                  SSSetMaxRows( hWnd, SS_GetRowsMax( lpSS ) - 1 + InsCnt );
                  SSInsRowRange( hWnd, lpSS->BlockCellUL.ssRow,
                                 lpSS->BlockCellLR.ssRow );
               }
            }
         }
         else
         if ( lpSS->BlockCellUL.ssRow == -1 || lpSS->BlockCellLR.ssRow == -1 )
         {
            InsCnt = lpSS->BlockCellLR.ssCol - lpSS->BlockCellUL.ssCol + 1;
            if ( InsCnt > 0 )
            {
               if ( !SS_SendMsgInsDel( hWnd, SS_INSDEL_TYPE_INS,
                                       SS_INSDEL_OP_KEY, SS_INSDEL_COL,
                                       lpSS->BlockCellUL.ssCol,
                                       lpSS->BlockCellLR.ssCol) )
               {
                  SSSetMaxCols( hWnd, SS_GetColsMax( lpSS ) - 1 + InsCnt );
                  SSInsColRange( hWnd, lpSS->BlockCellUL.ssCol,
                                 lpSS->BlockCellLR.ssCol );
               }
            }
         }

         SSSetBool( hWnd, SSB_REDRAW, fRedrawOld );
      }
   }
   else
   if ( lpSS->nVKeyDel && wParam == (WORD) lpSS->nVKeyDel &&
        !HIBYTE( GetKeyState( VK_CONTROL) ) &&
        !HIBYTE( GetKeyState( VK_SHIFT) ) )
   {
      SS_VBDelBlock( hWnd, lpSS, SS_INSDEL_OP_KEY );
   }

   if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && lpSS->ModeBlock &&
        (wParam == VK_UP || wParam == VK_DOWN || wParam == VK_PRIOR ||
         wParam == VK_NEXT || wParam == VK_HOME || wParam == VK_END) )
   {
      SS_HiliteCell( hWnd, FALSE );
   }

// DrUnlockTaskMemory( lpSS );
   switch ( wParam )
   {
      case VK_SHIFT:
         lpSS = SS_Lock( hWnd );
         lpSS->BlockCellULPrev.ssCol = lpSS->BlockCellUL.ssCol;
         lpSS->BlockCellULPrev.ssRow = lpSS->BlockCellUL.ssRow;
         lpSS->BlockCellLRPrev.ssCol = lpSS->BlockCellLR.ssCol;
         lpSS->BlockCellLRPrev.ssRow = lpSS->BlockCellLR.ssRow;

   //      if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
   //         SS_ExtSelBegin( hWnd, lpSS );

         lpSS->fExtSelHilite = TRUE;

//       DrUnlockTaskMemory( lpSS );
         break;

      case VK_SPACE:
         if ( lpSS->wOpMode == SS_OPMODE_NORMAL )
         {
            if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
                 HIBYTE( GetKeyState( VK_SHIFT ) ) )
            {
               if ( SS_IsBlockSelected( lpSS ) &&
                    lpSS->BlockCellUL.ssCol == 0 &&
                    lpSS->BlockCellUL.ssRow == 0 &&
                    lpSS->BlockCellLR.ssCol == -1 &&
                    lpSS->BlockCellLR.ssRow == -1 )
               {
                  ;
               }
               else
               {
                  BlockCellUL.ssCol = 0;
                  BlockCellUL.ssRow = 0;
                  BlockCellLR.ssCol = -1;
                  BlockCellLR.ssRow = -1;
                  SSSetSelectBlock( hWnd, &BlockCellUL, &BlockCellLR );

                  SS_SendMsgCommand( hWnd, 0, SSN_BLOCKCHANGED, FALSE );
               }

               return( 1 );
            }

            if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            {
               if ( SS_IsBlockSelected( lpSS ) &&
                    lpSS->BlockCellUL.ssCol == 0 &&
                    lpSS->BlockCellUL.ssRow != -1 &&
                    lpSS->BlockCellLR.ssCol == -1 &&
                    lpSS->BlockCellLR.ssRow != -1 )
               {
                  ;
               }
               else
               {
                  BlockCellUL.ssCol = 0;

                  if ( SS_IsBlockSelected( lpSS ) )
                     BlockCellUL.ssRow = lpSS->BlockCellUL.ssRow;
                  else
                     BlockCellUL.ssRow = lpSS->Row.ssCurrAt;

                  BlockCellLR.ssCol = -1;

                  if ( SS_IsBlockSelected( lpSS ) )
                     BlockCellLR.ssRow = lpSS->BlockCellLR.ssRow;
                  else
                     BlockCellLR.ssRow = lpSS->Row.ssCurrAt;

                  SSSetSelectBlock( hWnd, &BlockCellUL, &BlockCellLR );

                  SS_SendMsgCommand( hWnd, 0, SSN_BLOCKCHANGED, FALSE );
               }

               return( 1 );
            }
            else
            if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            {
               if ( SS_IsBlockSelected( lpSS ) &&
                    lpSS->BlockCellUL.ssCol != -1 &&
                    lpSS->BlockCellUL.ssRow == 0 &&
                    lpSS->BlockCellLR.ssCol != -1 &&
                    lpSS->BlockCellLR.ssRow == -1 )
               {
                  ;
               }
               else
               {
                  if ( SS_IsBlockSelected( lpSS ) )
                     BlockCellUL.ssCol = lpSS->BlockCellUL.ssCol;
                  else
                     BlockCellUL.ssCol = lpSS->Col.ssCurrAt;

                  BlockCellUL.ssRow = 0;

                  if ( SS_IsBlockSelected( lpSS ) )
                     BlockCellLR.ssCol = lpSS->BlockCellLR.ssCol;
                  else
                     BlockCellLR.ssCol = lpSS->Col.ssCurrAt;

                  BlockCellLR.ssRow = -1;
                  SSSetSelectBlock( hWnd, &BlockCellUL, &BlockCellLR );

                  SS_SendMsgCommand( hWnd, 0, SSN_BLOCKCHANGED, FALSE );
                  SS_SendMsgCommand( hWnd, 0, SSN_BLOCKSELECTED, FALSE );
               }

               return( 1 );
            }
         }
#ifndef SS_NOLISTBOX
         else
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
         {
            if ( !HIBYTE( GetKeyState( VK_CONTROL ) ) )
               SS_ResetBlock( hWnd, TRUE );

            SS_SelectRow( hWnd, lpSS );
         }
#endif

         break;

      // Enter Key - deactivate the active cell.
      case VK_RETURN:
         lpSS = SS_Lock( hWnd );
         SS_ResetBlock( hWnd, TRUE );

         if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
         {
            if ( mIs_hWnd( hWnd ) == FALSE )
               return( 0 );

            SS_HiliteCell( hWnd, TRUE );
         }
         else
         {
            if ( HIBYTE( GetKeyState( VK_CONTROL ) ) ||
                 lpSS->wEnterAction == SS_ENTERACTION_NONE ||
                 lpSS->wEnterAction == SS_ENTERACTION_SAME )
            {
               x  = SS_GetCellPosX( lpSS, lpSS->Col.UL, lpSS->Col.ssCurrAt );
               y  = SS_GetCellPosY( lpSS, lpSS->Row.UL, lpSS->Row.ssCurrAt );

               lpSS->fNoSetCapture = TRUE;
               SendMessage( hWnd, WM_LBUTTONDBLCLK, 0, MAKELONG( x + 1, y + 1 ) );
            }
            else
               PostMessage( hWnd, SSM_PROCESSENTERKEY, 0, 0L );
         }

//       DrUnlockTaskMemory( lpSS );
         UpdateWindow( hWnd );
         lRC = 1;
         break;

      case VK_ESCAPE:
         return( 0 );

      case VK_TAB:
         lpSS = SS_Lock( hWnd );

         if ( lpSS->fProcessTab )
         {
            if ( fMoveActiveCell )
            {
               SS_ResetBlock( hWnd, TRUE );

               if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
                  SS_ScrollPrevCell( hWnd );
               else
                  SS_ScrollNextCell( hWnd );

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( 0 );

               UpdateWindow( hWnd );
            }
         }
         else
         {
            if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
               PostMessage( GetParent( hWnd ), WM_NEXTDLGCTL, 1, 0L );
            else
               PostMessage( GetParent( hWnd ), WM_NEXTDLGCTL, 0, 0L );
         }

//       DrUnlockTaskMemory( lpSS );
         lRC = 0;
         break;

      // Left key - maps to horizontal scroll code
      case VK_LEFT:
         lpSS = SS_Lock( hWnd );
         SS_ScrollLineUL( hWnd, lpSS, SS_F_SCROLL_LEFT |
                          (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );
//       DrUnlockTaskMemory( lpSS );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

         UpdateWindow( hWnd );
         lRC = 0;
         break;

      // Right key - maps to horizontal scroll code
      case VK_RIGHT:
         lpSS = SS_Lock( hWnd );
         SS_ScrollLineLR( hWnd, lpSS, SS_F_SCROLL_RIGHT |
                          (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );

//       DrUnlockTaskMemory( lpSS );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

         UpdateWindow( hWnd );
         lRC = 0;
         break;

      // Up key - maps to vertical scroll code
      case VK_UP:
         lpSS = SS_Lock( hWnd );
         SS_ScrollLineUL( hWnd, lpSS, SS_F_SCROLL_UP |
                          (fSingleSel ? SS_F_MOVEACTIVECELL :
                          (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0)) );
         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

#ifndef SS_NOLISTBOX
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            SS_SelectRow( hWnd, lpSS );
#endif

//       DrUnlockTaskMemory( lpSS );
         UpdateWindow( hWnd );
         lRC = 0;
         break;

      // Down key - maps to vertical scroll code
      case VK_DOWN:
         lpSS = SS_Lock( hWnd );
         SS_ScrollLineLR( hWnd, lpSS, SS_F_SCROLL_DOWN |
                          (fSingleSel ? SS_F_MOVEACTIVECELL :
                          (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0)) );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

#ifndef SS_NOLISTBOX
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            SS_SelectRow( hWnd, lpSS );
#endif

//       DrUnlockTaskMemory( lpSS );
         UpdateWindow( hWnd );
         lRC = 0;
         break;

      // Page Up key - maps to vertical scroll code
      case VK_PRIOR:
         lpSS = SS_Lock( hWnd );

         if ( lpSS->ModeBlock == 0 )
            SS_ResetBlock( hWnd, TRUE );

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            SS_ScrollPageUL( hWnd, lpSS, SS_F_SCROLL_LEFT |
                             (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );
         else
            SS_ScrollPageUL( hWnd, lpSS, SS_F_SCROLL_UP |
                             (fSingleSel ? SS_F_MOVEACTIVECELL :
                             (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0)) );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

#ifndef SS_NOLISTBOX
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            SS_SelectRow( hWnd, lpSS );
#endif

//       DrUnlockTaskMemory( lpSS );
         UpdateWindow( hWnd );
         lRC = 0;
         break;

      // Page Down key - maps to vertical scroll code
      case VK_NEXT:
         lpSS = SS_Lock( hWnd );

         if ( lpSS->ModeBlock == 0 )
            SS_ResetBlock( hWnd, TRUE );

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            SS_ScrollPageLR( hWnd, lpSS, SS_F_SCROLL_RIGHT |
                             (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );
         else
            SS_ScrollPageLR( hWnd, lpSS, SS_F_SCROLL_DOWN |
                             (fSingleSel ? SS_F_MOVEACTIVECELL :
                             (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0)) );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

#ifndef SS_NOLISTBOX
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            SS_SelectRow( hWnd, lpSS );
#endif

//       DrUnlockTaskMemory( lpSS );

         UpdateWindow( hWnd );
         lRC = 0;
         break;

      // Home - Go to beginning of spreadsheet
      case VK_HOME:
      {
         lpSS = SS_Lock( hWnd );

         if ( lpSS->ModeBlock == 0 &&
              (fMoveActiveCell || lpSS->wOpMode == SS_OPMODE_EXTSEL) )
         {
            SS_ResetBlock( hWnd, TRUE );
         }

         SS_CellEditModeOff( hWnd, lpSS, 0 );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
         {
            ssRowTopOld = lpSS->Row.UL;
            ssColLeftOld = lpSS->Col.UL;
            ssRowAtOld = lpSS->Row.ssCurrAt;
            ssColAtOld = lpSS->Col.ssCurrAt;

            if ( lpSS->ModeBlock )
            {
               ssColAtTemp = lpSS->Col.BlockCellCurrentPos;
               ssRowAtTemp = lpSS->Row.BlockCellCurrentPos;
            }
            else
            {
               ssColAtTemp = lpSS->Col.ssCurrAt;
               ssRowAtTemp = lpSS->Row.ssCurrAt;
            }

            ssColAtNew = lpSS->Col.nHeaderCnt;
            ssRowAtNew = lpSS->Row.nHeaderCnt;

            while ( ssColAtNew < SS_GetColsMax( lpSS ) - 1 &&
                    SS_GetColWidth( lpSS, ssColAtNew ) == 0 )
               ssColAtNew++;

            while ( ssRowAtNew < SS_GetRowsMax( lpSS ) - 1 &&
                    SS_GetRowHeight( lpSS, ssRowAtNew ) == 0 )
            {
               ssRowAtNew++;
            }

            if ( (lpSS->Col.UL == lpSS->Col.nHeaderCnt +
                  lpSS->Col.Frozen && ssColAtTemp <= lpSS->Col.LR) &&
                 (lpSS->Row.UL == lpSS->Row.nHeaderCnt +
                  lpSS->Row.Frozen && ssRowAtTemp <= lpSS->Row.LR) )
            {
               if ( lpSS->ModeBlock )
               {
                  if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                     lpSS->Row.ssCurrAt = ssRowAtNew;

                  SSx_SelectBlock( hWnd, lpSS, ssColAtNew, ssRowAtNew );
               }
               else
               if ( fMoveActiveCell || fSingleSel )
               {
                  SS_HiliteCell( hWnd, FALSE );
                  lpSS->Col.ssCurrAt = ssColAtNew;
                  lpSS->Row.ssCurrAt = ssRowAtNew;
                  if ( SS_LeaveCell( hWnd, lpSS, ssColAtOld, ssColLeftOld,
                                     ssRowAtOld, ssRowTopOld, 0 ) )
                     SS_HiliteCell( hWnd, TRUE );

                  if ( mIs_hWnd( hWnd ) == FALSE )
                     return( 0 );
               }
            }
            else
            if ( lpSS->Row.UL > lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ||
                 lpSS->Col.UL > lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
            {
               if ( lpSS->ModeBlock )
               {
                  BlockCellCurrentRow = ssRowAtNew;
                  BlockCellCurrentCol = ssColAtNew;
                  if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                     lpSS->Row.ssCurrAt = ssRowAtNew;
               }
               else
               if ( fMoveActiveCell || fSingleSel )
               {
                  SS_HiliteCell( hWnd, FALSE );

                  lpSS->Row.ssCurrAt = ssRowAtNew;
                  lpSS->Col.ssCurrAt = ssColAtNew;
               }

               lpSS->Row.UL = lpSS->Row.nHeaderCnt + lpSS->Row.Frozen;
               lpSS->Col.UL = lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;

               if ( lpSS->ModeBlock ||
                    SS_LeaveCell( hWnd, lpSS, ssColAtOld, ssColLeftOld,
                                  ssRowAtOld, ssRowTopOld, 0) )
               {
                  if ( ssRowTopOld > lpSS->Row.nHeaderCnt + lpSS->Row.Frozen &&
                       ssColLeftOld > lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
                  {
                     lpSS->wScrollDirection = SS_VSCROLL_HOME;
                     SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
                     SS_TopLeftChange( hWnd );
                  }
                  else
                  if ( ssRowTopOld > lpSS->Row.nHeaderCnt +
                           lpSS->Row.Frozen )
                  {
                     lpSS->wScrollDirection = SS_VSCROLL_HOME;
                     SS_InvalidateScrollArea( hWnd, lpSS,
                                              SS_SCROLLAREA_ROWS );
                     SS_TopRowChange( hWnd );
                  }
                  else
                  if ( ssColLeftOld > lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
                  {
                     SS_InvalidateScrollArea( hWnd, lpSS,
                                              SS_SCROLLAREA_COLS );
                     SS_LeftColChange( hWnd );
                  }

                  SS_SetVScrollBar( hWnd, lpSS );
                  SS_SetHScrollBar( hWnd, lpSS );
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( 0 );

               if ( lpSS->ModeBlock )
                  SSx_SelectBlock( hWnd, lpSS, BlockCellCurrentCol,
                                   BlockCellCurrentRow );
            }
         }
         else
         {
            SS_ScrollHome( hWnd, lpSS, SS_F_SCROLL_ROW |
                           (fSingleSel ? SS_F_MOVEACTIVECELL :
                           (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0)) );
         }

#ifndef SS_NOLISTBOX
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            SS_SelectRow( hWnd, lpSS );
#endif

//       DrUnlockTaskMemory( lpSS );

         UpdateWindow( hWnd );
         lRC = 0;
         break;
      }

      // End - Go to end of SpreadSheet
      case VK_END:
      {
         lpSS = SS_Lock( hWnd );

         if ( lpSS->ModeBlock == 0 &&
              (fMoveActiveCell || lpSS->wOpMode == SS_OPMODE_EXTSEL) )
         {
            SS_ResetBlock( hWnd, TRUE );
         }

         SS_CellEditModeOff( hWnd, lpSS, 0 );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

         if ( HIBYTE( GetKeyState( VK_CONTROL) ) )
         {
            ssRowTopOld = lpSS->Row.UL;
            ssColLeftOld = lpSS->Col.UL;
            ssRowAtOld = lpSS->Row.ssCurrAt;
            ssColAtOld = lpSS->Col.ssCurrAt;

            ssColCnt = lpSS->Col.DataCnt;

            if ( ssColCnt == lpSS->Col.nHeaderCnt && ssColCnt <= lpSS->Col.Max )
            {
               ssColCnt++;

               while ( ssColCnt - 1 < SS_GetColsMax( lpSS ) - 1 &&
                       SS_GetColWidth( lpSS, ssColCnt - 1 ) == 0 )
               {
                  ssColCnt++;
               }
            }

            while ( ssColCnt - 1 > lpSS->Col.nHeaderCnt &&
                   SS_GetColWidth( lpSS, ssColCnt - 1 ) == 0 )
               ssColCnt--;

            ssRowCnt = lpSS->Row.DataCnt;

            if ( ssRowCnt == lpSS->Row.nHeaderCnt && ssRowCnt <= lpSS->Row.Max )
               ssRowCnt++;

            while ( ssRowCnt - 1 > lpSS->Row.nHeaderCnt &&
                    SS_GetRowHeight( lpSS, ssRowCnt - 1 ) == 0 )
            {
               ssRowCnt--;
            }

            if ( ssColCnt - 1 >= lpSS->Col.UL &&
                 ssColCnt - 1 <= lpSS->Col.LRAllVis &&
                 ssRowCnt - 1 >= lpSS->Row.UL &&
                 ssRowCnt - 1 <= lpSS->Row.LRAllVis )
            {
               if ( lpSS->ModeBlock )
               {
                  if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                     lpSS->Row.ssCurrAt = ssRowCnt - 1;

                  SSx_SelectBlock( hWnd, lpSS, ssColCnt - 1, ssRowCnt - 1 );
               }
               else
               if ( fMoveActiveCell || fSingleSel )
               {
                  SS_HiliteCell( hWnd, FALSE );
                  lpSS->Col.ssCurrAt = ssColCnt - 1;
                  lpSS->Row.ssCurrAt = ssRowCnt - 1;
                  if ( SS_LeaveCell( hWnd, lpSS, ssColAtOld, ssColLeftOld,
                                     ssRowAtOld, ssRowTopOld, 0 ) )
                     SS_HiliteCell( hWnd, TRUE );

                  if ( mIs_hWnd( hWnd ) == FALSE )
                     return( 0 );
               }
            }
            else
            {
               SS_GetClientRect( hWnd, &rect );
               SS_HiliteCell( hWnd, FALSE );

               // determine new x coordinates
               x = SS_GetCellPosX( lpSS, lpSS->Col.ssCurrAt,
                                  lpSS->Col.ssCurrAt );

               for ( ssColAt = ssColCnt; ssColAt > lpSS->Col.nHeaderCnt +
                     lpSS->Col.Frozen && x <= rect.right - rect.left; )
               {
                  if ( --ssColAt < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
                     break;

                  lWidth = SS_GetColWidth( lpSS, ssColAt );
                  if ( x + lWidth > rect.right - rect.left )
                  {
                     ssColAt++;
                     break;
                  }

                  x += lWidth;
               }

               ssColAt = mMin( ssColAt, lpSS->Col.Max );

               lpSS->Col.UL = mMax( lpSS->Col.nHeaderCnt + lpSS->Col.Frozen,
                                    ssColAt );

               if ( lpSS->ModeBlock )
                  BlockCellCurrentCol = ssColCnt - 1;
               else
               if ( fMoveActiveCell )
                  lpSS->Col.ssCurrAt = ssColCnt - 1;

               // determine new y coordinates
               y = SS_GetCellPosY( lpSS, lpSS->Row.ssCurrAt,
                                   lpSS->Row.ssCurrAt );

               for ( ssRowAt = ssRowCnt; ssRowAt > lpSS->Row.nHeaderCnt +
                     lpSS->Row.Frozen && y <= rect.bottom - rect.top; )
               {
                  if ( --ssRowAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
                     break;

                  lHeight = SS_GetRowHeight( lpSS, ssRowAt );

                  if ( y + lHeight > rect.bottom - rect.top )
                  {
                     ssRowAt++;
                     break;
                  }

                  y += lHeight;
               }

               ssRowAt = mMin( ssRowAt, lpSS->Row.Max );
               lpSS->Row.UL = mMax( lpSS->Row.nHeaderCnt + lpSS->Row.Frozen,
                                    ssRowAt );

               if ( lpSS->ModeBlock )
               {
                  BlockCellCurrentRow = ssRowCnt - 1;
                  if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                     lpSS->Row.ssCurrAt = ssRowCnt - 1;
               }
               else
               if ( fMoveActiveCell || fSingleSel )
                  lpSS->Row.ssCurrAt = ssRowCnt - 1;

               // repaint spreadsheet
               if ( !fMoveActiveCell || lpSS->ModeBlock ||
                    SS_LeaveCell( hWnd, lpSS, ssColAtOld, ssColLeftOld,
                                  ssRowAtOld, ssRowTopOld, 0 ) )
               {
                  lpSS->wScrollDirection = SS_VSCROLL_DOWN;
                  SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

                  SS_SetHScrollBar( hWnd, lpSS );
                  SS_SetVScrollBar( hWnd, lpSS );
                  SS_TopLeftChange( hWnd );

                  if ( lpSS->ModeBlock )
                     SSx_SelectBlock( hWnd, lpSS, BlockCellCurrentCol,
                                      BlockCellCurrentRow );
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( 0 );
            }
         }
         else
         {
            SS_ScrollEnd( hWnd, lpSS, SS_F_SCROLL_ROW |
                          (fSingleSel ? SS_F_MOVEACTIVECELL :
                          (fMoveActiveCell ? SS_F_MOVEACTIVECELL : 0)) );
         }

#ifndef SS_NOLISTBOX
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
            SS_SelectRow( hWnd, lpSS );
#endif

//       DrUnlockTaskMemory( lpSS );

         UpdateWindow( hWnd );
         lRC = 0;

         break;
      }

      case VK_DELETE:
         if ( HIBYTE( GetKeyState( VK_SHIFT) ) )
         {
            lpSS = SS_Lock( hWnd );
            if ( lpSS->fAutoClipboard )
               SSClipboardCut( hWnd );

//          DrUnlockTaskMemory( lpSS );
         }

         lpSS->wMsgLast = 0;
         return( 0 );

      case VK_INSERT:
         if ( HIBYTE( GetKeyState( VK_SHIFT) ) )
         {
            lpSS = SS_Lock( hWnd );
            if ( lpSS->fAutoClipboard )
               SSClipboardPaste( hWnd );

//          DrUnlockTaskMemory( lpSS );
         }
         else
         if ( HIBYTE( GetKeyState( VK_CONTROL) ) )
         {
            lpSS = SS_Lock( hWnd );
            if ( lpSS->fAutoClipboard )
               SSClipboardCopy( hWnd );

//          DrUnlockTaskMemory( lpSS );
         }

         lpSS->wMsgLast = 0;
         return( 0 );

      default:
         break;
   }

   if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && lpSS->ModeBlock &&
        (wParam == VK_UP || wParam == VK_DOWN || wParam == VK_PRIOR ||
         wParam == VK_NEXT || wParam == VK_HOME || wParam == VK_END) )
   {
      SS_HiliteCell( hWnd, TRUE );
   }

   if ( lRC != -1 )
   {
      lpSS = SS_Lock( hWnd );
      if ( lpSS->fInvertBlock && lpSS->ModeBlock == 0 )
      {
         lpSS->fSuspendInvert = FALSE;
         SS_HiliteCell( hWnd, TRUE );
         SS_InvertBlock( hWnd, 0, lpSS );
      }

//    DrUnlockTaskMemory( lpSS );
      return( lRC );
   }

   lpSS = SS_Lock( hWnd );
   lpSS->wMsgLast = 0;
// DrUnlockTaskMemory( lpSS );

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_alloc.cpp

LPSS_CELL
SS_AllocCell( LPSPREADSHEET lpSS,
              SS_COORD CellCol,
              SS_COORD CellRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocCell" );
   LPSS_ALLOCITEM  hCellItem = 0;
   LPSS_CELL       lpCell = 0;
   LPSS_ROW        lpRow;
   zBOOL           fIsNew;

   if ( (lpRow = SS_AllocLockRow( lpSS, CellRow )) != 0 )
   {
      hCellItem = SS_AllocItems( lpSS, &lpRow->Cells,
                                 CellCol, zsizeof( ZSS_Cell ), 0,
                                 SS_GetColsMax( lpSS ), &fIsNew );

//    lpCell = (LPSS_CELL) DrLockTaskMemory( hCellItem );
      lpCell = (LPSS_CELL) hCellItem;
      if ( fIsNew && hCellItem )
      {
         lpCell->m_FontId = SS_FONT_NONE;
//       lpCell = 0;    // This is to get around an internal compiler
//                      // error in MSVC 8.0
//       DrUnlockTaskMemory( lpCell );

         if ( CellCol >= lpSS->Col.AllocCnt )
            lpSS->Col.AllocCnt = CellCol + 1;
      }

      lpSS->LastCell.Coord.ssRow = CellRow;
      lpSS->LastCell.Coord.ssCol = CellCol;
      lpSS->LastCell.hItem = (zPCHAR) hCellItem;

      SS_UnlockRowItem( lpSS, CellRow );
   }

   return( lpCell );
}

LPSS_COL
SS_AllocCol( LPSPREADSHEET lpSS,
             SS_COORD CellCol )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocCol" );
   LPSS_ALLOCITEM  hColItem;
   LPSS_COL        lpColItem = 0;
   zBOOL           fIsNew;

   // Make room for column item.
   if ( (hColItem = SS_AllocItems( lpSS, &lpSS->Col.Items, CellCol,
                                   zsizeof( SS_COL ), 0,
                                   SS_GetColsMax( lpSS ), &fIsNew )) != 0 &&
        fIsNew )
   {
//    lpColItem = (LPSS_COL) DrLockTaskMemory( hColItem );
      lpColItem = (LPSS_COL) hColItem;
      lpColItem->FontId = SS_FONT_NONE;
      lpColItem->lColWidthX100 = SS_WIDTH_DEFAULT;
      lpColItem->nColWidthInPixels = SS_WIDTH_DEFAULT;

//    DrUnlockTaskMemory( lpColItem );
      if ( CellCol >= lpSS->Col.AllocCnt )
         lpSS->Col.AllocCnt = CellCol + 1;
   }
   else
   if ( hColItem == 0 )
      return( 0 );

   lpSS->Col.LastRefCoord.Coord.ssCol = CellCol;
   lpSS->Col.LastRefCoord.hItem = (zPCHAR) hColItem;

   return( (LPSS_COL) hColItem );
}

LPSS_ROW
SS_AllocLockRow( LPSPREADSHEET lpSS, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocLockRow" );
   LPSS_ROW  hRow;
   LPSS_ROW  lpRow = 0;

#ifdef  DKS_TEST
// TraceLineI( "SS_AllocLockRow ssRow ", ssRow );
   if ( fnListVBuffer( lpSS, TRUE ) )
      ::MessageBox( 0, "Before AllocLockRow", "VBuffer Error", MB_OK );
#endif

#if 0    // we always want to get into the virtual row buffer

   if ( lpSS->Row.LastRefCoord.Coord.ssRow == ssRow &&
        lpSS->Row.LastRefCoord.hItem )
   {
//    lpRow = (LPSS_ROW) DrLockTaskMemory( lpSS->Row.LastRefCoord.hItem );
      lpRow = (LPSS_ROW) lpSS->Row.LastRefCoord.hItem;
#ifdef  DKS_TEST
      if ( ssRow && lpSS->fVirtualMode && lpRow->fDataSet == FALSE )
      {
         TraceLineI( "Virtual Buffer with no data set ", ssRow );
         fnListVBuffer( lpSS, FALSE );
      }
#endif
   }
   else

#endif   // we always want to get into the virtual row buffer

   if ( (hRow = SS_AllocRow( lpSS, ssRow )) != 0 )
   {
//    lpRow = (LPSS_ROW) DrLockTaskMemory( hRow );
      lpRow = (LPSS_ROW) hRow;
   }

#ifdef  DKS_TEST
   if ( fnListVBuffer( lpSS, TRUE ) )
      ::MessageBox( 0, "After AllocLockRow", "VBuffer Error", MB_OK );
#endif

   return( lpRow );
}

#ifdef DKS_VIRT

LPSS_ROW
SS_VAllocInitRow( LPSPREADSHEET lpSS,
                  SS_COORD ssCellRow,
                  SS_COORD ssBufferIdx )
{
   zPCHAR    hRowItem;
   LPSS_ROW  lpRowItem;

   // Free up memory for rows to be discarded from row buffer pool.
   hRowItem = lpSS->Virtual.VRows[ ssBufferIdx ].hItems;
   if ( hRowItem )
   {
      // Remove all other data associated with this row???
//    lpRowItem = (LPSS_ROW) DrLockTaskMemory( hRowItem );
      lpRowItem = (LPSS_ROW) hRowItem;
      SSx_FreeRow( lpSS->hWnd, lpSS, lpRowItem );
      DrUnlockTaskMemory( lpRowItem );
      DrFreeTaskMemory( lpRowItem );
   }

   if ( DrAllocTaskMemory( (zCOREMEM) &hRowItem,
                           zsizeof( SS_ROW ), 1051 ) != 0 )
   {
      TraceLineI( "Memory allocation error for virtual row ",
                  ssCellRow );
      return( 0 );
   }

   lpRowItem = (LPSS_ROW) DrLockTaskMemory( hRowItem );
   lpSS->Virtual.VRows[ ssBufferIdx ].hItems = (zPCHAR) lpRowItem;
   lpSS->Virtual.VRows[ ssBufferIdx ].dwItemCnt = ssCellRow;

#ifdef  DKS_TEST
   TraceLineI( "Allocated Buffer for row ", ssCellRow );
   TraceLineI( "                  at idx ", ssBufferIdx );
   TraceLineI( "               start row ", lpSS->Virtual.VStartRow );
   TraceLineI( "               start idx ", lpSS->Virtual.VStartIdx );
   TraceLineI( "=========== hi water idx ", lpSS->Virtual.VHighWaterIdx );
#endif

   lpRowItem = (LPSS_ROW) DrLockTaskMemory( hRowItem );
   lpRowItem->FontId = SS_FONT_NONE;
   lpRowItem->RowMaxFontId = SS_FONT_NONE;
   lpRowItem->lRowHeightX100 = SS_HEIGHT_DEFAULT;
   lpRowItem->nRowHeightInPixels = SS_HEIGHT_DEFAULT;
   lpRowItem->fDataSet = FALSE;
// DrUnlockTaskMemory( lpRowItem );

   if ( ssCellRow >= lpSS->Row.AllocCnt )    // what is the value of this ...
      lpSS->Row.AllocCnt = ssCellRow + 1;    // what is its impact???

   return( lpRowItem );
}

#endif   // #ifdef DKS_VIRT

LPSS_ROW
SS_AllocRow( LPSPREADSHEET lpSS,
             SS_COORD ssCellRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocRow" );
   LPSS_ROW      lpRowItem;
   zBOOL         fIsNew;

   if ( ssCellRow == -1 )
      return( 0 );

#ifdef DKS_VIRT
   // Handle virtual mode row allocation differently
   if ( lpSS->fVirtualMode )
   {
      SS_COORD ssBufferIdx = -1;
      LPSS_ALLOCITEM lpRows;
      zBOOL bAllocated = FALSE;

//    lpRows = (LPSS_ALLOCITEM) DrLockTaskMemory( lpSS->Virtual.VRows );
      lpRows = (LPSS_ALLOCITEM) lpSS->Virtual.VRows;

#ifdef  DKS_TEST
      SS_VIRTUAL ssVirtStart = lpSS->Virtual;   // debug only???
      zPCHAR lpCase = "";
#endif
      // we ensure that the header row is always available
      if ( lpSS->Virtual.VStartRow == -1 )
      {
         lpSS->Virtual.VStartRow = 0;
         lpSS->Virtual.VStartIdx = 0;
         lpSS->Virtual.VHighWaterIdx = 0;

         lpRowItem = SS_VAllocInitRow( lpSS, 0, 0 );
//       lpRowItem = (LPSS_ROW) DrLockTaskMemory( lpRowItem );
         lpRowItem->fDataSet = FALSE;
//       DrUnlockTaskMemory( lpRowItem );
      }

      if ( ssCellRow == 0 )
      {
         ssBufferIdx = 0;     // looking for header row
#ifdef  DKS_TEST
         lpCase = "Header ssRow";
#endif
      }
      else
      {
         if ( ssCellRow >= lpSS->Virtual.VStartRow &&
              ssCellRow <
                      lpSS->Virtual.VStartRow + lpSS->Virtual.VHighWaterIdx )
         {
            // the row being requested is already in our buffer pool ...
            // just pick it out
#ifdef  DKS_TEST
            lpCase = "Pick ssRow";
            TraceLineI( "pick row ", ssCellRow );
#endif
            ssBufferIdx = lpSS->Virtual.VStartIdx +
                                       (ssCellRow - lpSS->Virtual.VStartRow);
            if ( ssBufferIdx > lpSS->Virtual.VHighWaterIdx )
               ssBufferIdx -= lpSS->Virtual.VHighWaterIdx;

            lpRowItem = (LPSS_ROW) lpSS->Virtual.VRows[ ssBufferIdx ].hItems;
            if ( lpRowItem == 0 )
            {
               lpRowItem = SS_VAllocInitRow( lpSS, ssCellRow, ssBufferIdx );
            }
         }
         else     // guaranteed good logic to here
         {
            // The row being requested is not in the current buffer pool.
            // Determine which rows can be kept and which (if any) must be
            // released from the buffer pool.  Free up memory for all rows
            // forced out of the buffer pool.
            zLONG lDiff;
            zSHORT nDirection;
            zBOOL  bClearAll = FALSE;

            if ( lpSS->Virtual.VStartIdx == 0 )
            {
               lDiff = 0;           // we have just begun
               bClearAll = TRUE;
            }
            else
            if ( ssCellRow > lpSS->Virtual.VStartRow )
            {
#ifdef  DKS_TEST
               lpCase = "Row After";
               TraceLineI( "Row After ", ssCellRow );

//             if ( ssCellRow > zVBUFFER_SIZE )
//                TraceLineI( "CellRow > zVBUFFER_SIZE ", ssCellRow );
//             if ( ssCellRow == 3037 ) // || ssCellRow == 3038 )
//             {
//                TraceLineI( "CellRow > zVBUFFER_SIZE ", ssCellRow );
//                fnListVBuffer( lpSS, FALSE );
//             }
#endif

               // The row being requested is after what we currently have.
               // Retain the current start index and start row (unless the
               // entire buffer pool is released).
               lDiff = ssCellRow -
                             (lpSS->Virtual.VStartRow + lpSS->Virtual.VSize);
#ifdef  DKS_TEST
                  TraceLineI( "Row After lDiff ", lDiff );
#endif

               // do we need to clean up all we have?
               if ( lDiff >= lpSS->Virtual.VHighWaterIdx - 1 )
               {
                  lDiff = lpSS->Virtual.VSize;  // yes ... clean it all up
               }
               else
               {
                  // ssBufferIdx needs to be set to 1 below the target index
                  if ( lDiff < 0 )
                  {
                     ssBufferIdx = lpSS->Virtual.VStartIdx - 1 +
                                       (ssCellRow - lpSS->Virtual.VStartRow);
                     lDiff = 1; // there's room to add another row to the pool
                     nDirection = 0;   // do not move VStartRow or VStartIdx
                  }
                  else
                  {
                     lDiff++;
                     ssBufferIdx = lpSS->Virtual.VStartIdx +
                                                     lpSS->Virtual.VSize - 1;
                     nDirection = 1;
                  }

                  if ( ssBufferIdx > lpSS->Virtual.VSize )
                  {
                     ssBufferIdx -= lpSS->Virtual.VSize;
                     lpSS->Virtual.VHighWaterIdx = lpSS->Virtual.VSize;
                  }
               }
            }
            else
            {
#ifdef  DKS_TEST
               lpCase = "Row Before";
               TraceLineI( "Row Before ", ssCellRow );
#endif
               // The row being requested is below what we currently have.
               nDirection = -1;
               lDiff = lpSS->Virtual.VStartRow - ssCellRow;

#ifdef  DKS_TEST
               TraceLineI( "Row Before lDiff ", lDiff );
#endif

               ssBufferIdx = lpSS->Virtual.VStartIdx;
            }

            if ( lDiff >= lpSS->Virtual.VSize )
            {
               // free up memory for ALL rows in the buffer pool
               bClearAll = TRUE;
               lDiff = lpSS->Virtual.VHighWaterIdx;
               ssBufferIdx = lpSS->Virtual.VHighWaterIdx + 1;
               nDirection = -1;
            }

            // At the end of this while loop, ssBufferIdx should be set
            // appropriately for the requested row.
            while ( lDiff-- )
            {
               if ( nDirection )
               {
                  lpSS->Virtual.VStartRow += nDirection;
                  lpSS->Virtual.VStartIdx += nDirection;
                  if ( lpSS->Virtual.VStartIdx == 0 )
                     lpSS->Virtual.VStartIdx = lpSS->Virtual.VSize;
                  else
                  if ( lpSS->Virtual.VStartIdx > lpSS->Virtual.VSize )
                     lpSS->Virtual.VStartIdx = 1;
               }

               if ( nDirection < 0 )   // we need this test
               {
                  ssBufferIdx--;
                  if ( ssBufferIdx == 0 )
                     ssBufferIdx = lpSS->Virtual.VSize;
               }
               else
               {
                  ssBufferIdx++;
                  if ( ssBufferIdx > lpSS->Virtual.VSize )
                     ssBufferIdx = 1;
               }

               if ( ssBufferIdx > lpSS->Virtual.VHighWaterIdx )
                  lpSS->Virtual.VHighWaterIdx = ssBufferIdx;

               if ( ssBufferIdx == 0 )
                  ::MessageBox( 0, "Deleting row 0!!!", "SSW", MB_OK );

               lpRowItem = (LPSS_ROW) lpSS->Virtual.VRows[ ssBufferIdx ].hItems;
               if ( lpRowItem )
               {
                  // remove all other data associated with this row???
//                lpRowItem = (LPSS_ROW) DrLockTaskMemory( lpRowItem );
                  SSx_FreeRow( lpSS->hWnd, lpSS, lpRowItem );
                  lpRowItem = (LPSS_ROW) DrUnlockTaskMemory( lpRowItem );

                  // now free the row
                  DrFreeTaskMemory( lpRowItem );
                  lpSS->Virtual.VRows[ ssBufferIdx ].hItems = 0;
                  lpSS->Virtual.VRows[ ssBufferIdx ].dwItemCnt = 0;
               }
            }

            if ( bClearAll )
            {
               ssBufferIdx = 1;
               lpSS->Virtual.VStartRow = ssCellRow;
               lpSS->Virtual.VStartIdx = 1;
               lpSS->Virtual.VHighWaterIdx = 1;
            }
            else
            if ( lpSS->Virtual.VStartRow == 0 )
               lpSS->Virtual.VStartRow = ssCellRow;

            lpRowItem = SS_VAllocInitRow( lpSS, ssCellRow, ssBufferIdx );
         }
      }

      lpRowItem = (LPSS_ROW) lpSS->Virtual.VRows[ ssBufferIdx ].hItems;

#ifdef  DKS_TEST
      if ( lpSS->Virtual.VRows[ ssBufferIdx ].dwItemCnt != ssCellRow ||
           fnListVBuffer( lpSS, TRUE ) )
      {

         TraceLineI( "Virtual VRows out of sync for row = ", ssCellRow );
         TraceLineI( "          (dwItemCnt) row located = ",
                     lpSS->Virtual.VRows[ ssBufferIdx ].dwItemCnt );
         TraceLineI( "                      ssBufferIdx = ", ssBufferIdx );

         TraceLineI( "                ssVirtStart.lVStyle ", ssVirtStart.lVStyle );
         TraceLineI( "                   ssVirtStart.VMax ", ssVirtStart.VMax );
         TraceLineI( "                   ssVirtStart.VTop ", ssVirtStart.VTop );
         TraceLineI( "                  ssVirtStart.VSize ", ssVirtStart.VSize );
         TraceLineI( "               ssVirtStart.VOverlap ", ssVirtStart.VOverlap );
         TraceLineI( "              ssVirtStart.VPhysSize ", ssVirtStart.VPhysSize );
         TraceLineI( "              ssVirtStart.VStartIdx ", ssVirtStart.VStartIdx );
         TraceLineI( "              ssVirtStart.VStartRow ", ssVirtStart.VStartRow );
         TraceLineI( "          ssVirtStart.VHighWaterIdx ", ssVirtStart.VHighWaterIdx );
         TraceLineI( "lpSS->Virtual.VRows[ VStartIdx ].dwItemCnt ",
                      lpSS->Virtual.VRows[ ssVirtStart.VStartIdx ].dwItemCnt );
         TraceLineI( "lpSS->Virtual.VRows[ VHighWaterIdx ].dwItemCnt ",
                      lpSS->Virtual.VRows[ ssVirtStart.VHighWaterIdx ].dwItemCnt );
         fnListVBuffer( lpSS, FALSE );
         TraceLineS( " ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Case = ", lpCase );

//       ::MessageBox( 0, "Virtual VRows out of sync", "SSW", MB_OK );
      }
#endif

//    DrUnlockTaskMemory( lpRows );

//    TraceLineI( "SS_AllocRow ", ssCellRow );
//    TraceLineI( "   hRowItem ", (zLONG) hRowItem );

      if ( lpRowItem )
      {
         lpSS->Row.LastRefCoord.Coord.ssRow = ssCellRow;
         lpSS->Row.LastRefCoord.hItem = (zPCHAR) lpRowItem;
      }

      return( lpRowItem );
   }
#endif   // #ifdef DKS_VIRT

   // Make room for ssRow item.
   if ( (lpRowItem = (LPSS_ROW) SS_AllocItems( lpSS, &lpSS->Row.Items,
                                              ssCellRow, zsizeof( SS_ROW ), 0,
                                   SS_GetRowsMax( lpSS ), &fIsNew )) != 0 &&
        fIsNew )
   {
      lpRowItem = (LPSS_ROW) DrLockTaskMemory( lpRowItem );

      lpRowItem->FontId = SS_FONT_NONE;
      lpRowItem->RowMaxFontId = SS_FONT_NONE;
      lpRowItem->lRowHeightX100 = SS_HEIGHT_DEFAULT;
      lpRowItem->nRowHeightInPixels = SS_HEIGHT_DEFAULT;

//    DrUnlockTaskMemory( lpRowItem );

      if ( ssCellRow >= lpSS->Row.AllocCnt )
         lpSS->Row.AllocCnt = ssCellRow + 1;
   }

   if ( lpRowItem == 0 )
      return( 0 );

   lpSS->Row.LastRefCoord.Coord.ssRow = ssCellRow;
   lpSS->Row.LastRefCoord.hItem = (zPCHAR) lpRowItem;

   return( lpRowItem );
}

LPSS_ALLOCITEM
SS_AllocItems( LPSPREADSHEET lpSS,           // pointer to spreadsheet struct
               LPSS_ALLOCITEM lpAllocItem,   // pointer to the alloc item
               SS_COORD ssItemNbr,           // number of item to allocate
               WORD     wItemSize,           // size of item to allocate
               zPCHAR   hItem,
               SS_COORD ssMaxItems,
               zBOOL    *lpfIsNew )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocItems" );
   LPSS_ALLOCITEM   lpItems;
   zPCHAR           *lpItems2;
   DWORD            dwItemAt;
   DWORD            dwItemCnt;
   WORD             wItem2At;
   WORD             wItem2Cnt;

   if ( lpfIsNew )
      *lpfIsNew = FALSE;

   // if we are requesting more items than the maximum permitted, get out!
   if ( ssItemNbr + 1 > ssMaxItems )
      return( 0 );

   // note that since SS_ALLOCITEMCNT is defined to be 32 ...
   // CALCALLOCITEM essentially performs a "divide by 32" on ssItemNbr, and
   // CALCALLOCITEMREM essentially performs a "modulus 32" on ssItemNbr ...
   // so that for ssItemNbr == 65
   //       CALCALLOCITEM( 65 ) == 2 and
   //       CALCALLOCITEMREM( 65 ) == 1
   //
   // I am not yet sure of the rationale for doing this.  It appears that
   // it is an attempt to break up memory requirements into manageable
   // chunks ... for us, we are going to circumvent the problem as much
   // as possible in the virtual mode by using a "rolling" row buffer in
   // which the number of rows managed at any one time remains constant ...
   // any particular row may be "in" or "out" of scope at a given time.  If
   // a row needs to be brought "in scope", another row may be forced "out
   // of scope" - which means the memory for the "out of scope" row is
   // freed and memory required for the "in scope" row is allocated.
   //
   dwItemAt = CALCALLOCITEM( ssItemNbr );

   // this was in the original, but it does not seem to have any effect
   // since the next executable line overlays dwItemCnt
// dwItemCnt = mMax( dwItemAt + 1,
//                  lpAllocItem->dwItemCnt + ARRAY1_ALLOC_INC );

   // The first allocation is for the list (array) of alloc item pointers.
   // There must be enough entries in the list to hold the current alloc
   // item pointer.
   dwItemCnt = dwItemAt + 1;
   if ( dwItemCnt > lpAllocItem->dwItemCnt )
      dwItemCnt += ARRAY1_ALLOC_INC;

   if ( dwItemCnt > lpAllocItem->dwItemCnt )
   {
      if ( lpAllocItem->dwItemCnt == 0 )
      {
         if ( DrAllocTaskMemory( (zCOREMEM) &lpAllocItem->hItems,
                                 (zLONG) (dwItemCnt * zsizeof( SS_ALLOCITEM )),
                                 1052 ) != 0 )
         {
            return( 0 );
         }
      }
      else
      {
         lpAllocItem->hItems =
                          (zPCHAR) DrUnlockTaskMemory( lpAllocItem->hItems );
         if ( DrReAllocTaskMemory( (zCOREMEM) &lpAllocItem->hItems, (zLONG)
                                (dwItemCnt * zsizeof( SS_ALLOCITEM )) ) != 0 )
         {
            return( 0 );
         }
      }

      lpAllocItem->dwItemCnt = dwItemCnt;

      // Get addressability to the list of alloc item pointers.
      lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpAllocItem->hItems );
      lpAllocItem->hItems = (zPCHAR) lpItems;
   }
   else
      lpItems = (LPSS_ALLOCITEM) lpAllocItem->hItems;

   // Now we know there is room in the list (array) of alloc item pointers
   // to hold the pointer to the alloc item for the current item.
   //

   // CALCALLOCITEMREM essentially performs a "modulus 32" on ssItemNbr
   wItem2At = CALCALLOCITEMREM( ssItemNbr );

   wItem2Cnt = wItem2At + 1;
   if ( wItem2Cnt > lpItems[ dwItemAt ].dwItemCnt )
      wItem2Cnt = mMin( (zLONG) wItem2Cnt + ARRAY2_ALLOC_INC, SS_ALLOCITEMCNT );

   if ( wItem2Cnt > lpItems[ dwItemAt ].dwItemCnt )
   {
      if ( lpItems[ dwItemAt ].dwItemCnt == 0 )
      {
         if ( DrAllocTaskMemory( (zCOREMEM) &lpItems[ dwItemAt ].hItems,
                                 (zLONG) (wItem2Cnt *
                                          sizeof( zPCHAR )), 1053 ) != 0 )
         {
//          DrUnlockTaskMemory( lpItems );
            return( 0 );
         }
      }
      else
      {
         lpItems[ dwItemAt ].hItems = (zPCHAR)
                           DrUnlockTaskMemory( lpItems[ dwItemAt ].hItems );
         if ( DrReAllocTaskMemory( (zCOREMEM) &lpItems[ dwItemAt ].hItems,
                                   (zLONG) (wItem2Cnt *
                                            sizeof( zPCHAR )) ) != 0 )
         {
//          DrUnlockTaskMemory( lpItems );
            return( 0 );
         }

      }

      lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ dwItemAt ].hItems );
      lpItems[ dwItemAt ].hItems = (zPCHAR) lpItems2;
      zmemset( &lpItems2[ lpItems[ dwItemAt ].dwItemCnt ], 0,
               sizeof( zPCHAR ) * (wItem2Cnt -
                                      (WORD) lpItems[ dwItemAt ].dwItemCnt) );
//    DrUnlockTaskMemory( lpItems2 );

      lpItems[ dwItemAt ].dwItemCnt = wItem2Cnt;
   }

// lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ dwItemAt ].hItems );
   lpItems2 = (zPCHAR *) lpItems[ dwItemAt ].hItems;
   if ( !lpItems2[ wItem2At ] )
   {
      if ( hItem )
         lpItems2[ wItem2At ] = hItem;
      else
      // if ( (hItem = (zPCHAR) MemAllocFS( need pool size here )) != 0 )
      // we need to have an allocation scheme in which we don't need to lock
      // the returned memory ... in Windows, it looks like we are OK, but in
      // other systems???
      if ( DrAllocTaskMemory( (zCOREMEM) &hItem, wItemSize, 1054 ) == 0 )
      {
         hItem = (zPCHAR) DrLockTaskMemory( hItem );
         zmemset( hItem, 0, wItemSize );
         lpItems2[ wItem2At ] = hItem;
      }
      else
         hItem = 0;

      if ( lpfIsNew )
         *lpfIsNew = TRUE;
   }
   else
      hItem = lpItems2[ wItem2At ];

// DrUnlockTaskMemory( lpItems2 );
// DrUnlockTaskMemory( lpItems );

   return( (LPSS_ALLOCITEM) hItem );
}

LPSS_CELL
SS_AllocLockCell( LPSPREADSHEET lpSS, LPSS_ROW lpRow,
                  SS_COORD dCol, SS_COORD dRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocLockCell" );
   LPSS_CELL  hCell;
   LPSS_CELL  lpCell = 0;

   if ( lpSS->LastCell.Coord.ssRow == dRow &&
        lpSS->LastCell.Coord.ssCol == dCol &&
        lpSS->LastCell.hItem )
   {
//    lpCell = (LPSS_CELL) DrLockTaskMemory( lpSS->LastCell.hItem );
      lpCell = (LPSS_CELL) lpSS->LastCell.hItem;
   }
   else
   if ( (hCell = SS_AllocCell( lpSS, dCol, dRow )) != 0 )
   {
//    lpCell = (LPSS_CELL) DrLockTaskMemory( hCell );
      lpCell = (LPSS_CELL) hCell;
   }

   return( lpCell );
}

LPSS_COL
SS_AllocLockCol( LPSPREADSHEET lpSS, SS_COORD dCol )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_AllocLockCol" );
   LPSS_COL  hCol;
   LPSS_COL  lpCol = 0;

   if ( lpSS->Col.LastRefCoord.Coord.ssCol == dCol &&
        lpSS->Col.LastRefCoord.hItem )
   {
//    lpCol = (LPSS_COL) DrLockTaskMemory( lpSS->Col.LastRefCoord.hItem );
      lpCol = (LPSS_COL) lpSS->Col.LastRefCoord.hItem;
   }
   else
   if ( (hCol = SS_AllocCol( lpSS, dCol )) != 0 )
   {
//    lpCol = (LPSS_COL) DrLockTaskMemory( hCol );
      lpCol = (LPSS_COL) hCol;
   }

   return( lpCol );
}

void
SS_ClearAll( HWND hWnd, LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_ClearAll" );
   LPSS_CONTROL  Controls;
   SS_COORD      ss;

   SS_ClearUndoBuffer( hWnd, lpSS );

   SSx_FreeCells( hWnd, lpSS );

   if ( lpSS->hControls )
   {
//    Controls = (LPSS_CONTROL) DrLockTaskMemory( lpSS->hControls );
      Controls = (LPSS_CONTROL) lpSS->hControls;
      for ( ss = 0; ss < lpSS->lControlsCnt; ss++ )
      {
         if ( Controls[ ss ].nCtrlID )
            DestroyWindow( Controls[ ss ].hWnd );
      }

      lpSS->hControls = (zPCHAR) DrUnlockTaskMemory( Controls );
      DrFreeTaskMemory( lpSS->hControls );
      lpSS->hControls = 0;
      lpSS->lControlsCnt = 0;
      lpSS->lControlsAllocCnt = 0;
   }

#ifndef SS_NOCALC
   SS_FreeCalcList( &lpSS->CalcAllDependents );

   if ( lpSS->hCalcTable )
   {
      lpSS->hCalcTable = DrUnlockTaskMemory( lpSS->hCalcTable );
      DrFreeTaskMemory( lpSS->hCalcTable );
      lpSS->hCalcTable = 0;
   }
#endif

   SSx_FreeData( &lpSS->DefaultData );

   if ( lpSS->hBuffer )
   {
      lpSS->hBuffer = (zPCHAR) DrUnlockTaskMemory( lpSS->hBuffer );
      DrFreeTaskMemory( lpSS->hBuffer );
      lpSS->hBuffer = 0;
   }

   if ( lpSS->hPicList )
   {
      lpSS->hPicList = (zPCHAR) DrUnlockTaskMemory( lpSS->hPicList );
      DrFreeTaskMemory( lpSS->hPicList );
      lpSS->hPicList = 0;
      lpSS->nPicListCnt = 0;
   }

   if ( lpSS->hXtra )
   {
      lpSS->hXtra = (zPCHAR) DrUnlockTaskMemory( lpSS->hXtra );
      DrFreeTaskMemory( lpSS->hXtra );
      lpSS->hXtra = 0;
      lpSS->lXtraLth = 0;
   }

   lpSS->EditModeOn = FALSE;
   lpSS->EditModeTurningOff = FALSE;
   lpSS->HiliteOn = FALSE;
   lpSS->FreezeHilite = FALSE;
   lpSS->NoEraseBkgnd = FALSE;
   lpSS->Row.ssCurrAt = 0;
   lpSS->Col.ssCurrAt = 0;
   lpSS->Row.UL = 0;
   lpSS->Col.UL = 0;
   lpSS->Row.LR = 0;
   lpSS->Col.LR = 0;
   lpSS->Row.AllocCnt = 0;
   lpSS->Col.AllocCnt = 0;
   lpSS->Row.DataCnt = 0;
   lpSS->Col.DataCnt = 0;
   lpSS->nCalcTableCnt = 0;
   lpSS->nCalcTableAllocCnt = 0;
   lpSS->hCalcTable = 0;
   lpSS->DefaultData.m_DataType = 0;
   zmemset( &lpSS->CurVisCell, 0, zsizeof( SS_CELLCOORD ) );
   zmemset( &lpSS->CurCalcCell, 0, zsizeof( SS_CELLCOORD ) );
//? zmemset( &lpSS->CalcAllDependents, 0, zsizeof( SS_CALCLIST ) );
   lpSS->ModeResize = 0;
   lpSS->lResizeOffset = 0;
   lpSS->lResizeCurrentPos = 0;
   lpSS->lResizeStartPos = 0;
   lpSS->lResizeMinPos = 0;
   lpSS->ssResizeCoord = 0;
   lpSS->ModeBlock = 0;
   zmemset( &lpSS->BlockCellUL, 0, zsizeof( SS_CELLCOORD ) );
   zmemset( &lpSS->BlockCellLR, 0, zsizeof( SS_CELLCOORD ) );
   lpSS->Col.BlockCellCurrentPos = 0;
   lpSS->Row.BlockCellCurrentPos = 0;

   lpSS->Col.LastRefCoord.Coord.ssCol = -1;
   lpSS->Row.LastRefCoord.Coord.ssRow = -1;
   lpSS->LastCell.Coord.ssCol = -1;
   lpSS->LastCell.Coord.ssRow = -1;

   //lpSS->DefaultFontId = lpSS->DefaultFontIdOrig;
}

zBOOL
SS_DeAllocCell( HWND hWnd, SS_COORD CellCol, SS_COORD CellRow,
                zBOOL fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_DeAllocCell" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;

   lpSS = SS_Lock( hWnd );

   if ( CellRow >= lpSS->Row.AllocCnt || CellCol >= lpSS->Col.AllocCnt )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   lpRow = SS_LockRowItem( lpSS, CellRow );
   SSx_DeAllocCell( hWnd, lpRow, CellCol, CellRow, fSendChangeMsg );

   if ( lpRow != 0 )
      SS_UnlockRowItem( lpSS, CellRow );

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL
SSx_DeAllocCell( HWND hWnd, LPSS_ROW lpRow, SS_COORD CellCol,
                 SS_COORD CellRow, zBOOL fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_DeAllocCell" );
   LPSPREADSHEET lpSS;
   LPSS_CELL     lpCell;
   zBOOL         fDataChange = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( CellRow >= lpSS->Row.AllocCnt || CellCol >= lpSS->Col.AllocCnt )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, CellCol, CellRow )) != 0 )
   {
      if ( fSendChangeMsg && lpCell->m_CellData.m_DataType )
         fDataChange = TRUE;

      SS_ResetCellOverflow( hWnd, lpSS, CellCol, CellRow );
      SSx_FreeCell( hWnd, lpSS, lpCell );
      SS_UnlockCellItem( lpSS, CellCol, CellRow );

      if ( lpRow )
         SS_DeleteItem( lpSS, &lpRow->Cells, CellCol, SS_GetColsMax( lpSS ) );

      lpSS->LastCell.Coord.ssCol = -1;
      lpSS->LastCell.Coord.ssRow = -1;

      if ( fDataChange )
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ),
                           CellCol, CellRow );
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void
SS_DeAllocCellType( HWND          hWnd,
                    LPSPREADSHEET lpSS,
                    zPCHAR        hCellType )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_DeAllocCellType" );
   LPSS_CELLTYPE CellType;
   LPSS_CONTROL  Controls;

   if ( hCellType )
   {
//    CellType = (LPSS_CELLTYPE) DrLockTaskMemory( hCellType );
      CellType = (LPSS_CELLTYPE) hCellType;

      switch ( CellType->chType )
      {
         case SS_TYPE_COMBOBOX:
            if ( CellType->Spec.ComboBox.hItems )
            {
               CellType->Spec.ComboBox.hItems = (zPCHAR)
                        DrUnlockTaskMemory( CellType->Spec.ComboBox.hItems );
               DrFreeTaskMemory( CellType->Spec.ComboBox.hItems );
               CellType->Spec.ComboBox.hItems = 0;
            }

            break;

         case SS_TYPE_PIC:
            if ( CellType->Spec.Pic.hMask )
            {
               CellType->Spec.Pic.hMask = (zPCHAR)
                              DrUnlockTaskMemory( CellType->Spec.Pic.hMask );
               DrFreeTaskMemory( CellType->Spec.Pic.hMask );
               CellType->Spec.Pic.hMask = 0;
            }

            break;

         case SS_TYPE_PICTURE:
            if ( CellType->Spec.ViewPict.hPictName &&
                 !(CellType->lStyle & VPS_HANDLE) )
            {
               CellType->Spec.ViewPict.hPictName = (zPCHAR)
                     DrUnlockTaskMemory( CellType->Spec.ViewPict.hPictName );
               DrFreeTaskMemory( CellType->Spec.ViewPict.hPictName );
               CellType->Spec.ViewPict.hPictName = 0;
            }

            if ( CellType->Spec.ViewPict.hPictName &&
                 (CellType->lStyle & VPS_HANDLE) &&
                 CellType->Spec.ViewPict.fDeleteHandle )
            {
               DeleteObject( (HANDLE) CellType->Spec.ViewPict.hPictName );
            }

            if ( CellType->nControlID )
            {
//             Controls = (LPSS_CONTROL) DrLockTaskMemory( lpSS->hControls );
               Controls = (LPSS_CONTROL) lpSS->hControls;

               DestroyWindow( SS_GetControlhWnd( lpSS, CellType->nControlID ) );
               Controls[ CellType->nControlID - SS_CONTROL_BASEID ].nCtrlID = 0;
               Controls[ CellType->nControlID - SS_CONTROL_BASEID ].hWnd = 0;

//             DrUnlockTaskMemory( Controls );
            }

            break;

         case SS_TYPE_BUTTON:
            if ( CellType->Spec.Button.hText )
            {
               CellType->Spec.Button.hText = (zPCHAR)
                           DrUnlockTaskMemory( CellType->Spec.Button.hText );
               DrFreeTaskMemory( CellType->Spec.Button.hText );
               CellType->Spec.Button.hText = 0;
            }

            if ( CellType->Spec.Button.hPictName &&
                 !(CellType->Spec.Button.nPictureType & SUPERBTN_PICT_HANDLE) )
            {
               CellType->Spec.Button.hPictName = (zPCHAR)
                       DrUnlockTaskMemory( CellType->Spec.Button.hPictName );
               DrFreeTaskMemory( CellType->Spec.Button.hPictName );
               CellType->Spec.Button.hPictName = 0;
            }

            if ( CellType->Spec.Button.hPictDownName &&
                 !(CellType->Spec.Button.nPictureDownType & SUPERBTN_PICT_HANDLE) )
            {
               CellType->Spec.Button.hPictDownName = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.Button.hPictDownName );
               DrFreeTaskMemory( CellType->Spec.Button.hPictDownName );
               CellType->Spec.Button.hPictDownName = 0;
            }

            if ( CellType->Spec.Button.hPictName &&
                 (CellType->Spec.Button.nPictureType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.Button.fDelHandle )
            {
               SS_PicDeref( hWnd, (HANDLE) CellType->Spec.Button.hPictName );
            }

            if ( CellType->Spec.Button.hPictDownName &&
                 (CellType->Spec.Button.nPictureDownType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.Button.fDelDownHandle )
            {
               SS_PicDeref( hWnd, (HANDLE) CellType->Spec.Button.hPictDownName );
            }

            break;

         case SS_TYPE_CHECKBOX:
            if ( CellType->Spec.CheckBox.hText )
            {
               CellType->Spec.CheckBox.hText = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.CheckBox.hText );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hText );
               CellType->Spec.CheckBox.hText = 0;
            }

            if ( CellType->Spec.CheckBox.hPictUpName &&
                 !(CellType->Spec.CheckBox.bPictUpType & BT_HANDLE) )
            {
               CellType->Spec.CheckBox.hPictUpName = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.CheckBox.hPictUpName );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hPictUpName );
               CellType->Spec.CheckBox.hPictUpName = 0;
            }

            if ( CellType->Spec.CheckBox.hPictDownName &&
                 !(CellType->Spec.CheckBox.bPictDownType & BT_HANDLE) )
            {
               CellType->Spec.CheckBox.hPictDownName = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.CheckBox.hPictDownName );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hPictDownName );
               CellType->Spec.CheckBox.hPictDownName = 0;
            }

            if ( CellType->Spec.CheckBox.hPictFocusUpName &&
                 !(CellType->Spec.CheckBox.bPictFocusUpType & BT_HANDLE) )
            {
               CellType->Spec.CheckBox.hPictFocusUpName = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.CheckBox.hPictFocusUpName );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hPictFocusUpName );
               CellType->Spec.CheckBox.hPictFocusUpName = 0;
            }

            if ( CellType->Spec.CheckBox.hPictFocusDownName &&
                 !(CellType->Spec.CheckBox.bPictFocusDownType & BT_HANDLE) )
            {
               CellType->Spec.CheckBox.hPictFocusDownName = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.CheckBox.hPictFocusDownName );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hPictFocusDownName );
               CellType->Spec.CheckBox.hPictFocusDownName = 0;
            }

            if ( CellType->Spec.CheckBox.hPictDisabledUpName &&
                 !(CellType->Spec.CheckBox.bPictDisabledUpType & BT_HANDLE) )
            {
               CellType->Spec.CheckBox.hPictDisabledUpName = (zPCHAR)
                  DrUnlockTaskMemory( CellType->Spec.CheckBox.hPictDisabledUpName );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hPictDisabledUpName );
               CellType->Spec.CheckBox.hPictDisabledUpName = 0;
            }

            if ( CellType->Spec.CheckBox.hPictDisabledDownName &&
                 !(CellType->Spec.CheckBox.bPictDisabledDownType & BT_HANDLE) )
            {
               CellType->Spec.CheckBox.hPictDisabledDownName = (zPCHAR)
                  DrFreeTaskMemory( CellType->Spec.CheckBox.hPictDisabledDownName );
               DrFreeTaskMemory( CellType->Spec.CheckBox.hPictDisabledDownName );
               CellType->Spec.CheckBox.hPictDisabledDownName = 0;
            }

            if ( CellType->Spec.CheckBox.hPictUpName &&
                 (CellType->Spec.CheckBox.bPictUpType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.CheckBox.fDelUpHandle )
            {
               SS_PicDeref( hWnd,
                            (HANDLE) CellType->Spec.CheckBox.hPictUpName );
            }

            if ( CellType->Spec.CheckBox.hPictDownName &&
                 (CellType->Spec.CheckBox.bPictDownType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.CheckBox.fDelDownHandle )
            {
               SS_PicDeref( hWnd,
                            (HANDLE) CellType->Spec.CheckBox.hPictDownName );
            }

            if ( CellType->Spec.CheckBox.hPictFocusUpName &&
                 (CellType->Spec.CheckBox.bPictFocusUpType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.CheckBox.fDelFocusUpHandle )
            {
               SS_PicDeref( hWnd,
                            (HANDLE) CellType->Spec.CheckBox.hPictFocusUpName );
            }

            if ( CellType->Spec.CheckBox.hPictFocusDownName &&
                 (CellType->Spec.CheckBox.bPictFocusDownType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.CheckBox.fDelFocusDownHandle )
            {
               SS_PicDeref( hWnd,
                            (HANDLE) CellType->Spec.CheckBox.hPictFocusDownName );
            }

            if ( CellType->Spec.CheckBox.hPictDisabledUpName &&
                 (CellType->Spec.CheckBox.bPictDisabledUpType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.CheckBox.fDelDisabledUpHandle )
            {
               SS_PicDeref( hWnd,
                            (HANDLE) CellType->Spec.CheckBox.hPictDisabledUpName );
            }

            if ( CellType->Spec.CheckBox.hPictDisabledDownName &&
                 (CellType->Spec.CheckBox.bPictDisabledDownType & SUPERBTN_PICT_HANDLE) &&
                 CellType->Spec.CheckBox.fDelDisabledDownHandle )
            {
               SS_PicDeref( hWnd,
                            (HANDLE) CellType->Spec.CheckBox.hPictDisabledDownName );
            }

            break;
      }

      hCellType = (zPCHAR) DrUnlockTaskMemory( CellType );
      DrFreeTaskMemory( hCellType );
   }
}

zBOOL
SS_DeAllocCol( HWND hWnd, SS_COORD ssCol, zBOOL fSendChangeMsg, zBOOL bSkipHeader )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_DeAllocCol" );
   LPSPREADSHEET  lpSS;
   LPSS_ALLOCITEM lpAllocItem;
   LPSS_COL       lpCol;
   SS_COORD       lMaxCols;
   SS_COORD       ss;

   lpSS = SS_Lock( hWnd );

   if ( ssCol >= lpSS->Col.AllocCnt )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   for ( ss = bSkipHeader ? 1 : 0; ss < lpSS->Row.AllocCnt; ss++ )
      SS_DeAllocCell( hWnd, ssCol, ss, fSendChangeMsg );

   if ( (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
   {
      SSx_FreeCol( hWnd, lpSS, lpCol );

      SS_UnlockColItem( lpSS, ssCol );
      lMaxCols = SS_GetColsMax( lpSS );
      lpAllocItem = &lpSS->Col.Items;
      SS_DeleteItem( lpSS, lpAllocItem, ssCol, lMaxCols );
   }

   lpSS->Col.LastRefCoord.Coord.ssCol = -1;

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL
SS_DeAllocRow( HWND hWnd, SS_COORD ssRow, zBOOL fSendChangeMsg, zBOOL bSkipHeader )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_DeAllocRow" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   SS_COORD      ss;

   lpSS = SS_Lock( hWnd );

   if ( ssRow >= lpSS->Row.AllocCnt )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
   {
      for ( ss = bSkipHeader ? 1 : 0; ss < lpSS->Col.AllocCnt; ss++ )
         SSx_DeAllocCell( hWnd, lpRow, ss, ssRow, fSendChangeMsg );

      if ( lpRow->fRowSelected )
         lpSS->MultiSelCnt--;

      SSx_FreeRow( hWnd, lpSS, lpRow );

      SS_UnlockRowItem( lpSS, ssRow );
      SS_DeleteItem( lpSS, &lpSS->Row.Items, ssRow, SS_GetRowsMax( lpSS ) );

      lpSS->Row.LastRefCoord.Coord.ssRow = -1;
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void
SS_FreeAllocItem( LPSS_ALLOCITEM lpAllocItem )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_FreeAllocItem" );
   LPSS_ALLOCITEM  lpItems;
   zPCHAR          *lpItems2;
   DWORD           dw;
   WORD            k;

   if ( lpAllocItem->hItems )
   {
//    lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpAllocItem->hItems );
      lpItems = (LPSS_ALLOCITEM) lpAllocItem->hItems;
      for ( dw = 0; dw < lpAllocItem->dwItemCnt; dw++ )
      {
         if ( lpItems[ dw ].hItems )
         {
//          lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ dw ].hItems );
            lpItems2 = (zPCHAR *) lpItems[ dw ].hItems;

            for ( k = 0; k < lpItems[ dw ].dwItemCnt; k++ )
            {
               if ( lpItems2[ k ] )
               {
                  lpItems2[ k ] = (zPCHAR) DrUnlockTaskMemory( lpItems2[ k ] );
                  DrFreeTaskMemory( lpItems2[ k ] );
               }
            }

            lpItems[ dw ].hItems = (zPCHAR) DrUnlockTaskMemory( lpItems2 );
            DrFreeTaskMemory( lpItems[ dw ].hItems );
         }
      }

      lpAllocItem->hItems = (zPCHAR) DrUnlockTaskMemory( lpItems );
      DrFreeTaskMemory( lpAllocItem->hItems );

      lpAllocItem->hItems = 0;
      lpAllocItem->dwItemCnt = 0;
   }
}

void
SSx_FreeCell( HWND hWnd, LPSPREADSHEET lpSS, LPSS_CELL lpCell )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_FreeCell" );
   SSx_FreeData( &lpCell->m_CellData );

#ifndef SS_NOCALC
   if ( lpCell->m_hCalc )
   {
      SS_FreeCalc( lpSS, lpCell->m_hCalc );
      lpCell->m_hCalc = 0;
   }

   SS_FreeCalcList( &lpCell->m_Dependents );
#endif

   SS_DeAllocCellType( hWnd, lpSS, lpCell->m_hCellType );
}

void
SSx_FreeCells( HWND hWnd, LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_FreeCells" );
   LPSS_ALLOCITEM  lpItems;
   zPCHAR          *lpItems2;
   LPSS_ROW        lpRow;
   LPSS_COL        lpCol;
   SS_COORD        ss;
   zUSHORT         k;

   if ( lpSS->Row.Items.hItems )
   {
//    lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpSS->Row.Items.hItems );
      lpItems = (LPSS_ALLOCITEM) lpSS->Row.Items.hItems;
      for ( ss = 0; ss < (zSHORT) lpSS->Row.Items.dwItemCnt; ss++ )
      {
         if ( lpItems[ ss ].dwItemCnt )
         {
//          lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ ss ].hItems );
            lpItems2 = (zPCHAR *) lpItems[ ss ].hItems;

            for ( k = 0; k < lpItems[ ss ].dwItemCnt; k++ )
            {
               if ( lpItems2[ k ] )
               {
//                lpRow = (LPSS_ROW) DrLockTaskMemory( lpItems2[ k ] );
                  lpRow = (LPSS_ROW) lpItems2[ k ];

#ifndef SS_NOCALC
                  SS_FreeCalcList( &lpRow->Dependents );

                  if ( lpRow->hCalc )
                     SSx_FreeCalc( lpSS, lpRow->hCalc );
#endif

                  SS_DeAllocCellType( hWnd, lpSS, lpRow->hCellType );
                  SSx_FreeData( &lpRow->Data );

                  if ( lpRow->Cells.dwItemCnt )
                     SSx_FreeRowCells( hWnd, lpSS, &lpRow->Cells );

                  lpItems2[ k ] = (zPCHAR) DrUnlockTaskMemory( lpRow );
                  DrFreeTaskMemory( lpItems2[ k ] );
               }
            }

            lpItems[ ss ].hItems = (zPCHAR) DrUnlockTaskMemory( lpItems2 );
            DrFreeTaskMemory( lpItems[ ss ].hItems );
         }
      }

      lpSS->Row.Items.hItems = (zPCHAR) DrUnlockTaskMemory( lpItems );
      DrFreeTaskMemory( lpSS->Row.Items.hItems );
      lpSS->Row.Items.hItems = 0;
      lpSS->Row.Items.dwItemCnt = 0;
   }

   if ( lpSS->Col.Items.hItems )
   {
//    lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpSS->Col.Items.hItems );
      lpItems = (LPSS_ALLOCITEM) lpSS->Col.Items.hItems;
      for ( ss = 0; ss < (zLONG) lpSS->Col.Items.dwItemCnt; ss++ )
      {
         if ( lpItems[ ss ].dwItemCnt )
         {
//          lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ ss ].hItems );
            lpItems2 = (zPCHAR *) lpItems[ ss ].hItems;

            for ( k = 0; k < lpItems[ ss ].dwItemCnt; k++ )
            {
               if ( lpItems2[ k ] )
               {
//                lpCol = (LPSS_COL) DrLockTaskMemory( lpItems2[ k ] );
                  lpCol = (LPSS_COL) lpItems2[ k ];

#ifndef SS_NOCALC
                  SS_FreeCalcList( &lpCol->Dependents );

                  if ( lpCol->hCalc )
                     SSx_FreeCalc( lpSS, lpCol->hCalc );
#endif

                  SS_DeAllocCellType( hWnd, lpSS, lpCol->hCellType );
                  SSx_FreeData( &lpCol->Data );

                  lpItems2[ k ] = (zPCHAR) DrUnlockTaskMemory( lpCol );
                  DrFreeTaskMemory( lpItems2[ k ] );
               }
            }

            lpItems[ ss ].hItems = (zPCHAR) DrUnlockTaskMemory( lpItems2 );
            DrFreeTaskMemory( lpItems[ ss ].hItems );
         }
      }

      lpSS->Col.Items.hItems = (zPCHAR) DrUnlockTaskMemory( lpItems );
      DrFreeTaskMemory( lpSS->Col.Items.hItems );
      lpSS->Col.Items.hItems = 0;
      lpSS->Col.Items.dwItemCnt = 0;
   }
}

void
SSx_FreeCol( HWND hWnd, LPSPREADSHEET lpSS, LPSS_COL lpCol )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_FreeCol" );
   SS_DeAllocCellType( hWnd, lpSS, lpCol->hCellType );
   SSx_FreeData( &lpCol->Data );

#ifndef SS_NOCALC
   SS_FreeCalcList( &lpCol->Dependents );

   if ( lpCol->hCalc )
   {
      SS_FreeCalc( lpSS, lpCol->hCalc );
      lpCol->hCalc = 0;
   }
#endif

#ifdef SS_BOUNDCONTROL
   if ( lpCol->hFieldName )
   {
      lpCol->hFieldName = DrUnlockTaskMemory( lpCol->hFieldName );
      DrFreeTaskMemory( lpCol->hFieldName );
      lpSS->DataFieldNameCnt--;
   }
#endif
}

void
SSx_FreeRow( HWND hWnd, LPSPREADSHEET lpSS, LPSS_ROW lpRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_FreeRow" );
   SS_DeAllocCellType( hWnd, lpSS, lpRow->hCellType );

#ifndef SS_NOCALC
   SS_FreeCalcList( &lpRow->Dependents );

   if ( lpRow->hCalc )
   {
      SS_FreeCalc( lpSS, lpRow->hCalc );
      lpRow->hCalc = 0;
   }
#endif

   SSx_FreeData( &lpRow->Data );

   if ( lpRow->Cells.dwItemCnt )
      SS_FreeAllocItem( &lpRow->Cells );
}

zPCHAR
SS_GetItem( LPSPREADSHEET lpSS, LPSS_ALLOCITEM lpAllocItem,
            register SS_COORD ssItemNbr, SS_COORD ssMaxItems )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_GetItem" );
   LPSS_ALLOCITEM  lpItems;
   zPCHAR          *lpItems2;
   zPCHAR          hItem = 0;
   DWORD           dwItemAt;
   WORD            wItem2At;

   if ( ssItemNbr + 1 > ssMaxItems )
      return( 0 );

   dwItemAt = CALCALLOCITEM( ssItemNbr );

   if ( dwItemAt < lpAllocItem->dwItemCnt )
   {
//    lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpAllocItem->hItems );
      lpItems = (LPSS_ALLOCITEM) lpAllocItem->hItems;
      wItem2At = CALCALLOCITEMREM( ssItemNbr );

      if ( wItem2At < lpItems[ dwItemAt ].dwItemCnt )
      {
//       lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ dwItemAt ].hItems );
         lpItems2 = (zPCHAR *) lpItems[ dwItemAt ].hItems;
         hItem = lpItems2[ wItem2At ];
//       DrUnlockTaskMemory( lpItems2 );
      }

//    DrUnlockTaskMemory( lpItems );
   }

   return( hItem );
}

zPCHAR
SS_GrabItemHandle( LPSPREADSHEET lpSS,
                   LPSS_ALLOCITEM lpAllocItem, SS_COORD ssItemNbr,
                   SS_COORD ssMaxItems )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_GrabItemHandle" );
   LPSS_ALLOCITEM  lpItems;
   zPCHAR          *lpItems2;
   zPCHAR          hItem = 0;
   DWORD           dwItemAt;
   WORD            wItem2At;

   if ( ssItemNbr + 1 <= ssMaxItems )
   {
      dwItemAt = CALCALLOCITEM( ssItemNbr );

      if ( dwItemAt < lpAllocItem->dwItemCnt )
      {
//       lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpAllocItem->hItems );
         lpItems = (LPSS_ALLOCITEM) lpAllocItem->hItems;
         wItem2At = CALCALLOCITEMREM( ssItemNbr );

         if ( wItem2At < lpItems[ dwItemAt ].dwItemCnt )
         {
            if ( lpItems[ dwItemAt ].hItems )
            {
//             lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ dwItemAt ].hItems );
               lpItems2 = (zPCHAR *) lpItems[ dwItemAt ].hItems;

               if ( lpItems2[ wItem2At ] )
               {
                  hItem = lpItems2[ wItem2At ];
                  lpItems2[ wItem2At ] = 0;
               }

//             DrUnlockTaskMemory( lpItems2 );
            }
         }

//       DrUnlockTaskMemory( lpItems );
      }
   }

   return( hItem );
}


HPSTR
SS_HugeBufferAlloc( HPSTR Buffer, zPLONG BufferLth, void *Value,
                    zLONG lValueLth, zPLONG AllocLth, zPCHAR        *hBuffer )
{
#if 0
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_HugeBufferAlloc" );
   if ( *BufferLth + ValueLth > *AllocLth )
   {
      *AllocLth += mMax( ValueLth, BUFFER_ALLOC_CNT );

      if ( !(*hBuffer) )
      {
         if ( (*hBuffer = GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT,
                                       *AllocLth )) == 0 )
         {
            *BufferLth = 0;
            return( FALSE );
         }
      }
      else
      {
         GlobalUnlock( *hBuffer );
         if ( (*hBuffer = GlobalReAlloc( *hBuffer, *AllocLth,
                                         GMEM_MOVEABLE | GMEM_ZEROINIT )) == 0 )
         {
            *BufferLth = 0;
            return( FALSE );
         }
      }

      Buffer = (HPSTR) GlobalLock( *hBuffer );
   }

   if ( ValueLth > 0 )
   {
      if ( Value )
         MemHugeCpy( &Buffer[ *BufferLth ], Value, ValueLth );
      else
         MemHugeSet( &Buffer[ *BufferLth ], 0, ValueLth );
   }

   *BufferLth += ValueLth;

   return( Buffer );
#else
   MESSAGE_DEBUG( "<SS_ALLOC.CPP>", "SS_HugeBufferAlloc" );
   return( 0 );
#endif

}

LPSPREADSHEET
SS_Lock( HWND hWnd )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_Lock" );
   LPSPREADSHEET lpSS = (LPSPREADSHEET) GetWindowLong( hWnd, zGWL_HSS );
//           DrLockTaskMemory( (zPCHAR) GetWindowLong( hWnd, zGWL_HSS ) );

   return( lpSS );
}

LPSS_CELL
SS_LockCellItem( LPSPREADSHEET lpSS, SS_COORD dCol, SS_COORD dRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_LockCellItem" );
   return( SSx_LockCellItem( lpSS, 0, dCol, dRow ) );
}

LPSS_COL
SS_LockColItem( LPSPREADSHEET lpSS, SS_COORD dCol )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_LockColItem" );
   zPCHAR        hCol;
   LPSS_COL      lpCol = 0;

   if ( lpSS->Col.LastRefCoord.Coord.ssCol == dCol &&
        lpSS->Col.LastRefCoord.hItem )
   {
//    lpCol = (LPSS_COL) DrLockTaskMemory( lpSS->Col.LastRefCoord.hItem );
      lpCol = (LPSS_COL) lpSS->Col.LastRefCoord.hItem;
   }
   else
   if ( (hCol = SS_GetItem( lpSS, &lpSS->Col.Items, dCol,
                            SS_GetColsMax( lpSS ) )) != 0 )
   {
      lpSS->Col.LastRefCoord.Coord.ssCol = dCol;
      lpSS->Col.LastRefCoord.hItem = hCol;
//    lpCol = (LPSS_COL) DrLockTaskMemory( hCol );
      lpCol = (LPSS_COL) hCol;
   }

   return( lpCol );
}

LPSS_ROW
SS_LockRowItem( LPSPREADSHEET lpSS, SS_COORD ssCellRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_LockRowItem" );
   zPCHAR    hRow;
   LPSS_ROW  lpRow = 0;

   if ( ssCellRow == -1 )
      return( 0 );

   if ( lpSS->Row.LastRefCoord.Coord.ssRow == ssCellRow &&
        lpSS->Row.LastRefCoord.hItem )
   {
//    lpRow = (LPSS_ROW) DrLockTaskMemory( lpSS->Row.LastRefCoord.hItem );
      lpRow = (LPSS_ROW) lpSS->Row.LastRefCoord.hItem;
   }
#ifdef DKS_VIRT
   else
   if ( lpSS->fVirtualMode ) // Handle virtual mode row allocation differently
   {
      SS_COORD ssBufferIdx = -1;
      LPSS_ALLOCITEM lpRows;

//    lpRows = (LPSS_ALLOCITEM) DrLockTaskMemory( lpSS->Virtual.VRows );
      lpRows = (LPSS_ALLOCITEM) lpSS->Virtual.VRows;

#if 0
      TraceLineI( "SS_LockRowItem ", ssCellRow );
      SS_COORD k;
      zCHAR szTemp[ 256 ];

      for ( k = lpSS->Virtual.VStartRow;
            k < lpSS->Virtual.VStartRow + lpSS->Virtual.VHighWaterIdx; k++ )
      {
         SSGetData( lpSS->hWnd, 1, k, szTemp ); // causes stack fault
         strcat_s( szTemp, zsizeof( szTemp ), ", " );
         TraceLineI( szTemp, k );
      }
#endif

      if ( ssCellRow == 0 )
      {
         ssBufferIdx = 0;     // looking for header row
      }
      else
      if ( ssCellRow >= lpSS->Virtual.VStartRow &&
           ssCellRow <=
                      lpSS->Virtual.VStartRow + lpSS->Virtual.VHighWaterIdx )
      {
         // it's already in our buffer pool ... just pick it out
         ssBufferIdx = lpSS->Virtual.VStartIdx +
                                    (ssCellRow - lpSS->Virtual.VStartRow);
         if ( ssBufferIdx > lpSS->Virtual.VHighWaterIdx )
            ssBufferIdx -= lpSS->Virtual.VHighWaterIdx;
      }

      if ( ssBufferIdx != -1 )
      {
         hRow = lpSS->Virtual.VRows[ ssBufferIdx ].hItems;
         lpSS->Row.LastRefCoord.Coord.ssRow = ssCellRow;
         lpSS->Row.LastRefCoord.hItem = hRow;
//       lpRow = (LPSS_ROW) DrLockTaskMemory( hRow );
         lpRow = (LPSS_ROW) hRow;
      }
      else
      {
//       TraceLineI( "Virtual row not located ", ssCellRow );
      }

//    DrUnlockTaskMemory( lpRows );
   }
#endif   // #ifdef DKS_VIRT
   else
   if ( (hRow = SS_GetItem( lpSS, &lpSS->Row.Items, ssCellRow,
                            SS_GetRowsMax( lpSS ) )) != 0 )
   {
      lpSS->Row.LastRefCoord.Coord.ssRow = ssCellRow;
      lpSS->Row.LastRefCoord.hItem = hRow;
//    lpRow = (LPSS_ROW) DrLockTaskMemory( hRow );
      lpRow = (LPSS_ROW) hRow;
   }

   return( lpRow );
}

zBOOL
SS_MoveAllocItems( LPSPREADSHEET lpSS,
                   LPSS_ALLOCITEM lpAllocItem, SS_COORD ssItemNbrSrc1,
                   SS_COORD ssItemNbrSrc2, SS_COORD ssItemNbrDest,
                   SS_COORD ssMaxItems )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_MoveAllocItems" );
   LPSS_ALLOCITEM  hItem;
   SS_COORD        wSrc;
   SS_COORD        wDest;

   wDest = ssItemNbrDest;

   if ( ssItemNbrSrc1 == ssItemNbrSrc2 + 1 )
      return( TRUE );
   else
   if ( ssItemNbrDest == ssItemNbrSrc1 || ssItemNbrSrc1 > ssItemNbrSrc2 )
      return( FALSE );
   else
   if ( ssItemNbrDest < ssItemNbrSrc1 )
   {
      for ( wSrc = ssItemNbrSrc1; wSrc <= ssItemNbrSrc2; wSrc++, wDest++ )
      {
         if ( (hItem = (LPSS_ALLOCITEM) SS_GrabItemHandle( lpSS, lpAllocItem,
                                                           wSrc, ssMaxItems )) != 0 )
         {
            if ( (hItem = SS_AllocItems( lpSS, lpAllocItem,
                                         wDest, 0, (zPCHAR) hItem,
                                         ssMaxItems, 0 )) == 0 )
            {
               return( FALSE );
            }
         }
      }
   }
   else
   {
      wDest = ssItemNbrSrc2 + (ssItemNbrDest - ssItemNbrSrc1);

      for ( wSrc = ssItemNbrSrc2; wSrc >= ssItemNbrSrc1; wSrc--, wDest-- )
      {
         if ( wDest < ssMaxItems )
         {
            if ( (hItem = (LPSS_ALLOCITEM) SS_GrabItemHandle( lpSS,
                                                              lpAllocItem, wSrc,
                                                              ssMaxItems )) != 0 )
            {
               if ( (hItem = SS_AllocItems( lpSS, lpAllocItem,
                                            wDest, 0, (zPCHAR) hItem,
                                            ssMaxItems, 0 )) == 0 )
               {
                  return( FALSE );
               }
            }
         }
      }
   }

   return( TRUE );
}

zLONG
SS_SendMsg( HWND hWnd, WORD uMsg, WPARAM wParam, LPARAM lParam )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_SendMsg" );
   LPSPREADSHEET lpSS;
   zLONG         lRet;

   lpSS = SS_Lock( hWnd );

   lpSS->wMessageBeingSent++;
   lRet = SSx_SendMsg( hWnd, lpSS, uMsg, wParam, lParam );

   if ( mIs_hWnd( hWnd ) == FALSE )
      return( lRet );

   lpSS->wMessageBeingSent--;

// DrUnlockTaskMemory( lpSS );
   return( lRet );
}

zLONG
SS_SendMsgCommand( HWND hWnd, LPSPREADSHEET lpSS,
                   WORD wCmd, zBOOL fNoMsgCnt )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_SendMsgCommand" );
   WORD wID = (WORD) GetWindowLong( hWnd, GWL_ID );
   WPARAM wParam = MAKELONG( wID, wCmd );
   if ( fNoMsgCnt )
      return( SSx_SendMsg( hWnd, lpSS, WM_COMMAND,
                           wParam, (LPARAM) hWnd ) );
   else
      return( SS_SendMsg( hWnd, WM_COMMAND,
                          wParam, (LPARAM) hWnd ) );
}

zLONG
SS_SendMsgCoords( HWND hWnd, WORD uMsg, WPARAM wParam,
                  SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_SendMsgCoords" );
#ifdef SS_USE16BITCOORDS
   return( SS_SendMsg( hWnd, uMsg, wParam, MAKELONG( ssCol, ssRow ) ) );
#else
   SS_CELLCOORD CellCoord;

   CellCoord.ssCol = ssCol;
   CellCoord.ssRow = ssRow;

   return( SS_SendMsg( hWnd, uMsg, wParam, (LPARAM)(zPCHAR) &CellCoord ) );
#endif
}

zLONG
SS_SendMsgEnterRow( HWND hWnd, WORD uMsg, WPARAM wParam,
                    SS_COORD ssRow, zBOOL fNewRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_SendMsgEnterRow" );
#ifdef SS_USE16BITCOORDS
   return( SS_SendMsg( hWnd, uMsg, wParam, MAKELONG( ssRow, (WORD) fNewRow ) ) );
#else
   SS_ENTERROW EnterRow;

   EnterRow.ssRow = ssRow;
   EnterRow.fNewRow = fNewRow;

   return( SS_SendMsg( hWnd, uMsg, wParam, (zLONG)(zPCHAR) &EnterRow ) );
#endif
}

zLONG
SS_SendMsgInsDel( HWND hWnd, WORD wType, WORD wOperation,
                  WORD wDirection, SS_COORD Coord1, SS_COORD Coord2 )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_SendMsgInsDel" );
   SS_INSDEL InsDel;

   InsDel.wType = wType;
   InsDel.wOperation = wOperation;
   InsDel.wDirection = wDirection;
   InsDel.Coord1 = Coord1;
   InsDel.Coord2 = Coord2;

   return( SS_SendMsg( hWnd, SSM_INSDEL, GetWindowLong( hWnd, GWL_ID ),
                       (zLONG)(zPCHAR) &InsDel ) );
}

zBOOL
SS_Unlock( HWND hWnd )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "DrUnlockTaskMemory" );
   return( SSx_Unlock( hWnd ) );
}

zPCHAR
SSx_AllocList( zPCHAR  hList,
               zPLONG  pnItemCnt,
               zSHORT  nItemSize )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_AllocList" );
   *pnItemCnt += 10;

   if ( hList )
   {
      hList = (zPCHAR) DrUnlockTaskMemory( hList );
      DrReAllocTaskMemory( (zCOREMEM) &hList, *pnItemCnt * nItemSize );
   }
   else
   {
      DrAllocTaskMemory( (zCOREMEM) &hList, *pnItemCnt * nItemSize, 1055 );
   }

   if ( hList == 0 )
      *pnItemCnt = 0;

   return( (zPCHAR) DrLockTaskMemory( hList ) );
}

void
SS_Free( HWND hWnd )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_Free" );
   LPSPREADSHEET lpSS;

   g_nSpreadInstCnt--;
   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      if ( lpSS->fVirtualMode && lpSS->Virtual.VSize )
      {
         zPCHAR         hRowItem;
         LPSS_ALLOCITEM lpRows;
         LPSS_ROW       lpRowItem;

//       lpRows = (LPSS_ALLOCITEM) DrLockTaskMemory( lpSS->Virtual.VRows );
         lpRows = (LPSS_ALLOCITEM) lpSS->Virtual.VRows;
         SS_COORD ssBufferIdx = lpSS->Virtual.VSize + 1;

         while ( ssBufferIdx-- )
         {
            hRowItem = lpSS->Virtual.VRows[ ssBufferIdx ].hItems;
            if ( hRowItem )
            {
               // remove all other data associated with this row???
//             lpRowItem = (LPSS_ROW) DrLockTaskMemory( hRowItem );
               lpRowItem = (LPSS_ROW) hRowItem;
               SSx_FreeRow( lpSS->hWnd, lpSS, lpRowItem );
               hRowItem = (zPCHAR) DrUnlockTaskMemory( lpRowItem );
               DrFreeTaskMemory( hRowItem );
            }
         }

         lpRows = (LPSS_ALLOCITEM) DrUnlockTaskMemory( lpRows );
         DrFreeTaskMemory( lpRows );
         lpSS->Virtual.VRows = 0;
         lpSS->Virtual.VSize = 0;
      }

      if ( lpSS->Col.hCellSizeList )
      {
         lpSS->Col.hCellSizeList = (zPCHAR)
                              DrUnlockTaskMemory( lpSS->Col.hCellSizeList );
         DrFreeTaskMemory( lpSS->Col.hCellSizeList );
         lpSS->Col.hCellSizeList = 0;
      }

      if ( lpSS->Row.hCellSizeList )
      {
         lpSS->Row.hCellSizeList = (zPCHAR)
                              DrUnlockTaskMemory( lpSS->Row.hCellSizeList );
         DrFreeTaskMemory( lpSS->Row.hCellSizeList );
         lpSS->Row.hCellSizeList = 0;
      }

      SS_FreeCalcFunctions( lpSS );
      SS_ClearAll( hWnd, lpSS );

      SetCursor( LoadCursor( 0, IDC_ARROW ) );
      SetClassLong( hWnd, GCL_HCURSOR, (zLONG) LoadCursor( 0, IDC_ARROW ) );

      DestroyCursor( lpSS->hCursorPointer );
      DestroyCursor( lpSS->hCursorResizeH );
      DestroyCursor( lpSS->hCursorResizeV );

      DeleteBrush( lpSS->hBrushLines );
      DeleteBrush( lpSS->hBrushLines2 );
      DeleteBitmap( lpSS->hBitmapLines );
      DeleteBitmap( lpSS->hBitmapLines2 );
      DeleteBitmap( lpSS->hBitmapComboBtn );

#ifndef SS_NOCHILDSCROLL
      if ( lpSS->hWndVScroll )
         DestroyWindow( lpSS->hWndVScroll );

      if ( lpSS->hWndHScroll )
         DestroyWindow( lpSS->hWndHScroll );
#endif


#ifdef SS_NOSUPERCLASS
   {
      LPSPREADSHEET lpSS;

      lpSS = (LPSPREADSHEET) MAKELONG( GetProp( hWnd, "Xtra1" ),
                                       GetProp( hWnd, "Xtra2" ) );
      DrFreeTaskMemory( lpSS );
      RemoveProp( hWnd, "Xtra1" );
      RemoveProp( hWnd, "Xtra2" );
   }
#else
      lpSS = (LPSPREADSHEET) DrUnlockTaskMemory( lpSS );
      DrFreeTaskMemory( lpSS );
      SetWindowLong( hWnd, zGWL_HSS, 0 );
#endif
   }

   if ( g_nSpreadInstCnt == 0 )
   {
      SS_DeleteColorTable( );
      SS_DeleteFontTable( );
   }
}

void
SSx_FreeData( LPSS_DATA lpData )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_FreeData" );
   if ( (lpData->m_DataType == SS_TYPE_EDIT ||
         lpData->m_DataType == SS_TYPE_COMBOBOX) && lpData->m_Data.hszData )
   {
      lpData->m_Data.hszData = (zPCHAR)
                            DrUnlockTaskMemory( lpData->m_Data.hszData );
      DrFreeTaskMemory( lpData->m_Data.hszData );
      lpData->m_Data.hszData = 0;
   }

   lpData->m_DataType = 0;
}

void
SSx_FreeRowCells( HWND hWnd, LPSPREADSHEET lpSS,
                  LPSS_ALLOCITEM lpCells )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_FreeRowCells" );
   LPSS_ALLOCITEM   lpItems;
   zPCHAR           *lpItems2;
   LPSS_CELL        lpCell;
   SS_COORD         ss;
   zUSHORT          k;

   if ( lpCells->hItems )
   {
//    lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpCells->hItems );
      lpItems = (LPSS_ALLOCITEM) lpCells->hItems;

      for ( ss = 0; ss < (zLONG) lpCells->dwItemCnt; ss++ )
      {
         if ( lpItems[ ss ].dwItemCnt )
         {
//          lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ ss ].hItems );
            lpItems2 = (zPCHAR *) lpItems[ ss ].hItems;

            for ( k = 0; k < lpItems[ ss ].dwItemCnt; k++ )
            {
               if ( lpItems2[ k ] )
               {
//                lpCell = (LPSS_CELL) DrLockTaskMemory( lpItems2[ k ] );
                  lpCell = (LPSS_CELL) lpItems2[ k ];

#ifndef SS_NOCALC
                  SS_FreeCalcList( &lpCell->m_Dependents );

                  if ( lpCell->m_hCalc )
                     SSx_FreeCalc( lpSS, lpCell->m_hCalc );
#endif

                  SS_DeAllocCellType( hWnd, lpSS, lpCell->m_hCellType );
                  SSx_FreeData( &lpCell->m_CellData );

                  lpItems2[ k ] = (zPCHAR) DrUnlockTaskMemory( lpCell );
                  DrFreeTaskMemory( lpItems2[ k ] );
               }
            }

            lpItems[ ss ].hItems = (zPCHAR) DrUnlockTaskMemory( lpItems2 );
            DrFreeTaskMemory( lpItems[ ss ].hItems );
         }
      }

      lpCells->hItems = (zPCHAR) DrUnlockTaskMemory( lpItems );
      DrFreeTaskMemory( lpCells->hItems );
      lpCells->hItems = 0;
      lpCells->dwItemCnt = 0;
   }
}

LPSS_CELL
SSx_LockCellItem( LPSPREADSHEET lpSS, LPSS_ROW lpRow,
                  SS_COORD dCol, SS_COORD dRow )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_LockCellItem" );
   zPCHAR        hCell;
   LPSS_CELL     lpCell = 0;
   LPSS_ROW      lpRowOld = lpRow;

   if ( lpSS->LastCell.Coord.ssRow == dRow &&
        lpSS->LastCell.Coord.ssCol == dCol )
   {
//    lpCell = (LPSS_CELL) DrLockTaskMemory( lpSS->LastCell.hItem );
      lpCell = (LPSS_CELL) lpSS->LastCell.hItem;
   }

   if ( lpRow || (lpRow = SS_LockRowItem( lpSS, dRow )) != 0 )
   {
      if ( (hCell = SS_GetItem( lpSS, &lpRow->Cells, dCol,
                                SS_GetColsMax( lpSS ) )) != 0 )
      {
         lpSS->LastCell.Coord.ssRow = dRow;
         lpSS->LastCell.Coord.ssCol = dCol;
         lpSS->LastCell.hItem = hCell;

//       lpCell = (LPSS_CELL) DrLockTaskMemory( hCell );
         lpCell = (LPSS_CELL) hCell;
      }

      if ( lpRowOld == 0 )
         SS_UnlockRowItem( lpSS, dRow );
   }

   return( lpCell );
}

zLONG
SSx_SendMsg( HWND hWnd, LPSPREADSHEET lpSS, WORD uMsg,
             WPARAM wParam, LPARAM lParam )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_SendMsg" );
   zLONG lRet = 0;

   if ( uMsg == SSM_DATACHANGE )
      lpSS->fChangeMade = TRUE;

   if ( lpSS->lpfnCallBack == 0 )
   {
      lRet = SendMessage( hWnd, uMsg, wParam, lParam );
      if ( mIs_hWnd( hWnd ) == FALSE )
         return( lRet );
   }

   if ( lpSS->hWndOwner )
   {
      lRet = SendMessage( lpSS->hWndOwner, uMsg, wParam, lParam );
      if ( mIs_hWnd( hWnd ) == FALSE )
         return( lRet );
   }

   if ( lpSS->lpfnCallBack )
      lRet = (*lpSS->lpfnCallBack)( hWnd, uMsg, wParam, lParam );

   return( lRet );
}

zBOOL
SSx_Unlock( HWND hWnd )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SSx_Unlock" );
   zPCHAR        hSpread;

   hSpread = (zPCHAR) GetWindowLong( hWnd, zGWL_HSS );
   return( hSpread ? TRUE : FALSE );

// if ( hSpread )
//    return( (zBOOL) SS_Unlock( hSpread ) );
//
// return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ss_bool.cpp

zBOOL OPERATION
SSSetBool( HWND   hWnd,
           zSHORT nIndex,
           zBOOL  fNewVal )
{
   TRACE_DEBUG( "<SS_BOOL.CPP>", "SSSetBool" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   RECT          rect;
   zBOOL         fPrevVal = 0;

   switch ( nIndex )
   {
      case SSB_PROTECT:
         fPrevVal = lpSS->DocumentProtected;
         lpSS->DocumentProtected = fNewVal;
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
         break;

      case SSB_RESTRICTROWS:
         fPrevVal = lpSS->RestrictRows;
         lpSS->RestrictRows = fNewVal;
         break;

      case SSB_RESTRICTCOLS:
         fPrevVal = lpSS->RestrictCols;
         lpSS->RestrictCols = fNewVal;
         break;

      case SSB_SHOWCOLHEADERS:
         if ( fNewVal )
         {
            if ( SS_GetRowHeight( lpSS, 0 ) == 0 )
               SSSetRowHeight( hWnd, SS_HEADER, SS_HEIGHT_DEFAULT );
         }
         else
            SSSetRowHeight( hWnd, SS_HEADER, 0 );

         break;

      case SSB_SHOWROWHEADERS:
         if ( fNewVal )
         {
            if ( SS_GetColWidth( lpSS, 0 ) == 0 )
               SSSetColWidth( hWnd, SS_HEADER, -2 );
         }
         else
            SSSetColWidth( hWnd, SS_HEADER, 0 );

         break;

      case SSB_ALLOWUSERRESIZE:
         if ( lpSS->wUserResize & (SS_USERRESIZE_COL | SS_USERRESIZE_ROW) )
            fPrevVal = TRUE;
         else
            fPrevVal = FALSE;

         if ( fPrevVal != fNewVal )
         {
            if ( fNewVal )
               lpSS->wUserResize |= SS_USERRESIZE_COL | SS_USERRESIZE_ROW;
            else
               lpSS->wUserResize &= ~(SS_USERRESIZE_COL | SS_USERRESIZE_ROW);
         }

         break;

      case SSB_ALLOWUSERSELBLOCK:
         if ( lpSS->fEditModePermanent == 0 )
         {
            if ( lpSS->wSelBlockOption &
                 (SS_SELBLOCK_COLS | SS_SELBLOCK_ROWS |
                  SS_SELBLOCK_BLOCKS | SS_SELBLOCK_ALL) )
            {
               fPrevVal = TRUE;
            }
            else
               fPrevVal = FALSE;

            if ( fPrevVal != fNewVal )
            {
               if ( fNewVal )
                  lpSS->wSelBlockOption |= SS_SELBLOCK_COLS |
                                           SS_SELBLOCK_ROWS |
                                           SS_SELBLOCK_BLOCKS |
                                           SS_SELBLOCK_ALL;
               else
                  lpSS->wSelBlockOption &= ~(SS_SELBLOCK_COLS |
                                             SS_SELBLOCK_ROWS |
                                             SS_SELBLOCK_BLOCKS |
                                             SS_SELBLOCK_ALL);

               SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
            }
         }

         break;

      case SSB_AUTOCALC:
         fPrevVal = lpSS->CalcAuto;
         lpSS->CalcAuto = fNewVal;
         break;

      case SSB_REDRAW:
         fPrevVal = lpSS->Redraw;
         if ( fPrevVal != fNewVal )
         {
            lpSS->Redraw = fNewVal;
            if ( fNewVal )
            {
               if ( lpSS->fUpdateAutoSize )
                  SS_AutoSize( hWnd, FALSE );
               else
               {
                  SS_SetVScrollBar( hWnd, lpSS );
                  SS_SetHScrollBar( hWnd, lpSS );
               }

               if ( mIs_hWndVisible( hWnd ) )
               {
                  if ( GetUpdateRect( hWnd, &rect, FALSE ) )
                     UpdateWindow( hWnd );
                  else
                  if ( fPrevVal )
                     SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
               }
            }
         }

         break;

      case SSB_GRIDLINES:
         if ( lpSS->wGridType & (SS_GRID_HORIZONTAL | SS_GRID_VERTICAL) )
            fPrevVal = TRUE;
         else
            fPrevVal = FALSE;

         if ( fPrevVal != fNewVal )
         {
            if ( fNewVal )
               lpSS->wGridType |= SS_GRID_HORIZONTAL | SS_GRID_VERTICAL;
            else
               lpSS->wGridType &= ~(SS_GRID_HORIZONTAL | SS_GRID_VERTICAL);

            SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
         }

         break;

      case SSB_HORZSCROLLBAR:
         fPrevVal = lpSS->HorzScrollBar;
         if ( fPrevVal != fNewVal )
         {
            lpSS->HorzScrollBar = fNewVal;
            if ( fNewVal )
               SS_SetHScrollBar( hWnd, lpSS );
            else
               SetScrollRange( hWnd, SB_HORZ, 0, 0, TRUE );

            SS_AutoSize( hWnd, FALSE );
         }

         break;

      case SSB_VERTSCROLLBAR:
         fPrevVal = lpSS->VertScrollBar;
         if ( fPrevVal != fNewVal )
         {
            lpSS->VertScrollBar = fNewVal;
            if ( fNewVal )
               SS_SetVScrollBar( hWnd, lpSS );
            else
               SetScrollRange( hWnd, SB_VERT, 0, 0, TRUE );

            SS_AutoSize( hWnd, FALSE );
         }

         break;

#ifndef SS_NOCALC
      case SSB_ALLOWUSERFORMULAS:
         fPrevVal = lpSS->AllowUserFormulas;
         lpSS->AllowUserFormulas = fNewVal;
         break;
#endif

      case SSB_EDITMODEPERMANENT:
         fPrevVal = lpSS->fEditModePermanent;

         // Added !lpSS->wMessageBeingSent to fix a painting problem
         // when EDITMODEPERMANENT is called from the LEAVECELL message.

         if ( fPrevVal != fNewVal && lpSS->wMessageBeingSent == 0 )
            SS_HiliteCell( hWnd, FALSE );

         lpSS->fEditModePermanent = fNewVal;

         if ( fPrevVal != fNewVal && lpSS->wMessageBeingSent == 0 )
            SS_HiliteCell( hWnd, TRUE );

         if ( fNewVal )
         {
            if ( lpSS->fAllowEditModePermSel )
               lpSS->wSelBlockOption = SS_SELBLOCK_COLS | SS_SELBLOCK_ROWS;
            else
               lpSS->wSelBlockOption = 0;
         }

         break;

      case SSB_AUTOSIZE:
         fPrevVal = lpSS->fAutoSize;
         lpSS->fAutoSize = fNewVal;

         if ( fPrevVal != fNewVal )
         {
            SS_AutoSize( hWnd, FALSE );
            InvalidateRect( hWnd, 0, TRUE );
            UpdateWindow( hWnd );
         }

         break;

      case SSB_SCROLLBAREXTMODE:
         fPrevVal = lpSS->fScrollBarExtMode;
         lpSS->fScrollBarExtMode = fNewVal;

         if ( fPrevVal != fNewVal )
         {
            SS_AutoSize( hWnd, FALSE );
            InvalidateRect( hWnd, 0, TRUE );
            UpdateWindow( hWnd );
         }

         break;

      case SSB_SCROLLBARSHOWMAX:
         fPrevVal = lpSS->fScrollBarShowMax;
         lpSS->fScrollBarShowMax = fNewVal;
         SS_SetVScrollBar( hWnd, lpSS );
         SS_SetHScrollBar( hWnd, lpSS );
         break;

      case SSB_SCROLLBARMAXALIGN:
         fPrevVal = lpSS->fScrollBarMaxAlign;
         lpSS->fScrollBarMaxAlign = fNewVal;
         SS_SetVScrollBar( hWnd, lpSS );
         SS_SetHScrollBar( hWnd, lpSS );
         break;

      case SSB_PROCESSTAB:
         fPrevVal = lpSS->fProcessTab;
         lpSS->fProcessTab = fNewVal;
         break;

      case SSB_ARROWSEXITEDITMODE:
         fPrevVal = lpSS->fArrowsExitEditMode;
         lpSS->fArrowsExitEditMode = fNewVal;
         break;

      case SSB_MOVEACTIVEONFOCUS:
         fPrevVal = lpSS->fMoveActiveOnFocus;
         lpSS->fMoveActiveOnFocus = fNewVal;
         break;

      case SSB_RETAINSELBLOCK:
         fPrevVal = lpSS->fRetainSelBlock;
         lpSS->fRetainSelBlock = fNewVal;
         break;

      case SSB_EDITMODEREPLACE:
         fPrevVal = lpSS->fEditModeReplace;
         lpSS->fEditModeReplace = fNewVal;
         break;

      case SSB_NOBORDER:
         fPrevVal = lpSS->fNoBorder;
         lpSS->fNoBorder = fNewVal;

         if ( fPrevVal != fNewVal )
            SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

         break;

      case SSB_NOBEEP:
         fPrevVal = lpSS->fNoBeep;
         lpSS->fNoBeep = fNewVal;
         break;

      case SSB_AUTOCLIPBOARD:
         fPrevVal = lpSS->fAutoClipboard;
         lpSS->fAutoClipboard = fNewVal;
         break;

      case SSB_ALLOWMULTIBLOCKS:
         fPrevVal = lpSS->fAllowMultipleSelBlocks;
         lpSS->fAllowMultipleSelBlocks = fNewVal;
         break;

      case SSB_ALLOWCELLOVERFLOW:
         fPrevVal = lpSS->fAllowCellOverflow;
         lpSS->fAllowCellOverflow = fNewVal;

         if ( lpSS->fAllowCellOverflow )
         {
            LPSS_CELL lpCell;
            SS_COORD  k;
            SS_COORD  j;

            for ( k = lpSS->Row.nHeaderCnt; k < lpSS->Row.DataCnt; k++ )
            {
               for ( j = lpSS->Col.nHeaderCnt; j < lpSS->Col.DataCnt; j++ )
               {
                  if ( (lpCell = SS_LockCellItem( lpSS, j, k )) != 0 )
                  {
                     SS_CalcCellOverflow( hWnd, lpSS, lpCell, 0, j, k );
                     SS_UnlockCellItem( lpSS, j, k );
                  }
               }
            }
         }

         if ( fPrevVal != fNewVal )
            SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

         break;

      case SSB_VIRTUALMODE:
         fPrevVal = lpSS->fVirtualMode;
         lpSS->fVirtualMode = fNewVal;

         if ( fNewVal )
         {
            zBOOL fRedraw;

            lpSS->Virtual.VPhysSize = 0;
            lpSS->Virtual.VTop = lpSS->Row.nHeaderCnt;
            lpSS->Row.UL = lpSS->Row.nHeaderCnt;
            lpSS->Row.ssCurrAt = lpSS->Row.nHeaderCnt;

            fRedraw = lpSS->Redraw;
            lpSS->Redraw = FALSE;

            if ( lpSS->Virtual.VMax == -1 )
               SS_SetMaxRows( hWnd, SS_VMAXROWS );
            else
               SS_SetMaxRows( hWnd, lpSS->Virtual.VMax );

            lpSS->Redraw = fRedraw;
            InvalidateRect( hWnd, 0, TRUE );
            UpdateWindow( hWnd );
         }

         break;

      case SSB_ALLOWDRAGDROP:
         fPrevVal = lpSS->fAllowDragDrop;
         lpSS->fAllowDragDrop = fNewVal;
         break;

      case SSB_CHANGEMADE:
         fPrevVal = lpSS->fChangeMade;
         lpSS->fChangeMade = fNewVal;
         break;
   }

// DrUnlockTaskMemory( lpSS );
   return( fPrevVal );
}


zBOOL OPERATION
SSGetBool( HWND   hWnd,
           zSHORT nIndex )
{
   TRACE_DEBUG( "<SS_BOOL.CPP>", "SSGetBool" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zLONG         dxRowHeight;
   zLONG         dxColWidth;
   zBOOL         fPrevVal = 0;

   switch ( nIndex )
   {
      case SSB_PROTECT:
         fPrevVal = lpSS->DocumentProtected;
         break;

      case SSB_RESTRICTROWS:
         fPrevVal = lpSS->RestrictRows;
         break;

      case SSB_RESTRICTCOLS:
         fPrevVal = lpSS->RestrictCols;
         break;

      case SSB_SHOWCOLHEADERS:
         if ( SSGetRowHeight( hWnd, SS_HEADER, &dxRowHeight ) &&
              dxRowHeight == 0.0 )
         {
            fPrevVal = FALSE;
         }
         else
            fPrevVal = TRUE;

         break;

      case SSB_SHOWROWHEADERS:
            if ( SSGetColWidth( hWnd, SS_HEADER, &dxColWidth ) &&
                dxColWidth == 0.0 )
            fPrevVal = FALSE;
         else
            fPrevVal = TRUE;

         break;

      case SSB_ALLOWUSERRESIZE:
         if ( lpSS->wUserResize & (SS_USERRESIZE_COL | SS_USERRESIZE_ROW) )
            fPrevVal = TRUE;
         else
            fPrevVal = FALSE;

         break;

      case SSB_ALLOWUSERSELBLOCK:
         if ( lpSS->wSelBlockOption & (SS_SELBLOCK_COLS | SS_SELBLOCK_ROWS |
                                       SS_SELBLOCK_BLOCKS | SS_SELBLOCK_ALL) )
         {
            fPrevVal = TRUE;
         }
         else
            fPrevVal = FALSE;

         break;

      case SSB_AUTOCALC:
         fPrevVal = lpSS->CalcAuto;
         break;

      case SSB_REDRAW:
         fPrevVal = lpSS->Redraw;
         break;

      case SSB_GRIDLINES:
         if ( lpSS->wGridType & (SS_GRID_HORIZONTAL | SS_GRID_VERTICAL) )
            fPrevVal = TRUE;
         else
            fPrevVal = FALSE;

         break;

      case SSB_HORZSCROLLBAR:
         fPrevVal = lpSS->HorzScrollBar;
         break;

      case SSB_VERTSCROLLBAR:
         fPrevVal = lpSS->VertScrollBar;
         break;

#ifndef SS_NOCALC
      case SSB_ALLOWUSERFORMULAS:
         fPrevVal = lpSS->AllowUserFormulas;
         break;
#endif

      case SSB_EDITMODEPERMANENT:
         fPrevVal = lpSS->fEditModePermanent;
         break;

      case SSB_AUTOSIZE:
         fPrevVal = lpSS->fAutoSize;
         break;

      case SSB_SCROLLBAREXTMODE:
         fPrevVal = lpSS->fScrollBarExtMode;
         break;

      case SSB_SCROLLBARSHOWMAX:
         fPrevVal = lpSS->fScrollBarShowMax;
         break;

      case SSB_SCROLLBARMAXALIGN:
         fPrevVal = lpSS->fScrollBarMaxAlign;
         break;

      case SSB_PROCESSTAB:
         fPrevVal = lpSS->fProcessTab;
         break;

      case SSB_ARROWSEXITEDITMODE:
         fPrevVal = lpSS->fArrowsExitEditMode;
         break;

      case SSB_MOVEACTIVEONFOCUS:
         fPrevVal = lpSS->fMoveActiveOnFocus;
         break;

      case SSB_RETAINSELBLOCK:
         fPrevVal = lpSS->fRetainSelBlock;
         break;

      case SSB_EDITMODEREPLACE:
         fPrevVal = lpSS->fEditModeReplace;
         break;

      case SSB_NOBORDER:
         fPrevVal = lpSS->fNoBorder;
         break;

      case SSB_NOBEEP:
         fPrevVal = lpSS->fNoBeep;
         break;

      case SSB_AUTOCLIPBOARD:
         fPrevVal = lpSS->fAutoClipboard;
         break;

      case SSB_ALLOWMULTIBLOCKS:
         fPrevVal = lpSS->fAllowMultipleSelBlocks;
         break;

      case SSB_ALLOWCELLOVERFLOW:
         fPrevVal = lpSS->fAllowCellOverflow;
         break;

      case SSB_VIRTUALMODE:
         fPrevVal = lpSS->fVirtualMode;
         break;

      case SSB_ALLOWDRAGDROP:
         fPrevVal = lpSS->fAllowDragDrop;
         break;

      case SSB_CHANGEMADE:
         fPrevVal = lpSS->fChangeMade;
         break;
   }

// DrUnlockTaskMemory( lpSS );
   return( fPrevVal );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_data.cpp

zBOOL
SS_ClearDataRange( HWND hWnd, SS_COORD ssCol, SS_COORD ssRow,
                   SS_COORD ssCol2, SS_COORD ssRow2,
                   zBOOL fIgnoreLockedCells, zBOOL fSendDataChange )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SS_ClearDataRange" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   LPSS_CELL     lpCell;
   SS_COORD      ssColAtOld;
   SS_COORD      ssColLeftOld;
   SS_COORD      ssRowAtOld;
   SS_COORD      ssRowTopOld;
   SS_COORD      ssColAt;
   SS_COORD      ssRowAt;
   SS_COORD      ss;
   SS_COORD      j;
   zBOOL         fEditMode = FALSE;
   zBOOL         Ret = FALSE;

   lpSS = SS_Lock( hWnd );

   ssColAtOld = lpSS->Col.ssCurrAt;
   ssColLeftOld = lpSS->Col.UL;
   ssRowAtOld = lpSS->Row.ssCurrAt;
   ssRowTopOld = lpSS->Row.UL;

   if ( ssCol == -1 )
      ssCol = lpSS->Col.nHeaderCnt;

   if ( ssCol == -1 || ssCol2 == -1 )
      ssCol2 = lpSS->Col.DataCnt - 1;

   if ( ssRow == -1 )
      ssRow = lpSS->Row.nHeaderCnt;

   if ( lpSS->fVirtualMode )
      ssRow = mMax( ssRow, lpSS->Virtual.VTop );

   if ( ssRow == -1 || ssRow2 == -1 )
      ssRow2 = lpSS->Row.DataCnt - 1;

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );

   if ( lpSS->wOpMode == SS_OPMODE_ROWMODE )
   {
      SS_CellEditModeOff( hWnd, lpSS, 0 );
      SS_HiliteCell( hWnd, FALSE );
   }
   else
   if ( lpSS->EditModeOn &&
        lpSS->Col.ssCurrAt >= ssCol && lpSS->Col.ssCurrAt <= ssCol2 &&
        lpSS->Row.ssCurrAt >= ssRow && lpSS->Row.ssCurrAt <= ssRow2 )
   {
      fEditMode = TRUE;
   }

   for ( ss = ssRow; ss <= ssRow2; ss++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
      {
         for ( j = ssCol; j <= ssCol2; j++ )
         {
            if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, j, ss )) != 0 )
            {
               if ( !fIgnoreLockedCells || !SS_GetLock( hWnd, lpSS, j, ss ) )
               {
                  if ( lpCell->m_CellData.m_DataType )
                  {
                     SSx_FreeData( &lpCell->m_CellData );
                     SS_ResetCellOverflow( hWnd, lpSS, j, ss );

                     if ( fSendDataChange )
                        SS_SendMsgCoords( hWnd, SSM_DATACHANGE,
                                          GetDlgCtrlID( hWnd ),
                                          j - lpSS->Col.nHeaderCnt + 1,
                                          ss - lpSS->Row.nHeaderCnt + 1 );

                     if ( lpSS->CalcAuto )
                     {
                        ssColAt = lpSS->Col.ssCurrAt;
                        ssRowAt = lpSS->Row.ssCurrAt;
                        lpSS->Col.ssCurrAt = j;
                        lpSS->Row.ssCurrAt = ss;
                        SS_CalcDependencies( hWnd, lpSS, j, ss );
                        lpSS->Col.ssCurrAt = ssColAt;
                        lpSS->Row.ssCurrAt = ssRowAt;
                     }
                  }
               }

               SSx_UnlockCellItem( lpSS, lpRow, j, ss );
            }
         }

         SS_UnlockRowItem( lpSS, ss );
      }
   }

   {
      if ( (ssCol == SS_ALLCOLS || ssCol >= lpSS->Col.nHeaderCnt) &&
           (ssCol2 == SS_ALLCOLS || ssCol2 <= lpSS->Col.DataCnt - 1) &&
           ssRow2 >= lpSS->Row.DataCnt - 1 )
      {
         if ( lpSS->wOpMode == SS_OPMODE_ROWMODE &&
              lpSS->Row.ssCurrAt > lpSS->Row.DataCnt )
         {
            lpSS->Row.ssCurrAt = lpSS->Row.DataCnt;
         }
      }

      SS_AdjustDataColCnt( lpSS, ssCol, ssCol2 );
      SS_AdjustDataRowCnt( lpSS, ssRow, ssRow2 );

      Ret = TRUE;
   }

   SS_InvalidateRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2 );

   if ( lpSS->wOpMode == SS_OPMODE_ROWMODE )
   {
      if ( lpSS->fRowModeEditing )
      {
         lpSS->fRowModeEditing = FALSE;
#ifndef SS_NOBUTTONDRAWMODE
         SS_InvalidateButtons( hWnd, lpSS, ssColAtOld, ssColLeftOld,
                               ssRowAtOld, ssRowTopOld );
#endif
      }

      SS_HiliteCell( hWnd, TRUE );
   }
   else
   if ( fEditMode )
      SS_SetEditModeOnData( hWnd, lpSS );

// DrUnlockTaskMemory( lpSS );
   return( Ret );
}

void
SS_ClearUndoBuffer( HWND hWnd,
                    LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_ClearUndoBuffer" );
}

zPVOID  OPERATION
SSClipOut( HWND      hWnd,
           SS_COORD  ssCol,
           SS_COORD  ssRow,
           SS_COORD  ssCol2,
           SS_COORD  ssRow2 )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSClipOut" );
   return( SS_ClipOut( hWnd, ssCol, ssRow, ssCol2, ssRow2, FALSE ) );
}

zPVOID
SS_ClipOut( HWND      hWnd,
            SS_COORD  ssCol,
            SS_COORD  ssRow,
            SS_COORD  ssCol2,
            SS_COORD  ssRow2,
            zBOOL     fValue )
{
#if 0
   TRACE_DEBUG( "<SS_DATA.CPP>", "SS_ClipOut" );
   LPSPREADSHEET  lpSS;
   zPCHAR         hBuffer = 0;
   zPCHAR         hData;
   SS_CELLTYPE    CellType;
   zPCHAR         Buffer = 0;   // HPSTR???
   zPCHAR         lpData;
   zLONG          lBufferLth = 0;
   zLONG          lAllocLth = 0;
   zSHORT         nLth;
   SS_COORD       x;
   SS_COORD       y;

   lpSS = SS_Lock( hWnd );

   if ( ssCol != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col, 0 );

   if ( ssRow != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row );

   if ( ssCol2 != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col2, 0 );

   if ( ssRow2 != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row2 );

   if ( ssCol == -1 )
   {
      ssCol = 0;
      ssCol2 = lpSS->Col.DataCnt - 1;
   }
   else
   if ( ssCol2 == -1 )
      ssCol2 = lpSS->Col.DataCnt - 1;

   if ( ssRow == -1 )
   {
      ssRow = 0;
      ssRow2 = lpSS->Row.DataCnt - 1;
   }
   else
   if ( ssRow2 == -1 )
      ssRow2 = lpSS->Row.DataCnt - 1;

   if ( ssRow > 0 )
      ssCol2 = mMin( ssCol2, lpSS->Col.DataCnt - 1 );

   if ( ssCol > 0 )
      ssRow2 = mMin( ssRow2, lpSS->Row.DataCnt - 1 );

   if ( ssCol > ssCol2 || ssRow > ssRow2 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( 0 );
   }

   for ( y = ssRow; y <= ssRow2; y++ )
   {
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         SS_RetrieveCellType( lpSS, &CellType, 0, x, y );

         if ( fValue )
            hData = SS_GetValue( hWnd, lpSS, x, y );
         else
            hData = SS_GetData( lpSS, &CellType, x, y, FALSE );

         if ( hData )
         {
//          lpData = (zPCHAR) DrLockTaskMemory( hData );
            lpData = (zPCHAR) hData;
            nLth = zstrlen( lpData );
            Buffer = SS_HugeBufferAlloc( Buffer, &lBufferLth, lpData, nLth,
                                         &lAllocLth, &hBuffer );
//          DrUnlockTaskMemory( lpData );

            if ( fValue )
            {
               hData = DrUnlockTaskMemory( lpData );
               DrFreeTaskMemory( hData );
            }
         }

         if ( x == ssCol2 )
            Buffer = SS_HugeBufferAlloc( Buffer, &lBufferLth, "\r\n", 2,
                                         &lAllocLth, &hBuffer );
         else
            Buffer = SS_HugeBufferAlloc( Buffer, &lBufferLth, "\t", 1,
                                         &lAllocLth, &hBuffer );
      }
   }

   Buffer = SS_HugeBufferAlloc( Buffer, &lBufferLth, 0, 1,
                                &lAllocLth, &hBuffer );

   if ( hBuffer )
      GlobalUnlock( hBuffer );

// DrUnlockTaskMemory( lpSS );
   return( hBuffer );
#else
   MESSAGE_DEBUG( "<SS_DATA.CPP>", "SS_ClipOut" );
   return( 0 );
#endif
}

zSHORT OPERATION
SSGetData( HWND      hWnd,
           SS_COORD  ssCol,
           SS_COORD  ssRow,
           zPCHAR    lpBuffer )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetData" );
   LPSPREADSHEET  lpSS;
   zPCHAR         hData;
   SS_CELLTYPE    CellType;
   zPCHAR         lpData;
   zBOOL          Ret = FALSE;

   if ( lpBuffer == 0 )
      return( 0 );

   lpBuffer[ 0 ] = 0;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, ssRow );

   if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol && lpSS->Row.ssCurrAt == ssRow )
      hData = SS_GetEditModeOnData( hWnd, lpSS, FALSE );
   else
      hData = SS_GetData( lpSS, &CellType, ssCol, ssRow, FALSE );

   if ( hData )
   {
//    lpData = (zPCHAR) DrLockTaskMemory( hData );
      lpData = (zPCHAR) hData;
      strcpy_s( lpBuffer, lpData );
//    DrUnlockTaskMemory( lpData );

      if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol &&
           lpSS->Row.ssCurrAt == ssRow )
      {
         hData = (zPCHAR) DrUnlockTaskMemory( lpData );
         DrFreeTaskMemory( hData );
      }

      Ret = TRUE;
   }

// DrUnlockTaskMemory( lpSS );

   return( zstrlen( lpBuffer ) );
}

zSHORT OPERATION
SSGetDataLth( HWND      hWnd,
              SS_COORD  ssCol,
              SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetDataLth" );
   LPSPREADSHEET  lpSS;
   SS_CELLTYPE    CellType;
   zPCHAR         hData;
   zPCHAR         lpData;
   zSHORT         nLth = 0;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, ssRow );

   if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol && lpSS->Row.ssCurrAt == ssRow )
      hData = SS_GetEditModeOnData( hWnd, lpSS, FALSE );
   else
      hData = SS_GetData( lpSS, &CellType, ssCol, ssRow, FALSE );

   if ( hData )
   {
//    lpData = (zPCHAR) DrLockTaskMemory( hData );
      lpData = (zPCHAR) hData;
      nLth = zstrlen( lpData );
//    DrUnlockTaskMemory( lpData );

      if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol &&
           lpSS->Row.ssCurrAt == ssRow )
      {
         hData = (zPCHAR) DrUnlockTaskMemory( lpData );
         DrFreeTaskMemory( hData );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( nLth );
}

zPCHAR
SS_GetEditModeOnData( HWND  hWnd,
                      LPSPREADSHEET lpSS,
                      zBOOL fValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SS_GetEditModeOnData" );
   zPCHAR         hGlobalData = 0;
   HWND           hWndCtrl = 0;
   LPSS_CELL      lpCell;
   LPSS_CELLTYPE  CellType;
   SS_CELLTYPE    CellTypeTemp;
   zPCHAR         Data;
   zCHAR          Buffer[40];
   zDECIMAL       dxValue;
   zSHORT         nLth;

   if ( lpSS->EditModeOn )
   {
      lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
      CellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                      lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( CellType->nControlID )
         hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );

      switch ( CellType->chType )
      {
         case SS_TYPE_EDIT:
         case SS_TYPE_CUSTOM:
            if ( CellType->chType == SS_TYPE_EDIT )
               nLth = CellType->Spec.Edit.nLth;
            else
               nLth = GetWindowTextLength( hWndCtrl );

            if ( hWndCtrl &&
                 DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                    nLth + 1, 1056 ) == 0 )
            {
               Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
               zmemset( Data, 0, nLth + 1 );
               GetWindowText( hWndCtrl, Data, nLth + 1 );

               if ( CellType->Spec.Edit.nChrSet == SS_CHRSET_ALPHA ||
                    CellType->Spec.Edit.nChrSet == SS_CHRSET_ALPHANUM )
               {
                  while ( *Data && *Data == ' ' )
                     Data++;
               }

               hGlobalData = Data;
//             DrUnlockTaskMemory( Data );
            }

            break;

         case SS_TYPE_DATE:
         case SS_TYPE_TIME:
         case SS_TYPE_INTEGER:
         case SS_TYPE_FLOAT:
         case SS_TYPE_PIC:
            if ( lpSS->fFormulaMode )
            {
               CellType = &lpSS->FormulaCellType;
               hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );
               if ( hWndCtrl &&
                    DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                       CellType->Spec.Edit.nLth + 1,
                                       1057 ) == 0 )
               {
                  Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                  GetWindowText( hWndCtrl, Data, CellType->Spec.Edit.nLth + 1 );
                  hGlobalData = Data;
//                DrUnlockTaskMemory( Data );
               }
            }
            else
            if ( CellType->chType == SS_TYPE_FLOAT )
            {
#ifdef SS_USEAWARE
               if ( hWndCtrl == 0 ||
                    FloatGetValue( hWndCtrl, &dxValue ) == 0 )
               {
                  *Buffer = 0;
               }
               else
               {
                  fpDoubleToString( Buffer, dxValue,
                                    CellType->Spec.Float.Right,
                                    '.', 0, FALSE,
                                    (CellType->lStyle & FS_MONEY) ? TRUE : FALSE,
                                    TRUE, FALSE, 0, 0 );
               }
#else
               if ( hWndCtrl )
                  FloatGetValue( hWndCtrl, &dxValue );
               else
                  dxValue = (ZDecimal) 0;

               SS_FloatFormat( lpSS, CellType, dxValue, Buffer, fValue );
#endif

               if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                       zstrlen( Buffer ) + 1, 1058 ) == 0 )
               {
                  Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                  strcpy_s( Data, Buffer );
                  hGlobalData = Data;
//                DrUnlockTaskMemory( Data );
               }
            }
            else
            {
               if ( hWndCtrl && CellType->chType == SS_TYPE_PIC )
                  nLth = GetWindowTextLength( hWndCtrl );
               else
                  nLth = 40;

               if ( hWndCtrl &&
                    DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                       nLth + 1, 1059 ) == 0 )
               {
                  Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                  zmemset( Data, 0, nLth + 1 );
                  GetWindowText( hWndCtrl, Data, nLth + 1 );
                  hGlobalData = Data;
//                DrUnlockTaskMemory( Data );
               }
            }

            break;

#ifndef SS_NOCT_COMBO
         case SS_TYPE_COMBOBOX:
            if ( hWndCtrl )
            {
               nLth = (zSHORT) SendMessage( hWndCtrl, WM_GETTEXTLENGTH,
                                            0, 0L );

               if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                       nLth + 1 + 5 + 1, 1060 ) == 0 )
               {
                  zPCHAR  lpItems;
                  zUSHORT k;
                  zLONG   lSel;

                  Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                  SendMessage( hWndCtrl, WM_GETTEXT, nLth + 1, (zLONG) Data );

//                lpItems = (zPCHAR)
//                        DrLockTaskMemory( CellType->Spec.ComboBox.hItems );
                  lpItems = (zPCHAR) CellType->Spec.ComboBox.hItems;

                  for ( k = 0, lSel = -1;
                        k < CellType->Spec.ComboBox.nItemCnt;
                        k++ )
                  {
                     if ( zstricmp( Data, lpItems ) == 0 )
                     {
                        lSel = k;
                        break;
                     }

                     lpItems += zstrlen( lpItems ) + 1;
                  }

                  wsprintf( &Data[ nLth + 1 ], "%d", lSel );

                  hGlobalData = Data;
//                DrUnlockTaskMemory( Data );
//                DrUnlockTaskMemory( lpItems );
               }
            }

            break;
#endif

         case SS_TYPE_BUTTON:
         {
            zPCHAR lpData;
            zBOOL fButtonState;

            if ( hWndCtrl && CellType &&
                 CellType->Spec.Button.nButtonType == SUPERBTN_TYPE_2STATE )
            {
               fButtonState = (zBOOL) SendMessage( hWndCtrl,
                                                   SBM_GETBUTTONSTATE, 0, 0L );

               if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                       1L + 1L, 1061 ) == 0 )
               {
                  lpData = (zPCHAR) DrLockTaskMemory( hGlobalData );
                  lpData[ 0 ] = (char) '0' + (char) (fButtonState ? 1 : 0);
                  hGlobalData = lpData;
//                DrUnlockTaskMemory( lpData );
               }
            }

            break;
         }

         case SS_TYPE_CHECKBOX:
         {
            zPCHAR lpData;
            WORD  wButtonState;

            if ( hWndCtrl )
               wButtonState = (WORD) SendMessage( hWndCtrl,
                                                  BM_GETCHECK, 0, 0L );
            else
               wButtonState = 0;

            if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                    1L + 1L, 1062 ) == 0 )
            {
               lpData = (zPCHAR) DrLockTaskMemory( hGlobalData );
               lpData[ 0 ] = (char) '0' + (char) wButtonState;
               hGlobalData = lpData;
//             DrUnlockTaskMemory( lpData );
            }

            break;
         }
      }

      if ( lpCell )
         SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
   }

   return( hGlobalData );
}

zPCHAR
SS_GetValue( HWND           hWnd,
             LPSPREADSHEET  lpSS,
             SS_COORD       ssCol,
             SS_COORD       ssRow )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SS_GetValue" );
   zPCHAR        hData;
   zPCHAR        hValue = 0;
   SS_CELLTYPE   CellType;
   zPCHAR        lpData;

   SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, ssRow );

   if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol && lpSS->Row.ssCurrAt == ssRow )
      hData = SS_GetEditModeOnData( hWnd, lpSS, TRUE );
   else
      hData = SS_GetData( lpSS, &CellType, ssCol, ssRow, TRUE );

   if ( hData )
   {
   // lpData = (zPCHAR) DrLockTaskMemory( hData );
      lpData = (zPCHAR) hData;
      hValue = SS_UnFormatData( lpSS, &CellType, lpData );
      DrUnlockTaskMemory( lpData );

      if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol &&
           lpSS->Row.ssCurrAt == ssRow )
      {
         hData = (zPCHAR) DrUnlockTaskMemory( lpData );
         DrFreeTaskMemory( hData );
      }
   }

   return( hValue );
}

zBOOL OPERATION
SSSetData( HWND      hWnd,
           SS_COORD  ssCol,
           SS_COORD  ssRow,
           zPCHAR    pchData )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetData" );
   return( SSSetDataRange( hWnd, ssCol, ssRow, ssCol, ssRow, pchData ) );
}

zBOOL OPERATION
SSSetDataRange( HWND      hWnd,
                SS_COORD  ssCol,
                SS_COORD  ssRow,
                SS_COORD  ssCol2,
                SS_COORD  ssRow2,
                zPCHAR    pchData )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetDataRange" );
   LPSPREADSHEET lpSS;
   zBOOL         fRet;

   lpSS = SS_Lock( hWnd );
   fRet = SS_SetDataRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2,
                           pchData, FALSE, FALSE );
// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL OPERATION
SSSetValue( HWND      hWnd,
            SS_COORD  ssCol,
            SS_COORD  ssRow,
            zPCHAR    pchData )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetValue" );
   return( SSSetValueRange( hWnd, ssCol, ssRow, ssCol, ssRow, pchData ) );
}

zBOOL OPERATION
SSSetValueRange( HWND      hWnd,
                 SS_COORD  ssCol,
                 SS_COORD  ssRow,
                 SS_COORD  ssCol2,
                 SS_COORD  ssRow2,
                 zPCHAR    pchData )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetValueRange" );
   LPSPREADSHEET lpSS;
   zBOOL         fRet;

   lpSS = SS_Lock( hWnd );
   fRet = SS_SetDataRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2,
                           pchData, TRUE, FALSE );
// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL
SS_SetDataRange( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 SS_COORD      ssCol,
                 SS_COORD      ssRow,
                 SS_COORD      ssCol2,
                 SS_COORD      ssRow2,
                 zPCHAR        pchData,
                 zBOOL         fValue,
                 zBOOL         fIgnoreLockedCells )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SS_SetDataRange" );
   zBOOL          RedrawOld;
   zBOOL          fSetVScrollBar = FALSE;
   zBOOL          fEmpty = FALSE;
   zBOOL          Ret = TRUE;
   SS_COORD       x;
   SS_COORD       y;

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );

   if ( pchData && fValue != SS_VALUE_FLOAT /* 2 */ &&
        fValue != SS_VALUE_INT /* 3 */ && *pchData == 0 )
   {
      pchData = 0;
   }

   if ( pchData == 0 || *pchData == 0 )
      fEmpty = TRUE;

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
   {
      RedrawOld = lpSS->Redraw;
      lpSS->Redraw = FALSE;
      lpSS->fNoInvalidate = TRUE;

      for ( y = ssRow; y <= ssRow2; y++ )
      {
         for ( x = ssCol; x <= ssCol2; x++ )
         {
            if ( !fIgnoreLockedCells || !SS_GetLock( hWnd, lpSS, x, y ) )
            {
               if ( !SSx_SetData( hWnd, lpSS, x, y, pchData, fValue,
                                  &fSetVScrollBar, fIgnoreLockedCells ) )
               {
                  return( FALSE );
               }
            }
         }
      }
   }
   else
   if ( fIgnoreLockedCells == 0 || !SS_GetLock( hWnd, lpSS, ssCol, ssRow ) )
   {
      if ( !SSx_SetData( hWnd, lpSS, ssCol, ssRow, pchData, fValue,
                         &fSetVScrollBar, fIgnoreLockedCells ) )
      {
         return( FALSE );
      }
   }

   if ( fEmpty )
   {
      SS_AdjustDataColCnt( lpSS, ssCol, ssCol2 );
      SS_AdjustDataRowCnt( lpSS, ssRow, ssRow2 );
   }

   if ( lpSS->fNoInvalidate )
   {
      lpSS->fNoInvalidate = FALSE;
      SS_InvalidateRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2 );
      SSSetBool( hWnd, SSB_REDRAW, RedrawOld );
   }

   return( Ret );
}

zBOOL
SSx_SetData( HWND          hWnd,
             LPSPREADSHEET lpSS,
             SS_COORD      ssCol,
             SS_COORD      ssRow,
             zPCHAR        pchData,
             zBOOL         fValue,
             zBOOL         *lpfSetVScrollBar,
             zBOOL         fIgnoreLockedCells )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSx_SetData" );
   SS_CELLTYPE CellType;
   LPSS_CELL   lpCell;
   LPSS_ROW    lpRow;
   LPSS_COL    lpCol;
   SS_COORD    ssColAt;
   SS_COORD    ssRowAt;
   ZSS_Data    SSData;
   zBOOL       bEditMode = FALSE;
   zBOOL       bRC = TRUE;
   zCHAR       fHiliteOn;
   SS_COORD    x1;
   SS_COORD    x2;
   SS_COORD    y1;
   SS_COORD    y2;
   SS_COORD    k;
   SS_COORD    j;

   if ( lpSS->EditModeOn && lpSS->Row.ssCurrAt == ssRow && lpSS->Col.ssCurrAt == ssCol )
      bEditMode = TRUE;

   lpCell = SS_LockCellItem( lpSS, ssCol, ssRow );

   SS_RetrieveCellType( lpSS, &CellType, lpCell, ssCol, ssRow );

   if ( (fValue == SS_VALUE_FLOAT && CellType.chType != SS_TYPE_FLOAT) ||
        (fValue == SS_VALUE_INT && CellType.chType != SS_TYPE_INTEGER) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( lpSS->fAllowCellOverflow )
      SS_ResetCellOverflow( hWnd, lpSS, ssCol, ssRow );

   zmemset( &SSData, 0, zsizeof( ZSS_Data ) );

   if ( pchData && !SS_FormatData( lpSS, &SSData, &CellType, pchData, fValue ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( ssRow >= lpSS->Row.nHeaderCnt && ssCol != SS_ALLCOLS &&
        ssCol >= lpSS->Col.DataCnt && SSData.m_DataType )
   {
      SS_SetDataColCnt( lpSS, ssCol + 1 );
      *lpfSetVScrollBar = TRUE;
   }

   if ( ssCol >= lpSS->Col.nHeaderCnt && ssRow != SS_ALLROWS &&
        ssRow >= lpSS->Row.DataCnt && SSData.m_DataType )
   {
      SS_SetDataRowCnt( lpSS, ssRow + 1 );
      *lpfSetVScrollBar = TRUE;
   }

   // set cell
   if ( ssCol != SS_ALLCOLS && ssRow != SS_ALLROWS )
   {
      zPCHAR hCell = 0;

      if ( pchData && lpCell == 0 &&
           (hCell = (zPCHAR) SS_AllocCell( lpSS, ssCol, ssRow )) == 0 )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }

      if ( hCell )
      {
//       lpCell = (LPSS_CELL) DrLockTaskMemory( hCell );
         lpCell = (LPSS_CELL) hCell;
      }

      if ( lpCell || (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 )
      {
         SSx_FreeData( &lpCell->m_CellData );

         if ( lpSS->fAllowCellOverflow )
            SS_ResetCellOverflow( hWnd, lpSS, ssCol, ssRow );

         zmemcpy( &lpCell->m_CellData, &SSData, zsizeof( ZSS_Data ) );

         if ( lpSS->fAllowCellOverflow )
            SS_CalcCellOverflow( hWnd, lpSS, lpCell, &CellType, ssCol, ssRow );

         SS_InvalidateCell( hWnd, lpSS, ssCol, ssRow );

         if ( lpSS->fAllowCellOverflow &&
              lpCell->m_CellData.m_DataType == 0 )
         {
            SS_OverflowAdjustNeighbor( hWnd, lpSS, ssCol, ssRow );
         }

         SS_UnlockCellItem( lpSS, ssCol, ssRow );
      }
   }
   else     // set col
   if ( ssCol != SS_ALLCOLS && ssRow == SS_ALLROWS )
   {
      if ( pchData && !SS_AllocCol( lpSS, ssCol ) )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }

      for ( k = 1; k < lpSS->Row.AllocCnt; k++ )
      {
         if ( (lpCell = SS_LockCellItem( lpSS, ssCol, k )) != 0 )
         {
            if ( !fIgnoreLockedCells || !SS_GetLock( hWnd, lpSS, ssCol, k ) )
            {
               SSx_FreeData( &lpCell->m_CellData );
               SS_ResetCellOverflow( hWnd, lpSS, ssCol, k );
            }

            SS_UnlockCellItem( lpSS, ssCol, k );
         }
      }

      if ( (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
      {
         if ( !fIgnoreLockedCells || !SS_GetLock( hWnd, lpSS, ssCol, SS_ALLROWS ) )
         {
            SSx_FreeData( &lpCol->Data );
            SS_ResetCellOverflow( hWnd, lpSS, ssCol, ssRow );

            zmemcpy( &lpCol->Data, &SSData, zsizeof( ZSS_Data ) );
         }

         SS_UnlockColItem( lpSS, ssCol );
      }

      SS_InvalidateCol( hWnd, lpSS, ssCol );
   }
   else  // set row
   if ( ssCol == SS_ALLCOLS && ssRow != SS_ALLROWS )
   {
      ::MessageBox( 0, "SetData", "AllocRow", MB_OK );
      if ( pchData && !SS_AllocRow( lpSS, ssRow ) )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }

      if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
      {
         for ( k = 1; k < lpSS->Col.AllocCnt; k++ )
         {
            if ( (lpCell = SS_LockCellItem( lpSS, k, ssRow )) != 0 )
            {
               if ( fIgnoreLockedCells == 0 ||
                    SS_GetLock( hWnd, lpSS, k, ssRow ) == 0 )
               {
                  SSx_FreeData( &lpCell->m_CellData );
                  SS_ResetCellOverflow( hWnd, lpSS, k, ssRow );
               }

               SS_LockCellItem( lpSS, k, ssRow );
            }
         }

         if ( fIgnoreLockedCells == 0 ||
              SS_GetLock( hWnd, lpSS, SS_ALLCOLS, ssRow ) == 0 )
         {
            SSx_FreeData( &lpRow->Data );
            SS_ResetCellOverflow( hWnd, lpSS, ssCol, ssRow );

            zmemcpy( &lpRow->Data, &SSData, zsizeof( ZSS_Data ) );
         }

         SS_UnlockRowItem( lpSS, ssRow );
      }

      SS_InvalidateRow( hWnd, lpSS, ssRow );
   }
   else  // set all
   {
      // Clear All ssCols, ssRows, and Cells.
      for ( k = 1; k < lpSS->Row.AllocCnt; k++ )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, k )) != 0 )
         {
            if ( !fIgnoreLockedCells || !SS_GetLock( hWnd, lpSS,
                                                     SS_ALLCOLS, k ) )
            {
               SSx_FreeData( &lpRow->Data );
               SS_ResetCellOverflow( hWnd, lpSS, ssCol, k );

               for ( j = 1; j < lpSS->Col.AllocCnt; j++ )
               {
                  if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, j, k )) != 0 )
                  {
                     if ( !fIgnoreLockedCells ||
                          !SS_GetLock( hWnd, lpSS, j, k ) )
                     {
                        SSx_FreeData( &lpCell->m_CellData );
                        SS_ResetCellOverflow( hWnd, lpSS, j, k );
                     }

                     SSx_UnlockCellItem( lpSS, lpRow, j, k );
                  }
               }
            }

            SS_UnlockRowItem( lpSS, k );
         }
      }

      for ( k = 1; k < lpSS->Col.AllocCnt; k++ )
      {
         if ( (lpCol = SS_LockColItem( lpSS, k )) != 0 )
         {
            if ( !fIgnoreLockedCells ||
                 !SS_GetLock( hWnd, lpSS, k, SS_ALLROWS ) )
            {
               SSx_FreeData( &lpCol->Data );
               SS_ResetCellOverflow( hWnd, lpSS, k, ssRow );
            }

            SS_UnlockColItem( lpSS, k );
         }
      }

      if ( !fIgnoreLockedCells ||
           !SS_GetLock( hWnd, lpSS, SS_ALLCOLS, SS_ALLROWS ) )
      {
         SSx_FreeData( &lpSS->DefaultData );
         zmemcpy( &lpSS->DefaultData, &SSData, zsizeof( ZSS_Data ) );
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
      }
   }

   if ( bRC == FALSE )
      return( FALSE );

   if ( lpSS->nCalcTableCnt && lpSS->CalcAuto &&
        (CellType.chType == SS_TYPE_FLOAT ||
         CellType.chType == SS_TYPE_INTEGER) )
   {
      if ( ssCol == -1 )
      {
         x1 = lpSS->Col.nHeaderCnt;
         x2 = lpSS->Col.DataCnt - 1;
      }
      else
      {
         x1 = ssCol;
         x2 = ssCol;
      }

      if ( ssRow == -1 )
      {
         y1 = lpSS->Row.nHeaderCnt;
         y2 = lpSS->Row.DataCnt - 1;
      }
      else
      {
         y1 = ssRow;
         y2 = ssRow;
      }

      ssColAt = lpSS->Col.ssCurrAt;
      ssRowAt = lpSS->Row.ssCurrAt;

      fHiliteOn = lpSS->HiliteOn;
      SS_HiliteCell( hWnd, FALSE );
      lpSS->FreezeHilite = TRUE;

      for ( k = x1; k <= x2; k++ )
      {
         for ( j = y1; j <= y2; j++ )
         {
            lpSS->Col.ssCurrAt = k;
            lpSS->Row.ssCurrAt = j;
            SS_CalcDependencies( hWnd, lpSS, k, j );
         }
      }

      lpSS->FreezeHilite = FALSE;

      lpSS->Col.ssCurrAt = ssColAt;
      lpSS->Row.ssCurrAt = ssRowAt;

      SS_HiliteCell( hWnd, fHiliteOn );
   }

   if ( bEditMode )
      SS_SetEditModeOnData( hWnd, lpSS );
   else
   if ( CellType.chType == SS_TYPE_CHECKBOX ||
        (CellType.chType == SS_TYPE_BUTTON &&
         CellType.Spec.Button.nButtonType == SUPERBTN_TYPE_2STATE) )
   {
      lpSS->CurCellSendingMsg.ssCol = ssCol;
      lpSS->CurCellSendingMsg.ssRow = ssRow;

      if ( pchData == 0 || pchData[ 0 ] == 0 ||
           (pchData[ 1 ] == 0 && pchData[ 0 ] == 0) )
         SS_SendMsgCommand( hWnd, 0, SSN_BUTTONUP, FALSE );
      else
         SS_SendMsgCommand( hWnd, 0, SSN_BUTTONDOWN, FALSE );
   }

   if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
      lpRow->fDataSet = TRUE;

// TraceLineI( "SetData lpRow ", (zLONG) lpRow );
// TraceLineI( "SetData ssRow ", ssRow );
// TraceLineI( "======> ssCol ", ssCol );

   return( bRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_doc.cpp

zLONG
SS_CalcCellMetrics( HDC hDC, HWND hWnd, LPSPREADSHEET lpSS,
                    LPSS_COL lpCol, LPSS_ROW lpRow, LPSS_CELL Cell,
                    LPSS_CELLTYPE CellType, LPRECT  lpRect,
                    SS_COORD CellCol, SS_COORD CellRow, zBOOL fCalcWidth,
                    zBOOL fCalcHeight, zPLONG lpJust, zPCHAR pchData )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_CalcCellMetrics" );
   zPCHAR        hGlobalData = 0;
   LPSS_CELL     lpCellOrig = Cell;
   LPSS_COL      lpColOrig = lpCol;
   LPSS_ROW      lpRowOrig = lpRow;
   HFONT         hFontOld;
   HFONT         hFont;
   RECT          rectTemp;
   RECT          rectNew;
   SS_CELLTYPE   CellTypeTemp;
   ZSS_Data      SSData;
   LPSS_FONT     Font;
   SS_FONT       FontTemp;
   SS_FONTID     FontId;
   zCHAR         Temp[10];
   zCHAR         Buffer[200];
   zPCHAR        pchDecimalData = 0;
   HWND          hWndCtrl = 0;
   zDECIMAL      dxValue;
   zLONG         lWidth = 0;
   zLONG         lHeight = 0;
   SS_COORD      Number;
   zLONG         lJust;
   zSHORT        nExtraX;
   zSHORT        nExtraY;
   zLONG         lJustRet = DT_LEFT;
   zUSHORT       k;
   SIZE          size;

   if ( lpCol == 0 && CellCol != -1 )
      lpCol = SS_LockColItem( lpSS, CellCol );

   if ( lpRow == 0 && CellRow != -1 )
      lpRow = SS_LockRowItem( lpSS, CellRow );

   if ( Cell == 0 )
      Cell = SSx_LockCellItem( lpSS, lpRow, CellCol, CellRow );

   if ( lpRect == 0 )
   {
      lpRect = &rectNew;
      lpRect->left = 0;
      lpRect->top = 0;
      lpRect->bottom = SS_GetRowHeight( lpSS, CellRow ) - 1;
      lpRect->right = SS_GetColWidth( lpSS, CellCol ) - 1;
   }

   lWidth = 0;
   lHeight = 0;

   if ( !CellType )
      CellType = SSx_RetrieveCellType( lpSS, &CellTypeTemp, lpCol, lpRow,
                                       Cell, CellCol, CellRow );

   SSData.m_DataType = 0;

   if ( pchData )
      pchDecimalData = pchData;

   else
   {
      if ( CellType->chType == SS_TYPE_FLOAT )
      {
         if ( (hGlobalData = SSx_GetData( lpSS, lpCol, lpRow,
                                          Cell, CellType, &SSData,
                                          CellCol, CellRow, FALSE )) != 0 )
         {
//          pchDecimalData = (zPCHAR) DrLockTaskMemory( hGlobalData );
            pchDecimalData = (zPCHAR) hGlobalData;
         }
         else
         if ( SSData.m_DataType == SS_TYPE_FLOAT )
            pchDecimalData = (zPCHAR) 1;
      }
      else
      {
         if ( (hGlobalData = SSx_GetData( lpSS, lpCol, lpRow,
                                          Cell, CellType, 0,
                                          CellCol, CellRow, FALSE )) != 0 )
         {
//          pchDecimalData = (zPCHAR) DrLockTaskMemory( hGlobalData );
            pchDecimalData = (zPCHAR) hGlobalData;
         }
      }

      // format Headers
      if ( CellCol >= lpSS->Col.nHeaderCnt && CellRow < lpSS->Row.nHeaderCnt &&
           (!hGlobalData || !Cell || !Cell->m_CellData.m_DataType) )
      {
         if ( CellRow == 0 )
         {
            Number = CellCol - lpSS->Col.nHeaderCnt + lpSS->Col.NumStart;

            if ( lpSS->ColHeaderDisplay == SS_HEADERDISPLAY_LETTERS )
               SS_DrawFormatHeaderLetter( Temp, Number );
            else
            if ( lpSS->ColHeaderDisplay == SS_HEADERDISPLAY_NUMBERS )
               SS_DrawFormatHeaderNumber( Temp, Number );
            else
               Temp[ 0 ] = 0;

            pchDecimalData = Temp;
         }
      }
      else
      if ( CellRow >= lpSS->Row.nHeaderCnt && CellCol < lpSS->Col.nHeaderCnt &&
           (!hGlobalData || !Cell || !Cell->m_CellData.m_DataType) )
      {
         if ( CellCol == 0 )
         {
            if ( lpSS->fVirtualMode &&
                 (lpSS->Virtual.lVStyle & SSV_NOROWNUMBERS) )
            {
               pchDecimalData = "";
            }
            else
            {
               Number = CellRow - lpSS->Row.nHeaderCnt + lpSS->Row.NumStart;

               if ( lpSS->RowHeaderDisplay == SS_HEADERDISPLAY_LETTERS )
                  SS_DrawFormatHeaderLetter( Temp, Number );
               else
               if ( lpSS->RowHeaderDisplay == SS_HEADERDISPLAY_NUMBERS )
                  SS_DrawFormatHeaderNumber( Temp, Number );
               else
                  Temp[ 0 ] = 0;

               pchDecimalData = Temp;
            }
         }
      }
      else
      if ( CellCol == 0 && CellRow == 0 &&
           (!Cell || !Cell->m_CellData.m_DataType) )
      {
         pchDecimalData = "";
      }
   }

   FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow, Cell, CellCol, CellRow );
   Font = SS_GetFont( lpSS, &FontTemp, FontId );
   if ( Font )
   {
      hFont = Font->hFont;
      hFontOld = SelectFont( hDC, hFont );
   }
   else
      hFontOld = 0;

   switch ( CellType->chType )
   {
      case SS_TYPE_STATICTEXT:
         if ( pchDecimalData )
         {
            lJust = SS_RetrieveJust( (zSHORT) CellType->lStyle );
            lJustRet = lJust;

            if ( fCalcHeight )
            {
               if ( lJust & DT_WORDBREAK )
               {
                  SetRect( &rectTemp, lpRect->left + lpSS->nMarginX,
                           lpRect->top + lpSS->nMarginY,
                           lpRect->right - lpSS->nMarginX,
                           lpRect->top + lpSS->nMarginY );

                  DrawText( hDC, pchDecimalData, -1, &rectTemp, lJust | DT_CALCRECT );
                  lHeight = rectTemp.bottom - rectTemp.top + (lpSS->nMarginY * 2);
               }
               else
               {
                  GetTextExtentPoint( hDC, pchDecimalData, zstrlen( pchDecimalData ), &size );
                  lHeight = size.cy;
               }
            }

            if ( fCalcWidth )
            {
               if ( CellRow == 0 || !(lJust & DT_WORDBREAK) )
               {
                  GetTextExtentPoint( hDC, pchDecimalData, zstrlen( pchDecimalData ), &size );
                  lWidth = size.cx + (lpSS->nMarginX * 2);
               }
               else
                  lWidth = lpRect->right - lpRect->left;
            }
         }

         break;

      case SS_TYPE_PICTURE:
         hWndCtrl = SS_RetrieveControlhWnd( hWnd, lpSS, CellCol, CellRow );
         if ( fCalcWidth )
            lWidth = lpRect->right - lpRect->left;

         if ( fCalcHeight )
            lHeight = lpRect->bottom - lpRect->top;

         break;

      case SS_TYPE_BUTTON:
      case SS_TYPE_CHECKBOX:
         if ( fCalcWidth )
            lWidth = lpRect->right - lpRect->left;

         if ( fCalcHeight )
            lHeight = lpRect->bottom - lpRect->top;

         break;

      case SS_TYPE_COMBOBOX:
         if ( pchDecimalData )
         {
            if ( fCalcHeight )
            {
               GetTextExtentPoint( hDC, pchDecimalData, zstrlen( pchDecimalData ), &size );
               lHeight = size.cy + (2 * lpSS->nMarginY);
            }

            if ( fCalcWidth )
            {
               GetTextExtentPoint( hDC, pchDecimalData, zstrlen( pchDecimalData ), &size );
               lWidth = size.cx + lpSS->nMarginX;
            }
         }

         if ( fCalcWidth )
            lWidth += lpSS->nComboButtonBitmapWidth;

         break;

      case SS_TYPE_EDIT:
      case SS_TYPE_PIC:
      case SS_TYPE_DATE:
      case SS_TYPE_TIME:
      case SS_TYPE_INTEGER:
      case SS_TYPE_FLOAT:
         if ( (CellType->chType == SS_TYPE_EDIT &&
               (CellType->lStyle & ES_RIGHT)) ||
              CellType->chType == SS_TYPE_INTEGER ||
              CellType->chType == SS_TYPE_FLOAT )
         {
            lJustRet = DT_RIGHT;
         }
         else
         if ( CellType->lStyle & ES_CENTER )
            lJustRet = DT_CENTER;

         if ( CellType->chType == SS_TYPE_EDIT &&
              ((CellType->lStyle & ES_MULTILINE) ||
               (lpSS->fPrintExpandMultiLine && lpSS->fPrintingInProgress)) )
         {
            lpRect->left += 2;
            lpRect->top += 1;
            lpRect->right -= 1;
            lpRect->bottom -= 1;

            nExtraX = 4;
            nExtraY = 3;
         }
         else
         {
            lpRect->right -= lpSS->nMarginX;
            lpRect->left += lpSS->nMarginX;
            lpRect->top += lpSS->nMarginY;

            nExtraX = (2 * lpSS->nMarginX);
            nExtraY = (2 * lpSS->nMarginY);
         }

         if ( SSData.m_DataType == SS_TYPE_FLOAT )
         {
            zmemset( Buffer, 0, zsizeof( Buffer ) );
            SS_FloatFormat( lpSS, CellType, SSData.m_Data.dxValue,
                            Buffer, FALSE );
            pchDecimalData = Buffer;
         }
         else
         if ( CellType->chType == SS_TYPE_FLOAT )
         {
            if ( pchDecimalData && *pchDecimalData )
            {
               zmemset( Buffer, 0, zsizeof( Buffer ) );
               dxValue = ZDecimal( pchDecimalData );
            // StringToFloat( pchDecimalData, &dxValue ),
               SS_FloatFormat( lpSS, CellType, dxValue, Buffer, FALSE );
               pchDecimalData = Buffer;
            }
         }
         else
         if ( CellType->chType == SS_TYPE_EDIT &&
              (CellType->lStyle & ES_PASSWORD) )
         {
            for ( k = 0; k < zstrlen( pchDecimalData ); k++ )
               Buffer[ k ] = '*';

            Buffer[ zstrlen( pchDecimalData ) ] = 0;
            pchDecimalData = Buffer;
         }

         if ( pchDecimalData )
         {
            if ( fCalcHeight )
            {
               if ( CellType->chType == SS_TYPE_EDIT &&
                    ((CellType->lStyle & ES_MULTILINE) ||
                     (lpSS->fPrintExpandMultiLine &&
                      lpSS->fPrintingInProgress)) )
               {
                  CopyRect( &rectTemp, lpRect );
                  DrawText( hDC, pchDecimalData, -1, &rectTemp, DT_LEFT | DT_WORDBREAK |
                            DT_CALCRECT | DT_NOPREFIX );
                  lHeight = rectTemp.bottom - rectTemp.top;
               }
               else
               {
                  GetTextExtentPoint( hDC, pchDecimalData, zstrlen( pchDecimalData ), &size );
                  lHeight = size.cy;
               }

               lHeight += nExtraY;
            }

            if ( fCalcWidth )
            {
               GetTextExtentPoint( hDC, pchDecimalData, zstrlen( pchDecimalData ), &size );
               lWidth = size.cx + nExtraX;

               if ( CellType->chType == SS_TYPE_EDIT &&
                    ((CellType->lStyle & ES_MULTILINE) ||
                     (lpSS->fPrintExpandMultiLine &&
                      lpSS->fPrintingInProgress)) )
               {
                  lWidth = mMin( lWidth, lpRect->right - lpRect->left );
               }
            }
         }

         break;
   }

   if ( hFontOld )
      SelectFont( hDC, hFontOld );

// if ( hGlobalData )
//    DrUnlockTaskMemory( pchDecimalData );

   if ( lpCellOrig == 0 )
      SSx_UnlockCellItem( lpSS, lpRow, CellCol, CellRow );

   if ( lpColOrig == 0 )
      SS_UnlockColItem( lpSS, CellCol );

   if ( lpRowOrig == 0 )
      SS_UnlockRowItem( lpSS, CellRow );

   if ( lpJust )
      *lpJust = lJustRet;

   return( MAKELONG( lWidth, lHeight ) );
}

zBOOL OPERATION
SS_ClipboardCopy( HWND  hWnd,
                  zBOOL fCut )
{
#if 0
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_ClipboardCopy" );
   LPSPREADSHEET lpSS;
   zPCHAR        hClip;
   zPCHAR        lpstrTemp;
   SS_CELLCOORD  CellUL;
   SS_CELLCOORD  CellLR;
   SS_COORD      ssCol;
   SS_COORD      ssRow;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( SS_USESINGLESELBAR( lpSS ) )
   {
      if ( SSGetActiveCell( hWnd, &Col, &Row ) )
         hClip = SSClipOut( hWnd, 1, ssRow, lpSS->Col.AllocCnt - 1, ssRow );
   }
   else
   if ( SSGetSelectBlock( hWnd, &CellUL, &CellLR ) )
   {
      hClip = SSClipOut( hWnd, CellUL.ssCol, CellUL.ssRow,
                         CellLR.ssCol, CellLR.ssRow );
   }
   else
   if ( lpSS->wOpMode == SS_OPMODE_READONLY )
   {
//    DrUnlockTaskMemory( lpSS );
      return( 0 );
   }
   else
   if ( SSGetActiveCell( hWnd, &Col, &Row ) )
   {
      CellUL.ssCol = ssCol;
      CellUL.ssRow = ssRow;
      CellLR.ssCol = ssCol;
      CellLR.ssRow = ssRow;

      if ( DrAllocTaskMemory( (zCOREMEM) &hClip,
                              SSGetDataLth( hWnd, ssCol, ssRow ) + 1,
                              1063 ) == 0 )
      {
         lpstrTemp = (zPCHAR) DrLockTaskMemory( hClip );
         SSGetData( hWnd, ssCol, ssRow, lpstrTemp );
leak?    DrUnlockTaskMemory( lpstrTemp );
      }
   }

   if ( hClip && OpenClipboard( hWnd ) )
   {
      EmptyClipboard( );
      SetClipboardData( CF_TEXT, hClip );
      CloseClipboard( );
      fRet = TRUE;

      if ( fCut && !SS_USESINGLESELBAR( lpSS ) )
      {
         SS_AdjustCellRangeCoords( lpSS, &CellUL.ssCol, &CellUL.ssRow,
                                   &CellLR.ssCol, &CellLR.ssRow );
         SS_ClearDataRange( hWnd, CellUL.ssCol, CellUL.ssRow,
                            CellLR.ssCol, CellLR.ssRow, TRUE, TRUE );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
#else
   MESSAGE_DEBUG( "<SS_DOC.CPP>", "SS_ClipboardCopy" );
   return( FALSE );
#endif
}

void
SS_ClearColorItem( LPSS_COLORITEM lpColor,
                   COLORREF  Background,
                   COLORREF  Foreground )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_ClearColorItem" );
   if ( Background != RGBCOLOR_DEFAULT )
      lpColor->BackgroundId = 0;

   if ( Foreground != RGBCOLOR_DEFAULT )
      lpColor->ForegroundId = 0;
}

zBOOL OPERATION
SSClipboardCopy( HWND hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSClipboardCopy" );
   return( SS_ClipboardCopy( hWnd, FALSE ) );
}

zBOOL OPERATION
SSClipboardCut( HWND hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSClipboardCut" );
   return( SS_ClipboardCopy( hWnd, TRUE ) );
}

zBOOL OPERATION
SSClipboardPaste( HWND hWnd )
{
#if 0
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSClipboardPaste" );
   LPSPREADSHEET lpSS;
   zPCHAR        hClip;
   zPCHAR        lpStrOut;
   SS_CELLCOORD CellUL;
   SS_CELLCOORD CellLR;
   SS_CELLCOORD CellLRVal;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( !SS_USESINGLESELBAR( lpSS ) )
   {
      if ( (lpSS->wOpMode != SS_OPMODE_READONLY ||
            SSGetSelectBlock( hWnd, &CellUL, &CellLR )) &&
           OpenClipboard( hWnd ) )
      {
         if ( IsClipboardFormatAvailable( CF_TEXT ) ||
              IsClipboardFormatAvailable( CF_OEMTEXT ) )
         {
            hClip = GetClipboardData( CF_TEXT );

            if ( hClip )
            {
leak?          if ( (lpStrOut = (zPCHAR) DrLockTaskMemory( hClip )) != 0 )
               {
                  if ( !SSGetSelectBlock( hWnd, &CellUL, &CellLR ) )
                  {
                     SSGetActiveCell( hWnd, &CellUL.ssCol, &CellUL.ssRow );
                     CellLR.ssCol = lpSS->Col.Max;
                     CellLR.ssRow = lpSS->Row.Max;
                     zmemcpy( &CellLRVal, &CellLR, zsizeof( SS_CELLCOORD ) );
                  }
                  else
                     zmemcpy( &CellLRVal, &CellLR, zsizeof( SS_CELLCOORD ) );

                  if ( SS_ValidatePasteData( hWnd, lpSS, lpStrOut, &CellUL,
                                             &CellLRVal, FALSE ) )
                  {
                     fRet = SS_ClipIn( hWnd, CellUL.ssCol, CellUL.ssRow,
                                       CellLRVal.ssCol, CellLRVal.ssRow, lpStrOut,
                                       zstrlen( lpStrOut ), FALSE, TRUE );
                  }

                  DrUnlockTaskMemory( lpStrOut );
               }
            }
         }

         CloseClipboard( );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
#else
   MESSAGE_DEBUG( "<SS_DOC.CPP>", "SSClipboardPaste" );
   return( FALSE );
#endif
}

zBOOL OPERATION
SSGetActiveCell( HWND          hWnd,
                 LPSS_COORD    CellCol,
                 LPSS_COORD    CellRow )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetActiveCell" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( CellRow )
   {
      if ( lpSS->Row.ssCurrAt < lpSS->Row.nHeaderCnt )
         *CellRow = SS_HEADER - lpSS->Row.ssCurrAt;
      else
         *CellRow = lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1;
   }

   if ( CellCol )
   {
      if ( lpSS->Col.ssCurrAt < lpSS->Col.nHeaderCnt )
         *CellCol = SS_HEADER - lpSS->Col.ssCurrAt;
      else
         *CellCol = lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1;
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL OPERATION
SSGetDocVisible( HWND hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetDocVisible" );
   return( mIs_hWndVisible( hWnd ) );
}

zBOOL OPERATION
SSGetSelectBlock( HWND           hWnd,
                  LPSS_CELLCOORD CellUL,
                  LPSS_CELLCOORD CellLR )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetSelectBlock" );
   LPSPREADSHEET  lpSS;

   lpSS = SS_Lock( hWnd );

   if ( CellUL )
   {
      CellUL->ssCol = -1;
      CellUL->ssRow = -1;
   }

   if ( CellLR )
   {
      CellLR->ssCol = -1;
      CellLR->ssRow = -1;
   }

   if ( (lpSS->BlockCellUL.ssCol == -1 && lpSS->BlockCellUL.ssRow == -1) ||
        (lpSS->BlockCellUL.ssCol == lpSS->BlockCellLR.ssCol &&
         lpSS->BlockCellUL.ssRow == lpSS->BlockCellLR.ssRow &&
         lpSS->BlockCellUL.ssRow != -1 && lpSS->BlockCellUL.ssCol != -1) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( CellUL )
   {
      CellUL->ssRow = mMax( lpSS->BlockCellUL.ssRow - lpSS->Row.nHeaderCnt + 1, -1 );
      CellUL->ssCol = mMax( lpSS->BlockCellUL.ssCol - lpSS->Col.nHeaderCnt + 1, -1 );
   }

   if ( CellLR )
   {
      CellLR->ssRow = mMax( lpSS->BlockCellLR.ssRow - lpSS->Row.nHeaderCnt + 1, -1 );
      CellLR->ssCol = mMax( lpSS->BlockCellLR.ssCol - lpSS->Col.nHeaderCnt + 1, -1 );
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void OPERATION
SSSetDocLock( HWND  hWnd,
              zBOOL Lock )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetDocLock" );
   LPSPREADSHEET lpSS;
   LPSS_CELL     lpCell;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_COORD      k;
   SS_COORD      j;

   lpSS = SS_Lock( hWnd );

   // Clear All ssCols, ssRows, and Cells.
   for ( k = 1; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, k )) != 0 )
      {
         lpRow->fRowLocked = SS_LOCKED_DEF;

         for ( j = 1; j < lpSS->Col.AllocCnt; j++ )
            if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, j, k )) != 0 )
               lpCell->m_CellLocked = SS_LOCKED_DEF;
      }
   }

   for ( k = 1; k < lpSS->Col.AllocCnt; k++ )
   {
      if ( (lpCol = SS_LockColItem( lpSS, k )) != 0 )
         lpCol->chColLocked = SS_LOCKED_DEF;
   }

   lpSS->DocumentLocked = Lock;

   if ( lpSS->LockColor.ForegroundId || lpSS->LockColor.BackgroundId )
      SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

// DrUnlockTaskMemory( lpSS );
}

zBOOL OPERATION
SSSetActiveCell( HWND          hWnd,
                 SS_COORD      CellCol,
                 SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetActiveCell" );
   LPSPREADSHEET lpSS;
   SS_COORD      ssColOld;
   SS_COORD      ssRowOld;
   zBOOL         fRet;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, &CellCol, &CellRow );

   if ( CellCol == -1 || CellCol >= SS_GetColsMax( lpSS ) ||
        CellRow == -1 || CellRow >= SS_GetRowsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   lpSS->fSetActiveCellCalled = TRUE;

   ssColOld = lpSS->Col.ssCurrAt;
   ssRowOld = lpSS->Row.ssCurrAt;

   if ( lpSS->fSuspendInvert == 0 )
      SS_ResetBlock( hWnd, TRUE );

   fRet = SS_ActivateCell( hWnd, CellCol, CellRow, TRUE );

#ifndef SS_NOBUTTONDRAWMODE
   if ( lpSS->wButtonDrawMode )
      SS_InvalidateButtons( hWnd, lpSS, ssColOld, lpSS->Col.UL, ssRowOld,
                            lpSS->Row.UL );
#endif

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

void OPERATION
SS_SetDocColor( HWND          hWnd,
                LPSPREADSHEET lpSS,
                COLORREF      Background,
                COLORREF      Foreground )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_SetDocColor" );
   LPSS_CELL     lpCell;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_COORD      ss;
   SS_COORD      j;

   // Clear All ssCols, ssRows, and Cells.
   for ( ss = 1; ss < lpSS->Row.AllocCnt; ss++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
      {
         zmemset( &lpRow->Color, 0, zsizeof( SS_COLORITEM ) );
         SS_ClearColorItem( &lpRow->Color, Background, Foreground );

         for ( j = 1; j < lpSS->Col.AllocCnt; j++ )
         {
            if ( (lpCell = SS_LockCellItem( lpSS, j, ss )) != 0 )
               SS_ClearColorItem( &lpCell->m_Color, Background, Foreground );
         }
      }
   }

   for ( ss = 1; ss < lpSS->Col.AllocCnt; ss++ )
   {
      if ( (lpCol = SS_LockColItem( lpSS, ss )) != 0 )
         SS_ClearColorItem( &lpCol->Color, Background, Foreground );
   }

   if ( Background != RGBCOLOR_DEFAULT )
      lpSS->Color.BackgroundId  = SS_AddColor( lpSS, Background );

   if ( Foreground != RGBCOLOR_DEFAULT )
      lpSS->Color.ForegroundId  = SS_AddColor( lpSS, Foreground );

   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
}

zBOOL OPERATION
SSSetDocDefaultCellType( HWND hWnd,
                         LPSPREADSHEET lpSS,
                         LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetDocDefaultCellType" );
   LPSS_CELL lpCell;
   LPSS_COL  lpCol;
   LPSS_ROW  lpRow;
   SS_COORD  ss;
   SS_COORD  j;

   // Clear All ssCols, ssRows, and Cells.
   for ( ss = 1; ss < lpSS->Row.AllocCnt; ss++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
      {
         SS_DeAllocCellType( hWnd, lpSS, lpRow->hCellType );
         lpRow->hCellType = 0;

         for ( j = 1; j < lpSS->Col.AllocCnt; j++ )
         {
            if ( (lpCell = SS_LockCellItem( lpSS, j, ss )) != 0 )
            {
               SS_DeAllocCellType( hWnd, lpSS, lpCell->m_hCellType );
               lpCell->m_hCellType = 0;
            }
         }
      }
   }

   for ( ss = 1; ss < lpSS->Col.AllocCnt; ss++ )
   {
      if ( (lpCol = SS_LockColItem( lpSS, ss )) != 0 )
      {
         SS_DeAllocCellType( hWnd, lpSS, lpCol->hCellType );
         lpCol->hCellType = 0;
      }
   }

   zmemcpy( &lpSS->DefaultCellType, CellType, zsizeof( SS_CELLTYPE ) );
   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

   return( TRUE );
}

zBOOL OPERATION
SSSetDocDefaultFont( HWND          hWnd,
                     LPSPREADSHEET lpSS,
                     HFONT         hFont,
                     zBOOL         DeleteFont,
                     zBOOL         *lpfDeleteFontObject )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetDocDefaultFont" );
   LPSS_CELL     lpCell;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_COORD      k;
   SS_COORD      j;

   lpSS->DefaultFontId = SS_InitFont( hWnd, hFont, DeleteFont, TRUE,
                                      lpfDeleteFontObject );

   if ( lpSS->DefaultFontIdOrig == 0 )
      lpSS->DefaultFontIdOrig = lpSS->DefaultFontId;

   lpSS->Row.nCellSizeInPixels = SS_CalcRowHeight( lpSS, -1 );

   // Clear All ssCols, ssRows, and Cells.
   for ( k = 1; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, k )) != 0 )
      {
         lpRow->FontId = SS_FONT_NONE;
         if ( lpRow->lRowHeightX100 != SS_WIDTH_DEFAULT )
            lpRow->nRowHeightInPixels = SS_CalcRowHeight( lpSS, k );
         else
            lpRow->nRowHeightInPixels = SS_WIDTH_DEFAULT;

         for ( j = 1; j < lpSS->Col.AllocCnt; j++ )
            if ( (lpCell = SS_LockCellItem( lpSS, j, k )) != 0 )
               lpCell->m_FontId = SS_FONT_NONE;
      }
   }

   for ( k = 1; k < lpSS->Col.AllocCnt; k++ )
      if ( (lpCol = SS_LockColItem( lpSS, k )) != 0 )
         lpCol->FontId = SS_FONT_NONE;

   if ( lpSS->fAllowCellOverflow )
      for ( k = lpSS->Row.nHeaderCnt; k < lpSS->Row.DataCnt; k++ )
         for ( j = lpSS->Col.nHeaderCnt; j < lpSS->Col.DataCnt; j++ )
            SS_OverflowAdjustNeighbor( hWnd, lpSS, j, k );

   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
   return( TRUE );
}

void OPERATION
SSSetMaxCols( HWND          hWnd,
              SS_COORD      MaxCols )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetMaxCols" );
   LPSPREADSHEET lpSS;
   SS_COORD      ssColLeft;
   SS_COORD      ssColULOld;
   SS_COORD      MaxColsOld;

   lpSS = SS_Lock( hWnd );

   if ( MaxCols < 0 )
      MaxCols = SS_MAXCOLS;
   else
      MaxCols = mMin( MaxCols, SS_MAXCOLS );

   if ( lpSS->Col.ssCurrAt < lpSS->Col.nHeaderCnt )
      lpSS->Col.ssCurrAt = mMin( lpSS->Col.nHeaderCnt, MaxCols );

   if ( lpSS->Col.UL < lpSS->Col.nHeaderCnt )
      lpSS->Col.UL = mMin( lpSS->Col.nHeaderCnt, MaxCols );

   if ( lpSS->Col.AllocCnt < lpSS->Col.nHeaderCnt )
      lpSS->Col.AllocCnt = mMin( lpSS->Col.nHeaderCnt, MaxCols + 1 );

   if ( lpSS->Col.DataCnt < lpSS->Col.nHeaderCnt )
      lpSS->Col.DataCnt = mMin( lpSS->Col.nHeaderCnt, MaxCols + 1 );

   if ( MaxCols < lpSS->Col.AllocCnt - 1 )
   {
      SSClearRange( hWnd, MaxCols + 1, SS_ALLROWS,
                    lpSS->Col.AllocCnt - 1, SS_ALLROWS );

      lpSS->Col.AllocCnt = mMin( lpSS->Col.AllocCnt, MaxCols + 1 );
      lpSS->Col.DataCnt = mMin( lpSS->Col.DataCnt, MaxCols + 1 );
   }

   MaxColsOld = lpSS->Col.Max;
   ssColULOld = lpSS->Col.UL;
   lpSS->Col.Max = MaxCols;

   lpSS->Col.ssCurrAt = mMin( lpSS->Col.ssCurrAt, lpSS->Col.Max );
   lpSS->Col.UL = mMin( lpSS->Col.UL, lpSS->Col.Max );

   if ( lpSS->Col.UL > (ssColLeft = SS_GetLastPageLeftCol( hWnd, lpSS )) )
      lpSS->Col.UL = ssColLeft;

   if ( lpSS->Col.UL < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
      lpSS->Col.UL = mMin( MaxCols, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );

   if ( ssColULOld != lpSS->Col.UL )
      InvalidateRect( hWnd, 0, TRUE );

   if ( SS_IsBlockSelected( lpSS ) )
   {
      if ( lpSS->BlockCellUL.ssCol >= SS_GetColsMax( lpSS ) )
      {
         lpSS->BlockCellUL.ssRow = -1;
         lpSS->BlockCellUL.ssCol = -1;

         lpSS->BlockCellLR.ssRow = -1;
         lpSS->BlockCellLR.ssCol = -1;
      }
      else
      if ( lpSS->BlockCellLR.ssCol >= SS_GetColsMax( lpSS ) )
         lpSS->BlockCellLR.ssCol = SS_GetColsMax( lpSS ) - 1;
  }

   SSBuildDependencies( hWnd );

   SS_AutoSize( hWnd, FALSE );
   SS_InvalidateColRange( hWnd, lpSS, mMin( lpSS->Col.Max + 1, MaxColsOld ),
                          mMax( lpSS->Col.Max, MaxColsOld ) );

// DrUnlockTaskMemory( lpSS );
}

void OPERATION
SSSetMaxRows( HWND          hWnd,
              SS_COORD      MaxRows )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetMaxRows" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->fVirtualMode )
      lpSS->Virtual.VMinVisibleCnt = MaxRows;
   else
      SS_SetMaxRows( hWnd, MaxRows );

// DrUnlockTaskMemory( lpSS );
}

void
SS_SetMaxRows( HWND     hWnd,
               SS_COORD MaxRows )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_SetMaxRows" );
   LPSPREADSHEET lpSS;
   SS_COORD      ssRowTop;
   SS_COORD      ssRowULOld;
   SS_COORD      MaxRowsOld;

   lpSS = SS_Lock( hWnd );

   if ( MaxRows < 0 )
      MaxRows = SS_MAXROWS;
   else
      MaxRows = mMin( MaxRows, SS_MAXROWS );

   if ( lpSS->Row.ssCurrAt < lpSS->Row.nHeaderCnt )
      lpSS->Row.ssCurrAt = mMin( lpSS->Row.nHeaderCnt, MaxRows );

   if ( lpSS->Row.UL < lpSS->Row.nHeaderCnt )
      lpSS->Row.UL = mMin( lpSS->Row.nHeaderCnt, MaxRows );

   if ( lpSS->Row.AllocCnt < lpSS->Row.nHeaderCnt )
      lpSS->Row.AllocCnt = mMin( lpSS->Row.nHeaderCnt, MaxRows + 1 );

   if ( lpSS->Row.DataCnt < lpSS->Row.nHeaderCnt )
      lpSS->Row.DataCnt = mMin( lpSS->Row.nHeaderCnt, MaxRows + 1 );

   if ( MaxRows < lpSS->Row.AllocCnt - 1 )
   {
      SSClearRange( hWnd, SS_ALLCOLS, MaxRows + 1,
                    SS_ALLCOLS, lpSS->Row.AllocCnt - 1 );

      lpSS->Row.AllocCnt = mMin( lpSS->Row.AllocCnt, MaxRows + 1 );
      lpSS->Row.DataCnt = mMin( lpSS->Row.DataCnt, MaxRows + 1 );
   }

   MaxRowsOld = lpSS->Row.Max;
   ssRowULOld = lpSS->Row.UL;
   lpSS->Row.Max = MaxRows;

   lpSS->Row.ssCurrAt = mMin( lpSS->Row.ssCurrAt, lpSS->Row.Max );
   lpSS->Row.UL = mMin( lpSS->Row.UL, lpSS->Row.Max );

   if ( lpSS->Row.UL > (ssRowTop = SS_GetLastPageTopRow( hWnd, lpSS )) )
      lpSS->Row.UL = ssRowTop;

   if ( lpSS->Row.UL < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
      lpSS->Row.UL = mMin( MaxRows, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );

   if ( ssRowULOld != lpSS->Row.UL )
      InvalidateRect( hWnd, 0, TRUE );

   if ( SS_IsBlockSelected( lpSS ) )
   {
      if ( lpSS->BlockCellUL.ssRow >= SS_GetRowsMax( lpSS ) )
      {
         lpSS->BlockCellUL.ssRow = -1;
         lpSS->BlockCellUL.ssCol = -1;

         lpSS->BlockCellLR.ssRow = -1;
         lpSS->BlockCellLR.ssCol = -1;
      }
      else
      if ( lpSS->BlockCellLR.ssRow >= SS_GetRowsMax( lpSS ) )
         lpSS->BlockCellLR.ssRow = SS_GetRowsMax( lpSS ) - 1;
   }

   SSBuildDependencies( hWnd );
   SS_AutoSize( hWnd, FALSE );
   SS_InvalidateRowRange( hWnd, lpSS, mMin( lpSS->Row.Max + 1, MaxRowsOld ),
                          mMax( lpSS->Row.Max, MaxRowsOld ) );
// DrUnlockTaskMemory( lpSS );
}

zBOOL OPERATION
SSSetSelectBlock( HWND           hWnd,
                  LPSS_CELLCOORD CellUL,
                  LPSS_CELLCOORD CellLR )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetSelectBlock" );
   LPSPREADSHEET  lpSS;

   lpSS = SS_Lock( hWnd );
   SS_ResetBlock( hWnd, TRUE );

   if ( CellUL->ssRow == SS_HEADER )
      CellUL->ssRow = -1;

   if ( CellUL->ssCol == SS_HEADER )
      CellUL->ssCol = -1;

   if ( CellLR->ssRow == SS_HEADER )
      CellLR->ssRow = -1;

   if ( CellLR->ssCol == SS_HEADER )
      CellLR->ssCol = -1;

   if ( CellUL->ssRow == -1 )
      lpSS->BlockCellUL.ssRow = 0;
   else
      lpSS->BlockCellUL.ssRow = CellUL->ssRow + lpSS->Row.nHeaderCnt - 1;

   if ( CellUL->ssCol == -1 )
      lpSS->BlockCellUL.ssCol = 0;
   else
      lpSS->BlockCellUL.ssCol = CellUL->ssCol + lpSS->Col.nHeaderCnt - 1;

   if ( CellLR->ssRow == -1 )
      lpSS->BlockCellLR.ssRow = -1;
   else
      lpSS->BlockCellLR.ssRow = CellLR->ssRow + lpSS->Row.nHeaderCnt - 1;

   if ( CellLR->ssCol == -1 )
      lpSS->BlockCellLR.ssCol = -1;
   else
      lpSS->BlockCellLR.ssCol = CellLR->ssCol + lpSS->Col.nHeaderCnt - 1;

   lpSS->Col.BlockCellCurrentPos = lpSS->BlockCellLR.ssCol;
   lpSS->Row.BlockCellCurrentPos = lpSS->BlockCellLR.ssRow;

   lpSS->fSuspendInvert = TRUE;
   SSSetActiveCell( hWnd, mMax( lpSS->Col.nHeaderCnt, CellUL->ssCol ),
                    mMax( lpSS->Row.nHeaderCnt, CellUL->ssRow ) );
   lpSS->fSuspendInvert = FALSE;

   if ( lpSS->wMessageBeingSent &&
        (lpSS->wMsgLast == WM_KEYDOWN || lpSS->wMsgLast == WM_LBUTTONDOWN) )
   {
      lpSS->fSuspendInvert = TRUE;
      SS_HiliteCell( hWnd, FALSE );
      lpSS->fInvertBlock = TRUE;
   }
   else
   if ( (GetFocus( ) == hWnd || lpSS->fRetainSelBlock) )
      SS_InvertBlock( hWnd, 0, lpSS );

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void
SS_Undo( HWND hWnd,
         LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_Undo" );
}

zBOOL
SS_ValidatePasteData( HWND hWnd, LPSPREADSHEET lpSS, zPCHAR lpszText,
                      LPSS_CELLCOORD lpCellUL, LPSS_CELLCOORD lpCellLR,
                      zBOOL fValidate )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_ValidatePasteData" );
   zBOOL         fRet = FALSE;

   if ( lpSS->fAutoClipboard == 0 || lpSS->fValidateClipboardPaste == 0 )
      return( TRUE );

   return( TRUE );
}

void
SS_VBDelBlock( HWND hWnd,
               LPSPREADSHEET lpSS,
               WORD  wInsDelOperation )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_VBDelBlock" );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif
