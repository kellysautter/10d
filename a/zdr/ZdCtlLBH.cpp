/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctllbh.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon ListBox with headers control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2003.04.30    DKS    Z10
//    Enhanced SetZCtrlProperty to permit setting of a column width.
//
// 2003.04.29    DKS    Z10
//    Reduce painting on hide of ToolTip.
//
// 2003.03.24    DKS    Z10
//    Attempt to minimize the number of OnItemChanged events for
//    multi-select listboxes.
//
// 2002.11.18    DKS
//    Permit setting of Left/Center/Right Justification of a ListBox column
//    using SetZCtrlProperty.
//
// 2002.11.15    DKS
//    When refreshing the list box and remember sort is not set, reset
//    the sort string and SetSortImage column to -1.
//
// 2002.07.30    DKS
//    When checking to see if the column attribute is already specified in
//    the sort string, ensure the attribute found is complete.
//
// 2002.07.08    DKS
//    Retain sorted order when possible by using bubblesort.
//
// 2002.04.17    DKS
//    Implemented tooltip on truncated listbox column data.
//
// 2002.03.12    DKS
//    Fix for recursive selchange events.
//
// 2002.03.04    TMV
//    Fix for Drag & Drop within a listbox / drop after last item.
//
// 2002.02.14    DKS
//    Added Domain translation via code.
//
// 2001.10.05    DKS   Z10
//    Validate all "remembered" sort order entities and attributes.
//
// 2001.10.01    DKS   54532
//    Sort on displayed data.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.21    DKS   Enhance
//    Added SetCtrlMapping to remap a Control.
//
// 2001.09.12    DKS   Z10
//    Added checks for mapping when the Subtask (window) is in a DELETE state.
//
// 2001.08.03    DKS   Z10
//    FormatTextAtPosition suppressed when the Subtask (window) is in a
//    DELETE state.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.03.27    DKS   QS999
//    Cleaned up memory leak.
//
// 2001.03.14    DKS   51255
//    Honor common "invisible" setting.
//
// 2001.03.14    DKS   51284
//    In multi-select list, deselection of an item is correctly reflected
//    in the OI.
//
// 2001.02.06    DKS   Enhance
//    Sort order visual representation final implementation.
//
// 2001.01.18    DKS   Frank H.
//    Sort order fix.
//
// 2001.01.08    DKS   Z10
//    Permit sizeable border (based on flag from common).
//
// 2001.01.02    DKS   Z10
//    Check for valid sort order from .ini file.
//
// 2000.10.09    DKS
//    Use contexts when sorting.
//
// 2000.10.02    DKS   Z10
//    Bianca ... Hopefully this solves the "mouse captured" problem between
//    the painter and the listbox.  When selecting a button in the painter
//    using the right mouse selection rectangle, followed by selecting an
//    edit box for update, followed by updating (and changing) mapping,
//    the OK or Cancel buttons on the mapping dialog do not work on the
//    first click.
//
// 2000.06.19    DKS   Z10
//    Bianca and Jeff ... AutoGray now working for selected entities.
//
// 2000.06.13    DKS   Z10
//    Button headers now use NLS when specified.
//
// 2000.05.20    BL    Z10
//    Modified ZListCtrl::OnLButtonUp: if Auto Drag Sort and relative
//    Position for Source Item > relative Position for Target Item,
//    then the Entity Cursor is not correct.
//
// 2000.03.17    TMV   WEB
//    Validate parent window's handle prior to call Create/CreateEx.
//
// 2000.03.15    DKS   Z10       Enhance
//    ListCtrl retains sort order.
//
// 1999.12.03    DKS   Z10       QS999
//    ListCtrl now protects against invalid views properly.
//
// 1999.12.01    DKS   Z10       QS999
//    ListCtrl uses new flag zCONTROL_STATUS_REPAINT in SetZCtrlState.
//    Disable as ReadOnly is now honored.  Fixed positioning bug.
//
// 1999.11.09    DKS   Z9J/Z10   RG999
//    ListCtrl SortByColumn checks to ensure that a sort has previously
//    occurred, otherwise it exits if the column to sort on is invalid.
//
// 1999.10.14    DKS   Z9J/Z10   FH999
//    ListCtrl now paints properly within a composite (e.g. GroupBox) ctrl.
//
// 1999.09.14    DKS   Z10       QS999
//    Do not hilite at cursor position unless selected for SINGLE select
//    option of the listbox ... but then do hilite first selected item.
//
// 1999.08.27    DKS   Z10       QS999
//    Protect against dropping incorrect local list view.
//
// 1999.07.30    DKS   Z10       QS999
//    Multi-select case brings first selected entity into view + debugging.
//
// 1999.07.08    DKS   Z10       QS999
//    Fixed a (sub-classing) bug that causes a system crash in Win9x.
//
// 1999.07.08    DKS   Z10       QS999
//    Stopped excessive flashing by removing the call to LockWindowUpdate.
//
// 1999.06.28    TMV   Z10       TB568
//    ZListCtrl::SetRowColText fixed a bug that causes a system crash.
//
// 1999.06.28    DKS   Z10       TB568
//    SetRowColText implemented for header listbox.
//
// 1999.05.11    DKS   Z10       ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//    Permit Drag and Drop to function as in ZListBox.
//
// 1999.04.06    DKS
//    Attempt to stop excessive flashing by removing the call to Update
//    on refresh (MapFromOI).
//
// 1999.02.15    DKS
//    HiliteSelectedText implemented.
//
// 1999.02.09    DKS
//    Honor right justify requests for columns.
//
// 1999.01.11    DKS
//    Ensure first column attribute is updateable if it is not protected.
//
// 1999.01.11    DKS
//    Eliminated "junk" painted in any column(s) with no width (TB 464).
//
// 1999.01.05    DKS
//    Color schemes carried through properly in DrawItem.
//
// 1998.12.01    DKS
//    Fixed painting when listbox is sitting on a groupbox.  Limited text
//    to 255 characters.
//
// 1998.11.20    DKS
//    Added sort flag to Button Header ListBox type.
//
// 1998.11.17    DKS
//    Implemented auto sort.
//
// 1998.11.12    DKS
//    First incarnation of Button Header Listbox ... drag & drop are not
//    yet implemented.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//
// 1998.07.31    DKS
//    Fixed list box when listing with scope of OI
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlLbh.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZListCtrl, CListCtrl )

BEGIN_MESSAGE_MAP( ZListCtrl, CListCtrl )
#if 0
   ON_CONTROL_REFLECT( LVN_INSERTITEM, LVNInsertItem )
   ON_CONTROL_REFLECT( LVN_DELETEITEM, LVNDeleteItem )
   ON_CONTROL_REFLECT( LVN_DELETEALLITEMS, LVNDeleteAllItems )
   ON_CONTROL_REFLECT( LVN_DELETEALLITEMS, LVNDeleteAllItems )
#endif

   ON_NOTIFY_REFLECT( LVN_COLUMNCLICK, OnColumnClick )
   ON_NOTIFY_REFLECT( LVN_GETDISPINFO, OnGetDispInfo )
   ON_NOTIFY_REFLECT( LVN_SETDISPINFO, OnSetDispInfo )
   ON_NOTIFY_REFLECT( LVN_ITEMCHANGING, OnItemChanging )
   ON_NOTIFY_REFLECT( LVN_ITEMCHANGED, OnItemChanged )
   ON_NOTIFY_REFLECT( LVN_BEGINDRAG, OnBeginDrag )
   ON_NOTIFY_REFLECT( LVN_BEGINRDRAG, OnBeginRDrag )
   ON_NOTIFY_REFLECT( LVN_ITEMACTIVATE, OnItemActivate )
   ON_NOTIFY_REFLECT( LVN_ODSTATECHANGED, OnOdStateChanged )
   ON_NOTIFY_REFLECT( LVN_BEGINLABELEDIT, OnBeginLabelEdit )
   ON_NOTIFY_REFLECT( LVN_ENDLABELEDIT, OnEndLabelEdit )
   ON_MESSAGE( LVM_SETIMAGELIST, OnSetImageList )
   ON_MESSAGE( LVM_SETTEXTCOLOR, OnSetTextColor )
   ON_MESSAGE( LVM_SETTEXTBKCOLOR, OnSetTextBkColor )
   ON_MESSAGE( LVM_SETBKCOLOR, OnSetBkColor )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_CREATE( )
   ON_WM_ERASEBKGND( )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_ENABLE( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_RBUTTONDBLCLK( )
   ON_WM_MOUSEMOVE( )
   ON_WM_KEYDOWN( )
   ON_WM_CHAR( )
   ON_WM_SIZE( )
   ON_WM_MEASUREITEM_REFLECT( )
   ON_WM_MEASUREITEM( )

   ON_COMMAND( ID_EDIT_COPY, OnEditCopy )
// ON_UPDATE_COMMAND_UI( ID_EDIT_COPY, OnUpdateEditCopy )
//  ON_COMMAND( ID_EDIT_CUT, OnEditCut )
// ON_UPDATE_COMMAND_UI( ID_EDIT_CUT, OnUpdateEditCut )
   ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )
// ON_UPDATE_COMMAND_UI( ID_EDIT_PASTE, OnUpdateEditPaste )

END_MESSAGE_MAP( )

// The default select set used by the Object Engine is 1.  There are two
// possible select sets used by a list box at any point ... the DISPLAY
// select set (entities marked as selected in the DISPLAY select set are
// displayed in the list box) and the SELECT select set (this select set
// is maintained by the list box to indicate those entities which have
// been selected by the user via the list box).  If the application wants
// to use a DISPLAY select set, it can use the default select set, but
// the SELECT select set must be different from the DISPLAY select set.

#define m_pzsScope  m_pzsAName // AN was stolen for the scoping entity name
#define zLISTBOX_SORTEDHEADERS      zCONTROL_SPECIFIC_2
#define zLISTBOX_COLUMNHEADERS      zCONTROL_SPECIFIC_3
#define zLISTBOX_SCOPE_PARENT       zCONTROLX_SPECIFIC_1

// This can be used since one of the above must be set as well.
#define zLISTBOX_SEPARATORS         zCONTROL_RESERVED_UNUSABLE

#define zLISTBOX_LABELEDIT          zMAPACT_CTRL_SPECIFIC1
#define zLISTBOX_RECURSIVECLICK     zMAPACT_CTRL_SPECIFIC2
#define zLISTBOX_ITEMCHANGE_ERROR   zMAPACT_CTRL_SPECIFIC3
#define zLISTBOX_RECURSIVESELCHANGE zMAPACT_CTRL_SPECIFIC4
#define zLISTBOX_SETFOCUS           zMAPACT_CTRL_SPECIFIC5
#define zLISTBOX_ON_CLICK           zMAPACT_CTRL_SPECIFIC6
#define zLISTBOX_PARENT_ENTITY      zMAPACT_CTRL_SPECIFIC7

#if !defined( _MSC_VER ) || _MSC_VER <= 1100
#define SubItemHitTest( plvhti ) (int) SNDMSG( m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)(LPLVHITTESTINFO)( plvhti ) )
#endif

// LRESULT CALLBACK
// SubClassHeaderButton( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#define LBH_TEXT_LTH  1024

// ZListCtrl - ctor
ZListCtrl::ZListCtrl( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CListCtrl( ),
           ZMapAct( pZSubtask,          // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "ListBox" ),
           m_csViewName( "_" ),
           m_csViewNameList( "__" ),
           m_wndListTip( pZSubtask->m_pZTask->m_lHoverDelay )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "Bomb" ) == 0 )
   {
      TraceLineS( "ListCtrl::ctor Name ", *m_pzsTag );
      Attr.H = 410;
      Attr.W = 410;
   }

   if ( m_pzsVName )
   {
      TraceLineS( "ListCtrl::ctor Name ", *m_pzsTag );
      TraceLineS( "ListCtrl::ctor View Name ", *m_pzsVName );
   }
#endif

   m_vApp = m_vAppList = 0;
   m_csViewName += *m_pzsTag;
   m_csViewNameList += *m_pzsTag;

   m_nRetryMapFromOI = 0;
   m_lEntityCnt = 0;
   m_pchListInfo = 0;
   m_lEntityNbr = -1;
   m_lLastSelected = -1;
   m_nCols = 0;
   m_nColsMax = 0;
   m_nColLastSorted = -1;
   m_nLinesPerRow = 1;
   m_nDisplaySelSet = 0;
   m_nSelectSelSet = 1;
   m_pCol = 0;
   m_bLabelEdit = FALSE;
   m_pchLabelText = 0;
   m_bInHilite = FALSE;

   m_nCntPerPage = 0;
   m_bFullRowSel = TRUE;
   m_bClientWidthSel = TRUE;
   m_nClientWidth = (zSHORT) Attr.X;
   m_nStateImageOffset = 0;
   m_chAscendDescend = 'A';
   m_lpfnHeaderButtonWndProc = 0;

   m_clrText = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrTextBk = ::GetSysColor( COLOR_WINDOW );
   m_clrBkgnd = ::GetSysColor( COLOR_WINDOW );
   m_clrTextSel = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
   m_clrTextBkSel = ::GetSysColor( COLOR_HIGHLIGHT );

   m_pRowColColorList = 0;
   m_pColorList = 0;

   m_pFontNormal = 0;
   m_pFontBold = 0;
   m_pFontItalic = 0;
   m_pFontUnderline = 0;

// if ( pCtrlDef->Subtype & zLISTBOX_SEPARATORS )
//    m_bSeparatorLines = TRUE;
// else
      m_bSeparatorLines = FALSE;

// TraceLineX( "ZListCtrl::ctor m_clrText: ", (zLONG) m_clrText );
// TraceLineX( "ZListCtrl::ctor m_clrTextBk: ", (zLONG) m_clrTextBk );
// TraceLineX( "ZListCtrl::ctor m_clrBkgnd: ", (zLONG) m_clrBkgnd );

   m_bRememberSort = FALSE;
   m_bSortDisplay = FALSE;
   m_pSortColumnList = 0;
   m_EntityKeyList.RemoveAll( );
   m_EntityKeyList.SetSize( 100, 100 );
   if ( pCtrlDef->Subtype & zLISTBOX_SORTEDHEADERS )
   {
      m_bAutoSort = TRUE;
      if ( pCtrlDef->Subtype & zLISTBOX_COLUMNHEADERS )
         m_bRememberSort = TRUE;

      if ( pCtrlDef->Subtype & zLISTBOX_SORT_DISPLAY )
      {
      // m_bSortDisplay = TRUE;  turned off (untested DKS 9/19/2003)
      }
   }
   else
      m_bAutoSort = FALSE;

// TraceLineS( "LBH Tag: ", *m_pzsTag );
// TraceLineI( "LBH AutoSort: ", m_bAutoSort );
   if ( pCtrlDef->Subtype & zLISTBOX_SCOPE_OI )
      m_ulFlag = zQUAL_SCOPE_OI;
   else
   {
      m_ulFlag = 0;
      if ( pCtrlDef->StyleX & zLISTBOX_SCOPE_PARENT )
         m_ulMapActFlags |= zLISTBOX_PARENT_ENTITY;
   }

   if ( pCtrlDef->Subtype & zLISTBOX_AUTODRAGSORT )
   {
      m_ulMapActFlags |= zMAPACT_DRAG_ENABLED;
      m_bAutoDragSort = TRUE;
      if ( m_pchDragDrop == 0 )
      {
         m_pchDragDrop = new char[ 2 ];
         m_pchDragDrop[ 0 ] = 0;
      }
   }
   else
      m_bAutoDragSort = FALSE;

   // The painter must ensure Entity if ViewName.
   if ( m_pzsVName )
   {
      zPCHAR pch;

#ifdef DEBUG_ALL
      TraceLineI( "In ctor for ZListCtrl", (zLONG) this );
      TraceLineS( "ZListCtrl View ", *m_pzsVName );
      if ( m_pzsEName )
         TraceLineS( "ZListCtrl Entity ", *m_pzsEName );

      if ( m_pzsScope )
         TraceLineS( "ZListCtrl Scope ", *m_pzsScope );
#endif

      GetAttributeLength( &m_ulListLth, vDialog, szlCtrl, szlCtrlBOI );
      if ( m_ulListLth )
      {
         m_pchListInfo = new char[ m_ulListLth ];
         GetVariableFromAttribute( m_pchListInfo, &m_ulListLth, zTYPE_BLOB, m_ulListLth, vDialog, szlCtrl, szlCtrlBOI, 0, 0 );

         // Convert now forces the first tab stop to always be zero.  We now have to convert longs to integers prior to calling
         // the Windows API SetTabStops (for NT, we should not have to do this).
         m_nCols = (zSHORT) (*((zPLONG) (m_pchListInfo + sizeof( zLONG ))));
         m_nColsMax = m_nCols;
         m_pCol = new zLB_COL[ (m_nCols + 1) ];
         zmemset( m_pCol, 0, sizeof( zLB_COL ) * (m_nCols + 1) );

         // We first have 3 longs + one long for each column in the listbox, then there is the list of tag/text labels
         // for each column, followed by the mapping information for each column.
         zSHORT nLists = 3;
         zULONG ulTagLth = (zUSHORT) (m_nCols + 3) * sizeof( zLONG );
         zULONG ulLth = (zUSHORT) (*((zPLONG) (m_pchListInfo + 2 * sizeof( zLONG ))));
         for ( zSHORT nCol = 1; nCol <= m_nCols; nCol++ )
         {
            m_pCol[ nCol ].nTabPos = (zSHORT) *((zPLONG) (m_pchListInfo + ((nCol + nLists) * sizeof( zLONG ))));
            if ( nCol == 1 && m_pCol[ nCol ].nTabPos == -1 )
            {
               m_bFullRowSel = FALSE;
               m_bLabelEdit = TRUE;
               m_pCol[ nCol ].nTabPos = 0;
            }

            if ( m_pZSubtask->m_bUseMapDlgUnits )
            {
               m_pCol[ nCol ].nTabPos = mConvertMapDlgToPixelX( m_pCol[ nCol ].nTabPos );
            }
            else
            {
               m_pCol[ nCol ].nTabPos = mConvertDlgUnitToPixelX( m_pCol[ nCol ].nTabPos );
            }

            // Get the number of dialog units allotted for this attribute.
            m_pCol[ nCol ].nColLth = (zSHORT) *((zPLONG) (m_pchListInfo + ulLth));
            if ( m_pCol[ nCol ].nColLth < 0 ) // convert flag to right justify
            {
               m_pCol[ nCol ].nColLth = -(m_pCol[ nCol ].nColLth);

               if ( nCol == 1 && m_bLabelEdit ) // cannot right justify first column
                  m_pCol[ nCol ].nJustify = LVCFMT_LEFT;    // left justify
               else
                  m_pCol[ nCol ].nJustify = LVCFMT_RIGHT;   // right justify
            }
            else
            {
               m_pCol[ nCol ].nJustify = LVCFMT_LEFT;       // left justify
            }

            if ( m_pZSubtask->m_bUseMapDlgUnits )
            {
               m_pCol[ nCol ].nExtent = mConvertMapDlgToPixelX( m_pCol[ nCol ].nColLth ) + 2;
               m_pCol[ nCol ].nColLth = mConvertMapDlgToCharX( m_pCol[ nCol ].nColLth );
            }
            else
            {
               m_pCol[ nCol ].nExtent = mConvertDlgUnitToPixelX( m_pCol[ nCol ].nColLth ) + 2;
               m_pCol[ nCol ].nColLth = mConvertDlgUnitToCharX( m_pCol[ nCol ].nColLth );
            }

            m_pCol[ nCol ].nMinColWidth = 0;
            m_pCol[ nCol ].nMaxColWidth = 0;

#ifdef DEBUG_ALL
            TraceLineI( "nChar DlgToChar = ", m_pCol[ nCol ].nColLth );
            TraceLineI( "nExtent DlgToPixel = ", m_pCol[ nCol ].nExtent );
#endif
            zLONG  k;

            ulLth += sizeof( zLONG );
            if ( m_pchListInfo[ ulLth ] )
            {
               k = *((zPLONG) (m_pchListInfo + ulLth)) + 1;
               ulLth += sizeof( zLONG );
               m_pCol[ nCol ].pchEntity = new char[ k ];
               strcpy_s( m_pCol[ nCol ].pchEntity, k, m_pchListInfo + ulLth );
               ulLth += k;   // skip past entity name (embedded names are forced by convert
                             // to maintain sizeof( zLONG ) byte boundaries)
            }
            else
            {
               ulLth += sizeof( zLONG );
               k = zstrlen( *m_pzsEName ) + 1;
               m_pCol[ nCol ].pchEntity = new char[ k ];
               strcpy_s( m_pCol[ nCol ].pchEntity, k, *m_pzsEName );
            }

            k = *((zPLONG) (m_pchListInfo + ulLth)) + 1;
            ulLth += sizeof( zLONG );

            m_pCol[ nCol ].pchAttrib = new char[ k ];
            strcpy_s( m_pCol[ nCol ].pchAttrib, k, m_pchListInfo + ulLth );
            ulLth += k;   // skip past attribute name (embedded names are forced by convert
                          // to maintain sizeof( zLONG ) byte boundaries)

            k = *((zPLONG) (m_pchListInfo + ulLth)) + 1;
            ulLth += sizeof( zLONG );
            if ( k == 1 )  // if k is 1 then there is no context
            {
               m_pCol[ nCol ].pchContext = 0;
            }
            else
            {
               m_pCol[ nCol ].pchContext = new char[ k ];
               strcpy_s( m_pCol[ nCol ].pchContext, k, m_pchListInfo + ulLth );
               ulLth += k;   // skip past context (embedded names are forced by convert
                             // to maintain sizeof( zLONG ) byte boundaries)
            }

            // Now get the tag and text information.
            k = *((zPLONG) (m_pchListInfo + ulTagLth)) + 1;
            ulTagLth += sizeof( zLONG );
            m_pCol[ nCol ].pchTag = new char[ k ];
            strcpy_s( m_pCol[ nCol ].pchTag, k, m_pchListInfo + ulTagLth );
            ulTagLth += k;   // skip past tag name (embedded names are forced by convert
                             // to maintain sizeof( zLONG ) byte boundaries)

            k = *((zPLONG) (m_pchListInfo + ulTagLth)) + 1;
            ulTagLth += sizeof( zLONG );
            m_pCol[ nCol ].pchText = new char[ k ];
            zmemcpy( m_pCol[ nCol ].pchText, m_pchListInfo + ulTagLth, k );
            ulTagLth += k;   // skip past text value (embedded names are forced by convert
                             // to maintain sizeof( zLONG ) byte boundaries)

            pch = zstrchr( m_pCol[ nCol ].pchText, '\t' );
            if ( pch )
            {
               zLONG  lNLS;
               zPCHAR pchTab = zstrchr( pch + 1, '\t' );
               *pch = 0;
               pch++;
               if ( pchTab )
               {
                  *pchTab = 0;
                  lNLS = *((zPLONG) (pchTab + 2));
                  if ( lNLS >= 0 )
                  {
                     SetAttributeFromInteger( vDialog, szlCtrl, "NLS", lNLS );
                     GetAddrForAttribute( (zPCHAR *) &pchTab, vDialog, szlCtrl, "D_Text" );
                     if ( *pchTab )
                     {
                        delete [] m_pCol[ nCol ].pchText;
                        k = zstrlen( pchTab ) + 1;
                        m_pCol[ nCol ].pchText = new char[ k ];
                        strcpy_s( m_pCol[ nCol ].pchText, k, pchTab );
                        pch = m_pCol[ nCol ].pchText;
                     }
                  }
               }

               k = zstrlen( pch ) + 1;
               m_pCol[ nCol ].pchMB_Text = new char[ k ];
               strcpy_s( m_pCol[ nCol ].pchMB_Text, k, pch );
            }
            else
            {
               m_pCol[ nCol ].pchMB_Text = 0;
            }

         }  // end of:  for ( zSHORT nCol = 0; nCol < m_nCols; nCol++ )

         m_pCol[ 1 ].nTabPos = 3;      // hard-code border indent
      }
   }

   Attr.Style = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS;

   if ( m_pZSubtask->m_pZTask->m_nOS_Version >= 60 )
      Attr.Style &= ~WS_BORDER;  // border causes problems in XP
   else
   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( pCtrlDef->Subtype & zCONTROL_BORDEROFF )
      Attr.Style &= ~WS_BORDER;
   else
      Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

// if ( pCtrlDef->Subtype & zLISTBOX_NOINTEGRALHEIGHT ) // always with 3D
//    Attr.Style |= LBS_NOINTEGRALHEIGHT;  NOT WITH OWNERDRAW

   if ( pCtrlDef->Subtype & zLISTBOX_MULTIPLESEL )
   {
      m_chMultiSelect = 'Y';
   }
   else
   {
      Attr.Style |= LVS_SINGLESEL;
      m_chMultiSelect = 0;
   }

   if ( pCtrlDef->Subtype & zLISTBOX_LISTWHENPARENTSEL )
      m_chListOnlyWhenSelected = 'Y';
   else
      m_chListOnlyWhenSelected = 0;

   if ( pCtrlDef->Subtype & zLISTBOX_HILITEATPOSITION )
      m_chHilitePosition = 'Y';
   else
      m_chHilitePosition = 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// Using CListCtrl
