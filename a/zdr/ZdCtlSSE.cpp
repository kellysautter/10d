/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlsse.cpp
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
// 1999.03.10  DKS
//    Initialized uninitialized variables.
//
// 1991.01.12    DKS
//    Hi-bit not set fix for Combo-box SpreadSheet subcontrol.
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

// #define DEBUG_ALL

///////// Local Defines /////////
#define zSS_CELLDIRTY      (WORD) 0x0001  // cell is 'dirty' state
#define zSS_COMPLETE       (WORD) 0x0002  // the view completed processing
#define zSS_UPDATETOP      (WORD) 0x0004  // only top row needs update
#define zSS_UPDATEBOTTOM   (WORD) 0x0010  // only bottom row needs update
#define zSS_INITIAL        (WORD) 0x0020  // initial spreadsheet state
#define zSS_RETURNED_PRIOR (WORD) 0x0040  // returned to a prior cell state
#define zSS_ERROR          (WORD) 0x0100  // state of error processing

IMPLEMENT_DYNAMIC( ZSS, CWnd )

BEGIN_MESSAGE_MAP( ZSS, CWnd )
   ON_WM_CREATE( )
   ON_WM_VSCROLL( )
   ON_WM_HSCROLL( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_SIZE( )
   ON_WM_PAINT( )
   ON_CONTROL_REFLECT( SSN_TOPROWCHANGE, OnTopRowChange )
   ON_CONTROL_REFLECT( SSN_LEAVECELL, OnLeaveCell )
   ON_CONTROL_REFLECT( SSM_DBLCLK, OnDblClk )
   ON_CONTROL_REFLECT( SSN_BLOCKSELECTED, OnBlockSelected )
   ON_CONTROL_REFLECT( SSN_BUTTONUP, OnButtonUp )
   ON_CONTROL_REFLECT( SSN_ENTERCELL, OnEnterCell )
   ON_CONTROL_REFLECT( SSN_DATACHANGE, OnDataChange )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )


// ZSS - ctor
ZSS::ZSS( ZSubtask *pZSubtask,
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
     m_csViewNameList( "__" )
{
#ifdef DEBUG_ALL
   if ( m_pzsVName )
   {
      TraceLineS( "SS::ctor Name ", *m_pzsTag );
      TraceLineS( "SS::ctor View Name ", *m_pzsVName );
   }
#endif

   // left, center, right justify unused.
   m_ulMapActFlags &= ~(zCONTROL_CENTERJUSTIFY | zCONTROL_RIGHTJUSTIFY | zCONTROL_LEFTJUSTIFY);

   m_vAppList = 0;
   m_csViewName += *m_pzsTag;
   m_csViewNameList += *m_pzsTag;

   m_pchListInfo = 0;
   m_lTopEntityPos = 1;
   m_lMaxDisplayPos = 0;
   m_nVisibleRows = 0;
   m_nVisibleCols = 0;
   m_uStateFlags = zSS_INITIAL;
   m_lReturnOldRow = 0;
   m_lReturnOldCol = 0;
   m_lEntityCnt = 0;
   m_pfnCallBack = 0;
   m_ulSubtype = pCtrlDef->Subtype;
   m_cpcMB_Text = 0;
   m_nCols = 0;

   // The painter must ensure Entity/Attribute if ViewName???
   if ( m_pzsVName )
   {
#ifdef DEBUG_ALL
      TraceLineS( "ZSS::ctor: ", *m_pzsTag );
      TraceLineS( "ZSS ViewName", *m_pzsVName );
      TraceLineS( "ZSS Entity", *m_pzsEName );
#endif

      GetAttributeLength( &m_ulListLth, vDialog, szlCtrl, szlCtrlBOI );
      if ( m_ulListLth )
      {
         m_pchListInfo = new char[ m_ulListLth ];
         GetVariableFromAttribute( m_pchListInfo, &m_ulListLth, zTYPE_BLOB,
                                   m_ulListLth, vDialog, szlCtrl, szlCtrlBOI,
                                   0, 0 );
      }
   }

// TraceLineI( "Loading SpreadSheet Blob for lth: ", m_ulListLth );
   Attr.Style |= WS_CHILD | WS_TABSTOP;
   if ( pCtrlDef->Subtype & zSS_HIDEROWHEADER )
      Attr.Style |= SSS_HIDEROWHEADERS;

   if ( pCtrlDef->Subtype & zSS_HIDEGRIDLINES )
      Attr.Style |= SSS_NOGRIDLINES;

   Attr.Style |= SSS_NOSELECTBLOCK;
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

   m_chMultiSelect = 0;
   m_chListOnlyWhenSelected = 0;

// if ( (m_ulSubtype & SS_NOINTEGRALHEIGHT) == 0 )  flag disabled
   {
      zSHORT k = 3 * GetSystemMetrics( SM_CYHSCROLL );
      if ( Attr.H > 2 * k )
         Attr.H -= k;
   }

   if ( m_pchListInfo )
   {
      m_nCols = (zSHORT) *((zPLONG) m_pchListInfo);
      m_cpcMB_Text = (zCPCHAR *) new char[ m_nCols * zsizeof( zCPCHAR ) ];
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
ZSS::CreateZ( )
{
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
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( WS_EX_CLIENTEDGE, VA_SPREAD_CLASS_NAME, *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZSS::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::OnCreate: ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZSS::OnCreate ", "Enabled" );
      else
         TraceLineS( "ZSS::OnCreate ", "Disabled" );
   }
   else
      TraceLineS( "ZSS::OnCreate ", "Invalid" );
#endif

   CRect    rect;
   zLONG    lHeight;     // height of row;
   zPCHAR   pch;
   SS_COORD ssCol;

   int nRC = CWnd::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   if ( m_pchListInfo == 0 )
      return( nRC );

   SetMaxCols( m_nCols );

   if ( m_pZSubtask->m_pFontCtrl )
      SSSetFont( m_hWnd, SS_ALLCOLS, SS_ALLROWS,
                 (HFONT) *(m_pZSubtask->m_pFontCtrl), FALSE );
   else
   if ( m_pZSubtask->m_pFontWS )
      SSSetFont( m_hWnd, SS_ALLCOLS, SS_ALLROWS,
                 (HFONT) *(m_pZSubtask->m_pFontWS), FALSE );

   GetRowHeightInPixels( 1, &lHeight );
   GetClientRect( rect );
   m_nVisibleRows = (zSHORT) ((rect.bottom - rect.top) / lHeight) + 1;
// TraceLineI( "ZSS display rows = ", m_nVisibleRows );
// if ( bResize )
//    m_nVisibleRows--;

   if ( m_nVisibleRows < 3 )
   {
      m_nVisibleRows = 3;
      rect.bottom = 3 * lHeight + rect.top;
   // TraceLineS( "ZSS::OnCreate: ", "SetWindowPos" );
      SetWindowPos( 0, rect.left, rect.top,
                    rect.right - rect.left,
                    rect.bottom - rect.top,
                    SWP_NOZORDER | SWP_NOMOVE );
   }

//xuRows = GetMaxRows( );
   for ( ssCol = 1; ssCol <= m_nVisibleRows; ssCol++ )
      Clear( SS_ALLCOLS, ssCol );

   // Set defaults
   SetColHeaderDisplay( SS_HEADERDISPLAY_BLANK );
   SetBool( SSB_PROCESSTAB, FALSE );
   SetUserResizeRow( SS_ALLROWS, FALSE );
// SetSelBlockOptions( SS_SELBLOCK_ROWS );
   SetOperationMode( SS_OPMODE_ROWMODE );
// SetOperationMode( SS_OPMODE_NORMAL );
// SetOperationMode( SS_OPMODE_EXTSEL );
// SetOperationMode( SS_OPMODE_SINGLESEL );
// SetOperationMode( SS_OPMODE_MULTISEL );
// VScrollSetSpecial( TRUE, 0 );

   zCHAR szBuffer[ 40 ];
   zSHORT nLth = zsizeof( "-2147483647," ) * m_nCols;
   zPCHAR pchBuffer = new char[ nLth + 1 ];
   zPCHAR pchComma;
   zPCHAR pchValue;

   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   GetWindowPreferenceString( m_pZSubtask->m_vDialog,
                              szBuffer, pchBuffer, nLth );

   ssCol = 0;
   if ( *pchBuffer )
   {
      pchValue = pchBuffer;
      while ( pchValue )
      {
         pchValue = strchr( pchValue, ',' );
         if ( pchValue )
            pchValue++;

         ssCol++;
      }
   }

   if ( ssCol == m_nCols )
      pchValue = pchBuffer;
   else
      pchValue = 0;

   pch = m_pchListInfo + *((zPLONG) (m_pchListInfo + sizeof( zLONG )));

   // Set the titles and column widths for this spreadsheet.
   zLONG lChar;
   zLONG lCumulativeWidth = 0;

   for ( ssCol = 1; ssCol <= m_nCols; ssCol++ )
   {
      // Get the number of dialog units allotted for this attribute.
      lChar = *((zPLONG) pch);

      if ( pchValue )
      {
         pchComma = strchr( pchValue, ',' );
         if ( pchComma )
            *pchComma = 0;

         lChar = zatol( pchValue );
         SetColWidthInPixels( ssCol, lChar );

         if ( pchComma )
            pchValue = pchComma + 1;
         else
            pchValue = 0;
      }
      else
      {
         if ( m_pZSubtask->m_bUseMapDlgUnits )
            lChar = mConvertMapDlgToCharX( lChar ) / 2 + 1;
         else
            lChar = mConvertDlgUnitToCharX( lChar ) / 2 + 1;

         SetColWidth( ssCol, lChar );
      }

      lCumulativeWidth += lChar;
      if ( lCumulativeWidth > Attr.W )
      {
         if ( m_nVisibleCols == 0 )
            m_nVisibleCols = ssCol - 1;
      }

      pch += sizeof( zLONG );
      lChar = *((zPLONG) pch);
      pch += sizeof( zLONG );
      pchComma = zstrchr( pch, '\t' );
      if ( pchComma )
         *pchComma = 0;

      SetColumnTitle( pch, ssCol );
      if ( pchComma )
      {
         *pchComma = '\t';
         m_cpcMB_Text[ ssCol - 1 ] = pchComma + 1;
      }
      else
         m_cpcMB_Text[ ssCol - 1 ] = 0;

      pch += lChar;

//    TraceLineS( "SS tag: ", pch + zsizeof(zPLONG) );

      // skip past tag
      lChar = *((zPLONG) pch);
      pch += sizeof( zLONG ) + lChar;
   }

   mDeleteInitA( pchBuffer );

   if ( m_nVisibleCols == 0 )
      m_nVisibleCols = m_nCols;

   SetMaxRows( m_nVisibleRows );
// if ( (m_ulSubtype & SS_NOINTEGRALHEIGHT) == 0 )
     SetBool( SSB_AUTOSIZE, TRUE );

   SetAutoSizeVisible( m_nVisibleCols, m_nVisibleRows );
// if ( (m_ulSubtype & SS_NOINTEGRALHEIGHT) == 0 )
      SetBool( SSB_AUTOSIZE, FALSE );

   m_pfnCallBack = (WNDPROC) fnMakeProcInstance( (FARPROC) SpreadSheetCallBack,
                                                 g_hInstanceDLL );
   SSSetCallBack( m_hWnd, m_pfnCallBack );

// VSetBufferSize( m_nVisibleRows, 0 );  now defaults to 256, 0
// SetBool( SSB_VIRTUALMODE, TRUE );     now the default

#ifdef DEBUG_ALL
   TraceLineS( "ZSS::OnCreate (post): ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZSS::OnCreate ", "Enabled" );
      else
         TraceLineS( "ZSS::OnCreate ", "Disabled" );
   }
   else
      TraceLineS( "ZSS::OnCreate ", "Invalid" );
#endif

   return( nRC );
}

BOOL
ZSS::DestroyWindow( )
{
   SS_TraceDone( );

   zCHAR    szBuffer[ 40 ];  // tag length + 7
   zPCHAR   pch = new char[ zsizeof( "-2147483647," ) * m_nCols ];
   LPSPREADSHEET lpSS = SS_Lock( m_hWnd );
   zLONG    lColWidth;
   zSHORT   nLth = 0;
   SS_COORD ssCol;

   m_lEntityCnt = 0;

   for ( ssCol = 1; ssCol <= m_nCols; ssCol++ )
   {
      lColWidth = SS_GetColWidth( lpSS, ssCol );
      _ltoa_s( lColWidth, pch + nLth );
      nLth = zstrlen( pch );
      if ( ssCol < m_nCols )
      {
         pch[ nLth ] = ',';
         nLth++;
      }
   }

   pch[ nLth ] = 0;
   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );
   mDeleteInitA( pch );

   CheckBoxUnloadDefBitmaps( );
   SSClearDoc( m_hWnd );
   SS_Free( m_hWnd );
   if ( m_pfnCallBack )
   {
      fnFreeProcInstance( (FARPROC) SpreadSheetCallBack );
      m_pfnCallBack = 0;
   }

   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

void
ZSS::OnPaint( )
{
   CPaintDC dc( this );
#if 0 //def DEBUG_ALL lots of messages
   TraceRect( "ZSS::OnPaint", dc.m_ps.rcPaint );
#endif
   SS_Paint( m_hWnd, dc );
}

zPCHAR
ZSS::GetClassName( )
{
  return( "ZSS_Spread" );
}

void
ZSS::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   zLONG    lColWidth;
   zLONG    lTotalWidth = 0;
   LPSPREADSHEET lpSS = SS_Lock( m_hWnd );
   SS_COORD ssCol;

   for ( ssCol = 0; ssCol <= m_nCols; ssCol++ )
   {
      lColWidth = SS_GetColWidth( lpSS, ssCol );
      if ( pt.x >= lTotalWidth && pt.x <= lTotalWidth + lColWidth )
      {
         break;
      }

      lTotalWidth += lColWidth;
   }

   if ( ssCol > 0 && ssCol <= m_nCols )
      m_pZSubtask->InformDriverOfFlyBy( this, m_cpcMB_Text[ ssCol - 1 ] );
   else
      m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZSS - dtor
ZSS::~ZSS( )
{
// TraceLineS( "ZSS::~ZSS", "" );
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::dtor ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

   mDeleteInitA( m_pchListInfo );
   mDeleteInitA( m_cpcMB_Text );
// delete( *m_pzsScope ); // please do not delete m_pzsScope cuz it's
                          // just an alias for m_pzsAName
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZSS::MapRowsFromOI( SS_COORD ssRow, zLONG lRows, zSHORT nDisplayRows )
{
#ifdef DEBUG_ALL
// if ( ssRow == 0 )
   {
      TraceLineS( "ZSS::MapRowsFromOI: ", *m_pzsTag );
      TraceLineI( "ZSS::MapRowsFromOI ssRow: ", ssRow );
      TraceLineI( "ZSS::MapRowsFromOI lRows: ", lRows );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapRowsFromOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapRowsFromOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapRowsFromOI ", "Invalid" );
   }
#endif

   zVIEW  vApp = 0;
   if ( m_pchListInfo && GetViewByName( &vApp, m_csViewName,
                                        m_pZSubtask->m_vDialog,
                                        zLEVEL_SUBTASK ) > 0 )
   {
      zCPCHAR  cpcEntityName;
      zPCHAR   pchAttributeName;
      zCPCHAR  cpcContextName;
      zCPCHAR  cpcScope;
      zPCHAR   pchText;
      zPCHAR   pchOn;
      zPCHAR   pch;
      zPCHAR   pchBuffer;
      zPVOID   pvCursor;
      zCHAR    szMap[ 32768 ];
      zULONG   ulStyle;
      zULONG   ulSubtype;
      zULONG   ulAttrLth;
      zUSHORT  uTypeStyle;
      SS_CELLTYPE SSCellType;
      zULONG   ulLth;
      zSHORT   nRC;
      zSHORT   k;

#if 0
///// code to run thru each CtrlMap entity
   zVIEW  vApp = 0;
   if ( m_pchListInfo && GetViewByName( &vApp, *m_pzsVName,
                                        m_pZSubtask->m_vDialog,
                                        zLEVEL_ANY ) > 0 )
      if ( m_vAppList == 0 )
      {
         CreateViewFromViewForTask( &m_vAppList, vApp,
                                    m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewNameList,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent core from
                                    // automatically cleaning it up
      }

      zKZWDLGXO_CtrlMap_DEF CtrlMapDef;

#ifdef DEBUG_LODSTRUCT
      zmemset( &CtrlMapDef, 0xFE, sizeof( zKZWDLGXO_CtrlMap_DEF ) );
   // CtrlMapDef.CheckByte = 0xFE;
#endif
      CtrlMapDef.lStructSize = sizeof( zKZWDLGXO_CtrlMap_DEF );
      nRC = SetCursorFirstEntity( m_pZSubtask->m_vDialog, szlCtrlMap, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         GetStructFromEntityAttrs( (zPCHAR) &CtrlMapDef, m_pZSubtask->m_vDialog, szlCtrlMap );
#ifdef DEBUG_LODSTRUCT
         if ( CtrlMapDef.CheckByte != 0xFE )
            MessageBox( 0, "CtrlMapDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );
      // zPCHAR pchCheckByte = (zPCHAR) &(CtrlMapDef.CheckByte);
      // pchCheckByte--;
      // if ( *pchCheckByte == (char) 0xFE )
      //    TraceLineS( "zdctlss CtrlMapDef CheckByte error??", "" );
#endif

         GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                   zsizeof( szMap ) - 1,
                                   m_vAppList, CtrlMapDef.EN,
                                   CtrlMapDef.AN, CtrlMapDef.Context,
                                   CtrlMapDef.Context[ 0 ] ? 0 :
                                                      zUSE_DEFAULT_CONTEXT );

      if ( CtrlMapDef.VN[ 0 ] )
      {
         if ( CtrlMapDef.EN[ 0 ] == 0 )
            OperatorSend( 0, szlDrvr, "View name with no entity!!!", TRUE );

         if ( CtrlMapDef.AN[ 0 ] == 0 )
            TraceLineS( "ZSS::MapRows View Name with no attribute!!!", "" );
      }

      // If there is a view name, then fill in the rest of the attributes,
      // otherwise let them stay null.
      if ( CtrlMapDef.VN[ 0 ] )
      {
         m_pzsVName = new CString( CtrlMapDef.VN );
         m_pzsEName = new CString( CtrlMapDef.EN );
         m_pzsAName = new CString( CtrlMapDef.AN );
         m_pzsContext = new CString( CtrlMapDef.Context );
      }
   }

///// end code for CtrlMap
#endif

#ifdef DEBUG_ALL
// if ( ssRow == 0 )
   {
      TraceLineS( "ZSS::MapRowsFromOI (mid): ", *m_pzsTag );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapRowsFromOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapRowsFromOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapRowsFromOI ", "Invalid" );
   }
#endif

      zLONG lCols = *((zPLONG) m_pchListInfo);

      if ( m_vAppList == 0 )
      {
         CreateViewFromViewForTask( &m_vAppList, vApp,
                                    m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewNameList,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent the OE from
                                    // automatically cleaning it up during
                                    // the deletion of an OE subtask.
      }

      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                             zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                             ssRow - 1, cpcScope, 0 );

      while ( lRows <= nDisplayRows && nRC > zCURSOR_UNCHANGED )
      {
//       TraceLineI( "ZSS::MapFromOI row ", ssRow );

         // Loop through m_pchListInfo to see what type of cells should
         // be created for the attributes
         pch = m_pchListInfo + sizeof( zLONG ) + sizeof( zULONG );
         for ( k = 1; k <= lCols; k++ )
         {
//          TraceLineI( "ZSS::MapFromOI col ", k );
            if ( *pch )
            {
               cpcEntityName = pch + sizeof( zLONG );

               // skip past entity name
               pch += sizeof( zLONG ) + *((zPLONG) pch);
            }
            else
            {
               pch += sizeof( zLONG );
               cpcEntityName = *m_pzsEName;
            }

            pchAttributeName = pch + sizeof( zLONG );
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past attribute
            cpcContextName = pch + sizeof( zLONG );
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past context
            ulStyle = *((zPULONG) pch);
            pch += sizeof( zLONG );     // skip past cell style
            ulSubtype = *((zPULONG) pch);

            // This check permits us to do the MiGetUpdate at most one time
            // for each column.
            // Sizeable borders do not make sense for a spreadsheet
            // sub-control ... so we ensure the flag is set off at conversion,
            // and at runtime we use this flag position to implement MiUpdate.
            if ( (ulSubtype & zCONTROL_SIZEABLEBORDER) == 0 )
            {
//             if ( (ulStyle & 0x0FFF0000L) == zGRID_CHECKBOX )
//                SetValue( k, 0, "" );  // turn off checkbox header text

               ulSubtype |= zCONTROL_SIZEABLEBORDER;
               if ( (ulSubtype & zCONTROL_DISABLED) == 0 )
               {
                  if ( GetLock( k, SS_ALLROWS ) ||
                       MiGetUpdateForViewEntityAttr( m_vAppList,
                                                     cpcEntityName,
                                                     pchAttributeName,
                                                     0 ) <= 0 )
                  {
                     ulSubtype |= zCONTROL_DISABLED;
                  }
               }

               *((zPULONG) pch) = ulSubtype;
               if ( ulSubtype & zCONTROL_DISABLED )
                  SetLock( k, SS_ALLROWS, TRUE ); // LockColumn( k, TRUE );
            }

            pch += sizeof( zLONG );     // skip past Subtype
            GetAttributeDisplayLength( &ulAttrLth, m_vAppList,
                                       cpcEntityName, pchAttributeName,
                                       cpcContextName );

            uTypeStyle = zLOUSHORT( ulSubtype );

            switch ( ulStyle & 0x0FFF0000L )
            {
               case zGRID_EDITBOX:
//                TraceLineI( "SpreadSheet editbox ", ulStyle );

                  if ( uTypeStyle & zEDITBOX_UPPERCASE )
                  {
                     uTypeStyle = SS_CASE_UCASE;
                     ulStyle |= ES_UPPERCASE;
                  }
                  else
                  if ( uTypeStyle & zEDITBOX_LOWERCASE )
                  {
                     uTypeStyle = SS_CASE_LCASE;
                     ulStyle |= ES_LOWERCASE;
                  }
                  else
                  {
                     uTypeStyle = SS_CASE_NOCASE;
                  }

                  if ( ulSubtype & zCONTROL_DISABLED )
                  {
                     // We are a static control.
                     if ( ulSubtype & zCONTROL_RIGHTJUSTIFY )
                        uTypeStyle |= ES_AUTOHSCROLL | SS_TEXT_RIGHT;
                     else
                        uTypeStyle |= ES_AUTOHSCROLL | SS_TEXT_LEFT;

                     uTypeStyle |= SS_TEXT_SHADOWIN;
                     SetTypeStaticText( &SSCellType, uTypeStyle );
                  }
                  else
                  {
                     // We are an edit control.
                     if ( ulSubtype & zCONTROL_RIGHTJUSTIFY )
                        ulStyle = ES_AUTOHSCROLL | ES_RIGHT;
                     else
                        ulStyle = ES_AUTOHSCROLL | ES_LEFT;

                     SetTypeEdit( &SSCellType, ulStyle,
                                  (zSHORT) ulAttrLth, SS_CHRSET_CHR,
                                  uTypeStyle );
                  }

                  SetCellType( k, ssRow, &SSCellType );

                  // This is the attribute to present to the spreadsheet.
                  GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                            zsizeof( szMap ) - 1,
                                            m_vAppList, cpcEntityName,
                                            pchAttributeName, cpcContextName,
//                                            cpcContextName &&
//                                               cpcContextName[ 0 ] ? 0 :
//                                                      zUSE_DEFAULT_CONTEXT );
                                            cpcContextName && cpcContextName[ 0 ] ?
                                              zACCEPT_NULL_ENTITY :
                                              zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

                  SetValue( k, ssRow, szMap );
               // zCHAR szMsg[ zsizeof( szMap ) + 100 ];
               //
               // sprintf_s( szMsg, zsizeof( szMsg ), "SetValue for Row: %d Col: %d %s",
               //           (zLONG) ssRow, (zLONG) k, szMap );
               // TraceLineS( szMsg, "" );
                  break;

               case zGRID_COMBOBOX:
//                TraceLineI( "SpreadSheet combobox ", ulStyle );
                  if ( uTypeStyle & zCOMBOBOX_SIMPLE )
                     ulStyle = CBS_SIMPLE;
                  else
                  if ( uTypeStyle & zCOMBOBOX_DROPDOWN )
                     ulStyle = CBS_DROPDOWN;
                  else
               // if ( uTypeStyle & zCOMBOBOX_DROPDOWN_LIST )
                     ulStyle = CBS_DROPDOWNLIST;

                  if ( uTypeStyle & zCOMBOBOX_SORT )
                     ulStyle |= CBS_SORT;

                  GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                            zsizeof( szMap ) - 1,
                                            m_vAppList, cpcEntityName,
                                            pchAttributeName, cpcContextName,
//                                          cpcContextName &&
//                                             cpcContextName[ 0 ] ?
//                                                0 : zUSE_DEFAULT_CONTEXT );
                                            cpcContextName && cpcContextName[ 0 ] ?
                                              zACCEPT_NULL_ENTITY :
                                              zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

                  if ( (ulSubtype & zCONTROL_DISABLED) == 0 )
                  {
                     if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
                     {
                        IssueError( m_pZSubtask->m_vDialog, 0, 0, "Memory alloc error - Combo SS" );
                        break;
                     }

                     pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );
                     ulLth = 0;
                     nRC = GetFirstTableEntryForAttribute( pchBuffer, zMAX_BLOB, m_vAppList, cpcEntityName, pchAttributeName, cpcContextName, &pvCursor );
                     while ( nRC == 0 )
                     {
                        ulLth += zstrlen( pchBuffer + ulLth );
                        *(pchBuffer + ulLth++) = '\t';
                        nRC = GetNextTableEntryForAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, m_vAppList, cpcEntityName, pchAttributeName, cpcContextName, &pvCursor );
                     }

                     SetTypeComboBox( &SSCellType, ulStyle, pchBuffer );
                     pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
                     DrFreeTaskMemory( pchBuffer );
                  }
                  else
                  {
                     ulStyle |= WS_DISABLED;
                     SetTypeComboBox( &SSCellType, ulStyle, szMap );
                  }

                  SetCellType( k, ssRow, &SSCellType );
                  nRC = SetData( k, ssRow, szMap );
                  break;

               case zGRID_CHECKBOX:
//                TraceLineI( "SpreadSheet checkbox ", ulStyle );
                  ulStyle = BS_CHECKBOX;
                  if ( ulSubtype & zCONTROL_DISABLED )
                     ulStyle |= WS_DISABLED;

                  pchText = pch + sizeof( zLONG );
                  pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past text

                  SetTypeCheckBox( &SSCellType, ulStyle, pchText );
                  SetCellType( k, ssRow, &SSCellType );

                  // This is the attribute to present to the spreadsheet.
                  GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                            zsizeof( szMap ) - 1,
                                            m_vAppList, cpcEntityName,
                                            pchAttributeName, cpcContextName,
//                                          cpcContextName &&
//                                             cpcContextName[ 0 ] ? 0 :
//                                                    zUSE_DEFAULT_CONTEXT );
                                            cpcContextName && cpcContextName[ 0 ] ?
                                              zACCEPT_NULL_ENTITY :
                                              zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );


                  pchOn = pch + sizeof( zLONG );
                  pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past ON
                  if ( zstrcmp( szMap, pchOn ) == 0 )
                     SetValue( k, ssRow, "1" );
                  else
                     SetValue( k, ssRow, "0" );

                  pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past OFF
                  break;


               case zGRID_DATE:
               case zGRID_FLOAT:
               case zGRID_INTEGER:
               case zGRID_PIC:
               case zGRID_PICTURE:
               case zGRID_TEXT:
               case zGRID_TIME:

               default:
                  TraceLineI( "Invalid SpreadSheet style ", ulStyle );
            }
         }

         nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                zPOS_NEXT, 0, 0, 0, 0,
                                cpcScope, 0 );
         ssRow++;
         lRows++;

      }  // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru fields
   }

#ifdef DEBUG_ALL
// if ( ssRow == 0 )
   {
      TraceLineS( "ZSS::MapRowsFromOI (post): ", *m_pzsTag );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapRowsFromOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapRowsFromOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapRowsFromOI ", "Invalid" );
   }
#endif
}

zSHORT
ZSS::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::MapFromOI: ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZSS::MapFromOI ", "Enabled" );
      else
         TraceLineS( "ZSS::MapFromOI ", "Disabled" );
   }
   else
      TraceLineS( "ZSS::MapFromOI ", "Invalid" );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   m_lEntityCnt = 0;
   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }
// else
//    TraceLineS( "ZSS::MapFromOI m_vAppList null ", *m_pzsTag );

   zVIEW    vApp = 0;
   SS_COORD ssRow;
   SS_COORD ssCol;
   zSHORT   nRC;

   for ( ssCol = 1; ssCol <= m_nVisibleRows; ssCol++ )
      Clear( SS_ALLCOLS, ssCol );

   if ( mIs_hWnd( m_hWnd ) &&
        m_pchListInfo && GetViewByName( &vApp, *m_pzsVName,
                                        m_pZSubtask->m_vDialog,
                                        zLEVEL_ANY ) > 0 )
   {
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::MapFromOI ", *m_pzsTag );
#endif
      zLONG    lRows;
      zSHORT   nDisplayRows;
      SS_COORD ssRowPrev;

//x   zBOOL bPrevRedraw = SetBool( SSB_REDRAW, FALSE );

      SetNameForView( vApp, m_csViewName,
                      m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );

   // m_lEntityCnt = CountEntitiesForView( vApp, *m_pzsEName );
      CreateViewFromViewForTask( &m_vAppList, vApp, m_pZSubtask->m_vDialog );
      SetNameForView( m_vAppList, m_csViewNameList,
                      m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
   // SfLockView( m_vAppList );  // we lock this view because it is
                                 // unnamed ... to prevent core from
                                 // automatically cleaning it up

      // This is now done for every MapFromOI ... may be too much???
      // Just done at Setup is not sufficient!

      zCPCHAR  cpcScope;

      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                             zPOS_FIRST, 0, 0, 0, 0, cpcScope, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         m_lEntityCnt++;
         nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                zPOS_NEXT, 0, 0, 0, 0, cpcScope, 0 );

      }  // end of:  while ( nRC > zCURSOR_UNCHANGED )

      VSetMax( m_lEntityCnt );

      GetTopLeftPrev( &ssCol, &ssRowPrev );
      GetTopLeftCell( &ssCol, &ssRow );
//    TraceLineI( "SS ssRowPrev = ", ssRowPrev );
//    TraceLineI( "SS ssRowCurr = ", ssRow );

#ifdef DEBUG_ALL
      TraceLineS( "ZSS::MapFromOI (mid1): ", *m_pzsTag );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapFromOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapFromOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapFromOI ", "Invalid" );
#endif

      // Loop through each entity in the instance which is within the
      // current spreadsheet viewing range and data-fill the corresponding
      // cells in the spreadsheet.  In the following tests, ssRow will be
      // to the position within the object instance; lRows will be set to
      // the first display row; nDisplayRows will be set to the last
      // display row.
      if ( ssRow > 0 && ssRowPrev > 0 )
      {
         zLONG k = ssRow - ssRowPrev;
         if ( k == 1 || k == -1 )
         {
            // we need to map exactly one row!
            nDisplayRows = 1;
            lRows = 1;
            if ( k == 1 )
            {
               ssRow = ssRowPrev + m_nVisibleRows;
            }
            else
            {
               // leave ssRow as it was
            }
         }
         else
         {
            lRows = 1;
            nDisplayRows = m_nVisibleRows;
            // leave ssRow as it was
         }
      }
      else
      {
         lRows = 1;
         nDisplayRows = m_nVisibleRows;
         ssRow = 1;
      }

#ifdef DEBUG_ALL
      TraceLineS( "ZSS::MapFromOI (mid2): ", *m_pzsTag );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapFromOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapFromOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapFromOI ", "Invalid" );

      TraceLineI( "SS lRows = ", lRows );
      TraceLineI( "...nDisplayRows = ", nDisplayRows );
      TraceLineI( "...ssRow (position within oi) = ", ssRow );
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#endif

//    MapRowsFromOI( ssRow, lRows, nDisplayRows );
      MapRowsFromOI( ssRow, lRows, m_nVisibleRows ); // force all rows to Map

#ifdef DEBUG_ALL
      TraceLineS( "ZSS::MapFromOI (mid3): ", *m_pzsTag );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapFromOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapFromOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapFromOI ", "Invalid" );
#endif

