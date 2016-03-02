/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlss.cpp
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
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.01.02    DKS   Z10       QS999
//    Grid does a correct CheckExistenceOfEntity to determine if it
//    should be disabled (for lack of data).
//
// 2000.03.17    TMV   WEB
//    Validate parent window's handle prior to call Create/CreateEx
//
// 1999.12.03    DKS   Z10       QS999
//    SpreadSheet now protects against invalid views properly.
//
// 1999.11.01    DKS   Z10    FH999
//    SetLock holds across MapFromOI.
//
// 1999.10.19    DKS   Z10    QS999
//    Disabled NOINTEGRALHEIGHT flag.
//
// 1999.09.22    DKS   Z10    QS999
//    Fixed text color problem and started memory leak analyis.
//
// 1999.09.15    DKS   Z10    ActiveX
//    Added Mapping List.
//
// 1999.08.27    DKS   Z10    QS999
//    Protect against dropping incorrect local list view.
//
// 1999.06.29    DKS   Z10    TB568
//    Permit change of header text.
//
// 1999.05.11    DKS   Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.02.03    DKS
//    Fix to THUMBTRACK message when one or two lines in SS (Helmut bug).
//
// 1999.01.04    DKS
//    Drop view in dtor now verifies that the view is valid.
//
// 1998.12.22    DKS
//    Hi-bit not set fix for Bianca.
//
// 1998.12.15    DKS
//    Fix to support LockColumn operations.
//
// 1998.10.30    DKS
//    Changed background color.  Fixed edit leave/enter cell processing.
//
// 1998.09.16    DKS
//    Fix to permit delete of all rows.
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define zGLOBAL_DATA
#include <kzoegdef.h>

/////////////////////////////////////////////////////////////////////////////
// SpreadSheet globals
//

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

#ifdef GRIDCONTROL_NO_CLIPBOARD
#pragma message(" -- ZGrid: No clipboard support")
#else
#pragma message(" -- ZGrid: Clipboard support enabled for Grid")
#endif

IMPLEMENT_DYNAMIC( ZGrid, CWnd )

BEGIN_MESSAGE_MAP( ZGrid, CWnd )
//EFW - Added ON_WM_RBUTTONUP
//{{AFX_MSG_MAP( ZGrid )
   ON_WM_CREATE( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )

   ON_WM_PAINT( )
   ON_WM_HSCROLL( )
   ON_WM_VSCROLL( )
   ON_WM_SIZE( )
   ON_WM_RBUTTONUP( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_MOUSEMOVE( )
   ON_WM_TIMER( )
   ON_WM_GETDLGCODE( )
   ON_WM_SYSKEYDOWN( )
   ON_WM_KEYDOWN( )
   ON_WM_CHAR( )
   ON_WM_ERASEBKGND( )

   ON_UPDATE_COMMAND_UI( ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll )

   ON_COMMAND( ID_EDIT_SELECT_ALL, OnEditSelectAll )
   ON_MESSAGE( zZEIDON_CTRL_MSG + 1, OnPostedClickDown )

#ifndef _WIN32_WCE_NO_CURSOR
   ON_WM_SETCURSOR( )
#endif
#ifndef _WIN32_WCE
   ON_WM_RBUTTONUP( )
   ON_WM_SYSCOLORCHANGE( )
   ON_WM_CAPTURECHANGED( )
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
   ON_COMMAND( ID_EDIT_COPY, OnEditCopy )
   ON_UPDATE_COMMAND_UI( ID_EDIT_COPY, OnUpdateEditCopy )
   ON_COMMAND( ID_EDIT_CUT, OnEditCut )
   ON_UPDATE_COMMAND_UI( ID_EDIT_CUT, OnUpdateEditCut )
   ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )
   ON_UPDATE_COMMAND_UI( ID_EDIT_PASTE, OnUpdateEditPaste )
#endif
#if ( _WIN32_WCE >= 210 )
   ON_WM_SETTINGCHANGE( )
#endif
#if !defined( _WIN32_WCE ) && ( _MFC_VER >= 0x0421 )
   ON_WM_MOUSEWHEEL( )
#endif
   ON_MESSAGE( WM_SETFONT, OnSetFont )
   ON_MESSAGE( WM_GETFONT, OnGetFont )
   ON_MESSAGE( WM_IME_CHAR, OnImeChar )
   ON_NOTIFY( GVN_ENDEDIT, IDC_INPLACE_CONTROL, OnEndInPlaceEdit )
//}}AFX_MSG_MAP
END_MESSAGE_MAP( )

#define m_pzsScope  m_pzsAName // AN was stolen for the scoping entity name

/////////////////////////////////////////////////////////////////////////////
// ZGrid
// ZGrid - ctor
ZGrid::ZGrid( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
       CWnd( ),
       ZMapAct( pZSubtask,            // base class ctor
                pzmaComposite,
                pWndParent,
                this,
                vDialog,
                nOffsetX,
                nOffsetY,
                pCtrlDef,
                "Grid" ),
       m_csViewName( "_" ),
       m_csViewNameList( "__" ),
       m_wndTitleTip( m_pZSubtask->m_pZTask->m_lHoverDelay )
{
   zPCHAR pch;
   zPCHAR pchComma;

#ifdef DEBUG_ALL
   if ( m_pzsVName )
   {
      TraceLineS( "SS::ctor Name ", *m_pzsTag );
      TraceLineS( "SS::ctor View Name ", *m_pzsVName );
   }
#endif

   // left, center, right justify unused at the grid level.
   m_ulMapActFlags &= ~(zCONTROL_CENTERJUSTIFY | zCONTROL_RIGHTJUSTIFY | zCONTROL_LEFTJUSTIFY);

   RegisterWindowClass( );

#if !defined( GRIDCONTROL_NO_DRAGDROP ) || !defined( GRIDCONTROL_NO_CLIPBOARD )
   _AFX_THREAD_STATE *pState = AfxGetThreadState( );
   if ( !pState->m_bNeedTerm && !AfxOleInit( ) )
      AfxMessageBox( _T( "OLE initialization failed. Make sure that "
                         "the OLE libraries are the correct version" ) );
#endif

//?m_bListMode = TRUE;
//?SetHeaderSort( bHeaderSort );
//?SetSingleRowSelection( bSingleSelMode );
//?SetSingleColSelection( bSingleColSelMode );
//?EnableSelection( bSelectable );
//?SetRowResize( bAllowRowResize );
//?SetColumnResize( bAllowColumnResize );
#if 0
   if ( bItalics )
   {
      CFont *pFont = GetFont( );
      if ( pFont )
      {
         LOGFONT lf;
         pFont->GetLogFont( &lf );
         lf.lfItalic = (BYTE) bItalics;

         GetDefaultCell( TRUE, FALSE )->SetFont( &lf );
         GetDefaultCell( FALSE, TRUE )->SetFont( &lf );
         GetDefaultCell( TRUE, TRUE )->SetFont( &lf );

         Invalidate( );
         AutoSizeColumns( GVS_HEADER );
      }
   }
//?EnableTitleTips( bTitleTips );

   m_bTrackFocus = FALSE;
   m_bFrameFocus = FALSE;
   m_bVirtualMode = TRUE;
   m_bCallback = TRUE;
   m_bVertical = TRUE;
   m_bExpandUseFixed = FALSE;
   m_bRejectEditAttempts = TRUE;
   m_bRejectEditChanges = TRUE;
   m_bRow2Col2Hidden = FALSE;
#endif

#if 0
   // Init spin controls.
   m_RowSpin.SetRange( 0, 999 );
   m_FixRowSpin.SetRange( 0, 999 );
   m_FixColSpin.SetRange( 0, 999 );
   m_ColSpin.SetRange( 0, 999 );
#endif

   /////////////////////////////////////////////////////////////////////////
   // Initialize grid properties.
   /////////////////////////////////////////////////////////////////////////

// m_ImageList.Create( MAKEINTRESOURCE( IDB_IMAGES ), 16, 1,
//                     RGB( 255, 255, 255 ) );
// SetImageList( &m_ImageList );

   EnableDragAndDrop( TRUE );
   GetDefaultCell( FALSE, FALSE )->SetBackClr( RGB( 0xFF, 0xFF, 0xE0 ) );

   SetCallbackFunc( GridCallback, 0 );

   SetFixedColumnSelection( FALSE );
   SetFixedRowSelection( TRUE );
   EnableColumnHide( );

   SetCompareFunction( CellNumericCompare );

   m_vApp = 0;
   m_vAppList = 0;
   m_csViewName += *m_pzsTag;
   m_csViewNameList += *m_pzsTag;

   m_ulEntityKey = -1;
   m_pColorList = 0;

   m_nRetryMapFromOI = 0;
   m_lEntityCnt = 0;
   m_pZeidonWnd = 0;
   m_ulSubtype = pCtrlDef->Subtype;
   m_pchListInfo = 0;
   m_nDisplaySelSet = 0;
   m_nSelectSelSet = 1;
   m_pCol = 0;
   m_bSelectAll = FALSE;
   m_bSelect = FALSE;

   // Store the system colors in case they change. The gridctrl uses
   // these colors, and in OnSysColorChange we can check to see if
   // the gridctrl colors have been changed from the system colors.
   // If they have, then leave them, otherwise change them to reflect
   // the new system colors.
   m_clrWindowText = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrWindow = ::GetSysColor( COLOR_WINDOW );
   m_clr3DFace = ::GetSysColor( COLOR_3DFACE );
   m_clrShadow = ::GetSysColor( COLOR_3DSHADOW );
   m_clrGridLine = RGB( 192, 192, 192 );

   m_bVirtualMode = FALSE;
   m_pfnCallback = 0;

   if ( pCtrlDef->StyleX & zSS_SCOPE_OI )
      m_ulFlag = zQUAL_SCOPE_OI;
   else
      m_ulFlag = 0;

   // Set up the initial grid size.
   m_lRows = 0;
   m_nCols = 0;
   m_nColsMax = 0;

   // Sorting stuff.
   m_bSortOnClick = TRUE;   // sort on header row click
   m_nSortColumn = -1;
   m_nColLastSorted =-1;
   m_pfnCompare = 0;
   m_bAutoSort = FALSE;
   m_bRememberSort = FALSE;
   m_bSortDisplay = FALSE;
   m_pSortColumnList = 0;

   if ( pCtrlDef->OptionFlags )
   {
      m_nExtraRowsPerItem = 1;
      m_nExtraCol = zLOUSHORT( pCtrlDef->OptionFlags );
      m_nExtraOffset = zHIUSHORT( pCtrlDef->OptionFlags );
   }
   else
   {
      m_nExtraRowsPerItem = 0;
      m_nExtraCol = 0;
      m_nExtraOffset = 0;
   }

   if ( (pch = zstrchr( *m_pzsTag, '@' )) != 0 )
   {
      pch++;
      m_nExtraRowsPerItem = zatol( pch );
      m_nExtraRowsPerItem = min( 1, m_nExtraRowsPerItem );
      if ( (pch = zstrchr( pch, ';' )) != 0 )
      {
         pch++;
         m_nExtraCol = zatol( pch );
         if ( (pch = zstrchr( pch, ';' )) != 0 )
         {
            pch++;
            m_nExtraOffset = zatol( pch );
         }
      }
   }

   if ( (m_ulSubtype & zSS_HIDEROWHEADER) == 0 )
   {
      m_nFixedRows = 1;
      if ( (pCtrlDef->StyleX & zSS_SORTEDHEADERS) ||
           (pCtrlDef->StyleX & zSS_REMEMBERSORT) )
      {
         m_bAutoSort = TRUE;
         if ( pCtrlDef->StyleX & zSS_REMEMBERSORT )
            m_bRememberSort = TRUE;

      // if ( pCtrlDef->Subtype & zSS_SORT_DISPLAY )
      //    m_bSortDisplay = TRUE;
      }
   }
   else
   {
      m_nFixedRows = 0;
   }

   if ( (m_ulSubtype & zSS_HIDEROWSELECTIONCOL) == 0 )
      m_nFixedCols = 1;
   else
      m_nFixedCols = 0;

   // Because of settings for the original SpreadSheet cannot be
   // changed, the following code is a bit convoluted.
   // If both HideGridLines and ShowVerticalGridLines are set, show
   // vertical gridlines only.
   // If HideGridLines is set and ShowVerticalGridLines is NOT set, hide
   // both gridlines.
   // If HideGridLines is NOT set and ShowVerticalGridLines is set, show
   // horizontal gridlines only.
   // If neither HideGridLines nor ShowVerticalGridLines are set, show
   // both gridlines.

   if ( (m_ulSubtype & zSS_HIDEGRIDLINES) == 0 &&
        (pCtrlDef->StyleX & zSS_SHOWVERTICALGRIDLINES) == 0 )
   {
      m_nGridLines = GVL_BOTH;
   }
   else
   if ( (m_ulSubtype & zSS_HIDEGRIDLINES) != 0 &&
        (pCtrlDef->StyleX & zSS_SHOWVERTICALGRIDLINES) == 0 )
   {
      m_nGridLines = GVL_NONE;
   }
   else
   if ( (m_ulSubtype & zSS_HIDEGRIDLINES) == 0 &&
        (pCtrlDef->StyleX & zSS_SHOWVERTICALGRIDLINES) != 0 )
   {
      m_nGridLines = GVL_HORZ;
   }
   else
// if ( (m_ulSubtype & zSS_HIDEGRIDLINES) != 0 &&
//      (pCtrlDef->StyleX & zSS_SHOWVERTICALGRIDLINES) != 0 )
   {
      m_nGridLines = GVL_VERT;
   }

   // The painter must ensure Entity/Attribute if ViewName???
   int nCols = 0;
   if ( m_pzsVName )
   {
#ifdef DEBUG_ALL
      TraceLineS( "ZGrid::ctor: ", *m_pzsTag );
      TraceLineS( "ZGrid ViewName", *m_pzsVName );
      TraceLineS( "ZGrid Entity", *m_pzsEName );
#endif

      zULONG ulListLth;

      GetAttributeLength( &ulListLth, vDialog, szlCtrl, szlCtrlBOI );
      if ( ulListLth )
      {
      // zCHAR  szSubBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
         zLONG  lChar;
         zLONG lLth;
         zSHORT k;

         m_pchListInfo = new char[ ulListLth ];
         GetVariableFromAttribute( m_pchListInfo, &ulListLth, zTYPE_BLOB,
                                   ulListLth, vDialog, szlCtrl, szlCtrlBOI,
                                   0, 0 );

         nCols = (zSHORT) *((zPLONG) m_pchListInfo) + m_nFixedCols;
         m_pCol = new zLB_COL[ nCols ];
         zmemset( m_pCol, 0, sizeof( zLB_COL ) * nCols );

         // Offset to tags is stored in second slot.
         pch = m_pchListInfo + *((zPLONG) (m_pchListInfo + sizeof( zLONG )));

         // Set the titles and column widths for this grid.
         for ( k = m_nFixedCols; k < nCols; k++ )
         {
            // New grid with possible sub-controls.
            if ( *pch == (char) 0xFF )
               pch += (3 * sizeof( zLONG )) + 34; // zsizeof( szSubBlob );

            // Get the number of dialog units allotted for this attribute.
            lChar = *((zPLONG) pch);
            ASSERT( lChar >= 0 );

            if ( m_pZSubtask->m_bUseMapDlgUnits )
               lChar = mConvertMapDlgToCharX( lChar ) / 2 + 1;
            else
               lChar = mConvertDlgUnitToCharX( lChar ) / 2 + 1;

            m_pCol[ k ].nExtent = (zSHORT) lChar;

            pch += sizeof( zLONG );
            lChar = *((zPLONG) pch);
            ASSERT( lChar >= 0 );
            pch += sizeof( zLONG );
            pchComma = zstrchr( pch, '\t' );
            if ( pchComma )
               *pchComma = 0;

            lLth = zstrlen( pch ) + 1;
            m_pCol[ k ].pchText = new char[ lLth ];
            strcpy_s( m_pCol[ k ].pchText, lLth, pch );

            if ( pchComma )
            {
               *pchComma = '\t';
               lLth = zstrlen( pchComma + 1 ) + 1;
               m_pCol[ k ].pchMB_Text = new char[ lLth ];
               strcpy_s( m_pCol[ k ].pchMB_Text, lLth, pchComma + 1 );
            }
            else
               m_pCol[ k ].pchMB_Text = 0;

            pch += lChar;

            // Set tag.
            lChar = *((zPLONG) pch);
            ASSERT( lChar >= 0 );
            pch += sizeof( zLONG );
            lLth = zstrlen( pch ) + 1;
            m_pCol[ k ].pchTag = new char[ lLth ];
            strcpy_s( m_pCol[ k ].pchTag, lLth, pch );

            // Skip past tag.
            pch += lChar;
            ASSERT( (pch - m_pchListInfo) <= (zLONG) ulListLth );
         }

         // Loop through m_pchListInfo to see what type of cells should
         // be created for the attributes.
         pch = m_pchListInfo + sizeof( zLONG ) + sizeof( zULONG );
         for ( k = m_nFixedCols; k < nCols; k++ )
         {
//          TraceLineI( "ZSS::ctor col ", k );
            if ( *pch )
            {
               lChar = *((zPLONG) pch);
               pch += sizeof( zLONG );
               m_pCol[ k ].pchEntity = new char[ lChar ];
               strcpy_s( m_pCol[ k ].pchEntity, lChar, pch );

               // Skip past entity name.
               pch += lChar;
            }
            else
            {
               pch += sizeof( zLONG );
               lLth = m_pzsEName->GetLength() + 1;
               m_pCol[ k ].pchEntity = new char[ lLth ];
               strcpy_s( m_pCol[ k ].pchEntity, lLth, *m_pzsEName );
            }

            lChar = *((zPLONG) pch);
            pch += sizeof( zLONG );
            m_pCol[ k ].pchAttrib = new char[ lChar ];
            strcpy_s( m_pCol[ k ].pchAttrib, lChar, pch );

            // Skip past attribute name.
            pch += lChar;

            lChar = *((zPLONG) pch);
            pch += sizeof( zLONG );
            m_pCol[ k ].pchContext = new char[ lChar ];
            strcpy_s( m_pCol[ k ].pchContext, lChar, pch );

            // Skip past context name.
            pch += lChar;

            m_pCol[ k ].ulStyle = *((zPULONG) pch);
            pch += sizeof( zLONG );     // skip past cell Style

            m_pCol[ k ].ulSubtype = *((zPULONG) pch);

            // Hold on to original disabled setting.
            if ( m_pCol[ k ].ulSubtype & zCONTROL_DISABLED )
               m_pCol[ k ].ulSubtype |= zCONTROL_SIZEABLEBORDER;
            else
               m_pCol[ k ].ulSubtype &= ~zCONTROL_SIZEABLEBORDER;

            pch += sizeof( zLONG );     // skip past cell Subtype

            if ( m_pCol[ k ].ulSubtype & zCONTROL_RIGHTJUSTIFY )
               m_pCol[ k ].nJustify = DT_RIGHT;   // right justify
            else
            if ( m_pCol[ k ].ulSubtype & zCONTROL_CENTERJUSTIFY )
               m_pCol[ k ].nJustify = DT_CENTER;  // center justify
            else
               m_pCol[ k ].nJustify = DT_LEFT;    // left justify

            if ( (m_pCol[ k ].ulStyle & 0x0FFF0000L) == zGRID_CHECKBOX )
            {
               lLth = sizeof( zLONG ) + *((zPLONG) pch); // skip past Text
               lLth += sizeof( zLONG ) + *((zPLONG) (pch + lLth)); // skip ON
               lLth += sizeof( zLONG ) + *((zPLONG) (pch + lLth)); // skip OFF

               m_pCol[ k ].pchData = new char[ lLth ];
               zmemcpy( m_pCol[ k ].pchData, pch, lLth );
               pch += lLth;
            }
         }
      }
   }

   // Initialize printing stuff.
   m_nPageWidth = 0;
   m_nPrintColumn = 0;
   m_nCurrPrintRow = 0;
   m_nNbrPages = 0;
   m_nPageMultiplier = 0;

   // Initialize scroll position.
   m_nVScrollMax = 0;
   m_nHScrollMax = 0;

   // Get the number of lines per mouse wheel notch to scroll.
   m_nRowsPerWheelNotch = GetMouseScrollLines( );
   m_nBarState = GVL_NONE;
   m_MouseMode = MOUSE_NOTHING;
   m_bEditable = TRUE;
   m_bListMode = FALSE;
   m_bSingleRowSelection = FALSE;
   m_bSingleColSelection = FALSE;
   m_bLMouseButtonDown = FALSE;
   m_bRMouseButtonDown = FALSE;
   m_bAllowDraw = TRUE;   // allow draw updates
   m_bEnableSelection = TRUE;
   m_bFixedColumnSelection = TRUE;
   m_bFixedRowSelection = TRUE;
   m_bAllowRowResize = TRUE;
   m_bAllowColumnResize = TRUE;
   m_bHandleTabKey = TRUE;
#ifdef _WIN32_WCE
   m_bDoubleBuffer = FALSE;  // Use double buffering to avoid flicker?
#else
   m_bDoubleBuffer = TRUE;   // Use double buffering to avoid flicker?
#endif
   m_bTitleTips = TRUE;   // show cell title tips

   m_bWysiwygPrinting = FALSE;  // use size-to-width printing

   m_bHiddenColUnhide = TRUE;   // 0 - width columns can be expanded via mouse
   m_bHiddenRowUnhide = TRUE;   // 0 - Height rows can be expanded via mouse

   m_bAllowColHide = TRUE;   // columns can be contracted to 0-width via mouse
   m_bAllowRowHide = TRUE;   // rows can be contracted to 0-height via mouse

   m_nAutoSizeColumnStyle = GVS_BOTH; // Autosize grid using header and data info

   m_nTimerID = 0;      // For drag-selection
   m_nTimerInterval = 25;     // (in milliseconds)
   m_nResizeCaptureRange = 3;      // When resizing columns/row, the cursor
                                   // has to be within +/-3 pixels of the
                                   // dividing line for resizing to be possible
   m_pImageList = 0;      // Images in the grid
   m_bAllowDragAndDrop = FALSE;  // for drag and drop - EFW - off by default
   m_bTrackFocusCell = TRUE;   // Track Focus cell?
   m_bFrameFocus = TRUE;   // Frame the selected cell?

   m_ulKeydownMessageTime = 0;
   m_ulDblclkTime = GetDoubleClickTime( );
   m_szKeyFind[ 0 ] = 0;
   m_nKeydownCnt = 0;

   m_pRtcDefault = RUNTIME_CLASS( ZGridCell );

   SetupDefaultCells( );
// SetGridBkColor( m_clrShadow );
   SetGridBkColor( m_clr3DFace );

   TRY
   {
   // SetRowCount( m_lRows );
      SetFixedRowCount( m_nFixedRows );
      SetFixedColumnCount( m_nFixedCols );
      SetRowCount( m_nFixedRows ); // corrected later in MapFromOI
      SetColumnCount( nCols ); // m_nCols set from CtrlBOI (includes fixed)
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return;
   }
   END_CATCH

   SetTitleTipTextClr( CLR_DEFAULT );  // FNA
   SetTitleTipBackClr( CLR_DEFAULT );

   // Set initial selection range (i.e. none).
   m_SelectedCellMap.RemoveAll( );
   m_PrevSelectedCellMap.RemoveAll( );

#if !defined ( _WIN32_WCE_NO_PRINTING ) && !defined ( GRIDCONTROL_NO_PRINTING )
   // EFW - Added to support shaded/unshaded printout and
   // user-definable margins.
   m_bShadedPrintOut = TRUE;
   SetPrintMarginInfo( 2, 2, 4, 4, 1, 1, 1 );
#endif

// TraceLineI( "Loading Grid Blob for lth: ", m_ulListLth );
   Attr.Style |= WS_CHILD | WS_TABSTOP;
//xif ( pCtrlDef->Subtype & zSS_HIDEROWHEADER )
//x   Attr.Style |= SSS_HIDEROWHEADERS;
//x
//xif ( pCtrlDef->Subtype & zSS_HIDEGRIDLINES )
//x   Attr.Style |= SSS_NOGRIDLINES;
//x
//xAttr.Style |= SSS_NOSELECTBLOCK;
// Attr.Style |= SSS_SCROLLBAREXTMODE;
//
// | SSS_RESTRICTROWS;
// Attr.Style |= WS_HSCROLL | WS_VSCROLL;
//
   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) == 0 )
      Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   if ( pCtrlDef->StyleX & zSS_MULTISELECT )
      m_chMultiSelect = 'Y';
   else
      m_chMultiSelect = 0;

   m_chListOnlyWhenSelected = 0;