//
// Use a list control to display any arrangement of icons with labels,
// as in the Windows 95 Explorer, or columnar lists of text, with or
// without icons.  For a description of the four possible "views" (not
// to be confused with MFC views) you can have in a list control — icon
// view, small icon view, list view, and report view — see Views in the
// CListCtrl class overview.  In some views, users can drag icons to
// different positions or edit icon labels.  For examples, see:
//
//  The right-hand pane in Microsoft Windows 95 Explorer, which uses a
// list control in a nondialog window.  You can experiment with the
// available views in Explorer's View menu.
//  The Settings For box in the Developer Studio Build Settings dialog
// box, which uses a list control in a dialog box.
//
// For related information, see List View Controls in the Win32 SDK
// documentation.
//
// Note The Win32 SDK refers to list controls as "list view controls."
// This usage of "view" does not refer to MFC view classes,
// particularly CListView.  For more information, see List Control and
// List View.
//
// Structure of This Article
//
// This article covers the following topics:
//
//  List Control and List View
//  List Control Examples
//  List Items and Image Lists
//  Working with a List Control
//  Creating the List Control
//  Creating the Image Lists
//  Adding Columns to the Control (Report View)
//  Adding Items to the Control
//  Scrolling, Arranging, Sorting, and Finding
//  Processing Notification Messages
//  Changing List Control Styles
//  Destroying the List Control
//
// List Control and List View
//
// For convenience, MFC encapsulates the list control in two ways.  You
// can use list controls:
//
//  Directly, by embedding a CListCtrl object in a dialog class.
//  Indirectly, by using class CListView.
//
// CListView makes it easy to integrate a list control with the MFC
// document/view architecture, encapsulating the control much as
// CEditView encapsulates an edit control: the control fills the entire
// surface area of an MFC view.  (The view is the control, cast to
// CListView.)
//
// A CListView object inherits from CCtrlView and its base classes and
// adds a member function to retrieve the underlying list control.  Use
// view members to work with the view as a view.  Use the GetListCtrl
// member function to gain access to the list control's member
// functions.  Use these members to:
//
//  Add, delete, or manipulate "items" in the list.
//  Set or get list control attributes.
//
// To obtain a reference to the CListCtrl underlying a CListView, call
// GetListCtrl from your list view class:
//
// CListCtrl& ctlList = GetListCtrl();
//
// This article describes both ways to use the list control.
//
// List Control Examples
//
// The DAOVIEW sample application uses a CListView and, by extension,
// the underlying CListCtrl.
//
// Tip DAOVIEW also supplies a helpful support class for working with
// list views — see class CListCtrlEx in the sample.  Although
// CListCtrlEx is not a documented MFC class, you might find it useful
// in your own applications.
//
// List Items and Image Lists
// ==========================
// An "item" in a list control consists of an icon, a label, and
// possibly other information (in "subitems").
//
// The icons for list control items are contained in image lists.  One
// image list contains full-sized icons used in icon view.  A second,
// optional, image list contains smaller versions of the same icons for
// use in other views of the control.  A third optional list can contain
// "state" images, such as check boxes, for display in front of the
// small icons in certain views.
//
// For more information about list items, see List View Image Lists and
// Items and Subitems in the Win32 SDK documentation.  Also see class
// CImageList in the Class Library Reference and Using CImageList in
// this technical note.
//
// To create a list control, you need to supply image lists to be used
// when you insert new items into the list.  However, if you don't plan
// to display icons in your list view or list control, you don't need
// image lists.  See the DAOVIEW sample application for an illustration
// of a list view with no icons.
//
// Working with a List Control
// ===========================
// You can either use CListCtrl directly or use it via CListView.  MFC
// calls InitCommonControls for you.  The key tasks are as follows:
//
//  Creating the list control
//  Creating the image lists that the control uses
//  Adding columns to the control (report view)
//  Adding items to the control
//  Scrolling, arranging, sorting, and finding
//  Processing notification messages from the control to its parent window
//  Changing list control styles
//  Destroying the list control and its data
//
// Some of these tasks are different if you use a CListView rather than
// a CListCtrl.
//
// Creating the List Control
// =========================
// How the list control is created depends on whether you're using the
// control directly or using class CListView instead.  If you use
// CListView, the framework constructs the view as part of its
// document/view creation sequence.  Creating the list view creates the
// list control as well (the two are the same thing).  The control is
// created in the view's OnCreate handler function.  In this case, the
// control is ready for you to add items, via a call to GetListCtrl.
//
// To use CListCtrl directly in a dialog box
//
// 1. In the dialog editor, add a List Control to your dialog template
// resource.  Specify its control ID.
//
// 2. Use ClassWizard to add a member variable of type CListCtrl with
// the Control property.  You can use this member to call CListCtrl
// member functions.
//
// 3. Use ClassWizard to map handler functions in the dialog class for
// any list control notification messages you need to handle.
//
// 4. In OnInitDialog, set the styles for the CListCtrl.  See Changing
// List Control Styles.  This determines the kind of "view" you get in
// the control, although you can change the view later.
//
// To use CListCtrl in a nondialog window
//
// 1. Define the control in the view or window class.
// 2. Call the control's Create member function, possibly in
// OnInitialUpdate, possibly as early as the parent window's OnCreate
// handler function (if you're subclassing the control).  Set the styles
// for the control.
//
// Creating the Image Lists
// ========================
// Creating image lists is the same whether you use CListView or
// CListCtrl.
//
// Note You only need image lists if your list control includes the
// LVS_ICON style.
//
// Use class CImageList to create one or more image lists (for
// full-size icons, small icons, and states).  See CImageList, and see
// List View Image Lists in the Win32 SDK documentation.
//
// Call CListCtrl::SetImageList for each image list; pass a pointer to
// the appropriate CImageList object.
//
// Adding Columns to the Control (Report View)
//
// Adding columns, which contain "subitems," applies only to the
// LVS_REPORT style, also known as report view.  For example, the
// Details view in Windows 95 Explorer is a report view.  The first
// column lists folder and file icons and labels.  Other columns list
// file size, file type, date last modified, and so on.  Each column
// normally has a header control (see CHeaderCtrl) that labels the
// column and allows users to resize the column.  For a code example,
// see the DAOVIEW sample application.
//
// Adding columns is the same whether you use CListView or CListCtrl.
//
// If your list control will have a report view, add columns to the
// control.  Prepare an LV_COLUMN structure and call InsertColumn for
// each column.
//
// Adding Items to the Control
// ===========================
// Add items to the control.  Call one of several versions of the
// InsertItem member function, depending on what information you have.
// One version takes a prepared LV_ITEM structure.  Another version
// takes the index of the item in an image list plus the item's label,
// and so on.  Generally, the list control manages storage for list
// items, but you can store some of the information in your application
// instead, using "callback items."
//
// Callback Items
// ==============
// A "callback item" is a list view item for which the application -
// rather than the control - stores the text, icon, or both.  Although
// a list view control can store these attributes for you, you may want
// to use callback items if your application already maintains some of
// this information.  The callback mask specifies which item state bits
// are maintained by the application, and it applies to the whole
// control rather than to a specific item.  The callback mask is zero
// by default, meaning that the control tracks all item states.  If an
// application uses callback items or specifies a nonzero callback mask,
// it must be able to supply list view item attributes on demand.
//
// You can define a callback item by specifying appropriate values for
// the pszText and iImage members of the LV_ITEM structure (see
// CListCtrl::GetItem).  If the application maintains the item's or
// subitem's text, specify the LPSTR_TEXTCALLBACK value for the pszText
// member.  If the application keeps track of the icon for the item,
// specify the I_IMAGECALLBACK value for the iImage member.
//
// For more information, see Adding Items to a List View Control and
// Callback Items and the Callback Mask in the Win32 SDK documentation.
//
// Scrolling, Arranging, Sorting, and Finding
// ==========================================
// List controls are scrollable by default.  For more information, see
// Scroll Position in the Win32 SDK documentation and the Scroll member
// function.
//
// You can call CListCtrl member functions to arrange list items in the
// control, sort items, and find particular items.  For more
// information, see Arranging, Sorting, and Finding in the Win32 SDK
// documentation and the CListCtrl members Arrange, SortItems, and
// FindItem.
//
// Processing Notification Messages
// ================================
// As users click column headers, drag icons, edit labels, and so on,
// the list control sends notification messages to its parent window.
// Handle these messages if you want to do something in response.   For
// example, when the user clicks a column header, you might want to
// sort the items based on the contents of the clicked column, as in
// Microsoft Mail.
//
// Process WM_NOTIFY messages from the list control in your view or
// dialog class.  Use WizardBar or ClassWizard to create an
// OnChildNotify handler function with a switch statement based on
// which notification message is being handled.
//
// For a list of the notifications a list control can send to its
// parent window, see List View Control Notification Messages in the
// Win32 SDK documentation.
//
// Changing List Control Styles
// ============================
// You can change the window style of a list control at any time after
// you create it.  By changing the window style, you change the kind of
// view the control uses.  For example, to emulate the Windows 95
// Explorer, you might supply menu items or toolbar buttons for
// switching the control between different views: icon view, list view,
// and so on.
//
// When the user selects your menu item, call GetWindowLong to retrieve
// the current style of the control.  Then call SetWindowLong to reset
// the style.  For more information, see Changing List View Styles in
// the Win32 SDK documentation.
//
// Available styles, which you can combine with the bitwise-OR
// operator, are listed in Create.  The styles LVS_ICON, LVS_SMALLICON,
// LVS_LIST, and LVS_REPORT designate the four list control views.
//
// Note that you can combine some of these styles: for example,
//
// LVS_ICON | LVS_REPORT
//
// gives a report view with multiple columns of information.  The first
// column contains icons and labels.  If you use LVS_REPORT alone, you
// don't get icons, and you don't need image lists.  See the DAOVIEW
// sample application for an example.
//
// Destroying the List Control
// ===========================
// If you embed your CListCtrl object as a data member of a view or
// dialog class, it is destroyed when its owner is destroyed.  If you
// use a CListView, the framework destroys the control when it destroys
// the view.
//
// If you arrange for some of your list data to be stored in the
// application rather than the list control, you will need to arrange
// for its deallocation.  For more information, see Callback Items and
// the Callback Mask in the Win32 SDK documentation.
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
// The dwStyle parameter can be a combination of the following values:
//
// LVS_ALIGNLEFT - Specifies that items are left-aligned in icon and
//                 small icon view.
// LVS_ALIGNTOP - Specifies that items are aligned with the top of the
//                control in icon and small icon view.
// LVS_AUTOARRANGE - Specifies that icons are automatically kept arranged
//                   in icon view and small icon view.
// LVS_EDITLABELS - Allows item text to be edited in place.  The parent
//                  window must process the LVN_ENDLABELEDIT notification
//                  message.
// LVS_ICON - Specifies icon view.
// LVS_LIST - Specifies list view.
// LVS_NOCOLUMNHEADER - Specifies that a column header is not displayed
//                      in report view.  By default, columns have headers
//                      in report view.
// LVS_NOLABELWRAP - Displays item text on a single line in icon view.
//                   By default, item text can wrap in icon view.
// LVS_NOSCROLL - Disables scrolling.  All items must be within the client
//                area.
// LVS_NOSORTHEADER - Specifies that column headers do not work like buttons.
//                    This style is useful if clicking a column header in
//                    report view does not carry out an action, such as sorting.
// LVS_OWNERDRAWFIXED - Enables the owner window to paint items in report view.
//                      The list view control sends a WM_DRAWITEM message to
//                      paint each item; it does not send separate messages for
//                      each subitem.  The itemData member of the DRAWITEMSTRUCT
//                      structure contains the item data for the specified list
//                      view item.
// LVS_REPORT - Specifies report view.
// LVS_SHAREIMAGELISTS - Specifies that the control does not take ownership of
//                       the image lists assigned to it (that is, it does not
//                       destroy the image lists when it is destroyed).  This
//                       style enables the same image lists to be used with
//                       multiple list view controls.
// LVS_SHOWSELALWAYS - Always show the selection, if any, even if the
//                     control does not have the focus.
// LVS_SINGLESEL - Allows only one item at a time to be selected.  By default,
//                 multiple items can be selected.
// LVS_SMALLICON - Specifies small icon view.
// LVS_SORTASCENDING - Sorts items based on item text in ascending order.
// LVS_SORTDESCENDING - Sorts items based on item text in descending order.
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
// CListCtrl::SetItem
//
// BOOL SetItem( const LV_ITEM *pItem );
//
// BOOL SetItem( int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
//               int nImage, UINT nState, UINT nStateMask, LPARAM lParam );
//
// Return Value:  Nonzero if successful; otherwise zero.
//
// Parameters:
//
//    pItem - Address of an LV_ITEM structure that contains the new item
//       attributes.  The iItem and iSubItem members identify the item or
//       subitem, and the mask member specifies which attributes to set.
//       For more information on the mask member, see the Remarks.  For
//       more information on the LV_ITEM structure, see CListCtrl::GetItem.
//    nItem - Index of the item whose attributes are to be set.
//    nSubItem - Index of the subitem whose attributes are to be set.
//    nMask - Specifies which attributes are to be set (see the Remarks).
//    lpszItem - Address of a null-terminated string specifying the
//       item's label.
//    nImage - Index of the item's image within the image list.
//    nState - Specifies values for states to be changed (see the Remarks).
//    nStateMask - Specifies which states are to be changed (see the Remarks).
//    lParam - A 32-bit application-specific value to be associated with
//       the item.
//
// Remarks:
//    Sets some or all of a list view item's attributes.
//
//    The iItem and iSubItem members of the LV_ITEM structure and the nItem
//    and nSubItem parameters identify the item and subitem whose attributes
//    are to be set.
//
//    The mask member of the LV_ITEM structure and the nMask parameter
//    specify which item attributes are to be set:
//      LVIF_TEXT - The pszText member or the lpszItem parameter
//       is the address of a null-terminated string; the cchTextMax
//       member is ignored.
//      LVIF_STATE - The stateMask member or nStateMask parameter
//       specifies which item states to change and the state member
//       or nState parameter contains the values for those states.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// BOOL GetItem( LV_ITEM *pItem ) const;
//
// Return Value:  Nonzero if successful; otherwise zero.
//
// Parameters:
//
// pItem - Pointer to an LV_ITEM structure that receives the item's
//         attributes.
//
// Remarks:  Retrieves some or all of a list view item's attributes.
//
// The LV_ITEM structure specifies or receives the attributes of a list
// view item:
//
// typedef struct _LV_ITEM
// {
//    UINT   mask;         // see below
//    int    iItem;        // see below
//    int    iSubItem;     // see below
//    UINT   state;        // see below
//    UINT   stateMask;    // see below
//    LPSTR  pszText;      // see below
//    int    cchTextMax;   // see below
//    int    iImage;       // see below
//    LPARAM lParam;       // 32-bit value to associate with item
//
// } LV_ITEM;
//
// Members are as follows:
//
//    mask - Variable specifying which members contain valid data or
//           which members are to be filled in.  It can be one or more
//           of these values:
//       LVIF_TEXT  - the pszText member is valid.
//       LVIF_IMAGE - the iImage member is valid
//       LVIF_PARAM - the lParam member is valid.
//       LVIF_STATE - the state member is valid.
//
//    iItem - Index of the item this structure refers to.
//
//    iSubItem - A "subitem" is a string that, in report view, can be
//           displayed in a column to the right of an item's icon and
//           label.  All items in a list view have the same number of
//           subitems.  This member is the one-based index of a subitem,
//           or zero if the structure contains information about an item.
//
//    state and stateMask - Current state of the item, and the valid
//          states of the item.  These members can be any valid
//          combination of the following state flags:
//      LVIS_CUT - The item is marked for a cut and paste operation.
//      LVIS_DROPHILITED - The item is highlighted as a drag and drop target.
//      LVIS_FOCUSED - The item has the focus, so it is surrounded by a
//          standard focus rectangle.  Although more than one item may be
//          selected, only one item can have the focus.
//      LVIS_SELECTED - The item is selected.  The appearance of a selected
//          item depends on whether it has the focus and on the system colors
//          used for selection.
//      LVIS_OVERLAYMASK - The application stores the image list index of the
//          current overlay image for each item.
//      LVIS_STATEIMAGEMASK - The application stores the image list index of
//          the current state image for each item.
//
//    pszText - Address of a null-terminated string containing the item text
//       if the structure specifies item attributes.  If this member is the
//       LPSTR_TEXTCALLBACK value, the item is a callback item.  If the
//       structure is receiving item attributes, this member is the address
//       of the buffer that receives the item text.
//
//    cchTextMax - Size of the buffer pointed to by the pszText member if
//       the structure is receiving item attributes.  If the structure
//       specifies item attributes, this member is ignored.
//
//    iImage - Index of the list view item's icon in the large icon and
//       small icon image lists.  If this member is the I_IMAGECALLBACK
//       value, the item is a callback item.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
void
ZListCtrl::CreateZ( )
{
#ifdef DEBUG_ALL
   if ( zstrstr( *m_pzsTag, "RosterList" ) )
   {
      TraceLine( "ZListCtrl::CreateZ: Dlg.Wnd.Ctrl %s.%s.%s",
                 *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), *m_pzsTag );
   }
#endif
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

// if ( (m_ulMapActFlags & zMAPACT_ENABLED) || (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( Attr.Style, CRect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H ), m_pWndParent, m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   // SetExtendedStyle( LVS_EX_HEADERDRAGDROP );
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
// int
// InsertColumn( int nCol, LPCTSTR lpszColumnHeading,
//               int nFormat = LVCFMT_LEFT,
//               int nWidth = -1, int nSubItem = -1 );
//
// Return Value:
// The index of the new column if successful or -1 otherwise.
//
// Parameters
//
// nCol - The index of the new column.
// lpszColumnHeading - Address of a string containing the column's heading.
// nFormat - Integer specifying the alignment of the column.  It can be one
//           of these values:  LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
// nWidth - Width of the column, in pixels.  If this parameter is -1, the
//          column width is not set.
// nSubItem - Index of the subitem associated with the column.  If this
//            parameter is -1, no subitem is associated with the column.
//
/////////////////////////////////////////////////////////////////////////////

BOOL
ZListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "DKS" ) == 0 )
   {
      TraceLineS( "ZListCtrl::PreCreateWindow DKS: ", *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZListCtrl::PreCreateWindow: ", m_pCol[ 1 ].pchAttrib );
   }
   TraceLineS( "ZListCtrl::PreCreateWindow ", *m_pzsTag );
#endif
   // Default is report view and full row selection.
   cs.style &= ~LVS_TYPEMASK;
   cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED;
// m_bFullRowSel = TRUE;

   if ( m_bLabelEdit )
   {
      cs.dwExStyle |= WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE; // | LVS_EX_LABELTIP;
      if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 || (m_ulMapActFlags & zMAPACT_AUTO_DISABLED) )
      {
         cs.style &= ~LVS_EDITLABELS;
      }
      else
      {
         cs.style |= LVS_EDITLABELS;
      }
   }
   else
   {
      cs.dwExStyle |= WS_EX_NOPARENTNOTIFY | WS_EX_STATICEDGE; // | LVS_EX_LABELTIP;
   }

   return( CListCtrl::PreCreateWindow( cs ) );
}

void
ZListCtrl::GetPreferencesFromIni( )
{
   zCHAR  szBuffer[ 40 ];
   int    nLth = 120 * m_nCols;  // (Ent.Attr.Context + fudge) * # of columns
   zPCHAR pchBuffer = new char[ nLth + 1 ];
   zPCHAR pchComma;
   zPCHAR pch;
   int    nCol;

   strcpy_s( szBuffer, sizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, sizeof( szBuffer ) - 5, *m_pzsTag );
   GetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pchBuffer, nLth );

   nCol = 0;
   if ( *pchBuffer )
   {
      pch = pchBuffer;
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
      for ( nCol = 1; nCol <= m_nCols; nCol++ )
      {
         pchComma = zstrchr( pch, ',' );
         if ( pchComma )
            *pchComma = 0;

         m_pCol[ nCol ].nExtent = (zSHORT) zatol( pch );

         if ( pchComma )
            pch = pchComma + 1;
         else
            break;
      }

      if ( m_bRememberSort )
      {
         zBOOL bLocateSort = FALSE;

         strcpy_s( szBuffer, sizeof( szBuffer ), "Sort:" );
         strcpy_s( szBuffer + 5, sizeof( szBuffer ) - 5, *m_pzsTag );
         GetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pchBuffer, nLth );
         zSHORT k = (zSHORT) zstrlen( pchBuffer );
         if ( k )
         {
            // Look for "check bytes" at end of string.
            if ( k > 1 && pchBuffer[ k - 1 ] == '#' && pchBuffer[ k - 2 ] == '#' )
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

                     for ( nCol = 1; nCol <= m_nCols; nCol++ )
                     {
                        if ( zstrcmp( m_pCol[ nCol ].pchEntity, pch ) == 0 &&
                             zstrcmp( m_pCol[ nCol ].pchAttrib, pchPeriod + 1 ) == 0 &&
                             ((pchBracket == 0 && (m_pCol[ nCol ].pchContext == 0 || m_pCol[ nCol ].pchContext[ 0 ] == 0)) ||
                              (pchBracket && m_pCol[ nCol ].pchContext && zstrcmp( m_pCol[ nCol ].pchContext, pchSpace + 4 ) == 0)) )
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
                     zLONG lMsgLth = zstrlen( pchBuffer ) + 256;
                     zPCHAR pchMsg = new char[ lMsgLth ];

                     // Invalid entry ... get out!
                     sprintf_s( pchMsg, lMsgLth, "ZListCtrl invalid sort order: %s=%s", szBuffer, pchBuffer );
                     TraceLineS( pchMsg, "" );
                     pchBuffer[ 0 ] = 0;
                     m_nColLastSorted = -1;
                     m_chAscendDescend = 0;
                     delete( pchMsg );
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
}

int
ZListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   if ( zstrstr( *m_pzsTag, "RosterList" ) )
   {
      TraceLine( "ZListCtrl::OnCreate: Dlg.Wnd.Ctrl %s.%s.%s",
                 *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), *m_pzsTag );
   }
#endif

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   if ( mIs_hWnd( m_hWnd ) )
      m_wndListTip.Create( this );

   SetWindowText( *m_pzsTag );
   m_nCntPerPage = GetCountPerPage( );

   // Moved to MeasureItem so it is done in time.
// TraceLineS( "ZListCtrl::OnCreate SetFontOverride for ListBox: ", *m_pzsTag );
// SetFontOverride( );

   GetPreferencesFromIni( );

   // Loop processing each column.
   LV_COLUMN lvc;
   zSHORT    nCol;

   for ( nCol = 0; nCol < m_nCols; nCol++ )
   {
      lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
      lvc.fmt = m_pCol[ nCol + 1 ].nJustify;
      lvc.pszText = (zPCHAR) m_pCol[ nCol + 1 ].pchText;
      lvc.iSubItem = nCol;
      lvc.cx = m_pCol[ nCol + 1 ].nExtent;
//    if ( InsertColumn( nCol, m_pCol[ nCol + 1 ].pchText,
//                       m_pCol[ nCol + 1 ].nJustify,
//                       m_pCol[ nCol + 1 ].nExtent, nCol ) != nCol )
      if ( InsertColumn( nCol, &lvc ) != nCol )
      {
         TraceLineI( "ZListCtrl::OnCreate InsertColumn failed: ", nCol + 1 );
      }
   }

#if 0
   // Subclass the Button control.
// CWnd *pButtonWnd = GetWindow( GW_CHILD );
   CHeaderCtrl *pButtonWnd = GetHeaderCtrl( );
   if ( pButtonWnd && mIs_hWnd( pButtonWnd->m_hWnd ) )
   {
      ::SetWindowLong( pButtonWnd->m_hWnd, GWL_USERDATA, (zLONG) this );
      m_lpfnHeaderButtonWndProc =
            (WNDPROC) ::SetWindowLong( pButtonWnd->m_hWnd, GWL_WNDPROC,
                                       (DWORD) SubClassHeaderButton );
   }
#endif

   CWnd *pWnd = GetWindow( GW_CHILD );
   if ( pWnd )
   {
      m_hWndChild = pWnd->m_hWnd;
      if ( m_bAutoSort && m_hWndChild )
      {
         // Item 0 is the Header Control.
         m_SortedHeaderCtrl.SubclassWindow( m_hWndChild );
      }
   }

   return( nRC );
}

BOOL
ZListCtrl::DestroyWindow( )
{
#ifdef DEBUG_ALL
   if ( zstrstr( *m_pzsTag, "RosterList" ) )
   {
      TraceLine( "ZListCtrl::DestroyWindow: Dlg.Wnd.Ctrl %s.%s.%s",
                 *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), *m_pzsTag );
   }
#endif
   zCHAR szBuffer[ 40 ]; // tag length + 7

   //
   // Remove the subclass for the Button control.
   //
   if ( m_lpfnHeaderButtonWndProc )
   {
      CWnd *pButtonWnd = GetWindow( GW_CHILD );
      SetWindowLong( pButtonWnd->m_hWnd, GWL_WNDPROC, (DWORD) m_lpfnHeaderButtonWndProc );
      m_lpfnHeaderButtonWndProc = 0;
   }

   m_lEntityCnt = 0;
   if ( GetViewByName( &m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }

   if ( m_pRowColColorList )
   {
      ZNumberItem *pNumberItem = (ZNumberItem *) m_pRowColColorList->DropHead( );
      ZPairedList *pPairedList;
      ZPairedItem *pPairedItem;
      ZColorItem  *pColorItem;

      while ( pNumberItem )
      {
         pPairedList = (ZPairedList *) pNumberItem->m_lNumber;
         pPairedItem = pPairedList->DropHead( );
         while ( pPairedItem )
         {
            pColorItem = (ZColorItem *) pPairedItem->m_lValue2;
            delete( pPairedItem );
            pPairedItem = pPairedList->DropHead( );
         }

         delete( pPairedList );
         delete( pNumberItem );
         pNumberItem = (ZNumberItem *) m_pRowColColorList->DropHead( );
      }

      delete( m_pRowColColorList );
      m_pRowColColorList = 0;
   }

   zLONG  lMaxLth = sizeof("-2147483647,") * (m_nCols + 1);
   zPCHAR pch = new char[ lMaxLth ];
   zSHORT nLth = 0;
   zSHORT k;

   for ( k = 0; k < m_nCols; k++ )
   {
      _ltoa_s( GetColumnWidth( k ), pch + nLth, lMaxLth - nLth, 10 );
      nLth = (zSHORT) zstrlen( pch );
      if ( k < m_nCols - 1 )
      {
         pch[ nLth ] = ',';
         nLth++;
      }
   }

   pch[ nLth ] = 0;
   strcpy_s( szBuffer, sizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, sizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );
   if ( m_bRememberSort )
   {
      strcpy_s( szBuffer, sizeof( szBuffer ), "Sort:" );
      strcpy_s( szBuffer + 5, sizeof( szBuffer ) - 5, *m_pzsTag );

      // Set the "check bytes" at end of string.
      m_csSortBuffer += _T( "##" );
      SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, m_csSortBuffer );
      m_csSortBuffer.TrimRight( '#' );
   }

   mDeleteInitA( pch );
   if ( mIs_hWnd( m_wndListTip.m_hWnd ) )
      m_wndListTip.DestroyWindow( );

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CListCtrl::DestroyWindow( ) );
}

void
ZListCtrl::SetColorAttribute( zCPCHAR cpcAttributeName, zSHORT nCol )
{
   mDeleteInitA( m_pCol[ nCol ].pchColorAttribute );
   zLONG lLth = zstrlen( cpcAttributeName ) + 1;
   m_pCol[ nCol ].pchColorAttribute = new char[ lLth ];
   strcpy_s( m_pCol[ nCol ].pchColorAttribute, lLth, cpcAttributeName );
}

void
ZListCtrl::SetColorAssociation( zLONG   lColumn,
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
ZListCtrl::RemoveColorAssociations( )
{
   if ( mIs_hWnd( m_hWnd ) )
   {
      zLONG lRow;
      for ( lRow = 0; lRow < m_lEntityCnt; lRow++ )
         SetItemData( lRow, 0 );
   }

   for ( zSHORT nCol = 0; nCol <= m_nColsMax; nCol++ )
      mDeleteInitA( m_pCol[ nCol ].pchColorAttribute );

   if ( m_pRowColColorList )
   {
      ZNumberItem *pNumberItem = (ZNumberItem *) m_pRowColColorList->DropHead( );
      ZPairedList *pPairedList;
      ZPairedItem *pPairedItem;
      ZColorItem  *pColorItem;

      while ( pNumberItem )
      {
         pPairedList = (ZPairedList *) pNumberItem->m_lNumber;
         pPairedItem = pPairedList->DropHead( );
         while ( pPairedItem )
         {
            pColorItem = (ZColorItem *) pPairedItem->m_lValue2;
            delete( pPairedItem );
            pPairedItem = pPairedList->DropHead( );
         }

         delete( pPairedList );
         delete( pNumberItem );
         pNumberItem = (ZNumberItem *) m_pRowColColorList->DropHead( );
      }

      delete( m_pRowColColorList );
      m_pRowColColorList = 0;
   }

   mDeleteInit( m_pColorList );
}

void
ZListCtrl::SetDisplaySelSet( zSHORT nSelSet )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::SetDisplaySelSet ", nSelSet );
#endif
   if ( nSelSet == m_nSelectSelSet )
   {
      TraceLineI( "ZListCtrl::SetDisplaySelSet warning - setting Display select set to Select select set", nSelSet );
   }

   m_nDisplaySelSet = nSelSet;
   if ( m_nDisplaySelSet )
      m_ulFlag |= zQUAL_SELECTED;
   else
      m_ulFlag &= ~zQUAL_SELECTED;
}

void
ZListCtrl::SetSelectSelSet( zSHORT nSelSet )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::SetSelectSet", nSelSet );
#endif
   if ( nSelSet == m_nDisplaySelSet )
   {
      TraceLineI( "ZListCtrl::SetDisplaySelSet warning - setting Display select set to Select select set", nSelSet );
   }

   m_nSelectSelSet = nSelSet;
}

void
ZListCtrl::SetSelectedPosition( zLONG lDisplayPos )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineI( "ZListCtrl::SetSelectedPosition", lDisplayPos );
#endif

#if defined( _MSC_VER ) && _MSC_VER > 1100

   if ( lDisplayPos == -1 )
   {
      zLONG  lRowCnt = m_EntityKeyList.GetSize( );
      zLONG  lRow;
      zULONG ulKey = GetEntityKey( m_vApp, *m_pzsEName );

      for ( lRow = 0; lRow < lRowCnt; lRow++ )
      {
         if ( ulKey == (zULONG) m_EntityKeyList.GetAt( lRow ) )
         {
            lDisplayPos = lRow + 1;
            break;
         }
      }
   }

   POSITION pos = GetFirstSelectedItemPosition( );
   if ( pos )
   {
      zLONG lCurrSel = GetNextSelectedItem( pos ) + 1;
      if ( m_nCntPerPage > lDisplayPos )
      {
         if ( lCurrSel > lDisplayPos )
            EnsureVisible( lCurrSel - lDisplayPos, FALSE );
         else
            EnsureVisible( 0, FALSE );

         if ( lCurrSel + m_nCntPerPage < m_lEntityCnt )
            EnsureVisible( lCurrSel + m_nCntPerPage - lDisplayPos, FALSE );
      }

      EnsureVisible( lCurrSel, FALSE );
      Update( lCurrSel );
   }
#endif
}