//    if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
      if ( SetEntityCursor( vApp, *m_pzsEName, 0,
                            zPOS_FIRST | zTEST_CSR_RESULT,
                            0, 0, 0, 0, cpcScope, 0 ) >= 0 )
      {
         zCPCHAR cpcScope;

         if ( m_pzsScope && *m_pzsScope )
            cpcScope = *m_pzsScope;
         else
            cpcScope = 0;

         SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
         SetEntityCursor( vApp, *m_pzsEName, 0,
                          zPOS_FIRST | zQUAL_SELECTED,
                          0, 0, 0, 0, cpcScope, 0 );
         lRows = GetRelativeEntityNumber( vApp, *m_pzsEName,
                                          cpcScope, 0L );

#ifdef DEBUG_ALL
      TraceLineS( "ZSS::MapFromOI (mid4): ", *m_pzsTag );
      DisplayObjectInstance( vApp, *m_pzsEName, 0 );
#endif

//???    if ( GetActiveCell( &ssCol, &ssRow ) == FALSE )
//???       ssCol = 1;
//???
//???    SetActiveCell( ssCol, lRows + 1 );
//       ShowActiveCell( SS_SHOW_CENTERLEFT );
      }
      else
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZSS::MapFromOI (mid5): ", *m_pzsTag );
         DisplayObjectInstance( vApp, 0, 0 );
#endif
         // There are no rows to show ... disable the control.
         SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
         {
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, zsizeof( szMsg ), "Grid: %s - no %s entities",
                      *m_pzsTag, *m_pzsEName );
            TraceLineS( szMsg, "" );
         }
      }

//x   SetBool( SSB_REDRAW, bPrevRedraw );
//    TraceLineX( "Invalidating SS hWnd: ", (zLONG) m_hWnd );
      Invalidate( TRUE );  // this must be after the SetRedraw
      nRC = 0;
   }
   else
   {
#ifdef DEBUG_ALL
      TraceLineS( "ZSS::MapFromOI (mid6): ", *m_pzsTag );
#endif

      m_lEntityCnt = 0;
      if ( mIs_hWnd( m_hWnd ) )
         VSetMax( m_lEntityCnt );

      nRC = -1;
   }

#if 0
   if ( mIs_hWnd( m_hWnd ) )
   {
      LPSPREADSHEET lpSS;
      lpSS = SS_Lock( m_hWnd );
      if ( lpSS )
      {
         if ( mIs_hWnd( lpSS->hWndHScroll ) )
         {
            ::SendMessage( m_hWnd, WM_HSCROLL, MAKELONG( SB_THUMBPOSITION, 0 ),
                           (LPARAM) lpSS->hWndHScroll );
         // ShowScrollBar( lpSS->hWndHScroll, SB_CTL, TRUE );
         // SetScrollRange( lpSS->hWndHScroll, SB_CTL, 0, -1, TRUE );
         // ::SetScrollPos( lpSS->hWndHScroll, SB_CTL, 0, FALSE );
         // lpSS->Col.DataCnt = 1;
         }

         if ( mIs_hWnd( lpSS->hWndVScroll ) )
         {
            ::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG( SB_THUMBPOSITION, 0 ),
                           (LPARAM) lpSS->hWndVScroll );
         // ShowScrollBar( lpSS->hWndVScroll, SB_CTL, TRUE );
         // SetScrollRange( lpSS->hWndVScroll, SB_CTL, 0, -1, TRUE );
         // ::SetScrollPos( lpSS->hWndVScroll, SB_CTL, 0, FALSE );
         // lpSS->Row.DataCnt = 1;
         }
      }

   // DrUnlockTaskMemory( lpSS );
   }
#endif

#ifdef DEBUG_ALL
   TraceLineS( "ZSS::MapFromOI (post): ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZSS::MapFromOI ", "Enabled" );
      else
         TraceLineS( "ZSS::MapFromOI ", "Disabled" );
   }
   else
      TraceLineS( "ZSS::MapFromOI ", "Invalid" );
#endif

   m_lReturnOldRow = 0;
   m_lReturnOldCol = 0;
   OnEnterCell( );

   return( nRC );
}

zSHORT
ZSS::MapToOI( zLONG lFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::MapToOI: ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZSS::MapToOI ", "Enabled" );
      else
         TraceLineS( "ZSS::MapToOI ", "Disabled" );
   }
   else
      TraceLineS( "ZSS::MapToOI ", "Invalid" );
#endif

   return( MapRowToOI( lFlag, 0 ) );
}

zSHORT
ZSS::MapRowToOI( zLONG lFlag, zLONG lRow )
{
#ifdef DEBUG_ALL
   if ( lRow == 1 )
   {
      TraceLineS( "ZSS::MapRowToOI: ", *m_pzsTag );
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( mIs_hWndEnabled( m_hWnd ) )
            TraceLineS( "ZSS::MapRowToOI ", "Enabled" );
         else
            TraceLineS( "ZSS::MapRowToOI ", "Disabled" );
      }
      else
         TraceLineS( "ZSS::MapRowToOI ", "Invalid" );
   }
#endif

   zVIEW  vApp;

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pchListInfo && (vApp = IsVisibleForMapToOI( m_csViewName )) != 0 )
   {
      zCPCHAR  cpcEntityName;
      zCPCHAR  cpcAttributeName;
      zCPCHAR  cpcContextName;
      zCPCHAR  cpcScope;
      zPCHAR   pchOn;
      zPCHAR   pchOff;
      zPCHAR   pch;
      zBOOL    bContinue;
      zULONG   ulStyle;
      zULONG   ulSubtype;
      zCHAR    szMap[ 32768 ];
      zLONG    lCols;
      zLONG    k;

      SS_COORD ssActiveRow;    // Row and column for the currently
      SS_COORD ssActiveCol;    // active cell
      zSHORT nRC = 0;

      lCols = *((zPLONG) m_pchListInfo);

      // retrieve the active cell
      if ( lRow )
      {
         ssActiveRow = lRow;
         ssActiveCol = -1;
      }
      else
      if ( GetActiveCell( &ssActiveCol, &ssActiveRow ) == 0 ||
           ssActiveRow == 0 || ssActiveCol == 0 )
      {
         return( 0 ); // cannot retrieve the active cell or not in data cell
      }

      if ( ssActiveCol > lCols )
         ssActiveCol = lCols;

//    TraceLineI( "Current row: ", ssActiveRow );
//    TraceLineI( "Current col: ", ssActiveCol );

      if ( m_vAppList == 0 )
      {
         CreateViewFromViewForTask( &m_vAppList, vApp,
                                    m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewNameList,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent core from
                                    // automatically cleaning it up
      }

//    SetRedraw( FALSE );

//    TraceLineI( "ZSS::MapToOI row ", ssActiveRow );

      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                             zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                             ssActiveRow - 1, cpcScope, 0 );
      if ( nRC > zCURSOR_UNCHANGED )
      {
         nRC = 0;
         pch = m_pchListInfo + sizeof( zLONG ) + sizeof( zULONG );
         for ( k = 1; k <= lCols; k++ )
         {
//          TraceLineI( "ZSS::MapToOI col ", k );

            if ( *pch )
            {
               cpcEntityName = pch + sizeof( zLONG );
               pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past entity name
            }
            else
            {
               pch += sizeof( zLONG );
               cpcEntityName = *m_pzsEName;
            }

            cpcAttributeName = pch + sizeof( zLONG );
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past attribute
            cpcContextName = pch + sizeof( zLONG );
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past context
            ulStyle = *((zPLONG) pch);
            pch += sizeof( zLONG );     // skip past cell style
            ulSubtype = *((zPLONG) pch);
            pch += sizeof( zLONG );     // skip past Subtype

            if ( (ulSubtype & zCONTROL_DISABLED) ||
                 ssActiveCol != -1 && k != ssActiveCol )
            {
               // we don't want to map, but we want to skip thru ListInfo
               bContinue = TRUE;
            }
            else
            {
               // we want to map and skip thru ListInfo
               bContinue = FALSE;
            }

            switch ( ulStyle & 0x0FFF0000L )
            {
               case zGRID_EDITBOX:
               case zGRID_COMBOBOX:
                  if ( bContinue )
                     continue;

                  if ( GetLock( k, SS_ALLROWS ) )
                  {
                  // TraceLineI( "zdctlss continue on GetLock1 ", k );
                     continue;
                  }

               // nRC = GetValue( k, ssActiveRow, szMap );
                  nRC = GetData( k, ssActiveRow, szMap );
               // zCHAR szMsg[ zsizeof( szMap ) + 100 ];
               //
               // sprintf_s( szMsg, zsizeof( szMsg ), "GetValue for Row: %d Col: %d %s",
               //           (zLONG) ssActiveRow, (zLONG) k, szMap );
               // TraceLineS( szMsg, "" );

                  // Set the attribute from the spreadsheet value.
                  if ( CheckExistenceOfEntity( m_vAppList,
                                           cpcEntityName ) == zCURSOR_SET &&
                       MiGetUpdateForViewEntityAttr( m_vAppList,
                                                     cpcEntityName,
                                                     cpcAttributeName,
                                                     0 ) > 0 )
                  {
                     nRC = SetAttributeFromVariable( m_vAppList,
                                             cpcEntityName,
                                             cpcAttributeName,
                                             szMap, zTYPE_STRING,
                                             zsizeof( szMap ) - 1,
                                             cpcContextName,
                                             (cpcContextName &&
                                              cpcContextName[ 0 ]) ?
                                              lFlag :
                                              lFlag | zUSE_DEFAULT_CONTEXT );
                  }

                  break;

               case zGRID_CHECKBOX:
                  pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past Text
                  pchOn = pch + sizeof( zLONG );
                  pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past ON
                  pchOff = pch + sizeof( zLONG );
                  pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past OFF

                  if ( bContinue )
                     continue;

                  if ( GetLock( k, SS_ALLROWS ) )  // needed???
                  {
                  // TraceLineI( "zdctlss continue on GetLock2 ", k );
                     continue;
                  }

                  GetData( k, ssActiveRow, szMap );
                  if ( CheckExistenceOfEntity( m_vAppList, cpcEntityName )
                                                           == zCURSOR_SET &&
                       MiGetUpdateForViewEntityAttr( m_vAppList,
                                                     cpcEntityName,
                                                     cpcAttributeName,
                                                     0 ) > 0 )
                  {
                     nRC = SetAttributeFromVariable( m_vAppList,
                                           cpcEntityName,
                                           cpcAttributeName,
                                           *szMap == '1' ? pchOn : pchOff,
                                           zTYPE_STRING,
                                           zsizeof( szMap ) - 1,
                                           cpcContextName,
                                           (cpcContextName &&
                                            cpcContextName[ 0 ]) ?
                                              lFlag :
                                              lFlag | zUSE_DEFAULT_CONTEXT );
                  }

                  break;

               case zGRID_DATE:
               case zGRID_FLOAT:
               case zGRID_INTEGER:
               case zGRID_PIC:
               case zGRID_PICTURE:
               case zGRID_TEXT:
               case zGRID_TIME:
               default:
                  break;
            }

            if ( k == ssActiveCol )
               break;
         }
      }  // end of:  if ( nRC > zCURSOR_UNCHANGED ) ... mapping one cell
      else
         nRC = 1;

//    SetRedraw( TRUE );
      return( nRC );
   }

   return( 0 );
}

void
ZSS::OnTopRowChange( )
{
#ifdef DEBUG_ALL
   SS_COORD ssRowPrev;
   SS_COORD ssColPrev;
   SS_COORD ssRowNext;
   SS_COORD ssColNext;

   TraceLineI( "ZSS::OnTopRowChange ", (zLONG) this );

   GetTopLeftPrev( &ssColPrev, &ssRowPrev );
   GetTopLeftCell( &ssColNext, &ssRowNext );
   TraceLineI( "SSN_TOPROWCHANGE prev: ", ssRowPrev );
// TraceLineI( "Left col prev: ", ssColPrev );
   TraceLineI( "Top row change: ", ssRowNext );
// TraceLineI( "Left col change: ", ssColNext );
#endif

   zVIEW  vApp = 0;
   if ( m_pchListInfo && GetViewByName( &vApp, m_csViewName,
                                        m_pZSubtask->m_vDialog,
                                        zLEVEL_SUBTASK ) > 0 )
   {
      zLONG    lRows;
      zSHORT   nDisplayRows;
      SS_COORD ssRowPrev;
      SS_COORD ssRow;
      SS_COORD ssCol;

      GetTopLeftPrev( &ssCol, &ssRowPrev );
      GetTopLeftCell( &ssCol, &ssRow );
//    TraceLineI( "SS ssRowPrev = ", ssRowPrev );
//    TraceLineI( "SS ssRowCurr = ", ssRow );

      // Loop through each entity in the instance which is within the
      // current spreadsheet viewing range and data-fill the corresponding
      // cells in the spreadsheet.  In the following tests, ssRow will be
      // to the position within the object instance; lRows will be set to
      // the first display row; nDisplayRows will be set to the last
      // display row.
      if ( ssRow > 0 && ssRowPrev > 0 )
      {
         zLONG k = ssRow - ssRowPrev;
         if ( k == 1 || k == -1 )
         {
            // we need to map exactly one row!
            nDisplayRows = 1;
            lRows = 1;
            if ( k == 1 )
            {
               ssRow = ssRowPrev + m_nVisibleRows;
            }
            else
            {
               // leave ssRow as it was
            }
         }
         else
         {
            lRows = 1;
            nDisplayRows = m_nVisibleRows;
            // leave ssRow as it was
         }
      }
      else
      {
         lRows = 1;
         nDisplayRows = m_nVisibleRows;
         ssRow = 1;
      }

#ifdef DEBUG_ALL
      TraceLineI( "SS lRows = ", lRows );
      TraceLineI( "...nDisplayRows = ", nDisplayRows );
      TraceLineI( "...ssRow (position within oi) = ", ssRow );
      TraceLineI( "...SS display rows = ", m_nVisibleRows );
#endif

      zBOOL bPrevRedraw = SetBool( SSB_REDRAW, FALSE );

      MapRowsFromOI( ssRow, lRows, nDisplayRows );

      if ( CheckExistenceOfEntity( vApp,
                                   *m_pzsEName ) == zCURSOR_SET )
      {
         zCPCHAR cpcScope;

         if ( m_pzsScope && *m_pzsScope )
            cpcScope = *m_pzsScope;
         else
            cpcScope = 0;

         SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
         SetEntityCursor( vApp, *m_pzsEName, 0,
                          zPOS_FIRST | zQUAL_SELECTED,
                          0, 0, 0, 0, cpcScope, 0 );
         lRows = GetRelativeEntityNumber( vApp, *m_pzsEName,
                                          cpcScope, 0L );

//???    if ( GetActiveCell( &ssCol, &ssRow ) == FALSE )
//???       ssCol = 1;
//???
//???    SetActiveCell( ssCol, lRows + 1 );
//       ShowActiveCell( SS_SHOW_CENTERLEFT );
      }
      else
      {
         SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );
      }

      SetBool( SSB_REDRAW, bPrevRedraw );
   }

   ProcessEvent( this, SSN_TOPROWCHANGE );
}

void
ZSS::OnLeaveCell( )
{
   SS_COORD ssRow;
   SS_COORD ssCol;
   SS_COORD ssActiveRow;    // Row and column for the currently
   SS_COORD ssActiveCol;    // active cell

#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnLeaveCell ", (zLONG) this );

   GetActiveCell( &ssCol, &ssRow );
   TraceLineI( "EvLeaveCell Row = ", ssRow );
   TraceLineI( "EvLeaveCell Col = ", ssCol );
#endif

   if ( m_ulMapActFlags & zMAPACT_CTRL_SPECIFIC1 )  // prevent recursion
      return;

   m_ulMapActFlags |= zMAPACT_CTRL_SPECIFIC1;

   GetTopLeftCell( &ssCol, &ssRow );
   if ( GetActiveCell( &ssActiveCol, &ssActiveRow ) &&
        ssActiveRow && ssActiveCol )
   {
      if ( ssActiveRow >= ssRow && ssActiveRow < ssRow + m_nVisibleRows )
      {
         if ( MapToOI( ) )
         {
            SetActiveCell( ssActiveCol, ssActiveRow );
         }
      }
   }

   m_ulMapActFlags &= ~zMAPACT_CTRL_SPECIFIC1;

 // TraceLineI( "ZSS::OnLeaveCell ActiveRow ", (zLONG) ssActiveRow );
 // TraceLineI( "~~~~~~~~~~~~~~ DisplayRows ", m_nVisibleRows );
}

void
ZSS::OnEnterCell( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::OnEnterCell: ", *m_pzsTag );
   if ( mIs_hWnd( m_hWnd ) )
   {
      if ( mIs_hWndEnabled( m_hWnd ) )
         TraceLineS( "ZSS::OnEnterCell ", "Enabled" );
      else
         TraceLineS( "ZSS::OnEnterCell ", "Disabled" );
   }
   else
      TraceLineS( "ZSS::OnEnterCell ", "Invalid" );
#endif

   // We are about to enter a new cell
   SS_COORD ssActiveRow;    // Row and column for the currently
   SS_COORD ssActiveCol;    // active cell

   CWnd *pWnd = GetFocus( );
   if ( pWnd == 0 || pWnd->m_hWnd != m_hWnd ||
        (m_ulMapActFlags & zSS_RECURSIVE_EVENT) )
   {
      return;
   }

   m_ulMapActFlags |= zSS_RECURSIVE_EVENT;

   // retrieve the active cell
   if ( GetActiveCell( &ssActiveCol, &ssActiveRow ) )
   {
      zVIEW  vApp;
//    TraceLineI( "OnEnterCell row", ssActiveRow );
//    TraceLineI( "OnEnterCell col", ssActiveCol );

      // reset View's position to correspond to the new cell's
      // absolute position
      if ( m_lReturnOldCol == 0 && m_lReturnOldRow == 0 &&
           GetViewByName( &vApp, m_csViewName,
                          m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
      {
         zCPCHAR  cpcScope;
         if ( m_pzsScope && *m_pzsScope )
            cpcScope = *m_pzsScope;
         else
            cpcScope = 0;

         SetEntityCursor( vApp, *m_pzsEName, 0,
                          zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                          ssActiveRow - 1, cpcScope, 0 );

         ProcessImmediateEvent( this, SSN_ENTERCELL );   // 72 = 0x48
         m_uStateFlags &= ~zSS_RETURNED_PRIOR;
      }
      else
      {
         if ( (m_uStateFlags & zSS_RETURNED_PRIOR) &&
              (m_uStateFlags & zSS_ERROR) == 0 )
         {
            m_uStateFlags |= zSS_CELLDIRTY;
            MapToOI( );
         }

         SetActiveCell( m_lReturnOldCol, m_lReturnOldRow );
         m_uStateFlags |= zSS_RETURNED_PRIOR;
      }
   }

   m_ulMapActFlags &= ~zSS_RECURSIVE_EVENT;
}

void
ZSS::OnDataChange( )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnDataChange ... need to map??? ", (zLONG) this );
#endif

   // Data is about to be changed in a cell
   m_uStateFlags |= zSS_CELLDIRTY;

#if 0
   if ( MapToOI( ) )
   {
      GetActiveCell( &m_lReturnOldCol, &m_lReturnOldRow );
   }
   else
   {
      m_lReturnOldRow = 0;
      m_lReturnOldCol = 0;
      m_uStateFlags &= ~zSS_RETURNED_PRIOR;
   }
#endif
}

void
ZSS::OnDblClk( )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnDblClk ", (zLONG) this );
#endif

   zVIEW    vApp;
   SS_COORD ssRow;
   SS_COORD ssCol;

   if ( GetViewByName( &vApp, m_csViewName,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      zCPCHAR  cpcScope;
      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      GetActiveCell( &ssCol, &ssRow );
      if ( ssRow > 0 &&
           SetEntityCursor( vApp, *m_pzsEName, 0,
                            zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                            ssRow - 1, cpcScope, 0 ) > zCURSOR_UNCHANGED )
      {
         SetSelectStateOfEntity( vApp, *m_pzsEName, TRUE );
      }
   }

   ProcessEvent( this, SSM_DBLCLK );
}

void
ZSS::OnBlockSelected( )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::BlockSelected ", (zLONG) this );
#endif

   zVIEW    vApp;
   SS_COORD ssRow, ssCol;

   if ( GetViewByName( &vApp, m_csViewName,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      zCPCHAR  cpcScope;
      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      GetActiveCell( &ssCol, &ssRow );
      if ( ssRow > 0 &&
           SetEntityCursor( vApp, *m_pzsEName, 0,
                            zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                            ssRow - 1, cpcScope, 0 ) > zCURSOR_UNCHANGED )
      {
         SetSelectStateOfEntity( vApp, *m_pzsEName, TRUE );
      }
   }

   ProcessEvent( this, SSN_BLOCKSELECTED );
}

void
ZSS::OnButtonUp( )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnButtonUp ", (zLONG) this );
#endif

   zVIEW    vApp;
   SS_COORD nRow, nCol;

   if ( GetViewByName( &vApp, m_csViewName,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      zCPCHAR  cpcScope;
      if ( m_pzsScope && *m_pzsScope )
         cpcScope = *m_pzsScope;
      else
         cpcScope = 0;

      GetActiveCell( &nCol, &nRow );
      if ( nRow > 0 &&
           SetEntityCursor( vApp, *m_pzsEName, 0,
                            zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                            nRow - 1, cpcScope, 0 ) > zCURSOR_UNCHANGED )
      {
         SetSelectStateOfEntity( vApp, *m_pzsEName, TRUE );
      }
   }

   ProcessEvent( this, 1 );
}

void
ZSS::OnSize( UINT sizeType, int cx, int cy )
{
// CWnd::OnSize( sizeType, size );        // this causes stack fault

#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnSize ", (zLONG) this );
#endif

   CRect   rect;
   zLONG   lHeight;        // height of row;
   zSHORT  nVisibleRows;

   GetRowHeightInPixels( 1, &lHeight );
   GetClientRect( rect );
   nVisibleRows = ((rect.bottom - rect.top) / lHeight) + 1;
// TraceLineI( "ZSS::OnSize rows = ", nVisibleRows );

   if ( nVisibleRows >= 3 && nVisibleRows != m_nVisibleRows )
   {
#ifdef DEBUG_ALL
      TraceLineI( "ZSS::OnSize prev display rows = ", m_nVisibleRows );
      TraceLineI( "ZSS::OnSize curr display rows = ", nVisibleRows );
#endif
      m_nVisibleRows = nVisibleRows;

//    if ( (m_ulSubtype & SS_NOINTEGRALHEIGHT) == 0 )
         SetBool( SSB_AUTOSIZE, TRUE );

      SetAutoSizeVisible( m_nVisibleCols, m_nVisibleRows );

//    if ( (m_ulSubtype & SS_NOINTEGRALHEIGHT) == 0 )
         SetBool( SSB_AUTOSIZE, FALSE );

//    VSetBufferSize( m_nVisibleRows, 0 );  now defaults to 256, 0

      SS_COORD ssRow;
      SS_COORD ssCol;

      // there is a possibility that remapping may have to occur for
      // the virtual spreadsheet, so force it to happen ... set up
      // so that MapFromOI will map all rows
      GetTopLeftCell( &ssCol, &ssRow );
      SetStartingNumbers( ssCol, 0 );
      SetStartingNumbers( ssCol, ssRow );
   }
}

#ifdef DEBUG_ALL
zPCHAR g_szScrollCode[ 10 ] =
{
   "SB_LINEUP/LEFT",
   "SB_LINEDOWN/RIGHT",
   "SB_PAGEUP/LEFT",
   "SB_PAGEDOWN/RIGHT",
   "SB_THUMBPOSITION",
   "SB_THUMBTRACK",
   "SB_TOP/LEFT",
   "SB_BOTTOM/RIGHT",
   "SB_ENDSCROLL"
};
#endif

void
ZSS::OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnHScroll ", (zLONG) this );
   TraceLineS( "ZSS::OnHScroll SBCode: ", g_szScrollCode[ nSBCode ] );
#endif

   CWnd::OnHScroll( nSBCode, uThumbPos, pScrollBar );
   SSScrollEvent( nSBCode, uThumbPos, m_hWnd, FALSE );
}

void
ZSS::OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZSS::OnVScroll ", *m_pzsTag );
   TraceLineS( "ZSS::OnVScroll SBCode: ", g_szScrollCode[ nSBCode ] );
#endif

   SS_COORD ssRow;
   SS_COORD ssCol;

   if ( GetActiveCell( &ssCol, &ssRow ) && ssCol && ssRow )
   {
      SS_COORD ssRowCurr;
      SS_COORD ssColCurr;

      GetTopLeftCell( &ssColCurr, &ssRowCurr );

      if ( (nSBCode == SB_LINEDOWN && ssRow == ssRowCurr) ||
           (nSBCode == SB_LINEUP && ssRow == ssRowCurr + m_nVisibleRows - 1) ||
           ((nSBCode != SB_LINEUP && nSBCode != SB_LINEDOWN &&
             nSBCode != SB_ENDSCROLL &&
             (ssRow >= ssRowCurr && ssRow < ssRowCurr + m_nVisibleRows))) )
      {
         MapRowToOI( 0, ssRow );
      }
   }

   CWnd::OnVScroll( nSBCode, uThumbPos, pScrollBar );
   SSScrollEvent( nSBCode, uThumbPos, m_hWnd, TRUE );
}

void
ZSS::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnSetFocus ", (zLONG) this );
#endif

   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
      Default( );             // pass it along to parent
}

void
ZSS::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::OnKillFocus ", (zLONG) this );
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

zSHORT
ZSS::LockColumn( zSHORT nCol,
                 zBOOL  bLock )
{
   zLONG    lCols = *((zPLONG) m_pchListInfo);
   zPCHAR   pch = m_pchListInfo + sizeof( zLONG ) + sizeof( zULONG );
   zULONG   ulStyle;
   zULONG   ulSubtype;
   zSHORT   k;

   for ( k = 1; k <= lCols; k++ )
   {
      if ( *pch )
         pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past entity name
      else
         pch += sizeof( zLONG );

      pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past attribute
      pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past context
      ulStyle = *((zPLONG) pch);
      pch += sizeof( zLONG );     // skip past cell style
      ulSubtype = *((zPLONG) pch);
      if ( k == nCol )
      {
         ulSubtype &= ~zCONTROL_SIZEABLEBORDER;
         *((zPULONG) pch) = ulSubtype;
         break;
      }

      pch += sizeof( zLONG );     // skip past Subtype

      switch ( ulStyle & 0x0FFF0000L )
      {
         case zGRID_CHECKBOX:
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past Text
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past ON
            pch += sizeof( zLONG ) + *((zPLONG) pch); // skip past OFF
            break;

         default:
            break;
      }
   }

   return( SetLock( nCol, SS_ALLROWS, bLock ) );
}

zBOOL
ZSS::SetColumnTitle( zPCHAR pchTitle, SS_COORD ssColumn )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZSS::SetColumnTitle ", ssColumn );
#endif

   return( SetData( ssColumn, SS_HEADER, pchTitle ) );
}

zSHORT
ZSS::GetColumnTitle( zPCHAR pchTitle, SS_COORD ssColumn )
{
   return( GetData( ssColumn, SS_HEADER, pchTitle ) );
}

LRESULT CALLBACK
SpreadSheetCallBack( HWND hWnd, WORD wMsg,
                     WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineI( "SpreadSheetCallback ", wMsg );
#endif

   zLONG lRC = 0;

   switch ( wMsg )
   {
      case SSM_VQUERYDATA:
      {
         ZSS *pSS = DYNAMIC_DOWNCAST( ZSS, CWnd::FromHandle( hWnd ) );
      // TraceLineX( "SpreadSheetCallback pSS: ", (zLONG) pSS );
         if ( pSS == 0 )
            break;

         LPSS_VQUERYDATA pVQuery = (LPSS_VQUERYDATA) lParam;

//       TraceLineI( "SSM_VQUERYDATA Row ", pVQuery->ssRow );
//       TraceLineI( "               RowsNeeded ", pVQuery->ssRowsNeeded );
//       TraceLineI( " ------------- wDirection ", pVQuery->wDirection );

// SS_COORD ssRow;                       // Virtual top row to begin
//                                     // assignment of data
// SS_COORD ssRowsNeeded;                // The number of rows requested
// SS_COORD ssRowsLoaded;                // The application should set the
//                                     // number of rows actually loaded
//                                     // before returning
// WORD     wDirection;                // Direction to retrieve data
//                                     //    SS_VDOWN
//                                     //    SS_VUP
//                                     //    SS_VTOP
//                                     //    SS_VBOTTOM
//                                     //    SS_VREFRESH
// zBOOL    fAtTop;                    // The application should set this
//                                     // flag to TRUE if the top of the
//                                     // data has been reached
// zBOOL    fAtBottom;                 // The application should set this
//                                     // flag to TRUE if the bottom of the
//                                     // data has been reached

         if ( pVQuery->wDirection == SS_VUP )
            pVQuery->ssRow = max( 0, pVQuery->ssRow - pVQuery->ssRowsNeeded );
         else
         if ( pVQuery->wDirection == SS_VBOTTOM )
            pVQuery->ssRow = max( 0, pVQuery->ssRow - pVQuery->ssRowsNeeded );

         pSS->MapRowsFromOI( pVQuery->ssRow, pVQuery->ssRowsNeeded,
                             pSS->m_nVisibleRows );

         pVQuery->ssRowsLoaded = pVQuery->ssRowsNeeded;
         pVQuery->fAtTop = FALSE;
         pVQuery->fAtBottom = FALSE;

#if 0
         if ( pSS->m_uStateFlags & zSS_INITIAL )
         {
            pSS->m_uStateFlaglags &= ~zSS_INITIAL;
            lRC = 1;
            break;
         }

         if ( pSS->m_uStateFlags & zSS_COMPLETE )
         {
            LPSS_VQUERYDATA pVQuery = (LPSS_VQUERYDATA) lParam;

            if ( abs( pVQuery->ssRow - pSS->m_lTopEntityPos ) == 1 )
            {
               if ( pVQuery->wDirection == SS_VUP )   // going 1 row up
               {
                  pSS->m_uStateFlags |= zSS_UPDATETOP;
               }
               else
               if ( pVQuery->wDirection == SS_VDOWN ) // going 1 row down
               {
                  pSS->m_uStateFlags |= zSS_UPDATEBOTTOM;
               }
            }

            if ( pSS->m_lTopEntityPos != (UINT) pVQuery->ssRow )
            {
               pSS->m_lTopEntityPos = pVQuery->ssRow;
               if ( pSS->ShowData( pSS->m_pZView,
                    eFMDBC_SYNC_CTRL_PRIVATE ) != eFMDBC_RC_SUCCESS )
               {
                  AfxMessageBox( "Unable to show data" );
               }
            }

            lRC = 1;
         }
#endif
         lRC = 1;
         break;
      }

#if 0    // we don't need this yet...

      case SSM_COLWIDTHCHANGE:
      {
         SS_COORDRANGE *pRange = (LPSS_COORDRANGE) lParam;

         WORD wColumn = pRange->Coord1;
         double dNewColWidth;

         pSS->SSGetColWidth( wColumn, &dNewColWidth );

         FMDBM_FLDID FldId = pSS->GetFieldForColumn( wColumn );

         pSS->SetColumnWidth( FldId, (float) dNewColWidth );

         lRC = 1;
         break;
      }

#endif

      default:
         break;
   }

   return( lRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL
ZSS::GetBool( zSHORT nIndex )
{
   return( SSGetBool( m_hWnd, nIndex ) );
}

zBOOL
ZSS::SetBool( zSHORT nIndex, zBOOL bNewVal )
{
   return( SSSetBool( m_hWnd, nIndex, bNewVal ) );
}

zBOOL
ZSS::AddCustomFunction( zPCHAR  FunctionName,
                        zSHORT  ParameterCnt,
                        FARPROC lpfnFunctionProc )
{
   return( SSAddCustomFunction( m_hWnd, FunctionName,
                                ParameterCnt, lpfnFunctionProc ) );
}

zBOOL
ZSS::AddMultiSelBlocks( LPSS_SELBLOCK lpSelBlock )
{
   return( SSAddMultiSelBlocks( m_hWnd, lpSelBlock ) );
}

zBOOL
ZSS::BuildDependencies( )
{
   return( SSBuildDependencies( m_hWnd ) );
}

zBOOL
ZSS::Clear( )
{
   return( SSClear( m_hWnd, SS_ALLCOLS, SS_ALLROWS ) );
}

zBOOL
ZSS::Clear( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSClear( m_hWnd, ssCol, ssRow ) );
}

zBOOL
ZSS::ClearData( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSClearData( m_hWnd, ssCol, ssRow ) );
}

zBOOL
ZSS::ClearDataRange( SS_COORD ssCol,  SS_COORD ssRow,
                     SS_COORD ssCol2, SS_COORD ssRow2 )
{
   return( SSClearDataRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2 ) );
}

zBOOL
ZSS::ClearRange( SS_COORD ssCol,  SS_COORD ssRow,
                 SS_COORD ssCol2, SS_COORD ssRow2 )
{
   return( SSClearRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2 ) );
}

zBOOL
ZSS::ClipboardCopy( )
{
   return( (zBOOL) SSClipboardCopy( m_hWnd ) );
}

zBOOL
ZSS::ClipboardCut( )
{
   return( SSClipboardCut( m_hWnd ) );
}

zBOOL
ZSS::ClipboardPaste( )
{
   return( SSClipboardPaste( m_hWnd ) );
}