// if ( (m_ulSubtype & SS_NOINTEGRALHEIGHT) == 0 )  flag disabled
//x{
//x   k = 3 * GetSystemMetrics( SM_CYHSCROLL );
//x   if ( Attr.H > 2 * k )
//x      Attr.H -= k;
//x}

// SetVirtualMode( TRUE );
   SetEditable( TRUE );
   SetCallbackFunc( GridCallback, (LPARAM) this );

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

// ZGrid - dtor
ZGrid::~ZGrid( )
{
// TraceLineS( "ZGrid::~ZGrid", "" );
#ifdef DEBUG_ALL
   TraceLineS( "ZGrid::dtor ", *m_pzsTag );
#endif

   mDeleteInitA( m_pchListInfo );
   mDeleteInit( m_pColorList );

   if ( m_pCol )
   {
      for ( zSHORT nCol = 0; nCol < m_nCols; nCol++ )
      {
         mDeleteInitA( m_pCol[ nCol ].pchEntity );
         mDeleteInitA( m_pCol[ nCol ].pchAttrib );
         mDeleteInitA( m_pCol[ nCol ].pchContext );
         mDeleteInitA( m_pCol[ nCol ].pchTag );
         mDeleteInitA( m_pCol[ nCol ].pchText );
         mDeleteInitA( m_pCol[ nCol ].pchData );
         mDeleteInitA( m_pCol[ nCol ].pchMB_Text );
         mDeleteInitA( m_pCol[ nCol ].pchEntityX );
         mDeleteInitA( m_pCol[ nCol ].pchAttribX );
         mDeleteInitA( m_pCol[ nCol ].pchContextX );
         mDeleteInitA( m_pCol[ nCol ].pchColorAttribute );
      }

      mDeleteInitA( m_pCol );
   }

   DeleteAllItems( );

#ifndef GRIDCONTROL_NO_TITLETIPS
   if ( m_bTitleTips && ::IsWindow( m_wndTitleTip.GetSafeHwnd( ) ) )
      m_wndTitleTip.DestroyWindow( );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

#if !defined( GRIDCONTROL_NO_DRAGDROP ) || !defined( GRIDCONTROL_NO_CLIPBOARD )
    // BUG FIX - EFW
   COleDataSource *pSource = COleDataSource::GetClipboardOwner( );
   if ( pSource )
      COleDataSource::FlushClipboard( );
#endif

// delete( *m_pzsScope ); // please do not delete m_pzsScope cuz it's
                          // just an alias for m_pzsAName
}

void
ZGrid::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   if ( (m_ulMapActFlags & zMAPACT_ENABLED) ||
        (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
   {
      Attr.Style &= ~WS_DISABLED;
   }
   else
      Attr.Style |= WS_DISABLED;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else

   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( CRect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H ),
                 m_pWndParent, m_nIdNbr,
                 Attr.Style | WS_CHILD | WS_BORDER | WS_TABSTOP )) )
     // CreateEx( WS_EX_CLIENTEDGE, GRIDCTRL_CLASSNAME, *m_pzsText,
     //           Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
     //           m_pWndParent->m_hWnd, (HMENU) m_nIdNbr ) )
   {
   // CRect rect;
   // GetClientRect( rect );
   // AutoSize( );
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZGrid::GetPreferencesFromIni( )
{
   zCHAR  szBuffer[ 40 ];
   int    nRowHeight = 0;
   int    nLth = 120 * m_nCols;  // (Ent.Attr.Context + fudge) * # of columns
   zPCHAR pchBuffer = new char[ nLth + 1 ];
   zPCHAR pchComma;
   zPCHAR pch;
   int    nCol;
   int    k;

   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   GetWindowPreferenceString( m_pZSubtask->m_vDialog,
                              szBuffer, pchBuffer, nLth );

   nCol = 0;
   if ( *pchBuffer )
   {
      pch = pchBuffer;
      if ( *pch == '~' )
      {
         pch++;
         nRowHeight = (zSHORT) zatol( pch );
         pchComma = zstrchr( pch, ',' );
         if ( pchComma )
            pch = pchComma + 1;
      }

      while ( pch )
      {
         pch = strchr( pch, ',' );
         if ( pch )
            pch++;

         nCol++;
      }
   }

   if ( nCol == m_nCols )
   {
      pch = pchBuffer;
      if ( *pch == '~' )
      {
         pchComma = zstrchr( pch, ',' );
         if ( pchComma )
            pch = pchComma + 1;
      }

      for ( nCol = 0; nCol < m_nCols; nCol++ )
      {
         pchComma = zstrchr( pch, ',' );
         if ( pchComma )
            *pchComma = 0;

         m_pCol[ nCol ].nExtent = (zSHORT) zatol( pch );
         m_arColWidths[ nCol ] = m_pCol[ nCol ].nExtent;

         if ( pchComma )
            pch = pchComma + 1;
         else
            break;
      }

      if ( m_bRememberSort )
      {
         zBOOL bLocateSort = FALSE;

         strcpy_s( szBuffer, zsizeof( szBuffer ), "Sort:" );
         strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
         GetWindowPreferenceString( m_pZSubtask->m_vDialog,
                                    szBuffer, pchBuffer, nLth );
         k = (zSHORT) zstrlen( pchBuffer );
         if ( k )
         {
            // Look for "check bytes" at end of string.
            if ( k > 1 &&
                 pchBuffer[ k - 1 ] == '#' && pchBuffer[ k - 2 ] == '#' )
            {
               k -= 2;
               pchBuffer[ k ] = 0;  // we found it so things should be OK
               while ( k && zisspace( pchBuffer[ k - 1 ] ) )
               {
                  k--;
                  pchBuffer[ k ] = 0;
               }

               bLocateSort = TRUE;
            }

            // We need to do this to get the first sort order even if we found
            // valid "check bytes".
            {
               // Determine if this is a valid set of entity/attributes
               // the good old fashioned way ... brute force.
               //
               // Sort:List=W_MetaDef.LastUpdateDate D [ListDisplayNumeric]
               //           W_MetaDef.Name D W_MetaDef.Desc D

               while ( k && zisspace( pchBuffer[ k - 1 ] ) )
               {
                  k--;
                  pchBuffer[ k ] = 0;
               }

               zBOOL  bError;
               zPCHAR pchPeriod;
               zPCHAR pchSpace;
               zPCHAR pchBracket;

               pch = pchBuffer;
               while ( pch && *pch )
               {
                  bError = TRUE;
                  pchSpace = 0;
                  pchBracket = 0;
                  pchPeriod = zstrchr( pch, '.' );
                  if ( pchPeriod )
                  {
                     pchSpace = zstrchr( pchPeriod, ' ' );
                     if ( pchSpace &&
                          (*(pchSpace + 1) == 'A' || *(pchSpace + 1) == 'D') )
                     {
                        if ( m_nColLastSorted == -1 )
                           m_chAscendDescend = *(pchSpace + 1);

                        if ( *(pchSpace + 2) == ' ' &&
                             *(pchSpace + 3) == '[' )
                        {
                           pchBracket = zstrchr( pchSpace + 4, ']' );
                           if ( pchBracket )
                              bError = FALSE;
                        }
                        else
                           bError = FALSE;
                     }
                  }

                  if ( bError == FALSE )
                  {
                     *pchPeriod = 0;
                     *pchSpace = 0;
                     if ( pchBracket )
                        *pchBracket = 0;

                     for ( nCol = m_nFixedCols; nCol < m_nCols; nCol++ )
                     {
                        if ( zstrcmp( m_pCol[ nCol ].pchEntity,
                                      pch ) == 0 &&
                             zstrcmp( m_pCol[ nCol ].pchAttrib,
                                      pchPeriod + 1 ) == 0 &&
                             ((pchBracket == 0 &&
                               (m_pCol[ nCol ].pchContext == 0 ||
                                m_pCol[ nCol ].pchContext[ 0 ] == 0)) ||
                              (pchBracket && m_pCol[ nCol ].pchContext &&
                               zstrcmp( m_pCol[ nCol ].pchContext,
                                        pchSpace + 4 ) == 0)) )
                        {
                           if ( m_nColLastSorted == -1 )
                              m_nColLastSorted = nCol;

                           break;   // out of for
                        }
                     }
                  }

                  if ( pchPeriod )
                  {
                     *pchPeriod = '.';
                     if ( pchSpace )
                     {
                        *pchSpace = ' ';
                        if ( pchBracket )
                           *pchBracket = ']';
                     }
                  }

                  if ( bError || nCol >= m_nCols )
                  {
                     // Invalid entry ... get out!
                     TraceLine( "ZGrid invalid sort order: %s=%s",
                                szBuffer, pchBuffer );
                     pchBuffer[ 0 ] = 0;
                     m_nColLastSorted = -1;
                     m_chAscendDescend = 0;
                     break;
                  }

               // if ( bLocateSort && m_nColLastSorted >= 0 )  we gotta look at all entries!
               //    break;   // out of while

                  if ( pchBracket )
                  {
                     if ( *(pchBracket + 1) )
                        pch = pchBracket + 2;
                     else
                        pch = pchBracket + 1;
                  }
                  else
                  {
                     if ( *(pchSpace + 2 ) )
                        pch = pchSpace + 3;
                     else
                        pch = pchSpace + 2;
                  }
               }
            }
         }

         m_csSortBuffer = pchBuffer;
      }
   }
   else
      m_csSortBuffer = "";

   mDeleteInitA( pchBuffer );
   return( nRowHeight );
}

zSHORT
ZGrid::PostCreateCtrls( WPARAM wFlag )
{
   if ( m_pZSubtask->m_vPE == 0 )
   {
      // Set the column widths for this spreadsheet.
      zPCHAR pch = m_pchListInfo + *((zPLONG) (m_pchListInfo + sizeof( zLONG )));
      zCHAR  szSubBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
      zLONG  lCumulativeWidth = 0;
      zLONG  lChar;
      int    nWidth = m_cellDefault.GetWidth( );
      int    k;

      for ( k = m_nFixedCols; k < m_nCols; k++ )
      {
         m_arColWidths[ k ] = nWidth;

         // New grid with possible sub-controls.
         if ( *pch == (char) 0xFF )
         {
            zmemcpy( szSubBlob, pch, zsizeof( szSubBlob ) );
            szSubBlob[ 0 ] = 0;
            GRID_SetColumnCtrl( m_pZSubtask->m_vDialog, *m_pzsTag,
                                k + (1 - m_nFixedCols),
                                szSubBlob + (3 * sizeof( zLONG )),
                                *((zPSHORT) (szSubBlob + 2)) );
            pch += zsizeof( szSubBlob );
         }

         // Get the number of dialog units allotted for this attribute.
         lChar = *((zPLONG) pch);

         if ( m_pZSubtask->m_bUseMapDlgUnits )
            lChar = mConvertMapDlgToPixelX( lChar );
         else
            lChar = mConvertDlgUnitToPixelX( lChar );

         m_arColWidths[ k ] = lChar;

         pch += sizeof( zLONG );
         lChar = *((zPLONG) pch);
         pch += sizeof( zLONG ) + lChar;

         // Skip past tag.
         lChar = *((zPLONG) pch);
         pch += sizeof( zLONG ) + lChar;
      }
   }

   return( 0 );
}

int
ZGrid::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CWnd::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );  // do this before setting heights to get correct values

   // Initialize( ); - called in PreSubclassWnd

   // The number of rows and columns will only be non-zero if the constructor
   // was called with non-zero initializing parameters. If this window was created
   // using a dialog template then the number of rows and columns will be 0 (which
   // means that the code below will not be needed - which is lucky 'cause it ain't
   // gonna get called in a dialog-template-type-situation).

   int nHeight = m_cellDefault.GetHeight( );
   int nColCnt = GetColumnCount( );
   int nCol;
   int k;

   // Loop processing each column.  Fill header row with text.
   for ( nCol = 0; nCol < nColCnt; nCol++ )
   {
      GV_ITEM Item;

      Item.mask = GVIF_TEXT;
      Item.lRow = 0;
      Item.nCol = nCol;
      Item.csText = m_pCol[ nCol ].pchText;
      SetItem( &Item );
   }

   for ( k = 0; k < m_lRows; k++ )
      m_arRowHeights[ k ] = nHeight;

   PostCreateCtrls( 0 );

   if ( m_nFixedCols > 0 )
   {
      CClientDC dc( this );
      CSize size = dc.GetTextExtent( "9999" );
      m_arColWidths[ 0 ] = size.cx;
   }

   int nRowHeight = GetPreferencesFromIni( );
   SetSortColumn( m_nColLastSorted );

   if ( nRowHeight && nRowHeight != nHeight )
      SetRowHeight( 0, nRowHeight );

   return( nRC );
}