zLONG
ZListCtrl::SetZCtrlProperty( zLONG   lPropertyType,
                             zULONG  ulNumericProperty,
                             zCPCHAR cpcStringProperty )
{
   if ( lPropertyType == zCONTROL_PROPERTY_SUBCTRL_JUSTIFY )
   {
      zSHORT nCol = 1;
      while ( nCol <= m_nCols )
      {
         if ( m_pCol[ nCol ].pchTag &&
              zstrcmp( m_pCol[ nCol ].pchTag, cpcStringProperty ) == 0 )
         {
            if ( ulNumericProperty == 1 )
               m_pCol[ nCol ].nJustify = LVCFMT_RIGHT;   // right justify
            else
            if ( ulNumericProperty == 2 )
               m_pCol[ nCol ].nJustify = LVCFMT_CENTER;  // center justify
            else
               m_pCol[ nCol ].nJustify = LVCFMT_LEFT;    // left justify

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
      zSHORT nCol = 1;
      while ( nCol <= m_nCols )
      {
         if ( m_pCol[ nCol ].pchTag &&
              zstrcmp( m_pCol[ nCol ].pchTag, cpcStringProperty ) == 0 )
         {
            LV_COLUMN lvc;

            m_pCol[ nCol ].nExtent = (zSHORT) ulNumericProperty;
            lvc.mask = LVCF_WIDTH;
            lvc.iSubItem = nCol - 1;
            lvc.cx = m_pCol[ nCol ].nExtent;
            SetColumn( nCol - 1, &lvc );
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
         zLONG lLth = zstrlen( cpcStringProperty ) + 1;
         m_pCol[ nCol ].pchTag = new char[ lLth ];
         strcpy_s( m_pCol[ nCol ].pchTag, lLth, cpcStringProperty );
         return( 0 );
      }
   }

   return( ZMapAct::SetZCtrlProperty( lPropertyType, ulNumericProperty, cpcStringProperty ) );
}

zLONG
ZListCtrl::GetZCtrlProperty( zLONG   lPropertyType,
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

   return( ZMapAct::GetZCtrlProperty( lPropertyType, pulNumericProperty, pchStringProperty, lMaxLth ) );
}

zLONG
ZListCtrl::SetZCtrlState( zLONG  lStatusType,
                          zULONG ulValue )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "TableListBox" ) == 0 )
   {
      TraceLineS( "ZListCtrl::SetZCtrlState ", *m_pzsTag );
      TraceLineS( "ZListCtrl::SetZCtrlState: ", *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZListCtrl::SetZCtrlState: ", m_pCol[ 1 ].pchAttrib );
   }
#endif

   zLONG lRC = 0;

   // We do not want the listbox to be disabled other than to ensure that
   // if the LVS_EDITLABELS style is set, to turn it off when disabling
   // the control.

   if ( lStatusType == zCONTROL_STATUS_ENABLED ||
        lStatusType == zCONTROL_STATUS_AUTOGRAY )
   {
      // Shut off auto enabled flag ... user is overriding enabled status.
      if ( lStatusType == zCONTROL_STATUS_ENABLED )
      {
         m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
         if ( ulValue )
            m_ulMapActFlags |= zMAPACT_ENABLED;
         else
            m_ulMapActFlags &= ~zMAPACT_ENABLED;
      }
      else
   // if ( lStatusType == zCONTROL_STATUS_AUTOGRAY )
      {
         if ( ulValue )
         {
            // We can't auto-enable a control that was manually disabled.
            if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
            {
               m_ulMapActFlags |= zMAPACT_ENABLED;
               m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
            }
            else
               ulValue = 0;
         }
         else
         {
            // We can't auto-disable a control that was manually disabled.
            if ( m_ulMapActFlags & zMAPACT_ENABLED )
            {
               m_ulMapActFlags &= ~zMAPACT_ENABLED;
               m_ulMapActFlags |= zMAPACT_AUTO_DISABLED;
            }
            else
               ulValue = 1;
         }
      }

#ifdef zREMOTE_SERVER
      zBOOL b = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;
      SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", b ? "Y" : "N" );
#else
      if ( mIs_hWnd( m_hWnd ) )
      {
         if ( m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
         {
            if ( m_bLabelEdit )
            {
               if ( ulValue )
                  ModifyStyle( 0L, LVS_EDITLABELS );  // add style bit
               else
                  ModifyStyle( LVS_EDITLABELS, 0L );  // remove style bit
            }
         }
         else
         {
            EnableWindow( m_ulMapActFlags & zMAPACT_ENABLED );
         }
      }
#endif
   }
   else
   if ( lStatusType == zCONTROL_STATUS_REPAINT )
   {
      if ( mIs_hWnd( m_hWnd ) )
      {
         // If we don't have a list view ... punt and repaint the entire ctrl.
         if ( ulValue == 0 || m_vAppList == 0 || m_pchListInfo == 0 || MiValidView( m_vAppList ) == 0 )
         {
            Invalidate( );
         }
         else
         if ( ulValue == 2 )
         {
            zLONG lTop = GetTopIndex( );
            MapFromOI( );

            if ( lTop + m_nCntPerPage < m_lEntityCnt )
            {
               EnsureVisible( m_nCntPerPage + lTop, FALSE );
               EnsureVisible( lTop, FALSE );
            }
            else
            {
               EnsureVisible( m_lEntityCnt - m_nCntPerPage, FALSE );
               EnsureVisible( m_lEntityCnt - 1, FALSE );
            }
         }
         else
         {
            CRect rect;
         // zCHAR  szMsg[ 128 ];
         // zLONG lTop = GetTopIndex( );
            zLONG lEntityNbr = GetRelativeEntityNumber( m_vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, m_ulFlag );
            GetItemRect( lEntityNbr, rect, LVIR_BOUNDS );
         // sprintf_s( szMsg, sizeof( szMsg ), "REPAINT top(%ld) item %ld:", lTop, lEntityNbr );
         // TraceRect( szMsg, rect );
            InvalidateRect( rect );
         }
      }
   }
   else
      lRC = ZMapAct::SetZCtrlState( lStatusType, ulValue );

   return( lRC );
}

#ifdef DEBUG_ALL
void
DisplayEntityKey( zCPCHAR cpcText, zVIEW vApp, zVIEW vAppList,
                  zLPLB_COL pCol )
{
   zCHAR  szMsg[ 2 * LBH_TEXT_LTH ];
   zCHAR  szText[ LBH_TEXT_LTH ];
   zULONG ulKey;

   if ( vApp )
   {
      ulKey = GetEntityKey( vApp, pCol[ 1 ].pchEntity );
      GetVariableFromAttribute( szText, 0, zTYPE_STRING,
                                LBH_TEXT_LTH, // nMaxLth - 1 ... bombs when > 255
                                vApp,
                                pCol[ 1 ].pchEntity,
                                pCol[ 1 ].pchAttrib,
                                pCol[ 1 ].pchContext,
                                pCol[ 1 ].pchContext ? zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
      sprintf_s( szMsg, sizeof( szMsg ), " App: %6d %s", ulKey, szText );
      TraceLineS( cpcText, szMsg );
   }

   if ( vAppList )
   {
      ulKey = GetEntityKey( vAppList, pCol[ 1 ].pchEntity );
      GetVariableFromAttribute( szText, 0, zTYPE_STRING,
                                LBH_TEXT_LTH, // nMaxLth - 1 ... bombs when > 255
                                vAppList,
                                pCol[ 1 ].pchEntity,
                                pCol[ 1 ].pchAttrib,
                                pCol[ 1 ].pchContext,
                                pCol[ 1 ].pchContext ? zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
      sprintf_s( szMsg, sizeof( szMsg ), " AppList: %6d %s", ulKey, szText );
      TraceLineS( cpcText, szMsg );
   }
}
#else
   #define DisplayEntityKey( cpcText, vApp, vAppList, pCol )
#endif

// Load pchText with the value to put in the listbox.
zSHORT
ZListCtrl::FormatTextAtPosition( zPCHAR pchText,
                                 zLONG  lRow,
                                 zSHORT nCol,
                                 zSHORT nMaxLth )
{
   nCol++;  // compensate for 1-based index into m_pCol table
   pchText[ 0 ] = 0; // initialize to null string in case there are problems

   if ( nCol == 2 )
      TraceLineI( "FormatTextAtPosition Col: 2   MaxLth: ", nMaxLth );

   // If we don't have list info or we are in a delete state ... quit.
   if ( m_pchListInfo == 0 || (ZSubtask::GetSubtaskState( m_pZSubtask ) & zSUBTASK_STATE_DELETED) )
   {
      mDeleteInitA( m_pchListInfo );
      return( -1 );
   }

   if ( m_lEntityCnt > 0 )
   {
      if ( MiValidView( m_vAppList ) == 0 || CheckExistenceOfEntity( m_vAppList, *m_pzsEName ) != 0 ) // no data to display
      {
         TraceLine( "ZListCtrl::FormatTextAtPosition for Control: %s  Row: %d  Col: %d  "
                      "Count: %d  has no data to display ... a refresh of this control may be required",
                    *m_pzsTag, lRow, nCol, m_lEntityCnt );
         m_lEntityCnt = 0;
      }
   }

   if ( m_lEntityCnt <= 0 )  // no data to display
      return( 0 );

// DisplayCellData( "ZGrid::FormatTextForCell1" );

   if ( GetViewByName( &m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
#ifdef DEBUG_ALL
      if ( zstrcmp( m_pzsTag->GetString(), "ER_AttributeList" ) == 0 )
      {
         TraceLine( "ZListCtrl::FormatTextAtPosition ER_AttributeList: %s  Attribute: %s",
                     *(m_pZSubtask->m_pzsWndTag), m_pCol[ nCol ].pchAttrib );
      }
#endif

#if 0
      m_nRetryMapFromOI++;
      if ( m_nRetryMapFromOI <= 1 &&
           (m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) == 0 )
      {
         zCHAR szMsg[ 2 * LBH_TEXT_LTH ];

         sprintf_s( szMsg, sizeof( szMsg ), "Unable to load view: %s for %s.%s.%s",
                   *m_pzsVName, *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), *m_pzsTag );
         TraceLineS( "ZListCtrl::FormatTextAtPosition Retry MapFromOI: ", szMsg );

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
            zCHAR szMsg[ 2 * LBH_TEXT_LTH ];

            sprintf_s( szMsg, sizeof( szMsg ), "Unable to load view: %s for %s.%s.%s",
                      (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                      (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString() );
            TraceLineS( "ZListCtrl::FormatTextAtPosition ", szMsg );
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
      // m_ulEntityKey = GetEntityKey( m_vApp, *m_pzsEName );
      // SetEntityCursor( m_vAppList, *m_pzsEName, 0,
      //                  m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
      //                  (zCPVOID) &m_ulEntityKey,
      //                  0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
      }
   }

   // Ensure at least one entity.
   if ( lRow >= m_EntityKeyList.GetSize( ) ||
        SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                         0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) < 0 ) // m_pzsAName
   {
      if ( m_nRetryMapFromOI < 2 && m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
      {
         zCHAR szMsg[ 2 * LBH_TEXT_LTH ];

         sprintf_s( szMsg, sizeof( szMsg ), "Error in entity count for view: %s  entity: %s for %s.%s.%s",
                   (*m_pzsVName).GetString(), (*m_pzsEName).GetString(), (*m_pZSubtask->m_pzsDlgTag).GetString(),
                   (*m_pZSubtask->m_pzsWndTag).GetString(), (*m_pzsTag).GetString() );
         TraceLineS( "ZListCtrl::FormatTextAtPosition ", szMsg );
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
      SetCursorNextEntity( m_vAppList, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );  // m_pzsAName
      ulEntityKey = GetEntityKey( m_vAppList, *m_pzsEName );
   }

   m_lEntityNbr = lRow;
   if ( ulKey != ulEntityKey &&
        SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                         (zCPVOID) &ulKey, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) < zCURSOR_SET ) // m_pzsAName
   {
      if ( m_nRetryMapFromOI < 2 && m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
      {
         zCHAR szMsg[ 2 * LBH_TEXT_LTH ];

         sprintf_s( szMsg, sizeof( szMsg ), "Entity Key Error: %s  Entity: %s  row: %d for %s.%s.%s",
                   (*m_pzsVName).GetString(), (*m_pzsEName).GetString(), lRow + 1, (*m_pZSubtask->m_pzsDlgTag).GetString(),
                   (*m_pZSubtask->m_pzsWndTag).GetString(), (*m_pzsTag).GetString() );
         TraceLineS( "ZListCtrl::FormatTextAtPosition ", szMsg );
      }

      m_lEntityNbr = -1;
      m_nRetryMapFromOI = 2;
      return( -1 );
   }

   m_nRetryMapFromOI = 0;

   // This is the attribute to present to the list.
   zSHORT nRC;

   if ( m_pCol[ nCol ].pchAttrib == 0 || m_pCol[ nCol ].pchAttrib[ 0 ] == 0 )
      nRC = zVAR_NULL;
   else
   {
      nRC = GetVariableFromAttribute( pchText, 0, zTYPE_STRING,
                                      nMaxLth,  // nMaxLth - 1 ... bombs when > 255
                                      m_vAppList,
                                      m_pCol[ nCol ].pchEntity,
                                      m_pCol[ nCol ].pchAttrib,
                                      m_pCol[ nCol ].pchContext,
                                      m_pCol[ nCol ].pchContext ? zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
   }

#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
// if ( m_bSortDisplay )
   {
      zCHAR szMsg[ 2 * LBH_TEXT_LTH ];

      sprintf_s( szMsg, sizeof( szMsg ), "ZListCtrl::FormatTextAtPosition Tag: %s EntityNbr: %d  Row: %ld  Col: %d  MaxLth: %d  VN: %s   DlgTag: %s  WndTag: %s - ",
                m_pzsTag->GetString(), m_lEntityNbr, lRow, nCol, nMaxLth, m_pzsVName->GetString(),
                m_pZSubtask->m_pzsDlgTag->GetString(), m_pZSubtask->m_pzsWndTag->GetString() );
      TraceLineS( szMsg, pchText );
   }
#endif

   if ( nRC == zVAR_NULL )
      pchText[ 0 ] = 0;
   else
   if ( nCol > 1 || m_bLabelEdit == FALSE )
   {
      zPCHAR  pchPos;

      // Replace any CR or LF with spaces.
      while ( (pchPos = zstrchr( pchText, '\n' )) != 0 )
      {
         *pchPos = ' ';
      }

      while ( (pchPos = zstrchr( pchText, '\r' )) != 0 )
      {
         *pchPos = ' ';
      }
   }

   if ( m_pColorList &&
        (m_pCol[ nCol ].pchColorAttribute || m_pCol[ 0 ].pchColorAttribute) )
   {
      if ( m_pRowColColorList == 0 )
         m_pRowColColorList = new ZNumberList;

      ZPairedList *pPairedList;
      ZColorItem *pColorItem = 0;
      DWORD dw = GetItemData( lRow );
      if ( dw == 0 || dw == (DWORD) -1 )
      {
         pPairedList = new ZPairedList;
         m_pRowColColorList->AddTail( (zLONG) pPairedList );
         SetItemData( lRow, (DWORD) pPairedList );
      }
      else
      {
         pPairedList = (ZPairedList *) dw;
      }

      if ( pPairedList->Find( nCol, 0, 0, 0, 1 ) == 0 )
      {
         zCHAR szValue[ 2 * LBH_TEXT_LTH ];

         GetStringFromAttribute( szValue, sizeof( szValue ), m_vAppList, *m_pzsEName,
                                 m_pCol[ nCol ].pchColorAttribute ? m_pCol[ nCol ].pchColorAttribute : m_pCol[ 0 ].pchColorAttribute );

         // Use the setting for the column (if it exists).  Otherwise use the setting for the whole row if we can find it.
         zSHORT k;
         pColorItem = m_pColorList->Find( nCol, szValue, TRUE );
         if ( pColorItem )
            k = nCol;
         else
         {
            k = 0;
            pColorItem = m_pColorList->Find( 0, szValue, TRUE );
         }

         if ( pColorItem )
            pPairedList->Add( k, (zLONG) pColorItem );
      }
   }

   TranslateValue( pchText, nMaxLth, TRUE, nCol - 1 );

#ifdef DEBUG_ALL
// if ( zstrcmp( "CurrentShipments", *m_pzsTag ) == 0 )
   if ( zstrcmp( m_pzsTag->GetString(), "DKS" ) == 0 )
      TraceLineS( "ZListCtrl::FormatText Text: ", pchText );
#endif

   return( 0 );
}

zSHORT OPERATION
fnCompareListData( CArray<zLONG, zLONG> *pca,
                   zLONG  lLeft,
                   zLONG  lLargest,
                   zPVOID pvSortData )
{
   ZListCtrl *pListCtrl = (ZListCtrl *) pvSortData;
   zLONG lKey1 = pca->GetAt( lLeft - 1 );
   zLONG lKey2 = pca->GetAt( lLargest - 1 );

   if ( lKey1 == pListCtrl->m_lKey2 || lKey2 == pListCtrl->m_lKey1 )
   {
      zVIEW vTemp = pListCtrl->m_vApp1;
      pListCtrl->m_vApp1 = pListCtrl->m_vApp2;
      pListCtrl->m_vApp2 = vTemp;
   }

   if ( lKey1 != pListCtrl->m_lKey1 )
   {
      pListCtrl->m_lKey1 = lKey1;
      if ( SetEntityCursor( pListCtrl->m_vApp1, *(pListCtrl->m_pzsEName), 0,
                            pListCtrl->m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &lKey1, 0, 0, 0, *(pListCtrl->m_pzsScope), 0 ) < 0 )
      {
         pListCtrl->m_lKey1 = -1;
      }
   }

   if ( lKey2 != pListCtrl->m_lKey2 )
   {
      pListCtrl->m_lKey2 = lKey2;
      if ( SetEntityCursor( pListCtrl->m_vApp2, *(pListCtrl->m_pzsEName), 0,
                            pListCtrl->m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &lKey2, 0, 0, 0, *(pListCtrl->m_pzsScope), 0 ) < 0 )
      {
         pListCtrl->m_lKey2 = -1;
      }
   }

   if ( pListCtrl->m_lKey1 == pListCtrl->m_lKey2 )
      return( 0 );
   else
   if ( pListCtrl->m_lKey1 < 0 )
      return( -1 );
   else
   if ( pListCtrl->m_lKey2 < 0 )
      return( 1 );

   zCHAR  szText1[ LBH_TEXT_LTH ];
   zCHAR  szText2[ LBH_TEXT_LTH ];
   zLONG  lCol;
   zBOOL  bAscend;
   zSHORT nRC = 0;

   ZNumberItem *pItem = pListCtrl->m_pSortColumnList->m_pHeadNumber;
   while ( pItem )
   {
      lCol = pItem->m_lNumber;
      if ( lCol < 0 )
      {
         lCol = -lCol;
         bAscend = FALSE;
      }
      else
         bAscend = TRUE;

      lCol--;
      nRC =
      GetVariableFromAttribute( szText1, 0, zTYPE_STRING,
                                sizeof( szText1 ), // ... bombs if > 255
                                pListCtrl->m_vApp1,
                                pListCtrl->m_pCol[ lCol ].pchEntity,
                                pListCtrl->m_pCol[ lCol ].pchAttrib,
                                pListCtrl->m_pCol[ lCol ].pchContext,
                                pListCtrl->m_pCol[ lCol ].pchContext ? zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
      nRC =
      GetVariableFromAttribute( szText2, 0, zTYPE_STRING,
                                sizeof( szText2 ), // ... bombs if > 255
                                pListCtrl->m_vApp2,
                                pListCtrl->m_pCol[ lCol ].pchEntity,
                                pListCtrl->m_pCol[ lCol ].pchAttrib,
                                pListCtrl->m_pCol[ lCol ].pchContext,
                                pListCtrl->m_pCol[ lCol ].pchContext ? zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

      nRC = zstrcmp( szText1, szText2 );
      if ( nRC )
      {
         if ( bAscend == FALSE )
            nRC = -nRC;

         break;
      }

      pItem = pItem->m_pNextNumber;
   }

   return( nRC );
}

void
ZListCtrl::SortByDisplayData( )
{
   if ( m_vAppList == 0 || m_lEntityCnt == 0 || m_csSortBuffer.IsEmpty( ) )
      return;

   m_lKey1 = -1;
   m_lKey2 = -1;
   CreateViewFromViewForTask( &m_vApp1, m_vAppList, m_pZSubtask->m_vDialog );
   CreateViewFromViewForTask( &m_vApp2, m_vAppList, m_pZSubtask->m_vDialog );
   m_pSortColumnList = new ZNumberList;

   zLONG  lLth = m_csSortBuffer.GetLength( ) + 1;
   zPCHAR pchSortBuffer = new char[ lLth ];
   zPCHAR pchEntity;
   zPCHAR pchAttrib;
   zPCHAR pchContext;
   zPCHAR pchEnd;
   zBOOL  bAscend;
   zSHORT k;

   // TRANSACTION.State A [STATE]
   strcpy_s( pchSortBuffer, lLth, m_csSortBuffer );
   pchEntity = pchSortBuffer;
   while ( *pchEntity )
   {
      while ( *pchEntity && isspace( *pchEntity ) )
         pchEntity++;

      if ( *pchEntity == 0 )
         break;

      pchAttrib = zstrchr( pchEntity, '.' );
      if ( pchAttrib )
      {
         *pchAttrib = 0;
         pchAttrib++;
      }
      else
      {
         TraceLineS( "ZListCtrl::SortByDisplayData Unexpected Entity/Attribute condition: ", pchEntity );
         break;
      }

      pchEnd = pchAttrib;
      while ( *pchEnd && isspace( *pchEnd ) == FALSE )
         pchEnd++;

      while ( *pchEnd && isspace( *pchEnd ) )
      {
         *pchEnd = 0;
         pchEnd++;
      }

      if ( *pchEnd == 'A' )
         bAscend = TRUE;
      else
      if ( *pchEnd == 'D' )
         bAscend = FALSE;
      else
      {
         TraceLineS( "ZListCtrl::SortByDisplayData Unexpected Ascend/Descend condition: ", pchEnd );
         break;
      }

      pchEnd++;
      while ( *pchEnd && isspace( *pchEnd ) )
      {
         *pchEnd = 0;
         pchEnd++;
      }

      if ( *pchEnd == '[' )
      {
         pchContext = pchEnd + 1;
         pchEnd = zstrchr( pchEnd, ']' );
         if ( pchEnd )
         {
            *pchEnd = 0;
            pchEnd++;
         }
         else
         {
            TraceLineS( "ZListCtrl::SortByDisplayData Unexpected Scope condition: ", pchEntity );
            pchContext = 0;
            break;
         }
      }
      else
         pchContext = 0;

      for ( k = 1; k <= m_nCols; k++ )
      {
         if ( zstrcmp( pchEntity, m_pCol[ k ].pchEntity ) == 0 &&
              zstrcmp( pchAttrib, m_pCol[ k ].pchAttrib ) == 0 &&
              (pchContext == 0 || zstrcmp( pchContext, m_pCol[ k ].pchContext ) == 0) )
         {
            if ( bAscend )
               m_pSortColumnList->AddTail( k );
            else
               m_pSortColumnList->AddTail( -k );

            break;
         }
      }

      if ( k > m_nCols )
      {
         TraceLineS( "ZListCtrl::SortByDisplayData Unexpected Entity name: ", pchEntity );
         break;
      }

      pchEntity = pchEnd;
   }

   delete [] pchSortBuffer;

#ifdef DEBUG_ALL
   zLONG  lKey;
   zSHORT nLth;
   zCHAR  szMsg[ 512 ];

   TraceLineI( "HeapSort EntityKeyList count: ", m_lEntityCnt );
   for ( k = 0; k < m_lEntityCnt; k++ )
   {
      lKey = m_EntityKeyList.GetAt( k );
      sprintf_s( szMsg, sizeof( szMsg ), " %ld - %ld: ", k, lKey );
      nLth = (zSHORT) zstrlen( szMsg );
      if ( SetEntityCursor( m_vApp1, m_pzsEName->GetString(), 0,
                            m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &lKey, 0, 0, 0,
                            m_pzsScope ? m_pzsScope->GetString() : (zCPCHAR) 0, 0 ) >= 0 ) // m_pzsAName
      {
         GetVariableFromAttribute( szMsg + nLth, 0, zTYPE_STRING,
                                   sizeof( szMsg ) - nLth,
                                   m_vApp1,
                                   m_pCol[ 1 ].pchEntity,
                                   m_pCol[ 1 ].pchAttrib,
                                   m_pCol[ 1 ].pchContext,
                                   m_pCol[ 1 ].pchContext ?
                                    zACCEPT_NULL_ENTITY :
                                    zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
      }

      TraceLineS( szMsg, "" );
   }
#endif

   HeapSortArray( (CArray<void *, void *> *) &m_EntityKeyList,
                  (zFARPROC_COMPARE) fnCompareListData, m_lEntityCnt, this );

#ifdef DEBUG_ALL
   TraceLineS( "HeapSort EntityKeyList (after): ", m_csSortBuffer );
   for ( k = 0; k < m_lEntityCnt; k++ )
   {
      lKey = m_EntityKeyList.GetAt( k );
      sprintf_s( szMsg, sizeof( szMsg ), " %ld - %ld: ", k, lKey );
      nLth = (zSHORT) zstrlen( szMsg );
      if ( SetEntityCursor( m_vApp1, *m_pzsEName, 0,
                            m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &lKey, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) >= 0 ) // m_pzsAName
      {
         GetVariableFromAttribute( szMsg + nLth, 0, zTYPE_STRING,
                                   sizeof( szMsg ) - nLth,
                                   m_vApp1,
                                   m_pCol[ 1 ].pchEntity,
                                   m_pCol[ 1 ].pchAttrib,
                                   m_pCol[ 1 ].pchContext,
                                   m_pCol[ 1 ].pchContext ?
                                    zACCEPT_NULL_ENTITY :
                                    zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
      }

      TraceLineS( szMsg, "" );
   }
#endif

   DropView( m_vApp1 );
   DropView( m_vApp2 );
   mDeleteInit( m_pSortColumnList );
}

zSHORT
ZListCtrl::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "OperationList" ) == 0 )
   {
      TraceLineS( "ZListCtrl::MapFromOI ", *m_pzsTag );
      TraceLineS( "ZListCtrl::MapFromOI: ", *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZListCtrl::MapFromOI: ", m_pCol[ 1 ].pchAttrib );
      TraceLineS( "ZListCtrl::MapFromOI ViewName ", *m_pzsVName );
   }

// if ( zstrcmp( *(m_pZSubtask->m_pzsWndTag), "Invoice" ) == 0 )
//    MessageBox( "ZListCtrl::MapFromOI Begin", *m_pzsTag );
#endif

   m_lEntityNbr = -1;
   m_lLastSelected = -1;

   if ( IsVisibleForMapFromOI( ) == FALSE || // if ctrl not Visible, do not map
        m_pchListInfo == 0 )  // if there is no list info, do not map
   {
      return( 0 );
   }

   SetRedraw( FALSE );

// zBOOL bLock = LockWindowUpdate( );  causes repaint of entire desktop
   zBOOL bBubbleSort = (m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) ? TRUE : FALSE;

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1 | zMAPACT_MAPPING_FROM_OI;
   zLONG lTop = GetTopIndex( );
   if ( GetViewByName( &m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }
// else
//    TraceLineS( "ZListCtrl::MapFromOI m_vAppList null ", *m_pzsTag );

   // Auto-enable if this control was auto-disabled.
   if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
   {
      m_ulMapActFlags |= zMAPACT_ENABLED;
      m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
      EnableWindow( TRUE );
   }

   m_lEntityCnt = 0;
   DeleteAllItems( );
   if ( (m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) )
      m_ulMapActFlags |= zMAPACT_AUTO_NOFOCUS;

   // If we cannot get the view ... quit.
   if ( m_pzsVName == 0 || **m_pzsVName == 0 || GetViewByName( &m_vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
      {
         TraceLine( "ZListCtrl::MapFromOI Unable to load view: %s for %s.%s.%s",
                    *m_pzsVName, *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), *m_pzsTag );
      }

   // if ( bLock )
   //    UnlockWindowUpdate( );  causes repaint of entire desktop

      SetRedraw( TRUE );

      m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
   // Invalidate( TRUE );
      return( 0 );
   }

   SetNameForView( m_vApp, m_csViewName, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
   mDeleteInitA( m_pchLabelText );

#ifdef DEBUG_ALL
   if ( zstrcmp( "EntityList", m_pzsTag->GetString() ) == 0 )
   {
      TraceLineS( "ZListCtrl::MapFromOI ", *m_pzsTag );
      TraceLineS( "ZListCtrl::MapFromOI SortBuffer ", m_csSortBuffer );
   }
#endif

   if ( (wFlag & zMAP_NO_SORT) == 0 )
   {
   // TraceLine( "ZListCtrl::MapFromOI for Control: %s   Sort Buffer: %s   Remember Sort: %s",
   //            *m_pzsTag, m_csSortBuffer, m_bRememberSort ? "True" : "False" );
      if ( m_bRememberSort && m_csSortBuffer.IsEmpty( ) == FALSE )
      {
         if ( m_nColLastSorted >= 0 )
         {
            m_SortedHeaderCtrl.SetSortImage( m_nColLastSorted - 1, m_chAscendDescend == 'A' ? TRUE : FALSE );
         }

      // BubbleSort is toooo slow for anything over 1000 entities!  Plus, we don't think we need it here.
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
            OrderEntityForView( m_vApp, *m_pzsEName, m_csSortBuffer );
      }
      else
      if ( m_nColLastSorted >= 0 )
      {
         m_nColLastSorted = -1;
         m_csSortBuffer = "";
         m_SortedHeaderCtrl.SetSortImage( -1, FALSE );
      }
   }

   zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, 1 );  // just to get the
   SetSelectSetForView( m_vApp, nPrevSelSet );             // previous selset
   while ( m_ulListLth )  // purist's goto
   {
      int    iRelativeNbr = 0;  // this must be int for use in API call
      zULONG ulKey;

      if ( m_ulMapActFlags & zLISTBOX_PARENT_ENTITY )
      {
         zCHAR szScope[ zTAG_LTH ];

         mDeleteInit( m_pzsScope );
         MiGetParentEntityNameForView( szScope, m_vApp, *m_pzsEName );
         if ( szScope[ 0 ] )
            m_pzsScope = new CString( szScope );
      }

      if ( m_chListOnlyWhenSelected )
      {
         if ( m_pzsScope == 0 || m_pzsScope->IsEmpty( ) )
         {
            zCHAR szScope[ zTAG_LTH ];

            mDeleteInit( m_pzsScope );
            MiGetParentEntityNameForView( szScope, m_vApp, *m_pzsEName );
            if ( szScope[ 0 ] )
               m_pzsScope = new CString( szScope );
         }

         SetSelectSetForView( m_vApp, m_nSelectSelSet );

         // If there is no selected parent entity ... quit.
         if ( m_pzsScope == 0 || m_pzsScope->IsEmpty( ) ||
              SetEntityCursor( m_vApp, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0,  // m_pzsAName
                               0, m_ulFlag | zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN,
                               0, 0, 0, 0, 0, 0 ) < 0 )
         {
            break;  // goto end of while
         }
      }

      if ( m_nDisplaySelSet )
         SetSelectSetForView( m_vApp, m_nDisplaySelSet );

      // If there are no entities to list ... quit.  Note that this has been
      // the source of a bug prior to 10C when the Scope Entity in the
      // painter has been set to Parent Entity.
      if ( CheckEntityInView( m_vApp, *m_pzsEName ) != 0 ||
           SetEntityCursor( m_vApp, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                            0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) < 0 )
      {
         break;  // goto end of while
      }

      CreateViewFromViewForTask( &m_vAppList, m_vApp, m_pZSubtask->m_vDialog );
      SetNameForView( m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
   // SfLockView( m_vAppList );  // we lock this view because it is
                                 // unnamed ... to prevent the OE from
                                 // automatically cleaning it up during
                                 // the deletion of an OE subtask.

   // TraceLineS( "ZListCtrl::MapFromOI ObjectName ", szViewObjectName );
   // TraceLineX( "ZListCtrl::MapFromOI creating m_vAppList ", (zLONG) m_vAppList );

   // DisplayEntityKey( "ZListCtrl::MapFromOI", m_vApp, m_vAppList, m_pCol );

      // This count may be larger than the final count because of filtering
      // that may go on, but that's alright since we are just trying to
      // have the control pre-allocate sufficient memory blocks.
      m_lEntityCnt = CountEntitiesForView( m_vAppList, *m_pzsEName );
      SetItemCount( m_lEntityCnt );

      zSHORT nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                                    m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );  // m_pzsAName
      if ( nRC > zCURSOR_UNCHANGED )
      {
         for ( zSHORT nCol = 1; nCol <= m_nCols; nCol++ )
         {
            if ( m_pCol[ nCol ].pchEntity &&
                 m_pCol[ nCol ].pchEntity[ 0 ] &&
                 m_pCol[ nCol ].pchAttrib &&
                 m_pCol[ nCol ].pchAttrib[ 0 ] )
            {
               GetAttributeDisplayLength( &m_pCol[ nCol ].ulLth, m_vApp,
                                          m_pCol[ nCol ].pchEntity,
                                          m_pCol[ nCol ].pchAttrib,
                                          m_pCol[ nCol ].pchContext ?
                                             m_pCol[ nCol ].pchContext : 0 );
               if ( nCol == 1 && m_bLabelEdit )
               {
               // if ( MiGetUpdateForView( m_vApp ) <= 0 )
                  if ( MiGetUpdateForViewEntityAttr( m_vApp,
                                                     m_pCol[ nCol ].pchEntity,
                                                     m_pCol[ nCol ].pchAttrib,
                                                     0 ) <= 0 )
                  {
                     m_bLabelEdit = FALSE;
                     ModifyStyle( LVS_EDITLABELS, 0L );
                  }
                  else
                  if ( m_pCol[ 1 ].ulLth > 255 )
                  {
                     TraceLineS( "Listbox edit length restricted ", "to 255 characters" );
                     m_bLabelEdit = FALSE;
                     ModifyStyle( LVS_EDITLABELS, 0L );
                  }
                  else
                  {
                     ModifyStyle( 0L, LVS_EDITLABELS );
                     m_pchLabelText = new char[ 256 ];
                  }
               }

               if ( m_pCol[ nCol ].ulLth > 255 )
                  m_pCol[ nCol ].ulLth = 255;
            }
            else
               m_pCol[ nCol ].ulLth = 0;

#ifdef DEBUG_ALL
         // TraceLineI( *m_pzsTag, nCol );
         // TraceLineI( "nTabPos = ", m_pCol[ nCol ].nTabPos );
         // TraceLineI( "nColLth = ", m_pCol[ nCol ].nColLth );
         // TraceLineI( "nJustify = ", m_pCol[ nCol ].nJustify );
         // TraceLineI( "nExtent = ", m_pCol[ nCol ].nExtent );
         // TraceLineI( "ulLth = ", m_pCol[ nCol ].ulLth );
#endif
         }

         while ( nRC > zCURSOR_UNCHANGED )
         {
            // InsertItem returns the index of the new item if successful
            // or -1 otherwise.
            if ( InsertItem( iRelativeNbr, LPSTR_TEXTCALLBACK ) == -1 )
            {
               TraceLineI( "(lbh) Error inserting item: ", iRelativeNbr );
               break;
            }

        // if ( m_bSortDisplay )
            {
               ulKey = GetEntityKey( m_vAppList, *m_pzsEName );
               m_EntityKeyList.SetAtGrow( iRelativeNbr, ulKey );
            }

            iRelativeNbr++;
            nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                                   m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );  // m_pzsAName

         } // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities
      }

   // if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   //    TraceLineS( "", "" );
      m_lEntityCnt = iRelativeNbr;  // set correct entity count

      if ( (m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) && m_lEntityCnt > 0 )
         m_ulMapActFlags &= ~zMAPACT_AUTO_NOFOCUS;

      if ( m_bSortDisplay )
         SortByDisplayData( );

      if ( m_chHilitePosition == 'Y' && m_vApp && CheckExistenceOfEntity( m_vApp, *m_pzsEName ) == zCURSOR_SET )
      {
         SetSelectSetForView( m_vApp, m_nSelectSelSet );
         if ( (m_ulFlag & zQUAL_SCOPE_OI) && (m_pzsScope == 0 || **m_pzsScope == 0) )
         {
            SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
         }
         else
         {
            SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
         }

         SetSelectStateOfEntity( m_vApp, *m_pzsEName, TRUE );

      // DisplayEntityKey( "ZListCtrl::MapFromOI Relative", m_vApp, m_vAppList, m_pCol );

         if ( m_nDisplaySelSet )
            SetSelectSetForView( m_vApp, m_nDisplaySelSet );
         else
            SetSelectSetForView( m_vApp, nPrevSelSet );

         m_lEntityNbr = GetRelativeEntityNumber( m_vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, m_ulFlag );
         if ( m_lEntityNbr >= m_lEntityCnt )
         {
            // Position was not established on a "selected" entity.  Reset
            // position to a valid entity.
            zSHORT nSet = SetSelectSetForView( m_vApp, m_nSelectSelSet );
            SetSelectStateOfEntity( m_vApp, *m_pzsEName, FALSE );
            m_lEntityNbr = m_lEntityCnt - 1;
            SetEntityCursor( m_vApp, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                             m_lEntityNbr, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
            SetSelectStateOfEntity( m_vApp, *m_pzsEName, TRUE );
            SetSelectSetForView( m_vApp, nSet );
         }

      // if ( m_bSortDisplay )
         {
            zULONG ulKey = m_EntityKeyList.GetAt( m_lEntityNbr );
            if ( ulKey != GetEntityKey( m_vAppList, *m_pzsEName ) )
            {
               SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST, (zCPVOID) &ulKey,
                                0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
               if ( ulKey != GetEntityKey( m_vAppList, *m_pzsEName ) )
               {
                  TraceLine( "ZListCtrl::MapFromOI ... Refresh of ListBox: %s required?", *m_pzsTag );
                  MessageBox( *m_pzsTag, *m_pzsEName, MB_OK | MB_TASKMODAL );
               }
            }
         }
      // else
      // {
      //    SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
      //                     m_lEntityNbr, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
      // }

         // Get item data.
         LV_ITEM lvi;

         lvi.mask = LVIF_STATE;
         lvi.iItem = m_lEntityNbr;
         lvi.iSubItem = 0;
         lvi.stateMask = 0xFFFF;      // get all state flags
         GetItem( &lvi );

         lvi.state |= LVIS_SELECTED;
         if ( GetFocus( ) == this )
            lvi.state |= LVIS_FOCUSED;

         SetItem( &lvi );

         if ( m_nCntPerPage > 4 )
         {
            if ( m_lEntityNbr + m_nCntPerPage < m_lEntityCnt )
               EnsureVisible( m_lEntityNbr + m_nCntPerPage - 4, FALSE );
            else
               EnsureVisible( m_lEntityCnt - 2, FALSE );
         }

         EnsureVisible( m_lEntityNbr, FALSE );
         Update( m_lEntityNbr );

      // TraceLineI( "MapFromOI HilitePosition: ", m_lEntityNbr );
      }
      else
      if ( m_chMultiSelect )
      {
         lTop = HiliteAllSelectedEntities( );

#ifdef DEBUG_ALL
      // if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
            TraceLineS( "ZListCtrl::MapFromOI ", *m_pzsTag );
#endif

         if ( lTop >= 0 )
         {
            if ( m_nCntPerPage > 4 )
            {
               if ( lTop + m_nCntPerPage < m_lEntityCnt )
                  EnsureVisible( lTop + m_nCntPerPage - 4, FALSE );
               else
                  EnsureVisible( m_lEntityCnt - 2, FALSE );
            }

            m_lEntityNbr = lTop;
            EnsureVisible( m_lEntityNbr, FALSE );
            Update( m_lEntityNbr );
         }
      }
      else
      if ( lTop < m_lEntityCnt )
      {
         SetSelectSetForView( m_vApp, m_nSelectSelSet );
         if ( SetEntityCursor( m_vApp, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zQUAL_SELECTED,
                               0, 0, 0, m_lEntityNbr, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) >= 0 ) // m_pzsAName
         {
            m_lEntityNbr = GetRelativeEntityNumber( m_vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, m_ulFlag );
         }
         else
            m_lEntityNbr = lTop;

      // if ( m_bSortDisplay )
      // {
            zLONG lKey = m_EntityKeyList.GetAt( m_lEntityNbr );
            SetEntityCursor( m_vApp, *m_pzsEName, 0, m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                             (zCPVOID) &lKey, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );  // m_pzsAName
            SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                             (zCPVOID) &lKey, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );  // m_pzsAName
      // }
      // else
      // {
      //    SetEntityCursor( m_vApp, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
      //                     m_lEntityNbr, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
      //    SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
      //                     m_lEntityNbr, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
      // }

         // Get item data.
         LV_ITEM lvi;

         lvi.mask = LVIF_STATE;
         lvi.iItem = m_lEntityNbr;
         lvi.iSubItem = 0;
         lvi.stateMask = 0xFFFF;      // get all state flags
         GetItem( &lvi );

         if ( GetSelectStateOfEntity( m_vApp, *m_pzsEName ) )
            lvi.state |= LVIS_SELECTED;

         if ( GetFocus( ) == this )
            lvi.state |= LVIS_FOCUSED;

         SetItem( &lvi );
         EnsureVisible( m_lEntityNbr, FALSE );
      // Update( m_lEntityNbr );
      // TraceLineI( "MapFromOI Top: ", m_lEntityNbr );
      }
      else
      {
         SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                          m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
         m_lEntityNbr = 0;
      // TraceLineI( "MapFromOI First: ", m_lEntityNbr );
      // RedrawItems( m_lEntityNbr, m_lEntityNbr );
      }

      break;  // quit purist's goto after the first time thru
   }

   SetSelectSetForView( m_vApp, nPrevSelSet );
   m_lLastSelected = m_lEntityNbr;
   m_lEntityNbr = -1;

   // Repaint changes
// if ( bLock )
//    UnlockWindowUpdate();   causes repaint of entire desktop
   SetRedraw( TRUE );

#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::MapFromOI term ", *m_pzsTag );
#endif

// DisplayEntityKey( "ZListCtrl::MapFromOI term", m_vApp, m_vAppList, m_pCol );

   m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
// Invalidate( TRUE );
   return( 0 );
}

zSHORT
ZListCtrl::MapToOI( zLONG lFlag )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "DKS" ) == 0 )
   {
      TraceLineS( "ZListCtrl::MapToOI DKS: ", *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZListCtrl::MapToOI: ", m_pCol[ 1 ].pchAttrib );
   }
   TraceLineS( "ZListCtrl::MapToOI ", *m_pzsTag );
#endif

   if ( m_pchListInfo == 0 ||         // if there is no list info, don't map
        IsVisibleForMapToOI( ) == 0 ) // if ctrl not Visible/Enabled, don't map
                                      // note ... don't check view name here!
   {
      return( 0 );
   }

   if ( (m_ulMapActFlags & zLISTBOX_LABELEDIT) )
   {
      if ( m_lEntityNbr >= 0 &&
           m_pCol[ 1 ].pchEntity[ 0 ] && m_pCol[ 1 ].pchAttrib[ 0 ] )
      {
         CEdit *pEdit = GetEditControl( );
         if ( pEdit )
         {
            CString cs;

            pEdit->GetWindowText( cs );
            SetAttributeFromVariable( m_vApp, m_pCol[ 1 ].pchEntity,
                                      m_pCol[ 1 ].pchAttrib, cs,
                                      zTYPE_STRING, 0,
                                      m_pCol[ 1 ].pchContext,
                                      m_pCol[ 1 ].pchContext ? 0 : zUSE_DEFAULT_CONTEXT );
         }
      }

      m_ulMapActFlags &= ~zLISTBOX_LABELEDIT;
   }

   return( 0 );
}

zLONG
ZListCtrl::GetZCtrlState( zLONG lStatusType )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "DKS" ) == 0 )
   {
      TraceLineS( "ZListCtrl::GetZCtrlState DKS: ", *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZListCtrl::GetZCtrlState: ", m_pCol[ 1 ].pchAttrib );
   }
   TraceLineS( "ZListCtrl::GetZCtrlState", "" );
#endif
   if ( lStatusType == zCONTROL_STATUS_ENTER )
      return( TRUE );
   else
      return( ZMapAct::GetZCtrlState( lStatusType ) );
}

void
ZListCtrl::AddCol( zCPCHAR cpcText, zLONG lColumn )
{
   if ( lColumn > m_nColsMax )
   {
      zLPLB_COL pCol = new zLB_COL[ (lColumn + 1) ];
      zmemset( pCol, 0, sizeof( zLB_COL ) * (lColumn + 1) );
      zmemcpy( pCol, m_pCol, sizeof( zLB_COL ) * m_nColsMax );
      delete [] m_pCol;
      m_pCol = pCol;
      m_nColsMax = (zSHORT) lColumn;
   }

   // Loop adding new columns.
   LV_COLUMN lvc;

   while ( m_nCols < lColumn )
   {
      m_nCols++;
      lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
      lvc.fmt = m_pCol[ m_nCols ].nJustify;

      mDeleteInitA( m_pCol[ m_nCols ].pchText );
      if ( m_nCols == lColumn )
      {
         zLONG lLth = zstrlen(cpcText) + 1;
         m_pCol[ m_nCols ].pchText = new char[ lLth ];
         strcpy_s( m_pCol[ m_nCols ].pchText, lLth, cpcText );
      }
      else
      {
         m_pCol[ m_nCols ].pchText = new char[ 1 ];
         m_pCol[ m_nCols ].pchText[ 0 ] = 0;
      }

      lvc.pszText = (zPCHAR) m_pCol[ m_nCols ].pchText;
      lvc.iSubItem = m_nCols - 1;
      if ( m_pCol[ m_nCols ].nExtent == 0 )
         m_pCol[ m_nCols ].nExtent = 20;

      lvc.cx = m_pCol[ m_nCols ].nExtent;
      if ( InsertColumn( m_nCols - 1, &lvc ) != m_nCols - 1 )
      {
         TraceLineI( "ZListCtrl::SetRowColText InsertColumn failed: ", m_nCols );
      }
   }
}

zSHORT
ZListCtrl::SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "DKS" ) == 0 )
   {
      TraceLineS( "ZListCtrl::SetRowColText DKS: ", *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZListCtrl::SetRowColText: ", m_pCol[ 1 ].pchAttrib );
   }
   TraceLineS( "ZListCtrl::SetRowColText ", *m_pzsTag );
#endif

   if ( lColumn > m_nCols && lRow == 0 )
      AddCol( cpcText, lColumn );

   if ( lColumn <= m_nColsMax && lColumn > 0 )
   {
      if ( lRow == 0 )
      {
         mDeleteInitA( m_pCol[ lColumn ].pchText );
         zLONG lLth = zstrlen( cpcText ) + 1;
         m_pCol[ lColumn ].pchText = new char[ lLth ];
         strcpy_s( m_pCol[ lColumn ].pchText, lLth, cpcText );
      }

      // Actually set the text for the row/column.
      LV_COLUMN lvc;

#if 1

      // If LVCF_TEXT is specified in lvc.mask a buffer and a buffer length
      // must be provided prior to call GetColumn(...).  Otherwise, GetColumn
      // overwrites the call stack (TMV).

   // lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
   // lvc.cchTextMax = zstrlen( m_pCol[ lColumn ].pchText ) + 1;
      lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH;

      lvc.iSubItem = lColumn - 1;
      if ( GetColumn( lColumn - 1, &lvc ) )
      {
         lvc.pszText = m_pCol[ lColumn ].pchText;
         lvc.mask |= LVCF_TEXT;
         SetColumn( lColumn - 1, &lvc );
      }
      else
      {
         return( -1 );
      }

      if ( lColumn == m_nCols - 1 )
      {
         while ( m_nCols > 0 &&
                 (m_pCol[ lColumn ].pchText == 0 || m_pCol[ lColumn ].pchText[ 0 ] == 0) &&
                 (m_pCol[ lColumn ].pchAttrib == 0 || m_pCol[ lColumn ].pchAttrib[ 0 ] == 0) )
         {
            m_nCols--;
            if ( DeleteColumn( m_nCols ) == FALSE )
            {
               TraceLineI( "ZListCtrl::SetRowColText DeleteColumn failed: ", m_nCols );
            }

            lColumn--;
         }
      }

#else
      // not quite right!
      SetItemText( 0, lColumn + 1, m_pCol[ lColumn ].pchText );
#endif
   }

   return( 0 );
}

zSHORT
ZListCtrl::GetRowColText( zPCHAR pchText, zUSHORT uLth,
                          zLONG lRow, zLONG lColumn )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::GetRowColText ", *m_pzsTag );
#endif

   pchText[ 0 ] = 0;
   if ( lColumn <= m_nCols && lColumn > 0 )
   {
      if ( lRow == 0 )
         strncat_s( pchText, uLth, m_pCol[ lColumn ].pchText, uLth - 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  PURPOSE:    Set new Mapping for a ListBox or a ListBox column
//
//  NOTE:  If the "last" column's if text and attribute are empty, it is
//         deleted from the list of columns.
//
//  PARAMETERS: vSubtask         - The subtask view for the window containing
//                                 the control to be mapped.
//              cpcCtrlTag       - The unique name of the control.
//              cpcViewName      - new View Name to be mapped
//              cpcEntityName    - new Entity Name to be mapped
//              cpcAttributeName - new Attribute Name to be mapped
//              cpcContextName   - new Context Name to be mapped
//              lColumn          - Column for which the new mapping is to be
//                                 applied (0 ==> change listbox mapping).
//
//  RETURNS:    0 - New Mapping set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZListCtrl::SetCtrlMapping( zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                           zCPCHAR cpcAttrib, zCPCHAR cpcContext, zLONG lColumn )
{
   if ( lColumn == 0 )  // remap the listbox
   {
      ZMapAct::SetCtrlMapping( cpcViewName, cpcEntity, cpcAttrib, cpcContext, lColumn );
      if ( GetViewByName( &m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
      {
         DropView( m_vAppList );
      }
   }
   else
   {
      if ( cpcViewName )
         *m_pzsVName = cpcViewName;

      if ( lColumn > 0 && lColumn <= m_nColsMax )
      {
         mDeleteInitA( m_pCol[ lColumn ].pchEntity );
         mDeleteInitA( m_pCol[ lColumn ].pchAttrib );
         mDeleteInitA( m_pCol[ lColumn ].pchContext );
         zLONG lLth = zstrlen( cpcEntity ) + 1;
         m_pCol[ lColumn ].pchEntity = new char[ lLth ];
         strcpy_s( m_pCol[ lColumn ].pchEntity, lLth, cpcEntity );
         lLth = zstrlen( cpcAttrib ) + 1;
         m_pCol[ lColumn ].pchAttrib = new char[ lLth ];
         strcpy_s( m_pCol[ lColumn ].pchAttrib, lLth, cpcAttrib );
         if ( cpcContext && *cpcContext )
         {
            lLth = zstrlen( cpcContext ) + 1;
            m_pCol[ lColumn ].pchContext = new char[ lLth ];
            strcpy_s( m_pCol[ lColumn ].pchContext, lLth, cpcContext );
         }

         // Remove "last" column if text and attribute are empty.
         if ( lColumn >= m_nCols )
         {
            AddCol( "", lColumn );  // + 1 ??? dks 2003.07.21
         // AddCol( "", lColumn + 1 );
         }

      // if ( lColumn == m_nCols - 1 )
         if ( lColumn == m_nCols )  // - 1 ??? dks 2003.07.21
         {
            while ( m_nCols > 0 &&
                    (m_pCol[ lColumn ].pchText == 0 || m_pCol[ lColumn ].pchText[ 0 ] == 0) &&
                    (m_pCol[ lColumn ].pchAttrib == 0 || m_pCol[ lColumn ].pchAttrib[ 0 ] == 0) )
            {
               m_nCols--;
               if ( DeleteColumn( m_nCols ) == FALSE )
               {
                  TraceLineI( "ZListCtrl::SetCtrlMapping DeleteColumn failed: ", m_nCols );
               }

               lColumn--;
            }
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  PURPOSE:    Get new Mapping for a ListBox or a ListBox column
//
//  PARAMETERS: pchViewName      - Mapping View Name
//              pchEntityName    - Mapping Entity Name
//              pchAttributeName - Mapping Attribute Name
//              pchContextName   - Mapping Context Name
//              lColumn          - Column for which the new mapping is to be
//                                 retrieved (0 ==> retrieve listbox mapping).
//
//  RETURNS:    0 - Mapping returned successfully
//              1 - Invalid request (e.g.  outside range of valid columns)
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZListCtrl::GetCtrlMapping( zPCHAR pchViewName, zPCHAR pchEntity,
                           zPCHAR pchAttrib, zPCHAR pchContext,
                           zLONG lColumn )
{
   if ( lColumn == 0 )
   {
      ZMapAct::GetCtrlMapping( pchViewName, pchEntity,
                               pchAttrib, pchContext, lColumn );
   }
   else
   {
      if ( m_pzsVName )
         strcpy_s( pchViewName, zTAG_LTH, *m_pzsVName );
      else
         pchViewName[ 0 ] = 0;

      if ( lColumn > 0 && lColumn <= m_nCols )
      {
         if ( m_pCol[ lColumn ].pchEntity )
            strcpy_s( pchEntity, zTAG_LTH, m_pCol[ lColumn ].pchEntity );

         if ( m_pCol[ lColumn ].pchAttrib )
            strcpy_s( pchAttrib, zTAG_LTH, m_pCol[ lColumn ].pchAttrib );

         if ( m_pCol[ lColumn ].pchContext )
            strcpy_s( pchContext, zTAG_LTH, m_pCol[ lColumn ].pchContext );
      }
      else
      {
         pchEntity[ 0 ] = 0;
         pchAttrib[ 0 ] = 0;
         pchContext[ 0 ] = 0;
         return( 1 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This operation is used to keep the visual hilighting of the listbox
// in sync with the underlying select state of the object instance.
// This operation permits control over the hilighting of listbox
// items without the overhead of rebuilding the list from the object
// instance.  There are three methods of hilighting as follows:
//
// lType  -  >= 0   ==> hilite at given offset
//           -1     ==> hilite at current cursor position
//           -2     ==> hilite selected entities
//           -3     ==> select or deselect all items (assumes entities
//                      aready selected/deselected appropriately)
//
// There are two cases of a listbox to consider - single and multiple
// select.
//
// Single select is quite straightforward.  If bHilite is FALSE, no
// matter what, every item will be deselected.  If bHilite is TRUE,
// processing will occur as follows:
//
//    Hilite at given offset - reset the hilite to the entity at the
//                             given offset.
//    Hilite at current ...  - reset the hilite to the entity at the
//                             current cursor position.
//    Hilite selected entity - reset the hilite to the selected entity.
//
// Multiple select:
//
//    Hilite at given offset - hilite the item corresponding to the
//                             entity at the given offset.
//
//    Hilite at current ...  - hilite the item corresponding to the
//                             entity at the current cursor position.
//
//    Hilite selected entities - hilite the item(s) corresponding to
//                              the selected entity(s).  N.B.  bHilite
//                              is ignored.
//
/////////////////////////////////////////////////////////////////////////////
void
ZListCtrl::HiliteText( zBOOL bHilite,
                       zLONG lType,
                       zLONG lRange,
                       zBOOL bHiliteItem )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineI( "ZListCtrl::Hilite type ", lType );
#endif

// zVIEW  vApp;

// if ( m_pchListInfo && *m_pzsVName &&
//      GetViewByName( &vApp, m_csViewName,
//                     m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   if ( m_pchListInfo && m_vApp )
   {
      zLONG  lOffset;
      zLONG  lControl;

//x   zBOOL bLock = LockWindowUpdate( );  causes repaint of entire desktop
      SetRedraw( FALSE );
      m_bInHilite = TRUE;
      zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, m_nSelectSelSet );

      do    // purist's goto
      {
         if ( lType >= 0 )    // relative position
         {
            lControl = zPOS_FIRST | zPOS_RELATIVE | zSPARENT_KEEPCHILDREN;
            if ( m_nDisplaySelSet )
            {
               lControl |= zQUAL_SELECTED;
               SetSelectSetForView( m_vApp, m_nDisplaySelSet );
            }

            if ( SetEntityCursor( m_vApp, *m_pzsEName, 0, lControl, 0, 0, 0, lType,
                                  m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) < zCURSOR_SET )
            {
               break;  // since we could not set the cursor ... nothing to do
            }

            lOffset = lType;
         }
         else
         {
            if ( lType == -1 )   // current cursor position
            {
               if ( CheckExistenceOfEntity( m_vApp, *m_pzsEName ) < zCURSOR_SET )
               {
                  break;   // if there's no entity ... nothing to do
               }

               if ( m_nDisplaySelSet )
               {
                  SetSelectSetForView( m_vApp, m_nDisplaySelSet );
                  if ( GetSelectStateOfEntity( m_vApp, *m_pzsEName ) < 0 )
                  {
                     break;   // some kind of error ... nothing to do
                  }
               }
            }
            else
            if ( lType == -2 )   // selected entity(s)
            {
               if ( m_chMultiSelect )
               {
                  // Set all selections OFF.

                  zLONG   lItem;
                  LV_ITEM lvi;

                  for ( lItem = GetNextItem( -1, LVNI_SELECTED );
                        lItem != -1;
                        lItem = GetNextItem( lItem, LVNI_SELECTED ) )
                  {
                     lvi.mask = LVIF_STATE;
                     lvi.iItem = lItem;
                     lvi.iSubItem = 0;
                     lvi.stateMask = 0xFFFF;      // get all state flags
                     GetItem( &lvi );

                     lvi.state &= ~LVIS_SELECTED;
                     SetItem( &lvi );
                  }

                  lOffset = -1;
               }

               if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                     zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN,
                                     0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) < 0 )
               {
                  if ( m_chMultiSelect == 0 )
                  {
                     zLONG   lItem;
                     LV_ITEM lvi;

                     for ( lItem = GetNextItem( -1, LVNI_SELECTED );
                           lItem != -1;
                           lItem = GetNextItem( lItem, LVNI_SELECTED ) )
                     {
                        lvi.mask = LVIF_STATE;
                        lvi.iItem = lItem;
                        lvi.iSubItem = 0;
                        lvi.stateMask = 0xFFFF;      // get all state flags
                        GetItem( &lvi );

                        lvi.state &= ~LVIS_SELECTED;
                        SetItem( &lvi );
                     }

                     SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
                  }

                  break;   // nothing to do
               }
            }
            else
            if ( lType == -3 && m_chMultiSelect )   // select or deselect all items
            {
               zLONG   lItem;
               LV_ITEM lvi;

               for ( lItem = GetNextItem( -1, 0 ); // LVNI_SELECTED );
                     lItem != -1;
                     lItem = GetNextItem( lItem, 0 ) ) // LVNI_SELECTED ) )
               {
                  lvi.mask = LVIF_STATE;
                  lvi.iItem = lItem;
                  lvi.iSubItem = 0;
                  lvi.stateMask = 0xFFFF;      // get all state flags
                  GetItem( &lvi );

                  if ( bHilite )
                  {
                     if ( (lvi.state & LVIS_SELECTED) == 0 )
                     {
                        lvi.state |= LVIS_SELECTED;
                        SetItem( &lvi );
                     }
                  }
                  else
                  {
                     if ( lvi.state & LVIS_SELECTED )
                     {
                        lvi.state &= ~LVIS_SELECTED;
                        SetItem( &lvi );
                     }
                  }
               }

               lOffset = -1;
               break;
            }
            else
               break;   // nothing to do

            if ( m_nDisplaySelSet )
               SetSelectSetForView( m_vApp, m_nDisplaySelSet );

            lOffset = GetRelativeEntityNumber( m_vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, m_ulFlag );
         }

         SetSelectSetForView( m_vApp, m_nSelectSelSet );
         if ( m_chMultiSelect )
         {
            if ( lType == -2 )  // multi-select ... potential of multiple selections
            {
               while ( lOffset >= 0 )
               {
                  if ( m_nDisplaySelSet )
                  {
                     SetSelectSetForView( m_vApp, m_nDisplaySelSet );
                     if ( GetSelectStateOfEntity( m_vApp, *m_pzsEName ) )
                        HiliteAllSelectedEntities( );

                     SetSelectSetForView( m_vApp, m_nSelectSelSet );
                  }
                  else
                     HiliteAllSelectedEntities( );

                  if ( SetEntityCursor( m_vApp, *m_pzsEName, 0, zPOS_NEXT | zQUAL_SELECTED,
                                        0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) >= 0 )
                  {
                     lOffset = 1;
                  }
                  else
                     lOffset = -1;
               }
            }
            else
            {
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );

               SetSelectStateOfEntity( m_vApp, *m_pzsEName, bHilite );
               HiliteAllSelectedEntities( );
            }
         }
         else
         {
            SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
            if ( bHilite )
            {
               SetSelectStateOfEntity( m_vApp, *m_pzsEName, TRUE );
               HiliteAllSelectedEntities( );
            }
            else
            {
               zLONG   lItem;
               LV_ITEM lvi;

               for ( lItem = GetNextItem( -1, LVNI_SELECTED );
                     lItem != -1;
                     lItem = GetNextItem( lItem, LVNI_SELECTED ) )
               {
                  lvi.mask = LVIF_STATE;
                  lvi.iItem = lItem;
                  lvi.iSubItem = 0;
                  lvi.stateMask = 0xFFFF;      // get all state flags
                  GetItem( &lvi );

                  lvi.state &= ~LVIS_SELECTED;
                  SetItem( &lvi );
               }
            }
         }

      } while ( FALSE );    // end of: purist's goto

      m_bInHilite = FALSE;
      SetSelectSetForView( m_vApp, nPrevSelSet );
//x   if ( bLock )
//x      UnlockWindowUpdate( );  causes repaint of entire desktop
      SetRedraw( TRUE );
   }
}

zLONG
ZListCtrl::SelectAll( )
{
// TraceLine( "ZListCtrl::SelectAll (Multi=%c): %s", m_chMultiSelect, *m_pzsTag );
   if ( m_chMultiSelect == 'Y' )
   {
      zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, m_nSelectSelSet );

      if ( (m_ulFlag & zQUAL_SCOPE_OI) && (m_pzsScope == 0 || **m_pzsScope == 0) )
      {
         SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, TRUE, (zCPCHAR) zSCOPE_OI );
      }
      else
      {
         SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, TRUE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
      }

      SetSelectSetForView( m_vApp, nPrevSelSet );
      HiliteText( TRUE, -3 );
      return( m_lEntityCnt );
   }
   else
      return( 1 );
}

zLONG
ZListCtrl::GetSortOrderForKey( zLONG lKey )
{
   for ( zLONG k = 0; k < m_lEntityCnt; k++ )
   {
      if ( lKey == (zLONG) m_EntityKeyList.GetAt( k ) )
      {
         return( k );
      }
   }

   return( -1 );
}

zLONG
ZListCtrl::HiliteAllSelectedEntities( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineS( "ZListCtrl::HiliteAllSelectedEntities ", *m_pzsTag );
#endif

   zVIEW   vApp;
   LV_ITEM lvi;
   zULONG  ulFlag;
   zSHORT  nDisplaySet;
   zLONG   lFirstSelected = -1;

   CreateViewFromViewForTask( &vApp, m_vApp, m_pZSubtask->m_vDialog );
   SetViewFromView( vApp, m_vApp );
   zSHORT  nPrevSelSet = SetSelectSetForView( vApp, 1 );  // just to get the
                                                          // previous selset
   if ( m_nDisplaySelSet )
   {
      nDisplaySet = m_nDisplaySelSet;
      ulFlag = m_ulFlag | zQUAL_SELECTED;
      SetSelectSetForView( vApp, m_nDisplaySelSet );
   }
   else
   {
      ulFlag = m_ulFlag;
      SetSelectSetForView( vApp, nPrevSelSet );
      nDisplaySet = nPrevSelSet;
   }

   SetSelectSetForView( vApp, m_nSelectSelSet );
   zSHORT nRC = SetEntityCursor( vApp, *m_pzsEName, 0, zPOS_FIRST | ulFlag, 0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
   lvi.iSubItem = 0;

   while ( nRC >= zCURSOR_SET )
   {
      SetSelectSetForView( vApp, nDisplaySet );
      if ( GetSelectStateOfEntity( vApp, *m_pzsEName ) )
      {
         // Get item data.
         lvi.mask = LVIF_STATE;
         if ( m_bSortDisplay )
         {
         // DisplayEntityInstance( vApp, *m_pzsEName );
            zULONG ulKey = GetEntityKey( vApp, *m_pzsEName );
            lvi.iItem = GetSortOrderForKey( ulKey );
         }
         else
         {
            lvi.iItem = GetRelativeEntityNumber( vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, ulFlag );
         }

         if ( lFirstSelected < 0 )
            lFirstSelected = lvi.iItem;

         if ( lvi.iItem >= 0 )
         {
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state |= LVIS_SELECTED;
            SetItem( &lvi );

            Update( lvi.iItem );
         }
      }

      SetSelectSetForView( vApp, m_nSelectSelSet );
      nRC = SetEntityCursor( vApp, *m_pzsEName, 0, zPOS_NEXT | ulFlag, 0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
   }

// SetSelectSetForView( vApp, nPrevSelSet );
   DropView( vApp );
   return( lFirstSelected );
}

#if 0
void
ZListCtrl::ProcessCommand( zUSHORT uNotificationCode )
{
   if ( m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI )
      return;

   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::ProcessCommand notification ", uNotificationCode );
#endif

   // The painter must ensure Entity/Attribute if ViewName???
   if ( m_pchListInfo && m_pzsVName )
   {
      GetViewByName( &vApp, m_csViewName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
      if ( vApp )
      {
         zSHORT nPrevSelSet;

         if ( m_nDisplaySelSet )
            nPrevSelSet = SetSelectSetForView( vApp, m_nDisplaySelSet );
         else
         {
            nPrevSelSet = SetSelectSetForView( vApp, 1 ); // just to get the
            SetSelectSetForView( vApp, nPrevSelSet );     // previous selset
         }

         if ( SetEntityCursor( vApp, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zSPARENT_KEEPCHILDREN,
                               0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) > zCURSOR_UNCHANGED )
         {
            int    iRelIdx; // this must be int for use in API call
            zSHORT nSelCnt;

            nSelCnt = SetSelectSetForView( vApp, m_nSelectSelSet );
            if ( nPrevSelSet == 0 )
               nPrevSelSet = nSelCnt;

            SetAllSelectStatesForEntity( vApp, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
            if ( m_chMultiSelect )
            {
               nSelCnt = GetSelectedCount( );
               if ( nSelCnt == 1 )
                  GetSelItems( 1, &iRelIdx );
            }
            else
            {
               nSelCnt = 1;
               iRelIdx = GetCurSel( );
            }

#ifdef DEBUG_ALL
            TraceLineI( "OnListBoxPick or Enter ============== SelCnt ", nSelCnt );
#endif
            if ( nSelCnt == 1 )
               SelectHilitedEntity( vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, iRelIdx );
            else
            if ( nSelCnt > 1 )
            {
               int *pIntArray = new int[ nSelCnt ]; // this must be int for use in API call

               nSelCnt = GetSelItems( nSelCnt, pIntArray );
               for ( iRelIdx = 0; iRelIdx < nSelCnt; iRelIdx++ )
               {
                  SelectHilitedEntity( vApp, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, pIntArray[ iRelIdx ] );
               }

               delete [] pIntArray;
            }

            //if ( nSelCnt != m_nSelCnt )       // we need to do this all the
            //{                                 // time since the number
               m_nSelCnt = nSelCnt ? 1 : 0;     // selected may not change,
               m_pZSubtask->AutoGrayActions( ); // but AutoGray may.
            //}
         }

         SetSelectSetForView( vApp, nPrevSelSet );
         if ( m_chMultiSelect )
         {
            LRESULT lrRelIdx = GetCaretIndex( );
            if ( lrRelIdx >= 0 )
            {
               SetEntityCursor( vApp, *m_pzsEName, 0, zPOS_FIRST | zPOS_RELATIVE | zSPARENT_KEEPCHILDREN | m_ulFlag,
                                0, 0, 0, lrRelIdx, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
            }
         }
      }
   }
}
#endif

zBOOL
ZListCtrl::HandleQuickKey( zACCEL_DEF ad )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::HandleQuickKey ", *m_pzsTag );
#endif
   if ( ad.uVK == VK_TAB && ad.fAlt == 0 && ad.fCtrl == 0 )
   {
      ZMapAct *pzma = m_pZSubtask->GetFocusCtrlWithinComposite( 0, this, !ad.fShift );
      if ( pzma && pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
      {
//       TraceLineS( "ZListCtrl::HandleQuickKey setting focus to: ",
//                   *(pzma->m_pzsTag) );
         pzma->m_pCtrl->SetFocus( );
      }

      return( TRUE );
   }
   else
   if ( ad.uVK == VK_RETURN && ad.fAlt == 0 && ad.fCtrl == 0 )
   {
      ProcessImmediateEvent( this, zLISTBOX_LButtonEnter );
      return( TRUE );
   }

   return( FALSE );
}

void
ZListCtrl::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnSetFocus ", *m_pzsTag );
#endif
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      Default( );              // pass it along to parent
      ProcessImmediateEvent( this, zLISTBOX_SetFocus );
   }

   // If we are not getting focus from label edit box, repaint items
   // that should change appearance.
   if ( pWndLostFocus == 0 || mIs_hWnd( pWndLostFocus->m_hWnd ) == 0 || pWndLostFocus->GetParent( ) != this )
   {
   // if ( m_bFullRowSel && (GetStyle( ) & LVS_TYPEMASK) == LVS_REPORT )
      {
         int nItem = GetNextItem( -1, LVNI_SELECTED );
      // TraceLineI( "ZListCtrl::OnSetFocus nItem: ", nItem );

         if ( nItem < 0 && m_lEntityCnt )
            nItem = 0;

         if ( nItem >= 0 )
         {
            m_ulMapActFlags |= zLISTBOX_SETFOCUS;

            // Get item data.
            LV_ITEM lvi;

            lvi.mask = LVIF_STATE;
            lvi.iItem = nItem;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state |= LVIS_FOCUSED;
            SetItem( &lvi );
            m_ulMapActFlags &= ~zLISTBOX_SETFOCUS;
         }

         RepaintSelectedItems( );
      }
   }
}

void
ZListCtrl::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnKillFocus", *m_pzsTag );
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );              // pass it along to parent
   ProcessImmediateEvent( this, zLISTBOX_KillFocus );

   // If we are not losing focus to label edit box, repaint items
   // that should change appearance.
   if ( pWndGetFocus == 0 ||
        mIs_hWnd( pWndGetFocus->m_hWnd ) == 0 ||
        pWndGetFocus->GetParent( ) != this )
   {
   // if ( m_bFullRowSel && (GetStyle( ) & LVS_TYPEMASK) == LVS_REPORT )
         RepaintSelectedItems( );
   }
}

void
ZListCtrl::OnEnable( BOOL bEnable )
{
   if ( bEnable == FALSE && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
   {
      EnableWindow( );  // do not disable a readonly listbox ... 2006.08.28
      if ( m_bLabelEdit )
         ModifyStyle( LVS_EDITLABELS, 0L );  // remove style bit ... prevent edit
   }
}

#if 0
void
ZListCtrl::SelectHilitedEntity( zVIEW  vApp,
                               zCPCHAR cpcEName,
                               zCPCHAR cpcScope,
                               zSHORT  nRelIdx )
{
   zSHORT nPrevSelSet;

#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::SelectHilitedEntity nRelIdx ", nRelIdx );
#endif

   if ( m_nDisplaySelSet )
      nPrevSelSet = SetSelectSetForView( vApp, m_nDisplaySelSet );
   else
      nPrevSelSet = SetSelectSetForView( vApp, m_nSelectSelSet );

   if ( SetEntityCursor( vApp, cpcEName, 0, zPOS_FIRST | zPOS_RELATIVE | zSPARENT_KEEPCHILDREN | m_ulFlag,
                         0, 0, 0, nRelIdx, cpcScope, 0 ) >= 0 )
   {
      if ( m_nDisplaySelSet )
         SetSelectSetForView( vApp, m_nSelectSelSet );

      SetSelectStateOfEntity( vApp, cpcEName, TRUE );
   }

   SetSelectSetForView( vApp, nPrevSelSet );
}
#endif

UINT
ZListCtrl::OnGetDlgCode( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnGetDlgCode ", *m_pzsTag );
#endif

   UINT uCode = CListCtrl::OnGetDlgCode( );
   uCode = uCode | DLGC_WANTALLKEYS;
   return( uCode );
}

void
ZListCtrl::OnRButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "ZListCtrl::OnRButtonDown ", pt );
#endif

   CPoint ptScreen( pt );

// m_pZSubtask->m_uPressedFlags = uModKeys | 0x0200;  // add ZDr mouse flag
   m_ulMapActFlags &= ~zLISTBOX_ITEMCHANGE_ERROR;
   CListCtrl::OnRButtonDown( uModKeys, pt );  // let base class do its thing

   ClientToScreen( &ptScreen );
   zULONG ulPoint = zMAKELONG( ptScreen.x, ptScreen.y );

   ProcessImmediateEvent( this, zLISTBOX_RButtonDown, (zPVOID) ulPoint );
// m_pZSubtask->m_uPressedFlags = 0;
   if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) == 0 )
      return;

// m_pZSubtask->m_uMouseStatus = zMOUSE_CAPTURE_RIGHT;
// SetCapture( );
   m_ptMouseDown = pt;
   m_ptMouseUp = m_ptMouseDown;
}

void
ZListCtrl::OnRButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnRButtonUp ", *m_pzsTag );
#endif
   CRect rect;

   CListCtrl::OnRButtonUp( uModKeys, pt );  // let base class do its thing
   ProcessImmediateEvent( this, zLISTBOX_RButtonUp );

   GetClientRect( rect );
   if ( rect.PtInRect( pt ) )
   {
      ProcessImmediateEvent( this, zLISTBOX_RButtonClick );
      if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG_RIGHT) == zMOUSE_DRAG_RIGHT )
         m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_DO_DROP, 0, (zLONG)(ZMapAct *) this );
   }

// if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_RIGHT) == zMOUSE_CAPTURE_RIGHT )
//    ReleaseCapture( );

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
// Default( );              // pass it along to parent
}

void
ZListCtrl::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnRButtonDblClk ", *m_pzsTag );
#endif
   CListCtrl::OnRButtonDblClk( uModKeys, pt );  // let base class do its thing
   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
   ProcessImmediateEvent( this, zLISTBOX_RButtonDblclk );
}

/////////////////////////////////////////////////////////////////////////////
//
// HitTest flags - Variable that receives information about the results of
//    a hit test. It can be one or more of these values:
//
//    LVHT_ABOVE - The position is above the client area of the control.
//
//    LVHT_BELOW - The position is below the client area of the control.
//
//    LVHT_NOWHERE - The position is inside the list view control's client
//       window but is not over a list item.
//
//    LVHT_ONITEMICON - The position is over a list view item's icon.
//
//    LVHT_ONITEMLABEL - The position is over a list view item's text.
//
//    LVHT_ONITEMSTATEICON - The position is over the state image of a
//       list view item.
//
//    LVHT_TOLEFT - The position is to the left of the list view control's
//       client area.
//
//    LVHT_TORIGHT - The position is to the right of the list view control's
//       client area.
//
// You can use the LVHT_ABOVE, LVHT_BELOW, LVHT_TOLEFT, and LVHT_TORIGHT
// values to determine whether to scroll the contents of a list view control.
// Two of these flags can be combined, for example, if the position is above
// and to the left of the client area.
//
// You can test for the LVHT_ONITEM value to determine whether a given
// position is over a list view item. This value is a bitwise-OR operation
// on the LVHT_ONITEMICON, LVHT_ONITEMLABEL, and LVHT_ONITEMSTATEICON values.
//
/////////////////////////////////////////////////////////////////////////////
void
ZListCtrl::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "ListBox2" ) == 0 )
      TracePoint( "ZListCtrl::OnLButtonDown", pt );