zBOOL
ZSS::ClipIn( SS_COORD ssCol,  SS_COORD ssRow,
             SS_COORD ssCol2, SS_COORD ssRow2,
             zPCHAR Buffer, zLONG lBufferLth )
{
   return( SSClipIn( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, Buffer, lBufferLth ) );
}

GLOBALHANDLE
ZSS::ClipOut( SS_COORD ssCol, SS_COORD ssRow,
              SS_COORD ssCol2,SS_COORD ssRow2 )
{
   return( SSClipOut( m_hWnd, ssCol, ssRow, ssCol2, ssRow2 ) );
}

zBOOL
ZSS::ClipValueIn( SS_COORD ssCol,  SS_COORD ssRow,
                  SS_COORD ssCol2, SS_COORD ssRow2,
                  zPCHAR lpBuffer, zLONG lBufferLth )
{
   return( (zBOOL) SSClipValueIn( m_hWnd, ssCol, ssRow,
                                  ssCol2, ssRow2, lpBuffer, lBufferLth ) );
}

GLOBALHANDLE
ZSS::ClipValueOut( SS_COORD ssCol,  SS_COORD ssRow,
                   SS_COORD ssCol2, SS_COORD ssRow2 )
{
   return( (GLOBALHANDLE) SSClipValueOut( m_hWnd, ssCol, ssRow, ssCol2, ssRow2 ) );
}

zSHORT
ZSS::ColWidthToLogUnits( zLONG dxColWidth )
{
   return( SSColWidthToLogUnits( m_hWnd, dxColWidth ) );
}

zLONG
ZSS::ComboBoxSendMessage( zSHORT dCol, zSHORT dRow,
                          WORD uMsg, WPARAM wParam, LPARAM lParam )
{
   return( SSComboBoxSendMessage( m_hWnd, dCol, dRow, uMsg, wParam, lParam ) );
}

#ifdef DKS_COPY
zBOOL
ZSS::CopyRange( SS_COORD ssCol, SS_COORD ssRow,
                SS_COORD ssCol2, SS_COORD ssRow2,
                SS_COORD ssColDest, SS_COORD ssRowDest )
{
   return( SSCopyRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2,
                        ssColDest, ssRowDest ) );
}
#endif

zBOOL
ZSS::DelCol( SS_COORD ssCol )
{
   return( SSDelCol( m_hWnd, ssCol ) );
}

zBOOL
ZSS::DelColRange( SS_COORD ssCol, SS_COORD ssCol2 )
{
   return( SSDelColRange( m_hWnd, ssCol, ssCol2 ) );
}

zBOOL
ZSS::DelRow( SS_COORD ssRow )
{
   return( SSDelRow( m_hWnd, ssRow ) );
}

zBOOL
ZSS::DelRowRange( SS_COORD ssRow, SS_COORD ssRow2 )
{
   return( SSDelRowRange( m_hWnd, ssRow, ssRow2 ) );
}

zBOOL
ZSS::DeSelectBlock( )
{
   return( SSDeSelectBlock( m_hWnd ) );
}

void
ZSS::FloatFormat( LPSS_CELLTYPE lpCellType,
                  zDECIMAL dxVal,
                  zPCHAR   lpszText,
                  zBOOL    fValue )
{
   SSFloatFormat( m_hWnd, lpCellType, dxVal, lpszText, fValue );
}

zBOOL
ZSS::GetActiveCell( LPSS_COORD ssCol, LPSS_COORD ssRow )
{
   return( SSGetActiveCell( m_hWnd, ssCol, ssRow ) );
}

void
ZSS::GetAutoSizeVisible( LPSS_COORD lpVisibleCols,
                         LPSS_COORD lpVisibleRows )
{
   SSGetAutoSizeVisible( m_hWnd, lpVisibleCols, lpVisibleRows );
}

zBOOL
ZSS::GetBorder( SS_COORD ssCol, SS_COORD ssRow,
                LPWORD lpwBorderType, LPWORD lpwBorderStyle,
                LPCOLORREF lpColor )
{
   return( SSGetBorder( m_hWnd, ssCol, ssRow,
                        lpwBorderType, lpwBorderStyle, lpColor ) );
}

zLONG
ZSS::GetBottomRightCell( LPSS_COORD ACol, LPSS_COORD ARow )
{
   return( SSGetBottomRightCell( m_hWnd, ACol, ARow ) );
}

WORD
ZSS::GetButtonDrawMode( )
{
   return( SSGetButtonDrawMode( m_hWnd ) );
}

zBOOL
ZSS::GetButtonText( SS_COORD ssCol, SS_COORD ssRow, zPCHAR lpText )
{
   return( SSGetButtonText( m_hWnd, ssCol, ssRow, lpText ) );
}

WNDPROC
ZSS::GetCallBack( )
{
   return( SSGetCallBack( m_hWnd ) );
}

zBOOL
ZSS::GetCellFromPixel( LPSS_COORD lpCol, LPSS_COORD lpRow,
                       zSHORT MouseX, zSHORT MouseY )
{
   return( SSGetCellFromPixel( m_hWnd, lpCol, lpRow, MouseX, MouseY ) );
}

zBOOL
ZSS::GetCellRect( SS_COORD ssCol, SS_COORD ssRow, LPRECT lpRect )
{
   return( SSGetCellRect( m_hWnd, ssCol, ssRow, lpRect ) );
}

void
ZSS::GetCellSendingMsg( LPSS_COORD lpCol, LPSS_COORD lpRow )
{
   SSGetCellSendingMsg( m_hWnd, lpCol, lpRow );
}

zBOOL
ZSS::GetCellType( SS_COORD ssCol, SS_COORD ssRow,
                  LPSS_CELLTYPE CellType )
{
   return( SSGetCellType( m_hWnd, ssCol, ssRow, CellType ) );
}

void
ZSS::GetClientRect( LPRECT lpRect )
{
   SSGetClientRect( m_hWnd, lpRect );
}

zBOOL
ZSS::GetColPageBreak( SS_COORD col )
{
   return( (zBOOL) SSGetColPageBreak( m_hWnd, col ) );
}

zBOOL
ZSS::SetColPageBreak( SS_COORD col, zBOOL bPageBreak )
{
   return( (zBOOL) SSSetColPageBreak( m_hWnd, col, bPageBreak ) );
}

zBOOL
ZSS::GetColor( SS_COORD ssCol, SS_COORD ssRow,
               LPCOLORREF lpBackground, LPCOLORREF lpForeground )
{
   return( SSGetColor( m_hWnd, ssCol, ssRow, lpBackground, lpForeground ) );
}

#ifndef SS_NOUSERDATA
zBOOL
ZSS::GetColUserData( SS_COORD ssCol, LPLONG lplUserData )
{
   return( SSGetColUserData( m_hWnd, ssCol, lplUserData ) );
}
#endif

zBOOL
ZSS::GetColWidth( SS_COORD ssCol, zPLONG lpColWidth )
{
   return( SSGetColWidth( m_hWnd, ssCol, lpColWidth ) );
}

zBOOL
ZSS::GetColWidthInPixels( SS_COORD ssCol, zPLONG lpdWidthPixels )
{
   return( SSGetColWidthInPixels( m_hWnd, ssCol, lpdWidthPixels ) );
}

zBOOL
ZSS::GetCurrSelBlockPos( LPSS_COORD CurrentCol,
                         LPSS_COORD CurrentRow )
{
   return( SSGetCurrSelBlockPos( m_hWnd, CurrentCol, CurrentRow ) );
}

HCURSOR
ZSS::GetCursor( WORD wType )
{
   return( SSGetCursor( m_hWnd, wType ) );
}

zSHORT
ZSS::GetData( SS_COORD ssCol, SS_COORD ssRow, zPCHAR Data )
{
   return( SSGetData( m_hWnd, ssCol, ssRow, Data ) );
}

zBOOL
ZSS::GetDataCnt( LPSS_COORD lpColCnt, LPSS_COORD lpRowCnt )
{
   return( SSGetDataCnt( m_hWnd, lpColCnt, lpRowCnt ) );
}

zSHORT
ZSS::GetDataLth( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSGetDataLth( m_hWnd, ssCol, ssRow ) );
}

zBOOL
ZSS::GetDefFloatFormat( LPFLOATFORMAT lpFloatFormat )
{
   return( SSGetDefFloatFormat( m_hWnd, lpFloatFormat ) );
}

WORD
ZSS::GetEditEnterAction( )
{
   return( SSGetEditEnterAction( m_hWnd ) );
}

zBOOL
ZSS::GetEditMode( )
{
   return( SSGetEditMode( m_hWnd ) );
}

zBOOL
ZSS::GetFirstValidCell( LPSS_COORD lpCol, LPSS_COORD lpRow )
{
   return( SSGetFirstValidCell( m_hWnd, lpCol, lpRow ) );
}

zBOOL
ZSS::GetLastValidCell( LPSS_COORD lpCol, LPSS_COORD lpRow )
{
   return( SSGetLastValidCell( m_hWnd, lpCol, lpRow ) );
}

zBOOL
ZSS::GetRowPageBreak( SS_COORD row )
{
   return( SSGetRowPageBreak( m_hWnd, row ) );
}

zBOOL
ZSS::GetFloat( SS_COORD ssCol, SS_COORD ssRow, zDECIMAL *pdxValue )
{
   return( SSGetFloat( m_hWnd, ssCol, ssRow, pdxValue ) );
}

HFONT
ZSS::GetFont( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSGetFont( m_hWnd, ssCol, ssRow ) );
}

zSHORT
ZSS::GetFormula( SS_COORD ssCol, SS_COORD ssRow, zPCHAR lpFormula )
{
   return( SSGetFormula( m_hWnd, ssCol, ssRow, lpFormula ) );
}

zSHORT
ZSS::GetFormulaLth( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSGetFormulaLth( m_hWnd, ssCol, ssRow ) );
}

zBOOL
ZSS::GetFreeze( LPSS_COORD lpColsFrozen,
                LPSS_COORD lpRowsFrozen )
{
   return( SSGetFreeze( m_hWnd, lpColsFrozen, lpRowsFrozen ) );
}

void
ZSS::GetGrayAreaColor( LPCOLORREF lpBackground,
                       LPCOLORREF lpForeground )
{
   SSGetGrayAreaColor( m_hWnd, lpBackground, lpForeground );
}

WORD
ZSS::GetGridType( )
{
   return( SSGetGridType( m_hWnd ) );
}

COLORREF
ZSS::GetGridColor( )
{
   return( SSGetGridColor( m_hWnd ) );
}

zBOOL
ZSS::GetInteger( SS_COORD ssCol, SS_COORD ssRow, LPLONG lplValue )
{
   return( SSGetInteger( m_hWnd, ssCol, ssRow, lplValue ) );
}

zBOOL
ZSS::GetLock( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSGetLock( m_hWnd, ssCol, ssRow ) );
}

void
ZSS::GetLockColor( LPCOLORREF lpBackground,
                   LPCOLORREF lpForeground )
{
   SSGetLockColor( m_hWnd, lpBackground, lpForeground );
}

SS_COORD
ZSS::GetMaxCols( )
{
   return( SSGetMaxCols( m_hWnd ) );
}

SS_COORD
ZSS::GetMaxRows( )
{
   return( SSGetMaxRows( m_hWnd ) );
}

zBOOL
ZSS::GetMaxTextCellSize( SS_COORD ssCol, SS_COORD ssRow,
                         zPLONG pdxWidth, zPLONG pdxHeight )
{
   return( SSGetMaxTextCellSize( m_hWnd, ssCol, ssRow, pdxWidth, pdxHeight ) );
}

zBOOL
ZSS::GetMaxTextColWidth( SS_COORD ssCol, zPLONG pdxColWidth )
{
   return( SSGetMaxTextColWidth( m_hWnd, ssCol, pdxColWidth ) );
}

zBOOL
ZSS::GetMaxTextRowHeight( SS_COORD ssRow, zPLONG pdxRowHeight )
{
   return( SSGetMaxTextRowHeight( m_hWnd, ssRow, pdxRowHeight ) );
}

zPCHAR
ZSS::GetMultiSelBlocks( zPLONG lpdSelBlockCnt )
{
   return( SSGetMultiSelBlocks( m_hWnd, lpdSelBlockCnt ) );
}

WORD
ZSS::GetOperationMode( )
{
   return( SSGetOperationMode( m_hWnd ) );
}

HWND
ZSS::GetOwner( )
{
   return( SSGetOwner( m_hWnd ) );
}

zBOOL
ZSS::GetRowHeight( SS_COORD ssRow, zPLONG lpHeight )
{
   return( (zBOOL) SSGetRowHeight( m_hWnd, ssRow, lpHeight ) );
}

zBOOL
ZSS::GetRowHeightInPixels( SS_COORD ssRow, zPLONG lpdHeightPixels )
{
   return( SSGetRowHeightInPixels( m_hWnd, ssRow, lpdHeightPixels ) );
}

#ifndef SS_NOUSERDATA
zBOOL
ZSS::GetRowUserData( SS_COORD ssRow, LPLONG lplUserData )
{
   return( SSGetRowUserData( m_hWnd, ssRow, lplUserData ) );
}
#endif

zBOOL
ZSS::GetSel( zPLONG lSelStart, zPLONG lSelEnd )
{
   return( SSGetSel( m_hWnd, lSelStart, lSelEnd ) );
}

WORD
ZSS::GetSelBlockOptions( )
{
   return( SSGetSelBlockOptions( m_hWnd ) );
}

zBOOL
ZSS::GetSelectBlock( LPSS_CELLCOORD CellUL, LPSS_CELLCOORD CellLR )
{
   return( SSGetSelectBlock( m_hWnd, CellUL, CellLR ) );
}

zPCHAR
ZSS::GetSelText( )
{
   return( SSGetSelText( m_hWnd ) );
}

void
ZSS::GetShadowColor( LPCOLORREF lpShadowColor,
                     LPCOLORREF lpShadowText,
                     LPCOLORREF lpShadowDark,
                     LPCOLORREF lpShadowLight )
{
   SSGetShadowColor( m_hWnd, lpShadowColor, lpShadowText,
                     lpShadowDark, lpShadowLight );
}

zSHORT
ZSS::GetUserResizeCol( SS_COORD ssCol )
{
   return( SSGetUserResizeCol( m_hWnd, ssCol ) );
}

zSHORT
ZSS::GetUserResizeRow( SS_COORD ssRow )
{
   return( SSGetUserResizeRow( m_hWnd, ssRow ) );
}

zLONG
ZSS::GetTopLeftCell( LPSS_COORD ACol, LPSS_COORD ARow )
{
   return( SSGetTopLeftCell( m_hWnd, ACol, ARow ) );
}

WORD
ZSS::GetUnitType( )
{
   return( SSGetUnitType( m_hWnd ) );
}

#ifndef SS_NOUSERDATA
zLONG
ZSS::GetUserData( )
{
   return( SSGetUserData( m_hWnd ) );
}
#endif

WORD
ZSS::GetUserResize( )
{
   return( SSGetUserResize( m_hWnd ) );
}

zSHORT
ZSS::GetValue( SS_COORD ssCol, SS_COORD ssRow, zPCHAR lpBuffer )
{
   return( SSGetValue( m_hWnd, ssCol, ssRow, lpBuffer ) );
}

zSHORT
ZSS::GetValueLth( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSGetValueLth( m_hWnd, ssCol, ssRow ) );
}

zBOOL
ZSS::GetVisible( SS_COORD ssCol, SS_COORD ssRow, zSHORT Visible )
{
   return( SSGetVisible( m_hWnd, ssCol, ssRow, Visible ) );
}

zBOOL
ZSS::InsCol( SS_COORD ssCol )
{
   return( SSInsCol( m_hWnd, ssCol ) );
}

zBOOL
ZSS::InsColRange( SS_COORD ssCol, SS_COORD ssCol2 )
{
   return( SSInsColRange( m_hWnd, ssCol, ssCol2 ) );
}

zBOOL
ZSS::InsRow( SS_COORD ssRow )
{
   return( SSInsRow( m_hWnd, ssRow ) );
}

zBOOL
ZSS::InsRowRange( SS_COORD ssRow, SS_COORD ssRow2 )
{
   return( SSInsRowRange( m_hWnd, ssRow, ssRow2 ) );
}

zBOOL
ZSS::IsCellInSelection( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSIsCellInSelection( m_hWnd, ssCol, ssRow ) );
}

zBOOL
ZSS::LoadFromBuffer( zPCHAR hBuffer, zLONG lBufferLth )
{
   return( SSLoadFromBuffer( m_hWnd, hBuffer, lBufferLth ) );
}

zBOOL
ZSS::LoadFromFile( zPCHAR lpFileName )
{
   return( SSLoadFromFile( m_hWnd, lpFileName ) );
}

void
ZSS::LogUnitsToColWidth( zSHORT dUnits, zPLONG lpWidth )
{
   SSLogUnitsToColWidth( m_hWnd, dUnits, lpWidth );
}

void
ZSS::LogUnitsToRowHeight( SS_COORD ssRow, zSHORT dUnits, zPLONG lpHeight )
{
   SSLogUnitsToRowHeight( m_hWnd, ssRow, dUnits, lpHeight );
}

#ifdef DKS_COPY
zBOOL
ZSS::MoveRange( SS_COORD ssCol,  SS_COORD ssRow,
                SS_COORD ssCol2, SS_COORD ssRow2,
                SS_COORD ssColDest, SS_COORD ssRowDest )
{
   return( SSMoveRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, ssColDest, ssRowDest ) );
}
#endif

zBOOL
ZSS::Print( zPCHAR  lpszPrintJobName,
            LPSS_PRINTFORMAT lpPrintFormat,
            FARPROC lpfnPrintProc,
            zLONG   lAppData,
            FARPROC lpfnAbortProc )
{
   return( SSPrint( m_hWnd, lpszPrintJobName, lpPrintFormat,
                    lpfnPrintProc, lAppData, lpfnAbortProc ) );
}

zBOOL
ZSS::ReCalc( )
{
// SS_NOCALC is defined
   //x return( SSReCalc( m_hWnd ) );
   return( FALSE );
}

zBOOL
ZSS::ReplaceSel( zPCHAR lpText )
{
   return( SSReplaceSel( m_hWnd, lpText ) );
}

zSHORT
ZSS::RowHeightToLogUnits( SS_COORD ssRow, zLONG dxRowHeight )
{
   return( RowHeightToLogUnits( ssRow, dxRowHeight ) );
}

GLOBALHANDLE
ZSS::SaveToBuffer( zPLONG lpBufferLth, zBOOL fDataOnly )
{
   return( SSSaveToBuffer( m_hWnd, lpBufferLth, fDataOnly ) );
}

zBOOL
ZSS::SaveToFile( zPCHAR lpFileName, zBOOL fDataOnly )
{
   return( SSSaveToFile( m_hWnd, lpFileName, fDataOnly ) );
}

zBOOL
ZSS::SetActiveCell( SS_COORD ssCol, SS_COORD ssRow )
{
   return( SSSetActiveCell( m_hWnd, ssCol, ssRow ) );
}

void
ZSS::SetAutoSizeVisible( zSHORT VisibleCols, zSHORT VisibleRows )
{
   SSSetAutoSizeVisible( m_hWnd, VisibleCols, VisibleRows );
}

zBOOL
ZSS::SetBorder( SS_COORD ssCol,
                SS_COORD ssRow,
                WORD wBorderType,
                WORD wBorderStyle,
                COLORREF Color )
{
   return( SSSetBorder( m_hWnd, ssCol, ssRow, wBorderType, wBorderStyle, Color ) );
}

zBOOL
ZSS::SetBorderRange( SS_COORD ssCol,  SS_COORD ssRow,
                     SS_COORD ssCol2, SS_COORD ssRow2,
                     WORD wBorderType, WORD wBorderStyle,
                     COLORREF Color )
{
   return( SSSetBorderRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2,
                             wBorderType, wBorderStyle, Color ) );
}

WORD
ZSS::SetButtonDrawMode( WORD wMode )
{
   return( SSSetButtonDrawMode( m_hWnd, wMode ) );
}

zBOOL
ZSS::SetButtonText( SS_COORD ssCol, SS_COORD ssRow, zPCHAR lpText )
{
   return( SSSetButtonText( m_hWnd, ssCol, ssRow, lpText ) );
}

WNDPROC
ZSS::SetCallBack( WNDPROC lpfnCallBack )
{
   return( SSSetCallBack( m_hWnd, lpfnCallBack ) );
}

zBOOL
ZSS::SetCellType( SS_COORD ssCol, SS_COORD ssRow,
                  LPSS_CELLTYPE CellType )
{
   return( SSSetCellType( m_hWnd, ssCol, ssRow, CellType ) );
}

zBOOL
ZSS::SetCellTypeRange( SS_COORD ssCol,  SS_COORD ssRow,
                       SS_COORD ssCol2, SS_COORD ssRow2,
                       LPSS_CELLTYPE CellType )
{
   return( SSSetCellTypeRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, CellType ) );
}

zBOOL
ZSS::SetColor( SS_COORD ssCol, SS_COORD ssRow,
               COLORREF Background, COLORREF Foreground )
{
   return( SSSetColor( m_hWnd, ssCol, ssRow, Background, Foreground ) );
}

zBOOL
ZSS::SetColorRange( SS_COORD ssCol, SS_COORD ssRow,
                    SS_COORD ssCol2, SS_COORD ssRow2,
                    COLORREF Background, COLORREF Foreground )
{
   return( SSSetColorRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2,
                            Background, Foreground ) );
}

#ifndef SS_NOUSERDATA
zBOOL
ZSS::SetColUserData( SS_COORD ssCol, zLONG lUserData )
{
   return( SSSetColUserData( m_hWnd, ssCol, lUserData ) );
}
#endif

zBOOL
ZSS::SetColWidth( SS_COORD ssCol, zLONG lWidth )
{
   return( SSSetColWidth( m_hWnd, ssCol, lWidth ) );
}

zBOOL
ZSS::SetColWidthInPixels( SS_COORD ssCol, zLONG dWidth )
{
   return( SSSetColWidthInPixels( m_hWnd, ssCol, dWidth ) );
}

zBOOL
ZSS::SetCursor( WORD wType, HCURSOR hCursor )
{
   return( SSSetCursor( m_hWnd, wType, hCursor ) );
}

zBOOL
ZSS::SetData( SS_COORD ssCol, SS_COORD ssRow, zPCHAR pchData )
{
   return( SSSetData( m_hWnd, ssCol, ssRow, pchData ) );
}

zBOOL
ZSS::SetDataRange( SS_COORD ssCol,  SS_COORD ssRow,
                   SS_COORD ssCol2, SS_COORD ssRow2, zPCHAR lpData )
{
   return( SSSetDataRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, lpData ) );
}

zBOOL
ZSS::SetDefFloatFormat( LPFLOATFORMAT lpFloatFormat )
{
   return( SSSetDefFloatFormat( m_hWnd, lpFloatFormat ) );
}

WORD
ZSS::SetEditEnterAction( WORD wAction )
{
   return( SSSetEditEnterAction( m_hWnd, wAction ) );
}

zBOOL
ZSS::SetEditMode( zBOOL fEditModeOn )
{
   return( SSSetEditMode( m_hWnd, fEditModeOn ) );
}

zBOOL
ZSS::SetFont( SS_COORD ssCol, SS_COORD ssRow,
              CFont& hFont, zBOOL fDeleteFont )
{
   return( SSSetFont( m_hWnd, ssCol, ssRow, (HFONT) hFont, fDeleteFont ) );
}

zBOOL
ZSS::SetFontRange( SS_COORD ssCol,  SS_COORD ssRow,
                   SS_COORD ssCol2, SS_COORD ssRow2,
                   CFont& hFont, zBOOL fDeleteFont )
{
   return( SSSetFontRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2,
                           (HFONT) hFont, fDeleteFont ) );
}

zBOOL
ZSS::SetFormula( SS_COORD ssCol, SS_COORD ssRow,
                 zPCHAR Formula, zBOOL BuildDependencies )
{
   return( SSSetFormula( m_hWnd, ssCol, ssRow, Formula, BuildDependencies ) );
}

zBOOL
ZSS::SetFormulaRange( SS_COORD ssCol,  SS_COORD ssRow,
                      SS_COORD ssCol2, SS_COORD ssRow2,
                      zPCHAR Formula, zBOOL BuildDependencies )
{
   return( SSSetFormulaRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2,
                              Formula, BuildDependencies ) );
}

zBOOL
ZSS::SetFloat( SS_COORD ssCol, SS_COORD ssRow, zDECIMAL dxValue )
{
   return( SSSetFloat( m_hWnd, ssCol, ssRow, dxValue ) );
}

zBOOL
ZSS::SetFloatRange( SS_COORD ssCol, SS_COORD ssRow,
                    SS_COORD ssCol2, SS_COORD ssRow2, zDECIMAL dxValue )
{
   return( SSSetFloatRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, dxValue ) );
}

zBOOL
ZSS::SetFreeze( zSHORT nColsFrozen, zLONG lRowsFrozen )
{
   return( SSSetFreeze( m_hWnd, nColsFrozen, lRowsFrozen ) );
}

void
ZSS::SetGrayAreaColor( COLORREF Background, COLORREF Foreground )
{
   SSSetGrayAreaColor( m_hWnd, Background, Foreground );
}

WORD
ZSS::SetGridType( WORD wGridType )
{
   return( SSSetGridType( m_hWnd, wGridType ) );
}

COLORREF
ZSS::SetGridColor( COLORREF Color )
{
   return( SSSetGridColor( m_hWnd, Color ) );
}

zBOOL
ZSS::SetInteger( SS_COORD ssCol, SS_COORD ssRow, zLONG lValue )
{
   return( SSSetInteger( m_hWnd, ssCol, ssRow, lValue ) );
}

zBOOL
ZSS::SetIntegerRange( SS_COORD ssCol, SS_COORD ssRow,
                      SS_COORD ssCol2, SS_COORD ssRow2, zLONG lValue )
{
   return( SSSetIntegerRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, lValue ) );
}

zBOOL
ZSS::SetLock( SS_COORD ssCol, SS_COORD ssRow, zBOOL Lock )
{
   return( SSSetLock( m_hWnd, ssCol, ssRow, Lock ) );
}

void
ZSS::SetLockColor( COLORREF Background, COLORREF Foreground )
{
   SSSetLockColor( m_hWnd, Background, Foreground );
}

zBOOL
ZSS::SetLockRange( SS_COORD ssCol,  SS_COORD ssRow,
                   SS_COORD ssCol2, SS_COORD ssRow2, zBOOL Lock )
{
   return( SSSetLockRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, Lock ) );
}

void
ZSS::SetMaxCols( SS_COORD MaxCols )
{
   SSSetMaxCols( m_hWnd, MaxCols );
}

void
ZSS::SetMaxRows( SS_COORD MaxRows )
{
   SSSetMaxRows( m_hWnd, MaxRows );
}

zBOOL
ZSS::SetMultiSelBlocks( LPSS_SELBLOCK lpSelBlockList,
                        zLONG dSelBlockCnt )
{
   return( SSSetMultiSelBlocks( m_hWnd, lpSelBlockList, dSelBlockCnt ) );
}

WORD
ZSS::SetOperationMode( WORD wMode )
{
   return( SSSetOperationMode( m_hWnd, wMode ) );
}

HWND
ZSS::SetOwner( HWND hWndOwner )
{
   return( SSSetOwner( m_hWnd, hWndOwner ) );
}

zBOOL
ZSS::SetRowHeight( SS_COORD ssRow, zLONG Height )
{
   return( SSSetRowHeight( m_hWnd, ssRow, Height ) );
}

zBOOL
ZSS::SetRowHeightInPixels( SS_COORD ssRow, zSHORT dHeight )
{
   return( SSSetRowHeightInPixels( m_hWnd, ssRow, dHeight ) );
}

#ifndef SS_NOUSERDATA
zBOOL
ZSS::SetRowUserData( SS_COORD ssRow, zLONG lUserData )
{
   return( SSSetRowUserData( m_hWnd, ssRow, lUserData ) );
}
#endif

zBOOL
ZSS::SetSel( zLONG lSelStart, zLONG lSelEnd )
{
   return( SSSetSel( m_hWnd, lSelStart, lSelEnd ) );
}

zBOOL
ZSS::SetSelectBlock( LPSS_CELLCOORD CellUL, LPSS_CELLCOORD CellLR )
{
   return( SSSetSelectBlock( m_hWnd, CellUL, CellLR ) );
}

void
ZSS::SetShadowColor( COLORREF ShadowColor, COLORREF ShadowText,
                     COLORREF ShadowDark,  COLORREF ShadowLight )
{
   SSSetShadowColor( m_hWnd, ShadowColor, ShadowText,
                     ShadowDark, ShadowLight );
}

LPSS_CELLTYPE
ZSS::SetTypeButton( LPSS_CELLTYPE CellType, zLONG Style,
                    zPCHAR Text, zPCHAR Picture,
                    zSHORT nPictureType, zPCHAR PictureDown,
                    zSHORT nPictureDownType, zSHORT ButtonType,
                    zSHORT nShadowSize, LPSUPERBTNCOLOR lpColor )
{
   return( SSSetTypeButton( m_hWnd, CellType, Style,
                            Text, Picture, nPictureType,
                            PictureDown, nPictureDownType,
                            ButtonType, nShadowSize, lpColor ) );
}

LPSS_CELLTYPE
ZSS::SetTypeCheckBox( LPSS_CELLTYPE lpCellType,
                      zLONG lStyle, zPCHAR lpText,
                      zPCHAR lpPictUp, WORD wPictUpType,
                      zPCHAR lpPictDown, WORD wPictDownType,
                      zPCHAR lpPictFocusUp, WORD wPictFocusUpType,
                      zPCHAR lpPictFocusDown, WORD wPictFocusDownType,
                      zPCHAR lpPictDisabledUp, WORD wPictDisabledUpType,
                      zPCHAR lpPictDisabledDown, WORD wPictDisabledDownType )
{
   return( SSSetTypeCheckBox( m_hWnd, lpCellType, lStyle, lpText,
                              lpPictUp, wPictUpType,
                              lpPictDown, wPictDownType,
                              lpPictFocusUp, wPictFocusUpType,
                              lpPictFocusDown, wPictFocusDownType,
                              lpPictDisabledUp, wPictDisabledUpType,
                              lpPictDisabledDown, wPictDisabledDownType ) );
}

LPSS_CELLTYPE
ZSS::SetTypeComboBox( LPSS_CELLTYPE CellType,
                      zLONG Style, zPCHAR lpItems )
{
   return( SSSetTypeComboBox( m_hWnd, CellType, Style, lpItems ) );
}

LPSS_CELLTYPE
ZSS::SetTypeDate( LPSS_CELLTYPE CellType, zLONG Style,
                  LPDATEFORMAT lpFormat, LPDATE lpMin, LPDATE lpMax )
{
   return( SSSetTypeDate( m_hWnd, CellType, Style, lpFormat, lpMin, lpMax ) );
}

LPSS_CELLTYPE
ZSS::SetTypeEdit( LPSS_CELLTYPE CellType, zLONG Style, zSHORT nLth,
                  zSHORT ChrSet, zUSHORT uCase )
{
   return( SSSetTypeEdit( m_hWnd, CellType, Style, nLth, ChrSet, uCase ) );
}

LPSS_CELLTYPE
ZSS::SetTypeFloat( LPSS_CELLTYPE CellType, zLONG Style,
                   zSHORT Left, zSHORT Right, zDECIMAL dxMin, zDECIMAL dxMax )
{
   return( SSSetTypeFloat( m_hWnd, CellType, Style, Left, Right, dxMin, dxMax ) );
}

LPSS_CELLTYPE
ZSS::SetTypeFloatExt( LPSS_CELLTYPE CellType, zLONG Style,
                      zSHORT Left, zSHORT Right, zDECIMAL dxMin,
                      zDECIMAL dxMax, LPFLOATFORMAT lpFormat )
{
   return( SSSetTypeFloatExt( m_hWnd, CellType, Style,
                              Left, Right, dxMin, dxMax, lpFormat ) );
}

LPSS_CELLTYPE
ZSS::SetTypeInteger( LPSS_CELLTYPE CellType, zLONG Min, zLONG Max )
{
   return( SSSetTypeInteger( m_hWnd, CellType, Min, Max ) );
}

LPSS_CELLTYPE
ZSS::SetTypeIntegerExt( LPSS_CELLTYPE CellType, zLONG lStyle,
                        zLONG lMin, zLONG lMax,
                        zBOOL fSpinWrap, zLONG lSpinInc )
{
   return( SSSetTypeIntegerExt( m_hWnd, CellType, lStyle,
                                lMin, lMax, fSpinWrap, lSpinInc ) );
}

LPSS_CELLTYPE
ZSS::SetTypeOwnerDraw( LPSS_CELLTYPE lpCellType, zLONG lStyle )
{
   return( SSSetTypeOwnerDraw( m_hWnd, lpCellType, lStyle ) );
}

LPSS_CELLTYPE
ZSS::SetTypePic( LPSS_CELLTYPE CellType, zLONG Style, zPCHAR Mask )
{
   return( SSSetTypePic( m_hWnd, CellType, Style, Mask ) );
}