BOOL
ZGrid::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGrid::DestroyWindow ", *m_pzsTag );
   TraceLineI( "ZGrid::DestroyWindow EntityCnt ", m_lEntityCnt );
#endif
   zCHAR szBuffer[ 40 ]; // tag length + 7

   if ( (ZSubtask::GetSubtaskState( m_pZSubtask ) & zSUBTASK_STATE_DELETED) == 0 )
      EndEditing( );      // destroy any InPlaceEdit's

   m_lEntityCnt = 0;
   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }

   zLONG  lLth = zsizeof( "-2147483647," ) * (m_nCols + 1);
   zPCHAR pch = new char[ lLth ];
   zSHORT nHeight = GetRowHeight( 0 );
   zSHORT nPos = 0;
   zSHORT k;

   if ( nHeight != m_cellDefault.GetHeight( ) )
   {
      *pch = '~';
      _ltoa_s( nHeight, pch + nPos + 1, lLth - nPos - 1, 10 );
      nPos = (zSHORT) zstrlen( pch );
      pch[ nPos++ ] = ',';
   }

   for ( k = 0; k < m_nCols; k++ )
   {
      _ltoa_s( GetColumnWidth( k ), pch + nPos, lLth - nPos, 10 );
      nPos = (zSHORT) zstrlen( pch );
      if ( k < m_nCols - 1 )
         pch[ nPos++ ] = ',';
   }

   pch[ nPos ] = 0;
   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );
   if ( m_bRememberSort )
   {
      strcpy_s( szBuffer, zsizeof( szBuffer ), "Sort:" );
      strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );

      // Set the "check bytes" at end of string.
      m_csSortBuffer += _T( "##" );
      SetWindowPreferenceString( m_pZSubtask->m_vDialog,
                                 szBuffer, m_csSortBuffer );
      m_csSortBuffer.TrimRight( '#' );
   }

   mDeleteInitA( pch );

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

void
ZGrid::SetColorAttribute( zCPCHAR cpcAttributeName, zSHORT nCol )
{
   mDeleteInitA( m_pCol[ nCol ].pchColorAttribute );
   zLONG lLth = zstrlen( cpcAttributeName ) + 1;
   m_pCol[ nCol ].pchColorAttribute = new char[ lLth ];
   strcpy_s( m_pCol[ nCol ].pchColorAttribute, lLth, cpcAttributeName );
}

void
ZGrid::SetColorAssociation( zLONG   lColumn,
                            zCPCHAR cpcValue,
                            zLONG   lClrText,
                            zLONG   lClrBack,
                            zLONG   lFlags )
{
   if ( m_pColorList == 0 )
      m_pColorList = new ZColorList( CompareCell );

   m_pColorList->Add( lColumn, cpcValue, lClrText, lClrBack, lFlags );
}

void
ZGrid::RemoveColorAssociations( )
{
   mDeleteInit( m_pColorList );
}

#if 0 //def DEBUG_ALL lots of messages
void
ZGrid::DisplayCellData( zCPCHAR cpcText )
{
// LPARAM lCellData;
   if ( zstrcmp( *m_pzsTag, "SSQueryAttributes" ) == 0 )
   {
      ZGridCellBase *pCell;
      long lRow;
      int  nCol;
      for ( lRow = 0; lRow < m_lRows; lRow++ )
      {
         for ( nCol = 0; nCol < m_nCols; nCol++ )
         {
            pCell = GetCell( lRow, nCol );
         // lCellData = GetItemData( lRow, nCol );
            TraceLine( "GetItemData1 Row: %d  Col: %d  CellData: %s", lRow, nCol, pCell->m_csText );
         }
      }
   }
}
#else
#define DisplayCellData
#endif

// Load csText with the value to put in the listbox.
zSHORT
ZGrid::FormatTextForCell( CString& csText, zLONG lRow, zSHORT nCol )
{
   csText = "";  // initialize to null string in case there are problems
   zLONG lStyle = GetWindowLong( m_hWnd, GWL_STYLE );

   // If we don't have grid column info or we are in a delete state ... quit.
   if ( m_pCol == 0 || (ZSubtask::GetSubtaskState( m_pZSubtask ) & zSUBTASK_STATE_DELETED) )
   {
      mDeleteInit( m_pCol );
      return( -1 );
   }

   if ( m_lEntityCnt > 0 )
   {
      if ( MiValidView( m_vAppList ) == 0 ||
           CheckEntityExistence( m_vAppList, *m_pzsEName ) != 0 ) // no data to display
      {
         TraceLine( "ZGrid::FormatTextForCell for Control: %s  Row: %d  Col: %d  "
                      "Count: %d  has no data to display ... a refresh of this control"
                      " may be required",
                    *m_pzsTag, lRow, nCol, m_lEntityCnt );
         m_lEntityCnt = 0;
      }
   }

   if ( m_lEntityCnt <= 0 )  // no data to display
      return( 0 );

   DisplayCellData( "ZGrid::FormatTextForCell1" );

   if ( GetViewByName( &m_vAppList, m_csViewNameList,  m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
#ifdef DEBUG_ALL
      if ( zstrcmp( "__GridListAccount", *m_pzsTag ) == 0 && nCol == 4 )
      {
         TraceLine( "ZGrid::FormatTextForCell __GridListAccount: %s  Attribute: %s",
                     *(m_pZSubtask->m_pzsWndTag), m_pCol[ nCol ].pchAttrib );
      }
#endif

#if 0
      m_nRetryMapFromOI++;
      if ( m_nRetryMapFromOI <= 1 &&
           (m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ), "Unable to load view: %s for %s.%s.%s",
                   *m_pzsVName, *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), *m_pzsTag );
         TraceLineS( "ZGrid::FormatTextForCell Retry MapFromOI: ", szMsg );

      // MapFromOI( );  cannot do this since it destroys tables that
      //                are still needed by calling code
         continue;
      }
#endif

      // If we don't have the view for the list, see if we can create one.
      if ( GetViewByName( &m_vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         if ( m_nRetryMapFromOI < 2 && m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
         {
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, zsizeof( szMsg ), "Unable to load view: %s for %s.%s.%s",
                      (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                      (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString() );
            TraceLineS( "ZGrid::FormatTextForCell ", szMsg );
         }

      // m_vAppList = 0;
         return( -1 );
      }
      else
      {
         SetNameForView( m_vApp, m_csViewName, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
         CreateViewFromViewForTask( &m_vAppList, m_vApp, m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent the OE from
                                    // automatically cleaning it up during
                                    // the deletion of an OE subtask.
         m_ulEntityKey = GetEntityKey( m_vApp, *m_pzsEName );
         SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                          (zCPVOID) &m_ulEntityKey, 0, 0, 0, *m_pzsScope, 0 );
      }
   }

   // Ensure at least one entity.
   if ( lRow >= m_EntityKeyList.GetSize( ) ||
        SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                         m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                         0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
   {
      if ( m_nRetryMapFromOI < 2 && m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ), "Error in entity count for view: %s Entity: %s for %s.%s.%s",
                   (*m_pzsVName).GetString(), (*m_pzsEName).GetString(), (*m_pZSubtask->m_pzsDlgTag).GetString(),
                   (*m_pZSubtask->m_pzsWndTag).GetString(), (*m_pzsTag).GetString() );
         TraceLineS( "ZGrid::FormatTextForCell ", szMsg );
      }

   // DropView( m_vAppList );
   // m_vAppList = 0;
      m_nRetryMapFromOI = 2;
      return( -1 );
   }

   zULONG ulKey = m_EntityKeyList.GetAt( lRow );
   zULONG ulEntityKey = GetEntityKey( m_vAppList, *m_pzsEName );
   if ( ulKey != ulEntityKey )  // performance enhancement
   {
      SetCursorNextEntity( m_vAppList, *m_pzsEName, *m_pzsScope );
      ulEntityKey = GetEntityKey( m_vAppList, *m_pzsEName );
   }

   m_ulEntityKey = ulKey;
   if ( ulKey != ulEntityKey &&
        SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                         m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                         (zCPVOID) &ulKey, 0, 0, 0,
                         *m_pzsScope, 0 ) < zCURSOR_SET )
   {
      if ( m_nRetryMapFromOI < 2 &&
           m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ),
                   "Entity Key Error: %s  Entity: %s  for %s.%s.%s",
                   (*m_pzsVName).GetString(), (*m_pzsEName).GetString(), (*m_pZSubtask->m_pzsDlgTag).GetString(),
                   (*m_pZSubtask->m_pzsWndTag).GetString(), (*m_pzsTag).GetString() );
         TraceLineS( "ZGrid::FormatTextForCell ", szMsg );
      }

      m_ulEntityKey = -1;
      m_nRetryMapFromOI = 2;
      return( -1 );
   }

   m_nRetryMapFromOI = 0;

   // This is the attribute to present to the list.
   zCHAR  szMap[ 65534 ];
   zSHORT nRC;

   if ( m_pCol[ nCol ].pchAttrib == 0 || m_pCol[ nCol ].pchAttrib[ 0 ] == 0 )
      nRC = zVAR_NULL;
   else
   if ( (m_pCol[ nCol ].ulStyle & 0x0FFF0000L) == zGRID_CHECKBOX &&
        m_pCol[ nCol ].pchAttribX && m_pCol[ nCol ].pchAttribX[ 0 ] )
   {
      zCPCHAR cpcEntity = m_pCol[ nCol ].pchEntityX;
      zCPCHAR cpcContext = m_pCol[ nCol ].pchContextX;

      if ( cpcEntity == 0 || cpcEntity[ 0 ] == 0 )
         cpcEntity = m_pCol[ nCol ].pchEntity;

      nRC =
      GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                zsizeof( szMap ) - 1,
                                m_vAppList,
                                cpcEntity,
                                m_pCol[ nCol ].pchAttribX,
                                cpcContext,
                                cpcContext && cpcContext[ 0 ] ?
                                      zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
   }
   else
   {
      nRC =
      GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                zsizeof( szMap ) - 1,
                                m_vAppList,
                                m_pCol[ nCol ].pchEntity,
                                m_pCol[ nCol ].pchAttrib,
                                m_pCol[ nCol ].pchContext,
                                (m_pCol[ nCol ].pchContext &&
                                 m_pCol[ nCol ].pchContext[ 0 ]) ?
                                      zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

#if 0
      if ( **m_pzsTag == 'S' && nCol == 5 && m_ulEntityKey == 8312 )
      {
         TraceLine( "ZGrid::FormatTextForCell Tag: %s  RC: %d  "
                      "View: 0x%08x  EntityKey: %d  Row: %ld  Col: %d  VN: %s   "
                      "DlgTag: %s  WndTag: %s - %s",
                    *m_pzsTag, nRC, m_vAppList,
                    GetEntityKey( m_vAppList, m_pCol[ nCol ].pchEntity ),
                    lRow, nCol, *m_pzsVName,
                    *m_pZSubtask->m_pzsDlgTag, *m_pZSubtask->m_pzsWndTag, szMap );
         DisplayEntityInstance( m_vAppList, m_pCol[ nCol ].pchEntity );
      }
#endif
   }

#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "lbAttribute" ) == 0 )
   if ( m_bSortDisplay )
   {
      TraceLine( "ZGrid::FormatTextForCell Tag: %s  "
                   "View: 0x%08x  EntityKey: %d  Row: %ld  Col: %d  VN: %s   "
                   "DlgTag: %s  WndTag: %s - %s",
                 *m_pzsTag, m_vAppList, GetEntityKey( m_vAppList, m_pCol[ nCol ].pchEntity ),
                 lRow, nCol, *m_pzsVName,
                 *m_pZSubtask->m_pzsDlgTag, *m_pZSubtask->m_pzsWndTag, szMap );
   }
#endif

   if ( nRC == zVAR_NULL )
      szMap[ 0 ] = 0;

   // We have to do this every time to get the whole row colored if specified.
   if ( m_pColorList &&
        (m_pCol[ nCol ].pchColorAttribute || m_pCol[ 0 ].pchColorAttribute) )
   {
      // Do this every time since the data may change (unlike the ListBox).
   // LPARAM lCellData = GetItemData( lRow, nCol );
   // if ( lCellData == 0 )
      {
         zCHAR szValue[ 256 ];

         GetStringFromAttribute( szValue, zsizeof( szValue ), m_vAppList, *m_pzsEName,
                                 m_pCol[ nCol ].pchColorAttribute ?
                                   m_pCol[ nCol ].pchColorAttribute :
                                   m_pCol[ 0 ].pchColorAttribute );

         // Use the setting for the whole row (if it exists).  Otherwise
         // use whatever setting we can find (column specific).
         ZColorItem *pColorItem = m_pColorList->Find( 0, szValue, TRUE );
         if ( pColorItem == 0 )
            pColorItem = m_pColorList->Find( nCol, szValue, FALSE );

         SetItemData( lRow, nCol, (LPARAM) pColorItem );

      // lCellData = GetItemData( lRow, nCol );
      // TraceLine( "GetItemData Row: %d  Col: %d  CellData: %d",
      //            lRow, nCol, lCellData );
      }
   }

   TranslateValue( szMap, zsizeof( szMap ), TRUE, nCol );
   csText = szMap;

#ifdef DEBUG_ALL
// if ( zstrcmp( "__GridListAccount", *m_pzsTag ) == 0 && nCol == 4 )
   {
   // DisplayEntityInstance( m_vAppList, *m_pzsEName );
      TraceLine( "ZGrid::FormatTextForCell (%d, %d) Key: %d  Text: %s",
                 lRow, nCol, m_ulEntityKey, szMap );
   }
#endif

// TraceLine( "ZGrid::FormatTextForCell (%d, %d) Key: %d  Text: %s",
//            lRow, nCol, m_ulEntityKey, szMap );
   DisplayCellData( "ZGrid::FormatTextForCell2" );
   return( 0 );
}