#endif
   m_ulMapActFlags &= ~zLISTBOX_ITEMCHANGE_ERROR;
   if ( (m_ulMapActFlags & zLISTBOX_RECURSIVECLICK) )
      return;

   m_ulMapActFlags |= zLISTBOX_RECURSIVECLICK;

#ifdef DEBUG_ALL
   CRect rect;
   int nTop = GetTopIndex( );
   int nCnt = GetCountPerPage( );
   GetItemRect( 0, &rect, LVIR_LABEL );
   TraceRect( "ZListCtrl::OnLButtonDown rect label:", rect );
   GetItemRect( 0, &rect, LVIR_BOUNDS );
   TraceRect( "ZListCtrl::OnLButtonDown rect bounds:", rect );
   TracePoint( "ZListCtrl::OnLButtonDown point:", pt );
#endif

   // Get the index of the item under the mouse (and store current position).
   UINT  uFlags = 0;
   zLONG lRow = HitTest( pt, &uFlags );
#if 0
   if ( GetViewByName( &m_vApp, m_csViewName, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
      if ( MiValidView( m_vAppList ) )
      {
         DropNameForView( m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      }

      m_vAppList = 0;
      GetViewByName( &m_vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
      if ( m_vApp )
         SetNameForView( m_vApp, m_csViewName, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
   }

   if ( MiValidView( m_vAppList ) == 0 ||
        GetViewByName( &m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
      if ( m_vApp )
      {
         CreateViewFromViewForTask( &m_vAppList, m_vApp, m_pZSubtask->m_vDialog );
         if ( m_vAppList )
         {
         // zSHORT nPrevSelSet;

            SetNameForView( m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );

         // if ( m_nDisplaySelSet )
         //    nPrevSelSet = SetSelectSetForView( m_vAppList, m_nDisplaySelSet );
         // else
         //    nPrevSelSet = SetSelectSetForView( m_vAppList, m_nSelectSelSet );

         // if ( m_bSortDisplay )
            {
               zLONG lKey = m_EntityKeyList.GetAt( lRow );
               SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                                (zCPVOID) &lKey, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
            }
         // else
         // {
         //    SetEntityCursor( m_vAppList, *m_pzsEName, 0,
         //                     zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
         //                     0, 0, 0, lRow, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
         // }

         // SetSelectSetForView( m_vAppList, nPrevSelSet );
            m_lEntityNbr = lRow;
         }
      }
      else
         m_vAppList = 0;
   }
#endif
   m_ptMouseDown = pt;
   m_ulMapActFlags |= zLISTBOX_ON_CLICK;

   // This call can cause this control to be destroyed ... we need to check
   // for this condition.
// TraceLineX( "ZListCtrl::OnLButtonDown ON ", m_pZSubtask->m_uMouseStatus );
   CListCtrl::OnLButtonDown( uModKeys, pt );  // let base class do its thing
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   CorrelateZeidonSelectToListSelect( );
   m_ulMapActFlags &= ~zLISTBOX_ON_CLICK;
   SetLastSelectedZeidonEntity( -1 );
// TraceLineX( "ZListCtrl::OnLButtonDown OFF ", m_pZSubtask->m_uMouseStatus );
   m_lSelSrc = lRow; // HitTest( m_ptMouseDown, &uFlags );
//?SetCapture( );    // this is already done by the framework

   m_ptMouseUp = m_ptMouseDown;
   m_pZSubtask->m_uMouseStatus = zMOUSE_CAPTURE_LEFT;

   // If the item is already selected, defer selection processing until
   // mouse button up because this may be the start of a drag operation.
   if ( m_pchDragDrop && GetItemState( m_lSelSrc, LVIS_SELECTED ) )
   {
#ifdef DEBUG_ALL
      TraceLineI( "LButtonDown nSelection selected ", m_lSelSrc );
#endif
      m_pZSubtask->m_uMouseStatus |= zMOUSE_SELECT_PENDING;
   }
   else
   {
#ifdef DEBUG_ALL
      TraceLineI( "LButtonDown nSelection NOT selected ", m_lSelSrc );
#endif
      m_pZSubtask->m_uMouseStatus &= ~zMOUSE_SELECT_PENDING;

      // I don't think this is necessary except in the case in which the item
      // was not already selected, since we do selection processing in the
      // ItemChanged event (DKS 2003.03.03).
      if ( lRow >= 0 && m_chMultiSelect == 'Y' )
      {
         zVIEW  vApp;

         if ( m_pchListInfo && *m_pzsVName &&
              GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
         {
         // if ( m_bSortDisplay )
            {
               zLONG lKey = m_EntityKeyList.GetAt( lRow );
               SetEntityCursor( m_vApp, *m_pzsEName, 0, m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                                (zCPVOID) &lKey, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
            }
         // else
         // {
         //    SetEntityCursor( vApp, *m_pzsEName, 0,
         //                     m_ulFlag | zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
         //                     lRow, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
         // }
         }
      }
   }

// TraceLineX( "ZListCtrl::OnLButtonDown CAPTURE_LEFT? ", m_pZSubtask->m_uMouseStatus );
   ProcessImmediateEvent( this, zLISTBOX_LButtonDown );
   m_ulMapActFlags &= ~zLISTBOX_RECURSIVECLICK;
}

void
ZListCtrl::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "ZListCtrl::OnLButtonUp", pt );
#endif
   CListCtrl::OnLButtonUp( uModKeys, pt );  // let base class do its thing
   ProcessImmediateEvent( this, zLISTBOX_LButtonUp );
   if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_LEFT) == zMOUSE_CAPTURE_LEFT )
   {
   //?ReleaseCapture( );

      // If the mouse has been captured ... check for drop target.
      if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) && (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) == zMOUSE_DRAG )
      {
//       if ( m_chMultiSelect == 0 &&
//            m_pchDragDrop && *m_pchDragDrop == 'B' &&
//            zstrcmp( m_pchDragDrop + (2 * sizeof( zLONG )),
//                     m_pchDragDrop + (2 * sizeof( zLONG )) +
//                    *((zLONG *) (m_pchDragDrop + sizeof( zLONG ))) ) == 0 )

         int nSelSrc = GetNextItem( -1, LVNI_SELECTED );

         if ( m_bAutoDragSort &&
              (m_chMultiSelect == 0 ||
               (m_chMultiSelect && (nSelSrc == -1 || GetNextItem( nSelSrc, LVNI_SELECTED ) == -1))) )
         {
            CRect rect;

            // Are we the target of our own drop?
            GetClientRect( rect );
            if ( rect.PtInRect( pt ) )
            {
               zVIEW vSrc;

               // If we cannot get the view ... quit.
               if ( GetViewByName( &vSrc, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
               {
                  zVIEW vTgt;
#ifdef DEBUG_ALL
                  TraceLineI( "SelSrc = ", nSelSrc );
#endif
                  SetEntityCursor( vSrc, *m_pzsEName, 0, zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                   0, 0, 0, nSelSrc, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
//                SetCursorFirstSelectedEntity( vSrc, *m_pzsEName, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );

                  CreateViewFromViewForTask( &vTgt, vSrc, m_pZSubtask->m_vDialog );
                  UINT uFlags = 0;
                  int  nSelTgt = HitTest( pt, &uFlags );
#ifdef DEBUG_ALL
                  TraceLineI( "SelTgt = ", nSelTgt );
#endif
                  zSHORT nRC;
                  if ( nSelTgt < 0 )
                  {
                     // The position is inside the list view control's
                     // client window, but it is not over a list item,
                     // so set cursor to the last entity.
                     if ( uFlags & LVHT_NOWHERE )
                     {
                        nRC = SetEntityCursor( vTgt, *m_pzsEName, 0, zPOS_LAST | m_ulFlag,
                                               0, 0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
                     }
                  }
                  else
                  {
                     nRC = SetEntityCursor( vTgt, *m_pzsEName, 0, zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                            0, 0, 0, nSelTgt, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
                  }

                  if ( nRC >= zCURSOR_SET )
                  {
                     MoveSubobject( vTgt, *m_pzsEName, vSrc, *m_pzsEName, zPOS_AFTER, zREPOS_NEXT );
                  }

                  DropView( vTgt );

                  zSHORT nPrevSelSet = SetSelectSetForView( vSrc, m_nSelectSelSet );
                  if ( (m_ulFlag & zQUAL_SCOPE_OI) && (m_pzsScope == 0 || *m_pzsScope == (zCPCHAR) 0) )
                  {
                     SetAllSelectStatesForEntity( vSrc, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
                  }
                  else
                  {
                     SetAllSelectStatesForEntity( vSrc, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
                  }
/*
                  SetEntityCursor( vSrc, *m_pzsEName, 0, zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                   0, 0, 0, nSelSrc > nSelTgt ? nSelSrc - 1 : nSelTgt, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
*/
//BL Fix for Auto Drag Sort
                  SetEntityCursor( vSrc, *m_pzsEName, 0, zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                   0, 0, 0, nSelSrc > nSelTgt ? nSelTgt + 1 : nSelTgt, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );

                  if ( nRC >= zCURSOR_SET )
                     SetSelectStateOfEntity( vSrc, *m_pzsEName, TRUE );

                  SetSelectSetForView( vSrc, nPrevSelSet );

                  zCHAR chHilitePosition = m_chHilitePosition;
                  m_chHilitePosition = 'Y';
                  MapFromOI( );
                  m_chHilitePosition = chHilitePosition;
               }
            }
         }
         else
         if ( m_bAutoDragSort && m_chMultiSelect )
         {
            CRect rect;

            // Are we the target of our own drop?
            GetClientRect( rect );
            if ( rect.PtInRect( pt ) )
               MessageBox( "Drag sort disabled for multiple selections" );
         }

         // We need to check the return from this call in order to know if the drop was accepted or not ...
         // it doesn't really matter to the list box.
         m_pZSubtask->m_pZFWnd->SendMessage( zZEIDON_DO_DROP, uModKeys, (LPARAM)(ZMapAct *) this );
         pt.x = m_ptMouseDown.x;
         pt.y = m_ptMouseDown.y;
      }
      else
      if ( m_pZSubtask->m_uMouseStatus & zMOUSE_SELECT_PENDING )
      {
         m_ptMouseUp = pt;
         m_lEntityNbr = HitTest( m_ptMouseUp );
         if ( m_lEntityNbr >= 0 )
         {
            SetSelectSetForView( m_vApp, m_nSelectSelSet );
            if ( (m_ulFlag & zQUAL_SCOPE_OI) && (m_pzsScope == 0 || **m_pzsScope == 0) )
            {
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
            }
            else
            {
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
            }

            SetEntityCursor( m_vAppList, *m_pzsEName, 0, m_ulFlag | zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                             m_lEntityNbr, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );

            // Get item data.
            LV_ITEM lvi;

            lvi.mask = LVIF_STATE;
            lvi.iItem = m_lEntityNbr;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state |= LVIS_SELECTED;
            if ( GetFocus( ) == this )
               lvi.state |= LVIS_FOCUSED;

            SetItem( &lvi );
            EnsureVisible( m_lEntityNbr, TRUE );
            Update( m_lEntityNbr );

            TraceLineI( "Drag HilitePosition: ", m_lEntityNbr );
         }
      }
   }

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
}

void
ZListCtrl::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "ZListCtrl::OnLButtonDblClk", pt );
#endif
   CListCtrl::OnLButtonDblClk( uModKeys, pt );  // let base class do its thing
   if ( HitTest( pt ) < 0 )
      return;

   ProcessImmediateEvent( this, zLISTBOX_LButtonEnter );
}

void
ZListCtrl::OnMouseMove( UINT uModKeys, CPoint pt )
{
#if 0 //def DEBUG_ALL lots of messages
   TracePoint( "ZListCtrl::OnMouseMove: ", pt );
#endif

   if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE) == zMOUSE_CAPTURE )
   {
      CWnd *pWnd = GetCapture( );
      if ( (uModKeys & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) == 0 )
//         (pWnd = GetCapture( )) == 0 || pWnd->m_hWnd != m_hWnd )
      {
         m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
         ReleaseCapture( );
      }
      else
      {
      //?if ( pWnd == 0 )
      //?   SetCapture( );

         m_ptMouseUp = pt;

         if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) != zMOUSE_DRAG )
         {
            // See if the mouse has moved far enough to start a drag operation.
            if ( m_ptMouseUp.x - m_ptMouseDown.x > 4 ||
                 m_ptMouseUp.y - m_ptMouseDown.y > 4 ||
                 m_ptMouseDown.x - m_ptMouseUp.x > 4 ||
                 m_ptMouseDown.y - m_ptMouseUp.y > 4 )
            {
               m_pZSubtask->m_uMouseStatus |= zMOUSE_DRAG;
               m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_BEGIN_DRAG, uModKeys, (LPARAM)(ZMapAct *) this );
            }
         }

         // Tell the parent that a drag operation is continuing.
         if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) == zMOUSE_DRAG )
         {
            m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_QUERY_DROP, uModKeys, (LPARAM)(ZMapAct *) this );
            if ( m_ulMapActFlags & zMAPACT_DRAG_ENABLED )
               return;
         }
      }
   }