LPSS_CELLTYPE
ZSS::SetTypePicture( LPSS_CELLTYPE CellType, zLONG Style,
                              zPCHAR PictName )
{
   return( SSSetTypePicture( m_hWnd, CellType, Style, PictName ) );
}

LPSS_CELLTYPE
ZSS::SetTypePictureHandle( LPSS_CELLTYPE CellType, zLONG Style,
                           HANDLE hPict, HPALETTE hPal,
                           zBOOL fDeleteHandle )
{
   return( SSSetTypePictureHandle( m_hWnd, CellType, Style,
                                   (zPCHAR) hPict, hPal, fDeleteHandle ) );
}

LPSS_CELLTYPE
ZSS::SetTypeStaticText( LPSS_CELLTYPE CellType, zSHORT TextStyle )
{
   return( SSSetTypeStaticText( m_hWnd, CellType, TextStyle ) );
}

LPSS_CELLTYPE
ZSS::SetTypeTime( LPSS_CELLTYPE CellType, zLONG Style,
                  LPTIMEFORMAT lpFormat, LPTIME lpMin, LPTIME lpMax )
{
   return( SSSetTypeTime( m_hWnd, CellType, Style, lpFormat, lpMin, lpMax ) );
}

WORD
ZSS::SetUnitType( WORD wUnitType )
{
   return( SSSetUnitType( m_hWnd, wUnitType ) );
}

#ifndef SS_NOUSERDATA
zLONG
ZSS::SetUserData( zLONG lUserData )
{
   return( SSSetUserData( m_hWnd, lUserData ) );
}
#endif

zSHORT
ZSS::SetUserResizeCol( SS_COORD ssCol, zSHORT dOption )
{
   return( SSSetUserResizeCol( m_hWnd, ssCol, dOption ) );
}

zSHORT
ZSS::SetUserResizeRow( SS_COORD ssRow, zSHORT dOption )
{
   return( SSSetUserResizeRow( m_hWnd, ssRow, dOption ) );
}

WORD
ZSS::SetUserResize( WORD wUserResize )
{
   return( SSSetUserResize( m_hWnd, wUserResize ) );
}

WORD
ZSS::SetSelBlockOptions( WORD wOption )
{
   return( SSSetSelBlockOptions( m_hWnd, wOption ) );
}

zBOOL
ZSS::SetValue( SS_COORD ssCol, SS_COORD ssRow, zPCHAR lpData )
{
   return( SSSetValue( m_hWnd, ssCol, ssRow, lpData ) );
}

zBOOL
ZSS::SetValueRange( SS_COORD ssCol,  SS_COORD ssRow,
                    SS_COORD ssCol2, SS_COORD ssRow2, zPCHAR lpData )
{
   return( SSSetValueRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, lpData ) );
}

zBOOL
ZSS::ShowActiveCell( zLONG lPosition )
{
   return( SSShowActiveCell( m_hWnd, lPosition ) );
}

zBOOL
ZSS::ShowCell( SS_COORD ssCol, SS_COORD ssRow, zLONG lPosition )
{
   return( SSShowCell( m_hWnd, ssCol, ssRow, lPosition ) );
}

zBOOL
ZSS::ShowCol( SS_COORD ssCol, zBOOL fShow )
{
   return( SSShowCol( m_hWnd, ssCol, fShow ) );
}

zBOOL
ZSS::ShowRow( SS_COORD ssRow, zBOOL fShow )
{
   return( SSShowRow( m_hWnd, ssRow, fShow ) );
}

#ifdef DKS_COPY
zBOOL
ZSS::Sort( SS_COORD ssCol, SS_COORD ssRow,
           SS_COORD ssCol2, SS_COORD ssRow2, LPSS_SORT lpSort )
{
   return( SSSort( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, lpSort ) );
}

zBOOL
ZSS::SwapRange( SS_COORD ssCol, SS_COORD ssRow,
                SS_COORD ssCol2, SS_COORD ssRow2,
                SS_COORD ssColDest, SS_COORD ssRowDest )
{
   return( SSSwapRange( m_hWnd, ssCol, ssRow, ssCol2, ssRow2, ssColDest, ssRowDest ) );
}
#endif

zBOOL
ZSS::ValidateFormula( zPCHAR lpszFormula )
{
// SS_NOCALC is defined
   //x return( SSValidateFormula( m_hWnd, lpszFormula ) );
   return( FALSE );
}

void
ZSS::VGetBufferSize( LPSS_COORD lpdBufferSize,
                     LPSS_COORD lpdOverlap )
{
   SSVGetBufferSize( m_hWnd, lpdBufferSize, lpdOverlap );
}

SS_COORD
ZSS::VGetMax( )
{
   return( SSVGetMax( m_hWnd ) );
}

SS_COORD
ZSS::VGetPhysBufferSize( )
{
   return( SSVGetPhysBufferSize( m_hWnd ) );
}

SS_COORD
ZSS::VGetPhysBufferTop( )
{
   return( SSVGetPhysBufferTop( m_hWnd ) );
}

zLONG
ZSS::VGetStyle( )
{
   return( SSVGetStyle( m_hWnd ) );
}

zBOOL
ZSS::VScrollGetSpecial( LPWORD lpwOptions )
{
   return( SSVScrollGetSpecial( m_hWnd, lpwOptions ) );
}

zBOOL
ZSS::VScrollSetSpecial( zBOOL fUseSpecialVScroll, WORD wOptions )
{
   return( SSVScrollSetSpecial( m_hWnd, fUseSpecialVScroll, wOptions ) );
}

void
ZSS::VSetBufferSize( SS_COORD dBufferSize, SS_COORD dOverlap )
{
   SSVSetBufferSize( m_hWnd, dBufferSize, dOverlap );
}

SS_COORD
ZSS::VSetMax( SS_COORD Max )
{
   return( SSVSetMax( m_hWnd, Max ) );
}

zLONG
ZSS::VSetStyle( zLONG lStyle )
{
   return( SSVSetStyle( m_hWnd, lStyle ) );
}

WORD
ZSS::SetColHeaderDisplay( WORD wDisplay )
{
   return( SSSetColHeaderDisplay( m_hWnd, wDisplay ) );
}

void
ZSS::GetTopLeftPrev( LPSS_COORD lpCol,
                     LPSS_COORD lpRow )
{
   SSGetTopLeftPrev( m_hWnd, lpCol, lpRow );
}

void
ZSS::SetStartingNumbers( SS_COORD StartingColNumber,
                         SS_COORD StartingRowNumber )
{
   SSSetStartingNumbers( m_hWnd, StartingColNumber, StartingRowNumber );
}

zSHORT
ZSS::SetRowColText( zCPCHAR cpcText, zLONG lRow, zSHORT nCol )
{
   if ( nCol <= m_nCols && nCol > 0 )
   {
      nCol--;
      if ( lRow == 0 )
      {
         SetColumnTitle( (zPCHAR) cpcText, nCol );
      }
      else
      {
         SetData( nCol, lRow, (zPCHAR) cpcText );
      }
   }

   return( 0 );
}

zSHORT
ZSS::GetRowColText( zPCHAR pchText, zUSHORT uLth,
                    zLONG lRow, zSHORT nCol )
{
   if ( nCol <= m_nCols && nCol > 0 )
   {
      nCol--;
      if ( lRow == 0 )
      {
         GetColumnTitle( pchText, nCol );
      }
      else
      {
         GetData( nCol, lRow, pchText );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

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

   return( new ZSS( pZSubtask, pWndParent,
                    pzmaComposite, vDialog,
                    nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=SS_LockColumn
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SS_LockColumn
//                                              05/27/93 Modified: 05/27/93
//
//  PURPOSE:    This operation locks/unlocks a spreadsheet column.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nColNbr     - The number of the column to be enabled/disabled
//              bEnable     - TRUE -  lock the column
//                            FALSE - unlock the column
//
//  RETURNS:    0 - Control column locked\unlocked
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
SS_LockColumn( zVIEW    vSubtask,
               zCPCHAR  cpcCtrlTag,
               zSHORT   nColNbr,
               zSHORT   bLock )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SS_LockColumn ", (zLONG) vSubtask );
#endif
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZSS *pSS = DYNAMIC_DOWNCAST( ZSS, pzma->m_pCtrl );
      if ( pSS )
      {
         zSHORT nRC = pSS->LockColumn( nColNbr, (zBOOL) bLock );
         return( nRC );
      }

      TraceLineS( "drvr - Invalid control type for SS_LockColumn ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=SS_GetActiveCell
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SS_GetActiveCell
//                                              05/27/93 Modified: 05/27/93
//
//  PURPOSE:    This operation gets the active spreadsheet row and column.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the control to be disabled.
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
SS_GetActiveCell( zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag,
                  zPLONG   plActiveRow,
                  zPLONG   plActiveCol )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SS_GetActiveCell ", (zLONG) vSubtask );
#endif

   *plActiveRow = *plActiveCol = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZSS *pSS = DYNAMIC_DOWNCAST( ZSS, pzma->m_pCtrl );
      if ( pSS )
      {
         if ( pSS->GetActiveCell( plActiveCol, plActiveRow ) == 0 ||
              *plActiveRow == 0 || *plActiveCol == 0 )
         {
            return( 1 ); // can't retrieve the active cell or not in data cell
         }
         else
         {
            return( 0 );
         }
      }

      TraceLineS( "drvr - Invalid control type for SS_GetActiveCell ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=SS_GetCellData
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SS_GetCellData
//                                              05/27/93 Modified: 05/27/93
//
//  PURPOSE:    This operation gets the data in spreadsheet cell (row/column).
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the control to be disabled.
//              cpcCtrlTag - The unique name of the control.
//              pchReturnData - The data (return) from the specified cell
//              lRowNbr    - row number
//              lColNbr    - column number
//
//  RETURNS:    -1 - Error locating control
//              otw - length of return data
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
SS_GetCellData( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zPCHAR  pchReturnData,
                zLONG   lMaxLth,
                zLONG   lRowNbr,
                zLONG   lColNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SS_GetCellData ", (zLONG) vSubtask );
#endif

   *pchReturnData = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZSS *pSS = DYNAMIC_DOWNCAST( ZSS, pzma->m_pCtrl );
      if ( pSS )
         return( pSS->GetData( lColNbr, lRowNbr, pchReturnData ) );

      TraceLineS( "drvr - Invalid control type for SS_GetCellData ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=SS_SetCellData
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SS_SetCellData
//                                              05/27/93 Modified: 05/27/93
//
//  PURPOSE:    This operation gets the data in spreadsheet cell (row/column).
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                            the control to be disabled.
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
SS_SetCellData( zVIEW    vSubtask,
                zCPCHAR  cpcCtrlTag,
                zPCHAR   pchData,
                zLONG    lRowNbr,
                zLONG    lColNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SS_SetCellData ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZSS *pSS = DYNAMIC_DOWNCAST( ZSS, pzma->m_pCtrl );
      if ( pSS )
         return( !pSS->SetData( lColNbr, lRowNbr, pchData ) );

      TraceLineS( "drvr - Invalid control type for SS_SetCellData ",
                  cpcCtrlTag );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL
SS_EditIsValidKey( LPSS_CELLTYPE CellType,
                   zSHORT        nKey )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_EditIsValidKey" );
   switch ( CellType->chType )
   {
      case SS_TYPE_EDIT:
         switch ( CellType->Spec.Edit.nChrSet )
         {
            case SS_CHRSET_CHR:
               return( TRUE );

            case SS_CHRSET_ALPHA:
               if ( IsCharAlpha( (char) nKey ) || nKey == ' ' )
                  return( TRUE );

               break;

            case SS_CHRSET_ALPHANUM:
               if ( IsCharAlphaNumeric( (char) nKey ) ||
                    nKey == ' ' || nKey == '.' ||
                    nKey == ',' || nKey == '-' )
               {
                  return( TRUE );
               }

               break;

            case SS_CHRSET_NUM:
               if ( isdigit( nKey ) || nKey == '.' || nKey == '-' )
                  return( TRUE );

               break;
         }

         break;
   }

   return( FALSE );
}

void
SS_ProcessEnterKey( HWND  hWndSpread )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_ProcessEnterKey" );
   LPSPREADSHEET lpSS;

   SendMessage( hWndSpread, SSM_SETEDITMODE, FALSE, 0L );

   lpSS = SS_Lock( hWndSpread );
   switch ( lpSS->wEnterAction )
   {
      case SS_ENTERACTION_UP:
         SendMessage( hWndSpread, WM_KEYDOWN, VK_UP, 0L );
         break;

      case SS_ENTERACTION_DOWN:
         SendMessage( hWndSpread, WM_KEYDOWN, VK_DOWN, 0L );
         break;

      case SS_ENTERACTION_LEFT:
         SendMessage( hWndSpread, WM_KEYDOWN, VK_LEFT, 0L );
         break;

      case SS_ENTERACTION_RIGHT:
         SendMessage( hWndSpread, WM_KEYDOWN, VK_RIGHT, 0L );
         break;

      case SS_ENTERACTION_NEXT:
         SendMessage( hWndSpread, SSM_NEXTCELL, 0, 0L );
         break;

      case SS_ENTERACTION_PREV:
         SendMessage( hWndSpread, SSM_PREVCELL, 0, 0L );
         break;

      case SS_ENTERACTION_NEXTROW:
         SendMessage( hWndSpread, SSM_NEXTROW, 0, 0L );
         break;
   }

// DrUnlockTaskMemory( lpSS );
}


// ss_emode.cpp

zBOOL
SS_CellEditModeOff( HWND          hWnd,
                    LPSPREADSHEET lpSS,
                    HWND          hWndFocus )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_CellEditModeOff" );
   zPCHAR         hGlobalData;
   zPCHAR         hText;
   HWND           hWndCtrl = 0;
   LPSS_CELL      lpCell;
   LPSS_CELLTYPE  CellType;
   SS_CELLTYPE    CellTypeTemp;
   zPCHAR         lpItems;
   zPCHAR         lpText;
   zPCHAR         Data;
   zCHAR          Buffer[ 40 ];
   zDECIMAL       dxValue;
   zBOOL          EscapeInProgress;
   zBOOL          fChangeMade = FALSE;
   zBOOL          fErase;
   zBOOL          fNoEraseBkgnd;
   zBOOL          fNoSetFocus = FALSE;
#ifndef SS_USEAWARE
   BYTE           bFractionalWidth;
#endif
   zLONG          lSel;
   zSHORT         nLth;
   zSHORT         k;

   EscapeInProgress = lpSS->EscapeInProgress;
   lpSS->EscapeInProgress = FALSE;

   if ( lpSS->EditModeOn && lpSS->wMessageBeingSent == 0 &&
        lpSS->EditModeTurningOff == 0 )
   {
      lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
      CellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                      lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( CellType->nControlID )
         hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );

      lpSS->EditModeTurningOff = TRUE;
      switch ( CellType->chType )
      {
         case SS_TYPE_EDIT:
         case SS_TYPE_CUSTOM:
            fErase = FALSE;

            if ( EscapeInProgress == 0 &&
                 (fChangeMade = (zBOOL) SendMessage( hWndCtrl, EM_GETMODIFY,
                                                     0, 0L )) != 0 )
            {
               // Send notification to parent.
               SS_DataChangeNotify( hWnd );

               if ( CellType->chType == SS_TYPE_EDIT )
                  nLth = CellType->Spec.Edit.nLth;
               else
                  nLth = GetWindowTextLength( hWndCtrl );

               if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                       nLth + 1, 1017 ) == 0 )
               {
                  Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                  zmemset( Data, 0, nLth + 1 );
                  GetWindowText( hWndCtrl, Data, nLth + 1 );

                  if ( CellType->Spec.Edit.nChrSet == SS_CHRSET_ALPHA ||
                       CellType->Spec.Edit.nChrSet == SS_CHRSET_ALPHANUM )
                  {
                     while ( *Data && *Data == ' ' )
                     {
                        fErase = TRUE;
                        Data++;
                     }
                  }

                  SSSetCellData( hWnd, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                                 Data, zstrlen( Data ) );
                  DrUnlockTaskMemory( Data );
                  DrFreeTaskMemory( hGlobalData );

#ifndef SS_NOOVERFLOW
                  SS_OverflowInvalidateCell( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                             lpSS->Row.ssCurrAt );
#endif
               }
            }
            else
            {
#ifndef SS_NOOVERFLOW
               SS_OverflowAdjustNeighbor( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                          lpSS->Row.ssCurrAt );
#endif
            }

            HideCaret( hWndCtrl );

            if ( !EscapeInProgress )
            {
               SendMessage( hWndCtrl, EM_SETSEL, 0, 0 );

               if ( CellType->lStyle & (ES_AUTOHSCROLL | ES_RIGHT | ES_CENTER) )
                  ;

#ifdef SS_GP
               else
               if ( CellType->Spec.Edit.dxWidth )
                  ;
#endif

               else
               if ( !fErase )
                  lpSS->NoEraseBkgnd = TRUE;
            }

            if ( hWndFocus == 0 )
               SetFocus( hWnd );
            else
            if ( hWndFocus != (HWND) -1 )
               SetFocus( hWndFocus );

#ifndef SS_USEAWARE
            if ( CellType->lStyle & (ES_AUTOHSCROLL | ES_RIGHT | ES_CENTER) )
            {
            }
#ifdef SS_GP
            else
            if ( CellType->Spec.Edit.dxWidth )
            {
            }
#endif
            else
            if ( !EscapeInProgress && !fErase )
               lpSS->NoEraseBkgnd = TRUE;
#endif

         // TraceLineS( "SS_CellEditModeOff: ", "SetWindowPos" );
            if ( lpSS->NoEraseBkgnd )
               SetWindowPos( hWndCtrl, 0, 0, 0, 0, 0,
                             SWP_HIDEWINDOW | SWP_NOREDRAW | SWP_NOZORDER );
            else
               SetWindowPos( hWndCtrl, 0, 0, 0, 0, 0,
                             SWP_HIDEWINDOW | SWP_NOZORDER );

            if ( lpSS->Redraw )
               UpdateWindow( hWnd );

            lpSS->NoEraseBkgnd = FALSE;
            break;

         case SS_TYPE_BUTTON:
         case SS_TYPE_CHECKBOX:
            if ( !EscapeInProgress )
            {
#ifndef SS_NOCT_BUTTON
               if ( CellType->chType == SS_TYPE_BUTTON )
                  fChangeMade = SS_SetButtonData( hWnd, hWndCtrl,
                                                  lpSS, lpCell, CellType );
#endif

#ifndef SS_NOCT_CHECK
               if ( CellType->chType == SS_TYPE_CHECKBOX )
                  fChangeMade = SS_SetCheckBoxData( hWnd, hWndCtrl,
                                                    lpSS, lpCell, CellType );
#endif
            }

            if ( hWndFocus == 0 )
               SetFocus( hWnd );
            else
            if ( hWndFocus != (HWND) -1 )
               SetFocus( hWndFocus );

            InvalidateRect( hWndCtrl, 0, FALSE );
            UpdateWindow( hWndCtrl );

            if ( CellType->chType == SS_TYPE_CHECKBOX )
               lpSS->NoEraseBkgnd = TRUE;

            ShowWindow( hWndCtrl, SW_HIDE );

            if ( lpSS->Redraw )
               UpdateWindow( hWnd );

            lpSS->NoEraseBkgnd = FALSE;

            break;

         case SS_TYPE_PICTURE:
            lpSS->NoEraseBkgnd = TRUE;
            ShowWindow( hWndCtrl, SW_HIDE );

            if ( lpSS->Redraw )
               UpdateWindow( hWnd );

            lpSS->NoEraseBkgnd = FALSE;
            break;

         case SS_TYPE_DATE:
         case SS_TYPE_TIME:
         case SS_TYPE_INTEGER:
         case SS_TYPE_FLOAT:
         case SS_TYPE_PIC:
            fErase = FALSE;
            if ( !EscapeInProgress )
            {
               if ( lpSS->fFormulaMode )
               {
#ifndef SS_NOCALC
                  CellType = &lpSS->FormulaCellType;
                  hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );

                  if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                          CellType->Spec.Edit.nLth + 1,
                                          1018 ) == 0 )
                  {
                     Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                     GetWindowText( hWndCtrl, Data,
                                    CellType->Spec.Edit.nLth + 1 );

                     if ( Data[ 0 ] == '=' )
                     {
                        if ( SSSetFormula( hWnd, lpSS->Col.ssCurrAt -
                                                    lpSS->Col.nHeaderCnt + 1,
                                           lpSS->Row.ssCurrAt -
                                                    lpSS->Row.nHeaderCnt + 1,
                                           Data, TRUE ) )
                        {
                           SS_SendMsgCommand( hWnd, 0,
                                              SSN_USERFORMULA, FALSE );
                        }
                     }
                     else
                     {
                        if ( SSSetFormula( hWnd, lpSS->Col.ssCurrAt -
                                                      lpSS->Col.nHeaderCnt + 1,
                                           lpSS->Row.ssCurrAt -
                                                      lpSS->Row.nHeaderCnt + 1,
                                           "", TRUE ) )
                        {
                           SS_SendMsgCommand( hWnd, 0,
                                              SSN_USERFORMULA, FALSE );
                        }
                     }

                     DrUnlockTaskMemory( Data );
                     DrFreeTaskMemory( hGlobalData );
                  }
#endif
               }

               else
               if ( CellType->chType == SS_TYPE_FLOAT )
               {
                  if ( (fChangeMade = (zBOOL) SendMessage( hWndCtrl,
                                                           EM_GETMODIFY,
                                                           0, 0L ) ) != 0 )
                  {
#ifdef SS_USEAWARE
                     if ( hWndFocus == 0 )
                        SetFocus( hWnd );
                     else
                     if ( hWndFocus != -1 )
                        SetFocus( hWndFocus );

                     fNoSetFocus = TRUE;

                     zBOOL fNull  = (zBOOL) SendMessage( hWndCtrl, EM_GETNULL,
                                                         0, 0L );
                     zBOOL fValid = (zBOOL) SendMessage( hWndCtrl,
                                                         EM_CHECKVALIDITY,
                                                         0, 0L );
                     if ( fNull || fValid == FALSE )
                     {
                        if ( fValid == FALSE )
                           SS_SendMsgCommand( hWnd, 0,
                                              EN_INVALIDDATA, FALSE );
                        *Buffer = (char) 0;
                     }
                     else
                     {
                        FloatGetValue( hWndCtrl, &dxValue );
                        fpDoubleToString( Buffer, dxValue,
                                          CellType->Spec.Float.Right,
                                          '.', 0, FALSE,
                                          (CellType->lStyle & FS_MONEY) ?
                                                               TRUE : FALSE,
                                          TRUE, FALSE, 0, 0 );
                     }
#else
                     FloatGetValue( hWndCtrl, &dxValue );

                     if ( CellType->Spec.Float.nRight == 0 )
                        bFractionalWidth = 1;
                     else
                        bFractionalWidth = (BYTE) CellType->Spec.Float.nRight;

                     MessageBox( 0, "StrPrintf", "spread.cpp", MB_OK );
                     Buffer[ 0 ] = 0;
                 //? StrPrintf( Buffer, "%.*f", bFractionalWidth, dxValue );
                 //? if ( CellType->Spec.Float.Right == 0 )
                 //?    Buffer[ zstrlen( Buffer ) - 2 ] = 0;
#endif

                     SSSetCellData( hWnd, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                                    Buffer, zstrlen( Buffer ) );
#ifndef SS_NOOVERFLOW
                     SS_OverflowInvalidateCell( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                               lpSS->Row.ssCurrAt );
#endif
                  }
                  else
                  {
#ifndef SS_NOOVERFLOW
   //                if ( lpCell == 0 || lpCell->m_Data.bDataType == 0 )
                        SS_OverflowAdjustNeighbor( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                                  lpSS->Row.ssCurrAt );
#endif
                  }
               }
               else
               {
                  if ( (fChangeMade = (zBOOL) SendMessage( hWndCtrl,
                                                           EM_GETMODIFY,
                                                           0, 0L )) != 0 )
                  {
                     if ( CellType->chType == SS_TYPE_PIC )
                        nLth = GetWindowTextLength( hWndCtrl );
                     else
                        nLth = 40;

                     if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData,
                                             nLth + 1, 1019 ) == 0 )
                     {
                        Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                        zmemset( Data, 0, nLth + 1 );
#ifdef SS_USEAWARE
                        // rdw - check for invalid or blank here
                        zBOOL fNull  = (zBOOL) SendMessage( hWndCtrl, EM_GETNULL,
                                                            0, 0L );
                        zBOOL fValid = (zBOOL) SendMessage( hWndCtrl,
                                                            EM_CHECKVALIDITY,
                                                            0, 0L );
                        if ( fNull || fValid == FALSE )
                        {
                           if ( fValid == FALSE )
                              SS_SendMsgCommand( hWnd, 0,
                                                 EN_INVALIDDATA, FALSE );

                           *Data = (char) 0;
                        }
                        else
                           GetWindowText( hWndCtrl, Data, nLth + 1 );
#else
                        GetWindowText( hWndCtrl, Data, nLth + 1 );
#endif

                        SSSetCellData( hWnd, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                                        Data, zstrlen( Data ) );
#ifndef SS_NOOVERFLOW
                        SS_OverflowInvalidateCell( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                                   lpSS->Row.ssCurrAt );
#endif

                        DrUnlockTaskMemory( Data );
                        DrFreeTaskMemory( hGlobalData );
                     }
                  }
                  else
                  {
#ifndef SS_NOOVERFLOW
   //                  if ( lpCell == 0 || lpCell->m_Data.bDataType == 0 )
                        SS_OverflowAdjustNeighbor( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                                   lpSS->Row.ssCurrAt );
#endif
                  }
               }
            }

            else
            if ( lpSS->fFormulaMode )
            {
               CellType = &lpSS->FormulaCellType;
               hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );
            }

            else
            {
#ifndef SS_NOOVERFLOW
               SS_OverflowAdjustNeighbor( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                          lpSS->Row.ssCurrAt );
#endif
            }

            HideCaret( hWndCtrl );

            if ( (CellType->chType == SS_TYPE_DATE && (CellType->lStyle & DS_SPIN)) ||
                 (CellType->chType == SS_TYPE_TIME && (CellType->lStyle & TS_SPIN)) ||
                 (CellType->chType == SS_TYPE_INTEGER && (CellType->lStyle & IS_SPIN)) )
               fErase = TRUE;

            fNoEraseBkgnd = FALSE;
            if ( !EscapeInProgress )
            {
               SendMessage( hWndCtrl, EM_SETSEL, 0, 0 );

               lpSS->NoEraseBkgnd = FALSE;

               if ( ((CellType->chType == SS_TYPE_INTEGER ||
                      CellType->chType == SS_TYPE_FLOAT) &&
                     ((CellType->lStyle & ES_CENTER) ||
                      (CellType->lStyle & ES_LEFTALIGN))) ||
                    ((CellType->chType == SS_TYPE_DATE ||
                      CellType->chType == SS_TYPE_TIME ||
                      CellType->chType == SS_TYPE_PIC) &&
                      (CellType->lStyle & (ES_CENTER | ES_RIGHT))) )
               {
                  ;
               }
               else
               if ( lpSS->fFormulaMode == 0 && fErase == 0 &&
                    ((CellType->chType != SS_TYPE_FLOAT &&
                      CellType->chType != SS_TYPE_INTEGER) || fChangeMade) )
               {
                  fNoEraseBkgnd = TRUE;
#ifndef SS_USEAWARE
                  lpSS->NoEraseBkgnd = TRUE;
#endif
               }
            }

            if ( !fNoSetFocus )
            {
               if ( hWndFocus == 0 )
                  SetFocus( hWnd );
               else
               if ( hWndFocus != (HWND) -1 )
                  SetFocus( hWndFocus );
            }

            ShowWindow( hWndCtrl, SW_HIDE );

#ifndef SS_USEAWARE
            if ( fNoEraseBkgnd )
               lpSS->NoEraseBkgnd = TRUE;
#endif

            if ( lpSS->Redraw || lpSS->fFormulaMode )
               UpdateWindow( hWnd );

#ifndef SS_USEAWARE
            if ( !fErase )
               lpSS->NoEraseBkgnd = TRUE;
#endif

         // TraceLineS( "SS_CellEditModeOff: ", "MoveWindow" );
            MoveWindow( hWndCtrl, 0, 0, 0, 0, FALSE );
            lpSS->NoEraseBkgnd = FALSE;

            break;

#ifndef SS_NOCT_COMBO
         case SS_TYPE_COMBOBOX:
            if ( !EscapeInProgress )
            {
               nLth = (zSHORT) SendMessage( hWndCtrl, WM_GETTEXTLENGTH, 0, 0L );

               if ( DrAllocTaskMemory( (zCOREMEM) &hText, nLth + 1, 1020 ) == 0 )
               {
                  lpText = (zPCHAR) DrLockTaskMemory( hText );
                  SendMessage( hWndCtrl, WM_GETTEXT, nLth + 1, (zLONG) lpText );

//                lpItems = (zPCHAR) DrLockTaskMemory( CellType->Spec.ComboBox.hItems );
                  lpItems = (zPCHAR) CellType->Spec.ComboBox.hItems;
                  fChangeMade = TRUE;

                  if ( (hGlobalData = SSx_GetData( lpSS, 0, 0, lpCell,
                                                   CellType, 0,
                                                   lpSS->Col.ssCurrAt,
                                                   lpSS->Row.ssCurrAt,
                                                   FALSE )) != 0 )
                  {
                  // Data = (zPCHAR) DrLockTaskMemory( hGlobalData );
                     Data = (zPCHAR) hGlobalData;
                     if ( *Data && zstricmp( lpText, Data ) == 0 )
                        fChangeMade = FALSE;
                  }

                  if ( fChangeMade )
                  {
                     lSel = -1;

                     for ( k = 0; k < CellType->Spec.ComboBox.nItemCnt; k++ )
                     {
                        if ( zstricmp( lpText, lpItems ) == 0 )
                        {
                           lSel = k;
                           break;
                        }

                        lpItems += zstrlen( lpItems ) + 1;
                     }

                     wsprintf( Buffer, "%d", lSel );

                     zLONG lTextLth = zstrlen( lpText ) + 1;
                     zLONG lBufferLth = zstrlen( lpText ) + 1;
                     if ( DrAllocTaskMemory( (zCOREMEM) &hGlobalData, lTextLth + lBufferLth, 1022 ) == 0 )
                     {
                        Data = (zPCHAR) DrLockTaskMemory( hGlobalData );

                        strcpy_s( Data, lTextLth, lpText );
                        strcpy_s( Data + lTextLth, lBufferLth, Buffer );

                        SSSetCellData( hWnd, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, Data, lTextLth + lBufferLth );

#ifndef SS_NOOVERFLOW
                        SS_OverflowInvalidateCell( hWnd, lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
#endif

                        DrUnlockTaskMemory( Data );
                        DrFreeTaskMemory( hGlobalData );
                     }
                  }

//                DrUnlockTaskMemory( lpItems );
                  DrUnlockTaskMemory( lpText );
                  DrFreeTaskMemory( hText );
               }
            }

            if ( hWndFocus == 0 )
               SetFocus( hWnd );
            else
            if ( hWndFocus != (HWND) -1 )
               SetFocus( hWndFocus );

            InvalidateRect( hWndCtrl, 0, FALSE );
            UpdateWindow( hWndCtrl );
            ShowWindow( hWndCtrl, SW_HIDE );

            UpdateWindow( hWnd );
            break;
#endif

      }

      lpSS->EditModeOn = FALSE;

      if ( fChangeMade && !EscapeInProgress )
      {
         lpSS->fRowModeChangeMade = TRUE;

         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ),
                           lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                           lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 );
      }

      if ( !EscapeInProgress && lpSS->CalcAuto && lpSS->fFormulaMode == 0 )
         SS_CalcDependencies( hWnd, lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      lpSS->fFormulaMode = FALSE;

      lpSS->EditModeTurningOff = FALSE;

      lpSS->fSetActiveCellCalled = FALSE;
      lpSS->fSetFocusWhileMsgBeingSent = FALSE;

#ifdef SS_USE16BITCOORDS
      SS_SendMsg( hWnd, SSM_EDITMODEOFF, (zSHORT) fChangeMade,
                  MAKELONG( lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                  lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 ) );
#else
   {
      SS_EDITMODEOFF EditModeOff;

      EditModeOff.fChangeMade = fChangeMade;
      EditModeOff.ssCol = lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1;
      EditModeOff.ssRow = lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1;
      SS_SendMsg( hWnd, SSM_EDITMODEOFF, GetDlgCtrlID( hWnd ), (zLONG)(zPCHAR) &EditModeOff );
   }
#endif

      if ( mIs_hWnd( hWnd ) == FALSE )
         return( FALSE );

      if ( lpSS->fProcessingKillFocus && lpSS->fSetFocusWhileMsgBeingSent && hWnd == GetFocus( ) )
      {
         SS_HiliteCell( hWnd, TRUE );
      }

      lpSS->fSetFocusWhileMsgBeingSent = FALSE;

      if ( lpCell )
         SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( lpSS->fSetActiveCellCalled )
      {
         lpSS->fSetActiveCellCalled = FALSE;
         SS_HiliteCell( hWnd, TRUE );
         return( FALSE );
      }

      return( TRUE );
   }

   return( TRUE );
}