// Load csText with the value to put in the listbox.
zSHORT
ZGrid::TransferDataFromCellToOI( zCPCHAR cpcText, zLONG lRow, zSHORT nCol )
{
   if ( **m_pzsTag == 'G' && lRow == 10 && nCol == 5 )
      TraceLineS( "", "" );

   // If we don't have list info or we are in a delete state ... quit.
   if ( m_pCol == 0 ||
        (ZSubtask::GetSubtaskState( m_pZSubtask ) & zSUBTASK_STATE_DELETED) )
   {
      mDeleteInit( m_pCol );
      return( -1 );
   }

   if ( m_lEntityCnt <= 0 )  // nothing to store
      return( 0 );

   DisplayCellData( "ZGrid::TransferDataFromCellToOI1" );
   zULONG ulKey = m_EntityKeyList[ lRow ];

#ifdef DEBUG_ALL
   if ( zstrcmp( "__GridListAccount", *m_pzsTag ) == 0 && nCol == 4 )
      TraceLine( "ZGrid::TransferDataFromCellToOI (%d, %d) Key: %d  Text: %s",
                 lRow, nCol, ulKey, cpcText );
#endif

   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
      // If we don't have the view for the list, see if we can create one.
      if ( GetViewByName( &m_vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
         {
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, zsizeof( szMsg ),
                      "Unable to load view: %s for %s.%s.%s",
                      (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                      (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString() );
            TraceLineS( "ZGrid::TransferDataFromCellToOI ", szMsg );
         }

         m_vAppList = 0;
         return( -1 );
      }
      else
      {
         SetNameForView( m_vApp, m_csViewName,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
         CreateViewFromViewForTask( &m_vAppList, m_vApp,
                                    m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewNameList,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent the OE from
                                    // automatically cleaning it up during
                                    // the deletion of an OE subtask.
         m_ulEntityKey = GetEntityKey( m_vApp, *m_pzsEName );
         SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                          m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                          (zCPVOID) &m_ulEntityKey,
                          0, 0, 0, *m_pzsScope, 0 );
      }
   }

   // Ensure at least one entity.
   if ( SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                         m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                         0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, zsizeof( szMsg ),
                "Error in entity count for view: %s for %s.%s.%s",
                (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString() );
      TraceLineS( "ZGrid::TransferDataFromCellToOI ", szMsg );
      DropView( m_vAppList );
      m_vAppList = 0;
      return( -1 );
   }

   if ( ulKey != GetEntityKey( m_vAppList, *m_pzsEName ) )
   {
      if ( SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                            m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &ulKey,
                            0, 0, 0, *m_pzsScope, 0 ) != 0 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ),
                   "Entity Key Error: %s for %s.%s.%s",
                   (*m_pzsVName).GetString(), (*m_pZSubtask->m_pzsDlgTag).GetString(),
                   (*m_pZSubtask->m_pzsWndTag).GetString(), (*m_pzsTag).GetString() );
         m_ulEntityKey = -1;
         TraceLineS( "ZGrid::TransferDataFromCellToOI ", szMsg );
         return( -1 );
      }

      m_ulEntityKey = ulKey;
   }

   // This is the attribute to present to the list.
   zCHAR  szMap[ 65534 ];

   // Set the attribute from the grid value.
   if ( CheckEntityExistence( m_vAppList,
                              m_pCol[ nCol ].pchEntity ) == zCURSOR_SET &&
        MiGetUpdateForViewEntityAttr( m_vAppList,
                                      m_pCol[ nCol ].pchEntity,
                                      m_pCol[ nCol ].pchAttrib,
                                      zUPD_NONPERSIST ) > 0 )
   {
      szMap[ 0 ] = 0;
      strncat_s( szMap, zsizeof( szMap ), cpcText, zsizeof( szMap ) - 1 );
      zBOOL bTranslated = TranslateValue( szMap, zsizeof( szMap ), FALSE, nCol );
      zSHORT nRC;

      if ( bTranslated )
      {
         nRC = SetAttributeFromVariable( m_vAppList,
                                         m_pCol[ nCol ].pchEntity,
                                         m_pCol[ nCol ].pchAttrib,
                                         szMap, zTYPE_STRING,
                                         zsizeof( szMap ) - 1,
                                         m_pCol[ nCol ].pchContext,
                                         (m_pCol[ nCol ].pchContext &&
                                          m_pCol[ nCol ].pchContext[ 0 ]) ?
                                                0 : zUSE_DEFAULT_CONTEXT );
      }
      else
      {
         nRC = SetAttributeFromVariable( m_vAppList,
                                         m_pCol[ nCol ].pchEntity,
                                         m_pCol[ nCol ].pchAttrib,
                                         cpcText, zTYPE_STRING,
                                         zstrlen( cpcText ),
                                         m_pCol[ nCol ].pchContext,
                                         (m_pCol[ nCol ].pchContext &&
                                          m_pCol[ nCol ].pchContext[ 0 ]) ?
                                                0 : zUSE_DEFAULT_CONTEXT );
      }

#ifdef DEBUG_ALL
// if ( zstrcmp( "__GridListAccount", *m_pzsTag ) == 0 && nCol == 4 )
   {
   // DisplayEntityInstance( m_vAppList, *m_pzsEName );
      TraceLine( "ZGrid::TransferDataFromCellToOI (%d, %d) Key: %d  Text: %s",
                 lRow, nCol, m_ulEntityKey, szMap );
   }
#endif
   }

   DisplayCellData( "ZGrid::TransferDataFromCellToOI2" );
   return( 0 );
}

BOOL CALLBACK
GridCallback( GV_DISPINFO *pDispInfo, LPARAM lParam )
{
   ZGrid *pGrid = (ZGrid *) lParam;
   if ( pDispInfo->item.lRow == 0 || pDispInfo->item.nCol == 0 )
   {
      if ( pDispInfo->item.lRow == 0 && pDispInfo->item.nCol == 0 )
      {
      }
      else
      if ( pDispInfo->item.lRow == 0 )
      {
         pDispInfo->item.csText = pGrid->m_pCol[ pDispInfo->item.nCol ].pchText;
      }
      else
      {
         pDispInfo->item.csText.Format( _T( "%d" ), pDispInfo->item.lRow );
      }
   }
   else
   {
      pDispInfo->item.csText.Format( _T( "Default %d,%d" ),
                                     pDispInfo->item.lRow, pDispInfo->item.nCol );
      pGrid->FormatTextForCell( pDispInfo->item.csText,
                                pDispInfo->item.lRow, pDispInfo->item.nCol );
   }

   return( TRUE );
}

zSHORT
ZGrid::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGrid::MapFromOI: ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZGrid::MapFromOI ", "Enabled" );
      else
         TraceLineS( "ZGrid::MapFromOI ", "Disabled" );
   }
   else
      TraceLineS( "ZGrid::MapFromOI ", "Invalid" );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   zBOOL bBubbleSort = ((m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) &&
                        (wFlag & zMAP_BY_SORT)) ? TRUE : FALSE;

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1 | zMAPACT_MAPPING_FROM_OI;
   m_EntityKeyList.RemoveAll( );
   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                    m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }

   if ( (wFlag & zMAP_BY_SORT) == 0 || m_nCols != GetColumnCount( ) )
   {
      wFlag &= ~zMAP_BY_SORT;
      m_lEntityCnt = 0;
      SetRowCount( m_nFixedRows ); // corrected later in MapFromOI
      SetColumnCount( m_nCols );   // set from CtrlBOI (includes fixed)
   }

   GetViewByName( &m_vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
   if ( m_vApp && (wFlag & zMAP_NO_SORT) == 0 )
   {
   // TraceLine( "ZGrid::MapFromOI for Control: %s   Sort Buffer: %s   Remember Sort: %s",
   //            *m_pzsTag, m_csSortBuffer, m_bRememberSort ? "True" : "False" );
      if ( m_bRememberSort && m_csSortBuffer.IsEmpty( ) == FALSE )
      {
      // if ( m_nColLastSorted >= 0 )
      // {
      //    m_SortedHeaderCtrl.SetSortImage( m_nColLastSorted,
      //                                     m_chAscendDescend == 'A' ?
      //                                                      TRUE : FALSE );
      // }

      // BubbleSort is toooo slow for anything over 1000 entities!  Plus,
      // we don't think we need it here.
      // if ( bBubbleSort )
      // {
      //    CString cs = ".bubblesort ";
      //    cs += m_csSortBuffer;
      //    int nPos = cs.Find( ']', 0 );
      //    if ( nPos >= 0 )
      //       cs.SetAt( nPos + 1, 0 );
      //
      //    OrderEntityForView( m_vApp, *m_pzsEName, cs );
      // }
      // else
         {
            TraceLine( "Grid: %s   sorting entity: %s   using sort string: %s",
                       *m_pzsTag, *m_pzsEName, m_csSortBuffer );
            OrderEntityForView( m_vApp, *m_pzsEName, m_csSortBuffer );
         }
      }
      else
      if ( m_nColLastSorted >= 0 )
      {
         m_nColLastSorted = -1;
         m_csSortBuffer = "";
      // m_SortedHeaderCtrl.SetSortImage( -1, FALSE );
      }
   }

   zLONG  lFocusRow = -1;
   zSHORT nFocusCol = -1;
   zLONG  lRow;
   int    nCol;
   zSHORT nRC;

   while ( mIs_hWnd( m_hWnd ) && m_pCol && m_vApp )  // purist's goto
   {
#ifdef DEBUG_ALL
      TraceLineS( "ZGrid::MapFromOI ", *m_pzsTag );
#endif

      SetNameForView( m_vApp, m_csViewName,
                      m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );

   // m_lEntityCnt = CountEntitiesForView( vApp, *m_pzsEName );
      if ( m_nDisplaySelSet )
         SetSelectSetForView( m_vApp, m_nDisplaySelSet );

      // If there are no entities to list ... quit.
      if ( (nRC = SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                   m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                                   0, 0, 0, 0, *m_pzsScope, 0 )) < 0 )
      {
         break;  // goto end of while
      }

      if ( (wFlag & zMAP_BY_SORT) == 0 ||
           GetViewByName( &m_vAppList, m_csViewNameList,
                          m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
      {
         wFlag &= ~zMAP_BY_SORT;
         CreateViewFromViewForTask( &m_vAppList, m_vApp,
                                    m_pZSubtask->m_vDialog );
         SetViewFromView( m_vAppList, m_vApp );
         SetNameForView( m_vAppList, m_csViewNameList,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent the OE from
                                    // automatically cleaning it up during
                                    // the deletion of an OE subtask.
      }

      // This is now done for every MapFromOI ... may be too much???
      // Just done at Setup is not sufficient!

      zCPCHAR cpcScope;
      zULONG  ulKey;
      zULONG  ulFocusKey;

      if ( nRC == 0 )
      {
        ulFocusKey = GetEntityKey( m_vApp, *m_pzsEName );
      }
      else
      {
         SetEntityCursor( m_vApp, *m_pzsEName, 0,
                          m_ulFlag | zPOS_FIRST,
                          0, 0, 0, 0, *m_pzsScope, 0 );
      }

      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      // Just to get the previous selset.
      zSHORT nPrevSelSet = SetSelectSetForView( m_vAppList, 1 );

      if ( m_nDisplaySelSet )
         SetSelectSetForView( m_vAppList, m_nDisplaySelSet );
      else
         SetSelectSetForView( m_vAppList, nPrevSelSet );

      zSHORT nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                    m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                                    cpcScope, 0 );

      zLONG lHoldEntityCnt = m_lEntityCnt;
      m_lEntityCnt = m_nFixedRows;
      while ( nRC > zCURSOR_UNCHANGED )
      {
         ulKey = GetEntityKey( m_vAppList, *m_pzsEName );
         m_EntityKeyList.SetAtGrow( m_lEntityCnt, ulKey );
         if ( ulKey == ulFocusKey )
            lFocusRow = m_lEntityCnt;

         m_lEntityCnt++;
         nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                                cpcScope, 0 );

      }  // end of:  while ( nRC > zCURSOR_UNCHANGED )

      m_lEntityCnt -= m_nFixedRows;
      if ( (wFlag & zMAP_BY_SORT) == 0 || lHoldEntityCnt != m_lEntityCnt )
      {
         wFlag &= ~zMAP_BY_SORT;
         SetRowCount( m_lEntityCnt + m_nFixedRows );
      }

      CString csText;
      zPCHAR  pch;
      zLONG   lLth;

#if 0
      LPARAM lData;
      for ( lRow = 0; lRow < m_lRows; lRow++ )
      {
         for ( nCol = 0; nCol < m_nCols; nCol++ )
         {
            lData = GetItemData( lRow, nCol );
            if ( lData )
               TraceLineX( "GetItemData: ", lData );
         }
      }
#endif

      // Loop through columns to see what type of cells should
      // be created for the attributes.
      nCol = m_nCols;
      while ( nCol > m_nFixedCols )
      {
         nCol--;

         if ( (wFlag & zMAP_BY_SORT) != 0 &&
              (m_pCol[ nCol ].ulStyle & 0x0FFF0000L) != zGRID_CHECKBOX )
         {
            continue;
         }

         // This check permits us to do the MiGetUpdate at most one time
         // for each column.
         // Sizeable borders do not make sense for a grid sub-control ...
         // so we ensure the flag is set off at conversion, and at
         // runtime we use this flag position to implement MiUpdate.

         // Alright already ... we have to recheck MiGetUpdate every time
         // we map because someone may have made a view updatable (that was
         // previously not), or vice-versa.  So code below has been
         // commented as //. to cause the recheck.

         // Reset original disabled setting.
         if ( m_pCol[ nCol ].ulSubtype & zCONTROL_SIZEABLEBORDER )
            m_pCol[ nCol ].ulSubtype |= zCONTROL_DISABLED;
         else
            m_pCol[ nCol ].ulSubtype &= ~zCONTROL_DISABLED;

      //.if ( (m_pCol[ nCol ].ulSubtype & zCONTROL_SIZEABLEBORDER) == 0 )
         {
         // if ( (m_pCol[ nCol ].ulStyle & 0x0FFF0000L) == zGRID_CHECKBOX )
         //    SetValue( nCol, 0, "" );  // turn off checkbox header text

         //.m_pCol[ nCol ].ulSubtype |= zCONTROL_SIZEABLEBORDER;
         //.m_pCol[ nCol ].ulSubtype &= ~zCONTROL_DISABLED;
         //.if ( (m_pCol[ nCol ].ulSubtype & zCONTROL_DISABLED) == 0 )
            {
               if ( (m_pCol[ nCol ].ulStyle & 0x0FFF0000L) != zGRID_ZEIDON &&
                    MiGetUpdateForViewEntityAttr( m_vAppList,
                                                  m_pCol[ nCol ].pchEntity,
                                                  m_pCol[ nCol ].pchAttrib,
                                                  zUPD_NONPERSIST ) <= 0 )
               {
                  m_pCol[ nCol ].ulSubtype |= zCONTROL_DISABLED;
               }
            }
         }

         if ( m_pCol[ nCol ].ulSubtype & zCONTROL_DISABLED )
            SetAllCellStates( nCol, GVIS_READONLY, 1 );
         else
         if ( nFocusCol == -1 || nCol < nFocusCol )
            nFocusCol = nCol;

         zUSHORT uTypeStyle = zLOUSHORT( m_pCol[ nCol ].ulSubtype );
         zULONG  ulAttrLth;

         GetAttributeDisplayLength( &ulAttrLth, m_vAppList,
                                    m_pCol[ nCol ].pchEntity,
                                    m_pCol[ nCol ].pchAttrib,
                                    m_pCol[ nCol ].pchContext );

         switch ( m_pCol[ nCol ].ulStyle & 0x0FFF0000L )
         {
            case zGRID_EDITBOX:
//             TraceLineI( "Grid editbox ", ulStyle );

               if ( uTypeStyle & zEDITBOX_UPPERCASE )
               {
                  uTypeStyle = SS_CASE_UCASE;
                  m_pCol[ nCol ].ulStyle |= ES_UPPERCASE;
               }
               else
               if ( uTypeStyle & zEDITBOX_LOWERCASE )
               {
                  uTypeStyle = SS_CASE_LCASE;
                  m_pCol[ nCol ].ulStyle |= ES_LOWERCASE;
               }
               else
               {
                  uTypeStyle = SS_CASE_NOCASE;
               }

               if ( m_pCol[ nCol ].ulSubtype & zCONTROL_DISABLED )
               {
                  // We are a static control.
                  if ( m_pCol[ nCol ].ulSubtype & zCONTROL_RIGHTJUSTIFY )
                     uTypeStyle |= ES_AUTOHSCROLL | SS_TEXT_RIGHT;
                  else
                     uTypeStyle |= ES_AUTOHSCROLL | SS_TEXT_LEFT;

                  uTypeStyle |= SS_TEXT_SHADOWIN;
               // SetTypeStaticText( &SSCellType, uTypeStyle );
               }
               else
               {
                  // We are an edit control.
                  if ( m_pCol[ nCol ].ulSubtype & zCONTROL_RIGHTJUSTIFY )
                     m_pCol[ nCol ].ulStyle = ES_AUTOHSCROLL | ES_RIGHT;
                  else
                     m_pCol[ nCol ].ulStyle = ES_AUTOHSCROLL | ES_LEFT;

               // SetTypeEdit( &SSCellType, ulStyle,
               //              (zSHORT) ulAttrLth, SS_CHRSET_CHR,
               //              uTypeStyle );
               }

            // SetCellType( nCol, lRow, &SSCellType );

            // SetValue( nCol, lRow, szMap );
            // zCHAR szMsg[ zsizeof( szMap ) + 100 ];
            //
            // sprintf_s( szMsg, zsizeof( szMsg ), "SetValue for Row: %d Col: %d %s",
            //           (zLONG) lRow, (zLONG) nCol, szMap );
            // TraceLineS( szMsg, "" );
               break;

            case zGRID_ZEIDON:
            {
               ZSubtask *pZSubtask;
               ZMapAct  *pzma;

               if ( zstrncmp( "__Draw", m_pCol[ nCol ].pchTag, 6 ) == 0 )
                  m_pCol[ nCol ].ulStyle |= zGRID_OWNERDRAW;

               if ( GetWindowAndCtrl( &pZSubtask, &pzma,
                                      m_pZSubtask->m_vDialog,
                                      m_pCol[ nCol ].pchData ) == 0 )
               {
                  if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
                  {
                     pzma->m_pzmaComposite = this;
                     pzma->m_pCtrl->SetParent( this );
                     pzma->m_pCtrl->ShowWindow( SW_HIDE );
                     pzma->m_ulMapActFlags &= ~zMAPACT_VISIBLE;
                  }

                  lRow = m_lEntityCnt + m_nFixedRows;
                  while ( lRow > m_nFixedRows )
                  {
                     lRow--;
                     ZGridCellZeidon *pCell = (ZGridCellZeidon *)
                             SetCellType( lRow, nCol,
                                          RUNTIME_CLASS( ZGridCellZeidon ) );
                     ZGridCellBase *pCellBase = GetCell( lRow, nCol );
                     if ( pCell )
                        pCell->m_pzma = pzma;
                  }

               // Invalidate( );
               }

               break;
            }

            case zGRID_CHECKBOX:
            {
            // TraceLineI( "Grid checkbox ", ulStyle );
               pch = m_pCol[ nCol ].pchData;
               lLth = sizeof( zLONG ) + *((zPLONG) pch); // skip past Text
               pch += lLth + sizeof( zLONG ); // point to "on" text

               lRow = m_lEntityCnt + m_nFixedRows;
               while ( lRow > m_nFixedRows )
               {
                  lRow--;
               // OnCellNormal( );
               // SetCellType( lRow, nCol, RUNTIME_CLASS( ZGridCell ) );
               // SetItemText( lRow, nCol, _T( "" ) );
                  ZGridCellCheck *pCell = (ZGridCellCheck *)
                     SetCellType( lRow, nCol, RUNTIME_CLASS( ZGridCellCheck ) );
                  if ( pCell )
                  {
                     pCell->SetBackClr( ::GetSysColor( COLOR_3DFACE ) );
                     pCell->SetCheck( -1, lRow, nCol );  // determine check from OI
                  }

               // Invalidate( );
               }

               break;
            }

            default:
               TraceLine( "Invalid control type specified for this Grid control 0x%08x",
                          m_pCol[ nCol ].ulStyle & 0x0FFF0000L );
               MessageBox( "Invalid control type specified for this Grid control",
                           "Grid control error" );
               break;
         }
      }

      break;  // end of: purist's goto
   }

   ZCellId cellId = GetFocusCell( );
// TraceLine( "ZGrid::MapFromOI Focus0 Row: %d  Col: %d   (%d, %d)",
//            cellId.m_lRow, cellId.m_nCol, lFocusRow, nFocusCol );
   SetFocusCell( -1, -1 );  // to force reset
   if ( lFocusRow >= 0 )
   {
      cellId.m_lRow = lFocusRow;
      if ( (wFlag & zMAP_BY_SORT) == 0 && nFocusCol >= 0 )
         cellId.m_nCol = nFocusCol;
   }

   // Set initial selection range (i.e. none).
   SetSelectedRange( -1, -1, -1, -1, FALSE, TRUE );

#if 0
   // Set state of all cells as not selected.
   UINT uState;

   lRow = m_lEntityCnt + m_nFixedRows;
   while ( lRow > m_nFixedRows )
   {
      lRow--;
      nCol = m_nCols;
      while ( nCol > m_nFixedCols )
      {
         nCol--;
         uState = GetItemState( lRow, nCol );
         if ( uState & GVIS_SELECTED )
         {
            uState &= ~GVIS_SELECTED;
            SetItemState( lRow, nCol, uState );
         }
      }
   }
#endif

   SetFocusCell( cellId );
// TraceLine( "ZGrid::MapFromOI Focus Row: %d  Col: %d",
//            cellId.m_lRow, cellId.m_nCol );

#if 0  //???
   cellId = GetFocusCell( );
   if ( cellId.m_nCol > 0 )
   {
      cellId.m_nCol = 0;
      SetFocusCell( cellId );
   }
#endif

#if 0
   lRow = m_lEntityCnt + m_nFixedRows;
   while ( lRow > m_nFixedRows )
   {
      lRow--;
      nCol = m_nCols;
      while ( nCol > m_nFixedCols )
      {
         nCol--;
         uState = GetItemState( lRow, nCol );
         if ( uState & GVIS_SELECTED )
         {
            TraceLineI( "MapFromOI selected row: ", lRow );
            break;
         }
      }
   }
#endif

   if ( cellId.m_lRow >= 0 && cellId.m_nCol >= 0 )
   {
      if ( IsValid( cellId ) )
         EnsureVisible( cellId );

      m_cellSelectionStart = cellId;
      m_MouseMode = MOUSE_SELECT_ROW;  // DKS???
      OnSelecting( cellId );
   }

#if 0
   lRow = m_lEntityCnt + m_nFixedRows;
   while ( lRow > m_nFixedRows )
   {
      lRow--;
      nCol = m_nCols;
      while ( nCol > m_nFixedCols )
      {
         nCol--;
         uState = GetItemState( lRow, nCol );
         if ( uState & GVIS_SELECTED )
         {
            TraceLineI( "MapFromOI (post OnSelecting) selected row: ", lRow );
            break;
         }
      }
   }
#endif

   int nRowHeight = GetRowHeight( 0 );
   if ( nRowHeight != m_cellDefault.GetHeight( ) )
      SetRowHeight( 0, nRowHeight );

   Invalidate( );
   m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
// TraceLineS( "ZGrid::MapFromOI ", *m_pzsTag );
   return( 0 );
}