// TraceLineS( "ZListCtrl::OnMouseMove", " default" );

   // Inform the frame window that the mouse is over this ctrl.

   LVHITTESTINFO HitTestInfo;
   HitTestInfo.pt = pt;
   HitTestInfo.flags = LVHT_ONITEM | LVHT_ABOVE;
   HitTestInfo.iItem = -1;
   HitTestInfo.iSubItem = -1;

   // Returns the one-based index of the item, or subitem, being tested
   // (if any), or -1 otherwise.
   if ( SubItemHitTest( &HitTestInfo ) >= 0 )
   {
      m_pZSubtask->InformDriverOfFlyBy( this, m_pCol[ HitTestInfo.iSubItem + 1 ].pchMB_Text );
      if ( m_wndListTip.GetSafeHwnd( ) && GetFocus( ) == this )
      {
         CClientDC dc( &m_wndListTip );
         CRect rectCell;
         zCHAR  szText[ LBH_TEXT_LTH ];

         GetSubItemRect( HitTestInfo.iItem, HitTestInfo.iSubItem, LVIR_BOUNDS, rectCell );
         if ( HitTestInfo.iSubItem == 0 )
         {
            CRect rectCell2;
            GetSubItemRect( HitTestInfo.iItem, 1, LVIR_BOUNDS, rectCell2 );
            rectCell.right = rectCell2.left - 1;
         }

         FormatTextAtPosition( szText, HitTestInfo.iItem, HitTestInfo.iSubItem, sizeof( szText ) );
         TraceLineI( "HitTestItem: ", HitTestInfo.iItem );
         TraceLineS( "LBH Text: ", szText );
      // CSize size = dc.GetTextExtent( szText );
         zSHORT nOffset = (zSHORT) dc.GetTextExtent( _T( " " ), 1 ).cx; // * 2;
      // rectCell.right = rectCell.left + size.cx + nOffset * 2;
         m_wndListTip.Show( rectCell, szText, nOffset );
      }
   }
   else
      m_pZSubtask->InformDriverOfFlyBy( this );

   CListCtrl::OnMouseMove( uModKeys, pt );  // let base class do its thing
}

zBOOL
ZListCtrl::BeginDrag( ZDragDrop *pDragDrop )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::BeginDrag", "" );
#endif

   if ( m_chMultiSelect )
   {
      if ( GetSelectedCount( ) > 1 )
         pDragDrop->fCopyMultiple |= 0x10;
      else
         pDragDrop->fCopyMultiple &= ~0x10;
   }

   return( TRUE );
}

zBOOL
ZListCtrl::QueryDrop( ZDragDrop *pDragDrop )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::QueryDrop: ", *m_pzsTag );
   TracePoint( "ZListCtrl::QueryDrop ptDrop:", pDragDrop->ptDrop );
#endif

   if ( ZMapAct::QueryDrop( pDragDrop ) == FALSE &&
        m_bAutoDragSort == FALSE ||
        (m_bAutoDragSort && pDragDrop->pzmaSrc != pDragDrop->pzmaTgt) )  //???
   {
      return( FALSE );
   }

   CRect rect;
   CRect rectItem;

   GetClientRect( rect );
// TraceRect( "ZListCtrl::QueryDrop ClientRect:", rect );
   GetItemRect( 0, rectItem, LVIR_LABEL );
   CSize size( 0, rectItem.bottom - rectItem.top );
// TraceLineI( "ZListCtrl::QueryDrop ItemHeight: ", size.cy );
   if ( pDragDrop->ptDrop.y < rect.top + size.cy + size.cy / 2 )
      SendMessage( WM_VSCROLL, MAKELONG( SB_LINEUP, 0 ), 0L );
   else
   if ( pDragDrop->ptDrop.y > rect.bottom - size.cy / 2 )
      SendMessage( WM_VSCROLL, MAKELONG( SB_LINEDOWN, 0 ), 0L );

// TraceLineX( "ZListCtrl::QueryDrop mouse status: ", m_pZSubtask->m_uMouseStatus );
   if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_LEFT) == zMOUSE_CAPTURE_LEFT )
   {
      // If the mouse has been captured ... check for drop target.
      if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) &&
           (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) == zMOUSE_DRAG )
      {
         return( TRUE );
      }
   }

   return( FALSE );
}