zBOOL
SS_CellEditModeOn( HWND          hWnd,
                   LPSPREADSHEET lpSS,
                   WORD          uMsg,
                   WPARAM        wParam,
                   LPARAM        lParam )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_CellEditModeOn" );
   LPSS_CELL     lpCell;
   LPSS_CELLTYPE CellType;
   SS_CELLTYPE   CellTypeTemp;
   zBOOL         fRet = FALSE;

   if ( lpSS->wOpMode != SS_OPMODE_READONLY && lpSS->wOpMode != SS_OPMODE_SINGLESEL &&
        lpSS->wOpMode != SS_OPMODE_MULTISEL && lpSS->wOpMode != SS_OPMODE_EXTSEL &&
        lpSS->EditModeOn == 0 && lpSS->EditModeTurningOn == 0 &&
        lpSS->EditModeTurningOff == 0 && lpSS->wMessageBeingSent == 0 )
   {
      lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( SS_GetLock( hWnd, lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt ) )
      {
         if ( !SS_SendMsgCoords( hWnd, SSM_CELLLOCKED, GetDlgCtrlID( hWnd ),
                                 lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                                 lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 ) )
         {
            SS_Beep( lpSS );
         }

         return( FALSE );
      }

      if ( lpSS->RestrictCols &&
           lpSS->Col.ssCurrAt > lpSS->Col.DataCnt &&
           lpSS->RestrictRows &&
           lpSS->Row.ssCurrAt > lpSS->Row.DataCnt )
      {
         if ( !SS_SendMsgCoords( hWnd, SSM_COLROWRESTRICTED,
                                 GetDlgCtrlID( hWnd ),
                                 lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                                 lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 ) )
            SS_Beep( lpSS );

         return( FALSE );
      }
      else
      if ( lpSS->RestrictCols && lpSS->Col.ssCurrAt > lpSS->Col.DataCnt )
      {
         if ( !SS_SendMsg( hWnd, SSM_COLRESTRICTED, GetDlgCtrlID( hWnd ),
                           (zLONG) (lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1) ) )
         {
            SS_Beep( lpSS );
         }

         return( FALSE );
      }
      else
      if ( lpSS->RestrictRows && lpSS->Row.ssCurrAt > lpSS->Row.DataCnt )
      {
         if ( !SS_SendMsg( hWnd, SSM_ROWRESTRICTED, GetDlgCtrlID( hWnd ),
                           (zLONG) (lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1) ) )
         {
            SS_Beep( lpSS );
         }

         return( FALSE );
      }

      CellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                      lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( CellType->chType == SS_TYPE_STATICTEXT ||
           CellType->chType == SS_TYPE_OWNERDRAW ||
           CellType->chType == SS_TYPE_PICTURE )
         return( FALSE );

      lpSS->fFormulaMode = FALSE;
      lpSS->EditModeTurningOn = TRUE;

#ifdef SS_GP
      if ( lpSS->fEditModePermanent == 0 )
#endif
         SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );

      SS_HiliteCell( hWnd, FALSE );

      SS_SendMsgCoords( hWnd, SSM_EDITMODEON, GetDlgCtrlID( hWnd ),
                        lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                        lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 );

      SS_HiliteCell( hWnd, FALSE );

      if ( mIs_hWnd( hWnd ) == FALSE )
         return( FALSE );

      SS_ClearUndoBuffer( hWnd, lpSS );

      if ( lpSS->wOpMode == SS_OPMODE_ROWMODE && lpSS->fRowModeEditing == 0 )
      {
         lpSS->fRowModeChangeMade = FALSE;
         lpSS->fRowModeEditing = TRUE;
         lpSS->DataRowCntOld = lpSS->Row.DataCnt;
         SS_SendMsgEnterRow( hWnd, SSM_ENTERROW, GetDlgCtrlID( hWnd ),
                             lpSS->Row.ssCurrAt,
                             lpSS->Row.ssCurrAt >= lpSS->Row.DataCnt ? 1 : 0 );
#ifndef SS_NOBUTTONDRAWMODE
         SSx_InvalidateButtons( hWnd, lpSS, lpSS->Col.UL,
                                lpSS->Row.UL, lpSS->Col.UL,
                                lpSS->Row.ssCurrAt, lpSS->Col.LR,
                                lpSS->Row.ssCurrAt );
#endif
      }

      if ( lpCell )
         SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      fRet = SSx_CellEditModeOn( hWnd, lpSS, uMsg, wParam, lParam );
      return( fRet );
   }

   return( FALSE );
}

zBOOL
SSx_CellEditModeOn( HWND           hWnd,
                    LPSPREADSHEET  lpSS,
                    WORD           uMsg,
                    WPARAM         wParam,
                    LPARAM         lParam )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SSx_CellEditModeOn" );
   zPCHAR        hData;
   zPCHAR        Data;
   HWND          hWndCtrl = 0;
   RECT          rect;
   LPSS_CELL     lpCell;
   LPSS_CELLTYPE CellType;
   SS_CELLTYPE   CellTypeTemp;
   SS_FONT       FontTemp;
   LPSS_FONT     Font;
// DATEFORMAT    DateFormat;
// zPCHAR        Mask;
   zPCHAR        lpItems;
   zBOOL         fButtonState;
   zCHAR         Buffer[ 60 ];
   zSHORT        nLth;
   zLONG         x;
   zLONG         y;
   zLONG         cx;
   zLONG         cy;
   zLONG         lSel;
   zSHORT        k;

   lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
   CellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                   lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
   hWndCtrl = SS_RetrieveControlhWnd( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                      lpSS->Row.ssCurrAt );

   if ( hWndCtrl == 0 )
      return( TRUE );

   Font = SS_RetrieveFont( lpSS, &FontTemp, lpCell,
                           lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

   SS_GetClientRect( hWnd, &rect );

   SS_GetCellCoord( lpSS, lpSS->Col.UL, lpSS->Row.UL,
                    lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, &x, &y, &cx, &cy );

   if ( x + cx > rect.right )
      cx = rect.right - x;

   if ( y + cy > rect.bottom )
      cy = rect.bottom - y;

#ifndef SS_NOOVERFLOW
{
   zLONG lWidthPrev;
   zLONG lWidthNext;

   if ( lpSS->fAllowCellOverflow &&
        SSx_ResetCellOverflow( hWnd, lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                               &lWidthPrev, &lWidthNext ) )
   {
      RECT  rectOverlap;
      RECT  rectTemp;

      if ( lWidthPrev || lWidthNext || (lpCell && lpCell->m_CellData.m_bOverflow) )
      {
         SS_GetScrollArea( hWnd, lpSS, &rectTemp, SS_SCROLLAREA_CELLS );

         if ( lWidthPrev )
         {
            SetRect( &rectOverlap, mMax( rectTemp.left, x - lWidthPrev ), y,
                     mMin( rectTemp.right, x ), y + cy );
            InvalidateRect( hWnd, &rectOverlap, TRUE );
         }

         if ( lWidthNext )
         {
            SetRect( &rectOverlap, x + cx - 1, y,
                     mMin( rectTemp.right, x + cx + lWidthNext ),
                     y + cy );
            InvalidateRect( hWnd, &rectOverlap, TRUE );
         }

         if ( lpCell && lpCell->m_CellData.m_bOverflow )
         {
            lpCell->m_CellData.m_bOverflow = 0;
            SetRect( &rectOverlap, x, y + 1, x + cx, y + cy );
            InvalidateRect( hWnd, &rectOverlap, TRUE );
         }

         UpdateWindow( hWnd );
         SS_HiliteCell( hWnd, FALSE );
      }
   }
}
#endif

   if ( lpCell )
      SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

   // If the cell is off the screen, move the edit field off the screen.
   if ( lpSS->fEditModePermanent && (lpSS->Row.LR || lpSS->Col.LR) &&
        ((lpSS->Col.ssCurrAt >= lpSS->Col.nHeaderCnt +
          lpSS->Col.Frozen && lpSS->Col.ssCurrAt < lpSS->Col.UL) ||
          lpSS->Col.ssCurrAt > lpSS->Col.LR ||
         (lpSS->Row.ssCurrAt >= lpSS->Row.nHeaderCnt +
          lpSS->Row.Frozen && lpSS->Row.ssCurrAt < lpSS->Row.UL) ||
          lpSS->Row.ssCurrAt > lpSS->Row.LR) )
   {
      x = -cx - 10;
   }

   lpSS->CurVisCell.ssRow = lpSS->Row.ssCurrAt;
   lpSS->CurVisCell.ssCol = lpSS->Col.ssCurrAt;

   // Set current font for entry field.
   SendMessage( hWndCtrl, WM_SETFONT, (WPARAM) Font->hFont, 1L );

   switch ( CellType->chType )
   {
      case SS_TYPE_EDIT:
         hData = SS_GetData( lpSS, CellType,
                             lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, FALSE );

         if ( (uMsg == WM_LBUTTONDBLCLK && lpSS->fEditModePermanent) == 0 ||
              lpSS->fEditModeReplace )
            SS_TypeEditOn( hWndCtrl, lpSS, CellType, hData, rect, x, y,
                           cx, cy, uMsg == WM_LBUTTONDOWN ? 0 : 2 );
         else
            SS_TypeEditOn( hWndCtrl, lpSS, CellType, hData, rect, x, y,
                           cx, cy, uMsg == WM_LBUTTONDOWN ? 0 : TRUE );

         break;

#ifndef SS_NOCT_BUTTON
      case SS_TYPE_BUTTON:
         fButtonState = 0;

         if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                   lpSS->Row.ssCurrAt, FALSE )) != 0 )
         {
//          Data = (zPCHAR) DrLockTaskMemory( hData );
            Data = (zPCHAR) hData;
            if ( Data[ 0 ] == '1' )
               fButtonState = 1;

//          DrUnlockTaskMemory( Data );
         }

         // Set Text.
         if ( CellType->Spec.Button.hText )
         {
//          Data = (zPCHAR) DrLockTaskMemory( CellType->Spec.Button.hText );
            Data = (zPCHAR) CellType->Spec.Button.hText;
            SetWindowText( hWndCtrl, Data );
//          DrUnlockTaskMemory( Data );
         }
         else
            SetWindowText( hWndCtrl, (zPCHAR)"" );

         // Set Picture.
         if ( CellType->Spec.Button.hPictName )
         {
            if ( CellType->Spec.Button.nPictureType & SUPERBTN_PICT_HANDLE )
               SendMessage( hWndCtrl, SBM_SETPICT,
                            CellType->Spec.Button.nPictureType,
                            (zLONG) CellType->Spec.Button.hPictName );
            else
            {
//             Data = (zPCHAR)
//                       DrLockTaskMemory( CellType->Spec.Button.hPictName );
               Data = (zPCHAR) CellType->Spec.Button.hPictName;

               zPCHAR        hGlobal;
               LPSUPERBTN    lpSuperBtn;

               lpSuperBtn = SuperBtnGetPtr( hWndCtrl, &hGlobal );
               SuperBtnSetPict( hWndCtrl,
                                (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                                &lpSuperBtn->Pict,
                                (char) CellType->Spec.Button.nPictureType,
                                (zLONG) Data );
//             DrUnlockTaskMemory( Data );
            }
         }
         else
            SendMessage( hWndCtrl, SBM_SETPICT, SUPERBTN_PICT_NONE, 0L );

         // Set Down Picture.
         if ( CellType->Spec.Button.hPictDownName )
         {
            if ( CellType->Spec.Button.nPictureDownType & SUPERBTN_PICT_HANDLE )
               SendMessage( hWndCtrl, SBM_SETPICT_BTNDOWN,
                            CellType->Spec.Button.nPictureDownType,
                            (zLONG) CellType->Spec.Button.hPictDownName );
            else
            {
//             Data = (zPCHAR)
//                   DrLockTaskMemory( CellType->Spec.Button.hPictDownName );
               Data = (zPCHAR) CellType->Spec.Button.hPictDownName;
               zPCHAR        hGlobal;
               LPSUPERBTN    lpSuperBtn;

               lpSuperBtn = SuperBtnGetPtr( hWndCtrl, &hGlobal );
               SuperBtnSetPict( hWndCtrl,
                               (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                               &lpSuperBtn->PictBtnDown,
                               (char) CellType->Spec.Button.nPictureDownType,
                               (zLONG) Data );
//             DrUnlockTaskMemory( Data );
            }
         }
         else
            SendMessage( hWndCtrl, SBM_SETPICT_BTNDOWN,
                         SUPERBTN_PICT_NONE, 0L );

         SendMessage( hWndCtrl, SBM_SETCOLOR, 0,
                      (zLONG) &CellType->Spec.Button.Color );

         SendMessage( hWndCtrl, SBM_SETBUTTONTYPE,
                      CellType->Spec.Button.nButtonType, 0L );

         SendMessage( hWndCtrl, SBM_SETSHADOWSIZE,
                      (zSHORT) CellType->Spec.Button.nShadowSize, 0L );

         lpSS->fIgnoreBNClicked = TRUE;
         if ( CellType->Spec.Button.nButtonType == SUPERBTN_TYPE_2STATE )
            SendMessage( hWndCtrl, SBM_SETBUTTONSTATE,
                        (zSHORT) fButtonState, 0L );

         lpSS->fIgnoreBNClicked = FALSE;

      // TraceLineS( "SSx_CellEditModeOn: ", "MoveWindow" );
         MoveWindow( hWndCtrl, rect.left + x + 1, rect.top + y + 1,
                     cx - 1, cy - 1, TRUE );

         lpSS->NoEraseBkgnd = TRUE;
         ShowWindow( hWndCtrl, SW_SHOWNORMAL );
         UpdateWindow( hWndCtrl );
         SetFocus( hWndCtrl );
         lpSS->NoEraseBkgnd = FALSE;
         break;
#endif

#ifndef SS_NOCT_CHECK
      case SS_TYPE_CHECKBOX:
         fButtonState = 0;

         if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                   lpSS->Row.ssCurrAt, FALSE )) != 0 )
         {
//          Data = (zPCHAR) DrLockTaskMemory( hData );
            Data = (zPCHAR) hData;

            if ( (CellType->lStyle & 0x0F) == BS_3STATE ||
                 (CellType->lStyle & 0x0F) == BS_AUTO3STATE )
            {
               if ( Data[ 0 ] == '1' )
                  fButtonState = 1;
               else
               if ( Data[ 0 ] == '2' )
                  fButtonState = 2;
            }
            else
            {
               if ( !Data[ 0 ] || ( !Data[ 1 ] && Data[ 0 ] == '0' ) )
                  fButtonState = 0;
               else
                  fButtonState = 1;
            }

//          DrUnlockTaskMemory( Data );
         }

         // Set Text.
         if ( CellType->Spec.CheckBox.hText )
         {
//          Data = (zPCHAR) DrLockTaskMemory( CellType->Spec.CheckBox.hText );
            Data = (zPCHAR) CellType->Spec.CheckBox.hText;
            SetWindowText( hWndCtrl, Data );
//          DrUnlockTaskMemory( Data );
         }
         else
            SetWindowText( hWndCtrl, (zPCHAR) "" );

         // Set Picture.
         SS_CheckBoxSetPict( hWnd, hWndCtrl, CellType->Spec.CheckBox.hPictUpName,
                             CellType->Spec.CheckBox.bPictUpType | BT_PICT_UP );
         SS_CheckBoxSetPict( hWnd, hWndCtrl, CellType->Spec.CheckBox.hPictDownName,
                             CellType->Spec.CheckBox.bPictDownType | BT_PICT_DOWN );
         SS_CheckBoxSetPict( hWnd, hWndCtrl,
                             CellType->Spec.CheckBox.hPictFocusUpName,
                             CellType->Spec.CheckBox.bPictFocusUpType |
                             BT_PICT_FOCUSUP );
         SS_CheckBoxSetPict( hWnd, hWndCtrl,
                             CellType->Spec.CheckBox.hPictFocusDownName,
                             CellType->Spec.CheckBox.bPictFocusDownType |
                             BT_PICT_FOCUSDOWN );
         SS_CheckBoxSetPict( hWnd, hWndCtrl,
                             CellType->Spec.CheckBox.hPictDisabledUpName,
                             CellType->Spec.CheckBox.bPictDisabledUpType |
                             BT_PICT_DISABLEDUP );
         SS_CheckBoxSetPict( hWnd, hWndCtrl,
                             CellType->Spec.CheckBox.hPictDisabledDownName,
                             CellType->Spec.CheckBox.bPictDisabledDownType |
                             BT_PICT_DISABLEDDOWN );

         SendMessage( hWndCtrl, BM_SETCHECK, (zSHORT) fButtonState, 0L );

      // TraceLineS( "SSx_CellEditModeOn2: ", "MoveWindow" );
         MoveWindow( hWndCtrl, rect.left + x + 1, rect.top + y + 1,
                     cx - 1, cy - 1, TRUE );

         lpSS->NoEraseBkgnd = TRUE;
         ShowWindow( hWndCtrl, SW_SHOWNORMAL );
         UpdateWindow( hWndCtrl );
         SetFocus( hWndCtrl );
         lpSS->NoEraseBkgnd = FALSE;
         break;
#endif

#ifndef SS_NOCT_COMBO
      case SS_TYPE_COMBOBOX:
         SendMessage( hWndCtrl, CB_RESETCONTENT, 0, 0L );
         SetWindowText( hWndCtrl, "" );

         if ( CellType->Spec.ComboBox.hItems )
         {
//          lpItems = (zPCHAR)
//                       DrLockTaskMemory( CellType->Spec.ComboBox.hItems );
            lpItems = (zPCHAR) CellType->Spec.ComboBox.hItems;

            for ( k = 0; k < CellType->Spec.ComboBox.nItemCnt; k++ )
            {
               SendMessage( hWndCtrl, CB_ADDSTRING, 0, (zLONG) lpItems );
               lpItems += zstrlen( lpItems ) + 1;
            }

//          DrUnlockTaskMemory( lpItems );

            if ( !((CellType->lStyle & 0x03 ) == SS_CB_DROPDOWN) &&
                    CellType->Spec.ComboBox.nItemCnt )
            {
               SendMessage( hWndCtrl, CB_SETCURSEL, 0, 0L );

//             lpItems = (zPCHAR)
//                         DrLockTaskMemory( CellType->Spec.ComboBox.hItems );
               lpItems = (zPCHAR) CellType->Spec.ComboBox.hItems;
               SetWindowText( hWndCtrl, lpItems );
//             DrUnlockTaskMemory( lpItems );
            }

            if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                      lpSS->Row.ssCurrAt, FALSE )) != 0 )
            {
//             Data = (zPCHAR) DrLockTaskMemory( hData );
               Data = (zPCHAR) hData;

               if ( zstrlen( Data ) )
               {
                  SetWindowText( hWndCtrl, Data );
                  lSel = zatol( &Data[ zstrlen( Data ) + 1 ] );

                  if ( lSel != -1 )
                     SendMessage( hWndCtrl, CB_SETCURSEL, lSel, 0L );
               }

//             DrUnlockTaskMemory( Data );
            }
         }

      // TraceLineS( "SSx_CellEditModeOn3: ", "MoveWindow" );
         MoveWindow( hWndCtrl, rect.left + x + 1, rect.top + y + 1,
                     cx - 1, cy - 1, TRUE );

         lpSS->NoEraseBkgnd = TRUE;
         ShowWindow( hWndCtrl, SW_SHOWNOACTIVATE );
         UpdateWindow( hWndCtrl );
         SetFocus( hWndCtrl );

         if ( uMsg == WM_LBUTTONDBLCLK &&
              !((CellType->lStyle & 0x03) == SS_CB_DROPDOWN) )
         {
            SendMessage( hWndCtrl, CB_SHOWDROPDOWN, TRUE, 0L );
         }
         else
         if ( uMsg == WM_LBUTTONDOWN && lpSS->fEditModePermanent == 0 )
         {
            SendMessage( hWndCtrl, CB_SHOWDROPDOWN, TRUE, 0L );
            uMsg = WM_LBUTTONDBLCLK;
         }

         lpSS->NoEraseBkgnd = FALSE;
         break;
#endif

      case SS_TYPE_DATE:
      case SS_TYPE_TIME:
      case SS_TYPE_INTEGER:
      case SS_TYPE_FLOAT:
      case SS_TYPE_PIC:
         switch ( CellType->chType )
         {
#ifndef SS_NOCT_DATE
            case SS_TYPE_DATE:
               if ( CellType->Spec.Date.SetRange )
                  DateSetRange( hWndCtrl, &CellType->Spec.Date.Min,
                                &CellType->Spec.Date.Max );

               if ( CellType->Spec.Date.SetFormat )
                  DateSetFormat( hWndCtrl, &CellType->Spec.Date.Format );

               else
               {
                  zmemcpy( &DateFormat, &lpSS->DefaultDateFormat,
                           zsizeof( DATEFORMAT ) );

                  if ( CellType->lStyle & DS_CENTURY )
                     DateFormat.bCentury = TRUE;

                  // Specify format flag.
                  if ( CellType->lStyle & DS_DDMONYY )
                     DateFormat.nFormat = IDF_DDMONYY;
                  else
                  if ( CellType->lStyle & DS_DDMMYY )
                     DateFormat.nFormat = IDF_DDMMYY;
                  else
                  if ( CellType->lStyle & DS_MMDDYY )
                     DateFormat.nFormat = IDF_MMDDYY;
                  else
                  if ( CellType->lStyle & DS_YYMMDD )
                     DateFormat.nFormat = IDF_YYMMDD;

                  DateSetFormat( hWndCtrl, &DateFormat );
               }

               break;
#endif

#ifndef SS_NOCT_TIME
            case SS_TYPE_TIME:
               if ( CellType->Spec.Time.SetRange )
                  TimeSetRange( hWndCtrl, &CellType->Spec.Time.Min,
                                &CellType->Spec.Time.Max );

               if ( CellType->Spec.Time.SetFormat )
                  TimeSetFormat( hWndCtrl, &CellType->Spec.Time.Format );

               else
               {
                  TIMEFORMAT TimeFormat;

                  zmemcpy( &TimeFormat, &lpSS->DefaultTimeFormat,
                           zsizeof( TIMEFORMAT ) );

                  if ( CellType->lStyle & TS_SECONDS )
                     TimeFormat.bSeconds = TRUE;

                  if ( CellType->lStyle & TS_24HOUR )
                     TimeFormat.b24Hour = TRUE;

                  TimeSetFormat( hWndCtrl, &TimeFormat );
               }

               break;
#endif

#ifndef SS_NOCT_PIC
            case SS_TYPE_PIC:
               if ( CellType->Spec.Pic.hMask )
               {
//                Mask = (zPCHAR) DrLockTaskMemory( CellType->Spec.Pic.hMask );
                  Mask = (zPCHAR) CellType->Spec.Pic.hMask;
               }

               SendMessage( hWndCtrl, EPM_SETMASK, 0, (zLONG) Mask );

//             if ( CellType->Spec.Pic.hMask )
//                DrUnlockTaskMemory( Mask );

               break;
#endif

            case SS_TYPE_INTEGER:
            case SS_TYPE_FLOAT:
#ifndef SS_NOCALC
               if ( wParam == '=' && lpSS->AllowUserFormulas )
               {
                  CellType = &lpSS->FormulaCellType;

                  if ( CellType->nControlID )
                     hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );

                  SendMessage( hWndCtrl, WM_SETFONT, (WPARAM) Font->hFont, 1L );

                  hData = SS_GetFormulaHandle( hWnd, lpSS->Col.ssCurrAt,
                                               lpSS->Row.ssCurrAt );

                  SS_TypeEditOn( hWndCtrl, lpSS, CellType, hData,
                                 rect, x, y, cx, cy, FALSE );

                  SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( 0, 0 ) );

                  if ( hData )
                  {
//                   Data = (zPCHAR) DrLockTaskMemory( hData );
                     Data = (zPCHAR) hData;

                     if ( Data[ 0 ] == '=' )
                        wParam = 0;

//                   DrUnlockTaskMemory( Data );
                  }

                  lpSS->fFormulaMode = TRUE;
               }
               else
               if ( CellType->chType == SS_TYPE_FLOAT )
#else
               if ( CellType->chType == SS_TYPE_FLOAT )
#endif
               {
#ifndef SS_USEAWARE
                  SetWindowText( hWndCtrl, "" );
                  FloatSetRange( hWndCtrl, CellType->Spec.Float.dxMin,
                                 CellType->Spec.Float.dxMax );

                  zmemset( Buffer, 0, zsizeof( Buffer ) );

                  for ( k = 0; k < CellType->Spec.Float.nLeft; k++ )
                     strcat_s( Buffer, "9" );

                  strcat_s( Buffer, "." );

                  for ( k = 0; k < CellType->Spec.Float.nRight; k++ )
                     strcat_s( Buffer, "9" );

                  FloatSetMask( hWndCtrl, Buffer );

#else
                  // rdw-12/19
                  SendMessage( hWndCtrl, DM_SETMAX, 0,
                               (zLONG) &CellType->Spec.Float.Max );
                  SendMessage( hWndCtrl, DM_SETMIN, 0,
                               (zLONG) &CellType->Spec.Float.Min );
                  // rdw-12/19.

                  if ( CellType->lStyle & FS_MONEY )
                  {
                     // rdw-12/11
                     zCHAR szCurrencySign[ 2 ] = { 0, 0 };
                     if ( CellType->Spec.Float.fSetFormat &&
                          CellType->Spec.Float.Format.cCurrencySign )
                        *szCurrencySign = CellType->Spec.Float.Format.cCurrencySign;
                     else
                        *szCurrencySign = lpSS->DefaultFloatFormat.cCurrencySign;

                     SendMessage( hWndCtrl, DM_SETCURRENCYSYMB,
                                  0, (zLONG)(zPCHAR) szCurrencySign );
                     // rdw-12/11.

                     SendMessage( hWndCtrl, DM_SETFRACTWIDTH, 2, 0L );
                     SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, DS_FIXEDPOINT );
                     SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, DS_CURRENCY );
                  }
                  else
                  {
                     SendMessage( hWndCtrl, DM_SETFRACTWIDTH,
                                  CellType->Spec.Float.Right, 0L );
                     SendMessage( hWndCtrl, EM_SETEXTSTYLE, FALSE, DS_FIXEDPOINT );
                     SendMessage( hWndCtrl, EM_SETEXTSTYLE, FALSE, DS_CURRENCY );

                  }
#endif

                  if ( CellType->Spec.Float.fSetFormat )
                     FloatSetFormat( hWndCtrl, &CellType->Spec.Float.Format );
               }
               else
               if ( CellType->chType == SS_TYPE_INTEGER )
               {
#ifndef SS_NOCT_INT
                  SetWindowText( hWndCtrl, "" );
                  wsprintf( Buffer, "%ld", CellType->Spec.Integer.Max );

                  for ( k = 0; k < zstrlen( Buffer ); k++ )
                     Buffer[ k ] = '9';

                  IntSetMask( hWndCtrl, Buffer );

                  IntSetRange( hWndCtrl, CellType->Spec.Integer.Min,
                               CellType->Spec.Integer.Max );

                  if ( CellType->lStyle & IS_SPIN )
                     IntSetSpin( hWndCtrl, TRUE, CellType->Spec.Integer.fSpinWrap,
                                 CellType->Spec.Integer.lSpinInc );
#endif
               }

               break;

         }

         if ( lpSS->fFormulaMode == 0 )
         {
            nLth = 0;

            if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                      lpSS->Row.ssCurrAt, FALSE )) != 0 )
            {
//             Data = (zPCHAR) DrLockTaskMemory( hData );
               nLth = zstrlen( hData );

               SetWindowText( hWndCtrl, (zPCHAR) hData );
//             DrUnlockTaskMemory( Data );
            }

            else
               SetWindowText( hWndCtrl, "" );

            SendMessage( hWndCtrl, EM_SETMODIFY, 0, 0L );

         // TraceLineS( "SSx_CellEditModeOn4: ", "MoveWindow" );
            MoveWindow( hWndCtrl, rect.left + x + 1, rect.top + y + 1,
                        cx - 1, cy - 1, TRUE );

            lpSS->NoEraseBkgnd = TRUE;
            ShowWindow( hWndCtrl, SW_SHOWNORMAL );
            UpdateWindow( hWndCtrl );
            SetFocus( hWndCtrl );
            lpSS->NoEraseBkgnd = FALSE;
         }

         break;
   }

   lpSS->EditModeOn = TRUE;

   if ( uMsg == WM_LBUTTONDOWN )
      SendMessage( hWndCtrl, uMsg, wParam, lParam );
   else
   if ( (CellType->chType == SS_TYPE_CHECKBOX ||
         CellType->chType == SS_TYPE_BUTTON) && uMsg == WM_CHAR && wParam )
   {
      SendMessage( hWndCtrl, WM_KEYDOWN, wParam, lParam );
   }
   else
   if ( uMsg == WM_CHAR && wParam )
      SendMessage( hWndCtrl, uMsg, wParam, lParam );

   lpSS->EditModeTurningOn = FALSE;
   return( TRUE );
}

zBOOL
SS_SetEditModeOnData( HWND           hWnd,
                      LPSPREADSHEET  lpSS )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_SetEditModeOnData" );
   zPCHAR         hData;
   zPCHAR         Data;
   HWND           hWndCtrl = 0;
   LPSS_CELL      lpCell;
   LPSS_CELLTYPE CellType;
   SS_CELLTYPE    CellTypeTemp;
   zPCHAR         lpItems;
   zBOOL          fButtonState;
   zLONG          lSel;
   zSHORT         k;

   lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
   CellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                   lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

   if ( CellType->nControlID )
      hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );

   if ( hWndCtrl == 0 )
      return( TRUE );

   switch ( CellType->chType )
   {
      case SS_TYPE_EDIT:
         hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                             lpSS->Row.ssCurrAt, FALSE );

         SSx_TypeEditOn( hWndCtrl, lpSS, CellType, hData, TRUE );
         break;

#ifndef SS_NOCT_BUTTON
      case SS_TYPE_BUTTON:
         fButtonState = 0;

         if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                   lpSS->Row.ssCurrAt, FALSE )) != 0 )
         {
//          Data = (zPCHAR) DrLockTaskMemory( hData );
            Data = (zPCHAR) hData;

            if ( Data[ 0 ] == '1' )
               fButtonState = 1;

//          DrUnlockTaskMemory( Data );
         }

         if ( CellType->Spec.Button.nButtonType == SUPERBTN_TYPE_2STATE )
            SendMessage( hWndCtrl, SBM_SETBUTTONSTATE,
                         (zSHORT) fButtonState, 0L );

         break;
#endif

#ifndef SS_NOCT_CHECK
      case SS_TYPE_CHECKBOX:
         fButtonState = 0;

         if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                   lpSS->Row.ssCurrAt, FALSE )) != 0 )
         {
//          Data = (zPCHAR) DrLockTaskMemory( hData );
            Data = (zPCHAR) hData;

            if ( (CellType->lStyle & 0x0F ) == BS_3STATE ||
                 (CellType->lStyle & 0x0F ) == BS_AUTO3STATE )
            {
               if ( Data[ 0 ] == '1' )
                  fButtonState = 1;
               else
               if ( Data[ 0 ] == '2' )
                  fButtonState = 2;
            }
            else
            {
               if ( !Data[ 0 ] || (!Data[ 1 ] && Data[ 0 ] == '0') )
                  fButtonState = 0;
               else
                  fButtonState = 1;
            }

//          DrUnlockTaskMemory( Data );
         }

         SendMessage( hWndCtrl, BM_SETCHECK, (zSHORT) fButtonState, 0L );
         break;
#endif

#ifndef SS_NOCT_COMBO
      case SS_TYPE_COMBOBOX:
         SendMessage( hWndCtrl, CB_RESETCONTENT, 0, 0L );

         if ( CellType->Spec.ComboBox.hItems )
         {
//          lpItems = (zPCHAR)
//                        DrLockTaskMemory( CellType->Spec.ComboBox.hItems );
            lpItems = (zPCHAR) CellType->Spec.ComboBox.hItems;

            for ( k = 0; k < CellType->Spec.ComboBox.nItemCnt; k++ )
            {
               SendMessage( hWndCtrl, CB_ADDSTRING, 0, (zLONG) lpItems );
               lpItems += zstrlen( lpItems ) + 1;
            }

//          DrUnlockTaskMemory( lpItems );
            SendMessage( hWndCtrl, CB_SETCURSEL, 0, 0L );
            if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                      lpSS->Row.ssCurrAt, FALSE )) != 0 )
            {
//             Data = (zPCHAR) DrLockTaskMemory( hData );
               Data = (zPCHAR) hData;
               SetWindowText( hWndCtrl, Data );

               if ( zstrlen( Data ) )
               {
                  lSel = zatol( &Data[zstrlen( Data ) + 1] );
                  SendMessage( hWndCtrl, CB_SETCURSEL, lSel, 0L );
               }

//             DrUnlockTaskMemory( Data );
            }
         }

         break;