zSHORT
ZGrid::MapToOI( zLONG lFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGrid::MapToOI: ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZGrid::MapToOI ", "Enabled" );
      else
         TraceLineS( "ZGrid::MapToOI ", "Disabled" );
   }
   else
      TraceLineS( "ZGrid::MapToOI ", "Invalid" );
#endif

   EndEditing( FALSE );  // do not destroy InPlaceEdit's
   return( 0 );
}

zLONG
ZGrid::SetZCtrlProperty( zLONG   lPropertyType,
                         zULONG  ulNumericProperty,
                         zCPCHAR cpcStringProperty )
{
   zLONG lLth;

   if ( lPropertyType == zCONTROL_PROPERTY_SUBCTRL_JUSTIFY )
   {
      UINT uFormat = DT_VCENTER | DT_SINGLELINE;
      zSHORT nCol = m_nFixedCols;
      while ( nCol < m_nCols )
      {
         if ( zstrcmp( m_pCol[ nCol ].pchTag, cpcStringProperty ) == 0 )
         {
            if ( ulNumericProperty & 1 )
            {
               uFormat |= DT_RIGHT;
               m_pCol[ nCol ].nJustify = LVCFMT_RIGHT;   // right justify
            }
            else
            if ( ulNumericProperty & 2 )
            {
               uFormat |= DT_CENTER;
               m_pCol[ nCol ].nJustify = LVCFMT_CENTER;  // center justify
            }
            else
            {
               uFormat |= DT_LEFT;
               m_pCol[ nCol ].nJustify = LVCFMT_LEFT;    // left justify
            }

            for ( long lRow = 0; lRow < m_lRows; lRow++ )
               SetItemFormat( lRow, nCol, uFormat );

            if ( mIs_hWnd( m_hWnd ) )
               Invalidate( );

            return( nCol );
         }

         nCol++;
      }

      return( -1 );
   }
   else
   if ( lPropertyType == zCONTROL_PROPERTY_INTEGER_DATA )
   {
      zSHORT nCol = m_nFixedCols;
      while ( nCol < m_nCols )
      {
         if ( zstrcmp( m_pCol[ nCol ].pchTag, cpcStringProperty ) == 0 )
         {
            m_pCol[ nCol ].nExtent = (zSHORT) ulNumericProperty;
            m_arColWidths[ nCol ] = m_pCol[ nCol ].nExtent;
            return( nCol );
         }

         nCol++;
      }

      return( -1 );
   }
   else
   if ( lPropertyType == zCONTROL_PROPERTY_TAG )
   {
      if ( ulNumericProperty > 0 )
      {
         zSHORT nCol = (zSHORT) ulNumericProperty - 1;
         mDeleteInit( m_pCol[ nCol ].pchTag );
         lLth = zstrlen( cpcStringProperty ) + 1;
         m_pCol[ nCol ].pchTag = new char[ lLth ];
         strcpy_s( m_pCol[ nCol ].pchTag, lLth, cpcStringProperty );
         return( 0 );
      }
   }

   return( ZMapAct::SetZCtrlProperty( lPropertyType, ulNumericProperty,
                                      cpcStringProperty ) );
}

zLONG
ZGrid::GetZCtrlProperty( zLONG   lPropertyType,
                         zPULONG pulNumericProperty,
                         zPCHAR  pchStringProperty,
                         zLONG   lMaxLth )
{
   if ( lPropertyType == zCONTROL_PROPERTY_COLUMN_COUNT )
   {
      return( m_nCols );
   }
   else
   if ( lPropertyType == zCONTROL_PROPERTY_TAG )
   {
      if ( *pulNumericProperty > 0 )
      {
         zSHORT nCol = (zSHORT) *pulNumericProperty - 1;
         strcpy_s( pchStringProperty, lMaxLth, m_pCol[ nCol ].pchTag );
         return( 0 );
      }
   }

   return( ZMapAct::GetZCtrlProperty( lPropertyType, pulNumericProperty,
                                      pchStringProperty, lMaxLth ) );
}