BOOL
ZListCtrl::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN )
   {
   // TraceLine( "ZGrid::PreTranslateMessage KEYDOWN Char: %d Flags: %d", pMsg->wParam, pMsg->lParam );
      // This permits copy/paste clipboard processing to occur.
      if ( mIsCTRL_Pressed( ) &&
           (pMsg->wParam == 'C' || pMsg->wParam == 'V' || pMsg->wParam == 'X' ||
            pMsg->wParam == 'c' || pMsg->wParam == 'v' || pMsg->wParam == 'x') )
      {
         ::TranslateMessage( pMsg );
         ::DispatchMessage( pMsg );
         return( TRUE );                    // DO NOT process further
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

// Move around using the keyboard.
void
ZListCtrl::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLine( "ZListCtrl::OnKeyDown Char: %d (%c) Flags: %d", uChar, (int) uChar, uFlags );
   if ( mIsCTRL_Pressed( ) && uChar != 17 )
   {
      switch ( uChar )
      {
         case VK_INSERT:
         case 'C':
            OnEditCopy( );
            break;

         case 'V':
            OnEditPaste( );
            break;
      }
   }

   if ( mIsSHIFT_Pressed( ) && (uChar == VK_INSERT) )
      OnEditPaste( );

   CListCtrl::OnKeyDown( uChar, uRepCnt, uFlags );
}

void
ZListCtrl::OnChar( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLine( "ZListCtrl::OnChar Char: %d (%c) Flags: %d", uChar, (int) uChar, uFlags );
   CListCtrl::OnChar( uChar, uRepCnt, uFlags );
}

void
ZListCtrl::OnEditCopy( )
{
   COleDataSource *pSource = CopyTextFromListbox( );
   if ( pSource == 0 )
      return;

   pSource->SetClipboard( );
}

void
ZListCtrl::OnEditPaste( )
{
   // Attach a COleDataObject to the clipboard and paste the data to the grid.
   COleDataObject obj;
   ZCellId cellId;

   cellId.m_lRow = m_lEntityCnt;
   cellId.m_nCol = 0;

   if ( obj.AttachClipboard( ) )
      PasteTextToListbox( cellId, &obj );
}

void
ZListCtrl::WriteRow( CSharedFile& sf, zLONG lMinRow, zLONG lMaxRow,
                     int nMinCol, int nMaxCol )
{
   // Get a tab delimited string to copy to cache.
   CString cs;
   zCHAR   szText[ LBH_TEXT_LTH ];
   long lRow;
   int  nCol;

   for ( lRow = lMinRow; lRow <= lMaxRow; lRow++ )
   {
      CString csText;
      cs.Empty( );
      for ( nCol = nMinCol; nCol <= nMaxCol; nCol++ )
      {
         FormatTextAtPosition( szText, lRow - 1, nCol - 1, sizeof( szText ) );
         // if ( !pCell->GetText( lRow, nCol ) )
         //    cs += _T( " " );
         // else
               cs += szText;

         if ( nCol != m_nCols )
            cs += _T( "\t" );
      }

      if ( lRow != lMaxRow )
         cs += _T( "\n" );

      sf.Write( T2A( cs.GetBuffer( 1 ) ), cs.GetLength( ) );
      cs.ReleaseBuffer( );
   }
}

// Copies text from the selected cells to the clipboard.
COleDataSource *
ZListCtrl::CopyTextFromListbox( )
{
   // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE,
   // so we need to convert).
   CSharedFile sf( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT );

   WriteRow( sf, 1, m_lEntityCnt, 1, m_nCols );

   zCHAR c = 0;
   sf.Write( &c, 1 );

   DWORD dwLth = (DWORD) sf.GetLength( );
   HGLOBAL hMem = sf.Detach( );
   if ( hMem == 0 )
      return( 0 );

   hMem = ::GlobalReAlloc( hMem, dwLth, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT );
   if ( hMem == 0 )
      return( 0 );

   // Cache data.
   COleDataSource *pSource = new COleDataSource( );
   pSource->CacheGlobalData( CF_TEXT, hMem );

   return( pSource );
}

// Pastes text from the clipboard to the selected cells.
zBOOL
ZListCtrl::PasteTextToListbox( ZCellId cellId, COleDataObject *pDataObject )
{
   // Get the text from the COleDataObject.
   HGLOBAL hmem = pDataObject->GetGlobalData( CF_TEXT );
   CMemFile sf( (BYTE *) ::GlobalLock( hmem ), ::GlobalSize( hmem ) );

   // CF_TEXT is ANSI text, so we need to allocate a char buffer to hold this.
   LPSTR pchBuffer = new char[ ::GlobalSize( hmem ) ];
   if ( pchBuffer == 0 )
      return( FALSE );

   sf.Read( pchBuffer, ::GlobalSize( hmem ) );
   ::GlobalUnlock( hmem );

   // Now store in generic TCHAR form so we no longer have to deal with
   // ANSI/UNICODE problems.
   CString csText = pchBuffer;
   delete [] pchBuffer;

   // Parse text data and set in cells ...
   csText.LockBuffer( );
   CString csLine = csText;
   zLONG   lLine = 0;
   zLONG   lRowCnt = m_lEntityCnt;
   int     nColCnt = m_nCols;
   zSHORT  nFlag;
// zBOOL   bSkipLine = TRUE;

   zGRID_CELL cell;
   zLONG lRC;

   // Find the end of the first line.
   int nIndex;
   do
   {
      int nCol = 0;
      nIndex = csLine.Find( _T( "\n" ) );

      // Store the remaining chars after the newline.
      CString csNext = (nIndex < 0) ? _T( "" ) : csLine.Mid( nIndex + 1 );

#if 1 // We are not going to paste to the listbox, rather we will create a new
      // entity for each "line" and set the data.  In the end we will refresh
      // the listbox to show the new data.
      CreateEntity( m_vAppList, *m_pzsEName, zPOS_LAST );
//    if ( bSkipLine )
//    {
//       csLine = csNext;
//       bSkipLine = FALSE;
//    }
#endif

      // Remove all chars after the newline.
      if ( nIndex >= 0 )
         csLine = csLine.Left( nIndex );

      int nLineIndex = csLine.FindOneOf( _T( "\t," ) );
      CString csCellText = (nLineIndex >= 0) ? csLine.Left( nLineIndex ) : csLine;

      while ( csLine.IsEmpty( ) == FALSE && nCol < nColCnt )
      {
#if 1
         nCol++;  // increment prior to SetAttribute
         nFlag = (m_pCol[ nCol ].pchContext &&
                  m_pCol[ nCol ].pchContext[ 0 ]) ? 0 : zUSE_DEFAULT_CONTEXT;

         cell.lRow = m_lEntityCnt;
         cell.nCol = 1;

         TraceLine( "ZGrid::PasteTextToListbox BeforePasteCell( %d, %d ) Text: %s", cell.lRow, cell.nCol, csCellText );
         cell.pchText = csCellText.GetBuffer( csCellText.GetLength( ) );

         lRC = ProcessImmediateEvent( this, zLB_BEFOREPASTECELL, &cellId );
         if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
         {
            if ( SetAttributeFromVariable( m_vAppList,
                                           m_pCol[ nCol ].pchEntity,
                                           m_pCol[ nCol ].pchAttrib,
                                           csCellText, zTYPE_STRING,
                                           zstrlen( csCellText ),
                                           m_pCol[ nCol ].pchContext,
                                           nFlag | zVALIDATE ) == 0 )
            {
               TraceLine( "Paste Entity: %s   Attribute: %s   Value: %s",
                                         m_pCol[ nCol ].pchEntity,
                                         m_pCol[ nCol ].pchAttrib,
                                         csCellText );
               SetAttributeFromVariable( m_vAppList,
                                         m_pCol[ nCol ].pchEntity,
                                         m_pCol[ nCol ].pchAttrib,
                                         csCellText, zTYPE_STRING,
                                         zstrlen( csCellText ),
                                         m_pCol[ nCol ].pchContext,
                                         nFlag );
               lRC = ProcessImmediateEvent( this, zLB_AFTERPASTECELL, &cellId );
            }
         }

         csCellText.ReleaseBuffer( );
#else
         // Skip hidden columns.
         int nColVis = cellId.m_nCol + nCol;
         while ( nColVis < nColCnt )
         {
            if ( GetColumnWidth( nColVis ) > 0 )
               break;

            nCol++;
            nColVis++;
         }

         ZCellId TargetCell( nRowVis, nColVis );
         if ( IsValid( TargetCell ) )
         {
            csCellText.TrimLeft( );
            csCellText.TrimRight( );

            ValidateAndModifyCellContents( TargetCell.m_lRow, TargetCell.m_nCol,
                                           csCellText );

            // Make sure cell is not selected to avoid data loss.
            SetItemState( TargetCell.m_lRow, TargetCell.m_nCol,
                          GetItemState( TargetCell.m_lRow,
                                        TargetCell.m_nCol ) & ~GVIS_SELECTED );

            if ( nRowVis > PasteRange.GetMaxRow( ) )
               PasteRange.SetMaxRow( nRowVis );

            if ( nColVis > PasteRange.GetMaxCol( ) )
               PasteRange.SetMaxCol( nColVis );
         }

         nCol++;
#endif

         csLine = (nLineIndex >= 0) ? csLine.Mid( nLineIndex + 1 ) : _T( "" );
         nLineIndex = csLine.FindOneOf( _T( "\t," ) );
         csCellText = (nLineIndex >= 0) ? csLine.Left( nLineIndex ) : csLine;
      }

      csLine = csNext;
      lLine++;

   } while ( nIndex >= 0 );

   csText.UnlockBuffer( );

   MapFromOI( );
   return( TRUE );
}

// ZListCtrl - dtor
ZListCtrl::~ZListCtrl( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::dtor", "" );
#endif

   mDeleteInit( m_pFontNormal );
   mDeleteInit( m_pFontBold );
   mDeleteInit( m_pFontItalic );
   mDeleteInit( m_pFontUnderline );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

   RemoveColorAssociations( );
   for ( zSHORT nCol = 0; nCol <= m_nColsMax; nCol++ )
   {
      mDeleteInitA( m_pCol[ nCol ].pchEntity );
      mDeleteInitA( m_pCol[ nCol ].pchAttrib );
      mDeleteInitA( m_pCol[ nCol ].pchContext );
      mDeleteInitA( m_pCol[ nCol ].pchTag );
      mDeleteInitA( m_pCol[ nCol ].pchText );
      mDeleteInitA( m_pCol[ nCol ].pchData );
      mDeleteInitA( m_pCol[ nCol ].pchMB_Text );
   // mDeleteInitA( m_pCol[ nCol ].pchColorAttribute );
   }

   mDeleteInitA( m_pCol );

   m_EntityKeyList.RemoveAll( );
   mDeleteInitA( m_pchLabelText );
   mDeleteInitA( m_pchListInfo );
// mDeleteInit( m_pzsScope ); // please do not delete m_pzsScope cuz it's
                              // just an alias for m_pzsAName
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// nCol        - The number of the column on which to sort
//               (0 ==> resort using the current column)
// nAscending  - -1 ==> invert current sort order
//                0 ==> retain current sort order
//                1 ==> sort ascending
//                2 ==> sort descending
void
ZListCtrl::SortByColumn( zSHORT nCol, zSHORT nAscending )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::SortByColumn ", *m_pzsTag );
#endif
   if ( nCol <= 0 || nCol > m_nCols )
   {
      nCol = m_nColLastSorted;
      if ( nCol <= 0 && m_nCols > 0 )
         nCol = 1;
   }

   if ( nCol <= 0 )
   {
      m_csSortBuffer = "";
      return;
   }

   if ( nAscending == 1 )
      m_chAscendDescend = 'A';
   else
   if ( nAscending == 2 )
      m_chAscendDescend = 'D';
   else
   if ( nAscending == -1 )
   {
      if ( m_nColLastSorted == nCol )
      {
         if ( m_chAscendDescend == 'A' )
            m_chAscendDescend = 'D';
         else
            m_chAscendDescend = 'A';
      }
      else
         m_chAscendDescend = 'A';
   }

   if ( m_vApp && m_pCol[ nCol ].pchAttrib[ 0 ] )
   {
      if ( m_chAscendDescend == 'D' )
         m_nColLastSorted = -1;
      else
         m_nColLastSorted = nCol;

      zSHORT nLth;
      zSHORT nFind;
      zCHAR szBuffer[ 4096 ];

      strcpy_s( szBuffer, sizeof( szBuffer ), m_pCol[ nCol ].pchEntity );
      nLth = (zSHORT) zstrlen( szBuffer );
      szBuffer[ nLth++ ] = '.';
      strcpy_s( szBuffer + nLth, sizeof( szBuffer ) - nLth, m_pCol[ nCol ].pchAttrib );
      nLth = (zSHORT) zstrlen( szBuffer );
      szBuffer[ nLth++ ] = ' ';
      szBuffer[ nLth ] = 0;

   // "Abteilung.BezOrgText A"                           <== szBuffer
   // "Abteilung.InterneNummer A Abteilung.BezOrgText A" <== m_csSortBuffer
      nFind = 0;
      while ( (nFind = m_csSortBuffer.Find( szBuffer, nFind )) >= 0 )
      {
         if ( nFind == 0 || m_csSortBuffer.GetAt( nFind - 1 ) == ' ' )
            break;

         nFind += nLth;
      }

      if ( nFind >= 0 )
      {
         zPCHAR pch;
         zPCHAR p = m_csSortBuffer.GetBuffer( m_csSortBuffer.GetLength( ) + nLth );
         if ( *(p + nFind + nLth + 1) )
            strcpy_s( p + nFind, zstrlen( p + nFind ), p + nFind + nLth + 2 );
         else
            p[ nFind ] = 0;

         if ( p[ nFind ] == '[' )
         {
            pch = zstrchr( p + nFind + 1, ']' );
            if ( pch )
               strcpy_s( p + nFind, zstrlen( p + nFind ), pch + 1 );
         }

         if ( p[ nFind ] == ' ' )
         {
            pch = p + nFind + 1;
            while ( *pch == ' ' )
               pch++;

            strcpy_s( p + nFind, zstrlen( p + nFind ), pch );
         }

         m_csSortBuffer.ReleaseBuffer( );
      }

      szBuffer[ nLth++ ] = m_chAscendDescend;

      if ( m_pCol[ nCol ].pchContext )
      {
         szBuffer[ nLth++ ] = ' ';
         szBuffer[ nLth++ ] = '[';
         strcpy_s( szBuffer + nLth, sizeof( szBuffer ) - nLth, m_pCol[ nCol ].pchContext );
         nLth = (zSHORT) zstrlen( szBuffer );
         szBuffer[ nLth++ ] = ']';
      }

      szBuffer[ nLth ] = 0;
      CString cs = m_csSortBuffer;
      m_csSortBuffer = szBuffer;
      if ( cs.IsEmpty( ) == FALSE )
      {
         m_csSortBuffer += " ";
         m_csSortBuffer += cs;
      }

      m_SortedHeaderCtrl.SetSortImage( nCol - 1,
                                       m_chAscendDescend == 'A' ?
                                                              TRUE : FALSE );
      if ( m_bSortDisplay )
      {
         zLONG   lItem;
         LV_ITEM lvi;
         zLONG   lKey = 0;

         for ( lItem = GetNextItem( -1, LVNI_SELECTED );
               lItem != -1;
               lItem = GetNextItem( lItem, LVNI_SELECTED ) )
         {
            if ( lKey == 0 )
               lKey = m_EntityKeyList.GetAt( lItem );

            lvi.mask = LVIF_STATE;
            lvi.iItem = lItem;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state &= ~(LVIS_SELECTED| LVIS_FOCUSED);
            SetItem( &lvi );
            Update( lItem );
         }

         SortByDisplayData( );
         if ( lKey && (lvi.iItem = GetSortOrderForKey( lKey )) >= 0 )
         {
            lvi.mask = LVIF_STATE;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state |= LVIS_SELECTED | LVIS_FOCUSED;
            SetItem( &lvi );
         }

         lItem = HiliteAllSelectedEntities( );
         if ( lItem >= 0 )
         {
            EnsureVisible( lItem, FALSE );
            Update( lItem );
         }
      }
      else
      {
         if ( m_bRememberSort == FALSE )
         {
            if ( cs.Compare( m_csSortBuffer ) == 0 )
            {
               cs = ".bubblesort ";
               cs += m_csSortBuffer;
               OrderEntityForView( m_vApp, *m_pzsEName, cs );
            }
            else
               OrderEntityForView( m_vApp, *m_pzsEName, m_csSortBuffer );

            MapFromOI( zMAP_NO_SORT );
         }
         else
            MapFromOI( );
      }

      Invalidate( );
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZListCtrl message handlers
#if 0
typedef struct tagNM_LISTVIEW
{
   NMHDR hdr;
   int   iItem;
   int   iSubItem;
   UINT  uNewState;
   UINT  uOldState;
   UINT  uChanged;
   POINT ptAction;
   LPARAM lParam;
} NM_LISTVIEW;
#endif

void
ZListCtrl::OnColumnClick( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnColumnClick ", *m_pzsTag );
#endif
   NM_LISTVIEW *pListView = (NM_LISTVIEW *) pNMHDR;

#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnColumnClick ",
               m_pCol[ pListView->iSubItem + 1 ].pchTag );
#endif

// DisplayEntityKey( "ZListCtrl::OnColumnClick",
//                   m_vApp, m_vAppList, m_pCol );

   zLONG lRC =
      ProcessImmediateEvent( this, zLISTBOX_LButtonClickPreSort,
                             (zPVOID) m_pCol[ pListView->iSubItem + 1 ].pchTag );
   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      if ( m_bAutoSort )
         SortByColumn( pListView->iSubItem + 1, -1 );

      ProcessImmediateEvent( this, zLISTBOX_LButtonClick,
                             (zPVOID) m_pCol[ pListView->iSubItem + 1 ].pchTag );
   }

// DisplayEntityKey( "ZListCtrl::OnColumnClick2",
//                   m_vApp, m_vAppList, m_pCol );

   *pResult = 0;
}

#if 0
typedef struct tagNMHDR
{
   HWND hwndFrom;
   UINT idFrom;
   UINT code;
} NMHDR;

typedef struct tagLV_DISPINFO
{
   NMHDR   hdr;
   LV_ITEM item;
} LV_DISPINFO;

typedef struct _LV_ITEM
{
   UINT    mask;
   int     iItem;
   int     iSubItem;
   UINT    state;
   UINT    stateMask;
   LPTSTR  pszText;
   int     cchTextMax;
   int     iImage;       // index of the list view item's icon
   LPARAM  lParam;       // 32-bit value to associate with item
} LV_ITEM;
#endif

void
ZListCtrl::OnGetDispInfo( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineS( "ZListCtrl::OnGetDispInfo ", *m_pzsTag );
#endif

   LV_DISPINFO *pDispInfo = (LV_DISPINFO *) pNMHDR;
   if ( pDispInfo->item.mask & LVIF_TEXT )
   {
#if 0
      if ( zstrcmp( "DKS", *m_pzsTag ) == 0 )
      if ( pDispInfo->item.iItem == 1 && pDispInfo->item.iSubItem == 3 )
      {
         TraceLine( "ZListCtrl::OnGetDispInfo for ListBox: %s  Item: %d,%d  Mask %d",
                    *m_pzsTag, pDispInfo->item.iItem,
                    pDispInfo->item.iSubItem, pDispInfo->item.mask );
      // if ( m_bLabelEdit && pDispInfo->item.iSubItem == 0 )
      //    pDispInfo->item.pszText = m_pchLabelText;
      }
#endif

      FormatTextAtPosition( pDispInfo->item.pszText,
                            pDispInfo->item.iItem,
                            pDispInfo->item.iSubItem,
                            pDispInfo->item.cchTextMax );
   }

   *pResult = 0;
}

void
ZListCtrl::OnSetDispInfo( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineS( "ZListCtrl::OnSetDispInfo ", *m_pzsTag );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// To set a minimum column width, we have to override the OnNotify( )
// function.  The code below restricts the columns to a minimum width
// of 80 pixels. The logic can be extended to restrict the width within
// a range or even specify different range for different columns.  To
// extend the functionality, you would need to add member variables to
// the class to track the different settings.
//
BOOL
ZListCtrl::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult )
{
   HD_NOTIFY *pHDN = (HD_NOTIFY *) lParam;

   if ( (pHDN->hdr.code == HDN_ITEMCHANGINGW || pHDN->hdr.code == HDN_ITEMCHANGINGA) )
   {
      if ( m_pCol[ pHDN->iItem ].nMinColWidth )
      {
         if ( pHDN->pitem->cxy < m_pCol[ pHDN->iItem ].nMinColWidth )
         {
            pHDN->pitem->cxy = m_pCol[ pHDN->iItem ].nMinColWidth;
         }

         m_pCol[ pHDN->iItem ].nMinColWidth = 0;
      }

      if ( m_pCol[ pHDN->iItem ].nMaxColWidth )
      {
         if ( pHDN->pitem->cxy > m_pCol[ pHDN->iItem ].nMaxColWidth )
         {
            pHDN->pitem->cxy = m_pCol[ pHDN->iItem ].nMaxColWidth;
         }

         m_pCol[ pHDN->iItem ].nMaxColWidth = 0;
      }

   // *pResult = TRUE;                // disable change
   // return( TRUE );                 // Processed message
   }

   return( CListCtrl::OnNotify( wParam, lParam, pResult ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// This routine auto-sizes columns. This routine allows for a minimum
// column width (so columns don't disappear, or get too small for inplace
// editing) and fits to the larger of the column heading and the actual cell
// values.
//
// If you don't supply a column number, it will resize all columns. If you do
// supply a column numner, it will resize that column only. For example, you
// may only want to resize the first column when you do label editing (or
// another column if you use inplace cell editing or combos). But you might
// want to resize all columns after loading the list control with data.
//
// Notice that the use of SetRedraw to stop flickering. You should use my
// custom version of this to avoid problems with nested calls to
// SetRedraw (and to remove the need for a final call to Invalidate( )).
//
zSHORT
ZListCtrl::GetColumnCount( ) const
{
   // Get the header control
   CHeaderCtrl *pHeader = (CHeaderCtrl *) GetDlgItem( 0 );

   // Return the number of items in it - i.e. the number of columns.
   return( (zSHORT) pHeader->GetItemCount( ) );
}

void
ZListCtrl::AutoSizeColumns( zLONG lCol, zLONG lTgtMinWidth, zLONG lTgtMaxWidth )
{
   // Call this after your list control is filled.
   SetRedraw( FALSE );
   zLONG lMinCol = lCol < 0 ? 0 : lCol;
   zLONG lMaxCol = lCol < 0 ? GetColumnCount( ) - 1 : lCol;
   for ( lCol = lMinCol; lCol <= lMaxCol; lCol++ )
   {
      SetColumnWidth( lCol, LVSCW_AUTOSIZE );
      zLONG lColWidth1 = GetColumnWidth( lCol );
      SetColumnWidth( lCol, LVSCW_AUTOSIZE_USEHEADER );
      zLONG lColWidth2 = GetColumnWidth( lCol );

      if ( lTgtMinWidth > 0 )
      {
         m_pCol[ lCol ].nMinColWidth =
                             (zSHORT) mConvertMapDlgToPixelX( lTgtMinWidth );
      }

      if ( lTgtMaxWidth > 0 )
      {
         m_pCol[ lCol ].nMaxColWidth =
                             (zSHORT) mConvertMapDlgToPixelX( lTgtMaxWidth );
      }

      zLONG lColWidth = min( max( m_pCol[ lCol ].nMinColWidth,
                                  max( lColWidth1, lColWidth2 ) ),
                             m_pCol[ lCol ].nMaxColWidth );
      SetColumnWidth( lCol, lColWidth );
   }

   // RecalcHeaderTips( ); *** uncomment this if you use my header tips method
   SetRedraw( TRUE );
   Invalidate( ); // *** uncomment this if you don't use my SetRedraw function
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Returns TRUE to prevent the change, or FALSE to allow the change.
// typedef struct tagNMLISTVIEW
// {
//     NMHDR   hdr;
//     int     iItem;
//     int     iSubItem;
//     UINT    uNewState;
//     UINT    uOldState;
//     UINT    uChanged;
//     POINT   ptAction;
//     LPARAM  lParam;
// } NMLISTVIEW, FAR *LPNMLISTVIEW;
//
// Contains information about a list view notification message.
// This structure is the same as the NM_LISTVIEW structure but
// has been renamed to fit standard naming conventions:
//     hdr - NMHDR structure that contains information about this
//           notification message.
//     iItem - Identifies the list view item, or -1 if not used.
//     iSubItem - Identifies the subitem, or zero if none.
//     uNewState - New item state. This member is zero for
//                 notification messages that do not use it.
//     uOldState - Old item state. This member is zero for
//                 notification messages that do not use it.
//     uChanged - Set of flags that indicate the item attributes
//                that have changed. This member is zero for
//                notifications that do not use it. Otherwise,
//                it can have the same values as the mask member
//                of the LVITEM structure.
//     ptAction - POINT structure that indicates the location at
//                which the event occurred. This member is undefined
//                for notification messages that do not use it.
//     lParam - Application-defined 32-bit value of the item. This
//              member is undefined for notification messages that
//              do not use it.
//
// #define LVIS_FOCUSED            0x0001
// #define LVIS_SELECTED           0x0002
// #define LVIS_CUT                0x0004
// #define LVIS_DROPHILITED        0x0008
// #define LVIS_ACTIVATING         0x0020
// #define LVIS_OVERLAYMASK        0x0F00
// #define LVIS_STATEIMAGEMASK     0xF000
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// List view item states:
// An item's state value consists of the item's state, an optional
// overlay mask index, and an optional state image mask index.  An
// item's state determines its appearance and functionality. The state
// can be zero or one or more of the following values:
//
// Item State        Description
// ---------------   --------------------------------------------------------
// LVIS_ACTIVATING - Not currently supported.
// LVIS_CUT - The item is marked for a cut-and-paste operation.
// LVIS_DROPHILITED - The item is highlighted as a drag-and-drop target.
// LVIS_FOCUSED - The item has the focus, so it is surrounded by a
// standard focus rectangle. Although more than one item may be
// selected, only one item can have the focus.
// LVIS_SELECTED - The item is selected. The appearance of a selected item
// depends on whether it has the focus and also on the system colors used
// for selection.  You can use the LVIS_OVERLAYMASK mask to isolate the
// one-based index of the overlay image. You can use the LVIS_STATEIMAGEMASK
// mask to isolate the one-based index of the state image.
// LVITEM - Specifies or receives the attributes of a list view item.
// This structure has been updated to support a new mask value
// (LVIF_INDENT) that enables item indenting. This structure supersedes
// the LV_ITEM structure.
//
// typedef struct _LVITEM
// {
//     UINT mask;
//     int iItem;
//     int iSubItem;
//     UINT state;
//     UINT stateMask;
//     LPTSTR pszText;
//     int cchTextMax;
//     int iImage;
//     LPARAM lParam;
// #if (_WIN32_IE >= 0x0300)
//     int iIndent;
// #endif
// } LVITEM, FAR *LPLVITEM;
//
// Members:
//
// mask - Set of flags that specify which members of this structure
// contain data to be set or which members are being requested. This
// member can have one or more of the following
//
// flags set:
// Flag    Description
// ------  ------------------------------------------------------------------
// LVIF_DI_SETITEM - The operating system should store the requested list
//    item information and not ask for it again. This flag is used only
//    with the LVN_GETDISPINFO notification message.
// LVIF_IMAGE - The iImage member is valid or must be filled in.
// LVIF_INDENT - The iIndent member is valid or must be filled in.
// LVIF_NORECOMPUTE - The control will not generate LVN_GETDISPINFO to
//    retrieve text information if it receives an LVM_GETITEM message.
//    Instead, the pszText member will contain LPSTR_TEXTCALLBACK.
// LVIF_PARAM - The lParam member is valid or must be filled in.
// LVIF_STATE - The state member is valid or must be filled in.
// LVIF_TEXT - The pszText member is valid or must be filled in.
//
// iItem - Zero-based index of the item to which this structure refers.
//
// iSubItem - One-based index of the subitem to which this structure refers,
//    or zero if this structure refers to an item rather than a subitem.
//
// state - Indicates the item's state, state image, and overlay image. The
// stateMask member indicates the valid bits of this member.  Bits 0
// through 7 of this member contain the item state flags. This can be
// one or more of the item state values.  Bits 8 through 11 of this
// member specify the one-based overlay image index. Both the
// full-sized icon image list and the small icon image list can have
// overlay images.  The overlay image is superimposed over the item's
// icon image. If these bits are zero, the item has no overlay image.
// To isolate these bits, use the LVIS_OVERLAYMASK mask.  To set the
// overlay image index in this member, you should use the
// INDEXTOOVERLAYMASK macro. The image list's overlay images are set
// with the ImageList_SetOverlayImage function.  Bits 12 through 15 of
// this member specify the state image index. The state image is
// displayed next to an item's icon to indicate an application-defined
// state. If these bits are zero, the item has no state image. To
// isolate these bits, use the LVIS_STATEIMAGEMASK mask. To set the
// state image index, use the INDEXTOSTATEIMAGEMASK macro. The state
// image index specifies the index of the image in the state image list
// that should be drawn. The state image list is specified with the
// LVM_SETIMAGELIST message.
//
// stateMask - Value specifying which bits of the state member will be
// retrieved or modified. For example, setting this member to LVIS_SELECTED
// will cause only the item's selection state to be retrieved.  This member
// allows you to modify one or more item states without having to
// retrieve all of the item states first. For example, setting this
// member to LVIS_SELECTED and state to zero will cause the item's
// selection state to be cleared, but none of the other states will be
// affected.  To retrieve or modify all of the states, set this member
// to (UINT)-1.  You can use the macro ListView_SetItemState both to
// set and to clear bits.
//
// pszText - If the structure specifies item attributes, pszText is the
// address of a null-terminated string containing the item text. If the
// structure is receiving item attributes, this member is the address
// of the buffer that receives the item text.  If the value of pszText
// is LPSTR_TEXTCALLBACK, the item is a callback item. If the callback
// text changes, you must explicitly set pszText to LPSTR_TEXTCALLBACK
// and notify the list view control of the change by sending an
// LVM_SETITEM OR LVM_SETITEMTEXT message.  Do not set pszText to
// LPSTR_TEXTCALLBACK if the list view control has the
// LVS_SORTASCENDING or LVS_SORTDESCENDING style.
//
// cchTextMax - Size of the buffer pointed to by the pszText member if the
// structure is receiving item attributes. This member is ignored if the
// structure specifies item attributes.
//
// iImage - Index of the item's icon in the control's image list. This
// applies to both the large and small image list.  If this member is the
// I_IMAGECALLBACK value, the parent window is responsible for storing
// the index. In this case, the list view control sends the parent an
// LVN_GETDISPINFO notification message to get the index when it needs
// to display the image.
//
// lParam - 32-bit value specific to the item. If you use the LVM_SORTITEMS
// message, the list view control passes this value to the
// application-defined comparison function. You can also use the
// LVM_FINDITEM message to search a list view control for an item with
// a specified lParam value.
//
// iIndent - Version 4.70. Number of image widths to indent the item. A single
// indentation equals the width of an item image. Therefore, the value
// 1 indents the item by the width of one image, the value 2 indents by
// two images, and so on. Note that this field is supported only for
// items. Attempting to set subitem indentation will cause the calling
// function to fail.
//
// Remarks:  The LVITEM structure is used with a number of messages,
// including LVM_GETITEM.
//
/////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_ALL

typedef struct tagZSTATE_INFO
{
   zPCHAR pchName;
   UINT   uState;

} ZSTATE_INFO, FAR *LPZSTATE_INFO;

ZSTATE_INFO g_pState[] =
{
   "LVIS_FOCUSED", 0x0001,
   "LVIS_SELECTED", 0x0002,
   "LVIS_CUT", 0x0004,
   "LVIS_DROPHILITED", 0x0008,
   "LVIS_ACTIVATING", 0x0020,
   "LVIS_OVERLAYMASK", 0x0F00,
   "LVIS_STATEIMAGEMASK", 0xF000,
   "", 0xFFFF,
};

void
fnTextState( UINT uState, zPCHAR pchTextState )
{
   ZSTATE_INFO *pState = g_pState;
   zSHORT nLth;
   zSHORT k = 0;
   zSHORT j = 0;

   pchTextState[ 0 ] = '(';
   _ltoa_s( uState, pchTextState + 1, LBH_TEXT_LTH - 1, 10 );
   nLth = (zSHORT) zstrlen( pchTextState );
   pchTextState[ nLth++ ] = ')';
   if ( uState )
   {
      pchTextState[ nLth++ ] = ':';
      pchTextState[ nLth++ ] = ' ';
   }

   while ( pState->pchName[ 0 ] )
   {
      if ( pState->uState & uState )
      {
         if ( j )
            pchTextState[ nLth++ ] = '+';

         j++;
         strcpy_s( pchTextState + nLth, LBH_TEXT_LTH - nLth, pState->pchName );
         nLth += (zSHORT) zstrlen( pchTextState + nLth );
      }

      pState++;
   }

   pchTextState[ nLth ] = 0;
}

#endif

void
ZListCtrl::OnItemChanging( LPNMHDR pNMHDR, LRESULT *pResult )
{
   if ( m_bInHilite )
   {
      *pResult = 0;
      return;
   }

#if 0  // This code was removed to prevent a hang when a right mouse click,
       // which caused an item change, also caused a MessagePrompt.

   zSHORT k = 0;
   while ( m_ulMapActFlags & zLISTBOX_RECURSIVESELCHANGE )
   {
      if ( k < 10 )
      {
         k++;
         Sleep( 10 );
      }
      else
         return;
   }

#else

   if ( m_ulMapActFlags & zLISTBOX_RECURSIVESELCHANGE )
      return;

#endif

   m_ulMapActFlags |= zLISTBOX_RECURSIVESELCHANGE;
   LPNMLISTVIEW pnmv = (LPNMLISTVIEW) pNMHDR;

#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
   {
      zCHAR szTextState[ LBH_TEXT_LTH ];

      TraceLineS( "ZListCtrl::OnItemChanging ", *m_pzsTag );
      TraceLineI( "  Item    : ", pnmv->iItem );
      TraceLineI( "  LastSel : ", m_lLastSelected );
      fnTextState( pnmv->uNewState, szTextState );
      TraceLineS( "  NewState: ", szTextState );
      fnTextState( pnmv->uOldState, szTextState );
      TraceLineS( "  OldState: ", szTextState );
      TraceLineI( "ZListCtrl::OnItemChanging ItemState: ",
                  GetItemState( pnmv->iItem,
                               LVIS_SELECTED | LVIS_FOCUSED |
                                            LVIS_CUT | LVIS_DROPHILITED ) );
   }
#endif

   if ( (m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI) != 0 ||
        (m_ulMapActFlags & zLISTBOX_ITEMCHANGE_ERROR) != 0 ||
        pnmv->iItem != m_lLastSelected ||
        (pnmv->uOldState & LVIS_SELECTED) == 0 ||
        pnmv->uNewState != 0 ||
        GetItemState( pnmv->iItem, LVIS_SELECTED ) == 0 )
   {
   // TraceLineI( "Early return OldState: ", pnmv->uOldState );
#if 0
      if ( (m_ulMapActFlags & zLISTBOX_ITEMCHANGE_ERROR) != 0 )
      {
         if ( pnmv->iItem != m_lLastSelected &&
              (pnmv->uNewState & LVIS_SELECTED) )
         {
            LV_ITEM lvi;

            lvi.mask = LVIF_STATE;
            lvi.iItem = pnmv->iItem;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );
            lvi.state &= ~LVIS_SELECTED;
            SetItem( &lvi );

            lvi.mask = LVIF_STATE;
            lvi.iItem = m_lLastSelected;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );
            lvi.state |= LVIS_SELECTED;
            SetItem( &lvi );

            Update( pnmv->iItem );
            Update( m_lLastSelected );
         // m_ulMapActFlags &= ~zLISTBOX_ITEMCHANGE_ERROR;
         // TraceLineS( "ZListCtrl::OnItemChanging resetting ERROR: ", "" );
         }

         *pResult = 0;
         return;
      }
#else
      if ( m_ulMapActFlags & zLISTBOX_ITEMCHANGE_ERROR )
      {
         if ( pnmv->iItem == m_lLastSelected &&
              (pnmv->uOldState & LVIS_SELECTED) == 0 )
         {
            m_ulMapActFlags &= ~zLISTBOX_ITEMCHANGE_ERROR;
         // TraceLineS( "ZListCtrl::OnItemChanging resetting ERROR: ", "" );
         }

         *pResult = 1;  // prevent change!
      }
      else
      {
         *pResult = 0;
      }
#endif
      m_ulMapActFlags &= ~zLISTBOX_RECURSIVESELCHANGE;
      return;
   }

// TraceLineS( "ZListCtrl::OnItemChanging ", *m_pzsTag );
   zLONG lRC = ProcessImmediateEvent( this, zLISTBOX_ItemChanging );
   if ( lRC == zNO_APPLICATION_EVENT )
   {
   // TraceLineI( "OnItemChanging returning: ", 0 );
      *pResult = 0;
   }
   else
   {
   // TraceLineI( "OnItemChanging returning RC: ", lRC );
   // if ( lRC == 0 )
      if ( (zSHORT) zLOUSHORT( lRC ) >= 0 )
         *pResult = 0;
      else
      {
         *pResult = 1;  // prevent change!
         m_ulMapActFlags |= zLISTBOX_ITEMCHANGE_ERROR;
         m_ulMapActFlags &= ~zLISTBOX_RECURSIVESELCHANGE;
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// int GetNextItem( int nItem, int nFlags ) const;
//
// Return Value:  The index of the next item if successful, or -1 otherwise.
//
// Parameters:
//
//    nItem - Index of the item to begin the searching with, or -1
//       to find the first item that matches the specified flags.
//       The specified item itself is excluded from the search.
//
//    nFlags - Geometric relation of the requested item to the
//       specified item, and the state of the requested item.
//       The geometric relation can be one of these values:
//
//          LVNI_ABOVE - Searches for an item that is above
//             the specified item.
//          LVNI_ALL - Searches for a subsequent item by
//             index (the default value).
//          LVNI_BELOW - Searches for an item that is below
//             the specified item.
//          LVNI_TOLEFT - Searches for an item to the left of
//             the specified item.
//          LVNI_TORIGHT - Searches for an item to the right
//             of the specified item.
//
// The state can be zero, or it can be one or more of these
// values:
//
//    LVNI_DROPHILITED - The item has the LVIS_DROPHILITED state flag set.
//    LVNI_FOCUSED - The item has the LVIS_FOCUSED state flag set.
//    LVNI_SELECTED - The item has the LVIS_SELECTED state flag set.
//
// If an item does not have all of the specified state flags set,
// the search continues with the next item.
//
// Remarks:  Searches for a list view item that has the specified
//           properties and that bears the specified relationship
//           to a given item.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// typedef struct tagNMLISTVIEW{
//     NMHDR   hdr;
//     int     iItem;
//     int     iSubItem;
//     UINT    uNewState;
//     UINT    uOldState;
//     UINT    uChanged;
//     POINT   ptAction;
//     LPARAM  lParam;
// } NMLISTVIEW, FAR *LPNMLISTVIEW;
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZListCtrl::CorrelateZeidonSelectToListSelect( )
{
   zVIEW vAppList;
   zLONG lItem;

   if ( m_vApp == 0 )
      return;

   CreateViewFromViewForTask( &vAppList, m_vApp,
                              m_pZSubtask->m_vDialog );
   SetViewFromView( vAppList, m_vApp );  // to get SelectSets
   zSHORT nPrevSelSet = SetSelectSetForView( vAppList, m_nSelectSelSet );
   for ( lItem = GetNextItem( -1, LVNI_SELECTED );
         lItem != -1;
         lItem = GetNextItem( lItem, LVNI_SELECTED ) )
   {
   // if ( m_bSortDisplay )
      {
         zLONG lKey = m_EntityKeyList.GetAt( lItem );
         SetEntityCursor( vAppList, *m_pzsEName, 0,
                          m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                          (zCPVOID) &lKey,
                          0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
      }
   // else
   // {
   //    SetEntityCursor( vAppList, *m_pzsEName, 0,
   //                     zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
   //                     0, 0, 0, lItem, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
   // }

      SetSelectStateOfEntity( vAppList, *m_pzsEName, TRUE );
   }

   SetSelectSetForView( vAppList, nPrevSelSet );
   SetViewFromView( m_vApp, vAppList );  // to get SelectSets
   DropView( vAppList );
}

void
ZListCtrl::SetLastSelectedZeidonEntity( zSHORT nPrevSelectSet )
{
// TraceLineI( "OnItemChanged Before: ", pnmv->iItem );
// DisplayEntityInstance( m_vApp, *m_pzsEName );
   zSHORT k;

   if ( m_lLastSelected < 0 )
      return;

   if ( m_nDisplaySelSet )
   {
      k = SetSelectSetForView( m_vApp, m_nDisplaySelSet );
      if ( nPrevSelectSet == -1 )
         nPrevSelectSet = k;
   }

   if ( GetItemState( m_lLastSelected, LVIS_SELECTED ) & LVIS_SELECTED )
   {
   // if ( m_bSortDisplay )
      {
         zLONG lKey = m_EntityKeyList.GetAt( m_lLastSelected );
         SetEntityCursor( m_vApp, *m_pzsEName, 0,
                          m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                          (zCPVOID) &lKey,
                          0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
      }
   // else
   // {
   //    SetEntityCursor( m_vApp, *m_pzsEName, 0,
   //                     zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
   //                     0, 0, 0, m_lLastSelected, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 );
   // }

      DisplayEntityKey( "ZListCtrl::OnItemChanged",
                        m_vApp, m_vAppList, m_pCol );
   }

   k = SetSelectSetForView( m_vApp, m_nSelectSelSet );
   if ( nPrevSelectSet == -1 )
      nPrevSelectSet = k;

   // TraceLineS( "OnItemChanged", "Middle" );
   // DisplayEntityInstance( m_vApp, *m_pzsEName );

   if ( m_chMultiSelect == 0 )
      SetSelectStateOfEntity( m_vApp, *m_pzsEName, TRUE );

   SetSelectSetForView( m_vApp, nPrevSelectSet );
   m_pZSubtask->AutoGrayActions( );
   if ( (m_ulMapActFlags & zLISTBOX_ITEMCHANGE_ERROR) == 0 &&
        (m_ulMapActFlags & zLISTBOX_ON_CLICK) == 0 )
   {
      ProcessImmediateEvent( this, zLISTBOX_LButtonPick );
   }

   // TraceLineS( "OnItemChanged", "After" );
   // DisplayEntityInstance( m_vApp, *m_pzsEName );
}

void
ZListCtrl::OnItemChanged( LPNMHDR pNMHDR, LRESULT *pResult )
{
   *pResult = 0;
   if ( m_bInHilite )
      return;

   NM_LISTVIEW *pnmv = (NM_LISTVIEW *) pNMHDR;
// zLONG lKey;

#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
   {
      zCHAR szTextState[ LBH_TEXT_LTH ];

      TraceLineS( "ZListCtrl::OnItemChanged ", *m_pzsTag );
      TraceLineI( "  Item    : ", pnmv->iItem );
      TraceLineI( "  LastSel : ", m_lLastSelected );
      fnTextState( pnmv->uNewState, szTextState );
      TraceLineS( "  NewState: ", szTextState );
      fnTextState( pnmv->uOldState, szTextState );
      TraceLineS( "  OldState: ", szTextState );
      TraceLineI( "ZListCtrl::OnItemChanged ItemState: ",
                  GetItemState( pnmv->iItem,
                                LVIS_SELECTED | LVIS_FOCUSED |
                                             LVIS_CUT | LVIS_DROPHILITED ) );
   }

   if ( zstrcmp( m_pzsTag->GetString(), "States" ) == 0 && pnmv->iItem == 53 )
      TraceLineI( "  Item    : ", pnmv->iItem );
#endif

   if ( (m_ulMapActFlags & zLISTBOX_SETFOCUS) )
   {
      m_ulMapActFlags &= ~zLISTBOX_RECURSIVESELCHANGE;
      return;
   }

   if ( (m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI) == 0 &&
        MiValidView( m_vApp ) )
   {
      if ( pnmv->iItem < 0 ||
           (pnmv->uNewState == 0 &&
            (pnmv->uOldState == 0 || m_chMultiSelect == FALSE)) )
      {
         m_ulMapActFlags &= ~zLISTBOX_RECURSIVESELCHANGE;
         return;
      }

      zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, m_nSelectSelSet );

      m_lLastSelected = pnmv->iItem;
      if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
           (m_pzsScope == 0 || **m_pzsScope == 0) )
      {
         SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE,
                                      (zCPCHAR) zSCOPE_OI );
      }
      else
      {
         SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE,
                                      m_pzsScope ? *m_pzsScope : (zCPCHAR) 0 );
      }

      if ( m_chMultiSelect )
      {
         if ( (m_ulMapActFlags & zLISTBOX_ON_CLICK) == 0 )
         {
            CorrelateZeidonSelectToListSelect( );
            SetLastSelectedZeidonEntity( nPrevSelSet );
         }
      }
      else
      {
         SetLastSelectedZeidonEntity( nPrevSelSet );
      }

      SetSelectSetForView( m_vApp, nPrevSelSet );
   }
   else
   {
      m_pZSubtask->AutoGrayActions( );
   }

   if ( (m_ulMapActFlags & zLISTBOX_ITEMCHANGE_ERROR) &&
        (pnmv->uNewState & LVIS_SELECTED) &&
        m_lLastSelected == pnmv->iItem )
   {
      m_ulMapActFlags &= ~zLISTBOX_ITEMCHANGE_ERROR;
   // TraceLineS( "ZListCtrl::OnItemChanged resetting ERROR: ", "" );
   }

   m_ulMapActFlags &= ~zLISTBOX_RECURSIVESELCHANGE;
// Default( );  // let base class do its thing
}

void
ZListCtrl::OnBeginDrag( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnBeginDrag", "" );
#endif

   // If not enabled as a drag source, return.
   if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) == 0 )
   {
      // cannot be drag source control
   }

   *pResult = 0;
// Default( );  // let base class do its thing
}

void
ZListCtrl::OnBeginRDrag( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnBeginRDrag", "" );
#endif
   *pResult = 0;
// Default( );  // let base class do its thing
}

void
ZListCtrl::OnItemActivate( LPNMHDR pNMHDR, LRESULT *pResult )
{
   NM_LISTVIEW *pnmv = (NM_LISTVIEW *) pNMHDR;
   *pResult = 0;

#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineI( "ZListCtrl::OnItemActivate", pnmv->iItem );
#endif
}

void
ZListCtrl::OnOdStateChanged( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnOdStateChanged", "" );
#endif

   *pResult = 0;
}

void
ZListCtrl::OnBeginLabelEdit( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::OnBeginLabelEdit TextLimit: ",
               m_pCol[ 1 ].ulLth );
#endif

   m_ulMapActFlags |= zLISTBOX_LABELEDIT;
   if ( m_vApp && m_pCol[ 1 ].pchEntity[ 0 ] && m_pCol[ 1 ].pchAttrib[ 0 ] )
   {
      CEdit *pEdit = GetEditControl( );
   // zULONG ulLth;
      if ( pEdit )
      {
      // GetAttributeDisplayLength( &ulLth, m_vApp, m_pCol[ 1 ].pchEntity,
      //                            m_pCol[ 1 ].pchAttrib,
      //                            m_pCol[ 1 ].pchContext ?
      //                                        m_pCol[ 1 ].pchContext : 0 );
         pEdit->SetLimitText( m_pCol[ 1 ].ulLth );
      }
   }

   *pResult = 0;
// Default( );  // let base class do its thing
}

void
ZListCtrl::OnEndLabelEdit( LPNMHDR pNMHDR, LRESULT *pResult )
{
   LV_DISPINFO *pDispInfo = (LV_DISPINFO *) pNMHDR;
   LV_ITEM     *pItem = &(pDispInfo->item);

#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnEndLabelEdit: ", pItem->pszText );
#endif

   if ( m_ulMapActFlags & zLISTBOX_LABELEDIT )
   {
      if ( m_lEntityNbr >= 0 &&
           m_pCol[ 1 ].pchEntity[ 0 ] && m_pCol[ 1 ].pchAttrib[ 0 ] )
      {
         CEdit *pEdit = GetEditControl( );
         if ( pEdit )
         {
            CString cs;

            pEdit->GetWindowText( cs );
            SetAttributeFromVariable( m_vApp, m_pCol[ 1 ].pchEntity,
                                      m_pCol[ 1 ].pchAttrib, cs,
                                      zTYPE_STRING, 0,
                                      m_pCol[ 1 ].pchContext,
                                      m_pCol[ 1 ].pchContext ?
                                                  0 : zUSE_DEFAULT_CONTEXT );
         }
      }

      m_ulMapActFlags &= ~zLISTBOX_LABELEDIT;
   }

// if ( pItem->pszText )
//    SetItemText( pItem->nItem, pItem->iSubItem, plvItem->pszText );

   *pResult = 0;
// Default( );  // let base class do its thing
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL
ZListCtrl::AddColumn( zCPCHAR cpcItem,
                      zSHORT nItem, zSHORT nSubItem,
                      zSHORT nMask, zSHORT nFmt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::AddColumn: ", cpcItem );
#endif

   LV_COLUMN lvc;
   lvc.mask = nMask;
   lvc.fmt = nFmt;
   lvc.pszText = (zPCHAR) cpcItem;
   lvc.cx = GetStringWidth( lvc.pszText ) + 15;
   if ( nMask & LVCF_SUBITEM )
   {
      if ( nSubItem != -1 )
         lvc.iSubItem = nSubItem;
      else
         lvc.iSubItem = nItem;
   }

   return( InsertColumn( nItem, &lvc ) );
}

zBOOL
ZListCtrl::AddItem( zSHORT  nItem,
                    zSHORT  nSubItem,
                    zCPCHAR cpcItem,
                    zSHORT  nImageIndex )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::AddItem: ", cpcItem );
#endif

   LV_ITEM lvItem;
   lvItem.mask = LVIF_TEXT;
   lvItem.iItem = nItem;
   lvItem.iSubItem = nSubItem;
   lvItem.pszText = (zPCHAR) cpcItem;
   lvItem.cchTextMax = zstrlen( cpcItem ) + 1;
   if ( nImageIndex != -1 )
   {
      lvItem.mask |= LVIF_IMAGE;
      lvItem.iImage |= LVIF_IMAGE;
   }

   if ( nSubItem == 0 )
      return( InsertItem( &lvItem ) );

   return( SetItem( &lvItem ) );
}

void
ZListCtrl::CheckItem( zSHORT nNewCheckedItem )
{
   // Reset if there is checked item.

#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::CheckItem: ", nNewCheckedItem );
#endif

   if ( m_lCheckedItem != -1 )
   {
      if ( m_bStateIcons )
      {
         SetItemState( m_lCheckedItem,
                       INDEXTOSTATEIMAGEMASK( 1 ),
                       LVIS_STATEIMAGEMASK );
      }
      else
      {
         SetItemState( m_lCheckedItem,
                       INDEXTOSTATEIMAGEMASK( 1 ),
                       LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK );
      }

      ::DestroyIcon( m_hSmallIcon );
   // ::DestroyIcon( m_hLargeIcon );
   }

#if 0
   // Check new item and set its icon as the app icon.
   CWnd *pMainWnd = AfxGetMainWnd( );

   if ( m_lCheckedItem == nNewCheckedItem )
   {
      m_lCheckedItem = -1;
      pMainWnd->SetIcon( NULL, FALSE );
      pMainWnd->SetIcon( NULL, TRUE );
   }
   else
   if ( (m_lCheckedItem = nNewCheckedItem) != -1 )
   {
      if ( m_bStateIcons )
      {
         SetItemState( m_lCheckedItem,
                       INDEXTOSTATEIMAGEMASK( 2 ),
                       LVIS_STATEIMAGEMASK );
      }
      else
      {
         SetItemState( m_lCheckedItem,
                       INDEXTOSTATEIMAGEMASK( 2 ) | INDEXTOOVERLAYMASK( 1 ),
                       LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK );
      }

      m_hSmallIcon =
         ListCtrl.GetImageList( LVSIL_SMALL )->ExtractIcon( nNewCheckedItem );
      pMainWnd->SetIcon( m_hSmallIcon,FALSE );
      m_hLargeIcon =
        ListCtrl.GetImageList( LVSIL_NORMAL )->ExtractIcon( nNewCheckedItem );
      pMainWnd->SetIcon( m_hLargeIcon,TRUE );
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZListCtrl helpers

#if 0
zBOOL
ZListCtrl::SetViewType( DWORD dwViewType )
{
   return( ModifyStyle( LVS_TYPEMASK, dwViewType & LVS_TYPEMASK ) );
}

DWORD
ZListCtrl::GetViewType( )
{
   return( GetStyle( ) & LVS_TYPEMASK );
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZListCtrl construction/destruction

zBOOL
ZListCtrl::SetFullRowSel( zBOOL bFullRowSel )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListCtrl::SetFullRowSel: ", bFullRowSel );
#endif

   // No painting during change.
//xLockWindowUpdate( );    causes repaint of entire desktop
   SetRedraw( FALSE );

   m_bFullRowSel = bFullRowSel;

   zBOOL bRet;

   if ( m_bFullRowSel )
      bRet = ModifyStyle( 0L, LVS_OWNERDRAWFIXED );
   else
      bRet = ModifyStyle( LVS_OWNERDRAWFIXED, 0L );

   // repaint window if we are not changing view type
   if ( bRet && (GetStyle( ) & LVS_TYPEMASK) == LVS_REPORT )
      Invalidate( );

   // Repaint changes.
//xUnlockWindowUpdate( );  causes repaint of entire desktop
   SetRedraw( TRUE );
   return( bRet );
}

zBOOL
ZListCtrl::GetFullRowSel( )
{
#ifdef DEBUG_ALL
// TraceLineS( "ZListCtrl::GetFullRowSel ", *m_pzsTag );
#endif
   return( m_bFullRowSel );
}

/////////////////////////////////////////////////////////////////////////////
// ZListCtrl drawing

zBOOL
ZListCtrl::IsItemSelected( zLONG lOffset )
{
   zVIEW  vApp;
   zVIEW  vAppList;
   zBOOL  bSelected = FALSE;

   if ( m_pchListInfo && *m_pzsVName &&
        GetViewByName( &vApp, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      CreateViewFromViewForTask( &vAppList, vApp, m_pZSubtask->m_vDialog );
      SetViewFromView( vAppList, vApp );

      zLONG  lControl;
      zSHORT nPrevSelSet = SetSelectSetForView( vAppList, m_nSelectSelSet );

      lControl = zPOS_FIRST | zPOS_RELATIVE | zSPARENT_KEEPCHILDREN;
      if ( m_nDisplaySelSet )
      {
         lControl |= zQUAL_SELECTED;
         SetSelectSetForView( vAppList, m_nDisplaySelSet );
      }

   // if ( m_bSortDisplay )
      {
         zLONG lKey = m_EntityKeyList.GetAt( lOffset );
         if ( SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                               m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                               (zCPVOID) &lKey,
                               0, 0, 0, m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) >= zCURSOR_SET )
         {
            SetSelectSetForView( vAppList, m_nSelectSelSet );
            if ( GetSelectStateOfEntity( vAppList, *m_pzsEName ) )
               bSelected = TRUE;
         }
      }
   // else
   // if ( SetEntityCursor( vAppList, *m_pzsEName, 0,
   //                       lControl, 0, 0, 0, lOffset,
   //                       m_pzsScope ? *m_pzsScope : (zCPCHAR) 0, 0 ) >= zCURSOR_SET )
   // {
   //    SetSelectSetForView( vAppList, m_nSelectSelSet );
   //    if ( GetSelectStateOfEntity( vAppList, *m_pzsEName ) )
   //       bSelected = TRUE;
   // }

      SetSelectSetForView( vAppList, nPrevSelSet );
      DropView( vAppList );
   }

   return( bSelected );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Offsets for first and other columns.
// #define OFFSET_FIRST   2
// #define OFFSET_OTHER   6

zBOOL
ZListCtrl::PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle, zLONG lFlags,
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
      for ( nCol = 1; nCol <= m_nCols; nCol++ )
      {
         lWidth = GetColumnWidth( nCol - 1 );
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

   m_wndListTip.Hide( );
   return( TRUE );  // this ctrl can be printed!
}

void
fnFormatString( zPCHAR pchString, zCPCHAR cpcFormat )
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
   strcpy_s( pchString, nLth, pchNewString );
   delete [] pchNewString;
}

#define pDC   pZSubtask->m_pZPrintout->m_pDC

zSHORT
ZListCtrl::PrintZPage( zLONG  lPageNbr,
                       zLONG  lPageWidth,
                       zLONG  lPageHeight )
{
   ZSubtask *pZSubtask;
   zCHAR   szText[ LBH_TEXT_LTH ];
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
         m_nMaxPage = (GetItemCount( ) / m_nLinesPerPage) + 1;
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

   zLONG lRowCnt = GetItemCount( );

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

   pZPrintout->RptDrawText( pDC, cs, rectItem, DT_SINGLELINE | DT_VCENTER | DT_LEFT );

   if ( pZPrintout->m_csReportInfo.IsEmpty( ) )
      cs = pZPrintout->m_csReportDefName;
   else
      cs = pZPrintout->m_csReportInfo;

   cs += "   ";
   SysGetDateTime( szText, sizeof( szText ) );
   szText[ 14 ] = 0;
   fnFormatString( szText, "####.##.##  ##:##:##" );
   cs += szText;
   pZPrintout->RptDrawText( pDC, cs, rectItem, DT_SINGLELINE | DT_VCENTER | DT_RIGHT );

   rectItem = rect;
   rectItem.bottom = rectItem.top + sizeNormal.cy;

   CRect rectAllLabels;
   GetItemRect( lFirst, rectAllLabels, LVIR_BOUNDS );

   pDC->SelectObject( m_pFontBold );

   CHeaderCtrl *pHeader = (CHeaderCtrl *) GetDlgItem( 0 );
   static TCHAR buf[ LBH_TEXT_LTH ];
   HD_ITEM hditem;

   zLONG x1 = rect.left, x2 = rect.left;
   for ( nCol = 0; nCol < m_nCols; nCol++ )
   {
      hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_LPARAM | HDI_WIDTH;
      hditem.pszText = buf;
      hditem.cchTextMax = sizeof( buf ) - 1;

      pHeader->GetItem( nCol, &hditem );
      lJustify = DT_LEFT;
      if ( hditem.fmt & HDF_CENTER )
         lJustify |= DT_CENTER;
      else
      if ( hditem.fmt & HDF_RIGHT )
         lJustify |= DT_RIGHT;

      x2 += hditem.cxy * rectItem.Width( ) / rectAllLabels.Width( );

      CRect rectSubItem;

      rectSubItem.top = rectItem.top;
      rectSubItem.bottom = rectItem.bottom;
      rectSubItem.left = x1;
      rectSubItem.right = x2;
      x1 = x2;

      CRect rectTextItem( rectSubItem );
      rectTextItem.left += sizeNormal.cx;
      rectTextItem.right -= sizeNormal.cx;

      CString csBuf( buf );

#if 0
      if ( wndHeader.pImageList && csBuf.Right( 1 ) == "_" )
      {
         zSHORT iIdx = atoi( cs );
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
                                  DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS );
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

   DWORD dw;
   for ( lRow = lFirst; lRow < lLast; lRow++ )
   {
      LV_ITEM lvi;
      lvi.mask = LVIF_IMAGE | LVIF_STATE;
      lvi.iItem = lRow;
      lvi.iSubItem = 0;
      lvi.stateMask = 0xFFFF;      // get all state flags
      GetItem( &lvi );

      if ( bPrintCSV )
         PrintTextToCSV( m_pZSubtask->m_vDialog, 0, 0 );

      dw = GetItemData( lRow );
      x1 = x2 = rect.left;
      for ( nCol = 0; nCol < m_nCols; nCol++ )
      {
         LV_COLUMN lvc;
         lvc.mask = LVCF_FMT | LVCF_WIDTH;
         GetColumn( nCol, &lvc );
         lJustify = DT_LEFT;
         switch ( lvc.fmt & LVCFMT_JUSTIFYMASK )
         {
            case LVCFMT_RIGHT:
               lJustify = DT_RIGHT;
               break;

            case LVCFMT_CENTER:
               lJustify = DT_CENTER;
               break;
         }

         x2 += lvc.cx * rectItem.Width( ) / rectAllLabels.Width( );

      // GetItemTextEx( lRow, nCol, cs, &lData );
         nRC = FormatTextAtPosition( szText, lRow, nCol, sizeof( szText ) );
         if ( bPrintCSV )
            PrintTextToCSV( m_pZSubtask->m_vDialog, szText, nCol + 1 );

         if ( nRC )
            break;

         cs = szText;
         CRect rectSubItem;

         rectSubItem.top = rectItem.top;
         rectSubItem.bottom = rectItem.bottom;
         rectSubItem.left = x1;
         rectSubItem.right = x2;
         x1 = x2;

         CRect rectTextItem( rectSubItem );
         rectTextItem.left += sizeNormal.cx;
         rectTextItem.right -= sizeNormal.cx;

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
      pZPrintout->RptDrawText( pDC, cs, rectItem,
                               DT_SINGLELINE | DT_CENTER | DT_TOP );
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

   // if ( lPageNbr > 1 )
   {
      zCHAR  szPageNbr[ 32 ];
      zSHORT k;

      _ltoa_s( lPageNbr, szPageNbr, sizeof( szPageNbr ), 10 );
      k = (zSHORT) zstrlen( szPageNbr );
      szPageNbr[ k ] = '/';
      _ltoa_s( m_nMaxPage, szPageNbr + k + 1, sizeof( szPageNbr ) - k - 1, 10 );
      rectItem = rect;
      rectItem.bottom += 3 * m_nTextHeightBold;
      rectItem.top = rectItem.bottom;
      rectItem.bottom += m_nTextHeightBold;
      pDC->SelectObject( m_pFontBold );
      pZPrintout->RptDrawText( pDC, szPageNbr, rectItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
   }

   // Restore the pen, font, map mode, and background mode.
   pDC->SelectObject( pOldPen );
   pDC->SelectObject( pFontOld );
   pDC->SetMapMode( nOldMapMode );
   pDC->SetBkMode( nOldBkMode );

   if ( nRC == 0 && lLast < m_EntityKeyList.GetSize( ) &&
        FormatTextAtPosition( szText, lLast, 0, sizeof( szText ) ) == 0 )
   {
      nRC = 1;     // more pages
   }
   else
   {
      nRC = 0;
   }

   return( nRC );
}

void
ZListCtrl::OnMeasureItem( int nIdCtl, LPMEASUREITEMSTRUCT lpMIS )
{
   CListCtrl::OnMeasureItem( nIdCtl, lpMIS );
}

void
ZListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMIS )
{
// TraceLineS( "ZListCtrl::OnCreate SetFontOverride for ListBox: ", *m_pzsTag );
   SetFontOverride( );

// TraceLineS( "ZListCtrl::MeasureItem for ListBox: ", *m_pzsTag );

   TEXTMETRIC tm;
   HDC hDC = ::GetDC( 0 );
   CFont *pFont = GetFont( );
   HFONT hFontOld = (HFONT) SelectObject( hDC, pFont->GetSafeHandle( ) );
// HFONT hFont = (CFont *) GetStockObject( DEFAULT_GUI_FONT );
// HFONT hFont = (HFONT) GetStockObject( DEFAULT_GUI_FONT );
// HFONT hFontOld = (HFONT) SelectObject( hDC, hFont );
   GetTextMetrics( hDC, &tm );
   lpMIS->itemHeight = ((tm.tmHeight + tm.tmExternalLeading) * m_nLinesPerRow) + 1;
   SelectObject( hDC, hFontOld );
   ::ReleaseDC( 0, hDC );
}

#undef pDC

void
ZListCtrl::DrawItemText( CDC *pDC, ZPairedList *pPairedList,
                         CRect& rectItem, CRect& rectLabel, zSHORT nCol,
                         zCPCHAR cpcText, UINT uFormat, zLONG lFlags )
{
// TraceRect( "ZListCtrl::DrawItemText rectItem", rectItem );

   if ( pPairedList )
   {
      CFont    *pFontOld = 0;
      CFont    *pFont = 0;
      COLORREF clrTxSave = CLR_INVALID;
      COLORREF clrBkSave = CLR_INVALID;

      // Try to find a column specific value.
      ZPairedItem *pPairedItem = pPairedList->Find( nCol, 0, 0, 0, 1 );
      if ( pPairedItem )
      {
         ZColorItem *pColorItem = (ZColorItem *) pPairedItem->m_lValue2;

         // Flag - 1=Bold 2=Italic 4=Underline 8=Strikeout
         if ( pColorItem->m_lClrFlags & 0x0000000F )
         {
            LOGFONT lf;

            pFont = new CFont;
            pFontOld = pDC->GetCurrentFont( );
            pFontOld->GetLogFont( &lf );
            if ( pColorItem->m_lClrFlags & 0x00000001 )
               lf.lfWeight = FW_BOLD;
            else
               lf.lfWeight = FW_NORMAL;

            if ( pColorItem->m_lClrFlags & 0x00000002 )
               lf.lfItalic = TRUE;
            else
               lf.lfItalic = FALSE;

            if ( pColorItem->m_lClrFlags & 0x00000004 )
               lf.lfUnderline = TRUE;
            else
               lf.lfUnderline = FALSE;

            if ( pColorItem->m_lClrFlags & 0x00000008 )
               lf.lfStrikeOut = TRUE;
            else
               lf.lfStrikeOut = FALSE;

            if ( pFont->CreateFontIndirect( &lf ) )
               pFontOld = pDC->SelectObject( pFont );
            else
            {
               delete( pFont );
               pFontOld = 0;
            }
         }

         if ( lFlags & 0x00000001 )  // selected
         {
            if ( (pColorItem->m_lClrFlags & 0x40000000) == 0 ) // OK to use colors for selected item
            {
               if ( pColorItem->m_lClrSelText != CLR_INVALID )  // default color
                  clrTxSave = pDC->SetTextColor( pColorItem->m_lClrSelText );

               if ( pColorItem->m_lClrSelBack != CLR_INVALID )  // default color
               {
                  clrBkSave = pDC->SetBkColor( pColorItem->m_lClrSelBack );
                  pDC->FillRect( rectItem, &CBrush( pColorItem->m_lClrSelBack ) );
               }
            }
         }
         else
         {
            if ( pColorItem->m_lClrText != CLR_INVALID )  // default color
               clrTxSave = pDC->SetTextColor( pColorItem->m_lClrText );

            if ( pColorItem->m_lClrBack != CLR_INVALID )  // default color
            {
               clrBkSave = pDC->SetBkColor( pColorItem->m_lClrBack );
               pDC->FillRect( rectItem, &CBrush( pColorItem->m_lClrBack ) );
            }
         }
      }

      pDC->DrawText( cpcText, -1, rectLabel, uFormat );
      if ( pFontOld )
      {
         pDC->SelectObject( pFontOld );
         pFont->DeleteObject( );
         delete( pFont );
      }

      if ( clrBkSave != CLR_INVALID )
         pDC->SetBkColor( clrBkSave );

      if ( clrTxSave != CLR_INVALID )
         pDC->SetTextColor( clrTxSave );
   }
   else
      pDC->DrawText( cpcText, -1, rectLabel, uFormat );
}

void
ZListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( *m_pzsTag, "States" ) == 0 && lpDIS->itemID == 53 )
   {
      TraceLineS( "ZListCtrl::DrawItem ", *m_pzsTag );
   }
#endif
   zBOOL    bFocus = (GetFocus( ) == this);
// TraceLineS( "DrawItem Focus: ", bFocus ? "TRUE" : "FALSE" );
   CDC      *pDC = CDC::FromHandle( lpDIS->hDC );
   CRect    rectItem( lpDIS->rcItem );
   UINT     uiFlags = ILD_TRANSPARENT;
   CImageList *pImageList;
   zLONG    lItem = lpDIS->itemID;
   CFont    *pFontOld = 0;
   CFont    *pFont = 0;
   COLORREF clrTextSave;
   COLORREF clrBackSave;
   COLORREF clrImage = m_clrBkgnd;
   zCHAR    szText[ LBH_TEXT_LTH + 21 ];
   ZPairedList *pPairedList = 0;
   ZColorItem *pColorItem;
   DWORD    dw;

   if ( mIs_hWnd( m_hWnd ) == 0 || lItem >= GetItemCount( ) )
      return;

   szText[ LBH_TEXT_LTH + 1 ] = (char) 0xFE;

   // Same as label offsets in CHeaderCtrl
   zLONG OFFSET_FIRST = pDC->GetTextExtent( _T(" "), 1 ).cx; // * 2;
   zLONG OFFSET_OTHER = OFFSET_FIRST;

   // Get item data.
   LV_ITEM lvi;
   lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
   lvi.iItem = lItem;
   lvi.iSubItem = 0;
   lvi.pszText = szText;
   lvi.cchTextMax = sizeof( szText ) - 22;
   lvi.stateMask = 0xFFFF;      // get all state flags
   GetItem( &lvi );
   if ( szText[ LBH_TEXT_LTH + 1 ] != (char) 0xFE )
      MessageBox( "ZListCtrl::DrawItem", "overflow detected", MB_OK );

   zBOOL bSelected = (bFocus || (GetStyle( ) & LVS_SHOWSELALWAYS)) &&
                     (lvi.state & LVIS_SELECTED);
   bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

#if 0 //def DEBUG_ALL lots of messages
   if ( zstrcmp( *m_pzsTag, "States" ) == 0 &&
        lpDIS->itemID == 53 &&
        bSelected == FALSE )
   {
      TraceLineS( "ZListCtrl::DrawItem ", *m_pzsTag );
   }

// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
   {
      if ( bSelected )
      {
         TraceLineS( "ZListCtrl::DrawItem SELECTED: ", szText );
         if ( IsItemSelected( lItem ) == FALSE )
            TraceLineS( "ZListCtrl::DrawItem Should NOT be SELECTED: ", szText );
      }
      else
      {
         TraceLineS( "ZListCtrl::DrawItem NOT selected: ", szText );
         if ( IsItemSelected( lItem ) )
            TraceLineS( "ZListCtrl::DrawItem Should BE SELECTED: ", szText );
      }
   }
#endif

   // Set colors if item is selected.
   CRect rectAllLabels;
   GetItemRect( lItem, rectAllLabels, LVIR_BOUNDS );

   CRect rectLabel;
   GetItemRect( lItem, rectLabel, LVIR_LABEL );

   rectAllLabels.left = rectLabel.left;
   if ( m_bClientWidthSel && rectAllLabels.right < m_nClientWidth )
      rectAllLabels.right = m_nClientWidth;

// TraceRect( "ZListCtrl::DrawItem rectAllLabels", rectAllLabels );

   dw = GetItemData( lItem );
   pPairedList = (ZPairedList *) ((dw == 0 || dw == (DWORD) -1) ? 0 : dw);
   if ( bSelected )
   {
      if ( pPairedList == 0 )
      {
         clrTextSave = pDC->SetTextColor( ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
         clrBackSave = pDC->SetBkColor( ::GetSysColor( COLOR_HIGHLIGHT ) );
      }
      else
      {
         ZPairedItem *pPairedItem = pPairedList->Find( 0, 0, 0, 0, 1 );
         if ( pPairedItem )  // color full line
         {
            pColorItem = (ZColorItem *) pPairedItem->m_lValue2;

            // Flag - 1=Bold 2=Italic 4=Underline 8=Strikeout
            if ( pColorItem->m_lClrFlags & 0x0000000F )
            {
               LOGFONT lf;

               pFont = new CFont;
               pFontOld = pDC->GetCurrentFont( );
               pFontOld->GetLogFont( &lf );
               if ( pColorItem->m_lClrFlags & 0x00000001 )
                  lf.lfWeight = FW_BOLD;
               else
                  lf.lfWeight = FW_NORMAL;

               if ( pColorItem->m_lClrFlags & 0x00000002 )
                  lf.lfItalic = TRUE;
               else
                  lf.lfItalic = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000004 )
                  lf.lfUnderline = TRUE;
               else
                  lf.lfUnderline = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000008 )
                  lf.lfStrikeOut = TRUE;
               else
                  lf.lfStrikeOut = FALSE;

               if ( pFont->CreateFontIndirect( &lf ) )
                  pFontOld = pDC->SelectObject( pFont );
               else
               {
                  delete( pFont );
                  pFontOld = 0;
               }
            }

            if ( pColorItem->m_lClrText == CLR_INVALID )  // default color
               clrTextSave = pDC->SetTextColor( m_clrText );
            else
               clrTextSave = pDC->SetTextColor( pColorItem->m_lClrText );

            if ( pColorItem->m_lClrBack == CLR_INVALID )  // default color
               clrBackSave = pDC->SetBkColor( m_clrTextBk );
            else
               clrBackSave = pDC->SetBkColor( pColorItem->m_lClrBack );
         }
         else
         {
            clrTextSave = pDC->SetTextColor( m_clrTextSel );
            clrBackSave = pDC->SetBkColor( m_clrTextBkSel );
         }
      }

      pDC->FillRect( rectAllLabels, &CBrush( ::GetSysColor( COLOR_HIGHLIGHT ) ) );
   }
   else
   {
      if ( pPairedList == 0 )
      {
      // if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
      // {
      //    clrTextSave = pDC->SetTextColor( m_clrText );
      //    clrBackSave = pDC->SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
      // }
      // else
         {
            clrTextSave = pDC->SetTextColor( m_clrText );
            clrBackSave = pDC->SetBkColor( m_clrTextBk );
         }
      }
      else
      {
         ZPairedItem *pPairedItem = pPairedList->Find( 0, 0, 0, 0, 1 );
         if ( pPairedItem )  // color full line
         {
            pColorItem = (ZColorItem *) pPairedItem->m_lValue2;

            // Flag - 1=Bold 2=Italic 4=Underline 8=Strikeout
            if ( pColorItem->m_lClrFlags & 0x0000000F )
            {
               LOGFONT lf;

               pFont = new CFont;
               pFontOld = pDC->GetCurrentFont( );
               pFontOld->GetLogFont( &lf );
               if ( pColorItem->m_lClrFlags & 0x00000001 )
                  lf.lfWeight = FW_BOLD;
               else
                  lf.lfWeight = FW_NORMAL;

               if ( pColorItem->m_lClrFlags & 0x00000002 )
                  lf.lfItalic = TRUE;
               else
                  lf.lfItalic = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000004 )
                  lf.lfUnderline = TRUE;
               else
                  lf.lfUnderline = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000008 )
                  lf.lfStrikeOut = TRUE;
               else
                  lf.lfStrikeOut = FALSE;

               if ( pFont->CreateFontIndirect( &lf ) )
                  pFontOld = pDC->SelectObject( pFont );
               else
               {
                  delete( pFont );
                  pFontOld = 0;
               }
            }

            if ( pColorItem->m_lClrText == CLR_INVALID )  // default color
               clrTextSave = pDC->SetTextColor( m_clrText );
            else
               clrTextSave = pDC->SetTextColor( pColorItem->m_lClrText );

            if ( pColorItem->m_lClrBack == CLR_INVALID )  // default color
               clrBackSave = pDC->SetBkColor( m_clrTextBk );
            else
               clrBackSave = pDC->SetBkColor( pColorItem->m_lClrBack );
         }
         else
         {
            clrTextSave = pDC->SetTextColor( m_clrText );
            clrBackSave = pDC->SetBkColor( m_clrTextBk );
         }
      }

   // if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
   //    pDC->FillRect( rectAllLabels, &CBrush( ::GetSysColor( COLOR_BTNFACE ) ) );
   // else
         pDC->FillRect( rectAllLabels, &CBrush( m_clrTextBk ) );
   }

   // Set color and mask for the icon.
   if ( lvi.state & LVIS_CUT )
   {
      clrImage = m_clrBkgnd;
      uiFlags |= ILD_BLEND50;
   }
   else
   if ( bSelected )
   {
      clrImage = ::GetSysColor( COLOR_HIGHLIGHT );
      uiFlags |= ILD_BLEND50;
   }

   // Draw state icon.
   UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
   if ( nStateImageMask )
   {
      int nImage = (nStateImageMask >> 12) - 1;
      pImageList = GetImageList( LVSIL_STATE );
      if ( pImageList )
      {
      // TraceRect( "ZListCtrl::DrawItem ImageList", rectItem );
         pImageList->Draw( pDC, nImage,
                           CPoint( rectItem.left, rectItem.top ),
                           ILD_TRANSPARENT );
      }
   }

   // Draw normal and overlay icon.
   CRect rectIcon;
   GetItemRect( lItem, rectIcon, LVIR_ICON );

   pImageList = GetImageList( LVSIL_SMALL );
   if ( pImageList )
   {
      UINT nOvlImageMask = lvi.state & LVIS_OVERLAYMASK;
      if ( rectItem.left < rectItem.right - 1 )
      {
      // TraceRect( "ZListCtrl::DrawItem rectIcon", rectIcon );
         ImageList_DrawEx( pImageList->m_hImageList, lvi.iImage,
                           pDC->m_hDC, rectIcon.left, rectIcon.top, 16, 16,
                           m_clrBkgnd, clrImage, uiFlags | nOvlImageMask );
      }
   }

   CString cs( szText );
   UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;

   // Draw item label.
   GetItemRect( lItem, rectItem, LVIR_LABEL );
   rectItem.right -= m_nStateImageOffset;
   if ( cs.GetLength( ) == 0 )
      szText[ 0 ] = 0;
   else
      MakeShortString( pDC, szText, m_pCol[ 1 ].ulLth + 1, cs, rectItem.right - rectItem.left, 2 * OFFSET_FIRST );

   if ( zstrcmp( szText, cs.GetString() ) == 0 && m_pCol[ 1 ].nJustify == LVCFMT_RIGHT )
      uFormat |= DT_RIGHT;
   else
      uFormat |= DT_LEFT;

   rectLabel = rectItem;
   rectLabel.left += OFFSET_FIRST;
   rectLabel.right -= OFFSET_FIRST;

   if ( rectItem.right > rectItem.left )
   {
      DrawItemText( pDC, pPairedList, rectItem, rectLabel, 1, szText,
                    uFormat, bSelected ? 1 : 0 );
   }

   // Draw labels for extra columns.
   LV_COLUMN lvc;
   lvc.mask = LVCF_FMT | LVCF_WIDTH;

   for ( int nCol = 1; GetColumn( nCol, &lvc ); nCol++ )
   {
      rectItem.left = rectItem.right;
      rectItem.right += lvc.cx;

   // TraceLineI( "ZListCtrl::DrawItem item# ", nCol );
   // TraceRect( "ZListCtrl::DrawItem rect", rectItem );
      if ( rectItem.right <= rectItem.left )
         continue;

      cs = GetItemText( lItem, nCol );
      if ( cs.GetLength( ) == 0 )
         continue;

      MakeShortString( pDC, szText, m_pCol[ nCol + 1 ].ulLth + 1, cs, rectItem.right - rectItem.left, 2 * OFFSET_OTHER );

      if ( m_nLinesPerRow > 1 )
         uFormat = DT_NOPREFIX | DT_NOCLIP;
      else
         uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;

      if ( zstrcmp( szText, cs.GetString() ) == 0 )
      {
         switch ( lvc.fmt & LVCFMT_JUSTIFYMASK )
         {
            case LVCFMT_RIGHT:
               uFormat |= DT_RIGHT;
               break;

            case LVCFMT_CENTER:
               uFormat |= DT_CENTER;
               break;

            default:
               uFormat |= DT_LEFT;
               break;
         }
      }
      else
         uFormat |= DT_LEFT;

      rectLabel = rectItem;
      rectLabel.left += OFFSET_OTHER;
      rectLabel.right -= OFFSET_OTHER;

   // TraceRect( "ZListCtrl::DrawItem rectLabel", rectLabel );
   // TraceRect( "ZListCtrl::DrawItem rectItem", rectItem );

      DrawItemText( pDC, pPairedList, rectItem, rectLabel, nCol + 1, szText,
                    uFormat, bSelected ? 1 : 0 );
   }

   // Draw focus rectangle if item has focus.
   if ( (lvi.state & LVIS_FOCUSED) && bFocus )
   {
   // TraceRect( "ZListCtrl::DrawItem rectFocus", rectAllLabels );
      pDC->DrawFocusRect( rectAllLabels );
   }

   // Set original font and colors.
   if ( pFontOld )
   {
      pDC->SelectObject( pFontOld );
      pFont->DeleteObject( );
      delete( pFont );
   }

   pDC->SetBkColor( clrBackSave );
   pDC->SetTextColor( clrTextSave );
}

void
ZListCtrl::RepaintSelectedItems( )
{
   CRect rectItem;
   CRect rectLabel;

   // Invalidate focused item so it can repaint properly.
   zLONG lItem = GetNextItem( -1, LVNI_FOCUSED );

#ifdef DEBUG_ALL
   TraceLineI( "RepaintSelectedItems: ", lItem );
#endif

   if ( lItem != -1 )
   {
      GetItemRect( lItem, rectItem, LVIR_BOUNDS );
      GetItemRect( lItem, rectLabel, LVIR_LABEL );
      rectItem.left = rectLabel.left;
      InvalidateRect( &rectItem, FALSE );
   }

   // If selected items should not be preserved, invalidate them.
   if ( (GetStyle( ) & LVS_SHOWSELALWAYS) == 0 )
   {
      for ( lItem = GetNextItem( -1, LVNI_SELECTED );
            lItem != -1;
            lItem = GetNextItem( lItem, LVNI_SELECTED ) )
      {
         GetItemRect( lItem, rectItem, LVIR_BOUNDS );
         GetItemRect( lItem, rectLabel, LVIR_LABEL );
         rectItem.left = rectLabel.left;
         InvalidateRect( &rectItem, FALSE );
      // TraceLineI( "RepaintSelectedItem: ", lItem );
      }
   }

   UpdateWindow( );  // update changes
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZListCtrl message handlers
//
LRESULT
ZListCtrl::OnSetImageList( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::OnSetImageList ", *m_pzsTag );
#endif
   if ( (int) wParam == LVSIL_STATE )
   {
      int cx, cy;

      if ( ::ImageList_GetIconSize( (HIMAGELIST) lParam, &cx, &cy ) )
         m_nStateImageOffset = cx;
      else
         m_nStateImageOffset = 0;
   }

   return( Default( ) );
}

LRESULT
ZListCtrl::OnSetTextColor( WPARAM wParam, LPARAM lParam )
{
   m_clrText = (COLORREF) lParam;
#ifdef DEBUG_ALL
   TraceLineX( "ZListCtrl::OnSetTextColor m_clrBkgnd: ", (zLONG) m_clrText );
#endif
   return( Default( ) );
}

LRESULT
ZListCtrl::OnSetTextBkColor( WPARAM wParam, LPARAM lParam )
{
   m_clrTextBk = (COLORREF) lParam;
#ifdef DEBUG_ALL
   TraceLineX( "ZListCtrl::OnSetTextBkColor m_clrBkgnd: ",
               (zLONG) m_clrTextBk );
#endif
   return( Default( ) );
}

LRESULT
ZListCtrl::OnSetBkColor( WPARAM wParam, LPARAM lParam )
{
   m_clrBkgnd = (COLORREF) lParam;
#ifdef DEBUG_ALL
   TraceLineX( "ZListCtrl::OnSetBkColor m_clrBkgnd: ", (zLONG) m_clrBkgnd );
#endif
   return( Default( ) );
}

void
ZListCtrl::OnSize( UINT uType, int cx, int cy )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( m_pzsTag->GetString(), "TableListBox" ) == 0 )
      TraceLineS( "ZListCtrl::OnSize ", *m_pzsTag );
#endif

   m_nClientWidth = cx;
   CListCtrl::OnSize( uType, cx, cy );
   m_nCntPerPage = GetCountPerPage( );
}