#endif

      case SS_TYPE_DATE:
      case SS_TYPE_TIME:
      case SS_TYPE_INTEGER:
      case SS_TYPE_FLOAT:
      case SS_TYPE_PIC:
         if ( lpSS->fFormulaMode == 0 )
         {
            if ( (hData = SS_GetData( lpSS, CellType, lpSS->Col.ssCurrAt,
                                      lpSS->Row.ssCurrAt, FALSE )) != 0 )
            {
//             Data = (zPCHAR) DrLockTaskMemory( hData );
               Data = (zPCHAR) hData;

#if 0
               if ( CellType->chType == SS_TYPE_FLOAT )
               {
                  SS_FloatToString( lpSS, CellType, Buffer, Data );
                  SetWindowText( hWndCtrl, Buffer );
               }
               else
#endif
                  SetWindowText( hWndCtrl, Data );

//             DrUnlockTaskMemory( Data );
            }
            else
               SetWindowText( hWndCtrl, "" );

            SendMessage( hWndCtrl, EM_SETMODIFY, 0, 0L );
         }

         break;
   }

   if ( lpCell )
      SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

   return( TRUE );
}

zBOOL
SS_SetButtonData( HWND hWnd,
                  HWND hWndCtrl,
                  LPSPREADSHEET lpSS,
                  LPSS_CELL lpCell,
                  LPSS_CELLTYPE lpCellType )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_SetButtonData" );
   zPCHAR         hData;
   zPCHAR         hDataOld;
   zPCHAR         lpData;
   zPCHAR         lpszDataOld;
   zBOOL          fButtonState;
   zBOOL          fChangeMade = FALSE;

   if ( lpCellType &&
        lpCellType->Spec.Button.nButtonType == SUPERBTN_TYPE_2STATE )
   {
      fButtonState = (zBOOL)
                     SendMessage( hWndCtrl, SBM_GETBUTTONSTATE, 0, 0L );

      if ( DrAllocTaskMemory( (zCOREMEM) &hData, 1L + 1L, 1023 ) == 0 )
      {
         lpData = (zPCHAR) DrLockTaskMemory( hData );
         lpData[ 0 ] = (char) '0' + (char) (fButtonState ? 1 : 0);
         fChangeMade = TRUE;

         if ( (hDataOld = SSx_GetData( lpSS, 0, 0,
                                       lpCell, lpCellType, 0,
                                       lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                                       FALSE )) != 0 )
         {
//          lpszDataOld = (zPCHAR) DrLockTaskMemory( hDataOld );
            lpszDataOld = (zPCHAR) hDataOld;

            if ( *lpszDataOld && zstricmp( lpData, lpszDataOld ) == 0 )
               fChangeMade = FALSE;

//          DrUnlockTaskMemory( lpszDataOld );
         }

         SSSetCellData( hWnd, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                        lpData, zstrlen( lpData ) );

         DrUnlockTaskMemory( lpData );
         DrFreeTaskMemory( hData );
      }
   }

   return( fChangeMade );
}

#ifndef SS_NOCT_CHECK

void
SS_CheckBoxSetPict( HWND          hWnd,
                    HWND          hWndCtrl,
                    zPCHAR        hPictName,
                    zSHORT        wPictureType )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_CheckBoxSetPict" );
   LPCHECKBOX    lpCheckBox;
// zPCHAR        lpData;

   if ( hPictName )
   {
      if ( hWndCtrl )
         lpCheckBox = CheckBoxGetPtr( hWndCtrl );
      else
         lpCheckBox = 0;

      if ( lpCheckBox && lpCheckBox->fUseDefPicts )
      {
         zmemset( &lpCheckBox->Picts, 0, zsizeof( CHECKBOXPICTS ) );
         lpCheckBox->fUseDefPicts = FALSE;
      }

      if ( wPictureType & BT_HANDLE )
      {
         CheckBoxSetPict( 0, 0, &lpCheckBox->Picts,
                          wPictureType, (zLONG) hPictName );
      }
      else
      if ( lpCheckBox )
      {
//       lpData = (zPCHAR) DrLockTaskMemory( hPictName );
         CheckBoxSetPict( hWndCtrl,
                          (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                          &lpCheckBox->Picts, wPictureType, (zLONG) hPictName );
//       DrUnlockTaskMemory( lpData );
      }

//    DrUnlockTaskMemory( lpCheckBox );
   }
   else
   if ( hWndCtrl )
      SendMessage( hWndCtrl, BM_SETPICT, wPictureType, 0L );
}

#endif

zBOOL
SS_SetCheckBoxData( HWND hWnd,
                    HWND hWndCtrl,
                    LPSPREADSHEET lpSS,
                    LPSS_CELL lpCell,
                    LPSS_CELLTYPE lpCellType )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_SetCheckBoxData" );
   zPCHAR         hData;
   zPCHAR         hDataOld;
   zPCHAR         lpData;
   zPCHAR         lpszDataOld;
   WORD           wButtonState;
   zBOOL          fChangeMade = FALSE;

   if ( hWndCtrl )
      wButtonState = (WORD) SendMessage( hWndCtrl, BM_GETCHECK, 0, 0L );
   else
      wButtonState = 0;

   if ( DrAllocTaskMemory( (zCOREMEM) &hData, 1L + 1L, 1024 ) == 0 )
   {
      lpData = (zPCHAR) DrLockTaskMemory( hData );
      lpData[ 0 ] = (char) '0' + (char) wButtonState;

      fChangeMade = TRUE;
      if ( (hDataOld = SSx_GetData( lpSS, 0, 0, lpCell, lpCellType, 0,
                                    lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                                    FALSE )) != 0 )
      {
//       lpszDataOld = (zPCHAR) DrLockTaskMemory( hDataOld );
         lpszDataOld = (zPCHAR) hDataOld;

         if ( *lpszDataOld && zstricmp( lpData, lpszDataOld ) == 0 )
            fChangeMade = FALSE;

//       DrUnlockTaskMemory( lpszDataOld );
      }

      SSSetCellData( hWnd, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                     lpData, zstrlen( lpData ) );
      DrUnlockTaskMemory( lpData );
      DrFreeTaskMemory( hData );
   }

   return( fChangeMade );
}

void
SSx_TypeEditOn( HWND           hWndCtrl,
                LPSPREADSHEET  lpSS,
                LPSS_CELLTYPE  CellType,
                zPCHAR         hData,
                zBOOL          fSetSel )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SSx_TypeEditOn" );
   zPCHAR         Data;
   zSHORT         nLth = 0;

   if ( hData && hWndCtrl )
   {
//    Data = (zPCHAR) DrLockTaskMemory( hData );
      Data = (zPCHAR) hData;
      SetWindowText( hWndCtrl, Data );
      nLth = zstrlen( Data );
//    DrUnlockTaskMemory( Data );
   }

#ifndef SS_USEAWARE
   else
   if ( CellType->lStyle & (ES_AUTOHSCROLL | ES_MULTILINE) )
      SetWindowText( hWndCtrl, " " );
#endif
   else
      SetWindowText( hWndCtrl, "" );

   if ( hWndCtrl )
      SendMessage( hWndCtrl, EM_SETMODIFY, 0, 0L );

   lpSS->fEditModeDataChange = FALSE;

#ifndef SS_USEAWARE
   if ( hWndCtrl &&
        (CellType->lStyle & (ES_AUTOHSCROLL | ES_MULTILINE)) && !hData )
   {
      SetWindowText( hWndCtrl, "" );
   }
#endif

   if ( hWndCtrl && fSetSel && nLth )
   {
      if ( lpSS->fEditModePermanent || fSetSel == 2 )
         SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( 0, nLth ) );
      else
         SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( nLth, nLth ) );
   }
}

void
SS_TypeEditOn( HWND           hWndCtrl,
               LPSPREADSHEET  lpSS,
               LPSS_CELLTYPE  CellType,
               zPCHAR         hData,
               RECT           rect,
               zLONG          x,
               zLONG          y,
               zLONG          cx,
               zLONG          cy,
               zBOOL          fSetSel )
{
   TRACE_DEBUG( "<SS_EMODE.CPP>", "SS_TypeEditOn" );
   zPCHAR         Data;
   zSHORT         nLth = 0;

   if ( hWndCtrl == 0 )
      return;

   SetWindowLong( hWndCtrl, GWL_STYLE, WS_CHILD | CellType->lStyle );
   SendMessage( hWndCtrl, EM_LIMITTEXT, CellType->Spec.Edit.nLth, 0L );

   if ( hData )
   {
//    Data = (zPCHAR) DrLockTaskMemory( hData );
      Data = (zPCHAR) hData;
      SendMessage( hWndCtrl, WM_SETTEXT, 0, (zLONG) Data );
      nLth = zstrlen( Data );
//    DrUnlockTaskMemory( Data );
   }

#ifndef SS_USEAWARE
   else
   if ( CellType->lStyle & (ES_AUTOHSCROLL | ES_MULTILINE) )
      SetWindowText( hWndCtrl, " " );
#endif

   else
      SetWindowText( hWndCtrl, "" );

   SendMessage( hWndCtrl, EM_SETMODIFY, 0, 0L );
   lpSS->fEditModeDataChange = FALSE;

#ifdef SS_GP

   SS_SizeExpandingEdit( lpSS, hWndCtrl, rect, CellType,
                         x, y, cx, cy, 0, hData );

#else

// TraceLineS( "SS_TypeEditOn: ", "MoveWindow" );
#ifndef SS_USEAWARE
   if ( CellType->lStyle & ES_AUTOHSCROLL )
      MoveWindow( hWndCtrl, rect.left + x + 3, rect.top + y + 2,
                  cx - 4, cy - 3, TRUE );
   else
   if ( CellType->lStyle & ES_MULTILINE )
      MoveWindow( hWndCtrl, rect.left + x + 3, rect.top + y + 2,
                  cx - 5, cy - 3, TRUE );
   else
#endif
      MoveWindow( hWndCtrl, rect.left + x + 1, rect.top + y + 1,
                  cx - 1, cy - 1, TRUE );

#endif

   lpSS->NoEraseBkgnd = TRUE;
   ShowWindow( hWndCtrl, SW_SHOWNORMAL );
   UpdateWindow( hWndCtrl );

#ifndef SS_USEAWARE
   if ( (CellType->lStyle & (ES_AUTOHSCROLL | ES_MULTILINE)) && hData == 0 )
      SetWindowText( hWndCtrl, "" );
#endif

   SetFocus( hWndCtrl );
   lpSS->NoEraseBkgnd = FALSE;

   if ( fSetSel && nLth )
   {
      if ( fSetSel == 2 )
         SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( 0, nLth ) );
      else
         SendMessage( hWndCtrl, EM_SETSEL, 0, MAKELONG( nLth, nLth ) );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_font.cpp

SS_FONTPOINTS DefaultFontHeight[] =
{
   { 11, 10 },                       // 8  Point
   { 13, 12 },                       // 10 Point
   { 16, 15 },                       // 12 Point
   { 18, 18 },                       // 14 Point
   { 24, 22 },                       // 18 Point
   { 32, 28 }                        // 24 Point
};

zPCHAR
SS_FontAllocItem( zPCHAR        hList,
                  zPLONG        plItemCnt,
                  zSHORT        nItemSize )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_FontAllocItem" );
   return( SSx_AllocList( hList, plItemCnt, nItemSize ) );
}

LPSS_FONT
SS_GetFont( LPSPREADSHEET lpSS,
            LPSS_FONT     Font,
            SS_FONTID     FontId )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_GetFont" );
   LPSS_FONT     lpFontTable;

// lpFontTable = (LPSS_FONT) DrLockTaskMemory( g_hFontTable );
   lpFontTable = (LPSS_FONT) g_hFontTable;
   if ( FontId > 0 && FontId <= g_nFontTableCnt )
   {
      zmemcpy( Font, &lpFontTable[ FontId - 1 ], zsizeof( SS_FONT ) );
//    DrUnlockTaskMemory( lpFontTable );
      return( Font );
   }

// DrUnlockTaskMemory( lpFontTable );
   return( 0 );
}

SS_FONTID
SS_InitFont( HWND  hWnd,
             HFONT hFont,
             zBOOL fDeleteFont,
             zBOOL fCreateFont,
             zBOOL *lpfDeleteFontObject )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_InitFont" );
   HDC        hdc;
   HFONT      hFontOld;
   TEXTMETRIC fm;
   LOGFONT    LogFont;
   LPSS_FONT  lpFontTable;
   SS_FONTID  FontId;
   SS_FONTID  FontIdTemp;
   zLONG      FontCellHeight = 0;
   zLONG      lSize;

   zmemset( &LogFont, 0, sizeof( LOGFONT ) );
   GetObject( hFont, sizeof( LOGFONT ), &LogFont );

   if ( lpfDeleteFontObject )
      *lpfDeleteFontObject = FALSE;

   if ( g_nFontTableCnt >= g_lFontTableAllocCnt )
      g_hFontTable = SS_FontAllocItem( g_hFontTable,
                                       &g_lFontTableAllocCnt,
                                       zsizeof( SS_FONT ) );

   if ( g_hFontTable == 0 )
      return( -1 );

   lpFontTable = (LPSS_FONT) g_hFontTable;
   for ( FontIdTemp = 0; FontIdTemp < g_nFontTableCnt; FontIdTemp++ )
   {
      if ( lpFontTable[ FontIdTemp ].hFont == hFont ||
           zmemcmp( &LogFont, &lpFontTable[ FontIdTemp ].LogFont,
                    sizeof( LOGFONT ) ) == 0 )
      {
         if ( lpFontTable[ FontIdTemp ].hFont != hFont && fDeleteFont == -1 )
            if ( lpfDeleteFontObject )
               *lpfDeleteFontObject = TRUE;

//       DrUnlockTaskMemory( lpFontTable );
         return( FontIdTemp + 1 );
      }
   }

   // Add Font to font table.
   if ( g_nFontTableCnt )
      FontId = g_nFontTableCnt;
   else
      FontId = 0;

   if ( hFont && fCreateFont )
      hFont = (HFONT) CreateFontIndirect( &LogFont );

   lpFontTable[ g_nFontTableCnt ].FontId = FontId;
   lpFontTable[ g_nFontTableCnt ].hFont = hFont;
   lpFontTable[ g_nFontTableCnt ].DeleteFont = fDeleteFont;

   zmemcpy( &lpFontTable[ g_nFontTableCnt ].LogFont,
            &LogFont, sizeof( LOGFONT ) );

   hdc = GetDC( hWnd );
   hFontOld = SelectFont( hdc, hFont );
   GetTextMetrics( hdc, &fm );
   SelectFont( hdc, hFontOld );
   ReleaseDC( hWnd, hdc );

   lpFontTable[ g_nFontTableCnt ].nFontHeight = (zSHORT) fm.tmHeight + 2;

   if ( fm.tmInternalLeading == 0 )
      lSize = fm.tmAscent;
   else
      lSize = fm.tmHeight - fm.tmInternalLeading;

   for ( FontIdTemp = 0;
         FontIdTemp < sizeof( DefaultFontHeight ) / sizeof( SS_FONTPOINTS );
         FontIdTemp++ )
   {
      if ( lSize <= DefaultFontHeight[ FontIdTemp ].nSize )
      {
         FontCellHeight = (DefaultFontHeight[ FontIdTemp ].CellPoints * lSize) /
                          DefaultFontHeight[ FontIdTemp ].nSize;
         break;
      }
   }

   if ( FontCellHeight == 0.0 )
      FontCellHeight = (DefaultFontHeight[ FontIdTemp - 1 ].CellPoints * lSize ) /
                       DefaultFontHeight[ FontIdTemp - 1 ].nSize;

   lpFontTable[ g_nFontTableCnt ].FontCellHeight = FontCellHeight;
   g_nFontTableCnt++;
   FontId = g_nFontTableCnt;

// DrUnlockTaskMemory( lpFontTable );

   return( FontId );
}

LPSS_FONT
SS_RetrieveFont( LPSPREADSHEET lpSS,
                 LPSS_FONT     Font,
                 LPSS_CELL     Cell,
                 SS_COORD      CellCol,
                 SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_RetrieveFont" );
   SS_FONTID     FontId;

   FontId = SS_RetrieveFontId( lpSS, Cell, CellCol, CellRow );
   return( SS_GetFont( lpSS, Font, FontId ) );
}

SS_FONTID
SS_RetrieveFontId( LPSPREADSHEET lpSS,
                   LPSS_CELL     Cell,
                   SS_COORD      CellCol,
                   SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_RetrieveFontId" );
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_FONTID     FontId;

   lpCol = SS_LockColItem( lpSS, CellCol );
   lpRow = SS_LockRowItem( lpSS, CellRow );

   FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow, Cell, CellCol, CellRow );

   SS_UnlockColItem( lpSS, CellCol );
   SS_UnlockRowItem( lpSS, CellRow );

   return( FontId );
}

SS_FONTID
SSx_RetrieveFontId( LPSPREADSHEET lpSS,
                    LPSS_COL      lpCol,
                    LPSS_ROW      lpRow,
                    LPSS_CELL     Cell,
                    SS_COORD      CellCol,
                    SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SSx_RetrieveFontId" );
   SS_FONTID     FontId;

   if ( Cell && Cell->m_FontId != SS_FONT_NONE )
      FontId = Cell->m_FontId;
   else
   if ( lpRow && CellRow < lpSS->Row.nHeaderCnt &&
        lpRow->FontId != SS_FONT_NONE )
      FontId = lpRow->FontId;
   else
   if ( lpCol && lpCol->FontId != SS_FONT_NONE )
      FontId = lpCol->FontId;
   else
   if ( lpRow && lpRow->FontId != SS_FONT_NONE )
      FontId = lpRow->FontId;
   else
      FontId = lpSS->DefaultFontId;

   return( FontId );
}

zBOOL
SS_SetFont( LPSPREADSHEET lpSS,
            LPSS_FONT lpFont,
            SS_FONTID FontId )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_SetFont" );
   LPSS_FONT lpFontTable;
   zBOOL     fRet = FALSE;

// lpFontTable = (LPSS_FONT) DrLockTaskMemory( g_hFontTable );
   lpFontTable = (LPSS_FONT) g_hFontTable;

   if ( FontId > 0 && FontId <= g_nFontTableCnt )
   {
      zmemcpy( &lpFontTable[FontId - 1], lpFont, zsizeof( SS_FONT ) );
      fRet = TRUE;
   }

// DrUnlockTaskMemory( lpFontTable );
   return( fRet );
}

void
SS_SetRowMaxFont( LPSPREADSHEET lpSS,
                  LPSS_ROW lpRow,
                  SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_FONT.CPP>", "SS_SetRowMaxFont" );
   LPSS_FONT lpFontTable;
   LPSS_CELL lpCell;
   zSHORT    nFontHeight = 0;
   SS_FONTID FontId;
   zSHORT    k;
   zBOOL     fRowLocked;

   if ( lpRow == 0 )
   {
      lpRow = SS_LockRowItem( lpSS, ssRow );
      fRowLocked = TRUE;
   }

// lpFontTable = (LPSS_FONT) DrLockTaskMemory( g_hFontTable );
   lpFontTable = (LPSS_FONT) g_hFontTable;

   FontId = lpRow->FontId;

   if ( FontId > 0 )
      nFontHeight = lpFontTable[ FontId - 1 ].nFontHeight;

   for ( k = 0; k < lpSS->Col.AllocCnt; k++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, k, ssRow )) != 0 )
      {
         if ( lpCell->m_FontId > 0 )
         {
            if ( nFontHeight < lpFontTable[ lpCell->m_FontId - 1 ].nFontHeight )
            {
               nFontHeight = lpFontTable[ lpCell->m_FontId - 1 ].nFontHeight;
               FontId = lpCell->m_FontId;
            }
         }
      }
   }

// DrUnlockTaskMemory( lpFontTable );

   if ( FontId > 0 )
      lpRow->RowMaxFontId = FontId;
   else
      lpRow->RowMaxFontId = SS_FONT_NONE;

   if ( lpRow->lRowHeightX100 != SS_WIDTH_DEFAULT ||
        lpRow->RowMaxFontId != SS_FONT_NONE )
      lpRow->nRowHeightInPixels = SS_CalcRowHeight( lpSS, ssRow );
   else
      lpRow->nRowHeightInPixels = SS_WIDTH_DEFAULT;

   if ( fRowLocked )
      SS_UnlockRowItem( lpSS, ssRow );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
SS_LeaveCellNotify( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_LeaveCellNotify" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_SendMsgCommand( hWnd, 0, SSN_LEAVECELL, FALSE );
// DrUnlockTaskMemory( lpSS );
}

/////////////////////////////////////////////////////////////////////////////
void
SS_EnterCellNotify( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_EnterCellNotify" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_SendMsgCommand( hWnd, 0, SSN_ENTERCELL, FALSE );
// DrUnlockTaskMemory( lpSS );
}

/////////////////////////////////////////////////////////////////////////////
void
SS_DragDropNotify( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_DragDropNotify" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   SS_SendMsgCommand( hWnd, 0, SSN_DRAGDROP, FALSE );
// DrUnlockTaskMemory( lpSS );
}

/////////////////////////////////////////////////////////////////////////////
void
SS_DataChangeNotify( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_DataChangeNotify" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_SendMsgCommand( hWnd, 0, SSN_DATACHANGE, FALSE );
// DrUnlockTaskMemory( lpSS );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL
SS_SetCellColor( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 SS_COORD      CellCol,
                 SS_COORD      CellRow,
                 COLORREF      Background,
                 COLORREF      Foreground )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_SetCellColor" );
   LPSS_CELL     lpCell;
   zBOOL         fRet = FALSE;

   if ( (lpCell = SS_AllocLockCell( lpSS, 0, CellCol, CellRow )) != 0 )
   {
      if ( Background != RGBCOLOR_DEFAULT )
         lpCell->m_Color.BackgroundId = SS_AddColor( lpSS, Background );

      if ( Foreground != RGBCOLOR_DEFAULT )
         lpCell->m_Color.ForegroundId = SS_AddColor( lpSS, Foreground );

      SS_UnlockCellItem( lpSS, CellCol, CellRow );
      SS_InvalidateCell( hWnd, lpSS, CellCol, CellRow );

      fRet = TRUE;
   }

   return( fRet );
}

zBOOL
SSSetCellData( HWND       hWnd,
                SS_COORD  CellCol,
                SS_COORD  CellRow,
                zPCHAR    pchData,
                zSHORT    nLth )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSSetCellData" );
   LPSPREADSHEET lpSS;
   SS_COORD      DataColCntOld;
   SS_COORD      DataRowCntOld;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );

   DataColCntOld = lpSS->Col.DataCnt;
   DataRowCntOld = lpSS->Row.DataCnt;

   if ( (fRet = SSx_SetCellData( hWnd, lpSS, CellCol, CellRow,
                                 pchData, nLth )) != 0 )
   {
      if ( CellCol >= DataColCntOld )
         SS_SetHScrollBar( hWnd, lpSS );

      if ( CellRow >= DataRowCntOld )
         SS_SetVScrollBar( hWnd, lpSS );

      if ( lpSS->EditModeTurningOff == 0 || CellCol != lpSS->Col.ssCurrAt ||
           CellRow != lpSS->Row.ssCurrAt || lpSS->fFormulaMode )
      {
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ),
                           CellCol - lpSS->Col.nHeaderCnt + 1,
                           CellRow - lpSS->Row.nHeaderCnt + 1 );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}


zBOOL
SSx_SetCellData( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 SS_COORD      CellCol,
                 SS_COORD      CellRow,
                 zPCHAR        pchData,
                 zSHORT        nLth )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSx_SetCellData" );
   LPSS_CELLTYPE lpCellType = 0;
   SS_CELLTYPE   CellType;
   LPSS_CELL     lpCell;
   zPCHAR        pchDataTemp;
   zSHORT        nLthTemp;
   zBOOL         fRet = FALSE;

   if ( (lpCell = SS_AllocLockCell( lpSS, 0, CellCol, CellRow )) != 0 )
   {
      if ( lpCell->m_CellData.m_DataType || nLth )
      {
         if ( lpCell->m_CellData.m_DataType == SS_TYPE_EDIT )
         {
//          pchDataTemp = (zPCHAR)
//                     DrLockTaskMemory( lpCell->m_CellData.m_Data.hszData );
            pchDataTemp = (zPCHAR) lpCell->m_CellData.m_Data.hszData;

            if ( zstrlen( pchDataTemp ) == (zUSHORT) nLth &&
                 zstrncmp( pchDataTemp, pchData, nLth ) == 0 )
            {
               fRet = TRUE;
            }

//          DrUnlockTaskMemory( pchDataTemp );

            if ( fRet )
               return( TRUE );
         }

         SSx_FreeData( &lpCell->m_CellData );
         SS_ResetCellOverflow( hWnd, lpSS, CellCol, CellRow );

         if ( nLth )
         {
            lpCellType = SS_RetrieveCellType( lpSS, &CellType, lpCell,
                                              CellCol, CellRow );

#ifndef SS_NOOVERFLOW
            lpCell->m_CellData.m_bOverflow = 0;
#endif

            if ( CellType.chType == SS_TYPE_FLOAT )
            {
               zDECIMAL dxTemp;

               lpCell->m_CellData.m_DataType = SS_TYPE_FLOAT;
               dxTemp = ZDecimal( pchData );
            // StringToFloat( pchData, &dxTemp );
               lpCell->m_CellData.m_Data.dxValue = dxTemp;
            }
            else
            {
               SSx_FreeData( &lpCell->m_CellData );
               lpCell->m_CellData.m_DataType = SS_TYPE_EDIT;

               if ( CellType.chType == SS_TYPE_COMBOBOX )
                  nLthTemp = nLth + 1 + 5 + 1;
               else
                  nLthTemp = nLth + 1;

               if ( DrAllocTaskMemory( (zCOREMEM) &lpCell->
                                                   m_CellData.m_Data.hszData,
                                       nLthTemp, 1025 ) != 0 )
               {
                  return( FALSE );
               }

               pchDataTemp = (zPCHAR)
                       DrLockTaskMemory( lpCell->m_CellData.m_Data.hszData );
               lpCell->m_CellData.m_Data.hszData = pchDataTemp;

#ifndef SS_NOCT_COMBO
               if ( CellType.chType == SS_TYPE_COMBOBOX )
               {
                  zPCHAR hText =
                     SS_FormatComboBox( &CellType, pchData, pchDataTemp, FALSE );
                  DrFreeTaskMemory( hText );
               }
               else
#endif
                  zmemcpy( pchDataTemp, pchData, nLth );

//             DrUnlockTaskMemory( pchDataTemp );
            }
         }

         if ( lpCell->m_CellData.m_DataType && CellCol >= lpSS->Col.DataCnt )
            SS_SetDataColCnt( lpSS, CellCol + 1 );

         if ( lpCell->m_CellData.m_DataType && CellRow >= lpSS->Row.DataCnt )
            SS_SetDataRowCnt( lpSS, CellRow + 1 );

         if ( lpCell->m_CellData.m_DataType == 0 )
         {
            SS_AdjustDataColCnt( lpSS, CellCol, CellCol );
            SS_AdjustDataRowCnt( lpSS, CellRow, CellRow );
         }
      }

      SS_CalcCellOverflow( hWnd, lpSS, lpCell, lpCellType, CellCol, CellRow );
      if ( lpCell->m_CellData.m_DataType == 0 )
         SS_OverflowAdjustNeighbor( hWnd, lpSS, CellCol, CellRow );

      SS_UnlockCellItem( lpSS, CellCol, CellRow );
      fRet = TRUE;
   }

   return( fRet );
}

zBOOL OPERATION
SSSetCellFont( HWND          hWnd,
               LPSPREADSHEET lpSS,
               SS_COORD      CellCol,
               SS_COORD      CellRow,
               HFONT         hFont,
               zBOOL         DeleteFont,
               zBOOL         *lpfDeleteFontObject )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSSetCellFont" );
   LPSS_CELL     lpCell;
   zBOOL         fRet = FALSE;

   if ( CellCol < SS_GetColsMax( lpSS ) && CellRow < SS_GetRowsMax( lpSS ) )
   {
      if ( (lpCell = SS_AllocLockCell( lpSS, 0, CellCol, CellRow )) != 0 )
      {
         lpCell->m_FontId = SS_InitFont( hWnd, hFont, DeleteFont, TRUE,
                                         lpfDeleteFontObject );

         SS_SetRowMaxFont( lpSS, 0, CellRow );
         SS_CalcCellOverflow( hWnd, lpSS, lpCell, 0, CellCol, CellRow );

         if ( SS_IsRowVisible( hWnd, lpSS, CellRow, SS_VISIBLE_PARTIAL ) )
            SS_InvalidateRowRange( hWnd, lpSS, CellRow, -1 );

         SS_UnlockCellItem( lpSS, CellCol, CellRow );
         fRet = TRUE;
      }
   }

   return( fRet );
}

zBOOL
SS_SetCellLock( HWND          hWnd,
                LPSPREADSHEET lpSS,
                SS_COORD      CellCol,
                SS_COORD      CellRow,
                zBOOL         Lock )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_SetCellLock" );
   LPSS_CELL     lpCell;
   zBOOL         fRet = FALSE;

   if ( (lpCell = SS_AllocLockCell( lpSS, 0, CellCol, CellRow )) != 0 )
   {
      lpCell->m_CellLocked = Lock;
      SS_UnlockCellItem( lpSS, CellCol, CellRow );
      fRet = TRUE;

      if ( lpSS->LockColor.ForegroundId || lpSS->LockColor.BackgroundId )
         SS_InvalidateCell( hWnd, lpSS, CellCol, CellRow );
   }

   return( fRet );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_copy.cpp
#ifdef DKS_COPY
void
SS_CopyAdjustDataCnt( HWND hWnd, LPSPREADSHEET lpSS, BYTE bDataType,
                      SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopyAdjustDataCnt" );
   if ( bDataType )
   {
      if ( ssCol != SS_ALLCOLS && ssCol >= lpSS->Col.DataCnt )
      {
         SS_SetDataColCnt( lpSS, ssCol + 1 );
         SS_SetHScrollBar( hWnd, lpSS );
      }

      if ( ssRow != SS_ALLROWS && ssRow >= lpSS->Row.DataCnt )
      {
         SS_SetDataRowCnt( lpSS, ssRow + 1 );
         SS_SetVScrollBar( hWnd, lpSS );
      }
   }
   else
   {
      if ( ssCol != SS_ALLCOLS )
         SS_AdjustDataColCnt( lpSS, ssCol, ssCol );

      if ( ssRow != SS_ALLROWS )
         SS_AdjustDataRowCnt( lpSS, ssRow, ssRow );
   }
}

zBOOL
SS_CopyCell( HWND          hWnd,
             LPSPREADSHEET lpSS,
             SS_COORD      ssCol,
             SS_COORD      ssRow,
             LPSS_CELL     lpCellDest,
             LPSS_CELL     lpCellSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopyCell" );
   zmemcpy( lpCellDest, lpCellSrc, zsizeof( ZSS_Cell ) );

   if ( lpCellDest->Data.bDataType == SS_TYPE_EDIT )
      SSx_CopyData( &lpCellDest->Data, &lpCellSrc->Data );

   lpCellDest->hCellType = SSx_CopyCellType( hWnd, lpCellSrc->hCellType );

#ifndef SS_NOCALC
   lpCellDest->hCalc = SSx_CopyCalc( lpSS, lpCellSrc->hCalc, ssCol, ssRow );
   zmemset( &lpCellDest->Dependents, 0, zsizeof( SS_CALCLIST ) );
#endif

   SS_CopyAdjustDataCnt( hWnd, lpSS, lpCellDest->Data.bDataType, ssCol, ssRow );

#ifndef SS_NOOVERFLOW
   SS_OverflowAdjustNeighbor( hWnd, lpSS, ssCol, ssRow );
#endif

   return( TRUE );
}