zLONG
ZGrid::SetZCtrlState( zLONG  lStatusType,
                      zULONG ulValue )
{
   // For grid controls, we don't want to actually disable if the grid is
   // to be disabled as ReadOnly.
   if ( lStatusType == zCONTROL_STATUS_ENABLED &&
        (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
   {
      if ( ulValue )
         m_ulMapActFlags |= zMAPACT_ENABLED;
      else
         m_ulMapActFlags &= ~zMAPACT_ENABLED;

      return( 0 );
   }

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

zLONG
ZGrid::SelectAll( )
{
// TraceLine( "ZGrid::SelectAll (Multi=%c): %s", m_chMultiSelect, *m_pzsTag );
   if ( m_chMultiSelect == 'Y' )
   {
      OnEditSelectAll( );
      return( m_lEntityCnt );
   }
   else
      return( 1 );
}

zLONG
ZGrid::TransferTextFromInplace( zCPCHAR cpcText )
{
   ZCellId cellId = GetFocusCell( );
// TraceLine( "ZGrid::TransferTextFromInplace: %s (Row: %d, Col: %d)",
//            *m_pzsTag, cellId.m_lRow, cellId.m_nCol );
   SetRowColText( cpcText, cellId.m_lRow, cellId.m_nCol );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Offsets for first and other columns.
// #define OFFSET_FIRST   2
// #define OFFSET_OTHER   6

zBOOL
ZGrid::PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle, zLONG lFlags,
                   zSHORT nFontSize, zCPCHAR cpcFontFace )
{
   if ( m_pZSubtask && bTest == FALSE )
   {
      zSHORT nCol;
      zLONG  lWidth;
      zBOOL  bPrompt;

      m_lPrintFlags = lFlags;
      m_nLinesPerPage = 0;
      m_nTextHeight = -1;
      for ( nCol = 0; nCol < m_nCols; nCol++ )
      {
         lWidth = GetColumnWidth( nCol );
         m_pCol[ nCol ].nColWidth = (zSHORT) mConvertPixelTo256thInchX( lWidth );
      }

      lFlags = zREPORT_REVERSE_PARM;
      lFlags |= (m_lPrintFlags & 0x00000002) ? zREPORT_LANDSCAPE : 0;
      lFlags |= (m_lPrintFlags & 0x00000004) ? zREPORT_PRINTPREVIEW : 0;
      lFlags |= (m_lPrintFlags & 0x00000010) ? zREPORT_PRINT2CSV : 0;
      bPrompt = (m_lPrintFlags & 0x00000008) ? TRUE : FALSE;

      OpenReport( m_pZSubtask->m_vDialog, "ZDRAPP", "PrintCtrlPage",
                  lFlags, TRUE, TRUE, bPrompt,
                  cpcTitle, (zPCHAR)(zCPCHAR) *m_pzsTag );
   }

   m_wndTitleTip.Hide( );
   return( TRUE );  // this ctrl can be printed!
}

void
fnFormatString( zPCHAR pchString, zCPCHAR cpcFormat );
#if 0
{
   zSHORT nLth = (zSHORT) zstrlen( pchString );
   zSHORT nFormatLth = (zSHORT) zstrlen( cpcFormat );
   zSHORT i, j, k;
   zPCHAR pchNewString = new char[ nFormatLth + 1 ];

   i = j = k = 0;
   while ( j < nFormatLth )
   {
      if ( cpcFormat[ j ] =='#' ||  // numeric
           cpcFormat[ j ] =='^' ||  // hex
           cpcFormat[ j ] =='@' ||  // alpha
           cpcFormat[ j ] =='*' )   // alphanumeric
      {
         if ( i < nLth )
            pchNewString[ k++ ] = pchString[ i++ ];
         else
            pchNewString[ k++ ] = ' ';

         j++;
      }
      else
      {
         pchNewString[ k++ ] = cpcFormat[ j++ ];
      }
   }

   pchNewString[ k ] = 0;
   strcpy_s( pchString, commented out, pchNewString );
   delete [] pchNewString;
}
#endif

#define pDC   pZSubtask->m_pZPrintout->m_pDC

void
ZGrid::GetRowBoundary( zLONG lRow, CRect& rect )
{
   zSHORT k;

   GetCellRect( lRow, m_nFixedCols, rect );
   for ( k = m_nFixedCols + 1; k < m_nCols; k++ )
   {
      rect.right += GetColumnWidth( k );
   }
}

zSHORT
ZGrid::PrintZPage( zLONG  lPageNbr,
                   zLONG  lPageWidth,
                   zLONG  lPageHeight )
{
   ZSubtask *pZSubtask;
   zCHAR   szText[ 512 ];
// zLONG   lOffset;
   zSHORT  nRC = 0;
   zLONG   lRow;
   zSHORT  nCol;
   zLONG   lJustify;
// zLONG   lLeft256ths;
// zLONG   lRight256ths;
// zLONG   lTop256ths;
// zLONG   lBottom256ths;
   CString cs;
   CSize   size;
   CFont   *pFontOld = 0;
   int     nOldMapMode;
   zBOOL   bPrintCSV;

   if ( m_pZSubtask && m_pZSubtask->m_pZTask &&
        m_pZSubtask->m_pZTask->m_pZSubtaskPrint )
   {
      pZSubtask = m_pZSubtask->GetSplitterMainSubtask( );
      if ( pZSubtask == 0 )
         pZSubtask = m_pZSubtask;
   }
   else
      pZSubtask = m_pZSubtask;

   if ( pZSubtask == 0 ||
        pZSubtask->m_pZPrintout == 0 ||
        pZSubtask->m_pZPrintout->m_pDC == 0 ||
        pZSubtask->m_pZPrintout->m_pDC->m_hDC == 0 )
   {
      return( 0 );
   }

   ZPrintout *pZPrintout = pZSubtask->m_pZPrintout;
   if ( pZPrintout->m_bPrintToCSV &&  // printing CSV file
        pZPrintout->m_lFileCSV >= 0 )
   {
      bPrintCSV = TRUE;
   }
   else
   {
      bPrintCSV = FALSE;
   }

   // -1, -2 ==> BeginPrint ... -3, -4 ==> EndPrint ... -2, -4 ==> PrintPreview
   if ( lPageNbr < 0 )
   {
      if ( lPageNbr == -1 || lPageNbr == -2 )
      {
         nOldMapMode = pDC->SetMapMode( MM_TEXT );

         m_pFontNormal = new CFont;
         m_pFontBold = new CFont;
         m_pFontItalic = new CFont;
         m_pFontUnderline = new CFont;
         m_pFontNormal->CreatePointFont( 90, "Arial", pDC );
         LOGFONT lf;
         m_pFontNormal->GetLogFont( &lf );
         lf.lfWeight = 700;
         m_pFontBold->CreateFontIndirect( &lf );
         lf.lfWeight = 0;
         lf.lfItalic = TRUE;
         m_pFontItalic->CreateFontIndirect( &lf );
         lf.lfItalic = FALSE;
         lf.lfUnderline = TRUE;
         m_pFontUnderline->CreateFontIndirect( &lf );

         m_rectDraw.SetRect( 0, 0, pDC->GetDeviceCaps( HORZRES ),
                             pDC->GetDeviceCaps( VERTRES ) );
         pDC->DPtoLP( &m_rectDraw );

         CRect rectMarginMM( 1, 1, 1, 1 );  // hardwired
         CSize sizeUp( rectMarginMM.left * 100, rectMarginMM.top * 100 );
         CSize sizeDown( rectMarginMM.right * 100, rectMarginMM.bottom * 100 );
         pDC->HIMETRICtoDP( &sizeUp );
         pDC->HIMETRICtoDP( &sizeDown );
         m_rectDraw.top  += sizeUp.cy;
         m_rectDraw.left += sizeUp.cx;
         m_rectDraw.right -= sizeDown.cx;
         m_rectDraw.bottom -= sizeDown.cy;

         pFontOld = pDC->SelectObject( m_pFontNormal );
         CSize sizeNormal = pDC->GetTextExtent( "JjQqWwYy" );
         sizeNormal.cx /= 8;
         pDC->SelectObject( m_pFontBold );
         CSize sizeBold = pDC->GetTextExtent( "JjQqWwYy" );
         sizeBold.cx /= 8;
         m_nTextHeight = (zSHORT) sizeNormal.cy;
         m_nTextHeightBold = (zSHORT) sizeBold.cy;
         m_nLinesPerPage = (m_rectDraw.Height( ) - (m_nTextHeightBold * 4)) /
                                                               m_nTextHeight;
         m_nMaxPage = (zSHORT) (GetRowCount( ) / m_nLinesPerPage) + 1;
         pDC->SetMapMode( nOldMapMode );
         pDC->SelectObject( pFontOld );
         pFontOld = 0;
      }
      else
      if ( lPageNbr == -3 || lPageNbr == -4 )
      {
         if ( bPrintCSV )
            PrintTextToCSV( m_pZSubtask->m_vDialog, 0, 0 );

         mDeleteInit( m_pFontNormal );
         mDeleteInit( m_pFontBold );
         mDeleteInit( m_pFontItalic );
         mDeleteInit( m_pFontUnderline );
      }

      return( 0 );
   }

   nOldMapMode = pDC->SetMapMode( MM_TEXT );
   pFontOld = pDC->SelectObject( m_pFontNormal );

// CPen penShadow( PS_SOLID, 1, ::GetSysColor( COLOR_GRAYTEXT ) );
   CPen penShadow( PS_SOLID, 1, RGB( 128, 128, 128 ) );
   CPen *pOldPen = pDC->SelectObject( &penShadow );

#if 0

   lPageHeight -= m_nTextHeight;
   lRow = (lPageNbr - 1) * m_nLinesPerPage;

   m_pZSubtask->m_pZFWnd->GetWindowText( cs );
   if ( cs.IsEmpty( ) )
   {
      cs = *(m_pZSubtask->m_pzsDlgTag);
      cs += ".";
      cs += *(m_pZSubtask->m_pzsDlgTag);
      cs += ".";
      cs += *(m_pZSubtask->m_pzsWndTag);
   }

   PrintTextToReport( m_pZSubtask->m_vDialog, cs, 0, 0, lPageWidth, m_nTextHeight, 96, 0x00000F00, 0, 0, 0 );

   cs = pZSubtask->m_pZPrintout->m_csReportDefName;
   cs += "   ";
   SysGetDateTime( szText );
   szText[ 14 ] = 0;
   fnFormatString( szText, "####.##.##  ##:##:##" );
   cs += szText;
   PrintTextToReport( m_pZSubtask->m_vDialog, cs, 0, 0, lPageWidth, m_nTextHeight, 100, 0, 0, 0, 0 );

   lOffset = GetTextWidth( m_pZSubtask->m_vDialog, _T( " " ) );
   lLeft256ths = 0;
   lTop256ths = 2 * m_nTextHeight;
   for ( nCol = 0; nCol < m_nCols; nCol++ )
   {
      lRight256ths = lLeft256ths + m_pCol[ nCol ].nColWidth;
      lBottom256ths = lTop256ths + m_nTextHeight;
      size.cx = m_pCol[ nCol ].nColWidth;
      size.cx = (size.cx * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &size );
      MakeShortString( pZSubtask->m_pZPrintout->m_pDC, szText, zsizeof( szText ), m_pCol[ nCol ].pchText, size.cx, 0 );
      PrintTextToReport( m_pZSubtask->m_vDialog, szText, lLeft256ths, lTop256ths, lRight256ths, lBottom256ths, 34, 0x00000F00, 0, 0, 0 );
      lLeft256ths = lRight256ths;
   }

   lRow = 0;
   while ( lRow++ < m_nLinesPerPage && nRC == 0 )
   {
      lTop256ths += m_nTextHeight;
      lLeft256ths = lOffset;
      for ( nCol = 0; nCol < m_nCols; nCol++ )
      {
         nRC = FormatTextAtPosition( szText, lRow,
                                     nCol, zsizeof( szText ) );
         if ( nRC )
            break;

      // if ( szText[ 0 ] == 'Z' && szText[ 1 ] == 'Z' )
      //    TraceLineS( "", "" );

         size.cx = m_pCol[ nCol ].nColWidth;
         size.cx = (size.cx * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
         pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &size );

         cs = szText;
         MakeShortString( pZSubtask->m_pZPrintout->m_pDC, szText, zsizeof( szText ), cs, size.cx, 0 );

         if ( m_pCol[ nCol ].nJustify == LVCFMT_RIGHT )
            lJustify = 4;
         else
      // if ( m_pCol[ nCol ].nJustify == LVCFMT_LEFT )
            lJustify = 0;

         lRight256ths = lLeft256ths + m_pCol[ nCol ].nColWidth;
         lBottom256ths = lTop256ths + m_nTextHeight;
         PrintTextToReport( m_pZSubtask->m_vDialog, szText, lLeft256ths, lTop256ths, lRight256ths, lBottom256ths, lJustify + 64 + 32, 0x00000F00, 0, 0, 0 );

         lLeft256ths = lRight256ths;
      }

      lRow++;
   }

   if ( pFontOld )
      pDC->SelectObject( pFontOld );

#else

   CRect rect( m_rectDraw );
   CRect rectMarginMM( 10, 10, 5, 10 );  // hardwired

   CSize sizeUp( rectMarginMM.left * 100, rectMarginMM.top * 100 );
   CSize sizeDown( rectMarginMM.right * 100, rectMarginMM.bottom * 100 );
   pDC->HIMETRICtoDP( &sizeUp );
   pDC->HIMETRICtoDP( &sizeDown );
   rect.top += sizeUp.cy;
   rect.left += sizeUp.cx;
   rect.right -= sizeDown.cx;
   rect.bottom -= sizeDown.cy;

   zLONG lRowCnt = GetRowCount( );

   zLONG lFirst = (lPageNbr - 1) * m_nLinesPerPage;
   zLONG lLast = lPageNbr * m_nLinesPerPage;
   if ( lLast > lRowCnt || bPrintCSV )
      lLast = lRowCnt;

   zSHORT nOldBkMode = pDC->SetBkMode( TRANSPARENT );
   pFontOld = pDC->SelectObject( m_pFontNormal );

   CSize sizeNormal = pDC->GetTextExtent( "JjQqWwYy" );
   sizeNormal.cx /= 8;
   pDC->SelectObject( m_pFontBold );
   CSize sizeBold = pDC->GetTextExtent( "JjQqWwYy" );
   sizeBold.cx /= 8;

   CRect rectItem( rect );

   rectItem.top -= m_nTextHeightBold;
   rectItem.bottom = rectItem.top;
   rectItem.top -= m_nTextHeightBold;

   m_pZSubtask->m_pZFWnd->GetWindowText( cs );
   if ( cs.IsEmpty( ) )
   {
      cs = *(m_pZSubtask->m_pzsDlgTag);
      cs += ".";
      cs += *(m_pZSubtask->m_pzsWndTag);
      cs += ".";
      cs += *m_pzsTag;
   }

   pZPrintout->RptDrawText( pDC, cs, rectItem,
                            DT_SINGLELINE | DT_VCENTER | DT_LEFT );

   if ( pZPrintout->m_csReportInfo.IsEmpty( ) )
      cs = pZPrintout->m_csReportDefName;
   else
      cs = pZPrintout->m_csReportInfo;

   cs += "   ";
   SysGetDateTime( szText, zsizeof( szText ) );
   szText[ 14 ] = 0;
   fnFormatString( szText, "####.##.##  ##:##:##" );
   cs += szText;
   pZPrintout->RptDrawText( pDC, cs, rectItem,
                            DT_SINGLELINE | DT_VCENTER | DT_RIGHT );

   rectItem = rect;
   rectItem.bottom = rectItem.top + sizeNormal.cy;

   CRect rectAllLabels;
   GetRowBoundary( lFirst, rectAllLabels );

   double dSizeFactor = ((double) rect.Width( )) / rectAllLabels.Width( );

   pDC->SelectObject( m_pFontBold );

   // CHeaderCtrl *pHeader = (CHeaderCtrl *) GetDlgItem( 0 );

   // Print the header.
   zLONG x1 = rect.left;
   zLONG x2 = rect.left;
   for ( nCol = m_nFixedCols; nCol < m_nCols; nCol++ )
   {
      if ( m_pCol[ nCol ].nJustify & LVCFMT_CENTER )
         lJustify = DT_CENTER;
      else
      if ( m_pCol[ nCol ].nJustify & LVCFMT_RIGHT )
         lJustify = DT_RIGHT;
      else
         lJustify = DT_LEFT;

      x2 += (zLONG) (dSizeFactor * GetColumnWidth( nCol ));

      CRect rectSubItem;

      rectSubItem.top = rectItem.top;
      rectSubItem.bottom = rectItem.bottom;
      rectSubItem.left = x1;
      rectSubItem.right = x2;
      x1 = x2;

      CRect rectTextItem( rectSubItem );
      rectTextItem.left += sizeNormal.cx;
      rectTextItem.right -= sizeNormal.cx;

      CString csBuf;
      GetItemText( csBuf, lFirst, nCol );

#if 0
      if ( wndHeader.pImageList && csBuf.Right( 1 ) == "_" )
      {
         zSHORT nIdx = atoi( cs );
         IMAGEINFO ii;
         if ( wndHeader.pImageList->GetImageInfo( nCol, &ii ) )
         {
            CRect rectImage( ii.rcImage );
            zSHORT x = rectSubItem.left + 1;
            zSHORT y = rectSubItem.top + 1;

            if ( rectTextItem.left < rectTextItem.right-1 )
            {
               CDC memDC;
               memDC.CreateCompatibleDC( pDC );
               CBitmap bmp;
               bmp.CreateCompatibleBitmap( pDC, rectImage.Width( ),
                                           rectImage.Height( ) );
               CBitmap *ob = memDC.SelectObject( &bmp );
               memDC.FillSolidRect( CRect( 0, 0, rectImage.Width( ),
                                           rectImage.Height( ) ),
                                           RGB( 255, 255, 255 ) );
               ImageList_DrawEx( wndHeader.pImageList->m_hImageList,
                                 nCol, memDC.m_hDC, 0, 0,
                                 rectImage.Width( ), rectImage.Height( ) - 1,
                                 m_clrBkgnd, m_clrBkgnd, ILD_TRANSPARENT );

               zSHORT dx = rectSubItem.Width( ) / rectImage.Width( );
               zSHORT dy = rectSubItem.Height( ) / rectImage.Height( );
               zSHORT dd = dx < dy ? dx : dy;
               dx = rectImage.Width( ) * dd;
               dy = rectImage.Height( ) * dd;
               x = rectSubItem.left + (rectSubItem.Width( ) - dx) / 2;
               pDC->StretchBlt( x, y, dx, dy, &memDC, 0, 0,
                                rectImage.Width( ), rectImage.Height( ), SRCCOPY );
               memDC.SelectObject( ob );
            }
         }
      }
      else
#endif
      {
         if ( bPrintCSV )
            PrintTextToCSV( m_pZSubtask->m_vDialog, csBuf, nCol + 1 );

         pZPrintout->RptDrawText( pDC, csBuf, rectTextItem,
                                  DT_SINGLELINE | DT_VCENTER |
                                    DT_CENTER | DT_END_ELLIPSIS );
      }

      pDC->SelectObject( &penShadow );
      pZPrintout->RptMoveTo( pDC, rectSubItem.right - 1, rectSubItem.bottom - 1 );
      pZPrintout->RptLineTo( pDC, rectSubItem.right - 1, rectSubItem.top - 1 );
   }

   pDC->SelectObject( &penShadow );
   pZPrintout->RptMoveTo( pDC, rectItem.right - 1, rectItem.top - 1 );
   pZPrintout->RptLineTo( pDC, rectItem.left, rectItem.top - 1 );
   pZPrintout->RptLineTo( pDC, rectItem.left, rectItem.bottom - 1 );
   pZPrintout->RptLineTo( pDC, rectItem.right - 1, rectItem.bottom - 1 );
   rectItem.OffsetRect( 0, sizeNormal.cy );

   pDC->SelectObject( m_pFontNormal );

   // Print the row data.
   for ( lRow = lFirst + m_nFixedRows; lRow <= lLast - m_nFixedRows; lRow++ )
   {
      GV_ITEM gvi;
      gvi.mask = GVIF_IMAGE | GVIF_STATE;
      gvi.lRow = lRow;
      gvi.nCol = 0;
      gvi.mask = 0xFFFF;      // get all state flags
      GetItem( &gvi );

      if ( bPrintCSV )
         PrintTextToCSV( m_pZSubtask->m_vDialog, 0, 0 );

      x1 = x2 = rect.left;
      for ( nCol = m_nFixedCols; nCol < m_nCols; nCol++ )
      {
         if ( m_pCol[ nCol ].nJustify & LVCFMT_CENTER )
            lJustify |= DT_CENTER;
         else
         if ( m_pCol[ nCol ].nJustify & LVCFMT_RIGHT )
            lJustify |= DT_RIGHT;
         else
            lJustify = DT_LEFT;

      // x2 += lvc.cx * rectItem.Width( ) / rectAllLabels.Width( );
         x2 += (zLONG) (dSizeFactor * GetColumnWidth( nCol ));

         CRect rectSubItem;

         rectSubItem.top = rectItem.top;
         rectSubItem.bottom = rectItem.bottom;
         rectSubItem.left = x1;
         rectSubItem.right = x2;
         x1 = x2;

         CRect rectTextItem( rectSubItem );
         rectTextItem.left += sizeNormal.cx;
         rectTextItem.right -= sizeNormal.cx;

         GetItemText( cs, lRow, nCol );
         if ( bPrintCSV )
            PrintTextToCSV( m_pZSubtask->m_vDialog, cs, nCol - m_nFixedCols + 1 );

#if 0
         if ( (dwFlag & fSubItemImages) &&
              (dwhImages & 1 << GetColumnIndex( nCol )) )
         {
            zSHORT nImage = atoi( cs );
            CImageList *pImageList = pItemImageList; //GetImageList( LVSIL_SMALL );
            if ( pImageList )
            {
               IMAGEINFO ii;
               pImageList->GetImageInfo( nImage, &ii );
               CRect rectImage( ii.rcImage );

               zSHORT x = rectSubItem.left + 1;
               zSHORT y = rectSubItem.top + 1;

               UINT nOvlImageMask = lvi.state & LVIS_OVERLAYMASK;
               if ( rectTextItem.left < rectTextItem.right-1 )
               {
                  CDC memDC;
                  memDC.CreateCompatibleDC( pDC );
                  CBitmap bmp;
                  bmp.CreateCompatibleBitmap( pDC, rectImage.Width( ),
                                              rectImage.Height( ) );
                  CBitmap *ob = memDC.SelectObject( &bmp );
                  memDC.FillSolidRect( CRect( 0, 0, rectImage.Width( ),
                                              rectImage.Height( ) ),
                                       RGB( 255,255,255 ) );
                  ImageList_DrawEx( pImageList->m_hImageList, nImage,
                                    memDC.m_hDC, 0, 0,
                                    rectImage.Width( ), rectImage.Height( ) - 1,
                                    m_clrBkgnd, m_clrBkgnd,
                                    ILD_TRANSPARENT | nOvlImageMask );

                  zSHORT dx = rectSubItem.Width( ) / rectImage.Width( );
                  zSHORT dy = rectSubItem.Height( ) / rectImage.Height( );
                  zSHORT dd = dx < dy ? dx : dy;
                  dx = rectImage.Width( ) * dd;
                  dy = rectImage.Height( ) * dd;
                  x = rectSubItem.left + ( rectSubItem.Width( ) - dx ) / 2;
                  pDC->StretchBlt( x, y, dx, dy, &memDC, 0, 0,
                                   rectImage.Width( ), rectImage.Height( ), SRCCOPY );

                  memDC.SelectObject( ob );
               }
            }
         }
         else
         {
            if ( NeedColExInfo( nCol ) )
            {
               LXHDREX  lxhd;

               lxhd.iNotifyCode = NTEX_ITEMINFO;
               lxhd.CtrlId = iCtrlId < 0 ? GetDlgCtrlID( ) : iCtrlId;
               lxhd.iItem = lRow;
               lxhd.iSubItem = GetColumnIndex( nCol );
               lxhd.dwFlag = 0;
               lxhd.dwItemData = GetItemData( lRow );

               SendInfoExMsg( &lxhd );

               CFont *usf = 0;
               if ( dwPrintFlag & ptUseFonts )
               {
                  if ( lxhd.dwFlag & LXHDREX::exHFont )
                  {
                     LOGFONT lf;
                     CFont *ft = CFont::FromHandle( lxhd.hfText );
                     ft->GetLogFont( &lf );
                     usf = new CFont;
                     lf.lfHeight *= 8;
                     usf->CreatePointFontIndirect( &lf, pDC );
                     pDC->SelectObject( usf );
                  }
                  else
                  if ( lxhd.dwFlag & LXHDREX::exBold )
                     pDC->SelectObject( m_pFontBold );
                  else
                  if ( lxhd.dwFlag & LXHDREX::exItalic )
                     pDC->SelectObject( m_pFontItalic );
                  else
                  if ( lxhd.dwFlag & LXHDREX::exUnderline )
                     pDC->SelectObject( m_pFontUnderline );
                  else
                     pDC->SelectObject( m_pFontNormal );
               }

               COLORREF clrOld = pDC->GetTextColor( );

               if ( dwPrintFlag & ptUseColor )
               {
                  if ( lxhd.dwFlag & LXHDREX::exTextColor )
                     pDC->SetTextColor( lxhd.crText );
               }

               pZPrintout->RptDrawText( pDC, cs, rectTextItem,
                                        DT_SINGLELINE | DT_VCENTER |
                                          lJustify | DT_END_ELLIPSIS );
               pDC->SetTextColor( clrOld );
               pDC->SelectObject( m_pFontNormal );
               if ( usf )
                  delete usf;
            }
            else
#endif
               pZPrintout->RptDrawText( pDC, cs, rectTextItem,
                                        DT_SINGLELINE | DT_VCENTER |
                                          lJustify | DT_END_ELLIPSIS );
      // }

         if ( m_lPrintFlags & 0x00000001 )
         {
            pDC->SelectObject( &penShadow );
            pZPrintout->RptMoveTo( pDC, rectSubItem.right - 1, rectSubItem.bottom - 1 );
            pZPrintout->RptLineTo( pDC, rectSubItem.right - 1, rectSubItem.top - 1 );
         }
      }

      if ( m_lPrintFlags & 0x00000001 )
      {
         pDC->SelectObject( &penShadow );
         pZPrintout->RptMoveTo( pDC, rectItem.left, rectItem.top - 1 );
         pZPrintout->RptLineTo( pDC, rectItem.left, rectItem.bottom - 1 );
         pZPrintout->RptLineTo( pDC, rectItem.right - 1, rectItem.bottom - 1 );
      }

      rectItem.OffsetRect( 0, sizeNormal.cy );
   }

// rectItem.OffsetRect( 0, sizeNormal.cy );
#if 0
   if ( (dwPrintFlag & ptListName) && csPrintName != "" )
   {
      pZPrintout->RptDrawText( pDC, csPrintName, rectItem,
                               DT_SINGLELINE | DT_LEFT | DT_TOP );
   }

   if ( dwPrintFlag & ptPageNumbers )
   {
      CString cs;
      cs.Format( "%d", pInfo->m_nCurPage );
      pDC->DrawText( cs, rectItem, DT_SINGLELINE | DT_CENTER | DT_TOP );
   }

   if ( dwPrintFlag & ptPageDate )
   {
      CTime tm = CTime::GetCurrentTime( );
      CString cs;
      cs.Format( "%02d/%02d/%d", tm.GetDay( ),
                 tm.GetMonth( ), tm.GetYear( ) );
      pZPrintout->RptDrawText( pDC, cs, rectItem,
                               DT_SINGLELINE | DT_RIGHT | DT_TOP );
   }
#endif

#endif

   // if ( lPageNbr > 1 )
   {
      zCHAR  szPageNbr[ 32 ];
      zSHORT k;

      _ltoa_s( lPageNbr, szPageNbr, zsizeof( szPageNbr ), 10 );
      k = (zSHORT) zstrlen( szPageNbr );
      szPageNbr[ k ] = '/';
      _ltoa_s( m_nMaxPage, szPageNbr + k + 1, zsizeof( szPageNbr ) - k - 1, 10 );
      rectItem = rect;
      rectItem.bottom += 3 * m_nTextHeightBold;
      rectItem.top = rectItem.bottom;
      rectItem.bottom += m_nTextHeightBold;
      pDC->SelectObject( m_pFontBold );
      pZPrintout->RptDrawText( pDC, szPageNbr, rectItem,
                               DT_SINGLELINE | DT_VCENTER | DT_CENTER );
   }

   // Restore the pen, font, map mode, and background mode.
   pDC->SelectObject( pOldPen );
   pDC->SelectObject( pFontOld );
   pDC->SetMapMode( nOldMapMode );
   pDC->SetBkMode( nOldBkMode );

   if ( nRC == 0 && lRow < m_lRows &&
        FormatTextForCell( cs, lRow + 1, 0 ) == 0 )
   {
      nRC = 1;     // more pages
   }
   else
   {
      nRC = 0;
   }

   return( nRC );
}

#undef pDC

void
ZGrid::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      CWnd::OnSetFocus( pWndLostFocus );
      ZCellId cellId = GetFocusCell( );
      InvalidateCellRect( cellId );
   }
}

void
ZGrid::OnKillFocus( CWnd *pWndGetFocus )
{
   if ( pWndGetFocus != m_pZeidonWnd )
   {
      EndEditing( );      // destroy any InPlaceEdit's
      m_pZSubtask->InformDriverOfKillFocus( this );
      CWnd::OnKillFocus( pWndGetFocus );
      ZCellId cellId = GetFocusCell( );
      InvalidateCellRect( cellId );
   }
}

zBOOL
ZGrid::InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGrid InformCompositeOfKillFocus pzma = ",
               *(pzmaLosingFocus->m_pzsTag) );
#endif

   if ( pzmaLosingFocus->m_ulMapActFlag2 & zMAPACT_INPLACE_CTRL )
   {
      zCHAR   szText[ 65534 ];

      GetZCtrlText( pzmaLosingFocus, szText, zsizeof( szText ) );
      pzmaLosingFocus->m_pzmaComposite->TransferTextFromInplace( szText );

      pzmaLosingFocus->m_ulMapActFlags &= ~zMAPACT_VISIBLE;
      pzmaLosingFocus->m_ulMapActFlag2 &= ~zMAPACT_INPLACE_VISIBLE;
      if ( pzmaLosingFocus->m_pWndLastFocus && ::IsWindow( pzmaLosingFocus->m_pWndLastFocus->m_hWnd ) )
         pzmaLosingFocus->m_pWndLastFocus->SetFocus( );

      pzmaLosingFocus->m_pWndLastFocus = 0;
      pzmaLosingFocus->MapToOI( 0 );
      pzmaLosingFocus->m_pCtrl->SetParent( pzmaLosingFocus->m_pWndParent );
      pzmaLosingFocus->m_pCtrl->ShowWindow( SW_HIDE );
   }

   EndEditing( );      // destroy any InPlaceEdit's
   return( TRUE );
}