#if 0  // needed if graying listbox when disabled as readonly
BOOL
ZListCtrl::OnEraseBkgnd( CDC *pDC )
{
   if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
   {
      CBrush br( GetSysColor( COLOR_BTNFACE ) );
      CRect rect;
      pDC->GetClipBox( rect );
      CBrush *pOldBrush = pDC->SelectObject( &br );
      pDC->PatBlt( rect.left, rect.top, rect.Width( ), rect.Height( ), PATCOPY );
      pDC->SelectObject( pOldBrush );
   // TraceLineS( "ZListCtrl::OnEraseBkgnd DISABLE READONLY: ", *m_pzsTag );
   }
   else
   {
      CBrush br( GetSysColor( COLOR_WINDOW ) );
      CRect rect;
      pDC->GetClipBox( rect );
      CBrush *pOldBrush = pDC->SelectObject( &br );
      pDC->PatBlt( rect.left, rect.top, rect.Width( ), rect.Height( ), PATCOPY );
      pDC->SelectObject( pOldBrush );
   // TraceLineS( "ZListCtrl::OnEraseBkgnd ENABLED: ", *m_pzsTag );
   }

   return( TRUE );
}

#else

BOOL
ZListCtrl::OnEraseBkgnd( CDC *pDC )
{
   CBrush br( GetSysColor( COLOR_WINDOW ) );
   CRect rect;
   pDC->GetClipBox( rect );
   CBrush *pOldBrush = pDC->SelectObject( &br );
   pDC->PatBlt( rect.left, rect.top, rect.Width( ), rect.Height( ), PATCOPY );
   pDC->SelectObject( pOldBrush );
// TraceLineS( "ZListCtrl::OnEraseBkgnd ENABLED: ", *m_pzsTag );

   return( TRUE );
}
#endif