zPCHAR
SSx_CopyCellType( HWND    hWnd,
                  zPCHAR  hCellTypeSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSx_CopyCellType" );
   zPCHAR         hCellTypeDest = 0;
   LPSS_CELLTYPE  lpCellTypeDest;
   LPSS_CELLTYPE  lpCellTypeSrc;
   zPCHAR         lpPict;

   if ( hCellTypeSrc )
   {
//    lpCellTypeSrc = (LPSS_CELLTYPE) DrLockTaskMemory( hCellTypeSrc );
      lpCellTypeSrc = (LPSS_CELLTYPE) hCellTypeSrc;

      if ( DrAllocTaskMemory( (zCOREMEM) &hCellTypeDest,
                              zsizeof( SS_CELLTYPE ), 1026 ) == 0 )
      {
         lpCellTypeDest = (LPSS_CELLTYPE) DrLockTaskMemory( hCellTypeDest );
         zmemcpy( lpCellTypeDest, lpCellTypeSrc, zsizeof( SS_CELLTYPE ) );

         switch ( lpCellTypeSrc->chType )
         {
            case SS_TYPE_PIC:
               SSx_kzCopyItem( &lpCellTypeDest->Spec.Pic.hMask,
                               &lpCellTypeSrc->Spec.Pic.hMask );

               break;

            case SS_TYPE_BUTTON:
               SSx_CopyItem( &lpCellTypeDest->Spec.Button.hText,
                             &lpCellTypeSrc->Spec.Button.hText );
               if ( !(lpCellTypeDest->Spec.Button.nPictureType &
                      SUPERBTN_PICT_HANDLE) )
               {
                  SSx_CopyItem( &lpCellTypeDest->Spec.Button.hPictName,
                                &lpCellTypeSrc->Spec.Button.hPictName );
               }

               if ( !(lpCellTypeDest->Spec.Button.nPictureDownType &
                      SUPERBTN_PICT_HANDLE) )
               {
                  SSx_CopyItem( &lpCellTypeDest->Spec.Button.hPictDownName,
                                &lpCellTypeSrc->Spec.Button.hPictDownName );
               }

#ifdef SS_VB
               if ( lpCellTypeDest->Spec.Button.hPic )
                  VBRefPic( lpCellTypeDest->Spec.Button.hPic );

               if ( lpCellTypeDest->Spec.Button.hPicDown )
                  VBRefPic( lpCellTypeDest->Spec.Button.hPicDown );
#endif

               break;

            case SS_TYPE_CHECKBOX:
               SSx_CopyItem( &lpCellTypeDest->Spec.CheckBox.hText,
                             &lpCellTypeSrc->Spec.CheckBox.hText );

               if ( !(lpCellTypeDest->Spec.CheckBox.bPictUpType & BT_HANDLE) )
                  SSx_kzCopyItem( &lpCellTypeDest->Spec.CheckBox.hPictUpName,
                                  &lpCellTypeSrc->Spec.CheckBox.hPictUpName );

               if ( !(lpCellTypeDest->Spec.CheckBox.bPictDownType & BT_HANDLE) )
                  SSx_kzCopyItem( &lpCellTypeDest->Spec.CheckBox.hPictDownName,
                                  &lpCellTypeSrc->Spec.CheckBox.hPictDownName );

               if ( !(lpCellTypeDest->Spec.CheckBox.bPictFocusUpType & BT_HANDLE) )
                  SSx_kzCopyItem( &lpCellTypeDest->Spec.CheckBox.hPictFocusUpName,
                                  &lpCellTypeSrc->Spec.CheckBox.hPictFocusUpName );

               if ( !(lpCellTypeDest->Spec.CheckBox.bPictFocusDownType & BT_HANDLE) )
                  SSx_kzCopyItem( &lpCellTypeDest->Spec.CheckBox.hPictFocusDownName,
                                 &lpCellTypeSrc->Spec.CheckBox.hPictFocusDownName );
               if ( !(lpCellTypeDest->Spec.CheckBox.bPictDisabledUpType & BT_HANDLE) )
                  SSx_kzCopyItem( &lpCellTypeDest->Spec.CheckBox.hPictDisabledUpName,
                         &lpCellTypeSrc->Spec.CheckBox.hPictDisabledUpName );

               if ( !(lpCellTypeDest->Spec.CheckBox.bPictDisabledDownType & BT_HANDLE) )
                  SSx_kzCopyItem( &lpCellTypeDest->Spec.CheckBox.hPictDisabledDownName,
                       &lpCellTypeSrc->Spec.CheckBox.hPictDisabledDownName );

#ifdef SS_VB
               if ( lpCellTypeDest->Spec.CheckBox.hPicUp )
                  VBRefPic( lpCellTypeDest->Spec.CheckBox.hPicUp );

               if ( lpCellTypeDest->Spec.CheckBox.hPicDown )
                  VBRefPic( lpCellTypeDest->Spec.CheckBox.hPicDown );

               if ( lpCellTypeDest->Spec.CheckBox.hPicFocusUp )
                  VBRefPic( lpCellTypeDest->Spec.CheckBox.hPicFocusUp );

               if ( lpCellTypeDest->Spec.CheckBox.hPicFocusDown )
                  VBRefPic( lpCellTypeDest->Spec.CheckBox.hPicFocusDown );

               if ( lpCellTypeDest->Spec.CheckBox.hPicDisabledUp )
                  VBRefPic( lpCellTypeDest->Spec.CheckBox.hPicDisabledUp );

               if ( lpCellTypeDest->Spec.CheckBox.hPicDisabledDown )
                  VBRefPic( lpCellTypeDest->Spec.CheckBox.hPicDisabledDown );
#endif

               break;

            case SS_TYPE_PICTURE:
               if ( lpCellTypeDest->lStyle & VPS_HANDLE )
                  SSSetTypePictureHandle( hWnd, lpCellTypeDest,
                                 lpCellTypeDest->lStyle,
                                 lpCellTypeDest->Spec.ViewPict.hPictName,
                                 lpCellTypeDest->Spec.ViewPict.hPal, FALSE );
               else
               {
                  lpPict = 0;
                  if ( lpCellTypeDest->Spec.ViewPict.hPictName )
                  {
//                   lpPict = (zPCHAR) DrLockTaskMemory( lpCellTypeDest->
//                                                 Spec.ViewPict.hPictName );
                     lpPict = (zPCHAR) lpCellTypeDest->
                                                     Spec.ViewPict.hPictName;

                     SSSetTypePicture( hWnd, lpCellTypeDest,
                                       lpCellTypeDest->lStyle, lpPict );
//                   DrUnlockTaskMemory( lpCellTypeDest->
//                                                 Spec.ViewPict.hPictName );
                  }
               }

#ifdef SS_VB
               if ( lpCellTypeDest->Spec.ViewPict.hPic )
                  VBRefPic( lpCellTypeDest->Spec.ViewPict.hPic );
#endif

               break;

            case SS_TYPE_COMBOBOX:
               if ( lpCellTypeDest->Spec.ComboBox.hItems )
                  DrFreeTaskMemory( lpCellTypeDest->Spec.ComboBox.hItems );

               SSx_kzCopyItem( &lpCellTypeDest->Spec.ComboBox.hItems,
                               &lpCellTypeSrc->Spec.ComboBox.hItems );

               break;
         }

//       DrUnlockTaskMemory( lpCellTypeDest );
      }

//    DrUnlockTaskMemory( lpCellTypeSrc );
   }

   return( hCellTypeDest );
}

zBOOL
SS_CopyCol( HWND          hWnd,
            LPSPREADSHEET lpSS,
            SS_COORD      ssCol,
            LPSS_COL      lpColDest,
            LPSS_COL      lpColSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopyCol" );
   zmemcpy( lpColDest, lpColSrc, zsizeof( SS_COL ) );

   if ( lpColDest->Data.bDataType == SS_TYPE_EDIT )
      SSx_CopyData( &lpColDest->Data, &lpColSrc->Data );

   lpColDest->hCellType = SSx_CopyCellType( hWnd, lpColSrc->hCellType );

#ifndef SS_NOCALC
   lpColDest->hCalc = SSx_CopyCalc( lpSS, lpColSrc->hCalc, ssCol, -1 );
   zmemset( &lpColDest->Dependents, 0, zsizeof( SS_CALCLIST ) );
#endif

   SS_CopyAdjustDataCnt( hWnd, lpSS, lpColDest->Data.bDataType,
                         ssCol, SS_ALLROWS );

   return( TRUE );
}

zBOOL
SSx_CopyData( LPSS_DATA lpDataDest,
              LPSS_DATA lpDataSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSx_CopyData" );
   zBOOL     fRet = TRUE;

   if ( lpDataSrc->bDataType == SS_TYPE_EDIT )
   {
      if ( (fRet = SSx_kzCopyItem( &lpDataDest->Data.hszData,
                                   &lpDataSrc->Data.hszData )) == 0 )
      {
         lpDataDest->bDataType = 0;
      }
   }

   return( fRet );
}

zBOOL
SSx_CopyItem( zPCHAR lphItemDest,
              zPCHAR lphItemSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSx_CopyItem" );
   zPCHAR lpText;
   zPCHAR lpTextDest;
   zBOOL  fRet = FALSE;

   if ( *lphItemSrc )
   {
//    lpText = (zPCHAR) DrLockTaskMemory( *lphItemSrc );
      lpText = (zPCHAR) *lphItemSrc;

      zLONG lTextLth = zstrlen( lpText ) + 1;
      if ( (DrAllocTaskMemory( (zCOREMEM) lphItemDest, lTextLth, 1027 )) == 0 )
      {
         lpTextDest = (zPCHAR) DrLockTaskMemory( *lphItemDest );
         strcpy_s( lpTextDest, lTextLth, lpText );

         *lphItemDest = lpTextDest;
//       DrUnlockTaskMemory( lpTextDest );
         fRet = TRUE;
      }

//    DrUnlockTaskMemory( lpText );
   }

   return( fRet );
}

zBOOL
SS_CopyRow( HWND          hWnd,
            LPSPREADSHEET lpSS,
            SS_COORD      ssRow,
            LPSS_ROW      lpRowDest,
            LPSS_ROW      lpRowSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopyRow" );
   zmemcpy( lpRowDest, lpRowSrc, zsizeof( SS_ROW ) );

   if ( lpRowDest->Data.bDataType == SS_TYPE_EDIT )
      SSx_CopyData( &lpRowDest->Data, &lpRowSrc->Data );

   lpRowDest->hCellType = SSx_CopyCellType( hWnd, lpRowSrc->hCellType );

#ifndef SS_NOCALC
   lpRowDest->hCalc = SSx_CopyCalc( lpSS, lpRowSrc->hCalc, -1, ssRow );
   zmemset( &lpRowDest->Dependents, 0, zsizeof( SS_CALCLIST ) );
#endif

   lpRowDest->Cells.dwItemCnt = 0;
   lpRowDest->Cells.hItems = 0;
   SS_CopyAdjustDataCnt( hWnd, lpSS, lpRowDest->Data.bDataType,
                         SS_ALLCOLS, ssRow );

   return( TRUE );
}

zBOOL OPERATION
SSMoveRange( HWND hWnd, SS_COORD ssCol, SS_COORD ssRow,
             SS_COORD ssCol2, SS_COORD ssRow2, SS_COORD ssColDest,
             SS_COORD ssRowDest )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSMoveRange" );
   return( SS_CopySwapRange( hWnd, ssCol, ssRow, ssCol2, ssRow2,
                             ssColDest, ssRowDest, SS_CMD_MOVE, FALSE ) );
}

zBOOL
SS_CopySwapCell( HWND hWnd, LPSPREADSHEET lpSS, SS_COORD x, SS_COORD y,
                 SS_COORD xDest, SS_COORD yDest, WORD  wCmd,
                 zBOOL fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopySwapCell" );
   zPCHAR         hItem = 0;
   zPCHAR         hItemDest = 0;
   LPSS_ROW       lpRow;
   LPSS_ROW       lpRowDest;
   LPSS_CELL      lpCell;
   LPSS_CELL      lpCellDest;
   BYTE           bDataType;
   zBOOL          fRet = TRUE;


   // copy a cell
   if ( wCmd == SS_CMD_COPY )
   {
      SS_DeAllocCell( hWnd, xDest, yDest, FALSE );

      if ( (lpCell = SS_LockCellItem( lpSS, x, y )) != 0 )
      {
         if ( (lpCellDest = SS_AllocLockCell( lpSS, 0, xDest, yDest )) != 0 )
         {
            SS_CopyCell( hWnd, lpSS, xDest, yDest, lpCellDest, lpCell );
            SS_UnlockCellItem( lpSS, xDest, yDest );
         }

         SS_UnlockCellItem( lpSS, x, y );
      }

      if ( fSendChangeMsg )
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ),
                           xDest, yDest );

      SS_InvalidateCell( hWnd, lpSS, xDest, yDest );
   }
   else     // swap a cell
   if ( wCmd == SS_CMD_SWAP )
   {
      if ( (lpRowDest = SS_LockRowItem( lpSS, yDest )) != 0 )
      {
         hItemDest = SS_GrabItemHandle( lpSS, &lpRowDest->Cells, xDest,
                                        SS_GetColsMax( lpSS ) );
         SS_UnlockRowItem( lpSS, yDest );
      }

      if ( (lpRow = SS_LockRowItem( lpSS, y )) != 0 )
      {
         hItem = SS_GrabItemHandle( lpSS, &lpRow->Cells, x,
                                    SS_GetColsMax( lpSS ) );
         SS_UnlockRowItem( lpSS, y );
      }

      if ( (lpRowDest = SS_AllocLockRow( lpSS, yDest )) != 0 )
      {
         if ( hItem )
         {
            if ( (hItem = SS_AllocItems( lpSS,
                                         &lpRowDest->Cells, xDest, 0, hItem,
                                         SS_GetColsMax( lpSS ),
                                         0 )) == 0 )
            {
               fRet = FALSE;
            }
            else
            {
#ifndef SS_NOOVERFLOW
               SS_OverflowAdjustNeighbor( hWnd, lpSS, xDest, yDest );
#endif
            }
         }

         SS_UnlockRowItem( lpSS, yDest );
      }

      if ( (lpRow = SS_AllocLockRow( lpSS, y )) != 0 )
      {
         if ( hItemDest )
         {
            if ( (hItem = SS_AllocItems( lpSS,
                                         &lpRow->Cells, x, 0, hItemDest,
                                         SS_GetColsMax( lpSS ), 0 )) == 0 )
            {
               fRet = FALSE;
            }
            else
            {
#ifndef SS_NOOVERFLOW
               SS_OverflowAdjustNeighbor( hWnd, lpSS, x, y );
#endif
            }
         }

         SS_UnlockRowItem( lpSS, y );
      }

      SS_InvalidateCell( hWnd, lpSS, x, y );
      SS_InvalidateCell( hWnd, lpSS, xDest, yDest );

      bDataType = 0;

      if ( (lpCellDest = SS_LockCellItem( lpSS, xDest, yDest )) != 0 )
      {
         bDataType = lpCellDest->Data.bDataType;
         SS_UnlockCellItem( lpSS, xDest, yDest );
      }

      SS_CopyAdjustDataCnt( hWnd, lpSS, bDataType, xDest, yDest );

      if ( fSendChangeMsg )
      {
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ), x, y );
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ),
                           xDest, yDest );
      }
   }
   else     // move a cell
   {
      SS_DeAllocCell( hWnd, xDest, yDest, FALSE );

      if ( (lpRow = SS_LockRowItem( lpSS, y )) != 0 )
      {
         hItem = SS_GrabItemHandle( lpSS, &lpRow->Cells, x,
                                    SS_GetColsMax( lpSS ) );

         if ( (lpRowDest = SS_AllocLockRow( lpSS, yDest )) != 0 )
         {
            if ( hItem )
               if ( (hItem = SS_AllocItems( lpSS, &lpRowDest->Cells,
                                            xDest, 0, hItem,
                                            SS_GetColsMax( lpSS ),
                                            0 )) == 0 )
               {
                  fRet = FALSE;
               }
            else
            {
#ifndef SS_NOOVERFLOW
               SS_OverflowAdjustNeighbor( hWnd, lpSS, xDest, yDest );
#endif
            }

            SS_UnlockRowItem( lpSS, yDest );
         }

         SS_UnlockRowItem( lpSS, y );
      }

      SS_InvalidateCell( hWnd, lpSS, x, y );
      SS_InvalidateCell( hWnd, lpSS, xDest, yDest );

      bDataType = 0;

      if ( (lpCellDest = SS_LockCellItem( lpSS, xDest, yDest )) != 0 )
      {
         bDataType = lpCellDest->Data.bDataType;
         SS_UnlockCellItem( lpSS, xDest, yDest );
      }

      SS_CopyAdjustDataCnt( hWnd, lpSS, bDataType, xDest, yDest );

      if ( fSendChangeMsg )
      {
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ), x, y );
         SS_SendMsgCoords( hWnd, SSM_DATACHANGE, GetDlgCtrlID( hWnd ),
                           xDest, yDest );
      }
   }

   return( fRet );
}

zBOOL
SS_CopySwapCol( HWND hWnd, LPSPREADSHEET lpSS, SS_COORD x,
                SS_COORD xDest, WORD  wCmd )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopySwapCol" );
   zPCHAR         hItem;
   LPSS_COL       lpCol;
   LPSS_COL       lpColDest;
   LPSS_ROW       lpRow;
   LPSS_CELL      lpCell;
   LPSS_CELL      lpCellDest;
   zBOOL          fRet = TRUE;
   SS_COORD       ss;

   // Copy a col.
   if ( wCmd == SS_CMD_COPY )
   {
      SS_DeAllocCol( hWnd, xDest, FALSE );

      if ( (lpCol = SS_LockColItem( lpSS, x )) != 0 )
      {
         if ( (lpColDest = SS_AllocLockCol( lpSS, xDest )) != 0 )
         {
            SS_CopyCol( hWnd, lpSS, xDest, lpColDest, lpCol );
            SS_UnlockColItem( lpSS, xDest );
         }

         SS_UnlockColItem( lpSS, x );
      }

      for ( ss = 0; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpCell = SSx_LockCellItem( lpSS, 0, x, ss )) != 0 )
         {
            if ( (lpCellDest = SS_AllocLockCell( lpSS, 0, xDest, ss )) != 0 )
            {
               SS_CopyCell( hWnd, lpSS, xDest, ss, lpCellDest, lpCell );
               SS_UnlockCellItem( lpSS, xDest, ss );
            }

            SS_UnlockCellItem( lpSS, x, ss );
         }
      }

      SS_InvalidateColRange( hWnd, lpSS, xDest, -1 );
   }
   else     // swap a col
   if ( wCmd == SS_CMD_SWAP )
   {
      hItem = SS_GrabItemHandle( lpSS, &lpSS->Col.Items, xDest,
                                 SS_GetColsMax( lpSS ) );

      fRet = SS_MoveAllocItems( lpSS, &lpSS->Col.Items, x, x, xDest,
                                SS_GetColsMax( lpSS ) );

      if ( hItem )
      {
         if ( (hItem = SS_AllocItems( lpSS, &lpSS->Col.Items, x, 0, hItem,
                                      SS_GetColsMax( lpSS ), 0 )) == 0 )
         {
            fRet = FALSE;
         }
      }

      for ( ss = 0; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
         {
            hItem = SS_GrabItemHandle( lpSS, &lpRow->Cells, xDest,
                                       SS_GetColsMax( lpSS ) );

            fRet = SS_MoveAllocItems( lpSS, &lpRow->Cells, x, x, xDest,
                                      SS_GetColsMax( lpSS ) );

            if ( hItem )
            {
               if ( (hItem = SS_AllocItems( lpSS, &lpRow->Cells, x, 0, hItem,
                                            SS_GetColsMax( lpSS ),
                                            0 ) ) == 0 )
               {
                  fRet = FALSE;
               }
            }

            SS_UnlockRowItem( lpSS, ss );
         }
      }

      if ( mMax( x, xDest ) >= lpSS->Col.DataCnt - 1 )
      {
         lpSS->Col.DataCnt = mMax( x, xDest ) + 1;
         SS_AdjustDataColCnt( lpSS, mMin( x, xDest ), mMax( x, xDest ) );
      }

      SS_InvalidateColRange( hWnd, lpSS, x, -1 );
      SS_InvalidateColRange( hWnd, lpSS, xDest, -1 );
   }
   else  // move a col
   {
      SS_DeAllocCol( hWnd, xDest, FALSE );

      fRet = SS_MoveAllocItems( lpSS, &lpSS->Col.Items, x, x, xDest,
                                SS_GetColsMax( lpSS ) );

      for ( ss = 0; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
         {
            SS_DeAllocCell( hWnd, xDest, ss, FALSE );

            fRet = SS_MoveAllocItems( lpSS, &lpRow->Cells, x, x, xDest,
                                      SS_GetColsMax( lpSS ) );

            SS_UnlockRowItem( lpSS, ss );
         }
      }

      if ( xDest >= lpSS->Col.AllocCnt )
         lpSS->Col.AllocCnt = xDest + 1;

      if ( mMax( x, xDest ) >= lpSS->Col.DataCnt - 1 )
      {
         lpSS->Col.DataCnt = mMax( x, xDest ) + 1;
         SS_AdjustDataColCnt( lpSS, mMin( x, xDest ), mMax( x, xDest ) );
      }

      SS_InvalidateColRange( hWnd, lpSS, x, -1 );
      SS_InvalidateColRange( hWnd, lpSS, xDest, -1 );
   }

   return( fRet );
}

zBOOL
SS_CopySwapRange( HWND hWnd, SS_COORD ssCol, SS_COORD ssRow, SS_COORD ssCol2,
                  SS_COORD ssRow2, SS_COORD ssColDest, SS_COORD ssRowDest,
                  WORD  wCmd, zBOOL fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopySwapRange" );
   LPSPREADSHEET lpSS;
   SS_COORD      x;
   SS_COORD      xInc;
   SS_COORD      xDest;
   SS_COORD      xDestEnd;
   SS_COORD      y;
   SS_COORD      yInc;
   SS_COORD      yDest;
   SS_COORD      yDestEnd;
   zBOOL         RedrawOld;
   zBOOL         fRet = TRUE;

   if ( ssCol == SS_ALLCOLS || ssCol2 == SS_ALLCOLS )
   {
      ssCol = SS_ALLCOLS;
      ssCol2 = SS_ALLCOLS;
   }

   if ( ssRow == SS_ALLROWS || ssRow2 == SS_ALLROWS )
   {
      ssRow = SS_ALLROWS;
      ssRow2 = SS_ALLROWS;
   }

   if ( ssCol != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col, 0 );

   if ( ssRow != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row );

   if ( ssCol2 != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col2, 0 );

   if ( ssRow2 != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row2 );

   if ( (Row != SS_ALLROWS && ssRowDest == SS_ALLROWS) ||
        (Col != SS_ALLCOLS && ssColDest == SS_ALLCOLS) )
   {
      return( FALSE );
   }

   lpSS = SS_Lock( hWnd );

   if ( ssColDest != SS_ALLCOLS )
   {
      if ( ssColDest >= SS_GetColsMax( lpSS ) )
         return( FALSE );

      SS_AdjustCellCoords( hWnd, &Col2, 0 );
   }

   if ( ssRowDest != SS_ALLROWS )
   {
      if ( ssRowDest >= SS_GetRowsMax( lpSS ) )
         return( FALSE );

      SS_AdjustCellCoords( hWnd, 0, &Row2 );
   }

   // Calculate coordinates.
   if ( ssRow != SS_ALLROWS )
   {
      if ( ssRowDest >= ssRow && ssRowDest <= ssRow2 )
      {
         y = ssRow2;
         yInc = -1;
         yDest = mMin( ssRowDest + (Row2 - ssRow), SS_GetRowsMax( lpSS ) - 1 );
         yDestEnd = ssRowDest - 1;
      }
      else
      {
         y = ssRow;
         yInc = 1;
         yDest = ssRowDest;
         yDestEnd = mMin( ssRowDest + (Row2 - ssRow),
                          SS_GetRowsMax( lpSS ) - 1 ) + 1;
      }
   }
   else
   {
      y = -1;
      yInc = 1;
      yDest = -1;
      yDestEnd = 0;
   }

   lpSS->Col.LastRefCoord.Coord.ssCol = -1;
   lpSS->Row.LastRefCoord.Coord.ssRow = -1;
   lpSS->LastCell.Coord.ssCol = -1;
   lpSS->LastCell.Coord.ssRow = -1;

   RedrawOld = lpSS->Redraw;
   lpSS->Redraw = FALSE;

   for ( ; yDest != yDestEnd && fRet; y += yInc, yDest += yInc )
   {
      if ( ssCol != SS_ALLCOLS )
      {
         if ( ssColDest >= ssCol && ssColDest <= ssCol2 )
         {
            x = ssCol2;
            xInc = -1;
            xDest = mMin( ssColDest + (Col2 - ssCol), SS_GetColsMax( lpSS ) - 1 );
            xDestEnd = ssColDest - 1;
         }
         else
         {
            x = ssCol;
            xInc = 1;
            xDest = ssColDest;
            xDestEnd = mMin( ssColDest + (Col2 - ssCol),
                             SS_GetColsMax( lpSS ) - 1 ) + 1;
         }
      }
      else
      {
         x = -1;
         xInc = 1;
         xDest = -1;
         xDestEnd = 0;
      }

      for ( ; xDest != xDestEnd && fRet; x += xInc, xDest += xInc )
      {
         if ( x != SS_ALLCOLS && y != SS_ALLROWS )          // Cell
            fRet = SS_CopySwapCell( hWnd, lpSS, x, y,
                                    xDest, yDest, wCmd, fSendChangeMsg );
         else
         if ( x != SS_ALLCOLS && y == SS_ALLROWS )     // ssCol
            fRet = SS_CopySwapCol( hWnd, lpSS, x, xDest, wCmd );
         else
         if ( x == SS_ALLCOLS && y != SS_ALLROWS )     // ssRow
            fRet = SS_CopySwapRow( hWnd, lpSS, y, yDest, wCmd );
      }
   }

   SSBussldDependencsses( hWnd );
   SSSetBool( hWnd, SSB_REDRAW, RedrawOld );

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL
SS_CopySwapRow( HWND hWnd, LPSPREADSHEET lpSS,
                SS_COORD y, SS_COORD yDest, WORD  wCmd )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SS_CopySwapRow" );
   zPCHAR         hItem;
   LPSS_ROW       lpRow;
   LPSS_ROW       lpRowDest;
   LPSS_CELL      lpCell;
   LPSS_CELL      lpCellDest;
   zBOOL          fRet = TRUE;
   SS_COORD       ss;

   // Copy a row.
   if ( wCmd == SS_CMD_COPY )
   {
      SS_DeAllocRow( hWnd, yDest, FALSE );

      if ( (lpRow = SS_LockRowItem( lpSS, y )) != 0 )
      {
         if ( (lpRowDest = SS_AllocLockRow( lpSS, yDest )) != 0 )
         {
            SS_CopyRow( hWnd, lpSS, yDest, lpRowDest, lpRow );
            SS_UnlockRowItem( lpSS, yDest );
         }

         // Copy each cell within the row.
         for ( ss = 0; ss < lpSS->Col.AllocCnt; ss++ )
         {
            if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, ss, y )) != 0 )
            {
               if ( (lpCellDest = SS_AllocLockCell( lpSS, lpRow, ss, yDest )) != 0 )
               {
                  SS_CopyCell( hWnd, lpSS, ss, yDest, lpCellDest, lpCell );
                  SS_UnlockCellItem( lpSS, ss, yDest );
               }

               SS_UnlockCellItem( lpSS, ss, y );
            }
         }

         SS_UnlockRowItem( lpSS, y );
      }

      SS_InvalidateRowRange( hWnd, lpSS, yDest, -1 );
   }
   else   // swap a row
   if ( wCmd == SS_CMD_SWAP )
   {
      hItem = SS_GrabItemHandle( lpSS, &lpSS->Row.Items, yDest,
                                 SS_GetRowsMax( lpSS ) );

      fRet = SS_MoveAllocItems( lpSS, &lpSS->Row.Items, y, y, yDest,
                                SS_GetRowsMax( lpSS ) );

      if ( hItem )
      {
         if ( (hItem = SS_AllocItems( lpSS, &lpSS->Row.Items, y, 0, hItem,
                                      SS_GetRowsMax( lpSS ), 0 ) ) == 0 )
         {
            fRet = FALSE;
         }
      }

      if ( mMax( y, yDest ) >= lpSS->Row.DataCnt - 1 )
      {
         lpSS->Row.DataCnt = mMax( y, yDest ) + 1;
         SS_AdjustDataRowCnt( lpSS, mMin( y, yDest ), mMax( y, yDest ) );
      }

      SS_InvalidateRowRange( hWnd, lpSS, y, -1 );
      SS_InvalidateRowRange( hWnd, lpSS, yDest, -1 );
   }
   else   // move a row

   {
      SS_DeAllocRow( hWnd, yDest, FALSE );

      fRet = SS_MoveAllocItems( lpSS, &lpSS->Row.Items, y, y, yDest,
                                SS_GetRowsMax( lpSS ) );

      if ( yDest >= lpSS->Row.AllocCnt )
         lpSS->Row.AllocCnt = yDest + 1;

      if ( mMax( y, yDest ) >= lpSS->Row.DataCnt - 1 )
      {
         lpSS->Row.DataCnt = mMax( y, yDest ) + 1;
         SS_AdjustDataRowCnt( lpSS, mMin( y, yDest ), mMax( y, yDest ) );
      }

      SS_InvalidateRowRange( hWnd, lpSS, y, -1 );
      SS_InvalidateRowRange( hWnd, lpSS, yDest, -1 );
   }

   return( fRet );
}

zBOOL
SSx_kzCopyItem( zPCHAR  pchItemDest,
                zPCHAR  pchItemSrc )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSx_kzCopyItem" );
   zPCHAR  lpText;
   zPCHAR  lpTextDest;
   zBOOL   fRet = FALSE;

   if ( *pchItemSrc )
   {
//    lpText = (zPCHAR) DrLockTaskMemory( *pchItemSrc );
      lpText = (zPCHAR) *pchItemSrc;

      zLONG lTextLth = zstrlen( lpText ) + 1;
      if ( DrAllocTaskMemory( pchItemDest, lTextLth, 1028 ) == 0 )
      {
         lpTextDest = (zPCHAR) DrLockTaskMemory( *pchItemDest );
         strcpy_s( lpTextDest, lTextLth, lpText );

         *pchItemDest = lpTextDest;
//       DrUnlockTaskMemory( lpTextDest );
         fRet = TRUE;
      }

//    DrUnlockTaskMemory( lpText );
   }

   return( fRet );
}

#endif   // #ifdef DKS_COPY

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL
SS_ClearCell( HWND          hWnd,
              SS_COORD      CellCol,
              SS_COORD      CellRow,
              zBOOL         Paint,
              zBOOL         fSendChangeMsg )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_ClearCell" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( CellCol >= SS_GetColsMax( lpSS ) ||
        CellRow >= SS_GetRowsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

// DrUnlockTaskMemory( lpSS );

   SS_DeAllocCell( hWnd, CellCol, CellRow, fSendChangeMsg );

   if ( Paint )
      SS_InvalidateCell( hWnd, lpSS, CellCol, CellRow );

   return( TRUE );
}

zBOOL
SS_ClearCol( HWND hWnd, SS_COORD ssCol,
             zBOOL Paint, zBOOL fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SS_ClearCol" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( ssCol >= lpSS->Col.AllocCnt || ssCol >= SS_GetColsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_DeAllocCol( hWnd, ssCol, fSendChangeMsg );

   if ( Paint && SS_IsColVisible( hWnd, lpSS, ssCol, SS_VISIBLE_PARTIAL ) )
      SS_InvalidateColRange( hWnd, lpSS, ssCol, -1 );

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL OPERATION
SSClearDoc( HWND hWnd, zBOOL bSkipHeader )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSClearDoc" );
   LPSPREADSHEET lpSS;
   SS_COORD      ss;

   lpSS = SS_Lock( hWnd );

   for ( ss = lpSS->Col.nHeaderCnt; ss < lpSS->Col.AllocCnt; ss++ )
      SS_DeAllocCol( hWnd, ss, FALSE, bSkipHeader );

   for ( ss = lpSS->Row.nHeaderCnt; ss < lpSS->Row.AllocCnt; ss++ )
      SS_DeAllocRow( hWnd, ss, FALSE, bSkipHeader );

   lpSS->Col.AllocCnt = lpSS->Col.nHeaderCnt;
   lpSS->Row.AllocCnt = lpSS->Row.nHeaderCnt;
   lpSS->Col.DataCnt = lpSS->Col.nHeaderCnt;
   lpSS->Row.DataCnt = lpSS->Row.nHeaderCnt;

// DrUnlockTaskMemory( lpSS );
   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

   return( TRUE );
}

zPCHAR
FindDIBBits( zPCHAR lpbi )
{
   TRACE_DEBUG( "<TBDIB.CPP>", "FindDIBBits" );
   return( lpbi + *((DWORD *) lpbi) +
           ::PaletteSize( (LPBITMAPINFOHEADER) lpbi ) );
}