zSHORT
ZGrid::SetRowColText( zCPCHAR cpcText, zLONG lRow, zSHORT nCol )
{
#if 0
   // Fill header row with text.
   int nColCnt = GetColumnCount( )

   for ( zSHORT nCol = 0; nCol < nColCnt; nCol++ )
   {
      GV_ITEM Item;

      Item.mask = GVIF_TEXT;
      Item.lRow = 0;
      Item.nCol = nCol;
      Item.csText = m_pCol[ nCol ].pchText;
      SetItem( &Item );
   }
#endif

   if ( nCol < m_nCols && nCol >= 0 )
   {
      return( SetItemText( lRow, nCol, cpcText ) );
   }

   return( 0 );
}

zSHORT
ZGrid::GetRowColText( zPCHAR pchText, zUSHORT uLth,
                      zLONG lRow, zSHORT nCol )
{
   pchText[ 0 ] = 0;
   if ( nCol < m_nCols && nCol >= 0 )
   {
      CString csText;
      GetItemText( csText, lRow, nCol );
      strncat_s( pchText, uLth, csText, uLth - 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( ZGridCellZeidon, ZGridCell )

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////

// Create a control to do the editing.
BOOL
ZGridCellZeidon::Edit( long lRow, int nCol, CRect rect,
                       CPoint pt, UINT uId, UINT uChar )
{
   CString cs;
   if ( SetEditing( TRUE ) == FALSE )
      return( FALSE );

   // CInPlaceEdit auto-deletes itself.
   GetText( cs, lRow, nCol );

   if ( m_pGrid && m_pzma && m_pzma->m_pCtrl &&
        mIs_hWnd( m_pzma->m_pCtrl->m_hWnd ) )
   {
      zULONG ulKey = m_pGrid->m_EntityKeyList[ lRow ];
      if ( ulKey != GetEntityKey( m_pGrid->m_vAppList,
                                  *(m_pGrid->m_pzsEName) ) &&
           SetEntityCursor( m_pGrid->m_vAppList,
                            *(m_pGrid->m_pzsEName), 0,
                            m_pGrid->m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &ulKey,
                            0, 0, 0, *(m_pGrid->m_pzsScope), 0 ) < zCURSOR_SET )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ),
                   "Entity Key Error: %s for %s.%s.%s",
                   (*(m_pGrid->m_pzsVName)).GetString(),
                   (*(m_pGrid->m_pZSubtask->m_pzsDlgTag)).GetString(),
                   (*(m_pGrid->m_pZSubtask->m_pzsWndTag)).GetString(),
                   (*(m_pzma->m_pzsTag)).GetString() );
         TraceLineS( "ZGridCellZeidon::Edit ", szMsg );
         return( FALSE );
      }

      int nHeight = m_pzma->Attr.H;
      int nWidth = m_pzma->Attr.W;

      if ( (m_pzma->m_ulMapActFlags & zMAPACT_GRIDCOL_SETWIDTH) || // set ctrl width
           (m_pzma->m_ulMapActFlags & zMAPACT_GRIDCOL_MINWIDTH) )  // min ctrl width
      {
         nWidth = rect.right - rect.left;
         if ( m_pzma->m_ulMapActFlags & zMAPACT_GRIDCOL_MINWIDTH ) // min ctrl width
         {
            if ( nWidth < m_pzma->Attr.W )
               nWidth = m_pzma->Attr.W;
         }
      }

      if ( (m_pzma->m_ulMapActFlags & zMAPACT_GRIDCOL_SETHEIGHT) || // set ctrl height
           (m_pzma->m_ulMapActFlags & zMAPACT_GRIDCOL_MINHEIGHT) )  // min ctrl height
      {
         nHeight = rect.bottom - rect.top;
         if ( m_pzma->m_ulMapActFlags & zMAPACT_GRIDCOL_MINHEIGHT ) // min ctrl height
         {
            if ( nHeight < m_pzma->Attr.H )
               nHeight = m_pzma->Attr.H;
         }
      }

      m_pzma->SetMapViewFromView( m_pGrid->m_vAppList,
                                  *(m_pGrid->m_pzsEName), TRUE );
      m_pzma->m_pCtrl->SetParent( m_pGrid );
      m_pGrid->m_pZeidonWnd = m_pzma->m_pCtrl;
      m_pzma->m_ulMapActFlags |= zMAPACT_VISIBLE;
      m_pzma->MapFromOI( );
      m_pzma->m_pCtrl->SetWindowPos( &CWnd::wndTop,
                                     rect.left, rect.top,
                                     nWidth, nHeight,
                                     SWP_SHOWWINDOW );
      m_pzma->m_pCtrl->SetFocus( );
   }

   return( TRUE );
}

// Cancel the editing.
void
ZGridCellZeidon::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCellZeidon::EndEdit Destroy: ", bDestroy ? "TRUE" : "FALSE" );
#endif
   if ( m_pGrid && m_pGrid->m_pZSubtask &&
        m_pGrid->m_pZSubtask->IsValidCtrl( m_pzma ) &&
        m_pzma->m_pCtrl && mIs_hWnd( m_pzma->m_pCtrl->m_hWnd ) )
   {
#if 0
      zULONG ulKey = m_EntityKeyList[ lRow ];
      if ( SetEntityCursor( m_pGrid->m_vAppList,
                            *(m_pGrid->m_pzsEName), 0,
                            m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &ulKey,
                            0, 0, 0, *m_pzsScope, 0 ) != 0 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ),
                   "Entity Key Error: %s for %s.%s.%s",
                   *(m_pGrid->m_pzsVName),
                   *(m_pGrid->m_pZSubtask->m_pzsDlgTag),
                   *(m_pGrid->m_pZSubtask->m_pzsWndTag),
                   *(m_pzma->m_pzsTag) );
         TraceLineS( "ZGridCellZeidon::EndEdit ", szMsg );
      }
      else
#endif
      {
         if ( m_pGrid->m_pZeidonWnd )
         {
            if ( bDestroy && m_pGrid->m_pZeidonWnd != m_pzma->m_pCtrl &&
                 mIs_hWnd( m_pGrid->m_pZeidonWnd->m_hWnd ) )
            {
               m_pGrid->m_pZeidonWnd->ShowWindow( SW_HIDE );
            // m_pGrid->m_pZeidonWnd->UpdateWindow( );
            }

            m_pGrid->m_pZeidonWnd = 0;
         }

         if ( MiValidView( m_pGrid->m_vAppList ) &&
              CheckEntityExistence( m_pGrid->m_vAppList, *(m_pGrid->m_pzsEName) ) == 0 )
         {
            m_pzma->SetMapViewFromView( m_pGrid->m_vAppList,
                                        *(m_pGrid->m_pzsEName), FALSE );
            m_pzma->MapToOI( );
         }

         if ( bDestroy )
         {
            m_pzma->m_ulMapActFlags &= ~zMAPACT_VISIBLE;
            m_pzma->m_pCtrl->SetWindowPos( &CWnd::wndTop,
                                           m_rect.left, m_rect.top,
                                           m_rect.right, m_rect.bottom,
                                           SWP_HIDEWINDOW | SWP_NOSIZE |
                                                                SWP_NOMOVE );
         // m_pGrid->UpdateWindow( );
         }

      // m_pGrid->SetFocus( );
      }
   }

   if ( bDestroy )
      SetEditing( FALSE );
}

BOOL
ZGridCellZeidon::Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                       BOOL bEraseBkgnd, BOOL bDrawText /*=TRUE*/ )
{
// TraceLine( "GridCell::Draw Row: %d   Col: %d", lRow, nCol );
   zBOOL bRC = FALSE;

   // As good a place as any to store the bounds of the rect.
   m_rect = rect;

   if ( m_pGrid && m_pGrid->m_pCol && mIs_hWnd( m_pGrid->m_hWnd ) && m_pzma &&
        (m_pGrid->m_pCol[ nCol ].ulStyle & zGRID_OWNERDRAW) != 0 )
   {
      HFONT hFont = (HFONT) GetStockObject( DEFAULT_GUI_FONT );
      if ( hFont == 0 )
         hFont = (HFONT) GetStockObject( ANSI_VAR_FONT );

      CFont *pFontOld = pDC->SelectObject( CFont::FromHandle( hFont ) );
      bRC = m_pzma->DrawZCtrl( pDC, rect );
      pDC->SelectObject( pFontOld );
   }

   if ( bRC == FALSE )
      bRC = ZGridCell::Draw( pDC, lRow, nCol, rect, bEraseBkgnd, bDrawText );

   return( bRC );
}

// virtual
BOOL
ZGridCellZeidon::WantKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// if ( uChar == VK_SPACE )
// {
//    CRect rect = GetCheckPlacement( );
//    CPoint pt( (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 );
//    pt -= m_rect.TopLeft( );

//    OnClick( pt );
// }

// return( TRUE );
   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef _NOANCHOR
AFX_EXT_API
CWnd * OPERATION
SS( ZSubtask *pZSubtask,
    CWnd     *pWndParent,
    ZMapAct  *pzmaComposite,
    zVIEW    vDialog,
    zSHORT   nOffsetX,
    zSHORT   nOffsetY,
    zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   if ( pCtrlDef->StyleX & zSS_NEWGRID )
   {
      return( Grid( pZSubtask, pWndParent,
              pzmaComposite, vDialog,
              nOffsetX, nOffsetY, pCtrlDef ) );
   }

   return( 0 );
}
#endif

//./ ADD NAME=GRID_SortByColumn
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SortByColumn
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the current sort column for a listbox
//              control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//              lColumn     - The number of the column on which to sort
//                            (0 ==> resort using the current column)
//              nAscending  - -1 ==> invert current sort order
//                             0 ==> retain current sort order
//                             1 ==> sort ascending
//                             2 ==> sort descending
//
//  RETURNS:    0 - Sort successful
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_SortByColumn( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zSHORT  lColumn,
                   zSHORT  nAscending )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->SortByColumn( lColumn, nAscending );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SortByColumn ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_GetSortOrder
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_GetSortOrder
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation gets the current sort order for a listbox
//              control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//              pchReturnOrder - The current sort order string.
//              nMaxReturnLth - Maximum return length (including null term)
//
//  RETURNS:    0 - Returned current sort order successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_GetSortOrder( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zPCHAR  pchReturnOrder,
                   zSHORT  nMaxReturnLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   pchReturnOrder[ 0 ] = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         if ( pGrid->m_csSortBuffer.IsEmpty( ) )
            pGrid->GetPreferencesFromIni( );

         strncpy_s( pchReturnOrder, nMaxReturnLth, pGrid->m_csSortBuffer, nMaxReturnLth - 1 );
         pchReturnOrder[ nMaxReturnLth ] = 0;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_GetSortOrder ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetSortOrder
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetSortOrder
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the current sort order for a listbox
//              control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//              cpcNewSortOrder - The new sort order string.
//
//  RETURNS:    0 - Sort order set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GRID_SetSortOrder( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zCPCHAR cpcNewSortOrder )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->m_csSortBuffer = cpcNewSortOrder;
         pGrid->MapFromOI( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetSortOrder ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetSelectedPosition
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetSelectedPosition
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation moves the currently selected item
//              to the specified display position.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//              lDisplayPos - The display position.
//
//  RETURNS:    0 - Display position set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GRID_SetSelectedPosition( zVIEW   vSubtask,
                          zCPCHAR cpcCtrlTag,
                          zLONG   lDisplayPos )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->SetSelectedPosition( lDisplayPos );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetSelectedPosition ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetColorAttributeName
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetColorAttributeName
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the attribute name to use to control
//              the color of the grid lines.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//              cpcAttributeName - The attribute name used to control color.
//              lColumn     - Column number (0 ==> all columns)
//
//  RETURNS:    0 - Attribute name set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GRID_SetColorAttributeName( zVIEW   vSubtask,
                            zCPCHAR cpcCtrlTag,
                            zCPCHAR cpcAttributeName,
                            zLONG   lColumn )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )             // change dks 2013.11.09 to prevent crash
      {
         if ( lColumn > 0 && lColumn <= pGrid->GetColumnCount( ) )
         {
            pGrid->SetColorAttribute( cpcAttributeName, (zSHORT) lColumn - 1 );
            return( 0 );
         }
         else
         if ( lColumn == 0 )
         {
            lColumn = pGrid->GetColumnCount( );
            while ( lColumn > 0 )
            {
               lColumn--;
               pGrid->SetColorAttribute( cpcAttributeName, (zSHORT) lColumn );
            }

            return( 0 );
         }
         else
         {
            TraceLineI( "drvr - Invalid column number for GRID_SetColorAttributeName ",
                        lColumn );
         }
      }
      else
      {
         TraceLineS( "drvr - Invalid control type for GRID_SetColorAttributeName ",
                     cpcCtrlTag );
      }
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetColorAssociation
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetColorAssociation
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the associations between the value of
//              the (color control) attribute and the color to be displayed.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//              lColumn     - Column number (0 ==> all columns;
//                                          -1 ==> all first muli-columns;
//                                          -2 ==> all second multi-columns)
//              cpcAttributeValue - The value of the attribute corresponding
//                                  to the specified color.
//              lClrText    - The RGB color in which text is displayed.
//              lClrBack    - The RGB color in which to display background.
//              lFlags      - 1=Bold 2=Italic 4=Underline 8=Strikeout
//                            0x10000000=numeric test
//                            0x00800000=.ALWAYS_EQUAL. test always equal
//                            0x01000000=.LT.  less than
//                            0x02000000=.LE.  less than or equal
//                            0x00000000=.EQ.  equal (default)
//                            0x08000000=.GT.  greater than
//                            0x04000000=.GE.  greater than or equal
//                            0x00400000=.NE.  not equal
//                            0x00200000=.NZ.  null or zero
//                            0x00100000=.NN.  not null/zero
//                            0x00080000=.IN.  in (contains)
//                            0x00040000=.NI.  not in (does not contain)
//                            0x00020000=.SW.  starts with
//                            0x00010000=.NS.  does not start with
//
//  RETURNS:    0 - Color association set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
GRID_SetColorAssociation( zVIEW   vSubtask,
                          zCPCHAR cpcCtrlTag,
                          zLONG   lColumn,
                          zCPCHAR cpcAttributeValue,
                          zLONG   lClrText,
                          zLONG   lClrBack,
                          zLONG   lFlags )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         if ( pGrid->GetFixedColumnCount( ) == 0 )
            lColumn--;

         pGrid->SetColorAssociation( lColumn, cpcAttributeValue,
                                     lClrText, lClrBack, lFlags );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetColorAssociation ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_RemoveColorAssociations
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_RemoveColorAssociation
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation removes color associations for a grid control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the grid control.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Color associations removed successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
GRID_RemoveColorAssociations( zVIEW   vSubtask,
                              zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->RemoveColorAssociations( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_RemoveColorAssociations ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetCheckTextAttribute
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetCheckTextAttribute
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the displayable text attribute for
//              a CheckBox column control.  If this attribute is not set,
//              the checkbox appears with no text.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control containing the checkbox.
//              cpcCtrlTag - The unique name of the control.
//              lColumn    - Checkbox column number.
//              cpcTextEntity    - Entity on which attribute is defined (may be
//                                 null in which case the Grid entity is used).
//              cpcTextAttribute - Attribute to display with checkbox.
//              cpcTextContext   - Context of attribute to display with checkbox.
//
//  RETURNS:    0 - Attribute set successfully
//             -1 - Error locating control
//             -2 - Column is not a CheckBox type
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
GRID_SetCheckAttribute( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zLONG   lCol,
                        zCPCHAR cpcTextEntity,
                        zCPCHAR cpcTextAttribute,
                        zCPCHAR cpcTextContext )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zLONG    lLth;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         if ( pGrid->GetFixedColumnCount( ) == 0 )
            lCol--;

         if ( (pGrid->m_pCol[ lCol ].ulStyle & 0x0FFF0000L) ==
                                                      zGRID_CHECKBOX )
         {
            mDeleteInitA( pGrid->m_pCol[ lCol ].pchEntityX );
            mDeleteInitA( pGrid->m_pCol[ lCol ].pchAttribX );
            mDeleteInitA( pGrid->m_pCol[ lCol ].pchContextX );
            if ( cpcTextEntity && *cpcTextEntity )
            {
               lLth = zstrlen( cpcTextEntity ) + 1;
               pGrid->m_pCol[ lCol ].pchEntityX = new char[ lLth ];
               strcpy_s( pGrid->m_pCol[ lCol ].pchEntityX, lLth, cpcTextEntity );
            }

            if ( cpcTextAttribute && *cpcTextAttribute )
            {
               lLth = zstrlen( cpcTextAttribute ) + 1;
               pGrid->m_pCol[ lCol ].pchAttribX = new char[ lLth ];
               strcpy_s( pGrid->m_pCol[ lCol ].pchAttribX, lLth, cpcTextAttribute );
            }

            if ( cpcTextContext && *cpcTextContext )
            {
               lLth = zstrlen( cpcTextContext ) + 1;
               pGrid->m_pCol[ lCol ].pchContextX = new char[ lLth ];
               strcpy_s( pGrid->m_pCol[ lCol ].pchContextX, lLth, cpcTextContext );
            }

            pGrid->Invalidate( );
            return( 0 );
         }

         return( -2 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetCheckAttribute ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetRowColumnColors
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetColumnColors
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the foreground and background colors for
//              the cell at the specified row/column.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the gird control.
//              cpcCtrlTag - The unique name of the control.
//              lRow       - Row number (0 for header row).
//              lCol       - Column number (0 for header column).
//              clrText    - Text color (-1 for default).
//              clrBack    - Text background color (-1 for default).
//
//  RETURNS:    0 - Colors set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
GRID_SetRowColumnColors( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         zLONG   lRow,
                         zLONG   lColumn,
                         zLONG   clrText,
                         zLONG   clrBack )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         lColumn -= pGrid->GetFixedColumnCount( );
         if ( clrText == -1 )
            clrText = CLR_DEFAULT;

         if ( clrBack == -1 )
            clrBack = CLR_DEFAULT;

         GV_ITEM Item;

         Item.mask = (GVIF_BKCLR | GVIF_FGCLR);
         Item.lRow = lRow;
         Item.nCol = lColumn;
         Item.clrBk = clrBack;    // or - SetItemBkColor( lRow, nCol, clrTextBk );
         Item.clrFg = clrText;    // or - SetItemFgColor( lRow, nCol, clrText );
         pGrid->SetItem( &Item );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetRowColumnColors ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetColumnCtrl
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetColumnCtrl
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the control for the specified column.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the gird control.
//              cpcCtrlTag - The unique name of the control.
//              lColumn    - Column number (0 for header column).
//              cpcSubCtrlTag - The unique name of the sub-control.
//              lFlag      - 0 ==> do not reset ctrl width and height
//                           1 ==> set ctrl width to column size
//                           2 ==> set ctrl height to column size
//                           4 ==> set ctrl width to column size (do not set
//                                 smaller than ctrl's design width).
//                           8 ==> set ctrl height to column size (do not set
//                                 smaller than ctrl's design height).
//
//  RETURNS:    0 - Control set successfully
//             -1 - Error locating control or sub-control
//             -2 - Invalid column number
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
GRID_SetColumnCtrl( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag,
                    zLONG   lColumn,
                    zCPCHAR cpcSubCtrlTag,
                    zLONG   lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid &&
           GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcSubCtrlTag ) == 0 )
      {
         if ( lColumn > 0 && lColumn <= pGrid->GetColumnCount( ) + pGrid->GetFixedColumnCount( ) )
         {
            ZGridCellBase *pCellBase;
            CRuntimeClass *pRuntime;
            ZGridCellZeidon *pCell;
            zLONG lRow = pGrid->m_lEntityCnt + pGrid->GetFixedRowCount( );
            zSHORT nCol = (zSHORT) ((pGrid->GetFixedColumnCount( ) == 0) ?
                                                      lColumn - 1 : lColumn);

            while ( lRow > pGrid->GetFixedRowCount( ) )
            {
               lRow--;
               pCellBase = pGrid->GetCell( lRow, nCol );
               pRuntime = pCellBase->GetRuntimeClass( );
               if ( zstrcmp( pRuntime->m_lpszClassName, "ZGridCellZeidon" ) )
               {
                  pCell = (ZGridCellZeidon *)
                      pGrid->SetCellType( lRow, nCol,
                                          RUNTIME_CLASS( ZGridCellZeidon ) );
               }
               else
               {
                  pCell = (ZGridCellZeidon *) pCellBase;
               }

               if ( pCell )
                  pCell->m_pzma = pzma;
            }

            if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
            {
               pzma->m_pzmaComposite = pGrid;
               pzma->m_pCtrl->SetParent( pGrid );
               pzma->m_pCtrl->ShowWindow( SW_HIDE );
               pzma->m_ulMapActFlags &= ~zMAPACT_VISIBLE;
            }

#if 0 // debugging only
            CString csText;
            ZCellId cellId = pGrid->GetFocusCell( );
            pGrid->GetItemText( csText, cellId.m_lRow, nCol );
            SetZCtrlText( pzma, csText );
            TraceLine( "SetColumnCtrl setting ctrl: %s  to have text: %s",
                       (zCPCHAR) *(pzma->m_pzsTag), csText );
#endif
            pzma->m_ulMapActFlags &= ~zMAPACT_VISIBLE;
            pGrid->m_pCol[ nCol ].ulStyle |= zGRID_ZEIDON;
            mDeleteInitA( pGrid->m_pCol[ nCol ].pchData );
            zLONG lLth = zstrlen( cpcSubCtrlTag ) + 1;
            pGrid->m_pCol[ nCol ].pchData = new char[ lLth ];
            strcpy_s( pGrid->m_pCol[ nCol ].pchData, lLth, cpcSubCtrlTag );

            // Stealing flags that have no meaning if set as a column control.
            pzma->m_ulMapActFlags &= ~(zMAPACT_GRIDCOL_SETWIDTH |
                                       zMAPACT_GRIDCOL_SETHEIGHT |
                                       zMAPACT_GRIDCOL_MINWIDTH |
                                       zMAPACT_GRIDCOL_MINHEIGHT);

            if ( lFlag & 0x00000001 )  // set ctrl width
               pzma->m_ulMapActFlags |= zMAPACT_GRIDCOL_SETWIDTH;

            if ( lFlag & 0x00000002 )  // set ctrl height
               pzma->m_ulMapActFlags |= zMAPACT_GRIDCOL_SETHEIGHT;

            if ( lFlag & 0x00000004 )  // min ctrl width
               pzma->m_ulMapActFlags |= zMAPACT_GRIDCOL_MINWIDTH;

            if ( lFlag & 0x00000008 )  // min ctrl height
               pzma->m_ulMapActFlags |= zMAPACT_GRIDCOL_MINHEIGHT;

            pzma->m_ulMapActFlag2 |= zMAPACT_INPLACE_CTRL;
            return( 0 );
         }
         else
            return( -2 );
      }
      else
      if ( pGrid )
      {
         TraceLine( "drvr - GRID_SetColumnCtrl cannot find Ctrl: %s ... %s "
                      "may not be created yet or may be outside of the grid "
                      "control's (%s) group",
                    cpcSubCtrlTag, cpcSubCtrlTag, cpcCtrlTag );
      }
      else
      {
         TraceLineS( "drvr - Invalid control type for GRID_SetColumnCtrl ",
                     cpcCtrlTag );
      }
   }

   return( -1 );
}

//./ ADD NAME=GRID_LockColumn
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_LockColumn
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation locks/unlocks a grid column.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              nColNbr    - The number of the column to be enabled/disabled
//              bEnable    - TRUE -  lock the column
//                           FALSE - unlock the column
//
//  RETURNS:    0 - Control column locked\unlocked
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_LockColumn( zVIEW    vSubtask,
                 zCPCHAR  cpcCtrlTag,
                 zSHORT   nColNbr,
                 zSHORT   bLock )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_LockColumn ", (zLONG) vSubtask );
#endif
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pSS = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pSS )
      {
         zSHORT nRC = 0; //?pSS->LockColumn( nColNbr, (zBOOL) bLock );
         return( nRC );
      }

      TraceLineS( "drvr - Invalid control type for GRID_LockColumn ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_GetActiveCell
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_GetActiveCell
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation gets the active grid row and column.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              plActiveRow   - Pointer to (return) active row number
//              plActiveCol   - Pointer to (return) active column number
//
//  RETURNS:    0 - Active cell located
//              1 - No current active data cell
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_GetActiveCell( zVIEW    vSubtask,
                    zCPCHAR  cpcCtrlTag,
                    zPLONG   plActiveRow,
                    zPLONG   plActiveCol )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_GetActiveCell ", (zLONG) vSubtask );
#endif

   *plActiveRow = *plActiveCol = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         ZCellId cellId = pGrid->GetFocusCell( );
         *plActiveRow = cellId.m_lRow - pGrid->GetFixedRowCount( ) + 1;
         *plActiveCol = cellId.m_nCol - pGrid->GetFixedColumnCount( ) + 1;
         if ( *plActiveRow <= 0 || *plActiveCol <= 0 )
            return( 1 ); // can't retrieve the active cell or not in data cell
         else
            return( 0 ); // we are in a data cell
      }

      TraceLineS( "drvr - Invalid control type for GRID_GetActiveCell ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetActiveCell
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetActiveCell
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation gets the active grid row and column.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              plActiveRow   - Pointer to (return) active row number
//              plActiveCol   - Pointer to (return) active column number
//
//  RETURNS:    0 - Active cell located
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_SetActiveCell( zVIEW    vSubtask,
                    zCPCHAR  cpcCtrlTag,
                    zLONG    lActiveRow,
                    zLONG    lActiveCol )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_SetActiveCell ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->SetFocusCell( lActiveRow, lActiveCol );
         return( 0 ); // we are in a data cell
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetActiveCell ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_GetPasteCellAndText
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_GetPasteCellAndText
//                                           2006/10/02 Modified: 2006/10/02
//
//  PURPOSE:    This operation gets the text and the grid cell (row/column).
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              plRowNbr   - return row number
//              plColNbr   - return column number
//              pchText    - return text
//              nMaxTextLth - maximum length of returned text
//
//  RETURNS:    -1 - Error locating control
//               0 - data returned successfully
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zLONG OPERATION
GRID_GetPasteCellAndText( zVIEW    vSubtask,
                          zCPCHAR  cpcCtrlTag,
                          zPLONG   plRowNbr,
                          zPLONG   plColNbr,
                          zPCHAR   pchText,
                          zSHORT   nMaxTextLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_GetPasteCellAndText ", (zLONG) vSubtask );
#endif

   pchText[ 0 ] = 0; // initialize here for strncat_s
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         zGRID_CELL *pCell = (zGRID_CELL *) pZSubtask->m_pEventParms;
         *plRowNbr = pCell->lRow;
         *plColNbr = pCell->nCol;
         strncat_s( pchText, nMaxTextLth, pCell->pchText, nMaxTextLth - 1 );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_GetPasteCellAndText ", cpcCtrlTag );
   }

   *plRowNbr = -1;
   *plColNbr = -1;
   return( -1 );
}

//./ ADD NAME=GRID_GetCellData
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_GetCellData
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation gets the data in grid cell (row/column).
//              If lRowNbr and lColNbr are set to -1, use the current
//              settings from the Subtask.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              lRowNbr    - row number
//              lColNbr    - column number
//
//  RETURNS:    -1 - Error locating control
//              otw - pointer to data
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
GRID_GetCellData( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zLONG   lRowNbr,
                  zLONG   lMaxLth,
                  zLONG   lColNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_GetCellData ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         if ( lRowNbr == -1 && lColNbr == -1 )
         {
            zGRID_CELL *pCell = (zGRID_CELL *) pZSubtask->m_pEventParms;
            lRowNbr = pCell->lRow;
            lColNbr = pCell->nCol;
         }

         return( pGrid->GetItemData( lRowNbr, lColNbr ) );
      }

      TraceLineS( "drvr - Invalid control type for GRID_GetCellData ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetCellData
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetCellData
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the data in grid cell (row/column).
//              If lRowNbr and lColNbr are set to -1, use the current
//              settings from the Subtask.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              pchData    - The data to set in the specified cell
//              lRowNbr    - row number
//              lColNbr    - column number
//
//  RETURNS:     0 - Data set
//               1 - Data not set
//              -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
GRID_SetCellData( zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag,
                  zPCHAR   pchData,
                  zLONG    lRowNbr,
                  zLONG    lColNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_SetCellData ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         if ( lRowNbr == -1 && lColNbr == -1 )
         {
            zGRID_CELL *pCell = (zGRID_CELL *) pZSubtask->m_pEventParms;
            lRowNbr = pCell->lRow;
            lColNbr = pCell->nCol;
         }

         return( pGrid->SetItemData( lRowNbr, lColNbr, (LPARAM) pchData ) );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetCellData ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetGridLines
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetGridLines
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets/removes grid lines in a grid control.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the grid control.
//              cpcCtrlTag - The unique name of the control.
//              pchData    - The data to set in the specified cell
//              bHorizontal - TRUE=set horizontal lines; FALSE=otw
//              bVertical   - TRUE=set vertical lines; FALSE=otw
//
//  RETURNS:     0 - Grid lines set
//              -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_SetGridLines( zVIEW    vSubtask,
                   zCPCHAR  cpcCtrlTag,
                   zSHORT   bHorizontal,
                   zSHORT   bVertical )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GRID_SetCellData ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         int nGridLines = pGrid->GetGridLines( );
         if ( bVertical )
            nGridLines |= GVL_VERT;
         else
            nGridLines &= ~GVL_VERT;

         if ( bHorizontal )
            nGridLines |= GVL_HORZ;
         else
            nGridLines &= ~GVL_HORZ;

         pGrid->SetGridLines( nGridLines );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetGridLines ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetDisplaySelectSet
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetDisplaySelectSet
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the current display select set for
//              a grid control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nSelectSet  - The select set for the grid to display
//
//  RETURNS:    0 - Select set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GRID_SetDisplaySelectSet( zVIEW   vSubtask,
                          zCPCHAR cpcCtrlTag,
                          zSHORT  nSelectSet )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->SetDisplaySelSet( nSelectSet );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetDisplaySelectSet ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_SetSelectSelectSet
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_SetSelectSelectSet
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation sets the current selection select set for
//              a grid control. The selection select set is the select
//              set used by the grid to communicate back to the
//              application any selections made.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nSelectSet  - The select set for the grid to return
//                            selections.
//
//  RETURNS:    0 - Select set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GRID_SetSelectSelectSet( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         zSHORT  nSelectSet )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         pGrid->SetSelectSelSet( nSelectSet );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_SetSelectSelectSet ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=GRID_ReplaceEntityKey
// Source Module=zdctlss.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GRID_ReplaceEntityKey
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation replaces an entity key in the entity key list.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the list control.
//              cpcCtrlTag  - The unique name of the control.
//              ulOldKey    - Old entity key
//              ulNewKey    - New entity key
//
//  RETURNS:    0 - Entity key successfully changed
//              1 - Error locating old entity key in list
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GRID_ReplaceEntityKey( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zULONG  ulOldKey,
                       zULONG  ulNewKey )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZGrid *pGrid = DYNAMIC_DOWNCAST( ZGrid, pzma->m_pCtrl );
      if ( pGrid )
      {
         zLONG  lRowCnt = pGrid->m_EntityKeyList.GetSize( );
         zLONG  lRow;
         zULONG ulKey;

         for ( lRow = 0; lRow < lRowCnt; lRow++ )
         {
            ulKey = pGrid->m_EntityKeyList.GetAt( lRow );
            if ( ulKey == ulOldKey )
            {
               pGrid->m_EntityKeyList.SetAt( lRow, ulNewKey );
               return( 0 );
            }
         }

         return( 1 );
      }

      TraceLineS( "drvr - Invalid control type for GRID_ReplaceEntityKey ",
                  cpcCtrlTag );
   }

   return( -1 );
}