void
ZListCtrl::OnNcPaint( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "EdtNcPaint", *m_pzsTag ) == 0 )
      TraceLineS( "ZListCtrl::OnNcPaint: ", *m_pzsTag );
#endif

   if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
        mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
   {
   // TraceLineS( "ZListCtrl::OnNcPaint Updating Composite: ",
   //             *m_pzmaComposite->m_pzsTag );
      m_pzmaComposite->m_pCtrl->UpdateWindow( );
   }

   CListCtrl::OnNcPaint( );
}

void
ZListCtrl::OnPaint( )
{
#ifdef DEBUG_ALL // lots of messages
// if ( zstrcmp( "LBAllTransactions", *m_pzsTag ) == 0 )
   {
      CRect rectClient;
      CRect rectWindow;

      TraceLineS( "ZListCtrl::OnPaint Begin: ", *m_pzsTag );
      GetClientRect( rectClient );
      GetWindowRect( rectWindow );
      TraceRect( "ZListCtrl::OnPaint ClientRect: ", rectClient );
      TraceRect( "ZListCtrl::OnPaint WindowRect: ", rectWindow );
      if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
           mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
      {
         CPaintDC dc( m_pzmaComposite->m_pCtrl );
         CRect rectClip;

         dc.GetClipBox( rectClip );

         if ( rectClip.IsRectNull( ) )
         {
         }
         else
         {
            m_pzmaComposite->m_pCtrl->Invalidate( FALSE );
            TraceLineS( "ZListCtrl::OnPaint Composite: ",
                        *m_pzmaComposite->m_pzsTag );
            TraceRect( "ZListCtrl::OnPaint ClipRect: ", rectClip );
            m_pzmaComposite->m_pCtrl->ScreenToClient( rectWindow );
            TraceRect( "ZListCtrl::OnPaint ScreenToClient WindowRect: ", rectWindow );
            m_pzmaComposite->m_pCtrl->InvalidateRect( rectWindow, FALSE );
         }
      }
   }
#endif

   if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
        (m_pWndParent == 0 ||
         m_pWndParent->m_hWnd != m_pzmaComposite->m_pCtrl->m_hWnd) &&
        mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
   {
   // TraceLineS( "ZListCtrl::OnPaint Updating Composite: ",
   //             *m_pzmaComposite->m_pzsTag );
      m_pzmaComposite->m_pCtrl->UpdateWindow( );
   }

   // In full row select mode, we need to extend the clipping region
   // so we can paint a selection all the way to the right.
   if ( m_bClientWidthSel &&
        (GetStyle( ) & LVS_TYPEMASK) == LVS_REPORT &&
        GetFullRowSel( ) )
   {
      CRect rectAllLabels;

      if ( GetItemRect( 0, &rectAllLabels, LVIR_BOUNDS ) &&
           rectAllLabels.right < m_nClientWidth )
      {
      // CRect rectClient;
      // GetClientRect( rectClient );
      // TraceRect( "ZListCtrl::OnPaint ClientRect:", rectClient );

         // Need to call BeginPaint (in CPaintDC ctor)
         // to get correct clipping rectangle.
         CPaintDC dc( this );
         CRect rectClip;

         dc.GetClipBox( rectClip );

         rectClip.left = min( rectAllLabels.right - 1, rectClip.left );
         rectClip.right = m_nClientWidth;

      // TraceRect( "ZListCtrl::OnPaint ClipBox:", rectClip );
         InvalidateRect( &rectClip, FALSE );
      // if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
      //    dc.FillRect( rectClip, &CBrush( ::GetSysColor( COLOR_3DFACE ) ) );

         // EndPaint will be called in CPaintDC dtor
      }
   }

   CListCtrl::OnPaint( );
   DrawSeparatorLines( );

#ifdef DEBUG_ALL
   if ( zstrcmp( "LBAllTransactions", m_pzsTag->GetString() ) == 0 )
      TraceLineS( "ZListCtrl::OnPaint End: ", m_pzsTag->GetString() );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0

LRESULT CALLBACK
SubClassHeaderButton( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineI( "SubClassHeaderButton Msg: ", uMsg );
#endif

   ZListCtrl *pCtrl = (ZListCtrl *) ::GetWindowLong( hWnd, GWL_USERDATA );
   if ( pCtrl )
   {
      if ( uMsg == WM_MOUSEMOVE )
         pCtrl->SendMessage( uMsg, wParam, lParam );

      return( CallWindowProc( pCtrl->m_lpfnHeaderButtonWndProc,
                              hWnd, uMsg, wParam, lParam ) );
   }

   return( 0 );
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0

void
ZListCtrl::OnBeginDrag( LPNMHDR pNMHDR, LRESULT *pResult )
{
   CPoint       ptItem;
   CPoint       ptAction;
   CPoint       ptImage;
   NM_LISTVIEW  *pnmListView = (NM_LISTVIEW *) pNMHDR;

   ((CListCtrlPage *) GetParent( ))->ShowNotification( pNMHDR, pResult );
   ASSERT( m_bDragging == FALSE );
   m_bDragging = TRUE;
   m_iItemDrag = pnmListView->iItem;
   ptAction = pnmListView->ptAction;
   GetItemPosition( m_iItemDrag, &ptItem );  // ptItem is relative to ( 0, 0 )
                                             // and not the view origin
   GetOrigin( &m_ptOrigin );

   ASSERT( m_pimageListDrag == NULL );
   m_pimageListDrag = CreateDragImage( m_iItemDrag, &ptImage );
   m_sizeDelta = ptAction - ptImage;   // difference between cursor pos and image pos
   m_ptHotSpot = ptAction - ptItem + m_ptOrigin;  // calculate hotspot for the cursor
   m_pimageListDrag->DragShowNolock( TRUE );  // lock updates and show drag image
   m_pimageListDrag->SetDragCursorImage( 0, m_ptHotSpot );  // define the hot spot for the new cursor image
   m_pimageListDrag->BeginDrag( 0, CPoint( 0, 0 ) );
   ptAction -= m_sizeDelta;
   m_pimageListDrag->DragEnter( this, ptAction );
   m_pimageListDrag->DragMove( ptAction );  // move image to overlap original icon
//?SetCapture( );
}

void
ZListCtrl::OnButtonUp( CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "ZListCtrl::OnButtonUp", pt );
#endif
   if ( m_bDragging )  // end of the drag operation
   {
      long      lStyle;
      CString   cs;

      lStyle = GetWindowLong( m_hWnd, GWL_STYLE ) & LVS_TYPEMASK;
      m_bDragging = FALSE;

      ASSERT( m_pimageListDrag );
      m_pimageListDrag->DragLeave( this );
      m_pimageListDrag->EndDrag( );
      delete m_pimageListDrag;
      m_pimageListDrag = NULL;

      if ( lStyle == LVS_REPORT && m_nItemDrop != m_nItemDrag )
      {
         cs = GetItemText( m_nItemDrag, 0 );
         SetItemText( m_nItemDrop, 1, cs );  // drop subitem text is dragged main item text
      }

      if ( lStyle == LVS_LIST && m_nItemDrop != m_nItemDrag )  //add ** to the drop item text
      {
         cs = GetItemText( m_nItemDrop, 0 );
         cs += _T( "**" );
         SetItemText( m_nItemDrop, 0, cs );
      }

      if ( lStyle == LVS_ICON || lStyle == LVS_SMALLICON )  // move the icon
      {
         pt -= m_ptHotSpot;  // the icon should be drawn exactly where the image is
         pt += m_ptOrigin;
         SetItemPosition( m_nItemDrag, pt );  // just move the dragged item
      }

   //?::ReleaseCapture( );
   }

   CListCtrl::OnButtonUp( pt );
}

void
ZListCtrl::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "ZListCtrl::OnLButtonDown", pt );
#endif
   UINT uFlags = 0;
   int nHitItem = GetListCtrl( ).HitTest( pt, &uFlags );

   // We need additional checking in owner-draw mode because we only
   // get LVHT_ONITEM.
   zBOOL bHit = FALSE;
   if ( uFlags == LVHT_ONITEM && (GetStyle( ) & LVS_OWNERDRAWFIXED) )
   {
      CRect rect;
      GetListCtrl( ).GetItemRect( nHitItem, rect, LVIR_ICON );

      // Check if hit was on a state icon.
      if ( m_bStateIcons && pt.x < rect.left )
         bHit = TRUE;
   }
   else
   if ( uFlags & LVHT_ONITEMSTATEICON )
      bHit = TRUE;

   if ( bHit )
      CheckItem( nHitItem );
   else
      CListViewEx::OnLButtonDown( uModKeys, pt );
}

void
ZListCtrl::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "ZListCtrl::OnLButtonDblClk", pt );
#endif
   UINT uFlags = 0;
   int nHitItem = GetListCtrl( ).HitTest( pt, &uFlags );

   if ( uFlags & LVHT_ONITEM )
   {
      // Double click works only if we don't have state icons,
      // or if we are in icon or small icon view.
      if ( m_bStateIcons == FALSE || GetViewType( ) == LVS_ICON ||
           GetViewType( ) == LVS_SMALLICON )
      {
         CheckItem( nHitItem );
      }
   }

   CListViewEx::OnLButtonDblClk( uModKeys, pt );
}

void
ZListCtrl::OnMouseMove( UINT nFlags, CPoint pt )
{
#ifdef DEBUG_ALL
// TraceLineS( "ZListCtrl::OnMouseMove ", *m_pzsTag );
#endif
   zLONG     lStyle;
   zSHORT    nItem;
   LV_ITEM   lvitem;

   lStyle = GetWindowLong( m_hWnd, GWL_STYLE );
   lStyle &= LVS_TYPEMASK;  // drag will do different things in list and report mode
   if ( m_bDragging )
   {
      ASSERT( m_pimageListDrag );
      m_pimageListDrag->DragMove( pt - m_sizeDelta );  // move the image
      if ( (nItem = HitTest( pt )) != -1 )
      {
         m_nItemDrop = nItem;
         m_pimageListDrag->DragLeave( this ); // unlock the window and hide drag image
         if ( lStyle == LVS_REPORT || lStyle == LVS_LIST )
         {
            lvitem.nItem = nItem;
            lvitem.iSubItem = 0;
            lvitem.mask = LVIF_STATE;
            lvitem.stateMask = LVIS_DROPHILITED;  // highlight the drop target
            SetItem( &lvitem );
         }

         pt -= m_sizeDelta;
         m_pimageListDrag->DragEnter( this, pt );  // lock updates and show drag image
      }
   }

   CListCtrl::OnMouseMove( nFlags, pt );
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZListCtrl::DrawSeparatorLines( )
{
   if ( m_bSeparatorLines )
   {
      // Draw the lines only for LVS_REPORT mode.
      if ( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT )
      {
         // Get the number of columns.
         CClientDC dc( this );
         zSHORT nColumnCount = GetColumnCount( );

         // The bottom of the header corresponds to the top of the line
         CHeaderCtrl *pHeader = (CHeaderCtrl *) GetDlgItem( 0 );
         CRect rect;

         pHeader->GetClientRect( &rect );
         zLONG lTop = rect.bottom;

         // Now get the client rect so we know the line length and
         // when to stop.
         GetClientRect( &rect );

         // The border of the column is offset by the horz scroll.
         zLONG lBorderX = -GetScrollPos( SB_HORZ );
         for ( zSHORT k = 0; k < nColumnCount; k++ )
         {
            // Get the next border
            lBorderX += GetColumnWidth( k );

            // If next border is outside client area, break out.
            if ( lBorderX >= rect.right )
               break;

            // Draw the line..
            dc.MoveTo( lBorderX - 1, lTop );
            dc.LineTo( lBorderX - 1, rect.bottom );
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZSortedHeaderCtrl, CHeaderCtrl )

BEGIN_MESSAGE_MAP( ZSortedHeaderCtrl, CHeaderCtrl )
   //{{AFX_MSG_MAP( ZSortedHeaderCtrl )
      // NOTE - the ClassWizard will add and remove mapping macros here.
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZSortedHeaderCtrl::ZSortedHeaderCtrl( )
{
   m_nSortCol = -1;
   m_bSortAsc = TRUE;
}

ZSortedHeaderCtrl::~ZSortedHeaderCtrl( )
{
}

zSHORT
ZSortedHeaderCtrl::SetSortImage( zSHORT nCol, zBOOL bAsc )
{
   zSHORT nPrevCol = m_nSortCol;
   if ( mIs_hWnd( m_hWnd ) )
   {
      m_nSortCol = nCol;
      m_bSortAsc = bAsc;

      // Set OwnerDraw for this column.
      HD_ITEM hditem;

      hditem.mask = HDI_FORMAT;
      GetItem( nCol, &hditem );
      hditem.fmt |= HDF_OWNERDRAW;
      SetItem( nCol, &hditem );

      // Invalidate header control so that it gets redrawn.
      Invalidate( );
   }

   return( nPrevCol );
}

void
ZSortedHeaderCtrl::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
   CDC *pDC = CDC::FromHandle( lpDIS->hDC );

   // Get the column rect
   CRect rectLabel( lpDIS->rcItem );

   // Draw the background
    pDC->FillRect( rectLabel, &CBrush( ::GetSysColor( COLOR_3DFACE ) ) );

   // Labels are offset by a certain amount.
   // This offset is related to the width of a space character.
   zSHORT nOffset = (zSHORT) pDC->GetTextExtent( _T( " " ), 1 ).cx * 2;

   // Get the column text and format.
   TCHAR   szBuffer[ 256 ];
   HD_ITEM hditem;

   hditem.mask = HDI_TEXT | HDI_FORMAT;
   hditem.pszText = szBuffer;
   hditem.cchTextMax = sizeof( szBuffer ) - 1;

   GetItem( lpDIS->itemID, &hditem );

   // Determine format for drawing column label.
   UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;

   if ( hditem.fmt & HDF_CENTER )
      uFormat |= DT_CENTER;
   else
   if ( hditem.fmt & HDF_RIGHT )
      uFormat |= DT_RIGHT;
   else
      uFormat |= DT_LEFT;

   // Adjust the rect if the mouse button is pressed on it.
   if ( lpDIS->itemState == ODS_SELECTED )
   {
      rectLabel.left++;
      rectLabel.top += 2;
      rectLabel.right++;
   }

   // Adjust the rect further if sort arrow is to be displayed.
   if ( lpDIS->itemID == (UINT) m_nSortCol )
   {
      rectLabel.right -= 3 * nOffset;
   }

   rectLabel.left += nOffset;
   rectLabel.right -= nOffset;

   // Draw column label.
   pDC->DrawText( szBuffer, -1, rectLabel, uFormat );

   // Draw the sort arrow.
   if ( lpDIS->itemID == (UINT) m_nSortCol )
   {
      CRect rectIcon( lpDIS->rcItem );

      // Set up pens to use for drawing the triangle.
      CPen penLight( PS_SOLID, 1, ::GetSysColor( COLOR_3DHILIGHT ) );
      CPen penShadow( PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW ) );
      CPen *pOldPen = pDC->SelectObject( &penLight );

      if ( m_bSortAsc )
      {
         // Draw triangle pointing upwards.
         pDC->MoveTo( rectIcon.right - 2 * nOffset, nOffset - 1 );
         pDC->LineTo( rectIcon.right - 3 * nOffset /2,
                      rectIcon.bottom - nOffset );
         pDC->LineTo( rectIcon.right - 5 * nOffset / 2 - 2,
                      rectIcon.bottom - nOffset );
         pDC->MoveTo( rectIcon.right - 5 * nOffset / 2 - 1,
                      rectIcon.bottom - nOffset - 1 );

         pDC->SelectObject( &penShadow );
         pDC->LineTo( rectIcon.right - 2 * nOffset, nOffset - 2 );
      }
      else
      {
         // Draw triangle pointing downwards.
         pDC->MoveTo( rectIcon.right - 3 * nOffset / 2, nOffset - 1 );
         pDC->LineTo( rectIcon.right - 2 * nOffset - 1,
                      rectIcon.bottom - nOffset + 1 );
         pDC->MoveTo( rectIcon.right - 2 * nOffset - 1,
                      rectIcon.bottom - nOffset );

         pDC->SelectObject( &penShadow );
         pDC->LineTo( rectIcon.right - 5 * nOffset / 2 - 1, nOffset - 1 );
         pDC->LineTo( rectIcon.right - 3 * nOffset / 2, nOffset - 1 );
      }

      // Restore the pen.
      pDC->SelectObject( pOldPen );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
ListCtrl( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZListCtrl( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=LB_SortByColumn
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SortByColumn
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the current sort column for a listbox
//              control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the listbox control.
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
LB_SortByColumn( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zSHORT  lColumn,
                 zSHORT  nAscending )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->SortByColumn( lColumn, nAscending );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_SortByColumn ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_GetSortOrder
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_GetSortOrder
//                                              12/28/01 Modified: 12/28/01
//
//  PURPOSE:    This operation gets the current sort order for a listbox
//              control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the listbox control.
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
LB_GetSortOrder( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zPCHAR  pchReturnOrder,
                 zSHORT  nMaxReturnLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   pchReturnOrder[ 0 ] = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         if ( pListCtrl->m_csSortBuffer.IsEmpty( ) )
            pListCtrl->GetPreferencesFromIni( );

         strncpy_s( pchReturnOrder, nMaxReturnLth, pListCtrl->m_csSortBuffer, nMaxReturnLth - 1 );
         pchReturnOrder[ nMaxReturnLth ] = 0;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_GetSortOrder ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_SetSortOrder
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SetSortOrder
//                                              12/28/01 Modified: 12/28/01
//
//  PURPOSE:    This operation sets the current sort order for a listbox
//              control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the listbox control.
//              cpcCtrlTag  - The unique name of the control.
//              cpcNewSortOrder - The new sort order string.
//
//  RETURNS:    0 - Sort order set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
LB_SetSortOrder( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zCPCHAR cpcNewSortOrder )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->m_csSortBuffer = cpcNewSortOrder;
         pListCtrl->MapFromOI( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_SetSortOrder ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_SetSelectedPosition
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SetSelectedPosition
//                                              08/03/99 Modified: 08/03/99
//
//  PURPOSE:    This operation moves the currently selected item
//              to the specified display position.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the listbox control.
//              cpcCtrlTag  - The unique name of the control.
//              lDisplayPos - The display position.
//
//  RETURNS:    0 - Display position set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
LB_SetSelectedPosition( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zLONG   lDisplayPos )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->SetSelectedPosition( lDisplayPos );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_SetSelectedPosition ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_SetColorAttributeName
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SetColorAttributeName
//                                              07/12/00 Modified: 07/12/00
//
//  PURPOSE:    This operation sets the attribute name to use to control
//              the color of the list box lines.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the listbox control.
//              cpcCtrlTag  - The unique name of the control.
//              cpcAttributeName - The attribute name used to control color.
//              lColumn     - Column number (0 ==> all columns)
//
//  RETURNS:    0 - Attribute name set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
LB_SetColorAttributeName( zVIEW   vSubtask,
                          zCPCHAR cpcCtrlTag,
                          zCPCHAR cpcAttributeName,
                          zLONG   lColumn )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->SetColorAttribute( cpcAttributeName, (zSHORT) lColumn );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_SetColorAttributeName ",  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_SetColorAssociation
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SetColorAssociation
//                                           2000/07/12 Modified: 2003/06/10
//
//  PURPOSE:    This operation sets the associations between the value of
//              the (color control) attribute and the color to be displayed.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the listbox control.
//              cpcCtrlTag  - The unique name of the control.
//              lColumn     - Column number (0 ==> all columns)
//              cpcAttributeValue - The value of the attribute corresponding
//                                  to the specified color.
//              lClrText    - The RGB color in which text is displayed.
//              lClrBack    - The RGB color in which to display background.
//              lFlags      - 1=Bold 2=Italic 4=Underline 8=Strikeout
//                            0x10000000=numeric test
//                            0x20000000=ignore case
//                            0x40000000=do not use colors for selected items
//                            0x80000000=set selected colors (ignore above flags)
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
//  Note that to set selected colors, the call to this operation must follow
//  a call to set normal (unselected) colors.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
LB_SetColorAssociation( zVIEW   vSubtask,
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
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->SetColorAssociation( lColumn, cpcAttributeValue,
                                         lClrText, lClrBack, lFlags );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_SetColorAssociation ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_RemoveColorAssociations
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_RemoveColorAssociations
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation removes color associations for a list control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the list control.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Color associations removed successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
LB_RemoveColorAssociations( zVIEW   vSubtask,
                            zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->RemoveColorAssociations( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_RemoveColorAssociations ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_ReplaceEntityKey
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_ReplaceEntityKey
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
LB_ReplaceEntityKey( zVIEW   vSubtask,
                     zCPCHAR cpcCtrlTag,
                     zULONG  ulOldKey,
                     zULONG  ulNewKey )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         zLONG  lRowCnt = pListCtrl->m_EntityKeyList.GetSize( );
         zLONG  lRow;
         zULONG ulKey;

         for ( lRow = 0; lRow < lRowCnt; lRow++ )
         {
            ulKey = pListCtrl->m_EntityKeyList.GetAt( lRow );
            if ( ulKey == ulOldKey )
            {
               pListCtrl->m_EntityKeyList.SetAt( lRow, ulNewKey );
               return( 0 );
            }
         }

         return( 1 );
      }

      TraceLineS( "drvr - Invalid control type for LB_ReplaceEntityKey ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_AutoSizeColumns
// Source Module=zdctllbh.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_AutoSizeColumns
//                                           2003/05/20 Modified: 2003/05/20
//
//  PURPOSE:    This operation auto sizes header listbox columns.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the list control.
//              cpcCtrlTag  - The unique name of the control.
//              lCol        - Column number (one-based index) ... 0 ==> all
//                    Dialog Units
//              lTgtMinWidth - Target Minimum width of column (ignored if 0)
//              lTgtMaxWidth - TargetMaximum width of column (ignored if 0)
//
//  RETURNS:    0 - success
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
LB_AutoSizeColumns( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag,
                    zLONG   lCol,
                    zLONG   lTgtMinWidth,
                    zLONG   lTgtMaxWidth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   ASSERT( lCol >= 0 );

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->AutoSizeColumns( lCol - 1, lTgtMinWidth, lTgtMaxWidth );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for LB_ReplaceEntityKey ",
                  cpcCtrlTag );
   }

   return( -1 );
}