void
InitBitmapInfoHeader( LPBITMAPINFOHEADER lpBmInfoHdr,
                      DWORD dwWidth,
                      DWORD dwHeight, int nBPP )
{
   TRACE_DEBUG( "<TBDIB.CPP>", "InitBitmapInfoHeader" );
   zmemset( lpBmInfoHdr, 0, zsizeof( BITMAPINFOHEADER ) );

   lpBmInfoHdr->biSize      = zsizeof( BITMAPINFOHEADER );
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

HANDLE
BitmapToDIB( HBITMAP hBitmap, HPALETTE hPal )
{
   TRACE_DEBUG( "<TBDIB.CPP>", "BitmapToDIB" );
   BITMAP             Bitmap;
   BITMAPINFOHEADER   bmInfoHdr;
   LPBITMAPINFOHEADER lpbmInfoHdr;
   zPVOID             lpBits;
   HDC                hMemDC;
   HANDLE             hDIB;
   HPALETTE           hOldPal = 0;

   if ( hBitmap == 0 )
      return( 0 );

   if ( !GetObject( hBitmap, zsizeof( Bitmap ), (zPCHAR) &Bitmap ) )
      return( 0 );

   InitBitmapInfoHeader( &bmInfoHdr,
                         Bitmap.bmWidth,
                         Bitmap.bmHeight,
                         Bitmap.bmPlanes * Bitmap.bmBitsPixel );

   // Now allocate memory for the DIB.  Then, set the BITMAPINFOHEADER
   //  into this memory, and find out where the bitmap bits go.
   hDIB = GlobalAlloc( GHND, zsizeof( BITMAPINFOHEADER ) +
                       ::PaletteSize( &bmInfoHdr ) +
                       bmInfoHdr.biSizeImage );

   if ( !hDIB )
      return( 0 );

   lpbmInfoHdr  = (LPBITMAPINFOHEADER) GlobalLock( hDIB );
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

   if ( !GetDIBits( hMemDC, hBitmap, 0, Bitmap.bmHeight, lpBits,
                    (LPBITMAPINFO) lpbmInfoHdr, DIB_RGB_COLORS ) )
   {
      GlobalUnlock( hDIB );
      GlobalFree( hDIB );
      hDIB = 0;
   }
   else
      GlobalUnlock( hDIB );

   if ( hOldPal )
      SelectPalette( hMemDC, hOldPal, FALSE );

   ReleaseDC( 0, hMemDC );

   return( hDIB );
}

int
PalEntriesOnDevice( HDC hDC )
{
   TRACE_DEBUG( "<TBDIB.CPP>", "PalEntriesOnDevice" );
   int nColors;

   // Find out the number of palette entries on this device.
   nColors = GetDeviceCaps( hDC, SIZEPALETTE );

   // For non-palette devices, we'll use the # of system
   // colors for our palette size.

   if ( !nColors )
      nColors = GetDeviceCaps( hDC, NUMCOLORS );

   return( nColors );
}

#define PALVERSION      0x300

HPALETTE
GetSystemPalette( )
{
   TRACE_DEBUG( "<TBDIB.CPP>", "GetSystemPalette" );
   HDC           hDC;
   HPALETTE      hPal = 0;
   HANDLE        hLogPal;
   LPLOGPALETTE  lpLogPal;
   int           ss, nColors;

   // Find out how many palette entries we want.
   hDC = GetDC( 0 );
   if ( !hDC )
      return( 0 );

   nColors = PalEntriesOnDevice( hDC );
   ReleaseDC( 0, hDC );

   // Allocate room for the palette and lock it.
   hLogPal = GlobalAlloc( GHND, zsizeof( LOGPALETTE ) +
                          nColors * zsizeof( PALETTEENTRY ) );
   if ( hLogPal == 0 )
      return( 0 );

   lpLogPal = (LPLOGPALETTE) GlobalLock( hLogPal );

   lpLogPal->palVersion    = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   for ( ss = 0; ss < nColors; ss++ )
   {
      lpLogPal->palPalEntry[ss].peBlue  = 0;
      *((WORD *) (&lpLogPal->palPalEntry[ ss ].peRed)) = ss;
      lpLogPal->palPalEntry[ ss ].peFlags = PC_EXPLICIT;
   }

   // Go ahead and create the palette.  Once it's created,
   // we no longer need the LOGPALETTE, so free it.
   hPal = CreatePalette( lpLogPal );

   GlobalUnlock( hLogPal );
   GlobalFree( hLogPal );

   return( hPal );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
SS_EditWndProc( HWND    hWnd,
                WORD    uMsg,
                WPARAM  wParam,
                LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_EditProc" );
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_EditWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   return( g_lpfnEditProc( hWnd, uMsg, wParam, lParam ) );
// return( SSx_EditProc( g_lpfnEditProc, hWnd, uMsg, wParam, lParam ) );
}

LRESULT CALLBACK
SS_EditHScrollWndProc( HWND    hWnd,
                       WORD    uMsg,
                       WPARAM  wParam,
                       LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_EditHScrollProc" );
   zLONG lValue;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_EditHScrollWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_KEYDOWN:
         GetToolBoxGlobal( TBG_CLEARFIELD, &lValue );

         if ( wParam == (WORD) lValue )
         {
            SetWindowText( hWnd, "" );
            return( 0 );
         }

         break;
   }

   return( SSx_EditProc( g_lpfnEditHScrollProc, hWnd, uMsg, wParam, lParam ) );
}

zLONG
SSx_EditProc( WNDPROC  lpfnProc,
              HWND     hWnd,
              WORD     uMsg,
              WPARAM   wParam,
              LPARAM   lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SSx_EditProc" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   WORD          wShift = 0;
   zBOOL         fCtrlState;
   zLONG         lRet;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SSx_EditProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_PASTE:
         return( SS_EditPasteProc( lpfnProc, hWnd, uMsg, wParam, lParam ) );

      case WM_KEYDOWN:
         if ( (wParam == VK_INSERT && HIBYTE( GetKeyState( VK_SHIFT ) )) ||
              (HIBYTE( GetKeyState( VK_CONTROL ) ) &&
               (wParam == 'v' || wParam == 'V')) )
         {
            return( SS_EditPasteProc( lpfnProc, hWnd, uMsg, wParam, lParam ) );
         }

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
              (wParam == 'c' || wParam == 'C') )
         {
            break;
         }

         switch ( wParam )
         {
            case VK_UP:
            case VK_DOWN:
               if ( GetWindowLong( hWnd, GWL_STYLE ) & ES_MULTILINE )
               {
                  lpSS = SS_Lock( GetParent( hWnd ) );
                  fCtrlState = HIBYTE( GetKeyState( VK_CONTROL ) );

                  wShift = 0;

                  if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
                     wShift |= 0x01;

                  if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
                     wShift |= 0x02;

                  if ( (lpSS->fArrowsExitEditMode && !fCtrlState &&
                        !HIBYTE( GetKeyState( VK_SHIFT ) )) ||
                       (lpSS->fArrowsExitEditMode == 0 && fCtrlState) )
                  {
//                   DrUnlockTaskMemory( lpSS );
                     SendMessage( GetParent( hWnd ), WM_KEYDOWN, wParam, 0L );
                     return( 0 );
                  }

//                DrUnlockTaskMemory( lpSS );

                  if ( SS_SendMsg( GetParent( hWnd ), SSM_KEYDOWN,
                                   GetWindowLong( GetParent( hWnd ), GWL_ID ),
                                                  MAKELONG( wParam, wShift ) ) )
                     return( 0 );

                  return( CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg,
                                          wParam, lParam ) );
               }

               break;

            case VK_LEFT:
            case VK_RIGHT:
               if ( GetWindowLong( hWnd, GWL_STYLE ) &
                    (ES_MULTILINE | ES_AUTOHSCROLL) )
               {
                  HWND hWndSpread = GetParent( hWnd );

                  lpSS = SS_Lock( hWndSpread );
                  fCtrlState = HIBYTE( GetKeyState( VK_CONTROL ) );

                  if ( (lpSS->fArrowsExitEditMode && !fCtrlState &&
                        !HIBYTE( GetKeyState( VK_SHIFT ) )) ||
                       (lpSS->fArrowsExitEditMode == 0 && fCtrlState) )
                  {
//                   DrUnlockTaskMemory( lpSS );
                     SendMessage( hWndSpread, WM_KEYDOWN, wParam, 0L );
                     return( 0 );
                  }
                  else
                  if ( lpSS->fArrowsExitEditMode && fCtrlState )
                  {
                     BYTE pbKeyState[ 256 ];
                     BYTE bControlState;

                     GetKeyboardState( pbKeyState );
                     bControlState = pbKeyState[ VK_CONTROL ];
                     pbKeyState[ VK_CONTROL ] = 0;
                     SetKeyboardState( pbKeyState );
                     CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg,
                                     wParam, lParam );
                     pbKeyState[ VK_CONTROL ] = bControlState;
                     SetKeyboardState( pbKeyState );
                     return( 0 );
                  }

//                DrUnlockTaskMemory( lpSS );
#ifdef SS_VB
                  SS_SendMessageToVB( hWndSpread, uMsg, wParam, lParam );
#endif
                  return( CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg,
                                          wParam, lParam ) );
               }

               break;

            case VK_RETURN:
               if ( GetWindowLong( hWnd, GWL_STYLE ) & ES_WANTRETURN )
               {
                  if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
                     wShift |= 0x01;

                  if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
                     wShift |= 0x02;

                  if ( SS_SendMsg( GetParent( hWnd ), SSM_KEYDOWN,
                                   GetWindowLong( GetParent( hWnd ), GWL_ID ),
                                   MAKELONG( wParam, wShift ) ) )
                  {
                     return( 0 );
                  }

                  return( CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg,
                                          wParam, lParam ) );
               }

               break;
         }

         break;

      case WM_CHAR:
         lpSS = SS_Lock( GetParent( hWnd ) );

         if ( lpSS->fFormulaMode )
            zmemcpy( &CellType, &lpSS->FormulaCellType,
                     zsizeof( SS_CELLTYPE ) );
         else
            SS_RetrieveCellType( lpSS, &CellType, 0,
                                 lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

//       DrUnlockTaskMemory( lpSS );

         if ( wParam == '\r' && (GetWindowLong( hWnd, GWL_STYLE ) & ES_WANTRETURN) )
            lRet = CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg,
                                   wParam, lParam );
         else
         if ( SS_EditIsValidKey( &CellType, wParam ) || wParam == '\b' )
         {
            if ( CellType.Spec.Edit.uCase == SS_CASE_UCASE )
               wParam = (WORD) zstrupr( (zPCHAR) MAKELONG( wParam, 0 ) );
            else
            if ( CellType.Spec.Edit.uCase == SS_CASE_LCASE )
               wParam = (WORD) zstrlwr( (zPCHAR) MAKELONG( wParam, 0 ) );

            lRet = SS_DefProc( lpfnProc, GetParent( hWnd ), hWnd, uMsg,
                               wParam, lParam );
         }
         else
            return( 0 );

#ifdef SS_GP
      {
         TBzPCHAR hText;
         LPSS_CELL      lpCell;
         LPSS_CELLTYPE  lpCellType;
         SS_CELLTYPE    CellTypeTemp;
         POINT          pt;
         RECT           rectCurrent;
         RECT           rect;
         zPCHAR         lpszText;
         zSHORT         nLth;
         zLONG          x;
         zLONG          y;
         zLONG          cx;
         zLONG          cy;

         SS_GetClientRect( GetParent( hWnd ), &Rect );

         lpSS = SS_Lock( GetParent( hWnd ) );
         lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt,
                                   lpSS->Row.ssCurrAt );
         lpCellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                           lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

         SS_GetCellCoord( lpSS, lpSS->Col.UL, lpSS->Row.UL,
                          lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt, &x, &y, &cx, &cy );

         if ( x + cx > rect.rssght )
            cx = rect.rssght - x;

         if ( y + cy > rect.bottom )
            cy = rect.bottom - y;

         GetWindowRect( hWnd, &RectCurrent );
         pt.x = 0;
         pt.y = 0;
         ClientToScreen( GetParent( hWnd ), &Point );
         OffsetRect( &RectCurrent, -Point.x, -Point.y );

         nLth = lpCellType->Spec.Edit.nLth;

         if ( DrAllocTaskMemory( (zCOREMEM) &hText, nLth + 1, 1029 ) == 0 )
         {
            lpszText = (zPCHAR) DrLockTaskMemory( hText );
            zmemset( lpszText, 0, nLth + 1 );
            GetWindowText( hWnd, lpszText, nLth + 1 );
            DrUnlockTaskMemory( lpszText );
         }

         zBOOL fModify;

         fModify = (zBOOL) SendMessage( hWnd, EM_GETMODIFY, 0, 0L );
         SS_SizeExpandingEdit( lpSS, hWnd, rect, lpCellType, x, y, cx, cy,
                               &RectCurrent, hText );
         SendMessage( hWnd, EM_SETMODIFY, (WORD)fModify, 0L );

         if ( hText )
            DrFreeTaskMemory( hText );

         SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
//       DrUnlockTaskMemory( lpSS );
      }
#endif
         return( lRet );
   }

   return( SS_DefProc( lpfnProc, GetParent( hWnd ), hWnd, uMsg,
                       wParam, lParam ) );
}

long
SS_EditPasteProc( WNDPROC lpfnProc,
                  HWND    hWnd,
                  WORD    uMsg,
                  WPARAM  wParam,
                  LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_EditPasteProc" );
   LPSPREADSHEET lpSS;
   HANDLE        hText;
   HANDLE        hTextOld;
   SS_CELLCOORD CellUL;
   SS_CELLCOORD CellLR;
   SS_CELLTYPE   CellType;
   zPCHAR        lpszText;
   zPCHAR        lpszTextTemp;
   zLONG         lRet = 0;
   zCHAR         chChar;
   zSHORT        nDest;
   zSHORT        nLth;
   zSHORT        k;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_EditPasteProc Msg: ", msgMap.GetMsgName( uMsg ) );

   lpSS = SS_Lock( GetParent( hWnd ) );
   if ( lpSS->fAutoClipboard )
   {
      // retrieve Clipboard text
      if ( (hTextOld = SS_GetTextFromClipboard( hWnd )) != 0 )
      {
         if ( lpSS->fFormulaMode )
            zmemcpy( &CellType, &lpSS->FormulaCellType,
                     zsizeof( SS_CELLTYPE ) );
         else
            SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

         lpszText = (zPCHAR) GlobalLock( hTextOld );

         CellUL.ssCol = lpSS->Col.ssCurrAt;
         CellUL.ssRow = lpSS->Row.ssCurrAt;
         CellLR.ssCol = lpSS->Col.ssCurrAt;
         CellLR.ssRow = lpSS->Row.ssCurrAt;

         if ( !SS_ValidatePasteData( GetParent( hWnd ), lpSS, lpszText, &CellUL, &CellLR, TRUE ) )
         {
            GlobalUnlock( hTextOld );
            return( 0 );
         }

         nLth = zstrlen( lpszText );
         GlobalUnlock( hTextOld );

         if ( (hText = GlobalAlloc( GHND, nLth + 1 )) != 0 )
         {
            lpszText = (zPCHAR) GlobalLock( hText );
            lpszTextTemp = (zPCHAR) GlobalLock( hTextOld );
            strcpy_s( lpszText, nLth + 1, lpszTextTemp );
            GlobalUnlock( hTextOld );

            for ( k = 0, nDest = 0; k < nLth; k++ )
            {
               chChar = lpszText[ k ];
               if ( SS_EditIsValidKey( &CellType, chChar ) )
               {
                  if ( CellType.Spec.Edit.uCase == SS_CASE_UCASE )
                     chChar = (char)(zLONG) zstrupr( (zPCHAR) MAKELONG( (WORD) chChar, 0 ) );
                  else
                  if ( CellType.Spec.Edit.uCase == SS_CASE_LCASE )
                     chChar = (char)(zLONG) zstrupr( (zPCHAR) MAKELONG( (WORD) chChar, 0 ) );

                  lpszText[ nDest++ ] = chChar;
               }
            }

            lpszText[ nDest ] = 0;
            GlobalUnlock( hText );

            if ( OpenClipboard( hWnd ) )
            {
               EmptyClipboard( );
               SetClipboardData( CF_TEXT, hText );
               CloseClipboard( );

               lRet = SS_DefProc( lpfnProc, GetParent( hWnd ), hWnd, uMsg, wParam, lParam );
               if ( OpenClipboard( hWnd ) )
               {
                  EmptyClipboard( );
                  SetClipboardData( CF_TEXT, hTextOld );
                  CloseClipboard( );
               }

               return( lRet );
            }
         }
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( lRet );
}

HANDLE
SS_GetTextFromClipboard( HWND   hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SS_GetTextFromClipboard" );
   HANDLE hMem;
   HANDLE hText = 0;
   zPCHAR lpszMem;
   zPCHAR lpszText;

   OpenClipboard( hWnd );
   if ( (hMem = GetClipboardData( CF_TEXT )) != 0 )
   {
      lpszMem = (zPCHAR) GlobalLock( hMem );

      if ( zstrlen( lpszMem ) )
      {
         zLONG lTextLth = zstrlen( lpszMem ) + 1;
         if ( (hText = GlobalAlloc( GHND, (DWORD) lTextLth )) != 0 )
         {
            lpszText = (zPCHAR) GlobalLock( hText );
            strcpy_s( lpszText, lTextLth, lpszMem );
            GlobalUnlock( hText );
         }
      }

      GlobalUnlock( hMem );
   }

   CloseClipboard( );
   return( hText );
}

long
SS_EditProcessTab( HWND    hWndSpread,
                   WORD    uMsg,
                   WPARAM  wParam,
                   LPARAM  lParam,
                   WORD    wShift )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_EditProcessTab" );
   LPSPREADSHEET lpSS;
   zBOOL         fProcessTab;

   lpSS = SS_Lock( hWndSpread );
   fProcessTab = lpSS->fProcessTab;
// DrUnlockTaskMemory( lpSS );

   if ( fProcessTab )
   {
      PostMessage( hWndSpread, SSM_PROCESSTAB, wParam, lParam );
   }
   else
   {
      if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
         PostMessage( GetParent( hWndSpread ), WM_NEXTDLGCTL, 1, 0L );
      else
         PostMessage( GetParent( hWndSpread ), WM_NEXTDLGCTL, 0, 0L );
   }

   return( TRUE );
}

void
SS_ShowEditField( HWND  hWndSpread,
                  HWND  hWnd )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_ShowEditField" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWndSpread );

   if ( lpSS->fEditModePermanent && lpSS->fEditModeDataChange == 0 )
   {
      SS_ShowActiveCell( hWndSpread, SS_SHOW_NEAREST );
      lpSS->fEditModeDataChange = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
}

zLONG OPERATION
SS_DefProc( WNDPROC  lpfnProc,
            HWND     hWndSS,
            HWND     hWnd,
            WORD     uMsg,
            WPARAM   wParam,
            LPARAM   lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_DefProc" );
   LPSPREADSHEET lpSS;
   SS_CELLCOORD  CellUL;
   SS_CELLCOORD  CellLR;
   WORD          wShift;
   zBOOL         fCtrlState;
   zLONG         lExtra = 0;
   zLONG         lRet = 0;
   zLONG         lRetTemp = 0;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_DefProc Msg: ", msgMap.GetMsgName( uMsg ) );

   switch ( uMsg )
   {
      case WM_GETDLGCODE:
         lpSS = SS_Lock( hWndSS );

         if ( lpSS->fEditModePermanent )
            lExtra = DLGC_WANTTAB;

//       DrUnlockTaskMemory( lpSS );
         return( CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg,
                                 wParam, lParam ) |
                 DLGC_WANTALLKEYS | lExtra );

      case WM_PASTE:
         lpSS = SS_Lock( hWndSS );

         CellUL.ssCol = lpSS->Col.ssCurrAt;
         CellUL.ssRow = lpSS->Row.ssCurrAt;
         CellLR.ssCol = lpSS->Col.ssCurrAt;
         CellLR.ssRow = lpSS->Row.ssCurrAt;

         if ( !SS_ValidatePasteData( hWndSS, lpSS, 0, &CellUL, &CellLR,
                                     TRUE ) )
         {
//          DrUnlockTaskMemory( lpSS );
            return( 0 );
         }

//       DrUnlockTaskMemory( lpSS );
         break;

      case WM_KEYDOWN:
         wShift = 0;
         SS_ShowEditField( hWndSS, hWnd );

         if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            wShift |= 0x01;

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            wShift |= 0x02;

         if ( lParam & 0x20000000L )        // Alt
            wShift |= 0x04;

#ifdef SS_GP
         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) &&
              (wParam == 'z' || wParam == 'Z') )
            wParam = VK_ESCAPE;
#endif

         if ( (wParam == VK_INSERT && HIBYTE( GetKeyState( VK_SHIFT ) )) ||
              (HIBYTE( GetKeyState( VK_CONTROL )) &&
               (wParam == 'v' || wParam == 'V')) )
         {
            lpSS = SS_Lock( hWndSS );

            CellUL.ssCol = lpSS->Col.ssCurrAt;
            CellUL.ssRow = lpSS->Row.ssCurrAt;
            CellLR.ssCol = lpSS->Col.ssCurrAt;
            CellLR.ssRow = lpSS->Row.ssCurrAt;

            if ( !SS_ValidatePasteData( hWndSS, lpSS, 0, &CellUL,
                                        &CellLR, TRUE ) )
            {
//             DrUnlockTaskMemory( lpSS );
               return( 0 );
            }

//          DrUnlockTaskMemory( lpSS );
         }

         // Check to see if the clipboard processing is turned off.
         lpSS = SS_Lock( hWndSS );
         if ( lpSS->fAutoClipboard == 0 )
         {
            if ( (wParam == VK_INSERT &&
                  (HIBYTE( GetKeyState( VK_SHIFT ) ) ||
                   HIBYTE( GetKeyState( VK_CONTROL ) ))) ||
                 (wParam == VK_DELETE &&
                  HIBYTE( GetKeyState( VK_SHIFT ) )) ||
                 (HIBYTE( GetKeyState( VK_CONTROL ) ) &&
                  (wParam == 'v' || wParam == 'V' ||
                   wParam == 'c' || wParam == 'C' ||
                   wParam == 'x' || wParam == 'X')) )
            {
//             DrUnlockTaskMemory( lpSS );
               return( 0 );
            }
         }

//       DrUnlockTaskMemory( lpSS );

         switch ( wParam )
         {
            case VK_ESCAPE:
#ifdef SS_VB
               SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
#endif

               lpSS = SS_Lock( hWndSS );
               lpSS->EscapeInProgress = TRUE;

//             DrUnlockTaskMemory( lpSS );
               PostMessage( hWndSS, SSM_SETEDITMODE, FALSE, 0L );
               return( TRUE );

            case VK_RETURN:
#ifdef SS_VB
               SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
#endif
   //            if ( SS_SendMsg( hWndSS, SSM_KEYDOWN,
   //                             GetWindowLong( hWndSS, GWL_ID ),
   //                             MAKELONG( wParam, wShift ) ) )
   //               return( 0 );

               PostMessage( hWndSS, SSM_PROCESSENTERKEY, 0, 0L );
               return( TRUE );

            case VK_UP:
            case VK_DOWN:
               SendMessage( hWndSS, WM_KEYDOWN, wParam, 0L );
               return( 0 );

            case VK_PRIOR:
            case VK_NEXT:
            case VK_F1:
               SendMessage( hWndSS, WM_KEYDOWN, wParam, 0L );
               return( 0 );

            case VK_LEFT:
            case VK_RIGHT:
               lpSS = SS_Lock( hWndSS );
               fCtrlState = HIBYTE( GetKeyState( VK_CONTROL ) );

               if ( ( lpSS->fArrowsExitEditMode && !fCtrlState &&
                    !HIBYTE( GetKeyState( VK_SHIFT ) ) ) ||
                   ( lpSS->fArrowsExitEditMode == 0 && fCtrlState ) )
               {
//                DrUnlockTaskMemory( lpSS );
                  SendMessage( hWndSS, WM_KEYDOWN, wParam, 0L );
                  return( 0 );
               }

//             DrUnlockTaskMemory( lpSS );

#ifdef SS_VB
               SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
#endif
   //            if ( SS_SendMsg( hWndSS, SSM_KEYDOWN,
   //                             GetWindowLong( hWndSS, GWL_ID ),
   //                             MAKELONG( wParam, wShift ) ) )
   //               return( 0 );

               break;

            case VK_HOME:
            case VK_END:
               if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
               {
                  SendMessage( hWndSS, WM_KEYDOWN, wParam, 0L );
                  return( 0 );
               }

#ifdef SS_VB
               SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
#endif
               break;

            case VK_TAB:
               return( SS_EditProcessTab( hWndSS, uMsg,
                                          wParam, lParam, wShift ) );

            default:
#ifdef SS_VB
               SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
#endif
               break;
         }

         break;

      case WM_KEYUP:
         wShift = 0;

         if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            wShift |= 0x01;

         if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            wShift |= 0x02;

         if ( lParam & 0x20000000L )        // Alt
            wShift |= 0x04;

#ifdef SS_VB
         SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
#endif
   //      if ( SS_SendMsg( hWndSS, SSM_KEYUP,
   //                       GetWindowLong( hWndSS, GWL_ID ),
   //                       MAKELONG( wParam, wShift ) ) )
   //         return( 0 );

         break;

      case WM_CHAR:
         lpSS = SS_Lock( hWndSS );
         lRet = -1;

   // #ifdef SS_VB
   //      SS_SendMessageToVB( hWndSS, uMsg, wParam, lParam );
   // #endif
         if ( lpSS->EditModeTurningOn == 0 &&
              SS_SendMsg( hWndSS, SSM_KEYPRESS,
                          GetWindowLong( hWndSS, GWL_ID ),
                          (WPARAM) &wParam ) ||
              wParam == 0 )
         {
            lRet = 0;
         }

         if ( wParam == '\r' || wParam == '\t' )
            lRet = 1;
         else
         if ( wParam == 27 )           // Do not send escape key to control
            lRet = 0;
         else
         {
            SS_ShowEditField( hWndSS, hWnd );

            lpSS = SS_Lock( hWndSS );

            if ( lpSS->fProhibitTypingWhenSel && SS_IsBlockSelected( lpSS ) &&
                 !( wParam == 3 || wParam == 22 || wParam == 24 ) )
            {
               if ( !SS_SendMsgCommand( hWndSS, 0, SSN_TYPINGPROHIBITED,
                                        FALSE ) )
               {
                  lRet = 0;
               }
            }
         }

//       DrUnlockTaskMemory( lpSS );

         if ( lRet != -1 )
            return( lRet );

         break;

      case WM_KILLFOCUS:
         CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg, wParam, lParam );

         lpSS = SS_Lock( hWndSS );

         lpSS->hWndFocusPrev = hWnd;

         if ( lpSS->EditModeTurningOn == 0 &&
              lpSS->EditModeTurningOff == 0 &&
              lpSS->wMessageBeingSent == 0 )
         {
            SendMessage( hWndSS, uMsg, wParam, lParam );
         }

//       DrUnlockTaskMemory( lpSS );

         return( 0 );
   }

   return( CallWindowProc( (WNDPROC) lpfnProc, hWnd, uMsg, wParam, lParam ) );
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
SS_ListBoxWndProc( HWND    hWnd,
                   WORD    uMsg,
                   WPARAM  wParam,
                   LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_EDIT.CPP>", "SS_ListBoxWndProc" );
   LPSPREADSHEET   lpSS;
   HFONT           hFontOld;
   POINT           pt;
   RECT            rect;
   HWND            hWndSS;
   HWND            hWndTemp;
   zSHORT          nShift;
   zLONG           lCurSel;
   zLONG           lSel;
   zLONG           lCount;
   HDC             hDC;
   TEXTMETRIC      tm;
   zLONG           lRet;
// static ZWndMsgMap msgMap;
//
// if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
//    TraceLineS( "SS_ListBoxWndProc Msg: ", msgMap.GetMsgName( uMsg ) );

   hWndSS = (HWND) GetProp( hWnd, "HWNDSPREAD" );
   hWndTemp = (HWND) GetProp( hWnd, "HWNDCOMBO" );

   switch ( uMsg )
   {
      case WM_SETFOCUS:
         InvalidateRect( hWnd, 0, TRUE );
         UpdateWindow( hWnd );
         break;

      case WM_MOUSEACTIVATE:
         return( 0 );

      case WM_LBUTTONDOWN:
         hDC = GetDC( hWnd );
         hFontOld = SelectFont( hDC,
                                (HFONT) SendMessage( hWnd, WM_GETFONT,
                                                     0, 0L ) );
         GetTextMetrics( hDC, &tm );
         SelectFont( hDC, hFontOld );
         ReleaseDC( hWnd, hDC );

         lCurSel = (HIWORD( lParam ) / tm.tmHeight) +
                      SendMessage( hWnd, LB_GETTOPINDEX, 0, 0L );

         lCurSel = mMin( lCurSel,
                         SendMessage( hWnd, LB_GETCOUNT, 0, 0L ) - 1 );

         if ( lCurSel != SendMessage( hWnd, LB_GETCURSEL, 0, 0L ) )
         {
            SendMessage( hWnd, LB_SETCURSEL, lCurSel, 0L );

            hWndTemp = (HWND) GetProp( hWnd, "HWNDCOMBO" );
            SendMessage( hWndTemp, WM_COMMAND, 0,
                         MAKELONG( 0, LBN_SELCHANGE ) );
         }

         SetCapture( hWnd );
         return( 0 );

      case WM_LBUTTONUP:
         ReleaseCapture( );
         PostMessage( hWndSS, SSM_PROCESSENTERKEY, 0, 0L );
         return( 0 );

      case WM_MOUSEMOVE:
         if ( wParam & MK_LBUTTON )
         {
            hWndTemp = GetCapture( );
            if ( hWndTemp && hWndTemp != hWnd )
            {
               SendMessage( hWndTemp, WM_LBUTTONUP, 0, 0L );
               SendMessage( hWnd, WM_LBUTTONDOWN, 0, 0L );
            }
            else
            {
               GetClientRect( hWnd, &rect );
               hDC = GetDC( hWnd );
               hFontOld = SelectFont( hDC,
                                      (HFONT) SendMessage( hWnd,
                                                           WM_GETFONT,
                                                           0, 0L ) );
               GetTextMetrics( hDC, &tm );
               SelectFont( hDC, hFontOld );
               ReleaseDC( hWnd, hDC );

               for ( ;; )
               {
                  if ( GetSystemMetrics( SM_SWAPBUTTON ) )
                  {
                     if ( (GetAsyncKeyState( VK_RBUTTON ) & 0x8000) == 0 )
                        break;
                  }
                  else
                  if ( (GetAsyncKeyState( VK_LBUTTON ) & 0x8000) == 0 )
                     break;

                  GetCursorPos( &pt );
                  ScreenToClient( hWnd, &pt );

                  lSel = SendMessage( hWnd, LB_GETCURSEL, 0, 0L );
                  lCount = SendMessage( hWnd, LB_GETCOUNT, 0, 0L );

                  if ( pt.y < 0 )
                  {
                     if ( lSel > 0 )
                        SendMessage( hWnd, WM_KEYDOWN, VK_UP, 0L );
                  }
                  else
                  if ( pt.y > rect.bottom )
                  {
                     if ( lSel < lCount - 1 )
                        SendMessage( hWnd, WM_KEYDOWN, VK_DOWN, 0L );
                  }
                  else
                  {
                     lCurSel = (pt.y / tm.tmHeight) +
                                  SendMessage( hWnd, LB_GETTOPINDEX, 0, 0L );

                     lCurSel = mMin( lCurSel, lCount - 1 );

                     if ( lCurSel != lSel )
                     {
                        SendMessage( hWnd, LB_SETCURSEL, lCurSel, 0L );

                        hWndTemp = (HWND) GetProp( hWnd, "HWNDCOMBO" );
                        SendMessage( hWndTemp, WM_COMMAND, 0,
                                     MAKELONG( 0, LBN_SELCHANGE ) );
                     }
                  }
               }
            }
         }

         break;

      case WM_KEYDOWN:
      {
         nShift = 0;

         SS_ShowEditField( hWndSS, hWnd );

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
               if ( SS_SendMsg( hWndSS, SSM_KEYDOWN,
                                GetWindowLong( hWndSS, GWL_ID ),
                                MAKELONG( wParam, nShift ) ) )
               {
                  return( 0 );
               }

               lpSS = SS_Lock( hWndSS );
               lpSS->EscapeInProgress = TRUE;

//             DrUnlockTaskMemory( lpSS );
               PostMessage( hWndSS, SSM_SETEDITMODE, FALSE, 0L );
               return( TRUE );

            case VK_RETURN:
               if ( SS_SendMsg( hWndSS, SSM_KEYDOWN,
                                GetWindowLong( hWndSS, GWL_ID ),
                                MAKELONG( wParam, nShift ) ) )
               {
                  return( 0 );
               }

               PostMessage( hWndSS, SSM_PROCESSENTERKEY, 0, 0L );
               return( TRUE );

            case VK_F1:
               SendMessage( hWndSS, WM_KEYDOWN, wParam, 0L );
               return( 0 );

            case VK_RIGHT:
            case VK_LEFT:
            case VK_UP:
            case VK_DOWN:
               if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
               {
                  SendMessage( hWndSS, WM_KEYDOWN, wParam, 0L );
                  return( 0 );
               }

               if ( SS_SendMsg( hWndSS, SSM_KEYDOWN,
                                GetWindowLong( hWndSS, GWL_ID ),
                                MAKELONG( wParam, nShift ) ) )
               {
                  return( 0 );
               }

               break;

            case VK_TAB:
               return( SS_EditProcessTab( hWndSS, uMsg,
                                          wParam, lParam, nShift ) );
         }

         lRet = CallWindowProc( (WNDPROC) g_lpfnListBoxProc,
                                hWnd, uMsg, wParam, lParam );

         hWndTemp = (HWND) GetProp( hWnd, "HWNDCOMBO" );
         SendMessage( hWndTemp, WM_COMMAND, 0, MAKELONG( 0, LBN_SELCHANGE ) );
         return( lRet );
      }
   }

   lRet = SS_DefProc( g_lpfnListBoxProc, hWndSS,
                      hWnd, uMsg, wParam, lParam );

   if ( uMsg == WM_LBUTTONUP )
      lRet = SS_DefProc( g_lpfnListBoxProc, hWndSS, hWnd,
                         WM_KEYDOWN, VK_RETURN, 0L );

   return( lRet );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif
